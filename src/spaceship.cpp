// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::SpaceShip
    \brief SpaceShip.

    Spaceship is a spaceobject, that can move in the space and is controlled by
    pilot (player or AI). Can carry weapon and defensive equipment.
*/

#include "spaceship.h"
#include "spaceobjectmanager.h"

namespace SpaceArena {

SpaceShip::SpaceShip(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String shipShemeName, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
					 Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness) :
                     SpaceObject(ID, sm, nxScene, meshName, materialName, name, meshScale, boundingSphereScale, position, direction, orientation, density, toughness) {
    setType(SO_SPACESHIP);
    _capacitorCharge = 0;
    _shieldsCharge = 0;
	_shipShemeName = shipShemeName;
	_longitudinalThrust = 0;
	_transversalThrust = 0;
	_fireMode = FM_MANUAL;
	_currentWeaponSlot = 0;
	_coldTime = 0;
	_power = 0;
	_transversalFactor = 0.5;
};

SpaceShip::~SpaceShip()
{
	// Delete all slots
	if (!_equipmentSlots.empty())
	{
		for (std::vector<EquipmentSlot*>::iterator i = _equipmentSlots.begin(); i != _equipmentSlots.end(); i++)
		{
			delete *i;
		};
	};
	// Delete hold
	if (!_hold.empty())
	{
		for (std::vector<HoldSlot*>::iterator i = _hold.begin(); i != _hold.end(); i++)
		{
			delete *i;
		};
	};
};

void SpaceShip::addSlot(EquipmentSlot &slot)
{
	EquipmentSlot* newSlot = new EquipmentSlot(slot.getType());
	newSlot->setShemePosition(slot.getShemePosition());
	if (slot.getInstalledEquipment() != NULL)
		newSlot->installEquipment(slot.getInstalledEquipment()->getID());
	_equipmentSlots.push_back(newSlot);
};

void SpaceShip::addSlot(EQUIPMENT_TYPE type, Ogre::Vector3 shemePosition, Ogre::String equipmentID)
{
	EquipmentSlot* slot = new EquipmentSlot(type);
	slot->setShemePosition(shemePosition);
	slot->installEquipment(equipmentID);
	_equipmentSlots.push_back(slot);
};

void SpaceShip::putToTheHold(Ogre::String equipmentID, Ogre::uint amount)
{
	// Check if this equipment is already in the hold
	if (!_hold.empty())
	{
		for (std::vector<HoldSlot*>::iterator i = _hold.begin(); i != _hold.end(); i++)
		{
			// If it is here, increase amount in existing slot
			if ((*i)->getInstalledEquipment()->getID() == equipmentID)
			{
				Ogre::uint cur_amount = (*i)->getAmount();
				cur_amount += amount;
				(*i)->setAmount(cur_amount);
				return;
			};
		};
	};
	// Else create new slot with desired amount
	HoldSlot* tmpSlot = new HoldSlot();
	tmpSlot->setType(equipments[equipmentID]->getType());
	tmpSlot->installEquipment(equipmentID, amount);
	_hold.push_back(tmpSlot);
};

void SpaceShip::setFireMode(FireMode mode)
{
	// Calculate first appropriate fire slot
	if ((mode == FM_MANUAL) || (mode == FM_VOLLEY_ALL) || (mode == FM_ROUNDROBIN) || (mode == FM_VOLLEY_GROUP))
	{
		_fireMode = mode;
		_currentWeaponSlot = 0;
	};
};

Ogre::uint SpaceShip::getSlotNum(Ogre::uint weaponSlotNum)
{
	// Calculate first appropriate weapon slot
	Ogre::uint curAbsSlotNum = 0;
	Ogre::uint curWeaponSlotNum = 0;
	for (std::vector<EquipmentSlot*>::iterator pSlot = _equipmentSlots.begin(); pSlot != _equipmentSlots.end(); pSlot++)
	{
		if ((*pSlot)->getType() == EQ_WEAPON)
		{
			if (weaponSlotNum == curWeaponSlotNum) return curAbsSlotNum;
			curWeaponSlotNum++;
		};
		curAbsSlotNum++;
	};
	return INVALID_WEAPON_SLOT;
};

Ogre::uint SpaceShip::getWeaponGroupSlot(Ogre::uint weaponGroupNum)
{
	// Vector of all weapon ID's, installed on the ship
	std::vector<Ogre::String> weapons;
	Ogre::String weaponID;
	Ogre::uint curWeaponGroup = 0;
	Ogre::uint curSlot = 0;
	bool alreadyGot = false;

	// create vector of weapon groups, installed on ship
	weapons.empty();
	for (std::vector<EquipmentSlot*>::iterator pSlot = _equipmentSlots.begin(); pSlot != _equipmentSlots.end(); pSlot++)
	{
		if ((*pSlot)->getType() == EQ_WEAPON)
		{
			weaponID = (*pSlot)->getInstalledEquipment()->getID();
			for (std::vector<Ogre::String>::iterator i = weapons.begin(); i != weapons.end(); i++)
			{
				if ((*i) == weaponID) alreadyGot = true;
			};
			if (!alreadyGot) { weapons.push_back(weaponID); curWeaponGroup++; }
			if (curWeaponGroup == (weaponGroupNum+1)) return curSlot;
			alreadyGot = false;
		};
		curSlot++;
	};
	return INVALID_WEAPON_SLOT;
};

Ogre::String SpaceShip::getActiveWeaponID()
{
	Ogre::uint slotNum = getSlotNum(_currentWeaponSlot);
	if (slotNum == INVALID_WEAPON_SLOT) return "";
	EquipmentSlot* slotEquip = getEquipmentSlot(slotNum);
	if (slotEquip == NULL) return "";
	Equipment* equip = slotEquip->getInstalledEquipment();
	if (equip == NULL) return "";
	return equip->getID();
};

void SpaceShip::fire()
{
	// We fired recently, colding weapon
	if (_coldTime != 0) return;

	Ogre::String wID = getActiveWeaponID();
	Ogre::String ammoID = static_cast<WeaponEquipment*>(equipments[wID])->getAmmunitionID();
	Ogre::Real projectileThrust = static_cast<AmmunitionEquipment*>(equipments[ammoID])->getThrust();

	// Check if ammunition is available
	HoldSlot* hSlot;
	Ogre::uint curHSlot = 0;
	Equipment* e;
	while (true)
	{
		if ((hSlot = getHoldSlot(curHSlot)) == NULL) return; // no ammo - no fire
		e = hSlot->getInstalledEquipment(); if (e == NULL) return; // no ammo - no fire
		if (e->getType() != EQ_AMMO) { curHSlot++; continue; }; // there is no ammo in current slot, look further
		if (e->getID() != ammoID) { curHSlot++; continue; } // ammo in current slot is for another weapon, look further
		// Okay, there it is, take one
		hSlot->setAmount(hSlot->getAmount() - 1);
		if (hSlot->getAmount() == 0)
    		hSlot->installEquipment("", 0);
		break;
	};
	Ogre::String s = static_cast<WeaponEquipment*>(equipments[wID])->getTemplateName();
	if (s == "") return;
	Ogre::uint ID = _managedBy->getNewID();
	Ogre::Vector3 objectPosition = getPosition();
	objectPosition += (getOrientation() * Ogre::Vector3::UNIT_X) * (_boundingSphereScale + spaceObjectTemplates[s].boundingSphereScale + 1);
	Ogre::Vector3 pos = getOrientation() * Ogre::Vector3::UNIT_X;
	SpaceObject* object = (SpaceObject*) new Projectile(ID, _sceneManager, _nxScene,
					spaceObjectTemplates[s].meshName,
					spaceObjectTemplates[s].materialName,
					"projectile" + Ogre::StringConverter::toString(ID),
					spaceObjectTemplates[s].meshScale,
					spaceObjectTemplates[s].boundingSphereScale,
					objectPosition,
					getOrientation() * Ogre::Vector3::UNIT_X,
                    Ogre::Quaternion(spaceObjectTemplates[s].orientation_x, spaceObjectTemplates[s].orientation_y, spaceObjectTemplates[s].orientation_z, spaceObjectTemplates[s].orientation_w),
                    spaceObjectTemplates[s].density,
					PROJECTILE_TOUGHNESS);
	_managedBy->addObject(object);
	object->setOrientation(this->getOrientation());
	static_cast<Projectile*>(object)->setOwner(this->getID());
	static_cast<Projectile*>(object)->setRespectiveWeapon(*(static_cast<WeaponEquipment*>(equipments[wID])));
	object->applyForce((this->getOrientation() * Ogre::Vector3::UNIT_X) * projectileThrust);
	_coldTime = static_cast<WeaponEquipment*>(equipments[wID])->getFirePeriod();
};

void SpaceShip::applyImpact(Ogre::Real impact)
{
	_shieldsCharge -= impact;
	if (_shieldsCharge < 0)
	{ // If value is negative (all shields exhaused), then decrease ship toughness (add negative value of shields charge)
		setCurToughness(getCurToughness() + _shieldsCharge);
		if (getCurToughness() < 0) setCurToughness(0);
		_shieldsCharge = 0;
	};
};

void SpaceShip::ñontrol(int shipControlFlag, Ogre::Real timeDelta) {
    // Control longitudinal accelerator
    if ((shipControlFlag & (SC_TRACTION_FORWARD | SC_STABILIZE)) == SC_TRACTION_FORWARD)
    { // accelerate forward
		_nxActor->addForce(toNx((getOrientation() * Ogre::Vector3::UNIT_X) * _longitudinalThrust), NX_FORCE);
    };
	if ((shipControlFlag & (SC_TRACTION_BACKWARD | SC_STABILIZE)) == SC_TRACTION_BACKWARD)
    { // accelerate backward
		_nxActor->addForce(-toNx((getOrientation() * Ogre::Vector3::UNIT_X) * _longitudinalThrust), NX_FORCE);
    };
    // Control transversal accelerator (rotator)
    if ((shipControlFlag & (SC_DISPLACE_LEFT | SC_STABILIZE)) == SC_DISPLACE_LEFT)
	{ // displace left
		_nxActor->addForce(toNx(-(getOrientation() * Ogre::Vector3::UNIT_X).crossProduct(Ogre::Vector3::UNIT_Z)*_transversalThrust), NX_FORCE);
    };
    if ((shipControlFlag & (SC_DISPLACE_RIGHT | SC_STABILIZE)) == SC_DISPLACE_RIGHT)
	{ // displace right
		_nxActor->addForce(toNx((getOrientation() * Ogre::Vector3::UNIT_X).crossProduct(Ogre::Vector3::UNIT_Z)*_transversalThrust), NX_FORCE);
    };
    // Automatic ship stabilization (has priority over all other movement control commands,
	// i.e. if stabilization is in action, other commands not processed - look booleans above)
    if ((shipControlFlag & SC_STABILIZE) == SC_STABILIZE) {
		if (timeDelta != 0) {
			NxVec3 stabAccel = -_nxActor->getLinearVelocity() / timeDelta;
			NxVec3 stabThrust = stabAccel * _nxActor->getMass();
			if (stabThrust.magnitude() > getMaxTangentialAcceleration()) {
				stabThrust.normalize();
				stabThrust = stabThrust * _longitudinalThrust;
            };
			_nxActor->addForce(stabThrust);
        };
		
    };

	// Control rotation accelerators
	NxReal turnDirection;
	switch (shipControlFlag & (SC_TURN_LEFT|SC_TURN_RIGHT))
	{
		case SC_TURN_LEFT: turnDirection = 1; break;
		case SC_TURN_RIGHT: turnDirection = -1; break;
		default: turnDirection = 0;
	};
	NxSphereShape* boundingSphere = _nxActor->getShapes()[0]->isSphere();
	if (boundingSphere == NULL)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: SpaceObject bounding shape is unexpectedly NOT A SPHERE!");
		throw 0;
	};
	Ogre::Vector3 shipDirection = getOrientation() * Ogre::Vector3::UNIT_X;
	NxVec3 force = toNx(shipDirection.crossProduct(Ogre::Vector3::UNIT_Z)*_transversalThrust*_transversalFactor);
	NxVec3 position = toNx(shipDirection * boundingSphere->getRadius());
	if (turnDirection != 0)
	{
		_nxActor->addLocalForceAtLocalPos(force, -position * (turnDirection));
		_nxActor->addLocalForceAtLocalPos(-force, position * (turnDirection));
	}
	else
	{
		// If angular velocity greater that zero
		if (_nxActor->getAngularVelocity().magnitude() > EPSILON)
		{
			// Calculate torque for rotation compensation
			NxVec3 stabTorque = - _nxActor->getAngularMomentum() / timeDelta;
			// If required value if greater of maximum possible
			if (stabTorque.magnitude() > _transversalThrust * position.magnitude())
			{
				// then set compensation torque to maximal
				stabTorque.normalize();
				stabTorque = stabTorque * _transversalThrust * position.magnitude();
			};
			// Apply torque to spaceship
			_nxActor->addLocalTorque(stabTorque);
		};
	};
	// Control fire mode
	Ogre::uint mode = shipControlFlag & (SC_MANUAL_1 | SC_MANUAL_2 | SC_MANUAL_3 | SC_MANUAL_4 | SC_MANUAL_5 | SC_MANUAL_6 | SC_MANUAL_7 | SC_MANUAL_8 | SC_MANUAL_9);
	if ( mode != 0)
    { // select manual fire mode with desired weapon slot
		Ogre::uint wSlot;
		switch (mode)
		{
			case SC_MANUAL_1: wSlot = 0; break;
			case SC_MANUAL_2: wSlot = 1; break;
			case SC_MANUAL_3: wSlot = 2; break;
			case SC_MANUAL_4: wSlot = 3; break;
			case SC_MANUAL_5: wSlot = 4; break;
			case SC_MANUAL_6: wSlot = 5; break;
			case SC_MANUAL_7: wSlot = 6; break;
			case SC_MANUAL_8: wSlot = 7; break;
			case SC_MANUAL_9: wSlot = 8; break;
		};
		if (getSlotNum(wSlot) != INVALID_WEAPON_SLOT)
		{
			setFireMode(FM_MANUAL);
			_currentWeaponSlot = wSlot;
		};
    };
	if ((shipControlFlag & (SC_ROUNDROBIN)) == SC_ROUNDROBIN)
    { // select round robin fire mode
		if (getFireMode() == FM_ROUNDROBIN)
		{
			if (getSlotNum(_currentWeaponSlot+1) != INVALID_WEAPON_SLOT) _currentWeaponSlot++; else _currentWeaponSlot = 0;
		}
		else
		{
			setFireMode(FM_ROUNDROBIN);
			_currentWeaponSlot = 0;
		};
    };
	if ((shipControlFlag & (SC_VOLLEY)) == SC_VOLLEY)
    { // select volley fire mode
		if (getFireMode() == FM_VOLLEY_GROUP)
		{
			if (getWeaponGroupSlot(_currentWeaponSlot+1) != INVALID_WEAPON_SLOT)
			{
				_currentWeaponSlot++;
			}
			else 
			{
				setFireMode(FM_VOLLEY_ALL);
				_currentWeaponSlot = 0;
			};
		}
		else
		{
			setFireMode(FM_VOLLEY_GROUP);
			_currentWeaponSlot = 0;
		};
    };
	// Control fire
	if ((shipControlFlag & SC_FIRE) == SC_FIRE)
	{
		fire();
		if (_fireMode == FM_ROUNDROBIN)
		{
			// Change active weapon
			if (getSlotNum(_currentWeaponSlot+1) != INVALID_WEAPON_SLOT) _currentWeaponSlot++; else _currentWeaponSlot = 0;
		};
	};
};

void SpaceShip::calculateEquipmentParameters()
{
	_longitudinalThrust = 0;
	_transversalThrust = 0;
	_power = 0;

	for (std::vector<EquipmentSlot*>::iterator pSlot = _equipmentSlots.begin(); pSlot != _equipmentSlots.end(); pSlot++)
	{
		if ((*pSlot)->getType() == EQ_DRIVE)
		{
			DriveEquipment* tmp = static_cast<DriveEquipment*>((*pSlot)->getInstalledEquipment());
			if (tmp != NULL)
			{
				_longitudinalThrust += tmp->getLongitudinalThrust();
				_transversalThrust += tmp->getTransversalThrust();
			};
		};
		if ((*pSlot)->getType() == EQ_ENERGY_CELL)
		{
			EnergyEquipment* tmp = static_cast<EnergyEquipment*>((*pSlot)->getInstalledEquipment());
			if (tmp != NULL)
			{
				_power += tmp->getPower();
			};
		};
	};
	_currentWeaponSlot = 0;
	// If there is no power cells, we can't move
	if (_power == 0) {
		_longitudinalThrust = 0;
		_transversalThrust = 0;
	};
};

Ogre::Real SpaceShip::getMaxTangentialAcceleration()
{
	return _longitudinalThrust / _nxActor->getMass();
};

}; // namespace SpaceArena
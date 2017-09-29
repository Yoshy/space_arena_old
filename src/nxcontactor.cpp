// (C) 2010 Yoshy
// Реализация класса NxContactor - обработчик уведомлений о событиях
// контактирования актеров в физической сцене PhysX.

#include "nxcontactor.h"
#include "spaceobject.h"
#include "spaceship.h"
#include "projectile.h"
#include "equipment.h"

namespace SpaceArena {

void NxContactor::onContactNotify(NxContactPair& pair, NxU32 events)
{
	SpaceObject* object1 = (SpaceObject*)pair.actors[0]->userData;
	SpaceObject* object2 = (SpaceObject*)pair.actors[1]->userData;

	Ogre::Real explosiveEnergy = 0;
	if (!pair.isDeletedActor[0])
	{ // calculate object1 impact
		if (object2->getType() == SO_PROJECTILE)
		{ // another object is explosive stuff, so impact will be much more
			if ((object1->getType() == SO_SPACESHIP) &&
				(static_cast<SpaceShip*>(object1)->getID() == static_cast<Projectile*>(object2)->getOwner()))
			{ // its our surprise
			}
			else
			{ // Explode the object, in this case projectile is destroyed and its explosion energy goes to damage of other object
				WeaponEquipment w = static_cast<Projectile*>(object2)->getRespectiveWeapon();
				explosiveEnergy = static_cast<AmmunitionEquipment*>(equipments[w.getAmmunitionID()])->getExplosiveEnergy();
				object2->_isSafeToDelete = true;
			}
		};
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: =========================================");
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: Detect collision of objects: '"+object1->getName()+"' and '"+object2->getName()+"'");
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: pair.sumNormalForce.magnitude(): "+Ogre::StringConverter::toString(pair.sumNormalForce.magnitude(), 20, 10));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: object '"+object1->getName()+"' take impact");
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: mass: "+Ogre::StringConverter::toString(object1->getMass(), 20, 10));
		Ogre::Vector3 relativeVelocity = object1->getLinearVelocity() - object2->getLinearVelocity();
		Ogre::Real V = relativeVelocity.length();
		Ogre::Real impactEnergy = (V * V * object2->getMass())/2;
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: relativeVelocity magnitude: "+Ogre::StringConverter::toString(V, 20, 10));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: impactEnergy: "+Ogre::StringConverter::toString(impactEnergy, 20, 10));
		if (object1->getType() == SO_SPACESHIP)
			Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _shieldsCharge before: "+Ogre::StringConverter::toString(static_cast<SpaceShip*>(object1)->getShieldsCharge()));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _curToughness before: "+Ogre::StringConverter::toString(object1->getCurToughness()));
		Ogre::Real impact = pair.sumNormalForce.magnitude() / object1->getMass() / 1000;
		object1->applyImpact(impact + explosiveEnergy);
		if (object1->getType() == SO_SPACESHIP)
			Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _shieldsCharge after: "+Ogre::StringConverter::toString(static_cast<SpaceShip*>(object1)->getShieldsCharge()));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _curToughness after: "+Ogre::StringConverter::toString(object1->getCurToughness()));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: -----------------------------------------");
	}

	explosiveEnergy = 0;
	if (!pair.isDeletedActor[1])
	{ // calculate object2 impact
		if (object1->getType() == SO_PROJECTILE)
		{ // another object is explosive stuff, so impact will be much more
			if ((object2->getType() == SO_SPACESHIP) &&
				(static_cast<SpaceShip*>(object2)->getID() == static_cast<Projectile*>(object1)->getOwner()))
			{ // its our surprise
			}
			else
			{
				WeaponEquipment w = static_cast<Projectile*>(object1)->getRespectiveWeapon();
				explosiveEnergy = static_cast<AmmunitionEquipment*>(equipments[w.getAmmunitionID()])->getExplosiveEnergy();
				object1->_isSafeToDelete = true;
			}
		};
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: object '"+object2->getName()+"' take impact");
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: mass: "+Ogre::StringConverter::toString(object2->getMass(), 20, 10));
		Ogre::Vector3 relativeVelocity = object2->getLinearVelocity() - object1->getLinearVelocity();
		Ogre::Real V = relativeVelocity.length();
		Ogre::Real impactEnergy = (V * V * object1->getMass())/2;
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: relativeVelocity magnitude: "+Ogre::StringConverter::toString(V, 20, 10));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: impactEnergy: "+Ogre::StringConverter::toString(impactEnergy, 20, 10));
		if (object2->getType() == SO_SPACESHIP)
			Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _shieldsCharge before: "+Ogre::StringConverter::toString(static_cast<SpaceShip*>(object2)->getShieldsCharge()));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _curToughness before: "+Ogre::StringConverter::toString(object2->getCurToughness()));
		Ogre::Real impact = pair.sumNormalForce.magnitude() / object2->getMass() / 1000;
		object2->applyImpact(impact + explosiveEnergy);
		if (object2->getType() == SO_SPACESHIP)
			Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _shieldsCharge after: "+Ogre::StringConverter::toString(static_cast<SpaceShip*>(object2)->getShieldsCharge()));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: _curToughness after: "+Ogre::StringConverter::toString(object2->getCurToughness()));
		Ogre::LogManager::getSingletonPtr()->logMessage("CONTACTOR: =========================================");
	}
};

};
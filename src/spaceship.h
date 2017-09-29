// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::SpaceShip
    \brief SpaceShip

    Spaceship is a spaceobject, that can move in the space and is controlled by
    pilot (player or AI). Can carry weapon and defensive equipment.
*/

#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

//#include "spaceobject.h"
#include "projectile.h"
#include "equipment.h"

namespace SpaceArena {

class SpaceShip : public SpaceObject
{
public:
	/// Constructor
	SpaceShip(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String shipShemeName, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
		Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness);
	/// Destructor
	virtual ~SpaceShip();
	/// Add equipment slot to the ship 
	void addSlot(EQUIPMENT_TYPE type, Ogre::Vector3 shemePosition, Ogre::String equipmentID);
	/// Add equipment slot to the ship 
	void addSlot(EquipmentSlot &slot);
	/// Add equipment to the hold
	void putToTheHold(Ogre::String equipmentID, Ogre::uint amount);
	/// Return number of available slots
	inline Ogre::uint getNumSlots() { return _equipmentSlots.size(); };
	/// Return ship sheme name
	inline Ogre::String getShipShemeName() { return _shipShemeName; };
	/// Return pointer to equipment slot with number 'num', or NULL, if there is no such slot
	inline EquipmentSlot* getEquipmentSlot(Ogre::uint num) { if (num < _equipmentSlots.size()) return _equipmentSlots[num]; else return NULL; };
	/// Return pointer to hold slot with number 'num', or NULL, if there is no such slot
	inline HoldSlot* getHoldSlot(Ogre::uint num) { if (num < _hold.size()) return _hold[num]; else return NULL; };
	/// Get current fire mode
	inline FireMode getFireMode() { return _fireMode; };
	/// Set fire mode
	inline void setFireMode(FireMode mode);
	/// Return current active weapon slot
	inline Ogre::uint getCurrentWeaponSlot() { return _currentWeaponSlot; };
	/// Return absolute slot number for weapon slot number <weaponSlotNum> or value of INVALID_WEAPON_SLOT
	/// For example:
	/// slot number: 0 1 2 3 4 5 6
	/// weapon slot: - - + - - + +
	/// In this example, f(0) return 2, f(2) return 6, and f(3) return INVALID_WEAPON_SLOT
	Ogre::uint getSlotNum(Ogre::uint weaponSlotNum);
	/// Return absolute slot number for weapon group number <weaponGroupNum> or value of INVALID_WEAPON_SLOT
	/// Weapon group slot is first slot containing a weapon of specific ID, all other slots contain weapon of that ID belongs to group
	/// For example:
	/// slot number: 0 1 2 3 4 5 6
	/// weapon slot: - X Y - X Z Y
	/// In this example, f(0) return 1, f(2) return 5, and f(3) return INVALID_WEAPON_SLOT
	/// Slots 1,4 is weapon group 0, 2,6 - group 1, 5 - group 2
	Ogre::uint getWeaponGroupSlot(Ogre::uint weaponGroupNum);
	/// Get active weapon ID
	Ogre::String getActiveWeaponID();
	/// Fires with current active weapon or group of weapon
	void fire();
	// Energy stuff
	inline Ogre::Real getCapacitorCharge() { return _capacitorCharge; };
	inline Ogre::Real getShieldsCharge() { return _shieldsCharge; };
	inline void setCapacitorCharge(Ogre::Real value) { _capacitorCharge = value; if (_capacitorCharge > (POWER_CHARGE_FACTOR * _power)) _capacitorCharge = (POWER_CHARGE_FACTOR * _power); };
	inline void setShieldsCharge(Ogre::Real value) { _shieldsCharge = value; if (_shieldsCharge > (POWER_CHARGE_FACTOR * _power)) _shieldsCharge = (POWER_CHARGE_FACTOR * _power); };
	/// Apply external impact to ship
	inline virtual void applyImpact(Ogre::Real impact);
	/// Send ship control comand
    void ñontrol(int shipControlFlag, Ogre::Real timeDelta);
	/// Calculate spaceship parameters depending on installed equipment
	void calculateEquipmentParameters();
	Ogre::Real getMaxTangentialAcceleration();
	Ogre::Real getPower() { return _power; };
	//
	void setColdTime(Ogre::Real time) { _coldTime = time; };
	Ogre::Real getColdTime() { return _coldTime; };
private:
    /// Capacitor charge
    Ogre::Real _capacitorCharge;
    /// Energy shields charge
    Ogre::Real _shieldsCharge;
	/// Spaceship sheme name
	Ogre::String _shipShemeName;
	/// Spaceship equipment slots
	std::vector<EquipmentSlot*> _equipmentSlots;
	/// Spaceship hold
	std::vector<HoldSlot*> _hold;
	/// Dynamic ship parameters
	Ogre::Real _longitudinalThrust;
	Ogre::Real _transversalThrust;
	Ogre::Real _transversalFactor;
	Ogre::Real _power;
	/// Current fire mode
	FireMode _fireMode;
	/// For FM_MANUAL mode has no meaning
	/// For FM_ROUNDROBIN mode contain current active weapon slot. After each shot changes to another (or first) weapon slot.
	/// For FM_VOLLEY_ALL mode has no meaning
	/// For FM_VOLLEY_GROUP contain first weapon slot of a group. Ship fires with all slots of same weapon type.
	Ogre::uint _currentWeaponSlot;
	/// Weapon cold time
	Ogre::Real _coldTime;
};

}; // namespace SpaceArena

#endif // _SPACESHIP_H_

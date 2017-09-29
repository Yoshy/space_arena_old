// (C) 2009 Yoshy
/*! 
	\class SpaceArena::Projectile
    \brief Projectile

    Projectile is a spaceobject, that fly out of the weapon.
*/

#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "spaceobject.h"
#include "equipment.h"

namespace SpaceArena {

class Projectile : public SpaceObject
{
public:
    Projectile(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
		Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness);
	void setRespectiveWeapon(WeaponEquipment& weapon) { _weapon = weapon; };
	WeaponEquipment& getRespectiveWeapon() { return _weapon; };
	void setOwner(Ogre::uint id) { _ownerID = id; };
	Ogre::uint getOwner() { return _ownerID; };
private:
	// Respective weapon of this projectile object
	WeaponEquipment _weapon;
	// ID of owner (who fired this projectile)
	Ogre::uint _ownerID;
};

}; // namespace SpaceArena

#endif // _PROJECTILE_H_

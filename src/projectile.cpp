// (C) 2010 Yoshy
/*! 
	\class SpaceArena::Projectile
    \brief Projectile

    Projectile is a spaceobject, that fly out of the weapon
*/

#include "Projectile.h"

namespace SpaceArena {

Projectile::Projectile(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
				   Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness) :
                   SpaceObject(ID, sm, nxScene, meshName, materialName, name, meshScale, boundingSphereScale, position, direction, orientation, density, toughness)
{
    setType(SO_PROJECTILE);
};

}; // namespace SpaceArena
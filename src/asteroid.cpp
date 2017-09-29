// (C) 2009 Yoshy
/*! 
	\class SpaceArena::Asteroid
    \brief Asteroid

    Asteroid is a spaceobject, that just passively resides in space.
*/

#include "asteroid.h"

namespace SpaceArena {

Asteroid::Asteroid(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
				   Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness) :
                   SpaceObject(ID, sm, nxScene, meshName, materialName, name, meshScale, boundingSphereScale, position, direction, orientation, density, toughness)
{
    setType(SO_ASTEROID);
};

}; // namespace SpaceArena
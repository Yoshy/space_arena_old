// (C) 2009 Yoshy
/*! 
	\class SpaceArena::Asteroid
    \brief Asteroid

    Asteroid is a spaceobject, that just passively resides in space.
*/

#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include "spaceobject.h"

namespace SpaceArena {

class Asteroid : public SpaceObject
{
public:
    Asteroid(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
		Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness);
};

}; // namespace SpaceArena

#endif // _ASTEROID_H_

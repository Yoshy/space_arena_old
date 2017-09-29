// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::Universe
    \brief Universe - there all action goes

    Universe contains all entities of SpaceArena world
*/

#include "universe.h"

namespace SpaceArena {

void Universe::bigBang(Ogre::SceneManager* sceneManager, NxScene* nxScene)
{
    Singularity singularity(sceneManager, nxScene);
	singularity.initResources();
	singularity.initUniverse(_spaceObjects);
};

void Universe::process(Ogre::Real timeDelta)
{
    _spaceObjects.processObjects(timeDelta);
    _characters.thinking();
	_spaceObjects.purgeObjects();
};
void Universe::release()
{
	_spaceObjects.clear();
	_characters.clear();
	// Delete all equipment
	if (!equipments.empty())
	{
		for (std::map<Ogre::String, Equipment*>::iterator i = equipments.begin(); i != equipments.end(); i++)
		{
			delete (*i).second;
		};
	};
};

Universe::~Universe() {
};

}; // namespace SpaceArena
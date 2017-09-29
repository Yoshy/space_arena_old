// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::Singularity
    \brief Singularity - origin of everything in universe.

	From the singularity appear all space objects and everything else in the big bang.
*/

#ifndef _SINGULARITY_H_
#define _SINGULARITY_H_

#include <fstream>
#include "rapidxml.hpp"
#include "asteroid.h"
#include "spaceship.h"
#include "spaceobjectmanager.h"

namespace SpaceArena {

class Singularity
{
public:
	Singularity(Ogre::SceneManager* sceneManager, NxScene* nxScene);
	~Singularity();
	/// Load and initialise game data
	void initResources();
	/// Load world data and create predefined universe
	void initUniverse(SpaceObjectManager& spaceObjects);
private:
	/// Load data from XML file and parse it
	/// Application must delete xml_document and XMLBuffer after use
	rapidxml::xml_document<>* loadAndParseXMLData(Ogre::String fileName, char* XMLBuffer);

	Ogre::SceneManager* _sceneManager;
	NxScene* _nxScene;
};

}; // namespace SpaceArena

#endif // _SINGULARITY_H_

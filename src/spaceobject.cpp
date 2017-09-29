// (C) 2009-2010 Yoshy
// Реализация "абстрактного" класса SpaceObject - космический объект.
// Космический объект - это любая "материальная" сущность,
// находящаяся во вселенной.

#include "spaceobject.h"

namespace SpaceArena {

SpaceObject::SpaceObject(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
                         Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness) {
    //Ogre::String name;
    Ogre::SceneNode* node;

    _id = ID;
    _sceneManager = sm;
	_nxScene = nxScene;
	_boundingSphereScale = boundingSphereScale;
    //name = "SpaceObject" + Ogre::StringConverter::toString(_id);
    _ogreEntity = _sceneManager->createEntity(name, meshName);
    _ogreEntity->setMaterialName(materialName);
    _name = name;
    
    // инициализируем графическое представление
    name = "SpaceObjectNode" + Ogre::StringConverter::toString(_id);
    node = sm->getRootSceneNode()->createChildSceneNode(name);
    node->attachObject(_ogreEntity);
    _meshScale = meshScale;
    node->setScale(Ogre::Vector3(meshScale));
	node->setPosition(position);
	_baseOrientation = orientation;
	if (_baseOrientation == Ogre::Quaternion::ZERO) _baseOrientation = Ogre::Quaternion::IDENTITY;
	node->setOrientation(_baseOrientation);
    node->setDebugDisplayEnabled(true);

    // Initialise physical representation
	// Add one-shaped actor to physical scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= (NxReal)boundingSphereScale;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);
	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= density;
	actorDesc.globalPose.t	= toNx(position);
	actorDesc.userData = this;
	if ((_nxActor = _nxScene->createActor(actorDesc)) == NULL)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Unable to create NxActor ");
	};
    _maxToughness = toughness;
    _curToughness = toughness;
	_isSafeToDelete = false;
};

SpaceObject::~SpaceObject() {
	_nxScene->releaseActor(*_nxActor);
	_sceneManager->getRootSceneNode()->removeAndDestroyChild(_ogreEntity->getParentSceneNode()->getName());
	_sceneManager->destroyEntity(_ogreEntity);
};

void SpaceObject::applyImpact(Ogre::Real impact)
{
	_curToughness -= impact;
	if (_curToughness < 0) _curToughness = 0;
};

}; // namespace SpaceArena
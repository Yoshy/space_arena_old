/*! 
	\class SpaceArena::SpaceObject
    \brief Космический объект.

    Космический объект - это любая "материальная" сущность,
    находящаяся во вселенной.
*/

#ifndef _SPACEOBJECT_H_
#define _SPACEOBJECT_H_

#include "ogre.h"
#include "nxphysics.h"
#include "globals.h"

namespace SpaceArena {

class SpaceObject
{
public:
    friend class SpaceObjectManager;
    SpaceObject(unsigned ID, Ogre::SceneManager* sm, NxScene* nxScene, Ogre::String meshName, Ogre::String materialName, Ogre::String name,
		Ogre::Real meshScale, Ogre::Real boundingSphereScale, Ogre::Vector3& position, Ogre::Vector3& direction, Ogre::Quaternion& orientation, Ogre::Real density, Ogre::Real toughness);
    virtual ~SpaceObject();
    inline void setName(Ogre::String name) { _name = name; };
	inline virtual void applyImpact(Ogre::Real impact);
    inline void setCurToughness(Ogre::Real T) { if (T > 0) _curToughness = T; else _curToughness = 0; };
    inline void setMaxToughness(Ogre::Real T) { if (T > 0) _maxToughness = T; else _maxToughness = 0; };
	inline void applyForce(Ogre::Vector3& force) { _nxActor->addForce(toNx(force), NX_FORCE); };
	inline Ogre::uint getID() { return _id; };
    inline SpaceObjectType getType() { return _type; };
    inline Ogre::String getName() { return _name; };
	inline Ogre::Vector3 getPosition() { return toOgre(_nxActor->getGlobalPosition()); };
	inline void setOrientation(Ogre::Quaternion orientation) { _nxActor->setGlobalOrientationQuat(toNx(orientation)); };
	inline Ogre::Quaternion getOrientation() { return toOgre(_nxActor->getGlobalOrientationQuat()); };
    inline Ogre::Real getCurToughness() { return _curToughness; };
    inline Ogre::Real getMaxToughness() { return _maxToughness; };
	inline Ogre::Real getMass() { return _nxActor->getMass(); };
	inline Ogre::Vector3 getLinearVelocity() { return toOgre(_nxActor->getLinearVelocity()); };
    /// Можно ли удалять данный объект
    bool _isSafeToDelete;
protected:
	/// Pointer to containing space object manager
	/// Space object manager instance automatically initiates this field, when object is added to it.
	SpaceObjectManager* _managedBy;
    inline void setType(SpaceObjectType type) { _type = type; };
    inline void setOgreSceneNodePosition(Ogre::Vector3 position) { _ogreEntity->getParentSceneNode()->setPosition(position); };
    inline void setOgreSceneNodeOrientation(Ogre::Quaternion orientation) { _ogreEntity->getParentSceneNode()->setOrientation(orientation); };
	Ogre::SceneManager* _sceneManager; /// Графическая сцена, которой принадлежит объект
    NxScene* _nxScene; /// Физическая сцена, которой принадлежит объект
	/// Указатель на объект NxActor для связи космического объекта
    /// с его физической моделью
    NxActor* _nxActor;
	/// Bounding sphere radius
	Ogre::Real _boundingSphereScale;
private:
	/// Object identifier
	Ogre::uint _id;
	 /// Object type (look at globals.h)
    SpaceObjectType _type;
	/// Имя объекта
    Ogre::String _name;
    /// Pointer to the object of the class Ogre::Entity to map spaceobject with his visual representation
    Ogre::Entity* _ogreEntity;
	/// Base mesh scale of the object
    unsigned _meshScale;
	/// Base object orientation
	Ogre::Quaternion _baseOrientation;
	/// Прочность объекта. Когда она достигает нуля, объект разрушается. Значения (0..inf)
    Ogre::Real _curToughness;
    Ogre::Real _maxToughness;
};

}; // namespace SpaceArena

#endif // _SPACEOBJECT_H_

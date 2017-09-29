// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::Universe
    \brief Universe - there all action goes

    Universe contains all entities of SpaceArena world
*/

#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include "spaceobjectmanager.h"
#include "charactermanager.h"
#include "singularity.h"

namespace SpaceArena {

class Universe
{
public:
	/// Big Bang creates the game universe, all objects and content
    void bigBang(Ogre::SceneManager* sceneManager, NxScene* nxScene);
    /// Пересчет состояния всех объектов Вселенной
    void process(Ogre::Real timeDelta);
    /// возвращает указатель на первый космический объект в списке или НУЛЬ, если список пуст
    SpaceObject* getFirstSpaceObject() { return _spaceObjects.getFirstObject(); };
    /// возвращает указатель на текущий объект и увеличивает указатель
    /// текущего объекта. Если НУЛЬ, то конец списка.
    SpaceObject* getCurrentSpaceObject() { return _spaceObjects.getCurrentObject(); };
    /// возвращает указатель на объект с заданным ID или НУЛЬ, если нет такого объекта
    SpaceObject* getSpaceObjectByID(int ID) { return _spaceObjects.getObjectByID(ID); };
	/// добавляет персонажа во вселенную
	inline void addCharacter(Character* character) { _characters.addCharacter(character); };
	/// удаляет все объекты из вселенной, готовясь к уничтожению
	void release();
    /// Деструктор
    ~Universe();
private:
    SpaceObjectManager _spaceObjects;
    CharacterManager _characters;
};

}; // namespace SpaceArena

#endif // _UNIVERSE_H_

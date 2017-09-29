/*! 
    \class SpaceArena::SpaceObjectManager
    \brief Менеджер космических объектов.

    Менеджер космических объектов управляет всеми космическими объектами
    во вселенной - добавляет, удаляет, перемещает и т.п.
*/

#ifndef _SPACEOBJECTMANAGER_H_
#define _SPACEOBJECTMANAGER_H_

#include <list>
#include "spaceobject.h"
#include "spaceship.h"

namespace SpaceArena {

struct SpaceObjectTemplate
{
	SpaceObjectType type;
	Ogre::String name;
	Ogre::String meshName;
	Ogre::String materialName;
	Ogre::String shipShemeName;
	Ogre::Real meshScale;
	Ogre::Real boundingSphereScale;
	Ogre::Real orientation_x;
	Ogre::Real orientation_y;
	Ogre::Real orientation_z;
	Ogre::Real orientation_w;
	Ogre::Real density;
	std::vector<EquipmentSlot*> equipmentSlots;
	std::vector<HoldSlot*> holdSlots;
};
typedef std::map<Ogre::String, SpaceObjectTemplate> SpaceObjectTemplatesMap;
typedef SpaceObject* pSpaceObject;
typedef std::list<pSpaceObject> SpaceObjectList;
typedef std::list<pSpaceObject>::iterator SpaceObjectListIterator;

extern SpaceObjectTemplatesMap spaceObjectTemplates;

class SpaceObjectManager
{
public:
	SpaceObjectManager() { _IDCounter = 0; };
    ~SpaceObjectManager();
    /// Добавляет объект в список. Объект должен быть создан. Удалять его вне класса
    /// не нужно - деструктор ~SpaceObjectManager() побеспокоится об этом.
    void addObject(SpaceObject* object);
    /// Возвращает указатель на объект с заданным ID или НУЛЬ, если нет такого объекта
    SpaceObject* getObjectByID(int ID);
    /// Возвращает тип объекта или SO_INVALID, если нет объекта c таким ID
    SpaceObjectType getObjectType(int ID);
    /// Возвращает указатель на первый объект в списке или НУЛЬ, если список пуст
	/// текущим становится следующий за первым объект
    SpaceObject* getFirstObject(void);
    /// Возвращает указатель на текущий объект и увеличивает указатель
    /// текущего объекта. Если НУЛЬ, то конец списка.
    SpaceObject* getCurrentObject(void);
    /// Взаимодействие объектов и обновление их состояний
    void processObjects(Ogre::Real timeDelta);
	/// Удаляет из списка неиспользуемые (разрушенные) объекты.
	/// Желательно запускать как можно реже, так как реализация индусская
    void purgeObjects();
	/// Удаляет из списка все объекты
	void clear();
	/// Return ID for new object.
	Ogre::uint getNewID() { _IDCounter++; return _IDCounter-1; };
private:
	// Counter for generating unique ID's
	Ogre::uint _IDCounter;
    SpaceObjectListIterator _currentObject;
    SpaceObjectList _spaceObjects;
};

}; // namespace SpaceArena

#endif // _SPACEOBJECTMANAGER_H_

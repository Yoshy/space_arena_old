/*! 
    \class SpaceArena::SpaceObjectManager
    \brief �������� ����������� ��������.

    �������� ����������� �������� ��������� ����� ������������ ���������
    �� ��������� - ���������, �������, ���������� � �.�.
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
    /// ��������� ������ � ������. ������ ������ ���� ������. ������� ��� ��� ������
    /// �� ����� - ���������� ~SpaceObjectManager() ������������� �� ����.
    void addObject(SpaceObject* object);
    /// ���������� ��������� �� ������ � �������� ID ��� ����, ���� ��� ������ �������
    SpaceObject* getObjectByID(int ID);
    /// ���������� ��� ������� ��� SO_INVALID, ���� ��� ������� c ����� ID
    SpaceObjectType getObjectType(int ID);
    /// ���������� ��������� �� ������ ������ � ������ ��� ����, ���� ������ ����
	/// ������� ���������� ��������� �� ������ ������
    SpaceObject* getFirstObject(void);
    /// ���������� ��������� �� ������� ������ � ����������� ���������
    /// �������� �������. ���� ����, �� ����� ������.
    SpaceObject* getCurrentObject(void);
    /// �������������� �������� � ���������� �� ���������
    void processObjects(Ogre::Real timeDelta);
	/// ������� �� ������ �������������� (�����������) �������.
	/// ���������� ��������� ��� ����� ����, ��� ��� ���������� ���������
    void purgeObjects();
	/// ������� �� ������ ��� �������
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

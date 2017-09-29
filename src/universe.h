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
    /// �������� ��������� ���� �������� ���������
    void process(Ogre::Real timeDelta);
    /// ���������� ��������� �� ������ ����������� ������ � ������ ��� ����, ���� ������ ����
    SpaceObject* getFirstSpaceObject() { return _spaceObjects.getFirstObject(); };
    /// ���������� ��������� �� ������� ������ � ����������� ���������
    /// �������� �������. ���� ����, �� ����� ������.
    SpaceObject* getCurrentSpaceObject() { return _spaceObjects.getCurrentObject(); };
    /// ���������� ��������� �� ������ � �������� ID ��� ����, ���� ��� ������ �������
    SpaceObject* getSpaceObjectByID(int ID) { return _spaceObjects.getObjectByID(ID); };
	/// ��������� ��������� �� ���������
	inline void addCharacter(Character* character) { _characters.addCharacter(character); };
	/// ������� ��� ������� �� ���������, �������� � �����������
	void release();
    /// ����������
    ~Universe();
private:
    SpaceObjectManager _spaceObjects;
    CharacterManager _characters;
};

}; // namespace SpaceArena

#endif // _UNIVERSE_H_

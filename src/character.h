// (C) 2009 Yoshy
// ��������� ������ Character - ��������.
// ��������� - "��������" ��������, ������� ��������� ������������ ���������.


#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "spaceship.h"

namespace SpaceArena {

class Character
{
public:
    Character(Ogre::String name, SpaceShip* ship) { _name = name; _ship = ship; _isAlive = true; };
    inline const Ogre::String getName() { return _name; };
	inline const Ogre::Vector3 getSpaceShipPosition()
	{
		if (_ship != NULL ) return _ship->getPosition();
		else return Ogre::Vector3::ZERO;
	};
    // ������ ���������� ����� � �������� �������
    virtual void think() = 0;
    // ������� ���������
    SpaceShip* _ship;
	// ��� �� ��������
	bool _isAlive;
protected:
    // ��� ���������
    Ogre::String _name;
};

}; // namespace SpaceArena

#endif // _CHARACTER_H_

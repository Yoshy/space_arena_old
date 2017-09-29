// (C) 2009 Yoshy
// Интерфейс класса Character - персонаж.
// Персонажи - "разумные" существа, которые управляют космическими кораблями.


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
    // Анализ окружающей среды и принятие решения
    virtual void think() = 0;
    // Корабль персонажа
    SpaceShip* _ship;
	// Жив ли персонаж
	bool _isAlive;
protected:
    // Имя персонажа
    Ogre::String _name;
};

}; // namespace SpaceArena

#endif // _CHARACTER_H_

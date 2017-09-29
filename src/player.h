// (C) 2009 Yoshy
// Интерфейс класса Player - игрок.
// Игрок - персонаж "по ту сторону монитора", взаимодействующий с игровыми
// миром.


#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

namespace SpaceArena {

class Player : public Character {
public:
    Player(Ogre::String name, SpaceShip* ship);
    // Отдать команду управления кораблем
    void shipControl(int shipControlFlag, Ogre::Real timeDelta);
    virtual void think();
};

}; // namespace SpaceArena

#endif // _PLAYER_H_

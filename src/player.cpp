// (C) 2009 Yoshy
// Реализация класса Player - игрок.
// Игрок - персонаж "по ту сторону монитора", взаимодействующий с игровыми
// миром.

#include "player.h"

namespace SpaceArena {

Player::Player(Ogre::String name, SpaceShip* ship) : Character(name, ship) {
};

void Player::shipControl(int shipControlFlag, Ogre::Real timeDelta) {
    if (_ship != NULL) _ship->сontrol(shipControlFlag, timeDelta);
};

void Player::think() {
    // Игрок сам за себя думает
};

}; // namespace SpaceArena
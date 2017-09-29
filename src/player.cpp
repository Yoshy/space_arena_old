// (C) 2009 Yoshy
// ���������� ������ Player - �����.
// ����� - �������� "�� �� ������� ��������", ����������������� � ��������
// �����.

#include "player.h"

namespace SpaceArena {

Player::Player(Ogre::String name, SpaceShip* ship) : Character(name, ship) {
};

void Player::shipControl(int shipControlFlag, Ogre::Real timeDelta) {
    if (_ship != NULL) _ship->�ontrol(shipControlFlag, timeDelta);
};

void Player::think() {
    // ����� ��� �� ���� ������
};

}; // namespace SpaceArena
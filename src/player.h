// (C) 2009 Yoshy
// ��������� ������ Player - �����.
// ����� - �������� "�� �� ������� ��������", ����������������� � ��������
// �����.


#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"

namespace SpaceArena {

class Player : public Character {
public:
    Player(Ogre::String name, SpaceShip* ship);
    // ������ ������� ���������� ��������
    void shipControl(int shipControlFlag, Ogre::Real timeDelta);
    virtual void think();
};

}; // namespace SpaceArena

#endif // _PLAYER_H_

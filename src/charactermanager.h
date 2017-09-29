/*! 
    \class SpaceArena::CharacterManager
    \brief �������� ����������.

    �������� ���������� ��������� ����� ����������� �� ��������� - ���������, �������, ���������� � �.�.
*/

#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

#include <list>
#include "character.h"

namespace SpaceArena {

typedef Character* pCharacter;
typedef std::list<pCharacter> CharacterList;
typedef std::list<pCharacter>::iterator CharacterListIterator;

class CharacterManager
{
public:
    ~CharacterManager();
    void init();
    /// ��������� ��������� � ������. �������� ������ ���� ������. ������� ��� ��� ������
    /// �� ����� - ���������� ~CharacterManager() ������������� �� ����
    void addCharacter(Character* character);
    /// ������� ��������� �� ������ (��������, ����� ��� �������)
    void removeCharacter(Character* character);
    /// ���������, �� ���� �� ��� �� ����������
    void checkForDead();
    /// ��������� �������� ������������� �������� ��� ���� ����������
    void thinking();
    /// ������� �� ������ ��� �������
    void clear();
private:
    CharacterListIterator _currentCharacter;
    CharacterList _characters;
};

}; // namespace SpaceArena

#endif // _CHARACTERMANAGER_H_
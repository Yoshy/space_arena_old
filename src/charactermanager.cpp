/*! 
    \class SpaceArena::CharacterManager
    \brief Менеджер персонажей.

    Менеджер персонажей управляет всеми персонажами во вселенной - добавляет, удаляет, перемещает и т.п.
*/

#include "charactermanager.h"

namespace SpaceArena {

CharacterManager::~CharacterManager()
{
	clear();
};

void CharacterManager::clear()
{
    pCharacter C;
    CharacterListIterator iterator;

    if (_characters.empty()) return;
    for (iterator = _characters.begin(); iterator != _characters.end(); iterator++)
    {
       	C = *iterator;
        delete C;
    }
	_characters.clear();
};

void CharacterManager::init()
{
};

void CharacterManager::addCharacter(Character* C)
{
	_characters.push_front(C);
};

void CharacterManager::removeCharacter(Character* C)
{
    /// \todo CharacterManager::removeCharacter - реализовать
};

void CharacterManager::checkForDead()
{
};

void CharacterManager::thinking()
{
    pCharacter C;
    CharacterListIterator iterator;

    if (_characters.empty()) return;
    for (iterator = _characters.begin(); iterator != _characters.end(); iterator++)
    {
       	C = *iterator;
        C->think();
		if ((C->_ship != NULL) && (C->_ship->_isSafeToDelete == true)) // The ship is destroyed, it's time to die.
		{
			// Invalidate ship pointer, SpaceObjectManager::purgeObjects actually deletes this object
			C->_ship = NULL;
			C->_isAlive = false;
		};
    }
};

}; // namespace SpaceArena
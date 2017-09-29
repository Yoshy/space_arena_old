/*! 
    \class SpaceArena::CharacterManager
    \brief Менеджер персонажей.

    Менеджер персонажей управляет всеми персонажами во вселенной - добавляет, удаляет, перемещает и т.п.
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
    /// Добавляет персонажа в список. Персонаж должен быть создан. Удалять его вне класса
    /// не нужно - деструктор ~CharacterManager() побеспокоится об этом
    void addCharacter(Character* character);
    /// Удаляет персонажа из списка (например, когда тот умирает)
    void removeCharacter(Character* character);
    /// Проверяет, не умер ли кто из персонажей
    void checkForDead();
    /// Запускает итерацию мыслительного процесса для всех персонажей
    void thinking();
    /// Удаляет из списка все объекты
    void clear();
private:
    CharacterListIterator _currentCharacter;
    CharacterList _characters;
};

}; // namespace SpaceArena

#endif // _CHARACTERMANAGER_H_
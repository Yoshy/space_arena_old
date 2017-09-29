// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::HUD
    \brief Head-Up Display.

    HUD содержит коллекцию HUDElement и ответственен за их отображение.
*/
#ifndef _HUD_H_
#define _HUD_H_

#include <list>
#include <ogre.h>

namespace SpaceArena {

class HUDElement
{
public:
    virtual void update() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
	virtual void changeVisibility() = 0;
private:
	Ogre::String _name;
};

typedef HUDElement* pHUDElement;
typedef std::list<pHUDElement> HUDElementList;
typedef std::list<pHUDElement>::iterator HUDElementListIterator;

class HUD
{
public:
    ~HUD();
    void init();
    /// Добавляет элемент в список. Элемент должен быть создан. Удалять его вне класса
    /// не нужно - деструктор ~HUD() побеспокоится об этом.
    void addHUDElement(HUDElement* he);
    void update();
    void show();
    void hide();
private:
    HUDElementListIterator _currentHUDElement;
    HUDElementList _HUDElements;
};

}; // namespace SpaceArena

#endif // _HUD_H_
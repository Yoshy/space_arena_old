// (C) 2009 Yoshy
// Интерфейс класса HUDDebug - отладочный элемент.
// Отображает полезную инфу.

#ifndef _HUD_DEBUG_H_
#define _HUD_DEBUG_H_

#include "hud.h"
#include "universe.h"
#include "spaceobject.h"

namespace SpaceArena {

class HUDDebug : public HUDElement
{
public:    
    HUDDebug(Universe* universe, Ogre::RenderWindow* window);
    virtual void update();
    virtual void show();
    virtual void hide();
	virtual void changeVisibility();
private:
    Ogre::RenderWindow* _window;
    Universe* _universe;
};

}; // namespace SpaceArena

#endif // _HUD_DEBUG_H_
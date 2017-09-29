// (C) 2010 Yoshy
/*! 
	\class SpaceArena::HUDSheme
    \brief Схема оборудования корабля.

    На схеме оборудования корабля отображается количество и расположение слотов
    оборудования, а также оборудование в них установленное.
*/

#ifndef _HUD_SHEME_H_
#define _HUD_SHEME_H_

#include "hud.h"
#include "player.h"

namespace SpaceArena {

class HUDSheme : public HUDElement
{
public:    
	HUDSheme(Ogre::RenderWindow*, Player* player);
    virtual void update();
    virtual void show();
    virtual void hide();
	virtual void changeVisibility();
private:

    //Universe* _universe;
    //Ogre::Camera* _camera;
    Ogre::RenderWindow* _window;
	Player * _player;
};

}; // namespace SpaceArena

#endif // _HUD_SHEME_H_
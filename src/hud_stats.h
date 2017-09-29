// (C) 2009 Yoshy
// Интерфейс класса HUDShipStats - индикаторы корабля.
// На индикаторах корабля отображаются важнейшие показатели.

#ifndef _HUD_STATS_H_
#define _HUD_STATS_H_

#include "hud.h"
#include "player.h"

namespace SpaceArena {

class HUDShipStats : public HUDElement
{
public:    
    HUDShipStats(Player* player);
    virtual void update();
    virtual void show();
    virtual void hide();
	virtual void changeVisibility();
private:
    Player* _player;
};

}; // namespace SpaceArena

#endif // _HUD_STATS_H_
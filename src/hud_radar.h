// (C) 2009 Yoshy
// Интерфейс класса HUDRadar - радар.
// Радар отображает ближайшие объекты.

#ifndef _HUD_RADAR_H_
#define _HUD_RADAR_H_

#include "hud.h"
#include "universe.h"
#include "spaceobject.h"

namespace SpaceArena {

// Actual radar dimensions in pixels
#define RADAR_RADIUS 64

class RadarObject
{
public:
    Ogre::OverlayElement* _overlayElement;
    Ogre::SceneNode* _node;
};

typedef RadarObject* pRadarObject;
typedef std::vector<pRadarObject> radarObjectsVector;
typedef std::vector<pRadarObject>::iterator radarObjectsVectorIterator;

class HUDRadar : public HUDElement
{
public:    
    HUDRadar(Universe* universe, Ogre::RenderWindow* window, Ogre::Camera* camera);
    virtual void update();
    virtual void show();
    virtual void hide();
	virtual void changeVisibility();
private:
    Universe* _universe;
    Ogre::Camera* _camera;
    Ogre::RenderWindow* _window;
    radarObjectsVector _radarObjects ;
    Ogre::Real _scale;
};

}; // namespace SpaceArena

#endif // _HUD_RADAR_H_
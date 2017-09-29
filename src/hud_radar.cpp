// (C) 2009 Yoshy
// Реализация класса HUDRadar - радар.
// Радар отображает ближайшие объекты.

#include "hud_radar.h"

namespace SpaceArena {

#define OVERLAY_NAME "HUD/radarOverlay"

HUDRadar::HUDRadar(Universe* universe, Ogre::RenderWindow* window, Ogre::Camera* camera)
{
    show();
    _universe = universe;
    _camera = camera;
    _window = window;
    _scale = 100;
};

void HUDRadar::update()
{
    SpaceObject* so;
    pRadarObject radarObject;
    Ogre::Vector3 objectPosition, cameraPosition;
    Ogre::Overlay* radarOverlay = Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME);
    Ogre::OverlayContainer* radarContainer = radarOverlay->getChild("HUD/radar");

    // Clear radarObjects vector
    if(!_radarObjects.empty())
	{
		for(radarObjectsVectorIterator iterator = _radarObjects.begin(); iterator != _radarObjects.end(); iterator++)
		{
			radarObject = *iterator;
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(radarObject->_overlayElement);
            if (radarObject != NULL) delete radarObject;
		};
	};
    _radarObjects.clear();
    cameraPosition = _camera->getPosition();
    cameraPosition.z = 0;
    // For any object in Universe...
    if ((so = _universe->getFirstSpaceObject()) != NULL)
    {
        while (so != NULL)
        {
            objectPosition = so->getPosition();
            // ...if it is near camera...
            if (objectPosition.distance(cameraPosition) > /* _window->getWidth() * */(radarContainer->getWidth()/2) * _scale)
            {
                so = _universe->getCurrentSpaceObject();
                continue;
            };
            // ...then add object to radarObjects vector
            radarObject = new(RadarObject);
            radarObject->_overlayElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("HUD/radarDot", "Panel", "radarObject_" + so->getName());
            radarContainer->addChild(radarObject->_overlayElement);
            // set object position on radar
            Ogre::Real origin_x = (radarContainer->getWidth() / 2) - (radarObject->_overlayElement->getWidth() / 2);
            Ogre::Real origin_y = (radarContainer->getHeight() / 2) - (radarObject->_overlayElement->getHeight() / 2);
            Ogre::Real dx = ((cameraPosition.x - objectPosition.x) / _scale);// / _window->getWidth();
            Ogre::Real dy = ((cameraPosition.y - objectPosition.y) / _scale);// / _window->getHeight();
            radarObject->_overlayElement->setPosition(origin_x - dx, origin_y + dy);
            switch (so->getType())
            {
            case SO_ASTEROID:
                radarObject->_overlayElement->setMaterialName("HUD/radar/GreenCircle");
                break;
            case SO_SPACESHIP:
                radarObject->_overlayElement->setMaterialName("HUD/radar/RedSquare");
                break;
            default:
                break;
            };
            _radarObjects.push_back(radarObject);
            so = _universe->getCurrentSpaceObject();
        };
    };
};

void HUDRadar::show()
{
    Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->show();
};
void HUDRadar::hide()

{
    Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->hide();
};

void HUDRadar::changeVisibility()

{
	Ogre::Overlay* O = Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME);
    if (O->isVisible()) O->hide(); else O->show();
};

}; // namespace SpaceArena

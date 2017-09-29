// (C) 2009 Yoshy
// Реализация класса HUDDebug - отладочный элемент.
// Отображает полезную инфу.

#include "hud_debug.h"

namespace SpaceArena {

#define OVERLAY_NAME "HUD/debugOverlay"

HUDDebug::HUDDebug(Universe* universe, Ogre::RenderWindow* window) {
    show();
    _universe = universe;
    _window = window;
};

void HUDDebug::update() {
    int shipsCount = 0, planetsCount = 0;
    Ogre::String planetNames = "";
    Ogre::String shipNames = "";
    SpaceObject* so;

    Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/CurrFps")->setCaption("FPS: " + Ogre::StringConverter::toString(_window->getLastFPS()));
    if ((so = _universe->getFirstSpaceObject()) != NULL)
    {
        while (so != NULL)
        {
            if (so->getType() == SO_ASTEROID) { planetsCount++; planetNames += so->getName() + ", "; };
            if (so->getType() == SO_SPACESHIP) { shipsCount++; shipNames += so->getName() + ", "; };
            so = _universe->getCurrentSpaceObject();
        };
		if (planetNames.size() >= 2) planetNames.resize(planetNames.size() - 2);
		if (shipNames.size() >= 2) shipNames.resize(shipNames.size() - 2);
        // Objects count
        Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/NumPlanets")->setCaption("Asteroids: " + Ogre::StringConverter::toString(planetsCount) + " (" + planetNames + ")");
        Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/NumShips")->setCaption("Ships: " + Ogre::StringConverter::toString(shipsCount) + " (" + shipNames + ")");
        // Players spaceship parameters
		if (_universe->getSpaceObjectByID(PLAYER_SHIP_ID) != NULL)
		{
			//Ogre::Quaternion q = static_cast<SpaceShip*>(_universe->getSpaceObjectByID(PLAYER_SHIP_ID))->getSceneNodeOrientation();
			//Ogre::Real aa = static_cast<SpaceShip*>(_universe->getSpaceObjectByID(PLAYER_SHIP_ID))->getAngleAcceleration();
			//Ogre::Real av = static_cast<SpaceShip*>(_universe->getSpaceObjectByID(PLAYER_SHIP_ID))->getAngleVelocity();
			//Ogre::Vector3 at = static_cast<SpaceShip*>(_universe->getSpaceObjectByID(PLAYER_SHIP_ID))->getAcceleration();
			//Ogre::Vector3 v = static_cast<SpaceShip*>(_universe->getSpaceObjectByID(PLAYER_SHIP_ID))->getVelocity();
			//Ogre::Vector3 dir = static_cast<SpaceShip*>(_universe->getSpaceObjectByID(PLAYER_SHIP_ID))->getOrientation();
			//Ogre::Real coldtime = static_cast<SpaceShip*>(_universe->getSpaceObjectByID(PLAYER_SHIP_ID))->getColdTime();
			Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/text1")->setCaption("");
			Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/text2")->setCaption("");
			Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/text3")->setCaption("");
			Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/text4")->setCaption("");
			Ogre::OverlayManager::getSingleton().getOverlayElement(OVERLAY_NAME"/text5")->setCaption("");
		};
    };
};

void HUDDebug::show() {
    Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->show();
};

void HUDDebug::hide() {
    Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->hide();
};

void HUDDebug::changeVisibility()

{
	Ogre::Overlay* O = Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME);
    if (O->isVisible()) O->hide(); else O->show();
};

}; // namespace SpaceArena
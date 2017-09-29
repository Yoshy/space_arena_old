/*! 
	\class SpaceArena::HUDShipStats
    \brief HUD - Индикаторы корабля.

    На индикаторах корабля отображаются важнейшие показатели - заряд конденсатора, щитов и целостность корпуса.
*/

#include "hud_stats.h"

namespace SpaceArena {

#define SHIPSTATS_OVERLAY_NAME "HUD/shipstatsOverlay"
#define SHIPSTATS_OVERLAY_CONTAINER_NAME "HUD/shipstats"
#define ENERGY_ELEMENT_NAME "HUD/shipstats/energyBar"
#define SHIELD_ELEMENT_NAME "HUD/shipstats/shieldBar"
#define INTACT_ELEMENT_NAME "HUD/shipstats/intactBar"
#define TOPSTATUSBAR_OVERLAY_NAME "HUD/topStatusBarOverlay"
#define TOPSTATUSBAR_OVERLAY_CONTAINER_NAME "HUD/topStatusBar"
#define TOPSTATUSBAR_TEXTAREA1_NAME "HUD/topStatusBar/statusText1"
#define TOPSTATUSBAR_TEXTAREA2_NAME "HUD/topStatusBar/statusText2"

HUDShipStats::HUDShipStats(Player* player)
{
	_player = player;
    show();
};

void HUDShipStats::update()
{
    Ogre::Overlay* shipStatsOverlay = Ogre::OverlayManager::getSingleton().getByName(SHIPSTATS_OVERLAY_NAME);
    Ogre::OverlayContainer* shipStatsContainer = shipStatsOverlay->getChild(SHIPSTATS_OVERLAY_CONTAINER_NAME);
	Ogre::OverlayElement* statBar;

	// update intact
	Ogre::Real shipCurToughness, shipMaxToughness;
	if (_player->_ship == NULL) {
		shipCurToughness = 0;
		shipMaxToughness = 1;
	}
	else
	{
		shipCurToughness = _player->_ship->getCurToughness();
		shipMaxToughness = _player->_ship->getMaxToughness();
	};
	statBar = shipStatsContainer->getChild(INTACT_ELEMENT_NAME);
	statBar->setWidth(shipCurToughness * 100 / shipMaxToughness);
	
	// update capacitorCharge & shieldsCharge
	Ogre::Real shipCapacitorCharge;
	Ogre::Real shipShieldsCharge;
	Ogre::Real statBarMagnitude;
	if (_player->_ship == NULL)
	{
		shipCapacitorCharge = 0;
		shipShieldsCharge = 0;
		statBarMagnitude = 1;
	}
	else
	{
		shipCapacitorCharge = _player->_ship->getCapacitorCharge();
		shipShieldsCharge = _player->_ship->getShieldsCharge();
		statBarMagnitude = 100 / (POWER_CHARGE_FACTOR * _player->_ship->getPower());
	};
	statBar = shipStatsContainer->getChild(ENERGY_ELEMENT_NAME);
	statBar->setWidth(shipCapacitorCharge * statBarMagnitude);
	statBar = shipStatsContainer->getChild(SHIELD_ELEMENT_NAME);
	statBar->setWidth(shipShieldsCharge * statBarMagnitude);

	// update top status bar
	Ogre::OverlayElement* statusText1 = Ogre::OverlayManager::getSingleton().getByName(TOPSTATUSBAR_OVERLAY_NAME)->getChild(TOPSTATUSBAR_OVERLAY_CONTAINER_NAME)->getChild(TOPSTATUSBAR_TEXTAREA1_NAME);
	Ogre::OverlayElement* statusText2 = Ogre::OverlayManager::getSingleton().getByName(TOPSTATUSBAR_OVERLAY_NAME)->getChild(TOPSTATUSBAR_OVERLAY_CONTAINER_NAME)->getChild(TOPSTATUSBAR_TEXTAREA2_NAME);
	if (_player->_ship != NULL)
	{
		FireMode fireMode = _player->_ship->getFireMode();
		Ogre::String fireModeString;
		Ogre::String currentWeaponSlotWeaponName = _player->_ship->getEquipmentSlot(_player->_ship->getSlotNum(_player->_ship->getCurrentWeaponSlot()))->getInstalledEquipment()->getName();
		switch (fireMode)
		{
			case FM_MANUAL: fireModeString = "Manual fire with "; break;
			case FM_ROUNDROBIN: fireModeString = "Round robin fire, current "; break;
			case FM_VOLLEY_ALL: fireModeString = "Volley fire with ALL weapon"; currentWeaponSlotWeaponName = ""; break;
			case FM_VOLLEY_GROUP: fireModeString = "Volley fire with "; break;
			default: break;
		};
		Ogre::Real coldTime = _player->_ship->getColdTime();
		statusText1->setCaption("Fire mode: " + fireModeString + currentWeaponSlotWeaponName);
		statusText2->setCaption("Cold time: " + Ogre::StringConverter::toString(coldTime, 1));
	}
	else
	{
		statusText1->setCaption("Your ship is destroyed");
		statusText2->setCaption("");
	};
};

void HUDShipStats::show()
{
    Ogre::OverlayManager::getSingleton().getByName(SHIPSTATS_OVERLAY_NAME)->show();
    Ogre::OverlayManager::getSingleton().getByName(TOPSTATUSBAR_OVERLAY_NAME)->show();
};

void HUDShipStats::hide()

{
    Ogre::OverlayManager::getSingleton().getByName(SHIPSTATS_OVERLAY_NAME)->hide();
    Ogre::OverlayManager::getSingleton().getByName(TOPSTATUSBAR_OVERLAY_NAME)->hide();
};

void HUDShipStats::changeVisibility()

{
	Ogre::Overlay* O;
	O = Ogre::OverlayManager::getSingleton().getByName(SHIPSTATS_OVERLAY_NAME);
    if (O->isVisible()) O->hide(); else O->show();
	O = Ogre::OverlayManager::getSingleton().getByName(TOPSTATUSBAR_OVERLAY_NAME);
    if (O->isVisible()) O->hide(); else O->show();
};

}; // namespace SpaceArena

// (C) 2010 Yoshy
/*! 
	\class SpaceArena::HUDSheme
    \brief Схема оборудования корабля.

    На схеме оборудования корабля отображается количество и расположение слотов
    оборудования, а также оборудование в них установленное.
*/

#include "hud_sheme.h"

namespace SpaceArena {

#define OVERLAY_NAME "Equipment/ShipShemeOverlay"
#define OVERLAY_SIZE 256 // Overlay is square-shaped

HUDSheme::HUDSheme(Ogre::RenderWindow* window, Player* player)
{
	EquipmentSlot* slot;
	HoldSlot* holdSlot;
	Ogre::OverlayElement* overlayElement;
	Ogre::OverlayElement* slotTextElement;

	_player = player;
	_window = window;

	if (_player->_ship == NULL) return;

	// Set ship sheme overlay centered on the screen
	Ogre::Real posX = (_window->getWidth()/2 - OVERLAY_SIZE/2);
	Ogre::Real posY = (_window->getHeight()/2 - OVERLAY_SIZE/2);
	Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->setPosition(posX, posY);
	Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipShemeHoldAmountText")->setPosition(posX, posY);
	// set appropriate sheme for players spaceship
	Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("shemes/" + _player->_ship->getShipShemeName() + ".png");

	// Create ship slots
	for (Ogre::uint i = 0; i < _player->_ship->getNumSlots(); i++)
	{
		// Slot square
		slot = _player->_ship->getEquipmentSlot(i);
		overlayElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("Equipment/Slot", "Panel", "slot" + Ogre::StringConverter::toString(i));
		overlayElement->setPosition(slot->getShemePosition().x, slot->getShemePosition().y);
		Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->addChild(overlayElement);

		// Show letter in empty slot
		if (slot->getInstalledEquipment() == NULL)
		{
			slotTextElement = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "slotText" + Ogre::StringConverter::toString(i));
			slotTextElement->setMetricsMode(Ogre::GMM_PIXELS);
			slotTextElement->setPosition(slot->getShemePosition().x + 5, slot->getShemePosition().y);
			slotTextElement->setDimensions(32, 32);
			slotTextElement->setParameter("font_name", "Sector17");
			slotTextElement->setParameter("char_height", "25");
			switch (slot->getType())
			{
				case EQ_DRIVE: slotTextElement->setCaption("D"); break;
				case EQ_ENERGY_CELL: slotTextElement->setCaption("E"); break;
				case EQ_WEAPON: slotTextElement->setCaption("W"); break;
			};
			Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->addChild(slotTextElement);
		} else // show icon of installed equipment in non-empy slot
		{
			overlayElement = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "equipment" + Ogre::StringConverter::toString(i));
			overlayElement->setMetricsMode(Ogre::GMM_PIXELS);
			overlayElement->setPosition(slot->getShemePosition().x, slot->getShemePosition().y);
			overlayElement->setDimensions(32, 32);
			overlayElement->setMaterialName("Equipment/" + slot->getInstalledEquipment()->getID());
			Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->addChild(overlayElement);
		};
	};
	// Create hold slots (only 8 first slots)
	for (Ogre::uint i = 0; i < 8; i++)
	{
		overlayElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("Equipment/Slot", "Panel", "holdSlot" + Ogre::StringConverter::toString(i));
		overlayElement->setPosition(i*32, 256);
		Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->addChild(overlayElement);
		// Display equipment in slot
		holdSlot = _player->_ship->getHoldSlot(i);
		if (holdSlot == NULL) continue;
		if (holdSlot->getInstalledEquipment() != NULL)
		{ // show icon of installed equipment in non-empy slot
			overlayElement = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "equipmentOnHold" + Ogre::StringConverter::toString(i));
			overlayElement->setMetricsMode(Ogre::GMM_PIXELS);
			overlayElement->setPosition(i*32, 256);
			overlayElement->setDimensions(32, 32);
			overlayElement->setMaterialName("Equipment/" + holdSlot->getInstalledEquipment()->getID());
			Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->addChild(overlayElement);
		};
		slotTextElement = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "holdSlotText" + Ogre::StringConverter::toString(i));
		slotTextElement->setMetricsMode(Ogre::GMM_PIXELS);
		slotTextElement->setPosition(i*32 + 5, 259);
		slotTextElement->setDimensions(32, 32);
		slotTextElement->setParameter("font_name", "Digital");
		slotTextElement->setParameter("char_height", "13");
		slotTextElement->setCaption(Ogre::StringConverter::toString(holdSlot->getAmount()));
		Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipShemeHoldAmountText")->addChild(slotTextElement);
	};
	show();
};

void HUDSheme::update()
{
	if (!Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->isVisible()) return; // save the cycles
	HoldSlot* holdSlot;
	Ogre::OverlayElement* overlayElement;
	Ogre::OverlayElement* slotTextElement;

	// Update hold slots (only 8 first slots)
	for (Ogre::uint i = 0; i < 8; i++)
	{
		Ogre::OverlayManager::getSingleton().destroyOverlayElement("holdSlot" + Ogre::StringConverter::toString(i), false);
		overlayElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("Equipment/Slot", "Panel", "holdSlot" + Ogre::StringConverter::toString(i));
		overlayElement->setPosition(i*32, 256);
		Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->addChild(overlayElement);
		// Display equipment in slot
		holdSlot = _player->_ship->getHoldSlot(i);
		if (holdSlot == NULL) continue;
		try	{ Ogre::OverlayManager::getSingleton().destroyOverlayElement("equipmentOnHold" + Ogre::StringConverter::toString(i)); } catch(...) {};
		if (holdSlot->getInstalledEquipment() != NULL)
		{ // show icon of installed equipment in non-empy slot
			overlayElement = Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "equipmentOnHold" + Ogre::StringConverter::toString(i));
			overlayElement->setMetricsMode(Ogre::GMM_PIXELS);
			overlayElement->setPosition(i*32, 256);
			overlayElement->setDimensions(32, 32);
			overlayElement->setMaterialName("Equipment/" + holdSlot->getInstalledEquipment()->getID());
			Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipSheme")->addChild(overlayElement);
		};
		try	{ Ogre::OverlayManager::getSingleton().destroyOverlayElement("holdSlotText" + Ogre::StringConverter::toString(i), false); } catch(...) {};
		if (holdSlot->getAmount() != 0)
		{
			slotTextElement = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "holdSlotText" + Ogre::StringConverter::toString(i));
			slotTextElement->setMetricsMode(Ogre::GMM_PIXELS);
			slotTextElement->setPosition(i*32 + 5, 259);
			slotTextElement->setDimensions(32, 32);
			slotTextElement->setParameter("font_name", "Digital");
			slotTextElement->setParameter("char_height", "13");
			slotTextElement->setCaption(Ogre::StringConverter::toString(holdSlot->getAmount()));
			Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->getChild("Equipment/ShipShemeHoldAmountText")->addChild(slotTextElement);
		};
	};
};

void HUDSheme::show()
{
    Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->show();
};

void HUDSheme::hide()

{
    Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME)->hide();
};

void HUDSheme::changeVisibility()

{
	Ogre::Overlay* O = Ogre::OverlayManager::getSingleton().getByName(OVERLAY_NAME);
    if (O->isVisible()) O->hide(); else O->show();
};

}; // namespace SpaceArena

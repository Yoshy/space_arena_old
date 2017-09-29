// (C) 2009 Yoshy
// Реализация класса HUD - Head-Up Display.
// HUD содержит коллекцию HUDElement ответственен за их отображение.

#include "hud.h"

namespace SpaceArena {

HUD::~HUD()
{
    pHUDElement HE;
    HUDElementListIterator iterator;

    if (_HUDElements.empty()) return;
    for (iterator = _HUDElements.begin(); iterator != _HUDElements.end(); iterator++)
    {
       	HE = *iterator;
        if (HE != NULL) delete HE;
    }
}

void HUD::init()
{
}

void HUD::addHUDElement(HUDElement* he)
{
	_HUDElements.push_front(he);
}

void HUD::update()
{
    pHUDElement HE;
    HUDElementListIterator iterator;

    if (_HUDElements.empty()) return;
    for (iterator = _HUDElements.begin(); iterator != _HUDElements.end(); iterator++)
    {
       	HE = *iterator;
        HE->update();
    }
}

void HUD::show()
{
    pHUDElement HE;
    HUDElementListIterator iterator;

    if (_HUDElements.empty()) return;
    for (iterator = _HUDElements.begin(); iterator != _HUDElements.end(); iterator++)
    {
       	HE = *iterator;
        HE->show();
    }
}

void HUD::hide()
{
    pHUDElement HE;
    HUDElementListIterator iterator;

    if (_HUDElements.empty()) return;
    for (iterator = _HUDElements.begin(); iterator != _HUDElements.end(); iterator++)
    {
       	HE = *iterator;
        HE->hide();
    }
}

}; // namespace SpaceArena
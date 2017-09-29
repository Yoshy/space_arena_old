// (C) 2010 Yoshy
/*! 
    \class SpaceArena::Equipment
    \brief Оборудование кораблей.

    Оборудование кораблей - двигатели, оружие, энергоблоки.
*/

#include "equipment.h"

namespace SpaceArena {

std::map<Ogre::String, Equipment*> equipments;

EquipmentSlot::EquipmentSlot(const EquipmentSlot& src)
{
	_installedEquipment = src._installedEquipment;
	_shemePosition = src._shemePosition;
	_type = src._type;
};

void EquipmentSlot::installEquipment(Ogre::String ID)
{
	if ((ID == "") || (equipments[ID] == NULL)) { _installedEquipment = NULL; return; }
	if (_type == equipments[ID]->getType())
		_installedEquipment = equipments[ID];
};

Equipment::Equipment(const Equipment& src)
{
	_ID = src._ID;
	_type = src._type;
	_name = src._name;
	_description = src._description;
};

void Equipment::operator =(const Equipment& other)
{
	_ID = other._ID;
	_type = other._type;
	_name = other._name;
	_description = other._description;
};

} // namespace SpaceArena
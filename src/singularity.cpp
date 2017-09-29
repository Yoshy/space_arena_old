// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::Singularity
    \brief Singularity - origin of everything in universe.

	From the singularity appear all space objects and everything else in the big bang.
*/

#include "singularity.h"

namespace SpaceArena {

Singularity::Singularity(Ogre::SceneManager* sceneManager, NxScene* nxScene) {
    _sceneManager = sceneManager;
	_nxScene = nxScene;
};

Singularity::~Singularity()
{
};

void Singularity::initResources()
{
	char *XMLBuffer = NULL;
	rapidxml::xml_document<>* XMLDocument;
	Ogre::String s, name, description, ID;
	EQUIPMENT_TYPE type;
	SpaceObjectTemplatesMap& soTemplates = spaceObjectTemplates;
	SpaceObjectTemplate tmpTemplate;

	// create equipment database
	XMLDocument = loadAndParseXMLData(EQUIPMENT_DATA_FILENAME, XMLBuffer);
	rapidxml::xml_node<>* equipmentsNode = XMLDocument->first_node("equipments");
	for (rapidxml::xml_node<>* equipmentNode = equipmentsNode->first_node("equipment"); equipmentNode; equipmentNode = equipmentNode->next_sibling("equipment"))
	{
		// create Equipment object
		name = equipmentNode->first_attribute("name")->value();
		description = equipmentNode->first_attribute("description")->value();
		ID = equipmentNode->first_attribute("ID")->value();
		s = equipmentNode->first_attribute("type")->value();
		if (s == "EQ_DRIVE") 
		{
			type = EQ_DRIVE;
			equipments[ID] = new DriveEquipment(ID, type, name, description);
			if (equipmentNode->first_node("longitudinalThrust") != NULL)
				static_cast<DriveEquipment*>(equipments[ID])->setLongitudinalThrust(Ogre::StringConverter::parseReal(equipmentNode->first_node("longitudinalThrust")->value()));
			if (equipmentNode->first_node("transversalThrust") != NULL)
				static_cast<DriveEquipment*>(equipments[ID])->setTransversalThrust(Ogre::StringConverter::parseReal(equipmentNode->first_node("transversalThrust")->value()));
		};
		if (s == "EQ_ENERGY_CELL") 
		{
			type = EQ_ENERGY_CELL;
			equipments[ID] = new EnergyEquipment(ID, type, name, description);
			if (equipmentNode->first_node("power") != NULL)
				static_cast<EnergyEquipment*>(equipments[ID])->setPower(Ogre::StringConverter::parseReal(equipmentNode->first_node("power")->value()));
		};
		if (s == "EQ_WEAPON")
		{
			type = EQ_WEAPON;
			equipments[ID] = new WeaponEquipment(ID, type, name, description);
			if (equipmentNode->first_node("ammunitionID") != NULL)
				static_cast<WeaponEquipment*>(equipments[ID])->setAmmunitionID(equipmentNode->first_node("ammunitionID")->value());
			if (equipmentNode->first_node("objectTemplate") != NULL)
				static_cast<WeaponEquipment*>(equipments[ID])->setTemplateName(equipmentNode->first_node("objectTemplate")->value());
			if (equipmentNode->first_node("firePeriod") != NULL)
				static_cast<WeaponEquipment*>(equipments[ID])->setFirePeriod(Ogre::StringConverter::parseReal(equipmentNode->first_node("firePeriod")->value()));
			if (equipmentNode->first_node("ttl") != NULL)
				static_cast<WeaponEquipment*>(equipments[ID])->setTTL(Ogre::StringConverter::parseReal(equipmentNode->first_node("ttl")->value()));
		};
		if (s == "EQ_AMMO")
		{
			type = EQ_AMMO;
			equipments[ID] = new AmmunitionEquipment(ID, type, name, description);
			if (equipmentNode->first_node("cartridges") != NULL)
				static_cast<AmmunitionEquipment*>(equipments[ID])->setNumCartridges(Ogre::StringConverter::parseReal(equipmentNode->first_node("cartridges")->value()));
			if (equipmentNode->first_node("thrust") != NULL)
				static_cast<AmmunitionEquipment*>(equipments[ID])->setThrust(Ogre::StringConverter::parseReal(equipmentNode->first_node("thrust")->value()));
			if (equipmentNode->first_node("explosiveEnergy") != NULL)
				static_cast<AmmunitionEquipment*>(equipments[ID])->setExplosiveEnergy(Ogre::StringConverter::parseReal(equipmentNode->first_node("explosiveEnergy")->value()));
		};
		// create material for equipment
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Equipment/" + ID, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::Technique *technique = material->createTechnique();
		technique->createPass();
		material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		material->getTechnique(0)->getPass(0)->createTextureUnitState(ID+".png");
	};
	// delete unnecessary XML data from memory
	delete XMLDocument; delete XMLBuffer;
	// initialise space object templates database
	XMLDocument = loadAndParseXMLData(TEMPLATES_DATA_FILENAME, XMLBuffer);
	rapidxml::xml_node<> *templatesRoot = XMLDocument->first_node("SpaceObjectTemplates");
	for (rapidxml::xml_node<> *templateNode = templatesRoot->first_node("SpaceObjectTemplate"); templateNode; templateNode = templateNode->next_sibling("SpaceObjectTemplate"))
	{
		s = templateNode->first_attribute("type")->value();
		if (s == "SO_SPACESHIP")
		{
			tmpTemplate.type = SO_SPACESHIP;
			tmpTemplate.shipShemeName = templateNode->first_attribute("spaceshipShemeName")->value();
			// load slot data
			for (rapidxml::xml_node<> *slotNode = templateNode->first_node("Slot"); slotNode; slotNode = slotNode->next_sibling("Slot"))
			{
				s = slotNode->first_attribute("type")->value();
				type = EQ_UNKNOWN;
				if (s == "EQ_DRIVE") type = EQ_DRIVE;
				if (s == "EQ_ENERGY_CELL") type = EQ_ENERGY_CELL;
				if (s == "EQ_WEAPON") type = EQ_WEAPON;
				if (type == EQ_UNKNOWN) throw "Illegal slot type while parsing game data";
				Ogre::Real xpos = Ogre::StringConverter::parseReal(slotNode->first_attribute("xpos")->value());
				Ogre::Real ypos = Ogre::StringConverter::parseReal(slotNode->first_attribute("ypos")->value());
				s = slotNode->first_attribute("installed")->value();
				EquipmentSlot* tmpSlot = new EquipmentSlot(type);
				tmpSlot->setShemePosition(Ogre::Vector3(xpos, ypos, 0));
				tmpSlot->installEquipment(s);
				tmpTemplate.equipmentSlots.push_back(tmpSlot);
			};
			// load hold data
			for (rapidxml::xml_node<> *holdNode = templateNode->first_node("Hold"); holdNode; holdNode = holdNode->next_sibling("Hold"))
			{
				s = holdNode->first_attribute("type")->value();
				type = EQ_UNKNOWN;
				if (s == "EQ_DRIVE") type = EQ_DRIVE;
				if (s == "EQ_ENERGY_CELL") type = EQ_ENERGY_CELL;
				if (s == "EQ_WEAPON") type = EQ_WEAPON;
				if (s == "EQ_AMMO") type = EQ_AMMO;
				if (type == EQ_UNKNOWN) throw "Illegal slot type while parsing game data";
				s = holdNode->first_attribute("installed")->value();
				Ogre::Real amount = Ogre::StringConverter::parseReal(holdNode->first_attribute("amount")->value());
				HoldSlot* tmpSlot = new HoldSlot();
				tmpSlot->setType(type);
				tmpSlot->installEquipment(s, amount);
				tmpTemplate.holdSlots.push_back(tmpSlot);
			};
		};
		if (s == "SO_ASTEROID") tmpTemplate.type = SO_ASTEROID;
		if (s == "SO_PROJECTILE") tmpTemplate.type = SO_PROJECTILE;
		tmpTemplate.name = templateNode->first_attribute("name")->value();
		tmpTemplate.meshName = templateNode->first_attribute("meshName")->value();
		tmpTemplate.materialName = templateNode->first_attribute("materialName")->value();
		s = templateNode->first_attribute("meshScale")->value(); tmpTemplate.meshScale = Ogre::StringConverter::parseReal(s);
		s = templateNode->first_attribute("boundingSphereScale")->value(); tmpTemplate.boundingSphereScale = Ogre::StringConverter::parseReal(s);
		s = templateNode->first_attribute("orientation_x")->value(); tmpTemplate.orientation_x = Ogre::StringConverter::parseReal(s);
		s = templateNode->first_attribute("orientation_y")->value(); tmpTemplate.orientation_y = Ogre::StringConverter::parseReal(s);
		s = templateNode->first_attribute("orientation_z")->value(); tmpTemplate.orientation_z = Ogre::StringConverter::parseReal(s);
		s = templateNode->first_attribute("orientation_w")->value(); tmpTemplate.orientation_w = Ogre::StringConverter::parseReal(s);
		s = templateNode->first_attribute("density")->value(); tmpTemplate.density = Ogre::StringConverter::parseReal(s);
		soTemplates[tmpTemplate.name] = tmpTemplate;
		tmpTemplate.equipmentSlots.clear();
		tmpTemplate.holdSlots.clear();
	};
	// delete unnecessary XML data from memory
	delete XMLDocument; delete XMLBuffer;
};

void Singularity::initUniverse(SpaceObjectManager& spaceObjects)
{
	char* XMLBuffer = NULL;
	rapidxml::xml_document<>* worldXMLdocument;
	SpaceObject* object;
	Ogre::uint ID = spaceObjects.getNewID();
	Ogre::String s, name, templateName;
	Ogre::Real x, y, direction_x, direction_y, toughness;
	SpaceObjectTemplatesMap& soTemplates = spaceObjectTemplates;

	worldXMLdocument = loadAndParseXMLData(WORLD_DATA_FILENAME, XMLBuffer);
	rapidxml::xml_node<> *spaceDataNode = worldXMLdocument->first_node("World")->first_node("SpaceData");
	for (rapidxml::xml_node<> *spaceObjectNode = spaceDataNode->first_node("SpaceObject"); spaceObjectNode; spaceObjectNode = spaceObjectNode->next_sibling("SpaceObject"))
	{
		name = spaceObjectNode->first_attribute("name")->value();
		s = spaceObjectNode->first_attribute("x")->value(); x = Ogre::StringConverter::parseReal(s);
		s = spaceObjectNode->first_attribute("y")->value(); y = Ogre::StringConverter::parseReal(s);
		s = spaceObjectNode->first_attribute("direction_x")->value(); direction_x = Ogre::StringConverter::parseReal(s);
		s = spaceObjectNode->first_attribute("direction_y")->value(); direction_y = Ogre::StringConverter::parseReal(s);
		s = spaceObjectNode->first_attribute("toughness")->value(); toughness = Ogre::StringConverter::parseReal(s);
		templateName = spaceObjectNode->first_attribute("template")->value();
		switch (soTemplates[templateName].type)
		{
            case SO_ASTEROID:
                object = (SpaceObject*) new Asteroid(
					ID, _sceneManager, _nxScene,
					soTemplates[templateName].meshName,
					soTemplates[templateName].materialName,
					name,
					soTemplates[templateName].meshScale,
					soTemplates[templateName].boundingSphereScale,
					Ogre::Vector3(x, y, 0),
					Ogre::Vector3(direction_x, direction_y, 0),
                    Ogre::Quaternion(soTemplates[templateName].orientation_x, soTemplates[templateName].orientation_y, soTemplates[templateName].orientation_z, soTemplates[templateName].orientation_w),
                    soTemplates[templateName].density,
					toughness);
                break;
            case SO_SPACESHIP:
                object = (SpaceObject*) new SpaceShip(
					ID, _sceneManager, _nxScene,
					soTemplates[templateName].shipShemeName,
					soTemplates[templateName].meshName,
					soTemplates[templateName].materialName,
					name,
					soTemplates[templateName].meshScale,
					soTemplates[templateName].boundingSphereScale,
					Ogre::Vector3(x, y, 0),
					Ogre::Vector3(direction_x, direction_y, 0),
					Ogre::Quaternion(soTemplates[templateName].orientation_x, soTemplates[templateName].orientation_y, soTemplates[templateName].orientation_z, soTemplates[templateName].orientation_w),
                    soTemplates[templateName].density,
					toughness);
				if (!soTemplates[templateName].equipmentSlots.empty()) {
					for (std::vector<EquipmentSlot*>::iterator i = soTemplates[templateName].equipmentSlots.begin(); i != soTemplates[templateName].equipmentSlots.end(); i++)
					{
						EquipmentSlot tmpSlot(EQ_UNKNOWN);
						tmpSlot = *(*i);
						static_cast<SpaceShip*>(object)->addSlot(tmpSlot);
					};
					static_cast<SpaceShip*>(object)->calculateEquipmentParameters();
				};
				if (!soTemplates[templateName].holdSlots.empty()) {
					for (std::vector<HoldSlot*>::iterator i = soTemplates[templateName].holdSlots.begin(); i != soTemplates[templateName].holdSlots.end(); i++)
					{
						static_cast<SpaceShip*>(object)->putToTheHold((*i)->getInstalledEquipment()->getID(), (*i)->getAmount());
					};
				};
				break;
            default:
				Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: While parsing, wrong 'spaceObject' type in world.xml ");
                throw "Wrong SpaceObject type in world.xml";
                break;
        };
		spaceObjects.addObject(object);
		ID = spaceObjects.getNewID();
	};
	delete worldXMLdocument; delete XMLBuffer;
};

rapidxml::xml_document<>* Singularity::loadAndParseXMLData(Ogre::String fileName, char* XMLBuffer)
{
	int len;
	std::fstream XMLFile(fileName.c_str(), std::ios::in);
	rapidxml::xml_document<>* XMLDocument = new rapidxml::xml_document<>();
	if (!XMLFile.is_open()) throw "Error opening " + fileName;
	// Get file length
	XMLFile.seekg(0, std::ios_base::end);
	len = XMLFile.tellg();
	XMLFile.seekg(0, std::ios_base::beg);
	// Allocate memory
	XMLBuffer = new char[len]; memset(XMLBuffer, 0, len);
	// and read file
    XMLFile.read(XMLBuffer, len);
	XMLFile.close();
	XMLDocument->parse<rapidxml::parse_full>(XMLBuffer);
	return XMLDocument;
};

}; // namespace SpaceArena
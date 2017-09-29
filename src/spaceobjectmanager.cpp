/*! 
    \class SpaceArena::SpaceObjectManager
    \brief Менеджер космических объектов.

    Менеджер космических объектов управляет всеми космическими объектами
    во вселенной - добавляет, удаляет, перемещает и т.п.
*/

#include "spaceobjectmanager.h"

namespace SpaceArena {

SpaceObjectTemplatesMap spaceObjectTemplates;

void SpaceObjectManager::addObject(SpaceObject* pObject) {
	pObject->_managedBy = this;
	_spaceObjects.push_front(pObject);
}

SpaceObject* SpaceObjectManager::getObjectByID(int ID) {
	SpaceObject* pObject;
    SpaceObjectListIterator iterator;

	if(!_spaceObjects.empty())
	{
		for(iterator = _spaceObjects.begin(); iterator != _spaceObjects.end(); iterator++)
		{
			pObject = *iterator;
			if (pObject->_id == ID) return pObject;
		}
	}
    return NULL;
}

SpaceObjectType SpaceObjectManager::getObjectType(int ID) {
	SpaceObject* pObject;
    SpaceObjectListIterator iterator;

	if(!_spaceObjects.empty())
	{
		for(iterator = _spaceObjects.begin(); iterator != _spaceObjects.end(); iterator++)
		{
			pObject = *iterator;
            if (pObject->_id == ID) return pObject->_type;
		}
	}
    return SO_INVALID;
};

SpaceObject* SpaceObjectManager::getFirstObject(void)
{
    if (_spaceObjects.empty()) return NULL;
    _currentObject = _spaceObjects.begin();
	while ((*_currentObject)->getCurToughness() == 0)
	{
		_currentObject++;
		if (_currentObject == _spaceObjects.end()) return NULL;
	};
    return *_currentObject;
}

SpaceObject* SpaceObjectManager::getCurrentObject(void)
{
    _currentObject++;
    if (_currentObject == _spaceObjects.end()) return NULL;
	while ((*_currentObject)->getCurToughness() == 0)
	{
		_currentObject++;
		if (_currentObject == _spaceObjects.end()) return NULL;
	};
    return *_currentObject;
}

void SpaceObjectManager::processObjects(Ogre::Real timeDelta)
{
	SpaceObject *pObject;
    SpaceObjectListIterator iterator;

    if (_spaceObjects.empty()) return;
	// iterate through all objects
    for (iterator = _spaceObjects.begin(); iterator != _spaceObjects.end(); iterator++)
    {
        pObject = *iterator;
		// skip destroyed objects
		if (pObject->getCurToughness() == 0)
		{
			pObject->_isSafeToDelete = true;
			continue;
		};
		// move objects
		pObject->setOgreSceneNodePosition(toOgre(pObject->_nxActor->getGlobalPosition()));
		pObject->setOgreSceneNodeOrientation(toOgre(pObject->_nxActor->getGlobalOrientationQuat()) * pObject->_baseOrientation);
		if (pObject->getType() == SO_PROJECTILE)
		{
			// check and update projectile ttl
			Ogre::Real ttl = static_cast<Projectile*>(pObject)->getRespectiveWeapon().getTTL();
			ttl -= timeDelta;
			if (ttl <= 0) pObject->_isSafeToDelete = true;
			static_cast<Projectile*>(pObject)->getRespectiveWeapon().setTTL(ttl);
		}
		if (pObject->getType() == SO_SPACESHIP)
		{
			// update weapon cold timer
			static_cast<SpaceShip*>(pObject)->setColdTime(static_cast<SpaceShip*>(pObject)->getColdTime() - timeDelta);
			if (static_cast<SpaceShip*>(pObject)->getColdTime() < 0) static_cast<SpaceShip*>(pObject)->setColdTime(0);
			// update ship charging elements
			Ogre::Real chargeRate = static_cast<SpaceShip*>(pObject)->getPower() * timeDelta * 2;
			static_cast<SpaceShip*>(pObject)->setCapacitorCharge(static_cast<SpaceShip*>(pObject)->getCapacitorCharge() + chargeRate);
			static_cast<SpaceShip*>(pObject)->setShieldsCharge(static_cast<SpaceShip*>(pObject)->getShieldsCharge() + chargeRate);
		};
    };
};

void SpaceObjectManager::purgeObjects()
{
	SpaceObject *pObject;
	SpaceObjectListIterator iterator;
	
	iterator = _spaceObjects.begin();
	while (iterator != _spaceObjects.end())
	{
		pObject = *iterator;
		if (pObject->_isSafeToDelete)
		{
    		_spaceObjects.erase(iterator);
			delete pObject;
			iterator = _spaceObjects.begin();
		}
		else
		{
			iterator++;
		};
	};
};

void SpaceObjectManager::clear()
{
	SpaceObject* pObject;
	if(!_spaceObjects.empty())
	{
		for(SpaceObjectListIterator iterator = _spaceObjects.begin(); iterator != _spaceObjects.end(); iterator++)
		{
			pObject = *iterator;
            if (pObject) delete pObject;
		};
	};
    _spaceObjects.clear();
};

SpaceObjectManager::~SpaceObjectManager()
{
	clear();
};

}; // namespace SpaceArena
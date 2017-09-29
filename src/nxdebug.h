// (C) 2009 Yoshy
// Интерфейс класса NxDebugRenderer - визуализатор отладочной информации PhysX.
// Визуализация векторов сил, точек колизий, осей и т.п.

#ifndef _NXDEBUG_H_
#define _NXDEBUG_H_

#include "ogre.h"
#include "nxphysics.h"

namespace SpaceArena {

class NxDebugRenderer
{
public: 
    NxDebugRenderer(Ogre::SceneManager* sm);
    ~NxDebugRenderer();
    // рендеринг дебага .... рендерим только линии ... 
    void renderData(const NxDebugRenderable* data);
    inline void clear(){ _renderObject->clear(); }
private:
    Ogre::SceneNode* _node;
    Ogre::SceneManager* _sm;
    Ogre::ManualObject* _renderObject;  
    Ogre::ColourValue _color;
};

};
#endif _NXDEBUG_H_

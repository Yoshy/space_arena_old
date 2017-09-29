// (C) 2009 Yoshy
// Реализация класса NxDebugRenderer - визуализатор отладочной информации PhysX.
// Визуализация векторов сил, точек колизий, осей и т.п.

#include "nxdebug.h"

namespace SpaceArena {

NxDebugRenderer::NxDebugRenderer (Ogre::SceneManager* sm) : _sm(sm), _node(sm->getRootSceneNode()->createChildSceneNode())
{
    _renderObject = _sm->createManualObject("NX_DEBUG_RENDERER_OBJECT");
    _node->attachObject(_renderObject); 
}

NxDebugRenderer::~NxDebugRenderer ()
{
    _node->detachAllObjects();
    _node->getParentSceneNode()->removeAndDestroyChild(_node->getName()); 
    _sm->destroyManualObject(_renderObject);
}

void NxDebugRenderer::renderData(const NxDebugRenderable* data)
{
    _renderObject->clear();

    NxU32 numPoints = data->getNbPoints();
    NxU32 numLines = data->getNbLines();
    NxU32 numTriangles = data->getNbTriangles();
    if (numLines > 0)
    {
        const NxDebugLine* lines = data->getLines();
        _renderObject->begin("" ,Ogre::RenderOperation::OT_LINE_LIST);
        NxU32 color;
        const NxVec3 *p;
        while(numLines--)
        {
            color = lines->color;
            _color.r = NxF32((color >> 16) & 0xFF) / 255.0f;
            _color.g = NxF32((color >> 8) & 0xFF) / 255.0f;
            _color.b = NxF32((color) & 0xFF) / 255.0f;
            p = &lines->p0;
            _renderObject->position(p->x, p->y, p->z);
            _renderObject->colour(_color);
            p = &lines->p1;
            _renderObject->position(p->x, p->y, p->z);
            _renderObject->colour(_color);
            lines++;
        };
    _renderObject->end();
    };
};

};
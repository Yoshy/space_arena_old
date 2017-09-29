// (C) 2009 Yoshy
// ��������� ������ NxDebugRenderer - ������������ ���������� ���������� PhysX.
// ������������ �������� ���, ����� �������, ���� � �.�.

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
    // ��������� ������ .... �������� ������ ����� ... 
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

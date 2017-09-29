// (C) 2009-2010 Yoshy
/*! 
	\class SpaceArena::SpaceArena
    \brief Main game class.

    Main game class, entry point, mediator between subjective and virtual reality of the project,
	primary arbitrator between other project classes.
	Основной класс игры, точка входа, посредник между субъективной и
    виртуальной реальностью проекта, главный арбитр между остальными
    классами проекта.
*/

#ifndef _SPACEARENA_H_
#define _SPACEARENA_H_

#include "ogre.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "OIS.h"
#include "nxphysics.h"
#include <fstream>
#include "rapidxml.hpp"

#include "globals.h"
#include "nxdebug.h"
#include "nxcontactor.h"
#include "player.h"
#include "universe.h"
#include "hud.h"
#include "hud_debug.h"
#include "hud_radar.h"
#include "hud_stats.h"
#include "hud_sheme.h"

namespace SpaceArena {

class SpaceArena : public Ogre::FrameListener, OIS::KeyListener, OIS::MouseListener
{
public:
    SpaceArena() : _nxPhysicsSDK(NULL), _nxScene(NULL), _ogreRoot(NULL), _ogreSceneManager(NULL), _ogreCamera(NULL), _ogreFrameListener(NULL), _ogreWindow (NULL), _inputManager(NULL), _lastFrameTime(0) { };
    ~SpaceArena();
    bool init();
    bool createScene();
    void run();
    bool frameStarted(const Ogre::FrameEvent& e);
    bool frameEnded(const Ogre::FrameEvent& e);

private:
	/// Prepare game to shutdown
    void shutdown();
	/// OIS events
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
    bool mouseMoved(const OIS::MouseEvent &e );
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    void processUnbufferedInput();
	/// MyGUI callbacks
	void GUIButtonCallback(MyGUI::WidgetPtr _sender);

    bool _isRunning;
    bool _isCameraFollowPlayerShip;

    // Объекты Ogre3D
    Ogre::Root* _ogreRoot;
    Ogre::SceneManager* _ogreSceneManager;
    Ogre::Camera* _ogreCamera;
    Ogre::FrameListener* _ogreFrameListener;
    Ogre::RenderWindow* _ogreWindow;

    // Объекты OIS
    OIS::Mouse* _mouse;
    OIS::Keyboard* _keyboard;
    OIS::InputManager* _inputManager;

    // Объекты PhysX
    NxPhysicsSDK* _nxPhysicsSDK;
    NxScene* _nxScene;
    NxDebugRenderer* _nxDebugRenderer;
	NxContactor _nxContactor;
    
    // Внутриигровые объекты
	MyGUI::Gui* _GUI;
	MyGUI::OgrePlatform* _myGUIOgrePlatform;
    Universe _universe;
    GameState _gameState;
    HUD _HUD;
	HUDElement* _playerShipSheme;
    Player* _player;
    Ogre::Real _lastFrameTime;
};

}; // namespace SpaceArena

#endif // _SPACEARENA_H_

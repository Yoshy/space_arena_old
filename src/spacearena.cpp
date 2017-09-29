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

#include "spacearena.h"
#include <direct.h>

namespace SpaceArena {

SpaceArena::~SpaceArena()
{
    _universe.release();

	_GUI->shutdown();
	delete _GUI;
	_myGUIOgrePlatform->shutdown();
	delete _myGUIOgrePlatform;

	if (_nxDebugRenderer) delete _nxDebugRenderer;
    if (_nxPhysicsSDK && _nxScene) _nxPhysicsSDK->releaseScene(*_nxScene);
    if (_nxPhysicsSDK) _nxPhysicsSDK->release();

    if (_ogreFrameListener) delete _ogreFrameListener;
    if (_ogreRoot) delete _ogreRoot;
    if (_inputManager)
    {
        _inputManager->destroyInputObject(_mouse);
        _inputManager->destroyInputObject(_keyboard);
        OIS::InputManager::destroyInputSystem(_inputManager);
        _inputManager = 0;
    };
};
bool SpaceArena::init() {
    Ogre::ConfigFile cf;
    Ogre::String secName, typeName, archName;

    _ogreRoot = new Ogre::Root();
	cf.load("resources.cfg");
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        };
    };
    if (_ogreRoot->showConfigDialog())
    {
        _ogreWindow = _ogreRoot->initialise(true, "Космическая арена "SPACE_ARENA_VERSION);
    }
    else
    {
        return false;
    };
    _ogreSceneManager = _ogreRoot->createSceneManager(Ogre::ST_GENERIC, "MainSceneManager");
    _ogreCamera = _ogreSceneManager->createCamera("MainCamera");
    _ogreCamera->setPosition(Ogre::Vector3(CAMERA_INITIAL_X, CAMERA_INITIAL_Y, CAMERA_INITIAL_Z));
    _ogreCamera->lookAt(Ogre::Vector3(CAMERA_INITIAL_X, CAMERA_INITIAL_Y, 0));
    //_ogreCamera->setNearClipDistance(5);
    // Create one viewport, entire window
    Ogre::Viewport* vp = _ogreWindow->addViewport(_ogreCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    _ogreRoot->addFrameListener(this);
    _ogreSceneManager->setSkyBox(true, "SpaceArena/SpaceSkyBox");
	Ogre::LogManager::getSingletonPtr()->logMessage("INFO: Ogre core initialized");

    // Initialise OIS
    OIS::ParamList pl;	
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    unsigned int width, height, depth;
    int left, top;

    _ogreWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    _inputManager = OIS::InputManager::createInputSystem( pl );

    //Create all devices
    _keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject(OIS::OISKeyboard, true));
    _keyboard->setEventCallback(this);
    _mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, true));
    _mouse->setEventCallback(this);
    _ogreWindow->getMetrics(width, height, depth, left, top);
    //Set Mouse Region.. if window resizes, we should alter this to reflect as well
    const OIS::MouseState &ms = _mouse->getMouseState();
    ms.width = width;
    ms.height = height;
	Ogre::LogManager::getSingletonPtr()->logMessage("INFO: 'OIS' subsystem initialized");

    // Initialise physics engine
    _nxPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if (!_nxPhysicsSDK) {
		MessageBox(0, "Unable to initialize PhysX engine", "Error", MB_OK);
		Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Unable to initialize PhysX engine");
		return false;
	};
	_nxPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);
    _nxPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 10);
    _nxPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
    _nxPhysicsSDK->setParameter(NX_VISUALIZE_BODY_LIN_VELOCITY, 1);
    _nxPhysicsSDK->setParameter(NX_VISUALIZE_BODY_ANG_VELOCITY, 1);
    _nxPhysicsSDK->setParameter(NX_VISUALIZE_WORLD_AXES, 1);
    _nxPhysicsSDK->setParameter(NX_VISUALIZE_BODY_AXES, 1);
    _nxPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
	// Collision detection debug
	_nxPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1);
	_nxPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_FORCE, 1);
    NxSceneDesc sceneDesc;
    sceneDesc.simType = NX_SIMULATION_SW;
    sceneDesc.groundPlane = false;
	sceneDesc.gravity.set(0, 0, 0);
	sceneDesc.userContactReport	= &_nxContactor;
    _nxScene = _nxPhysicsSDK->createScene(sceneDesc);
    if (!_nxScene) return false;
	_nxScene->setActorGroupPairFlags(0, 0, NX_NOTIFY_ON_START_TOUCH|NX_NOTIFY_FORCES);
	_nxScene->setTiming( 1.0f / 60.0f, 8, NX_TIMESTEP_FIXED );
	// Set default material properties
	NxMaterial * defaultMaterial = _nxScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.0f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);
	// Initialise debug renderer
    _nxDebugRenderer = new NxDebugRenderer(_ogreSceneManager);
	Ogre::LogManager::getSingletonPtr()->logMessage("INFO: PhysX engine initialized");

    // Initialise content
    if (!createScene()) return false;
    _universe.bigBang(_ogreSceneManager, _nxScene);
	Ogre::LogManager::getSingletonPtr()->logMessage("INFO: Singularity has been exploded");

    // Create player
    SpaceShip* playerShip = static_cast<SpaceShip*>(_universe.getSpaceObjectByID(PLAYER_SHIP_ID));
	if (playerShip == NULL)
		throw "spaceObject 'player ship' not found";
    // at, an, aa, vm, vam
    playerShip->_isSafeToDelete = false;
    _player = new Player("Player", playerShip);
	_universe.addCharacter(_player);
    _isCameraFollowPlayerShip = true;

	// Initialise GUI
	_myGUIOgrePlatform = new MyGUI::OgrePlatform();
	_myGUIOgrePlatform->initialise(_ogreWindow, _ogreSceneManager);
	_GUI = new MyGUI::Gui();
	_GUI->initialise();
	MyGUI::ButtonPtr button = _GUI->createWidget<MyGUI::Button>("Button", width-110, height-36, 100, 26, MyGUI::Align::Default, "Main", "ExitButton");
	button->setCaption("exit");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &SpaceArena::GUIButtonCallback);

    // Initialise HUD
    _HUD.addHUDElement(new HUDDebug(&_universe, _ogreWindow));
    _HUD.addHUDElement(new HUDRadar(&_universe, _ogreWindow, _ogreCamera));
    _HUD.addHUDElement(new HUDShipStats(_player));
    _playerShipSheme = new HUDSheme(_ogreWindow, _player); _HUD.addHUDElement(_playerShipSheme);
	_playerShipSheme->hide();
    _HUD.init();
	Ogre::LogManager::getSingletonPtr()->logMessage("INFO: HUD initialized");

	// Let's go
    _gameState = GS_INGAME;
    return true;
};
bool SpaceArena::createScene()
{
    _ogreSceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));
    return true;
};

void SpaceArena::run()
{
    _isRunning = true;
    _ogreRoot->startRendering();
};

bool SpaceArena::frameStarted(const Ogre::FrameEvent& e)
{
	switch (_gameState)
    {
    case GS_INGAME:
		// Просчитываем физическую сцену
		_nxScene->simulate(_lastFrameTime);
		_nxScene->flushStream();
		_nxScene->fetchResults(NX_ALL_FINISHED, true);
		// И обновляем графическую сцену, согласно результатам обсчета физики
        _universe.process(_lastFrameTime);
		_HUD.update();
        _nxDebugRenderer->renderData(_nxScene->getDebugRenderable());
		//if (!_player->_isAlive) isRunning = false;
        break;
    case GS_PAUSE:
        break;
    case GS_MENU:
        break;
    default:
        break;
    };
	return _isRunning;
};

bool SpaceArena::frameEnded(const Ogre::FrameEvent& e)
{
    _mouse->capture();
    _keyboard->capture();
    _lastFrameTime = e.timeSinceLastFrame;

    processUnbufferedInput();
    return _isRunning;
};
bool SpaceArena::keyPressed(const OIS::KeyEvent &e)
{
	// Transfer data to MyGUI subsystem
	_GUI->injectKeyPress(MyGUI::KeyCode::Enum(e.key), e.text);
	// Process keys
	if (e.key == OIS::KC_ESCAPE) _isRunning = false;
    if (e.key == OIS::KC_F12) _isCameraFollowPlayerShip = !_isCameraFollowPlayerShip;
	if (e.key == OIS::KC_E) _playerShipSheme->changeVisibility();
    if (e.key == OIS::KC_P)
    {
        if (_gameState == GS_INGAME)
        {
            _gameState = GS_PAUSE;
        } else
        {
            _gameState = GS_INGAME;
        };
    };
	Ogre::uint shipControlFlag = 0;
	if (e.key == OIS::KC_0) shipControlFlag |= SC_ROUNDROBIN; // select round robin fire mode
	if (e.key == OIS::KC_GRAVE) shipControlFlag |= SC_VOLLEY; // select volley fire mode
	if (e.key == OIS::KC_1) shipControlFlag |= SC_MANUAL_1; // select manual fire mode with slot 1
	if (e.key == OIS::KC_2)	shipControlFlag |= SC_MANUAL_2; // select manual fire mode with slot 2
	if (e.key == OIS::KC_3)	shipControlFlag |= SC_MANUAL_3; // select manual fire mode with slot 3
	if (e.key == OIS::KC_4)	shipControlFlag |= SC_MANUAL_4; // select manual fire mode with slot 4
	if (e.key == OIS::KC_5)	shipControlFlag |= SC_MANUAL_5; // select manual fire mode with slot 5
	if (e.key == OIS::KC_6)	shipControlFlag |= SC_MANUAL_6; // select manual fire mode with slot 6
	if (e.key == OIS::KC_7)	shipControlFlag |= SC_MANUAL_7; // select manual fire mode with slot 7
	if (e.key == OIS::KC_8)	shipControlFlag |= SC_MANUAL_8; // select manual fire mode with slot 8
	if (e.key == OIS::KC_9)	shipControlFlag |= SC_MANUAL_9; // select manual fire mode with slot 9
	_player->shipControl(shipControlFlag, _lastFrameTime);
    return true;
};
bool SpaceArena::keyReleased(const OIS::KeyEvent &e)
{
	// Transfer data to MyGUI subsystem
	_GUI->injectKeyRelease(MyGUI::KeyCode::Enum(e.key));
    return true;
};
bool SpaceArena::mouseMoved(const OIS::MouseEvent &e )
{
	// Transfer data to MyGUI subsystem
	_GUI->injectMouseMove(e.state.X.abs, e.state.Y.abs, e.state.Z.abs);
	// Zoom using mouse wheel
    if (e.state.Z.rel > 0)
    {
        if (_ogreCamera->getPosition().z > CAMERA_MIN_Z) _ogreCamera->moveRelative(Ogre::Vector3(0, 0, -CAMERA_STEP*CAMERA_STEP));
    };
    if (e.state.Z.rel < 0)
    {
        if (_ogreCamera->getPosition().z < CAMERA_MAX_Z) _ogreCamera->moveRelative(Ogre::Vector3(0, 0, CAMERA_STEP*CAMERA_STEP));
    };
    // Move camera using right mouse button
    if (e.state.buttonDown(OIS::MB_Right))
    {
        _ogreCamera->moveRelative(Ogre::Vector3(-e.state.X.rel, e.state.Y.rel, 0));
    };

    return true;
};
bool SpaceArena::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	// Transfer data to MyGUI subsystem
	_GUI->injectMousePress(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
    return true;
};
bool SpaceArena::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	// Transfer data to MyGUI subsystem
	_GUI->injectMouseRelease(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
    return true;
};
void SpaceArena::processUnbufferedInput()
{
    // Camera movement (arrow keys, numpad +, numpad -, numpad *)
    if (_keyboard->isKeyDown(OIS::KC_LEFT)) {
        _ogreCamera->moveRelative(Ogre::Vector3(-CAMERA_STEP, 0, 0));
    };
    if (_keyboard->isKeyDown(OIS::KC_RIGHT)) {
        _ogreCamera->moveRelative(Ogre::Vector3(CAMERA_STEP, 0, 0));
    };
    if (_keyboard->isKeyDown(OIS::KC_UP)) {
        _ogreCamera->moveRelative(Ogre::Vector3(0, CAMERA_STEP, 0));
    };
    if (_keyboard->isKeyDown(OIS::KC_DOWN)) {
        _ogreCamera->moveRelative(Ogre::Vector3(0, -CAMERA_STEP, 0));
    };
    if (_keyboard->isKeyDown(OIS::KC_SUBTRACT)) {
        if (_ogreCamera->getPosition().z < CAMERA_MAX_Z) _ogreCamera->moveRelative(Ogre::Vector3(0, 0, CAMERA_STEP));
    };
    if (_keyboard->isKeyDown(OIS::KC_ADD)) {
        if (_ogreCamera->getPosition().z > CAMERA_MIN_Z) _ogreCamera->moveRelative(Ogre::Vector3(0, 0, -CAMERA_STEP));
    };
    if ((_keyboard->isKeyDown(OIS::KC_MULTIPLY)) || (_isCameraFollowPlayerShip)) {
		Ogre::Vector3 pos = _player->getSpaceShipPosition();
		if (pos != Ogre::Vector3::ZERO) _ogreCamera->setPosition(Ogre::Vector3(pos.x, pos.y, _ogreCamera->getPosition().z));
    };
    // Player ship movement
	Ogre::uint shipControlFlag = 0;
    if (_keyboard->isKeyDown(OIS::KC_NUMPAD8)) { // forward traction
        shipControlFlag |= SC_TRACTION_FORWARD;
    };
    if (_keyboard->isKeyDown(OIS::KC_NUMPAD2)) { // backward traction
        shipControlFlag |= SC_TRACTION_BACKWARD;
    };
    if (_keyboard->isKeyDown(OIS::KC_NUMPAD4)) { // rotate counter clockwise
        shipControlFlag |= SC_TURN_LEFT;
    };
    if (_keyboard->isKeyDown(OIS::KC_NUMPAD6)) { // rotate clockwise
        shipControlFlag |= SC_TURN_RIGHT;
    };
    if (_keyboard->isKeyDown(OIS::KC_NUMPAD7)) { // displace left
        shipControlFlag |= SC_DISPLACE_LEFT;
    };
    if (_keyboard->isKeyDown(OIS::KC_NUMPAD9)) { // displace right
        shipControlFlag |= SC_DISPLACE_RIGHT;
    };
    if (_keyboard->isKeyDown(OIS::KC_NUMPAD0)) { // stabilize
        shipControlFlag |= SC_STABILIZE;
	};
    if (_keyboard->isKeyDown(OIS::KC_SPACE)) { // fire
		shipControlFlag |= SC_FIRE;
    };
    _player->shipControl(shipControlFlag, _lastFrameTime);
};

void SpaceArena::GUIButtonCallback(MyGUI::WidgetPtr _sender)
{
	Ogre::String wName = _sender->getName();
	if (wName == "ExitButton") shutdown();
};

void SpaceArena::shutdown()
{
    _isRunning = false;
};

}; // namespace SpaceArena
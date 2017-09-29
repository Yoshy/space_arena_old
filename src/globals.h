/// \file globals.h √лобальные определени€, параметры и перечислени€.

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/// ¬ерси€ major.minor.patch
#define SPACE_ARENA_VERSION "0.2.4"

namespace SpaceArena {

/// XML file with game data
#define WORLD_DATA_FILENAME "../../data/world/world.xml"
#define EQUIPMENT_DATA_FILENAME "../../data/world/equipment.xml"
#define TEMPLATES_DATA_FILENAME "../../data/world/objecttemplates.xml"

/// "положительное сколь угодно малое вещественное число"
#define EPSILON 1e-09

/// ќграничени€ дл€ камеры
#define CAMERA_INITIAL_X 0
#define CAMERA_INITIAL_Y 0
#define CAMERA_INITIAL_Z 5000
#define CAMERA_MIN_Z 500
#define CAMERA_MAX_Z 7000
#define CAMERA_STEP 20

//
#define PLAYER_SHIP_ID 0

/// Ship control flags
#define SC_TRACTION_FORWARD  0x00000001
#define SC_TRACTION_BACKWARD 0x00000002
#define SC_TURN_LEFT         0x00000004
#define SC_TURN_RIGHT        0x00000008
#define SC_DISPLACE_LEFT     0x00000010
#define SC_DISPLACE_RIGHT    0x00000020
#define SC_STABILIZE         0x00000040
#define SC_FIRE              0x00000080
#define SC_ROUNDROBIN        0x00000100
#define SC_VOLLEY            0x00000200
#define SC_MANUAL_1          0x00000400
#define SC_MANUAL_2          0x00000800
#define SC_MANUAL_3          0x00001000
#define SC_MANUAL_4          0x00002000
#define SC_MANUAL_5          0x00004000
#define SC_MANUAL_6          0x00008000
#define SC_MANUAL_7          0x00010000
#define SC_MANUAL_8          0x00020000
#define SC_MANUAL_9          0x00040000

/// SpaceShip::_currentWeaponSlot takes this value, when there is no appropriate slot
/// i.e. there is no weapon on spaceship
#define INVALID_WEAPON_SLOT    0xFFFFFFFF

/// Fire modes
enum FireMode {FM_MANUAL, FM_ROUNDROBIN, FM_VOLLEY_ALL, FM_VOLLEY_GROUP};

/// Default projectile toughness
#define PROJECTILE_TOUGHNESS 10

/// Power factor for shields and capacitor.
/// This factor multiplied to cumulative power of all installed power cells determine the maximum charge of shields and capacitor.
#define POWER_CHARGE_FACTOR 100

/// Game states
enum GameState {GS_MENU, GS_INGAME, GS_PAUSE};

/// Space objects types
enum SpaceObjectType {SO_INVALID, SO_ASTEROID, SO_SPACESHIP, SO_PROJECTILE};

/// —огласование структур Ogre3D и PhysX
inline Ogre::Vector3 toOgre(const NxVec3 &v)
{
    return Ogre::Vector3(v.x, v.y, v.z);
};

inline NxVec3 toNx(const Ogre::Vector3 &v)
{
    return NxVec3(v.x, v.y, v.z);
};

inline Ogre::Quaternion toOgre(const NxQuat &q)
{
    return Ogre::Quaternion(q.w, q.x, q.y, q.z);
};

inline NxQuat toNx(Ogre::Quaternion& q, bool _normalise = true)
{
	if (_normalise)	q.normalise();
	NxQuat a;	a.x = q.x;	a.y = q.y;	a.z = q.z;	a.w = q.w;
	return a;
};

}; // namespace SpaceArena

#endif // _GLOBALS_H_
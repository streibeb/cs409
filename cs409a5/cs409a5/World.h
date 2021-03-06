//
//  World.h
//

#ifndef WORLD_H
#define WORLD_H

#include "../../ObjLibrary/Vector3.h"

#include "ExplosionManagerInterface.h"
#include "WorldInterface.h"
#include "GeometricCollisions.h"
#include "Planetoid.h"
#include "RingSystem.h"
#include "Ship.h"
#include "Bullet.h"

//
//  World
//
//  A simple class that implements the WorldInterface interface.
//    This class contains a system for managing explosions, but
//    nothing for managing rings or physics objects.  Explosions
//    can be added to the world, but other add functions only
//    print a message to std::out saying would have been added.
//
//  The files for the ExplosionManager associated with this
//    World must be loaded after GLUT is initialized
//    but before draw() is called.  This can be done by calling
//
//      world.init();
//
//    between the glutInit() and glutMainLoop() functions.
//
//  Class Invariant():
//    <1> mp_explosion_manager != NULL
//

class World : public WorldInterface
{
private:
    static const unsigned int MOON_COUNT = 10;
    static const unsigned int SHIP_COUNT = 250;
    static const unsigned int BULLET_COUNT = 100;
    
    struct objInfo
    {
        std::string filename;
        float radius;
        Vector3 position;
    };
    
    objInfo skyboxInfo = {
        "Models/Skybox.obj", 1.f, {0.f, 0.f, 0.f}
    };
    
    objInfo planetInfo = {
        "Models/Saturn.obj", 60000.f, {0.f, 0.0f, 0.0f}
    };
    
    objInfo moonInfo[MOON_COUNT] = {
        {"Models/MoonA.obj", 3300.f, {0.f, 0.f, 140000.f}},
        {"Models/MoonB.obj", 4300.f, {85000.f, 0.f, 75000.f}},
        {"Models/MoonC.obj", 2000.f, {130000.f, 0.f, 40000.f}},
        {"Models/MoonD.obj", 3400.f, {110000.f, 0.f, -60000.f}},
        {"Models/MoonE.obj", 5000.f, {100000.f, 0.f, -70000.f}},
        {"Models/MoonF.obj", 3100.f, {20000.f, 0.f, -135000.f}},
        {"Models/MoonG.obj", 2600.f, {-60000.f, 0.f, -80000.f}},
        {"Models/MoonH.obj", 2200.f, {-95000.f, 0.f, -70000.f}},
        {"Models/MoonI.obj", 4700.f, {-90000.f, 0.f, -40000.f}},
        {"Models/MoonJ.obj", 3800.f, {-100000.f, 0.f, 50000.f}}
    };

    objInfo ringInfo = {
        "Models/Ring.obj", 60000.f, {0.0f, 0.f, 0.0f}
    };

    // So explosions look nice on Windows
#ifdef _WIN32
    const std::string EXPLOSION_FILENAME = "Explode1.png";
#else
    const std::string EXPLOSION_FILENAME = "Explode1.bmp";
#endif
    
    const double RING_MOON_PADDING      =   2.0e3;
    const double RING_HALF_THICKNESS    =  12.0e3;
    const double RING_INNER_RADIUS      =  75.0e3;
    const double RING_OUTER_RADIUS_BASE = 150.0e3;
    const double RING_DENSITY_MAX       = 6.0;
    const double RING_DENSITY_FACTOR    = 0.0002;
    
public:
    Ship player_ship;
    
private:
    DisplayList skybox;
    DisplayList ring_list;
    Planetoid planet;
    Planetoid moons[MOON_COUNT];
    RingSystem g_rings;
    Ship ships[SHIP_COUNT];
    Bullet bullets[BULLET_COUNT];
    int nextBullet = 0;
    
    DisplayList planet_dl;
    DisplayList moon_dl[MOON_COUNT];
    DisplayList ring_dl;
    DisplayList ship_dl;
    DisplayList bullet_dl;
    
public:
//
//  Default Constructor
//
//  Purpose: To create an World.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new World is created.
//

	World ();

//
//  Copy Constructor
//
//  Purpose: To create an World as a copy of another.
//  Parameter(s):
//    <1> original: The World to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new World is created containing the
//               same explosions as original.
//

	World (const World& original);

//
//  Destructor
//
//  Purpose: To safely destroy an World without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this World is freed.
//

	virtual ~World ();

//
//  Assignment Operator
//
//  Purpose: To modify this World to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The World to copy
//  Precondition(s): N/A
//  Returns: A reference to this World.
//  Side Effect: Any existing explosions are removed.  A copy of
//               each expliosion in original is added to this
//               World.
//

	World& operator= (const World& original);

//
//  isInitialized
//
//  Purpose: To determine if this World has been
//           initialized.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this World is initialized.
//  Side Effect: N/A
//

	bool isInitialized () const;

//
//  draw
//
//  Purpose: To display everything in this Worlds.  In
//           this implementation, there are only explosions to
//           display.
//  Parameter(s):
//    <1> camera_forward: The camera forward vector
//    <2> camera_up: The camera up vector
//  Precondition(s):
//    <1> isInitialized()
//    <2> camera_forward.isNormal()
//    <3> camera_up.isNormal()
//    <4> camera_forward.isOrthogonal(camera_up)
//  Returns: N/A
//  Side Effect: All explosions are drawn.
//

	void draw (const Vector3& camera_forward,
	           const Vector3& camera_up) const;

//
//  init
//
//  Purpose: To initialize this World.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The ExplosionManager associated with this
//               World is initialized.  If this
//               World was laready initialized,
//               there is no effect.
//

	void init ();

//
//  reset
//
//  Purpose: To restore this World to its initial
//           initialized state.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: N/A
//  Side Effect: All explosions are removed.
//

	void reset ();

//
//  updateAll
//
//  Purpose: To update everything in this World for
//           one frame.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: N/A
//  Side Effect: All explosions are updated for one frame.
//

	void updateAll ();

///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from WorldInterface
//  Implemented in World2.h
//

//
//  getRingDensity
//
//  Purpose: To determine the average density of ring particles
//           near a position.  This function only checks the
//           average density value, not the individual ring
//           particles.  To get the individual ring particles,
//           use getRingParticles.
//  Parameter(s):
//    <1> position: THe position to query
//  Precondition(s): N/A
//  Returns: The density of ring particles at position position.
//           This is the expected number of particles within a
//           1 km cube.  The ring density varies across space,
//           however, so the real number of particles in a 1 km
//           cube may be different.  This implementation of the
//           function always returns 0.0.
//  Side Effect: N/A
//

	virtual double getRingDensity (const Vector3& position) const;

//
//  getRingParticles
//
//  Purpose: To determine the position and radius of all ring
//           particles intersecting the specified sphere.  This
//           function is much slower than getRingDensity.
//  Parameter(s):
//    <1> sphere_center: The center of the sphere
//    <2> sphere_radius: The radius of the sphere
//  Precondition(s):
//    <1> sphere_radius >= 0.0
//  Returns: A STL vector containing all ring particles within
//           a sphere of radius sphere_radius centered on
//           position sphere_center.  This includes particles
//           that are only partially within the sphere.  This
//           implementation of the function always returns an
//           empty vector.
//  Side Effect: N/A
//

	virtual std::vector<RingParticleData> getRingParticles (
	                               const Vector3& sphere_center,
	                               double sphere_radius) const;

//
//  getFleetCount
//
//  Purpose: To determine the number of fleets in the world.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of fleets in the world.  This includes
//           the "nature" fleet that PhysicsObjects such as
//           planetoids are on.  This implementation always
//           returns 0.
//  Side Effect: N/A
//

	virtual unsigned int getFleetCount () const;

//
//  getFleetScore
//
//  Purpose: To determine the score of the specified fleet.
//  Parameter(s):
//    <1> fleet: The fleet to query
//  Precondition(s):
//    <1> fleet <  getFleetCount()
//    <2> fleet != PhysicsObjectId::FLEET_NATURE
//  Returns: The current score of fleet fleet.  This
//           implementation always returns 0.0.
//  Side Effect: N/A
//

	virtual float getFleetScore (unsigned int fleet) const;

//
//  isFleetAlive
//
//  Purpose: To determine if the the specified fleet is
//           currently alive.  A fleet is said to be alive if
//           and only if its command ship is alive.
//  Parameter(s):
//    <1> fleet: The fleet to query
//  Precondition(s):
//    <1> fleet <  getFleetCount()
//    <2> fleet != PhysicsObjectId::FLEET_NATURE
//  Returns: Whether fleet fleet has a living command ship.
//           This implementation always returns false.
//  Side Effect: N/A
//

	virtual bool isFleetAlive (unsigned int fleet) const;

//
//  getFleetCommandShipId
//
//  Purpose: To determine the id for the command ship for fleet
//           fleet.
//  Parameter(s):
//    <1> fleet: The fleet to query
//  Precondition(s):
//    <1> fleet <  getFleetCount()
//    <2> fleet != PhysicsObjectId::FLEET_NATURE
//  Returns: The id for the command ship for fleet fleet.  This
//           implementation always returns ID_NOTHING.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getFleetCommandShipId (
	                                   unsigned int fleet) const;

//
//  getFleetFighterIds
//
//  Purpose: To determine the ids of the surviving fighters that
//           are part of fleet fleet.
//  Parameter(s):
//    <1> fleet: The fleet to query
//  Precondition(s):
//    <1> fleet <  getFleetCount()
//    <2> fleet != PhysicsObjectId::FLEET_NATURE
//  Returns: A STL vector of the ids for the living fighters in
//           fleet fleet.  This implementation always returns an
//           empty vector.
//  Side Effect: N/A
//

	virtual std::vector<PhysicsObjectId> getFleetFighterIds (
	                                   unsigned int fleet) const;

//
//  getFleetMissileIds
//
//  Purpose: To determine the ids of the missiles associated
//           with fleet fleet that are currently alive.
//  Parameter(s):
//    <1> fleet: The fleet to query
//  Precondition(s):
//    <1> fleet <  getFleetCount()
//    <2> fleet != PhysicsObjectId::FLEET_NATURE
//  Returns: A STL vector of the ids for the living missiles in
//           fleet fleet.  This implementation always returns an
//           empty vector.
//  Side Effect: N/A
//

	virtual std::vector<PhysicsObjectId> getFleetMissileIds (
	                                  unsigned int fleet) const;

//
//  getPlanetId
//
//  Purpose: To determine the id for the planet.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The id for the planet.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getPlanetId () const;

//
//  getMoonCount
//
//  Purpose: To determine the number of moons in the world.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of moons in the world.
//  Side Effect: N/A
//

	virtual unsigned int getMoonCount () const;

//
//  getMoonId
//
//  Purpose: To determine the id for the specified moon.
//  Parameter(s):
//    <1> moon: Which moon
//  Precondition(s):
//    <1> moon < getMoonCount()
//  Returns: The id for moon moon.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getMoonId (unsigned int moon) const;

//
//  getNearestPlanetoidId
//
//  Purpose: To determine the id of the planetoid nearest to the
//           specified position.  This function can be used for
//           collision avoidance.
//  Parameter(s):
//    <1> position: The position to check from
//  Precondition(s): N/A
//  Returns: The id of the planetoid closest to position
//           position.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getNearestPlanetoidId (
	                             const Vector3& position) const;

//
//  getShipIds
//
//  Purpose: To determine the ids of all ships within the
//           specified sphere.
//  Parameter(s):
//    <1> sphere_center: The center of the sphere
//    <2> sphere_radius: The radius of the sphere
//  Precondition(s):
//    <1> sphere_radius >= 0.0
//  Returns: A STL vector containing the ids of all ships within
//           a sphere of radius sphere_radius centered on
//           position sphere_center.  The same ids may appear in
//           the vector more than once.  This implementation
//           always returns an empty vector.
//  Side Effect: N/A
//

	virtual std::vector<PhysicsObjectId> getShipIds (
	                               const Vector3& sphere_center,
	                               double sphere_radius) const;

//
//  isAlive
//
//  Purpose: To determine if the specified id value identifies a
//           living PhysicsObject.
//  Parameter(s):
//    <1> id: The id to query
//  Precondition(s): N/A
//  Returns: true if id identifies any living thing in the world.
//           false if id never identified anything or if that
//           thing is now dead.
//  Side Effect: N/A
//

	virtual bool isAlive (const PhysicsObjectId& id) const;

//
//  getPosition
//
//  Purpose: To determine the position of the PhysicsObject with
//           the specified id.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: The position of PhysicsObject id.  This
//           implementation always returns the zero vector.
//  Side Effect: N/A
//

	virtual Vector3 getPosition (const PhysicsObjectId& id) const;

//
//  getRadius
//
//  Purpose: To determine the radius of the PhysicsObject with
//           the specified id.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: The radius of PhysicsObject id.  This
//           implementation always returns zero.
//  Side Effect: N/A
//

	virtual double getRadius (const PhysicsObjectId& id) const;

//
//  getVelocity
//
//  Purpose: To determine the velocity of the PhysicsObject with
//           the specified id.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: The velocity of PhysicsObject id.  This
//           implementation always returns the zero vector.
//  Side Effect: N/A
//

	virtual Vector3 getVelocity (const PhysicsObjectId& id) const;

//
//  getSpeed
//
//  Purpose: To determine the current speed of the PhysicsObject
//           with the specified id.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: The speed of PhysicsObject id.  This implementation
//           always returns zero.
//  Side Effect: N/A
//

	virtual double getSpeed (const PhysicsObjectId& id) const;

//
//  getForward
//
//  Purpose: To determine the direction that the PhysicsObject
//           with the specified id is facing.  All
//           PhysicsObjects have an orientation, even if it
//           isn't used for anything important.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: A unit vector pointing in the local forward
//           direction for PhysicsObject id.  This
//           implementation always returns Vector3::UNIT_X_PLUS.
//  Side Effect: N/A
//

	virtual Vector3 getForward (const PhysicsObjectId& id) const;

//
//  getUp
//
//  Purpose: To determine the local up direction for the
//           PhysicsObject with the specified id.  All
//           PhysicsObjects have an orientation, even if it
//           isn't used for anything important.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: A unit vector pointing in the local up direction
//           for PhysicsObject id.  This implementation always
//           returns Vector3::UNIT_Y_PLUS.
//  Side Effect: N/A
//

	virtual Vector3 getUp (const PhysicsObjectId& id) const;

//
//  getRight
//
//  Purpose: To determine the local right direction for the
//           PhysicsObject with the specified id.  All
//           PhysicsObjects have an orientation, even if it
//           isn't used for anything important.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: A unit vector pointing in the local right direction
//           for PhysicsObject id.
//  Side Effect: N/A
//

	virtual Vector3 getRight (const PhysicsObjectId& id) const;

//
//  isPlanetoidMoon
//
//  Purpose: To determine if the specified planetoid is a moon.
//  Parameter(s):
//    <1> id: The id to query
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_PLANETOID
//    <2> isAlive(id)
//  Returns: Whether id identifies a moon.
//  Side Effect: N/A
//

	virtual bool isPlanetoidMoon (const PhysicsObjectId& id) const;

//
//  getPlanetoidRingDistance
//
//  Purpose: To determine the distance from the center of the
//           specified planetoid to where the ring particles
//           begin.  All planetoids exist in holes in the ring.
//  Parameter(s):
//    <1> id: The id for the planetoid
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_PLANETOID
//    <2> isAlive(id)
//  Returns: The distance from the center of planetoid id to
//           the surface of the spherical hole in the ring,
//           including the radius of the planetoid.  If the
//           planetoid is a moon, this is also the distance a
//           command ship must be within to claim it.  There is
//           a bit of fuzziness in the ring density, so some
//           ring particles may be closer than this number.
//  Side Effect: N/A
//

	virtual double getPlanetoidRingDistance (
	                           const PhysicsObjectId& id) const;

//
//  getPlanetoidOwner
//
//  Purpose: To determine which fleet owns the specified
//           planetoid.
//  Parameter(s):
//    <1> id: The id for the planetoid
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_PLANETOID
//    <2> isAlive(id)
//  Returns: The current owner of planetoid id.  If planetoid id
//           is the planet or an unclaimed moon, this function
//           will return PhysicsObjectId::FLEET_NATURE.
//  Side Effect: N/A
//

	virtual unsigned int getPlanetoidOwner (
	                           const PhysicsObjectId& id) const;

//
//  isPlanetoidActivelyClaimed
//
//  Purpose: To determine if the owner of the specified
//           planetoid has a command ship close enougth to it to
//           claim ownership . Note that there may be guards
//           near the planetoid even if this functions return
//           false.
//  Parameter(s):
//    <1> id: The id for the planetoid
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_PLANETOID
//    <2> isAlive(id)
//  Returns: Whether the current owner of planetoid id
//           currently has a command ship close enougth to claim
//           it.  If no fleet owns planetoid id, this function
//           always returns false.
//  Side Effect: N/A
//

	virtual bool isPlanetoidActivelyClaimed (
	                           const PhysicsObjectId& id) const;

//
//  isShipCommandShip
//
//  Purpose: To determine if the ship with the specified id is a
//           command ship.
//  Parameter(s):
//    <1> id: The id for the ship
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_SHIP
//    <2> isAlive(id)
//  Returns: Whether ship id is a command ship.  Otherwise, ship
//           id is a fighter.
//  Side Effect: N/A
//

	virtual bool isShipCommandShip (
	                           const PhysicsObjectId& id) const;

//
//  getShipSpeedMax
//
//  Purpose: To determine the maximum speed of the ship with the
//           specified id.
//  Parameter(s):
//    <1> id: The id for the ship
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_SHIP
//    <2> isAlive(id)
//  Returns: The maximum speed of ship id.
//  Side Effect: N/A
//

	virtual double getShipSpeedMax (
	                           const PhysicsObjectId& id) const;

//
//  getShipAcceleration
//
//  Purpose: To determine the acceleration of the ship with the
//           specified id.  Note that the acceleration of a Ship
//           is only used for changes to speed, not for turning.
//  Parameter(s):
//    <1> id: The id for the ship
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_SHIP
//    <2> isAlive(id)
//  Returns: The maximum acceleration of ship id.
//  Side Effect: N/A
//

	virtual double getShipAcceleration (
	                           const PhysicsObjectId& id) const;

//
//  getShipRotationRate
//
//  Purpose: To determine the turning rate in radians per second
//           for the ship with the specified id.  This value is
//           only used for turning, not for changes to speed.
//  Parameter(s):
//    <1> id: The id for the ship
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_SHIP
//    <2> isAlive(id)
//  Returns: The maximum rotation rate of ship id in radians per
//           second.
//  Side Effect: N/A
//

	virtual double getShipRotationRate (
	                           const PhysicsObjectId& id) const;

//
//  getShipHealthCurrent
//
//  Purpose: To determine the current health of the ship with
//           the specified id.
//  Parameter(s):
//    <1> id: The id for the ship
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_SHIP
//    <2> isAlive(id)
//  Returns: The current health of ship id.
//  Side Effect: N/A
//

	virtual float getShipHealthCurrent (
	                           const PhysicsObjectId& id) const;

//
//  getShipHealthMaximum
//
//  Purpose: To determine the maximum health of the ship with
//           the specified id.
//  Parameter(s):
//    <1> id: The id for the ship
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_SHIP
//    <2> isAlive(id)
//  Returns: The maximum health of ship id.
//  Side Effect: N/A
//

	virtual float getShipHealthMaximum (
	                           const PhysicsObjectId& id) const;

//
//  isMissileOutOfFuel
//
//  Purpose: To determine the if the missile with the specified
//           id is out of fuel.
//  Parameter(s):
//    <1> id: The id for the missile
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_MISSILE
//    <2> isAlive(id)
//  Returns: Whether missile id has used up its fuel.  This
//           implementation always returns false.
//  Side Effect: N/A
//

	virtual bool isMissileOutOfFuel (
	                           const PhysicsObjectId& id) const;

//
//  getMissileTarget
//
//  Purpose: To determine the target that the missile with the
//           specified id is homing on.
//  Parameter(s):
//    <1> id: The id for the missile
//  Precondition(s):
//    <1> id.m_type == PhysicsObjectId::TYPE_MISSILE
//    <2> isAlive(id)
//  Returns: The target that missile id is homing on.  If the
//           missile is not homing on a target,
//           PhysicsObjectId::ID_NOTHING is returned.  This
//           implementation always returns
//           PhysicsObjectId::ID_NOTHING.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getMissileTarget (
	                           const PhysicsObjectId& id) const;

//
//  addExplosion
//
//  Purpose: To add an explosion to the world at the specified
//           position with the specified size.
//  Parameter(s):
//    <1> position: The position of the explosion
//    <2> size: The size of the explosion
//    <3> type: The type of the explosion
//  Precondition(s):
//    <1> size >= 0.0
//  Returns: N/A
//  Side Effect: An explosion of size size is added to this
//               World at position position.  The effect of the
//               type variable is implementation-defined,
//               although 0 must always be a valid type.  In
//               this implementation, the type parameter has no
//               effect.
//

	virtual void addExplosion (const Vector3& position,
	                           double size,
	                           unsigned int type);

//
//  addBullet
//
//  Purpose: To add a bullet to the world at the specified
//           position, moving in the specified diection, and
//           with the specified source.
//  Parameter(s):
//    <1> position: The initial position for the bullet
//    <2> forward: The direction the bullet is moving
//    <3> source_id: The id for the source that fired the bullet
//  Precondition(s):
//    <1> forward.isNormal()
//  Returns: The id of the new bullet.  If for some reason the
//           bullet was not added, PhysicsObjectId::ID_NOTHING
//           is returned.
//  Side Effect: A bullet is added to this World at position
//               position.  It is set to have been fired in
//               direction forward by the source with id
//               source_id.
//

	virtual PhysicsObjectId addBullet (
	                          const Vector3& position,
	                          const Vector3& forward,
	                          const PhysicsObjectId& source_id);

//
//  addMissile
//
//  Purpose: To add a missile to the world at the specified
//           position, moving in the specified diection, and
//           with the specified source and target.
//  Parameter(s):
//    <1> position: The initial position for the missile
//    <2> forward: The direction the missile is moving
//    <3> source_id: The id for the source that fired the
//                   missile
//    <4> target_id: The id for the missile target
//  Precondition(s):
//    <1> forward.isNormal()
//  Returns: The id of the new missile.  If for some reason the
//           missile was not added, PhysicsObjectId::ID_NOTHING
//           is returned.
//  Side Effect: A missile is added to this World at position
//               position.  It is set to have been fired in
//               direction forward by the source with id
//               source_id.  If
//               target_id == PhysicsObjectId::ID_NOTHING, the
//               missile is set not to home on any target.
//               Otherwise, it is set to home of the target
//               with id target_id.
//

	virtual PhysicsObjectId addMissile (
	                          const Vector3& position,
	                          const Vector3& forward,
	                          const PhysicsObjectId& source_id,
	                          const PhysicsObjectId& target_id);

private:
///////////////////////////////////////////////////////////////
//
//  Helper function not inherited from anywhere
//

//
//  invariant
//
//  Purpose: To determine if the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	ExplosionManagerInterface* mp_explosion_manager;
    
//
//  handleCollisions
//
//  Purpose: A function which handles all collisions
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: N/A
//

    void handleCollisions();

//
//  handleShipCollisions
//
//  Purpose: A function which handles all collisions for ships
//  Parameter(s):
//    <1> ship: The ship we are testing collisions against
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Identifies whether a collision has occurred
//               for the given ship and calls the appropriate
//               resolution function.
//
    
    void handleShipCollisions(Ship& ship);

//
//  handleBulletCollisions
//
//  Purpose: A function which handles all collisions for bullets
//  Parameter(s):
//    <1> bullet: The bullet we are testing collisions against
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Identifies whether a collision has occurred
//               for the given bullet and calls the appropriate
//               resolution function.
//

    void handleBulletCollisions(Bullet& bullet);

//
//  resolvePlanetoidCollision
//
//  Purpose: A function which resolves a collision between a
//           PhysicsObject and a Planetoid
//  Parameter(s):
//    <1> obj: The PhysicsObject which collided with a Planetoid
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Marks the PhysicsObject as "dying"
//
    
    void resolvePlanetoidCollision(PhysicsObject& obj);

//
//  resolveShipCollision
//
//  Purpose: A function which resolves a collision between two
//           Ships
//  Parameter(s):
//    <1> obj1: One of the ships involved in the collision
//    <2> obj2: The other ship involved in the collision
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Marks both ships as dying
//

    void resolveShipCollision(Ship& obj1, Ship& obj2);

//
//  resolveBulletCollision
//
//  Purpose: A function which resolves a collision between a
//           bullet and a ship
//  Parameter(s):
//    <1> b: The bullet involved in the collision
//    <2> obj: The ship involved in the collision
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Marks the bullet as dying and subtract 1
//               from the ship's health
//
    
    void resolveBulletCollision(Bullet& b, Ship& obj);
    
    void drawSkybox() const;
};



#endif

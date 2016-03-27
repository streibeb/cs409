//
//  WorldTestUnitAi.h
//

#ifndef WORLD_TEST_UNIT_AI_H
#define WORLD_TEST_UNIT_AI_H

#include "ObjLibrary/Vector3.h"

#include "Orrery.h"
#include "ExplosionManagerInterface.h"
#include "PhysicsObjectId.h"
#include "Ship.h"
#include "Bullet.h"
#include "SimpleMarker.h"
#include "WorldInterface.h"

class PhysicsObject;



//
//  WorldTestUnitAi
//
//  An implementation of the WorldInterface interface used to
//    test the moon guard unit AI.  The world implemented here
//    contains 1 moon, a number of ring particles, and 2 ships,
//    refered to as the target and the agent.
//
//  The target always has a position calculated according to an
//    Orrery.  It always has a current and maximum health of
//    1.0.  Whenever the target is hit, it will disappear for a
//    few seconds.  During that time, all queries will behave as
//    if it does not exist.
//
//  The agent is a Ship controlled by the unit AI.
//
//   MORE NEEDED <|>
//
//  All queries not related to the target return default values.
//    Unless otherwise stated, these are false for booleans, 0
//    for numbers, Vector3::ZERO for Vector3s, and empty vectors
//    for STL vectors.  Commands to add something to the world
//    have no effect.
//
//  Class Invariant:
//    <1> mp_explosion_manager != NULL
//    <2> m_ring_particle_count <= RING_PARTICLE_COUNT_MAX
//    <3> m_next_bullet <= BULLET_COUNT_MAX
//    <4> m_next_simple_marker <= SIMPLE_MARKER_COUNT_MAX
//

class WorldTestUnitAi : public WorldInterface
{
public:
//
//  ID_MOON
//
//  The id of the moon.  This id is always valid.
//

	static const PhysicsObjectId ID_MOON;

//
//  ID_TARGET
//
//  The id of the target.  This id is usually valid, but it
//    becomes invalid for a short time whenever the target is
//    hit.
//

	static const PhysicsObjectId ID_TARGET;

//
//  ID_AGENT
//
//  The id of the agent.  This id is always valid.
//

	static const PhysicsObjectId ID_AGENT;

//
//  RING_PARTICLE_COUNT_MAX
//
//  The maximum number of ring particles.
//

	static const unsigned int RING_PARTICLE_COUNT_MAX = 2500;

//
//  RING_PARTICLE_COUNT_DEFAULT
//
//  The number of ring particles in a newly-created
//    WorldTestUnitAi.
//

	static const unsigned int RING_PARTICLE_COUNT_DEFAULT = 1500;

//
//  RING_CLUMP_DISTANCE
//
//  How far the center of the ring particle clump is from the
//    origin.
//

	static const double RING_CLUMP_DISTANCE;

//
//  RING_CLUMP_RADIUS
//
//  The radius of the ring particle clump.
//

	static const double RING_CLUMP_RADIUS;

//
//  MOON_RADIUS
//
//  The radius of the moon at the center of the coordinate
//    system.
//

	static const double MOON_RADIUS;

//
//  TARGET_HIT_DISAPPEAR_TIME
//
//  The time in seconds that the target disappears for when it
//    is hit.
//

	static const double TARGET_HIT_DISAPPEAR_TIME;

//
//  BULLET_COUNT_MAX
//
//  The maximum number of bullets that can be in a
//    WorldTestUnitAi simultaneously.
//

	static const unsigned int BULLET_COUNT_MAX = 12 + 1;
	// one extra, just to be safe

//
//  SIMPLE_MARKER_COUNT_MAX
//
//  The maximum number of simple markers that can be in a
//    WorldTestUnitAi simultaneously.
//

	static const unsigned int SIMPLE_MARKER_COUNT_MAX = 10;

public:
//
//  Class Function: isModelsLoaded
//
//  Purpose: To determine of the models for the
//           WorldTestUnitAi class have been loaded.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the models have been loaded.
//  Side Effect: N/A
//

	static bool isModelsLoaded ();

//
//  Class Function: loadModels
//
//  Purpose: To load the models for the WorldTestUnitAi class.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> !isModelsLoaded()
//  Returns: N/A
//  Side Effect: The models for the WorldTestUnitAi class are
//               loaded.  These models are shared between all
//               instances of the class, so this function only
//               needs to be called once.  If the models for the
//               Orrery class have not been initialized, they
//               are also initialized.
//

	static void loadModels ();

public:
//
//  Default Constructor
//
//  Purpose: To create an WorldTestUnitAi.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isModelsLoaded()
//  Returns: N/A
//  Side Effect: A new WorldTestUnitAi is created with the
//               target at the origin and not moving.
//

	WorldTestUnitAi ();

//
//  Copy Constructor
//
//  Purpose: To create an WorldTestUnitAi as a copy of another.
//  Parameter(s):
//    <1> original: The WorldTestUnitAi to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorldTestUnitAi is created with the
//               same target as original.
//

	WorldTestUnitAi (const WorldTestUnitAi& original);

//
//  Destructor
//
//  Purpose: To safely destroy an WorldTestUnitAi without
//           memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this WorldTestUnitAi is freed.
//

	virtual ~WorldTestUnitAi ();

//
//  Assignment Operator
//
//  Purpose: To modify this WorldTestUnitAi to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The WorldTestUnitAi to copy
//  Precondition(s): N/A
//  Returns: A reference to this WorldTestUnitAi.
//  Side Effect: This WorldTestUnitAi is set to have the same
//               target position and velocity as original.
//

	WorldTestUnitAi& operator= (
	                         const WorldTestUnitAi& original);

//
//  getRingParticleCount
//
//  Purpose: To determine the number of ring particles in this
//           WorldTestUnitAi.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of ring particles.
//  Side Effect: N/A
//

	unsigned int getRingParticleCount () const;

//
//  getTargetShotCount
//
//  Purpose: To determine the number of times that the target
//           has been hit by the agent's bullets.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The times the target was hit since reset was
//           called.
//  Side Effect: N/A
//

	unsigned int getTargetShotCount () const;

//
//  getTargetCollisionCount
//
//  Purpose: To determine the number of times that the agent
//           collided with the target.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The times the agent and the target collided since
//           reset was called.
//  Side Effect: N/A
//

	unsigned int getTargetCollisionCount () const;

//
//  getAgentCollisionCount
//
//  Purpose: To determine the number of times that the agent was
//           in a collision with ring particles or the planet.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of agent collisions since this
//           WorldAssignment5A was last reset, excluding those
//           with the target.
//  Side Effect: N/A
//

	unsigned int getAgentCollisionCount () const;

//
//  getAgentAiTimeCumulative
//
//  Purpose: To determine the total time spent by the agent AI
//           so far.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The cumulative time spent by the AI for the agent
//           since this WorldAssignment5A was last reset.
//  Side Effect: N/A
//

	float getAgentAiTimeCumulative () const;

//
//  getAgentBulletCount
//
//  Purpose: To determine the total number of bullets fired by
//           the agent.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of bullets fires since reset was called.
//  Side Effect: N/A
//

	unsigned int getAgentBulletCount () const;

//
//  draw
//
//  Purpose: To display this WorldTestUnitAi.
//  Parameter(s):
//    <1> camera_forward: The camera forward vector
//    <2> camera_up: The camera up vector
//    <3> draw_orrery: Whether the Orrery should be displayed
//  Precondition(s):
//    <1> isModelsLoaded()
//    <2> camera_forward.isNormal()
//    <3> camera_up.isNormal()
//    <4> camera_forward.isOrthogonal(camera_up)
//  Returns: N/A
//  Side Effect: The moon, target, and explosions for this
//               WorldTestUnitAi are drawn.  If
//               draw_orrery == true, the Orrery the target
//               is attached to is also drawn.
//

	void draw (const Vector3& camera_forward,
	           const Vector3& camera_up,
	           bool draw_orrery) const;

//
//  setRingParticleCount
//
//  Purpose: To change the number of ring particles in this
//           WorldTestUnitAi.
//  Parameter(s):
//    <1> count: The new number of ring particles
//  Precondition(s):
//    <1> count <= RING_PARTICLE_COUNT_MAX
//  Returns: N/A
//  Side Effect: This WorldTestUnitAi is set to contain count
//               ring particles.  The AI may have cached
//               information about the ring partiocles, so this
//               command should be used with caution.
//

	void setRingParticleCount (unsigned int count);

//
//  update
//
//  Purpose: To update this WorldTestUnitAi for one frame.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The orrery, ships, and any shots in this World
//               are updated.
//

	void update ();

//
//  reset
//
//  Purpose: To reset this WorldTestUnitAi to its initial state.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isModelsLoaded()
//  Returns: N/A
//  Side Effect: This WorldTestUnitAi is reset to the state it
//               is in immediately after the default constructor
//               is called.  New ring particles are generated.
//

	void reset ();

///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from WorldInterface
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
//           cube may be different.
//  Side Effect: N/A
//

	virtual double getRingDensity (
	                             const Vector3& position) const;

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
//           that are only partially within the sphere.
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
//           planetoids are on.
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
//  Returns: The current score of fleet fleet.  This version of
//           the function always returns 0.0.
//  Side Effect: N/A
//

	virtual float getFleetScore (unsigned int fleet) const;

//
//  isFleetCommandShipAlive
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
//           This version of the function always returns false.
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
//           version of the function always returns ID_NOTHING.
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
//           fleet fleet.  This version of the function always
//           returns an empty vector.
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
//           fleet fleet.  This version of the function always
//           returns an empty vector.
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
//  Returns: The id for the planet.  This version of the
//           function always returns ID_NOTHING.
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
//           the vector more than once.
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
//  Returns: The position of PhysicsObject id.
//  Side Effect: N/A
//

	virtual Vector3 getPosition (
	                           const PhysicsObjectId& id) const;

//
//  getRadius
//
//  Purpose: To determine the radius of the PhysicsObject with
//           the specified id.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: The radius of PhysicsObject id.
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
//  Returns: The velocity of PhysicsObject id.
//  Side Effect: N/A
//

	virtual Vector3 getVelocity (
	                           const PhysicsObjectId& id) const;

//
//  getSpeed
//
//  Purpose: To determine the current speed of the PhysicsObject
//           with the specified id.
//  Parameter(s):
//    <1> id: The id for the PhysicsObject
//  Precondition(s):
//    <1> isAlive(id)
//  Returns: The speed of PhysicsObject id.
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
//           direction for PhysicsObject id.
//  Side Effect: N/A
//

	virtual Vector3 getForward (
	                           const PhysicsObjectId& id) const;

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
//           for PhysicsObject id.
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

	virtual bool isPlanetoidMoon (
	                           const PhysicsObjectId& id) const;

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
//           will return PhysicsObjectId::FLEET_NATURE.  This
//           version of the function always returns
//           PhysicsObjectId::FLEET_NATURE for all planetoids.
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
//           always returns false.  This version of the function
//           always returns false for all planetoids.
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
//           version of the function always returns false.
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
//           version of the function always returns
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
//               although 0 must always be a valid type.
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
//           is returned.  This version of the function always
//           calls addBullet and returns the result.
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
//  Helper functions not inherited from anywhere
//

//
//  getNearerRingClump
//
//  Purpose: To determine the center of the ring particle
//           clump closest to the specified position.
//  Parameter(s):
//    <1> position: The position to test
//  Precondition(s): N/A
//  Returns: The center of the ring particle clump closest to
//           position position.
//  Side Effect: N/A
//

	Vector3 getNearerRingClump (const Vector3& position) const;

//
//  initRingParticles
//
//  Purpose: To initialize the ring particles.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The ring particles are initialized.
//

	void initRingParticles ();

//
//  initBullets
//
//  Purpose: To initialize the bullets particles.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All bullets are initialized.
//

	void initBullets ();

//
//  updateTarget
//
//  Purpose: To update the position, forward direction, and
//           speed of the target based on the orrery.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The target value are updated.
//

	void updateTarget ();

//
//  checkCollisionsAll
//
//  Purpose: To detect and resolve all collisions for all
//           PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All collisions are resolved.
//

	void checkCollisionsAll ();

//
//  getCollisionForObject
//
//  Purpose: To determine if the specified PhysicsObject is in a
//           collision, and, if so, what with.
//  Parameter(s):
//    <1> object: The PhysicsObject to check
//  Precondition(s): N/A
//  Returns: The id of the object that object collides with.  If
//           there is no collision, PhysicsObjectId::ID_NOTHING
//           is returned.  If the collision is with a ring
//           particle, an id with hidden type TYPE_RING_PARTICLE
//           is returned.
//  Side Effect: N/A
//

	PhysicsObjectId getCollisionForObject (
	                         const PhysicsObject& object) const;

//
//  copyArrays
//
//  Purpose: To copy the ring particles, bullets, and simple
//           markers from the specified WorldTestUnitAi to this
//           WorldTestUnitAi.
//  Parameter(s):
//    <1> original: The WorldTestUnitAi to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The ring particles, bullets, and simple markers
//               in this WorldTestUnitAi are replced with copies
//               of the ones in original.
//

	void copyArrays (const WorldTestUnitAi& original);

//
//  invariant
//
//  Purpose: To determine if the class invarint is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	Orrery m_orrery;
	ExplosionManagerInterface* mp_explosion_manager;
	RingParticleData ma_ring_particles[RING_PARTICLE_COUNT_MAX];
	unsigned int m_ring_particle_count;
	Ship         m_target;
	unsigned int m_target_shot_count;
	unsigned int m_target_collision_count;
	double       m_target_disappear_time;
	Ship         m_agent;
	PhysicsObjectId m_agent_collision_last;
	unsigned int m_agent_collision_count;
	float        m_agent_ai_time_cumulative;
	unsigned int m_agent_bullet_count;
	Bullet       ma_bullets[BULLET_COUNT_MAX];
	unsigned int m_next_bullet;
	SimpleMarker ma_simple_markers[SIMPLE_MARKER_COUNT_MAX];
	unsigned int m_next_simple_marker;
};



#endif

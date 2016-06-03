//
//  WorldInterface.h
//
//  DO NOT MODIFY THIS FILE
//

#ifndef WORLD_INTERFACE_H
#define WORLD_INTERFACE_H

#include <vector>

#include "ObjLibrary/Vector3.h"

#include "PhysicsObjectId.h"



//
//  WorldInterface
//
//  An abstract interface to allow objects to interact with the
//    world around them.  Functions exist to query the rings,
//    fleets, planetoids, ships, bullets, missiles, and markers
//    in the world and to add explosions, bullets, and missiles
//    to it.
//

class WorldInterface
{
public:
	//
	//  RingParticleData
	//
	//  A record to store basic information about a single ring
	//    particle.
	//

	struct RingParticleData
	{
		Vector3 m_position;
		double m_radius;
	};

public:
//
//  Destructor
//
//  Purpose: To safely destroy a WorldInterface without memory
//           leaks.  We need a virtual destructor in the base
//           class so that the destructor for the correct
//           derived class will be invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this WorldInterface is freed.
//

	virtual ~WorldInterface ()
	{ }  // do nothing

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
	                         const Vector3& position) const = 0;

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
	                            double sphere_radius) const = 0;

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

	virtual unsigned int getFleetCount () const = 0;

//
//  getFleetScore
//
//  Purpose: To determine the score of the specified fleet.
//  Parameter(s):
//    <1> fleet: The fleet to query
//  Precondition(s):
//    <1> fleet <  getFleetCount()
//    <2> fleet != PhysicsObjectId::FLEET_NATURE
//  Returns: The current score of fleet fleet.
//  Side Effect: N/A
//

	virtual float getFleetScore (unsigned int fleet) const = 0;

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
//  Side Effect: N/A
//

	virtual bool isFleetAlive (unsigned int fleet) const = 0;

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
//  Returns: The id for the command ship for fleet fleet.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getFleetCommandShipId (
	                              unsigned int fleet) const = 0;

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
//           fleet fleet.
//  Side Effect: N/A
//

	virtual std::vector<PhysicsObjectId> getFleetFighterIds (
	                              unsigned int fleet) const = 0;

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
//           fleet fleet.
//  Side Effect: N/A
//

	virtual std::vector<PhysicsObjectId> getFleetMissileIds (
	                              unsigned int fleet) const = 0;

//
//  getPlanetId
//
//  Purpose: To determine the id for the planet.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The id for the planet.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getPlanetId () const = 0;

//
//  getMoonCount
//
//  Purpose: To determine the number of moons in the world.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of moons in the world.
//  Side Effect: N/A
//

	virtual unsigned int getMoonCount () const = 0;

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

	virtual PhysicsObjectId getMoonId (
	                               unsigned int moon) const = 0;

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
	                         const Vector3& position) const = 0;

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
	                            double sphere_radius) const = 0;

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

	virtual bool isAlive (const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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

	virtual double getRadius (
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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

	virtual double getSpeed (
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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

	virtual Vector3 getUp (const PhysicsObjectId& id) const = 0;

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

	virtual Vector3 getRight (
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
	                       const PhysicsObjectId& id) const = 0;

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
//  Returns: Whether missile id has used up its fuel.
//  Side Effect: N/A
//

	virtual bool isMissileOutOfFuel (
	                       const PhysicsObjectId& id) const = 0;

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
//           PhysicsObjectId::ID_NOTHING is returned.
//  Side Effect: N/A
//

	virtual PhysicsObjectId getMissileTarget (
	                       const PhysicsObjectId& id) const = 0;

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
	                           unsigned int type) = 0;

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
	                      const PhysicsObjectId& source_id) = 0;

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
	                      const PhysicsObjectId& target_id) = 0;

};



#endif

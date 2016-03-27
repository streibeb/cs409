//
//  ShipAiInterface.h
//
//  An abstract interface to allow unit AIs to interact with
//    ships.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef SHIP_AI_INTERFACE_H
#define SHIP_AI_INTERFACE_H

class Vector3;

struct PhysicsObjectId;
class  WorldInterface;
class  UnitAiSuperclass;



//
//  ShipAiInterface
//
//  An abstract interface to allow unit AIs to interact with
//    ships.  Functions exist to change and run the current unit
//    AI and for the unit AI to control the ship with.
//
//  It is assumed that the ship will be a subclass of
//    PhysicsObject, allowing it to inherit other functions
//    including getClone, isAlive, and markDead.
//

class ShipAiInterface
{
public:
//
//  Destructor
//
//  Purpose: To safely destroy a ShipAiInterface without memory
//           leaks.  We need a virtual destructor in the base
//           class so that the destructor for the correct
//           derived class will be invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this ShipAiInterface is freed.
//

	virtual ~ShipAiInterface ()
	{ }  // do nothing

//
//  isUnitAiSet
//
//  Purpose: To determine if a UnitAi is set for this Ship.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Ship is controlled by a UnitAi.
//  Side Effect: N/A
//

	virtual bool isUnitAiSet () const = 0;

//
//  getUnitAi
//
//  Purpose: To retreive a reference to the AI for this Ship.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isUnitAiSet()
//  Returns: A reference to the UnitAi for this Ship.
//  Side Effect: N/A
//

	virtual UnitAiSuperclass& getUnitAi () const = 0;

//
//  drawUnitAi
//
//  Purpose: To display visual information for the AI for this
//           Ship.  This function is intended for debugging use
//           only.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isUnitAiSet()
//  Returns: N/A
//  Side Effect: Any visual information for the AI that
//               currently controls this Ship is displayed.
//

	virtual void drawUnitAi () const = 0;

//
//  setDesiredVelocity
//
//  Purpose: To change this Ship to have the specified desired
//           velocity.
//  Parameter(s):
//    <1> desired_velocity: The desired velocity
//  Precondition(s):
//    <1> isUnitAiSet()
//    <2> desired_velocity.isNormLessThan(getSpeedMax())
//  Returns: N/A
//  Side Effect: This Ship is set to have a desired velocity of
//               desired_velocity.  This Ship will turn and
//               accelerate to match velocity desired_velocity
//               during subsequent calls to update.
//

	virtual void setDesiredVelocity (
	                       const Vector3& desired_velocity) = 0;

//
//  markFireBulletDesired
//
//  Purpose: To mark this Ship as ready to fire a bullet.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isUnitAiSet()
//  Returns: N/A
//  Side Effect: This Ship is marked to fire a Bullet during the
//               next update cycle.
//

	virtual void markFireBulletDesired () = 0;

//
//  markFireMissileDesired
//
//  Purpose: To mark this Ship as ready to fire a missile at the
//           target with the specified id.
//  Parameter(s):
//    <1> id_target: The id for the target
//  Precondition(s):
//    <1> isUnitAiSet()
//    <2> id_target != PhysicsObjectId::ID_NOTHING
//  Returns: N/A
//  Side Effect: This Ship is marked to fire a Missile at the
//               target with id id_target during the next update
//               cycle.  If id_target refers to a living object,
//               the missile will gome on that object.
//               Otherwise, it will just fly straight.  If this
//               Ship has an ammunition count of 0, there is no
//               effect.
//

	virtual void markFireMissileDesired (
	                      const PhysicsObjectId& id_target) = 0;

//
//  runAi
//
//  Purpose: To run the Ai for this Ship for one AI cycle.
//  Parameter(s):
//    <1> world: The World that the Ship is in
//  Precondition(s):
//    <1> isUnitAiSet()
//  Returns: N/A
//  Side Effect: If this Ship is alive, its unit AI is run.  The
//               desired velocity for this Ship may be updated
//               and this Ship may be marked to fire bullets and
//               missiles.  If this Ship is not alive, there is
//               no effect.
//

	virtual void runAi (const WorldInterface& world) = 0;

//
//  setUnitAi
//
//  Purpose: To specify the UnitAI to control this Ship.
//  Parameter(s):
//    <1> p_unit_ai: A pointer to the new UnitAi
//  Precondition(s):
//    <1> p_unit_ai != NULL
//    <2> p_unit_ai->isControlledShip(*this)
//  Returns: N/A
//  Side Effect: This Ship is set to be controlled by UnitAi
//               p_unit_ai.  This Ship will ensure that
//               p_unit_ai is eventually deallocated.  If a
//               desired velocity was set for this Ship, that
//               value may be lost.
//

	virtual void setUnitAi (UnitAiSuperclass* p_unit_ai) = 0;

//
//  setUnitAiNone
//
//  Purpose: To mark this Ship as not being controlled by a
//           UnitAi.  This is suitable for the player ship.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Ship is set to not have a UnitAi.
//

	virtual void setUnitAiNone () = 0;

};



#endif

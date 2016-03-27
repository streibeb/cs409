//
//  Ship.h
//

#ifndef SHIP_H
#define SHIP_H

#include "../../ObjLibrary/Vector3.h"

#include "PhysicsObject.h"
#include "ShipManeuverInterface.h"
#include "ShipAiInterface.h"

class DisplayList;

class WorldInterface;
struct PhysicsObjectId;
class UnitAiSuperclass;



//
//  Ship
//
//  A class to represent a spaceship.  In addition to
//    PhysicsObject fields, a Ship has a current health, a
//    reload timer, and an ammunition counter.
//
//  A Ship can be alive or dead.  It is alive if its current
//    health is greater than HEALTH_DEAD_AT, a fudge factor just
//    above 0.0.  If a dead Ship gains health, it may become
//    alive again.
//
//  The weapons for a Ship cannot fire continually.  This is
//    accomplished by a reload timer, which is set to 0.0
//    whenever the ship fires.  The values is then increased
//    during each update cycle.  When it reaches RELOAD_TIME or
//    larger, the Ship can fire again.
//
//  The Ship's id is used in collision checking.  Each time the
//    Ship fires a weapon, the shot fired shout be assigned the
//    id from the Ship that fired it.  Then, if the shot
//    collides with a Ship with the same source id, the
//    collision should be ignored.  The bullet is colliding
//    with the Ship that fired it because it hasn't moved far
//    enough yet.  If we don't do this, Ships will destroy
//    themselves by firing their weapons.  Each Ship should be
//    assigned a unique id.
//
//  Class Invariant:
//    <1> m_ammo >= 0
//

class Ship
: public PhysicsObject, public ShipManeuverInterface, public ShipAiInterface
{
public:
    //
    //  RADUIS
    //
    //  The default radius of the sphere used to represent a Ship
    //    for collision checking.
    //
    
    static const double RADIUS;
    
    //
    //  HEALTH_DEAD_AT
    //
    //  A constant indicating the health value at or below which a
    //    Ship is considered to be dead.  This value is slightly
    //    more than 0.0, and is used to avoid the effects of
    //    floating-point rounding errors.
    //
    
    static const float HEALTH_DEAD_AT;
    
    //
    //  RELOAD_TIME
    //
    //  The time it takes for the weapons on a Ship to reload after
    //    firing.
    //
    
    static const float RELOAD_TIME;
    
public:
    //
    //  Default Constructor
    //
    //  Purpose: To create a new stationary Ship at the origin.
    //  Paremeter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: A new Ship is created at the origin.  The new
    //               Ship is not moving and dead.
    //
    
    Ship ();
    
    //
    //  Constructor
    //
    //  Purpose: To create a new Ship with the specified position,
    //           velocity, health, and ammunition count.  The Ship
    //           will be displayed using the specified display list.
    //  Paremeter(s):
    //    <1> id: The id
    //    <2> position: The position
    //    <3> velocity: The starting velocity
    //    <4> display_list: The DisplayList to display with
    //    <5> display_scale: The scaling factor for display_list
    //    <6> health: The starting health
    //    <7> ammo: The ammunition count
    //  Precondition(s)
    //    <1> id != PhysicsObjectId::ID_NOTHING
    //    <2> display_list.isReady()
    //    <3> display_scale >= 0.0
    //    <4> ammo >= 0
    //  Returns: N/A
    //  Side Effect: A new Ship is created at position position with
    //               id id and velocity velocity.  It has health
    //               health and an ammunition count of ammo.  The
    //               new Ship is displayed with display list
    //               display_list scaled scaling factor
    //               display_scale.
    //
    
    Ship (const PhysicsObjectId& id,
          const Vector3& position,
          const Vector3& velocity,
          const DisplayList& display_list,
          double display_scale,
          float health,
          int ammo,
          float max_speed,
          float max_acceleration,
          float max_rotation_rate);
    
    //
    //  Copy Constructor
    //
    //  Purpose: To create a new Ship as a copy of another.
    //  Paremeter(s):
    //    <1> original: The Ship to copy
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: A new Ship is created with the same values as
    //               original.  This includes id, position,
    //               velocity, display list, health, reload counter,
    //               and ammo.
    //
    
    Ship (const Ship& original);
    
    //
    //  Destructor
    //
    //  Purpose: To safely destroy a Ship withoutt memory leaks.
    //  Paremeter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: All dynamically allocated memory associated
    //               with this Ship is freed.
    //
    
    virtual ~Ship ();
    
    //
    //  Assignment Operator
    //
    //  Purpose: To modify this Ship to be a copy of another.
    //  Paremeter(s):
    //    <1> original: The Ship to copy
    //  Precondition(s): N/A
    //  Returns: A reference to this Ship.
    //  Side Effect: The Ship is set to have the same values as
    //               original.  This includes id, position,
    //               velocity, display list, health, reload counter,
    //               and ammo.
    //
    
    Ship& operator= (const Ship& original);
    
    //
    //  getHealth
    //
    //  Purpose: To determine the current health of this Ship.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The health of this Ship.
    //  Side Effect: N/A
    //
    
    float getHealth () const;
    
    //
    //  isReloaded
    //
    //  Purpose: To determine if the weapons for this Ship are
    //           reloaded.  This depends on the current value of the
    //           reload counter.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: Whether the weapons for this Ship are reloaded.
    //  Side Effect: N/A
    //
    
    bool isReloaded () const;
    
    //
    //  getAmmo
    //
    //  Purpose: To determine the current ammunition of this Ship.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The ammunition of this Ship.
    //  Side Effect: N/A
    //
    
    int getAmmo () const;
    
    //
    //  setupCamera
    //
    //  Purpose: To set up the OpenGL modelview matrix as a
    //           3rd-person camera attached to this Ship.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: The camera is set to look in the direction this
    //               Ship is flying from behind and above.
    //
    
    void setupCamera () const;
    
    //
    //  getCameraCoordinateSystem
    //
    //  Purpose: To calculate the coordinate system for a 3rd-person
    //           camera attached to this Ship.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The camera cooridnate system.
    //  Side Effect: N/A
    //
    
    CoordinateSystem getCameraCoordinateSystem () const;
    
    //
    //  setHealth
    //
    //  Purpose: To change the current health of this Ship to the
    //           specified value.
    //  Parameter(s):
    //    <1> health: The new health
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This Ship is set to have health health.  If
    //               health is greater than HEALTH_DEAD_AT, the Ship
    //               is marked as not dead.  If it is less and the
    //               Ship was previously alive, it will be marked as
    //               dead during its next update cycle.
    //
    
    void setHealth (float health);
    
    //
    //  addHealth
    //
    //  Purpose: To increase the current health of this Ship by the
    //           specified amount.  Note that increasing health by a
    //           negative amount decreases it.
    //  Parameter(s):
    //    <1> increase: The amount to increase health by
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: The health of this Ship is increased by
    //               increase.  If the new health is greater than
    //               HEALTH_DEAD_AT, the Ship is is marked as not
    //               dead.  If it is less and the Ship was
    //               previously alive, it will be marked as dead
    //               during its next update cycle.
    //
    
    void addHealth (float increase);
    
    //
    //  markReloading
    //
    //  Purpose: To mark this Ship as reloading its weapons.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This Ship is set to have a reload timer time of
    //               0.0.  An effect of this is that it is not ready
    //               to fire its weapons.
    //
    
    void markReloading ();
    
    //
    //  markNotReloading
    //
    //  Purpose: To mark this Ship as ready to fire its weapons.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This Ship is set to have a reload timer time of
    //               RELOAD_TIME, making it ready to fire its
    //               weapons.
    //
    
    void markNotReloading ();
    
    //
    //  setAmmo
    //
    //  Purpose: To change the current ammunition count of this Ship
    //           to the specified value.
    //  Parameter(s):
    //    <1> ammo: The new ammunition count
    //  Precondition(s):
    //    <1> ammo >= 0
    //  Returns: N/A
    //  Side Effect: This Ship is set to have ammo ammunition.
    //
    
    void setAmmo (int ammo);
    
    //
    //  addAmmo
    //
    //  Purpose: To increase the current ammunition count of this
    //           Ship by the specified amount.  Note that increasing
    //           ammo by a negative amount decreases it.
    //  Parameter(s):
    //    <1> increase: The amount to increase ammo by
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: The ammunition count of this Ship is increased
    //               by increase.  Attepting to reduce the ammuntion
    //               count below 0 set it to 0.
    //
    
    void addAmmo (int increase);
    
    //
    //  updateForPause
    //
    //  Purpose: To update any internal time values for this Ship
    //           and its AI after the game has been paused.  This
    //           function should be called once each time the game
    //           is paused.  Calling this function at other times
    //           may result in unexpected behaviour.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This Ship is updated for the most recent
    //               pause.  The associated unit AI, if any, is
    //               updated.
    //
    
    void updateForPause ();
    
    ///////////////////////////////////////////////////////////////
    //
    //  Virtual functions inherited from PhysicsObject
    //
    
    //
    //  getClone
    //
    //  Purpose: To create a dynamically allocated copy of this
    //           PhysicsObject.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: A deep copy of this PhysicsObject.  This copy
    //           should be created using the copy constructor for
    //           the derived class.
    //  Side Effect: N/A
    //
    
    virtual PhysicsObject* getClone () const;
    
    //
    //  isAlive
    //
    //  Purpose: To determine if this PhysicsObject is alive.  A
    //           Ship with non-positive hit points is marked as not
    //           alive during its next update cycle.  A small fudge
    //           factor HEALTH_DEAD_AT is used to avoid
    //           floating-point rounding errors.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: Whether this PhysicsObject is alive.
    //  Side Effect: N/A
    //
    
    virtual bool isAlive () const;
    
    //
    //  isDying
    //
    //  Purpose: To determine if this PhysicsObject is currently
    //           dying.  This function can be used to prevent a
    //           PhysicsObject being counted twice.  For example, a
    //           bullet should not hit the same target twice.
    //  Parameter(s): N/A
    //  Precondition(s):
    //    <1> isAlive()
    //  Returns: Whether this PhysicsObject is going to be marked as
    //           dead the next time that update is called.  If this
    //           PhysicsObject is of a type that cannot become dead,
    //           false is returned.
    //  Side Effect: N/A
    //
    
    virtual bool isDying () const;
    
    //
    //  markDead
    //
    //  Purpose: To mark this PhysicsObject as dead.  Its current
    //           health is set to HEALTH_DEAD_AT.
    //  Parameter(s):
    //    <1> instant: Whether the PhysicsObject should be marked as
    //                 dead immediately
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: If this PhysicsObject is of a type that cannot
    //               become dead, there is no effect. Otherwise, if
    //               instant == true, this PhysicisObject is
    //               marked as dead and no death actions - such as
    //               generating a death explosion - are performed.
    //               If this PhysicsObject can become dead and
    //               instant == false, this PhysicsObject is marked
    //               as dying.  The next time that update is called,
    //               any death actions - such as generating a death
    //               explosion - will be performed and this
    //               PhysicsObject will be marked as dead.
    //
    
    virtual void markDead (bool instant);
    
    //
    //  update
    //
    //  Purpose: To update this PhysicsObject for one frame.  This
    //           function does not perform collision checking or
    //           handling.
    //  Parameter(s):
    //    <1> r_world: An interface to the world this PhysicsObject
    //                 is in
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This PhysicsObject is updated for one frame.
    //               Any queries about or changes to the world it is
    //               in are resolved through r_world.
    //
    
    virtual void update (WorldInterface& r_world);
    
    //
    //  getSpeedMax
    //
    //  Purpose: To determine the maximum speed for this Ship.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The maximum speed of this Ship.
    //  Side Effect: N/A
    //
    
    virtual double getSpeedMax () const;
    
    //
    //  getAcceleration
    //
    //  Purpose: To determine the acceleration rate for this Ship.
    //           Note that the acceleration is only used for changes
    //           to speed, not for turning.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The maximum acceleration of this Ship.
    //  Side Effect: N/A
    //
    
    virtual double getAcceleration () const;
    
    //
    //  getRotationRate
    //
    //  Purpose: To determine the turning rate for this Ship.  This
    //           value is only used for turning, not for changes to
    //           speed.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The maximum rotation rate for this Ship in radians
    //           per second.
    //  Side Effect: N/A
    //
    
    virtual double getRotationRate () const;
    
    //
    //  setManeuverability
    //
    //  Purpose: To change the maximum speed, acceleration, and
    //           rotation rate of this Ship to the specified values.
    //  Parameter(s):
    //    <1> speed_max: The new maximum speed
    //    <2> acceleration: The new accleration
    //    <3> rotation_rate: The new rotation rate in radians per
    //                       second
    //  Precondition(s):
    //    <1> speed_max >= 0.0
    //    <2> acceleration >= 0.0
    //    <3> rotation_rate >= 0.0
    //  Returns: N/A
    //  Side Effect: This Ship is set to have a maximum speed of
    //               speed_max, a maximum acceleration of
    //               acceleration, and a maximum rotation rate of
    //               rotation_rate radians per second.  If the
    //               current speed is larger than speed_max, the
    //               current speed is reduced.
    //
    
    virtual void setManeuverability (double speed_max,
                                     double acceleration,
                                     double rotation_rate);
    
    //
    //  isUnitAiSet
    //
    //  Purpose: To determine if a UnitAi is set for this Ship.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: Whether this Ship is controlled by a UnitAi.
    //  Side Effect: N/A
    //
    
    virtual bool isUnitAiSet () const;
    
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
    
    virtual UnitAiSuperclass& getUnitAi () const;
    
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
    
    virtual void drawUnitAi () const;
    
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
    
    virtual void setDesiredVelocity (const Vector3& desired_velocity);
    
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
    
    virtual void markFireBulletDesired ();
    
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
    
    virtual void markFireMissileDesired (const PhysicsObjectId& id_target);
    
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
    
    virtual void runAi (const WorldInterface& world);
    
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
    
    virtual void setUnitAi (UnitAiSuperclass* p_unit_ai);
    
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
    
    virtual void setUnitAiNone ();
    
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
    float m_health;
    float m_reload_timer;
    int m_ammo;
    bool m_is_dead;
    float max_speed;
    float max_acceleration;
    float max_rotation_rate;
    
    UnitAiSuperclass* unitAi;
    Vector3 desired_velocity;
    bool wantsToFire;
};



#endif

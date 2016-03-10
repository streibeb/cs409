//
//  PhysicsObject.h
//
//  An abstract superclass for objects that move around in the
//    game according to physics.
//

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"
#include "PhysicsObjectId.h"
#include "CoordinateSystem.h"

class DisplayList;
class WorldInterface;



//
//  PhysicsObject
//
//  An abstract superclass for objects that move around in the
//    game according to physics.  A PhysicsObject has its own
//    local coordinate system, including a position and an
//    orientation, as well as a size and a current speed.  It
//    also has a previous position, an id, a display list, and
//    a scaling factor.
//
//  The id for a PhysicsObject is represented by a
//    PhysicsObjectId instance, which can be implicitly
//    constructed from and converted to an unsigned integer.
//    This allows unsigned integers to be used as ids, and ids
//    to be stored in unsigned integrs.  No data is lost as a
//    result of these conversions.
//
//  A PhysicsObject always moves in the forward direction of its
//    local coordinate system.  Thus the current velocity is
//    always equal to the forward unit vector multiplied by the
//    current speed.
//
//  There exist functions to determine and change the position,
//    orientation, size, current speed, and id.  There are also
//    functions to give a PhysicsObject a random orientation.
//
//  The previous position is intended to be used for death
//    explosions.  If the current position is used, the
//    explosion will often appeatr inside something (such as a
//    ring particle), making it impossible to see.  The previous
//    position is updated whenever the updateBaic function is
//    called.  It is also set to the current position when
//    initPhysics or setPreviousPositionToCurrent is called.
//
//  A PhysicsObject is displayed with its current display list.
//    If a display list has not been set, calling draw does
//    nothing.  If a display list has been set, it is drawn at
//    the current position, rotated according to the local
//    coordinate system, and scaled according to the scaling
//    factor provided.
//
//  A PhysicsObject defines the pure virtual functions isAlive,
//    isDying, markDead, and update. Each (non-abstract)
//    subclass must provide implementations for these functions.
//    A protected helper function named updateBasic is provided
//    that moves a PhysicsObject according to its current
//    velocity.
//

class PhysicsObject
{
private:
    PhysicsObjectId id;
    CoordinateSystem coordinateSystem;
    Vector3 velocity;
    Vector3 previousPosition;
    double radius;
    DisplayList displayList;
    double scale;
    
public:
//
//  Default Constructor
//
//  Purpose: To create a new PhysicsObject with default values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PhysicsObject is created at the origin
//               with an id of ID_DEFAULT and a radius of 0.0.
//               The new PhysicsObject is not moving and is
//               facing along the positive X-axis with +Y up.
//               No display list is set.
//

	PhysicsObject ();

//
//  Constructor
//
//  Purpose: To create a new stationary PhysicsObject of the
//           specified size.
//  Parameter(s):
//    <1> radius: The radius
//  Precondition(s):
//    <1> radius >= 0.0
//  Returns: N/A
//  Side Effect: A new PhysicsObject is created at the origin
//               with an id of ID_DEFAULT and a radius of
//               radius.  The new PhysicsObject is not moving
//               and is facing along the positive X-axis with
//               +Y up.  No display list is set.
//

	PhysicsObject (double radius);

//
//  Constructor
//
//  Purpose: To create a new PhysicsObject at the specified
//           position with the specified id and size size and
//           that is moving at the specified velocity.  This
//           PhysicsObject is also set to display with the
//           specified display list, scaled by the specified
//           scaling factor.
//  Parameter(s):
//    <1> id: The id
//    <2> position: The starting position
//    <3> radius: The radius
//    <4> velocity: The starting velocity
//    <5> display_list: The DisplayList to display with
//    <6> display_scale: The scaling factor for display_list
//  Precondition(s):
//    <1> id != PhysicsObjectId::ID_NOTHING
//    <2> radius >= 0.0
//    <3> display_list.isReady()
//    <4> display_scale >= 0.0
//  Returns: N/A
//  Side Effect: A new PhysicsObject is created at position
//               position with an id of id and a radius of
//               radius.  The new PhysicsObject is moving with
//               velocity velocity.  If velocity is non-zero,
//               the new PhysicsObject is facing in the
//               direction it is moving.  The up vector is
//               undefined but guarenteed to be a right angle to
//               the forward vector.  If velocity is zero, the
//               new PhysicsObject is facing along the positive
//               X-axis with +Y as up.  In either case, the new
//               PhysicsObject is set to be displayed with
//               display list display_list scaled by scaling
//               factor display_scale.
//

	PhysicsObject (const PhysicsObjectId& id,
	               const Vector3& position,
	               double radius,
	               const Vector3& velocity,
	               const DisplayList& display_list,
	               double display_scale);

//
//  Copy Constructor
//
//  Purpose: To create a new PhysicsObject as a copy of another.
//  Parameter(s):
//    <1> original: The PhysicsObject to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PhysicsObject is created at the same id,
//               position and with the same orientation, size,
//               velocity, and display list as original.
//

	PhysicsObject (const PhysicsObject& original);

//
//  Destructor
//
//  Purpose: To safely destroy a PhysicsObject without memory
//           leaks.  This function must be virtual to ensure
//           that the destructor for the correct derived class
//           is invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this PhysicsObject is freed.
//

    virtual ~PhysicsObject ();

//
//  Assignment Operator
//
//  Purpose: To modify this PhysicsObject to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The PhysicsObject to copy
//  Precondition(s): N/A
//  Returns: A reference to this PhysicsObject
//  Side Effect: This PhysicsObject is set to be at the same id,
//               position and to have the same orientation,
//               size, velocity, and display list as original.
//

	PhysicsObject& operator= (const PhysicsObject& original);

//
//  getId
//
//  Purpose: To determine the id of this PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The id of this PhysicsObject.
//  Side Effect: N/A
//

	const PhysicsObjectId& getId () const;

//
//  getPosition
//
//  Purpose: To determine the position of this PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The position of this PhysicsObject.
//  Side Effect: N/A
//

	const Vector3& getPosition () const;

//
//  getPositionMin
//
//  Purpose: To determine the minimum position of the
//           axis-aligned cuboid containing this PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The minimum position for the axis-aligned cuboid
//           containing this PhysicsObject.  On each axis, this
//           is equal to the position of this PhysicsObject
//           minus the radius.
//  Side Effect: N/A
//

	Vector3 getPositionMin () const;

//
//  getPositionMax
//
//  Purpose: To determine the maximum position of the
//           axis-aligned cuboid containing this PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The minimum position for the axis-aligned cuboid
//           containing this PhysicsObject.  On each axis, this
//           is equal to the position of this PhysicsObject plus
//           the radius.
//  Side Effect: N/A
//

	Vector3 getPositionMax () const;

//
//  getPositionPrevious
//
//  Purpose: To determine the previous position of this
//           PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The previous position of this PhysicsObject.
//  Side Effect: N/A
//

	const Vector3& getPositionPrevious () const;

//
//  getPositionPreviousMin
//
//  Purpose: To determine the minimum position of the
//           axis-aligned cuboid that contained this
//           PhysicsObject when it was at its previous position.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The minimum previous position for the axis-aligned
//           cuboid that contained this PhysicsObject.  On each
//           axis, this is equal to the previous position of
//           this PhysicsObject minus the radius.
//  Side Effect: N/A
//

	Vector3 getPositionPreviousMin () const;

//
//  getPositionPreviousMax
//
//  Purpose: To determine the maximum position of the
//           axis-aligned cuboid that contained this
//           PhysicsObject when it was at its previous position.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The maximum previous position for the axis-aligned
//           cuboid that contained this PhysicsObject.  On each
//           axis, this is equal to the previous position of
//           this PhysicsObject plus the radius.
//  Side Effect: N/A
//

	Vector3 getPositionPreviousMax () const;

//
//  getRadius
//
//  Purpose: To determine the size of this PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The size of this PhysicsObject.  This value is
//           always non-negative.
//  Side Effect: N/A
//

	double getRadius () const;

//
//  getForward
//
//  Purpose: To determine the direction this PhysicsObject is
//           facing.  If this PhysicsObject is moving, it is
//           moving in this direction.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The direction this PhysicsObject is facing.  This
//           value is always a normal vector (i.e. it has a
//           length of 1.0).
//  Side Effect: N/A
//

	const Vector3& getForward () const;

//
//  getUp
//
//  Purpose: To determine the local up direction for this
//           PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The local up direction for this PhysicsObject.
//           This value is always a normal vector (i.e. it has a
//           length of 1.0).
//  Side Effect: N/A
//

	const Vector3& getUp () const;

//
//  getRight
//
//  Purpose: To determine the local right direction for this
//           PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The local right direction for this PhysicsObject.
//           This value is always a normal vector (i.e. it has a
//           length of 1.0).
//  Side Effect: N/A
//

	Vector3 getRight () const;

//
//  getSpeed
//
//  Purpose: To determine the current speed of this
//           PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The speed of this PhysicsObject.  This value is
//           always non-negative.
//  Side Effect: N/A
//

	double getSpeed () const;

//
//  getVelocity
//
//  Purpose: To determine the current velocity of this
//           PhysicsObject.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The velocity of this PhysicsObject.
//  Side Effect: N/A
//

	Vector3 getVelocity () const;

//
//  isDisplayListSet
//
//  Purpose: To determine if the display list for this Ship is
//           set.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Ship has a display list set.
//  Side Effect: N/A
//

	bool isDisplayListSet () const;

//
//  draw
//
//  Purpose: To display this PhysicsObject in its local
//           coordinate system using OpenGL graphics.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is displayed using its
//               current display list.  The display list is
//               translated and rotated based on the local
//               coordinate system, and scaled based on its
//               scaling factor.
//

	void draw () const;

//
//  initPhysics
//
//  Purpose: To initialize this PhysicsObject to be at the
//           specified position with the specified size and to
//           be moving with the specified velocity.  This
//           PhysicsObject is also set to display with the
//           specified display list, scaled by the specified
//           scaling factor.
//  Parameter(s):
//    <1> id: The id
//    <2> position: The starting position
//    <3> radius: The radius
//    <4> velocity: The starting velocity
//    <5> display_list: The DisplayList to display with
//    <6> display_scale: The scaling factor for display_list
//  Precondition(s):
//    <1> id != PhysicsObjectId::ID_NOTHING
//    <2> radius >= 0.0
//    <3> display_list.isReady()
//    <4> display_scale >= 0.0
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be at position
//               position with an id of id and a radius of
//               radius.  This PhysicsObject set to move at
//               velocity velocity.  If velocity is non-zero,
//               this PhysicsObject is facing in the direction
//               it is moving.  The up vector is undefined but
//               guarenteed to be a right angle to the forward
//               vector.  If velocity is zero, this
//               PhysicsObject is facing along the positive
//               X-axis with +Y as up.  In either case, this
//               PhysicsObject has a previous position of the
//               current position and is set to be displayed
//               with display list display_list scaled by
//               scaling factor display_scale.
//

	void initPhysics (const PhysicsObjectId& id,
	                  const Vector3& position,
	                  double radius,
	                  const Vector3& velocity,
	                  const DisplayList& display_list,
	                  double display_scale);

//
//  setId
//
//  Purpose: To change the id for this PhysicsObject.
//  Parameter(s):
//    <1> id: The new id
//  Precondition(s):
//    <1> id != PhysicsObjectId::ID_NOTHING
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to have an id of id.
//

	void setId (const PhysicsObjectId& id);

//
//  setPosition
//
//  Purpose: To change the position of this PhysicsObject to the
//           specified value.
//  Parameter(s):
//    <1> position: The new position
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be at position
//               position.
//

	void setPosition (const Vector3& position);

//
//  addPosition
//
//  Purpose: To increase the position of this PhysicsObject by
//           the specified amount.
//  Parameter(s):
//    <1> increase: The amount to change the position by
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The position of this PhysicsObject is increased
//               by increase.
//

	void addPosition (const Vector3& increase);

//
//  setPositionPreviousToCurrent
//
//  Purpose: To change the previous position of this
//           PhysicsObject to the current position.
//  Parameter(s):
//    <1> position: The new position
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The previous position for this PhysicsObject is
//               set to be at its current position.
//

	void setPositionPreviousToCurrent ();

//
//  setRadius
//
//  Purpose: To change the size of this PhysicsObject.
//  Parameter(s):
//    <1> radius: The new radius
//  Precondition(s):
//    <1> radius >= 0.0
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be have a size of
//               radius radius.
//

	void setRadius (double radius);

//
//  setOrientation
//
//  Purpose: To change the direction this PhysicsObject is
//           facing.
//  Parameter(s):
//    <1> forward: The new forward direction
//  Precondition(s):
//    <1> forward.isNormal()
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be facing in
//               direction forward.  The new value for the up
//               vector is undefined but guarenteed to be a
//               right angle to forward.  This PhysicsObject is
//               set to be moving in direction forward at its
//               previous speed.
//

	void setOrientation (const Vector3& forward);

//
//  setOrientation
//
//  Purpose: To change the direction this PhysicsObject is
//           facing.
//  Parameter(s):
//    <1> forward: The new forward direction
//    <2> up: The new up vector
//  Precondition(s):
//    <1> forward.isNormal()
//    <2> up.isNormal()
//    <3> forward.isOrthogonal(up)
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be facing in
//               direction forward with an up vector of up.
//               This PhysicsObject is set to be moving in
//               direction forward at its previous speed.
//

	void setOrientation (const Vector3& forward,
	                     const Vector3& up);

//
//  setUpVector
//
//  Purpose: To change the up vector of the local ccordinate
//           system for this PhysicsObject.
//  Parameter(s):
//    <1> up: The new up vector
//  Precondition(s):
//    <1> up.isNormal()
//    <2> getForward().isOrthogonal(up)
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to have a local up
//               vector of up.
//

	void setUpVector (const Vector3& up);

//
//  rotateAroundForward
//
//  Purpose: To rotate this PhysicsObject around its local
//           forward vector by the specified angle in radians.
//  Parameter(s):
//    <1> radians: The angle to rotate by
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is rotated by an angle of
//               radians radians around its forward vector.
//               This changes its up vector.
//

	void rotateAroundForward (double radians);

//
//  rotateAroundUp
//
//  Purpose: To rotate this PhysicsObject around its local up
//           vector by the specified angle in radians.
//  Parameter(s):
//    <1> radians: The angle to rotate by
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is rotated by an angle of
//               radians radians around its up vector.  This
//               changes its forward vector.
//

	void rotateAroundUp (double radians);

//
//  rotateAroundRight
//
//  Purpose: To rotate this PhysicsObject around its local
//           right vector by the specified angle in radians.
//  Parameter(s):
//    <1> radians: The angle to rotate by
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is rotated by an angle of
//               radians radians around its right vector.  This
//               changes its forward and up vectors.
//

	void rotateAroundRight (double radians);

//
//  rotateTowards
//
//  Purpose: To rotate this PhysicsObject towards the specified
//           direction.  The rotation is limited to a maximum of
//           the specified angle in radians.
//  Parameter(s):
//    <1> forward: The desired forward direction
//    <2> max_radians: The maximum angle to rotate by
//  Precondition(s):
//    <1> forward.isNormal()
//    <2> max_radians >= 0.0
//  Returns: N/A
//  Side Effect: This PhysicsObject is rotated by an angle of up
//               to max_radians radians towards forward.  This
//               changes its forward, up, and right vectors.
//

	void rotateTowards (const Vector3& forward,
	                    double max_radians);

//
//  setSpeed
//
//  Purpose: To change the speed this PhysicsObject is moving
//           at.
//  Parameter(s):
//    <1> speed: The new speed
//  Precondition(s):
//    <1> speed >= 0.0
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be moving at speed
//               speed in its current forward direction.  This
//               is the same direction as the previous velocity
//               of this PhysicsObject.
//

	void setSpeed (double speed);

//
//  setVelocity
//
//  Purpose: To change the velocity this PhysicsObject is
//           moving at.
//  Parameter(s):
//    <1> velocity: The new velocity
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be moving at
//               velocity velocity.  If velocity is not the zero
//               vector, this PhysicsObject is oriented to be
//               facing in the direction indicated by velocity.
//               The new up vector is undefined but guarenteed
//               to be at a right angle to velocity.  If
//               velocity is the zero vector, the orientation of
//               this PhysicsObject is unchanged.
//

	void setVelocity (const Vector3& velocity);

//
//  addVelocity
//
//  Purpose: To increase the velocity this PhysicsObject is
//           moving at.
//  Parameter(s):
//    <1> increase: The change in velocity
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The velocity of this PhysicsObject is increased
//               by increase.  If the new velocity is not the
//               zero vector, this PhysicsObject is oriented to
//               be facing in the direction indicated by the new
//               velocity.  The new up vector is undefined but
//               guarenteed to be at a right angle to velocity.
//               If the new velocity is the zero vector, the
//               orientation of this PhysicsObject is unchanged.
//

	void addVelocity (const Vector3& increase);

//
//  setDisplayList
//
//  Purpose: To set this PhysicsObject to display using the
//           specified display list scaled by the specified
//           scaling factor. 
//  Parameter(s):
//    <1> display_list: The new display list
//    <2> display_scale: The scaling factor to display this Ship
//                       with
//  Precondition(s)
//    <1> display_list.isReady()
//    <2> display_scale >= 0.0
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to be displayed with
//               display list display_list, scaled by scaling
//               factor display_scale.
//

	void setDisplayList (const DisplayList& display_list,
	                     double display_scale);

//
//  setDisplayListNone
//
//  Purpose: To mark this PhysicsObject as not having a display
//           list to display with.  Calling display on this
//           PhysicsObject when a display list is not set will
//           have no effect.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is set to not have a current
//               display list.
//

	void setDisplayListNone ();

//
//  randomizeUpVector
//
//  Purpose: To rotate this PhysicsObject so that the up vector
//           of its local coordinate system is in a random
//           direction.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is rotated around its
//               forward vector by a random amount, changing its
//               local up vector.  The local up vector is
//               undefined but guarenteed to be at a right angle
//               to the forward vector.  The local right vector
//               will be correct for the new orientation.  The
//               current velocity does not change.
//

	void randomizeUpVector ();

//
//  randomizeOrientation
//
//  Purpose: To rotate this PhysicsObject so that its local
//           coordinate system is oriented randomly.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is rotated to be facing in a
//               random direction.  The local up vector is
//               undefined but guarenteed to be at a right angle
//               to the forward vector.  The local right vector
//               will be correct for the new orientation.  The
//               current speed does not change, but the
//               direction of motion is set to the new forward
//               vector.
//

	void randomizeOrientation ();

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

	virtual PhysicsObject* getClone () const = 0;

//
//  isAlive
//
//  Purpose: To determine if this PhysicsObject is alive.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this PhysicsObject is alive.  If this
//           PhysicsObject is of a type that cannot become dead,
//           true is returned.
//  Side Effect: N/A
//

	virtual bool isAlive () const = 0;

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

	virtual bool isDying () const = 0;

//
//  markDead
//
//  Purpose: To mark this PhysicsObject as dead.
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

	virtual void markDead (bool instant) = 0;

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

	virtual void update (WorldInterface& r_world) = 0;

protected:
//
//  Helper Function: updateBasic
//
//  Purpose: To update the current position of this
//           PhysicsObject for one frame.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PhysicsObject is moved according to its
//               velocity.
//

	void updateBasic ();
    
private:
    void copy(const PhysicsObject& p);

};



#endif

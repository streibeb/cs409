//
//  Bullet.h
//

#ifndef BULLET_H
#define BULLET_H

#include "../../ObjLibrary/Vector3.h"

#include "PhysicsObjectId.h"
#include "PhysicsObject.h"

class DisplayList;

class WorldInterface;



//
//  Bullet
//
//  A class to represent a bullet fired.  In addition to
//    PhysicsObject fields, a Bullet has a id for the source
//    (normally a Ship) that shot it and a creation time.
//
//  A Bullet disappears a fixed time interval LIFESPAN after its
//    creation time.  This prevents mis-aimed Bullets from
//    accumulation on the map and slowing down display and
//    collision checking.  A creation time is used instead of a
//    lifespan remaining counter to reuce the updating needed.
//    A Bullet is said to be alive before its lifespan has
//    elapsed and dead after.
//
//  A Bullet should not collide with the source that shot it.
//    Otherwise Ships will blow themselves up when they try to
//    shoot enemies.
//

class Bullet : public PhysicsObject
{
public:
//
//  RADUIS
//
//  The default radius of the sphere used to represent a Bullet
//    for collision checking.
//

	static const double RADIUS;

//
//  LIFESPAN
//
//  The time between when a Bullet is created and when it
//    disappears.
//

	static const double LIFESPAN;

//
//  SPEED
//
//  The speed a Bullet flies at.
//

	static const double SPEED;

public:
//
//  Default Constructor
//
//  Purpose: To create a new stationary Bullet at the origin.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Bullet is created at the origin.  The new
//               Bullet is not moving, has a creation time in
//               the distant past, and has no known source.  No
//               display list is set.
//

	Bullet ();

//
//  Constructor
//
//  Purpose: To create a new Bullet with the specified id,
//           position, velocity, and source.
//  Paremeter(s):
//    <1> id: The id
//    <2> position: The position
//    <3> forward: The starting direction
//    <4> display_list: The DisplayList to display with
//    <5> display_scale: The scaling factor for display_list
//    <6> source_id: The id for the source that fired the new
//                   Bullet
//  Precondition(s)
//    <1> id != PhysicsObjectId::ID_NOTHING
//    <2> forward.isNormal()
//    <3> display_list.isReady()
//    <4> display_scale >= 0.0
//  Returns: N/A
//  Side Effect: A new Bullet is created at position position
//               with id id and moving in direction forward with
//               speed SPEED.  It is marked as fired from source
//               source_id and has a creation time of the current
//               time.  The new Bullet is displayed with display
//               list display_list scaled by scaling factor
//               display_scale.
//

	Bullet (const PhysicsObjectId& id,
	        const Vector3& position,
	        const Vector3& forward,
	        const DisplayList& display_list,
	        double display_scale,
	        const PhysicsObjectId& source_id);

//
//  Copy Constructor
//
//  Purpose: To create a new Bullet as a copy of another.
//  Paremeter(s):
//    <1> original: The Bullet to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Bullet is created with the same values as
//               original.  This includes id, position,
//               velocity, display list, source id, and creation
//               time.
//

	Bullet (const Bullet& original);

//
//  Destructor
//
//  Purpose: To safely destroy a Bullet without memory leaks.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this Bullet is freed.
//

	virtual ~Bullet ();

//
//  Assignment Operator
//
//  Purpose: To modify this Bullet to be a copy of another.
//  Paremeter(s):
//    <1> original: The Bullet to copy
//  Precondition(s): N/A
//  Returns: A reference to this Bullet.
//  Side Effect: The Bullet is set to have the same values as
//               original.  This includes id, position,
//               velocity, display list, source id, and creation
//               time.
//

	Bullet& operator= (const Bullet& original);

//
//  getSourceId
//
//  Purpose: To determine the id of the source of this Bullet.
//           This is probably the Ship that fired it.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The id of the source of this Bullet.  Collisions
//           between this Bullet and its source should be
//           ignored.
//  Side Effect: N/A
//

	const PhysicsObjectId& getSourceId () const;

//
//  fire
//
//  Purpose: To modifiy this Bullet to have just been fired from
//           the specified position by the specified source with
//           the specified velocity.
//  Parameter(s):
//    <1> position: The new position this Bullet was fired from
//    <2> forward: The direction this Bullet was fired in
//    <3> source_id: The id for the source that fired this Bullet
//  Precondition(s):
//    <1> forward.isNormal()
//  Returns: N/A
//  Side Effect: This Bullet is set to be at position position
//               and moving in direction forward with speed
//               SPEED.  It is marked as fired from the source
//               with id source_id and has a creation time of
//               the current time.
//

	void fire (const Vector3& position,
	           const Vector3& forward,
	           const PhysicsObjectId& source_id);

//
//  setSourceId
//
//  Purpose: To change the id of source for this Bullet.
//           Collisions between a Bullet and its source should
//           be ignored.
//  Parameter(s):
//    <1> source_id: The id of the new source
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Bullet is set to have source id source_id.
//

	void setSourceId (const PhysicsObjectId& source_id);

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
//  Purpose: To determine if this PhysicsObject is alive.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this PhysicsObject is alive.  If this
//           PhysicsObject is of a type that cannot become dead,
//           true is returned.
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
//  Purpose: To mark this PhysicsObject as dead.  Its creation
//           time is set to somewhere in the distant past. 
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

private:
	PhysicsObjectId m_source_id;
	double m_creation_time;
	bool m_is_dead;
};



#endif

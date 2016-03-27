//
//  CollisionSystemInterface.h
//
//  An abstract interface for a class to detect potential
//    collisions between objects.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef COLLISION_SYSTEM_INTERFACE_H
#define COLLISION_SYSTEM_INTERFACE_H

#include <vector>

#include "PhysicsObjectId.h"

class Vector3;



//
//  CollisionSystemInterface
//
//  An abstract interface for a class to detect potential
//    collisions between objects.  Functions exist to add
//    objects, remove, and move objects, and to query for
//    collisions.  Object and query shapes may be specified as a
//    point or a bounding cuboid.  There is also a function to
//    remove all objects.
//
//  The potential collisions detected are only between points
//    and axis-aligned bounding boxes.  If the objects have
//    other shapes, it will be necessary for the client code to
//    perform additional tests to determine if a true collision
//    occurs.  For example, if the objects are spheres, the
//    corners of the bounding boxes could overlap without the
//    spheres touching.  In the case of spheres, in fact, there
//    will only be a collision about half (~52%) of the time.
//
//  The objects are represented by a PhysicsObjectId instance,
//    which can be implicitly constructed from and converted to
//    an unsigned integer.  This allows unsigned integers to be
//    used as object identifiers as function parameters, and the
//    results of queries to be stored in unsigned integrs.  No
//    data is lost as a result of these conversions.
//

class CollisionSystemInterface
{
public:
//
//  Destructor
//
//  Purpose: To safely destroy a CollisionSystemInterface
//           without memory leaks.  We need a virtual destructor
//           in the base class so that the destructor for the
//           correct derived class will be invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this CollisionSystemInterface is freed.
//

	virtual ~CollisionSystemInterface ()
	{ }  // do nothing

//
//  isEmpty
//
//  Purpose: To determine if there are any objects in this
//           CollisionSystem.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: true if this CollisionSystem contians no objects.
//           false if there is 1 or more objects.
//  Side Effect: N/A
//

	virtual bool isEmpty () const = 0;

//
//  getCollisions
//
//  Purpose: To determine all pontential collisions at the
//           specified position.
//  Parameter(s):
//    <1> position: The position to query
//  Precondition(s): N/A
//  Returns: A std::vector containing the ids for all the
//           objects that might collide with something at
//           position position.  The vector is not sorted and
//           may include duplicates.
//  Side Effect: N/A
//

	virtual std::vector<PhysicsObjectId> getCollisions (
	                         const Vector3& position) const = 0;

//
//  getCollisions
//
//  Purpose: To determine all pontential collisions in the
//           specified axis-aligned cuboid.
//  Parameter(s):
//    <1> corner_min: The corner of the cuboid with the minimum
//                    value for each coordinate
//    <2> corner_max: The corner of the cuboid with the maximum
//                    value for each coordinate
//  Precondition(s):
//    <1> corner_min.isAllComponentsLessThanOrEqual(corner_max)
//  Returns: A std::vector containing the ids for all the
//           objects that might collide with something in the
//           cuboid from corner_min to corner_max.  The vector
//           is not sorted and may include duplicates.
//  Side Effect: N/A
//

	virtual std::vector<PhysicsObjectId> getCollisions (
	                       const Vector3& corner_min,
	                       const Vector3& corner_max) const = 0;

//
//  getClone
//
//  Purpose: To create a dynamically-allocated copy of this
//           CollisionSystemInterface.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A pointer to a dynamically-allocated deep copy of
//           this CollisionSystemInterface.
//  Side Effect: N/A
//

	virtual CollisionSystemInterface* getClone () const = 0;

//
//  add
//
//  Purpose: To add a point object with the specified id at the
//           specified position.
//  Parameter(s):
//    <1> id: The id for the object
//    <2> position: The position for the object
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A point-sized object with id id is added
//               at position position.
//

	virtual void add (const PhysicsObjectId& id,
	                  const Vector3& position) = 0;

//
//  add
//
//  Purpose: To add an object with the specified id occupying
//           the specified axis-aligned cuboid.
//  Parameter(s):
//    <1> id: The id for the object
//    <2> corner_min: The corner of the cuboid with the minimum
//                    value for each coordinate
//    <3> corner_max: The corner of the cuboid with the maximum
//                    value for each coordinate
//  Precondition(s):
//    <1> corner_min.isAllComponentsLessThanOrEqual(corner_max)
//  Returns: N/A
//  Side Effect: An object with id id is added covering the
//               axis-aligned cuboid from corner_min to
//               corner_max.
//

	virtual void add (const PhysicsObjectId& id,
	                  const Vector3& corner_min,
	                  const Vector3& corner_max) = 0;

//
//  remove
//
//  Purpose: To remove a point object with the specified id that
//           is at the specified position.
//  Parameter(s):
//    <1> id: The id for the object
//    <2> position: The position for the object
//  Precondition(s): N/A
//  Returns: Whether the object was removed.
//  Side Effect: If a point-sized object with id id is at
//               position position, it is removed.  If there is
//               no object with id id, there is no effect.  If
//               there is an object, but it is not at position
//               position, whether it is removed is
//               implementation-dependant.  If there is more
//               than one object with id id, the effect is
//               implementation-dependant.
//

	virtual bool remove (const PhysicsObjectId& id,
	                     const Vector3& position) = 0;

//
//  remove
//
//  Purpose: To remove an object with the specified id occupying
//           the specified axis-aligned cuboid.
//  Parameter(s):
//    <1> id: The id for the object
//    <2> corner_min: The corner of the cuboid with the minimum
//                    value for each coordinate
//    <3> corner_max: The corner of the cuboid with the maximum
//                    value for each coordinate
//  Precondition(s):
//    <1> corner_min.isAllComponentsLessThanOrEqual(corner_max)
//  Returns: Whether the object was removed.
//  Side Effect: If an object with id id is covering the cuboid
//               from corner_min to corner_max, it is removed.
//               If there is no object with id id, there is no
//               effect.  If there is an object, but it is not
//               covering the cuboid, whether it is removed is
//               implementation-dependant.  If there is more
//               than one object with id id, the effect is
//               implementation-dependant.
//

	virtual bool remove (const PhysicsObjectId& id,
	                     const Vector3& corner_min,
	                     const Vector3& corner_max) = 0;

//
//  removeAll
//
//  Purpose: To remove all objects from this CollisionSystem.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All iobjects are removed.
//

	virtual void removeAll () = 0;

};



#endif

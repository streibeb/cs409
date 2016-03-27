//
//  CollisionSystemLinear.h
//
//  A dummy class that implements the CollisionSystemInterface
//    interface.
//

#ifndef COLLISION_SYSTEM_LINEAR_H
#define COLLISION_SYSTEM_LINEAR_H

#include <vector>

#include "PhysicsObjectId.h"
#include "CollisionSystemInterface.h"

class Vector3;



//
//  CollisionSystemLinear
//
//  A dummy class that implements the CollisionSystemInterface
//    interface.  This class ignores the positions of the added
//    objects.  It returns all added objects as potential
//    collisions for all positions and cuboids.  Moving and
//    removing objects is slow and inefficent.
//

class CollisionSystemLinear : public CollisionSystemInterface
{
public:
//
//  Default Constructor
//
//  Purpose: To create a CollisionSystemLinear containing no
//           objects.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new CollisionSystemLinear is created.  No
//               objects are added.
//

	CollisionSystemLinear ();

//
//  Copy Constructor
//
//  Purpose: To create a CollisionSystemLinear containing the
//           same objects as another.
//  Parameter(s):
//    <1> original: The CollisionSystemLinear to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new CollisionSystemLinear is created.  A copy
//               of each object in original is added.
//

	CollisionSystemLinear (
	                     const CollisionSystemLinear& original);

//
//  Destructor
//
//  Purpose: To safely destroy a CollisionSystemLinear without
//           memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this CollisionSystemLinear is freed.
//

	virtual ~CollisionSystemLinear ();

//
//  Assignment Operator
//
//  Purpose: To modify this a CollisionSystemLinear to contain
//           the same objects as another.
//  Parameter(s):
//    <1> original: The CollisionSystemLinear to copy
//  Precondition(s): N/A
//  Returns: A reference to this CollisionSystemLinear.
//  Side Effect: This CollisionSystemLinear is set to contian a
//               copy of each object in original.  Any existing
//               objects are removed.
//

	CollisionSystemLinear& operator= (
	                     const CollisionSystemLinear& original);

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

	virtual bool isEmpty () const;

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
	                             const Vector3& position) const;

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
	                           const Vector3& corner_max) const;

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

	virtual CollisionSystemInterface* getClone () const;

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
	                  const Vector3& position);

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
	                  const Vector3& corner_max);

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
	                     const Vector3& position);

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
	                     const Vector3& corner_max);

//
//  removeAll
//
//  Purpose: To remove all objects from this CollisionSystem.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All iobjects are removed.
//

	virtual void removeAll ();

private:
	std::vector<PhysicsObjectId> mv_objects;
};



#endif

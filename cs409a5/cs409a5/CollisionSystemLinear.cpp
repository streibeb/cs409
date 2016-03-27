//
//  CollisionSystemLinear.cpp
//

#include <cassert>
#include <vector>

#include "../../ObjLibrary/Vector3.h"

#include "PhysicsObjectId.h"
#include "CollisionSystemInterface.h"
#include "CollisionSystemLinear.h"

using namespace std;



CollisionSystemLinear :: CollisionSystemLinear ()
		: mv_objects()
{
	assert(isEmpty());
}

CollisionSystemLinear :: CollisionSystemLinear (const CollisionSystemLinear& original)
		: mv_objects(original.mv_objects)
{
}

CollisionSystemLinear :: ~CollisionSystemLinear ()
{
}

CollisionSystemLinear& CollisionSystemLinear :: operator= (const CollisionSystemLinear& original)
{
	if(&original != this)
		mv_objects = original.mv_objects;
	return *this;
}



bool CollisionSystemLinear :: isEmpty () const
{
	return mv_objects.empty();
}

vector<PhysicsObjectId> CollisionSystemLinear :: getCollisions (const Vector3& position) const
{
	// we aren't keeping track of object positions, so all
	//  objects are potential collisions
	return mv_objects;
}

vector<PhysicsObjectId> CollisionSystemLinear :: getCollisions (const Vector3& corner_min,
                                                                const Vector3& corner_max) const
{
	assert(corner_min.isAllComponentsLessThanOrEqual(corner_max));

	// we aren't keeping track of object positions, so all
	//  objects are potential collisions
	return mv_objects;
}

CollisionSystemInterface* CollisionSystemLinear :: getClone () const
{
	return new CollisionSystemLinear(*this);
}



void CollisionSystemLinear :: add (const PhysicsObjectId& id,
                                   const Vector3& position)
{
	mv_objects.push_back(id);
}

void CollisionSystemLinear :: add (const PhysicsObjectId& id,
                                   const Vector3& corner_min,
                                   const Vector3& corner_max)
{
	assert(corner_min.isAllComponentsLessThanOrEqual(corner_max));

	mv_objects.push_back(id);
}

bool CollisionSystemLinear :: remove (const PhysicsObjectId& id,
                                      const Vector3& position)
{
	// a better implementation would be more efficient...
	for(unsigned int i = 0; i < mv_objects.size(); i++)
		if(mv_objects[i] == id)
		{
			mv_objects[i] = mv_objects.back();
			mv_objects.pop_back();

			// only remove one copy
			return true;
		}

	return false;  // we didn't find the object
}

bool CollisionSystemLinear :: remove (const PhysicsObjectId& id,
                                      const Vector3& corner_min,
                                      const Vector3& corner_max)
{
	assert(corner_min.isAllComponentsLessThanOrEqual(corner_max));

	// a better implementation would be more efficient...
	for(unsigned int i = 0; i < mv_objects.size(); i++)
		if(mv_objects[i] == id)
		{
			mv_objects[i] = mv_objects.back();
			mv_objects.pop_back();

			// only remove one copy
			return true;
		}

	return false;  // we didn't find the object
}

void CollisionSystemLinear :: removeAll ()
{
	mv_objects.clear();

	assert(isEmpty());
}


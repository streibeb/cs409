//
//  GeometricCollisions.cpp
//

#include <cassert>
#include <iostream>

#include "../../ObjLibrary/Vector3.h"

#include "GeometricCollisions.h"

using namespace std;
namespace
{
	const bool DEBUGGING_SPHERE_VS_CUBOID = false;
}



bool GeometricCollisions :: pointVsSphere (const Vector3& point,
                                           const Vector3& sphere_center,
                                           double sphere_radius)

{
	assert(sphere_radius >= 0.0);

	if(point.isDistanceLessThan(sphere_center, sphere_radius))
		return true;
	else
		return false;
}

bool GeometricCollisions :: sphereVsSphere (const Vector3& sphere1_center,
                                            double sphere1_radius,
                                            const Vector3& sphere2_center,
                                            double sphere2_radius)

{
	assert(sphere1_radius >= 0.0);
	assert(sphere2_radius >= 0.0);

	if(sphere1_center.isDistanceLessThan(sphere2_center, sphere1_radius + sphere2_radius))
		return true;
	else
		return false;
}

bool GeometricCollisions :: pointVsCuboid (const Vector3& point,
                                           const Vector3& cuboid_center,
                                           const Vector3& cuboid_size)

{
	assert(cuboid_size.isAllComponentsNonNegative());

	Vector3 difference = point - cuboid_center;
	if(fabs(difference.x) >= cuboid_size.x) return false;
	if(fabs(difference.y) >= cuboid_size.y) return false;
	if(fabs(difference.z) >= cuboid_size.z) return false;
	return true;
}

bool GeometricCollisions :: cuboidVsCuboid (const Vector3& cuboid1_center,
                                            const Vector3& cuboid1_size,
                                            const Vector3& cuboid2_center,
                                            const Vector3& cuboid2_size)

{
	assert(cuboid1_size.isAllComponentsNonNegative());
	assert(cuboid2_size.isAllComponentsNonNegative());

	Vector3 difference = cuboid1_center - cuboid2_center;
	Vector3 size_sum   = cuboid1_size   + cuboid2_size;
	if(fabs(difference.x) >= size_sum.x) return false;
	if(fabs(difference.y) >= size_sum.y) return false;
	if(fabs(difference.z) >= size_sum.z) return false;
	return true;
}

bool GeometricCollisions :: sphereVsCuboid (const Vector3& sphere_center,
                                            double sphere_radius,
                                            const Vector3& cuboid_center,
                                            const Vector3& cuboid_size)

{
	assert(sphere_radius >= 0.0);
	assert(cuboid_size.isAllComponentsNonNegative());

	// this one is hard!

	Vector3 difference         = sphere_center - cuboid_center;
	Vector3 abs_difference(fabs(difference.x), fabs(difference.y), fabs(difference.z));
	Vector3 reduced_difference = abs_difference - cuboid_size;
	Vector3 fixed_difference   = reduced_difference.getMaxComponents(Vector3::ZERO);

	if(DEBUGGING_SPHERE_VS_CUBOID)
	{
		cout << "Sphere vs. cuboid collision check:" << endl;
		cout << "\tdifference:         " << difference << endl;
		cout << "\tabs_difference:     " << abs_difference << endl;
		cout << "\treduced_difference: " << reduced_difference << endl;
		cout << "\tfixed_difference:   " << fixed_difference << endl;
		cout << "\tfixed norm vs max:  " << fixed_difference.getNorm() << "\t/ " << sphere_radius << endl;
	}

	// equals case includes all inside square, so must be included
	if(fixed_difference.isNormLessThan(sphere_radius))  // include equals
		return true;
	else
		return false;
}

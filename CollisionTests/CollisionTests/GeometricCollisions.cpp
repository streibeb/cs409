//
//  GeometricCollisions.cpp
//  cs409a4
//
//  Created by Vince Streibel on 2016-03-10.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#include "GeometricCollisions.h"
#include "../../ObjLibrary/Vector3.h"

namespace GeometricCollisions
{
    
    bool pointVsSphere (const Vector3& point,
                        const Vector3& sphere_center,
                        double sphere_radius)
    {
        return (point.isDistanceLessThan(sphere_center, sphere_radius));
    }
    
    bool sphereVsSphere (const Vector3& sphere1_center,
                         double sphere1_radius,
                         const Vector3& sphere2_center,
                         double sphere2_radius)
    {
        return (sphere1_center.
                isDistanceLessThan(sphere2_center, sphere1_radius + sphere2_radius));
    }

    bool pointVsCuboid (const Vector3& point,
                        const Vector3& cuboid_center,
                        const Vector3& cuboid_size)
    {
        Vector3 min = cuboid_center - cuboid_size;
        Vector3 max = cuboid_center + cuboid_size;
        
        if (point.x < min.x || point.x > max.x)
            return false;
        if (point.y < min.y || point.y > max.y)
            return false;
        if (point.z < min.z || point.z > max.z)
            return false;
        return true;
    }
    
    bool cuboidVsCuboid (const Vector3& cuboid1_center,
                         const Vector3& cuboid1_size,
                         const Vector3& cuboid2_center,
                         const Vector3& cuboid2_size)
    {
        Vector3 min1 = cuboid1_center - cuboid1_size;
        Vector3 max1 = cuboid1_center + cuboid1_size;
        Vector3 min2 = cuboid2_center - cuboid2_size;
        Vector3 max2 = cuboid2_center + cuboid2_size;
        
        if (min2.x >= max1.x || min1.x >= max2.x)
            return false;
        if (min2.y >= max1.y || min1.y >= max2.y)
            return false;
        if (min2.z >= max1.z || min1.z >= max2.z)
            return false;
        return true;
    }
    
    bool sphereVsCuboid (const Vector3& sphere_center,
                         double sphere_radius,
                         const Vector3& cuboid_center,
                         const Vector3& cuboid_size)
    {
        Vector3 minBox = cuboid_center - cuboid_size;
        Vector3 maxBox = cuboid_center + cuboid_size;
        double x, y, z;
        
        if (sphere_center.x <= minBox.x)
            x = minBox.x;
        else if (sphere_center.x >= maxBox.x)
            x = maxBox.x;
        else
            x = sphere_center.x;
        
        if (sphere_center.y <= minBox.y)
            y = minBox.y;
        else if (sphere_center.y >= maxBox.y)
            y = maxBox.y;
        else
            y = sphere_center.y;
        
        if (sphere_center.z <= minBox.z)
            z = minBox.z;
        else if (sphere_center.z >= maxBox.z)
            z = maxBox.z;
        else
            z = sphere_center.z;
        
        Vector3 closest(x, y, z);
        return (closest.isDistanceLessThan(sphere_center, sphere_radius));
    }
    
}  // end of namespace Collisions

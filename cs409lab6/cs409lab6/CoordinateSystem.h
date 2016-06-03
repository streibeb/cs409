//
//  CoordinateSystem.hpp
//  cs409lab6
//
//  Created by Vince Streibel on 2016-01-13.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#ifndef CoordinateSystem_hpp
#define CoordinateSystem_hpp

#include <stdlib.h>
#include "GetGlut.h"
#include "ObjLibrary/Vector3.h"

class CoordinateSystem
{
private:
    Vector3 forward;
    Vector3 up;
    Vector3 position;
    
public:
    CoordinateSystem();
    ~CoordinateSystem();
    
    void setRoll(double theta);
    void setYaw(double theta);
    void setPitch(double theta);
    void rotateToVector(const Vector3& target_facing, double max_radians);
    
    void moveForward(double distance);
    void moveBackward(double distance);
    void moveUp(double distance);
    void moveDown(double distance);
    void moveLeft(double distance);
    void moveRight(double distance);
    
    Vector3 getForward();
    Vector3 getUp();
    Vector3 getPosition();
    
    void reset();
    void setCamera();
};

#endif /* CoordinateSystem_hpp */

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
#include "../../ObjLibrary/Vector3.h"

class CoordinateSystem
{
private:
    Vector3 forward;
    Vector3 up;
    Vector3 position;
    
public:
    CoordinateSystem();
    ~CoordinateSystem();
    
    // Rotation around Z axis
    void setRoll(double theta);
    
    // Rotation around Y axis
    void setYaw(double theta);
    
    // Rotation around X axis
    void setPitch(double theta);
    
    // Rotate camera to point in the direction of a given vector
    void rotateToVector(const Vector3& target_facing, double max_radians);
    
    // Set position in (x,y,z)
    void setPosition(const Vector3& pos);
    
    // Move distance along forward vector
    void moveForward(double distance);
    
    // Move distance along negative forward vector
    void moveBackward(double distance);
    
    // Move distance along up vector
    void moveUp(double distance);
    
    // Move distance along negative up vector
    void moveDown(double distance);
    
    // Move distance along negative right vector
    void moveLeft(double distance);
    
    // Move distance along right vector
    void moveRight(double distance);
    
    Vector3 getForward();
    Vector3 getUp();
    Vector3 getPosition();
    
    // Resets camera's orientation and position back to default
    void reset();
    
    // Sets camera perspective
    void setCamera();
};

#endif /* CoordinateSystem_hpp */

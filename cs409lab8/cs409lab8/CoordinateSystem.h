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
#include <cmath>
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
    CoordinateSystem(const Vector3& pos, const Vector3& fwd);
    CoordinateSystem(const Vector3& pos, const Vector3& fwd, const Vector3& up);
    CoordinateSystem(const CoordinateSystem& c);
    ~CoordinateSystem();
    
    CoordinateSystem& operator=(const CoordinateSystem& c);
    
    void init(const Vector3& pos, const Vector3& fwd, const Vector3& up);
    
    const Vector3& getForward() const;
    const Vector3& getUp() const;
    const Vector3& getPosition() const;
    Vector3 getRight() const;

    void setPosition(const Vector3& pos);
    void setOrientation(const Vector3& fwd, const Vector3& up);
    void setOrientation(const Vector3& fwd);
    void calculateRandomOrientation();
    
    void rotateAroundForward(double theta);
    void rotateAroundUp(double theta);
    void rotateAroundRight(double theta);
    void rotateToVector(const Vector3& target_facing, double max_radians);
    void rotateUpright(double max_radians);
    
    void moveForward(double distance);
    void moveUp(double distance);
    void moveRight(double distance);
    
    void applyTransformation() const;
    
    void reset();
    void setCamera();
    
protected:
    Vector3 calculateUpVector(const Vector3& fwd) const;
    
private:
    void copy(const CoordinateSystem& c);
};

#endif /* CoordinateSystem_hpp */

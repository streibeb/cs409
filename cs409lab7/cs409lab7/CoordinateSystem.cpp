//
//  CoordinateSystem.cpp
//  cs409lab6
//
//  Created by Vince Streibel on 2016-01-13.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem()
{
    reset();
}

CoordinateSystem::~CoordinateSystem()
{

}

CoordinateSystem::CoordinateSystem(Vector3 pos, Vector3 fwd, Vector3 up)
{
    this->position = pos;
    this->forward = fwd;
    this->up = up;
}

void CoordinateSystem::rotateAroundForward(double theta)
{
    up.rotateArbitrary(forward, theta);
}

void CoordinateSystem::rotateAroundUp(double theta)
{
    forward.rotateArbitrary(up, theta);
}

void CoordinateSystem::rotateAroundRight(double theta)
{
    Vector3 right = getRight();
    forward.rotateArbitrary(right, theta);
    up.rotateArbitrary(right, theta);
}

void CoordinateSystem::rotateToVector(const Vector3& target_facing, double max_radians)
{
    if (target_facing.isZero())
        return;
    Vector3 axis = forward.crossProduct(target_facing);
    if (axis.isZero())
        axis = up;
    else
        axis.normalize();
    double radians = forward.getAngleSafe(target_facing);
    if (radians > max_radians)
        radians = max_radians;
    forward.rotateArbitrary(axis, radians);
    up     .rotateArbitrary(axis, radians);
}

void CoordinateSystem::moveForward(double distance)
{
    position += forward * distance;
}

void CoordinateSystem::moveBackward(double distance)
{
    position -= forward * distance;
}

void CoordinateSystem::moveUp(double distance)
{
    position += up * distance;
}

void CoordinateSystem::moveDown(double distance)
{
    position -= up * distance;
}

void CoordinateSystem::moveRight(double distance)
{
    position += getRight() * distance;
}

void CoordinateSystem::moveLeft(double distance)
{
    position -= getRight() * distance;
}

Vector3 CoordinateSystem::getRight() const
{
    return forward.crossProduct(up);
}

Vector3 CoordinateSystem::getForward() const
{
    return forward;
}

Vector3 CoordinateSystem::getUp() const
{
    return up;
}

Vector3 CoordinateSystem::getPosition() const
{
    return position;
}

void CoordinateSystem::applyTransformation() const
{
    glTranslated(position.x, position.y, position.z);
    
    Vector3 right = getRight();
    double a_matrix[16] =
    {
        right.x, right.y, right.z, 0.0,
        forward.x, forward.y, forward.z, 0.0,
        up.x, up.y, up.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    glMultMatrixd(a_matrix);
}

void CoordinateSystem::reset()
{
    forward = Vector3( 0,  0, -1);
    up = Vector3( 0,  1,  0);
    position = Vector3(0, 0, 0);
}

void CoordinateSystem::setCamera()
{
    Vector3 look_at = position + forward;
    gluLookAt(position.x, position.y, position.z,
              look_at.x, look_at.y, look_at.z,
              up.x,      up.y,      up.z);
}
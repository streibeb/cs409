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

void CoordinateSystem::setRoll(double theta)
{
    up.rotateArbitrary(forward, theta);
}

void CoordinateSystem::setYaw(double theta)
{
    forward.rotateArbitrary(up, theta);
}

void CoordinateSystem::setPitch(double theta)
{
    Vector3 right = forward.crossProduct(up);
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

void CoordinateSystem::setPosition(const Vector3& pos)
{
    position = pos;
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
    position += forward.crossProduct(up) * distance;
}

void CoordinateSystem::moveLeft(double distance)
{
    position -= forward.crossProduct(up) * distance;
}

Vector3 CoordinateSystem::getForward()
{
    return forward;
}

Vector3 CoordinateSystem::getUp()
{
    return up;
}

Vector3 CoordinateSystem::getPosition()
{
    return position;
}

void CoordinateSystem::reset()
{
    forward = Vector3(0, 0, -1);
    up = Vector3(0, 1, 0);
    position = Vector3(70000.0, 0.0, 70000.0);
}

void CoordinateSystem::setCamera()
{
    Vector3 look_at = position + forward;
    gluLookAt(position.x, position.y, position.z,
              look_at.x, look_at.y, look_at.z,
              up.x,      up.y,      up.z);
}
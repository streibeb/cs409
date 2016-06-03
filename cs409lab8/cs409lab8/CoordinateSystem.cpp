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

CoordinateSystem::CoordinateSystem(const Vector3& pos, const Vector3& fwd)
{
    this->position = pos;
    this->forward = fwd;
    this->up = calculateUpVector(fwd);
}

CoordinateSystem::CoordinateSystem(const Vector3& pos, const Vector3& fwd, const Vector3& up)
{
    this->position = pos;
    this->forward = fwd;
    this->up = up;
}

CoordinateSystem::CoordinateSystem(const CoordinateSystem& c)
{
    copy(c);
}

CoordinateSystem& CoordinateSystem::operator=(const CoordinateSystem& c)
{
    copy(c);
    return *this;
}

void CoordinateSystem::init(const Vector3& pos, const Vector3& fwd, const Vector3& up)
{
    this->position = pos;
    this->forward = fwd;
    this->up = up;
}

void CoordinateSystem::copy(const CoordinateSystem& c)
{
    this->position = c.position;
    this->forward = c.forward;
    this->up = c.up;
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

void CoordinateSystem::rotateUpright(double max_radians)
{
    Vector3 desired_up = calculateUpVector(forward);
    Vector3 axis = up.crossProduct(desired_up);
    if (axis.isZero()) axis = forward;
    else axis.normalize();
    double radians = up.getAngleSafe(desired_up);
    if (radians > max_radians) radians = max_radians;
    up.rotateArbitrary(axis, radians);
}

void CoordinateSystem::moveForward(double distance)
{
    position += forward * distance;
}

void CoordinateSystem::moveUp(double distance)
{
    position += up * distance;
}

void CoordinateSystem::moveRight(double distance)
{
    position += getRight() * distance;
}

Vector3 CoordinateSystem::getRight() const
{
    return forward.crossProduct(up);
}

const Vector3& CoordinateSystem::getForward() const
{
    return forward;
}

const Vector3& CoordinateSystem::getUp() const
{
    return up;
}

const Vector3& CoordinateSystem::getPosition() const
{
    return position;
}

void CoordinateSystem::setPosition(const Vector3& pos)
{
    this->position = pos;
}

void CoordinateSystem::setOrientation(const Vector3& fwd, const Vector3& up)
{
    this->forward = fwd;
    this->up = up;
}

void CoordinateSystem::setOrientation(const Vector3& fwd)
{
    this->forward = fwd;
    this->up = calculateUpVector(fwd);
}

void CoordinateSystem::calculateRandomOrientation()
{
    double random0to1 = rand() / (RAND_MAX + 1.0);
    up.rotateArbitrary(forward, (2 * M_PI) * random0to1);
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
    this->forward = Vector3( 0,  0, -1);
    this->up = Vector3( 0,  1,  0);
    this->position = Vector3(0, 0, 0);
}

void CoordinateSystem::setCamera()
{
    Vector3 look_at = position + forward;
    gluLookAt(position.x, position.y, position.z,
              look_at.x, look_at.y, look_at.z,
              up.x,      up.y,      up.z);
}

Vector3 CoordinateSystem::calculateUpVector(const Vector3& fwd) const
{
    static const Vector3 IDEAL_UP_VECTOR(0.0, 1.0, 0.0);
    static const double HALF_PI = 1.5707963267948966;
    
    if (fwd.isZero()) return IDEAL_UP_VECTOR;
    
    Vector3 axis = fwd.crossProduct(IDEAL_UP_VECTOR);
    if (axis.isZero()) return Vector3(1.0, 0.0, 0.0);
    else
    {
        axis.normalize();
        Vector3 l_up = fwd.getRotatedArbitrary(axis, HALF_PI);
        return l_up;
    }
}
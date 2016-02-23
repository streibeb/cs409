//
//  PhysicsObject.cpp
//  cs409a3
//
//  Created by Vince Streibel on 2016-02-18.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#include "PhysicsObject.h"


PhysicsObject::PhysicsObject ()
{
    displayListSet = false;
}

PhysicsObject::PhysicsObject (double radius)
{
    this->radius = radius;
}

PhysicsObject::PhysicsObject (const PhysicsObjectId& id,
                              const Vector3& position,
                              double radius,
                              const Vector3& velocity,
                              const DisplayList& display_list,
                              double display_scale)
{
    this->id = id;
    CoordinateSystem(position, velocity);
    this->radius = radius;
    this->velocity = velocity;
    this->displayList = display_list;
    this->scale = display_scale;
    displayListSet = true;
}

PhysicsObject::PhysicsObject (const PhysicsObject& original)
{
    copy(original);
}

PhysicsObject& PhysicsObject::operator= (const PhysicsObject& original)
{
    copy(original);
    return *this;
}

void PhysicsObject::copy(const PhysicsObject &p)
{
    this->id = p.id;
    coordinateSystem.init(p.coordinateSystem.getPosition(),
                          p.coordinateSystem.getForward(),
                          p.coordinateSystem.getUp());
    this->radius = p.radius;
    this->velocity = p.velocity;
    this->displayList = p.displayList;
    this->scale = p.scale;
    displayListSet = true;
}

const PhysicsObjectId& PhysicsObject::getId () const
{
    return id;
}

const Vector3& PhysicsObject::getPosition () const
{
    return coordinateSystem.getPosition();
}

Vector3 PhysicsObject::getPositionMin () const
{
    Vector3 pos = coordinateSystem.getPosition();
    return Vector3(pos.x - radius, pos.y - radius, pos.z - radius);
}

Vector3 PhysicsObject::getPositionMax () const
{
    Vector3 pos = coordinateSystem.getPosition();
    return Vector3(pos.x + radius, pos.y + radius, pos.z + radius);
}

const Vector3& PhysicsObject::getPositionPrevious () const
{
    return previousPosition;
}

Vector3 PhysicsObject::getPositionPreviousMin () const
{
    Vector3 pos = previousPosition;
    return Vector3(pos.x - radius, pos.y - radius, pos.z - radius);
}

Vector3 PhysicsObject::getPositionPreviousMax () const
{
    Vector3 pos = previousPosition;
    return Vector3(pos.x - radius, pos.y - radius, pos.z - radius);
}

double PhysicsObject::getRadius () const
{
    return radius;
}

const Vector3& PhysicsObject::getForward () const
{
    return coordinateSystem.getForward();
}

const Vector3& PhysicsObject::getUp () const
{
    return coordinateSystem.getUp();
}

Vector3 PhysicsObject::getRight () const
{
    return coordinateSystem.getRight();
}

double PhysicsObject::getSpeed () const
{
    Vector3 pos = coordinateSystem.getPosition();
    return sqrt(pow(pos.x, 2.0) + pow(pos.y, 2.0) + pow(pos.z, 2.0));
}

Vector3 PhysicsObject::getVelocity () const
{
    return velocity;
}

bool PhysicsObject::isDisplayListSet () const
{
    return displayListSet;
}

void PhysicsObject::draw () const
{
    Vector3 pos = coordinateSystem.getPosition();
    glPushMatrix();
        glTranslated(pos.x, pos.y, pos.z);
        glScalef(radius, radius, radius);
        displayList.draw();
    glPopMatrix();
}

void PhysicsObject::initPhysics (const PhysicsObjectId& id,
                                 const Vector3& position,
                                 double radius,
                                 const Vector3& velocity,
                                 const DisplayList& display_list,
                                 double display_scale)
{
    this->id = id;
    coordinateSystem.setPosition(position);
    this->radius = radius;
    this->velocity = velocity;
    this->displayList = display_list;
    this->scale = display_scale;
    setPositionPreviousToCurrent();
}

void PhysicsObject::setId (const PhysicsObjectId& id)
{
    this->id = id;
}

void PhysicsObject::setPosition (const Vector3& position)
{
    this->coordinateSystem.setPosition(position);
}

void PhysicsObject::addPosition (const Vector3& increase)
{
    Vector3 newPos = coordinateSystem.getPosition() + increase;
    coordinateSystem.setPosition(newPos);
}

void PhysicsObject::setPositionPreviousToCurrent ()
{
    previousPosition = coordinateSystem.getPosition();
}

void PhysicsObject::setRadius (double radius)
{
    this->radius = radius;
}

void PhysicsObject::setOrientation (const Vector3& forward)
{
    this->setOrientation(forward);
}

void PhysicsObject::setOrientation (const Vector3& forward,
                                    const Vector3& up)
{
    coordinateSystem.setOrientation(forward, up);
}

void PhysicsObject::setUpVector (const Vector3& up)
{
    coordinateSystem.setOrientation(getForward(), up);
}

void PhysicsObject::rotateAroundForward (double radians)
{
    coordinateSystem.rotateAroundForward(radians);
}

void PhysicsObject::rotateAroundUp (double radians)
{
    coordinateSystem.rotateAroundUp(radians);
}

void PhysicsObject::rotateAroundRight (double radians)
{
    coordinateSystem.rotateAroundRight(radians);
}

void PhysicsObject::rotateTowards (const Vector3& forward,
                                   double max_radians)
{
    coordinateSystem.rotateToVector(forward, max_radians);
}

void PhysicsObject::setSpeed (double speed)
{
    velocity.normalize();
    velocity = velocity * speed;
}

void PhysicsObject::setVelocity (const Vector3& velocity)
{
    this->velocity = velocity;
}

void PhysicsObject::addVelocity (const Vector3& increase)
{
    velocity += increase;
}

void PhysicsObject::setDisplayList (const DisplayList& display_list,
                                    double display_scale)
{
    this->displayList = display_list;
    this->scale = display_scale;
    displayListSet = true;
}

void PhysicsObject::setDisplayListNone ()
{
    displayListSet = false;
}

void PhysicsObject::randomizeUpVector ()
{
    coordinateSystem.randomizeUpVector();
}

void PhysicsObject::randomizeOrientation ()
{
    coordinateSystem.randomizeOrientation();
}

void PhysicsObject::updateBasic ()
{
    setPositionPreviousToCurrent();
    coordinateSystem.moveForward(getSpeed());
}

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
}

PhysicsObject::PhysicsObject (const PhysicsObject& original)
{
    copy(original);
}

PhysicsObject::~PhysicsObject()
{
    
}

PhysicsObject& PhysicsObject::operator= (const PhysicsObject& original)
{
    if(&original != this)
    {
        copy(original);
    }
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
    return coordinateSystem.getForward();;
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
    Vector3 pos = velocity;
    return pos.getNorm();
    //return sqrt((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z));
}

Vector3 PhysicsObject::getVelocity () const
{
    return velocity;
}

bool PhysicsObject::isDisplayListSet () const
{
    return !displayList.isEmpty();
}

void PhysicsObject::draw () const
{
    glPushMatrix();
        coordinateSystem.applyTransformation();
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
    // Fix orientation issue here
    this->id = id;
    coordinateSystem.setPosition(position);
    this->radius = radius;
    this->velocity = velocity;
    if (!velocity.isZero()) setOrientation(velocity.getNormalized());
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
    coordinateSystem.setOrientation(forward);
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
    velocity = getForward() * speed;
}

void PhysicsObject::setVelocity (const Vector3& velocity)
{
    this->velocity = velocity;
    if (!velocity.isZero()) setOrientation(velocity.getNormalized());
}

void PhysicsObject::addVelocity (const Vector3& increase)
{
    velocity += increase;
    if (!velocity.isZero()) setOrientation(velocity.getNormalized());
}

void PhysicsObject::setDisplayList (const DisplayList& display_list,
                                    double display_scale)
{
    this->displayList = display_list;
    this->scale = display_scale;
}

void PhysicsObject::setDisplayListNone ()
{
    this->displayList.makeEmpty();
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
    addPosition(velocity * TimeSystem::getFrameDuration());
}

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
    
}

PhysicsObject::PhysicsObject (const PhysicsObjectId& id,
                              const Vector3& position,
                              double radius,
                              const Vector3& velocity,
                              const DisplayList& display_list,
                              double display_scale)
{
    
}

PhysicsObject::PhysicsObject (const PhysicsObject& original)
{
    
}

PhysicsObject& PhysicsObject::operator= (const PhysicsObject& original)
{
    
}

const PhysicsObjectId& PhysicsObject::getId () const
{
    
}

const Vector3& PhysicsObject::getPosition () const
{
    
}

Vector3 PhysicsObject::getPositionMin () const
{
    
}

Vector3 PhysicsObject::getPositionMax () const
{
    
}

Vector3 PhysicsObject::getPositionPreviousMin () const
{
    
}

Vector3 PhysicsObject::getPositionPreviousMax () const
{
    
}

double PhysicsObject::getRadius () const
{
    
}

const Vector3& PhysicsObject::getForward () const
{
    
}

const Vector3& PhysicsObject::getUp () const
{
    
}

Vector3 PhysicsObject::getRight () const
{
    
}

double PhysicsObject::getSpeed () const
{
    
}

Vector3 PhysicsObject::getVelocity () const
{
    
}

bool PhysicsObject::isDisplayListSet () const
{
    
}

void PhysicsObject::draw () const
{
    
}

void PhysicsObject::initPhysics (const PhysicsObjectId& id,
                                 const Vector3& position,
                                 double radius,
                                 const Vector3& velocity,
                                 const DisplayList& display_list,
                                 double display_scale)
{
    
}

void PhysicsObject::setId (const PhysicsObjectId& id)
{
    
}

void PhysicsObject::setPosition (const Vector3& position)
{
    
}

void PhysicsObject::addPosition (const Vector3& increase)
{
    
}

void PhysicsObject::setPositionPreviousToCurrent ()
{
    
}

void PhysicsObject::setRadius (double radius)
{
    
}

void PhysicsObject::setOrientation (const Vector3& forward)
{
    
}

void PhysicsObject::setOrientation (const Vector3& forward,
                                    const Vector3& up)
{
    
}

void PhysicsObject::setUpVector (const Vector3& up)
{
    
}

void PhysicsObject::rotateAroundForward (double radians)
{
    
}

void PhysicsObject::rotateAroundUp (double radians)
{
    
}

void PhysicsObject::rotateAroundRight (double radians)
{
    
}

void PhysicsObject::rotateTowards (const Vector3& forward,
                                   double max_radians)
{
    
}

void PhysicsObject::setSpeed (double speed)
{
    
}

void PhysicsObject::setVelocity (const Vector3& velocity)
{
    
}

void PhysicsObject::addVelocity (const Vector3& increase)
{
    
}

void PhysicsObject::setDisplayList (const DisplayList& display_list,
                                    double display_scale)
{
    
}

void PhysicsObject::setDisplayListNone ()
{
    
}

void PhysicsObject::randomizeUpVector ()
{
    
}

void PhysicsObject::randomizeOrientation ()
{
    
}

void PhysicsObject::updateBasic ()
{
    
}

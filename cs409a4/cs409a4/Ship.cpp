//
//  Ship.cpp
//  cs409a3
//
//  Created by Vince Streibel on 2016-02-21.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#include "Ship.h"

const double Ship :: RADIUS         = 10.0;
const float  Ship :: HEALTH_DEAD_AT =  0.001f;
const float  Ship :: RELOAD_TIME    =  0.25f;

Ship::Ship() : PhysicsObject(RADIUS)
{
    reloading = false;
}

Ship::Ship(int health, int ammo) : PhysicsObject(RADIUS)
{
    this->health = health;
    this->ammoCount = ammo;
}

Ship::Ship(const Ship& s) : PhysicsObject(s)
{
    copy(s);
}

Ship::~Ship()
{
    // empty
}

Ship& Ship::operator=(const Ship& s)
{
    PhysicsObject::operator=(s);
    copy(s);
    return *this;
}

void Ship::copy(const Ship& s)
{
    this->camera = s.camera;
    this->alive = s.alive;
    this->health = s.health;
    this->ammoCount = s.ammoCount;
    this->reloadTimer = s.reloadTimer;
}

float Ship::getHealth() const
{
    return health;
}

bool Ship::isReloaded() const
{
    return (reloadTimer <= 0.f);
}

int Ship::getAmmo() const
{
    return ammoCount;
}

void Ship::setupCamera() const
{
    CoordinateSystem camera = getCameraCoordinateSystem();
    const Vector3& camera_position = camera.getPosition();
    const Vector3& camera_up       = camera.getUp();
    
    Vector3 look_at = camera_position + camera.getForward();
    
    gluLookAt(camera_position.x, camera_position.y, camera_position.z,
              look_at.x,         look_at.y,         look_at.z,
              camera_up.x,       camera_up.y,       camera_up.z);
}

CoordinateSystem Ship::getCameraCoordinateSystem() const
{
    const Vector3& forward = getForward();
    const Vector3& up      = getUp();
    
    Vector3 camera_at = getPosition() +
    forward * CAMERA_FORWARD_DISTANCE +
    up      * CAMERA_UP_DISTANCE;
    
    return CoordinateSystem(camera_at, forward, up);
}

void Ship::setHealth(float health)
{
    if (health > 0 && !alive)
    {
        alive = true;
    }
    this->health = health;
}

void Ship::addHealth(float increase)
{
    this->health += increase;
}

void Ship::markReloading()
{
    reloading = true;
    reloadTimer = RELOAD_TIME;
}

void Ship::markNotReloading()
{
    reloading = false;
}

void Ship::setAmmo(int newAmt)
{
    ammoCount = newAmt;
}

void Ship::addAmmo(int increase)
{
    ammoCount += increase;
}

PhysicsObject* Ship::getClone() const
{
    return new Ship(*this);
}

bool Ship::isAlive() const
{
    return alive;
}

bool Ship::isDying() const
{
    return (alive && health <= 0);
}

void Ship::markDead(bool instant)
{
    if (instant)
    {
        alive = false;
    }
    else
    {
        health = -1.0;
    }
}

void Ship::update(WorldInterface& r_world)
{
    updateBasic();
    if (reloading)
    {
        reloadTimer -= TimeSystem::getFrameDuration();
        if (reloadTimer <= 0) markNotReloading();
    }
    if (health <= 0.0)
    {
        markDead(true);
        r_world.addExplosion(getPosition(), 50.0, 0);
    }
}

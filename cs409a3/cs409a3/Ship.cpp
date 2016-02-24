//
//  Ship.cpp
//  cs409a3
//
//  Created by Vince Streibel on 2016-02-21.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#include "Ship.h"

Ship::Ship() : PhysicsObject()
{
    reloading = false;
    this->reloadTimer = RELOAD_TIME;
}

Ship::Ship(int health, int ammo) : PhysicsObject()
{
    this->health = health;
    this->ammoCount = ammo;
    this->reloadTimer = RELOAD_TIME;
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

void Ship::setupCamera()
{
    Vector3 p = getPosition();
    Vector3 f = getForward();
    Vector3 u = getUp();
    Vector3 position = p - 75*f + 15*u;
    
    camera.setPosition(position);
    gluLookAt(position.x, position.y, position.z,
              p.x, p.y, p.z,
              u.x,      u.y,      u.z);
}

CoordinateSystem Ship::getCameraCoordinateSystem() const
{
    return camera;
}

void Ship::setHealth(float health)
{
    if (health > 0 && !alive)
    {
        alive = true;
    }
    this->health = health;
}

void Ship::markReloading()
{
    reloading = true;
}

void Ship::markNotReloading()
{
    reloading = false;
    reloadTimer = RELOAD_TIME;
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

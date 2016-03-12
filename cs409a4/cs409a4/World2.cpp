//
//  World2.cpp
//

#include <cassert>
#include <iostream>

#include "../../ObjLibrary/ObjModel.h"
#include "../../ObjLibrary/Vector3.h"

#include "PhysicsObjectId.h"
#include "PhysicsObject.h"
#include "ExplosionManagerInterface.h"
#include "ExplosionManager.h"
#include "WorldInterface.h"
#include "World.h"

using namespace std;

///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from WorldInterface
//

double World :: getRingDensity (const Vector3& position) const
{
    
    cout << "Error: World::getRingDensity is not implemented" << endl;
    return 0.0f;
}

vector<WorldInterface::RingParticleData> World :: getRingParticles (const Vector3& sphere_center,
                                                                    double sphere_radius) const
{
    cout << "Error: World::getRingParticles is not implemented" << endl;
    assert(sphere_radius >= 0.0);
    
    return vector<RingParticleData>();
}

unsigned int World :: getFleetCount () const
{
    cout << "Error: World::getFleetCount is not implemented" << endl;
    return 0;
}

float World :: getFleetScore (unsigned int fleet) const
{
    cout << "Error: World::getFleetScore is not implemented" << endl;
    assert(fleet <  getFleetCount());
    assert(fleet != PhysicsObjectId::FLEET_NATURE);
    
    return 0;
}

bool World :: isFleetAlive (unsigned int fleet) const
{
    cout << "Error: World::isFleetAlive is not implemented" << endl;
    assert(fleet <  getFleetCount());
    assert(fleet != PhysicsObjectId::FLEET_NATURE);
    
    return false;
}

PhysicsObjectId World :: getFleetCommandShipId (unsigned int fleet) const
{
    cout << "Error: World::getFleetCommandShipId is not implemented" << endl;
    assert(fleet <  getFleetCount());
    assert(fleet != PhysicsObjectId::FLEET_NATURE);
    
    return PhysicsObjectId::ID_NOTHING;
}

vector<PhysicsObjectId> World :: getFleetFighterIds (unsigned int fleet) const
{
    cout << "Error: World::getFleetFighterIds is not implemented" << endl;
    assert(fleet <  getFleetCount());
    assert(fleet != PhysicsObjectId::FLEET_NATURE);
    
    return vector<PhysicsObjectId>();
}

vector<PhysicsObjectId> World :: getFleetMissileIds (unsigned int fleet) const
{
    cout << "Error: World::getFleetMissileIds is not implemented" << endl;
    assert(fleet <  getFleetCount());
    assert(fleet != PhysicsObjectId::FLEET_NATURE);
    
    return vector<PhysicsObjectId>();
}

PhysicsObjectId World :: getPlanetId () const
{
    //cout << "Error: World::getPlanetId is not implemented" << endl;
    //return PhysicsObjectId::ID_NOTHING;
    
    return planet.getId();
}

unsigned int World :: getMoonCount () const
{
    //cout << "Error: World::getMoonCount is not implemented" << endl;
    //return 0;
    
    return MOON_COUNT;
}

PhysicsObjectId World :: getMoonId (unsigned int moon) const
{
    //cout << "Error: World::getMoonId is not implemented" << endl;
    assert(moon < getMoonCount());
    //return PhysicsObjectId::ID_NOTHING;
    
    return moons[moon].getId();
}

PhysicsObjectId World :: getNearestPlanetoidId (const Vector3& position) const
{
    //cout << "Error: World::getPlanetId is not implemented" << endl;
    //return PhysicsObjectId::ID_NOTHING;
    
    int index = 0;
    double min_distance = position.getDistance(moons[0].getPosition());
    for (int i = 1; i < MOON_COUNT; i++)
    {
        double temp = position.getDistance(moons[i].getPosition());
        if (temp < min_distance)
        {
            index = i;
            temp = min_distance;
        }
    }
    return PhysicsObjectId::ID_NOTHING;
}

vector<PhysicsObjectId> World :: getShipIds (const Vector3& sphere_center,
                                             double sphere_radius) const
{
    cout << "Error: World::getShipIds is not implemented" << endl;
    assert(sphere_radius >= 0.0);
    
    return vector<PhysicsObjectId>();
}

bool World :: isAlive (const PhysicsObjectId& id) const
{
    //cout << "Error: World::isAlive is not implemented" << endl;
    //return false;
    
    if (planet.getId() == id) planet.isAlive();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].isAlive();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].isAlive();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].isAlive();
    }
    
    return false;
}

Vector3 World :: getPosition (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (planet.getId() == id) planet.getPosition();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].getPosition();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].getPosition();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].getPosition();
    }
    
    return Vector3::ZERO;
}

double World :: getRadius (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (planet.getId() == id) planet.getRadius();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].getRadius();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].getRadius();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].getRadius();
    }
    
    return 0.0;
}

Vector3 World :: getVelocity (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (planet.getId() == id) planet.getVelocity();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].getVelocity();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].getVelocity();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].getVelocity();
    }
    
    return Vector3::ZERO;
}

double World :: getSpeed (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (planet.getId() == id) planet.getSpeed();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].getSpeed();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].getSpeed();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].getSpeed();
    }
    
    return 0.0;
}

Vector3 World :: getForward (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (planet.getId() == id) planet.getForward();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].getForward();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].getForward();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].getForward();
    }
    
    return Vector3::UNIT_X_PLUS;
}

Vector3 World :: getUp (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (planet.getId() == id) planet.getUp();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].getUp();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].getUp();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].getUp();
    }
    
    return Vector3::UNIT_Y_PLUS;
}

Vector3 World :: getRight (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (planet.getId() == id) planet.getRight();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) moons[i].getRight();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) ships[i].getRight();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) bullets[i].getRight();
    }
    
    return Vector3::UNIT_Z_PLUS;
}

bool World :: isPlanetoidMoon (const PhysicsObjectId& id) const
{
    assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
    assert(isAlive(id));
    
    return (getPlanetId() == id);
}

double World :: getPlanetoidRingDistance (const PhysicsObjectId& id) const
{
    cout << "Error: World::getPlanetoidRingDistance is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
    assert(isAlive(id));
    assert(isPlanetoidMoon(id));
    
    return 0.0;
}

unsigned int World :: getPlanetoidOwner (const PhysicsObjectId& id) const
{
    cout << "Error: World::getPlanetoidOwner is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
    assert(isAlive(id));
    
    return PhysicsObjectId::FLEET_NATURE;
}

bool World :: isPlanetoidActivelyClaimed (const PhysicsObjectId& id) const
{
    cout << "Error: World::isPlanetoidActivelyClaimed is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
    assert(isAlive(id));
    
    return false;
}

bool World :: isShipCommandShip (const PhysicsObjectId& id) const
{
    cout << "Error: World::isShipCommand is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    return false;
}

double World :: getShipSpeedMax (const PhysicsObjectId& id) const
{
    cout << "Error: World::getShipSpeedMax is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    return 1.0;
}

double World :: getShipAcceleration (const PhysicsObjectId& id) const
{
    cout << "Error: World::getShipAcceleration is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    return 0.0;
}

double World :: getShipRotationRate (const PhysicsObjectId& id) const
{
    cout << "Error: World::getShipRotationRate is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    return 0.0;
}

float World :: getShipHealthCurrent (const PhysicsObjectId& id) const
{
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getHealth();
    }
    return 0.0;
}

float World :: getShipHealthMaximum (const PhysicsObjectId& id) const
{
    cout << "Error: World::getShipHealthMaximum is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    return 1.0f;
}

bool World :: isMissileOutOfFuel (const PhysicsObjectId& id) const
{
    cout << "Error: World::isMissileOutOfFuel is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_MISSILE);
    assert(isAlive(id));
    
    return 0.0f;
}

PhysicsObjectId World :: getMissileTarget (const PhysicsObjectId& id) const
{
    cout << "Error: World::getMissileTarget is not implemented" << endl;
    assert(id.m_type == PhysicsObjectId::TYPE_MISSILE);
    assert(isAlive(id));
    
    return PhysicsObjectId::ID_NOTHING;
}



void World :: addExplosion (const Vector3& position,
                            double size,
                            unsigned int type)
{
    assert(size >= 0.0);
    
    mp_explosion_manager->add(position, size);
    
    assert(invariant());
}

PhysicsObjectId World :: addBullet (const Vector3& position,
                                    const Vector3& forward,
                                    const PhysicsObjectId& source_id)
{
    assert(forward.isNormal());
    
    bullets[nextBullet].fire(position, forward, source_id);
    PhysicsObjectId b_id = bullets[nextBullet].getId();
    nextBullet = (nextBullet + 1) % BULLET_COUNT;
    
    assert(invariant());
    return b_id;
}

PhysicsObjectId World :: addMissile (const Vector3& position,
                                     const Vector3& forward,
                                     const PhysicsObjectId& source_id,
                                     const PhysicsObjectId& target_id)
{
    cout << "Error: World::addMissile is not implemented" << endl;
    assert(forward.isNormal());
    
    assert(invariant());
    return PhysicsObjectId::ID_NOTHING;
}
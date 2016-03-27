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
    return g_rings.getDensityAtPosition(position);;
}

vector<RingParticleData> World :: getRingParticles (const Vector3& sphere_center,
                                                                    double sphere_radius) const
{
    assert(sphere_radius >= 0.0);
    
    return g_rings.getRingParticles(sphere_center, sphere_radius);
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
    return planet.getId();
}

unsigned int World :: getMoonCount () const
{
    return MOON_COUNT;
}

PhysicsObjectId World :: getMoonId (unsigned int moon) const
{
    assert(moon < getMoonCount());
    return moons[moon].getId();
}

PhysicsObjectId World :: getNearestPlanetoidId (const Vector3& position) const
{
    int index = 0;
    double min_distance = position.getDistance(moons[0].getPosition());
    for (int i = 1; i < MOON_COUNT; i++)
    {
        double temp = position.getDistanceSquared(moons[i].getPosition());
        if (temp < min_distance * min_distance)
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
    assert(sphere_radius >= 0.0);
    
    vector<PhysicsObjectId> list;
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        Ship s = ships[i];
        if (!s.isAlive()) continue;
        
        double temp = sphere_center.getDistanceSquared(s.getPosition());
        if (temp < sphere_radius * sphere_radius)
        {
            list.push_back(s.getId());
        }
    }
    
    return list;
}

bool World :: isAlive (const PhysicsObjectId& id) const
{
    if (player_ship.getId() == id) return player_ship.isAlive();
    if (planet.getId() == id)
    {
        return planet.isAlive();
    }
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].isAlive();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].isAlive();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].isAlive();
    }
    
    return false;
}

Vector3 World :: getPosition (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getPosition();
    if (planet.getId() == id) return planet.getPosition();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].getPosition();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getPosition();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].getPosition();
    }
    
    return Vector3::ZERO;
}

double World :: getRadius (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getRadius();
    if (planet.getId() == id) return planet.getRadius();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].getRadius();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getRadius();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].getRadius();
    }
    
    return 0.0;
}

Vector3 World :: getVelocity (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getVelocity();
    if (planet.getId() == id) return planet.getVelocity();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].getVelocity();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getVelocity();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].getVelocity();
    }
    
    return Vector3::ZERO;
}

double World :: getSpeed (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getSpeed();
    if (planet.getId() == id) return planet.getSpeed();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].getSpeed();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getSpeed();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].getSpeed();
    }
    
    return 0.0;
}

Vector3 World :: getForward (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getForward();
    if (planet.getId() == id) return planet.getForward();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].getForward();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getForward();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].getForward();
    }
    
    return Vector3::UNIT_X_PLUS;
}

Vector3 World :: getUp (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getUp();
    if (planet.getId() == id) return planet.getUp();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].getUp();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getUp();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].getUp();
    }
    
    return Vector3::UNIT_Y_PLUS;
}

Vector3 World :: getRight (const PhysicsObjectId& id) const
{
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getRight();
    if (planet.getId() == id) return planet.getRight();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        if (moons[i].getId() == id) return moons[i].getRight();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id) return ships[i].getRight();
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].getId() == id) return bullets[i].getRight();
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
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getSpeedMax();
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id)
        {
            return ships[i].getSpeedMax();
        }
    }
    
    return 1.0;
}

double World :: getShipAcceleration (const PhysicsObjectId& id) const
{
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getAcceleration();
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id)
        {
            return ships[i].getAcceleration();
        }
    }
    
    return 0.0;
}

double World :: getShipRotationRate (const PhysicsObjectId& id) const
{
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getRotationRate();
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].getId() == id)
        {
            return ships[i].getRotationRate();
        }
    }
    
    return 0.0;
}

float World :: getShipHealthCurrent (const PhysicsObjectId& id) const
{
    assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
    assert(isAlive(id));
    
    if (player_ship.getId() == id) return player_ship.getHealth();
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
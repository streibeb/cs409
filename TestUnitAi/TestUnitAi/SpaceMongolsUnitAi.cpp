//
//  SpaceMongolsUnitAi.cpp
//

#include <cassert>

#include "WorldInterface.h"
#include "PhysicsObjectId.h"
#include "PhysicsObject.h"
#include "ShipAiInterface.h"
#include "Ship.h"
#include "Bullet.h"
#include "UnitAiSuperclass.h"
#include "SpaceMongolsUnitAi.h"

#include <limits>

using namespace SpaceMongols;



UnitAiMoonGuard :: UnitAiMoonGuard (const AiShipReference& ship,
                                    const WorldInterface& world,
                                    const PhysicsObjectId& id_moon)
		: UnitAiSuperclass(ship)
{
	assert(ship.isShip());
	assert(id_moon.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(world.isAlive(id_moon));
	assert(world.isPlanetoidMoon(id_moon));

    steeringBehaviour = new FleetName::SteeringBehaviour(ship.getId());
    moon = id_moon;
    scanCount = rand() % SCAN_COUNT_MAX;
    
}

UnitAiMoonGuard :: UnitAiMoonGuard (const UnitAiMoonGuard& original,
                                    const AiShipReference& ship)
		: UnitAiSuperclass(ship)
{
	assert(ship.isShip());

    steeringBehaviour = new FleetName::SteeringBehaviour(ship.getId());
    moon = original.moon;
    scanCount = rand() % SCAN_COUNT_MAX;
}

UnitAiMoonGuard :: ~UnitAiMoonGuard ()
{
}



///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from UnitAiSuperclass
//

UnitAiSuperclass* UnitAiMoonGuard :: getCloneForShip (const AiShipReference& ship) const
{
	assert(ship.isShip());

	return new UnitAiMoonGuard(*this, ship);
}

void UnitAiMoonGuard :: draw () const
{
	; // do nothing
}

void UnitAiMoonGuard :: updateForPause ()
{
	; // do nothing
}

void UnitAiMoonGuard :: run (const WorldInterface& world)
{
	assert(world.isAlive(getShipId()));

    scan(world);
    
    Vector3 v = getShip().getVelocity();
    Vector3 orig_velocity = getShip().getForward();
    if (nearestEnemyShip != PhysicsObjectId::ID_NOTHING)
    {
        v = chargeAtTarget(world, nearestEnemyShip, v);
        shootAtShip(world, nearestEnemyShip);
    }
    else
    {
        Vector3 moon_pos = world.getPosition(moon);
        double moon_radius = world.getRadius(moon);
        v = steeringBehaviour->patrolSphere(world,
                                            moon_pos,
                                            moon_radius,
                                            PLANETOID_AVOID_DISTANCE);
    }
    v = avoidShips(world, v);
    v = avoidRingParticles(world, v);

    getShipAi().setDesiredVelocity(v);
}

void UnitAiMoonGuard::scan(const WorldInterface& world)
{
    scanCount++;
    if (scanCount == SCAN_COUNT_MAX)
    {
        printf("Scanning...\n");
        Vector3 ship_pos = getShip().getPosition();
        
        nearbyShips = world.getShipIds(ship_pos, SCAN_DISTANCE_SHIP);
        getClosestShip(world);
        getClosestEnemyShip(world);
        
        Vector3 position = ship_pos + (getShip().getForward() * 500.f);
        nearbyRingParticles = world.getRingParticles(position, SCAN_DISTANCE_RING_PARTICLE);
        
        nearestPlanetoid = world.getNearestPlanetoidId(ship_pos);
        
        scanCount = 0;
    }
}

void UnitAiMoonGuard::getClosestShip(const WorldInterface& world)
{
    PhysicsObjectId nearestShip;
    Vector3 ship_pos = getShip().getPosition();
    
    double min_distance = std::numeric_limits<double>::max();
    nearestShip = PhysicsObjectId::ID_NOTHING;
    for (int i = 0; i < nearbyShips.size(); i++)
    {
        if (!world.isAlive(nearbyShips[i])) continue;
        if (nearbyShips[i] == getShipId()) continue;
        
        double temp = ship_pos.getDistanceSquared(world.getPosition(nearbyShips[i]));
        if (temp < min_distance)
        {
            min_distance = temp;
            nearestShip = nearbyShips[i];
        }
    }
    
    this->nearestShip = nearestShip;
}

void UnitAiMoonGuard::getClosestEnemyShip(const WorldInterface& world)
{
    PhysicsObjectId nearestShip;
    Vector3 ship_pos = getShip().getPosition();
    
    double min_distance = std::numeric_limits<double>::max();
    nearestShip = PhysicsObjectId::ID_NOTHING;
    for (int i = 0; i < nearbyShips.size(); i++)
    {
        if (!world.isAlive(nearbyShips[i])) continue;
        if (nearbyShips[i].m_fleet == getShipId().m_fleet) continue;
        
        double temp = ship_pos.getDistanceSquared(world.getPosition(nearbyShips[i]));
        if (temp < min_distance)
        {
            min_distance = temp;
            nearestShip = nearbyShips[i];
        }
    }
    
    this->nearestEnemyShip = nearestShip;
}

void UnitAiMoonGuard::shootAtShip(const WorldInterface& world, const PhysicsObjectId& target)
{
    if (!world.isAlive(target)) return;
    
    Vector3 target_pos = world.getPosition(target);
    Vector3 target_vel = world.getVelocity(target);
    
    Vector3 aim_dir = steeringBehaviour->getAimDirection(getShip().getPosition(),
                                                         Bullet::SPEED,
                                                         target_pos,
                                                         target_vel);
    double angle = getShip().getVelocity().getAngleSafe(aim_dir);
    if (angle <= SHOOT_ANGLE_RADIANS_MAX && getShip().isReloaded())
    {
        getShipAi().markFireBulletDesired();
        printf("\tShooting at (%f, %f, %f)\n", target_pos.x, target_pos.y, target_pos.z);
    };
}

Vector3 UnitAiMoonGuard::chargeAtTarget(const WorldInterface& world, const PhysicsObjectId& target, const Vector3& orig_velocity)
{
    if (!world.isAlive(target))
    {
        return orig_velocity;
    }
    
    Vector3 unit_pos = getShip().getPosition();
    Vector3 target_pos = world.getPosition(target);
    
    //Vector3 desired_velocity = steeringBehaviour->seek(world, target);
    Vector3 desired_velocity = steeringBehaviour->aim(world, target, Bullet::SPEED);
    Vector3 v = avoidPlanetoids(world, desired_velocity);

    printf("\tRamming\n");
    return v;
}

Vector3 UnitAiMoonGuard::avoidShips(const WorldInterface &world, const Vector3& orig_velocity)
{
    if (nearestShip == PhysicsObjectId::ID_NOTHING || !world.isAlive(nearestShip))
    {
        return orig_velocity;
    }
    
    Vector3 target_pos = world.getPosition(nearestShip);
    double target_radius = world.getRadius(nearestShip);
    
    Vector3 v = steeringBehaviour->avoid(world,
                                         orig_velocity,
                                         target_pos,
                                         target_radius,
                                         SHIP_CLEARANCE,
                                         SHIP_AVOID_DISTANCE);
    printf("\tAvoiding Ships\n");
    return v;
}

Vector3 UnitAiMoonGuard::avoidRingParticles(const WorldInterface& world, const Vector3& orig_velocity)
{
    if (nearbyRingParticles.size() == 0)
    {
        return orig_velocity;
    }
        
    RingParticleData nearestParticle;
    Vector3 ship_pos = getShip().getPosition();
    
    double min_distance = ship_pos.getDistanceSquared(nearbyRingParticles[0].m_position);
    nearestParticle = nearbyRingParticles[0];
    
    for (int i = 1; i < nearbyRingParticles.size(); i++)
    {
        double temp = ship_pos.getDistanceSquared(nearbyRingParticles[0].m_position);
        if (temp < min_distance)
        {
            min_distance = temp;
            nearestParticle = nearbyRingParticles[i];
        }
    }
    
    Vector3 v = steeringBehaviour->avoid(world,
                                         orig_velocity,
                                         nearestParticle.m_position,
                                         nearestParticle.m_radius,
                                         RING_PARTICLE_CLEARANCE,
                                         RING_PARTICLE_AVOID_DISTANCE);
    
    double ringDensity = world.getRingDensity(ship_pos);
    if (ringDensity >= 1)
    {
        v -= (v * ringDensity/100);
    }
    
    printf("Avoiding Ring Particles\n");
    return v;
}

Vector3 UnitAiMoonGuard::avoidPlanetoids(const WorldInterface &world, const Vector3& orig_velocity)
{
    Vector3 planetoid_pos = world.getPosition(nearestPlanetoid);
    double planetoid_radius = world.getRadius(nearestPlanetoid);
    
    Vector3 v = steeringBehaviour->avoid(world,
                                         orig_velocity,
                                         planetoid_pos,
                                         planetoid_radius,
                                         PLANETOID_CLEARANCE,
                                         PLANETOID_AVOID_DISTANCE);
    printf("Avoiding Planetoid\n");
    return v;
}

///////////////////////////////////////////////////////////////
//
//  Helper functions not inherited from anywhere
//

//
//  Copy Constructor
//  Assignment Operator
//
//  These functions have intentionally not been implemented.
//    Use the modified copy constructor instead.
//
//  UnitAiMoonGuard :: UnitAiMoonGuard (const UnitAiMoonGuard& original);
//  UnitAiMoonGuard& UnitAiMoonGuard :: operator= (const UnitAiMoonGuard& original);
//

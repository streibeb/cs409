//
//  WorldTestUnitAi2.cpp
//
//  This file contains the part of the WorldTestUnitAi class
//    that implements the WorldInterface interface.  The rest of
//    the WorldTestUnitAi class is in WorldTestUnitAi.cpp.
//

#include <cassert>
#include <iostream>

#include "Pi.h"
#include "ObjLibrary/Vector3.h"

#include "TimeSystem.h"
#include "Orrery.h"
#include "ExplosionManagerInterface.h"
#include "PhysicsObjectId.h"
#include "WorldInterface.h"
#include "WorldTestUnitAi.h"

using namespace std;
namespace
{
	const double EXPLOSION_SCALE = 2.0;

}  // end of anonymous namespace



double WorldTestUnitAi :: getRingDensity (const Vector3& position) const
{
	static const double CLUMP_VOLUME = FOUR_THIRD_PI * RING_CLUMP_RADIUS * RING_CLUMP_RADIUS * RING_CLUMP_RADIUS;
	static const double DENSITY_FACTOR = 1.0e9 / (CLUMP_VOLUME * 2.0);

	double distance = position.getDistance(getNearerRingClump(position));
	if(distance < RING_CLUMP_RADIUS)
		return m_ring_particle_count * DENSITY_FACTOR;
	else
		return 0.0;
}

vector<RingParticleData> WorldTestUnitAi ::getRingParticles (
	const Vector3& sphere_center,
	double sphere_radius) const
{
	assert(sphere_radius >= 0.0);

	vector<RingParticleData> v_particles;
	for(unsigned int i = 0; i < m_ring_particle_count; i++)
	{
		assert(i < RING_PARTICLE_COUNT_MAX);
		double distance_max = sphere_radius + ma_ring_particles[i].m_radius;
		if(ma_ring_particles[i].m_position.isDistanceLessThan(sphere_center, distance_max))
			v_particles.push_back(ma_ring_particles[i]);
	}

	return v_particles;
}

unsigned int WorldTestUnitAi :: getFleetCount () const
{
	return 3;
}

float WorldTestUnitAi :: getFleetScore (unsigned int fleet) const
{
	cout << "Error: WorldTestUnitAi::getFleetScore is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return 0.0f;
}

bool WorldTestUnitAi :: isFleetAlive (unsigned int fleet) const
{
	cout << "Error: WorldTestUnitAi::isFleetAlive is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return false;
}

PhysicsObjectId WorldTestUnitAi :: getFleetCommandShipId (unsigned int fleet) const
{
	cout << "Error: WorldTestUnitAi::getFleetCommandShipId is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return PhysicsObjectId::ID_NOTHING;
}

vector<PhysicsObjectId> WorldTestUnitAi :: getFleetFighterIds (unsigned int fleet) const
{
	cout << "Error: WorldTestUnitAi::getFleetFighterIds is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return vector<PhysicsObjectId>();
}

vector<PhysicsObjectId> WorldTestUnitAi :: getFleetMissileIds (unsigned int fleet) const
{
	cout << "Error: WorldTestUnitAi::getFleetMissileIds is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return vector<PhysicsObjectId>();
}

PhysicsObjectId WorldTestUnitAi :: getPlanetId () const
{
	cout << "Error: WorldTestUnitAi::getPlanetId is not implemented" << endl;
	return PhysicsObjectId::ID_NOTHING;
}

unsigned int WorldTestUnitAi :: getMoonCount () const
{
	return 1;
}

PhysicsObjectId WorldTestUnitAi :: getMoonId (unsigned int moon) const
{
	assert(moon < getMoonCount());

	return ID_MOON;
}

PhysicsObjectId WorldTestUnitAi :: getNearestPlanetoidId (const Vector3& position) const
{
	return ID_MOON;
}

vector<PhysicsObjectId> WorldTestUnitAi :: getShipIds (const Vector3& sphere_center,
                                                       double sphere_radius) const
{
	assert(sphere_radius >= 0.0);

	vector<PhysicsObjectId> v_ids;

	if(isAlive(m_target.getId()) &&
	   m_target.getPosition().isDistanceLessThan(sphere_center,
	                                             sphere_radius + m_target.getRadius()))
	{
		v_ids.push_back(ID_TARGET);
	}

	if(m_agent.isAlive() &&
	   m_agent.getPosition().isDistanceLessThan(sphere_center,
	                                            sphere_radius + m_agent.getRadius()))
	{
		v_ids.push_back(ID_AGENT);
	}

	return v_ids;
}

bool WorldTestUnitAi :: isAlive (const PhysicsObjectId& id) const
{
	if(id == ID_MOON)
		return true;
	if(id == ID_TARGET && m_target_disappear_time <= 0.0)
		return true;
	if(id == ID_AGENT)
		return true;
	return false;
}

Vector3 WorldTestUnitAi :: getPosition (const PhysicsObjectId& id) const
{
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getPosition();
	if(id == ID_AGENT)
		return m_agent.getPosition();
	return Vector3::ZERO;
}

double WorldTestUnitAi :: getRadius (const PhysicsObjectId& id) const
{
	assert(isAlive(id));

	if(id == ID_MOON)
		return MOON_RADIUS;
	if(id == ID_TARGET)
		return m_target.getRadius();
	if(id == ID_AGENT)
		return m_agent.getRadius();
	return 0.0;
}

Vector3 WorldTestUnitAi :: getVelocity (const PhysicsObjectId& id) const
{
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getVelocity();
	if(id == ID_AGENT)
		return m_agent.getVelocity();
	return Vector3::ZERO;
}

double WorldTestUnitAi :: getSpeed (const PhysicsObjectId& id) const
{
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getSpeed();
	if(id == ID_AGENT)
		return m_agent.getSpeed();
	return 0.0;
}

Vector3 WorldTestUnitAi :: getForward (const PhysicsObjectId& id) const
{
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getForward();
	if(id == ID_AGENT)
		return m_agent.getForward();
	return Vector3::UNIT_X_PLUS;
}

Vector3 WorldTestUnitAi :: getUp (const PhysicsObjectId& id) const
{
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getUp();
	if(id == ID_AGENT)
		return m_agent.getUp();
	return Vector3::UNIT_Y_PLUS;
}

Vector3 WorldTestUnitAi :: getRight (const PhysicsObjectId& id) const
{
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getRight();
	if(id == ID_AGENT)
		return m_agent.getRight();
	return Vector3::UNIT_Z_PLUS;
}

bool WorldTestUnitAi :: isPlanetoidMoon (const PhysicsObjectId& id) const
{
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));

	return true;
}

double WorldTestUnitAi :: getPlanetoidRingDistance (const PhysicsObjectId& id) const
{
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));

	return RING_CLUMP_DISTANCE - RING_CLUMP_RADIUS;
}

unsigned int WorldTestUnitAi :: getPlanetoidOwner (const PhysicsObjectId& id) const
{
	cout << "Error: WorldTestUnitAi::getMoonOwner is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));

	return PhysicsObjectId::FLEET_NATURE;
}

bool WorldTestUnitAi :: isPlanetoidActivelyClaimed (const PhysicsObjectId& id) const
{
	cout << "Error: WorldTestUnitAi::isMoonActivelyClaimed is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));

	return false;
}

bool WorldTestUnitAi :: isShipCommandShip (const PhysicsObjectId& id) const
{
	cout << "Error: WorldTestUnitAi::isShipCommand is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return false;
}

double WorldTestUnitAi :: getShipSpeedMax (const PhysicsObjectId& id) const
{
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getSpeedMax();
	if(id == ID_AGENT)
		return m_agent.getSpeedMax();
	return 0.0;
}

double WorldTestUnitAi :: getShipAcceleration (const PhysicsObjectId& id) const
{
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getAcceleration();
	if(id == ID_AGENT)
		return m_agent.getAcceleration();
	return 0.0;
}

double WorldTestUnitAi :: getShipRotationRate (const PhysicsObjectId& id) const
{
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	if(id == ID_TARGET)
		return m_target.getRotationRate();
	if(id == ID_AGENT)
		return m_agent.getRotationRate();
	return 0.0;
}

float WorldTestUnitAi :: getShipHealthCurrent (const PhysicsObjectId& id) const
{
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return 1.0;
}

float WorldTestUnitAi :: getShipHealthMaximum (const PhysicsObjectId& id) const
{
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return 1.0;
}

bool WorldTestUnitAi :: isMissileOutOfFuel (const PhysicsObjectId& id) const
{
	cout << "Error: WorldTestUnitAi::isMissileOutOfFuel is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_MISSILE);
	assert(isAlive(id));

	return false;
}

PhysicsObjectId WorldTestUnitAi :: getMissileTarget (const PhysicsObjectId& id) const
{
	cout << "Error: WorldTestUnitAi::getMissileTarget is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_MISSILE);
	assert(isAlive(id));

	return PhysicsObjectId::ID_NOTHING;
}



void WorldTestUnitAi :: addExplosion (const Vector3& position,
                                      double size,
                                      unsigned int type)
{
	assert(size >= 0.0);

	mp_explosion_manager->add(position, size * EXPLOSION_SCALE);

	assert(invariant());
}

PhysicsObjectId WorldTestUnitAi :: addBullet (const Vector3& position,
                                              const Vector3& forward,
                                              const PhysicsObjectId& source_id)
{
	assert(forward.isNormal());

	ma_bullets[m_next_bullet].fire(position, forward, source_id);
	PhysicsObjectId bullet_id(PhysicsObjectId::TYPE_BULLET,
	                          PhysicsObjectId::FLEET_ENEMY,
	                          m_next_bullet);
	m_next_bullet = (m_next_bullet + 1) % BULLET_COUNT_MAX;
	m_agent_bullet_count++;

	assert(invariant());
	return bullet_id;
}

PhysicsObjectId WorldTestUnitAi :: addMissile (const Vector3& position,
                                               const Vector3& forward,
                                               const PhysicsObjectId& source_id,
                                               const PhysicsObjectId& target_id)
{
	cout << "Warning: WorldTestUnitAi::addMissile adds bullets" << endl;
	assert(forward.isNormal());

	assert(invariant());
	return addBullet(position, forward, source_id);
}


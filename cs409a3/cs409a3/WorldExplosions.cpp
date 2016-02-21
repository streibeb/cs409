//
//  WorldExplosions.cpp
//

#include <cassert>
#include <iostream>

#include "../../ObjLibrary/Vector3.h"

#include "PhysicsObjectId.h"
#include "PhysicsObject.h"
#include "ExplosionManagerInterface.h"
#include "ExplosionManager.h"
#include "WorldInterface.h"
#include "WorldExplosions.h"

using namespace std;



WorldExplosions :: WorldExplosions ()
		: mp_explosion_manager(new ExplosionManager())
{
	assert(invariant());
}

WorldExplosions :: WorldExplosions (const WorldExplosions& original)
		: mp_explosion_manager(original.mp_explosion_manager->getClone())
{
	assert(invariant());
}

WorldExplosions :: ~WorldExplosions ()
{
	delete mp_explosion_manager;
}

WorldExplosions& WorldExplosions :: operator= (const WorldExplosions& original)
{
	if(&original != this)
	{
		delete mp_explosion_manager;

		mp_explosion_manager = original.mp_explosion_manager->getClone();
	}

	assert(invariant());
	return *this;
}



bool WorldExplosions :: isInitialized () const
{
	assert(mp_explosion_manager != NULL);
	return mp_explosion_manager->isInitialized();
}

void WorldExplosions :: draw (const Vector3& camera_forward,
                              const Vector3& camera_up) const
{
	assert(isInitialized());
	assert(camera_forward.isNormal());
	assert(camera_up.isNormal());
	assert(camera_forward.isOrthogonal(camera_up));

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->draw(camera_forward, camera_up);
}



void WorldExplosions :: init ()
{
	if(!isInitialized())
	{
		assert(mp_explosion_manager != NULL);
		mp_explosion_manager->init("Explode1.png", 15);
	}

	assert(invariant());
}

void WorldExplosions :: reset ()
{
	assert(isInitialized());

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->removeAll();

	assert(invariant());
}

void WorldExplosions :: updateAll ()
{
	assert(isInitialized());

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->update();

	assert(invariant());
}



///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from WorldInterface
//

double WorldExplosions :: getRingDensity (const Vector3& position) const
{
	cout << "Error: WorldExplosions::getRingDensity is not implemented" << endl;
	return 0.0f;
}

vector<WorldInterface::RingParticleData> WorldExplosions :: getRingParticles (const Vector3& sphere_center,
                                                                              double sphere_radius) const
{
	cout << "Error: WorldExplosions::getRingParticles is not implemented" << endl;
	assert(sphere_radius >= 0.0);

	return vector<RingParticleData>();
}

unsigned int WorldExplosions :: getFleetCount () const
{
	cout << "Error: WorldExplosions::getFleetCount is not implemented" << endl;
	return 0;
}

float WorldExplosions :: getFleetScore (unsigned int fleet) const
{
	cout << "Error: WorldExplosions::getFleetScore is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return 0;
}

bool WorldExplosions :: isFleetAlive (unsigned int fleet) const
{
	cout << "Error: WorldExplosions::isFleetAlive is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return false;
}

PhysicsObjectId WorldExplosions :: getFleetCommandShipId (unsigned int fleet) const
{
	cout << "Error: WorldExplosions::getFleetCommandShipId is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return PhysicsObjectId::ID_NOTHING;
}

vector<PhysicsObjectId> WorldExplosions :: getFleetFighterIds (unsigned int fleet) const
{
	cout << "Error: WorldExplosions::getFleetFighterIds is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return vector<PhysicsObjectId>();
}

vector<PhysicsObjectId> WorldExplosions :: getFleetMissileIds (unsigned int fleet) const
{
	cout << "Error: WorldExplosions::getFleetMissileIds is not implemented" << endl;
	assert(fleet <  getFleetCount());
	assert(fleet != PhysicsObjectId::FLEET_NATURE);

	return vector<PhysicsObjectId>();
}

PhysicsObjectId WorldExplosions :: getPlanetId () const
{
	cout << "Error: WorldExplosions::getPlanetId is not implemented" << endl;
	return PhysicsObjectId::ID_NOTHING;
}

unsigned int WorldExplosions :: getMoonCount () const
{
	cout << "Error: WorldExplosions::getMoonCount is not implemented" << endl;
	return 0;
}

PhysicsObjectId WorldExplosions :: getMoonId (unsigned int moon) const
{
	cout << "Error: WorldExplosions::getMoonId is not implemented" << endl;
	assert(moon < getMoonCount());

	return PhysicsObjectId::ID_NOTHING;
}

PhysicsObjectId WorldExplosions :: getNearestPlanetoidId (const Vector3& position) const
{
	cout << "Error: WorldExplosions::getPlanetId is not implemented" << endl;
	return PhysicsObjectId::ID_NOTHING;
}

vector<PhysicsObjectId> WorldExplosions :: getShipIds (const Vector3& sphere_center,
                                                       double sphere_radius) const
{
	cout << "Error: WorldExplosions::getShipIds is not implemented" << endl;
	assert(sphere_radius >= 0.0);

	return vector<PhysicsObjectId>();
}

bool WorldExplosions :: isAlive (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::isAlive is not implemented" << endl;
	return false;
}

Vector3 WorldExplosions :: getPosition (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getPosition is not implemented" << endl;
	assert(isAlive(id));

	return Vector3::ZERO;
}

double WorldExplosions :: getRadius (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getRadius is not implemented" << endl;
	assert(isAlive(id));

	return 0.0;
}

Vector3 WorldExplosions :: getVelocity (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getVelocity is not implemented" << endl;
	assert(isAlive(id));

	return Vector3::ZERO;
}

double WorldExplosions :: getSpeed (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getSpeed is not implemented" << endl;
	assert(isAlive(id));

	return 0.0;
}

Vector3 WorldExplosions :: getForward (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getForward is not implemented" << endl;
	assert(isAlive(id));

	return Vector3::UNIT_X_PLUS;
}

Vector3 WorldExplosions :: getUp (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getUp is not implemented" << endl;
	assert(isAlive(id));

	return Vector3::UNIT_Y_PLUS;
}

Vector3 WorldExplosions :: getRight (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getRight is not implemented" << endl;
	assert(isAlive(id));

	return Vector3::UNIT_Z_PLUS;
}

bool WorldExplosions :: isPlanetoidMoon (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::isPlanetoidMoon is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));

	return false;
}

double WorldExplosions :: getPlanetoidRingDistance (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getPlanetoidRingDistance is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));
	assert(isPlanetoidMoon(id));

	return 0.0;
}

unsigned int WorldExplosions :: getPlanetoidOwner (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getPlanetoidOwner is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));

	return PhysicsObjectId::FLEET_NATURE;
}

bool WorldExplosions :: isPlanetoidActivelyClaimed (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::isPlanetoidActivelyClaimed is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_PLANETOID);
	assert(isAlive(id));

	return false;
}

bool WorldExplosions :: isShipCommandShip (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::isShipCommand is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return false;
}

double WorldExplosions :: getShipSpeedMax (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getShipSpeedMax is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return 1.0;
}

double WorldExplosions :: getShipAcceleration (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getShipAcceleration is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return 0.0;
}

double WorldExplosions :: getShipRotationRate (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getShipRotationRate is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return 0.0;
}

float WorldExplosions :: getShipHealthCurrent (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getShipHealthCurrent is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return 0.0f;
}

float WorldExplosions :: getShipHealthMaximum (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getShipHealthMaximum is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_SHIP);
	assert(isAlive(id));

	return 1.0f;
}

bool WorldExplosions :: isMissileOutOfFuel (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::isMissileOutOfFuel is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_MISSILE);
	assert(isAlive(id));

	return 0.0f;
}

PhysicsObjectId WorldExplosions :: getMissileTarget (const PhysicsObjectId& id) const
{
	cout << "Error: WorldExplosions::getMissileTarget is not implemented" << endl;
	assert(id.m_type == PhysicsObjectId::TYPE_MISSILE);
	assert(isAlive(id));

	return PhysicsObjectId::ID_NOTHING;
}



void WorldExplosions :: addExplosion (const Vector3& position,
                                      double size,
                                      unsigned int type)
{
	assert(size >= 0.0);

	mp_explosion_manager->add(position, size);

	assert(invariant());
}

PhysicsObjectId WorldExplosions :: addBullet (const Vector3& position,
                                              const Vector3& forward,
                                              const PhysicsObjectId& source_id)
{
	cout << "Error: WorldExplosions::addBullet is not implemented" << endl;
	assert(forward.isNormal());

	assert(invariant());
	return PhysicsObjectId::ID_NOTHING;
}

PhysicsObjectId WorldExplosions :: addMissile (const Vector3& position,
                                               const Vector3& forward,
                                               const PhysicsObjectId& source_id,
                                               const PhysicsObjectId& target_id)
{
	cout << "Error: WorldExplosions::addMissile is not implemented" << endl;
	assert(forward.isNormal());

	assert(invariant());
	return PhysicsObjectId::ID_NOTHING;
}



///////////////////////////////////////////////////////////////
//
//  Helper function not inherited from anywhere
//

bool WorldExplosions :: invariant () const
{
	if(mp_explosion_manager == NULL) return false;
	return true;
}

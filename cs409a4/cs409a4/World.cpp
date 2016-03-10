//
//  World.cpp
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



World :: World ()
		: mp_explosion_manager(new ExplosionManager())
{
	assert(invariant());
}

World :: World (const World& original)
		: mp_explosion_manager(original.mp_explosion_manager->getClone())
{
	assert(invariant());
}

World :: ~World ()
{
	delete mp_explosion_manager;
}

World& World :: operator= (const World& original)
{
	if(&original != this)
	{
		delete mp_explosion_manager;

		mp_explosion_manager = original.mp_explosion_manager->getClone();
	}

	assert(invariant());
	return *this;
}



bool World :: isInitialized () const
{
	assert(mp_explosion_manager != NULL);
	return mp_explosion_manager->isInitialized();
}

void World :: draw (const Vector3& camera_forward,
                              const Vector3& camera_up) const
{
	assert(isInitialized());
	assert(camera_forward.isNormal());
	assert(camera_up.isNormal());
	assert(camera_forward.isOrthogonal(camera_up));
    
    planet.draw();
    for (int i = 0; i < MOON_COUNT; i++)
    {
        moons[i].draw();
    }
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        ships[i].draw();
    }
    
	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->draw(camera_forward, camera_up);
}



void World :: init ()
{
	if(!isInitialized())
	{
        // Skybox Init
        ObjModel sb;
        sb.load("Models/Skybox.obj");
        skybox = sb.getDisplayList();
        
        // Planet Init
        ObjModel p;
        p.load(planetInfo.filename);
        DisplayList p_dl = p.getDisplayList();
        PhysicsObjectId p_id = PhysicsObjectId(PhysicsObjectId::TYPE_PLANETOID,
                                             PhysicsObjectId::FLEET_NATURE,
                                             0);
        planet.initPlanetoid(p_id, planetInfo.position, planetInfo.radius, p_dl, 1.f);
        
        // Moon Init
        for (int i = 0; i < MOON_COUNT; i++)
        {
            ObjModel m;
            m.load(moonInfo[i].filename);
            DisplayList m_dl = m.getDisplayList();
            PhysicsObjectId m_id = PhysicsObjectId(PhysicsObjectId::TYPE_PLANETOID,
                                                   PhysicsObjectId::FLEET_NATURE,
                                                   i + 1);
            moons[i].initPlanetoid(m_id, moonInfo[i].position, moonInfo[i].radius, m_dl, 1.f);
        }
        
        // Ship Init
        ObjModel s;
        s.load("Models/Grapple.obj");
        DisplayList s_dl = s.getDisplayList();
        for (int i = 0; i < SHIP_COUNT; i++)
        {
            PhysicsObjectId s_id = PhysicsObjectId(PhysicsObjectId::TYPE_SHIP,
                                                   PhysicsObjectId::FLEET_ENEMY,
                                                   i);
            
            int moonIndex = rand() % MOON_COUNT;
            Vector3 pos = moonInfo[moonIndex].position
                            + Vector3::getRandomUnitVector()
                            * (moonInfo[moonIndex].radius + 500.0);
            
            ships[i].initPhysics(s_id, pos, 100.f, Vector3::getRandomUnitVector(), s_dl, 1.f);
            ships[i].setHealth(1);
            ships[i].setAmmo(0);
            ships[i].setSpeed(0.f);
        }
        
        // Add Bullets
        ObjModel b;
        b.load("Models/Bolt.obj");
        DisplayList b_dl = b.getDisplayList();
        for (int i = 0; i < BULLET_COUNT; i++)
        {
            PhysicsObjectId b_id = PhysicsObjectId(PhysicsObjectId::TYPE_BULLET,
                                                   PhysicsObjectId::FLEET_MAX,
                                                   i);
            bullets[i].initPhysics(b_id, {0, 0, 0}, 1.f, {0, 0, 0}, b_dl, 2.5f);
        }

		assert(mp_explosion_manager != NULL);
		mp_explosion_manager->init("Explode1.png", 15);
	}

	assert(invariant());
}

void World :: reset ()
{
	assert(isInitialized());

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->removeAll();

	assert(invariant());
}

void World :: updateAll ()
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
    if (player_ship.getId() == id) player_ship.isAlive();
    
    // TODO: Add Bullets
    
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
    if (player_ship.getId() == id) player_ship.getPosition();
    
    // TODO: Add Bullets
    
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
    if (player_ship.getId() == id) player_ship.getRadius();
    
    // TODO: Add Bullets
    
    return 0.0;
}

Vector3 World :: getVelocity (const PhysicsObjectId& id) const
{
	cout << "Error: World::getVelocity is not implemented" << endl;
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
    if (player_ship.getId() == id) player_ship.getVelocity();
    
    // TODO: Add Bullets


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
    if (player_ship.getId() == id) player_ship.getSpeed();
    
    // TODO: Add Bullets


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
    if (player_ship.getId() == id) player_ship.getForward();
    
    // TODO: Add Bullets

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
    if (player_ship.getId() == id) player_ship.getUp();
    
    // TODO: Add Bullets


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
    if (player_ship.getId() == id) player_ship.getRight();
    
    // TODO: Add Bullets


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
	cout << "Error: World::addBullet is not implemented" << endl;
	assert(forward.isNormal());

	assert(invariant());
	return PhysicsObjectId::ID_NOTHING;
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



///////////////////////////////////////////////////////////////
//
//  Helper function not inherited from anywhere
//

bool World :: invariant () const
{
	if(mp_explosion_manager == NULL) return false;
	return true;
}

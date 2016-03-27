//
//  WorldTestUnitAi.cpp
//
//  This file contains the parts of the WorldTestUnitAi class
//    other than the implementation of the WorldInterface
//    interface.  The implementation of the WorldInterface
//    interface is in WorldTestUnitAi2.cpp.
//

#include <cassert>
#include "GetGlut.h"

#include "Pi.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"

#include "TimeSystem.h"
#include "Orrery.h"
#include "ExplosionManagerInterface.h"
#include "ExplosionManager.h"
#include "PhysicsObjectId.h"
#include "Ship.h"
#include "UnitAiSuperclass.h"
#include "WorldInterface.h"
#include "WorldTestUnitAi.h"

#include "ExplosionManager.h"
//#include "RedDuckUnitAi.h"  // REPLACE WITH YOUR OWN UNIT AI
#include "SpaceMongolsUnitAi.h"

namespace
{
	bool g_is_models_loaded = false;

	const double TARGET_SPEED_MAX     = 500.0;    // m / s
	const double TARGET_ACCELERATION  = 200.0;    // m / s^2
	const double TARGET_ROTATION_RATE = HALF_PI;  // radians per second

	const double AGENT_START_ABOVE_MOON = 1000.0;
	const double AGENT_SPEED_MAX        =  300.0;    // m / s
	const double AGENT_ACCELERATION     =  250.0;    // m / s^2
	const double AGENT_ROTATION_RATE    = THIRD_PI;  // radians per second

	const double RING_PARTICLE_SIZE_MIN =  10.0;
	const double RING_PARTICLE_SIZE_MAX = 100.0;

	const double SHIP_SCALE   = 20.0;
	const double BULLET_SCALE = 10.0;
	DisplayList g_moon_list;
	DisplayList g_ship_list;
	DisplayList g_ring_particle_list;
	DisplayList g_bullet_list;

	const double TARGET_HIT_EXPPLOSION_SIZE = 100.0;  // multiplied by EXPLOSION_SCALE

	const unsigned int TYPE_RING_PARTICLE = 4;


	inline double random0 ()
	{
		return rand () / (RAND_MAX + 1.0);
	}

	inline double random2 (double min_value, double max_value)
	{
		assert(min_value <= max_value);

		return min_value + random0() * (max_value - min_value);
	}

}  // end of anonymous namespace



const PhysicsObjectId WorldTestUnitAi :: ID_MOON  (PhysicsObjectId::TYPE_PLANETOID, PhysicsObjectId::FLEET_NATURE, 0);
const PhysicsObjectId WorldTestUnitAi :: ID_TARGET(PhysicsObjectId::TYPE_SHIP,      PhysicsObjectId::FLEET_PLAYER, 0);
const PhysicsObjectId WorldTestUnitAi :: ID_AGENT (PhysicsObjectId::TYPE_SHIP,      PhysicsObjectId::FLEET_ENEMY,  0);

const double WorldTestUnitAi :: MOON_RADIUS               = 1000.0;
const double WorldTestUnitAi :: RING_CLUMP_DISTANCE       = 3000.0;
const double WorldTestUnitAi :: RING_CLUMP_RADIUS         = 1500.0;
const double WorldTestUnitAi :: TARGET_HIT_DISAPPEAR_TIME =    5.0;



bool WorldTestUnitAi :: isModelsLoaded ()
{
	return g_is_models_loaded;
}

void WorldTestUnitAi :: loadModels ()
{
	assert(!isModelsLoaded());

	if(!Orrery::isModelsInitialized())
		Orrery::initModels();
	assert(Orrery::isModelsInitialized());

	g_moon_list          = ObjModel("Models/MoonA.obj").getDisplayList();
	g_ship_list          = ObjModel("Models/Grapple.obj").getDisplayList();
	g_ring_particle_list = ObjModel("Models/RingParticleA0.obj").getDisplayList();
	g_bullet_list        = ObjModel("Models/Bolt.obj").getDisplayList();

	SimpleMarker::initModel();

	g_is_models_loaded = true;

	assert(isModelsLoaded());
}



WorldTestUnitAi :: WorldTestUnitAi ()
		: m_orrery(),
		  mp_explosion_manager(new ExplosionManager()),
		  m_ring_particle_count(RING_PARTICLE_COUNT_DEFAULT),
		  m_target(ID_TARGET,
		           Vector3::ZERO, Vector3::ZERO,
		           g_ship_list, SHIP_SCALE,
		           1.0f, 0,
		           TARGET_SPEED_MAX, TARGET_ACCELERATION, TARGET_ROTATION_RATE),
		  m_target_disappear_time(0.0),
		  m_target_shot_count(0),
		  m_target_collision_count(0),
		  m_agent(ID_AGENT,
		          Vector3::ZERO, Vector3::ZERO,
		          g_ship_list, SHIP_SCALE,
		          1.0f, 0,
		          AGENT_SPEED_MAX, AGENT_ACCELERATION, AGENT_ROTATION_RATE),
		  m_agent_collision_last(PhysicsObjectId::ID_NOTHING),
		  m_agent_collision_count(0),
		  m_agent_ai_time_cumulative(0.0f),
		  m_agent_bullet_count(0),
		  m_next_bullet(0),
		  m_next_simple_marker(0)
{
	assert(isModelsLoaded());

	static const Vector3 EPICYCLE1_AXIS = Vector3( 1.0, 4.0,  0.0).getNormalized();
	static const Vector3 EPICYCLE2_AXIS = Vector3( 0.0, 3.0,  1.0).getNormalized();
	static const Vector3 EPICYCLE5_AXIS = Vector3(-1.0, 0.0, -1.0).getNormalized();

	// main circle
	m_orrery.addEpicycle(Vector3( 0.0,  1.0,  0.0), 3000.0,  0.025, Vector3(0.75, 0.75, 0.75));
	// variantions in main
	m_orrery.addEpicycle(EPICYCLE1_AXIS,             750.0,  0.15,  Vector3(0.75, 0.25, 0.75));
	m_orrery.addEpicycle(EPICYCLE2_AXIS,             600.0,  0.2,   Vector3(0.25, 0.25, 0.75));
	// smaller dodging
	m_orrery.addEpicycle(Vector3(-1.0,  0.0,  0.0),  125.0,  0.5,   Vector3(0.25, 0.75, 0.75));
	m_orrery.addEpicycle(Vector3( 0.0,  0.0, -1.0),  100.0,  0.8,   Vector3(0.25, 0.75, 0.25));
	m_orrery.addEpicycle(EPICYCLE5_AXIS,              75.0,  1.0,   Vector3(0.75, 0.75, 0.25));
	m_orrery.addEpicycle(Vector3( 0.0, -1.0,  0.0),   50.0,  1.25,  Vector3(0.75, 0.25, 0.25));

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->init("Explode1.bmp", 15);

	initBullets();

	reset();

	assert(invariant());
}

WorldTestUnitAi :: WorldTestUnitAi (const WorldTestUnitAi& original)
		: m_orrery                  (original.m_orrery),
		  mp_explosion_manager      (original.mp_explosion_manager->getClone()),
		  m_ring_particle_count     (original.m_ring_particle_count),
		  m_target                  (original.m_target),
		  m_target_disappear_time   (original.m_target_disappear_time),
		  m_target_shot_count       (original.m_target_shot_count),
		  m_target_collision_count  (original.m_target_collision_count),
		  m_agent                   (original.m_agent),
		  m_agent_collision_last    (original.m_agent_collision_last),
		  m_agent_collision_count   (original.m_agent_collision_count),
		  m_agent_ai_time_cumulative(original.m_agent_ai_time_cumulative),
		  m_agent_bullet_count      (original.m_agent_bullet_count),
		  m_next_bullet             (original.m_next_bullet),
		  m_next_simple_marker      (original.m_next_simple_marker)
{
	copyArrays(original);

	assert(invariant());
}

WorldTestUnitAi :: ~WorldTestUnitAi ()
{
	delete mp_explosion_manager;
}

WorldTestUnitAi& WorldTestUnitAi :: operator= (const WorldTestUnitAi& original)
{
	if(&original != this)
	{
		delete mp_explosion_manager;

		m_orrery                   = original.m_orrery;
		mp_explosion_manager       = original.mp_explosion_manager->getClone();
		m_ring_particle_count      = original.m_ring_particle_count;
		m_target                   = original.m_target;
		m_target_disappear_time    = original.m_target_disappear_time;
		m_target_shot_count        = original.m_target_shot_count;
		m_target_collision_count   = original.m_target_collision_count;
		m_agent                    = original.m_agent;
		m_agent_collision_last     = original.m_agent_collision_last;
		m_agent_collision_count    = original.m_agent_collision_count;
		m_agent_ai_time_cumulative = original.m_agent_ai_time_cumulative;
		m_agent_bullet_count       = original.m_agent_bullet_count;
		m_next_bullet              = original.m_next_bullet;
		m_next_simple_marker       = original.m_next_simple_marker;

		copyArrays(original);
	}

	assert(invariant());
	return *this;
}



unsigned int WorldTestUnitAi :: getRingParticleCount () const
{
	return m_ring_particle_count;
}

unsigned int WorldTestUnitAi :: getTargetShotCount () const
{
	return m_target_shot_count;
}

unsigned int WorldTestUnitAi :: getTargetCollisionCount () const
{
	return m_target_collision_count;
}

unsigned int WorldTestUnitAi :: getAgentCollisionCount () const
{
	return m_agent_collision_count;
}

float WorldTestUnitAi :: getAgentAiTimeCumulative () const
{
	return m_agent_ai_time_cumulative;
}

unsigned int WorldTestUnitAi :: getAgentBulletCount () const
{
	return m_agent_bullet_count;
}

void WorldTestUnitAi :: draw (const Vector3& camera_forward,
                              const Vector3& camera_up,
                              bool draw_orrery) const
{
	assert(isModelsLoaded());
	assert(camera_forward.isNormal());
	assert(camera_up.isNormal());
	assert(camera_forward.isOrthogonal(camera_up));

	// draw moon
	glPushMatrix();
		glScaled(MOON_RADIUS, MOON_RADIUS, MOON_RADIUS);
		g_moon_list.draw();
	glPopMatrix();

	// draw orrery
	if(draw_orrery)
		m_orrery.draw();

	// draw ring particles
	for(unsigned int i = 0; i < m_ring_particle_count; i++)
	{
		assert(i <= RING_PARTICLE_COUNT_MAX);
		const RingParticleData& rp = ma_ring_particles[i];

		glPushMatrix();
			glTranslated(rp.m_position.x, rp.m_position.y, rp.m_position.z);
			glScaled(rp.m_radius, rp.m_radius, rp.m_radius);
			g_ring_particle_list.draw();
		glPopMatrix();
	}

	// draw ships
	if(m_target_disappear_time <= 0.0)
		m_target.draw();
	m_agent.draw();

	// draw bullets
	for(unsigned int i = 0; i < BULLET_COUNT_MAX; i++)
		if(ma_bullets[i].isAlive())
			ma_bullets[i].draw();

	// draw simple markers
	for(unsigned int i = 0; i < SIMPLE_MARKER_COUNT_MAX; i++)
		if(ma_simple_markers[i].isAlive())
			ma_simple_markers[i].draw();

	// draw explosions
	mp_explosion_manager->draw(camera_forward, camera_up);
}



void WorldTestUnitAi :: setRingParticleCount (unsigned int count)
{
	assert(count <= RING_PARTICLE_COUNT_MAX);

	m_ring_particle_count = count;

	assert(invariant());
}

void WorldTestUnitAi :: update ()
{
	m_orrery.addTime(TimeSystem::getFrameDuration());

	mp_explosion_manager->update();

	m_target_disappear_time -= TimeSystem::getFrameDuration();
	updateTarget();
	m_agent.update(*this);

	TimeSystem::markAiStart(0.0f);
	m_agent.runAi(*this);
	m_agent_ai_time_cumulative += TimeSystem::getAiTimeElapsed();

	for(unsigned int i = 0; i < BULLET_COUNT_MAX; i++)
		if(ma_bullets[i].isAlive())
			ma_bullets[i].update(*this);

	checkCollisionsAll();

	assert(invariant());
}

void WorldTestUnitAi :: reset ()
{
	assert(isModelsLoaded());

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->removeAll();

	m_orrery.setTime(0.0);

	initRingParticles();

	m_target_disappear_time  = 0.0;
	m_target_shot_count      = 0;
	m_target_collision_count = 0;
	updateTarget();

	m_agent.setPosition(Vector3(-(MOON_RADIUS + AGENT_START_ABOVE_MOON), 0.0, 0.0));
	m_agent.setVelocity(Vector3::getRandomUnitVector() * AGENT_SPEED_MAX);

	// REPLACE WITH YOUR OWN UNIT AI
	//m_agent.setUnitAi(new RedDuck::UnitAiStop(m_agent, *this));
	m_agent.setUnitAi(new SpaceMongols::UnitAiMoonGuard(m_agent, *this, ID_MOON));

	m_agent_collision_last     = PhysicsObjectId::ID_NOTHING;
	m_agent_collision_count    = 0;
	m_agent_ai_time_cumulative = 0.0f;
	m_agent_bullet_count       = 0;

	for(unsigned int i = 0; i < BULLET_COUNT_MAX; i++)
		ma_bullets[i].markDead(true);
	m_next_bullet = 0;

	for(unsigned int i = 0; i < SIMPLE_MARKER_COUNT_MAX; i++)
		ma_simple_markers[i].markDead();
	m_next_simple_marker = 0;

	assert(invariant());
}



Vector3 WorldTestUnitAi :: getNearerRingClump (const Vector3& position) const
{
	return Vector3(0.0,
                   0.0,
                   ((position.z > 0) ?  RING_CLUMP_DISTANCE : -RING_CLUMP_DISTANCE));
}

void WorldTestUnitAi :: initRingParticles ()
{
	static const Vector3 A_CLUMP_CENTER[2] =
	{
		Vector3(0.0, 0.0,  RING_CLUMP_DISTANCE),
		Vector3(0.0, 0.0, -RING_CLUMP_DISTANCE),
	};

	for(unsigned int i = 0; i < RING_PARTICLE_COUNT_MAX; i++)
	{
		assert(i < RING_PARTICLE_COUNT_MAX);
		ma_ring_particles[i].m_position = A_CLUMP_CENTER[i % 2] +
		                                  Vector3::getRandomSphereVector() * RING_CLUMP_RADIUS;
		double radius1 = random2(RING_PARTICLE_SIZE_MIN, RING_PARTICLE_SIZE_MAX);
		double radius2 = random2(RING_PARTICLE_SIZE_MIN, RING_PARTICLE_SIZE_MAX);
		if(radius1 < radius2)
			ma_ring_particles[i].m_radius = radius1;
		else
			ma_ring_particles[i].m_radius = radius2;
	}

	assert(invariant());
}

void WorldTestUnitAi :: initBullets ()
{
	for(unsigned int i = 0; i < BULLET_COUNT_MAX; i++)
	{
		ma_bullets[i].setId(PhysicsObjectId(PhysicsObjectId::TYPE_BULLET,
		                                    PhysicsObjectId::FLEET_ENEMY,
		                                    i));
		ma_bullets[i].setDisplayList(g_bullet_list, BULLET_SCALE);
	}
	m_next_bullet = 0;

	assert(invariant());
}

void WorldTestUnitAi :: updateTarget ()
{
	m_target.setPosition(m_orrery.getTargetPosition());
	m_target.setVelocity(m_orrery.getTargetVelocity());

	assert(invariant());
}

void WorldTestUnitAi :: checkCollisionsAll ()
{
	for(unsigned int i = 0; i < BULLET_COUNT_MAX; i++)
	{
		PhysicsObjectId collision_id = getCollisionForObject(ma_bullets[i]);
		if(collision_id != PhysicsObjectId::ID_NOTHING)
		{
			if(collision_id == ID_TARGET)
			{
				ma_simple_markers[m_next_simple_marker].init(ma_bullets[i].getPosition(), false);
				m_next_simple_marker = (m_next_simple_marker + 1) % SIMPLE_MARKER_COUNT_MAX;

				m_target_disappear_time = TARGET_HIT_DISAPPEAR_TIME;
				m_target_shot_count++;
				mp_explosion_manager->add(m_target.getPosition(), TARGET_HIT_EXPPLOSION_SIZE);
			}
			ma_bullets[i].markDead(false);
		}
	}

	PhysicsObjectId agent_collision = getCollisionForObject(m_agent);
	if(agent_collision != m_agent_collision_last &&
	   agent_collision != PhysicsObjectId::ID_NOTHING)
	{
		ma_simple_markers[m_next_simple_marker].init(m_agent.getPosition(), true);
		m_next_simple_marker = (m_next_simple_marker + 1) % SIMPLE_MARKER_COUNT_MAX;

		if(agent_collision == ID_TARGET)
		{
			m_target_disappear_time = TARGET_HIT_DISAPPEAR_TIME;
			m_target_collision_count++;
			mp_explosion_manager->add(m_target.getPosition(), TARGET_HIT_EXPPLOSION_SIZE);
		}
		else
			m_agent_collision_count++;
	}
	m_agent_collision_last = agent_collision;

	assert(invariant());
}

PhysicsObjectId WorldTestUnitAi :: getCollisionForObject (const PhysicsObject& object) const
{
	double radius = object.getRadius();

	// check for collision with moon
	if(object.getPosition().isNormLessThan(MOON_RADIUS + radius))
		return ID_MOON;

	// check for collision with target
	if(object.getId() != ID_TARGET &&
	   m_target_disappear_time <= 0.0 &&
	   object.getPosition().isDistanceLessThan(m_target.getPosition(),
	                                           m_target.getRadius() + radius))
	{
		return ID_TARGET;
	}

	// don't check against agent
	// don't check against bullets

	// check for collision with ring particles
	for(unsigned int i = 0; i < m_ring_particle_count; i++)
	{
		assert(i < RING_PARTICLE_COUNT_MAX);
		const RingParticleData& rp = ma_ring_particles[i];
		if(object.getPosition().isDistanceLessThan(rp.m_position, rp.m_radius + radius))
			return PhysicsObjectId(TYPE_RING_PARTICLE, PhysicsObjectId::FLEET_NATURE, i);
	}

	// no collisions
	return PhysicsObjectId::ID_NOTHING;
}

void WorldTestUnitAi :: copyArrays (const WorldTestUnitAi& original)
{
	for(unsigned int i = 0; i < RING_PARTICLE_COUNT_MAX; i++)
		ma_ring_particles[i] = original.ma_ring_particles[i];
	for(unsigned int i = 0; i < BULLET_COUNT_MAX; i++)
		ma_bullets[i] = original.ma_bullets[i];
	for(unsigned int i = 0; i < SIMPLE_MARKER_COUNT_MAX; i++)
		ma_simple_markers[i] = original.ma_simple_markers[i];

	assert(invariant());
}

bool WorldTestUnitAi :: invariant () const
{
	if(mp_explosion_manager == NULL) return false;
	if(m_ring_particle_count > RING_PARTICLE_COUNT_MAX) return false;
	if(m_next_bullet > BULLET_COUNT_MAX) return false;
	if(m_next_simple_marker > SIMPLE_MARKER_COUNT_MAX) return false;
	return true;
}


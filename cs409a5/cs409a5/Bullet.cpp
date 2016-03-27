//
//  Bullet.cpp
//

#include <cassert>
#include "GetGlut.h"

#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"

#include "TimeSystem.h"
#include "WorldInterface.h"
#include "PhysicsObject.h"
#include "Bullet.h"

namespace
{
	const double LONG_AGO       = -1.0e20;  // creation time for bullets created with default constructor
	const double EXPLOSION_SIZE = 10.0;     // size of death explosion
}



const double Bullet :: RADIUS   =    0.0;
const double Bullet :: LIFESPAN =    3.0;
const double Bullet :: SPEED    = 1500.0;



Bullet :: Bullet ()
		: PhysicsObject(RADIUS),
		  m_source_id(PhysicsObjectId::ID_NOTHING),
		  m_creation_time(LONG_AGO),
		  m_is_dead(true)
{
}

Bullet :: Bullet (const PhysicsObjectId& id,
                  const Vector3& position,
                  const Vector3& forward,
                  const DisplayList& display_list,
                  double display_scale,
                  const PhysicsObjectId& source_id)
		: PhysicsObject(id,
		                position,
		                RADIUS,
		                forward * SPEED,
		                display_list,
		                display_scale),
		  m_source_id(source_id),
		  m_creation_time(TimeSystem::getFrameStartTime()),
		  m_is_dead(false)
{
	assert(id != PhysicsObjectId::ID_NOTHING);
	assert(forward.isNormal());
	assert(display_list.isReady());
	assert(display_scale >= 0.0);
}

Bullet :: Bullet (const Bullet& original)
		: PhysicsObject(original),
		  m_source_id(original.m_source_id),
		  m_creation_time(original.m_creation_time),
		  m_is_dead(original.m_is_dead)
{
}

Bullet :: ~Bullet ()
{
	// destructor for superclass will be invoked automatically
}

Bullet& Bullet :: operator= (const Bullet& original)
{
	if(&original != this)
	{
		PhysicsObject::operator=(original);

		m_source_id     = original.m_source_id;
		m_creation_time = original.m_creation_time;
		m_is_dead        = original.m_is_dead;
	}
	return *this;
}



const PhysicsObjectId& Bullet :: getSourceId () const
{
	return m_source_id;
}



void Bullet :: fire (const Vector3& position,
                     const Vector3& forward,
                     const PhysicsObjectId& source_id)
{
	assert(forward.isNormal());

	setPosition(position);
	setVelocity(forward * SPEED);
	randomizeUpVector();

	m_source_id     = source_id;
	m_creation_time = TimeSystem::getFrameStartTime();
	m_is_dead       = false;
}

void Bullet :: setSourceId (const PhysicsObjectId& source_id)
{
	m_source_id = source_id;
}



///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from PhysicsObject
//

PhysicsObject* Bullet :: getClone () const
{
	return new Bullet(*this);
}

bool Bullet :: isAlive () const
{
	return !m_is_dead;
}

bool Bullet :: isDying () const
{
	assert(isAlive());

	if(m_creation_time + LIFESPAN < TimeSystem::getFrameStartTime())
		return true;
	else
		return false;
}

void Bullet :: markDead (bool instant)
{
	m_creation_time = LONG_AGO;

	if(instant)
		m_is_dead = true;
}

void Bullet :: updateForPause ()
{
	if(m_creation_time != LONG_AGO)
		m_creation_time += TimeSystem::getPauseDuration();
}

void Bullet :: update (WorldInterface& r_world)
{
	// explode if out of lifespan (or markDead(false) was called)
	if(isAlive() && isDying())
	{
		r_world.addExplosion(getPositionPrevious(), EXPLOSION_SIZE, 0);
		m_is_dead = true;

		// no further updates
		return;
	}

	updateBasic();  // moves the Bullet
}


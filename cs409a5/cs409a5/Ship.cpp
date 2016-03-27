//
//  Ship.cpp
//

#include <cassert>
#include "GetGlut.h"

#include "Pi.h"
#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"

#include "TimeSystem.h"
#include "WorldInterface.h"
#include "PhysicsObject.h"
#include "Ship.h"

#include "UnitAiSuperclass.h"

namespace
{
	const float HEALTH_DEFAULT       = 0.0f;
	const int   AMMO_DEFAULT         = 0;
	const bool  IS_DEAD_DEFAULT      = true;

	const double CAMERA_FORWARD_DISTANCE = -100.0;
	const double CAMERA_UP_DISTANCE      =   20.0;

	const double EXPLOSION_SIZE_FACTOR = 5.0;     // size of death explosion
}



const double Ship :: RADIUS         = 10.0;
const float  Ship :: HEALTH_DEAD_AT =  0.001f;
const float  Ship :: RELOAD_TIME    =  0.25f;



Ship :: Ship ()
		: PhysicsObject(RADIUS),
		  m_health(HEALTH_DEFAULT),
		  m_reload_timer(RELOAD_TIME),
		  m_ammo(AMMO_DEFAULT),
		  m_is_dead(IS_DEAD_DEFAULT)
{
	assert(invariant());
    this->unitAi = NULL;
}

Ship :: Ship (const PhysicsObjectId& id,
              const Vector3& position,
              const Vector3& velocity,
              const DisplayList& display_list,
              double display_scale,
              float health,
              int ammo,
              float max_speed,
              float max_acceleration,
              float max_rotation_rate)
		: PhysicsObject(id,
		                position,
		                RADIUS,
		                velocity,
		                display_list,
		                display_scale),
		  m_health(health),
		  m_reload_timer(RELOAD_TIME),
		  m_ammo(ammo),
		  m_is_dead(health <= HEALTH_DEAD_AT)
{
	assert(id != PhysicsObjectId::ID_NOTHING);
	assert(display_list.isReady());
	assert(display_scale >= 0.0);
	assert(ammo >= 0);
    
    this->max_speed = max_speed;
    this->max_acceleration = max_acceleration;
    this->max_rotation_rate = max_rotation_rate;
    this->unitAi = NULL;

	assert(invariant());
}

Ship :: Ship (const Ship& original)
		: PhysicsObject(original),
		  m_health(original.m_health),
		  m_reload_timer(original.m_reload_timer),
		  m_ammo(original.m_ammo),
		  m_is_dead(original.m_is_dead)
{
	assert(invariant());
    this->unitAi = NULL;
}

Ship :: ~Ship ()
{
	// destructor for superclass will be invoked automatically
}

Ship& Ship :: operator= (const Ship& original)
{
	if(&original != this)
	{
		PhysicsObject::operator=(original);

		m_health       = original.m_health;
		m_reload_timer = original.m_reload_timer;
		m_ammo         = original.m_ammo;
		m_is_dead      = original.m_is_dead;
        
        if (isUnitAiSet()) delete unitAi;
        unitAi         = original.unitAi;
	}

	assert(invariant());
	return *this;
}



float Ship :: getHealth () const
{
	return m_health;
}

bool Ship :: isReloaded () const
{
	if(m_reload_timer >= RELOAD_TIME)
		return true;
	else
		return false;
}

int Ship :: getAmmo () const
{
	return m_ammo;
}

void Ship :: setupCamera () const
{
	CoordinateSystem camera = getCameraCoordinateSystem();
	const Vector3& camera_position = camera.getPosition();
	const Vector3& camera_up       = camera.getUp();

	Vector3 look_at = camera_position + camera.getForward();

	gluLookAt(camera_position.x, camera_position.y, camera_position.z,
	          look_at.x,         look_at.y,         look_at.z,
	          camera_up.x,       camera_up.y,       camera_up.z);
}

CoordinateSystem Ship :: getCameraCoordinateSystem () const
{
	const Vector3& forward = getForward();
	const Vector3& up      = getUp();

	Vector3 camera_at = getPosition() +
	                    forward * CAMERA_FORWARD_DISTANCE +
	                    up      * CAMERA_UP_DISTANCE;

	return CoordinateSystem(camera_at, forward, up);
}



void Ship :: setHealth (float health)
{
	m_health = health;
	if(m_health > HEALTH_DEAD_AT)
		m_is_dead = false;

	assert(invariant());
}

void Ship :: addHealth (float increase)
{
	m_health += increase;
	if(m_health > HEALTH_DEAD_AT)
		m_is_dead = false;

	assert(invariant());
}

void Ship :: markReloading ()
{
	m_reload_timer = 0.0;

	assert(invariant());
}

void Ship :: markNotReloading ()
{
	m_reload_timer = RELOAD_TIME;

	assert(invariant());
}

void Ship :: setAmmo (int ammo)
{
	assert(ammo >= 0);

	m_ammo = ammo;

	assert(invariant());
}

void Ship :: addAmmo (int increase)
{
	m_ammo += increase;
	if(m_ammo < 0)
		m_ammo = 0;

	assert(invariant());
}

void Ship :: updateForPause ()
{
	; // nothing yet

	assert(invariant());
}



///////////////////////////////////////////////////////////////
//
//   functions inherited from PhysicsObject
//

PhysicsObject* Ship :: getClone () const
{
	return new Ship(*this);
}

bool Ship :: isAlive () const
{
	return !m_is_dead;
}

bool Ship :: isDying () const
{
	assert(isAlive());

	if(m_health <= HEALTH_DEAD_AT)
		return true;
	else
		return false;
}



void Ship :: markDead (bool instant)
{
	m_health = 0.0;

	if(instant)
		m_is_dead = true;
}

void Ship :: update (WorldInterface& r_world)
{
    if (isUnitAiSet())
    {
        if (!desired_velocity.isZero())
        {
            double theta = max_rotation_rate * TimeSystem::getFrameDuration();
            if(theta > 0.0)
            {
                rotateTowards(desired_velocity.getNormalized(), theta);
            }
            
            rotateTowards(desired_velocity, max_rotation_rate);
            double delta = max_acceleration * TimeSystem::getFrameDuration();
            if(delta > 0.0)
            {
                double currentSpeed = getSpeed();
                double desiredSpeed = desired_velocity.getNorm();
                
                if (currentSpeed < desiredSpeed)
                {
                    currentSpeed += delta;
                    if (currentSpeed > desiredSpeed) currentSpeed = desiredSpeed;
                }
                else if (currentSpeed > desiredSpeed)
                {
                    currentSpeed -= delta;
                    if (currentSpeed < desiredSpeed) currentSpeed = desiredSpeed;
                }
                setSpeed(currentSpeed);
            }
        }
        
        if (wantsToFire && isReloaded())
        {
            r_world.addBullet(getPosition(), getForward(), getId());
            wantsToFire = false;
            markReloading();
        }
    }
    
	if(isAlive() && isDying())
	{
		r_world.addExplosion(getPositionPrevious(), getRadius() * EXPLOSION_SIZE_FACTOR, 0);
		m_is_dead = true;

		// no further updates
		assert(invariant());
		return;
	}

	if(isAlive())
	{
		updateBasic();  // moves the Ship

		m_reload_timer += TimeSystem::getFrameDuration();
	}

	assert(invariant());
}

double Ship::getSpeedMax () const
{
    return max_speed;
}

double Ship::getAcceleration () const
{
    return max_acceleration;
}

double Ship::getRotationRate () const
{
    return max_rotation_rate;
}

void Ship::setManeuverability (double speed_max,
                                 double acceleration,
                                 double rotation_rate)
{
    this->max_speed = speed_max;
    this->max_acceleration = acceleration;
    this->max_rotation_rate = rotation_rate;
}

bool Ship::isUnitAiSet () const
{
    return (unitAi != NULL);
}

UnitAiSuperclass& Ship::getUnitAi () const
{
    return *unitAi;
}

void Ship::drawUnitAi () const
{
    
}

void Ship::setDesiredVelocity (const Vector3& desired_velocity)
{
    this->desired_velocity = desired_velocity;
}

void Ship::markFireBulletDesired ()
{
    wantsToFire = true;
}

void Ship::markFireMissileDesired (const PhysicsObjectId& id_target)
{
    
}

void Ship::runAi (const WorldInterface& world)
{
    if (isUnitAiSet()) unitAi->run(world);
}

void Ship::setUnitAi (UnitAiSuperclass* p_unit_ai)
{
    if (isUnitAiSet()) delete unitAi;
    unitAi = p_unit_ai;
}

void Ship::setUnitAiNone ()
{
    assert(isUnitAiSet());
    
    delete unitAi;
    unitAi = NULL;
}

///////////////////////////////////////////////////////////////
//
//  Helper function not inherited from anywhere
//

bool Ship :: invariant () const
{
	if(m_ammo < 0) return false;
	return true;
}

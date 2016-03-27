//
//  PhysicsObject.cpp
//

#include <cassert>
#include <cstdlib>  // for rand()
#include <iostream>
#include "GetGlut.h"

#include "Pi.h"
#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"

#include "TimeSystem.h"
#include "CoordinateSystem.h"
#include "PhysicsObject.h"

using namespace std;
namespace
{
	const double RADIUS_DEFAULT        = 0.0;
	const double SPEED_DEFAULT         = 0.0;
	const double DISPLAY_SCALE_DEFAULT = 1.0;
}



//
//  Constructors, etc.
//

PhysicsObject :: PhysicsObject ()
		: m_id(PhysicsObjectId::ID_DEFAULT),
		  m_coordinates(),
		  m_position_previous(m_coordinates.getPosition()),
		  m_radius(RADIUS_DEFAULT),
		  m_speed(SPEED_DEFAULT),
		  mp_display_list(NULL),
		  m_display_scale(DISPLAY_SCALE_DEFAULT)
{
	assert(invariant());
}

PhysicsObject :: PhysicsObject (double radius)
		: m_id(PhysicsObjectId::ID_DEFAULT),
		  m_coordinates(),
		  m_position_previous(m_coordinates.getPosition()),
		  m_radius(radius),
		  m_speed(SPEED_DEFAULT),
		  mp_display_list(NULL),
		  m_display_scale(DISPLAY_SCALE_DEFAULT)
{
	assert(radius >= 0.0);

	assert(invariant());
}

PhysicsObject :: PhysicsObject (const PhysicsObjectId& id,
                                const Vector3& position,
                                double radius,
                                const Vector3& velocity,
                                const DisplayList& display_list,
                                double display_scale)
		: m_id(id),
		  m_coordinates(position),
		  m_position_previous(position),
		  m_radius(radius),
		  m_speed(velocity.getNorm()),
		  mp_display_list(&display_list),
		  m_display_scale(display_scale)
{
	assert(id != PhysicsObjectId::ID_NOTHING);
	assert(radius >= 0.0);
	assert(display_list.isReady());
	assert(display_scale >= 0.0);

	if(m_speed == 0)
		m_coordinates.setOrientation(Vector3(1.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));
	else
		m_coordinates.setOrientation(velocity / m_speed);

	assert(invariant());
}

PhysicsObject :: PhysicsObject (const PhysicsObject& original)
		: m_id(original.m_id),
		  m_coordinates(original.m_coordinates),
		  m_position_previous(original.m_position_previous),
		  m_radius(original.m_radius),
		  m_speed(original.m_speed),
		  mp_display_list(original.mp_display_list),
		  m_display_scale(original.m_display_scale)
{
	assert(invariant());
}

PhysicsObject :: ~PhysicsObject ()
{
}

PhysicsObject& PhysicsObject :: operator= (const PhysicsObject& original)
{
	if(&original != this)
	{
		m_id                = original.m_id;
		m_coordinates       = original.m_coordinates;
		m_position_previous = original.m_position_previous;
		m_radius            = original.m_radius;
		m_speed             = original.m_speed;
		mp_display_list     = original.mp_display_list;
		m_display_scale     = original.m_display_scale;
	}

	assert(invariant());
	return *this;
}



//
//  Query functions (getters)
//

const PhysicsObjectId& PhysicsObject :: getId () const
{
	return m_id;
}

const Vector3& PhysicsObject :: getPosition () const
{
	return m_coordinates.getPosition();
}

Vector3 PhysicsObject :: getPositionMin () const
{
	return m_coordinates.getPosition() - Vector3(m_radius, m_radius, m_radius);
}

Vector3 PhysicsObject :: getPositionMax () const
{
	return m_coordinates.getPosition() + Vector3(m_radius, m_radius, m_radius);
}

const Vector3& PhysicsObject :: getPositionPrevious () const
{
	return m_position_previous;
}

Vector3 PhysicsObject :: getPositionPreviousMin () const
{
	return m_position_previous - Vector3(m_radius, m_radius, m_radius);
}

Vector3 PhysicsObject :: getPositionPreviousMax () const
{
	return m_position_previous + Vector3(m_radius, m_radius, m_radius);
}

double PhysicsObject :: getRadius () const
{
	return m_radius;
}

const Vector3& PhysicsObject :: getForward () const
{
	return m_coordinates.getForward();
}

const Vector3& PhysicsObject :: getUp () const
{
	return m_coordinates.getUp();
}

Vector3 PhysicsObject :: getRight () const
{
	return m_coordinates.getRight();
}

double PhysicsObject :: getSpeed () const
{
	return m_speed;
}

Vector3 PhysicsObject :: getVelocity () const
{
	return m_coordinates.getForward() * m_speed;
}

bool PhysicsObject :: isDisplayListSet () const
{
	if(mp_display_list != NULL)
		return true;
	else
		return false;
}

void PhysicsObject :: draw () const
{
	if(isDisplayListSet())
	{
		const Vector3& position = getPosition();
		double scaling = m_display_scale;

		glPushMatrix();
			glTranslated(position.x, position.y, position.z);
			m_coordinates.setupOrientationMatrix();
			glScaled(scaling, scaling, scaling);

			mp_display_list->draw();
		glPopMatrix();
	}
}



//
//  Modifier functions (setters)
//

void PhysicsObject :: initPhysics (const PhysicsObjectId& id,
                                   const Vector3& position,
                                   double radius,
                                   const Vector3& velocity,
                                   const DisplayList& display_list,
                                   double display_scale)
{
	assert(id != PhysicsObjectId::ID_NOTHING);
	assert(radius >= 0.0);
	assert(display_list.isReady());
	assert(display_scale >= 0.0);

	double speed = velocity.getNorm();
	if(speed == 0)
		m_coordinates.init(position, Vector3(1.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));
	else
		m_coordinates.init(position, velocity / speed);

	m_id                = id;
	m_position_previous = position;
	m_radius            = radius;
	m_speed             = speed;
	mp_display_list     = &display_list;
	m_display_scale     = display_scale;

	assert(invariant());
}

void PhysicsObject :: setId (const PhysicsObjectId& id)
{
	assert(id != PhysicsObjectId::ID_NOTHING);

	m_id = id;

	assert(invariant());
}

void PhysicsObject :: setPosition (const Vector3& position)
{
	m_coordinates.setPosition(position);

	assert(invariant());
}

void PhysicsObject :: addPosition (const Vector3& increase)
{
	m_coordinates.addPosition(increase);

	assert(invariant());
}

void PhysicsObject :: setPositionPreviousToCurrent ()
{
	m_position_previous = m_coordinates.getPosition();

	assert(invariant());
}

void PhysicsObject :: setRadius (double radius)
{
	assert(radius >= 0.0);

	m_radius = radius;

	assert(invariant());
}

void PhysicsObject :: setOrientation (const Vector3& forward)
{
	assert(forward.isNormal());

	m_coordinates.setOrientation(forward);

	assert(invariant());
}

void PhysicsObject :: setOrientation (const Vector3& forward,
                                      const Vector3& up)
{
	assert(forward.isNormal());
	assert(up.isNormal());
	assert(forward.isOrthogonal(up));

	m_coordinates.setOrientation(forward, up);

	assert(invariant());
}

void PhysicsObject :: setUpVector (const Vector3& up)
{
	assert(up.isNormal());
	assert(getForward().isOrthogonal(up));

	m_coordinates.setUp(up);

	assert(invariant());
}

void PhysicsObject :: rotateAroundForward (double radians)
{
	m_coordinates.rotateAroundForward(radians);

	assert(invariant());
}

void PhysicsObject :: rotateAroundUp (double radians)
{
	m_coordinates.rotateAroundUp(radians);

	assert(invariant());
}

void PhysicsObject :: rotateAroundRight (double radians)
{
	m_coordinates.rotateAroundRight(radians);

	assert(invariant());
}

void PhysicsObject :: rotateTowards (const Vector3& forward,
                                     double max_radians)
{
	assert(forward.isNormal());
	assert(max_radians >= 0.0);

	m_coordinates.rotateToVector(forward, max_radians);

	assert(invariant());
}

void PhysicsObject :: setSpeed (double speed)
{
	assert(speed >= 0.0);

	m_speed = speed;

	assert(invariant());
}

void PhysicsObject :: setVelocity (const Vector3& velocity)
{
	m_speed = velocity.getNorm();
	if(!velocity.isZero())
		m_coordinates.rotateToVector(velocity, PI);

	assert(invariant());
}

void PhysicsObject :: addVelocity (const Vector3& increase)
{
	setVelocity(getVelocity() + increase);

	assert(invariant());
}

void PhysicsObject :: setDisplayList (const DisplayList& display_list,
                                      double display_scale)
{
	assert(display_list.isReady());
	assert(display_scale >= 0.0);

	mp_display_list = &display_list;
	m_display_scale = display_scale;

	assert(invariant());
}

void PhysicsObject :: setDisplayListNone ()
{
	mp_display_list = NULL;

	assert(invariant());
}

void PhysicsObject :: randomizeUpVector ()
{
	double random0to1 = rand() / (RAND_MAX + 1.0);
	m_coordinates.rotateAroundForward(random0to1 * TWO_PI);

	assert(invariant());
}

void PhysicsObject :: randomizeOrientation ()
{
	// randomize forward direction
	m_coordinates.setOrientation(Vector3::getRandomUnitVector());

	randomizeUpVector();

	assert(invariant());
}



//
//  Protected helper functions
//
//  To be used by subclasses
//

void PhysicsObject :: updateBasic ()
{
	m_position_previous = m_coordinates.getPosition();

	double distance = m_speed * TimeSystem::getFrameDuration();
	m_coordinates.moveForward(distance);

	assert(invariant());
}



//
//  Private functions
//

bool PhysicsObject :: invariant () const
{
	if(m_id == PhysicsObjectId::ID_NOTHING) return false;
	if(m_radius < 0.0) return false;
	if(m_speed  < 0.0) return false;
	if(mp_display_list != NULL && !mp_display_list->isReady()) return false;
	if(m_display_scale < 0.0) return false;
	return true;
}


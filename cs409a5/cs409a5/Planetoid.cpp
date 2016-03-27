//
//  Planetoid.cpp
//

#include <cassert>

#include "GetGlut.h"
#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"

#include "WorldInterface.h"
#include "PhysicsObject.h"
#include "Planetoid.h"

#define FORWARD_DEAFULT Vector3(0.0, 1.0, 0.0)
#define UP_DEFAULT      Vector3(0.0, 0.0, 1.0)



const double Planetoid :: RADIUS_DEFAULT = 1.0;



Planetoid :: Planetoid ()
		: PhysicsObject(RADIUS_DEFAULT),
		  m_owner(PhysicsObjectId::FLEET_NATURE),
		  m_is_actively_claimed(false)
{
	setOrientation(FORWARD_DEAFULT, UP_DEFAULT);

	assert(invariant());
}

Planetoid :: Planetoid (const PhysicsObjectId& id,
                        const Vector3& position,
                        double radius,
                        const DisplayList& display_list,
                        double display_scale)
		: PhysicsObject(id,
		                position,
		                radius,
		                Vector3::ZERO,
		                display_list,
		                display_scale),
		  m_owner(PhysicsObjectId::FLEET_NATURE),
		  m_is_actively_claimed(false)
{
	assert(id != PhysicsObjectId::ID_NOTHING);
	assert(radius >= 0.0);
	assert(display_list.isReady());
	assert(display_scale >= 0.0);

	setOrientation(FORWARD_DEAFULT, UP_DEFAULT);

	assert(invariant());
}

Planetoid :: Planetoid (const Planetoid& original)
		: PhysicsObject(original),
		  m_owner(original.m_owner),
		  m_is_actively_claimed(original.m_is_actively_claimed)
{
	assert(invariant());
}

Planetoid :: ~Planetoid ()
{
	// destructor for superclass is invoked automatically
}

Planetoid& Planetoid :: operator= (const Planetoid& original)
{
	if(&original != this)
	{
		PhysicsObject::operator=(original);

		m_owner               = original.m_owner;
		m_is_actively_claimed = original.m_is_actively_claimed;
	}

	assert(invariant());
	return *this;
}



unsigned int Planetoid :: getOwner () const
{
	return m_owner;
}

bool Planetoid :: isActivelyClaimed () const
{
	return m_is_actively_claimed;
}

void Planetoid :: drawOwner () const
{
	static const double RADIUS_PADDING = 250.0;
	static const unsigned int ENEMY_COLOUR_COUNT = 10;
	static const Vector3 A_ENEMY_COLOURS[ENEMY_COLOUR_COUNT] =
	{
		Vector3(1.0,  0.0,  0.0 ),  // red
		Vector3(1.0,  0.5,  0.0 ),  // orange
		Vector3(1.0,  0.0,  0.5 ),  // hot pink
		Vector3(1.0,  0.0,  1.0 ),  // purple
		Vector3(0.5,  0.0,  0.0 ),  // dark red
		Vector3(1.0,  0.5,  0.5 ),  // pale red
		Vector3(0.5,  0.25, 0.0 ),  // brown
		Vector3(0.5,  0.0,  0.5 ),  // maroon
		Vector3(1.0,  0.75, 0.25),  // pale orange
		Vector3(1.0,  0.5,  0.75),  // pale pink
	};

	if(m_owner == PhysicsObjectId::FLEET_NATURE)
		return;

	glPushMatrix();
		Vector3 position = getPosition();
		double  radius   = getRadius() + RADIUS_PADDING;

		if(m_owner == PhysicsObjectId::FLEET_PLAYER)
			glColor3d(0.0, 1.0, 0.0);
		else
		{
			assert(m_owner >= PhysicsObjectId::FLEET_ENEMY);
			unsigned int colour_index = m_owner - PhysicsObjectId::FLEET_ENEMY;
			colour_index %= ENEMY_COLOUR_COUNT;
			glColor3dv(A_ENEMY_COLOURS[colour_index].getAsArray());
		}

		glTranslated(position.x, position.y, position.z);
		glutWireSphere(radius, 16, 12);
	glPopMatrix();
}


	
void Planetoid :: initPlanetoid (const PhysicsObjectId& id,
                                 const Vector3& position,
                                 double radius,
                                 const DisplayList& display_list,
                                 double display_scale)
{
	assert(id != PhysicsObjectId::ID_NOTHING);
	assert(radius >= 0.0);
	assert(display_list.isReady());
	assert(display_scale >= 0.0);

	initPhysics(id,
	            position,
	            radius,
	            Vector3::ZERO,
	            display_list,
	            display_scale);
	setOrientation(FORWARD_DEAFULT, UP_DEFAULT);

	m_owner               = PhysicsObjectId::FLEET_NATURE;
	m_is_actively_claimed = false;

	assert(invariant());
}

void Planetoid :: setOwner (unsigned int owner)
{
	assert(owner <= PhysicsObjectId::FLEET_MAX);

	m_owner = owner;
	if(owner != PhysicsObjectId::FLEET_NATURE)
		m_is_actively_claimed = true;
	else
		m_is_actively_claimed = false;

	assert(invariant());
}

void Planetoid :: markNotActivelyClaimed ()
{
	m_is_actively_claimed = false;

	assert(invariant());
}



///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from PhysicsObject
//

PhysicsObject* Planetoid :: getClone () const
{
	return new Planetoid(*this);
}

bool Planetoid :: isAlive () const
{
	return true;
}

bool Planetoid :: isDying () const
{
	assert(isAlive());

	return false;
}

void Planetoid :: markDead (bool instant)
{
	;  // do nothing
}

void Planetoid :: update (WorldInterface& r_world)
{
	;  // do nothing
}



///////////////////////////////////////////////////////////////
//
//  Helper function not inherited from anywhere
//

bool Planetoid :: invariant () const
{
	if(m_owner >  PhysicsObjectId::FLEET_MAX) return false;
	if(m_owner == PhysicsObjectId::FLEET_NATURE && m_is_actively_claimed) return false;
	return true;
}

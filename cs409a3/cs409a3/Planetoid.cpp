//
//  Planetoid.cpp
//

#include <cassert>

#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"

#include "WorldInterface.h"
#include "PhysicsObject.h"
#include "Planetoid.h"

namespace
{
	const Vector3 FORWARD_DEAFULT(0.0, 1.0, 0.0);
	const Vector3 UP_DEFAULT     (0.0, 0.0, 1.0);
}



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
	            display_scale * radius);
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

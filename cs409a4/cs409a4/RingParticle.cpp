//
//  RingParticle.cpp
//

#include <cassert>
#include <cmath>
#include "GetGlut.h"

#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/ObjModel.h"
#include "../../ObjLibrary/MtlLibraryManager.h"
#include "../../ObjLibrary/Material.h"
#include "../../ObjLibrary/DisplayList.h"

#include "RingParticle.h"

namespace
{
	//
	//  sqr
	//
	//  Purpose: To efficently calculate the square of a number.
	//  Parameter(s):
	//    <1> n: The number to square
	//  Precondition(s): N/A
	//  Returns: n * n
	//  Side Effect: N/A
	//  T must define:
	//    <1> T operator* (const T& a, const T& b);
	//

	template <typename T>
	T sqr (const T& n)
	{
		return n * n;
	}

	//
	//  calculateNextPseudorandom
	//
	//  Purpose: To calculate the next pseudorandom value from
	//           the specified seed value.  The seed value is
	//           also updated.
	//  Parameter(s):
	//    <1> r_a: A reference to the seed value
	//  Precondition(s): N/A
	//  Returns: The next pseudorandom value.
	//  Side Effect: r_a is updated to the generated value.
	//
	//  http://www.jstatsoft.org/v08/i14/paper
	//

	static unsigned int calculateNextPseudorandom (
	                                          unsigned int& r_a)
	{
		r_a ^= r_a << 13;
		r_a ^= r_a >> 17;
		r_a ^= r_a << 5;
		return r_a;
	}



	bool g_is_loaded = false;

	DisplayList ga_ring_particle0_list;

	Material ga_ring_particle_material[RingParticle::MATERIAL_COUNT];

}  // end of anonymous namespace



const double RingParticle :: RADIUS_FACTOR = 10.0;
const double RingParticle :: RADIUS_MIN    = calcuateRadius(0.0);
const double RingParticle :: RADIUS_MAX    = calcuateRadius(1.0);



bool RingParticle :: isLoaded ()
{
	return g_is_loaded;
}

void RingParticle :: load ()
{
	assert(!isLoaded());

	static const char* aa_ring_particle_material[RingParticle::MATERIAL_COUNT] = 
	{	"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA",
		"ring_particleA"	};

	// we don't store the material here: we will set it each time instead
	ga_ring_particle0_list = ObjModel("Models/RingParticleA0.obj").getDisplayListMaterialNone();

	for(unsigned int i = 0; i < RingParticle::MATERIAL_COUNT; i++)
	{
		const Material* p_material = MtlLibraryManager::getMaterial("Models/RingParticles.mtl",
			                                                        aa_ring_particle_material[i]);
		ga_ring_particle_material[i] = *p_material;  // create a copy
	}

	g_is_loaded = true;

	assert(isLoaded());
}

double RingParticle :: calcuateRadius (double n)
{
	assert(n >= 0.0);
	assert(n <= 1.0);

	return (n * n * 9.0 + 1.0) * RADIUS_FACTOR;
}



RingParticle :: RingParticle ()
{
	init(Vector3::ZERO, 0);

	assert(invariant());
}

RingParticle :: RingParticle (const Vector3& position, unsigned int seed)
{
	init(position, seed);

	assert(invariant());
}

RingParticle :: RingParticle (const RingParticle& original)
		: m_position(original.m_position),
		  m_radius(original.m_radius),
		  m_material(original.m_material),
		  m_fixed_rotation_axis(original.m_fixed_rotation_axis),
		  m_fixed_rotation_degrees(original.m_fixed_rotation_degrees)
{
	assert(invariant());
}

RingParticle :: ~RingParticle ()
{
}

RingParticle& RingParticle :: operator= (const RingParticle& original)
{
	if(&original != this)
	{
		m_position = original.m_position;
		m_radius   = original.m_radius;
		m_material = original.m_material;
		m_fixed_rotation_axis    = original.m_fixed_rotation_axis;
		m_fixed_rotation_degrees = original.m_fixed_rotation_degrees;
	}

	assert(invariant());
	return *this;
}



const Vector3& RingParticle :: getPosition () const
{
	return m_position;
}

double RingParticle :: getRadius () const
{
	return m_radius;
}

void RingParticle :: draw (const Vector3& camera_position) const
{
	glPushMatrix();
		// set up coordinate system
		glTranslated(m_position.x, m_position.y, m_position.z);
		glRotated(m_fixed_rotation_degrees,
		          m_fixed_rotation_axis.x,
		          m_fixed_rotation_axis.y,
		          m_fixed_rotation_axis.z);
		glScaled(m_radius, m_radius, m_radius);

		// use the appropriate material
		assert(m_material < MATERIAL_COUNT);
		ga_ring_particle_material[m_material].activate();

		// draw the model using the material we chose
		ga_ring_particle0_list.draw();

		// deactivate the material
		Material::deactivate();
	glPopMatrix();
}



void RingParticle :: init (const Vector3& position, unsigned int seed)
{
	static const double SCALE_TO_01 = 1.0 / (~0u + 1.0);

	m_position = position;

	double random1 = calculateNextPseudorandom(seed) * SCALE_TO_01;
	double random2 = calculateNextPseudorandom(seed) * SCALE_TO_01;
	double random3 = calculateNextPseudorandom(seed) * SCALE_TO_01;
	double random4 = calculateNextPseudorandom(seed) * SCALE_TO_01;
	double random5 = calculateNextPseudorandom(seed) * SCALE_TO_01;

	m_radius   = calcuateRadius(random1);
	m_material = (unsigned int)(random2 * MATERIAL_COUNT);
	m_fixed_rotation_axis    = Vector3::getPseudorandomUnitVector(random3, random4);
	m_fixed_rotation_degrees = random5 * 360.0;

	assert(invariant());
}



bool RingParticle :: invariant () const
{
	if(m_radius <  0.0) return false;
	if(m_material >= MATERIAL_COUNT) return false;
	if(!m_fixed_rotation_axis.isNormal()) return false;
	if(m_fixed_rotation_degrees <    0.0) return false;
	if(m_fixed_rotation_degrees >= 360.0) return false;
	return true;
}

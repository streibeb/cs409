//
//  RingParticle.h
//

#ifndef RING_PARTICLE_H
#define RING_PARTICLE_H

#include "../../ObjLibrary/Vector3.h"



//
//  RingParticle
//
//  A class to represent a particle in the ring.  A RingParticle
//    is created from a position and a seed value.  It can be
//    reinitialized with a different position and seed, but
//    cannot be otherwise modified.  The position and radius of
//    a RingParticle can bbe queried, and a RingParticle can be
//    drawn to the screen.
//
//  Class Invariant:
//    <1> m_radius >= 0.0
//    <2> m_material < MATERIAL_COUNT
//    <3> m_fixed_rotation_axis.isNormal()
//    <4> m_fixed_rotation_degrees >=   0.0
//    <5> m_fixed_rotation_degrees <  360.0
//

class RingParticle
{
public:
//
//  RADIUS_FACTOR
//
//  A scaling factor for the radius for a RingParticle.  A
//    scaling larger factor results in larger particles.
//

	static const double RADIUS_FACTOR;

//
//  RADIUS_MIN
//
//  The maximum radius for a RingParticle.
//

	static const double RADIUS_MIN;

//
//  RADIUS_MAX
//
//  The maximum radius for a RingParticle.
//

	static const double RADIUS_MAX;

//
//  MATERIAL_COUNT
//
//  The number of materials used to display RingParticles.
//

	static const unsigned int MATERIAL_COUNT = 20;

public:
//
//  Class Function: isLoaded
//
//  Purpose: To determine if the models used to display
//           RingParticles have been loaded.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the models have been loaded.
//  Side Effect: N/A
//

	static bool isLoaded ();

//
//  Class Function: load
//
//  Purpose: To load the models used to display RingParticles.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> !isLoaded()
//  Returns: N/A
//  Side Effect: The models are loaded.
//

	static void load ();

private:
//
//  Class Function: calcuateRadius
//
//  Purpose: To calculate the radius for a ring particle based
//           on the specified random value.
//  Parameter(s):
//    <1> n: The random value
//  Precondition(s):
//    <1> n >= 0.0
//    <2> n <= 1.0
//  Returns: The ring particle radius.  The radius increases
//           monotonically with the value of n.
//  Side Effect: The models are loaded.
//

	static double calcuateRadius (double n);

public:
//
//  Default Constructor
//
//  Purpose: To create a RingParticle with a default position
//           and seed.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingParticle is created at the origin
//               with a seed value of 0.
//

	RingParticle ();

//
//  Constructor
//
//  Purpose: To create a RingParticle at the specified position
//           and with the specified seed value.
//  Parameter(s):
//    <1> position: The position for the RingParticle
//    <2> seed: The seed value to calculate the other attributes
//              from
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingParticle is created at position
//               position with a seed value of seed.
//

	RingParticle (const Vector3& position, unsigned int seed);

//
//  Copy Constructor
//
//  Purpose: To create a RingParticle as a copy of another.
//  Parameter(s):
//    <1> original: The RingParticle to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingParticle is created with the same
//               position and seed value as original.
//

	RingParticle (const RingParticle& original);

//
//  Destructor
//
//  Purpose: To safely destroy a RingParticle without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	~RingParticle ();

//
//  Assignment Operaator
//
//  Purpose: To modify this RingParticle to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The RingParticle to copy
//  Precondition(s): N/A
//  Returns: A reference to this RingParticle.
//  Side Effect: This RIngParticle is modified to have the same
//               position and seed value as original.
//

	RingParticle& operator= (const RingParticle& original);

//
//  getPosition
//
//  Purpose: To determine the position of this RingParticle.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The position of this RingParticle.
//  Side Effect: N/A
//

	const Vector3& getPosition () const;

//
//  getRadius
//
//  Purpose: To determine the radius of this RingParticle.  The
//           radius is calculated from the seed value.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The radius of this RingParticle.
//  Side Effect: N/A
//

	double getRadius () const;

//
//  draw
//
//  Purpose: To display this RingParticle using OpenGL graphics.
//  Parameter(s):
//    <1> camera_position: The position of the camera
//  Precondition(s):
//    <1> isLoaded()
//  Returns: N/A
//  Side Effect: This RingParticle is displayed at position.
//               All other properties of this RingParticle are
//               calculated from the seed value.  The LOD
//               displayed is based on the camera position.
//

	void draw (const Vector3& camera_position) const;

//
//  init
//
//  Purpose: To set this RingParticle to have the specified
//           position and seed value.
//  Parameter(s):
//    <1> position: The new position for the RingParticle
//    <2> seed: The new seed value to calculate the other
//              attributes from
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This RingParticle set to be at position
//               position and have a seed value of seed.
//

	void init (const Vector3& position, unsigned int seed);

private:
//
//  invariant
//
//  Purpose: To determine if the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	Vector3 m_position;
	double m_radius;
	unsigned int m_material;
	Vector3 m_fixed_rotation_axis;
	double m_fixed_rotation_degrees;
};



#endif

//
//  RingSystem.h
//

#ifndef RING_SYSTEM_H
#define RING_SYSTEM_H

#include <vector>

#include "../../ObjLibrary/Vector3.h"

#include "RingSectorIndex.h"
#include "RingParticle.h"
#include "RingSector.h"
#include "CoordinateSystem.h"
#include "WorleyPoint.h"
#include "FractalPerlinNoiseInterface.h"
#include "FractalPerlinNoiseDummy.h"
#include "GeometricCollisions.h"



//
//  RingSystem
//
//  A class to manage the ring for the planet.  This class
//    handles the ring particles.  The ring particles are not
//    actually stored; they are generated procedurally as
//    needed.
//
//  The ring is centered on the origin aligned with the XZ
//    plane.  The basic density of the ring at a position is
//    defined by these parameters:
//    <1> Half Thickness: The distance from the XZ plane that
//                        the ring stretches along the Y axis
//    <2> Inner Radius: The distance from the origin for the
//                      inside edge of the ring.  The hole for
//                      the planet inside the inside edge of the
//                      ring is shaped like a flat slice of a
//                      sphere.
//    <3> Base Outer Radius: The value the outer radius is
//                           calculated from.  The actual outer
//                           radius is a semicirle of radius
//                           thickness centered on the XZ plane
//                           at this distance out.
//    <4> Density Max: The theortical maximum density for the
//                     ring.  The real ring density will be less
//                     than this before randomness is added.
//    <5> Density Factor: A factor controlling how shaply ring
//                        density falls off near the edge of the
//                        ring.  A large value makes the density
//                        fall off more sharply.
//
//  A RingSystem also can can include 1 or more spherical holes.
//    No ring particles are generated in these holes.
//
//  Class Invariant:
//    <1> m_half_thickness >= 0.0
//    <2> m_inner_radius >= 0.0
//    <3> m_outer_radius_base >= m_inner_radius
//    <4> m_density_max >= 0.0
//    <5> m_density_factor >= 0.0
//    <6> m_density_factor <= 1.0
//

class RingSystem
{
public:
//
//  Default Constructor
//
//  Purpose: To create a RingSystem containing no ring particles
//           or holes.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingSystem is created with default ring
//               parameters.  This results in there being no
//               ring particles in the system.  No holes are
//               added.
//

	RingSystem ();

//
//  Constructor
//
//  Purpose: To create a RingSystem with the specified ring
//           parameters and no holes.
//  Parameter(s):
//    <1> thickness: The thickness of the ring at its widest
//    <2> inner_radius: The inner radius of the ring
//    <3> outer_radius_base: The base outer radius of the ring
//    <4> density_max: The theoretical maximum density of the
//                     ring
//    <5> density_factor: The rate at which ring density
//                        increases near the ring edge
//  Precondition(s):
//    <1> half_thickness >= 0.0
//    <2> inner_radius >= 0.0
//    <3> outer_radius_base >= inner_radius
//    <4> density_max >= 0.0
//    <5> density_factor >= 0.0
//    <6> density_factor <= 1.0
//  Returns: N/A
//  Side Effect: A new RingSystem is created with the specified
//               ring parameters.  No holes are added.
//

	RingSystem (double half_thickness,
	            double inner_radius,
	            double outer_radius_base,
	            double density_max,
	            double density_factor);

//
//  Copy Constructor
//
//  Purpose: To create a RingSystem containing the same ring
//           parameters and holes as another.
//  Parameter(s):
//    <1> original: The RingSystem to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingSystem is created with the same ring
//               parameters and holes as original.  This results
//               in it having the same ring particles.
//

	RingSystem (const RingSystem& original);

//
//  Destructor
//
//  Purpose: To safelty destroy a RingSystem without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	~RingSystem ();

//
//  Assignment Operator
//
//  Purpose: To modify this RingSystem to contain the same ring
//           parameters and holes as another.
//  Parameter(s):
//    <1> original: The RingSystem to copy
//  Precondition(s): N/A
//  Returns: A reference to this RingSystem.
//  Side Effect: This RingSystem is set to have the same ring
//               parameters and holes as original.  This results
//               in it having the same ring particles.
//

	RingSystem& operator= (const RingSystem& original);

//
//  getDensityAtPosition
//
//  Purpose: To determine the ring particle density for the
//           specified position.  At the center of a sector,
//           this is the number of ring particles in the
//           sector.
//  Parameter(s):
//    <1> position: The position to query the denisyt at
//  Precondition(s): N/A
//  Returns: The density of this RingSystem at position.
//  Side Effect: N/A
//

	double getDensityAtPosition (const Vector3& position) const;

//
//  draw
//
//  Purpose: To display the portion of this RingSystem in view
//           of the camera with the specified coordinates.
//  Parameter(s):
//    <1> camera_coordinates: The camera coordinates
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The ring particles in this RingSystem that are
//               in view of a camera with coordinate system
//               camera_coordinates are displayed.
//

	void draw(const CoordinateSystem& camera_coordinates) const;

//
//  init
//
//  Purpose: To set this RingSystem with the specified ring
//           parameters and no holes.
//  Parameter(s):
//    <1> thickness: The thickness of the ring at its widest
//    <2> inner_radius: The inner radius of the ring
//    <3> outer_radius_base: The base outer radius of the ring
//    <4> density_max: The theoretical maximum density of the
//                     ring
//    <5> density_factor: The rate at which ring density
//                        increases near the ring edge
//  Precondition(s):
//    <1> half_thickness >= 0.0
//    <2> inner_radius >= 0.0
//    <3> outer_radius_base >= inner_radius
//    <4> density_max >= 0.0
//    <5> density_factor >= 0.0
//    <6> density_factor <= 1.0
//  Returns: N/A
//  Side Effect: This RingSystem is set to have the specified
//               ring parameters.  An holes are removed.
//

	void init (double half_thickness,
	           double inner_radius,
	           double outer_radius_base,
	           double density_max,
	           double density_factor);

//
//  addHole
//
//  Purpose: To add a spherical hole to this RingSystem with the
//           specified center and radius.
//  Parameter(s):
//    <1> position: The hole center
//    <2> radius: The hole radius
//  Precondition(s):
//    <1> radius >= 0.0
//  Returns: N/A
//  Side Effect: A hole of radius radius is added to this
//               RingSystem centered on position position.  Ring
//               particles will not be generated inside this
//               hole.
//

	void addHole (const Vector3& position, double radius);

//
//  removeAllHoles
//
//  Purpose: To remove all holes from this RingSystem.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All holes in this RingSystem are removed.
//

	void removeAllHoles ();
    
//
//  handleRingParticleCollisions
//
//  Purpose: A function which handles all collisions with a given
//           sphere and a RingParticle
//  Parameter(s):
//    <1> sphere_centre: Centre of the object we are testing collisions
//                       against
//    <2> sphere_radius: Radius of the object we are testing collisions
//                       against
//  Precondition(s): N/A
//  Returns: Whether a collision has occurred with the given sphere
//  Side Effect: N/A
//
    bool handleRingParticleCollision(
                const Vector3& sphere_centre, double sphere_radius);
    
private:
//
//  getRingSector
//
//  Purpose: To generate the RingSector for a specifed sector in
//           the ring.
//  Parameter(s):
//    <1> index: The index of the ring sector to generate
//  Precondition(s): N/A
//  Returns: The information for ring sector index, including
//           the ring particles.
//  Side Effect: N/A
//

	RingSector getRingSector (
	                        const RingSectorIndex& index) const;

//
//  invariant
//
//  Purpose: To determine of the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	//
	//  Hole
	//
	//  A record to represent a hole in the ring.  A Hole has a
	//    position and a radius.  There is a hole in the ring
	//    particles around each Hole.
	//

	struct Hole
	{
		Vector3 m_position;
		double m_radius;

		Hole () : m_position(), m_radius(0.0) {}

		Hole (const Vector3& position, double radius)
			: m_position(position), m_radius(radius)
		{}
	};

private:
	double m_half_thickness;
	double m_inner_radius;
	double m_outer_radius_base;
	double m_density_max;
	double m_density_factor;
	std::vector<Hole> mv_holes;

	FractalPerlinNoiseDummy m_fractal_perlin_noise;
	WorleyPoint3 m_worley_points;
};



#endif

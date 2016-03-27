//
//  RingSystem.cpp
//

#include <cassert>
#include <cmath>
#include <vector>

#include "../../ObjLibrary/Vector3.h"

#include "RingSectorSize.h"
#include "RingSectorIndex.h"
#include "RingParticle.h"
#include "RingSector.h"
#include "CoordinateSystem.h"
#include "FractalPerlinNoiseInterface.h"
#include "FractalPerlinNoiseDummy.h"
#include "RingSystem.h"

using namespace std;
namespace
{
	const double HALF_THICKNESS_DEFAULT    = 0.0;
	const double INNER_RADIUS_DEFAULT      = 0.0;
	const double OUTER_RADIUS_BASE_DEFAULT = 0.0;
	const double DENSITY_MAX_DEFAULT       = 0.0;
	const double DENSITY_FACTOR_DEFAULT    = 0.0;

	const double PERLIN_NOISE_FACTOR = 0.2;  // 2.0 / 3.0; for real perlin noise



	void testFrequencyDistribution ()
	{
		static const unsigned int FREQUENCY_COUNT = 1000;
		static const unsigned int TRIALS = 1000000;

		unsigned int a_frequency[FREQUENCY_COUNT];

		for(unsigned int i = 0; i < FREQUENCY_COUNT; i++)
			a_frequency[i] = 0;

		unsigned int seed = 1;
		for(unsigned int i = 0; i < TRIALS; i++)
		{
			RingParticle rp(Vector3::ZERO, seed);

			double radius_fraction = rp.getRadius() / RingParticle::RADIUS_MAX;
			assert(radius_fraction >= 0.0);
			assert(radius_fraction < 1.0);
			unsigned int index = (unsigned int)(radius_fraction * FREQUENCY_COUNT);
			assert(index < FREQUENCY_COUNT);
			a_frequency[index]++;

			PseudorandomGridSuperclass::calculateNextPseudorandom(seed);
		}

		for(unsigned int i = 1; i < FREQUENCY_COUNT; i++)
			cout << a_frequency[i] << endl;
	}




	const bool DEBUGGING_CHOOSING_SECTORS = false;
	const bool DEBUGGING_SECTOR_DENSITY   = false;
}



RingSystem :: RingSystem ()
		: m_half_thickness(HALF_THICKNESS_DEFAULT),
		  m_inner_radius(INNER_RADIUS_DEFAULT),
		  m_outer_radius_base(OUTER_RADIUS_BASE_DEFAULT),
		  m_density_max(DENSITY_MAX_DEFAULT),
		  m_density_factor(DENSITY_FACTOR_DEFAULT),
		  mv_holes(),
		  m_fractal_perlin_noise(),
		  m_worley_points()
{
	//testFrequencyDistribution();

	assert(invariant());
}

RingSystem :: RingSystem (double half_thickness,
                          double inner_radius,
                          double outer_radius_base,
                          double density_max,
                          double density_factor)
		: m_half_thickness(half_thickness),
		  m_inner_radius(inner_radius),
		  m_outer_radius_base(outer_radius_base),
		  m_density_max(density_max),
		  m_density_factor(density_factor),
		  mv_holes(),
		  m_fractal_perlin_noise(),
		  m_worley_points()
{
	assert(half_thickness >= 0.0);
	assert(inner_radius >= 0.0);
	assert(outer_radius_base >= inner_radius);
	assert(density_max >= 0.0);
	assert(density_factor >= 0.0);
	assert(density_factor <= 1.0);

	assert(invariant());
}

RingSystem :: RingSystem (const RingSystem& original)
		: m_half_thickness(original.m_half_thickness),
		  m_inner_radius(original.m_inner_radius),
		  m_outer_radius_base(original.m_outer_radius_base),
		  m_density_max(original.m_density_max),
		  m_density_factor(original.m_density_factor),
		  mv_holes(original.mv_holes),
		  m_fractal_perlin_noise(original.m_fractal_perlin_noise),
		  m_worley_points(original.m_worley_points)
{
	assert(invariant());
}

RingSystem :: ~RingSystem ()
{
}

RingSystem& RingSystem :: operator= (const RingSystem& original)
{
	if(&original != this)
	{
		m_half_thickness       = original.m_half_thickness;
		m_inner_radius         = original.m_inner_radius;
		m_outer_radius_base    = original.m_outer_radius_base;
		m_density_max          = original.m_density_max;
		m_density_factor       = original.m_density_factor;
		mv_holes               = original.mv_holes;
		m_fractal_perlin_noise = original.m_fractal_perlin_noise;
		m_worley_points        = original.m_worley_points;
	}

	assert(invariant());
	return *this;
}



double RingSystem :: getDensityAtPosition (const Vector3& position) const
{
	static const double TWO_OVER_PI = 0.63661977236758134307553505349006;

	if(DEBUGGING_SECTOR_DENSITY)
		cout << "Calculating ring sector density at " << position << endl;

	double thickness_density  = m_half_thickness   - fabs((double)(position.y));
	double inner_edge_density = position.getNorm() - m_inner_radius;

	double combined_density = (thickness_density < inner_edge_density)
	                          ? thickness_density
	                          : inner_edge_density;

	if(position.isNormXZGreaterThan(m_outer_radius_base))
	{
		double horizontal = position.getNormXZ() - m_outer_radius_base;
		double vertical   = position.y;
		double outer_edge_density = m_half_thickness -
		                            sqrt(horizontal * horizontal +
		                                 vertical   * vertical);
		if(outer_edge_density < combined_density)
			combined_density = outer_edge_density;
	}

	if(DEBUGGING_SECTOR_DENSITY)
		cout << "\tBefore holes: " << combined_density << endl;

	for(unsigned int i = 0; i < mv_holes.size(); i++)
	{
		double hole_distance = position.getDistance(mv_holes[i].m_position);
		double hole_density  = hole_distance - mv_holes[i].m_radius;
		if(hole_density < combined_density)
			combined_density = hole_density;
	}

	if(DEBUGGING_SECTOR_DENSITY)
		cout << "\tAfter holes: " << combined_density << endl;

	double scaled_density = combined_density * m_density_factor;
	double curved_density = atan(scaled_density) * TWO_OVER_PI;

	if(DEBUGGING_SECTOR_DENSITY)
		cout << "\tAfter curving: " << curved_density << endl;

	double randomized_density = curved_density +
	                            m_fractal_perlin_noise.getAt(position / RING_SECTOR_SIZE) *
	                            PERLIN_NOISE_FACTOR;

	if(DEBUGGING_SECTOR_DENSITY)
		cout << "\tAfter randomization: " << randomized_density << endl;

	if(randomized_density < 0.0)
		return 0.0;
	else
		return randomized_density * m_density_max;
}

void RingSystem :: draw (const CoordinateSystem& camera_coordinates) const
{
	const Vector3& camera_position = camera_coordinates.getPosition();
	RingSectorIndex camera_index(camera_position);

	if(DEBUGGING_CHOOSING_SECTORS)
		cout << "Drawing around ring sector " << camera_index << endl;

	for(int dx = -RING_SECTOR_DRAW_FROM_CAMERA_COUNT; dx <= RING_SECTOR_DRAW_FROM_CAMERA_COUNT; dx++)
	{
		short x = camera_index.getX() + dx;
		for(int dy = -RING_SECTOR_DRAW_FROM_CAMERA_COUNT; dy <= RING_SECTOR_DRAW_FROM_CAMERA_COUNT; dy++)
		{
			short y = camera_index.getY() + dy;
			for(int dz = -RING_SECTOR_DRAW_FROM_CAMERA_COUNT; dz <= RING_SECTOR_DRAW_FROM_CAMERA_COUNT; dz++)
			{
				short z = camera_index.getZ() + dz;
				RingSectorIndex ring_sector_index(x, y, z);

				const RingSector& ring_sector = getRingSector(ring_sector_index);
				unsigned int particle_count = (unsigned int)ring_sector.mv_ring_particles.size();

				if(DEBUGGING_CHOOSING_SECTORS && dx == 0 && dy == 0 && dz == 0)
				{
					cout << "\tRing Sector " << ring_sector.m_index << "\t==> "     << ring_sector.m_index.getCenter() << endl;
					cout << "\t\t "          << particle_count      << " particles" << endl;
				}

				for(unsigned int i = 0; i < particle_count; i++)
				{
					ring_sector.mv_ring_particles[i].draw(camera_position);
					if(DEBUGGING_CHOOSING_SECTORS && dx == 0 && dy == 0 && dz == 0)
						cout << "\t\t#" << i << ":\t" << ring_sector.mv_ring_particles[i].getPosition() << endl;
					}
			}
		}
	}
}

void RingSystem :: init (double half_thickness,
                         double inner_radius,
                         double outer_radius_base,
                         double density_max,
                         double density_factor)
{
	assert(half_thickness >= 0.0);
	assert(inner_radius >= 0.0);
	assert(outer_radius_base >= inner_radius);
	assert(density_max >= 0.0);
	assert(density_factor >= 0.0);
	assert(density_factor <= 1.0);

	m_half_thickness    = half_thickness;
	m_inner_radius      = inner_radius;
	m_outer_radius_base = outer_radius_base;
	m_density_max       = density_max;
	m_density_factor    = density_factor;

	removeAllHoles();

	assert(invariant());
}

void RingSystem :: addHole(const Vector3& position, double radius)
{
	assert(radius >= 0.0);

	mv_holes.push_back(Hole(position, radius));

	assert(invariant());
}

void RingSystem :: removeAllHoles ()
{
	mv_holes.clear();

	assert(invariant());
}


RingSector RingSystem :: getRingSector (const RingSectorIndex& index) const
{
	Vector3 center = index.getCenter();

	double       real_density = getDensityAtPosition(center);
	unsigned int int_density  = (unsigned int)(real_density + 0.5);  // round, not truncate
	vector<WorleyPoint3::Point3> v_points = m_worley_points.getPoints(int_density,
	                                                                 index.getX(),
	                                                                 index.getY(),
	                                                                 index.getZ());
	RingSector ring_sector;
	ring_sector.m_index = index;
	ring_sector.m_density = int_density;
	ring_sector.mv_ring_particles.resize(int_density);
	for(unsigned int i = 0; i < int_density; i++)
	{
		const WorleyPoint3::Point3& point = v_points[i];
		Vector3 position = Vector3(point.m_x, point.m_y, point.m_z) * RING_SECTOR_SIZE;// + center;
		ring_sector.mv_ring_particles[i].init(position, v_points[i].m_seed);
	}

	return ring_sector;
}

bool RingSystem::handleRingParticleCollision(const Vector3& sphere_centre, double sphere_radius)
{
    RingSectorIndex centre(sphere_centre);
    
    for(int dx = -1; dx <= 1; dx++)
    {
        short x = (centre.getX() + dx);
        for(int dy = -1; dy <= 1; dy++)
        {
            short y = (centre.getY() + dy);
            for(int dz = -1; dz <= 1; dz++)
            {
                short z = (centre.getZ() + dz);
                
                RingSectorIndex index(x, y, z);
                const RingSector& ring_sector = getRingSector(index);
                
                if (!GeometricCollisions::sphereVsCuboid(sphere_centre,
                                                        sphere_radius,
                                                        index.getCenter(),
                                                        RING_SECTOR_HALF_SIZE))
                {
                    continue;
                }
                unsigned int particle_count = (unsigned int)ring_sector.mv_ring_particles.size();
                for(unsigned int i = 0; i < particle_count; i++)
                {
                    RingParticle particle = ring_sector.mv_ring_particles[i];
                    if (GeometricCollisions::sphereVsSphere(sphere_centre,
                                                            sphere_radius,
                                                            particle.getPosition(),
                                                            particle.getRadius()))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

std::vector<RingParticleData> RingSystem::getRingParticles(
                                           const Vector3& sphere_centre,
                                           double sphere_radius) const
{
    vector<RingParticleData> list;
    RingSectorIndex centre(sphere_centre);
    
    for(int dx = -1; dx <= 1; dx++)
    {
        short x = (centre.getX() + dx);
        for(int dy = -1; dy <= 1; dy++)
        {
            short y = (centre.getY() + dy);
            for(int dz = -1; dz <= 1; dz++)
            {
                short z = (centre.getZ() + dz);
                
                RingSectorIndex index(x, y, z);
                const RingSector& ring_sector = getRingSector(index);
                
                if (!GeometricCollisions::sphereVsCuboid(sphere_centre,
                                                         sphere_radius,
                                                         index.getCenter(),
                                                         RING_SECTOR_HALF_SIZE))
                {
                    continue;
                }
                unsigned int particle_count = (unsigned int)ring_sector.mv_ring_particles.size();
                for(unsigned int i = 0; i < particle_count; i++)
                {
                    RingParticleData temp = {
                        ring_sector.mv_ring_particles[i].getPosition(),
                        ring_sector.mv_ring_particles[i].getRadius()
                    };
                    list.push_back(temp);
                }
            }
        }
    }
    return list;
}

bool RingSystem :: invariant () const
{
	if (m_half_thickness < 0.0) return false;
	if (m_inner_radius < 0.0) return false;
	if (m_outer_radius_base < m_inner_radius) return false;
	if (m_density_max < 0.0) return false;
	if (m_density_factor < 0.0) return false;
	if (m_density_factor > 1.0) return false;
	return true;
}


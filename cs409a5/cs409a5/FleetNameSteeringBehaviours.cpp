//
//  RedSteeringBehaviours.cpp
//
//  All through this file, you should replace "Red",
//    "Red", and "RED" with the name of your
//     fleet.
//

#include <cassert>
#include <cstdlib>  // for rand()
#include <string>
#include <cmath>
#include <iostream>

#include "../../ObjLibrary/Vector3.h"

#include "TimeSystem.h"
#include "PhysicsObjectId.h"
#include "WorldInterface.h"
#include "FleetNameSteeringBehaviours.h"

using namespace std;
using namespace FleetName;
namespace
{
	const double DEFAULT_DESIRED_DISTANCE           = 0.0;
	const double DEFAULT_DESIRED_DISTANCE_TOLERANCE = 1.0;

	const double AVOID_SPEED_FACTOR_MIN  = 0.1;
	const double AVOID_SIDEWAYS_NORM_MIN = 0.01;

	const unsigned int EXPLORE_POSITION_ATTEMPT_COUNT = 100;

	const bool DEBUGGING_INTERSECTION_TIME = false;
	const bool DEBUGGING_PATROL_SPHERE     = false;
	const bool DEBUGGING_AVOID             = false;



	inline double random0 ()
	{
		return rand () / (RAND_MAX + 1.0);
	}

	inline double random2 (double min_value, double max_value)
	{
		assert(min_value <= max_value);

		return min_value + random0() * (max_value - min_value);
	}
}



const double SteeringBehaviour :: NO_INTERSECTION_POSSIBLE = -1.0e40;

const double SteeringBehaviour :: SLOW_DISTANCE_PADDING_FACTOR  =   2.0;
const double SteeringBehaviour :: EXPLORE_DISTANCE_NEW_POSITION = 100.0;



double SteeringBehaviour :: getIntersectionTime (const Vector3& agent_position,
                                                 double agent_speed,
                                                 const Vector3& target_position)
{
	assert(agent_speed >= 0.0);

	if(agent_speed == 0.0)
		return NO_INTERSECTION_POSSIBLE;  // can't catch if can't move

	Vector3 relative_position = target_position - agent_position;
	return relative_position.getNorm() / agent_speed;
}

double SteeringBehaviour :: getIntersectionTime (const Vector3& agent_position,
                                                 double agent_speed,
                                                 const Vector3& target_position,
                                                 const Vector3& target_velocity)
{
	assert(agent_speed >= 0.0);

	if(agent_speed == 0.0)
	{
		if(DEBUGGING_INTERSECTION_TIME)
			cout << "No #1" << endl;
		return NO_INTERSECTION_POSSIBLE;  // can't catch if can't move
	}

	if(target_velocity.isZero())
	{
		if(DEBUGGING_INTERSECTION_TIME)
			cout << "No #2" << endl;
		return getIntersectionTime(agent_position, agent_speed, target_position);
	}

	Vector3   relative_position = target_position - agent_position;
	double  agent_speed_squared = agent_speed * agent_speed;
	double target_speed_squared = target_velocity.getNormSquared();
	double     distance_squared = relative_position.getNormSquared();

	if(agent_speed_squared == target_speed_squared)
	{
		//
		//  When the speeds are the same.  I don't know why case
		//    this corresponds to, but we don't want a
		//    divide-by-0 error.  Some of these should have
		//    solutions, and those will sadly be missed.
		//
		if(DEBUGGING_INTERSECTION_TIME)
			cout << "No #3" << endl;
		return NO_INTERSECTION_POSSIBLE;
	}

	double a = target_speed_squared - agent_speed_squared;
	double b = target_velocity.dotProduct(-relative_position) * -2.0;
	double c = distance_squared;
	if(DEBUGGING_INTERSECTION_TIME)
	{
		cout << "\tdistance: " << sqrt(distance_squared) << endl;
		cout << "\ta: " << a << "\t==> " << (a / target_speed_squared) << endl;
		cout << "\tb: " << b << "\t==> " << (b / target_speed_squared) << endl;
		cout << "\tc: " << c << "\t==> " << (c / target_speed_squared) << endl;
	}

	double discriminant = b * b - 4 * a * c;
	if(discriminant < 0.0)
	{
		if(DEBUGGING_INTERSECTION_TIME)
			cout << "No #4" << endl;
		return NO_INTERSECTION_POSSIBLE;  // target is too fast
	}

	assert(discriminant >= 0.0);
	double sqrt_discriminant = sqrt(discriminant);

	assert(a != 0.0);
	double time1 = (-b - sqrt_discriminant) / (a * 2.0);
	if(time1 >= 0.0)
	{
		if(DEBUGGING_INTERSECTION_TIME)
		{
			cout << "time1: "       <<  time1
			     << "  \tcurrent: " <<          TimeSystem::getFrameStartTime()
			     << "  \ttotal: "   << (time1 + TimeSystem::getFrameStartTime()) << endl;
		}
		return time1;  // first possible intersection
	}
	
	assert(a != 0.0);
	double time2 = (-b + sqrt_discriminant) / (a * 2.0);
	if(time2 >= 0.0)
	{
		if(DEBUGGING_INTERSECTION_TIME)
		{
			cout << "time2: "       <<  time2
			     << "  \tcurrent: " <<          TimeSystem::getFrameStartTime()
			     << "  \ttotal: "   << (time2 + TimeSystem::getFrameStartTime()) << endl;
		}
		return time2;  // second possible intersection
	}

	if(DEBUGGING_INTERSECTION_TIME)
		cout << "No #5" << endl;
	return NO_INTERSECTION_POSSIBLE;  // both intersections are in the past
}

Vector3 SteeringBehaviour :: getAimDirection (const Vector3& shot_start,
                                              double shot_speed,
                                              const Vector3& target_position,
                                              const Vector3& target_velocity)
{
	assert(shot_speed >= 0.0);

	double intersection_time = target_velocity.isZero()
	                           ? getIntersectionTime(shot_start, shot_speed, target_position)
	                           : getIntersectionTime(shot_start, shot_speed, target_position, target_velocity);

	if(intersection_time == NO_INTERSECTION_POSSIBLE)
		return Vector3::ZERO;
	else
	{
		Vector3 intersection_position = target_position + target_velocity * intersection_time;
		return (intersection_position - shot_start).getNormalizedSafe();
	}
}



SteeringBehaviour :: SteeringBehaviour (const PhysicsObjectId& id_agent)
		: m_id_agent(id_agent),
		  m_steering_behaviour(STOP),
		  m_explore_position(),
		  m_sphere_center(),
		  m_desired_distance(DEFAULT_DESIRED_DISTANCE),
		  m_desired_distance_tolerance(DEFAULT_DESIRED_DISTANCE_TOLERANCE)
{
	assert(id_agent != PhysicsObjectId::ID_NOTHING);

	assert(invariant());
}

SteeringBehaviour :: SteeringBehaviour (const SteeringBehaviour& original)
		: m_id_agent(original.m_id_agent),
		  m_steering_behaviour(original.m_steering_behaviour),
		  m_explore_position(original.m_explore_position),
		  m_sphere_center(original.m_sphere_center),
		  m_desired_distance(original.m_desired_distance),
		  m_desired_distance_tolerance(original.m_desired_distance_tolerance)
{
	assert(invariant());
}

SteeringBehaviour :: ~SteeringBehaviour ()
{
}

SteeringBehaviour& SteeringBehaviour :: operator= (const SteeringBehaviour& original)
{
	if(&original != this)
	{
		m_id_agent                   = original.m_id_agent;
		m_steering_behaviour         = original.m_steering_behaviour;
		m_explore_position           = original.m_explore_position;
		m_sphere_center              = original.m_sphere_center;
		m_desired_distance           = original.m_desired_distance;
		m_desired_distance_tolerance = original.m_desired_distance_tolerance;
	}

	assert(invariant());
	return *this;
}



Vector3 SteeringBehaviour :: stop (const WorldInterface& world)
{
	m_steering_behaviour = STOP;

	assert(invariant());
	return Vector3::ZERO;
}

Vector3 SteeringBehaviour :: arrive (const WorldInterface& world,
                                     const PhysicsObjectId& id_target)
{
	assert(id_target != PhysicsObjectId::ID_NOTHING);

	// no initialization needed
	m_steering_behaviour = ARRIVE;

	if(!world.isAlive(id_target))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	assert(world.isAlive(id_target));
	Vector3 result = arrive(world, world.getPosition(id_target));
	assert(invariant());
	return result;
}

Vector3 SteeringBehaviour :: arrive (const WorldInterface& world,
                                     const Vector3& target_position)
{
	// no initialization needed
	m_steering_behaviour = ARRIVE;

	PhysicsObjectId id_agent = m_id_agent;

	if(!world.isAlive(id_agent))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position = world.getPosition(id_agent);

	if(agent_position == target_position)
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	double agent_speed = world.getShipSpeedMax(id_agent);
	assert(agent_speed >= 0.0);
	double agent_acceleration = world.getShipAcceleration(id_agent);
	assert(agent_acceleration >= 0.0);

	double distance       = agent_position.getDistance(target_position);
	double max_safe_speed = calculateMaxSafeSpeed(distance, agent_acceleration);
	if(max_safe_speed < agent_speed)
		agent_speed = max_safe_speed;
	assert(agent_speed >= 0.0);
	assert(agent_speed <= max_safe_speed);

	assert(invariant());
	return (target_position - agent_position).getCopyWithNorm(agent_speed);
}

Vector3 SteeringBehaviour :: seek (const WorldInterface& world,
                                   const PhysicsObjectId& id_target)
{
	assert(id_target != PhysicsObjectId::ID_NOTHING);

	// no initialization needed
	m_steering_behaviour = SEEK;

	if(!world.isAlive(id_target))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	assert(world.isAlive(id_target));
	Vector3 result = seek(world, world.getPosition(id_target));
	assert(invariant());
	return result;
}

Vector3 SteeringBehaviour :: seek (const WorldInterface& world,
                                   const Vector3& target_position)
{
	// no initialization needed
	m_steering_behaviour = SEEK;

	if(!world.isAlive(m_id_agent))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position = world.getPosition(m_id_agent);

	if(agent_position == target_position)
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	double agent_speed_max = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);
	return (target_position - agent_position).getCopyWithNorm(agent_speed_max);
}

Vector3 SteeringBehaviour :: flee (const WorldInterface& world,
                                   const PhysicsObjectId& id_target)
{
	assert(id_target != PhysicsObjectId::ID_NOTHING);

	// no initialization needed
	m_steering_behaviour = FLEE;

	if(!world.isAlive(id_target))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	assert(world.isAlive(id_target));
	Vector3 result = flee(world, world.getPosition(id_target));
	assert(invariant());
	return result;
}

Vector3 SteeringBehaviour :: flee (const WorldInterface& world,
                                   const Vector3& target_position)
{
	// no initialization needed
	m_steering_behaviour = FLEE;

	if(!world.isAlive(m_id_agent))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position = world.getPosition(m_id_agent);

	if(agent_position == target_position)
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	double agent_speed_max = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);
	return (agent_position - target_position).getCopyWithNorm(agent_speed_max);
}

Vector3 SteeringBehaviour :: pursue (const WorldInterface& world,
                                     const PhysicsObjectId& id_target)
{
	assert(id_target != PhysicsObjectId::ID_NOTHING);

	// no initialization needed
	m_steering_behaviour = PURSUE;

	if(!world.isAlive(m_id_agent) ||
	   !world.isAlive(  id_target))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position  = world.getPosition(m_id_agent);
	Vector3 target_position = world.getPosition(  id_target);

	if(agent_position == target_position)
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	assert(world.isAlive(m_id_agent ));
	assert(world.isAlive(  id_target));
	Vector3 target_velocity   = world.getVelocity(id_target);
	double  agent_speed_max   = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);
	Vector3 aim_direction = getAimDirection(agent_position,
	                                        agent_speed_max,
	                                        target_position,
	                                        target_velocity);
	if(aim_direction.isZero())
	{
		assert(invariant());
		return target_velocity.getCopyWithNormSafe(agent_speed_max);
	}
	else
	{
		assert(invariant());
		return aim_direction * agent_speed_max;
	}
}

Vector3 SteeringBehaviour :: evade (const WorldInterface& world,
                                    const PhysicsObjectId& id_target)
{
	assert(id_target != PhysicsObjectId::ID_NOTHING);

	// no initialization needed
	m_steering_behaviour = EVADE;

	if(!world.isAlive(m_id_agent) ||
	   !world.isAlive(  id_target))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position  = world.getPosition(m_id_agent);
	Vector3 target_position = world.getPosition(  id_target);

	if(agent_position == target_position)
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	assert(world.isAlive(m_id_agent ));
	assert(world.isAlive(  id_target));
	Vector3 target_velocity   = world.getVelocity(id_target);
	double  agent_speed_max   = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);
	Vector3 aim_direction = getAimDirection(agent_position,
	                                        agent_speed_max,
	                                        target_position,
	                                        target_velocity);
	if(aim_direction.isZero())
	{
		assert(invariant());
		return (agent_position - target_position).getCopyWithNorm(agent_speed_max);
	}
	else
	{
		assert(invariant());
		return aim_direction * -agent_speed_max;
	}
}

Vector3 SteeringBehaviour :: aim (const WorldInterface& world,
                                  const PhysicsObjectId& id_target,
                                  double shot_speed)
{
	assert(id_target != PhysicsObjectId::ID_NOTHING);
	assert(shot_speed >= 0.0);

	// no initialization needed
	m_steering_behaviour = AIM;

	if(!world.isAlive(m_id_agent) ||
	   !world.isAlive(  id_target))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position  = world.getPosition(m_id_agent);
	Vector3 target_position = world.getPosition(  id_target);

	if(agent_position == target_position)
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	assert(world.isAlive(m_id_agent ));
	assert(world.isAlive(  id_target));
	double  agent_speed_max = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);
	Vector3 target_velocity = world.getVelocity(id_target);
	Vector3   aim_direction = getAimDirection(agent_position, shot_speed, target_position, target_velocity);

	assert(invariant());
	return aim_direction * agent_speed_max;
}

Vector3 SteeringBehaviour :: explore (const WorldInterface& world,
                                      double distance_new_position_min,
                                      double distance_new_position_max)
{
	assert(distance_new_position_min >= 0.0);
	assert(distance_new_position_min <= distance_new_position_max);
	assert(distance_new_position_min >  EXPLORE_DISTANCE_NEW_POSITION);

	double position_distance           = (distance_new_position_max + distance_new_position_min) * 0.5;
	double position_distance_tolerance = (distance_new_position_max - distance_new_position_min) * 0.5;

	bool is_new_position_needed = false;
	if(m_steering_behaviour         != EXPLORE ||
	   m_desired_distance           != position_distance ||
	   m_desired_distance_tolerance != position_distance_tolerance)
	{
		m_steering_behaviour         = EXPLORE;
		m_desired_distance           = position_distance;
		m_desired_distance_tolerance = position_distance_tolerance;
		is_new_position_needed       = true;
	}

	if(!world.isAlive(m_id_agent))
	{
		// the explore position doesn't matter at this point

		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position = world.getPosition(m_id_agent);
	double agent_speed_max = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);

	if(agent_position.isDistanceLessThan(m_explore_position, EXPLORE_DISTANCE_NEW_POSITION) ||
	   is_new_position_needed)
	{
		m_explore_position = calculateExplorePosition(agent_position);
	}

	return (m_explore_position - agent_position).getCopyWithNorm(agent_speed_max);
}

Vector3 SteeringBehaviour :: escort (const WorldInterface& world,
                                     const PhysicsObjectId& id_target,
                                     const Vector3& offset)
{
	assert(id_target != PhysicsObjectId::ID_NOTHING);

	// no initialization needed
	m_steering_behaviour = ESCORT;

	//
	//  This is a variation on the arrive steering behaviour.
	//    First we work out the desired position from the
	//    offset.  Then we arrive at that position.  Finally we
	//    add on the velocity of the target and truncate to our
	//    maximum speed.
	//
	//  A more sophisticated implementation would use a form of
	//    arrive based on pursue instead of seek.  Or maybe this
	//    already happens; I'm not sure.  In either case,
	//    slowing down for arrival messes up time predictions.
	//

	if(!world.isAlive(m_id_agent) ||
	   !world.isAlive(  id_target))
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	assert(world.isAlive(m_id_agent ));
	assert(world.isAlive(  id_target));
	Vector3 agent_position  = world.getPosition(m_id_agent);
	Vector3 target_velocity = world.getVelocity(  id_target);
	Vector3 escort_position = calculateEscortPosition(world, id_target, offset);

	if(agent_position == escort_position)
	{
		// we are in the correct position, so just match speeds
		assert(invariant());
		return target_velocity;
	}

	double agent_speed_max    = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);
	double agent_acceleration = world.getShipSpeedMax(m_id_agent);
	assert(agent_acceleration >= 0.0);

	double current_distance = agent_position.getDistance(escort_position);

	double  desired_relative_speed    = calculateMaxSafeSpeed(current_distance, agent_acceleration);
	Vector3 desired_relative_velocity = (escort_position - agent_position).getCopyWithNorm(desired_relative_speed);
	Vector3 desired_agent_velocity    = desired_relative_velocity + target_velocity;

	// limit desired velocity to maximum speed
	assert(invariant());
	return desired_agent_velocity.getTruncated(agent_speed_max);
}

Vector3 SteeringBehaviour :: patrolSphere (const WorldInterface& world,
                                           const Vector3& sphere_center,
                                           double patrol_radius,
                                           double patrol_radius_tolerance)
{
	assert(patrol_radius >= 0.0);
	assert(patrol_radius_tolerance >= 0.0);

	bool is_new_position_needed = false;
	if(m_steering_behaviour         != PATROL_SPHERE ||
	   m_sphere_center              != sphere_center ||
	   m_desired_distance           != patrol_radius ||
	   m_desired_distance_tolerance != patrol_radius_tolerance)
	{
		m_steering_behaviour         = PATROL_SPHERE;
		m_sphere_center              = sphere_center;
		m_desired_distance           = patrol_radius;
		m_desired_distance_tolerance = patrol_radius_tolerance;
		is_new_position_needed       = true;
	}

	if(!world.isAlive(m_id_agent))
	{
		// the explore position doesn't matter at this point

		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position = world.getPosition(m_id_agent);

	if(isNearEnoughPatrolSpherePoint(agent_position) ||
	   is_new_position_needed)
	{
		m_explore_position = calculatePatrolSpherePosition(agent_position);
	}

	double agent_speed_max = world.getShipSpeedMax(m_id_agent);
	assert(agent_speed_max >= 0.0);

	if(DEBUGGING_PATROL_SPHERE)
	{
		cout << "Explore:" << endl;
		cout << "\tAgent position:   " << agent_position << endl;
		cout << "\tSphere position:  " << m_sphere_center << endl;
		cout << "\tExplore position: " << m_explore_position << endl;
	}

	double distance_from_sphere = agent_position.getDistance(m_sphere_center);

	double weight_parallel = 1.0;
	if(m_desired_distance_tolerance > 0.0)
	{
		double distance_outside_optimum = distance_from_sphere - m_desired_distance;

		weight_parallel = distance_outside_optimum / m_desired_distance_tolerance;
		if(weight_parallel > 1.0)
			weight_parallel = 1.0;
		if(weight_parallel < -1.0)
			weight_parallel = -1.0;

		if(DEBUGGING_PATROL_SPHERE)
		{
				cout << "\tSphere distance:  " << distance_from_sphere << endl;
				cout << "\tDesired distance: " << m_desired_distance << endl;
				cout << "\tTolerance:        " << m_desired_distance_tolerance << endl;
				cout << "\tweight_parallel:      " << weight_parallel << endl;
		}

		if(weight_parallel >= 0.0)
			weight_parallel =   weight_parallel * weight_parallel;
		else
			weight_parallel = -(weight_parallel * weight_parallel);

		if(DEBUGGING_PATROL_SPHERE)
				cout << "\t                   => " << weight_parallel << endl;

		assert(weight_parallel <=  1.0);
		assert(weight_parallel >= -1.0);
	}
	assert(weight_parallel <=  1.0);
	assert(weight_parallel >= -1.0);
	double weight_perpendicular = 1.0 - fabs(weight_parallel);
	assert(weight_perpendicular <=  1.0);
	assert(weight_perpendicular >= -1.0);

	if(DEBUGGING_PATROL_SPHERE)
		cout << "\tweight_perpendicular: " << weight_perpendicular << endl;

	Vector3 to_sphere               = m_sphere_center    - agent_position;
	Vector3 to_destination          = m_explore_position - agent_position;
	Vector3 parallel_to_normal      = to_destination.getProjection(to_sphere);
	Vector3 perpendicular_to_normal = to_destination - parallel_to_normal;
	Vector3 desired_vector          = parallel_to_normal      * weight_parallel +
	                                  perpendicular_to_normal * weight_perpendicular;

	assert(invariant());
	return desired_vector.getCopyWithNormSafe(agent_speed_max);
}


/*
// avoid within a spherical distance
Vector3 SteeringBehaviour :: avoid (const WorldInterface& world,
                                    const Vector3& original_velocity,
                                    const Vector3& sphere_center,
                                    double sphere_radius,
                                    double avoid_distance) const
{
	assert(sphere_radius >= 0.0);
	assert(avoid_distance > 0.0);

	if(!world.isAlive(m_id_agent) ||
	   original_velocity.isZero())
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position    = world.getPosition(m_id_agent);
	Vector3 relative_position = sphere_center - agent_position;
	if(relative_position.isNormGreaterThan(sphere_radius + avoid_distance))
	{
		assert(invariant());
		return original_velocity;  // too far away to worry about
	}

	double sphere_distance = relative_position.getNorm();
	assert(avoid_distance != 0.0);
	double distance_fraction = (sphere_distance - sphere_radius) / avoid_distance;
	if(distance_fraction <= 0.0)
		distance_fraction = 0.0;
	if(distance_fraction >= 1.0)  // sadly, there are floating point rounding errors
		distance_fraction = 1.0;
	assert(distance_fraction >= 0.0);
	assert(distance_fraction <= 1.0);

	assert(!original_velocity.isZero());
	Vector3 sideways_vector = -relative_position.getAntiProjection(original_velocity);
	while(sideways_vector.isNormLessThan(0.01))
	{
		// we have almost no sideways, so use a random value
		sideways_vector = Vector3::getRandomUnitVector().getAntiProjection(original_velocity);
		// keep trying until we get a good one
	}
	assert(!sideways_vector.isZero());
	sideways_vector.normalize();

	Vector3 interpolated;
	if(distance_fraction > 0.5)
	{
		// interpolate between forward and sideways
		double small_fraction = distance_fraction * 2.0 - 1.0;
		interpolated = original_velocity.getNormalized() *        small_fraction +
		               sideways_vector                   * (1.0 - small_fraction);
	}
	else
	{
		// interpolate between sideways and away
		double small_fraction = distance_fraction * 2.0;
		interpolated = sideways_vector                    *        small_fraction +
		               -relative_position.getNormalized() * (1.0 - small_fraction);
	}

	double desired_speed = world.getShipSpeedMax(m_id_agent);
	double speed_factor = sqrt(distance_fraction);
	if(speed_factor > AVOID_SPEED_FACTOR_MIN)
		desired_speed *= speed_factor;
	else
		desired_speed *= AVOID_SPEED_FACTOR_MIN;

	assert(invariant());
	return interpolated.getCopyWithNorm(desired_speed);
}
*/

// avoid only when going to collide
Vector3 SteeringBehaviour :: avoid (const WorldInterface& world,
                                    const Vector3& original_velocity,
                                    const Vector3& sphere_center,
                                    double sphere_radius,
                                    double clearance,
                                    double avoid_distance) const
{
	assert(sphere_radius >= 0.0);
	assert(clearance > 0.0);
	assert(clearance <= avoid_distance);

	if(!world.isAlive(m_id_agent) ||
	   original_velocity.isZero())
	{
		assert(invariant());
		return Vector3::ZERO;
	}

	Vector3 agent_position    = world.getPosition(m_id_agent);
	double  agent_radius      = world.getRadius(m_id_agent);
	double  desired_speed     = world.getShipSpeedMax(m_id_agent);
	Vector3 relative_position = sphere_center - agent_position;
	double  radius_sum        = sphere_radius + agent_radius;

	if(relative_position.isNormGreaterThan(radius_sum + avoid_distance))
	{
		if(DEBUGGING_AVOID)
			cout << "Avoid: Outside avoid distance" << endl;
		assert(invariant());
		return original_velocity.getTruncated(desired_speed);  // too far away to worry about
	}

	Vector3 agent_forward = world.getForward(m_id_agent);

	if(relative_position.dotProduct(agent_forward) < 0.0)
	{
		// past center of object; no cylinder
		if(DEBUGGING_AVOID)
			cout << "Avoid: Departing from object" << endl;

		if(relative_position.isNormLessThan(radius_sum + clearance))
		{
			// we are too close, so flee and slow down

			double distance_fraction = (relative_position.getNorm() - radius_sum) / clearance;
			if(DEBUGGING_AVOID)
				cout << "\tInside panic distance: fraction = " << distance_fraction << endl;
			if(distance_fraction < 0.0)
				distance_fraction = 0.0;

			Vector3 interpolated =  original_velocity.getNormalized() *        distance_fraction +
			                       -relative_position.getNormalized() * (1.0 - distance_fraction);

			if(distance_fraction > AVOID_SPEED_FACTOR_MIN)
				desired_speed *= distance_fraction;
			else
				desired_speed *= AVOID_SPEED_FACTOR_MIN;

			if(original_velocity.isNormLessThan(desired_speed))
				desired_speed = original_velocity.getNorm();

			assert(invariant());
			return interpolated.getCopyWithNorm(desired_speed);
		}
		else
		{
			if(DEBUGGING_AVOID)
				cout << "\tPast object" << endl;
			assert(invariant());
			return original_velocity.getTruncated(desired_speed);  // far enough past object
		}
	}
	else
	{
		// have not reached center of object; check against cylinder
		if(DEBUGGING_AVOID)
			cout << "Avoid: Approaching object" << endl;

		double distance_from_cylinder_center = relative_position.getAntiProjection(agent_forward).getNorm();
		double clearance_fraction            = (distance_from_cylinder_center - radius_sum) / clearance;
		if(DEBUGGING_AVOID)
		{
			cout << "\tTo sphere:         " << relative_position << endl;
			cout << "\tDistance_from_cylinder_center: " << distance_from_cylinder_center << endl;
			cout << "\tRadius_sum:        " << radius_sum << endl;
			cout << "\tClearance:         " << clearance << endl;
			cout << "\tFraction:          " << clearance_fraction << endl;
		}

		if(clearance_fraction < 0.0)
		{
			clearance_fraction = 0.0;
			if(DEBUGGING_AVOID)
				cout << "\tLined up at sphere" << endl;
		}

		if(clearance_fraction > 1.0)
		{
			if(DEBUGGING_AVOID)
				cout << "\tOutside cylinder" << endl;
			assert(invariant());
			return original_velocity;  // outside of danger cylinder
		}
		if(DEBUGGING_AVOID)
			cout << "\tModified fraction: " << clearance_fraction << endl;

		assert(!original_velocity.isZero());
		assert(!agent_forward.isZero());
		Vector3 sideways_vector = -relative_position.getAntiProjection(agent_forward);
		while(sideways_vector.isNormLessThan(AVOID_SIDEWAYS_NORM_MIN))
		{
			// we have almost no sideways, so use a random value
			sideways_vector = Vector3::getRandomUnitVector().getAntiProjection(agent_forward);
			// keep trying until we get a good one
		}
		assert(!original_velocity.isZero());
		assert(!sideways_vector.isZero());
		assert(sideways_vector.isOrthogonal(agent_forward));
		if(DEBUGGING_AVOID)
			cout << "\tSideways: " << sideways_vector << endl;

		Vector3 interpolated = original_velocity.getNormalized() *        clearance_fraction +
		                       sideways_vector  .getNormalized() * (1.0 - clearance_fraction);

		if(original_velocity.isNormLessThan(desired_speed))
			desired_speed = original_velocity.getNorm();

		if(DEBUGGING_AVOID)
			cout << "\tDodging out of cylinder: " << interpolated.getCopyWithNorm(desired_speed) << endl;
		assert(invariant());
		return interpolated.getCopyWithNorm(desired_speed);
	}
}



double SteeringBehaviour :: calculateMaxSafeSpeed (double distance,
                                                   double deceleration) const
{
	assert(distance >= 0.0);
	assert(deceleration >= 0.0);
	assert(m_steering_behaviour == ARRIVE ||
	       m_steering_behaviour == ESCORT);

	//
	//  We can determine when we need to slow down using physics.
	//
	//              d    = 1/2 at^2
	//             2d/a  =      t^2
	//    +/- sqrt(2d/a) =      t
	//
	//  and
	//
	//    v   = at
	//    v/a =  t
	//
	//  Substituting into the first equation gives:
	//
	//     d   = (at)t/2
	//     d   =  v  t/2
	//    2d/v =     t
	//
	//  And substituting again gives:
	//
	//             2d/v = t
	//             2d/v = v/a
	//             2da  = v^2
	//    +/- sqrt(2da) = v
	//
	//  Speed cannot be negative, so we have:
	//
	//    v = sqrt(2da)
	//
	//  Finally, we divide by the padding factor.  We don't
	//    want to slow down too late and overshoot.
	//
	//    v = sqrt(2da) / padding
	//

	return sqrt(2.0 * distance * deceleration) / SLOW_DISTANCE_PADDING_FACTOR;
}

Vector3 SteeringBehaviour :: calculateExplorePosition (const Vector3& agent_position) const
{
	assert(m_steering_behaviour == EXPLORE);
	assert(m_desired_distance > EXPLORE_DISTANCE_NEW_POSITION);

	for(unsigned int i = 0; i < EXPLORE_POSITION_ATTEMPT_COUNT; i++) // loop also ends on valid position
	{
		double distance_min = m_desired_distance - m_desired_distance_tolerance;
		if(distance_min < 0.0)
			distance_min = 0.0;
		double distance_max = m_desired_distance + m_desired_distance_tolerance;
		assert(distance_min <= distance_max);
		double distance = random2(distance_min, distance_max);

		Vector3 new_position = agent_position + Vector3::getRandomUnitVector() * distance;
		if(new_position.isDistanceGreaterThan(agent_position, EXPLORE_DISTANCE_NEW_POSITION))
		{
			// "infinite" loop ende here
			return new_position;
		}
	}

	// fallback
	return agent_position + Vector3::getRandomUnitVector() * m_desired_distance;
}

Vector3 SteeringBehaviour :: calculateEscortPosition (const WorldInterface& world,
                                                      const PhysicsObjectId& id_target,
                                                      const Vector3& offset) const
{
	assert(m_steering_behaviour == ESCORT);
	assert(id_target != PhysicsObjectId::ID_NOTHING);

	if(!world.isAlive(id_target))
		return Vector3::ZERO;

	Vector3 target_position = world.getPosition(id_target);
	Vector3 target_forward  = world.getForward (id_target);
	Vector3 target_up       = world.getUp      (id_target);
	Vector3 target_right    = world.getRight   (id_target);

	return target_position +
	       target_forward * offset.x +
	       target_up      * offset.y +
	       target_right   * offset.z;
}

Vector3 SteeringBehaviour :: calculatePatrolSpherePosition (const Vector3& agent_position) const
{
	assert(m_steering_behaviour == PATROL_SPHERE);

	for(unsigned int i = 0; i < EXPLORE_POSITION_ATTEMPT_COUNT; i++) // loop also ends on valid position
	{
		Vector3 new_position = m_sphere_center + Vector3::getRandomUnitVector() * m_desired_distance;
		if(!isNearEnoughPatrolSpherePoint(agent_position))
		{
			// stop when we find a valid position
			return new_position;
		}
	}

	// fallback
	return m_sphere_center + Vector3::getRandomUnitVector() * m_desired_distance;
}

bool SteeringBehaviour :: isNearEnoughPatrolSpherePoint (const Vector3& position) const
{
	assert(m_steering_behaviour == PATROL_SPHERE);

	if(position == m_sphere_center)
		return false;  // otherwise, we will get an infinte loop when choosing a new position

	Vector3 relative_position = position - m_sphere_center;
	assert(!relative_position.isZero());
	Vector3 on_sphere_position = m_sphere_center + relative_position.getCopyWithNormSafe(m_desired_distance);

	return on_sphere_position.isDistanceLessThan(m_explore_position, EXPLORE_DISTANCE_NEW_POSITION);
}

bool SteeringBehaviour :: invariant () const
{
	if(m_id_agent == PhysicsObjectId::ID_NOTHING) return false;
	if(m_steering_behaviour >= COUNT) return false;
	if(m_desired_distance < 0.0) return false;
	if(m_desired_distance_tolerance < 0.0) return false;
	return true;
}


//
//  RedSteeringBehaviours.h
//
//  All through this file, you should replace "Red",
//    "Red", and "RE" with the name of your
//     fleet.
//

#ifndef RED_STEERING_BEHAVIOURS_H
#define RED_STEERING_BEHAVIOURS_H

#include "ObjLibrary/Vector3.h"

#include "PhysicsObjectId.h"

class WorldInterface;



//
//  Red
//
//  A namespace to store classes, functions, and constants for
//    the various Red fleets.
//
//  The following standard terminology is used in this file:
//    -> agent: The unit that the behaviour is applied to
//    -> target: The position or unit being seeked, fled, etc.
//
//  This namespace contains a class that implements the steering
//    behaviours.  Each class is initialized by its
//    constructor and performs its calculations in the
//    calculateDesiredVelocity and isCompleted function.  These
//    classes are designed on the assumption that the AI system
//    will be frequently creating and destroying instances as it
//    uses different steering behaviours to control the ships.
//

namespace FleetName
{
//
//  SteeringBehaviour
//
//  A class that implements steering behaviours.  There is a
//    function for each steering behaviour and a common set of
//    data fields.
//
//  Most steering behaviours are implemented wth a consistant
//    interface.  The SteeringBehaviour stores the details of
//    the most recently run behaviour, including a behaviour
//    index and any relevant parameters.  When a steering
//    behaviour is run, it is compared against the most recent
//    steering behaviour.  If it is different, any
//    initialization is run.  Otherwise, the initialization is
//    skipped.  In either case, the desired velocity for the new
//    steering behaviour is calculated.  The desired velocity
//    will never be faster than the maximum speed for the agent.
//
//  The exception to this is the avoid steering behaviour, which
//    performs obstacle avoidance.  Avoid does not replace
//    another steering behaviour, is not remembered, and
//    requires no initialized.  Given an original desired
//    velocity and an obstacle, it calculates the new velocity
//    needed to avoid a collision with that obstacle.  In most
//    cases, this will be the original velocity.  The intended
//    design is for a desired velocity to be calculated and then
//    avoid called for each nearby object to be avoided.
//
//  There are also 3 static functions associated with the class.
//    They are useful for implementing steering behaviours, but
//    may also be useful elsewhere.  Two of these are versions
//    of a function named getIntersectionTime that calculate the
//    time that it would take for an object moving at a fixed
//    speed to reach a stationary or moving target.  The other
//    function is named getAimDirection, and it calculates the
//    direction a shot of a known speed would have to be fired
//    to hit a moving target.
//
//  Class Invariant:
//    <1> m_id_agent != PhysicsObjectId::ID_NOTHING
//    <2> m_steering_behaviour < COUNT
//    <3> m_desired_distance >= EXPLORE_DISTANCE_NEW_POSITION
//    <4> m_desired_distance_tolerance >= 0.0
//

	class SteeringBehaviour
	{
	public:
	//
	//  COUNT
	//
	//  The number of steering behaviours that can be
	//    represented.  This total includes the special stop
	//    behaviour, but does not include avoid.
	//

		static const unsigned int COUNT = 10;

	//
	//  STOP
	//
	//  A special steering behaviours used as a default.  An
	//    agent using the stop behaviour will stop moving as
	//    quickly as possible and take no other actions.
	//

		static const unsigned int STOP = 0;

	//
	//  ARRIVE
	//
	//  A steering behaviour that steers to the current position
	//    of a target and stops on top of it.
	//
	//  See also SEEK, PURSUE, AIM, ESCORT.
	//

		static const unsigned int ARRIVE = 1;

	//
	//  SEEK
	//
	//  A steering behaviour that steers to the current position
	//    of a target.
	//
	//  See also PURSUE, ARRIVE, AIM, ESCORT.
	//

		static const unsigned int SEEK = 2;

	//
	//  FLEE
	//
	//  A steering behaviour that steers away from the current
	//    position of a target.
	//
	//  See also EVADE.
	//

		static const unsigned int FLEE = 3;

	//
	//  PURSUE
	//
	//  A steering behaviour that steers to intersect the
	//    predicted future position of a target.
	//
	//  See also SEEK, ARRIVE, AIM, ESCORT.
	//

		static const unsigned int PURSUE = 4;

	//
	//  EVADE
	//
	//  A steering behaviour that steers to avoid the predicted
	//    future position of a target.
	//
	//  See also FLEE.
	//

		static const unsigned int EVADE = 5;

	//
	//  AIM
	//
	//  A steering behaviour that steers so that shots fired at
	//    a fixed speed in the agent's direction will hit a
	//    target.
	//
	//  See also SEEK, ARRIVE, PURSUE, ESCORT.
	//

		static const unsigned int AIM = 6;

	//
	//  EXPLORE
	//
	//  A steering behaviour that flies to an infinite series of
	//    positions, each chosen at random near the agent.
	//
	//  Explore is a replacement for of Craig Reynolds' wander
	//    behaviour.  Explore produces more distinct turns and
	//    is not dependant on the frame rate.
	//
	//  See also PATROL_SPHERE.
	//

		static const unsigned int EXPLORE = 7;

	//
	//  ESCORT
	//
	//  A steering behaviour that steers to have a specific
	//    offset from the position of a target while matching
	//    the target's velocity.
	//
	//  The offset is specified relative to the target's local
	//    coordinate system, so an escort in front of the target
	//    always stays in front, even if the target turns.  The
	//    offset is specified as a Vector3.  The interpretation
	//    of the components is as if the target was oriented to
	//    face along the +X axis with +Y as up.  Specifically:
	//    -> The X component is in the target's forward direction
	//    -> The Y component is in the target's up direction
	//    -> The Z component is in the target's right direction
	//
	//  See also SEEK, ARRIVE, PURSUE, AIM.
	//

		static const unsigned int ESCORT = 8;

	//
	//  PATROL_SPHERE
	//
	//  A steering behaviour that flies around the surface of a
	//    sphere to an infinite series of randomly-chosen
	//    positions.
	//
	//  See also EXPLORE.
	//

		static const unsigned int PATROL_SPHERE = 9;

	//
	//  NO_INTERSECTION_POSSIBLE
	//
	//  A constant indicating that the agent cannot intersect
	//    the target.  This normally happens when the target is
	//    faster than the agent.
	//
	//  To simplify comparisions, this value is guarenteed to be
	//    negative and to be the only negative value that the
	//    getIntersectionTime functions will ever return.
	//

		static const double NO_INTERSECTION_POSSIBLE;

	//
	//  SLOW_DISTANCE_PADDING_FACTOR
	//
	//  How close much sooner than the calculated latest point
	//    that the agent should to slow down when stopping.  A
	//    larger value takes longer, but has less chance of
	//    overshooting.  Causes of overshooting include:
	//    -> Changing circumstances (e.g. the target changes its
	//                               velocity or orientation)
	//    -> The descrete nature of the simulation
	//    -> Floating point rounding errors.
	//
	//  A value of 1.0 indicates to wait until the last possible
	//    moment, always producing an overshoot.  A value of 2.0
	//    indicates that the agent should begin to slow down at
	//    twice the theoretical distance, which works in most
	//    practical cases.
	//
	//  This constant is used by the arrival and escort steering
	//    behaviours.
	//

		static const double SLOW_DISTANCE_PADDING_FACTOR;

	//
	//  EXPLORE_DISTANCE_NEW_POSITION
	//
	//  How close the agent must get to the goal position before
	//    picking a new goal position.  If this distance is too
	//    small, the agent may get stuck trying to reach the
	//    goal point.
	//
	//  This constant is used by the explore and patrol sphere
	//    steering behaviours.
	//

		static const double EXPLORE_DISTANCE_NEW_POSITION;

	public:
	//
	//  getIntersectionTime
	//
	//  Purpose: To determine how long it would take an agent
	//           moving at the specified constant speed in a
	//           constant direction to reach a stationary
	//           target.
	//  Parameter(s):
	//    <1> agent_position: The position of the agent
	//    <2> agent_speed: The maximum speed of the agent
	//    <3> target_position: The position of the target
	//  Precondition(s):
	//    <1> agent_speed >= 0.0
	//  Returns: The time it would take for the agent to reach
	//           the target at speed agent_speed.  If it
	//           possible for the agent to reach the target,
	//           this value is always non-negative.  Otherwise,
	//           NO_INTERSECTION_POSSIBLE, a negative value, is
	//           returned.
	//  Side Effect: N/A
	//

		static double getIntersectionTime (
		                        const Vector3& agent_position,
		                        double agent_speed,
		                        const Vector3& target_position);

	//
	//  getIntersectionTime
	//
	//  Purpose: To determine how long it would take an agent
	//           moving at the specified constant speed in a
	//           constant direction to reach a target moving at
	//           the specified velocity.
	//  Parameter(s):
	//    <1> agent_position: The position of the agent
	//    <2> agent_speed: The maximum speed of the agent
	//    <3> target_position: The position of the target
	//    <4> target_velocity: The velocity of the target
	//  Precondition(s):
	//    <1> agent_speed >= 0.0
	//  Returns: The time it would take for the agent to reach
	//           the target at speed agent_speed.  If it
	//           possible for the agent to reach the target,
	//           this value is always non-negative.  If there
	//           are two possible solutions, the smaller
	//           (non-negative) one is returned.  If it is not
	//           possible for the agent to reach the target,
	//           NO_INTERSECTION_POSSIBLE, a negative value, is
	//           returned.
	//  Side Effect: N/A
	//

		static double getIntersectionTime (
		                        const Vector3& agent_position,
		                        double agent_speed,
		                        const Vector3& target_position,
		                        const Vector3& target_velocity);

	//
	//  getAimDirection
	//
	//  Purpose: To determine the direction to aim a shot with
	//           the specified constant speed so that it will
	//           hit a moving target with the specified position
	//           and velocity.
	//  Parameter(s):
	//    <1> shot_start: The starting position for the shot
	//    <2> shot_speed: The speed of the shot
	//    <3> target_position: The position of the target
	//    <4> target_velocity: The velocity of the target
	//  Precondition(s):
	//    <1> shot_speed >= 0.0
	//  Returns: The direction to fire the shot so that it hits
	//           the target.  If it is not possible for the shot
	//           to hit the target, Vector3::ZERO is returned.
	//  Side Effect: N/A
	//

		static Vector3 getAimDirection (
		                        const Vector3& shot_start,
		                        double shot_speed,
		                        const Vector3& target_position,
		                        const Vector3& target_velocity);

	public:
	//
	//  Constructor
	//
	//  Purpose: To create a SteeringBehaviour to control the
	//           agent with the specified id.
	//  Parameter(s):
	//    <1> id_agent: The id for the agent
	//  Precondition(s):
	//    <1> id_agent != PhysicsObjectId::ID_NOTHING
	//  Returns: N/A
	//  Side Effect: A new SteeringBehaviour is created to
	//               control the agent with id id_agent, and is marked
	//               as performing the stop behaviour.
	//

		SteeringBehaviour (const PhysicsObjectId& id_agent);

	//
	//  Copy Constructor
	//
	//  Purpose: To create a SteeringBehaviour to control the
	//           same agent as another, and with the same
	//           internal state.
	//  Parameter(s):
	//    <1> original: The SteeringBehaviour to copy
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: A new SteeringBehaviour is created to
	//               control the same agent as original.  The
	//               new SteeringBehaviour is set to be
	//               performing the same steering behaviour with
	//               the same stored details stored.
	//

		SteeringBehaviour (const SteeringBehaviour& original);

	//
	//  Destructor
	//
	//  Purpose: To safely destroy an SteeringBehaviour without
	//           memory leaks.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: All dynamically allocated memory associated
	//               with this SteeringBehaviour is freed.
	//

		~SteeringBehaviour ();

	//
	//  Assignment Operator
	//
	//  Purpose: To modify this SteeringBehaviour to control the
	//           same agent as another, and to have the same
	//           internal state.
	//  Parameter(s):
	//    <1> original: The SteeringBehaviour to copy
	//  Precondition(s): N/A
	//  Returns: A reference to this SteeringBehaviour.
	//  Side Effect: This SteeringBehaviour is set to control
	//               the same agent as original, and to be
	//               performing the same steering behaviour with
	//               the same stored details stored.
	//

		SteeringBehaviour& operator= (
		                     const SteeringBehaviour& original);

	//
	//  stop
	//
	//  Purpose: To stop the agent.  This is the default
	//           steering behaviour.  This steering behaviour
	//           can also be used to erase the internal state
	//           of another steering behaviour.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//  Precondition(s): N/A
	//  Returns: The desired velocity for the agent.  This is
	//           always the zero vector.
	//  Side Effect: The current steering behaviour is set to
	//               stop.
	//

		Vector3 stop (const WorldInterface& world);

	//
	//  arrive
	//
	//  Purpose: To arrive at the specified target.  This means
	//           steering directly to its current position and
	//           stopping on top of it.  When considering the
	//           most recent steering behaviour performed, this
	//           is considered to be same behaviour as the
	//           version of arrive that takes a Vector3 as a
	//           parameter.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to arrive at
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If arrive was not the most recent steering
	//               behaviour performed, the arrive steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 arrive (const WorldInterface& world,
		                const PhysicsObjectId& id_target);

	//
	//  arrive
	//
	//  Purpose: To arrive at the specified position.  This
	//           means steering directly towards it and stopping
	//           on top of it.  When considering the most recent
	//           steering behaviour performed, this is considered
	//           to be the same behaviour as the version of
	//           arrive that takes a PhysicsObjectId as a
	//           parameter.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> target_position: The position to arrive at
	//  Precondition(s): N/A
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If arrive was not the most recent steering
	//               behaviour performed, the arrive steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 arrive (const WorldInterface& world,
		                const Vector3& target_position);

	//
	//  seek
	//
	//  Purpose: To seek the specified target.  This means
	//           steering directly to its current position.
	//           When considering the most recent steering
	//           behaviour performed, this is considered to be
	//           the same behaviour as the version of seek that
	//           takes a Vector3 as a parameter.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to seek
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If seek was not the most recent steering
	//               behaviour performed, the seek steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 seek (const WorldInterface& world,
		              const PhysicsObjectId& id_target);

	//
	//  seek
	//
	//  Purpose: To seek the specified target.  This means
	//           steering directly to its current position.
	//           When considering the most recent steering
	//           behaviour performed, this is considered to be
	//           the same behaviour as the version of seek that
	//           takes a PhysicsObjectId as a parameter.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> target_position: The position to seek
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If seek was not the most recent steering
	//               behaviour performed, the seek steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 seek (const WorldInterface& world,
		              const Vector3& target_position);

	//
	//  flee
	//
	//  Purpose: To flee the specified target.  This means
	//           steering directly away from its current
	//           position.  When considering the most recent
	//           steering behaviour performed, this is
	//           considered to be the same behaviour as the
	//           version of flee that takes a Vector3 as a
	//           parameter
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to flee
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If flee was not the most recent steering
	//               behaviour performed, the flee steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 flee (const WorldInterface& world,
		              const PhysicsObjectId& id_target);

	//
	//  flee
	//
	//  Purpose: To flee the specified target.  This means
	//           steering directly away from its current
	//           position.  When considering the most recent
	//           steering behaviour performed, this is
	//           considered to be the same behaviour as the
	//           version of flee that takes a PhysicsObjectId as
	//           a parameter.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> target_position: The position to flee
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If flee was not the most recent steering
	//               behaviour performed, the flee steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 flee (const WorldInterface& world,
		              const Vector3& target_position);

	//
	//  pursue
	//
	//  Purpose: To pursue the specified target.  This means
	//           steering towards its predicted future position.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to seek
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If pursue was not the most recent steering
	//               behaviour performed, the pursue steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 pursue (const WorldInterface& world,
		                const PhysicsObjectId& id_target);

	//
	//  evade
	//
	//  Purpose: To flee the specified target.  This means
	//           steering away from its predicted future
	//           position.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to flee
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If evade was not the most recent steering
	//               behaviour performed, the evade steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 evade (const WorldInterface& world,
		               const PhysicsObjectId& id_target);

	//
	//  aim
	//
	//  Purpose: To aim a shot of the specified speed at the
	//           specified target.  This means steering so that
	//           shots fired at the speed will intersect the
	//           target at a predicted future position.  The
	//           getAimDirection function may be useful when
	//           determining if the agent is successfully aimed
	//           at the target.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to flee
	//    <3> shot_speed: The speed of the shots
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//    <2> shot_speed >= 0.0
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If aim was not the most recent steering
	//               behaviour performed, the aim steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 aim (const WorldInterface& world,
		             const PhysicsObjectId& id_target,
		             double shot_speed);

	//
	//  explore
	//
	//  Purpose: To explore the nearby area.  This means
	//           repeatedly choosing a nearby position and
	//           steering towards it.  When the position is
	//           reached, a new position is chosen.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> distance_new_position_min: The minimum distance
	//                                   for explore positions
	//    <3> distance_new_position_max: The maximum distance
	//                                   for explore positions
	//  Precondition(s):
	//    <1> distance_new_position_min >= 0.0
	//    <2> distance_new_position_min <=
	//        distance_new_position_max
	//    <3> distance_new_position_min >
	//        EXPLORE_DISTANCE_NEW_POSITION
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If explore was not the most recent steering
	//               behaviour performed, the explore steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 explore (const WorldInterface& world,
		                 double distance_new_position_min,
		                 double distance_new_position_max);

	//
	//  escort
	//
	//  Purpose: To escort the specified target with the
	//           specified offset.  This means steering to match
	//           the velocity of the target while maintaining
	//           the offset from its position.  The offset is
	//           specified in the target;s local coordinate
	//           system.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to flee
	//    <3> offset: The offset from the target
	//  Precondition(s):
	//    <1> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If escort was not the most recent steering
	//               behaviour performed, the escort steering
	//               behaviour is initialized.  The internal
	//               state of the SteeringBehaviour may also be
	//               updated as internal milestones are reached.
	//

		Vector3 escort (const WorldInterface& world,
		                const PhysicsObjectId& id_target,
		                const Vector3& offset);

	//
	//  patrolSphere
	//
	//  Purpose: To patrol around the specifed sphere.  This
	//           means repeatedly choosing a position the
	//           specified distance above the surface of the
	//           sphere steering around the sphere towards it.
	//           When the position is reached, a new position is
	//           chosen.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> sphere_center: The center of the sphere
	//    <3> patrol_radius: The distance from the sphere center
	//                       to patrol at
	//    <4> patrol_radius_tolerance: How far above of below
	//                                 the patrol distance the
	//                                 agent can safely be
	//  Precondition(s):
	//    <1> patrol_radius > EXPLORE_DISTANCE_NEW_POSITION
	//    <2> patrol_radius_tolerance >= 0.0
	//  Returns: The desired velocity for the agent.
	//  Side Effect: If patrol sphere was not the most recent
	//               steering behaviour performed, the patrol
	//               sphere steering behaviour is initialized.
	//               The internal state of the SteeringBehaviour
	//               may also be updated as internal milestones
	//               are reached.
	//

		Vector3 patrolSphere (const WorldInterface& world,
		                      const Vector3& sphere_center,
		                      double patrol_radius,
		                      double patrol_radius_tolerance);

	//
	//  avoid
	//
	//  Purpose: To calculate the revised desired velocity
	//           needed to the specified original desired
	//           velocity  to avoid a collision with the
	//           specified sphere.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> original_velocity: The original desired velocity
	//    <3> sphere_center: The center of the sphere
	//    <4> sphere_radius: The radius of the sphere
	//    <5> clearance: The distance to maintain from the
	//                   sphere
	//    <6> avoid_distance: The distance from the sphere
	//                        beyond which it can be ignored
	//  Precondition(s):
	//    <1> sphere_radius >= 0.0
	//    <2> clearance > 0.0
	//    <3> clearance <= avoid_distance
	//  Returns: The revised desired velocity.  If the distance
	//           from the agent to the sphere is greater than
	//           the sum of agent's radius, sphere_radius, and
	//           avoid_distance, original_velocity is always
	//           returned.  Otherwise, the velocity needed to
	//           avoid a collision with the sphere is returned.
	//           The returned velocity will always have a norm
	//           less than or equal to the maximum speed for the
	//           agent.
	//  Side Effect: N/A
	//

		Vector3 avoid (const WorldInterface& world,
		               const Vector3& original_velocity,
		               const Vector3& sphere_center,
		               double sphere_radius,
		               double clearance,
		               double avoid_distance) const;

	private:
	//
	//  calculateMaxSafeSpeed
	//
	//  Purpose: To determine the maximum speed the agent should
	//           be moving at in order to slow down in time to
	//           stop within the specified distance.
	//  Parameter(s):
	//    <1> distance: The distance to stop in
	//    <2> deceleration: The maximum deceleration rate for
	//                      the agent
	//  Precondition(s):
	//    <1> distance >= 0.0
	//    <2> deceleration >= 0.0
	//    <3> m_steering_behaviour == ARRIVE ||
	//        m_steering_behaviour == ESCORT
	//  Returns: The maximum speed the agent should be moving at
	//           in order to stop witihn distance distance.
	//  Side Effect: N/A
	//

		double calculateMaxSafeSpeed (double distance,
		                              double deceleration) const;

	//
	//  calculateExplorePosition
	//
	//  Purpose: To calculate a new goal position for the
	//           explore steering behaviour.
	//  Parameter(s):
	//    <1> agent_position: The current position of the agent
	//  Precondition(s):
	//    <1> m_steering_behaviour == EXPLORE
	//  Returns: The next position for the explore behaviour to
	//           steer to.
	//  Side Effect: N/A
	//

		Vector3 calculateExplorePosition (
		                   const Vector3& agent_position) const;

	//
	//  calculateEscortPosition
	//
	//  Purpose: To calculate the global cooridnates for the
	//           position to steer to for the escort steering
	//           behaviour.
	//  Parameter(s):
	//    <1> world: The World the agent is in
	//    <2> id_target: The target to flee
	//    <3> offset: The offset from the target
	//  Precondition(s):
	//    <1> m_steering_behaviour == ESCORT
	//    <2> id_target != PhysicsObjectId::ID_NOTHING
	//  Returns: The escort position in global coordinates.  If
	//           id_target is not alive, the zero vector is
	//           returned.
	//  Side Effect: N/A
	//

		Vector3 calculateEscortPosition (
		                       const WorldInterface& world,
		                       const PhysicsObjectId& id_target,
		                       const Vector3& offset) const;

	//
	//  calculatePatrolSpherePosition
	//
	//  Purpose: To calculate a new goal position for the patrol
	//           sphere steering behaviour.
	//  Parameter(s):
	//    <1> agent_position: The current position of the agent
	//  Precondition(s):
	//    <1> m_steering_behaviour == PATROL_SPHERE
	//  Returns: The next position for the patrol sphere
	//           behaviour to steer to.
	//  Side Effect: N/A
	//

		Vector3 calculatePatrolSpherePosition (
		                   const Vector3& agent_position) const;

	//
	//  isNearEnoughPatrolSpherePoint
	//
	//  Purpose: To determine if the specified position is close
	//           enough to the goal position for the patrol
	//           sphere steering behaviour to generate a new
	//           goal position.  This is not a simple distance
	//           calulation.
	//  Parameter(s):
	//    <1> position: The position to test
	//  Precondition(s):
	//    <1> m_steering_behaviour == PATROL_SPHERE
	//  Returns: Whether position position is close enough to
	//           the goal position.
	//  Side Effect: N/A
	//

		bool isNearEnoughPatrolSpherePoint (
		                         const Vector3& position) const;

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
		PhysicsObjectId m_id_agent;
		unsigned int m_steering_behaviour;
		Vector3 m_explore_position;
		Vector3 m_sphere_center;
		double  m_desired_distance;
		double  m_desired_distance_tolerance;
	};



}  // end of namespace RedFleets



#endif

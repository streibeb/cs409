//
//  Orrery.h
//

#ifndef ORRERY_H
#define ORRERY_H

#include <vector>

#include "ObjLibrary/Vector3.h"



//
//  Orrery
//
//  A class to represent and display the complex calculations
//    for the missile target position.
//
//  The missile position moves around the origin according to a
//    system of epicycles.  This, although scientifically
//    incorrect, is the longest-lasting theory of planatary
//    motion.  Notable systems of this type were designed by
//    Ptolemy of Thebaid, Copernicus (whose system was adopted
//    and promoted by Galileo), and Tycho Brahe.
//
//  This class does not follow any of these systems.  Instead,
//    it simply uses epicycles to move a single position along a
//    semi-predictable path.  The position is question is
//    intended for use as a homing missile target.
//
//  One benefit of an epicycle-based system over a gravity-based
//    ones is that the position of the end point can be
//    calculated independantly for any point in time.
//
//  Class Invariant:
//    <1> m_target_position == calculateTargetPosition(m_time)
//    <2> mv_epicycle[i].invariant()
//                            WHERE 0 <= i < mv_epicycles.size()
//

class Orrery
{
public:
//
//  Class Function: isModelsInitialized
//
//  Purpose: To determine of the models for the Orrery class
//           have been initialized.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the models have been initialized.
//  Side Effect: N/A
//

	static bool isModelsInitialized ();

//
//  Class Function: initModels
//
//  Purpose: To initialize the models for the Orrery class.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> !isModelsInitialized()
//  Returns: N/A
//  Side Effect: The models for the Orrery class are
//               initialized.  These models are shared between
//               all instances of the class, so this function
//               only needs to be called once.
//

	static void initModels ();

public:
//
//  Default Constructor
//
//  Purpose: To create a new Orrery with a default time value.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Orrery is created.  The time value is set
//               to 0.0, placing target position is at its
//               default position.
//

	Orrery ();

//
//  Constructor
//
//  Purpose: To create a new Orrery with the specified time
//           value.
//  Parameter(s):
//    <1> time: The time value
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Orrery is created with a time value of
//               time.  The target position is calculated based
//               on time.
//

	Orrery (double time);

//
//  Copy Constructor
//
//  Purpose: To create a new Orrery as a copy of another.
//  Parameter(s):
//    <1> original: The Orrery to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Orrery is created with the same time and
//               thus the same target position as original.
//

	Orrery (const Orrery& original);

//
//  Destructor
//
//  Purpose: To safely destroy an Orrery withoout memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this Orrery is freed.
//

	~Orrery ();

//
//  Assignment Operator
//
//  Purpose: To modify this Orrery to be a copy of another.
//  Parameter(s):
//    <1> original: The Orrery to copy
//  Precondition(s): N/A
//  Returns: A reference to this Orrery.
//  Side Effect: This Orrery is set to have the same time and
//               thus the same target position as original.
//

	Orrery& operator= (const Orrery& original);

//
//  getTime
//
//  Purpose: To determine the time value for this Orrery.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The time value.
//  Side Effect: N/A
//

	double getTime () const;

//
//  getTargetPosition
//
//  Purpose: To determine the target position for this Orrery.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The target position.  This is determined by the
//           time value.
//  Side Effect: N/A
//

	const Vector3& getTargetPosition () const;

//
//  getTargetVelocity
//
//  Purpose: To determine the velocity that the target position
//           for this Orrery is moving.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The instantaneous velocity that the target position
//           is moving at.  This value may be slightly off
//           depending on the shape of the curve.
//  Side Effect: N/A
//

	Vector3 getTargetVelocity () const;

//
//  draw
//
//  Purpose: To display the epicycles for this Orrery.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isModelsInitialized()
//  Returns: N/A
//  Side Effect: The epicycles for this Orrery are drawn in
//               their current positions.
//

	void draw () const;

//
//  addEpicycle
//
//  Purpose: To add an epicycle to this Orrery.
//  Parameter(s):
//    <1> axis: The axis the epicycle rotates around
//    <2> radius: The radius of the epicycle
//    <3> rotation_rate_radians: The rate the epicycle rotates
//                               in radians per second
//    <4> colour: The colour to display the epicycle in
//  Precondition(s):
//    <1> axis.isNormal()
//    <2> radius >= 0.0
//    <3> rotation_rate_radians   >= 0.0
//    <4> colour.isAllComponentsGreaterThanOrEqual(0.0)
//    <5> colour.isAllComponentsLessThanOrEqual(1.0)
//  Returns: N/A
//  Side Effect: An epicycle is added to this Orrery with an
//               axis of axis and a radius of radius.  The new
//               epicycle rotates at a rate of
//               rotation_rate_radians radians per second.  The
//               components of colour are used to display it.
//

	void addEpicycle (const Vector3& axis,
	                  double radius,
	                  double rotation_rate_radians,
	                  const Vector3& colour);

//
//  removeAllEpicycles
//
//  Purpose: To remove all epicycles from this Orrery.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All epicycles in this Orrery are rmeoved.
//

	void removeAllEpicycles ();

//
//  setTime
//
//  Purpose: To change the time value for this Orrery to the
//           specified value.
//  Parameter(s):
//    <1> time: The new time value
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Orrery is set to have a time value of
//               time.  The target position is updated.
//

	void setTime (double time);

//
//  addTime
//
//  Purpose: To increase the time value for this Orrery by the
//           specified amount.
//  Parameter(s):
//    <1> increase: The amount to increase time value by
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The time value for this Orrery is increased by
//               increase.  The target position is updated.
//               Note that a negative value for increase will
//               decrease the time value.
//

	void addTime (double increase);

private:
//
//  calculateTargetPosition
//
//  Purpose: To calculate the target position for the specified
//           time value.
//  Parameter(s):
//    <1> time: The time to calculate the target position for
//  Precondition(s): N/A
//  Returns: The target position at time time.
//  Side Effect: N/A
//

	Vector3 calculateTargetPosition (double time) const;

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
	//
	//  Epicycle
	//
	//  A record to encapsulate information about a single
	//    epicycle.
	//
	//  Struct Invariant (enforced by Orrery class):
	//    <1> m_axis.isNormal()
	//    <2> m_radius >= 0.0
	//    <3> m_initial_direction.isNormal()
	//    <4> m_initial_direction.isOrthogonal(m_axis)
	//    <5> m_rotation_rate_radians >= 0.0
	//    <6> m_colour.isAllComponentsGreaterThanOrEqual(0.0)
	//    <7> m_colour.isAllComponentsLessThanOrEqual(1.0)
	//

	struct Epicycle
	{
		Vector3 m_axis;
		double  m_radius;
		Vector3 m_initial_direction;
		double  m_rotation_rate_radians;  // radians per second
		Vector3 m_colour;

		Epicycle (const Vector3& axis,
		          double radius,
		          const Vector3& initial_direction,
		          double rotation_rate_radians,
		          const Vector3& colour);

		bool invariant () const;
	};

private:
	std::vector<Epicycle> mv_epicycles;
	double m_time;
	Vector3 m_target_position;
};



#endif

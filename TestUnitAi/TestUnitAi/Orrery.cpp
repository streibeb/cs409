//
//  Orrery.cpp
//

#include <cassert>
#include <cmath>
#include <iostream>
#include "GetGlut.h"

#include "Pi.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/DisplayList.h"

#include "Orrery.h"

using namespace std;
namespace
{
	const double TIME_DEFAULT = 0.0;
	const double TIME_OFFSET_FOR_VELOCITY = 0.001;

	bool g_is_models_initialized = false;

	const unsigned int RING_SPOKE_COUNT = 6;
	const unsigned int RING_POINT_COUNT = RING_SPOKE_COUNT * 10;
	const double RING_HUB_SCALING = 50.0;
	const Vector3 RING_MODEL_AXIS(0.0, 1.0, 0.0);
	DisplayList g_ring_list;
	DisplayList g_hub_list;



	//
	//  calculateRotationAxis
	//
	//  Purpose: To calculate the axis to rotate around to get
	//           from one direction to another.
	//  Parameter(s):
	//    <1> start: The starting direction
	//    <2> end: The desired direction
	//  Precondition(s):
	//    <1> start.isNormal()
	//    <2> end.isNormal()
	//  Returns: The single axis to rotate start around to
	//           produce end.  Of the 2 possible axes, the one
	//           requiring the smaller positive rotation is
	//           returned.  If the required rotation is the same
	//           in both cases, it is undefined which axis is
	//           returned.
	//  Side Effect: N/A
	//

	Vector3 calculateRotationAxis (const Vector3& start,
	                               const Vector3& end)
	{
		assert(start.isNormal());
		assert(end.isNormal());

		// calculate a Vector3 at right angles to axis
		Vector3 axis = start.crossProduct(end);
		if(axis.isZero())
		{
			axis = start.crossProduct(Vector3(1.0, 0.0, 0.0));
			if(axis.isZero())
				axis = Vector3(0.0, 1.0, 0.0);
			else
				axis.normalize();
		}
		else
			axis.normalize();

		assert(axis.isNormal());
		assert(axis.isOrthogonal(start));
		assert(axis.isOrthogonal(end));
		return axis;
	}

	//
	//  calculateRotationAngle
	//
	//  Purpose: To calculate the angle to rotate by to get from
	//           one direction to another.  This function
	//           assumes that the rotation will be around a
	//           single axis, and that the single axis requiring
	//           a smaller positive rotation will be used.
	//  Parameter(s):
	//    <1> start: The starting direction
	//    <2> end: The desired direction
	//  Precondition(s):
	//    <1> start.isNormal()
	//    <2> end.isNormal()
	//  Returns: The angle of rotation in radians.
	//  Side Effect: N/A
	//

	double calculateRotationAngle (const Vector3& start,
	                               const Vector3& end)
	{
		assert(start.isNormal());
		assert(end.isNormal());

		return start.getAngleNormal(end);
	}



	const bool DEBUGGING_TARGET_POSITION = false;
	const bool DEBUGGING_TARGET_VELOCITY = false;
	const bool DEBUGGING_RING_ALIGNMENT  = false;
	const double DEBUGGING_RING_AXES_SIZE = 1000.0;
}



bool Orrery :: isModelsInitialized ()
{
	return g_is_models_initialized;
}

void Orrery :: initModels ()
{
	assert(!isModelsInitialized());

	assert(RING_MODEL_AXIS == Vector3::UNIT_Y_PLUS);

	g_ring_list.begin();
		// rim
		glBegin(GL_LINE_LOOP);
			for(unsigned int i = 0; i < RING_POINT_COUNT; i++)
			{
				double radians = i * TWO_PI / RING_POINT_COUNT;
				glVertex3d(cos(radians), 0.0, sin(radians));
			}
		glEnd();

		// spokes
		glBegin(GL_LINES);
			for(unsigned int i = 0; i < RING_SPOKE_COUNT; i++)
			{
				double radians = i * TWO_PI / RING_SPOKE_COUNT;
				double x_raw = cos(radians);
				double z_raw = sin(radians);

				glVertex3d(0.0, 0.0, 0.0);
				if(i != 0)
				{
					glVertex3d(x_raw * 0.2, 0.0, z_raw * 0.2);
					glVertex3d(x_raw * 0.4, 0.0, z_raw * 0.4);
					glVertex3d(x_raw * 0.6, 0.0, z_raw * 0.6);
					glVertex3d(x_raw * 0.8, 0.0, z_raw * 0.8);
				}
				glVertex3d(x_raw, 0.0, z_raw);
			}
		glEnd();

		// hub is drawn seperately (so scaling can be independant)
	g_ring_list.end();

	g_hub_list.begin();
		glutWireSphere(1.0, 6, 4);
	g_hub_list.end();

	g_is_models_initialized = true;

	assert(isModelsInitialized());
}



Orrery :: Orrery ()
		: m_time(TIME_DEFAULT),
		  m_target_position(calculateTargetPosition(m_time))
{
	assert(invariant());
}

Orrery :: Orrery (double time)
		: m_time(time),
		  m_target_position(calculateTargetPosition(m_time))
{
	assert(invariant());
}

Orrery :: Orrery (const Orrery& original)
		: m_time(original.m_time),
		  m_target_position(original.m_target_position)
{
	assert(invariant());
}

Orrery :: ~Orrery ()
{
}

Orrery& Orrery :: operator= (const Orrery& original)
{
	if(&original != this)
	{
		m_time            = original.m_time;
		m_target_position = original.m_target_position;
	}

	assert(invariant());
	return *this;
}



double Orrery :: getTime () const
{
	return m_time;
}

const Vector3& Orrery :: getTargetPosition () const
{
	return m_target_position;
}

Vector3 Orrery :: getTargetVelocity () const
{
	Vector3 position_past   = calculateTargetPosition(m_time - TIME_OFFSET_FOR_VELOCITY);
	Vector3 position_future = calculateTargetPosition(m_time + TIME_OFFSET_FOR_VELOCITY);
	Vector3 target_velocity = (position_future - position_past) / (TIME_OFFSET_FOR_VELOCITY * 2.0);

	if(DEBUGGING_TARGET_VELOCITY)
	{
		cout << "Calculating target velocity:" << endl;
		cout << "\tPast position:   " << position_past   << endl;
		cout << "\tFuture position: " << position_future << endl;
		cout << "\tVelocity:        " << target_velocity << endl;
	}

	return target_velocity;
}

void Orrery :: draw () const
{
	assert(isModelsInitialized());

	Vector3 position;
	for(unsigned int i = 0; i < mv_epicycles.size(); i++)
	{
		double         epicycle_radians   = mv_epicycles[i].m_rotation_rate_radians * m_time;
		double         epicycle_degrees   = radiansToDegrees(epicycle_radians);
		const Vector3& epicycle_axis      = mv_epicycles[i].m_axis;
		const Vector3& epicycle_initial   = mv_epicycles[i].m_initial_direction;
		Vector3        epicycle_direction = epicycle_initial.getRotatedArbitrary(epicycle_axis, epicycle_radians);
		double         epicycle_radius    = mv_epicycles[i].m_radius;
		const Vector3& epicycle_colour    = mv_epicycles[i].m_colour;

		Vector3 alignment_axis    = calculateRotationAxis (RING_MODEL_AXIS, epicycle_axis);
		double  alignment_radians = calculateRotationAngle(RING_MODEL_AXIS, epicycle_axis);
		double  alignment_degrees = radiansToDegrees(alignment_radians);

		glPushMatrix();
			glTranslated(position.x, position.y, position.z);
			glRotated(epicycle_degrees, epicycle_axis.x, epicycle_axis.y, epicycle_axis.z);
			glRotated(alignment_degrees, alignment_axis.x, alignment_axis.y, alignment_axis.z);

			glColor3d(epicycle_colour.x, epicycle_colour.y, epicycle_colour.z);
			glPushMatrix();
				glScaled(RING_HUB_SCALING, RING_HUB_SCALING, RING_HUB_SCALING);
				g_hub_list.draw();
			glPopMatrix();
			glPushMatrix();
				glScaled(epicycle_radius, epicycle_radius, epicycle_radius);
				g_ring_list.draw();
			glPopMatrix();
		glPopMatrix();

		if(DEBUGGING_RING_ALIGNMENT)
		{
			glLineWidth(2.0);

			glPushMatrix();
				glTranslated(position.x, position.y, position.z);
				glScaled(DEBUGGING_RING_AXES_SIZE, DEBUGGING_RING_AXES_SIZE, DEBUGGING_RING_AXES_SIZE);

				glBegin(GL_LINES);
					glColor3d(1.0, 0.0, 0.0);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(epicycle_axis.x, epicycle_axis.y, epicycle_axis.z);
					glColor3d(0.0, 1.0, 0.0);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(epicycle_initial.x, epicycle_initial.y, epicycle_initial.z);
					glColor3d(0.0, 0.0, 1.0);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(epicycle_direction.x, epicycle_direction.y, epicycle_direction.z);
				glEnd();
			glPopMatrix();

			glLineWidth(1.0);
		}

		position += epicycle_direction * epicycle_radius;
	}
}



void Orrery :: addEpicycle (const Vector3& axis,
                            double radius,
                            double rotation_rate_radians,
                            const Vector3& colour)
{
	assert(!axis.isZero());
	assert(radius >= 0.0);
	assert(rotation_rate_radians >= 0.0);
	assert(colour.isAllComponentsGreaterThanOrEqual(0.0));
	assert(colour.isAllComponentsLessThanOrEqual(1.0));

	// any rotation axis is orthogonal to the epicycle axis
	Vector3 initial_direction = calculateRotationAxis(axis, Vector3(0.0, 0.0, 1.0));
	assert(initial_direction.isOrthogonal(axis));

	mv_epicycles.push_back(Epicycle(axis, radius, initial_direction, rotation_rate_radians, colour));
	m_target_position = calculateTargetPosition(m_time);

	assert(invariant());
}

void Orrery :: removeAllEpicycles ()
{
	mv_epicycles.clear();
	m_target_position.setZero();

	assert(invariant());
}

void Orrery :: setTime (double time)
{
	if(time != m_time)
	{
		m_time = time;
		m_target_position = calculateTargetPosition(m_time);
	}

	assert(invariant());
}

void Orrery :: addTime (double increase)
{
	if(increase != 0.0)
	{
		m_time += increase;
		m_target_position = calculateTargetPosition(m_time);
	}

	assert(invariant());
}



Vector3 Orrery :: calculateTargetPosition (double time) const
{
	if(DEBUGGING_TARGET_POSITION)
		cout << "Calculating target position for " << mv_epicycles.size() << " epicycles" << endl;

	Vector3 position;
	for(unsigned int i = 0; i < mv_epicycles.size(); i++)
	{
		double         epicycle_radians   = mv_epicycles[i].m_rotation_rate_radians * time;
		const Vector3& epicycle_axis      = mv_epicycles[i].m_axis;
		const Vector3& epicycle_initial   = mv_epicycles[i].m_initial_direction;
		Vector3        epicycle_direction = epicycle_initial.getRotatedArbitrary(epicycle_axis, epicycle_radians);
		Vector3        epicycle_offset    = epicycle_direction * mv_epicycles[i].m_radius;
		position += epicycle_offset;

		if(DEBUGGING_TARGET_POSITION)
		{
			cout << "\t" << i << ":" << endl;
			cout << "\t\tradians:\t"   << epicycle_radians << endl;
			cout << "\t\taxis:   \t"   << epicycle_axis << endl;
			cout << "\t\tinitial:\t"   << epicycle_initial << endl;
			cout << "\t\tdirection:\t" << epicycle_direction << endl;
			cout << "\t\tradius: \t"   << mv_epicycles[i].m_radius << endl;
			cout << "\t\toffset: \t"   << epicycle_offset << endl;
			cout << "\t\tposition:" << position << endl;
		}
	}
	return position;
}

bool Orrery :: invariant () const
{
	if(m_target_position != calculateTargetPosition(m_time)) return false;

	for(unsigned int i = 0; i < mv_epicycles.size(); i++)
		if(!mv_epicycles[i].invariant())
			return false;

	return true;
}




Orrery :: Epicycle :: Epicycle (const Vector3& axis,
                                double radius,
                                const Vector3& initial_direction,
                                double rotation_rate_radians,
                                const Vector3& colour)
		: m_axis(axis),
		  m_radius(radius),
		  m_initial_direction(initial_direction),
		  m_rotation_rate_radians(rotation_rate_radians),
		  m_colour(colour)
{
	assert(invariant());
}

bool Orrery :: Epicycle :: invariant () const
{
	if(!m_axis.isNormal()) return false;
	if(m_radius < 0.0) return false;
	if(!m_initial_direction.isNormal()) return false;
	if(!m_initial_direction.isOrthogonal(m_axis)) return false;
	if(m_rotation_rate_radians < 0.0) return false;
	if(!m_colour.isAllComponentsGreaterThanOrEqual(0.0)) return false;
	if(!m_colour.isAllComponentsLessThanOrEqual(1.0)) return false;
	return true;
}

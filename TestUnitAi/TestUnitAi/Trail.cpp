//
//  Trail.h
//

#include <cassert>
#include <vector>
#include "GetGlut.h"

#include "ObjLibrary/Vector3.h"

#include "Trail.h"

using namespace std;



const Vector3 Trail :: COLOUR_DEFAULT(1.0, 1.0, 1.0);



Trail :: Trail ()
		: mv_points(POINT_COUNT_DEFAULT, Vector3::ZERO),
		  m_next_point(0),
		  m_colour(COLOUR_DEFAULT)
{
	assert(invariant());
}

Trail :: Trail (unsigned int point_count,
                const Vector3& colour)
		: mv_points(point_count, Vector3::ZERO),
		  m_next_point(0),
		  m_colour(colour)
{
	assert(point_count >= 1);
	assert(colour.isAllComponentsGreaterThanOrEqual(0.0));
	assert(colour.isAllComponentsLessThanOrEqual(1.0));

	assert(invariant());
}

Trail :: Trail (unsigned int point_count,
                const Vector3& position,
                const Vector3& colour)
		: mv_points(point_count, position),
		  m_next_point(0),
		  m_colour(colour)
{
	assert(point_count >= 1);
	assert(colour.isAllComponentsGreaterThanOrEqual(0.0));
	assert(colour.isAllComponentsLessThanOrEqual(1.0));

	assert(invariant());
}

Trail :: Trail (const Trail& original)
		: mv_points(original.mv_points),
		  m_next_point(original.m_next_point),
		  m_colour(original.m_colour)
{
	assert(invariant());
}

Trail :: ~Trail ()
{
}

Trail& Trail :: operator= (const Trail& original)
{
	if(&original != this)
	{
		mv_points    = original.mv_points;
		m_next_point = original.m_next_point;
		m_colour     = original.m_colour;
	}

	assert(invariant());
	return *this;
}



unsigned int Trail :: getPointCount () const
{
	return mv_points.size();
}

const Vector3& Trail :: getColour () const
{
	return m_colour;
}

void Trail :: draw () const
{
	glBegin(GL_LINE_STRIP);
		unsigned int point_count = mv_points.size();
		for(unsigned int i = 0; i < point_count; i++)
		{
			double fraction = (double)(i) / point_count;
			double colour_factor = sqrt(fraction);
			glColor3d(m_colour.x * colour_factor,
			          m_colour.y * colour_factor,
			          m_colour.z * colour_factor);

			unsigned int point_index = (m_next_point + i) % point_count;
			const Vector3& point = mv_points[point_index];
			glVertex3d(point.x, point.y, point.z);
		}
	glEnd();
}



void Trail :: setPointCount (unsigned int point_count)
{
	assert(point_count >= 1);

	mv_points.resize(point_count, Vector3::ZERO);

	assert(invariant());
}

void Trail :: setPointCount (unsigned int point_count,
                             const Vector3& position)
{
	assert(point_count >= 1);

	mv_points.resize(point_count, position);

	assert(invariant());
}

void Trail :: addPoint (const Vector3& position)
{
	mv_points[m_next_point] = position;
	m_next_point = (m_next_point + 1) % mv_points.size();

	assert(invariant());
}

void Trail :: setAllPointsToPosition (const Vector3& position)
{
	unsigned int point_count = mv_points.size();
	for(unsigned int i = 0; i < point_count; i++)
		mv_points[i] = position;
	m_next_point = 0;

	assert(invariant());
}

void Trail :: setColour (const Vector3& colour)
{
	assert(colour.isAllComponentsGreaterThanOrEqual(0.0));
	assert(colour.isAllComponentsLessThanOrEqual(1.0));

	m_colour = colour;

	assert(invariant());
}



bool Trail :: invariant () const
{
	if(mv_points.size() < 1) return false;
	if(m_next_point >= mv_points.size()) return false;
	if(!m_colour.isAllComponentsGreaterThanOrEqual(0.0)) return false;
	if(!m_colour.isAllComponentsLessThanOrEqual(1.0)) return false;
	return true;
}



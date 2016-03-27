//
//  SimpleMarker.h
//

#include <cassert>
#include "GetGlut.h"

#include "ObjLibrary/Vector3.h"

#include "TimeSystem.h"
#include "SimpleMarker.h"

namespace
{
	const double LONG_AGO = -1.0e20;

	const bool IS_BLUE_DEFAULT = false;
}



const double SimpleMarker :: LIFESPAN =   4.0;
const double SimpleMarker :: RADIUS   = 100.0;

DisplayList SimpleMarker :: ms_model;



bool SimpleMarker :: isModelInitialized ()
{
	assert(!ms_model.isPartial());
	return ms_model.isReady();
}

void SimpleMarker :: initModel ()
{
	assert(!isModelInitialized());

	static const double PHI      = 1.6180339887498948482;  // the golden ratio
	static const double PHI_INV  = 1.0 / PHI;
	static const double PHI_INV2 = 1.0 / (PHI * PHI);

	ms_model.begin();
		//
		//  These lines correspond to the opposite vertexes of a
		//    regular dodecahedron, and should give our marker a
		//    radiant appearance.  The colour will vary and is
		//    set in the draw function.
		//
		//  https://en.wikipedia.org/wiki/Regular_dodecahedron#Cartesian_coordinates
		//  All terms divided by PHI
		//

		glBegin(GL_LINES);
			glVertex3d( PHI_INV,  PHI_INV,  PHI_INV);
			glVertex3d(-PHI_INV, -PHI_INV, -PHI_INV);
			glVertex3d( PHI_INV,  PHI_INV, -PHI_INV);
			glVertex3d(-PHI_INV, -PHI_INV,  PHI_INV);
			glVertex3d( PHI_INV, -PHI_INV,  PHI_INV);
			glVertex3d(-PHI_INV,  PHI_INV, -PHI_INV);
			glVertex3d( PHI_INV, -PHI_INV, -PHI_INV);
			glVertex3d(-PHI_INV,  PHI_INV,  PHI_INV);

			glVertex3d( 1.0,      PHI_INV2,  0.0);
			glVertex3d(-1.0,     -PHI_INV2,  0.0);
			glVertex3d(-1.0,      PHI_INV2,  0.0);
			glVertex3d( 1.0,     -PHI_INV2,  0.0);

			glVertex3d( 0.0,      1.0,      PHI_INV2);
			glVertex3d( 0.0,     -1.0,     -PHI_INV2);
			glVertex3d( 0.0,     -1.0,      PHI_INV2);
			glVertex3d( 0.0,      1.0,     -PHI_INV2);

			glVertex3d( PHI_INV2,  0.0,      1.0);
			glVertex3d(-PHI_INV2,  0.0,     -1.0);
			glVertex3d( PHI_INV2,  0.0,     -1.0);
			glVertex3d(-PHI_INV2,  0.0,      1.0);
		glEnd();
	ms_model.end();

	assert(isModelInitialized());
}



SimpleMarker :: SimpleMarker ()
		: m_position(),
		  m_is_blue(IS_BLUE_DEFAULT),
		  m_creation_time(LONG_AGO)
{
}

SimpleMarker :: SimpleMarker (const Vector3& position, bool is_blue)
		: m_position(position),
		  m_is_blue(is_blue),
		  m_creation_time(TimeSystem::getFrameStartTime())
{
}

SimpleMarker :: SimpleMarker (const SimpleMarker& original)
		: m_position(original.m_position),
		  m_is_blue(original.m_is_blue),
		  m_creation_time(original.m_creation_time)
{
}

SimpleMarker :: ~SimpleMarker ()
{
}

SimpleMarker& SimpleMarker :: operator= (const SimpleMarker& original)
{
	if(&original != this)
	{
		m_position      = original.m_position;
		m_is_blue       = original.m_is_blue;
		m_creation_time = original.m_creation_time;
	}
	return *this;
}



const Vector3& SimpleMarker :: getPosition () const
{
	return m_position;
}

bool SimpleMarker :: isAlive () const
{
	if(m_creation_time + LIFESPAN > TimeSystem::getFrameStartTime())
		return true;
	else
		return false;
}

void SimpleMarker :: draw () const
{
	assert(isModelInitialized());

	if(isAlive())
	{
		double lifespan_remaining = m_creation_time + LIFESPAN - TimeSystem::getFrameStartTime();
		assert(lifespan_remaining >= 0.0);
		assert(lifespan_remaining <= LIFESPAN);
		double lifespan_elapased_fraction = 1.0 - lifespan_remaining / LIFESPAN;
		assert(lifespan_elapased_fraction >= 0.0);
		assert(lifespan_elapased_fraction <= 1.0);

		// colours are clamped to range [0, 1] automatically
		double colour1 = 1.0 -  lifespan_elapased_fraction        / 0.2;
		double colour2 = 1.0 - (lifespan_elapased_fraction - 0.2) / 0.3;
		double colour3 = 1.0 - (lifespan_elapased_fraction - 0.5) / 0.5;
		if(m_is_blue)
		{
			// fade from white to yellow to red to transparent black
			glColor4d(colour1, colour2, colour3, colour3);
		}
		else
		{
			// fade from white to yellow to red to transparent black
			glColor4d(colour3, colour2, colour1, colour3);
		}

		glPushMatrix();
			glTranslated(m_position.x, m_position.y, m_position.z);
			glScaled(RADIUS, RADIUS, RADIUS);
			ms_model.draw();
		glPopMatrix();
	}
}



void SimpleMarker :: init (const Vector3& position, bool is_blue)
{
	m_position      = position;
	m_is_blue       = is_blue;
	m_creation_time = TimeSystem::getFrameStartTime();
}

void SimpleMarker :: markDead ()
{
	m_creation_time = LONG_AGO;
}


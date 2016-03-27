//
//  CoordinateSystem.cpp
//

#include "GetGlut.h"

#include "Pi.h"
#include "../../ObjLibrary/Vector3.h"

#include "CoordinateSystem.h"

//
//  There has got to be a better way to ensure that the default
//    value constants are initialized correctly.  For now,
//    however, preprocessor directives happen before anything
//    else.
//

#define POSITION_DEFAULT_MACRO Vector3( 0.0,  0.0,  0.0)
#define FORWARD_DEFAULT_MACRO  Vector3( 0.0,  0.0, -1.0)
#define UP_DEFAULT_MACRO       Vector3( 0.0,  1.0,  0.0)

const Vector3 CoordinateSystem :: POSITION_DEFAULT = POSITION_DEFAULT_MACRO;
const Vector3 CoordinateSystem :: FORWARD_DEFAULT  = FORWARD_DEFAULT_MACRO;
const Vector3 CoordinateSystem :: UP_DEFAULT       = UP_DEFAULT_MACRO;
const Vector3 CoordinateSystem :: RIGHT_DEFAULT    = Vector3( 1.0,  0.0,  0.0);



CoordinateSystem :: CoordinateSystem ()
		: m_position(POSITION_DEFAULT_MACRO),
		  m_forward(FORWARD_DEFAULT_MACRO),
		  m_up     (UP_DEFAULT_MACRO)
{
	assert(invariant());
}

CoordinateSystem :: CoordinateSystem (const Vector3& position)
		: m_position(position),
		  m_forward(FORWARD_DEFAULT_MACRO),
		  m_up     (UP_DEFAULT_MACRO)
{
	assert(invariant());
}

CoordinateSystem :: CoordinateSystem (const Vector3& position,
	                                  const Vector3& forward)
		: m_position(position),
		  m_forward(forward),
		  m_up     (calculateUp(forward))
{
	assert(forward.isNormal());

	assert(invariant());
}

CoordinateSystem :: CoordinateSystem (const Vector3& position,
	                                  const Vector3& forward,
	                                  const Vector3& up)
		: m_position(position),
		  m_forward(forward),
		  m_up     (up)
{
	assert(forward.isNormal());
	assert(up.isNormal());
	assert(forward.isOrthogonal(up));

	assert(invariant());
}


const Vector3& CoordinateSystem :: getPosition () const
{
	return m_position;
}

const Vector3& CoordinateSystem :: getForward () const
{
	return m_forward;
}

const Vector3& CoordinateSystem :: getUp () const
{
	return m_up;
}

Vector3 CoordinateSystem :: getRight () const
{
	return m_forward.crossProduct(m_up);
}

void CoordinateSystem :: setupCamera () const
{
	Vector3 look_at = m_position + m_forward;
	gluLookAt(m_position.x, m_position.y, m_position.z,
	          look_at.x,    look_at.y,    look_at.z,
	          m_up.x,       m_up.y,       m_up.z);
}

void CoordinateSystem :: setupOrientationMatrix () const
{
	//
	//  OpenGL's opinions on matrix specification
	//
	//  C[0]  C[4]  C[8]  C[12]     M[0]  M[4]  M[8]  M[12]     v[0]
	//  C[1]  C[5]  C[9]  C[13]     M[1]  M[5]  M[9]  M[13]     v[1]
	//  C[2]  C[6]  C[10] C[14]  x  M[2]  M[6]  M[10] M[14]  x  v[2]
	//  C[3]  C[7]  C[11] C[15]     M[3]  M[7]  M[11] M[15]     v[3]
	//
	//  C is the existing transforation matrix
	//  M is the new matrix
	//  v is the vector
	//
	//  https://www.opengl.org/sdk/docs/man2/xhtml/glMultMatrix.xml
	//

	Vector3 right = getRight();

	double a_matrix[16] =
	{   right.x,     right.y,     right.z,     0.0,
	    m_forward.x, m_forward.y, m_forward.z, 0.0,
	    m_up.x,      m_up.y,      m_up.z,      0.0,
	    0.0,         0.0,         0.0,         1.0,  };

	glMultMatrixd(a_matrix);
}



void CoordinateSystem :: init ()
{
	m_position = POSITION_DEFAULT_MACRO;
	m_forward  = FORWARD_DEFAULT_MACRO;
	m_up       = UP_DEFAULT_MACRO;

	assert(invariant());
}

void CoordinateSystem :: init (const Vector3& position)
{
	m_position = position;
	m_forward  = FORWARD_DEFAULT_MACRO;
	m_up       = UP_DEFAULT_MACRO;

	assert(invariant());
}

void CoordinateSystem :: init (const Vector3& position,
	                           const Vector3& forward)
{
	assert(forward.isNormal());

	m_position = position;
	m_forward  = forward;
	m_up       = calculateUp(forward);

	assert(invariant());
}

void CoordinateSystem :: init (const Vector3& position,
	                           const Vector3& forward,
	                           const Vector3& up)
{
	assert(forward.isNormal());
	assert(up.isNormal());
	assert(forward.isOrthogonal(up));

	m_position = position;
	m_forward  = forward;
	m_up       = up;

	assert(invariant());
}

void CoordinateSystem :: setPosition (const Vector3& position)
{
	m_position = position;

	assert(invariant());
}

void CoordinateSystem :: addPosition (const Vector3& increase)
{
	m_position += increase;

	assert(invariant());
}

void CoordinateSystem :: setOrientation (const Vector3& forward)
{
	assert(forward.isNormal());

	m_forward  = forward;
	m_up       = calculateUp(forward);

	assert(invariant());
}

void CoordinateSystem :: setOrientation (const Vector3& forward,
                                         const Vector3& up)
{
	assert(forward.isNormal());
	assert(up.isNormal());
	assert(forward.isOrthogonal(up));

	m_forward  = forward;
	m_up       = up;

	assert(invariant());
}

void CoordinateSystem :: setUp (const Vector3& up)
{
	assert(up.isNormal());
	assert(getForward().isOrthogonal(up));

	m_up = up;

	assert(invariant());
}

void CoordinateSystem :: moveForward (double distance)
{
	m_position += m_forward * distance;

	assert(invariant());
}

void CoordinateSystem :: moveUp (double distance)
{
	m_position += m_up * distance;

	assert(invariant());
}

void CoordinateSystem :: moveRight (double distance)
{
	m_position += m_forward.crossProduct(m_up) * distance;

	assert(invariant());
}

void CoordinateSystem :: rotateAroundForward (double radians)
{
	assert(m_forward.isNormal());
	m_up.rotateArbitrary(m_forward, radians);
	m_up.normalize();

	assert(invariant());
}

void CoordinateSystem :: rotateAroundUp (double radians)
{
	assert(m_up.isNormal());
	m_forward.rotateArbitrary(m_up, radians);
	m_forward.normalize();

	assert(invariant());
}

void CoordinateSystem :: rotateAroundRight (double radians)
{
	Vector3 right = m_forward.crossProduct(m_up);
	assert(right.isNormal());
	m_forward.rotateArbitrary(right, radians);
	m_up     .rotateArbitrary(right, radians);
	m_forward.normalize();
	m_up     .normalize();

	assert(invariant());
}

void CoordinateSystem :: rotateToVector (const Vector3& target,
                                         double max_radians)
{
	Vector3 axis = m_forward.crossProduct(target);
	if(axis.isZero())
		axis = m_up;
	else
		axis.normalize();
	assert(axis.isNormal());

	double radians = m_forward.getAngleSafe(target);
	if(radians > max_radians)
		radians = max_radians;
	m_forward.rotateArbitrary(axis, radians);
	m_up     .rotateArbitrary(axis, radians);
	m_forward.normalize();
	m_up     .normalize();

	assert(invariant());
}



Vector3 CoordinateSystem :: calculateUp (const Vector3& forward)
{
	Vector3 axis = forward.crossProduct(UP_DEFAULT_MACRO);
	if(axis.isZero())
	{
		assert(forward.isOrthogonal(-FORWARD_DEFAULT_MACRO));
		return -FORWARD_DEFAULT_MACRO;
	}
	else
	{
		axis.normalize();
		assert(axis.isNormal());
		assert(forward.isOrthogonal(forward.getRotatedArbitrary(axis, HALF_PI)));
		return forward.getRotatedArbitrary(axis, HALF_PI);
	}
}

bool CoordinateSystem :: invariant () const
{
	if (!m_forward.isNormal()) return false;
	if (!m_up.isNormal()) return false;
	if (!m_forward.isOrthogonal(m_up)) return false;
	if (!m_forward.isFinite()) return false;
	if (!m_up.isFinite()) return false;
	return true;
}

//
//  CoordinateSystem.h
//

#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

#include "../../ObjLibrary/Vector3.h"



//
//  
//
//  A class to represent a local coordinate system.
//
//  Class Invariant:
//    <1> m_forward.isNormal()
//    <2> m_up.isNormal()
//    <3> m_forward.isOrthogonal(m_up)
//    <4> m_forward.isFinite()
//    <5> m_up.isFinite()
//

class CoordinateSystem
{
public:
//
//  POSITION_DEFAULT
//
//  The default position for the origin of a CoordinateSystem.
//

	static const Vector3 POSITION_DEFAULT;

//
//  FORWARD_DEFAULT
//
//  The default forward vector for a CoordinateSystem.
//

	static const Vector3 FORWARD_DEFAULT;

//
//  UP_DEFAULT
//
//  The default up vector for a CoordinateSystem.
//

	static const Vector3 UP_DEFAULT;

//
//  RIGHT_DEFAULT
//
//  The default up vector for a CoordinateSystem.
//

	static const Vector3 RIGHT_DEFAULT;

public:
	CoordinateSystem ();
	CoordinateSystem (const Vector3& position);
	CoordinateSystem (const Vector3& position,
	                  const Vector3& forward);
	CoordinateSystem (const Vector3& position,
	                  const Vector3& forward,
	                  const Vector3& up);

	const Vector3& getPosition () const;
	const Vector3& getForward () const;
	const Vector3& getUp () const;
	Vector3 getRight () const;

	void setupCamera () const;
	void setupOrientationMatrix () const;

	void init();
	void init(const Vector3& position);
	void init(const Vector3& position,
	          const Vector3& forward);
	void init(const Vector3& position,
	          const Vector3& forward,
	          const Vector3& up);
	void setPosition (const Vector3& position);
	void addPosition (const Vector3& increase);
	void setOrientation (const Vector3& forward);
	void setOrientation (const Vector3& forward,
	                     const Vector3& up);

	void setUp (const Vector3& up);
	void moveForward (double distance);
	void moveUp (double distance);
	void moveRight (double distance);
	void rotateAroundForward (double radians);
	void rotateAroundUp (double radians);
	void rotateAroundRight (double radians);
	void rotateToVector (const Vector3& target,
	                     double max_radians);

private:
	static Vector3 calculateUp (const Vector3& forward);

private:
	bool invariant () const;

private:
	Vector3 m_position;
	Vector3 m_forward;
	Vector3 m_up;
};



#endif

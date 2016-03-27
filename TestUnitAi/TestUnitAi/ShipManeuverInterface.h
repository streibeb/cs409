//
//  ShipManeuverInterface.h
//
//  An abstract interface to allow the maneuverability of ships
//    to be queried and changed.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef SHIP_MANEUVER_INTERFACE_H
#define SHIP_MANEUVER_INTERFACE_H

class Vector3;

struct PhysicsObjectId;
class  WorldInterface;
class  UnitAiSuperclass;



//
//  ShipManeuverInterface
//
//  An abstract interface to allow the maneuverability of ships
//    to be queried and changed.
//
//  It is assumed that the ship will be a subclass of
//    PhysicsObject, allowing it to inherit other functions
//    including getClone, isAlive, and markDead.
//

class ShipManeuverInterface
{
public:
//
//  Destructor
//
//  Purpose: To safely destroy a ShipManeuverInterface without
//           memory leaks.  We need a virtual destructor in the
//           base class so that the destructor for the correct
//           derived class will be invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this ShipManeuverInterface is freed.
//

	virtual ~ShipManeuverInterface ()
	{ }  // do nothing

//
//  getSpeedMax
//
//  Purpose: To determine the maximum speed for this Ship.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The maximum speed of this Ship.
//  Side Effect: N/A
//

	virtual double getSpeedMax () const = 0;

//
//  getAcceleration
//
//  Purpose: To determine the acceleration rate for this Ship.
//           Note that the acceleration is only used for changes
//           to speed, not for turning.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The maximum acceleration of this Ship.
//  Side Effect: N/A
//

	virtual double getAcceleration () const = 0;

//
//  getRotationRate
//
//  Purpose: To determine the turning rate for this Ship.  This
//           value is only used for turning, not for changes to
//           speed.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The maximum rotation rate for this Ship in radians
//           per second.
//  Side Effect: N/A
//

	virtual double getRotationRate () const = 0;

//
//  setManeuverability
//
//  Purpose: To change the maximum speed, acceleration, and
//           rotation rate of this Ship to the specified values.
//  Parameter(s):
//    <1> speed_max: The new maximum speed
//    <2> acceleration: The new accleration
//    <3> rotation_rate: The new rotation rate in radians per
//                       second
//  Precondition(s):
//    <1> speed_max >= 0.0
//    <2> acceleration >= 0.0
//    <3> rotation_rate >= 0.0
//  Returns: N/A
//  Side Effect: This Ship is set to have a maximum speed of
//               speed_max, a maximum acceleration of
//               acceleration, and a maximum rotation rate of
//               rotation_rate radians per second.  If the
//               current speed is larger than speed_max, the
//               current speed is reduced.
//

	virtual void setManeuverability (double speed_max,
	                                 double acceleration,
	                                 double rotation_rate) = 0;

};



#endif

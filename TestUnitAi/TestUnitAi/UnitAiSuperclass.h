//
//  UnitAiSuperclass.h
//
//  An abstract superclass for classes to provide AI control for
//    a single Ship.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef UNIT_AI_SUPERCLASS_H
#define UNIT_AI_SUPERCLASS_H

#include "ShipAiInterface.h"
#include "Ship.h"
#include "AiShipReference.h"

struct PhysicsObjectId;
class  WorldInterface;



//
//  UnitAiSuperclass
//
//  An abstract superclass for classes to provide AI control for
//    a single Ship.  A UnitAiSuperclass contains a constant
//    reference to the controlled Ship.  Pure virtual functions
//    exist to run it for one AI cycle and to create a
//    near-duplicate of the current UnitAiSuperclass for another
//    Ship.  The function to create a near-duplicate takes the
//    place of standard duplication to allow the duplicate
//    UnitAiSuperclass to control a duplicate Ship rather than
//    the original.
//
//  An AI cycle will always be at least as long as the time
//    between physics updates.  However, there may be many
//    physics updates between each AI cycle.
//
//  The UnitAiSuperclass class stores a reference to the Ship it
//    controls.  The UnitAiSuperclass for a Ship will always be
//    destroyed before the Ship itself, there is no danger of
//    this producing a dangling pointer.  This reference is of
//    type AiShipReference to ensure that the unit AI does not
//    accidentally change the non-AI properties of the
//    controlled Ship.  A constant reference to the Ship to be
//    accessed with the getShip function and a non-constant
//    reference to the ship AI functions can be accessed with
//    the getAi function.
//
//  Class Invariant:
//    <1> ShipAiInterface.isShip()
//

class UnitAiSuperclass
{
public:
//
//  Destructor
//
//  Purpose: To safely destroy an UnitAiSuperclass without memory
//           leaks.  We need a virtual destructor in the base
//           class so that the destructor for the correct
//           derived class will be invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this UnitAiSuperclass is freed.
//

	virtual ~UnitAiSuperclass ()
	{ }  // do nothing

//
//  isControlledShip
//
//  Purpose: To determine if the specified Ship is the same Ship
//           controlled by this UnitAiSuperclass.  This function
//           is inlined for speed reasons.
//  Parameter(s):
//    <1> ship: The Ship to test
//  Precondition(s): N/A
//  Returns: Whether this UnitAiSuperclass controls Ship ship.
//  Side Effect: N/A
//

	bool isControlledShip (const Ship& ship) const
	{
		return (m_ship == ship);
	}

//
//  getCloneForShip
//
//  Purpose: To create a dynamically allocated near-copy of this
//           UnitAiSuperclass.  This near-copy should be a deep
//           copy of the current UnitAiSuperclass, except that
//           it should control the specified Ship.
//  Parameter(s):
//    <1> ship: The Ship for the copy to control
//  Precondition(s): N/A
//  Returns: A deep near-copy of this UnitAiSuperclass that
//           controls Ship ship is created.  This copy should be
//           created using a modified copy constructor for the
//           derived class.
//  Side Effect: N/A
//

	virtual UnitAiSuperclass* getCloneForShip (
	                     const AiShipReference& ship) const = 0;

//
//  draw
//
//  Purpose: To display the current state of this
//           UnitAiSuperclass.  This function is intended for
//           use in debugging.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Visual information for this UnitAiSuperclass is
//               displayed.  The default implementation for this
//               function does nothing.
//

	virtual void draw () const;

//
//  updateForPause
//
//  Purpose: To update any internal time values for this
//           UnitAiSuperclass after the game has been paused.
//           This function should be called once each time the
//           game is paused.  Calling this function at other
//           times may result in unexpected behaviour.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This UnitAiSuperclass is updated for the most
//               recent pause.  The default implementation for
//               this function does nothing.
//

	virtual void updateForPause ();

//
//  run
//
//  Purpose: To run this UnitAiSuperclass once.  This function
//           will set the desired velocity for the Ship and mark
//           the weapons to fire if that desired.  This function
//           must work correctly even if it is not called every
//           frame.
//  Parameter(s):
//    <1> world: The World that the Ship is in
//  Precondition(s):
//    <1> world.isAlive(getShipId())
//  Returns: N/A
//  Side Effect: The desired velocity for the controlled Ship is
//               updated.  Any weapons that should be fired are
//               marked acccordingly.
//

	virtual void run (const WorldInterface& world) = 0;

protected:
//
//  Constructor
//
//  Purpose: To create a UnitAiSuperclass to control the
//           specified Ship.
//  Parameter(s):
//    <1> ship: The Ship to be controlled
//  Precondition(s):
//    <1> ship.isShip()
//  Returns: N/A
//  Side Effect: A new UnitAi is created to control Ship ship.
//

	UnitAiSuperclass (const AiShipReference& ship);

//
//  Copy Constructor
//  Assignment Operator
//
//  These functions have intentionally not been implemented.
//

	UnitAiSuperclass (const UnitAiSuperclass& original);
	UnitAiSuperclass& operator= (
	                          const UnitAiSuperclass& original);

//
//  getShip
//
//  Purpose: To retrieve a constant reference to the controlled
//           Ship.  This function is inlined for speed reasons.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A constant reference to the controlled Ship.
//  Side Effect: N/A
//

	const Ship& getShip () const
	{	return m_ship.getShip();	}

//
//  getShipAi
//
//  Purpose: To retrieve a non-constant reference to the unit AI
//           functionality for the controlled Ship.  This
//           function is inlined for speed reasons.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A reference to the controlled Ship.
//  Side Effect: N/A
//

	ShipAiInterface& getShipAi () const
	{	return m_ship.getAi();	}

//
//  getShipId
//
//  Purpose: To determine the id for the controlled Ship.  This
//           function is inlined for speed reasons.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The id of the controlled Ship.
//  Side Effect: N/A
//

	const PhysicsObjectId& getShipId () const
	{	return m_ship.getId();	}

//
//  getShipAiShipReference
//
//  Purpose: To retreive an AiShipReference for the controlled
//           Ship.  The AiShipRefernce can be used to initialize
//           a replacement unit AI.  This function is inlined
//           for speed reasons.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: An AiShipReference for the controlled Ship.
//  Side Effect: N/A
//

	const AiShipReference& getShipAiShipReference () const
	{	return m_ship;	}

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
	AiShipReference m_ship;
};



#endif

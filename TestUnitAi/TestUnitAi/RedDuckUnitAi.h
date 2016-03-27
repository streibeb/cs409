//
//  RedDuckUnitAi.h
//
//  All through this file, you should replace "RedDuck",
//    "Red Duck", and "RED_DUCK" with the name of your
//     fleet.
//

#ifndef RED_DUCK_UNIT_AI_H
#define RED_DUCK_UNIT_AI_H

#include "ShipAiInterface.h"
#include "Ship.h"
#include "AiShipReference.h"
#include "UnitAiSuperclass.h"

struct PhysicsObjectId;
class  WorldInterface;



//
//  RedDuck
//
//  A namespace to store classes, functions, and constants for
//    the fleet Red Duck.  The name refers to the Ships
//    composing this fleet being "sitting ducks".
//

namespace RedDuck
{
//
//  UnitAiStop
//
//  A class inheriting from UnitAiSuperclass that makes a Ship
//    stop in place.  This is a minimal unit AI that does as
//    close to nothing as possible.
//

	class UnitAiStop : public UnitAiSuperclass
	{
	public:
	//
	//  Constructor
	//
	//  Purpose: To create a UnitAiStop for the specified Ship
	//           in the specified World.
	//  Parameter(s):
	//    <1> ship: The Ship to be controlled
	//    <2> world: The World the Ship is in
	//  Precondition(s):
	//    <1> ship.isShip()
	//  Returns: N/A
	//  Side Effect: A new UnitAiStop is created to control Ship
	//               ship in World world.
	//

		UnitAiStop (const AiShipReference& ship,
		            const WorldInterface& world);

	//
	//  Modified Copy Constructor
	//
	//  Purpose: To create a new UnitAiStop as a copy of
	//           another, except that it controls the specified
	//           Ship.
	//  Parameter(s):
	//    <1> original: The UnitAiStop to copy
	//    <2> ship: The Ship to control
	//  Precondition(s):
	//    <1> ship.isShip()
	//  Returns: N/A
	//  Side Effect: A new UnitAiStop is created to control Ship
	//               ship.  In all other ways, the new
	//               UnitAiStop is a deep copy of original.
	//

		UnitAiStop (const UnitAiStop& original,
		            const AiShipReference& ship);

	//
	//  Destructor
	//
	//  Purpose: To safely destroy an UnitAi without memory
	//           leaks.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: N/A
	//  Side Effect: All dynamically allocated memory associated
	//               with this UnitAi is freed.
	//

		virtual ~UnitAiStop ();

	///////////////////////////////////////////////////////////////
	//
	//  Virtual function inherited from UnitAiSuperclass
	//

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
	                         const AiShipReference& ship) const;

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
	//               displayed.  The default implementation does
	//               nothing.
	//

	//	virtual void draw () const;

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

		virtual void run (const WorldInterface& world);

	private:
	///////////////////////////////////////////////////////////////
	//
	//  Helper functions not inherited from anywhere
	//

	//
	//  Copy Constructor
	//  Assignment Operator
	//
	//  These functions have intentionally not been implemented.
	//    Use the modified copy constructor instead.
	//

		UnitAiStop (const UnitAiStop& original);
		UnitAiStop& operator= (const UnitAiStop& original);

	};



}  // end of namespace RedDuck



#endif

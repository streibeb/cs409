//
//  RedDuckUnitAi.cpp
//
//  All through this file, you should replace "RedDuck",
//    "Red Duck", and "RED_DUCK" with the name of your
//     fleet.
//

#include <cassert>

#include "ObjLibrary/Vector3.h"

#include "WorldInterface.h"
#include "PhysicsObjectId.h"
#include "ShipAiInterface.h"
#include "Ship.h"
#include "UnitAiSuperclass.h"
#include "RedDuckUnitAi.h"



using namespace RedDuck;



UnitAiStop :: UnitAiStop (const AiShipReference& ship,
                          const WorldInterface& world)
		: UnitAiSuperclass(ship)
{
	assert(ship.isShip());
}

UnitAiStop :: UnitAiStop (const UnitAiStop& original,
                          const AiShipReference& ship)
		: UnitAiSuperclass(ship)
{
	assert(ship.isShip());
}

UnitAiStop :: ~UnitAiStop ()
{
}



///////////////////////////////////////////////////////////////
//
//  Virtual function inherited from UnitAiSuperclass
//

UnitAiSuperclass* UnitAiStop :: getCloneForShip (const AiShipReference& ship) const
{
	assert(ship.isShip());

	return new UnitAiStop(*this, ship);
}

/*
void UnitAiStop :: draw () const
{
}
*/

void UnitAiStop :: run (const WorldInterface& world)
{
	assert(world.isAlive(getShipId()));

	getShipAi().setDesiredVelocity(Vector3::ZERO);
}



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
//	UnitAiStop :: UnitAiStop (const UnitAiStop& original);
//	UnitAiStop :: UnitAiStop& operator= (const UnitAiStop& original);
//

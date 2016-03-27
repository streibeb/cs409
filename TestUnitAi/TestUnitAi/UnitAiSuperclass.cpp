//
//  UnitAiSuperclass.cpp
//
//  DO NOT MODIFY THIS FILE
//

#include <cassert>

#include "ShipAiInterface.h"
#include "Ship.h"

#include "UnitAiSuperclass.h"



void UnitAiSuperclass :: draw () const
{
	; // do nothing
}



void UnitAiSuperclass :: updateForPause ()
{
	; // do nothing
}



UnitAiSuperclass :: UnitAiSuperclass (const AiShipReference& ship)
		: m_ship(ship)
{
	assert(ship.isShip());

	assert(invariant());
}

//
//  Copy Constructor
//  Assignment Operator
//
//  These functions have intentionally not been implemented.
//
//  UnitAiSuperclass :: UnitAiSuperclass (const UnitAiSuperclass& original);
//  UnitAiSuperclass& UnitAiSuperclass :: operator= (const UnitAiSuperclass& original);
//

bool UnitAiSuperclass :: invariant () const
{
	if(!m_ship.isShip()) return false;
	return true;
}


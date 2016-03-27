//
//  AiShipReference.h
//
//  DO NOT MODIFY THIS FILE
//

#ifndef AI_SHIP_REFERENCE_H
#define AI_SHIP_REFERENCE_H

#include <cassert>

#include "ObjLibrary/Vector3.h"

#include "ShipAiInterface.h"
#include "Ship.h"



//
//  AiShipReference
//
//  An encapsulated class to represent a Ship for AI classes,
//    such as those that inherit from UnitAiSuperclass or
//    FleetAiSuperclass.  A AiShipReference stores a Ship, but
//    only allows it to be accessed in a manner appropriate to
//    AI control.  The Ship class is assumed to implement the
//    ShipAiInterface interface.
//
//  A non-const Ship can be converted to a AiShipReference
//    implictly.  The const functionality of the Ship class can
//    be accessed with the getShip() function, and the entire
//    functionality of the ShipAiInterface interface can be
//    accessed with the getAi() function.
//
//  This class is entirely inlined for speed reasons.
//

class AiShipReference
{
public:
//
//  Default Constructor
//
//  Purpose: To create a AiShipReference without a Ship to
//           represent.
//  Parameter(s):
//    <1> ship: The Ship to represent
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new, uninitialized AiShipReference.
//

	AiShipReference ()
			: mp_ship(NULL)
	{
		assert(!isShip());
	}

//
//  Constructor
//
//  Purpose: To create a AiShipReference to represent the
//           specified Ship.
//  Parameter(s):
//    <1> r_ship: A reference to the Ship to represent
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new AiShipReference is created to represent
//               Ship r_ship.
//

	AiShipReference (Ship& r_ship)
			: mp_ship(&r_ship)
	{
		assert(isShip());
	}

//
//  Copy Constructor
//
//  Purpose: To create a AiShipReference to represent the same
//           Ship as another AiShipReference.
//  Parameter(s):
//    <1> original: The AiShipReference to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new AiShipReference is created to represent
//               the same Ship as original.  If original did not
//               represent a Ship, the new AiShipReference also
//               does not represent a Ship.
//

	AiShipReference (const AiShipReference& original)
			: mp_ship(original.mp_ship)
	{
		assert(isShip() == original.isShip());
	}

//
//  Destructor
//
//  Purpose: To safely destroy a AiShipReference without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this AiShipReference is freed.
//

	~AiShipReference ()
	{ }

//
//  Assignment Operator
//
//  Purpose: To modify this AiShipReference to represent the
//           same Ship as another.
//  Parameter(s):
//    <1> original: The AiShipReference to copy
//  Precondition(s): N/A
//  Returns: A reference to this AiShipReference.
//  Side Effect: This AiShipReference is set to represent the
//               same Ship as original.  If original did not
//               represent a Ship, the new AiShipReference also
//               does not represent a Ship.
//

	AiShipReference& operator= (const AiShipReference& original)
	{
		mp_ship = original.mp_ship;
		assert(isShip() == original.isShip());
		return *this;
	}

//
//  Assignment Operator
//
//  Purpose: To modify this AiShipReference to represent the
//           specified non-const Ship.
//  Parameter(s):
//    <1> r_ship: A reference to the Ship to represent
//  Precondition(s): N/A
//  Returns: A reference to this AiShipReference.
//  Side Effect: This AiShipReference is set to represent Ship
//               r_ship.
//

	AiShipReference& operator= (Ship& r_ship)
	{
		mp_ship = &r_ship;
		assert(isShip());
		return *this;
	}

//
//  Equality Test
//
//  Purpose: To determine if this AiShipReference refers to the
//           same Ship as as this AiShipReference.
//  Parameter(s):
//    <1> other: The AiShipReference to test
//  Precondition(s): N/A
//  Returns: Whether this AiShipReference and other refer to the
//           same Ship.
//  Side Effect: N/A
//

	bool operator== (const AiShipReference& other) const
	{
		return (mp_ship == other.mp_ship);
	}

//
//  Equality Test
//
//  Purpose: To determine if this AiShipReference refers to the
//           specified Ship.
//  Parameter(s):
//    <1> other: The Ship to test
//  Precondition(s): N/A
//  Returns: Whether this AiShipReference refers to Ship ship.
//  Side Effect: N/A
//

	bool operator== (const Ship& other) const
	{
		return (mp_ship == &other);
	}

//
//  isShip
//
//  Purpose: To determine if this AiShipReference has a Ship to
//           represent.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this AiShipReference represents a Ship.
//  Side Effect: N/A
//

	bool isShip () const
	{
		return (mp_ship != NULL);
	}

//
//  getShip
//
//  Purpose: To retrieve a constant reference to the Ship this
//           AiShipReference represents.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//  Returns: The Ship this AiShipReference represents.
//  Side Effect: N/A
//

	const Ship& getShip () const
	{
		assert(isShip());

		return *mp_ship;
	}

//
//  getAi
//
//  Purpose: To retrieve a non-constant reference to the AI
//           interface for the Ship this AiShipReference
//           represents.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//  Returns: The AI interface for the Ship this AiShipReference
//           represents.
//  Side Effect: N/A
//

	ShipAiInterface& getAi () const
	{
		assert(isShip());

		return *mp_ship;
	}

/*  CORRECTED VERSION HANDLES CONST CORRECTLY
    NOT ADDED TO AVOID INTERFERING WITH STUDENT PROGRAMS

//
//  getAi
//
//  Purpose: To retrieve a non-constant reference to the AI
//           interface for the Ship this AiShipReference
//           represents.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//  Returns: The AI interface for the Ship this AiShipReference
//           represents.  The reference returned is const if and
//           only if this AiShipReference is const.
//  Side Effect: N/A
//

	const ShipAiInterface& getAi () const
	{
		assert(isShip());

		return *mp_ship;
	}

	ShipAiInterface& getAi ()
	{
		assert(isShip());

		return *mp_ship;
	}
*/

//
//  getId
//
//  Purpose: To determine the ID of the Ship this
//           AiShipReference represents.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//  Returns: The ID the Ship this AiShipReference represents.
//  Side Effect: N/A
//

	const PhysicsObjectId& getId () const
	{
		assert(isShip());

		return mp_ship->getId();
	}

//
//  isUnitAiSet
//
//  Purpose: To determine if the Ship this AiShipReference
//           represents currently has a unit AI controlling it.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//  Returns: Whether the Ship this AiShipReference represents
//           has a unit AI.
//  Side Effect: N/A
//

	bool isUnitAiSet () const
	{
		assert(isShip());

		return mp_ship->isUnitAiSet();
	}

//
//  getUnitAi
//
//  Purpose: To retrieve a reference to the unit AI instance
//           that controls the Ship this AiShipReference
//           represents.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//    <2> isUnitAiSet()
//  Returns: The unit AI for the Ship this AiShipReference
//           represents.  The reference returned is const if and
//           only if this AiShipReference is const.
//  Side Effect: N/A
//

	const UnitAiSuperclass& getUnitAi () const
	{
		assert(isShip());
		assert(isUnitAiSet());

		return mp_ship->getUnitAi();
	}

	UnitAiSuperclass& getUnitAi ()
	{
		assert(isShip());
		assert(isUnitAiSet());

		return mp_ship->getUnitAi();
	}

//
//  isUnitAiOfType
//
//  Purpose: To determine if the Ship this AiShipReference
//           represents currently has a unit AI of the indicated
//           class controlling it.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//  Returns: false if the Ship this AiShipReference represents
//           is not controlled by a unit AI, or if the unit AI
//           is not of class UnitAiType (or a subclass of class
//           UnitAiType).
//  Side Effect: N/A
//

	template <typename UnitAiType>
	bool isUnitAiOfType () const
	{
		assert(isShip());

		if(!mp_ship->isUnitAiSet())
			return false;
		assert(mp_ship->isUnitAiSet());

		const UnitAiSuperclass* p_unit_ai = &(mp_ship->getUnitAi());
		assert(p_unit_ai != NULL);

		// if downcast succeeds, must be of type UnitAiType
		return (dynamic_cast<const UnitAiType*>(p_unit_ai) != NULL);
	}

//
//  getUnitAiByType
//
//  Purpose: To retrieve a reference to the unit AI instance
//           that controls the Ship this AiShipReference
//           represents, typecasting it to the indicated subclass.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isShip()
//    <2> isUnitAiOfType<UnitAiType>()
//  Returns: The unit AI for the Ship this AiShipReference
//           represents.  The reference returned is const if and
//           only if this AiShipReference is const.
//  Side Effect: N/A
//

	template <typename UnitAiType>
	const UnitAiType& getUnitAiByType () const
	{
		assert(isShip());
		assert(isUnitAiOfType<UnitAiType>());

		const UnitAiSuperclass* p_unit_ai = &(mp_ship->getUnitAi());
		assert(p_unit_ai != NULL);

		// if of type UnitAiType, downcast will succeed
		assert((dynamic_cast<const UnitAiType*>(p_unit_ai)) != NULL);
		return *(dynamic_cast<const UnitAiType*>(p_unit_ai));
	}

	template <typename UnitAiType>
	UnitAiType& getUnitAiByType ()
	{
		assert(isShip());
		assert(isUnitAiOfType<UnitAiType>());

		UnitAiSuperclass* p_unit_ai = &(mp_ship->getUnitAi());
		assert(p_unit_ai != NULL);

		// if of type UnitAiType, downcast will succeed
		assert((dynamic_cast<UnitAiType*>(p_unit_ai)) != NULL);
		return *(dynamic_cast<UnitAiType*>(p_unit_ai));
	}

//
//  clearShip
//
//  Purpose: To modify this AiShipReference to not represent any
//           Ship.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This AiShipReference is set to not represent
//               any Ship.
//

	void clearShip ()
	{
		mp_ship = NULL;
		assert(!isShip());
	}

private:
	Ship* mp_ship;
};



#endif

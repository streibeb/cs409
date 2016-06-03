//
//  PhysicsObjectId.h
//
//  A data structure to represent an identifier for a physics
//    object.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef PHYSICS_OBJECT_ID_H
#define PHYSICS_OBJECT_ID_H

#include <cassert>
#include <iostream>



//
//  PhysicsObjectId
//
//  This is not a subclass of PhysicsObject.
//
//  A data structure to represent an identifier for a physics
//    object.  A PhysicsObjectId can be created from an unsigned
//    integer or from an object type, a fleet, and index.  Once
//    created, it can be interpreted as an unsigned interger
//    (via implicit typecast) or as simple structured data
//    composed an object type, fleet, and index.
//
//  Numerical comparisons (equality test, etc.) of
//    PhysicsObjectIds are possible through to implicit
//    conversions to and from unsigned integers.
//
//  The relationship between the interpretations is consistant
//    and lossless but not otherwise undefined.  This means that
//    an PhysicsObjectId created from an object type, fleet, and
//    index can be stored in an unsigned integer, used to
//    construct a new PhysicsObjectId, and then queried to
//    successfully determine the original object type, fleet,
//    and index.  However, attempting to extract object type,
//    fleet, or index information from an PhysicsObjectId that
//    was original created from an unsigned integer may produce
//    unexpected results.
//
//  A number of constants are provided for object type and
//    fleet.  These are not an exaustive list: any value up to
//    (and including) TYPE_MAX or FLEET_MAX can be used.
//
//  This struct is entirely inlined for speed reasons.
//
//  The functions in this struct make heavy use of
//    reinterpret_cast<> to convert to unsigned integers and
//    back.  reinterpret_cast<> reinterprets a pointer of one
//    type as a pointer of another type without changing the
//    underlying bits (in practice, standard is undefined).
//    For example, a float pointer could be used as an integer
//    pointer (the integer would be gibberish).  The underlying
//    representation of the new form should be as large or
//    larger than the existing form.  Use with extreme caution
//    and avoid if possible.
//

struct PhysicsObjectId
{
public:
//
//  ID_NOTHING
//
//  A special id number that does not correspond to anything in
//    the world.
//
//  These constants are specified as unsigned integers.  This
//    allows them to be initialized inside the class declaration
//    (in the .h file) instead of in a .cpp file.  This, in
//    turn, guarentees that they are always initialized when
//    they are used.  This is important because
//    not-yet-initialized constants result in intermittent bugs
//    that are hard to find and even harder to fix.
//
//  Note: ~0u is always the largest possible unsigned integer,
//        regardless of word length.  ~1u is the second largest.
//

	static const unsigned int ID_NOTHING = ~0u;

//
//  ID_DEFAULT
//
//  The default id number for physics objects that have not had
//    their id set.  This id number should not be used as a real
//    id value.
//

	static const unsigned int ID_DEFAULT = ~1u;

//
//  TYPE_MAX
//
//  The largest legal value for the object type.
//

	static const unsigned int TYPE_MAX =
	                   (0x1 << (sizeof(unsigned char) * 8)) - 1;

//
//  FLEET_MAX
//
//  The largest legal value for the object fleet.
//

	static const unsigned int FLEET_MAX =
	                   (0x1 << (sizeof(unsigned char) * 8)) - 1;

//
//  INDEX_MAX
//
//  The largest legal value for the object index.
//

	static const unsigned int INDEX_MAX =
	                  (0x1 << (sizeof(unsigned short) * 8)) - 1;

//
//  TYPE_PLANETOID
//
//  A constant indicating that the object is a planetoid.
//

	static const unsigned int TYPE_PLANETOID = 0;

//
//  TYPE_SHIP
//
//  A constant indicating that the object is a ship.
//

	static const unsigned int TYPE_SHIP = 1;

//
//  TYPE_BULLET
//
//  A constant indicating that the object is a bullet.
//

	static const unsigned int TYPE_BULLET = 2;

//
//  TYPE_MISSILE
//
//  A constant indicating that the object is a missile.
//

	static const unsigned int TYPE_MISSILE = 3;

//
//  TYPE_MARKER
//
//  A constant indicating that the object is a marker.
//

	static const unsigned int TYPE_MARKER = 4;

//
//  FLEET_NATURE
//
//  A constant indicating that an object is not part of any
//    fleet because it is a natural phenomenon.  For example, a
//    planet.
//

	static const unsigned int FLEET_NATURE = 0;

//
//  FLEET_PLAYER
//
//  A constant indicating that an object is part of the same
//    fleet as the player.
//

	static const unsigned int FLEET_PLAYER = 1;

//
//  FLEET_ENEMY
//
//  A constant indicating that an object is part of the enemy
//    fleet.  For multi-sided combat, a different value should
//    be used for each enemy fleet.
//

	static const unsigned int FLEET_ENEMY = 2;

public:
//
//  Default Constructor
//
//  Purpose: To create a new PhysicsObjectId with default
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PhysicsObjectId is created with an object
//               type, fleet, and index of 0.  Interpreted as an
//               unsigned integer, the value is also 0.
//

	PhysicsObjectId ()
			: m_type(0), m_fleet(0), m_index(0)
	{ }

//
//  Constructor
//
//  Purpose: To create a new PhysicsObjectId representing an
//           object with the specified type, fleet, and index.
//  Parameter(s):
//    <1> type: The object type
//    <2> fleet: The object fleet
//    <3> index: The object index
//  Precondition(s):
//    <1> type <= TYPE_MAX
//    <2> fleet <= FLEET_MAX
//    <3> index <= INDEX_MAX
//  Returns: N/A
//  Side Effect: A new PhysicsObjectId is created with an object
//               type of type, an object fleet of fleet, and an
//               object index of index.
//

	PhysicsObjectId (unsigned char type,
	                 unsigned char fleet,
	                 unsigned short index)
			: m_type(type), m_fleet(fleet), m_index(index)
	{
		assert(type <= TYPE_MAX);
		assert(fleet <= FLEET_MAX);
		assert(index <= INDEX_MAX);
	}

//
//  Constructor
//
//  Purpose: To create a new PhysicsObjectId from the specified
//           unsigned integer.  The value for the object type,
//           fleet, and index are determined consistantly from
//           the unsigned integer, but are otherwise undefined.
//  Parameter(s):
//    <1> value: The unsigned integer
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PhysicsObjectId is created from value.
//               If value was determined from another
//               PhysicsObjectId, the new PhysicsObjectId has
//               the same object type, fleet, and index as that
//               PhysicsObjectId.
//

	PhysicsObjectId (unsigned int value)
	{
		// see note on reinterpret_cast<> above
		assert(sizeof(PhysicsObjectId) == sizeof(unsigned int));
		*(reinterpret_cast<unsigned int*>(this)) = value;
	}

//
//  Copy Constructor
//
//  Purpose: To create a new PhysicsObjectId representing the
//           same object type, fleet, and index as another.
//  Parameter(s):
//    <1> original: The PhysicsObjectId to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PhysicsObjectId is created with the same
//               object type, fleet, and index as original.
//

	PhysicsObjectId (const PhysicsObjectId& original)
			: m_type(original.m_type),
			  m_fleet(original.m_fleet),
			  m_index(original.m_index)
	{ }

//
//  Destructor
//
//  Purpose: To safely destroy a PhysicsObjectId without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: None.
//

	~PhysicsObjectId ()
	{ }

//
//  Assignment Operator
//
//  Purpose: To modify this PhysicsObjectId to represent the
//           same object type, fleet, and index as another.
//  Parameter(s):
//    <1> original: The PhysicsObjectId to copy
//  Precondition(s): N/A
//  Returns: A reference to this PhysicsObjectId.
//  Side Effect: This PhysicsObjectId is set to have the same
//               object type, fleet, and index as original.
//

	PhysicsObjectId& operator= (const PhysicsObjectId& original)
	{
		//
		//  Checking for equality here is a waste of time.  By
		//    reinterpreting our data as unsigned integers, we
		//    can copy it in one operation.  Even if
		//    self-assignment occurs, we will still get the
		//    correct result.
		//

		// see note on reinterpret_cast<> above
		assert(sizeof(PhysicsObjectId) == sizeof(unsigned int));
		*(reinterpret_cast<unsigned int*>(this)) =
		    *(reinterpret_cast<const unsigned int*>(&original));

		return *this;
	}

//
//  Assignment Operator
//
//  Purpose: To modify this PhysicsObjectId to have a
//           representation determined by an unsigned integer.
//  Parameter(s):
//    <1> value: The unsigned integer value
//  Precondition(s): N/A
//  Returns: A reference to this PhysicsObjectId.
//  Side Effect: This PhysicsObjectId is set based on value.  If
//               value was determined from another
//               PhysicsObjectId, this PhysicsObjectId has the
//               same object type, fleet, and index as that
//               PhysicsObjectId.
//

	PhysicsObjectId& operator= (unsigned int value)
	{
		// see note on reinterpret_cast<> above
		assert(sizeof(PhysicsObjectId) == sizeof(unsigned int));
		*(reinterpret_cast<unsigned int*>(this)) = value;

		return *this;
	}

//
//  Typecast to Unsigned Integer
//
//  Purpose: To determine the unsigned integrer representation
//           of this PhysicsObjectId.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: This PhysicsObjectId, represented as an unsigned
//           integer.  The value returned is not an lvalue, but
//           assignment should still be possible using the
//           assignment operator.
//  Side Effect: N/A
//

	operator unsigned int () const
	{
		// see note on reinterpret_cast<> above
		assert(sizeof(PhysicsObjectId) == sizeof(unsigned int));
		return *(reinterpret_cast<const unsigned int*>(this));
	}

public:
		unsigned char m_type;
		unsigned char m_fleet;
		unsigned short m_index;
};

//
//  Stream Insertion Operator
//
//  Purpose: To print the specified PhysicsObjectId to the
//           specified output stream.
//  Parameter(s):
//    <1> r_os: The output stream
//    <2> id: The PhysicisObjectId
//  Precondition(s): N/A
//  Returns: A reference to r_os.
//  Side Effect: id is printed to r_os.
//

inline std::ostream& operator<< (std::ostream& r_os,
                                 const PhysicsObjectId& id)
{
	r_os               << (unsigned int)(id)
	     << " (type "  << (unsigned int)(id.m_type)
	     << ", fleet " << (unsigned int)(id.m_fleet)
	     << ", index " << (unsigned int)(id.m_index) << ")";
	return r_os;
}



#endif

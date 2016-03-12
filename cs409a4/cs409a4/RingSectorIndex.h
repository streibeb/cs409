//
//  RingSectorIndex.h
//
//  A class to represent the index of a ring sector in space.
//    The ring sector index must be represented as structured
//    data because the number of ring sectors is too large to be
//    represented in a 32-bit int.
//
//  This class is not a subclass of RingSector.
//

#ifndef RING_SECTOR_INDEX_H
#define RING_SECTOR_INDEX_H

#include <cmath>
#include <iostream>

#include "../../ObjLibrary/Vector3.h"

#include "RingSectorSize.h"



//
//  RingSectorIndex
//
//  A class to represent the position of a sector in the 3D grid
//    that covers the game space.  Once created, a Sector cannot
//    be modified.
//
//  A RingSectorIndex is composed of 3 short (16-bit) ints
//    representing X, Y, and Z.  Positions exactly on the
//    dividing line are assumed to lie within the sector with
//    the greater index.  For example, 0.0 goes with 0, not -1.
//
//  This class is entirely inlined for speed reasons.
//

class RingSectorIndex
{
public:
//
//  Default Constructor
//
//  Purpose: To create a RingSectorIndex for the origin.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingSectorIndex is created with all
//               components set to 0.
//

	RingSectorIndex ()
			: m_x(0), m_y(0), m_z(0) // no short literals in C++
	{	}

//
//  Initializing Constructor
//
//  Purpose: To create a RingSectorIndex with the specified
//           coordinates.
//  Parameter(s):
//    <1> x: The x coordinate
//    <2> y: The y coordinate
//    <3> z: The z coordinate
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingSectorIndex is created for the ring
//               sector with coordinates (x, y, z).
//

	RingSectorIndex (short x, short y, short z)
			: m_x(x), m_y(y), m_z(z)
	{	}

//
//  Constructor
//
//  Purpose: To create a RingSectorIndex for the specified
//           position.
//  Parameter(s):
//    <1> position: The position in world space
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingSectorIndex is created for the ring
//               sector that includes position position.
//

	RingSectorIndex (const Vector3& position)
			: m_x((short)(floor(position.x / RING_SECTOR_SIZE))),
			  m_y((short)(floor(position.y / RING_SECTOR_SIZE))),
			  m_z((short)(floor(position.z / RING_SECTOR_SIZE)))
	{	}

//
//  Copy Constructor
//
//  Purpose: To create a RingSectorIndex for the same ring
//           sector as another.
//  Parameter(s):
//    <1> original: The RingSectorIndex to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new RingSectorIndex is created with the same
//               components as original.
//

	RingSectorIndex (const RingSectorIndex& original)
			: m_x(original.m_x),
			  m_y(original.m_y),
			  m_z(original.m_z)
	{	}

//
//  Destructor
//
//  Purpose: To safely destroy a RingSectorIndex.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	~RingSectorIndex ()
	{	}

//
//  Assignment Operator
//
//  Purpose: To modify this RingSectorIndex to represent the
//           same sector as another.
//  Parameter(s):
//    <1> original: The RingSectorIndex to copy
//  Precondition(s): N/A
//  Returns: A reference to this SectorIndex
//  Side Effect: This RingSectorIndex is set to have the same
//               components as original.
//

	RingSectorIndex& operator= (const RingSectorIndex& original)
	{
		// cheching for self-assignment would be a waste of time
		m_x = original.m_x;
		m_y = original.m_y;
		m_z = original.m_z;
		return *this;
	}

//
//  Equality Test Operator
//
//  Purpose: To determine if this RingSectorIndex and another
//           represent the same ring sector.
//  Parameter(s):
//    <1> other: The SectorIndex to compare to
//  Precondition(s): N/A
//  Returns: Whether this RingSectorIndex represents the same
//           sector of ring space as other.
//  Side Effect: N/A
//

	bool operator== (const RingSectorIndex& other)
	{
		if(m_x != other.m_x) return false;
		if(m_y != other.m_y) return false;
		if(m_z != other.m_z) return false;
		return true;
	}

//
//  Inequality Test Operator
//
//  Purpose: To determine if this RingSectorIndex and another do not
//           represent the same sector.
//  Parameter(s):
//    <1> other: The RingSectorIndex to compare to
//  Precondition(s): N/A
//  Returns: Whether this RingSectorIndex does not represent the
//           same sector of ring space as other.
//  Side Effect: N/A
//

	bool operator!= (const RingSectorIndex& other)
	{
		if(m_x != other.m_x) return true;
		if(m_y != other.m_y) return true;
		if(m_z != other.m_z) return true;
		return false;
	}

//
//  getX
//  getY
//  getZ
//
//  Purpose: To determine the X/Y/Z component of this
//           SectorIndex.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The X/Y/Z component of this RingSectorIndex.
//  Side Effect: N/A
//

	short getX () const
	{	return m_x;	}
	short getY () const
	{	return m_y;	}
	short getZ () const
	{	return m_z;	}

//
//  getCenter
//
//  Purpose: To determine the center of this RingSectorIndex.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The center of the ring sector represented by this
//           RingSectorIndex.
//  Side Effect: N/A
//

	Vector3 getCenter () const
	{
		return Vector3((m_x + 0.5) * RING_SECTOR_SIZE,
		               (m_y + 0.5) * RING_SECTOR_SIZE,
		               (m_z + 0.5) * RING_SECTOR_SIZE);
	}

private:
	short m_x;
	short m_y;
	short m_z;
};



//
//  Stream Insertion Operator
//
//  Purpose: To print the specified RingSectorIndex to the
//           specified output stream.
//  Parameter(s):
//    <1> r_os: A reference to the output stream
//    <2> index: The RingSectorIndex
//  Precondition(s): N/A
//  Returns: A reference to r_os.
//  Side Effect: index is printed to r_os.
//

inline std::ostream& operator<< (std::ostream& r_os,
                                 const RingSectorIndex& index)
{
	r_os << "("  << index.getX()
	     << ", " << index.getY()
	     << ", " << index.getZ() << ")";
	return r_os;
}



#endif

//
//  WorleyPoint.h
//

#ifndef WORLEY_POINT_H
#define WORLEY_POINT_H

#include <vector>

#include "PseudorandomGrid.h"



//
//  WorleyPoint1
//

class WorleyPoint1 : public PseudorandomGrid1
{
public:
	//
	//  Point1
	//
	//  A record to represent the data for a single Worley point
	//    in 1-dimensional space.  The point has 1 coordinate
	//    (x) and an associated seed.  The seed can be used to
	//    generate additional pseudorandom properties for the
	//    point.
	//

	struct Point1
	{
		float m_x;
		unsigned int m_seed;
	};

private:
//
//  Class Function: generatePoint
//
//  Purpose: To create a Point1 from the specified psuedorandom
//           seed.  This function is inlined for speed reasons.
//  Parameter(s):
//    <1> r_a: A reference to the seed value
//  Precondition(s): N/A
//  Returns: A Point1 calculated from r_a.  The coordinate is in
//           the range [0, 1), and the seed is in the range
//           [0, MAX_INT].  The coordinate and the seed are
//           equally distributed.
//  Side Effect: r_a is updated to be ready to generate another
//               point.
//

	static Point1 generatePoint (unsigned int& r_a)
	{
		static const float SCALE_TO_01 = 1.0f / (~0u + 1.0f);

		Point1 result;
		result.m_x    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_seed = calculateNextPseudorandom(r_a);
		return result;
	}

public:
//
//  Default Constructor
//
//  Purpose: To create a new WorleyPoint1 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint1 is created with default seed
//               values.
//

	WorleyPoint1 ();

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint1 with the specified
//           seed values.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The directionless seed values
//    <4> seed_x1
//    <5> seed_x2: The coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint1 is created with quadratic
//               seed values of seed0, seed1 and coordinate seed
//               values of seed2, seed_x1, and seed_x2.
//

	WorleyPoint1 (unsigned int seed0,
	              unsigned int seed1,
	              unsigned int seed2,
	              unsigned int seed_x1,
	              unsigned int seed_x2);

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint1 with the specified
//           seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 5 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new WorleyPoint1 is created with the first
//               SEED_COUNT = 5 values in a_seeds as seed
//               values.
//

	WorleyPoint1 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new WorleyPoint1 with the same seed
//           values as another.
//  Parameter(s):
//    <1> original: The WorleyPoint1 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint1 is created with a the same
//               seed values as original.
//

	WorleyPoint1 (const WorleyPoint1& original);

//
//  Destructor
//
//  Purpose: To safely destroy a WorleyPoint1 without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~WorleyPoint1 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this WorleyPoint1 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The WorleyPoint1 to copy
//  Precondition(s): N/A
//  Returns: A reference to this WorleyPoint1.
//  Side Effect: This WorleyPoint1 is set to have the same seed
//               values as original.
//

	WorleyPoint1& operator= (const WorleyPoint1& original);

//
//  getPoints
//
//  Purpose: To determine the positions of the first points in
//           the specified cell.
//  Parameter(s):
//    <1> count: How many point positions
//    <2> x: The cell coordinate
//  Precondition(s): N/A
//  Returns: An std::vector containing the data for the first
//           count points in cell (x).  The position returned is
//           in global space, not the position within the cell.
//           For example, cell (2) would have an X coordinate in
//           the range [2, 3).
//  Side Effect: N/A
//

	std::vector<Point1> getPoints (unsigned int count,
	                               int x) const;
};



//
//  WorleyPoint2
//

class WorleyPoint2 : public PseudorandomGrid2
{
public:
	//
	//  Point2
	//
	//  A record to represent the data for a single Worley point
	//    in 2-dimensional space.  The point has 2 coordinates
	//    (x, y) and an associated seed.  The seed can be used
	//    to generate additional pseudorandom properties for the
	//    point.
	//

	struct Point2
	{
		float m_x;
		float m_y;
		unsigned int m_seed;
	};

private:
//
//  Class Function: generatePoint
//
//  Purpose: To create a Point2 from the specified psuedorandom
//           seed.  This function is inlined for speed reasons.
//  Parameter(s):
//    <1> r_a: A reference to the seed value
//  Precondition(s): N/A
//  Returns: A Point2 calculated from r_a.  The coordinates are
//           both in the range [0, 1), and the seed is in the
//           range [0, MAX_INT].  The coordinate and the seed
//           are equally distributed.
//  Side Effect: r_a is updated to be ready to generate another
//               point.
//

	static Point2 generatePoint (unsigned int& r_a)
	{
		static const float SCALE_TO_01 = 1.0f / (~0u + 1.0f);

		Point2 result;
		result.m_x    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_y    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_seed = calculateNextPseudorandom(r_a);
		return result;
	}

public:
//
//  Default Constructor
//
//  Purpose: To create a new WorleyPoint2 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint2 is created with default seed
//               values.
//

	WorleyPoint2 ();

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint2 with the specified
//           seed values.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The directionless seed values
//    <4> seed_x1
//    <5> seed_x2: The X coordinate seed values
//    <6> seed_y1
//    <7> seed_y2: The Y coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint1 is created with a
//               directionless seed values of seed0, seed1, and
//               seed2, X coordinate seed values of seed_x1 and
//               seed_x2, and Y coordinate seed values of
//               seed_y1 and seed_y2.
//

	WorleyPoint2 (unsigned int seed0,
	              unsigned int seed1,
	              unsigned int seed2,
	              unsigned int seed_x1,
	              unsigned int seed_x2,
	              unsigned int seed_y1,
	              unsigned int seed_y2);

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint2 with the specified
//           seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 7 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new WorleyPoint2 is created with the first
//               SEED_COUNT = 7 values in a_seeds as seed
//               values.
//

	WorleyPoint2 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new WorleyPoint2 with the same seed
//           values as another.
//  Parameter(s):
//    <1> original: The WorleyPoint2 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint2 is created with a the same
//               seed values as original.
//

	WorleyPoint2 (const WorleyPoint2& original);

//
//  Destructor
//
//  Purpose: To safely destroy a WorleyPoint2 without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~WorleyPoint2 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this WorleyPoint2 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The WorleyPoint2 to copy
//  Precondition(s): N/A
//  Returns: A reference to this WorleyPoint2.
//  Side Effect: This WorleyPoint2 is set to have the same seed
//               values as original.
//

	WorleyPoint2& operator= (const WorleyPoint2& original);

//
//  getPoints
//
//  Purpose: To determine the positions of the first points in
//           the specified cell.
//  Parameter(s):
//    <1> count: How many point positions
//    <2> x
//    <3> y: The cell coordinate
//  Precondition(s): N/A
//  Returns: An std::vector containing the data for the first
//           count points in cell (x, y).  The position returned
//           is in global space, not the position within the
//           cell.  For example, cell (2, 5) would have an X
//           coordinate in the range [2, 3) and a Y coordinate
//           in the range [5, 6).
//  Side Effect: N/A
//

	std::vector<Point2> getPoints (unsigned int count,
	                               int x, int y) const;
};



//
//  WorleyPoint3
//

class WorleyPoint3 : public PseudorandomGrid3
{
public:
	//
	//  Point3
	//
	//  A record to represent the data for a single Worley point
	//    in 3-dimensional space.  The point has 3 coordinates
	//    (x, y, z) and an associated seed.  The seed can be
	//    used to generate additional pseudorandom properties
	//    for the point.
	//

	struct Point3
	{
		float m_x;
		float m_y;
		float m_z;
		unsigned int m_seed;
	};

private:
//
//  Class Function: generatePoint
//
//  Purpose: To create a Point3 from the specified psuedorandom
//           seed.  This function is inlined for speed reasons.
//  Parameter(s):
//    <1> r_a: A reference to the seed value
//  Precondition(s): N/A
//  Returns: A Point3 calculated from r_a.  The coordinates are
//           each in the range [0, 1), and the seed is in the
//           range [0, MAX_INT].  The coordinate and the seed
//           are equally distributed.
//  Side Effect: r_a is updated to be ready to generate another
//               point.
//

	static Point3 generatePoint (unsigned int& r_a)
	{
		static const float SCALE_TO_01 = 1.0f / (~0u + 1.0f);

		Point3 result;
		result.m_x    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_y    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_z    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_seed = calculateNextPseudorandom(r_a);
		return result;
	}

public:
//
//  Default Constructor
//
//  Purpose: To create a new WorleyPoint3 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint3 is created with default seed
//               values.
//

	WorleyPoint3 ();

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint3 with the specified
//           seed values.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The directionless seed values
//    <4> seed_x1
//    <5> seed_x2: The X coordinate seed values
//    <6> seed_y1
//    <7> seed_y2: The Y coordinate seed values
//    <8> seed_z1
//    <9> seed_z2: The Z coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint1 is created with a
//               directionless seed values of seed0, seed1, and
//               seed2, X coordinate seed values of seed_x1 and
//               seed_x2, Y coordinate seed values of seed_y1
//               and seed_y2, and Z coordinate seed values of
//               seed_z1 and seed_z2.
//

	WorleyPoint3 (unsigned int seed0, 
	              unsigned int seed1, 
	              unsigned int seed2,
	              unsigned int seed_x1, 
	              unsigned int seed_x2,
	              unsigned int seed_y1, 
	              unsigned int seed_y2,
	              unsigned int seed_z1, 
	              unsigned int seed_z2);

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint3 with the specified
//           seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 9 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new WorleyPoint3 is created with the first
//               SEED_COUNT = 9 values in a_seeds as seed
//               values.
//

	WorleyPoint3 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new WorleyPoint3 with the same seed
//           values as another.
//  Parameter(s):
//    <1> original: The WorleyPoint3 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint3 is created with a the same
//               seed values as original.
//

	WorleyPoint3 (const WorleyPoint3& original);

//
//  Destructor
//
//  Purpose: To safely destroy a WorleyPoint3 without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~WorleyPoint3 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this WorleyPoint3 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The WorleyPoint3 to copy
//  Precondition(s): N/A
//  Returns: A reference to this WorleyPoint3.
//  Side Effect: This WorleyPoint3 is set to have the same seed
//               values as original.
//

	WorleyPoint3& operator= (const WorleyPoint3& original);

//
//  getPoints
//
//  Purpose: To determine the positions of the first points in
//           the specified cell.
//  Parameter(s):
//    <1> count: How many point positions
//    <2> x
//    <3> y
//    <4> z: The cell coordinate
//  Precondition(s): N/A
//  Returns: An std::vector containing the data for the first
//           count points in cell (x, y, z).  The position
//           returned is in global space, not the position
//           within the cell.  For example, cell (2, 5, 8) would
//           have an X coordinate in the range [2, 3), a Y
//           coordinate in the range [5, 6), and a Z coordinate
//           in the range [8, 9).
//  Side Effect: N/A
//

	std::vector<Point3> getPoints (unsigned int count,
	                              int x, int y, int z) const;
};



//
//  WorleyPoint4
//

class WorleyPoint4 : public PseudorandomGrid4
{
	//
	//  Point4
	//
	//  A record to represent the data for a single Worley point
	//    in 4-dimensional space.  The point has 4 coordinates
	//    (x, y, z, w) and an associated seed.  The seed can be
	//    used to generate additional pseudorandom properties
	//    for the point.
	//

	struct Point4
	{
		float m_x;
		float m_y;
		float m_z;
		float m_w;
		unsigned int m_seed;
	};

private:
//
//  Class Function: generatePoint
//
//  Purpose: To create a Point4 from the specified psuedorandom
//           seed.  This function is inlined for speed reasons.
//  Parameter(s):
//    <1> r_a: A reference to the seed value
//  Precondition(s): N/A
//  Returns: A Point4 calculated from r_a.  The coordinates are
//           each in the range [0, 1), and the seed is in the
//           range [0, MAX_INT].  The coordinate and the seed
//           are equally distributed.
//  Side Effect: r_a is updated to be ready to generate another
//               point.
//

	static Point4 generatePoint (unsigned int& r_a)
	{
		static const float SCALE_TO_01 = 1.0f / (~0u + 1.0f);

		Point4 result;
		result.m_x    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_y    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_z    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_w    = calculateNextPseudorandom(r_a) *
		                                            SCALE_TO_01;
		result.m_seed = calculateNextPseudorandom(r_a);
		return result;
	}

public:
//
//  Default Constructor
//
//  Purpose: To create a new WorleyPoint4 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint4 is created with default seed
//               values.
//

	WorleyPoint4 ();

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint4 with the specified
//           seed values.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The directionless seed values
//    <4> seed_x1
//    <5> seed_x2: The X coordinate seed values
//    <6> seed_y1
//    <7> seed_y2: The Y coordinate seed values
//    <8> seed_z1
//    <9> seed_z2: The Z coordinate seed values
//    <10>seed_w1
//    <11>seed_w2: The W coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint1 is created with a
//               directionless seed values of seed0, seed1, and
//               seed2, X coordinate seed values of seed_x1 and
//               seed_x2, Y coordinate seed values of seed_y1
//               and seed_y2, Z coordinate seed values of
//               seed_z1 and seed_z2, and W coordinate seed
//               values of seed_w1 and seed_w2.
//

	WorleyPoint4 (unsigned int seed0, 
	              unsigned int seed1, 
	              unsigned int seed2,
	              unsigned int seed_x1, 
	              unsigned int seed_x2,
	              unsigned int seed_y1, 
	              unsigned int seed_y2,
	              unsigned int seed_z1, 
	              unsigned int seed_z2,
	              unsigned int seed_w1, 
	              unsigned int seed_w2);

//
//  Constructor
//
//  Purpose: To create a new WorleyPoint4 with the specified
//           seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 11 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new WorleyPoint4 is created with the first
//               SEED_COUNT = 11 values in a_seeds as seed
//               values.
//

	WorleyPoint4 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new WorleyPoint4 with the same seed
//           values as another.
//  Parameter(s):
//    <1> original: The WorleyPoint4 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new WorleyPoint4 is created with a the same
//               seed values as original.
//

	WorleyPoint4 (const WorleyPoint4& original);

//
//  Destructor
//
//  Purpose: To safely destroy a WorleyPoint4 without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~WorleyPoint4 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this WorleyPoint4 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The WorleyPoint4 to copy
//  Precondition(s): N/A
//  Returns: A reference to this WorleyPoint4.
//  Side Effect: This WorleyPoint4 is set to have the same seed
//               values as original.
//

	WorleyPoint4& operator= (const WorleyPoint4& original);

//
//  getPoints
//
//  Purpose: To determine the positions of the first points in
//           the specified cell.
//  Parameter(s):
//    <1> count: How many point positions
//    <2> x
//    <3> y
//    <4> z
//    <5> w: The cell coordinate
//  Precondition(s): N/A
//  Returns: An std::vector containing the data for the first
//           count points in cell (x, y, z, w).  The position
//           returned is in global space, not the position
//           within the cell.  For example, cell (2, 5, 8, 11)
//           would have an X coordinate in the range [2, 3), a Y
//           coordinate in the range [5, 6), a Z coordinate in
//           the range [8, 9), and a W coordinate in the range
//           [11, 12).
//  Side Effect: N/A
//

	std::vector<Point4> getPoints (unsigned int count,
	                              int x, int y, int z, int w)
	                                                      const;
};



#endif

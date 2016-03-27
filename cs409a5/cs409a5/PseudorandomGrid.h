//
//  PseudorandomGrid.h
//
//  A module to generate consistant pseudorandom values in an
//    infinite grid of 1 to 4 dimensions.
//

#ifndef RANDOM_GRID_H
#define RANDOM_GRID_H

#include <vector>



//
//  PseudorandomGrid*
//
//  A group of classes to generate pseudorandom values for
//    infinite grids of 1 to 4 dimensions.  These values can be
//    used for procedural noise generation, including value,
//    Perlin, and Worley noise.
//
//  The grid has values spaced in a regular square lattice.  The
//    value at each lattice point is a psuedorandom unsigned
//    integer the range [0, 2^32), corresponding to all possible
//    values for a 32-bit int.  The values are calculated from
//    the position and the seed values and can be efficently
//    calculated independantly.
//
//  There are 3 quadratic seed values and two additional seed
//    values for each dimension.  The value at a lattice point
//    is calculated in three parts.  First, the position and the
//    first of the seed values for each dimension are combined
//    to form a single position value.  Second, this position
//    value is combined with the quadratic seeds.  Thirdly, the
//    result of this is combined with the combination of each
//    coordinate and the other seed value for that dimension.
//
//  For example, the value for a lattice point in a 2D grid is:
//
//    n = (m_seed_x1 * x) ^ (m_seed_x2 * y);
//    value = (((m_seed2 * n) ^ m_seed1) * n) ^ m_seed0 ^
//            (x * m_seed_x2) ^
//            (y * m_seed_y2);
//
//    where x and y are the coordinates, m_seed0, m_seed1, and
//    m_seed2 are the constant seed values, and m_seed_*1 and
//    m_seed_*2 values are the first and second seed value for
//    each coordinate.
//
//  Seed values are integers filled with random bits.  They can
//    be signed or unsigned data, but are treated as unsigned
//    internally.  A sample of seed values are included in the
//    C++ file, along with instructions for generating more.
//



//
//  PseudorandomGridSuperclass
//
//  A superclass to generate pseudorandom values for infinite
//    grids.  This classes contains the parts of the generation
//    system that are independant of the number of dimensions.
//
//  This class can be instantiated, but has no useful function
//    on its own.
//

class PseudorandomGridSuperclass
{
public:
//
//  SEED0_DEFAULT
//  SEED1_DEFAULT
//  SEED2_DEFAULT
//
//  The default seed values for the coordinate.
//

	static const unsigned int SEED0_DEFAULT;
	static const unsigned int SEED1_DEFAULT;
	static const unsigned int SEED2_DEFAULT;

public:
//
//  calculateNextPseudorandom
//
//  Purpose: To calculate the next pseudorandom value from the
//           specified value.  The provided value is also
//           updated.  This function is not used in generating
//           the grid of values, but is included because it is
//           often of value to classes using the grid of values.
//  Parameter(s):
//    <1> r_a: A reference to the current value
//  Precondition(s): N/A
//  Returns: The next pseudorandom value.
//  Side Effect: r_a is updated to the generated value.
//
//  http://www.jstatsoft.org/v08/i14/paper
//

	static unsigned int calculateNextPseudorandom (
	                                          unsigned int& r_a)
	{
		r_a ^= r_a << 13;
		r_a ^= r_a >> 17;
		r_a ^= r_a << 5;
		return r_a;
	}

public:
//
//  Default Constructor
//
//  Purpose: To create a new PseudorandomGridSuperclass with
//           default quadratic seed values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGridSuperclass is created
//               with default quadratic seed values.
//

	PseudorandomGridSuperclass ();

//
//  Constructor
//
//  Purpose: To create a new PseudorandomGridSuperclass with the
//           specified quadratic seed values.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The directionless seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGridSuperclass is created
//               with quadratic seed values of seed0, seed1, and
//               seed2.
//

	PseudorandomGridSuperclass (unsigned int seed0,
	                            unsigned int seed1,
	                            unsigned int seed2);

//
//  Copy Constructor
//
//  Purpose: To create a new PseudorandomGridSuperclass with the
//           same quadratic seed values as another.
//  Parameter(s):
//    <1> original: The PseudorandomGridSuperclass to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGridSuperclass is created
//               with a the same quadratic seed values as
//               original.
//

	PseudorandomGridSuperclass (
	                const PseudorandomGridSuperclass& original);

//
//  Destructor
//
//  Purpose: To safely destroy a PseudorandomGridSuperclass
//           without memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~PseudorandomGridSuperclass ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this PseudorandomGridSuperclass to be a
//           copy of another.
//  Parameter(s):
//    <1> original: The PseudorandomGridSuperclass to copy
//  Precondition(s): N/A
//  Returns: A reference to this PseudorandomGridSuperclass.
//  Side Effect: This PseudorandomGridSuperclass is set to have
//               the same quadratic seed values as original.
//

	PseudorandomGridSuperclass& operator= (
	                const PseudorandomGridSuperclass& original);

//
//  getSeedQuadratic0
//  getSeedQuadratic1
//  getSeedQuadratic2
//
//  Purpose: To determine the first/second/third quadratic seed
//           value.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The first/second/third quadratic seed value.
//  Side Effect: N/A
//

	unsigned int getSeedQuadratic0 () const
	{ return m_seed0; }
	unsigned int getSeedQuadratic1 () const
	{ return m_seed1; }
	unsigned int getSeedQuadratic2 () const
	{ return m_seed2; }

//
//  setSeedsQuadratic
//
//  Purpose: To change the quadratic seed values.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The quadratic seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGridSuperclass is set to have
//               quadratic seed values of seed0, seed1, and
//               seed2.
//

	void setSeedsQuadratic (unsigned int seed0,
	                        unsigned int seed1,
	                        unsigned int seed2);

protected:
	unsigned int m_seed0;
	unsigned int m_seed1;
	unsigned int m_seed2;
};



//
//  PseudorandomGrid1
//

class PseudorandomGrid1 : public PseudorandomGridSuperclass
{
public:
//
//  SEED_COUNT
//
//  The number of random seeds needed to initialize a
//    PseudorandomGrid1.
//

	static const unsigned int SEED_COUNT = 5;

//
//  SEED_X1_DEFAULT
//  SEED_X2_DEFAULT
//
//  The default seed values for the coordinate.
//

	static const unsigned int SEED_X1_DEFAULT;
	static const unsigned int SEED_X2_DEFAULT;

public:
//
//  Default Constructor
//
//  Purpose: To create a new PseudorandomGrid1 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid1 is created with default
//               seed values.
//

	PseudorandomGrid1 ();

//
//  Constructor
//
//  Purpose: To create a new PseudorandomGrid1 with the
//           specified seed values.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The directionless seed values
//    <4> seed_x1
//    <5> seed_x2: The coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid1 is created with
//               quadratic seed values of seed0, seed1 and
//               coordinate seed values of seed2, seed_x1, and
//               seed_x2.
//

	PseudorandomGrid1 (unsigned int seed0,
	                   unsigned int seed1,
	                   unsigned int seed2,
	                   unsigned int seed_x1,
	                   unsigned int seed_x2);

//
//  Constructor
//
//  Purpose: To create a new PseudorandomGrid1 with the
//           specified seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 5 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid1 is created with the
//               first SEED_COUNT = 5 values in a_seeds as seed
//               values.
//

	PseudorandomGrid1 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new PseudorandomGrid1 with the same
//           seed values as another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid1 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid1 is created with a the
//               same seed values as original.
//

	PseudorandomGrid1 (const PseudorandomGrid1& original);

//
//  Destructor
//
//  Purpose: To safely destroy a PseudorandomGrid1 without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~PseudorandomGrid1 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this PseudorandomGrid1 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid1 to copy
//  Precondition(s): N/A
//  Returns: A reference to this PseudorandomGrid1.
//  Side Effect: This PseudorandomGrid1 is set to have the same
//               seed values as original.
//

	PseudorandomGrid1& operator= (
	                         const PseudorandomGrid1& original);

//
//  getSeedX1
//  getSeedX2
//
//  Purpose: To determine the first/second seed value for the
//           coordinate.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The first/second coordinate seed value.
//  Side Effect: N/A
//

	unsigned int getSeedX1 () const
	{ return m_seed_x1; }
	unsigned int getSeedX2 () const
	{ return m_seed_x2; }

//
//  getAt
//
//  Purpose: To determine the pseudorandom value for the
//           specified cell.  This function is inlined for speed
//           reasons.
//  Parameter(s):
//    <1> x: The cell coordinate
//  Precondition(s): N/A
//  Returns: The pseudorandom value for cell (x).
//  Side Effect: N/A
//

	unsigned int getAt (int x) const
	{
		unsigned int n = m_seed_x1 * x;
		return (((m_seed2 * n) ^ m_seed1) * n) ^ m_seed0 ^
		       (x * m_seed_x2);
	}

//
//  setSeedsX
//
//  Purpose: To change the seed values for the coordinate.
//  Parameter(s):
//    <1> seed_x1
//    <2> seed_x2: The coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid1 is set to have
//               coordinate seed values of seed_x1, and seed_x2.
//

	void setSeedsX (unsigned int seed_x1, 
	                unsigned int seed_x2);

//
//  setSeedsAll
//
//  Purpose: To change all the seed values for this
//           PseudorandomGrid1.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The quadratic seed values
//    <4> seed_x1
//    <5> seed_x2: The coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid1 is set to have quadratic
//               seed values of seed0, seed1, and seed2, and to
//               have coordinate seed values of seed_x1, and
//               seed_x2.
//

	void setSeedsAll (unsigned int seed0,
	                  unsigned int seed1,
	                  unsigned int seed2,
	                  unsigned int seed_x1,
	                  unsigned int seed_x2);

//
//  setSeedsAll
//
//  Purpose: To change all the seed values for this
//           PseudorandomGrid1.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 5 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: This PseudorandomGrid1 is set to have the first
//               SEED_COUNT = 5 values in a_seeds as seed
//               values.
//

	void setSeedsAll (const unsigned int a_seeds[]);

private:
	unsigned int m_seed_x1;
	unsigned int m_seed_x2;
};



//
//  PseudorandomGrid2
//

class PseudorandomGrid2 : public PseudorandomGridSuperclass
{
public:
//
//  SEED_COUNT
//
//  The number of random seeds needed to initialize a
//    PseudorandomGrid2.
//

	static const unsigned int SEED_COUNT = 7;

//
//  SEED_X1_DEFAULT
//  SEED_X2_DEFAULT
//  SEED_Y1_DEFAULT
//  SEED_Y2_DEFAULT
//
//  The default seed values for the X/Y coordinate.
//

	static const unsigned int SEED_X1_DEFAULT;
	static const unsigned int SEED_X2_DEFAULT;
	static const unsigned int SEED_Y1_DEFAULT;
	static const unsigned int SEED_Y2_DEFAULT;

public:
//
//  Default Constructor
//
//  Purpose: To create a new PseudorandomGrid2 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid2 is created with default
//               seed values.
//

	PseudorandomGrid2 ();

//
//  Constructor
//
//  Purpose: To create a new PseudorandomGrid2 with the
//           specified seed values.
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
//  Side Effect: A new PseudorandomGrid2 is created with a
//               directionless seed values of seed0, seed1, and
//               seed2, X coordinate seed values of seed_x1 and
//               seed_x2, and Y coordinate seed values of
//               seed_y1 and seed_y2.
//

	PseudorandomGrid2 (unsigned int seed0,
	                   unsigned int seed1,
	                   unsigned int seed2,
	                   unsigned int seed_x1,
	                   unsigned int seed_x2,
	                   unsigned int seed_y1,
	                   unsigned int seed_y2);

//
//  Constructor
//
//  Purpose: To create a new PseudorandomGrid2 with the
//           specified seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 7 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid2 is created with the
//               first SEED_COUNT = 7 values in a_seeds as seed
//               values.
//

	PseudorandomGrid2 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new PseudorandomGrid2 with the same
//           seed values as another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid2 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid2 is created with a the
//               same seed values as original.
//

	PseudorandomGrid2 (const PseudorandomGrid2& original);

//
//  Destructor
//
//  Purpose: To safely destroy a PseudorandomGrid2 without
//           memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~PseudorandomGrid2 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this PseudorandomGrid2 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid2 to copy
//  Precondition(s): N/A
//  Returns: A reference to this PseudorandomGrid2.
//  Side Effect: This PseudorandomGrid2 is set to have the same
//               seed values as original.
//

	PseudorandomGrid2& operator= (const PseudorandomGrid2& original);

//
//  getSeedX1
//  getSeedX2
//  getSeedY1
//  getSeedY2
//
//  Purpose: To determine the first/second seed value for the
//           X/Y coordinate.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The first/second X/Y coordinate seed value.
//  Side Effect: N/A
//

	unsigned int getSeedX1 () const
	{ return m_seed_x1; }
	unsigned int getSeedX2 () const
	{ return m_seed_x2; }
	unsigned int getSeedY1 () const
	{ return m_seed_y1; }
	unsigned int getSeedY2 () const
	{ return m_seed_y2; }

//
//  getAt
//
//  Purpose: To determine the pseudorandom value for the
//           specified cell.  This function is inlined for speed
//           reasons.
//  Parameter(s):
//    <1> x
//    <2> y: The cell coordinates
//  Precondition(s): N/A
//  Returns: The pseudorandom value for cell (x, y).
//  Side Effect: N/A
//

	unsigned int getAt (int x, int y) const
	{
		unsigned int n = (m_seed_x1 * x) ^
		                 (m_seed_y1 * y);
		return (((m_seed2 * n) ^ m_seed1) * n) ^ m_seed0 ^
		       (x * m_seed_x2) ^
		       (y * m_seed_y2);
	}

//
//  setSeedsX
//
//  Purpose: To change the seed values for the X coordinate.
//  Parameter(s):
//    <1> seed_x1
//    <2> seed_x2: The X coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid2 is set to have X
//               coordinate seed values of seed_x1 and seed_x2.
//

	void setSeedsX (unsigned int seed_x1,
	                unsigned int seed_x2);

//
//  setSeedsY
//
//  Purpose: To change the seed values for the Y coordinate.
//  Parameter(s):
//    <1> seed_y1
//    <2> seed_y2: The Y coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid2 is set to have Y
//               coordinate seed values of seed_y1 and seed_y2.
//

	void setSeedsY (unsigned int seed_y1,
	                unsigned int seed_y2);

//
//  setSeedsAll
//
//  Purpose: To change all the seed values for this
//           PseudorandomGrid2.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The quadratic seed values
//    <4> seed_x1
//    <5> seed_x2: The X coordinate seed values
//    <6> seed_y1
//    <7> seed_y2: The Y coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid2 is set to have quadratic
//               seed values of seed0, seed1, and seed2, to have
//               X coordinate seed values of seed_x1 and
//               seed_x2, and to have Y coordinate seed values
//               of seed_y1 and seed_y2.
//

	void setSeedsAll (unsigned int seed0, 
	                  unsigned int seed1, 
	                  unsigned int seed2,
	                  unsigned int seed_x1, 
	                  unsigned int seed_x2,
	                  unsigned int seed_y1, 
	                  unsigned int seed_y2);

//
//  setSeedsAll
//
//  Purpose: To change all the seed values for this
//           PseudorandomGrid2.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 7 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: This PseudorandomGrid2 is set to have the first
//               SEED_COUNT = 7 values in a_seeds as seed
//               values.
//

	void setSeedsAll (const unsigned int a_seeds[]);

private:
	unsigned int m_seed_x1;
	unsigned int m_seed_x2;
	unsigned int m_seed_y1;
	unsigned int m_seed_y2;
};



//
//  PseudorandomGrid3
//

class PseudorandomGrid3 : public PseudorandomGridSuperclass
{
public:
//
//  SEED_COUNT
//
//  The number of random seeds needed to initialize a
//    PseudorandomGrid3.
//

	static const unsigned int SEED_COUNT = 9;

//
//  SEED_X1_DEFAULT
//  SEED_X2_DEFAULT
//  SEED_Y1_DEFAULT
//  SEED_Y2_DEFAULT
//  SEED_Z1_DEFAULT
//  SEED_Z2_DEFAULT
//
//  The default seed values for the X/Y/Z coordinate.
//

	static const unsigned int SEED_X1_DEFAULT;
	static const unsigned int SEED_X2_DEFAULT;
	static const unsigned int SEED_Y1_DEFAULT;
	static const unsigned int SEED_Y2_DEFAULT;
	static const unsigned int SEED_Z1_DEFAULT;
	static const unsigned int SEED_Z2_DEFAULT;

public:
//
//  Default Constructor
//
//  Purpose: To create a new PseudorandomGrid3 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid3 is created with default
//               seed values.
//

	PseudorandomGrid3 ();

//
//  Constructor
//
//  Purpose: To create a new PseudorandomGrid3 with the
//           specified seed values.
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
//  Side Effect: A new PseudorandomGrid3 is created with a
//               directionless seed values of seed0, seed1, and
//               seed2, X coordinate seed values of seed_x1 and
//               seed_x2, Y coordinate seed values of seed_y1
//               and seed_y2, and Z coordinate seed values of
//               seed_z1 and seed_z2.
//

	PseudorandomGrid3 (unsigned int seed0, 
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
//  Purpose: To create a new PseudorandomGrid3 with the
//           specified seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 9 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid3 is created with the
//               first SEED_COUNT = 9 values in a_seeds as seed
//               values.
//

	PseudorandomGrid3 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new PseudorandomGrid3 with the same
//           seed values as another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid3 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid3 is created with a the
//               same seed values as original.
//

	PseudorandomGrid3 (const PseudorandomGrid3& original);

//
//  Destructor
//
//  Purpose: To safely destroy a PseudorandomGrid3 without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~PseudorandomGrid3 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this PseudorandomGrid3 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid3 to copy
//  Precondition(s): N/A
//  Returns: A reference to this PseudorandomGrid3.
//  Side Effect: This PseudorandomGrid3 is set to have the same
//               seed values as original.
//

	PseudorandomGrid3& operator= (
	                         const PseudorandomGrid3& original);

//
//  getSeedX1
//  getSeedX2
//  getSeedY1
//  getSeedY2
//  getSeedZ1
//  getSeedZ2
//
//  Purpose: To determine the first/second seed value for the
//           X/Y/Z coordinate.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The first/second X/Y/Z coordinate seed value.
//  Side Effect: N/A
//

	unsigned int getSeedX1 () const
	{ return m_seed_x1; }
	unsigned int getSeedX2 () const
	{ return m_seed_x2; }
	unsigned int getSeedY1 () const
	{ return m_seed_y1; }
	unsigned int getSeedY2 () const
	{ return m_seed_y2; }
	unsigned int getSeedZ1 () const
	{ return m_seed_z1; }
	unsigned int getSeedZ2 () const
	{ return m_seed_z2; }

//
//  getAt
//
//  Purpose: To determine the pseudorandom value for the
//           specified cell.  This function is inlined for speed
//           reasons.
//  Parameter(s):
//    <1> x
//    <2> y
//    <3> z: The cell coordinates
//  Precondition(s): N/A
//  Returns: The pseudorandom value for cell (x, y, z).
//  Side Effect: N/A
//

	unsigned int getAt (int x, int y, int z) const
	{
		unsigned int n = (m_seed_x1 * x) ^
		                 (m_seed_y1 * y) ^
		                 (m_seed_z1 * z);
		return (((m_seed2 * n) ^ m_seed1) * n) ^ m_seed0 ^
		       (x * m_seed_x2) ^
		       (y * m_seed_y2) ^
		       (z * m_seed_z2);
	}

//
//  setSeedsX
//
//  Purpose: To change the seed values for the X coordinate.
//  Parameter(s):
//    <1> seed_x1
//    <2> seed_x2: The X coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid3 is set to have X
//               coordinate seed values of seed_x1 and seed_x2.
//

	void setSeedsX (unsigned int seed_x1, 
	                unsigned int seed_x2);

//
//  setSeedsY
//
//  Purpose: To change the seed values for the Y coordinate.
//  Parameter(s):
//    <1> seed_y1
//    <2> seed_y2: The Y coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid3 is set to have Y
//               coordinate seed values of seed_y1 and seed_y2.
//

	void setSeedsY (unsigned int seed_y1, 
	                unsigned int seed_y2);

//
//  setSeedsZ
//
//  Purpose: To change the seed values for the Z coordinate.
//  Parameter(s):
//    <1> seed_z1
//    <2> seed_z2: The Z coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid3 is set to have Z
//               coordinate seed values of seed_z1 and seed_z2.
//

	void setSeedsZ (unsigned int seed_z1, 
	                unsigned int seed_z2);

//
//  setSeedsAll
//
//  Purpose: To change all the seed values for this
//           PseudorandomGrid3.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The quadratic seed values
//    <4> seed_x1
//    <5> seed_x2: The X coordinate seed values
//    <6> seed_y1
//    <7> seed_y2: The Y coordinate seed values
//    <8> seed_z1
//    <9> seed_z2: The Z coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid3 is set to have quadratic
//               seed values of seed0, seed1, and seed2, to have
//               X coordinate seed values of seed_x1 and
//               seed_x2, to have Y coordinate seed values of
//               seed_y1 and seed_y2, and to have Z coordinate
//               seed values of seed_z1 and seed_z2.
//

	void setSeedsAll (unsigned int seed0, 
	                  unsigned int seed1, 
	                  unsigned int seed2,
	                  unsigned int seed_x1, 
	                  unsigned int seed_x2,
	                  unsigned int seed_y1, 
	                  unsigned int seed_y2,
	                  unsigned int seed_z1, 
	                  unsigned int seed_z2);

//
//  setSeedsAll
//
//  Purpose: To change all the seed values for this
//           PseudorandomGrid3.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 9 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: This PseudorandomGrid3 is set to have the first
//               SEED_COUNT = 9 values in a_seeds as seed
//               values.
//

	void setSeedsAll (const unsigned int a_seeds[]);

private:
	unsigned int m_seed_x1;
	unsigned int m_seed_x2;
	unsigned int m_seed_y1;
	unsigned int m_seed_y2;
	unsigned int m_seed_z1;
	unsigned int m_seed_z2;
};



//
//  PseudorandomGrid4
//

class PseudorandomGrid4 : public PseudorandomGridSuperclass
{
public:
//
//  SEED_COUNT
//
//  The number of random seeds needed to initialize a
//    PseudorandomGrid4.
//

	static const unsigned int SEED_COUNT = 11;

//
//  SEED_X1_DEFAULT
//  SEED_X2_DEFAULT
//  SEED_Y1_DEFAULT
//  SEED_Y2_DEFAULT
//  SEED_Z1_DEFAULT
//  SEED_Z2_DEFAULT
//  SEED_W1_DEFAULT
//  SEED_W2_DEFAULT
//
//  The default seed values for the X/Y/Z/W coordinate.
//

	static const unsigned int SEED_X1_DEFAULT;
	static const unsigned int SEED_X2_DEFAULT;
	static const unsigned int SEED_Y1_DEFAULT;
	static const unsigned int SEED_Y2_DEFAULT;
	static const unsigned int SEED_Z1_DEFAULT;
	static const unsigned int SEED_Z2_DEFAULT;
	static const unsigned int SEED_W1_DEFAULT;
	static const unsigned int SEED_W2_DEFAULT;

public:
//
//  Default Constructor
//
//  Purpose: To create a new PseudorandomGrid4 with default seed
//           values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid4 is created with default
//               seed values.
//

	PseudorandomGrid4 ();

//
//  Constructor
//
//  Purpose: To create a new PseudorandomGrid4 with the
//           specified seed values.
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
//  Side Effect: A new PseudorandomGrid4 is created with a
//               directionless seed values of seed0, seed1, and
//               seed2, X coordinate seed values of seed_x1 and
//               seed_x2, Y coordinate seed values of seed_y1
//               and seed_y2, Z coordinate seed values of
//               seed_z1 and seed_z2, and W coordinate seed
//               values of seed_w1 and seed_w2.
//

	PseudorandomGrid4 (unsigned int seed0, 
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
//  Purpose: To create a new PseudorandomGrid4 with the
//           specified seed values.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 11 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid4 is created with the
//               first SEED_COUNT = 11 values in a_seeds as seed
//               values.
//

	PseudorandomGrid4 (const unsigned int a_seeds[]);

//
//  Copy Constructor
//
//  Purpose: To create a new PseudorandomGrid4 with the same
//           seed values as another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid4 to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new PseudorandomGrid4 is created with a the
//               same seed values as original.
//

	PseudorandomGrid4 (const PseudorandomGrid4& original);

//
//  Destructor
//
//  Purpose: To safely destroy a PseudorandomGrid4 without memory
//           leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	virtual ~PseudorandomGrid4 ();

//
//  Assignemnt Operator
//
//  Purpose: To modify this PseudorandomGrid4 to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The PseudorandomGrid4 to copy
//  Precondition(s): N/A
//  Returns: A reference to this PseudorandomGrid4.
//  Side Effect: This PseudorandomGrid4 is set to have the same
//               seed values as original.
//

	PseudorandomGrid4& operator= (
	                         const PseudorandomGrid4& original);

//
//  getSeedX1
//  getSeedX2
//  getSeedY1
//  getSeedY2
//  getSeedZ1
//  getSeedZ2
//  getSeedW1
//  getSeedW2
//
//  Purpose: To determine the first/second seed value for the
//           X/Y/Z/W coordinate.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The first/second X/Y/Z/W coordinate seed value.
//  Side Effect: N/A
//

	unsigned int getSeedX1 () const
	{ return m_seed_x1; }
	unsigned int getSeedX2 () const
	{ return m_seed_x2; }
	unsigned int getSeedY1 () const
	{ return m_seed_y1; }
	unsigned int getSeedY2 () const
	{ return m_seed_y2; }
	unsigned int getSeedZ1 () const
	{ return m_seed_z1; }
	unsigned int getSeedZ2 () const
	{ return m_seed_z2; }
	unsigned int getSeedW1 () const
	{ return m_seed_w1; }
	unsigned int getSeedW2 () const
	{ return m_seed_w2; }

//
//  getAt
//
//  Purpose: To determine the pseudorandom value for the
//           specified cell.  This function is inlined for speed
//           reasons.
//  Parameter(s):
//    <1> x
//    <2> y
//    <3> z
//    <4> w: The cell coordinates
//  Precondition(s): N/A
//  Returns: The pseudorandom value for cell (x, y, z, w).
//  Side Effect: N/A
//

	unsigned int getAt (int x, int y, int z, int w) const
	{
		unsigned int n = (m_seed_x1 * x) ^
		                 (m_seed_y1 * y) ^
		                 (m_seed_z1 * z) ^
		                 (m_seed_w1 * w);
		return (((m_seed2 * n) ^ m_seed1) * n) ^ m_seed0 ^
		       (x * m_seed_x2) ^
		       (y * m_seed_y2) ^
		       (z * m_seed_z2) ^
		       (w * m_seed_w2);
	}

//
//  setSeedsX
//
//  Purpose: To change the seed values for the X coordinate.
//  Parameter(s):
//    <1> seed_x1
//    <2> seed_x2: The X coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid4 is set to have X
//               coordinate seed values of seed_x1 and seed_x2.
//

	void setSeedsX (unsigned int seed_x1, 
	                unsigned int seed_x2);

//
//  setSeedsY
//
//  Purpose: To change the seed values for the Y coordinate.
//  Parameter(s):
//    <1> seed_y1
//    <2> seed_y2: The Y coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid4 is set to have Y
//               coordinate seed values of seed_y1 and seed_y2.
//

	void setSeedsY (unsigned int seed_y1, 
	                unsigned int seed_y2);

//
//  setSeedsZ
//
//  Purpose: To change the seed values for the Z coordinate.
//  Parameter(s):
//    <1> seed_z1
//    <2> seed_z2: The Z coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid4 is set to have Z
//               coordinate seed values of seed_z1 and seed_z2.
//

	void setSeedsZ (unsigned int seed_z1, 
	                unsigned int seed_z2);

//
//  setSeedsW
//
//  Purpose: To change the seed values for the W coordinate.
//  Parameter(s):
//    <1> seed_w1
//    <2> seed_w2: The W coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid4 is set to have W
//               coordinate seed values of seed_w1 and seed_w2.
//

	void setSeedsW (unsigned int seed_w1, 
	                unsigned int seed_w2);

//
//  setSeedsAll
//
//  Purpose: To change all the seed values for this
//           PseudorandomGrid4.
//  Parameter(s):
//    <1> seed0
//    <2> seed1
//    <3> seed2: The quadratic seed values
//    <4> seed_x1
//    <5> seed_x2: The X coordinate seed values
//    <6> seed_y1
//    <7> seed_y2: The Y coordinate seed values
//    <8> seed_z1
//    <9> seed_z2: The Z coordinate seed values
//    <8> seed_w1
//    <9> seed_w2: The W coordinate seed values
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This PseudorandomGrid4 is set to have quadratic
//               seed values of seed0, seed1, and seed2, to have
//               X coordinate seed values of seed_x1 and
//               seed_x2, to have Y coordinate seed values of
//               seed_y1 and seed_y2, to have Z coordinate seed
//               values of seed_z1 and seed_z2, and to have W
//               coordinate seed values of seed_w1 and seed_w2.
//

	void setSeedsAll (unsigned int seed0, 
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
//  setSeedsAll
//
//  Purpose: To change the all seed values for this
//           PseudorandomGrid4.
//  Parameter(s):
//    <1> a_seeds: An array of at least SEED_COUNT = 11 seed
//                 values
//  Precondition(s):
//    <1> a_seeds != NULL
//  Returns: N/A
//  Side Effect: This PseudorandomGrid4 is set to have the first
//               SEED_COUNT = 11 values in a_seeds as seed
//               values.
//

	void setSeedsAll (const unsigned int a_seeds[]);

private:
	unsigned int m_seed_x1;
	unsigned int m_seed_x2;
	unsigned int m_seed_y1;
	unsigned int m_seed_y2;
	unsigned int m_seed_z1;
	unsigned int m_seed_z2;
	unsigned int m_seed_w1;
	unsigned int m_seed_w2;
};



#endif

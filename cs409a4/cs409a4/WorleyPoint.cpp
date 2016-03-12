//
//  WorleyPoint.cpp
//

#include <cassert>
#include <vector>

#include "PseudorandomGrid.h"
#include "WorleyPoint.h"

using namespace std;



WorleyPoint1 :: WorleyPoint1 ()
		: PseudorandomGrid1()
{
}

WorleyPoint1 :: WorleyPoint1 (unsigned int seed0,
                              unsigned int seed1,
                              unsigned int seed2,
                              unsigned int seed_x1,
                              unsigned int seed_x2)
		: PseudorandomGrid1(seed0, seed1, seed2,
		                    seed_x1, seed_x2)
{
}

WorleyPoint1 :: WorleyPoint1 (const unsigned int a_seeds[])
		 : PseudorandomGrid1(a_seeds)
{
	assert(a_seeds != NULL);
}

WorleyPoint1 :: WorleyPoint1 (const WorleyPoint1& original)
		 : PseudorandomGrid1(original)
{
}

WorleyPoint1 :: ~WorleyPoint1 ()
{
}

WorleyPoint1& WorleyPoint1 :: operator= (const WorleyPoint1& original)
{
	PseudorandomGrid1::operator=(original);
	return *this;
}



vector<WorleyPoint1::Point1> WorleyPoint1 :: getPoints (unsigned int count,
                                                        int x) const
{
	vector<WorleyPoint1::Point1> v_results(count);
	unsigned int a = getAt(x);

	for(unsigned int i = 0; i < count; i++)
	{
		v_results[i] = generatePoint(a);
		v_results[i].m_x += x;
	}

	return v_results;
}





WorleyPoint2 :: WorleyPoint2 ()
		: PseudorandomGrid2()
{
}

WorleyPoint2 :: WorleyPoint2 (unsigned int seed0,
                              unsigned int seed1,
                              unsigned int seed2,
                              unsigned int seed_x1,
                              unsigned int seed_x2,
                              unsigned int seed_y1,
                              unsigned int seed_y2)
		: PseudorandomGrid2(seed0, seed1, seed2,
		                    seed_x1, seed_x2,
		                    seed_y1, seed_y2)
{
}

WorleyPoint2 :: WorleyPoint2 (const unsigned int a_seeds[])
		 : PseudorandomGrid2(a_seeds)
{
	assert(a_seeds != NULL);
}

WorleyPoint2 :: WorleyPoint2 (const WorleyPoint2& original)
		 : PseudorandomGrid2(original)
{
}

WorleyPoint2 :: ~WorleyPoint2 ()
{
}

WorleyPoint2& WorleyPoint2 :: operator= (const WorleyPoint2& original)
{
	PseudorandomGrid2::operator=(original);
	return *this;
}



vector<WorleyPoint2::Point2> WorleyPoint2 :: getPoints (unsigned int count,
                                                        int x, int y) const
{
	vector<WorleyPoint2::Point2> v_results(count);
	unsigned int a = getAt(x, y);

	for(unsigned int i = 0; i < count; i++)
	{
		v_results[i] = generatePoint(a);
		v_results[i].m_x += x;
		v_results[i].m_y += y;
	}

	return v_results;
}





WorleyPoint3 :: WorleyPoint3 ()
		: PseudorandomGrid3()
{
}

WorleyPoint3 :: WorleyPoint3 (unsigned int seed0,
                              unsigned int seed1,
                              unsigned int seed2,
                              unsigned int seed_x1,
                              unsigned int seed_x2,
                              unsigned int seed_y1,
                              unsigned int seed_y2,
                              unsigned int seed_z1,
                              unsigned int seed_z2)
		: PseudorandomGrid3(seed0, seed1, seed2,
		                    seed_x1, seed_x2,
		                    seed_y1, seed_y2,
		                    seed_z1, seed_z2)
{
}

WorleyPoint3 :: WorleyPoint3 (const unsigned int a_seeds[])
		 : PseudorandomGrid3(a_seeds)
{
	assert(a_seeds != NULL);
}

WorleyPoint3 :: WorleyPoint3 (const WorleyPoint3& original)
		 : PseudorandomGrid3(original)
{
}

WorleyPoint3 :: ~WorleyPoint3 ()
{
}

WorleyPoint3& WorleyPoint3 :: operator= (const WorleyPoint3& original)
{
	PseudorandomGrid3::operator=(original);
	return *this;
}



vector<WorleyPoint3::Point3> WorleyPoint3 :: getPoints (unsigned int count,
                                                        int x, int y, int z) const
{
	vector<WorleyPoint3::Point3> v_results(count);
	unsigned int a = getAt(x, y, z);

	for(unsigned int i = 0; i < count; i++)
	{
		v_results[i] = generatePoint(a);
		v_results[i].m_x += x;
		v_results[i].m_y += y;
		v_results[i].m_z += z;
	}

	return v_results;
}





WorleyPoint4 :: WorleyPoint4 ()
		: PseudorandomGrid4()
{
}

WorleyPoint4 :: WorleyPoint4 (unsigned int seed0,
                              unsigned int seed1,
                              unsigned int seed2,
                              unsigned int seed_x1,
                              unsigned int seed_x2,
                              unsigned int seed_y1,
                              unsigned int seed_y2,
                              unsigned int seed_z1,
                              unsigned int seed_z2,
                              unsigned int seed_w1,
                              unsigned int seed_w2)
		: PseudorandomGrid4(seed0, seed1, seed2,
		                    seed_x1, seed_x2,
		                    seed_y1, seed_y2,
		                    seed_z1, seed_z2,
		                    seed_w1, seed_w2)
{
}

WorleyPoint4 :: WorleyPoint4 (const unsigned int a_seeds[])
		 : PseudorandomGrid4(a_seeds)
{
	assert(a_seeds != NULL);
}

WorleyPoint4 :: WorleyPoint4 (const WorleyPoint4& original)
		 : PseudorandomGrid4(original)
{
}

WorleyPoint4 :: ~WorleyPoint4 ()
{
}

WorleyPoint4& WorleyPoint4 :: operator= (const WorleyPoint4& original)
{
	PseudorandomGrid4::operator=(original);
	return *this;
}



vector<WorleyPoint4::Point4> WorleyPoint4 :: getPoints (unsigned int count,
                                                        int x, int y, int z, int w) const
{
	vector<WorleyPoint4::Point4> v_results(count);
	unsigned int a = getAt(x, y, z, w);

	for(unsigned int i = 0; i < count; i++)
	{
		v_results[i] = generatePoint(a);
		v_results[i].m_x += x;
		v_results[i].m_y += y;
		v_results[i].m_z += z;
		v_results[i].m_w += w;
	}

	return v_results;
}


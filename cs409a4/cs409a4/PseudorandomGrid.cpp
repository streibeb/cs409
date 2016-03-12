//
//  PseudorandomGrid.cpp
//

#include <cassert>
#include <vector>

#include "PseudorandomGrid.h"

using namespace std;



//
//  To generate random seeds
//
//  1.  Generate (100 pairs of) 2 16-bit random integers on the same line
//        http://www.random.org/integers/?num=100&min=1&max=65535&col=2&base=16&format=html&rnd=new
//  2.  Regex-find-and-replace
//        ^{[0-9a-f]+}	{[0-9a-f]+}
//      with
//        0x\1\2
//
/*
0x6aa5c142
0x2277abbe
0x4cc85ec3
0x70ae6130
0x0e13c9a3
0x34263713
0x9750b680
0xb1f91d65
0xb5e01350
0xaacdf50f
0x4765cf99
0x65d22872
0xeb33b18e
0x9e44a5d9
0x765cf210
0x30879c40
0x52e2ff27
0xad4665d2
0x83720cec
0x1d7203c5
0xae4329aa
0x506a8fc3
0xdb7c15bc
0x6586204f
0x9cdcac47
*/

const unsigned int PseudorandomGridSuperclass :: SEED0_DEFAULT = 0xf1fbc75f;
const unsigned int PseudorandomGridSuperclass :: SEED1_DEFAULT = 0xd8ba099c;
const unsigned int PseudorandomGridSuperclass :: SEED2_DEFAULT = 0xaddf0d81;

const unsigned int PseudorandomGrid1 :: SEED_X1_DEFAULT = 0x89705ede;
const unsigned int PseudorandomGrid1 :: SEED_X2_DEFAULT = 0xddc2c9ad;

const unsigned int PseudorandomGrid2 :: SEED_X1_DEFAULT = 0x0916402e;
const unsigned int PseudorandomGrid2 :: SEED_X2_DEFAULT = 0x977cb7a1;
const unsigned int PseudorandomGrid2 :: SEED_Y1_DEFAULT = 0x4aa37110;
const unsigned int PseudorandomGrid2 :: SEED_Y2_DEFAULT = 0x50b5ca14;

const unsigned int PseudorandomGrid3 :: SEED_X1_DEFAULT = 0x7bd0c388;
const unsigned int PseudorandomGrid3 :: SEED_X2_DEFAULT = 0xdba9ddde;
const unsigned int PseudorandomGrid3 :: SEED_Y1_DEFAULT = 0x6b288355;
const unsigned int PseudorandomGrid3 :: SEED_Y2_DEFAULT = 0x2a720adc;
const unsigned int PseudorandomGrid3 :: SEED_Z1_DEFAULT = 0x3e053df9;
const unsigned int PseudorandomGrid3 :: SEED_Z2_DEFAULT = 0xda74198f;

const unsigned int PseudorandomGrid4 :: SEED_X1_DEFAULT = 0x8081cb68;
const unsigned int PseudorandomGrid4 :: SEED_X2_DEFAULT = 0x5e1db5f4;
const unsigned int PseudorandomGrid4 :: SEED_Y1_DEFAULT = 0x9bdbf435;
const unsigned int PseudorandomGrid4 :: SEED_Y2_DEFAULT = 0x94b31bdd;
const unsigned int PseudorandomGrid4 :: SEED_Z1_DEFAULT = 0xc62630e2;
const unsigned int PseudorandomGrid4 :: SEED_Z2_DEFAULT = 0x861bf2cc;
const unsigned int PseudorandomGrid4 :: SEED_W1_DEFAULT = 0xd81e8811;
const unsigned int PseudorandomGrid4 :: SEED_W2_DEFAULT = 0xccc3775a;



PseudorandomGridSuperclass :: PseudorandomGridSuperclass ()
		: m_seed0(SEED0_DEFAULT),
		  m_seed1(SEED1_DEFAULT),
		  m_seed2(SEED2_DEFAULT)
{
}

PseudorandomGridSuperclass :: PseudorandomGridSuperclass (unsigned int seed0,
                                                          unsigned int seed1,
                                                          unsigned int seed2)
		: m_seed0(seed0),
		  m_seed1(seed1),
		  m_seed2(seed2)
{
}

PseudorandomGridSuperclass :: PseudorandomGridSuperclass (
                           const PseudorandomGridSuperclass& original)
		: m_seed0(original.m_seed0),
		  m_seed1(original.m_seed1),
		  m_seed2(original.m_seed2)
{
}

PseudorandomGridSuperclass :: ~PseudorandomGridSuperclass ()
{
}

PseudorandomGridSuperclass& PseudorandomGridSuperclass :: operator= (
                           const PseudorandomGridSuperclass& original)
{
	if(&original != this)
	{
		m_seed0 = original.m_seed0;
		m_seed1 = original.m_seed1;
		m_seed2 = original.m_seed2;
	}
	return *this;
}



void PseudorandomGridSuperclass :: setSeedsQuadratic (unsigned int seed0,
                                                      unsigned int seed1,
                                                      unsigned int seed2)
{
	m_seed0 = seed0;
	m_seed1 = seed1;
	m_seed2 = seed2;
}





PseudorandomGrid1 :: PseudorandomGrid1 ()
		: PseudorandomGridSuperclass(),
		  m_seed_x1(SEED_X1_DEFAULT),
		  m_seed_x2(SEED_X2_DEFAULT)
{
}

PseudorandomGrid1 :: PseudorandomGrid1 (unsigned int seed0,
                                        unsigned int seed1,
                                        unsigned int seed2,
                                        unsigned int seed_x1,
                                        unsigned int seed_x2)
		: PseudorandomGridSuperclass(seed0, seed1, seed2),
		  m_seed_x1(seed_x1),
		  m_seed_x2(seed_x2)
{
}

PseudorandomGrid1 :: PseudorandomGrid1 (const unsigned int a_seeds[])
		: PseudorandomGridSuperclass(),
		  m_seed_x1(),
		  m_seed_x2()
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
}

PseudorandomGrid1 :: PseudorandomGrid1 (const PseudorandomGrid1& original)
		 : PseudorandomGridSuperclass(original),
		  m_seed_x1(original.m_seed_x1),
		  m_seed_x2(original.m_seed_x2)
{
}

PseudorandomGrid1 :: ~PseudorandomGrid1 ()
{
}

PseudorandomGrid1& PseudorandomGrid1 :: operator= (const PseudorandomGrid1& original)
{
	if(&original != this)
	{
		PseudorandomGridSuperclass::operator=(original);

		m_seed_x1 = original.m_seed_x1;
		m_seed_x2 = original.m_seed_x2;
	}
	return *this;
}



void PseudorandomGrid1 :: setSeedsX (unsigned int seed_x1,
                                     unsigned int seed_x2)
{
	m_seed_x1 = seed_x1;
	m_seed_x2 = seed_x2;
}

void PseudorandomGrid1 :: setSeedsAll (unsigned int seed0,
                                       unsigned int seed1,
                                       unsigned int seed2,
                                       unsigned int seed_x1,
                                       unsigned int seed_x2)
{
	setSeedsQuadratic(seed0, seed1, seed2);
	setSeedsX(seed_x1, seed_x2);
}

void PseudorandomGrid1 :: setSeedsAll (const unsigned int a_seeds[])
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
}





PseudorandomGrid2 :: PseudorandomGrid2 ()
		: PseudorandomGridSuperclass(),
		  m_seed_x1(SEED_X1_DEFAULT),
		  m_seed_x2(SEED_X2_DEFAULT),
		  m_seed_y1(SEED_Y1_DEFAULT),
		  m_seed_y2(SEED_Y2_DEFAULT)
{
}

PseudorandomGrid2 :: PseudorandomGrid2 (unsigned int seed0,
                                        unsigned int seed1,
                                        unsigned int seed2,
                                        unsigned int seed_x1,
                                        unsigned int seed_x2,
                                        unsigned int seed_y1,
                                        unsigned int seed_y2)
		: PseudorandomGridSuperclass(seed0, seed1, seed2),
		  m_seed_x1(seed_x1),
		  m_seed_x2(seed_x2),
		  m_seed_y1(seed_y1),
		  m_seed_y2(seed_y2)
{
}

PseudorandomGrid2 :: PseudorandomGrid2 (const unsigned int a_seeds[])
		: PseudorandomGridSuperclass(),
		  m_seed_x1(),
		  m_seed_x2(),
		  m_seed_y1(),
		  m_seed_y2()
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
	setSeedsY(a_seeds[5], a_seeds[6]);
}

PseudorandomGrid2 :: PseudorandomGrid2 (const PseudorandomGrid2& original)
		 : PseudorandomGridSuperclass(original),
		  m_seed_x1(original.m_seed_x1),
		  m_seed_x2(original.m_seed_x2),
		  m_seed_y1(original.m_seed_y1),
		  m_seed_y2(original.m_seed_y2)
{
}

PseudorandomGrid2 :: ~PseudorandomGrid2 ()
{
}

PseudorandomGrid2& PseudorandomGrid2 :: operator= (const PseudorandomGrid2& original)
{
	if(&original != this)
	{
		PseudorandomGridSuperclass::operator=(original);

		m_seed_x1 = original.m_seed_x1;
		m_seed_x2 = original.m_seed_x2;
		m_seed_y1 = original.m_seed_y1;
		m_seed_y2 = original.m_seed_y2;
	}
	return *this;
}



void PseudorandomGrid2 :: setSeedsX (unsigned int seed_x1,
                                     unsigned int seed_x2)
{
	m_seed_x1 = seed_x1;
	m_seed_x2 = seed_x2;
}

void PseudorandomGrid2 :: setSeedsY (unsigned int seed_y1,
                                     unsigned int seed_y2)
{
	m_seed_y1 = seed_y1;
	m_seed_y2 = seed_y2;
}

void PseudorandomGrid2 :: setSeedsAll (unsigned int seed0,
                                       unsigned int seed1,
                                       unsigned int seed2,
                                       unsigned int seed_x1,
                                       unsigned int seed_x2,
                                       unsigned int seed_y1,
                                       unsigned int seed_y2)
{
	setSeedsQuadratic(seed0, seed1, seed2);
	setSeedsX(seed_x1, seed_x2);
	setSeedsY(seed_y1, seed_y2);
}

void PseudorandomGrid2 :: setSeedsAll (const unsigned int a_seeds[])
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
	setSeedsY(a_seeds[5], a_seeds[6]);
}





PseudorandomGrid3 :: PseudorandomGrid3 ()
		: PseudorandomGridSuperclass(),
		  m_seed_x1(SEED_X1_DEFAULT),
		  m_seed_x2(SEED_X2_DEFAULT),
		  m_seed_y1(SEED_Y1_DEFAULT),
		  m_seed_y2(SEED_Y2_DEFAULT),
		  m_seed_z1(SEED_Z1_DEFAULT),
		  m_seed_z2(SEED_Z2_DEFAULT)
{
}

PseudorandomGrid3 :: PseudorandomGrid3 (unsigned int seed0,
                                        unsigned int seed1,
                                        unsigned int seed2,
                                        unsigned int seed_x1,
                                        unsigned int seed_x2,
                                        unsigned int seed_y1,
                                        unsigned int seed_y2,
                                        unsigned int seed_z1,
                                        unsigned int seed_z2)
		: PseudorandomGridSuperclass(seed0, seed1, seed2),
		  m_seed_x1(seed_x1),
		  m_seed_x2(seed_x2),
		  m_seed_y1(seed_y1),
		  m_seed_y2(seed_y2),
		  m_seed_z1(seed_z1),
		  m_seed_z2(seed_z2)
{
}

PseudorandomGrid3 :: PseudorandomGrid3 (const unsigned int a_seeds[])
		: PseudorandomGridSuperclass(),
		  m_seed_x1(),
		  m_seed_x2(),
		  m_seed_y1(),
		  m_seed_y2(),
		  m_seed_z1(),
		  m_seed_z2()
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
	setSeedsY(a_seeds[5], a_seeds[6]);
	setSeedsZ(a_seeds[7], a_seeds[8]);
}

PseudorandomGrid3 :: PseudorandomGrid3 (const PseudorandomGrid3& original)
		 : PseudorandomGridSuperclass(original),
		  m_seed_x1(original.m_seed_x1),
		  m_seed_x2(original.m_seed_x2),
		  m_seed_y1(original.m_seed_y1),
		  m_seed_y2(original.m_seed_y2),
		  m_seed_z1(original.m_seed_z1),
		  m_seed_z2(original.m_seed_z2)
{
}

PseudorandomGrid3 :: ~PseudorandomGrid3 ()
{
}

PseudorandomGrid3& PseudorandomGrid3 :: operator= (const PseudorandomGrid3& original)
{
	if(&original != this)
	{
		PseudorandomGridSuperclass::operator=(original);

		m_seed_x1 = original.m_seed_x1;
		m_seed_x2 = original.m_seed_x2;
		m_seed_y1 = original.m_seed_y1;
		m_seed_y2 = original.m_seed_y2;
		m_seed_z1 = original.m_seed_z1;
		m_seed_z2 = original.m_seed_z2;
	}
	return *this;
}



void PseudorandomGrid3 :: setSeedsX (unsigned int seed_x1,
                                     unsigned int seed_x2)
{
	m_seed_x1 = seed_x1;
	m_seed_x2 = seed_x2;
}

void PseudorandomGrid3 :: setSeedsY (unsigned int seed_y1,
                                     unsigned int seed_y2)
{
	m_seed_y1 = seed_y1;
	m_seed_y2 = seed_y2;
}

void PseudorandomGrid3 :: setSeedsZ (unsigned int seed_z1,
                                     unsigned int seed_z2)
{
	m_seed_z1 = seed_z1;
	m_seed_z2 = seed_z2;
}

void PseudorandomGrid3 :: setSeedsAll (unsigned int seed0,
                                       unsigned int seed1,
                                       unsigned int seed2,
                                       unsigned int seed_x1,
                                       unsigned int seed_x2,
                                       unsigned int seed_y1,
                                       unsigned int seed_y2,
                                       unsigned int seed_z1,
                                       unsigned int seed_z2)
{
	setSeedsQuadratic(seed0, seed1, seed2);
	setSeedsX(seed_x1, seed_x2);
	setSeedsY(seed_y1, seed_y2);
	setSeedsZ(seed_z1, seed_z2);
}

void PseudorandomGrid3 :: setSeedsAll (const unsigned int a_seeds[])
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
	setSeedsY(a_seeds[5], a_seeds[6]);
	setSeedsZ(a_seeds[7], a_seeds[8]);
}





PseudorandomGrid4 :: PseudorandomGrid4 ()
		: PseudorandomGridSuperclass(),
		  m_seed_x1(SEED_X1_DEFAULT),
		  m_seed_x2(SEED_X2_DEFAULT),
		  m_seed_y1(SEED_Y1_DEFAULT),
		  m_seed_y2(SEED_Y2_DEFAULT),
		  m_seed_z1(SEED_Z1_DEFAULT),
		  m_seed_z2(SEED_Z2_DEFAULT),
		  m_seed_w1(SEED_W1_DEFAULT),
		  m_seed_w2(SEED_W2_DEFAULT)
{
}

PseudorandomGrid4 :: PseudorandomGrid4 (unsigned int seed0,
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
		: PseudorandomGridSuperclass(seed0, seed1, seed2),
		  m_seed_x1(seed_x1),
		  m_seed_x2(seed_x2),
		  m_seed_y1(seed_y1),
		  m_seed_y2(seed_y2),
		  m_seed_z1(seed_z1),
		  m_seed_z2(seed_z2),
		  m_seed_w1(seed_w1),
		  m_seed_w2(seed_w2)
{
}

PseudorandomGrid4 :: PseudorandomGrid4 (const unsigned int a_seeds[])
		: PseudorandomGridSuperclass(),
		  m_seed_x1(),
		  m_seed_x2(),
		  m_seed_y1(),
		  m_seed_y2(),
		  m_seed_z1(),
		  m_seed_z2(),
		  m_seed_w1(),
		  m_seed_w2()
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
	setSeedsY(a_seeds[5], a_seeds[6]);
	setSeedsZ(a_seeds[7], a_seeds[8]);
	setSeedsW(a_seeds[9], a_seeds[10]);
}

PseudorandomGrid4 :: PseudorandomGrid4 (const PseudorandomGrid4& original)
		 : PseudorandomGridSuperclass(original),
		  m_seed_x1(original.m_seed_x1),
		  m_seed_x2(original.m_seed_x2),
		  m_seed_y1(original.m_seed_y1),
		  m_seed_y2(original.m_seed_y2),
		  m_seed_z1(original.m_seed_z1),
		  m_seed_z2(original.m_seed_z2),
		  m_seed_w1(original.m_seed_w1),
		  m_seed_w2(original.m_seed_w2)
{
}

PseudorandomGrid4 :: ~PseudorandomGrid4 ()
{
}

PseudorandomGrid4& PseudorandomGrid4 :: operator= (const PseudorandomGrid4& original)
{
	if(&original != this)
	{
		PseudorandomGridSuperclass::operator=(original);

		m_seed_x1 = original.m_seed_x1;
		m_seed_x2 = original.m_seed_x2;
		m_seed_y1 = original.m_seed_y1;
		m_seed_y2 = original.m_seed_y2;
		m_seed_z1 = original.m_seed_z1;
		m_seed_z2 = original.m_seed_z2;
		m_seed_w1 = original.m_seed_w1;
		m_seed_w2 = original.m_seed_w2;
	}
	return *this;
}



void PseudorandomGrid4 :: setSeedsX (unsigned int seed_x1,
                                     unsigned int seed_x2)
{
	m_seed_x1 = seed_x1;
	m_seed_x2 = seed_x2;
}

void PseudorandomGrid4 :: setSeedsY (unsigned int seed_y1,
                                     unsigned int seed_y2)
{
	m_seed_y1 = seed_y1;
	m_seed_y2 = seed_y2;
}

void PseudorandomGrid4 :: setSeedsZ (unsigned int seed_z1,
                                     unsigned int seed_z2)
{
	m_seed_z1 = seed_z1;
	m_seed_z2 = seed_z2;
}

void PseudorandomGrid4 :: setSeedsW (unsigned int seed_w1,
                                     unsigned int seed_w2)
{
	m_seed_w1 = seed_w1;
	m_seed_w2 = seed_w2;
}

void PseudorandomGrid4 :: setSeedsAll (unsigned int seed0,
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
{
	setSeedsQuadratic(seed0, seed1, seed2);
	setSeedsX(seed_x1, seed_x2);
	setSeedsY(seed_y1, seed_y2);
	setSeedsZ(seed_z1, seed_z2);
	setSeedsZ(seed_w1, seed_w2);
}

void PseudorandomGrid4 :: setSeedsAll (const unsigned int a_seeds[])
{
	assert(a_seeds != 0);

	setSeedsQuadratic(a_seeds[0], a_seeds[1], a_seeds[2]);
	setSeedsX(a_seeds[3], a_seeds[4]);
	setSeedsY(a_seeds[5], a_seeds[6]);
	setSeedsZ(a_seeds[7], a_seeds[8]);
	setSeedsZ(a_seeds[9], a_seeds[10]);
}



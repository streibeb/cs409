//
//  RingSector.h
//

#ifndef RING_SECTOR_H
#define RING_SECTOR_H

#include <vector>

#include "RingSectorIndex.h"
#include "RingParticle.h"



//
//  RingSector
//
//  A record to represent a sector in space.  A sector has a
//    unique identifier representing its position.  It also
//    has a density and a list of ring particles that it
//    contains.
//
//  After it is created, a RingSector should not be modified.
//

struct RingSector
{
	RingSectorIndex m_index;
	unsigned int m_density;
	std::vector<RingParticle> mv_ring_particles;
};



#endif

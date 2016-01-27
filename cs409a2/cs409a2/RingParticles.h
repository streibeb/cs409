//
//  RingParticles.h
//  cs409a2
//
//  Created by Vince Streibel on 2016-01-25.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#ifndef RingParticles_h
#define RingParticles_h

#include "GetGlut.h"
#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/ObjModel.h"

class Sector;

struct SectorId
{
    int x, y, z;
};

class RingParticles
{
private:
    int radiusOfNearbySectors;
    int diameterOfNearbySectors;
    int sectorSize;
    Sector*** nearbySectors;
    
public:
    RingParticles(int radiusOfNearbySectors, int sectorSize, int particlesPerSector);
    ~RingParticles();
    
    SectorId calculateSectorId(Vector3 pos);
    void calculateNearbySectors(Vector3 cameraPosition);
    void DrawNearbySectors();
};

class Sector
{
private:
    const static int NUM_PARTICLE_MAX = 12;
    
    SectorId id;
    int size;
    int numOfParticles;
    
    ObjModel particleModel;
    Vector3 particlePosition[NUM_PARTICLE_MAX];

public:
    Sector();
    ~Sector();
    
    void init(int size, int numOfParticles);
    void setId(SectorId id);
    SectorId getId();
    void draw();
    Vector3 calculateSectorCentre();
};

#endif /* RingParticles_h */

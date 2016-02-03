//
//  Sector.h
//  cs409a2
//
//  Created by Vince Streibel on 2016-01-25.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#ifndef Sector_h
#define Sector_h

#include "GetGlut.h"
#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/ObjModel.h"
#include "../../ObjLibrary/DisplayList.h"
#include "World.h"

struct SectorId
{
    int x, y, z;
};

// Calculates the sector id given a location in the world
SectorId calculateSectorId(Vector3 p, int size);

// Calculates the position of a sector in world coordinates
// based on its sector id and sector size
Vector3 calculateVector3(SectorId s, int size);

//
//  xorshift
//
//  Purpose: To calculate the next pseudorandom value from
//           the specified seed value.
//  Parameter(s):
//    <1> a: The seed value
//  Precondition(s): N/A
//  Returns: The next pseudorandom value after a.
//  Side Effect: N/A
//
//  http://www.jstatsoft.org/v08/i14/paper
//
unsigned int xorshift(unsigned int a);

class Sector
{
private:
    SectorId id;
    int size;
    int density;
    
    DisplayList particleModel_list;
    struct particleInfo {
        double x, y, z, scale;
    };
    particleInfo whorleyPoints[12];

public:
    Sector();
    ~Sector();
    
    // Sets the display list of the sector
    void init(DisplayList d);
    
    void setId(SectorId id);
    SectorId getId();
    void setDensity(int density);
    int getDensity();
    void setSize(int size);
    int getSize();
    void draw();
    
private:
    // Calculates and returns the position of the centre of
    // the sector
    Vector3 calculateSectorCentre();
    
    // Generates a pseduorandom seed value based on sector ID
    unsigned int generateSeed();
    
    // Calculates the required number of pseduorandom values
    // for procedurally generating the sector's particles
    void calculateWorleyPoints();
};

#endif /* Sector_h */

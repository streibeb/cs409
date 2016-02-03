//
//  World.h
//  cs409a2
//
//  Created by Vince Streibel on 2016-01-28.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#ifndef World_h
#define World_h

#include <string>
#include "../../ObjLibrary/ObjModel.h"
#include "../../ObjLibrary/DisplayList.h"
#include "CoordinateSystem.h"
#include "Sector.h"

class Sector;
struct SectorId;

class World
{
private:
    static const int MOON_COUNT = 10;
    static const int RADIUS_NEARBY_SECTORS = 3;
    static const int SECTOR_SIZE = 500;
    
    struct objectInfo
    {
        std::string filename;
        float radius;
        float x;
        float y;
        float z;
        
        Vector3 getVector3() const
        {
            return {x, y, z};
        }
    };
    
    // Models
    // Skybox
    DisplayList skybox_list;
    objectInfo skyboxInfo = {
        "Models/Skybox.obj", 1.f, 0.f, 0.f, 0.f
    };
    
    // Saturn
    DisplayList saturn_list;
    objectInfo saturnInfo = {
        "Models/Saturn.obj", 60000.f, 0.f, 0.0f, 0.0f
    };
    
    // Moons
    DisplayList moons_list[MOON_COUNT];
    objectInfo moonInfo[MOON_COUNT] = {
        {"Models/MoonA.obj", 3300.f, 0.f, 0.f, 140000.f},
        {"Models/MoonB.obj", 4300.f, 85000.f, 0.f, 75000.f},
        {"Models/MoonC.obj", 2000.f, 130000.f, 0.f, 40000.f},
        {"Models/MoonD.obj", 3400.f, 110000.f, 0.f, -60000.f},
        {"Models/MoonE.obj", 5000.f, 100000.f, 0.f, -70000.f},
        {"Models/MoonF.obj", 3100.f, 20000.f, 0.f, -135000.f},
        {"Models/MoonG.obj", 2600.f, -60000.f, 0.f, -80000.f},
        {"Models/MoonH.obj", 2200.f, -95000.f, 0.f, -70000.f},
        {"Models/MoonI.obj", 4700.f, -90000.f, 0.f, -40000.f},
        {"Models/MoonJ.obj", 3800.f, -100000.f, 0.f, 50000.f}
    };
    
    // Ring
    DisplayList ring_list;
    objectInfo ringInfo = {
        "Models/Ring.obj", 60000.f, 0.0f, 0.f, 0.0f
    };
    
    // Sectors
    int diameterOfNearbySectors;
    Sector*** nearbySectors;
    
public:
    // Initializes all elements of the world
    // (Skybox, Saturn, Moons, Rings, Sectors)
    void init();

    // World element drawing functions
    void drawSkybox(CoordinateSystem& camera);
    void drawSaturn();
    void drawMoons();
    void drawRings(CoordinateSystem& camera);
    
private:
    // World element initialization functions
    // These are all called by init()
    void initSkybox();
    void initSaturn();
    void initMoons();
    void initRings();
    void initSectors();
    
    // Calculates the following:
    // (a) What sectors are within a given radius of the camera
    // (b) That sector's SectorId
    // (c) Sets sector's size
    // (d) Sets sector's density based on location in ring
    void calculateNearbySectors(Vector3 p);
    
    // Calculates the sector's density by calling helper functions
    // to calculate the smallest density
    // (a) raw density in respect to its position in the ring and
    // (b) raw density in respect to the large world objects in the ring
    // An equivalent mathematical equation is:
    // minDensity = min(calculateDensityRing(), calculateDensityHoles())
    //
    // Returns 6 * atan(minDensity/5000) * (2/PI)
    int calculateSectorDensity(SectorId& sid);
    
    // Calculates the sector's density in respect to its position in the ring
    // If the sector is within the ring's cylinder, the formula is
    //   density = 12000 - abs(pos.y)
    // Else
    //   density = 12000 - sqrt(pow(pos.y, 2) + pow(d_saturn - 150000, 2)
    // where pos is the position of the sector and d_saturn is the distance to saturn
    double calculateDensityRing(Vector3& p);
    
    // Calculates the sector's density in respect to each large world object
    // in (or nearby) the ring.  These include the moons and Saturn.
    double calculateDensityHoles(Vector3& p);
    
    // Draws all sectors within a close radius of the camera.
    void drawNearbySectors();
};

#endif /* World_h */

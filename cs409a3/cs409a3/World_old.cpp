//
//  World.cpp
//  cs409a2
//
//  Created by Vince Streibel on 2016-01-28.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#include "World_old.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

void World_old::init()
{
    diameterOfNearbySectors = (RADIUS_NEARBY_SECTORS * 2) + 1;
    
    initSkybox();
    initSaturn();
    initMoons();
    initRings();
}

void World_old::initSkybox()
{
    ObjModel skybox;
    skybox.load(skyboxInfo.filename);
    skybox_list = skybox.getDisplayList();
}

void World_old::initSaturn()
{
    ObjModel saturn;
    saturn.load(saturnInfo.filename);
    saturn_list = saturn.getDisplayList();
}

void World_old::initMoons()
{
    ObjModel moons[MOON_COUNT];
    for (int i = 0; i < MOON_COUNT; i++)
    {
        moons[i].load(moonInfo[i].filename);
        moons_list[i] = moons[i].getDisplayList();
    }
}

void World_old::initRings()
{
    ObjModel ring;
    ring.load(ringInfo.filename);
    ring_list = ring.getDisplayList();
    
    ObjModel particleModel;
    particleModel.load("Models/RingParticleA0.obj");
    DisplayList particleModel_list = particleModel.getDisplayList();
    
    // Init 3D array of sectors and set default DisplayList for the sector
    int d = diameterOfNearbySectors;
    nearbySectors = new Sector**[d];
    for (int i = 0; i < d; i++)
    {
        nearbySectors[i] = new Sector*[d];
        for (int j = 0; j < d; j++)
        {
            nearbySectors[i][j] = new Sector[d];
            for (int k = 0; k < d; k++)
            {
                nearbySectors[i][j][k].init(particleModel_list);
            }
        }
    }
}

void World_old::drawSkybox(CoordinateSystem& camera)
{
    glPushMatrix();
        glTranslatef(camera.getPosition().x,
                     camera.getPosition().y,
                     camera.getPosition().z);
        glDepthMask(GL_FALSE);
        skybox_list.draw();
        glDepthMask(GL_TRUE);
    glPopMatrix();
}

void World_old::drawSaturn()
{
    glPushMatrix();
        glTranslatef(saturnInfo.x, 0.f, saturnInfo.z);
        glScalef(saturnInfo.radius, saturnInfo.radius, saturnInfo.radius);
        saturn_list.draw();
    glPopMatrix();
}

void World_old::drawMoons()
{
    for (int i = 0; i < MOON_COUNT; i++)
    {
        glPushMatrix();
            glTranslatef(moonInfo[i].x, 0.f, moonInfo[i].z);
            glScalef(moonInfo[i].radius, moonInfo[i].radius, moonInfo[i].radius);
            moons_list[i].draw();
        glPopMatrix();
    }
}

void World_old::drawRings(CoordinateSystem& camera)
{
    calculateNearbySectors(camera.getPosition());
    drawNearbySectors();
    
    glPushMatrix();
        glTranslatef(ringInfo.x, 0.f, ringInfo.z);
        glScalef(ringInfo.radius, ringInfo.radius, ringInfo.radius);
        ring_list.draw();
    glPopMatrix();
}

void World_old::calculateNearbySectors(Vector3 p)
{
    SectorId camId = calculateSectorId(p, SECTOR_SIZE);
    int d = diameterOfNearbySectors;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            for (int k = 0; k < d; k++)
            {
                SectorId id = {
                    (camId.x + (i-4)),
                    (camId.y + (j-4)),
                    (camId.z + (k-4))
                };
                nearbySectors[i][j][k].setId(id);
                nearbySectors[i][j][k].setSize(500);
                nearbySectors[i][j][k].setDensity(calculateSectorDensity(id));
            }
        }
    }
}

int World_old::calculateSectorDensity(SectorId& sid)
{
    Vector3 p = calculateVector3(sid, SECTOR_SIZE);
    double d = fmin(calculateDensityRing(p), calculateDensityHoles(p));
    double c = atan(d/5000) * M_2_PI;
    return c * 6;
}

double World_old::calculateDensityRing(Vector3& p)
{
    double d_saturn = p.getNormXZ();
    if (d_saturn < 150000)
    {
        return 12000 - fabs(p.y);
    }
    else
    {
        return 12000 - sqrt(pow(p.y, 2) + pow(d_saturn - 150000, 2));
    }
}

double World_old::calculateDensityHoles(Vector3& p)
{
    double d_min = p.getDistance(saturnInfo.getVector3()) - (saturnInfo.radius + 15000);
    for (int i = 0; i < MOON_COUNT; i++)
    {
        double d_moon = p.getDistance(moonInfo[i].getVector3()) - (moonInfo[i].radius + 2000);
        if (d_moon < d_min) d_min = d_moon;
    }
    return d_min;
}

void World_old::drawNearbySectors()
{
    int d = diameterOfNearbySectors;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            for (int k = 0; k < d; k++)
            {
                nearbySectors[i][j][k].draw();
            }
        }
    }
}
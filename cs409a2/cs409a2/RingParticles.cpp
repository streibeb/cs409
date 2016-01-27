//
//  RingParticles.cpp
//  cs409a2
//
//  Created by Vince Streibel on 2016-01-25.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#include <cmath>
#include "RingParticles.h"

RingParticles::RingParticles(int radiusOfNearbySectors, int sectorSize, int particlesPerSector)
{
    this->radiusOfNearbySectors = radiusOfNearbySectors;
    this->sectorSize = sectorSize;
    this->diameterOfNearbySectors = (radiusOfNearbySectors * 2) + 1;
    
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
                nearbySectors[i][j][k].init(sectorSize, particlesPerSector);
            }
        }
    }
}

RingParticles::~RingParticles()
{
    
}

SectorId RingParticles::calculateSectorId(Vector3 pos)
{
    SectorId returnVal;
    returnVal.x = floor(pos.x / sectorSize);
    returnVal.y = floor(pos.y / sectorSize);
    returnVal.z = floor(pos.z / sectorSize);
    return returnVal;
}

void RingParticles::calculateNearbySectors(Vector3 cameraPosition)
{
    SectorId camId = calculateSectorId(cameraPosition);
    int d = diameterOfNearbySectors;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            for (int k = 0; k < d; k++)
            {
                SectorId id;
                id.x = (camId.x + (i-4));
                id.y = (camId.y + (j-4));
                id.z = (camId.z + (k-4));
                nearbySectors[i][j][k].setId(id);
            }
        }
    }
}

void RingParticles::DrawNearbySectors()
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

Sector::Sector()
{
    
}

Sector::~Sector()
{

}

void Sector::init(int size, int numOfParticles)
{
    particleModel.load("Models/RingParticleA0.obj");
    this->size = size;
    this->numOfParticles = numOfParticles;
}

void Sector::setId(SectorId id)
{
    this->id = id;
}

SectorId Sector::getId()
{
    return id;
}

void Sector::draw()
{
    Vector3 pos = calculateSectorCentre();
    for (int i = 0; i < numOfParticles; i++)
    {
        glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glScalef(10.f, 10.f, 10.f);
            particleModel.draw();
        glPopMatrix();
    }
}

Vector3 Sector::calculateSectorCentre()
{
    Vector3 p1(id.x * size,
               id.y * size,
               id.z * size);
    Vector3 p2((id.x * size) + size,
               (id.y * size) + size,
               (id.z * size) + size);
    Vector3 returnVal((p1.x + p2.x)/2,
                      (p1.y + p2.y)/2,
                      (p1.z + p2.z)/2);
    return returnVal;
}
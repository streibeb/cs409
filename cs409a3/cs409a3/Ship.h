//
//  Ship.h
//  cs409a3
//
//  Created by Vince Streibel on 2016-02-21.
//  Copyright © 2016 Brayden Streibel. All rights reserved.
//

#ifndef Ship_h
#define Ship_h

#include "PhysicsObject.h"
#include "WorldInterface.h"
#include "TimeSystem.h"

class DisplayList;
class WorldInterface;
struct PhysicsObjectId;

class Ship : public PhysicsObject
{
private:
    const float RELOAD_TIME = 0.25;
    
private:
    CoordinateSystem camera;
    bool alive;
    float health;
    int ammoCount;
    bool reloading;
    double reloadTimer;
    
public:
    Ship();
    Ship(int health, int ammo);
    Ship(const Ship& s);
    ~Ship();
    
    Ship& operator=(const Ship& s);
    
    float getHealth() const;
    bool isReloaded() const;
    int getAmmo() const;
    void setupCamera();
    CoordinateSystem getCameraCoordinateSystem() const;
    void setHealth(float health);
    void markReloading();
    void markNotReloading();
    void setAmmo(int newAmt);
    void addAmmo(int increase);
    
    PhysicsObject* getClone() const;
    bool isAlive() const;
    bool isDying() const;
    void markDead(bool instant);
    void update(WorldInterface& r_world);
    
private:
    void copy(const Ship& s);
};

#endif /* Ship_h */

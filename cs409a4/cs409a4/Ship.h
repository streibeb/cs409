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

namespace
{
    const double CAMERA_FORWARD_DISTANCE = -100.0;
    const double CAMERA_UP_DISTANCE      =   20.0;
}

class Ship : public PhysicsObject
{
public:
    //
    //  RADUIS
    //
    //  The default radius of the sphere used to represent a Ship
    //    for collision checking.
    //
    
    static const double RADIUS;
    
    //
    //  HEALTH_DEAD_AT
    //
    //  A constant indicating the health value at or below which a
    //    Ship is considered to be dead.  This value is slightly
    //    more than 0.0, and is used to avoid the effects of
    //    floating-point rounding errors.
    //
    
    static const float HEALTH_DEAD_AT;
    
    //
    //  RELOAD_TIME
    //
    //  The time it takes for the weapons on a Ship to reload after
    //    firing.
    //
    
    static const float RELOAD_TIME;
    
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
    
    //
    //  getHealth
    //
    //  Purpose: To determine the health of the ship
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The health of this ship
    //  Side Effect: N/A
    //
    
    float getHealth() const;
    
    //
    //  isReloaded
    //
    //  Purpose: To determine if this ship is currently
    //           reloading
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: Whether this ship is reloading. True means
    //           the ship is done reloading, and vice versa.
    //  Side Effect: N/A
    //
    
    bool isReloaded() const;
    
    //
    //  getAmmo
    //
    //  Purpose: To determine the amount of ammo this ship has
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The amount of ammo this ship has
    //  Side Effect: N/A
    //
    
    int getAmmo() const;
    
    //
    //  setupCamera
    //
    //  Purpose: To position the camera behind and aboce this ship
    //           looking in the same direction of the ship's forward
    //           vector
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: Positions the camera in a location behind and
    //               above this ship, looking in the same direction
    //               as the ship
    //
    
    void setupCamera() const;
    
    //
    //  getCameraCoordinateSystem
    //
    //  Purpose: To allow other objects access to this ship's camera's
    //           coordinate system
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: The coordinate system of the camera attached to this
    //           ship
    //  Side Effect: N/A
    //
    
    CoordinateSystem getCameraCoordinateSystem() const;
    
    //
    //  setHealth
    //
    //  Purpose: To change this ship's health
    //  Parameter(s):
    //    <1> health: The new health of the ship
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This ship's health is set to the new health value
    //
    
    void setHealth(float health);
    
    //
    //  addHealth
    //
    //  Purpose: To add to this ship's health
    //  Parameter(s):
    //    <1> increase: The amount of ammo to add
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This ship's health is increased by increase
    //
    
    void addHealth(float increase);
    
    //
    //  markReloading
    //
    //  Purpose: To begin the reload timer
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This ship's reload timer begins, preventing any
    //               firing until complete.
    //
    
    void markReloading();
    
    //
    //  markNotReloading
    //
    //  Purpose: To end the reload timer
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This ship's reload timer ends, allowing the ship
    //               to fire again
    //
    
    void markNotReloading();
    
    //
    //  setAmmo
    //
    //  Purpose: To change this ship's ammo count
    //  Parameter(s):
    //    <1> newAmt: The new ammo count of the ship
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This ship's ammo count is set to the new value
    //
    
    void setAmmo(int newAmt);
    
    //
    //  addAmmo
    //
    //  Purpose: To add ammo to the ship's ammo count
    //  Parameter(s):
    //    <1> increase: The amount of ammo to add
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This ship's ammo count is increased by increase
    //
    
    void addAmmo(int increase);
    
    //
    //  getClone
    //
    //  Purpose: To create a dynamically allocated copy of this
    //           PhysicsObject.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: A deep copy of this PhysicsObject.  This copy
    //           should be created using the copy constructor for
    //           the derived class.
    //  Side Effect: N/A
    //
    
    virtual PhysicsObject* getClone () const;
    
    //
    //  isAlive
    //
    //  Purpose: To determine if this PhysicsObject is alive.
    //  Parameter(s): N/A
    //  Precondition(s): N/A
    //  Returns: Whether this PhysicsObject is alive.  If this
    //           PhysicsObject is of a type that cannot become dead,
    //           true is returned.
    //  Side Effect: N/A
    //
    
    virtual bool isAlive () const;
    
    //
    //  isDying
    //
    //  Purpose: To determine if this PhysicsObject is currently
    //           dying.  This function can be used to prevent a
    //           PhysicsObject being counted twice.  For example, a
    //           bullet should not hit the same target twice.
    //  Parameter(s): N/A
    //  Precondition(s):
    //    <1> isAlive()
    //  Returns: Whether this PhysicsObject is going to be marked as
    //           dead the next time that update is called.  If this
    //           PhysicsObject is of a type that cannot become dead,
    //           false is returned.
    //  Side Effect: N/A
    //
    
    virtual bool isDying () const;
    
    //
    //  markDead
    //
    //  Purpose: To mark this PhysicsObject as dead.
    //  Parameter(s):
    //    <1> instant: Whether the PhysicsObject should be marked as
    //                 dead immediately
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: If this PhysicsObject is of a type that cannot
    //               become dead, there is no effect. Otherwise, if
    //               instant == true, this PhysicisObject is
    //               marked as dead and no death actions - such as
    //               generating a death explosion - are performed.
    //               If this PhysicsObject can become dead and
    //               instant == false, this PhysicsObject is marked
    //               as dying.  The next time that update is called,
    //               any death actions - such as generating a death
    //               explosion - will be performed and this
    //               PhysicsObject will be marked as dead.
    //
    
    virtual void markDead (bool instant);
    
    //
    //  update
    //
    //  Purpose: To update this PhysicsObject for one frame.  This
    //           function does not perform collision checking or
    //           handling.
    //  Parameter(s):
    //    <1> r_world: An interface to the world this PhysicsObject
    //                 is in
    //  Precondition(s): N/A
    //  Returns: N/A
    //  Side Effect: This PhysicsObject is updated for one frame.
    //               Any queries about or changes to the world it is
    //               in are resolved through r_world.
    //

    void update(WorldInterface& r_world);
    
private:
    void copy(const Ship& s);
};

#endif /* Ship_h */

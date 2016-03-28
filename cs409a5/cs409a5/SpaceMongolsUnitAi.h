//
//  SpaceMongolsUnitAi.h
//

#ifndef SPACE_MONGOLS_UNIT_AI_H
#define SPACE_MONGOLS_UNIT_AI_H

#include <vector>

#include "PhysicsObjectId.h"
#include "WorldInterface.h"
#include "ShipAiInterface.h"
#include "Ship.h"
#include "AiShipReference.h"
#include "UnitAiSuperclass.h"
#include "FleetNameSteeringBehaviours.h"



//
//  SpaceMongols
//
//  A namespace to store classes, functions, and constants for
//    fleet Space Mongols.
//

namespace SpaceMongols
{
    
    //
    //  UnitAiMoonGuard
    //
    //  A class inheriting from UnitAiSuperclass that makes a Ship
    //    guard a moon.  The Ship will normally fly around the moon
    //    erratically.  However, if the player is nearby, the Ship
    //    will stop patrolling and attack the player.  If the player
    //    leaves the moon or is destroyed, the Ship will start
    //    flying around the moon again.
    //
    
    const double VERY_LARGE_ANGLE               = 1.0e6;
    
    const double SCAN_DISTANCE_SHIP             = 2500.0;
    const double SCAN_DISTANCE_RING_PARTICLE    = 500.0;
    
    const double SHIP_CLEARANCE                 = 50.0;
    const double SHIP_AVOID_DISTANCE            = 200.0;
    const double RING_PARTICLE_CLEARANCE        = 100.0;
    const double RING_PARTICLE_AVOID_DISTANCE   = 500.0;
    const double PLANETOID_CLEARANCE            = 200.0;
    const double PLANETOID_AVOID_DISTANCE       = 1000.0;
    
    const double SHOOT_ANGLE_RADIANS_MAX        = 0.1;
    
    const unsigned int SCAN_COUNT_MAX           = 5;

    
    class UnitAiMoonGuard : public UnitAiSuperclass
    {
    private:
        FleetName::SteeringBehaviour* steeringBehaviour;
        PhysicsObjectId moon;
        std::vector<PhysicsObjectId> nearbyShips;
        std::vector<RingParticleData> nearbyRingParticles;
        PhysicsObjectId nearestPlanetoid;
        PhysicsObjectId nearestShip;
        PhysicsObjectId nearestEnemyShip;
        int scanCount;
        
    public:
        //
        //  Constructor
        //
        //  Purpose: To create a UnitAiMoonGuard for the specified
        //           Ship to guard the specified moon in the
        //           specified World.
        //  Parameter(s):
        //    <1> ship: The Ship to be controlled
        //    <2> world: The World the Ship is in
        //    <3> id_moon: The moon to guard
        //  Precondition(s):
        //    <1> ship.isShip()
        //    <2> id_moon.m_type == PhysicsObjectId::TYPE_PLANETOID
        //    <3> world.isAlive(id_moon)
        //    <4> world.isPlanetoidMoon(id_moon)
        //  Returns: N/A
        //  Side Effect: A new UnitAiMoonGuard is created to control
        //               Ship ship as it guards the moon with id
        //               id_moon in World world.
        //
        
        UnitAiMoonGuard (const AiShipReference& ship,
                         const WorldInterface& world,
                         const PhysicsObjectId& id_moon);
        
        //
        //  Modified Copy Constructor
        //
        //  Purpose: To create a new UnitAiMoonGuard as a copy of
        //           another, except that it controls the specified
        //           Ship.
        //  Parameter(s):
        //    <1> original: The UnitAiMoonGuard to copy
        //    <2> ship: The Ship to control
        //  Precondition(s):
        //    <1> ship.isShip()
        //  Returns: N/A
        //  Side Effect: A new UnitAiMoonGuard is created to control
        //               Ship ship.  In all other ways, the new
        //               UnitAiMoonGuard is a deep copy of original.
        //
        
        UnitAiMoonGuard (const UnitAiMoonGuard& original,
                         const AiShipReference& ship);
        
        //
        //  Destructor
        //
        //  Purpose: To safely destroy an UnitAi without memory
        //           leaks.
        //  Parameter(s): N/A
        //  Precondition(s): N/A
        //  Returns: N/A
        //  Side Effect: All dynamically allocated memory associated
        //               with this UnitAi is freed.
        //
        
        virtual ~UnitAiMoonGuard ();
        
        ///////////////////////////////////////////////////////////////
        //
        //  Virtual function inherited from UnitAiSuperclass
        //
        
        //
        //  getCloneForShip
        //
        //  Purpose: To create a dynamically allocated near-copy of this
        //           UnitAiSuperclass.  This near-copy should be a deep
        //           copy of the current UnitAiSuperclass, except that
        //           it should control the specified Ship.
        //  Parameter(s):
        //    <1> ship: The Ship for the copy to control
        //  Precondition(s):
        //    <1> ship.isShip()
        //  Returns: A deep near-copy of this UnitAiSuperclass that
        //           controls Ship ship is created.  This copy should be
        //           created using a modified copy constructor for the
        //           derived class.
        //  Side Effect: N/A
        //
        
        virtual UnitAiSuperclass* getCloneForShip (
                                                   const AiShipReference& ship) const;
        
        //
        //  draw
        //
        //  Purpose: To display the current state of this
        //           UnitAiSuperclass.  This function is intended for
        //           use in debugging.
        //  Parameter(s): N/A
        //  Precondition(s): N/A
        //  Returns: N/A
        //  Side Effect: Visual information for this UnitAiSuperclass is
        //               displayed.  The default implementation for this
        //               function does nothing.
        //
        
        virtual void draw () const;
        
        //
        //  updateForPause
        //
        //  Purpose: To update any internal time values for this
        //           UnitAiSuperclass after the game has been paused.
        //           This function should be called once each time the
        //           game is paused.  Calling this function at other
        //           times may result in unexpected behaviour.
        //  Parameter(s): N/A
        //  Precondition(s): N/A
        //  Returns: N/A
        //  Side Effect: This UnitAiSuperclass is updated for the most
        //               recent pause.  The default implementation for
        //               this function does nothing.
        //
        
        virtual void updateForPause ();
        
        //
        //  run
        //
        //  Purpose: To run this UnitAiSuperclass once.  This function
        //           will set the desired velocity for the Ship and mark
        //           the weapons to fire if that desired.  This function
        //           must work correctly even if it is not called every
        //           frame.
        //  Parameter(s):
        //    <1> world: The World that the Ship is in
        //  Precondition(s):
        //    <1> world.isAlive(getShipId())
        //  Returns: N/A
        //  Side Effect: The desired velocity for the controlled Ship is
        //               updated.  Any weapons that should be fired are
        //               marked acccordingly.
        //
        
        virtual void run (const WorldInterface& world);
        
    private:
        void scan (const WorldInterface& world);
        void getClosestShip(const WorldInterface& world);
        void getClosestEnemyShip(const WorldInterface& world);
        void shootAtShip(const WorldInterface& world,
                         const PhysicsObjectId& target);
        Vector3 chargeAtTarget(const WorldInterface& world,
                               const PhysicsObjectId& target,
                               const Vector3& orig_velocity);
        Vector3 avoidShips(const WorldInterface& world,
                           const Vector3& orig_velocity);
        Vector3 avoidPlanetoids(const WorldInterface& world,
                                const Vector3& orig_velocity);
        Vector3 avoidRingParticles(const WorldInterface& world,
                                   const Vector3& orig_velocity);
        
    private:
        //
        //  Copy Constructor
        //  Assignment Operator
        //
        //  These functions have intentionally not been implemented.
        //    Use the modified copy constructor instead.
        //
        
        UnitAiMoonGuard (const UnitAiMoonGuard& original);
        UnitAiMoonGuard& operator= (
                                    const UnitAiMoonGuard& original);
        
    };
    
    
    
}  // end of namespace SpaceMongols



#endif

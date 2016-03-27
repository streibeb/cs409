//
//  Planetoid.h
//

#ifndef PLANETOID_H
#define PLANETOID_H

#include "../../ObjLibrary/Vector3.h"

#include "PhysicsObject.h"

class DisplayList;

class WorldInterface;
struct PhysicsObjectId;



//
//  Planetoid
//
//  A class to represent a major astronomical body, such as a
//    planet or a moon.  Planetoids are assumed to be physically
//    unmoving and unchanging, and do not require updates.  A
//    Planetoid does have an owner, however, which can be
//    changed.  A Planetoid also stores whether the owner is
//    close enough to enforce their claim, of whether it could
//    be claimed by another fleet.
//
//  Class Invariant:
//    <1> m_owner <= PhysicsObjectId::FLEET_MAX
//    <2> m_owner != PhysicsObjectId::FLEET_NATURE ||
//        !m_is_actively_claimed
//

class Planetoid : public PhysicsObject
{
public:
//
//  RADIUS_DEFAULT
//
//  The default radius for an Planetoid.
//

	static const double RADIUS_DEFAULT;

public:
//
//  Default Constructor
//
//  Purpose: To create a new Planetoid at the origin.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Planetoid is created at the origin with
//               an id of ID_DEFAULT and a radius of
//               RADIUS_DEFAULT.  It is owned by
//               PhysicsObjectId::FLEET_NATURE, is not actively
//               claimed, and does not have a display list is
//               set.
//

	Planetoid ();

//
//  Constructor
//
//  Purpose: To create a new Planetoid with the
//           specified id, position, and radius.
//  Paremeter(s):
//    <1> id: The id
//    <2> position: The position
//    <3> radius: The radius
//    <4> display_list: The DisplayList to display with
//    <5> display_scale: The scaling factor for display_list
//  Precondition(s)
//    <1> id != PhysicsObjectId::ID_NOTHING
//    <2> radius >= 0.0
//    <3> display_list.isReady()
//    <4> display_scale >= 0.0
//  Returns: N/A
//  Side Effect: A new Planetoid is created at position position
//               with an id of id and a radius of radius.  The
//               new Planetoid oriented to be facing along the
//               positive Y-axis with +Z as up.  This matches
//               the orientation of an unrotated model.  The new
//               planetoid is owned by
//               PhysicsObjectId::FLEET_NATURE, but is not
//               actively claimed.  It is displayed with display
//               list display_list scaled by scaling factor
//               display_scale.
//

	Planetoid (const PhysicsObjectId& id,
	           const Vector3& position,
	           double radius,
	           const DisplayList& display_list,
	           double display_scale);

//
//  Copy Constructor
//
//  Purpose: To create a new Planetoid as a copy of another.
//  Paremeter(s):
//    <1> original: The Planetoid to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Planetoid is created with the same id,
//               position, radius, display information, and
//               ownership status as original.
//

	Planetoid (const Planetoid& original);

//
//  Destructor
//
//  Purpose: To safely destroy a Planetoid without memory leaks.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this Planetoid is freed.
//

	virtual ~Planetoid ();

//
//  Assignment Operator
//
//  Purpose: To modify this Planetoid to be a copy of another.
//  Paremeter(s):
//    <1> original: The Planetoid to copy
//  Precondition(s): N/A
//  Returns: A reference to this Planetoid.
//  Side Effect: The Planetoid is set to have the same id,
//               position, radius, display information, and
//               ownership status as original.
//

	Planetoid& operator= (const Planetoid& original);

//
//  getOwner
//
//  Purpose: To determine which fleet currently owns this
//           Planetoid.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The index for the fleet that currently owns this
//           Planetoid.  This value is never larger than
//           PhysicsObjectId::FLEET_MAX.
//  Side Effect: N/A
//

	unsigned int getOwner () const;

//
//  isActivelyClaimed
//
//  Purpose: To determine if the fleet that currently owns this
//           Planetoid currently has a command ship close enough
//           to enforce the claim.  If this function returns
//           false, the moon can be claimed.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the command ship of the fleet that
//           currently owns this Planetoid is close enough to
//           claim it. If the owner is
//           PhysicsObjectId::FLEET_NATURE, this value is always
//           false.
//  Side Effect: N/A
//

	bool isActivelyClaimed () const;

//
//  drawOwner
//
//  Purpose: To display graphically which fleet currently owns
//           this Planetoid.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: If this Planetoid is currently owned by fleet
//           PhysicsObjectId::FLEET_NATURE, there is no effect.
//           Otherwise, a wire sphere is displayed around the
//           planetoid to represent which fleet owns it.  If the
//           player fleet owns the planetoid, the sphere is
//           green.  Otherwise, the sphere is a colour close to
//           red.
//  Side Effect: N/A
//

	void drawOwner () const;

//
//  initPlanetoid
//
//  Purpose: To initialize this Planetoid to be at the
//           specified position with the specified size.  This
//           Planetoid is also set to display with the specified
//           display list, scaled by the specified scaling
//           factor.
//  Parameter(s):
//    <1> id: The id
//    <2> position: The starting position
//    <3> radius: The radius
//    <4> display_list: The DisplayList to display with
//    <5> display_scale: The scaling factor for display_list
//  Precondition(s):
//    <1> id != PhysicsObjectId::ID_NOTHING
//    <2> radius >= 0.0
//    <3> display_list.isReady()
//    <4> display_scale >= 0.0
//  Returns: N/A
//  Side Effect: This Planetoid is set to be at position
//               position with an id of id and a radius of
//               radius.  This Planetoid set to be stationary
//               and facing along the positive Y-axis with +Z
//               as up.  This matches the orientation of
//               unrotated model.  In either case, this
//               Planetoid has a previous position, is owned by
//               PhysicsObjectId::FLEET_NATURE, and is not
//               actively claimed.  It is displayed with display
//               list display_list scaled by scaling factor
//               display_scale.
//

	void initPlanetoid (const PhysicsObjectId& id,
	                    const Vector3& position,
	                    double radius,
	                    const DisplayList& display_list,
	                    double display_scale);

//
//  setOwner
//
//  Purpose: To change which fleet owns this Planetoid.
//  Parameter(s):
//    <1> owner: The new owner fleet
//  Precondition(s):
//    <1> owner <= PhysicsObjectId::FLEET_MAX
//  Returns: N/A
//  Side Effect: This Planetoid is set to be owned by fleet
//               owner.  If
//               owner == PhysicsObjectId::FLEET_NATURE, this
//               Planetoid is marked as not actively claimed.
//               Otherwise, this Planetoid is marked as being
//               actively claimed.
//

	void setOwner (unsigned int owner);

//
//  markNotActivelyClaimed
//
//  Purpose: To mark this Planetoid as not actively claimed.
//           This means that this Planetoid can be claimed by
//           any command ship that gets close enough.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Planetoid is marked as not actively
//               claimed.
//

	void markNotActivelyClaimed ();

///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from PhysicsObject
//

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
//  Purpose: To mark this PhysicsObject as dead.  This function
//           does nothing.
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
//               in are resolved through r_world.  This function
//               implementation does nothing.
//

	virtual void update (WorldInterface& r_world);

private:
///////////////////////////////////////////////////////////////
//
//  Helper function not inherited from anywhere
//

//
//  invariant
//
//  Purpose: To determine if the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invarint is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	unsigned int m_owner;
	bool m_is_actively_claimed;
};



#endif

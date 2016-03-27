//
//  ExplosionManager.h
//
//  A module to store and display explosions as animated
//    billboards.
//

#ifndef EXPLOSION_MANAGER_H
#define EXPLOSION_MANAGER_H

#include <string>
#include <vector>

#include "../../ObjLibrary/Vector3.h"
#include "../../ObjLibrary/DisplayList.h"

#include "ExplosionManagerInterface.h"



//
//  ExplosionManager
//
//  A class to represent and store explosions in the world.
//    Explosions can be added at any time.  An explosion will be
//    displayed as an animated billboard facing the camera and
//    will be removed automatically when the animation is
//    complete.  It is also possible to remove all explosions.
//
//  The files for the ExplosionManager must be loaded after GLUT
//    is initialized but before drawAll() is called.  This can
//    be done by calling
//
//      my_explosion_manager->init();
//
//    between the glutInit() and glutMainLoop() functions.  The
//    files are specific to the ExplosionManager they are in.
//    This allows different ExplosionManagers to be created for
//    different explosion animations.
//
//  The explosions are represented in a circular array.
//    Positions are stored for the oldest explosion still active
//    and for the position to add the next explosion.
//
//  Class Invariant:
//    <1> mv_frames.size() == m_frame_count
//    <2> mv_frames.empty() || !ma_frames[0].isPartial()
//    <3> mv_frames.empty() ||
//        mv_frames[0].getState() == ma_frames[i].getStare()
//                                  WHERE 1 <= i < m_frame_count
//    <4> m_oldest_explosion < EXPLOSION_COUNT_MAX
//    <5> m_next_explosion < EXPLOSION_COUNT_MAX
//

class ExplosionManager : public ExplosionManagerInterface
{
public:
//
//  EXPLOSION_COUNT_MAX
//
//  The maximum number of explosions that can be in an
//    ExplosionManager at the same time.
//

	static const unsigned int EXPLOSION_COUNT_MAX = 256;

//
//  LIFESPAN
//
//  The lifespan of the explosion in seconds
//

	static const double LIFESPAN;

public:
//
//  Default Constructor
//
//  Purpose: To create an ExplosionManager containing no
//           explosions.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new ExplosionManager is created.  It contains
//               no explosions.
//

	ExplosionManager ();

//
//  Copy Constructor
//
//  Purpose: To create an ExplosionManager as a copy of another.
//  Parameter(s):
//    <1> original: The ExplosionManager to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new ExplosionManager is created.  If original
//               is not initialized, the new ExplosionManager is
//               not initialized.  Otherwise, the new
//               ExplosionManager is initialized and contains a
//               copy of each explosion in original.
//

	ExplosionManager (const ExplosionManager& original);

//
//  Destructor
//
//  Purpose: To safely destroy an ExplosionManager without
//           memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this ExplosionManager is freed.
//

	virtual ~ExplosionManager ();

//
//  Assignment Operator
//
//  Purpose: To modify this ExplosionManager to be a copy of
//           another.
//  Parameter(s):
//    <1> original: The ExplosionManager to copy
//  Precondition(s): N/A
//  Returns: A reference to this ExplosionManager.
//  Side Effect: All explosions in this ExplosionManager are
//               removed.  If original is not initialized, this
//               ExplosionManager is marked as not initialized.
//               Otherwise, this ExplosionManager is marked as
//               initialized and set to contain a copy of each
//               explosion in original.
//

	ExplosionManager& operator= (
	                          const ExplosionManager& original);

//
//  updateForPause
//
//  Purpose: To update any internal time values for the
//           explosions in the ExplosionManager after the game
//           has been paused.  This function should be called
//           once each time the game is paused.  Calling this
//           function at other times may result in unexpected
//           behaviour.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isInitialized()
//  Returns: N/A
//  Side Effect: All explosions in this ExplosionManager are
//               updated for the most recent pause.
//

	void updateForPause ();

///////////////////////////////////////////////////////////////
//
//  Virtual functions inherited from ExplosionManagerInterface
//

//
//  isInitialized
//
//  Purpose: To determine if this ExplosionManager has been
//           initialized.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this ExplosionManager is initialized.
//  Side Effect: N/A
//

	virtual bool isInitialized () const;

//
//  isEmpty
//
//  Purpose: To determine if this ExplosionManager contains no
//           explosions.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether there are no explosions in this
//           ExplosionManager.
//  Side Effect: N/A
//

	virtual bool isEmpty () const;

//
//  getClone
//
//  Purpose: To create a dynamically-allocated copy of this
//           ExplosionManager.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A pointer to a dynamically-allocated deep copy of
//           this ExplosionManager.
//  Side Effect: N/A
//

	virtual ExplosionManager* getClone () const;

//
//  draw
//
//  Purpose: To display all the explosions in this
//           ExplosionManager.
//  Parameter(s):
//    <1> camera_forward: The camera forward vector
//    <2> camera_up: The camera up vector
//  Precondition(s):
//    <1> isInitialized()
//    <2> camera_forward.isNormal()
//    <3> camera_up.isNormal()
//    <4> camera_forward.isOrthogonal(camera_up)
//  Returns: N/A
//  Side Effect: All explosions in this ExplosionManager are
//               displayed to face a camera facing in direction
//               camera_forward and with camera_up as up.
//

	virtual void draw (const Vector3& camera_forward,
	                   const Vector3& camera_up) const;

//
//  init
//
//  Purpose: To initialize this ExplosionManager.
//  Parameter(s):
//    <1> filename: The name of the file containing the frames
//    <2> frame_count: The number of frames in the animation
//  Precondition(s):
//    <1> filename != ""
//    <2> frame_count > 0
//  Returns: N/A
//  Side Effect: This ExplosionManager is initialized to display
//               animations of frame_count frames from file
//               filename.  All textures are loaded to video
//               memory and all display lists are constructed.
//               The existing frames, if any, are lost.
//

	virtual void init (const std::string& filename,
	                   unsigned int frame_count);

//
//  add
//
//  Purpose: To add an explosion of the specified size at the
//           specified position.
//  Parameter(s):
//    <1> position: Where to add the explosion
//    <2> size: The radius of the explosion
//  Precondition(s):
//    <1> size >= 0.0
//  Returns: N/A
//  Side Effect: An explosion is added to this
//               ExplosionManagerInterface at position position.
//               The new explosion has a radius of size and is
//               at the beginning of its animation cycle.
//

	virtual void add (const Vector3& position, double size);

//
//  update
//
//  Purpose: To update all explosions in this ExplosionManager
//           for one frame cycle.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All explosions in this ExplosionManager are
//               updated.
//

	virtual void update ();

//
//  removeAll
//
//  Purpose: To remove all explosions from this
//           ExplosionManager.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All explosions in this ExplosionManager are
//               removed.
//

	virtual void removeAll ();

private:
///////////////////////////////////////////////////////////////
//
//  Helper functions not inherited from anywhere
//

//
//  copy
//
//  Purpose: To copy the frames and explosions from another
//           ExplosionManager to this one.
//  Parameter(s):
//    <1> original: The ExplosionManager to copy from
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Any existing frames and explosions in this
//               ExplosionManager are removed.  The explosions
//               and frames in original are copied to this
//                ExplosionManager.
//

	void copy (const ExplosionManager& original);

//
//  invariant
//
//  Purpose: To determine if the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	//
	//  Explosion
	//
	//  A record to represent a single explosion.  An Explosion
	//    has a position, a size, and a creation time.
	//    Explosions with a creation time more than LIFESPAN
	//    before the current time are assumed not to exist.
	//

	struct Explosion
	{
		Vector3 m_position;
		double m_size;
		double m_creation_time;
	};

private:
	std::vector<DisplayList> mv_frames;
	unsigned int m_frame_count;
	Explosion ma_explosions[EXPLOSION_COUNT_MAX];
	unsigned int m_oldest_explosion;
	unsigned int m_next_explosion;
};



#endif

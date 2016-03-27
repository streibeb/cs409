//
//  ExplosionManagerInterface.h
//
//  An abstract interface for a module to store and display
//    explosions.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef EXPLOSION_MANAGER_INTERFACE_H
#define EXPLOSION_MANAGER_INTERFACE_H

#include <string>



class Vector3;

//
//  ExplosionManagerInterface
//
//  An abstract interface for a class to represent and store
//    explosions.  Functions exist to initialize the class, add
//    a single explosion, and to update and display all
//    explosions.
//
//  The files for the ExplosionManager must be loaded after GLUT
//    is initialized but before drawAll() is called.  This can
//    be done by calling
//
//      my_explosion_manager.init("filename.png", frame_count);
//
//    between the glutInit() and glutMainLoop() functions.
//
//  A dynamically-alocated copy of an instance of a derived
//    class can be created with getClone() function.
//

class ExplosionManagerInterface
{
public:
//
//  Destructor
//
//  Purpose: To safely destroy an ExplosionManagerInterface
//           without memory leaks.  We need a virtual destructor
//           in the base class so that the destructor for the
//           correct derived class will be invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this ExplosionManagerInterface is freed.
//

	virtual ~ExplosionManagerInterface ()
	{ }  // do nothing

//
//  isInitialized
//
//  Purpose: To determine if this ExplosionManagerInterface has
//           been initialized.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this ExplosionManagerInterface is
//           initialized.
//  Side Effect: N/A
//

	virtual bool isInitialized () const = 0;

//
//  isEmpty
//
//  Purpose: To determine if this ExplosionManagerInterface
//           contains no explosions.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether there are no explosions in this
//           ExplosionManagerInterface.
//  Side Effect: N/A
//

	virtual bool isEmpty () const = 0;

//
//  getClone
//
//  Purpose: To create a dynamically-allocated copy of this
//           ExplosionManagerInterface.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A pointer to a dynamically-allocated deep copy of
//           this ExplosionManagerInterface.
//  Side Effect: N/A
//

	virtual ExplosionManagerInterface* getClone () const = 0;

//
//  draw
//
//  Purpose: To display all the explosions in this
//           ExplosionManagerInterface.
//  Parameter(s):
//    <1> camera_forward: The camera forward vector
//    <2> camera_up: The camera up vector
//  Precondition(s):
//    <1> isInitialized()
//    <2> camera_forward.isNormal()
//    <3> camera_up.isNormal()
//    <4> camera_forward.isOrthongonal(camera_up)
//  Returns: N/A
//  Side Effect: All explosions in this
//               ExplosionManagerInterface are displayed to face
//               a camera facing in direction camera_forward and
//               with camera_up as up.
//

	virtual void draw (const Vector3& camera_forward,
	                   const Vector3& camera_up) const = 0;

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
	                   unsigned int frame_count) = 0;

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

	virtual void add (const Vector3& position, double size) = 0;

//
//  update
//
//  Purpose: To update all explosions in this
//           ExplosionManagerInterface for one frame cycle.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All explosions in this
//               ExplosionManagerInterface are updated.
//

	virtual void update () = 0;

//
//  removeAll
//
//  Purpose: To remove all explosions from this
//           ExplosionManagerInterface.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All explosions in this
//               ExplosionManagerInterface are removed.
//

	virtual void removeAll () = 0;
};



#endif

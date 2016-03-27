//
//  SimpleMarker.h
//

#ifndef SIMPLE_MARKER_H
#define SIMPLE_MARKER_H

#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/DisplayList.h"



//
//  SimpleMarker
//
//  A class to represent a marker in 3d space.  A SimpleMarker
//    is displayed for a fixed time after it is created or
//    initialized and then disappears.  A SimpleMarker that is
//    being displayed is refered to as alive, while one that
//    this not displayed is refered to as dead.  A simple marker
//    can be blue or red.
//

class SimpleMarker
{
public:
//
//  LIFESPAN
//
//  A constant indicating the time between when a SimpleMarker
//    is created and when it disappears.
//

	static const double LIFESPAN;

//
//  RADIUS
//
//  The distance from the center of a SimpleMarker and its edge.
//

	static const double RADIUS;

public:
//
//  Class Function: isModelInitialized
//
//  Purpose: To determine of the model for the SimpleMarker
//           class has been initialized.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the models have been initialized.
//  Side Effect: N/A
//

	static bool isModelInitialized ();

//
//  Class Function: initModel
//
//  Purpose: To initialize the model for the SimpleMarker class.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> !isModelInitialized()
//  Returns: N/A
//  Side Effect: The model for the SimpleMarker class is
//               initialized.  This model is shared between
//               all instances of the class, so this function
//               only needs to be called once.
//

	static void initModel ();

public:
//
//  Default Constructor
//
//  Purpose: To create a new stationary SimpleMarker at the origin.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new SimpleMarker is created at the origin.  The
//               new SimpleMarker has a creation time in the
//               distant past and is red in colour.
//

	SimpleMarker ();

//
//  Constructor
//
//  Purpose: To create a new SimpleMarker at the specified
//           position.
//  Paremeter(s):
//    <1> position: The position
//    <2> is_blue: Whether the new SimpleMarker is blue
//  Precondition(s) N/A
//  Returns: N/A
//  Side Effect: A new SimpleMarker is created at position
//               position with a creation time of the current
//               time.  If is_blue == true, the new SimpleMarker
//               is blue in colour.  Otherwise, it is red.
//

	SimpleMarker (const Vector3& position, bool is_blue);

//
//  Copy Constructor
//
//  Purpose: To create a new SimpleMarker as a copy of another.
//  Paremeter(s):
//    <1> original: The SimpleMarker to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new SimpleMarker is created with the same
//               position and creation time as original.
//

	SimpleMarker (const SimpleMarker& original);

//
//  Destructor
//
//  Purpose: To safely destroy a SimpleMarker without memory
//           leaks.
//  Paremeter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this SimpleMarker is freed.
//

	~SimpleMarker ();

//
//  Assignment Operator
//
//  Purpose: To modify this SimpleMarker to be a copy of another.
//  Paremeter(s):
//    <1> original: The SimpleMarker to copy
//  Precondition(s): N/A
//  Returns: A reference to this SimpleMarker.
//  Side Effect: The SimpleMarker is set to have the same
//               position and creation time as original.
//

	SimpleMarker& operator= (const SimpleMarker& original);

//
//  getPosition
//
//  Purpose: To determine the position of this SimpleMarker.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The position of this SimpleMarker.
//  Side Effect: N/A
//

	const Vector3& getPosition () const;

//
//  isBlue
//
//  Purpose: To determine if this SimpleMarker is blue in
//           colour.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this SimpleMarker is blue.
//  Side Effect: N/A
//

	bool isBlue () const;

//
//  isAlive
//
//  Purpose: To determine if this SimpleMarker is alive.  A
//           SimpleMarker is alive if the current time is less
//           than LIFESPAN after its creation time.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this SimpleMarker is alive.
//  Side Effect: N/A
//

	bool isAlive () const;

//
//  draw
//
//  Purpose: To display this SimpleMarker.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> isModelInitialized()
//  Returns: N/A
//  Side Effect: This SimpleMarker is drawn to the screen.
//

	void draw () const;

//
//  init
//
//  Purpose: To initialize this SimpleMarker to have been just
//           created at the specified position.
//  Paremeter(s):
//    <1> position: The position
//    <2> is_blue: Whether the new SimpleMarker is blue
//  Precondition(s) N/A
//  Returns: N/A
//  Side Effect: This SimpleMarker is set to be at position
//               position with a creation time of the current
//               time.  If is_blue == true, this SimpleMarker is
//               marked as blue in colour.  Otherwise, it is
//               marked as red.
//

	void init (const Vector3& position, bool is_blue);

//
//  markDead
//
//  Purpose: To mark this SimpleMarker as dead.
//  Paremeter(s): N/A
//  Precondition(s) N/A
//  Returns: N/A
//  Side Effect: This SimpleMarker is set to have a creation
//               time of long ago.
//

	void markDead ();

private:
	static DisplayList ms_model;

private:
	Vector3 m_position;
	bool m_is_blue;
	double m_creation_time;
};



#endif

//
//  Trail.h
//

#ifndef TRAIL_H
#define TRAIL_H

#include <vector>

#include "ObjLibrary/Vector3.h"



//
//  Trail
//
//  A class to represent a trail through 3d space, as might be
//    left by something moving.  A Trail is represented as a
//    series of points, connected into a polyline.
//
//  There are commands to add trail points and to display the
//    trail.  When all the trail points have been used, adding
//    additional points will cause the oldest points to be
//    removed.  There is no way to control which points are
//    displayed, so when a trial is initialized, it is a good
//    idea to move all the points to the starting end of the
//    trail.
//
//  The colour that the Trail is displayed in can be set.  The
//    back of the trail always fades to black, so setting the
//    Trail to black can lead to confusion.
//
//  Class Invaraint:
//    <1> mv_points.size() >= 1
//    <2> m_next_point < mv_points.size()
//    <3> m_colour.isAllComponentsGreaterThanOrEqual(0.0)
//    <4> m_colour.isAllComponentsLessThanOrEqual(1.0)
//

class Trail
{
public:
//
//  POINT_COUNT_DEFAULT
//
//  The default number of points in a Trail.
//

	static const unsigned int POINT_COUNT_DEFAULT = 200;

//
//  COLOUR_DEFAULT
//
//  The default colour for a Trail.
//

	static const Vector3 COLOUR_DEFAULT;

public:
//
//  Default Constructor
//
//  Purpose: To create a new Trail with a default number of
//           points and display colour.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Trail is created containing
//               POINT_COUNT_DEFAULT points and with a colour of
//               COLOUR_DEFAULT.  All trail points are placed at
//               the origin.
//

	Trail ();

//
//  Constructor
//
//  Purpose: To create a new Trail with the specified number of
//           points and display colour.
//  Parameter(s):
//    <1> point_count: The number of points in the trail
//    <2> colour: The colour to display the trail in
//  Precondition(s):
//    <1> point_count >= 1
//    <2> colour.isAllComponentsGreaterThanOrEqual(0.0)
//    <3> colour.isAllComponentsLessThanOrEqual(1.0)
//  Returns: N/A
//  Side Effect: A new Trail is created containing point_count
//               points and with a colour of colour.  All trail
//               points are placed at the origin.
//

	Trail (unsigned int point_count,
	       const Vector3& colour);

//
//  Constructor
//
//  Purpose: To create a new Trail with the specified number of
//           points at the specified position and the specified
//           display colour.
//  Parameter(s):
//    <1> point_count: The number of points in the trail
//    <2> position: Where the trail starts
//    <3> colour: The colour to display the trail in
//  Precondition(s):
//    <1> point_count >= 1
//    <2> colour.isAllComponentsGreaterThanOrEqual(0.0)
//    <3> colour.isAllComponentsLessThanOrEqual(1.0)
//  Returns: N/A
//  Side Effect: A new Trail is created containing point_count
//               points and with a colour of colour.  All trail
//               points are placed at position.
//

	Trail (unsigned int point_count,
	       const Vector3& position,
	       const Vector3& colour);

//
//  Copy Constructor
//
//  Purpose: To create a new Trail as a copy of another.
//  Parameter(s):
//    <1> original: The Trail to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Trail is created with the same points and
//               display colour as original.
//

	Trail (const Trail& original);

//
//  Destructor
//
//  Purpose: To safely destroy an Trail withoout memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this Trail is freed.
//

	~Trail ();

//
//  Assignment Operator
//
//  Purpose: To modify this Trail to be a copy of another.
//  Parameter(s):
//    <1> original: The Trail to copy
//  Precondition(s): N/A
//  Returns: A reference to this Trail.
//  Side Effect: This Trail is set to have the same same points
//               and display colour as original.
//

	Trail& operator= (const Trail& original);

//
//  getPointCount
//
//  Purpose: To determine the number of points in this Trail.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of points composing this Trail.
//  Side Effect: N/A
//

	unsigned int getPointCount () const;

//
//  getColour
//
//  Purpose: To determine the display colour for this Trail.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The colour this Trail is displayed in.
//  Side Effect: N/A
//

	const Vector3& getColour () const;

//
//  draw
//
//  Purpose: To display this Trail using OpenGL graphics.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Trail is displayed.
//

	void draw () const;

//
//  setPointCount
//
//  Purpose: To change the number of points in this Trail.  This
//           function erases the current trail.
//  Parameter(s):
//    <1> point_count: The new number of points
//  Precondition(s):
//    <1> point_count >= 1
//  Returns: N/A
//  Side Effect: This Trail is set to contain point_count
//               points.  All points are moved to the origin.
//

	void setPointCount (unsigned int point_count);

//
//  setPointCount
//
//  Purpose: To change the number of points in this Trail.  This
//           function erases the current trail.
//  Parameter(s):
//    <1> point_count: The new number of points
//    <2> position: Where the trail starts
//  Precondition(s):
//    <1> point_count >= 1
//  Returns: N/A
//  Side Effect: This Trail is set to contain point_count
//               points.  All point are moved to position
//               position.
//

	void setPointCount (unsigned int point_count,
	                    const Vector3& position);

//
//  addPoint
//
//  Purpose: To add a point to the front of this Trail.
//  Parameter(s):
//    <1> position: The position of the new point
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A point at position position is added to this
//               Trail.
//

	void addPoint (const Vector3& position);

//
//  setAllPointsToPosition
//
//  Purpose: To move all points in this Trail to the specified
//           position.  This function effectively restarts the
//           trail at the specified position.
//  Parameter(s):
//    <1> position: The position to move the points
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All points in this Trail are moved to position
//               position.
//

	void setAllPointsToPosition (const Vector3& position);

//
//  setColour
//
//  Purpose: To change the display colour for this Trail.
//  Parameter(s):
//    <1> colour: The colour to display the trail in
//  Precondition(s):
//    <1> colour.isAllComponentsGreaterThanOrEqual(0.0)
//    <2> colour.isAllComponentsLessThanOrEqual(1.0)
//  Returns: N/A
//  Side Effect: This Trail is set to display in colour colour.
//

	void setColour (const Vector3& colour);

private:
//
//  invariant
//
//  Purpose: To determine if the class invarint is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	std::vector<Vector3> mv_points;
	unsigned int m_next_point;
	Vector3 m_colour;
};



#endif

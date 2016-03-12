//
//  FractalPerlinNoiseDummy.h
//

#ifndef FRACTAL_PERLIN_NOISE_DUMMY_H
#define FRACTAL_PERLIN_NOISE_DUMMY_H

#include "FractalPerlinNoiseInterface.h"

class Vector3;



//
//  FractalPerlinNoiseDummy
//
//  An dummy implementation of the FractalPerlinNoiseInterface
//    interface intended for testing client code.  A
//    FractalPerlinNoiseDummy can be queried at any given point
//    to return a value in the range [-1, 1] inclusive.  The
//    mean and median of the values returned is 0.  The result
//    at any point is consistant between successive calls to the
//    function.
//
//  The values are generated using sine waves instead of fractal
//    perlin noise.  These waves should have approximately the
//    same numeric properties as fractal perlin noise.
//
//  There are no parameters to set for a FractalPerlinNoiseDummy.
//    All FractalPerlinNoiseDummys are identical.
//

class FractalPerlinNoiseDummy : public FractalPerlinNoiseInterface
{
public:
//
//  Default Constructor
//
//  Purpose: To create a FractalPerlinNoiseDummy.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A FractalPerlinNoiseDummy is created.
//

	FractalPerlinNoiseDummy ();

//
//  Copy Constructor
//
//  Purpose: To create a FractalPerlinNoiseDummy as a copy of
//           another.
//  Parameter(s):
//    <1> original: The FractalPerlinNoiseDummy to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A FractalPerlinNoiseDummy is created as a copy
//               of original.
//

	FractalPerlinNoiseDummy (
	                   const FractalPerlinNoiseDummy& original);

//
//  Destructor
//
//  Purpose: To safely destroy an FractalPerlinNoiseDummy
//           without memory leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this FractalPerlinNoiseDummy is freed.
//

	virtual ~FractalPerlinNoiseDummy ();

//
//  Assignment Operator
//
//  Purpose: To modifiy this FractalPerlinNoiseDummy to be a
//           copy of another.
//  Parameter(s):
//    <1> original: The FractalPerlinNoiseDummy to copy
//  Precondition(s): N/A
//  Returns: A reference to this FractalPerlinNoiseDummy.
//  Side Effect: None.  All FractalPerlinNoises are already
//               identical.
//

	FractalPerlinNoiseDummy& operator= (
	                   const FractalPerlinNoiseDummy& original);

//
//  getAt
//
//  Purpose: To query this FractalPerlinNoiseDummy at the
//           specified point.
//  Parameter(s):
//    <1> position: The position to query the noise at
//  Precondition(s): N/A
//  Returns: The value of this FractalPerlinNoiseDummy at
//           position position.  This value is always in the
//           range [-1, 1].
//  Side Effect: N/A
//

	virtual double getAt (const Vector3& position) const;

//
//  getClone
//
//  Purpose: To create a dynamically-allocated copy of this
//           FractalPerlinNoiseInterface.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A pointer to a dynamically-allocated deep copy of
//           this FractalPerlinNoiseInterface.
//  Side Effect: N/A
//

	virtual FractalPerlinNoiseInterface* getClone () const;
};



#endif

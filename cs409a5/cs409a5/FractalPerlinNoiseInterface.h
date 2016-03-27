//
//  FractalPerlinNoiseInterface.h
//
//  An abstract interface for a 3D fractal perlin noise field.
//
//  DO NOT MODIFY THIS FILE
//

#ifndef FRACTAL_PERLIN_NOISE_INTERFACE_H
#define FRACTAL_PERLIN_NOISE_INTERFACE_H

class Vector3;



//
//  FractalPerlinNoiseInterface
//
//  An abstract interface for a 3D fractal perlin noise field.
//    A FractalPerlinNoiseInterface can be queried at any given
//    point to return a value in the range [-1, 1] inclusive.
//    The mean and median of the values returned is 0.  The
//    result at any point is consistant between successive calls
//    to the function.
//

class FractalPerlinNoiseInterface
{
public:
//
//  Destructor
//
//  Purpose: To safely destroy an FractalPerlinNoiseInterface
//           without memory leaks.  We need a virtual destructor
//           in the base class the that the destructor for the
//           correct derived class will be invoked.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory associated
//               with this FractalPerlinNoiseInterface is freed.
//

	virtual ~FractalPerlinNoiseInterface ()
	{ }  // do nothing

//
//  getAt
//
//  Purpose: To query this FractalPerlinNoiseInterface at the
//           specified point.
//  Parameter(s):
//    <1> position: The position to query the noise at
//  Precondition(s): N/A
//  Returns: The value of the perlin noise field for this
//           FractalPerlinNoiseInterface at position position.
//           This value is always in the range [-1, 1].
//  Side Effect: N/A
//

	virtual double getAt (const Vector3& position) const = 0;

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

	virtual FractalPerlinNoiseInterface* getClone () const = 0;

};



#endif

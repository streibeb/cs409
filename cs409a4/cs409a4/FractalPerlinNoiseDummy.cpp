//
//  FractalPerlinNoiseDummy.cpp
//

#include "../../ObjLibrary/Vector3.h"

#include "FractalPerlinNoiseInterface.h"
#include "FractalPerlinNoiseDummy.h"



FractalPerlinNoiseDummy :: FractalPerlinNoiseDummy ()
		: FractalPerlinNoiseInterface()
{
	// no member fields to initialize
}

FractalPerlinNoiseDummy :: FractalPerlinNoiseDummy (const FractalPerlinNoiseDummy& original)
		: FractalPerlinNoiseInterface(original)
{
	// no member fields to copy
}

FractalPerlinNoiseDummy :: ~FractalPerlinNoiseDummy ()
{
	// nothing to do in destructor
}

FractalPerlinNoiseDummy& FractalPerlinNoiseDummy :: operator= (
	                const FractalPerlinNoiseDummy& original)
{
	return *this;
}



double FractalPerlinNoiseDummy :: getAt (const Vector3& position) const
{
	const double SQRT_2 = 1.4142135623730950488016887242097;
	const double SQRT_3 = 1.7320508075688772935274463415059;

	return (sin(position.x) +
	        sin(position.y * SQRT_2) +
	        sin(position.z * SQRT_3)) / 3.0;
}

FractalPerlinNoiseInterface* FractalPerlinNoiseDummy :: getClone () const
{
	return new FractalPerlinNoiseDummy(*this);
}

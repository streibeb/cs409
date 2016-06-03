//
//  OrientationCube.cpp
//

#include <cassert>

#include "GetGlut.h"

#include "OrientationCube.h"

namespace
{
	static const unsigned int OCTAHEDRON_COUNT = 6;
	static const unsigned int OCTAHEDRON_COORDINATE_MAX = OCTAHEDRON_COUNT - 1;

	static const float CUBE_SIZE = 10.0f;
	static const float CUBE_OFFSET = CUBE_SIZE * -0.5f;
	static const float OCTAHEDRON_POSITION_FACTOR =
	                                     CUBE_SIZE / OCTAHEDRON_COORDINATE_MAX;

	static const float OCTAHEDRON_SIZE = 0.5;
	static const float COLOUR_FACTOR = 1.0f / OCTAHEDRON_COORDINATE_MAX;



	//
	//  setColour
	//
	//  Purpose: To activate the appropriate colour for the
	//           specified octahredon in the cube.
	//  Parameter(s):
	//    <1> x
	//    <2> y
	//    <3> z: The position in octahedron coordinate
	//  Precondtion(s):
	//    <1> x < OCTAHEDRON_COUNT
	//    <2> y < OCTAHEDRON_COUNT
	//    <3> z < OCTAHEDRON_COUNT
	//  Returns: N/A
	//  Side Effect: The OpenGL colour is set.
	//

	inline void setColour (unsigned int x,
	                       unsigned int y,
	                       unsigned int z)
	{
		assert(x < OCTAHEDRON_COUNT);
		assert(y < OCTAHEDRON_COUNT);
		assert(z < OCTAHEDRON_COUNT);

		glColor3f(x * COLOUR_FACTOR,
		          y * COLOUR_FACTOR,
		          z * COLOUR_FACTOR);
	}

	//
	//  drawOctahedron
	//
	//  Purpose: To display the appropriate octahedron for the
	//           specified position in the cube.
	//  Parameter(s):
	//    <1> x
	//    <2> y
	//    <3> z: The position in octahedron coordinate
	//  Precondtion(s):
	//    <1> x < OCTAHEDRON_COUNT
	//    <2> y < OCTAHEDRON_COUNT
	//    <3> z < OCTAHEDRON_COUNT
	//  Returns: N/A
	//  Side Effect: An appropriately-coloured octahedron is
	//               drawn at the correct position for
	//               octahedron coordinates (x, y, z).
	//

	void drawOctahedron (unsigned int x,
	                     unsigned int y,
	                     unsigned int z)
	{
		assert(x < OCTAHEDRON_COUNT);
		assert(y < OCTAHEDRON_COUNT);
		assert(z < OCTAHEDRON_COUNT);

		glPushMatrix();
			glTranslatef(x * OCTAHEDRON_POSITION_FACTOR + CUBE_OFFSET,
			             y * OCTAHEDRON_POSITION_FACTOR + CUBE_OFFSET,
			             z * OCTAHEDRON_POSITION_FACTOR + CUBE_OFFSET);
			glScalef(OCTAHEDRON_SIZE,
			         OCTAHEDRON_SIZE,
			         OCTAHEDRON_SIZE);

			setColour(x, y, z);
			glutSolidOctahedron();
		glPopMatrix();
	}

	//
	//  drawVertex
	//
	//  Purpose: To draw a vertex at the appropriate position
	//           for the specified octahredon in the cube.
	//  Parameter(s):
	//    <1> x
	//    <2> y
	//    <3> z: The position in octahedron coordinate
	//  Precondtion(s):
	//    <1> x < OCTAHEDRON_COUNT
	//    <2> y < OCTAHEDRON_COUNT
	//    <3> z < OCTAHEDRON_COUNT
	//  Returns: N/A
	//  Side Effect: The vertex is drawn to OpenGL.
	//

	inline void drawVertex (unsigned int x,
	                        unsigned int y,
	                        unsigned int z)
	{
		assert(x < OCTAHEDRON_COUNT);
		assert(y < OCTAHEDRON_COUNT);
		assert(z < OCTAHEDRON_COUNT);

		glVertex3f(x * OCTAHEDRON_POSITION_FACTOR + CUBE_OFFSET,
				   y * OCTAHEDRON_POSITION_FACTOR + CUBE_OFFSET,
				   z * OCTAHEDRON_POSITION_FACTOR + CUBE_OFFSET);
	}

	//
	//  drawWireSquareX
	//  drawWireSquareY
	//  drawWireSquareZ
	//
	//  Purpose: To draw an appropriately-coloured wire square
	//           around the cube.  The square is drawn around
	//           the indicated axis at the position along it
	//           specified.
	//  Parameter(s):
	//    <1> x: The distance along the X-axis in octahedron
	//           coordinates
	//    <1> y: The distance along the X-axis in octahedron
	//           coordinates
	//    <1> z: The distance along the X-axis in octahedron
	//           coordinates
	//  Precondtion(s):
	//    <1> x < OCTAHEDRON_COUNT
	//    <1> y < OCTAHEDRON_COUNT
	//    <1> z < OCTAHEDRON_COUNT
	//  Returns: N/A
	//  Side Effect: A square is drawn around the X-/Y-/Z-axis
	//               the size of the cube and around coordinate
	//               x/y/z.
	//

	void drawWireSquareX (unsigned int x)
	{
		glBegin(GL_LINE_LOOP);
			setColour (x, 0,                         0);
			drawVertex(x, 0,                         0);
			setColour (x, OCTAHEDRON_COORDINATE_MAX, 0);
			drawVertex(x, OCTAHEDRON_COORDINATE_MAX, 0);
			setColour (x, OCTAHEDRON_COORDINATE_MAX, OCTAHEDRON_COORDINATE_MAX);
			drawVertex(x, OCTAHEDRON_COORDINATE_MAX, OCTAHEDRON_COORDINATE_MAX);
			setColour (x, 0,                         OCTAHEDRON_COORDINATE_MAX);
			drawVertex(x, 0,                         OCTAHEDRON_COORDINATE_MAX);
		glEnd();
	}

	void drawWireSquareY (unsigned int y)
	{
		glBegin(GL_LINE_LOOP);
			setColour (0,                         y, 0);
			drawVertex(0,                         y, 0);
			setColour (OCTAHEDRON_COORDINATE_MAX, y, 0);
			drawVertex(OCTAHEDRON_COORDINATE_MAX, y, 0);
			setColour (OCTAHEDRON_COORDINATE_MAX, y, OCTAHEDRON_COORDINATE_MAX);
			drawVertex(OCTAHEDRON_COORDINATE_MAX, y, OCTAHEDRON_COORDINATE_MAX);
			setColour (0,                         y, OCTAHEDRON_COORDINATE_MAX);
			drawVertex(0,                         y, OCTAHEDRON_COORDINATE_MAX);
		glEnd();
	}

	void drawWireSquareZ (unsigned int z)
	{
		glBegin(GL_LINE_LOOP);
			setColour (0,                         0,                         z);
			drawVertex(0,                         0,                         z);
			setColour (OCTAHEDRON_COORDINATE_MAX, 0,                         z);
			drawVertex(OCTAHEDRON_COORDINATE_MAX, 0,                         z);
			setColour (OCTAHEDRON_COORDINATE_MAX, OCTAHEDRON_COORDINATE_MAX, z);
			drawVertex(OCTAHEDRON_COORDINATE_MAX, OCTAHEDRON_COORDINATE_MAX, z);
			setColour (0,                         OCTAHEDRON_COORDINATE_MAX, z);
			drawVertex(0,                         OCTAHEDRON_COORDINATE_MAX, z);
		glEnd();
	}
}



void drawOrientationCube ()
{
	// draw the cube of octahedrons
	for(unsigned int x = 0; x < OCTAHEDRON_COUNT; x++)
		for(unsigned int y = 0; y < OCTAHEDRON_COUNT; y++)
			for(unsigned int z = 0; z < OCTAHEDRON_COUNT; z++)
			{
				// only draw octahedrons on the edge of the cube
				if(x == 0 || x == OCTAHEDRON_COUNT - 1 ||
				   y == 0 || y == OCTAHEDRON_COUNT - 1 ||
				   z == 0 || z == OCTAHEDRON_COUNT - 1)
				{
					drawOctahedron(x, y, z);
				}
			}

	// draw wirefame lines
	for(unsigned int x = 0; x < OCTAHEDRON_COUNT; x++)
		drawWireSquareX(x);
	for(unsigned int y = 0; y < OCTAHEDRON_COUNT; y++)
		drawWireSquareY(y);
	for(unsigned int z = 0; z < OCTAHEDRON_COUNT; z++)
		drawWireSquareZ(z);
}


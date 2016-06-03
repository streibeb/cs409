//
//  OrientationCube.h
//
//  A module to display a cube of connected ocathedrons.  The
//    octahedrons are colour-coded based on their positions.
//

#ifndef ORIENTATION_CUBE_H
#define ORIENTATION_CUBE_H



//
//  drawOrientationCube
//
//  Purpose: To display the orientation cube using OpenGL
//           graphics.  The orientation cube is 6 octahedrons
//           across.  The octahedrons have a radius of 0.5 and
//           their centers range from (-5, -5, -5) to (5, 5, 5).
//           The octahedrons are colour-coded, with the X, Y and
//           Z coordinates determining the R, G, and B 
//           components of their colour.  There also lines
//           connecting adjacent octahedorns.
//  Parameter(s): N/A
//  Precondtion(s): N/A
//  Returns: N/A
//  Side Effect: The orientation cube is drawn centered on the
//               origin.
//

void drawOrientationCube ();



#endif

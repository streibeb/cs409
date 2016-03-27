//
//  RingSectorSize.h
//
//  A file contianing the ring sector size constant.  It doesn't
//    really belong with any one class and I don't want it to be
//    included (or defined) twice.
//
//  This is not a subclass of RingSector.
//

#ifndef RING_SECTOR_SIZE_H
#define RING_SECTOR_SIZE_H



//
//  RING_SECTOR_SIZE
//
//  The dimension of a ring sector along 1 axis.  Each ring
//    sector is a cube with this as the side length.
//

const double RING_SECTOR_SIZE = 500.0;

//
//  RING_SECTOR_HALF_SIZE
//
//  The distance from the center of a ring sector to the edge
//    along each axis.
//

const Vector3 RING_SECTOR_HALF_SIZE(RING_SECTOR_SIZE * 0.5,
                                    RING_SECTOR_SIZE * 0.5,
                                    RING_SECTOR_SIZE * 0.5);

//
//  RING_SECTORS_PER_KM3
//
//  The number of ring sectors in 1 km3.  This serves as a
//    conversion factor for ring density.
//

const double RING_SECTORS_PER_KM3 =
                                   (1000.0 / RING_SECTOR_SIZE) *
                                   (1000.0 / RING_SECTOR_SIZE) *
                                   (1000.0 / RING_SECTOR_SIZE);



//
//  RING_SECTOR_DRAW_FROM_CAMERA_COUNT
//
//  The ring sector that the camera is in is always draw.  This
//    is the number of ring sectors on each side of the camera
//    to draw.  For example, in 2D, a value of 3 would draw:
//
//  +---+---+---+---+---+---+---+
//  |   |   |   |   |   |   |   |
//  +---+---+---+---+---+---+---+
//  |   |   |   |   |   |   |   |
//  +---+---+---+---+---+---+---+
//  |   |   |   |   |   |   |   |
//  +---+---+---+---+---+---+---+
//  |   |   |   | C |   |   |   |
//  +---+---+---+---+---+---+---+
//  |   |   |   |   |   |   |   |
//  +---+---+---+---+---+---+---+
//  |   |   |   |   |   |   |   |
//  +---+---+---+---+---+---+---+
//  |   |   |   |   |   |   |   |
//  +---+---+---+---+---+---+---+
//
//  where C is the ring sector containing the camera.
//

const int RING_SECTOR_DRAW_FROM_CAMERA_COUNT = 4;

//
//  RING_PARTICLE_DRAW_DISTANCE_MAX
//
//  The maximum distance to draw a ring particle at.  Farther
//    ring particles are not displayed.
//

const double RING_PARTICLE_DRAW_DISTANCE_MAX =
                            RING_SECTOR_DRAW_FROM_CAMERA_COUNT *
                            RING_SECTOR_SIZE;



#endif

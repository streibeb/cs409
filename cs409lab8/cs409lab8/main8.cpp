//
//  Main7.cpp
//
#include <stdlib.h>
#include "GetGlut.h"
#include "Sleep.h"

#include "OrientationCube.h"
#include "ObjLibrary/Vector3.h"
#include "CoordinateSystem.h"

void initDisplay();
void keyboard(unsigned char key, int x, int y);
void special(int special_key, int x, int y);
void update();
void reshape(int w, int h);
void display();
void rotateCameraToVector(const Vector3& target_facing, double max_radians);

CoordinateSystem camera;
CoordinateSystem teapot(Vector3(10.0, -5.0, 20.0),
                        Vector3(0.0, 1.0, 0.0),
                        Vector3(0.0, 0.0, 1.0));

int main(int argc, char* argv[])
{
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Local Camera Coordinates");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	initDisplay();

	glutMainLoop();

	return 1;
}

void initDisplay()
{
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glColor3f(0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    Vector3 origin = Vector3::ZERO;
    Vector3 direction_to_origin;
	switch (key)
	{
        case 27: // on [ESC]
            exit(0); // normal exit
            break;
        case ' ':
            camera.moveForward(0.5);
            break;
        case 'w':
            camera.moveUp(0.5);
            break;
        case 'a':
            camera.moveRight(-0.5);
            break;
        case 's':
            camera.moveUp(-0.5);
            break;
        case 'd':
            camera.moveRight(0.5);
            break;
        case 'h':
            direction_to_origin = origin - camera.getPosition();
            camera.rotateToVector(direction_to_origin, 0.1);
            break;
        case 'r':
            camera.rotateToVector(teapot.getPosition(), 0.1);
            break;
        case ',':
            camera.rotateAroundForward(0.1);
            break;
        case '.':
            camera.rotateAroundForward(-0.1);
            break;
        case '/':
            camera.moveForward(-0.5);
            break;
        case 't':
            camera.setOrientation(Vector3::getRandomUnitVector());
            break;
        case 'u':
            camera.rotateUpright(0.1);
            break;
        case 'y':
            camera.calculateRandomOrientation();
            break;
	}
}

void special(int special_key, int x, int y)
{
	switch(special_key)
	{
        case GLUT_KEY_RIGHT:
            camera.rotateAroundUp(-0.1);
            break;
        case GLUT_KEY_LEFT:
            camera.rotateAroundUp(0.1);
            break;
        case GLUT_KEY_UP:
            camera.rotateAroundRight(0.1);
            break;
        case GLUT_KEY_DOWN:
            camera.rotateAroundRight(-0.1);
            break;
        case GLUT_KEY_END:
            camera.reset();
            break;
	}
}

void update()
{
	sleep(1.0 / 60.0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)w / (GLdouble)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display

	glLoadIdentity();
	// set up the camera here
    camera.setCamera();

	// camera is set up - any drawing before here will display incorrectly

	drawOrientationCube();
	
    // Draw teapot
    glColor3f(0.75f, 0.75f, 0.75f);
    glPushMatrix();
        teapot.applyTransformation();
        glFrontFace(GL_CW);
        glutSolidTeapot(2.0);
        glFrontFace(GL_CW);
    glPopMatrix();

	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

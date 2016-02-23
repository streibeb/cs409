//
//  Main6.cpp
//

#include <stdlib.h>
#include "GetGlut.h"
#include "Sleep.h"
#include "World.h"
#include "TimeSystem.h"

void init();
void initDisplay();
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void special(int special_key, int x, int y);
void specialUp(int special_key, int x, int y);
void update();
void reshape(int w, int h);
void display();

// Models
const float MOVE_SPEED_FAST = 500.0;
const float MOVE_SPEED_SLOW = 50.0;
const float THETA = 0.05;

CoordinateSystem camera;
World world;

// Improved keyboard stuff
bool key_pressed[256];
bool special_key_pressed[128];

int main(int argc, char* argv[])
{
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("CS 409 Assignment 3");
	glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

    TimeSystem::init(5, 60, 0.1);
    init();
    TimeSystem::markPauseEnd();
    
	glutMainLoop();

	return 1;
}

void init()
{
    for (int i = 0; i < 256; i++)
    {
        key_pressed[i] = false;
    }
    
    for (int i = 0; i < 128; i++)
    {
        special_key_pressed[i] = false;
    }
    
    initDisplay();
    camera.setPosition({70000.0, 0.0, 70000.0});
    world.init();
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
    key_pressed[key] = true;
    
	switch (key)
	{
        case 27: // on [ESC]
            exit(0); // normal exit
            break;
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
    key_pressed[key] = false;
}

void special(int special_key, int x, int y)
{
    special_key_pressed[special_key] = true;
    
	switch(special_key)
	{
        case GLUT_KEY_END:
            camera.reset();
            break;
	}
}

void specialUp(int special_key, int x, int y)
{
    special_key_pressed[special_key] = false;
}

void update()
{
    if (key_pressed['f'] || key_pressed['F'])
    {
        camera.moveForward(MOVE_SPEED_FAST);
    }
    if (key_pressed[' '])
    {
        camera.moveForward(MOVE_SPEED_SLOW);
    }
    if (key_pressed['w'] || key_pressed['W'])
    {
        camera.moveUp(MOVE_SPEED_SLOW);
    }
    if (key_pressed['a'] || key_pressed['A'])
    {
        camera.moveRight(-MOVE_SPEED_SLOW);
    }
    if (key_pressed['s'] || key_pressed['S'])
    {
        camera.moveUp(-MOVE_SPEED_SLOW);
    }
    if (key_pressed['d'] || key_pressed['D'])
    {
        camera.moveRight(MOVE_SPEED_SLOW);
    }
    if (key_pressed['h'])
    {
        Vector3 origin = Vector3::ZERO;
        Vector3 direction_to_origin;
        direction_to_origin = origin - camera.getPosition();
        camera.rotateToVector(direction_to_origin, THETA);
    }
    if (key_pressed[','] || key_pressed['<'])
    {
        camera.rotateAroundForward(THETA);
    }
    if (key_pressed['.'] || key_pressed['>'])
    {
        camera.rotateAroundForward(-THETA);
    }
    if (key_pressed['/'] || key_pressed['?'])
    {
        camera.moveForward(-MOVE_SPEED_SLOW);
    }
    if (special_key_pressed[GLUT_KEY_RIGHT])
    {
        camera.rotateAroundUp(-THETA);
    }
    if (special_key_pressed[GLUT_KEY_LEFT])
    {
        camera.rotateAroundUp(THETA);
    }
    if (special_key_pressed[GLUT_KEY_UP])
    {
        camera.rotateAroundRight(THETA);
    }
    if (special_key_pressed[GLUT_KEY_DOWN])
    {
        camera.rotateAroundRight(-THETA);
    }
    
	sleep(TimeSystem::getTimeToNextFrame());
    TimeSystem::markFrameEnd();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)w / (GLdouble)h, 1.0, 1000000.0);
	glMatrixMode(GL_MODELVIEW);

    TimeSystem::markPauseEnd();
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display

	glLoadIdentity();
	// set up the camera here
    camera.setCamera();

    // Draw the world
    world.drawSkybox(camera);
    world.drawSaturn();
    world.drawMoons();
    world.drawRings(camera);
    
	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

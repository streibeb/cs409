//
//  Main6.cpp
//

#include <stdlib.h>
#include "GetGlut.h"
#include "Sleep.h"
#include "TimeSystem.h"
#include "../../ObjLibrary/ObjModel.h"
#include "../../ObjLibrary/DisplayList.h"
#include "World.h"

void init();
void initDisplay();
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void special(int special_key, int x, int y);
void specialUp(int special_key, int x, int y);
void update();
void reshape(int w, int h);
void display();

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
	glutCreateWindow("CS 409 Assignment 4");
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
}

void specialUp(int special_key, int x, int y)
{
    special_key_pressed[special_key] = false;
}

void update()
{
    float TURN_SPEED = 0.05;
    if (key_pressed['f'] || key_pressed['F'])
    {
        world.player_ship.setSpeed(2500);
    }
    else if (key_pressed['s'] || key_pressed['S'])
    {
        world.player_ship.setSpeed(50);
        TURN_SPEED /= 5;
    }
    else
    {
        world.player_ship.setSpeed(250);
    }
    
    if (special_key_pressed[GLUT_KEY_RIGHT])
    {
        world.player_ship.rotateAroundUp(-TURN_SPEED);
    }
    if (special_key_pressed[GLUT_KEY_LEFT])
    {
        world.player_ship.rotateAroundUp(TURN_SPEED);
    }
    if (special_key_pressed[GLUT_KEY_UP])
    {
        world.player_ship.rotateAroundRight(TURN_SPEED);
    }
    if (special_key_pressed[GLUT_KEY_DOWN])
    {
        world.player_ship.rotateAroundRight(-TURN_SPEED);
    }
    
    if (key_pressed[' '])
    {
        if (world.player_ship.isReloaded())
        {
            world.addBullet(world.player_ship.getPosition(),
                            world.player_ship.getForward(),
                            world.player_ship.getId());
            world.player_ship.markReloading();
        }
    }
    
    world.updateAll();
    
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
    world.player_ship.setupCamera();
    CoordinateSystem camera = world.player_ship.getCameraCoordinateSystem();
    
    // Draw player ship
    world.player_ship.draw();
    world.draw(camera.getForward(), camera.getUp());
    
	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

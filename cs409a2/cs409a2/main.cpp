//
//  Main6.cpp
//

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <stdlib.h>
#include <string>
#include "GetGlut.h"
#include "Sleep.h"

// I did this so I could have a common ObjLibrary shared
// amongst all assignments
#include "../../ObjLibrary/ObjModel.h"
#include "CoordinateSystem.h"

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
struct objectInfo
{
    std::string filename;
    float radius;
    float x;
    float z;
};

const float MOVE_SPEED_FAST = 500.0;
const float MOVE_SPEED_SLOW = 50.0;
const float THETA = 0.05;

CoordinateSystem camera;
ObjModel skybox;
ObjModel saturn;
objectInfo saturnInfo = {
    "Models/Saturn.obj", 60000.f, 0.0f, 0.0f
};
ObjModel moons[10];
objectInfo moonInfo[10] = {
    {"Models/MoonA.obj", 3300.f, 0.f, 140000.f},
    {"Models/MoonB.obj", 4300.f, 85000.f, 75000.f},
    {"Models/MoonC.obj", 2000.f, 130000.f, 40000.f},
    {"Models/MoonD.obj", 3400.f, 110000.f, -60000.f},
    {"Models/MoonE.obj", 5000.f, 100000.f, -70000.f},
    {"Models/MoonF.obj", 3100.f, 20000.f, -135000.f},
    {"Models/MoonG.obj", 2600.f, -60000.f, -80000.f},
    {"Models/MoonH.obj", 2200.f, -95000.f, -70000.f},
    {"Models/MoonI.obj", 4700.f, -90000.f, -40000.f},
    {"Models/MoonJ.obj", 3800.f, -100000.f, 50000.f}
};
ObjModel ring;
objectInfo ringInfo = {
    "Models/Ring.obj", 60000.f, 0.0f, 0.0f
};

// Improved keyboard stuff
bool key_pressed[256];
bool special_key_pressed[128];

int main(int argc, char* argv[])
{
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("CS 409 Assignment 1");
	glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
	glutIdleFunc(update);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

    init();
    
	glutMainLoop();

	return 1;
}

void init()
{
    for (int i = 0; i < 256; i++)
    {
        key_pressed[i] = false;
    }
    
    for (int i = 0; i < 20; i++)
    {
        special_key_pressed[i] = false;
    }
    
    initDisplay();
    camera.setPosition(Vector3(70000.0, 0.0, 70000.0));
    skybox.load("Models/Skybox.obj");
    saturn.load(saturnInfo.filename);
    for (int i = 0; i < 10; i++)
    {
        moons[i].load(moonInfo[i].filename);
    }
    ring.load(ringInfo.filename);
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
        camera.moveLeft(MOVE_SPEED_SLOW);
    }
    if (key_pressed['s'] || key_pressed['S'])
    {
        camera.moveDown(MOVE_SPEED_SLOW);
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
        camera.setRoll(THETA);
    }
    if (key_pressed['.'] || key_pressed['>'])
    {
        camera.setRoll(-THETA);
    }
    if (key_pressed['/'] || key_pressed['?'])
    {
        camera.moveBackward(MOVE_SPEED_SLOW);
    }
    if (special_key_pressed[GLUT_KEY_RIGHT])
    {
        camera.setYaw(-THETA);
    }
    if (special_key_pressed[GLUT_KEY_LEFT])
    {
        camera.setYaw(THETA);
    }
    if (special_key_pressed[GLUT_KEY_UP])
    {
        camera.setPitch(THETA);
    }
    if (special_key_pressed[GLUT_KEY_DOWN])
    {
        camera.setPitch(-THETA);
    }
    
	sleep(1.0 / 60.0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)w / (GLdouble)h, 1.0, 1000000.0);
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
    
    // Draw Skybox
    glPushMatrix();
        glTranslatef(camera.getPosition().x,
                     camera.getPosition().y,
                     camera.getPosition().z);
        glDepthMask(GL_FALSE);
        skybox.draw();
        glDepthMask(GL_TRUE);
    glPopMatrix();

    // Draw Saturn
    glPushMatrix();
        glTranslatef(saturnInfo.x, 0.f, saturnInfo.z);
        glScalef(saturnInfo.radius, saturnInfo.radius, saturnInfo.radius);
        saturn.draw();
    glPopMatrix();
    
    // Draw Moons
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
            glTranslatef(moonInfo[i].x, 0.f, moonInfo[i].z);
            glScalef(moonInfo[i].radius, moonInfo[i].radius, moonInfo[i].radius);
            moons[i].draw();
        glPopMatrix();
    }
    
    // Draw rings
    glPushMatrix();
        glTranslatef(ringInfo.x, 0.f, ringInfo.z);
        glScalef(ringInfo.radius, ringInfo.radius, ringInfo.radius);
        ring.draw();
    glPopMatrix();
    
	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

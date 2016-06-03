//
//  Main4.cpp
//

#include <cassert>
#include <cmath>
#include <string>
#include <iostream>

#include "GetGlut.h"
#include "Sleep.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/TextureManager.h"

using namespace std;

void init();
void initDisplay();
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);
void update();
void display();

ObjModel spiky_obj;
ObjModel bucket;
ObjModel skybox;

int yRotation = 0;

int main(int argc, char* argv[])
{
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Models with Textures");
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutDisplayFunc(display);

	init();

	glutMainLoop();

	return 1;
}

void init()
{
    initDisplay();
    spiky_obj.load("Spiky.obj");
    bucket.load("firebucket.obj");
    skybox.load("Skybox2.obj");
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
	switch (key)
	{
	case 27: // on [ESC]
		exit(0); // normal exit
		break;
	}
}

void reshape(int w, int h)
{
	glViewport (0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)w / (GLdouble)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void update()
{
	//update your variables here
    ++yRotation %= 360;
    
	sleep(1.0 / 60.0);
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(2.0, 1.0, 4.0,
	          0.0, 0.0, 0.0,
	          0.0, 1.0, 0.0);
    
    glPushMatrix();
        glTranslatef(2.0, 1.0, 4.0);
        glRotatef(yRotation, 0.f, 1.f, 0.f);
        glDepthMask(GL_FALSE);
        skybox.draw();
        glDepthMask(GL_TRUE);
    glPopMatrix();

	// display positive X, Y, and Z axes near origin
	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(2.0, 0.0, 0.0);
		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 2.0, 0.0);
		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 2.0);
	glEnd();

	// draw a purple wireframe cube
	glColor3d(1.0, 0.0, 1.0);
	glPushMatrix();
		glTranslated(0.0, 0.0, 0.0);
		glRotated(45, 0.0, 1.0, 0.0);
		glScaled(1.0, 1.0, 1.0);
		glutWireCube(1.0);
	glPopMatrix();
    
    glColor3d(1.0, 0.5, 0.0);
    for (int i = 1; i <= 3; i++)
    {
        glPushMatrix();
            glTranslatef(i, 0.f, 0.f);
            glScalef(0.45f, 0.45f, 0.45f);
            spiky_obj.draw();
        glPopMatrix();
    }
    
    for (int i = 1; i <= 3; i++)
    {
        glPushMatrix();
            glTranslatef(-i, 0.f, 0.f);
            glScalef(0.005f, 0.005f, 0.005f);
            bucket.draw();
        glPopMatrix();
    }
    
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        TextureManager::activate("tetrahedron.bmp");
        glColor3d(1.0, 1.0, 1.0);
        glScalef(0.5f, 0.5f, 0.5f);
    glTranslatef(0.f, 0.f, -3.0f);
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2d(0.0, 0.0);
            glVertex3d(-1.0, 0.0,  1.0);
            glTexCoord2d(0.0, 1.0);
            glVertex3d( 1.0, 0.0,  1.0);
            glTexCoord2d(0.5, 0.0);
            glVertex3d( 0.0, 2.0,  0.0);
            glTexCoord2d(0.5, 1.0);
            glVertex3d( 0.0, 0.0, -1.0);
            glTexCoord2d(1.0, 0.0);
            glVertex3d(-1.0, 0.0,  1.0);
            glTexCoord2d(1.0, 1.0);
            glVertex3d( 1.0, 0.0,  1.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
	glutSwapBuffers();
}

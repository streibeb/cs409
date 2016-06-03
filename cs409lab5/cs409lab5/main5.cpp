//
//  Main.cpp
//

#include <cassert>
#include <cmath>
#include <string>

#include "GetGlut.h"
#include "Sleep.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/TextureManager.h"
#include "ObjLibrary/DisplayList.h"

using namespace std;

void init();
void initDisplay();
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);
void update();
void display();

ObjModel bucket;
DisplayList bucket_list;

int main(int argc, char* argv[])
{
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Display Lists and Better Input");
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
	bucket.load("firebucket.obj");
    bucket_list = bucket.getDisplayList();
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
	gluPerspective(60, (GLdouble)w / (GLdouble)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void update()
{
	//update your variables here
	
	sleep(1.0 / 60.0);
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display

	glLoadIdentity();
	gluLookAt(5.0, 3.0, 4.0,
	          0.0, 0.0, 0.0,
	          0.0, 1.0, 0.0);
	// set up the camera - any drawing before here will display incorrectly

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

	// draw the buckets
	for (int i = 1; i < 4; i++)
	{
		glPushMatrix();
			glTranslated((double)(-i), 0.0, 0.0);
			glRotated(0.0, 0.0, 1.0, 0.0);
			glScaled(0.005, 0.005, 0.005);
			bucket_list.draw();
		glPopMatrix();
	}
	
	// draw the tetrahedron
	glPushMatrix();
		glScaled(0.5, 0.5, 0.5);

		// draw the tetrahedron
		glEnable(GL_TEXTURE_2D);
		TextureManager::activate("tetrahedron.bmp");
		glColor3d(1.0, 1.0, 1.0);
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

	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

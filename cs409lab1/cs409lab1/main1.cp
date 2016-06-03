#include <stdlib.h>
#include <GLUT/glut.h>
#include "Sleep.h"

// prototypes
void display ();
void idle();
void keyboard(unsigned char key, int x, int y);
void special(int special_key, int x, int y);

//declare global variables here
float theta = 0.0;
float xPosition = 0.0;
float xIncrement = 0.1;
float yPosition = 0.0;
float yIncrement = 0.1;

int main (int argc, char** argv)
{
	// initialize OpenGL; pass it the command line arguments
	glutInit(&argc, argv);	
	
	glutCreateWindow("First OpenGL Program");
	glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
	glutMainLoop();
	return 1;
}

void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef(xPosition, yPosition, 0.0f);
        glScalef(0.25f, 0.25f, 0.25f);
        glColor3f(0.0, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
            glVertex2f( 0.0, 0.5);
            glVertex2f( 0.5, 0.5);
            glVertex2f(0.5, -0.5);
        glEnd();
    glPopMatrix();
    
    glColor3f(1.0, 0.5f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glRotatef(theta, 0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
            glVertex2f(0.0, -0.5);
            glVertex2f(-0.5, 0.0);
            glVertex2f( 0.0, 0.5);
            glVertex2f( 0.5, 0.5);
            glVertex2f(0.5, -0.5);
        glEnd();
    glPopMatrix();
    
	// ensure that all buffered OpenGL commands get done
	glFlush();	
}

void idle()
{
//    theta += 2.0;
//    if (theta >= 360.0)
//        theta -= 360.0;
//    xPosition += xIncrement;
//    if (xPosition > 1 || xPosition < -1)
//        xIncrement = -xIncrement;
    //sleep(0.01);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'A':
        case 'a':
            xPosition -= xIncrement;
            break;
        case 'D':
        case 'd':
            xPosition += xIncrement;
            break;
        case 'W':
        case 'w':
            yPosition += yIncrement;
            break;
        case 'S':
        case 's':
            yPosition -= yIncrement;
            break;

        case 'R':
        case 'r':
            xPosition = 0.0;
            break;
        case 27:
            exit(0);
            break;
    }
}

void special(int special_key, int x, int y)
{
    switch (special_key)
    {
        case GLUT_KEY_LEFT:
            theta -= 2.0;
            if (theta <= 0)
                theta += 360.0;
            break;
        case GLUT_KEY_RIGHT:
            theta += 2.0;
            if (theta >= 360.0)
                theta -= 360.0;
            break;
    }
}
//
//  Main6.cpp
//

#include <stdlib.h>
#include "GetGlut.h"
#include "Sleep.h"
#include "World_old.h"
#include "TimeSystem.h"

#include "WorldExplosions.h"
#include "PhysicsObject.h"
#include "Planetoid.h"
#include "Ship.h"
#include "Bullet.h"

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
const int MOON_COUNT = 10;
const int SHIP_COUNT = 250;
const int BULLET_COUNT = 100;

struct objInfo
{
    std::string filename;
    float radius;
    Vector3 position;
};

objInfo moonInfo[MOON_COUNT] = {
    {"Models/MoonA.obj", 3300.f, {0.f, 0.f, 140000.f}},
    {"Models/MoonB.obj", 4300.f, {85000.f, 0.f, 75000.f}},
    {"Models/MoonC.obj", 2000.f, {130000.f, 0.f, 40000.f}},
    {"Models/MoonD.obj", 3400.f, {110000.f, 0.f, -60000.f}},
    {"Models/MoonE.obj", 5000.f, {100000.f, 0.f, -70000.f}},
    {"Models/MoonF.obj", 3100.f, {20000.f, 0.f, -135000.f}},
    {"Models/MoonG.obj", 2600.f, {-60000.f, 0.f, -80000.f}},
    {"Models/MoonH.obj", 2200.f, {-95000.f, 0.f, -70000.f}},
    {"Models/MoonI.obj", 4700.f, {-90000.f, 0.f, -40000.f}},
    {"Models/MoonJ.obj", 3800.f, {-100000.f, 0.f, 50000.f}}
};

WorldExplosions explosions;
World_old world;

Ship ships[SHIP_COUNT];
Ship player_ship;
Bullet bullets[BULLET_COUNT];
int nextBullet = 0;

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
    explosions.init();
    world.init();
    
    // Ship Init
    ObjModel s;
    s.load("Models/Grapple.obj");
    DisplayList s_dl = s.getDisplayList();
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        PhysicsObjectId s_id = PhysicsObjectId(PhysicsObjectId::TYPE_SHIP,
                                               PhysicsObjectId::FLEET_ENEMY,
                                               i);
        
        int moonIndex = rand() % MOON_COUNT;
        Vector3 pos = moonInfo[moonIndex].position
        + Vector3::getRandomUnitVector()
        * (moonInfo[moonIndex].radius + 500.0);
        
        ships[i].initPhysics(s_id, pos, 100.f, Vector3::getRandomUnitVector(), s_dl, 1.f);
        ships[i].setHealth(1);
        ships[i].setAmmo(0);
        ships[i].setSpeed(0.f);
    }
    
    // Player Ship Init
    PhysicsObjectId ps_id = PhysicsObjectId(PhysicsObjectId::TYPE_SHIP,
                                            PhysicsObjectId::FLEET_PLAYER,
                                            0);
    Vector3 pos = Vector3(0.f, 15000.f, 140000.f);
    player_ship.initPhysics(ps_id, pos, 10.f, Vector3::getRandomUnitVector(), s_dl, 1.f);
    player_ship.setHealth(10);
    player_ship.setAmmo(8);
    player_ship.setSpeed(250.f);

    ObjModel b;
    b.load("Models/Bolt.obj");
    DisplayList b_dl = b.getDisplayList();
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        bullets[i].initPhysics(PhysicsObjectId::TYPE_BULLET, {0, 0, 0}, 10.f, {0, 0, 0}, b_dl, 1.f);
    }
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
        case ' ':
            if (player_ship.isReloaded())
            {
                printf("Can't fire bullet\n");
                break;
            }
            bullets[nextBullet].fire(player_ship.getPosition(),
                                     player_ship.getForward(),
                                     player_ship.getId());
            nextBullet = (nextBullet + 1) % BULLET_COUNT;
            player_ship.markReloading();
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
        player_ship.setSpeed(2500);
    }
    else if (key_pressed['s'] || key_pressed['S'])
    {
        player_ship.setSpeed(50);
        TURN_SPEED /= 5;
    }
    else
    {
        player_ship.setSpeed(250);
    }
    
    if (special_key_pressed[GLUT_KEY_RIGHT])
    {
        player_ship.rotateAroundUp(-TURN_SPEED);
    }
    if (special_key_pressed[GLUT_KEY_LEFT])
    {
        player_ship.rotateAroundUp(TURN_SPEED);
    }
    if (special_key_pressed[GLUT_KEY_UP])
    {
        player_ship.rotateAroundRight(TURN_SPEED);
    }
    if (special_key_pressed[GLUT_KEY_DOWN])
    {
        player_ship.rotateAroundRight(-TURN_SPEED);
    }
    
    player_ship.update(explosions);
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        ships[i].update(explosions);
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        bullets[i].update(explosions);
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
    player_ship.setupCamera();

    // Draw the world
    CoordinateSystem camera = player_ship.getCameraCoordinateSystem();
    Vector3 p = camera.getPosition();
    world.drawSkybox(camera);
    world.drawSaturn();
    world.drawMoons();
    world.drawRings(camera);
    
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        ships[i].draw();
    }
    player_ship.draw();
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        bullets[i].draw();
    }
    
	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

//
//  TestUnitAi.cpp
//

#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "GetGlut.h"

#include "Sleep.h"
#include "Pi.h"
#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/ObjModel.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/SpriteFont.h"

#include "TimeSystem.h"
#include "PhysicsObjectId.h"
#include "WorldTestUnitAi.h"
#include "Trail.h"
#include "TestUnitAi.h"

//  This links to FleetNameSteeringBehaviour.lib in Visual Studio
//  Using g++ (or similar), you will need a command line argument
# pragma comment (lib, "SteeringBehaviour.lib")

using namespace std;
namespace
{
	const double FIELD_OF_VIEW_Y     =    60.0;
	const double CLIPPING_PLANE_NEAR =     1.0;
	const double CLIPPING_PLANE_FAR  = 40000.0;

	const unsigned int WINDOW_WIDTH_DEFAULT  = 640;
	const unsigned int WINDOW_HEIGHT_DEFAULT = 480;
	unsigned int g_window_width  = WINDOW_WIDTH_DEFAULT;
	unsigned int g_window_height = WINDOW_HEIGHT_DEFAULT;

	SpriteFont g_font;

	const float FRAME_RATE_MIN       =  5.0f;
	const float FRAME_RATE_DESIRED   = 60.0f;
	const float FRAME_RATE_SMOOTHING =  0.1f;
	const bool  FRAME_RATE_SHOW      = false;

	const double SLEEP_TIME_PAUSED = 1.0 / 40.0;
	bool g_is_paused = false;

	const unsigned int KEY_PRESSED_ARRAY_SIZE  = 256 + 5;
	const unsigned int KEY_PRESSED_ARROW_LEFT  = 256;
	const unsigned int KEY_PRESSED_ARROW_RIGHT = 257;
	const unsigned int KEY_PRESSED_ARROW_UP    = 258;
	const unsigned int KEY_PRESSED_ARROW_DOWN  = 259;
	const unsigned int KEY_PRESSED_ARROW_END   = 260;
	bool ga_is_key_pressed[KEY_PRESSED_ARRAY_SIZE];

	const double CAMERA_ROTATION_Y_DEFAULT = QUARTER_PI;
	const double CAMERA_ROTATION_Z_DEFAULT = SIXTH_PI;
	const double CAMERA_ROTATION_Z_MIN = PI * -0.499999;
	const double CAMERA_ROTATION_Z_MAX = PI *  0.499999;
	const double CAMERA_DISTANCE  = 10000.0;
	const double CAMERA_TURN_RATE = 0.05;  // radians
	double g_camera_rotation_y = CAMERA_ROTATION_Y_DEFAULT;
	double g_camera_rotation_z = CAMERA_ROTATION_Z_DEFAULT;

	WorldTestUnitAi* gp_world = NULL;

	const double POSITION_MARKER_SCALE = 100.0;
	DisplayList g_position_marker_list;

	double       g_reset_time;
	unsigned int g_reset_frame_number;

	const unsigned int RING_PARTICLE_INCREMENT   = 250;
	const unsigned int RING_PARTICLE_VERY_DENSE  = 2100;
	const unsigned int RING_PARTICLE_DENSE       = 1700;
	const unsigned int RING_PARTICLE_NORMAL      = 1300;
	const unsigned int RING_PARTICLE_SPARSE      =  900;
	const unsigned int RING_PARTICLE_VERY_SPARSE =  500;
	const unsigned int RING_PARTICLE_FRINGE      =    1;

	const Vector3 TARGET_COLOUR(1.0, 1.0, 1.0);
	const Vector3 AGENT_COLOUR(1.0, 1.0, 0.0);

	const unsigned int TRAIL_POINT_COUNT = 2500;
	Trail g_target_trail(TRAIL_POINT_COUNT, TARGET_COLOUR);
	Trail g_agent_trail (TRAIL_POINT_COUNT, AGENT_COLOUR);
	bool  g_target_trail_reset = false;

	const double AXES_SCALE = 5000.0;
	bool g_draw_commands   = true;
	bool g_draw_axes       = false;
	bool g_draw_orrery     = false;
	bool g_draw_markers    = false;
	bool g_draw_trails     = true;
	bool g_draw_statistics = true;

}  // end of anonymous namespace



int main (int argc, char* argv[])
{
	glutInitWindowSize(WINDOW_WIDTH_DEFAULT, WINDOW_HEIGHT_DEFAULT);
	glutInitWindowPosition(0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Unit AI test program");
	glutKeyboardFunc(&keyboard);
	glutKeyboardUpFunc(&keyboardUp);
	glutSpecialFunc(&special);
	glutSpecialUpFunc(&specialUp);
	glutIdleFunc(&update);
	glutReshapeFunc(&reshape);
	glutDisplayFunc(&display);

	initDisplay();
	g_font.load("FontCourierNew10Bold.bmp");
	loadModels();
	TimeSystem::init(FRAME_RATE_MIN, FRAME_RATE_DESIRED, FRAME_RATE_SMOOTHING);
	gp_world = new WorldTestUnitAi();
	resetAll();
	TimeSystem::markPauseEnd();

	glutMainLoop();

	return 1;
}

void initDisplay ()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);

	glutPostRedisplay();
}

void loadModels ()
{
	WorldTestUnitAi::loadModels();

	g_position_marker_list.begin();
		glPushMatrix();
			glScaled(POSITION_MARKER_SCALE, POSITION_MARKER_SCALE, POSITION_MARKER_SCALE);
			glutWireOctahedron();
		glPopMatrix();
	g_position_marker_list.end();
}

void resetAll ()
{
	assert(gp_world != NULL);
	gp_world->reset();

	g_target_trail.setAllPointsToPosition(gp_world->getPosition(WorldTestUnitAi::ID_TARGET));
	g_agent_trail .setAllPointsToPosition(gp_world->getPosition(WorldTestUnitAi::ID_AGENT));
	g_target_trail_reset = false;

	g_camera_rotation_y = CAMERA_ROTATION_Y_DEFAULT;
	g_camera_rotation_z = CAMERA_ROTATION_Z_DEFAULT;

	TimeSystem::markPauseEnd();
	g_reset_time         = TimeSystem::getFrameStartTime();
	g_reset_frame_number = TimeSystem::getFrameNumber();
}



void keyboard (unsigned char key, int x, int y)
{
	ga_is_key_pressed[getFixedKeyCode(key)] = true;
}

void keyboardUp(unsigned char key, int x, int y)
{
	ga_is_key_pressed[getFixedKeyCode(key)] = false;
}

unsigned char getFixedKeyCode (unsigned char key)
{
	if(key >= 'a' && key <= 'z')
		return key - 'a' + 'A';
	if(key == '?')
		return '/';
	if(key == '=')
		return '+';
	if(key == '_')
		return '-';

	return key;
}

void special(int special_key, int x, int y)
{
	switch(special_key)
	{
	case GLUT_KEY_RIGHT:
		ga_is_key_pressed[KEY_PRESSED_ARROW_LEFT]  = true;
		break;
	case GLUT_KEY_LEFT:
		ga_is_key_pressed[KEY_PRESSED_ARROW_RIGHT] = true;
		break;
	case GLUT_KEY_UP:
		ga_is_key_pressed[KEY_PRESSED_ARROW_UP]    = true;
		break;
	case GLUT_KEY_DOWN:
		ga_is_key_pressed[KEY_PRESSED_ARROW_DOWN]  = true;
		break;
	case GLUT_KEY_END:
		ga_is_key_pressed[KEY_PRESSED_ARROW_END]  = true;
		break;
	}
}

void specialUp(int special_key, int x, int y)
{
	switch(special_key)
	{
	case GLUT_KEY_RIGHT:
		ga_is_key_pressed[KEY_PRESSED_ARROW_LEFT]  = false;
		break;
	case GLUT_KEY_LEFT:
		ga_is_key_pressed[KEY_PRESSED_ARROW_RIGHT] = false;
		break;
	case GLUT_KEY_UP:
		ga_is_key_pressed[KEY_PRESSED_ARROW_UP]    = false;
		break;
	case GLUT_KEY_DOWN:
		ga_is_key_pressed[KEY_PRESSED_ARROW_DOWN]  = false;
		break;
	case GLUT_KEY_END:
		ga_is_key_pressed[KEY_PRESSED_ARROW_END]  = false;
		break;
	}
}



void update ()
{
	if(g_is_paused)
	{
		handleInputPaused();
		sleep(SLEEP_TIME_PAUSED);
	}
	else
	{
		assert(gp_world != NULL);
		gp_world->update();
		if(gp_world->isAlive(WorldTestUnitAi::ID_TARGET))
		{
			Vector3 trail_point = gp_world->getPosition(WorldTestUnitAi::ID_TARGET);
			if(g_target_trail_reset)
			{
				g_target_trail.setAllPointsToPosition(trail_point);
				g_target_trail_reset = false;
			}
			else
				g_target_trail.addPoint(trail_point);
		}
		else
			g_target_trail_reset = true;

		assert(gp_world->isAlive(WorldTestUnitAi::ID_AGENT));
		g_agent_trail.addPoint(gp_world->getPosition(WorldTestUnitAi::ID_AGENT));

		handleInputRunning();

		if(FRAME_RATE_SHOW)
			cout << "Frame rate: " << (1.0f / TimeSystem::getFrameDuration()) << endl;
		float sleep_time = TimeSystem::getTimeToNextFrame();
		if(sleep_time > 0.0f)
			sleep(sleep_time);
		TimeSystem::markFrameEnd();
	}

	glutPostRedisplay();
}

void handleInputRunning ()
{
	handleInputBoth();

	if(ga_is_key_pressed[' '])
	{
		g_is_paused = true;
		if(FRAME_RATE_SHOW)
			cout << "Paused" << endl;

		// don't unpause until the user presses the key again
		ga_is_key_pressed[' '] = false;
	}
}

void handleInputPaused ()
{
	handleInputBoth();

	if(ga_is_key_pressed[' '])
	{
		TimeSystem::markPauseEnd();
		g_is_paused = false;
		if(FRAME_RATE_SHOW)
			cout << "Unpaused" << endl;

		// don't re-pause until the user presses the key again
		ga_is_key_pressed[' '] = false;
	}
}

void handleInputBoth ()
{
	if(ga_is_key_pressed[27]) // on [ESC]
		exit(0); // normal exit

	// turn camera based on keyboard input
	if(ga_is_key_pressed[KEY_PRESSED_ARROW_RIGHT])
		g_camera_rotation_y -= CAMERA_TURN_RATE;
	if(ga_is_key_pressed[KEY_PRESSED_ARROW_LEFT])
		g_camera_rotation_y += CAMERA_TURN_RATE;
	if(ga_is_key_pressed[KEY_PRESSED_ARROW_UP])
	{
		g_camera_rotation_z += CAMERA_TURN_RATE;
		if(g_camera_rotation_z > CAMERA_ROTATION_Z_MAX)
			g_camera_rotation_z = CAMERA_ROTATION_Z_MAX;
	}
	if(ga_is_key_pressed[KEY_PRESSED_ARROW_DOWN])
	{
		g_camera_rotation_z -= CAMERA_TURN_RATE;
		if(g_camera_rotation_z < CAMERA_ROTATION_Z_MIN)
			g_camera_rotation_z = CAMERA_ROTATION_Z_MIN;
	}

	if(ga_is_key_pressed['-'])
	{
		unsigned int ring_particle_count = gp_world->getRingParticleCount();
		if(ring_particle_count > RING_PARTICLE_INCREMENT)
			gp_world->setRingParticleCount(ring_particle_count - RING_PARTICLE_INCREMENT);
		else
			gp_world->setRingParticleCount(0);

		// only one change per key press
		ga_is_key_pressed['-'] = false;
	}
	if(ga_is_key_pressed['+'])
	{
		unsigned int ring_particle_count_new = gp_world->getRingParticleCount() + RING_PARTICLE_INCREMENT;
		if(ring_particle_count_new > WorldTestUnitAi::RING_PARTICLE_COUNT_MAX)
			gp_world->setRingParticleCount(WorldTestUnitAi::RING_PARTICLE_COUNT_MAX);
		else
			gp_world->setRingParticleCount(ring_particle_count_new);

		// only one change per key press
		ga_is_key_pressed['+'] = false;
	}

	if(ga_is_key_pressed[KEY_PRESSED_ARROW_END])
	{
		resetAll();

		// only one reset per key press
		ga_is_key_pressed[KEY_PRESSED_ARROW_END] = false;
	}

	// display options
	if(ga_is_key_pressed['1'])
	{
		g_draw_commands = !g_draw_commands;
		ga_is_key_pressed['1'] = false;  // only switch once
	}
	if(ga_is_key_pressed['2'])
	{
		g_draw_axes = !g_draw_axes;
		ga_is_key_pressed['2'] = false;  // only switch once
	}
	if(ga_is_key_pressed['3'])
	{
		g_draw_orrery = !g_draw_orrery;
		ga_is_key_pressed['3'] = false;  // only switch once
	}
	if(ga_is_key_pressed['4'])
	{
		g_draw_markers = !g_draw_markers;
		ga_is_key_pressed['4'] = false;  // only switch once
	}
	if(ga_is_key_pressed['5'])
	{
		g_draw_trails = !g_draw_trails;
		ga_is_key_pressed['5'] = false;  // only switch once
	}
	if(ga_is_key_pressed['6'])
	{
		g_draw_statistics = !g_draw_statistics;
		ga_is_key_pressed['6'] = false;  // only switch once
	}
}



void reshape (int w, int h)
{
	g_window_width  = w;
	g_window_height = h;

	glViewport (0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW_Y,
	               (GLdouble)w / (GLdouble)h,
	               CLIPPING_PLANE_NEAR,
	               CLIPPING_PLANE_FAR);
	glMatrixMode(GL_MODELVIEW);

	TimeSystem::markPauseEnd();  // doesn't apply to moving the window, unfortunately
	glutPostRedisplay();
}

void display ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the screen - any drawing before here will not display

	Vector3 camera_position = Vector3(CAMERA_DISTANCE, 0.0, 0.0);
	camera_position.rotateZ(g_camera_rotation_z);
	camera_position.rotateY(g_camera_rotation_y);

	Vector3 camera_up = Vector3::UNIT_Y_PLUS;
	camera_up.rotateZ(g_camera_rotation_z);
	camera_up.rotateY(g_camera_rotation_y);

	glLoadIdentity();
	gluLookAt(camera_position.x, camera_position.y, camera_position.z,
	          0.0,               0.0,               0.0,
	          camera_up.x,       camera_up.y,       camera_up.z);
	// camera is set up - any drawing before here will display incorrectly

	// draw world
	Vector3 camera_forward = -camera_position.getNormalized();
	assert(gp_world != NULL);
	gp_world->draw(camera_forward, camera_up, g_draw_orrery);
	if(g_draw_axes)
	{
		glLineWidth(2.0);
		drawAxes(AXES_SCALE);
		glLineWidth(1.0);
		drawAxes(-AXES_SCALE);
	}

	// draw target extras
	assert(gp_world != NULL);
	if(g_draw_markers && gp_world->isAlive(WorldTestUnitAi::ID_TARGET))
	{
		glColor3d(TARGET_COLOUR.x, TARGET_COLOUR.y, TARGET_COLOUR.z);
		glPushMatrix();
			assert(gp_world->isAlive(WorldTestUnitAi::ID_TARGET));
			Vector3 target_position = gp_world->getPosition(WorldTestUnitAi::ID_TARGET);
			glTranslated(target_position.x, target_position.y, target_position.z);
			g_position_marker_list.draw();
		glPopMatrix();
	}
	if(g_draw_trails)
		g_target_trail.draw();

	// draw agent extras
	assert(gp_world != NULL);
	if(g_draw_markers && gp_world->isAlive(WorldTestUnitAi::ID_AGENT))
	{
		glColor3d(AGENT_COLOUR.x, AGENT_COLOUR.y, AGENT_COLOUR.z);
		glPushMatrix();
			assert(gp_world->isAlive(WorldTestUnitAi::ID_TARGET));
			Vector3 agent_position = gp_world->getPosition(WorldTestUnitAi::ID_AGENT);
			glTranslated(agent_position.x, agent_position.y, agent_position.z);
			g_position_marker_list.draw();
		glPopMatrix();
	}
	if(g_draw_trails)
		g_agent_trail.draw();

	drawOverlays();

	// send the current image to the screen - any drawing after here will not display
	glutSwapBuffers();
}

void drawAxes (double length)
{
	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(length, 0.0, 0.0);

		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, length, 0.0);

		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, length);
	glEnd();
}

void drawTrail (const Vector3 a_points[],
                unsigned int point_count,
                unsigned int point_next,
                const Vector3& colour)
{
	glBegin(GL_LINE_STRIP);
		for(unsigned int i = 0; i < point_count; i++)
		{
			double fraction = (double)(i) / point_count;
			double colour_factor = sqrt(fraction);
			glColor3d(colour.x * colour_factor,
			          colour.y * colour_factor,
			          colour.z * colour_factor);

			unsigned int point_index = (point_next + i) % point_count;
			const Vector3& point = a_points[point_index];
			glVertex3d(point.x, point.y, point.z);
		}
	glEnd();
}

void drawOverlays ()
{
	SpriteFont::setUp2dView(g_window_width, g_window_height);

	if(g_draw_commands)
		drawCommands();
	if(g_draw_statistics)
		drawStatistics();

	SpriteFont::unsetUp2dView();
}

void drawCommands ()
{
	double x1 = 16;
	double x2 = x1 + 8;
	
	double y1 =  16;
	double y2 = 192;

	g_font.draw("Input:",                          x1, y1 +  0, 0xFF, 0xFF, 0xFF);
	g_font.draw("  [ARROWS] Move camera",          x2, y1 + 16, 0xFF, 0xFF, 0xFF);
	g_font.draw("   [SPACE] Pause / unpause",      x2, y1 + 32, 0xFF, 0xFF, 0xFF);
	g_font.draw("       [+] More ring particles",  x2, y1 + 48, 0xFF, 0xFF, 0xFF);
	g_font.draw("       [-] Fewer ring particles", x2, y1 + 64, 0xFF, 0xFF, 0xFF);
	g_font.draw("     [END] Reset all",            x2, y1 + 80, 0xFF, 0xFF, 0xFF);
	g_font.draw("     [ESC] Exit program",         x2, y1 + 96, 0xFF, 0xFF, 0xFF);

	g_font.draw("Display Options:",            x1, y2 +   0, 0xFF, 0xFF, 0xFF);
	g_font.draw("[1]  Toggle command overlay", x2, y2 +  16, 0xFF, 0xFF, 0xFF);
	g_font.draw("[2]  Toggle global axes",     x2, y2 +  32, 0xFF, 0xFF, 0xFF);
	g_font.draw("[3]  Toggle path orrery",     x2, y2 +  48, 0xFF, 0xFF, 0xFF);
	g_font.draw("[4]  Toggle markers",         x2, y2 +  64, 0xFF, 0xFF, 0xFF);
	g_font.draw("[5]  Toggle trails",          x2, y2 +  80, 0xFF, 0xFF, 0xFF);
	g_font.draw("[6]  Toggle statistics",      x2, y2 +  96, 0xFF, 0xFF, 0xFF);
}

void drawStatistics ()
{
	double x1 = g_window_width - 336;
	double x2 = x1 + 8;
	
	double y1 =  16;

	stringstream ss_current_time;
	float current_time = TimeSystem::getFrameStartTime() - g_reset_time;
	ss_current_time << "    Current time: " << current_time << " s";

	stringstream ss_frame_rate;
	float frame_length = TimeSystem::getFrameDuration();
	if(frame_length > 0.0)
		ss_frame_rate << "     Frames Rate: " << setprecision(3) << (1.0 / frame_length) << " fps";
	else
		ss_frame_rate << "     Frames Rate: Very high";

	stringstream ss_agent_speed1;
	double agent_speed = gp_world->getSpeed(WorldTestUnitAi::ID_AGENT);
	double agent_speed_max = gp_world->getShipSpeedMax(WorldTestUnitAi::ID_AGENT);
	ss_agent_speed1 << "     Agent speed: " << setprecision(3) << agent_speed;
	ss_agent_speed1 << " / " << setprecision(3) << agent_speed_max << " m/s";

	stringstream ss_agent_speed2;
	ss_agent_speed2 << "                    => " << setprecision(3);
	ss_agent_speed2 << (agent_speed / agent_speed_max * 100.0) << "%";

	stringstream ss_target_speed1;
	stringstream ss_target_speed2;
	ss_target_speed1 << "    Target speed: " << setprecision(3);
	if(gp_world->isAlive(WorldTestUnitAi::ID_TARGET))
	{
		double target_speed = gp_world->getSpeed(WorldTestUnitAi::ID_TARGET);
		double target_speed_max = gp_world->getShipSpeedMax(WorldTestUnitAi::ID_TARGET);
		ss_target_speed1 << target_speed << " / " << setprecision(3) << target_speed_max << " m/s";
		ss_target_speed2 << "                    => " << setprecision(3);
		ss_target_speed2 << (target_speed / target_speed_max * 100.0) << "%";
	}
	else
		ss_target_speed1 << "N/A";

	stringstream ss_seperation;
	if(gp_world->isAlive(WorldTestUnitAi::ID_TARGET))
	{
		Vector3 target_position = gp_world->getPosition(WorldTestUnitAi::ID_TARGET);
		Vector3  agent_position = gp_world->getPosition(WorldTestUnitAi::ID_AGENT);
		ss_seperation << "       Sepration: " << setprecision(6);
		ss_seperation << target_position.getDistance(agent_position) << " m";
	}
	else
		ss_seperation << "       Sepration: N/A";

	stringstream ss_agent_ai_time;
	float total_ai_time = gp_world->getAgentAiTimeCumulative();
	ss_agent_ai_time << "   Total AI time: " << setprecision(6) << total_ai_time << " s";

	stringstream ss_agent_ai_per_frame;
	unsigned int current_frame = TimeSystem::getFrameNumber() - g_reset_frame_number;
	ss_agent_ai_per_frame << "   Frame AI time: ";
	if(current_frame > 0)
	{
		ss_agent_ai_per_frame << setprecision(3);
		ss_agent_ai_per_frame << (total_ai_time / current_frame * 1000000.0) << " us";
	}
	else
		ss_agent_ai_per_frame << " N/A";

	stringstream ss_ring_density;
	unsigned int ring_particle_count = gp_world->getRingParticleCount();
	ss_ring_density << "  Particle count: " << ring_particle_count;
	if(ring_particle_count >= RING_PARTICLE_VERY_DENSE)
		ss_ring_density << " (very dense)";
	else if(ring_particle_count >= RING_PARTICLE_DENSE)
		ss_ring_density << " (dense)";
	else if(ring_particle_count >= RING_PARTICLE_NORMAL)
		ss_ring_density << " (average)";
	else if(ring_particle_count >= RING_PARTICLE_SPARSE)
		ss_ring_density << " (sparse)";
	else if(ring_particle_count >= RING_PARTICLE_VERY_SPARSE)
		ss_ring_density << " (very sparse)";
	else if(ring_particle_count >= RING_PARTICLE_FRINGE)
		ss_ring_density << " (fringe)";

	stringstream ss_agent_collisions;
	unsigned int agent_collision_count = gp_world->getAgentCollisionCount();
	ss_agent_collisions << "Agent collisions: " << setw(4) << agent_collision_count;
	ss_agent_collisions << setprecision(3) << " => " << agent_collision_count / current_time << "/s";

	stringstream ss_target_collisions;
	unsigned int target_collision_count = gp_world->getTargetCollisionCount();
	ss_target_collisions << "Agent hit target: " << setw(4) << target_collision_count;
	ss_target_collisions << setprecision(3) << " => " << target_collision_count / current_time << "/s";

	stringstream ss_shots_fired;
	unsigned int shot_fired_count = gp_world->getAgentBulletCount();
	ss_shots_fired << "     Shots fired: " << setw(4) << shot_fired_count;
	ss_shots_fired << setprecision(3) << " => " << shot_fired_count / current_time << "/s";

	stringstream ss_shots_hit1;
	stringstream ss_shots_hit2;
	unsigned int target_shot_count = gp_world->getTargetShotCount();
	ss_shots_hit1 << "Shots hit target: " << setw(4) << target_shot_count;
	ss_shots_hit1 << setprecision(3) << " => " << target_shot_count / current_time << "/s";
	if(shot_fired_count > 0)
	{
		ss_shots_hit2 << "                       => " << setprecision(3);
		ss_shots_hit2 << (target_shot_count * 100.0 / shot_fired_count) << "%";
	}

	g_font.draw("Statistics:",               x1, y1 +   0, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_current_time.str(),       x2, y1 +  16, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_frame_rate.str(),         x2, y1 +  32, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_agent_ai_time.str(),      x2, y1 +  48, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_agent_ai_per_frame.str(), x2, y1 +  64, 0xFF, 0xFF, 0xFF);

	g_font.draw(ss_ring_density.str(),       x2, y1 +  88, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_agent_speed1.str(),       x2, y1 + 104, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_agent_speed2.str(),       x2, y1 + 120, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_target_speed1.str(),      x2, y1 + 136, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_target_speed2.str(),      x2, y1 + 152, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_seperation.str(),         x2, y1 + 168, 0xFF, 0xFF, 0xFF);

	g_font.draw(ss_agent_collisions.str(),   x2, y1 + 192, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_target_collisions.str(),  x2, y1 + 208, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_shots_fired.str(),        x2, y1 + 224, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_shots_hit1.str(),         x2, y1 + 240, 0xFF, 0xFF, 0xFF);
	g_font.draw(ss_shots_hit2.str(),         x2, y1 + 256, 0xFF, 0xFF, 0xFF);
}

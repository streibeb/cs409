//
//  TestUnitAi.h
//

#ifndef TEST_UNIT_AI_H
#define TEST_UNIT_AI_H

class Vector3;
class SimpleMarker;



int main (int argc, char* argv[]);
void initDisplay ();
void loadModels ();
void resetAll ();

void keyboard (unsigned char key, int x, int y);
void keyboardUp (unsigned char key, int x, int y);
unsigned char getFixedKeyCode (unsigned char key);
void special (int special_key, int x, int y);
void specialUp (int special_key, int x, int y);

void update ();
void handleInputRunning ();
void handleInputPaused ();
void handleInputBoth ();

void reshape (int w, int h);
void display ();
void drawAxes (double length);
void drawTrail (const Vector3 a_points[],
                unsigned int point_count,
                unsigned int point_next,
                const Vector3& colour);
void drawOverlays ();
void drawCommands ();
void drawStatistics ();



#endif

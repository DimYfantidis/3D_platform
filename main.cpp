#define _USE_MATH_DEFINES

#include <cmath>
#include <chrono>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>

#include "shapes.h"
#include "logging.h"
#include "typedefs.h"
#include "materials.h"
#include "rendering.h"


// Window's dimension and info.
bool FULLSCREEN = true;
int windowWidth = (FULLSCREEN ? 2560 : 1000);
int windowHeight = (FULLSCREEN ? 1440 : 800);
int windowCenter[2] = { windowWidth / 2, windowHeight / 2 };
int window_menu_id;
float windowMatrix[16];

ScreenLogger logger(windowWidth, windowHeight);

// Camera movement speed.
double move_speed = 20;

// Examplary objects.
engine::rectangle ground(100.0f, 100.0f);
engine::cuboid cuboid_object(8.0f, 8.0f, 8.0f);
engine::sphere sphere_object(10.0f);
engine::sphere ball(0.5f);

engine::light_source lamp(GL_LIGHT0);

double cam_height = 1.7;

volatile vector3d cam_dir = { 0.0, 0.0, -1.0 };
volatile vector3d cam_pos = { 0.0, cam_height, 0.0 };
volatile vector3d torso_dir = { 0.0, 0.0, -1.0 };
volatile vector3d left_dir = { -1.0, 0.0, 0.0 };


#include "callbacks.h"
#include "menu.h"


void init_lists(void) {}

int main(int argc, char* argv[])
{
	// Window Creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("3D Platformer");

	if (FULLSCREEN)
		glutFullScreen();

	// Attributes
	glEnable(GL_DEPTH_TEST);					// Depth Buffer
	glClearColor(0.05f, 0.12f, 0.20f, 1.0f);	// Night Sky Background
	glEnable(GL_LIGHTING);						// Lighting
	glEnable(GL_LIGHT0);						// Light Source
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_NORMALIZE);				// Normals Preservation for units
	//glEnable(GL_COLOR_MATERIAL);			// Make glColorf() as Material

	glLineWidth(0.1f);
	glPointSize(0.3f);

	glColor3f(1.0f, 1.0f, 1.0f);
	// Pre-compiled lists initialization
	init_lists();

	// Window menu
	create_window_menu();

	// ----------- Window Matrix (BEGIN) ----------- //
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Initialization of Window Matrix for on-screen string rendering.
	glPushMatrix();
	{
		gluOrtho2D(0.0, (double)windowWidth, 0.0, (double)windowHeight);
		glGetFloatv(GL_PROJECTION_MATRIX, windowMatrix);
	}
	glPopMatrix();
	// ----------- Window Matrix (BEGIN) ----------- //

	// Returns mouse to the center of the window.
	glutWarpPointer(windowCenter[0], windowCenter[1]);
	// Hides cursor.
	glutSetCursor(GLUT_CURSOR_NONE);

	glutIgnoreKeyRepeat(1);

	// ----------- CALLBACK FUNCTIONS (BEGIN) ----------- //
	glutSpecialFunc(NULL);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(passiveMotion);
	// ----------- CALLBACK FUNCTIONS (END) ----------- //

	glutMainLoop();

	return EXIT_SUCCESS;
}

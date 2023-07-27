#define _USE_MATH_DEFINES

#include <cmath>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>

#include "shapes.h"
#include "logging.h"
#include "typedefs.h"


int windowWidth = 800;
int windowHeight = 800;
int windowCenter[2] = { windowWidth / 2, windowHeight / 2 };
int window_menu_id;

double move_speed = 2.0;

engine::rectangle ground(100.0f, 100.0f);
engine::cuboid cuboid_object(8.0f, 8.0f, 8.0f);
engine::sphere sphere_object(10.0f);
engine::light_source sun(GL_LIGHT0);

vector3d cam_dir = { 0.0f, 0.0f, -1.0f };
vector3d cam_pos = { 0.0f, 0.0f, 0.0f };

#include "callbacks.h"
#include "menu.h"


int main(int argc, char* argv[])
{
	// Window Creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Platformer");

	// Attributes
	glEnable(GL_DEPTH_TEST);				// Depth Buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Black Background
	glEnable(GL_LIGHTING);					// Lighting
	glEnable(GL_LIGHT0);					// Light Source
	//glEnable(GL_NORMALIZE);				// Normals Preservation for units
	//glEnable(GL_COLOR_MATERIAL);			// Make glColorf() as Material

	// Pre-compiled lists initialization
	//init_lists();

	// Window menu
	create_window_menu();

	// ----------- CALLBACK FUNCTIONS (BEGIN) ----------- //

	std::cout << sizeof(engine::shape) << std::endl;

	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	glutSpecialFunc(NULL);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(passiveMotion);

	// ----------- CALLBACK FUNCTIONS (END) ----------- //

	glutMainLoop();

	return EXIT_SUCCESS;
}

#define _USE_MATH_DEFINES

#include <cmath>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>

#include "Cuboid.h"
#include "logging.h"
#include "typedefs.h"


Cuboid ground(100.0f, 0.1f, 100.0f);

int windowWidth = 800;
int windowHeight = 600;
int windowCenter[2] = { windowWidth / 2, windowHeight / 2 };

vector3d cam_dir = { 0.0f, 0.0f, -1.0f };
vector3d cam_pos = { 0.0f, 0.0f, 0.0f };


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-60.0, 60.0, -60.0, 60.0, -300.0, 300.0);

	gluLookAt(
		cam_pos[0], cam_pos[1], cam_pos[2], 
		cam_dir[0], cam_dir[1], cam_dir[2], 
		0.0, 1.0, 0.0
	);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ground.color(0.0f, 1.0f, 0.0f).spawnAt(0.0f, 0.0f, 0.0f);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (tolower(key)) {
	case 'w':
		logMessage("Forward movement\n");
		break;
	case 'a':
		logMessage("Left movement\n");
		break;
	case 's':
		logMessage("Backwards movement\n");
		break;
	case 'd':
		logMessage("Right movement\n");
		break;
	}
}

void passiveMotion(int x, int y)
{
	constexpr static double step = 0.001745;
	static double horizontal_angle = M_PI_2;
	static double vertical_angle = -M_PI_2;

	int x_prev = windowCenter[0];
	int y_prev = windowCenter[1];

	int dx = x - x_prev;
	int dy = y - y_prev;

	horizontal_angle += -dx * step;
	vertical_angle += dy * step;
	
	if (horizontal_angle < -M_PI) {
		horizontal_angle += 2 * M_PI;
	}
	else if (horizontal_angle > M_PI) {
		horizontal_angle -= 2 * M_PI;
	}
	if (vertical_angle < -M_PI) {
		vertical_angle += 2 * M_PI;
	}
	else if (vertical_angle > M_PI) {
		vertical_angle -= 2 * M_PI;
	}
	cam_dir[0] = cos(vertical_angle) * sin(horizontal_angle);
	cam_dir[1] = sin(vertical_angle);
	cam_dir[2] = cos(vertical_angle) * cos(horizontal_angle);

	glutWarpPointer(windowCenter[0], windowCenter[1]);

	glutPostRedisplay();
}

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
	//glEnable(GL_LIGHTING);					// Lighting
	//glEnable(GL_LIGHT0);					// Light Source
	//glEnable(GL_NORMALIZE);				// Normals Preservation for units
	//glEnable(GL_COLOR_MATERIAL);			// Make glColorf() as Material

	// Pre-compiled lists initialization
	//init_lists();

	// Window menu (4 options)
	//create_menu();

	// ----------- CALLBACK FUNCTIONS (BEGIN) ----------- //

	glutWarpPointer(windowWidth / 2, windowHeight / 2);

	glutSpecialFunc(NULL);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(passiveMotion);

	// ----------- CALLBACK FUNCTIONS (END) ----------- //


	glutMainLoop();

	return EXIT_SUCCESS;
}

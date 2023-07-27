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

double move_speed = 2.0;

engine::rectangle ground(100.0f, 100.0f);
engine::cuboid cuboid_object(8.0f, 8.0f, 8.0f);
engine::sphere sphere_object(10.0f);

vector3d cam_dir = { 0.0f, 0.0f, -1.0f };
vector3d cam_pos = { 0.0f, 0.0f, 0.0f };


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-60.0, 60.0, -60.0, 60.0, -300.0, 300.0);

	gluLookAt(
		0.0, 0.0, 0.0, 
		cam_dir[0], cam_dir[1], cam_dir[2],
		0.0, 1.0, 0.0
	);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ground
		.color(0.0f, 1.0f, 0.0f)
		.spawn(0.0f, 0.0f, 0.0f);
	cuboid_object
		.color(0.0f, 0.0f, 1.0f)
		.spawn(20.0f, 20.0f, 20.0f)
		.spawn(-20.0f, 20.0f, -20.0f);
	sphere_object
		.color(1.0f, 1.0f, 0.0f)
		.spawn(-20.0f, 30.0f, 0.0f);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (tolower(key)) {
	case 'w':
		cam_pos[0] += cam_dir[0] * move_speed;
		cam_pos[1] += cam_dir[1] * move_speed;
		cam_pos[2] += cam_dir[2] * move_speed;
		logMessage("Forward movement\n");
		break;
	case 'a':
		logMessage("Left movement\n");
		break;
	case 's':
		cam_pos[0] -= cam_dir[0] * move_speed;
		cam_pos[1] -= cam_dir[1] * move_speed;
		cam_pos[2] -= cam_dir[2] * move_speed;
		logMessage("Backwards movement\n");
		break;
	case 'd':
		logMessage("Right movement\n");
		break;
	}
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

void passiveMotion(int x, int y)
{
	constexpr static double step = 0.001745;
	static double horizontal_angle = M_PI_2;
	static double vertical_angle = -M_PI_2;

	int x_prev = windowCenter[0];
	int y_prev = windowCenter[1];

	int dx = x_prev - x;
	int dy = y_prev - y;

	horizontal_angle += dx * step;
	
	if (horizontal_angle < -M_PI) {
		horizontal_angle += 2 * M_PI;
	}
	else if (horizontal_angle > M_PI) {
		horizontal_angle -= 2 * M_PI;
	}
	if (sgn(dy) != sgn(vertical_angle) || abs(vertical_angle) < M_PI_2) {
		vertical_angle += dy * step;
	}
	if (abs(vertical_angle) > M_PI_2) {
		vertical_angle = sgn(vertical_angle) * M_PI_2;
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

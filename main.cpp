#define _USE_MATH_DEFINES

#include <cmath>
#include <chrono>
#include <cctype>
#include <vector>
#include <thread>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <GL/glut.h>
#include <Windows.h>
#include <mmsystem.h>

#include "Shape.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "Cuboid.h"
#include "LightSource.h"
#include "ScreenLogger.h"
#include "typedefs.h"
#include "materials.h"
#include "rendering.h"
#include "display_lists.h"
#include "globals.h"
#include "ControllerInterface.h"
#include "FileManager.h"
#include "menu.h"


int main(int argc, char* argv[])
{
	// Window Creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("3D Platformer");

	ScreenLogger::getInstance().setDimensions(windowWidth, windowHeight);

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
	initDisplayLists();

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
	// Ignores prolonged push of a key.
	glutIgnoreKeyRepeat(TRUE);

	ControllerInterface::initializeWorld();

	// ----------- CALLBACK FUNCTIONS (BEGIN) ----------- //
	glutDisplayFunc(ControllerInterface::Callback::display);
	glutKeyboardFunc(ControllerInterface::Callback::keyboardDown);
	glutKeyboardUpFunc(ControllerInterface::Callback::keyboardUp);
	glutPassiveMotionFunc(ControllerInterface::Callback::passiveMotion);
	glutSpecialFunc(nullptr);
	glutIdleFunc(nullptr);
	// ----------- CALLBACK FUNCTIONS (END) ----------- //

	
	PlaySound(
		FileManager::getInstance().mainAmbienceTheme().data(), 
		nullptr, 
		SND_ASYNC | SND_NODEFAULT | SND_LOOP
	);
	glutMainLoop();

	return EXIT_SUCCESS;
}

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
#include "logging.h"
#include "typedefs.h"
#include "materials.h"
#include "rendering.h"
#include "display_lists.h"
#include "globals.h"
#include "ControllerInterface.h"


//#include "callbacks.h"
#include "menu.h"


void initDirectories(void)
{
	WCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");

	workingDir = std::wstring(buffer).substr(0, pos);
	mainThemeFile = workingDir + L"\\Music\\lake_wind_ambience.wav";
}

int main(int argc, char* argv[])
{
	// Window Creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("3D Platformer");

	ScreenLogger::getInstance().setDimensions(windowWidth, windowHeight);

	initDirectories();
	
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

	// ----------- CALLBACK FUNCTIONS (BEGIN) ----------- //
	glutSpecialFunc(nullptr);
	glutDisplayFunc(ControllerInterface::display);
	glutIdleFunc(nullptr);
	glutKeyboardFunc(ControllerInterface::keyboardDown);
	glutKeyboardUpFunc(ControllerInterface::keyboardUp);
	glutPassiveMotionFunc(ControllerInterface::passiveMotion);
	// ----------- CALLBACK FUNCTIONS (END) ----------- //

	PlaySound(mainThemeFile.data(), nullptr, SND_ASYNC | SND_NODEFAULT | SND_LOOP);
	glutMainLoop();

	return EXIT_SUCCESS;
}

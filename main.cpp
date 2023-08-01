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


// Files and Directories
std::wstring workingDir;
std::wstring mainThemeFile;


// Window's dimension and info.
bool FULLSCREEN = true;
int windowWidth = (FULLSCREEN ? 2560 : 1000);
int windowHeight = (FULLSCREEN ? 1440 : 800);
int windowCenter[2] = { windowWidth / 2, windowHeight / 2 };
int window_menu_id;
float windowMatrix[16];

bool ambientMusicPlaying;

ScreenLogger logger(windowWidth, windowHeight);

// Camera movement speed.
double move_speed = 20;

// Examplary objects.
engine::Rectangle ground(100.0f, 100.0f);
engine::Cuboid cuboid_object(8.0f, 8.0f, 8.0f);
engine::Sphere sphere_object(10.0f);
engine::Sphere ball(0.5f);

engine::LightSource lamp(GL_LIGHT0);

double cam_height = 1.7;

volatile vector3d cam_dir = { 0.0, 0.0, -1.0 };
volatile vector3d cam_pos = { 0.0, cam_height, 0.0 };
volatile vector3d torso_dir = { 0.0, 0.0, -1.0 };
volatile vector3d left_dir = { -1.0, 0.0, 0.0 };

#include "callbacks.h"
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
	glutSpecialFunc(NULL);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(passiveMotion);
	// ----------- CALLBACK FUNCTIONS (END) ----------- //

	PlaySound(mainThemeFile.data(), NULL, SND_ASYNC | SND_NODEFAULT | SND_LOOP);
	glutMainLoop();

	return EXIT_SUCCESS;
}

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <GL/glut.h>

#include "Cuboid.h"
#include "logging.h"


Cuboid ground(100.0f, 0.1f, 100.0f);


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ground
		.color(0.0f, 1.0f, 0.0f)
		.spawnAt(0.0f, 0.0f, 0.0f);

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

int main(int argc, char* argv[])
{
	// Window Creation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
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

	// Window menu (4 options)
	//create_menu();

	// ----------- CALLBACK FUNCTIONS (BEGIN) ----------- //

	glutSpecialFunc(NULL);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutKeyboardFunc(keyboard);

	// ----------- CALLBACK FUNCTIONS (END) ----------- //


	glutMainLoop();

	return EXIT_SUCCESS;
}
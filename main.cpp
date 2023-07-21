#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <GL/freeglut.h>


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'w':
		printf("Forward Movement");
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
	glutKeyboardFunc(NULL);

	// ----------- CALLBACK FUNCTIONS (END) ----------- //


	glutMainLoop();

	return EXIT_SUCCESS;
}
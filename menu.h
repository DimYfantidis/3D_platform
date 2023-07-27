#pragma once


void window_menu(int op_id)
{
	exit(EXIT_SUCCESS);
}

void create_window_menu(void)
{
	window_menu_id = glutCreateMenu(window_menu);
	{
		glutAddMenuEntry("Exit", 1);
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
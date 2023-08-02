#pragma once


void window_menu(int op_id)
{
	switch (op_id)
	{
	case 1:
		if (sound_on)
			PlaySound(nullptr, nullptr, 0);
		else
			PlaySound(mainThemeFile.data(), nullptr, SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		sound_on = !sound_on;
		break;
	case 2:
		exit(EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

void create_window_menu(void)
{
	window_menu_id = glutCreateMenu(window_menu);
	{
		glutAddMenuEntry("Toggle Sound", 1);
		glutAddMenuEntry("Exit", 2);
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
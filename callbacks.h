#pragma once

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

	cuboid_object
		.materialv(GL_AMBIENT, { 0.3f, 0.3f, 0.3f, 1.0f })
		.materialv(GL_SPECULAR, { 1.0f, 1.0f, 1.0f, 1.0f })
		.materialv(GL_DIFFUSE, { 0.3f, 0.3f, 0.3f, 1.0f })
		.spawn(20.0f, 20.0f, 20.0f)
		.spawn(-20.0f, 20.0f, -20.0f);
	ground
		.materialv(GL_AMBIENT, { 0.3f, 1.0f, 0.3f, 0.0f })
		.materialv(GL_SPECULAR, { 0.0f, 0.0f, 0.0f, 0.0f })
		.materialv(GL_DIFFUSE, { 0.3f, 1.0f, 0.3f, 0.3f })
		.spawn(0.0f, 0.0f, 0.0f);

	sphere_object
		.spawn(-20.0f, 30.0f, 0.0f);
	
	sun.emission({ 0.2f, 0.2f, 0.0f, 1.0f }).spawn(10.0f, 10.0f, 10.0f);

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

void passiveMotion(int x, int y)
{
	using engine::sgn;

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

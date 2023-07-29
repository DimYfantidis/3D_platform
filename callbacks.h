#pragma once


void display(void)
{
	static std::chrono::steady_clock::time_point start;
	static std::chrono::steady_clock::time_point stop;

	if constexpr (LOG_FPS)
		start = std::chrono::high_resolution_clock::now();

	static const double aspect_ratio = (double)(windowWidth / windowHeight);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// gluPerspective(90.0, aspect_ratio, 0.1, 100.0);
	// glOrtho(-60.0, 60.0, -60.0, 60.0, -300.0, 300.0);

	gluLookAt(
		cam_pos[0], cam_pos[1], cam_pos[2],
		cam_pos[0] + cam_dir[0], cam_pos[1] + cam_dir[1], cam_pos[2] + cam_dir[2],
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
		.materialv(GL_AMBIENT, { 0.3f, 0.3f, 0.3f, 1.0f })
		.materialv(GL_SPECULAR, { 1.0f, 1.0f, 1.0f, 1.0f })
		.materialv(GL_DIFFUSE, { 0.3f, 0.3f, 0.3f, 1.0f })
		.spawn(-20.0f, 30.0f, 0.0f);

	mini_sphere
		.materialv(GL_AMBIENT, { 0.3f, 0.3f, 0.3f, 1.0f })
		.materialv(GL_SPECULAR, { 1.0f, 1.0f, 1.0f, 1.0f })
		.materialv(GL_DIFFUSE, { 0.3f, 0.3f, 0.3f, 1.0f })
		.spawn(1.0f, 1.0f, 1.0f)
		.spawn(3.0f, 1.0f, 7.0f)
		.spawn(8.0f, 1.0f, -6.0f)
		.spawn(10.0f, 1.0f, -12.0f)
		.spawn(9.0f, 1.0f, 15.0f);
	
	lamp
		.emission({ 0.2f, 0.2f, 0.0f, 1.0f })
		.materialv(GL_DIFFUSE, { 1.0f, 1.0f, 0.0f, 1.0f })
		.lightv(GL_SPECULAR, { 0.8f, 0.8f, 0.8f, 1.0f })
		.lightv(GL_DIFFUSE, { 0.8f, 0.8f, 0.8f, 1.0f })
		.spawn(10.0f, 10.0f, 10.0f);

	glutSwapBuffers();

	if constexpr (LOG_FPS)
	{
		stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time = stop - start;
		logMessage("FPS: %.3lf\n", 1.0 / time.count());
	}
}

void keyboard(unsigned char key, int x, int y)
{
	// Escape key
	if (key == 27) {
		exit(EXIT_SUCCESS);
	}

	// Camera movement (currently not working)
	switch (tolower(key)) {
	case 'w':
		cam_pos[0] += torso_dir[0] * move_speed;
		cam_pos[2] += torso_dir[2] * move_speed;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logMessage("Forward movement\n");
		break;
	case 'a':
		cam_pos[0] += left_dir[0] * move_speed;
		cam_pos[2] += left_dir[2] * move_speed;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logMessage("Left movement\n");
		break;
	case 's':
		cam_pos[0] -= torso_dir[0] * move_speed;
		cam_pos[2] -= torso_dir[2] * move_speed;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logMessage("Backwards movement\n");
		break;
	case 'd':
		cam_pos[0] -= left_dir[0] * move_speed;
		cam_pos[2] -= left_dir[2] * move_speed;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logMessage("Right movement\n");
		break;
	}
}

// Camera rotation
void passiveMotion(int x, int y)
{
	using engine::sgn;

	constexpr static double step = 0.001745;
	static double horizontal_angle = -M_PI_2;
	static double vertical_angle = 0;

	int x_prev = windowCenter[0];
	int y_prev = windowCenter[1];

	// Camera's rotation speed is proportionate to mouse speed.
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
	double sin_vert = sin(vertical_angle);
	double sin_horz = sin(horizontal_angle);
	double cos_vert = cos(vertical_angle);
	double cos_horz = cos(horizontal_angle);

	cam_dir[0] = cos_vert * sin_horz;
	cam_dir[1] = sin_vert;
	cam_dir[2] = cos_vert * cos_horz;

	torso_dir[0] = sin_horz;
	torso_dir[2] = cos_horz;

	left_dir[0] = cos_horz;
	left_dir[2] = -sin_horz;

	if constexpr (LOG_TORSO_ORIENTATION)
		logMessage(
			"Torso: (%.3lf, %.3lf, %.3lf)\t Left: (%.3lf, %.3lf, %.3lf)\n",
			torso_dir[0], torso_dir[1], torso_dir[2],
			left_dir[0], left_dir[1], left_dir[2]
		);

	if constexpr (LOG_CAMERA_ROTATATION)
		logMessage(
			"Now looking towards: (%.3lf, %.3lf, %.3lf)\n",
			cam_dir[0], cam_dir[1], cam_dir[2]
		);

	// Returns mouse to the center of the window.
	glutWarpPointer(windowCenter[0], windowCenter[1]);

	glutPostRedisplay();
}

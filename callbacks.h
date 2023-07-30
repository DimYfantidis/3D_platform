#pragma once


static volatile bool keystates[UCHAR_MAX + 1] = { false };
static double deltaTime;

inline void move(void)
{
	if constexpr (LOG_CAMERA_MOVEMENT)
		logger.logMessage(
			"Camera Position: (%.3lf, %.3lf, %.3lf)",
			cam_pos[0], cam_pos[1], cam_pos[2]
		);
	if (keystates['w'] || keystates['W']) 
	{
		cam_pos[0] += torso_dir[0] * move_speed * deltaTime;
		cam_pos[2] += torso_dir[2] * move_speed * deltaTime;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logger.logMessage("Forward movement");
	}
	if (keystates['a'] || keystates['A'])
	{
		cam_pos[0] += left_dir[0] * move_speed * deltaTime;
		cam_pos[2] += left_dir[2] * move_speed * deltaTime;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logger.logMessage("Left movement");
	}
	if (keystates['s'] || keystates['S'])
	{
		cam_pos[0] -= torso_dir[0] * move_speed * deltaTime;
		cam_pos[2] -= torso_dir[2] * move_speed * deltaTime;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logger.logMessage("Backwards movement");
	}
	if (keystates['d'] || keystates['D'])
	{
		cam_pos[0] -= left_dir[0] * move_speed * deltaTime;
		cam_pos[2] -= left_dir[2] * move_speed * deltaTime;
		if constexpr (LOG_CAMERA_MOVEMENT)
			logger.logMessage("Right movement");
	}
}

void display(void)
{
	static auto beginTime = std::chrono::high_resolution_clock::now();
	static auto currentTime = std::chrono::high_resolution_clock::now();
	static auto oldTime = std::chrono::high_resolution_clock::now();
	static std::chrono::duration<double> totalProgramRuntime;
	static std::chrono::duration<double> dt;
	static const double aspect_ratio = (double)(windowWidth / windowHeight);


	currentTime = std::chrono::high_resolution_clock::now();
	dt = currentTime - oldTime;
	deltaTime = dt.count();

	// Escape Button
	if (keystates[27]) {
		exit(EXIT_SUCCESS);
	}

	if constexpr (LOG_ELAPSED_FRAME_TIME)
		totalProgramRuntime = currentTime - beginTime;

	if constexpr (LOG_ELAPSED_FRAME_TIME)
		logger.logMessage("Elapsed Time: %.3lfs", totalProgramRuntime.count());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, aspect_ratio, 0.1, 100.0);
	// glOrtho(-60.0, 60.0, -60.0, 60.0, -300.0, 300.0);

	move();

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

	ball
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

	if constexpr (LOG_FPS)
		logger.logFPS(1.0 / deltaTime);

	logger.flushLogBuffer();
	oldTime = currentTime;
	glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y)
{
	keystates[key] = true;
	glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y)
{
	keystates[key] = false;
	glutPostRedisplay();
}

// Camera rotation
void passiveMotion(int x, int y)
{
	using engine::sgn;

	constexpr static double step = 0.1745;
	static double horizontal_angle = -M_PI_2;
	static double vertical_angle = 0;

	int x_prev = windowCenter[0];
	int y_prev = windowCenter[1];

	// Camera's rotation speed is proportionate to mouse speed.
	int dx = x_prev - x;
	int dy = y_prev - y;

	horizontal_angle += dx * step * deltaTime;

	if (horizontal_angle < -M_PI) {
		horizontal_angle += 2 * M_PI;
	}
	else if (horizontal_angle > M_PI) {
		horizontal_angle -= 2 * M_PI;
	}
	if (sgn(dy) != sgn(vertical_angle) || abs(vertical_angle) < M_PI_2) {
		vertical_angle += dy * step * deltaTime;
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
	{
		logger.logMessage("Body Orientation:");
		logger.logMessage(
			"> Torso: (%.3lf, %.3lf, %.3lf)",
			torso_dir[0], torso_dir[1], torso_dir[2]
		);
		logger.logMessage(
			"> Left:  (%.3lf, %.3lf, %.3lf)",
			left_dir[0], left_dir[1], left_dir[2]
		);
	}

	if constexpr (LOG_CAMERA_ROTATATION)
		logger.logMessage(
			"Camera Orientation: (%.3lf, %.3lf, %.3lf)",
			cam_dir[0], cam_dir[1], cam_dir[2]
		);

	// Returns mouse to the center of the window.
	glutWarpPointer(windowCenter[0], windowCenter[1]);

	glutPostRedisplay();
}

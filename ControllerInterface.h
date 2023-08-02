#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <chrono>
#include <climits>
#include <GL/glut.h>

#include "Shape.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "Cuboid.h"
#include "LightSource.h"
#include "logging.h"
#include "materials.h"
#include "globals.h"


// Examplary objects.
engine::Rectangle ground(100.0f, 100.0f);
engine::Cuboid cuboid_object(8.0f, 8.0f, 8.0f);
engine::Sphere sphere_object(10.0f);
engine::Sphere ball(0.5f);

engine::LightSource lamp(GL_LIGHT0);

class ControllerInterface
{
private:
	inline static ScreenLogger& logger = ScreenLogger::getInstance();

	inline static bool keystates[UCHAR_MAX + 1] = { false };

	inline static double deltaTime;

	inline static double move_speed = 20.0;
	inline static double cam_height = 1.7;

	inline static volatile vector3d cam_dir = { 0.0, 0.0, -1.0 };
	inline static volatile vector3d cam_pos = { 0.0, cam_height, 0.0 };
	inline static volatile vector3d torso_dir = { 0.0, 0.0, -1.0 };
	inline static volatile vector3d left_dir = { -1.0, 0.0, 0.0 };

public:
	ControllerInterface() = delete;

	static void move(void)
	{
		double delta_move = move_speed * deltaTime;

		if constexpr (LOG_CAMERA_MOVEMENT)
			logger.logMessage(
				"Camera Position: (%.3lf, %.3lf, %.3lf)",
				cam_pos[0], cam_pos[1], cam_pos[2]
			);
		if (keystates['w'])
		{
			cam_pos[0] += torso_dir[0] * delta_move;
			cam_pos[2] += torso_dir[2] * delta_move;
			if constexpr (LOG_CAMERA_MOVEMENT)
				logger.logMessage("Forward movement");
		}
		if (keystates['a'])
		{
			cam_pos[0] += left_dir[0] * delta_move;
			cam_pos[2] += left_dir[2] * delta_move;
			if constexpr (LOG_CAMERA_MOVEMENT)
				logger.logMessage("Left movement");
		}
		if (keystates['s'])
		{
			cam_pos[0] -= torso_dir[0] * delta_move;
			cam_pos[2] -= torso_dir[2] * delta_move;
			if constexpr (LOG_CAMERA_MOVEMENT)
				logger.logMessage("Backwards movement");
		}
		if (keystates['d'])
		{
			cam_pos[0] -= left_dir[0] * delta_move;
			cam_pos[2] -= left_dir[2] * delta_move;
			if constexpr (LOG_CAMERA_MOVEMENT)
				logger.logMessage("Right movement");
		}
		if (keystates[' '])
		{
			cam_pos[1] += delta_move;
		}
		if (keystates['x'])
		{
			cam_pos[1] -= delta_move;
		}
	}

	static void menuDisplay(void)
	{

	}

	static void check_menu(void)
	{
		if (keystates[27])
		{
			glutDisplayFunc(menuDisplay);
			glutKeyboardFunc(nullptr);
			glutKeyboardUpFunc(nullptr);
			glutPassiveMotionFunc(nullptr);
		}
	}

	static void display(void)
	{
		constexpr float light_radius = 15.0f;
		static vector3f light_pos = { light_radius, 10.0f, 0.0f };
		static float light_angle = 0.0f;
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
		// check_menu();

		light_angle = light_angle + 2.0f * (float)deltaTime;

		if (light_angle > 2.0 * M_PI)
			light_angle = 0.0f;

		light_pos[0] = light_radius * cos(light_angle);
		light_pos[2] = light_radius * sin(light_angle);

		if constexpr (LOG_ELAPSED_FRAME_TIME)
			totalProgramRuntime = currentTime - beginTime;

		if constexpr (LOG_ELAPSED_FRAME_TIME)
			logger.logMessage("Elapsed Time: %.3lfs", totalProgramRuntime.count());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.0, aspect_ratio, 0.1, 150.0);
		// glOrtho(-60.0, 60.0, -60.0, 60.0, -300.0, 300.0);

		move();

		gluLookAt(
			cam_pos[0], cam_pos[1], cam_pos[2],
			cam_pos[0] + cam_dir[0], cam_pos[1] + cam_dir[1], cam_pos[2] + cam_dir[2],
			0.0, 1.0, 0.0
		);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		ground
			.materialv(GL_AMBIENT, materials::ground_amb)
			.materialv(GL_SPECULAR, materials::ground_spec)
			.materialv(GL_DIFFUSE, materials::ground_diff)
			.resolution(20)
			.spawn(0.0f, 0.0f, 0.0f);

		cuboid_object
			.resolution(10)
			.material(materials::jade)
			.spawn(20.0f, 20.0f, 20.0f)
			.material(materials::gold)
			.spawn(-20.0f, 20.0f, -20.0f);


		sphere_object
			.resolution(6)
			.material(materials::pearl)
			.spawn(-20.0f, 30.0f, 0.0f);


		ball
			.material(materials::pearl)
			.spawn(1.0f, 1.0f, 1.0f)
			.spawn(3.0f, 1.0f, 7.0f)
			.spawn(8.0f, 1.0f, -6.0f)
			.spawn(10.0f, 1.0f, -12.0f)
			.spawn(9.0f, 1.0f, 15.0f);

		lamp
			.emission(materials::lamp_emsn)
			.materialv(GL_DIFFUSE, materials::lamp_diff)
			.lightv(GL_SPECULAR, materials::light_spec)
			.lightv(GL_DIFFUSE, materials::light_diff)
			.spawn(light_pos);

		if constexpr (LOG_FPS)
			logger.logFPS(1.0 / deltaTime);

		if constexpr (LOG_SOUNDS)
			logger.logMessage((sound_on ? "Playing Music: lake_wind_ambience.wav" : "Music Disabled"));

		logger.flushLogBuffer();
		oldTime = currentTime;
		glutSwapBuffers();
	}

	static void keyboardDown(unsigned char key, int x, int y)
	{
		if (isalpha(key))
			key = tolower(key);
		keystates[key] = true;
		glutPostRedisplay();
	}

	static void keyboardUp(unsigned char key, int x, int y)
	{
		if (isalpha(key))
			key = tolower(key);
		keystates[key] = false;
		glutPostRedisplay();
	}

	// Camera rotation
	static void passiveMotion(int x, int y)
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

		double delta_step = step * deltaTime;

		horizontal_angle += dx * delta_step;

		if (horizontal_angle < -M_PI) {
			horizontal_angle += 2 * M_PI;
		}
		else if (horizontal_angle > M_PI) {
			horizontal_angle -= 2 * M_PI;
		}
		if (sgn(dy) != sgn(vertical_angle) || abs(vertical_angle) < M_PI_2 - 0.2 * step) {
			vertical_angle += dy * delta_step;
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
};

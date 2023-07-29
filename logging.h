#pragma once


#include <array>
#include <cstdarg>
#include <iostream>

#include "rendering.h"


constexpr bool LOG_SHAPES = false;
constexpr bool LOG_CAMERA_ROTATATION = false;
constexpr bool LOG_TORSO_ORIENTATION = false;
constexpr bool LOG_CAMERA_MOVEMENT = false;
constexpr bool LOG_FPS = true;


inline void logMessage(const char* format...)
{
	static char LOG_BUFFER[BUFSIZ];
	static vector3f text_color = { 1.0f, 1.0f, 1.0f };

	va_list args;
	va_start(args, format);

	std::vsnprintf(LOG_BUFFER, BUFSIZ, format, args);

	va_end(args);
	
	engine::renderString(0.0f, 0.0f, GLUT_BITMAP_9_BY_15, LOG_BUFFER, text_color);
}

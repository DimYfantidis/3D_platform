#pragma once


#include <array>
#include <string>
#include <vector>
#include <cstdarg>
#include <iostream>

#include "rendering.h"


constexpr bool LOG_SHAPES = false;
constexpr bool LOG_CAMERA_ROTATATION = false;
constexpr bool LOG_TORSO_ORIENTATION = false;
constexpr bool LOG_CAMERA_MOVEMENT = false;
constexpr bool LOG_FPS = true;


class ScreenLogger
{
public:
	ScreenLogger(int scrWidth, int scrHeight)
		: m_width(scrWidth), m_height(scrHeight),
		m_info_buffer(), m_warning_buffer(), m_line(), m_fps()
	{}

	void logMessage(const char* format...)
	{
		va_list args;
		va_start(args, format);

		std::vsnprintf(m_line, BUFSIZ, format, args);

		va_end(args);

		m_info_buffer.push_back(m_line);
	}

	void logWarning(const char* format...)
	{
		va_list args;
		va_start(args, format);

		std::vsnprintf(m_line, BUFSIZ, format, args);

		va_end(args);

		m_warning_buffer.push_back(m_line);
	}

	void logFPS(double fps) {
		std::snprintf(m_fps, 30, "FPS: %.3lf", fps);
	}

	void flushLogBuffer(void)
	{
		int i = 0;
		int j = 0;

		static vector3f info_color = { 1.0f, 1.0f, 1.0f };
		static vector3f warning_color = { 1.0f, 0.0f, 0.0f };
		
		engine::renderString(10.0f, (float)m_height - 20.0f,
			GLUT_BITMAP_9_BY_15, m_fps, info_color
		);
		i++;

		for (const std::string& info : m_info_buffer) 
		{
			engine::renderString(
				10.0f, (float)m_height - 20.0f - i * 17.0f, 
				GLUT_BITMAP_9_BY_15, info.c_str(), info_color
			);
			i++;
		}
		for (const std::string& warning : m_warning_buffer)
		{
			engine::renderString(
				10.0f, 20.0f + j * 17.0f,
				GLUT_BITMAP_9_BY_15, warning.c_str(), warning_color
			);
			j++;
		}
		m_info_buffer.clear();
		m_warning_buffer.clear();
	}

private:
	int m_width;
	int m_height;

	std::vector<std::string> m_info_buffer;
	std::vector<std::string> m_warning_buffer;

	char m_line[BUFSIZ];
	char m_fps[30];
};
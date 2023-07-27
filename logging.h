#pragma once

#include <iostream>
#include <cstdarg>


constexpr bool LOGGING_ENABLED = true;

constexpr bool LOG_SHAPES = false;

constexpr bool LOG_CAMERA_ROTATATION = true;


inline void logMessage(const char* format...)
{
	if constexpr (LOGGING_ENABLED)
	{
		static char LOG_BUFFER[BUFSIZ];
		static std::ostream& logger = std::cout;

		va_list args;
		va_start(args, format);

		std::vsnprintf(LOG_BUFFER, BUFSIZ, format, args);

		va_end(args);

		logger << LOG_BUFFER;
	}
}

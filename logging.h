#pragma once

#include <iostream>
#include <cstdarg>


constexpr bool LOGGING_ENABLED = false;

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

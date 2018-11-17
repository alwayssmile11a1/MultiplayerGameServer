#pragma once
#include <string>
#include <debugapi.h>
#include <stdarg.h>

namespace StringUtils
{
	std::string GetCommandLineArg(int inIndex);

	std::string Sprintf(const char* inFormat, ...);

	void	Log(const char* inFormat);
	void	Log(const char* inFormat, ...);
}

#define LOG( ... ) StringUtils::Log( __VA_ARGS__ );
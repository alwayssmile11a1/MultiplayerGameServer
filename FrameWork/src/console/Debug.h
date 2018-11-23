#ifndef __GUICON_H__
#define __GUICON_H__

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

class Debug
{
public:
	static void RedirectIOToConsole();
	static void Log(const char* message, ...);
};



#endif

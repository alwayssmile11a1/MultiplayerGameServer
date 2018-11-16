#include "Console.h"



Console::Console()
{
}


Console::~Console()
{
}

void Console::Log(const std::string &message)
{
	//get path from string
	std::wstring stemp = StringToWstring(message);
	LPCWSTR mes = stemp.c_str();

	OutputDebugString(mes);
}
#include "Time.h"

float Time::GetTime()
{
	float time = GetTickCount() / 1000.0;
	return time;
}
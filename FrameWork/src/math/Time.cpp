#include "Time.h"

float Time::GetTimeF()
{
	float time = GetTickCount() / 1000.0;
	return time;
}
#include "Time.h"

float Time::start;
void Time::Init()
{
	start = clock();
}

float Time::GetTimeFSinceGameStart()
{
	return (clock() - start)/CLOCKS_PER_SEC;
}
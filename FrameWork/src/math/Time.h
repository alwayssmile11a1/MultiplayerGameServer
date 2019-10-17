#ifndef TIME_H
#define TIME_H

#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */

class Time
{
private:
	static float start;

public:
	static void Init();

	//Get time since the start of frame
	static float GetTimeFSinceGameStart();


};

#endif // ! TIME_H

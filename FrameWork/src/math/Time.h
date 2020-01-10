#ifndef TIME_H
#define TIME_H

#include <windows.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */
#include <winnt.h>
#include <iostream>

//class Time
//{
//private:
//	static float start;
//
//public:
//	static void Init();
//
//	//Get time since the start of frame
//	static float GetTimeFSinceGameStart();
//
//
//};

//class Time
//{
//public:
//
//	Time();
//
//	void Update();
//
//	float GetDeltaTime() const { return mDeltaTime; }
//
//	double GetTime() const;
//
//	float GetTimef() const
//	{
//		return static_cast< float >(GetTime());
//	}
//
//	float GetFrameStartTime() const { return mFrameStartTimef; }
//
//
//	static Time sInstance;
//
//private:
//	float		mDeltaTime;
//	uint64_t	mDeltaTick;
//
//	double		mLastFrameStartTime;
//	float		mFrameStartTimef;
//	double		mPerfCountDuration;
//
//};

class Time
{
public:

	Time();

	void Update(float dt);

	float GetFrameStartTime() const;


	static Time sInstance;

private:
	float start;
	float		mFrameStartTimef;
};

#endif // ! TIME_H

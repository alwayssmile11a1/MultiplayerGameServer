#include "Time.h"
#include "HanabiConsole.h"

//float Time::start;
//void Time::Init()
//{
//	start = clock();
//}
//
//float Time::GetTimeFSinceGameStart()
//{
//	return (clock() - start)/CLOCKS_PER_SEC;
//}

//#if !_WIN32
//#include <chrono>
//using namespace std::chrono;
//#endif
//
//Time	Time::sInstance;
//
//namespace
//{
//#if _WIN32
//	LARGE_INTEGER sStartTime = { 0 };
//#else
//	high_resolution_clock::time_point sStartTime;
//#endif
//}
//
//Time::Time()
//{
//#if _WIN32
//	LARGE_INTEGER perfFreq;
//	QueryPerformanceFrequency(&perfFreq);
//	mPerfCountDuration = 1.0 / perfFreq.QuadPart;
//
//	QueryPerformanceCounter(&sStartTime);
//
//	mLastFrameStartTime = GetTime();
//#else
//	sStartTime = high_resolution_clock::now();
//#endif
//}
//
//void Time::Update()
//{
//
//	double currentTime = GetTime();
//
//	mDeltaTime = (float)(currentTime - mLastFrameStartTime);
//
//	mLastFrameStartTime = currentTime;
//	mFrameStartTimef = static_cast< float > (mLastFrameStartTime);
//
//}
//
//double Time::GetTime() const
//{
//#if _WIN32
//	LARGE_INTEGER curTime, timeSinceStart;
//	QueryPerformanceCounter(&curTime);
//
//	timeSinceStart.QuadPart = curTime.QuadPart - sStartTime.QuadPart;
//
//	return timeSinceStart.QuadPart * mPerfCountDuration;
//#else
//	auto now = high_resolution_clock::now();
//	auto ms = duration_cast< milliseconds >(now - sStartTime).count();
//	//a little uncool to then convert into a double just to go back, but oh well.
//	return static_cast< double >(ms) / 1000;
//#endif
//}

Time	Time::sInstance;

Time::Time()
{
	start = clock();
}

void Time::Update(float dt)
{
	mFrameStartTimef += dt + 0.001f;
}

float Time::GetFrameStartTime() const
{
	return mFrameStartTimef;
	//return (clock() - start) / CLOCKS_PER_SEC;
}
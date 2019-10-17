#ifndef AVERAGEFLOATVARIABLE_H
#define AVERAGEFLOATVARIABLE_H

#include "../math/Time.h"

class AverageFloatVariable
{
public:
	AverageFloatVariable(float duration = 1.0f)
	{
		mValue = 0.0f;
		mDuration = duration;
		mTimeLastEntry = Time::GetTimeFSinceGameStart();
	}

	void Update(float inValue)
	{
		float time = Time::GetTimeFSinceGameStart();
		float timeSinceLastEntry = time - mTimeLastEntry;

		//now update our value by whatever amount of the duration that was..
		float fractionOfDuration = (timeSinceLastEntry / mDuration);
		if (fractionOfDuration > 1.f) { fractionOfDuration = 1.f; }

		mValue = mValue * (1.f - fractionOfDuration) + inValue * fractionOfDuration;

		mTimeLastEntry = time;
	}

	float GetValue() const { return mValue; }

private:
	float mValue;
	float mDuration;
	float mTimeLastEntry;
};

#endif


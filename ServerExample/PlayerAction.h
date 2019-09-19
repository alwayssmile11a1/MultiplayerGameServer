#pragma once

#include "HanabiMath.h"
#include "HanabiMultiplayer.h"

//Hold the action information of the current player
class PlayerAction
{
private:
	float mTimeStamp;
	Vector2 mVelocity;
	float mDeltaTime;
	bool mIsShooting;

public:
	PlayerAction();
	PlayerAction(float timeStamp, float deltaTime, const Vector2 &velocity, bool isShooting);
	~PlayerAction();

	const Vector2& GetVelocity() const { return mVelocity; }
	bool GetIsShooting() const { return mIsShooting; }
	float GetDeltaTime() const { return mDeltaTime; }
	float GetTimeStamp() const { return mTimeStamp; }

	void OnNetworkRead(InputMemoryBitStream & inInputStream);

};

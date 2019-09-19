#include "PlayerAction.h"

PlayerAction::PlayerAction()
{
}

PlayerAction::PlayerAction(float timeStamp, float deltaTime, const Vector2 &velocity, bool isShooting)
{
	mTimeStamp = timeStamp;
	mDeltaTime = deltaTime;
	mVelocity = velocity;
	mIsShooting = isShooting;
}

PlayerAction::~PlayerAction()
{
}

void PlayerAction::OnNetworkRead(InputMemoryBitStream & inInputStream)
{
	inInputStream.Read(mVelocity);
	inInputStream.Read(mDeltaTime);
	inInputStream.Read(mVelocity);
	inInputStream.Read(mIsShooting);
}

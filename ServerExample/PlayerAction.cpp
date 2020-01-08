#include "PlayerAction.h"

PlayerAction::PlayerAction()
{
	mTimeStamp = 0.0f;
	mVelocity.Set(0,0);
	mDeltaTime = 0.0f;
	mIsShooting = false;
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
	bool hasTeamNumber;
	inInputStream.Read(hasTeamNumber);
	if (hasTeamNumber)
	{
		inInputStream.Read(mTeamNumber);
	}
	inInputStream.Read(mTimeStamp);
	inInputStream.Read(mDeltaTime);
	inInputStream.Read(mVelocity);
	inInputStream.Read(mIsShooting);
}

//PLAYER ACTIONS//
int PlayerActions::Count()
{
	return mPlayerActions.size();
}

bool PlayerActions::AddPlayerAction(const PlayerAction &playerAction)
{
	// we might have already received this move in another packet(since we're sending the same move in multiple packets )
	//so make sure it's new...
	//Get timestamp
	float timeStamp = playerAction.GetTimeStamp();

	if (timeStamp > mLastActionTimeStamp)
	{
		mLastActionTimeStamp = timeStamp;
		mPlayerActions.emplace_back(playerAction);
		return true;
	}

	return false;
}

void PlayerActions::Clear()
{
	mPlayerActions.clear();
}

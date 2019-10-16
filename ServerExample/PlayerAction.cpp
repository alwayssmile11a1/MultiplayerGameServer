#include "PlayerAction.h"

PlayerAction::PlayerAction()
{
}

PlayerAction::~PlayerAction()
{
}

void PlayerAction::OnNetworkRead(InputMemoryBitStream & inInputStream) const
{
	inInputStream.Read(mVelocity);
	inInputStream.Read(mDeltaTime);
	inInputStream.Read(mVelocity);
	inInputStream.Read(mIsShooting);
}

//PLAYER ACTIONS//
int PlayerActions::Count()
{
	return mPlayerActions.size();
}

const PlayerAction& PlayerActions::AddPlayerAction(const PlayerAction &playerAction)
{
	// we might have already received this move in another packet(since we're sending the same move in multiple packets )
	//so make sure it's new...
	//Get timestamp
	float timeStamp = playerAction.GetTimeStamp();

	if (timeStamp > mLastActionTimeStamp)
	{
		mLastActionTimeStamp = timeStamp;
		mPlayerActions.emplace_back(playerAction);
	}
}

#include "PlayerAction.h"

PlayerAction::PlayerAction()
{
}

PlayerAction::PlayerAction(float timeStamp, const Vector2 &velocity, bool isShooting)
{
	mTimeStamp = timeStamp;
	mVelocity = velocity;
	mIsShooting = isShooting;
}

PlayerAction::~PlayerAction()
{
}

void PlayerAction::OnNetworkWrite(OutputMemoryBitStream & inOutputStream)
{

}



//PLAYER ACTIONS//
std::unique_ptr< PlayerActions >	PlayerActions::sInstance;

const std::unique_ptr<PlayerActions>& PlayerActions::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance.reset(new PlayerActions());
	}

	return sInstance;
}

int PlayerActions::Count()
{
	return mPlayerActions.size();
}

const PlayerAction& PlayerActions::AddPlayerAction(float timeStamp, const Vector2& velocity, bool isShooting)
{
	mPlayerActions.emplace_back(timeStamp, velocity, isShooting);
	return mPlayerActions.back();
}

void PlayerActions::RemovePlayerAction(float timeStamp)
{

}

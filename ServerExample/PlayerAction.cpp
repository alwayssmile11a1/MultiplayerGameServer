#include "PlayerAction.h"

PlayerAction::PlayerAction()
{
}

PlayerAction::PlayerAction(const Vector2 &velocity, bool isShooting)
{
	mVelocity = velocity;
	mIsShooting = isShooting;
}

PlayerAction::~PlayerAction()
{
}

void PlayerAction::OnNetworkRead(InputMemoryBitStream & inInputputStream)
{

}

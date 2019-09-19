#include "Player.h"

Player::Player()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Dynamic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(100, 100);
	//bodyDef.linearDrag.Set(10, 10);
	//bodyDef.mass = 2;
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->PutExtra(this);
}

Player::~Player()
{
}

void Player::Update(float dt)
{

}

uint32_t Player::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t dirtyState) const
{
	if (dirtyState & PRS_PlayerId)
	{
		inOutputStream.Write(mPlayerId);
	}

	if (dirtyState & PRS_Position)
	{
		inOutputStream.Write(mMainBody->GetPosition());
	}

	if (dirtyState & PRS_Velocity)
	{
		inOutputStream.Write(mMainBody->GetVelocity());
	}

	if (dirtyState & PRS_Rotation)
	{
		inOutputStream.Write(mRotation);
	}

	if (dirtyState & PRS_Health)
	{
		inOutputStream.Write(mHealth);
	}

	return PRS_AllState;
}


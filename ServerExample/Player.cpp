#include "Player.h"

Player::Player()
{
	
}

Player::~Player()
{
}

void Player::Update(float dt)
{
	

	//update sprite position
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
}

uint32_t Player::Write(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	return uint32_t();
}

void Player::Read(InputMemoryBitStream & inInputStream)
{

}

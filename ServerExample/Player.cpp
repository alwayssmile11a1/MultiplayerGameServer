#include "Player.h"

Player::Player()
{
	
}

Player::~Player()
{
}

void Player::Update(float dt)
{

}

uint32_t Player::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	inOutputStream.Write(mPosition);


	return 1;
}


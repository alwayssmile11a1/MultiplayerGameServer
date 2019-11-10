#include "Enemy.h"



Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::Render(SpriteBatch * batch)
{
}

void Enemy::Update(float dt)
{
}

uint32_t Enemy::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	return uint32_t();
}

void Enemy::OnNetworkDestroy()
{
}

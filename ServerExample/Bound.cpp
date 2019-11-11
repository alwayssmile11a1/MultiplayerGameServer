#include "Bound.h"

Bound::Bound()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Static;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(8 * 3.1f, 8 * 3.1f);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = BOUND_BIT;
	mMainBody->maskBits = PLAYER_BIT | BULLET_BIT | ENEMY_BIT;
	mMainBody->PutExtra(this);
}
Bound::~Bound()
{

}

uint32_t Bound::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	inOutputStream.Write(mMainBody->GetPosition());
	inOutputStream.Write(mMainBody->GetSize());

	return 1;
}
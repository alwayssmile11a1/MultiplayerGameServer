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
	mMainBody->maskBits = PLAYER_BIT | BULLET_BIT;
	mMainBody->PutExtra(this);
}
Bound::~Bound()
{

}

void Bound::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 position;
	inInputStream.Read(position);

	Vector2 size;
	inInputStream.Read(size);

	//set the current position back to the position of this player on server 
	mMainBody->SetPosition(position.x, position.y);
	mMainBody->SetSize(size.x, size.y);
}
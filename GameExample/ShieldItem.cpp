#include "ShieldItem.h"



ShieldItem::ShieldItem()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.isSensor = true;
	bodyDef.position.Set(30, 30);
	bodyDef.size.Set(30, 28);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = SHIELD_BIT;
	mMainBody->maskBits = PLAYER_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("item_shield")[0]);
	mSprite.SetSize(30, 28);
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
}


ShieldItem::~ShieldItem()
{
}

void ShieldItem::OnHitPlayer()
{
}

void ShieldItem::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 position;
	inInputStream.Read(position);
	//set the current position back to the position of this player on server 
	mMainBody->SetPosition(position.x, position.y);
	mSprite.SetPosition(position.x, position.y);
}

void ShieldItem::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

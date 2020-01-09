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

uint32_t ShieldItem::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	inOutputStream.Write(mMainBody->GetPosition());

	return 1;
}

void ShieldItem::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

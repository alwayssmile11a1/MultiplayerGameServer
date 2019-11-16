#include "StarItem.h"



StarItem::StarItem()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Static;
	bodyDef.position.Set(30, 30);
	bodyDef.size.Set(30, 28);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = STAR_BIT;
	mMainBody->maskBits = PLAYER_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("item_star")[0]);
	mSprite.SetSize(30, 28);
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
}


StarItem::~StarItem()
{
}

void StarItem::OnHitPlayer()
{
}

void StarItem::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 position;
	inInputStream.Read(position);
	//set the current position back to the position of this player on server 
	mMainBody->SetPosition(position.x, position.y);
	mSprite.SetPosition(position.x, position.y);
}

void StarItem::OnNetworkDestroy()
{
}

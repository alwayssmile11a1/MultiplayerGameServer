#include "Metal.h"

Metal::Metal()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Static;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(8 * 3.1, 8 * 3.1);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = METAL_BIT;
	mMainBody->maskBits = PLAYER_BIT | BULLET_BIT | ENEMY_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("metal")[0]);
	mSprite.SetSize(8 * 3.1, 8 * 3.1);
}

Metal::~Metal()
{

}

void Metal::Render(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}

void Metal::Update(float dt)
{

}

void Metal::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 position;
	inInputStream.Read(position);
	//set the current position back to the position of this player on server 
	mMainBody->SetPosition(position.x, position.y);
	mSprite.SetPosition(position.x, position.y);
}

void Metal::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}
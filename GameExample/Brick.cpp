#include "Brick.h"

Brick::Brick()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Static;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(8 * 3.1, 8 * 3.1);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = BRICK_BIT;
	mMainBody->maskBits = PLAYER_BIT | BULLET_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("brick")[0]);
	mSprite.SetSize(8 * 3.1, 8 * 3.1);
}
Brick::~Brick()
{

}

void Brick::Render(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}

void Brick::Update(float dt)
{

}

void Brick::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 position;
	inInputStream.Read(position);
	//set the current position back to the position of this player on server 
	mMainBody->SetPosition(position.x, position.y);
	mSprite.SetPosition(position.x, position.y);
}

void Brick::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}
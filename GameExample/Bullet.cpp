#include "Bullet.h"

Bullet::Bullet()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Static;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(3, 4);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = BULLET_BIT;
	mMainBody->maskBits = PLAYER_BIT | BRICK_BIT | METAL_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("bullet")[0]);
	mSprite.SetSize(3, 4);
}
Bullet::~Bullet()
{

}

void Bullet::Render(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}

void Bullet::Update(float dt)
{

}

void Bullet::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 position;
	inInputStream.Read(position);
	//set the current position back to the position of this player on server 
	mMainBody->SetPosition(position.x, position.y);
	mSprite.SetPosition(position.x, position.y);
}

void Bullet::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}
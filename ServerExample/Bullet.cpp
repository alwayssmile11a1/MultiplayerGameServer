#include "Bullet.h"

Bullet::Bullet()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(4, 4);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = BULLET_BIT;
	mMainBody->maskBits = PLAYER_BIT | BRICK_BIT | METAL_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("bullet")[0]);
	mSprite.SetSize(4, 4);

	mSpeed = 6.0f;
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
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
}

uint32_t Bullet::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	inOutputStream.Write(mMainBody->GetPosition());
	inOutputStream.Write(mMainBody->GetVelocity());

	return 1;
}

void Bullet::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}
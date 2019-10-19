#include "Brick.h"

Brick::Brick()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Static;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(8 * 3.1f, 8 * 3.1f);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = BRICK_BIT;
	mMainBody->maskBits = PLAYER_BIT | BULLET_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("brick")[0]);
	mSprite.SetSize(26, 26);
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

uint32_t Brick::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	inOutputStream.Write(mMainBody->GetPosition());

	return 1;
}

void Brick::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}
#include "Metal.h"

Metal::Metal()
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
	mSprite.SetRegion(p.GetRegion("metal")[0]);
	mSprite.SetSize(26, 26);
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
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
}

uint32_t Metal::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	inOutputStream.Write(mMainBody->GetPosition());

	return 1;
}

void Metal::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}
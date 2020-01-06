#include "Grass.h"

Grass::Grass()
{
	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("grass")[0]);
	mSprite.SetSize(26, 26);
}
Grass::~Grass()
{

}

void Grass::LateRender(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}

void Grass::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 position;
	inInputStream.Read(position);
	mSprite.SetPosition(position.x, position.y);
	ClientNetworkManager::Instance->AddToLateDrawnObjects(this);
}
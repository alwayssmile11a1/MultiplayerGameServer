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

void Grass::Render(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}

uint32_t Grass::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	inOutputStream.Write(mSprite.GetPosition());

	return 1;
}
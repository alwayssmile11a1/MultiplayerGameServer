#include "LobbyPlayer.h"

LobbyPlayer::LobbyPlayer()
{
	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("greentank_1")[0]);
	mSprite.SetSize(26, 26);
}

LobbyPlayer::~LobbyPlayer()
{
}

void LobbyPlayer::Render(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}
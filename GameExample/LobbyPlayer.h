#pragma once

#include "HanabiSprite.h"
#include "SharedTextures.h"

class LobbyPlayer : public GameObject
{
private:

	Sprite mSprite;

public:

	LobbyPlayer();
	~LobbyPlayer();

	void Render(SpriteBatch *batch);

	void SetSpriteRegion(const TextureRegion& region)
	{
		mSprite.SetRegion(region);
	}
	void SetPosition(const Vector2& position) { mSprite.SetPosition(position.x, position.y); }
};

#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "SharedTextures.h"
#include "CollisionBit.h"

class Brick : public NetworkGameObject
{

private:
	Body* mMainBody;
	Texture mTexture;
	Sprite mSprite;

public:
	CLASS_IDENTIFICATION('BR', Brick);

	Brick();
	~Brick();

	uint32_t GetAllStateMask() const override { return 1; };

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;

	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;
};
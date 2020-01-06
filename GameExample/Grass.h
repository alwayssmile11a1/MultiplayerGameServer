#pragma once

#include "ClientNetworkManager.h"
#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "SharedTextures.h"

class Grass : public NetworkGameObject
{

private:
	Sprite mSprite;

public:
	CLASS_IDENTIFICATION('GR', Grass);

	Grass();
	~Grass();

	uint32_t GetAllStateMask() const override { return 1; };

	void SetPosition(const Vector2 &inPosition)
	{
		mSprite.SetPosition(inPosition.x, inPosition.y);
	};

	void LateRender(SpriteBatch *batch);
	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
};
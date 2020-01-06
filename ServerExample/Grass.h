#pragma once

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

	void Render(SpriteBatch *batch) override;
	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
};
#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "SharedTextures.h"

class Brick : public NetworkGameObject
{

private:
	Body * mMainBody;
	Sprite mSprite;

public:
	CLASS_IDENTIFICATION('BR', Brick);

	Brick();
	~Brick();

	uint32_t GetAllStateMask() const override { return 1; };

	void SetPosition(const Vector2 &inPosition)
	{
		mMainBody->SetPosition(inPosition.x, inPosition.y);
		mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
	};

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;

	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void OnNetworkDestroy() override;
};
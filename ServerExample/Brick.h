#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"

class Brick : public NetworkGameObject
{

private:
	Body * mMainBody;
	Texture mTexture;

public:
	CLASS_IDENTIFICATION('BR', Brick);

	Brick();
	~Brick();

	uint32_t GetAllStateMask() const override { return 1; };

	void SetPosition(const Vector2 &inPosition) { mMainBody->SetPosition(inPosition.x, inPosition.y); };

	void Update(float dt) override;

	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void OnNetworkDestroy() override;
};
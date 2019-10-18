#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"

class Bound : public NetworkGameObject
{

private:
	Body * mMainBody;

public:
	CLASS_IDENTIFICATION('BO', Bound);

	Bound();
	~Bound();

	uint32_t GetAllStateMask() const override { return 1; };

	void SetPosition(const Vector2 &inPosition) { mMainBody->SetPosition(inPosition.x, inPosition.y); };
	void SetBodySize(float x, float y) { mMainBody->SetSize(x, y); };

	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
};
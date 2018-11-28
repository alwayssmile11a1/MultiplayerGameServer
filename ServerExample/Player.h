#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"

class Player : public NetworkGameObject
{
private:
	int mPlayerId;
	Body *mMainBody;
	Vector2 mPosition;

public:
	CLASS_IDENTIFICATION('PL', Player);

	Player();
	~Player();
	
	void SetPosition(const Vector2 &inPosition) { mPosition = inPosition; };
	void SetPlayerId(int playerId) { mPlayerId = playerId; };
	int GetPlayerId() {return mPlayerId;};
	uint32_t GetAllStateMask() const override { return 1; };
	void Update(float dt) override;
	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;

};

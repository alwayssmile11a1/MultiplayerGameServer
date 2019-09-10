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
	int mHealth;
	Body *mMainBody;

public:
	CLASS_IDENTIFICATION('PL', Player);

	enum PlayerReplicationState
	{
		PRS_PlayerId = 1 << 0,
		PRS_Position = 1 << 1,
		PRS_Health = 1 << 2,

		PRS_AllState = PRS_PlayerId | PRS_Position | PRS_Health
	};

	Player();
	~Player();
	
	uint32_t GetAllStateMask() const override { return PRS_AllState; };
	void SetPosition(const Vector2 &inPosition) { mMainBody->SetPosition(inPosition.x, inPosition.y); };
	void SetPlayerId(int playerId) { mPlayerId = playerId; };
	int GetPlayerId() {return mPlayerId;};
	void Update(float dt) override;
	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;

};
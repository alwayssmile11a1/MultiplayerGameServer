#pragma once

#include "ClientNetworkManager.h"
#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "Proxy.h"
class Player : public NetworkGameObject
{
private:
	int mPlayerId;
	int mHealth;
	Texture mTexture;
	Sprite mSprite;
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

	void SetPlayerId(int playerId) { mPlayerId = playerId; };
	int GetPlayerId() {return mPlayerId;};

	uint32_t GetAllStateMask() const override { return PRS_AllState; };

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;

	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;

};

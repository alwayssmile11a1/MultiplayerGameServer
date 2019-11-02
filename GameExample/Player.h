#pragma once

#include "PlayerAction.h"
#include "ClientNetworkManager.h"
#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "Proxy.h"
#include "ExplosionEffect.h"
#include "SharedTextures.h"
#include "CollisionBit.h"

class Player : public NetworkGameObject
{
private:

	float mMoveSpeed;
	int mPlayerId;
	int mHealth;
	bool mIsShooting;
	
	float mShootingRate;
	float mShootingTimer;

	Body *mMainBody;
	Sprite mSprite;

	//Network stuff
	float mTimeLocationBecameOutOfSync = 0.0f;
	float mTimeVelocityBecameOutOfSync = 0.0f;

	void SimulateAction(const PlayerAction& playerAction);
	void SimulateAction(float totalTime);
	void InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition, const Vector2& oldVelocity);

public:
	CLASS_IDENTIFICATION('PL', Player);

	enum PlayerReplicationState
	{
		PRS_PlayerId = 1 << 0,
		PRS_Position = 1 << 1,
		PRS_Velocity = 1 << 2,
		PRS_Health = 1 << 3,

		PRS_AllState = PRS_PlayerId | PRS_Position | PRS_Velocity | PRS_Health
	};

	Player();
	~Player();

	
	void SetPlayerId(int playerId) { mPlayerId = playerId; };
	int GetPlayerId() {return mPlayerId;};

	uint32_t GetAllStateMask() const override { return PRS_AllState; };

	void UpdateRotation();

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;
	
	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;

};

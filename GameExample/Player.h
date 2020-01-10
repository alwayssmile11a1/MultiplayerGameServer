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
#include "LobbyPlayer.h"
#include <vector>

class Player : public NetworkGameObject
{
private:

	float mMoveSpeed;
	int mPlayerId;
	int mHealth;
	bool mIsShooting;
	float mShootingRate;
	float mShootingTimer;
	int mTeamNumber = 0;
	bool hasShield = false;
	Body *mMainBody;
	Sprite mSprite;
	Sprite mShieldSprite;
	LobbyPlayer lobbyPlayer;



	//Network stuff
	float mTimeLocationBecameOutOfSync = 0.0f;
	float mTimeVelocityBecameOutOfSync = 0.0f;

	void SimulateAction(const PlayerAction& playerAction);
	void SimulateAction(float totalTime);
	void InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition, const Vector2& oldVelocity, bool alwaysInterpolate);

	static std::vector<Player*> mPlayers;

public:

	CLASS_IDENTIFICATION('PL', Player);

	enum PlayerReplicationState
	{
		PRS_PlayerId = 1 << 0,
		PRS_Position = 1 << 1,
		PRS_Velocity = 1 << 2,
		PRS_Health = 1 << 3,
		PRS_TeamNumber = 1 << 4,
		PRS_Upgrade = 1 << 5,
		PRS_Shield = 1 << 6,

		PRS_AllState = PRS_PlayerId | PRS_Position | PRS_Velocity | PRS_Health | PRS_TeamNumber | PRS_Upgrade | PRS_Shield
	};

	Player();
	~Player();

	
	void SetPlayerId(int playerId) { mPlayerId = playerId; };
	int GetPlayerId() {return mPlayerId;};
	Body* GetBody() {return mMainBody;}
	uint32_t GetAllStateMask() const override { return PRS_AllState; };

	void UpdateRotation();

	void Render(SpriteBatch *batch) override;
	void RenderLobbyPlayer(SpriteBatch *batch);
	void Update(float dt) override;
	
	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;

};

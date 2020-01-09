#pragma once

#include "ServerNetworkManager.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "SharedTextures.h"

class Player : public NetworkGameObject
{
private:
	int mPlayerId;
	int mHealth;
	Body *mMainBody;
	bool mIsShooting;
	float mMoveSpeed;
	Sprite mSprite;
	int mTeamNumber = 0;
	float mShootingRate;
	float mShootingTimer;
	bool mHasShield = false;
	void SimulateAction(const PlayerAction& playerAction);

public:
	CLASS_IDENTIFICATION('PL', Player);

	static std::vector<Player*> mPlayers;

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
	
	uint32_t GetAllStateMask() const override { return PRS_AllState; };
	void SetPosition(const Vector2 &inPosition) { mMainBody->SetPosition(inPosition.x, inPosition.y); };
	void SetPlayerId(int playerId) { mPlayerId = playerId; };
	int GetPlayerId() {return mPlayerId;};
	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;
	void UpdateRotation();
	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void OnNetworkDestroy() override;
	int GetTeamNumber() {
		return mTeamNumber;
	}
	void UpgradePlayer();
	void SetHasShield(bool hasShield);
	bool HasShield() {
		return mHasShield;
	}
};
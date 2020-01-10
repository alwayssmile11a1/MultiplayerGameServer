#pragma once

#include "ClientNetworkManager.h"
#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "SharedTextures.h"

class Enemy: public NetworkGameObject
{
protected: 
	int mEnemyNetworkGameObjectId;
	int mHealth;
	Body *mMainBody;
	bool mIsShooting;
	float mMoveSpeed;
	Sprite mSprite;
	float mTimeLocationBecameOutOfSync = 0.0f;
	float mShootingRate;
	float mShootingTimer;

	void SimulateAction(float totalTime);
	void InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition, const Vector2& oldVelocity);
public:
	CLASS_IDENTIFICATION('EN', Enemy);

	Enemy();
	~Enemy();

	enum EnemyReplicationState
	{
		ERS_EnemyID = 1 << 0,
		ERS_Position = 1 << 1,
		ERS_Velocity = 1 << 2,
		ERS_Rotation = 1 << 3,
		ERS_Health	 = 1 << 4,

		ERS_AllState = ERS_EnemyID | ERS_Position | ERS_Velocity | ERS_Rotation | ERS_Health
	};

	uint32_t GetAllStateMask() const override { return ERS_AllState; };

	void SetPosition(const Vector2 &inPosition) { mMainBody->SetPosition(inPosition.x, inPosition.y); }
	void SetIgnoredCollisionBody(Body* body) { mMainBody->SetIgnoredCollisionBody(body); }
	void SetVelocity(const Vector2 &inVelocity)
	{
		mMainBody->SetVelocity(inVelocity.x, inVelocity.y);

		if (inVelocity.x == 0)
		{
			if (inVelocity.y > 0)
			{
				mSprite.SetRotation(0);
			}
			else
			{
				mSprite.SetRotation(180);
			}
		}
		else
		{
			if (inVelocity.x > 0)
			{
				mSprite.SetRotation(90);
			}
			else
			{
				mSprite.SetRotation(-90);
			}
		}

	}

	float GetSpeed() { return mMoveSpeed; }

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;
	void UpdateRotation();
	void SetPlayerNetworkGameObjectID(int playerNetworkGameObjectId) { mEnemyNetworkGameObjectId = playerNetworkGameObjectId; }
	int GetPlayerNetworkGameObjectId() { return mEnemyNetworkGameObjectId; }
	Body* GetBody() { return mMainBody; }
	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;
};

#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "ServerNetworkManager.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "SharedTextures.h"

class Enemy: public NetworkGameObject
{
protected: 
	int mEnemyNetworkGameObjectId;
	int mHealth;
	Body *mMainBody;
	float mMoveSpeed;
	Sprite mSprite;

	float mShootingRate;
	float mShootingTimer;
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
	void RandomVelocity();

	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void OnNetworkDestroy() override;
};

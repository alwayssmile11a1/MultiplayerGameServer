#pragma once

#include "ServerNetworkManager.h"
#include "HanabiSprite.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "SharedTextures.h"
#include "CollisionBit.h"

class Bullet : public NetworkGameObject
{

private:
	Body * mMainBody;
	Texture mTexture;
	Sprite mSprite;

	float mSpeed;

	int mIgnoredNetworkGameObjectId;

public:
	CLASS_IDENTIFICATION('BU', Bullet);

	enum BulletReplicationState
	{
		BRS_IgnoredID = 1 << 0,
		BRS_Position = 1 << 1,
		BRS_Velocity = 1 << 2,
		BRS_AllState = BRS_IgnoredID| BRS_Position | BRS_Velocity
	};

	Bullet();
	~Bullet();

	uint32_t GetAllStateMask() const override { return BRS_AllState; };

	void SetPosition(const Vector2 &inPosition) { mMainBody->SetPosition(inPosition.x, inPosition.y); }
	void SetIgnoredCollisionBody(Body* body) {mMainBody->SetIgnoredCollisionBody(body);}
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

	float GetSpeed() { return mSpeed; }

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;
	void SetIgnoredNetworkGameObjectID(int ignoredNetworkGameObjectId){mIgnoredNetworkGameObjectId = ignoredNetworkGameObjectId;}
	int GetIgnoredNetworkGameObjectId() { return mIgnoredNetworkGameObjectId;}

	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void OnNetworkDestroy() override;
};
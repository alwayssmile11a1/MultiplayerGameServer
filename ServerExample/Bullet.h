#pragma once

#include "HanabiMultiplayer.h"
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

	int mPlayerNetworkGameObjectId;

public:
	CLASS_IDENTIFICATION('BU', Bullet);

	Bullet();
	~Bullet();

	uint32_t GetAllStateMask() const override { return 1; };

	void SetPosition(const Vector2 &inPosition) { mMainBody->SetPosition(inPosition.x, inPosition.y); }

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
	void SetPlayerNetworkGameObjectID(int playerNetworkGameObjectId){mPlayerNetworkGameObjectId = playerNetworkGameObjectId;}
	int GetPlayerNetworkGameObjectId() { return mPlayerNetworkGameObjectId;}

	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void OnNetworkDestroy() override;
};
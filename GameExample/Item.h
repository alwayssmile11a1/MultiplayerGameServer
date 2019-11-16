#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "SharedTextures.h"

class Item: public NetworkGameObject
{
protected:
	Body * mMainBody;
	Texture mTexture;
	Sprite mSprite;

	int mItemNetworkGameObjectId;
public:
	
	Item();
	~Item();

	uint32_t GetAllStateMask() const override { return 1; };

	void SetPosition(const Vector2 &inPosition)
	{
		mMainBody->SetPosition(inPosition.x, inPosition.y);
		mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
	};

	void Render(SpriteBatch *batch) override;
	virtual void OnHitPlayer() {}
	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;
};


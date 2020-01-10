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
	void Update(float dt) override;
	void Render(SpriteBatch *batch) override;
	virtual void OnHitPlayer() {}
	uint32_t OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void OnNetworkDestroy() override;
};


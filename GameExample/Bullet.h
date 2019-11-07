#pragma once

#include "ClientNetworkManager.h"
#include "HanabiSprite.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "ExplosionEffect.h"
#include "SharedTextures.h"
#include "CollisionBit.h"

class Bullet : public NetworkGameObject
{

private:
	Body * mMainBody;
	Texture mTexture;
	Sprite mSprite;

	float mSpeed;

	//float mTimeLocationBecameOutOfSync;

	void SimulateMovement(float totalTime);
	void InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition);

public:
	CLASS_IDENTIFICATION('BU', Bullet);

	enum BulletReplicationState
	{
		BRS_PlayerID = 1 << 0,
		BRS_Position = 1 << 1,
		BRS_Velocity = 1 << 2,

		BRS_AllState = BRS_PlayerID | BRS_Position | BRS_Velocity
	};

	Bullet();
	~Bullet();

	uint32_t GetAllStateMask() const override { return 1; };

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;

	void SetPosition(float x, float y){mMainBody->SetPosition(x, y);}
	void SetVelocity(float x, float y) { mMainBody->SetVelocity(x, y); }
	void FakeExplosion();
	float GetSpeed() { return mSpeed;}

	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;
};
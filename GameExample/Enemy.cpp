#include "Enemy.h"



Enemy::Enemy()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.position.Set(30, 40);
	bodyDef.size.Set(26, 30);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = ENEMY_BIT;
	mMainBody->maskBits = PLAYER_BIT | BRICK_BIT | METAL_BIT | BOUND_BIT | ENEMY_BIT | BULLET_BIT;
	mMainBody->PutExtra(this);
	mMainBody->SetVelocity(0, 1.5f);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("enemy_1")[0]);
	mSprite.SetSize(26, 30);;

	mMoveSpeed = 2.0f;
}


Enemy::~Enemy()
{
}

void Enemy::Render(SpriteBatch * batch)
{
	batch->Draw(mSprite);
}

void Enemy::Update(float dt)
{
	//WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, dt);

	UpdateRotation();
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);

	//Debug::Log("dt %f\n", dt);
	//Debug::Log("origin %f %f\n", mMainBody->GetPosition().x, mMainBody->GetPosition().y);
}

void Enemy::UpdateRotation() 
{
	if (mMainBody->GetVelocity().x == 0 && mMainBody->GetVelocity().y == 0) return;

	if (mMainBody->GetVelocity().x == 0)
	{
		if (mMainBody->GetVelocity().y > 0)
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
		if (mMainBody->GetVelocity().x > 0)
		{
			mSprite.SetRotation(90);
		}
		else
		{
			mSprite.SetRotation(-90);
		}
	}
}

void Enemy::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	//Save the current position on client
	Vector2 oldPosition = mMainBody->GetPosition();
	Vector2 oldVelocity = mMainBody->GetVelocity();

	if (dirtyState & ERS_EnemyID)
	{
		inInputStream.Read(mEnemyNetworkGameObjectId);
	}

	if (dirtyState & ERS_Position)
	{
		Vector2 position;
		inInputStream.Read(position);
		//set the current position back to the position of this bullet on server 
		mMainBody->SetPosition(position.x, position.y);
	}

	if (dirtyState & ERS_Velocity)
	{
		Vector2 velocity;
		inInputStream.Read(velocity);
		//set the current velocity back to the velocity of this player on server 
		mMainBody->SetVelocity(velocity.x, velocity.y);
	}

	if (dirtyState & ERS_Health) 
	{
		inInputStream.Read(mHealth);
	}

	if ((dirtyState & ERS_EnemyID) == 0)
	{
		//Simulate movement with round trip time for remote players
		//Debug::Log("rtt %f\n", ClientNetworkManager::Instance->GetAverageRoundTripTime());
		SimulateAction(ClientNetworkManager::Instance->GetAverageRoundTripTime());
		InterpolateClientSidePrediction(ClientNetworkManager::Instance->GetAverageRoundTripTime(), oldPosition, oldVelocity);
	}
}

void Enemy::SimulateAction(float totalTime)
{
	//simulate movement for an additional totalTime
	//Predict that the current player will continue moving in the direction that it did previously
	//let's break into framerate sized chunks though so that we don't run through walls and do crazy things...
	float chunkTime = 1.0f / 60.0f;

	while (true)
	{
		if (totalTime < chunkTime)
		{
			//Update word to simulate the current player action (also check collisions)
			WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, totalTime);
			break;
		}
		else
		{
			//Update word to simulate the current player action (also check collisions)
			WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, chunkTime);
			totalTime -= chunkTime;
		}
	}
}

void Enemy::InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition, const Vector2& oldVelocity)
{

	//Different. Interpolate the old position towards the simulated position and set it to the current player
	if (oldPosition.x != mMainBody->GetPosition().x || oldPosition.y != mMainBody->GetPosition().y)
	{
		//have we been out of sync, or did we just become out of sync?
		float time = Time::sInstance.GetFrameStartTime();
		if (mTimeLocationBecameOutOfSync == 0.0f)
		{
			mTimeLocationBecameOutOfSync = time;
		}

		float durationOutOfSync = time - mTimeLocationBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			//Lerp by an amount of durationOutOfSync / roundTripTime
			//Debug::Log("old %f %f\n", oldPosition.x, oldPosition.y);
			//Debug::Log("new %f %f\n", mMainBody->GetPosition().x, mMainBody->GetPosition().y);
			Vector2 interpolatedPosition = Math2D::Lerp(oldPosition, mMainBody->GetPosition(), /*durationOutOfSync / roundTripTime*/0.1f);
			mMainBody->SetPosition(interpolatedPosition.x, interpolatedPosition.y);
			//Debug::Log("result2: %f %f\n", interpolatedPosition.x, interpolatedPosition.y);
		}
		else
		{
			//since we don't lerp anymore, we are in sync with the server
			mTimeLocationBecameOutOfSync = 0.0f;
		}
	}
	else
	{
		mTimeLocationBecameOutOfSync = 0.0f;
	}
}

void Enemy::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

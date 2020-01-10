#include "Bullet.h"

Bullet::Bullet()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(4, 4);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = BULLET_BIT;
	mMainBody->maskBits = PLAYER_BIT | BRICK_BIT | METAL_BIT | BOUND_BIT | ENEMY_BIT;
	mMainBody->PutExtra(this);

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("bullet")[0]);
	mSprite.SetSize(4, 4);

	mSpeed = 5.0f;

	//mTimeLocationBecameOutOfSync = 0.0f;
}
Bullet::~Bullet()
{

}

void Bullet::Render(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}

void Bullet::Update(float dt)
{
	//WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, dt);

	if (mMainBody == nullptr) return;

	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);

	if (mFakeExplosion)
	{
		ExplosionEffectCollector::PlayEffect(mMainBody->GetPosition());
		mFakeExplosion = false;
	}
}

void Bullet::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	Vector2 oldPosition = mMainBody->GetPosition();

	if (dirtyState & BRS_PlayerID)
	{
		int playerID;
		inInputStream.Read(playerID);
		NetworkGameObjectPtr networkGameObjectPtr = NetworkLinkingContext::GetGameObject(playerID);
		//Debug::Log("%d\n", playerID);
		if (networkGameObjectPtr != nullptr)
		{
			Player* player = (Player*)networkGameObjectPtr.get();
			mMainBody->SetIgnoredCollisionBody(player->GetBody());
			//Debug::Log("%f\n", player->GetBody()->GetPosition().x);
		}
	}

	if (dirtyState & BRS_Position)
	{
		Vector2 position;
		inInputStream.Read(position);
		//set the current position back to the position of this bullet on server 
		mMainBody->SetPosition(position.x, position.y);
	}

	if (dirtyState & BRS_Velocity)
	{
		Vector2 velocity;
		inInputStream.Read(velocity);
		mMainBody->SetVelocity(velocity.x, velocity.y);
	}

	//if (dirtyState & BRS_Velocity == 0)
	//{
	//	SimulateMovement(ClientNetworkManager::Instance->GetAverageRoundTripTime());
	//	InterpolateClientSidePrediction(ClientNetworkManager::Instance->GetAverageRoundTripTime(), oldPosition);
	//}
}

void Bullet::SimulateMovement(float totalTime)
{
	//simulate movement for an additional totalTime
	//let's break into framerate sized chunks though so that we don't run through walls and do crazy things...
	float chunkTime = 1.0f / 60.0f;

	while (true)
	{
		if (totalTime < chunkTime)
		{
			//Update word to simulate the current bullet (also check collisions)
			WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, totalTime);
			break;
		}
		else
		{
			//Update word to simulate the current bullet (also check collisions)
			WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, chunkTime);
			totalTime -= chunkTime;
		}
	}
}

void Bullet::InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition)
{
	//Different. Interpolate the old position towards the simulated position and set it to the current player
	if (oldPosition.x != mMainBody->GetPosition().x || oldPosition.y != mMainBody->GetPosition().y)
	{
		////have we been out of sync, or did we just become out of sync?
		//float time = Time::GetTimeFSinceGameStart();
		//if (mTimeLocationBecameOutOfSync == 0.0f)
		//{
		//	mTimeLocationBecameOutOfSync = time;
		//}

		//float durationOutOfSync = time - mTimeLocationBecameOutOfSync;
		//if (durationOutOfSync < roundTripTime)
		//{
			//Lerp by an amount of durationOutOfSync / roundTripTime
			Vector2 interpolatedPosition = Math2D::Lerp(oldPosition, mMainBody->GetPosition(), 0.001f);
			mMainBody->SetPosition(interpolatedPosition.x, interpolatedPosition.y);
		//}
		//else
		//{
		//	//since we don't lerp anymore, we are in sync with the server
		//	mTimeLocationBecameOutOfSync = 0.0f;
		//}
	}
	//else
	//{
	//	mTimeLocationBecameOutOfSync = 0.0f;
	//}
}

void Bullet::OnNetworkDestroy()
{
	ExplosionEffectCollector::PlayEffect(mMainBody->GetPosition());
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

void Bullet::FakeExplosion()
{
	mFakeExplosion = true;
}
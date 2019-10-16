#include "Player.h"

Player::Player()
{
	//Setup sprite 
	mTexture = Texture("../Resources/mouse.png");
	mSprite.SetTexture(&mTexture);
	mSprite.ResetToWhole();
	mSprite.SetPosition(0, 0);
	mSprite.SetRotation(-90);

	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Dynamic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(100, 100);
	//bodyDef.linearDrag.Set(10, 10);
	//bodyDef.mass = 2;
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->PutExtra(this);
}

Player::~Player()
{
}

void Player::Render(SpriteBatch *batch)
{
	batch->Draw(mSprite);
}

void Player::Update(float dt)
{
	if (GetPlayerId() == Proxy::GetPlayerId())
	{
		if (Input::GetKey(DIK_RIGHT))
		{
			mMainBody->SetVelocity(5, mMainBody->GetVelocity().y);
		}

		if (Input::GetKey(DIK_LEFT))
		{
			mMainBody->SetVelocity(-5, mMainBody->GetVelocity().y);
		}

		if (Input::GetKey(DIK_UP))
		{
			mMainBody->SetVelocity(mMainBody->GetVelocity().x, 5);
		}

		if (Input::GetKey(DIK_DOWN))
		{
			mMainBody->SetVelocity(mMainBody->GetVelocity().x, -5);
		}

		if (Input::GetKeyUp(DIK_UP) || Input::GetKeyUp(DIK_DOWN))
		{
			mMainBody->SetVelocity(mMainBody->GetVelocity().x, 0);
		}

		if (Input::GetKeyUp(DIK_LEFT) || Input::GetKeyUp(DIK_RIGHT))
		{
			mMainBody->SetVelocity(0, mMainBody->GetVelocity().y);
		}

		if (Input::GetKey(DIK_SPACE))
		{
			//Shoot bullets
			mIsShooting = true;
		}
		else
		{

			mIsShooting = false;
		}

		//Add playerAction to list
		PlayerActions::GetInstance()->AddPlayerAction(Time::GetTimeF(), dt, mMainBody->GetVelocity(), mIsShooting);

		////just for testing purpose only
		//OutputMemoryBitStream outputStream;
		//outputStream.Write(PacketType::PT_State, 2);
		//outputStream.Write(mMainBody->GetPosition());
		//ClientNetworkManager::Instance->SendPacketToDestination(outputStream);
	}

	//update sprite position
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);

}

void Player::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	//Save the current position on client
	Vector2 oldPosition = mMainBody->GetPosition();
	Vector2 oldVelocity = mMainBody->GetVelocity();
	int oldRotation = mRotation;

	//Read info from server
	if (dirtyState & PRS_PlayerId)
	{
		inInputStream.Read(mPlayerId);
	}

	if (dirtyState & PRS_Position)
	{
		Vector2 position;
		inInputStream.Read(position);
		//set the current position back to the position of this player on server 
		mMainBody->SetPosition(position.x, position.y); 
	}

	if (dirtyState & PRS_Velocity)
	{
		Vector2 velocity;
		inInputStream.Read(velocity);
		//set the current velocity back to the velocity of this player on server 
		mMainBody->SetVelocity(velocity.x, velocity.y);
	}

	if (dirtyState & PRS_Rotation)
	{
		//set the current rotation back to the rotation of this player on server 
		inInputStream.Read(mRotation);
	}

	if (dirtyState & PRS_Health)
	{
		inInputStream.Read(mHealth);
	}

	if (GetPlayerId() == Proxy::GetPlayerId())
	{
		//all processed moves have been removed, so all that are left are unprocessed moves
		//so we must apply them...
		//TODO: remove processed move before this function
		for (const PlayerAction& playerAction : *PlayerActions::GetInstance())
		{
			//simulate movement
			SimulateAction(playerAction);
			InterpolateClientSidePrediction(ClientNetworkManager::Instance->GetAverageRoundTripTime(), oldPosition, oldVelocity, oldRotation);
		}
	}
	else
	{
		//Simulate movement with round trip time for remote players
		SimulateAction(ClientNetworkManager::Instance->GetAverageRoundTripTime());
		InterpolateClientSidePrediction(ClientNetworkManager::Instance->GetAverageRoundTripTime(), oldPosition, oldVelocity, oldRotation);
	}

}

void Player::SimulateAction(const PlayerAction& playerAction)
{
	//Set velocity
	Vector2 velocity = playerAction.GetVelocity();
	mMainBody->SetVelocity(velocity.x, velocity.y);
	
	//Set shooting
	mIsShooting = playerAction.GetIsShooting();

	//Update word to simulate the current player action (also check collisions)
	WorldCollector::GetWorld('PS')->Update(playerAction.GetDeltaTime());
}

void Player::SimulateAction(float totalTime)
{
	//simulate movement for an additional totalTime
	//Predict that the current player will continue moving in the direction that it did previously
	//let's break into framerate sized chunks though so that we don't run through walls and do crazy things...
	float chunkTime = 1.0f/60.0f;

	while (true)
	{
		if (totalTime < chunkTime)
		{
			//Update word to simulate the current player action (also check collisions)
			WorldCollector::GetWorld('PS')->Update(totalTime);
			break;
		}
		else
		{
			//Update word to simulate the current player action (also check collisions)
			WorldCollector::GetWorld('PS')->Update(chunkTime);
			totalTime -= chunkTime;
		}
	}
}

void Player::InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition, const Vector2& oldVelocity, int oldRotation)
{

	//Different. Interpolate the old position towards the simulated position and set it to the current player
	if (oldPosition.x != mMainBody->GetPosition().x || oldPosition.y != mMainBody->GetPosition().y)
	{
		//have we been out of sync, or did we just become out of sync?
		float time = Time::GetTimeF();
		if (mTimeLocationBecameOutOfSync == 0.0f)
		{
			mTimeLocationBecameOutOfSync = time;
		}

		float durationOutOfSync = time - mTimeLocationBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			if (GetPlayerId() == Proxy::GetPlayerId())
			{
				//Lerp by an amount of 0.1 (can be a different number but we use it for now)
				Vector2 interpolatedPosition = Math2D::Lerp(oldPosition, mMainBody->GetPosition(), 0.1f);
				mMainBody->SetPosition(interpolatedPosition.x, interpolatedPosition.y);
			}
			else
			{
				//Lerp by an amount of durationOutOfSync / roundTripTime
				Vector2 interpolatedPosition = Math2D::Lerp(oldPosition, mMainBody->GetPosition(), durationOutOfSync / roundTripTime);
				mMainBody->SetPosition(interpolatedPosition.x, interpolatedPosition.y);
			}
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


	//Different. Interpolate the old velocity towards the simulated velocity and set it to the current player
	if (oldVelocity.x != mMainBody->GetVelocity().x || oldVelocity.y != mMainBody->GetVelocity().y)
	{
		//have we been out of sync, or did we just become out of sync?
		float time = Time::GetTimeF();
		if (mTimeVelocityBecameOutOfSync == 0.f)
		{
			mTimeVelocityBecameOutOfSync = time;
		}

		//now interpolate to the correct value...
		float durationOutOfSync = time - mTimeVelocityBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			if (GetPlayerId() == Proxy::GetPlayerId())
			{
				//Lerp by an amount of 0.1 (can be a different number but we use it for now)
				Vector2 interpolatedVelocity = Math2D::Lerp(oldVelocity, mMainBody->GetVelocity(), 0.1f);
				mMainBody->SetVelocity(interpolatedVelocity.x, interpolatedVelocity.y);
			}
			else
			{
				//Lerp by an amount of durationOutOfSync / roundTripTime
				Vector2 interpolatedVelocity = Math2D::Lerp(oldVelocity, mMainBody->GetVelocity(), durationOutOfSync / roundTripTime);
				mMainBody->SetVelocity(interpolatedVelocity.x, interpolatedVelocity.y);
			}
		}
		else
		{
			//since we don't lerp anymore, we are in sync with the server
			mTimeVelocityBecameOutOfSync = 0.0f;
		}
	}
	else
	{
		mTimeVelocityBecameOutOfSync = 0;
	}


	if (oldRotation != mRotation)
	{
		//Different. Don't need to do anything for now since it's not something which is too noticable
	}

}

void Player::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

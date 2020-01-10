#include "Player.h"

std::vector<Player*> Player::mPlayers;

Player::Player()
{
	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(26, 26);
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);
	mMainBody->categoryBits = PLAYER_BIT;
	mMainBody->maskBits = BRICK_BIT | METAL_BIT | BULLET_BIT | PLAYER_BIT | ENEMY_BIT | BOUND_BIT | STAR_BIT | SHIELD_BIT;

	mMainBody->PutExtra(this);

	mMoveSpeed = 2.0f;

	mShootingRate = 1.0f;
	mShootingTimer = 0.0f;

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mShieldSprite.SetRegion(p.GetRegion("shield")[0]);
	mShieldSprite.SetSize(30, 30);
}

Player::~Player()
{
}

void Player::Render(SpriteBatch *batch)
{
	if (hasShield)
	{
		batch->Draw(mShieldSprite);
	}

	batch->Draw(mSprite);
}

void Player::RenderLobbyPlayer(SpriteBatch *batch)
{
	lobbyPlayer.Render(batch);
}

void Player::Update(float dt)
{
	//Debug::Log("%f\n", Time::sInstance.GetFrameStartTime());

	if (mMainBody == nullptr) return;

	if (!PlayerActions::GetInstance()->GetPlayerReady() && GetPlayerId() == Proxy::GetPlayerId())
	{
		if (Input::GetKeyDown(DIK_R))
		{
			PlayerActions::GetInstance()->SetPlayerReady(true);
			PlayerActions::GetInstance()->AddPlayerAction(Time::sInstance.GetFrameStartTime(), dt, mMainBody->GetVelocity(), mIsShooting);
		}

		if (Input::GetKeyDown(DIK_LEFT))
		{
			PlayerAction action(Time::sInstance.GetFrameStartTime(), dt, mMainBody->GetVelocity(), mIsShooting);
			action.SetPlayerTeamNumber(0);
			PlayerActions::GetInstance()->AddPlayerAction(action);
		}

		if (Input::GetKeyDown(DIK_RIGHT))
		{
			PlayerAction action(Time::sInstance.GetFrameStartTime(), dt, mMainBody->GetVelocity(), mIsShooting);
			action.SetPlayerTeamNumber(1);
			PlayerActions::GetInstance()->AddPlayerAction(action);
		}
	}

	if (ClientNetworkManager::Instance->GetIsAllPlayerReady())
	{
		if (GetPlayerId() == Proxy::GetPlayerId())
		{
			//if (mMainBody->GetVelocity().x == 0)
			{
				if (Input::GetKey(DIK_UP))
				{
					mMainBody->SetVelocity(0, mMoveSpeed);
				}

				if (Input::GetKey(DIK_DOWN))
				{
					mMainBody->SetVelocity(0, -mMoveSpeed);
				}

				if (Input::GetKeyUp(DIK_UP) || Input::GetKeyUp(DIK_DOWN))
				{
					mMainBody->SetVelocity(mMainBody->GetVelocity().x, 0);
				}
			}

			//if (mMainBody->GetVelocity().y == 0)
			{
				if (Input::GetKey(DIK_RIGHT))
				{
					mMainBody->SetVelocity(mMoveSpeed, 0);
				}

				if (Input::GetKey(DIK_LEFT))
				{
					mMainBody->SetVelocity(-mMoveSpeed, 0);
				}

				if (Input::GetKeyUp(DIK_LEFT) || Input::GetKeyUp(DIK_RIGHT))
				{
					mMainBody->SetVelocity(0, mMainBody->GetVelocity().y);
				}
			}

			if (Input::GetKey(DIK_SPACE) && Time::sInstance.GetFrameStartTime() >= mShootingTimer + 1 / mShootingRate)
			{
				//Shoot bullets
				mIsShooting = true;

				mShootingTimer = Time::sInstance.GetFrameStartTime();
			}
			else
			{

				mIsShooting = false;
			}

			//Add playerAction to list
			PlayerActions::GetInstance()->AddPlayerAction(Time::sInstance.GetFrameStartTime(), dt, mMainBody->GetVelocity(), mIsShooting);
		}
		else
		{
			//SimulateAction(dt);

			if (mMainBody->GetVelocity().x == 0 && mMainBody->GetVelocity().y == 0)
			{
				//we're in sync if our velocity is 0
				mTimeLocationBecameOutOfSync = 0.f;
			}
		}
	}

	//update sprite position
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);

	if (hasShield)
	{
		mShieldSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
	}

	//update rotation
	UpdateRotation();
}

void Player::UpdateRotation()
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

void Player::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
	//Save the current position on client
	Vector2 oldPosition = mMainBody->GetPosition();
	Vector2 oldVelocity = mMainBody->GetVelocity();
	//int oldRotation = mRotation;

	//Read info from server
	if (dirtyState & PRS_PlayerId)
	{
		inInputStream.Read(mPlayerId);
		mPlayers.push_back(this);
	}

	if (dirtyState & PRS_TeamNumber)
	{
		inInputStream.Read(mTeamNumber);
		if (mTeamNumber == 0)
		{
			if (mPlayerId == Proxy::GetPlayerId())
			{
				Proxy::SetTeamNumber(mTeamNumber);

				TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
				mSprite.SetRegion(p.GetRegion("greentank_1")[0]);
				mSprite.SetSize(26, 26);

				lobbyPlayer.SetSpriteRegion(p.GetRegion("greentank_1")[0]);
			}
			else
			{
				TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
				mSprite.SetRegion(p.GetRegion("yellowtank_1")[0]);
				mSprite.SetSize(26, 26);

				lobbyPlayer.SetSpriteRegion(p.GetRegion("yellowtank_1")[0]);
			}
		}
		else
		{
			if (mPlayerId == Proxy::GetPlayerId())
			{
				Proxy::SetTeamNumber(mTeamNumber);

				TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
				mSprite.SetRegion(p.GetRegion("greentank_1")[0]);
				mSprite.SetSize(26, 26);

				lobbyPlayer.SetSpriteRegion(p.GetRegion("greentank_1")[0]);
			}
			else
			{
				TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
				mSprite.SetRegion(p.GetRegion("yellowtank_1")[0]);
				mSprite.SetSize(26, 26);

				lobbyPlayer.SetSpriteRegion(p.GetRegion("yellowtank_1")[0]);
			}
		}

		int index = 0;
		for (const auto& player : mPlayers)
		{
			if (player->mTeamNumber == 0)
			{
				player->lobbyPlayer.SetPosition(Vector2(520 / 2, 800 / 2 - 35 * index));
				index++;
			}

			if (player->GetPlayerId() != Proxy::GetPlayerId())
			{
				if (player->mTeamNumber == Proxy::GetTeamNumber())
				{
					TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
					player->mSprite.SetRegion(p.GetRegion("yellowtank_1")[0]);
					player->mSprite.SetSize(26, 26);
				}
				else
				{
					TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
					player->mSprite.SetRegion(p.GetRegion("redtank_1")[0]);
					player->mSprite.SetSize(26, 26);
				}
			}
		}

		index = 0;
		for (const auto& player : mPlayers)
		{
			if (player->mTeamNumber == 1)
			{
				player->lobbyPlayer.SetPosition(Vector2(520 / 2 + 100, 800 / 2 - 35 * index));
				index++;
			}
		}
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

	//if (dirtyState & PRS_Rotation)
	//{
	//	//set the current rotation back to the rotation of this player on server 
	//	inInputStream.Read(mRotation);
	//}

	if (dirtyState & PRS_Health)
	{
		inInputStream.Read(mHealth);
	}

	if (dirtyState & PRS_Upgrade)
	{
		inInputStream.Read(mShootingRate);
	}

	if (dirtyState & PRS_Shield)
	{
		inInputStream.Read(hasShield);
	}

	//Don't do anything if it's the very first packet
	if ((dirtyState & PRS_PlayerId) == 0)
	{
		if (GetPlayerId() == Proxy::GetPlayerId())
		{
			//all processed moves have been removed, so all that are left are unprocessed moves
			//so we must apply them...
			//TODO: remove processed move before this function
			for (const PlayerAction& playerAction : *PlayerActions::GetInstance())
			{
				//simulate movement
				SimulateAction(playerAction);
			}

			InterpolateClientSidePrediction(ClientNetworkManager::Instance->GetAverageRoundTripTime(), oldPosition, oldVelocity, false);
		}
		else
		{
			if ((mMainBody->GetVelocity().x != 0 || mMainBody->GetVelocity().y != 0) && (mMainBody->GetVelocity().x == oldVelocity.x && mMainBody->GetVelocity().y == oldVelocity.y))
			{
				//Simulate movement with round trip time for remote players
				SimulateAction(ClientNetworkManager::Instance->GetAverageRoundTripTime());
				InterpolateClientSidePrediction(ClientNetworkManager::Instance->GetAverageRoundTripTime(), oldPosition, oldVelocity, true);
			}
			//else
			//{
			//	SimulateAction(ClientNetworkManager::Instance->GetAverageRoundTripTime());
			//	InterpolateClientSidePrediction(ClientNetworkManager::Instance->GetAverageRoundTripTime(), oldPosition, oldVelocity, 0.3f);
			//}
		}
	}
}

void Player::SimulateAction(const PlayerAction& playerAction)
{
	//Set velocity
	Vector2 velocity = playerAction.GetVelocity();
	mMainBody->SetVelocity(velocity.x, velocity.y);
	
	//Simulate collision
	WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, playerAction.GetDeltaTime());

	if (mMainBody == nullptr) return;

	UpdateRotation();

	//Set shooting
	mIsShooting = playerAction.GetIsShooting();
}

void Player::SimulateAction(float totalTime)
{
	//simulate movement for an additional totalTime
	//Predict that the current player will continue moving in the direction that it did previously
	//let's break into framerate sized chunks though so that we don't run through walls and do crazy things...
	float chunkTime = 1.0f/60.0f;
	
	//Debug::Log("%f\n", totalTime);

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

void Player::InterpolateClientSidePrediction(float roundTripTime, const Vector2& oldPosition, const Vector2& oldVelocity, bool alwaysInterpolate)
{

	//Different. Interpolate the old position towards the simulated position and set it to the current player
	if (oldPosition.x != mMainBody->GetPosition().x || oldPosition.y != mMainBody->GetPosition().y)
	{
		//have we been out of sync, or did we just become out of sync?
		float startTime = Time::sInstance.GetFrameStartTime();
		if (mTimeLocationBecameOutOfSync == 0.0f)
		{
			mTimeLocationBecameOutOfSync = startTime;
		}

		float durationOutOfSync = startTime - mTimeLocationBecameOutOfSync;
		if (durationOutOfSync < roundTripTime || alwaysInterpolate)
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
				Vector2 interpolatedPosition = Math2D::Lerp(oldPosition, mMainBody->GetPosition(), /*durationOutOfSync / roundTripTime*/0.1f);
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


	////Different. Interpolate the old velocity towards the simulated velocity and set it to the current player
	//if (oldVelocity.x != mMainBody->GetVelocity().x || oldVelocity.y != mMainBody->GetVelocity().y)
	//{
	//	//have we been out of sync, or did we just become out of sync?
	//	float time = Time::GetTimeFSinceGameStart();
	//	if (mTimeVelocityBecameOutOfSync == 0.f)
	//	{
	//		mTimeVelocityBecameOutOfSync = time;
	//	}

	//	//now interpolate to the correct value...
	//	float durationOutOfSync = time - mTimeVelocityBecameOutOfSync;
	//	if (durationOutOfSync < roundTripTime)
	//	{
	//		if (GetPlayerId() == Proxy::GetPlayerId())
	//		{
	//			//Lerp by an amount of 0.1 (can be a different number but we use it for now)
	//			Vector2 interpolatedVelocity = Math2D::Lerp(oldVelocity, mMainBody->GetVelocity(), 0.1f);
	//			mMainBody->SetVelocity(interpolatedVelocity.x, interpolatedVelocity.y);
	//		}
	//		else
	//		{
	//			//Lerp by an amount of durationOutOfSync / roundTripTime
	//			Vector2 interpolatedVelocity = Math2D::Lerp(oldVelocity, mMainBody->GetVelocity(), durationOutOfSync / roundTripTime);
	//			mMainBody->SetVelocity(interpolatedVelocity.x, interpolatedVelocity.y);
	//		}
	//	}
	//	else
	//	{
	//		//since we don't lerp anymore, we are in sync with the server
	//		mTimeVelocityBecameOutOfSync = 0.0f;
	//	}
	//}
	//else
	//{
	//	mTimeVelocityBecameOutOfSync = 0;
	//}


	//if (oldRotation != mRotation)
	//{
	//	//Different. Don't need to do anything for now since it's not something which is too noticable
	//}

}

void Player::OnNetworkDestroy()
{
	ExplosionEffectCollector::PlayEffect(mMainBody->GetPosition());
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;

	std::vector<Player*>::iterator p = std::find(mPlayers.begin(), mPlayers.end(), this);
	if (p != mPlayers.end())
	{
		mPlayers.erase(p);
	}
}

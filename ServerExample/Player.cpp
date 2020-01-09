#include "Player.h"

std::vector<Player*> Player::mPlayers;

Player::Player()
{
	mHealth = 10;
	mIsShooting = false;

	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(26, 26);
	//bodyDef.linearDrag.Set(10, 10);
	//bodyDef.mass = 2;
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);

	mMainBody->categoryBits = PLAYER_BIT;
	mMainBody->maskBits = BRICK_BIT | METAL_BIT | BULLET_BIT | PLAYER_BIT | BOUND_BIT | ENEMY_BIT | STAR_BIT | SHIELD_BIT;

	mMainBody->PutExtra(this);

	mMoveSpeed = 1.5f;

	mShootingRate = 1.0f;
	mShootingTimer = 0.0f;

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("yellowtank_1")[0]);
	mSprite.SetSize(26, 26);

	mPlayers.push_back(this);
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
	if (mMainBody == nullptr) return;

	//Get old variables
	Vector2 oldPosition = mMainBody->GetPosition();
	Vector2 oldVelocity = mMainBody->GetVelocity();

	ClientProxyPtr clientProxy = ServerNetworkManager::Instance->GetClientProxy(GetPlayerId());
	if (clientProxy != nullptr)
	{
		PlayerActions& playerActions = clientProxy->GetPlayerActions();
		for (const PlayerAction& playerAction : playerActions)
		{
			SimulateAction(playerAction);
		}

		playerActions.Clear();
	}

	//Get new variables
	Vector2 newPosition = mMainBody->GetPosition();
	Vector2 newVelocity = mMainBody->GetVelocity();

	if (oldPosition.x != newPosition.x || oldPosition.y != newPosition.y)
	{
		ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Position);
	}

	if (oldVelocity.x != newVelocity.x || oldVelocity.y != newVelocity.y)
	{
		ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Velocity);
	}

	//if (oldRotation != newRotation)
	//{
	//	ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Rotation);
	//}

	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);

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

void Player::SimulateAction(const PlayerAction& playerAction)
{
	if (playerAction.GetPlayerTeamNumber() > -1)
	{
		mTeamNumber = playerAction.GetPlayerTeamNumber();
		ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_TeamNumber);
	}

	//Set velocity
	Vector2 velocity = playerAction.GetVelocity();

	//Clamp velocity
	velocity.x = Math2D::Clamp(velocity.x, -mMoveSpeed, mMoveSpeed);
	velocity.y = Math2D::Clamp(velocity.y, -mMoveSpeed, mMoveSpeed);

	mMainBody->SetVelocity(velocity.x, velocity.y);

	//check collisions
	WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, playerAction.GetDeltaTime());

	if (mMainBody == nullptr) return;

	UpdateRotation();

	//Handle shooting
	mIsShooting = playerAction.GetIsShooting();
	if (mIsShooting == true && playerAction.GetTimeStamp() >= mShootingTimer + 1 / mShootingRate) //Double check to prevent cheating
	{
		//Shoot
		ServerNetworkManager::Instance->CreateBullet(GetNetworkId(), mMainBody, mSprite.GetRotation());

		mShootingTimer = playerAction.GetTimeStamp();
	}
	else
	{
		mIsShooting = false;
	}
}

uint32_t Player::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t dirtyState) const
{
	if (dirtyState & PRS_PlayerId)
	{
		inOutputStream.Write(mPlayerId);
	}

	if (dirtyState & PRS_TeamNumber)
	{
		inOutputStream.Write(mTeamNumber);
	}

	if (dirtyState & PRS_Position)
	{
		inOutputStream.Write(mMainBody->GetPosition());
		//Debug::Log("DIRTY: %f   %f %f\n", Time::GetTimeF(), mMainBody->GetPosition().x, mMainBody->GetPosition().y);
	}
	
	if (dirtyState & PRS_Velocity)
	{
		inOutputStream.Write(mMainBody->GetVelocity());
	}

	//if (dirtyState & PRS_Rotation)
	//{
	//	inOutputStream.Write(mRotation);
	//}

	if (dirtyState & PRS_Health)
	{
		inOutputStream.Write(mHealth);
	}

	if (dirtyState & PRS_Upgrade)
	{
		inOutputStream.Write(mShootingRate);
	}

	if (dirtyState & PRS_Shield)
	{
		inOutputStream.Write(mHasShield);
	}

	//Debug::Log("%f   %f %f\n\n", Time::GetTimeF(), mMainBody->GetPosition().x, mMainBody->GetPosition().y);

	return dirtyState;
}

void Player::UpgradePlayer()
{
	mShootingRate = 2.0f;
	ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Upgrade);
}

void Player::SetHasShield(bool hasShield)
{
	mHasShield = hasShield;
	ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Shield);
}

void Player::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
	std::vector<Player*>::iterator p = std::find(mPlayers.begin(), mPlayers.end(), this);
	if (p != mPlayers.end())
	{
		mPlayers.erase(p);
	}
}


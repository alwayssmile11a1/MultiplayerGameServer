#include "Player.h"

Player::Player()
{
	mHealth = 10;
	mIsShooting = false;
	mRotation = 0;

	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Kinematic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(26, 26);
	//bodyDef.linearDrag.Set(10, 10);
	//bodyDef.mass = 2;
	mMainBody = WorldCollector::GetWorld('PS')->CreateBody(bodyDef);

	mMainBody->categoryBits = PLAYER_BIT;
	mMainBody->maskBits = BRICK_BIT | METAL_BIT | BULLET_BIT;

	mMainBody->PutExtra(this);

	mMoveSpeed = 2.0f;
}

Player::~Player()
{
}

void Player::Update(float dt)
{
	//Get old variables
	Vector2 oldPosition = mMainBody->GetPosition();
	Vector2 oldVelocity = mMainBody->GetVelocity();
	float oldRotation = mRotation;

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
	float newRotation = mRotation;

	if (oldPosition.x != newPosition.x || oldPosition.y != newPosition.y)
	{
		ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Position);
	}

	if (oldVelocity.x != newVelocity.x || oldVelocity.y != newVelocity.y)
	{
		ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Velocity);
	}

	if (oldRotation != newRotation)
	{
		ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), PRS_Rotation);
	}
}

void Player::SimulateAction(const PlayerAction& playerAction)
{
	//Set velocity
	Vector2 velocity = playerAction.GetVelocity();

	//Clamp velocity
	velocity.x = Math2D::Clamp(velocity.x, -mMoveSpeed, mMoveSpeed);
	velocity.y = Math2D::Clamp(velocity.y, -mMoveSpeed, mMoveSpeed);

	mMainBody->SetVelocity(velocity.x, velocity.y);

	//Debug::Log("%f %f\n", velocity.x, velocity.y);

	//Update word to simulate the current player action (also check collisions)
	WorldCollector::GetWorld('PS')->Update(playerAction.GetDeltaTime());

	//TODO: Handle shooting
	mIsShooting = playerAction.GetIsShooting();
}

uint32_t Player::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t dirtyState) const
{
	if (dirtyState & PRS_PlayerId)
	{
		inOutputStream.Write(mPlayerId);
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

	if (dirtyState & PRS_Rotation)
	{
		inOutputStream.Write(mRotation);
	}

	if (dirtyState & PRS_Health)
	{
		inOutputStream.Write(mHealth);
	}

	//Debug::Log("%f   %f %f\n\n", Time::GetTimeF(), mMainBody->GetPosition().x, mMainBody->GetPosition().y);

	return dirtyState;
}

void Player::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}
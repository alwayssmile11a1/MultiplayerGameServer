#include "Player.h"

Player::Player()
{
	mHealth = 10;
	mIsShooting = false;
	mRotation = 0;

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
		ServerNetworkManager::Instance->SetStateDirty(GetNetworkId(), PRS_Position);
	}

	if (oldVelocity.x != newVelocity.x || oldVelocity.y != newVelocity.y)
	{
		ServerNetworkManager::Instance->SetStateDirty(GetNetworkId(), PRS_Velocity);
	}

	if (oldRotation != newRotation)
	{
		ServerNetworkManager::Instance->SetStateDirty(GetNetworkId(), PRS_Rotation);
	}
}

void Player::SimulateAction(const PlayerAction& playerAction)
{
	//Set velocity
	Vector2 velocity = playerAction.GetVelocity();
	mMainBody->SetVelocity(velocity.x, velocity.y);

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

	return dirtyState;
}


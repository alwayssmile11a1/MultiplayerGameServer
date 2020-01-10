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

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");
	mSprite.SetRegion(p.GetRegion("enemy_1")[0]);
	mSprite.SetSize(26, 30);;

	mMoveSpeed = 1.5f;
	mMainBody->SetVelocity(0, mMoveSpeed);

	mShootingTimer = 0;
	mShootingRate = 2;

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
	WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, dt);

	//Get old variables
	Vector2 oldVelocity = mMainBody->GetVelocity();
	UpdateRotation();
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);

	mShootingTimer += dt;
	if (mShootingTimer > mShootingRate) {
		//Shoot
		//ServerNetworkManager::Instance->CreateBullet(GetNetworkId(), mMainBody, mSprite.GetRotation());
		mShootingTimer = 0;
	}

	ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), ERS_Position);
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

void Enemy::RandomVelocity()
{
	Vector2 velocity = mMainBody->GetVelocity();
	int rand = std::rand() % 4 + 1;
	int dir = velocity.x < 0 ? 1 : velocity.x > 0 ? 3 : velocity.y > 0 ? 2 : 4;
	while (rand == dir)
	{
		rand = std::rand() % 4 + 1;
	}
	switch (rand)
	{
	case 1:
		mMainBody->SetVelocity(-mMoveSpeed, 0);
		break;
	case 2:
		mMainBody->SetVelocity(0, mMoveSpeed);
		break;
	case 3:
		mMainBody->SetVelocity(mMoveSpeed, 0);
		break;
	case 4:
		mMainBody->SetVelocity(0, -mMoveSpeed);
		break;
	default:
		break;
	}
	ServerNetworkManager::Instance->UpdateNetworkGameObject(GetNetworkId(), ERS_Velocity);
}

uint32_t Enemy::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	if (inDirtyState & ERS_EnemyID)
	{
		inOutputStream.Write(mEnemyNetworkGameObjectId);
	}

	if (inDirtyState & ERS_Position)
	{
		inOutputStream.Write(mMainBody->GetPosition());
	}

	if (inDirtyState & ERS_Velocity)
	{
		inOutputStream.Write(mMainBody->GetVelocity());
	}

	//if (dirtyState & PRS_Rotation)
	//{
	//	inOutputStream.Write(mRotation);
	//}

	if (inDirtyState & ERS_Health)
	{
		inOutputStream.Write(mHealth);
	}

	return inDirtyState;
}

void Enemy::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

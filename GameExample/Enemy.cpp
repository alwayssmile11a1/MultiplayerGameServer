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
	UpdateRotation();
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
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
}

void Enemy::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

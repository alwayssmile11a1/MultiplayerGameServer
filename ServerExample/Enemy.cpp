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
	mMainBody->maskBits = PLAYER_BIT | BRICK_BIT | METAL_BIT | BOUND_BIT | ENEMY_BIT;
	mMainBody->PutExtra(this);

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
	mMainBody->SetVelocity(0, mMoveSpeed);
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
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

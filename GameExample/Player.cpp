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
	if (GetNetworkId() == Proxy::playerObjectId)
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

		
	}
	//update sprite position
	mSprite.SetPosition(mMainBody->GetPosition().x, mMainBody->GetPosition().y);
}

void Player::OnNetworkRead(InputMemoryBitStream & inInputStream)
{
	Vector2 position;
	inInputStream.Read(position);
	mMainBody->SetPosition(position.x, position.y);
}

void Player::OnNetworkDestroy()
{
	WorldCollector::GetWorld('PS')->DestroyBody(mMainBody);
	mMainBody = nullptr;
}

#include "Cat.h"

Cat::Cat()
{
	//Setup sprite 
	texture = Texture("../Resources/mouse.png");
	sprite.SetTexture(&texture);
	sprite.ResetToWhole();
	sprite.SetPosition(0, 0);
	sprite.SetRotation(-90);

	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Dynamic;
	bodyDef.position.Set(0, 0);
	bodyDef.size.Set(100, 100);
	//bodyDef.linearDrag.Set(10, 10);
	//bodyDef.mass = 2;
	//mainBody = PlayScene::Instance->GetWorld()->CreateBody(bodyDef);
	mainBody->PutExtra(this);
}

Cat::~Cat()
{
}

void Cat::Render(SpriteBatch *batch)
{
	batch->Draw(sprite);
}

void Cat::Update(float dt)
{
	if (Input::GetKey(DIK_RIGHT))
	{
		mainBody->SetVelocity(5, mainBody->GetVelocity().y);
	}

	if (Input::GetKey(DIK_LEFT))
	{
		mainBody->SetVelocity(-5, mainBody->GetVelocity().y);
	}

	if (Input::GetKey(DIK_UP))
	{
		mainBody->SetVelocity(mainBody->GetVelocity().x, 5);
	}

	if (Input::GetKey(DIK_DOWN))
	{
		mainBody->SetVelocity(mainBody->GetVelocity().x, -5);
	}

	if (Input::GetKeyUp(DIK_UP) || Input::GetKeyUp(DIK_DOWN))
	{
		mainBody->SetVelocity(mainBody->GetVelocity().x, 0);
	}

	if (Input::GetKeyUp(DIK_LEFT) || Input::GetKeyUp(DIK_RIGHT))
	{
		mainBody->SetVelocity(0, mainBody->GetVelocity().y);
	}

	//update sprite position
	sprite.SetPosition(mainBody->GetPosition().x, mainBody->GetPosition().y);
}

uint32_t Cat::Write(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	return uint32_t();
}

void Cat::Read(InputMemoryBitStream & inInputStream)
{

}

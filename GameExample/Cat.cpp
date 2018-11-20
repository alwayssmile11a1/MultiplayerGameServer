#include "Cat.h"

Cat::Cat()
{
}

Cat::~Cat()
{
}

void Cat::Init(World* world)
{

	//Setup sprite 
	texture = Texture("../Resources/mouse.png");
	SetTexture(&texture);
	ResetToWhole();
	SetPosition(0, 0);
	SetRotation(-90);

	//Setup body
	BodyDef bodyDef;
	bodyDef.bodyType = Body::BodyType::Dynamic;
	bodyDef.position.Set(0,0);
	bodyDef.size.Set(100,100);
	//bodyDef.linearDrag.Set(10, 10);
	//bodyDef.mass = 2;
	mainBody = world->CreateBody(bodyDef);
	mainBody->PutExtra(this);

}

void Cat::Render(SpriteBatch *batch)
{
	batch->Draw(*this);
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

	//update position
	SetPosition(mainBody->GetPosition().x, mainBody->GetPosition().y);
}

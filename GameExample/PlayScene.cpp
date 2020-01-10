#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

//std::unique_ptr<PlayScene>PlayScene:: Instance;
//
//void PlayScene::Init()
//{
//	Instance.reset(new PlayScene());
//}

void PlayScene::SetBatch(SpriteBatch* batch)
{
	this->batch = batch;
	batch->SetCamera(&camera);
}

void PlayScene::SetClientNetworkManager(ClientNetworkManager* networkManager)
{
	this->networkManager = networkManager;
	this->networkManager->SetCamera(&camera);
}

//create stuff here
void PlayScene::Create()
{
	//setup camera
	camera.SetPosition(640/2, 640/2);

	//create world
	world = WorldCollector::CreateWorld('PS');
	world->SetGravity(0);
	world->SetContactListener(&contactListener);

}

//update scene here
void PlayScene::Update(float dt)
{
	//Debug::Log("Start Update\n");

	world->Update(dt);

	networkManager->ReceiveIncomingPackets();
	
	networkManager->Update(dt);

	networkManager->SendOutgoingPackets();

	//draw everything
	Render();

	//Debug::Log("End Update\n");
}

void PlayScene::Render()
{
	//draw every thing between batch begin and end
	batch->Begin();
	
	networkManager->Render(batch);

	//Render the shape of bodies
	//world->RenderBodiesDebug(batch);

	batch->End();
}

//release scene here
void PlayScene::Release()
{
	networkManager->Release();
}

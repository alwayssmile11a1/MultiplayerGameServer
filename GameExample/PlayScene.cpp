#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

std::unique_ptr<PlayScene>PlayScene:: Instance;

void PlayScene::Init()
{
	Instance.reset(new PlayScene());
}

void PlayScene::SetBatch(SpriteBatch* batch)
{
	this->batch = batch;
	batch->SetCamera(&camera);
}

void PlayScene::SetClientNetworkManager(ClientNetworkManager* networkManager)
{
	networkManager = networkManager;
}

//create stuff here
void PlayScene::Create()
{
	camera.SetPosition(0, 0);

	world.SetGravity(0);

}

//update scene here
void PlayScene::Update(float dt)
{

	world.Update(dt);

	//draw everything
	Render();
}

void PlayScene::Render()
{
	//draw every thing between batch begin and end
	batch->Begin();
	

	//Render the shape of bodies
	//world.RenderBodiesDebug(batch);

	batch->End();
}

//release scene here
void PlayScene::Release()
{
	world.Release();
}

#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::SetBatch(SpriteBatch* batch)
{
	this->batch = batch;
	batch->SetCamera(&camera);
}

//create stuff here
void PlayScene::Create()
{
	camera.SetPosition(0, 0);

	world.SetGravity(0);

	cat.Init(&world);

}

//update scene here
void PlayScene::Update(float dt)
{

	world.Update(dt);
	cat.Update(dt);


	//draw everything
	Render();
}

void PlayScene::Render()
{
	//draw every thing between batch begin and end
	batch->Begin();
	
	cat.Render(batch);


	//Render the shape of bodies
	//world.RenderBodiesDebug(batch);

	batch->End();
}

//release scene here
void PlayScene::Release()
{
	world.Release();
}

#include "GameExample.h"

GameExample::GameExample(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate)
	:Game(hInstance, windowName, screenWidth, screenHeight, isFullScreen, frameRate)
{

}
GameExample::~GameExample()
{

}

//Create game here
void GameExample::CreateGame()
{
	//create batch to draw everything
	batch.Create();

	//play scene
	playScene.Create();
	playScene.SetBatch(&batch);
	

	//Set scene to render
	SetScene(&playScene);

}

//update game here
void GameExample::UpdateGame(float dt)
{
	Game::UpdateGame(dt);

}

//remember to release everything
void GameExample::Release()
{
	Game::Release();
	playScene.Release();
	batch.Release();
}


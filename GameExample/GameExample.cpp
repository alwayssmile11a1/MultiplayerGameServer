#include "GameExample.h"

GameExample::GameExample(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate)
	:Game(hInstance, windowName, screenWidth, screenHeight, isFullScreen, frameRate)
{

}
GameExample::~GameExample()
{

}


void GameExample::CreateGame()
{
	
}
void GameExample::UpdateGame(float dt)
{
	

}

void GameExample::Release()
{
	Game::Release();
	
}


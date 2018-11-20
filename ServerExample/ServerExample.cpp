#include "ServerExample.h"

ServerExample::ServerExample(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate)
	:Game(hInstance, windowName, screenWidth, screenHeight, isFullScreen, frameRate)
{

}
ServerExample::~ServerExample()
{

}

//Create game here
void ServerExample::CreateGame()
{
	
}

//update game here
void ServerExample::UpdateGame(float dt)
{
	Game::UpdateGame(dt);

}

//remember to release everything
void ServerExample::Release()
{
	Game::Release();
}


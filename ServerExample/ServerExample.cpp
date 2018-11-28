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
	serverNetworkManager.Init(8080);

}

//update game here
void ServerExample::UpdateGame(float dt)
{
	Game::UpdateGame(dt);
	serverNetworkManager.ReceiveIncomingPackets();
	serverNetworkManager.SendOutgoingPackets();
}

//remember to release everything
void ServerExample::Release()
{
	Game::Release();
}


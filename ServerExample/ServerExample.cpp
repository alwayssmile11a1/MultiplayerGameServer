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
	mServerNetworkManager.Init(8080);
	mWorld = WorldCollector::CreateWorld('PS');
	mWorld->SetGravity(0);
	mWorld->SetContactListener(&mWorldListener);
	batch.Create();
}

//update game here
void ServerExample::UpdateGame(float dt)
{
	Game::UpdateGame(dt);
	mServerNetworkManager.ReceiveIncomingPackets();
	mServerNetworkManager.SendOutgoingPackets();
	mServerNetworkManager.Update(dt);

	mWorld->Update(dt);

	batch.Begin();

	mWorld->RenderBodiesDebug(&batch);

	batch.End();
}

//remember to release everything
void ServerExample::Release()
{
	Game::Release();
	batch.Release();
}


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
	camera.SetPosition(0, 0);
	batch.SetCamera(&camera);
}

//update game here
void ServerExample::UpdateGame(float dt)
{
	mServerNetworkManager.ReceiveIncomingPackets();

	mServerNetworkManager.Update(dt);
	
	mWorld->Update(dt);

	mServerNetworkManager.SendOutgoingPackets();

	//Render
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


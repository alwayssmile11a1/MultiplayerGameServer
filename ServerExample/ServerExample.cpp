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
	mWorld = WorldCollector::CreateWorld('PS');
	mWorld->SetGravity(0);
	mWorld->SetContactListener(&mWorldListener);
	batch.Create();
	camera.SetPosition(640/2, 640/2);
	batch.SetCamera(&camera);
	SharedTextures::Init();
	mServerNetworkManager.Init(8080);
}

//update game here
void ServerExample::UpdateGame(float dt)
{
	mServerNetworkManager.ReceiveIncomingPackets();

	mWorld->Update(dt);

	mServerNetworkManager.Update(dt);

	mServerNetworkManager.SendOutgoingPackets();

	//Render
	batch.Begin();
	
	mServerNetworkManager.Render(&batch);

	//mWorld->RenderBodiesDebug(&batch);

	batch.End();
}

//remember to release everything
void ServerExample::Release()
{
	Game::Release();
	batch.Release();
}


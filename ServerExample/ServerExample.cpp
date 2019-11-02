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

	// String to hold file contents
	std::string fileContents = "";
	int serverPort;
	// Attempt to load file using provided file path
	if (LoadFile("../Resources/networkinfo.xml", fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse map data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get server node
		rapidxml::xml_node<> *serverNode = currentMap.first_node("server");

		serverPort = atoi(serverNode->first_attribute("port")->value());
	}

	mServerNetworkManager.Init(serverPort);
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


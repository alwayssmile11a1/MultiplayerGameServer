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
	//Load client info
	// String to hold file contents
	std::string fileContents = "";
	std::string serverIP = "";
	std::string serverPort = "";
	// Attempt to load file using provided file path
	if (LoadFile("../Resources/networkinfo.xml", fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse map data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get server node
		rapidxml::xml_node<> *serverNode = currentMap.first_node("server");

		serverIP = serverNode->first_attribute("ip")->value();
		serverPort = serverNode->first_attribute("port")->value();
	}

	//init network manager
	clientNetworkManager.Init(serverIP + ":" + serverPort, "MyName");

	//create batch to draw everything
	batch.Create();

	//play scene
	playScene.Create();
	playScene.SetBatch(&batch);
	playScene.SetClientNetworkManager(&clientNetworkManager);

	//Set scene to render
	SetScene(&playScene);

	SharedTextures::Init();
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


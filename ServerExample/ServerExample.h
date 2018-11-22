#pragma once


#include "HanabiGame.h"
#include "ServerNetworkManager.h"

class ServerExample : public Game
{
private:
	ServerNetworkManager serverNetworkManager;

protected:
	//create all the things of a game here, such as world, object, ...
	void CreateGame() override;
	//update game by one frame (render, control game) - just write your game here
	void UpdateGame(float dt) override;


public:
	ServerExample(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate);
	~ServerExample();

	void Release() override;
};
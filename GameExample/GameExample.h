#pragma once


#include "HanabiGame.h"
#include "HanabiMultiplayer.h"
#include "PlayScene.h"
#include "ClientNetworkManager.h"


class GameExample : public Game
{
private:

	ClientNetworkManager clientNetworkManager;

	PlayScene playScene;
	SpriteBatch batch;


protected:
	//create all the things of a game here, such as world, object, ...
	void CreateGame() override;
	//update game by one frame (render, control game) - just write your game here
	void UpdateGame(float dt) override;


public:
	GameExample(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate);
	~GameExample();

	void Release() override;
};
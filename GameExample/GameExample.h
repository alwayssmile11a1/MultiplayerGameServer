#pragma once

#include "ClientNetworkManager.h"
#include "HanabiMultiplayer.h"
#include "HanabiGame.h"
#include "PlayScene.h"



class GameExample : public Game
{
private:
	PlayScene playScene;
	ClientNetworkManager clientNetworkManager;
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
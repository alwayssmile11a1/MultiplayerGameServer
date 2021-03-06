#pragma once

#include "HanabiScene.h"
#include "HanabiCamera.h"
#include "HanabiSprite.h"
#include "HanabiWorld.h"
#include "ClientNetworkManager.h"
#include "WorldCollisionListener.h"


class PlayScene : public Scene
{
private:
	WorldPtr world;
	SpriteBatch * batch;
	Camera camera;
	WorldCollisionListener contactListener;

	ClientNetworkManager* networkManager;

public:

	PlayScene();
	~PlayScene();

	void SetBatch(SpriteBatch* batch);
	void SetClientNetworkManager(ClientNetworkManager* networkManager);
	void Render();

	void Create() override;
	void Update(float dt) override;
	void Release() override;
};
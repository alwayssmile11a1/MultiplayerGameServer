#pragma once

#include "HanabiScene.h"
#include "HanabiCamera.h"
#include "HanabiSprite.h"
#include "HanabiWorld.h"
#include "ClientNetworkManager.h"

class PlayScene : public Scene
{
private:
	
	World world;
	SpriteBatch * batch;
	Camera camera;

	ClientNetworkManager networkManager;

	PlayScene();

public:

	~PlayScene();

	static void Init();
	static std::unique_ptr<PlayScene> Instance;

	World* GetWorld() { return &world; };

	void SetBatch(SpriteBatch* batch);
	void SetClientNetworkManager(ClientNetworkManager* networkManager);
	void Render();

	void Create() override;
	void Update(float dt) override;
	void Release() override;
};
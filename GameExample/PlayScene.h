#pragma once

#include "HanabiScene.h"
#include "HanabiCamera.h"
#include "HanabiSprite.h"
#include "Cat.h"
#include "HanabiWorld.h"

class PlayScene : public Scene
{
private:
	SpriteBatch * batch;
	Camera camera;

	World world;

	Cat cat;


public:
	PlayScene();
	~PlayScene();

	void SetBatch(SpriteBatch* batch);

	void Render();

	void Create() override;
	void Update(float dt) override;
	void Release() override;
};
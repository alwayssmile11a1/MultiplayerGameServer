#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "PlayScene.h"

class Cat : public NetworkGameObject
{
private:
	Texture texture;
	Sprite sprite;
	Body *mainBody;

public:
	CLASS_IDENTIFICATION('CAT',Cat);

	Cat();
	~Cat();

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;

	uint32_t Write(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void Read(InputMemoryBitStream & inInputStream) override;

};

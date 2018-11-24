#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"

class Cat : public Sprite
{
private:
	Texture texture;
	Body *mainBody;

public:
	Cat();
	~Cat();

	void Init(World *world);

	void Render(SpriteBatch *batch);
	void Update(float dt);

};

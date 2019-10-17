#pragma once
#include "HanabiSprite.h"

class SharedTextures
{
public:
	static void Init()
	{
		BattleCityTexture = Texture("../Resources/battlecityspritesheet.png");
	}

	static Texture BattleCityTexture;
};
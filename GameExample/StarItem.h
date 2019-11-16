#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "SharedTextures.h"
#include "Item.h"

class StarItem: public Item
{
public:
	CLASS_IDENTIFICATION('IS', StarItem);
	StarItem();
	~StarItem();

	void OnHitPlayer() override;
	void OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState) override;
	void OnNetworkDestroy() override;
};


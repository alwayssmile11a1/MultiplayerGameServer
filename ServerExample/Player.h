#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"

class Player : public NetworkGameObject
{
private:
	int mPlayerId;
	Texture mTexture;
	Sprite mSprite;
	Body *mMainBody;

public:
	CLASS_IDENTIFICATION('PL', Player);

	Player();
	~Player();

	void SetPlayerId(int playerId) { mPlayerId = playerId; };

	uint32_t GetAllStateMask() const override { return 0; };
	void Update(float dt) override;
	uint32_t Write(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const override;
	void Read(InputMemoryBitStream & inInputStream) override;

};

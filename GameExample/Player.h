#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiSprite.h"
#include "HanabiInput.h"
#include "HanabiBody.h"
#include "HanabiWorld.h"
#include "Proxy.h"
class Player : public NetworkGameObject
{
private:
	//int mPlayerId;
	Texture mTexture;
	Sprite mSprite;
	Body *mMainBody;

public:
	CLASS_IDENTIFICATION('PL', Player);

	Player();
	~Player();

	//void SetPlayerId(int playerId) { mPlayerId = playerId; };
	//int GetPlayerId() {return mPlayerId;};

	void Render(SpriteBatch *batch) override;
	void Update(float dt) override;

	void OnNetworkRead(InputMemoryBitStream & inInputStream) override;
	void OnNetworkDestroy() override;

};

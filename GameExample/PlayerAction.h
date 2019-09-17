#pragma once

#include "HanabiMath.h"
#include "HanabiMultiplayer.h"

//Hold the action information of the current player
class PlayerAction 
{
private:
	Vector2 mVelocity;
	bool mIsShooting;

public:
	PlayerAction();
	PlayerAction(const Vector2 &velocity, bool isShooting);
	~PlayerAction();

	void OnNetworkWrite(OutputMemoryBitStream & inOutputStream);

};

#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiMath.h"

//Hold the action information of the current player
class PlayerAction
{
private:
	float mTimeStamp;
	Vector2 mVelocity;
	float mDeltaTime;
	bool mIsShooting;

public:
	PlayerAction();
	~PlayerAction();

	const Vector2& GetVelocity() const { return mVelocity; }
	bool GetIsShooting() const { return mIsShooting; }
	float GetDeltaTime() const { return mDeltaTime; }
	float GetTimeStamp() const { return mTimeStamp; }

	void OnNetworkRead(InputMemoryBitStream & inInputStream) const;
};

//Hold list of playerActions
class PlayerActions
{
private:
	std::deque<PlayerAction> mPlayerActions;
	static std::unique_ptr<PlayerActions>	sInstance;
	float mLastActionTimeStamp;

public:

	typedef std::deque< PlayerAction >::const_iterator			const_iterator;
	typedef std::deque< PlayerAction >::const_reverse_iterator	const_reverse_iterator;

	//for for each, we have to match stl calling convention
	const_iterator	begin()					const { return mPlayerActions.begin(); }
	const_iterator	end()					const { return mPlayerActions.end(); }

	int Count();
	const PlayerAction& AddPlayerAction(const PlayerAction &playerAction);
	float GetLastActionTimeStamp() { return mLastActionTimeStamp; }
};

#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiMath.h"
#include <iostream>
#include <deque>

//Hold the action information of the current player
class PlayerAction
{
private:
	float mTimeStamp;
	Vector2 mVelocity;
	float mDeltaTime;
	bool mIsShooting;
	int mTeamNumber = -1;
public:
	PlayerAction();
	PlayerAction(float timeStamp, float deltaTime, const Vector2 &velocity, bool isShooting);
	~PlayerAction();

	const Vector2& GetVelocity() const { return mVelocity; }
	bool GetIsShooting() const { return mIsShooting; }
	float GetDeltaTime() const { return mDeltaTime; }
	float GetTimeStamp() const { return mTimeStamp; }
	int GetPlayerTeamNumber() const { return mTeamNumber; }
	void OnNetworkRead(InputMemoryBitStream & inInputStream);
};

//Hold list of playerActions
class PlayerActions
{
private:
	std::deque<PlayerAction> mPlayerActions;
	float mLastActionTimeStamp;
	bool mIsReady = false;
public:

	PlayerActions() { mLastActionTimeStamp = -1.0f; }

	typedef std::deque< PlayerAction >::const_iterator			const_iterator;
	typedef std::deque< PlayerAction >::const_reverse_iterator	const_reverse_iterator;

	//for for each, we have to match stl calling convention
	const_iterator	begin()					const { return mPlayerActions.begin(); }
	const_iterator	end()					const { return mPlayerActions.end(); }

	int Count();
	//Add player action if new
	bool AddPlayerAction(const PlayerAction &playerAction);
	float GetLastActionTimeStamp() { return mLastActionTimeStamp; }
	void Clear();
	void SetPlayerReady(bool isReady) { mIsReady = isReady; }
	bool GetPlayerReady() { return mIsReady; }
};
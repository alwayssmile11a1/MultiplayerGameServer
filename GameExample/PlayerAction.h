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

	void SetPlayerTeamNumber(int teamNumber) { mTeamNumber = teamNumber; }
	int GetPlayerTeamNumber() const { return mTeamNumber; }

	const Vector2& GetVelocity() const { return mVelocity;}
	bool GetIsShooting() const { return mIsShooting; }
	float GetDeltaTime() const { return mDeltaTime; }
	float GetTimeStamp() const { return mTimeStamp; }
	void OnNetworkWrite(OutputMemoryBitStream & inOutputStream) const;

};


//Hold list of playerActions
class PlayerActions
{
private:
	std::deque<PlayerAction> mPlayerActions;
	static std::unique_ptr<PlayerActions>	sInstance;
	bool mIsReady = false;
public:

	static const std::unique_ptr<PlayerActions>& GetInstance();

	typedef std::deque< PlayerAction >::const_iterator			const_iterator;
	typedef std::deque< PlayerAction >::const_reverse_iterator	const_reverse_iterator;

	//for for each, we have to match stl calling convention
	const_iterator	begin()					const { return mPlayerActions.begin(); }
	const_iterator	end()					const { return mPlayerActions.end(); }

	int Count();
	const void AddPlayerAction(const PlayerAction& action)
	{
		mPlayerActions.push_back(action);
	}
	const PlayerAction& AddPlayerAction(float timeStamp, float deltaTime, const Vector2& velocity, bool isShooting);
	void RemovePlayerActions(float timeStamp);
	void SetPlayerReady(bool isReady);
	bool GetPlayerReady() const { return mIsReady; }
};
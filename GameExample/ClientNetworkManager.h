#pragma once

#include <iostream>
#include <vector>
#include "HanabiMultiplayer.h"
#include "HanabiMath.h"
#include "HanabiConsole.h"
#include "Player.h"
#include "Grass.h"
#include "Brick.h"
#include "Metal.h"
#include "Enemy.h"
#include "StarItem.h"
#include "Bullet.h"
#include "Bound.h"
#include "Proxy.h"
#include "HanabiLabel.h"
#include "ExplosionEffect.h"

class ClientNetworkManager : public NetworkManager
{
private:
	Camera* mCamera;
	const float kTimeBetweenSendingHelloPacket = 1.0f;
	const float kTimeBetweenSendingGamePacket = 1.0f / 60.0f;

	float mTimeOfLastHello;
	float mTimeOfLastGamePacket;
	bool mIsAllPlayerReady = false;
	Font font;
	Label lobbyLabel;
	Label readyLabel;
	Label wonLabel;

	int mTeamWon = -1;

	AverageFloatVariable mAverageRoundTripTime;

	//just some states to keep track of what we have to do next 
	//eg. Uninitialized -> Call Init function -> SayingHello until server responses -> and we are Welcomed by server
	enum NetworkClientState
	{
		Uninitialized,
		SayingHello,
		Welcomed
	};

	std::string mPlayerName;
	SocketAddress mDestinationAddress;
	NetworkClientState mState;
	ClientReplicationManager clientReplicationManager;

	std::vector<NetworkGameObject*> lateDrawnObjects;

	void SendHelloPacket();
	void SendGamePackets();
	void HandleWelcomePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);
	void HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);
	void ReadLastActionProcessedOnServerTimeStamp(InputMemoryBitStream& inputMemoryStream);

public:

	static ClientNetworkManager* Instance;

public:
	ClientNetworkManager();
	~ClientNetworkManager();

	void Init(const std::string &destination, const std::string &playerName);
	void SetCamera(Camera* camera)
	{
		mCamera = camera;
	}
	void OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress) override;
	void OnSendPackets() override;
	void SendPacketToDestination(const OutputMemoryBitStream &outputMemoryStream) { SendPacket(outputMemoryStream, mDestinationAddress); };
	void Update(float dt);
	void Render(SpriteBatch* spriteBatch);

	void AddToLateDrawnObjects(NetworkGameObject* gameObjectPtr);

	float GetAverageRoundTripTime() { return mAverageRoundTripTime.GetValue(); }

	bool GetIsAllPlayerReady() { return mIsAllPlayerReady; }

	void Release();
};
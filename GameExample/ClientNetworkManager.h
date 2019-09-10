#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiMath.h"
#include "HanabiConsole.h"
#include "Player.h"
#include "Proxy.h"

class ClientNetworkManager: public NetworkManager
{
private:

	const float kTimeBetweenSendingHelloPacket = 1.0f;
	const float kTimeBetweenSendingGamePacket = 1.0/60;

	float mTimeOfLastHello;
	float mTimeOfLastGamePacket;

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
	

	void SendHelloPacket();
	void SendGamePackets();
	void HandleWelcomePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);
	void HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);

public:

	static ClientNetworkManager* instance;

public:
	ClientNetworkManager();
	~ClientNetworkManager();

	void Init(const std::string &destination, const std::string &playerName);
	void OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress) override;
	void OnSendPackets() override;
	void SendPacketToDestination(const OutputMemoryBitStream &outputMemoryStream) { SendPacket(outputMemoryStream, mDestinationAddress); };
	void Update(float dt);
	void Render(SpriteBatch* spriteBatch);

};
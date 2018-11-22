#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiMath.h"
#include "HanabiConsole.h"

class ServerNetworkManager : public NetworkManager
{
private:
	enum NetworkServerState
	{
		Uninitialized,
		GoodToGo,
	};

	NetworkServerState mState;


	void SendWelcomePacket();
	void SendPackets();

public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init(uint16_t inPort);
	void OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress) override;
	void OnSendPackets() override;
};
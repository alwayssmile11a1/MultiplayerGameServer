#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiMath.h"
#include "HanabiConsole.h"
#include "ClientProxy.h"

class ServerNetworkManager : public NetworkManager
{
private:
	enum NetworkServerState
	{
		Uninitialized,
		GoodToGo,
	};

	NetworkServerState mState;
	int mNewPlayerId;


	std::unordered_map<SocketAddress, ClientProxyPtr> mSocketAddressToClientMap;

	void HandleHelloPacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);
	void HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);

	void SendPackets();

public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init(uint16_t inPort);
	void OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress) override;
	void OnSendPackets() override;
};
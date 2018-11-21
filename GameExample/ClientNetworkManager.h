#pragma once

#include "HanabiMultiplayer.h"

class ClientNetworkManager: NetworkManager
{
private:
	

public:
	ClientNetworkManager();
	~ClientNetworkManager();


	void Init(const std::string &destination, const std::string &playerName);
	
	void SendOutgoingPackets();

	void OnPacketReceived(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress) override;

};
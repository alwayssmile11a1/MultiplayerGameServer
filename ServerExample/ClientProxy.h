#pragma once

#include "HanabiMultiplayer.h"

//Represent a client
class ClientProxy
{
private:

	int mPlayerId;
	std::string mPlayerName;

	ServerReplicationManager  serverReplicationManager;

public:

	ClientProxy(int mPlayerId, const std::string& mPlayerName);

	int GetPlayerId() const { return mPlayerId; }
	const std::string& GetPlayerName() const{ return mPlayerName; }
	ServerReplicationManager& GetServerReplicationManager() { return serverReplicationManager; }
	


};

typedef std::shared_ptr<ClientProxy> ClientProxyPtr;

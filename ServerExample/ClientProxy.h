#pragma once

#include "HanabiMultiplayer.h"

//Represent a client
class ClientProxy
{
private:

	int mPlayerId;
	std::string mPlayerName;
	NetworkGameObjectPtr mClientObject;
	ServerReplicationManager  mServerReplicationManager;

public:

	ClientProxy(int mPlayerId, const std::string& mPlayerName);
	void SetClientObject(NetworkGameObjectPtr inGameObject) { mClientObject = inGameObject; };
	NetworkGameObjectPtr GetClientObject() { return mClientObject; };
	int GetPlayerId() const { return mPlayerId; };
	const std::string& GetPlayerName() const { return mPlayerName; };
	ServerReplicationManager& GetServerReplicationManager() { return mServerReplicationManager; };
	


};

typedef std::shared_ptr<ClientProxy> ClientProxyPtr;

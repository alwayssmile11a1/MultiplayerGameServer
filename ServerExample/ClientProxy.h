#pragma once

#include "HanabiMultiplayer.h"
#include "PlayerAction.h"

//Represent a client
class ClientProxy
{
private:

	int mPlayerId;
	bool mIsLastActionTimeStampDirty;
	std::string mPlayerName;
	NetworkGameObjectPtr mClientObject;
	ServerReplicationManager  mServerReplicationManager;
	PlayerActions mPlayerActions;

public:

	ClientProxy(int mPlayerId, const std::string& mPlayerName);
	void SetClientObject(NetworkGameObjectPtr inGameObject) { mClientObject = inGameObject; };
	NetworkGameObjectPtr GetClientObject() { return mClientObject; };
	int GetPlayerId() const { return mPlayerId; };
	const std::string& GetPlayerName() const { return mPlayerName; };
	ServerReplicationManager& GetServerReplicationManager() { return mServerReplicationManager; };
	PlayerActions& GetPlayerActions() { return mPlayerActions; }
	void SetLastActionTimeStampDirty(bool isDirty) { mIsLastActionTimeStampDirty = isDirty; }
	bool IsLastActionTimeStampDirty() { return mIsLastActionTimeStampDirty; }
};

typedef std::shared_ptr<ClientProxy> ClientProxyPtr;
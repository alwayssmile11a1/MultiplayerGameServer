#pragma once

//just contain some static information
class Proxy
{
	static int playerId;
	static int playerNetworkGameObjectId;
public:

	static int GetPlayerId() {
		return playerId;
	}

	static int GetPlayerNetworkGameObjectId() {
		return playerNetworkGameObjectId;
	}
};
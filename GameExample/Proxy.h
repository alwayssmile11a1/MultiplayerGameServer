#pragma once

//just contain some static information
class Proxy
{
	static int playerId;
	static int playerNetworkGameObjectId;
	static int teamNumber;
public:
	static int GetPlayerId() {
		return playerId;
	}

	static void SetPlayerId(int id) {
		playerId = id;
	}

	static int GetTeamNumber() {
		return teamNumber;
	}

	static void SetTeamNumber(int number) {
		teamNumber = number;
	}

	static int GetPlayerNetworkGameObjectId() {
		return playerNetworkGameObjectId;
	}

	static void SetPlayerNetworkGameObjectId(int id) {
		playerNetworkGameObjectId = id;
	}
};
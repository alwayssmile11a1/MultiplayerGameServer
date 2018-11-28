#ifndef NETWORKLINKINGCONTEXT_H
#define NETWORKLINKINGCONTEXT_H

#include <unordered_map>
#include "NetworkGameObject.h"
class NetworkLinkingContext
{
private:
	static std::unordered_map<int, NetworkGameObjectPtr> mNetworkIdToGameObjectMap;

public:
	static NetworkGameObjectPtr GetGameObject(int inNetworkId);
	static void AddToNetworkIdToGameObjectMap(NetworkGameObjectPtr inGameObject);
	static void RemoveFromNetworkIdToGameObjectMap(NetworkGameObjectPtr inGameObject);
	static const std::unordered_map<int, NetworkGameObjectPtr>& GetNetworkIdToGameObjectMap();

};

#endif

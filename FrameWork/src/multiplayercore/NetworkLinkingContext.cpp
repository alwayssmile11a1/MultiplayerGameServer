#include "NetworkLinkingContext.h"

std::unordered_map<int, NetworkGameObjectPtr> NetworkLinkingContext::mNetworkIdToGameObjectMap;


NetworkGameObjectPtr NetworkLinkingContext::GetGameObject(int inNetworkId)
{
	auto gameObjectIt = mNetworkIdToGameObjectMap.find(inNetworkId);
	if (gameObjectIt != mNetworkIdToGameObjectMap.end())
	{
		return gameObjectIt->second;
	}
	else
	{
		return nullptr;
	}
}

void NetworkLinkingContext::AddToNetworkIdToGameObjectMap(NetworkGameObjectPtr inGameObject)
{
	mNetworkIdToGameObjectMap[inGameObject->GetNetworkId()] = inGameObject;
}

void NetworkLinkingContext::RemoveFromNetworkIdToGameObjectMap(NetworkGameObjectPtr inGameObject)
{
	mNetworkIdToGameObjectMap.erase(inGameObject->GetNetworkId());
}

const std::unordered_map<int, NetworkGameObjectPtr>& NetworkLinkingContext::GetNetworkIdToGameObjectMap()
{
	return mNetworkIdToGameObjectMap;
}
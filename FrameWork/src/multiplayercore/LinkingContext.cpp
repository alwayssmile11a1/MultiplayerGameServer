#include "LinkingContext.h"

LinkingContext::LinkingContext()
{
	mNextNetworkId = 1;
}

LinkingContext::~LinkingContext()
{
}

uint32_t LinkingContext::GetNetworkId(NetworkGameObject* inGameObject, bool inShouldCreateIfNotFound)
{
	auto it = mGameObjectToNetworkIdMap.find(inGameObject);
	if (it != mGameObjectToNetworkIdMap.end())
	{
		return it->second;
	}
	else if (inShouldCreateIfNotFound)
	{
		uint32_t newNetworkId = mNextNetworkId++;
		AddGameObject(inGameObject, newNetworkId);
		return newNetworkId;
	}
	else
	{
		return 0;
	}
}

NetworkGameObject* LinkingContext::GetGameObject(uint32_t inNetworkId) const
{
	auto it = mNetworkIdToGameObjectMap.find(inNetworkId);
	if (it != mNetworkIdToGameObjectMap.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void LinkingContext::AddGameObject(NetworkGameObject* inGameObject, uint32_t inNetworkId)
{
	mNetworkIdToGameObjectMap[inNetworkId] = inGameObject;
	mGameObjectToNetworkIdMap[inGameObject] = inNetworkId;
}

void LinkingContext::RemoveGameObject(NetworkGameObject *inGameObject)
{
	uint32_t networkId = mGameObjectToNetworkIdMap[inGameObject];
	mGameObjectToNetworkIdMap.erase(inGameObject);
	mNetworkIdToGameObjectMap.erase(networkId);
}



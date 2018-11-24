//#ifndef LINKINGCONTEXT_h
//#define LINKINGCONTEXT_h
//
//#include <unordered_map>
//#include<stdint.h>
//#include "NetworkGameObject.h"
//
////this class is deprecated
////a container that links networkids with network gameobjects
//class LinkingContext
//{
//public:
//
//	LinkingContext();
//	~LinkingContext();
//
//	uint32_t GetNetworkId(NetworkGameObject* inGameObject, bool inShouldCreateIfNotFound);
//
//	NetworkGameObject* GetGameObject(uint32_t inNetworkId) const;
//
//	void AddGameObject(NetworkGameObject* inGameObject, uint32_t inNetworkId);
//
//	void RemoveGameObject(NetworkGameObject *inGameObject);
//
//private:
//	std::unordered_map< uint32_t, NetworkGameObject* > mNetworkIdToGameObjectMap;
//	std::unordered_map< const NetworkGameObject*, uint32_t > mGameObjectToNetworkIdMap;
//
//	uint32_t mNextNetworkId;
//};
//
//#endif

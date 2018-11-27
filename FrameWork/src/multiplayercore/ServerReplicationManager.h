#ifndef SERVERREPLICATIONMANAGER_H
#define SERVERREPLICATIONMANAGER_H

#include "ReplicationCommand.h"
#include "MemoryBitStream.h"
#include <unordered_map>
#include "NetworkGameObject.h"

class ServerReplicationManager
{
public:
	void ReplicateCreate(NetworkGameObjectPtr gameObject, uint32_t inInitialDirtyState);
	void ReplicateDestroy(NetworkGameObjectPtr gameObject);
	void AddDirtyState(NetworkGameObjectPtr gameObject, uint32_t inDirtyState);
	void RemoveFromReplication(NetworkGameObjectPtr gameObject);

	void Write(OutputMemoryBitStream& inOutputStream);


private:

	uint32_t WriteCreateAction(OutputMemoryBitStream& inOutputStream, NetworkGameObjectPtr gameObject, uint32_t inDirtyState);
	uint32_t WriteUpdateAction(OutputMemoryBitStream& inOutputStream, NetworkGameObjectPtr gameObject, uint32_t inDirtyState);
	uint32_t WriteDestroyAction(OutputMemoryBitStream& inOutputStream, NetworkGameObjectPtr gameObject, uint32_t inDirtyState);

	std::unordered_map< NetworkGameObjectPtr, ReplicationCommand > mNetworkGameObjectToReplicationCommand;
	std::vector< NetworkGameObjectPtr > mNetworkGameObjectsToRemove;

};


#endif
#include "ServerReplicationManager.h"

void ServerReplicationManager::ReplicateCreate(NetworkGameObjectPtr gameObject, uint32_t inInitialDirtyState)
{
	mNetworkGameObjectToReplicationCommand[gameObject] = ReplicationCommand(inInitialDirtyState);
}

void ServerReplicationManager::ReplicateDestroy(NetworkGameObjectPtr gameObject)
{
	mNetworkGameObjectToReplicationCommand[gameObject].SetDestroy();
}

void ServerReplicationManager::RemoveFromReplication(NetworkGameObjectPtr gameObject)
{
	mNetworkGameObjectToReplicationCommand.erase(gameObject);
}

void ServerReplicationManager::AddDirtyState(NetworkGameObjectPtr gameObject, uint32_t inDirtyState)
{
	mNetworkGameObjectToReplicationCommand[gameObject].AddDirtyState(inDirtyState);
}

void ServerReplicationManager::Write(OutputMemoryBitStream& inOutputStream)
{
	//run through each replication command and do something...
	for (auto& pair : mNetworkGameObjectToReplicationCommand)
	{
		ReplicationCommand& replicationCommand = pair.second;
		if (replicationCommand.HasDirtyState())
		{
			int networkId = pair.first->GetNetworkId();

			//well, first write the network id...
			inOutputStream.Write(networkId);

			//only need 2 bits for action...
			ReplicationAction action = replicationCommand.GetAction();
			inOutputStream.Write(action, 2);

			uint32_t writtenState = 0;
			uint32_t dirtyState = replicationCommand.GetDirtyState();

			//now do what?
			switch (action)
			{
			case RA_Create:
				writtenState = WriteCreateAction(inOutputStream, pair.first, dirtyState);
				//once the create action is transmitted, future replication
				//of this object should be updates instead of creates
				replicationCommand.SetAction(RA_Update);
				break;
			case RA_Update:
				writtenState = WriteUpdateAction(inOutputStream, pair.first, dirtyState);
				break;
			case RA_Destroy:
				//don't need anything other than state!
				writtenState = WriteDestroyAction(inOutputStream, pair.first, dirtyState);
				//add this to the list of replication commands to remove
				mNetworkGameObjectsToRemove.emplace_back(pair.first);
				break;
			}

			//let's pretend everything was written- don't make this too hard
			replicationCommand.ClearDirtyState(writtenState);

		}
	}

	//remove replication commands for destroyed objects
	if (!mNetworkGameObjectsToRemove.empty())
	{
		for (auto gameObject : mNetworkGameObjectsToRemove)
		{
			RemoveFromReplication(gameObject);
		}

		mNetworkGameObjectsToRemove.clear();
	}
}


uint32_t ServerReplicationManager::WriteCreateAction(OutputMemoryBitStream& inOutputStream, NetworkGameObjectPtr gameObject, uint32_t inDirtyState)
{
	inOutputStream.Write(gameObject->GetClassId());
	return gameObject->Write(inOutputStream, inDirtyState);
}

uint32_t ServerReplicationManager::WriteUpdateAction(OutputMemoryBitStream& inOutputStream, NetworkGameObjectPtr gameObject, uint32_t inDirtyState)
{
	uint32_t writtenState = gameObject->Write(inOutputStream, inDirtyState);

	return writtenState;
}

uint32_t ServerReplicationManager::WriteDestroyAction(OutputMemoryBitStream& inOutputStream, NetworkGameObjectPtr gameObject, uint32_t inDirtyState)
{
	//don't have to do anything- action already written

	return inDirtyState;
}
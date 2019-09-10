#include "ClientReplicationManager.h"

void ClientReplicationManager::Read(InputMemoryBitStream& inInputStream)
{
	while (inInputStream.GetRemainingBitCount() >= 32)
	{
		//first read the network id
		int networkId;
		inInputStream.Read(networkId);

		//read action
		ReplicationAction action;
		inInputStream.Read(action, 2);

		//read dirty state
		uint32_t dirtyState;
		inInputStream.Read(dirtyState);

		switch (action)
		{
		case RA_Create:
			ReadAndDoCreateAction(inInputStream, networkId, dirtyState);
			break;
		case RA_Update:
			ReadAndDoUpdateAction(inInputStream, networkId, dirtyState);
			break;
		case RA_Destroy:
			ReadAndDoDestroyAction(inInputStream, networkId, dirtyState);
			break;
		}

	}

}

void ClientReplicationManager::ReadAndDoCreateAction(InputMemoryBitStream& inInputStream, int inNetworkId, uint32_t dirtyState)
{
	//need 4 cc
	uint32_t fourCCName;
	inInputStream.Read(fourCCName);

	//we might already have this object- could happen if our ack of the create got dropped so server resends create request 
	//( even though we might have created )
	NetworkGameObjectPtr gameObject = NetworkLinkingContext::GetGameObject(inNetworkId);
	if (!gameObject)
	{
		//create the object and map it...
		gameObject = NetworkGameObjectRegister::CreateGameObject(fourCCName);
		gameObject->SetNetworkId(inNetworkId);
		NetworkLinkingContext::AddToNetworkIdToGameObjectMap(gameObject);

	}

	//and read state
	gameObject->OnNetworkRead(inInputStream, dirtyState);
}

void ClientReplicationManager::ReadAndDoUpdateAction(InputMemoryBitStream& inInputStream, int inNetworkId, uint32_t dirtyState)
{
	//need object
	NetworkGameObjectPtr gameObject = NetworkLinkingContext::GetGameObject(inNetworkId);

	//gameObject MUST be found, because create was ack'd if we're getting an update...
	//and read state
	gameObject->OnNetworkRead(inInputStream, dirtyState);
}

void ClientReplicationManager::ReadAndDoDestroyAction(InputMemoryBitStream& inInputStream, int inNetworkId, uint32_t dirtyState)
{
	//if something was destroyed before the create went through, we'll never get it
	//but we might get the destroy request, so be tolerant of being asked to destroy something that wasn't created
	NetworkGameObjectPtr gameObject = NetworkLinkingContext::GetGameObject(inNetworkId);
	if (gameObject)
	{
		gameObject->OnNetworkDestroy();
		NetworkLinkingContext::RemoveFromNetworkIdToGameObjectMap(gameObject);
	}
}
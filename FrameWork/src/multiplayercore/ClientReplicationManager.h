#ifndef CLIENTREPLICATIONMANAGER_H
#define CLIENTREPLICATIONMANAGER_H

#include "MemoryBitStream.h"
#include "NetworkGameObject.h"
#include "ReplicationCommand.h"
#include "NetworkGameObjectRegister.h"
#include "NetworkLinkingContext.h"

class ClientReplicationManager
{
public:
	void Read(InputMemoryBitStream& inInputStream);

private:

	void ReadAndDoCreateAction(InputMemoryBitStream& inInputStream, int inNetworkId, uint32_t dirtyState);
	void ReadAndDoUpdateAction(InputMemoryBitStream& inInputStream, int inNetworkId, uint32_t dirtyState);
	void ReadAndDoDestroyAction(InputMemoryBitStream& inInputStream, int inNetworkId, uint32_t dirtyState);

};

#endif
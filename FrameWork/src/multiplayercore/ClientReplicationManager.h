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

	void ReadAndDoCreateAction(InputMemoryBitStream& inInputStream, int inNetworkId);
	void ReadAndDoUpdateAction(InputMemoryBitStream& inInputStream, int inNetworkId);
	void ReadAndDoDestroyAction(InputMemoryBitStream& inInputStream, int inNetworkId);

};

#endif
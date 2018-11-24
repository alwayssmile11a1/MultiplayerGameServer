#ifndef NETWORKGAMEOBJECT_H
#define NETWORKGAMEOBJECT_H

#include<memory>
#include "GameObject.h"
#include "../multiplayercore/MemoryBitStream.h"
#include "SocketInclude.h"

#define CLASS_IDENTIFICATION( inId, inClass ) \
virtual uint32_t GetClassId() const { return inId; } \
static NetworkGameObject* CreateInstance() { return static_cast< NetworkGameObject* >( new inClass() ); } \

class NetworkGameObject: public GameObject
{
public:

	CLASS_IDENTIFICATION('GOBJ', GameObject)

	NetworkGameObject();
	virtual ~NetworkGameObject();

	int GetNetworkId() const;
	void SetNetworkId(int inNetworkId);

	virtual uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const = 0;
	virtual void Read(InputMemoryBitStream& inInputStream) = 0;

private:

	int	mNetworkId;

};

typedef std::shared_ptr< NetworkGameObject > NetworkGameObjectPtr;

#endif
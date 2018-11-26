#ifndef NETWORKGAMEOBJECT_H
#define NETWORKGAMEOBJECT_H

#include<memory>
#include "GameObject.h"
#include "../multiplayercore/MemoryBitStream.h"
#include "SocketInclude.h"
#include "../drawable/SpriteBatch.h"

#define CLASS_IDENTIFICATION( inId, inClass ) \
static uint32_t GetClassId() { return inId; } \
static NetworkGameObjectPtr CreateInstance() {  return NetworkGameObjectPtr( new inClass());  } \

class NetworkGameObject: public GameObject
{
public:

	NetworkGameObject();
	virtual ~NetworkGameObject();

	int GetNetworkId() const;
	void SetNetworkId(int inNetworkId);

	virtual uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const = 0;
	virtual void Read(InputMemoryBitStream& inInputStream) = 0;
	virtual void Render(SpriteBatch *batch) = 0;
	virtual void Update(float dt) = 0;

private:

	int	mNetworkId;

};

typedef std::shared_ptr< NetworkGameObject > NetworkGameObjectPtr;

#endif
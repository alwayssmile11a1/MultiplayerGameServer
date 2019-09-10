#ifndef NETWORKGAMEOBJECT_H
#define NETWORKGAMEOBJECT_H

#include<memory>
#include "GameObject.h"
#include "../multiplayercore/MemoryBitStream.h"
#include "SocketInclude.h"
#include "../drawable/SpriteBatch.h"

//just a quick macro to define this class 
#define CLASS_IDENTIFICATION( inId, inClass) \
virtual uint32_t GetClassId() const { return inId; } \
static uint32_t GetId() { return inId; } \
static NetworkGameObjectPtr CreateInstance() {  return NetworkGameObjectPtr( new inClass());  } \

class NetworkGameObject: public GameObject
{
public:
	NetworkGameObject();
	virtual ~NetworkGameObject();

	int GetNetworkId() const;
	void SetNetworkId(int inNetworkId);

	virtual uint32_t GetAllStateMask() const { return 0; };
	virtual uint32_t GetClassId() const = 0;
	virtual void Render(SpriteBatch *batch) {};
	virtual void Update(float dt) {};
	
	virtual uint32_t OnNetworkWrite(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const { return uint32_t(); };
	virtual void OnNetworkRead(InputMemoryBitStream& inInputStream, uint32_t inDirtyState) {};
	virtual void OnNetworkDestroy() {};

private:

	int	mNetworkId;

};

typedef std::shared_ptr< NetworkGameObject > NetworkGameObjectPtr;

#endif
#ifndef NETWORKGAMEOBJECTREGISTER_H
#define NETWORKGAMEOBJECTREGISTER_H
#include "SocketInclude.h"
#include "NetworkGameObject.h"
#include<stdint.h>
#include <unordered_map>
#include <memory>

typedef NetworkGameObjectPtr(*GameObjectCreationFunc)();

class NetworkGameObjectRegister
{
public:

	static void RegisterCreationFunction(uint32_t inFourCCName, GameObjectCreationFunc inCreationFunction);

	static NetworkGameObjectPtr CreateGameObject(uint32_t inFourCCName);

private:

	static std::unordered_map< uint32_t, GameObjectCreationFunc > mNameToGameObjectCreationFunctionMap;

};

#endif
#include "NetworkGameObjectRegister.h"

std::unordered_map< uint32_t, GameObjectCreationFunc > NetworkGameObjectRegister::mNameToGameObjectCreationFunctionMap;

void NetworkGameObjectRegister::RegisterCreationFunction(uint32_t inFourCCName, GameObjectCreationFunc inCreationFunction)
{
	mNameToGameObjectCreationFunctionMap[inFourCCName] = inCreationFunction;
}

NetworkGameObjectPtr NetworkGameObjectRegister::CreateGameObject(uint32_t inFourCCName)
{
	//no error checking- if the name isn't there, exception!
	GameObjectCreationFunc creationFunc = mNameToGameObjectCreationFunctionMap[inFourCCName];

	NetworkGameObjectPtr gameObject = creationFunc();

	return gameObject;
}
#include "NetworkGameObject.h"

NetworkGameObject::NetworkGameObject()
{
}

NetworkGameObject::~NetworkGameObject()
{
}

int NetworkGameObject::GetNetworkId() const
{
	return mNetworkId;
}
void NetworkGameObject::SetNetworkId(int inNetworkId)
{
	mNetworkId = inNetworkId;
}
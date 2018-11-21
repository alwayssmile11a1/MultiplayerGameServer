#include "ClientNetworkManager.h"

ClientNetworkManager::ClientNetworkManager()
{
}

ClientNetworkManager::~ClientNetworkManager()
{
}

void ClientNetworkManager::Init(const std::string &destination, const std::string &playerName)
{
	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(destination);



}

void ClientNetworkManager::SendOutgoingPackets()
{

}

void ClientNetworkManager::OnPacketReceived(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress)
{

}
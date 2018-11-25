#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
{
	mState = NetworkServerState::Uninitialized;
	mNewPlayerId = 1;
}

ServerNetworkManager::~ServerNetworkManager()
{
}

void ServerNetworkManager::Init(uint16_t inPort)
{
	NetworkManager::InitUDPSocket(inPort);
	mState = NetworkServerState::GoodToGo;
}

void ServerNetworkManager::OnSendPackets()
{
	switch (mState)
	{
	case Uninitialized:

		break;
	case GoodToGo:
		SendPackets();
		break;
	}
}


void ServerNetworkManager::SendPackets()
{
}

void ServerNetworkManager::OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	uint32_t packetType;
	inputMemoryStream.Read(packetType);

	//this is a hello packet, means a new player
	if (packetType == kHelloCC)
	{
		HandleHelloPacket(inputMemoryStream, fromAddress);
	}
	else
	{
		HandleGamePacket(inputMemoryStream, fromAddress);
	}

}

void ServerNetworkManager::HandleHelloPacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	//since hello packet can be sent multiple time, check to be sure we haven't received one from this address
	auto it = mSocketAddressToClientMap.find(fromAddress);

	//don't find one, create welcome packet and send back to client
	if (it == mSocketAddressToClientMap.end())
	{
		//read the name
		std::string playerName;
		inputMemoryStream.Read(playerName);
		Debug::Log("Received from %s\n", playerName);

		//create a new client proxy to store necessary info about new client
		ClientProxyPtr newClientProxy = std::make_shared< ClientProxy >(mNewPlayerId, playerName);
		mSocketAddressToClientMap[fromAddress] = newClientProxy;

		//write welcome packet
		OutputMemoryBitStream welcomePacket;
		welcomePacket.Write(kWelcomeCC);
		welcomePacket.Write(mNewPlayerId);
		//send to client
		SendPacket(welcomePacket, fromAddress);

		mNewPlayerId++;
	}
}

void ServerNetworkManager::HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{

}
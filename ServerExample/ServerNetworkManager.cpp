#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
{
	mState = NetworkServerState::Uninitialized;

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

void ServerNetworkManager::SendWelcomePacket()
{
	
}

void ServerNetworkManager::SendPackets()
{
	
}

void ServerNetworkManager::OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	//read the beginning- is it a hello?
	uint32_t packetType;
	inputMemoryStream.Read(packetType);
	if (packetType == kHelloCC)
	{
		Debug::Log("Received from you\n");

		//read the name
		std::string name;
		inputMemoryStream.Read(name);

		//send welcome packet
		OutputMemoryBitStream welcomePacket;
		welcomePacket.Write(kWelcomeCC);
		SendPacket(welcomePacket, fromAddress);
	}
	else
	{

	}

}
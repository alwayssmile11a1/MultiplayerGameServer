#include "ClientNetworkManager.h"

ClientNetworkManager::ClientNetworkManager()
{
	mState = NetworkClientState::Uninitialized;
	mTimeOfLastHello = Time::GetTime();
	mTimeOfLastGamePacket = Time::GetTime();

}

ClientNetworkManager::~ClientNetworkManager()
{
}

void ClientNetworkManager::Init(const std::string &destination, const std::string &playerName)
{
	mPlayerName = playerName;
	mDestinationAddress = *SocketAddressFactory::CreateIPv4FromString(destination);
	
	//Init in port 0
	NetworkManager::InitUDPSocket(0);

	//init done, now prepare to send hello packet
	mState = NetworkClientState::SayingHello;
}

void ClientNetworkManager::OnSendPackets()
{
	switch (mState)
	{
	case Uninitialized:
		
		break;
	case SayingHello:
		SendHelloPacket();
		break;
	case Welcomed:
		SendGamePackets();
		break;
	}
}

void ClientNetworkManager::SendHelloPacket()
{
	float currentTime = Time::GetTime();

	if (currentTime > mTimeOfLastHello + kTimeBetweenSendingHelloPacket)
	{
		OutputMemoryBitStream helloPacket;
		helloPacket.Write(kHelloCC);
		helloPacket.Write(mPlayerName);

		SendPacket(helloPacket, mDestinationAddress);

		mTimeOfLastHello = currentTime;
	}
}

void ClientNetworkManager::SendGamePackets()
{
	float currentTime = Time::GetTime();

	if (currentTime > mTimeOfLastGamePacket + kTimeBetweenSendingGamePacket)
	{
		//just send something here

		mTimeOfLastHello = currentTime;
	}
}

void ClientNetworkManager::OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	uint32_t	packetType;
	inputMemoryStream.Read(packetType);
	switch (packetType)
	{
	case kWelcomeCC:
		DebugLog("Thanks");
		break;
	case kStateCC:
		
		break;
	}
}
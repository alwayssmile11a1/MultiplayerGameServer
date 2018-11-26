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

	//Init Register 
	NetworkGameObjectRegister::RegisterCreationFunction(Cat::GetClassId(), Cat::CreateInstance);

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
	uint32_t packetType;
	inputMemoryStream.Read(packetType);
	switch (packetType)
	{
	case kWelcomeCC:
		HandleWelcomePacket(inputMemoryStream, fromAddress);
		break;
	case kStateCC:
		HandleGamePacket(inputMemoryStream, fromAddress);
		break;
	}
}

void ClientNetworkManager::HandleWelcomePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	//Read playerId
	inputMemoryStream.Read(mPlayerId);
	Debug::Log("MyPlayerId is: %d", mPlayerId);

	//Change state to welcome
	mState = Welcomed;
}

void ClientNetworkManager::HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	std::string a;
	inputMemoryStream.Read(a);

	if (a == "Create")
	{
		//read networkId
		int networkId;
		inputMemoryStream.Read(networkId);
		//read classId
		int classId;
		inputMemoryStream.Read(classId);
		//create new network game object from classId
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject(classId);
		gameObject->SetNetworkId(networkId);

		//Add to map
		AddToNetworkIdToGameObjectMap(gameObject);
	}
	else
	{

	}
}

void ClientNetworkManager::Update(float dt)
{
	for (auto pair : networkIdToGameObjectMap)
	{
		pair.second->Update(dt);
	}
}

void ClientNetworkManager::Render(SpriteBatch* spriteBatch)
{
	for (auto pair : networkIdToGameObjectMap)
	{
		pair.second->Render(spriteBatch);
	}
}
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
	NetworkGameObjectRegister::RegisterCreationFunction(Player::GetId(), Player::CreateInstance);

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
		helloPacket.Write(PacketType::PT_Hello, 2); //only need 2 bits
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
	//Read packet type
	PacketType packetType;
	inputMemoryStream.Read(packetType, 2);

	switch (packetType)
	{
	case PacketType::PT_Welcome:
		HandleWelcomePacket(inputMemoryStream, fromAddress);
		break;
	case PacketType::PT_State:
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
	//read networkId
	int networkId;
	inputMemoryStream.Read(networkId);

	//read action
	ReplicationAction action;
	inputMemoryStream.Read(action, 2);

	switch (action)
	{
		case ReplicationAction::RA_Create:
		{
			//read classId
			uint32_t classId;
			inputMemoryStream.Read(classId);
			//create new network game object from classId
			NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject(classId);
			gameObject->SetNetworkId(networkId);

			//Add to map
			AddToNetworkIdToGameObjectMap(gameObject);
			break;
		}
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
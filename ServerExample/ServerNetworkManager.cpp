#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
{
	mNewPlayerId = 1;
	mNetworkId = 1;
}

ServerNetworkManager::~ServerNetworkManager()
{
}

void ServerNetworkManager::Init(uint16_t inPort)
{
	NetworkManager::InitUDPSocket(inPort);

	//Register function
	NetworkGameObjectRegister::RegisterCreationFunction(Player::GetId(), Player::CreateInstance);
}

void ServerNetworkManager::OnSendPackets()
{
	for (const auto& pair : mSocketAddressToClientMap)
	{
		//first write packet type
		OutputMemoryBitStream packet;
		packet.Write(PacketType::PT_State, 2);

		//then write world state
		pair.second->GetServerReplicationManager().Write(packet);

		//Send ..
		SendPacket(packet, pair.first);
	}
}


void ServerNetworkManager::OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	uint8_t packetType;
	inputMemoryStream.Read(packetType, 2);
	//this is a hello packet, means a new player
	switch (packetType)
	{
		case PacketType::PT_Hello:
		{
			HandleHelloPacket(inputMemoryStream, fromAddress);
			break;
		}
		case PacketType::PT_State:
		{
			HandleGamePacket(inputMemoryStream, fromAddress);
			break;
		}
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
		Debug::Log("Received from %s\n", playerName.c_str());

		//create a new client proxy to store necessary info about new client
		ClientProxyPtr newClientProxy = std::make_shared< ClientProxy >(mNewPlayerId, playerName);
		mSocketAddressToClientMap[fromAddress] = newClientProxy;
		mPlayerIdToClientMap[mNewPlayerId] = newClientProxy;

		//Tell this player to create current world state
		for (const auto& pair : networkIdToGameObjectMap)
		{
			newClientProxy->GetServerReplicationManager().ReplicateCreate(pair.second, pair.second->GetAllStateMask());
		}

		//create new player
		CreateNewPlayer(mNewPlayerId);

		//increase playerId
		mNewPlayerId++;

		//And finally write welcome packet
		OutputMemoryBitStream welcomePacket;
		welcomePacket.Write(PacketType::PT_Welcome, 2);
		welcomePacket.Write(mNewPlayerId);
		//send to client
		SendPacket(welcomePacket, fromAddress);
	}
}

void ServerNetworkManager::CreateNewPlayer(int playerId)
{
	//Create player
	NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('PL');
	Player* player = (Player*)gameObject.get();
	player->SetPlayerId(playerId);
	//Register this player
	RegisterGameObject(gameObject);
}

void ServerNetworkManager::HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{

}

void ServerNetworkManager::OnConnectionReset(const SocketAddress& inFromAddress)
{
	
}

void ServerNetworkManager::Update(float dt)
{
	for (auto pair : networkIdToGameObjectMap)
	{
		pair.second->Update(dt);
	}
}

int ServerNetworkManager::GetNewNetworkId()
{
	return mNetworkId++;
}

void ServerNetworkManager::RegisterGameObject(NetworkGameObjectPtr inGameObject)
{
	//assign network id
	int newNetworkId = GetNewNetworkId();
	inGameObject->SetNetworkId(newNetworkId);

	//add mapping from network id to game object
	AddToNetworkIdToGameObjectMap(inGameObject);

	//tell all players this is new...
	for (const auto& pair : mPlayerIdToClientMap)
	{
		pair.second->GetServerReplicationManager().ReplicateCreate(inGameObject, inGameObject->GetAllStateMask());
	}
}


void ServerNetworkManager::UnregisterGameObject(NetworkGameObjectPtr inGameObject)
{
	int networkId = inGameObject->GetNetworkId();
	networkIdToGameObjectMap.erase(networkId);

	//tell all players to remove this
	for (const auto& pair : mPlayerIdToClientMap)
	{
		pair.second->GetServerReplicationManager().ReplicateDestroy(inGameObject);
	}
}
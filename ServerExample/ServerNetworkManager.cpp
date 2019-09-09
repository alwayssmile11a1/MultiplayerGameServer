#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
{
	mNewPlayerId = 1;
	mNetworkId = 1;
	mNewSpawnPosition.Set(-350, 0);
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
	for (const auto& pair : mSocketAddressToClientProxyMap)
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
	PacketType packetType;
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
	auto it = mSocketAddressToClientProxyMap.find(fromAddress);

	//don't find one, create welcome packet and send back to client
	if (it == mSocketAddressToClientProxyMap.end())
	{
		//read the name
		std::string playerName;
		inputMemoryStream.Read(playerName);
		Debug::Log("Received from %s\n", playerName.c_str());

		//create a new client proxy to store necessary info about new client
		ClientProxyPtr newClientProxy = std::make_shared< ClientProxy >(mNewPlayerId, playerName);
		mSocketAddressToClientProxyMap[fromAddress] = newClientProxy;
		mPlayerIdToClientProxyMap[mNewPlayerId] = newClientProxy;

		//Tell this client to create current world state
		for (const auto& pair : NetworkLinkingContext::GetNetworkIdToGameObjectMap())
		{
			newClientProxy->GetServerReplicationManager().ReplicateCreate(pair.second, pair.second->GetAllStateMask());
		}

		//create new player
		CreateNewPlayer(newClientProxy);

		//And finally write welcome packet
		OutputMemoryBitStream welcomePacket;
		welcomePacket.Write(PacketType::PT_Welcome, 2);
		//write player id
		welcomePacket.Write(mNewPlayerId);
		//write player object id
		welcomePacket.Write(newClientProxy->GetClientObject()->GetNetworkId());
		//send to client
		SendPacket(welcomePacket, fromAddress);

		//increase playerId
		mNewPlayerId++;
	}
}

const Vector2& ServerNetworkManager::GetNewSpawnPosition()
{
	mNewSpawnPosition.Set(mNewSpawnPosition.x + 150, mNewSpawnPosition.y);
	return mNewSpawnPosition;
}

void ServerNetworkManager::CreateNewPlayer(ClientProxyPtr clientProxy)
{
	//Create player
	NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('PL');
	Player* player = (Player*)gameObject.get();
	player->SetPlayerId(clientProxy->GetPlayerId());
	Vector2 newPosition = GetNewSpawnPosition();
	player->SetPosition(newPosition);

	clientProxy->SetClientObject(gameObject);

	//Register this player
	RegisterGameObject(gameObject);
}

void ServerNetworkManager::HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	//tell all players to remove this
	auto it = mSocketAddressToClientProxyMap.find(fromAddress);
	//fromClient existed
	if (it != mSocketAddressToClientProxyMap.end())
	{
		NetworkGameObjectPtr fromObject = it->second->GetClientObject();
		for (const auto& pair : mPlayerIdToClientProxyMap)
		{
			//tell all clients except the fromAddress one. 
			if (fromObject != pair.second->GetClientObject())
			{
				pair.second->GetServerReplicationManager().AddDirtyState(pair.second->GetClientObject(), 1);
			}
		}
	}
}

void ServerNetworkManager::OnConnectionReset(const SocketAddress& inFromAddress)
{
	//Remove this client immediately for now
	auto it = mSocketAddressToClientProxyMap.find(inFromAddress);
	if (it != mSocketAddressToClientProxyMap.end())
	{
		NetworkGameObjectPtr gameObject = it->second->GetClientObject();

		//do something to remove player
	  	Player* player = (Player*)(gameObject.get());

		//remove from map
		mSocketAddressToClientProxyMap.erase(inFromAddress);
		mPlayerIdToClientProxyMap.erase(player->GetPlayerId());

		//unregister
		UnregisterGameObject(gameObject);
	}

}

void ServerNetworkManager::Update(float dt)
{
	for (const auto& pair : NetworkLinkingContext::GetNetworkIdToGameObjectMap())
	{
		pair.second->Update(dt);
	}
}

int ServerNetworkManager::GetNewNetworkId()
{
	return mNetworkId++;
}

//this should be put somewhere inside Framework rather than here
void ServerNetworkManager::RegisterGameObject(NetworkGameObjectPtr inGameObject)
{
	//assign network id
	int newNetworkId = GetNewNetworkId();
	inGameObject->SetNetworkId(newNetworkId);

	//add mapping from network id to game object
	NetworkLinkingContext::AddToNetworkIdToGameObjectMap(inGameObject);

	//tell all players this is new...
	for (const auto& pair : mPlayerIdToClientProxyMap)
	{
		pair.second->GetServerReplicationManager().ReplicateCreate(inGameObject, inGameObject->GetAllStateMask());
	}
}

//this should be put somewhere inside Framework rather than here
void ServerNetworkManager::UnregisterGameObject(NetworkGameObjectPtr inGameObject)
{
	//int networkId = inGameObject->GetNetworkId();
	NetworkLinkingContext::RemoveFromNetworkIdToGameObjectMap(inGameObject);

	//tell all players to remove this
	for (const auto& pair : mPlayerIdToClientProxyMap)
	{
		pair.second->GetServerReplicationManager().ReplicateDestroy(inGameObject);
	}
}
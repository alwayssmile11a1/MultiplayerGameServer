#include "ServerNetworkManager.h"

ServerNetworkManager* ServerNetworkManager::Instance;

ServerNetworkManager::ServerNetworkManager()
{
	mNewPlayerId = 1;
	mNetworkId = 1;
	mNewSpawnPosition.Set(-350, 0);
	Instance = this;
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

		//write last action timestamp
		packet.Write(pair.second->GetPlayerActions().GetLastActionTimeStamp());

		//then write world replication state
		pair.second->GetServerReplicationManager().Write(packet);

		//Send packet
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
		case PacketType::PT_Input:
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
	//find fromObject
	auto it = mSocketAddressToClientProxyMap.find(fromAddress);

	//fromClient existed
	if (it != mSocketAddressToClientProxyMap.end())
	{
		//Get client proxy
		ClientProxyPtr fromClientProxy = it->second;

		//Handle PlayerActions
		uint32_t actionCount = 0;
		inputMemoryStream.Read(actionCount, 2);

		//Add playeractions to list
		for (; actionCount > 0; --actionCount)
		{
			PlayerAction playerAction;
			playerAction.OnNetworkRead(inputMemoryStream);
			fromClientProxy->GetPlayerActions().AddPlayerAction(playerAction);
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

ClientProxyPtr ServerNetworkManager::GetClientProxy(int inPlayerId) const
{
	//find fromObject
	auto it = mPlayerIdToClientProxyMap.find(inPlayerId);

	if (it != mPlayerIdToClientProxyMap.end())
	{
		return it->second;
	}
	
	return nullptr;
}

void ServerNetworkManager::SetStateDirty(int networkId, uint32_t dirtyState)
{
	//tell everybody this is dirty
	NetworkGameObjectPtr networkGameObject = NetworkLinkingContext::GetGameObject(networkId);
	for (const auto& pair : mPlayerIdToClientProxyMap)
	{
		pair.second->GetServerReplicationManager().AddDirtyState(NetworkLinkingContext::GetGameObject(networkId), dirtyState);
	}
}
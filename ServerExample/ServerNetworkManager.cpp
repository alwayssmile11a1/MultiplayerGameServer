#include "ServerNetworkManager.h"

ServerNetworkManager* ServerNetworkManager::Instance;

ServerNetworkManager::ServerNetworkManager()
{
	mNewPlayerId = 1;
	mNetworkId = 1;
	mNewSpawnPositionIndex = 0;
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
	NetworkGameObjectRegister::RegisterCreationFunction(Brick::GetId(), Brick::CreateInstance);

	//Setup map
	mapLoader.AddMap("map1", "../Resources/battlecitymap.tmx", 3.1);
	map = mapLoader.GetMap("map1");

	//create brick
	std::vector<Shape::Rectangle> brickRects = map->GetObjectGroup("Brick")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = brickRects.begin(); rect != brickRects.end(); ++rect)
	{
		//Create brick
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('BR');
		Brick* brick = (Brick*)gameObject.get();
		brick->SetPosition(Vector2(rect->x, rect->y));

		//Register this brick
		RegisterGameObject(gameObject);
	}
}

void ServerNetworkManager::OnSendPackets()
{
	for (const auto& pair : mSocketAddressToClientProxyMap)
	{
		//first write packet type
		OutputMemoryBitStream packet;
		packet.Write(PacketType::PT_State, 2);

		//write last action timestamp
		packet.Write(pair.second->IsLastActionTimeStampDirty());
		if (pair.second->IsLastActionTimeStampDirty())
		{
			packet.Write(pair.second->GetPlayerActions().GetLastActionTimeStamp());
			pair.second->SetLastActionTimeStampDirty(false);
		}

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
	float x = map->GetObjectGroup("TankSpawnPosition")->GetRects()[mNewSpawnPositionIndex].x;
	float y = map->GetObjectGroup("TankSpawnPosition")->GetRects()[mNewSpawnPositionIndex].y;

	mNewSpawnPositionIndex = (mNewSpawnPositionIndex+1)%(map->GetObjectGroup("TankSpawnPosition")->GetRects().size());

	return Vector2(x, y);
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

			if (fromClientProxy->GetPlayerActions().AddPlayerAction(playerAction))
			{
				fromClientProxy->SetLastActionTimeStampDirty(true);
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
	inGameObject->OnNetworkDestroy();

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

void ServerNetworkManager::UpdateNetworkGameObject(int networkId, uint32_t dirtyState)
{
	//tell everybody this is dirty
	NetworkGameObjectPtr networkGameObject = NetworkLinkingContext::GetGameObject(networkId);
	for (const auto& pair : mPlayerIdToClientProxyMap)
	{
		pair.second->GetServerReplicationManager().AddDirtyState(NetworkLinkingContext::GetGameObject(networkId), dirtyState);
	}
}

void ServerNetworkManager::DestroyNetworkGameObject(int networkId)
{
	UnregisterGameObject(NetworkLinkingContext::GetGameObject(networkId));
}
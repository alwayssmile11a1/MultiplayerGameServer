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
	NetworkGameObjectRegister::RegisterCreationFunction(Metal::GetId(), Metal::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Bound::GetId(), Bound::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Bullet::GetId(), Bullet::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Enemy::GetId(), Enemy::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(StarItem::GetId(), StarItem::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(ShieldItem::GetId(), ShieldItem::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Grass::GetId(), Grass::CreateInstance);

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

	//create metal
	std::vector<Shape::Rectangle> metalRects = map->GetObjectGroup("Metal")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = metalRects.begin(); rect != metalRects.end(); ++rect)
	{
		//Create metal
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('ME');
		Metal* metal = (Metal*)gameObject.get();
		metal->SetPosition(Vector2(rect->x, rect->y));

		//Register this metal
		RegisterGameObject(gameObject);
	}

	//create grass
	std::vector<Shape::Rectangle> grassRects = map->GetObjectGroup("Grass")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = grassRects.begin(); rect != grassRects.end(); ++rect)
	{
		//Create grass
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('GR');
		Grass* grass = (Grass*)gameObject.get();
		grass->SetPosition(Vector2(rect->x, rect->y));

		//Register this grass
		RegisterGameObject(gameObject);
	}

	//create shield
	std::vector<Shape::Rectangle> shieldRects = map->GetObjectGroup("Shield")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = shieldRects.begin(); rect != shieldRects.end(); ++rect)
	{
		//Create grass
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('SI');
		ShieldItem* shield = (ShieldItem*)gameObject.get();
		shield->SetPosition(Vector2(rect->x, rect->y));

		//Register this grass
		RegisterGameObject(gameObject);
	}

	//create star
	std::vector<Shape::Rectangle> starRects = map->GetObjectGroup("Star")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = starRects.begin(); rect != starRects.end(); ++rect)
	{
		//Create grass
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('IS');
		StarItem* star = (StarItem*)gameObject.get();
		star->SetPosition(Vector2(rect->x, rect->y));

		//Register this grass
		RegisterGameObject(gameObject);
	}

	//create enemy
	std::vector<Shape::Rectangle> enemyRects = map->GetObjectGroup("Enemy")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = enemyRects.begin(); rect != enemyRects.end(); ++rect)
	{
		//Create grass
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('EN');
		Enemy* enemy = (Enemy*)gameObject.get();
		enemy->SetPosition(Vector2(rect->x, rect->y));

		//Register this grass
		RegisterGameObject(gameObject);
	}

	//create bound
	std::vector<Shape::Rectangle> boundRects = map->GetObjectGroup("Bound")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator rect = boundRects.begin(); rect != boundRects.end(); ++rect)
	{
		//Create bound
		NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('BO');
		Bound* bound = (Bound*)gameObject.get();
		bound->SetPosition(Vector2(rect->x, rect->y));
		bound->SetBodySize(rect->width, rect->height);

		//Register this bound
		RegisterGameObject(gameObject);
	}
}

void ServerNetworkManager::OnSendPackets()
{
	bool isAllPlayerReady = true;
	for (const auto& pair : mSocketAddressToClientProxyMap)
	{
		if (!pair.second->GetPlayerActions().GetPlayerReady())
		{
			isAllPlayerReady = false;
			break;
		}
	}

	int teamWon = -1;
	if (isAllPlayerReady)
	{
		for (const auto& player : Player::mPlayers)
		{
			if (teamWon == -1)
			{
				teamWon = player->GetTeamNumber();
			}
			else
			{
				//there is still another team
				if (teamWon != player->GetTeamNumber())
				{
					teamWon = -1;
					break;
				}
			}
		}
	}

	for (const auto& pair : mSocketAddressToClientProxyMap)
	{
		//first write packet type
		OutputMemoryBitStream packet;
		packet.Write(PacketType::PT_State, 2);

		//write IsAllPlayerReady 
		packet.Write(isAllPlayerReady);

		//write IsGameEnded
		if (teamWon > -1)
		{
			packet.Write(true);
			packet.Write(teamWon);
		}
		else
		{
			packet.Write(false);
		}

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

void ServerNetworkManager::CreateBullet(int playerNetworkGameObjectId, Body* playerBody, int playerRotation)
{
	//Create bullet
	NetworkGameObjectPtr gameObject = NetworkGameObjectRegister::CreateGameObject('BU');
	Bullet* bullet = (Bullet*)gameObject.get();

	Vector2 velocity;

	switch (playerRotation)
	{
		case 0:
		{
			velocity.Set(0, bullet->GetSpeed());
			break;
		}
		case 180:
		{
			velocity.Set(0, -bullet->GetSpeed());
			break;
		}
		case 90:
		{
			velocity.Set(bullet->GetSpeed(), 0);
			break;
		}
		case -90:
		{
			velocity.Set(-bullet->GetSpeed(), 0);
			break;
		}
	}

	bullet->SetVelocity(velocity);

	//Do this so bullet don't collide with the player that created it
	Vector2 position = playerBody->GetPosition();
	position.x += velocity.x * 1.5;
	position.y += velocity.y * 1.5;
	bullet->SetPosition(position);

	bullet->SetIgnoredCollisionBody(playerBody);
	bullet->SetIgnoredNetworkGameObjectID(playerNetworkGameObjectId);

	//Register this bullet
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

		//Read ready state
		bool isPlayerReady;
		inputMemoryStream.Read(isPlayerReady);
		fromClientProxy->GetPlayerActions().SetPlayerReady(isPlayerReady);

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

	for (const auto& id : mDestroyList)
	{
		UnregisterGameObject(NetworkLinkingContext::GetGameObject(id));
	}
	mDestroyList.clear();
}

void ServerNetworkManager::Render(SpriteBatch* spriteBatch)
{
	for (const auto& pair : NetworkLinkingContext::GetNetworkIdToGameObjectMap())
	{
		pair.second->Render(spriteBatch);
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
	if (inGameObject == nullptr) return;

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
	mDestroyList.push_back(networkId);
}

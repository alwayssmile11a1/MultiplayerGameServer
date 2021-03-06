#pragma once

#include "ClientProxy.h"
#include "Player.h"
#include "Brick.h"
#include "Metal.h"
#include "Grass.h"
#include "Bound.h"
#include "Bullet.h"
#include "Enemy.h"
#include "StarItem.h"
#include "ShieldItem.h"
#include "HanabiMath.h"
#include "HanabiConsole.h"
#include "HanabiMap.h"

class ServerNetworkManager : public NetworkManager
{
private:
	int mNewPlayerId;
	int mNetworkId;
	int mNewSpawnPositionIndex;

	std::unordered_map<SocketAddress, ClientProxyPtr> mSocketAddressToClientProxyMap;
	std::unordered_map<int, ClientProxyPtr> mPlayerIdToClientProxyMap;

	std::vector<int> mDestroyList;

	TMXLoader mapLoader;
	TMXMap* map;

	void HandleHelloPacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);
	void HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);

	void OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress) override;
	void OnSendPackets() override;
	void OnConnectionReset(const SocketAddress& inFromAddress) override;

	int GetNewNetworkId();
	void RegisterGameObject(NetworkGameObjectPtr inGameObject);
	void UnregisterGameObject(NetworkGameObjectPtr inGameObject);

	const Vector2& GetNewSpawnPosition();
	void CreateNewPlayer(ClientProxyPtr clientProxy);


public:
	static ServerNetworkManager* Instance;

public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init(uint16_t inPort);
	void Update(float dt);
	void Render(SpriteBatch* spriteBatch);

	ClientProxyPtr GetClientProxy(int inPlayerId) const;
	void UpdateNetworkGameObject(int networkId, uint32_t dirtyState);
	void DestroyNetworkGameObject(int networkId);
	void CreateBullet(int playerNetworkGameObjectId, Body* playerBody, int rotation);
};
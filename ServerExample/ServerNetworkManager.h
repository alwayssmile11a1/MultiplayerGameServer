#pragma once

#include "HanabiMultiplayer.h"
#include "HanabiMath.h"
#include "HanabiConsole.h"
#include "ClientProxy.h"
#include "Player.h"

class ServerNetworkManager : public NetworkManager
{
private:
	int mNewPlayerId;
	int mNetworkId;
	Vector2 mNewSpawnPosition;

	std::unordered_map<SocketAddress, ClientProxyPtr> mSocketAddressToClientMap;
	std::unordered_map<int, ClientProxyPtr> mPlayerIdToClientMap;

	void HandleHelloPacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);
	void HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress);

	void OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress) override;
	void OnSendPackets() override;
	void OnConnectionReset(const SocketAddress& inFromAddress) override;

	void Update(float dt);

	int GetNewNetworkId();
	void RegisterGameObject(NetworkGameObjectPtr inGameObject);
	void UnregisterGameObject(NetworkGameObjectPtr inGameObject);

	const Vector2& GetNewSpawnPosition();
	void CreateNewPlayer(ClientProxyPtr clientProxy);


public:
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init(uint16_t inPort);
};
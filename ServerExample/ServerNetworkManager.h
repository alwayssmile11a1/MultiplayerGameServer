#pragma once

#include "HanabiMultiplayer.h"
#include "ClientProxy.h"
#include "Player.h"
#include "HanabiMath.h"
#include "HanabiConsole.h"

class ServerNetworkManager : public NetworkManager
{
private:
	int mNewPlayerId;
	int mNetworkId;
	Vector2 mNewSpawnPosition;

	std::unordered_map<SocketAddress, ClientProxyPtr> mSocketAddressToClientProxyMap;
	std::unordered_map<int, ClientProxyPtr> mPlayerIdToClientProxyMap;

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
	ServerNetworkManager();
	~ServerNetworkManager();

	void Init(uint16_t inPort);
	void Update(float dt);
};
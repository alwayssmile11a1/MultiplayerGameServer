#include "ClientNetworkManager.h"

ClientNetworkManager* ClientNetworkManager:: Instance;

ClientNetworkManager::ClientNetworkManager()
{
	mState = NetworkClientState::Uninitialized;
	mTimeOfLastHello = Time::sInstance.GetFrameStartTime();
	mTimeOfLastGamePacket = Time::sInstance.GetFrameStartTime();
	Instance = this;
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
	NetworkGameObjectRegister::RegisterCreationFunction(Brick::GetId(), Brick::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Metal::GetId(), Metal::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Bound::GetId(), Bound::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Bullet::GetId(), Bullet::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Enemy::GetId(), Enemy::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(StarItem::GetId(), StarItem::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(ShieldItem::GetId(), ShieldItem::CreateInstance);
	NetworkGameObjectRegister::RegisterCreationFunction(Grass::GetId(), Grass::CreateInstance);

	//init done, now prepare to send hello packet
	mState = NetworkClientState::SayingHello;


	mTimeOfLastHello = Time::sInstance.GetFrameStartTime();
	mTimeOfLastGamePacket = Time::sInstance.GetFrameStartTime();

	font = Font("Arial", 10, 30);
	lobbyLabel = Label("Lobby", &font, 560 / 2, 500, 640, 480);
	readyLabel = Label("Ready", &font, 560 / 2, 200, 640, 480);
	wonLabel = Label("Team Won", &font, 560 / 2, 400, 640, 480);
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
	float currentTime = Time::sInstance.GetFrameStartTime();

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
	float currentTime = Time::sInstance.GetFrameStartTime();

	if (currentTime > mTimeOfLastGamePacket + kTimeBetweenSendingGamePacket)
	{
		//Send PlayerActions
		if (PlayerActions::GetInstance()->Count() > 0)
		{
			OutputMemoryBitStream inputPacket;

			//write packet type
			inputPacket.Write(PacketType::PT_Input, 2); //only need 2 bits

			//eventually write the latest playeractions so they have three chances to get through...
			int playerActionCount = PlayerActions::GetInstance()->Count();
			int sentCount = 3;
			int firstPlayerActionIndex = playerActionCount - sentCount;
			if (firstPlayerActionIndex < sentCount)
			{
				firstPlayerActionIndex = 0;
			}
			auto playerAction = PlayerActions::GetInstance()->begin() + firstPlayerActionIndex;

			//write isPlayerReady or not
			inputPacket.Write(PlayerActions::GetInstance()->GetPlayerReady());

			//only need two bits to write the player action count, because it's 0, 1, 2 or 3
			inputPacket.Write(playerActionCount - firstPlayerActionIndex, 2);

			for (; firstPlayerActionIndex < playerActionCount; ++firstPlayerActionIndex, ++playerAction)
			{
				playerAction->OnNetworkWrite(inputPacket);
			}

			SendPacket(inputPacket, mDestinationAddress);
		}

		mTimeOfLastGamePacket = currentTime;
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
	int playerId;
	inputMemoryStream.Read(playerId);
	Proxy::SetPlayerId(playerId);
	Debug::Log("MyPlayerId is: %d \n", playerId);

	int playerObjectId;
	inputMemoryStream.Read(playerObjectId);
	Proxy::SetPlayerNetworkGameObjectId(playerObjectId);
	Debug::Log("MyPlayerNetworkGameObjectId is: %d \n", playerObjectId);
	//Change state to welcome
	mState = Welcomed;
}

void ClientNetworkManager::HandleGamePacket(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress)
{
	ReadLastActionProcessedOnServerTimeStamp(inputMemoryStream);

	clientReplicationManager.Read(inputMemoryStream);
}

void ClientNetworkManager::ReadLastActionProcessedOnServerTimeStamp(InputMemoryBitStream& inputMemoryStream)
{
	//Read isAllPlayerReady
	inputMemoryStream.Read(mIsAllPlayerReady);

	//Read teamWon
	bool isGameEnded;
	inputMemoryStream.Read(isGameEnded);
	if (isGameEnded)
	{
		inputMemoryStream.Read(mTeamWon);
	}

	//read timeStamp
	bool isTimeStampDirty;
	inputMemoryStream.Read(isTimeStampDirty);

	if (!isTimeStampDirty) return;

	float lastActionProcessedByServerTimestamp;
	inputMemoryStream.Read(lastActionProcessedByServerTimestamp);

	//remove processed action
	PlayerActions::GetInstance()->RemovePlayerActions(lastActionProcessedByServerTimestamp);

	//Update averageRoundTripTime
	float rtt = Time::sInstance.GetFrameStartTime() - lastActionProcessedByServerTimestamp;
	mAverageRoundTripTime.Update(rtt);
	//Debug::Log("%f - %f\n", rtt, mAverageRoundTripTime.GetValue());
}


void ClientNetworkManager::Update(float dt)
{
	for (const auto& pair : NetworkLinkingContext::GetNetworkIdToGameObjectMap())
	{
		pair.second->Update(dt);
	}

	ExplosionEffectCollector::Update(dt);
}

void ClientNetworkManager::Render(SpriteBatch* spriteBatch)
{
	if (!mIsAllPlayerReady)
	{
		lobbyLabel.Draw(mCamera);
		for (const auto& pair : NetworkLinkingContext::GetNetworkIdToGameObjectMap())
		{
			Player* player = dynamic_cast<Player*>(pair.second.get());
			if (player)
			{
				player->RenderLobbyPlayer(spriteBatch);
			}
		}

		if (PlayerActions::GetInstance()->GetPlayerReady())
		{
			readyLabel.Draw(mCamera);
		}
	}
	else
	{
		for (const auto& pair : NetworkLinkingContext::GetNetworkIdToGameObjectMap())
		{
			pair.second->Render(spriteBatch);
		}

		for (const auto& lateDrawnObject : lateDrawnObjects)
		{
			Grass* grass = (Grass*)(lateDrawnObject);
			if (grass != nullptr)
			{
				grass->LateRender(spriteBatch);
			}
		}

		ExplosionEffectCollector::Render(spriteBatch);
	}

	if (mTeamWon > -1)
	{
		std::string text = "Team " + std::to_string(mTeamWon) + " Won";
		wonLabel.SetText(text);
		wonLabel.Draw(mCamera);
	}
}

void ClientNetworkManager::AddToLateDrawnObjects(NetworkGameObject* gameObjectPtr)
{
	lateDrawnObjects.push_back(gameObjectPtr);
}

void ClientNetworkManager::Release()
{
	font.Release();
}
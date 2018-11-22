#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
	
}
NetworkManager::~NetworkManager()
{
}

bool NetworkManager::InitUDPSocket(uint16_t inPort)
{
	mUDPSocket = SocketUtil::CreateUDPSocket(INET);
	SocketAddress ownAddress(INADDR_ANY, inPort);
	mUDPSocket->Bind(ownAddress);

	LOG("Initializing NetworkManager at port %d", inPort);

	//did we bind okay?
	if (mUDPSocket == nullptr)
	{
		return false;
	}

	if (mUDPSocket->SetNonBlockingMode(true) != NO_ERROR)
	{
		return false;
	}

	return true;
}

void NetworkManager::SendPacket(const OutputMemoryBitStream& outputMemoryStream, const SocketAddress& destinationAddress)
{
	int sentByteCount = mUDPSocket->SendTo(outputMemoryStream.GetBufferPtr(), outputMemoryStream.GetByteLength(), destinationAddress);
}

void NetworkManager::ReceiveIncomingPackets()
{
	ReadIncomingPacketsIntoQueue();

	ProcessQueuedPackets();
}

void NetworkManager::SendOutgoingPackets()
{
	OnSendPackets();
}


void NetworkManager::ReadIncomingPacketsIntoQueue()
{
	//should we just keep a static one?
	char packetMem[1500];
	int packetSize = sizeof(packetMem);
	InputMemoryBitStream inputStream(packetMem, packetSize * 8);
	SocketAddress fromAddress;

	//keep reading until we don't have anything to read ( or we hit a max number that we'll process per frame )
	int receivedPackedCount = 0;
	int totalReadByteCount = 0;

	while (receivedPackedCount < mMaxPacketsPerFrameCount)
	{
		int readByteCount = mUDPSocket->ReceiveFrom(packetMem, packetSize, fromAddress);
		if (readByteCount == 0)
		{
			//nothing to read
			break;
		}
		else if (readByteCount == -WSAECONNRESET)
		{
			//port closed on other end, so DC this person immediately
			OnConnectionReset(fromAddress);
		}
		else if (readByteCount > 0)
		{
			inputStream.ResetToCapacity(readByteCount);
			++receivedPackedCount;
			totalReadByteCount += readByteCount;

			////now, should we drop the packet?
			//if (GetRandomFloat() >= mDropPacketChance)
			//{
			//	//we made it
				//float simulatedReceivedTime = Timing::sInstance.GetTimef() + mSimulatedLatency;
				mReceivedPacketQueue.emplace(Time::GetCurrentTime(), inputStream, fromAddress);
			//}
			//else
			//{
			//	LOG("Dropped packet!", 0);
			//	//dropped!
			//}
		}
		else
		{
			//uhoh, error? exit or just keep going?
		}
	}

}
void NetworkManager::ProcessQueuedPackets()
{
	//look at the front packet...
	while (!mReceivedPacketQueue.empty())
	{
		ReceivedPacket& currentPacket = mReceivedPacketQueue.front();
		//if (Timing::sInstance.GetTimef() > currentPacket.GetReceivedTime())
		//{
			OnPacketReceived(currentPacket.GetPacketBuffer(), currentPacket.GetFromAddress());
			mReceivedPacketQueue.pop();
		//}
		//else
		//{
		//	break;
		//}

	}
}
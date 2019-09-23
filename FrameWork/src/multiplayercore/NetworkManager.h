#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "SocketInclude.h"
#include "MemoryBitStream.h"
#include "SocketAddress.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "NetworkHelper.h"
#include <queue>
#include <list>
#include "../src/math/Time.h"
#include <unordered_map>
#include "NetworkGameObject.h"

enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

enum PacketType : uint8_t // Use uint8_t to prevent the problem between Read and Write BitStream
{
	PT_Hello,
	PT_Welcome,
	PT_State,
	PT_Input
};

//Hold a UDPSocket (for now) and manage sending and receiving packets
class NetworkManager
{
private:

	int mMaxPacketsPerFrameCount = 10;

	//UDPSocket of current client 
	UDPSocketPtr mUDPSocket;

	int mBytesSentThisFrame;

	//Just a wrapper class for packets received from somewhere (server, other clients)
	class ReceivedPacket
	{
	public:
		ReceivedPacket(float inReceivedTime, InputMemoryBitStream& ioInputMemoryBitStream, const SocketAddress& inFromAddress) :
			mReceivedTime(inReceivedTime),
			mFromAddress(inFromAddress),
			mPacketBuffer(ioInputMemoryBitStream)
		{
		}

		const	SocketAddress&			GetFromAddress()	const { return mFromAddress; }
		float					GetReceivedTime()	const { return mReceivedTime; }
		InputMemoryBitStream&	GetPacketBuffer() { return mPacketBuffer; }

	private:

		float					mReceivedTime;
		InputMemoryBitStream	mPacketBuffer;
		SocketAddress			mFromAddress;

	};

	//A queue of packet received from somewhere (server, another client)
	std::queue< ReceivedPacket, std::list< ReceivedPacket > > mReceivedPacketQueue;

	//some helper functions 
	void ReadIncomingPacketsIntoQueue();
	void ProcessQueuedPackets();

	//the average round trip time
	float mAverageRoundTripTime = 0;

	//for debug purpose only
	float mDropPacketChance = 0;
	float mSimulatedLatency = 0;

protected:
	/*std::unordered_map< int, NetworkGameObjectPtr > networkIdToGameObjectMap;*/

	//Port closed on other end, so do something (this function might be called on server side only)
	virtual void OnConnectionReset(const SocketAddress& inFromAddress) {}
	//Call back when a packet is received
	virtual void OnPacketReceived(InputMemoryBitStream& inputMemoryStream, const SocketAddress& fromAddress) = 0;
	//Process and send neccessary packets inside this function
	virtual void OnSendPackets() = 0;

public:
	NetworkManager();
	virtual ~NetworkManager();
	
	//Init a UDP socket 
	bool InitUDPSocket(uint16_t inPort);

	//Send packet to an address
	void SendPacket(const OutputMemoryBitStream& outputMemoryStream, const SocketAddress& destinationAddress);

	//Call this function on Update or somewhere similar to receive incoming packets
	//If any packet is received, OnPacketReceived will be called
	void ReceiveIncomingPackets();

	//Cal this function on Update or somewhere similar to send outgoing packets
	void SendOutgoingPackets();

	//Default is 10 
	void SetMaxPacketsPerFrame(float inMaxPacketsPerFrame) { mMaxPacketsPerFrameCount = inMaxPacketsPerFrame;  }
	//For debug purpose only
	void SetDropPacketChance(float inChance) { mDropPacketChance = inChance; }
	//For debug purpose only
	void SetSimulatedLatency(float inLatency) { mSimulatedLatency = inLatency; }

	float GetAverageRoundTripTime() { return mAverageRoundTripTime; }

	//helper function
	static UDPSocketPtr	CreateUDPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr	CreateTCPSocket(SocketAddressFamily inFamily);

	//this function needs to be called first before doing any networking-related stuff
	static bool	StaticInit();
	//remember to call this on game end 
	static void	CleanUp();
};


#endif
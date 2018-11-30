#ifndef DELIVERYNOTIFICATIONMANAGER_H
#define DELIVERYNOTIFICATIONMANAGER_H

#include "MemoryBitStream.h"
#include "AckRange.h"
#include <deque>

class DeliveryNotificationManager
{
public:
	const float kDelayBeforeAckTimeout = 0.5f;


	DeliveryNotificationManager(bool inShouldSendAcks, bool inShouldProcessAcks);
	~DeliveryNotificationManager();

	InFlightPacket*		WriteState(OutputMemoryBitStream& inOutputStream);
	bool				ReadAndProcessState(InputMemoryBitStream& inInputStream);

	void				ProcessTimedOutPackets();

	uint32_t			GetDroppedPacketCount()		const { return mDroppedPacketCount; }
	uint32_t			GetDeliveredPacketCount()	const { return mDeliveredPacketCount; }
	uint32_t			GetDispatchedPacketCount()	const { return mDispatchedPacketCount; }

	const std::deque< InFlightPacket >&	GetInFlightPackets()	const { return mInFlightPackets; }

private:



	InFlightPacket*		WriteSequenceNumber(OutputMemoryBitStream& inOutputStream);
	void				WriteAckData(OutputMemoryBitStream& inOutputStream);

	//returns wether to drop the packet- if sequence number is too low!
	bool				ProcessSequenceNumber(InputMemoryBitStream& inInputStream);
	void				ProcessAcks(InputMemoryBitStream& inInputStream);


	void				AddPendingAck(PacketSequenceNumber inSequenceNumber);
	void				HandlePacketDeliveryFailure(const InFlightPacket& inFlightPacket);
	void				HandlePacketDeliverySuccess(const InFlightPacket& inFlightPacket);

	PacketSequenceNumber	mNextOutgoingSequenceNumber;
	PacketSequenceNumber	mNextExpectedSequenceNumber;

	std::deque< InFlightPacket >	mInFlightPackets;
	std::deque< AckRange >		mPendingAcks;

	bool					mShouldSendAcks;
	bool					mShouldProcessAcks;

	uint32_t		mDeliveredPacketCount;
	uint32_t		mDroppedPacketCount;
	uint32_t		mDispatchedPacketCount;

};


#endif
#ifndef ACKRANGE_H
#define ACKRANGE_H

#include "InFlightPacket.h"
#include "MemoryBitStream.h"

class AckRange
{
public:
	AckRange() : mStart(0), mCount(0) {}

	AckRange(PacketSequenceNumber inStart) : mStart(inStart), mCount(1) {}

	//if this is the next in sequence, just extend the range
	bool ExtendIfShould(PacketSequenceNumber inSequenceNumber);

	PacketSequenceNumber	GetStart() const { return mStart; }
	uint32_t		GetCount() const { return mCount; }

	void Write(OutputMemoryBitStream& inOutputStream) const;
	void Read(InputMemoryBitStream& inInputStream);

private:
	PacketSequenceNumber	mStart;
	uint32_t		mCount;
};

#endif // !ACKRANGE_H


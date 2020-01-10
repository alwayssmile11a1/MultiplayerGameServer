#include "InFlightPacket.h"

InFlightPacket::InFlightPacket(PacketSequenceNumber inSequenceNumber)
{
	mSequenceNumber = inSequenceNumber;
	mTimeDispatched = Time::sInstance.GetFrameStartTime();
}


void InFlightPacket::HandleDeliveryFailure() const
{
	for (const auto& pair : mTransmissionDataMap)
	{
		pair.second->HandleDeliveryFailure();
	}
}

void InFlightPacket::HandleDeliverySuccess() const
{
	for (const auto& pair : mTransmissionDataMap)
	{
		pair.second->HandleDeliverySuccess();
	}
}
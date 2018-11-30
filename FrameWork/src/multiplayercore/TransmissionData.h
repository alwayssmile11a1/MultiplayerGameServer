#ifndef TRANSMISSIONDATA_H
#define TRANSMISSIONDATA_H

#include<memory>

class TransmissionData
{
public:
	virtual void HandleDeliveryFailure() const = 0;
	virtual void HandleDeliverySuccess() const = 0;
};
typedef std::shared_ptr< TransmissionData > TransmissionDataPtr;

#endif
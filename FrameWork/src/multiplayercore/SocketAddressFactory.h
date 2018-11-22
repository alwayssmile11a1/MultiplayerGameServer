#ifndef SOCKETADDRESSFACTORY_H
#define SOCKETADDRESSFACTORY_H

#include "SocketAddress.h"
#include "NetworkHelper.h"

class SocketAddressFactory
{
public:

	static SocketAddressPtr CreateIPv4FromString(const std::string& inString);
};

#endif
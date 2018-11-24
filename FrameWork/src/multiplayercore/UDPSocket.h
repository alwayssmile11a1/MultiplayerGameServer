#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "SocketInclude.h"
#include "SocketAddress.h"
#include "NetworkHelper.h"
#include <memory>
//A wrapper class for SOCKET 
class UDPSocket
{
public:

	~UDPSocket();

	int Bind(const SocketAddress& inToAddress);
	int SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress);
	int ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress);

	/*
	int SendTo( const MemoryOutputStream& inMOS, const SocketAddress& inToAddress );
	int ReceiveFrom( MemoryInputStream& inMIS, SocketAddress& outFromAddress );
	*/

	int SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	friend class NetworkManager;
	UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;

};

typedef std::shared_ptr< UDPSocket >	UDPSocketPtr;

#endif
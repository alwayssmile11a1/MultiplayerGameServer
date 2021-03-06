#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "SocketInclude.h"
#include "SocketAddress.h"
#include "NetworkHelper.h"
#include <memory>
//A wrapper class for SOCKET
class TCPSocket
{
public:
	~TCPSocket();

	int								Connect(const SocketAddress& inAddress);
	int								Bind(const SocketAddress& inToAddress);
	int								Listen(int inBackLog = 32);
	std::shared_ptr< TCPSocket >			Accept(SocketAddress& inFromAddress);
	int32_t							Send(const void* inData, size_t inLen);
	int32_t							Receive(void* inBuffer, size_t inLen);
private:
	friend class NetworkManager;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET		mSocket;
};
typedef std::shared_ptr< TCPSocket > TCPSocketPtr;

#endif
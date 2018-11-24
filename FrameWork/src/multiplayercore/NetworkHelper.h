#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include "SocketInclude.h"
#include <vector>
#include "StringUtils.h"
#include "../console/Debug.h"

class NetworkHelper
{
public:
	static void Log(const char* message, ...);
	static void	ReportError(const char* message);
	static int GetLastError();

	//this function is really helpful when a large number of users connects to server, but we don't need to use this for now 
	//static int Select(const std::vector< TCPSocketPtr >* inReadSet,
	//	std::vector< TCPSocketPtr >* outReadSet,
	//	const std::vector< TCPSocketPtr >* inWriteSet,
	//	std::vector< TCPSocketPtr >* outWriteSet,
	//	const std::vector< TCPSocketPtr >* inExceptSet,
	//	std::vector< TCPSocketPtr >* outExceptSet);


private:

	//inline static fd_set* FillSetFromVector(fd_set& outSet, const std::vector< TCPSocketPtr >* inSockets, int& ioNaxNfds);
	//inline static void FillVectorFromSet(std::vector< TCPSocketPtr >* outSockets, const std::vector< TCPSocketPtr >* inSockets, const fd_set& inSet);
};

#endif
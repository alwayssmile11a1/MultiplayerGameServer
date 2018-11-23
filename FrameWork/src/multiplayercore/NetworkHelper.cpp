#include "NetworkHelper.h"



void NetworkHelper::ReportError(const char* inOperationDesc)
{
//#if _WIN32
//	LPVOID lpMsgBuf;
//	DWORD errorNum = GetLastError();
//
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER |
//		FORMAT_MESSAGE_FROM_SYSTEM |
//		FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL,
//		errorNum,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf,
//		0, NULL);
//
//
//	LOG("Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
//#else
//	LOG("Error: %hs", inOperationDesc);
//#endif
	Debug::Log("ERROR %s: %d \n", inOperationDesc, GetLastError());

}

int NetworkHelper::GetLastError()
{
#if _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif

}

//fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const std::vector< TCPSocketPtr >* inSockets, int& ioNaxNfds)
//{
//	if (inSockets)
//	{
//		FD_ZERO(&outSet);
//
//		for (const TCPSocketPtr& socket : *inSockets)
//		{
//			FD_SET(socket->mSocket, &outSet);
//#if !_WIN32
//			ioNaxNfds = std::max(ioNaxNfds, socket->mSocket);
//#endif
//		}
//		return &outSet;
//	}
//	else
//	{
//		return nullptr;
//	}
//}
//
//void SocketUtil::FillVectorFromSet(std::vector< TCPSocketPtr >* outSockets, const std::vector< TCPSocketPtr >* inSockets, const fd_set& inSet)
//{
//	if (inSockets && outSockets)
//	{
//		outSockets->clear();
//		for (const TCPSocketPtr& socket : *inSockets)
//		{
//			if (FD_ISSET(socket->mSocket, &inSet))
//			{
//				outSockets->push_back(socket);
//			}
//		}
//	}
//}
//
//int SocketUtil::Select(const std::vector< TCPSocketPtr >* inReadSet,
//	std::vector< TCPSocketPtr >* outReadSet,
//	const std::vector< TCPSocketPtr >* inWriteSet,
//	std::vector< TCPSocketPtr >* outWriteSet,
//	const std::vector< TCPSocketPtr >* inExceptSet,
//	std::vector< TCPSocketPtr >* outExceptSet)
//{
//	//build up some sets from our vectors
//	fd_set read, write, except;
//
//	int nfds = 0;
//
//	fd_set *readPtr = FillSetFromVector(read, inReadSet, nfds);
//	fd_set *writePtr = FillSetFromVector(read, inWriteSet, nfds);
//	fd_set *exceptPtr = FillSetFromVector(read, inExceptSet, nfds);
//
//	int toRet = select(nfds + 1, readPtr, writePtr, exceptPtr, nullptr);
//
//	if (toRet > 0)
//	{
//		FillVectorFromSet(outReadSet, inReadSet, read);
//		FillVectorFromSet(outWriteSet, inWriteSet, write);
//		FillVectorFromSet(outExceptSet, inExceptSet, except);
//	}
//	return toRet;
//}

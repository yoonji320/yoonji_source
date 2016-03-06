/// @file		client.h
///	@brief		클라이언트
/// @author		yoonzz320
///	@date		2016-02-03

#ifndef	__CLIENT_H
#define	__CLIENT_H

typedef struct ClientArg
{
	int		nSock;
	char	sAddr[128];
	int		nPort;
} ClientArg;

extern int TCPLogConnect(
					ClientArg* pClientArg);

extern int TCPLogClientWrite(
					ClientArg* pClientArg,
					char* sLogData);

extern ClientArg* TCPLogClientCreate(
					int		nPort,
					char*	sAddr);

extern int TCPLogClientDestroy(ClientArg* pClientArg);

#endif // __CLIENT_H

/// @file   server.h
/// @brief  서버 
/// @author yoonzz320
/// @date   2016-02-03

#ifndef __SERVER_H
#define __SERVER_H

typedef void* (*Func)(void*);

// 리스닝기능을 하는 쓰레드 함수
typedef struct ListenArg
{
	int		nServerSocket;
	Func	pFunc;
} ListenArg;

typedef struct ServerArg
{
	int		nSock;
	Func	pFunc;
} ServerArg;

extern int TCPLogServerRead(
					ServerArg* pServerArg);

extern int ThreadSendRecv(
					ServerArg* pServerArg);

extern int TCPLogServerStart(
					int nPort,
					Func pFunc);

extern int TCPLogServerDestroy();

#endif // __SERVER_H

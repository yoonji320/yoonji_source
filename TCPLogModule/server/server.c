/// @file       server.c
/// @brief     	서버
/// @author     yoonzz320
/// @date       2016-02-03

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "debug.h"
#include "thread.h"

#define BUFSIZE 102400

// 데이터를 받아오는 함수
int TCPLogServerRead(
					ServerArg* pServerArg)
{
	int		nLen = 0;
	int		nRet = 0;
	char	sData[BUFSIZE] = {0, };

	if (pServerArg == NULL || pServerArg->nSock <= 0)
	{
		debug();

		return -1;
	}

	nRet = recv(pServerArg->nSock, &nLen, sizeof(nLen), MSG_WAITALL);
	
	if (nRet < 1)
	{
		debug("nRet1=%d", nRet);

		// 소켓 닫고
		close(pServerArg->nSock);
		pServerArg->nSock = 0;

		return -1;
	}

	nLen = ntohl(nLen);

	// debug("###nLen=%d", nLen);

	// 원하는 데이터를 모두 전송 받지 못했을 경우 return 하지 않고 기다리는 flag 준다
	nRet = recv(pServerArg->nSock, sData, nLen, MSG_WAITALL);
	
	//	debug("###diff nLen:nRet(%d:%d)", nLen, nRet);

	if (nRet < 1)
	{
		debug("nRet2=%d", nRet);
		
		// 소켓 닫고
		close(pServerArg->nSock);
		pServerArg->nSock = 0;

		return -1;
	}

	// 구조체 내에 있는 받아온 함수를 실행
	pServerArg->pFunc(sData);

	return 0;
}

// 리스닝기능을 하는 쓰레드 함수
int ThreadListen(
					ListenArg* pListenArg)
{
	int			nClientSocket = 0;
	int			nClientAddrSize = 0;
	ServerArg* 	pServerArg = NULL;

	struct sockaddr_in	hClientAddr;

	// client 연결이 accept 되면, thread 생성 후, send->recv
	
	while(1)	// 리스닝 하는 소켓
	{
		nClientAddrSize = sizeof(hClientAddr);
		nClientSocket = accept(pListenArg->nServerSocket, (struct sockaddr*)&hClientAddr, (socklen_t*)&nClientAddrSize);

		if (nClientSocket < 0)
		{
			debug("client accept error!\n");
			
			return -1;
		}

		pServerArg = malloc(sizeof(*pServerArg));
	
		if (pServerArg == NULL)
		{
			debug();
			
			return -1;
		}
	
		memset(pServerArg, 0, sizeof(*pServerArg));

		// ServerArg 구조체에 할당..
		pServerArg->nSock = nClientSocket;
		pServerArg->pFunc = pListenArg->pFunc;

		if (pServerArg->nSock > 0)
		{
			// 스레드 생성
			if (ThreadStart((ThreadFunc)ThreadSendRecv, pServerArg) < 0)
			{
				debug();
			}
		}

		else
		{
			debug("accept error\n");
		}
	}
	
	close(pListenArg->nServerSocket);

	free(pListenArg);

	return 0;
}

// 쓰레드 함수
int	ThreadSendRecv(
					ServerArg* pServerArg)
{
	if (pServerArg == NULL)
	{
		debug();

		return -1;
	}

	while (1)
	{
		if ((TCPLogServerRead(pServerArg)) < 0 )
		{
			debug();

			return -1;
		}
	}

	// pServerArg Free 해준다
	free(pServerArg);

	return 0;
}

// 서버로그 start 함수
int TCPLogServerStart(
					int nPort,
					Func pFunc)
{
	int 		nServerSocket = 0;
	ListenArg*	pListenArg = NULL;

	struct sockaddr_in	hServerAddr;

	if (nPort < 0 || pFunc == NULL)
	{
		debug();

		return -1;
	}

	// 소켓 디스크립터를 얻어온다.
	nServerSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (nServerSocket < 0)
	{
		debug("socket error\n");

		return -1;
	}

	// 리스닝 정보 설정
	hServerAddr.sin_family		= AF_INET;
	hServerAddr.sin_port		= htons(nPort);
	hServerAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(nServerSocket, (struct sockaddr*)&hServerAddr, sizeof(hServerAddr)) == -1)
	{
		debug("bind error\n");

		return -1;
	}

	if (listen(nServerSocket, 5) == -1)
	{
		debug("listen error\n");

		return -1;
	}

	pListenArg = malloc(sizeof(*pListenArg));
	
	if (pListenArg == NULL)
	{
		debug();

		return -1;
	}

	memset(pListenArg, 0, sizeof(*pListenArg));
	
	// 받아온 pFunc 를 ListenArg 의 pFunc 으로 할당해준 뒤에 넘겨야 함
	pListenArg->nServerSocket = nServerSocket;
	pListenArg->pFunc = pFunc; 

	if (ThreadStart((ThreadFunc)ThreadListen, pListenArg) < 0)
	{
		debug();
	}

	return 0;
}


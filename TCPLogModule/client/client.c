/// @file		client.c
///	@brief		클라이언트
/// @author		yoonzz320
///	@date		2016-02-03

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"
#include "debug.h"

// Connect 함수
int TCPLogConnect(
					ClientArg* pClientArg)
{
	struct sockaddr_in	hServerAddr;

	if (pClientArg == NULL)
	{
		debug();

		return -1;
	}

	// 소켓 디스크립터를 얻어온다.
	pClientArg->nSock = socket(PF_INET, SOCK_STREAM, 0);

	if (pClientArg->nSock < 0 )
	{
		debug("sock error");

		return -1;
	}

	// 리스닝 정보를 설정한다.
	hServerAddr.sin_family		= AF_INET;
	hServerAddr.sin_port		= htons(pClientArg->nPort);
	hServerAddr.sin_addr.s_addr	= inet_addr(pClientArg->sAddr);

	// 서버로 연결 시도
	if (connect(pClientArg->nSock, (struct sockaddr*)&hServerAddr, sizeof(hServerAddr)) < 0)
	{
		debug("connect error\n");

		// 소켓 닫는다
		close(pClientArg->nSock);

		return -1;
	}

	return 0;
}

// Send 하는 함수
int TCPLogClientWrite(
					ClientArg* pClientArg,
					char* sLogData)
{
	int nDataSize = 0;
	int nSendRes1 = 0;
	int nSendRes2 = 0;

	if (pClientArg == NULL || sLogData == NULL)
	{
		debug();

		return -1;
	}

	//printf("###strlen(%d)\n", strlen(sLogData));

	// 전송할 데이터의 크기를 nDataSiVze 변수에 담는다.
	nDataSize = strlen(sLogData);	

	nDataSize = htonl(nDataSize);
	nSendRes1 = send(pClientArg->nSock, &nDataSize, sizeof(nDataSize), 0);
	nSendRes2 = send(pClientArg->nSock, sLogData, strlen(sLogData), 0);

	//printf("###nDataSize(%d)\n", nDataSize);

	// 연결이 끊어졌을 경우 예외처리
	if (nSendRes1 <= 0 || nSendRes2 <= 0)
	{
		close(pClientArg->nSock);

		// 재연결
		if ((TCPLogConnect(pClientArg)) < 0)
		{
			debug();

			return -1;
		}

		// 다시 send!
		send(pClientArg->nSock, &nDataSize, sizeof(nDataSize), 0);
		send(pClientArg->nSock, sLogData, nDataSize, 0);
	}

	return 0;
}

// Client 생성
ClientArg*	TCPLogClientCreate(
					int		nPort,
					char*	sAddr)
{
	ClientArg* pClientArg = NULL;

	if ((pClientArg = malloc(sizeof(*pClientArg))) == NULL)
	{
		debug();

		return NULL;
	}

	// 초기화
	memset(pClientArg, 0, sizeof(*pClientArg));

	// 받아온 값을 ClientArg 구조체로 assign..
	pClientArg->nPort = nPort;
	strncpy(pClientArg->sAddr, sAddr, sizeof(pClientArg->sAddr));

	// connect
	if ((TCPLogConnect(pClientArg)) < 0)
	{
		debug("Connect error \n");

		return NULL;
	}

	return pClientArg;
}

// Client 해제
int TCPLogClientDestroy(
					ClientArg* pClientArg)
{
	close(pClientArg->nSock);
	free(pClientArg);

	return 0;
}



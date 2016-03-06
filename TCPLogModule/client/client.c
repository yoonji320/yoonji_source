/// @file		client.c
///	@brief		Ŭ���̾�Ʈ
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

// Connect �Լ�
int TCPLogConnect(
					ClientArg* pClientArg)
{
	struct sockaddr_in	hServerAddr;

	if (pClientArg == NULL)
	{
		debug();

		return -1;
	}

	// ���� ��ũ���͸� ���´�.
	pClientArg->nSock = socket(PF_INET, SOCK_STREAM, 0);

	if (pClientArg->nSock < 0 )
	{
		debug("sock error");

		return -1;
	}

	// ������ ������ �����Ѵ�.
	hServerAddr.sin_family		= AF_INET;
	hServerAddr.sin_port		= htons(pClientArg->nPort);
	hServerAddr.sin_addr.s_addr	= inet_addr(pClientArg->sAddr);

	// ������ ���� �õ�
	if (connect(pClientArg->nSock, (struct sockaddr*)&hServerAddr, sizeof(hServerAddr)) < 0)
	{
		debug("connect error\n");

		// ���� �ݴ´�
		close(pClientArg->nSock);

		return -1;
	}

	return 0;
}

// Send �ϴ� �Լ�
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

	// ������ �������� ũ�⸦ nDataSiVze ������ ��´�.
	nDataSize = strlen(sLogData);	

	nDataSize = htonl(nDataSize);
	nSendRes1 = send(pClientArg->nSock, &nDataSize, sizeof(nDataSize), 0);
	nSendRes2 = send(pClientArg->nSock, sLogData, strlen(sLogData), 0);

	//printf("###nDataSize(%d)\n", nDataSize);

	// ������ �������� ��� ����ó��
	if (nSendRes1 <= 0 || nSendRes2 <= 0)
	{
		close(pClientArg->nSock);

		// �翬��
		if ((TCPLogConnect(pClientArg)) < 0)
		{
			debug();

			return -1;
		}

		// �ٽ� send!
		send(pClientArg->nSock, &nDataSize, sizeof(nDataSize), 0);
		send(pClientArg->nSock, sLogData, nDataSize, 0);
	}

	return 0;
}

// Client ����
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

	// �ʱ�ȭ
	memset(pClientArg, 0, sizeof(*pClientArg));

	// �޾ƿ� ���� ClientArg ����ü�� assign..
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

// Client ����
int TCPLogClientDestroy(
					ClientArg* pClientArg)
{
	close(pClientArg->nSock);
	free(pClientArg);

	return 0;
}



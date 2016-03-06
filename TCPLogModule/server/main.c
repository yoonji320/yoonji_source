#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "thread.h"
#include "debug.h"

FILE*	pFile = NULL;
char	sRecvFile[128]="recvfile";

// print 함수
int PrintFunc(
			void* sData)
{
	fprintf(pFile, "%s", (char*)sData);
	fflush(pFile);

	return 0;
}

int main(
			int		nParam,
			char**	sParam)
{
	int nPort = 0;

	if (nParam < 2 || !sParam)
	{
		printf("usage: %s <port>\n", sParam[0]);

		return -1;
	}
	
	nPort = atoi(sParam[1]);

	if ((pFile = fopen64(sRecvFile, "w")) == NULL)
	{
		debug();

		return -1;
	}
	
	// 클라이언트와 연결
	TCPLogServerStart(nPort, (Func)PrintFunc);

	while (1)
	{
	}

	return 0;
}


#ifndef	_LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "debug.h"

#define BUFSIZE 102400

int ClientStart(
				int 	nParam,
				char**	sParam)
{
	char	sFileName[128]={0,};
	char	sAddr[256]={0,};
	int		nPort = 0;

	if (nParam < 4 || !sParam)
	{
		debug();

		return -1;
	}

	strcpy(sFileName, sParam[1]);
	strcpy(sAddr, sParam[2]);
	nPort = atoi(sParam[3]);

	ClientArg* pClientArg = NULL;

	pClientArg = TCPLogClientCreate(nPort, sAddr);

	// 파일 한줄씩 읽어오기
	FILE* pFile = NULL;

	if (sFileName == NULL)
	{
		debug();

		return -1;
	}

	if ((pFile =fopen64(sFileName, "r")) == NULL)
	{
		debug("sFileName=%s", sFileName);

		return -1;
	}

	while (!feof(pFile))
	{
		char sLogData[BUFSIZE] = {0,};

		if (fgets(sLogData, sizeof(sLogData), pFile) != NULL)
		{
			TCPLogClientWrite(pClientArg, sLogData);
		}
	}
	
	TCPLogClientDestroy(pClientArg);

	fclose(pFile);

	return 0;
}

int main(
			int		nParam,
			char**	sParam)
{
	int nRet = 0;

	if (nParam != 4)
	{
		printf("usage: %s <file_name> <Addr> <port>\n", sParam[0]);

		return -1;
	}

	nRet = ClientStart(nParam, sParam);

	return 0;
}

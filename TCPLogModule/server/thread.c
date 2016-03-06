/// @brief		쓰레드
/// @author 	yoonzz320
/// @date		2016-02-03

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "thread.h"
#include "debug.h"
#include "server.h"

// 쓰레드 start 함수
int ThreadStart(
					ThreadFunc	pRun,		// 스레드로 실행시킬 함수
					void* 		pServerArg)	// 해당 함수의 인자
{
	pthread_t	tThread;

	if (pRun == NULL || pServerArg == NULL)
	{
		debug();

		return -1;
	}

	if (pthread_create(&tThread, NULL, pRun, pServerArg) != 0
			|| pthread_detach(tThread) != 0)
	{
		debug();

		return -1;
	}
	
	return 0;
}

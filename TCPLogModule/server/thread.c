/// @brief		������
/// @author 	yoonzz320
/// @date		2016-02-03

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "thread.h"
#include "debug.h"
#include "server.h"

// ������ start �Լ�
int ThreadStart(
					ThreadFunc	pRun,		// ������� �����ų �Լ�
					void* 		pServerArg)	// �ش� �Լ��� ����
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

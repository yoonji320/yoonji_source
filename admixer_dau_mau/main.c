/// @file		main.c
/// @brief		daily active user ī��Ʈ 
/// @author		yoonji
///	@date		2015-12-09

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "admixer_dau.h"
#include <mysql.h>

/// @brief
///
/// @return		0 (����), -1 (����)

int main(
			int		nParam,		///< ���� ���� ����
			char**	sParam)		///< ���� ����
{
	int 	nRet = 0;
	time_t	nStartTime = 0;
	time_t	nEndTime = 0;

	//���α׷� ���� �ð�
	time(&nStartTime);
	printf(ctime(&nStartTime));

	if (nParam < 2 )
	{
		printf("usage: %s -option <dau|mau> [config_filename]\n", sParam[0]);

		return -1;
	}

	nRet = Admixer_DauStart(nParam, sParam);

	mysql_library_end();

	//���α׷� ���� �ð�
	time(&nEndTime);
	printf(ctime(&nEndTime));

	//���α׷� �� �ɸ� �ð�
	printf("�ɸ� �ð�: %lu ��\n", nEndTime - nStartTime);

	return nRet;
}

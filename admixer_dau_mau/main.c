/// @file		main.c
/// @brief		daily active user 카운트 
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
/// @return		0 (성공), -1 (실패)

int main(
			int		nParam,		///< 실행 인자 개수
			char**	sParam)		///< 실행 인자
{
	int 	nRet = 0;
	time_t	nStartTime = 0;
	time_t	nEndTime = 0;

	//프로그램 시작 시각
	time(&nStartTime);
	printf(ctime(&nStartTime));

	if (nParam < 2 )
	{
		printf("usage: %s -option <dau|mau> [config_filename]\n", sParam[0]);

		return -1;
	}

	nRet = Admixer_DauStart(nParam, sParam);

	mysql_library_end();

	//프로그램 종료 시각
	time(&nEndTime);
	printf(ctime(&nEndTime));

	//프로그램 총 걸린 시간
	printf("걸린 시간: %lu 초\n", nEndTime - nStartTime);

	return nRet;
}

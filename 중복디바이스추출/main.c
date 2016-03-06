
/// @file		main.c
/// @brief		중복네트워크 별 디바이스ID 추출
/// @author		yoonji
/// @date		2015-05

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "device.h"

///
/// @brief		
///
/// @return		0 (성공), -1 (실패)
///
int main(
			int 	nParam,		///< 실행 인자 개수
			char** 	sParam)		///< 실행 인자
{
	int nRet = 0;
	
	if (nParam != 5)
	{
		printf("usage: %s <work_id> <filename> <hashsize> <type>\n", sParam[0]);
		
		return 1;
	}

	//nRet = DeviceStart(sParam[1], atoi(sParam[2]));
	nRet = DeviceStart(nParam, sParam);

	return nRet;
}


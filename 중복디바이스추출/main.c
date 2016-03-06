
/// @file		main.c
/// @brief		�ߺ���Ʈ��ũ �� ����̽�ID ����
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
/// @return		0 (����), -1 (����)
///
int main(
			int 	nParam,		///< ���� ���� ����
			char** 	sParam)		///< ���� ����
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


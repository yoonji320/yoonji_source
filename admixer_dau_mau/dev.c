/// @file       dev.c
/// @brief     	디바이스 해시테이블
/// @author     yoonzz320
/// @date       2015-12-11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hash.h"
#include "dev.h"
#include "app.h"
#include "net.h"
#include "report.h"
#include "admixer_dau.h"

// dev 테이블 생성
DevTable*	DevTableCreate(
					int nNumEntry)
{
	int			nNumSlot = 0;
	DevTable*	pDevTable = NULL;

	if (nNumEntry < 1)
	{
		nNumEntry = 1;
	}

	if ((nNumSlot = HashGetPrime(nNumEntry*10)) < 0)
	{
		debug();

		return NULL;
	}

	if ((pDevTable = malloc(sizeof(*pDevTable))) == NULL)
	{
		debug();

		return NULL;
	}

	memset(pDevTable, 0, sizeof(*pDevTable));

	// pDeTable->pDevEntry 를 메모리 할당
	if ((pDevTable->pDevEntry = malloc(sizeof(DevEntry*)*nNumSlot)) == NULL)
	{
		debug();

		// 할당 실패 --> 해시테이블 free..
		
		free(pDevTable);

		return NULL;
	}

	// 초기화
	memset(pDevTable->pDevEntry, 0, sizeof(DevEntry*)*nNumSlot);
	pDevTable->nNumSlot = nNumSlot;
	pDevTable->nNumEntry = 0;

	return pDevTable;
}

// dev 엔트리 해제
static int  DevEntryDestroy(
					DevEntry* pDevEntry)
{
	if (pDevEntry == NULL)
	{
		return -1;
	}
	
	// AppList Destroy 호출
	AppListDestroy(pDevEntry->pAppList);
	
	// NetList Destroy 호출
	NetListDestroy(pDevEntry->pNetList);

	free(pDevEntry);

	return 0;
}

// dev 테이블 해제
int	DevTableDestroy(
					DevTable* pDevTable)
{	
	int i = 0;

	if (pDevTable == NULL)
	{
		debug();

		return -1;
	}

	// pDevEntryDestroy
	for (i=0; i<pDevTable->nNumSlot; ++i)
	{
		if (pDevTable->pDevEntry[i])
		{
			//debug("index=%d,dev_id=%s", i, pDevTable->pDevEntry[i]->sDevID);
			DevEntryDestroy(pDevTable->pDevEntry[i]);
		}
	}

	free(pDevTable->pDevEntry);
	free(pDevTable);

	return 0;
}

// dev 해시엔트리 create
DevEntry* DevEntryCreate(
					DevEntry*	pDevEntry)
{
	DevEntry* pEntry = NULL;

	if ((pEntry = malloc(sizeof(*pEntry))) == NULL)
	{
		debug();

		return NULL;
	}

	memcpy(pEntry, pDevEntry, sizeof(*pEntry));

	// AppList Create 함수 호출
	pEntry->pAppList = AppListCreate();

	// NetList Create 함수 호출
	pEntry->pNetList = NetListCreate();

	return pEntry;
}

// dev 테이블 insert
DevEntry*	DevTableInsert(
					DevTable*	pDevTable,
					DevEntry*	pDevEntry,
					int*		pFlag)
{
	int nIndex = 0;
	int i = 0;

	if (!pDevTable || !pDevEntry || !pFlag)
	{
		debug();

		return NULL;
	}

	// 해시테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(pDevEntry->sDevID, sizeof(pDevEntry->sDevID), pDevTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	for (i=0; i<pDevTable->nNumSlot; ++i)
	{
		// 해당 슬롯이 비었다면, 엔트리를 삽입
		if ((pDevTable->pDevEntry[nIndex]) == NULL)
		{
			if ((pDevTable->pDevEntry[nIndex] = DevEntryCreate(pDevEntry)) == NULL)
			{
				debug();

				return NULL;
			}
		
			pDevTable->nNumEntry++;
			
			// 플래그 셋팅
			*pFlag = 1;
			
			return (pDevTable->pDevEntry[nIndex]);
		}

		// 엔트리가 존재할 경우, sDevID 가 같은 데이터인지 확인,, 같으면,,
		else if (!memcmp(pDevTable->pDevEntry[nIndex]->sDevID, pDevEntry->sDevID, sizeof(pDevEntry->sDevID)))
		{
			// 플래그 셋팅
			*pFlag = 0;
			return (pDevTable->pDevEntry[nIndex]);
		}

		// 다음 슬롯 검사
		nIndex = (nIndex+1) % pDevTable->nNumSlot;
	}

	return NULL;
}

int DevTablePrint(
					DevTable*	pDevTable)
{
	int i = 0;

	if (pDevTable == NULL)
	{
		debug();

		return -1;
	}

	for (i=0; i<pDevTable->nNumSlot; ++i)
	{
		// 값이 없을 경우 계속..
		if (pDevTable->pDevEntry[i] == NULL)
		{
			continue;
		}

		// 값이 있으면..
		//snprintf(sBuf, sizeof(sBuf), "%d, %s,",i, pDevTable->pDevEntry[i]->sDevID);
		
		printf("%d, %s,",i, pDevTable->pDevEntry[i]->sDevID);

		if (pDevTable->pDevEntry[i]->pAppList)
		{
			AppListPrint(pDevTable->pDevEntry[i]->pAppList);
		}

		if (pDevTable->pDevEntry[i]->pNetList)
		{
			NetListPrint(pDevTable->pDevEntry[i]->pNetList);
		}
		
		printf("\n");
	}

	return 0;
}

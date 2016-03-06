///	@file		report.c
///	@brief		리포트 해시테이블
///	@author		yoonzz320
///	@date		2015-12-16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "report.h"
#include "hash.h"
#include "debug.h"

// 리포트 테이블 Create
RptTable* RptTableCreate(int nNumEntry)
{
	int 		nNumSlot = 0;
	RptTable* 	pRptTable = NULL;

	if (nNumEntry < 1)
	{
		nNumEntry = 1;
	}
	
	if ((nNumSlot = HashGetPrime(nNumEntry*10)) < 0)
	{
		debug();

		return NULL;
	}
	
	if ((pRptTable = malloc(sizeof(*pRptTable))) == NULL)
	{
		debug();

		return NULL;
	}
	
	memset(pRptTable, 0, sizeof(*pRptTable));

	if ((pRptTable->pRptEntry = malloc(sizeof(RptEntry*)*nNumSlot)) == NULL)
	{
		debug();

		free(pRptTable);

		return NULL;
	}
	
	memset(pRptTable->pRptEntry, 0, sizeof(RptEntry*)*nNumSlot);
	pRptTable->nNumSlot = nNumSlot;
	pRptTable->nNumEntry = 0;

	return pRptTable;
}

// 리포트 테이블 엔트리 destroy
static int RptEntryDestroy(
			RptEntry* pRptEntry)
{
	if (pRptEntry == NULL)
	{
		return -1;
	}

	free(pRptEntry);

	return 0;
}

// 리포트 테이블 destroy
int RptTableDestroy(
					RptTable*	pRptTable)
{
	int i = 0;

	if (pRptTable == NULL)
	{
		return -1;
	}

	for (i=0; i<pRptTable->nNumSlot; ++i)
	{
		RptEntryDestroy(pRptTable->pRptEntry[i]);
	}

	free(pRptTable->pRptEntry);
	free(pRptTable);

	return 0;
}

// 리포트 테이블 엔트리 Create
RptEntry* RptEntryCreate(
					RptEntry* pRptEntry)
{
	RptEntry* pEntry = NULL;

	if (!pRptEntry)
	{
		debug();

		return NULL;
	}

	if ((pEntry = malloc(sizeof(*pRptEntry))) == NULL)
	{
		debug();

		return NULL;
	}

	memcpy(pEntry, pRptEntry, sizeof(*pEntry));

	return pEntry;
}

// 리포트 테이블 Insert
RptEntry* RptTableInsert(
					RptTable* pRptTable,
					RptEntry* pRptEntry)
{
	int nIndex = 0;
	int i = 0 ;

	if (!pRptTable || !pRptEntry)
	{
		debug();

		return NULL;
	}

	// 해시테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex((char*)pRptEntry, sizeof(RptEntry)-sizeof(pRptEntry->nActiveUser), pRptTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}
	
	for (i=0; i<pRptTable->nNumSlot; ++i)
	{
		// 해당 슬롯이 비었다면, 
		if ((pRptTable->pRptEntry[nIndex]) == NULL)
		{
			if ((pRptTable->pRptEntry[nIndex] = RptEntryCreate(pRptEntry)) == NULL)
			{
				debug();

				return NULL;
			}

			pRptTable->nNumEntry++;

			return(pRptTable->pRptEntry[nIndex]);
		}

		// 엔트리가 존재할 경우, 같은 데이터인지 확인
		else if (!memcmp(pRptTable->pRptEntry[nIndex], pRptEntry, sizeof(RptEntry)-sizeof(pRptEntry->nActiveUser)))
		{
			return (pRptTable->pRptEntry[nIndex]);
		}

		// 다음 슬롯 검사
		nIndex = (nIndex+1) % pRptTable->nNumSlot;
	}
	
	return NULL;
}

// 리포트에 넣고, count
int ReportCount(
					RptTable* pRptTable,
					RptEntry* pRptEntry)
{
	// 리포트 테이블에 insert
	RptEntry* pEntry = NULL;

	if (!pRptTable || !pRptEntry)
	{
		debug();

		return -1;
	}

	if ((pEntry = RptTableInsert(pRptTable, pRptEntry)) == NULL)
	{
		debug();

		return -1;
	}
	
	// active user 값 증가
	if (pEntry != NULL)
	{
		pEntry->nActiveUser++;
	}

	return 0;
}


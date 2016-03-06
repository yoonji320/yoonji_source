///	@file		report.c
///	@brief		����Ʈ �ؽ����̺�
///	@author		yoonzz320
///	@date		2015-12-16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "report.h"
#include "hash.h"
#include "debug.h"

// ����Ʈ ���̺� Create
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

// ����Ʈ ���̺� ��Ʈ�� destroy
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

// ����Ʈ ���̺� destroy
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

// ����Ʈ ���̺� ��Ʈ�� Create
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

// ����Ʈ ���̺� Insert
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

	// �ؽ����̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex((char*)pRptEntry, sizeof(RptEntry)-sizeof(pRptEntry->nActiveUser), pRptTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}
	
	for (i=0; i<pRptTable->nNumSlot; ++i)
	{
		// �ش� ������ ����ٸ�, 
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

		// ��Ʈ���� ������ ���, ���� ���������� Ȯ��
		else if (!memcmp(pRptTable->pRptEntry[nIndex], pRptEntry, sizeof(RptEntry)-sizeof(pRptEntry->nActiveUser)))
		{
			return (pRptTable->pRptEntry[nIndex]);
		}

		// ���� ���� �˻�
		nIndex = (nIndex+1) % pRptTable->nNumSlot;
	}
	
	return NULL;
}

// ����Ʈ�� �ְ�, count
int ReportCount(
					RptTable* pRptTable,
					RptEntry* pRptEntry)
{
	// ����Ʈ ���̺� insert
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
	
	// active user �� ����
	if (pEntry != NULL)
	{
		pEntry->nActiveUser++;
	}

	return 0;
}


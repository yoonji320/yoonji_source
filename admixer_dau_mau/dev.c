/// @file       dev.c
/// @brief     	����̽� �ؽ����̺�
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

// dev ���̺� ����
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

	// pDeTable->pDevEntry �� �޸� �Ҵ�
	if ((pDevTable->pDevEntry = malloc(sizeof(DevEntry*)*nNumSlot)) == NULL)
	{
		debug();

		// �Ҵ� ���� --> �ؽ����̺� free..
		
		free(pDevTable);

		return NULL;
	}

	// �ʱ�ȭ
	memset(pDevTable->pDevEntry, 0, sizeof(DevEntry*)*nNumSlot);
	pDevTable->nNumSlot = nNumSlot;
	pDevTable->nNumEntry = 0;

	return pDevTable;
}

// dev ��Ʈ�� ����
static int  DevEntryDestroy(
					DevEntry* pDevEntry)
{
	if (pDevEntry == NULL)
	{
		return -1;
	}
	
	// AppList Destroy ȣ��
	AppListDestroy(pDevEntry->pAppList);
	
	// NetList Destroy ȣ��
	NetListDestroy(pDevEntry->pNetList);

	free(pDevEntry);

	return 0;
}

// dev ���̺� ����
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

// dev �ؽÿ�Ʈ�� create
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

	// AppList Create �Լ� ȣ��
	pEntry->pAppList = AppListCreate();

	// NetList Create �Լ� ȣ��
	pEntry->pNetList = NetListCreate();

	return pEntry;
}

// dev ���̺� insert
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

	// �ؽ����̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex(pDevEntry->sDevID, sizeof(pDevEntry->sDevID), pDevTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	for (i=0; i<pDevTable->nNumSlot; ++i)
	{
		// �ش� ������ ����ٸ�, ��Ʈ���� ����
		if ((pDevTable->pDevEntry[nIndex]) == NULL)
		{
			if ((pDevTable->pDevEntry[nIndex] = DevEntryCreate(pDevEntry)) == NULL)
			{
				debug();

				return NULL;
			}
		
			pDevTable->nNumEntry++;
			
			// �÷��� ����
			*pFlag = 1;
			
			return (pDevTable->pDevEntry[nIndex]);
		}

		// ��Ʈ���� ������ ���, sDevID �� ���� ���������� Ȯ��,, ������,,
		else if (!memcmp(pDevTable->pDevEntry[nIndex]->sDevID, pDevEntry->sDevID, sizeof(pDevEntry->sDevID)))
		{
			// �÷��� ����
			*pFlag = 0;
			return (pDevTable->pDevEntry[nIndex]);
		}

		// ���� ���� �˻�
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
		// ���� ���� ��� ���..
		if (pDevTable->pDevEntry[i] == NULL)
		{
			continue;
		}

		// ���� ������..
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

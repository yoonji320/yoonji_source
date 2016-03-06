
/// @file		hash.c
/// @brief		�ؽ�
/// @author		yoonzz320
/// @date		2015-10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "hash.h"

static int HashIsPrime(
					unsigned int num)
{
	unsigned int i;

	if (num <= 2)
	{
		return -1;
	}
	else if (num % 2 == 0)
	{
		return -1;
	}

	for (i=3; i<num; i+=2)
	{
		if (num % i == 0)
		{
			return -1;
		}
	}
	
	return 0;
}

int HashGetPrime(
					unsigned int num)
{
	unsigned int result;

	result = num;
	while (HashIsPrime(result))
	{
		result++;
	}

	return result;
}

/*
// �ؽ� ���̺� ����
HashTable* HashTableCreate(
							int nNumEntry)
{
	int			nNumSlot = 0;
	HashTable*	pTable = NULL;

	if (nNumEntry < 1)
	{
		nNumEntry = 1;
	}

	if ((nNumSlot = HashGetPrime(nNumEntry*10)) < 0)
	{
		debug();

		return NULL;
	}

	if ((pTable = malloc(sizeof(*pTable))) == NULL)
	{
		debug();

		return NULL;
	}
	
	memset(pTable, 0, sizeof(*pTable));

	// pTable->pEntry �� �޸� �Ҵ�
	if ((pTable->pEntry = malloc(sizeof(HashEntry*)*nNumSlot)) == NULL)
	{
		debug();

		// ��Ʈ�� �޸� �Ҵ翡 ���������Ƿ�, �Ҵ��ߴ� �ؽ����̺� free���ش�.
		free(pTable);

		return NULL;
	}

	// �ʱ�ȭ
	memset(pTable->pEntry, 0, sizeof(HashEntry*)*nNumSlot);
	pTable->nNumSlot = nNumSlot;
	pTable->nNumEntry = 0;

	return pTable;

}

// �ؽ� ���̺� ����
int HashTableDestroy(
					HashTable* pTable)
{
	int i = 0;

	if (pTable == NULL)
	{
		debug();

		return -1;
	}
	
	for (i=0; i<pTable->nNumSlot; i++)
	{
		if (pTable->pEntry[i])
		{
			free(pTable->pEntry[i]);
		}
	}

	free(pTable->pEntry);
	free(pTable);

	return 0;
}
*/

// �ؽÿ��� �ε����� ���´�.
inline int HashGetIndex(
					const char* value,	// �ؽð��� ����� �Է°�
					unsigned int len,			// �Է°��� ����
					unsigned int nNumSlot)		// �ؽ����̺� ���� ����
{
	unsigned int a;
	unsigned int b;
	unsigned int i;

	if (!value || len < 1 || nNumSlot < 1)
	{
		return -1;
	}

	a = b = 0;
	for (i=0; i<len; ++i)
	{
		a = (a << 4) + (*(value + i));
		b = a & 0xf0000000;

		if (b)
		{
			a = a ^ (b >> 24);
			a = a ^ b;
		}
	}

	return ( a % nNumSlot);
}

/*
inline int HashGetIndex2(
		const char*     sValue,
		unsigned int    nLen,
		unsigned int    nNumSlot)
{
	unsigned long   nHash = 5381;
	int             nNum;

	int i;

	if (!sValue || nLen < 1 || nNumSlot < 1)
	{
		return -1;
	}

	for (i=0; i<nLen; ++i)
	{
		nNum = sValue[i];
		nHash = ((nHash << 5) + nHash) + nNum;
	}

	return (nHash % nNumSlot);
}


inline int HashGetIndex3(
		const char*     sValue,
		unsigned int    nLen,
		unsigned int    nNumSlot)
{
	unsigned long   nHash = 0;
	int             nNum;

	int i;

	if (!sValue || nLen < 1 || nNumSlot < 1)
	{
		return -1;
	}

	for (i=0; i<nLen; ++i)
	{
		nNum = sValue[i];
		nHash = nNum + (nHash << 6) + (nHash << 16) - nHash;
	}

	return (nHash % nNumSlot);
}

inline int HashGetIndex4(
		const char*     sValue,
		unsigned int    nLen,
		unsigned int    nNumSlot)
{
	unsigned long   nHash = 0;
	int             nNum;

	int i;

	if (!sValue || nLen < 1 || nNumSlot < 1)
	{
		return -1;
	}

	for (i=0; i<nLen; ++i)
	{
		nNum = sValue[i];
		nHash += nNum;
	}

	return (nHash % nNumSlot);
}
*/
/*
// �ؽ� ���̺��� ��Ʈ�� ����
HashEntry* EventHashTableInsert(
					HashTable*	pTable,
					HashEntry*	pEntry)
{
	int nIndex = 0;
	int i = 0;

	// �ؽ����̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex3((char*)pEntry, sizeof(HashEntry)-sizeof(pEntry->sYmd), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	//debug("nIndex=%d", nIndex);

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// �ش� ������ ����ٸ�, ��Ʈ���� ����
		if (pTable->pEntry[nIndex] == NULL)
		{
			if ((pTable->pEntry[nIndex] = malloc(sizeof(HashEntry))) == NULL)
			{
				debug();

				return NULL;
			}

			memcpy(pTable->pEntry[nIndex], pEntry, sizeof(*pEntry));

			pTable->nNumEntry++;

			return (pTable->pEntry[nIndex]);
		}

		// ��Ʈ���� ������ ���, ���� ���������� Ȯ��
		else if (!memcmp(pTable->pEntry[nIndex], pEntry, sizeof(*pEntry))) 
		{
			return (pTable->pEntry[nIndex]);
		}
		
		// ���� ���� �˻�
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}
*/
/*
// �ؽ� ���̺��� ��Ʈ�� ����
HashEntry* EventHashTableInsert(
					HashTable*	pTable,
					HashEntry*	pEntry,
					char*		sKey)
{
	int nIndex = 0;
	int i = 0;

	// �ؽ����̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex(sKey, strlen(sKey), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	//debug("nIndex=%d", nIndex);

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// �ش� ������ ����ٸ�, ��Ʈ���� ����
		if (pTable->pEntry[nIndex] == NULL)
		{
			if ((pTable->pEntry[nIndex] = malloc(sizeof(HashEntry))) == NULL)
			{
				debug();

				return NULL;
			}

			memcpy(pTable->pEntry[nIndex], pEntry, sizeof(*pEntry));

			pTable->nNumEntry++;

			return (pTable->pEntry[nIndex]);
		}

		// ��Ʈ���� ������ ���, ���� ���������� Ȯ��
		else if (!memcmp(pTable->pEntry[nIndex], pEntry, sizeof(*pEntry))) 
		{
			return (pTable->pEntry[nIndex]);
		}
		
		// ���� ���� �˻�
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}
*/
/*
// ķ���� �ؽ����̺� ��Ʈ�� ����
HashEntry* CampHashTableInsert(
					HashTable*	pTable,
					HashEntry*	pEntry)
{
	int nIndex = 0;
	int i = 0;

	// �ؽ����̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex((char*)&pEntry->nCampID, sizeof(pEntry->nCampID), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	//debug("nIndex=%d", nIndex);

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// �ش� ������ ����ٸ�, ��Ʈ���� ����
		if (pTable->pEntry[nIndex] == NULL)
		{
			if ((pTable->pEntry[nIndex] = malloc(sizeof(HashEntry))) == NULL)
			{
				debug();

				return NULL;
			}

			memcpy(pTable->pEntry[nIndex], pEntry, sizeof(*pEntry));

			pTable->nNumEntry++;

			return (pTable->pEntry[nIndex]);
		}

		// ��Ʈ���� ������ ���, ���� ���������� Ȯ��
		else if (pTable->pEntry[nIndex]->nCampID == pEntry->nCampID) 
		{
			return (pTable->pEntry[nIndex]);
		}
		
		// ���� ���� �˻�
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}
*/
/*
// �ؽ� ���̺��� ��Ʈ�� ����
int HashTableDelete(
					HashTable* pTable,
					HashEntry* pEntry)
{
	int nIndex = 0;
	int i = 0;

	// �ؽ����̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex(pEntry->sDevID, sizeof(pEntry->sDevID), pTable->nNumSlot)) < 0 )
	{
		debug();

		return -1;
	}

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// �ش� ��Ʈ���� ����ٸ�, 
		if (pTable->pEntry[nIndex] == NULL)
		{
			debug();

			return -1;
		}

		// �����ϰ��� �ϴ� �����Ͱ� �´��� Ȯ��, ������ �ش� ��Ʈ�� ����
		else if (!memcmp(pTable->pEntry[nIndex]->sDevID, pEntry->sDevID, sizeof(pEntry->sDevID)))
		{
			free(pTable->pEntry[nIndex]);

			pTable->pEntry[nIndex] = NULL;
			pTable->nNumSlot--;

			return 0;
		}

		// �ƴϸ�, ���� ���� �˻�
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return -1;
}
*/

/*
//�ؽ� ���̺��� ��Ʈ���� ã�´�.
void* HashTableSearch(
					HashTable* pTable,
					HashEntry* pEntry)
{
	int i = 0;
	int nIndex = 0;

	if (!pTable || !pEntry)
	{
		debug();

		return NULL;
	}
		
	// �ε����� ���´�.
	if ((nIndex = HashGetIndex(pEntry->sDevID, sizeof(pEntry->sDevID), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}
	
	//debug("nIndex=%d", nIndex);
	
	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// �ش� ������ ����ٸ�,
		if (pTable->pEntry[nIndex] == NULL)
		{
			debug();

			return NULL;
		}

		// ���� �ִٸ�,ã���� �ϴ� �����Ͱ� �´��� Ȯ��.
		if (!memcmp(pTable->pEntry[nIndex]->sDevID, pEntry->sDevID, sizeof(pEntry->sDevID)))
		{
			return pTable->pEntry[nIndex];
		}
		
		// �ƴϸ�, ���� ���� �˻�
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}

// �ؽ� ���̺� ���
int HashTablePrint(
					HashTable* 	pTable,
					int			nFD)
{
	int i = 0;
	char sBuf[1024];

	if (pTable == NULL || nFD < 0)
	{
		debug();

		return -1;
	}

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// ���� ���� ��� ���~
		if (pTable->pEntry[i] == NULL)
		{
			continue;
		}
		
		// ���� ������ ���
		snprintf(sBuf, sizeof(sBuf), "%d,%s\n", i, (char*)pTable->pEntry[i]);
		write(nFD, sBuf, strlen(sBuf));
	}
	
	return 0;
}
*/

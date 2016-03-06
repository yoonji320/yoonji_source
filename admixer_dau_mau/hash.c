
/// @file		hash.c
/// @brief		해시
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
// 해시 테이블 생성
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

	// pTable->pEntry 를 메모리 할당
	if ((pTable->pEntry = malloc(sizeof(HashEntry*)*nNumSlot)) == NULL)
	{
		debug();

		// 엔트리 메모리 할당에 실패했으므로, 할당했던 해시테이블도 free해준다.
		free(pTable);

		return NULL;
	}

	// 초기화
	memset(pTable->pEntry, 0, sizeof(HashEntry*)*nNumSlot);
	pTable->nNumSlot = nNumSlot;
	pTable->nNumEntry = 0;

	return pTable;

}

// 해시 테이블 해제
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

// 해시에서 인덱스를 얻어온다.
inline int HashGetIndex(
					const char* value,	// 해시값을 계산할 입력값
					unsigned int len,			// 입력값의 길이
					unsigned int nNumSlot)		// 해시테이블 슬롯 갯수
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
// 해시 테이블에서 엔트리 삽입
HashEntry* EventHashTableInsert(
					HashTable*	pTable,
					HashEntry*	pEntry)
{
	int nIndex = 0;
	int i = 0;

	// 해시테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex3((char*)pEntry, sizeof(HashEntry)-sizeof(pEntry->sYmd), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	//debug("nIndex=%d", nIndex);

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// 해당 슬롯이 비었다면, 엔트리를 삽입
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

		// 엔트리가 존재할 경우, 같은 데이터인지 확인
		else if (!memcmp(pTable->pEntry[nIndex], pEntry, sizeof(*pEntry))) 
		{
			return (pTable->pEntry[nIndex]);
		}
		
		// 다음 슬롯 검사
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}
*/
/*
// 해시 테이블에서 엔트리 삽입
HashEntry* EventHashTableInsert(
					HashTable*	pTable,
					HashEntry*	pEntry,
					char*		sKey)
{
	int nIndex = 0;
	int i = 0;

	// 해시테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(sKey, strlen(sKey), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	//debug("nIndex=%d", nIndex);

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// 해당 슬롯이 비었다면, 엔트리를 삽입
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

		// 엔트리가 존재할 경우, 같은 데이터인지 확인
		else if (!memcmp(pTable->pEntry[nIndex], pEntry, sizeof(*pEntry))) 
		{
			return (pTable->pEntry[nIndex]);
		}
		
		// 다음 슬롯 검사
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}
*/
/*
// 캠페인 해시테이블에 엔트리 삽입
HashEntry* CampHashTableInsert(
					HashTable*	pTable,
					HashEntry*	pEntry)
{
	int nIndex = 0;
	int i = 0;

	// 해시테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex((char*)&pEntry->nCampID, sizeof(pEntry->nCampID), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	//debug("nIndex=%d", nIndex);

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// 해당 슬롯이 비었다면, 엔트리를 삽입
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

		// 엔트리가 존재할 경우, 같은 데이터인지 확인
		else if (pTable->pEntry[nIndex]->nCampID == pEntry->nCampID) 
		{
			return (pTable->pEntry[nIndex]);
		}
		
		// 다음 슬롯 검사
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}
*/
/*
// 해시 테이블에서 엔트리 삭제
int HashTableDelete(
					HashTable* pTable,
					HashEntry* pEntry)
{
	int nIndex = 0;
	int i = 0;

	// 해시테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(pEntry->sDevID, sizeof(pEntry->sDevID), pTable->nNumSlot)) < 0 )
	{
		debug();

		return -1;
	}

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// 해당 엔트리가 비었다면, 
		if (pTable->pEntry[nIndex] == NULL)
		{
			debug();

			return -1;
		}

		// 삭제하고자 하는 데이터가 맞는지 확인, 맞으면 해당 엔트리 삭제
		else if (!memcmp(pTable->pEntry[nIndex]->sDevID, pEntry->sDevID, sizeof(pEntry->sDevID)))
		{
			free(pTable->pEntry[nIndex]);

			pTable->pEntry[nIndex] = NULL;
			pTable->nNumSlot--;

			return 0;
		}

		// 아니면, 다음 슬롯 검사
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return -1;
}
*/

/*
//해시 테이블에서 엔트리를 찾는다.
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
		
	// 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(pEntry->sDevID, sizeof(pEntry->sDevID), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}
	
	//debug("nIndex=%d", nIndex);
	
	for (i=0; i<pTable->nNumSlot; ++i)
	{
		// 해당 슬롯이 비었다면,
		if (pTable->pEntry[nIndex] == NULL)
		{
			debug();

			return NULL;
		}

		// 값이 있다면,찾고자 하는 데이터가 맞는지 확인.
		if (!memcmp(pTable->pEntry[nIndex]->sDevID, pEntry->sDevID, sizeof(pEntry->sDevID)))
		{
			return pTable->pEntry[nIndex];
		}
		
		// 아니면, 다음 슬롯 검사
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}

	return NULL;
}

// 해시 테이블 출력
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
		// 값이 없을 경우 계속~
		if (pTable->pEntry[i] == NULL)
		{
			continue;
		}
		
		// 값이 있으면 출력
		snprintf(sBuf, sizeof(sBuf), "%d,%s\n", i, (char*)pTable->pEntry[i]);
		write(nFD, sBuf, strlen(sBuf));
	}
	
	return 0;
}
*/


/// @file		hash.c
/// @brief		해시 모듈, 해시값 계산에 필요한 인터페이스
/// @author		hansch
/// @date		2010-05

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

#include "hash.h"

/////////////////////////////////////////////////////////////////////////////
//
//
//			내부 인터페이스
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		인자가 소수인지 확인한다.
///
/// @return		0	: 소수
///				-1 	: 소수가 아님
///
static int HashIsPrime(
					unsigned int num)			///< 정수
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

///
/// @brief		해시 엔트리를 생성한다.
///
/// @return
///
/*static HashEntry* HashEntryCreate(
					void* 	pKey,			///< 키
					int		nKeyLen,		///< 키 길이
					void*	pData,			///< 데이터
					int		nDataLen)		///< 데이터 길이
{
	HashEntry* pEntry;

	if (!pKey || nKeyLen < 1 || !pData || nDataLen < 0)
	{
		debug();

		return NULL;
	}

	if ((pEntry = malloc(sizeof(*pEntry))) == NULL)
	{
		debug();

		return NULL;
	}

	memset(pEntry, 0, sizeof(*pEntry));

	// 키를 생성/복사한다.
	if ((pEntry->pKey = malloc(nKeyLen)) == NULL)
	{
		debug();

		free(pEntry);

		return NULL;
	}

	memcpy(pEntry->pKey, pKey, nKeyLen);
	pEntry->nKeyLen = nKeyLen;

	// 데이터를 생성/복사한다.
	if (nDataLen)
	{
		if ((pEntry->pData = malloc(nDataLen)) == NULL)
		{
			debug();

			free(pEntry->pKey);
			free(pEntry);

			return NULL;
		}

		memcpy(pEntry->pData, pData, nDataLen);
		pEntry->nDataLen = nDataLen;
	}

	// 데이터 길이가 0 이라면 주소값만 복사한다.
	else
	{
		pEntry->pData 		= pData;
		pEntry->nDataLen 	= 0;
	}

	return pEntry;
}
*/

///
/// @brief		해시 엔트리를 메모리에서 해제한다.
///
/// @return
///

/*
 static void* HashEntryDestroy(
					HashEntry* 	pEntry)		///< 해시 엔트리
{
	void* pData;

	if (pEntry == NULL)
	{
		return NULL;
	}

	free(pEntry->pKey);

	// 직접 할당한 데이터는 free() 한다.
	if (pEntry->nDataLen > 0 && pEntry->pData)
	{
		free(pEntry->pData);
		free(pEntry);

		return NULL;
	}

	// 외부에서 할당한 데이터는 리턴한다.
	pData = pEntry->pData;
	free(pEntry);

	return pData;
}
*/


/////////////////////////////////////////////////////////////////////////////
//
//
//			외부 인터페이스
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		인자보다 큰 가장 작은 소수를 구한다.
///
/// @return		소수	: 성공
///
unsigned int HashGetPrime(
					unsigned int num)			///< 기준값
{
	unsigned int result;

	result = num;
	while (HashIsPrime(result))
	{
		result++;
	}

	return result;
}

///
/// @brief		해시값을 계산한다.
///
/// @return		해시 인덱스 	: 성공
///				-1 				: 실패
///
inline int HashGetIndex(
					const char* value, 			///< 해시값을 계산할 입력값 
					unsigned int len, 			///< 입력값의 길이
					unsigned int num_slot)		///< 해시 테이블의 슬롯수
{
	unsigned int a;
	unsigned int b;

	unsigned int i;

	if (!value || len < 1 || num_slot < 1)
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

	return (a % num_slot);
}

///
/// @brief		해시 테이블을 생성한다.
///
/// @return
///
/*HashTable* HashTableCreate(
					int	nNumEntry)				///< 엔트리 개수
{
	int			nNumSlot;
	HashTable* 	pTable;

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

	if ((pTable->pSlot = malloc(sizeof(List*)*nNumSlot)) == NULL)
	{
		debug();

		free(pTable);

		return NULL;
	}

	memset(pTable->pSlot, 0, sizeof(List*)*nNumSlot);
	pTable->nNumSlot = nNumSlot;
	pTable->nNumEntry = 0;

	return pTable;
}

///
/// @brief		해시 테이블을 메모리에서 해제한다.
///
/// @return
///
int HashTableDestroy(
					HashTable* 	pTable,		///< 해시 테이블
					DestroyFunc	pFree)		///< 데이터 해제 함수
{
	int i;

	if (pTable == NULL)
	{
		debug();

		return -1;
	}

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		if (pTable->pSlot[i])
		{
			ListNode* pNode = pTable->pSlot[i]->pHead;

			while (pNode)
			{
				void* pData = HashEntryDestroy(pNode->pData);;
				
				if (pData && pFree)
				{
					pFree(pData);
				}

				pNode = pNode->pNext;
			}

			ListDestroy(pTable->pSlot[i], NULL);
		}
	}

	free(pTable->pSlot);
	free(pTable);

	return 0;
}

///
/// @brief		해시 테이블에 엔트리를 삽입한다.
///
/// @return
///
void* HashTableInsert(
					HashTable*	pTable,		///< 해시 테이블
					void*		pKey,		///< 키
					int			nKeyLen,	///< 키 길이
					void*		pData,		///< 데이터
					int			nDataLen)	///< 데이터 길이
{
	int 		nIndex;
	HashEntry*	pEntry;
	ListNode*	pNode;

	if (!pTable || !pTable->pSlot || !pKey || nKeyLen < 1 || !pData || nDataLen < 0)
	{
		debug();

		return NULL;
	}

	// 해시 테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(pKey, nKeyLen, pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	// 슬롯이 비었음 : 해당 슬롯에 추가함
	if (pTable->pSlot[nIndex] == NULL)
	{
		// 엔트리 리스트 생성
		if ((pTable->pSlot[nIndex] = ListCreate()) == NULL)
		{
			debug();

			return NULL;
		}

		if ((pEntry = HashEntryCreate(pKey, nKeyLen, pData, nDataLen)) == NULL)
		{
			debug();

			ListDestroy(pTable->pSlot[nIndex], NULL);
			pTable->pSlot[nIndex] = NULL;

			return NULL;
		}

		if (ListInsert(pTable->pSlot[nIndex], pEntry, 0) == NULL)
		{
			debug();

			HashEntryDestroy(pEntry);
			ListDestroy(pTable->pSlot[nIndex], NULL);
			pTable->pSlot[nIndex] = NULL;

			return NULL;
		}

		pTable->nNumEntry++;

		return pEntry;
	}

	// 동일한 엔트리가 있다면 리턴~	
	
	pNode = pTable->pSlot[nIndex]->pHead;

	while (pNode)
	{
		pEntry = pNode->pData;

		if (pEntry->nKeyLen == nKeyLen && !memcmp(pEntry->pKey, pKey, nKeyLen))
		{
			return pEntry;
		}

		pNode = pNode->pNext;
	}

	// 충돌 : 리스트에 추가
	
	if ((pEntry = HashEntryCreate(pKey, nKeyLen, pData, nDataLen)) == NULL)
	{
		debug();

		return NULL;
	}

	if (ListInsert(pTable->pSlot[nIndex], pEntry, 0) == NULL)
	{
		debug();

		HashEntryDestroy(pEntry);

		return NULL;
	}

	pTable->nNumEntry++;

	return pEntry;
}

///
/// @brief		해시 테이블에서 엔트리를 삭제한다.
///
/// @return
///
void* HashTableDelete(
					HashTable*	pTable,		///< 해시 테이블
					void*		pKey,		///< 키
					int			nKeyLen)	///< 키 길이
{
	int 		nIndex;
	HashEntry* 	pEntry;
	ListNode*	pNode;

	if (!pTable || !pTable->pSlot || !pKey || nKeyLen < 1)
	{
		debug();

		return NULL;
	}

	// 해시 테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(pKey, nKeyLen, pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	// 슬롯이 비었음 : 해당 엔트리가 존재하지 않음
	if (pTable->pSlot[nIndex] == NULL)
	{
		return NULL;
	}

	// 슬롯 리스트 검색
	
	pNode = pTable->pSlot[nIndex]->pHead;

	while (pNode)
	{
		pEntry = pNode->pData;
		
		// 엔트리 발견 : 삭제~
		if (pEntry->nKeyLen == nKeyLen && !memcmp(pEntry->pKey, pKey, nKeyLen))
		{
			ListDeleteNode(pTable->pSlot[nIndex], pNode);
			pTable->nNumEntry--;
			
			return (HashEntryDestroy(pEntry));
		}

		pNode = pNode->pNext;
	}

	// 엔트리 없음
	return NULL;
}

///
/// @brief		해시 테이블에서 엔트리를 찾는다.
///
/// @return
///
void* HashTableSearch(
					HashTable*	pTable,		///< 해시 테이블
					void*		pKey,		///< 키
					int			nKeyLen)	///< 키 길이
{
	int 		nIndex;
	HashEntry* 	pEntry;
	ListNode*	pNode;

	if (!pTable || !pTable->pSlot || !pKey || nKeyLen < 1)
	{
		debug();

		return NULL;
	}

	// 해시 테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(pKey, nKeyLen, pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	// 슬롯이 비었음 : 해당 엔트리가 존재하지 않음
	if (pTable->pSlot[nIndex] == NULL)
	{
		return NULL;
	}

	// 슬롯 리스트 검색
	
	pNode = pTable->pSlot[nIndex]->pHead;

	while (pNode)
	{
		pEntry = pNode->pData;
		
		// 엔트리 발견 : 리턴~
		if (pEntry->nKeyLen == nKeyLen && !memcmp(pEntry->pKey, pKey, nKeyLen))
		{
			return pEntry;
		}

		pNode = pNode->pNext;
	}

	// 엔트리 없음
	return NULL;
}

///
/// @brief		해시 테이블을 출력한다.
///
/// @return
///
int HashTablePrint(
					HashTable* 	pTable,		///< 해시 테이블
					int			nFD,		///< 파일 디스크립터
					PrintFunc	pPrint)		///< 출력 함수
{
	int 	nNum = 0;
	char	sBuf[1024];

	int i;

	if (!pTable || nFD < 1 || !pPrint)
	{
		debug();

		return -1;
	}

	for (i=0; i<pTable->nNumSlot; ++i)
	{
		ListNode* pNode;

		if (pTable->pSlot[i] == NULL)
		{
			continue;
		}

		pNode = pTable->pSlot[i]->pHead;

		while (pNode)
		{
			HashEntry* pEntry = pNode->pData;

			snprintf(sBuf, sizeof(sBuf)-1, "%5u [%8u] ", ++nNum, i);
			write(nFD, sBuf, strlen(sBuf));
			pPrint(pEntry->pData, nFD);
			
			pNode = pNode->pNext;
		}
	}

	return 0;
}
*/

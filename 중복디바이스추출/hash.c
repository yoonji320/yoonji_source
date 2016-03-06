
/// @file		hash.c
/// @brief		�ؽ� ���, �ؽð� ��꿡 �ʿ��� �������̽�
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
//			���� �������̽�
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		���ڰ� �Ҽ����� Ȯ���Ѵ�.
///
/// @return		0	: �Ҽ�
///				-1 	: �Ҽ��� �ƴ�
///
static int HashIsPrime(
					unsigned int num)			///< ����
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
/// @brief		�ؽ� ��Ʈ���� �����Ѵ�.
///
/// @return
///
/*static HashEntry* HashEntryCreate(
					void* 	pKey,			///< Ű
					int		nKeyLen,		///< Ű ����
					void*	pData,			///< ������
					int		nDataLen)		///< ������ ����
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

	// Ű�� ����/�����Ѵ�.
	if ((pEntry->pKey = malloc(nKeyLen)) == NULL)
	{
		debug();

		free(pEntry);

		return NULL;
	}

	memcpy(pEntry->pKey, pKey, nKeyLen);
	pEntry->nKeyLen = nKeyLen;

	// �����͸� ����/�����Ѵ�.
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

	// ������ ���̰� 0 �̶�� �ּҰ��� �����Ѵ�.
	else
	{
		pEntry->pData 		= pData;
		pEntry->nDataLen 	= 0;
	}

	return pEntry;
}
*/

///
/// @brief		�ؽ� ��Ʈ���� �޸𸮿��� �����Ѵ�.
///
/// @return
///

/*
 static void* HashEntryDestroy(
					HashEntry* 	pEntry)		///< �ؽ� ��Ʈ��
{
	void* pData;

	if (pEntry == NULL)
	{
		return NULL;
	}

	free(pEntry->pKey);

	// ���� �Ҵ��� �����ʹ� free() �Ѵ�.
	if (pEntry->nDataLen > 0 && pEntry->pData)
	{
		free(pEntry->pData);
		free(pEntry);

		return NULL;
	}

	// �ܺο��� �Ҵ��� �����ʹ� �����Ѵ�.
	pData = pEntry->pData;
	free(pEntry);

	return pData;
}
*/


/////////////////////////////////////////////////////////////////////////////
//
//
//			�ܺ� �������̽�
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		���ں��� ū ���� ���� �Ҽ��� ���Ѵ�.
///
/// @return		�Ҽ�	: ����
///
unsigned int HashGetPrime(
					unsigned int num)			///< ���ذ�
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
/// @brief		�ؽð��� ����Ѵ�.
///
/// @return		�ؽ� �ε��� 	: ����
///				-1 				: ����
///
inline int HashGetIndex(
					const char* value, 			///< �ؽð��� ����� �Է°� 
					unsigned int len, 			///< �Է°��� ����
					unsigned int num_slot)		///< �ؽ� ���̺��� ���Լ�
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
/// @brief		�ؽ� ���̺��� �����Ѵ�.
///
/// @return
///
/*HashTable* HashTableCreate(
					int	nNumEntry)				///< ��Ʈ�� ����
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
/// @brief		�ؽ� ���̺��� �޸𸮿��� �����Ѵ�.
///
/// @return
///
int HashTableDestroy(
					HashTable* 	pTable,		///< �ؽ� ���̺�
					DestroyFunc	pFree)		///< ������ ���� �Լ�
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
/// @brief		�ؽ� ���̺� ��Ʈ���� �����Ѵ�.
///
/// @return
///
void* HashTableInsert(
					HashTable*	pTable,		///< �ؽ� ���̺�
					void*		pKey,		///< Ű
					int			nKeyLen,	///< Ű ����
					void*		pData,		///< ������
					int			nDataLen)	///< ������ ����
{
	int 		nIndex;
	HashEntry*	pEntry;
	ListNode*	pNode;

	if (!pTable || !pTable->pSlot || !pKey || nKeyLen < 1 || !pData || nDataLen < 0)
	{
		debug();

		return NULL;
	}

	// �ؽ� ���̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex(pKey, nKeyLen, pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	// ������ ����� : �ش� ���Կ� �߰���
	if (pTable->pSlot[nIndex] == NULL)
	{
		// ��Ʈ�� ����Ʈ ����
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

	// ������ ��Ʈ���� �ִٸ� ����~	
	
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

	// �浹 : ����Ʈ�� �߰�
	
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
/// @brief		�ؽ� ���̺��� ��Ʈ���� �����Ѵ�.
///
/// @return
///
void* HashTableDelete(
					HashTable*	pTable,		///< �ؽ� ���̺�
					void*		pKey,		///< Ű
					int			nKeyLen)	///< Ű ����
{
	int 		nIndex;
	HashEntry* 	pEntry;
	ListNode*	pNode;

	if (!pTable || !pTable->pSlot || !pKey || nKeyLen < 1)
	{
		debug();

		return NULL;
	}

	// �ؽ� ���̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex(pKey, nKeyLen, pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	// ������ ����� : �ش� ��Ʈ���� �������� ����
	if (pTable->pSlot[nIndex] == NULL)
	{
		return NULL;
	}

	// ���� ����Ʈ �˻�
	
	pNode = pTable->pSlot[nIndex]->pHead;

	while (pNode)
	{
		pEntry = pNode->pData;
		
		// ��Ʈ�� �߰� : ����~
		if (pEntry->nKeyLen == nKeyLen && !memcmp(pEntry->pKey, pKey, nKeyLen))
		{
			ListDeleteNode(pTable->pSlot[nIndex], pNode);
			pTable->nNumEntry--;
			
			return (HashEntryDestroy(pEntry));
		}

		pNode = pNode->pNext;
	}

	// ��Ʈ�� ����
	return NULL;
}

///
/// @brief		�ؽ� ���̺��� ��Ʈ���� ã�´�.
///
/// @return
///
void* HashTableSearch(
					HashTable*	pTable,		///< �ؽ� ���̺�
					void*		pKey,		///< Ű
					int			nKeyLen)	///< Ű ����
{
	int 		nIndex;
	HashEntry* 	pEntry;
	ListNode*	pNode;

	if (!pTable || !pTable->pSlot || !pKey || nKeyLen < 1)
	{
		debug();

		return NULL;
	}

	// �ؽ� ���̺� �ε����� ���´�.
	if ((nIndex = HashGetIndex(pKey, nKeyLen, pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}

	// ������ ����� : �ش� ��Ʈ���� �������� ����
	if (pTable->pSlot[nIndex] == NULL)
	{
		return NULL;
	}

	// ���� ����Ʈ �˻�
	
	pNode = pTable->pSlot[nIndex]->pHead;

	while (pNode)
	{
		pEntry = pNode->pData;
		
		// ��Ʈ�� �߰� : ����~
		if (pEntry->nKeyLen == nKeyLen && !memcmp(pEntry->pKey, pKey, nKeyLen))
		{
			return pEntry;
		}

		pNode = pNode->pNext;
	}

	// ��Ʈ�� ����
	return NULL;
}

///
/// @brief		�ؽ� ���̺��� ����Ѵ�.
///
/// @return
///
int HashTablePrint(
					HashTable* 	pTable,		///< �ؽ� ���̺�
					int			nFD,		///< ���� ��ũ����
					PrintFunc	pPrint)		///< ��� �Լ�
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

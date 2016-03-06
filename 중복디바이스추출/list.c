
/// @file		list.c
/// @brief		���� ��ũ�� ����Ʈ
/// @author		hansch
/// @date		2010-05

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include "debug.h"

#include "list.h"

/////////////////////////////////////////////////////////////////////////////
//
//
//			���� �������̽�
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		��带 �����Ѵ�.
///
/// @return
///
static ListNode* ListNodeCreate(
					void* 	pData,			///< ������
					int		nDataLen)		///< ������ ���� (0=�ּҰ��� ����)
{
	ListNode* pNode;

	if ((pNode = malloc(sizeof(*pNode))) == NULL)
	{
		debug();

		return NULL;
	}

	memset(pNode, 0, sizeof(*pNode));

	if (nDataLen > 0)
	{
		if ((pNode->pData = malloc(nDataLen)) == NULL)
		{
			debug();

			free(pNode);

			return NULL;
		}

		memcpy(pNode->pData, pData, nDataLen);
		pNode->nDataLen = nDataLen;
	}

	else
	{
		pNode->pData = pData;
	}

	return pNode;;
}

///
/// @brief		��带 �����Ѵ�.
///
/// @return
///
static void* ListNodeDestroy(
					ListNode* pNode)		///< ���
{
	void* pData;

	if (pNode == NULL)
	{
		debug();

		return NULL;
	}

	// ���� �Ҵ��� �����ʹ� free() �Ѵ�.
	if (pNode->nDataLen > 0 && pNode->pData)
	{
		free(pNode->pData);
		free(pNode);

		return NULL;
	}

	// �ܺο��� �Ҵ��� �����ʹ� �����Ѵ�.
	pData = pNode->pData;
	free(pNode);

	return pData;
}


/////////////////////////////////////////////////////////////////////////////
//
//
//			�ܺ� �������̽�
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		����Ʈ�� �����Ѵ�.
///
/// @return
///
List* ListCreate()
{
	List* pList;

	if ((pList = malloc(sizeof(*pList))) == NULL)
	{
		debug();

		return NULL;
	}

	memset(pList, 0, sizeof(*pList));

	return pList;
}

///
/// @brief		����Ʈ�� �����Ѵ�.
///
/// @return
///
int ListDestroy(
				List* 		pList,		///< ����Ʈ
				DataFree 	pFree)		///< ������ ���� �Լ�
{
	ListNode* pCur;
	ListNode* pNext;

	int i;

	if (pList == NULL)
	{
		return -1;
	}

	pCur = pList->pHead;

	for (i=0; i<pList->nNumNode && pCur; ++i)
	{
		void* pData;

		pNext = pCur->pNext;
		pData = ListNodeDestroy(pCur);

		// �ܺο��� �Ҵ��� �����͸� �����Ѵ�.
		if (pData && pFree)
		{
			pFree(pData);
		}

		pCur = pNext;
	}

	free(pList);

	return 0;
}

///
/// @brief		����Ʈ�� ����Ѵ�.
///
/// @return
///
int ListPrint(
				List* pList)			///< ����Ʈ
{
	ListNode* pNode;

	int i;

	if (pList == NULL)
	{
		debug();

		return -1;
	}

	pNode = pList->pHead;

	for (i=0; i<pList->nNumNode && pNode; ++i)
	{
		printf("%3d. %s\n", i+1, (char*)pNode->pData);
		pNode = pNode->pNext;
	}

	return 0;
}

///
/// @brief		����Ʈ�� ��带 �߰��Ѵ�.
///
/// @return
///
ListNode* ListInsert(
				List* 	pList,			///< ����Ʈ
				void* 	pData,			///< ������
				int		nDataLen)		///< ������ ���� (0=�ּҰ��� ����)
{
	ListNode* pNode;

	if (pList == NULL)
	{
		debug();

		return NULL;
	}

	if ((pNode = ListNodeCreate(pData, nDataLen)) == NULL)
	{
		debug();

		return NULL;
	}

	// ����Ʈ�� ����� ���
	if (pList->pHead == NULL && pList->pTail == NULL)
	{
		pList->pHead = pNode;
		pList->pTail = pNode;
	}

	else
	{
		pNode->pPrev = pList->pTail;
		pList->pTail->pNext = pNode;
		pList->pTail = pNode;
	}

	pList->nNumNode++;

	return pNode;
}

///
/// @brief		����Ʈ���� ù��° ��带 �����Ѵ�.
///
/// @return
///
void* ListDeleteHead(
				List*	pList)			///< ����Ʈ
{
	ListNode* 	pNode;

	if (!pList)
	{
		debug();

		return NULL;
	}

	pNode = pList->pHead;

	// ����Ʈ�� ��尡 ����
	if (pNode == NULL)
	{
		return NULL;
	}

	// ��尡 1���� ���
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}

	// ��尡 2�� �̻��� ���
	else
	{
		pList->pHead 		= pNode->pNext;
		pList->pHead->pPrev = NULL;
	}

	// ��带 �޸𸮿��� �����Ѵ�.
	return (ListNodeDestroy(pNode));
}

///
/// @brief		����Ʈ���� ������ ��带 �����Ѵ�.
///
/// @return
///
void* ListDeleteTail(
				List*	pList)			///< ����Ʈ
{
	ListNode* 	pNode;

	if (!pList)
	{
		debug();

		return NULL;
	}

	pNode = pList->pTail;
	
	// ����Ʈ�� ��尡 ����
	if (pNode == NULL)
	{
		return NULL;
	}

	// ��尡 1���� ���
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}
	
	// ��尡 2�� �̻��� ���
	else
	{
		pList->pTail 		= pNode->pPrev;
		pList->pTail->pNext = NULL;
	}

	// ��带 �޸𸮿��� �����Ѵ�.
	return (ListNodeDestroy(pNode));
}

///
/// @brief		����Ʈ���� Ư�� ��带 �����Ѵ�.
///
/// @return
///
void* ListDeleteNode(
				List* 		pList,		///< ����Ʈ
				ListNode*	pNode)		///< ������ ���
{
	if (!pList || !pNode)
	{
		debug();

		return NULL;
	}

	// ù��° ����� ���
	if (pNode == pList->pHead)
	{
		return (ListDeleteHead(pList));
	}

	// ������ ����� ���
	else if (pNode == pList->pTail)
	{
		return (ListDeleteTail(pList));
	}

	// �� �� ���
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;

	// ��带 �޸𸮿��� �����Ѵ�.
	return (ListNodeDestroy(pNode));
}



/// @file		list.c
/// @brief		범용 링크드 리스트
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
//			내부 인터페이스
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		노드를 생성한다.
///
/// @return
///
static ListNode* ListNodeCreate(
					void* 	pData,			///< 데이터
					int		nDataLen)		///< 데이터 길이 (0=주소값만 복사)
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
/// @brief		노드를 해제한다.
///
/// @return
///
static void* ListNodeDestroy(
					ListNode* pNode)		///< 노드
{
	void* pData;

	if (pNode == NULL)
	{
		debug();

		return NULL;
	}

	// 직접 할당한 데이터는 free() 한다.
	if (pNode->nDataLen > 0 && pNode->pData)
	{
		free(pNode->pData);
		free(pNode);

		return NULL;
	}

	// 외부에서 할당한 데이터는 리턴한다.
	pData = pNode->pData;
	free(pNode);

	return pData;
}


/////////////////////////////////////////////////////////////////////////////
//
//
//			외부 인터페이스
//
//
/////////////////////////////////////////////////////////////////////////////

///
/// @brief		리스트를 생성한다.
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
/// @brief		리스트를 해제한다.
///
/// @return
///
int ListDestroy(
				List* 		pList,		///< 리스트
				DataFree 	pFree)		///< 데이터 해제 함수
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

		// 외부에서 할당한 데이터를 해제한다.
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
/// @brief		리스트를 출력한다.
///
/// @return
///
int ListPrint(
				List* pList)			///< 리스트
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
/// @brief		리스트에 노드를 추가한다.
///
/// @return
///
ListNode* ListInsert(
				List* 	pList,			///< 리스트
				void* 	pData,			///< 데이터
				int		nDataLen)		///< 데이터 길이 (0=주소값만 복사)
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

	// 리스트가 비었을 경우
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
/// @brief		리스트에서 첫번째 노드를 삭제한다.
///
/// @return
///
void* ListDeleteHead(
				List*	pList)			///< 리스트
{
	ListNode* 	pNode;

	if (!pList)
	{
		debug();

		return NULL;
	}

	pNode = pList->pHead;

	// 리스트에 노드가 없음
	if (pNode == NULL)
	{
		return NULL;
	}

	// 노드가 1개인 경우
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}

	// 노드가 2개 이상인 경우
	else
	{
		pList->pHead 		= pNode->pNext;
		pList->pHead->pPrev = NULL;
	}

	// 노드를 메모리에서 해제한다.
	return (ListNodeDestroy(pNode));
}

///
/// @brief		리스트에서 마지막 노드를 삭제한다.
///
/// @return
///
void* ListDeleteTail(
				List*	pList)			///< 리스트
{
	ListNode* 	pNode;

	if (!pList)
	{
		debug();

		return NULL;
	}

	pNode = pList->pTail;
	
	// 리스트에 노드가 없음
	if (pNode == NULL)
	{
		return NULL;
	}

	// 노드가 1개인 경우
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}
	
	// 노드가 2개 이상인 경우
	else
	{
		pList->pTail 		= pNode->pPrev;
		pList->pTail->pNext = NULL;
	}

	// 노드를 메모리에서 해제한다.
	return (ListNodeDestroy(pNode));
}

///
/// @brief		리스트에서 특정 노드를 삭제한다.
///
/// @return
///
void* ListDeleteNode(
				List* 		pList,		///< 리스트
				ListNode*	pNode)		///< 삭제할 노드
{
	if (!pList || !pNode)
	{
		debug();

		return NULL;
	}

	// 첫번째 노드인 경우
	if (pNode == pList->pHead)
	{
		return (ListDeleteHead(pList));
	}

	// 마지막 노드인 경우
	else if (pNode == pList->pTail)
	{
		return (ListDeleteTail(pList));
	}

	// 그 외 경우
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;

	// 노드를 메모리에서 해제한다.
	return (ListNodeDestroy(pNode));
}


/// @file		list.c
/// @brief		리스트
/// @author		yoonzz320
/// @date		2015-10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

#include "list.h"


// 노드 생성
static ListNode* ListNodeCreate(
					void*	pData,		// 데이터
					int		nDataLen)	// 데이터 길이
{
	ListNode* pNode = NULL;

	if ((pNode = malloc(sizeof(*pNode))) == NULL )
	{
		debug();

		return NULL;
	}

	// pNode 초기화
	memset(pNode, 0, sizeof(*pNode));

	if (nDataLen > 0 )
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
	
	// 데이터 길이가 0이면, 주소값만 복사
	else 
	{
		pNode->pData = pData;
	}

	return pNode;
}

// 노드 해제
static void* ListNodeDestroy(
					ListNode*	pNode)
{
	void* pData = NULL;

	if (pNode == NULL )
	{
		debug();

		return NULL;
	}

	// 직접 할당한 데이터는 free
	if (pNode->nDataLen > 0 )
	{
		free(pNode->pData);
		free(pNode);

		return NULL;
	}
	
	// 외부에서 할당한 데이터는 리턴한다.
	pData = pNode->pData; // 내부 변수에 담는다.
	free(pNode);

	return pData;	// 데이터 리턴
}

// 리스트 생성
List* ListCreate ()
{
	List* pList = NULL;
	
	// 메모리할당
	if ((pList = malloc(sizeof(*pList))) == NULL )
	{
		debug();

		return NULL;
	}

	memset(pList, 0, sizeof(*pList));

	return pList;
}

// 리스트 해제
int ListDestroy (
					List* 		pList,
					DataFree	pFree)	// 데이터 해제 함수
{
	ListNode*	pCur = NULL;
	ListNode*	pNext = NULL;

	int i;
	
	if (pList == NULL)
	{
		debug();

		return -1;
	}
	
	pCur = pList->pHead;
	
	// 리스트 내부의 모든 노드들을 제거
	for (i=0; i<pList->nNumNode && pCur; ++i)
	{
		void* pData;

		// pCur 을 해제해버리면, next를 알 수 없으므로 pNext를 미리 셋팅
		pNext = pCur->pNext;

		// 노드해제 함수의 return 값을 내부변수로 받아서 해제한다.
		pData = ListNodeDestroy(pCur);

		// 외부에서 할당한 데이터 해제
		if (pData && pFree)
		{
			pFree(pData);
		}

		pCur = pNext; // 다음노드가 현재노드가 되도록셋팅
	}
	// 리스트를 해제한다.
	free(pList);

	return 0;
}

// 리스트에 노드 추가
ListNode* ListInsert(
					List*	pList,		// 리스트
					void*	pData,		// 데이터
					int		nDataLen)	// 데이터 길이
{
	ListNode* pNode = NULL;

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

	//리스트가 비었을 경우
	if (pList->pHead == NULL && pList->pTail == NULL)
	{
		pList->pHead = pNode;
		pList->pTail = pNode;
	}

	else
	{
		pNode->pPrev = pList->pTail;
		pList->pTail->pNext= pNode;
		pList->pTail = pNode;
	}

	//추가했으므로 노드갯수 증가
	pList->nNumNode++;

	return pNode;
}

// 리스트에 첫번째 노드 삭제
void* ListDeleteHead(
					List*	pList)
{
	ListNode* pNode = NULL;

	if (!pList)
	{
		debug();
		return NULL;
	}

	pNode = pList->pHead;

	// 리스트에 노드가 없다면,
	if (pNode == NULL)
	{
		return NULL;
	}

	// 노드가 1개이면,
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}

	// 노드가 2개이상이면,
	else
	//if (pList->nNumNode > 1)
	{
		pList->pHead 		= pNode->pNext;
		pList->pHead->pPrev = NULL;
	}

	pList->nNumNode--;

	//노드를 메모리에서 해제
	return (ListNodeDestroy(pNode));
}

//리스트에 마지막 노드 삭제
void* ListDeleteTail(
					List*	pList)
{
	ListNode* pNode = NULL;

	if (!pList)
	{
		debug();
		return NULL;
	}
	
	pNode = pList->pTail;
	
	//리스트에 노드가 없다면,
	if (pNode == NULL)
	{
		return NULL;
	}

	//노드가 1개이면,
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}

	//노드가 2개 이상이면,
	else 
	{
		pList->pTail		= pNode->pPrev;
		pList->pTail->pNext	= NULL;
	}

	pList->nNumNode--;
	
	//노드를 메모리에서 해제
	return (ListNodeDestroy(pNode));
}
					

//리스트에 특정  노드 삭제
void* ListDeleteNode(
					List*		pList,
					ListNode*	pNode)
{
	if (!pList || !pNode)
	{
		debug();
		return NULL;
	}

	//첫번째 노드인 경우
	if (pNode == pList->pHead)
	{
		return (ListDeleteHead(pList));	
	}

	//마지막 노드인 경우
	if (pNode  == pList->pTail)
	{	
		return (ListDeleteTail(pList));
	}	
	
	//그 외의 경우
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;

	pList->nNumNode--;

	//노드 해제
	return (ListNodeDestroy(pNode));
}

//리스트 프린트
int ListPrint(
					List* pList)
{
	ListNode* pNode = NULL;
	int i = 0;

	if (pList == NULL)
	{
		debug();
		return -1;
	}

	pNode = pList->pHead;

	for (i=0; i<pList->nNumNode && pNode; ++i)
	{
		printf("%s\n", (char*)pNode->pData);
		pNode = pNode->pNext;
	}

	return 0;
}

/*
int main()
{
	List* pList;
//	DataFree pFree = NULL;

	char sBuf1[] = "ab";
	char sBuf2[] = "cc";
	char sBuf3[] = "dd";
	char sBuf4[] = "ee";

	pList = ListCreate();

	ListInsert(pList, sBuf1, sizeof(sBuf1));
	ListInsert(pList, sBuf2, sizeof(sBuf2));
	ListInsert(pList, sBuf3, sizeof(sBuf3));
	ListInsert(pList, sBuf4, sizeof(sBuf4));

	ListDeleteNode(pList, pList->pHead->pNext->pNext);
	//ListDeleteHead(pList);
	//ListDeleteTail(pList);

	ListPrint(pList);
	
	ListDestroy(pList, NULL);

	printf("%lu\n", sizeof(sBuf1));

	return 0;
}
*/

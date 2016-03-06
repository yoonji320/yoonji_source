///	@file		app.c
///	@brief		앱 리스트
/// @author		yoonzz320
///	@date		2015-12-16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "app.h"
#include "net.h"

// AppList Create함수
AppList* AppListCreate()
{
	AppList* pAppList = NULL;

	if ((pAppList = malloc(sizeof(*pAppList))) == NULL)
	{
		debug();

		return NULL;
	}
	
	memset(pAppList, 0, sizeof(*pAppList));

	return pAppList;
}

// AppNode Destroy 함수
static int AppNodeDestroy(
				AppNode* pAppNode)
{
	if (pAppNode == NULL)
	{
		debug();

		return -1;
	}

	// NetListDestroy함수 호출
	NetListDestroy(pAppNode->pNetList);
	
	free(pAppNode);

	return 0;
}

// AppList Destroy 함수
int AppListDestroy(
					AppList* pAppList)
{
	AppNode* pCurNode = NULL;
	AppNode* pNextNode = NULL;

	int i = 0;

	if (!pAppList)
	{
		debug();

		return -1;
	}

	pCurNode = pAppList->pHead;

	// 리스트의 노드를 돌면서 해제시킴
	//while (pCurNode)
	for (i=0; i<pAppList->nNumNode; ++i)
	{
		pNextNode = pCurNode->pNext;
	
		// debug("AppID=%d,NetList=%p", pCurNode->nAppID, pCurNode->pNetList);

		// AppNodeDestroy 호출
		AppNodeDestroy(pCurNode);
	
		pCurNode = pNextNode;
	}

	free(pAppList);

	return 0;
}

// AppNode Create 함수
AppNode* AppNodeCreate(
					AppNode* pSrcNode)
{
	AppNode* pNode = NULL;

	if (!pSrcNode)
	{
		debug();

		return NULL;
	}

	if ((pNode = malloc(sizeof(*pNode))) == NULL)
	{
		debug();

		return NULL;
	}

	memcpy(pNode, pSrcNode, sizeof(*pNode));
	
	pNode->pPrev 	= NULL;
	pNode->pNext 	= NULL;
	pNode->pNetList = NetListCreate();

	return pNode;
}

// App리스트 insert
AppNode* AppListInsert(
					AppList* pAppList,
					AppNode* pSrcNode,
					int*	 pFlag)
{
	AppNode* pNode = NULL;
	
	int i;
	
	// 유효성 검사
	if (!pAppList || !pSrcNode || !pFlag)
	{
		debug();

		return NULL;
	}

	pNode = pAppList->pHead;

	for (i=0; i<pAppList->nNumNode; ++i)
	{
		if (pNode->nAppID == pSrcNode->nAppID)
		{
			*pFlag = 0;

			return pNode;
		}
		
		pNode = pNode->pNext;
	}
	
	// AppNode 생성(메모리 할당)
	if ((pNode = AppNodeCreate(pSrcNode)) == NULL)
	{
		debug();

		return NULL;
	}
	
	*pFlag = 1;
	
	//리스트가 비어있는 경우
	if (pAppList->pHead == NULL && pAppList->pTail == NULL)
	{
		pAppList->pHead = pNode;
		pAppList->pTail = pNode;
	}

	else // 리스트에 값이 있지만 AppID가 다른 경우에 해당됨..
	{
		pNode->pPrev = pAppList->pTail;
		pAppList->pTail->pNext = pNode;
		pAppList->pTail = pNode;
	}

	//노드의 갯수 증가
	pAppList->nNumNode++;

	return pNode;
}

// AppList 프린트 함수
int AppListPrint(
					AppList* pAppList)
{
	AppNode* pAppNode = NULL;
	int i=0;

	if (pAppList == NULL)
	{
		debug();
		return -1;
	}

	pAppNode = pAppList->pHead;

	//for (i=0; i<pAppList->nNumNode && pAppNode; ++i)
	for (i=0; pAppNode; ++i)
	{
		printf("AppID: %d,", pAppNode->nAppID);

		if (pAppNode->pNetList)
		{
			NetListPrint(pAppNode->pNetList);
		}

		pAppNode = pAppNode->pNext;
	}

	return 0;
}

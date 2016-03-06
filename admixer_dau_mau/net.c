///	@file		net.c
///	@brief		네트워크 리스트
///	@author		yoonzz320
///	@date		2015-12-16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "net.h"
#include "app.h"

// NetList Create함수
NetList*	NetListCreate()
{
	NetList* pNetList = NULL;

	if ((pNetList = malloc(sizeof(*pNetList))) == NULL)
	{
		debug();

		return NULL;
	}

	memset(pNetList, 0, sizeof(*pNetList));

	return pNetList;
}

// NetNode Create 함수
NetNode* NetNodeCreate(
					NetNode* pSrcNode)
{

	NetNode* pNode = NULL;
	
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

	pNode->pPrev = NULL;
	pNode->pNext = NULL;

	return pNode;
}

// NetNode Destroy 함수
static void* NetNodeDestroy(
					NetNode* pNetNode)
{
	if (pNetNode == NULL)
	{
		debug();

		return NULL;
	}
	
	free(pNetNode);

	return NULL;
}

// NetList Destroy 함수
int NetListDestroy(
					NetList*	pNetList)
{
	NetNode* pCurNode = NULL;
	NetNode* pNextNode = NULL;

	int i = 0;

	if (!pNetList)
	{
		debug();

		return -1;
	}

	pCurNode = pNetList->pHead;

	//while (pCurNode)
	for (i=0; i<pNetList->nNumNode; ++i)
	{
		pNextNode = pCurNode->pNext;

		NetNodeDestroy(pCurNode);

		pCurNode = pNextNode;
	}

	free(pNetList);	

	return 0;
}

// Net리스트 insert
NetNode* NetListInsert(
					NetList* pNetList,
					NetNode* pSrcNode,
					int*	 pFlag)
{
	int i = 0;
	NetNode* pNode = NULL;

	//유효성 검사
	if (!pNetList || !pSrcNode || !pFlag)
	{
		debug();

		return NULL;
	}

	pNode = pNetList->pHead;

	//while (pNode)
	for (i=0; i<pNetList->nNumNode; ++i)
	{
		if (pNode->nNetID == pSrcNode->nNetID && pNode->nAdsType == pSrcNode->nAdsType)
		{
			*pFlag = 0;

			return pNode;
		}

		pNode = pNode->pNext;
	}

	// NetNode 생성(메모리 할당)
	if ((pNode = NetNodeCreate(pSrcNode)) == NULL)
	{
		debug();

		return NULL;
	}

	*pFlag = 1;

	//리스트가 비어있는 경우
	if (pNetList->pHead == NULL && pNetList->pTail == NULL)
	{
		pNetList->pHead = pNode;
		pNetList->pTail = pNode;
	}
	
	else
	{
		pNode->pPrev = pNetList->pTail;
		pNetList->pTail->pNext = pNode;
		pNetList->pTail = pNode;
	}

	//노드의 갯수 증가
	pNetList->nNumNode++;

	return pNode;
}

// NetList 프린트 함수
int NetListPrint(
					NetList* pNetList)
{
	NetNode* pNetNode = NULL;
	int i=0;

	if (pNetList == NULL)
	{
		debug();
		return -1;
	}

	pNetNode = pNetList->pHead;

	for (i=0; i<pNetList->nNumNode && pNetNode; ++i)
	{
		printf("NetID: %d, AdsType: %d", pNetNode->nNetID, pNetNode->nAdsType);
		pNetNode = pNetNode->pNext;
	}
		
	return 0;
}

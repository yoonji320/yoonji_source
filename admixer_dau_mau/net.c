///	@file		net.c
///	@brief		��Ʈ��ũ ����Ʈ
///	@author		yoonzz320
///	@date		2015-12-16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "net.h"
#include "app.h"

// NetList Create�Լ�
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

// NetNode Create �Լ�
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

// NetNode Destroy �Լ�
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

// NetList Destroy �Լ�
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

// Net����Ʈ insert
NetNode* NetListInsert(
					NetList* pNetList,
					NetNode* pSrcNode,
					int*	 pFlag)
{
	int i = 0;
	NetNode* pNode = NULL;

	//��ȿ�� �˻�
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

	// NetNode ����(�޸� �Ҵ�)
	if ((pNode = NetNodeCreate(pSrcNode)) == NULL)
	{
		debug();

		return NULL;
	}

	*pFlag = 1;

	//����Ʈ�� ����ִ� ���
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

	//����� ���� ����
	pNetList->nNumNode++;

	return pNode;
}

// NetList ����Ʈ �Լ�
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

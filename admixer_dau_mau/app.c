///	@file		app.c
///	@brief		�� ����Ʈ
/// @author		yoonzz320
///	@date		2015-12-16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "app.h"
#include "net.h"

// AppList Create�Լ�
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

// AppNode Destroy �Լ�
static int AppNodeDestroy(
				AppNode* pAppNode)
{
	if (pAppNode == NULL)
	{
		debug();

		return -1;
	}

	// NetListDestroy�Լ� ȣ��
	NetListDestroy(pAppNode->pNetList);
	
	free(pAppNode);

	return 0;
}

// AppList Destroy �Լ�
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

	// ����Ʈ�� ��带 ���鼭 ������Ŵ
	//while (pCurNode)
	for (i=0; i<pAppList->nNumNode; ++i)
	{
		pNextNode = pCurNode->pNext;
	
		// debug("AppID=%d,NetList=%p", pCurNode->nAppID, pCurNode->pNetList);

		// AppNodeDestroy ȣ��
		AppNodeDestroy(pCurNode);
	
		pCurNode = pNextNode;
	}

	free(pAppList);

	return 0;
}

// AppNode Create �Լ�
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

// App����Ʈ insert
AppNode* AppListInsert(
					AppList* pAppList,
					AppNode* pSrcNode,
					int*	 pFlag)
{
	AppNode* pNode = NULL;
	
	int i;
	
	// ��ȿ�� �˻�
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
	
	// AppNode ����(�޸� �Ҵ�)
	if ((pNode = AppNodeCreate(pSrcNode)) == NULL)
	{
		debug();

		return NULL;
	}
	
	*pFlag = 1;
	
	//����Ʈ�� ����ִ� ���
	if (pAppList->pHead == NULL && pAppList->pTail == NULL)
	{
		pAppList->pHead = pNode;
		pAppList->pTail = pNode;
	}

	else // ����Ʈ�� ���� ������ AppID�� �ٸ� ��쿡 �ش��..
	{
		pNode->pPrev = pAppList->pTail;
		pAppList->pTail->pNext = pNode;
		pAppList->pTail = pNode;
	}

	//����� ���� ����
	pAppList->nNumNode++;

	return pNode;
}

// AppList ����Ʈ �Լ�
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

/// @file		list.c
/// @brief		����Ʈ
/// @author		yoonzz320
/// @date		2015-10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

#include "list.h"


// ��� ����
static ListNode* ListNodeCreate(
					void*	pData,		// ������
					int		nDataLen)	// ������ ����
{
	ListNode* pNode = NULL;

	if ((pNode = malloc(sizeof(*pNode))) == NULL )
	{
		debug();

		return NULL;
	}

	// pNode �ʱ�ȭ
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
	
	// ������ ���̰� 0�̸�, �ּҰ��� ����
	else 
	{
		pNode->pData = pData;
	}

	return pNode;
}

// ��� ����
static void* ListNodeDestroy(
					ListNode*	pNode)
{
	void* pData = NULL;

	if (pNode == NULL )
	{
		debug();

		return NULL;
	}

	// ���� �Ҵ��� �����ʹ� free
	if (pNode->nDataLen > 0 )
	{
		free(pNode->pData);
		free(pNode);

		return NULL;
	}
	
	// �ܺο��� �Ҵ��� �����ʹ� �����Ѵ�.
	pData = pNode->pData; // ���� ������ ��´�.
	free(pNode);

	return pData;	// ������ ����
}

// ����Ʈ ����
List* ListCreate ()
{
	List* pList = NULL;
	
	// �޸��Ҵ�
	if ((pList = malloc(sizeof(*pList))) == NULL )
	{
		debug();

		return NULL;
	}

	memset(pList, 0, sizeof(*pList));

	return pList;
}

// ����Ʈ ����
int ListDestroy (
					List* 		pList,
					DataFree	pFree)	// ������ ���� �Լ�
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
	
	// ����Ʈ ������ ��� ������ ����
	for (i=0; i<pList->nNumNode && pCur; ++i)
	{
		void* pData;

		// pCur �� �����ع�����, next�� �� �� �����Ƿ� pNext�� �̸� ����
		pNext = pCur->pNext;

		// ������� �Լ��� return ���� ���κ����� �޾Ƽ� �����Ѵ�.
		pData = ListNodeDestroy(pCur);

		// �ܺο��� �Ҵ��� ������ ����
		if (pData && pFree)
		{
			pFree(pData);
		}

		pCur = pNext; // ������尡 �����尡 �ǵ��ϼ���
	}
	// ����Ʈ�� �����Ѵ�.
	free(pList);

	return 0;
}

// ����Ʈ�� ��� �߰�
ListNode* ListInsert(
					List*	pList,		// ����Ʈ
					void*	pData,		// ������
					int		nDataLen)	// ������ ����
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

	//����Ʈ�� ����� ���
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

	//�߰������Ƿ� ��尹�� ����
	pList->nNumNode++;

	return pNode;
}

// ����Ʈ�� ù��° ��� ����
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

	// ����Ʈ�� ��尡 ���ٸ�,
	if (pNode == NULL)
	{
		return NULL;
	}

	// ��尡 1���̸�,
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}

	// ��尡 2���̻��̸�,
	else
	//if (pList->nNumNode > 1)
	{
		pList->pHead 		= pNode->pNext;
		pList->pHead->pPrev = NULL;
	}

	pList->nNumNode--;

	//��带 �޸𸮿��� ����
	return (ListNodeDestroy(pNode));
}

//����Ʈ�� ������ ��� ����
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
	
	//����Ʈ�� ��尡 ���ٸ�,
	if (pNode == NULL)
	{
		return NULL;
	}

	//��尡 1���̸�,
	if (pList->pHead == pList->pTail)
	{
		pList->pHead = NULL;
		pList->pTail = NULL;
	}

	//��尡 2�� �̻��̸�,
	else 
	{
		pList->pTail		= pNode->pPrev;
		pList->pTail->pNext	= NULL;
	}

	pList->nNumNode--;
	
	//��带 �޸𸮿��� ����
	return (ListNodeDestroy(pNode));
}
					

//����Ʈ�� Ư��  ��� ����
void* ListDeleteNode(
					List*		pList,
					ListNode*	pNode)
{
	if (!pList || !pNode)
	{
		debug();
		return NULL;
	}

	//ù��° ����� ���
	if (pNode == pList->pHead)
	{
		return (ListDeleteHead(pList));	
	}

	//������ ����� ���
	if (pNode  == pList->pTail)
	{	
		return (ListDeleteTail(pList));
	}	
	
	//�� ���� ���
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;

	pList->nNumNode--;

	//��� ����
	return (ListNodeDestroy(pNode));
}

//����Ʈ ����Ʈ
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

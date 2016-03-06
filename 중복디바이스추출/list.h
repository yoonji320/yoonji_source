
/// @file		list.h
/// @brief		���� ��ũ�� ����Ʈ
/// @author		yoonzz320
/// @date		2015-05

#ifndef __LIST_H
#define __LIST_H

typedef int (*DataFree)(void*);

typedef struct ListNode ListNode;
typedef struct List List;

/// ����Ʈ ���
struct ListNode
{
	ListNode*	pPrev;			///< ���� ���
	ListNode* 	pNext;			///< ���� ���
	void*		pData;			///< ������
	int			nDataLen;		///< ������ ���� (0=�ּҰ��� ����)
};

/// ����Ʈ
struct List
{
	int			nNumNode;		///< ��� ����
	ListNode*	pHead;			///< ���� ���
	ListNode*	pTail;			///< ������ ���
};

extern List* ListCreate();

extern int ListDestroy(
				List* 		pList,		///< ����Ʈ
				DataFree 	pFree);		///< ������ ���� �Լ�

extern int ListPrint(
				List* pList);			///< ����Ʈ

extern ListNode* ListInsert(
				List* 	pList,			///< ����Ʈ
				void* 	pData,			///< ������
				int		nDataLen);		///< ������ ���� (0=�ּҰ��� ����)

extern void* ListDeleteHead(
				List*	pList);			///< ����Ʈ

extern void* ListDeleteTail(
				List*	pList);			///< ����Ʈ

extern void* ListDeleteNode(
				List* 		pList,		///< ����Ʈ
				ListNode*	pNode);		///< ������ ���

#endif	// __LIST_H


/// @file	list.h
/// @brief	¸®½ºÆ®
///	@author	yoonzz320
///	@date	2015-10

#ifndef __LIST_H
#define	__LIST_H


typedef int (*DataFree)(void*);

typedef struct AppNode
{
	int 	nAppID;
	List*	pNetNode;
	struct AppNode* pPrev;
	struct AppNode* pNext;
} AppNode;

typedef struct NetNode
{
	int		nNetID;
	int		nAdsType;
	struct	NetNode* pPrev;
	struct 	NetNode* pNext;
} NetNode;

typedef struct ListNode
{
	struct ListNode* pPrev;
	struct ListNode* pNext;
	void* 			 pData;
	int				 nDataLen;
} ListNode;

typedef struct List
{
	int				nNumNode;
	ListNode* 		pHead;
	ListNode* 		pTail;
} List;

extern List* ListCreate();

extern int ListDestroy(
					List*		pList,
					DataFree	pFree);

extern ListNode* ListInsert(
					List*	pList,
					void*	pData,
					int		nDataLen);

extern void* ListDeleteHead(
					List*	pList);

extern void* ListDeleteTail(
					List*	pList);

extern void* ListDeleteNode(
					List*		pList,
					ListNode*	pNode);

extern int ListPrint(
				List*	pList);


#endif	// __LIST_H


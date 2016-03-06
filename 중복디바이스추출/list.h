
/// @file		list.h
/// @brief		범용 링크드 리스트
/// @author		yoonzz320
/// @date		2015-05

#ifndef __LIST_H
#define __LIST_H

typedef int (*DataFree)(void*);

typedef struct ListNode ListNode;
typedef struct List List;

/// 리스트 노드
struct ListNode
{
	ListNode*	pPrev;			///< 이전 노드
	ListNode* 	pNext;			///< 다음 노드
	void*		pData;			///< 데이터
	int			nDataLen;		///< 데이터 길이 (0=주소값만 복사)
};

/// 리스트
struct List
{
	int			nNumNode;		///< 노드 개수
	ListNode*	pHead;			///< 시작 노드
	ListNode*	pTail;			///< 마지막 노드
};

extern List* ListCreate();

extern int ListDestroy(
				List* 		pList,		///< 리스트
				DataFree 	pFree);		///< 데이터 해제 함수

extern int ListPrint(
				List* pList);			///< 리스트

extern ListNode* ListInsert(
				List* 	pList,			///< 리스트
				void* 	pData,			///< 데이터
				int		nDataLen);		///< 데이터 길이 (0=주소값만 복사)

extern void* ListDeleteHead(
				List*	pList);			///< 리스트

extern void* ListDeleteTail(
				List*	pList);			///< 리스트

extern void* ListDeleteNode(
				List* 		pList,		///< 리스트
				ListNode*	pNode);		///< 삭제할 노드

#endif	// __LIST_H


/// @file	net.h
/// @brief	³Ý
/// @author	yoonzz320
///	@date	2015-12-11

#ifndef __NET_H
#define __NET_H

typedef struct NetNode NetNode;

struct NetNode
{
	NetNode*	pPrev;
	NetNode*	pNext;
	int			nNetID;
	int			nAdsType;
};

typedef struct NetList
{
	int			nNumNode;
	NetNode*	pHead;
	NetNode*	pTail;
} NetList;

extern NetList* NetListCreate();

extern NetNode* NetNodeCreate(
					NetNode* pSrcNode);

extern int NetListDestroy(
					NetList* pNetList);

extern NetNode* NetListInsert(
					NetList* pNetList,
					NetNode* pSrcNode,
					int*	 pFlag);

extern int NetListPrint(
			NetList* pNetList);
#endif	//_NET_H

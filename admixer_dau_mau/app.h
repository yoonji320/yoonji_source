/// @file	app.h
/// @brief 	¾Û
/// @author yoonzz320
/// @date	2015-12-11

#ifndef __APP_H
#define __APP_H

#include "net.h"

//typedef int (*DataFree)(void*);

typedef struct AppNode
{
	struct AppNode* pPrev;
	struct AppNode* pNext;
	int 		nAppID;
	NetList*	pNetList;
} AppNode;

typedef struct AppList
{
	int			nNumNode;
	AppNode*	pHead;
	AppNode*	pTail;
} AppList;

extern AppList* AppListCreate();

extern int AppListDestroy(
					AppList* pAppList);

extern AppNode* AppNodeCreate(
			AppNode* pSrcNode);

extern AppNode* AppListInsert(
			AppList* pAppList,
			AppNode* pSrcNode,
			int*	 pFlag);

extern int AppListPrint(
			AppList* pAppList);

#endif // __APP_H

/// @file	dev.h
/// @brief	디바이스 엔트리
/// @author yoonzz320
/// @date	2015-12-11

#ifndef __DEV_H
#define __DEV_H

#include "app.h"
#include "net.h"

typedef struct DevEntry
{
	char	 	sDevID[64];
	AppList*	pAppList;
	NetList*	pNetList;
} DevEntry;

typedef struct DevTable
{
	int			nNumSlot;
	int			nNumEntry;
	DevEntry**	pDevEntry;
} DevTable;

extern DevTable*	DevTableCreate(
					int nNumEntry);

extern int DevTableDestroy(
					DevTable* pDevTable);

extern DevEntry*	DevEntryCreate(
					DevEntry* pDevEntry);

extern DevEntry*	DevTableInsert(
					DevTable* pDevTable,
					DevEntry* pDevEntry,
					int*	  pFlag);

extern int DevTablePrint(
			DevTable*	pDevTable);

#endif // __DEV_H

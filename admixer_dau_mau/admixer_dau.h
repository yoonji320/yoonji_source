///	@file		admixer_dau.h
///	@brief		¾Öµå¹Í¼­ Daily Active User
///	@author		yoonzz320
///	@date		2015-12-17

#ifndef __ADMIXER_DAU_H
#define __ADMIXER_DAU_H
#include "dbman.h"
#include "dev.h"
#include "report.h"
#include "app.h"
#include "net.h"
#include "admixer_dau.h"

extern int Admixer_DauStart(
					int		nParam,
					char**	sParam);

extern int DauSet(
					char*		sFileName,
					char*		sOption,
					DevTable*	pDevTable,
					RptTable*	pRptTable);

extern int DBManStart(
					char*		sOption,
					RptTable*	pRptTable);

extern int AppListPrint(
					AppList*	pAppList);

extern int NetListPrint(
					NetList*	pNetList);

#endif // __ADMIXER_DAU_H

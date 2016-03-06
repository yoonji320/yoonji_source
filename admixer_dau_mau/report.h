/// @file	report.h
///	@brief	¸®Æ÷Æ®
///	@author	yoonzz320
///	@date	2015-12-11

#ifndef	__REPORT_H
#define	__REPORT_H

typedef struct RptEntry
{
	char sYmd[32];
	int nAppID;
	int nNetID;
	int nAdsType;
	int nDevType;
	int nActiveUser;
} RptEntry;

typedef struct RptTable
{
	int			nNumSlot;
	int			nNumEntry;
	RptEntry**	pRptEntry;
} RptTable;

extern RptTable* RptTableCreate(int nNumEntry);

extern int RptTableDestroy(
			RptTable* pRptTable);

extern RptEntry* RptEntryCreate(RptEntry* pRptEntry);

extern RptEntry* RptTableInsert(
					RptTable* pRptTable,
					RptEntry* pRptEntry);

extern int ReportCount(
					RptTable* pRptTable,
					RptEntry* pRptEntry);

#endif // __REPORT_H

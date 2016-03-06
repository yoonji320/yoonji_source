#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "admixer_dau.h"
#include "debug.h"
#include "dbman.h"
#include "dev.h"
#include "config.h"
#include "report.h"

#define BUFFER 1024

// 앱, 네트워크 별 count
int AppNetCount(
					RptTable*	pRptTable,
					AppNode*	pAppNode,
					char*		sYmd,
					int			nNetID,
					int			nAdsType,
					char*		sGaid,
					char*		sIdfa)
{
	int 		nFlag = 0;
	RptEntry	hRptEntry = {{0,},};
	NetNode		hNetNode  = {0,};
	NetNode*	pNetNode  = NULL;

	if (!pRptTable || !pAppNode)
	{
		debug();

		return -1;
	}

	if (!sYmd || nNetID < 1 || nAdsType < 1)
	{
		//debug("ymd = %s, NetID = %d, AdsType = %d", sYmd, nNetID, nAdsType);

		return -1;
	}

	// 파라미터로 받아온 nNetID, nAdsType 을 노드로 assign 한다.
	hNetNode.nNetID = nNetID;
	hNetNode.nAdsType = nAdsType;

	// print로 확인
	//NetListPrint(pAppNode->pNetList);

	// AppID 안에 있는 NetID 리스트에 노드 삽입하는 함수
	if ((pNetNode = NetListInsert(pAppNode->pNetList, &hNetNode, &nFlag)) == NULL)
	{
		debug();

		return -1;
	}

	if (nFlag == 1)
	{
		// 리포트 엔트리 셋팅

		strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
		hRptEntry.nAppID = pAppNode->nAppID;
		hRptEntry.nNetID = nNetID;
		hRptEntry.nAdsType = nAdsType;
		hRptEntry.nDevType = 0;

		// 리포트테이블에 insert
		if (ReportCount(pRptTable, &hRptEntry) < 0)
		{
			debug();

			return -1;
		}

		// 앱별, 네트워크 별 카운트(sGaid 에 값이 있으면, DevType 이 1인 경우)
		if (strcmp(sGaid, "null") != 0)
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = pAppNode->nAppID;
			hRptEntry.nNetID = nNetID;
			hRptEntry.nAdsType = nAdsType;
			hRptEntry.nDevType = 1;

			// 리포트테이블에 insert
			if (ReportCount(pRptTable, &hRptEntry) < 0)
			{
				debug();

				return -1;
			}
		}

		// 앱별, 네트워크 별 카운트(sIdfa 에 값이 있으면, DevType 이 2인 경우)
		else if (strcmp(sIdfa, "null") != 0)
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = pAppNode->nAppID;
			hRptEntry.nNetID = nNetID;
			hRptEntry.nAdsType = nAdsType;
			hRptEntry.nDevType = 2;

			// 리포트테이블에 insert
			if (ReportCount(pRptTable, &hRptEntry) < 0)
			{
				debug();

				return -1;
			}
		}

		// 앱별, 네트워크 별 카운트(둘 다 아니면, DevType 이 3인 경우)
		else
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = pAppNode->nAppID;
			hRptEntry.nNetID = nNetID;
			hRptEntry.nAdsType = nAdsType;
			hRptEntry.nDevType = 3;

			// 리포트테이블에 insert
			if (ReportCount(pRptTable, &hRptEntry) < 0)
			{
				debug();

				return -1;
			}
		}
	}

	return 0;
}

// 앱 단위 별 카운트
int AppCount(
					RptTable*	pRptTable,
					DevEntry*	pDevEntry,
					char*		sYmd,
					int			nAppID,
					int			nNetID,
					int			nAdsType,
					char*		sGaid,
					char*		sIdfa)
{
	int 		nFlag = 0;
	RptEntry	hRptEntry = {{0,},};
	AppNode		hAppNode = {0,};
	AppNode*	pAppNode = NULL;

	if (!pDevEntry || !pRptTable)
	{
		debug();

		return -1;
	}

	if (!sYmd || nAppID < 1 || nNetID < 0 || nAdsType < 0)
	{
		debug("ymd = %s, AppID = %d, NetID = %d, AdsType = %d ", sYmd, nAppID, nNetID, nAdsType);

		return -1;
	}
	
	hAppNode.nAppID = nAppID;

	// Applist에 AppNode를 삽입
	if ((pAppNode = AppListInsert(pDevEntry->pAppList, &hAppNode, &nFlag)) == NULL)
	{
		debug();

		return -1;
	}

	if (nFlag == 1)
	{
		// 리포트 엔트리 단위를 만든다.(ymd, appID, 0, 0, 0 인 리포트 셋팅)

		strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
		hRptEntry.nAppID = nAppID,
		hRptEntry.nNetID = 0;
		hRptEntry.nAdsType = 0;
		hRptEntry.nDevType = 0;

		// 리포트 테이블에 insert]
		if (ReportCount(pRptTable, &hRptEntry) < 0)
		{
			debug();

			return -1;
		}

		// appID 별, 엔트리 단위(sGaid 값이 있으면, DevType 이 1인 경우)
		if (strcmp(sGaid, "null") != 0)
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = nAppID,
			hRptEntry.nNetID = 0;
			hRptEntry.nAdsType = 0;
			hRptEntry.nDevType = 1;

			if (ReportCount(pRptTable, &hRptEntry) < 0)
			{
				debug();

				return -1;
			}
		}
		
		// appID 별, 엔트리 단위(sIdfa 값이 있으면, DevType 이 2인 경우)
		else if (strcmp(sIdfa, "null") != 0)
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = nAppID,
			hRptEntry.nNetID = 0;
			hRptEntry.nAdsType = 0;
			hRptEntry.nDevType = 2;

			if (ReportCount(pRptTable, &hRptEntry) < 0)
			{
				debug();

				return -1;
			}
		}
		
		// appID 별, 엔트리 단위(둘 다 아니면, DevType 이 3인 경우)
		else
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = nAppID,
			hRptEntry.nNetID = 0;
			hRptEntry.nAdsType = 0;
			hRptEntry.nDevType = 3;

			if (ReportCount(pRptTable, &hRptEntry) < 0)
			{
				debug();

				return -1;
			}
		}
	}

	if (nNetID > 0 && nAdsType > 0)
	{
		// AppID 노드는 NetID 리스트를 가지고 있으므로
		AppNetCount(pRptTable, pAppNode, sYmd, nNetID, nAdsType, sGaid, sIdfa);
	}

	return 0;
}

// 네트워크ID 별 카운트
int NetCount(
					RptTable*	pRptTable,
					DevEntry*	pDevEntry,
					char*		sYmd,
					int			nNetID,
					int			nAdsType,
					char*		sGaid,
					char*		sIdfa)
{
	int 		nFlag = 0;
	RptEntry	hRptEntry = {{0,},};
	NetNode		hNetNode  = {0,};
	NetNode*	pNetNode  = NULL;

	// 파라미터 유효성 검사
	if (!pRptTable || !pDevEntry)
	{
		debug();

		return -1;
	}

	if (!sYmd || nNetID < 1 || nAdsType < 1)
	{
		//debug("ymd = %s, NetID = %d, nAdsType = %d", sYmd, nNetID, nAdsType);

		return -1;
	}
	
	// NetNode에 값을 assign 한다.
	hNetNode.nNetID = nNetID;
	hNetNode.nAdsType = nAdsType;

	// print 로 확인
	//NetListPrint(pDevEntry->pNetList);

	// list에 NetNode를 삽입
	if ((pNetNode = NetListInsert(pDevEntry->pNetList, &hNetNode, &nFlag)) == NULL)
	{
		debug();

		return -1;
	}

	if (nFlag == 1)
	{
		// 리포트 엔트리 단위를 만든다.(ymd, 0, netID, AdsType 인 리포트 셋팅)

		strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
		hRptEntry.nAppID = 0;
		hRptEntry.nNetID = nNetID;
		hRptEntry.nAdsType = nAdsType;
		hRptEntry.nDevType = 0;

		// 리포트테이블에 insert
		if ((ReportCount(pRptTable, &hRptEntry)) < 0)
		{
			debug();

			return -1;
		}

		// 네트워크 별, active user카운트(sGaid 에 값이 있으면, DevType 이 1인 경우)
		if (strcmp(sGaid, "null") != 0)
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = 0;
			hRptEntry.nNetID = nNetID;
			hRptEntry.nAdsType = nAdsType;
			hRptEntry.nDevType = 1;

			// 리포트테이블에 insert
			if ((ReportCount(pRptTable, &hRptEntry)) < 0)
			{
				debug();

				return -1;
			}
		}

		// 네트워크 별, active user카운트(sIdfa 에 값이 있으면, DevType 이 2인 경우)
		else if (strcmp(sIdfa, "null") != 0)
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = 0;
			hRptEntry.nNetID = nNetID;
			hRptEntry.nAdsType = nAdsType;
			hRptEntry.nDevType = 2;

			// 리포트테이블에 insert
			if ((ReportCount(pRptTable, &hRptEntry)) < 0)
			{
				debug();

				return -1;
			}
		}

		// 네트워크 별, active user카운트(둘 다 아니면, DevType 이 3인 경우)
		else
		{
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = 0;
			hRptEntry.nNetID = nNetID;
			hRptEntry.nAdsType = nAdsType;
			hRptEntry.nDevType = 3;

			// 리포트테이블에 insert
			if ((ReportCount(pRptTable, &hRptEntry)) < 0)
			{
				debug();

				return -1;
			}
		}
	}

	return 0;
}

// 로그 파일 읽어서 셋팅
int DauSet(
					char*		sFileName,
					char*		sOption,
					DevTable*	pDevTable,
					RptTable*	pRptTable)
{
	FILE* 		pFile = NULL;
	long long	nCnt = 0;
	int			nOption = 0;

	// 유효성 검사
	if (!sFileName || !pDevTable || !pRptTable)
	{
		debug();

		return -1;
	}

	if ((pFile = fopen64(sFileName, "r")) == NULL)
	{
		debug("sFileName=%s", sFileName);

		return -1;
	}
	
	//sOption 을 int 로...dau 이면 0, mau 이면 1로 셋팅)
	if (!strcmp(sOption, "dau"))
	{
		nOption = 0;
	}

	else
	{
		nOption = 1;
	}

	while (!feof(pFile))
	{
		char		sBuf[BUFFER] = {0};
		int			nFlag = 0;
		DevEntry	hEntry = {{0,},};
		DevEntry*	pDevEntry = NULL;
		RptEntry	hRptEntry = {{0,},};
		char		sYmd[32] = {0,};
		int			nAppID = 0;
		int			nNetID = 0;
		int			nAdsType = 0;
		char		sGaid[64] = {0,};
		char		sIdfa[64] = {0,};
		int			nRet = 0; // 인자 갯수

		// 파일을 읽었을 때 NULL인 경우
		if (fgets(sBuf, sizeof(sBuf), pFile) == NULL)
		{
			continue;
		}
		
		// 파일을 읽었을 때 NULL 이 아니면,
		// 만약 옵션이 dau 라면,
		if (nOption == 0)
		{
			nRet = sscanf(sBuf, "%8s %*s %*s %*d %s %*d %d %d %*d %*d %*d %d %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %s",
					sYmd, hEntry.sDevID, &nAppID, &nNetID, &nAdsType, sIdfa, sGaid);
		}
		
		// mau 이면, 
		else
		{
			nRet = sscanf(sBuf, "%6s %*s %s %d %d %d %s %s ", sYmd, hEntry.sDevID, &nAppID, &nNetID, &nAdsType, sIdfa, sGaid);
		}
	
		//printf("Ymd = %s, DevID = %s, AppID = %d, NetID = %d, AdsType = %d, Idfa = %s, Gaid= %s\n", sYmd, hEntry.sDevID, nAppID, nNetID, nAdsType, sIdfa, sGaid);

		if ((pDevEntry = DevTableInsert(pDevTable, &hEntry, &nFlag)) == NULL)
		{
			debug();

			return -1;
		}
		
		nCnt++;

		if (nCnt % 10000 == 0)
		{
			printf("%lld\n", nCnt);
		}

		if (nFlag == 1)
		{
			// 전체 active user 카운트 하기 위한  Entry 셋팅
			strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
			hRptEntry.nAppID = 0;
			hRptEntry.nNetID = 0;
			hRptEntry.nAdsType = 0;
			hRptEntry.nDevType = 0;

			// 셋팅된 엔트리를 넣고 count(전체 active 유저)
			if (ReportCount(pRptTable, &hRptEntry) < 0)
			{
				debug();

				return -1;
			}

			// 전체 active user 카운트(단, sGaid 에 값이 있으면,DevType 이 1인 경우)
			if (strcmp(sGaid, "null") != 0)
			{
				strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
				hRptEntry.nAppID = 0;
				hRptEntry.nNetID = 0;
				hRptEntry.nAdsType = 0;
				hRptEntry.nDevType = 1;

				if (ReportCount(pRptTable, &hRptEntry) < 0)
				{
					debug();

					return -1;
				}
			}
			
			// 전체 active user 카운트(단, sIdfa 에 값이 있으면, DevType 이 2인 경우)
			else if (strcmp(sIdfa, "null") != 0)
			{
				strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
				hRptEntry.nAppID = 0;
				hRptEntry.nNetID = 0;
				hRptEntry.nAdsType = 0;
				hRptEntry.nDevType = 2;

				if (ReportCount(pRptTable, &hRptEntry) < 0)
				{
					debug();

					return -1;
				}
			}

			// 전체 active user 카운트(둘 다 아니면, DevType 이 3인 경우)
			else
			{
				strncpy(hRptEntry.sYmd, sYmd, sizeof(hRptEntry.sYmd));
				hRptEntry.nAppID = 0;
				hRptEntry.nNetID = 0;
				hRptEntry.nAdsType = 0;
				hRptEntry.nDevType = 3;

				if (ReportCount(pRptTable, &hRptEntry) < 0)
				{
					debug();

					return -1;
				}
			}
		}

		// 앱별 active user (AppCount 내부에서 앱,네트워크 별 count)
		AppCount(pRptTable, pDevEntry, sYmd, nAppID, nNetID, nAdsType, sGaid, sIdfa);
	
		// 네트워크별 active user
		NetCount(pRptTable, pDevEntry, sYmd, nNetID, nAdsType, sGaid, sIdfa);
	}
	
	fclose(pFile);

	return 0;
}

// DB 매니저 시작!!
int DBManStart(
					char*		sOption,
					RptTable*	pRptTable)
{
	DBMan*	pDBMan = NULL;
	int		nRet = 0;
	int		nOption = 0;
	char	sQuery[1024] = {0,};

	int i = 0;

	// 파라미터 유효성 검사
	if (!pRptTable)
	{
		debug();

		return -1;
	}

	// DB매니저 create
	if ((pDBMan = DBManCreate(g_sHost, g_sID, g_sPasswd, g_sDB, g_nPort)) == NULL)
	{
		debug();

		return -1;
	}
	
	if (!strcmp(sOption, "dau"))
	{
		nOption = 0;
	}
	
	else
	{
		nOption = 1;
	}

	// report 해시테이블을 돌면서, 
	for (i=0; i<pRptTable->nNumSlot; ++i)
	{
		if (pRptTable->pRptEntry[i] == NULL)
		{
			continue;
		}

		// 만약 dau이면, DB에 ymd 형식...
		if (nOption == 0)
		{
			// Rptable 의 엔트리 값을 DB 테이블에 insert 쿼리를 질의한다.
			snprintf(sQuery, sizeof(sQuery)-1,
					"insert into dau_count set ymd='%s', app_id='%d', net_id='%d',"
					"ads_type='%d', dev_type='%d', active_user='%d';"
					,pRptTable->pRptEntry[i]->sYmd, pRptTable->pRptEntry[i]->nAppID
					,pRptTable->pRptEntry[i]->nNetID, pRptTable->pRptEntry[i]->nAdsType
					,pRptTable->pRptEntry[i]->nDevType, pRptTable->pRptEntry[i]->nActiveUser);
		}

		// 만약 mau이면, DB에 ym 형식...
		else
		{
			// Rptable 의 엔트리 값을 DB 테이블에 insert 쿼리를 질의한다.
			snprintf(sQuery, sizeof(sQuery)-1,
					"insert into mau_count set ym='%s', app_id='%d', net_id='%d',"
					"ads_type='%d', dev_type='%d', active_user='%d';"
					,pRptTable->pRptEntry[i]->sYmd, pRptTable->pRptEntry[i]->nAppID
					,pRptTable->pRptEntry[i]->nNetID, pRptTable->pRptEntry[i]->nAdsType
					,pRptTable->pRptEntry[i]->nDevType, pRptTable->pRptEntry[i]->nActiveUser);
		}


		if ((nRet = DBManQuery(pDBMan, sQuery)) != 0)
		{
			debug("Query=%s,Err=%s", sQuery, DBManError(pDBMan));

			//DBManDestroy(pDBMan);

			//return -1;
		}
	}

	// DBMan 해제
	nRet = DBManDestroy(pDBMan);

	return 0;
}

int Admixer_DauStart(
					int		nParam,
					char**	sParam)
{
	char	sConfigFile[128] = "admixer_dau.conf";
	int		nDBRes = 0;

	DevTable*	pDevTable = NULL;
	RptTable*	pRptTable = NULL;

	if (nParam < 1 || !sParam)
	{
		debug();

		return -1;
	}

	if (nParam > 1)
	{
		if (!strcmp(sParam[1], "--help"))
		{
			ConfigUsage();

			exit(0);
		}
	}

	// 기본적으로 설정파일을 읽어와서 셋팅
	if (ConfigLoadFile(sConfigFile) < 0)
	{
		debug();

		return -1;
	}
	
	// 파라미터 값이 있다면, ConfigLoadParam 호출
	if (nParam > 1 )
	{
		ConfigLoadParam(nParam, sParam);
	}

	// dev 테이블 생성
	if ((pDevTable = DevTableCreate(g_nHashSize)) == NULL)
	{
		debug();

		return -1;
	}

	// report 테이블 생성
	if ((pRptTable = RptTableCreate(100000)) == NULL)
	{
		debug();

		return -1;
	}

	// 분석할 로그 파일 읽어서 셋팅
	if (DauSet(g_sLogFileName, g_sOption, pDevTable, pRptTable) < 0)
	{
		debug();

		// dev 테이블 해제
		DevTableDestroy(pDevTable);

		// report 테이블 해제
		RptTableDestroy(pRptTable);

		return -1;
	}

	if ((nDBRes = DBManStart(g_sOption, pRptTable)) < 0)
	{
		debug();

		// report테이블 해제
		RptTableDestroy(pRptTable);

		return -1;
	}

	DevTableDestroy(pDevTable);

	RptTableDestroy(pRptTable);

	return 0;
}

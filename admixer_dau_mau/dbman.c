/// @file	dbman.c
/// @brief	Database Manager\n
///			데이터베이스 관리 모듈
/// @author	yoonzz320
/// @date	2015-11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbman.h"
#include "debug.h"

// DB Manager 생성
DBMan* DBManCreate(
		const char*		sHost,		// 호스트
		const char*		sID,		// 아이디
		const char*		sPasswd,	// 패스워드
		const char*		sDB,		// db이름
		unsigned short 	uPort)		// 포트 번호
{
	DBMan*	pDBM = NULL;
	int		nOpt = 0;

	if (!sHost || !sID || !sPasswd || !sDB)
	{
		debug();

		return NULL;
	}
	
	// 메모리 할당
	if ((pDBM = malloc(sizeof(DBMan))) == NULL)
	{
		debug();

		return NULL;
	}
	
	// pDBM 초기화
	memset(pDBM, 0, sizeof(DBMan));
	
	if ((pDBM->pConn = mysql_init(NULL)) == NULL)
	{
		free(pDBM);
		
		debug();

		return NULL;
	}
	
	// DB가 끊어지면 다시 연결(옵션 설정)
	nOpt = 1;
	mysql_options(pDBM->pConn, MYSQL_OPT_RECONNECT, &nOpt);

	// DB연결
	if (mysql_real_connect(pDBM->pConn, sHost, sID, sPasswd, sDB, uPort, NULL, 0) == NULL)
	{
		// 연결을 종료
		mysql_close(pDBM->pConn);
		free(pDBM);
		
		debug();

		return NULL;
	}

	return pDBM;
}

// DB매니저 해제함수
int DBManDestroy(
					DBMan* pDBM) // DB 관리자
{
	if (pDBM == NULL)
	{
		return -1;
	}

	if (pDBM->pConn)
	{
		mysql_close(pDBM->pConn);
	}

	free(pDBM);

	return 0;
}

// DB서버로 질의한다.
int DBManQuery(
					DBMan*			pDBM,	 // DB 관리자
					const char*		sQuery)	 // 쿼리문
{
	int			nRet = 0;

	if (!pDBM || !sQuery)
	{
		return -1;
	}

	// 질의 요청
	nRet = mysql_query(pDBM->pConn, sQuery);
	
	return nRet;
}

/*
// DB서버로 질의한다.
int DBManQuery(
					DBMan*			pDBM,	 // DB 관리자
					const char*		sQuery,	 // 쿼리문
					DBManResult* 	pResult) // 질의 결과
{
	int			nRet = 0;
	MYSQL_RES*	pRes = NULL;

	if (!pDBM || !sQuery)
	{
		return -1;
	}

	// 질의 요청
	nRet = mysql_query(pDBM->pConn, sQuery);
	
	// 결과 저장
	if (pResult)
	{
		if ((pRes = mysql_store_result(pDBM->pConn)) == NULL)
		{
			return -1;
		}

		// 필드,열의 개수,열포인터,결과버퍼를 할당
		pResult->pBuf = pRes;
		pResult->hRow = mysql_fetch_row(pRes);
		pResult->nNumRow = mysql_num_rows(pRes);
		pResult->nNumField = mysql_num_fields(pRes); 
	}

	return nRet;
}
*/

// 다음 열을 알아오는 함수
int DBManResultNext(
					DBManResult* pResult) // 질의 결과
{
	if (!pResult || !pResult->pBuf)
	{
		return -1;
	}

	pResult->hRow = mysql_fetch_row(pResult->pBuf);

	return 0;
}

int DBManResultRelease(
					DBManResult* pResult) // 질의 결과
{
	if (pResult == NULL)
	{
		return -1;
	}

	if (pResult->pBuf)
	{
		mysql_free_result(pResult->pBuf);
	}

	return 0;
}

char* DBManError(
					DBMan* pDBM) //DB 관리자
{
	if (!pDBM)
	{
		return NULL;
	}

	return ((char*)mysql_error(pDBM->pConn));
}

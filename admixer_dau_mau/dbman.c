/// @file	dbman.c
/// @brief	Database Manager\n
///			�����ͺ��̽� ���� ���
/// @author	yoonzz320
/// @date	2015-11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbman.h"
#include "debug.h"

// DB Manager ����
DBMan* DBManCreate(
		const char*		sHost,		// ȣ��Ʈ
		const char*		sID,		// ���̵�
		const char*		sPasswd,	// �н�����
		const char*		sDB,		// db�̸�
		unsigned short 	uPort)		// ��Ʈ ��ȣ
{
	DBMan*	pDBM = NULL;
	int		nOpt = 0;

	if (!sHost || !sID || !sPasswd || !sDB)
	{
		debug();

		return NULL;
	}
	
	// �޸� �Ҵ�
	if ((pDBM = malloc(sizeof(DBMan))) == NULL)
	{
		debug();

		return NULL;
	}
	
	// pDBM �ʱ�ȭ
	memset(pDBM, 0, sizeof(DBMan));
	
	if ((pDBM->pConn = mysql_init(NULL)) == NULL)
	{
		free(pDBM);
		
		debug();

		return NULL;
	}
	
	// DB�� �������� �ٽ� ����(�ɼ� ����)
	nOpt = 1;
	mysql_options(pDBM->pConn, MYSQL_OPT_RECONNECT, &nOpt);

	// DB����
	if (mysql_real_connect(pDBM->pConn, sHost, sID, sPasswd, sDB, uPort, NULL, 0) == NULL)
	{
		// ������ ����
		mysql_close(pDBM->pConn);
		free(pDBM);
		
		debug();

		return NULL;
	}

	return pDBM;
}

// DB�Ŵ��� �����Լ�
int DBManDestroy(
					DBMan* pDBM) // DB ������
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

// DB������ �����Ѵ�.
int DBManQuery(
					DBMan*			pDBM,	 // DB ������
					const char*		sQuery)	 // ������
{
	int			nRet = 0;

	if (!pDBM || !sQuery)
	{
		return -1;
	}

	// ���� ��û
	nRet = mysql_query(pDBM->pConn, sQuery);
	
	return nRet;
}

/*
// DB������ �����Ѵ�.
int DBManQuery(
					DBMan*			pDBM,	 // DB ������
					const char*		sQuery,	 // ������
					DBManResult* 	pResult) // ���� ���
{
	int			nRet = 0;
	MYSQL_RES*	pRes = NULL;

	if (!pDBM || !sQuery)
	{
		return -1;
	}

	// ���� ��û
	nRet = mysql_query(pDBM->pConn, sQuery);
	
	// ��� ����
	if (pResult)
	{
		if ((pRes = mysql_store_result(pDBM->pConn)) == NULL)
		{
			return -1;
		}

		// �ʵ�,���� ����,��������,������۸� �Ҵ�
		pResult->pBuf = pRes;
		pResult->hRow = mysql_fetch_row(pRes);
		pResult->nNumRow = mysql_num_rows(pRes);
		pResult->nNumField = mysql_num_fields(pRes); 
	}

	return nRet;
}
*/

// ���� ���� �˾ƿ��� �Լ�
int DBManResultNext(
					DBManResult* pResult) // ���� ���
{
	if (!pResult || !pResult->pBuf)
	{
		return -1;
	}

	pResult->hRow = mysql_fetch_row(pResult->pBuf);

	return 0;
}

int DBManResultRelease(
					DBManResult* pResult) // ���� ���
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
					DBMan* pDBM) //DB ������
{
	if (!pDBM)
	{
		return NULL;
	}

	return ((char*)mysql_error(pDBM->pConn));
}

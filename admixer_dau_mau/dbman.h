/// @file	dbman.h
/// @brief	Database Manager\n
///			�����ͺ��̽� ���� ���
/// @author	yoonzz320
/// @date	2015-11

#ifndef __DBMAN_H
#define __DBMAN_H

#include <mysql.h>
#include <errmsg.h>
#include <mysqld_error.h>

typedef struct DBManResult
{
	MYSQL_RES*		pBuf;		// Mysql ��� ����
	MYSQL_ROW		hRow;		// ���� ���� ����Ű�� Mysql �� ������
	unsigned int 	nNumRow;	// ���� ����
	unsigned int 	nNumField;	// �ʵ��� ����
} DBManResult;

typedef struct DBMan
{
	MYSQL*		pConn;
} DBMan;

extern DBMan* DBManCreate(
		const char*		sHost,		// ȣ��Ʈ
		const char*		sID,		// ���̵�
		const char*		sPasswd,	// �н�����
		const char*		sDB,		// db�̸�
		unsigned short 	uPort);		// ��Ʈ ��ȣ

extern int DBManDestroy(
					DBMan* pDBM);	// DB ������
/*
extern int DBManQuery(
					DBMan*			pDBM,	 // DB ������
					const char*		sQuery,	 // ������
					DBManResult* 	pResult);// ���� ���
*/
extern int DBManQuery(
					DBMan*			pDBM,	 // DB ������
					const char*		sQuery); // ������

extern int DBManResultNext(
					DBManResult* pResult);	 //���� ���

extern int DBManResultRelease(
					DBManResult* pResult);	 //���� ���

extern char* DBManError(
					DBMan* pDBM);			 // DB ������

#endif

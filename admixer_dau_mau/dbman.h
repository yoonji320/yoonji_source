/// @file	dbman.h
/// @brief	Database Manager\n
///			데이터베이스 관리 모듈
/// @author	yoonzz320
/// @date	2015-11

#ifndef __DBMAN_H
#define __DBMAN_H

#include <mysql.h>
#include <errmsg.h>
#include <mysqld_error.h>

typedef struct DBManResult
{
	MYSQL_RES*		pBuf;		// Mysql 결과 버퍼
	MYSQL_ROW		hRow;		// 현재 열을 가리키는 Mysql 열 포인터
	unsigned int 	nNumRow;	// 열의 개수
	unsigned int 	nNumField;	// 필드의 개수
} DBManResult;

typedef struct DBMan
{
	MYSQL*		pConn;
} DBMan;

extern DBMan* DBManCreate(
		const char*		sHost,		// 호스트
		const char*		sID,		// 아이디
		const char*		sPasswd,	// 패스워드
		const char*		sDB,		// db이름
		unsigned short 	uPort);		// 포트 번호

extern int DBManDestroy(
					DBMan* pDBM);	// DB 관리자
/*
extern int DBManQuery(
					DBMan*			pDBM,	 // DB 관리자
					const char*		sQuery,	 // 쿼리문
					DBManResult* 	pResult);// 질의 결과
*/
extern int DBManQuery(
					DBMan*			pDBM,	 // DB 관리자
					const char*		sQuery); // 쿼리문

extern int DBManResultNext(
					DBManResult* pResult);	 //질의 결과

extern int DBManResultRelease(
					DBManResult* pResult);	 //질의 결과

extern char* DBManError(
					DBMan* pDBM);			 // DB 관리자

#endif

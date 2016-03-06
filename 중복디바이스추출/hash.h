
/// @file		hash.h
/// @brief		해시 모듈, 해시값 계산에 필요한 인터페이스
/// @author		hansch
/// @date		2010-05

#ifndef __HASH_H
#define __HASH_H

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*DestroyFunc)(void*);
typedef int (*PrintFunc)(void*,int);

typedef struct HashEntry
{
	void*	pKey;				///< 키
	int		nKeyLen;			///< 키 길이
	void*	pData;				///< 데이터
	int		nDataLen;			///< 데이터 길이

} HashEntry;

typedef struct HashTable
{
	int			nNumSlot;		///< 슬롯 개수
	int			nNumEntry;		///< 엔트리 개수
	List**		pSlot;			///< 슬롯

} HashTable;


extern unsigned int HashGetPrime(
					unsigned int num);		///< 기준값

extern int HashGetIndex(
					const char* value, 		///< 해시값을 계산할 입력값 
					unsigned int len, 		///< 입력값의 길이
					unsigned int num_slot);	///< 해시 테이블의 슬롯수

extern HashTable* HashTableCreate(
					int	nNumEntry);			///< 엔트리 개수

extern int HashTableDestroy(
					HashTable* 	pTable,		///< 해시 테이블
					DestroyFunc	pFree);		///< 데이터 해제 함수

extern void* HashTableInsert(
					HashTable*	pTable,		///< 해시 테이블
					void*		pKey,		///< 키
					int			nKeyLen,	///< 키 길이
					void*		pData,		///< 데이터
					int			nDataLen);	///< 데이터 길이

extern void* HashTableDelete(
					HashTable*	pTable,		///< 해시 테이블
					void*		pKey,		///< 키
					int			nKeyLen);	///< 키 길이

extern void* HashTableSearch(
					HashTable*	pTable,		///< 해시 테이블
					void*		pKey,		///< 키
					int			nKeyLen);	///< 키 길이

extern int HashTablePrint(
					HashTable* 	pTable,		///< 해시 테이블
					int			nFD,		///< 파일 디스크립터
					PrintFunc	pPrint);	///< 출력 함수

#ifdef __cplusplus
}
#endif

#endif	// __HASH_H


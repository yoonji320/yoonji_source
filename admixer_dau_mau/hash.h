
/// @file		hash.h
/// @brief		해시
/// @author		yoonzz320
/// @date		2015-10

#ifndef __HASH_H
#define __HASH_H
/*
typedef struct HashEntry
{
	char	sDevID[64];
	int		nCampID;
	int		nAdsGroupID;
	int		nAdsID;
	int		nEventID;
	int 	nDevIDType;
	char	sYmd[16];
	//char sKey[100];

} HashEntry;

typedef struct HashTable
{
	int			nNumSlot;
	int			nNumEntry;
	HashEntry** pEntry;

} HashTable;
*/
extern int HashGetPrime(
					unsigned int num);
/*
extern HashTable* HashTableCreate(
					int nNumEntry);	// 엔트리 갯수

extern int HashTableDestroy(
					HashTable*	pTable); // 해시테이블
*/
extern int HashGetIndex(
					const char* value,
					unsigned int len,
					unsigned int nNumSlot);
/*
extern int HashGetIndex2(
					const char* value,
					unsigned int len,
					unsigned int nNumSlot);

extern int HashGetIndex3(
					const char* value,
					unsigned int len,
					unsigned int nNumSlot);

extern int HashGetIndex4(
					const char* value,
					unsigned int len,
					unsigned int nNumSlot);

extern HashEntry*	EventHashTableInsert(
					HashTable*	pTable,
					HashEntry*	pEntry);
*/
/*
extern HashEntry* 	EventHashTableInsert(
					HashTable*	pTable,	// 해시테이블
					HashEntry*	pEntry,
					char*		sKey); // 해시엔트리
*/
/*
extern HashEntry*	CampHashTableInsert(
					HashTable*	pCampTable,	// 해시테이블
					HashEntry*	pCampEntry); // 해시엔트리

extern int HashTableDelete(
					HashTable* pTable,
					HashEntry* pEntry);

extern void* HashTableSearch(
					HashTable* pTable,
					HashEntry* pEntry);

extern int HashTablePrint(
					HashTable*  pTable,
					int			nFD);
*/
#endif // __HASH_H

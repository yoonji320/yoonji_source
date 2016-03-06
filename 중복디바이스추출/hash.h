
/// @file		hash.h
/// @brief		�ؽ� ���, �ؽð� ��꿡 �ʿ��� �������̽�
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
	void*	pKey;				///< Ű
	int		nKeyLen;			///< Ű ����
	void*	pData;				///< ������
	int		nDataLen;			///< ������ ����

} HashEntry;

typedef struct HashTable
{
	int			nNumSlot;		///< ���� ����
	int			nNumEntry;		///< ��Ʈ�� ����
	List**		pSlot;			///< ����

} HashTable;


extern unsigned int HashGetPrime(
					unsigned int num);		///< ���ذ�

extern int HashGetIndex(
					const char* value, 		///< �ؽð��� ����� �Է°� 
					unsigned int len, 		///< �Է°��� ����
					unsigned int num_slot);	///< �ؽ� ���̺��� ���Լ�

extern HashTable* HashTableCreate(
					int	nNumEntry);			///< ��Ʈ�� ����

extern int HashTableDestroy(
					HashTable* 	pTable,		///< �ؽ� ���̺�
					DestroyFunc	pFree);		///< ������ ���� �Լ�

extern void* HashTableInsert(
					HashTable*	pTable,		///< �ؽ� ���̺�
					void*		pKey,		///< Ű
					int			nKeyLen,	///< Ű ����
					void*		pData,		///< ������
					int			nDataLen);	///< ������ ����

extern void* HashTableDelete(
					HashTable*	pTable,		///< �ؽ� ���̺�
					void*		pKey,		///< Ű
					int			nKeyLen);	///< Ű ����

extern void* HashTableSearch(
					HashTable*	pTable,		///< �ؽ� ���̺�
					void*		pKey,		///< Ű
					int			nKeyLen);	///< Ű ����

extern int HashTablePrint(
					HashTable* 	pTable,		///< �ؽ� ���̺�
					int			nFD,		///< ���� ��ũ����
					PrintFunc	pPrint);	///< ��� �Լ�

#ifdef __cplusplus
}
#endif

#endif	// __HASH_H


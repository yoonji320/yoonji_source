
#ifndef	_LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "device.h"
#include "debug.h"
#include "hash.h"
#include "list.h"

#define BUFFER 1024
#define NUM_NET 20

///
///	@brief		디바이스 테이블을 생성한다.
///
///	@return
///
DeviceTable* DeviceTableCreate(
								int nNumEntry)	///< 엔트리의 개수
{
	int				nNumSlot = 0;		
	DeviceTable*	pTable = NULL;

	if (nNumEntry < 1)
	{
		nNumEntry = 1;
	}

	if ((nNumSlot = HashGetPrime(nNumEntry*10)) < 0)
	{
		debug();

		return NULL;
	}

	if ((pTable = malloc(sizeof(*pTable))) == NULL)
	{
		debug();
		
		return NULL;
	}

	memset(pTable, 0, sizeof(*pTable));
	
	if ((pTable->pDevice = malloc(sizeof(Device*)*nNumSlot)) == NULL)
	{
		debug();

		free(pTable);

		return NULL;
	}
	
	memset(pTable->pDevice, 0, sizeof(Device*)*nNumSlot);
	pTable->nNumSlot = nNumSlot;
	pTable->nNumEntry = 0;

	return pTable;
}

///
///	@brief		디바이스 테이블을 해제한다.
///
///	@return
///
int DeviceTableDestroy(
						DeviceTable* pTable)	///< 디바이스 테이블
{
	int i;

	if (pTable == NULL)
	{
		debug();

		return -1;
	}
	
	for (i=0; i<pTable->nNumSlot; i++)
	{
		if (pTable->pDevice[i])
		{
			free(pTable->pDevice[i]);
		}
	}
	
	free(pTable->pDevice);
	free(pTable);

	return 0;
}

///
/// @brief		디바이스 테이블에 디바이스 추가한다.
///
/// @return
///
Device* DeviceTableInsert(
						DeviceTable* 	pTable,			///< 디바이스 테이블
						Device*			pDevice)		///< 디바이스
{
	int		nIndex =0;
	int i;

	if (!pTable || !pDevice)
	{
		debug();
	
		return NULL;
	}

	//해시 테이블 인덱스를 얻어온다.
	if ((nIndex = HashGetIndex(pDevice->sDevID, sizeof(pDevice->sDevID), pTable->nNumSlot)) < 0)
	{
		debug();

		return NULL;
	}
	//printf("aaa%d\n",nIndex);
	
	for (i=0; i<pTable->nNumSlot; ++i)
	{
		//슬롯이 비었음 : 해당 슬롯에 추가한다.
		if (pTable->pDevice[nIndex]  == NULL)
		{
			pTable->pDevice[nIndex] = malloc(sizeof(*pDevice));
			
			//예외처리 (슬롯에 추가 했는데도 값이 없는 경우)
			if (pTable->pDevice[nIndex] == NULL)	
			{
				debug();

				return NULL;
			}
		
			memcpy(pTable->pDevice[nIndex], pDevice, sizeof(*pDevice));
			pTable->pDevice[nIndex]->nNumNetID = 1;
			
			pTable->nNumEntry++;
			
			return (pTable->pDevice[nIndex]);
		}	
		
		//동일한 엔트리가 존재함 : 해당 엔트리 리턴
		else if (!strcmp(pTable->pDevice[nIndex]->sDevID, pDevice->sDevID))
		{
			//if (pTable->pDevice[nIndex]->nNetID !=  pDevice->nNetID)	//이렇게하면안됨., 1010 1000를 비교하면,
			//테이블의 nNetID와, 구조체의 nNetID는 다르지만 OR연산하면 결국 같은 값인데,if문안에서 nNumNetID가 또 카운트 되므로..
			if (pTable->pDevice[nIndex]->nNetID != (pTable->pDevice[nIndex]->nNetID | pDevice->nNetID))
			{ 
				pTable->pDevice[nIndex]->nNetID = pTable->pDevice[nIndex]->nNetID | pDevice->nNetID;
			
				pTable->pDevice[nIndex]->nNumNetID++;
			}
			
			return (pTable->pDevice[nIndex]);
		}

		//충돌 : 다음 슬롯 검사
		nIndex = (nIndex+1) % pTable->nNumSlot;
	}
	
	//슬롯이 가득 찼음
	debug();
	
	return NULL;
}

////함수원형////
///
/// @brief 로그에서 필요한 디바이스ID와 네트워크ID를 뽑아서 해시테이블에 저장하는 함수.
///		
/// @return
int DeviceSet( 
				char* 			pFileName,			///<입력받을 파일이름
				DeviceTable*	pTable)				///<디바이스 테이블
{
	FILE*	pFile = NULL;
	
	if (!pFileName || !pTable)
	{
		debug();

		return -1;
	}

	if ((pFile = (FILE*)fopen64(pFileName, "r")) == NULL)
	{
		debug();

		return -1;
	}

	while (!feof(pFile))
	{
		char	sBuf[BUFFER] = {0};
		Device 	hDevice = {{0,},};
		Device*	pDevice = NULL;
		int		nNetID = 0;

		if (fgets(sBuf, sizeof(sBuf), pFile)!= NULL)
		{
			if (sscanf(sBuf, "%s %d", hDevice.sDevID, &nNetID) != 2)
			//if (sscanf(sBuf, "%*s\t%*s\t%*s\t%s\t%*s\t%*s\t%d", hDevice.sDevID, &nNetID) != 2)
			{
				continue;
			}
			
			else
			{
				if(nNetID < 1 || nNetID > NUM_NET)
				{
					continue;
				}
				//printf("%s\t%d\n", hDevice.sDevID, nNetID);
				if (DeviceSetNetID(&hDevice, nNetID) == -1)
				{
					debug();

					return -1;
				}

				//printf("DeviceSetNetIDTest:%s///%d\n", hDevice.sDevID, hDevice.nNetID);
		
				if ((pDevice = DeviceTableInsert(pTable, &hDevice)) == NULL)
				{
					debug();
					
					printf("###\n");
					printf("DeviceSetNetIDTest:%s///%d\n", hDevice.sDevID, hDevice.nNetID);
					printf("###\n");
					return -1;
				}
			}
			
			//printf("insertTest:%s/////%d\n", pDevice->sDevID, pDevice->nNetID);
		}	
	}

	fclose(pFile);
	
	return 0;
}

///
///	@brief		디바이스 테이블로 부터 데이터를 가져온다.
///
///	@return
///
int DeviceGet(	
				DeviceTable*	 pTable, 					///< 디바이스 테이블
				int				 nNetIDCount[NUM_NET], 		///< 네트워크 별 디바이스 카운트
				int				 nOutput[NUM_NET][NUM_NET])	///< 중복디바이스 조합 출력
{
	int nIndex = 0;

	if (!pTable || !nNetIDCount || !nOutput)
	{
		debug();

		return -1;
	}
	
	for (nIndex=0; nIndex<pTable->nNumSlot; nIndex++)
	{
		//슬롯이 비었다면,
		if (pTable->pDevice[nIndex] == NULL)
		{
			continue;
		}
		
		//값이 있다면,
		else	
		{
			if (DeviceNetIDCount(pTable->pDevice[nIndex], nNetIDCount) == -1)
			{
				debug();

				return -1;
			}

			//값을 가져온다.
			if (DeviceGetNetID(pTable->pDevice[nIndex], nOutput) == -1)
			{
				debug();

				return -1;
			}
		}
	}

	return 0;
}

///
///	@brief		네트워크ID를 bit연산에 맞게 변환한다.\n
///				ex) 3을 입력하면, 100 이므로 2^(3-1)
///
///	@return
///
int DeviceSetNetID(
					Device* pDevice,	///< 디바이스
					int 	nNetID)		///< 네트워크 ID수
{
	if (!pDevice || nNetID < 1)
	{
		debug();

		return -1;
	}

	pDevice->nNetID = pow(2, nNetID-1);

	return 0;
}


///
///	@brief	네트워크ID별 디바이스 ID 카운트한다.
///
///	@return
///
int DeviceNetIDCount(
						Device*	pDevice,				///< 디바이스
						int		nNetIDCount[NUM_NET])	///< 네트워크 별 디바이스 카운트
{
	int i;

	if (!pDevice || !nNetIDCount)
	{
		debug();

		return -1;
	}

	for (i=0; i<NUM_NET; i++)
	{
		int nNet = (int)pow(2,i);

		if (pDevice->nNetID & nNet)
		{
			nNetIDCount[i]++;
		}
	}
	
	//printf("dev:%s %u net20:%d num:%d\n", pDevice->sDevID, pDevice->nNetID, nNetIDCount[19], pDevice->nNumNetID);
	
	return 0;
}


///	@brief	네트워크ID로 조합 만든다.
///
///	@return
///
int DeviceGetNetID(
					Device*	pDevice,					///< 디바이스
					int 	nOutput[NUM_NET][NUM_NET])	///< 중복 디바이스 조합 출력
{
	//원래 저장되어 있는 nNetID와 2^0, 2^1,..AND연산으로 한자리씩 비교.
	int nIndex = 0;
	int pNetID[NUM_NET] = {0,};
	int i;
	int j;

	if (!pDevice || !nOutput)
	{
		debug();

		return -1;
	}
	
	//for문으로 pNetID의 조합을 만들기 전에 만들 pNetID가 최소 2개이상인지 유효성검사를 해줘야함.
	if (pDevice->nNumNetID < 2)
	{	
		//에러는 아니지만 조합을 만들지 못하므로 1 리턴. 만약 -1리턴을 하면 오류로 인식..
		return 1;	
	}

	for (i=0; i<NUM_NET; i++)
	{
		if ((pDevice->nNetID & (int)pow(2, i)) !=0)
		{	
			//원래의 네트워크ID를 구해서 pNetID라는 배열에 넣어야됨
			pNetID[nIndex] = i+1;	
			//printf("pNetID[%d]=%d\n", nIndex, pNetID[nIndex]); 
			nIndex++; 
		}
		
		//효율적으로 20번까지 돌 필요없이, 네트워크ID갯수만큼만 돌게 하려고..
		if (nIndex == pDevice->nNumNetID)		
		{
			break;
		}
	}

	//조합만드는 부분
	for (i=0; i<pDevice->nNumNetID; i++)
	{
		for (j=i+1; j<pDevice->nNumNetID; j++)
		{
			//조합에서 [18][19] 가 최대이므로.
			nOutput[pNetID[i]-1][pNetID[j]-1]++;
			
			//printf("nOutput[%d][%d]=%d\n", pNetID[i], pNetID[j],nOutput[pNetID[i]-1][pNetID[j]-1]);
		}
	}
	
	return 0;
}

///	@brief	네트워크아이디별 디바이스 수를 프린트한다.
///
///	@return
///
void PrintNetIDCount(
					int		work_id,				///< work_id
					int		nNetIDCount[NUM_NET],	///< 네트워크별 디바이스ID 수
					char	type[4])				///< App정보인지, All(모든 정보인지)에 관한 값
{
	int i;

	if (nNetIDCount == NULL)
	{
		debug();

		return;
	}

	for (i=0; i<NUM_NET; i++)
	{
		if (nNetIDCount[i]>0)
		{
			printf("%d,%d,%d,%s\n",work_id, i+1, nNetIDCount[i], type);
		}
	}

	printf("--------------\n");
	return;
	//printf("111####%d\n",nNetIDCount[19]);
}

///	@brief	중복 네트워크 조합을 프린트 한다.
///
///	@return
///
void PrintOutput(
					int		work_id,					///< work_id
					int 	nOutput[NUM_NET][NUM_NET],	///< output 조합
					char 	type[4])					///< App정보인지, All(모든 정보인지)에 관한 값
{	
	int i;
	int j;
	
	if (nOutput == NULL)
	{
		debug();

		return;
	}
	
	//조합은 0 - 19 까지이므로
	for (i=0; i<20; i++) 
	{
		for (j=i+1; j<20; j++)
		{
			//nOutput조합이 있다면,
			if (nOutput[i][j]>0)	
			{
				printf("%d,%d,%d,%d,%s\n", work_id, i+1, j+1, nOutput[i][j], type); 
			}
		}
	}

	return;
}

int DeviceStart(
				int		nParam,	///< 파라미터 개수
				char**	sParam)	///< 파라미터 값
{
	int	nOutput[NUM_NET][NUM_NET]={{0,},};
	int	nNetIDCount[NUM_NET]={0,};
	
	int nNumEntry=0;
	int work_id=0;
	char pFileName[50]={0,};
	char type[4]={0,};
	DeviceTable* pTable = NULL;
	
	work_id=atoi(sParam[1]);
	strcpy(pFileName, sParam[2]);
	strcpy(type, sParam[4]);
	nNumEntry = atoi(sParam[3]);
	//mode =atoi(sParam[5]);

	if (nParam != 5 || !sParam)
	{
		debug();

		return -1;
	}
	
	
	
	/*if (pFileName == NULL)
	{
		debug();

		return -1;
	}*/

	//해시 테이블 생성
	if ((pTable = DeviceTableCreate(nNumEntry)) == NULL)	
	{
		debug();

		return -1;
	}
	
	//해시 테이블 저장
	if (DeviceSet(pFileName, pTable) < 0)
	{
		debug();

		return -1;
	}
	
	//해시 테이블에서 데이터를 꺼내온다
	if (DeviceGet(pTable, nNetIDCount, nOutput) < 0)
	{
		debug();

		return -1;
	}
	
	//NetID 별 DevID 수 카운트를 print한다.
	PrintNetIDCount(work_id, nNetIDCount, type);

	//조합 print 해서 count한다
	PrintOutput(work_id, nOutput, type);					 

	if (DeviceTableDestroy(pTable) < 0)
	{
		debug();

		return -1;
	}
	
	return 0;
}


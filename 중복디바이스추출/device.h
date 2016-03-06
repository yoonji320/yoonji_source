/// @file	device.h
/// @brief	device ID, network ID 저장에 필요한 인터페이스
/// @author	yoonzz
/// @date	2015-05

#ifndef __DEVICE_H
#define __DEVICE_H

// 해시테이블 안에 있는 entry의 구조체
typedef struct Device
{
	char		sDevID[100];
	int			nNetID;		// int 로 줬기 때문에, 4byte = 32bit. 구조체를 만들 필요 없이 int 자체에 값을 넣는다..
	int			nNumNetID;
} Device;

typedef struct DeviceTable
{
	int			nNumSlot;
	int			nNumEntry;
	Device**	pDevice;
} DeviceTable;


//int DeviceStart(char* pFileName, int nNumEntry);
int DeviceStart(int nParam, char** sParam);

int DeviceGet(DeviceTable* pTable, int nNetIDCount[20], int nOutput[20][20]);
int DeviceSet(char* pFileName, DeviceTable* pTable);

int DeviceSetNetID(Device* pDevice, int nNetID);
int DeviceNetIDCount(Device* pDevice, int nNetIDCount[20]);
int DeviceGetNetID(Device* pDevice, int nOutput[20][20]);
//int DeviceGetNetID(Device* pDevice, int* pNetID);
//int SetOutput(Device* pDevice, int* pNetID, int nOutput[20][20]);

void PrintNetIDCount(int work_id, int nNetIDCount[20], char type[4]);
void PrintOutput(int work_id, int nOutput[20][20], char type[4]);

#endif // __DEVICE_H
			

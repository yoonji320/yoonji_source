/// @file	device.h
/// @brief	device ID, network ID ���忡 �ʿ��� �������̽�
/// @author	yoonzz
/// @date	2015-05

#ifndef __DEVICE_H
#define __DEVICE_H

// �ؽ����̺� �ȿ� �ִ� entry�� ����ü
typedef struct Device
{
	char		sDevID[100];
	int			nNetID;		// int �� ��� ������, 4byte = 32bit. ����ü�� ���� �ʿ� ���� int ��ü�� ���� �ִ´�..
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
			

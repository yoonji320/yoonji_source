#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "config.h"

// 전역변수 선언(전역변수는 초기화 필요 없음)
char g_sHost[256] 		= "10.10.10.185";
char g_sID[256] 		= "nsmart";
char g_sPasswd[256] 	= "nsmart@dlcjsrn";
char g_sDB[256]			= "admixer_dau";
int  g_nPort			= 3306;
char g_sLogFileName[128];
char g_sOption[128];	// mau 인지 dau 인지 옵션 값(기본 적으로 dau로 셋팅)
int  g_nHashSize;

static ConfigMap s_hConfigMap[] = 
{
	{CONFIG_HOST,		"-host",	"DBHOST"},
	{CONFIG_DBID,		"-id",		"DBID"},
	{CONFIG_DBPW,		"-pw",		"DBPasswd"},
	{CONFIG_DBNAME,		"-name",	"DBName"},
	{CONFIG_DBPORT,		"-port",	"DBPort"},
	{CONFIG_LOGFILE,	"-file",	"LogFile"},
	{CONFIG_HASHSIZE,	"-hash",	"HashSize"},	
	{CONFIG_OPTION,		"-option",	"Option"},	
	{CONFIG_UNKNOWN, 	"",			""}
};

static int ConfigGetCode(
					const char* sName)
{
	int i;

	if (sName == NULL)
	{
		debug();

		return -1;
	}

	for (i=0; i<CONFIG_UNKNOWN; ++i)
	{
		if (!strcmp(sName, s_hConfigMap[i].sName1))
		{
			return (s_hConfigMap[i].nCode);
		}
	
		else if (!strcmp(sName, s_hConfigMap[i].sName2))
		{
			return (s_hConfigMap[i].nCode);
		}
	}	

	return CONFIG_UNKNOWN;
}

// config Usage
int ConfigUsage()
{
	int i;

	printf("Options : \n");

	for (i=0; i<CONFIG_UNKNOWN; ++i)
	{
		printf("%24s ", s_hConfigMap[i].sName1);
		
		if ((i+1) % 5 ==0)
		{
			printf("\n");
		}
	}

	printf("\n");

	return 0;
}

// 파라미터로 설정 값 셋팅
int ConfigLoadParam(
					int		nParam,
					char**	sParam)
{
	int i = 0;

	for (i=1; i<nParam; i+=2)
	{
		ConfigSetValue(sParam[i], sParam[i+1]);
	}

	return 0;
}

// 설정 파일로 설정 값 셋팅
int ConfigLoadFile(
					const char* pConfigFile)
{
	FILE*	pFile = NULL;
	int		nCurLine = 0;		
	int		nRet = 0;
	char	sBuf[1024] = {0,};
	char	sName[256] = {0,};
	char	sValue[256] = {0,};
	char	sDummy[256] = {0,};

	if (!pConfigFile)
	{
		debug();

		return -1;
	}

	if ((pFile = fopen(pConfigFile, "r")) == NULL)
	{
		debug();

		return -1;
	}

	while (!feof(pFile))
	{
		nCurLine++;

		fgets(sBuf, sizeof(sBuf), pFile);

		if (sBuf[0] == '#' || sBuf[0] == '/' || sBuf[0] == '\n')
		{
			continue;
		}

		nRet = sscanf(sBuf, "%256s %256s %256s", sName, sValue, sDummy);
		
		if (nRet != 2)
		{
			debug("Line <%d> is broken.", nCurLine);
			
			fclose(pFile);

			return -1;
		}

		if ((nRet = ConfigSetValue(sName, sValue)) < 0)
		{
			debug("Line <%d> is broken.", nCurLine);

			fclose(pFile);

			return -1;
		}
	}
	
	fclose(pFile);

	return 0;
}

// 설정 값 셋팅
int ConfigSetValue(
					char* sName,
					char* sValue)
{
	int nCode;

	if (!sName || !sValue)
	{
		debug();

		return -1;
	}

	nCode = ConfigGetCode(sName);

	switch (nCode)
	{
		case CONFIG_HOST:
		{
			strncpy(g_sHost, sValue, sizeof(g_sHost));
			break;
		}

		case CONFIG_DBID:
		{
			strncpy(g_sID, sValue, sizeof(g_sID));
			break;
		}

		case CONFIG_DBPW:
		{
			strncpy(g_sPasswd, sValue, sizeof(g_sPasswd));
			break;
		}

		case CONFIG_DBNAME:
		{	
			strncpy(g_sDB, sValue, sizeof(g_sDB));
			break;
		}

		case CONFIG_DBPORT:
		{
			g_nPort = atoi(sValue);
			break;
		}

		case CONFIG_LOGFILE:
		{
			strncpy(g_sLogFileName, sValue, sizeof(g_sLogFileName));
			break;
		}

		case CONFIG_HASHSIZE:
		{
			g_nHashSize = atoi(sValue);
			break;
		}

		case CONFIG_OPTION:
		{
			strncpy(g_sOption, sValue, sizeof(g_sOption));
			break;
		}

		default:
		{
			debug("default....");
			return -1;
		}
	}

	return 0;
}

/*
// switch문 쓰지않고 if 문으로 
int ConfigSetValue(
					char* sName,
					char* sValue)
{
	if (!sName || !sValue)
	{
		debug();

		return -1;
	}

	if (!strcmp(sName, "LogFile"))
	{
		strncpy(g_sLogFileName, sValue, sizeof(g_sLogFileName));			
	}

	if (!strcmp(sName, "DBHost"))
	{
		strncpy(g_sHost, sValue, sizeof(g_sHost));
	}

	if (!strcmp(sName, "DBID"))
	{
		strncpy(g_sID, sValue, sizeof(g_sID));
	}

	if (!strcmp(sName, "DBPasswd"))
	{
		strncpy(g_sPasswd, sValue, sizeof(g_sPasswd));
	}

	if (!strcmp(sName, "DBName"))
	{
		strncpy(g_sDB, sValue, sizeof(g_sDB));
	}

	if (!strcmp(sName, "DBPort"))
	{
		g_nPort = atoi(sValue);
	}

	if (!strcmp(sName, "HashSize"))
	{
		g_nHashSize = atoi(sValue);
	}

	return 0;
}
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

enum
{
	CONFIG_HOST = 1,
	CONFIG_DBID, 
	CONFIG_DBPW, 
	CONFIG_DBNAME,
	CONFIG_DBPORT, 
	CONFIG_LOGFILE, 
	CONFIG_HASHSIZE,
	CONFIG_OPTION,
	CONFIG_UNKNOWN
};

//전역변수 선언
extern char g_sHost[256];
extern char g_sID[256];
extern char g_sPasswd[256];
extern char g_sDB[256];
extern char g_sDB2[256];
extern int  g_nPort;
extern char g_sLogFileName[128];
extern char g_sOption[128];
extern int	g_nHashSize;

typedef struct ConfigMap
{
	int		nCode;
	char	sName1[32];
	char	sName2[32];
} ConfigMap;

extern int ConfigUsage();

extern int ConfigLoadParam(
					int		nParam,
					char**	sParam);

extern int ConfigLoadFile(
					const char* sFile);

extern int ConfigSetValue(
					char* sName,
					char* sValue);

#endif

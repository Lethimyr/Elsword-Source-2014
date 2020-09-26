#ifndef __PURPLEFORCLIENT__
#define __PURPLEFORCLIENT__

#ifdef PURPLEFORCLIENT_EXPORTS
	#define PURPLE_API __declspec(dllexport)
#else
	#define PURPLE_API __declspec(dllimport)
	#ifdef _DEBUG
		#pragma comment(lib, "PurpleForClientD.lib") 
	#else
		#pragma comment(lib, "PurpleForClient.lib") 
	#endif
#endif

#define SERVICE_NATION			0x000000FF
#define SERVICE_JPN				0x00000003

#define SERVICE_TYPE			0x00000F00
#define SERVICE_ALPHA			0x00000100
#define SERVICE_REAL			0x00000200
#define SERVICE_BETA			0x00000300

#define SERVICE_SITE			0x0000F000
#define SERVICE_HANGAME			0x00001000

//. string size define 
#define AUTHSTRING_SIZE			2048
#define GAMESTRING_SIZE			4096
#define MEMBERID_SIZE			20
#define BILLNUM_SIZE			20

//. error code define
#define PURPLE_RETURN_OK								0
#define PURPLE_ALLOCATE_FAILED							-1110
#define PURPLE_GAMESTRING_INVALID						-2220
//#define PURPLE_MODULE_LOAD_FAILED						-3330
#define PURPLE_AUTHMODULE_LOAD_FAILED					-3340
#define PURPLE_GAMEINFOMODULE_LOAD_FAILED				-3350
#define PURPLE_REPORTMODULE_LOAD_FAILED					-3360
#define PURPLE_AUTHMODULE_GETPROC_FAILED				-3440
#define PURPLE_GAMEINFOMODULE_GETPROC_FAILED			-3450
#define PURPLE_REPORTMODULE_GETPROC_FAILED				-3460


//. client excute from HanStart.
PURPLE_API	int __stdcall PurpleModuleInit();

//. client excute from other launcher.
//.PURPLE_API	int __stdcall PurpleModuleInitGameString(/*IN*/ char* szGameString);

PURPLE_API	const char*	__stdcall PurpleGetGameString();

PURPLE_API	const char*	__stdcall PurpleGetAuthString();

PURPLE_API	const char*	__stdcall PurpleGetUserID();

PURPLE_API	int	__stdcall PurpleGetServiceCode();

PURPLE_API	int	__stdcall PurpleGetBillUserNum();

PURPLE_API	int	__stdcall PurpleGetValue(char* szValue, char* szOutput, int nSize);

PURPLE_API	const char*	__stdcall PurpleGetBillUserNumString();

//PURPLE_API	int __stdcall PurpleAuthRefresh();

PURPLE_API	int __stdcall PurpleAuthRefreshEx();

PURPLE_API	int __stdcall PurpleWbersAceSend(char* szSubject, char* szReportString);

PURPLE_API	int __stdcall PurpleModuleTerminate();

#endif
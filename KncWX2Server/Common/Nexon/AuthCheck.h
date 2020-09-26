#pragma once

#ifndef EXTERN_C
	#ifdef __cplusplus
		#define EXTERN_C    extern "C"
	#else
		#define EXTERN_C    extern
	#endif
#endif

#ifndef	AUTHCHECK_EXPORTS
	#define AUTHCHECK_API		EXTERN_C __declspec(dllimport)
#else
	#define	AUTHCHECK_API		EXTERN_C __declspec(dllexport)
#endif

#ifndef IN
	#define IN
#endif

#ifndef OUT
	#define OUT
#endif

enum AuthCheckError
{
	AUTHCHECK_ERROR_OK 	= 0,							//	0: OK
	AUTHCHECK_ERROR_INVALID_ARGUMENT,					//	1: Argument is not valid
	AUTHCHECK_ERROR_INVALID_PASSPORT,					//	2: NexonPassport is not valid
	AUTHCHECK_ERROR_TIMEOUT,							//	3: [obsolete] same as AUTHCHECK_ERROR_EXPIRED
	AUTHCHECK_ERROR_EXPIRED = AUTHCHECK_ERROR_TIMEOUT,	//	3: NexonPassport is valid, but arleady expired
	AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN,				//	4: Authentication server is shutdown
	AUTHCHECK_ERROR_UNSUPPORTED_LOCALE,					//	5: Unsupported Locale ID
	AUTHCHECK_ERROR_SET_LOCALE_NEEDED,					//	6: call AuthCheck_SetLocale first
	AUTHCHECK_ERROR_NO_SERVER_INFORMATION,				//	7: no server information in config file
	AUTHCHECK_ERROR_USER_IP_MISMATCHED,					//	8: client ip address is mismatched with sso servers data
	AUTHCHECK_ERROR_NO_DECRYPTION_KEY,					//	9: There is no passport decryption key. Check AuthCheck_LoadKey function or key file.
	AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT,					//	10: passport level is lower than minimum passport level
	AUTHCHECK_ERROR_GAMECODE_MISMATCHED,				//	11: Mismatch between the caller's gamecode and the passport's gamecode.
	AUTHCHECK_ERROR_SESSION_NOT_EXIST,					//	12: User session data not exists. ( Maybe timeout or something. )
	AUTHCHECK_ERROR_DISCONNECTED,						//	13: User IP or session key missmatched with current login session. Maybe logged out by another session.
	AUTHCHECK_ERROR_INVALID_CHANNEL_CODE,				//	14:	Invalid channel code.
	AUTHCHECK_ERROR_INVALID_CONFIGURATION,				//	15:	Invalid configuration
	AUTHCHECK_ERROR_SERVER_FAILED	= 100,				//	100: Internal error.
};

enum SsnCheckError
{
	SSNCHECK_ERROR_MATCH = 0,
	SSNCHECK_ERROR_MISMATCH,
	SSNCHECK_ERROR_UNSUPPORTED_LOCALE,
	SSNCHECK_ERROR_INVALID_PASSPORT,
	SSNCHECK_ERROR_SERVICE_SHUTDOWN,
	SSNCHECK_ERROR_INVALID_ARGUMENT,
	SSNCHECK_ERROR_BLOCKED_IP,
	SSNCHECK_ERROR_USER_NOT_EXIST,
	SSNCHECK_ERROR_INTERNAL_FAILED,
};

#ifndef _NMLOCALEID_DEFINED_
#define _NMLOCALEID_DEFINED_
enum	NMLOCALEID
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//
	// ----------------------------- WARNNING -----------------------------------
	//
	// 더 이상의 NMLOCALEID는 추가하지 않습니다.
	// NMLOCALEID를 통한 soap endpoint 지정은 AuthCheck.dll.config를 사용합니다.
	//
	// ----------------------------------------------------------------- selfinder
	//
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	// old style
	kLocaleID_JP_Test		= 0x10000100,
	kLocaleID_JP2			= 0x00000101,	//	Japan arad
	kLocaleID_JP3			= 0x00000103,	//	Japan TalesWeaver/CSO
	kLocaleID_JP4			= 0x00000104,	//	Japan All(new IDC)
	kLocaleID_CN_CNC		= 0x00000111,	//	China / CNC
	kLocaleID_CN_CT			= 0x00000112,	//	China / CT
	kLocaleID_TW2			= 0x00000107,	//	Taiwan MH (GASH)
	kLocaleID_US_Test		= 0x10000200,

	kLocaleID_Null			= 0x00000000,

	kLocaleID_KR			= 0x00000001,	//	Korea, Republic of
	kLocaleID_KR_Test		= 0x10000001,	//	Test

	kLocaleID_JP			= 0x00000100,	//	Japan
	kLocaleID_CN			= 0x00000101,	//	China(Tiancity)
	kLocaleID_TW			= 0x00000102,	//	Taiwan CSO/MH
	kLocaleID_TH			= 0x00000103,	//	Thailand Kart - 종료
	kLocaleID_VN			= 0x00000104,	//	Vietnam Kart - 종료
	kLocaleID_SG			= 0x00000105,	//	Singapore CSO
	kLocaleID_ID			= 0x00000106,	//	Indonesia CSO
	kLocaleID_CN2			= 0x00000107,	//	China BF(Shanda) - SSO 사용 안함
	kLocaleID_ID2			= 0x00000108,	//	Indonesia(Kart) - SSO 사용 안함
	kLocaleID_TH2			= 0x00000109,	//	Thailand(AsiaSoft)

	kLocaleID_US			= 0x00000200,	//	United States
	kLocaleID_EU			= 0x00000300,	//	Europe
	kLocaleID_RU			= 0x00000301,	//	Russia - SSO 사용 안함
	kLocaleID_RU2			= 0x00000302,	//	Russia(Syncopate)
	kLocaleID_BR			= 0x00000400,	//	Brazil CombatArms
};
#endif

#ifndef _NMPASSPORTTYPE_DEFINED_
#define _NMPASSPORTTYPE_DEFINED_
enum	PassportType
{
	kPassportType_Undefined,
	kPassportType_Nexon,
	kPassportType_Guest,
};
#endif

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_UNSUPPORTED_LOCALE
*/
AUTHCHECK_API
AuthCheckError AuthCheck_SetLocale
( 
	IN		NMLOCALEID		uLocale
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_CONFIGURATION
*/
AUTHCHECK_API
AuthCheckError AuthCheck_LoadConfigA
(
	IN		const char *	pszKeyConfigFileName	= NULL
);

AUTHCHECK_API
AuthCheckError AuthCheck_LoadConfigW
(
	IN		const WCHAR *	pszKeyConfigFileName	= NULL
);

AUTHCHECK_API
void AuthCheck_SetTimeout
( 
	IN		UINT32			uMilliseconds
);

AUTHCHECK_API
void AuthCheck_SetIntranetMode
( 
	IN		BOOL			bSet
);

AUTHCHECK_API
void AuthCheck_SetPassportLevel
( 
 IN		UINT32			uPassportLevel
 );

AUTHCHECK_API
void AuthCheck_SetSoapHostA
( 
	IN		const char *	szHost						// 32
);

AUTHCHECK_API
void AuthCheck_SetSoapHostW
( 
	IN		const WCHAR *	szHost						// 32
);

AUTHCHECK_API
PassportType AuthCheck_GetPassportTypeA
(
	IN		const char *	szPassport					// 1024
);

AUTHCHECK_API
PassportType AuthCheck_GetPassportTypeW
(
	IN		const WCHAR	*	szPassport					// 1024
);

AUTHCHECK_API
void AuthCheck_LoadKeyA
(
	IN		const char *	pszKeyConfigFileName
);

AUTHCHECK_API
void AuthCheck_LoadKeyW
(
	IN		const WCHAR *	pszKeyConfigFileName
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_LocalCheckPassportA
( 
	IN		const char*		szPassport,					// 1024
	OUT		char*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		char*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		UINT32*			pnServerHash	= NULL,
	OUT		char*			pszServerName	= NULL,		// 32
	OUT		char*			pszNationCode	= NULL		// 3
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_LocalCheckPassportW
( 
	IN		const WCHAR*	szPassport,					// 1024
	OUT		WCHAR*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		WCHAR*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		UINT32*			pnServerHash	= NULL,
	OUT		WCHAR*			pszServerName	= NULL,		// 32
	OUT		WCHAR*			pszNationCode	= NULL		// 3
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_LocalCheckPassport2A
( 
 IN			const char*		szPassport,					// 1024
 OUT		char*			pszId			= NULL,		// 32
 OUT		UINT32*			pnNexonSN		= NULL,
 OUT		char*			pszLoginIp		= NULL,		// 32
 OUT		UINT32*			puSex			= NULL,
 OUT		UINT32*			puAge			= NULL,
 OUT		UINT32*			puPwdHash		= NULL,
 OUT		UINT32*			puSsnHash		= NULL,
 OUT		UINT32*			puFlag0			= NULL,
 OUT		UINT32*			puFlag1			= NULL,
 OUT		UINT32*			pnServerHash	= NULL,
 OUT		char*			pszServerName	= NULL,		// 32
 OUT		char*			pszMeta			= NULL		// 1024
 );

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_LocalCheckPassport2W
( 
 IN			const WCHAR*	szPassport,					// 1024
 OUT		WCHAR*			pszId			= NULL,		// 32
 OUT		UINT32*			pnNexonSN		= NULL,
 OUT		WCHAR*			pszLoginIp		= NULL,		// 32
 OUT		UINT32*			puSex			= NULL,
 OUT		UINT32*			puAge			= NULL,
 OUT		UINT32*			puPwdHash		= NULL,
 OUT		UINT32*			puSsnHash		= NULL,
 OUT		UINT32*			puFlag0			= NULL,
 OUT		UINT32*			puFlag1			= NULL,
 OUT		UINT32*			pnServerHash	= NULL,
 OUT		WCHAR*			pszServerName	= NULL,		// 32
 OUT		WCHAR*			pszMeta			= NULL		// 1024
 );

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckPassportA
( 
	IN		const char*		szPassport,					// 1024
	IN		const char*		szUserIp,					// 32
	IN		BOOL			bRemoteOnly		= TRUE,
	OUT		char*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		char*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		char*			pszMeta			= NULL,		// 1024
	OUT		char*			pszNationCode	= NULL		// 3
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckPassportW
( 
	IN		const WCHAR*	szPassport,					// 1024
	IN		const WCHAR*	szUserIp,					// 32
	IN		BOOL			bRemoteOnly		= TRUE,
	OUT		WCHAR*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		WCHAR*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		WCHAR*			pszMeta			= NULL,		// 1024
	OUT		WCHAR*			pszNationCode	= NULL		// 3
	);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
			AUTHCHECK_ERROR_GAMECODE_MISMATCHED
			AUTHCHECK_ERROR_SESSION_NOT_EXIST
			AUTHCHECK_ERROR_DISCONNECTED
			AUTHCHECK_ERROR_SERVER_FAILED
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckSessionA
( 
	IN		BOOL			bRemoteCheck,
	IN		const char*		szPassport,					// 1024
	IN		const char*		szUserIp,					// 32
	IN		const char*		szHWID,						// 64
	IN		UINT32			uGameCode,
	OUT		char*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		char*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		char*			pszNationCode	= NULL,		// 3
	OUT		char*			pszMeta			= NULL,		// 1024
	OUT		UINT8*			puSecureCode	= NULL,
	OUT		UINT8*			puRegisteredPC	= NULL		// Not used
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
			AUTHCHECK_ERROR_SET_LOCALE_NEEDED
			AUTHCHECK_ERROR_USER_IP_MISMATCHED
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
			AUTHCHECK_ERROR_GAMECODE_MISMATCHED
			AUTHCHECK_ERROR_SESSION_NOT_EXIST
			AUTHCHECK_ERROR_DISCONNECTED
			AUTHCHECK_ERROR_SERVER_FAILED
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckSessionW
( 
	IN		BOOL			bRemoteCheck,
	IN		const WCHAR*	szPassport,					// 1024
	IN		const WCHAR*	szUserIp,					// 32
	IN		const WCHAR*	szHWID,						// 64
	IN		UINT32			uGameCode,
	OUT		WCHAR*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		WCHAR*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		WCHAR*			pszNationCode	= NULL,		// 3
	OUT		WCHAR*			pszMeta			= NULL,		// 1024
	OUT		UINT8*			puSecureCode	= NULL,
	OUT		UINT8*			puRegisteredPC	= NULL		// Not used
);

/*
returns :	AUTHCHECK_ERROR_OK
AUTHCHECK_ERROR_INVALID_ARGUMENT
AUTHCHECK_ERROR_INVALID_PASSPORT
AUTHCHECK_ERROR_EXPIRED
AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
AUTHCHECK_ERROR_GAMECODE_MISMATCHED
AUTHCHECK_ERROR_SESSION_NOT_EXIST
AUTHCHECK_ERROR_DISCONNECTED
AUTHCHECK_ERROR_SERVER_FAILED
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckSession2A
( 
	IN		BOOL			bRemoteCheck,
	IN		const char*		szPassport,					// 1024
	IN		const char*		szUserIp,					// 32
	IN		const char*		szHWID,						// 64
	IN		UINT32			uGameCode,
	OUT		char*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		char*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		char*			pszNationCode	= NULL,		// 3
	OUT		char*			pszMeta			= NULL,		// 1024
	OUT		UINT8*			puSecureCode	= NULL,
	OUT		UINT8*			puChannelCode	= NULL,
	OUT		char*			pszChannelUID	= NULL		// 100
);

/*
returns :	AUTHCHECK_ERROR_OK
AUTHCHECK_ERROR_INVALID_ARGUMENT
AUTHCHECK_ERROR_INVALID_PASSPORT
AUTHCHECK_ERROR_EXPIRED
AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
AUTHCHECK_ERROR_SET_LOCALE_NEEDED
AUTHCHECK_ERROR_USER_IP_MISMATCHED
AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
AUTHCHECK_ERROR_GAMECODE_MISMATCHED
AUTHCHECK_ERROR_SESSION_NOT_EXIST
AUTHCHECK_ERROR_DISCONNECTED
AUTHCHECK_ERROR_SERVER_FAILED
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckSession2W
( 
	IN		BOOL			bRemoteCheck,
	IN		const WCHAR*	szPassport,					// 1024
	IN		const WCHAR*	szUserIp,					// 32
	IN		const WCHAR*	szHWID,						// 64
	IN		UINT32			uGameCode,
	OUT		WCHAR*			pszId			= NULL,		// 32
	OUT		UINT32*			pnNexonSN		= NULL,
	OUT		WCHAR*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		UINT32*			puPwdHash		= NULL,
	OUT		UINT32*			puSsnHash		= NULL,
	OUT		UINT32*			puFlag0			= NULL,
	OUT		UINT32*			puFlag1			= NULL,
	OUT		WCHAR*			pszNationCode	= NULL,		// 3
	OUT		WCHAR*			pszMeta			= NULL,		// 1024
	OUT		UINT8*			puSecureCode	= NULL,
	OUT		UINT8*			puChannelCode	= NULL,
	OUT		WCHAR*			pszChannelUID	= NULL		// 100
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
			AUTHCHECK_ERROR_GAMECODE_MISMATCHED
			AUTHCHECK_ERROR_SESSION_NOT_EXIST
			AUTHCHECK_ERROR_DISCONNECTED
			AUTHCHECK_ERROR_SERVER_FAILED
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckSession3A
( 
	IN		BOOL			bRemoteCheck,
	IN		const char*		szPassport,					// 1024
	IN		const char*		szUserIp,					// 32
	IN		const char*		szHWID,						// 64
	IN		UINT32			uGameCode,
	OUT		char*			pszId			= NULL,		// 32
	OUT		UINT64*			pnNexonSN64		= NULL,
	OUT		char*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		char*			pszNationCode	= NULL,		// 3
	OUT		char*			pszMeta			= NULL,		// 1024
	OUT		UINT8*			puSecureCode	= NULL,
	OUT		UINT8*			puChannelCode	= NULL,
	OUT		char*			pszChannelUID	= NULL,		// 100
	OUT		BOOL*			pbNewMembership	= NULL,
	OUT		INT8*			pnMainAuthLevel	= NULL,
	OUT		INT8*			pnSubAuthLevel	= NULL
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
			AUTHCHECK_ERROR_EXPIRED
			AUTHCHECK_ERROR_AUTH_SERVER_SHUTDOWN
			AUTHCHECK_ERROR_SET_LOCALE_NEEDED
			AUTHCHECK_ERROR_USER_IP_MISMATCHED
			AUTHCHECK_ERROR_LOW_LEVEL_PASSPORT
			AUTHCHECK_ERROR_GAMECODE_MISMATCHED
			AUTHCHECK_ERROR_SESSION_NOT_EXIST
			AUTHCHECK_ERROR_DISCONNECTED
			AUTHCHECK_ERROR_SERVER_FAILED
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckSession3W
( 
	IN		BOOL			bRemoteCheck,
	IN		const WCHAR*	szPassport,					// 1024
	IN		const WCHAR*	szUserIp,					// 32
	IN		const WCHAR*	szHWID,						// 64
	IN		UINT32			uGameCode,
	OUT		WCHAR*			pszId			= NULL,		// 32
	OUT		UINT64*			pnNexonSN64		= NULL,
	OUT		WCHAR*			pszLoginIp		= NULL,		// 32
	OUT		UINT32*			puSex			= NULL,
	OUT		UINT32*			puAge			= NULL,
	OUT		WCHAR*			pszNationCode	= NULL,		// 3
	OUT		WCHAR*			pszMeta			= NULL,		// 1024
	OUT		UINT8*			puSecureCode	= NULL,
	OUT		UINT8*			puChannelCode	= NULL,
	OUT		WCHAR*			pszChannelUID	= NULL,		// 100
	OUT		BOOL*			pbNewMembership	= NULL,
	OUT		INT8*			pnMainAuthLevel	= NULL,
	OUT		INT8*			pnSubAuthLevel	= NULL
);

AUTHCHECK_API
UINT32 AuthCheck_GetHashValueA
( 
	const char* szString
);

AUTHCHECK_API
UINT32 AuthCheck_GetHashValueW
(
	const WCHAR* wszString
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckGuestPassportA
( 
	IN	const char *	szPassport,					// 1024
	OUT	INT32 *			pnGameCode		= NULL,
	OUT	INT64 *			pnGuestSN		= NULL,
	OUT char *			pszGuestId		= NULL,		// 32
	OUT char *			pszLoginIp		= NULL,		// 32
	OUT SYSTEMTIME *	pTimeCreated	= NULL,
	OUT SYSTEMTIME *	pTimeToExpire	= NULL
);

/*
returns :	AUTHCHECK_ERROR_OK
			AUTHCHECK_ERROR_INVALID_ARGUMENT
			AUTHCHECK_ERROR_INVALID_PASSPORT
*/
AUTHCHECK_API
AuthCheckError AuthCheck_CheckGuestPassportW
( 
	IN	const WCHAR *	szPassport,					// 1024
	OUT	INT32 *			pnGameCode		= NULL,
	OUT	INT64 *			pnGuestSN		= NULL,
	OUT WCHAR *			pszGuestId		= NULL,		// 32
	OUT WCHAR *			pszLoginIp		= NULL,		// 32
	OUT SYSTEMTIME *	pTimeCreated	= NULL,
	OUT SYSTEMTIME *	pTimeToExpire	= NULL
);

AUTHCHECK_API
UINT32 AuthCheck_LoginA
( 
 IN	const char*		szID,						// 32
 IN	const char*		szPwd,						// 32
 IN	const char*		szIP,						// 32
 IN	UINT32			gameCode,
 IN	UINT32			locale,
 OUT char*			pszPassport			= NULL,	// 1024
 OUT UINT32*			pnUnreadNoteCount	= NULL,
 OUT UINT32*			pnStatusFlag		= NULL,
 OUT UINT32*			pnUpdateInterval	= NULL
 );

AUTHCHECK_API
UINT32 AuthCheck_LoginW
( 
 IN	const WCHAR*	szID,						// 32
 IN	const WCHAR*	szPwd,						// 32
 IN	const WCHAR*	szIP,						// 32
 IN	UINT32			gameCode,
 IN	UINT32			locale,
 OUT WCHAR*			pszPassport			= NULL,	// 1024
 OUT UINT32*			pnUnreadNoteCount	= NULL,
 OUT UINT32*			pnStatusFlag		= NULL,
 OUT UINT32*			pnUpdateInterval	= NULL
 );

AUTHCHECK_API
UINT32 AuthCheck_LogoutA
( 
 IN	const char*		szPassport,		// 1024
 IN	const char*		szIP			// 32
);

AUTHCHECK_API
UINT32 AuthCheck_LogoutW
( 
 IN	const WCHAR*	szPassport,		// 1024
 IN	const WCHAR*	szIP			// 32
);

AUTHCHECK_API
SsnCheckError AuthCheck_CheckSsnFullA
(
	IN	const char*		szPassport,		// 1024
	IN	const char*		szSsnFull		// 14
);

AUTHCHECK_API
SsnCheckError AuthCheck_CheckSsnFullW
(
	IN	const WCHAR*	szPassport,		// 1024
	IN	const WCHAR*	szSsnFull		// 14
);

AUTHCHECK_API
SsnCheckError AuthCheck_CheckSsnSubA
(
	IN	const char*		szPassport,		// 1024
	IN	const char*		szSsnSub		// 8
);

AUTHCHECK_API
SsnCheckError AuthCheck_CheckSsnSubW
(
	IN	const WCHAR*	szPassport,		// 1024
	IN	const WCHAR*	szSsnSub		// 8
);

#if defined( UNICODE ) || defined( _UNICODE )
	#define AuthCheck_LoadConfig			AuthCheck_LoadConfigW
	#define AuthCheck_SetSoapHost			AuthCheck_SetSoapHostW
	#define AuthCheck_GetPassportType		AuthCheck_GetPassportTypeW
	#define AuthCheck_LoadKey				AuthCheck_LoadKeyW
	#define AuthCheck_LocalCheckPassport	AuthCheck_LocalCheckPassportW
	#define AuthCheck_LocalCheckPassport2	AuthCheck_LocalCheckPassport2W
	#define AuthCheck_CheckPassport			AuthCheck_CheckPassportW
	#define AuthCheck_GetHashValue			AuthCheck_GetHashValueW
	#define AuthCheck_CheckGuestPassport	AuthCheck_CheckGuestPassportW
	#define AuthCheck_Login					AuthCheck_LoginW
	#define AuthCheck_Logout				AuthCheck_LogoutW
	#define AuthCheck_CheckSsnFull			AuthCheck_CheckSsnFullW
	#define AuthCheck_CheckSsnSub			AuthCheck_CheckSsnSubW
	#define AuthCheck_CheckSession			AuthCheck_CheckSessionW
	#define AuthCheck_CheckSession2			AuthCheck_CheckSession2W
	#define AuthCheck_CheckSession3			AuthCheck_CheckSession3W
#else
	#define AuthCheck_LoadConfig			AuthCheck_LoadConfigA
	#define AuthCheck_SetSoapHost			AuthCheck_SetSoapHostA
	#define AuthCheck_GetPassportType		AuthCheck_GetPassportTypeA
	#define AuthCheck_LoadKey				AuthCheck_LoadKeyA
	#define AuthCheck_LocalCheckPassport	AuthCheck_LocalCheckPassportA
	#define AuthCheck_LocalCheckPassport2	AuthCheck_LocalCheckPassport2A
	#define AuthCheck_CheckPassport			AuthCheck_CheckPassportA
	#define AuthCheck_GetHashValue			AuthCheck_GetHashValueA
	#define AuthCheck_CheckGuestPassport	AuthCheck_CheckGuestPassportA
	#define AuthCheck_Login					AuthCheck_LoginA
	#define AuthCheck_Logout				AuthCheck_LogoutA
	#define AuthCheck_CheckSsnFull			AuthCheck_CheckSsnFullA
	#define AuthCheck_CheckSsnSub			AuthCheck_CheckSsnSubA
	#define AuthCheck_CheckSession			AuthCheck_CheckSessionA
	#define AuthCheck_CheckSession2			AuthCheck_CheckSession2A
	#define AuthCheck_CheckSession3			AuthCheck_CheckSession3A
#endif

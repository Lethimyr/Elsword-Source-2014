#pragma once



//{{ 허상형 : [2009/11/12] //	핵쉴드 국가별 라이선스 키
#if	 defined( _NEXON_KR_ )		//	한국
	#define HS_LICENSE_KEY	L"A6E13604BC3E9D3838CAC0BA"
	#define HS_GAME_CODE	9928
#elif defined( CLIENT_COUNTRY_TW )	//	대만
	#define HS_LICENSE_KEY L"8F791743D021D76DEDE1D892"
	#define HS_GAME_CODE	5820
#elif defined( CLIENT_COUNTRY_HK )	// 홍콩
	#define HS_LICENSE_KEY L"00476EC7ED0A77525B360F0E"
	#define HS_GAME_CODE	5825
#elif defined( CLIENT_COUNTRY_EU )	// 유럽
	#define HS_LICENSE_KEY L"3364D9435F7FFBC188C74E66"
	#define HS_GAME_CODE	5960
#else							//	기타
	#define HS_LICENSE_KEY L""
	#define HS_GAME_CODE	0
#endif
//}} 허상형 : [2009/11/12] //	핵쉴드 국가별 라이선스 키

//{{ 2010/10/14	조효진	핵쉴드 업데이트 기능
#ifdef HACKSHIELD_AUTO_UPDATE
BOOL	File_Copy_Overwrite(const std::wstring& wstrSrcFilePath, const std::wstring& wstrDstFilePath);
BOOL	HS_PrepareUpdate();
BOOL	HS_Update();
#endif HACKSHIELD_AUTO_UPDATE
//}}

//{{ 2011.3.21	조효진	핵쉴드 모니터링 서버 연동
#ifdef HACKSHIELD_MORNITORING
BOOL	HS_StartMonitor();
#endif HACKSHIELD_MORNITORING
//}}

//{{ 허상형 : [2009/8/3] //	핵쉴드 관련 함수
//	핵쉴드 초기화
BOOL	HS_Init();

//	핵쉴드 서비스 시작
BOOL	HS_StartService();

//	핵쉴드 서비스 일시 정지
BOOL	HS_PauseService();

//	핵쉴드 서비스 재개
BOOL	HS_ResumeService();

//	핵쉴드 서비스 정지
BOOL	HS_StopService();

//	핵쉴드 종료
BOOL	HS_UnInit();

//	콜백함수
int	__stdcall	HS_CallbackProc( long lCode, long lParamSize, void* pParam );

//	에러메시지 출력
void ShowErrorMsg( int iErrorMsg );

//	비정상 종료시 처리 함수
LONG WINAPI GAME_UnhandledExceptionHandler(struct _EXCEPTION_POINTERS *lpTopLevelExceptionFilter);

//	에러 발견시 메일 전송 함수
void	SendHackShieldLog( int iError, char *wszMsg );

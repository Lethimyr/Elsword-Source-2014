// 국가에 무관하게 표준으로 쓸 수 있는 셋팅을 다 해 두고
// 꼭 바꿔야 하는 국가는 그 위에 define을 덮도록 합니다. by 박진웅 13.11.12

#define MAX_RETRY						64                          // 최대 재시도 횟수
#define MAX_DELAY						128                         // 재시도 지연 간격

#define LOAD_CUR_FILE_BAR_X				48
#define LOAD_CUR_FILE_BAR_Y				517
#define LOAD_CUR_FILE_BAR_WIDTH			530
#define LOAD_CUR_FILE_BAR_HEIGHT		31

#define LOAD_CUR_FILE_PER_X				449
#define LOAD_CUR_FILE_PER_Y				285
#define LOAD_CUR_FILE_PER_WIDTH			60
#define LOAD_CUR_FILE_PER_HEIGHT		30

#define LOAD_NEW_CUR_FILE_BAR_X			119
#define LOAD_NEW_CUR_FILE_BAR_Y			493
#define LOAD_NEW_CUR_FILE_BAR_WIDTH		535
#define LOAD_NEW_CUR_FILE_BAR_HEIGHT	12

#define LOAD_ALL_FILE_BAR_X				48
#define LOAD_ALL_FILE_BAR_Y				517
#define LOAD_ALL_FILE_BAR_WIDTH			530
#define LOAD_ALL_FILE_BAR_HEIGHT		31

#define LOAD_NEW_ALL_FILE_BAR_X			119
#define LOAD_NEW_ALL_FILE_BAR_Y			517
#define LOAD_NEW_ALL_FILE_BAR_WIDTH		535
#define LOAD_NEW_ALL_FILE_BAR_HEIGHT	12

#define LOAD_ALL_FILE_PER_X				449
#define LOAD_ALL_FILE_PER_Y				305
#define LOAD_ALL_FILE_PER_WIDTH			60
#define LOAD_ALL_FILE_PER_HEIGHT		30

/// 게이지 크기
#define GAUGE_NEW_BAR_WIDTH				536
#define GAUGE_NEW_BAR_HEIGHT			11

#define BANNER_WIDTH					722
#define BANNER_HEIGHT					391
#define BANNER_X						9
#define BANNER_Y						45


#define PATCHER_FONT					L"Tahoma"
#define PATCHER_FONT_SIZE				14



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(PATCHER_COUNTRY_DE) || (PATCHER_COUNTRY_ES) || (PATCHER_COUNTRY_FR) || (PATCHER_COUNTRY_IT) || (PATCHER_COUNTRY_PL) || (PATCHER_COUNTRY_EN)
	#define PATCHER_COUNTRY_EU
#elif defined(PATCHER_COUNTRY_TW) || (PATCHER_COUNTRY_HK)
	#define PATCHER_COUNTRY_TWHK
#endif //PATCHER_COUNTRY_XX

// elsword.exe 자체 패치하는 기능. 기본 on
#define PATCHER_SELF_PATCH

#ifdef PATCHER_COUNTRY_EU

	#define MUTEX_NAME "ElswordX2_EU"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "pxk19slammsu286nfha02kpqnf729ck"		// argv[]의 제일 뒤가 이 스트링이 맞아야 통과.
	#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_JP)

	// 패쳐 자체 패치 기능 일본 off
	#undef PATCHER_SELF_PATCH

	#define MUTEX_NAME "ElswordX2_JP"
	#define FOREIGN_CONFIG_FILE		// 외부파일로 경로 읽는 부분 추가

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_TWHK)

	#undef PATCHER_FONT
	#undef PATCHER_FONT_SIZE
	#define PATCHER_FONT		L"DFPPOP1W7U-B5"
	#define PATCHER_FONT_SIZE	15

	#ifdef _SERVICE_
		#define BLOCK_MANUAL_LOGIN

		#ifdef _OPEN_TEST_
			#define BLOCK_MANUAL_LOGIN_BYPASS_FOR_TEST_SERVER
		#endif _OPEN_TEST_
	#endif

	#define LOAD_WEB_MAIN		// Main 창에 Web 로드 여부 결정함
	#ifdef PATCHER_COUNTRY_HK
		#define MUTEX_NAME "ElswordX2_HK"
		const WCHAR g_pcBannerLink[] = L"http://www.elsonline.com.hk/launcher/launcher.aspx";	// 홍콩 라이브 URL
	#else //PATCHER_COUNTRY_HK
		#define MUTEX_NAME "ElswordX2_TW"
		const WCHAR g_pcBannerLink[] = L"https://tw.event.beanfun.com/ELSWORD/PATCH/PATCH.ASPX";		// 대만 라이브 URL
	#endif //PATCHER_COUNTRY_HK

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_US)

	#define MUTEX_NAME "ElswordX2_US"

	//-- 2010. 12. 13.  지헌 : XTRAP 런쳐 flag
	#ifdef _SERVICE_
		#define LAUNCHER_USE_XTRAP
		#define PATCHER_RUN_ONLY "pxk19slammsu286nfha02kpqnf729ck"		// 클라 ktdx.h와 통일시킬 것. argv[]의 제일 뒤에 이 스트링을 붙여서 클라를 실행한다.
	#endif _SERVICE_

	#define LOAD_WEB_MAIN		// Main 창에 Web 로드 여부 결정함
	#if !defined(_SERVICE_)
		const WCHAR g_pcBannerLink[] = L"";														// 사내 빈칸
	#elif defined(_OPEN_TEST_)
		const WCHAR g_pcBannerLink[] = L"http://launchertest.elswordonline.com/";	// 미국 테섭
	#else //defined(_SERVICE_)
		const WCHAR g_pcBannerLink[] = L"http://launcher.elswordonline.com/";		// 미국 본섭
	#endif //defined(_SERVICE_)

	//#define PATCHER_CHANNELING_AERIA// 아리아 채널링 패쳐 작업. 아리아 중간 런쳐 있으면 elsworld.exe 바로 실행 안되도록

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_TH)

	#define MUTEX_NAME "ElswordX2_TH"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "8f9slxa02nkp29ak1u26mqpcmshnf7k"		// argv[]의 제일 뒤가 이 스트링이 맞아야 통과.
	#endif _SERVICE_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_ID)

	#define MUTEX_NAME "ElswordX2_ID"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "8f9slxa02nkp29ak1u26mqpcmshnf7k"		// argv[]의 제일 뒤가 이 스트링이 맞아야 통과.
	#endif _SERVICE_

	#define LOAD_WEB_MAIN		// Main 창에 Web 로드 여부 결정함
	const WCHAR g_pcBannerLink[] = L"http://elsword.netmarble.co.id/launcher/launcher.html";

	#define DLL_COPY_RUN_ELSWORD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_BR)

	#define MUTEX_NAME "ElswordX2_BR"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "pxk19slammsu286nfha02kpqnf729ck"		// argv[]의 제일 뒤가 이 스트링이 맞아야 통과.
	#endif _SERVICE_

	#define LOAD_WEB_MAIN		// Main 창에 Web 로드 여부 결정함
	const WCHAR g_pcBannerLink[] = L"http://levelupgames.uol.com.br/Launchers/Elsword";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_PH)

	#define MUTEX_NAME "ElswordX2_PH"

	#ifdef _SERVICE_
		#define PATCHER_RUN_GARENA_MESSENGER // 2013.07.31 lygan_조성욱 // 동남아시아 런처 실행용으로 가레나 메신저를 통해서 인증값을 받아서 처리 해야 한다.
	#endif _SERVICE_

	#define DLL_COPY_RUN_ELSWORD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_IN)

	#define MUTEX_NAME "ElswordX2_IN"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "8f9slxa02nkp29ak1u26mqpcmshnf7k"		// argv[]의 제일 뒤가 이 스트링이 맞아야 통과.
	#endif _SERVICE_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else //PATCHER_COUNTRY_XX

	#define MUTEX_NAME "ElswordX2"

#endif //PATCHER_COUNTRY_XX

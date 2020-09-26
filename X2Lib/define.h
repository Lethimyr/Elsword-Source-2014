


#if defined( CLIENT_COUNTRY_TWHK )

#ifdef _OPEN_TEST_
#define PATCH_ORG_SRC		"http://210.208.90.241/Patch/"						// 테섭
#else _OPEN_TEST_
#define PATCH_ORG_SRC		"http://patch.elsword.com.tw/"						// 본섭
#endif _OPEN_TEST_

#define PATCH_TEST_SRC		"http://192.168.71.247/"							// 사내
#define PATCH_PATH_FILE		"PatchPath.dat"

#elif defined( CLIENT_COUNTRY_JP )

#ifdef _OPEN_TEST_
#define PATCH_ORG_SRC		"http://192.168.71.24/client/patch/"				// 테섭
#else _OPEN_TEST_
#define PATCH_ORG_SRC		"http://192.168.71.57/JP_Client_Real/"				// 본섭
#endif _OPEN_TEST_

#define PATCH_TEST_SRC		"http://192.168.71.239/"							// 사내
#define PATCH_PATH_FILE		"PatchPath.dat"

#elif defined( CLIENT_COUNTRY_EU )

#define PATCH_EU_SERVICE_SRC	"S"
#define PATCH_EU_TEST_SRC		"T"
#define PATCH_EU_INTERNAL_SRC	"I"
#define PATCH_PATH_FILE	"PatchPath_.dat"

#elif defined( CLIENT_COUNTRY_US )

#if defined (_OPEN_TEST_)
#	define PATCH_ORG_SRC		"http://222.127.25.26/patch/"	// 오픈테섭
#	define PATCH_PATH_FILE		"PatchPath.dat"   // 오픈테섭
#elif defined (_SERVICE_)
#	define PATCH_ORG_SRC		"http://gamepatch.elswordonline.com/"	// 본섭
#	define PATCH_PATH_FILE		"PatchPath.dat"     // 본섭
#else
#	define PATCH_TEST_SRC		"http://192.168.71.240/"                  // 국내사내버전
#	define PATCH_PATH_FILE		"PatchPath_.dat"     // 테스트 
#endif

#elif defined( CLIENT_COUNTRY_CN )

#ifdef CHINA_PATH_SERVER_CONNECT

#ifdef _SERVICE_

#ifdef _OPEN_TEST_
const char g_pcGameServerListXmlAddr[] = "testels.ztgame.net";
const char g_pcGameServerListXmlPath[] = "autopatchelstest/srvlist.xml";
#else _OPEN_TEST_
const char g_pcGameServerListXmlAddr[] = "updateels.ztgame.net";
const char g_pcGameServerListXmlPath[] = "autopatchels/srvlist.xml";
#endif _OPEN_TEST_
#else _SERVICE_
const char g_pcGameServerListXmlAddr[] = "192.168.71.235";
const char g_pcGameServerListXmlPath[] = "patch/srvlist_test.xml";
#endif _SERVICE_

#else CHINA_PATH_SERVER_CONNECT

#define PATCH_ORG_SRC		""
#define PATCH_ORG_SRC		""
#define PATCH_PATH_FILE		""

#endif CHINA_PATH_SERVER_CONNECT

#elif defined( CLIENT_COUNTRY_ID )

#ifdef _SERVICE_
	#ifdef _OPEN_TEST_
		#define PATCH_ORG_SRC		"http://203.209.190.30"
		#define PATCH_TEST_SRC		"http://203.209.190.30"
	#else _OPEN_TEST_
		#define PATCH_ORG_SRC		"http://update.netmarble.co.id/Elsword/"
		#define PATCH_TEST_SRC		"http://update.netmarble.co.id/Elsword/"
	#endif _OPEN_TEST_

#else _SERVICE_
	#define PATCH_ORG_SRC		"http://192.168.71.238/Internal_ID/"
	#define PATCH_TEST_SRC		"http://192.168.71.238/Internal_ID/"
#endif _SERVICE_

#define PATCH_PATH_FILE		"PatchPath.dat"     // 본섭

#elif defined( CLIENT_COUNTRY_TH )

#ifdef _SERVICE_
	#ifdef _OPEN_TEST_
		#define PATCH_ORG_SRC		"http://119.46.249.11/patch/";										// Thailand Beta
		#define PATCH_PATH_FILE		"patchpath.dat"												// Thailand Beta
	#else _OPEN_TEST_
		#define PATCH_ORG_SRC		"http://auto-es.asiasoft.co.th/real/";								// Thailand Service
		#define PATCH_PATH_FILE		"patchpath.dat"													// Thailand Servie
	#endif _OPEN_TEST_
#else _SERVICE_
	#define PATCH_TEST_SRC		"http://192.168.71.236/";											// Thailand Beta
	#define PATCH_PATH_FILE		"Patchpath_TH_INTERNAL.dat"											// Thailand Bet
#endif _SERVICE_
#else
//---------------------------------------------------------------------------------
// 해외팀 사용 안함 국내 것!
#if defined(TAIWAN)
#define PATCH_ORG_SRC       "http://elsword.nefficient.co.kr/elsword/SERVICE/PATCH/"	// 넥슨 패치 경로
#define PATCH_TEST_SRC		"http://210.208.90.241/Patch/"								// 대만사내버전
#else
	#if defined (_OPEN_TEST_2_)
	#	define PATCH_ORG_SRC		"http://elsword-nexon17.ktics.co.kr/BATTLE_FIELD_BETA/Patch/"
	#	define PATCH_PATH_FILE		"PatchPathBattleFieldBeta.dat"
	#elif defined (_OPEN_TEST_)
	#	define PATCH_ORG_SRC		"http://elsword-nexon17.ktics.co.kr/BETA/Patch/"	// 오픈테섭
	#	define PATCH_PATH_FILE		"PatchPathBeta.dat"
	#elif defined (_SERVICE_)
	#	define PATCH_ORG_SRC		"http://elsword-nexon17.ktics.co.kr/SERVICE/PATCH/"	// 본섭
	#	define PATCH_PATH_FILE		"PatchPath.dat"
	#elif defined (_IN_HOUSE_SERVICE_READY_QA_)
	#	define PATCH_TEST_SRC		"http://192.168.66.204/Patch/"						// 국내 QA버전
	#	define PATCH_PATH_FILE		"PatchPathQA_.dat"
	#else
	#	define PATCH_TEST_SRC		"http://192.168.66.204/Patch/"						// 국내사내버전
	#	define PATCH_PATH_FILE		"PatchPath_.dat"
	//#	define PATCH_TEST_SRC		"http://192.168.66.204/Patch/"						// 국내 장기 프로젝트 서버버전
	//#	define PATCH_PATH_FILE		"PatchPathBattleField_.dat"
	#endif
#endif	
//---------------------------------------------------------------------------------
#endif


#define MAX_RETRY           64                          // 최대 재시도 횟수
#define MAX_DELAY           128                         // 재시도 지연 간격


#define LOAD_CUR_FILE_BAR_X			72
#define LOAD_CUR_FILE_BAR_Y			285
#define LOAD_CUR_FILE_BAR_WIDTH		373 - 5
#define LOAD_CUR_FILE_BAR_HEIGHT	473 - 462

#define LOAD_CUR_FILE_PER_X			449
#define LOAD_CUR_FILE_PER_Y			285
#define LOAD_CUR_FILE_PER_WIDTH		60
#define LOAD_CUR_FILE_PER_HEIGHT	30

#define LOAD_ALL_FILE_BAR_X			72
#define LOAD_ALL_FILE_BAR_Y			305
#define LOAD_ALL_FILE_BAR_WIDTH		373 - 5
#define LOAD_ALL_FILE_BAR_HEIGHT	473 - 462

#define LOAD_ALL_FILE_PER_X			449
#define LOAD_ALL_FILE_PER_Y			305
#define LOAD_ALL_FILE_PER_WIDTH		60
#define LOAD_ALL_FILE_PER_HEIGHT	30

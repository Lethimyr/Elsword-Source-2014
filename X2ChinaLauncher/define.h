
#ifndef GATHER_PATCH_INFO
#define GATHER_PATCH_INFO
#endif GATHER_PATCH_INFO


//#define PATCH_ORG_SRC       "http://elsword.nefficient.co.kr/elsword/SERVICE/PATCH/"	// 넥슨 패치 경로
//#define PATCH_TEST_SRC		"http://192.168.140.223/Patch2/";                  // 국내사내버전

#ifdef _SERVICE_
#define PATCH_PATH_FILE			"PatchPath.dat"
#else
#define PATCH_PATH_FILE			"PatchPath_.dat"
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

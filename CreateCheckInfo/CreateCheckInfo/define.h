

#if defined(TAIWAN)
#define PATCH_ORG_SRC       "http://elsword.nefficient.co.kr/elsword/SERVICE/PATCH/"	// ³Ø½¼ ÆÐÄ¡ °æ·Î
#define PATCH_TEST_SRC		"http://210.208.90.241/Patch/"                  // ´ë¸¸»ç³»¹öÀü
#else
#if defined (_OPEN_TEST_)
#	define PATCH_ORG_SRC       "http://elsword-nexon17.ktics.co.kr/BETA/Patch/"	// ¿ÀÇÂÅ×¼·
#elif defined (_SERVICE_)
#	define PATCH_ORG_SRC       "http://elsword-nexon17.ktics.co.kr/SERVICE/PATCH/"	// º»¼·
#else
#	define PATCH_ORG_SRC       "http://elsword-nexon17.ktics.co.kr/SERVICE/PATCH/"	//  ³Ø½¼ ÆÐÄ¡ °æ·Î
#endif
#define PATCH_TEST_SRC		"http://sekinternal.kog.co.kr/Patch/"                  // ±¹³»»ç³»¹öÀü
#endif 


#if defined (KR_SERVICE_PATCHER)
#	define PATCH_PATH_FILE			"PatchPath.dat"     // º»¼·
#elif defined (KR_OPENTEST_PATCHER)
#	define PATCH_PATH_FILE			"PatchPathBeta.dat"   // ¿ÀÇÂÅ×¼·
#else
#	define PATCH_PATH_FILE			"PatchPath_.dat"     // Å×½ºÆ® 
#endif


#define MAX_RETRY           64                          // ÃÖ´ë Àç½Ãµµ È½¼ö
#define MAX_DELAY           128                         // Àç½Ãµµ Áö¿¬ °£°Ý


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

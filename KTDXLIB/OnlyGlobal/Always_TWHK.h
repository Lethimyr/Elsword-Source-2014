// undef 시작
// 기존 비던 셋옵에 문제가 있어서 undef
// 비던 개편(하멜 추가 및 장비 변경) 이 들어가면 undef 풀어줘야 합니다.
#undef ADJUST_SECRET_ITEM_OPTION
// undef 끝

#if defined( _SERVICE_ ) || defined( _OPEN_TEST_ )	// 테스트 및 라이브 버전만 적용
#define	HACK_SHIELD
//#define ALWAYS_CHECK_KOM_LIVE_CHECK
#endif

#if defined( _SERVICE_ ) && !defined( _OPEN_TEST_ )	// 라이브 버전만 적용
#define BEANFUN_UPGRADE_HOMEPAGE
#endif

#if !defined( _SERVICE_ )
#define SHOW_ATTACK_BOX_DUMMY
#endif

#ifdef HACK_SHIELD
#define	HS_LMP
#define HS_NO_ADMIN_NEED
#define HACKSHIELD_AUTO_UPDATE
#define HACKSHIELD_MORNITORING
#define ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
//#define ADDITIONAL_HS							// 메시지 후킹 방지 디파인. 이 디파인을 사용하면 대만 IME의 메시지 후킹이 막혀 채팅을 할 수 없으므로 사용하지 않음.
#define HS_REQUEST_OPTION
#endif HACK_SHIELD

#undef REMOVE_KR_SERVER_TEXTURE
#undef PC_BANG_BENEFIT_GUIDE
#undef NEXON_CASH_CHARGE_NEW
#undef NO_PRESENT_BUTTON
#undef PATCHER_RUN_ONLY

#define NO_GAMEGUARD
#define SCRIPT_SELECT
#define NEW_LOGIN_PROCESS						// 해외팀 로그인 방식
#define _LANGUAGE_FIX_CHINESE_					// 중국어라서 고쳐야 하는 부분 ( 일단은 닉네임 필터링에서 한글제한->중국어제한 )
#define _LANGUAGE_FIX_TW_HK						// 언어 어순 등의 문제로 고치는 부분
#define ARGUMENT_LOGIN							// argv[1], [2] 들어오면 오토로그인, argv[1]만 들어오면 패처로 실행된 건지 체크 후 매뉴얼 로그인. 안 들어오면 종료
#define USE_MAXLEVEL_LIMIT_VAL const int g_iMaxLevel = 70;	// MAXLevel 수정 시 GameSysValTable.lua에서도 값 수정해야함
#define USE_CHANNEL_SERVER_IP_AT_SCRIPT			// 허상형 : [2012/6/14] //	SERVER_GROUP_UI_ADVANCED 쓸때 중국 빼고 전국가 적용 , 중국은 런쳐에서 아규먼트로 받아옴
#define CHEAT_WCHART_TO_UTF8					// 2012. 07. 06 lygan_조성욱 // 중문 들어간 캐릭터 닉네임으로 치트 사용시 글자 깨져서 치트 안되는 문제 수정
#define HIDE_HAIR_BY_EP_AC_FACE2				// 특정 아이템(악세서리 얼굴(중))을 장착시 헤어가 보이지 않도록 하기 위한 작업(아누비스의 투구)
#define SEARCH_UNIT_NICKNAME_TW					// 닉네임으로 친구 찾기
#define USE_FREE_TYPE_SUB_FONT					// 프리타입 서브 폰트 기능

#ifdef CLIENT_COUNTRY_TW
#undef REFORM_ENTRY_POINT_FOR_ONE_SERVER_GROUP
#endif CLIENT_COUNTRY_TW

#ifdef CLIENT_COUNTRY_HK
#undef SERVER_INTEGRATION_CHANNEL_NAME_FIX
#define REMOVE_INVITE_PARTY_SERVER_BUTTON		// 작업내용: 파티 초대 팝업에서 솔레스 , 가이아 버튼 제거
#endif CLIENT_COUNTRY_HK

//////////////////////////////////////////////////////////////////////////
// 이름: 이지헌
// 아이디: ninil
// 작업시작날짜: 2013-04-02
// 패치날짜: 2013-04-24
// 작업내용: 몬스터 카드 이벤트. 일정확률로 적이 소환되는 카드
// 수정내용: 9월 5일 최민철 수정 여러마리 소환되게 수정
/*
#define EVENT_MONSTER_CARD_SUMMON_ENEMY
#define EVENT_MONSTER_CARD_ALLY_ID	782	// 아군 몬스터 ID
#define EVENT_MONSTAR_CARD_RATE		50		// 적이 소환될 확률
#define EVENT_MONSTER_CARD_ENEMY_ID	783	// 적 몬스터 ID
#define EVENT_MONSTER_CARD_MAX_ENENY	4	// 소환할 몬스터 수
*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 이름: 김준환
// 아이디: jhkim86
// 작업시작날짜: 2013-05-21\
// 패치날짜: 미정
// 작업내용: 아담스의 상점
//#define ALWAYS_EVENT_ADAMS_UI_SHOP
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 이름: 최민철
// 아이디: hohocmc
// 작업시작날짜: 2014-01-07
// 패치날짜: 2014-01-15
// 작업내용: 춘절 특별 상점 폭죽 아이템
#define EVENT_FIRE_CRACKER_THROW_ITEM
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 이름: 최민철
// 아이디: hohocmc
// 작업시작날짜: 2013-05-10
// 패치날짜: 	
// 작업내용: 해외팀 공용 라인브레이커 
#define CLIENT_GLOBAL_LINEBREAK
//////////////////////////////////////////////////////////////////////////

// 2012.09.12 임홍락 말줄임
#define ELLIPSE_GLOBAL

//////////////////////////////////////////////////////////////////////////
// 이름: 조성욱
// 아이디: darkstarbt
// 작업시작날짜: 2013-05-29
// 패치날짜: 2013-06-19
// 작업내용: 칭호 설명 부분 스크롤 다는 기능
#define TITLE_DESCRIPTION_RENEWAL
//////////////////////////////////////////////////////////////////////////
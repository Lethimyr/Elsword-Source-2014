#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )     // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
_ENUM( S_EXIT )

_ENUM( S_UID_UPDATED )
_ENUM( S_LOGINED )

//{{ 2011. 06. 23	최육사	2차 비번 예외처리
//#ifdef SERV_CHECK_SECOND_PW_STATE
_ENUM( S_CHECK_SECOND_PW )
//#endif SERV_CHECK_SECOND_PW_STATE
//}}
_ENUM( S_SERVER_SELECT )	// 여기까지는 순서 바꾸지 말아야 한다. 인증 과정상 여기까지 순차적으로 흘러온다.

_ENUM( S_FIELD_MAP )
_ENUM( S_ROOM )

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

// S_LOGINED 부터 STATE_SENTINEL 전까지 인증된 유저이다.
#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( I_TO_INIT )     // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
_ENUM( I_TO_EXIT )

_ENUM( I_TO_UID_UPDATED )
_ENUM( I_TO_LOGINED )

_ENUM( I_TO_SERVER_SELECT )
//{{ 2011. 06. 23	최육사	2차 비번 예외처리
//#ifdef SERV_CHECK_SECOND_PW_STATE
_ENUM( I_TO_CHECK_SECOND_PW )
//#endif SERV_CHECK_SECOND_PW_STATE
//}}

_ENUM( I_TO_FIELD_MAP )
_ENUM( I_TO_ROOM )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

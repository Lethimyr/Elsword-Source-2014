#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )             // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
_ENUM( S_CLOSE )
_ENUM( S_WAIT )
//{{ 2012. 05. 23	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
_ENUM( S_TIME_COUNT )
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}
_ENUM( S_LOAD )
_ENUM( S_PLAY )
_ENUM( S_RESULT )
//{{ 2012. 06. 07	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
_ENUM( S_RETURN_TO_FIELD )
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2013. 04. 15	최육사	어둠의 문 개편
//#ifdef SERV_NEW_DEFENCE_DUNGEON
_ENUM( S_WAIT_FOR_DEFENCE )
//#endif SERV_NEW_DEFENCE_DUNGEON
//}}

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
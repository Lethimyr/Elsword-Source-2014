#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )
_ENUM( S_LOGINED )
//{{ 2012. 07. 02	최육사		대기열 시스템
//#ifdef SERV_WAIT_QUEUE_SYSTEM
_ENUM( S_WAIT_QUEUE )
//#endif SERV_WAIT_QUEUE_SYSTEM
//}}
_ENUM( S_EXIT )

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

// S_LOGINED 부터 STATE_SENTINEL 전까지 인증된 유저이다.
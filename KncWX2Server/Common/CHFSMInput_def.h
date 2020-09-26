#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( I_TO_AUTHENTICATED )
//{{ 2012. 07. 02	최육사		대기열 시스템
//#ifdef SERV_WAIT_QUEUE_SYSTEM
_ENUM( I_TO_WAIT_QUEUE )
//#endif SERV_WAIT_QUEUE_SYSTEM
//}}
_ENUM( I_TO_EXIT )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

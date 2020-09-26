#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )             // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
_ENUM( S_CLOSE )
_ENUM( S_AUTO_PARTY_MAKING )
_ENUM( S_CHECK_ACCEPT )
_ENUM( S_REGROUP_PARTY )
_ENUM( S_PLAY )

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
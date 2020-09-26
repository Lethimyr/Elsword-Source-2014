#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( STATE_ZERO_NO_USE )

_ENUM( S_INIT )             	// 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
_ENUM( S_CLOSE )
_ENUM( S_MAKING_PARTY )			// 파티가 생성 중인 상태
_ENUM( S_WAIT )					// 파티가 생성 완료되고난 후 default상태
_ENUM( S_CHECK_PLAY_CONDITION ) // 게임 시작전에 조건 체크
_ENUM( S_CHECK_AUTO_PARTY )		// 자동 파티 신청을 위한 체크 중인 상태
_ENUM( S_ACCEPT_FOR_PLAY )		// 파티 게임을 하기 위해 accept의견을 묻는 상태
_ENUM( S_PREPARE_FOR_PLAY )		// 모든 파티원들의 accept의견을 확인했고 게임 시작을 준비하는 상태

_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
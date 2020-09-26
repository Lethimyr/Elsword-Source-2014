//////////////////////////////////////////////////////////////////////////
// 로봇 전용 FSM class

// 2008.12.06   최육사   GSFSM으로 신광장 버전의 로봇으로 수정하기에 한계가 있어 RBFSM을 따로 만듬



#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( STATE_ZERO_NO_USE )

	_ENUM( S_INIT )     // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
	_ENUM( S_EXIT )

	_ENUM( S_CHANNEL_SELECT )

	_ENUM( S_UID_UPDATED )
	_ENUM( S_LOGINED )

	_ENUM( S_SERVER_SELECT )
	_ENUM( S_UNIT_SELECT )

	_ENUM( S_FIELD_MAP )
	
	_ENUM( S_PVP_LOBBY )
	_ENUM( S_PARTY )
	_ENUM( S_TRAINING_CENTER )

	_ENUM( S_ROOM )

	_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

// S_LOGINED 부터 STATE_SENTINEL 전까지 인증된 유저이다.
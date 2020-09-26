#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( I_TO_INIT )     // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
	_ENUM( I_TO_EXIT )

	_ENUM( I_TO_CHANNEL_SELECT )

	_ENUM( I_TO_UID_UPDATED )
	_ENUM( I_TO_LOGINED )

	_ENUM( I_TO_SERVER_SELECT )
	_ENUM( I_TO_UNIT_SELECT )

	_ENUM( I_TO_FIELD_MAP )

	_ENUM( I_TO_PVP_LOBBY )
	_ENUM( I_TO_PARTY )
	_ENUM( I_TO_TRAINING_CENTER )

	_ENUM( I_TO_ROOM )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif

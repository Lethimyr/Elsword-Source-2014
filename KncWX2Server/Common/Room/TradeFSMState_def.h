#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( STATE_ZERO_NO_USE )

	_ENUM( S_INIT )             // 초기 상태는 무조건 1이어야 한다. 0은 문제가 생겼을 경우 사용. 초기에 1로 초기화함.
	_ENUM( S_CLOSE )
	_ENUM( S_WAIT )
	_ENUM( S_CONFIRM )
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
//#ifdef SERV_TRADE_DDOS_DEFENCE
	_ENUM( S_DB_UPDATE )
//#endif SERV_TRADE_DDOS_DEFENCE
	//}}

	_ENUM( STATE_SENTINEL )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
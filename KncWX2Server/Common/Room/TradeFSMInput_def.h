#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( I_TO_INIT )
	_ENUM( I_TO_CLOSE )
	_ENUM( I_TO_WAIT )
	_ENUM( I_TO_CONFIRM )
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
//#ifdef SERV_TRADE_DDOS_DEFENCE
	_ENUM( I_TO_DB_UPDATE )
//#endif SERV_TRADE_DDOS_DEFENCE
	//}}

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
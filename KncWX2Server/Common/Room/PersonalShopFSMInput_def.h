#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( I_TO_INIT )
	_ENUM( I_TO_CLOSE )
	_ENUM( I_TO_WAIT )
	_ENUM( I_TO_SHOP )
	//{{ 2011. 04. 12	최육사	대리 상인
//#ifdef SERV_PSHOP_AGENCY	
	_ENUM( I_TO_STOP_SALE )
//#endif SERV_PSHOP_AGENCY
	//}}

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
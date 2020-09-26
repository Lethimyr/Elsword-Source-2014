#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM( I_TO_INIT )
	_ENUM( I_TO_CLOSE )
	_ENUM( I_TO_WAIT )

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
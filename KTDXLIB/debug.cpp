#include "stdafx.h"
#include <stdarg.h>
#include <windows.h>
#include <stdio.h>

int debugPrintf( char *fmt, ... ) {
	 va_list l;
	 va_start( l, fmt );
	 char buf[1024];
#ifdef	CONVERSION_VS
	 vsprintf_s( buf, _countof(buf), fmt, l );
	 ZeroMemory( buf, 1024 );
#else	CONVERSION_VS
	 vsprintf( buf, fmt, l );
#endif	CONVERSION_VS	 
	 va_end( l );
	 OutputDebugStringA( buf );
	 return 0;
}

int debugWPrintf( WCHAR *fmt, ... ) 
{
	va_list l;
	va_start( l, fmt );
	WCHAR buf[1024];
#ifdef	CONVERSION_VS
	vswprintf( buf, _countof(buf), fmt, l );
#else	CONVERSION_VS
	vswprintf( buf, fmt, l );
#endif	CONVERSION_VS	
	va_end( l );
	OutputDebugStringW( buf );
	return 0;
}
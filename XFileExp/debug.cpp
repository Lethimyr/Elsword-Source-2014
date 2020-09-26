#include <stdarg.h>
#include <windows.h>
#include <stdio.h>

int debugPrintf( char *fmt, ... ) {
	 va_list l;
	 va_start( l, fmt );
	 char buf[1024];
	 vsprintf( buf, fmt, l );
	 va_end( l );
	 OutputDebugStringA( buf );
	 return 0;
}

int debugWPrintf( WCHAR *fmt, ... ) 
{
	va_list l;
	va_start( l, fmt );
	WCHAR buf[1024];
	vswprintf( buf, fmt, l );
	va_end( l );
	OutputDebugStringW( buf );
	return 0;
}
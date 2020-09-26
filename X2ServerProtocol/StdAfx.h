#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#pragma comment( lib, "ws2_32.lib" )

#include <Winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <atltime.h>    // CString
#include <time.h>

#pragma warning ( disable : 4702 )
#pragma warning ( disable : 4995 )
#pragma warning ( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds


#ifdef _DEBUG
#define  DebugAssert( exp ) { if( exp == false ) __asm int 3 }
#else
#define  DebugAssert( exp ) { exp; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE( p )	if( p != NULL ){ delete p; p = NULL; }
#endif
#define ERRORMSG( msg )		MessageBox( NULL, msg, L"Error", MB_OK | MB_ICONERROR );




#include <Winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <time.h>

//#include <KNCSingleton.h>

#include <ImportKncSerializer.h>
#include <ImportKncDebug.h>
#include <ImportKncSecurity.h>

//#include <boost/utility.hpp>
//#include <boost/bind.hpp>

#include "RTTI.h"
#include "Socket/NetCommon.h"

#include "X2ServerProtocolLib.h"
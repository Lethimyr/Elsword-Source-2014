// stdafx.h : 잘 변경되지 않고 자주 사용하는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows 헤더에서 거의 사용되지 않는 내용을 제외시킵니다.
#endif

// 아래 지정된 플랫폼보다 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0410 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0400	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소
#include <afxext.h>         // MFC 익스텐션

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxole.h>

#include <CdErr.h>
#include <shlwapi.h>

extern UINT g_uCustomClipbrdFormat;
extern bool g_bNT;

// To do..
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "zlibd.lib" )
#else
#pragma comment( lib, "zlib.lib" )
#endif

#pragma comment( lib, "zlibstat.lib" )

#include <process.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>


inline void MakeUpperCase(char* str) 
{
	for( char* i = str; (*i) != 0; i++ ) 
		*i = (char)toupper(*i);
}

inline void MakeUpperCase(std::string &str) 
{
	for( std::string::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (char)toupper(*i);
}

inline void MakeUpperCase(WCHAR* str) 
{
	for( WCHAR* i = str; (*i) != 0; i++ ) 
		*i = (WCHAR)towupper(*i);
}

inline void MakeUpperCase(std::wstring &str) 
{
	for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (WCHAR)towupper(*i);
}

inline void ConvertCharToWCHAR( std::wstring& dest, const char* pSrc )
{
	if ( pSrc == NULL )
		return;

	int iSize = strlen( pSrc );
	if ( iSize <= 0 )
		return;

	WCHAR* wcharBuf = (WCHAR*) alloca( (iSize + 1) * sizeof( WCHAR ) );
	ZeroMemory( wcharBuf, (iSize + 1) * sizeof( WCHAR ) );

	MultiByteToWideChar( CP_ACP, 0, pSrc, -1, wcharBuf, iSize );
	dest = wcharBuf;
}
inline void ConvertWCHARToChar( std::string& dest, const WCHAR* pSrc )
{
	if ( pSrc == NULL )
		return;

	int iSize = wcslen( pSrc ) * sizeof( WCHAR );
	if ( iSize <= 0 )
		return;

	char* charBuf = (char*) alloca( iSize + 1 );
	ZeroMemory( charBuf, iSize + 1 );

	WideCharToMultiByte( CP_ACP, 0, pSrc, -1, charBuf, iSize, NULL, NULL );
	dest = charBuf;
}

#define ERRORMSG( s )	::MessageBox( NULL, s, L"ERROR.", MB_OK | MB_ICONERROR )
#define INFORMSG( s )	::MessageBox( NULL, s, L"Information.!", MB_OK | MB_ICONINFORMATION )

#include "zlib.h"

#include "KGCBufferManager.h"
#include "MassFile.h"

#include "X2KomFileViewer.h"
#include "X2KomFileViewerDlg.h"
#include "LuaCompileUtil.h"
#include "X2KomFileManager.h"
#include <afxdhtml.h>

//lua 사용을 위한 
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "lualibd.lib" )
#else
#pragma comment( lib, "lualib.lib" )
#endif
//#include <ImportKncLua.h>
#include <lua_tinker.h>
#include "KLuaManager.h"


extern lua_State* g_pLua;

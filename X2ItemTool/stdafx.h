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
#include <afxdisp.h>        // MFC 자동화 클래스

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT



#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::wstring;
using std::string;
using std::wstringstream;
using std::stringstream;



#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "luaLibD.lib" )
#else
#pragma comment( lib, "luaLib.lib" )
#endif


#include "../luaLib/KLuaManager.h"
#include "../luaLib/KLuaBinder.h"

inline void TableBind( KLuaManager* pLuaManager, KLuabinder* pLuaBinder )
{
	bool	retVal = true;
	int		index = 1;
	string	buffer;

	retVal = pLuaManager->GetValue(index,buffer);
	while( retVal == true )
	{
		pLuaBinder->DoString( buffer.c_str() );
		retVal = pLuaManager->GetValue(index,buffer);
		index++;
	}
}











#include "zlib/zlib.h"
#pragma comment( lib, "zlib/zlibstat.lib" )


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif    


typedef __int64 UidType;




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

inline void ConvertCharToWCHAR( wstring& dest, const char* pSrc )
{
	KTDXPROFILE();

//{{ robobeg : 2008-01-08
	//if ( pSrc == NULL )
	//	return;

	//int iSize = strlen( pSrc );
	//if ( iSize <= 0 )
	//	return;

	//WCHAR* wcharBuf = (WCHAR*) alloca( (iSize + 1) * sizeof( WCHAR ) );
	//ZeroMemory( wcharBuf, (iSize + 1) * sizeof( WCHAR ) );

	//MultiByteToWideChar( CP_ACP, 0, pSrc, -1, wcharBuf, iSize );
	//dest = wcharBuf;

    dest.clear();
    if ( pSrc == NULL )
        return;

    int iSrcLength = strlen( pSrc );
    if ( iSrcLength <= 0 )
    {
        dest = L"";
        return;
    }//if

    int iDestSize = ::MultiByteToWideChar( CP_ACP, 0, pSrc, iSrcLength, NULL, 0 );
    if ( iDestSize <= 0 )
    {
        dest = L"";
        return;
    }//if

	WCHAR* wcharBuf = (WCHAR*) _alloca( iDestSize * sizeof( WCHAR ) );
    ASSERT( wcharBuf != NULL );
    int iNewSize = ::MultiByteToWideChar( CP_ACP, 0, pSrc, iSrcLength, wcharBuf, iDestSize );
    ASSERT( iNewSize == iDestSize );
    dest.assign( wcharBuf, iNewSize );
//}} robobeg : 2008-01-08
}
inline void ConvertWCHARToChar( string& dest, const WCHAR* pSrc )
{
	KTDXPROFILE();

//{{ robobeg : 2008-01-08
	//if ( pSrc == NULL )
	//	return;

	//int iSize = wcslen( pSrc ) * sizeof( WCHAR );
	//if ( iSize <= 0 )
	//	return;

	//char* charBuf = (char*) alloca( iSize + 1 );
	//ZeroMemory( charBuf, iSize + 1 );

	//WideCharToMultiByte( CP_ACP, 0, pSrc, -1, charBuf, iSize, NULL, NULL );
	//dest = charBuf;

    dest.clear();
    if ( pSrc == NULL )
        return;

    int iSrcLength = wcslen( pSrc );
    if ( iSrcLength <= 0 )
    {
        dest = "";
        return;
    }//if

    int iDestSize = ::WideCharToMultiByte( CP_ACP, 0, pSrc, iSrcLength, NULL, 0, NULL, NULL );
    if ( iDestSize <= 0 )
    {
        dest = "";
        return;
    }//if

    char* pszConv = (char*) _alloca( iDestSize * sizeof(char) );
    ASSERT( pszConv != NULL );
    int iNewSize = ::WideCharToMultiByte( CP_ACP, 0, pSrc, iSrcLength, pszConv, iDestSize, NULL, NULL );
    ASSERT( iNewSize == iDestSize );
    dest.assign( pszConv, iNewSize );
//}} robobeg : 2008-01-08
}



#include "KGCMassFileManager.h"

#include "x2itemtoolX2Item.h"
#include "x2itemtoolX2ItemManager.h"

#include "GridCtrl/GridCtrl.h"
#include "X2ItemToolCommon.h"

extern CX2ItemToolCommon* g_pItemToolCommon;


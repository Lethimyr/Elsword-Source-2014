// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__96D5C147_EDDE_4F62_9B9B_BA4C7EC96B1D__INCLUDED_)
#define AFX_STDAFX_H__96D5C147_EDDE_4F62_9B9B_BA4C7EC96B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4005 )

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define WINVER 0x0500       // Enable Win2K features
#define _WIN32_IE 0x0500    // Enable IE5 common control features

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
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

#include <string>
#include <map>
#include <vector>
#include <stdio.h>

inline void MakeUpperCase(std::string &str) 
{
	for( std::string::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (char)toupper(*i);
}

inline void MakeUpperCase(std::wstring &str) 
{
	for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (WCHAR)towupper(*i);
}

#define ERRORMSG( s )	::MessageBox( NULL, s, L"ERROR.", MB_OK | MB_ICONERROR )
#define INFORMSG( s )	::MessageBox( NULL, s, L"Information.!", MB_OK | MB_ICONINFORMATION )

#include "zlib.h"

#include "KGCBufferManager.h"
#include "MassFile.h"

#include "X2MassFile.h"
#include "X2MassFileDlg.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__96D5C147_EDDE_4F62_9B9B_BA4C7EC96B1D__INCLUDED_)

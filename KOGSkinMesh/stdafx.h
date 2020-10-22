//--------------------------------------------------------------------------------------
// File: DxStdAfx.h
//
// Desc: Standard includes and precompiled headers for DXUT
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DXSDK_STDAFX_H
#define DXSDK_STDAFX_H

#ifndef STRICT
#define STRICT
#endif

// If app hasn't choosen, set to work with Windows 98, Windows Me, Windows 2000, Windows XP and beyond
#ifndef WINVER
#define WINVER         0x0410
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0500 
#endif

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

//#include <afxwin.h>
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <math.h>      
#include <limits.h>      
#include <stdio.h>
#include <multimon.h> 
#include <string.h>
#include <malloc.h>


#include <shlobj.h>

// CRT's memory leak detection
#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif


// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

// DirectSound includes
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

// strsafe.h deprecates old unsecure string functions.  If you 
// really do not want to it to (not recommended), then uncomment the next line 
//#define STRSAFE_NO_DEPRECATE

#ifndef STRSAFE_NO_DEPRECATE
#pragma deprecated("strncpy")
#pragma deprecated("wcsncpy")
#pragma deprecated("_tcsncpy")
#pragma deprecated("wcsncat")
#pragma deprecated("strncat")
#pragma deprecated("_tcsncat")
#endif

#include <strsafe.h>

#include "DXUT.h"
#include "DXUTmisc.h"
#include "DXUTenum.h"
#include "DXUTmesh.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"
#include "DXUTSound.h"



#if defined(_DEBUG)

// ASSERTwnx
//
#if !defined(ASSERT)
#define ASSERT(exp)         _ASSERTE(exp)
#endif // #if !defined(ASSERT)

#else

#if !defined(ASSERT)
#define ASSERT(exp)         (exp)
#endif // #if !defined(ASSERT)

#endif

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif
#endif

#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))

#ifndef SAFE_CALL
#define SAFE_CALL(p)			if(p) p
#endif    
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p)        { if(p) { ::free(p); (p)=NULL; } }
#endif // #ifndef SAFE_FREE

#ifndef SAFE_REALLOC
#define SAFE_REALLOC(p,s)   \
{   if((s)==0){ SAFE_FREE(p); }   \
else if (!(p))    { (p) = ::malloc(s); ASSERT(p); } \
else {  (p) = ::realloc((p),(s)); ASSERT(p); } }
#endif // #ifndef SAFE_REALLOC

#ifndef SAFE_TYPEDREALLOC
#define SAFE_TYPEDREALLOC(type,p,s)   \
{   if((s)==0){ SAFE_FREE(p); }   \
else if (!(p))    { (p) = (type*) ::malloc((s) * sizeof(type)); ASSERT(p); } \
else {  (p) = (type*) ::realloc((p), (s) * sizeof(type)); ASSERT(p); } }
#endif // #ifndef SAFE_TYPEDREALLOC

#ifndef SAFE_CLOSE
#define SAFE_CLOSE(p)			{ if(p) { g_pKTDXApp->GetDeviceManager()->CloseDevice( (p)->GetDeviceID() );     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_DIALOG
#define SAFE_DELETE_DIALOG(p)	{ DialogLog("SAFE_DELETE_DIALOG"); if(p) { g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg( (p) );		 (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_XMESH_PLAYER_INSTANCE
#define SAFE_DELETE_XMESH_PLAYER_INSTANCE(xmesh,p)	{ if(p) { (xmesh)->DestroyInstance( (p) );					 (p)=NULL; } }
#endif
#ifndef CREATE_VECTOR3
#define CREATE_VECTOR3(v,a,b,c)	{ D3DXVECTOR3 (v); (v).x = (a); (v).y = (b); (v).z = (c); }
#endif  
#ifndef INIT_VECTOR3
#define INIT_VECTOR3(v,a,b,c)	{ (v).x = (a); (v).y = (b); (v).z = (c); }
#endif  
#ifndef CREATE_VECTOR2
#define CREATE_VECTOR2(v,a,b)	{ D3DXVECTOR2 (v); (v).x = (a); (v).y = (b); }
#endif  
#ifndef INIT_VECTOR2
#define INIT_VECTOR2(v,a,b)		{ (v).x = (a); (v).y = (b); }
#endif
#ifndef INIT_COLOR
#define INIT_COLOR(v,red,green,blue,alpha)		{ (v).r = (red); (v).g = (green); (v).b = (blue); (v).a = (alpha); }
#endif

#define SETVECTOR4(vector,_x,_y,_z,_w) {vector.x = _x; vector.y = _y; vector.z = _z; vector.w = _w;}
#define SETVECTOR3(vector,_x,_y,_z) {vector.x = _x; vector.y = _y; vector.z = _z;}
#define SETVECTOR2(vector,_x,_y) {vector.x = _x; vector.y = _y;}

struct KTDGSphere
{
	D3DXVECTOR3	center;
	float		fRadius;
};

//{{ robobeg : 2008-01-08
// 아래 헤더파일들은 KTDXLIB 내의 파일들이 제대로 컴파일되도록 하기 위해
// KTDX.h에서 필요한 부분들을 가져온 것이다. 만약 KTDX.h 수정된 뒤
// KOGSkinMesh 컴파일시 오류가 발생하면 추가로 필요한 헤더 파일이나 정의들을 가져와야 한다.
// ( 코드 공유의 비애 -- )
//}} robobeg : 2008-01-08

using namespace std;

#include <io.h> 
#include <vector>
using std::vector;
#include <stack>
using std::stack;
#include <queue>
using std::queue;
#include <list>
using std::list;
#include <map>
using std::map;
#include <string>
using std::wstring;
using std::string;
using std::wstringstream;
using std::stringstream;
#include <fstream>
#include <math.h>
#include <iostream>
using std::wcout;
using std::cout;
#include <set>
using std::set;
#include <sys/stat.h>

#define XOR_KEY0 0xc6f8aa02
#define XOR_KEY1 0x2647abdc
#define XOR_KEY2 0x9800bbef

#define KTDXPROFILE()
#define ErrorLogMsg( errorEnum, errorMsg )
#define ErrorLog( errorEnum )
#define KLOG(filename)

#include "..\KTDXLIB\KTDXCommonFunc.h"
#include "..\KTDXLIB\KTDXDevice.h"

#ifndef _KSMTOOL
#include    "..\KTDXLIB\KTDXCollision.h"
#endif // #ifndef _KSMTOOL

#include "..\KTDXLIB\KFileSerializer.h"
#include "..\KTDXLIB\KTDXDeviceXSkinMesh.h"
#include "..\KTDXLIB\KTDXAnimationController.h"
#include "..\KTDXLIB\KTDXDeviceXMesh.h"



#endif // !defined(DXSDK_STDAFX_H)

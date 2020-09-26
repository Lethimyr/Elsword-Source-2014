/** @file   KProfileMan.h
    @brief  interface of KProfileMan
    @author seojt@kogstudios.com
    @since  2006-4-17, 17:50:40
*/

#pragma once

#ifndef _KPROFILEMAN_H
#define _KPROFILEMAN_H

//#include "KJohnGen.h"
#include <d3dx9.h> 
#include "KStaticSingleton.h"
#include "jprofile/RHProfile.h"
//#include "KTimer.h"



///-----------------------------------------------------------------------------
/// @class KProfileMan
///
class KProfileMan : public KStaticSingleton<KProfileMan>
{
private:
	RHProfileIterator*  m_pitorRefRoot;
    RHProfileIterator*  m_pitorRef;
    int         m_iNumNodes;    ///< number of current nodes
    int         m_iCursor;      ///< current highlighted node
    float       m_fx;           ///< display left
    float       m_fy;           ///< display top
    float       m_fUpdateInterval;
    float       m_fUpdateTime;

    float       m_fThisTime;
    char        m_buf[1024];
    float       m_fBarScaleFactor;
    float       m_fMaxTime;

	bool		m_bEnableProfile;

	bool		m_bReservedClearRoot;

public:
    KProfileMan();
    ~KProfileMan();

private:
    void _Init();

public:
    void SafeDelete();
    void SetProfileIterator(RHProfileIterator* pitorRef_);
    void SetLeftTop(float fx, float fy) { m_fx = fx; m_fy = fy; }
    void Reset();

    void OnPreFrameMove(float fElapsedTime);
    BOOL OnFrameRender(IDirect3DDevice9* pd3dDevice, float fElapsedTime);
	void OnKeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown);

	// dmlee, 2007/06/12
	void OnCreateDevice( IDirect3DDevice9* pd3dDevice );
	void OnResetDevice( IDirect3DDevice9* pd3dDevice );
	void OnLostDevice();
	void OnDestroyDevice();

};//class KProfileMan

#endif // _KPROFILEMAN_H


/** @example    KProfileMan
    @code

  1) 필요한 파일들:

    jprofile/KBlockProfile.cpp
    jprofile/KBlockProfile.h
    jprofile/KProfile.cpp
    jprofile/KProfile.h
    jprofile/RHProfile.cpp
    jprofile/RHProfile.h

    KJohnGen.h
    KJohnStrUtil.cpp
    KJohnStrUtil.h
    KMessageConsole.cpp
    KMessageConsole.h
    KProfileMan.cpp
    KProfileMan.h
    KStaticSingleton.h
    KTimer.cpp
    KTimer.h

  2) 프로젝트 설정: Project -> Properties -> C/C++ -> Preprocessor에 아래의 문자열을 설정

    PROJECT_PCH=dxstdafx.h

    예) WIN32;_DEBUG;_WINDOWS;PROFILE;PROJECT_PCH=dxstdafx.h

  3) include:

    #include "KProfileMan.h"

  4) 전역 변수

    RHProfileIterator*  g_pitor = NULL;

  5) OnCreateDevice():

    g_pitor = RHProfileManager::Get_Iterator();
    ASSERT( g_pitor );
    KProfileMan::GetSingleton().SetProfileIterator( g_pitor );

  6) OnFrameMove():

    // 반드시 OnFrameMove()의 제일 처음에 위치해야 한다.
    // 이 함수부터 OnFrameRender()사이에만 profile이 가능하다.
    //
    KProfileMan::GetSingleton().OnPreFrameMove( fElapsedTime );

  7) OnFrameRender()의 EndScene()바로 전에:

    KProfileMan::GetSingleton().OnFrameRender( pd3dDevice, fElapsedTime );


  8) KeyboardProc():

    KProfileMan::GetSingleton().OnKeyboardProc( nChar, bKeyDown, bAltDown );

  9) OnDestroyDevice():

    ASSERT( g_pitor );
    RHProfileManager::Release_Iterator( g_pitor );
    g_pitor = NULL;
    KProfileMan::GetSingleton().SetProfileIterator( NULL );

  10) 프로파일 추가:

    block의 처음에 PROFILE()매크로를 추가하면, 함수이름의 프로파일 노드가 만들어 짐
    이름을 정하고 싶으면 PROFILE2("이름") 형식의 매크로를 사용하면 됨.

    @endcode
*/

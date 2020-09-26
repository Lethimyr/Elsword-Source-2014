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

//////////////////////////////////////////////////////////////////////////
//## KTDX Engine begin
//## Desc : 툴에서 사용하게 될 엔진 라이브러리 추가.

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4311)

//#include "KTDX.h"
#include "X2Main.h"

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4311)

//## KTDX Engine End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//## User source begin
//## Desc : 사용자 코드 영역.. 
#pragma NOTE( "Inhouse3.h 에 있는 #define EVENT_SCENE | EVENT_SCENE_TOOL 이 주석해제 되어 있어야 함" )

#define ERRORMSG(str)	MessageBox( NULL, str, L"error", MB_ICONERROR | MB_OK);

#include "..\CommonObject.hpp"
#include "..\EventScriptParsing.h"
#include "..\RenderFrm.h"
#include "..\DlgProc.h"

extern CRenderFrm * g_pRenderFrm;

inline CRenderFrm * GetRenderFrm(void)
{
	if( g_pRenderFrm == NULL )
	{
		//assert(g_pRenderFrm == NULL);
		g_pRenderFrm = new CRenderFrm();
	}

	return g_pRenderFrm;
}

#define MAX_SCRIPT_BUFFER 5048
//## User source end
//////////////////////////////////////////////////////////////////////////

extern CDXUTDialogResourceManager g_DialogResourceManager;

#endif // !defined(DXSDK_STDAFX_H)

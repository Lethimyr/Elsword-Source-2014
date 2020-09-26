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

#include "KTDX.h"

#pragma warning(disable:4996)
#pragma warning(disable:4267)

//## KTDX Engine End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//## User source begin
//## Desc : 사용자 코드 영역.. 

#include "../KTDGScenarioObject.hpp"
#include "../X2ScenarioDlgProc.h"
#include "../KTDGScenarioManager.h"
#include "../X2ScenarioToolStage.h"

extern CX2ScenarioToolStage* g_pScenarioToolStage;

inline CX2ScenarioToolStage* GetScenarioTool(void)
{
	if(g_pScenarioToolStage == NULL)
	{
		assert(g_pScenarioToolStage == NULL);
	}
	return g_pScenarioToolStage;
};

//## User source end
//////////////////////////////////////////////////////////////////////////

extern CDXUTDialogResourceManager g_DialogResourceManager;

#endif // !defined(DXSDK_STDAFX_H)

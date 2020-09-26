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

#define _HAS_ITERATOR_DEBUGGING 0

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "KTDXLIBD.lib" )
#pragma comment( lib, "X2LibD.lib" )
#else
#pragma comment( lib, "KTDXLIB.lib" )
#pragma comment( lib, "X2Lib.lib" )
#endif

#include "X2Main.h"

#pragma warning(disable: 4390)

#include "../X2EffectToolMain.h"
//#include "../X2DialogToolMain.h"
//#include "../X2TextureViewer.h"
//#include "../X2SkillTreeTool.h"

#include "../bandicap.h"




#endif // !defined(DXSDK_STDAFX_H)

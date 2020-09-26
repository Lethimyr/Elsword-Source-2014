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

#pragma warning(disable:4996)
#pragma warning(disable:4267)

//#include "KTDX.h"
#include "../X2Lib/X2Main.h"

#pragma warning(disable:4996)
#pragma warning(disable:4267)


#include "../X2DTEditBox.h"
#include "../X2DTListBox.h"
#include "../X2DTDialog.h"
#include "../X2DTWorld.h"
#include "../X2DTWorldManager.h"
#include "../X2EnumManager.h"
#include "../X2DTNpcScaleManager.h"
#include "../X2DTDungeonMapUI.h"
#include "../X2DungeonTool.h"

extern CDXUTDialogResourceManager g_DialogResourceManager;
extern CDXUTDialog				  g_HUD;


#endif // !defined(DXSDK_STDAFX_H)

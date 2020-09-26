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

#pragma warning(disable:4267)
#include "KTDX.h"

#define ERRORMSG(str)	MessageBox( NULL, str, L"error", MB_ICONERROR | MB_OK); \
						DestroyWindow( g_pKTDXApp->GetHWND() );
#define WARNINGMSG(str)	MessageBox( NULL, str, L"error", MB_ICONQUESTION | MB_OK);

#define WIDTH 1280
#define HEIGHT 960

struct IMPACT_DATA
{
	float	fMin;
	float	fMax;
	float	fAnimTime;

	IMPACT_DATA()
	{
		fMin		= 0.0f;
		fMax		= 0.0f;
		fAnimTime	= 0.0f;
	}
};

struct TEX_STAGE_DATA
{
	D3DXVECTOR2	vMin;
	D3DXVECTOR2	vMax;
	float		fAnimTime;
	float		fNowAnimTime;

	TEX_STAGE_DATA()
	{
		vMin.x = 0.0f;
		vMin.y = 0.0f;
		vMax.x = 0.0f;
		vMax.y = 0.0f;
		fAnimTime = 0.0f;
		fNowAnimTime = 0.0f;
	}
};

struct SWeaponInfo
{
	wstring			m_BoneName;		/// 붙을 뼈대 이름
	D3DXMATRIX*		m_pMatrix;		/// 행렬
	D3DXVECTOR3		m_vRotate;		/// 무기 회전
	CKTDGXSkinAnim*	m_pXSkinAnim;	/// 스킨 메쉬

	SWeaponInfo()
	{
		m_BoneName.clear();
		m_pMatrix = NULL;
		INIT_VECTOR3( m_vRotate, 0.f, 0.f, 0.f );
		m_pXSkinAnim = NULL;
	}
};

struct SAccessoryInfo
{
	wstring				m_BoneName;		/// 붙을 뼈대 이름
	D3DXMATRIX*			m_pMatrix;		/// 행렬
	D3DXVECTOR3			m_vMove;		/// 이동
	D3DXVECTOR3			m_vRotate;		/// 회전
	D3DXVECTOR3			m_vScale;		/// 크기
	CKTDXDeviceXMesh*	m_pXMesh;		/// 메쉬

	SAccessoryInfo()
	{
		m_BoneName.clear();
		m_pMatrix = NULL;
		INIT_VECTOR3( m_vMove, 0.f, 0.f, 0.f );
		INIT_VECTOR3( m_vRotate, 0.f, 0.f, 0.f );
		INIT_VECTOR3( m_vScale, 1.f, 1.f, 1.f );
		m_pXMesh = NULL;
	}
};

#pragma region CHARACTER_ID
enum CHARACTER_ID
{
	CI_NONE = -1,
	CI_ELSWORD = 0, 
		CI_SWORD_KNIGHT,	CI_MAGIC_KNIGHT,	CI_SHEATH_KNIGHT,
		CI_LORD_KNIGHT,		CI_RUNE_SLAYER,		CI_INFINITE_SWORD,
	CI_AISHA,
		CI_HIGH_MAGICIAN,		CI_DARK_MAGICIAN,	CI_BATTLE_MAGICIAN,
		CI_ELEMENTAL_MASTER,	CI_VOID_PRINCESS,	CI_DIMENSION_WITCH,
	CI_LENA,
		CI_COMBAT_RANGER,	CI_SNIPING_RANGER,	CI_TRAPPING_RANGER,
		CI_WIND_SNIKER,		CI_GRAND_ARCHER,	CI_NIGHT_WATCHER,
	CI_RAVEN,
		CI_SWORD_TAKER,		CI_OVER_TAKER,		CI_WEAPON_TAKER,
		CI_BLADE_MASTER,	CI_RECKRESS_FIST,	CI_VETERAN_COMMANDER,
	CI_EVE,
		CI_EXOTIC_GEAR,		CI_ARCHTECTURE,		CI_ELECTRA,
		CI_CODE_NEMESIS,	CI_CODE_EMPRESS,	CI_BATTLE_SERAPH,
	CI_CHUNG,
		CI_FURY_GUARDIAN,	CI_SHOOTING_GUARDIAN,	CI_SHELLING_GUARDING,
		CI_IRON_PALADIN,	CI_DEADLY_CHASER,		CI_TATICAL_TROOPER,
	CI_ARA,
		CI_LITTLE_HSIEN,	CI_LITTLE_DEVIL,
		CI_SAKRA_DEVANAM,	CI_YAMA_RAJA, 
	CI_ELISIS,
		CI_SAVER_KNIGHT,	CI_PYRO_KINIGHT,
};
#pragma endregion 엘소드 캐릭터 ID

#pragma region MODIFY_BONE_STYLE
enum MODIFY_BONE_STYLE
{
	MBS_NONE	= 0,
	MBS_MOVE	= 1,
	MBS_SCALE	= 2,
};
#pragma endregion 뼈대 변환 방법

#include <algorithm>

typedef std::vector<CKTDXDeviceXSkinMesh::MultiAnimFrame*> VecMultiAnimFrame;
typedef std::map<CHARACTER_ID, wstring> MapCharacterID;

// x2viewer
#include "../X2ViewerObject.h"
#include "../X2ViewerGrid.h"
#include "../X2ViewerCamera.h"
#include "../X2ViewerMesh.h"
#include "../X2ViewerSkinMesh.h"
#include "../X2ViewerParticleEditor.h"
#include "../X2ViewerParticle.h"
#include "../X2ViewerParam.h"
#include "../X2ViewerFileOS.h"
#include "../X2ViewerUI.h"
#include "../X2ViewerWorldMesh.h"
#include "../X2ViewerMain.h"

extern CDXUTDialogResourceManager g_DialogResourceManager;

extern ID3DXFont*              g_pFont;
extern ID3DXSprite*            g_pTextSprite;

#endif // !defined(DXSDK_STDAFX_H)

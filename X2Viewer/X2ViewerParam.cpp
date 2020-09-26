/**	@file	: X2ViewerParam.cpp
	@desc	: 렌더링 파라미터
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#include "dxstdafx.h"
#include ".\x2viewerparam.h"

#define ui( s )	CX2ViewerUI::##s

#pragma region CX2ViewerParam
CX2ViewerParam::CX2ViewerParam(void) : 
m_pRenderParam( NULL )
{
	Reset();

	//RENDER_TYPE
	m_mapRenderType.insert( std::make_pair( L"REAL_COLOR",				CKTDGXRenderer::RT_REAL_COLOR ) );
	m_mapRenderType.insert( std::make_pair( L"CARTOON_BLACK_EDGE",		CKTDGXRenderer::RT_CARTOON_BLACK_EDGE ) );
	m_mapRenderType.insert( std::make_pair( L"CARTOON_COLOR_EDGE",		CKTDGXRenderer::RT_CARTOON_COLOR_EDGE ) );
	m_mapRenderType.insert( std::make_pair( L"CARTOON",					CKTDGXRenderer::RT_CARTOON ) );

	m_mapRenderType.insert( std::make_pair( L"BLACK_EDGE",				CKTDGXRenderer::RT_BLACK_EDGE ) );
	m_mapRenderType.insert( std::make_pair( L"COLOR_EDGE",				CKTDGXRenderer::RT_COLOR_EDGE ) );

	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_WORLD_X",		CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_X ) );
	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_WORLD_Y",		CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_Y ) );
	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_WORLD_Z",		CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_Z ) );
	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_WORLD_POINT",	CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_POINT ) );
	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_LOCAL_X",		CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_X ) );
	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_LOCAL_Y",		CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Y ) );
	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_LOCAL_Z",		CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Z ) );
	m_mapRenderType.insert( std::make_pair( L"LIGHT_FLOW_LOCAL_POINT",	CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_POINT ) );

	//CARTOON TEXTURE TYPE
	m_mapCartoonTexType.insert( std::make_pair( L"NORMAL",		CKTDGXRenderer::CTT_NORMAL ) );
	m_mapCartoonTexType.insert( std::make_pair( L"RED",			CKTDGXRenderer::CTT_RED ) );
	m_mapCartoonTexType.insert( std::make_pair( L"GREEN",		CKTDGXRenderer::CTT_GREEN ) );
	m_mapCartoonTexType.insert( std::make_pair( L"VIOLET",		CKTDGXRenderer::CTT_VIOLET ) );
	m_mapCartoonTexType.insert( std::make_pair( L"COUNTER",		CKTDGXRenderer::CTT_NORMAL_COUNTER ) );
	m_mapCartoonTexType.insert( std::make_pair( L"DAMAGE",		CKTDGXRenderer::CTT_DAMAGE ) );
	m_mapCartoonTexType.insert( std::make_pair( L"INVINCIBLE",	CKTDGXRenderer::CTT_INVINCIBLE ) );
	m_mapCartoonTexType.insert( std::make_pair( L"PETRIFIED",	CKTDGXRenderer::CTT_PETRIFIED ) );
	m_mapCartoonTexType.insert( std::make_pair( L"SKYBLUE",		CKTDGXRenderer::CTT_SKYBLUE ) );
	m_mapCartoonTexType.insert( std::make_pair( L"SCARLET",		CKTDGXRenderer::CTT_SCARLET ) );

	//D3DCULL ENUM
	m_mapD3DCull.insert( std::make_pair( L"NONE",			D3DCULL_NONE ) );
	m_mapD3DCull.insert( std::make_pair( L"CW",				D3DCULL_CW ) );
	m_mapD3DCull.insert( std::make_pair( L"CCW",			D3DCULL_CCW ) );
	m_mapD3DCull.insert( std::make_pair( L"FORCE_DWORD",	D3DCULL_FORCE_DWORD ) );

	//D3DBLEND ENUM
	m_mapD3DBlend.insert( std::make_pair( L"ZERO",				D3DBLEND_ZERO ) );
	m_mapD3DBlend.insert( std::make_pair( L"ONE",				D3DBLEND_ONE ) );
	m_mapD3DBlend.insert( std::make_pair( L"SRCCOLOR",			D3DBLEND_SRCCOLOR ) );
	m_mapD3DBlend.insert( std::make_pair( L"INVSRCCOLOR",		D3DBLEND_INVSRCCOLOR ) );
	m_mapD3DBlend.insert( std::make_pair( L"SRCALPHA",			D3DBLEND_SRCALPHA ) );
	m_mapD3DBlend.insert( std::make_pair( L"INVSRCALPHA",		D3DBLEND_INVSRCALPHA ) );
	m_mapD3DBlend.insert( std::make_pair( L"DESTALPHA",			D3DBLEND_DESTALPHA ) );
	m_mapD3DBlend.insert( std::make_pair( L"INVDESTALPHA",		D3DBLEND_INVDESTALPHA ) );
	m_mapD3DBlend.insert( std::make_pair( L"DESTCOLOR",			D3DBLEND_DESTCOLOR ) );
	m_mapD3DBlend.insert( std::make_pair( L"INVDESTCOLOR",		D3DBLEND_INVDESTCOLOR ) );
	m_mapD3DBlend.insert( std::make_pair( L"SRCALPHASAT",		D3DBLEND_SRCALPHASAT ) );
	m_mapD3DBlend.insert( std::make_pair( L"BOTHSRCALPHA",		D3DBLEND_BOTHSRCALPHA ) );
	m_mapD3DBlend.insert( std::make_pair( L"BOTHINVSRCALPHA",	D3DBLEND_BOTHINVSRCALPHA ) );
	m_mapD3DBlend.insert( std::make_pair( L"BLENDFACTOR",		D3DBLEND_BLENDFACTOR ) );
	m_mapD3DBlend.insert( std::make_pair( L"INVBLENDFACTOR",	D3DBLEND_INVBLENDFACTOR ) );
	m_mapD3DBlend.insert( std::make_pair( L"FORCE_DWORD",		D3DBLEND_FORCE_DWORD ) );

	//True / False
	//m_mapTrueFalse.insert( std::make_pair( L"true",		(bool)true ) );
	//m_mapTrueFalse.insert( std::make_pair( L"false",	(bool)false ) );
}
#pragma endregion 생성자

#pragma region ~CX2ViewerParam
CX2ViewerParam::~CX2ViewerParam(void)
{
	m_mapRenderType.clear();
	m_mapCartoonTexType.clear();
	m_mapD3DCull.clear();
	m_mapD3DBlend.clear();
	//m_mapTrueFalse.clear();

	m_bAlpha = false;
	m_bZEnable = m_bZWritable = true;

	SAFE_DELETE( m_pRenderParam );
}
#pragma endregion 소멸자

#pragma region GetRenderParam
void CX2ViewerParam::GetRenderParam( CKTDGXRenderer::RenderParam* renderParam, IMPACT_DATA* impactData,
									 std::vector<TEX_STAGE_DATA>& vecTexStageData )
{
	D3DXVECTOR3	lightPos = renderParam->lightPos;

	*renderParam = *m_pRenderParam;
	renderParam->lightPos = lightPos;
	*impactData = m_ImpactData;

	for( int i = 0; i < (int)vecTexStageData.size(); ++i )
	{
		vecTexStageData[i] = m_vecTexStageData[i];
	}
}
#pragma endregion 파라미터 값 넘김

#pragma region SetParamDlg
void CX2ViewerParam::SetParamDlg( CDXUTDialog* pDlg )
{
	RenderTypeMap::iterator		rtItr;
	CartoonTexTypeMap::iterator	cttItr;
	D3DCullMap::iterator		cullItr;
	D3DBlendMap::iterator		blendItr;
	WCHAR	wsz[128] = L"";

	// RenderParam : renderType
	pDlg->GetComboBox( ui(UI_COMBO_RENDERTYPE) )->RemoveAllItems();
	for( rtItr = m_mapRenderType.begin(); rtItr != m_mapRenderType.end(); ++rtItr )
	{
		pDlg->GetComboBox( ui(UI_COMBO_RENDERTYPE) )->AddItem( rtItr->first.c_str(), NULL );

		if( rtItr->second == m_pRenderParam->renderType )
		{
			pDlg->GetComboBox( ui(UI_COMBO_RENDERTYPE) )->SetSelectedByText( rtItr->first.c_str() );
		}
	}

	// RenderParam : cartoonTexType
	pDlg->GetComboBox( ui(UI_COMBO_CARTOON_TEX_TYPE) )->RemoveAllItems();
	for( cttItr = m_mapCartoonTexType.begin(); cttItr != m_mapCartoonTexType.end(); ++cttItr )
	{
		pDlg->GetComboBox( ui(UI_COMBO_CARTOON_TEX_TYPE) )->AddItem( cttItr->first.c_str(), NULL );

		if( cttItr->second == m_pRenderParam->cartoonTexType )
		{
			pDlg->GetComboBox( ui(UI_COMBO_CARTOON_TEX_TYPE) )->SetSelectedByText( cttItr->first.c_str() );
		}
	}

	// RenderParam : cullMode
	pDlg->GetComboBox( ui(UI_COMBO_CULLMODE) )->RemoveAllItems();
	for( cullItr = m_mapD3DCull.begin(); cullItr != m_mapD3DCull.end(); ++cullItr )
	{
		pDlg->GetComboBox( ui(UI_COMBO_CULLMODE) )->AddItem( cullItr->first.c_str(), NULL );

		if( cullItr->second == m_pRenderParam->cullMode )
		{
			pDlg->GetComboBox( ui(UI_COMBO_CULLMODE) )->SetSelectedByText( cullItr->first.c_str() );
		}
	}

	// RenderParam : srcBlend
	pDlg->GetComboBox( ui(UI_COMBO_SRCBLEND) )->RemoveAllItems();
	for( blendItr = m_mapD3DBlend.begin(); blendItr != m_mapD3DBlend.end(); ++blendItr )
	{
		pDlg->GetComboBox( ui(UI_COMBO_SRCBLEND) )->AddItem( blendItr->first.c_str(), NULL );

		if( blendItr->second == m_pRenderParam->srcBlend )
		{
			pDlg->GetComboBox( ui(UI_COMBO_SRCBLEND) )->SetSelectedByText( blendItr->first.c_str() );
		}
	}

	// RenderParam : destBlend
	pDlg->GetComboBox( ui(UI_COMBO_DESTBLEND) )->RemoveAllItems();
	for( blendItr = m_mapD3DBlend.begin(); blendItr != m_mapD3DBlend.end(); ++blendItr )
	{
		pDlg->GetComboBox( ui(UI_COMBO_DESTBLEND) )->AddItem( blendItr->first.c_str(), NULL );

		if( blendItr->second == m_pRenderParam->destBlend )
		{
			pDlg->GetComboBox( ui(UI_COMBO_DESTBLEND) )->SetSelectedByText( blendItr->first.c_str() );
		}
	}

	// RenderParam : fOutLineWide
	swprintf( wsz, L"%.2f", m_pRenderParam->fOutLineWide );
	pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_WIDE) )->SetText( wsz );

	// RenderParam : outLineColor
	swprintf( wsz, L"%d", (int)(m_pRenderParam->outLineColor.a*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_A) )->SetText( wsz );
	swprintf( wsz, L"%d", (int)(m_pRenderParam->outLineColor.r*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_R) )->SetText( wsz );
	swprintf( wsz, L"%d", (int)(m_pRenderParam->outLineColor.g*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_G) )->SetText( wsz );
	swprintf( wsz, L"%d", (int)(m_pRenderParam->outLineColor.b*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_B) )->SetText( wsz );

	// RenderParam : color
	swprintf( wsz, L"%d", (int)(m_pRenderParam->color.a*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_COLOR_A) )->SetText( wsz );
	swprintf( wsz, L"%d", (int)(m_pRenderParam->color.r*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_COLOR_R) )->SetText( wsz );
	swprintf( wsz, L"%d", (int)(m_pRenderParam->color.g*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_COLOR_G) )->SetText( wsz );
	swprintf( wsz, L"%d", (int)(m_pRenderParam->color.b*255.0f) );
	pDlg->GetEditBox( ui(UI_EDIT_COLOR_B) )->SetText( wsz );

	// RenderParam : fLightFlowWide
	swprintf( wsz, L"%.2f", m_pRenderParam->fLightFlowWide );
	pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_WIDE) )->SetText( wsz );

	// RenderParam : fLightFlowImpact
	swprintf( wsz, L"%.2f", m_ImpactData.fMin );
	pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_IMPACT_MIN) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_ImpactData.fMax );
	pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_IMPACT_MAX) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_ImpactData.fAnimTime );
	pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_IMPACT_ANIMTIME) )->SetText( wsz );

	// RenderParam : lightFlowPoint
	swprintf( wsz, L"%.2f", m_pRenderParam->lightFlowPoint.x );
	pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_POINT_X) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_pRenderParam->lightFlowPoint.y );
	pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_POINT_Y) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_pRenderParam->lightFlowPoint.z );
	pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_POINT_Z) )->SetText( wsz );

	// RenderParam : texOffsetStage0
	swprintf( wsz, L"%.2f", m_vecTexStageData[0].vMin.x );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MIN_X) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[0].vMin.y );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MIN_Y) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[0].vMax.x );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MAX_X) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[0].vMax.y );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MAX_Y) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[0].fAnimTime );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_ANIMTIME) )->SetText( wsz );

	// RenderParam : texOffsetStage1
	swprintf( wsz, L"%.2f", m_vecTexStageData[1].vMin.x );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MIN_X) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[1].vMin.y );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MIN_Y) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[1].vMax.x );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MAX_X) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[1].vMax.y );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MAX_Y) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[1].fAnimTime );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_ANIMTIME) )->SetText( wsz );

	// RenderParam : texOffsetStage2
	swprintf( wsz, L"%.2f", m_vecTexStageData[2].vMin.x );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MIN_X) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[2].vMin.y );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MIN_Y) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[2].vMax.x );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MAX_X) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[2].vMax.y );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MAX_Y) )->SetText( wsz );
	swprintf( wsz, L"%.2f", m_vecTexStageData[2].fAnimTime );
	pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_ANIMTIME) )->SetText( wsz );

	// RenderParam : bAlphaBlend
	/*std::map<std::wstring, bool>::iterator rfItr;
	pDlg->GetComboBox( ui(UI_COMBO_ALPHA_BLEND) )->RemoveAllItems();
	for( rfItr = m_mapTrueFalse.begin(); rfItr != m_mapTrueFalse.end(); ++rfItr )
	{
		pDlg->GetComboBox( ui(UI_COMBO_ALPHA_BLEND) )->AddItem( rfItr->first.c_str(), NULL );

		if( rfItr->second == m_pRenderParam->bAlphaBlend )
		{
			pDlg->GetComboBox( ui(UI_COMBO_ALPHA_BLEND) )->SetSelectedByText( rfItr->first.c_str() );
		}
	}

	// RenderParam : bZEnable
	pDlg->GetComboBox( ui(UI_COMBO_ZENABLE) )->RemoveAllItems();
	for( rfItr = m_mapTrueFalse.begin(); rfItr != m_mapTrueFalse.end(); ++ rfItr )
	{
		pDlg->GetComboBox( ui(UI_COMBO_ZENABLE) )->AddItem( rfItr->first.c_str(), NULL );

		if( rfItr->second == m_pRenderParam->bZEnable )
		{
			pDlg->GetComboBox( ui(UI_COMBO_ZENABLE) )->SetSelectedByText( rfItr->first.c_str() );
		}
	}*/

	// Boolean
	pDlg->GetCheckBox( ui( UI_CHECK_ALPHA_BLEND ) )->SetChecked( m_pRenderParam->bAlphaBlend );
	pDlg->GetCheckBox( ui( UI_CHECK_ZENABLE ) )->SetChecked( m_pRenderParam->bZEnable );
	pDlg->GetCheckBox( ui( UI_CHECK_ZWRITABLE ) )->SetChecked( m_pRenderParam->bZWriteEnable );
}
#pragma endregion 렌더 파라미터 다이얼로그 설정

#pragma region GetParamDlg
void CX2ViewerParam::GetParamDlg( CDXUTDialog* pDlg )
{
	std::map<std::wstring, CKTDGXRenderer::RENDER_TYPE>::iterator		rtItr;
	std::map<std::wstring, CKTDGXRenderer::CARTOON_TEX_TYPE>::iterator	cttItr;
	std::map<std::wstring, D3DCULL>::iterator	cullItr;
	std::map<std::wstring, D3DBLEND>::iterator	blendItr;
	std::map<std::wstring, bool>::iterator		rfItr;
	DXUTComboBoxItem* pItem = NULL;

	// RenderParam : renderType
	pItem = pDlg->GetComboBox( ui(UI_COMBO_RENDERTYPE) )->GetSelectedItem();
	if( pItem == NULL )
	{
		WARNINGMSG( L"RenderParam:renderType 읽기오류" );
		return;
	}
	rtItr = m_mapRenderType.find( pItem->strText );
	if( rtItr == m_mapRenderType.end() )
	{
		WARNINGMSG( L"RenderParam:renderType 읽기오류" );
		return;
	}
	m_pRenderParam->renderType = rtItr->second;
	pItem = NULL;

	// RenderParam : cartoonTexType
	pItem = pDlg->GetComboBox( ui(UI_COMBO_CARTOON_TEX_TYPE) )->GetSelectedItem();
	if( pItem == NULL )
	{
		WARNINGMSG( L"RenderParam:cartoonTexType 읽기오류" );
		return;
	}
	cttItr = m_mapCartoonTexType.find( pItem->strText );
	if( cttItr == m_mapCartoonTexType.end() )
	{
		WARNINGMSG( L"RenderParam:cartoonTexType 읽기오류" );
		return;
	}
	m_pRenderParam->cartoonTexType = cttItr->second;
	pItem = NULL;

	// RenderParam : cullMode
	pItem = pDlg->GetComboBox( ui(UI_COMBO_CULLMODE) )->GetSelectedItem();
	if( pItem == NULL )
	{
		WARNINGMSG( L"RenderParam:cullMode 읽기오류" );
		return;
	}
	cullItr = m_mapD3DCull.find( pItem->strText );
	if( cullItr == m_mapD3DCull.end() )
	{
		WARNINGMSG( L"RenderParam:cullMode 읽기오류" );
		return;
	}
	m_pRenderParam->cullMode = cullItr->second;
	pItem = NULL;

	// RenderParam : srcBlend
	pItem = pDlg->GetComboBox( ui(UI_COMBO_SRCBLEND) )->GetSelectedItem();
	if( pItem == NULL )
	{
		WARNINGMSG( L"RenderParam:srcBlend 읽기오류" );
		return;
	}
	blendItr = m_mapD3DBlend.find( pItem->strText );
	if( blendItr == m_mapD3DBlend.end() )
	{
		WARNINGMSG( L"RenderParam:srcBlend 읽기오류" );
		return;
	}
	m_pRenderParam->srcBlend = blendItr->second;
	pItem = NULL;

	// RenderParam : destBlend
	pItem = pDlg->GetComboBox( ui(UI_COMBO_DESTBLEND) )->GetSelectedItem();
	if( pItem == NULL )
	{
		WARNINGMSG( L"RenderParam:destBlend 읽기오류" );
		return;
	}
	blendItr = m_mapD3DBlend.find( pItem->strText );
	if( blendItr == m_mapD3DBlend.end() )
	{
		WARNINGMSG( L"RenderParam:destBlend 읽기오류" );
		return;
	}
	m_pRenderParam->destBlend = blendItr->second;
	pItem = NULL;

	m_pRenderParam->bAlphaBlend = pDlg->GetCheckBox( ui( UI_CHECK_ALPHA_BLEND ) )->GetChecked();
	m_pRenderParam->bZEnable = pDlg->GetCheckBox( ui( UI_CHECK_ZENABLE ) )->GetChecked();
	m_pRenderParam->bZWriteEnable = pDlg->GetCheckBox( ui( UI_CHECK_ZWRITABLE ) )->GetChecked();

	// RenderParam : bAlphaBlend
	/*pItem = pDlg->GetComboBox( ui(UI_COMBO_ALPHA_BLEND) )->GetSelectedItem();
	if( pItem == NULL )
	{
		WARNINGMSG( L"RenderParam:bAlphaBlend 읽기오류" );
		return;
	}
	rfItr = m_mapTrueFalse.find( pItem->strText );
	if( rfItr == m_mapTrueFalse.end() )
	{
		WARNINGMSG( L"RenderParam:bAlphaBlend 읽기오류" );
		return;
	}
	m_pRenderParam->bAlphaBlend = rfItr->second;
	pItem = NULL;*/

	// RenderParam : bZEnable
	/*pItem = pDlg->GetComboBox( ui(UI_COMBO_ZENABLE) )->GetSelectedItem();
	if( pItem == NULL )
	{
		WARNINGMSG( L"RenderParam:bZEnable 읽기오류" );
		return;
	}
	rfItr = m_mapTrueFalse.find( pItem->strText );
	if( rfItr == m_mapTrueFalse.end() )
	{
		WARNINGMSG( L"RenderParam:bZEnable 읽기오류" );
		return;
	}
	m_pRenderParam->bZEnable = rfItr->second;
	pItem = NULL;*/

	// RenderParam : fOutLineWide
	m_pRenderParam->fOutLineWide = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_WIDE) )->GetText() );

	// RenderParam : outLineColor
	D3DXCOLOR color;
	color.a = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_A) )->GetText() )/255.0f);
	color.r = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_R) )->GetText() )/255.0f);
	color.g = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_G) )->GetText() )/255.0f);
	color.b = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_OUTLINE_COLOR_B) )->GetText() )/255.0f);
	m_pRenderParam->outLineColor = color;

	// RenderParam : color
	color.a = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_COLOR_A) )->GetText() )/255.0f);
	color.r = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_COLOR_R) )->GetText() )/255.0f);
	color.g = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_COLOR_G) )->GetText() )/255.0f);
	color.b = (float)(_wtof( pDlg->GetEditBox( ui(UI_EDIT_COLOR_B) )->GetText() )/255.0f);
	m_pRenderParam->color = color;

	// RenderParam : fLightFlowWide
	m_pRenderParam->fLightFlowWide = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_WIDE) )->GetText() );

	// RenderParam : fLightFlowImpact
	m_ImpactData.fMin		= (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_IMPACT_MIN) )->GetText() );
	m_ImpactData.fMax		= (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_IMPACT_MAX) )->GetText() );
	m_ImpactData.fAnimTime	= (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_IMPACT_ANIMTIME) )->GetText() );

	D3DXVECTOR3 vec3;
	D3DXVECTOR2 vec2;
	// RenderParam : lightFlowPoint
	vec3.x = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_POINT_X) )->GetText() );
	vec3.y = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_POINT_Y) )->GetText() );
	vec3.z = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_LIGHTFLOW_POINT_Z) )->GetText() );
	m_pRenderParam->lightFlowPoint = vec3;

	// RenderParam : texOffsetStage0
	m_vecTexStageData[0].vMin.x = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MIN_X) )->GetText() );
	m_vecTexStageData[0].vMin.y = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MIN_Y) )->GetText() );
	m_vecTexStageData[0].vMax.x = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MAX_X) )->GetText() );
	m_vecTexStageData[0].vMax.y = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_MAX_Y) )->GetText() );
	m_vecTexStageData[0].fAnimTime = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE0_ANIMTIME) )->GetText() );

	// RenderParam : texOffsetStage1
	m_vecTexStageData[1].vMin.x = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MIN_X) )->GetText() );
	m_vecTexStageData[1].vMin.y = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MIN_Y) )->GetText() );
	m_vecTexStageData[1].vMax.x = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MAX_X) )->GetText() );
	m_vecTexStageData[1].vMax.y = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_MAX_Y) )->GetText() );

	m_vecTexStageData[1].fAnimTime = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE1_ANIMTIME) )->GetText() );

	// RenderParam : texOffsetStage1
	m_vecTexStageData[2].vMin.x = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MIN_X) )->GetText() );
	m_vecTexStageData[2].vMin.y = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MIN_Y) )->GetText() );
	m_vecTexStageData[2].vMax.x = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MAX_X) )->GetText() );
	m_vecTexStageData[2].vMax.y = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_MAX_Y) )->GetText() );
	m_vecTexStageData[2].fAnimTime = (float)_wtof( pDlg->GetEditBox( ui(UI_EDIT_TEXOFFSET_STAGE2_ANIMTIME) )->GetText() );
}
#pragma endregion 다이얼로그 값 얻어오기

#pragma region Reset
void CX2ViewerParam::Reset()
{
	SAFE_DELETE( m_pRenderParam );

	m_pRenderParam = new CKTDGXRenderer::RenderParam();

	//캐릭터 기본속성 랜더링으로 셋팅.
	m_pRenderParam->renderType	= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	m_pRenderParam->fOutLineWide	= 1.5f;

	m_vecTexStageData.clear();
	TEX_STAGE_DATA texStageData;
	m_vecTexStageData.push_back( texStageData );
	m_vecTexStageData.push_back( texStageData );
	m_vecTexStageData.push_back( texStageData );

	m_ImpactData.fMin	= 0.0f;
	m_ImpactData.fMax	= 0.0f;
	m_ImpactData.fAnimTime = 0.0f;
}
#pragma endregion 재설정

#pragma region SetEffect
void CX2ViewerParam::SetEffect()
{
	m_pRenderParam->renderType	= CKTDGXRenderer::RT_REAL_COLOR;
	m_pRenderParam->bAlphaBlend	= true;
	m_pRenderParam->destBlend	= D3DBLEND_DESTALPHA;
}
#pragma endregion EffectButton 기능 수행
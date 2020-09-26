#include "StdAfx.h"
#include ".\ktdguilistcontrol.h"

CKTDGUIListControl::CKTDGUIListControl()
{
	m_ItemStartPos.x	= 0.0f;
	m_ItemStartPos.y	= 0.0f;

	m_ItemWidth			= 0;
	m_ItemHeight		= 0;
	m_NowStartItem		= 0;

	m_Column			= 0;
	m_Row				= 0;

	m_pBGTex		= NULL;
	m_pBGPoint		= NULL;
	m_pItemBGTex	= NULL;
}

CKTDGUIListControl::~CKTDGUIListControl(void)
{
	SAFE_DELETE( m_pItemBGTex );
	SAFE_DELETE( m_pBGPoint );
	SAFE_DELETE( m_pBGTex );
}

HRESULT	CKTDGUIListControl::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_bShow == false )
		return S_OK;

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	list<CKTDGUIControl*>::iterator iter = m_ItemList.begin();
	for( iter = m_ItemList.begin(); iter != m_ItemList.end(); iter++ )
	{
		CKTDGUIControl* pControl = *iter;
		pControl->SetShow( false );
	}

	int nowViewItemIndex = m_NowStartItem;
	for( int i = 0; i < m_Column; i++ )
	{
		for( int j = 0; j < m_Row; j++ )
		{
			CKTDGUIControl* pControl = GetItem( nowViewItemIndex );
			if( pControl != NULL )
			{
				//아이템이 있으면 아이템을 위치로 옮긴다
				pControl->SetShow( true );
				pControl->SetOffsetPos_LUA( m_ItemStartPos.x + (i * m_ItemWidth), m_ItemStartPos.y + (j * m_ItemHeight) );
			}
			nowViewItemIndex++;
		}
	}

	return S_OK;
}

HRESULT	CKTDGUIListControl::OnFrameRender()
{
	if( m_bShow == false )
		return S_OK;

	if( m_pBGTex != NULL && m_pBGPoint != NULL )
	{
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], m_Vertex[CKTDGUIControl::VP_RIGHT_TOP],
						m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
						*m_pBGPoint);
		RenderVertex( m_Vertex, *m_pBGPoint );
	}

	if( m_pItemBGTex == NULL )
		return S_OK;

	int nowViewItemIndex = m_NowStartItem;
	for( int i = 0; i < m_Column; i++ )
	{
		for( int j = 0; j < m_Row; j++ )
		{
			CKTDGUIControl* pControl = GetItem( nowViewItemIndex );
			if( pControl == NULL )
			{
				//아이템이 없으면 배경을 그린다
				CKTDGUIControl::UIPointData	itemPoint;
				itemPoint.pUITextureData		= m_pItemBGTex;
				itemPoint.leftTopPoint.x		= m_ItemStartPos.x + (i * m_ItemWidth);
				itemPoint.leftTopPoint.y		= m_ItemStartPos.y + (j * m_ItemHeight);
				itemPoint.rightTopPoint.x		= m_ItemStartPos.x + (i * m_ItemWidth) + m_ItemWidth;
				itemPoint.rightTopPoint.y		= m_ItemStartPos.y + (j * m_ItemHeight);
				itemPoint.leftBottomPoint.x		= m_ItemStartPos.x + (i * m_ItemWidth);
				itemPoint.leftBottomPoint.y		= m_ItemStartPos.y + (j * m_ItemHeight) + m_ItemHeight;
				itemPoint.rightBottomPoint.x	= m_ItemStartPos.x + (i * m_ItemWidth) + m_ItemWidth;
				itemPoint.rightBottomPoint.y	= m_ItemStartPos.y + (j * m_ItemHeight) + m_ItemHeight;
				itemPoint.fChangeTime			= 0.0f;
				itemPoint.color					= 0xffffffff;

				UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP],
								m_Vertex[CKTDGUIControl::VP_RIGHT_TOP],
								m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM],
								m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
								itemPoint );

				RenderVertex( m_Vertex, itemPoint );
			}
			nowViewItemIndex++;
		}
	}

	return S_OK;
}

void CKTDGUIListControl::SetItemControlScript_( const char* pFileName )
{
	wstring WName;
	ConvertCharToWCHAR( WName, pFileName );
	SetItemControlScript( WName.c_str() );
}

void CKTDGUIListControl::SetItemBackgroundTex_( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pItemBGTex );
	wstring wstrFileName;
	ConvertCharToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertCharToWCHAR( wstrKeyName, key );

	m_pItemBGTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUIListControl::SetBackgroundTex_( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pBGTex );
	wstring wstrFileName;
	ConvertCharToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertCharToWCHAR( wstrKeyName, key );

	m_pBGTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pBGPoint != NULL )
		m_pBGPoint->pUITextureData = m_pBGTex;
}

void CKTDGUIListControl::SetBackgroundPoint_()
{
	SAFE_DELETE( m_pBGPoint );
	m_pBGPoint = SetPointData_();
	m_pBGPoint->pUITextureData = m_pBGTex;
}

CKTDGUIControl* CKTDGUIListControl::GetItem( int index )
{
	if( m_ItemList.empty() == true )
		return NULL;

	list<CKTDGUIControl*>::iterator iter = m_ItemList.begin();
	for( int i = 0; i < index; i++ )
	{
		iter++;
		if( iter == m_ItemList.end() )
			return NULL;
	}

	CKTDGUIControl* pControl = *iter;
	return pControl;
}










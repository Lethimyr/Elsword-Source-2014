#include "StdAfx.h"
#include ".\x2textmanager.h"

CX2TextManager::CX2TextManager(void)
{

}

CX2TextManager::~CX2TextManager(void)
{
	for ( int i = 0; i < (int)m_vecTextList.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecTextList[i];
		SAFE_DELETE( pText );

	}
	m_vecTextList.clear();
}

CX2TextManager::CX2Text* CX2TextManager::AddText( int fontIndex, D3DXVECTOR2 pos, const WCHAR* pTextMsg, 
												 D3DXCOLOR color, D3DXCOLOR outlineColor, 
												 DWORD dtType /* = DT_LEFT */, float fLineSpace  /*= 1.0f */ )
{
	CX2TextManager::CX2Text* pText = new CX2TextManager::CX2Text( fontIndex, pos, pTextMsg, color, outlineColor, dtType, fLineSpace );
	m_vecTextList.push_back( pText );

	return pText;
}
/*
CX2TextManager::CX2Text* CX2TextManager::CreateText( int fontIndex, D3DXVECTOR2 pos, const WCHAR* pTextMsg, 
												 D3DXCOLOR color, D3DXCOLOR outlineColor, 
												 DWORD dtType , float fLineSpace  )
{
	CX2TextManager::CX2Text* pText = new CX2TextManager::CX2Text( fontIndex, pos, pTextMsg, color, outlineColor, dtType, fLineSpace );

	return pText;
}


CX2TextManager::CX2Text* CX2TextManager::AddText( CX2Text* pText )
{
	if( NULL != pText )
		m_vecTextList.push_back( pText );

	return pText;
}
*/
CX2TextManager::CX2Text* CX2TextManager::AddText_LUA( int fontIndex, D3DXVECTOR2 pos, const char* pTextMsg, 
													D3DXCOLOR color, D3DXCOLOR outlineColor )
{
	wstring textMsg;
	ConvertUtf8ToWCHAR( textMsg, pTextMsg );

	return AddText( fontIndex, pos, textMsg.c_str(), color, outlineColor );
}

bool CX2TextManager::CheckText( CX2Text* pTextForCheck )
{
	for ( int i = 0; i < (int)m_vecTextList.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecTextList[i];
		if ( pText == pTextForCheck )
		{
			return true;
		}
	}

	return false;
}

bool CX2TextManager::DeleteText( CX2TextManager::CX2Text* pTextForDelete )
{
	for ( int i = 0; i < (int)m_vecTextList.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecTextList[i];
		if ( pText == pTextForDelete )
		{
			SAFE_DELETE( pText );
			m_vecTextList.erase( m_vecTextList.begin() + i );
			return true;
		}
	}

	return false;
}

HRESULT	CX2TextManager::OnFrameMove( double fTime, float fElapsedTime )
{
	for ( int i = 0; i < (int)m_vecTextList.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecTextList[i];
		if ( pText != NULL )
		{
			if ( pText->GetIsAutoDeleteEnd() == true )
			{
				if ( DeleteText( pText ) == true )
				{
					--i;
					continue;
				}
			}

			pText->OnFrameMove( fTime, fElapsedTime );
		}
	}
	
	return S_OK;
}
HRESULT	CX2TextManager::OnFrameRender()
{
	for ( int i = 0; i < (int)m_vecTextList.size(); i++ )
	{
		CX2TextManager::CX2Text* pText = m_vecTextList[i];
		if ( pText != NULL )
		{
			pText->OnFrameRender();
		}
	}
	
	return S_OK;
}



CX2TextManager::CX2Text::CX2Text( int fontIndex, D3DXVECTOR2 pos, const WCHAR* pTextMsg, 
								 D3DXCOLOR color, D3DXCOLOR outlineColor, 
								 DWORD dtType /* = DT_LEFT */, float fLineSpace  /*= 1.0f */ )
{
	m_FontIndex		= fontIndex;

	m_TextMsg		= pTextMsg;
	m_DTType		= dtType;
	m_fLineSpace 	= fLineSpace;

	m_Pos			= pos;
	m_Color			= color;
	m_OutlineColor	= outlineColor;
	m_fAnimateTime	= 0.0f;

	m_GoalPos			= pos;
	m_GoalColor			= color;
	m_GoalOutlineColor	= outlineColor;
	m_fChangeTime		= 0.0f;

	m_bAutoDelete			= false;
	m_bCheckAutoDeleteEnd	= false;

	m_bShow					= true;
	m_bEnable				= true;

	
	m_iLineCount			= 1;
	if( NULL != pTextMsg )
	{
		int iFoundIndex = 0;
		int iEndIndex = 0;
		while( iFoundIndex != -1 && iFoundIndex < (int)m_TextMsg.length() )
		{
			iFoundIndex = m_TextMsg.find( L"\n", iEndIndex );
			if( iFoundIndex == std::wstring::npos || -1 == iFoundIndex || iFoundIndex == iEndIndex )
				break;

			m_iLineCount += 1;
			iEndIndex = iFoundIndex+1;
		}
	}


	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_FontIndex );
}

CX2TextManager::CX2Text::~CX2Text()
{
	m_pFont = NULL;
}

HRESULT CX2TextManager::CX2Text::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bEnable == false )
		return S_OK;

	if ( m_fChangeTime > 0.0f )
	{
		if ( m_fAnimateTime >= m_fChangeTime )
		{
			m_fAnimateTime = 0.0f;
			m_fChangeTime = 0.0f;

			m_Pos = m_GoalPos;
			m_Color = m_GoalColor;
			m_OutlineColor = m_GoalOutlineColor;

			if ( m_bAutoDelete == true )
			{
				m_bCheckAutoDeleteEnd = true;
			}
		}
		else
		{
			float _fRemainTime = m_fChangeTime - m_fAnimateTime;

			D3DXVECTOR2 remainPos		= m_GoalPos - m_Pos;
			m_Pos		+= (remainPos / _fRemainTime) * fElapsedTime;

			D3DXCOLOR remainColor = m_GoalColor - m_Color;
			m_Color += (remainColor / _fRemainTime)	* fElapsedTime;

			D3DXCOLOR remainOutlinColor = m_GoalOutlineColor - m_OutlineColor;
			m_OutlineColor += (remainOutlinColor / _fRemainTime )* fElapsedTime; 

			m_fAnimateTime += fElapsedTime;
		}
	}

	if ( m_fChangeTime == 0.0f )
	{
		if ( m_queMoveItems.empty() == false )
		{
			TextMoveItem tempMoveItem; 
			tempMoveItem = m_queMoveItems.front();

			m_TextMsg			= tempMoveItem.m_Msg;
			m_GoalPos			= tempMoveItem.m_Pos;
			m_GoalColor			= tempMoveItem.m_Color;
			m_GoalOutlineColor	= tempMoveItem.m_OutLineColor;
			m_fChangeTime		= tempMoveItem.m_fChangeTime;
			m_bAutoDelete		= tempMoveItem.m_bAutoDelete;

			m_queMoveItems.pop();
		}
	}

	return S_OK;
}

HRESULT CX2TextManager::CX2Text::OnFrameRender()
{
	if ( m_bShow == false )
		return S_OK;

	if ( m_pFont != NULL)
		m_pFont->OutTextMultiline( (int)m_Pos.x, (int)m_Pos.y, m_TextMsg.c_str(), m_Color, 
									CKTDGFontManager::FS_SHELL, m_OutlineColor,  m_fLineSpace, NULL, m_DTType );

	return S_OK;
}

void CX2TextManager::CX2Text::Move( D3DXVECTOR2 pos, const WCHAR* pMsg, D3DXCOLOR color, D3DXCOLOR outlineColor, 
								   float fChangeTime, bool bDirect /* = false */ , bool bAutoDelete /* = false */ )
{
	if ( pMsg == NULL )
		return;

	if ( bDirect == true )
	{
		while ( m_queMoveItems.empty() != true )
		{
			m_queMoveItems.pop();
		}

		m_fAnimateTime = 0.0f;

		m_TextMsg			= pMsg;
		m_GoalPos			= pos;
		m_GoalColor			= color;
		m_GoalOutlineColor	= outlineColor;
		m_fChangeTime		= fChangeTime;
		m_bAutoDelete		= bAutoDelete;
	}
	else
	{
		//큐에 집어넣어서 사용한다.

		TextMoveItem tempMoveItem;

		tempMoveItem.m_Msg			= pMsg;
		tempMoveItem.m_Pos			= pos;
		tempMoveItem.m_Color		= color;
		tempMoveItem.m_OutLineColor = outlineColor;
		tempMoveItem.m_fChangeTime	= fChangeTime;
		tempMoveItem.m_bAutoDelete	= bAutoDelete;

		m_queMoveItems.push( tempMoveItem );
	}	
}

void CX2TextManager::CX2Text::Move_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	bool bDirect;
	float fChangeTime;
	bool bAutoDelete;
	wstring wMsg;
	

	LUA_GET_VALUE( luaManager, "MSG", wMsg, L"" );
	LUA_GET_VALUE( luaManager, "IS_DIRECT", bDirect, false );
	LUA_GET_VALUE( luaManager, "CHANGE_TIME", fChangeTime, 0.0f );
	LUA_GET_VALUE( luaManager, "AUTO_DELETE", bAutoDelete, false );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    D3DXVECTOR2 pos;
    D3DXCOLOR color;
    D3DXCOLOR outlineColor;
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "POS", pos, D3DXVECTOR2(0,0) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "COLOR", color, D3DXCOLOR(1,1,1,1) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "OUTLINE_COLOR", outlineColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	D3DXVECTOR2 pos			= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "POS" );
	D3DXCOLOR color			= lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "COLOR" );
	D3DXCOLOR outlineColor	= lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "OUTLINE_COLOR" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	Move( pos, wMsg.c_str(), color, outlineColor, fChangeTime, bDirect, bAutoDelete );
}

int CX2TextManager::CX2Text::GetTextWidth( const WCHAR* pMsg )
{
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_FontIndex );
	if ( pFont == NULL )
		return -1;

	return pFont->GetWidth( pMsg );
}

int CX2TextManager::CX2Text::GetNowTextWidth()
{
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_FontIndex );
	if ( pFont == NULL )
		return -1;

	return pFont->GetWidth( m_TextMsg.c_str() );
}
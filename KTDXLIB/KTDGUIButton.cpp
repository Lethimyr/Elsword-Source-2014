#include "StdAfx.h"
#include ".\ktdguibutton.h"



CKTDGUIButton::CKTDGUIButton()
{
	m_ControlType	= CKTDGUIControl::UCT_BUTTON;
	m_ButtonState	= BCS_NORMAL;

	m_pNormalTex	= NULL;
	m_pMouseOverTex = NULL;
	m_pMouseDownTex = NULL;
	m_pDisableTex	= NULL;

	m_pNormalPoint		= NULL;
	m_pMouseOverPoint	= NULL;
	m_pMouseDownPoint	= NULL;
	m_pDisablePoint		= NULL;

	m_pStringData		= NULL;

	m_CustomMsgMouseOut		= -1;
	m_CustomMsgMouseOver	= -1;
	m_CustomMsgMouseDown	= -1;
	m_CustomMsgMouseUp		= -1;
	m_CustomMsgMouseRightUp = -1;
	m_CustomMsgMouseDblClk	= -1;

	m_CustomFuncMouseOver	= "";
	m_CustomFuncMouseDown	= "";
	m_CustomFuncMouseUp		= "";

	m_bIsSameNormalAndOverButton = false;

	m_bOverStateAtNormal	= false;
	m_bDownStateAtNormal	= false;
	m_bEnableClick			= true;

	m_bClicked				= false;
	m_bMouseOut				= false;

	m_pSndMouseOver			= NULL;
	m_pSndMouseUp			= NULL;
	m_pSndMouseDown			= NULL;

	m_EdgeColor = D3DXCOLOR( 1,1,1,1);
	m_EdgeWidth = 0;
	m_EdgeEnlargeValue = 0;
	m_pEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bDrawEdgeOut = false; //바깥쪽에 그릴것인가 안쪽에 그릴것인가?

	for ( int i = 0; i < CKTDGUIButton::BCS_END; i++ )
	{
		m_StaticControls[i] = NULL;
	}


//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring MouseOverSndFileName;
	wstring MouseUpSndFileName;

	LUA_GET_VALUE( kLuaManager, "Button_Mouse_Over", MouseOverSndFileName, L"" );
	LUA_GET_VALUE( kLuaManager, "Button_Mouse_Up",	 MouseUpSndFileName, L"" );

	m_pSndMouseOver = g_pKTDXApp->GetDeviceManager()->OpenSound( MouseOverSndFileName );
	m_pSndMouseUp = g_pKTDXApp->GetDeviceManager()->OpenSound( MouseUpSndFileName );
}

CKTDGUIButton::~CKTDGUIButton(void)
{

	SAFE_DELETE( m_pNormalTex );
	SAFE_DELETE( m_pMouseOverTex );
	SAFE_DELETE( m_pMouseDownTex );
	SAFE_DELETE( m_pDisableTex );

	SAFE_DELETE( m_pNormalPoint );
	SAFE_DELETE( m_pMouseOverPoint );
	SAFE_DELETE( m_pMouseDownPoint );
	SAFE_DELETE( m_pDisablePoint );

	SAFE_DELETE( m_pStringData );

	SAFE_CLOSE( m_pSndMouseOver );
	SAFE_CLOSE( m_pSndMouseUp );

	SAFE_CLOSE( m_pEdgeTexture );

	for ( int i = 0; i < CKTDGUIButton::BCS_END; i++ )
	{
		CKTDGUIStatic* pStatic = m_StaticControls[i];
		if ( pStatic != NULL && m_pDialog != NULL && m_pDialog->CheckControl(pStatic) == true )
			pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 0,0,0,0), 0.07f, true, true );
	}

	map< int, CKTDGUIControl::TexChangeData* >::iterator mit;
	for ( mit = m_mapMouseOverTex.begin(); mit != m_mapMouseOverTex.end(); mit++ )
	{
		CKTDGUIControl::TexChangeData* pTexChangeData = mit->second;
		SAFE_DELETE( pTexChangeData );
	}
	m_mapMouseOverTex.clear();

	for ( mit = m_mapMouseNormalTex.begin(); mit != m_mapMouseNormalTex.end(); mit++ )
	{
		CKTDGUIControl::TexChangeData* pTexChangeData = mit->second;
		SAFE_DELETE( pTexChangeData );
	}
	m_mapMouseNormalTex.clear();

	for ( mit = m_mapMouseDownTex.begin(); mit != m_mapMouseDownTex.end(); mit++ )
	{
		CKTDGUIControl::TexChangeData* pTexChangeData = mit->second;
		SAFE_DELETE( pTexChangeData );
	}
	m_mapMouseDownTex.clear();

	
}

HRESULT CKTDGUIButton::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return S_OK;
#endif
	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	if ( m_bHasFocus == true && m_EdgeWidth > 0 && 
		g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDialog ) == false )
	{
#ifdef SERV_KEY_MAPPING_INT
		if ( GET_KEY_STATE(GA_RETURN) || g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RETURN ) == TRUE )
#else // SERV_KEY_MAPPING_INT
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RETURN ) == TRUE )
#endif // SERV_KEY_MAPPING_INT
		{			
			LButtonMouseUp();						
		}
	}

	m_fElapsedTime = fElapsedTime;

	if( m_EndPoint.fChangeTime != m_NowPoint.fChangeTime )
	{
		ChangePoint( &m_NowPoint, &m_EndPoint );
	}

	if ( m_pDialog != NULL )
	{
		if ( m_bMouseOver == true && g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDialog ) == true )
			m_bMouseOver = false;
	}
	
	TexChange();

	switch( m_ButtonState )
	{
		case BCS_NORMAL:
			{
				
				
				if ( m_bOverStateAtNormal == true )
				{
					ChangeState( BCS_MOUSEOVER );
					PlaySound( m_pSndMouseOver );
				}
				else if ( m_bDownStateAtNormal == true )
				{
					ChangeState( BCS_MOUSEDOWN );
					PlaySound( m_pSndMouseOver );
				}
				else
				{
					if( m_bMouseOver == true )
					{
						ChangeState( BCS_MOUSEOVER );
						//if ( m_pSndMouseOver != NULL )
						//	m_pSndMouseOver->Play();
						PlaySound( m_pSndMouseOver );
					}
					else if( m_bEnable == false )
						ChangeState( BCS_DISABLE );
				}
			}
			break;

		case BCS_MOUSEOVER:
			{

				if ( m_bOverStateAtNormal == true )
				{
					if( m_bEnable == false )
						ChangeState( BCS_DISABLE );
				}
				else if ( m_bDownStateAtNormal == true )
				{
					if ( m_bEnable == true )
						ChangeState( BCS_NORMAL );
					else
						ChangeState( BCS_DISABLE );
				}	
				else
				{
					if( m_bMouseOver == false )
					{
						m_bMouseOut = true;
						ChangeState( BCS_NORMAL );
					}
					else if( m_bEnable == false )
						ChangeState( BCS_DISABLE );
				}

				ShowGuideDesc();
				
			}
			break;

		case BCS_MOUSEDOWN:
			{
				if ( m_bDownStateAtNormal == true )
				{
					if( m_bEnable == false )
						ChangeState( BCS_DISABLE );
				}	
				else
				{
					if( m_bMouseOver == false )
						ChangeState( BCS_NORMAL );
					else if( m_bEnable == false )
						ChangeState( BCS_DISABLE );
				}			
			}
			break;

		case BCS_DISABLE:
			{
#ifdef SHOW_DESCRIPTION_WHEN_DISABLE
				if( m_bShowDescriptionWhenDisable == true && m_bMouseOver == true )
				{
					ShowGuideDesc();
				}
#endif SHOW_DESCRIPTION_WHEN_DISABLE

				if( m_bEnable == true )
					ChangeState( BCS_NORMAL );
			}
			break;
	}

	return S_OK;
}

HRESULT CKTDGUIButton::OnFrameRender()
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

	if ( m_bHasFocus == true && m_EdgeWidth > 0 )
		DrawEdge( true );

	UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
		m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
		m_NowPoint );

	if ( m_bHasFocus == true )
	{
		/*
		D3DXCOLOR tempColor;

		tempColor.a = m_NowPoint.color.a * m_pDialog->GetColor().a * m_Color.a * 1;
		tempColor.r = m_NowPoint.color.r * m_pDialog->GetColor().r * m_Color.r * 0.8f;
		tempColor.g = m_NowPoint.color.g * m_pDialog->GetColor().g * m_Color.g * 0.8f;
		tempColor.b = m_NowPoint.color.b * m_pDialog->GetColor().b * m_Color.b * 0.8f;

		m_Vertex[CKTDGUIControl::VP_LEFT_TOP].color = tempColor;
		m_Vertex[CKTDGUIControl::VP_RIGHT_TOP].color = tempColor;
		m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM].color = tempColor;
		m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM].color = tempColor;
		*/
	}

	UpdateAnimVertex( m_Vertex, &m_NowTexChangeData );

	RenderVertex( m_Vertex, m_NowPoint );

	if ( m_bHasFocus == true && m_EdgeWidth > 0 )
		DrawEdge( false );

/*
	if( m_pStringData != NULL )
	{
		CKTDGFontManager::CKTDGFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont( m_pStringData->fontIndex );
		D3DXVECTOR2 pos;
		D3DXCOLOR	tempColor;
		D3DXCOLOR	tempOutLineColor;

		tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
		tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
		tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
		tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

		tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
		tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
		tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
		tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;


		pos = m_pStringData->pos + m_pDialog->GetPos() + m_OffsetPos;		
		pFont->Draw2DText( m_pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );
	}

*/
	if ( m_pStringData != NULL )
	{
		CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex );
		D3DXVECTOR2 pos;
		D3DXCOLOR	tempColor;
		D3DXCOLOR	tempOutLineColor;

		tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
		tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
		tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
		tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

		tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
		tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
		tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
		tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;




		pos = m_pStringData->pos + m_pDialog->GetPos() + m_OffsetPos;		
		//pFont->Draw2DText( pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle );
		pFont->OutTextMultiline( (int)pos.x, (int)pos.y, m_pStringData->msg.c_str(), tempColor, 
			m_pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, m_pStringData->sortFlag );

	}

	
	return S_OK;
}

void CKTDGUIButton::SetShow( bool bShow )
{
	CKTDGUIControl::SetShow( bShow );

	if ( bShow == false )
	{
		for ( int i = 0; i < CKTDGUIButton::BCS_END; i++ )
		{
			CKTDGUIStatic* pTempStatic = m_StaticControls[i];
			if ( pTempStatic != NULL )
				pTempStatic->SetShow( false );
		}
	}
	else
	{
		CKTDGUIStatic* pTempStatic = m_StaticControls[(int)m_ButtonState];
		if ( pTempStatic != NULL )
			pTempStatic->SetShow( true );
	}

	


	
}

void CKTDGUIButton::SetEnable( bool bEnable )
{
	CKTDGUIControl::SetEnable( bEnable );
}

//--------------------------------------------------------------------------------------


bool CKTDGUIButton::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false )
		return false;


	return false;
}


bool CKTDGUIButton::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

	if( m_bEnableClick == false )
		return false;

	switch( uMsg )
	{
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
			{
				if( ContainsPoint( pt ) == true )
				{
					if( m_bHasFocus == false )
					{	
						if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDialog ) == true )
							m_pDialog->RequestFocus( this );
					}

					// Pressed while inside the control
					PlaySound( m_pSndMouseDown );
					ChangeState( BCS_MOUSEDOWN );
					return true;
				}
				
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				// Button click
				if( ContainsPoint( pt ) == true )
				{
					if( m_CustomMsgMouseDblClk != -1 )
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseDblClk, (LPARAM)this );
					}

					PlaySound( m_pSndMouseUp );

					return true;
				}

			} break;

		case WM_LBUTTONUP:
			{
				// Button click
				if( ContainsPoint( pt ) == true && m_bHasFocus == true )
				{
					LButtonMouseUp();
				}
				return true;
				
			}
			break;

		case WM_RBUTTONUP:
			{
				// Button click
				if( ContainsPoint( pt ) == true && m_bHasFocus == true )
				{
					if( m_CustomMsgMouseRightUp != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseRightUp, (LPARAM)this );
					//{{ dmlee 2009/07/31 okmsgbox, okandcancel msgbox에서 bem_button_mouseup을 받으면 dialog를 지워버리게 되서 comment out
					//else
					//	g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_EVENT, BEM_BUTTON_MOUSEUP, (LPARAM)this, true );
					//}} dmlee 2009/07/31 okmsgbox, okandcancel msgbox에서 bem_button_mouseup을 받으면 dialog를 지워버리게 되서 comment out

					ChangeState( BCS_MOUSEOVER );

					PlaySound( m_pSndMouseUp );
				}
				return true;

			}
			break;

	};

	return false;
}

void CKTDGUIButton::ChangeState( BUTTON_CONTROL_STATE state, bool bForce )
{	
	if( m_ButtonState == state && bForce == false )
		return;

	if ( m_ButtonState != state && bForce == false )
	{
		m_NowTexChangeData.m_SequenceID = 0;
		m_NowTexChangeData.m_fShowTime = 0.0f;
		m_NowTexChangeData.m_bEndTexChange = false;
	}

	m_ButtonState = state;

	if ( bForce == false )
	{
		m_NowTexChangeData.m_fRotateDegree = 0.0f;
		m_NowTexChangeData.m_fScaleRotationDegree = 0.0f;
		m_NowTexChangeData.m_Pos = D3DXVECTOR2(0,0);
		m_NowTexChangeData.m_Scale = D3DXVECTOR2(1,1);
	}

	switch( m_ButtonState )
	{
		case BCS_NORMAL:
			{
				if( true == m_bMouseOut )
				{
					m_bMouseOut = false;
					if( m_CustomMsgMouseOut != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseOut, (LPARAM)this );
				}

				if( m_pNormalPoint != NULL )
				{
					m_EndPoint = *m_pNormalPoint;
					m_NowPoint.fChangeTime = 0.0f;
				}
			}
			break;

		case BCS_MOUSEOVER:
			{
				if( m_pMouseOverPoint != NULL )
					m_EndPoint = *m_pMouseOverPoint;
				else
					m_EndPoint = *m_pNormalPoint;

				m_NowPoint.fChangeTime = 0.0f;

				if( m_CustomMsgMouseOver != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseOver, (LPARAM)this );
				else
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, BEM_BUTTON_MOUSEOVER, (LPARAM)this );

				if ( m_CustomFuncMouseOver.empty() == false )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC, (WPARAM)m_CustomFuncMouseOver.c_str(), (LPARAM)this );
/*
				if ( m_CustomFuncMouseOver != "" )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC, (WPARAM)m_CustomFuncMouseOver.c_str(), (LPARAM)this );

				if ( m_CustomFuncMouseOver != "" )
				{
					string func;
					ConvertWCHARToChar( func, m_CustomFuncMouseOver.c_str() );
					lua_tinker::call<void>(m_CustomFuncMouseOver.c_str(), g_pKTDXApp, g_pX2Game, this );
				}
				*/
			}
			break;

		case BCS_MOUSEDOWN:
			{
				if( m_pMouseDownPoint != NULL )
					m_EndPoint = *m_pMouseDownPoint;
				else
					m_EndPoint = *m_pNormalPoint;

				m_NowPoint.fChangeTime = 0.0f;

				if( m_CustomMsgMouseDown != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseDown, (LPARAM)this );
				else
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, BEM_BUTTON_MOUSEDOWN, (LPARAM)this );

				if ( m_CustomFuncMouseDown.empty() == false )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC, (WPARAM)m_CustomFuncMouseDown.c_str(), (LPARAM)this );

				/*
				if ( m_CustomFuncMouseDown.empty() == false )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC, (WPARAM)m_CustomFuncMouseDown.c_str(), (LPARAM)this );	
					*/
			}
			break;

		case BCS_DISABLE:
			{
				if( m_pDisablePoint != NULL )
					m_EndPoint = *m_pDisablePoint;
				else
					m_EndPoint = *m_pNormalPoint;

				m_NowPoint.fChangeTime = 0.0f;

				if( m_bHasFocus == true )
				{
					if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDialog ) == true )
						m_pDialog->ClearFocus();
				}
			}
			break;
	}

#ifndef DLL_BUILD
	for ( int i = 0; i < CKTDGUIButton::BCS_END; i++ )
	{	
		CKTDGUIStatic* pTempStatic = m_StaticControls[i];
		if ( pTempStatic != NULL )
			pTempStatic->SetShow( false );
	}
	CKTDGUIStatic* pStaticToShow = m_StaticControls[m_ButtonState];
	if ( pStaticToShow != NULL )
		pStaticToShow->SetShow( true );
#endif

	if( m_EndPoint.fChangeTime == 0.0f )
	{
		m_edgePoint = m_NowPoint = m_EndPoint;
	}
}

bool CKTDGUIButton::SetName( const WCHAR* pName )
{
	if ( CKTDGUIControl::SetName( pName ) == true )
	 ReNameStatics();

	return true;
}

bool CKTDGUIButton::ContainsPoint( POINT pt )
{
	if ( CKTDGUIControl::ContainsPoint( pt ) == false )
		return false;

    if ( m_vEnableLeftTop != D3DXVECTOR2( 0, 0 ) && m_vEnableRightBottom != D3DXVECTOR2( 0, 0 ) )
	{
		D3DXVECTOR2 rightTopPoint = D3DXVECTOR2( m_vEnableRightBottom.x, m_vEnableLeftTop.y );
		D3DXVECTOR2 leftBottomPoint = D3DXVECTOR2( m_vEnableLeftTop.x, m_vEnableRightBottom.y );
	
		D3DXVECTOR2 resolPos = m_pDialog->GetPos();
		pt.x += (LONG)resolPos.x;
		pt.y += (LONG)resolPos.y;

		return Pick2DRect( pt, m_vEnableLeftTop, rightTopPoint, leftBottomPoint, m_vEnableRightBottom );
	}

	return true;
}

bool CKTDGUIButton::SetNormalTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pNormalTex );

	m_pNormalTex = SetTexureData_( pFileName, key );
	if( m_pNormalPoint != NULL )
		m_pNormalPoint->pUITextureData = m_pNormalTex;

	ChangeState( m_ButtonState, true );

	if( m_pNormalTex != NULL )
		return true;
	else
		return false;
}

void CKTDGUIButton::SetOverTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pMouseOverTex );

	m_pMouseOverTex = SetTexureData_( pFileName, key );
	if( m_pMouseOverPoint != NULL )
		m_pMouseOverPoint->pUITextureData = m_pMouseOverTex;

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetNormalAnimTex( CKTDGUIControl::TexChangeData* pTexChangeData )
{
	AddTexChangeData( m_mapMouseNormalTex, m_pNormalPoint, pTexChangeData );
}

void CKTDGUIButton::SetOverAnimTex( CKTDGUIControl::TexChangeData* pTexChangeData )
{
	AddTexChangeData( m_mapMouseOverTex, m_pMouseOverPoint, pTexChangeData );
}

void CKTDGUIButton::SetDownAnimTex( CKTDGUIControl::TexChangeData* pTexChangeData )
{
	AddTexChangeData( m_mapMouseDownTex, m_pMouseDownPoint, pTexChangeData );
}

void CKTDGUIButton::SetDownTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pMouseDownTex );

	m_pMouseDownTex = SetTexureData_( pFileName, key );
	if( m_pMouseDownPoint != NULL )
		m_pMouseDownPoint->pUITextureData = m_pMouseDownTex;

	ChangeState( m_ButtonState, true );
}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
void CKTDGUIButton::SetDisableTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pMouseDownTex );

	m_pDisableTex = SetTexureData_( pFileName, key );
	if( m_pDisablePoint != NULL )
		m_pDisablePoint->pUITextureData = m_pDisableTex;

	ChangeState( m_ButtonState, true );
}
#endif // UPGRADE_SKILL_SYSTEM_2013

void CKTDGUIButton::SetNormalTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pNormalTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pNormalTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pNormalPoint != NULL )
		m_pNormalPoint->pUITextureData = m_pNormalTex;
	
	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetOverTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pMouseOverTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	SetOverTex( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUIButton::SetDownTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pMouseDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pMouseDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pMouseDownPoint != NULL )
		m_pMouseDownPoint->pUITextureData = m_pMouseDownTex;

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetDisableTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pDisableTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pDisableTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pDisablePoint != NULL )
		m_pDisablePoint->pUITextureData = m_pDisableTex;

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetNormalAnimTex_LUA()
{
	SetNormalAnimTex( SetTexChangeData_() );
}

void CKTDGUIButton::SetOverAnimTex_LUA()
{	
	SetOverAnimTex( SetTexChangeData_() );
}

void CKTDGUIButton::SetDownAnimTex_LUA()
{
	SetDownAnimTex( SetTexChangeData_() );
}

void CKTDGUIButton::SetNormalStatic_LUA( CKTDGUIStatic* pStatic )
{
	m_StaticControls[CKTDGUIButton::BCS_NORMAL] = pStatic;

	if ( pStatic != NULL )
		pStatic->SetOffsetPos( m_OffsetPos );

	ReNameStatics();

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetOverStatic_LUA( CKTDGUIStatic* pStatic )
{
	m_StaticControls[CKTDGUIButton::BCS_MOUSEOVER] = pStatic;

	if ( pStatic != NULL )
		pStatic->SetOffsetPos( m_OffsetPos );

	ReNameStatics();

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetDownStatic_LUA( CKTDGUIStatic* pStatic )
{
	m_StaticControls[CKTDGUIButton::BCS_MOUSEDOWN] = pStatic;

	if ( pStatic != NULL )
		pStatic->SetOffsetPos( m_OffsetPos );

	ReNameStatics();

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetDisableStatic_LUA( CKTDGUIStatic* pStatic )
{
	m_StaticControls[CKTDGUIButton::BCS_DISABLE] = pStatic;

	if ( pStatic != NULL )
		pStatic->SetOffsetPos( m_OffsetPos );

	ReNameStatics();

	ChangeState( m_ButtonState, true );
}

CKTDGUIStatic* CKTDGUIButton::GetNormalStatic()
{
	return m_StaticControls[CKTDGUIButton::BCS_NORMAL];
}

void CKTDGUIButton::SetNormalPoint_LUA()
{
	SAFE_DELETE( m_pNormalPoint );
	m_pNormalPoint = SetPointData_();
	m_pNormalPoint->pUITextureData = m_pNormalTex;

	m_pNormalPoint->SetAutoPointByTextureSize();

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetOverPoint_LUA()
{
	SAFE_DELETE( m_pMouseOverPoint );

	m_pMouseOverPoint = SetPointData_();
	m_pMouseOverPoint->pUITextureData = m_pMouseOverTex;
	if ( m_pMouseOverPoint->pUITextureData == NULL )
	{
		if ( m_mapMouseOverTex.empty() == false )
		{
			map< int, CKTDGUIControl::TexChangeData* >::iterator mit;
			mit = m_mapMouseOverTex.find( 0 );
			if ( mit != m_mapMouseOverTex.end() )
			{
				m_pMouseOverPoint->pUITextureData = mit->second->m_pTexData;
			}
		}
	}

	m_pMouseOverPoint->SetAutoPointByTextureSize();
	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetDownPoint_LUA()
{
	SAFE_DELETE( m_pMouseDownPoint );
	m_pMouseDownPoint = SetPointData_();
	m_pMouseDownPoint->pUITextureData = m_pMouseDownTex;
	m_pMouseDownPoint->SetAutoPointByTextureSize();

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetDisablePoint_LUA()
{
	SAFE_DELETE( m_pDisablePoint );
	m_pDisablePoint = SetPointData_();
	m_pDisablePoint->pUITextureData = m_pDisableTex;
	m_pDisablePoint->SetAutoPointByTextureSize();

	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetString_LUA()
{
	SAFE_DELETE( m_pStringData );
	m_pStringData = SetStringData_();

	ChangeState( m_ButtonState, true );
}


void CKTDGUIButton::SetNormalPoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pNormalPoint );
	m_pNormalPoint = pUIPointData;
	m_pNormalPoint->pUITextureData = m_pNormalTex;
	m_pNormalPoint->SetAutoPointByTextureSize();
	ChangeState( m_ButtonState, true );
}

void CKTDGUIButton::SetOverPoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pMouseOverPoint );
	m_pMouseOverPoint = pUIPointData;
	m_pMouseOverPoint->pUITextureData = m_pMouseOverTex;
	m_pMouseOverPoint->SetAutoPointByTextureSize();
	ChangeState( m_ButtonState, true );
}
void CKTDGUIButton::SetDownPoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pMouseDownPoint );
	m_pMouseDownPoint = pUIPointData;
	m_pMouseDownPoint->pUITextureData = m_pMouseDownTex;
	m_pMouseDownPoint->SetAutoPointByTextureSize();
	ChangeState( m_ButtonState, true );
}
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
void CKTDGUIButton::SetDisablePoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pDisablePoint );
	m_pDisablePoint = pUIPointData;
	m_pDisablePoint->pUITextureData = m_pDisableTex;
	m_pDisablePoint->SetAutoPointByTextureSize();
	ChangeState( m_ButtonState, true );
}
#endif // UPGRADE_SKILL_SYSTEM_2013


D3DXVECTOR2 CKTDGUIButton::GetPos()
{
	assert( m_pNormalPoint != NULL );


	D3DXVECTOR2 pos = m_pNormalPoint->leftTopPoint + m_OffsetPos;


	return pos;
}

int CKTDGUIButton::GetWidth()
{
	assert( m_pNormalPoint != NULL );

	int width = -1;

	width = (int)(m_pNormalPoint->rightTopPoint.x - m_pNormalPoint->leftTopPoint.x);

	return width;
}

int CKTDGUIButton::GetHeight()
{
	assert( m_pNormalPoint != NULL );

	int height = -1;

	height = (int)(m_pNormalPoint->leftBottomPoint.y - m_pNormalPoint->leftTopPoint.y);

	return height;
}

void CKTDGUIButton::SetOffsetPos( D3DXVECTOR2 pos )
{
	CKTDGUIControl::SetOffsetPos( pos );

	for ( int i = 0; i < (int)CKTDGUIButton::BCS_END; i++ )
	{
		CKTDGUIStatic* pStatic = m_StaticControls[i];
		if ( pStatic != NULL )
			pStatic->SetOffsetPos( pos );
	}
}

void CKTDGUIButton::SetDownStateAtNormal( bool bCheck )
{ 
	m_bDownStateAtNormal = bCheck; 
}

D3DXVECTOR2 CKTDGUIButton::GetGuideDescPos()
{
	D3DXVECTOR2 _pos = D3DXVECTOR2(0,0);

	if ( m_vGuideDescAbsolutePos.x != 0 && m_vGuideDescAbsolutePos.y != 0 )
	{
		_pos = m_vGuideDescAbsolutePos;
	}
	else
	{
		if ( m_pMouseOverPoint != NULL )
		{
			_pos.x = ( m_pMouseOverPoint->leftTopPoint.x + m_pMouseOverPoint->rightTopPoint.x ) / 2.0f;
			_pos.y = m_pMouseOverPoint->leftTopPoint.y;
		}
		else
		{
			_pos = m_vGuideDescAbsolutePos;
		}
	}

	_pos += m_vGuideDescOffsetPos;

	return _pos;
}

void CKTDGUIButton::SetEdge( bool bDrawOut, int _width, D3DXCOLOR _color )
{
	m_bDrawEdgeOut = bDrawOut;
	m_EdgeWidth = _width;
	m_EdgeColor = _color;
	
}

void CKTDGUIButton::SetEdge2( bool bDrawOut, int _width, D3DXCOLOR _color, int enlargeValue )
{
	m_bDrawEdgeOut = bDrawOut;
	m_EdgeWidth = _width;
	m_EdgeColor = _color;
	m_EdgeEnlargeValue = enlargeValue;
}



void CKTDGUIButton::ChangeMouseUpSound( const WCHAR* wszFileName )
{
	SAFE_CLOSE( m_pSndMouseUp );

	if( NULL == wszFileName ||
		wcslen(wszFileName) <= 0 )
	{
		return;
	}

	m_pSndMouseUp = g_pKTDXApp->GetDeviceManager()->OpenSound( wszFileName );
}

void CKTDGUIButton::ChangeMouseUpSound_LUA( const char* szFileName )
{	
	wstring fileName;
	ConvertUtf8ToWCHAR( fileName, szFileName );
	ChangeMouseUpSound( fileName.c_str() );
}

void CKTDGUIButton::ChangeMouseDownSound( const WCHAR* wszFileName )
{
	SAFE_CLOSE( m_pSndMouseDown );
	m_pSndMouseDown = g_pKTDXApp->GetDeviceManager()->OpenSound( wszFileName );
}

void CKTDGUIButton::ChangeMouseDownSound_LUA( const char* szFileName )
{
	wstring fileName;
	ConvertUtf8ToWCHAR( fileName, szFileName );
	ChangeMouseDownSound( fileName.c_str() );
}

void CKTDGUIButton::ChangeMouseOverSound( const WCHAR* wszFileName )
{
	SAFE_CLOSE( m_pSndMouseOver );
	m_pSndMouseOver = g_pKTDXApp->GetDeviceManager()->OpenSound( wszFileName );
}

void CKTDGUIButton::ChangeMouseOverSound_LUA( const char* szFileName )
{
	wstring fileName;
	ConvertUtf8ToWCHAR( fileName, szFileName );
	ChangeMouseOverSound( fileName.c_str() );
}

void CKTDGUIButton::SetMaximize( bool val )
{
	CKTDGUIControl::SetMaximize( val );

	int iSign = 1;
	if ( val == false )
		iSign = -1;

	m_OffsetPos.x -= m_kMaximizeOffset.x * iSign;
	m_OffsetPos.y -= m_kMaximizeOffset.y * iSign;
}

void CKTDGUIButton::ReNameStatics()
{
	for ( int i = 0; i < CKTDGUIButton::BCS_END; i++ )
	{
		CKTDGUIStatic* pStatic = m_StaticControls[i];
		if ( pStatic != NULL && m_pDialog != NULL )
		{
			WCHAR buff[256] = {0};
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%s_Static_Uniqe_Button_%d", GetName(), i );
			pStatic->SetName( buff );
		}
	}
}



void CKTDGUIButton::DrawEdge( bool bDrawOut )
{
	if ( m_EdgeWidth == 0 || m_pEdgeTexture == NULL )
		return;

	D3DXCOLOR tempColor;

	tempColor.a = m_EdgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = m_EdgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = m_EdgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = m_EdgeColor.b * m_pDialog->GetColor().b * m_Color.b;


	int _width = (int)(m_edgePoint.rightBottomPoint.x - m_edgePoint.leftTopPoint.x);
	int _height = (int)(m_edgePoint.rightBottomPoint.y - m_edgePoint.leftTopPoint.y);

	if ( bDrawOut == true )
	{
		// 좌
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.leftTopPoint.x - m_EdgeWidth) + -m_EdgeEnlargeValue, 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftTopPoint.y - m_EdgeWidth) - m_EdgeEnlargeValue, 
			m_EdgeWidth, 
			_height + m_EdgeWidth + (m_EdgeEnlargeValue*2), 
			tempColor );

		// 하

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.leftTopPoint.x - m_EdgeWidth) + -m_EdgeEnlargeValue, 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftBottomPoint.y ) +m_EdgeEnlargeValue , 
			_width + m_EdgeWidth +m_EdgeEnlargeValue , 
			m_EdgeWidth , 
			tempColor );

		// 우

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.rightTopPoint.x ) + m_EdgeEnlargeValue, 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftTopPoint.y ) , 
			m_EdgeWidth, 
			_height + m_EdgeWidth + (m_EdgeEnlargeValue) ,
			tempColor );

		// 상

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftTopPoint.y - m_EdgeWidth ) + -m_EdgeEnlargeValue, 
			_width + m_EdgeWidth + (m_EdgeEnlargeValue*2), 
			m_EdgeWidth , 
			tempColor );
	}
	else
	{
		// 좌
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.leftTopPoint.x ) + -m_EdgeEnlargeValue, 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftTopPoint.y ) - m_EdgeEnlargeValue, 
			m_EdgeWidth, 
			_height + (m_EdgeEnlargeValue*2), 
			tempColor );

		// 하

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.leftTopPoint.x) + -m_EdgeEnlargeValue, 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftBottomPoint.y - m_EdgeWidth ) + m_EdgeEnlargeValue, 
			_width +m_EdgeEnlargeValue, 
			m_EdgeWidth , 
			tempColor );

		// 우

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.rightTopPoint.x - m_EdgeWidth ) + m_EdgeEnlargeValue, 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftTopPoint.y ), 
			m_EdgeWidth, 
			_height + (m_EdgeEnlargeValue), 
			tempColor );

		// 상

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_edgePoint.leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_edgePoint.leftTopPoint.y ) + -m_EdgeEnlargeValue, 
			_width + (m_EdgeEnlargeValue*2), 
			m_EdgeWidth , 
			tempColor );

	}


}

void CKTDGUIButton::LButtonMouseUp()
{
	if ( m_pDialog != NULL )
		m_pDialog->ClearFocus();

	if( m_CustomMsgMouseUp != -1 )
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseUp, (LPARAM)this );
	else
		g_pKTDXApp->SendGameMessage( CKTDXApp::KM_UI_CONTROL_EVENT, BEM_BUTTON_MOUSEUP, (LPARAM)this, true );

	if ( m_CustomFuncMouseUp.empty() == false )
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_FUNC, (WPARAM)m_CustomFuncMouseUp.c_str(), (LPARAM)this );

	ChangeState( BCS_NORMAL );

	//if ( m_pSndMouseUp != NULL )
	//	m_pSndMouseUp->Play();
	PlaySound( m_pSndMouseUp );
}

void CKTDGUIButton::TexChange()
{
	map< int, CKTDGUIControl::TexChangeData* >* pmapNowTexChangeData = NULL;
	CKTDGUIControl::UIPointData* pUIPointData = NULL;

	if ( m_ButtonState == CKTDGUIButton::BCS_MOUSEOVER )
	{
		pmapNowTexChangeData = &m_mapMouseOverTex;
		pUIPointData = m_pMouseOverPoint;
		
	}
	else if ( m_ButtonState == CKTDGUIButton::BCS_NORMAL )
	{
		pmapNowTexChangeData = &m_mapMouseNormalTex;
		pUIPointData = m_pNormalPoint;
	}
	else if ( m_ButtonState == CKTDGUIButton::BCS_MOUSEDOWN )
	{
		pmapNowTexChangeData = &m_mapMouseDownTex;
		pUIPointData = m_pMouseDownPoint;
	}

	if ( pmapNowTexChangeData != NULL && pmapNowTexChangeData->empty() == false && m_NowTexChangeData.m_bEndTexChange == false )
	{
		CKTDGUIControl::TexChangeData* pTexChangeData = NULL;

		map< int, CKTDGUIControl::TexChangeData* >::iterator mit;
		mit = pmapNowTexChangeData->find( m_NowTexChangeData.m_SequenceID );
		if ( mit != pmapNowTexChangeData->end() )
		{ 
			pTexChangeData = mit->second;
		}

		UpdateTexChangeData( pTexChangeData );

		if ( pTexChangeData != NULL && pTexChangeData->m_pTexData != NULL )
		{
			if( pUIPointData != NULL )
			{
				if (  pUIPointData->pUITextureData != pTexChangeData->m_pTexData && pTexChangeData->m_pTexData != NULL )
				{
					pUIPointData->pUITextureData = pTexChangeData->m_pTexData;
					pUIPointData->SetAutoPointByTextureSize();

					ChangeState( m_ButtonState, true );
				}
			}
		}
	}
}

/*virtual*/ void CKTDGUIButton::MoveControl( float fx, float fy )
{
	if( fx == 0.f && fy == 0.f )
		return;

	if( NULL != m_pNormalPoint )
	{
		m_pNormalPoint->Move( fx, fy );
		m_NowPoint = *m_pNormalPoint;
		m_EndPoint = *m_pNormalPoint;
	}

	if( NULL != m_pMouseOverPoint )
	{
		m_pMouseOverPoint->Move( fx, fy );
	}

	if( NULL != m_pMouseDownPoint )
	{
		m_pMouseDownPoint->Move( fx, fy );
	}

	if( NULL != m_pDisablePoint )
	{
		m_pDisablePoint->Move( fx, fy );
	}
}

/*virtual*/ void CKTDGUIButton::ScaleControl( float fx, float fy )
{
	if( fx == 1.f && fy == 1.f )
		return; 

	if( NULL != m_pNormalPoint )
	{
		m_pNormalPoint->Scale( fx, fy );
		m_pNormalPoint->bUseTextureSize = false;
		m_NowPoint = *m_pNormalPoint;
		m_EndPoint = *m_pNormalPoint;
	}

	if( NULL != m_pMouseOverPoint )
	{
		m_pMouseOverPoint->Scale( fx, fy );
		m_pMouseOverPoint->bUseTextureSize = false;
	}

	if( NULL != m_pMouseDownPoint )
	{
		m_pMouseDownPoint->Scale( fx, fy );
		m_pMouseDownPoint->bUseTextureSize = false;
	}

	if( NULL != m_pDisablePoint )
	{
		m_pDisablePoint->Scale( fx, fy );
		m_pDisablePoint->bUseTextureSize = false;
	}
}

#ifdef DLL_BUILD
vector<D3DXVECTOR2> CKTDGUIButton::GetPosList()
{
	vector<D3DXVECTOR2> ret;	

	if( NULL != m_pNormalPoint )
		ret.push_back(m_pNormalPoint->leftTopPoint);


	if( NULL != m_pMouseOverPoint )
		ret.push_back(m_pMouseOverPoint->leftTopPoint);

	if( NULL != m_pMouseDownPoint )
		ret.push_back(m_pMouseDownPoint->leftTopPoint);

	if( NULL != m_pDisablePoint && m_pDisablePoint->leftTopPoint.x != 0 && m_pDisablePoint->leftTopPoint.y != 0 )
		// 버튼들 중에 Disable설정이 없는것도 있다.
		ret.push_back(m_pDisablePoint->leftTopPoint);

	return ret;
}

D3DXVECTOR2 CKTDGUIButton::GetPos(wstring name)
{
	if( NULL != m_pNormalPoint && name == L"Normal" )
		return m_pNormalPoint->leftTopPoint;
	else if( NULL != m_pMouseOverPoint && name == L"Over" )
		return m_pMouseOverPoint->leftTopPoint;
	else if( NULL != m_pMouseDownPoint && name == L"Down" )
		return m_pMouseDownPoint->leftTopPoint;
	else if( NULL != m_pDisablePoint && name == L"Disable" )
		return m_pDisablePoint->leftTopPoint;

	return D3DXVECTOR2();

}

void CKTDGUIButton::MoveSubControl( float fx, float fy, wstring subControlName )
{
	if( NULL != m_pNormalPoint && subControlName == L"Normal" )
	{
		m_pNormalPoint->Move( fx, fy );
		m_NowPoint = *m_pNormalPoint;
		//m_EndPoint = *m_pNormalPoint;
	}
	else if( NULL != m_pMouseOverPoint && subControlName == L"Over" )
	{
		m_pMouseOverPoint->Move( fx, fy );
		m_NowPoint = *m_pMouseOverPoint;
		//m_EndPoint = *m_pMouseOverPoint;
	}
	else if( NULL != m_pMouseDownPoint && subControlName == L"Down" )
	{
		m_pMouseDownPoint->Move( fx, fy );
		m_NowPoint = *m_pMouseDownPoint;
		//m_EndPoint = *m_pMouseDownPoint;
	}

	else if( NULL != m_pDisablePoint && subControlName == L"Disable" )
	{
		m_pDisablePoint->Move( fx, fy );
		m_NowPoint = *m_pDisablePoint;
		//m_EndPoint = *m_pDisablePoint;
	}
}

void CKTDGUIButton::SetEditGUI( bool bEdit )
{
	m_bUpdate = !bEdit;
}

void CKTDGUIButton::ShowSubView( wstring name, bool bView )
{
	SetColor(D3DXCOLOR(0xffffffff));	

	if( true == bView )
	{
		if( name == L"Normal" )
			ChangeState( BCS_NORMAL );
		else if( name == L"Over" )
			ChangeState( BCS_MOUSEOVER );
		else if( name == L"Down" )
			ChangeState( BCS_MOUSEDOWN );
		else if( name == L"Disable" )
			ChangeState( BCS_DISABLE );
	}

	if( true == bView )
	{		
		OnFocusIn();
		SetEdge(true, 1,  D3DXCOLOR(1,0,0,1) );
		m_edgePoint = *m_pNormalPoint;
	}
	else
	{
		OnFocusOut();
		SetEdge(true, 0,  D3DXCOLOR(1,1,1,1) );
	}
}

CKTDGUIControl::UIPointData * CKTDGUIButton::_GetPointData( wstring name )
{
	if( NULL !=  m_pNormalPoint && name == L"Normal" )
		return m_pNormalPoint;

	else if( NULL != m_pMouseOverPoint && name == L"Over" )
		return m_pMouseOverPoint;		

	else if( NULL != m_pMouseDownPoint && name == L"Down" )
		return m_pMouseDownPoint;

	else if( NULL != m_pDisablePoint && name == L"Disable" )
		return m_pDisablePoint;

	return NULL;
}

wstring CKTDGUIButton::GetTextureName( wstring name )
{
	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );

	if( NULL != pPoint && NULL != pPoint->pUITextureData )
	{
		return pPoint->pUITextureData->texName;
	}

	return L"";
}

RECT CKTDGUIButton::GetTextureUV(wstring name)
{
	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );

	RECT rt;
	if( NULL != pPoint && NULL != pPoint->pUITextureData )
	{
		D3DXVECTOR2 leftTop = pPoint->pUITextureData->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP];
		D3DXVECTOR2 rightBottom = pPoint->pUITextureData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM];

		rt.left = leftTop.x;
		rt.top = leftTop.y;
		rt.right = rightBottom.x;
		rt.bottom = rightBottom.y;
	}

	return rt;
}

wstring CKTDGUIButton::GetTextureKey( wstring name )
{
	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );

	if( NULL != pPoint && NULL != pPoint->pUITextureData )
	{
		return pPoint->pUITextureData->keyName;
	}

	return L"";
}

vector<wstring> CKTDGUIButton::GetTextureKeyList( wstring name )
{
	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );

	vector<wstring> ret;
	if( NULL != pPoint && NULL != pPoint->pUITextureData && NULL != pPoint->pUITextureData->pTexture )
	{
		//GetMapTexUVRect
		const CKTDXDeviceTexture::KeyTexUVMap & uvMap = pPoint->pUITextureData->pTexture->GetMapTexUVRect();

		CKTDXDeviceTexture::KeyTexUVMap::const_iterator itor = uvMap.cbegin();
		for( ; itor != uvMap.end() ; itor++)
		{
			ret.push_back(itor->first);
		} 
	}
	
	return ret;
}

void CKTDGUIButton::SetTexture( wstring name, wstring fileName )
{		
	CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( fileName );
	if( NULL == pTexture)
		return;

	wstring key = L"NONE";
	const CKTDXDeviceTexture::KeyTexUVMap & uvMap = pTexture->GetMapTexUVRect();				
	CKTDXDeviceTexture::KeyTexUVMap::const_iterator itor = uvMap.cbegin();
	if( itor != uvMap.cend() )
		key = itor->first;

	// todo : SetNoarmlTex 인자에 CKTDXDeviceTexture 추가가 필요할것 같다.

	if( name == L"Normal" )
		SetNormalTex( fileName.c_str(), key.c_str() );

	else if( name == L"Over" )
		SetOverTex( fileName.c_str(), key.c_str() );

	else if( name == L"Down" )
		SetDownTex( fileName.c_str(), key.c_str() );

	else if( name == L"Disable" )
		SetDisableTex( fileName.c_str(), key.c_str() );

	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );
	if( NULL != pPoint ) 
		pPoint->SetAutoPointByTextureSize();

	m_NowPoint = *pPoint;
}

void CKTDGUIButton::SetTextureKey( wstring name, wstring key )
{
	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );

	if( NULL != pPoint && NULL != pPoint->pUITextureData && pPoint->pUITextureData->pTexture )
	{
		pPoint->pUITextureData->keyName	= key;
		MakeUpperCase(key);
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = pPoint->pUITextureData->pTexture->GetTexUV( key );
		if( pTexUV != NULL )
		{
			pPoint->pUITextureData->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]		= pTexUV->leftTop;
			pPoint->pUITextureData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
			pPoint->pUITextureData->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUV->leftBottom;
			pPoint->pUITextureData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUV->rightBottom;

			pPoint->pUITextureData->texSize = pTexUV->rectSize;

			pPoint->pUITextureData->SetTextureUV();
		}

		pPoint->SetAutoPointByTextureSize();

		m_NowPoint = *pPoint;
	}
}

#endif

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
/* virtual */	void	CKTDGUIButton::SetCustomMouseOverSound ( wstring wstrSoundFileName )
{
	m_pSndMouseOver = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrSoundFileName );
}
/* virtual */	void	CKTDGUIButton::SetCustomMouseUpSound  ( wstring wstrSoundFileName )
{
	m_pSndMouseUp = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrSoundFileName );
}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
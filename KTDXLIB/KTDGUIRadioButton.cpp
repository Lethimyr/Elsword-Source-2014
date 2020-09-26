#include "StdAfx.h"
#include ".\ktdguiradiobutton.h"


CKTDGUIRadioButton::CKTDGUIRadioButton()
{
	m_ControlType	= CKTDGUIControl::UCT_RADIOBUTTON;
	m_RadioButtonState	= RBCS_MOUSEOVER;
	m_bChecked		= false;
	m_bPressed		= false;
	m_bRButtonDown	= false;
	m_bEnableDoubleClick = false;
	m_bEnableCheckToggle = false;

	m_pNormalTex	= NULL;
	m_pMouseOverTex	= NULL;
	m_pDisableTex	= NULL;
	m_pCheckedTex	= NULL;

	m_pBGPoint			= NULL;
	m_pCheckedPoint		= NULL;
	m_pRadioButtonPoint = NULL;
	m_pBGMouseOverPoint = NULL;
	m_pDisabledPoint	= NULL;
	
	m_pSndCheck			= NULL;
	m_pSndMouseOver		= NULL;

	m_CustomMsgMouseOut			= -1;
	m_CustomMsgMouseOver		= -1;
	m_CustomMsgChecked			= -1;
	m_CustomMsgUnChecked		= -1;
	m_CustomMsgDoubleClicked	= -1;
	m_CustomMsgRButtonUp		= -1;


	m_pStringData = NULL;

	m_bShowOffBGByCheck = false;
	m_bFixOverByCheck = false;

	m_GroupID = -1;

	m_CheckedEdgeColor = D3DXCOLOR( 1, 1, 1, 1 );
	m_CheckedEdgeWidth = 0;
	m_pCheckedEdgeTexture = NULL;
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bDrawCheckedEdgeOut = true;


//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring checkSndFileName;

	LUA_GET_VALUE( kLuaManager, "RadioButton_Check", checkSndFileName, L"" );
	m_pSndCheck = g_pKTDXApp->GetDeviceManager()->OpenSound( checkSndFileName );

	LUA_GET_VALUE( kLuaManager, "RadioButton_MouseOver", checkSndFileName, L"" );
	m_pSndMouseOver = g_pKTDXApp->GetDeviceManager()->OpenSound( checkSndFileName );

#ifdef DLL_BUILD
	m_bEditEdge = false;
#endif

}

CKTDGUIRadioButton::~CKTDGUIRadioButton()
{
				   
	SAFE_DELETE( m_pNormalTex );
	SAFE_DELETE( m_pMouseOverTex );
	SAFE_DELETE( m_pDisableTex );
	SAFE_DELETE( m_pCheckedTex );

	SAFE_DELETE( m_pBGPoint );
	SAFE_DELETE( m_pCheckedPoint );
	SAFE_DELETE( m_pBGMouseOverPoint );
	SAFE_DELETE( m_pRadioButtonPoint );
	SAFE_DELETE( m_pDisabledPoint );


	SAFE_DELETE( m_pStringData );

	SAFE_CLOSE( m_pSndCheck );
	SAFE_CLOSE( m_pSndMouseOver );

	SAFE_CLOSE( m_pCheckedEdgeTexture );

	if ( m_pDialog != NULL )
	{
		CKTDGUIRadioButton::RadioButtonGroupInfo* pRadioButtonGroupInfo = m_pDialog->GetRadioButtonGroupInfo( m_GroupID );
		if ( pRadioButtonGroupInfo != NULL )
		{
			if( pRadioButtonGroupInfo->DeleteRadioButton( this ) == true )
			{
				int i = 0;
			}
		}
	}
}

HRESULT CKTDGUIRadioButton::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return S_OK;
#endif

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	m_fElapsedTime	= fElapsedTime;

	if( m_EndPoint.fChangeTime != m_NowPoint.fChangeTime )
	{
		ChangePoint( &m_NowPoint, &m_EndPoint );
	}
	if( m_MyEndPoint.fChangeTime != m_MyNowPoint.fChangeTime )
	{
		ChangePoint( &m_MyNowPoint, &m_MyEndPoint );
	}

	

	switch( m_RadioButtonState )
	{
	case RBCS_NORMAL:
		{
			if( m_bMouseOver == true )
			{
				if (  m_bChecked == false )
				{
					PlaySound( m_pSndMouseOver );
				}

				if( m_CustomMsgMouseOver != -1 )
				{
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseOver, (LPARAM)this );
				}

				ChangeState( RBCS_MOUSEOVER );
			}
			else if( m_bEnable == false )
				ChangeState( RBCS_DISABLE );
		}
		break;

	case RBCS_MOUSEOVER:
		{
			if( m_bMouseOver == false )
			{
				if( m_CustomMsgMouseOver != -1 )
				{
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseOut, (LPARAM)this );
				}
				ChangeState( RBCS_NORMAL );
			}
			else if( m_bEnable == false )
				ChangeState( RBCS_DISABLE );

			ShowGuideDesc();
		}
		break;

	case RBCS_DISABLE:
		{
#ifdef SHOW_DESCRIPTION_WHEN_DISABLE
			if( m_bShowDescriptionWhenDisable == true && m_bMouseOver == true )
			{
				ShowGuideDesc();
			}
#endif SHOW_DESCRIPTION_WHEN_DISABLE

			if( m_bEnable == true )
				ChangeState( RBCS_NORMAL );
		}
		break;
	}

	return S_OK;
}

HRESULT CKTDGUIRadioButton::OnFrameRender()
{
	HRESULT hr	= S_OK;

	if( m_bShow == false )
		return hr;

	//BG Draw

	if ( m_bChecked == true )
		DrawCheckedEdge( true );

#ifdef DLL_BUILD
	DrawEditEdge(m_pRadioButtonPoint);
#endif

	if ( m_bChecked == true && m_bShowOffBGByCheck == true )
	{
/*
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			*m_pCheckedPoint );

		RenderVertex( m_Vertex, *m_pCheckedPoint );
*/
	}
	else
	{
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			m_MyNowPoint );

		RenderVertex( m_Vertex, m_MyNowPoint );
	}

	if ( m_bChecked == true )
		DrawCheckedEdge( false );

	D3DXCOLOR	tempColor;
	D3DXCOLOR	tempOutLineColor;

	if ( m_pStringData != NULL )
	{
		tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
		tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
		tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
		tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

		tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
		tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
		tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
		tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

		CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex );
		D3DXVECTOR2 pos;
		pos = m_pStringData->pos + m_pDialog->GetPos() + m_OffsetPos;		
		//pFont->Draw2DText( m_pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );
		pFont->OutTextMultiline( (int)pos.x, (int)pos.y, m_pStringData->msg.c_str(), tempColor, m_pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, m_pStringData->sortFlag );
	}
	

	//Checked Mark Draw
	if( m_bChecked == true )
	{
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			*m_pCheckedPoint );

		RenderVertex( m_Vertex, *m_pCheckedPoint );
	}

	return hr;
}

//--------------------------------------------------------------------------------------
bool CKTDGUIRadioButton::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

	switch( uMsg )
	{
	case WM_RBUTTONDOWN:
		{
			if( ContainsPoint( pt ) )
			{
				m_bRButtonDown = true;
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif
				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				return true;
			}
		} break;

	case WM_RBUTTONUP:
		{
			if( true == m_bRButtonDown )
			{
				m_bRButtonDown = false;
				ReleaseCapture();

				if( ContainsPoint( pt ) == true && m_bHasFocus == true )
				{
					if( m_CustomMsgRButtonUp != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgRButtonUp, (LPARAM)this );

					//PlaySound( m_pSndCheck );
				}
				
				return true;
			}
		} break;

	case WM_LBUTTONDOWN:
		{
			if( false == m_bEnableCheckToggle )
			{
				if ( m_bChecked == true )
					return false;
			}

			if( ContainsPoint( pt ) )
			{
				// Pressed while inside the control
				m_bPressed = true;
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif
				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				return true;
			}
		}
		break;
	
	case WM_LBUTTONDBLCLK:
		{
			if ( m_bEnableDoubleClick == false )
				return true;

			if( ContainsPoint( pt ) )
			{
				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				PlaySound( m_pSndCheck );

				// radio group info가 없거나 최대 선택할 수 있는 개수가 1개이면 예전처럼 처리
				RadioButtonGroupInfo* pInfo = m_pDialog->GetRadioButtonGroupInfo( this->m_GroupID );
				if( NULL == pInfo || pInfo->m_iMaxCheckCount == 1 )
				{
					for( int i = 0; i < m_pDialog->GetControlSize(); i++ )
					{
						if( m_pDialog->GetControl(i)->GetType() == CKTDGUIControl::UCT_RADIOBUTTON )
						{
							CKTDGUIRadioButton* temp = (CKTDGUIRadioButton*) m_pDialog->GetControl(i);
							if( temp->GetGroupID() == m_GroupID && temp != this )
							{
								temp->SetChecked( false );
							}
						}	
					}
				}

				SetCheckedInternal( true, true, true );
				return true;
			}
		}
		break;
		

	case WM_LBUTTONUP:
		{
			if( m_bPressed == true )
			{
				m_bPressed = false;
				ReleaseCapture();

				// Button click
				if( ContainsPoint( pt ) )
				{
					PlaySound( m_pSndCheck );

					// radio group info가 없거나 최대 선택할 수 있는 개수가 1개이면 원래 코드에서 처리
					RadioButtonGroupInfo* pInfo = m_pDialog->GetRadioButtonGroupInfo( this->m_GroupID );
					if( NULL == pInfo || pInfo->m_iMaxCheckCount == 1 )
					{
						for( int i = 0; i < m_pDialog->GetControlSize(); i++ )
						{
							if( m_pDialog->GetControl(i)->GetType() == CKTDGUIControl::UCT_RADIOBUTTON )
							{
								CKTDGUIRadioButton* temp = (CKTDGUIRadioButton*) m_pDialog->GetControl(i);
#ifdef RADIOBUTTON_INIT_NORMAL
								if( temp->GetGroupID() == m_GroupID )
#else
								if( temp->GetGroupID() == m_GroupID && temp != this )
#endif //RADIOBUTTON_INIT_NORMAL
								{
									temp->SetChecked( false );
								}
							}	
						}
					}

					if( true == m_bEnableCheckToggle )
					{
						SetCheckedInternal( !m_bChecked, false, true );
					}
					else
					{
						SetCheckedInternal( true, false, true );
					}
				}
				return true;
			}

			break;
		}
	};

	return false;
}

void CKTDGUIRadioButton::ChangeState( RADIOBUTTON_CONTROL_STATE state )
{	
	if( m_RadioButtonState == state )
		return;
#ifdef RADIOBUTTON_INIT_NORMAL
	if( m_bChecked == true ) // 체크 상태인 버튼은 영향을 받지 않는다.
		return;
#endif //RADIOBUTTON_INIT_NORMAL
	m_RadioButtonState = state;


//	m_NowPoint = m_EndPoint = *m_pBGPoint;
	m_NowPoint = m_EndPoint = *m_pRadioButtonPoint;

	switch( m_RadioButtonState )
	{
	case RBCS_NORMAL:
		{
		//	m_MyNowPoint.pUITextureData	= m_pNormalTex;		

		//	m_EndPoint = *m_pBGPoint;
		//	m_NowPoint.fChangeTime = 0.0f;

			if ( m_bChecked == true && m_bFixOverByCheck == true )
			{
				if ( m_MyEndPoint.pUITextureData == NULL )
				{
					if( m_pBGMouseOverPoint != NULL )
						m_MyEndPoint = *m_pBGMouseOverPoint;
				}
			}
			else
			{
				if ( m_pBGPoint != NULL )
				{
					m_MyEndPoint = *m_pBGPoint;
				}
				m_MyNowPoint.fChangeTime = 0.0f;
			}
		}
		break;

	case RBCS_MOUSEOVER:
		{
			if( m_pBGMouseOverPoint != NULL )
				m_MyEndPoint = *m_pBGMouseOverPoint;
			else if (  m_pBGPoint != NULL )
				m_MyEndPoint = *m_pBGPoint;

			m_MyNowPoint.fChangeTime = 0.0f;

		}
		break;

	case RBCS_DISABLE:
		{
			if ( m_pDisabledPoint != NULL )
			{
				//m_MyEndPoint = *m_pBGPoint;
				m_MyEndPoint = *m_pDisabledPoint;
			}

			m_MyNowPoint.fChangeTime = 0.0f;

			if( m_bHasFocus == true )
				m_pDialog->ClearFocus();
		}
		break;
	}

	if( m_MyEndPoint.fChangeTime == 0.0f )	
	{
		m_MyNowPoint = m_MyEndPoint;
	}
	
}

void CKTDGUIRadioButton::SetNormalTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pNormalTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );
	m_pNormalTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pBGPoint != NULL )
		m_pBGPoint->pUITextureData = m_pNormalTex;
}

void CKTDGUIRadioButton::SetOverTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pMouseOverTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pMouseOverTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if ( m_pBGMouseOverPoint != NULL )
		m_pBGMouseOverPoint->pUITextureData = m_pMouseOverTex;
}

void CKTDGUIRadioButton::SetDisableTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pDisableTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pDisableTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pDisabledPoint != NULL )
		m_pDisabledPoint->pUITextureData = m_pDisableTex;
}

void CKTDGUIRadioButton::SetCheckedTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pCheckedTex );

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pCheckedTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pCheckedPoint != NULL )
		m_pCheckedPoint->pUITextureData = m_pCheckedTex;
}

#ifdef DLL_BUILD
void CKTDGUIRadioButton::SetNarmalTex( wstring fileName, wstring key )
{
	SAFE_DELETE( m_pNormalTex );

	m_pDisableTex = SetTexureData_( fileName.c_str(), key.c_str() );
	if( m_pDisabledPoint != NULL )
		m_pDisabledPoint->pUITextureData = m_pDisableTex;
}

void CKTDGUIRadioButton::SetOverTex( wstring fileName, wstring key )
{
	SAFE_DELETE( m_pMouseOverTex );

	m_pMouseOverTex = SetTexureData_( fileName.c_str(), key.c_str() );
	if ( m_pBGMouseOverPoint != NULL )
		m_pBGMouseOverPoint->pUITextureData = m_pMouseOverTex;
}

void CKTDGUIRadioButton::SetDisableTex( wstring fileName, wstring key )
{
	SAFE_DELETE( m_pDisableTex );

	m_pDisableTex = SetTexureData_( fileName.c_str(), key.c_str() );
	if( m_pDisabledPoint != NULL )
		m_pDisabledPoint->pUITextureData = m_pDisableTex;
}

void CKTDGUIRadioButton::SetCheckedTex( wstring fileName, wstring key )
{
	SAFE_DELETE( m_pCheckedTex );

	m_pCheckedTex = SetTexureData_( fileName.c_str(), key.c_str() );
	if( m_pCheckedPoint != NULL )
		m_pCheckedPoint->pUITextureData = m_pCheckedTex;
}
#endif

void CKTDGUIRadioButton::SetDisabledPoint_LUA()
{
	SAFE_DELETE( m_pDisabledPoint );
	m_pDisabledPoint = SetPointData_();
	m_pDisabledPoint->pUITextureData = m_pDisableTex;
	m_pDisabledPoint->SetAutoPointByTextureSize();
}


void CKTDGUIRadioButton::SetBGPoint_LUA()
{
	SAFE_DELETE( m_pBGPoint );
	m_pBGPoint = SetPointData_();
	m_pBGPoint->pUITextureData = m_pNormalTex;
	m_pBGPoint->SetAutoPointByTextureSize();
}

void CKTDGUIRadioButton::SetBGMouseOverPoint_LUA()
{
	SAFE_DELETE( m_pBGMouseOverPoint );
	m_pBGMouseOverPoint = SetPointData_();
	m_pBGMouseOverPoint->pUITextureData = m_pMouseOverTex;
	m_pBGMouseOverPoint->SetAutoPointByTextureSize();
}

void CKTDGUIRadioButton::SetRBPoint_LUA()
{
	SAFE_DELETE( m_pRadioButtonPoint );
	m_pRadioButtonPoint = SetPointData_();
	m_pRadioButtonPoint->pUITextureData = m_pNormalTex;
	m_pRadioButtonPoint->SetAutoPointByTextureSize();
}
		

void CKTDGUIRadioButton::SetCheckPoint_LUA()
{
	SAFE_DELETE( m_pCheckedPoint );
	m_pCheckedPoint = SetPointData_();
	m_pCheckedPoint->pUITextureData = m_pCheckedTex;
	m_pCheckedPoint->SetAutoPointByTextureSize();
}



void CKTDGUIRadioButton::SetDisabledPoint( UIPointData* pPointData )
{
	SAFE_DELETE( m_pDisabledPoint );
	m_pDisabledPoint = pPointData;
	m_pDisabledPoint->pUITextureData = m_pDisableTex;
	m_pDisabledPoint->SetAutoPointByTextureSize();
}


void CKTDGUIRadioButton::SetBGPoint( UIPointData* pPointData )
{
	SAFE_DELETE( m_pBGPoint );
	m_pBGPoint = pPointData;
	m_pBGPoint->pUITextureData = m_pNormalTex;
	m_pBGPoint->SetAutoPointByTextureSize();
}

void CKTDGUIRadioButton::SetBGMouseOverPoint( UIPointData* pPointData )
{
	SAFE_DELETE( m_pBGMouseOverPoint );
	m_pBGMouseOverPoint = pPointData;
	m_pBGMouseOverPoint->pUITextureData = m_pMouseOverTex;
	m_pBGMouseOverPoint->SetAutoPointByTextureSize();
}

void CKTDGUIRadioButton::SetRBPoint( UIPointData* pPointData )
{
	SAFE_DELETE( m_pRadioButtonPoint );
	m_pRadioButtonPoint = pPointData;
	m_pRadioButtonPoint->pUITextureData = m_pNormalTex;
	m_pRadioButtonPoint->SetAutoPointByTextureSize();

}

void CKTDGUIRadioButton::SetCheckPoint( UIPointData* pPointData )
{
	SAFE_DELETE( m_pCheckedPoint );
	m_pCheckedPoint = pPointData;
	m_pCheckedPoint->pUITextureData = m_pCheckedTex;
	m_pCheckedPoint->SetAutoPointByTextureSize();
}




CKTDGUIRadioButton* CKTDGUIRadioButton::GetCheckedControl()
{
	for ( int i = 0; ;i++ )
	{
		if ( m_pDialog->GetControl(i) == NULL  )
		{
			return NULL;
		}
		else
		{
			if ( m_pDialog->GetControl(i)->GetType() == CKTDGUIControl::UCT_RADIOBUTTON )
			{
				CKTDGUIRadioButton* temp = (CKTDGUIRadioButton*)(m_pDialog->GetControl(i));
				if ( temp->GetGroupID() == m_GroupID  )
				{
					if ( temp->GetChecked() == true )
					{
						return temp;
					}
				}
			}
		}
	}

	return NULL;
}

void CKTDGUIRadioButton::SetCheckedInternal( bool bChecked, bool bClickedDouble /*= false*/, bool bFromInput /*= false*/ )
{

	m_bChecked = bChecked;


	RadioButtonGroupInfo* pInfo = m_pDialog->GetRadioButtonGroupInfo( m_GroupID );
	if( NULL != pInfo && true == bChecked )
	{
		pInfo->SetChecked( this, bChecked );
	}


	if ( m_bChecked == false )
#ifdef	RADIOBUTTON_INIT_NORMAL
		ChangeState( RBCS_NORMAL );
#else //RADIOBUTTON_INIT_NORMAL
		ChangeState( RBCS_MOUSEOVER );
#endif //RADIOBUTTON_INIT_NORMAL

	if( true == bFromInput )
	{
		if( m_bChecked == true )
		{
			if ( bClickedDouble == true )
			{
				if( m_CustomMsgDoubleClicked != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgDoubleClicked, (LPARAM)this );
				else
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, RBEM_RADIOBUTTON_DOUBLE_CLICKED, (LPARAM)this );
			}
			else
			{
				if( m_CustomMsgChecked != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgChecked, (LPARAM)this );
				else
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, RBEM_RADIOBUTTON_CHECKED, (LPARAM)this );
			}
		}
		else
		{
			if( m_CustomMsgUnChecked != -1 )
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgUnChecked, (LPARAM)this );
			else
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, RBEM_RADIOBUTTON_UNCHECKED, (LPARAM)this );
		}
	}
}



void CKTDGUIRadioButton::SetGroupID_LUA( int iGroupID )
{
	m_GroupID = iGroupID;

	if( m_pDialog != NULL )
	{
		// 라디오 그룹 info가 없으면 dialog에 등록한다
		RadioButtonGroupInfo* pInfo = m_pDialog->GetRadioButtonGroupInfo( iGroupID );
		if( pInfo == NULL )
		{
			RadioButtonGroupInfo* pInfo = new RadioButtonGroupInfo( iGroupID );	
			m_pDialog->AddRadioButtonGroupInfo( pInfo );
		}
	}
}

// iMaxCheckCount : 라디오 그룹에서 최대로 선택할 수 있는 라디오 버튼 개수
void CKTDGUIRadioButton::SetGroupMaxCheckCount( int iMaxCheckCount )
{
	if( m_pDialog != NULL )
	{
		RadioButtonGroupInfo* pInfo = m_pDialog->GetRadioButtonGroupInfo( m_GroupID );
		if( pInfo != NULL )
		{
			pInfo->m_iMaxCheckCount = iMaxCheckCount;
		}
	}
}


D3DXVECTOR2 CKTDGUIRadioButton::GetPos()
{
	if( NULL != m_pRadioButtonPoint )
	{
		return m_pRadioButtonPoint->leftTopPoint + m_OffsetPos;
	}
	else
	{
		return D3DXVECTOR2(0,0);
	}
}

void CKTDGUIRadioButton::SetMaximize( bool val )
{
	CKTDGUIControl::SetMaximize( val );

	int iSign = 1;
	if ( val == false )
		iSign = -1;

	m_OffsetPos.x -= m_kMaximizeOffset.x * iSign;
	m_OffsetPos.y -= m_kMaximizeOffset.y * iSign;
}


void CKTDGUIRadioButton::SetCheckedEdge( bool bDrawOut, int _width, D3DXCOLOR _color )
{
	m_bDrawCheckedEdgeOut = bDrawOut;
	m_CheckedEdgeWidth = _width;
	m_CheckedEdgeColor = _color;
}


D3DXVECTOR2 CKTDGUIRadioButton::GetGuideDescPos()
{
	D3DXVECTOR2 _pos = D3DXVECTOR2(0,0);

	if ( m_vGuideDescAbsolutePos.x != 0 && m_vGuideDescAbsolutePos.y != 0 )
	{
		_pos = m_vGuideDescAbsolutePos;
	}
	else
	{
		if ( m_pBGMouseOverPoint != NULL )
		{
			_pos.x = ( m_pBGMouseOverPoint->leftTopPoint.x + m_pBGMouseOverPoint->rightTopPoint.x ) / 2.0f;
			_pos.y = m_pBGMouseOverPoint->leftTopPoint.y;
		}
		else
		{
			_pos = m_vGuideDescAbsolutePos;
		}
	}

	_pos += m_vGuideDescOffsetPos;

	return _pos;
}


void CKTDGUIRadioButton::DrawCheckedEdge( bool bDrawOut )
{
	if ( m_CheckedEdgeWidth == 0 || m_pCheckedEdgeTexture == NULL )
		return;

	D3DXCOLOR tempColor;

	tempColor.a = m_CheckedEdgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = m_CheckedEdgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = m_CheckedEdgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = m_CheckedEdgeColor.b * m_pDialog->GetColor().b * m_Color.b;


	int _width = (int)(m_MyNowPoint.rightBottomPoint.x - m_MyNowPoint.leftTopPoint.x);
	int _height = (int)(m_MyNowPoint.rightBottomPoint.y - m_MyNowPoint.leftTopPoint.y);

	if ( bDrawOut == true )
	{
		// 좌
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.leftTopPoint.x - m_CheckedEdgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftTopPoint.y - m_CheckedEdgeWidth), 
			m_CheckedEdgeWidth , 
			_height + m_CheckedEdgeWidth, 
			tempColor );

		// 하

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.leftTopPoint.x - m_CheckedEdgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftBottomPoint.y ), 
			_width + m_CheckedEdgeWidth, 
			m_CheckedEdgeWidth, 
			tempColor );

		// 우

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.rightTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftTopPoint.y ), 
			m_CheckedEdgeWidth, 
			_height + m_CheckedEdgeWidth, 
			tempColor );

		// 상

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftTopPoint.y - m_CheckedEdgeWidth ), 
			_width + m_CheckedEdgeWidth, 
			m_CheckedEdgeWidth, 
			tempColor );
	}
	else
	{
		// 좌
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftTopPoint.y ), 
			m_CheckedEdgeWidth , 
			_height, 
			tempColor );

		// 하

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.leftTopPoint.x), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftBottomPoint.y - m_CheckedEdgeWidth ), 
			_width, 
			m_CheckedEdgeWidth, 
			tempColor );

		// 우

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.rightTopPoint.x - m_CheckedEdgeWidth ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftTopPoint.y ), 
			m_CheckedEdgeWidth, 
			_height, 
			tempColor );

		// 상

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + m_MyNowPoint.leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + m_MyNowPoint.leftTopPoint.y ), 
			_width , 
			m_CheckedEdgeWidth, 
			tempColor );

	}
}

void CKTDGUIRadioButton::MoveControl( float fx, float fy )
{
	if( fx == 0.f && fy == 0.f )
		return;


	if( NULL != m_pBGPoint )
	{
		m_pBGPoint->Move( fx, fy );
		m_MyNowPoint = *m_pBGPoint;
		m_MyNowPoint = *m_pBGPoint;
	}

	if( NULL != m_pBGMouseOverPoint )
	{
		m_pBGMouseOverPoint->Move( fx, fy );
	}

	if( NULL != m_pCheckedPoint )
	{
		m_pCheckedPoint->Move( fx, fy );
	}

	if( NULL != m_pRadioButtonPoint )
	{
		m_pRadioButtonPoint->Move( fx, fy );
	}

	if( NULL != m_pDisabledPoint )
	{
		m_pDisabledPoint->Move( fx, fy );
	}
}

void CKTDGUIRadioButton::ScaleControl( float fx, float fy )
{
	if( fx == 1.f && fy == 1.f )
		return; 

	if( NULL != m_pBGPoint )
	{
		m_pBGPoint->Scale( fx, fy );
		m_pBGPoint->bUseTextureSize = false;
		m_MyNowPoint = *m_pBGPoint;
		m_MyNowPoint = *m_pBGPoint;
	}

	if( NULL != m_pBGMouseOverPoint )
	{
		m_pBGMouseOverPoint->Scale( fx, fy );
		m_pBGMouseOverPoint->bUseTextureSize = false;
	}

	if( NULL != m_pCheckedPoint )
	{
		m_pCheckedPoint->Scale( fx, fy );
		m_pCheckedPoint->bUseTextureSize = false;
	}

	if( NULL != m_pRadioButtonPoint )
	{
		m_pRadioButtonPoint->Scale( fx, fy );
		m_pRadioButtonPoint->bUseTextureSize = false;
	}

	if( NULL != m_pDisabledPoint )
	{
		m_pDisabledPoint->Scale( fx, fy );
		m_pDisabledPoint->bUseTextureSize = false;
	}
}

#ifdef DLL_BUILD
void CKTDGUIRadioButton::MoveSubControl( float fx, float fy, wstring subControlName )
{
	if( fx == 0.f && fy == 0.f )
		return;


	if( NULL != m_pBGPoint && subControlName == L"BG" )
	{
		m_pBGPoint->Move( fx, fy );
		m_MyNowPoint = *m_pBGPoint;
	}

	else if( NULL != m_pBGMouseOverPoint && subControlName == L"Over" )
	{
		m_pBGMouseOverPoint->Move( fx, fy );
		m_MyNowPoint = *m_pBGMouseOverPoint;
	}

	else if( NULL != m_pCheckedPoint && subControlName == L"CheckPoint" )
	{
		m_pCheckedPoint->Move( fx, fy );
		m_MyNowPoint = *m_pCheckedPoint;
	}

	else if( NULL != m_pRadioButtonPoint && subControlName == L"RB" )
	{
		m_pRadioButtonPoint->Move( fx, fy );
		m_MyNowPoint = *m_pRadioButtonPoint;
	}

	else if( NULL != m_pDisabledPoint && subControlName == L"Disable" )
	{
		m_pDisabledPoint->Move( fx, fy );
		m_MyNowPoint = *m_pDisabledPoint;
	}
}

void CKTDGUIRadioButton::SetEditGUI( bool bEdit )
{
	m_bUpdate = !bEdit;

	if( true == bEdit )
	{
		m_bChecked = false;		// 편집할때는 CheckPoint가 렌더링 되는 것을 막는다.		
	}
}

void CKTDGUIRadioButton::ShowSubView( wstring name, bool bView )
{
	SetColor(D3DXCOLOR(0xffffffff));		// 하위 컨트롤을 선택했을때 edge(테두리)를 보여주기 위해 색을 해제한다.

	if( true == bView )
	{
		if( name == L"BG" && NULL != m_pBGPoint )
			m_MyNowPoint = *m_pBGPoint;
		else if( name == L"Over" && NULL != m_pBGMouseOverPoint )
			m_MyNowPoint = *m_pBGMouseOverPoint;
		else if( name == L"RB" && NULL != m_pRadioButtonPoint )
			m_MyNowPoint = *m_pRadioButtonPoint;
		else if( name == L"CheckPoint" && NULL != m_pCheckedPoint )
			m_MyNowPoint = *m_pCheckedPoint;
		else if( name == L"Disable" && NULL != m_pDisabledPoint)
			m_MyNowPoint = *m_pDisabledPoint;
	}
	else
	{
		if( NULL != m_pBGPoint )
			m_MyNowPoint = *m_pBGPoint;
	}

	if( true == bView )
		m_bEditEdge = true;
	else
		m_bEditEdge = false;

}

vector<D3DXVECTOR2> CKTDGUIRadioButton::GetPosList()
{
	vector<D3DXVECTOR2> ret;	

	if( NULL != m_pBGPoint )
		ret.push_back(m_pBGPoint->leftTopPoint);

	if( NULL != m_pBGMouseOverPoint )
		ret.push_back(m_pBGMouseOverPoint->leftTopPoint);

	if( NULL != m_pCheckedPoint )
		ret.push_back(m_pCheckedPoint->leftTopPoint);

	if( NULL != m_pRadioButtonPoint )		
		ret.push_back(m_pRadioButtonPoint->leftTopPoint);

	if( NULL != m_pDisabledPoint )		
		ret.push_back(m_pDisabledPoint->leftTopPoint);

	return ret;
}

D3DXVECTOR2 CKTDGUIRadioButton::GetPos(wstring name)
{	
	if( name == L"BG" && NULL != m_pBGPoint )
		return m_pBGPoint->leftTopPoint;
	else if( name == L"Over" && NULL != m_pBGMouseOverPoint )
		return m_pBGMouseOverPoint->leftTopPoint;
	else if( name == L"RB" && NULL != m_pRadioButtonPoint )
		return m_pRadioButtonPoint->leftTopPoint;
	else if( name == L"CheckPoint" && NULL != m_pCheckedPoint )
		return m_pCheckedPoint->leftTopPoint;
	else if( name == L"Disable" && NULL != m_pDisabledPoint)
		return m_pDisabledPoint->leftTopPoint;
	///////////////////////////////
	if( name == L"RB_RIGHT_BOTTOM" && NULL != m_pRadioButtonPoint)
		return m_pRadioButtonPoint->rightBottomPoint;

	return D3DXVECTOR2(0,0);
}

void CKTDGUIRadioButton::DrawEditEdge( CKTDGUIControl::UIPointData*	m_pEditEdgePoint )
{
	if( false == m_bEditEdge )
		return;

	if(NULL == m_pEditEdgePoint) 
		return;

	if ( m_pCheckedEdgeTexture == NULL )
		return;


	const CKTDGUIControl::UIPointData & point = *m_pEditEdgePoint;
	D3DXCOLOR tempColor;

	int edgeWidth = 4;
	D3DXCOLOR edgeColor = D3DXCOLOR(0xffff0000);

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;


	int _width = (int)(point.rightBottomPoint.x - point.leftTopPoint.x);
	int _height = (int)(point.rightBottomPoint.y - point.leftTopPoint.y);

	//if ( m_bDrawEdgeOut == true )
	{
		// 좌
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftTopPoint.y - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftBottomPoint.y ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.rightTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftTopPoint.y ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상

		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + point.leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + point.leftTopPoint.y - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}	
}

CKTDGUIControl::UIPointData * CKTDGUIRadioButton::_GetPointData( wstring name )
{
	if( NULL !=  m_pBGPoint && name == L"Normal" )
		return m_pBGPoint;

	else if( NULL != m_pBGMouseOverPoint && name == L"Over" )
		return m_pBGMouseOverPoint;		

	else if( NULL != m_pCheckedPoint && name == L"Check" )
		return m_pCheckedPoint;

	else if( NULL != m_pDisabledPoint && name == L"Disable" )
		return m_pDisabledPoint;

	return NULL;
}

wstring CKTDGUIRadioButton::GetTextureName( wstring name )
{
	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );

	if( NULL != pPoint && NULL != pPoint->pUITextureData )
	{
		return pPoint->pUITextureData->texName;
	}

	return L"";
}

RECT CKTDGUIRadioButton::GetTextureUV( wstring name )
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

wstring CKTDGUIRadioButton::GetTextureKey( wstring name )
{
	CKTDGUIControl::UIPointData * pPoint = _GetPointData( name );

	if( NULL != pPoint && NULL != pPoint->pUITextureData )
	{
		return pPoint->pUITextureData->keyName;
	}

	return L"";
}

vector<wstring> CKTDGUIRadioButton::GetTextureKeyList( wstring name )
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

void CKTDGUIRadioButton::SetTexture( wstring name, wstring fileName )
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
		SetNarmalTex( fileName, key );		

	else if( name == L"Over" )
		SetOverTex( fileName, key );

	else if( name == L"Check" )
		SetCheckedTex( fileName, key );

	else if( name == L"Disable" )
		SetDisableTex( fileName, key );	
}

void CKTDGUIRadioButton::SetTextureKey( wstring name, wstring key )
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
	}
}

#endif

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
/* virtual */	void	CKTDGUIRadioButton::SetCustomMouseOverSound ( wstring wstrSoundFileName )
{
	 m_pSndMouseOver = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrSoundFileName );
}
/* virtual */	void	CKTDGUIRadioButton::SetCustomMouseUpSound  ( wstring wstrSoundFileName )
{
	 m_pSndCheck = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrSoundFileName );
}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
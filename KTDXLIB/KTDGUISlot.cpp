#include "StdAfx.h"
#include ".\ktdguislot.h"

#ifdef NEW_SKILL_TREE_UI

CKTDGUISlot*	CKTDGUISlot::s_pDragSlot = NULL;

/*static*/ void CKTDGUISlot::ReturnDraggingItem()
{
	if( NULL != CKTDGUISlot::s_pDragSlot )
	{
		CKTDGUISlot::s_pDragSlot->ChangeState( SCS_NORMAL, true );		
		CKTDGUISlot::s_pDragSlot = NULL;
	}
}


CKTDGUISlot::CKTDGUISlot() :
m_pItemTex(NULL),
m_pMouseOverTex(NULL),
m_pStringData(NULL),
m_pNormalPoint(NULL),
m_pMouseOverPoint(NULL),
m_pDraggingPoint(NULL),
//m_pOverPicture(NULL),
//m_pEtcPicture(NULL),
m_bDragable(true),
m_cDraggingSlotColor(1.0f, 1.0f, 1.0f, 1.0f),
m_cDraggedSlotColor(1.0f, 1.0f, 1.0f, 0.4f),
m_cNormalSlotColor(1.0f, 1.0f, 1.0f, 1.0f),
m_cMouseOverColor(1.0f, 1.0f, 1.0f, 1.0f),
m_pDroppedSlot(NULL),
m_eControlState(SCS_NORMAL),
m_pSndMouseOver(NULL),
m_pSndMouseUp(NULL),
m_pSndMouseDown(NULL),
m_CustomMsgMouseOut(-1),
m_CustomMsgMouseOver(-1),
m_CustomMsgMouseDown(-1),
m_CustomMsgMouseUp(-1),
m_CustomMsgMouseRightUp(-1),
m_CustomMsgDropped(-1),
m_bMouseOut(false),
m_pSlotData(NULL),
m_iID(-1),
m_pEdgeTexture(NULL)
{
	m_ControlType	= CKTDGUIControl::UCT_SLOT;
	m_eControlState	= SCS_NORMAL;
	m_pEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );

#ifdef DLL_BUILD
	m_bEditEdge = false;
#endif

/*	기본 사운드 설정 : 일단 주석처리. 필요할때만 사운드를 설정해주도록 하자.
	//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
	KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	string MouseOverSndFileName = "";
	string MouseUpSndFileName = "";

	LUA_GET_VALUE( kLuaManager, "Button_Mouse_Over", MouseOverSndFileName, "" );
	LUA_GET_VALUE( kLuaManager, "Button_Mouse_Up",	 MouseUpSndFileName, "" );

	wstring sndFileName = L"";

	ConvertUtf8ToWCHAR( sndFileName, MouseOverSndFileName.c_str() );
	m_pSndMouseOver = g_pKTDXApp->GetDeviceManager()->OpenSound( sndFileName );

	ConvertUtf8ToWCHAR( sndFileName, MouseUpSndFileName.c_str()  );
	m_pSndMouseUp = g_pKTDXApp->GetDeviceManager()->OpenSound( sndFileName );
*/

}

/*virtual*/ CKTDGUISlot::~CKTDGUISlot(void)
{
	if(CKTDGUISlot::s_pDragSlot == this)
	{
		s_pDragSlot = NULL;
	}

	RemoveAllPicture();

	SAFE_DELETE(m_pItemTex);				// 아이템 그림
	//SAFE_DELETE(m_pDraggingItemTex);		// 드래그 중인 아이템 그림
	SAFE_DELETE(m_pMouseOverTex);	

	SAFE_DELETE(m_pStringData);

	SAFE_DELETE(m_pNormalPoint);
	SAFE_DELETE(m_pMouseOverPoint);
	SAFE_DELETE(m_pDraggingPoint);

	SAFE_CLOSE(m_pSndMouseOver);
	SAFE_CLOSE(m_pSndMouseUp);
	SAFE_CLOSE(m_pSndMouseDown);

	SAFE_CLOSE( m_pEdgeTexture );

	SAFE_DELETE(m_pSlotData);
}

bool CKTDGUISlot::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return S_OK;
#endif
	
	switch( uMsg )
	{
	//case WM_RBUTTONDOWN:
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
				
				if(m_eControlState != SCS_DISABLE)
				{
					if(m_bDragable)
					{
						ChangeState( SCS_ONDRAG );
					}
					else
					{
						ChangeState( SCS_DOWN );
					}

					if( m_CustomMsgMouseDown != -1 )
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseDown, (LPARAM)this );
					}

				}
				
				return true;
			}

		}
		break;

	case WM_LBUTTONUP:
		{
			// Slot Drop or Non-Dragable Slot Click
			if( ContainsPoint( pt ) == true )
			{
				LButtonMouseUp();
			}
			return true;

		}
		break;

	case WM_RBUTTONDOWN:
		{
			// Button click
			
			if( m_bHasFocus == false )
			{	
				if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDialog ) == true )
					m_pDialog->RequestFocus( this );
			}
			
			if( ContainsPoint( pt ) == true && m_bHasFocus == true )
			{	
				if( m_eControlState == SCS_ONDRAG )
				{
					CKTDGUISlot::ReturnDraggingItem();
				}				

				ChangeState( SCS_DOWN );
				PlaySound( m_pSndMouseDown );
			}
			return true;
		} break;

	case WM_RBUTTONUP:
		{
			// Button click
			if( ContainsPoint( pt ) == true && m_bHasFocus == true )
			{
				if( m_CustomMsgMouseRightUp != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseRightUp, (LPARAM)this );
				
				ChangeState( SCS_MOUSEOVER );
				PlaySound( m_pSndMouseUp );
			}
			return true;

		}
		break;

	};

	return false;
}

void CKTDGUISlot::LButtonMouseUp()
{
	switch( m_eControlState )
	{
	case SCS_NORMAL:
	case SCS_MOUSEOVER:
		{
			if( CKTDGUISlot::s_pDragSlot != NULL )
			{
				// 뭔가 드롭되었다
				if( SCS_ONDRAG == CKTDGUISlot::s_pDragSlot->GetState() )
				{
					m_pDroppedSlot = CKTDGUISlot::s_pDragSlot;
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgDropped, (LPARAM)this );
					CKTDGUISlot::ReturnDraggingItem();
				}
				else
				{
					ASSERT( !"Non-Dragging Slot is Not Cleared" );
				}
			}
		} break;
	
	case SCS_ONDRAG:
		{
			if( CKTDGUISlot::s_pDragSlot != NULL &&
				CKTDGUISlot::s_pDragSlot == this )
			{
				// 자기 자신에게 떨어졌다
				CKTDGUISlot::ReturnDraggingItem();
			}
			else
			{
				ASSERT(!"2 or more Dragging State Slot exists!");
			}
		} break;
	case SCS_DOWN:
 		{
 			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseUp, (LPARAM)this );
			ChangeState(SCS_NORMAL);
 		}
	case SCS_DISABLE:
	case SCS_END:
	default:
		{
			// 뭔가 슬롯 위에 떨어지긴 했는데, 처리할 필요가 없는 상황
			CKTDGUISlot::ReturnDraggingItem();
		}
		break;

	}

	PlaySound( m_pSndMouseUp );
}


/*virtual*/	HRESULT	CKTDGUISlot::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return S_OK;
#endif

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );
	
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

	// Data가 바뀌었다면 거기에 맞게 UI를 업데이트 시키자.
	if( m_pSlotData != NULL && m_pSlotData->m_bDataUpdated )
	{
		m_pSlotData->UpdateUI( this );
		m_pSlotData->m_bDataUpdated = false;
	}

	switch( m_eControlState )
	{
	case SCS_NORMAL:
		{
			if( m_bMouseOver == true )
			{
				ChangeState( SCS_MOUSEOVER );
				PlaySound( m_pSndMouseOver );
			}
			else if( m_bEnable == false )
				ChangeState( SCS_DISABLE );
		}
		break;
	case SCS_DOWN:
		{
			if( m_bMouseOver == false )
				ChangeState( SCS_NORMAL );
			else if( m_bEnable == false )
				ChangeState( SCS_DISABLE );
			
		} break;

	case SCS_MOUSEOVER:
		{		
			ShowGuideDesc();
			if( m_bMouseOver == false )
			{
				m_bMouseOut = true;
				ChangeState( SCS_NORMAL );
			}
			else if( m_bEnable == false )
				ChangeState( SCS_DISABLE );
		}
		break;

	case SCS_ONDRAG:
		{
			if( m_bEnable == false )
				ChangeState( SCS_DISABLE );

			if( !m_bHasFocus )
			{
				CKTDGUISlot::ReturnDraggingItem();
				ChangeState( SCS_NORMAL );
			}
			
		}
		break;

	case SCS_DISABLE:
		{
#ifdef SHOW_DESCRIPTION_WHEN_DISABLE
			if( m_bShowDescriptionWhenDisable == true && m_bMouseOver == true )
			{
				ShowGuideDesc();
			}
#endif SHOW_DESCRIPTION_WHEN_DISABLE

			if( m_bEnable == true )
				ChangeState( SCS_NORMAL );
		}
		break;
	}

	for( map<int, CKTDGUIControl::CPictureData*>::iterator it = m_mapAdditionalPictureDataList.begin(); it != m_mapAdditionalPictureDataList.end(); ++it )
	{		
		CKTDGUIControl::CPictureData* pPictureData = it->second;
		if( NULL != pPictureData && true == pPictureData->GetShow() )
		{
			pPictureData->OnFrameMove( fTime, fElapsedTime );
		}
	}


	return S_OK;

}
/*virtual*/ HRESULT CKTDGUISlot::OnFrameRender()
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	DrawEditEdge();
#endif

	UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
		m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
		m_NowPoint );

	RenderVertex( m_Vertex, m_NowPoint );

	if(	m_bMouseOver && 
		m_eControlState != SCS_ONDRAG &&
		true == m_bEnable )
	{
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			*m_pMouseOverPoint );

		RenderVertex( m_Vertex, *m_pMouseOverPoint );
	}

	for( map<int, CKTDGUIControl::CPictureData*>::iterator it = m_mapAdditionalPictureDataList.begin(); it != m_mapAdditionalPictureDataList.end(); ++it )
	{
		KTDXPROFILE_BEGIN("Picture");
		CKTDGUIControl::CPictureData* pPictureData = it->second;

		if ( pPictureData->GetShow() == true 
			&& pPictureData->pPoint != NULL 
			&& pPictureData->pTexture != NULL 
			&& pPictureData->pTexture->pTexture != NULL )
		{
			if ( pPictureData->m_EdgeWidth != 0 )
				DrawEdge( pPictureData->m_bDrawEdgeOut, pPictureData->m_EdgeWidth, pPictureData->m_EdgeColor, pPictureData->pPoint );

			UpdateVertex( m_Vertex[VP_LEFT_TOP], m_Vertex[VP_RIGHT_TOP],
				m_Vertex[VP_LEFT_BOTTOM], m_Vertex[VP_RIGHT_BOTTOM],
				*pPictureData->pPoint );


			KD3DBEGIN()
				CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,			pPictureData->pPoint->m_eSrcBlend );
			CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND,		pPictureData->pPoint->m_eDestBlend );
			RenderVertex( m_Vertex, *pPictureData->pPoint, pPictureData->pPoint->m_iDrawCount );
			KD3DEND()

				if ( pPictureData->m_EdgeWidth != 0 )
					DrawEdge( pPictureData->m_bDrawEdgeOut, pPictureData->m_EdgeWidth, pPictureData->m_EdgeColor, pPictureData->pPoint );
		}
		KTDXPROFILE_END();
	}
	

//	UpdateAnimVertex( m_Vertex, &m_NowTexChangeData );

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


HRESULT	CKTDGUISlot::RenderDragSlot()
{
	KD3DBEGIN()

	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR   );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR   );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR   );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR   );

	CKTDGStateManager::PushStates( g_pKTDXApp->GetDGManager()->GetRSICKTDGUIDialog() );

	if(m_eControlState == SCS_ONDRAG &&	m_bHasFocus == true )
	{	
		if(m_pNormalPoint != NULL && m_pNormalPoint->pUITextureData != NULL )
		{
			D3DXVECTOR2 vMousePos;
			vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
			vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
			vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

			float halfwidth = (m_pDraggingPoint->rightTopPoint.x - m_pDraggingPoint->leftTopPoint.x) * 0.5f ;
			float halfheight = ( m_pDraggingPoint->leftBottomPoint.y - m_pDraggingPoint->leftTopPoint.y) * 0.5f;

			m_pDraggingPoint->leftTopPoint = vMousePos + D3DXVECTOR2(-halfwidth, -halfheight);
			m_pDraggingPoint->rightTopPoint = vMousePos + D3DXVECTOR2(halfwidth, -halfheight);
			m_pDraggingPoint->leftBottomPoint = vMousePos + D3DXVECTOR2(-halfwidth, halfheight);
			m_pDraggingPoint->rightBottomPoint = vMousePos + D3DXVECTOR2(halfwidth, halfheight);

			UpdateDraggingVertex( m_DragVertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_DragVertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_DragVertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_DragVertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				*m_pDraggingPoint );

			RenderVertex( m_DragVertex, *m_pDraggingPoint );
		}
	}

	KD3DEND()

	return S_OK;

}




/*virtual*/	HRESULT	CKTDGUISlot::OnFrameRenderEnd()
{
	return S_OK;
}


/*virtual*/ void CKTDGUISlot::SetEnable( bool bEnable )
{
	m_bEnable = bEnable;
	
	if(m_bEnable)
	{
		m_eControlState = SCS_NORMAL;
	}
	else 
	{
		m_eControlState = SCS_DISABLE;
		
		if(m_eControlState == SCS_ONDRAG)
		{
			CKTDGUISlot::ReturnDraggingItem();
		}
	}
	ChangeState( m_eControlState );
}

/*virtual*/ void CKTDGUISlot::SetShow( bool bShow )
{
	m_bShow = bShow;
	if(m_eControlState == SCS_ONDRAG && !m_bShow)
	{
		CKTDGUISlot::ReturnDraggingItem();
	}
	ChangeState( m_eControlState );
}

/*virtual*/ void CKTDGUISlot::OnFocusOut()
{
	CKTDGUIControl::OnFocusOut();

	if( m_eControlState == SCS_ONDRAG )
	{
		CKTDGUISlot::ReturnDraggingItem();
	}
	if(m_bEnable)
	{
		m_eControlState = SCS_NORMAL;
	}
	else
	{
		m_eControlState = SCS_DISABLE;
	}
	ChangeState( m_eControlState );
		
}

/*virtual*/ void CKTDGUISlot::ChangeState( SLOT_CONTROL_STATE state, bool bForce /*= false*/ )
{
	{	
		if( m_eControlState == state && bForce == false )
			return;

		if ( m_eControlState != state && bForce == false )
		{
			m_NowTexChangeData.m_SequenceID = 0;
			m_NowTexChangeData.m_fShowTime = 0.0f;
			m_NowTexChangeData.m_bEndTexChange = false;
		}

		m_eControlState = state;

		if ( bForce == false )
		{
			m_NowTexChangeData.m_fRotateDegree = 0.0f;
			m_NowTexChangeData.m_fScaleRotationDegree = 0.0f;
			m_NowTexChangeData.m_Pos = D3DXVECTOR2(0,0);
			m_NowTexChangeData.m_Scale = D3DXVECTOR2(1,1);
		}

		switch( m_eControlState )
		{
		case SCS_NORMAL:
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
				m_Color = m_cNormalSlotColor;
			}
			break;

		case SCS_MOUSEOVER:
			{

				m_Color = m_cNormalSlotColor;
				m_pMouseOverPoint->color = m_cMouseOverColor;

				if( m_CustomMsgMouseOver != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseOver, (LPARAM)this );
				
				if( m_pNormalPoint != NULL )
				{
					m_EndPoint = *m_pNormalPoint;
					m_NowPoint.fChangeTime = 0.0f;
				}

			}
			break;

		case SCS_DOWN:
			{
				m_Color = m_cNormalSlotColor;
// 				if( m_CustomMsgMouseDown != -1 )
// 					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseDown, (LPARAM)this );
				if( m_pNormalPoint != NULL )
				{
					m_EndPoint = *m_pNormalPoint;
					m_NowPoint.fChangeTime = 0.0f;
				}
			}
			break;

		case SCS_ONDRAG:
			{
				if(m_bHasFocus == false)
				{
					RequestFocus();
				}
				CKTDGUISlot::s_pDragSlot = this;

				if( m_pNormalPoint != NULL )
				{
					m_EndPoint = *m_pNormalPoint;
					m_NowPoint.fChangeTime = 0.0f;
				}
				if( m_pNormalPoint != NULL )
				{
					m_EndPoint = *m_pNormalPoint;
					m_NowPoint.fChangeTime = 0.0f;
				}
				m_Color = m_cDraggedSlotColor;

			}
			break;

		case SCS_DISABLE:
			{

				if( m_bHasFocus == true )
				{
					if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDialog ) == true )
						m_pDialog->ClearFocus();
				}
				m_Color = m_cNormalSlotColor;
			}
			break;
		}
	}

	if( m_EndPoint.fChangeTime == 0.0f )
	{
		m_NowPoint = m_EndPoint;
	}

}

bool CKTDGUISlot::ContainsPoint( POINT pt )
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

void CKTDGUISlot::SetItemTex( const WCHAR* pFileName )
{
	KTDXPROFILE();

	if( NULL == pFileName )
		return;

//#ifdef EFFICIENT_SET_TEX_FOR_UI
//	if( NULL != m_pItemTex )
//	{
//		if( 0 == m_pItemTex->texName.compare( pFileName) )
//		{
//			return;
//		}
//	}
//#endif EFFICIENT_SET_TEX_FOR_UI


	UITextureData* pTexData = new UITextureData();
	pTexData->texName	= pFileName;
	pTexData->pTexture	= g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexData->texName );
	if( pTexData->pTexture == NULL )
	{
		SAFE_DELETE( pTexData );
		if( 0 != wcscmp( pFileName, L"HQ_Shop_Ui_Noimage.dds" ) )		// 재귀호출을 막기 위해서
		{
			SetItemTex( L"HQ_Shop_Ui_Noimage.dds" );
		}
		return;
	}

	float fWidth	= (float)pTexData->pTexture->GetWidth();
	float fHeight	= (float)pTexData->pTexture->GetHeight();

	pTexData->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= D3DXVECTOR2(0,0);
	pTexData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= D3DXVECTOR2(fWidth,0);
	pTexData->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= D3DXVECTOR2(0,fHeight);
	pTexData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= D3DXVECTOR2(fWidth, fHeight);

	pTexData->texSize = D3DXVECTOR2( fWidth, fHeight );
	pTexData->SetTextureUV();

	SAFE_DELETE( m_pItemTex );
	m_pItemTex = pTexData;
	
	if( m_pNormalPoint != NULL )
		m_pNormalPoint->pUITextureData = m_pItemTex;

	if( m_pDraggingPoint != NULL )
		m_pDraggingPoint->pUITextureData = m_pItemTex;

	ChangeState( m_eControlState, true );

}

void CKTDGUISlot::SetItemTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pItemTex );

	m_pItemTex = SetTexureData_( pFileName, key );
	if(m_pItemTex == NULL )
	{
		// 실패하면 noimage를 발라버리자
		SetItemTex( L"HQ_Shop_Ui_Noimage.dds" );
		return;
	}

	if( m_pNormalPoint != NULL )
		m_pNormalPoint->pUITextureData = m_pItemTex;

	if( m_pDraggingPoint != NULL )
		m_pDraggingPoint->pUITextureData = m_pItemTex;

	ChangeState( m_eControlState, true );

// 	if( m_pItemTex != NULL )
// 		return true;
// 	else
// 		return false;

}

void CKTDGUISlot::SetItemTex_LUA( const char* pFileName, const char* key )
{
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	SetItemTex( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUISlot::SetItemTex_LUA2( const char* pFileName )
{
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	SetItemTex( wstrFileName.c_str() );
}


void CKTDGUISlot::SetMouseOverTex( const WCHAR* pFileName, const WCHAR* key )
{
	SAFE_DELETE( m_pMouseOverTex );

	m_pMouseOverTex = SetTexureData_( pFileName, key );
	if( m_pMouseOverPoint != NULL )
		m_pMouseOverPoint->pUITextureData = m_pMouseOverTex;

	ChangeState( m_eControlState, true );

// 	if( m_pMouseOverTex != NULL )
// 		return true;
// 	else
// 		return false;

}

void CKTDGUISlot::SetMouseOverTex_LUA( const char* pFileName, const char* key )
{
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	SetMouseOverTex( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUISlot::SetString_LUA()
{
	CKTDGUIControl::UIStringData* pString = SetStringData_();
	SetString( pString );
}

void CKTDGUISlot::SetPoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pNormalPoint );
	m_pNormalPoint = pUIPointData;
	m_pNormalPoint->pUITextureData = m_pItemTex;
	m_pNormalPoint->SetAutoPointByTextureSize();
	
	SetAllPointDatabyNormalPoint();

	m_NowPoint = *m_pNormalPoint;
	
	ChangeState( m_eControlState, true );
}

void CKTDGUISlot::SetPoint_LUA()
{
	SAFE_DELETE( m_pNormalPoint );
	m_pNormalPoint = SetPointData_();
	m_pNormalPoint->pUITextureData = m_pItemTex;

	m_pNormalPoint->SetAutoPointByTextureSize();

	SetAllPointDatabyNormalPoint();


	m_NowPoint = *m_pNormalPoint;

	ChangeState( m_eControlState, true );
}

void CKTDGUISlot::SetMouseOverPoint( CKTDGUIControl::UIPointData* pUIPointData )
{
	SAFE_DELETE( m_pMouseOverPoint );
	m_pMouseOverPoint = pUIPointData;
	m_pMouseOverPoint->pUITextureData = m_pMouseOverTex;
	m_pMouseOverPoint->SetAutoPointByTextureSize();
}

void CKTDGUISlot::SetMouseOverPoint_LUA()
{
	SAFE_DELETE( m_pMouseOverPoint );
	m_pMouseOverPoint = SetPointData_();
	m_pMouseOverPoint->pUITextureData = m_pMouseOverTex;

	m_pMouseOverPoint->SetAutoPointByTextureSize();

}

void CKTDGUISlot::SetAllPointDatabyNormalPoint()
{
	if(m_pDraggingPoint == NULL)
	{
		m_pDraggingPoint = new CKTDGUIControl::UIPointData(*m_pNormalPoint);
		m_pDraggingPoint->color = m_cDraggingSlotColor;
		m_pDraggingPoint->pUITextureData = m_pItemTex;
	}
	
	if(m_pMouseOverPoint == NULL)
	{
		m_pMouseOverPoint = new CKTDGUIControl::UIPointData(*m_pNormalPoint);
		m_pMouseOverPoint->pUITextureData = m_pMouseOverTex;
		m_pDraggingPoint->color = m_cMouseOverColor;
	}	

}

void CKTDGUISlot::UpdateDraggingVertex( VERTEX_UI& vertexLT, VERTEX_UI& vertexRT, VERTEX_UI& vertexLB, VERTEX_UI& vertexRB, 
									   const UIPointData& pointData  )
{
	if( m_pDialog == NULL )
	{
		return;
	}//if

	KTDXPROFILE();
	//0---1
	//2---3
	//Set up the 4 corners of a small square
	vertexLT.x		= pointData.leftTopPoint.x * g_pKTDXApp->GetResolutionScaleX();
	vertexLT.y		= pointData.leftTopPoint.y * g_pKTDXApp->GetResolutionScaleY();
	vertexLT.z		= 1.0f;   
	vertexLT.rhw	= 1.0f;
	vertexLT.color	= pointData.color;


	vertexRT.x		= pointData.rightTopPoint.x * g_pKTDXApp->GetResolutionScaleX();
	vertexRT.y		= pointData.rightTopPoint.y * g_pKTDXApp->GetResolutionScaleY();
	vertexRT.z		= 1.0f;   
	vertexRT.rhw	= 1.0f;
	vertexRT.color	= pointData.color;


	vertexLB.x		= pointData.leftBottomPoint.x * g_pKTDXApp->GetResolutionScaleX();
	vertexLB.y		= pointData.leftBottomPoint.y * g_pKTDXApp->GetResolutionScaleY();
	vertexLB.z		= 1.0f;   
	vertexLB.rhw	= 1.0f;
	vertexLB.color	= pointData.color;


	vertexRB.x		= pointData.rightBottomPoint.x * g_pKTDXApp->GetResolutionScaleX();
	vertexRB.y		= pointData.rightBottomPoint.y * g_pKTDXApp->GetResolutionScaleY();
	vertexRB.z		= 1.0f;   
	vertexRB.rhw	= 1.0f;
	vertexRB.color	= pointData.color;




	if( pointData.pUITextureData != NULL )
	{
		vertexLT.u	= pointData.pUITextureData->uvTexture[VP_LEFT_TOP].x;   
		vertexLT.v	= pointData.pUITextureData->uvTexture[VP_LEFT_TOP].y;

		vertexRT.u	= pointData.pUITextureData->uvTexture[VP_RIGHT_TOP].x;   
		vertexRT.v	= pointData.pUITextureData->uvTexture[VP_RIGHT_TOP].y;

		vertexLB.u	= pointData.pUITextureData->uvTexture[VP_LEFT_BOTTOM].x;   
		vertexLB.v	= pointData.pUITextureData->uvTexture[VP_LEFT_BOTTOM].y;

		vertexRB.u	= pointData.pUITextureData->uvTexture[VP_RIGHT_BOTTOM].x;   
		vertexRB.v	= pointData.pUITextureData->uvTexture[VP_RIGHT_BOTTOM].y;
	}





	vertexLT.x	= (int)vertexLT.x - 0.5f;     
	vertexLT.y	= (int)vertexLT.y - 0.5f;

	vertexRT.x	= (int)vertexRT.x - 0.5f;     
	vertexRT.y	= (int)vertexRT.y - 0.5f;

	vertexLB.x	= (int)vertexLB.x - 0.5f;     
	vertexLB.y	= (int)vertexLB.y - 0.5f;

	vertexRB.x	= (int)vertexRB.x - 0.5f;     
	vertexRB.y	= (int)vertexRB.y - 0.5f;

}

void CKTDGUISlot::SetSlotData( CKTDGUISlotData* pSlotData )
{
	m_pSlotData = pSlotData; 
	m_pSlotData->m_bDataUpdated = true;
}


void CKTDGUISlot::SwapSlotData( CKTDGUISlot* pSlot )
{
	CKTDGUISlotData* pTempSlotData;
	pTempSlotData = pSlot->GetSlotData();
	pSlot->SetSlotData( m_pSlotData );
	SetSlotData( pTempSlotData );
}

CKTDGUIControl::CPictureData* CKTDGUISlot::GetPicture( int index )
{
	if(m_mapAdditionalPictureDataList.empty() == true)
		return NULL;

	map<int, CKTDGUIControl::CPictureData*>::iterator it = m_mapAdditionalPictureDataList.find(index);
	if(it != m_mapAdditionalPictureDataList.end())
	{
		CKTDGUIControl::CPictureData* pPictureData = it->second;
		return pPictureData;
	}	
	
	return NULL;

}

void CKTDGUISlot::RemoveAllPicture()
{
	for( map<int, CKTDGUIControl::CPictureData*>::iterator it = m_mapAdditionalPictureDataList.begin(); it != m_mapAdditionalPictureDataList.end(); ++it )
	{		
		CKTDGUIControl::CPictureData* pPictureData = it->second;
		SAFE_DELETE( pPictureData );
	}
	m_mapAdditionalPictureDataList.clear();
}

void CKTDGUISlot::DrawEdge( bool bDrawOut, int edgeWidth, D3DXCOLOR edgeColor, CKTDGUIControl::UIPointData* pPoint )
{
	if ( edgeWidth == 0 || m_pEdgeTexture == NULL )
		return;

	if ( pPoint == NULL )
		return;

	int _width = (int)(pPoint->rightTopPoint.x - pPoint->leftTopPoint.x);
	int _height = (int)(pPoint->rightBottomPoint.y - pPoint->leftTopPoint.y);

	D3DXCOLOR tempColor;

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;

	if ( bDrawOut == true )
	{
		// 좌
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftBottomPoint.y ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->rightTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}
	else
	{
		// 좌
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			edgeWidth , 
			_height, 
			tempColor );

		// 하

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftBottomPoint.y - edgeWidth ), 
			_width, 
			edgeWidth, 
			tempColor );

		// 우

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->rightTopPoint.x - edgeWidth ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			edgeWidth, 
			_height, 
			tempColor );

		// 상

		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + pPoint->leftTopPoint.x ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + pPoint->leftTopPoint.y ), 
			_width , 
			edgeWidth, 
			tempColor );

	}

}


D3DXVECTOR2 CKTDGUISlot::GetGuideDescPos()
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

D3DXVECTOR2 CKTDGUISlot::GetSize()
{
	if( m_pNormalPoint != NULL )
        return D3DXVECTOR2( m_pNormalPoint->rightTopPoint.x - m_pNormalPoint->leftTopPoint.x, m_pNormalPoint->leftBottomPoint.y - m_pNormalPoint->leftTopPoint.y );
	else
		return D3DXVECTOR2(0, 0);

}

D3DXVECTOR2 CKTDGUISlot::GetPos()
{
	if( m_pNormalPoint != NULL )
		return m_pNormalPoint->leftTopPoint;
	else
		return D3DXVECTOR2(0, 0);
}

#ifdef DLL_BUILD
void CKTDGUISlot::MoveControl( float fx, float fy )
{
	if( m_pNormalPoint != NULL )
		m_pNormalPoint->Move(fx, fy);

	if( m_pMouseOverPoint != NULL )
		m_pMouseOverPoint->Move(fx, fy);

	m_NowPoint = *m_pNormalPoint;
}

void CKTDGUISlot::MoveSubControl( float fx, float fy, wstring subControlName )
{
	if( subControlName == L"Normal" && m_pNormalPoint != NULL )
	{
		m_pNormalPoint->Move(fx, fy);
		m_NowPoint = *m_pNormalPoint;
	}

	else if( subControlName == L"Over" && m_pMouseOverPoint != NULL )
	{
		m_pMouseOverPoint->Move(fx, fy);
		m_NowPoint = *m_pMouseOverPoint;
	}
}

void CKTDGUISlot::SetEditGUI( bool bEdit )
{
	SetColor( D3DXCOLOR(0xffffffff) );

	m_bUpdate = !bEdit;
	m_bEditEdge = bEdit;

	m_NowPoint = *m_pNormalPoint;
}

void CKTDGUISlot::ShowSubView( wstring name, bool bView )
{
	if( name == L"Normal" && m_pNormalPoint != NULL )
		 m_NowPoint = *m_pNormalPoint;

	else if( name == L"Over" && m_pMouseOverPoint != NULL )
		 m_NowPoint = *m_pMouseOverPoint;
}

vector<D3DXVECTOR2> CKTDGUISlot::GetPosList()
{
	vector<D3DXVECTOR2> ret;

	ret.push_back( GetPos() );

	if( m_pMouseOverPoint != NULL )
		ret.push_back( m_pMouseOverPoint->leftTopPoint );

	return ret;
}

D3DXVECTOR2 CKTDGUISlot::GetPos(wstring name)
{
	if( name == L"Normal" )
		return GetPos();

	else if( name == L"Over" &&  NULL != m_pMouseOverPoint )
		return m_pMouseOverPoint->leftTopPoint;

	return D3DXVECTOR2(0, 0);
}

void CKTDGUISlot::DrawEditEdge()
{
	if( false == m_bEditEdge )
		return;	

	if ( m_pEdgeTexture == NULL )
		return;	

	//const CKTDGUIControl::UIPointData & point = *m_pEditEdgePoint;
	D3DXCOLOR tempColor;

	int edgeWidth = 2;
	D3DXCOLOR edgeColor = D3DXCOLOR(0xffff0000);

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;

	RECT edgeRect;
	edgeRect.left = (int)m_NowPoint.leftTopPoint.x;
	edgeRect.top = (int)m_NowPoint.leftTopPoint.y;
	edgeRect.right = (int)m_NowPoint.rightBottomPoint.x;
	edgeRect.bottom = (int)m_NowPoint.rightBottomPoint.y;

	int _width = (int)(edgeRect.right - edgeRect.left);
	int _height = (int)(edgeRect.bottom - edgeRect.top);

	//if ( m_bDrawEdgeOut == true )
	{
		// 좌 left/top
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하left/bottom
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.bottom ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우right/top
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.right ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상left/top
		m_pEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}
}

#endif


#endif // NEW_SKILL_TREE_UI

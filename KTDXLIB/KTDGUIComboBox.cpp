#include "StdAfx.h"
#include ".\ktdguicombobox.h"

inline int RectWidth( RECT &rc ) { return ( (rc).right - (rc).left ); }
inline int RectHeight( RECT &rc ) { return ( (rc).bottom - (rc).top ); }
inline RECT AddRect( RECT &rc1, RECT &rc2 )
{
	RECT rc;
	rc.left = min(rc1.left, rc2.left);
	rc.right = max(rc1.right, rc2.right);
	rc.top = min(rc1.top, rc2.top);
	rc.bottom = max(rc1.bottom, rc2.bottom);

	return rc;
}

CKTDGUIComboBox::CKTDGUIComboBox()
: m_rcBoundingBox( RECT() )
, m_rcBoundingBox2( RECT() )
, m_rcButton( RECT() )
, m_rcDropdownText( RECT() )
, m_rcText( RECT() )
{
	m_ControlType = UCT_COMBOBOX;

	m_pScrollBar = NULL;

	m_pTextTex = NULL;
	m_pButtonTex = NULL;
	m_pDropdownTex = NULL;
	m_pDropdownTextTex = NULL;

	m_pButtonMouseDownTex = NULL;
	m_pButtonMouseOverTex = NULL;

	m_pStringData = NULL;

	m_nDropHeight = 100;
	m_nStaticHeight = 0;
	m_bIsStaticComboBox = false;

	m_nSBWidth = 16;
	m_bOpened = false;
	m_iSelected = -1;
	m_iFocused = -1;
	m_bPressed = false;
	m_bMouseOverCheck = false;

	m_ButtonOffsetX = 0;
	m_ButtonOffsetY = 0;

	m_ButtonAddWidth = 0;
	m_ButtonAddHeight = 0;

	m_CustomMsgSelectionChanged = -1;
	m_CustomMsgOpenDropdown	= -1;
	m_CustomMsgCloseDropdown = -1;

	m_pScollBarUpButtonTex = NULL;
	m_pScollBarUpButtonDownTex = NULL;
	m_pScollBarDownButtonTex = NULL;
	m_pScollBarDownButtonDownTex = NULL;
	m_pScollBarTrackTex = NULL;
	m_pScollBarThumbTex = NULL;

	m_bMousePointAdjust = false;

	m_x = m_y = 0;
	m_width = m_height = 0;

	m_TextX = m_TextY = 0;
	m_TextWidth = m_TextHeight = 0;

	m_ButtonX = m_ButtonY = 0;
	m_ButtonWidth = m_ButtonHeight = 0;

	m_TextStringOffsetPosX = 5;
	m_TextStringOffsetPosY = 5;

	m_bIsReverseDropDown = false;

	m_pSndButtonMouseDown = NULL;
	m_pSndButtonMouseOver = NULL;

    m_bSelectTextShow = true;
	m_iTextLimit = 0;

//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring checkSndFileName;

	LUA_GET_VALUE( kLuaManager, "ComboBox_Mouse_Down", checkSndFileName, L"" );
	m_pSndButtonMouseDown = g_pKTDXApp->GetDeviceManager()->OpenSound( checkSndFileName );

	LUA_GET_VALUE( kLuaManager, "ComboBox_Mouse_Over", checkSndFileName, L"" );
	m_pSndButtonMouseOver = g_pKTDXApp->GetDeviceManager()->OpenSound( checkSndFileName );

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;	
	m_colorEdge = D3DXCOLOR(0xffffffff);
#endif
}


CKTDGUIComboBox::~CKTDGUIComboBox()
{
	RemoveAllItems();

	SAFE_DELETE( m_pTextTex );
	SAFE_DELETE( m_pButtonTex );
	SAFE_DELETE( m_pButtonMouseDownTex );
	SAFE_DELETE( m_pButtonMouseOverTex );
	SAFE_DELETE( m_pDropdownTex );
	SAFE_DELETE( m_pDropdownTextTex );
	SAFE_DELETE( m_pStringData );

	SAFE_DELETE( m_pScrollBar );

	SAFE_DELETE( m_pScollBarDownButtonTex );
	SAFE_DELETE( m_pScollBarDownButtonDownTex );
	SAFE_DELETE( m_pScollBarUpButtonTex );
	SAFE_DELETE( m_pScollBarUpButtonDownTex );
	SAFE_DELETE( m_pScollBarTrackTex );
	SAFE_DELETE( m_pScollBarThumbTex );

	SAFE_CLOSE( m_pSndButtonMouseDown );
	SAFE_CLOSE( m_pSndButtonMouseOver );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif
}

void CKTDGUIComboBox::InitScrollBar_LUA()
{
	m_pScrollBar = new CKTDGUIScrollBar;

	m_pScrollBar->SetDialog( m_pDialog );

	m_pScrollBar->SetDownButtonTex( m_pScollBarDownButtonTex );
	m_pScrollBar->SetDownButtonDownTex( m_pScollBarDownButtonDownTex );
	m_pScrollBar->SetUpButtonTex( m_pScollBarUpButtonTex );
	m_pScrollBar->SetUpButtonDownTex( m_pScollBarUpButtonDownTex );
	m_pScrollBar->SetTrackTex( m_pScollBarTrackTex );
	m_pScrollBar->SetThumbTex( m_pScollBarThumbTex );
}


HRESULT	CKTDGUIComboBox::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bShow == false || m_bEnable == false )
	{
		return false;
	}

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->OnFrameMove( fTime, fElapsedTime );
	}


	m_rcBoundingBox2 = m_rcButton;

	FromRectToPoint(m_rcBoundingBox2, m_NowPoint);

	CKTDGUIControl*	pTempControl = NULL;
	for ( int i = 0; i < (int)m_Items.GetSize(); i++ )
	{
		pTempControl = m_Items[i]->pControl;

		if ( pTempControl != NULL )
		{
			pTempControl->OnFrameMove(fTime, fElapsedTime);
		}
	}

	if ( m_bMouseOver == true )
	{
		if ( m_bMouseOverCheck == false )
		{
			m_bMouseOverCheck = true;
			PlaySound( m_pSndButtonMouseOver );
		}
	}
	else
	{
		m_bMouseOverCheck = false;
	}


	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CKTDGUIComboBox::OnFrameRender()
{

	
	if ( m_bShow == false )
	{
		return false;
	}


#ifdef DLL_BUILD
	DrawEditEdge( m_colorEdge );
#endif

	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( !m_bOpened )
		iState = DXUT_STATE_HIDDEN;



	// Dropdown box

	// If we have not initialized the scroll bar page size,
	// do that now.

	CKTDGFontManager::CUKFont* pFont = NULL;

	if ( m_pStringData != NULL )
	{
		pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex );
	}

	static bool bSBInit;
	if( !bSBInit )
	{
		if ( m_pScrollBar != NULL )
		{
			if ( m_bIsStaticComboBox == false )
			{
				// Update the page size of the scroll bar
				if( pFont != NULL )
				{
					m_pScrollBar->SetPageSize( RectHeight( m_rcDropdownText ) / pFont->GetHeight() );
				}
				else
				{
					m_pScrollBar->SetPageSize( RectHeight( m_rcDropdownText ) );
				}
			}
			else
			{
				m_pScrollBar->SetPageSize( RectHeight( m_rcDropdownText ) / m_nStaticHeight );
			}
		}
		bSBInit = true;
	}

	iState = DXUT_STATE_NORMAL;



	if( m_bShow == false )
		iState = DXUT_STATE_HIDDEN;
	else if( m_bEnable == false )
		iState = DXUT_STATE_DISABLED;
	else if( m_bPressed )
	{
		iState = DXUT_STATE_PRESSED;
	}
	else if( m_bHasFocus )
		iState = DXUT_STATE_FOCUS;
	



	float fBlendRate = ( iState == DXUT_STATE_PRESSED ) ? 0.0f : 0.8f;

	RECT rcWindow = m_rcButton;
	OffsetRect( &rcWindow, 0, 0 );

	FromRectToPoint(rcWindow, m_TempPoint);
	if ( m_pButtonTex != NULL )
	{
		m_TempPoint.pUITextureData = m_pButtonTex;
	}

	if ( iState == DXUT_STATE_PRESSED && m_pButtonMouseDownTex != NULL )
	{
		m_TempPoint.pUITextureData = m_pButtonMouseDownTex;
	}
	else if ( m_bMouseOver == true )
	{
		m_TempPoint.pUITextureData = m_pButtonMouseOverTex;
	}


	if ( m_TempPoint.pUITextureData != NULL )
	{

		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			m_TempPoint );

		RenderVertex( m_Vertex, m_TempPoint );
	}

	if( m_bOpened )
		iState = DXUT_STATE_PRESSED;

	// Main text box
	//TODO: remove magic numbers


	FromRectToPoint(m_rcText, m_TempPoint);
	if ( m_pTextTex != NULL )
	{
		m_TempPoint.pUITextureData = m_pTextTex;

		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
			m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
			m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
			m_TempPoint );

		RenderVertex( m_Vertex, m_TempPoint );
	}

    if(m_bSelectTextShow == false)
        return S_OK;

	if( m_iSelected >= 0 && m_iSelected < (int) m_Items.GetSize() )
	{
		ComboBoxItem* pItem = m_Items.GetAt( m_iSelected );

		D3DXVECTOR2 tempPos;
		//tempPos.x = (float)m_rcText.left + 5;
		//tempPos.y = (float)m_rcText.top  + (pItem->rcActive.bottom - pItem->rcActive.top)/4 - 2;

		

		tempPos.x = (float)m_rcText.left + m_TextStringOffsetPosX;
		tempPos.y = (float)m_rcText.top  + m_TextStringOffsetPosY;
		D3DXVECTOR2 pos;

		if ( m_bIsStaticComboBox == false )
		{
			if( pItem != NULL && m_pStringData != NULL)
			{
				D3DXCOLOR	tempColor;
				D3DXCOLOR	tempOutLineColor;


/*
				tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
				tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
				tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
				tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

				tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
				tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
				tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
				tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

				
				CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont( m_pStringData->fontIndex );

				pos = tempPos  + m_pDialog->GetPos() + m_OffsetPos;		
				pFont->Draw2DText( pItem->strText, (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );
				*/

				tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
				tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
				tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
				tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

				tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
				tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
				tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
				tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;



				wstring wstrDrawText = pItem->strText;
				if(m_iTextLimit > 0)
				{
					wstrDrawText = wstrDrawText.substr(0, m_iTextLimit);
				}

				

				pos = tempPos  + m_pDialog->GetPos() + m_OffsetPos;				
				//pFont->Draw2DText( pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle );
				pFont->OutTextMultiline( (int)pos.x, (int)pos.y, wstrDrawText.c_str(), tempColor, m_pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, m_pStringData->sortFlag );

			}
		}
		else
		{
			pos = m_pDialog->GetPos() + m_OffsetPos;
			pos.x += m_rcText.left;
			pos.y += m_rcText.top;
			if ( pItem->pControl != NULL )
			{
				pItem->pControl->SetOffsetPos( pos);
				pItem->pControl->OnFrameRender();
			}
			//오프셋을 여기로 해서 그림 그리기
		}
	}


	return S_OK;
}

HRESULT CKTDGUIComboBox::OnFrameRenderEnd()
{
	if ( m_bShow == false )
	{
		return false;
	}


	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( !m_bOpened )
		iState = DXUT_STATE_HIDDEN;



	// Dropdown box

	// If we have not initialized the scroll bar page size,
	// do that now.

	CKTDGFontManager::CUKFont* pFont = NULL;

	if ( m_pStringData != NULL )
	{
		pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex );
	}


	if ( m_bOpened )
	{
		FromRectToPoint(m_rcDropdown, m_TempPoint);
		if ( m_pDropdownTex != NULL )
		{

			m_TempPoint.pUITextureData = m_pDropdownTex;

			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_TempPoint );
			
			RenderVertex( m_Vertex, m_TempPoint );
		}
	}

	// Selection outline


	if( pFont != NULL )
	{
		int curY = m_rcDropdownText.top;
		int nRemainingHeight = RectHeight( m_rcDropdownText );
		//WCHAR strDropdown[4096] = {0};

		if ( m_pScrollBar != NULL )
		{

			for( int i = m_pScrollBar->GetTrackPos(); i < m_Items.GetSize(); i++ )
			{
				ComboBoxItem* pItem = m_Items.GetAt( i );

				if ( m_bIsStaticComboBox == false )
				{
					// Make sure there's room left in the dropdown
					nRemainingHeight -= (pFont->GetHeight());
				}
				else
				{
					nRemainingHeight -= m_nStaticHeight;
				}
				if( nRemainingHeight < 0 )
				{
					pItem->bVisible = false;
					continue;
				}

				if ( m_bIsStaticComboBox == false )
				{
					SetRect( &pItem->rcActive, m_rcDropdownText.left, curY, m_rcDropdownText.right + m_pScrollBar->GetWidth(), curY + (pFont->GetHeight()) );
					curY += (pFont->GetHeight());
				}
				else
				{
					SetRect( &pItem->rcActive, m_rcDropdownText.left, curY, m_rcDropdownText.right + m_pScrollBar->GetWidth(), curY + m_nStaticHeight );
					curY += m_nStaticHeight;
				}

				//debug
				//int blue = 50 * i;
				//m_pDialog->DrawRect( &pItem->rcActive, 0xFFFF0000 | blue );

				pItem->bVisible = true;

				if( m_bOpened )
				{
					RECT rc;
					SetRect( &rc, m_rcDropdown.left, pItem->rcActive.top-2, m_rcDropdown.right, (pItem->rcActive.bottom+2) );

					if( (int)i == m_iFocused )
					{
						if ( m_bIsStaticComboBox == false )
						{
							FromRectToPoint(rc, m_TempPoint);
							if ( m_pDropdownTextTex != NULL )
							{

								m_TempPoint.pUITextureData = m_pDropdownTextTex;

								UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
									m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
									m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
									m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
									m_TempPoint );

								RenderVertex( m_Vertex, m_TempPoint );
							}





							if ( m_pStringData != NULL )
							{
								D3DXVECTOR2 tempPos;
								tempPos.x = (float)pItem->rcActive.left;
								tempPos.y = (float)pItem->rcActive.top;

								D3DXCOLOR	tempColor;
								D3DXCOLOR	tempOutLineColor;
/*
								tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
								tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
								tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

								tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

								D3DXVECTOR2 pos;
								pos = tempPos + m_pDialog->GetPos() + m_OffsetPos;		
								pFont->Draw2DText( pItem->strText, (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );
*/

								tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
								tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
								tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

								tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;




								D3DXVECTOR2 pos;
								pos = tempPos + m_pDialog->GetPos() + m_OffsetPos;	
								//pFont->Draw2DText( pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle );
								pFont->OutTextMultiline( (int)pos.x, (int)pos.y, pItem->strText, tempColor, 
														m_pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, m_pStringData->sortFlag );


							}

						}
						else
						{
							//스태틱 그림 그리기
							if ( pItem->pControl != NULL )
							{
								pItem->pControl->SetOffsetPos( D3DXVECTOR2( (float)rc.left, (float)rc.top) );
								pItem->pControl->OnFrameRender();
							}
						}
					}
					else
					{
						if ( m_bIsStaticComboBox == false)
						{
							if ( m_pStringData != NULL )
							{
								D3DXVECTOR2 tempPos;
								tempPos.x = (float)pItem->rcActive.left;
								tempPos.y = (float)pItem->rcActive.top;

								D3DXCOLOR	tempColor;
								D3DXCOLOR	tempOutLineColor;
/*
								tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
								tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
								tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

								tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

								D3DXVECTOR2 pos;
								pos = tempPos  + m_pDialog->GetPos() + m_OffsetPos;		
								pFont->Draw2DText( pItem->strText, (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );*/
								

								tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
								tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
								tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

								tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

								D3DXVECTOR2 pos;
								pos = tempPos  + m_pDialog->GetPos() + m_OffsetPos;	
								//pFont->Draw2DText( pStringData->msg.c_str(), (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, pStringData->sortFlag, pStringData->fontStyle );
								pFont->OutTextMultiline( (int)pos.x, (int)pos.y, pItem->strText, tempColor, 
														m_pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, m_pStringData->sortFlag );

							}
						}
						else
						{
							if ( pItem->pControl != NULL )
							{
								pItem->pControl->SetOffsetPos( D3DXVECTOR2( (float)rc.left, (float)rc.top) );
								pItem->pControl->OnFrameRender();
							}
						}

					}
				}
			}
		}
		else
		{	//스크롤바가 없는 경우

			for( int i = 0; i < m_Items.GetSize(); i++ )
			{
				ComboBoxItem* pItem = m_Items.GetAt( i );

				if ( m_bIsStaticComboBox == false )
				{
					// Make sure there's room left in the dropdown
					nRemainingHeight -= (pFont->GetHeight());
				}
				else
				{
					nRemainingHeight -= m_nStaticHeight;
				}
				if( nRemainingHeight < 0 )
				{
					pItem->bVisible = false;
					continue;
				}

				if ( m_bIsStaticComboBox == false )
				{
					if ( m_pScrollBar != NULL )
						SetRect( &pItem->rcActive, m_rcDropdownText.left, curY, m_rcDropdownText.right + m_pScrollBar->GetWidth(), curY + (pFont->GetHeight()) );
					else
						SetRect( &pItem->rcActive, m_rcDropdownText.left, curY, m_rcDropdownText.right, curY + (pFont->GetHeight()) );

					curY += (pFont->GetHeight());
				}
				else
				{
					if ( m_pScrollBar != NULL )
						SetRect( &pItem->rcActive, m_rcDropdownText.left, curY, m_rcDropdownText.right + m_pScrollBar->GetWidth(), curY + m_nStaticHeight );
					else
						SetRect( &pItem->rcActive, m_rcDropdownText.left, curY, m_rcDropdownText.right, curY + m_nStaticHeight );

					curY += m_nStaticHeight;
				}

				//debug
				//int blue = 50 * i;
				//m_pDialog->DrawRect( &pItem->rcActive, 0xFFFF0000 | blue );

				pItem->bVisible = true;

				if( m_bOpened )
				{
					RECT rc;
					SetRect( &rc, m_rcDropdown.left, pItem->rcActive.top-2, m_rcDropdown.right, (pItem->rcActive.bottom+2) );

					if( (int)i == m_iFocused )
					{
						if ( m_bIsStaticComboBox == false )
						{
							FromRectToPoint(rc, m_TempPoint);
							if ( m_pDropdownTextTex != NULL )
							{
								m_TempPoint.pUITextureData = m_pDropdownTextTex;

								UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
									m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
									m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
									m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
									m_TempPoint );

								RenderVertex( m_Vertex, m_TempPoint );
							}



							if ( m_pStringData != NULL )
							{
								D3DXVECTOR2 tempPos;
								tempPos.x = (float)pItem->rcActive.left;
								tempPos.y = (float)pItem->rcActive.top;

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

								D3DXVECTOR2 pos;
								pos = tempPos + m_pDialog->GetPos() + m_OffsetPos;		
								//pFont->Draw2DText( pItem->strText, (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );

								pFont->OutTextMultiline( (int)pos.x, (int)pos.y, pItem->strText, tempColor, 
														m_pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, m_pStringData->sortFlag );
							}

						}
						else
						{
							//스태틱 그림 그리기
							if ( pItem->pControl != NULL )
							{
								pItem->pControl->SetOffsetPos( D3DXVECTOR2( (float)rc.left, (float)rc.top) );
								pItem->pControl->OnFrameRender();
							}
						}
					}
					else
					{
						if ( m_bIsStaticComboBox == false)
						{
							if ( m_pStringData != NULL )
							{
								D3DXVECTOR2 tempPos;
								tempPos.x = (float)pItem->rcActive.left;
								tempPos.y = (float)pItem->rcActive.top;

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

								D3DXVECTOR2 pos;
								pos = tempPos  + m_pDialog->GetPos() + m_OffsetPos;		
								//pFont->Draw2DText( pItem->strText, (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );
								pFont->OutTextMultiline( (int)pos.x, (int)pos.y, pItem->strText, tempColor, 
														m_pStringData->fontStyle, tempOutLineColor, 1.0f, NULL, m_pStringData->sortFlag );
							}
						}
						else
						{
							if ( pItem->pControl != NULL )
							{
								pItem->pControl->SetOffsetPos( D3DXVECTOR2( (float)rc.left, (float)rc.top) );
								pItem->pControl->OnFrameRender();

							}
						}

					}
				}
			}

		}

	}


	if ( m_bIsStaticComboBox == false )
	{
		// Scroll bar
		if( m_bOpened && m_pScrollBar != NULL && pFont != NULL && m_nDropHeight < pFont->GetHeight() * m_Items.GetSize() )
		{
			m_pScrollBar->OnFrameRender();
		}
	}
	else
	{
		// Scroll bar
		if( m_bOpened && m_pScrollBar != NULL && m_nDropHeight < m_nStaticHeight * m_Items.GetSize() )
		{
			m_pScrollBar->OnFrameRender();
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
bool CKTDGUIComboBox::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	return false;
}


//--------------------------------------------------------------------------------------
bool CKTDGUIComboBox::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false || m_bEnable == false)
		return false;

	// Let the scroll bar handle it first.
	if( m_pScrollBar != NULL )
	{
		if ( m_pScrollBar->HandleMouse( uMsg, pt, wParam, lParam ) == true )
		{
			return true;
		}

	}

	switch( uMsg )
	{
	case WM_MOUSEMOVE:
		{
			//POINT temp;
			//GetCursorPos(&temp);
			if( ( m_bOpened && PtInRect( &m_rcDropdown, pt ) )   )
			{
				// Determine which item has been selected
				for( int i=0; i < m_Items.GetSize(); i++ )
				{
					ComboBoxItem* pItem = m_Items.GetAt( i );

					if( pItem -> bVisible &&
						PtInRect( &pItem->rcActive, pt ) )
					{
						m_iFocused = i;

					}
				}
//				return false;
				return true;
			}
			

			break;
		}

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( PtInRect(&m_rcBoundingBox2, pt) )
			{
				// Pressed while inside the control
				m_bPressed = true;
#ifndef DLL_BUILD
				SetCapture( DXUTGetHWND() );
#endif
				if( !m_bHasFocus )
				{
					m_pDialog->RequestFocus( this );
				}


				// Toggle dropdown
				if( m_bHasFocus )
				{
					//if ( m_pSndButtonMouseDown != NULL )
					//	m_pSndButtonMouseDown->Play();
					PlaySound( m_pSndButtonMouseDown );

					m_bOpened = !m_bOpened;

					if( !m_bOpened )
					{
						m_pDialog->ClearFocus();
					}

					if( m_bOpened )
					{
						if( m_CustomMsgOpenDropdown != -1 )
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgOpenDropdown, (LPARAM)this );
						}
					}
					else
					{
						if( m_CustomMsgCloseDropdown != -1 )
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgCloseDropdown, (LPARAM)this );
						}
					}
				}

				return true;
			}

			// Perhaps this click is within the dropdown
			if( ( m_bOpened && PtInRect( &m_rcDropdown, pt ) ) )
			{
				if ( m_pScrollBar != NULL )
				{

					// Determine which item has been selected
					for( int i= m_pScrollBar->GetTrackPos(); i < m_Items.GetSize(); i++ )
					{
						ComboBoxItem* pItem = m_Items.GetAt( i );

						if( pItem -> bVisible &&
							PtInRect( &pItem->rcActive, pt ) )
						{
							m_iFocused = m_iSelected = i;


							//if ( m_pSndButtonMouseDown != NULL )
							//	m_pSndButtonMouseDown->Play();
							PlaySound( m_pSndButtonMouseDown );


							if( m_CustomMsgSelectionChanged != -1 )
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
							}
							else
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
							}

							m_bOpened = false;

							m_pDialog->ClearFocus();

							break;
						}
					}

				}
				else
				{
					for( int i= 0; i < m_Items.GetSize(); i++ )
					{
						ComboBoxItem* pItem = m_Items.GetAt( i );

						if( pItem -> bVisible &&
							PtInRect( &pItem->rcActive, pt ) )
						{
							m_iFocused = m_iSelected = i;

							//if ( m_pSndButtonMouseDown != NULL )
							//	m_pSndButtonMouseDown->Play();

							PlaySound( m_pSndButtonMouseDown );


							if( m_CustomMsgSelectionChanged != -1 )
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
							}
							else
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
							}

							m_bOpened = false;

							// 다이얼로그 포커싱을 초기화하는 부분 제거
							// : 콤보 박스 선택후, 자동 포커싱이 필요한 경우가 있음
							//m_pDialog->ClearFocus();

							break;
						}
					}
				}

				return true;
			}

			// Mouse click not on main control or in dropdown, fire an event if needed
			if( m_bOpened )
			{
				m_iFocused = m_iSelected;

				//if ( m_pSndButtonMouseDown != NULL )
				//	m_pSndButtonMouseDown->Play();
				PlaySound( m_pSndButtonMouseDown );

				if( m_CustomMsgSelectionChanged != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
				else
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );

				m_bOpened = false;
			}

			// Make sure the control is no longer in a pressed state
			m_bPressed = false;

			m_pDialog->ClearFocus();

			break;
		}

	case WM_LBUTTONUP:
		{
			if( m_bPressed &&  PtInRect(&m_rcBoundingBox2, pt)  )
			{
				// Button click
				m_bPressed = false;
				ReleaseCapture();
				return true;
			}

			break;
		}

	case WM_MOUSEWHEEL:
		{
			int zDelta = (short) HIWORD(wParam) / WHEEL_DELTA;

			if( m_bOpened )
			{
				UINT uLines;
				SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &uLines, 0 );

				if ( m_pScrollBar != NULL )
				{
					m_pScrollBar->Scroll( -zDelta * uLines );
				}
			} 
			else
			{
				if( zDelta > 0 )
				{
					if( m_iFocused > 0 )
					{
						m_iFocused--;
						m_iSelected = m_iFocused;     

						if( !m_bOpened )
						{
							if( m_CustomMsgSelectionChanged != -1 )
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
							else
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
						}
					}          
				}
				else
				{
					if( m_iFocused+1 < (int)GetNumItems() )
					{
						m_iFocused++;
						m_iSelected = m_iFocused;   

						if( !m_bOpened )
						{
							if( m_CustomMsgSelectionChanged != -1 )
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
							else
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
						}
					}
				}
			}
			return false;
		}
	};

	CKTDGUIControl*	pTempControl = NULL;
	for ( int i = 0; i < (int)m_Items.GetSize(); i++ )
	{
		pTempControl = m_Items[i]->pControl;

		if ( pTempControl != NULL )
		{
			pTempControl->HandleMouse( uMsg, pt, wParam, lParam );
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
void CKTDGUIComboBox::UpdateRects()
{
	if ( m_bIsStaticComboBox == true )
		SetRect( &m_rcBoundingBox, m_x, m_y, m_x + m_width + m_height, m_y + m_height );
	else
		SetRect( &m_rcBoundingBox, m_x, m_y, m_x + m_width, m_y + m_height );

	if ( m_ButtonHeight != 0 && m_ButtonWidth != 0 )
	{
		m_rcButton.left = m_ButtonX;
		m_rcButton.right = m_ButtonX + m_ButtonWidth;
		m_rcButton.top = m_ButtonY;
		m_rcButton.bottom = m_ButtonY + m_ButtonHeight;
	}
	else
	{
		m_rcButton = m_rcBoundingBox;

		m_rcButton.left = m_rcButton.right - RectHeight( m_rcButton );
		m_rcButton.right += m_ButtonAddWidth;
		m_rcButton.top -= m_ButtonAddHeight/2;
		m_rcButton.bottom += m_ButtonAddHeight/2;
	}

	if ( m_TextHeight != 0 && m_TextWidth != 0 )
	{
		m_rcText.left = m_TextX;
		m_rcText.right = m_TextX + m_TextWidth;
		m_rcText.top = m_TextY;
		m_rcText.bottom = m_TextY + m_TextHeight;
	}
	else
	{
		m_rcText = m_rcBoundingBox;
		m_rcText.right = m_rcButton.left;
	}


	m_rcDropdown = m_rcText;
	
	if ( m_bIsReverseDropDown == false )
	{
		OffsetRect( &m_rcDropdown, 0, (int) (0.90f * RectHeight( m_rcText )) ); 
		m_rcDropdown.bottom += m_nDropHeight; 
	}
	else
	{
		m_rcDropdown.top -= m_nDropHeight*2;
		m_rcDropdown.bottom = m_rcText.top;
	}
	
	
	

	m_rcDropdownText = m_rcDropdown;
	m_rcDropdownText.left += (int) (0.1f * RectWidth( m_rcDropdown ));

	if ( m_pScrollBar != NULL )
		m_rcDropdownText.right -= (int) (0.1f * RectWidth( m_rcDropdown )) + m_pScrollBar->GetWidth();
	else
		m_rcDropdownText.right -= (int) (0.1f * RectWidth( m_rcDropdown ));

	m_rcDropdownText.top += (int) (0.1f * RectHeight( m_rcDropdown ));
	//m_rcDropdownText.bottom -= (int) (0.1f * RectHeight( m_rcDropdown ));






	if ( m_pScrollBar != NULL)
	{
		m_pScrollBar->SetLocation(m_rcDropdown.right, m_rcDropdown.top + 2 );
		m_pScrollBar->SetSize( m_nSBWidth, RectHeight( m_rcDropdown )-2 );
	}
	
	if ( m_pStringData != NULL && m_pScrollBar != NULL )
	{
		CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex ); 
	
		if ( m_bIsStaticComboBox == false )
			m_pScrollBar->SetPageSize( RectHeight( m_rcDropdownText ) / pFont->GetHeight() );
		else
			m_pScrollBar->SetPageSize( RectHeight( m_rcDropdownText ) / m_nStaticHeight );
		
		// The selected item may have been scrolled off the page.
		// Ensure that it is in page again.
		m_pScrollBar->ShowItem( m_iSelected );
		
	}
}


//--------------------------------------------------------------------------------------
void CKTDGUIComboBox::OnFocusOut()
{
	m_bMousePointAdjust = true;
	m_bHasFocus = false;
	m_bOpened = false;
	m_ButtonOffsetX = 0;
	m_ButtonOffsetY = 0;
}


void CKTDGUIComboBox::FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point )
{
	point.leftTopPoint.x = (float)rect.left;
	point.leftTopPoint.y = (float)rect.top;
	point.rightTopPoint.x = (float)rect.right;
	point.rightTopPoint.y = (float)rect.top;
	point.leftBottomPoint.x = (float)rect.left;
	point.leftBottomPoint.y = (float)rect.bottom;
	point.rightBottomPoint.x = (float)rect.right;
	point.rightBottomPoint.y = (float)rect.bottom;
}


//--------------------------------------------------------------------------------------
void CKTDGUIComboBox::OnHotkey()
{
	if( m_bOpened )
		return;

	if( m_iSelected == -1 )
		return;

	m_iSelected++;

	if( m_iSelected >= (int) m_Items.GetSize() )
		m_iSelected = 0;

	m_iFocused = m_iSelected;

	if( m_CustomMsgSelectionChanged != -1 )
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
	else
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
}


void CKTDGUIComboBox::SetDropdownTextTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pDropdownTextTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pDropdownTextTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );

}

void CKTDGUIComboBox::SetButtonTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );

}

void CKTDGUIComboBox::SetButtonMouseOverTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pButtonMouseOverTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pButtonMouseOverTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );

}

void CKTDGUIComboBox::SetButtonMouseDownTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pButtonMouseDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pButtonMouseDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() ); 

}

void CKTDGUIComboBox::SetDropdownTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pDropdownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pDropdownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUIComboBox::SetTextTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pTextTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pTextTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );

}

void	CKTDGUIComboBox::SetScollBarUpButtonTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pScollBarUpButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarUpButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIComboBox::SetScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pScollBarUpButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarUpButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIComboBox::SetScollBarDownButtonTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pScollBarDownButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarDownButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIComboBox::SetScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pScollBarDownButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarDownButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIComboBox::SetScollBarTrackTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pScollBarTrackTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarTrackTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIComboBox::SetScollBarThumbTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pScollBarThumbTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarThumbTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

//void	CKTDGUIComboBox::SetItemScriptName( const char* pFileName )
//{
//	if ( pFileName != NULL )
//	{
//		ConvertUtf8ToWCHAR( m_ItemScriptName, pFileName );
//		OpenScript( m_ItemScriptName.c_str() );
//	}
//}
//
//bool	CKTDGUIComboBox::OpenScript( const WCHAR* pFileName )
//{
//	if( NULL == pFileName )
//		return false;
//
//
//
//	//lua_getfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIDialog" );
//	//int iRefUIDialog = lua_ref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), true );
//	//lua_pushnil( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIDialog" );
//
//
//	//lua_getfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIComboBox" );
//	//int iRefUIComboBox = lua_ref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), true );
//	//lua_pushnil( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIComboBox" );
//
//	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIComboBox", this );
//	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIDialog", m_pDialog );
//
//
//	bool bResult = g_pKTDXApp->LoadLuaTinker( pFileName );
//
//
//	
//
//	//lua_getref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIComboBox );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIComboBox" );
//	//lua_unref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIComboBox );
//
//
//	//lua_getref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIDialog );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIDialog" );
//	//lua_unref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIDialog );
//
//	return false;
//}
//

void CKTDGUIComboBox::AddItem_LUA( const char* pName )
{
	wstring WName;
	ConvertUtf8ToWCHAR( WName, pName );

	AddItem(WName.c_str(), NULL);

}

//--------------------------------------------------------------------------------------
HRESULT CKTDGUIComboBox::AddItem( const WCHAR* strText, void* pData, bool bSendChangeMsg )
{
	// Validate parameters
	if( strText== NULL )
	{
		return E_INVALIDARG;
	}

	// Create a new item and set the data
	ComboBoxItem* pItem = new ComboBoxItem;
	if( pItem == NULL )
	{
		return DXTRACE_ERR_MSGBOX( L"new", E_OUTOFMEMORY );
	}

	ZeroMemory( pItem, sizeof(ComboBoxItem) );
	StringCchCopy( pItem->strText, 256, strText );
	pItem->pData = pData;

	m_Items.Add( pItem );

	// Update the scroll bar with new range
	
	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
	}

	// If this is the only item in the list, it's selected
	if( GetNumItems() == 1 )
	{
		m_iSelected = 0;
		m_iFocused = 0;

		if ( bSendChangeMsg == true )
		{
			if( m_CustomMsgSelectionChanged != -1 )
			{
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
			}
			else
			{
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
			}
		}
	}

	if ( m_pScrollBar == NULL )
	{
		if ( m_bIsStaticComboBox == false )
		{
			CKTDGFontManager::CUKFont* pFont = NULL;

			if ( m_pStringData != NULL )
				pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex );

			if ( pFont != NULL )
				SetDropHeight( pFont->GetHeight() * GetNumItems() );
		}
		else
		{
			SetDropHeight( m_nStaticHeight * GetNumItems() );
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
HRESULT CKTDGUIComboBox::AddControl( CKTDGUIControl* pControl )
{

	// Create a new item and set the data
	ComboBoxItem* pItem = new ComboBoxItem;
	if( pItem == NULL )
	{
		return DXTRACE_ERR_MSGBOX( L"new", E_OUTOFMEMORY );
	}

	ZeroMemory( pItem, sizeof(ComboBoxItem) );
	StringCchCopy( pItem->strText, 256, pControl->GetName() );
	pItem->pData = NULL;
	pControl->SetDialog(m_pDialog);
	pItem->pControl = pControl;

	m_Items.Add( pItem );

	// Update the scroll bar with new range

	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
	}

	// If this is the only item in the list, it's selected
	if( GetNumItems() == 1 )
	{
		m_iSelected = 0;
		m_iFocused = 0;

		if( m_CustomMsgSelectionChanged != -1 )
		{
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
		}
		else
		{
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
void CKTDGUIComboBox::RemoveItem( UINT index )
{
	ComboBoxItem* pItem = m_Items.GetAt( index );
	SAFE_DELETE( pItem );
	m_Items.Remove( index );


	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
	}

	if( m_iSelected >= m_Items.GetSize() )
		m_iSelected = m_Items.GetSize() - 1;
}


//--------------------------------------------------------------------------------------
void CKTDGUIComboBox::RemoveAllItems()
{
	for( int i=0; i < m_Items.GetSize(); i++ )
	{
		ComboBoxItem* pItem = m_Items.GetAt( i );
		SAFE_DELETE( pItem );
	}

	m_Items.RemoveAll();



	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->SetTrackRange( 0, 1 );
	}
	m_iFocused = m_iSelected = -1;
}



//--------------------------------------------------------------------------------------
bool CKTDGUIComboBox::ContainsItem( const WCHAR* strText, UINT iStart )
{
	return ( -1 != FindItem( strText, iStart ) );
}


//--------------------------------------------------------------------------------------
int CKTDGUIComboBox::FindItem( const WCHAR* strText, UINT iStart )
{
	if( strText == NULL )
		return -1;

	for( int i = iStart; i < m_Items.GetSize(); i++ )
	{
		ComboBoxItem* pItem = m_Items.GetAt(i);

		if( 0 == wcscmp( pItem->strText, strText ) )
		{
			return i;
		}
	}

	return -1;
}


//--------------------------------------------------------------------------------------
void* CKTDGUIComboBox::GetSelectedData()
{
	if( m_iSelected < 0 )
		return NULL;

	ComboBoxItem* pItem = m_Items.GetAt( m_iSelected );
	return pItem->pData;
}


//--------------------------------------------------------------------------------------
CKTDGUIComboBox::ComboBoxItem* CKTDGUIComboBox::GetSelectedItem()
{
	if( m_iSelected < 0 )
		return NULL;

	return m_Items.GetAt( m_iSelected );
}


//--------------------------------------------------------------------------------------
void* CKTDGUIComboBox::GetItemData( const WCHAR* strText )
{
	int index = FindItem( strText );
	if( index == -1 )
	{
		return NULL;
	}

	ComboBoxItem* pItem = m_Items.GetAt(index);
	if( pItem == NULL )
	{
		DXTRACE_ERR( L"CGrowableArray::GetAt", E_FAIL );
		return NULL;
	}

	return pItem->pData;
}


//--------------------------------------------------------------------------------------
void* CKTDGUIComboBox::GetItemData( int nIndex )
{
	if( nIndex < 0 || nIndex >= m_Items.GetSize() )
		return NULL;

	return m_Items.GetAt( nIndex )->pData;
}


//--------------------------------------------------------------------------------------
HRESULT CKTDGUIComboBox::SetSelectedByIndex( UINT index )
{
	if( index >= GetNumItems() )
		return E_INVALIDARG;

	m_iFocused = m_iSelected = index;

	if( m_CustomMsgSelectionChanged != -1 )
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
	}
	else
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
	}

	return S_OK;
}



//--------------------------------------------------------------------------------------
HRESULT CKTDGUIComboBox::SetSelectedByText( const WCHAR* strText, bool bFireMsg /*= true*/ )
{
	if( strText == NULL )
		return E_INVALIDARG;

	int index = FindItem( strText );
	if( index == -1 )
		return E_FAIL;

	m_iFocused = m_iSelected = index;

	if( true == bFireMsg )
	{
		if( m_CustomMsgSelectionChanged != -1 )
		{
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
		}
		else
		{
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
		}
	}

	return S_OK;
}



//--------------------------------------------------------------------------------------
HRESULT CKTDGUIComboBox::SetSelectedByData( void* pData )
{
	for( int i=0; i < m_Items.GetSize(); i++ )
	{
		ComboBoxItem* pItem = m_Items.GetAt(i);

		if( pItem->pData == pData )
		{
			m_iFocused = m_iSelected = i;
			
			if( m_CustomMsgSelectionChanged != -1 )
			{
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionChanged, (LPARAM)this );
			}
			else
			{
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, CBEM_SELECTION_CHANGED, (LPARAM)this );
			}

			return S_OK;
		}
	}

	return E_FAIL;
}

void CKTDGUIComboBox::SetUnSelected()
{
	m_iFocused = -1;
	m_iSelected = -1;
}

void CKTDGUIComboBox::SetPos_LUA( int x, int y )
{
	m_x = x;
	m_y = y;

	UpdateRects();
}

void CKTDGUIComboBox::SetSize_LUA(int width, int height )
{
	m_width = width;
	m_height = height;

	UpdateRects();
}

void CKTDGUIComboBox::SetButtonAddSize( int addWidth, int addHeight ) 
{ 
	m_ButtonAddWidth = addWidth;
	m_ButtonAddHeight = addHeight; 

	UpdateRects();
}

void CKTDGUIComboBox::SetTextPos_LUA( int x, int y )
{
	m_TextX = x;
	m_TextY = y;

	UpdateRects();
}

void CKTDGUIComboBox::SetTextSize_LUA( int width, int height )
{
	m_TextWidth = width;
	m_TextHeight = height;

	UpdateRects();
}

void CKTDGUIComboBox::SetButtonPos_LUA( int x, int y )
{
	m_ButtonX = x;
	m_ButtonY = y;

	UpdateRects();
}

void CKTDGUIComboBox::SetButtonSize_LUA( int width, int height )
{
	m_ButtonWidth = width;
	m_ButtonHeight = height;

	UpdateRects();
}

#ifdef DLL_BUILD

bool CKTDGUIComboBox::IsSelectByEditGui( POINT pt )
{
	RECT rect = AddRect(m_rcText, m_rcDropdown);

	D3DXVECTOR2 vLeftTop( (float) rect.left, (float) rect.top );
	D3DXVECTOR2 vRightTop( (float) rect.right, (float) rect.top );
	D3DXVECTOR2 vLeftBottom( (float) rect.left, (float) rect.bottom );
	D3DXVECTOR2 vRightBottom( (float) rect.right, (float) rect.bottom );
	if( true == Pick2DRect( pt, vLeftTop, vRightTop, vLeftBottom, vRightBottom ) )
		return true;


	return false;
}

void CKTDGUIComboBox::MoveControl( float fx, float fy )
{
	m_TextX += (int)fx;
	m_TextY += (int)fy;

	m_ButtonX += (int)fx;
	m_ButtonY += (int)fy;

	UpdateRects();
}

void CKTDGUIComboBox::SetEditGUI( bool bEdit )
{
	SetColor( D3DXCOLOR(0xffffffff) );
	m_bEditEdge = bEdit;
	m_bOpened = bEdit;

	//m_rcEdge = AddRect(m_rcText, m_rcButton);
	//m_rcEdge = AddRect(m_rcEdge, m_rcDropdown);

	m_colorEdge = D3DXCOLOR(0xff0000ff);	
}

D3DXVECTOR2 CKTDGUIComboBox::GetPos()
{
	return D3DXVECTOR2((float)m_TextX, (float)m_TextY);
}

vector<D3DXVECTOR2> CKTDGUIComboBox::GetPosList()
{
	vector<D3DXVECTOR2> ret;
	
	ret.push_back( D3DXVECTOR2((float)m_TextX, (float)m_TextY) );
	ret.push_back( D3DXVECTOR2((float)m_ButtonX, (float)m_ButtonY) );

	return ret;
}

void CKTDGUIComboBox::DrawEditEdge( D3DXCOLOR edgeColor )
{
	if( false == m_bEditEdge )
		return;	

	if ( m_pCheckedEdgeTexture == NULL )
		return;	

	//const CKTDGUIControl::UIPointData & point = *m_pEditEdgePoint;
	D3DXCOLOR tempColor;

	int edgeWidth = 2;
	//D3DXCOLOR edgeColor = D3DXCOLOR(0xffff0000);

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;

	RECT edgeRect = AddRect(m_rcText, m_rcButton);
	edgeRect = AddRect(edgeRect, m_rcDropdown);


	int _width = (int)(edgeRect.right - edgeRect.left);
	int _height = (int)(edgeRect.bottom - edgeRect.top);

	//if ( m_bDrawEdgeOut == true )
	{
		// 좌 left/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하left/bottom
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.bottom ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우right/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.right ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상left/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}
}


#endif

#ifdef COMBOBOX_ADD_ITEM_STR_ID
void CKTDGUIComboBox::AddItemStrID_LUA( int iIndex )
{
	wstring wstrName = GET_STRING( iIndex );
	AddItem(wstrName.c_str(), NULL);
}
#endif COMBOBOX_ADD_ITEM_STR_ID

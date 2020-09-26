#include "StdAfx.h"
#include ".\ktdguilistbox.h"

inline int RectWidth( RECT &rc ) { return ( (rc).right - (rc).left ); }
inline int RectHeight( RECT &rc ) { return ( (rc).bottom - (rc).top ); }

CKTDGUIListBox::CKTDGUIListBox()
: m_rcBoundingBox( RECT() )
, m_rcSelection( RECT() )
, m_rcText( RECT() )
#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
, m_bIsSelectItemAtMouseMove( false )
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
{
	m_ControlType = UCT_LISTBOX;

	m_pBoudingBoxTex = NULL;
	m_pTextTex = NULL;

	m_pButtonMouseOverTex			= NULL;
	m_pScollBarDownButtonTex		= NULL;
	m_pScollBarDownButtonDownTex	= NULL;
	m_pScollBarUpButtonTex			= NULL;
	m_pScollBarUpButtonDownTex		= NULL;
	m_pScollBarTrackTex				= NULL;
	m_pScollBarThumbTex				= NULL;

	m_pStringData		= NULL;
	m_pGroupStringData	= NULL;

	m_dwStyle		= 0;
	m_nSBWidth		= 16;
	m_nSelected		= -1;
	m_nSelStart		= 0;
	m_bDrag			= false;
	m_nBorder		= 6;
	m_nMargin		= 5;
	m_nTextHeight	= 0;

	m_bShowTextTex	= true;

	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;

	m_CustomMsgItemDoubleClick = -1;
	m_CustomMsgSelection = -1;
	m_CustomMsgSelection_End = -1;
	m_CustomMsgSelectionByRButton = -1;

	m_pScrollBar = NULL;

	m_ItemScriptName = L"";
	m_bIsStaticListBox = false;

	m_pSndMouseDown = NULL;

	m_bIgnoreListClick = false;
	m_bUseItemDisappear = false;

	m_pUKFont		= NULL;
	m_pGroupUKFont	= NULL;

	m_BlankWidthByDepth = 5;
	m_BlankWidthToGroupMark = 10;

	m_bUpdateScrollBarOnUpdate = true;
	m_bScrollToEndOnUpdateRects = false;

	m_pGroupMark = NULL;

	m_GroupMarkOffsetPos = D3DXVECTOR2( -10, -1 );
	m_GroupMarkAddSize	= D3DXVECTOR2( -2, -3 );

	m_pGroupMark = new CKTDGUIStatic();
	m_pGroupMark->SetDialog( NULL ); 
	CKTDGUIControl::CPictureData* pPictureData = new CKTDGUIControl::CPictureData();
	pPictureData->SetTex( L"DLG_Messenger.tga", L"Plus_Box" );
	pPictureData->SetPoint();
	pPictureData->SetSize( D3DXVECTOR2( pPictureData->GetSize().x + m_GroupMarkAddSize.x, pPictureData->GetSize().y + m_GroupMarkAddSize.y ) );

	m_pGroupMark->AddPicture( pPictureData );

	pPictureData = new CKTDGUIControl::CPictureData();
	pPictureData->SetTex( L"DLG_Messenger.tga", L"Minus_Box" );
	pPictureData->SetPoint();
	pPictureData->SetSize( D3DXVECTOR2( pPictureData->GetSize().x + m_GroupMarkAddSize.x, pPictureData->GetSize().y + m_GroupMarkAddSize.y ) );

	m_pGroupMark->AddPicture( pPictureData );

	//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
	KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring checkSndFileName;
	LUA_GET_VALUE( kLuaManager, "ListBox_Mouse_Down", checkSndFileName, L"" );
	m_pSndMouseDown = g_pKTDXApp->GetDeviceManager()->OpenSound( checkSndFileName );

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;
#endif
}


//--------------------------------------------------------------------------------------
CKTDGUIListBox::~CKTDGUIListBox()
{
	RemoveAllItems();

	SAFE_DELETE( m_pBoudingBoxTex );
	SAFE_DELETE( m_pTextTex );

	SAFE_DELETE( m_pStringData );
	SAFE_DELETE( m_pGroupStringData );

	SAFE_DELETE( m_pScrollBar );

	SAFE_DELETE( m_pButtonMouseOverTex );

	SAFE_DELETE(m_pScollBarDownButtonTex);
	SAFE_DELETE(m_pScollBarDownButtonDownTex);
	SAFE_DELETE(m_pScollBarUpButtonDownTex);
	SAFE_DELETE(m_pScollBarUpButtonTex);
	SAFE_DELETE(m_pScollBarTrackTex);
	SAFE_DELETE(m_pScollBarThumbTex);

	SAFE_CLOSE( m_pSndMouseDown );

	SAFE_DELETE( m_pGroupMark );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif

	m_pUKFont		= NULL;
	m_pGroupUKFont	= NULL;
}

CKTDGUIScrollBar* CKTDGUIListBox::InitScrollBar_LUA()
{
	m_pScrollBar = new CKTDGUIScrollBar();

	m_pScrollBar->SetDialog( m_pDialog );

	m_pScrollBar->SetDownButtonTex( m_pScollBarDownButtonTex );
	m_pScrollBar->SetUpButtonTex( m_pScollBarUpButtonTex );
	m_pScrollBar->SetDownButtonDownTex( m_pScollBarDownButtonDownTex );
	m_pScrollBar->SetUpButtonDownTex( m_pScollBarUpButtonDownTex );
	m_pScrollBar->SetTrackTex( m_pScollBarTrackTex );
	m_pScrollBar->SetThumbTex( m_pScollBarThumbTex );

	return m_pScrollBar;
}


HRESULT	CKTDGUIListBox::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if ( m_bShow == false || m_bEnable == false )
	{
		return S_OK;
	}

	m_pGroupMark->OnFrameMove( fTime, fElapsedTime );

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->OnFrameMove( fTime, fElapsedTime );
	}

	RECT tempRECT;

	tempRECT = m_rcBoundingBox;
	if ( m_pScrollBar != NULL )
	{
		//if( true == m_bScrollBarOnRight )
		//{ 
			tempRECT.left += m_pScrollBar->GetWidth();
			tempRECT.right += m_pScrollBar->GetWidth();
		//}
		//else
		//{
		//	tempRECT.left -= m_pScrollBar->GetWidth();
		//	tempRECT.right -= m_pScrollBar->GetWidth();
		//}
	}

	FromRectToPoint( tempRECT, m_NowPoint );

	CKTDGUIControl*	pTempControl = NULL;
	for ( int i = 0; i < (int)m_Items.GetSize(); i++ )
	{
		m_Items[i]->OnFrameMove( fTime, fElapsedTime );

		pTempControl = m_Items[i]->pControl;
		if ( pTempControl != NULL )
		{
			pTempControl->OnFrameMove(fTime, fElapsedTime);
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CKTDGUIListBox::OnFrameRender()
{
	KTDXPROFILE();

	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	DrawEditEdge();
#endif

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex ); 
	if ( pFont == NULL )
	{
		MessageBox(g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_954 ),  GET_STRING( STR_ID_955 ), MB_OK );
		return false;
	}

	FromRectToPoint(m_rcBoundingBox, m_TempPoint);
	if ( m_pBoudingBoxTex != NULL )
	{
		m_TempPoint.pUITextureData = m_pBoudingBoxTex;
	}

	UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
		m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
		m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
		m_TempPoint );

	RenderVertex( m_Vertex, m_TempPoint );


	// Render the text
	if( m_Items.GetSize() > 0 )
	{
		// Find out the height of a single line of text
		RECT rc = m_rcText;
		RECT rcSel = m_rcSelection;
		
		if ( m_bIsStaticListBox == false )
			rc.bottom = rc.top + pFont->GetHeight();
		else
			rc.bottom = rc.top + m_nTextHeight;

		// Update the line height formation
		if ( m_bIsStaticListBox == false )
			m_nTextHeight = rc.bottom - rc.top;

		static bool bSBInit;
		if( !bSBInit )
		{
			// Update the page size of the scroll bar
			if( m_nTextHeight && m_pScrollBar != NULL )
				m_pScrollBar->SetPageSize( RectHeight( m_rcText ) / m_nTextHeight );
			else if ( m_pScrollBar != NULL )
			{
				m_pScrollBar->SetPageSize( RectHeight( m_rcText ) );
			}
			bSBInit = true;
		}

		rc.right = m_rcText.right;

		if ( m_pScrollBar != NULL )
		{
			//{{ dmlee 2008.03.17 디버깅 용도
			//LastErrorLog( m_Name.c_str() );
			if( m_pScrollBar->GetTrackPos() < 0 )
			{
#ifdef LEAVE_LAST_ERROR_LOG_TEST
				WCHAR wszText[256] = L"";
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText),
                    L"%s, %d, %d, %d, %d", m_Name.c_str(), m_pScrollBar->GetTrackPos(), (int)m_Items.GetSize(),
					m_pScrollBar->GetTrackEndPos(), m_pScrollBar->GetPageSize() );
				LastErrorLog( wszText );
#endif LEAVE_LAST_ERROR_LOG_TEST
				ASSERT( !"CKTDGUIListBox::OnFrameRender()" );
			}
			//}} dmlee 2008.03.17 디버깅 용도


			//{{ dmlee 2008.04.10 - 일단 뻗지는 않게 막자
			if( m_pScrollBar->GetTrackPos() < 0 )
			{
				m_pScrollBar->SetTrackPos( 0 );
			}
			//}} dmlee 2008.04.10 - 일단 뻗지는 않게 막자

			//{{ 허상형 : [2009/7/22] //	나소드 메가폰 마우스오버 대상인지 검사
			for( int i = m_pScrollBar->GetTrackPos(); i < (int)m_Items.GetSize(); ++i )
			{
				if( rc.bottom > m_rcText.bottom )
					break;

				ListBoxItem *pItem = m_Items.GetAt( i );
				if( false == pItem->m_bShow )
				{
					OffsetRect( &rc, 0, m_nTextHeight );
					continue;
				}

				//	메가폰인지 확인
				else if( !( m_dwStyle & MULTISELECTION ) && pItem->GetMessageID() > -1 )
				{
					rcSel.top = rc.top; rcSel.bottom = rc.bottom;
					rcSel.right += m_pScrollBar->GetWidth();

					

					//	마우스 좌표 구하기
					D3DXVECTOR2 vMousePos;
					vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
					vMousePos.y =  (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
					vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

					//	채팅 박스의 ListBox 위치 구하고 컨버팅
					vMousePos.x -= GetDialog()->GetPos().x;
					vMousePos.y -= GetDialog()->GetPos().y;

					FromRectToPoint(rcSel, m_TempPoint);

					D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 380, 380 );

					m_TempPoint.rightTopPoint.x = GetPos().x + GetWidth();
					m_TempPoint.rightBottomPoint.x = GetPos().x + GetWidth();

					//	마우스 좌표가 컨트롤 안에 있으면 같은 렌더할 대상으로 지정
					if(  m_TempPoint.leftTopPoint.x < vMousePos.x && m_TempPoint.rightBottomPoint.x > vMousePos.x &&
						m_TempPoint.leftTopPoint.y < vMousePos.y && m_TempPoint.rightBottomPoint.y > vMousePos.y )
					{
						int iBeforeMsgID = pItem->m_iMessageID;

						for( int j = m_pScrollBar->GetTrackPos(); j < (int)m_Items.GetSize(); ++j )
						{
							ListBoxItem *pLoopItem = m_Items.GetAt( j );

							if( iBeforeMsgID == pLoopItem->m_iMessageID )
							{
								//	자기 자신의 메시지면 보여지면 안됨
								if( pLoopItem->m_bMyMessage == true )
								{
									pLoopItem->m_bMegaOver = false;
								}
								else
								{
									pLoopItem->m_bMegaOver = true;
								}
							}
							else
							{
								pLoopItem->m_bMegaOver = false;
							}
						}
						break;
					}
					else
					{
						pItem->m_bMegaOver = false;
					}
				}
				OffsetRect( &rc, 0, m_nTextHeight );
			}

			rc = m_rcText;
			rcSel = m_rcSelection;

			if ( m_bIsStaticListBox == false )
				rc.bottom = rc.top + pFont->GetHeight();
			else
				rc.bottom = rc.top + m_nTextHeight;

			//}} 허상형 : [2009/7/22] //	나소드 메가폰 마우스오버 대상인지 검사

			for( int i = m_pScrollBar->GetTrackPos(); i < (int)m_Items.GetSize(); ++i )
			{
				// Determine if we need to render this item with the
				// selected element.
				bool bSelectedStyle = false;

				if( rc.bottom > m_rcText.bottom )
					break;

				ListBoxItem *pItem = m_Items.GetAt( i );
				if( false == pItem->m_bShow )
				{
					OffsetRect( &rc, 0, m_nTextHeight );
					continue;
				}



				//{{ 허상형 : [2009/7/21] //	메가폰 마우스 클릭 여부에 따라 랜더링 할지 
				if( !( m_dwStyle & MULTISELECTION ) && pItem->m_bMegaSelect == true )
				{
					bSelectedStyle = true;
				}

				else if( !( m_dwStyle & MULTISELECTION ) && pItem->m_bMegaOver == true )
				{
					bSelectedStyle = true;
				}
				
				else
				//}} 허상형 : [2009/7/21] //	메가폰 마우스 클릭 여부에 따라 랜더링 할지 

				if( !( m_dwStyle & MULTISELECTION ) && i == m_nSelected )
					bSelectedStyle = true;
				else
				{
					if( m_dwStyle & MULTISELECTION )
					{
						if( m_bDrag &&
							( ( i >= m_nSelected && i < m_nSelStart ) ||
							( i <= m_nSelected && i > m_nSelStart ) ) )
							bSelectedStyle = m_Items[m_nSelStart]->bSelected;
						else
							if( pItem->bSelected )
								bSelectedStyle = true;
					}
				}

				if( bSelectedStyle )
				{
					rcSel.top = rc.top; rcSel.bottom = rc.bottom;
					//m_pDialog->DrawSprite( pSelElement, &rcSel );
					rcSel.right += m_pScrollBar->GetWidth();

					if ( m_bIsStaticListBox == false )
					{
						if (  ( pItem->itemType == CKTDGUIListBox::LBIT_DEFAULT ||
							pItem->itemType == CKTDGUIListBox::LBIT_NOT_ENABLE ) &&
							m_bShowTextTex == true )
						{
							FromRectToPoint(rcSel, m_TempPoint);

							//{{ 허상형 : [2009/7/22] //	채팅창 클릭 렌더의 경우 렌더 폭을 고정시켜 준다
							if( pItem->m_bMegaSelect == true || pItem->m_bMegaOver == true )
							{
								m_TempPoint.rightTopPoint.x = GetPos().x + GetWidth();
								m_TempPoint.rightBottomPoint.x = GetPos().x + GetWidth();
							}

							//}} 허상형 : [2009/7/22] //	채팅창 클릭 렌더의 경우 렌더 폭을 고정시켜 준다

							if ( m_pTextTex != NULL )
							{
								m_TempPoint.pUITextureData = m_pTextTex;
							}

							//{{ 허상형 : [2009/7/22] //	마우스 오버 텍스처
							if ( m_pButtonMouseOverTex != NULL && pItem->m_bMegaOver == true && pItem->m_bMegaSelect != true )
							{
								m_TempPoint.pUITextureData = m_pButtonMouseOverTex;
							}
							
							//}} 허상형 : [2009/7/22] //	마우스 오버 텍스처

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
							tempPos.x = (float)rc.left;
							tempPos.y = (float)rc.top;

							D3DXVECTOR2 pos;
							D3DXCOLOR	tempColor;
							D3DXCOLOR	tempOutLineColor;



							pos = tempPos  + m_pDialog->GetPos() + m_OffsetPos;		
							//pFont->Draw2DText( pItem->strText, (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );

							int offsetPosX;

							if ( pItem->itemType == CKTDGUIListBox::LBIT_DEFAULT || pItem->itemType == CKTDGUIListBox::LBIT_NOT_ENABLE )
							{
                                if(pItem->m_bColor == true)
                                    tempColor = pItem->m_color;

								if ( pItem->itemType == CKTDGUIListBox::LBIT_DEFAULT )
									tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								else
									tempColor.a = m_pStringData->color.a * 0.5f * m_pDialog->GetColor().a * m_Color.a;
 
                                if(pItem->m_bColor == false)
                                {
                                    tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
                                    tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
                                    tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;
                                }
								

								tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

								offsetPosX = (int)pos.x + (m_BlankWidthByDepth * pItem->itemDepth);
								if ( m_pUKFont != NULL )
								{
									if( true == pItem->bSpread )
									{
										wstring wstrText = pItem->strText;
										wstrText = wstrText.substr( 0, pItem->iSpreadCount );
										m_pUKFont->OutTextMultiline( offsetPosX, (int)pos.y, wstrText.c_str(), tempColor, m_pStringData->fontStyle, tempOutLineColor, 0.8f );
									}
									else
									{
										m_pUKFont->OutTextMultiline( offsetPosX, (int)pos.y, pItem->strText, tempColor, m_pStringData->fontStyle, tempOutLineColor, 0.8f );
									}
								}
							}
							else if ( pItem->itemType == CKTDGUIListBox::LBIT_GROUP_PLUS ||  
								pItem->itemType == CKTDGUIListBox::LBIT_GROUP_MINUS )
							{
								tempColor.a = m_pGroupStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								tempColor.r = m_pGroupStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
								tempColor.g = m_pGroupStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
								tempColor.b = m_pGroupStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

								tempOutLineColor.a = m_pGroupStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pGroupStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pGroupStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pGroupStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

								offsetPosX = (int)pos.x + (m_BlankWidthByDepth * pItem->itemDepth) + m_BlankWidthToGroupMark;

								D3DXVECTOR2 offsetGroupMarkPos = D3DXVECTOR2(0,0);
								offsetGroupMarkPos.x = tempPos.x + m_GroupMarkOffsetPos.x + (m_BlankWidthByDepth * pItem->itemDepth);
								offsetGroupMarkPos.y = tempPos.y + m_GroupMarkOffsetPos.y;

								if ( pItem->itemType == CKTDGUIListBox::LBIT_GROUP_PLUS )
								{
									m_pGroupMark->GetPicture( 0 )->SetShow( true );
									m_pGroupMark->GetPicture( 1 )->SetShow( false );
								}
								else
								{
									m_pGroupMark->GetPicture( 0 )->SetShow( false );
									m_pGroupMark->GetPicture( 1 )->SetShow( true );
								}

								m_pGroupMark->SetOffsetPos( offsetGroupMarkPos );
								m_pGroupMark->OnFrameRender();

								if ( m_pGroupUKFont != NULL )
								{
									if( true == pItem->bSpread )
									{
										wstring wstrText = pItem->strText;
										wstrText = wstrText.substr( 0, pItem->iSpreadCount );
										m_pGroupUKFont->OutTextMultiline( offsetPosX, (int)pos.y, wstrText.c_str(), tempColor, m_pGroupStringData->fontStyle, tempOutLineColor,  0.8f );
									}
									else
									{
										m_pGroupUKFont->OutTextMultiline( offsetPosX, (int)pos.y, pItem->strText, tempColor, m_pGroupStringData->fontStyle, tempOutLineColor,  0.8f );
									}
								}
							}
						}
					}
					else
					{
						if ( pItem->pControl != NULL )
						{
							pItem->pControl->SetOffsetPos (D3DXVECTOR2((float)rcSel.left, (float)rcSel.top) );
							pItem->pControl->OnFrameRender();
						}
					}	
				}
				else
				{
					if ( m_bIsStaticListBox == false )
					{

						if ( m_pStringData != NULL )
						{

							D3DXVECTOR2 tempPos;
							tempPos.x = (float)rc.left;
							tempPos.y = (float)rc.top;

							D3DXVECTOR2 pos;
							D3DXCOLOR	tempColor;
							D3DXCOLOR	tempOutLineColor;



							pos = tempPos  + m_pDialog->GetPos() + m_OffsetPos;		
							//pFont->Draw2DText( pItem->strText, (int)pos.x, (int)pos.y, tempColor, tempOutLineColor, m_pStringData->sortFlag, m_pStringData->fontStyle );

							int offsetPosX;
							if ( pItem->itemType == CKTDGUIListBox::LBIT_DEFAULT || pItem->itemType == CKTDGUIListBox::LBIT_NOT_ENABLE )
							{

                                if(pItem->m_bColor == true)
                                    tempColor = pItem->m_color;

								if ( pItem->itemType == CKTDGUIListBox::LBIT_DEFAULT )
									tempColor.a = m_pStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								else
									tempColor.a = m_pStringData->color.a * 0.5f * m_pDialog->GetColor().a * m_Color.a;

                                if(pItem->m_bColor == false)
                                {
                                    tempColor.r = m_pStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
                                    tempColor.g = m_pStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
                                    tempColor.b = m_pStringData->color.b * m_pDialog->GetColor().b * m_Color.b;
                                }
								

								tempOutLineColor.a = m_pStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

								offsetPosX = (int)pos.x + (m_BlankWidthByDepth * pItem->itemDepth);
								if ( m_pUKFont != NULL )
								{
									if( true == pItem->bSpread )
									{
										wstring wstrText = pItem->strText;
										wstrText = wstrText.substr( 0, pItem->iSpreadCount );
										m_pUKFont->OutTextMultiline( offsetPosX, (int)pos.y, wstrText.c_str(), tempColor, m_pStringData->fontStyle, tempOutLineColor, 0.8f );
									}
									else
									{
										m_pUKFont->OutTextMultiline( offsetPosX, (int)pos.y, pItem->strText, tempColor, m_pStringData->fontStyle, tempOutLineColor, 0.8f );

									}
								}


							}
							else if ( pItem->itemType == CKTDGUIListBox::LBIT_GROUP_PLUS ||  
								pItem->itemType == CKTDGUIListBox::LBIT_GROUP_MINUS )
							{
								tempColor.a = m_pGroupStringData->color.a * m_pDialog->GetColor().a * m_Color.a;
								tempColor.r = m_pGroupStringData->color.r * m_pDialog->GetColor().r * m_Color.r;
								tempColor.g = m_pGroupStringData->color.g * m_pDialog->GetColor().g * m_Color.g;
								tempColor.b = m_pGroupStringData->color.b * m_pDialog->GetColor().b * m_Color.b;

								tempOutLineColor.a = m_pGroupStringData->outlineColor.a * m_pDialog->GetColor().a * m_Color.a; 
								tempOutLineColor.r = m_pGroupStringData->outlineColor.r * m_pDialog->GetColor().r * m_Color.r;
								tempOutLineColor.g = m_pGroupStringData->outlineColor.g * m_pDialog->GetColor().g * m_Color.g;
								tempOutLineColor.b = m_pGroupStringData->outlineColor.b * m_pDialog->GetColor().b * m_Color.b;

								offsetPosX = (int)pos.x + (m_BlankWidthByDepth * pItem->itemDepth) + m_BlankWidthToGroupMark;

								D3DXVECTOR2 offsetGroupMarkPos = D3DXVECTOR2(0,0);
								offsetGroupMarkPos.x = tempPos.x + m_GroupMarkOffsetPos.x + (m_BlankWidthByDepth * pItem->itemDepth);
								offsetGroupMarkPos.y = tempPos.y + m_GroupMarkOffsetPos.y;

								m_pGroupMark->SetOffsetPos( offsetGroupMarkPos );

								if ( pItem->itemType == CKTDGUIListBox::LBIT_GROUP_PLUS )
								{
									m_pGroupMark->GetPicture( 0 )->SetShow( true );
									m_pGroupMark->GetPicture( 1 )->SetShow( false );
								}
								else
								{
									m_pGroupMark->GetPicture( 0 )->SetShow( false );
									m_pGroupMark->GetPicture( 1 )->SetShow( true );
								}


								m_pGroupMark->OnFrameRender();

								if ( m_pGroupUKFont != NULL )
								{

									if( true == pItem->bSpread )
									{
										wstring wstrText = pItem->strText;
										wstrText = wstrText.substr( 0, pItem->iSpreadCount );
										m_pGroupUKFont->OutTextMultiline( offsetPosX, (int)pos.y, wstrText.c_str(), tempColor, m_pGroupStringData->fontStyle, tempOutLineColor, 0.8f );
									}
									else
									{
										m_pGroupUKFont->OutTextMultiline( offsetPosX, (int)pos.y, pItem->strText, tempColor, m_pGroupStringData->fontStyle, tempOutLineColor, 0.8f );
									}
								}
							}
						}
					}
					else
					{

						if ( pItem->pControl != NULL )
						{
							pItem->pControl->SetOffsetPos (D3DXVECTOR2((float)rc.left, (float)rc.top) );
							pItem->pControl->OnFrameRender();
						}
					}	
				}
				OffsetRect( &rc, 0, m_nTextHeight );
			}
		}
	}

	// Render the scroll bar
	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->OnFrameRender();
	}


	return S_OK;
}



//--------------------------------------------------------------------------------------
void CKTDGUIListBox::UpdateRects()
{
	KTDXPROFILE();

	SetRect( &m_rcBoundingBox, m_x, m_y, m_x + m_width, m_y + m_height );

	m_rcSelection = m_rcBoundingBox;
	m_rcSelection.right -= m_nSBWidth;
	
	if ( m_bIsStaticListBox == false )
		InflateRect( &m_rcSelection, -m_nBorder, -m_nBorder );
	
	m_rcText = m_rcSelection;
	
	if ( m_bIsStaticListBox == false )
		InflateRect( &m_rcText, -m_nMargin, 0 );

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex ); 
	if ( pFont == NULL )
	{
		MessageBox(g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_954 ),  GET_STRING( STR_ID_955 ), MB_OK );
		return;
	}


	// Update the scrollbar's rects
	if ( m_pScrollBar != NULL && true == m_bUpdateScrollBarOnUpdate )
	{
		m_pScrollBar->SetLocation( m_rcBoundingBox.right, m_rcBoundingBox.top );
		m_pScrollBar->SetSize( m_nSBWidth, m_height );
	}
	
	if( pFont != NULL && m_pScrollBar != NULL)
	{
		if ( m_bIsStaticListBox == false )
			m_pScrollBar->SetPageSize( RectHeight( m_rcText ) / pFont->GetHeight() );
		else
			m_pScrollBar->SetPageSize( RectHeight( m_rcText ) / m_nTextHeight );

		if( false == m_bScrollToEndOnUpdateRects )
		{
			// The selected item may have been scrolled off the page.
			// Ensure that it is in page again.
			m_pScrollBar->ShowItem( m_nSelected );
		}
		else
		{
			SetScrollBarEndPos();
		}
	}
}


void CKTDGUIListBox::SetDialog( CKTDGUIDialogType pDialog )
{
	KTDXPROFILE();

	CKTDGUIControl::SetDialog( pDialog );
	m_pGroupMark->SetDialog( pDialog );
}

//--------------------------------------------------------------------------------------
bool CKTDGUIListBox::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;

	if ( m_bShow == false || m_bEnable == false )
	{
		return false;
	}

	return false;
}


//--------------------------------------------------------------------------------------
bool CKTDGUIListBox::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();

	if ( m_bShow == false || m_bEnable == false )
	{
		return false;
	}

	// First acquire focus
	if( WM_LBUTTONDOWN == uMsg )
		if( !m_bHasFocus )
			m_pDialog->RequestFocus( this );

	// Let the scroll bar handle it first.
	if( m_pScrollBar != NULL  )
	{
		if ( m_pScrollBar->HandleMouse( uMsg, pt, wParam, lParam ) == true )
		{
			if( true == m_bUseItemDisappear )
			{
				//{{ 허상형 : [2009/7/24] //	MAGIC_SHOW_TIME 20.0f -> 50.0f
				const float MAGIC_SHOW_TIME = 50.f;
				//}} 허상형 : [2009/7/24] //
				SetShowAllItems();
				SetShowTimeAllItems( MAGIC_SHOW_TIME );
			}
			return true;
		}
	}

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if ( PtInRect( &m_rcSelection, pt ) == TRUE )
			{
				// Check for clicks in the text area
				if( m_Items.GetSize() > 0 )
				{
					// Compute the index of the clicked item

					int nClicked;
					if( m_nTextHeight && m_pScrollBar != NULL )
						nClicked = m_pScrollBar->GetTrackPos() + ( pt.y - m_rcText.top ) / m_nTextHeight;
					else
						nClicked = -1;

					CKTDGUIListBox::ListBoxItem* pListBoxItem = GetItem( nClicked );
					if ( pListBoxItem == NULL )
						return true;

					// Only proceed if the click falls on top of an item.

					if( m_pScrollBar != NULL && nClicked >= m_pScrollBar->GetTrackPos() &&
						nClicked < (int)m_Items.GetSize() &&
						nClicked < m_pScrollBar->GetTrackPos() + m_pScrollBar->GetPageSize() )
					{
#ifndef DLL_BUILD
						SetCapture( DXUTGetHWND() );
#endif
						m_bDrag = true;

						// If this is a double click, fire off an event and exit
						// since the first click would have taken care of the selection
						// updating.
						if( uMsg == WM_LBUTTONDBLCLK )
						{

							//if ( m_pSndMouseDown != NULL )
							//	m_pSndMouseDown->Play();

							PlaySound( m_pSndMouseDown );

							if( m_CustomMsgItemDoubleClick != -1 )
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgItemDoubleClick, (LPARAM)this );
							}
							else
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_ITEM_DBLCLK, (LPARAM)this );
							}
							return true;

						}

						m_nSelected = nClicked;
						if( !( wParam & MK_SHIFT ) )
							m_nSelStart = m_nSelected;

						// If this is a multi-selection listbox, update per-item
						// selection data.

						if( m_dwStyle & MULTISELECTION )
						{
							// Determine behavior based on the state of Shift and Ctrl

							ListBoxItem *pSelItem = m_Items.GetAt( m_nSelected );
							if( ( wParam & (MK_SHIFT|MK_CONTROL) ) == MK_CONTROL )
							{
								// Control click. Reverse the selection of this item.

								pSelItem->bSelected = !pSelItem->bSelected;
							} else
								if( ( wParam & (MK_SHIFT|MK_CONTROL) ) == MK_SHIFT )
								{
									// Shift click. Set the selection for all items
									// from last selected item to the current item.
									// Clear everything else.

									int nBegin = __min( m_nSelStart, m_nSelected );
									int nEnd = __max( m_nSelStart, m_nSelected );

									for( int i = 0; i < nBegin; ++i )
									{
										ListBoxItem *pItem = m_Items.GetAt( i );
										pItem->bSelected = false;
									}

									for( int i = nEnd + 1; i < (int)m_Items.GetSize(); ++i )
									{
										ListBoxItem *pItem = m_Items.GetAt( i );
										pItem->bSelected = false;
									}

									for( int i = nBegin; i <= nEnd; ++i )
									{
										ListBoxItem *pItem = m_Items.GetAt( i );
										pItem->bSelected = true;
									}
								} else
									if( ( wParam & (MK_SHIFT|MK_CONTROL) ) == ( MK_SHIFT|MK_CONTROL ) )
									{
										// Control-Shift-click.

										// The behavior is:
										//   Set all items from m_nSelStart to m_nSelected to
										//     the same state as m_nSelStart, not including m_nSelected.
										//   Set m_nSelected to selected.

										int nBegin = __min( m_nSelStart, m_nSelected );
										int nEnd = __max( m_nSelStart, m_nSelected );

										// The two ends do not need to be set here.

										bool bLastSelected = m_Items.GetAt( m_nSelStart )->bSelected;
										for( int i = nBegin + 1; i < nEnd; ++i )
										{
											ListBoxItem *pItem = m_Items.GetAt( i );
											pItem->bSelected = bLastSelected;
										}

										pSelItem->bSelected = true;

										// Restore m_nSelected to the previous value
										// This matches the Windows behavior

										m_nSelected = m_nSelStart;
									} else
									{
										// Simple click.  Clear all items and select the clicked
										// item.


										for( int i = 0; i < (int)m_Items.GetSize(); ++i )
										{
											ListBoxItem *pItem = m_Items.GetAt( i );
											pItem->bSelected = false;
										}

										pSelItem->bSelected = true;
									}
						}  // End of multi-selection case

						//if ( m_pSndMouseDown != NULL )
						//	m_pSndMouseDown->Play();

						PlaySound( m_pSndMouseDown );

						if( m_CustomMsgSelection != -1 )
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection, (LPARAM)this );
						}
						else
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION, (LPARAM)this );
						}
					}
				}
				else
				{
					m_nSelected = -1;
				}

				// 텍스트 창을 클릭했을 경우, 메세지를 통과시킴

				//return false;
				if ( m_bIgnoreListClick == true )
					return false;

				return true;
			}
		}
		break;

	case WM_RBUTTONDOWN:
		{
			if ( PtInRect( &m_rcSelection, pt ) == TRUE )
			{
				// Check for clicks in the text area
				if( m_Items.GetSize() > 0 )
				{
					// Compute the index of the clicked item

					int nClicked;
					if( m_nTextHeight && m_pScrollBar != NULL )
						nClicked = m_pScrollBar->GetTrackPos() + ( pt.y - m_rcText.top ) / m_nTextHeight;
					else
						nClicked = -1;

					CKTDGUIListBox::ListBoxItem* pListBoxItem = GetItem( nClicked );
					if ( pListBoxItem == NULL )
						return true;
						

					// Only proceed if the click falls on top of an item.

					if( m_pScrollBar != NULL && nClicked >= m_pScrollBar->GetTrackPos() &&
						nClicked < (int)m_Items.GetSize() &&
						nClicked < m_pScrollBar->GetTrackPos() + m_pScrollBar->GetPageSize() )
					{
#ifndef DLL_BUILD
						SetCapture( DXUTGetHWND() );
#endif
						m_bDrag = true;

						m_nSelected = nClicked;
						if( !( wParam & MK_SHIFT ) )
							m_nSelStart = m_nSelected;

						// If this is a multi-selection listbox, update per-item
						// selection data.

						if( m_dwStyle & MULTISELECTION )
						{
							// Determine behavior based on the state of Shift and Ctrl

							ListBoxItem *pSelItem = m_Items.GetAt( m_nSelected );
							if( ( wParam & (MK_SHIFT|MK_CONTROL) ) == MK_CONTROL )
							{
								// Control click. Reverse the selection of this item.

								pSelItem->bSelected = !pSelItem->bSelected;
							} else
								if( ( wParam & (MK_SHIFT|MK_CONTROL) ) == MK_SHIFT )
								{
									// Shift click. Set the selection for all items
									// from last selected item to the current item.
									// Clear everything else.

									int nBegin = __min( m_nSelStart, m_nSelected );
									int nEnd = __max( m_nSelStart, m_nSelected );

									for( int i = 0; i < nBegin; ++i )
									{
										ListBoxItem *pItem = m_Items.GetAt( i );
										pItem->bSelected = false;
									}

									for( int i = nEnd + 1; i < (int)m_Items.GetSize(); ++i )
									{
										ListBoxItem *pItem = m_Items.GetAt( i );
										pItem->bSelected = false;
									}

									for( int i = nBegin; i <= nEnd; ++i )
									{
										ListBoxItem *pItem = m_Items.GetAt( i );
										pItem->bSelected = true;
									}
								} else
									if( ( wParam & (MK_SHIFT|MK_CONTROL) ) == ( MK_SHIFT|MK_CONTROL ) )
									{
										// Control-Shift-click.

										// The behavior is:
										//   Set all items from m_nSelStart to m_nSelected to
										//     the same state as m_nSelStart, not including m_nSelected.
										//   Set m_nSelected to selected.

										int nBegin = __min( m_nSelStart, m_nSelected );
										int nEnd = __max( m_nSelStart, m_nSelected );

										// The two ends do not need to be set here.

										bool bLastSelected = m_Items.GetAt( m_nSelStart )->bSelected;
										for( int i = nBegin + 1; i < nEnd; ++i )
										{
											ListBoxItem *pItem = m_Items.GetAt( i );
											pItem->bSelected = bLastSelected;
										}

										pSelItem->bSelected = true;

										// Restore m_nSelected to the previous value
										// This matches the Windows behavior

										m_nSelected = m_nSelStart;
									} else
									{
										// Simple click.  Clear all items and select the clicked
										// item.


										for( int i = 0; i < (int)m_Items.GetSize(); ++i )
										{
											ListBoxItem *pItem = m_Items.GetAt( i );
											pItem->bSelected = false;
										}

										pSelItem->bSelected = true;
									}
						}  // End of multi-selection case

						//if ( m_pSndMouseDown != NULL )
						//	m_pSndMouseDown->Play();

						PlaySound( m_pSndMouseDown );

						if( m_CustomMsgSelectionByRButton != -1 )
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionByRButton, (LPARAM)this );
						}
						else
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION_BY_RBUTTON, (LPARAM)this );
						}
					}
				}
				else
				{
					m_nSelected = -1;
				}

				/*
				// 텍스트 창을 클릭했을 경우, 메세지를 통과시킴

				//return false;
				if ( m_bIgnoreListClick == true )
					return false;
*/
				return true;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			m_bDrag = false;

			if( m_nSelected != -1 )
			{
				// Set all items between m_nSelStart and m_nSelected to
				// the same state as m_nSelStart
				int nEnd = __max( m_nSelStart, m_nSelected );

				if ( nEnd >= m_Items.GetSize() || NULL == m_Items[m_nSelected] || NULL == m_Items[m_nSelStart] )
					return false;

				for( int n = __min( m_nSelStart, m_nSelected ) + 1; n < nEnd; ++n )
				{
					if ( n < m_Items.GetSize() && NULL != m_Items[n] )
						m_Items[n]->bSelected = m_Items[m_nSelStart]->bSelected;
				}
				m_Items[m_nSelected]->bSelected = m_Items[m_nSelStart]->bSelected;

				// If m_nSelStart and m_nSelected are not the same,
				// the user has dragged the mouse to make a selection.
				// Notify the application of this.
				if( m_nSelStart != m_nSelected )
				{
					if( m_CustomMsgSelection != -1 )
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection, (LPARAM)this );
					}
					else
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION, (LPARAM)this );
					}
				}

				if( m_CustomMsgSelection_End != -1 )
				{
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection_End, (LPARAM)this );
				}
				else
				{
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION_END, (LPARAM)this );
				}
			}
			return false;

		
		}
		break;

	case WM_RBUTTONUP:
		{
			ReleaseCapture();
			m_bDrag = false;

			if( m_nSelected != -1 )
			{
				// Set all items between m_nSelStart and m_nSelected to
				// the same state as m_nSelStart
				int nEnd = __max( m_nSelStart, m_nSelected );

				if ( nEnd >= m_Items.GetSize() || NULL == m_Items[m_nSelected] || NULL == m_Items[m_nSelStart] )
					return false;

				for( int n = __min( m_nSelStart, m_nSelected ) + 1; n < nEnd; ++n )
				{
					if ( n < m_Items.GetSize() && NULL != m_Items[n] )
						m_Items[n]->bSelected = m_Items[m_nSelStart]->bSelected;
				}
				m_Items[m_nSelected]->bSelected = m_Items[m_nSelStart]->bSelected;

				// If m_nSelStart and m_nSelected are not the same,
				// the user has dragged the mouse to make a selection.
				// Notify the application of this.
				if( m_nSelStart != m_nSelected )
				{
					if( m_CustomMsgSelection != -1 )
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelectionByRButton, (LPARAM)this );
					}
					else
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION_BY_RBUTTON, (LPARAM)this );
					}
				}
			}
			return false;


		}
		break;

	case WM_MOUSEMOVE:
		{
#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
			/// 마우스 오버를 통해 아이템 선택 가능하게 하도록 설정시, 해당 기능 연산
			if ( true == m_bIsSelectItemAtMouseMove )
			{
				if ( PtInRect( &m_rcSelection, pt ) == TRUE && m_Items.GetSize() > 0 )
				// Check for clicks in the text area
				{
					int iMouseOverIndex;

					if( m_nTextHeight && m_pScrollBar != NULL )
						iMouseOverIndex = m_pScrollBar->GetTrackPos() + ( pt.y - m_rcText.top ) / m_nTextHeight;
					else
						iMouseOverIndex = -1;

					CKTDGUIListBox::ListBoxItem* pListBoxItem = GetItem( iMouseOverIndex );
					if ( pListBoxItem == NULL )
						return true;

					// Only proceed if the click falls on top of an item.

					if( m_pScrollBar != NULL && iMouseOverIndex >= m_pScrollBar->GetTrackPos() &&
						iMouseOverIndex < (int)m_Items.GetSize() &&
						iMouseOverIndex < m_pScrollBar->GetTrackPos() + m_pScrollBar->GetPageSize() )
					{
						m_nSelected = iMouseOverIndex;
					}
				}
				else
				{
					m_nSelected = -1;
				}
			}
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
		/*	
			if( m_bDrag )
			{
				// Compute the index of the item below cursor

				int nItem;
				if( m_nTextHeight && m_pScrollBar != NULL )
					nItem = m_pScrollBar->GetTrackPos() + ( pt.y - m_rcText.top ) / m_nTextHeight;
				else
					nItem = -1;

				// Only proceed if the cursor is on top of an item.

				if( m_pScrollBar != NULL && nItem >= (int)m_pScrollBar->GetTrackPos() &&
					nItem < (int)m_Items.GetSize() &&
					nItem < m_pScrollBar->GetTrackPos() + m_pScrollBar->GetPageSize() )
				{
					m_nSelected = nItem;

					if( m_CustomMsgSelection != -1 )
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection, (LPARAM)this );
					}
					else
					{
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION, (LPARAM)this );
					}

				} else
					if( m_pScrollBar != NULL && nItem < (int)m_pScrollBar->GetTrackPos() )
					{
						// User drags the mouse above window top
						m_pScrollBar->Scroll( -1 );
						m_nSelected = m_pScrollBar->GetTrackPos();

						if( m_CustomMsgSelection != -1 )
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection, (LPARAM)this );
						}
						else
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION, (LPARAM)this );
						}

					} else
						if( m_pScrollBar != NULL && nItem >= m_pScrollBar->GetTrackPos() + m_pScrollBar->GetPageSize() )
						{
							// User drags the mouse below window bottom
							m_pScrollBar->Scroll( 1 );
							m_nSelected = __min( (int)m_Items.GetSize(), m_pScrollBar->GetTrackPos() + m_pScrollBar->GetPageSize() ) - 1;

							if( m_CustomMsgSelection != -1 )
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection, (LPARAM)this );
							}
							else
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION, (LPARAM)this );
							}
						}
			}
*/
			if ( PtInRect( &m_rcSelection, pt ) == TRUE )
			{
				return true;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			UINT uLines;
			SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &uLines, 0 );
			int nScrollAmount = int((short)HIWORD(wParam)) / WHEEL_DELTA * uLines;
			if ( m_pScrollBar != NULL )
			{
				m_pScrollBar->Scroll( -nScrollAmount );
			}
			return true;
		}
		break;
	}

	CKTDGUIControl*	pTempControl = NULL;
	for ( int i = 0; i < (int)m_Items.GetSize(); i++ )
	{
		pTempControl = m_Items[i]->pControl;

		if ( pTempControl != NULL )
		{
			pTempControl->HandleMouse(uMsg, pt, wParam, lParam);
		}
	}

	return false;
}


//--------------------------------------------------------------------------------------
bool CKTDGUIListBox::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( WM_CAPTURECHANGED == uMsg )
	{
		// The application just lost mouse capture. We may not have gotten
		// the WM_MOUSEUP message, so reset m_bDrag here.
		m_bDrag = false;
	}

	return false;
}





void CKTDGUIListBox::AddItem_LUA( const char* pName )
{
	wstring WName;
	ConvertUtf8ToWCHAR( WName, pName );

	AddItem(WName.c_str(), NULL);

}

void CKTDGUIListBox::FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point )
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
HRESULT CKTDGUIListBox::AddString(const WCHAR* wszText, D3DXCOLOR aColor, void* pData, CKTDGUIControl* pControl)
{
    ListBoxItem *pNewItem = new ListBoxItem;
    if( !pNewItem )
        return E_OUTOFMEMORY;

    StringCchCopy( pNewItem->strText, 256, wszText );
    pNewItem->pData = pData;
    SetRect( &pNewItem->rcActive, 0, 0, 0, 0 );
    pNewItem->bSelected = false;
    pNewItem->itemDepth = 0;
    pNewItem->itemType = LBIT_DEFAULT;
    pNewItem->bSpread = false;
    pNewItem->fSpreadTimeGap = 0.f;
    pNewItem->m_color = aColor;
    pNewItem->m_bColor = true;
    pNewItem->pControl = pControl;

    HRESULT hr = m_Items.Add( pNewItem );
    if( FAILED(hr) )
    {
        SAFE_DELETE( pNewItem );
    }
    else
    {
        if ( m_pScrollBar != NULL )
        {
            m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
        }
    }

    return hr;
}

HRESULT CKTDGUIListBox::AddItem( const WCHAR *wszText, void *pData, int itemDepth /* = 0 */, CKTDGUIListBox::LIST_BOX_ITEM_TYPE itemType /* = LBIT_DEFAULT */,
								bool bSpread /*= false*/, float fSpreadTime /*= 0.f*/ )
{
	ListBoxItem *pNewItem = new ListBoxItem;
	if( !pNewItem )
		return E_OUTOFMEMORY;

	StringCchCopy( pNewItem->strText, 256, wszText );
	pNewItem->pData = pData;
	SetRect( &pNewItem->rcActive, 0, 0, 0, 0 );
	pNewItem->bSelected = false;
	pNewItem->itemDepth = itemDepth;
	pNewItem->itemType = itemType;
	pNewItem->bSpread = bSpread;
	pNewItem->fSpreadTimeGap = fSpreadTime;    
    pNewItem->m_color = D3DXCOLOR(0xffffffff);
    pNewItem->m_bColor = false;

	HRESULT hr = m_Items.Add( pNewItem );
	if( FAILED(hr) )
	{
		SAFE_DELETE( pNewItem );
	}
	else
	{
		if ( m_pScrollBar != NULL )
		{
			m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
		}
	}

	return hr;
}

//{{ 허상형 : [2009/7/20] //	메가폰 아이템 추가
HRESULT CKTDGUIListBox::AddItem( const WCHAR *wszText, void *pData, int iMessageID, wstring wstrName, bool bIsMine)
{
	ListBoxItem *pNewItem = new ListBoxItem;
	if( !pNewItem )
		return E_OUTOFMEMORY;

	StringCchCopy( pNewItem->strText, 256, wszText );
	pNewItem->pData = pData;
	SetRect( &pNewItem->rcActive, 0, 0, 0, 0 );
	pNewItem->bSelected = false;
	pNewItem->itemDepth = 0;
	pNewItem->itemType = LBIT_DEFAULT;
	pNewItem->bSpread = false;
	pNewItem->fSpreadTimeGap = 0.f;    
	pNewItem->m_color = D3DXCOLOR(0xffffffff);
	pNewItem->m_bColor = false;
	pNewItem->m_iMessageID = iMessageID;
	pNewItem->m_wstrData = wstrName;
	pNewItem->m_bMyMessage = bIsMine;

	HRESULT hr = m_Items.Add( pNewItem );
	if( FAILED(hr) )
	{
		SAFE_DELETE( pNewItem );
	}
	else
	{
		if ( m_pScrollBar != NULL )
		{
			m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
		}
	}

	return hr;
}
//}} 허상형 : [2009/7/20] //

//bool CKTDGUIListBox::OpenItemScript( const WCHAR* pFileName )
//{
//	if( NULL == pFileName )
//		return false;
//
//	//바인더에 등록
//
//	//lua_getfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIDialog" );
//	//int iRefUIDialog = lua_ref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), true );
//	//lua_pushnil( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIDialog" );
//
//	//lua_getfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIListBox" );
//	//int iRefUIListBox = lua_ref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), true );
//	//lua_pushnil( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIListBox" );
//
//
//	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIListBox", this );
//	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIDialog", m_pDialog );
//
//	bool bResult = g_pKTDXApp->LoadLuaTinker( pFileName );
//
//
//
//	//lua_getref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIListBox );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIListBox" );
//	//lua_unref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIListBox );
//
//
//	//lua_getref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIDialog );
//	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pUIDialog" );
//	//lua_unref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefUIDialog );
//
//	
//	return bResult;
//}

HRESULT CKTDGUIListBox::AddControl(  CKTDGUIItem* pItem )
{
	ListBoxItem *pNewItem = new ListBoxItem;

	StringCchCopy( pNewItem->strText, 256, pItem->GetName() );
	pNewItem->pData = NULL;
	SetRect( &pNewItem->rcActive, 0, 0, 0, 0 );
	pNewItem->bSelected = false;

	pItem->SetDialog(m_pDialog);
	pNewItem->pControl = pItem;

	HRESULT hr = m_Items.Add( pNewItem );
	if( FAILED(hr) )
	{
		SAFE_DELETE( pNewItem );
	}
	else
	{
		if ( m_pScrollBar != NULL )
		{
			m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
		}
	}

	return hr;
}


//--------------------------------------------------------------------------------------
HRESULT CKTDGUIListBox::InsertItem( int nIndex, const WCHAR *wszText, void *pData )
{
	ListBoxItem *pNewItem = new ListBoxItem;
	if( !pNewItem )
		return E_OUTOFMEMORY;

	StringCchCopy( pNewItem->strText, 256, wszText );
	pNewItem->pData = pData;
	SetRect( &pNewItem->rcActive, 0, 0, 0, 0 );
	pNewItem->bSelected = false;

	HRESULT hr = m_Items.Insert( nIndex, pNewItem );
	if( SUCCEEDED( hr ) )
	{
		if ( m_pScrollBar != NULL )
		{
			m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
		}
	}
	else
		SAFE_DELETE( pNewItem );

	return hr;
}


//--------------------------------------------------------------------------------------
void CKTDGUIListBox::RemoveItem( int nIndex )
{
	if( nIndex < 0 || nIndex >= (int)m_Items.GetSize() )
		return;

	ListBoxItem *pItem = m_Items.GetAt( nIndex );

	delete pItem;
	m_Items.Remove( nIndex );
	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->SetTrackRange( 0, m_Items.GetSize() );
	}
	if( m_nSelected >= (int)m_Items.GetSize() )
		m_nSelected = m_Items.GetSize() - 1;

	if( m_CustomMsgSelection != -1 )
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection, (LPARAM)this );
	}
	else
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION, (LPARAM)this );
	}
}


//--------------------------------------------------------------------------------------
int CKTDGUIListBox::RemoveItemByText( WCHAR *wszText )
{
	int nRemovedCount = 0;
	for( int i=0; i<m_Items.GetSize(); i++ )
	{
		CKTDGUIListBox::ListBoxItem* pItem = m_Items[i];
		if( 0 == wcscmp( pItem->strText, wszText ) )
		{
			RemoveItem(i);
			i--;
			nRemovedCount++;
		}
	}

	return nRemovedCount;
}


//--------------------------------------------------------------------------------------
void CKTDGUIListBox::RemoveItemByData( void *pData )
{
}

void CKTDGUIListBox::SetBoundingBoxTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pBoudingBoxTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pBoudingBoxTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void CKTDGUIListBox::SetTextTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pTextTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pTextTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}



void	CKTDGUIListBox::SetScollBarUpButtonTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pScollBarUpButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarUpButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIListBox::SetScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pScollBarUpButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarUpButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIListBox::SetScollBarDownButtonTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pScollBarDownButtonTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarDownButtonTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIListBox::SetScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pScollBarDownButtonDownTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarDownButtonDownTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIListBox::SetScollBarTrackTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pScollBarTrackTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarTrackTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

void	CKTDGUIListBox::SetScollBarThumbTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( m_pScollBarThumbTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pScollBarThumbTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
}

//{{ 허상형 : [2009/7/21] //	마우스오버 텍스쳐 루아 함수
void CKTDGUIListBox::SetButtonMouseOverTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pButtonMouseOverTex );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pButtonMouseOverTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );

}
//}} 허상형 : [2009/7/21] //	마우스오버 텍스쳐 루아 함수

void CKTDGUIListBox::SetScrollBarTrackRange( int nStart, int nEnd )
{
	KTDXPROFILE();

	if ( m_pScrollBar == NULL )
		return;

	m_pScrollBar->SetTrackRange( nStart, nEnd );
}

void CKTDGUIListBox::SetScrollBarTrackPos( int nPosition )
{
	if ( m_pScrollBar == NULL )
		return;

	m_pScrollBar->SetTrackPos( nPosition );
}

void CKTDGUIListBox::SetScrollBarEndPos()
{
	if ( m_pScrollBar == NULL )
		return;

	m_pScrollBar->SetTrankEndPos();
}


//void CKTDGUIListBox::SetItemScriptName_LUA( const char* fileName )
//{
//	ConvertUtf8ToWCHAR( m_ItemScriptName, fileName );
//	OpenItemScript( m_ItemScriptName.c_str() );
//}

void CKTDGUIListBox::SetShowAllItems( bool bShow /*= true*/  )
{
	for( int i = 0; i < m_Items.GetSize(); i++ )
	{
		ListBoxItem* pItem = m_Items.GetAt( i );
		if( NULL != pItem )
		{
			pItem->m_bShow = bShow;
			pItem->m_fTimeLeftShow = -1.f;
		}
	}
}


void CKTDGUIListBox::SetShowTimeAllItems( float fTimeLeft )
{
	for( int i = 0; i < m_Items.GetSize(); i++ )
	{
		ListBoxItem* pItem = m_Items.GetAt( i );
		if( NULL != pItem )
		{
			pItem->m_fTimeLeftShow = fTimeLeft;
		}
	}
}


//--------------------------------------------------------------------------------------
void CKTDGUIListBox::RemoveAllItems()
{
	for( int i = 0; i < m_Items.GetSize(); ++i )
	{
		ListBoxItem* pItem = m_Items.GetAt( i );
		delete pItem;
	}

	m_Items.RemoveAll();
	if ( m_pScrollBar != NULL )
	{
		m_pScrollBar->SetTrackRange( 0, 1 );
	}

	m_nSelected = -1;
}
//--------------------------------------------------------------------------------------
CKTDGUIListBox::ListBoxItem* CKTDGUIListBox::GetItem( int nIndex )
{
	KTDXPROFILE();

	if( nIndex < 0 || nIndex >= (int)m_Items.GetSize() )
		return NULL;

	return m_Items[nIndex];
}

CKTDGUIListBox::ListBoxItem* CKTDGUIListBox::GetLastItem()
{
	if( m_Items.GetSize() > 0 )
	{
		return m_Items.GetAt( m_Items.GetSize()-1 );
	}
	else
	{
		return NULL;
	}
}



//--------------------------------------------------------------------------------------
// For single-selection listbox, returns the index of the selected item.
// For multi-selection, returns the first selected item after the nPreviousSelected position.
// To search for the first selected item, the app passes -1 for nPreviousSelected.  For
// subsequent searches, the app passes the returned index back to GetSelectedIndex as.
// nPreviousSelected.
// Returns -1 on error or if no item is selected.
int CKTDGUIListBox::GetSelectedIndex( int nPreviousSelected )
{
	if( nPreviousSelected < -1 )
		return -1;

	if( m_dwStyle & MULTISELECTION )
	{
		// Multiple selection enabled. Search for the next item with the selected flag.
		for( int i = nPreviousSelected + 1; i < (int)m_Items.GetSize(); ++i )
		{
			ListBoxItem* pItem = m_Items.GetAt( i );

			if( pItem->bSelected )
				return i;
		}

		return -1;
	}
	else
	{
		// Single selection
		return m_nSelected;
	}
}


//--------------------------------------------------------------------------------------
#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
/// 선택 아이템 설정 후, 메시지 전송 시킬지 여부 추가를 위해 수정
void CKTDGUIListBox::SelectItem( IN int nNewIndex, IN bool bSendMessage /*= true*/ )
#else // UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
void CKTDGUIListBox::SelectItem( int nNewIndex )
#endif // UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
{
	KTDXPROFILE();

	// If no item exists, do nothing.
	if( m_Items.GetSize() == 0 )
		return;

	int nOldSelected = m_nSelected;

	// Adjust m_nSelected
	m_nSelected = nNewIndex;

	// Perform capping
	if( m_nSelected < 0 )
		m_nSelected = 0;
	if( m_nSelected >= (int)m_Items.GetSize() )
		m_nSelected = m_Items.GetSize() - 1;

	if( nOldSelected != m_nSelected )
	{
		if( m_dwStyle & MULTISELECTION )
		{
			m_Items[m_nSelected]->bSelected = true;
		}

		// Update selection start
		m_nSelStart = m_nSelected;

		// Adjust scroll bar

		if ( m_pScrollBar != NULL )
		{
			m_pScrollBar->ShowItem( m_nSelected );
		}
	}

#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
	/// 선택 아이템 설정 후, 메시지 전송 여부에 따라 처리
	if ( true == bSendMessage )
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
	{
		if( m_CustomMsgSelection != -1 )
		{
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgSelection, (LPARAM)this );
		}
		else
		{
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, LBEM_SELECTION, (LPARAM)this );
		}
	}
}


void CKTDGUIListBox::SetString( CKTDGUIControl::UIStringData* pString )
{
	if ( m_pStringData != NULL )
		SAFE_DELETE( m_pStringData );
/*
	if ( m_pUKFont != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pUKFont );
	}
*/
	m_pStringData = pString;
	m_pUKFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pStringData->fontIndex ); 

	//m_pUKFont = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUKFont( pFont->GetFontName(), pFont->GetHeight(), 1, true, pFont->GetWeight()  );	
}

void CKTDGUIListBox::SetGroupString( CKTDGUIControl::UIStringData* pString )
{
	if ( m_pGroupStringData != NULL )
		SAFE_DELETE( m_pGroupStringData );

	m_pGroupStringData = pString;
	m_pGroupUKFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( m_pGroupStringData->fontIndex ); 
}



void CKTDGUIListBox::SetMaximize( bool val )
{
	CKTDGUIControl::SetMaximize( val );

	int iSign = 1;
	if ( val == false )
		iSign = -1;

	D3DXVECTOR2 kPos = GetPos();
	D3DXVECTOR2 kSize;
	kSize.x = (float) GetWidth();
	kSize.y = (float) GetHeight();

	SetPos_LUA( (int) kPos.x - m_kMaximizeOffset.x * iSign, (int) kPos.y - m_kMaximizeOffset.y * iSign );
	SetSize_LUA( (int) kSize.x + m_kMaximizeOffset.x * iSign, (int) kSize.y + m_kMaximizeOffset.y * iSign );
}


bool CKTDGUIListBox::ContainsPoint( POINT pt )
{	
	/*
	D3DXVECTOR2 arPoint[4];

	arPoint[0] = m_NowPoint.leftTopPoint + m_OffsetPos;
	arPoint[1] = m_NowPoint.rightTopPoint + m_OffsetPos;
	arPoint[2] = m_NowPoint.leftBottomPoint + m_OffsetPos;
	arPoint[3] = m_NowPoint.rightBottomPoint + m_OffsetPos;

	return Pick2DRect( pt, arPoint[0], arPoint[1], arPoint[2], arPoint[3] ); */

	if( true == Pick2DRect( pt, m_NowPoint.leftTopPoint, m_NowPoint.rightTopPoint, m_NowPoint.leftBottomPoint ,m_NowPoint.rightBottomPoint ) )
		return true;


	RECT rect = m_pScrollBar->GetBoundingBox();
	D3DXVECTOR2 vLeftTop( (float) rect.left, (float) rect.top );
	D3DXVECTOR2 vRightTop( (float) rect.right, (float) rect.top );
	D3DXVECTOR2 vLeftBottom( (float) rect.left, (float) rect.bottom );
	D3DXVECTOR2 vRightBottom( (float) rect.right, (float) rect.bottom );
		
	return Pick2DRect( pt, vLeftTop, vRightTop, vLeftBottom, vRightBottom );
}

//{{ 허상형 : [2009/7/22] //	선택 해제 함수
void	CKTDGUIListBox::ClearSelect()
{ 
	m_nSelected = -1; 

	for(int i = 0; i < GetSize(); i++)
	{
		ListBoxItem *pItem = GetItem( i );
		if(pItem != NULL)
		{
			pItem->m_bMegaSelect = false;
		}
	}
}
//}} 허상형 : [2009/7/22] //	선택 해제 함수

#ifdef DLL_BUILD
void CKTDGUIListBox::MoveControl( float fx, float fy )
{
	m_x += fx;
	m_y += fy;

	UpdateRects();
}

void CKTDGUIListBox::SetEditGUI( bool bEdit )
{
	SetColor( D3DXCOLOR(0xffffffff) );

	m_bEditEdge = bEdit;
}

vector<D3DXVECTOR2> CKTDGUIListBox::GetPosList()
{
	vector<D3DXVECTOR2> ret;

	ret.push_back( GetPos() );

	return ret;
}

void CKTDGUIListBox::DrawEditEdge()
{
	if( false == m_bEditEdge )
		return;	

	if ( m_pCheckedEdgeTexture == NULL )
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
	edgeRect.left = m_NowPoint.leftTopPoint.x;
	edgeRect.top = m_NowPoint.leftTopPoint.y;
	edgeRect.right = m_NowPoint.rightBottomPoint.x;
	edgeRect.bottom = m_NowPoint.rightBottomPoint.y;

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

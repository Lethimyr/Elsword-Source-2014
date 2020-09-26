#include "StdAfx.h"
#include ".\ktdguieditbox.h"

bool	CKTDGUIEditBox::s_bHideCaret;   // If true, we don't render the caret.

CKTDGUIEditBox::CKTDGUIEditBox()
: m_pFont( NULL )
, m_rcText( RECT() )
{
	m_bBGCheck		= false;

	m_CustomMsgEditBoxChange	= -1;
	m_CustomMsgEditBoxEnter		= -1;
	m_CustomMsgEditBoxFocusOut	= -1;

	m_pNormalTex	= NULL;
	m_pDisableTex	= NULL;

	m_pNormalPoint	= NULL;
	m_pDisablePoint	= NULL;
	m_pTextPoint	= NULL;

	m_ControlType	= CKTDGUIControl::UCT_EDITBOX;

	m_bCaretOn		= true;
	m_dfBlink		= GetCaretBlinkTime() * 0.0005f;
	m_dfLastBlink	= DXUTGetGlobalTimer()->GetAbsoluteTime();
	s_bHideCaret	= false;
	m_nFirstVisible	= 0;
	m_TextColor		= D3DCOLOR_ARGB( 255, 16, 16, 16 );
	m_SelTextColor	= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_SelBkColor	= D3DCOLOR_ARGB( 255, 40, 50, 92 );
	m_CaretColor	= D3DCOLOR_ARGB( 255, 0, 0, 0 );
	m_nCaret		= m_nSelStart = 0;
	m_bInsertMode	= true;

	m_bMouseDrag	= false;

	m_NowFontColor			= 0xff000000;
	m_OutlineColor			= 0xffffffff;
	m_bOutlineCheck			= false;

	m_bPasswordEditBox		= false;
	m_HideChar = '*';

	m_pSndKeyDown	= NULL;

	m_bEnableInput			= true;

//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring keyDownSndFileName;

	LUA_GET_VALUE( kLuaManager, "EditBox_KeyDown", keyDownSndFileName, L"" );
	m_pSndKeyDown = g_pKTDXApp->GetDeviceManager()->OpenSound( keyDownSndFileName );
#ifdef FIX_IME_EDITBOX_LOSE_FIRST_CHAR
	m_rcText.left = 0;
	m_rcText.top = 0;
	m_rcText.right = 0;
	m_rcText.bottom = 0;
#endif FIX_IME_EDITBOX_LOSE_FIRST_CHAR

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;
#endif
}

CKTDGUIEditBox::~CKTDGUIEditBox(void)
{
	SAFE_DELETE( m_pNormalTex );
	SAFE_DELETE( m_pDisableTex );

	SAFE_DELETE( m_pNormalPoint );
	SAFE_DELETE( m_pDisablePoint );
	SAFE_DELETE( m_pTextPoint );

	SAFE_CLOSE( m_pSndKeyDown );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif
}

HRESULT CKTDGUIEditBox::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if( m_bEnable == false )
		return S_OK;

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	m_fElapsedTime = fElapsedTime;

	if( m_bBGCheck == true )
	{
		if( m_bEnable == false )
		{
			m_EndPoint = *m_pDisablePoint;
			m_NowPoint = *m_pDisablePoint;
		}
		else
		{
			m_EndPoint = *m_pNormalPoint;
			m_NowPoint = *m_pNormalPoint;
		}
	}
	else
	{
		m_NowPoint = *m_pTextPoint;
	}

	m_rcText.left	= (LONG)(m_pTextPoint->leftTopPoint.x);
	m_rcText.top	= (LONG)(m_pTextPoint->leftTopPoint.y);
	m_rcText.right	= (LONG)(m_pTextPoint->rightBottomPoint.x);
	m_rcText.bottom	= (LONG)(m_pTextPoint->rightBottomPoint.y);

	return S_OK;
}

HRESULT CKTDGUIEditBox::OnFrameRender()
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	DrawEditEdge();
#endif

	HRESULT hr;
	int nSelStartX = 0, nCaretX = 0;  // Left and right X cordinates of the selection region

	m_Buffer.SetFontNode( m_pFont );
	PlaceCaret( m_nCaret );  // Call PlaceCaret now that we have the font info (node),

	// Render the control graphics(배경 출력)
	if( m_bBGCheck == true )
	{
		UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
					m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
					m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
					m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
					m_NowPoint );

		RenderVertex( m_Vertex, m_NowPoint );
	}

	//
	// Compute the X coordinates of the first visible character.
	//
	int nXFirst;
	m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nXFirst );

	//
	// Compute the X coordinates of the selection rectangle
	//
	hr = m_Buffer.CPtoX( m_nCaret, FALSE, &nCaretX );

	if( m_nCaret != m_nSelStart )
		hr = m_Buffer.CPtoX( m_nSelStart, FALSE, &nSelStartX );
	else
		nSelStartX = nCaretX;

	//
	// Render the selection rectangle
	//
	RECT rcSelection;  // Make this available for rendering selected text
	if( m_nCaret != m_nSelStart )
	{
		int nSelLeftX = nCaretX, nSelRightX = nSelStartX;
		// Swap if left is bigger than right
		if( nSelLeftX > nSelRightX )
		{ int nTemp = nSelLeftX; nSelLeftX = nSelRightX; nSelRightX = nTemp; }

		SetRect( &rcSelection, nSelLeftX, m_rcText.top, nSelRightX, m_rcText.bottom );
		OffsetRect( &rcSelection, m_rcText.left - nXFirst, 0 );
		IntersectRect( &rcSelection, &m_rcText, &rcSelection );
		DrawRect( &rcSelection, m_SelBkColor );
	}

	//
	// Render the text
	//
	// Element 0 for text
	m_NowFontColor = m_TextColor;

	if ( m_bPasswordEditBox == false )
	{
		DrawText( m_Buffer.GetBuffer() + m_nFirstVisible, &m_rcText );
	}
	else if ( m_bPasswordEditBox == true )
	{
		wstring tempString;
		
		for ( int i = 0; i < m_Buffer.GetTextSize(); i++ )
		{
			tempString += m_HideChar;
		}

		DrawText(tempString.c_str() + m_nFirstVisible, &m_rcText);
	}
	

	// Render the selected text
	if( m_nCaret != m_nSelStart )
	{
		int nFirstToRender = __max( m_nFirstVisible, __min( m_nSelStart, m_nCaret ) );
		int nNumChatToRender = __max( m_nSelStart, m_nCaret ) - nFirstToRender;
		m_NowFontColor = m_SelTextColor;

		if ( m_bPasswordEditBox == false )
		{
#ifdef FIX_EDITBOX_SELECTION
			DrawText( m_Buffer.GetBuffer() + nFirstToRender, &rcSelection, nNumChatToRender );
#else //FIX_EDITBOX_SELECTION
			DrawText( m_Buffer.GetBuffer() + m_nFirstVisible, &rcSelection, nNumChatToRender );
#endif //FIX_EDITBOX_SELECTION
		}
		else if ( m_bPasswordEditBox == true )
		{
			wstring tempString;

			for ( int i = 0; i < m_Buffer.GetTextSize(); i++ )
			{
				tempString += m_HideChar;
			}
#ifdef FIX_EDITBOX_SELECTION
			DrawText(tempString.c_str() + nFirstToRender, &rcSelection, nNumChatToRender);
#else //FIX_EDITBOX_SELECTION
			DrawText(tempString.c_str() + m_nFirstVisible, &rcSelection, nNumChatToRender);
#endif //FIX_EDITBOX_SELECTION
		}

	}

	//
	// Blink the caret
	//
	if( DXUTGetGlobalTimer()->GetAbsoluteTime() - m_dfLastBlink >= m_dfBlink )
	{
		m_bCaretOn = !m_bCaretOn;
		m_dfLastBlink = DXUTGetGlobalTimer()->GetAbsoluteTime();
	}

	//
	// Render the caret if this control has the focus
	//
	if( m_bHasFocus && m_bCaretOn && !s_bHideCaret )
	{
		// Start the rectangle with insert mode caret
		RECT rcCaret = { m_rcText.left - nXFirst + nCaretX - 1, m_rcText.top,
			m_rcText.left - nXFirst + nCaretX + 1, m_rcText.bottom };

		// If we are in overwrite mode, adjust the caret rectangle
		// to fill the entire character.
		if( !m_bInsertMode )
		{
			// Obtain the right edge X coord of the current character
			int nRightEdgeX;
			m_Buffer.CPtoX( m_nCaret, TRUE, &nRightEdgeX );
			rcCaret.right = m_rcText.left - nXFirst + nRightEdgeX;
		}

		DrawRect( &rcCaret, m_CaretColor );
	}

	return S_OK;
}

bool CKTDGUIEditBox::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false )
		return false;

	bool bHandled = false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			if(m_bEnableInput == false)
				return true;

			switch( wParam )
			{
			case VK_TAB:
				// We don't process Tab in case keyboard input is enabled and the user
				// wishes to Tab to other controls.
				break;

			case VK_HOME:
				PlaceCaret( 0 );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_END:
				PlaceCaret( m_Buffer.GetTextSize() );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_INSERT:
				if( GetKeyState( VK_CONTROL ) < 0 )
				{
					// Control Insert. Copy to clipboard
					CopyToClipboard();
				} else
					if( GetKeyState( VK_SHIFT ) < 0 )
					{
						// Shift Insert. Paste from clipboard
						PasteFromClipboard();
					} else
					{
						// Toggle caret insert mode
						m_bInsertMode = !m_bInsertMode;
					}
					break;

			case VK_DELETE:
				// Check if there is a text selection.
				if( m_nCaret != m_nSelStart )
				{
					DeleteSelectionText();
					if( m_CustomMsgEditBoxChange != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
					else
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
				}
				else
				{
					// Deleting one character
					if( m_Buffer.RemoveChar( m_nCaret ) )
					{
						if( m_CustomMsgEditBoxChange != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
						else
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
					}
				}
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_LEFT:
				if( GetKeyState( VK_CONTROL ) < 0 )
				{
					// Control is down. Move the caret to a new item
					// instead of a character.
					m_Buffer.GetPriorItemPos( m_nCaret, &m_nCaret );
					PlaceCaret( m_nCaret );
				}
				else
					if( m_nCaret > 0 )
						PlaceCaret( m_nCaret - 1 );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_RIGHT:
				if( GetKeyState( VK_CONTROL ) < 0 )
				{
					// Control is down. Move the caret to a new item
					// instead of a character.
					m_Buffer.GetNextItemPos( m_nCaret, &m_nCaret );
					PlaceCaret( m_nCaret );
				}
				else
					if( m_nCaret < m_Buffer.GetTextSize() )
						PlaceCaret( m_nCaret + 1 );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_UP:
			case VK_DOWN:
				// Trap up and down arrows so that the dialog
				// does not switch focus to another control.
				bHandled = true;
				break;

			default:
				bHandled = wParam != VK_ESCAPE;  // Let the application handle Esc.
			}
		}
	}
	return bHandled;
}

bool CKTDGUIEditBox::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false )
		return false;

	pt.x = GET_X_LPARAM(lParam); 
	pt.y = GET_Y_LPARAM(lParam); 

	D3DXVECTOR2 pos = g_pKTDXApp->MouseConvertByResolution( (float)pt.x, (float)pt.y );
	pt.x = (LONG)pos.x;
	pt.y = (LONG)pos.y;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( !m_bHasFocus )
			{
				m_pDialog->RequestFocus( this );
			}

			if( !ContainPTForEditBox( pt ) )
				return false;

			m_bMouseDrag = true;
#ifndef DLL_BUILD
			SetCapture( DXUTGetHWND() );
#endif
			// Determine the character corresponding to the coordinates.
			int nCP, nTrail, nX1st;
			m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nX1st );  // X offset of the 1st visible char
			if( SUCCEEDED( m_Buffer.XtoCP( pt.x - m_rcText.left + nX1st, &nCP, &nTrail ) ) )
			{
				// Cap at the NULL character.
				if( nTrail && nCP < m_Buffer.GetTextSize() )
					PlaceCaret( nCP + 1 );
				else
					PlaceCaret( nCP );
				m_nSelStart = m_nCaret;
				ResetCaretBlink();
			}
			return true;
		}

	case WM_LBUTTONUP:
		ReleaseCapture();
		m_bMouseDrag = false;
		break;

	case WM_MOUSEMOVE:
		if( m_bMouseDrag )
		{
			// Determine the character corresponding to the coordinates.
			int nCP, nTrail, nX1st;
			m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nX1st );  // X offset of the 1st visible char
			if( SUCCEEDED( m_Buffer.XtoCP( pt.x - m_rcText.left + nX1st, &nCP, &nTrail ) ) )
			{
				// Cap at the NULL character.
				if( nTrail && nCP < m_Buffer.GetTextSize() )
					PlaceCaret( nCP + 1 );
				else
					PlaceCaret( nCP );
			}
		}
		break;
	}

	return false;
}

bool CKTDGUIEditBox::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

	switch( uMsg )
	{
	case WM_CHAR:
		{
			if(m_bEnableInput == false)
				return true;

			switch( (WCHAR)wParam )
			{
				// Backspace
			case VK_BACK:
				{
					//if ( m_pSndKeyDown != NULL )
					//	m_pSndKeyDown->Play();

					PlaySound( m_pSndKeyDown );

					// If there's a selection, treat this
					// like a delete key.
					if( m_nCaret != m_nSelStart )
					{
						DeleteSelectionText();

						if( m_CustomMsgEditBoxChange != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
						else
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
					}
					else
						if( m_nCaret > 0 )
						{
							// Move the caret, then delete the char.
							PlaceCaret( m_nCaret - 1 );
							m_nSelStart = m_nCaret;
							m_Buffer.RemoveChar( m_nCaret );

							if( m_CustomMsgEditBoxChange != -1 )
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
							else
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
						}
						ResetCaretBlink();
						break;
				}

			case 24:        // Ctrl-X Cut
			case VK_CANCEL: // Ctrl-C Copy
				{
					CopyToClipboard();

					// If the key is Ctrl-X, delete the selection too.
					if( (WCHAR)wParam == 24 )
					{
						DeleteSelectionText();

						if( m_CustomMsgEditBoxChange != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
						else
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
					}

					break;
				}

				// Ctrl-V Paste
			case 22:
				{
					PasteFromClipboard();

					if( m_CustomMsgEditBoxChange != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
					else
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
					break;
				}

				// Ctrl-A Select All
			case 1:
				if( m_nSelStart == m_nCaret )
				{
					m_nSelStart = 0;
					PlaceCaret( m_Buffer.GetTextSize() );
				}
				break;

			case VK_RETURN:
				// Invoke the callback when the user presses Enter.
				if( m_CustomMsgEditBoxEnter != -1 )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxEnter, (LPARAM)this );
				else
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_ENTER, (LPARAM)this );
				break;

				// Junk characters we don't want in the string
			case 26:  // Ctrl Z
			case 2:   // Ctrl B
			case 14:  // Ctrl N
			case 19:  // Ctrl S
			case 4:   // Ctrl D
			case 6:   // Ctrl F
			case 7:   // Ctrl G
			case 10:  // Ctrl J
			case 11:  // Ctrl K
			case 12:  // Ctrl L
			case 17:  // Ctrl Q
			case 23:  // Ctrl W
			case 5:   // Ctrl E
			case 18:  // Ctrl R
			case 20:  // Ctrl T
			case 25:  // Ctrl Y
			case 21:  // Ctrl U
			case 9:   // Ctrl I
			case 15:  // Ctrl O
			case 16:  // Ctrl P
			case 27:  // Ctrl [
			case 29:  // Ctrl ]
			case 28:  // Ctrl \ 
				break;

			default:
				{
					//if ( m_pSndKeyDown != NULL )
					//	m_pSndKeyDown->Play();

					PlaySound( m_pSndKeyDown );

					// If there's a selection and the user
					// starts to type, the selection should
					// be deleted.
					if( m_nCaret != m_nSelStart )
						DeleteSelectionText();

					// If we are in overwrite mode and there is already
					// a char at the caret's position, simply replace it.
					// Otherwise, we insert the char as normal.
					if( !m_bInsertMode && m_nCaret < m_Buffer.GetTextSize() )
					{
						m_Buffer[m_nCaret] = (WCHAR)wParam;
						PlaceCaret( m_nCaret + 1 );
						m_nSelStart = m_nCaret;
					} else
					{
						// Insert the char
						if( m_Buffer.InsertChar( m_nCaret, (WCHAR)wParam ) )
						{
							PlaceCaret( m_nCaret + 1 );
							m_nSelStart = m_nCaret;
						}
					}
					ResetCaretBlink();

					if( m_CustomMsgEditBoxChange != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
					else
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
				}
			}
			return true;
		}
	}
	return false;
}

#ifdef LOGIN_KEYBOARD_SECURITY
void CKTDGUIEditBox::ProcessBackspace()
{
	PlaySound( m_pSndKeyDown );

	// If there's a selection, treat this
	// like a delete key.
	if( m_nCaret != m_nSelStart )
	{
		DeleteSelectionText();

		if( m_CustomMsgEditBoxChange != -1 )
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
		else
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
	}
	else
		if( m_nCaret > 0 )
		{
			// Move the caret, then delete the char.
			PlaceCaret( m_nCaret - 1 );
			m_nSelStart = m_nCaret;
			m_Buffer.RemoveChar( m_nCaret );

			if( m_CustomMsgEditBoxChange != -1 )
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
			else
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, EEM_CHANGE, (LPARAM)this );
		}
		ResetCaretBlink();
}
#endif LOGIN_KEYBOARD_SECURITY

void CKTDGUIEditBox::SetNormalTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pNormalTex );

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pNormalTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pNormalPoint != NULL )
		m_pNormalPoint->pUITextureData = m_pNormalTex;

	m_bBGCheck = true;
}

void CKTDGUIEditBox::SetDisableTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pDisableTex );

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pDisableTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pDisablePoint != NULL )
		m_pDisablePoint->pUITextureData = m_pDisableTex;

	m_bBGCheck = true;
}

void CKTDGUIEditBox::SetNormalPoint_LUA()
{
	SAFE_DELETE( m_pNormalPoint );
	m_pNormalPoint = SetPointData_();
	if( m_pNormalTex != NULL )
	{
		m_pNormalPoint->pUITextureData = m_pNormalTex;
		m_pNormalPoint->SetAutoPointByTextureSize();
	}

	m_bBGCheck = true;
}

void CKTDGUIEditBox::SetDisablePoint_LUA()
{
	SAFE_DELETE( m_pDisablePoint );
	m_pDisablePoint = SetPointData_();
	if( m_pDisableTex != NULL )
	{
		m_pDisablePoint->pUITextureData = m_pDisableTex;
		m_pDisablePoint->SetAutoPointByTextureSize();
	}

	m_bBGCheck = true;
}

void CKTDGUIEditBox::SetTextPoint_LUA()
{
	SAFE_DELETE( m_pTextPoint );
	m_pTextPoint = SetPointData_();
}

void CKTDGUIEditBox::SetText( LPCWSTR wszText, bool bSelected )
{
	assert( wszText != NULL );

	m_Buffer.SetText( wszText );
	m_nFirstVisible = 0;
	// Move the caret to the end of the text
	PlaceCaret( m_Buffer.GetTextSize() );
	m_nSelStart = bSelected ? 0 : m_nCaret;
}

HRESULT CKTDGUIEditBox::GetTextCopy( LPWSTR strDest, UINT bufferCount )
{
	assert( strDest );

	StringCchCopy( strDest, bufferCount, m_Buffer.GetBuffer() );

	return S_OK;
}

void CKTDGUIEditBox::SetFont_LUA( int nFontIndex )
{
#ifdef  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetMappedUKFont( nFontIndex );
#else
	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont( nFontIndex );
#endif
}

void CKTDGUIEditBox::ClearText()
{
	m_Buffer.Clear();
	m_nFirstVisible = 0;
	PlaceCaret( 0 );
	m_nSelStart = 0;
}

void CKTDGUIEditBox::SetHideChar(WCHAR hideChar)
{
	m_HideChar = hideChar; 
	
	m_bPasswordEditBox = true;

	m_Buffer.SetHideChar( true, m_HideChar );
}

void CKTDGUIEditBox::SetHideChar_( const char* hideChar )
{
	wstring temp;
	ConvertUtf8ToWCHAR( temp , hideChar );

	wstring::iterator i = temp.begin();

	m_HideChar = (*i);

	m_bPasswordEditBox = true;

	m_Buffer.SetHideChar( true, m_HideChar );
}

void CKTDGUIEditBox::PlaceCaret( int nCP )
{
	assert( nCP >= 0 && nCP <= m_Buffer.GetTextSize() );
	m_nCaret = nCP;

	// Obtain the X offset of the character.
	int nX1st, nX, nX2;
	m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nX1st );  // 1st visible char
	m_Buffer.CPtoX( nCP, FALSE, &nX );  // LEAD
	// If nCP is the NULL terminator, get the leading edge instead of trailing.
	if( nCP == m_Buffer.GetTextSize() )
		nX2 = nX;
	else
		m_Buffer.CPtoX( nCP, TRUE, &nX2 );  // TRAIL

	// If the left edge of the char is smaller than the left edge of the 1st visible char,
	// we need to scroll left until this char is visible.
	if( nX < nX1st )
	{
		// Simply make the first visible character the char at the new caret position.
		m_nFirstVisible = nCP;
	}
	else
	{
		// If the right of the character is bigger than the offset of the control's
		// right edge, we need to scroll right to this character.
		if( nX2 > nX1st + RtWidth( m_rcText ) )
		{
			// Compute the X of the new left-most pixel
			int nXNewLeft = nX2 - RtWidth( m_rcText );

			// Compute the char position of this character
			int nCPNew1st, nNewTrail;
			m_Buffer.XtoCP( nXNewLeft, &nCPNew1st, &nNewTrail );

			// If this coordinate is not on a character border,
			// start from the next character so that the caret
			// position does not fall outside the text rectangle.
			int nXNew1st;
			m_Buffer.CPtoX( nCPNew1st, FALSE, &nXNew1st );
			if( nXNew1st < nXNewLeft )
				++nCPNew1st;

			m_nFirstVisible = nCPNew1st;
		}
	}
}

void CKTDGUIEditBox::DeleteSelectionText()
{
	int nFirst	= __min( m_nCaret, m_nSelStart );
	int nLast	= __max( m_nCaret, m_nSelStart );

	// Update caret and selection
	PlaceCaret( nFirst );
	m_nSelStart = m_nCaret;

	// Remove the characters
	for( int i = nFirst; i < nLast; ++i )
		m_Buffer.RemoveChar( nFirst );
}

void CKTDGUIEditBox::ResetCaretBlink()
{
	m_bCaretOn		= true;
	m_dfLastBlink	= DXUTGetGlobalTimer()->GetAbsoluteTime();
}

void CKTDGUIEditBox::CopyToClipboard()
{
	// Copy the selection text to the clipboard
	if( m_nCaret != m_nSelStart && OpenClipboard( NULL ) )
	{
		EmptyClipboard();

		HGLOBAL hBlock = GlobalAlloc( GMEM_MOVEABLE, sizeof(WCHAR) * ( m_Buffer.GetTextSize() + 1 ) );

		if( hBlock )
		{
			WCHAR *pwszText = (WCHAR*)GlobalLock( hBlock );
			if( pwszText )
			{
				int nFirst = __min( m_nCaret, m_nSelStart );
				int nLast = __max( m_nCaret, m_nSelStart );
				if( nLast - nFirst > 0 )
					CopyMemory( pwszText, m_Buffer.GetBuffer() + nFirst, (nLast - nFirst) * sizeof(WCHAR) );
				pwszText[nLast - nFirst] = L'\0';  // Terminate it
				GlobalUnlock( hBlock );
			}
			SetClipboardData( CF_UNICODETEXT, hBlock );
		}
		CloseClipboard();
		// We must not free the object until CloseClipboard is called.
		if( hBlock )
			GlobalFree( hBlock );
	}
}

void CKTDGUIEditBox::PasteFromClipboard()
{
	DeleteSelectionText();

	if( OpenClipboard( NULL ) )
	{
		HANDLE handle = GetClipboardData( CF_UNICODETEXT );
		if( handle )
		{
			// Convert the ANSI string to Unicode, then
			// insert to our buffer.
			WCHAR *pwszText = (WCHAR*)GlobalLock( handle );
			if( pwszText )
			{
				// Copy all characters up to null.
				if( m_Buffer.InsertString( m_nCaret, pwszText ) )
					PlaceCaret( m_nCaret + lstrlenW( pwszText ) );
				m_nSelStart = m_nCaret;
				GlobalUnlock( handle );
			}
		}
		CloseClipboard();
	}
}

HRESULT CKTDGUIEditBox::DrawText( LPCWSTR strText, RECT* prcDest, int nCount /* = -1 */, DWORD format /* = DT_LEFT | DT_VCENTER  */)
{
	RECT rcScreen = *prcDest;

	OffsetRect( &rcScreen,
		(LONG)(m_pDialog->GetPos().x + m_OffsetPos.x),
		(LONG)(m_pDialog->GetPos().y + m_OffsetPos.y) );

	if( m_bOutlineCheck == true )
	{
		m_pFont->DrawTextFont( strText, &rcScreen, nCount, m_NowFontColor, true, m_OutlineColor, format );
	}
	else
	{
		m_pFont->DrawTextFont( strText, &rcScreen, nCount, m_NowFontColor, false, m_OutlineColor, format );
	}

	return S_OK;
}

HRESULT CKTDGUIEditBox::DrawRect( RECT* prcDest, D3DCOLOR color )
{
	RECT rcScreen = *prcDest;

	rcScreen.left		= (LONG)(rcScreen.left * g_pKTDXApp->GetResolutionScaleX() + m_pDialog->GetPos().x * g_pKTDXApp->GetResolutionScaleX());
	rcScreen.top		= (LONG)(rcScreen.top * g_pKTDXApp->GetResolutionScaleY() + m_pDialog->GetPos().y * g_pKTDXApp->GetResolutionScaleY());
	rcScreen.right		= (LONG)(rcScreen.right * g_pKTDXApp->GetResolutionScaleX() + m_pDialog->GetPos().x * g_pKTDXApp->GetResolutionScaleX());
	rcScreen.bottom		= (LONG)(rcScreen.bottom * g_pKTDXApp->GetResolutionScaleY() + m_pDialog->GetPos().y * g_pKTDXApp->GetResolutionScaleY());

	CKTDGUIControl::VERTEX_UI vertices[4] =
	{
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.left	 - 0.5f, (float)rcScreen.top	-0.5f, 0.5f, 1.0f, color, 0, 0 ),
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.right - 0.5f, (float)rcScreen.top	-0.5f, 0.5f, 1.0f, color, 0, 0 ),
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.left	 - 0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 ),
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.right - 0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 )
		/*
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		{ (float)rcScreen.left	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0 },
		{ (float)rcScreen.right	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0 },
		{ (float)rcScreen.left	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 },
		{ (float)rcScreen.right	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 },
#else		
		(float)rcScreen.left	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0,
		(float)rcScreen.right	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0, 
		(float)rcScreen.right	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0, 
		(float)rcScreen.left	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0,
#endif
		*/
	};

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
//{{ robobeg : 2008-10-13
	//m_pFont->GetD3DSprite()->Begin( D3DXSPRITE_DONOTSAVESTATE );
	m_pFont->GetD3DSprite()->Begin( D3DXSPRITE_DONOTSAVESTATE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE );
//}} robobeg : 2008-10-13
	m_pFont->GetD3DSprite()->Flush();

	IDirect3DVertexDeclaration9 *pDecl = NULL;
	g_pKTDXApp->GetDevice()->GetVertexDeclaration( &pDecl );
	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
#endif

    KD3DBEGIN()

	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	RenderVertex( vertices, true );
#else
	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(CKTDGUIControl::VERTEX_UI) );
#endif

	CKTDGStateManager::SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	CKTDGStateManager::SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	g_pKTDXApp->GetDevice()->SetVertexDeclaration( pDecl );
	pDecl->Release();
#endif

    KD3DEND()

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_pFont->GetD3DSprite()->End();
#endif

	return S_OK;
}

bool CKTDGUIEditBox::ContainPTForEditBox(POINT pt)
{
	D3DXVECTOR2 arPoint[4];



	arPoint[0] = m_pDialog->GetPos() + m_NowPoint.leftTopPoint + m_OffsetPos;
	arPoint[1] = m_pDialog->GetPos() + m_NowPoint.rightTopPoint + m_OffsetPos;
	arPoint[2] = m_pDialog->GetPos() + m_NowPoint.leftBottomPoint + m_OffsetPos;
	arPoint[3] = m_pDialog->GetPos() + m_NowPoint.rightBottomPoint + m_OffsetPos;

	return Pick2DRect( pt, arPoint[0], arPoint[1], arPoint[2], arPoint[3] ); 
}

void CKTDGUIEditBox::OnFocusOut()
{
	bool bHasFocusOld = m_bHasFocus;

	CKTDGUIControl::OnFocusOut();

	if( true == bHasFocusOld &&
		-1 != m_CustomMsgEditBoxFocusOut )
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxFocusOut, (LPARAM)this );
	}
}

#ifdef DLL_BUILD
void CKTDGUIEditBox::MoveControl( float fx, float fy )
{
	if( NULL != m_pTextPoint )
		return m_pTextPoint->Move(fx, fy);
}

D3DXVECTOR2 CKTDGUIEditBox::GetPos()
{
	if( NULL != m_pTextPoint )
		return m_pTextPoint->leftTopPoint;

	return D3DXVECTOR2(0, 0);
}

vector<D3DXVECTOR2> CKTDGUIEditBox::GetPosList()
{
	vector<D3DXVECTOR2> ret;	

	if( NULL != m_pTextPoint )
	{
		ret.push_back(m_pTextPoint->leftTopPoint);
		ret.push_back(m_pTextPoint->rightTopPoint);
		ret.push_back(m_pTextPoint->leftBottomPoint);
		ret.push_back(m_pTextPoint->rightBottomPoint);
	}

	return ret;
}

void CKTDGUIEditBox::SetEditGUI( bool bEdit )
{
	SetColor(D3DXCOLOR(0xffffffff));
	m_bEditEdge = bEdit;
}

bool CKTDGUIEditBox::IsSelectByEditGui( POINT pt )
{
	return Pick2DRect( pt, m_NowPoint.leftTopPoint, m_NowPoint.rightTopPoint, m_NowPoint.leftBottomPoint ,m_NowPoint.rightBottomPoint );	
}

void CKTDGUIEditBox::DrawEditEdge()
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
	edgeRect.left = (int)m_NowPoint.leftTopPoint.x;
	edgeRect.top = (int)m_NowPoint.leftTopPoint.y;
	edgeRect.right = (int)m_NowPoint.rightBottomPoint.x;
	edgeRect.bottom = (int)m_NowPoint.rightBottomPoint.y;

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
#include "dxstdafx.h"
#include ".\x2dteditbox.h"

KX2DTEditBox::KX2DTEditBox( CDXUTDialog *pDialog ) : CDXUTEditBox( pDialog )
{
	m_bIsOnlyNumber = false;
	m_bMinusSign = false;
}

KX2DTEditBox::~KX2DTEditBox(void)
{
}

bool KX2DTEditBox::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( !m_bEnabled || !m_bVisible )
		return false;

	switch( uMsg )
	{
	case WM_CHAR:
		{
			switch( (WCHAR)wParam )
			{
				// Backspace
			case VK_BACK:
				{
					// If there's a selection, treat this
					// like a delete key.
					if( m_nCaret != m_nSelStart )
					{
						DeleteSelectionText();
						m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
					}
					else
						if( m_nCaret > 0 )
						{
							// Move the caret, then delete the char.
							PlaceCaret( m_nCaret - 1 );
							m_nSelStart = m_nCaret;
							m_Buffer.RemoveChar( m_nCaret );
							m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
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
						m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
					}

					break;
				}

				// Ctrl-V Paste
			case 22:
				{
					PasteFromClipboard();
					m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
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
				m_pDialog->SendEvent( EVENT_EDITBOX_STRING, true, this );
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
						if( m_bIsOnlyNumber && iswdigit( (WCHAR)wParam ) == 0 )
						{
							if( m_bMinusSign == false || wParam != (int)'-' )
								return true;
						}

						// Insert the char
						if( m_Buffer.InsertChar( m_nCaret, (WCHAR)wParam ) )
						{
							PlaceCaret( m_nCaret + 1 );
							m_nSelStart = m_nCaret;
						}
					}
					ResetCaretBlink();
					m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
				}
			}
			return true;
		}
	}
	return false;
}
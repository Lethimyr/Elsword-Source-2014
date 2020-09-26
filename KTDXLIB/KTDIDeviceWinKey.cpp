#include "StdAfx.h"

#ifdef CHANGE_KEY_DEVICE

#include ".\ktdideviceWinKey.h"

#define DOUBLE_INPUT_GAP 0.4f

CKTDIDeviceWinKey::CKTDIDeviceWinKey()
{

	// Clear the device variables
	Clear();

}

CKTDIDeviceWinKey::~CKTDIDeviceWinKey()
{
	// Free a prior install
	Free();

}

// Generic functions - all devices
BOOL CKTDIDeviceWinKey::Create()
{
	// Clear the device information
	Clear();

    SetupVKToDIKMap();

	// Return a success
	return TRUE;
}
BOOL CKTDIDeviceWinKey::Free()
{
	// Clear the data  
	Clear();

	return TRUE;
}

BOOL CKTDIDeviceWinKey::Clear()
{
	short i;


    ZeroMemory( &m_aVKtoDIKMap, 256 );

	ZeroMemory( &m_State, 256 );
#ifdef KEY_MAPPING
	ZeroMemory( &m_ActionMap, GAMEACTION_END*sizeof(unsigned char) );
#endif KEY_MAPPING

	for( i = 0; i < 256; i++ )
    {
        m_abQueuedVKState[ i ] = false;
        m_abCurrentVKState[ i ] = false;
		m_Locks[i]  = FALSE;
    }//if

	m_bExtraEnter = false;

    m_bActivated = true;

	return TRUE;
}


BOOL CKTDIDeviceWinKey::Read( DWORD dwStepTime_, float fElapsedTime_ )
{
    LONG    lDiff;

    while( m_dequeKeyTime.empty() == false )
    {
        const KeyTime& keyTime = m_dequeKeyTime.front();
        lDiff = (LONG) ( dwStepTime_ - keyTime.m_dwStamp );
        if ( lDiff < 0 )
            break;
        m_abCurrentVKState[ keyTime.m_byVirtualKey ] = keyTime.m_bDown;
        m_dequeKeyTime.pop_front();
    }//while

    ZeroMemory( m_State, 256 );
    int iDIKKey = 0;
    for( int i = 0; i < 256; i++ )
    {
        iDIKKey = m_aVKtoDIKMap[ i ];
        if ( iDIKKey <= 0 || iDIKKey >= 256 )
            continue;
        if ( m_abCurrentVKState[ i ] == true )
            m_State[ iDIKKey ] = 0x80;
    }//for

    m_abQueuedVKState[ VK_SNAPSHOT ] = false;
    m_abCurrentVKState[ VK_SNAPSHOT ] = false;

	// Released keys and button need to be unlocked
	for( int i = 0; i < 256; i++ ) 
	{
		if( !( m_State[i] & 0x80 ) )
			m_Locks[i] = FALSE;

		if( m_DoubleKeyCheck[i].bFirstDown == true 
			&& m_DoubleKeyCheck[i].bDoubleOK == false )
		{
			m_DoubleKeyCheck[i].fTimeOut -= fElapsedTime_;
			if( m_DoubleKeyCheck[i].fTimeOut < 0.0f )
			{
				m_DoubleKeyCheck[i].Init();						
			}
		}
	}

	// return a success
	return TRUE;
}


BOOL CKTDIDeviceWinKey::GetLock( unsigned char Num )
{
    return ( Num < 256 ) ? m_Locks[Num] : FALSE;
}

BOOL CKTDIDeviceWinKey::SetLock( unsigned char Num, BOOL State )
{
    if ( Num >= 256 )
        return FALSE;

	m_Locks[Num] = State;

	return TRUE;
}


// Keyboard specific functions
BOOL  CKTDIDeviceWinKey::GetKeyState( unsigned char Num )
{
    if ( Num >= 256 )
        return FALSE;

	// Check if key/button is pressed
	if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
	{
		m_Locks[Num] = TRUE;
		return TRUE;
	}
	return FALSE;
}

BOOL  CKTDIDeviceWinKey::SetKeyState( unsigned char Num, BOOL State )
{
    if ( Num >= 256 )
        return FALSE;

	m_State[Num] = State;
	return TRUE;
}

BOOL  CKTDIDeviceWinKey::GetPureKeyState( unsigned char Num )
{
    if ( Num >= 256 )
        return FALSE;

	return ((m_State[Num] & 0x80) ? TRUE : FALSE);
}

BOOL  CKTDIDeviceWinKey::GetDoubleKeyPureState( unsigned char Num )
{
    if ( Num >= 256 )
        return FALSE;

	if( m_DoubleKeyCheck[Num].bDoubleOK != true )
	{
		if( m_DoubleKeyCheck[Num].bFirstDown == false )
		{
			if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bFirstDown = true;
				m_DoubleKeyCheck[Num].fTimeOut = DOUBLE_INPUT_GAP;
			}
			return FALSE;

		}
		else
		{
			if( m_DoubleKeyCheck[Num].bKeyUp == false )
			{
				if( GetPureKeyState( Num ) == FALSE )
				{
					m_DoubleKeyCheck[Num].bKeyUp = true;
				}
				return FALSE;
			}
			else if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bDoubleOK = true;
			}
			else
				return FALSE;
		}
	}

	if( m_DoubleKeyCheck[Num].bDoubleOK == true
		&& GetPureKeyState( Num ) == TRUE )
		return TRUE;
	else
	{
		m_DoubleKeyCheck[Num].Init();
		return FALSE;
	}

}

BOOL  CKTDIDeviceWinKey::GetDoubleKeyState( unsigned char Num )
{
    if ( Num >= 256 )
        return FALSE;

	if( m_DoubleKeyCheck[Num].bDoubleOK != true )
	{
		if( m_DoubleKeyCheck[Num].bFirstDown == false )
		{
			if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bFirstDown = true;
				m_DoubleKeyCheck[Num].fTimeOut = DOUBLE_INPUT_GAP;
			}
			return FALSE;

		}
		else
		{
			if( m_DoubleKeyCheck[Num].bKeyUp == false )
			{
				if( GetPureKeyState( Num ) == FALSE )
				{
					m_DoubleKeyCheck[Num].bKeyUp = true;
				}
				return FALSE;
			}
			else if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bDoubleOK = true;
			}
			else
				return FALSE;
		}
	}

	if( m_DoubleKeyCheck[Num].bDoubleOK == true
		&& m_DoubleKeyCheck[Num].bDoubleKeyLock == false
		&& GetPureKeyState( Num ) == TRUE )
	{
		m_DoubleKeyCheck[Num].bDoubleKeyLock = true;
		return TRUE;
	}
	else
	{
		m_DoubleKeyCheck[Num].Init();
		return FALSE;
	}

}

void  CKTDIDeviceWinKey::SetDoubleKeyReset( unsigned char Num )
{
    if ( Num >= 256 )
        return;

	m_DoubleKeyCheck[Num].bDoubleOK			= false;
	m_DoubleKeyCheck[Num].bDoubleKeyLock	= false;
	m_DoubleKeyCheck[Num].bFirstDown		= false;
	m_DoubleKeyCheck[Num].bKeyUp			= false;
	m_DoubleKeyCheck[Num].fTimeOut			= 0.0f;

}


long  CKTDIDeviceWinKey::GetNumKeyPresses()
{
	long i, Num = 0;

	for(i=0;i<256;i++) 
	{
		if(m_State[i] & 0x80 && m_Locks[i] == FALSE)
			Num++;
	}

	return Num;
}

long  CKTDIDeviceWinKey::GetNumPureKeyPresses()
{
	long i, Num = 0;

	for(i=0;i<256;i++) 
	{
		if(m_State[i] & 0x80)
			Num++;
	}

	return Num;
}


#ifdef KEY_MAPPING
bool CKTDIDeviceWinKey::GetActionState( int Action, long* lpData )
{	
	return (m_State[m_ActionMap[Action]] ? true : false);
}

#ifdef KEY_MAPPING_UPGRADE
void CKTDIDeviceWinKey::SetAction( int Action, unsigned char Key, bool isDefault )
{
	if(!isDefault)
	{	
		for(int i = 0; i < GAMEACTION_END; i++)
		{
			if(i == Action)
				continue;
			if(m_ActionMap[i] == Key)
			{
				m_ActionMap[i] = 0;
			}
		}
	}
	m_ActionMap[Action] = Key;
	return;
}
#else KEY_MAPPING_UPGRADE
void CKTDIDeviceWinKey::SetAction( int Action, unsigned char Key )
{
	for(int i = 0; i < GAMEACTION_END; ++i)
	{
		if(i == Action)
			continue;
		if(m_ActionMap[i] == Key)
		{
			m_ActionMap[i] = 0;
		}
	}
	m_ActionMap[Action] = Key;
	return;
}
#endif KEY_MAPPING_UPGRADE

unsigned char CKTDIDeviceWinKey::GetActionKey( int Action )
{
	return m_ActionMap[Action];
}

#endif KEY_MAPPING



void    CKTDIDeviceWinKey::SetPushedKeys( DWORD dwNowTime )
{
    KeyTime keyTime;
    keyTime.m_dwStamp = dwNowTime;
    keyTime.m_byVirtualKey = 0;
    keyTime.m_bDown = true;

    for( int i = 0; i < 256; i++ )
    {
        bool    bDown = ( ::GetAsyncKeyState( i ) & 0x8000 ) != 0 ;
        if ( m_abQueuedVKState[ i ] != bDown )
        {
            m_abQueuedVKState[ i ] = bDown;
            keyTime.m_byVirtualKey = BYTE( i );
            m_dequeKeyTime.push_back( keyTime );
        }//if
    }//for
}//CKTDIDeviceWinKey::DetachAllKey()


void    CKTDIDeviceWinKey::ReleaseAllKeys( DWORD dwReleaseTime )
{
    KeyTime keyTime;
    keyTime.m_dwStamp = dwReleaseTime;
    keyTime.m_byVirtualKey = 0;
    keyTime.m_bDown = false;

    for( int i = 0; i < 256; i++ )
    {
        if ( m_abQueuedVKState[ i ] == true )
        {
            m_abQueuedVKState[ i ] = false;
            keyTime.m_byVirtualKey = BYTE( i );
            m_dequeKeyTime.push_back( keyTime );
        }//if
    }//for
}//CKTDIDeviceWinKey::DetachAllKey()



void    CKTDIDeviceWinKey::MsgProc_KeyMsg(
    HWND hWnd_
    , UINT uMsg_
    , WPARAM wParam_
    , LPARAM lParam_ )
{
    KeyTime keyTime;
    bool    bLeft;
    bool    bRight;

    switch( uMsg_ )
    {
    case WM_ACTIVATEAPP:
        {
            if ( wParam_ )
            {
                SetPushedKeys( DXUTGetLastPostMessageTime() );
                m_bActivated = true;
            }
            else
            {
                ReleaseAllKeys( DXUTGetLastPostMessageTime() );
                m_bActivated = false;
            }//if.. else..
        }
        break;
    //case WM_SETFOCUS:
    //    SetPushedKeys( DXUTGetLastPostMessageTime() );
    //    m_bActivated = true;
    //    break;
    //case WM_KILLFOCUS:
    //    ReleaseAllKeys( DXUTGetLastPostMessageTime() );
    //    m_bActivated = false;
    //    break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:

        DWORD	dwInputTime = DXUTGetLastPostMessageTime();
        bool bKeyDown = (uMsg_ == WM_KEYDOWN || uMsg_ == WM_SYSKEYDOWN);
        BYTE    byVirtualKey = (BYTE) (wParam_ & 0xFF);
		if( byVirtualKey == VK_PROCESSKEY )
        {
			byVirtualKey = (BYTE) ( DXUTGetVirtualKey() & 0xFF );
        }//if

        if ( bKeyDown == true && m_bActivated == false )
            break;

        switch( byVirtualKey )
        {
        case VK_SHIFT:
            bLeft = ( ::GetKeyState( VK_LSHIFT ) & 0x8000 ) != 0;
            bRight = ( ::GetKeyState( VK_RSHIFT ) & 0x8000 ) != 0;
            if ( m_abQueuedVKState[ VK_LSHIFT ] != bLeft )
            {
                m_abQueuedVKState[ VK_LSHIFT ] = bLeft;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = VK_LSHIFT;
                keyTime.m_bDown = bLeft;
                m_dequeKeyTime.push_back( keyTime );
            }
            if ( m_abQueuedVKState[ VK_RSHIFT ] != bRight )
            {
                m_abQueuedVKState[ VK_RSHIFT ] = bRight;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = VK_RSHIFT;
                keyTime.m_bDown = bRight;
                m_dequeKeyTime.push_back( keyTime );
            }
            break;
        case VK_CONTROL:
            bLeft = ( ::GetKeyState( VK_LCONTROL ) & 0x8000 ) != 0;
            bRight = ( ::GetKeyState( VK_RCONTROL ) & 0x8000 ) != 0;
            if ( m_abQueuedVKState[ VK_LCONTROL ] != bLeft )
            {
                m_abQueuedVKState[ VK_LCONTROL ] = bLeft;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = VK_LCONTROL;
                keyTime.m_bDown = bLeft;
                m_dequeKeyTime.push_back( keyTime );
            }
            if ( m_abQueuedVKState[ VK_RCONTROL ] != bRight )
            {
                m_abQueuedVKState[ VK_RCONTROL ] = bRight;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = VK_RCONTROL;
                keyTime.m_bDown = bRight;
                m_dequeKeyTime.push_back( keyTime );
            }
            break;
        case VK_MENU:
            bLeft = ( ::GetKeyState( VK_LMENU ) & 0x8000 ) != 0;
            bRight = ( ::GetKeyState( VK_RMENU ) & 0x8000 ) != 0;
            if ( m_abQueuedVKState[ VK_LMENU ] != bLeft )
            {
                m_abQueuedVKState[ VK_LMENU ] = bLeft;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = VK_LMENU;
                keyTime.m_bDown = bLeft;
                m_dequeKeyTime.push_back( keyTime );
            }
            if ( m_abQueuedVKState[ VK_RMENU ] != bRight )
            {
                m_abQueuedVKState[ VK_RMENU ] = bRight;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = VK_RMENU;
                keyTime.m_bDown = bRight;
                m_dequeKeyTime.push_back( keyTime );
            }
            break;
        case VK_SNAPSHOT:
            {
                m_abQueuedVKState[ byVirtualKey ] = true;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = byVirtualKey;
                keyTime.m_bDown = true;
                m_dequeKeyTime.push_back( keyTime );
            }
            break;
        default:
            if ( m_abQueuedVKState[ byVirtualKey ] != bKeyDown )
            {
                m_abQueuedVKState[ byVirtualKey ] = bKeyDown;
                keyTime.m_dwStamp = dwInputTime;
                keyTime.m_byVirtualKey = byVirtualKey;
                keyTime.m_bDown = bKeyDown;
                m_dequeKeyTime.push_back( keyTime );
            }//if

        }//switch        
        break;
    }//switch

}//CKTDIDeviceWinKey::OnMsgProc()



#define VK_TO_DIK( VK, DIK )   if ( (VK)>=0 && (VK)<=255 ) m_aVKtoDIKMap[ (VK) ] = DIK;


void    CKTDIDeviceWinKey::SetupVKToDIKMap()
{
    VK_TO_DIK( VK_BACK, DIK_BACK )
    VK_TO_DIK( VK_TAB, DIK_TAB )
    VK_TO_DIK( VK_RETURN, DIK_RETURN )
    VK_TO_DIK( VK_SHIFT, DIK_LSHIFT )
    VK_TO_DIK( VK_CONTROL, DIK_LCONTROL )
    VK_TO_DIK( VK_MENU , DIK_LMENU )
    VK_TO_DIK( VK_PAUSE, DIK_PAUSE )
    VK_TO_DIK( VK_CAPITAL, DIK_CAPITAL )
    VK_TO_DIK( VK_KANA, DIK_KANA )
    VK_TO_DIK( VK_KANJI, DIK_KANJI  )
    VK_TO_DIK( VK_ESCAPE, DIK_ESCAPE )
    VK_TO_DIK( VK_CONVERT, DIK_CONVERT	 )
    VK_TO_DIK( VK_NONCONVERT, DIK_NOCONVERT )
    VK_TO_DIK( VK_SPACE, DIK_SPACE )
    VK_TO_DIK( VK_PRIOR, DIK_PRIOR )
    VK_TO_DIK( VK_NEXT, DIK_NEXT )
    VK_TO_DIK( VK_END , DIK_END )
    VK_TO_DIK( VK_HOME, DIK_HOME )
    VK_TO_DIK( VK_LEFT, DIK_LEFT )
    VK_TO_DIK( VK_UP, DIK_UP )
    VK_TO_DIK( VK_RIGHT, DIK_RIGHT )
    VK_TO_DIK( VK_DOWN, DIK_DOWN )
    VK_TO_DIK( VK_SNAPSHOT, DIK_SYSRQ )
    VK_TO_DIK( VK_INSERT, DIK_INSERT )
    VK_TO_DIK( VK_DELETE, DIK_DELETE )

    VK_TO_DIK( '0', DIK_0 )
    VK_TO_DIK( '1', DIK_1 )
    VK_TO_DIK( '2', DIK_2 )
    VK_TO_DIK( '3', DIK_3 )
    VK_TO_DIK( '4', DIK_4 )
    VK_TO_DIK( '5', DIK_5 )
    VK_TO_DIK( '6', DIK_6 )
    VK_TO_DIK( '7', DIK_7 )
    VK_TO_DIK( '8', DIK_8 )
    VK_TO_DIK( '9', DIK_9 )

    VK_TO_DIK( 'A', DIK_A )
    VK_TO_DIK( 'B', DIK_B )
    VK_TO_DIK( 'C', DIK_C )
    VK_TO_DIK( 'D', DIK_D )
    VK_TO_DIK( 'E', DIK_E )
    VK_TO_DIK( 'F', DIK_F )
    VK_TO_DIK( 'G', DIK_G )
    VK_TO_DIK( 'H', DIK_H )
    VK_TO_DIK( 'I', DIK_I )
    VK_TO_DIK( 'J', DIK_J )
    VK_TO_DIK( 'K', DIK_K )
    VK_TO_DIK( 'L', DIK_L )
    VK_TO_DIK( 'M', DIK_M )
    VK_TO_DIK( 'N', DIK_N )
    VK_TO_DIK( 'O', DIK_O )
    VK_TO_DIK( 'P', DIK_P )
    VK_TO_DIK( 'Q', DIK_Q )
    VK_TO_DIK( 'R', DIK_R )
    VK_TO_DIK( 'S', DIK_S )
    VK_TO_DIK( 'T', DIK_T )
    VK_TO_DIK( 'U', DIK_U )
    VK_TO_DIK( 'V', DIK_V )
    VK_TO_DIK( 'W', DIK_W )
    VK_TO_DIK( 'X', DIK_X )
    VK_TO_DIK( 'Y', DIK_Y )
    VK_TO_DIK( 'Z', DIK_Z )

    VK_TO_DIK( VK_LWIN, DIK_LWIN )
    VK_TO_DIK( VK_RWIN, DIK_RWIN )
    VK_TO_DIK( VK_APPS, DIK_APPS )
    VK_TO_DIK( VK_SLEEP, DIK_SLEEP )
    VK_TO_DIK( VK_NUMPAD0, DIK_NUMPAD0  )
    VK_TO_DIK( VK_NUMPAD1, DIK_NUMPAD1 )
    VK_TO_DIK( VK_NUMPAD2, DIK_NUMPAD2 )
    VK_TO_DIK( VK_NUMPAD3, DIK_NUMPAD3 )
    VK_TO_DIK( VK_NUMPAD4, DIK_NUMPAD4 )
    VK_TO_DIK( VK_NUMPAD5, DIK_NUMPAD5 )
    VK_TO_DIK( VK_NUMPAD6, DIK_NUMPAD6 )
    VK_TO_DIK( VK_NUMPAD7, DIK_NUMPAD7 )
    VK_TO_DIK( VK_NUMPAD8, DIK_NUMPAD8 )
    VK_TO_DIK( VK_NUMPAD9, DIK_NUMPAD9 )
    VK_TO_DIK( VK_MULTIPLY, DIK_MULTIPLY )
    VK_TO_DIK( VK_ADD, DIK_ADD )
    VK_TO_DIK( VK_SUBTRACT, DIK_SUBTRACT )
    VK_TO_DIK( VK_DECIMAL, DIK_DECIMAL )
    VK_TO_DIK( VK_DIVIDE, DIK_DIVIDE )
    VK_TO_DIK( VK_F1, DIK_F1 )
    VK_TO_DIK( VK_F2, DIK_F2 )
    VK_TO_DIK( VK_F3, DIK_F3 )
    VK_TO_DIK( VK_F4, DIK_F4 )
    VK_TO_DIK( VK_F5, DIK_F5 )
    VK_TO_DIK( VK_F6, DIK_F6 )
    VK_TO_DIK( VK_F7, DIK_F7 )
    VK_TO_DIK( VK_F8, DIK_F8 )
    VK_TO_DIK( VK_F9, DIK_F9 )
    VK_TO_DIK( VK_F10, DIK_F10 )
    VK_TO_DIK( VK_F11, DIK_F11 )
    VK_TO_DIK( VK_F12, DIK_F12 )
    VK_TO_DIK( VK_F13, DIK_F13 )
    VK_TO_DIK( VK_F14, DIK_F14 )
    VK_TO_DIK( VK_F15, DIK_F15 )
	
    VK_TO_DIK( VK_NUMLOCK, DIK_NUMLOCK )
    VK_TO_DIK( VK_SCROLL, DIK_SCROLL )
    VK_TO_DIK( VK_OEM_NEC_EQUAL, DIK_NUMPADEQUALS )
    VK_TO_DIK( VK_LSHIFT, DIK_LSHIFT )
    VK_TO_DIK( VK_RSHIFT, DIK_RSHIFT )
    VK_TO_DIK( VK_LCONTROL, DIK_LCONTROL )
    VK_TO_DIK( VK_RCONTROL, DIK_RCONTROL )
    VK_TO_DIK( VK_LMENU, DIK_LMENU )
    VK_TO_DIK( VK_RMENU, DIK_RMENU )

#if(_WIN32_WINNT >= 0x0500)

    VK_TO_DIK( VK_BROWSER_BACK, DIK_WEBBACK )
    VK_TO_DIK( VK_BROWSER_FORWARD, DIK_WEBFORWARD )
    VK_TO_DIK( VK_BROWSER_REFRESH, DIK_WEBREFRESH )
    VK_TO_DIK( VK_BROWSER_STOP, DIK_WEBSTOP )
    VK_TO_DIK( VK_BROWSER_SEARCH, DIK_WEBSEARCH )
    VK_TO_DIK( VK_BROWSER_FAVORITES, DIK_WEBFAVORITES )
    VK_TO_DIK( VK_BROWSER_HOME, DIK_WEBHOME )
    VK_TO_DIK( VK_VOLUME_MUTE, DIK_MUTE )
    VK_TO_DIK( VK_VOLUME_DOWN, DIK_VOLUMEDOWN )
    VK_TO_DIK( VK_VOLUME_UP, DIK_VOLUMEUP )
    VK_TO_DIK( VK_MEDIA_NEXT_TRACK, DIK_NEXTTRACK )
    VK_TO_DIK( VK_MEDIA_PREV_TRACK, DIK_PREVTRACK )
    VK_TO_DIK( VK_MEDIA_STOP, DIK_MEDIASTOP )
    VK_TO_DIK( VK_MEDIA_PLAY_PAUSE, DIK_PLAYPAUSE )
    VK_TO_DIK( VK_LAUNCH_MAIL, DIK_MAIL )
    VK_TO_DIK( VK_LAUNCH_MEDIA_SELECT, DIK_MEDIASELECT )

#endif /* _WIN32_WINNT >= 0x0500 */

    VK_TO_DIK( VK_OEM_1, DIK_SEMICOLON )
    VK_TO_DIK( VK_OEM_PLUS, DIK_EQUALS )
    VK_TO_DIK( VK_OEM_COMMA, DIK_COMMA )
    VK_TO_DIK( VK_OEM_MINUS, DIK_MINUS )
    VK_TO_DIK( VK_OEM_PERIOD, DIK_PERIOD )
    VK_TO_DIK( VK_OEM_2, DIK_SLASH )
    VK_TO_DIK( VK_OEM_3, DIK_GRAVE )
    VK_TO_DIK( VK_OEM_4, DIK_LBRACKET )
    VK_TO_DIK( VK_OEM_5, DIK_BACKSLASH )
    VK_TO_DIK( VK_OEM_6, DIK_RBRACKET )
    VK_TO_DIK( VK_OEM_7, DIK_APOSTROPHE )
    VK_TO_DIK( VK_OEM_102, DIK_OEM_102 )

}//CKTDIDeviceWinKey::SetupVKToDIKMap()

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
BOOL CKTDIDeviceWinKey::ReleaseAllButtonsAndKeys()
{
	ZeroMemory( &m_State, 256 );

	for( short i = 0; i < 256; i++ )
		m_Locks[i]  = FALSE;

	m_bExtraEnter = false;

	return TRUE;
}
#endif

#endif //CHANGE_KEY_DEVICE
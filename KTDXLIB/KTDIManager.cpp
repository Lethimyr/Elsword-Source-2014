#include "StdAfx.h"
#include ".\ktdimanager.h"

#ifdef KEY_MAPPING
#define DOUBLE_INPUT_GAP 0.4f
//#define INPUT_DATA_BUFFER 20
#define AXIS_THRESHOLD  500
#endif KEY_MAPPING

CKTDIManager::CKTDIManager( HWND hWnd, HINSTANCE hInst )
{
	m_hWnd = hWnd;
	m_hInst = hInst;

	m_KTDIKeyboard	= NULL;
	m_KTDIMouse		= NULL;
	m_KTDIJoystic	= NULL;

	m_bEnabled		= true;

#ifdef KEY_MAPPING
	m_bJoyEnable	= false;
#ifdef SERV_KEY_MAPPING_INT
	m_bKeyChanging = false;
#endif // SERV_KEY_MAPPING_INT
	ZeroMemory( m_State, sizeof(bool)*GAMEACTION_END);
#endif KEY_MAPPING

	Init();

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
	m_bActivated = false;
#endif
}

CKTDIManager::~CKTDIManager()
{
	SAFE_DELETE(m_KTDIKeyboard);
	SAFE_DELETE(m_KTDIMouse);
	SAFE_DELETE(m_KTDIJoystic);
	SAFE_RELEASE(m_pDI);
}

HRESULT CKTDIManager::OnFrameMove( double fTime, float fElapsedTime )
{
	UpdateData( fElapsedTime );

	return S_OK;
}

void CKTDIManager::CreateKeyboard()
{
#ifdef  CHANGE_KEY_DEVICE
	m_KTDIKeyboard = new CKTDIDeviceWinKey();
	m_KTDIKeyboard->Create();
#else
	m_KTDIKeyboard = new CKTDIDevice( m_hWnd, m_pDI );
	m_KTDIKeyboard->Create( KEYBOARD );
#endif
}

void CKTDIManager::CreateMouse( BOOL windowed )
{
	m_KTDIMouse = new CKTDIDevice( m_hWnd, m_pDI );
	m_KTDIMouse->Create( MOUSE, windowed );
}

void CKTDIManager::CreateJoystic()
{
	m_KTDIJoystic = new CKTDIDevice( m_hWnd, m_pDI );
	m_KTDIJoystic->Create( JOYSTICK );
}

void CKTDIManager::DestroyKeyboard()
{
	SAFE_DELETE(m_KTDIKeyboard);
}

void CKTDIManager::DestroyMouse()
{
	SAFE_DELETE(m_KTDIMouse);
}

void CKTDIManager::DestroyJoystic()
{
	SAFE_DELETE(m_KTDIJoystic);
}

void CKTDIManager::UpdateData( float fElapsedTime )
{

	if( m_bEnabled )
	{
#ifdef KEY_MAPPING
		ZeroMemory( &m_State, GAMEACTION_END*sizeof(bool) );
#endif KEY_MAPPING

		if( m_KTDIKeyboard != NULL )
		{
#ifdef CHANGE_KEY_DEVICE
			//#ifdef  KEYPROCESS_BY_WINMSG
			//            MsgPeek_KeyMsgOnly();
			//            DWORD   dwTimestamp = 0;
			//            if ( g_pKTDXApp != NULL )
			//            {
			//                dwTimestamp = g_pKTDXApp->AdjustFrameTimestamp();
			//            }//if
			//			m_KTDIKeyboard->Read( dwTimestamp, fElapsedTime );
			//#else   KEYPROCESS_BY_WINMSG
			m_KTDIKeyboard->Read( g_pKTDXApp->GetFrameTimestamp(), fElapsedTime );
			//#endif  KEYPROCESS_BY_WINMSG
#else
			m_KTDIKeyboard->Read( fElapsedTime );
#endif			
		}//ikf

		if( m_KTDIMouse != NULL )
		{
			m_KTDIMouse->Read( fElapsedTime );

			// Update Mouse Delta Position
			m_KTDIMouse->UpdateMouseDeltaPosition();
		}

		if( m_KTDIJoystic != NULL )
			m_KTDIJoystic->Read( fElapsedTime );

#ifdef KEY_MAPPING
		for(int i=0; i<GAMEACTION_TOTAL; ++i)
		{
			if( i == GAMEACTION_END )
				continue;

#ifdef SERV_KEY_MAPPING_INT
			// 조이스틱
			if( m_bJoyEnable && m_KTDIJoystic != NULL )
			{
				bool bKeyChanging = false;
				if( true == m_KTDIJoystic->GetActionState(i) && m_bKeyChanging == true )
				{
					bKeyChanging = true;
				}

				if( bKeyChanging == true )
				{
				}
				else
				{
					HandleAction( i, m_KTDIJoystic->GetActionState( i ) );
				}
			}
#endif SERV_KEY_MAPPING_INT

			// 키보드
			if (m_KTDIKeyboard != NULL )
			{
				HandleAction(i, m_KTDIKeyboard->GetActionState(i) );
			}
		}

		for(int i=0; i<GAMEACTION_END; ++i)
		{
			ApplyActionInput((GAME_ACTION)i, fElapsedTime );
		}

#endif KEY_MAPPING
	}
}

bool CKTDIManager::Init()
{
	if(FAILED(DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
	{
		ErrorLog(KEM_ERROR62);
		return false;
	}

	return true;
}

#ifdef KEY_MAPPING
BOOL  CKTDIManager::GetActionState( GAME_ACTION Action )
{
	// Check if key/button is pressed
	if( m_State[Action] && m_Locks[Action] == false )
	{
		m_Locks[Action] = true;
		return TRUE;
	}

	return FALSE;
}
BOOL  CKTDIManager::SetActionState( GAME_ACTION Action, BOOL State )
{
	if( State == TRUE )
		m_State[Action] = true;
	else
		m_State[Action] = false | m_State[Action];		// true였으면 유지시킨다 (매 프레임 false로 리셋해주므로.. 같은 액션에 여러번 처리될때에 대비)

	return TRUE;
}


BOOL  CKTDIManager::GetPureActionState( GAME_ACTION Action )
{
	return (m_State[Action]) ? TRUE : FALSE;
}
BOOL  CKTDIManager::GetDoubleActionPureState( GAME_ACTION Action )
{
	if( m_DoubleKeyCheck[Action].bDoubleOK != true )
	{
		if( m_DoubleKeyCheck[Action].bFirstDown == false )
		{
			if( m_State[Action] && m_Locks[Action] == false )
			{
				m_DoubleKeyCheck[Action].bFirstDown = true;
				m_DoubleKeyCheck[Action].fTimeOut = DOUBLE_INPUT_GAP;
			}
			return FALSE;

		}
		else
		{
			if( m_DoubleKeyCheck[Action].bKeyUp == false )
			{
				if( GetPureActionState( Action ) == FALSE )
				{
					m_DoubleKeyCheck[Action].bKeyUp = true;
				}
				return FALSE;
			}
			else if( m_State[Action] && m_Locks[Action] == false )
			{
				m_DoubleKeyCheck[Action].bDoubleOK = true;
			}
			else
				return FALSE;
		}
	}

	if( m_DoubleKeyCheck[Action].bDoubleOK == true
		&& GetPureActionState( Action ) == TRUE )
		return TRUE;
	else
	{
		m_DoubleKeyCheck[Action].Init();
		return FALSE;
	}

}
BOOL  CKTDIManager::GetDoubleActionState( GAME_ACTION Action )
{
	if( m_DoubleKeyCheck[Action].bDoubleOK != true )
	{
		if( m_DoubleKeyCheck[Action].bFirstDown == false )
		{
			if( m_State[Action] && m_Locks[Action] == false )
			{
				m_DoubleKeyCheck[Action].bFirstDown = true;
				m_DoubleKeyCheck[Action].fTimeOut = DOUBLE_INPUT_GAP;
			}
			return FALSE;

		}
		else
		{
			if( m_DoubleKeyCheck[Action].bKeyUp == false )
			{
				if( GetPureActionState( Action ) == FALSE )
				{
					m_DoubleKeyCheck[Action].bKeyUp = true;
				}
				return FALSE;
			}
			else if( m_State[Action] && m_Locks[Action] == false )
			{
				m_DoubleKeyCheck[Action].bDoubleOK = true;
			}
			else
				return FALSE;
		}
	}

	if( m_DoubleKeyCheck[Action].bDoubleOK == true
		&& m_DoubleKeyCheck[Action].bDoubleKeyLock == false
		&& GetPureActionState( Action ) == TRUE )
	{
		m_DoubleKeyCheck[Action].bDoubleKeyLock = true;
		return TRUE;
	}
	else
	{
		m_DoubleKeyCheck[Action].Init();
		return FALSE;
	}

}
void CKTDIManager::SetDoubleActionReset( GAME_ACTION Action )
{
	m_DoubleKeyCheck[Action].bDoubleOK			= false;
	m_DoubleKeyCheck[Action].bDoubleKeyLock	= false;
	m_DoubleKeyCheck[Action].bFirstDown		= false;
	m_DoubleKeyCheck[Action].bKeyUp			= false;
	m_DoubleKeyCheck[Action].fTimeOut			= 0.0f;
}

BOOL CKTDIManager::GetLock( GAME_ACTION Action )
{
	if( m_Locks[Action] )
		return TRUE;
	else
		return FALSE;

}
BOOL CKTDIManager::SetLock( GAME_ACTION Action, BOOL State )
{
	if(State == TRUE)
		m_Locks[Action] = true;
	else
		m_Locks[Action] = false;

	return TRUE;
}

void CKTDIManager::HandleAction(UINT nAction, bool bState, long lData )
{
	int nAxisPos = lData;

	SetActionState((GAME_ACTION)nAction, bState);
}

void CKTDIManager::ApplyActionInput( GAME_ACTION Action, float fElapsedTime )
{
	if( !m_State[Action] )
		m_Locks[Action] = false;

	if( m_DoubleKeyCheck[Action].bFirstDown == true 
		&& m_DoubleKeyCheck[Action].bDoubleOK == false )
	{
		m_DoubleKeyCheck[Action].fTimeOut -= fElapsedTime;
		if( m_DoubleKeyCheck[Action].fTimeOut < 0.0f )
		{
			m_DoubleKeyCheck[Action].Init();						
		}
	}

}

bool CKTDIManager::ListenDeviceInput( unsigned char& ucoutUserInput, InputDevices eType )
{
	switch(eType)
	{
	case KEYBOARD:
		{
			if(m_KTDIKeyboard != NULL )
			{
#ifdef SERV_KEY_MAPPING_INT
				return m_KTDIKeyboard->ListenDeviceInput( ucoutUserInput );
#else SERV_KEY_MAPPING_INT
				for(int i=0; i<256; ++i)
				{
					if( m_KTDIKeyboard->GetKeyState(i) == TRUE )
					{
						ucoutUserInput = (unsigned char)i;
						return true;
					}
				}
#endif SERV_KEY_MAPPING_INT
			}			
		}
		break;
	case MOUSE:
		break;
#ifdef SERV_KEY_MAPPING_INT
	case JOYSTICK:
		{
			if( m_KTDIJoystic != NULL )
			{
				return m_KTDIJoystic->ListenDeviceInput( ucoutUserInput );
			}
		}
		break;
#endif SERV_KEY_MAPPING_INT
	}
	return false;
}

#ifdef SERV_KEY_MAPPING_INT
void CKTDIManager::SetKeyChanging( bool bKeyChanging )
{
	m_bKeyChanging = bKeyChanging;
}
void CKTDIManager::SetGamePadDefaultMap()
{
	// Default Mapping
	if(m_KTDIJoystic != NULL)
	{
		m_KTDIJoystic->SetAction( GAMEACTION_UP,					JB_UP );
		m_KTDIJoystic->SetAction( GAMEACTION_DOWN,					JB_DOWN );
		m_KTDIJoystic->SetAction( GAMEACTION_LEFT,					JB_LEFT );
		m_KTDIJoystic->SetAction( GAMEACTION_RIGHT,					JB_RIGHT );

		m_KTDIJoystic->SetAction( GAMEACTION_ATTACK_FAST,			JB_BUTTON2 );
		m_KTDIJoystic->SetAction( GAMEACTION_ATTACK_STRONG,			JB_BUTTON3 );
		m_KTDIJoystic->SetAction( GAMEACTION_SKILL1,				JB_BUTTON0 );
		m_KTDIJoystic->SetAction( GAMEACTION_SKILL2,				JB_BUTTON1 );
		m_KTDIJoystic->SetAction( GAMEACTION_SKILL3,				JB_BUTTON6 );
		m_KTDIJoystic->SetAction( GAMEACTION_SKILL4,				JB_BUTTON4 );
		m_KTDIJoystic->SetAction( GAMEACTION_AWAKE,					JB_BUTTON5 );
		m_KTDIJoystic->SetAction( GAMEACTION_SLOT_CHANGE,			JB_BUTTON7 );

		m_KTDIJoystic->SetAction( GAMEACTION_RETURN,				JB_BUTTON9 );
		m_KTDIJoystic->SetAction( GAMEACTION_ESCAPE,				JB_BUTTON8 );
		m_KTDIJoystic->SetAction( GAMEACTION_PARTYREADY,			JB_BUTTON10 );

		m_KTDIJoystic->SetAction( GAMEACTION_QUICKSLOT1,			JB_BUTTON11 );
		m_KTDIJoystic->SetAction( GAMEACTION_QUICKSLOT2,			JB_BUTTON12 );
		m_KTDIJoystic->SetAction( GAMEACTION_QUICKSLOT3,			JB_BUTTON13 );
		m_KTDIJoystic->SetAction( GAMEACTION_QUICKSLOT4,			JB_BUTTON14 );
		m_KTDIJoystic->SetAction( GAMEACTION_QUICKSLOT5,			JB_BUTTON15 );
		m_KTDIJoystic->SetAction( GAMEACTION_QUICKSLOT6,			JB_BUTTON16 );
	}
}
#endif // SERV_KEY_MAPPING_INT

void CKTDIManager::SetDefaultMap()
{
	// Default Mapping

	if( m_KTDIKeyboard != NULL )
	{
		m_KTDIKeyboard->SetAction( GAMEACTION_ATTACK_FAST, DIK_Z );
		m_KTDIKeyboard->SetAction( GAMEACTION_ATTACK_STRONG, DIK_X );

		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL1, DIK_A );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL2, DIK_S );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL3, DIK_D );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL4, DIK_C );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL_EX1, DIK_Q );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL_EX2, DIK_W );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL_EX3, DIK_E );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL_EX4, DIK_R );

		//m_KTDIKeyboard->SetAction( GAMEACTION_SCREENSHOT_MODE, DIK_DELETE );
		m_KTDIKeyboard->SetAction( GAMEACTION_SLOT_CHANGE, DIK_SPACE );
		m_KTDIKeyboard->SetAction( GAMEACTION_PVE, DIK_LBRACKET );
		m_KTDIKeyboard->SetAction( GAMEACTION_PVP, DIK_RBRACKET );
		m_KTDIKeyboard->SetAction( GAMEACTION_ZOOMIN, DIK_EQUALS );
		m_KTDIKeyboard->SetAction( GAMEACTION_ZOOMOUT, DIK_MINUS );
		m_KTDIKeyboard->SetAction( GAMEACTION_QUEST, DIK_L );
		m_KTDIKeyboard->SetAction( GAMEACTION_MAP, DIK_M );
		m_KTDIKeyboard->SetAction( GAMEACTION_COMMUNITY, DIK_T );
		m_KTDIKeyboard->SetAction( GAMEACTION_INFO, DIK_U );
		m_KTDIKeyboard->SetAction( GAMEACTION_INVENTORY, DIK_I );
		m_KTDIKeyboard->SetAction( GAMEACTION_OPTION, DIK_O );
		m_KTDIKeyboard->SetAction( GAMEACTION_PET, DIK_P );
		m_KTDIKeyboard->SetAction( GAMEACTION_GUILD, DIK_G );
		m_KTDIKeyboard->SetAction( GAMEACTION_SKILL_TREE, DIK_K );
		m_KTDIKeyboard->SetAction( GAMEACTION_NAME, DIK_N );
		m_KTDIKeyboard->SetAction( GAMEACTION_FRIEND, DIK_Y );
		m_KTDIKeyboard->SetAction( GAMEACTION_AWAKE, DIK_LCONTROL );

		m_KTDIKeyboard->SetAction( GAMEACTION_QUICKSLOT1, DIK_1 );
		m_KTDIKeyboard->SetAction( GAMEACTION_QUICKSLOT2, DIK_2 );
		m_KTDIKeyboard->SetAction( GAMEACTION_QUICKSLOT3, DIK_3 );
		m_KTDIKeyboard->SetAction( GAMEACTION_QUICKSLOT4, DIK_4 );
		m_KTDIKeyboard->SetAction( GAMEACTION_QUICKSLOT5, DIK_5 );
		m_KTDIKeyboard->SetAction( GAMEACTION_QUICKSLOT6, DIK_6 );

		m_KTDIKeyboard->SetAction( GAMEACTION_UP,		DIK_UP );
		m_KTDIKeyboard->SetAction( GAMEACTION_DOWN,		DIK_DOWN );
		m_KTDIKeyboard->SetAction( GAMEACTION_LEFT,		DIK_LEFT );
		m_KTDIKeyboard->SetAction( GAMEACTION_RIGHT,	DIK_RIGHT );
		m_KTDIKeyboard->SetAction( GAMEACTION_SIT,		DIK_V );

#ifdef RIDING_SYSTEM
		m_KTDIKeyboard->SetAction( GAMEACTION_RIDING,		DIK_F );
#endif // RIDING_SYSTEM
#ifdef ADDED_RELATIONSHIP_SYSTEM
		m_KTDIKeyboard->SetAction( GAMEACTION_RELATION,		DIK_H );
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef SERV_ADD_WARP_BUTTON
		m_KTDIKeyboard->SetAction( GAMEACTION_WARP,		DIK_B );
#endif // SERV_ADD_WARP_BUTTON

	}

}

#ifdef KEY_MAPPING_UPGRADE
bool CKTDIManager::GetNotCompleteBaseKey()
{
	for(int i = GAMEACTION_UP; i <= GAMEACTION_CHANGE_SKILLSLOT; i++)
	{
		if(m_KTDIKeyboard->GetActionKey(i) == 0)
		{
			return false;
		}
	}	
	return true;
}

bool CKTDIManager::GetNotCompleteQuickKey()
{
	for(int i = GAMEACTION_SPECIALA; i <= GAMEACTION_QUICKSLOT6; i++)
	{
		if(m_KTDIKeyboard->GetActionKey(i) == 0)
		{
			return false;
		}
	}	
	return true;
}

bool CKTDIManager::GetNotCompleteSysKey()
{
	for(int i = GAMEACTION_HELP; i <= GAMEACTION_PARTYREADY; i++)
	{
		if(m_KTDIKeyboard->GetActionKey(i) == 0)
		{
			return false;
		}
	}	
	return true;
}

bool CKTDIManager::GetNotCompleteOtherKey()
{
	for(int i = GAMEACTION_QUEST_WINDOW; i <= GAMEACTION_GUILD_WINDOW; i++)
	{
		if(m_KTDIKeyboard->GetActionKey(i) == 0)
		{
			return false;
		}
	}	
	return true;
}
#endif KEY_MAPPING_UPGRADE

#endif KEY_MAPPING

#ifdef  CHANGE_KEY_DEVICE
//void    CKTDIManager::MsgPeek_KeyMsgOnly( HACCEL hAccel )
//{
//    if ( m_KTDIKeyboard == NULL
//        || m_KTDIKeyboard->IsActivated() == false )
//        return;
//
//    std::pair<MSG,WORD> pairMSGVirtualKey;
//    MSG& msg = pairMSGVirtualKey.first;
//
//    HWND hWnd = DXUTGetHWND();
//
//
//    //m_bSkipKeyProcess = false;
//
//    while( 1 )
//    {
//        DWORD dwRet = ::GetQueueStatus( QS_KEY );
//        WORD    wHigh = HIWORD( dwRet );
//
//        if ( wHigh != QS_KEY )
//            break;
//
//        //m_bInPeek = true;
//        if ( PeekMessage( &msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE | PM_QS_INPUT ) == 0 )
//        {
//            //m_bInPeek = false;
//            break;
//        }//if
//        //m_bInPeek = false;
//
//		//{{ dmlee 2008.04.07 - 넥슨캐시충전페이지에서 주민번호입력시 다음 editbox로 자동이동할 수 있도록 keyup event 처리
//		if( WM_KEYUP == msg.message )
//		{
//			if( NULL != g_pKTDXApp )
//			{
//				g_pKTDXApp->OnWebBrowserKeyEvents( &msg );
//			}
//		}
//		//}} dmlee 2008.04.07 - 넥슨캐시충전페이지에서 주민번호입력시 다음 editbox로 자동이동할 수 있도록 keyup event 처리
//
//        // Translate and dispatch the message
//        if( hAccel == NULL || hWnd == NULL || 
//            0 == TranslateAccelerator( hWnd, hAccel, &msg ) )
//        {
//            pairMSGVirtualKey.second = 0;
//            if( msg.message == WM_KEYDOWN || msg.message == WM_KEYUP 
//                || msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP )
//            {
//                pairMSGVirtualKey.second = ( msg.wParam == VK_PROCESSKEY ) 
//                    ? ImmGetVirtualKey( hWnd ) : msg.wParam;
//                DXUTSetVirtualKey( pairMSGVirtualKey.second );
//            }//if
//
//			DXUTSetLastPostMessageTime( msg.time );
//            TranslateMessage( &msg );
//            MsgProc_KeyMsg( msg.hwnd, msg.message, msg.wParam, msg.lParam );
//            m_dequeWinMsg.push_back( pairMSGVirtualKey );
//        }
//    }
//
//}//CKTDIManager::MsgPeek_KeyMsgOnly()


//bool    CKTDIManager::MsgPeek_QueuedKeyMsg( MSG& msg )
//{
//    if ( m_dequeWinMsg.empty() == false )
//    {
//        msg = m_dequeWinMsg.front().first;
//        if ( ( msg.message == WM_KEYDOWN || msg.message == WM_KEYUP 
//                || msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP ) )
//        {
//            DXUTSetVirtualKey( m_dequeWinMsg.front().second );
//        }//if
//        m_dequeWinMsg.pop_front();
//        //m_bSkipKeyProcess = true;
//        return true;
//    }//if.. else..
//
//    //m_bSkipKeyProcess = false;
//    return false;
//}//CKTDIManager::MsgPeek_QueuedKeyMsg()


void    CKTDIManager::MsgProc_KeyMsg(     HWND hWnd_
									 , UINT uMsg_
									 , WPARAM wParam_
									 , LPARAM lParam_ )
{
	//if ( m_bSkipKeyProcess == false )
	//{
	if ( m_KTDIKeyboard != NULL )
		m_KTDIKeyboard->MsgProc_KeyMsg( hWnd_, uMsg_, wParam_, lParam_ );
	//}//if
}//CKTDIManager::MsgProc_KeyMsg()
#endif  CHANGE_KEY_DEVICE

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
bool CKTDIManager::IsActivated()
{
	return m_bActivated;
}
void CKTDIManager::ActivateApp()
{
	m_bActivated = true;
}
void CKTDIManager::DeactivateApp()
{
	m_bActivated = false;

	if ( m_KTDIKeyboard != NULL )
		m_KTDIKeyboard->ReleaseAllButtonsAndKeys();

	if ( m_KTDIMouse != NULL )
		m_KTDIMouse->ReleaseAllButtonsAndKeys();

	if ( m_KTDIJoystic != NULL )
		m_KTDIJoystic->ReleaseAllButtonsAndKeys();

}//CKTDIManager::ReleaseAllButtonsAndKeys()
#endif
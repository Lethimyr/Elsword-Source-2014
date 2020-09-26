#include "StdAfx.h"
#include "X2KeyPad.h"


#ifdef SERV_KEY_MAPPING_INT
// 이 클래스에선 게임패드용 매핑을 건드리면 안 됩니다. by 박진웅
#define GAMEACTION_END GAMEACTION_RETURN
#endif SERV_KEY_MAPPING_INT

#ifdef KEYPAD_MAPPING
const int CX2KeyPad::USE_KEY_NUMBER = CX2KeyPad::KBI_END;
const int CX2KeyPad::DISUSE_KEY_NUMBER = 63;
#else KEYPAD_MAPPING
const int CX2KeyPad::USE_KEY_NUMBER = 63;
const int CX2KeyPad::DISUSE_KEY_NUMBER = 51;
#endif KEYPAD_MAPPING

const unsigned char CX2KeyPad::USE_DIK_LIST[USE_KEY_NUMBER] =
{
	0x00, DIK_GRAVE, DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, DIK_9, DIK_0,
	DIK_MINUS, DIK_EQUALS, DIK_BACKSPACE, DIK_TAB, DIK_Q, DIK_W, DIK_E, DIK_R, DIK_T,
	DIK_Y, DIK_U, DIK_I, DIK_O, DIK_P, DIK_LBRACKET, DIK_RBRACKET, DIK_BACKSLASH,
	DIK_CAPITAL, DIK_A, DIK_S, DIK_D, DIK_F, DIK_G, DIK_H, DIK_J, DIK_K, DIK_L, DIK_SEMICOLON,
	DIK_APOSTROPHE, DIK_RETURN, DIK_LSHIFT, DIK_Z, DIK_X, DIK_C, DIK_V, DIK_B, DIK_N,
	DIK_M, DIK_COMMA, DIK_PERIOD, DIK_SLASH, DIK_RSHIFT, DIK_UP, DIK_LCONTROL,
	DIK_LALT, DIK_SPACE, DIK_RALT, DIK_RCONTROL, DIK_LEFT, DIK_DOWN, DIK_RIGHT,
#ifdef KEYPAD_MAPPING
	DIK_NUMLOCK, DIK_DIVIDE, DIK_MULTIPLY, DIK_SUBTRACT, DIK_ADD,
	DIK_NUMPAD0, DIK_NUMPAD1, DIK_NUMPAD2, DIK_NUMPAD3, DIK_NUMPAD4, DIK_NUMPAD5, DIK_NUMPAD6, DIK_NUMPAD7, DIK_NUMPAD8, DIK_NUMPAD9,
	DIK_DECIMAL, DIK_NUMPADENTER,
#endif KEYPAD_MAPPING
};
#ifdef KEY_MAPPING_USE_STRING_TABLE
const int CX2KeyPad::KEYPAD_STRING_INDEX[CX2KeyPad::KBI_END] = 
{
	STR_ID_24924, STR_ID_24925, STR_ID_24926, STR_ID_24927, STR_ID_24928, STR_ID_24929, STR_ID_24930, STR_ID_24931, STR_ID_24932, STR_ID_24933,
	STR_ID_24934, STR_ID_24935, STR_ID_24936, STR_ID_24937, STR_ID_24938, STR_ID_24939, STR_ID_24940, STR_ID_24941, STR_ID_24942, STR_ID_24943,
	STR_ID_24944, STR_ID_24945, STR_ID_24946, STR_ID_24947, STR_ID_24948, STR_ID_24949, STR_ID_24950, STR_ID_24951, STR_ID_24952, STR_ID_24953,
	STR_ID_24954, STR_ID_24955, STR_ID_24956, STR_ID_24957, STR_ID_24958, STR_ID_24959, STR_ID_24960, STR_ID_24961, STR_ID_24962, STR_ID_24963,
	STR_ID_24964, STR_ID_24965, STR_ID_24966, STR_ID_24967, STR_ID_24968, STR_ID_24969, STR_ID_24970, STR_ID_24971, STR_ID_24972, STR_ID_24973,
	STR_ID_24974, STR_ID_24975, STR_ID_24976, STR_ID_24977, STR_ID_24978, STR_ID_24979, STR_ID_24980, STR_ID_24981, STR_ID_24982, STR_ID_24983,
	STR_ID_24984, STR_ID_24985, STR_ID_24986,
#ifdef KEYPAD_MAPPING
	STR_ID_24987, STR_ID_24988, STR_ID_24989, STR_ID_24990, STR_ID_24991, STR_ID_24992, STR_ID_24993,
	STR_ID_24994, STR_ID_24995, STR_ID_24996, STR_ID_24997, STR_ID_24998, STR_ID_24999, STR_ID_25000, STR_ID_25001, STR_ID_25002, STR_ID_25003,
#endif KEYPAD_MAPPING
};
#else KEY_MAPPING_USE_STRING_TABLE
const wstring CX2KeyPad::KEYPAD_STRING[CX2KeyPad::KBI_END] = 
{
	L"", L"~", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"-", L"=", L"Back", 
	L"Tab", L"Q", L"W", L"E", L"R", L"T", L"Y", L"U", L"I", L"O", L"P", L"[", L"]", L"\\", 
	L"Caps", L"A", L"S", L"D", L"F", L"G", L"H", L"J", L"K", L"L", L";", L"'", L"Enter", 
	L"LShift", L"Z", L"X", L"C", L"V", L"B", L"N", L"M", L",", L".", L"/", L"RShift", L"Up",
	L"LCtrl", L"LAlt", L"Space", L"RAlt", L"RCtrl", L"Left", L"Down", L"Right"
#ifdef KEYPAD_MAPPING
	, L"NmLk", L"/", L"*", L"-", L"+",
	L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9",
	L"Del", L"Enter",
#endif KEYPAD_MAPPING
};
#endif KEY_MAPPING_USE_STRING_TABLE

#pragma region CX2KeyPad

	#pragma region CX2KeyPad
	/** @function 	: CX2KeyPad
		@brief 		: Map 데이터를 초기화한다.
	*/
	CX2KeyPad::CX2KeyPad(void)
		: m_DLGKeyPad( NULL )
		, m_bShowKeyPad( false )
	{
		InitUseKeySlotData();
		InitDisUseKeySlotData();
		
		m_KeyPadMap.clear();
		m_DeviceKeyMap.clear();
		m_KeyStringMap.clear();

		// KeyPadMap 초기화
		for( int i = 0; i < GAMEACTION_END; ++i )
		{
			m_KeyPadMap[(GAME_ACTION)i] = GAME_ACTION_NONE;
		}

		// Device Key Map, Key String Map 초기화
		for ( int i = KBI_GRAVE; i < KBI_END; ++i )
		{
			m_DeviceKeyMap.insert( value_t(USE_DIK_LIST[i], (KEY_BUTTON_INDEX)i) );
#ifdef KEY_MAPPING_USE_STRING_TABLE
			m_KeyStringMap[USE_DIK_LIST[i]] = GET_STRING( KEYPAD_STRING_INDEX[i] );
#else KEY_MAPPING_USE_STRING_TABLE
			m_KeyStringMap[USE_DIK_LIST[i]] = KEYPAD_STRING[i];
#endif KEY_MAPPING_USE_STRING_TABLE
		}
	}
	#pragma endregion 생성자

	#pragma region ~CX2KeyPad
	/** @function 	: ~CX2KeyPad
		@brief 		: KeyPad 다이얼로그 제거
	*/
	CX2KeyPad::~CX2KeyPad(void)
	{
		SAFE_DELETE_DIALOG( m_DLGKeyPad );
	}
	#pragma endregion 소멸자

	#pragma region ShowKeyPad
	/** @function 	: ShowKeyPad
		@brief 		: 키 패드 UI 활성화 / 비활성화
	*/
	void CX2KeyPad::ShowKeyPad( IN bool bShow_ )
	{
		if ( true == bShow_ )
		{
			SAFE_DELETE_DIALOG( m_DLGKeyPad );

			m_DLGKeyPad = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_KEY_SETTING.LUA" );
						
			if ( NULL != m_DLGKeyPad )
			{
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_DLGKeyPad );
				InitUseKeySlotData();
				InitDisUseKeySlotData();
				LoadSlotData();
			}
		}
		else
		{
			if ( NULL != m_DLGKeyPad )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_DLGKeyPad, NULL, false );
		}

		m_bShowKeyPad = bShow_;
	}
	#pragma endregion 함수

	#pragma region UICustomEventProc
	/** @function 	: UICustomEventProc
		@brief 		: 키 패드의 사용자 정의 이벤트를 처리한다.
	*/
	bool CX2KeyPad::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch ( wParam )
		{
		case KPUIM_EXIT:
			{
				ShowKeyPad( false );

				return true;
			} break;

		case KPUIM_DEFAULT:
			{
				DefaultSlotData();

				return true;
			} break;

		case KPUIM_CLEAR:
			{
				ClearSlotData();

				return true;
			} break;

		case KPUIM_PREV:
			{
				LoadSlotData();

				return true;
			} break;

		case KPUIM_SAVE:
			{
				SaveSlotData();
				ShowKeyPad( false );

				return true;
			} break;

		case KPUIM_CANCEL:
			{
				ShowKeyPad( false );

				return true;
			} break;

		case KPUIM_USESLOT_DROPPED:
			{
				return ProcessUseSlotDropped( lParam );
			} break;
		}

		return false;
	}
#pragma endregion 함수

	#pragma region UIServerEventProc
	bool CX2KeyPad::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		KTDXPROFILE();
		switch( wParam )
		{
		case EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK:
			{
				return Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK( hWnd, uMsg, wParam, lParam );
			} break;
		}
		return false;
	}
	#pragma endregion 함수

	#pragma region MsgProc
	/** @function 	: MsgProc
		@brief 		: 메시지 프로시저
	*/
	bool CX2KeyPad::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return false;
	}
	#pragma endregion 함수

	#pragma region ClearSlotData
	/** @function 	: ClearSlotData
		@brief 		: 모든 Slot의 SlotData를 DisUseSlot으로 보낸다
	*/
	void CX2KeyPad::ClearSlotData()
	{
		if ( NULL == m_DLGKeyPad )
			return;

		CKTDGUISlot* pSlot = NULL;

		// UseKeySlot
		for ( short i = 1; i < USE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );
			if(pSlot != NULL)
			{
				if ( pSlot->GetSlotData() )
					static_cast<CX2UseKeySlotData*>( pSlot->GetSlotData() )->Clear( i );
			}
		}

		// DisUseKeySlot
		for ( short i = USE_KEY_NUMBER; i < USE_KEY_NUMBER + DISUSE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );

			if(pSlot != NULL)
			{
				if ( pSlot->GetSlotData() )
					static_cast<CX2DisUseKeySlotData*>( pSlot->GetSlotData() )->Clear( i );
			}
		}

		// Key_Map 원래대로
		int slotIndex = USE_KEY_NUMBER;
		for( int i = 0; i < GAMEACTION_END; ++i )
		{
			m_KeyPadMap[(GAME_ACTION)i] = slotIndex;

			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( slotIndex );

			if(pSlot != NULL)
			{
				if ( pSlot->GetSlotData() )
					static_cast<CX2DisUseKeySlotData*>( pSlot->GetSlotData() )->m_eKeyActionID = (GAME_ACTION)i;
			}

			++slotIndex;
		}
	}
	#pragma endregion 함수

	#pragma region DefaultSlotData
	/** @function 	: DefaultSlotData
		@brief 		: 모든 Slot의 SlotData를 초기화 상태로 만든다.
	*/
	void CX2KeyPad::DefaultSlotData()
	{
		if ( NULL == m_DLGKeyPad )
			return;

		CKTDGUISlot* pSlot = NULL;

		// UseKeySlot
		for ( short i = 1; i < USE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );
			if(pSlot != NULL)
			{
				if ( pSlot->GetSlotData() )
					static_cast<CX2UseKeySlotData*>( pSlot->GetSlotData() )->Clear( i );
			}
		}

		// DisUseKeySlot
		for ( short i = USE_KEY_NUMBER; i < USE_KEY_NUMBER + DISUSE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );

			if(pSlot != NULL)
			{
				if ( pSlot->GetSlotData() )
					static_cast<CX2DisUseKeySlotData*>( pSlot->GetSlotData() )->Clear( i );
			}
		}

		// Key_Map 초기화
		m_KeyPadMap[GAMEACTION_QUICKSLOT1]	= KBI_1;
		m_KeyPadMap[GAMEACTION_QUICKSLOT2]	= KBI_2;
		m_KeyPadMap[GAMEACTION_QUICKSLOT3]	= KBI_3;
		m_KeyPadMap[GAMEACTION_QUICKSLOT4]	= KBI_4;
		m_KeyPadMap[GAMEACTION_QUICKSLOT5]	= KBI_5;
		m_KeyPadMap[GAMEACTION_QUICKSLOT6]	= KBI_6;

		m_KeyPadMap[GAMEACTION_ATTACK_FAST]		= KBI_Z;
		m_KeyPadMap[GAMEACTION_ATTACK_STRONG]	= KBI_X;
		m_KeyPadMap[GAMEACTION_AWAKE]			= KBI_LCTRL;
		m_KeyPadMap[GAMEACTION_SLOT_CHANGE]		= KBI_SPACE;
		m_KeyPadMap[GAMEACTION_ZOOMIN]			= KBI_EQUAL;
		m_KeyPadMap[GAMEACTION_ZOOMOUT]			= KBI_MINUS;
		m_KeyPadMap[GAMEACTION_NAME]			= KBI_N;
		
		m_KeyPadMap[GAMEACTION_SKILL1]	= KBI_A;
		m_KeyPadMap[GAMEACTION_SKILL2]	= KBI_S;
		m_KeyPadMap[GAMEACTION_SKILL3]	= KBI_D;
		m_KeyPadMap[GAMEACTION_SKILL4]	= KBI_C;
		
		m_KeyPadMap[GAMEACTION_SKILL_EX1]	= KBI_Q;
		m_KeyPadMap[GAMEACTION_SKILL_EX2]	= KBI_W;
		m_KeyPadMap[GAMEACTION_SKILL_EX3]	= KBI_E;
		m_KeyPadMap[GAMEACTION_SKILL_EX4]	= KBI_R;

		m_KeyPadMap[GAMEACTION_COMMUNITY]	= KBI_T;
		m_KeyPadMap[GAMEACTION_INFO]		= KBI_U;
		m_KeyPadMap[GAMEACTION_INVENTORY]	= KBI_I;
		m_KeyPadMap[GAMEACTION_OPTION]		= KBI_O;
		m_KeyPadMap[GAMEACTION_PET]			= KBI_P;
		m_KeyPadMap[GAMEACTION_PVE]			= KBI_LBRACKET;
		m_KeyPadMap[GAMEACTION_PVP]			= KBI_RBRACKET;
		m_KeyPadMap[GAMEACTION_SKILL_TREE]	= KBI_K;
		m_KeyPadMap[GAMEACTION_QUEST]		= KBI_L;
		m_KeyPadMap[GAMEACTION_MAP]			= KBI_M;
		m_KeyPadMap[GAMEACTION_GUILD]		= KBI_G;
		m_KeyPadMap[GAMEACTION_FRIEND]		= KBI_Y;

		m_KeyPadMap[GAMEACTION_UP]			= KBI_UP;
		m_KeyPadMap[GAMEACTION_DOWN]		= KBI_DOWN;
		m_KeyPadMap[GAMEACTION_LEFT]		= KBI_LEFT;
		m_KeyPadMap[GAMEACTION_RIGHT]		= KBI_RIGHT;

		m_KeyPadMap[GAMEACTION_SIT]			= KBI_V;
#ifdef RIDING_SYSTEM
		m_KeyPadMap[GAMEACTION_RIDING]		= KBI_F;
#endif //RIDING_SYSTEM
#ifdef SERV_ADD_WARP_BUTTON
		m_KeyPadMap[GAMEACTION_WARP]		= KBI_B;
#endif // SERV_ADD_WARP_BUTTON
#ifdef ADDED_RELATIONSHIP_SYSTEM
		m_KeyPadMap[GAMEACTION_RELATION]		= KBI_H;
#endif //ADDED_RELATIONSHIP_SYSTEM

		UpdateSlotData();
	}
	#pragma endregion 함수

	#pragma region SaveSlotData
	/** @function 	: SaveSlotData
		@brief 		: 설정한 키 값을 저장하도록 서버에 요청한다. 실패 시 저장되지 않고 이전 키값으로 적용된다.
	*/
	void CX2KeyPad::SaveSlotData( bool bfirst/*= false*/ )
	{
		if ( NULL == m_DLGKeyPad && false == bfirst )
			return;

		if ( NULL == g_pKTDXApp && NULL == g_pKTDXApp->GetDIManager() && 
			NULL == g_pKTDXApp->GetDIManager()->Getkeyboard() )
			return;

		for( int i = 0; i < GAMEACTION_END; ++i )
		{
			if ( GAME_ACTION_NONE != GAME_ACTION(i) )
			{
				int iKeyIndex = m_KeyPadMap.at( GAME_ACTION(i) );
				if ( iKeyIndex < USE_KEY_NUMBER )
				{
					unsigned char ucKey = m_DeviceKeyMap.right.at( KEY_BUTTON_INDEX( iKeyIndex ) );
					g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAME_ACTION(i), ucKey );
				}
				else
				{
					g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAME_ACTION(i), 0 );
				}
			}
		}

		Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ();
	}
	#pragma endregion 함수

	#pragma region LoadSlotData
	/** @function 	: LoadSlotData
		@brief 		: 이전에 저장된 키 세팅을 불려온다.
	*/
	void CX2KeyPad::LoadSlotData()
	{
		if ( NULL == g_pKTDXApp && NULL == g_pKTDXApp->GetDIManager() && 
			NULL == g_pKTDXApp->GetDIManager()->Getkeyboard() )
			return;

		int iDisUseIndex = USE_KEY_NUMBER;
		for( int i = 0; i < GAMEACTION_END; ++i )
		{
			unsigned char ucKey = g_pKTDXApp->GetDIManager()->Getkeyboard()->GetActionKey( GAME_ACTION(i) );

			if ( 0 != ucKey )
			{
				m_KeyPadMap[GAME_ACTION(i)] = m_DeviceKeyMap.left.at(ucKey);
			}
			else
			{
				m_KeyPadMap[GAME_ACTION(i)] = iDisUseIndex;
				++iDisUseIndex;
			}
		}

		if ( NULL == m_DLGKeyPad )
			return;

		CKTDGUISlot* pSlot = NULL;
		
		// UseKeySlot
		for ( short i = 1; i < USE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );
			if(pSlot != NULL)
			{
				if ( pSlot->GetSlotData() )
					static_cast<CX2UseKeySlotData*>( pSlot->GetSlotData() )->Clear( i );
			}
		}

		// DisUseKeySlot
		for ( short i = USE_KEY_NUMBER; i < USE_KEY_NUMBER + DISUSE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );

			if(pSlot != NULL)
			{
				if ( pSlot->GetSlotData() )
					static_cast<CX2DisUseKeySlotData*>( pSlot->GetSlotData() )->Clear( i );
			}
		}

		UpdateSlotData();
	}
	#pragma endregion 함수

	#pragma region UpdateSlotData
	/** @function 	: UpdateSlotData
		@brief 		: 변경된 키 값에 맞게 SlotDate 값을 갱신한다.
	*/
	void CX2KeyPad::UpdateSlotData()
	{
		if ( NULL == m_DLGKeyPad )
			return;

		CKTDGUISlot* pSlot = NULL;

		int slotIndex = -1;

		// 액션 키 설정
		for( int i = GAMEACTION_ATTACK_FAST; i < GAMEACTION_END; ++i )
		{
			int slotIndex = m_KeyPadMap[(GAME_ACTION)i];

			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( slotIndex );

			if( NULL != pSlot )
			{
				if ( NULL != pSlot->GetSlotData() )
				{
					if ( pSlot->GetSlotData()->m_eSlotDataType == CKTDGUISlotData::SDT_USE_KEY )
						static_cast<CX2UseKeySlotData*>( pSlot->GetSlotData() )->m_eKeyActionID = (GAME_ACTION)i;
					else if ( pSlot->GetSlotData()->m_eSlotDataType == CKTDGUISlotData::SDT_DISUSE_KEY )
						static_cast<CX2DisUseKeySlotData*>( pSlot->GetSlotData() )->m_eKeyActionID = (GAME_ACTION)i;
				}
			}

			++slotIndex;
		}
	}
	#pragma endregion 함수

	#pragma region InitUseKeySlotData
	/** @function 	: InitUseKeySlotData
		@brief 		: UseKey SlotData를 초기화한다.
	*/
	void CX2KeyPad::InitUseKeySlotData()
	{
		if ( NULL == m_DLGKeyPad )
			return;

		CKTDGUISlot* pSlot = NULL;

		for ( short i = 1; i < USE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );
			if(pSlot != NULL)
			{
				pSlot->SetSlotData(new CX2UseKeySlotData( i ));
			}
		}
	}
	#pragma endregion 함수

	#pragma region InitDisUseKeySlotData
	/** @function 	: InitDisUseKeySlotData
		@brief 		: DisUseKey SlotData를 초기화한다.
	*/
	void CX2KeyPad::InitDisUseKeySlotData()
	{
		if ( NULL == m_DLGKeyPad )
			return;

		CKTDGUISlot* pSlot = NULL;

		for ( short i = USE_KEY_NUMBER; i < USE_KEY_NUMBER + DISUSE_KEY_NUMBER; ++i )
		{
			pSlot = (CKTDGUISlot*) m_DLGKeyPad->GetControl( i );
			
			if(pSlot != NULL)
			{
				pSlot->SetSlotData(new CX2DisUseKeySlotData( i ));
			}
		}
	}
	#pragma endregion 함수

	#pragma region ProcessUseSlotDropped
	/** @function 	: ProcessUseSlotDropped
		@brief 		: USE_KEY 내에서 드래그 앤 드랍 했을 때의 처리
	*/
	bool CX2KeyPad::ProcessUseSlotDropped( IN LPARAM lParam_ )
	{
		CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam_;
		CX2UseKeySlotData* pUseSlotData = NULL;
		CX2DisUseKeySlotData* pDisUseSlotData = NULL;

		if ( NULL != pSlot )
		{
			CKTDGUISlot* pDroppedSlot = pSlot->GetDroppedSlot();
			CX2UseKeySlotData* pDroppedUseSlotData = NULL;
			CX2DisUseKeySlotData* pDroppedDisUseSlotData = NULL;

			switch ( pSlot->GetSlotData()->m_eSlotDataType )
			{
			case CKTDGUISlotData::SDT_USE_KEY:
				{
					pUseSlotData = (CX2UseKeySlotData*)pSlot->GetSlotData();

					if ( NULL != pDroppedSlot )
					{
						switch ( pDroppedSlot->GetSlotData()->m_eSlotDataType )
						{
						case CKTDGUISlotData::SDT_USE_KEY:
							{
								pDroppedUseSlotData = (CX2UseKeySlotData*)pDroppedSlot->GetSlotData();
								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );

								if ( pUseSlotData->m_eKeyActionID == pDroppedUseSlotData->m_eKeyActionID )
								{
									return false;
								}
								else
								{
									int eKeyActionID = pUseSlotData->m_eKeyActionID;
									pUseSlotData->m_eKeyActionID = pDroppedUseSlotData->m_eKeyActionID;
									pDroppedUseSlotData->m_eKeyActionID = GAME_ACTION(eKeyActionID);

									m_KeyPadMap[pUseSlotData->m_eKeyActionID] = pUseSlotData->m_iIndex;
									
									if ( GAME_ACTION_NONE != pDroppedUseSlotData->m_eKeyActionID )
										m_KeyPadMap[pDroppedUseSlotData->m_eKeyActionID] = pDroppedUseSlotData->m_iIndex;

									pUseSlotData->m_bDataUpdated = true;
									pDroppedUseSlotData->m_bDataUpdated = true;
																		
									return true;
								}
							} break;
						case CKTDGUISlotData::SDT_DISUSE_KEY:
							{
								pDroppedDisUseSlotData = (CX2DisUseKeySlotData*)pDroppedSlot->GetSlotData();
								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );

								if ( pUseSlotData->m_eKeyActionID == pDroppedDisUseSlotData->m_eKeyActionID )
								{
									return false;
								}
								else
								{
									int eKeyActionID = pUseSlotData->m_eKeyActionID;
									pUseSlotData->m_eKeyActionID = pDroppedDisUseSlotData->m_eKeyActionID;
									pDroppedDisUseSlotData->m_eKeyActionID = GAME_ACTION(eKeyActionID);

									m_KeyPadMap[pUseSlotData->m_eKeyActionID] = pUseSlotData->m_iIndex;
									
									if ( GAME_ACTION_NONE != pDroppedDisUseSlotData->m_eKeyActionID )
										m_KeyPadMap[pDroppedDisUseSlotData->m_eKeyActionID] = pDroppedDisUseSlotData->m_iIndex;									

									pUseSlotData->m_bDataUpdated = true;
									pDroppedDisUseSlotData->m_bDataUpdated = true;

									return true;
								}
							} break;
						}
					}
				} break;

			case CKTDGUISlotData::SDT_DISUSE_KEY:
				{
					pDisUseSlotData = (CX2DisUseKeySlotData*)pSlot->GetSlotData();
					
					if ( NULL != pDroppedSlot )
					{
						switch ( pDroppedSlot->GetSlotData()->m_eSlotDataType )
						{
						case CKTDGUISlotData::SDT_USE_KEY:
							{
								pDroppedUseSlotData = (CX2UseKeySlotData*)pDroppedSlot->GetSlotData();
								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );

								if ( pDisUseSlotData->m_eKeyActionID == pDroppedUseSlotData->m_eKeyActionID )
								{
									return false;
								}
								else
								{
									int eKeyActionID = pDisUseSlotData->m_eKeyActionID;
									pDisUseSlotData->m_eKeyActionID = pDroppedUseSlotData->m_eKeyActionID;
									pDroppedUseSlotData->m_eKeyActionID = GAME_ACTION(eKeyActionID);

									m_KeyPadMap[pDisUseSlotData->m_eKeyActionID] = pDisUseSlotData->m_iIndex;

									if ( GAME_ACTION_NONE != pDroppedUseSlotData->m_eKeyActionID )
										m_KeyPadMap[pDroppedUseSlotData->m_eKeyActionID] = pDroppedUseSlotData->m_iIndex;

									pDisUseSlotData->m_bDataUpdated = true;
									pDroppedUseSlotData->m_bDataUpdated = true;

									return true;
								}
							} break;
						case CKTDGUISlotData::SDT_DISUSE_KEY:
							{
								pDroppedDisUseSlotData = (CX2DisUseKeySlotData*)pDroppedSlot->GetSlotData();
								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );

								if ( pDisUseSlotData->m_eKeyActionID == pDroppedDisUseSlotData->m_eKeyActionID )
								{
									return false;
								}
								else
								{
									int eKeyActionID = pDisUseSlotData->m_eKeyActionID;
									pDisUseSlotData->m_eKeyActionID = pDroppedDisUseSlotData->m_eKeyActionID;
									pDroppedDisUseSlotData->m_eKeyActionID = GAME_ACTION(eKeyActionID);

									m_KeyPadMap[pDisUseSlotData->m_eKeyActionID] = pDisUseSlotData->m_iIndex;
									
									if ( GAME_ACTION_NONE != pDroppedDisUseSlotData->m_eKeyActionID )
										m_KeyPadMap[pDroppedDisUseSlotData->m_eKeyActionID] = pDroppedDisUseSlotData->m_iIndex;

									pDisUseSlotData->m_bDataUpdated = true;
									pDroppedDisUseSlotData->m_bDataUpdated = true;
									
									return true;
								}
							} break;
						}
					}
				} break;

			default:
				break;
			}
		}		

		return false;
	}
	#pragma endregion 함수

	#pragma region Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ
	/** @function 	: Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ
		@brief 		: 저장 요청
	*/
	bool CX2KeyPad::Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ()
	{
		KEGS_KEYBOARD_MAPPING_INFO_WRITE_REQ  kPacket;
		kPacket.m_mapKeyboardMappingInfo.clear();

		for( int i = 0; i < GAMEACTION_END; ++i )
		{
			kPacket.m_mapKeyboardMappingInfo[GAME_ACTION(i)]	= m_KeyPadMap[GAME_ACTION(i)];
		}

		g_pData->GetServerProtocol()->SendPacket( EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK );

		return true;
	}
	#pragma endregion 함수

	#pragma region Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK
	/** @function 	: Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK
		@brief 		: 저장 요청 확인
	*/
	bool CX2KeyPad::Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_KEYBOARD_MAPPING_INFO_WRITE_ACK kEvent;
		DeSerialize( pBuff, &kEvent );

		if( g_pMain->DeleteServerPacket( EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK ) == true )
		{
			if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
				UpdateEtcString();
				return true;
			}
		}

		return false;
	}
	#pragma endregion 함수

#ifdef STRING_KEYMARK
	wstring CX2KeyPad::GetKeyString( int iKeyIndex )
	{
		if ( iKeyIndex != -1 && iKeyIndex < USE_KEY_NUMBER )
		{
			unsigned char ucKey = m_DeviceKeyMap.right.at( KEY_BUTTON_INDEX( iKeyIndex ) );
			KeyStringMap::const_iterator cIter = m_KeyStringMap.find( ucKey );

			if ( cIter == m_KeyStringMap.end() )
				return L"";
			else
				return cIter->second;
		}

		return L"";
	}
#endif STRING_KEYMARK

	#pragma region GetKeyString
	/** @function 	: GetKeyString
		@brief 		: 복잡하게 돌고 돌아서 String을 넘겨준다.
	*/
	wstring CX2KeyPad::GetKeyString( GAME_ACTION eActionKey_ )
	{
		int iKeyIndex = m_KeyPadMap.at( eActionKey_ );

		if ( iKeyIndex != -1 && iKeyIndex < USE_KEY_NUMBER )
		{
			unsigned char ucKey = m_DeviceKeyMap.right.at( KEY_BUTTON_INDEX( iKeyIndex ) );
			KeyStringMap::const_iterator cIter = m_KeyStringMap.find( ucKey );

			if ( cIter == m_KeyStringMap.end() )
				return L"";
			else
				return cIter->second;
		}
		
		return L"";
	}
	#pragma endregion 

	#pragma region InitKeySlotData
	void CX2KeyPad::InitKeySlotData()
	{
		// Key_Map 초기화
		m_KeyPadMap[GAMEACTION_QUICKSLOT1]	= KBI_1;
		m_KeyPadMap[GAMEACTION_QUICKSLOT2]	= KBI_2;
		m_KeyPadMap[GAMEACTION_QUICKSLOT3]	= KBI_3;
		m_KeyPadMap[GAMEACTION_QUICKSLOT4]	= KBI_4;
		m_KeyPadMap[GAMEACTION_QUICKSLOT5]	= KBI_5;
		m_KeyPadMap[GAMEACTION_QUICKSLOT6]	= KBI_6;

		m_KeyPadMap[GAMEACTION_ATTACK_FAST]		= KBI_Z;
		m_KeyPadMap[GAMEACTION_ATTACK_STRONG]	= KBI_X;
		m_KeyPadMap[GAMEACTION_AWAKE]			= KBI_LCTRL;
		m_KeyPadMap[GAMEACTION_SLOT_CHANGE]		= KBI_SPACE;
		m_KeyPadMap[GAMEACTION_ZOOMIN]			= KBI_EQUAL;
		m_KeyPadMap[GAMEACTION_ZOOMOUT]			= KBI_MINUS;
		m_KeyPadMap[GAMEACTION_NAME]			= KBI_N;

		m_KeyPadMap[GAMEACTION_SKILL1]	= KBI_A;
		m_KeyPadMap[GAMEACTION_SKILL2]	= KBI_S;
		m_KeyPadMap[GAMEACTION_SKILL3]	= KBI_D;
		m_KeyPadMap[GAMEACTION_SKILL4]	= KBI_C;

		m_KeyPadMap[GAMEACTION_SKILL_EX1]	= KBI_Q;
		m_KeyPadMap[GAMEACTION_SKILL_EX2]	= KBI_W;
		m_KeyPadMap[GAMEACTION_SKILL_EX3]	= KBI_E;
		m_KeyPadMap[GAMEACTION_SKILL_EX4]	= KBI_R;

		m_KeyPadMap[GAMEACTION_COMMUNITY]	= KBI_T;
		m_KeyPadMap[GAMEACTION_INFO]		= KBI_U;
		m_KeyPadMap[GAMEACTION_INVENTORY]	= KBI_I;
		m_KeyPadMap[GAMEACTION_OPTION]		= KBI_O;
		m_KeyPadMap[GAMEACTION_PET]			= KBI_P;
		m_KeyPadMap[GAMEACTION_PVE]			= KBI_LBRACKET;
		m_KeyPadMap[GAMEACTION_PVP]			= KBI_RBRACKET;
		m_KeyPadMap[GAMEACTION_SKILL_TREE]	= KBI_K;
		m_KeyPadMap[GAMEACTION_QUEST]		= KBI_L;
		m_KeyPadMap[GAMEACTION_MAP]			= KBI_M;
		m_KeyPadMap[GAMEACTION_GUILD]		= KBI_G;
		m_KeyPadMap[GAMEACTION_FRIEND]		= KBI_Y;

		m_KeyPadMap[GAMEACTION_UP]			= KBI_UP;
		m_KeyPadMap[GAMEACTION_DOWN]		= KBI_DOWN;
		m_KeyPadMap[GAMEACTION_LEFT]		= KBI_LEFT;
		m_KeyPadMap[GAMEACTION_RIGHT]		= KBI_RIGHT;

		m_KeyPadMap[GAMEACTION_SIT]			= KBI_V;

#ifdef RIDING_SYSTEM
		m_KeyPadMap[GAMEACTION_RIDING]		= KBI_F;
#endif //RIDING_SYSTEM
#ifdef SERV_ADD_WARP_BUTTON
		m_KeyPadMap[GAMEACTION_WARP]		= KBI_B;
#endif // SERV_ADD_WARP_BUTTON
#ifdef ADDED_RELATIONSHIP_SYSTEM
		m_KeyPadMap[GAMEACTION_RELATION]		= KBI_H;
#endif //ADDED_RELATIONSHIP_SYSTEM

		if ( NULL == g_pKTDXApp && NULL == g_pKTDXApp->GetDIManager() && 
			NULL == g_pKTDXApp->GetDIManager()->Getkeyboard() )
			return;

		for( int i = 0; i < GAMEACTION_END; ++i )
		{
			if ( GAME_ACTION_NONE != GAME_ACTION(i) )
			{
				int iKeyIndex = m_KeyPadMap.at( GAME_ACTION(i) );
				if ( iKeyIndex < USE_KEY_NUMBER 
#ifdef SERV_KEY_MAPPING_INT
					&& iKeyIndex != GAME_ACTION_NONE
#endif // SERV_KEY_MAPPING_INT					
					)
				{
					unsigned char ucKey = m_DeviceKeyMap.right.at( KEY_BUTTON_INDEX( iKeyIndex ) );
					g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAME_ACTION(i), ucKey );
				}
				else
				{
					g_pKTDXApp->GetDIManager()->Getkeyboard()->SetAction( GAME_ACTION(i), 0 );
				}
			}
		}

		Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ();
	}
	#pragma endregion 함수

#pragma endregion 클래스

#pragma region CX2UseKeySlotData

	#pragma region CX2UseKeySlotData
	/** @function 	: CX2UseKeySlotData
		@brief 		: 생성자
	*/
	CX2UseKeySlotData::CX2UseKeySlotData()
	{
		Clear();
	}
		
	CX2UseKeySlotData::CX2UseKeySlotData( IN int index_ )
	{
		Clear( index_ );
	}
	#pragma endregion 생성자

	#pragma region ~CX2UseKeySlotData
	/** @function 	: ~CX2UseKeySlotData
		@brief 		: 소멸자
	*/
	/*virtual*/ CX2UseKeySlotData::~CX2UseKeySlotData()
	{

	}
	#pragma endregion 소멸자

	#pragma region Clear
	/** @function 	: Clear
		@brief 		: 클래스의 속성을 초기화
	*/
	void CX2UseKeySlotData::Clear( IN int index_/* = -1*/ )
	{
		m_eSlotDataType	= SDT_USE_KEY;
		m_bDataUpdated = true;
		m_eKeyActionID = GAME_ACTION_NONE;
		m_iIndex = index_;
	}
	#pragma endregion 함수

	#pragma region UpdateUI
	/** @function 	: UpdateUI
		@brief 		: SlotData를 갱신한다.
	*/
	/*virtual*/ void CX2UseKeySlotData::UpdateUI( IN CKTDGUISlot* pSlot_ )
	{
		if ( m_eKeyActionID == GAME_ACTION_NONE )
		{
			pSlot_->SetItemTex( L"DLG_Common_New_Texture03.TGA", L"invisible" );
			pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SLOT_EMPTY" );
			pSlot_->SetDragable( false );
			if ( NULL != pSlot_->GetString() )
#ifdef STRING_KEYMARK
			{
				wstring wstrKeyName = g_pMain->GetKeyPad()->GetKeyString( m_iIndex );
				if( wstrKeyName == L"Up" || wstrKeyName == L"Down" || wstrKeyName == L"Left" || wstrKeyName == L"Right" )
					wstrKeyName = L"";
				pSlot_->GetString()->msg = wstrKeyName;
				pSlot_->GetString()->color = D3DXCOLOR( 0.81f, 0.81f, 0.81f, 1.0f );
			}			
#else STRING_KEYMARK
				pSlot_->GetString()->msg = L"";
#endif STRING_KEYMARK

#ifdef KEY_MAPPING_TOOLTIP
			pSlot_->SetGuideDesc( L"" );
#endif KEY_MAPPING_TOOLTIP
		}
		else
		{
			switch ( m_eKeyActionID )
			{
			case GAMEACTION_QUICKSLOT1:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK1" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK1" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24426 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT2:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK2" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK2" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24427 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT3:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK3" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK3" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24428 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT4:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK4" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK4" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24429 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT5:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK5" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK5" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24430 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT6:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK6" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK6" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24431 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ATTACK_FAST:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_Z" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_Z" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24399 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ATTACK_STRONG:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_X" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_X" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24400 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_AWAKE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"HYPER" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"HYPER" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24425 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SLOT_CHANGE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SLOT_CHANGE" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SLOT_CHANGE" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24409 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ZOOMIN:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_IN" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_IN" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24412 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ZOOMOUT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_OUT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_OUT" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24413 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_NAME:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SHOW_NAME" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SHOW_NAME" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24423 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL1:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A1" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A1" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24401 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL2:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A2" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A2" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24402 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL3:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A3" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A3" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24403 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL4:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A4" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A4" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24404 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX1:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B1" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B1" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24405 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX2:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B2" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B2" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24406 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX3:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B3" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B3" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24407 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX4:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B4" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B4" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24408 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_COMMUNITY:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"COMMUNITY" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"COMMUNITY" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24416 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_INFO:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"CHARACTER" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"CHARACTER" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24417 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_INVENTORY:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"INVEN" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"INVEN" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24418 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_OPTION:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"OPTION" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"OPTION" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24419 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_PET:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"PET" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"PET" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24420 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_PVE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"DUNGEON" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"DUNGEON" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24410 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_PVP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"PVP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"PVP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24411 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_TREE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24422 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUEST:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUEST" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUEST" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24414 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_MAP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"MAP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"MAP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24415 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_GUILD:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"GUILD" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"GUILD" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24421 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_FRIEND:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"FRIEND" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"FRIEND" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24424 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_UP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_UP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_UP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24432 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_DOWN:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_DOWN" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_DOWN" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24433 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_LEFT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_LEFT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_LEFT" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24434 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_RIGHT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_RIGHT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_RIGHT" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24435 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SIT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SIT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SIT" );
					pSlot_->SetDragable( true );
				
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24436 );
#endif KEY_MAPPING_TOOLTIP
				} break;
				
#ifdef SERV_ADD_WARP_BUTTON
			case GAMEACTION_WARP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"WARP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"WARP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24619 );
#endif KEY_MAPPING_TOOLTIP
				} break;
#endif SERV_ADD_WARP_BUTTON

#ifdef RIDING_SYSTEM
			case GAMEACTION_RIDING:
				{
#ifdef SHOW_NO_UI_BEFORE_IN_GAME
#else //SHOW_NO_UI_BEFORE_IN_GAME
					pSlot_->SetItemTex( L"DLG_UI_Button_RidingPet_01.TGA", L"RIDING" );
					pSlot_->SetMouseOverTex( L"DLG_UI_Button_RidingPet_01.TGA", L"RIDING" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_28120 );
#endif KEY_MAPPING_TOOLTIP
#endif //SHOW_NO_UI_BEFORE_IN_GAME
				} break;
#endif //RIDING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
			case GAMEACTION_RELATION:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"Marriage" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"Marriage" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24462 );
#endif KEY_MAPPING_TOOLTIP
				} break;
#endif //ADDED_RELATIONSHIP_SYSTEM

			default:
				break;
			}

			if ( NULL != pSlot_->GetString() )
			{
				pSlot_->GetString()->msg = g_pMain->GetKeyPad()->GetKeyString( m_eKeyActionID ).c_str();
#ifdef STRING_KEYMARK
				pSlot_->GetString()->color = D3DXCOLOR( 0.0f, 0.5f, 1.0f, 1.0f );
#endif STRING_KEYMARK
			}
		}
	}
	#pragma endregion 함수

#pragma endregion 클래스

#pragma region CX2DisUseKeySlotData

	#pragma region CX2DisUseKeySlotData
	/** @function 	: CX2DisUseKeySlotData
		@brief 		: 생성자
	*/
	CX2DisUseKeySlotData::CX2DisUseKeySlotData()
	{
		Clear();
	}
		
	CX2DisUseKeySlotData::CX2DisUseKeySlotData( IN int index_ )
	{
		Clear( index_ );
	}
	#pragma endregion 생성자

	#pragma region ~CX2DisUseKeySlotData
	/** @function 	: ~CX2DisUseKeySlotData
		@brief 		: 소멸자
	*/
	/*virtual*/ CX2DisUseKeySlotData::~CX2DisUseKeySlotData()
	{

	}
	#pragma endregion 소멸자

	#pragma region Clear
	/** @function 	: Clear
		@brief 		: 클래스의 속성을 초기화
	*/
	void CX2DisUseKeySlotData::Clear( IN int index_/* = -1*/ )
	{
		m_eSlotDataType	= SDT_DISUSE_KEY;
		m_bDataUpdated = true;
		m_eKeyActionID = GAME_ACTION_NONE;
		m_iIndex = index_;
	}
	#pragma endregion 함수

	#pragma region UpdateUI
	/** @function 	: UpdateUI
		@brief 		: SlotData를 갱신한다.
	*/
	/*virtual*/ void CX2DisUseKeySlotData::UpdateUI( IN CKTDGUISlot* pSlot_ )
	{
		if ( m_eKeyActionID == GAME_ACTION_NONE )
		{
			pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SLOT_EMPTY" );
			pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SLOT_EMPTY" );
			pSlot_->SetDragable( false );
#ifdef KEY_MAPPING_TOOLTIP
			pSlot_->SetGuideDesc( L"" );
#endif KEY_MAPPING_TOOLTIP
		}
		else
		{
			switch ( m_eKeyActionID )
			{
			case GAMEACTION_QUICKSLOT1:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK1" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK1" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24426 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT2:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK2" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK2" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24427 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT3:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK3" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK3" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24428 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT4:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK4" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK4" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24429 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT5:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK5" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK5" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24430 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUICKSLOT6:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK6" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUICK6" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24431 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ATTACK_FAST:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_Z" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_Z" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24399 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ATTACK_STRONG:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_X" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_X" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24400 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_AWAKE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"HYPER" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"HYPER" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24425 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SLOT_CHANGE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SLOT_CHANGE" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SLOT_CHANGE" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24409 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ZOOMIN:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_IN" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_IN" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24412 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_ZOOMOUT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_OUT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"ZOOM_OUT" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24413 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_NAME:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SHOW_NAME" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SHOW_NAME" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24423 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL1:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A1" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A1" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24401 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL2:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A2" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A2" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24402 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL3:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A3" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A3" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24403 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL4:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A4" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_A4" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24404 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX1:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B1" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B1" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24405 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX2:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B2" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B2" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24406 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX3:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B3" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B3" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24407 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_EX4:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B4" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL_B4" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24408 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_COMMUNITY:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"COMMUNITY" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"COMMUNITY" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24416 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_INFO:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"CHARACTER" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"CHARACTER" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24417 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_INVENTORY:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"INVEN" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"INVEN" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24418 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_OPTION:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"OPTION" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"OPTION" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24419 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_PET:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"PET" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"PET" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24420 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_PVE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"DUNGEON" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"DUNGEON" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24410 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_PVP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"PVP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"PVP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24411 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SKILL_TREE:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SKILL" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24422 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_QUEST:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"QUEST" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"QUEST" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24414 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_MAP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"MAP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"MAP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24415 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_GUILD:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"GUILD" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"GUILD" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24421 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_FRIEND:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"FRIEND" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"FRIEND" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24424 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_UP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_UP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_UP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24432 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_DOWN:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_DOWN" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_DOWN" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24433 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_LEFT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_LEFT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_LEFT" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24434 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_RIGHT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_RIGHT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"KEY_RIGHT" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24435 );
#endif KEY_MAPPING_TOOLTIP
				} break;

			case GAMEACTION_SIT:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"SIT" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"SIT" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24436 );
#endif KEY_MAPPING_TOOLTIP
				} break;
#ifdef SERV_ADD_WARP_BUTTON
			case GAMEACTION_WARP:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"WARP" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"WARP" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24619 );
#endif KEY_MAPPING_TOOLTIP
				} break;
#endif SERV_ADD_WARP_BUTTON

#ifdef RIDING_SYSTEM
			case GAMEACTION_RIDING:
				{
#ifdef SHOW_NO_UI_BEFORE_IN_GAME
#else //SHOW_NO_UI_BEFORE_IN_GAME
					pSlot_->SetItemTex( L"DLG_UI_Button_RidingPet_01.TGA", L"RIDING" );
					pSlot_->SetMouseOverTex( L"DLG_UI_Button_RidingPet_01.TGA", L"RIDING" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_28120 );
#endif KEY_MAPPING_TOOLTIP
#endif //SHOW_NO_UI_BEFORE_IN_GAME
				} break;
#endif //RIDING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
			case GAMEACTION_RELATION:
				{
					pSlot_->SetItemTex( L"DLG_UI_KEYSETTING.TGA", L"Marriage" );
					pSlot_->SetMouseOverTex( L"DLG_UI_KEYSETTING.TGA", L"Marriage" );
					pSlot_->SetDragable( true );
#ifdef KEY_MAPPING_TOOLTIP
					pSlot_->SetGuideDesc_LUA( STR_ID_24462 );
#endif KEY_MAPPING_TOOLTIP
				} break;
#endif //ADDED_RELATIONSHIP_SYSTEM

			default:
				break;
			}
		}
	}

	/** @function : UpdateEtcString()
		@brief : 키설정에 따른 스트링 변경이 실시간으로 이루어지지 않는 항목에 대한 업데이트 
	*/
	void CX2KeyPad::UpdateEtcString()
	{
		if( NULL == g_pData )
			return;

		CX2UIManager* pUIManager = g_pData->GetUIManager();
		if( NULL != pUIManager )
		{
			if( NULL != pUIManager->GetUISkillTree())
			{//스킬슬롯 스트링 갱신
				pUIManager->GetUISkillTree()->UpdateSkillSlotString();
			}

			if( NULL != pUIManager->GetUIQuickSlot() )
			{//퀵슬롯 스트링 갱신
				pUIManager->GetUIQuickSlot()->ResetQuickSlotUI();
			}
#ifdef RIDING_SYSTEM
			if( NULL != CX2RidingPetManager::GetInstance() && NULL != CX2RidingPetManager::GetInstance()->GetSkillSlot() )
			{
				CX2RidingPetManager::GetInstance()->GetSkillSlot()->UpdateSkillSlotString();
				CX2RidingPetManager::GetInstance()->GetSkillSlot()->UpdateRideOffSlotGuide();
			}

			if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIPetList() )
			{
				bool IsSetKey = (0 == StrCmp(g_pMain->GetKeyPad()->GetKeyString( GA_RIDING ).c_str(), L"" ))? false : true;
				g_pData->GetUIManager()->GetUIPetList()->UpdateRidingButtonGuideDesc( IsSetKey );
			}
#endif //RIDING_SYSTEM
		}

#ifdef CHECK_INHERIT_STATE_MENU
		if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
		{
			CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
			if( NULL != pStateMenu)
			{//하단메뉴 스트링 갱신
				pStateMenu->UpdateMenuString();
			}
		}
	}
	#pragma endregion 함수

#pragma endregion 클래스


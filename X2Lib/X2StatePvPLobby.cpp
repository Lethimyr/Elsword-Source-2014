#include "StdAfx.h"


#include ".\x2statepvplobby.h"


CX2StatePvPLobby::CX2StatePvPLobby(void)
: m_KillCountForCreateRoomIndex( 0 )
, m_PlayTimeForCreateRoomIndex( 0 )
{ 
	DialogLog( "\nCX2StatePvPLobby::CX2StatePvPLobby start\n" );

	m_pDLGPvPLobby = NULL;
	m_pDLGPVPLobbyBack = NULL;
	m_pDLGJoinPasswordRoom = NULL;

	m_pDLGLoadingState = NULL;


	m_pDLGPvPLobby		= NULL;
	m_pDLGCreateRoom	= NULL;
	m_pDLGMsgBox		= NULL;

#ifdef DUNGEON_ITEM
	m_pDLGCreateRoom = new CKTDGUIDialog( this, L"DLG_UI_Pvp_Make_ItemMode.lua" );
#else
	m_pDLGCreateRoom = new CKTDGUIDialog( this, L"DLG_UI_Pvp_Make.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCreateRoom );

	m_pUINPC = g_pData->GetUIUnitManager()->GetUIUnit( CX2UnitManager::NUI_CAMILLA );
	if( NULL != m_pUINPC )
	{				
		m_pUINPC->SetAlphaObject(true);										
		m_pUINPC->StateChange( m_pUINPC->GetWaitStateID() );
		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUINPC );		
		m_pUINPC->SetShowObject( true );
		//카밀라 다리 짤려서 조금 밑으로 이동..
		D3DXVECTOR3 vPos = m_pUINPC->GetPos();
		vPos.y-=50;
		m_pUINPC->SetPos(vPos);

	}

	m_pDlgNpcMessage = new CX2TFieldNpcShop();
	//m_pDlgNpcMessage->SetLayer(XDL_MESSENGER);
	m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_CLOSE);
	m_pDlgNpcMessage->SetHouseName( GET_STRING( STR_ID_679 ) );
	m_pDlgNpcMessage->SetNpcMessage( GET_STRING( STR_ID_680 ) );
	m_pDlgNpcMessage->SetShow(true);
	m_pDlgNpcMessage->SetStage(this);

	// 변수 초기화
	m_iMaxNumOfRoom = 12;

#ifdef SERV_PVP_NEW_SYSTEM
	m_MaxChannelID		= 2;
#else
	m_MaxChannelID		= 6;
#endif

	m_NowPage			= 1;
	m_EndPage			= 1;

	m_bEnableRefreshRoomList = true;
	m_bEnableResetDLGRoomList = true;

	m_bCreateRoomObserver = false;


	m_iMaxNumOfRoom		= 12;			// const, 화면에 보이는 최대 room list 수
	m_iRoomListGameType	 = -1;		// room list의 game type

	m_TempChannelID			= 0;

	m_fTime = 2.5f;
	m_fRefreshTime = 5.0f;

	m_bIsPublicRoom = true;
	m_CreatePVPRoomType = CX2PVPRoom::PGT_TEAM_DEATH;

	m_JoinRoomUid			= -1;
	m_QuickJoinPVPGameType	= CX2PVPRoom::PGT_TEAM_DEATH;	

#ifdef DUNGEON_ITEM
	m_bItemMode = true;
	m_QuickJoinPVPItemMode = false;
#endif

	// 초기화 코드는 여기 쓰고
	for( int i = 0; i < m_iMaxNumOfRoom; i++ )
	{
		SimpleRoom* pSimpleRoom = new SimpleRoom();
		pSimpleRoom->pButton = NULL;
		pSimpleRoom->pStatic = NULL;

		m_SimpleRoomList.push_back( pSimpleRoom );
	}


	if( true == g_pMain->GetIsExitingTutorial() )
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );
		// 랜덤 로딩 화면 
		wstringstream wstmStaticName;
		wstmStaticName << L"Static_Loading_BG";
		wstmStaticName << g_pMain->GetTutorialRandomLoadingBG();
		m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );

		switch( g_pMain->GetStateIDBeforeTutorial() )
		{
		case CX2Main::XS_PVP_LOBBY:
		case CX2Main::XS_PVP_ROOM:
		default:
			{
				g_pMain->SetIsExitingTutorial( false );
			} break;
		}
	}

	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );
		// 랜덤 로딩 화면 
		wstringstream wstmStaticName;
		wstmStaticName << L"Static_Loading_BG";
		wstmStaticName << g_pMain->GetTutorialRandomLoadingBG();
		m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );

		Handler_EGS_STATE_CHANGE_FIELD_REQ();
		return;
	}

	if( NULL != m_pDLGCommonBG )
	{
		m_pDLGCommonBG->OpenScriptFile( L"DLG_PVP_Lobby_CharBG.lua" );
	}

	m_pDLGPVPLobbyBack = g_pMain->GetMemoryHolder()->GetPVPLobbyBack( this );
	m_pDLGPvPLobby = g_pMain->GetMemoryHolder()->GetPVPLobbyFront( this );

	OpenDialog();

	m_pDLGCreateRoom->SetShowEnable(false, false);

	TextureReady( L"DLG_Common3.tga" );
	TextureReady( L"DLG_CreateRoom0.tga" );
	Handler_EGS_ROOM_LIST_REQ( m_NowPage );

	g_pKTDXApp->SkipFrame();

	DialogLog( "\nCX2StatePvPLobby::CX2StatePvPLobby end\n" );

	SetShowMenu(false);
	g_pData->GetUIManager()->SetShowSkillSlot(false);
	g_pData->GetUIManager()->SetShowQuickSlot(false);
	
}

CX2StatePvPLobby::~CX2StatePvPLobby(void)
{
	DialogLog( "\nCX2StatePvPLobby::~CX2StatePvPLobby start\n" );

//	SAFE_DELETE_DIALOG( m_pDLGPVPLobbyBack );
//	SAFE_DELETE_DIALOG( m_pDLGPVPLobbyFront );
	if ( m_pDLGPVPLobbyBack != NULL )
	{
		m_pDLGPVPLobbyBack->SetShow( false );
		m_pDLGPVPLobbyBack->SetEnable( false );
	}


	if ( m_pDLGPvPLobby != NULL )
	{
		m_pDLGPvPLobby->SetShow( false );
		m_pDLGPvPLobby->SetEnable( false );
	}
	
	SAFE_DELETE_DIALOG( m_pDLGCreateRoom );
	SAFE_DELETE_DIALOG( m_pDLGJoinPasswordRoom );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState ); // note: 얘는 다이얼로그 매니저에서 관리되는 애가 아니라서~ 요렇게
#endif // DIALOG_HANDLE_TEST // 2009-8-19

	if( NULL != m_pDLGCommonBG )
	{
		m_pDLGCommonBG->DeleteControl( L"StaticPVP_Lobby_CharBG" );
	}

	for( int i = 0; i < (int)m_RoomList.size(); i++ )
	{
		CX2PVPRoom::PVPRoomSimpleInfo* pPVPRoomListData = m_RoomList[i];
		SAFE_DELETE( pPVPRoomListData );
	}
	m_RoomList.clear();

	for ( int i = 0; i < (int)m_SimpleRoomList.size(); i++ )
	{
		SimpleRoom* pSimpleRoom = m_SimpleRoomList[i];
		SAFE_DELETE( pSimpleRoom );
	}
	m_SimpleRoomList.clear();

	SAFE_DELETE( m_pDlgNpcMessage );
	if( m_pUINPC != NULL )
	{
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pUINPC );
		m_pUINPC->SetShowObject( false );
	}

	DialogLog( "\nCX2StatePvPLobby::~CX2StatePvPLobby end\n" );
}

HRESULT CX2StatePvPLobby::OnFrameMove(double fTime, float fElapsedTime )
{
	
	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );

	if( true == g_pMain->GetIsPlayingTutorial() )
		return S_OK;
	
	m_fTime += fElapsedTime;

	if ( m_fTime >= m_fRefreshTime )
	{
		m_fTime = 0.0f;
		Handler_EGS_ROOM_LIST_REQ( m_NowPage );
	}

	if( g_pData->GetMyUser() != NULL  &&
		g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR  )
	{
		//if ( m_pCharacterRoom != NULL && m_pCharacterRoom->GetOpen() == false )
#ifndef NEW_SKILL_TREE
		if( false == g_pInstanceData->GetSkillTreeUI()->GetShow() )
#endif
		{
			if ( m_bStateChangingVillageByForce == false && CheckUnitLevelByChannelList() == false )
			{
				m_bStateChangingVillageByForce = true;

				Handler_EGS_STATE_CHANGE_FIELD_REQ();

				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_681 ), this );
			}
		}
	}

	if(m_pDlgNpcMessage != NULL)
		m_pDlgNpcMessage->OnFrameMove( fTime, fElapsedTime );
	if(m_pUINPC != NULL)
		m_pUINPC->OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}

HRESULT CX2StatePvPLobby::OnFrameRender()
{
	if( true == g_pMain->GetIsPlayingTutorial() ||
		true == g_pMain->GetIsExitingTutorial() )
	{
		if( m_pDLGLoadingState != NULL )
			m_pDLGLoadingState->OnFrameRender();
		return S_OK;
	}

	CX2StateMenu::OnFrameRender();

	return S_OK;
}

bool CX2StatePvPLobby::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if(m_pDlgNpcMessage->UICustomEventProc(hWnd, uMsg, wParam, lParam) == 1)
	{
		Handler_EGS_STATE_CHANGE_FIELD_REQ();
		return true;
	}

	switch(wParam)
	{
	case UPLCM_EXIT:
		Handler_EGS_STATE_CHANGE_FIELD_REQ();
		return true;
	case UPLCM_SELECT_CHANNEL:
		{
			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;

			int channelID = GetChannelID( (KPVPChannelInfo::PVP_CHANNEL_CLASS)pRadioButton->GetDummyInt(0) );
			if ( channelID != -1 )
				return Handler_EGS_CHANGE_PVP_ROOM_LIST_REQ( channelID );
		}
		break;
	case UPLCM_GAMETYPE_ALL:
		m_iRoomListGameType = -1;
		m_NowPage = 1;
		return Handler_EGS_ROOM_LIST_REQ( m_NowPage );
		break;
	case UPLCM_GAMETYPE_TEAM:
		m_iRoomListGameType = (int)CX2PVPRoom::PGT_TEAM;
		m_NowPage = 1;
		return Handler_EGS_ROOM_LIST_REQ( m_NowPage );
		break;
	case UPLCM_GAMETYPE_TEAM_DEATH:
		m_iRoomListGameType = (int)CX2PVPRoom::PGT_TEAM_DEATH;
		m_NowPage = 1;
		return Handler_EGS_ROOM_LIST_REQ( m_NowPage );
		break;
	case UPLCM_GAMETYPE_SURVIVAL:
		m_iRoomListGameType = (int)CX2PVPRoom::PGT_SURVIVAL;
		m_NowPage = 1;
		return Handler_EGS_ROOM_LIST_REQ( m_NowPage );
		break;
	case UPLCM_QUICKSTART_TEAM:
		{

#ifdef TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
			if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
				return true;
#endif TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE

			Handler_EGS_QUICK_JOIN_REQ( CX2PVPRoom::PGT_TEAM );
		} break;
	case UPLCM_QUICKSTART_TEAM_DEATH:
		Handler_EGS_QUICK_JOIN_REQ( CX2PVPRoom::PGT_TEAM_DEATH );
		break;
	case UPLCM_QUICKSTART_SURVIVAL:
		{
#ifdef TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
			if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
				return true;
#endif TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
		
			Handler_EGS_QUICK_JOIN_REQ( CX2PVPRoom::PGT_SURVIVAL );
		} break;
	case UPLCM_CREATE_ROOM:
		{
			OpenCreateRoomDLG();
			return true;
		}
		break;
	case UPLCM_JOIN_ROOM:
		{
			CKTDGUIButton* pJoinButton	= (CKTDGUIButton*)lParam;
			CKTDGUIDialogType pDialog		= pJoinButton->GetDialog();

			int simpleRoomIndex = -1;

			simpleRoomIndex				= pJoinButton->GetDummyInt(0);
			SimpleRoom*	pSimpleRoom		= m_SimpleRoomList[simpleRoomIndex];
			m_JoinRoomUid				= pSimpleRoom->roomUid;

			if( simpleRoomIndex != -1 )
			{
				for ( int i = 0; i < (int)m_RoomList.size(); i++ )
				{
					CX2PVPRoom::PVPRoomSimpleInfo* pTempData = m_RoomList[i];

					if ( pTempData->m_RoomUID == m_JoinRoomUid )
					{
						if ( pTempData->m_bPublic == true )
						{
							return Handler_EGS_JOIN_ROOM_REQ( m_JoinRoomUid );
						}
						else
						{
							OpenDLGJoinPasswordRoom( m_JoinRoomUid );
							return true;
						}
					}
				}					
			}
			else
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_682 ), this );

		}
		break;
	case UPLCM_ROOMLIST_PREV_PAGE:
		{
			if( m_NowPage <= 1 )
				return true;

			m_NowPage--;
			return Handler_EGS_ROOM_LIST_REQ( m_NowPage );
		}
		break;
	case UPLCM_ROOMLIST_NEXT_PAGE:
		{
			if( m_NowPage >= m_EndPage )
				return true;

			m_NowPage++;
			return Handler_EGS_ROOM_LIST_REQ( m_NowPage );
		}
		break;


		// sub-dialog
		// ------------ 절 취 선 -------------
	case UPLCM_CREATEROOMDLG_EXIT:
		{
			m_pDLGCreateRoom->SetShowEnable(false, false);
			return true;
		}
		break;
	case UPLCM_CREATEROOMDLG_GAMETYPE_TEAM:
		{
			SelectGametypeOnCreateRoomDLG(CX2PVPRoom::PGT_TEAM);
			return true;
		}
		break;
	case UPLCM_CREATEROOMDLG_GAMETYPE_TEAM_DEATH:
		{
			SelectGametypeOnCreateRoomDLG(CX2PVPRoom::PGT_TEAM_DEATH);
			return true;
		}
		break;
	case UPLCM_CREATEROOMDLG_GAMETYPE_SURVIVAL:
		{
			SelectGametypeOnCreateRoomDLG(CX2PVPRoom::PGT_SURVIVAL);
			return true;
		}
		break;

	case UPLCM_CREATEROOMDLG_KILL_PLUS:
		{	

			switch(m_CreatePVPRoomType)
			{
			case CX2PVPRoom::PGT_TEAM_DEATH:
				{
					if( m_KillCountForCreateRoomIndex >= ARRAY_SIZE( TEAM_DEATH_PVP_KILL_COUNT )-1 )
					{
						m_KillCountForCreateRoomIndex = ARRAY_SIZE( TEAM_DEATH_PVP_KILL_COUNT )-1;
					}
					else
					{
						++m_KillCountForCreateRoomIndex;
					}
				} break;

			case CX2PVPRoom::PGT_SURVIVAL:
				{
					if( m_KillCountForCreateRoomIndex >= ARRAY_SIZE( SURVIVAL_PVP_KILL_COUNT )-1 )
					{
						m_KillCountForCreateRoomIndex = ARRAY_SIZE( SURVIVAL_PVP_KILL_COUNT )-1;
					}
					else
					{
						++m_KillCountForCreateRoomIndex;
					}
				} break;

			case CX2PVPRoom::PGT_TEAM:
			default:
				{
					ASSERT( !"NO" );
					return false;
				}break;				
			}

			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"%d", GetKillCountForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE) m_CreatePVPRoomType ) );
			//wsprintf( buff, L"%d", GetKillCountForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE) m_CreatePVPRoomType ) );

			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCreateRoom->GetControl( L"StaticKill" );
			pStatic->GetString(0)->msg = buff;
			return true;

		}
		break;


	case UPLCM_CREATEROOMDLG_KILL_MINUS:
		{
			switch( m_CreatePVPRoomType )
			{
			case CX2PVPRoom::PGT_TEAM_DEATH:
			case CX2PVPRoom::PGT_SURVIVAL:
				{
					if( m_KillCountForCreateRoomIndex <= 0 )
					{
						m_KillCountForCreateRoomIndex = 0;
					}
					else
					{
						--m_KillCountForCreateRoomIndex;
					}
				} break;

			case CX2PVPRoom::PGT_TEAM:
			default:
				{
					ASSERT( !"NO" );
				} break;

			}

			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"%d", GetKillCountForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE) m_CreatePVPRoomType ) );
			//wsprintf( buff, L"%d", GetKillCountForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE) m_CreatePVPRoomType ) );

			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCreateRoom->GetControl( L"StaticKill" );
			pStatic->GetString(0)->msg = buff;
			return true;
		}
		break;

	case UPLCM_CREATEROOMDLG_TIME_PLUS:
		{
			switch(m_CreatePVPRoomType)
			{
			case CX2PVPRoom::PGT_TEAM:
				{
					if( m_PlayTimeForCreateRoomIndex >= ARRAY_SIZE( TEAM_PVP_PLAY_TIME )-1 )
					{
						m_PlayTimeForCreateRoomIndex = ARRAY_SIZE( TEAM_PVP_PLAY_TIME )-1;
					}
					else
					{
						++m_PlayTimeForCreateRoomIndex;
					}
				} break;


			case CX2PVPRoom::PGT_TEAM_DEATH:
				{
					if( m_PlayTimeForCreateRoomIndex >= ARRAY_SIZE( TEAM_DEATH_PVP_PLAY_TIME )-1 )
					{
						m_PlayTimeForCreateRoomIndex = ARRAY_SIZE( TEAM_DEATH_PVP_PLAY_TIME )-1;
					}
					else
					{
						++m_PlayTimeForCreateRoomIndex;
					}
				} break;

			case CX2PVPRoom::PGT_SURVIVAL:
				{
					if( m_PlayTimeForCreateRoomIndex >= ARRAY_SIZE( SURVIVAL_PVP_PLAY_TIME )-1 )
					{
						m_PlayTimeForCreateRoomIndex = ARRAY_SIZE( SURVIVAL_PVP_PLAY_TIME )-1;
					}
					else
					{
						++m_PlayTimeForCreateRoomIndex;
					}
				} break;

			default:
				{
					ASSERT( !"NO" );
				} break;				
			}


			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"%d", (int) GetPlayTimeForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );
			/*wsprintf( buff, L"%d", (int) GetPlayTimeForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );*/

			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCreateRoom->GetControl( L"StaticTime" );
			pStatic->GetString(0)->msg = buff;
			return true;
		}
		break;


	case UPLCM_CREATEROOMDLG_TIME_MINUS:
		{

			switch(m_CreatePVPRoomType)
			{
			case CX2PVPRoom::PGT_TEAM:	
			case CX2PVPRoom::PGT_TEAM_DEATH:	
			case CX2PVPRoom::PGT_SURVIVAL:
				{
					if( m_PlayTimeForCreateRoomIndex <= 0 )
					{
						m_PlayTimeForCreateRoomIndex = 0;
					}
					else
					{
						--m_PlayTimeForCreateRoomIndex;
					}
				} break;


			default:
				{
					ASSERT( !"NO" );
				} break;				
			}

			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"%d", (int) GetPlayTimeForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );
			//wsprintf( buff, L"%d", (int) GetPlayTimeForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );

			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGCreateRoom->GetControl( L"StaticTime" );
			pStatic->GetString(0)->msg = buff;
			return true;
		}
		break;

	case UPLCM_CREATEROOMDLG_PRIVATE_CHECKED:
		{
			m_bIsPublicRoom = false;
			CKTDGUIIMEEditBox* pIMEEditBoxPassword = (CKTDGUIIMEEditBox*)m_pDLGCreateRoom->GetControl( L"IME_EditPassword" );
			pIMEEditBoxPassword->SetShowEnable( true, true );
			pIMEEditBoxPassword->RequestFocus();

			return true;
		}
		break;
	case UPLCM_CREATEROOMDLG_PRIVATE_UNCHECKED:
		{
			m_bIsPublicRoom = true;
			CKTDGUIIMEEditBox* pIMEEditBoxPassword = (CKTDGUIIMEEditBox*)m_pDLGCreateRoom->GetControl( L"IME_EditPassword" );
			pIMEEditBoxPassword->SetShowEnable( false, false );
			return true;
		}
		break;

	//{{ kimhc // 2009-05-04, 17:10
	// SpecialUser 이상의 등급일 경우에 생기는 Observer 모드 CheckBox Flag
	case UPLCM_CREATEROOMDLG_OBSERVER_CHECKED:
		{
			m_bCreateRoomObserver = true;
			break;
		}

	case UPLCM_CREATEROOMDLG_OBSERVER_UNCHECKED:
		{
			m_bCreateRoomObserver = false;
			break;
		}
	//}} kimhc // 2009-05-04, 17:10

	case UPLCM_CREATEROOMDLG_CREATE_CONFIRM:		
		return Handler_EGS_CREATE_ROOM_REQ();
	case UPLCM_PASSWORD_CANCLE:
		{
			//SAFE_DELETE_DIALOG(m_pDLGJoinPasswordRoom);
			m_pDLGJoinPasswordRoom->SetShow(false);

			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
			{
				g_pMain->GetStateAutoChanger().EndStateChange();
			}
		}
		break;
	case UPLCM_PASSWORD_OK:
		{
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGJoinPasswordRoom->GetControl( L"IME_EditJoinPassword" );
			wstring passWord = pIMEEditBox->GetText();
#ifdef SERV_NEW_PVPROOM_PROCESS
			g_pMain->GetStateAutoChanger().GetTargetDetail().m_bInvitation = false;
#endif SERV_NEW_PVPROOM_PROCESS
			Handler_EGS_JOIN_ROOM_REQ( m_JoinRoomUid, false, passWord );

			m_pDLGJoinPasswordRoom->SetShow(false);
			//SAFE_DELETE_DIALOG(m_pDLGJoinPasswordRoom);
		}
		break;
#ifdef DUNGEON_ITEM
	case UPLCM_CREATEROOMDLG_ITEMMODE_PLUS:
	case UPLCM_CREATEROOMDLG_ITEMMODE_MINUS:
		{
			if( m_CreatePVPRoomType == CX2PVPRoom::PGT_SURVIVAL )
			{
				m_bItemMode = true;
			}
			else
			{
				m_bItemMode = !m_bItemMode;
			}

			CKTDGUIStatic* pStaticItemMode = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"StaticItem" );
			if( m_bItemMode == false )
			{				
				pStaticItemMode->GetString(0)->msg = GET_STRING( STR_ID_4818 ); // 파워 전
			}
			else
			{				
				pStaticItemMode->GetString(0)->msg = GET_STRING( STR_ID_4817 ); // 아이템 전
			}

			return true;
		}
		break;
#ifdef DUNGEON_ITEM
	case UPLCM_QUICKSTART_TEAM_ITEMMODE:
		{
#ifdef TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
			if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
				return true;
#endif TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE

			Handler_EGS_QUICK_JOIN_REQ( CX2PVPRoom::PGT_TEAM, true );
		} break;
	case UPLCM_QUICKSTART_TEAM_DEATH_ITEMMODE:
		{
#ifdef TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
			if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
				return true;
#endif TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE

			Handler_EGS_QUICK_JOIN_REQ( CX2PVPRoom::PGT_TEAM_DEATH, true );
		} break;
#endif
#endif
	default:
		break;
	}

	return true;
}

bool CX2StatePvPLobby::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch(wParam)
	{	
		case EGS_CREATE_ROOM_ACK:
			return Handler_EGS_CREATE_ROOM_ACK( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_ROOM_LIST_ACK:
			return Handler_EGS_ROOM_LIST_ACK( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_JOIN_ROOM_ACK:
			return Handler_EGS_JOIN_ROOM_ACK( hWnd, uMsg, wParam, lParam );
			break;		

		case EGS_QUICK_JOIN_ACK:
			return Handler_EGS_QUICK_JOIN_ACK( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_CHANGE_PVP_ROOM_LIST_ACK:
			return Handler_EGS_CHANGE_PVP_ROOM_LIST_ACK( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_STATE_CHANGE_FIELD_ACK:
			return Handler_EGS_STATE_CHANGE_FIELD_ACK( hWnd, uMsg, wParam, lParam );
	}

	return false;
}


bool CX2StatePvPLobby::Handler_EGS_ROOM_LIST_REQ( int pageNum )
{
	if ( m_bEnableRefreshRoomList == false )
		return true;

	KEGS_ROOM_LIST_REQ	kPacket;
	kPacket.m_nViewPage			= pageNum;
	kPacket.m_cPVPGameType		= (char) m_iRoomListGameType ;

	g_pData->GetServerProtocol()->SendPacket( EGS_ROOM_LIST_REQ, kPacket );

	return true;
}

bool CX2StatePvPLobby::Handler_EGS_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ROOM_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		m_EndPage = kEvent.m_nTotalPage;

		for( int i = 0; i < (int)m_RoomList.size(); i++ )
		{
			CX2PVPRoom::PVPRoomSimpleInfo* pPVPRoomSimpleInfo = m_RoomList[i];
			SAFE_DELETE( pPVPRoomSimpleInfo );
		}
		m_RoomList.resize(0);

		for( int i = 0; i < (int)kEvent.m_vRoomInfo.size(); i++ )
		{
			KRoomInfo* pRoomInfo = &kEvent.m_vRoomInfo[i];
			CX2PVPRoom::PVPRoomSimpleInfo* pPVPRoomSimpleInfo = new CX2PVPRoom::PVPRoomSimpleInfo;
			pPVPRoomSimpleInfo->Set_KRoomInfo( pRoomInfo );

			m_RoomList.push_back( pPVPRoomSimpleInfo );
		}

		ResetRoomListUI();
	}
	return true;
}


bool CX2StatePvPLobby::Handler_EGS_CREATE_ROOM_REQ()
{
#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_CREATE_ROOM_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	m_bEnableRefreshRoomList = false;


	KEGS_CREATE_ROOM_REQ kEGS_CREATE_ROOM_REQ;

#ifdef PVP_NO_PASSWORD_ERROR
	if( false == CreatePVPRoomPacket( kEGS_CREATE_ROOM_REQ ) )
	{
		return false;
	}
#else
	CreatePVPRoomPacket( kEGS_CREATE_ROOM_REQ );
#endif PVP_NO_PASSWORD_ERROR

	kEGS_CREATE_ROOM_REQ.m_bObserver = m_bCreateRoomObserver;


	m_pDLGCreateRoom->SetShowEnable(false, false);

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_683 ), this );
	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_ROOM_REQ, kEGS_CREATE_ROOM_REQ );
	g_pMain->AddServerPacket( EGS_CREATE_ROOM_ACK );

	return true;
}

bool CX2StatePvPLobby::Handler_EGS_CREATE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	//SAFE_DELETE_DIALOG( m_pDLGCreateRoom );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_CREATE_ROOM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef HEAP_BROKEN_BY_ROOM
			CX2Room::InitializeRoomPacketData();
			CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
				kEvent.m_vecObserverSlot, kEvent.m_wstrCNIP );

			g_pData->DeleteAllRooms();
			g_pData->ResetPVPRoom();
			if ( NULL != g_pX2Room && CX2Room::IsNewDataSet() )		/// 새로운 데이터가 셋팅 되었으면
				g_pX2Room->ApplyRoomPacketData();
#else	HEAP_BROKEN_BY_ROOM
			//방 데이터 제작
			CX2PVPRoom* pCX2PVPRoom = g_pData->ResetPVPRoom();
			pCX2PVPRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
			pCX2PVPRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );
			pCX2PVPRoom->Set_KRoomObserverSlotInfoList( kEvent.m_vecObserverSlot );
			pCX2PVPRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
			pCX2PVPRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
           if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_PVP ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif // HEAP_BROKEN_BY_ROOM


			g_pData->GetUIManager()->CloseAll();

			//방으로 스테이트 이동
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_438 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );

			return true;
		}
	}

	m_bEnableRefreshRoomList = true;

	return true;
}


bool CX2StatePvPLobby::Handler_EGS_JOIN_ROOM_REQ( UidType roomUid, bool bIsObserver /*= false*/, wstring roomPassword /*= L""*/ )
{



#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_JOIN_ROOM_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	m_bEnableRefreshRoomList = false;

	KEGS_JOIN_ROOM_REQ kPacket;
	kPacket.m_iRoomUID			= roomUid;
	kPacket.m_wstrPassword		= roomPassword;
	kPacket.m_bIsObserver		= bIsObserver;
#ifdef SERV_NEW_PVPROOM_PROCESS
	kPacket.m_bInvitation		= g_pMain->GetStateAutoChanger().GetTargetDetail().m_bInvitation;
#endif SERV_NEW_PVPROOM_PROCESS

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_684 ), this );
	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_ROOM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_JOIN_ROOM_ACK );

	return true;
}

bool CX2StatePvPLobby::Handler_EGS_JOIN_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_JOIN_ROOM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef HEAP_BROKEN_BY_ROOM
			CX2Room::InitializeRoomPacketData();
			CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
				kEvent.m_vecObserverSlot, kEvent.m_wstrCNIP );

			g_pData->DeleteAllRooms();
			g_pData->ResetPVPRoom();
			if ( NULL != g_pX2Room && CX2Room::IsNewDataSet() )		/// 새로운 데이터가 셋팅 되었으면
				g_pX2Room->ApplyRoomPacketData();
#else	HEAP_BROKEN_BY_ROOM
			//방 데이터 제작
			CX2PVPRoom* pCX2PVPRoom = g_pData->ResetPVPRoom();
			pCX2PVPRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
			pCX2PVPRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );
			pCX2PVPRoom->Set_KRoomObserverSlotInfoList( kEvent.m_vecObserverSlot );
			pCX2PVPRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
			pCX2PVPRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
           if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_PVP ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif // HEAP_BROKEN_BY_ROOM



			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
			{
				if( CX2Main::XS_PVP_ROOM == g_pMain->GetStateAutoChanger().GetTargetStateID() )
				{
					g_pMain->GetStateAutoChanger().SetTryEnterRoomCount( 0 );
					g_pMain->GetStateAutoChanger().EndStateChange();
				}
			}

			g_pData->GetUIManager()->CloseAll();

			//방으로 스테이트 이동
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_438 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );	

			return true;
		}
		else
		{
			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
			{
				if( g_pMain->GetStateAutoChanger().GetTryEnterRoomCount() > 1 )
				{
					g_pMain->GetStateAutoChanger().SetTryEnterRoomCount( 0 );
					g_pMain->GetStateAutoChanger().EndStateChange();

					if( NetError::ERR_ROOM_04 == kEvent.m_iOK )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_685 ), this );
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_686 ), this );
					}
				}
				else
				{
					if( NetError::ERR_ROOM_04 == kEvent.m_iOK )
					{
						g_pMain->GetStateAutoChanger().TryEnterPVPRoom( false );
#ifdef SERV_NEW_PVPROOM_PROCESS
						g_pMain->GetStateAutoChanger().GetTargetDetail().m_bInvitation = false;	// 초대인지 아닌지 체크하는 변수 사용 다했으므로 초기화
#endif SERV_NEW_PVPROOM_PROCESS
					}
					else
					{
						g_pMain->GetStateAutoChanger().SetTryEnterRoomCount( 0 );
						g_pMain->GetStateAutoChanger().EndStateChange();
					}
				}
			}
		}

		m_bEnableRefreshRoomList = true;	
		return true;
	}


	m_bEnableRefreshRoomList = true;	
	return false;
}



bool CX2StatePvPLobby::Handler_EGS_CHANGE_PVP_ROOM_LIST_REQ( UINT channelID )
{
	if( g_pMain->GetConnectedChannelID() == channelID )
		return true;

#ifdef TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION
	// 특정 아이템을 가지고 있으면 대회 채널 입장 못함
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( channelID ) )
	{
		if( NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
		{	
 			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
 			{
 				g_pMain->GetStateAutoChanger().EndStateChange();
 			}

			return true;
		}
	}
#endif TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION
#ifdef TEMP_HERO_MATCH_ADMISSION_ITEM_POSSESSION
	// 특정 아이템을 가지고 있지 않으면 대회 채널 입장 못함
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( channelID ) )
	{
		if( NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
		{	
			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
			{
				g_pMain->GetStateAutoChanger().EndStateChange();
			}

			return true;
		}
	}
#endif

	m_TempChannelID = channelID;

	KEGS_CHANGE_PVP_ROOM_LIST_REQ kPacket;
	kPacket = channelID;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PVP_ROOM_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_PVP_ROOM_LIST_ACK );

	return true;
}


bool CX2StatePvPLobby::Handler_EGS_CHANGE_PVP_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ROOM_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_PVP_ROOM_LIST_ACK ) == true )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			SelectChannel( m_TempChannelID );

			m_NowPage = 1;
			Handler_EGS_ROOM_LIST_REQ( m_NowPage );


 			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
 			{
 				g_pMain->GetStateAutoChanger().DoStateChange( (int)g_pMain->GetNowStateID() );
 			}
		}
		else
		{
			SelectChannel( g_pMain->GetConnectedChannelID() );
			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
 			{
 				g_pMain->GetStateAutoChanger().EndStateChange();
 			}
		}
		return true;
	}
	return false;

}




void CX2StatePvPLobby::ResetRoomListUI()
{

	WCHAR buff[256] = {0};

	CKTDGUIStatic* pStaticPageNum = (CKTDGUIStatic*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_Roomlist_PageNum" );

	if( m_NowPage > m_EndPage )
		m_NowPage = m_EndPage;

	StringCchPrintf( buff, 256, L"%d/%d", m_NowPage, m_EndPage );
	//wsprintf( buff, L"%d/%d", m_NowPage, m_EndPage );
	pStaticPageNum->GetString(0)->msg = buff;

	// 초기화 : 다 꺼버려
	for( int i=0; i<m_iMaxNumOfRoom; i++ )
	{
		SimpleRoom* pSimpleRoom = m_SimpleRoomList[i];
		StringCchPrintf( buff, 256, L"ListButton%d", i);
		//wsprintf( buff, L"ListButton%d", i);
		pSimpleRoom->pButton = (CKTDGUIButton*) m_pDLGPvPLobby->GetControl( buff );
		StringCchPrintf( buff, 256, L"ListStatic%d", i);
		//wsprintf( buff, L"ListStatic%d", i);
		pSimpleRoom->pStatic = (CKTDGUIStatic*) m_pDLGPvPLobby->GetControl( buff );
		if ( pSimpleRoom->pStatic != NULL )
			pSimpleRoom->pStatic->SetShow(false);
		if ( pSimpleRoom->pButton != NULL )
			pSimpleRoom->pButton->SetShow(false);


		// 형식이 변경된 관계로 로그 삭제
		// 		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( pDialog ) == false )
		// 		{
		// 			ErrorLog( XEM_ERROR95 );
		// 			continue;
		// 		}
	}

	if ( m_bEnableResetDLGRoomList == false )		// 초기화 안 할때 
		return;


	// int iRoomCount = 0;
	for( UINT i=0; i<m_RoomList.size(); i++ )
	{
		CX2PVPRoom::PVPRoomSimpleInfo* pRoomData = m_RoomList[i];
		if ( pRoomData == NULL )
			continue;

		//  SimpleRoom* pSimpleRoom		= m_SimpleRoomList[iRoomCount];
		SimpleRoom* pSimpleRoom		= m_SimpleRoomList[i];
		if ( pSimpleRoom->pStatic == NULL || pSimpleRoom->pButton == NULL )
			continue;

		pSimpleRoom->roomUid		= pRoomData->m_RoomUID;

		// observer join 버튼은 운영자이상만 볼 수 있게 : 나중에
		// 		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER ) 
		// 		{
		// 			CKTDGUIButton* pButton_ObserverJoin = (CKTDGUIButton*) pDialog->GetControl( L"Button_Observer_Join" );
		// 			pButton_ObserverJoin->SetShowEnable( true, true );
		// 		}

		// 자물쇠 표시 (Picture0)
		if ( pRoomData->m_bPublic )
			pSimpleRoom->pStatic->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture02.TGA", L"unlock" );
		else
			pSimpleRoom->pStatic->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture02.TGA", L"lock" );


		// 방 상태 표시(Picture1)
		switch(pRoomData->m_RoomState)
		{
		case CX2Room::RS_PLAY:
		case CX2Room::RS_LOADING:
			if( pRoomData->m_PVPGameType == CX2PVPRoom::PGT_TEAM || pRoomData->m_JoinSlot >= pRoomData->m_MaxSlot )
				pSimpleRoom->pStatic->GetPicture(1)->SetTex( L"DLG_UI_Title01.TGA", L"play" );
			else
				pSimpleRoom->pStatic->GetPicture(1)->SetTex( L"DLG_UI_Title01.TGA", L"join" );
			break;
		default:
			if( pRoomData->m_JoinSlot >= pRoomData->m_MaxSlot )
				pSimpleRoom->pStatic->GetPicture(1)->SetTex( L"DLG_UI_Title01.TGA", L"full" );
			else
				pSimpleRoom->pStatic->GetPicture(1)->SetTex( L"DLG_UI_Title01.TGA", L"wait" );
			break;
		}

		// 		// 게임방식 표시(Picture2)
		// 		switch(pRoomData->m_PVPGameType)
		// 		{
		// 		case CX2PVPRoom::PGT_TEAM:
		// 			pSimpleRoom->pStatic->GetPicture(2)->SetTex( L"DLG_UI_Title01.TGA", L"team_match" );
		// 			break;
		// 		case CX2PVPRoom::PGT_TEAM_DEATH:
		// 			pSimpleRoom->pStatic->GetPicture(2)->SetTex( L"DLG_UI_Title01.TGA", L"death_match" );
		// 			break;
		// 		default:										// default 서바이벌
		// 		case CX2PVPRoom::PGT_SURVIVAL:
		// 			pSimpleRoom->pStatic->GetPicture(2)->SetTex( L"DLG_UI_Title01.TGA", L"survival_match" );
		// 			break;
		// 		}


		// 인원수 (String0)
		WCHAR userNumBuff[256] = {0};
		StringCchPrintf( userNumBuff, 256, L"%d/%d", pRoomData->m_JoinSlot, pRoomData->m_MaxSlot );
		//wsprintf( userNumBuff, L"%d/%d", pRoomData->m_JoinSlot, pRoomData->m_MaxSlot );
		pSimpleRoom->pStatic->GetString(0)->msg = userNumBuff;

		// 킬수(String1) : play channel 에서는 킬수 안 보이게 하자
		// 팀매치 게임 경우에도 킬수 안 보이게 하자
		if(KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID()) ||
			pRoomData->m_PVPGameType == CX2PVPRoom::PGT_TEAM)
		{
			pSimpleRoom->pStatic->GetString(1)->msg = L"";			
		}
		else
		{
			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"%d" ,pRoomData->m_WinKillNum );
			//wsprintf( buff,  L"%d" ,pRoomData->m_WinKillNum );
			pSimpleRoom->pStatic->GetString(1)->msg = buff;

		}

		// 방제 (String2)
		//pSimpleRoom->pStatic->GetString(2)->msg = L"";
#if defined(CLIENT_COUNTRY_US) || defined(CLIENT_COUNTRY_PH) || defined(CLIENT_COUNTRY_IN)
		if ( pRoomData->m_RoomName.size() < 20 )
#else // CLIENT_COUNTRY_US || defined(CLIENT_COUNTRY_PH) || defined(CLIENT_COUNTRY_IN)
		if ( pRoomData->m_RoomName.size() < 12 )
#endif // CLIENT_COUNTRY_US || defined(CLIENT_COUNTRY_PH) || defined(CLIENT_COUNTRY_IN)
		{
			pSimpleRoom->pStatic->GetString(2)->msg = pRoomData->m_RoomName;
		}
		else
		{
#if defined(CLIENT_COUNTRY_US) || defined(CLIENT_COUNTRY_PH) || defined(CLIENT_COUNTRY_IN)
			pSimpleRoom->pStatic->GetString(2)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( pRoomData->m_RoomName.c_str(), 170, SLOT_MANAGER_FONT_INDEX );
#else // CLIENT_COUNTRY_US || defined(CLIENT_COUNTRY_PH) || defined(CLIENT_COUNTRY_IN)
			pSimpleRoom->pStatic->GetString(2)->msg.assign( pRoomData->m_RoomName.begin(), pRoomData->m_RoomName.begin() + 12 );
#endif // CLIENT_COUNTRY_US || defined(CLIENT_COUNTRY_PH) || defined(CLIENT_COUNTRY_IN)
		}


		// 게임방식 (String3)
		// 왜 스트링3이냐면 이건 나중에 변경되었기 때문입니다
		switch(pRoomData->m_PVPGameType)
		{
		case CX2PVPRoom::PGT_TEAM:
			pSimpleRoom->pStatic->GetString(3)->msg = GET_STRING( STR_ID_687 );
			break;
		case CX2PVPRoom::PGT_TEAM_DEATH:
			pSimpleRoom->pStatic->GetString(3)->msg = GET_STRING( STR_ID_688 );
			break;
		default:										// default 서바이벌
		case CX2PVPRoom::PGT_SURVIVAL:
			pSimpleRoom->pStatic->GetString(3)->msg = GET_STRING( STR_ID_689 );
			break;
		}

#ifdef DUNGEON_ITEM
		// 아이템모드 (String4)
		// 왜 스트링3이냐면 이건 나중에 변경되었기 때문입니다
		if(pRoomData->m_bItemMode == true)	
		{
			// 아이템전
			pSimpleRoom->pStatic->GetString(4)->msg = GET_STRING( STR_ID_4817 );
		}
		else
		{
			// 파워전
			pSimpleRoom->pStatic->GetString(4)->msg = GET_STRING( STR_ID_4818 );
		}
#endif


		pSimpleRoom->pButton->SetShowEnable(true, true);
		pSimpleRoom->pStatic->SetShowEnable(true, true);
		// iRoomCount++;
	}
}

void CX2StatePvPLobby::OpenCreateRoomDLG()
{
	m_pDLGCreateRoom->SetShowEnable(true, true);

	//m_pDLGCreateRoom->SetName( L"DLG_Create_PVP_Room" );
	// 	// observer
	// 	CKTDGUICheckBox* pCheckBox_Observer_Create = (CKTDGUICheckBox*) m_pDLGCreateRoom->GetControl( L"CheckBox_Observer_Create" );
	// 	if( NULL != pCheckBox_Observer_Create )
	// 	{
	// 		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER ) 
	// 		{
	// 			pCheckBox_Observer_Create->SetShowEnable( true, true );
	// 		}
	// 		else
	// 		{
	// 			pCheckBox_Observer_Create->SetShowEnable( false, false );
	// 		}
	// 	}

	// 디폴트 세팅 : 팀데스 4킬 500초 공개방
	m_bIsPublicRoom = true;
	m_CreatePVPRoomType = CX2PVPRoom::PGT_TEAM_DEATH;
	m_KillCountForCreateRoomIndex = 0;
	m_PlayTimeForCreateRoomIndex = 1;
#ifdef DUNGEON_ITEM
	m_bItemMode = true;
#endif


	// 다이얼로그 초기화 : 일단 Control들을 얻어온다
	CKTDGUIRadioButton*	pTeamMode = (CKTDGUIRadioButton*)m_pDLGCreateRoom->GetControl( L"team_match" );
	CKTDGUIRadioButton*	pTeamDeathMode = (CKTDGUIRadioButton*)m_pDLGCreateRoom->GetControl( L"death_match" );
	CKTDGUIRadioButton*	pSurvivalMode = (CKTDGUIRadioButton*)m_pDLGCreateRoom->GetControl( L"survival_match" );

	CKTDGUIStatic* pStaticTeamInactive = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"Static_Team_Inactive" );
	CKTDGUIStatic* pStaticTeamDeathInactive = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"Static_Team_Death_Inactive" );
	CKTDGUIStatic* pStaticSurvivalInactive = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"Static_Survival_Inactive" );
	CKTDGUIStatic* pStaticPlayTime = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"StaticTime" );
	CKTDGUIButton* pTimeMinus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"kill_minus" );
	CKTDGUIButton* pTimePlus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"kill_plus" );
	CKTDGUIStatic* pStaticKillCount = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"StaticKill" );
	CKTDGUIButton* pKillMinus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"time_minus" );
	CKTDGUIButton* pKillPlus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"time_plus" );

#ifdef DUNGEON_ITEM
	CKTDGUIStatic* pStaticItemMode = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"StaticItem" );
	CKTDGUIButton* pItemModeMinus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"Item_minus" );
	CKTDGUIButton* pItemmodePlus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"Item_plus" );
#endif

	switch(g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ))
	{
	case KPVPChannelInfo::PCC_PLAY:
		// 연습채널. 팀매치 선택 불가능, 시간/킬수 설정되지 않음
		{
			pTeamMode->SetShowEnable(false, false);
			pTeamDeathMode->SetShowEnable(true, true);
			pSurvivalMode->SetShowEnable(true, true);

			pTeamDeathMode->SetChecked(true);

			pStaticTeamInactive->SetShowEnable(true, true);
			pStaticTeamDeathInactive->SetShowEnable(false, false);
			pStaticSurvivalInactive->SetShowEnable(false, false);

			pStaticPlayTime->SetShowEnable(false, false);
			pTimePlus->SetShowEnable(false, false);
			pTimeMinus->SetShowEnable(false, false);

			pStaticKillCount->SetShowEnable(false, false);
			pKillMinus->SetShowEnable(false, false);
			pKillPlus->SetShowEnable(false, false);

#ifdef DUNGEON_ITEM
			pStaticItemMode->SetShowEnable(true, true);
			pItemModeMinus->SetShowEnable(true, true);
			pItemmodePlus->SetShowEnable(true, true);
#endif
		}
		break;


	case KPVPChannelInfo::PCC_TOURNAMENT:
		{
#ifdef TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
			pTeamMode->SetShowEnable(false, false);
			pTeamDeathMode->SetShowEnable(true, true);
			pSurvivalMode->SetShowEnable(false, false);

			pTeamDeathMode->SetChecked(true);

	#ifdef FIX_HERO_MATCH_UI	/// 선택 못할꺼면 비활성화 하지 말고 그냥 숨기기만 하자
			pStaticTeamInactive->SetShowEnable(false, false);
			pStaticTeamDeathInactive->SetShowEnable(false, false);
			pStaticSurvivalInactive->SetShowEnable(false, false);
	#else  FIX_HERO_MATCH_UI
			pStaticTeamInactive->SetShowEnable(true, true);
			pStaticTeamDeathInactive->SetShowEnable(false, false);
			pStaticSurvivalInactive->SetShowEnable(true, true);
	#endif FIX_HERO_MATCH_UI

			m_CreatePVPRoomType	= CX2PVPRoom::PGT_TEAM_DEATH;
#endif TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE

#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING

	#ifndef FIX_HERO_MATCH_UI		/// 증감 화살표만 숨기자.
			pStaticPlayTime->SetShowEnable(false, false);
	#endif FIX_HERO_MATCH_UI

			pTimePlus->SetShowEnable(false, false);
			pTimeMinus->SetShowEnable(false, false);

	#ifndef FIX_HERO_MATCH_UI		/// 증감 화살표만 숨기자.
			pStaticKillCount->SetShowEnable(false, false);
	#endif FIX_HERO_MATCH_UI

			pKillMinus->SetShowEnable(false, false);
			pKillPlus->SetShowEnable(false, false);
#else
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_TIME
			pStaticPlayTime->SetShowEnable(false, false);
			pTimePlus->SetShowEnable(false, false);
			pTimeMinus->SetShowEnable(false, false);
#endif
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_KILLCOUNT
			pStaticKillCount->SetShowEnable(false, false);
			pKillMinus->SetShowEnable(false, false);
			pKillPlus->SetShowEnable(false, false);
#endif
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING			

#ifdef DUNGEON_ITEM

	#ifndef FIX_HERO_MATCH_UI		/// 증감 화살표만 숨기자.
			pStaticItemMode->SetShowEnable(false, false);
	#endif FIX_HERO_MATCH_UI

			pItemModeMinus->SetShowEnable(false, false);
			pItemmodePlus->SetShowEnable(false, false);

			m_bItemMode = false;
#endif
		}
		break;

	default:
		// 그 외 채널 : 다 정상
		{
			pTeamMode->SetShowEnable(true, true);
			pTeamDeathMode->SetShowEnable(true, true);
			pSurvivalMode->SetShowEnable(true, true);

			pTeamDeathMode->SetChecked(true);

			pStaticTeamInactive->SetShowEnable(false, false);
			pStaticTeamDeathInactive->SetShowEnable(false, false);
			pStaticSurvivalInactive->SetShowEnable(false, false);

			pStaticPlayTime->SetShowEnable(true, true);
			pTimePlus->SetShowEnable(true, true);
			pTimeMinus->SetShowEnable(true, true);

			pStaticKillCount->SetShowEnable(true, true);
			pKillMinus->SetShowEnable(true, true);
			pKillPlus->SetShowEnable(true, true);

#ifdef DUNGEON_ITEM
			pStaticItemMode->SetShowEnable(true, true);
			pItemModeMinus->SetShowEnable(true, true);
			pItemmodePlus->SetShowEnable(true, true);
#endif
		}
		break;
	}

	// 방제 설정 ( DLG_UI_PVP_Make.lua 최상단을 보시오 )
	CKTDGUIIMEEditBox* pRoomName	= (CKTDGUIIMEEditBox*)m_pDLGCreateRoom->GetControl( L"IME_EditRoomName" );	
	const int iNumRoomNamePreset = m_pDLGCreateRoom->GetDummyInt(0);
	int iRandomNameIndex = rand()%iNumRoomNamePreset;
	wstring roomName = m_pDLGCreateRoom->GetDummyString( iRandomNameIndex );
	pRoomName->SetText( roomName.c_str(), true );

	pRoomName->RequestFocus();

	WCHAR buff[256] = {0};

#ifdef DUNGEON_ITEM
	// 아이템전/파워전 초기 상태값
	if( m_bItemMode == false )
	{
		// 파워 전
		pStaticItemMode->GetString(0)->msg = GET_STRING( STR_ID_4818 );
	}
	else
	{
		// 아이템 전
		pStaticItemMode->GetString(0)->msg = GET_STRING( STR_ID_4817 );
	}	
#endif

	// 시간 써주고
	StringCchPrintf( buff, 256, L"%d", (int)GetPlayTimeForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );
	//wsprintf( buff, L"%d", (int)GetPlayTimeForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );
	pStaticPlayTime->GetString(0)->msg = buff;

	// 킬수 써주고
	StringCchPrintf( buff, 256, L"%d", (int)GetKillCountForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );
	//wsprintf( buff, L"%d", (int)GetKillCountForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType ) );
	pStaticKillCount->GetString(0)->msg = buff;

	// 체크박스 끄고
	CKTDGUICheckBox* pPrivateCheckBox = (CKTDGUICheckBox*)m_pDLGCreateRoom->GetControl( L"g_pCheckBoxsecret" );
	pPrivateCheckBox->SetShowEnable(true, true);
	pPrivateCheckBox->SetChecked(false);

	//{{ kimhc // 2009-05-04, 17:10
	// SpecialUser 이상의 등급일 경우 Observer 모드를 선택 할 수 있는 CheckBox 추가
	CKTDGUIStatic* pStaticObserver		= static_cast< CKTDGUIStatic* > ( m_pDLGCreateRoom->GetControl( L"StaticObserver" ) );
	CKTDGUICheckBox* pObserverCheckBox	= static_cast< CKTDGUICheckBox* > ( m_pDLGCreateRoom->GetControl( L"g_pCheckBoxObserver" ) );
	
	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER ) // SpecialUser 이상(SpecialUser, Operator, Dev) 일 경우
	{
		pStaticObserver->SetShowEnable( true, true );			// SpecialUser 이상(SpecialUser, Operator, Dev) 일 경우에 보임
		pObserverCheckBox->SetShowEnable( true, true );
	}
	else
	{
		pStaticObserver->SetShowEnable( false, false );			// NormalUser(SpecialUser, Operator, Dev 외) 일 경우에 안 보임
		pObserverCheckBox->SetShowEnable( false, false );
	}
	pObserverCheckBox->SetChecked( false );
	//}} kimhc // 2009-05-04, 17:10

	// 비번 박스 일단 끄고
	CKTDGUIIMEEditBox* pIMEEditBoxPassword = (CKTDGUIIMEEditBox*)m_pDLGCreateRoom->GetControl( L"IME_EditPassword" );
	pIMEEditBoxPassword->SetShowEnable( false, false );

}


void CX2StatePvPLobby::SelectGametypeOnCreateRoomDLG(CX2PVPRoom::PVP_GAME_TYPE GameType)
{

	m_CreatePVPRoomType = GameType;

#ifdef MODIFY_PVP_ROOM_ITEM_UI
	//{{ JHKang // 2010-08-23, 15:49 연습채널 아이템 대전 UI 수정
	CKTDGUIStatic* pStaticKillCount = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"StaticKill" );
	CKTDGUIButton* pKillMinus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"kill_minus" );
	CKTDGUIButton* pKillPlus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"kill_plus" );

#ifdef DUNGEON_ITEM
	CKTDGUIButton* pItemModeMinus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"Item_minus" );
	CKTDGUIButton* pItemmodePlus = (CKTDGUIButton*)m_pDLGCreateRoom->GetControl( L"Item_plus" );
	CKTDGUIStatic* pStaticItemMode = (CKTDGUIStatic*) m_pDLGCreateRoom->GetControl( L"StaticItem" );
#endif
	switch ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
	case KPVPChannelInfo::PCC_PLAY:

#ifdef TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
	case KPVPChannelInfo::PCC_TOURNAMENT:
		if (GameType == CX2PVPRoom::PGT_SURVIVAL)
		{
			m_bItemMode = true;		// 아이템 전으로 강제 변경
			pItemmodePlus->SetShowEnable( false, false );
			pItemModeMinus->SetShowEnable( false, false );
		}
		else
		{
			pItemmodePlus->SetShowEnable( true, true );
			pItemModeMinus->SetShowEnable( true, true );
		}
		break;
#endif TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE	
	default:
		switch(GameType)
		{
		case CX2PVPRoom::PGT_TEAM:
			{
				pStaticKillCount->SetShowEnable(false, false);
				pKillMinus->SetShowEnable(false, false);
				pKillPlus->SetShowEnable(false, false);

				m_KillCountForCreateRoomIndex = 0;
				m_PlayTimeForCreateRoomIndex = 0;

#ifdef DUNGEON_ITEM
				if(pItemModeMinus != NULL && pItemmodePlus != NULL)
				{
					m_bItemMode = true;
					pItemmodePlus->SetShowEnable(true, true);
					pItemModeMinus->SetShowEnable(true, true);
				}
#endif
			}
			break;
		case CX2PVPRoom::PGT_TEAM_DEATH:
			{
				pStaticKillCount->SetShowEnable(true, true);
				pKillMinus->SetShowEnable(true, true);
				pKillPlus->SetShowEnable(true, true);

				m_KillCountForCreateRoomIndex = 0;			// 4 kill
				m_PlayTimeForCreateRoomIndex = 1;			// 500 초

#ifdef DUNGEON_ITEM
				if(pItemModeMinus != NULL && pItemmodePlus != NULL)
				{
					m_bItemMode = true;
					pItemmodePlus->SetShowEnable(true, true);
					pItemModeMinus->SetShowEnable(true, true);
				}
#endif
			}
			break;
		case CX2PVPRoom::PGT_SURVIVAL:
			{
				pStaticKillCount->SetShowEnable(true, true);
				pKillMinus->SetShowEnable(true, true);
				pKillPlus->SetShowEnable(true, true);


				m_KillCountForCreateRoomIndex = 0;			// 3 kill
				m_PlayTimeForCreateRoomIndex = 1;			// 500 초

#ifdef DUNGEON_ITEM
				if(pItemModeMinus != NULL && pItemmodePlus != NULL)
				{
					m_bItemMode = true;
					pItemmodePlus->SetShowEnable(false, false);
					pItemModeMinus->SetShowEnable(false, false);
				}
#endif
			}
			break;
		}

		WCHAR buff[256] = {0};
		CKTDGUIStatic* pStatic = NULL;

		StringCchPrintf( buff, 256, L"%d", (int)GetKillCountForCreateRoom( GameType ) );
		//wsprintf( buff, L"%d", (int)GetKillCountForCreateRoom( GameType ) );
		pStatic = (CKTDGUIStatic*)m_pDLGCreateRoom->GetControl( L"StaticKill" );
		pStatic->GetString(0)->msg = buff;

		StringCchPrintf( buff, 256, L"%d", (int)GetPlayTimeForCreateRoom( GameType ) );
		//wsprintf( buff, L"%d", (int)GetPlayTimeForCreateRoom( GameType ) );
		pStatic = (CKTDGUIStatic*)m_pDLGCreateRoom->GetControl( L"StaticTime" );
		pStatic->GetString(0)->msg = buff;
	}
//}}
#endif MODIFY_PVP_ROOM_ITEM_UI

#ifdef DUNGEON_ITEM
	if( m_bItemMode == false )
	{				
		pStaticItemMode->GetString(0)->msg = GET_STRING( STR_ID_4818 ); // 파워 전
	}
	else
	{				
		pStaticItemMode->GetString(0)->msg = GET_STRING( STR_ID_4817 ); // 아이템 전
	}
#endif

	return;
}




float CX2StatePvPLobby::GetPlayTimeForCreateRoom( CX2PVPRoom::PVP_GAME_TYPE eGameType )
{
#if defined(TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING) || defined(TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_TIME)
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		return MAGIC_HERO_MATCH_GAME_TIME;
	}
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING		


	switch( eGameType )
	{
	case CX2PVPRoom::PGT_TEAM:
		{
			return (float) TEAM_PVP_PLAY_TIME[ m_PlayTimeForCreateRoomIndex ];
		} break;

	case CX2PVPRoom::PGT_TEAM_DEATH:
		{
			return (float) TEAM_DEATH_PVP_PLAY_TIME[ m_PlayTimeForCreateRoomIndex ];
		} break;

	case CX2PVPRoom::PGT_SURVIVAL:
		{
			return (float) SURVIVAL_PVP_PLAY_TIME[ m_PlayTimeForCreateRoomIndex ];
		} break;
	}

	return 1.f;
}


int CX2StatePvPLobby::GetKillCountForCreateRoom( CX2PVPRoom::PVP_GAME_TYPE eGameType )
{

#if defined(TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING) || defined(TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_KILLCOUNT)
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		return MAGIC_HERO_MATCH_GAME_KILL_COUNT;
	}
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING		



	switch( eGameType )
	{
	case CX2PVPRoom::PGT_TEAM:
		{
			return TEAM_PVP_KILL_COUNT;
		} break;

	case CX2PVPRoom::PGT_TEAM_DEATH:
		{
			return TEAM_DEATH_PVP_KILL_COUNT[ m_KillCountForCreateRoomIndex ];
		} break;

	case CX2PVPRoom::PGT_SURVIVAL:
		{
			return SURVIVAL_PVP_KILL_COUNT[ m_KillCountForCreateRoomIndex ];
		} break;
	}

	return 1;
}


void CX2StatePvPLobby::OpenDLGJoinPasswordRoom( UidType roomUID )
{
 	m_JoinRoomUid = roomUID;
 
 	SAFE_DELETE_DIALOG( m_pDLGJoinPasswordRoom );
 
	// 이 다이얼로그가 왠지 MsgProc 중에 crash한다 : 입력 받고 SAFE_DELETE로 처리했기 때문에 생겼던 문제.
 	m_pDLGJoinPasswordRoom = new CKTDGUIDialog( this, L"DLG_UI_Num_Input.lua" );
 	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGJoinPasswordRoom );
 
 	CKTDGUIIMEEditBox* pTempPasswordEditBox = (CKTDGUIIMEEditBox*)m_pDLGJoinPasswordRoom->GetControl( L"IME_EditJoinPassword" );
 	pTempPasswordEditBox->RequestFocus();
}


//bool CX2StatePvPLobby::ClickMenuBackButton()
//{
//	return Handler_EGS_STATE_CHANGE_FIELD_REQ();
//}

void CX2StatePvPLobby::SetShowStateDLG( bool bShow )
{

	if ( m_pDLGPVPLobbyBack != NULL )
	{
		m_pDLGPVPLobbyBack->SetEnable( bShow );
		m_pDLGPVPLobbyBack->SetShow( bShow );
	}
	
	if ( m_pDLGPvPLobby != NULL )
	{
		m_pDLGPvPLobby ->SetEnable( bShow );
		m_pDLGPvPLobby ->SetShow( bShow );
	}

	m_bEnableResetDLGRoomList = bShow;

	if ( m_pDLGCommonBG != NULL && m_pDLGCommonBG->GetControl( L"StaticPVP_Lobby_CharBG" ) != NULL )
	{
		m_pDLGCommonBG->GetControl( L"StaticPVP_Lobby_CharBG" )->SetShow( bShow );
	}

	if ( bShow == true )
	{
		ResetRoomListUI();
	}
	else
	{
		for ( int i = 0; i < (int)m_SimpleRoomList.size(); i++ )
		{
			SimpleRoom* pSimpleRoom = m_SimpleRoomList[i];
			if ( pSimpleRoom->pButton == NULL || pSimpleRoom->pStatic == NULL)
				continue;

			pSimpleRoom->pButton->SetShow( false );
			pSimpleRoom->pStatic->SetShow( false );
		}
	}
}


bool CX2StatePvPLobby::LastMsgByESC()
{
	OpenLastMsgPopUp( GET_STRING( STR_ID_690 ) );
	return true;
}

void CX2StatePvPLobby::LastMsgPopOk()
{
	Handler_EGS_STATE_CHANGE_FIELD_REQ();
}

void CX2StatePvPLobby::SettingChannelButton()
{
	wstring radioButtonName = L"Channel"; //1~9까지가 훈련병에서 E8까지 자유채널은 항상 Enable가능

	// 싸그리 끄고
	WCHAR buff[256] = {0};
	for ( int i = 0; i <= m_MaxChannelID; i++ )
	{
		StringCchPrintf( buff, 256, L"Channel%d", i );	
		//wsprintf( buff, L"Channel%d", i );	
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl( buff );
		StringCchPrintf( buff, 256, L"ChannelInactive%d", i );	
		//wsprintf( buff, L"ChannelInactive%d", i );	
		CKTDGUIStatic* pStaticInactive = (CKTDGUIStatic*)m_pDLGPvPLobby->GetControl( buff );
		if ( pRadioButton != NULL && pStaticInactive != NULL )
		{
			// 라디오는 끄고
			pRadioButton->SetEnable( false );
			pRadioButton->SetShow( false );
			// 비활성화 채널 스태틱을 여기서 일단 다 켜주자
			pStaticInactive->SetShow(true);
		}

	}

	for ( int i = 0; i <= m_MaxChannelID; i++ )
	{
		StringCchPrintf( buff, 256, L"Channel%d", i );
		//wsprintf( buff, L"Channel%d", i );	
		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl( buff );
		StringCchPrintf( buff, 256, L"ChannelInactive%d", i );	
		//wsprintf( buff, L"ChannelInactive%d", i );	
		CKTDGUIStatic* pStaticInactive = (CKTDGUIStatic*)m_pDLGPvPLobby->GetControl( buff );
		if ( pRadioButton != NULL && pStaticInactive != NULL )
		{
			std::vector<KPVPChannelInfo> channelList = g_pMain->GetChannelList();
			for ( int j = 0; j < (int)channelList.size(); j++ )
			{
				KPVPChannelInfo& channelInfo = channelList[j];
				//if ( channelInfo.m_iChannelID == ( PVP_CHANNEL_ID_RECRUIT + i - 1 ) )
				if ( channelInfo.m_cPVPChannelClass == pRadioButton->GetDummyInt(0) )
				{
					pRadioButton->SetEnable( true );
					pRadioButton->SetShow( true );
					// 비활성 스태틱 끄자
					pStaticInactive->SetShow(false);
				}
#ifdef TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION
				if( KPVPChannelInfo::PCC_TOURNAMENT == (KPVPChannelInfo::PVP_CHANNEL_CLASS) channelInfo.m_cPVPChannelClass )
				{
					if( NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
					{
						pRadioButton->SetShowEnable( false, false );
						// 비활성 스태틱을 켜자
						pStaticInactive->SetShow(true);
					}
				}
#endif TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION
#ifdef TEMP_HERO_MATCH_ADMISSION_ITEM_POSSESSION
				// 특정 아이템을 가지고 있지 않으면 대회 채널 입장 못함
				if( KPVPChannelInfo::PCC_TOURNAMENT == (KPVPChannelInfo::PVP_CHANNEL_CLASS) channelInfo.m_cPVPChannelClass )
				{
					if( NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
					{
						pRadioButton->SetShowEnable( false, false );
						// 비활성 스태틱을 켜자
						pStaticInactive->SetShow(true);
					}
				}
#endif
			}
		}
	}
}

int	CX2StatePvPLobby::GetChannelID( KPVPChannelInfo::PVP_CHANNEL_CLASS ePVP_CHANNEL_CLASS )
{
	std::vector<KPVPChannelInfo> channelList = g_pMain->GetChannelList();
	for ( int j = 0; j < (int)channelList.size(); j++ )
	{
		KPVPChannelInfo& channelInfo = channelList[j];
		if ( channelInfo.m_cPVPChannelClass == ePVP_CHANNEL_CLASS )
		{
			return channelInfo.m_iChannelID;
		}
	}


	return -1;
}


bool CX2StatePvPLobby::CheckUnitLevelByChannelList()
{
	 if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
		 return false;

	 CX2Unit* pNowUnit = g_pData->GetMyUser()->GetSelectUnit();

	 for ( int i = 0; i < (int)g_pMain->GetChannelList().size(); i++ )
	 {
		 KPVPChannelInfo kPVPChannel = g_pMain->GetChannelList().at(i);
#ifdef SERV_PVP_NEW_SYSTEM
		 if ( kPVPChannel.m_iChannelID == g_pMain->GetConnectedChannelID() &&
			 ( pNowUnit->GetUnitData().m_iRating < kPVPChannel.m_iMinRating || 
			 pNowUnit->GetUnitData().m_iRating > kPVPChannel.m_iMaxRating ) )
#else
		 //{{ 2007. 8. 28  최육사  level -> VP		 
		 if ( kPVPChannel.m_iChannelID == g_pMain->GetConnectedChannelID() &&
			 ( pNowUnit->GetUnitData().m_VSPointMax < kPVPChannel.m_nMinVSPoint || 
			 pNowUnit->GetUnitData().m_VSPointMax > kPVPChannel.m_nMaxVSPoint ) )
		 //}}
#endif
		 {
			 return false;
		 }
	 }

	 return true;
}



bool CX2StatePvPLobby::Handler_EGS_QUICK_JOIN_REQ( CX2PVPRoom::PVP_GAME_TYPE gameType, bool bItemMode )
{
	m_QuickJoinPVPGameType	= gameType;	

	KEGS_QUICK_JOIN_REQ kPacket;
	kPacket.m_iOption = (int)gameType;

#ifdef DUNGEON_ITEM
	bool bTempItemMode = bItemMode;
	if( gameType == CX2PVPRoom::PGT_SURVIVAL )
	{
		bTempItemMode = true;
	}
	kPacket.m_bIsItemMode = bTempItemMode;
	m_QuickJoinPVPItemMode = bTempItemMode;
#endif

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	g_pData->GetServerProtocol()->SendPacket( EGS_QUICK_JOIN_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_QUICK_JOIN_ACK );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_691 ), this );

	return true;
}

bool CX2StatePvPLobby::Handler_EGS_QUICK_JOIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_QUICK_JOIN_ACK ) == true )
	{
		if( kEvent.m_iOK == NetError::NET_OK )//g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			SAFE_DELETE_DIALOG( m_pDLGMsgBox );

#ifdef HEAP_BROKEN_BY_ROOM
			CX2Room::InitializeRoomPacketData();
			CX2Room::SetRoomPacketData( kEvent.m_RoomInfo, kEvent.m_vecSlot, 
				vector<KRoomSlotInfo>(), kEvent.m_wstrCNIP );

			g_pData->DeleteAllRooms();
			g_pData->ResetPVPRoom();
			if ( NULL != g_pX2Room && CX2Room::IsNewDataSet() )		/// 새로운 데이터가 셋팅 되었으면
				g_pX2Room->ApplyRoomPacketData();
#else	HEAP_BROKEN_BY_ROOM
			//방 데이터 제작
			CX2PVPRoom* pCX2PVPRoom = g_pData->ResetPVPRoom();
			pCX2PVPRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
			pCX2PVPRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot );
			pCX2PVPRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
			pCX2PVPRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
           if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_PVP ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif // HEAP_BROKEN_BY_ROOM


			g_pData->GetUIManager()->CloseAll();

			//방으로 스테이트 이동
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_438 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );

			return true;
		}
		else
		{
			// 퀵조인 실패시는 그냥 새로 방을 만든다
			KEGS_CREATE_ROOM_REQ kEGS_CREATE_ROOM_REQ;
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomType			= (CX2Room::ROOM_TYPE) CX2Room::RT_PVP;

			//TODO : 방만들기 패킷 보내기
			switch( m_QuickJoinPVPGameType )
			{
			case CX2PVPRoom::PGT_TEAM:
				{
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_PVPGameType	= CX2PVPRoom::PGT_TEAM;
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bCanIntrude	= false;
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= TEAM_PVP_KILL_COUNT;

					if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= 99999.f;
					}
					else
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= (float) TEAM_PVP_PLAY_TIME[0];
					}
#ifdef DUNGEON_ITEM
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode = m_QuickJoinPVPItemMode;
#endif
				}
				break;

			case CX2PVPRoom::PGT_TEAM_DEATH:
				{					
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_PVPGameType	= CX2PVPRoom::PGT_TEAM_DEATH;
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bCanIntrude	= true;
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= TEAM_DEATH_PVP_KILL_COUNT[0];
					if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime	= 99999.f;
					}
					else
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime	= (float)TEAM_DEATH_PVP_PLAY_TIME[1];
					}
#ifdef DUNGEON_ITEM
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode	= m_QuickJoinPVPItemMode;
#endif

#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING
					if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_PVPGameType	= CX2PVPRoom::PGT_TEAM_DEATH;
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bCanIntrude	= true;
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= MAGIC_HERO_MATCH_GAME_KILL_COUNT;
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= MAGIC_HERO_MATCH_GAME_TIME;
#ifdef DUNGEON_ITEM
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode	= false;
#endif
					}
#else
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING
					if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_PVPGameType	= CX2PVPRoom::PGT_TEAM_DEATH;
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bCanIntrude	= true;
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_KILLCOUNT
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= MAGIC_HERO_MATCH_GAME_KILL_COUNT;
#endif
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_TIME
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= MAGIC_HERO_MATCH_GAME_TIME;
#endif
#ifdef DUNGEON_ITEM
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode	= false;
#endif
					}
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING		

#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING		
				}
				break;

			case CX2PVPRoom::PGT_SURVIVAL:
				{
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_PVPGameType	= CX2PVPRoom::PGT_SURVIVAL;
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bCanIntrude	= true;
					kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= SURVIVAL_PVP_KILL_COUNT[0];
					if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= 99999.f;

#ifdef DUNGEON_ITEM
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode = true;
#endif
					}
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING
					else if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= MAGIC_HERO_MATCH_GAME_TIME;
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= (char) MAGIC_HERO_MATCH_GAME_KILL_COUNT;
#ifdef DUNGEON_ITEM
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode = false;
#endif
					}
#else
					else if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
					{
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_TIME
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= MAGIC_HERO_MATCH_GAME_TIME;
#endif
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_KILLCOUNT
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= (char) MAGIC_HERO_MATCH_GAME_KILL_COUNT;
#endif
#ifdef DUNGEON_ITEM
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode = false;
#endif
					}
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING
					else
					{
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= (float)SURVIVAL_PVP_PLAY_TIME[1];

#ifdef DUNGEON_ITEM
						kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode = true;
#endif

					}
				}
				break;
			}

			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomName		= GET_STRING( STR_ID_692 );
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_MaxSlot		= 8;
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic		= true;
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WorldID		= -1;
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_ShowPvpMapWorldID = -1;
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bTeamBalance	= true;

			g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_ROOM_REQ, kEGS_CREATE_ROOM_REQ );
			g_pMain->AddServerPacket( EGS_CREATE_ROOM_ACK );

			return true;
		}
	}

	return false;
}


#ifdef PVP_NO_PASSWORD_ERROR
bool CX2StatePvPLobby::CreatePVPRoomPacket( KEGS_CREATE_ROOM_REQ& kEGS_CREATE_ROOM_REQ )
#else
void CX2StatePvPLobby::CreatePVPRoomPacket( KEGS_CREATE_ROOM_REQ& kEGS_CREATE_ROOM_REQ )
#endif PVP_NO_PASSWORD_ERROR
{
	CKTDGUIIMEEditBox* pIMEEditBoxRoomName = (CKTDGUIIMEEditBox*)m_pDLGCreateRoom->GetControl( L"IME_EditRoomName" );

	wstring tempRoomName = pIMEEditBoxRoomName->GetText();

	// 대전방제 필터링
	tempRoomName = g_pMain->GetStringFilter()->FilteringChatString( tempRoomName.c_str(), L'♡' );

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomName = tempRoomName.substr( 0, 20 );
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomType			= (CX2Room::ROOM_TYPE) CX2Room::RT_PVP;

	switch(m_CreatePVPRoomType)
	{
	case CX2PVPRoom::PGT_TEAM:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bCanIntrude = false;
		}
		break;
	case CX2PVPRoom::PGT_TEAM_DEATH:
	case CX2PVPRoom::PGT_SURVIVAL:
		{
			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bCanIntrude = true;
		}
		break;
	}

	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_PVPGameType = m_CreatePVPRoomType;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_MaxSlot		= 8;

	if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= 99999.f;
		kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= (char)127;
	}
	else
	{
		kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime		= GetPlayTimeForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType );
		kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WinKillNum	= (char) GetKillCountForCreateRoom( (CX2PVPRoom::PVP_GAME_TYPE)m_CreatePVPRoomType );
	}


	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic		= m_bIsPublicRoom;
 
	CKTDGUIIMEEditBox* pIMEEditBoxRoomPassword		= (CKTDGUIIMEEditBox*)m_pDLGCreateRoom->GetControl( L"IME_EditPassword" );
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_wstrPassword	= pIMEEditBoxRoomPassword->GetText();



#ifdef PVP_NO_PASSWORD_ERROR
	if( kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_wstrPassword.length() == 0 && kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic == false )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(310,500), GET_STRING( STR_ID_22412 ), g_pMain->GetNowState() );
		return false;
	}
#endif PVP_NO_PASSWORD_ERROR

	
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_WorldID				= -1;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_ShowPvpMapWorldID		= -1;
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bTeamBalance	= true;	//현재 인터페이스 체크가 없기 때문에 임시로 항상 켜놓는다.

#ifdef DUNGEON_ITEM
	if( m_CreatePVPRoomType == CX2PVPRoom::PGT_SURVIVAL )
	{
		m_bItemMode = true;
	}
	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bIsItemMode = m_bItemMode;
#endif

#ifdef PVP_NO_PASSWORD_ERROR
	return true;
#endif PVP_NO_PASSWORD_ERROR

}

bool CX2StatePvPLobby::EnterTutorial()
{
	g_pMain->SetIsPlayingTutorial( true );
	return Handler_EGS_STATE_CHANGE_FIELD_REQ();
}

void CX2StatePvPLobby::SelectChannel( UINT channelID )
{
	g_pMain->SetConnectedChannelID( channelID );

	WCHAR buff[256] = {0};

	KPVPChannelInfo::PVP_CHANNEL_CLASS channelClass = KPVPChannelInfo::PCC_NONE;

	std::vector<KPVPChannelInfo> channelList = g_pMain->GetChannelList();
	for ( int j = 0; j < (int)channelList.size(); j++ )
	{
		KPVPChannelInfo& channelInfo = channelList[j];
		if ( channelInfo.m_iChannelID == channelID )
		{
			channelClass = (KPVPChannelInfo::PVP_CHANNEL_CLASS)channelInfo.m_cPVPChannelClass;
		}
	}

	//채널 탭을 눌러주고
	if ( channelClass != KPVPChannelInfo::PCC_NONE )
	{
		for ( int i = 0; i <= m_MaxChannelID; i++ )
		{
			StringCchPrintf( buff, 256, L"Channel%d", i );	
			//wsprintf( buff, L"Channel%d", i );	

			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl( buff );
			if ( pRadioButton != NULL && pRadioButton->GetDummyInt(0) == (int)channelClass )
			{
				pRadioButton->SetChecked( true );
			}
		}
	}


	
	{
		CKTDGUIButton* pButton_TeamMatch				= (CKTDGUIButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_Team_Quickstart" );		// 팀전 바로가기
		CKTDGUIButton* pButton_TeamDeathMatch			= (CKTDGUIButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_TeamDeath_Quickstart" );	// 팀데스 바로가기
		CKTDGUIButton* pButton_Survival					= (CKTDGUIButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_Survival_Quickstart" );	// 서바이벌 바로가기

#ifdef FIX_HERO_MATCH_UI
		CKTDGUIButton* pButton_TeamMatch_Item			= (CKTDGUIButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_Team_Item_Quickstart" );		// 팀전 바로가기
		CKTDGUIButton* pButton_TeamDeathMatch_Item		= (CKTDGUIButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_TeamDeath_Item_Quickstart" );	// 팀데스 바로가기
#endif FIX_HERO_MATCH_UI


		CKTDGUIRadioButton* pRadioButton_All			= (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_All_Tab" );		// 전체
		CKTDGUIRadioButton* pRadioButton_TeamMatch		= (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_Team_Tab" );		// 팀전 탭
		CKTDGUIRadioButton* pRadioButton_TeamDeathMatch	= (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_TeamDeath_Tab" );	// 팀데스 탭
		CKTDGUIRadioButton* pRadioButton_Survival		= (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl( L"UI_PvP_Lobby_Survival_Tab" );	// 서바이벌

		CKTDGUIStatic* pStatic_TeamMatch_Inactive		= (CKTDGUIStatic*) m_pDLGPvPLobby->GetControl( L"Static_Team_Inactive");
		CKTDGUIStatic* pStatic_TeamDeathMatch_Inactive  = (CKTDGUIStatic*) m_pDLGPvPLobby->GetControl( L"Static_TeamDeath_Inactive");
		CKTDGUIStatic* pStatic_Survival_Inactive		= (CKTDGUIStatic*) m_pDLGPvPLobby->GetControl( L"Static_Survival_Inactive");
		CKTDGUIStatic* pStatic_All_Inactive				= (CKTDGUIStatic*) m_pDLGPvPLobby->GetControl( L"Static_All_Inactive");


		switch( g_pMain->GetConnectedChannelID() )
		{
		case KPVPChannelInfo::PCC_PLAY:
			{
				// 연습채널 : 팀매치 버튼을 끄자
				pButton_TeamMatch->SetShowEnable( false, false );
				pRadioButton_TeamMatch->SetShowEnable( false, false );

				pButton_TeamDeathMatch->SetShowEnable( true, true );
				pRadioButton_TeamDeathMatch->SetShowEnable( true, true );

				// 해당 비활성 아이콘을 켜고 나머지는 다 끄자
				pStatic_TeamMatch_Inactive->SetShow(true);
				pStatic_TeamDeathMatch_Inactive->SetShow(false);
				pStatic_Survival_Inactive->SetShow(false);
				pStatic_All_Inactive->SetShow(false);

				//게임형식이 팀매치면 전체보기로 세트할 것
				// 연습채널에는 팀전이 없으므로 전체 보기를 default로
				if( m_iRoomListGameType == (int) CX2PVPRoom::PGT_TEAM )
				{
					if( NULL != pRadioButton_All )
					{
						pRadioButton_All->SetChecked( true );
						m_iRoomListGameType = -1;
					}
				}
			}
			break;


		case KPVPChannelInfo::PCC_TOURNAMENT:
			{
#ifdef TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
				// 대회채널 : 팀매치랑 데스매치 버튼을 끄자
				pButton_TeamMatch->SetShowEnable( false, false );
				pButton_TeamDeathMatch->SetShowEnable( true, true );
				pButton_Survival->SetShowEnable( false, false );

				pRadioButton_TeamMatch->SetShowEnable( false, false );
				pRadioButton_TeamDeathMatch->SetShowEnable( true, true );
				pRadioButton_Survival->SetShowEnable( false, false );

				// 해당 비활성 아이콘을 켜고 나머지는 다 끄자
				pStatic_TeamMatch_Inactive->SetShow(true);
				pStatic_TeamDeathMatch_Inactive->SetShow(false);
				pStatic_Survival_Inactive->SetShow(true);
				//pStatic_All_Inactive->SetShow(true);

				// 게임형식 탭을 서바이벌로 세트
				pRadioButton_TeamDeathMatch->SetChecked( true );
				m_iRoomListGameType = (int)CX2PVPRoom::PGT_TEAM_DEATH;
				m_bItemMode = false;
#endif TEMP_HERO_MATCH_PREVENT_GAME_TYPE_CHANGE
			} break;

#ifdef FIX_HERO_MATCH_UI
		case KPVPChannelInfo::PCC_E4:	/// 서버 채널 아이디가 아니라, 버튼에 있는 더미 인트 값으로 구분하고 있음;;;
			{
				// 대회채널 : 팀 데스 매치 파워전 빼고 다 숨기자
				pButton_TeamMatch->SetShowEnable( false, false );
				pButton_TeamDeathMatch->SetShowEnable( true, true );
				pButton_Survival->SetShowEnable( false, false );
				pButton_TeamMatch_Item->SetShowEnable( false, false );
				pButton_TeamDeathMatch_Item->SetShowEnable( false, false );

				pRadioButton_TeamMatch->SetShowEnable( false, false );
				pRadioButton_TeamDeathMatch->SetShowEnable( false, false );
				pRadioButton_Survival->SetShowEnable( false, false );

				// 게임형식 탭을 서바이벌로 세트
				pRadioButton_TeamDeathMatch->SetChecked( true );
				m_iRoomListGameType = (int)CX2PVPRoom::PGT_TEAM_DEATH;
				m_bItemMode = false;
			} break;
#endif FIX_HERO_MATCH_UI

		default:
			{
				pButton_TeamMatch->SetShowEnable( true, true );
				pButton_TeamDeathMatch->SetShowEnable( true, true );

				pRadioButton_TeamMatch->SetShowEnable( true, true );
				pRadioButton_TeamDeathMatch->SetShowEnable( true, true );

#ifdef FIX_HERO_MATCH_UI
				pButton_Survival->SetShowEnable( true, true );
				pButton_TeamMatch_Item->SetShowEnable( true, true );
				pButton_TeamDeathMatch_Item->SetShowEnable( true, true );

				pRadioButton_Survival->SetShowEnable( true, true );
#endif FIX_HERO_MATCH_UI

				// 비활성 다 끄자
				pStatic_TeamMatch_Inactive->SetShow(false);
				pStatic_TeamDeathMatch_Inactive->SetShow(false);
				pStatic_Survival_Inactive->SetShow(false);
				pStatic_All_Inactive->SetShow(false);

			}
			break;
		}
	}
}


// 
// void CX2StatePvPLobby::SetPingLevel( int iSimpleRoomIndex, int iLevel )
// {
// 	SimpleRoom* pSimpleRoom = m_SimpleRoomList[iSimpleRoomIndex];
// 	if( NULL == pSimpleRoom )
// 		return;
// 
// 	CKTDGUIDialogType pDialog = pSimpleRoom->pDialog;
// 	if( NULL == pDialog )
// 		return;
// 
// 	CKTDGUIStatic* pStatic_Ping = (CKTDGUIStatic*)pDialog->GetControl( L"PVP_Lobby_Room_Texture" );
// 	if( NULL == pStatic_Ping )
// 		return;
// 
// 	if( iLevel < 0 )
// 	{
// 		iLevel = 0;
// 	}
// 	else if( iLevel > 5 )
// 	{
// 		iLevel = 4; 
// 	}
// 
// 	for( int i=0; i<iLevel; i++ )
// 	{
// 		pStatic_Ping->GetPicture(i+1)->SetShow( true );
// 	}
// 
// 	for( int i=iLevel; i<5; i++ )
// 	{
// 		pStatic_Ping->GetPicture(i+1)->SetShow( false );
// 	}
// }


void CX2StatePvPLobby::OpenDialog()
{
	// 켤때
	// TODO : 채널 정하고, 게임방식 정하고, 룸리스트 받아오고.

#ifdef TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION
	// 특정아이템을 획득하고 나면 대회 채널에서 강제 퇴장
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		if( NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
		{
			UINT iTempChannelID = g_pMain->GetConnectedChannelID();

			std::vector<KPVPChannelInfo> channelList = g_pMain->GetChannelList();
			for ( int j = 0; j < (int)channelList.size(); j++ )
			{
				KPVPChannelInfo& channelInfo = channelList[j];
				if( KPVPChannelInfo::PCC_FREE == (KPVPChannelInfo::PVP_CHANNEL_CLASS) channelInfo.m_cPVPChannelClass )
				{
					iTempChannelID = channelInfo.m_iChannelID;
					break;
				}
			}

			Handler_EGS_CHANGE_PVP_ROOM_LIST_REQ( iTempChannelID );
			g_pMain->SetConnectedChannelID( iTempChannelID );
		}		
	}
#endif TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION
#ifdef TEMP_HERO_MATCH_ADMISSION_ITEM_POSSESSION
	// 특정 아이템을 가지고 있지 않으면 대회 채널 입장 못함
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		if( NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
		{
			UINT iTempChannelID = g_pMain->GetConnectedChannelID();

			std::vector<KPVPChannelInfo> channelList = g_pMain->GetChannelList();
			for ( int j = 0; j < (int)channelList.size(); j++ )
			{
				KPVPChannelInfo& channelInfo = channelList[j];
				if( KPVPChannelInfo::PCC_FREE == (KPVPChannelInfo::PVP_CHANNEL_CLASS) channelInfo.m_cPVPChannelClass )
				{
					iTempChannelID = channelInfo.m_iChannelID;
					break;
				}
			}

			Handler_EGS_CHANGE_PVP_ROOM_LIST_REQ( iTempChannelID );
			g_pMain->SetConnectedChannelID( iTempChannelID );
		}		
	}
#endif


	// 채널버튼 세팅하고 채널 하나 골라주고
	SettingChannelButton();
	SelectChannel( g_pMain->GetConnectedChannelID() );

	// 전체 탭 켜주고
	CKTDGUIRadioButton* pRadioButtonMode = (CKTDGUIRadioButton*)m_pDLGPvPLobby->GetControl(L"UI_PvP_Lobby_All_Tab"); 
	if ( pRadioButtonMode != NULL )
	{
		// 			if ( pRadioButtonMode->GetCheckedControl() != NULL )
		// 				pRadioButtonMode->GetCheckedControl()->SetChecked( false );
		pRadioButtonMode->SetChecked( true );
	}

	// 켜고
//	m_pDLGPvPLobby->SetShowEnable(true, true);
	g_pKTDXApp->SkipFrame();

	// 방리스트 받아오고
	m_bEnableResetDLGRoomList = true;
	ResetRoomListUI();
	Handler_EGS_ROOM_LIST_REQ( m_NowPage );

}




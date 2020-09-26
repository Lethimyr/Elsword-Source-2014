#include "StdAfx.h"
#include ".\x2statebattlefield.h"
//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
	KRenderState( D3DRS_ALPHATESTENABLE,	TRUE ),
	KRenderState( D3DRS_ALPHAFUNC,			D3DCMP_NOTEQUAL ),
	KRenderState( D3DRS_ALPHAREF,			0 ),

	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),

	KRenderState( D3DRS_ZENABLE,			FALSE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_CW ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//s_akStates[]
//}} robobeg : 2008-10-13

CX2StateBattleField::CX2StateBattleField()
	: m_pBattleFieldGame( NULL ),
	m_vOldEyePt( 0.0f, 0.0f, 0.0f ), m_vOldLookAtPt( 0.0f, 0.0f, 0.0f ),
	m_bNowMovingToOtherPlace( false ), m_TimerWaitingPortal( 3.0f ), m_RenderStateID( s_akStates ),
	m_TexDataMovingGageBG( L"DLG_LOADING_BAR.tga", L"LOADING_BG" ),
	m_TexDataMovingGage( L"DLG_LOADING_BAR.tga", L"LOADING_BAR" )
#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	, m_SumDelta( 0 )
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL
{
	g_pData->DeleteDungeonResultInfo();
	g_pData->DeletePVPResultInfo();
	
	CreateGame();
	
	GameLoadingStart();

	g_pKTDXApp->GetDGManager()->SetProjection( 
		g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(), true );

#ifdef CHARACTER_MOUSE_RBUTTON
	g_pData->GetUserListPopUp()->SetStage(this);
#endif CHARACTER_MOUSE_RBUTTON

	CreateMovingSmallBar();

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Start( true, true );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	//서버로부터 랙체크 활성화 여부를 알아낸다.
	CX2State::Handler_EGS_GET_ACTIVE_LAGCHECK_REQ();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    if ( g_pData->GetGameUDP() != NULL )
        g_pData->GetGameUDP()->RemoveAllPendingPingSends();
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
}

/*virtual*/ CX2StateBattleField::~CX2StateBattleField()
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );

	m_pBattleFieldGame->Release();
	SAFE_DELETE( m_pBattleFieldGame );
	
	DestroyMovingSmallBar();
}

/*virtual*/ HRESULT CX2StateBattleField::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );

	// 레벨업 처리 필요


// 	if ( m_pCashShop != NULL &&
// 		m_pCashShop->GetOpen() == false )
// 	{
		if ( g_pData->GetUIManager()->IsOpenedWindow() )
		{
			if( g_pData->GetUIManager()->GetUIQuestNew() != NULL )
				g_pData->GetUIManager()->GetUIQuestNew()->OnFrameMove( fTime, fElapsedTime );
		}
		else
		{
#ifdef RESTRICTION_PET_DIALOG_WITH_SYSTEM_MENU		/// 시스템 메뉴 켜져있을 땐, 마을처럼 펫 메뉴 열 수 없도록 수정
			if ( g_pKTDXApp->GetDIManager()->GetMouse()->GetPureButtonState( MOUSE_RBUTTON ) == TRUE &&
				 false == GetShowSystemMenu() )
#else  RESTRICTION_PET_DIALOG_WITH_SYSTEM_MENU
			if ( g_pKTDXApp->GetDIManager()->GetMouse()->GetPureButtonState( MOUSE_RBUTTON ) == TRUE )
#endif RESTRICTION_PET_DIALOG_WITH_SYSTEM_MENU
			{
				UserAndPetListPopUpMenuProcess();
			}
		}
	
		// Game OnFrameMove
		m_pBattleFieldGame->OnFrameMove( fTime, fElapsedTime );
		// Room OnFrameMove
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		MoveToOtherPlace( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		MoveToOtherPlace();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	
		g_pData->GetBattleFieldRoom()->OnFrameMove( fTime, fElapsedTime );	// FieldFix: 많이 뜯어 고쳐야 할 듯...
//	}

		//{{ kimhc // 2011-04-25 // 디펜스 던전, Time UI
#ifdef SERV_INSERT_GLOBAL_SERVER
		//	월드미션 관련 창 켜기
		if( NULL == g_pData->GetCashShop() || false == g_pData->GetCashShop()->GetOpen() )
		{
			if( g_pMain->IsInheritStateMenu() )
			{
				CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>(g_pMain->GetNowState());
				if( NULL != pStateMenu && 
					true == pStateMenu->GetShowUI() )
				{
					if( g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() == true )
					{ 
						g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( true );
					}
				}
			}
		}
#endif SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-25 // 디펜스 던전, Time UI


#ifdef MONSTER_STATE_LIST_TEST
		m_MonsterStateListDialog.KeyProcess( this );
#endif // MONSTER_STATE_LIST_TEST
	return S_OK;
}

/*virtual*/ HRESULT CX2StateBattleField::OnFrameRender()
{
	KTDXPROFILE();

// 	if ( NULL != m_pCashShop &&	
// 		!m_pCashShop->GetOpen() &&	// 캐시샵이 열려있지 않으면
// 		m_pBattleFieldGame != NULL )
// 	{
		m_pBattleFieldGame->OnFrameRender();

		if ( m_TimerWaitingPortal.GetSumOfElapsedTime() > 0.0f )
		{
			DrawMovingSmallBar();
		}
// 	}
// 	else
// 	{
// 		g_pKTDXApp->GetDGManager()->ObjectChainSort();
//		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
//		g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
// 		g_pKTDXApp->GetDGManager()->FrontUIRender();		
// 	}
	
	if ( g_pMain->GetTextManager() != NULL )
		g_pMain->GetTextManager()->OnFrameRender();

	if ( g_pMain->GetGameEdit() != NULL )
		g_pMain->GetGameEdit()->OnFrameRender();

	if( m_pCursor != NULL)
		m_pCursor->OnFrameRender();

	if( g_pInstanceData->GetMiniMapUI() != NULL )
	{
		g_pInstanceData->GetMiniMapUI()->UpdateEventNotice();
#ifdef EVENT_CARNIVAL_DECORATION
		g_pInstanceData->GetMiniMapUI()->UpdateCarnivalDeco();
#endif //EVENT_CARNIVAL_DECORATION
	}
#ifdef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE
	ProcessFadeInOutNotice();		/// 페이드 인 ~ 페이드 아웃 되는 공지 처리
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef FIELD_BOSS_RAID // FadeInOutNotice2nd 추가
	CX2BossRaidManager::GetInstance()->OnFrameRender();
#endif // FIELD_BOSS_RAID

	return S_OK;
}

/*virtual*/ HRESULT CX2StateBattleField::OnResetDevice()
{
 	if( m_pBattleFieldGame != NULL )
 		m_pBattleFieldGame->OnResetDevice();
	
	return S_OK;
}

/*virtual*/ HRESULT CX2StateBattleField::OnLostDevice()
{
 	if( m_pBattleFieldGame != NULL )
 		m_pBattleFieldGame->OnLostDevice();
	
	return S_OK;
}

/*virtual*/ bool CX2StateBattleField::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_NEW) )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->MsgProc(hWnd, uMsg, wParam, lParam );
	}

	if ( CX2StateMenu::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if( NULL != g_pX2Game )
	{
		if( true == g_pX2Game->MsgProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	switch ( uMsg )
	{	
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		} break;
	}
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL
	return false;
}

/*virtual*/ bool CX2StateBattleField::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	// 김상훈
#ifdef CHARACTER_MOUSE_RBUTTON
	if ( g_pData->GetUserListPopUp() != NULL && g_pData->GetUserListPopUp()->GetShowMenu() == true )
		g_pData->GetUserListPopUp()->UICustomEventProc( hWnd, uMsg, wParam , lParam );
#endif 
	// 김상훈

	if( g_pData->GetUIManager()->GetUIQuestNew() != NULL && 
		g_pData->GetUIManager()->GetUIQuestNew()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true )
		return true;

#ifdef KEYFRAME_CAMERA
	if(g_pX2Game != NULL &&
		g_pX2Game->GetX2Camera() != NULL &&
		g_pX2Game->GetWorldCameraEdit() == true)
	{
		if(g_pX2Game->GetX2Camera()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true )
			return true;
	}
#endif KEYFRAME_CAMERA

#ifdef MONSTER_STATE_LIST_TEST
	switch( wParam )
	{
	case CX2StateDungeonGame::DGUCM_SUMMON_SELECTED_MONSTER_DEBUG:
		{

			CKTDGUIListBox* pListBox = (CKTDGUIListBox*) lParam;
			if( NULL == pListBox )
				return false;

			CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBox->GetSelectedItem();
			if( NULL == pListBoxItem )
				return false; 

			if( pListBox->GetDialog() == m_MonsterStateListDialog.GetDialog() )
			{
				m_MonsterStateListDialog.OnCommand( *((const std::string*)(pListBoxItem->pData)) );
			}

			return true;
		} break;
	}
#endif // MONSTER_STATE_LIST_TEST

	return false;
}

/*virtual*/ bool CX2StateBattleField::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

	if ( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;		

	switch ( wParam )
	{
	case EGS_GAME_LOADING_ACK:
		return Handler_EGS_GAME_LOADING_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GAME_LOADING_NOT:
		return Handler_EGS_GAME_LOADING_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_PLAY_START_NOT:
		return Handler_EGS_PLAY_START_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_FIELD_ACK:
		return Handler_EGS_STATE_CHANGE_FIELD_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_JOIN_BATTLE_FIELD_NOT:
		return Handler_EGS_JOIN_BATTLE_FIELD_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_LEAVE_BATTLE_FIELD_NOT:
		return Handler_EGS_LEAVE_BATTLE_FIELD_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_BATTLE_FIELD_NPC_LOAD_NOT:
		{
			const bool bResult = Handler_EGS_BATTLE_FIELD_NPC_LOAD_NOT( hWnd, uMsg, wParam, lParam );

// 			if ( true == bResult )
// 				Handler_EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ();

			return bResult;
		} break;

	case EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK:
		return Handler_EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT:
		return Handler_EGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_NPC_UNIT_CREATE_ACK:
		return Handler_EGS_NPC_UNIT_CREATE_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_NPC_UNIT_CREATE_NOT:
		return Handler_EGS_NPC_UNIT_CREATE_NOT( hWnd, uMsg, wParam, lParam );
		break;

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	case EGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT :		// 보스 유닛 생성 패킷
		return Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_NOT ( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK :
		return Handler_EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK ( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT :
		return Handler_EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT( hWnd, uMsg, wParam, lParam );
		break;
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

	case EGS_DROP_ITEM_NOT:
		return Handler_EGS_DROP_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GET_ITEM_NOT:
		return Handler_EGS_GET_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GET_ITEM_REALTIME_NOT:
			return Handler_EGS_GET_ITEM_REALTIME_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DECREASE_ENDURANCE_NOT:
			return Handler_EGS_DECREASE_ENDURANCE_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_NPC_UNIT_DIE_ACK:
		{
			if( NULL != g_pX2Game )
				return g_pX2Game->Handler_EGS_NPC_UNIT_DIE_ACK( hWnd, uMsg, wParam, lParam );
			else
				return true;
		} break;

	case EGS_NPC_UNIT_DIE_NOT:
		{
			if( NULL != g_pX2Game )
				return g_pX2Game->Handler_EGS_NPC_UNIT_DIE_NOT( hWnd, uMsg, wParam, lParam );
			else
				return true;
		} break;

	case EGS_ADD_ON_STAT_ACK:
		return Handler_EGS_ADD_ON_STAT_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_ADD_ON_STAT_NOT:
		return Handler_EGS_ADD_ON_STAT_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHAR_LEVEL_UP_NOT:
		return Handler_EGS_CHAR_LEVEL_UP_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT:
		return Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT:
		return Hander_EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_BATTLE_FIELD_EVENT_MESSAGE_NOT:
		return Handler_EGS_BATTLE_FIELD_EVENT_MESSAGE_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DECREASE_SPIRIT_NOT:
		return Handler_EGS_DECREASE_SPIRIT_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT:
		return Handler_EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT:
		return Handler_EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_UPDATE_UNIT_INFO_NOT:
		return Handler_EGS_UPDATE_UNIT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_VISIT_CASH_SHOP_NOT:
		return Handler_EGS_VISIT_CASH_SHOP_NOT( hWnd, uMsg, wParam, lParam );
		break;

#ifdef SERV_CHECK_EXIST_MONSTER_UID
	case EGS_CHECK_EXIST_MONSTER_UID_ACK:
		return Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK( hWnd, uMsg, wParam, lParam );
		break;
#endif // SERV_CHECK_EXIST_MONSTER_UID

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case EGS_USE_FINALITY_SKILL_ACK:
		{
			if ( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_ACK( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;
#endif //FINALITY_SKILL_SYSTEM

// 	case EGS_DUNGEON_INTRUDE_SYNC_ACK:
// 		return Handler_EGS_DUNGEON_INTRUDE_SYNC_ACK( hWnd, uMsg, wParam, lParam );
// 		break;
// 
// 	case EGS_DUNGEON_INTRUDE_SYNC_NOT:
// 		return Handler_EGS_DUNGEON_INTRUDE_SYNC_NOT( hWnd, uMsg, wParam, lParam );
// 		break;
	}
#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif

	return false;
}

/*virtual*/ bool CX2StateBattleField::UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	m_pBattleFieldGame->MsgProc( hWnd, uMsg, wParam, lParam );

	return false;
}

/*virtual*/ bool CX2StateBattleField::ShortCutKeyProcess()
{
	bool bHideDialog = ProcessGameScoreAndPostEffect();	

	if ( CX2StateMenu::ShortCutKeyProcess() == true )
		return true;

	if ( true == g_pData->GetCashShop()->GetOpen() )
		return true;

	// 퀘스트 창
	if ( bHideDialog == false && GET_KEY_STATE( GA_QUEST ) == TRUE )
	{
		//g_pMain->GetNewQuestUI()->SetOpenQuestPopUpWindow( !g_pMain->GetNewQuestUI()->GetOpenQuestPopUpWindow() );
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUEST);

		return true;
	}

	// 캐릭터 정보창
	if ( bHideDialog == false && GET_KEY_STATE( GA_INFO ) == TRUE )
	{
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_CHARINFO);

		return true;
	}

	// 인벤토리
	if ( bHideDialog == false && GET_KEY_STATE( GA_INVENTORY ) == TRUE )
	{
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN);

		return true;
	}

	// 펫
	if ( bHideDialog == false && GET_KEY_STATE( GA_PET ) == TRUE )
	{
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PET_LIST);

		return true;
	}

	// 커뮤니티(친구탭)
	if ( bHideDialog == false && GET_KEY_STATE( GA_FRIEND ) == TRUE )
	{
		g_pData->GetMessenger()->SetFriendTab(true);
		g_pData->GetMessenger()->SetOpen( !g_pData->GetMessenger()->GetOpen() );

		return true;
	}

	// 커뮤니티(길드탭)
	if ( bHideDialog == false && GET_KEY_STATE( GA_GUILD ) == TRUE )
	{			
		g_pData->GetMessenger()->SetTabByShortCutKey( CX2Community::XMUT_GUILD );
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_COMMUNITY);
		return true;
	}

	if ( bHideDialog == false && GET_KEY_STATE( GA_NAME ) == TRUE )
	{
		g_pX2Game->SetRenderNPCName( !g_pX2Game->GetRenderNPCName() );
		return true;
	}

#ifdef SERV_ADD_WARP_BUTTON
	// 워프
	if ( bHideDialog == false && GET_KEY_STATE( GA_WARP ) == TRUE )
	{
		if( NULL != g_pData->GetMyUser() 
			&& NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
			if( pStateMenu != NULL )
			{
				if( pStateMenu->GetShowWarpDest() == false )
				{
					if( true == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_bWarpVip )
						pStateMenu->ClickWarpButton( false );
					else
						pStateMenu->ClickWarpButton( true );
				}
				else
				{
					// 꺼주자
					pStateMenu->SetShowWarpDest( false );
				}
			}
		}

		return true;
	}
#endif // SERV_ADD_WARP_BUTTON

	if ( false == bHideDialog && g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB) == TRUE &&
		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LALT) != TRUE )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->ToggleUserShowQuickQuestDLG();
	}

	if ( CameraZoomInAndOutKeyProcess() )
		return true;

	if ( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_SIT ) == TRUE )
	{
		CX2GUUser* pMyUnit = m_pBattleFieldGame->GetMyUnit();
		if ( NULL != pMyUnit )
		{
			if( pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
				return true;

#ifdef RIDING_SYSTEM
			if ( pMyUnit->GetNowStateID() != pMyUnit->GetWaitStateID() && pMyUnit->GetNowStateID() != pMyUnit->GetRidingWaitStateID() )
				return true;

			if( pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITREADY )
				return true;

			g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2501 ) );
#else //RIDING_SYSTEM
			wstring wstrName = L"Emotion_SitReady";
			pMyUnit->PlayEmotion( CX2Unit::ET_SITREADY, wstrName );
#endif //RIDING_SYSTEM
		}
	}

	return false;
}

void CX2StateBattleField::CreateGame()
{
	m_pBattleFieldGame = new CX2BattleFieldGame();
}

void CX2StateBattleField::GameLoadingStart()
{
	m_pBattleFieldGame->GameLoading( g_pData->GetBattleFieldRoom() );
	
	Handler_EGS_GAME_LOADING_REQ( 100 );
#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG
	if ( NULL != g_pInstanceData )
	{
		g_pInstanceData->SetProcessTerm( 
			static_cast<const char>( g_pInstanceData->GetElapsedTimerFromJoinBattleToGameLoading() ) );
	}
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

    if ( g_pData->GetGameUDP() != NULL )
        g_pData->GetGameUDP()->ResetConnectTestToPeersAll();

#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    if ( g_pData->GetBattleFieldRoom() != NULL )
        g_pData->GetBattleFieldRoom()->ResetSlotPingSendsAll();
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

}

bool CX2StateBattleField::Handler_EGS_GAME_LOADING_REQ( const int iLoadingProgressPercent_ )
{
	KEGS_GAME_LOADING_REQ kPacket;

	kPacket.m_iLoadingProgress = iLoadingProgressPercent_;

	g_pData->GetServerProtocol()->SendPacket( EGS_GAME_LOADING_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GAME_LOADING_ACK, 60.f );

	m_pBattleFieldGame->GameStart();
	return true;
}

bool CX2StateBattleField::Handler_EGS_GAME_LOADING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_GAME_LOADING_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{
			//m_pBattleFieldGame->GameStart();
			g_pKTDXApp->SkipFrame();
#ifdef REFORM_NOVICE_GUIDE
			if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
			{
				g_pData->GetPlayGuide()->ResetNoviceGuide();
			}
#endif //REFORM_NOVICE_GUIDE
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
			{
				g_pData->GetQuestManager()->CompleteQuestToVisitVillage();
			}
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
#ifdef QUEST_GUIDE
			if( NULL != g_pX2Game && NULL != static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator() )
			{
				static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator()->ResetGuideTargetInfo();
			}
#endif //QUEST_GUIDE
			if( NULL != g_pData && NULL != g_pData->GetUIManager() && 
				NULL != g_pData->GetUIManager()->GetUIQuestNew() )
				g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuestMonster();


#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD			
			if( NULL != g_pData && NULL != g_pData->GetCashShop() && 
				NULL != g_pX2Game )
			{
				std::vector< UidType > vecEnterCashShopUser;

				g_pData->GetCashShop()->GetEnterCashShopUserList(vecEnterCashShopUser);
				g_pData->GetCashShop()->ClearEnterCashShopUserList();
				BOOST_TEST_FOREACH( UidType, EnterCashShopUser, vecEnterCashShopUser )
				{
					CX2GUUser* pGUUser= g_pX2Game->GetUserUnitByUID( EnterCashShopUser );
					if ( NULL != pGUUser )
						pGUUser->SetEnterCashShop( true );
				}
			}
#endif //SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
			CX2State * pNowState = static_cast<CX2State *> ( g_pMain->GetNowState() );
			if ( NULL != pNowState )
			{
				if ( true == g_pMain->GetIsJumpingCharacter() )
				{
					// 전직 선택 알림창을 띄운다.
					pNowState->SetShowJumpingCharacterClassNoticeChangeDlg( true );
				}
			}
#endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터

#ifdef SERV_BATTLE_FIELD_BUFF		/// 필드 중간 보스 런칭 이벤트 - 필드 입장시 페이드 효과로 표시되는 필드 중간 보스 엠블럼
			if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
			{
				CKTDGParticleSystem::CParticleEventSequenceHandle m_Seq = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"UI_Field_Defence_StartEffect", 
					512, 284+60, 0, 9999, 9999, -1, 1, -1.0f, true, 1.2f, false );

				if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_Seq )
				{
					CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_Seq );
					if( pParticle != NULL )
					{
						pParticle->SetOverUI( true );
					}
				}
			}
#endif SERV_BATTLE_FIELD_BUFF
		}
	}
	return false;
}

bool CX2StateBattleField::Handler_EGS_GAME_LOADING_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GAME_LOADING_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
    if ( kPacket.m_iLoadingProgress >= 100 )
    {
        if ( g_pData->GetGameUDP() != NULL )
            g_pData->GetGameUDP()->ResetConnectTestToPeer( kPacket.m_iUnitUID );
    }//if
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	
	return true;
}

bool CX2StateBattleField::Handler_EGS_PLAY_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PLAY_START_NOT kPacket;
	DeSerialize( pBuff, &kPacket );
	
	return true;
}

/*virtual*/ bool CX2StateBattleField::Handler_EGS_JOIN_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 기존에 필드에 있는 사람만 받을 수 있음
	// 현재 들어오고 있는 사람은 X2StateBattleField 객체가 아직 생성되지 않아서 처리를 못하는듯...
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_BATTLE_FIELD_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	
	return m_pBattleFieldGame->Handler_EGS_JOIN_BATTLE_FIELD_NOT( kPacket );
}

bool CX2StateBattleField::Handler_EGS_LEAVE_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_BATTLE_FIELD_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	return m_pBattleFieldGame->Handler_EGS_LEAVE_BATTLE_FIELD_NOT( kPacket );
}

/** @function : Handler_EGS_NPC_UNIT_CREATE_NOT
	@brief : NPC_UNIT_CREATE_REQ 에 의해서 생성되는 몬스터 또는 몬스터 리스폰 시에 사용되는 패킷 핸들러
	@param : HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
	@return : bool
*/

bool CX2StateBattleField::Handler_EGS_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return m_pBattleFieldGame->Handler_EGS_NPC_UNIT_CREATE_ACK( hWnd, uMsg, wParam, lParam );
}

#ifdef FINALITY_SKILL_SYSTEM //JHKang
bool CX2StateBattleField::Handler_EGS_USE_FINALITY_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return m_pBattleFieldGame->Handler_EGS_USE_FINALITY_SKILL_ACK( hWnd, uMsg, wParam, lParam );
}
#endif //FINALITY_SKILL_SYSTEM



#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS

bool CX2StateBattleField::Handler_EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
// 대미지 맵 전달 ACK
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;	
}

bool CX2StateBattleField::Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return m_pBattleFieldGame->Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_ACK( hWnd, uMsg, wParam, lParam );
}

bool CX2StateBattleField::Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent.m_kCreatePacket );
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	bool bIsMainBossAlive = false;
	for ( int i = 0; i < static_cast <int> ( kEvent.m_kCreatePacket.m_vecNPCUnitAck.size() ); i++ )
	{
		KNPCUnitNot* pKNPCUnitNot = &kEvent.m_kCreatePacket.m_vecNPCUnitAck[i];
		if ( pKNPCUnitNot->m_kNPCUnitReq.m_bFocusCamera == true )
			bIsMainBossAlive = true;
	}

	if ( false == bIsMainBossAlive )
	{
		if ( static_cast <int> ( kEvent.m_kCreatePacket.m_vecNPCUnitAck.size() ) > 0 )
		{
			KNPCUnitNot* pKNPCUnitNot = &kEvent.m_kCreatePacket.m_vecNPCUnitAck[0];
			if( NULL != pKNPCUnitNot )
			{
				pKNPCUnitNot->m_kNPCUnitReq.m_bFocusCamera = true;
			}
		}
	}

	m_pBattleFieldGame->CreateBattleFieldNpcByNotData( kEvent.m_kCreatePacket.m_vecNPCUnitAck, kEvent.m_kCreatePacket.m_mapAttribNpcInfo );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	m_pBattleFieldGame->SetKAttribEnchantNpcInfoMap( kEvent.m_mapAttribNpcInfo );
	m_pBattleFieldGame->CreateBattleFieldNpcByNotData( kEvent.m_vecNPCUnitAck );
	m_pBattleFieldGame->ClearKAttribEnchantNpcInfoMap();
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

	
	return true;
}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS


bool CX2StateBattleField::Handler_EGS_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NPC_UNIT_CREATE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	m_pBattleFieldGame->CreateBattleFieldNpcByNotData( kEvent.m_vecNPCUnitAck, kEvent.m_mapAttribNpcInfo );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD


	m_pBattleFieldGame->SetKAttribEnchantNpcInfoMap( kEvent.m_mapAttribNpcInfo );
	m_pBattleFieldGame->CreateBattleFieldNpcByNotData( kEvent.m_vecNPCUnitAck );
	m_pBattleFieldGame->ClearKAttribEnchantNpcInfoMap();
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD


	return true;
}

/*virtual*/ bool CX2StateBattleField::Handler_EGS_STATE_CHANGE_FIELD_REQ( const UINT uiVillageId_, const USHORT usBattleFieldStartLineIndex_ )
{
	KEGS_STATE_CHANGE_FIELD_REQ kPacket;

	int iMapId = g_pData->GetLocationManager()->GetMapId( usBattleFieldStartLineIndex_ );

	if ( iMapId <= 0 )
	{
		iMapId = SEnum::VMI_RUBEN;
		ASSERT( !L"uiVillageId_ is wrong!" );
		StateLog( L"uiVillageId_ is wrong!" );
	}

	kPacket.m_iMapID =  iMapId;

	const D3DXVECTOR3 vStartPos = g_pData->GetLocationManager()->GetStartPosLoc( usBattleFieldStartLineIndex_ );

	ASSERT( vStartPos != D3DXVECTOR3( 0, 0, 0 ) );

	g_pData->GetLocationManager()->SetVillage( (SEnum::VILLAGE_MAP_ID)kPacket.m_iMapID, vStartPos );

	const bool bIsRight = g_pData->GetLocationManager()->GetIsRight(usBattleFieldStartLineIndex_);
	g_pData->GetLocationManager()->SetVillageIsRight(bIsRight);

	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_FIELD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_FIELD_ACK );

	return true;
}

/*virtual*/
void CX2StateBattleField::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	if( NULL == g_pData->GetBattleFieldRoom() )
		return; 

	CX2Room::SlotData* pkSlotData = g_pData->GetBattleFieldRoom()->GetSlotDataByUnitUID( iUnitUID_ );
	if ( pkSlotData == NULL )
		return;

	int iSlotIndex = pkSlotData->m_Index;
	if( iSlotIndex < 0 )
		return;	

	if( NULL != m_pBattleFieldGame )
	{
		CX2GUUser* pGUUser = m_pBattleFieldGame->GetUserUnitByUID( iUnitUID_ );
		if( NULL != pGUUser )
		{
#ifdef UNIT_EMOTION
			bool bCommandEmotion = g_pChatBox->IsEmotionID(pWstrMsg_);

			if( g_pChatBox != NULL && bCommandEmotion == false )
#endif
			{
                if( pGUUser->GetBoundingRadius() > 0 )
                {
				    D3DXVECTOR3 center;
				    pGUUser->GetTransformCenter( &center );
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
                    float   fScaledBoundingRadius =pGUUser->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
				    //컬링
				    float fScale;
				    if( pGUUser->GetMatrix().GetXScale() > pGUUser->GetMatrix().GetYScale() )
				    {
					    if( pGUUser->GetMatrix().GetXScale() > pGUUser->GetMatrix().GetZScale() )
					    {
						    //X가 제일 큼
						    fScale = pGUUser->GetMatrix().GetXScale();
					    }
					    else
					    {
						    //Z가 제일 큼
						    fScale = pGUUser->GetMatrix().GetZScale();
					    }
				    }
				    else
				    {
					    if( pGUUser->GetMatrix().GetYScale() > pGUUser->GetMatrix().GetZScale() )
					    {
						    //Y가 제일 큼
						    fScale = pGUUser->GetMatrix().GetYScale();
					    }
					    else
					    {
						    //Z가 제일 큼
						    fScale = pGUUser->GetMatrix().GetZScale();
					    }
				    }
                    float   fScaledBoundingRadius = pGUUser->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

				    if( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, fScaledBoundingRadius ) == false )
					    return;
                }


				CX2TalkBoxManagerImp::TalkBox talkBox;
				talkBox.m_OwnerUnitUID		= iUnitUID_;

				talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;

				talkBox.m_wstrTalkContent	= pWstrMsg_;
				talkBox.m_bTraceUnit		= true;
				talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
				talkBox.m_coTextColor		= coTextColor_;
				talkBox.m_BackTexColor		= coBackColor_;

				talkBox.m_bUseOutLine		 = true;
				if( coOutColor_ == 0x00000000 )	//	입력 없을 경우는 글씨색 그대로 쓰기
				{
					talkBox.m_bUseOutLine = false;
				}

				g_pX2Game->GetTalkBoxManager()->Push( talkBox );
			}


#ifdef UNIT_EMOTION
			// 이모션 연결시켜준다.
			if(g_pChatBox != NULL)
			{
				// 앉은 상태에서는 채팅필터링 먹히지 않도록 한다.(명령어로만 이모션동작취함)
				if( bCommandEmotion == false && pGUUser->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
					return;

				CX2Unit::EMOTION_TYPE eId = CX2Unit::ET_NONE;
				wstring wstrName = L"";
				g_pChatBox->GetEmotionIdAndName(eId, wstrName, pWstrMsg_);
#ifdef RIDING_SYSTEM
				if ( eId == CX2Unit::ET_SITREADY && true == pGUUser->GetRidingOn() && NULL != pGUUser->GetRidingPet() )
				{
					wstrName = pGUUser->GetRidingMotionName();
					pGUUser->GetRidingPet()->StateChange( pGUUser->GetRidingPet()->m_SitReadyState );
				}
#endif //RIDING_SYSTEM
				pGUUser->PlayEmotion( eId, wstrName );
			}
#endif

		}
	}

}

/*
void CX2StateBattleField::Handler_EGS_DUNGEON_INTRUDE_SYNC_REQ() const
{
	g_pData->GetServerProtocol()->SendID( EGS_DUNGEON_INTRUDE_SYNC_REQ );
	g_pMain->AddServerPacket( EGS_DUNGEON_INTRUDE_SYNC_ACK );
}

bool CX2StateBattleField::Handler_EGS_DUNGEON_INTRUDE_SYNC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_INTRUDE_SYNC_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_DUNGEON_INTRUDE_SYNC_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
			return true;			
	}

	ASSERT( !L"EGS_DUNGEON_INTRUDE_SYNC_ACK is Error" );
	return false;
}

bool CX2StateBattleField::Handler_EGS_DUNGEON_INTRUDE_SYNC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_INTRUDE_SYNC_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != m_pBattleFieldGame )
	{
		m_pBattleFieldGame->CreateNpcFromDungeonIntrudeSyncPacket( kEvent );
		return true;
	}
	else
	{
		ASSERT( !L"m_pBattleFieldGame is NULL" );
		return false;
	}
}
*/

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
void CX2StateBattleField::FlushSendFrameAverage()
{
	if ( NULL != m_pBattleFieldGame )
	{
        m_pBattleFieldGame->FlushSendFrameAverage();
    }
}
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

void CX2StateBattleField::MoveFromBattleFieldToOtherBattleField()
{
	if ( NULL != m_pBattleFieldGame )
	{
		m_pBattleFieldGame->ClearNpcCreatedInfoList();
		m_pBattleFieldGame->DeleteAllNPCUnit();
		m_pBattleFieldGame->SwapWorld();
		m_pBattleFieldGame->UnitLoading();
		m_pBattleFieldGame->EtcLoading();
		m_pBattleFieldGame->Send_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT();
		static_cast<CX2State*>( g_pMain->GetNowState() )->Send_EGS_UPDATE_PLAY_STATUS_NOT();
		SetNowMovingToOtherPlace( false );
		Handler_EGS_GAME_LOADING_REQ( 100 );

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
		if ( g_pKTDXApp->GetDeviceManager() != NULL )
			g_pKTDXApp->GetDeviceManager()->ReleaseAllMemoryBuffers();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

	    DXUTResetStatsUpdateFrame();

#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_pBattleFieldGame->AccessGameStatistics().Init();

		// 프레임 통계 초기화이후, 패킷 전송 여부도 초기화
		m_pBattleFieldGame->SetAlreadySendingFrame(false);

		// 필드 진입 10초 후 부터 프레임 정보 모으도록 초기화
		m_pBattleFieldGame->ResetTimerForSendingPositionInfo();
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX


	}
#ifdef ADD_RAID_FIELD_LOG
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"필드를 이동했으나 월드가 이동되지 않는 문제 발생.. 게임 포인터 오류", g_pMain->GetNowState() );
	}
#endif // ADD_RAID_FIELD_LOG


}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
FORCEINLINE void CX2StateBattleField::MoveToOtherPlace( float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
FORCEINLINE void CX2StateBattleField::MoveToOtherPlace()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( true == m_pBattleFieldGame->IsNearPortalLineMap() && 
		false == GetNowMovingToOtherPlace() )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_TimerWaitingPortal.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_TimerWaitingPortal.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		if ( m_TimerWaitingPortal.CheckAndResetElapsedTime() )
		{
			// 단축키 사용 못하는 경우, 포탈 사용 못하도록 설정.
			// 던전 시작 대기상태에서 입장하는 것을 막으려고 설정 했음.
			if( false == GetEnableShortCutKey() )
			{
				m_TimerWaitingPortal.ResetSumOfElapsedTime();
				return;
			}

#ifdef FIELD_BOSS_RAID 
			if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
			{	
				// 보스 필드에서는 이전 필드로만 이동
				MoveToBeforePlaceAtRaidField();
			}
			else
#endif // FIELD_BOSS_RAID
			{
				const int iLastTouchedLineMapIndex = m_pBattleFieldGame->GetMyGUUserLastTouchedLineMapIndex( true );
				CBattleFieldData::CBattleFieldPortalMovingInfoPtr pBattleFieldPortalMovingInfoPtr = g_pData->GetBattleFieldManager().GetPortalMovingInfoByLineMapIndexInNowBattleField( iLastTouchedLineMapIndex );

				if ( NULL != pBattleFieldPortalMovingInfoPtr )
				{
					const CX2BattleFieldManager::PORTAL_MOVE_TYPE ePortalMoveType = static_cast<const CX2BattleFieldManager::PORTAL_MOVE_TYPE>( pBattleFieldPortalMovingInfoPtr->GetPortalMoveType() );

					switch ( ePortalMoveType )
					{
					case CX2BattleFieldManager::PMT_MOVE_TO_BATTLE_FIELD:
						{
							Handler_EGS_JOIN_BATTLE_FIELD_REQ( pBattleFieldPortalMovingInfoPtr->GetPlaceIdToMove(), pBattleFieldPortalMovingInfoPtr->GetPositionIndexToMove() );
							SetNowMovingToOtherPlace( true );
						} break;

					case CX2BattleFieldManager::PMT_MOVE_TO_VILLAGE:
						{
							Handler_EGS_STATE_CHANGE_FIELD_REQ( pBattleFieldPortalMovingInfoPtr->GetPlaceIdToMove(), pBattleFieldPortalMovingInfoPtr->GetPositionIndexToMove() );
							SetNowMovingToOtherPlace( true );
						} break;
#ifdef FIELD_BOSS_RAID
					case CX2BattleFieldManager::PMT_MOVE_TO_RAID_FIELD:
						{
							if( true == CX2BossRaidManager::GetInstance()->IsActiveRaidPortal() )
							{
								Handler_EGS_JOIN_BATTLE_FIELD_REQ( CX2BossRaidManager::GetInstance()->GetBossRaidMapID(), pBattleFieldPortalMovingInfoPtr->GetPositionIndexToMove() );
								SetNowMovingToOtherPlace( true );
							}
						} break;
#endif // FIELD_BOSS_RAID
					default:
						{
							ASSERT( !L"BattleField PortalMoveType is worng!" );
							StateLog( L"BattleField PortalMoveType is worng!" );
						} break;
					}
				}
				else
				{
					ASSERT( !L"CBattleFieldPortalMovingInfoPtr is NULL!" );
					StateLog( L"CBattleFieldPortalMovingInfoPtr is NULL!" );
				}
			}
		}		
	}
	else
		m_TimerWaitingPortal.ResetSumOfElapsedTime();
}

bool CX2StateBattleField::Handler_EGS_BATTLE_FIELD_NPC_LOAD_NOT( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BATTLE_FIELD_NPC_LOAD_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	if ( NULL == m_pBattleFieldGame )
	{
		ASSERT( NULL != m_pBattleFieldGame );
		StateLog( L"m_pBattleFieldGame is NULL" );
		return false;
	}

#ifndef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	m_pBattleFieldGame->SetKAttribEnchantNpcInfoMap( kPacket.m_mapAttribNpcInfo );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

	ASSERT( NULL != m_pBattleFieldGame->GetMyUnit() );
	if ( NULL != m_pBattleFieldGame->GetMyUnit() )
	{
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		m_pBattleFieldGame->CreateNpcCreatedInfo( kPacket.m_vecNPCData, kPacket.m_mapAttribNpcInfo, true );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		m_pBattleFieldGame->CreateNpcCreatedInfo( kPacket.m_vecNPCData );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	}
	else
	{
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		m_pBattleFieldGame->CreateBattleFieldNpcAtStart( kPacket.m_vecNPCData, kPacket.m_mapAttribNpcInfo );
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		m_pBattleFieldGame->CreateBattleFieldNpcAtStart( kPacket.m_vecNPCData );
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	}

	return true;
}

void CX2StateBattleField::Handler_EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ()
{
#ifdef FIELD_BOSS_RAID
	//if( true == CX2BossRaidManager::GetInstance()->IsValideNpcLoadReq() )
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() &&
		true == g_pMain->IsWaitingServerPacket( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK) )
	{
		return;
	}
#endif // FIELD_BOSS_RAID

	g_pData->GetServerProtocol()->SendID( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ );
	g_pMain->AddServerPacket( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK );

#ifdef FIELD_BOSS_RAID
	//CX2BossRaidManager::GetInstance()->SetSendNpcLoadReq(true);	
#endif // FIELD_BOSS_RAID
}

bool CX2StateBattleField::Handler_EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{
#ifdef SERV_CHECK_EXIST_MONSTER_UID
			if ( NULL != m_pBattleFieldGame )	/// 정해진 시간 후에 싱크 패킷을 받았는지 체크하도록 함
				m_pBattleFieldGame->StartElapsedTimeSinceGettingFirstNpcSyncPacket();
#endif // SERV_CHECK_EXIST_MONSTER_UID
			return true;
		}
	}

	return false;	
}

bool CX2StateBattleField::Handler_EGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	if ( NULL != m_pBattleFieldGame )
	{
		KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT kPacketNpcUnitBuffInfo;

		/// 새로 들어온 유저에게 NPC 첫 NPC sync 패킷을 보내고, NPC들의 버프 정보를 얻어온다.
		m_pBattleFieldGame->SendNpcUnitFirstSyncPacketImmediateForce( kPacket.m_vecNonNpcSyncUserList, kPacketNpcUnitBuffInfo );

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

        kPacketNpcUnitBuffInfo.m_vecToUnitUID = kPacket.m_vecNonNpcSyncUserList;
        g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT, kPacketNpcUnitBuffInfo );

//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//
//		/// 새로들어온 유저에게 NPC 버프 정보를 보낸다
//		if ( !kPacketNpcUnitBuffInfo.m_vecNpcUnitBuff.empty() )
//		{
//			BOOST_FOREACH( const UidType uidType, kPacket.m_vecNonNpcSyncUserList )
//			{
//				kPacketNpcUnitBuffInfo.m_iToUnitUID = uidType;
//				g_pData->GetServerProtocol()->SendPacket( EGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT, kPacketNpcUnitBuffInfo );
//			}
//		}
//
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

		//m_pBattleFieldGame->SetPetFirstSyncPacketImmediateForce( kPacket.m_vecNonNpcSyncUserList );
		return true;
	}
	else 
		return false;
}

/*virtual*/ void CX2StateBattleField::UserAndPetListPopUpMenuProcess()
{
	ASSERT( NULL != g_pData->GetUserListPopUp() );
	if ( NULL == g_pData->GetUserListPopUp() )
		return;

	D3DXVECTOR3 startRay, endRay;
	g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
		g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

	const BOOL bRbutton = g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_RBUTTON );// BOOL

	if ( TRUE == bRbutton )
	{
		vector<pair<UidType,bool> > vTempUserList;

#ifdef SERV_PET_SYSTEM
		CX2PetManager* pPetManager = g_pData->GetPetManager();
		if( NULL != pPetManager && pPetManager->GetMyPetPick( false ) == true )  // 마우스에 펫이 있는 경우!?
			vTempUserList.push_back( make_pair( pPetManager->GetMyPet()->GetPetInfo().m_PetUid, true ) );
#endif

		const UINT		uiUnitListSize = g_pX2Game->GetUserUnitListSize();
		const UidType	uidMyGUUserUID = g_pX2Game->GetMyUnit()->GetUnitUID();

		for( UINT i = 0; i < uiUnitListSize; ++i )
		{
			CX2GUUser* pGUUserSelected = g_pX2Game->GetUserUnit( i );

			if( pGUUserSelected != NULL &&
				//pGUUserSelected->GetUnitUID() != uidMyGUUserUID && 
				pGUUserSelected->GetShowObject() )
			{
				D3DXVECTOR3 vPos = pGUUserSelected->GetPos();
				vPos.y += 100.0f;

				// pick.
				if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, vPos,	pGUUserSelected->GetBoundingRadius() * 0.5f ) == true )
					vTempUserList.push_back( make_pair( pGUUserSelected->GetUnitUID() , false  ) );
			}			
		}

		if ( false == vTempUserList.empty() )
		{
#ifdef SERV_PET_SYSTEM
			pPetManager->ClosePetPopupMenu();
#endif
			g_pData->GetUserListPopUp()->ClosePopupMenu();
			for ( UINT i = 0;i < vTempUserList.size(); ++i )
				g_pData->GetUserListPopUp()->AddList(  vTempUserList[i].first, vTempUserList[i].second );

			g_pData->GetUserListPopUp()->OpenPopupMenu( false );
		}
	}
}

/*virtual*/ bool CX2StateBattleField::Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_GAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	bool bResult = m_pBattleFieldGame->Handler_EGS_END_GAME_NOT( kEvent );

	if ( NULL != g_pData->GetBattleFieldRoom() )
	{
		UINT uiBattleFieldId = g_pData->GetBattleFieldRoom()->GetBattleFieldId();
		UINT uiVillageId = g_pData->GetBattleFieldManager().GetReturnVillageId( uiBattleFieldId );
		CX2State::Handler_EGS_STATE_CHANGE_FIELD_REQ( uiVillageId );
	}

	return bResult;
}

/*virtual*/ void CX2StateBattleField::SetShowCommonBG()
{
	if ( m_pCashShop != NULL && 
		m_pCashShop->GetOpen() == true )
	{
		SetOpenCommonBG( true );
	}
	else
	{
		SetOpenCommonBG( false );
	}
}

/*virtual*/ void CX2StateBattleField::SetShowStateDLG( bool bShow )
{
	if ( bShow )
	{
		g_pKTDXApp->GetDGManager()->GetCamera().Move( m_vOldEyePt.x, m_vOldEyePt.y, m_vOldEyePt.z );
		g_pKTDXApp->GetDGManager()->GetCamera().LookAt( m_vOldLookAtPt.x, m_vOldLookAtPt.y, m_vOldLookAtPt.z );

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), bShow );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->UnHideChatWindow();
		}		
	}
	else
	{
		m_vOldEyePt		= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
		m_vOldLookAtPt	= g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();

		g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,-5000,-1300, 0,-5000,0 );
		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), bShow );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->HideChatWindow();			
		}
	}

	if ( NULL != m_pBattleFieldGame )
	{
		m_pBattleFieldGame->GetWorld()->SetShowObject( bShow );
		m_pBattleFieldGame->SetShowAllUnitGageBar( bShow );
	}

	CX2GageManager::GetInstance()->SetShow( bShow );
}

bool CX2StateBattleField::Handler_EGS_BATTLE_FIELD_EVENT_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	switch ( kEvent.m_cEventType )
	{
	case KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT::BET_WARNING_MESSAGE:
		{
/*
// 오현빈 // 2012-10-10 // 보스시스템 제작 전 danger UI 제거
			CKTDGParticleSystem* pGameMinorParticle = g_pData->GetGameMinorParticle();

			if ( NULL != pGameMinorParticle )
			{
				pGameMinorParticle->CreateSequence( NULL,  L"BOSS_DANGER",		0, 0, 0, 1000, 1000, -1, 1, -1 );
				pGameMinorParticle->CreateSequence( NULL,  L"BOSS_DANGER_ADD",	0, 0, 0, 1000, 1000, -1, 1, -1 );
			}
*/
		} break;

	default:
		break;
	}

	return true;	
}

void CX2StateBattleField::DrawMovingSmallBar()
{
	const int iLastTouchedLineMapIndex = m_pBattleFieldGame->GetMyGUUserLastTouchedLineMapIndex( true );
	const CKTDGLineMap::LineData* pLastTouchedLineData = m_pBattleFieldGame->GetLineMap()->GetLineData( iLastTouchedLineMapIndex );
	D3DXVECTOR3 vPosMovingGage( ( (pLastTouchedLineData->startPos.x + pLastTouchedLineData->endPos.x ) / 2), pLastTouchedLineData->startPos.y + 300, pLastTouchedLineData->startPos.z );

	D3DXVECTOR3 vOut( 0.0f, 0.0f, 0.0f );
	D3DXVec3Project( &vOut, &vPosMovingGage, &g_pKTDXApp->GetViewport(), 
		&g_pKTDXApp->GetProjectionTransform(), &g_pKTDXApp->GetViewTransform(), &g_pKTDXApp->GetWorldTransform() );

	vOut.x -= ( 25 * g_pKTDXApp->GetResolutionScaleX() );
	vOut.y += ( -90 * g_pKTDXApp->GetResolutionScaleY() );

	m_TexDataMovingGageBG.pTexture->SetDeviceTexture();
	DrawFace( vOut.x+1.0f, vOut.y+1.0f, m_TexDataMovingGageBG,
 		D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	vOut.x += ( 13 * g_pKTDXApp->GetResolutionScaleX() );
	vOut.y += ( 25 * g_pKTDXApp->GetResolutionScaleY() );
	const float fNowPercent = m_TimerWaitingPortal.GetSumOfElapsedTime() / m_TimerWaitingPortal.GetTargetTime();
	m_TexDataMovingGage.pTexture->SetDeviceTexture();
	DrawFace( vOut.x, vOut.y, m_TexDataMovingGage,
		D3DCOLOR_ARGB( 255, 255, 255, 255 ), fNowPercent );
}

void CX2StateBattleField::DrawFace( const float fX_, const float fY_, const CKTDGUIControl::UITextureData& texData_, 
	D3DCOLOR color_ /* = 0xffffffff */, const float fWidthPercent_ /*= 1.0f*/  )
{
	const float fWidth_		= texData_.texSize.x * g_pKTDXApp->GetResolutionScaleX();
	const float fHeight_	= texData_.texSize.y * g_pKTDXApp->GetResolutionScaleY();


	float X1 = fX_ - 0.5f;
	float Y1 = fY_ - 0.5f;
	float X2 = fX_ + fWidth_ * fWidthPercent_;
	float Y2 = fY_ + fHeight_;

	float UMax = 1.0f;
	float VMax = 1.0f;

	DRAWFACE_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(DRAWFACE_RHW_VERTEX) * 4 );

	const float& fTextureLeftTopX		= texData_.uvTexture[CKTDGUIControl::VP_LEFT_TOP].x;
	const float& fTextureLeftBottomX	= texData_.uvTexture[CKTDGUIControl::VP_LEFT_BOTTOM].x;
	const float& fTextureRightTopX		= texData_.uvTexture[CKTDGUIControl::VP_RIGHT_TOP].x;
	const float& fTextureRightBottomX	= texData_.uvTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].x;
	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
	vertex[0].u = fTextureLeftTopX;   vertex[0].v = texData_.uvTexture[CKTDGUIControl::VP_LEFT_TOP].y;

	vertex[1].x = X1;     vertex[1].y = Y2;
	vertex[1].u = fTextureLeftBottomX;   vertex[1].v = texData_.uvTexture[CKTDGUIControl::VP_LEFT_BOTTOM].y;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;

	vertex[2].x = X2;     vertex[2].y = Y1;
	vertex[2].u = fTextureLeftTopX + ( fTextureRightTopX - fTextureLeftTopX ) * fWidthPercent_;   vertex[2].v = texData_.uvTexture[CKTDGUIControl::VP_RIGHT_TOP].y;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;

	vertex[3].x = X2;     vertex[3].y = Y2;
	vertex[3].u = fTextureLeftBottomX + ( fTextureRightBottomX - fTextureLeftBottomX )* fWidthPercent_;   vertex[3].v = texData_.uvTexture[CKTDGUIControl::VP_RIGHT_BOTTOM].y;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;

	vertex[0].color = vertex[1].color = 
		vertex[2].color = vertex[3].color = color_;

	KD3DPUSH( m_RenderStateID )

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		BOOST_STATIC_ASSERT( D3DFVF_DRAWFACE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//		g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAWFACE_RHW_VERTEX) );
//#endif

	KD3DEND()
}

void CX2StateBattleField::CreateMovingSmallBar()
{
	/// 로딩게이지 백그라운드
	m_TexDataMovingGageBG.pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( m_TexDataMovingGageBG.texName );
	const CKTDXDeviceTexture::TEXTURE_UV* pTexUvBG = m_TexDataMovingGageBG.pTexture->GetTexUV( m_TexDataMovingGageBG.keyName );

	if ( NULL != pTexUvBG )
	{
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]	= pTexUvBG->leftTop;
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]	= pTexUvBG->rightTop;
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUvBG->leftBottom;
		m_TexDataMovingGageBG.uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUvBG->rightBottom;
		m_TexDataMovingGageBG.texSize = pTexUvBG->rectSize;

		m_TexDataMovingGageBG.SetTextureUV();
	}

	/// 로딩게이지
	m_TexDataMovingGage.pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( m_TexDataMovingGage.texName );
	const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = m_TexDataMovingGage.pTexture->GetTexUV( m_TexDataMovingGage.keyName );

	if ( NULL != pTexUV )
	{
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]	= pTexUV->leftTop;
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]	= pTexUV->rightTop;
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]	= pTexUV->leftBottom;
		m_TexDataMovingGage.uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]	= pTexUV->rightBottom;
		m_TexDataMovingGage.texSize = pTexUV->rectSize;

		m_TexDataMovingGage.SetTextureUV();
	}
}

void CX2StateBattleField::DestroyMovingSmallBar()
{
	SAFE_CLOSE( m_TexDataMovingGageBG.pTexture );
	SAFE_CLOSE( m_TexDataMovingGage.pTexture );
}

bool CX2StateBattleField::ProcessStateChangeUnitSelectWithOptionWindow()
{
	if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow( false ) )
		return true;
	else
		return CX2StateMenu::ProcessStateChangeUnitSelectWithOptionWindow();

}

bool CX2StateBattleField::ProcessStateChangeServerSelectWithOptionWindow()
{
	if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow( false ) )
		return true;
	else
		return CX2StateMenu::ProcessStateChangeServerSelectWithOptionWindow();
}

bool CX2StateBattleField::Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_UNIT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != m_pBattleFieldGame )
		m_pBattleFieldGame->Handler_EGS_UPDATE_UNIT_INFO_NOT( kEvent );

	return true;
}

/** @function : Handler_EGS_VISIT_CASH_SHOP_NOT
	@brief : 배틀필드에서 캐시샵에 들어가거나 나온 유저가 누구인지 알려주는 패킷
*/
bool CX2StateBattleField::Handler_EGS_VISIT_CASH_SHOP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_VISIT_CASH_SHOP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2GUUser* pGUUser= g_pX2Game->GetUserUnitByUID( kEvent.m_iUnitUID );
	if ( NULL != pGUUser )
		pGUUser->SetEnterCashShop( kEvent.m_bEnterCashShop );
	return true;
}

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
bool CX2StateBattleField::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

#ifdef CHANGE_INVENTORY_TAB_BY_WHEEL
	// 인벤토리가 열려 있을 때는 인벤토리의 휠 처리만 하기
	if( NULL != g_pData &&
		NULL != g_pData->GetUIManager() && 
		NULL != g_pData->GetUIManager()->GetUIInventory() &&
		true == g_pData->GetUIManager()->GetUIInventory()->GetIsMouseOver() )
	{
		return true;
	}
#endif // CHANGE_INVENTORY_TAB_BY_WHEEL

	short zDelta = static_cast<short>(HIWORD(wParam));
	m_SumDelta += zDelta;
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			g_pMain->GetGameOption().CameraZoomIn( 1 );
			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{
			g_pMain->GetGameOption().CameraZoomIn( -1 );
			m_SumDelta += WHEEL_DELTA;
		}	
	}
	return true;
}
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL


#ifdef SERV_CHECK_EXIST_MONSTER_UID
/** @function : Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK
	@brief : 현재 필드에 존재하지 않는 몬스터의 UID가 담긴 패킷 처리, 해당 NPC들을 삭제해 준다.
	@return : 패킷을 정상적으로 처리 했으면 true, 아니면 false
*/
bool CX2StateBattleField::Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( NULL != m_pBattleFieldGame )
		return m_pBattleFieldGame->Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK( hWnd, uMsg, wParam, lParam );
	else
		return false;
}
#endif // SERV_CHECK_EXIST_MONSTER_UID



/// mauntain // 2013-05-03 // 결혼 시스템 배우자 소환 기능

/** @function	: Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT
	@brief		: 소환 대상자를 소환 요청자가 있는 마을로 이동 시키는 패킷을 처리
	@param		: 이벤트 메시지
	@return		: 패킷 처리 여부
*/

#ifdef ADDED_RELATIONSHIP_SYSTEM
bool CX2StateBattleField::Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );			

		bool						bCanCreateWorld		= true;
		const int					iLastTouchLineIndex	= kEvent.m_LastTouchLineIndex;
		const SEnum::VILLAGE_MAP_ID	eVillageId			= static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iMapID );
		const D3DXVECTOR3			vPos( kEvent.m_vPos.x, kEvent.m_vPos.y, kEvent.m_vPos.z );
		
		if ( g_pData == NULL || 
			g_pData->GetLocationManager() == NULL||
			eVillageId <= SEnum::VMI_INVALID ||
			eVillageId >= SEnum::VMI_VILLAGE_MAP_END )
		{
			bCanCreateWorld = false;		/// 마을을 생성할 수 없음.
		}

		// 마을 생성		
		if(bCanCreateWorld == true)
		{
			g_pData->GetLocationManager()->SetVillage( eVillageId, vPos, iLastTouchLineIndex );

			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, NULL, false );
		}
	}
	else
	{
		if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
		{
			CX2StateBattleField* pStateBattleField = static_cast<CX2StateBattleField*>( g_pMain->GetNowState() );
			pStateBattleField->SetNowMovingToOtherPlace(true);
		}

		// 이전위치로 복원			
		g_pData->GetLocationManager()->RestoreVillage();
		if(g_pTFieldGame != NULL)
			g_pTFieldGame->SetEnableKeyProcess(true);
	}

	g_pData->GetUIManager()->CloseAllNPCDlg();

	return true;
}

#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef PLAY_EMOTION_BY_USER_SELECT
/*virtual */void CX2StateBattleField::PlayEmotionByUserSelect()
{
	if( g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_LBUTTON ) == TRUE &&
		NULL != g_pChatBox && NULL != g_pX2Game )
	{
		D3DXVECTOR3 startRay, endRay;
		g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
			g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

		for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
		{
			CX2GameUnit* pCX2GameUnit = g_pX2Game->GetUnit(i);
			if( NULL != pCX2GameUnit &&
				true == pCX2GameUnit->IsMyUnit() )
			{
				D3DXVECTOR3 pos = pCX2GameUnit->GetPos();
				pos.y += 100.0f;

				if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, pos,	pCX2GameUnit->GetBoundingRadius()/2.0f ) == true )
				{
					int iRandVAlue = RandomInt(0,4);
					switch( iRandVAlue )
					{
					case 0 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2508 ) ); break;
					case 1 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2509 ) ); break;
					case 2 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2513 ) ); break;
					case 3 : g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2515 ) ); break;
					default: g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2503 ) ); break;
					}
					break;
				}
			}			
		}
	}
}
#endif // PLAY_EMOTION_BY_USER_SELECT

#ifdef FIELD_BOSS_RAID
/** @function : MoveToBeforePlaceAtRaidField
	@brief : 보스 필드 입장 이전 필드로 이동.
*/
void CX2StateBattleField::MoveToBeforePlaceAtRaidField()
{
	const UINT uiFieldID = CX2BossRaidManager::GetInstance()->GetBossRaidCreatorMapID();
	Handler_EGS_JOIN_BATTLE_FIELD_REQ( uiFieldID, g_pData->GetBattleFieldManager().GetRaidFieldPortalLineByFieldID( uiFieldID ) );
	SetNowMovingToOtherPlace( true );
}
#endif // FIELD_BOSS_RAID

#ifdef MONSTER_STATE_LIST_TEST
void MonsterStateListDialog::Update()
{
	if( m_pDLGMonsterStateList == NULL )
		return;

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGMonsterStateList->GetControl( L"ListBox_Monster_ID" );
	if( NULL == pListBox )
		return;

	if ( NULL == g_pMain->GetGameEdit() )
		return;


	int npcID = g_pMain->GetGameEdit()->GetLastCreatedMonster();
	CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByType(npcID);
	if( NULL == pNPC )
		return; 

	if( (CX2UnitManager::NPC_UNIT_ID)npcID == m_NPCID )
		return; 
	m_NPCID = (CX2UnitManager::NPC_UNIT_ID)npcID;

	m_vecStateName.resize(0); 
	std::vector< std::wstring > vecAnimationName;	
	pNPC->EnumerateStateAndAnimationName( m_vecStateName, vecAnimationName );
	ASSERT( m_vecStateName.size() == vecAnimationName.size() );

	pListBox->RemoveAllItems();
    std::wstring wstrTemp;
	for( int i=0; i<(int)vecAnimationName.size(); i++ )
	{
        wstrTemp.resize( 0 );
        ConvertUtf8ToWCHAR( wstrTemp, m_vecStateName[i] );
		vecAnimationName[i] += L"(";
		vecAnimationName[i] += wstrTemp;
		vecAnimationName[i] += L")";

		pListBox->AddItem( vecAnimationName[i].c_str(), (void*)&m_vecStateName[i] );
	}

	pListBox->SetScrollBarWidth( 26 );
	pListBox->SetScrollBarEndPos();
}

void MonsterStateListDialog::KeyProcess( CKTDXStage* pStage )
{
	if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
		return; 

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_PERIOD) == TRUE )
	{
		if( true == IsOpen() )
		{
			Close();
		}
		else
		{
			Open( pStage );
		}
	}
}

void MonsterStateListDialog::OnCommand( const std::string& monsterName )
{
	if( NULL != g_pX2Game )
	{
		g_pX2Game->EnableAllNPCAI( false ); 
	}

	WCHAR wszText[256] = L"";
    std::wstring    wstrTemp;
	StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"/msc %s", ConvertUtf8ToWCHAR( wstrTemp, monsterName ).c_str() ); 

	if ( NULL != g_pMain->GetGameEdit() )
		g_pMain->GetGameEdit()->ExecCommand( wszText );
}
#endif // MONSTER_STATE_LIST_TEST
#include "StdAfx.h"


#include ".\x2statepvproom.h"


CX2StatePVPRoom::CX2StatePVPRoom(void) 
: m_pCX2MapSelectWindow( NULL )
#ifdef SERVER_PVP_BASE_DEFENCE_TEST
, m_pDLGPVPGameOption( NULL )
#endif SERVER_PVP_BASE_DEFENCE_TEST
{
	DialogLog( "\nCX2StatePVPRoom::CX2StatePVPRoom Start\n" );
	//PVPRoomLog << __FUNCTION__ << fileout;

	m_pCX2PVPRoom = g_pData->GetPVPRoom();

	
	m_pDLGLoadingState	= NULL;
	m_bWaitTeamChangeAck = false;
	m_bInitUIFail		= false;
	m_pTalkBoxMgr		= NULL;
	

	m_fMotionChangeTime = 0;
	m_fMotionChangeRefreshTime = (float)RandomNumber( 10, 17 );

	m_bEnableMotionChange = true;

	m_iUserPopupSlotIdx			= 0;
	m_bBaned				= false;
	m_pDLGMsgBoxForBan		= NULL;
	m_UnitUIDToBan			= 0;

	m_bSendLeaveRoomReqByForce = false;

	m_pDLGTalkBox = NULL;

	m_fElapsedIdleTime = 0.f;

	m_bReserveCashShopOpen = false;

	m_pCX2MapSelectWindow = new CX2StatePVPRoom::CX2MapSelectWindow();

#ifdef FIX_INTRUSION_SKILL_BUG
	m_bDisableSkillUIOpen = false;
#endif //FIX_INTRUSION_SKILL_BUG

#ifdef SERV_NEW_PVPROOM_PROCESS
	m_pDLGSetPasswordRoom = new CKTDGUIDialog( this, L"DLG_UI_Pvp_Set_Password.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSetPasswordRoom );
	m_pDLGSetPasswordRoom->SetShowEnable(false, false);
#endif SERV_NEW_PVPROOM_PROCESS

	if ( UISetting() == false ) //실패한 경우
	{
		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), L"방 로드에 실패했습니다.", this );
		//MessageBox( g_pKTDXApp->GetHWND(), L"방 로드 실패\n로비로 되돌아옵니다.", L"오류", MB_OK );
		m_bInitUIFail = true;
		Handler_EGS_LEAVE_ROOM_REQ();
		return;
	}

	//m_pDLGPVPRoomFront->OpenScriptFile( L"DLG_PVP_Room_Lua_Func.lua" );
	//LuaFuncCall( L"InitUI" );
	InitPvpRoomUI();
#ifdef REFORM_UI_SKILLSLOT
	//대전방에서 퀵슬롯 안보이도록 설정.
	SetShowMenu(true);
	g_pData->GetUIManager()->SetShowQuickSlot(false);
	g_pData->GetUIManager()->SetShowSkillSlot(false);
#endif //REFORM_UI_SKILLSLOT

	//{{ dmlee 2008.7.13 play channel
	{
		CKTDGUIStatic* pStatic_PlayTime			= (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_TIME" );
		CKTDGUIStatic* pStatic_KillCount		= (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_KILL" );
		CKTDGUIStatic* pStatic_PlayChannelDesc	= (CKTDGUIStatic*)m_pDLGPVPRoomBack->GetControl( L"Practice_Channel_Notice" );

		if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
		{
			pStatic_PlayTime->SetShowEnable( false, false );
			pStatic_KillCount->SetShowEnable( false, false );
									
			if( NULL != pStatic_PlayChannelDesc )
			{
				pStatic_PlayChannelDesc->SetShowEnable( true, true );
			}
		}
		else
		{
			pStatic_PlayTime->SetShowEnable( true, true );

			if ( m_pCX2PVPRoom != NULL && m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM )
			{
				pStatic_KillCount->SetShowEnable( false, false );
			}
			else
			{
				pStatic_KillCount->SetShowEnable( true, true );
			}
			
			if( NULL != pStatic_PlayChannelDesc )
			{
				pStatic_PlayChannelDesc->SetShowEnable( false, false );
			}
		}
	}
	//}} dmlee 2008.7.13 play channel	



	


#ifdef TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		if( m_bSendLeaveRoomReqByForce == false && 
			NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
		{
			m_bSendLeaveRoomReqByForce = true;
			Handler_EGS_LEAVE_ROOM_REQ( NetError::NOT_LEAVE_ROOM_REASON_24 );
		}
	}
#endif TEMP_HERO_MATCH_PREVENT_ITEM_POSSESSION

#ifdef TEMP_HERO_MATCH_ADMISSION_ITEM_POSSESSION
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		if( m_bSendLeaveRoomReqByForce == false && 
			NULL == g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( MAGIC_HERO_MATCH_ITEM_ID ) )
		{
			m_bSendLeaveRoomReqByForce = true;
			Handler_EGS_LEAVE_ROOM_REQ( NetError::NOT_LEAVE_ROOM_REASON_24 );
		}
	}
#endif 




#if defined(TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING) || defined(TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_MAP)
	CKTDGUIButton* pButton_MapSelect = (CKTDGUIButton*) m_pDLGPVPRoomFront->GetControl( L"PVP_Room_Map_Select" );
	if( NULL != pButton_MapSelect )
	{
		if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
		{
			pButton_MapSelect->SetShowEnable( false, false );
		}
		else
		{
			pButton_MapSelect->SetShowEnable( true, true );
		}
	}
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING


	m_pFontForIPAndPort = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );
	g_pData->GetPVPRoom()->SetIntrudeGame( false );
	g_pKTDXApp->SkipFrame();

	DialogLog( "\nCX2StatePVPRoom::CX2StatePVPRoom End\n" );

#ifdef SERV_PET_SYSTEM
	if( m_pCX2PVPRoom != NULL )
	{
		for(int i=0; i<m_pCX2PVPRoom->GetSlotNum(); ++i)
		{
			CX2Room::SlotData *pSlot = m_pCX2PVPRoom->GetSlotData(i);
			if( pSlot != NULL && pSlot->m_pUnit != NULL && pSlot->m_pUnit->GetPetInfo() != NULL )
			{
				KPetInfo *pPetInfo = pSlot->m_pUnit->GetPetInfo();
				CX2PET::PetInfo petInfo;

				petInfo.m_PetUid = pPetInfo->m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
				petInfo.m_PetId = pPetInfo->m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
				petInfo.m_PetId = pPetInfo->m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
				petInfo.m_Evolution_Step = pPetInfo->m_cEvolutionStep;
				petInfo.m_wstrPetName = pPetInfo->m_wstrPetName;		
				petInfo.m_Intimacy = (float)pPetInfo->m_iIntimacy;
				petInfo.m_Satiety = pPetInfo->m_sSatiety;
				petInfo.m_Emotion = (int)pPetInfo->m_sEmotion;
				petInfo.m_Extroversion = (int)pPetInfo->m_sExtroversion;
#ifdef PET_DROP_ITEM_PICKUP
				petInfo.m_bIsDropItemPickup = pPetInfo->m_bAutoLooting;
#endif //PET_DROP_ITEM_PICKUP
#ifdef SERV_PET_SYSTEM_EX1
				petInfo.m_bAlwaysMaxSatiety = pPetInfo->m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1

				petInfo.m_bSummon = true;
#ifdef SERV_EVENT_PET_INVENTORY
				petInfo.m_bEventFoodEat		= pPetInfo->m_bEventFoodEat;
				petInfo.m_bIsEventPetID		= pPetInfo->m_bIsEventPetID;
#endif SERV_EVENT_PET_INVENTORY
				g_pData->GetPetManager()->CreatePet( pSlot->m_pUnit->GetUID(), petInfo );
			}
		}
	}
#endif

}

CX2StatePVPRoom::~CX2StatePVPRoom(void)
{
	DialogLog( "\nCX2StatePVPRoom::~CX2StatePVPRoom Start\n" );
	//PVPRoomLog << __FUNCTION__ << fileout;

//	SAFE_DELETE_DIALOG( m_pDLGPVPRoomBack );
//	SAFE_DELETE_DIALOG( m_pDLGPVPRoomFront );




	if ( m_pDLGPVPRoomBack != NULL )
	{
		m_pDLGPVPRoomBack->SetShow( false );
		m_pDLGPVPRoomBack->SetEnable( false );
	}

	if ( m_pDLGPVPRoomFront != NULL )
	{
		m_pDLGPVPRoomFront->SetShow( false );
		m_pDLGPVPRoomFront->SetEnable( false );
	}

	SAFE_DELETE_DIALOG( m_pDLGTalkBox );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState ); // note: 얘는 다이얼로그 매니저에서 관리되는 애가 아니라서~ 요렇게
#endif // DIALOG_HANDLE_TEST // 2009-8-19

	SAFE_DELETE_DIALOG( m_pDLGMsgBoxForBan );

#ifdef SERV_NEW_PVPROOM_PROCESS
	SAFE_DELETE_DIALOG( m_pDLGSetPasswordRoom );
#endif SERV_NEW_PVPROOM_PROCESS

	for ( int i = 0; i < MAX_PVP_ROOM_USER_NUM; i++ )
	{
		PVP_ROOM_USER_BOX* pPVPRoomUserBox = m_PVPRoomUserBoxList[i];
		SAFE_DELETE( pPVPRoomUserBox );

		m_PVPRoomUserBoxList[i] = NULL;
	}



	if ( m_bReserveCashShopOpen == true )
	{
		if ( g_pData->GetCashShop() != NULL )
			g_pData->GetCashShop()->SetAutoOpen( true );		
	}

	SAFE_DELETE( m_pCX2MapSelectWindow );

#ifdef SERVER_PVP_BASE_DEFENCE_TEST
#pragma NOTE( "주의!!! PVP 게임관련 코드 테스트중입니다. 본섭 패치에 포함되면 안됩니다." )
	SAFE_DELETE_DIALOG( m_pDLGPVPGameOption );
#endif SERVER_PVP_BASE_DEFENCE_TEST

	DialogLog( "\nCX2StatePVPRoom::~CX2StatePVPRoom End\n" );
	//g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForIPAndPort );

	// 파티 상태 복원
	//g_pPartyMsgerUI->SetPartyModeOnly( true );
	//g_pPartyMsgerUI->ChangeChatWindowAndIMEEditBox( CX2PartyMsgerUI::CWM_PARTY );


//#ifdef SERV_PET_SYSTEM
//	g_pData->GetPetManager()->AllRemovePet();
//#endif

}

HRESULT CX2StatePVPRoom::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if ( m_bInitUIFail == true )
		return S_OK;


	// 일정기간 입력이 없으면 방에서 나간다
	const float MAGIC_IDLE_TIME = 600.f;
	m_fElapsedIdleTime += fElapsedTime;
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetNumKeyPresses() > 0 )
	{
		m_fElapsedIdleTime = 0.f;
	}
	if( 0 != g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() ||
		0 != g_pKTDXApp->GetDIManager()->GetMouse()->GetYDelta() )
	{
		m_fElapsedIdleTime = 0.f;
	}
	if( m_fElapsedIdleTime > MAGIC_IDLE_TIME )
	{
		m_fElapsedIdleTime = 0.0f;
		Handler_EGS_LEAVE_ROOM_REQ();
	}

	

	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );


	//g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );
	if( NULL != g_pX2Room )
	{
		g_pX2Room->OnFrameMove( fTime, fElapsedTime );
	}

	//////////////////////////////////////////////////////////////////////////
	

	m_fMotionChangeTime += fElapsedTime;
	if ( m_fMotionChangeTime >= m_fMotionChangeRefreshTime )
	{
		m_fMotionChangeTime = 0.f;
		m_fMotionChangeRefreshTime = (float)RandomNumber( 10, 17 );

		ChangeMotion();
	}

	//////////////////////////////////////////////////////////////////////////

	UIFrameMove();

	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
	{
		g_pData->GetPlayGuide()->OnFrameMove(fTime, fElapsedTime);
	}
#ifdef POSTBOX
    // 우편함처리
    if(g_pMain->GetPostBox()->GetDialogShow() == true && g_pMain->GetPostBox()->GetCheckPitIn() == false)
    {
        if(g_pMain->GetPostBox()->GetSwitchState() == false)
        {
            if( NULL != g_pX2Room &&
				g_pX2Room->GetMySlot()->m_bReady == true )
            {
                Handler_EGS_CHANGE_READY_REQ();
            }

            Handler_EGS_CHANGE_PITIN_REQ();
        }
        
        g_pMain->GetPostBox()->SetCheckPitIn(true);
    }

    if(g_pMain->GetPostBox()->GetDialogShow() == false && g_pMain->GetPostBox()->GetCheckPitIn() == true)
    {
        if ( NULL != g_pX2Room && 
			g_pX2Room->GetMySlot()->m_bReady == true )
        {
            Handler_EGS_CHANGE_READY_REQ();
        }

        Handler_EGS_CHANGE_PITIN_REQ();
        g_pMain->GetPostBox()->SetCheckPitIn(false);
    }    
#endif

	if( NULL != g_pX2Room )
	{
		for ( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
			SetUserBox( pSlotData );
		}

#ifdef SERV_KEY_MAPPING_INT
		if( GET_KEY_STATE( GA_PARTYREADY ) == TRUE || g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F8) == TRUE )
#else // SERV_KEY_MAPPING_INT
		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F8) == TRUE )
#endif // SERV_KEY_MAPPING_INT
		{
			//{{ kimhc // 2011-03-02 // when skill window's been opened, you can't input the key, F8
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			CX2UISkillTreeNew* pUISkillTree = g_pData->GetUIManager()->GetUISkillTree();
		#else // UPGRADE_SKILL_SYSTEM_2013
			CX2UISkillTree* pUISkillTree = g_pData->GetUIManager()->GetUISkillTree();
		#endif // UPGRADE_SKILL_SYSTEM_2013

			if ( NULL == pUISkillTree ||
				 ( NULL != pUISkillTree && false == pUISkillTree->GetShow() ) )
			{			
				//{{ kimhc // 2011-02-21 // 드래그앤드랍 중에 F8 키를 누를 수 없도록 수정
				CX2SlotManager::CX2Slot** ppSlotBeforeDragging = g_pData->GetSlotBeforeDragging();

				// 드래깅중인게 없으면
				if ( NULL == *ppSlotBeforeDragging )
				{
					if( g_pX2Room->GetMySlot()->m_bHost == true )
					{
#ifndef NOT_APPLY_F8
						Handler_EGS_STATE_CHANGE_GAME_START_REQ();
#endif
					}
					else
					{
						if ( g_pX2Room->GetRoomState() == CX2Room::RS_LOADING || g_pData->GetPVPRoom()->GetRoomState() == CX2Room::RS_PLAY )
						{
#ifndef NOT_APPLY_F8
							Handler_EGS_STATE_CHANGE_GAME_INTRUDE_REQ();
#endif
						}
						else
						{
							Handler_EGS_CHANGE_READY_REQ();
						}

					}
				} // if
				//}} kimhc // 2011-02-21 // 드래그앤드랍 중에 F8 키를 누를 수 없도록 수정
			}
			//}} kimhc // 2011-03-02 // when skill window's opend, you can't input the key, F8
		}


		if ( g_pX2Room->GetBan() == true )
		{
			g_pX2Room->SetBan( false );

			m_bBaned = true;

			SAFE_DELETE_DIALOG( m_pDLGMsgBoxForBan );
			m_pDLGMsgBoxForBan = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_623 ), this );
		}
	}


	if ( m_bBaned == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGMsgBoxForBan ) == false )
		{
			m_pDLGMsgBoxForBan = NULL;
			m_bBaned = false;

			// 유저정보보기 창이 열려있는 경우에 onframerender에서 m_pCX2PVPRoom의 slot->unit을 참조하기 때문에 m_pCX2PVPRoom가 삭제되기 전에 유저정보보기창을 닫아준다
			if( true ==  g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_WATCH) )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, false);
			}

			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
			
#ifndef HEAP_BROKEN_BY_ROOM
			g_pData->DeletePVPRoom();			
#endif // HEAP_BROKEN_BY_ROOM

			m_pCX2PVPRoom = NULL; // added by wonpok, 20100318.

			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false" );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
		}
	}
	
	if( g_pData->GetMyUser() != NULL  &&
		g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR  )
	{
#ifndef NEW_SKILL_TREE
		if( false == g_pInstanceData->GetSkillTreeUI()->GetShow() )
#endif
		{
			if ( m_bSendLeaveRoomReqByForce == false && CheckUnitLevelByChannelList() == false )
			{
				m_bSendLeaveRoomReqByForce = true;
				Handler_EGS_LEAVE_ROOM_REQ( NetError::NOT_LEAVE_ROOM_REASON_04 );
			}
		}
	}

	//마우스 체크
	if( NULL != g_pX2Room )
	{
		if ( g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_RBUTTON ) == TRUE &&

#ifndef NEW_SKILL_TREE
			false == g_pInstanceData->GetSkillTreeUI()->GetShow() &&
#endif
			g_pData->GetUIManager()->IsOpenedWindow() == false && 		
			GetShowSystemMenu() == false &&
			( NULL == g_pData->GetMessenger() || g_pData->GetMessenger()->GetOverDialog() == false ) )
		{
			POINT mousePoint;
			GetCursorPos( &mousePoint );
			ScreenToClient( g_pKTDXApp->GetHWND(), &mousePoint );
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			if( m_pCX2PVPRoom != NULL )
			{
#ifdef SERV_PET_SYSTEM
				if( m_pCX2PVPRoom->GetClickPetSlot( mousePoint.x, mousePoint.y ) == true )
				{
					g_pData->GetPetManager()->OpenPetPopupMenu();
					g_pData->GetUserMenu()->ClosePopupMenu();
				}
				else
				{				
#endif
					int selectSlot = m_pCX2PVPRoom->GetClickSlot( mousePoint.x, mousePoint.y );
					if( selectSlot != -1 )
					{
						CX2Room::SlotData* pkSlotData = g_pX2Room->GetSlotData( selectSlot );
						if( pkSlotData != NULL && pkSlotData->m_UnitUID > 0 && pkSlotData->m_pUnit != NULL )
						{
							m_iUserPopupSlotIdx = selectSlot;

							if(g_pData->GetUserMenu() != NULL)
							{
#ifdef SERV_PET_SYSTEM
								g_pData->GetPetManager()->ClosePetPopupMenu();
#endif
								g_pData->GetUserMenu()->OpenUserPopupMenu(pkSlotData->m_UnitUID);
							}
						}
					}
#ifdef SERV_PET_SYSTEM
				}
#endif
			}			
		}
	}

	if ( m_pCX2MapSelectWindow != NULL )
	{
		m_pCX2MapSelectWindow->OnFrameMove( fTime, fElapsedTime );
	}


	return S_OK;
}




HRESULT CX2StatePVPRoom::OnFrameRender()
{

	if( true == g_pMain->GetIsPlayingTutorial() ||
		true == g_pMain->GetIsExitingTutorial() )
	{
		if( m_pDLGLoadingState != NULL )
			m_pDLGLoadingState->OnFrameRender();
		return S_OK;
	}

	if ( m_bInitUIFail == true )
		return S_OK;

	CX2StateMenu::OnFrameRender();


#ifdef SERV_PET_SYSTEM
	if( g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		g_pData->GetPetManager()->OnFrameRender();
	}
#endif


	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if ( g_pX2Room != NULL
#ifdef OBSERVER_DISABLE_IP
			&& g_pX2Room->GetMySlot()->IsObserver() == false
#endif // OBSERVER_DISABLE_IP
			)
		{
			m_pFontForIPAndPort->OutTextMultiline( 800, 10, g_pX2Room->GetCenterServerIP().c_str(),
				D3DXCOLOR( 1, 1, 1, 1 ), CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
		}
	}



	if ( g_pData->GetMyUser()->GetAuthLevel() > CX2User::XUAL_SPECIAL_USER )
	{
		if ( g_pX2Room != NULL
#ifdef OBSERVER_DISABLE_IP
			&& g_pX2Room->GetMySlot()->IsObserver() == false
#endif // OBSERVER_DISABLE_IP
			)
		{
			for ( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
			{
				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
				if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
					continue;

				wstring ipAndPort = pSlotData->m_pUnit->GetUnitData().m_IP + L":";
				WCHAR szPort[256] = {0};
				swprintf( szPort, L"%d", pSlotData->m_pUnit->GetUnitData().m_Port );
				ipAndPort += szPort;
				swprintf( szPort, L"\n%d", (int)pSlotData->m_PingTime );
				ipAndPort += szPort;

				//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
				if( g_pData->GetGameUDP() && g_pData->GetGameUDP()->GetPeer( pSlotData->m_UnitUID ) )
				{
					const CKTDNUDP::Peer* pPeer = g_pData->GetGameUDP()->GetPeer( pSlotData->m_UnitUID );
					if( pPeer->GetUseRelay() )
						ipAndPort += L" Relay";
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					else if( pPeer->m_eP2PConnectType == CKTDNUDP::P2PCONNECT_INTERNAL )
						ipAndPort += L" P2P using internal IP";
					else
						ipAndPort += L" P2P";
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					else
//						ipAndPort += L" P2P";
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					std::wstringstream wstrstm;
					wstrstm << std::endl 
                        << CKTDNUDP::ConvertAddressToIP( pPeer->m_IPAddress )
                        << L":" << pPeer->m_Port << std::endl << L"Tried " << pPeer->m_ConnectTestCount << L" times" << std::endl;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					std::wstringstream wstrstm;
//					wstrstm << std::endl 
//                        << pPeer->m_InternalIP 
//                        << L":" << pPeer->m_InternalPort << std::endl << L"Tried " << pPeer->m_ConnectTestCount << L" times" << std::endl;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

					ipAndPort += wstrstm.str();

				}
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
				//}}

				if( true == pSlotData->m_pUnit->GetUnitData().m_bMan )
				{
					ipAndPort += L" ";
                    ipAndPort += GET_STRING( STR_ID_535 );
				}
				else
				{
					ipAndPort += L" ";
                    ipAndPort += GET_STRING( STR_ID_536 );
				}

				ipAndPort += GET_REPLACED_STRING( ( STR_ID_537, "i", pSlotData->m_pUnit->GetUnitData().m_Age ) );



				D3DXVECTOR3 tempPos = m_pDLGPVPRoomBack->GetDummyPos( i + 8 );

				if ( i < 4 )
				{
					m_pFontForIPAndPort->OutTextMultiline( (int)tempPos.x, (int)tempPos.y - 100, ipAndPort.c_str(), D3DXCOLOR( 1, 1, 1, 1 ),  
						CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
				}
				else
				{
					m_pFontForIPAndPort->OutTextMultiline( (int)tempPos.x, (int)tempPos.y - 100, ipAndPort.c_str(), D3DXCOLOR( 1, 1, 1, 1 ),  
						CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
				}
			}
		}
	}

	return S_OK;
}





bool CX2StatePVPRoom::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;


	return false;
}

bool CX2StatePVPRoom::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch( wParam )
	{
		case PRUCM_RED_TEAM:
			Handler_EGS_CHANGE_TEAM_REQ( CX2Room::TN_RED );
			break;

		case PRUCM_BLUE_TEAM:
			Handler_EGS_CHANGE_TEAM_REQ( CX2Room::TN_BLUE );
			break;

		case PRUCM_READY:
			{
				Handler_EGS_CHANGE_READY_REQ();
			}
			break;

		case PRUCM_CHANGE_MAP:
			{
				if( m_pCX2PVPRoom != NULL && m_pCX2PVPRoom->IsRoomMaster() == true )
				{
					//OpenChangeMapDLG();
					m_pCX2MapSelectWindow->Open( m_pCX2PVPRoom->GetWorldIDForUI() );
				}
			}
			break;

#ifdef FREE_SELECT_PVP_MAP

		case PRUCM_CHANGE_MAP_SELECT_FOR_DEBUG:
			{
				CKTDGUIListBox* pListBox = (CKTDGUIListBox*) lParam;
				CKTDGUIListBox::ListBoxItem* pItem = pListBox->GetSelectedItem();
				if( NULL != pItem )
				{
					int iWorldID = (int) pItem->pData;
					m_pCX2MapSelectWindow->SetSelectedMapWorldIDForce( iWorldID );
				}
			} break;

#endif FREE_SELECT_PVP_MAP




		case PRUCM_CHANGE_MAP_SELECT:
			{
				CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

				if ( wcscmp( pControl->GetName(), L"g_pButton_ALL_RANDOM_BUTTON" ) == 0 )
				{
					m_pCX2MapSelectWindow->SetSelectMapByWorldID( pControl->GetDummyInt(0) );
				}
				else
				{
					if ( pControl->GetDialog() != NULL )
					{
						m_pCX2MapSelectWindow->SetSelectMapByWorldID( pControl->GetDialog()->GetDummyInt(0) );
					}
				}
			}
			break;

		case PRUCM_CHANGE_MAP_SELECT_OVER:
			{
				CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
				if ( pControl->GetDialog() != NULL )
				{
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pControl->GetDialog(), true );
				}
			}
			break;

		case PRUCM_CHANGE_MAP_SELECT_OUT:
			{
				if ( m_pCX2MapSelectWindow != NULL )
				{
					m_pCX2MapSelectWindow->ChangeSelectedMapDLGFirstSeq();
				}
			}
			break;

		case PRUCM_CHANGE_MAP_OK:
			{
				Handler_EGS_CHANGE_MAP_REQ( m_pCX2MapSelectWindow->GetSelectedMapWorldID() );
				m_pCX2MapSelectWindow->Close();	
			}
			break;

		case PRUCM_CHANGE_MAP_CANCEL:
			{
				m_pCX2MapSelectWindow->Close();
			}
			break;

		case PRUCM_START:
			Handler_EGS_STATE_CHANGE_GAME_START_REQ();
			break;

		case PRUCM_INTRUDE:
			Handler_EGS_STATE_CHANGE_GAME_INTRUDE_REQ();
			break;
		

		case PRUCM_BACK_TO_PVP_LOBBY:
			Handler_EGS_LEAVE_ROOM_REQ();
			break;

		case PRUCM_CHANGE_ROOM_INFO:
			//ChangePVPRoomInfoReq();
			break;

		case PRUCM_OPEN_OR_CLOSE_BUTTON:
			{
				/*
				if( g_pData->GetPVPRoom()->GetMySlot()->m_bHost == true )
				{
					CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
					Handler_EGS_CHANGE_SLOT_OPEN_REQ( pButton->GetName() );
				}
				*/
			}
			break;

		case PRUCM_NEXT_KILL_NUM:
			{
				if( m_pCX2PVPRoom == NULL )
					return false;

				if ( m_pCX2PVPRoom->IsRoomMaster() == false )
					return false;

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
				{
					if ( m_pCX2PVPRoom->GetWinKillNum() >= (int)CX2PVPRoom::DTWKNL_MAX )
						return false;

					return Handler_EGS_CHANGE_WINNING_NUM_KILL_REQ( m_pCX2PVPRoom->GetWinKillNum() + 4 );
				}
				else
				{
					if ( m_pCX2PVPRoom->GetWinKillNum() >= (int)CX2PVPRoom::SDWKNL_MAX )
						return false;

					return Handler_EGS_CHANGE_WINNING_NUM_KILL_REQ( m_pCX2PVPRoom->GetWinKillNum() + 3 );
				}

				
			}
			break;

		case PRUCM_PREV_KILL_NUM:
			{
				if( m_pCX2PVPRoom == NULL )
					return false;

				if ( m_pCX2PVPRoom->IsRoomMaster() == false )
					return false;

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
				{
					if ( m_pCX2PVPRoom->GetWinKillNum() <= (int)CX2PVPRoom::DTWKNL_MIN )
						return false;

					return Handler_EGS_CHANGE_WINNING_NUM_KILL_REQ( m_pCX2PVPRoom->GetWinKillNum() - 4 );
				}
				else
				{
					if ( m_pCX2PVPRoom->GetWinKillNum() <= (int)CX2PVPRoom::SDWKNL_MIN )
						return false;

					return Handler_EGS_CHANGE_WINNING_NUM_KILL_REQ( m_pCX2PVPRoom->GetWinKillNum() - 3 );
				}

				
			}
			break;

		case PRUCM_NEXT_PLAY_TIME:
			{
				if( m_pCX2PVPRoom == NULL )
					return false;

				if ( m_pCX2PVPRoom->IsRoomMaster() == false )
					return false;

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM )
				{
					if ( m_pCX2PVPRoom->GetPlayTimeLimit() >= (float)CX2PVPRoom::TPTL_MAX )
						return false;

					return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( m_pCX2PVPRoom->GetPlayTimeLimit() + 100 );
				}
				else
				{
					if ( m_pCX2PVPRoom->GetPlayTimeLimit() >= (float)CX2PVPRoom::DMPTL_MAX )
						return false;

					return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( m_pCX2PVPRoom->GetPlayTimeLimit() + 300 );
				}
			}
			break;

		case PRUCM_PREV_PLAY_TIME:
			{
				if( m_pCX2PVPRoom == NULL )
					return false;

				if ( m_pCX2PVPRoom->IsRoomMaster() == false )
					return false;


				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM )
				{
#ifndef _SERVICE_
					// 개발자면 50초 되게
					if( NULL != g_pData->GetMyUser() && 
						g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
					{
						if ( m_pCX2PVPRoom->GetPlayTimeLimit() <= (float)CX2PVPRoom::TPTL_MIN )
						{
							return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( 50 );
						}
					}
#endif

					if ( m_pCX2PVPRoom->GetPlayTimeLimit() <= (float)CX2PVPRoom::TPTL_MIN )
						return false;

					return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( m_pCX2PVPRoom->GetPlayTimeLimit() - 100 );
				}
				else
				{
#ifndef _SERVICE_
					// 개발자면 50초 되게
					if( NULL != g_pData->GetMyUser() && 
						g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
					{
						if ( m_pCX2PVPRoom->GetPlayTimeLimit() <= (float)CX2PVPRoom::DMPTL_MIN )
						{
							return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( 50 );
						}
					}
#endif

					if ( m_pCX2PVPRoom->GetPlayTimeLimit() <= (float)CX2PVPRoom::DMPTL_MIN )
						return false;

					return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( m_pCX2PVPRoom->GetPlayTimeLimit() - 300 );
				}
				
			}
			break;

		case PRUCM_BAN_UNIT:
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				CKTDGUIDialogType pDialog = pButton->GetDialog();

				if ( pDialog != NULL )
				{
					CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_SlotIndex" );
					if ( pStaticSlotIndex != NULL )
					{
						int slotIndex = _wtoi( pStaticSlotIndex->GetString(0)->msg.c_str() );
						OnBanUnit( slotIndex );
					}
				}
			}
			break;

		case PRUCM_BAN_UNIT_OK:
			{
				g_pX2Room->Handler_EGS_BAN_USER_REQ( m_UnitUIDToBan );
				CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			}
			break;

		case PRUCM_WHISPER:
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				CKTDGUIDialogType pDialog = pButton->GetDialog();
				if ( pDialog == NULL )
					break;

				CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_SlotIndex" );
				if ( pStaticSlotIndex == NULL )
					break;

				int slotIndex = _wtoi( pStaticSlotIndex->GetString(0)->msg.c_str() );
				if ( slotIndex == -1 )
					break;

				if( m_pCX2PVPRoom == NULL )
					break;

				m_iUserPopupSlotIdx = slotIndex;				

				CX2Room::SlotData* pkSlotData = m_pCX2PVPRoom->GetSlotData( slotIndex );
				if ( pkSlotData == NULL )
					break;

#ifndef DEPRECATED_SERVER_GROUP_MASK
#ifdef SERV_INTEGRATION
				//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
#ifdef EXTEND_SERVER_GROUP_MASK
				int iServerGroupID = -1;
				iServerGroupID = (int) g_pMain->ExtractServerGroupID(pkSlotData->m_UnitUID);
				if(g_pInstanceData->GetServerGroupID() != iServerGroupID)
#else
				SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
				eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(pkSlotData->m_UnitUID);
				if(g_pInstanceData->GetServerGroupID() != eServerGroupID)
#endif // EXTEND_SERVER_GROUP_MASK
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5130 ), g_pMain->GetNowState() );
					break;
				}
				//}}
#endif SERV_INTEGRATION
#endif DEPRECATED_SERVER_GROUP_MASK

				g_pChatBox->SetPickedUser( pkSlotData->m_UnitUID, pkSlotData->m_pUnit->GetNickName() );

				return g_pChatBox->OnOpenWhisperChatMode();

			} break;

		case PRUCM_USER_INFO:
			{
				CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
				CKTDGUIDialogType pDialog = pButton->GetDialog();
				if ( pDialog == NULL )
					break;

				CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_SlotIndex" );
				if ( pStaticSlotIndex == NULL )
					break;

				int slotIndex = _wtoi( pStaticSlotIndex->GetString(0)->msg.c_str() );
				if ( slotIndex == -1 )
					break;

				if( m_pCX2PVPRoom == NULL )
					break;

				CX2Room::SlotData* pkSlotData = m_pCX2PVPRoom->GetSlotData( slotIndex );
				if ( pkSlotData == NULL )
					break;
// 2008.12.17 김태완 : 구 유저정보를 엿보기로 연결				
#ifdef NEW_VILLAGE_UI
				return g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, true, pkSlotData->m_UnitUID );
#else
				if ( pkSlotData->m_pUnit == NULL )
					break;
				return Handler_EGS_SEARCH_UNIT_REQ( pkSlotData->m_pUnit->GetNickName() );
#endif

			} break;

		case PRUCM_CHANGE_MAP_NEXT_TITLE:
			{
				
			}
			break;

		case PRUCM_CHANGE_MAP_PREV_TITLE:
			{
				
			}
			break;
#ifdef DUNGEON_ITEM
		case PRUCM_CHANGE_PVP_ITEMMODE:
			{
				Handler_EGS_CHANGE_PVP_ITEMMODE_REQ();
			}
			break;
#endif





#ifdef SERVER_PVP_BASE_DEFENCE_TEST

		case PRUCM_CHECK_BASE_DEFENCE_MODE:
			{
				if( m_pCX2PVPRoom == NULL )
					return true;

				CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
				m_pCX2PVPRoom->GetPVPGameOption().m_bBaseDefence = pCheckBox->GetChecked();

				Handler_EGS_CHANGE_PVP_GAME_OPTION_REQ( m_pCX2PVPRoom->GetPVPGameOption().m_bBaseDefence, m_pCX2PVPRoom->GetPVPGameOption().m_bBossCombat );
				return true;

			} break;

		case PRUCM_CHECK_BOSS_COMBAT_MODE:
			{
				if( m_pCX2PVPRoom == NULL )
					return true;

				CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
				m_pCX2PVPRoom->GetPVPGameOption().m_bBossCombat = pCheckBox->GetChecked();

				Handler_EGS_CHANGE_PVP_GAME_OPTION_REQ( m_pCX2PVPRoom->GetPVPGameOption().m_bBaseDefence, m_pCX2PVPRoom->GetPVPGameOption().m_bBossCombat );
				return true;

			} break;
#endif SERVER_PVP_BASE_DEFENCE_TEST

#ifdef SERV_NEW_PVPROOM_PROCESS
		case PRUCM_CHANGE_PVP_ROOM_PUBLIC:
			{
				if( m_pCX2PVPRoom == NULL || m_pCX2PVPRoom->GetMySlot() == NULL )
					return true;

				if( m_pCX2PVPRoom->IsRoomMaster() == false )
					return true;

				if( m_pCX2PVPRoom->GetRoomPublic() == false )
				{
					Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_REQ(true, L"");
					return true;
				}
			} break;
		case PRUCM_CHANGE_PVP_ROOM_PRIVATE:
			{
				if( m_pCX2PVPRoom == NULL || m_pCX2PVPRoom->GetMySlot() == NULL )
					return true;

				if( m_pCX2PVPRoom->IsRoomMaster() == false )
					return true;

				if( m_pCX2PVPRoom->GetRoomPublic() == true )
				{
					OpenSetRoomPasswordDLG();
					return true;
				}
			} break;
		case PRUCM_SET_PVP_ROOM_PASSWORD_OK:
			{
				if( m_pCX2PVPRoom == NULL )
					return true;

				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGSetPasswordRoom->GetControl( L"IME_EditPassword" );
				wstring wstrPassword = pIMEEditBox->GetText();
				if( wstrPassword.length() == 0 )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300,300), GET_STRING( STR_ID_22237 ), g_pMain->GetNowState() );
					return true;
				}

				m_pDLGSetPasswordRoom->SetShow(false);

				if( m_pCX2PVPRoom->GetRoomPublic() == true )
				{
					Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_REQ(false, wstrPassword);
					return true;
				}
			} break;
		case PRUCM_SET_PVP_ROOM_PASSWORD_CANCLE:
		case PRUCM_SET_PVP_ROOM_PASSWORD_EXIT:
			{
				if( m_pCX2PVPRoom == NULL )
					return true;

				m_pDLGSetPasswordRoom->SetShow(false);
			} break;
		case PRUCM_CHANGE_PVP_ROOM_NAME_OK:
			{
				if( m_pCX2PVPRoom == NULL || m_pCX2PVPRoom->GetMySlot() == NULL )
					return true;

				if( m_pCX2PVPRoom->IsRoomMaster() == false )
					return true;

				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGPVPRoomBack->GetControl( L"IMEPVP_Room_Name" );
				wstring wstrRoomName = pIMEEditBox->GetText();
				wstrRoomName = g_pMain->GetStringFilter()->FilteringChatString( wstrRoomName.c_str(), L'♡' );

				if( m_pCX2PVPRoom->GetRoomName() == wstrRoomName )
					return true;

				Handler_EGS_CHANGE_PVP_ROOM_NAME_REQ(wstrRoomName);
			} break;
#endif SERV_NEW_PVPROOM_PROCESS
	}
	return true;
}


bool CX2StatePVPRoom::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;


	switch( wParam )
	{
		case EGS_JOIN_ROOM_NOT:
			return Handler_EGS_JOIN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_LEAVE_ROOM_ACK:
			return Handler_EGS_LEAVE_ROOM_ACK( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_LEAVE_ROOM_NOT:
			return Handler_EGS_LEAVE_ROOM_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_LEAVE_GAME_NOT:
			return Handler_EGS_LEAVE_GAME_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_CHANGE_TEAM_ACK:
			return Handler_EGS_CHANGE_TEAM_ACK(  hWnd, uMsg, wParam, lParam );
			break;
		case EGS_CHANGE_TEAM_NOT:
			return Handler_EGS_CHANGE_TEAM_NOT(  hWnd, uMsg, wParam, lParam );
			break;

		case EGS_CHANGE_READY_ACK:
			return Handler_EGS_CHANGE_READY_ACK(  hWnd, uMsg, wParam, lParam );
			break;
		case EGS_CHANGE_READY_NOT:
			return Handler_EGS_CHANGE_READY_NOT(  hWnd, uMsg, wParam, lParam );
			break;

		case EGS_CHANGE_MAP_ACK:
			return Handler_EGS_CHANGE_MAP_ACK(  hWnd, uMsg, wParam, lParam );
			break;
		case EGS_CHANGE_MAP_NOT:
			return Handler_EGS_CHANGE_MAP_NOT(  hWnd, uMsg, wParam, lParam );
			break;

		case EGS_CHANGE_SLOT_OPEN_ACK:
			return Handler_EGS_CHANGE_SLOT_OPEN_ACK(  hWnd, uMsg, wParam, lParam );
			break;

		case EGS_CHANGE_SLOT_OPEN_NOT:
			return Handler_EGS_CHANGE_SLOT_OPEN_NOT(  hWnd, uMsg, wParam, lParam );
			break;



		case EGS_STATE_CHANGE_GAME_START_ACK:
			return Handler_EGS_STATE_CHANGE_GAME_START_ACK( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_STATE_CHANGE_GAME_START_NOT:
			return Handler_EGS_STATE_CHANGE_GAME_START_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_STATE_CHANGE_GAME_INTRUDE_ACK:
			return Handler_EGS_STATE_CHANGE_GAME_INTRUDE_ACK( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_STATE_CHANGE_GAME_INTRUDE_NOT:
			return Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_PLAY_START_NOT:
			return Handler_EGS_PLAY_START_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_INTRUDE_START_NOT:
			return Handler_EGS_INTRUDE_START_NOT( hWnd, uMsg, wParam, lParam );
			break;

		//case EGS_END_GAME_NOT:
		//	return Handler_EGS_END_GAME_NOT( hWnd, uMsg, wParam, lParam );
		//	break;

		case EGS_RESULT_ALL_UNIT_SUCCESS_NOT:
			return Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_CHANGE_WINNING_NUM_KILL_ACK:
			return Handler_EGS_CHANGE_WINNING_NUM_KILL_ACK( hWnd, uMsg, wParam, lParam  );
			break;

		case EGS_CHANGE_WINNING_NUM_KILL_NOT:
			return Handler_EGS_CHANGE_WINNING_NUM_KILL_NOT( hWnd, uMsg, wParam, lParam  );
			break;

		case EGS_CHANGE_PLAY_TIME_LIMIT_ACK:
			return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_ACK( hWnd, uMsg, wParam, lParam  );
			break;

		case EGS_CHANGE_PLAY_TIME_LIMIT_NOT:
			return Handler_EGS_CHANGE_PLAY_TIME_LIMIT_NOT( hWnd, uMsg, wParam, lParam  );
			break;
		
		case EGS_CHANGE_MOTION_ACK:
			return Handler_EGS_CHANGE_MOTION_ACK(  hWnd, uMsg, wParam, lParam  );
			break;

		case EGS_CHANGE_MOTION_NOT:
			return Handler_EGS_CHANGE_MOTION_NOT(  hWnd, uMsg, wParam, lParam  );
			break;

		case EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT:
		case EGS_REPAIR_ITEM_NOT:
			return Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
			break;

		case EGS_BAN_USER_ACK:
			{
				if ( g_pX2Room != NULL )
					return g_pX2Room->Handler_EGS_BAN_USER_ACK( hWnd, uMsg, wParam, lParam );

				return true;
			}
			break;

		case EGS_BAN_USER_NOT:
			{
				if ( g_pX2Room != NULL )
					g_pX2Room->SetBan( true );

				return true;
			}
			break;

		case EGS_CHANGE_PITIN_ACK:
			{
				return Handler_EGS_CHANGE_PITIN_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_CHANGE_PITIN_NOT:
			{
				return Handler_EGS_CHANGE_PITIN_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_CHANGE_TRADE_NOT:
			{
				return Handler_EGS_CHANGE_TRADE_NOT( hWnd, uMsg, wParam, lParam );

			} break;
#ifdef DUNGEON_ITEM
		case EGS_CHANGE_PVP_ITEMMODE_ACK:
			{
				return Handler_EGS_CHANGE_PVP_ITEMMODE_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;
		case EGS_CHANGE_PVP_ITEMMODE_NOT:
			{
				return Handler_EGS_CHANGE_PVP_ITEMMODE_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
#endif


#ifdef SERVER_PVP_BASE_DEFENCE_TEST

		case EGS_CHANGE_PVP_GAME_OPTION_ACK:
			{
				return Handler_EGS_CHANGE_PVP_GAME_OPTION_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_CHANGE_PVP_GAME_OPTION_NOT:
			{
				return Handler_EGS_CHANGE_PVP_GAME_OPTION_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
#endif SERVER_PVP_BASE_DEFENCE_TEST

#ifdef SERV_NEW_PVPROOM_PROCESS
		case EGS_CHANGE_PVP_ROOM_PUBLIC_ACK:
			{
				return Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;
		case EGS_CHANGE_PVP_ROOM_PUBLIC_NOT:
			{
				return Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
		case EGS_CHANGE_PVP_ROOM_NAME_ACK:
			{
				return Handler_EGS_CHANGE_PVP_ROOM_NAME_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;
		case EGS_CHANGE_PVP_ROOM_NAME_NOT:
			{
				return Handler_EGS_CHANGE_PVP_ROOM_NAME_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
#endif SERV_NEW_PVPROOM_PROCESS
	}
	return false;
}

void CX2StatePVPRoom::OnBanUnit( int slotIndex )
{
	if ( slotIndex == -1 )
		slotIndex = m_iUserPopupSlotIdx;

	if ( g_pData->GetPVPRoom()->IsRoomMaster() == true || g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( slotIndex );
		if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			m_UnitUIDToBan = pSlotData->m_pUnit->GetUID();
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( 250, 300 ), 
				GET_STRING( STR_ID_628 ), PRUCM_BAN_UNIT_OK, this );
		}
	}
}


//bool CX2StatePVPRoom::ClickMenuBackButton()
//{
//	return Handler_EGS_LEAVE_ROOM_REQ();
//}

void CX2StatePVPRoom::SetShowStateDLG( bool bShow )
{
	if ( m_pDLGPVPRoomBack != NULL )
	{
		m_pDLGPVPRoomBack->SetEnable( bShow );
		m_pDLGPVPRoomBack->SetShow( bShow );
	}

	if ( m_pDLGPVPRoomFront != NULL )
	{
		m_pDLGPVPRoomFront->SetEnable( bShow );
		m_pDLGPVPRoomFront->SetShow( bShow );
	}

	if ( m_pTalkBoxMgr != NULL )
		m_pTalkBoxMgr->SetShow( bShow );
}


bool CX2StatePVPRoom::ToggleLobbyMenuWindow()
{
	m_bShowWindowMenu = !m_bShowWindowMenu;

	if ( m_bShowWindowMenu == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
		m_pDLGMenuWindow = new CKTDGUIDialog( this, L"DLG_Menu_Window.lua", 0.07f );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMenuWindow );
	}
	else
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
	}



	return true;
}



bool CX2StatePVPRoom::LastMsgByESC()
{
	OpenLastMsgPopUp( GET_STRING( STR_ID_629 ) );
	return true;
}

void CX2StatePVPRoom::LastMsgPopOk()
{
	Handler_EGS_LEAVE_ROOM_REQ();
}


bool CX2StatePVPRoom::Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	

	/*CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
	if ( pSlotData != NULL )
	{
		if ( pSlotData->m_pUnit != NULL )
		{
			pComboBox->RemoveItem( FindItem( pSlotData->m_pUnit->GetNickName() ) ) );
		}
	}
	*/

	if ( m_pCX2PVPRoom != NULL && m_pCX2PVPRoom->Handler_EGS_JOIN_ROOM_NOT( kEvent ) == true )
	{
		CX2Room::SlotData* pSlotData = m_pCX2PVPRoom->GetSlotData( kEvent.m_JoinSlot.m_Index );
		if( pSlotData == NULL )
			return false;

		pSlotData->MakeUnitViewer();
		pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING ); 


		return SetUserBox( pSlotData );
	}

	return false;
}

bool CX2StatePVPRoom::Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason /* = NetError::NOT_LEAVE_ROOM_REASON_00 */  )
{
	const double MAGIC_ELAPSED_TIME = 1.0;

	// 방장이 게임 시작한 직후에는 방을 나갈 수 없다. 게임에 들어가서 나가야 된다.
	if( m_Timer_EGS_STATE_CHANGE_GAME_START_NOT.elapsed() < MAGIC_ELAPSED_TIME )
		return false;

	// 난입을 시작한 직후에는 방을 나갈 수 없다.
	if( m_Timer_EGS_STATE_CHANGE_GAME_INTRUDE_NOT.elapsed() < MAGIC_ELAPSED_TIME )
		return false;


	if ( m_bBaned == true )
	{
		// 유저정보보기 창이 열려있는 경우에 onframerender에서 m_pCX2PVPRoom의 slot->unit을 참조하기 때문에 m_pCX2PVPRoom가 삭제되기 전에 유저정보보기창을 닫아준다
		if( true ==  g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_WATCH) )
		{
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, false);
		}

		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
		
#ifndef HEAP_BROKEN_BY_ROOM
		g_pData->DeletePVPRoom();			
#endif // HEAP_BROKEN_BY_ROOM

		m_pCX2PVPRoom = NULL; // added by wonpok, 20100318.
		DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false" );
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );

		return true;
	}

	m_bEnableMotionChange = false;

	if( m_pCX2PVPRoom == NULL )
		return true;

	return m_pCX2PVPRoom->Handler_EGS_LEAVE_ROOM_REQ( leaveRoomReason );
}

bool CX2StatePVPRoom::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom != NULL && // added by wonpok, 20100318. 크래쉬 예외처리
		m_pCX2PVPRoom->Handler_EGS_LEAVE_ROOM_ACK( kEvent ) == true )
	{
		// 유저정보보기 창이 열려있는 경우에 onframerender에서 m_pCX2PVPRoom의 slot->unit을 참조하기 때문에 m_pCX2PVPRoom가 삭제되기 전에 유저정보보기창을 닫아준다
		if( true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_WATCH) )
		{
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WATCH, false);
		}

		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
		
#ifndef HEAP_BROKEN_BY_ROOM
		g_pData->DeletePVPRoom();			
#endif // HEAP_BROKEN_BY_ROOM

		m_pCX2PVPRoom = NULL;
		DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false" );
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
		return true;
	}	

	// added by wonpok, 20100318, 크래쉬 예외처리
	if( m_pCX2PVPRoom == NULL )
	{
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
		return true;
	}

	return false;
}

bool CX2StatePVPRoom::Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );


	if ( m_pCX2PVPRoom != NULL && m_pCX2PVPRoom->Handler_EGS_LEAVE_ROOM_NOT( kEvent ) == true )
	{
		for ( int i = 0; i < MAX_PVP_ROOM_USER_NUM; i++ )
		{
			PVP_ROOM_USER_BOX* pPVPRoomUserBox = m_PVPRoomUserBoxList[ i ];
			
			if ( pPVPRoomUserBox != NULL && pPVPRoomUserBox->unitUID == kEvent.m_iUnitUID )
			{
				pPVPRoomUserBox->unitUID = 0;
				pPVPRoomUserBox->SetShow( false );

				return true;
			}
		}

		CX2GageManager::GetInstance()->RemovePvpMemberUIByUserUid( kEvent.m_iUnitUID );
	}
	
	return false;
}

bool CX2StatePVPRoom::Handler_EGS_LEAVE_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_GAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_LEAVE_GAME_NOT( kEvent );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_TEAM_REQ( CX2Room::TEAM_NUM teamNum )
{
	m_bWaitTeamChangeAck = true;

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_TEAM_REQ( teamNum );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_TEAM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_TEAM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	if ( m_pCX2PVPRoom->Handler_EGS_CHANGE_TEAM_ACK( kEvent ) == true )
	{
		return true;
	}
	else
	{
		m_bWaitTeamChangeAck = false;

		if ( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
		{

			CX2Room::SlotData* pSlotData = m_pCX2PVPRoom->GetMySlot();

			CKTDGUIRadioButton* pRadioButton;
			if ( pSlotData->m_TeamNum == CX2Room::TN_RED )
			{
				pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" );
				pRadioButton->SetChecked( true );
				pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" );
				pRadioButton->SetChecked( false );
			}
			else if ( pSlotData->m_TeamNum == CX2Room::TN_BLUE )
			{
				pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" );
				pRadioButton->SetChecked( false );
				pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" );
				pRadioButton->SetChecked( true );
			}

		}
	}
/*
	CX2Room::SlotData* pSlotData = m_pCX2PVPRoom->GetMySlot();

	CKTDGUIRadioButton* pRadioButton;
	if ( pSlotData->m_TeamNum == CX2Room::TN_RED )
	{
		 pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomFront->GetControl( L"RadioButtonRed" );
		 pRadioButton->SetChecked( true );
		 pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomFront->GetControl( L"RadioButtonBlue" );
		 pRadioButton->SetChecked( false );
	}
	else if ( pSlotData->m_TeamNum == CX2Room::TN_BLUE )
	{
		pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomFront->GetControl( L"RadioButtonRed" );
		pRadioButton->SetChecked( false );
		pRadioButton = (CKTDGUIRadioButton*)m_pDLGPVPRoomFront->GetControl( L"RadioButtonBlue" );
		pRadioButton->SetChecked( true );
	}
*/	
	return false;
	
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_TEAM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_TEAM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	if ( m_pCX2PVPRoom->Handler_EGS_CHANGE_TEAM_NOT( kEvent ) == true )
	{
		D3DXVECTOR2 talkBoxPos = D3DXVECTOR2( m_pDLGPVPRoomFront->GetDummyPos( kEvent.m_SwapSlotIndex ).x , m_pDLGPVPRoomFront->GetDummyPos( kEvent.m_SwapSlotIndex ).y );
		m_pTalkBoxMgr->SetPos( kEvent.m_UnitUID, talkBoxPos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT );

		if ( g_pData->GetPVPRoom()->GetMySlot()->m_UnitUID == kEvent.m_UnitUID )
		{
			m_bWaitTeamChangeAck = false;
		}

		/*
		map< UidType, CKTDGUIDialogType >::iterator i;
		i = m_mapPVPRoomUserBox.find( kEvent.m_UnitUID );
		
		if ( i == m_mapPVPRoomUserBox.end() )
			return false;

		CKTDGUIDialogType pDialog = i->second;
		if ( pDialog == NULL )
			return false;
		D3DXVECTOR3 tempPos = m_pDLGPVPRoomBack->GetDummyPos( kEvent.m_SwapSlotIndex + 8 );
		D3DXVECTOR2 pos = D3DXVECTOR2( tempPos.x, tempPos.y );
		pDialog->SetPos( pos );
		*/

		for ( int i = 0; i < MAX_PVP_ROOM_USER_NUM; i++ )
		{
			PVP_ROOM_USER_BOX* pPVPRoomUserBox = m_PVPRoomUserBoxList[ i ];
			if ( pPVPRoomUserBox->unitUID == kEvent.m_UnitUID )
			{
				D3DXVECTOR3 tempPos = m_pDLGPVPRoomBack->GetDummyPos( kEvent.m_SwapSlotIndex + 8 );
				D3DXVECTOR2 pos = D3DXVECTOR2( tempPos.x, tempPos.y );
				pPVPRoomUserBox->pDialog->SetPos( pos );
				
				return true;
			}
		}
	}
	return false;
}


bool CX2StatePVPRoom::Handler_EGS_CHANGE_READY_REQ()
{
	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_READY_REQ();
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_READY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_READY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_READY_ACK( kEvent );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_READY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_READY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if ( m_pCX2PVPRoom != NULL && m_pCX2PVPRoom->Handler_EGS_CHANGE_READY_NOT( kEvent ) == true )
	{
		CX2Room::SlotData* pSlotData = m_pCX2PVPRoom->GetSlotDataByUnitUID( kEvent.m_UnitUID );

		if ( pSlotData != NULL && pSlotData->m_pUnitViewer != NULL )
		{
			if ( kEvent.m_bReady == true )
				pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
			else
			{
				pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
			}

			return true;
		}
	}



	return false;
}


bool CX2StatePVPRoom::Handler_EGS_CHANGE_PITIN_REQ()
{
	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_PITIN_REQ();
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PITIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PITIN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_PITIN_ACK( kEvent );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PITIN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_PITIN_NOT( kEvent );

}


bool CX2StatePVPRoom::Handler_EGS_CHANGE_MAP_REQ( int worldID )
{
	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_MAP_REQ( worldID );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_MAP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_MAP_ACK( kEvent );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_MAP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_MAP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if ( m_pCX2PVPRoom != NULL && m_pCX2PVPRoom->Handler_EGS_CHANGE_MAP_NOT( kEvent ) == true )
	{
		ResetNowMap();

		return true;
	}

	return false;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_SLOT_OPEN_REQ( int slotIndex )
{
#ifdef TEMP_HERO_MATCH_PREVENT_ROOM_SLOT_CHANGE
	if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3746 ), this );
		return true;
	}
#endif TEMP_HERO_MATCH_PREVENT_ROOM_SLOT_CHANGE

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_SLOT_OPEN_REQ( slotIndex );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_SLOT_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SLOT_OPEN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_SLOT_OPEN_ACK( kEvent );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_SLOT_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SLOT_OPEN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return false;

	bool bRetVal = m_pCX2PVPRoom->Handler_EGS_CHANGE_SLOT_OPEN_NOT( kEvent );
	//SetUIOXButton();

	return bRetVal;
}



bool CX2StatePVPRoom::Handler_EGS_STATE_CHANGE_GAME_START_REQ()
{
#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_STATE_CHANGE_GAME_START_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST

#ifdef FIX_INTRUSION_SKILL_BUG
	//게임 시작시 스킬창 열 수 없도록 수정
	m_bDisableSkillUIOpen = true;
#endif //FIX_INTRUSION_SKILL_BUG


	CX2PVPRoom::SlotData* pSlotData = g_pData->GetPVPRoom()->GetMySlot();
	if( pSlotData == NULL || pSlotData->m_bTrade == true )
	{
		return false;
	}

	if( m_pCX2PVPRoom == NULL )
		return false;

	if( m_pCX2PVPRoom->IsRoomMaster() == false )
		return false;


	KEGS_STATE_CHANGE_GAME_START_REQ kPacket;
	if ( m_pCX2PVPRoom->GetWorldIDForUI() == -1 )
	{
		kPacket.m_sWorldID = m_pCX2MapSelectWindow->GetAllRandomMap();
	}
	else
	{
		kPacket.m_sWorldID = m_pCX2PVPRoom->GetWorldIDForUI();
	}

//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
	// Change_state_Game_start_req를 전송하면 직후에는 던전 게임이 시작 할 때 까지스킬슬롯을 바꿀수 없도록 함
	g_pInstanceData->SetSendGameStartReqPacket( true );
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정
	
	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_GAME_START_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_GAME_START_ACK );
	SAFE_DELETE_DIALOG(m_pDLGMsgBox);


	return true;
}

bool CX2StatePVPRoom::Handler_EGS_STATE_CHANGE_GAME_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_GAME_START_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_GAME_START_ACK ) == true )
	{
		//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == false )
			g_pInstanceData->SetSendGameStartReqPacket( false );
#else MODIFY_INFINITE_SKILL_BUG
		return g_pMain->IsValidPacket( kEvent.m_iOK );
#endif MODIFY_INFINITE_SKILL_BUG
		//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정	

#ifdef FIX_INTRUSION_SKILL_BUG
		//ACK 받은 이후에는 UI 열 수 있도록 수정
		m_bDisableSkillUIOpen = false;
#endif //FIX_INTRUSION_SKILL_BUG
	}
	return false;
}

bool CX2StatePVPRoom::Handler_EGS_STATE_CHANGE_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_GAME_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );

	if( m_pCX2PVPRoom == NULL )
		return false;

	m_pCX2PVPRoom->SetWorldID( (CX2World::WORLD_ID)kEvent.m_sWorldID );

	if( g_pData->GetGameUDP()->ConnectRelayTestResult() == true || m_pCX2PVPRoom->GetUserNum() == 1 )
	{
		if( g_pData->GetPVPRoom()->IsRoomMaster() == true || g_pData->GetPVPRoom()->GetMySlot()->m_bReady == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_695 ) );
		}

		g_pData->GetPVPRoom()->SetRoomState( (CX2Room::ROOM_STATE)kEvent.m_cRoomState );
		g_pData->GetPVPRoom()->Set_KRoomSlotInfoList( kEvent.m_vecSlot );

		if( g_pData->GetPVPRoom()->GetMySlot()->m_bTrade == false && 
			(g_pData->GetPVPRoom()->IsRoomMaster() == true || g_pData->GetPVPRoom()->GetMySlot()->m_bReady == true) )
		{
			m_Timer_EGS_STATE_CHANGE_GAME_START_NOT.restart();

			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_GAME, NULL, false" );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_GAME, NULL, false );
	
			//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
	#ifdef	MODIFY_INFINITE_SKILL_BUG
			g_pInstanceData->SetReceiveGameStartNotPacket( true );	// kimhc // 2010-03-23 // packet 을 받았음
	#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정
		}
		else
		{
			ResetNowMap();
		}
	}
	else
	{
		Handler_EGS_LEAVE_ROOM_REQ();
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_633 ), this );
		ErrorLogMsg( XEM_ERROR92, L"PVP" );
	}

	return true;
}


bool CX2StatePVPRoom::Handler_EGS_STATE_CHANGE_GAME_INTRUDE_REQ()
{
	if( m_pCX2PVPRoom == NULL )
		return true;

#ifdef FIX_INTRUSION_SKILL_BUG
	//게임 시작시 스킬창 열 수 없도록 수정
	m_bDisableSkillUIOpen = true;
#endif //FIX_INTRUSION_SKILL_BUG

	if( g_pData->GetGameUDP()->ConnectRelayTestResult() == true || m_pCX2PVPRoom->GetUserNum() == 1 )
	{
		//{{ kimhc // 2010.05.10 // 난입 시에 무한 스킬 버그 수정
#ifdef	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
		//kimhc // 2010.7.29 // 스킬슬롯 체인지 중이면(즉, SkillSlotChangeReq를 전송한 후 아직 Ack를 받지 못한 상태이면)
		if ( g_pData->GetSkillTree()->IsSkillSlotChanging() == true )
			return true;	// 난입할 수 없음

		g_pInstanceData->SetSendGameStartReqPacket( true );	// kimhc // 2010-05-10 // 난입시에는 intrude_req가 스타트임
#endif	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
		//}} kimhc // 2010.05.10 // 난입 시에 무한 스킬 버그 수정

		g_pData->GetServerProtocol()->SendID( EGS_STATE_CHANGE_GAME_INTRUDE_REQ );
		g_pMain->AddServerPacket( EGS_STATE_CHANGE_GAME_INTRUDE_ACK );

		SAFE_DELETE_DIALOG(m_pDLGMsgBox);
		m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_696 ), this );
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_633 ), this );
		ErrorLogMsg( XEM_ERROR92, L"PVP_INTRUDE" );
	}

	return true;
}

bool CX2StatePVPRoom::Handler_EGS_STATE_CHANGE_GAME_INTRUDE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_GAME_INTRUDE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_GAME_INTRUDE_ACK ) == true )
	{
		//{{ kimhc // 2010.05.10 // 난입 시에 무한 스킬 버그 수정
#ifdef	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
		g_pInstanceData->SetSendGameStartReqPacket( false );	// kimhc // 2010-05-10 // 난입시에는 intrude_ack가 스타트임
#endif	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
		//}} kimhc // 2010.05.10 // 난입 시에 무한 스킬 버그 수정

#ifdef FIX_INTRUSION_SKILL_BUG
		//ACK 받은 이후에는 UI 열 수 있도록 수정
		m_bDisableSkillUIOpen = false;
#endif //FIX_INTRUSION_SKILL_BUG
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}
	return false;
}

bool CX2StatePVPRoom::Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_GAME_INTRUDE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( kEvent.m_IntruderSlot.m_kRoomUserInfo.m_nUnitUID == g_pData->GetPVPRoom()->GetMySlot()->m_UnitUID )
	{
		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_695 ) );
	}

	g_pData->GetPVPRoom()->SetRoomState( (CX2Room::ROOM_STATE)kEvent.m_RoomState );
	g_pData->GetPVPRoom()->Set_KRoomSlotInfo( kEvent.m_IntruderSlot );

	if( kEvent.m_IntruderSlot.m_kRoomUserInfo.m_nUnitUID == g_pData->GetPVPRoom()->GetMySlot()->m_UnitUID )
	{
		m_Timer_EGS_STATE_CHANGE_GAME_INTRUDE_NOT.restart();

		g_pData->GetPVPRoom()->SetIntrudeGame( true );
		DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_GAME, NULL, false" );

		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_GAME, NULL, false );

		//{{ kimhc // 2010.05.10 // 난입 시에 무한 스킬 버그 수정
#ifdef	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
		g_pInstanceData->SetReceiveGameStartNotPacket( true );	// kimhc // 2010-05-10 // 난입시 NOT 패킷을 받음
#endif	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
		//}} kimhc // 2010.05.10 // 난입 시에 무한 스킬 버그 수정
	}
	return true;
}


bool CX2StatePVPRoom::Handler_EGS_PLAY_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PLAY_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	g_pData->GetPVPRoom()->SetRoomState( (CX2Room::ROOM_STATE)kEvent.m_RoomState );
	g_pData->GetPVPRoom()->Set_KRoomSlotInfoList( kEvent.m_vecSlot );

	return true;
}

bool CX2StatePVPRoom::Handler_EGS_INTRUDE_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INTRUDE_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	g_pData->GetPVPRoom()->SetRoomState( (CX2Room::ROOM_STATE)kEvent.m_RoomState );
	g_pData->GetPVPRoom()->Set_KRoomSlotInfo( kEvent.m_IntruderSlot );

	return true;
}

//bool CX2StatePVPRoom::Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_END_GAME_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
//
//	if ( g_pX2Room != NULL )
//		return g_pX2Room->Handler_EGS_END_GAME_NOT( kEvent );
//
//	return true;
//}

bool CX2StatePVPRoom::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pX2Room != NULL )
	{
		g_pX2Room->Set_KRoomInfo( kEvent.m_RoomInfo );
		g_pX2Room->Set_KRoomSlotInfoList( kEvent.m_vecSlot );

		return true;
	}

	return false;
}


bool CX2StatePVPRoom::Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( float fPlayTime )
{
	KEGS_CHANGE_PLAY_TIME_LIMIT_REQ kPacket;

	if( m_pCX2PVPRoom == NULL )
		return true;

	int iPlayTime = (int) fPlayTime;
	if( true == CX2PVPRoom::IsValidPvpPlayTime( (char)m_pCX2PVPRoom->GetPVPGameType(), iPlayTime ) )
	{
		fPlayTime = (float) iPlayTime;
	}
	else
	{
		fPlayTime = (float) CX2PVPRoom::GetValidPvpPlayTime( (char)m_pCX2PVPRoom->GetPVPGameType() );
	}
	kPacket.m_fPlayTime = fPlayTime;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PLAY_TIME_LIMIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_PLAY_TIME_LIMIT_ACK );

	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_697 ), this );


	return true;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PLAY_TIME_LIMIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PLAY_TIME_LIMIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_PLAY_TIME_LIMIT_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}
	return false;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PLAY_TIME_LIMIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PLAY_TIME_LIMIT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( m_pCX2PVPRoom == NULL )
		return true;

	m_pCX2PVPRoom->SetPlayTime( kEvent.m_fPlayTime );

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_TIME" );
	if ( pStatic != NULL && pStatic->GetString( 0 ) != NULL )
	{
		wstringstream tempBuff;
		int playTime = (int)m_pCX2PVPRoom->GetPlayTimeLimit();
		tempBuff << playTime;
		pStatic->GetString( 0 )->msg = tempBuff.str().c_str();
	}

	return true;
}


bool CX2StatePVPRoom::Handler_EGS_CHANGE_WINNING_NUM_KILL_REQ( int winKillNum )
{
	KEGS_CHANGE_WINNING_NUM_KILL_REQ kPacket;
	char cWindKillNum = (char) winKillNum;

	if( m_pCX2PVPRoom == NULL )
		return true;

	if( true == CX2PVPRoom::IsValidWinKillNum( (char)m_pCX2PVPRoom->GetPVPGameType(), cWindKillNum ) )
	{
		winKillNum = cWindKillNum;
	}
	else
	{
		winKillNum = CX2PVPRoom::GetValidWinKillNum( (char)m_pCX2PVPRoom->GetPVPGameType() );
	}
	kPacket.m_iWinKillNum = winKillNum;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_WINNING_NUM_KILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_WINNING_NUM_KILL_ACK );

	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_697 ), this );

	return true;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_WINNING_NUM_KILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_WINNING_NUM_KILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_WINNING_NUM_KILL_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}
	return false;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_WINNING_NUM_KILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_WINNING_NUM_KILL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( m_pCX2PVPRoom == NULL )
		return true;

	m_pCX2PVPRoom->SetWinKillNum( kEvent.m_iWinKillNum );

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_KILL" );
	if ( pStatic != NULL && pStatic->GetString( 0 ) != NULL )
	{
		wstringstream tempBuff;
		int winKillNum = (int)m_pCX2PVPRoom->GetWinKillNum();
		tempBuff << winKillNum;
		pStatic->GetString( 0 )->msg = tempBuff.str().c_str();
	}

	return true;
}





bool CX2StatePVPRoom::Handler_EGS_CHANGE_MOTION_REQ( KEGS_CHANGE_MOTION_REQ& kEGS_CHANGE_MOTION_REQ )
{
	if ( m_bEnableMotionChange == false )
		return true;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_MOTION_REQ, kEGS_CHANGE_MOTION_REQ );
	//g_pMain->AddServerPacket( EGS_CHANGE_MOTION_ACK );

	return true;
}
bool CX2StatePVPRoom::Handler_EGS_CHANGE_MOTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_MOTION_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	//if( g_pMain->DeleteServerPacket( EGS_CHANGE_MOTION_ACK ) == true )
	//{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
		}
	//}

	return true;
}
bool CX2StatePVPRoom::Handler_EGS_CHANGE_MOTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_MOTION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if ( g_pX2Room != NULL )
	{
		for ( int i = 0; i < (int)g_pX2Room->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL || pSlotData->m_pUnitViewer == NULL )
				continue;

			if ( pSlotData->m_pUnit->GetUID() == kEvent.m_UnitUID )
			{
#ifdef UNIT_EMOTION
				if ( (pSlotData->m_bHost == true || pSlotData->GetSlotState() == CX2Room::SS_WAIT && pSlotData->m_bReady == false) &&
					pSlotData->m_pUnitViewer->GetPlayerEmotion() == false )
#else
				if ( pSlotData->m_bHost == true || pSlotData->GetSlotState() == CX2Room::SS_WAIT && pSlotData->m_bReady == false )
#endif
					
				{
					//wstring motionName = g_pX2Room->GetMotionName( (CX2Room::ROOM_MOTION)kEvent.m_cMotionID );
					//pSlotData->m_pUnitViewer->PlayAnim(  motionName.c_str(), CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					g_pX2Room->PlayMotionByID( (CX2Room::ROOM_MOTION)kEvent.m_cMotionID, pSlotData->m_pUnitViewer );
					break;

				}
			}
		}
	}

	

	return true;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_pCX2PVPRoom == NULL )
		return false;

	return m_pCX2PVPRoom->Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( kEvent );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_TRADE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pX2Room )
	{
		CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
		if( NULL != pSlotData )
		{
			pSlotData->m_bTrade = kEvent.m_bTrade;
		}
	}

	return true;
}

#ifdef DUNGEON_ITEM
void CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ITEMMODE_REQ()
{
	g_pData->GetServerProtocol()->SendID(EGS_CHANGE_PVP_ITEMMODE_REQ);
	g_pMain->AddServerPacket( EGS_CHANGE_PVP_ITEMMODE_ACK );

	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_697 ), this );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ITEMMODE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ITEMMODE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_PVP_ITEMMODE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}
bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ITEMMODE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ITEMMODE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pCX2PVPRoom == NULL )
		return true;

	m_pCX2PVPRoom->SetItemMode( kEvent );

	//{{ JHKang // 2010-08-23, 15:49 연습채널 아이템 대전 UI 수정
	CKTDGUIDialogManager *pDialogManager = g_pKTDXApp->GetDGManager()->GetDialogManager();
	if( pDialogManager == NULL )
		return false;

	CKTDGUIDialogType pDialog = pDialogManager->GetDialog( L"DLG_PVP_Room_Front" );

	CKTDGUIStatic* pStatic = NULL; //(CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_ITEM" );

#ifdef MODIFY_PVP_ROOM_ITEM_UI
	//{{ JHKang // 2010-08-23, 15:49 연습채널 아이템 대전 UI 수정
	if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_ITEM" );
		pStatic->SetShowEnable( false, false );
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_Practice_ITEM" );
		pStatic->SetShowEnable( true, true );
	}
	else
	{
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_Practice_ITEM" );
		pStatic->SetShowEnable( false, false );
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_ITEM" );
		pStatic->SetShowEnable( true, true );
	}
	//}}
	//}}
#endif MODIFY_PVP_ROOM_ITEM_UI
	if ( pStatic != NULL && pStatic->GetString( 0 ) != NULL )
	{
		if( m_pCX2PVPRoom->GetItemMode() == true )
		{
			// 아이템전
			pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_4817 ); // 아이템 전
		}
		else
		{
			// 파워전
			pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_4818 ); // 파워 전
		}
	}

	return true;
}
#endif



#ifdef SERVER_PVP_BASE_DEFENCE_TEST

	bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_GAME_OPTION_REQ( bool bBaseDefence, bool bBossCombat )
	{
		KEGS_CHANGE_PVP_GAME_OPTION_REQ kPacket;
		kPacket.m_bBaseDefence = bBaseDefence;
		kPacket.m_bBossCombat = bBossCombat;

		g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PVP_GAME_OPTION_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_CHANGE_PVP_GAME_OPTION_ACK );

		return true;
	}


	bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_GAME_OPTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_CHANGE_PVP_GAME_OPTION_ACK kEvent;
		DeSerialize( pBuff, &kEvent );

		if( g_pMain->DeleteServerPacket( EGS_CHANGE_PVP_GAME_OPTION_ACK ) == true )
		{
			if( g_pMain->IsValidPacket( kEvent.m_iOK ) )
			{
			}
		}

		return true;
	}

	bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_GAME_OPTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		KSerBuffer* pBuff = (KSerBuffer*)lParam;
		KEGS_CHANGE_PVP_GAME_OPTION_NOT kEvent;
		DeSerialize( pBuff, &kEvent );



		if( NULL != m_pCX2PVPRoom )
		{
			m_pCX2PVPRoom->GetPVPGameOption().m_bBaseDefence = kEvent.m_bBaseDefence;
			m_pCX2PVPRoom->GetPVPGameOption().m_bBossCombat = kEvent.m_bBossCombat;
		}

		return true;
	}

#endif SERVER_PVP_BASE_DEFENCE_TEST



void CX2StatePVPRoom::ChangeMotion()
{
	if ( m_bBaned == true )
	{
		return;
	}

	KEGS_CHANGE_MOTION_REQ kPacket;
	int motionID = RandomNumber( 0, CX2Room::RM_END - 1 );
	kPacket.m_cMotionID = motionID;

	Handler_EGS_CHANGE_MOTION_REQ( kPacket );
}







bool CX2StatePVPRoom::UISetting()
{
	for ( int i = 0; i < MAX_PVP_ROOM_USER_NUM; i++ )
	{
		m_PVPRoomUserBoxList[i] = NULL;
	}

	if( m_pCX2PVPRoom == NULL )
		return true;


	if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
	{
		m_pDLGPVPRoomBack = g_pMain->GetMemoryHolder()->GetPVPRoomTeamBackDLG( this );
	}
	else
	{
		m_pDLGPVPRoomBack = g_pMain->GetMemoryHolder()->GetPVPRoomSurvivorBackDLG( this );
	}

	if ( m_pDLGPVPRoomBack->GetIsSuccessInitOpenScript() == true )
	{
		//PVPRoomLog << "m_pDLGPVPRoomBack OpenScript 성공" << fileout;
	}
	else
	{
		ErrorLog(XEM_ERROR46);
		if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_698, "S", L"DLG_PVP_Room_Team_Back.lua" ) ), this );
			//PVPRoomLog << "DLG_PVP_Room_Team_Back.lua OpenScript 실패" << fileout;
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_698, "S", L"DLG_PVP_Room_Death_Back.lua" ) ), this );
			//PVPRoomLog << "DLG_PVP_Room_Death_Back.lua OpenScript 실패" << fileout;
		}


		return false;
	}






	for ( int i = 0; i < MAX_PVP_ROOM_USER_NUM; i++ )
	{
		PVP_ROOM_USER_BOX* pPVPRoomUserBox = new PVP_ROOM_USER_BOX();
		m_PVPRoomUserBoxList[ i ] = pPVPRoomUserBox;
		pPVPRoomUserBox->unitUID = 0;
		pPVPRoomUserBox->pDialog = new CKTDGUIDialog( this, L"DLG_PVP_Room_UserInfo.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pPVPRoomUserBox->pDialog );

#ifdef REMOVE_KR_SERVER_TEXTURE
		CKTDGUIStatic* pStaticServerGroup = (CKTDGUIStatic*)pPVPRoomUserBox->pDialog->GetControl( L"Static_SERVER" );
		if( pStaticServerGroup != NULL )
			pStaticServerGroup->SetShow( false );
#endif //REMOVE_KR_SERVER_TEXTURE

		D3DXVECTOR3 offsetPos = m_pDLGPVPRoomBack->GetDummyPos( i + 8 );
		D3DXVECTOR2 pos = D3DXVECTOR2( offsetPos.x,  offsetPos.y );

		pPVPRoomUserBox->pDialog->SetPos( pos );


		pPVPRoomUserBox->pDialogUserBack = new CKTDGUIDialog( this, L"DLG_PVP_Room_Character_Slot.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pPVPRoomUserBox->pDialogUserBack );
		offsetPos = m_pDLGPVPRoomBack->GetDummyPos( i + MAX_PVP_ROOM_USER_NUM * 2 );
		pos = D3DXVECTOR2( offsetPos.x,  offsetPos.y );
		pPVPRoomUserBox->pDialogUserBack->SetPos( pos );
		
		pPVPRoomUserBox->SetShow( false );

	}

	m_pDLGPVPRoomFront = g_pMain->GetMemoryHolder()->GetPVPRoomFrontDLG( this );

	if ( m_pDLGPVPRoomFront->GetIsSuccessInitOpenScript() == true )
	{
		//PVPRoomLog << "PVPRoomFront OpenScript 성공" << fileout;
	}
	else
	{
		ErrorLog(XEM_ERROR45);
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_REPLACED_STRING( ( STR_ID_698, "S", L"DLG_PVP_Room_Front.lua" ) ), this );
		//PVPRoomLog << "DLG_PVP_Room_Front.lua OpenScript 실패" << fileout;


		return false;
	}


	m_pDLGTalkBox = new CKTDGUIDialog( this, L"DLG_Room_Talk_Box.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGTalkBox );
	
	m_pTalkBoxMgr = new CX2TalkBoxManagerUI();
	m_pDLGTalkBox->AddControl( m_pTalkBoxMgr );

#ifdef SERV_NEW_PVPROOM_PROCESS
	CKTDGUIIMEEditBox* pIMEEditRoomName = (CKTDGUIIMEEditBox*)m_pDLGPVPRoomBack->GetControl( L"IMEPVP_Room_Name" );
	pIMEEditRoomName->SetText( m_pCX2PVPRoom->GetRoomName() );

	if( m_pCX2PVPRoom->IsRoomMaster() == false )
	{
		pIMEEditRoomName->SetShowEnable( true, false );
	}
	else
	{
		pIMEEditRoomName->SetShowEnable( true, true );
	}
#else
	CKTDGUIStatic* pStaticRoomName = (CKTDGUIStatic*)m_pDLGPVPRoomBack->GetControl( L"StaticPVP_Room_Name" );
	pStaticRoomName->GetString(0)->msg = m_pCX2PVPRoom->GetRoomName();
#endif SERV_NEW_PVPROOM_PROCESS




	for( int i = 0; i < g_pData->GetPVPRoom()->GetSlotNum(); i++ )
	{
		CX2PVPRoom::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( i );

		if ( pSlotData == NULL )
			continue;

		

		switch( pSlotData->m_SlotState )
		{
			case CX2Room::SS_EMPTY:
				{
				}
				break;

			case CX2Room::SS_CLOSE:
				{
				}
				break;

			case CX2Room::SS_WAIT:
				{
					if( pSlotData->m_pUnitViewer == NULL )
						pSlotData->MakeUnitViewer();
					pSlotData->m_pUnitViewer->SetShowObject( true );

					if( pSlotData->m_bMySlot == true )
					{
						if( pSlotData->m_bReady == false )
						{
							pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING );							
						}
						else
						{
							pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
						}
					}
					else
					{
						if( pSlotData->m_bReady == false )
						{
							pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
						}
						else
						{
							pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
						}
					}
				}
				break;

			case CX2Room::SS_LOADING:
				{
					if( pSlotData->m_pUnitViewer == NULL )
						pSlotData->MakeUnitViewer();
					pSlotData->m_pUnitViewer->SetShowObject( true );

					pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
				}
				break;

			case CX2Room::SS_PLAY:
				{
					if( pSlotData->m_pUnitViewer == NULL )
						pSlotData->MakeUnitViewer();
					pSlotData->m_pUnitViewer->SetShowObject( true );

					pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
				}
				break;
		}

		SetUserBox( pSlotData );
	}



	//룸에서 뿌려줄 그림 정보 셋팅

	//확대된 맵 그림 셋팅
	

	ResetNowMap();

	return true;
}




void CX2StatePVPRoom::UIFrameMove()
{
	KTDXPROFILE();
/*
	slotNum = g_pX2Room:GetSlotNum()
		pPVPRoom = g_pData:GetPVPRoom()

		pDialogManager = g_pKTDXApp:GetDialogManager()
		pDialog = pDialogManager:GetDialog( "DLG_PVP_Room_Front" )

		pDialogBack = nil
		if ( pPVPRoom:GetPVPGameType() == PVP_GAME_TYPE["PGT_TEAM"] ) or ( pPVPRoom:GetPVPGameType() == PVP_GAME_TYPE["PGT_TEAM_DEATH"] ) then

			pDialogBack = pDialogManager:GetDialog( "DLG_PVP_Room_Team_Back" )

		else

		pDialogBack = pDialogManager:GetDialog( "DLG_PVP_Room_Death_Back" )

		end




		if ( pPVPRoom:GetPVPGameType() == PVP_GAME_TYPE["PGT_SURVIVAL"] )  or (pPVPRoom:GetPVPGameType() == PVP_GAME_TYPE["PGT_TEAM_DEATH"]) then

		pButton = pDialogBack:GetButton("PVP_Room_Kill_LeftArrow")
		pButton:SetShowEnable( true, true )

		pButton = pDialogBack:GetButton("PVP_Room_Kill_RightArrow")
		pButton:SetShowEnable( true, true )

		else

		pButton = pDialogBack:GetButton("PVP_Room_Kill_LeftArrow")
		pButton:SetShowEnable( false, false )

		pButton = pDialogBack:GetButton("PVP_Room_Kill_RightArrow")
		pButton:SetShowEnable( false, false )
		end

		pButton = pDialogBack:GetButton("PVP_Room_Time_LeftArrow")
		pButton:SetEnable( true )

		pButton = pDialogBack:GetButton("PVP_Room_Time_RightArrow")
		pButton:SetEnable( true )	

*/
	//////////////////////////////////////////////////////////////////////////
	
	if( m_pCX2PVPRoom == NULL )
		return;

	wstringstream tempBuff;

	CKTDGUIControl* pControl = NULL;
	CKTDGUIStatic* pStatic = NULL; 

	
	CKTDGUIControl* pPVP_Room_Start = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Start" );
	CKTDGUIControl* pPVP_Room_Ready = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Ready" );
	CKTDGUIControl* pPVP_Room_Play = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Play" );
	
	CX2Room::SlotData* pMySlotData = m_pCX2PVPRoom->GetMySlot();
	if ( pMySlotData != NULL )
	{
		if( m_pCX2PVPRoom->IsRoomMaster( pMySlotData ) == true )
		{
			pControl = pPVP_Room_Start;
			if ( pControl != NULL )
			{
				pControl->SetShowEnable( true, true );
			}

			pControl = pPVP_Room_Ready;
			if ( pControl != NULL )
			{
				pControl->SetShowEnable( false, false );
			}

			pControl = pPVP_Room_Play;
			if ( pControl != NULL )
			{
				pControl->SetShowEnable( false, false );
			}

#ifdef MODIFY_PVP_ROOM_ITEM_UI
			//{{ JHKang // 2010-08-25, 15:49 연습채널 아이템 대전 UI 수정
			CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
			CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );

			CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
			CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );

			CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
			CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );

			CKTDGUIButton* pButton_Practice_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Practice_Item_LeftArrow" );
			CKTDGUIButton* pButton_Practice_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Practice_Item_RightArrow" );
			//}}
#endif MODIFY_PVP_ROOM_ITEM_UI

			// dmlee : play channel
			if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
			{
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
				CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );

				CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );

				CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
				CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI

				pButton_KillCount_LeftArrow->SetShowEnable( false, false );
				pButton_KillCount_RightArrow->SetShowEnable( false, false );	

				pButton_PlayTime_LeftArrow->SetShowEnable( false, false );
				pButton_PlayTime_RightArrow->SetShowEnable( false, false );	

#ifdef DUNGEON_ITEM
#ifdef MODIFY_PVP_ROOM_ITEM_UI
				//{{ JHKang // 2010-08-23, 15:49 연습채널 아이템 대전 UI 수정
				pButton_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_ItemMode_RightArrow->SetShowEnable( false, false );

				pButton_Practice_ItemMode_LeftArrow->SetShowEnable( true, true );
				pButton_Practice_ItemMode_RightArrow->SetShowEnable( true, true );

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
				{
					pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
					pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );
				}
				//}}
#endif MODIFY_PVP_ROOM_ITEM_UI
#endif

			}
			else if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
			{
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
				CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );

				CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI


				pButton_KillCount_LeftArrow->SetShowEnable( false, false );
				pButton_KillCount_RightArrow->SetShowEnable( false, false );	

				pButton_PlayTime_LeftArrow->SetShowEnable( false, false );
				pButton_PlayTime_RightArrow->SetShowEnable( false, false );	
#else
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_KILLCOUNT
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
				CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI
				pButton_KillCount_LeftArrow->SetShowEnable( false, false );
				pButton_KillCount_RightArrow->SetShowEnable( false, false );
#endif
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_TIME
#ifndef MODIFY_PVP_ROOM_ITEM_UI			
				CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" )
#endif MODIFY_PVP_ROOM_ITEM_UI
				pButton_PlayTime_LeftArrow->SetShowEnable( false, false );
				pButton_PlayTime_RightArrow->SetShowEnable( false, false );
#endif			
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING

#ifdef DUNGEON_ITEM				
#ifndef MODIFY_PVP_ROOM_ITEM_UI	
				CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
				CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI

				pButton_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_ItemMode_RightArrow->SetShowEnable( false, false );
#ifdef MODIFY_PVP_ROOM_ITEM_UI
				//{{ JHKang // 2010-08-25, 15:49 연습채널 아이템 대전 UI 재수정
				pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
				{
					pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
					pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );
				}
				//}}
#endif MODIFY_PVP_ROOM_ITEM_UI
#endif
			}
			else
			{
#ifdef DUNGEON_ITEM
#ifndef MODIFY_PVP_ROOM_ITEM_UI	
				CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
				CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI

#ifdef MODIFY_PVP_ROOM_ITEM_UI
				//{{ JHKang // 2010-08-25, 15:49 연습채널 아이템 대전 UI 재수정
				pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );
				//}}
#endif MODIFY_PVP_ROOM_ITEM_UI

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
				{
					pButton_ItemMode_LeftArrow->SetShowEnable( false, false );
					pButton_ItemMode_RightArrow->SetShowEnable( false, false );
				}
				else
				{
					pButton_ItemMode_LeftArrow->SetShowEnable( true, true );
					pButton_ItemMode_RightArrow->SetShowEnable( true, true );
				}				
#endif
				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL || m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
				{
					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( true, true );
					}

					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( true, true );
					}
				}
				else
				{
					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( false, false );
					}

					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( false, false );
					}
				}

				pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, true );
				}

				pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, true );
				}
			}
		}
		else
		{
			pControl = pPVP_Room_Start;
			if ( pControl != NULL )
			{
				pControl->SetShowEnable( false, false );
			}

			pControl = pPVP_Room_Ready;
			if ( pControl != NULL )
			{
				pControl->SetShowEnable( true, true );
			}

			pControl = pPVP_Room_Play;
			if ( pControl != NULL )
			{
				pControl->SetShowEnable( false, false );
			}

#ifdef MODIFY_PVP_ROOM_ITEM_UI
			//{{ JHKang // 2010-08-25, 15:49 연습채널 아이템 대전 UI 수정
			CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
			CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );

			CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
			CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );

			CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
			CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );

			CKTDGUIButton* pButton_Practice_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Practice_Item_LeftArrow" );
			CKTDGUIButton* pButton_Practice_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Practice_Item_RightArrow" );
			//}}
#endif MODIFY_PVP_ROOM_ITEM_UI


			// dmlee : play channel
			if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
			{
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
				CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );

				CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI

#ifdef DUNGEON_ITEM

#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
				CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI
				pButton_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_ItemMode_RightArrow->SetShowEnable( false, false );

#ifdef MODIFY_PVP_ROOM_ITEM_UI
				//{{ JHKang // 2010-08-23, 15:49 연습채널 아이템 대전 UI 수정
				pButton_Practice_ItemMode_LeftArrow->SetShowEnable( true, false );
				pButton_Practice_ItemMode_RightArrow->SetShowEnable( true, false );
				//}}
#ifdef FREE_SURVIVAL_ArrowBug_Fix
				if ( CX2PVPRoom::PGT_SURVIVAL == m_pCX2PVPRoom->GetPVPGameType() )
				{
					pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
					pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );
				}
#endif //FREE_SURVIVAL_ArrowBug_Fix
#endif //MODIFY_PVP_ROOM_ITEM_UI
#endif

				pButton_KillCount_LeftArrow->SetShowEnable( false, false );
				pButton_KillCount_RightArrow->SetShowEnable( false, false );	

				pButton_PlayTime_LeftArrow->SetShowEnable( false, false );
				pButton_PlayTime_RightArrow->SetShowEnable( false, false );	

			}
			else if( KPVPChannelInfo::PCC_TOURNAMENT == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
			{
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
				CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );
				
				CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI

				pButton_KillCount_LeftArrow->SetShowEnable( false, false );
				pButton_KillCount_RightArrow->SetShowEnable( false, false );	

				pButton_PlayTime_LeftArrow->SetShowEnable( false, false );
				pButton_PlayTime_RightArrow->SetShowEnable( false, false );	
#else
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_KILLCOUNT
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_KillCount_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
				CKTDGUIButton* pButton_KillCount_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI
				pButton_KillCount_LeftArrow->SetShowEnable( false, false );
				pButton_KillCount_RightArrow->SetShowEnable( false, false );	
#endif
#ifdef TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING_TIME
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_PlayTime_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				CKTDGUIButton* pButton_PlayTime_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI
				pButton_PlayTime_LeftArrow->SetShowEnable( false, false );
				pButton_PlayTime_RightArrow->SetShowEnable( false, false );	
#endif
#endif TEMP_HERO_MATCH_PREVENT_CHANGE_SETTING

#ifdef DUNGEON_ITEM
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
				CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI

				pButton_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_ItemMode_RightArrow->SetShowEnable( false, false );

#ifdef MODIFY_PVP_ROOM_ITEM_UI
				//{{ JHKang // 2010-08-25, 15:49 연습채널 아이템 대전 UI 재수정
				pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
				{
					pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
					pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );
				}
				//}}
#endif MODIFY_PVP_ROOM_ITEM_UI
#endif
			}
			else
			{
#ifdef DUNGEON_ITEM
#ifndef MODIFY_PVP_ROOM_ITEM_UI
				CKTDGUIButton* pButton_ItemMode_LeftArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_LeftArrow" );
				CKTDGUIButton* pButton_ItemMode_RightArrow		= (CKTDGUIButton*) m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Item_RightArrow" );
#endif MODIFY_PVP_ROOM_ITEM_UI	

#ifdef MODIFY_PVP_ROOM_ITEM_UI
				//{{ JHKang // 2010-08-25, 15:49 연습채널 아이템 대전 UI 재수정
				pButton_Practice_ItemMode_LeftArrow->SetShowEnable( false, false );
				pButton_Practice_ItemMode_RightArrow->SetShowEnable( false, false );
				//}}
#endif MODIFY_PVP_ROOM_ITEM_UI

				if( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
				{
					pButton_ItemMode_LeftArrow->SetShowEnable( false, false );
					pButton_ItemMode_RightArrow->SetShowEnable( false, false );
				}
				else
				{
					pButton_ItemMode_LeftArrow->SetShowEnable( true, false );
					pButton_ItemMode_RightArrow->SetShowEnable( true, false );
				}
#endif

				if ( m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL || m_pCX2PVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
				{
					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( true, false );
					}

					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( true, false );
					}
				}
				else
				{
					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( false, false );
					}

					pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Kill_RightArrow" );
					if ( pControl != NULL )
					{
						pControl->SetShowEnable( false, false );
					}
				}

				pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_LeftArrow" );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, false );
				}

				pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Time_RightArrow" );
				if ( pControl != NULL )
				{
					pControl->SetShowEnable( true, false );
				}
			}
		}
	}
	

	if ( m_pCX2PVPRoom->GetRoomState() == CX2Room::RS_PLAY || m_pCX2PVPRoom->GetRoomState() == CX2Room::RS_LOADING )
	{
		pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Start" );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );
		}

		pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Ready" );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );
		}

		pControl = m_pDLGPVPRoomBack->GetControl( L"PVP_Room_Play" );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( true, true );
		}
	}

#ifdef SERV_NEW_PVPROOM_PROCESS
	CKTDGUIButton* pButtonLock = (CKTDGUIButton*)m_pDLGPVPRoomFront->GetControl( L"g_pButtonLock" );
	CKTDGUIButton* pButtonUnlock = (CKTDGUIButton*)m_pDLGPVPRoomFront->GetControl( L"g_pButtonUnLock" );
	CKTDGUIIMEEditBox* pIMEEditRoomName = (CKTDGUIIMEEditBox*)m_pDLGPVPRoomBack->GetControl( L"IMEPVP_Room_Name" );
	CX2Room::SlotData* pMySlot = m_pCX2PVPRoom->GetMySlot();

	if( pButtonLock != NULL )
	{
		if( m_pCX2PVPRoom->GetRoomPublic() == true )
		{
			pButtonLock->SetShow(false);
		}
		else
		{
			pButtonLock->SetShow(true);
		}

		if( m_pCX2PVPRoom->IsRoomMaster() == true )
		{
			pButtonLock->SetEnable(true);
		}
		else
		{
			pButtonLock->SetEnable(false);
		}
	}

	if( pButtonUnlock != NULL )
	{
		if( m_pCX2PVPRoom->GetRoomPublic() == true )
		{
			pButtonUnlock->SetShow(true);
		}
		else
		{
			pButtonUnlock->SetShow(false);
		}

		if( m_pCX2PVPRoom->IsRoomMaster() == true )
		{
			pButtonUnlock->SetEnable(true);
		}
		else
		{
			pButtonUnlock->SetEnable(false);
		}
	}

	if( pIMEEditRoomName != NULL )
	{
		pIMEEditRoomName->SetShow(true);

		if( m_pCX2PVPRoom->IsRoomMaster() == true )
		{
			pIMEEditRoomName->SetEnable(true);
		}
		else
		{
			pIMEEditRoomName->SetEnable(false);
		}
	}
#endif SERV_NEW_PVPROOM_PROCESS

	for ( int i = 0; i < m_pCX2PVPRoom->GetSlotNum(); i++ )
	{
		CX2Room::SlotData* pSlotData = m_pCX2PVPRoom->GetSlotData( i );
		if ( pSlotData == NULL )
			continue;
		
		tempBuff.str( L"" );
		tempBuff << L"PVP_Room_X_" << i + 1;
		pControl = m_pDLGPVPRoomBack->GetControl( tempBuff.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE )
			{
				pControl->SetShow( true );

				if ( m_pCX2PVPRoom->IsRoomMaster( pMySlotData )  == true )
				{
					pControl->SetEnable( true );
				}
			}
		}

	

		tempBuff.str( L"" );
		tempBuff << L"PVP_Room_O_" << i + 1;
		pControl = m_pDLGPVPRoomBack->GetControl( tempBuff.str().c_str() );
		if ( pControl != NULL )
		{
			pControl->SetShowEnable( false, false );

			if ( pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
			{
				
				pControl->SetShow( true );

				if ( pMySlotData != NULL && m_pCX2PVPRoom->IsRoomMaster( pMySlotData ) == true )
				{
					pControl->SetEnable( true );
				}
			}
		}

		pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"PVP_Room_Team_Host" );
		if ( pStatic != NULL && pStatic->GetPicture( i ) != NULL )
		{
			pStatic->GetPicture( i )->SetShow( false );

			if ( pSlotData->GetIsHost() == true && m_pCX2PVPRoom->IsRoomMaster( pSlotData ) == true )
			{
				if ( pSlotData->GetSlotState() != CX2Room::SS_PLAY )
				{
					pStatic->GetPicture( i )->SetShow( true );
				}
			}
		}

		pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"PVP_Room_Team_Play" );
		if ( pStatic != NULL && pStatic->GetPicture( i ) != NULL )
		{
			pStatic->GetPicture( i )->SetShow( false );

			if ( pSlotData->GetSlotState() == CX2Room::SS_PLAY )
			{
				pStatic->GetPicture( i )->SetShow( true );
			}
		}

		pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"PVP_Room_Team_Ready" );
		if ( pStatic != NULL && pStatic->GetPicture( i ) != NULL )
		{
			pStatic->GetPicture( i )->SetShow( false );

			if ( pSlotData->GetIsReady() == true && 
				m_pCX2PVPRoom->IsRoomMaster( pSlotData ) == false &&
				pSlotData->GetSlotState() != CX2Room::SS_PLAY )
			{
				pStatic->GetPicture( i )->SetShow( true );
			}
		}

		pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_PCRoom" );
		if ( pStatic != NULL && pStatic->GetPicture( i ) != NULL )
		{
			pStatic->GetPicture( i )->SetShow( false );
			/*
			if ( pSlotData->GetIsReady() == true && 
				pSlotData->GetIsHost() == false &&
				pSlotData->GetSlotState() != CX2Room::SS_PLAY )
			{
				pStatic->GetPicture( i )->SetShow( true );
			}
			*/

			if( pSlotData->m_SlotState == CX2DungeonRoom::SS_LOADING
				|| pSlotData->m_SlotState == CX2DungeonRoom::SS_PLAY
				|| pSlotData->m_SlotState == CX2DungeonRoom::SS_WAIT )
			{
				if ( pSlotData->m_pUnit != NULL )
				{
					if ( pSlotData->m_pUnit->GetUnitData().m_bIsGameBang == true )
					{
						pStatic->GetPicture( i )->SetShow( true );
					}
				}
			}
		}



		// note!! 임시로 머리위에 엠블렘 아이템 표시 
		CKTDGUIStatic* pStatic_Emblem_200		= (CKTDGUIStatic*) m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_200Emblem" );
		CKTDGUIStatic* pStatic_Emblem_GoldMedal = (CKTDGUIStatic*) m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_Gold_Emblem" ); 

		CKTDGUIControl::CPictureData* pPicture_Emblem_200		= pStatic_Emblem_200->GetPicture(i);
		CKTDGUIControl::CPictureData* pPicture_Emblem_GoldMedal = pStatic_Emblem_GoldMedal->GetPicture(i);

		pPicture_Emblem_200->SetShow( false );
		pPicture_Emblem_GoldMedal->SetShow( false );

		const int MAGIC_EMBLEM_ITEM_ID				= 129785;
		const int MAGIC_EMBLEM_ITEM_ID_GOLD_MEDAL	= 130002;

		if( pSlotData->m_pUnit != NULL )
		{
			if( pSlotData->m_SlotState == CX2DungeonRoom::SS_LOADING
				|| pSlotData->m_SlotState == CX2DungeonRoom::SS_PLAY
				|| pSlotData->m_SlotState == CX2DungeonRoom::SS_WAIT )
			{
				bool bEquippedEmblem = false;

				const CX2Item::ItemTemplet* pItemTempet = g_pData->GetItemManager()->GetItemTemplet( MAGIC_EMBLEM_ITEM_ID );
				if( NULL != pItemTempet )
				{
					CX2Item* pEmblemItem = pSlotData->m_pUnit->GetInventory().GetEquippingItemByEquipPos( pItemTempet->GetEqipPosition(), false );
					if( NULL != pEmblemItem && NULL != pEmblemItem->GetItemTemplet() &&
                        pEmblemItem->GetItemTemplet()->GetItemID() == pItemTempet->GetItemID()
                        )
					{
						pPicture_Emblem_200->SetShow( true );
					}
				}

				if( false == bEquippedEmblem )
				{
					pItemTempet = g_pData->GetItemManager()->GetItemTemplet( MAGIC_EMBLEM_ITEM_ID_GOLD_MEDAL );
					if( NULL != pItemTempet )
					{
						CX2Item* pEmblemItem = pSlotData->m_pUnit->GetInventory().GetEquippingItemByEquipPos( pItemTempet->GetEqipPosition(), false );
						if( NULL != pEmblemItem && NULL !=  pEmblemItem->GetItemTemplet() &&
                            pEmblemItem->GetItemTemplet()->GetItemID() == pItemTempet->GetItemID()
                            )
						{
							pPicture_Emblem_GoldMedal->SetShow( true );
						}
					}
				}
			}
		}


		pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"PVP_Room_Team_Equip" );
		if ( pStatic != NULL && pStatic->GetPicture( i ) != NULL )
		{
			pStatic->GetPicture( i )->SetShow( false );

			if ( pSlotData->GetSlotState() == CX2Room::SS_WAIT && 
				( pSlotData->GetIsPitIn() == true || pSlotData->m_bTrade == true ) )
			{
				pStatic->GetPicture( i )->SetShow( true );
			}
		}


		// 거래중 표시 
		pStatic = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"StaticPVP_Room_Trade" );
		if( pStatic != NULL && pStatic->GetPicture( i ) != NULL )
		{
			pStatic->GetPicture( i )->SetShow( false );
/*
			if ( pSlotData->GetSlotState() == CX2Room::SS_WAIT && pSlotData->m_bTrade == true )
			{
				pStatic->GetPicture( i )->SetShow( true );
			}
*/
		}
	}
	//////////////////////////////////////////////////////////////////////////
	

	if( NULL != g_pData->GetPVPRoom() && 
		( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH ) )
	{
		bool bBlueTeamFull	= true;
		bool bRedTeamFull	= true;

		for ( int i = 0; i < g_pData->GetPVPRoom()->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( i );
			
			if ( i < 4 )
			{
				if ( pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				{
					bRedTeamFull = false;
				}
			}
			else
			{
				if ( pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				{
					bBlueTeamFull = false;
				}	
			}
		}

		CX2Room::SlotData* pMySlotData = g_pData->GetPVPRoom()->GetMySlot();

		if ( m_bWaitTeamChangeAck == true )
		{
			if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" ) != NULL )
				m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" )->SetEnable( false );

			if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" ) != NULL )
				m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" )->SetEnable( false );
		}
		else
		{

			if ( pMySlotData != NULL )
			{
				if ( pMySlotData->m_TeamNum == CX2Room::TN_RED )
				{
					if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" ) != NULL )
						m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" )->SetEnable( false );

					if ( bBlueTeamFull == true )
					{
						if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" ) != NULL )
							m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" )->SetEnable( false ); 
					}
					else
					{
						if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" ) != NULL )
							m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" )->SetEnable( true );
					}
				}
				else
				{
					if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" ) != NULL )
						m_pDLGPVPRoomBack->GetControl( L"RadioButtonBlue" )->SetEnable( false ); 

					if ( bRedTeamFull == true )
					{
						if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" ) != NULL )
							m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" )->SetEnable( false );
					}
					else
					{
						if ( m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" ) != NULL )
							m_pDLGPVPRoomBack->GetControl( L"RadioButtonRed" )->SetEnable( true );
					}
				}
			}

		}

		
		
	}


	
	if( NULL != g_pData->GetPVPRoom() )
	{
		for( int i = 0; i < g_pData->GetPVPRoom()->GetSlotNum(); i++ )
		{
			CX2PVPRoom::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( i );

			if ( pSlotData == NULL )
				continue;

			//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
			switch(pSlotData->m_SlotState)
			{
			case CX2Room::SS_EMPTY:
				{

				} break;
			case CX2Room::SS_CLOSE:
				{

				} break;
			case CX2Room::SS_LOADING:
			case CX2Room::SS_PLAY:
			case CX2Room::SS_WAIT:
				{
					//유닛 찍기
					if( pSlotData->m_pUnitViewer != NULL )
					{
						if( pSlotData->m_pUnitViewer->GetXSkinAnim()->GetState() != CKTDGXSkinAnim::XAS_PLAYING )
						{
							if(	m_pCX2PVPRoom->IsRoomMaster( pSlotData ) == true || pSlotData->m_bReady == false )
							{
#ifdef UNIT_EMOTION
								if( pSlotData->m_SlotState != CX2Room::SS_LOADING
									&& pSlotData->m_SlotState != CX2Room::SS_PLAY 
									&& pSlotData->m_pUnitViewer->GetPlayerEmotion() == false)
#else
								if( pSlotData->m_SlotState != CX2Room::SS_LOADING
									&& pSlotData->m_SlotState != CX2Room::SS_PLAY )
#endif
								{
									pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
								}
							}

							//if( pSlotData->m_SlotState == CX2Room::SS_LOADING
							//	|| pSlotData->m_SlotState == CX2Room::SS_PLAY )
							//{
							//	pSlotData->m_pUnitViewer->PlayAnim( L"LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
							//}

						}
						UnitViewerProcess( pSlotData->m_pUnitViewer, i );
					}
				} break;
			default:
				{
					ASSERT( !"Unexpected Slot State enum" );
				} break;
			}//}}
			
		}
	}


}	

void CX2StatePVPRoom::UnitViewerProcess( CX2UnitViewerUI* pViewer, int slotNum )
{
	//pViewer->SetLightPos( 1000, 1000, -200 );
	//pViewer->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pViewer->SetLightPos( -250, 100, -600 );	// 캐릭터뷰어 라이트 위치 변경

	pViewer->GetMatrix().Move( m_pDLGPVPRoomBack->GetDummyPos(slotNum) );

	//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
	switch(slotNum)
	{
	case 0:
		{
			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f ); //( 1.5f,1.5f,1.5f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 1:
		{

			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f ); //( 1.2f,1.2f,1.2f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 2:
		{
			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 3:
		{
			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f ); //( 1.1f,1.1f,1.1f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 4:
		{
			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f ); //( 1.4f,1.4f,1.4f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(100),D3DXToRadian(0));
		} break;
	case 5:
		{
			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f ); //( 1.1f,1.1f,1.1f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		} break;
	case 6:
		{
			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f ); //( 1.2f,1.2f,1.2f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(100),D3DXToRadian(0));
		} break;
	case 7:
		{
			pViewer->GetMatrix().Scale( 1.3f,1.3f,1.3f ); //( 1.0f,1.0f,1.0f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		} break;
	default:
		{
			ASSERT( !"Unexpected slot Num" );
		} break;
	}//}}
	
	pViewer->OnFrameMove( m_fTime, m_fElapsedTime );
}


bool CX2StatePVPRoom::SetUserBox( CX2Room::SlotData* pSlotData )
{
	if ( pSlotData == NULL )
		return false;
	
	if ( pSlotData->m_pUnit != NULL )
	{

		CKTDGUIDialogType pDialogUserBack = m_PVPRoomUserBoxList[ pSlotData->m_Index ]->pDialogUserBack;
		if ( pDialogUserBack != NULL )
		{
			CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pDialogUserBack->GetControl( L"PVP_Room_Character_Slot" );
			if ( pStatic != NULL )
			{
				pStatic->SetShow( true );
				for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
				{
					if( pStatic->GetPicture(i) != NULL )
					{
						pStatic->GetPicture(i)->SetShow( false );		// note!! 신캐릭터 추가시 스크립트에 추가
					}
					else
					{
						ASSERT(!"NOT GOOD");
					}
				}

				if ( pStatic->GetPicture( pSlotData->m_pUnit->GetUnitTemplet()->m_UnitType - 1 ) != NULL )
				{
					pStatic->GetPicture( pSlotData->m_pUnit->GetUnitTemplet()->m_UnitType - 1 )->SetShow( true );
				}
			}
		}

		m_PVPRoomUserBoxList[ pSlotData->m_Index ]->SetShow( true );


		CKTDGUIDialogType pDialog = m_PVPRoomUserBoxList[ pSlotData->m_Index ]->pDialog;

		if ( pDialog == NULL )
			return false;

		
		//{{ kimhc // 2009-11-04 // 길드 명 추가
#ifdef	GUILD_MANAGEMENT
		// 길드명
		CKTDGUIStatic* pStaticGuildName = static_cast< CKTDGUIStatic* >( pDialog->GetControl( L"Static_GuildName" ) );
		if ( pStaticGuildName != NULL )
			pStaticGuildName->SetString( 0, g_pData->GetGuildManager()->ConvertDisplayGuildName( pSlotData->m_pUnit->GetUnitData().m_wstrGuildName ).c_str() );
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-11-04 // 길드 명 추가
		
		m_PVPRoomUserBoxList[ pSlotData->m_Index ]->unitUID = pSlotData->m_pUnit->GetUID();

		


		D3DXVECTOR3 dummyPos = m_pDLGPVPRoomBack->GetDummyPos( pSlotData->m_Index + 8 );
		pDialog->SetPos( D3DXVECTOR2( dummyPos.x, dummyPos.y ) );
		CKTDGUIStatic* pStaticSlotIndex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_SlotIndex" );
		WCHAR buf[256] = {0};
		_itow( pSlotData->m_Index, buf, 10 );
		pStaticSlotIndex->GetString( 0 )->msg = buf;


		CKTDGUIStatic* pStaticUserInfo2 = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_UserInfo" );
		//_itow( (int)pSlotData->m_pUnit->GetUnitData().m_Level, buf, 10 );
		//pStaticUserInfo->GetString( 1 )->msg = buf;
		pStaticUserInfo2->GetString( 2 )->msg = pSlotData->m_pUnit->GetNickName();


		CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)pDialog->GetControl( L"PVP_Room_UserInfo_Grade" );
		if ( pStaticUserInfo != NULL && pStaticUserInfo->GetPicture(0) != NULL )
		{
#ifdef PVP_SEASON2
			char cRank = pSlotData->m_cRank;
			CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( CX2PVPEmblem::PVP_RANK( cRank ) );
#else
			CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetPVPEmblem() );
#endif
			if ( pPVPEmblemData != NULL )
			{
				CKTDGUIControl::UITextureData* pUITextureData = pStaticUserInfo->GetPicture(0)->pTexture;
				if ( pUITextureData != NULL && 
					( pUITextureData->texName != pPVPEmblemData->m_TextureName || pUITextureData->keyName != pPVPEmblemData->m_TextureKey ) )
				{
					pStaticUserInfo->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
				}	
			}

		}
		
#ifdef SERV_INTEGRATION
		CKTDGUIStatic* pStaticServerGroup = (CKTDGUIStatic*)pDialog->GetControl( L"Static_SERVER" );

#ifdef REMOVE_KR_SERVER_TEXTURE
		if( pStaticServerGroup != NULL )
			pStaticServerGroup->SetShow( false );
#else REMOVE_KR_SERVER_TEXTURE
	
		SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;

		eServerGroupID = (SERVER_GROUP_ID) g_pMain->ExtractServerGroupID(pSlotData->m_pUnit->GetUnitData().m_UnitUID);

		if( eServerGroupID == SGI_SOLES)
		{
			pStaticServerGroup->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Soles");
		}
		else if( eServerGroupID == SGI_GAIA)
		{
			pStaticServerGroup->GetPicture(0)->SetTex( L"DLG_UI_Common_Texture11.tga", L"Gaia");
		}
		else
		{
			;
		}
#endif REMOVE_KR_SERVER_TEXTURE
#endif SERV_INTEGRATION
		if ( pSlotData != g_pData->GetPVPRoom()->GetMySlot() )
		{
			CKTDGUIStatic* pStaticUserInfo_BG = (CKTDGUIStatic*)pDialog->GetControl( L"PVP_Room_UserInfo_BG" );
			if ( pStaticUserInfo_BG != NULL )
				pStaticUserInfo_BG->SetShow( false );

			CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)pDialog->GetControl( L"g_pStaticPVP_Room_UserInfo" );
			if ( pStaticUserInfo != NULL )
				pStaticUserInfo->SetShow( true );
		}
		else
		{
			CKTDGUIStatic* pStaticUserInfo_BG = (CKTDGUIStatic*)pDialog->GetControl( L"PVP_Room_UserInfo_BG" );
			if ( pStaticUserInfo_BG != NULL )
				pStaticUserInfo_BG->SetShow( true );

			CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)pDialog->GetControl( L"g_pStaticPVP_Room_UserInfo" );
			if ( pStaticUserInfo != NULL )
				pStaticUserInfo->SetShow( false );
		}


		CKTDGUIButton* pButtonBan		= (CKTDGUIButton*) pDialog->GetControl( L"PVP_Room_User_exile" );
		CKTDGUIButton* pButtonWhisper	= (CKTDGUIButton*) pDialog->GetControl( L"PVP_Room_User_whisper" );
		
		


		if ( g_pData->GetPVPRoom()->GetMySlot() != NULL )
		{
			if ( g_pData->GetPVPRoom()->GetMySlot()->m_bHost == true || 
				g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
				pButtonBan->SetShow( true );
				pButtonBan->SetEnable( true );
				pButtonBan->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );
			}
			else
			{
				pButtonBan->SetShow( true );
				pButtonBan->SetEnable( false );
				pButtonBan->SetColor( D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) );
			}
		}


		pButtonWhisper->SetShow( true );
		pButtonWhisper->SetEnable( true );
		pButtonWhisper->SetColor( D3DXCOLOR(1.f, 1.f, 1.f, 1.f) );

		if ( pSlotData->m_bMySlot == true )
		{
			pButtonWhisper->SetShow( true );
			pButtonWhisper->SetEnable( false );
			pButtonWhisper->SetColor( D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) );

			pButtonBan->SetShow( true );
			pButtonBan->SetEnable( false );
			pButtonBan->SetColor( D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) );
		}


		CKTDGUIStatic* pStatic_Ping = (CKTDGUIStatic*) pDialog->GetControl( L"PVP_Room_UserInfo_Ping" );
		if( NULL != pStatic_Ping )
		{
			pStatic_Ping->GetPicture(0)->SetShow( false );
			pStatic_Ping->GetPicture(1)->SetShow( false );
			pStatic_Ping->GetPicture(2)->SetShow( false );
			pStatic_Ping->GetPicture(3)->SetShow( false );
			pStatic_Ping->GetPicture(4)->SetShow( false );


			if( pSlotData->m_fElapsedTimeAfterJoinRoom < 5.f )
			{
				pStatic_Ping->GetPicture(4)->SetShow( true );
			}
			else if( pSlotData->m_PingTime < 60 )
			{
				pStatic_Ping->GetPicture(3)->SetShow( true );
			}
			else if( pSlotData->m_PingTime < 120 )
			{
				pStatic_Ping->GetPicture(2)->SetShow( true );
			}
			else if( pSlotData->m_PingTime < 10000 )
			{
				pStatic_Ping->GetPicture(1)->SetShow( true );
			}
			else
			{
				pStatic_Ping->GetPicture(0)->SetShow( true );
			}
		}

		return true;
	}
	else
	{
		m_PVPRoomUserBoxList[ pSlotData->m_Index ]->SetShow( false );
	}

	return false;
}





bool CX2StatePVPRoom::RegisterLuaBind()
{
	CX2StateMenu::RegisterLuaBind();
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pState",		this );

	return true;
}

bool CX2StatePVPRoom::EnterTutorial()
{
	if( NULL == m_pDLGLoadingState )
	{
		m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua" );
		m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );
		// 랜덤 로딩 화면 
		wstringstream wstmStaticName;
		wstmStaticName << L"Static_Loading_BG";
		wstmStaticName << g_pMain->GetTutorialRandomLoadingBG();
		m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );
	}

	g_pMain->SetIsPlayingTutorial( true );
	return Handler_EGS_LEAVE_ROOM_REQ();
}


bool CX2StatePVPRoom::CheckUnitLevelByChannelList()
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
		//{{ 2007. 8. 28  최육사	level -> VP
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

void CX2StatePVPRoom::ToggleCashShop()
{
	m_bReserveCashShopOpen = true;
	Handler_EGS_LEAVE_ROOM_REQ();
}

void CX2StatePVPRoom::ResetNowMap()
{
	if ( m_pCX2MapSelectWindow != NULL )
	{
		if ( m_pCX2PVPRoom != NULL )
		{
			//맵 이미지랑 맵 이름 정보 보여주자 

			if ( m_pCX2PVPRoom->GetWorldIDForUI() == -1 )
			{
				CKTDGUIStatic* pStaticPVPRoomNowMap = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"PVP_Room_Now_Map" );
				if ( pStaticPVPRoomNowMap != NULL && pStaticPVPRoomNowMap->GetPicture(0) != NULL )
				{
					pStaticPVPRoomNowMap->GetPicture(0)->SetTex( L"DLG_New_PVP_Image03.dds", L"MAP_SELECT_IMAGE_ALL_RANDOM" );
				}


				CKTDGUIStatic* pStaticPVPRoomNowMapTitle = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"g_pStaticPVP_Room_Map_Select_TITLE" );
				if ( pStaticPVPRoomNowMapTitle != NULL && pStaticPVPRoomNowMapTitle->GetPicture(0) != NULL )
				{
					pStaticPVPRoomNowMapTitle->GetPicture(0)->SetTex( L"DLG_New_PVP_Image_TITLE.dds", L"MAP_TITLE_ALL_RANDOM" );
					if ( pStaticPVPRoomNowMapTitle->GetPicture(0)->pPoint != NULL )
						pStaticPVPRoomNowMapTitle->GetPicture(0)->pPoint->SetAutoPointByTextureSize();
				}

			}
			else
			{
				MAP_INFO* pMapInfo = m_pCX2MapSelectWindow->GetMapInfo( m_pCX2PVPRoom->GetWorldIDForUI() );

				if ( pMapInfo != NULL )
				{
					CKTDGUIStatic* pStaticPVPRoomNowMap = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"PVP_Room_Now_Map" );
					if ( pStaticPVPRoomNowMap != NULL && pStaticPVPRoomNowMap->GetPicture(0) != NULL )
					{
						pStaticPVPRoomNowMap->GetPicture(0)->SetTex( pMapInfo->m_MapTexName.c_str(), pMapInfo->m_MapTexKeyName.c_str() );
					}

					CKTDGUIStatic* pStaticPVPRoomNowMapTitle = (CKTDGUIStatic*)m_pDLGPVPRoomFront->GetControl( L"g_pStaticPVP_Room_Map_Select_TITLE" );
					if ( pStaticPVPRoomNowMapTitle != NULL && pStaticPVPRoomNowMapTitle->GetPicture(0) != NULL )
					{
						pStaticPVPRoomNowMapTitle->GetPicture(0)->SetTex( pMapInfo->m_MapNameTexName.c_str(), pMapInfo->m_MapNameTexKeyName.c_str() );
						if ( pStaticPVPRoomNowMapTitle->GetPicture(0)->pPoint != NULL )
							pStaticPVPRoomNowMapTitle->GetPicture(0)->pPoint->SetAutoPointByTextureSize();
					}
				}
			}
		}
	}
	
}

void CX2StatePVPRoom::OnChatModeOpen()
{	
	if( g_pChatBox->GetOpenChatWindow() == false )
	{
		//if( m_pCharacterRoom->GetOpen() == false )
#ifndef NEW_SKILL_TREE
		if( false == g_pInstanceData->GetSkillTreeUI()->GetShow() )
#endif
		{	
			if( g_pData->GetPartyManager()->GetMyPartyData()->m_iPartyUID <= 0 ) 
			{
				g_pChatBox->ChangeChatWindowAndIMEEditBox( CX2ChatBox::CWM_TOTAL );
			}

			g_pChatBox->OpenChatWindow();
		}
	}
	else
	{
		g_pChatBox->SetFocus();
	}
}

/*virtual*/
void CX2StatePVPRoom::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	if( NULL == m_pCX2PVPRoom )
		return; 

	CX2Room::SlotData* pkSlotData = m_pCX2PVPRoom->GetSlotDataByUnitUID( iUnitUID_ );
	if ( pkSlotData == NULL )
		return;

	int iSlotIndex = pkSlotData->m_Index;
	if( iSlotIndex < 0 )
		return;

#ifdef UNIT_EMOTION
	bool bCommandEmotion = g_pChatBox->IsEmotionID(pWstrMsg_);
	if( g_pChatBox != NULL && bCommandEmotion == false )
#endif
	{
		CX2TalkBoxManagerImp::TalkBox talkBox;
		talkBox.m_OwnerUnitUID		= iUnitUID_;

		talkBox.m_vPos				= D3DXVECTOR2( m_pDLGPVPRoomFront->GetDummyPos( iSlotIndex ).x, 
			m_pDLGPVPRoomFront->GetDummyPos( iSlotIndex ).y );
		talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;

		talkBox.m_wstrTalkContent	= pWstrMsg_;
		talkBox.m_bTraceUnit		= false;
		talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
		talkBox.m_coTextColor		= coTextColor_;
		talkBox.m_BackTexColor		= coBackColor_;
		
		talkBox.m_bUseOutLine		 = true;
		if( coOutColor_ == 0x00000000 )	//	입력 없을 경우는 글씨색 그대로 쓰기
		{
			talkBox.m_bUseOutLine = false;
		}

#ifdef FIX_CRASH_130926
		/// 만약, UISetting() 에서 토크 박스 객체를 못 얻어 와서,강제로 퇴장 당하기 전에 
		/// 해당 로직을 타게 되면 크래시가 발생할 수 있을 것으로 보임
		/// 아래 객체에 대한 NULL 처리 추가
		if ( NULL != m_pTalkBoxMgr )
#endif //FIX_CRASH_130926
			m_pTalkBoxMgr->Push( talkBox );
	}


#ifdef UNIT_EMOTION
	// 이모션 연결시켜준다.
	if( g_pChatBox != NULL && m_bEnableMotionChange == true && NULL != pkSlotData && NULL != pkSlotData->m_pUnitViewer )
	{
		// 앉은 상태에서는 채팅필터링 먹히지 않도록 한다.(명령어로만 이모션동작취함)
		if( bCommandEmotion == false && pkSlotData->m_pUnitViewer->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
			return;

		CX2Unit::EMOTION_TYPE eId = CX2Unit::ET_NONE;
		wstring wstrName = L"";
		g_pChatBox->GetEmotionIdAndName(eId, wstrName, pWstrMsg_);
		pkSlotData->m_pUnitViewer->PlayEmotion( eId, wstrName );
	}
#endif
	
}

void CX2StatePVPRoom::InitPvpRoomUI()
{

	CKTDGUIDialogManager *pDialogManager = g_pKTDXApp->GetDGManager()->GetDialogManager();
	if( pDialogManager == NULL )
		return;

	CX2PVPRoom *pPVPRoom = g_pData->GetPVPRoom();
	if( pPVPRoom == NULL )
		return;

	CKTDGUIDialogType pDialog = pDialogManager->GetDialog( L"DLG_PVP_Room_Front" );
	CKTDGUIDialogType pDialogBack = NULL;

	if ( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH ) // PVP_GAME_TYPE["PGT_TEAM_DEATH"]
		pDialogBack = pDialogManager->GetDialog( L"DLG_PVP_Room_Team_Back" );
	else
		pDialogBack = pDialogManager->GetDialog( L"DLG_PVP_Room_Death_Back" );


	//시간 과 킬수 실시간 변경?
	WCHAR wstrBuff[256] = {0,};
	CKTDGUIControl::UIStringData* pString = NULL;

	if( pDialog != NULL && pPVPRoom != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_TIME" );
		pString = pStatic->GetString(0);
		float playTime = pPVPRoom->GetPlayTimeLimit();	
		//wsprintf( wstrBuff, L"%d", (int)playTime );
		StringCchPrintf( wstrBuff, 256, L"%d", (int)playTime );
		pString->msg = wstrBuff;
	
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_KILL" );
		pString = pStatic->GetString(0);
		int winKillNum = pPVPRoom->GetWinKillNum();
		//wsprintf( wstrBuff, L"%d", winKillNum );
		StringCchPrintf( wstrBuff, 256, L"%d", winKillNum );
		pString->msg = wstrBuff;

#ifdef DUNGEON_ITEM
#ifdef MODIFY_PVP_ROOM_ITEM_UI
		//{{ JHKang // 2010-08-23, 15:49 연습채널 아이템 대전 UI 수정
		if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
		{
			pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_ITEM" );
			pStatic->SetShowEnable( false, false );
			pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_Practice_ITEM" );
			pStatic->SetShowEnable( true, true );
		}
		else
		{
			pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_Practice_ITEM" );
			pStatic->SetShowEnable( false, false );
			pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_ITEM" );
			pStatic->SetShowEnable( true, true );
		}
		//}}
#endif MODIFY_PVP_ROOM_ITEM_UI
		if(pPVPRoom->GetItemMode() == true)
		{
			// 아이템전
			pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_4817 ); // 아이템 전
		}
		else
		{
			// 파워전
			pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_4818 ); // 파워 전
		}
#endif
	}
		
#ifdef DUNGEON_ITEM
	CKTDGUIButton* pButtonR = (CKTDGUIButton*)pDialogBack->GetControl( L"PVP_Room_Item_RightArrow" );
	CKTDGUIButton* pButtonL = (CKTDGUIButton*)pDialogBack->GetControl( L"PVP_Room_Item_LeftArrow" );

	if( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
	{		
		pButtonR->SetShowEnable(true, true);	
		pButtonL->SetShowEnable(true, true);
	}
	else
	{
		pButtonR->SetShowEnable(false, false);	
		pButtonL->SetShowEnable(false, false);
	}		
#endif

	if( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM )
	{
		//일반 팀전일 경우
		CKTDGUIButton* pButton = NULL;
		pButton = (CKTDGUIButton*)pDialogBack->GetControl( L"PVP_Room_Kill_RightArrow" );
		pButton->SetShowEnable(false, false);

		pButton = (CKTDGUIButton*)pDialogBack->GetControl( L"PVP_Room_Kill_LeftArrow" );
		pButton->SetShowEnable(false, false);
        
		CKTDGUIStatic *pStatic = NULL;
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_KILL" );
		pStatic->SetShow( false );
		
		pStatic = (CKTDGUIStatic*)pDialogBack->GetControl( L"StaticPVP_Room_GameType" );
		pStatic->GetPicture(0)->SetShow(true);
		pStatic->GetPicture(1)->SetShow(false);
		pStatic->GetPicture(2)->SetShow(false);

		CX2Room::SlotData *pMySlot = pPVPRoom->GetMySlot();

		CKTDGUIRadioButton *pRadioButton = NULL;
		pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonRed" );
		pRadioButton->SetChecked(false);

		pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonBlue" );
		pRadioButton->SetChecked(false);

		if( pMySlot->GetTeamNum() == CX2Room::TN_RED )
		{
			pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonRed" );
			pRadioButton->SetChecked(true);
		}
		else
		{
			pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonBlue" );
			pRadioButton->SetChecked(true);
		}
	}	
	else
	{
		//서바이벌 모드일 경우
		CKTDGUIStatic *pStatic = NULL;
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"StaticPVP_Room_KILL" );
		pStatic->SetShow(true);

		if( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
		{
			pStatic = (CKTDGUIStatic*)pDialogBack->GetControl( L"StaticPVP_Room_GameType" );
			pStatic->GetPicture(0)->SetShow(false);
			pStatic->GetPicture(1)->SetShow(true);			
			pStatic->GetPicture(2)->SetShow(false);

			CX2Room::SlotData *pMySlot = pPVPRoom->GetMySlot();

			CKTDGUIRadioButton *pRadioButton = NULL;
			pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonRed" );
			pRadioButton->SetChecked(false);
			pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonBlue" );
			pRadioButton->SetChecked(false);

			if( pMySlot->GetTeamNum() == CX2Room::TN_RED )
			{
				pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonRed" );
				pRadioButton->SetChecked(true);
			}
			else
			{
				pRadioButton = (CKTDGUIRadioButton*)pDialogBack->GetControl( L"RadioButtonBlue" );
				pRadioButton->SetChecked(true);
			}
		}
		else
		{
			CKTDGUIStatic *pStatic = NULL;
			pStatic = (CKTDGUIStatic*)pDialogBack->GetControl( L"StaticPVP_Room_GameType" );
			pStatic->GetPicture(0)->SetShow(false);
			pStatic->GetPicture(1)->SetShow(false);
			pStatic->GetPicture(2)->SetShow(true);			
		}
	}


	if( pPVPRoom->GetRoomPublic() == true )
	{
#ifdef SERV_NEW_PVPROOM_PROCESS
		CKTDGUIButton* pButtonLock = (CKTDGUIButton*)pDialog->GetControl( L"g_pButtonLock" );
		CKTDGUIButton* pButtonUnlock = (CKTDGUIButton*)pDialog->GetControl( L"g_pButtonUnLock" );
		pButtonLock->SetShowEnable(false, false);

		if( pPVPRoom->IsRoomMaster() == true )
		{
			pButtonUnlock->SetShowEnable(true, true);
		}
		else
		{
			pButtonUnlock->SetShowEnable(true, false);
		}
#else
		CKTDGUIStatic *pStatic = NULL;
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"g_pStaticLock" );
		pStatic->SetShow(false);
#endif SERV_NEW_PVPROOM_PROCESS
	}
	else
	{
#ifdef SERV_NEW_PVPROOM_PROCESS
		CKTDGUIButton* pButtonLock = (CKTDGUIButton*)pDialog->GetControl( L"g_pButtonLock" );
		CKTDGUIButton* pButtonUnlock = (CKTDGUIButton*)pDialog->GetControl( L"g_pButtonUnLock" );
		pButtonUnlock->SetShowEnable(false, false);

		if( pPVPRoom->IsRoomMaster() == true )
		{
			pButtonLock->SetShowEnable(true, true);
		}
		else
		{
			pButtonLock->SetShowEnable(true, false);
		}
#else
		CKTDGUIStatic *pStatic = NULL;
		pStatic = (CKTDGUIStatic*)pDialog->GetControl( L"g_pStaticLock" );
		pStatic->SetShow(true);
#endif SERV_NEW_PVPROOM_PROCESS
	}



	for ( int i = 0; i < (int)g_pX2Room->GetSlotNum(); i++ )
	{
		CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
		if ( pSlotData == NULL || pSlotData->m_pUnit == NULL || pSlotData->m_pUnitViewer == NULL )
			continue;

		pSlotData->m_pUnitViewer->SetNowEmotionId(CX2Unit::ET_NONE);
	}
}


CX2StatePVPRoom::CX2MapSelectWindow::CX2MapSelectWindow()
: m_pDLGBack( NULL )
, m_pDLGSelected( NULL )
#ifdef FREE_SELECT_PVP_MAP
, m_pDLGPVPMapSelectionWindowForDebug( NULL )
#endif FREE_SELECT_PVP_MAP

{
	OpenScript( L"DLG_PVP_Map_List.lua" );	
	
	m_bOpen = false;
	m_SelectedMapWorldID = 0;
}

CX2StatePVPRoom::CX2MapSelectWindow::~CX2MapSelectWindow()
{
	for ( int i = 0; i < (int)m_vecpMapInfo.size(); i++ )
	{
		SAFE_DELETE( m_vecpMapInfo[i] );
	}
	m_vecpMapInfo.clear();

	for ( int i = 0; i < (int)m_vecpDLGMap.size(); i++ )
	{
		SAFE_DELETE_DIALOG( m_vecpDLGMap[i] );
	}
	m_vecpDLGMap.clear();
	m_pDLGSelected = NULL;

	SAFE_DELETE_DIALOG( m_pDLGBack );

#ifdef FREE_SELECT_PVP_MAP
	SAFE_DELETE_DIALOG( m_pDLGPVPMapSelectionWindowForDebug );	
#endif FREE_SELECT_PVP_MAP

}

void CX2StatePVPRoom::CX2MapSelectWindow::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bOpen == false )
		return;

	for ( int i = 0; i < (int)m_vecpMapInfo.size(); i++ )
	{
		CX2StatePVPRoom::MAP_INFO* pMAP_INFO = m_vecpMapInfo[i];
		if ( pMAP_INFO != NULL )
		{
			if ( i >= 0 && i < (int)m_vecpDLGMap.size() )
			{
				CKTDGUIDialogType pCKTDGUIDialog = m_vecpDLGMap[i];
				if ( pCKTDGUIDialog != NULL )
				{
					

					CKTDGUIControl* pControl = (CKTDGUIButton*)pCKTDGUIDialog->GetControl( L"g_pStaticMAP_SMALL_IMAGE_MESSAGE_HOT" );
					if ( pControl != NULL )
					{
						pControl->SetShow( pMAP_INFO->m_bHot );
						if ( pControl->GetIsMoving() == false )
						{
							pControl->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.5f );
							pControl->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,0,0,1), 0.2f );
							pControl->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.5f );
						}
					}
				}
			}
		}
	}
}

void CX2StatePVPRoom::CX2MapSelectWindow::Open( int selectedMapWorldID )
{
	if ( selectedMapWorldID == 0 )
		return;

	m_SelectedMapWorldID = selectedMapWorldID;

	m_bOpen = true;
	if ( m_pDLGBack == NULL )
	{
		m_pDLGBack = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Map_Select_Window.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBack );
	}
	m_pDLGBack->SetShowEnable( true, true );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGBack, true );

	if ( m_vecpDLGMap.empty() == true )
	{
		for ( int i = 0; i < PVP_MAP_SHOW_LIST; i++ )
		{
			CKTDGUIDialogType pCKTDGUIDialog = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Map_Select_Window_Instance.lua" );
			pCKTDGUIDialog->SetPos( D3DXVECTOR2( pCKTDGUIDialog->GetDummyPos(i).x, pCKTDGUIDialog->GetDummyPos(i).y ) );
			pCKTDGUIDialog->SetShowEnable( false, false );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pCKTDGUIDialog );
			m_vecpDLGMap.push_back( pCKTDGUIDialog );
		}
	}
	
	for ( int i = 0; i < (int)m_vecpDLGMap.size(); i++ )
	{
		CKTDGUIDialogType pCKTDGUIDialog = m_vecpDLGMap[i];
		if ( pCKTDGUIDialog != NULL )
		{
			pCKTDGUIDialog->SetShowEnable( false, false );
			
		}
	}


	ResetSelectedMapButton();




#ifdef FREE_SELECT_PVP_MAP

	OpenPVPMapSelectWindowForDebug( true );


#endif FREE_SELECT_PVP_MAP

}

void CX2StatePVPRoom::CX2MapSelectWindow::Close()
{
	m_bOpen = false;

	for ( int i = 0; i < (int)m_vecpDLGMap.size(); i++ )
	{
		CKTDGUIDialogType pCKTDGUIDialog = m_vecpDLGMap[i];
		if ( pCKTDGUIDialog != NULL )
		{
			pCKTDGUIDialog->SetShowEnable( false, false );
		}
	}

	if ( m_pDLGBack != NULL )
		m_pDLGBack->SetShowEnable( false, false );


#ifdef FREE_SELECT_PVP_MAP

	OpenPVPMapSelectWindowForDebug( false );

#endif FREE_SELECT_PVP_MAP

}

bool CX2StatePVPRoom::CX2MapSelectWindow::GetOpen()
{
	return m_bOpen;
}

int CX2StatePVPRoom::CX2MapSelectWindow::GetSelectedMapWorldID()
{
	return m_SelectedMapWorldID;
}
void CX2StatePVPRoom::CX2MapSelectWindow::SetSelectMapByWorldID( int worldID )
{
	if ( worldID == 0 )
		return;

	bool bCheck = false;
	//일단 유효한 이름인지 확인하자.
	if ( worldID == -1 )
	{
		bCheck = true;
	}
	else
	{
		for ( int i = 0; i < (int)m_vecpMapInfo.size(); i++ )
		{
			CX2StatePVPRoom::MAP_INFO* pMAP_INFO = m_vecpMapInfo[i];
			if ( pMAP_INFO != NULL )
			{
				if ( pMAP_INFO->m_WorldID == worldID )
				{
					bCheck = true;
					break;
				}
			}
		}
	}
	if ( bCheck == false )
		return;


	m_SelectedMapWorldID = worldID;

	//모든 버튼의 선택 상태를 일단 없애주고..
	


	//진짜 선택된 버튼의 상태를 오버 상태로 전환~ 고고고~~~
	//올랜덤.
	ResetSelectedMapButton();
}

void CX2StatePVPRoom::CX2MapSelectWindow::AddMapInfo_LUA()
{

//#ifdef LUA_TEST
//
//	lua_tinker::table luaTable( g_pKTDXApp->GetLuaBinder()->GetLuaState(), -1 );
//
//	CX2StatePVPRoom::MAP_INFO* pNew_MAP_INFO = new CX2StatePVPRoom::MAP_INFO();
//
//	LUA_GET_( luaTable, "m_WorldID",				pNew_MAP_INFO->m_WorldID,				0 );
//
//	LUA_GET_( luaTable, "m_bHot",					pNew_MAP_INFO->m_bHot,					false );
//	LUA_GET_( luaTable, "m_bNew",					pNew_MAP_INFO->m_bNew,					false );
//
//	LUA_GET_( luaTable, "m_MapName",				pNew_MAP_INFO->m_MapName,				L"" );	
//	LUA_GET_( luaTable, "m_MapTexName",				pNew_MAP_INFO->m_MapTexName,			L"" );
//	LUA_GET_( luaTable, "m_MapTexKeyName",			pNew_MAP_INFO->m_MapTexKeyName,			L"" );
//
//	LUA_GET_( luaTable, "m_MapNameTexName",			pNew_MAP_INFO->m_MapNameTexName,		L"" );
//	LUA_GET_( luaTable, "m_MapNameTexKeyName",		pNew_MAP_INFO->m_MapNameTexKeyName,		L"" );
//
//
//#else

	CX2StatePVPRoom::MAP_INFO* pNew_MAP_INFO = new CX2StatePVPRoom::MAP_INFO();
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE( luaManager, "m_WorldID",				pNew_MAP_INFO->m_WorldID,			0 );

	LUA_GET_VALUE( luaManager, "m_bHot",				pNew_MAP_INFO->m_bHot,				false );
	LUA_GET_VALUE( luaManager, "m_bNew",				pNew_MAP_INFO->m_bNew,				false );

    int iStringIndex;
	LUA_GET_VALUE( luaManager, "m_MapName",				iStringIndex,           			STR_ID_EMPTY );
    pNew_MAP_INFO->m_MapName = GET_STRING( iStringIndex );
	LUA_GET_VALUE( luaManager, "m_MapTexName",			pNew_MAP_INFO->m_MapTexName,		L"" );
	LUA_GET_VALUE( luaManager, "m_MapTexKeyName",		pNew_MAP_INFO->m_MapTexKeyName,		L"" );

	LUA_GET_VALUE( luaManager, "m_MapNameTexName",		pNew_MAP_INFO->m_MapNameTexName,	L"" );
	LUA_GET_VALUE( luaManager, "m_MapNameTexKeyName",	pNew_MAP_INFO->m_MapNameTexKeyName,	L"" );

//#endif // LUA_TEST


	if ( pNew_MAP_INFO->m_bNew == true )
	{
		for ( int i = 0; i < (int)m_vecpMapInfo.size(); i++ )
		{
			CX2StatePVPRoom::MAP_INFO* pTemp_MAP_INFO = m_vecpMapInfo[i];
			if ( pTemp_MAP_INFO->m_bNew == false )
			{
				m_vecpMapInfo.insert( m_vecpMapInfo.begin() + i, pNew_MAP_INFO );
				return;
			}
		}
	}

	m_vecpMapInfo.push_back( pNew_MAP_INFO );
}

CX2StatePVPRoom::MAP_INFO* CX2StatePVPRoom::CX2MapSelectWindow::GetMapInfo( int worldID )
{
	for ( int i = 0; i < (int)m_vecpMapInfo.size(); i++ )
	{
		CX2StatePVPRoom::MAP_INFO* pMAP_INFO = m_vecpMapInfo[i];
		if ( pMAP_INFO->m_WorldID == worldID )
			return pMAP_INFO;
	}

	return NULL;
}

void CX2StatePVPRoom::CX2MapSelectWindow::ChangeSelectedMapDLGFirstSeq()
{
	if ( m_pDLGSelected != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGSelected, true );
	}
}

int CX2StatePVPRoom::CX2MapSelectWindow::GetAllRandomMap()
{
	if ( m_vecpMapInfo.empty() == true )
		return 0;

	int mapID = 0;

	int index = RandomNumber( 0, m_vecpMapInfo.size() - 1 );

	CX2StatePVPRoom::MAP_INFO* pMAP_INFO = m_vecpMapInfo[index];
	if ( pMAP_INFO != NULL )
	{
		return pMAP_INFO->m_WorldID;
	}

	return 0;
}

bool CX2StatePVPRoom::CX2MapSelectWindow::OpenScript( const WCHAR* pFileName )
{
	for ( int i = 0; i < (int)m_vecpMapInfo.size(); i++ )
	{
		SAFE_DELETE( m_vecpMapInfo[i] );
	}
	m_vecpMapInfo.clear();

	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pMapSelectWindow", this );

    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR131, pFileName );

		return false;
    }

	return true;
}



void CX2StatePVPRoom::CX2MapSelectWindow::ResetSelectedMapButton()
{
	// -1이면 올랜덤으로
	if ( m_SelectedMapWorldID == -1 )
	{
		// 지금은 UI가 없다 ㅠ
		//
		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGBack->GetControl( L"g_pButton_ALL_RANDOM_BUTTON" );
		if ( pButton != NULL )
			pButton->SetOverStateAtNormal( true );

		CKTDGUIControl* pControl = (CKTDGUIControl*)m_pDLGBack->GetControl( L"g_pStaticMAP_SELECT_IMAGE_ALL_RANDOM" );
		if ( pControl != NULL )
			pControl->SetShowEnable( true, true );

		pControl = (CKTDGUIControl*)m_pDLGBack->GetControl( L"g_pStaticMAP_TITLE_ALL_RANDOM" );
		if ( pControl != NULL )
			pControl->SetShowEnable( true, true );

		
	}
	else
	{
		CKTDGUIButton* pButton = (CKTDGUIButton*)m_pDLGBack->GetControl( L"g_pButton_ALL_RANDOM_BUTTON" );
		if ( pButton != NULL )
			pButton->SetOverStateAtNormal( false );

		CKTDGUIControl* pControl = (CKTDGUIControl*)m_pDLGBack->GetControl( L"g_pStaticMAP_SELECT_IMAGE_ALL_RANDOM" );
		if ( pControl != NULL )
			pControl->SetShowEnable( false, false );

		pControl = (CKTDGUIControl*)m_pDLGBack->GetControl( L"g_pStaticMAP_TITLE_ALL_RANDOM" );
		if ( pControl != NULL )
			pControl->SetShowEnable( false, false );

	}

	//일단 맵 이미지 꺼주자.
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"g_pStaticMAP_SELECT_IMAGE" );
	if ( pStatic != NULL )
	{
		pStatic->SetShowEnable( false, false );
	}

	pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"g_pStaticMAP_TITLE" );
	if ( pStatic != NULL )
	{
		pStatic->SetShowEnable( false, false );
	}

	CKTDGUIDialogType pDialogToChangeSeq = NULL;

	for ( int i = 0; i < (int)m_vecpMapInfo.size(); i++ )
	{
		CX2StatePVPRoom::MAP_INFO* pMAP_INFO = m_vecpMapInfo[i];
		if ( pMAP_INFO != NULL )
		{
			if ( i >= 0 && i < (int)m_vecpDLGMap.size() )
			{
				CKTDGUIDialogType pCKTDGUIDialog = m_vecpDLGMap[i];
				if ( pCKTDGUIDialog != NULL )
				{
					pCKTDGUIDialog->ClearDummyInt();
					pCKTDGUIDialog->AddDummyInt( pMAP_INFO->m_WorldID );
					//여기서 이제 맵 정보에 따라 맵 텍스쳐, HOT인지 NEW인지 표시해주자 그리고.. 
					//현재 선택된 맵이면 버튼을 노멀상태에서 오버상태로 변경하고.
					pCKTDGUIDialog->SetShowEnable( true, true );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pCKTDGUIDialog, true );

					CKTDGUIButton* pButton = (CKTDGUIButton*)pCKTDGUIDialog->GetControl( L"g_pButton_MAP_SELECT_BUTTON" );
					if ( pButton != NULL )
					{
						pButton->SetGuideDescOffsetPos( D3DXVECTOR2(0,-40) );
						pButton->SetGuideDesc( pMAP_INFO->m_MapName.c_str() );
					}
					
					if ( m_SelectedMapWorldID == pCKTDGUIDialog->GetDummyInt(0) )
					{
						pDialogToChangeSeq = pCKTDGUIDialog;

						CKTDGUIButton* pButton = (CKTDGUIButton*)pCKTDGUIDialog->GetControl( L"g_pButton_MAP_SELECT_BUTTON" );
						if ( pButton != NULL )
						{
							pButton->SetDownStateAtNormal( true );
						}

						CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"g_pStaticMAP_SELECT_IMAGE" );
						if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
						{
							pStatic->SetShowEnable( true, true );
							pStatic->GetPicture(0)->SetTex( pMAP_INFO->m_MapTexName.c_str(), pMAP_INFO->m_MapTexKeyName.c_str() );
						}

						pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"g_pStaticMAP_TITLE" );
						if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
						{
							pStatic->SetShowEnable( true, true );
							pStatic->GetPicture(0)->SetTex( pMAP_INFO->m_MapNameTexName.c_str(), pMAP_INFO->m_MapNameTexKeyName.c_str() );
							if ( pStatic->GetPicture(0)->pPoint != NULL )
							{
								pStatic->GetPicture(0)->pPoint->SetAutoPointByTextureSize();
							}
						}
					}
					else
					{
						CKTDGUIButton* pButton = (CKTDGUIButton*)pCKTDGUIDialog->GetControl( L"g_pButton_MAP_SELECT_BUTTON" );
						if ( pButton != NULL )
						{
							pButton->SetDownStateAtNormal( false );
						}
					}

					CKTDGUIControl* pControl = pCKTDGUIDialog->GetControl( L"g_pStaticMAP_SMALL_IMAGE_MESSAGE_NEW" );
					if ( pControl != NULL )
						pControl->SetShow( pMAP_INFO->m_bNew );
					
					pControl = (CKTDGUIButton*)pCKTDGUIDialog->GetControl( L"g_pStaticMAP_SMALL_IMAGE_MESSAGE_HOT" );
					if ( pControl != NULL )
						pControl->SetShow( pMAP_INFO->m_bHot );


					CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pCKTDGUIDialog->GetControl( L"g_pStaticMAP_SMALL_IMAGE" );
					if ( pStatic != NULL )
					{
						if ( pStatic->GetPicture(0) != NULL )
						{
							pStatic->GetPicture(0)->SetTex( pMAP_INFO->m_MapTexName.c_str(), pMAP_INFO->m_MapTexKeyName.c_str() );
						}
					}
					
				}
			}
		}
	}

	m_pDLGSelected = pDialogToChangeSeq;
	ChangeSelectedMapDLGFirstSeq();
}





#ifdef FREE_SELECT_PVP_MAP





	void CX2StatePVPRoom::CX2MapSelectWindow::OpenPVPMapSelectWindowForDebug( bool bOpen )
	{
		if( true == bOpen )
		{
			if ( m_pDLGPVPMapSelectionWindowForDebug == NULL )
			{
				m_pDLGPVPMapSelectionWindowForDebug = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_PVP_MAP_SELECT_FOR_DEBUG.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPVPMapSelectionWindowForDebug );	

			}

			if ( m_pDLGPVPMapSelectionWindowForDebug != NULL )
			{
				m_pDLGPVPMapSelectionWindowForDebug->SetShowEnable( true, true );
				UpdatePVPMapSelectWindowForDebug();
			}
		}
		else
		{
			if ( m_pDLGPVPMapSelectionWindowForDebug != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGPVPMapSelectionWindowForDebug, NULL, false );
			m_pDLGPVPMapSelectionWindowForDebug = NULL;
		}
	}


	void CX2StatePVPRoom::CX2MapSelectWindow::UpdatePVPMapSelectWindowForDebug()
	{
		if ( m_pDLGPVPMapSelectionWindowForDebug == NULL )
			return; 

		CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGPVPMapSelectionWindowForDebug->GetControl( L"PVP_MAP_WORLD_ID_LIST" );
		if( NULL == pListBox )
			return;

		pListBox->RemoveAllItems();

		const std::vector<CX2World::WorldData*>& vecWorldData = g_pData->GetWorldManager()->GetWorldDataList();
		BOOST_TEST_FOREACH( const CX2World::WorldData*, pWorldData, vecWorldData )
		{
			pListBox->AddItem( pWorldData->worldName.c_str(), (void*) pWorldData->worldID );
		}

	}




#endif FREE_SELECT_PVP_MAP



#ifdef SERVER_PVP_BASE_DEFENCE_TEST

	bool CX2StatePVPRoom::IsPVPGameOptionOpened()
	{
		if( NULL != m_pDLGPVPGameOption )
		{
			return m_pDLGPVPGameOption->GetShow();
		}
		else
		{
			return false;
		}
	}



	void CX2StatePVPRoom::OpenPVPGameOption( bool bOpen )
	{
		if( true == bOpen )
		{
			if ( m_pDLGPVPGameOption == NULL )
			{
				m_pDLGPVPGameOption = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_PVP_GAME_OPTION_FOR_DEBUG.LUA" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGPVPGameOption );	

			}

			if ( m_pDLGPVPGameOption != NULL )
			{
				m_pDLGPVPGameOption->SetShowEnable( true, true );
				UpdatePVPGameOption();
			}
		}
		else
		{
			if ( m_pDLGPVPGameOption != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGPVPGameOption, NULL, false );
			m_pDLGPVPGameOption = NULL;
		}
	}



	void CX2StatePVPRoom::UpdatePVPGameOption()
	{
		if ( m_pDLGPVPGameOption == NULL )
			return; 


		if( NULL != m_pCX2PVPRoom )
		{
			CKTDGUICheckBox* pCheckBaseDefence = (CKTDGUICheckBox*) m_pDLGPVPGameOption->GetControl( L"CHECK_BASE_DEFENCE" );
			if( NULL != pCheckBaseDefence )
			{
				pCheckBaseDefence->SetChecked( m_pCX2PVPRoom->GetPVPGameOption().m_bBaseDefence );
			}


			CKTDGUICheckBox* pCheckBossCombat = (CKTDGUICheckBox*) m_pDLGPVPGameOption->GetControl( L"CHECK_BOSS_COMBAT" );
			if( NULL != pCheckBossCombat )
			{
				pCheckBossCombat->SetChecked( m_pCX2PVPRoom->GetPVPGameOption().m_bBossCombat );
			}
		}

	}
#endif SERVER_PVP_BASE_DEFENCE_TEST

#ifdef SERV_NEW_PVPROOM_PROCESS
void CX2StatePVPRoom::OpenSetRoomPasswordDLG()
{
	m_pDLGSetPasswordRoom->SetShowEnable(true, true);

	CKTDGUIIMEEditBox* pIMEEditBoxPassword = (CKTDGUIIMEEditBox*)m_pDLGSetPasswordRoom->GetControl( L"IME_EditPassword" );
	pIMEEditBoxPassword->ClearText();
	pIMEEditBoxPassword->SetShowEnable( true, true );

	m_pDLGSetPasswordRoom->RequestFocus( pIMEEditBoxPassword );
}

void CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_REQ(bool bPublic, std::wstring wstrPassword)
{
	KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ kPacket;
	kPacket.m_bPublic = bPublic;
	kPacket.m_wstrPassword = wstrPassword;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PVP_ROOM_PUBLIC_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_PVP_ROOM_PUBLIC_ACK );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_PVP_ROOM_PUBLIC_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			m_pCX2PVPRoom->SetRoomPublic( kEvent.m_bPublic );
		}
	}

	return true;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ROOM_PUBLIC_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if(m_pCX2PVPRoom != NULL)
	{
		m_pCX2PVPRoom->SetRoomPublic( kEvent.m_bPublic );
	}

	return true;
}

void CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ROOM_NAME_REQ(std::wstring wstrRoomName)
{
	KEGS_CHANGE_PVP_ROOM_NAME_REQ kPacket;
	kPacket.m_wstrRoomName = wstrRoomName;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PVP_ROOM_NAME_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_PVP_ROOM_NAME_ACK );
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ROOM_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ROOM_NAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_CHANGE_PVP_ROOM_NAME_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			m_pCX2PVPRoom->SetRoomName( kEvent.m_wstrRoomName );

			CKTDGUIIMEEditBox* pIMEEditRoomName = (CKTDGUIIMEEditBox*)m_pDLGPVPRoomBack->GetControl( L"IMEPVP_Room_Name");
			pIMEEditRoomName->SetText( kEvent.m_wstrRoomName.c_str() );
		}
	}

	return true;
}

bool CX2StatePVPRoom::Handler_EGS_CHANGE_PVP_ROOM_NAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ROOM_NAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_pCX2PVPRoom->SetRoomName( kEvent.m_wstrRoomName );

	CKTDGUIIMEEditBox* pIMEEditRoomName = (CKTDGUIIMEEditBox*)m_pDLGPVPRoomBack->GetControl( L"IMEPVP_Room_Name");
	pIMEEditRoomName->SetText( kEvent.m_wstrRoomName.c_str() );

	return true;
}
#endif SERV_NEW_PVPROOM_PROCESS
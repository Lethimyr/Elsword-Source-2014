#include "StdAfx.h"


#include ".\x2statepvpgame.h"

CX2StatePVPGame::CX2StatePVPGame(void) :
// 2009.01.09 김태완 : 초기화 코드정리
m_pDLGExitMsgBox(NULL),
m_pDLGLoadingState(NULL),
m_pDLGRankBox(NULL),
m_pDLGDeathTeamScore(NULL),
m_bIsOpenExitDLG(false),
m_pDLGF1Help(NULL),
m_pPVPGame(NULL),
m_bLoadingOK(false),
m_pCX2GameLoadingTip(NULL),
m_ConnectType(CX2Game::CT_START_GAME),
m_LoadingPercent(0),
m_LoadingRenderCount(0),
m_bResultStateReq(false),
m_bGameEnd(false),
m_fGameEndWaitTime(5.0f),
//m_DLGLoadingStateUnitInfoList;
m_bShowRankBox(false),
m_bStateChangingToResult(false),
m_iMyLastKillCount(0)
{
	// 초기화 ///////////////////////////////////////////////////////////

// 	m_pDLGExitMsgBox				= NULL;
// 
// 	m_pDLGLoadingState				= NULL;
// 	m_pDLGRankBox					= NULL;
// 	m_pDLGDeathTeamScore			= NULL;
// 	m_bIsOpenExitDLG				= false;
// 
// 
// 	m_bShowRankBox					= false;
// 
// 	m_pPVPGame						= NULL;	
// 	m_bLoadingOK					= false;

// 	m_ConnectType					= CX2Game::CT_START_GAME;
// 	m_LoadingPercent				= 0;
// 	m_LoadingRenderCount			= 0;
// 
// 	m_bResultStateReq				= false;
// 	m_bGameEnd						= false;
// 	m_fGameEndWaitTime				= 5.0f;
// 
// 	m_iMyLastKillCount				= 0;
//	m_bStateChangingToResult		= false;
// 
// 	m_pDLGF1Help					= NULL;
	

	/////////////////////////////////////////////////////////////////////
	DialogLog( "\nCX2StatePVPGame::CX2StatePVPGame Start\n" );

	/*
#ifdef SEPARATION_MOTION
	//XSkinMeshReady( L"Motion_Elsword_Common.x" );
	//XSkinMeshReady( L"Motion_Elsword_Emotion.x" );
	XSkinMeshReady( L"Motion_Elsword.x" );	
	XSkinMeshReady( L"Motion_Elsword_LK.x" );
	XSkinMeshReady( L"Motion_Elsword_MK.x" );
	XSkinMeshReady( L"Motion_Elsword_RS.x" );
	XSkinMeshReady( L"Motion_Elsword_SK.x" );	

	//XSkinMeshReady( L"Motion_Lire_Common.x" );
	//XSkinMeshReady( L"Motion_Lire_Emotion.x" );
	XSkinMeshReady( L"Motion_Lire.x" );	
	XSkinMeshReady( L"Motion_Lire_CR.x" );
	XSkinMeshReady( L"Motion_Lire_SR.x" );
	XSkinMeshReady( L"Motion_Lire_WS.x" );
	XSkinMeshReady( L"Motion_Lire_GA.x" );

	//XSkinMeshReady( L"Motion_Arme_Common.x" );
	//XSkinMeshReady( L"Motion_Arme_Emotion.x" );
	XSkinMeshReady( L"Motion_Arme.x" );	
	XSkinMeshReady( L"Motion_Arme_HM.x" );
	XSkinMeshReady( L"Motion_Arme_DM.x" );
	XSkinMeshReady( L"Motion_Arme_EM.x" );
	XSkinMeshReady( L"Motion_Arme_VP.x" );

	//XSkinMeshReady( L"Motion_Raven_Common.x" );
	//XSkinMeshReady( L"Motion_Raven_Emotion.x" );
	XSkinMeshReady( L"Motion_Raven.x" );	
	XSkinMeshReady( L"Motion_Raven_ST.x" );
	XSkinMeshReady( L"Motion_Raven_OT.x" );
	XSkinMeshReady( L"Motion_Raven_BM.x" );
	XSkinMeshReady( L"Motion_Raven_RF.x" );

	//XSkinMeshReady( L"Motion_EVE_Common.x" );
	//XSkinMeshReady( L"Motion_EVE_Emotion.x" );
	XSkinMeshReady( L"Motion_EVE.x" );	
	XSkinMeshReady( L"Motion_EVE_EG.x" );
	XSkinMeshReady( L"Motion_EVE_AT.x" );
	XSkinMeshReady( L"Motion_EVE_NS.x" );
	XSkinMeshReady( L"Motion_EVE_EP.x" );


#else
	XSkinMeshReady( L"Motion_Elsword_SwordMan.x" );
	XSkinMeshReady( L"Motion_Lire_ElvenRanger.x" );
	XSkinMeshReady( L"Motion_Arme_VioletMage.x" );
	XSkinMeshReady( L"Motion_Raven.x" );
	XSkinMeshReady( L"Motion_Eve.x" );
#endif
	*/

#ifdef SERV_PVP_NEW_SYSTEM
	g_pMain->GetPartyUI()->OpenPartyMenu(false);
#endif



	// 방을 나가는 순간에 방장이 게임을 시작해버리면 타이밍에 따라 방이 지워지고 게임이 시작될 수 있을 것 같다. 본섭 크래시 관련. 
	if( NULL == g_pX2Room ||
		NULL == g_pData->GetPVPRoom() )
	{
		//Handler_EGS_LEAVE_ROOM_REQ();

		StateLog( "exit room, null pvp room at game start" );

		KEGS_LEAVE_ROOM_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
		g_pData->GetServerProtocol()->SendPacket( EGS_LEAVE_ROOM_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_LEAVE_ROOM_ACK );

		
		return; 
	}






	m_DLGLoadingStateUnitInfoList.reserve( 8 );

	m_pCX2GameLoadingTip			= new CX2GameLoadingTip();


	LoadUI();
	OnFrameMove( 0.0f, 0.0f );
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
	m_LoadingPercent				= 10;
	if( g_pData->GetPVPRoom()->GetIntrudeGame() == false )
		GameLoadingReq( m_LoadingPercent );

	CreateGame();
	OnFrameMove( 0.0f, 0.0f );
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
	m_LoadingPercent				= 30;
	if( g_pData->GetPVPRoom()->GetIntrudeGame() == false )
		GameLoadingReq( m_LoadingPercent );

	GameLoadingStart();
	OnFrameMove( 0.0f, 0.0f );
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
	m_LoadingPercent				= 80;
	if( g_pData->GetPVPRoom()->GetIntrudeGame() == false )
		GameLoadingReq( m_LoadingPercent );

	m_LoadingRenderCount			= g_pKTDXApp->GetRenderCount();

	g_pKTDXApp->GetDGManager()->SetProjection( 
		g_pKTDXApp->GetDGManager()->GetNear(), 
		g_pKTDXApp->GetDGManager()->GetFar(), true );

	// note!! 임시로 대전게임 시작하면 카메라를 1500.f 거리로, 즉, 최대로 zoom out
	g_pMain->GetGameOption().CameraZoomIn( -100 );

	g_pData->PlayLobbyBGM( NULL, false );

	g_pKTDXApp->SkipFrame();
	m_bLoadingOK = true;

	DialogLog( "\nCX2StatePVPGame::CX2StatePVPGame End\n" );

#ifdef FIX_OBSERVER_MODE		/// 옵저버 모드는 랭크 박스 열고 게임 시작 하자.
	if( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver )
		m_bShowRankBox = true;
#endif FIX_OBSERVER_MODE

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

CX2StatePVPGame::~CX2StatePVPGame(void)
{	
	DialogLog( "\nCX2StatePVPGame::~CX2StatePVPGame Start\n" );

	SAFE_DELETE_DIALOG( m_pDLGExitMsgBox );


#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState ); // note: 얘는 다이얼로그 매니저에서 관리되는 애가 아니라서~ 요렇게
#endif // DIALOG_HANDLE_TEST // 2009-8-19

	SAFE_DELETE_DIALOG( m_pDLGRankBox );
	SAFE_DELETE_DIALOG( m_pDLGDeathTeamScore );

	m_pPVPGame->Release();
	SAFE_DELETE( m_pPVPGame );
	
	SAFE_DELETE_DIALOG( m_pDLGF1Help );
						
	SAFE_DELETE( m_pCX2GameLoadingTip );

	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
        SAFE_DELETE_DIALOG_HANDLE( pDialog );
#else
		SAFE_DELETE( pDialog );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
	}


	if( false == m_bStateChangingToResult )
		g_pData->PlayLobbyBGM( L"Lobby.ogg", true );

#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pX2Game != NULL )
		g_pX2Game->DeleteAllNPCUnit();
#endif

	DialogLog( "\nCX2StatePVPGame::~CX2StatePVPGame End\n" );
}

void CX2StatePVPGame::LoadUI()
{	

	if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		m_pDLGRankBox = new CKTDGUIDialog( this, L"DLG_PVP_Game_State_RankBox_NEW.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRankBox );
	}



	//로딩 상태
	m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua", 0.0f );
	m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );

	// 랜덤 로딩 화면 
	int nLoadingBG = m_pDLGLoadingState->GetDummyInt(0);
	if( nLoadingBG <= 0 )
		nLoadingBG = 1;
	int iRandBG = rand()%nLoadingBG;

	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		iRandBG = g_pMain->GetTutorialRandomLoadingBG();
	}


	wstringstream wstmStaticName;
	wstmStaticName << L"Static_Loading_BG";
	wstmStaticName << iRandBG;

	if( NULL != m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() ) )
	{
		m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );
		//m_pDLGLoadingState->GetControl( L"PVPLoadingBG" )->SetShow( true );
	}

	if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		if (  g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
		{
			m_pDLGDeathTeamScore = new CKTDGUIDialog( this, L"DLG_PVP_Game_State_TeamDeath_Score_NEW.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDeathTeamScore );
			m_pDLGDeathTeamScore->SetPos( D3DXVECTOR2( 0, 17 ) );
		}
		else if ( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
		{
			m_pDLGDeathTeamScore = new CKTDGUIDialog( this, L"DLG_PVP_Game_State_Death_Surva_Score.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDeathTeamScore );
			m_pDLGDeathTeamScore->SetPos( D3DXVECTOR2( 0, 20 ) );
		}
	}


	// note!! 새 캐릭터 추가되면 여기에 추가해야함
	/*enum LOADING_USER_INFO
	{
		LUI_ELSWORD_COLOR = 0,
		LUI_ARME_COLOR,
		LUI_LIRE_COLOR,
		LUI_RAVEN_COLOR,
		LUI_EVE_COLOR,
		LUI_CHUNG_COLOR,
		
		LUI_ELSWORD_BLACK,
		LUI_ARME_BLACK,
		LUI_LIRE_BLACK,
		LUI_RAVEN_BLACK,
		LUI_EVE_BLACK,
		LUI_CHUNG_BLACK,

		LUI_EMPTY,

#ifdef SERV_PVP_NEW_SYSTEM
		LUI_UNKNOWN_COLOR,
		LUI_UNKNOWN_BLACK,
#endif
	};*/

	for ( int i = 0; i < g_pData->GetPVPRoom()->GetSlotNum(); i++ )
	{
		CKTDGUIDialogType pDialog = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State_User.lua", 0.0f );
		if( NULL == pDialog )
			continue;
		pDialog->SetPos( D3DXVECTOR2( pDialog->GetDummyPos(i).x, pDialog->GetDummyPos(i).y ) );

		CKTDGUIStatic* pStaticUnitInfoTex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoadingUnitInfoTex" );
		if( NULL == pStaticUnitInfoTex )
			continue;

		CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( i );
		if( NULL == pSlotData )
			continue;

#ifdef SERV_PVP_NEW_SYSTEM
		UidType iUnitUid = 0;
		if( pSlotData->m_bNpc == false )
		{
			if( pSlotData->m_pUnit == NULL || pSlotData->m_bReady == false )
			{
				pStaticUnitInfoTex->GetPicture( LUI_EMPTY )->SetShow( true );
				m_DLGLoadingStateUnitInfoList.push_back( pDialog );
				continue;
			}
			iUnitUid = pSlotData->m_pUnit->GetUID();
		}
		else
		{
			iUnitUid = pSlotData->m_iNpcUid;
		}

		bool bMyParty = false;
		if( g_pMain->GetConnectedChannelID() != KPVPChannelInfo::PCC_OFFICIAL )
			bMyParty = true;
		else if( pSlotData->m_bMySlot == true )
			bMyParty = true;
		else if( g_pData->GetPartyManager()->DoIHaveParty() == true && 
			g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iUnitUid ) != NULL )
		{
			bMyParty = true;
		}
#else
		if( pSlotData->m_pUnit == NULL || pSlotData->m_bReady == false )
		{
			pStaticUnitInfoTex->GetPicture( LUI_EMPTY )->SetShow( true );
			m_DLGLoadingStateUnitInfoList.push_back( pDialog );
			continue;
		}
#endif

#ifdef SERV_PVP_NEW_SYSTEM
		if( bMyParty == true )
#endif
		{
			// note!! 캐릭터 추가되면 수정
			switch( pSlotData->m_pUnit->GetType() )
			{
			case CX2Unit::UT_ELSWORD:
				{
					pStaticUnitInfoTex->GetPicture( LUI_ELSWORD_BLACK )->SetShow( true );
				} break;
			case CX2Unit::UT_ARME:
				{
					pStaticUnitInfoTex->GetPicture( LUI_ARME_BLACK )->SetShow( true );
				} break;

			case CX2Unit::UT_LIRE:
				{
					pStaticUnitInfoTex->GetPicture( LUI_LIRE_BLACK )->SetShow( true );
				} break;

			case CX2Unit::UT_RAVEN:
				{
					pStaticUnitInfoTex->GetPicture( LUI_RAVEN_BLACK )->SetShow( true );
				} break;


			case CX2Unit::UT_EVE:
				{
					pStaticUnitInfoTex->GetPicture( LUI_EVE_BLACK )->SetShow( true );		
				} break;

				//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UT_CHUNG:
				{
					pStaticUnitInfoTex->GetPicture( LUI_CHUNG_BLACK )->SetShow( true );		
				} break;
#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UT_ARA:
				{
					pStaticUnitInfoTex->GetPicture( LUI_ARA_BLACK )->SetShow( true );		
				} break;
#endif
#ifdef NEW_CHARACTER_EL
			case CX2Unit::UT_ELESIS:
				{
					pStaticUnitInfoTex->GetPicture( LUI_EL_BLACK )->SetShow( true );		
				} break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			case CX2Unit::UT_ADD:		/// 변경이 필요 하다.
				{
					pStaticUnitInfoTex->GetPicture( LUI_ADD_BLACK )->SetShow( true );		
				} break;
#endif //SERV_9TH_NEW_CHARACTER

			default:
				{
					ASSERT( !"Unexpected Class" );
				} break;
			}
		}
#ifdef SERV_PVP_NEW_SYSTEM
		else
		{
			pStaticUnitInfoTex->GetPicture( LUI_UNKNOWN_BLACK )->SetShow( true );
		}
#endif
		
		CKTDGUIStatic* pStaticUnitInfoPercent = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_Percent" );
		if( NULL != pStaticUnitInfoPercent )
		{
			pStaticUnitInfoPercent->GetString( 0 )->msg = L"0%";
		}

#ifdef SERV_PVP_NEW_SYSTEM
		if( bMyParty == false )
		{
			m_DLGLoadingStateUnitInfoList.push_back( pDialog );
			continue;
		}
#endif

		CKTDGUIStatic* pStaticUnitInfoGrade = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoadingUnitInfoGrade" );
		if ( pStaticUnitInfoGrade != NULL && pStaticUnitInfoGrade->GetPicture(0) != NULL )
		{
#ifdef PVP_SEASON2
			char cRank = pSlotData->m_cRank;
			CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( CX2PVPEmblem::PVP_RANK( cRank ) );
#else
			CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetPVPEmblem() );
#endif
			if ( pPVPEmblemData != NULL )
			{
				pStaticUnitInfoGrade->SetShow( true );
				pStaticUnitInfoGrade->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
			}
		}

		CKTDGUIStatic* pStaticUnitInfo = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_UserInfo" );
		if( NULL != pStaticUnitInfo )
		{
			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"       %s", pSlotData->m_pUnit->GetNickName() );
			//wsprintf( buff, L"       %s", pSlotData->m_pUnit->GetNickName() );
			pStaticUnitInfo->GetString( 0 )->msg = buff;
		}

		m_DLGLoadingStateUnitInfoList.push_back( pDialog );
	}	



	m_LoadingRenderCount = g_pKTDXApp->GetRenderCount();
}

void CX2StatePVPGame::CreateGame()
{
	switch( g_pData->GetPVPRoom()->GetPVPGameType() )
	{
	case CX2PVPRoom::PGT_TEAM:
		{
			m_pPVPGame		= new CX2PVPGameTeam();
			StateLog( L"PGT_TEAM" );
		} break;

	case CX2PVPRoom::PGT_TEAM_DEATH:
		{
			m_pPVPGame		= new CX2PVPGameTeamDeath();
			StateLog( L"PGT_TEAM_DEATH" );
		} break;

	case CX2PVPRoom::PGT_SURVIVAL:
		{
			m_pPVPGame		= new CX2PVPGameSurvival();
			StateLog( L"PGT_SURVIVAL" );
		} break;

	default:
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_675 ), this );
		break;
	}

	m_LoadingRenderCount = g_pKTDXApp->GetRenderCount();
}

void CX2StatePVPGame::GameLoadingStart()
{


	switch( g_pData->GetPVPRoom()->GetPVPGameType() )
	{
	case CX2PVPRoom::PGT_TEAM:
		{
			if( g_pData->GetPVPRoom()->GetIntrudeGame() == true )
			{
				m_ConnectType = CX2Game::CT_INTRUDE;

				m_pPVPGame->GameLoading( g_pData->GetPVPRoom() );
				//m_pPVPGame->P2PSetPeer();
				//m_pPVPGame->P2PConnectTest();
			}
			else
			{
				m_ConnectType = CX2Game::CT_START_GAME;

				m_pPVPGame->GameLoading( g_pData->GetPVPRoom() );

				//m_pPVPGame->P2PSetPeer();
				//m_pPVPGame->P2PConnectTest();
			}
		}
		break;

	case CX2PVPRoom::PGT_TEAM_DEATH:
		{
			if( g_pData->GetPVPRoom()->GetIntrudeGame() == true )
			{
				m_ConnectType = CX2Game::CT_INTRUDE;

				m_pPVPGame->GameLoading( g_pData->GetPVPRoom() );
				//m_pPVPGame->P2PSetPeer();
				//m_pPVPGame->P2PConnectTest();
			}
			else
			{
				m_ConnectType = CX2Game::CT_START_GAME;

				m_pPVPGame->GameLoading( g_pData->GetPVPRoom() );

				//m_pPVPGame->P2PSetPeer();
				//m_pPVPGame->P2PConnectTest();
			}
		}
		break;


	case CX2PVPRoom::PGT_SURVIVAL:
		{
			if ( g_pData->GetPVPRoom()->GetIntrudeGame() == true )
			{
				m_ConnectType = CX2Game::CT_INTRUDE;

				m_pPVPGame->GameLoading( g_pData->GetPVPRoom() );
				//m_pPVPGame->P2PSetPeer();
				//m_pPVPGame->P2PConnectTest();
			}
			else
			{
				m_ConnectType = CX2Game::CT_START_GAME;
				m_pPVPGame->GameLoading( g_pData->GetPVPRoom() );

				//m_pPVPGame->P2PSetPeer();
				//m_pPVPGame->P2PConnectTest();
			}
		}
		break;

	default:
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_675 ), this );
		break;
	}

	m_LoadingRenderCount = g_pKTDXApp->GetRenderCount();

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

    if ( g_pData->GetGameUDP() != NULL )
        g_pData->GetGameUDP()->ResetConnectTestToPeersAll();

#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    if ( g_pX2Room != NULL )
        g_pX2Room->ResetSlotPingSendsAll();
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

}



bool CX2StatePVPGame::QuitGame()
{
	//{{ kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08
	if ( m_pPVPGame != NULL &&
		m_pPVPGame->GetCanUseEscFlag() == true )
	{
		OpenExitDLG();
		m_pCursor->SetShow(true);
	}
	
	//}} kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08

	return true;
}

bool CX2StatePVPGame::ShortCutKeyProcess()
{
	if ( CX2State::ShortCutKeyProcess() == true )
		return true;

	if ( GET_KEY_STATE( GA_OPTION ) == TRUE )
	{
		if( true == IsOptionWindowOpen() )
			CloseOptionWindow();
		else
			OpenOptionWindow();

		return true;
	}

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP

	if ( GET_KEY_STATE( GA_FRIEND ) == TRUE )
	{
		g_pData->GetMessenger()->SetFriendTab(true);
		g_pData->GetMessenger()->SetOpen( !g_pData->GetMessenger()->GetOpen() );

		return true;
	}

#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP

	//{{ kimhc // 2009-10-13 // 길드 탭UI 단축키 지정
#ifdef	GUILD_MANAGEMENT
	// 커뮤니티(친구탭)
	if ( GET_KEY_STATE( GA_GUILD ) == TRUE )
	{			
		g_pData->GetMessenger()->SetTabByShortCutKey( CX2Community::XMUT_GUILD );
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_COMMUNITY);
		return true;
	}
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-13 // 길드 탭UI 단축키 지정

	wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
	D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F11) == TRUE )
	{
		g_pMain->GetGameOption().SetMusic( !g_pMain->GetGameOption().GetOptionList().m_bMusic );
		g_pMain->GetGameOption().SaveScriptFile();
		if( m_bIsOptionWindowOpen == true )
		{
			InitSoundOption();
		}


		if( true == g_pMain->GetGameOption().GetOptionList().m_bMusic )
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_358 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}
		}
		else
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_359 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}
		}						
		return true;
	}

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F10) == TRUE )
	{
		g_pMain->GetGameOption().SetSound( !g_pMain->GetGameOption().GetOptionList().m_bSound );
		g_pMain->GetGameOption().SaveScriptFile();
		if( m_bIsOptionWindowOpen == true )
		{
			InitSoundOption();
		}


		if( true == g_pMain->GetGameOption().GetOptionList().m_bSound )
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_356 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}
		}
		else
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_357 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}

		}						
		return true;
	}

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F9) == TRUE )
	{
		g_pMain->GetGameOption().SetDynamicCamera( !g_pMain->GetGameOption().GetOptionList().m_bDynamicCamera );
		g_pMain->GetGameOption().SaveScriptFile();
		if( m_bIsOptionWindowOpen == true )
		{
			InitOtherOption();
		}

		if( true == g_pMain->GetGameOption().GetOptionList().m_bDynamicCamera )
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_354 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}

		}
		else
		{
			if( NULL != g_pChatBox )
			{

				g_pChatBox->AddChatLog( GET_STRING( STR_ID_355 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			}
		}			

		return true;
	}
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB) == TRUE  &&
		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LALT) != TRUE )
	{
#ifdef FIX_OBSERVER_MODE
		if( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver )
			m_bShowRankBox = true;		/// 옵저버는 랭크 창 끌 수 없도록 하자. ( 자신의 점수가 없어서, 빈 칸으로 보임 )
		else
#endif FIX_OBSERVER_MODE
			m_bShowRankBox = !m_bShowRankBox;

		return true;
	}

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_DELETE) == TRUE )
	{
		bool bShow = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetShow();
		g_pKTDXApp->GetDGManager()->GetDialogManager()->SetShow( !bShow );
		bool bHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();
		
#ifdef ALWAYS_SCREEN_SHOT_TEST
		if(g_pInstanceData->GetScreenShotTest() == false)
		{
			g_pInstanceData->SetScreenShotTest(true);
		}
		else
		{
			g_pInstanceData->SetScreenShotTest(false);
		}
#endif ALWAYS_SCREEN_SHOT_TEST

		if( g_pData->GetPicCharGameScore() != NULL )		
		{
			if( bHide == true )
			{
				g_pData->GetPicCharGameScore()->Clear();
			}
			else
			{
				if( g_pX2Game != NULL )
					g_pX2Game->SetGameScore();
			}
		}

		return true;
	}
#endif

	if ( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_SIT ) == TRUE ) //DIK_V
	{
		if( NULL != m_pPVPGame )
		{
			CX2GUUser* pMyUnit = m_pPVPGame->GetMyUnit();
			if ( NULL != pMyUnit )
			{
				if( pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
					return true;

#ifdef RIDING_SYSTEM
				if ( pMyUnit->GetNowStateID() != pMyUnit->GetWaitStateID() )
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
	}

	if ( ProcessGameScoreAndPostEffect() )
		return true;

	return false;
}

HRESULT CX2StatePVPGame::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	CX2StateOption::OnFrameMove( fTime, fElapsedTime );

	if( m_pDLGLoadingState != NULL && m_LoadingPercent < 100 )
	{
		m_pDLGLoadingState->OnFrameMove( fTime, fElapsedTime );	

		if ( m_pCX2GameLoadingTip != NULL )
			m_pCX2GameLoadingTip->OnFrameMove( fTime, fElapsedTime );

		for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
		{
			CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
			pDialog->OnFrameMove( fTime, fElapsedTime );
		}

	}

	if( g_pData->GetPVPRoom() != NULL )
	{

		if( m_pPVPGame != NULL )
		{
			m_pPVPGame->OnFrameMove( fTime, fElapsedTime );

			if( m_pPVPGame->GetGameState() == CX2Game::GS_PLAY )
			{
				FrameMoveInPlayMode( fTime, fElapsedTime );

				// ping 
				for ( int i = 0; i < g_pData->GetPVPRoom()->GetSlotNum(); i++ )
				{
					CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( i );
					if ( pSlotData->m_pUnit == NULL )
					{
						continue;
					}

#ifdef SERV_PVP_NEW_SYSTEM
					if( pSlotData->m_bNpc == true )
						continue;
#endif

// 					CX2GUUser* pGUUser = m_pPVPGame->GetUserUnitByUID( pSlotData->m_pUnit->GetUID() );
// 					if( NULL == pGUUser )
// 						continue;
// 
// 					if( pSlotData->m_PingTime < 60 )
// 					{
// 						pGUUser->GetGageManager()->SetPingLevel( 3 );
// 					}
// 					else if( pSlotData->m_PingTime < 120 )
// 					{
// 						pGUUser->GetGageManager()->SetPingLevel( 2 );
// 					}
// 					else if( pSlotData->m_PingTime < 10000 )
// 					{
// 						pGUUser->GetGageManager()->SetPingLevel( 1 );
// 					}
// 					else
// 					{
// 						pGUUser->GetGageManager()->SetPingLevel( 0 );
// 					}
				}	

			}


			if( g_pData->GetGameUDP()->ConnectTestResult() == true && m_bLoadingOK == true )
			{
				switch( m_ConnectType )
				{
				case CX2Game::CT_CONNECT_SUCCESS:
					break;

				case CX2Game::CT_START_GAME:
					{
						m_LoadingPercent = 100;
						if( g_pData->GetPVPRoom()->GetIntrudeGame() == false )
							GameLoadingReq( m_LoadingPercent );
					}
					break;

				case CX2Game::CT_INTRUDE:
					{
						m_LoadingPercent = 100;
						Handler_EGS_INTRUDE_START_REQ();
					}
					break;

				case CX2Game::CT_CONNECT_TO_INTRUDER:
					{
					}
					break;
				}	

				m_ConnectType = CX2Game::CT_CONNECT_SUCCESS;

			}

		}
		g_pData->GetPVPRoom()->OnFrameMove( fTime, fElapsedTime );
	}

	UIFrameMove( fTime, fElapsedTime );

	return S_OK;
}

HRESULT CX2StatePVPGame::OnFrameRender()
{

	if( m_pPVPGame != NULL )
	{
		if( m_pPVPGame->GetGameState() != CX2Game::GS_PLAY )
		{
			if( m_pDLGLoadingState != NULL )
			{
				m_pDLGLoadingState->OnFrameRender();

				if ( m_pCX2GameLoadingTip != NULL )
					m_pCX2GameLoadingTip->OnFrameRender();

				for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
				{
					CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
					pDialog->OnFrameRender();
				}
			}
		}
		else
		{
			m_pPVPGame->OnFrameRender();
		}
	}
	else if( m_pDLGLoadingState != NULL )
	{
		m_pDLGLoadingState->OnFrameRender();
		if ( m_pCX2GameLoadingTip != NULL )
			m_pCX2GameLoadingTip->OnFrameRender();

		for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
		{
			CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
			pDialog->OnFrameRender();
		}
	}

	if( m_pPVPGame != NULL && m_pPVPGame->GetGameState() != CX2Game::GS_PLAY )
	{
		if ( m_pDLGExitMsgBox != NULL )
			m_pDLGExitMsgBox->OnFrameRender();
	}

	if ( g_pMain->GetTextManager() != NULL )
		g_pMain->GetTextManager()->OnFrameRender();

	if ( g_pMain->GetGameEdit() != NULL )
		g_pMain->GetGameEdit()->OnFrameRender();

	//{{ 09.03.23 김태완 : 이벤트 타이머 위치 이동
	if( g_pInstanceData != NULL &&
		g_pInstanceData->GetMiniMapUI() != NULL )
	{
		g_pInstanceData->GetMiniMapUI()->UpdateEventNotice();
#ifdef EVENT_CARNIVAL_DECORATION
		g_pInstanceData->GetMiniMapUI()->UpdateCarnivalDeco();
#endif //EVENT_CARNIVAL_DECORATION
	}
	//RenderMarketingEventTimer();
	//}}


	if( m_pCursor != NULL )
		m_pCursor->OnFrameRender();

	return S_OK;
}



HRESULT CX2StatePVPGame::OnResetDevice()
{
	if( m_pPVPGame != NULL )
		m_pPVPGame->OnResetDevice();
	if ( m_pDLGLoadingState != NULL)
		m_pDLGLoadingState->OnResetDevice();

	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
		if ( pDialog != NULL )
			pDialog->OnResetDevice();
	}

	return S_OK;
}

HRESULT CX2StatePVPGame::OnLostDevice()
{
	if( m_pPVPGame != NULL )
		m_pPVPGame->OnLostDevice();
	if ( m_pDLGLoadingState != NULL)
		m_pDLGLoadingState->OnLostDevice();

	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
		if ( pDialog != NULL )
			pDialog->OnLostDevice();
	}

	return S_OK;
}


bool CX2StatePVPGame::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case PGUCM_GAME_LOADING:
		{
			SAFE_DELETE_DIALOG( m_pDLGMsgBox );
			m_pDLGMsgBox	= g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_676 ), this );
			m_pPVPGame->GameLoading( g_pData->GetPVPRoom() );
			g_pKTDXApp->SkipFrame();

			//LoadingCompleteReq();
		}
		break;

	case PGUCM_GAME_START:
		{
			m_pPVPGame->GameStart();
			g_pKTDXApp->SkipFrame();
		}
		break;

	case PGUCM_GAME_EXIT_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGExitMsgBox, NULL, false );
			m_pDLGExitMsgBox = NULL;

			if ( g_pData->GetPVPRoom()->GetCanIntrude() == false )
			{
				Handler_EGS_LEAVE_ROOM_REQ();
			}
			else
			{		
				Handler_EGS_LEAVE_GAME_REQ();
			}

		}
		break;

	case PGUCM_GAME_EXIT_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGExitMsgBox, NULL, false );
			m_pDLGExitMsgBox = NULL;
		}
		break;

		//case PGUCM_GAME_CHAT_ENTER:
		//	{

		//		if ( m_pPVPGame->GetOpenChatBox() == true )
		//		{
		//			m_pDLGChatBox->ClearFocus();
		//			m_pPVPGame->SetOpenChatBox( false );
		//			m_pDLGChatBox->Move(D3DXVECTOR2(-450,0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0.2f, true, false);
		//			CKTDGUIIMEEditBox* pChatBox = (CKTDGUIIMEEditBox*)m_pDLGChatBox->GetControl(L"IMEEditBoxGameChatBox");
		//			pChatBox->HandleKeyboard( WM_KEYDOWN, VK_END, NULL );
		//			//g_pX2Game->SetEnableKeyProcess(true);

		//			Handler_EGS_CHAT_REQ( pChatBox->GetText() );
		//			pChatBox->ClearText();
		//		}

		//	}
		//	break;
	}
	return CX2StateOption::UICustomEventProc( hWnd, uMsg, wParam, lParam );
}

bool CX2StatePVPGame::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateOption::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
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

	case EGS_GAME_LOADING_TIME_OUT_NOT:
		return Handler_EGS_GAME_LOADING_TIME_OUT_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_LEAVE_GAME_ACK:
		return Handler_EGS_LEAVE_GAME_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_LEAVE_GAME_NOT:
		return Handler_EGS_LEAVE_GAME_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_TEAM_NOT:
		return Handler_EGS_CHANGE_TEAM_NOT( hWnd, uMsg, wParam, lParam );
		break;


	case EGS_PLAY_START_NOT:
		return Handler_EGS_PLAY_START_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_GAME_INTRUDE_NOT:
		return Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_INTRUDE_START_ACK:
		return Handler_EGS_INTRUDE_START_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_INTRUDE_START_NOT:
		return Handler_EGS_INTRUDE_START_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GAME_LOADING_ACK:
		return GameLoadingAck( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GAME_LOADING_NOT:
		return GameLoadingNot( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GAME_LOADING_ALL_UNIT_OK_NOT:
		return GameLoadingAllUnitOkNot();
		break;


	case EGS_REMAINING_PLAY_TIME_NOT:
		return Handler_EGS_REMAINING_PLAY_TIME_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CURRENT_KILL_SCORE_NOT:
		return CurrentKillScoreNot( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CURRENT_TEAM_KILL_SCORE_NOT:
		return Handler_EGS_CURRENT_TEAM_KILL_SCORE_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_PLAY_TIME_OUT_NOT:
		return PlayTimeOutNot();
		break;

	case EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK:
		return MyUserUnitInfoToServerAck( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_USER_UNIT_RE_BIRTH_POS_ACK:
		return UserUnitRebirthPosAck( hWnd, uMsg, wParam, lParam );
		break;

		//case EGS_USER_UNIT_RE_BIRTH_ACK:
		//	return UserUnitRebirthAck( hWnd, uMsg, wParam, lParam );
		//	break;

	case EGS_USER_UNIT_RE_BIRTH_NOT:
		return UserUnitRebirthNot( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_NPC_UNIT_CREATE_ACK:
		{
			if( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_NPC_UNIT_CREATE_ACK( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;

	case EGS_NPC_UNIT_CREATE_NOT:
		{
			if( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_NPC_UNIT_CREATE_NOT( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;

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

#ifdef SERV_INSERT_GLOBAL_SERVER
	case EGS_CREATE_ATTRIB_NPC_NOT:
		{
			if( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_CREATE_ATTRIB_NPC_NOT( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;
#endif SERV_INSERT_GLOBAL_SERVER

	case EGS_NPC_UNIT_DIE_ACK:
		{
			if( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_NPC_UNIT_DIE_ACK( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;

	case EGS_NPC_UNIT_DIE_NOT:
		{
			if( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_NPC_UNIT_DIE_NOT( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;

	case EGS_END_GAME_PVP_RESULT_DATA_NOT:
		return Handler_EGS_END_GAME_PVP_RESULT_DATA_NOT( hWnd, uMsg, wParam, lParam );
		break;


	case EGS_STATE_CHANGE_RESULT_ACK:
		return Handler_EGS_STATE_CHANGE_RESULT_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_RESULT_NOT:
		return Handler_EGS_STATE_CHANGE_RESULT_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_UPDATE_UNIT_INFO_NOT:
		return Handler_EGS_UPDATE_UNIT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_ADD_ON_STAT_ACK:
		return Handler_EGS_ADD_ON_STAT_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_ADD_ON_STAT_NOT:
		return Handler_EGS_ADD_ON_STAT_NOT( hWnd, uMsg, wParam, lParam );
		break;

		// remove!!
	//case EGS_ITEM_EXPIRATION_NOT:
	//	{
	//		if( NULL != m_pPVPGame )
	//		{
	//			return m_pPVPGame->Handler_EGS_ITEM_EXPIRATION_NOT( hWnd, uMsg, wParam, lParam );
	//		}
	//		else
	//		{
	//			return false;
	//		}
	//	} break;

	case EGS_CHANGE_PITIN_NOT:
		{
			return Handler_EGS_CHANGE_PITIN_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

		//{{ 허상형 : [2009/8/20] //	신학기 이벤트 킬, 데스 메시지
#ifdef NEW_TERM_EVENT
	case EGS_PVP_KILL_DIE_INFO_NOT:
		{
			return Handler_EGS_PVP_KILL_DIE_INFO_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //	NEW_TERM_EVENT
		//}} 허상형 : [2009/8/20] //	신학기 이벤트 킬, 데스 메시지

#ifdef DUNGEON_ITEM
	case EGS_GET_ITEM_ACK:
		return Handler_EGS_GET_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GET_ITEM_NOT:
		return Handler_EGS_GET_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CREATE_PVP_ITEM_ACK:
		return Handler_EGS_CREATE_PVP_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CREATE_PVP_ITEM_NOT:
		return Handler_EGS_CREATE_PVP_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_PVP_ITEM_NOT:
		return Handler_EGS_CHANGE_PVP_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_PVP_ITEM_POS_ACK:
		return Handler_EGS_CHANGE_PVP_ITEM_POS_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_CHANGE_PVP_ITEM_POS_NOT:
		return Handler_EGS_CHANGE_PVP_ITEM_POS_NOT( hWnd, uMsg, wParam, lParam );
#endif

#ifdef SERV_PVP_NEW_SYSTEM
	case EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK:
		return Handler_EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK:
		return Handler_EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK( hWnd, uMsg, wParam, lParam );
		break;
#endif
		
	case EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT:
		return Handler_EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT:
		return Handler_EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT:
		return Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );	
		break;

#ifdef SERV_WORLD_TRIGGER_RELOCATION
	case EGS_WORLD_TRIGGER_RELOCATION_ACK:
		{
			if( g_pX2Game != NULL )
				return g_pX2Game->Handler_EGS_WORLD_TRIGGER_RELOCATION_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_WORLD_TRIGGER_RELOCATION_NOT:
		{
			if( g_pX2Game != NULL )
				return g_pX2Game->Handler_EGS_WORLD_TRIGGER_RELOCATION_NOT( hWnd, uMsg, wParam, lParam );
		} break;			
#endif
	}

	if( NULL != m_pPVPGame )
	{
		return m_pPVPGame->MsgProc( hWnd, uMsg, wParam, lParam );
	}
	else
	{
		return false;
	}
	
}


bool CX2StatePVPGame::UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return m_pPVPGame->MsgProc( hWnd, uMsg, wParam, lParam );
}


bool CX2StatePVPGame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( CX2State::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
        return true;

    if( NULL != g_pX2Game )
        return g_pX2Game->MsgProc( hWnd, uMsg, wParam, lParam );

    return false;
}

void CX2StatePVPGame::UIFrameMove( double fTime, float fElapsedTime )
{
	RankBoxFrameMove( fTime, fElapsedTime );
#ifdef REFORM_UI_SKILLSLOT
	//대전에서 스킬창 켜주기 위해 추가
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_SKILL_SLOT, true);
#endif //REFORM_UI_SKILLSLOT
}

void CX2StatePVPGame::FrameMoveInPlayMode( double fTime, float fElapsedTime )
{
	if( m_bGameEnd == true )
	{
		m_fGameEndWaitTime -= m_fElapsedTime;
		if( m_fGameEndWaitTime < 0.0f && m_bResultStateReq == false )
		{
			m_bResultStateReq = true;
			if( g_pX2Game->IsHost() == true )
				Handler_EGS_STATE_CHANGE_RESULT_REQ();
		}
	}
}

void CX2StatePVPGame::RankBoxFrameMove( double fTime, float fElapsedTime )
{
	CX2PVPRoom* pPVPRoom = g_pData->GetPVPRoom();
	if( NULL == pPVPRoom || 
		NULL == m_pPVPGame || 
		m_pPVPGame->GetGameState() != CX2Game::GS_PLAY )
	{
		return;
	}

	if( m_pDLGRankBox == NULL )
		return;
	CKTDGUIStatic* pStaticBG			 = (CKTDGUIStatic*) m_pDLGRankBox->GetControl( L"BG" );
	CKTDGUIStatic* pStaticRankBG		 = (CKTDGUIStatic*) m_pDLGRankBox->GetControl( L"RankBG" );
	CKTDGUIStatic* pStaticTab			 = (CKTDGUIStatic*) m_pDLGRankBox->GetControl( L"Tab" );
	CKTDGUIStatic* pStaticStringCategory = (CKTDGUIStatic*) m_pDLGRankBox->GetControl( L"StringCategory" );
	CKTDGUIStatic* pStaticStringRankInfo = (CKTDGUIStatic*) m_pDLGRankBox->GetControl( L"StringRankInfo" );

	if( NULL == pStaticBG || NULL == pStaticRankBG || NULL == pStaticTab || NULL == pStaticStringCategory || NULL == pStaticStringRankInfo )
		return;
	
	for(int i=0; i<16; i++)
		pStaticRankBG->GetPicture(i)->SetShow(false);

	for(int i=0; i<5; i++)
		pStaticStringCategory->GetString(i)->msg = L"";

	for(int i=0; i<8; i++)
	{
		pStaticStringRankInfo->GetString(i*5+0)->msg = L"";
		pStaticStringRankInfo->GetString(i*5+1)->msg = L"";
		pStaticStringRankInfo->GetString(i*5+2)->msg = L"";
		pStaticStringRankInfo->GetString(i*5+3)->msg = L"";
		pStaticStringRankInfo->GetString(i*5+4)->msg = L"";
	}


	if( false == m_bShowRankBox ) // 탭 닫혀 있을 때
	{
		CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetMySlot();
		if( NULL == pSlotData )
			return;

		CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( pSlotData->m_pUnit->GetUID() );
		if( NULL == pGUUser )
			return;

		//뒷 배경 설정.
		pStaticBG->GetPicture(0)->SetPos(D3DXVECTOR2(774.f, 700.f));
		pStaticBG->GetPicture(1)->SetShow(false);
		pStaticBG->GetPicture(2)->SetShow(true);
		pStaticBG->GetPicture(2)->SetPos(D3DXVECTOR2(774.f, 730.f));
		pStaticBG->GetPicture(3)->SetShow(false);

		//탭 위치 설정
		pStaticTab->GetPicture(0)->SetPos(D3DXVECTOR2(993.f, 703.f));

		//팀에 따라 다른 색 설정
		if( (pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || 
			pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH))			
		{
			if( CX2Room::TN_RED == pSlotData->GetTeamNum() )
			{
				pStaticRankBG->GetPicture(7)->SetShow( true );
			}
			else
			{
				pStaticRankBG->GetPicture(15)->SetShow( true );
			}
			
		}
		else
		{
			pStaticRankBG->GetPicture(7)->SetShow( true );
		}

		//대전 현황 스트링 설정
		WCHAR szTemp[32] = L"";
		pStaticStringRankInfo->GetString( 5*7+0 )->msg = pSlotData->m_pUnit->GetNickName(); //닉네임

		StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d", pGUUser->m_KillUserUnitList.size()*3 + pGUUser->m_MaxDamageKillUserUnitList.size()*2 - pGUUser->GetDieCount()*1 );
		pStaticStringRankInfo->GetString( 5*7+1 )->msg = szTemp; //총점

		StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d", pGUUser->m_KillUserUnitList.size() );
		pStaticStringRankInfo->GetString( 5*7+2 )->msg = szTemp; //킬수

		StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d", pGUUser->m_MaxDamageKillUserUnitList.size() );
		pStaticStringRankInfo->GetString( 5*7+3 )->msg = szTemp; //어시

		StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d", pGUUser->GetDieCount());
		pStaticStringRankInfo->GetString( 5*7+4 )->msg = szTemp; //데스
		
		return;
	}
	else
	{
		vector< KillData > vecSlotSorted; 
		SortByKillCount( pPVPRoom, vecSlotSorted );
		//여백으로 둘 공간
		int iOffsetMember = 8 - vecSlotSorted.size(); 
		//탭 위치 설정
		pStaticTab->GetPicture(0)->SetPos(D3DXVECTOR2(993.f, 538.f + iOffsetMember*21));

		//뒷 배경 설정.
		pStaticBG->GetPicture(0)->SetPos(D3DXVECTOR2(774.f, 533.f + iOffsetMember*21 ));//상단

		pStaticBG->GetPicture(1)->SetPos(D3DXVECTOR2(774.f, 563.f + iOffsetMember*21 ));//중단
		pStaticBG->GetPicture(1)->SetSizeY(static_cast<float>(vecSlotSorted.size())*21);
		pStaticBG->GetPicture(1)->SetShow(true);

		pStaticBG->GetPicture(2)->SetPos(D3DXVECTOR2(774.f, 563.f + iOffsetMember*21 + vecSlotSorted.size()*21 ));//중단
		pStaticBG->GetPicture(2)->SetShow(true);

		pStaticBG->GetPicture(3)->SetPos(D3DXVECTOR2(872.f, 572.f + iOffsetMember*21 ));//중단
		pStaticBG->GetPicture(3)->SetSizeY(static_cast<float>(vecSlotSorted.size())*21);
		pStaticBG->GetPicture(3)->SetShow(true);


		// 화면에 출력, // team match, team death match이면 team으로 먼저 sorting하고, 그 다음 kill 수로 sorting
		int iPrevOneKillCount = -1;
		int iRank = 0, iCount = 0;
		WCHAR szTemp[32] = L"";			
		for( UINT i=0; i<vecSlotSorted.size(); i++ )
		{
			KillData& killData = vecSlotSorted[i];

			bool bMyUnit = false;
			CX2GameUnit *pGameUnit = NULL;

			if( killData.iUnitUid > 0 )				
				pGameUnit = (CX2GameUnit *)g_pX2Game->GetUserUnitByUID( killData.iUnitUid );
			else
				pGameUnit = (CX2GameUnit *)g_pX2Game->GetNPCUnitByUID( (int)killData.iUnitUid );

			if( pGameUnit == NULL )
				continue;

			if( pGameUnit->GetUnitUID() == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
				bMyUnit = true;

			iRank++;


			if( bMyUnit == true )
			{
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 0)->color = D3DXCOLOR( 1,1,0,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 1)->color = D3DXCOLOR( 1,1,0,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 2)->color = D3DXCOLOR( 1,1,0,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 3)->color = D3DXCOLOR( 1,1,0,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 4)->color = D3DXCOLOR( 1,1,0,1 );
			}
			else
			{
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 0)->color = D3DXCOLOR( 1,1,1,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 1)->color = D3DXCOLOR( 1,1,1,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 2)->color = D3DXCOLOR( 1,1,1,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 3)->color = D3DXCOLOR( 1,1,1,1 );
				pStaticStringRankInfo->GetString((iCount+iOffsetMember)*5 + 4)->color = D3DXCOLOR( 1,1,1,1 );
			}

			if( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || 
				pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
			{
				if( CX2Room::TN_RED == pGameUnit->GetTeam() )
				{
					pStaticRankBG->GetPicture( (iCount+iOffsetMember) )->SetShow( true );
				}
				else
				{
					pStaticRankBG->GetPicture( (iCount+iOffsetMember) + 8 )->SetShow( true );
				}
			}
			else
			{
				if( bMyUnit == true )
				{
					pStaticRankBG->GetPicture( (iCount+iOffsetMember) )->SetShow( true );
				}
				else
				{
					pStaticRankBG->GetPicture( (iCount+iOffsetMember) + 8 )->SetShow( true );
				}
			}
			//대전 현황 스트링 설정
			pStaticStringCategory->SetOffsetPos( D3DXVECTOR2(0.f, static_cast<float>(iOffsetMember)*21.f ));
#ifdef PVP_TAP_SHORT_STRING
			pStaticStringCategory->GetString(0)->msg = GET_STRING(STR_ID_1127);
			pStaticStringCategory->GetString(1)->msg = GET_STRING(STR_ID_4413);
			pStaticStringCategory->GetString(2)->msg = GET_STRING(STR_ID_25077);
			pStaticStringCategory->GetString(3)->msg = GET_STRING(STR_ID_25078);
			pStaticStringCategory->GetString(4)->msg = GET_STRING(STR_ID_25079);
#else //PVP_TAP_SHORT_STRING
			pStaticStringCategory->GetString(0)->msg = GET_STRING(STR_ID_1127);
			pStaticStringCategory->GetString(1)->msg = GET_STRING(STR_ID_17269);
			pStaticStringCategory->GetString(2)->msg = GET_STRING(STR_ID_17270);
			pStaticStringCategory->GetString(3)->msg = GET_STRING(STR_ID_17271);
			pStaticStringCategory->GetString(4)->msg = GET_STRING(STR_ID_17272);
#endif //PVP_TAP_SHORT_STRING

			WCHAR szTemp[32] = L"";
			pStaticStringRankInfo->GetString( 5*(iCount+iOffsetMember)+0 )->msg = pGameUnit->GetUnitName(); //닉네임

			StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d", pGameUnit->m_KillUserUnitList.size()*3 + pGameUnit->m_MaxDamageKillUserUnitList.size()*2 - pGameUnit->m_iDieCount*1 );
			pStaticStringRankInfo->GetString( 5*(iCount+iOffsetMember)+1 )->msg = szTemp; //총점

			StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d",  pGameUnit->m_KillUserUnitList.size() );
			pStaticStringRankInfo->GetString( 5*(iCount+iOffsetMember)+2 )->msg = szTemp; //킬수

			StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d", pGameUnit->m_MaxDamageKillUserUnitList.size() );
			pStaticStringRankInfo->GetString( 5*(iCount+iOffsetMember)+3 )->msg = szTemp; //어시

			StringCchPrintf( szTemp, ARRAY_SIZE(szTemp), L"%d", pGameUnit->m_iDieCount);
			pStaticStringRankInfo->GetString( 5*(iCount+iOffsetMember)+4 )->msg = szTemp; //데스

			iCount++;
		}

	}
}

void CX2StatePVPGame::SortByKillCount( CX2PVPRoom* pPVPRoom, vector<KillData>& vecSlotSorted )
{
	vecSlotSorted.resize(0);

	for( int i=0; i<pPVPRoom->GetSlotNum(); i++ )
	{
		CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( i );

		if( pSlotData == NULL || i>pPVPRoom->GetSlotNum()-1 || 
			pSlotData->GetSlotState() == CX2Room::SS_EMPTY || 
			pSlotData->GetSlotState() == CX2Room::SS_CLOSE )
		{
			continue;
		}

		CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( pSlotData->m_pUnit->GetUID() );
		if( NULL == pGUUser )
		{	
			continue;
		}

		KillData killData;
#ifdef SERV_PVP_NEW_SYSTEM
		killData.iUnitUid	= pGUUser->GetUnitUID();
		killData.pGameUnit	= (CX2GameUnit *)pGUUser;
#else
		killData.pSlotData	= pSlotData;
		killData.pUser		= pGUUser;
#endif
		killData.killNum	= (int) pGUUser->m_KillUserUnitList.size();
		killData.mdKillNum	= (int) pGUUser->m_MaxDamageKillUserUnitList.size();

		vecSlotSorted.push_back( killData );
	}

#ifdef SERV_PVP_NEW_SYSTEM
	std::vector< CX2Room::RoomNpcSlot > &vecNpcSlot = pPVPRoom->GetNpcSlot();

	for( int i=0; i<(int)vecNpcSlot.size(); ++i)
	{
		CX2GameUnit *pGameUnit = g_pX2Game->GetNPCUnitByUID( (int)(vecNpcSlot[i].m_iNpcUid) );

		if( pGameUnit != NULL )
		{
			KillData killData;
			killData.iUnitUid	= pGameUnit->GetUnitUID();
			killData.pGameUnit	= pGameUnit;
			killData.killNum	= pGameUnit->m_KillUserUnitList.size();
			killData.mdKillNum	= pGameUnit->m_MaxDamageKillUserUnitList.size();

			vecSlotSorted.push_back( killData );
		}
	}
#endif

	std::sort( vecSlotSorted.begin(), vecSlotSorted.end(), HigherRank( pPVPRoom->GetPVPGameType() ) );
}











//방 정보 변경 관련
bool CX2StatePVPGame::Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	return m_pPVPGame->Handler_EGS_JOIN_ROOM_NOT( kEvent );
}

bool CX2StatePVPGame::Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason /* = NetError::NOT_LEAVE_ROOM_REASON_00 */ )
{
	return m_pPVPGame->Handler_EGS_LEAVE_ROOM_REQ( leaveRoomReason );
}

bool CX2StatePVPGame::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pPVPGame->Handler_EGS_LEAVE_ROOM_ACK( kEvent ) == true )
	{
#ifdef SERV_PVP_NEW_SYSTEM
		if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
		{

#ifndef HEAP_BROKEN_BY_ROOM
			g_pData->DeletePVPRoom();
#endif // HEAP_BROKEN_BY_ROOM

			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if( pState != NULL )
				pState->ReturnToPlaceWhereBeforeDungeonStart();
		}
		else
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
			
#ifndef HEAP_BROKEN_BY_ROOM
			g_pData->DeletePVPRoom();
#endif // HEAP_BROKEN_BY_ROOM

			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
			DialogLog( "  XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false  " );
		}
#else
		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
		
#ifndef HEAP_BROKEN_BY_ROOM
		g_pData->DeletePVPRoom();
#endif // HEAP_BROKEN_BY_ROOM

		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
		DialogLog( "  XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false  " );
#endif

		CX2GageManager::GetInstance()->ClearPvpMemberUI();

		return true;
	}	
	return false;
}

bool CX2StatePVPGame::Handler_EGS_GAME_LOADING_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
	{
#ifndef HEAP_BROKEN_BY_ROOM
		g_pData->DeletePVPRoom();
#endif // HEAP_BROKEN_BY_ROOM

		CX2State* pState = (CX2State*)g_pMain->GetNowState();
		if( pState != NULL )
			pState->Handler_EGS_STATE_CHANGE_FIELD_REQ();	/// kimhc // 이렇게 되면... 어느 마을로 가지?
	}
	else
	{
		g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
		
#ifndef HEAP_BROKEN_BY_ROOM
		g_pData->DeletePVPRoom();
#endif // HEAP_BROKEN_BY_ROOM

		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
		DialogLog( "  XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false  " );
	}
#else
	g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_437 ) );
	
#ifndef HEAP_BROKEN_BY_ROOM
	g_pData->DeletePVPRoom();
#endif // HEAP_BROKEN_BY_ROOM

	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
	DialogLog( "  XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false  " );
#endif
	return true;
}

bool CX2StatePVPGame::Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	return m_pPVPGame->Handler_EGS_LEAVE_ROOM_NOT( kEvent );
}


bool CX2StatePVPGame::Handler_EGS_LEAVE_GAME_REQ()
{
	return m_pPVPGame->Handler_EGS_LEAVE_GAME_REQ();
}

bool CX2StatePVPGame::Handler_EGS_LEAVE_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//게임중에 강제로 나갔을때...
	KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_GAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( m_pPVPGame->Handler_EGS_LEAVE_GAME_ACK( kEvent ) == true )
	{
		return true;
	}	
	return false;
}

bool CX2StatePVPGame::Handler_EGS_LEAVE_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_GAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );



	return m_pPVPGame->Handler_EGS_LEAVE_GAME_NOT( kEvent );
}


bool CX2StatePVPGame::Handler_EGS_CHANGE_TEAM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_TEAM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	return g_pData->GetPVPRoom()->Handler_EGS_CHANGE_TEAM_NOT( kEvent );
}

bool CX2StatePVPGame::Handler_EGS_STATE_CHANGE_RESULT_REQ()
{
	return m_pPVPGame->Handler_EGS_STATE_CHANGE_RESULT_REQ();
}

bool CX2StatePVPGame::Handler_EGS_STATE_CHANGE_RESULT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_RESULT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	return m_pPVPGame->Handler_EGS_STATE_CHANGE_RESULT_ACK( kEvent );
}

bool CX2StatePVPGame::Handler_EGS_STATE_CHANGE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_RESULT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	m_bStateChangingToResult = true; 
	return m_pPVPGame->Handler_EGS_STATE_CHANGE_RESULT_NOT( kEvent );
}



bool CX2StatePVPGame::GameLoadingReq( int percent )
{
	if ( g_pData->GetPVPRoom() != NULL )
	{
		if ( g_pData->GetPVPRoom()->GetMySlot() != NULL )
		{
			if ( g_pData->GetPVPRoom()->GetMySlot()->m_bObserver == true )
				return true;
		}	
	}

	KEGS_GAME_LOADING_REQ kPacket;

	kPacket.m_iLoadingProgress = percent;

	g_pData->GetServerProtocol()->SendPacket( EGS_GAME_LOADING_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GAME_LOADING_ACK, 60.0f );

	return true;
}

bool CX2StatePVPGame::GameLoadingAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_GAME_LOADING_ACK ) == true )
	{		
		if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{
		}
	}
	return true;
}

bool CX2StatePVPGame::GameLoadingNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GAME_LOADING_NOT kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );


	// note!! 신캐릭터 추가되면 추가해야함
	/*enum LOADING_USER_INFO
	{
		LUI_ELSWORD_COLOR = 0,
		LUI_ARME_COLOR,
		LUI_LIRE_COLOR,
		LUI_RAVEN_COLOR,
		LUI_EVE_COLOR,
		LUI_CHUNG_COLOR,

		LUI_ELSWORD_BLACK,
		LUI_ARME_BLACK,
		LUI_LIRE_BLACK,
		LUI_RAVEN_BLACK,
		LUI_EVE_BLACK,
		LUI_CHUNG_BLACK,

		LUI_EMPTY,

#ifdef SERV_PVP_NEW_SYSTEM
		LUI_UNKNOWN_COLOR,
		LUI_UNKNOWN_BLACK,
#endif
	};*/

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
    if ( kPacket.m_iLoadingProgress >= 100 )
    {
        if ( g_pData->GetGameUDP() != NULL )
            g_pData->GetGameUDP()->ResetConnectTestToPeer( kPacket.m_iUnitUID );
    }//if
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


	for ( int i = 0; i < g_pData->GetPVPRoom()->GetSlotNum(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];

		CKTDGUIStatic* pStaticUnitInfoTex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoadingUnitInfoTex" );

		CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData( i );

#ifdef SERV_PVP_NEW_SYSTEM
		if( pSlotData->m_bNpc == false && pSlotData->m_pUnit == NULL )
			continue;
		if( pSlotData->m_bNpc == true )
		{
			if( pSlotData->m_iNpcUid != kPacket.m_iUnitUID )
				continue;
		}
		else
		{
			if ( pSlotData->m_pUnit->GetUID() != kPacket.m_iUnitUID )
				continue;
		}

		bool bMyParty = false;
		if( g_pMain->GetConnectedChannelID() != KPVPChannelInfo::PCC_OFFICIAL )
			bMyParty = true;
		else if( pSlotData->m_bMySlot == true )
			bMyParty = true;
		else if( g_pData->GetPartyManager()->DoIHaveParty() == true && 
			g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( kPacket.m_iUnitUID ) != NULL )
		{
			bMyParty = true;
		}
#else
		if ( pSlotData->m_pUnit == NULL )
			continue;
		if ( pSlotData->m_pUnit->GetUID() != kPacket.m_iUnitUID )
			continue;
#endif

		

		// note!! 캐릭터 추가되면 수정
		if ( kPacket.m_iLoadingProgress >= 100 )
		{
#ifdef SERV_PVP_NEW_SYSTEM
			if( bMyParty == true )
#endif
			{
				switch(pSlotData->m_pUnit->GetType())
				{
				case CX2Unit::UT_ELSWORD:
					{
						pStaticUnitInfoTex->GetPicture( LUI_ELSWORD_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_ELSWORD_COLOR )->SetShow( true );
					} break;

				case CX2Unit::UT_ARME:
					{
						pStaticUnitInfoTex->GetPicture( LUI_ARME_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_ARME_COLOR )->SetShow( true );
					} break;

				case CX2Unit::UT_LIRE:
					{
						pStaticUnitInfoTex->GetPicture( LUI_LIRE_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_LIRE_COLOR )->SetShow( true );
					} break;

				case CX2Unit::UT_RAVEN:
					{
						pStaticUnitInfoTex->GetPicture( LUI_RAVEN_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_RAVEN_COLOR )->SetShow( true );
					} break;


				case CX2Unit::UT_EVE:
					{
						pStaticUnitInfoTex->GetPicture( LUI_EVE_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_EVE_COLOR )->SetShow( true );
					} break;

					//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
				case CX2Unit::UT_CHUNG:
					{
						pStaticUnitInfoTex->GetPicture( LUI_CHUNG_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_CHUNG_COLOR )->SetShow( true );
					} break;
#endif	NEW_CHARACTER_CHUNG
					//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
				case CX2Unit::UT_ARA:
					{
						pStaticUnitInfoTex->GetPicture( LUI_ARA_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_ARA_COLOR )->SetShow( true );
					} break;
#endif
#ifdef NEW_CHARACTER_EL
				case CX2Unit::UT_ELESIS:
					{
						pStaticUnitInfoTex->GetPicture( LUI_EL_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_EL_COLOR )->SetShow( true );
					} break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UT_ADD:		/// 변경이 필요 하다.
					{
						pStaticUnitInfoTex->GetPicture( LUI_ADD_BLACK )->SetShow( false );
						pStaticUnitInfoTex->GetPicture( LUI_ADD_COLOR )->SetShow( true );
					} break;
#endif //SERV_9TH_NEW_CHARACTER
				default:
					{
						ASSERT( !"Unexpected UnitClass!" );
					} break;
				}
			}
#ifdef SERV_PVP_NEW_SYSTEM
			else
			{
				pStaticUnitInfoTex->GetPicture( LUI_UNKNOWN_BLACK )->SetShow( false );
				pStaticUnitInfoTex->GetPicture( LUI_UNKNOWN_COLOR )->SetShow( true );
			}
#endif
		}


		WCHAR buff[256] = {0};
		CKTDGUIStatic* pStaticUnitInfoPercent = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_Percent" );
		StringCchPrintf( buff, 256, L"%d%%", kPacket.m_iLoadingProgress );
		//wsprintf( buff, L"%d%%", kPacket.m_iLoadingProgress );
		pStaticUnitInfoPercent->GetString( 0 )->msg = buff;
		CKTDGUIStatic* pStaticUnitInfo = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_UserInfo" );

#ifdef SERV_PVP_NEW_SYSTEM
		if( bMyParty == true )			
#endif
		{
			StringCchPrintf( buff, 256,  L"       %s", pSlotData->m_pUnit->GetNickName() );		
			pStaticUnitInfo->GetString( 0 )->msg = buff;
		}		

		break;
	}	

	return true;
}

bool CX2StatePVPGame::GameLoadingAllUnitOkNot()
{	
	return true;
}

bool CX2StatePVPGame::Handler_EGS_PLAY_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PLAY_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	m_pDLGLoadingState->SetShow( false );

	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
		pDialog->SetShow( false );
	}

#ifdef PVP_SEASON2
	if ( NULL != g_pX2Game )
	{	
		for( int i = 0; i < (int)g_pX2Game->GetUserUnitListSize(); ++i )
		{
			CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnit( i );
			if ( NULL != pCX2GUUser )
			{
				CX2Unit* pUnit = pCX2GUUser->GetUnit(); 
				if ( NULL != pUnit ) 
				{
					CX2Unit::UnitData* pUnitData = &pCX2GUUser->GetUnit()->AccessUnitData();
                    pUnitData->ClearPremiumBuffInfo();
				}
			}
		}
	}
#endif

	return m_pPVPGame->Handler_EGS_PLAY_START_NOT( kEvent );
}

bool CX2StatePVPGame::Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_GAME_INTRUDE_NOT kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	return m_pPVPGame->Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( kPacket );
}

bool CX2StatePVPGame::Handler_EGS_INTRUDE_START_REQ()
{
	return m_pPVPGame->Handler_EGS_INTRUDE_START_REQ();
}

bool CX2StatePVPGame::Handler_EGS_INTRUDE_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INTRUDE_START_ACK kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	return m_pPVPGame->Handler_EGS_INTRUDE_START_ACK( kPacket );
}

bool CX2StatePVPGame::Handler_EGS_INTRUDE_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INTRUDE_START_NOT kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	m_pDLGLoadingState->SetShow(false);

	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
		pDialog->SetShow( false );
	}

	return m_pPVPGame->Handler_EGS_INTRUDE_START_NOT( kPacket );
}


bool CX2StatePVPGame::CurrentKillScoreNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CURRENT_KILL_SCORE_NOT kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	for( int i = 0; i < (int)m_pPVPGame->GetUserUnitNum(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_pPVPGame->GetUserUnit( i );
		if( pCX2GUUser == NULL )
			continue;

		std::map< UidType, std::pair< int, int > >::iterator iter;
		iter = kPacket.m_mapKillScore.find( pCX2GUUser->GetUnitUID() );
		std::map< UidType, int >::iterator iter2;
		iter2 = kPacket.m_mapMDKillScore.find( pCX2GUUser->GetUnitUID() );
		if( iter != kPacket.m_mapKillScore.end() && iter2 != kPacket.m_mapMDKillScore.end() )
		{
			std::pair< int, int > data = iter->second;
			m_pPVPGame->SetKillScore( pCX2GUUser->GetUnitUID(), data.first, data.second, iter2->second );
		}
	}

	return true;
}

bool CX2StatePVPGame::Handler_EGS_CURRENT_TEAM_KILL_SCORE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CURRENT_TEAM_KILL_SCORE_NOT kPacket;
	DeSerialize( pBuff, &kPacket );
	//CX2PacketLog::PrintLog( &kPacket );

	if( NULL != m_pPVPGame )
		m_pPVPGame->Handler_EGS_CURRENT_TEAM_KILL_SCORE_NOT( kPacket );

	return true;
}

bool CX2StatePVPGame::Handler_EGS_REMAINING_PLAY_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_REMAINING_PLAY_TIME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
#ifndef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	CX2PacketLog::PrintLog( &kEvent );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	if( NULL != m_pPVPGame )
		m_pPVPGame->Handler_EGS_REMAINING_PLAY_TIME_NOT( kEvent ); 

	return true;
}

bool CX2StatePVPGame::PlayTimeOutNot()
{
	if( NULL == m_pPVPGame )
		return false;

	if( m_pPVPGame->GetTimeStop() == false )
	{
		m_pPVPGame->TimeOut();
		m_pPVPGame->Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ();
		
#ifdef SERV_PVP_NEW_SYSTEM
		// 대전 npc 존재하면 (only host)
		if( g_pX2Game->IsHost() == true )
			Handler_EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ();
#endif
	}

	return true;
}

bool CX2StatePVPGame::MyUserUnitInfoToServerAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{			
		}
	}

	return true;
}

bool CX2StatePVPGame::UserUnitRebirthPosReq( int startPosIndex )
{
	KEGS_USER_UNIT_RE_BIRTH_POS_REQ kEGS_USER_UNIT_RE_BIRTH_POS_REQ;
	kEGS_USER_UNIT_RE_BIRTH_POS_REQ.m_StartPosIndex = startPosIndex;

	g_pData->GetServerProtocol()->SendPacket( EGS_USER_UNIT_RE_BIRTH_POS_REQ, kEGS_USER_UNIT_RE_BIRTH_POS_REQ );
	g_pMain->AddServerPacket( EGS_USER_UNIT_RE_BIRTH_POS_ACK, 60.0f );

	return true;
}

bool CX2StatePVPGame::UserUnitRebirthPosAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//KSerBuffer* pBuff = (KSerBuffer*)lParam;


	if( g_pMain->DeleteServerPacket( EGS_USER_UNIT_RE_BIRTH_POS_ACK ) == true )
	{
	}

	return true;
}

bool CX2StatePVPGame::UserUnitRebirthReq( UidType unitUID )
{
	return true;
}

bool CX2StatePVPGame::UserUnitRebirthAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return true;
}

bool CX2StatePVPGame::UserUnitRebirthNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_UNIT_RE_BIRTH_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	m_pPVPGame->RebirthUserUnit( kEvent.m_UserUnitUID, kEvent.m_StartPosIndex );

	return true;
}

/*virtual*/ bool CX2StatePVPGame::Handler_EGS_USER_UNIT_DIE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_UNIT_DIE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_USER_UNIT_DIE_ACK ) == true )
	{
		CX2GUUser* pCX2GUUser = m_pPVPGame->GetMyUnit();
		if( pCX2GUUser != NULL )
		{
			if( kEvent.m_fReBirthTime > 0 )
			{
				
				ASSERT( m_pPVPGame->GetWorld()->GetLineMap()->GetStartPosNum() > 0 );
				int iStartPositionIndex = 0;
				if( m_pPVPGame->GetWorld()->GetLineMap()->GetStartPosNum() > 0 )
				{
					iStartPositionIndex = rand() % m_pPVPGame->GetWorld()->GetLineMap()->GetStartPosNum();
				}

				UserUnitRebirthPosReq( iStartPositionIndex );
				if( pCX2GUUser != NULL )
					pCX2GUUser->SetRebirthTime( kEvent.m_fReBirthTime );
			}
		}
	}

	m_pPVPGame->Handler_EGS_USER_UNIT_DIE_ACK( kEvent );

	return true;
}

/*virtual*/ bool CX2StatePVPGame::Handler_EGS_USER_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_UNIT_DIE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	m_pPVPGame->UserUnitDieNot( kEvent );

#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pX2Game->IsHost() == true )
	{
		if( g_pX2Room->IsNpcSlot( kEvent.m_KilledUserUnitUID ) == true )
		{
			int iStartPositionIndex = 0;
			if( m_pPVPGame->GetWorld()->GetLineMap()->GetStartPosNum() > 0 )
			{
				iStartPositionIndex = rand() % m_pPVPGame->GetWorld()->GetLineMap()->GetStartPosNum();
			}

			Handler_EGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ( kEvent.m_KilledUserUnitUID, iStartPositionIndex );
		}	
	}	
#endif

	return true;
}

/*virtual*/ bool CX2StatePVPGame::Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_UNIT_DIE_COMPLETE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	m_pPVPGame->Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( kEvent );

	return true;
}

bool CX2StatePVPGame::Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_GAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	return m_pPVPGame->Handler_EGS_END_GAME_NOT( kEvent );
}

//bool CX2StatePVPGame::Handler_EGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//	//CX2PacketLog::PrintLog( &kEvent );
//
//	//m_bGameEnd = true;
//
//	return m_pPVPGame->Handler_EGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT( kEvent );
//}
//
//bool CX2StatePVPGame::Handler_EGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//	//CX2PacketLog::PrintLog( &kEvent );
//
//	//m_bGameEnd = true;
//
//	return m_pPVPGame->Handler_EGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT( kEvent );
//}
//
//bool CX2StatePVPGame::Handler_EGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	//m_bGameEnd = true;
//
//	return m_pPVPGame->Handler_EGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT( kEvent );
//}

bool CX2StatePVPGame::Handler_EGS_END_GAME_PVP_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_GAME_PVP_RESULT_DATA_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//m_bGameEnd = true;

	//{{ 2007. 10. 5  최육사  증가된 근성도 받기
	g_pData->GetMyUser()->GetSelectUnit()->SetSpirit( kEvent.m_iSpirit );
	//}}

	return m_pPVPGame->Handler_EGS_END_GAME_PVP_RESULT_DATA_NOT( kEvent );
}

bool CX2StatePVPGame::Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_UNIT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bGameEnd = true;

	{
		g_pMain->ResetDungeonRewardItem();
		std::vector<KItemInfo> vecItemInfo; 
		std::map< int, int >::iterator iT;
		for ( iT = kEvent.m_mapItemObtained.begin(); iT != kEvent.m_mapItemObtained.end(); ++iT )
		{
			int iItemTID = (*iT).first;
			int iQuantity = (*iT).second;

			KItemInfo kItemInfo;
			kItemInfo.m_iItemID = iItemTID;
			kItemInfo.m_iQuantity = iQuantity;
			kItemInfo.m_cUsageType = g_pData->GetItemManager()->GetItemTemplet( iItemTID )->GetPeriodType();

			vecItemInfo.push_back( kItemInfo );
		}
		g_pMain->AddDungeonRewardItem( vecItemInfo );
	}

	return m_pPVPGame->Handler_EGS_UPDATE_UNIT_INFO_NOT( kEvent );
}

//bool CX2StatePVPGame::Handler_EGS_NOTIFY_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_NOTIFY_MSG_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	return m_pPVPGame->Handler_EGS_NOTIFY_MSG_NOT( kEvent );
//}

bool CX2StatePVPGame::Handler_EGS_ADD_ON_STAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADD_ON_STAT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	return m_pPVPGame->Handler_EGS_ADD_ON_STAT_ACK( kEvent );
}

bool CX2StatePVPGame::Handler_EGS_ADD_ON_STAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADD_ON_STAT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	return m_pPVPGame->Handler_EGS_ADD_ON_STAT_NOT( kEvent );
}

bool CX2StatePVPGame::Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PITIN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if ( g_pX2Room != NULL )
		return g_pX2Room->Handler_EGS_CHANGE_PITIN_NOT( kEvent );

	return false;
}

//{{ 허상형 : [2009/8/20] //	신학기 이벤트 대전방 패킷 관련
#ifdef NEW_TERM_EVENT
bool CX2StatePVPGame::Handler_EGS_PVP_KILL_DIE_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_KILL_DIE_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//	PVP중, 던전게임중이 아니면 화면에 표시하지 않음
	if ( g_pMain->GetNowStateID() != CX2Main::XS_PVP_GAME && g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_GAME )
	{
		return true;
	}

	//	킬 점수 표시
	if( true == kEvent.m_bShowKillCount )
	{
		g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_REPLACED_STRING( ( STR_ID_4405, "i", kEvent.m_iKillCount ) ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );

		//	50번째 킬 
		if( kEvent.m_iKillCount == 50 )
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_4407 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
		}
	}
	//	데스 점수 표시
	else
	{
		g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_REPLACED_STRING( ( STR_ID_4406, "i", kEvent.m_iDieCount ) ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );

		//	50번째 데스
		if( kEvent.m_iDieCount == 50 )
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_4408 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
		}
	}

	return true;
}
#endif	//	NEW_TERM_EVENT
//}} 허상형 : [2009/8/20] //	신학기 이벤트 대전방 패킷 관련

/*
bool CX2StatePVPGame::SendChatMsgReq( KEGS_CHAT_REQ& kPacket )
{
	if ( kPacket.m_wstrMsg.compare( L"" ) == 0 )
	{
		return false;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_CHAT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHAT_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	//m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"채팅 메시지를 보내는 중입니다.", this );

	return true;
}

bool CX2StatePVPGame::SendChatMsgAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if( g_pMain->DeleteServerPacket( EGS_CHAT_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
		}
	}
	return true;
}


bool CX2StatePVPGame::BroadCastPVPGameChatInfoNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHAT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2GUUser* pGameUnit = g_pX2Game->GetUserUnitByUID( kEvent.m_UnitUID );
	if( pGameUnit == NULL || pGameUnit->GetUnit() == NULL )
		return true;

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"DLG_PVP_Game_ChatBox_Pos.lua" );
	if( Info == NULL )
	{
		return true;
	}

//{{ robobeg : 2008-10-28
	//KLuaManager kLuamanager;
    KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28
	if( kLuamanager.DoMemory( Info->pRealData, Info->size
#ifdef  X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
        , L"DLG_PVP_Game_ChatBox_Pos.lua"
#endif  X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION    
    ) == false )
		return true;

	CX2TalkBoxManager::TalkBox* talkBox = new CX2TalkBoxManager::TalkBox();

	talkBox->talkContent	= kEvent.m_wstrMsg;
	talkBox->unitUID		= kEvent.m_UnitUID;



	CX2ChatWindow::ChatData* pChatData = new CX2ChatWindow::ChatData();
	pChatData->senderName	= pGameUnit->GetUnit()->GetNickName();
	pChatData->message		= kEvent.m_wstrMsg;
	pChatData->chatType		= CX2ChatWindow::CT_CHAT;
	m_pChatWindow->PushChatData( pChatData );





	char tableName[255] = "";
	float fX, fY;

	int posIndex = pGameUnit->GetGageManager()->GetIndex() + 1;

	if ( pGameUnit->GetGageManager()->GetIndex() == -1 )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_UP_LEFT_TO_RIGHT;
	}
	else if (   pGameUnit->GetGageManager()->GetIndex() == 0  )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_UP_LEFT_TO_RIGHT;
	}
	else if (   pGameUnit->GetGageManager()->GetIndex() == 1  )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_UP_LEFT_TO_RIGHT;
	}
	else if (   pGameUnit->GetGageManager()->GetIndex() == 2  )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_UP_LEFT_TO_RIGHT;
	}
	else if (   pGameUnit->GetGageManager()->GetIndex() == 3  )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_DOWN;
	}
	else if (   pGameUnit->GetGageManager()->GetIndex() == 4  )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_DOWN;
	}
	else if (   pGameUnit->GetGageManager()->GetIndex() == 5  )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_DOWN;
	}
	else if (   pGameUnit->GetGageManager()->GetIndex() == 6  )
	{
		talkBox->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_DOWN;
	}

	sprintf( tableName, "CHAT_POS%d", posIndex );
	if( kLuamanager.BeginTable( tableName ) == true )
	{
		LUA_GET_VALUE( kLuamanager, "X", fX, 0.0f );
		LUA_GET_VALUE( kLuamanager, "Y", fY, 0.0f );

		talkBox->pos = D3DXVECTOR2( fX, fY );
	}

	m_pTalkBoxMgr->Push( talkBox );


	// 유저 유닛 따라 다니는 말칸 
	CX2TalkBoxManager::TalkBox* talkBox2 = new CX2TalkBoxManager::TalkBox();
	*talkBox2 = *talkBox;
	talkBox2->m_bTraceUnit = true;
	talkBox2->talkBoxType = CX2TalkBoxManager::TalkBox::TBT_DOWN;
	talkBox2->talkContent = kEvent.m_wstrMsg;
	m_pTalkBoxMgr->Push( talkBox2 );

	return true;
}
*/




void CX2StatePVPGame::OpenExitDLG()
{
	switch( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
	case KPVPChannelInfo::PCC_PLAY:
	case KPVPChannelInfo::PCC_FREE:
	case KPVPChannelInfo::PCC_TOURNAMENT:
		{
			m_pDLGExitMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_562 ), PGUCM_GAME_EXIT_OK, this, PGUCM_GAME_EXIT_CANCEL );
			if( m_pDLGExitMsgBox != NULL )
			{
				CKTDGUIButton* pCancelButton = (CKTDGUIButton*)m_pDLGExitMsgBox->GetControl( L"MsgBoxOkAndCancelCancelButton" );		
				pCancelButton->RequestFocus();
			}
			return;
		} break;

	default:
		break;
	}

	if ( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM )
	{
		if ( g_pX2Game->GetMyUnit() != NULL && g_pX2Game->GetMyUnit()->GetNowHp() > 0 )
		{
			m_pDLGExitMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_677 ), PGUCM_GAME_EXIT_OK, this, PGUCM_GAME_EXIT_CANCEL );
		}
		else
		{
			m_pDLGExitMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_678 ), PGUCM_GAME_EXIT_OK, this, PGUCM_GAME_EXIT_CANCEL );
		}
	}
	else
	{
		m_pDLGExitMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_677 ), PGUCM_GAME_EXIT_OK, this, PGUCM_GAME_EXIT_CANCEL );
	}

	if( m_pDLGExitMsgBox != NULL )
	{
		CKTDGUIButton* pCancelButton = (CKTDGUIButton*)m_pDLGExitMsgBox->GetControl( L"MsgBoxOkAndCancelCancelButton" );		
		pCancelButton->RequestFocus();
	}
}



/*virtual*/
void CX2StatePVPGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	if( NULL == g_pData->GetPVPRoom() )
		return; 

	CX2Room::SlotData* pkSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( iUnitUID_ );
	if ( pkSlotData == NULL )
		return;

	int iSlotIndex = pkSlotData->m_Index;
	if( iSlotIndex < 0 )
		return;




	if( NULL != m_pPVPGame )
	{
		CX2GUUser* pGUUser = m_pPVPGame->GetUserUnitByUID( iUnitUID_ );
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
				pGUUser->PlayEmotion( eId, wstrName );
			}
#endif			
		}
	}
}


#ifdef DUNGEON_ITEM
/*virtual*/ bool CX2StatePVPGame::Handler_EGS_GET_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//KSerBuffer* pBuff = (KSerBuffer*)lParam;
	//KEGS_GET_ITEM_NOT kEvent;
	//DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	if ( true == CX2State::Handler_EGS_GET_ITEM_NOT( hWnd, uMsg, wParam, lParam ) )
	{
		m_pPVPGame->SetSepcialItemId( 0 );
		m_pPVPGame->ResetCreateItemTimer();
		
		m_pPVPGame->SetSepcialItemUid( 0 );

		return true;
	}
	else
	{
		return false;
	}
}

bool CX2StatePVPGame::Handler_EGS_CREATE_PVP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_PVP_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_pPVPGame != NULL )
	{
		m_pPVPGame->SetSepcialItemId(0);
		m_pPVPGame->SetSepcialItemUid(0);
	}

	if( g_pMain->DeleteServerPacket( EGS_CREATE_PVP_ITEM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2StatePVPGame::Handler_EGS_CREATE_PVP_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_PVP_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_pPVPGame != NULL )
	{	
		int itemId = kEvent.m_kDropItemData.m_iItemID;
		int itemUid = kEvent.m_kDropItemData.m_iDropItemUID;
		int startPos = kEvent.m_iPositionIndex;

		m_pPVPGame->CreateSpecialItem(itemId, itemUid, startPos);
	}

	return true;
}

bool CX2StatePVPGame::Handler_EGS_CHANGE_PVP_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_pPVPGame != NULL )
	{	
		m_pPVPGame->ChangeSpecialItem(kEvent.m_iItemID, kEvent.m_iDropItemUID);
	}		

	return true;
}

bool CX2StatePVPGame::Handler_EGS_CHANGE_PVP_ITEM_POS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ITEM_POS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		return true;
	}

	return true;
}
bool CX2StatePVPGame::Handler_EGS_CHANGE_PVP_ITEM_POS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PVP_ITEM_POS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_pPVPGame != NULL )
	{
		m_pPVPGame->Handler_EGS_CHANGE_PVP_ITEM_POS_NOT( kEvent.m_iItemUID, kEvent.m_iPos );
	}
	return true;
}
#endif //DUNGEON_ITEM

#ifdef SERV_PVP_NEW_SYSTEM
bool CX2StatePVPGame::Handler_EGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ( UidType iUid, int iStartPos )
{
	KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ kEGS_INTRUDE_START_REQ;
	
	kEGS_INTRUDE_START_REQ.m_iPvpNpcUnitUID = iUid;
	kEGS_INTRUDE_START_REQ.m_iStartPosIndex = iStartPos;

	g_pData->GetServerProtocol()->SendPacket( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ, kEGS_INTRUDE_START_REQ );
	g_pMain->AddServerPacket( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK );

	return true;
}
bool CX2StatePVPGame::Handler_EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}
bool CX2StatePVPGame::Handler_EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ()
{
	KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ kEGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ;

	std::vector< CX2Room::RoomNpcSlot > &vecNpcSlot = g_pX2Room->GetNpcSlot();
	if( vecNpcSlot.size() <= 0 )
		return true;

	for(UINT i=0; i<vecNpcSlot.size(); ++i)
	{
		CX2GUNPC *pNpc = g_pX2Game->GetNPCUnitByUID( (int)vecNpcSlot[i].m_iNpcUid );
		if( pNpc != NULL )
		{
			kEGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ.m_mapPvpNpcInfo.insert( std::make_pair( pNpc->GetUID(), pNpc->GetNowHp() ) );
		}
	}

	if( kEGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ.m_mapPvpNpcInfo.empty() == true )
		return true;

	g_pData->GetServerProtocol()->SendPacket( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ, kEGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ );
	g_pMain->AddServerPacket( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, 60.0f );

	return true;
}
bool CX2StatePVPGame::Handler_EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK kPacket;
	DeSerialize( pBuff, &kPacket );
	CX2PacketLog::PrintLog( &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{			
		}
	}

	return true;
}
#endif

/** @function : Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT
	@brief : 같은 Room(개념으로써의)내에 있는 유저중에서 장비를 변경 한 경우에 처리하는 패킷
	@param : HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
	@return : 패킷 처리에 성공한 경우 true, 실패한 경웨 false
*/
bool CX2StatePVPGame::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pX2Room )
		return g_pX2Room->Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( kEvent );
	else
		return false;
}
#ifdef PLAY_EMOTION_BY_USER_SELECT
/*virtual */void CX2StatePVPGame::PlayEmotionByUserSelect()
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
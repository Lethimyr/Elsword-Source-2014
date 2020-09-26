#include "StdAfx.h"

#include ".\x2statedungeongame.h"




#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#include "./X2GUElsword_SwordMan.h"
#include "./X2GUArme_VioletMage.h"
#include "./X2GULire_ElvenRanger.h"
#include "./X2GURaven.h"
#include "./X2GuEve.h"

//{{ kimhc // 2010.11.25 // 2010-12-23 New Character CHUNG //
#ifdef	NEW_CHARACTER_CHUNG
#include "./X2GUChung.h"
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.25 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
#include "./X2GUAra.h"
#endif

#ifdef NEW_CHARACTER_EL
#include "./X2GUEl.h"
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
#include "./X2GUAdd.h"
#endif //SERV_9TH_NEW_CHARACTER


#endif REDUCED_PRECOMPILED_HEADER_TEST

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



CX2StateDungeonGame::CX2StateDungeonGame(void)
	: m_pDLGExitMsgBox( NULL )
	, m_pDLGLoadingState( NULL )
	, m_pDLGTutorialInputControl( NULL )
	, m_pDLGGoToFirstDungeon( NULL )
	, m_pDLGInputUI( NULL )
	, m_pDLGTutorialExitButton( NULL )
	//, m_pDLGF1Help( NULL )
	, m_pDLGF1Help2( NULL )
	, m_pDLGArcadeFrame( NULL )
	, m_pDungeonGame( NULL )
	, m_bLoadingOK( false )
	, m_pCX2GameLoadingTip( NULL )
	, m_HostIP( L"" )
	, m_HostPort( 0 )
	, m_bSubStageLoading( false )
	, m_SubStageLoadingRenderCount( 0 )
	//, m_WinTeam( CX2Room::TN_NONE )
	, m_bResultStateReq( false )
	, m_bGameEnd( false )
	, m_fGameEndWaitTime( 5.f )
	, m_pLoadingOutLineTex( NULL )
	, m_pLoadingBasicTex( NULL )
	, m_LoadingPercent( 0 )
	, m_LoadingRenderCount( 0 )
	, m_ConnectType( CX2Game::CT_START_GAME )
	, m_NextStageNum( -2 )
	, m_RenderCount( 0 )
#ifdef REFORM_ENTRY_POINT
	, m_pDLGLoadingFront( NULL )
#else //REFORM_ENTRY_POINT
	, m_DLGLoadingStateUnitInfoList( NULL )
#endif //REFORM_ENTRY_POINT
	, m_TutorialBeforeUserStateID( -1 )
	//, m_vecTutorialInputTreeParticle;
	, m_vTutorialParticlePos( D3DXVECTOR3( 80, 700, 0 ) )
	, m_bShowSkilList( false )
	, m_pFontForSkillList( NULL )
	, m_HighLightSkillID( -1 )
	, m_fTimeLeftForHighlight( 0.f )
	, m_coHighlight( D3DXCOLOR(1,1,1,1) )
	, m_coHighLightIncrement( D3DXCOLOR(0.04f, 0.04f, 0.04f, 0.f) )
	, m_iTutorialMsgFlashCount( 0 )
	, m_bCreateCommandSuccessSplash( false )
	, m_hTutorialMissionSplashParticle( INVALID_PARTICLE_SEQUENCE_HANDLE )
	//, m_mapUnitCommandList;
	, m_pDungeonMapUI( NULL )
	, m_bWasQuestUIMinimized( false )
	, m_bWasMiniMapMinimized( false )
	, m_bSend_KEGS_END_GAME_REQ( false )
	, m_bStateChangeLocalMap( false )
	, m_bSend_KEGS_LEAVE_ROOM_REQ( false )
	, m_bReceive_KEGS_LEAVE_ROOM_ACK( false )
	, m_bReceive_KEGS_UNIT_INFO_UPDATE( false )
	, m_bLeaveRoomAtTutorial( false )
	//, m_DungeonEndingEvent;
	, m_pDLGArcadeMission( NULL )
	, m_fTimeShowArcadeMission( 0.f )
#ifdef NEW_HENIR_TEST
	, m_iStartSecretStageEnteringEvent( 0 )
#endif NEW_HENIR_TEST
	, m_bAutoShowOffArcadeMission( true )
	, m_TimerWaitingPortal(3.f)
	, m_RenderStateID(s_akStates)
	, m_TexDataMovingGageBG( L"DLG_LOADING_BAR.tga", L"LOADING_BG" )
	, m_TexDataMovingGage( L"DLG_LOADING_BAR.tga", L"LOADING_BAR" )
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	, m_pDLGValentineTimer ( NULL )		/// 발렌타인 던전 타이머 UI
	, m_fValentineDungeonRemainTime ( 0.f )
	, m_iValentineDungeonStage ( 1 )
	, m_bIsValentineDungeon( false )
	, m_hStageStartEffect1( INVALID_PARTICLE_SEQUENCE_HANDLE )
	, m_hStageStartEffect2( INVALID_PARTICLE_SEQUENCE_HANDLE )
#endif //SERV_EVENT_VALENTINE_DUNGEON
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
	, m_bEnterDefenceDungeon( false )		/// 어둠의 문 입장 처리 여부
	, m_fWaitDefenceDungeonStartTime( -1.f )	/// 어둠의 문 시작까지의 대기 시간
#endif // SERV_NEW_DEFENCE_DUNGEON
#ifdef DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
	, m_SumDelta( 0 )
#endif //DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
{
	DialogLog( "\nCX2StateDungeonGame::CX2StateDungeonGame Start\n" );

	m_pCX2GameLoadingTip	= new CX2GameLoadingTip();
#ifdef REFORM_ENTRY_POINT
	if( NULL != m_pCX2GameLoadingTip && NULL != g_pData->GetPartyManager() && NULL != g_pData->GetPartyManager()->GetMyPartyData() )
	{
		if( true == CX2Dungeon::IsHenirDungeon( static_cast<const SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) ) 
			m_pCX2GameLoadingTip->SetVillageSetting();
	}
#endif //REFORM_ENTRY_POINT

#ifdef SERV_CATCH_HACKUSER_INFO
	if ( g_pData->GetDungeonRoom() == NULL )
	{
		KEGS_CATCH_HACKUSER_INFO_NOT kPacket;
		kPacket.m_iUserUID = g_pData->GetMyUser()->GetUID();
		kPacket.m_iUnitUID = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		kPacket.m_iCrashType = 1;

		Handler_EGS_CATCH_HACKUSER_INFO_NOT(kPacket);
	}
#endif SERV_CATCH_HACKUSER_INFO

	CX2DungeonRoom* pDungeonRoom = g_pData->GetDungeonRoom();

	if (  NULL != pDungeonRoom )
	{
		const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( pDungeonRoom->GetDungeonID() );
		if ( pDungeonData != NULL )
		{
			wstringstream dungeonName;
			dungeonName << pDungeonData->m_DungeonName.c_str() << L" ";
			if ( g_pData->GetDungeonRoom()->GetDifficulty() == CX2Dungeon::DL_NORMAL )
			{
				dungeonName << GET_STRING( STR_ID_385 );
			}
			else if (  g_pData->GetDungeonRoom()->GetDifficulty() == CX2Dungeon::DL_HARD )
			{
				dungeonName << GET_STRING( STR_ID_386 );
			}
			else if (  g_pData->GetDungeonRoom()->GetDifficulty() == CX2Dungeon::DL_EXPERT )
			{
				dungeonName << GET_STRING( STR_ID_388 );
			}

			StateLog( dungeonName.str().c_str() );
			StateLog( L"StartPlayerList" );

			for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
			{
				CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );
				if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
					continue;

				StateLog( pSlotData->m_pUnit->GetNickName() );
			}
		}
	}





#ifdef OLD_COMBO_LIST_UI

	//{{AFX
	// skill list 관련 string 초기화
	if( true == g_pMain->GetIsPlayingTutorial() ||
		( NULL != g_pData->GetDungeonRoom() && 
		( SEnum::DI_EL_FOREST_GATE_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() ||
		SEnum::DI_EL_FOREST_WEST_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() )
		) )
	{
		CreateComboListForTutorial();
	}
	//}}AFX

#endif OLD_COMBO_LIST_UI

#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	DeallocateFaultyPlayerWarningDlg();		
#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

	LoadUI();
	OnFrameMove( 0.0f, 0.0f );
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
	m_LoadingPercent				= 10;
	if( g_pData->GetDungeonRoom()->GetIntrudeGame() == false )
		GameLoadingReq( m_LoadingPercent );


	CreateGame();
	OnFrameMove( 0.0f, 0.0f );
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
	m_LoadingPercent				= 30;
	if( g_pData->GetDungeonRoom()->GetIntrudeGame() == false )
		GameLoadingReq( m_LoadingPercent );

	GameLoadingStart();
	OnFrameMove( 0.0f, 0.0f );
	g_pKTDXApp->GetDevice()->BeginScene();
	OnFrameRender();
	g_pKTDXApp->GetDevice()->EndScene();
	g_pKTDXApp->GetDevice()->Present( NULL, NULL, NULL, NULL );
	m_LoadingPercent				= 80;
	if( g_pData->GetDungeonRoom()->GetIntrudeGame() == false )
	{
		GameLoadingReq( m_LoadingPercent );
	}


	// tutorial 관련
	m_pFontForSkillList			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_20_BOLD );


	g_pKTDXApp->GetDGManager()->SetProjection( 
		g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(), true );

	g_pData->PlayLobbyBGM( NULL, false );

	SoundReady( L"Go.ogg" );
	SoundReady( L"UseItem.ogg" );




	SetOpenCommonBG( false );

	// 09.04.27 태완 : 특정던전에서 퀵슬롯 못쓰게
#ifdef CHILDRENS_DAY_EVENT_DUNGEON
	if( NULL != g_pData->GetDungeonRoom() )
	{
		switch(g_pData->GetDungeonRoom()->GetDungeonID())
		{
		case SEnum::DI_EVENT_KIDDAY_RUBEN:
		case SEnum::DI_EVENT_KIDDAY_ELDER:
		case SEnum::DI_EVENT_KIDDAY_BESMA:
		case SEnum::DI_EVENT_KIDDAY_ALTERA:
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
		case SEnum::DI_EVENT_HALLOWEEN_DAY:
#endif //SERV_HALLOWEEN_EVENT_2013
			{
				g_pData->GetUIManager()->SetShowQuickSlot(false);
			} break;
		default:
			break;
		}
	}
#endif CHILDRENS_DAY_EVENT_DUNGEON

	if( true == CX2Dungeon::IsHenirDungeon( static_cast<const SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) &&
		CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
	{
		g_pData->GetUIManager()->SetShowQuickSlot(false);
	}

	if( NULL != g_pMain && true == g_pMain->GetIsPlayingTutorial() )
	{
		CreateMovingSmallBar();
	}

	m_bLoadingOK = true;	// 여기까지 실행되면 로딩이 완료된 것으로 
	g_pKTDXApp->SkipFrame();

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

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-07
	/// 만약 어둠의 문 입장중이라면, 소환 퀵슬롯 설정
	if ( NULL != g_pData && 
		NULL != g_pData->GetDungeonRoom() &&
		NULL != g_pData->GetUIManager() && 
		NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
	{
		if ( true == g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) )
		{
			g_pData->GetUIManager()->GetUIQuickSlot()->SetIsSummonCardSlot( true );
			g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
			g_pData->GetUIManager()->GetUIQuickSlot()->SetShow( true );
		}
	}
#endif // SERV_NEW_DEFENCE_DUNGEON

	DialogLog( "\nCX2StateDungeonGame::CX2StateDungeonGame End\n" );

}

CX2StateDungeonGame::~CX2StateDungeonGame(void)
{
	DialogLog( "\nCX2StateDungeonGame::~CX2StateDungeonGame Start\n" );
	



#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-07
	if(	NULL != g_pData && NULL != g_pData->GetUIManager() )
	{
		CX2UIQuickSlot* pQuickSlot = g_pData->GetUIManager()->GetUIQuickSlot();
		if( NULL != pQuickSlot )
		{
			if ( NULL != g_pData->GetDungeonRoom() && 
				true == g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) )
			{
				pQuickSlot->SetIsSummonCardSlot( false );
				pQuickSlot->ResetQuickSlotUI();
				pQuickSlot->SetShow( true );

				if( NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					const int iMaxQuickSlotNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemMaxNum( CX2Inventory::ST_E_QUICK_SLOT );
					pQuickSlot->SetExpandQuickSlot(iMaxQuickSlotNum);
				}
			}

			pQuickSlot->SetEnable(true);
		}
	}
#else // SERV_NEW_DEFENCE_DUNGEON
	// 09.04.27 태완 : 아까 못쓰게 했던 퀵슬롯 원래대로 돌려주자
//#ifdef CHILDRENS_DAY_EVENT_DUNGEON
	if(	g_pData->GetUIManager() != NULL && 
		g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
	{
		g_pData->GetUIManager()->GetUIQuickSlot()->SetEnable(true);
	}
//#endif
#endif // SERV_NEW_DEFENCE_DUNGEON


#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	SAFE_DELETE_DIALOG( m_pDLGFaultyPlayerWarning );
#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG


	SAFE_CLOSE( m_pLoadingOutLineTex );
	SAFE_CLOSE( m_pLoadingBasicTex );
	SAFE_DELETE_DIALOG( m_pDLGExitMsgBox );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    SAFE_DELETE_DIALOG_HANDLE( m_pDLGLoadingState );
#else
	SAFE_DELETE( m_pDLGLoadingState ); // note: 얘는 다이얼로그 매니저에서 관리되는 애가 아니라서~ 요렇게
#endif // DIALOG_HANDLE_TEST // 2009-8-19

	SAFE_DELETE( m_pCX2GameLoadingTip );

	SAFE_DELETE_DIALOG( m_pDLGF1Help );
	SAFE_DELETE_DIALOG( m_pDLGF1Help2 );

	SAFE_DELETE_DIALOG( m_pDLGTutorialExitButton );

	SAFE_DELETE_DIALOG( m_pDLGTutorialInputControl );


	SAFE_DELETE_DIALOG( m_pDLGGoToFirstDungeon );

	SAFE_DELETE_DIALOG( m_pDLGInputUI );
	SAFE_DELETE_DIALOG( m_pDLGArcadeFrame );
	SAFE_DELETE_DIALOG( m_pDLGArcadeMission );
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	SAFE_DELETE_DIALOG( m_pDLGValentineTimer ); /// 발렌타인 던전 타이머 UI
#endif //SERV_EVENT_VALENTINE_DUNGEON

	ClearTutorialInputTreeParticle();
	

	SAFE_DELETE( m_pDungeonMapUI );

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hTutorialMissionSplashParticle )
	{
		if( NULL != g_pX2Game && NULL != g_pX2Game->GetMajorParticle() )
		{
			g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hTutorialMissionSplashParticle );
		}
	}

#ifdef REFORM_ENTRY_POINT
	SAFE_DELETE_DIALOG( m_pDLGLoadingFront );
#else //REFORM_ENTRY_POINT
	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
        SAFE_DELETE_DIALOG_HANDLE( pDialog );
#else
		SAFE_DELETE( pDialog );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
	}
#endif //REFORM_ENTRY_POINT

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );

	m_pDungeonGame->Release();
	SAFE_DELETE( m_pDungeonGame );
	
	DestroyMovingSmallBar();
	DialogLog( "\nCX2StateDungeonGame::~CX2StateDungeonGame End\n" );

#ifdef SERV_EVENT_VALENTINE_DUNGEON		/// 스테이지 변경시 발생되는 안내 문구 이펙트 해제
	if( NULL != g_pData->GetUIMajorParticle() )		
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hStageStartEffect1 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hStageStartEffect2 );
	}
#endif SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-07
	/// 룸을 나가면 무조건 어둠의 문 처리 해제
	if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
	{
		g_pData->GetUIManager()->GetUIQuickSlot()->SetIsSummonCardSlot( false );
		g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
		g_pData->GetUIManager()->GetUIQuickSlot()->SetShow( true );
	}
#endif // SERV_NEW_DEFENCE_DUNGEON
}


void CX2StateDungeonGame::LoadUI()
{
	if( NULL != g_pData->GetDungeonRoom() )
	{
		if( SEnum::DI_EL_FOREST_GATE_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() ||
			SEnum::DI_EL_FOREST_WEST_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() )
		{
			m_pDLGInputUI = new CKTDGUIDialog( this, L"DLG_Novice_InputUI.lua", 0.07f );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInputUI );

			//if( SEnum::DI_EL_FOREST_WEST_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() )
			//{
			//	// 메인메뉴 높이만큼 위로 올린다.
			//	m_pDLGInputUI->SetPos(D3DXVECTOR2(0.f, -50.f));
			//}



#ifdef OLD_COMBO_LIST_UI

//{{AFX
			CKTDGUIStatic* pStatic_SkillList_Fold	= (CKTDGUIStatic*) m_pDLGInputUI->GetControl( L"Static_Skill_List_Fold" );
			CKTDGUIStatic* pStatic_SkillList_Unfold	= (CKTDGUIStatic*) m_pDLGInputUI->GetControl( L"Static_Skill_List_Unfold" );

			if( true == m_bShowSkilList )
			{
				pStatic_SkillList_Fold->SetShow( false );
				pStatic_SkillList_Unfold->SetShow( true );
			}
			else
			{
				pStatic_SkillList_Fold->SetShow( true );
				pStatic_SkillList_Unfold->SetShow( false );	
			}


			if( NULL != pStatic_SkillList_Unfold->GetString(0) )
			{
				wstringstream wstrmCommandList;

				map<char, wstring>& mapCommandList = m_mapUnitCommandList[ g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass ];
				map<char, wstring>::iterator it;

				for( it = mapCommandList.begin() ; it != mapCommandList.end(); it++ )
				{
					wstrmCommandList << it->second.c_str() << std::endl;
				}

				pStatic_SkillList_Unfold->GetString(0)->msg = wstrmCommandList.str();
			}
//}}AFX
#endif OLD_COMBO_LIST_UI


		}

	}

	m_pLoadingOutLineTex	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"HQ_LoadingOutLine.tga" );


	// D:\ProjectX2_SVN\resource\KR\Trunk\dat\Texture\UI에 파일들이 있습니다
	std::vector< std::wstring > vecLoadingTextureName;
	vecLoadingTextureName.reserve( 16 );
#ifdef DUNGEON_LOADING_ADVERTISEMENT
	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
    if ( g_pKTDXApp->LoadAndDoMemory( &kLuamanager, L"Dungeon_Loading_Advertisement.lua" ) == true )
	{
		if ( true == kLuamanager.BeginTable( "TEXTURE_NAME" ) )
		{
			int tableIndex = 1;
			wstring textureName;
			while( kLuamanager.GetValue( tableIndex, textureName ) == true )
			{
				vecLoadingTextureName.push_back( textureName );
				tableIndex++;
			}
			kLuamanager.EndTable();
		}
	}
	if( true == vecLoadingTextureName.empty() )
	{
		vecLoadingTextureName.push_back( L"HQ_LoadingBasic_111.tga" );
	}
#else
	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_111.tga" );
	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_222.tga" );
	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_333.tga" );
	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_444.tga" );
	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_555.tga" );
	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_666.tga" );
	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_777.tga" );
//	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_888.tga" );
//	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_999.tga" );
// 	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_1000.tga" );
// 	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_1001.tga" );
// 	vecLoadingTextureName.push_back( L"HQ_LoadingBasic_1002.tga" );
#endif //DUNGEON_LOADING_ADVERTISEMENT

	int iRandomLoadingImageIndex = rand() % (int) vecLoadingTextureName.size();
	m_pLoadingBasicTex = g_pKTDXApp->GetDeviceManager()->OpenTexture( vecLoadingTextureName[ iRandomLoadingImageIndex ] );


	m_pDLGLoadingState = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State.lua", 0.0f );
	m_pDLGLoadingState->SetColor( D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) );



	//{{ 2009.01.22 김태완 : 던전별 로딩화면
#ifdef ENTER_SCREEN_FOR_DUNGEON
	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( g_pData->GetDungeonRoom()->GetDungeonID() );
	if ( pDungeonData != NULL )
	{
		bool bCheck = false;
		if(pDungeonData->m_UIData.m_LoadingScreenFileName != L"" )
		{
			CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGLoadingState->GetControl( L"Static_Loading_Dungeon_BG" );

			if(pDungeonData->m_UIData.m_LoadingScreenFileName2 != L"" )
			{
				// 그림 두장
				if( pStatic != NULL )
				{
					if(pStatic->GetPicture(0) != NULL)
					{
						pStatic->GetPicture(0)->SetShow(false);
					}
					bool bCheck1 = false;
					bool bCheck2 = false;
					if(pStatic->GetPicture(1) != NULL)
					{
						bCheck1 = pStatic->GetPicture(1)->SetTex( pDungeonData->m_UIData.m_LoadingScreenFileName.c_str() );
					}
					if(pStatic->GetPicture(2) != NULL)
					{
						bCheck2 = pStatic->GetPicture(2)->SetTex( pDungeonData->m_UIData.m_LoadingScreenFileName2.c_str() );
					}
					bCheck = bCheck1 && bCheck2;
				}				
			}
			else
			{				
				// 그림 한장				
				if( pStatic != NULL )
				{
					if(pStatic->GetPicture(0) != NULL)
					{
						bCheck = pStatic->GetPicture(0)->SetTex( pDungeonData->m_UIData.m_LoadingScreenFileName.c_str() );
					}
					if(pStatic->GetPicture(1) != NULL)
						pStatic->GetPicture(1)->SetShow(false);
					if(pStatic->GetPicture(2) != NULL)
						pStatic->GetPicture(2)->SetShow(false);
				}
			}


			if(bCheck == true)
			{
				// 정상적으로 그림이 나오는 상황 : LoadingTip Object를 한줄팁 표시로 바꾸자.
				pStatic->SetShow(true);

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-17
				bool bIsDefenceDungeon = false;		/// 어둠의 문인가

				if ( NULL != g_pData &&
					 NULL != g_pData->GetDungeonRoom() &&
					 SEnum::DI_DEFENCE_DUNGEON_ELDER_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() )
				{
					 bIsDefenceDungeon = true;
				}

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
				bool bIsHalloweenDungeon = false;		/// 어둠의 문인가

				if ( NULL != g_pData &&
					NULL != g_pData->GetDungeonRoom() &&
					SEnum::DI_EVENT_HALLOWEEN_DAY == g_pData->GetDungeonRoom()->GetDungeonID() )
				{
					bIsHalloweenDungeon = true;
				}

				if ( false == bIsDefenceDungeon && false == bIsHalloweenDungeon )		/// 어둠의 문, 할로윈이 아닐때만, 한줄 툴팁 설정
#else //SERV_HALLOWEEN_EVENT_2013
				if ( false == bIsDefenceDungeon )		/// 어둠의 문이 아닐때만, 한줄 툴팁 설정
#endif //SERV_HALLOWEEN_EVENT_2013
#endif // SERV_NEW_DEFENCE_DUNGEON
				{
					// 로딩팁 생성
#ifdef HIDE_LOADING_TIP
				if( pDungeonData->m_bHideLoadingTip == false )
#endif HIDE_LOADING_TIP					
					m_pCX2GameLoadingTip->CreateLoadingOneLineTip();
				}
			}
			else
			{
				// 텍스쳐가 정상로딩이 안 되었거나 스태틱이 상태가 안 좋거나.. 아무튼 안 좋은 상황.
				// 기존의 랜덤 로딩화면을 불러준다
				// ASSERT( !"Dungeon Loading Screen Texture Load Failed" );
				ErrorLogMsg( XEM_ERROR134, pDungeonData->m_UIData.m_LoadingScreenFileName.c_str() );
			}

		}

		if (false == bCheck)
		{
			// 없는 경우 : 기존의 랜덤 로딩화면을 사용. 기존처럼 열어 준다.
			// 랜덤 로딩 화면 
			const int nLoadingBG = m_pDLGLoadingState->GetDummyInt(0);
			int iRandBG = rand()%nLoadingBG;

			if( true == g_pMain->GetIsPlayingTutorial() )
			{
				iRandBG = g_pMain->GetTutorialRandomLoadingBG();
			}

			wstringstream wstmStaticName;
			wstmStaticName << L"Static_Loading_BG";
			wstmStaticName << iRandBG;

			m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );
			//m_pDLGLoadingState->GetControl( L"DungeonLoadingBG" )->SetShow( true );

			// 로딩팁 화면 생성. ( ENTER_SCREEN_FOR_DUNGEON가 define 되어있지 않으면 CreateLoadingTipUI가 로딩팁의 생성자에서 빠진다. )
			m_pCX2GameLoadingTip->CreateLoadingTipUI();
		}
	}
#else
	// 랜덤 로딩 화면 
	const int nLoadingBG = m_pDLGLoadingState->GetDummyInt(0);
	int iRandBG = rand()%nLoadingBG;

	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		iRandBG = g_pMain->GetTutorialRandomLoadingBG();
	}

	wstringstream wstmStaticName;
	wstmStaticName << L"Static_Loading_BG";
	wstmStaticName << iRandBG;

	m_pDLGLoadingState->GetControl( wstmStaticName.str().c_str() )->SetShow( true );
	//m_pDLGLoadingState->GetControl( L"DungeonLoadingBG" )->SetShow( true );
#endif
	//}}

	// note!! 신캐릭터 추가되면 추가해야함
	/*enum LOADING_USER_INFO
	{
		LUI_ELSWORD_COLOR = 0,
		LUI_ARME_COLOR,
		LUI_LIRE_COLOR,
		LUI_RAVEN_COLOR,
		LUI_EVE_COLOR,
		LUI_CHUNG_COLOR,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_COLOR,
#endif

		LUI_ELSWORD_BLACK,
		LUI_ARME_BLACK,
		LUI_LIRE_BLACK,
		LUI_RAVEN_BLACK,
		LUI_EVE_BLACK,
		LUI_CHUNG_BLACK,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_BLACK,
#endif
		
		LUI_EMPTY,
	};
	*/

#ifdef REFORM_ENTRY_POINT
	if( NULL == m_pDLGLoadingFront )
	{
		m_pDLGLoadingFront = new CKTDGUIDialog( this, L"DLG_Loading_Front.lua", 0.0f );

		if( NULL != m_pDLGLoadingFront )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGLoadingFront );
			SetLoadingGageBar(0);
		}
	}
#else //REFORM_ENTRY_POINT
	for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
	{
		CKTDGUIDialogType pDialog = new CKTDGUIDialog( this, L"DLG_PVP_Game_Loading_State_User.lua", 0.0f );
		pDialog->SetPos( D3DXVECTOR2( pDialog->GetDummyPos(i + 4).x, pDialog->GetDummyPos(i + 4).y ) );

		CKTDGUIStatic* pStaticUnitInfoTex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoadingUnitInfoTex" );

		CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );

		// note!! 전직 추가되면 수정, 혹은 unit_type으로
		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		if ( pSlotData->m_pUnit == NULL )
		{
			pStaticUnitInfoTex->GetPicture( LUI_EMPTY )->SetShow( true );
			m_DLGLoadingStateUnitInfoList.push_back( pDialog );
			continue;
		}

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
		case CX2Unit::UT_ADD:
			{
				pStaticUnitInfoTex->GetPicture( LUI_ADD_BLACK )->SetShow( true );
			} break;
#endif //SERV_9TH_NEW_CHARACTER

		default:
			{
				ASSERT( !"Unexpected UnitClass" );
			} break;

		} //}}




		CKTDGUIStatic* pStaticUnitInfoPercent = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_Percent" );
		pStaticUnitInfoPercent->GetString( 0 )->msg = L"0%";
		CKTDGUIStatic* pStaticUnitInfo = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_UserInfo" );
		WCHAR buff[256] = {0};
		StringCchPrintf( buff, 256, L"LV.%d %s", (int)pSlotData->m_pUnit->GetUnitData().m_Level, pSlotData->m_pUnit->GetNickName() );
		//wsprintf( buff, L"LV.%d %s", (int)pSlotData->m_pUnit->GetUnitData().m_Level, pSlotData->m_pUnit->GetNickName() );
		pStaticUnitInfo->GetString( 0 )->msg = buff;

		m_DLGLoadingStateUnitInfoList.push_back( pDialog );
	}	
#endif //REFORM_ENTRY_POINT

	//int redTeam = 0; 
	//int blueTeam = 4;
	//int index = -1;

	//wstring tempInit = L" : 로딩중.. 0%";
	//wstring tempString = L"";

	//for( int i= 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
	//{
	//	//UI에 그리고 있던 모델을 뺀다
	//	CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );		
	//	if( pSlotData == NULL )
	//		continue;

	//	if ( pSlotData->m_pUnit == NULL )
	//		continue;

	//	if ( i >= 0 && i <= 3 )
	//	{
	//		index = redTeam;
	//	}
	//	else
	//	{
	//		index = blueTeam;
	//	}

	//	CKTDGUIStatic* pStaticLoadingState = (CKTDGUIStatic*)m_pDLGLoadingState->GetControl(L"StaticPVPGameLoadingState");
	//	tempString = pSlotData->m_pUnit->GetNickName() + tempInit;
	//	pStaticLoadingState->GetString(index)->msg = tempString;

	//	if ( i >= 0 && i <= 3 )
	//	{
	//		redTeam++;
	//	}
	//	else
	//	{
	//		blueTeam++;
	//	}

	//	//g_pKTDXApp->GetDGManager()->RemoveObjectChain( pSlotData->pUnitViewer );
	//}

	m_LoadingRenderCount = g_pKTDXApp->GetRenderCount();
}

void CX2StateDungeonGame::CreateGame()
{
	m_pDungeonGame = new CX2DungeonGame();

	m_LoadingRenderCount = g_pKTDXApp->GetRenderCount();
}

void CX2StateDungeonGame::GameLoadingStart()
{
	m_ConnectType = CX2Game::CT_START_GAME;
	m_pDungeonGame->GameLoading( g_pData->GetDungeonRoom() );

    ASSERT( m_pDungeonMapUI == NULL );
	m_pDungeonMapUI			= new DungeonMapUI( m_pDungeonGame->GetDungeon(), (CX2State*) this );

	//m_pDungeonGame->P2PSetPeer();
	//m_pDungeonGame->P2PConnectTest();

	GetDungeonEndingEvent()->m_vecSpeech2.clear();
	GetDungeonEndingEvent()->m_vecSpeech2.push_back( g_pX2Game->GetMyUnit()->GetRandomWinSpeech() );

	m_LoadingRenderCount = g_pKTDXApp->GetRenderCount();
}



bool CX2StateDungeonGame::QuitGame()
{
	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		OpenExitDLG( GET_STRING( STR_ID_562 ) );
	}
	else
	{
		//{{ kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08
		if ( m_pDungeonGame != NULL &&
			m_pDungeonGame->GetCanUseEscFlag() == true )
		{
			OpenExitDLG( GET_STRING( STR_ID_2631 ) );
		}
		//}} kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08
	}

	m_pCursor->SetShow(true);

	return true;
}


HRESULT CX2StateDungeonGame::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );

	if ( m_bReceive_KEGS_LEAVE_ROOM_ACK == true && m_bStateChangeLocalMap == false )
	{
		if ( m_bReceive_KEGS_UNIT_INFO_UPDATE == true )
		{
			m_bStateChangeLocalMap = true;
			//StateChangeLocalMapReq();
			StateChangeFieldReq();
		}
		else if ( m_bLeaveRoomAtTutorial == true )
		{
			m_bStateChangeLocalMap = true;
			//StateChangeLocalMapReq();

			// 튜토리얼 끝내고 마을로 진입시 마을및 위치 지정
			int iMapId = 0, startPos = 0;
			D3DXVECTOR3 vStartPos( 0.0f, 0.0f, 0.0f );

			if(g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL)
				iMapId = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_nMapID;

			if(g_pData->GetLocationManager()->GetVillageMapTemplet((SEnum::VILLAGE_MAP_ID)iMapId) == NULL)
			{
				// 강제 루벤이동
				iMapId = SEnum::VMI_RUBEN;
				startPos = 1;
			}	
			else
			{
				startPos = g_pData->GetLocationManager()->GetLoginPos( iMapId );		
			}	

			vStartPos = g_pData->GetLocationManager()->GetStartPosLoc(startPos);
			g_pData->GetLocationManager()->SetVillage((SEnum::VILLAGE_MAP_ID)iMapId, vStartPos);
			g_pData->GetLocationManager()->SetLastPos(vStartPos);
			//g_pData->GetLocationManager()->SetLastPosLineIndex( );

			// 마을 이동 요청
			StateChangeFieldReq();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    MoveToRubenVillage( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	MoveToRubenVillage();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	if ( m_pDLGArcadeMission != NULL )
	{
		if ( m_bAutoShowOffArcadeMission == false )
		{
			m_fTimeShowArcadeMission += fElapsedTime;
			if ( m_fTimeShowArcadeMission > 4.0f )
			{
				m_bAutoShowOffArcadeMission = true;
				m_pDLGArcadeMission->SetShowEnable( false, false );
			}
		}
	}

#ifdef SERV_EPIC_QUEST
	if( g_pData->GetUIManager()->GetUIQuestNew() != NULL )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->OnFrameMove( fTime, fElapsedTime );
	}
#else
	// 09.01.05 김태완
	// 	if ( g_pMain->GetNewQuestUI() != NULL )
	// 		g_pMain->GetNewQuestUI()->OnFrameMove( fTime, fElapsedTime );
	if( g_pData->GetUIManager()->GetUIQuest() != NULL )
	{
		g_pData->GetUIManager()->GetUIQuest()->OnFrameMove( fTime, fElapsedTime );
	}
#endif SERV_EPIC_QUEST
	if( m_pDLGLoadingState != NULL && m_LoadingPercent < 100 )
	{
		m_pDLGLoadingState->OnFrameMove( fTime, fElapsedTime );

		if ( m_pCX2GameLoadingTip != NULL )
			m_pCX2GameLoadingTip->OnFrameMove( fTime, fElapsedTime );

#ifdef REFORM_ENTRY_POINT
		if( NULL != m_pDLGLoadingFront )
			m_pDLGLoadingFront->OnFrameMove( fTime, fElapsedTime );
#else //REFORM_ENTRY_POINT
		for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
		{
			CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
			pDialog->OnFrameMove( fTime, fElapsedTime );
		}
#endif //REFORM_ENTRY_POINT
	}


	if( g_pData->GetDungeonRoom() != NULL )
	{
		if( m_pDungeonGame != NULL )
		{
			if( m_bGameEnd == true )
			{
				m_fGameEndWaitTime -= m_fElapsedTime;
				if( m_fGameEndWaitTime < 0.0f && m_bResultStateReq == false )
				{
					m_bResultStateReq = true;

					if( g_pX2Game->IsHost() == true )
						StateChangeResultReq();					
				}
			}

			if( m_NextStageNum > -1 && m_RenderCount + 20 < g_pKTDXApp->GetRenderCount() )
			{
				if( m_pDungeonGame->GetGameState() == CX2Game::GS_LOADING )
				{
					m_pDungeonGame->StageLoading( m_NextStageNum );
					DungeonStageLoadCompleteReq( m_pDungeonGame->GetDungeon()->GetNowStage()->GetStageData()->m_bSecretStage );
		
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

					m_NextStageNum = -1;
				}
				if( g_pKTDXApp->GetDGManager()->GetIsFading() == false && m_pDungeonGame->GetGameState() != CX2Game::GS_LOADING )
				{
					m_pDungeonGame->SetGameState( CX2Game::GS_LOADING );

					g_pKTDXApp->GetDGManager()->StartFadeIn( 0.0f );

					m_RenderCount = g_pKTDXApp->GetRenderCount();
				}				

				return S_OK;
			}

			m_pDungeonGame->OnFrameMove( fTime, fElapsedTime );


			if( false == g_pMain->GetIsPlayingTutorial() )
			{
				GetDungeonEndingEvent()->OnFrameMove( fTime, fElapsedTime );

#ifdef HENIR_TEST
				GetSecretStageEnteringEvent()->OnFrameMove( fTime, fElapsedTime );
#endif HENIR_TEST
			}

			if( g_pData->GetGameUDP()->ConnectTestResult() == true && m_bLoadingOK == true )
			{
				switch( m_ConnectType )
				{
				case CX2Game::CT_CONNECT_SUCCESS:
					break;

				case CX2Game::CT_START_GAME:
					{
						if( g_pData->GetDungeonRoom()->GetIntrudeGame() == false )
							GameLoadingReq( 100 );
					}
					break;
				}	

				m_ConnectType = CX2Game::CT_CONNECT_SUCCESS;
			}







#ifdef OLD_COMBO_LIST_UI
			//{{AFX
			if( NULL != g_pData->GetDungeonRoom() )
			{
				if( SEnum::DI_EL_FOREST_GATE_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() ||
					SEnum::DI_EL_FOREST_WEST_NORMAL == g_pData->GetDungeonRoom()->GetDungeonID() )
				{
					if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_L) == TRUE && m_pDungeonGame->GetOpenChatBox() == false )
					{
						m_bShowSkilList = !m_bShowSkilList;

						if( NULL != m_pDLGInputUI )
						{
							CKTDGUIStatic* pStatic_SkillList_Fold	= (CKTDGUIStatic*) m_pDLGInputUI->GetControl( L"Static_Skill_List_Fold" );
							CKTDGUIStatic* pStatic_SkillList_Unfold	= (CKTDGUIStatic*) m_pDLGInputUI->GetControl( L"Static_Skill_List_Unfold" );

							pStatic_SkillList_Fold->SetShow( !m_bShowSkilList );
							pStatic_SkillList_Unfold->SetShow( m_bShowSkilList );
						}
					}

				}
			}
			//}}AFX
#endif OLD_COMBO_LIST_UI





#ifdef CREATE_MONSTER_LIST_TEST
			m_CreateMonsterListDialog.KeyProcess( this ); 
#endif CREATE_MONSTER_LIST_TEST

#ifdef MONSTER_STATE_LIST_TEST
			m_MonsterStateListDialog.KeyProcess( this );
#endif MONSTER_STATE_LIST_TEST


#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
			if ( NULL != m_pDLGFaultyPlayerWarning)	
			{
				if ( true == m_bFaultyPlayerWarningDlgExitButtonPressed )
				{
					DeallocateFaultyPlayerWarningDlg ();
				}
			}


#else  // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
			if ( true == m_BadAttitudeUserWarningDialog.IsOpen() )
			{
				m_BadAttitudeUserWarningDialog.Update( fElapsedTime );
			}

#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG



			// ping 
// 			for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
// 			{
// 				CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );
// 				if ( pSlotData->m_pUnit == NULL )
// 				{
// 					continue;
// 				}
// 
// 				CX2GUUser* pGUUser = m_pDungeonGame->GetUserUnitByUID( pSlotData->m_pUnit->GetUID() );
// 				if( NULL == pGUUser )
// 					continue;
// 
// 				if( pSlotData->m_PingTime < 60 )
// 				{
// 					pGUUser->GetGageManager()->SetPingLevel( 3 );
// 				}
// 				else if( pSlotData->m_PingTime < 120 )
// 				{
// 					pGUUser->GetGageManager()->SetPingLevel( 2 );
// 				}
// 				else if( pSlotData->m_PingTime < 10000 )
// 				{
// 					pGUUser->GetGageManager()->SetPingLevel( 1 );
// 				}
// 				else
// 				{
// 					pGUUser->GetGageManager()->SetPingLevel( 0 );
// 				}
// 			}	
		}
		g_pData->GetDungeonRoom()->OnFrameMove( fTime, fElapsedTime );

	}

#ifdef SERV_EVENT_VALENTINE_DUNGEON
	if( true == m_bIsValentineDungeon )
	{
		ValentineDungeonTimer_OnFrameMove(fElapsedTime);
	}
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-14
	WaitDefenceDungeonStart( fElapsedTime );		/// 특정 시간 이후 어둠의 문 스테이트 변경을 처리
#endif // SERV_NEW_DEFENCE_DUNGEON
	return S_OK;
}

HRESULT CX2StateDungeonGame::OnFrameRender()
{
	KTDXPROFILE();

	if( m_pDungeonGame != NULL )
	{
		if( m_pDungeonGame->GetGameState() != CX2Game::GS_PLAY )
		{
			if( m_NextStageNum == -2 )
			{
				if( m_pDLGLoadingState != NULL)
				{
					m_pDLGLoadingState->OnFrameRender();

					if ( m_pCX2GameLoadingTip != NULL )
						m_pCX2GameLoadingTip->OnFrameRender();

#ifdef REFORM_ENTRY_POINT
					if( NULL != m_pDLGLoadingFront )
						m_pDLGLoadingFront->OnFrameRender();
#else //REFORM_ENTRY_POINT
					for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
					{
						CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
						pDialog->OnFrameRender();
					}
#endif //REFORM_ENTRY_POINT
				}
			}
			else
			{
				if( m_pDungeonGame->GetLastKillCheck() == true && g_pKTDXApp->GetCannotStretchRect() == false )
	#ifdef SET_LAST_KILL_SHOT_HIDE_UI // 김태환		최대 화면 크기로 확대
					m_pDungeonGame->GetLastKillShot()->Draw( 0, 0, 1024, 768 );// CopySurfaceToBackBuffer();
	#else // SET_LAST_KILL_SHOT_HIDE_UI
					m_pDungeonGame->GetLastKillShot()->Draw( 95,92, 889,665 );// CopySurfaceToBackBuffer();
	#endif // SET_LAST_KILL_SHOT_HIDE_UI
				else
					m_pLoadingBasicTex->Draw( 0,0, 1024,768 );

				if( m_pLoadingOutLineTex != NULL )
					m_pLoadingOutLineTex->Draw( 0,0, 1024,768 );
			}
		}


		m_pDungeonGame->OnFrameRender();

		if ( m_pDungeonGame->GetGameState() != CX2Game::GS_PLAY )
		{
			if ( m_pDLGExitMsgBox != NULL )
				m_pDLGExitMsgBox->OnFrameRender();
		}

		if ( NULL != g_pMain && true == g_pMain->GetIsPlayingTutorial()
			&&m_TimerWaitingPortal.GetSumOfElapsedTime() > 0.0f )
		{
			DrawMovingSmallBar();
		}
	}
	else
	{
		if( m_pDLGLoadingState != NULL )
			m_pDLGLoadingState->OnFrameRender();

		if ( m_pCX2GameLoadingTip != NULL )
			m_pCX2GameLoadingTip->OnFrameRender();
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

	if ( NULL != m_pCursor )
		m_pCursor->OnFrameRender();

	return S_OK;
}

HRESULT CX2StateDungeonGame::OnResetDevice()
{
	if( m_pDungeonGame != NULL )
		m_pDungeonGame->OnResetDevice();
	if ( m_pDLGLoadingState != NULL)
	{
		m_pDLGLoadingState->OnResetDevice();
	}

#ifdef REFORM_ENTRY_POINT
	if( NULL != m_pDLGLoadingFront )
		m_pDLGLoadingFront->OnResetDevice();
#else //REFORM_ENTRY_POINT
	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
		if ( pDialog != NULL )
			pDialog->OnResetDevice();
	}
#endif //REFORM_ENTRY_POINT

	return S_OK;
}

HRESULT CX2StateDungeonGame::OnLostDevice()
{
	if( m_pDungeonGame != NULL )
		m_pDungeonGame->OnLostDevice();
	if ( m_pDLGLoadingState != NULL)
		m_pDLGLoadingState->OnLostDevice();

#ifdef REFORM_ENTRY_POINT
	if( NULL != m_pDLGLoadingFront )
		m_pDLGLoadingFront->OnLostDevice();
#else //REFORM_ENTRY_POINT
	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
		if ( pDialog != NULL )
			pDialog->OnLostDevice();
	}
#endif //REFORM_ENTRY_POINT


	return S_OK;
}


bool CX2StateDungeonGame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 개발자 모드일 경우
#ifndef _SERVICE_
	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )
	{
		switch( uMsg )
		{
		case WM_KEYDOWN:
			switch( wParam )
			{			
			case VK_F5:
				// 게임 클리어
				if( g_pX2Game->IsHost() == true )
				{
					switch( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_DUNGEON_GAME:
						{
							EndDungeonGameReq( CX2Room::TN_RED );

						} break;
					}
				}
				break;
			case VK_F4:
				// 게임 클리어(실패)
				if( g_pX2Game->IsHost() == true )
				{
					switch( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_DUNGEON_GAME:
						{
							EndDungeonGameReq( CX2Room::TN_MONSTER );

						} break;

					case CX2Main::XS_TRAINING_GAME:
						{
							CX2StateTrainingGame* pStateTrainingGame = (CX2StateTrainingGame*) g_pMain->GetNowState();
							pStateTrainingGame->Handler_EGS_END_TC_GAME_REQ( false );
						} break;
					}					
				}
				break;
			}		
			break;
		case WM_ACTIVATEAPP:
			{
				//if( wParam == FALSE )
				// 활성/비활성 시마다 포인터 변경한다.
				{					
					if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL && g_pX2Game->GetMyUnit()->GetGageData() )
					{
						g_pX2Game->GetMyUnit()->GetGageData()->SwapGage();
					}
				}
			}
			break;
			


		}	
	}
#endif

#ifdef SERV_EPIC_QUEST
	if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_NEW) )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->MsgProc(hWnd, uMsg, wParam, lParam );
	}
#else
	// 09.01.05 김태완
	// 	if ( g_pMain->GetNewQuestUI()->GetQuestPopUpWindow() != NULL )
	// 		g_pMain->GetNewQuestUI()->GetQuestPopUpWindow()->MsgProc( hWnd, uMsg, wParam, lParam );
	if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST) )
	{
		g_pData->GetUIManager()->GetUIQuest()->MsgProc(hWnd, uMsg, wParam, lParam );
	}
#endif SERV_EPIC_QUEST
	if ( CX2State::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
	switch ( uMsg )
	{	
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		} break;
	}
#endif //DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL

	if( NULL != g_pX2Game )
		return g_pX2Game->MsgProc( hWnd, uMsg, wParam, lParam );

	return false;
}


bool CX2StateDungeonGame::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case DGUCM_GAME_LOADING:
		{

			m_pDungeonGame->GameLoading( g_pData->GetDungeonRoom() );
			g_pKTDXApp->SkipFrame();

			//LoadingCompleteReq();
		}
		break;

	case DGUCM_GAME_START:
		{
			m_pDungeonGame->GameStart();
			g_pKTDXApp->SkipFrame();

			return true;
		}
		break;

	case DGUCM_GAME_EXIT_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGExitMsgBox, NULL, false );
			m_pDLGExitMsgBox = NULL;

			if( true == g_pMain->GetIsPlayingTutorial() )
			{
				g_pMain->SetIsPlayingTutorial( false );
				m_bLeaveRoomAtTutorial = true;

				g_pMain->SetIsExitingTutorial( true );
				if ( m_bSend_KEGS_LEAVE_ROOM_REQ == false )
					m_pDungeonGame->Handler_EGS_LEAVE_ROOM_REQ();

				m_bSend_KEGS_LEAVE_ROOM_REQ = true;
			}
			else
			{
				if ( m_bSend_KEGS_LEAVE_ROOM_REQ == false )
					m_pDungeonGame->Handler_EGS_LEAVE_ROOM_REQ();

				//{{ kimhc // 실시간 엘소드 중 실시간 ED 획득 관련
#ifdef	REAL_TIME_ELSWORD
				// 던전 이탈시 ED의 패널티가 적용된 메뉴 효과를 위한 용도
				if ( g_pInstanceData != NULL )
					g_pInstanceData->SetInstanceED( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED );
#endif	REAL_TIME_ELSWORD
				//}} kimhc // 실시간 엘소드 중 실시간 ED 획득 관련


				m_bSend_KEGS_LEAVE_ROOM_REQ = true;
			}

			return true;

		}
		break;

	case DGUCM_GAME_EXIT_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGExitMsgBox, NULL, false );
			m_pDLGExitMsgBox = NULL;

			return true;
		}
		break;


	case DGUCM_GOTO_FIRST_DUNGEON_OK:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGoToFirstDungeon, NULL, false );
			m_pDLGGoToFirstDungeon = NULL;


			CX2StateAutoChanger::TARGET_DETAIL targetDetail;
			targetDetail.m_iChannelID = (int) 104;		// note!!! 채널번호 일단 하드코딩
			targetDetail.m_iDungeonID = (int) SEnum::DI_EL_FOREST_GATE_NORMAL;
			targetDetail.m_iRoomUID = -1;

			g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_DUNGEON_GAME, targetDetail, 
				g_pData->GetMyUser()->GetSelectUnit()->GetUID() );

			return true;
		} break;

	case DGUCM_GOTO_FIRST_DUNGEON_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGGoToFirstDungeon, NULL, false );
			m_pDLGGoToFirstDungeon = NULL;

			g_pMain->SetIsExitingTutorial( true );
			m_pDungeonGame->Handler_EGS_LEAVE_ROOM_REQ();

			return true;
		} break;


	case DGUCM_TOTORIAL_EXIT:
		{
			QuitGame();
			return true;
		} break;


#ifndef _SERVICE_
		case DGUCM_SUMMON_SELECTED_MONSTER_DEBUG:
			{
				CKTDGUIListBox* pListBox = (CKTDGUIListBox*) lParam;
				if( NULL == pListBox )
					return false;

				CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBox->GetSelectedItem();
				if( NULL == pListBoxItem )
					return false; 



#	ifdef CREATE_MONSTER_LIST_TEST
				if( pListBox->GetDialog() == m_CreateMonsterListDialog.GetDialog() )
				{
					m_CreateMonsterListDialog.OnCommand( pListBoxItem->strText ); 
				}
#	endif CREATE_MONSTER_LIST_TEST
				
#ifdef MONSTER_STATE_LIST_TEST
				if( pListBox->GetDialog() == m_MonsterStateListDialog.GetDialog() )
				{
					m_MonsterStateListDialog.OnCommand( *((const std::string*)(pListBoxItem->pData)) );
				}
#endif MONSTER_STATE_LIST_TEST

				return true;
			} break;


		case DGUCM_SORT_MONSTER_LIST_DEBUG:
			{
				CKTDGUIControl* pControl = (CKTDGUIControl*) lParam;
				if( NULL == pControl )
					return false;

#	ifdef CREATE_MONSTER_LIST_TEST
				if( pControl->GetDialog() == m_CreateMonsterListDialog.GetDialog() )
				{
					m_CreateMonsterListDialog.OnChangeSorting();
				}
#	endif CREATE_MONSTER_LIST_TEST

				return true;
			} break;
#endif _SERVICE_


#ifdef HENIR_TEST

#ifdef NEW_HENIR_TEST

#else
	case DGUCM_WANT_TO_ENTER_SECRET_STAGE_OK:
		{
			Send_EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT( true );

			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WANT_TO_ENTER_SECRET_STATE, userData );


			return true;
		} break;


	case DGUCM_WANT_TO_ENTER_SECRET_STAGE_CANCEL:
		{
			Send_EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT( false );

			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WANT_TO_ENTER_SECRET_STATE, userData );

			return true;
		} break;
#endif NEW_HENIR_TEST
#endif HENIR_TEST


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM 
		case DGUCM_CLOSE_BAD_ATTITUDE_WARNING_DIALOG :		// 강퇴 경고 DLG 의 X 버튼
		{
			if ( NULL != m_pDungeonGame )
			{

#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG 
				ReservingWarningDlgDelete();	// 해당 강퇴 경고 DLG 삭제 예약
#else // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG	
				if ( true == m_BadAttitudeUserWarningDialog.IsOpen() )
					m_BadAttitudeUserWarningDialog.Close();
#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

				return true;
			}
		}
		break;

#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // 적용날짜: 2013-04-22
		case DGUCM_AGREE_ENTER_GATE_OF_DARKNESS:		/// 어둠의 문 입장 동의
			{
				Sned_Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT( true );
			} break;

		case DGUCM_NOT_AGREE_ENTER_GATE_OF_DARKNESS:	/// 어둠의 문 입장 거부
			{
				Sned_Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT( false );
			} break;
#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

	}

	if ( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef SERV_EPIC_QUEST
	if( g_pData->GetUIManager()->GetUIQuestNew() != NULL && 
		g_pData->GetUIManager()->GetUIQuestNew()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true )
		return true;
#else
	if( g_pData->GetUIManager()->GetUIQuest() != NULL && 
		g_pData->GetUIManager()->GetUIQuest()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true )
		return true;
#endif SERV_EPIC_QUEST


#ifdef KEYFRAME_CAMERA
	if(g_pX2Game != NULL &&
		g_pX2Game->GetX2Camera() != NULL &&
		g_pX2Game->GetWorldCameraEdit() == true)
	{
		if(g_pX2Game->GetX2Camera()->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true )
			return true;
	}
#endif KEYFRAME_CAMERA



	return false;
}

bool CX2StateDungeonGame::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

	if ( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;		

	switch( wParam )
	{

	case EGS_REMAINING_PLAY_TIME_NOT:
		return Handler_EGS_REMAINING_PLAY_TIME_NOT( hWnd, uMsg, wParam, lParam );
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

	case EGS_PLAY_START_NOT:
		return PlayStartNot();
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

	case EGS_END_GAME_ACK:
		return Handler_EGS_END_GAME_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_RESULT_ACK:
		return StateChangeResultAck( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_STATE_CHANGE_RESULT_NOT:
		return StateChangeResultNot( hWnd, uMsg, wParam, lParam );
		break;

#if 0 
	case EGS_STATE_CHANGE_LOCAL_MAP_ACK:
		return StateChangeLocalMapAck( hWnd, uMsg, wParam, lParam );
		break;
#endif

	case EGS_DROP_ITEM_NOT:
		return Handler_EGS_DROP_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GET_ITEM_ACK:
		return Handler_EGS_GET_ITEM_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_GET_ITEM_NOT:
		return Handler_EGS_GET_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK :
		return Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK( hWnd, uMsg, wParam, lParam );
		break;

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	case EGS_BAD_ATTITUDE_USER_NOT:		// 불량 유저에 대한 정보를 담은 패킷, 해당 유저 PartyUI 에 강퇴 버튼을 활성화 시킨다.
		if ( NULL != m_pDungeonGame )
		{
			return m_pDungeonGame->Handler_EGS_BAD_ATTITUDE_USER_NOT( hWnd, uMsg, wParam, lParam );	
		}
		break;

	case EGS_FORCED_EXIT_VOTE_ACK :
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();	// GageManager Instance 를 얻어옴
			if ( NULL != pGageManager )
			{
				return pGageManager->Handler_EGS_FORCED_EXIT_VOTE_ACK( hWnd, uMsg, wParam, lParam );
			}
		}
		break;

	case EGS_BAD_ATTITUDE_USER_MSG_NOT :
		return Handler_EGS_BAD_ATTITUDE_USER_MSG_NOT( hWnd, uMsg, wParam, lParam );
		break;

#endif // #define SERV_DUNGEON_FORCED_EXIT_SYSTEM

	case EGS_END_GAME_DUNGEON_RESULT_DATA_NOT:
		return Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( hWnd, uMsg, wParam, lParam );
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



	case EGS_DUNGEON_STAGE_LOAD_ACK:
		return DungeonStageLoadAck( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_STAGE_LOAD_NOT:
		return DungeonStageLoadNot( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK:
		return DungeonStageLoadCompleteAck( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT:
		return DungeonStageLoadCompleteNot( hWnd, uMsg, wParam, lParam );
		break; 

	case EGS_DUNGEON_STAGE_START_NOT:
		return DungeonStageStartNot( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_SUB_STAGE_OPEN_ACK:
		return Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_SUB_STAGE_OPEN_NOT:
		return Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK:
		return Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_SUB_STAGE_START_NOT:
		return Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK:
		return Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT:
		return Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( hWnd, uMsg, wParam, lParam );
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

	case EGS_CHANGE_PITIN_NOT:
		{
			return Handler_EGS_CHANGE_PITIN_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_PLAY_TIME_OUT_NOT:
		{
			return Handler_EGS_PLAY_TIME_OUT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
	case EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT:
		{
			return Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_DUNGEON_EFFECT_TIME_OUT_NOT:
		{
			return Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif DUNGEON_TIME_OUT_FRAMEMOVE

		//{{ kimhc //	실시간 엘소드 중 실시간 레벨업, 실시간 장비 및 아바타 교체,
		//				실시간 아이템 획득, 실시간 내구도 감소 이벤트 핸들링
#ifdef REAL_TIME_ELSWORD 
	case EGS_CHAR_LEVEL_UP_NOT:
		{
			return Handler_EGS_CHAR_LEVEL_UP_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT:
		{
			return Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_GET_ITEM_REALTIME_NOT:
		{
			return Handler_EGS_GET_ITEM_REALTIME_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_DECREASE_ENDURANCE_NOT:
		{
			return Handler_EGS_DECREASE_ENDURANCE_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT:
		{
			return Hander_EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_DECREASE_SPIRIT_NOT:
		{
			return Handler_EGS_DECREASE_SPIRIT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif REAL_TIME_ELSWORD
		//}} kimhc //	실시간 엘소드 중 실시간 레벨업, 실시간 장비 및 아바타 교체,
		//				실시간 아이템 획득, 실시간 내구도 감소 이벤트 핸들링





#ifdef HENIR_TEST


	case EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK:
		{
			return Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK( hWnd, uMsg, wParam, lParam );
		} break;


	case EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT:
		{
			return Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT( hWnd, uMsg, wParam, lParam );
		} break;


	case EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT:
		{
			return Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#endif HENIR_TEST

	case EGS_DUNGEON_KILLALLNPC_CHECK_ACK:
		{
			return Handler_EGS_DUNGEON_KILLALLNPC_CHECK_ACK( hWnd, uMsg, wParam, lParam );
		} break;

		//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	case EGS_USER_GAME_STAT_NOT:
		{
			return Handler_EGS_USER_GAME_STAT_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

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

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
	case EGS_ADMIN_NPC_UNIT_CREATE_ACK:
		{
			if( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_ADMIN_NPC_UNIT_CREATE_ACK( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;
	case EGS_ADMIN_NPC_UNIT_CREATE_NOT:
		{
			if( NULL != g_pX2Game )
			{
				return g_pX2Game->Handler_EGS_ADMIN_NPC_UNIT_CREATE_NOT( hWnd, uMsg, wParam, lParam );
			}
			return true;
		} break;
#endif SERV_CREATE_ELITE_FOR_ADMIN

	case EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT:
		return Handler_EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT:
		return Handler_EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT( hWnd, uMsg, wParam, lParam );
		break;
#ifdef SERV_EVENT_VALENTINE_DUNGEON 
	case EGS_SYNC_DUNGEON_TIMER_NOT:
		return Handler_EGS_SYNC_DUNGEON_TIMER_NOT( hWnd, uMsg, wParam, lParam );
		break;
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_NEW_DEFENCE_DUNGEON
	case EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT:			/// 보스 몬스터 사망 직후, 어둠의 문 입장신을 위하여 보내지는 패킷에 대한 처리
		return Handler_EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_UPDATE_INVENTORY_SLOT_INFO_NOT:				/// 몬스터 카드 목록( 인벤토리 ) 갱신을 위한 패킷 처리
		return Handler_EGS_UPDATE_INVENTORY_SLOT_INFO_NOT( hWnd, uMsg, wParam, lParam );
		break;

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // 적용날짜: 2013-04-23
	case EGS_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT:	///  어둠의 문 입장 거부로 인한 던전 퇴장 패킷 처리
		return Handler_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT( hWnd, uMsg, wParam, lParam );
		break;

	case EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT:				/// 어둠의 문 입장 여부 팝업 생성 패킷 처리
		return Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT( hWnd, uMsg, wParam, lParam );
		break;
#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES // 오현빈
	case EGS_DUNGEON_KILLALLNPC_CHECK_NOT:
		{
			return Handler_EGS_DUNGEON_KILLALLNPC_CHECK_NOT();
		} break;
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	case EGS_DUNGEON_SUB_STAGE_CLEAR_ACK:
		return Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( hWnd, uMsg, wParam, lParam );
		break;
#endif // SERV_STAGE_CLEAR_IN_SERVER
	}

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif 
	return false;
}


bool CX2StateDungeonGame::UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	m_pDungeonGame->MsgProc( hWnd, uMsg, wParam, lParam );

	return false;
}



bool CX2StateDungeonGame::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );


	if( m_pDungeonGame->Handler_EGS_LEAVE_ROOM_ACK( kEvent ) == true )
	{
		m_bSend_KEGS_LEAVE_ROOM_REQ = false;
		m_bReceive_KEGS_LEAVE_ROOM_ACK = true;
	}

	return true;
}

bool CX2StateDungeonGame::Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if ( g_pData->GetDungeonRoom() != NULL )
	{
		CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
		if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			StateLog( L"leave while playing dungeon game : " );
			StateLog( pSlotData->m_pUnit->GetNickName() );
		}
	}

	return m_pDungeonGame->Handler_EGS_LEAVE_ROOM_NOT( kEvent );
}

bool CX2StateDungeonGame::Handler_EGS_GAME_LOADING_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//StateChangeLocalMapReq();
	StateChangeFieldReq();

	return true;
}





bool CX2StateDungeonGame::GameLoadingReq( int percent )
{
	KEGS_GAME_LOADING_REQ kPacket;

	kPacket.m_iLoadingProgress = percent;

	g_pData->GetServerProtocol()->SendPacket( EGS_GAME_LOADING_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GAME_LOADING_ACK, 60.f );

	return true;
}

bool CX2StateDungeonGame::GameLoadingAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_GAME_LOADING_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{
			return true;
		}
	}
	return false;
}

bool CX2StateDungeonGame::GameLoadingNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GAME_LOADING_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	//CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotDataByUnitUID( kPacket.m_iUnitUID );
	//WCHAR tempStr[50] = {0,};
	//wsprintf( tempStr, L"%s : 로딩중..  %d 퍼센트", pSlotData->m_pUnit->GetNickName(), kPacket.m_iLoadingProgress );
	

	//CKTDGUIStatic* pStaticLoadingState = (CKTDGUIStatic*)m_pDLGLoadingState->GetControl( L"StaticPVPGameLoadingState" );
	//pStaticLoadingState->GetString(pSlotData->m_Index)->msg = tempStr;
	
	// note!! 신캐릭터 추가되면 추가해야함
	/*enum LOADING_USER_INFO
	{
		LUI_ELSWORD_COLOR = 0,
		LUI_ARME_COLOR,
		LUI_LIRE_COLOR,
		LUI_RAVEN_COLOR,
		LUI_EVE_COLOR,
		LUI_CHUNG_COLOR,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_COLOR,
#endif

		LUI_ELSWORD_BLACK,
		LUI_ARME_BLACK,
		LUI_LIRE_BLACK,
		LUI_RAVEN_BLACK,
		LUI_EVE_BLACK,
		LUI_CHUNG_BLACK,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_BLACK,
#endif

		LUI_EMPTY,
	};*/
#ifdef REFORM_ENTRY_POINT
	DUNGEON_LOADING_DATA sDungeonLoadingData;
	
	if( m_vecDungeonLoadingData.empty() )
	{
		sDungeonLoadingData.uidUnitUID = kPacket.m_iUnitUID;
		sDungeonLoadingData.iLoadingPercent = kPacket.m_iLoadingProgress;

		m_vecDungeonLoadingData.push_back(sDungeonLoadingData);
	}
	else
	{

		bool IsHaveUID = false;
		BOOST_FOREACH( DUNGEON_LOADING_DATA& LoadingData, m_vecDungeonLoadingData )
		{	
			if( LoadingData.uidUnitUID == kPacket.m_iUnitUID )
			{
				IsHaveUID = true;
				LoadingData.uidUnitUID = kPacket.m_iUnitUID;
				LoadingData.iLoadingPercent = kPacket.m_iLoadingProgress;
			}
		}

		if( IsHaveUID == false )
		{
			sDungeonLoadingData.uidUnitUID = kPacket.m_iUnitUID;
			sDungeonLoadingData.iLoadingPercent = kPacket.m_iLoadingProgress;

			m_vecDungeonLoadingData.push_back(sDungeonLoadingData);
		}
	}
	if( g_pData->GetDungeonRoom()->GetUserNum() == static_cast<int>(m_vecDungeonLoadingData.size()) )
	{
		int iLoadingPercent1 = 100;
		BOOST_FOREACH( DUNGEON_LOADING_DATA LoadingData, m_vecDungeonLoadingData )
		{	
			if( LoadingData.iLoadingPercent < iLoadingPercent1 )
				iLoadingPercent1 = LoadingData.iLoadingPercent;
		}

		SetLoadingGageBar(iLoadingPercent1);
	}

#else //REFORM_ENTRY_POINT

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-22
	/*
		어둠의 문 입장시, 이전 던전( 일반 던전 )에 입장한 생태에서 이전 던전을 지우고 새로 생성한다.
		이 과정에서, 일부 네트워크 환경이 좋지 않은 유저는 아직 이전 던전에 있는 상태에서 EGS_GAME_LOADING_NOT을 받게 된다.
		m_DLGLoadingStateUnitInfoList가 비어있는 상태에서, 해당 패킷을 통해 정보를 갱신하려다 보니 크래시가 발생했다.
		( m_DLGLoadingStateUnitInfoList은 던전이 생성될 때 삽입되고 게임 시작시 지워지므로, 해당 시점에서는 비어있는 상태이다. )
		이 부분에 대하여, m_DLGLoadingStateUnitInfoList가 비어있는 상태라면,로딩 진행 사항을 처리하지 않도록 한다.
		( 해당 패킷의 내용을 처리하지 않아도 던전 로딩에는 영향이 없다. )
	*/
	if ( m_DLGLoadingStateUnitInfoList.empty() )
		return false;
#endif // SERV_NEW_DEFENCE_DUNGEON
	

	for ( int i = 0; i < g_pData->GetDungeonRoom()->GetSlotNum(); i++ )
	{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-22
		if ( i < static_cast<int>( m_DLGLoadingStateUnitInfoList.size() ) )
#endif // SERV_NEW_DEFENCE_DUNGEON
		{
			CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];

			CKTDGUIStatic* pStaticUnitInfoTex = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoadingUnitInfoTex" );

			CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetSlotData( i );

			if ( pSlotData->m_pUnit == NULL )
			{
				continue;
			}

			if ( pSlotData->m_pUnit->GetUID() != kPacket.m_iUnitUID )
			{
				continue;
			}

			if ( kPacket.m_iLoadingProgress >= 100 )
			{
				switch( pSlotData->m_pUnit->GetType() )
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
						ASSERT( !"Unexpected UnitClass" );
					} break;

				}
			}


			WCHAR buff[256] = {0};
			CKTDGUIStatic* pStaticUnitInfoPercent = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_Percent" );
			StringCchPrintf( buff, 256, L"%d%%", kPacket.m_iLoadingProgress );
			//wsprintf( buff, L"%d%%", kPacket.m_iLoadingProgress );
			pStaticUnitInfoPercent->GetString( 0 )->msg = buff;
			CKTDGUIStatic* pStaticUnitInfo = (CKTDGUIStatic*)pDialog->GetControl( L"StaticLoading_UserInfo" );

			StringCchPrintf( buff, 256, L"LV.%d %s", (int)pSlotData->m_pUnit->GetUnitData().m_Level, pSlotData->m_pUnit->GetNickName() );
			//wsprintf( buff, L"LV.%d %s", (int)pSlotData->m_pUnit->GetUnitData().m_Level, pSlotData->m_pUnit->GetNickName() );
			pStaticUnitInfo->GetString( 0 )->msg = buff;

			break;
		}
	}	
#endif //REFORM_ENTRY_POINT

	return true;
}

bool CX2StateDungeonGame::GameLoadingAllUnitOkNot()
{
	g_pMain->GetMemoryHolder()->ClearEquip();

	return true;
}

bool CX2StateDungeonGame::PlayStartNot()
{
	//SAFE_DELETE( m_pDLGLoadingState );
		
#ifdef MACHINE_ID
	ELSWORD_VIRTUALIZER_START
	Handler_EGS_CHECK_MACHINE_ID_REQ();
	ELSWORD_VIRTUALIZER_END
#endif
	
#ifdef REFORM_ENTRY_POINT
	SAFE_DELETE_DIALOG( m_pDLGLoadingFront );
#else //REFORM_ENTRY_POINT
	for ( int i = 0; i < (int)m_DLGLoadingStateUnitInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_DLGLoadingStateUnitInfoList[i];
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
        SAFE_DELETE_DIALOG_HANDLE( pDialog );
#else
		SAFE_DELETE( pDialog );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
	}
	m_DLGLoadingStateUnitInfoList.clear();
#endif //REFORM_ENTRY_POINT

	m_pDungeonGame->GameStart();
	g_pKTDXApp->SkipFrame();


	if( m_pDungeonGame->IsHost() == true )
	{
		DungeonStageLoadReq( 0 );
	}

	return true;
}




bool CX2StateDungeonGame::EndDungeonGameReq( CX2Room::TEAM_NUM winTeam  )
{
	if ( m_bSend_KEGS_END_GAME_REQ == true )
		return true;

	KEGS_END_GAME_REQ kEvent;
	kEvent.m_iWinTeam = winTeam;

	g_pData->GetServerProtocol()->SendPacket( EGS_END_GAME_REQ, kEvent );
	g_pMain->AddServerPacket( EGS_END_GAME_ACK, 30.0f );

	m_bSend_KEGS_END_GAME_REQ = true;

	return true;
}

bool CX2StateDungeonGame::Handler_EGS_END_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_END_GAME_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

/*virtual*/ bool CX2StateDungeonGame::Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_GAME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( true == g_pMain->GetIsPlayingTutorial() )
	{
	}				
	//	g_pX2Room->Set_KRoomSlotInfoList( kEvent.m_vecSlot, false );
	m_pDungeonGame->Handler_EGS_END_GAME_NOT( kEvent );

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();
	if ( NULL != pGageManager )
	{
// 게임이 끝났을 때, 모든 파티 멤버들의 투표 타입을 None 상태로 돌린다.
		pGageManager->SetAllPartyMemberVoteButtonType( CX2PartyMemberGageUI::BVBT_NONE );
		pGageManager->SetMyUserIsFaultyPlayer( false );
	}
	
	
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-09
	if ( false == GetEnterDefenceDungeon() )		/// 어둠의 문 입장할 예정이 없으면, 승리 혹은 패배 처리
#endif // SERV_NEW_DEFENCE_DUNGEON
	{

		CX2GameUnit* pMyUserUnit = m_pDungeonGame->GetMyUnit();
		if ( pMyUserUnit != NULL )
		{
			if( pMyUserUnit->GetTeam() == kEvent.m_iWinTeam )
			{
				pMyUserUnit->Win();

				GetDungeonEndingEvent()->m_fElapsedTime_WinEffect = 0.f;

				if( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON ) // result 화면에서 win or lose를 표시하기 위해서
				{			
					CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
					if( NULL != pDungeonGame )
						pDungeonGame->SetWindTeam( (CX2Room::TEAM_NUM)pMyUserUnit->GetTeam() );
				}

				// 이겼을 때는 남아있는 npc들의 AI를 끈다. 졌을 때는 남은 NPC의 AI를 그냥 켜둔다.
				if( NULL != g_pX2Game )
				{
					for( int i=0; i<g_pX2Game->GetNPCUnitListSize(); i++ )
					{
						CX2GUNPC* pGUNPC = g_pX2Game->GetNPCUnit(i);
						if( NULL != pGUNPC )
						{
							pGUNPC->SetAIEnable( false );
						}
					}
				}
			}
			else
			{
				pMyUserUnit->Lose();

				if( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON )
				{			
					CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
					if( NULL != pDungeonGame )
						pDungeonGame->SetWindTeam( CX2Room::TN_MONSTER );
				}
			}

		}

	}

	//m_bGameEnd = true;

	/// 결과 처리 정보를 서버로 보냄
	m_pDungeonGame->Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ();

	return true;
}

bool CX2StateDungeonGame::StateChangeFieldReq()
{
	ReturnToPlaceWhereBeforeDungeonStart();
	//Handler_EGS_STATE_CHANGE_FIELD_REQ();
	return true;
}


bool CX2StateDungeonGame::StateChangeLocalMapReq()
{
#if 0 
	KStateChangeReq kPacket;
	kPacket.m_nMapID = g_pMain->GetPrevLocalStateID();	
	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_LOCAL_MAP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_LOCAL_MAP_ACK );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_565 ), this );
#endif	
	return true;
}

bool CX2StateDungeonGame::StateChangeLocalMapAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#if 0 
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KStateChangeAck kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_LOCAL_MAP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_566 ) );
			g_pData->DeleteDungeonRoom(); 
			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_LOCAL_MAP, kEvent.m_nMapID, false" );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_LOCAL_MAP, kEvent.m_nMapID, false );
		}
	}
#endif
	return true;
}

bool CX2StateDungeonGame::StateChangeResultReq()
{
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
	if ( true == GetEnterDefenceDungeon() )		/// 어둠의 문 입장 예정이라면, 처리하지 않는다.
		return true;
#endif // SERV_NEW_DEFENCE_DUNGEON

	KEGS_STATE_CHANGE_RESULT_REQ kEGS_STATE_CHANGE_RESULT_REQ;

	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_RESULT_REQ, kEGS_STATE_CHANGE_RESULT_REQ );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_RESULT_ACK );


	return true;
}

bool CX2StateDungeonGame::StateChangeResultAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_RESULT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2StateDungeonGame::StateChangeResultNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_STATE_CHANGE_RESULT_NOT kEvent;
	//std::vector< KRoomSlotInfo > kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2PacketLog::PrintLog( &kEvent );

	m_pDungeonGame->Handler_EGS_STATE_CHANGE_RESULT_NOT( kEvent );


	return true;
}

void CX2StateDungeonGame::OpenExitDLG( const WCHAR* wszMsg )
{
	m_pDLGExitMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wszMsg, DGUCM_GAME_EXIT_OK, this, DGUCM_GAME_EXIT_CANCEL );
	
	if( m_pDLGExitMsgBox != NULL )
	{
		CKTDGUIButton* pCancelButton = (CKTDGUIButton*)m_pDLGExitMsgBox->GetControl( L"MsgBoxOkAndCancelCancelButton" );		
		pCancelButton->RequestFocus();
	}
}

bool CX2StateDungeonGame::DungeonStageLoadReq( int stageNum )
{
	KEGS_DUNGEON_STAGE_LOAD_REQ kPacket;
	kPacket.m_iStageID		= stageNum;

	ASSERT( stageNum >= 0 );
	if( stageNum < 0 )
	{
		StateLog( L"stage load req < 0" );
		return false;
	}

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG

	if ( NULL != m_pDungeonGame
		 && NULL != m_pDungeonGame->GetDungeon()
		 && NULL != m_pDungeonGame->GetDungeon()->GetNowStage() )
	{
		CX2DungeonSubStage* pDungeonSubStage
			= m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage();
		if ( NULL != pDungeonSubStage )
		{
			kPacket.m_cNormalStage = static_cast<char>( pDungeonSubStage->GetNextNormalStageIndex() );
			kPacket.m_cSecretStage = static_cast<char>( pDungeonSubStage->GetNextSecretStageIndex() );
			kPacket.m_cNormalStage = static_cast<char>( pDungeonSubStage->GetNextEliteStageIndex() );
			kPacket.m_cClearConditionCheckCount = pDungeonSubStage->GetClearConditionCheckCount();
		}		
	}
#endif // SERV_DUNGEON_STAGE_LOAD_LOG


	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_STAGE_LOAD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_DUNGEON_STAGE_LOAD_ACK, 60.f ); 


	return true;
}

bool CX2StateDungeonGame::DungeonStageLoadAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_STAGE_LOAD_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( m_pDLGF1Help != NULL )
			{
				m_pDLGF1Help->SetShowEnable( false, false );
			}

			if ( m_pDLGF1Help2 != NULL )
			{
				m_pDLGF1Help2->SetShowEnable( false, false );
			}

			return true;
		}
	}

	return false;
}



bool CX2StateDungeonGame::DungeonStageLoadNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_STAGE_LOAD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	//{{ hoons.09-07-15. 스테이지 로딩을 시작하면 이브 캐릭터 들의 메가 일랙트론볼 차지를 리셋해준다.
	if( m_pDungeonGame != NULL &&
		m_pDungeonGame->GetMyUnit() != NULL &&
		m_pDungeonGame->GetMyUnit()->GetUnit() != NULL )
	{
		CX2GUUser* pX2GUUser = m_pDungeonGame->GetMyUnit();
		if( pX2GUUser->GetUnit()->GetType() == CX2Unit::UT_EVE )
		{
			CX2GUEve* pX2GUEve = (CX2GUEve*)pX2GUUser;
			pX2GUEve->ResetMegaEletronCharge();
#ifdef SERV_EVE_BATTLE_SERAPH
			pX2GUEve->ResetTaserPilumCharge();
#endif
		}

		// 지헌 : 던전 사용 아이템 버프를 풀어준다.
#ifdef SPECIAL_USE_ITEM
		for( int i=0; i<m_pDungeonGame->GetUserUnitListSize(); i++ )
		{
			CX2GUUser* pUser = m_pDungeonGame->GetUserUnit(i);
			if( pUser == NULL )
				continue;

			pUser->StopSpecialItemBuffWind();
		}
#endif
	}
	//}}



	//{{ kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08
	if ( m_pDungeonGame != NULL &&
		m_pDungeonGame->GetCanUseEscFlag() == true )
	{
		m_pDungeonGame->SetCanUseEscFlag( false );
	}
	//}} kimhc // 게임 로딩이 끝날때 까지 ESC 처리 안되게 하는 작업 // 2009-06-08

	if( NULL != g_pX2Room )
	{
		g_pX2Room->SetNumOfPlayers( kEvent.m_iNumMember );
	}





	m_pDungeonGame->SetHideDungeonPlayTimeAfterLoading( true );
	m_pDungeonGame->GetDungeon()->SetStageStaticNPC( kEvent.m_iStageID, kEvent.m_mapNPCData );

	if ( kEvent.m_iStageID < 0 || kEvent.m_iStageID >= (int)m_pDungeonGame->GetDungeon()->GetStageDataList().size() )
	{
		// nothing; 
	}
	else
	{
		m_pDungeonGame->GetDungeon()->GetStageDataList()[kEvent.m_iStageID]->m_mapAttribEnchantNpcInfo = kEvent.m_mapAttribNpcInfo;
	}


	m_NextStageNum	= kEvent.m_iStageID;
	m_RenderCount	= g_pKTDXApp->GetRenderCount();

	if( kEvent.m_iStageID != 0 )
	{
		g_pKTDXApp->GetDGManager()->StartFadeOut( 1.0f );
	}
#if defined(ADD_COLLECT_CLIENT_INFO) && defined(CHECK_ALL_WINDOW)
	if( g_pInstanceData != NULL && g_pMain != NULL )
	{
		HWND hWnd = ::GetTopWindow(NULL);
		while(hWnd)
		{			
			wstring strWndClass = L"WTWindow";			
			WCHAR wTextClass[_MAX_PATH] = L"";			
			int hr2 = GetClassName(hWnd, wTextClass, _MAX_PATH);
				
			if( hr2 > 0)
			{
				if( strWndClass.compare( wTextClass ) == 0 )
				{
					string strClassName = "";
					ConvertWCHARToChar( strClassName, wTextClass );
					g_pMain->SendHackInfo5(ANTIHACKING_ID::ANTIHACKING_FILGER_34, strClassName.c_str(), false, true );
				}
			}

			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
		}
	}	
#endif

	g_pKTDXApp->SkipFrame();

#ifdef NEW_HENIR_TEST
	InitStartSecretStageEnteringEvent();
#endif NEW_HENIR_TEST
	return true;
}

bool CX2StateDungeonGame::DungeonStageLoadCompleteReq( bool bIsSecretStage )
{
	KEGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ kPacket;
	kPacket.m_bIsSecretStage = bIsSecretStage;

	//{{ kimhc // 2011-06-13 // 통계 로그
#ifdef SERV_DUNGEON_STAGE_LOG
	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
		kPacket.m_iDamage = g_pX2Game->GetMyUnit()->GetTotalAttackedDamage();
	else
	{
		ASSERT( !L"g_pX2Game or g_pX2Game->GetMyUnit() is NULL" );
		kPacket.m_iDamage = 0;
	}
#endif SERV_DUNGEON_STAGE_LOG
	//}} kimhc // 2011-06-13 // 통계 로그

	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, 60.f );

	return true;
}

bool CX2StateDungeonGame::DungeonStageLoadCompleteAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}


bool CX2StateDungeonGame::DungeonStageLoadCompleteNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

    if ( g_pData->GetGameUDP() != NULL )
    {
        g_pData->GetGameUDP()->ResetConnectTestToPeer( kEvent.m_iUnitUID );
    }

#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


	return true;
}


bool CX2StateDungeonGame::DungeonStageStartNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_STAGE_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	m_pDungeonGame->StageStart();

	if ( m_pDLGArcadeMission != NULL )
		m_pDLGArcadeMission->SetShowEnable( false, false );

	//{{ 허상형 : [2009/8/20] //	신학기 이벤트 던전 점수 표시
#ifdef NEW_TERM_EVENT
	//	헤니르 시공

	if( true == kEvent.m_bIsHenirDungeon )
	{
		if( kEvent.m_iHenirEventPoint != 0 && kEvent.m_iStageID != 0 )	//	게임 시작 시 점수 보이지 않게 하기
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_REPLACED_STRING( ( STR_ID_4404, "i", kEvent.m_iHenirEventPoint ) ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
		}
	}
	//	일반 던전
	else
	{
		if( kEvent.m_iDungeonEventPoint != 0  && kEvent.m_iStageID != 0 )	//	게임 시작 시 점수 보이지 않게 하기

		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_REPLACED_STRING( ( STR_ID_4403, "i", kEvent.m_iDungeonEventPoint ) ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
		}
	}
    
#endif	//	NEW_TERM_EVENT
	//}} 허상형 : [2009/8/20] //	신학기 이벤트 던전 점수 표시

	return true;
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SUB_STAGE_OPEN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	return m_pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( kEvent );
}


bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SUB_STAGE_OPEN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	m_pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( kEvent );
	return true;
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	return m_pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( kEvent );
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	return m_pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( kEvent );
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	return m_pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( kEvent );
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SUB_STAGE_START_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if( 0 == kEvent.m_iSubStageID )
	{
		m_pDungeonGame->SetHideDungeonPlayTimeAfterLoading( false );
	}

	return m_pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( kEvent );
}

bool CX2StateDungeonGame::Handler_EGS_REMAINING_PLAY_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KEGS_REMAINING_PLAY_TIME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	//CX2PacketLog::PrintLog( &kEvent );

	m_pDungeonGame->Handler_EGS_REMAINING_PLAY_TIME_NOT( kEvent );
#else//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	float kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	m_pDungeonGame->Handler_EGS_REMAINING_PLAY_TIME_NOT( kEvent );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	return true;
}

bool CX2StateDungeonGame::Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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

bool CX2StateDungeonGame::Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT kPacket;
	DeSerialize( pBuff, &kPacket );
	//CX2PacketLog::PrintLog( &kPacket );

	//if( g_pMain->DeleteServerPacket( EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK ) == true )
	//{
	//	if ( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
	//	{			
	//	}
	//}

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
	if ( true == GetEnterDefenceDungeon() )		/// 어둠의 문 입장 예정이라면, 처리하지 않는다.
		return false;
#endif // SERV_NEW_DEFENCE_DUNGEON
	
	m_pDungeonGame->Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( kPacket );

	bool bDieMyUnit = true;

	for ( int i = 0; i < (int)kPacket.m_vecDungeonUnitInfo.size(); i++ )
	{
		KDungeonUnitResultInfo& kDungeonUnitResultInfo = kPacket.m_vecDungeonUnitInfo[i];
		if ( kDungeonUnitResultInfo.m_UnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
		{
			bDieMyUnit = kDungeonUnitResultInfo.m_bIsDie;
			break;
		}
	}

	if( kPacket.m_bIsWin == true && bDieMyUnit == false )
	{
		int realDungeonID = kPacket.m_iDungeonID + (int)kPacket.m_cDifficulty;
		CX2Unit* pMyUnit = g_pX2Game->GetMyUnit()->GetUnit();
		if( NULL != pMyUnit )
		{
			//현재 던전을 깼으면 깬 던전 ID룰 받아오자...			
			if( false == pMyUnit->IsClearDungeon( realDungeonID ) )
				pMyUnit->AddClearDungeon( realDungeonID );
		}
	}

	//m_bGameEnd = true;

	//{{ 허상형 : [2009/8/20] //	신학기 이벤트 던전 점수 표시
#ifdef NEW_TERM_EVENT
	//	헤니르 시공

	if( true == kPacket.m_bIsWin )
	{
		if( true == kPacket.m_bIsHenirDungeon )
		{
			if( kPacket.m_iHenirEventPoint != 0 )
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_REPLACED_STRING( ( STR_ID_4404, "i", kPacket.m_iHenirEventPoint ) ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
			}
		}
		//	일반 던전
		else
		{
			if( kPacket.m_iDungeonEventPoint != 0 )

			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_REPLACED_STRING( ( STR_ID_4403, "i", kPacket.m_iDungeonEventPoint ) ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 5.f );
			}
		}
	}

#endif	//	NEW_TERM_EVENT
	//}} 허상형 : [2009/8/20] //	신학기 이벤트 던전 점수 표시


	return true;
}

bool CX2StateDungeonGame::Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_UNIT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	//m_bGameEnd = true;
	if ( kEvent.m_bGameEnd == true )
	{
		m_bGameEnd = true;
	}

	m_bReceive_KEGS_UNIT_INFO_UPDATE = true;


	/*
	//////////////////////////////////////////////////////////////////////////
	// 새로운 던전이 열렸는지 체크해서 팝업창 띄우자~~

	if( true == kEvent.m_bGameEnd )
	{
	CX2Unit* pMyUnit = g_pX2Game->GetMyUnit()->GetUnit();
	if( NULL != pMyUnit )
	{
	//현재 던전을 깼으면 깬 던전 ID룰 받아오자...

	std::map< int, KDungeonClearInfo >::iterator it;
	for( it=kEvent.m_kUnitInfo.m_mapDungeonClear.begin(); it!=kEvent.m_kUnitInfo.m_mapDungeonClear.end(); it++ )
	{
	if( false == pMyUnit->IsClearDungeon( (int)it->first ) )
	{
	//새로 클리어 된 던전이다~					
	vector<int> vecNewDungeonID = g_pData->GetDungeonManager()->GetNextDungeon( (int) it->first );
	for( UINT i=0; i<vecNewDungeonID.size(); i++ )
	{

	if( false == pMyUnit->IsClearDungeon( (int)vecNewDungeonID[i] ) )
	{
	// 새로 열린 던전이다
	pMyUnit->AddNewlyOpenedDungeon( vecNewDungeonID[i] );
	}
	}
	}
	}
	}
	}

	//////////////////////////////////////////////////////////////////////////
	*/




	// 던전 보상 아이템 정보를 임시로 x2main에 저장해서 State Dungeon Result에서 화면에 뿌려
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

	bool bRet = m_pDungeonGame->Handler_EGS_UPDATE_UNIT_INFO_NOT( kEvent );



	// 던전 게임 끝나고 인벤토리에 획득 아이템 정보 update 끝나면 임시로 저장해뒀던 획득 아이템 정보를 clear, 
	// 퀘스트 정보창에서 수량 표시 오류나는 것 막기 위해서
	if( kEvent.m_bGameEnd == true )
	{
		// 		if( NULL != g_pMain->GetNewQuestUI() &&
		// 			NULL != g_pMain->GetNewQuestUI()->GetQuestPopUpWindow() )
		// 		{
		// 			g_pMain->GetNewQuestUI()->GetQuestPopUpWindow()->InitGetItemMap();	
		// 		}
		
	}

	return bRet;

}
//bool CX2StateDungeonGame::Handler_EGS_NOTIFY_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_NOTIFY_MSG_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	return m_pDungeonGame->Handler_EGS_NOTIFY_MSG_NOT( kEvent );;
//}

bool CX2StateDungeonGame::Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PITIN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	if ( g_pX2Room != NULL )
		return g_pX2Room->Handler_EGS_CHANGE_PITIN_NOT( kEvent );


	return false;
}

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
//{{ oasis907 : 김상윤 [2010.5.6] // 
bool CX2StateDungeonGame::Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL == m_pDungeonGame )
		return true;

	return m_pDungeonGame->Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( kEvent );

}
//}}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_EFFECT_TIME_OUT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL == m_pDungeonGame )
		return true;

	return m_pDungeonGame->Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( kEvent );
}
#endif DUNGEON_TIME_OUT_FRAMEMOVE





bool CX2StateDungeonGame::Handler_EGS_PLAY_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL == m_pDungeonGame )
		return true;

	if( true == m_pDungeonGame->GetGameEnd() )
		return true;


	if( true == m_pDungeonGame->GetKilledBoss() )		// 보스를 죽였으면 time out을 무시한다
		return true;


#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
	//{{ oasis907 : 김상윤 [2010.4.26] // 
	if( NULL != g_pData->GetDungeonRoom() )
	{
		switch(g_pData->GetDungeonRoom()->GetDungeonID())
		{
		case SEnum::DI_ALTERA_SECRET_COMMON:
		case SEnum::DI_ALTERA_SECRET_HELL:
			{
				m_pDungeonGame->CreateDamageDataForDungeonTimeOut();
			} 
			break;
			//{{ 허상형 : [2011/3/21/] //	월드미션
#ifdef SERV_INSERT_GLOBAL_SERVER
		CASE_DEFENCE_DUNGEON
			{
				if( CX2Room::TN_NONE == m_pDungeonGame->GetWinTeam() )
				{
					m_pDungeonGame->SetWindTeam( CX2Room::TN_RED );
					EndDungeonGameReq( (CX2Room::TEAM_NUM) m_pDungeonGame->GetWinTeam() );
				}
			}
			break;
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 허상형 : [2011/3/21/] //	월드미션
		default:
			{
				if( CX2Room::TN_NONE == m_pDungeonGame->GetWinTeam() )
				{
					m_pDungeonGame->SetWindTeam( CX2Room::TN_MONSTER );
					EndDungeonGameReq( (CX2Room::TEAM_NUM) m_pDungeonGame->GetWinTeam() );
				}
			}
			break;
		}
	}
#else
	if( CX2Room::TN_NONE == m_pDungeonGame->GetWinTeam() )			// 던전게임이 끝나지 않은 상황에서 time_out_not가 오면 던전 실패
	{
		m_pDungeonGame->SetWindTeam( CX2Room::TN_MONSTER );
		EndDungeonGameReq( (CX2Room::TEAM_NUM) m_pDungeonGame->GetWinTeam() );
	}
	//}} oasis907 : 김상윤 [2010.4.26] // 
#endif DUNGEON_TIME_OUT_FRAMEMOVE

	return true;
}

#ifdef HENIR_TEST

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ()
{
	if( NULL != g_pX2Game )
	{
		if( true == g_pX2Game->IsHost() )
		{
			g_pData->GetServerProtocol()->SendID( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
			g_pMain->AddServerPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK );
			return true;
		}
	}

	return false;
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK ) == true )
	{
		if( false == kEvent.m_bCanEnterSecretStage )
		{
			// 일반 스테이지로 진행한다
			if( NULL != m_pDungeonGame &&
				NULL != m_pDungeonGame->GetDungeon() &&
				NULL != m_pDungeonGame->GetDungeon()->GetNowStage() &&
				NULL != m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage() )
			{
#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
				SecretDungeonStageLoadReq( CX2DungeonSubStage::SSP_NORMAL );
#else // SERV_STAGE_CLEAR_IN_SERVER
				ASSERT( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextNormalStageIndex() > 0 );

				CX2StateDungeonGame* pCX2StateDungeonGame = (CX2StateDungeonGame*)g_pMain->GetNowState();
				DungeonStageLoadReq( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextNormalStageIndex() );
#endif // SERV_STAGE_CLEAR_IN_SERVER
			}
		}
	}

	return true;
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef NEW_HENIR_TEST
	if( NULL != m_pDungeonGame &&
		NULL != m_pDungeonGame->GetDungeon() &&
		NULL != m_pDungeonGame->GetDungeon()->GetNowStage() &&
		NULL != m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage() )
	{
#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
		m_iStartSecretStageEnteringEvent = kEvent.m_iStartSecretStageEnteringEvent;
#else // SERV_STAGE_CLEAR_IN_SERVER
		CX2StateDungeonGame* pCX2StateDungeonGame = (CX2StateDungeonGame*)g_pMain->GetNowState();

		if( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextSecretStageIndex() > 0 &&
			m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextEliteStageIndex() > 0)
		{
			m_iStartSecretStageEnteringEvent = 3;
		}
		else if( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextEliteStageIndex() > 0 )
		{
			m_iStartSecretStageEnteringEvent = 2;
		}
		else if( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextSecretStageIndex() > 0 )
		{
			m_iStartSecretStageEnteringEvent = 1;
		}
		else
		{
			m_iStartSecretStageEnteringEvent = 0;
		}
#endif // SERV_STAGE_CLEAR_IN_SERVER
	}
#else
	// 비밀스테이지 입장 연출을 시작한다.
	GetSecretStageEnteringEvent()->m_fElapsedTime_ShowClearEffect = 0.f;
#endif NEW_HENIR_TEST
	return true;
}

bool CX2StateDungeonGame::Send_EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT( bool bWantToEnterSecretStage )
{
	KEGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT kPacket;
	kPacket.m_bWantToEnterSecretStage = bWantToEnterSecretStage;

	g_pData->GetServerProtocol()->SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT, kPacket );

	return true;
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	
	if( NULL != m_pDungeonGame &&
		NULL != m_pDungeonGame->GetDungeon() &&
		NULL != m_pDungeonGame->GetDungeon()->GetNowStage() &&
		NULL != m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage() )
	{
		CX2StateDungeonGame* pCX2StateDungeonGame = (CX2StateDungeonGame*)g_pMain->GetNowState();

		if( g_pX2Game->IsHost() == true )
		{
			if( true == kEvent.m_bProceedToEnterSecretStage )
			{
#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
				SecretDungeonStageLoadReq( CX2DungeonSubStage::SSP_SECRET );
#else // SERV_STAGE_CLEAR_IN_SERVER
				ASSERT( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextSecretStageIndex() > 0 );
				DungeonStageLoadReq( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextSecretStageIndex() );
#endif // SERV_STAGE_CLEAR_IN_SERVER
			}
			else
			{
#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
				SecretDungeonStageLoadReq( CX2DungeonSubStage::SSP_NORMAL );
#else // SERV_STAGE_CLEAR_IN_SERVER
				ASSERT( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextNormalStageIndex() > 0 );
				DungeonStageLoadReq( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextNormalStageIndex() );
#endif // SERV_STAGE_CLEAR_IN_SERVER
			}
		}
	}

	return true;
}

#endif HENIR_TEST

#ifdef NEW_HENIR_TEST
bool CX2StateDungeonGame::SecretDungeonStageLoadReq(int iVal)
{
	if( NULL != m_pDungeonGame &&
		NULL != m_pDungeonGame->GetDungeon() &&
		NULL != m_pDungeonGame->GetDungeon()->GetNowStage() &&
		NULL != m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage() )
	{
		if( g_pX2Game->IsHost() == true )
		{
#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
			ASSERT( iVal >= 1 && iVal <= 3 );
			if( iVal < 1 || iVal > 3 )
			{
				StateLog( L"iVal < 1 || iVal > 3" );
				return false;
			}

			KEGS_SECRET_STAGE_LOAD_REQ kPacket;
			kPacket.m_iPadID = iVal;

			g_pData->GetServerProtocol()->SendPacket( EGS_SECRET_STAGE_LOAD_REQ, kPacket ); 
			g_pMain->AddServerPacket( EGS_DUNGEON_STAGE_LOAD_ACK, 60.f );
#else // SERV_STAGE_CLEAR_IN_SERVER
			if( iVal == 1 )
			{
				ASSERT( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextSecretStageIndex() > 0 );
				DungeonStageLoadReq( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextSecretStageIndex() );
			}
			else if( iVal == 2 )
			{
				ASSERT( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextEliteStageIndex() > 0 );
				DungeonStageLoadReq( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextEliteStageIndex() );
			}
			else
			{
				ASSERT( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextNormalStageIndex() > 0 );
				DungeonStageLoadReq( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetNextNormalStageIndex() );
			}
#endif // SERV_STAGE_CLEAR_IN_SERVER
		}
	}
	return true;
}
#endif NEW_HENIR_TEST


bool CX2StateDungeonGame::Handler_EGS_DUNGEON_KILLALLNPC_CHECK_REQ()
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

	if( NULL != g_pX2Game )
	{
		if( true == g_pX2Game->IsHost() )
		{
			g_pData->GetServerProtocol()->SendID( EGS_DUNGEON_KILLALLNPC_CHECK_REQ );
			g_pMain->AddServerPacket( EGS_DUNGEON_KILLALLNPC_CHECK_ACK );
			return true;
		}
	}		

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif
	return false;
}

bool CX2StateDungeonGame::Handler_EGS_DUNGEON_KILLALLNPC_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

	bool bResult = false;
	CX2DungeonSubStage *pSubStage = NULL;

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_KILLALLNPC_CHECK_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_pDungeonGame != NULL && 
		m_pDungeonGame->GetDungeon() != NULL && 
		m_pDungeonGame->GetDungeon()->GetNowStage() != NULL )
	{
		pSubStage = m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage();
	}

	if( g_pMain->DeleteServerPacket( EGS_DUNGEON_KILLALLNPC_CHECK_ACK ) == true )
	{
		// kill all npc 관련 함수를 호출할수 있는 시점이라면
		if( g_pX2Game->IsHost() == true && kEvent.m_bResult == true )
		{
			// active false인 몬스터만 죽인다.
			for( int i=0; i<g_pX2Game->GetNPCUnitListSize(); i++ )
			{
				CX2GUNPC* pCX2GUNPC = g_pX2Game->GetNPCUnit(i);
				if( pCX2GUNPC != NULL 
					&& pCX2GUNPC->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_THING_CHECKER
					)
				{					
					pCX2GUNPC->ResetGameUnitWhoAttackedMe();

					if(	pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID != CX2UnitManager::NUI_ENT_EVENT
						&& pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID != CX2UnitManager::NUI_TREE_KNIGHT_EVENT
						&& pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID != CX2UnitManager::NUI_ENT_SMALL_EVENT 
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
						&& pCX2GUNPC->GetNPCTemplet().m_nNPCUnitID != CX2UnitManager::NUI_SEED_OF_DARK_PORTAL 
#endif // SERV_NEW_DEFENCE_DUNGEON
						)
						pCX2GUNPC->SetNowHp( 0.f );
					
					//pCX2GUNPC->GetGageManager()->GetHPGage()->fNow = 0.0f;
					pCX2GUNPC->SetSelfResurrection( false );				
				}
			}

			bResult = true;
		}
	}

#ifdef _SERVICE_
	// 핵유저로 판단된다면 핵유저등록하고 핵실드검사 요청한다.
	if( bResult == false &&
		g_pData != NULL &&
		g_pData->GetMyUser() != NULL )
	{
		g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
		g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
		g_pData->GetServerProtocol()->SendID( EGS_REQUEST_HACKSHIELD_CHECK_NOT );

#ifdef ADD_COLLECT_CLIENT_INFO	// 임규수 일본 추가
#ifdef ADD_HACK_FILTER_58
		//{{ // 2012.07.03 lygan_조성욱 // 비정상 적인 곳에서 KILLALLNPC 가 된다면 우선 킵하자
		if( g_pMain != NULL )
			g_pMain->SendHackInfo2( ANTIHACKING_ID::ANTIHACKING_GAME_58, "", false, false );
		//}}
#endif ADD_HACK_FILTER_58
#endif //ADD_COLLECT_CLIENT_INFO
		//if( g_pMain != NULL )
		//	g_pMain->SendHackMail_DamageHistory("AntiHacking AllKillNpc");
	}
#endif

#ifndef SERV_FIX_NONE_NPC_DUNGEON_LINES // 오현빈
	// 던전클리어 진행시킨다.
	if( pSubStage != NULL )
		pSubStage->ClearDungeonGame();
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES
	

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

	return bResult;
}

//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
bool CX2StateDungeonGame::Handler_EGS_USER_GAME_STAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USER_GAME_STAT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( m_pDungeonGame != NULL )
	{
		CX2GUUser*	pUser = m_pDungeonGame->GetUserUnitByUID( kEvent.m_iUnitUID );
		if ( pUser == NULL )
			return false;

		CX2Unit*	pUnit = pUser->GetUnit();
		if ( pUnit == NULL )
			return false;

		CX2Unit::UnitData*	pUnitData	= &pUnit->AccessUnitData();

		pUser->SetGameStat( kEvent.m_kGameStat );
		pUnitData->m_GameStat.SetKStat( kEvent.m_kGameStat );

		pUser->SetMaxHp( static_cast< const float >( kEvent.m_kGameStat.m_iBaseHP ) );
		if ( pUser->GetNowHp() > pUser->GetMaxHp() )
		{
			pUser->SetNowHp( pUser->GetMaxHp() );
		}

		pUser->UpdateSocketDataAndEnchantData();
		pUser->UpdatePassiveAndActiveSkillState();

		pUser->ResetMaxMP();
		if( pUser->GetNowMp() > pUser->GetMaxMp() )
		{
			pUser->SetNowMp( pUser->GetMaxMp() );
		}
		bool bPartyHasTooniEventTitle = false;
		set<int>::const_iterator sItrTitleID = kEvent.m_setTitleBuffList.begin();
		while ( sItrTitleID != kEvent.m_setTitleBuffList.end() )
		{
			if ( TOONILAND_CHANNELING_EVENT_TITLE == *sItrTitleID )
			{
				bPartyHasTooniEventTitle = true;
				break;
			}

			sItrTitleID++;
		}

		g_pData->GetPremiumBuffManager()->SetTooniLandChannelingEvent( bPartyHasTooniEventTitle );
		pUser->SetShowTooniLandEventEffectSet( bPartyHasTooniEventTitle );
		return true;
	}

	return false;
}
#endif	SERV_TOONILAND_CHANNELING_EVENT
//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM



bool CX2StateDungeonGame::Handler_EGS_BAD_ATTITUDE_USER_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )	// 불량 유저 MSG 에 대한 처리
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BAD_ATTITUDE_USER_MSG_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	switch ( kEvent.m_MsgType )
	{
		case KEGS_BAD_ATTITUDE_USER_MSG_NOT::MT_BAD_ATTITUDE_USER_WARNING :			// 불량 유저에 대한 경고 메시지
		{
#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
			AllocateFaultyPlayerWarningDlg();
#else 
			m_BadAttitudeUserWarningDialog.Open( static_cast<CKTDXStage*> ( g_pMain->GetNowState() ) );	// m_BadAttitudeUserWarningDialog 를 생성한다.
#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG		
		}
		break;


		case KEGS_BAD_ATTITUDE_USER_MSG_NOT::MT_BAD_ATTITUDE_USER_EXIT :			// 불량 유저가 강퇴당했다고 일반 유저에게 전하는 메시지
		{

			CX2GageManager* pGageManager = CX2GageManager::GetInstance();		// 인스턴스를 받아온다.
			if ( NULL != pGageManager )
			{
				CX2PartyMemberGageUI * pPartyMemberGageUI = pGageManager->GetPartyMemberDataByUID ( kEvent.m_iUserUID );// 받아온 인스턴스로 Party UI 를 받아온다.

				if ( NULL != pPartyMemberGageUI )
				{
					wstring wstrMessage = pPartyMemberGageUI->GetNicknameThisPartyMemeber() + GET_STRING ( STR_ID_22468 );	//	해당 파티원의 NickName 을 받아와서 문자열을 만든다.
					g_pChatBox->AddChatLog( wstrMessage.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" ); // ### 유저가 놀아서 튕겼습니다.
				}
			}
		}
		break;


		case KEGS_BAD_ATTITUDE_USER_MSG_NOT::MT_FORCED_EXIT :		// 불량 유저 강퇴 메시지
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(310,500), GET_STRING( STR_ID_22422 ), g_pMain->GetNowState() );		// 메시지 박스를 연다/.
		}
		break;
	}
	
	return true;
}




#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

//////////////////////////////////////////////////////////////////////////
void CX2StateDungeonGame::ClearTutorialInputTreeParticle()
{
	for( UINT i=0; i<m_vecTutorialInputTreeParticle.size(); i++ )
	{
		CKTDGParticleSystem::CParticleEventSequenceHandle hSeq = m_vecTutorialInputTreeParticle[i];
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( hSeq );
	}
	m_vecTutorialInputTreeParticle.erase( m_vecTutorialInputTreeParticle.begin(), m_vecTutorialInputTreeParticle.end() );


	m_vTutorialParticlePos = D3DXVECTOR3(80, 700, 0);

}



HRESULT CX2StateDungeonGame::TutorialFrameMove( double fTime, float fElapsedTime )
{
	if( false == m_pDungeonGame->GetShowTutorialUI() )
	{
		m_pDLGTutorialInputControl->SetShow(false);
		return S_OK;
	}
	else
	{
		m_pDLGTutorialInputControl->SetShow(true);
	}

	CKTDGUIStatic* pStatic_SkillList_Fold	= (CKTDGUIStatic*) m_pDLGTutorialInputControl->GetControl( L"Static_Skill_List_Fold" );
	CKTDGUIStatic* pStatic_SkillList_Unfold	= (CKTDGUIStatic*) m_pDLGTutorialInputControl->GetControl( L"Static_Skill_List_Unfold" );


	if( NULL != g_pData->GetSlideShot() && 
		true == g_pData->GetSlideShot()->IsPresentNow() )
	{
		if( NULL != m_pDLGTutorialInputControl )
		{
			m_pDLGTutorialInputControl->SetShowEnable( false, false );
		}

		m_pDungeonGame->SetIsThereTutorialMessage( false );
	}
	else
	{
#ifdef OLD_COMBO_LIST_UI
		//{{AFX

		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_L) == TRUE && m_pDungeonGame->GetOpenChatBox() == false )
		{
			if( true == m_bShowSkilList )
			{
				m_bShowSkilList = false;
			}
			else
			{
				m_bShowSkilList = true;
			}
		}


		if( true == m_bShowSkilList )
		{
			pStatic_SkillList_Fold->SetShow( false );
			pStatic_SkillList_Unfold->SetShow( true );
		}
		else
		{
			pStatic_SkillList_Fold->SetShow( true );
			pStatic_SkillList_Unfold->SetShow( false );	
		}
		//}}AFX
#endif OLD_COMBO_LIST_UI

		if( NULL != m_pDLGTutorialInputControl )
		{
			m_pDLGTutorialInputControl->SetShowEnable( true, true );
		}
	}

	CKTDGUIStatic* pStatic_TutorialMsg_Flash = m_pDLGTutorialInputControl->GetStatic_LUA( "Static_Tutorial_Message" );

	if( true == m_pDungeonGame->GetIsThereTutorialMessage() )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hTutorialMissionSplashParticle )
		{
			m_hTutorialMissionSplashParticle = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"TutorialMissionSplash", 
				512, 274, 1000, 1000, 1, 1 );
		}
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hTutorialMissionSplashParticle );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject(false);
		}
	}
	else
	{
		pStatic_TutorialMsg_Flash->SetShow( false );
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hTutorialMissionSplashParticle );
	}




	if( m_fTimeLeftForHighlight > 0.f )
	{
		m_fTimeLeftForHighlight -= fElapsedTime;
		if( m_fTimeLeftForHighlight <= 0.f )
		{
			m_fTimeLeftForHighlight = 0.f;
			m_HighLightSkillID = (char)-1;
		}
	}


	if( true == m_bShowSkilList )
	{
		CX2GUUser* pCX2GUUser = m_pDungeonGame->GetMyUnit();
		if( pCX2GUUser != NULL )
		{

			CX2Unit::UNIT_TYPE eUnitType = pCX2GUUser->GetUnit()->GetType();
			char nowUserStateID = (char)pCX2GUUser->GetNowStateID();
			// note!! 전직 커맨드 추가되면 여기에 작업
			switch( eUnitType )
			{
			case CX2Unit::UT_ELSWORD:
				{
					switch( nowUserStateID )
					{
					case CX2GUElsword_SwordMan::ESSI_COMBO_Z4:
					case CX2GUElsword_SwordMan::ESSI_COMBO_X3a:
					case CX2GUElsword_SwordMan::ESSI_COMBO_X3:
					case CX2GUElsword_SwordMan::ESSI_COMBO_Z4a:
					case CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z2:
					case CX2GUElsword_SwordMan::ESSI_DASH_ATTACK_X:
					case CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_Z:
					case CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_X:
					case CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_Z3:
					case CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2:

						// knight
					case CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3:
					case CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3a:
					case CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X3:

						// magic knight 
					case CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2a:
						{
							if( m_HighLightSkillID != nowUserStateID )
								CreateSkillSuccessEffect();

							m_HighLightSkillID = nowUserStateID;
							m_fTimeLeftForHighlight = 0.5f;
						} break;
					}
				} break;

			case CX2Unit::UT_ARME:
				{
					switch( nowUserStateID )
					{
					case CX2GUArme_VioletMage::AVSI_COMBO_Z3:
					case CX2GUArme_VioletMage::AVSI_COMBO_Z4up:
					case CX2GUArme_VioletMage::AVSI_COMBO_X3:
					case CX2GUArme_VioletMage::AVSI_COMBO_X3down:
					case CX2GUArme_VioletMage::AVSI_COMBO_X3front:

					case CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2:
					case CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2a:
					case CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X:

					case CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_Z:
					case CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_X:

					case CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_Z:
					case CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X:

						// high magician
					case CX2GUArme_VioletMage::AVSI_COMBO_X3back:
					case CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X2:
					case CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X2:

						// dark magician
					case CX2GUArme_VioletMage::AVSI_COMBO_X4down:
						//case CX2GUArme_VioletMage::AVSI_COMBO_X3back:
					case CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z3up:
						{
							if( m_HighLightSkillID != nowUserStateID )
								CreateSkillSuccessEffect();

							m_HighLightSkillID = nowUserStateID;
							m_fTimeLeftForHighlight = 0.5f;

						} break;
					}
				} break;

			case CX2Unit::UT_LIRE:
				{
					switch( nowUserStateID )
					{
					case CX2GULire_ElvenRanger::LESI_COMBO_Z4:
					case CX2GULire_ElvenRanger::LESI_COMBO_Z4UP:
					case CX2GULire_ElvenRanger::LESI_COMBO_Z4a:
					case CX2GULire_ElvenRanger::LESI_COMBO_X4:
					case CX2GULire_ElvenRanger::LESI_COMBO_X4UP:
					case CX2GULire_ElvenRanger::LESI_COMBO_X4DOWN:
					case CX2GULire_ElvenRanger::LESI_DASH_COMBO_Z2:
					case CX2GULire_ElvenRanger::LESI_DASH_ATTACK_X:
					case CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_Z:
					case CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_X:
					case CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP_ATTACK_Z:
					case CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_Z:
					case CX2GULire_ElvenRanger::LESI_DASH_JUMP_COMBO_X3:

						// combat ranger
					case CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_COMBO_Z2:
					case CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3:

						// sniping ranger
		#ifndef BALANCE_GRAND_ARCHER_20121213
					case CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3:
		#endif //BALANCE_GRAND_ARCHER_20121213
					case CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X:

						{
							if( m_HighLightSkillID != nowUserStateID )
								CreateSkillSuccessEffect();

							m_HighLightSkillID = nowUserStateID;
							m_fTimeLeftForHighlight = 0.5f;

						} break;
					}
				} break;

			case CX2Unit::UT_RAVEN:
				{
					switch( nowUserStateID )
					{
					case CX2GURaven::RSI_COMBO_ZZZZ:				
					case CX2GURaven::RSI_COMBO_ZZZX:				
					case CX2GURaven::RSI_COMBO_ZZXX:				
					case CX2GURaven::RSI_COMBO_XXX:				
					case CX2GURaven::RSI_DASH_COMBO_ZZ:			
					case CX2GURaven::RSI_DASH_COMBO_X:			
					case CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ:		
					case CX2GURaven::RSI_DASH_JUMP_COMBO_X:	
					case CX2GURaven::RSI_SOUL_TAKER_COMBO_ZZZupZ:		
					case CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8Z:		
					case CX2GURaven::RSI_SOUL_TAKER_DASH_COMBO_ZZZ:	
					case CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownXdownX:	
					case CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX:				
					case CX2GURaven::RSI_OVER_TAKER_COMBO_XXfrontX:			
						{
							if( m_HighLightSkillID != nowUserStateID )
								CreateSkillSuccessEffect();

							m_HighLightSkillID = nowUserStateID;
							m_fTimeLeftForHighlight = 0.5f;

						} break;
					}
				} break;


			case CX2Unit::UT_EVE:
				{
					switch( nowUserStateID )
					{
					case CX2GUEve::ENSI_COMBO_ZZZZ:			
					case CX2GUEve::ENSI_COMBO_ZZZdownZ:		
					case CX2GUEve::ENSI_COMBO_ZZZX:			
					case CX2GUEve::ENSI_COMBO_XXX:			
					case CX2GUEve::ENSI_COMBO_XXfrontX:		
					case CX2GUEve::ENSI_COMBO_XXdownX:		
					case CX2GUEve::ENSI_DASH_COMBO_Z:		
					case CX2GUEve::ENSI_DASH_COMBO_X:		
					case CX2GUEve::ENSI_DASH_JUMP_COMBO_ZZZ:	
					case CX2GUEve::ENSI_DASH_JUMP_COMBO_X:	
#ifdef EVE_FIRST_CHANGE_JOB
					case CX2GUEve::ENSI_COMBO_XXZ4:				
					case CX2GUEve::ENSI_DASH_JUMP_COMBO_XZ:
					case CX2GUEve::ENSI_COMBO_ZZZdownZZ:
					case CX2GUEve::ENSI_DASH_COMBO_ZZZ:
					case CX2GUEve::ENSI_DASH_COMBO_XZ:
#endif
						{
							if( m_HighLightSkillID != nowUserStateID )
								CreateSkillSuccessEffect();

							m_HighLightSkillID = nowUserStateID;
							m_fTimeLeftForHighlight = 0.5f;

						} break;
					}
				} break;

		//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
	#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UT_CHUNG:
				{
					switch ( nowUserStateID )
					{
					case CX2GUUser::USI_WAIT:
						{
							if( m_HighLightSkillID != nowUserStateID )
								CreateSkillSuccessEffect();

							m_HighLightSkillID = nowUserStateID;
							m_fTimeLeftForHighlight = 0.5f;
						} break;
					} // switch

				} break;

	#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UT_ARA:
				{
					switch ( nowUserStateID )
					{
					case CX2GUUser::USI_WAIT:
						{
							if( m_HighLightSkillID != nowUserStateID )
								CreateSkillSuccessEffect();

							m_HighLightSkillID = nowUserStateID;
							m_fTimeLeftForHighlight = 0.5f;
						} break;
					}

				} break;
#endif

			}
		}
	}




	if( NULL != m_pDLGTutorialInputControl )
	{
		// 키입력이 일정 이상 계속 되면 강제로 삭제 해주자
		const UINT MAGIC_NUM_KEYBOARD_PARTICLE = 8;
		if( m_vecTutorialInputTreeParticle.size() > MAGIC_NUM_KEYBOARD_PARTICLE )
		{
			ClearTutorialInputTreeParticle();
		}
#if 0
		CX2GUUser* pCX2GUUser = m_pDungeonGame->GetMyUnit();
		if( pCX2GUUser != NULL )
		{
			const CX2GUUser::InputData& inputData = pCX2GUUser->GetInputData();

			//if( false == g_pMain->GetIsFreeStyleTutorial() )
			{
				CKTDGUIStatic* pStatic_InputControl = m_pDLGTutorialInputControl->GetStatic_LUA( "Static_InputControl" );

				if( true == inputData.pureZ )
				{
					pStatic_InputControl->GetPicture( 0 )->SetShow( false );
					pStatic_InputControl->GetPicture( 1 )->SetShow( true );
				}
				else
				{
					pStatic_InputControl->GetPicture( 0 )->SetShow( true );
					pStatic_InputControl->GetPicture( 1 )->SetShow( false );
				}

				if( true == inputData.pureX )
				{
					pStatic_InputControl->GetPicture( 2 )->SetShow( false );
					pStatic_InputControl->GetPicture( 3 )->SetShow( true );
				}
				else
				{
					pStatic_InputControl->GetPicture( 2 )->SetShow( true );
					pStatic_InputControl->GetPicture( 3 )->SetShow( false );
				}


				if( true == inputData.pureLeft )
				{
					pStatic_InputControl->GetPicture( 4 )->SetShow( false );
					pStatic_InputControl->GetPicture( 5 )->SetShow( true );
				}
				else
				{
					pStatic_InputControl->GetPicture( 4 )->SetShow( true );
					pStatic_InputControl->GetPicture( 5 )->SetShow( false );
				}

				if( true == inputData.pureRight )
				{
					pStatic_InputControl->GetPicture( 6 )->SetShow( false );
					pStatic_InputControl->GetPicture( 7 )->SetShow( true );
				}
				else
				{
					pStatic_InputControl->GetPicture( 6 )->SetShow( true );
					pStatic_InputControl->GetPicture( 7 )->SetShow( false );
				}



				if( true == inputData.pureUp )
				{
					pStatic_InputControl->GetPicture( 8 )->SetShow( false );
					pStatic_InputControl->GetPicture( 9 )->SetShow( true );
				}
				else
				{
					pStatic_InputControl->GetPicture( 8 )->SetShow( true );
					pStatic_InputControl->GetPicture( 9 )->SetShow( false );
				}

				if( true == inputData.pureDown )
				{
					pStatic_InputControl->GetPicture( 10 )->SetShow( false );
					pStatic_InputControl->GetPicture( 11 )->SetShow( true );
				}
				else
				{
					pStatic_InputControl->GetPicture( 10 )->SetShow( true );
					pStatic_InputControl->GetPicture( 11 )->SetShow( false );
				}
			}
		}
#endif
	}

	return S_OK;
}


void CX2StateDungeonGame::TextOutSkillList( const char unitStateID )
{
	if( false == m_bShowSkilList )
		return;

	if( NULL == m_pDungeonGame )
		return;
	CX2GUUser* pCX2GUUser = m_pDungeonGame->GetMyUnit();
	if( NULL == pCX2GUUser )
		return;

	CX2Unit::UNIT_CLASS unitClass = pCX2GUUser->GetUnit()->GetClass();
	char nowUserStateID = (char)pCX2GUUser->GetNowStateID();


	const D3DXCOLOR coOutline		= D3DXCOLOR(  0.f, 0.f,  0.f,  1.f );
	const D3DXCOLOR coText			= D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.f );
	const D3DXCOLOR coTextHighlight	= D3DXCOLOR( 0.2f, 1.f,  0.5f, 1.f );
	m_coHighlight += m_coHighLightIncrement;
	if( m_coHighlight.g > coTextHighlight.g )
	{
		m_coHighlight = coTextHighlight;
		m_coHighLightIncrement = -m_coHighLightIncrement;
	}
	else if( m_coHighlight.g < coText.g )
	{
		m_coHighlight = coText;
		m_coHighLightIncrement = -m_coHighLightIncrement;
	}


	const int iPosX = 770;
	int iPosY = 320;
	const int iPosHeight = 22;


	map<char, wstring>& mapCommandList = m_mapUnitCommandList[ g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass ];
	map<char, wstring>::iterator it;

	for( it = mapCommandList.begin() ; it != mapCommandList.end(); it++ )
	{
		if( it->first == unitStateID )
		{
			m_pFontForSkillList->OutTextXY( iPosX, iPosY, it->second.c_str(), 
				m_coHighlight, CKTDGFontManager::FS_SHELL, coOutline, NULL, DT_LEFT );

			if( true == m_bCreateCommandSuccessSplash )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = 
					g_pX2Game->GetMajorParticle()->CreateSequence( NULL,  L"SkillSuccessSplash", (float)iPosX+120, (float)iPosY+10, 0, 1000, 1000, 1, 1 );
				m_bCreateCommandSuccessSplash = false;
			}
		}
		else
		{
			m_pFontForSkillList->OutTextXY( iPosX, iPosY, it->second.c_str(), 
				coText, CKTDGFontManager::FS_SHELL, coOutline, NULL, DT_LEFT );
		}
		iPosY += iPosHeight;
	}
}

/*virtual*/
void CX2StateDungeonGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	if( NULL == g_pData->GetDungeonRoom() )
		return; 

	CX2Room::SlotData* pkSlotData = g_pData->GetDungeonRoom()->GetSlotDataByUnitUID( iUnitUID_ );
	if ( pkSlotData == NULL )
		return;

	int iSlotIndex = pkSlotData->m_Index;
	if( iSlotIndex < 0 )
		return;	

	if( NULL != m_pDungeonGame )
	{
		CX2GUUser* pGUUser = m_pDungeonGame->GetUserUnitByUID( iUnitUID_ );
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



void CX2StateDungeonGame::ToggleDungeonMapUI()
{
	//비던, 어문의 경우에는 던전맵 보여주지 않도록 설정
	if( NULL != m_pDungeonGame && NULL != m_pDungeonGame->GetDungeon() &&
		NULL != m_pDungeonGame->GetDungeon()->GetDungeonData() )
	{
		const CX2Dungeon::DungeonData* pDungeonData = m_pDungeonGame->GetDungeon()->GetDungeonData();
		if( NULL != pDungeonData )
		{
#ifdef EVENT_BM_DUNGEON_MINIMAP_OFF
			if( SEnum::DI_EVENT_AISHA_ELDER == pDungeonData->m_DungeonID )
				return;	
#endif //EVENT_BM_DUNGEON_MINIMAP_OFF
			if( CX2Dungeon::DT_SECRET == pDungeonData->m_eDungeonType )
				return;

			if( true == g_pData->GetDungeonManager()->IsDefenceDungeon( pDungeonData->m_DungeonID )  )
				return;

#ifdef SERV_EVENT_VALENTINE_DUNGEON
			if( SEnum::DI_EVENT_VALENTINE_DAY == pDungeonData->m_DungeonID )
				return;
#endif //SERV_EVENT_VALENTINE_DUNGEON
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
			if( SEnum::DI_EVENT_VALENTINE_DUNGEON_INT == pDungeonData->m_DungeonID )
				return;
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
			if ( SEnum::DI_EVENT_HALLOWEEN_DAY == pDungeonData->m_DungeonID )
				return;
#endif //SERV_HALLOWEEN_EVENT_2013
		}
	}

	if( NULL != m_pDungeonMapUI )
	{
		if( true == m_pDungeonMapUI->GetShow() )
		{
			m_pDungeonMapUI->SetShow( false );
			m_pDungeonMapUI->SetToOriginalPos();
		}
		else
		{
			m_pDungeonMapUI->SetShow( true );
			m_pDungeonMapUI->SetToOriginalPos();
		}
	}
}


//{{ seojt // 2009-8-20, 11:55
//void CX2StateDungeonGame::OpenMonsterListWindow()
//{
//	if( m_pDLGMonsterList_Debug == NULL )
//	{
//		m_pDLGMonsterList_Debug = new CKTDGUIDialog( this, L"DLG_Monster_List_Window.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMonsterList_Debug );
//
//		UpdateMonsterListWindow( m_bSortMonsterList );
//	}
//
//	if( m_pDLGMonsterList_Debug != NULL )
//	{
//		m_pDLGMonsterList_Debug->SetShowEnable( true, true );
//	}
//}//CX2StateDungeonGame::OpenMonsterListWindow()
//}} seojt // 2009-8-20, 11:55


bool CX2StateDungeonGame::ShortCutKeyProcess()
{
	bool bHideDialog = ProcessGameScoreAndPostEffect();	

	if ( CX2StateMenu::ShortCutKeyProcess() == true )
		return true;

	if( NULL == g_pX2Game )
		return true;

	// 던전내에서 먼전 처리해야 할 키들
#ifndef _SERVICE_
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_SEMICOLON) == TRUE )
	{
		if( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
		{
			if( NULL != g_pX2Game )
			{
				g_pX2Game->KillAllNPC_LUA( false );
				return true;
			}
		}
	}
#endif _SERVICE_




	//if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_O) == TRUE )
	//{
	//	if( true == IsOptionWindowOpen() )
	//		CloseOptionWindow();
	//	else
	//		OpenOptionWindow();

	//	return true;
	//}

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


	//if( NULL != m_pDungeonGame )
	//{
	//	if ( m_pDungeonGame->UseQuickSlot() == true )
	//		return true;
	//}
	if ( bHideDialog == false && GET_KEY_STATE( GA_NAME ) == TRUE )
	{
		g_pX2Game->SetRenderNPCName( !g_pX2Game->GetRenderNPCName() );
		return true;
	}




	if ( CameraZoomInAndOutKeyProcess() )
		return true;

	if ( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_SIT ) == TRUE )
	{
		CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
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

	//if( NULL != m_pDungeonGame )
	//{
	//	if ( m_pDungeonGame->UseQuickSlot() == true )
	//		return true;
	//}

	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV )
	{
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_9 ) == TRUE )
		{
			m_pDungeonGame->GetWorld()->SetLOD( m_pDungeonGame->GetWorld()->GetLOD() - 5 );
			return true;
		}

		//if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_0 ) == TRUE )
		//{
		//	m_pDungeonGame->GetWorld()->SetLOD( m_pDungeonGame->GetWorld()->GetLOD() + 5 );
		//	return true;
		//}
#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_0) == TRUE )
		{
			g_pInstanceData->SetFrameScale( !g_pInstanceData->GetFrameScale() );
		}		
#endif

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_7 ) == TRUE )
		{
			g_pData->SetModelDetailPercent( g_pData->GetModelDetailPercent() - 5 );
			if ( g_pData->GetModelDetailPercent() <= 0 )
				g_pData->SetModelDetailPercent( 0 );

			for ( int i = 0; i < m_pDungeonGame->GetUnitNum(); i++ )
			{
				CX2GameUnit* pGameUnit = m_pDungeonGame->GetUnit( i );
				if ( pGameUnit != NULL )
					pGameUnit->ChangeModelDetail( g_pData->GetModelDetailPercent() );

			}


			return true;
		}

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_8 ) == TRUE )
		{
			g_pData->SetModelDetailPercent( g_pData->GetModelDetailPercent() + 5 );
			if ( g_pData->GetModelDetailPercent() >= 100 )
				g_pData->SetModelDetailPercent( 100 );

			for ( int i = 0; i < m_pDungeonGame->GetUnitNum(); i++ )
			{
				CX2GameUnit* pGameUnit = m_pDungeonGame->GetUnit( i );
				if ( pGameUnit != NULL )
					pGameUnit->ChangeModelDetail( g_pData->GetModelDetailPercent() );

			}

			return true;
		}

#ifdef RIDE_MACHINE_GUN

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_R ) == TRUE )
		{
			if (  m_pDungeonGame->GetMyUnit()->GetUnit()->GetUnitTemplet()->m_UnitType == CX2Unit::UT_ELSWORD )
			{
				if ( m_pDungeonGame->GetMyUnit()->GetIsRidingMachine() == false )
					m_pDungeonGame->GetMyUnit()->StateChange( CX2GUElsword_SwordMan::ESSI_MACHINE_GUN_RIDE );
				else
					m_pDungeonGame->GetMyUnit()->StateChange( CX2GUElsword_SwordMan::ESSI_MACHINE_GUN_LEAVE );
			}

			return true;
		}




#endif //RIDE_MACHINE_GUN

#ifdef LEAP_TEST

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_Y ) == TRUE )
		{
			if (  m_pDungeonGame->GetMyUnit()->GetUnit()->GetUnitTemplet()->m_UnitType == CX2Unit::UT_ELSWORD )
			{
				m_pDungeonGame->GetMyUnit()->StateChange( CX2GUElsword_SwordMan::ESSI_LEAP );
			}

			return true;
		}


#endif //LEAP_TEST

#ifdef FRAME_MOVE_CULL

		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_T ) == TRUE )
		{
			CKTDGXSkinAnim::SetTestUpdate( !CKTDGXSkinAnim::GetTestUpdate() );

			return true;
		}

#endif //FRAME_MOVE_CULL

#ifdef FOG_WORLD
		if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_APOSTROPHE ) == TRUE )
		{
			m_pDungeonGame->GetWorld()->ToggleFog();
			return true;
		}
#endif        
	}


	// 공용 키 처리 (던전내에서도 공용키처리할 경우
	//return g_pMain->KeyProcess();
	//return false;

	if ( bHideDialog == false && GET_KEY_STATE( GA_QUEST ) == TRUE )
	{
		//g_pMain->GetNewQuestUI()->SetOpenQuestPopUpWindow( !g_pMain->GetNewQuestUI()->GetOpenQuestPopUpWindow() );
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUEST);

		return true;
	}

	//{{ kimhc // 실시간 엘소드 중 실시간 인벤토리 열기 및 갱신
#ifdef	REAL_TIME_ELSWORD 
	if ( bHideDialog == false && GET_KEY_STATE( GA_INFO ) == TRUE )
	{
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_CHARINFO);

		return true;
	}

	if ( bHideDialog == false && GET_KEY_STATE( GA_INVENTORY ) == TRUE )
	{
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN);

		return true;
	}
#endif	REAL_TIME_ELSWORD 
	//}} kimhc // 실시간 엘소드 중 실시간 인벤토리 열기 및 갱신

#ifdef SERV_PET_SYSTEM
	if ( bHideDialog == false && GET_KEY_STATE( GA_PET ) == TRUE )
	{
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PET_LIST);

		return true;
	}
#endif

#ifndef OPEN_TEST_1_NO_MESSENGER_CASHSHOP

	// 던전게임중에는 커뮤니티 (전체탭 열지 못하도록한다.)
	//if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_T) == TRUE )
	//{
	//	g_pData->GetMessenger()->SetFriendTab(false);
	//	g_pData->GetMessenger()->SetOpen( !g_pData->GetMessenger()->GetOpen() );
	//	return true;
	//}
	if ( bHideDialog == false && GET_KEY_STATE( GA_FRIEND ) == TRUE )
	{
		g_pData->GetMessenger()->SetFriendTab(true);
		g_pData->GetMessenger()->SetOpen( !g_pData->GetMessenger()->GetOpen() );

		return true;
	}

#endif OPEN_TEST_1_NO_MESSENGER_CASHSHOP

	//{{ kimhc // 2009-10-13 // 길드 탭UI 단축키 지정
#ifdef	GUILD_MANAGEMENT
	// 커뮤니티(친구탭)
	if ( bHideDialog == false && GET_KEY_STATE( GA_GUILD ) == TRUE )
	{			
		g_pData->GetMessenger()->SetTabByShortCutKey( CX2Community::XMUT_GUILD );
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_COMMUNITY);
		return true;
	}
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-10-13 // 길드 탭UI 단축키 지정


#if 0
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_LALT ) == TRUE ) 
	{
		g_pX2Game->SetRenderNPCName( g_pX2Game->GetRenderNPCName() );
		return true;
	}	
#endif	

	//{{ 09.03.11 김태완 : 미니맵 최소화 기능 제거
	// 	if( false == g_pMain->GetIsPlayingTutorial() )
	// 	{
	// 		if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB) == TRUE )
	// 		{
	// 			if( NULL != g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap() )
	// 			{
	// 				g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->SetMinimized( !g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->GetMinimized() );
	// 			}
	// 			return true;
	// 		}
	//	}
	//}}

	if ( false == bHideDialog && 
		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB) == TRUE  &&
		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_LALT) != TRUE )
	{
		g_pData->GetUIManager()->GetUIQuestNew()->ToggleUserShowQuickQuestDLG();
	}

	return false;
}

/*virtual*/ void CX2StateDungeonGame::ProcessShowMiniMap( const bool bHide_ )
{
	if( g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )			
		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( g_pInstanceData->GetMiniMapUI()->GetCurrentMiniMapType(), !bHide_ );
}

#ifdef OLD_COMBO_LIST_UI
//{{AFX
void CX2StateDungeonGame::CreateComboListForTutorial()
{
	// note!! 전직 추가되면 수정

	m_mapUnitCommandList.clear();

	map<char, wstring>& mapElswordSwordman = m_mapUnitCommandList[ CX2Unit::UC_ELSWORD_SWORDMAN ];
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_COMBO_Z4] 					= GET_STRING( STR_ID_569 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_COMBO_Z4a]					= GET_STRING( STR_ID_570 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_COMBO_X3] 					= GET_STRING( STR_ID_571 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_COMBO_X3a]					= GET_STRING( STR_ID_572 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z2] 				= GET_STRING( STR_ID_573 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_DASH_ATTACK_X] 				= GET_STRING( STR_ID_574 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_Z]				= GET_STRING( STR_ID_575 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_X]				= GET_STRING( STR_ID_576 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_Z3]			= GET_STRING( STR_ID_577 );
	mapElswordSwordman[CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2]			= GET_STRING( STR_ID_578 );


	map<char, wstring>& mapElswordKnight = m_mapUnitCommandList[ CX2Unit::UC_ELSWORD_KNIGHT ];
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_COMBO_Z4] 						= GET_STRING( STR_ID_569 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_COMBO_Z4a]						= GET_STRING( STR_ID_570 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_COMBO_X3] 						= GET_STRING( STR_ID_571 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_COMBO_X3a]						= GET_STRING( STR_ID_572 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3] 				= GET_STRING( STR_ID_579 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3a] 				= GET_STRING( STR_ID_580 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_DASH_ATTACK_X] 				= GET_STRING( STR_ID_574 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_Z]					= GET_STRING( STR_ID_575 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_X]					= GET_STRING( STR_ID_576 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_Z3]			= GET_STRING( STR_ID_577 );
	mapElswordKnight[CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X3]			= GET_STRING( STR_ID_581 );


	map<char, wstring>& mapElswordMagicKnight = m_mapUnitCommandList[ CX2Unit::UC_ELSWORD_MAGIC_KNIGHT ];
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_COMBO_Z4] 				= GET_STRING( STR_ID_569 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_COMBO_Z4a]				= GET_STRING( STR_ID_570 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_COMBO_X3] 				= GET_STRING( STR_ID_571 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_COMBO_X3a]				= GET_STRING( STR_ID_572 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3] 			= GET_STRING( STR_ID_579 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_DASH_COMBO_Z3a] 			= GET_STRING( STR_ID_580 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_DASH_ATTACK_X] 			= GET_STRING( STR_ID_574 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_Z]			= GET_STRING( STR_ID_575 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_JUMP_ATTACK_X]			= GET_STRING( STR_ID_576 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_Z3]		= GET_STRING( STR_ID_577 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2]		= GET_STRING( STR_ID_578 );
	mapElswordMagicKnight[CX2GUElsword_SwordMan::ESSI_DASH_JUMP_COMBO_X2a]		= GET_STRING( STR_ID_582 );


	map<char, wstring>& mapArmeVioletMage = m_mapUnitCommandList[ CX2Unit::UC_ARME_VIOLET_MAGE ];
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_COMBO_Z3]						= GET_STRING( STR_ID_583 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_COMBO_Z4up]					= GET_STRING( STR_ID_584 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_COMBO_X3]						= GET_STRING( STR_ID_571 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_COMBO_X3down]					= GET_STRING( STR_ID_585 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_COMBO_X3front] 				= GET_STRING( STR_ID_586 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2] 				= GET_STRING( STR_ID_573 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2a]				= GET_STRING( STR_ID_587 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X] 				= GET_STRING( STR_ID_574 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_Z] 				= GET_STRING( STR_ID_575 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_X] 				= GET_STRING( STR_ID_576 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_Z] 			= GET_STRING( STR_ID_588 );
	mapArmeVioletMage[CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X] 			= GET_STRING( STR_ID_589 );

	map<char, wstring>& mapArmeHighMagician = m_mapUnitCommandList[ CX2Unit::UC_ARME_HIGH_MAGICIAN ];
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_COMBO_Z3]					= GET_STRING( STR_ID_583 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_COMBO_Z4up]					= GET_STRING( STR_ID_584 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_COMBO_X3]					= GET_STRING( STR_ID_571 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_COMBO_X3down]				= GET_STRING( STR_ID_585 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_COMBO_X3front] 				= GET_STRING( STR_ID_586 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_COMBO_X3back] 				= GET_STRING( STR_ID_590 );		// added
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2] 				= GET_STRING( STR_ID_573 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2a]				= GET_STRING( STR_ID_587 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X2] 				= GET_STRING( STR_ID_591 );		// modified
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_Z] 				= GET_STRING( STR_ID_575 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_X] 				= GET_STRING( STR_ID_576 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_Z] 			= GET_STRING( STR_ID_588 );
	mapArmeHighMagician[CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X2] 		= GET_STRING( STR_ID_578 );	// modified

	map<char, wstring>& mapArmeDarkMagician = m_mapUnitCommandList[ CX2Unit::UC_ARME_DARK_MAGICIAN ];
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_COMBO_Z3]					= GET_STRING( STR_ID_583 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_COMBO_Z4up]					= GET_STRING( STR_ID_584 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_COMBO_X3]					= GET_STRING( STR_ID_571 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_COMBO_X4down]				= GET_STRING( STR_ID_592 );		// modified
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_COMBO_X3front] 				= GET_STRING( STR_ID_586 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_COMBO_X3back] 				= GET_STRING( STR_ID_590 );		// added
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z2] 				= GET_STRING( STR_ID_573 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_DASH_COMBO_Z3up]				= GET_STRING( STR_ID_593 );		// modified
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_DASH_ATTACK_X] 				= GET_STRING( STR_ID_574 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_Z] 				= GET_STRING( STR_ID_575 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_JUMP_ATTACK_X] 				= GET_STRING( STR_ID_576 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_Z] 			= GET_STRING( STR_ID_588 );
	mapArmeDarkMagician[CX2GUArme_VioletMage::AVSI_DASH_JUMP_ATTACK_X] 			= GET_STRING( STR_ID_589 );


	map<char, wstring>& mapLireElvenRanger = m_mapUnitCommandList[ CX2Unit::UC_LIRE_ELVEN_RANGER ];
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4]					= GET_STRING( STR_ID_569 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4UP]					= GET_STRING( STR_ID_584 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4a]					= GET_STRING( STR_ID_570 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4]					= GET_STRING( STR_ID_594 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4UP]					= GET_STRING( STR_ID_595 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4DOWN]				= GET_STRING( STR_ID_596 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_DASH_COMBO_Z2]				= GET_STRING( STR_ID_573 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_DASH_ATTACK_X]				= GET_STRING( STR_ID_574 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_Z]				= GET_STRING( STR_ID_575 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_X]				= GET_STRING( STR_ID_576 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP_ATTACK_Z]		= GET_STRING( STR_ID_597 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_Z]			= GET_STRING( STR_ID_588 );
	mapLireElvenRanger[CX2GULire_ElvenRanger::LESI_DASH_JUMP_COMBO_X3]			= GET_STRING( STR_ID_581 );


	map<char, wstring>& mapLireCombatRanger = m_mapUnitCommandList[ CX2Unit::UC_LIRE_COMBAT_RANGER ];
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4]									= GET_STRING( STR_ID_569 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4UP]									= GET_STRING( STR_ID_584 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4a]									= GET_STRING( STR_ID_570 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4]									= GET_STRING( STR_ID_594 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4UP]									= GET_STRING( STR_ID_595 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4DOWN]								= GET_STRING( STR_ID_596 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_COMBO_Z2]				= GET_STRING( STR_ID_573 );			// 변경 
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_DASH_ATTACK_X]								= GET_STRING( STR_ID_574 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_Z]								= GET_STRING( STR_ID_575 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_X]								= GET_STRING( STR_ID_576 );
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP]	= GET_STRING( STR_ID_598 );		// modified
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3]			= GET_STRING( STR_ID_577 );		// 변경 
	mapLireCombatRanger[CX2GULire_ElvenRanger::LESI_DASH_JUMP_COMBO_X3]							= GET_STRING( STR_ID_581 );



	map<char, wstring>& mapLireSnipingRanger = m_mapUnitCommandList[ CX2Unit::UC_LIRE_SNIPING_RANGER ];
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4]								= GET_STRING( STR_ID_569 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4UP]							= GET_STRING( STR_ID_584 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_COMBO_Z4a]								= GET_STRING( STR_ID_570 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4]								= GET_STRING( STR_ID_594 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4UP]							= GET_STRING( STR_ID_595 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_COMBO_X4DOWN]							= GET_STRING( STR_ID_596 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_DASH_COMBO_Z2]							= GET_STRING( STR_ID_573 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DASH_COMBO_X3]			= GET_STRING( STR_ID_599 );		// 변경
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_Z]							= GET_STRING( STR_ID_575 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_JUMP_ATTACK_X]							= GET_STRING( STR_ID_576 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_DOUBLE_JUMP_ATTACK_Z]					= GET_STRING( STR_ID_597 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X]	= GET_STRING( STR_ID_600 );		 // 추가
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_DASH_JUMP_ATTACK_Z]					= GET_STRING( STR_ID_588 );
	mapLireSnipingRanger[CX2GULire_ElvenRanger::LESI_DASH_JUMP_COMBO_X3]					= GET_STRING( STR_ID_581 );

	map<char, wstring>& mapRavenFighter = m_mapUnitCommandList[ CX2Unit::UC_RAVEN_FIGHTER ];
	mapRavenFighter[CX2GURaven::RSI_COMBO_ZZZZ]								= GET_STRING( STR_ID_569 );
	mapRavenFighter[CX2GURaven::RSI_COMBO_ZZZX]								= GET_STRING( STR_ID_570 );
	mapRavenFighter[CX2GURaven::RSI_COMBO_ZZXX]								= GET_STRING( STR_ID_601 );
	mapRavenFighter[CX2GURaven::RSI_COMBO_XXX]								= GET_STRING( STR_ID_571 );
	mapRavenFighter[CX2GURaven::RSI_DASH_COMBO_ZZ]							= GET_STRING( STR_ID_573 );
	mapRavenFighter[CX2GURaven::RSI_DASH_COMBO_X]							= GET_STRING( STR_ID_574 );
	mapRavenFighter[CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ]						= GET_STRING( STR_ID_602 );
	mapRavenFighter[CX2GURaven::RSI_DASH_JUMP_COMBO_X]						= GET_STRING( STR_ID_589 );


	map<char, wstring>& mapRavenSoulTaker = m_mapUnitCommandList[ CX2Unit::UC_RAVEN_SOUL_TAKER ];
	mapRavenSoulTaker[CX2GURaven::RSI_COMBO_ZZZZ]							= GET_STRING( STR_ID_569 );
	mapRavenSoulTaker[CX2GURaven::RSI_SOUL_TAKER_COMBO_ZZZupZ]				= GET_STRING( STR_ID_584 );		// added
	mapRavenSoulTaker[CX2GURaven::RSI_COMBO_ZZZX]							= GET_STRING( STR_ID_570 );
	mapRavenSoulTaker[CX2GURaven::RSI_COMBO_ZZXX]							= GET_STRING( STR_ID_601 );
	mapRavenSoulTaker[CX2GURaven::RSI_COMBO_XXX]							= GET_STRING( STR_ID_571 );
	mapRavenSoulTaker[CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8Z]				= GET_STRING( STR_ID_604 );		// added		
	mapRavenSoulTaker[CX2GURaven::RSI_SOUL_TAKER_DASH_COMBO_ZZZ]			= GET_STRING( STR_ID_579 );	// modified
	mapRavenSoulTaker[CX2GURaven::RSI_DASH_COMBO_X]							= GET_STRING( STR_ID_574 );
	mapRavenSoulTaker[CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ]					= GET_STRING( STR_ID_602 );
	mapRavenSoulTaker[CX2GURaven::RSI_DASH_JUMP_COMBO_X]					= GET_STRING( STR_ID_589 );


	map<char, wstring>& mapRavenOverTaker = m_mapUnitCommandList[ CX2Unit::UC_RAVEN_OVER_TAKER ];
	mapRavenOverTaker[CX2GURaven::RSI_COMBO_ZZZZ]							= GET_STRING( STR_ID_569 );
	mapRavenOverTaker[CX2GURaven::RSI_COMBO_ZZZX]							= GET_STRING( STR_ID_570 );
	mapRavenOverTaker[CX2GURaven::RSI_COMBO_ZZXX]							= GET_STRING( STR_ID_601 );
	mapRavenOverTaker[CX2GURaven::RSI_COMBO_XXX]							= GET_STRING( STR_ID_571 );
	mapRavenOverTaker[CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownXdownX]	= GET_STRING( STR_ID_605 );     // added
	mapRavenOverTaker[CX2GURaven::RSI_OVER_TAKER_COMBO_XXfrontX]			= GET_STRING( STR_ID_606 );		// added
	mapRavenOverTaker[CX2GURaven::RSI_DASH_COMBO_ZZ]						= GET_STRING( STR_ID_573 );
	mapRavenOverTaker[CX2GURaven::RSI_DASH_COMBO_X]							= GET_STRING( STR_ID_574 );
	mapRavenOverTaker[CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ]					= GET_STRING( STR_ID_602 );
	mapRavenOverTaker[CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX]		= GET_STRING( STR_ID_607 ); 	// added
	mapRavenOverTaker[CX2GURaven::RSI_DASH_JUMP_COMBO_X]					= GET_STRING( STR_ID_589 );



	map<char, wstring>& mapEveNasod = m_mapUnitCommandList[ CX2Unit::UC_EVE_NASOD ];
	mapEveNasod[CX2GUEve::ENSI_COMBO_ZZZZ]						= GET_STRING( STR_ID_569 );
	mapEveNasod[CX2GUEve::ENSI_COMBO_ZZZdownZ]					= GET_STRING( STR_ID_608 );
	mapEveNasod[CX2GUEve::ENSI_COMBO_ZZZX]						= GET_STRING( STR_ID_570 );
	mapEveNasod[CX2GUEve::ENSI_COMBO_XXX]						= GET_STRING( STR_ID_571 );
	mapEveNasod[CX2GUEve::ENSI_COMBO_XXfrontX]					= GET_STRING( STR_ID_586 );
	mapEveNasod[CX2GUEve::ENSI_COMBO_XXdownX]					= GET_STRING( STR_ID_606 );
	mapEveNasod[CX2GUEve::ENSI_DASH_COMBO_Z]					= GET_STRING( STR_ID_610 );
	mapEveNasod[CX2GUEve::ENSI_DASH_COMBO_X]					= GET_STRING( STR_ID_574 );
	mapEveNasod[CX2GUEve::ENSI_DASH_JUMP_COMBO_ZZZ]				= GET_STRING( STR_ID_611 );
	mapEveNasod[CX2GUEve::ENSI_DASH_JUMP_COMBO_X]				= GET_STRING( STR_ID_589 );


	map<char, wstring>& mapEveNasodEG = m_mapUnitCommandList[ CX2Unit::UC_EVE_EXOTIC_GEAR ];
	mapEveNasodEG[CX2GUEve::ENSI_COMBO_ZZZZ]					= GET_STRING( STR_ID_569 );
	mapEveNasodEG[CX2GUEve::ENSI_COMBO_ZZZdownZZ]				= GET_STRING( STR_ID_612 );     // modified
	mapEveNasodEG[CX2GUEve::ENSI_COMBO_ZZZX]					= GET_STRING( STR_ID_570 );
	mapEveNasodEG[CX2GUEve::ENSI_COMBO_XXX]						= GET_STRING( STR_ID_571 );
	mapEveNasodEG[CX2GUEve::ENSI_COMBO_XXfrontX]				= GET_STRING( STR_ID_586 );
	mapEveNasodEG[CX2GUEve::ENSI_COMBO_XXdownX]					= GET_STRING( STR_ID_606 );
	mapEveNasodEG[CX2GUEve::ENSI_DASH_COMBO_ZZZ]				= GET_STRING( STR_ID_579 ); 	// modified
	mapEveNasodEG[CX2GUEve::ENSI_DASH_COMBO_XZ]					= GET_STRING( STR_ID_613 );		// modified
	mapEveNasodEG[CX2GUEve::ENSI_DASH_JUMP_COMBO_ZZZ]			= GET_STRING( STR_ID_611 );
	mapEveNasodEG[CX2GUEve::ENSI_DASH_JUMP_COMBO_X]				= GET_STRING( STR_ID_589 );

	map<char, wstring>& mapEveNasodAT = m_mapUnitCommandList[ CX2Unit::UC_EVE_ARCHITECTURE ];
	mapEveNasodAT[CX2GUEve::ENSI_COMBO_ZZZZ]					= GET_STRING( STR_ID_569 );
	mapEveNasodAT[CX2GUEve::ENSI_COMBO_ZZZdownZZ]				= GET_STRING( STR_ID_612 ); 	// modified
	mapEveNasodAT[CX2GUEve::ENSI_COMBO_ZZZX]					= GET_STRING( STR_ID_570 );
	mapEveNasodAT[CX2GUEve::ENSI_COMBO_XXX]						= GET_STRING( STR_ID_571 );
	mapEveNasodAT[CX2GUEve::ENSI_COMBO_XXZ4]					= GET_STRING( STR_ID_572 );		// added
	mapEveNasodAT[CX2GUEve::ENSI_COMBO_XXfrontX]				= GET_STRING( STR_ID_609 );
	mapEveNasodAT[CX2GUEve::ENSI_COMBO_XXdownX]					= GET_STRING( STR_ID_585 );
	mapEveNasodAT[CX2GUEve::ENSI_DASH_COMBO_Z]					= GET_STRING( STR_ID_610 );
	mapEveNasodAT[CX2GUEve::ENSI_DASH_COMBO_X]					= GET_STRING( STR_ID_574 );
	mapEveNasodAT[CX2GUEve::ENSI_DASH_JUMP_COMBO_ZZZ]			= GET_STRING( STR_ID_611 );
	mapEveNasodAT[CX2GUEve::ENSI_DASH_JUMP_COMBO_XZ]			= GET_STRING( STR_ID_582 );	    // modified





}






//}}AFX

#endif OLD_COMBO_LIST_UI

void CX2StateDungeonGame::DungeonMapUI::CreatStageGraph( CX2Dungeon* pDungeon )
{
	if( NULL == pDungeon )
		return;

	map< std::pair<int, int>, SubStageNode>::iterator it;
	m_mapStageGraph.clear();

	vector<CX2DungeonStage::StageData*>& vecStageList = pDungeon->GetStageDataList();
	for( UINT i=0; i<vecStageList.size(); i++ )
	{
		CX2DungeonStage::StageData* pStageData = vecStageList[i];

		for( UINT j=0; j<pStageData->m_SubStageDataList.size(); j++ )
		{
			CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[j];

			it = m_mapStageGraph.find( std::make_pair(i,j) );
			if( it == m_mapStageGraph.end() )
			{
				SubStageNode subStageNode;
				m_mapStageGraph[ std::make_pair(i,j) ] = subStageNode;
			}

			it = m_mapStageGraph.find( std::make_pair(i,j) );
			SubStageNode& nowSubStageNode = it->second;


			for( UINT k=0; k<pSubStageData->m_vecClearCondData.size(); k++ )
			{
				CX2DungeonSubStage::ClearCondtionData& clearCond = pSubStageData->m_vecClearCondData[k];
				for( UINT m=0; m<clearCond.m_vecNextStage.size(); m++ )
				{
					CX2DungeonSubStage::NextStageData& nextStageData = clearCond.m_vecNextStage[m];
					switch( nextStageData.m_eClearType )
					{
					case CX2DungeonSubStage::CT_GAME:
						{
						} break;

					case CX2DungeonSubStage::CT_STAGE:
					case CX2DungeonSubStage::CT_STAGE_FORCE:
					case CX2DungeonSubStage::CT_SUB_STAGE:
					case CX2DungeonSubStage::CT_STAGE_SECRET:
						{
							nowSubStageNode.m_vecNextSubStageIndex.push_back( std::make_pair( nextStageData.m_iStageIndex, nextStageData.m_iSubStageIndex ) );

							it = m_mapStageGraph.find( std::make_pair( nextStageData.m_iStageIndex, nextStageData.m_iSubStageIndex ) );
							if( it == m_mapStageGraph.end() )
							{
								SubStageNode nextSubStageNode;
								m_mapStageGraph[ std::make_pair(nextStageData.m_iStageIndex, nextStageData.m_iSubStageIndex) ] = nextSubStageNode;
							}
						} break;
					}
				}
			}
		}
	}
}

void CX2StateDungeonGame::DungeonMapUI::CreateDungeonMapUI()
{
	if( NULL == m_pDLGDungeonMap )
		return;

	const map< std::pair<int,int>, CX2Dungeon::DungeonMapData >& mapDungeonMapData = m_pDungeon->GetMapDungeonMapData();
	map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::const_iterator it;
	map< std::pair<int, int>, SubStageNode>::iterator it_line;


	//////////////////////////////////////////////////////////////////////////
	// line
	CKTDGUIStatic* pStatic_Line = (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Static_Stage_Lines" );
	if( NULL == pStatic_Line )
		return;

	int iLinePictureInex = 0;
	for( it_line = m_mapStageGraph.begin(); it_line != m_mapStageGraph.end(); it_line++ )
	{
		const int iStageIndex		= it_line->first.first;
		const int iSubStageIndex	= it_line->first.second;
		const SubStageNode& stageGraphNode = it_line->second;
		int iNextStageIndex = 0;
		int iNextSubStageIndex = 0;


		for( UINT i=0; i< stageGraphNode.m_vecNextSubStageIndex.size(); i++ )
		{
			iNextStageIndex = stageGraphNode.m_vecNextSubStageIndex[i].first;
			iNextSubStageIndex = stageGraphNode.m_vecNextSubStageIndex[i].second;


			it = mapDungeonMapData.find( std::make_pair( iStageIndex, iSubStageIndex ) );
			if( it == mapDungeonMapData.end() )
				continue;
			const CX2Dungeon::DungeonMapData& nowDungeonMapData = it->second;
			it = mapDungeonMapData.find( std::make_pair( iNextStageIndex, iNextSubStageIndex ) );
			if( it == mapDungeonMapData.end() )
				continue;
			const CX2Dungeon::DungeonMapData& nextDungeonMapData = it->second;

			CKTDGUIControl::CPictureData* pPicture = NULL;
			if( iLinePictureInex > pStatic_Line->GetPictureNum()-1 )
			{
				pPicture = new CKTDGUIControl::CPictureData();
				pPicture->SetTex( L"DLG_Common_Texture01.tga", L"White" );
				pStatic_Line->AddPicture( pPicture );
			}
			else
			{
				pPicture= pStatic_Line->GetPicture( iLinePictureInex );
			}
			iLinePictureInex++;


			D3DXVECTOR2 vDir = nextDungeonMapData.m_vPos - nowDungeonMapData.m_vPos;
			const float MAGIC_LINE_WIDTH = 2.f;

			if( vDir.x == 0.f )
			{
				CKTDGUIControl::UIPointData* pNewPoint = new CKTDGUIControl::UIPointData();
				pNewPoint->bUseTextureSize = false;
				pNewPoint->fChangeTime = 0.0f;
				pNewPoint->color				= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				pNewPoint->leftTopPoint			= D3DXVECTOR2( nowDungeonMapData.m_vPos.x - MAGIC_LINE_WIDTH, min( nowDungeonMapData.m_vPos.y, nextDungeonMapData.m_vPos.y ) );
				pNewPoint->rightTopPoint		= D3DXVECTOR2( nowDungeonMapData.m_vPos.x + MAGIC_LINE_WIDTH, min( nowDungeonMapData.m_vPos.y, nextDungeonMapData.m_vPos.y ) );
				pNewPoint->leftBottomPoint		= D3DXVECTOR2( nowDungeonMapData.m_vPos.x - MAGIC_LINE_WIDTH, max( nowDungeonMapData.m_vPos.y, nextDungeonMapData.m_vPos.y ) );
				pNewPoint->rightBottomPoint		= D3DXVECTOR2( nowDungeonMapData.m_vPos.x + MAGIC_LINE_WIDTH, max( nowDungeonMapData.m_vPos.y, nextDungeonMapData.m_vPos.y ) );

				pPicture->SetPoint( pNewPoint );
			}
			else if( vDir.y == 0.f )
			{
				CKTDGUIControl::UIPointData* pNewPoint = new CKTDGUIControl::UIPointData();
				pNewPoint->bUseTextureSize = false;
				pNewPoint->fChangeTime = 0.0f;
				pNewPoint->color				= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				pNewPoint->leftTopPoint			= D3DXVECTOR2( min( nowDungeonMapData.m_vPos.x, nextDungeonMapData.m_vPos.x), nowDungeonMapData.m_vPos.y - MAGIC_LINE_WIDTH );
				pNewPoint->rightTopPoint		= D3DXVECTOR2( max( nowDungeonMapData.m_vPos.x, nextDungeonMapData.m_vPos.x), nowDungeonMapData.m_vPos.y - MAGIC_LINE_WIDTH );
				pNewPoint->leftBottomPoint		= D3DXVECTOR2( min( nowDungeonMapData.m_vPos.x, nextDungeonMapData.m_vPos.x), nowDungeonMapData.m_vPos.y + MAGIC_LINE_WIDTH );
				pNewPoint->rightBottomPoint		= D3DXVECTOR2( max( nowDungeonMapData.m_vPos.x, nextDungeonMapData.m_vPos.x), nowDungeonMapData.m_vPos.y + MAGIC_LINE_WIDTH );

				pPicture->SetPoint( pNewPoint );
			}
			else
			{									
				D3DXVECTOR2 vXAxis(1,0);
				float fLength = D3DXVec2Length( &vDir );
				D3DXVec2Normalize( &vDir, &vDir );
				float fDot = D3DXVec2Dot( &vDir, &vXAxis );

				CKTDGUIControl::UIPointData* pNewPoint = new CKTDGUIControl::UIPointData();
				pNewPoint->bUseTextureSize = false;
				pNewPoint->fChangeTime = 0.0f;
				pNewPoint->color				= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				pNewPoint->leftTopPoint			= D3DXVECTOR2( nowDungeonMapData.m_vPos.x, nowDungeonMapData.m_vPos.y - MAGIC_LINE_WIDTH );
				pNewPoint->rightTopPoint		= D3DXVECTOR2( nowDungeonMapData.m_vPos.x + fLength, nowDungeonMapData.m_vPos.y - MAGIC_LINE_WIDTH );
				pNewPoint->leftBottomPoint		= D3DXVECTOR2( nowDungeonMapData.m_vPos.x, nowDungeonMapData.m_vPos.y + MAGIC_LINE_WIDTH );
				pNewPoint->rightBottomPoint		= D3DXVECTOR2( nowDungeonMapData.m_vPos.x + fLength, nowDungeonMapData.m_vPos.y + MAGIC_LINE_WIDTH );

				pPicture->SetPoint( pNewPoint );
				if( vDir.y > 0.f )
				{
					pPicture->RotatePoint( nowDungeonMapData.m_vPos, D3DXToDegree( acos(fDot) ) );
				}
				else
				{
					pPicture->RotatePoint( nowDungeonMapData.m_vPos, -D3DXToDegree( acos(fDot) ) );
				}
			}
			pPicture->SetColor( D3DXCOLOR(0.29f,0.29f,0.29f,1) );
		}
	}

#ifdef FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON			/// 던전 진행 맵에서 던전 클리어 아이콘이 케릭터 얼굴 아이콘 위에 생성되는 문제 수정
	//////////////////////////////////////////////////////////////////////////
	// user icon

	/// 기존에 스테이지 아이콘과 유저 아이콘을 같은 스테틱으로 사용하던 것을 분리시킴
	/// 스테이지 아이콘 -> 클리어 아이콘 -> 유저 아이콘	순서로 그리도록 변경
	CKTDGUIStatic* pStatic_UserIcon = (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Static_User_Icon" );
	if( NULL == pStatic_UserIcon )
		return;

	if( NULL == pStatic_UserIcon->GetPicture( 0 ) )		/// 유저 아이콘은 하나만 생성한다.
	{
		CKTDGUIControl::CPictureData* pUserPicture = new CKTDGUIControl::CPictureData();

		if( NULL == pUserPicture )
			return;

		// note!! 캐릭터 추가되면 수정해야 됨
		switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
		{
		default:
		case CX2Unit::UT_ELSWORD:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"ELSWORD" );
			} break;
		case CX2Unit::UT_ARME:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"AISHA" );
			} break;
		case CX2Unit::UT_LIRE:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"RENA" );
			} break;
		case CX2Unit::UT_RAVEN:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"RAVEN" );
			} break;

		case CX2Unit::UT_EVE:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"EVE" );
			} break;

			//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
	#ifdef	NEW_CHARACTER_CHUNG
		case CX2Unit::UT_CHUNG:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"CHUNG" );
			} break;
	#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

	#ifdef ARA_CHARACTER_BASE
		case CX2Unit::UT_ARA:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"ARA" );
			} break;
	#endif
	#ifdef NEW_CHARACTER_EL
		case CX2Unit::UT_ELESIS:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"ELESIS" );
			} break;
	#endif // NEW_CHARACTER_EL
	#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
		case CX2Unit::UT_ADD:
			{
				pUserPicture->SetTex( L"DLG_UI_Common_Texture80_NEW.tga", L"ADD_MapSD" );
			} break;
	#endif //SERV_9TH_NEW_CHARACTER
		}
		pUserPicture->SetPoint();

		pStatic_UserIcon->AddPicture( pUserPicture );
	}


	//////////////////////////////////////////////////////////////////////////
	// stage icon
	CKTDGUIStatic* pStatic_Stage = (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Static_Stage_Pictures" );
	if( NULL == pStatic_Stage )
		return;

	while( pStatic_Stage->GetPictureNum() < (int)mapDungeonMapData.size() )
	{
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();

		if( NULL == pPicture )
			return;

		pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"FRONT_ROAD" );
		pPicture->SetPoint();

		pStatic_Stage->AddPicture( pPicture );
	}
#else  FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON
	//////////////////////////////////////////////////////////////////////////
	// stage 
	CKTDGUIStatic* pStatic_Stage = (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Static_Stage_Pictures" );
	if( NULL == pStatic_Stage )
		return;

	while( pStatic_Stage->GetPictureNum() < (int)mapDungeonMapData.size()+1 )
	{
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();

		if( pStatic_Stage->GetPictureNum() == (int)mapDungeonMapData.size() )
		{
			// note!! 캐릭터 추가되면 수정해야 됨
			switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
			{
			default:
			case CX2Unit::UT_ELSWORD:
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"ELSWORD" );
				} break;
			case CX2Unit::UT_ARME:
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"AISHA" );
				} break;
			case CX2Unit::UT_LIRE:
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"RENA" );
				} break;
			case CX2Unit::UT_RAVEN:
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"RAVEN" );
				} break;

			case CX2Unit::UT_EVE:
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"EVE" );
				} break;

				//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
	#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UT_CHUNG:
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"CHUNG" );
				} break;
	#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

	#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UT_ARA:
				{
					pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"ARA" );
				} break;
	#endif
	#ifdef NEW_CHARACTER_EL
		case CX2Unit::UT_ELESIS:
			{
				pUserPicture->SetTex( L"DLG_Common_Emblem00.tga", L"ELESIS" );
			} break;
	#endif // NEW_CHARACTER_EL
			}
			pPicture->SetPoint();
		}
		else
		{
			pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"FRONT_ROAD" );
			pPicture->SetPoint();
		}

		pStatic_Stage->AddPicture( pPicture );
	}
#endif FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON

	// stage is random
	CKTDGUIStatic* pStatic_Stage_Random = (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Static_Stage_Random_Pictures" );
	if( NULL == pStatic_Stage_Random )
		return;

	while( pStatic_Stage_Random->GetPictureNum() < (int)mapDungeonMapData.size() )
	{
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
		pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"Compass" );
		pPicture->SetPoint();
		pStatic_Stage_Random->AddPicture( pPicture );
	}


	int iPictureIndex = 0;
	for( it = mapDungeonMapData.begin(); it != mapDungeonMapData.end(); it++, iPictureIndex++ )
	{
		CKTDGUIControl::CPictureData* pPicture = pStatic_Stage->GetPicture( iPictureIndex );
		if( pPicture == NULL )
			continue;


		CKTDGUIControl::CPictureData* pPicture_Random = pStatic_Stage_Random->GetPicture( iPictureIndex );
		if( pPicture_Random == NULL )
			continue;


		const int iStageIndex = it->first.first;
		const int iSubStageIndex = it->first.second;
		const CX2Dungeon::DungeonMapData& dungeonMapData = it->second;

		if( dungeonMapData.m_bIsBossStage == true )
		{
			pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"Boss_Mark" );
			pPicture_Random->SetShow( false );
		}
#ifdef NEW_HENIR_TEST
		else if( dungeonMapData.m_bRestRoomState == true )
		{
			pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"map_rest" );
			pPicture_Random->SetShow( false );
		}
#endif NEW_HENIR_TEST
		else
		{
#ifdef NEW_HENIR_TEST
			if( dungeonMapData.m_bOpen == true || dungeonMapData.m_bAutoOpen == true )
#else
			if( dungeonMapData.m_bOpen == true )
#endif NEW_HENIR_TEST
			{
				pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"Back_Road" );

			}
			else
			{
				pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"FRONT_ROAD" );
			}

			if( iStageIndex == 0 && iSubStageIndex == 0 )
			{
				pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"Start" );
			}


			// stage random mark
			if( dungeonMapData.m_bIsNextStageRandom == true )
			{
				pPicture_Random->SetShow( true );
				D3DXVECTOR2 vPos = dungeonMapData.m_vPos - pPicture_Random->GetSize() * 0.5f;
				vPos.x += pPicture->GetSize().x * 0.4f;
				pPicture_Random->SetPos( vPos );
			}
			else
			{
				pPicture_Random->SetShow( false );
			}
		}

		D3DXVECTOR2 vPos = dungeonMapData.m_vPos - pPicture->GetSize() * 0.5f;
		pPicture->SetPos( vPos );

		if( iStageIndex == m_pDungeon->GetNowStageIndex() &&
			iSubStageIndex == m_pDungeon->GetNowStage()->GetNowSubStageIndex() )
		{
#ifdef FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON		/// 스테이지 아이콘에서 유저 아이콘을 분리함 -> 유저 아이콘만 따로 불러옴
			CKTDGUIControl::CPictureData* pPicture_Me = pStatic_UserIcon->GetPicture( 0 );
#else  FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON
			CKTDGUIControl::CPictureData* pPicture_Me = pStatic_Stage->GetPicture( pStatic_Stage->GetPictureNum()-1 );
#endif FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON
			if( NULL != pPicture_Me )
			{
				vPos = dungeonMapData.m_vPos - pPicture_Me->GetSize() * 0.5f;
				vPos.y -= 19.f;
				pPicture_Me->SetPos( vPos );
				pPicture_Me->Vibrate( 9999.f, 5.f, D3DXVECTOR2(0, -20) ); 
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// stage clear mark
	CKTDGUIStatic* pStatic_Stage_Clear= (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Static_Stage_Clear_Pictures" );
	if( NULL == pStatic_Stage_Clear )
		return;

	while( pStatic_Stage_Clear->GetPictureNum() < (int)mapDungeonMapData.size() )
	{
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
		pPicture->SetTex( L"DLG_Common_Emblem00.tga", L"CLEAR" );
		pPicture->SetPoint();
		pStatic_Stage_Clear->AddPicture( pPicture );
	}



	const int iNowStageIndex = m_pDungeon->GetNowStageIndex();
	const int iNowSubStageIndex = m_pDungeon->GetNowStage()->GetNowSubStageIndex();


	iPictureIndex = 0;
	for( it = mapDungeonMapData.begin(); it != mapDungeonMapData.end(); it++, iPictureIndex++ )
	{
		CKTDGUIControl::CPictureData* pPicture = pStatic_Stage_Clear->GetPicture( iPictureIndex );
		if( pPicture == NULL )
			continue;

		const int iStageIndex = it->first.first;
		const int iSubStageIndex = it->first.second;
		const CX2Dungeon::DungeonMapData& dungeonMapData = it->second;


		if( dungeonMapData.m_bOpen == true )
		{
			if( iStageIndex != iNowStageIndex || iSubStageIndex != iNowSubStageIndex )
			{
				pPicture->SetShow( true );
			}
			else
			{
				pPicture->SetShow( false );
			}
		}
		else
		{
			pPicture->SetShow( false );
		}

		D3DXVECTOR2 vPos = dungeonMapData.m_vPos - pPicture->GetSize() * 0.5f;
		pPicture->SetPos( vPos );
	}


	ResizeBackground();
}


void CX2StateDungeonGame::DungeonMapUI::ResizeBackground()
{
	if( NULL == m_pDLGDungeonMap )
		return;

	CKTDGUIStatic* pStatic_Stage = (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Static_Stage_Pictures" );
	if( NULL == pStatic_Stage )
		return;

	CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGDungeonMap->GetControl( L"Dungeon_Map_BG" );
	if( NULL == pStatic_BG )
		return;


	D3DXVECTOR2 vBGLeftTopPos(10000, 10000);
	D3DXVECTOR2 vBGRightBottomPos(0, 0);

#ifdef FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON		/// 스테이지 아이콘에서 유저 아이콘을 분리했기 때문에, 인덱스 모두 검사
	for( int i=0; i<pStatic_Stage->GetPictureNum(); i++ )
#else  FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON
	for( int i=0; i<pStatic_Stage->GetPictureNum()-1; i++ )
#endif FIX_CHARACTER_ICON_OVERLAB_BY_CLEAR_ICON
	{
		CKTDGUIStatic::CPictureData* pPicture = pStatic_Stage->GetPicture(i);
		if( NULL != pPicture )
		{
			D3DXVECTOR2 vLeftTopPos = pPicture->GetPos();
			D3DXVECTOR2 vRightBottomPos = vLeftTopPos + pPicture->GetSize();

			if( vBGLeftTopPos.x > vLeftTopPos.x )
				vBGLeftTopPos.x = vLeftTopPos.x;

			if( vBGLeftTopPos.y > vLeftTopPos.y )
				vBGLeftTopPos.y = vLeftTopPos.y;

			if( vBGRightBottomPos.x < vRightBottomPos.x )
				vBGRightBottomPos.x = vRightBottomPos.x;
			if( vBGRightBottomPos.y < vRightBottomPos.y )
				vBGRightBottomPos.y = vRightBottomPos.y;
		}
	}

	vBGLeftTopPos		-= D3DXVECTOR2( 20.f, 20.f );
	vBGRightBottomPos	+= D3DXVECTOR2( 20.f, 20.f );

	D3DXVECTOR2 vPos(0, 0);
	D3DXVECTOR2 vSize(0, 0);

	// left top
	vPos = vBGLeftTopPos;
	vPos -= pStatic_BG->GetPicture(0)->GetSize();
	pStatic_BG->GetPicture(0)->SetPos( vPos );

	// top center
	vPos = vBGLeftTopPos;
	vPos.y -= pStatic_BG->GetPicture(1)->GetSize().y;
	pStatic_BG->GetPicture(1)->SetPos( vPos );
	pStatic_BG->GetPicture(1)->SetSizeX( vBGRightBottomPos.x - vBGLeftTopPos.x );

	// right top
	vPos = vBGLeftTopPos;
	vPos.y -= pStatic_BG->GetPicture(2)->GetSize().y;
	vPos.x = vBGRightBottomPos.x;
	pStatic_BG->GetPicture(2)->SetPos( vPos );

	// left center
	vPos = vBGLeftTopPos;
	vPos.x -= pStatic_BG->GetPicture(3)->GetSize().x;
	vSize = pStatic_BG->GetPicture(3)->GetSize();
	vSize.y = vBGRightBottomPos.y - vBGLeftTopPos.y;
	pStatic_BG->GetPicture(3)->SetPos( vPos );
	pStatic_BG->GetPicture(3)->SetSize( vSize );

	// left bottom
	vPos = vBGRightBottomPos;
	vPos.x = vBGLeftTopPos.x - pStatic_BG->GetPicture(4)->GetSize().x;
	pStatic_BG->GetPicture(4)->SetPos( vPos );

	// center bottom
	vPos = vBGRightBottomPos;
	vPos.x = vBGLeftTopPos.x;
	pStatic_BG->GetPicture(5)->SetPos( vPos );
	pStatic_BG->GetPicture(5)->SetSizeX( vBGRightBottomPos.x - vBGLeftTopPos.x );


	// right bottom
	vPos = vBGRightBottomPos;
	pStatic_BG->GetPicture(6)->SetPos( vPos );

	// right center
	vPos = vBGRightBottomPos;
	vPos.y = vBGLeftTopPos.y;
	vSize = pStatic_BG->GetPicture(7)->GetSize();
	vSize.y = vBGRightBottomPos.y - vBGLeftTopPos.y;
	pStatic_BG->GetPicture(7)->SetPos( vPos );
	pStatic_BG->GetPicture(7)->SetSize( vSize );


	// center
	pStatic_BG->GetPicture(8)->SetPos( vBGLeftTopPos );
	pStatic_BG->GetPicture(8)->SetSize( vBGRightBottomPos - vBGLeftTopPos );

}



void CX2StateDungeonGame::DungeonMapUI::OnFirstSubStage()
{
	if( true == m_bShow )
		return;

	if( NULL == m_pDLGDungeonMap )
		return;

	D3DXVECTOR2 vPos = m_vPos;
	vPos.x = 3000.f;

	m_pDLGDungeonMap->SetShowEnable( true, true );
	m_pDLGDungeonMap->Move( vPos, D3DXCOLOR(0,0,0,0), 0.01f, true );
	m_pDLGDungeonMap->Move( m_vPos, D3DXCOLOR(1,1,1,1), 1.f, true );
	m_pDLGDungeonMap->Move( m_vPos, D3DXCOLOR(1,1,1,1), 3.f );
	vPos.x = -2000.f;
	m_pDLGDungeonMap->Move( vPos, D3DXCOLOR(0,0,0,0), 1.f );

}




//////////////////////////////////////////////////////////////////////////
CX2StateDungeonGame::DungeonEndingEvent::DungeonEndingEvent()
{
	m_pDLGSpeechBalloon = NULL;
	m_pDLGCharacterPicture = NULL;
	m_bStarted = false;
	m_bEnd = false;
	m_fElapsedTime = 0.f;
	m_fSpeechElapsedTime = 0.f;

	m_iIndexSpeech1 = 0;
	m_iIndexSpeech2 = 0;

	m_bEndChapter1 = false;
	m_bStartChapter2 = false;

	m_fElapsedTime_ShowClearEffect = -1.f;
	m_fElapsedTime_WinEffect = -1.f;
}

CX2StateDungeonGame::DungeonEndingEvent::~DungeonEndingEvent()
{
	SAFE_DELETE_DIALOG( m_pDLGSpeechBalloon );
	SAFE_DELETE_DIALOG( m_pDLGCharacterPicture );
}

void CX2StateDungeonGame::DungeonEndingEvent::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_fElapsedTime_ShowClearEffect >= 0.f )
	{
		m_fElapsedTime_ShowClearEffect += fElapsedTime;
		if( m_fElapsedTime_ShowClearEffect > 2.9f )
		{
			m_fElapsedTime_ShowClearEffect = -1.f;
			Start();
		}
	}

	if( true == m_bEndChapter1 && m_fElapsedTime_WinEffect >= 0.f )
	{
		m_fElapsedTime_WinEffect += fElapsedTime;
		if( m_fElapsedTime_WinEffect > 1.2f )
		{
			m_fElapsedTime_WinEffect = -1.f;
			StartChapter2();
		}
	}

	if( true == m_bStarted )
	{
		m_fElapsedTime += fElapsedTime;

		if( m_fElapsedTime > 3.f && m_pDLGCharacterPicture == NULL )
		{
			m_pDLGCharacterPicture = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Dungeon_Ending_Event_Character.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCharacterPicture );

			m_pDLGCharacterPicture->Move( D3DXVECTOR2(1024-361, 0), D3DXCOLOR(1, 1, 1, 0), 0.01f, true );
			m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 1), 0.25f, false );


			
			CKTDGUIStatic* pStatic_Character = (CKTDGUIStatic*) m_pDLGCharacterPicture->GetControl( L"Character_Picture" );
			if( NULL != pStatic_Character && NULL != pStatic_Character->GetPicture(0) )
			{
				//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
		#ifdef	NEW_CHARACTER_CHUNG
				SetDungeonClearTexture( pStatic_Character );
		#else	NEW_CHARACTER_CHUNG
				wstring fileName;
				wstring pieceName;
				if( true == CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear ) )
				{
					if( true == pieceName.empty() )
					{
						string strFileName;
						ConvertWCHARToChar( strFileName, fileName.c_str() );
						pStatic_Character->GetPicture(0)->SetTex_LUA2( strFileName.c_str() );
					}
					else
					{
						pStatic_Character->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
					}
				}
				else
				{
					ASSERT( !"NO" );
				}
		#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
			}
		}

		if( m_fElapsedTime > 3.25f && m_pDLGSpeechBalloon == NULL )
		{
			m_pDLGSpeechBalloon = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Dungeon_Ending_Event_Speech.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSpeechBalloon );



			CKTDGUIStatic* pStatic_Character_Name = (CKTDGUIStatic*) m_pDLGSpeechBalloon->GetControl( L"Static_Ending_Speech" );
			if( NULL != pStatic_Character_Name && NULL != pStatic_Character_Name->GetString(0) )
			{
				switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
				{
				case CX2Unit::UT_ELSWORD:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_614 );
					} break;

				case CX2Unit::UT_ARME:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_615 );
					} break;

				case CX2Unit::UT_LIRE:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_616 );
					} break;

				case CX2Unit::UT_RAVEN:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_617 );
					} break;

				case CX2Unit::UT_EVE:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_618 );
					} break;

			//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
		#ifdef	NEW_CHARACTER_CHUNG
				case CX2Unit::UT_CHUNG:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_9739 );
					} break;
		#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

		#ifdef ARA_CHARACTER_BASE
				case CX2Unit::UT_ARA:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_21181 );
					} break;
		#endif
		#ifdef NEW_CHARACTER_EL
				case CX2Unit::UT_ELESIS:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_25873 );
					} break;
		#endif // NEW_CHARACTER_EL
		#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UT_ADD:
					{
						pStatic_Character_Name->GetString(0)->msg = GET_STRING( STR_ID_29422 );
					} break;
		#endif //SERV_9TH_NEW_CHARACTER

				}
			}

			m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.01f, true );
			m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 1), 0.25f, false );

			m_iIndexSpeech1 = 0;
			m_iIndexSpeech2 = 0;

			m_fSpeechElapsedTime = 100.f;

			// 던전클리어 대사 칠때 메신져 숨기기
			if(g_pData != NULL && g_pData->GetMessenger() != NULL)
			{
				g_pData->GetMessenger()->SetHideWindow(true);
			}
		}


		if( NULL != m_pDLGCharacterPicture && NULL != m_pDLGSpeechBalloon )
		{
			if( m_fElapsedTime > 3.5f && m_bEndChapter1 == false )
			{
				const float MAGIC_SPEECH_TIME = 2.5f;
				m_fSpeechElapsedTime += fElapsedTime;
				if( m_fSpeechElapsedTime > MAGIC_SPEECH_TIME )
				{
					if( m_iIndexSpeech1 < (int) m_vecSpeech1.size() )
					{
						CKTDGUIStatic* pStatic_Speech = (CKTDGUIStatic*) m_pDLGSpeechBalloon->GetControl( L"Static_Ending_Speech" );
#ifdef CLIENT_GLOBAL_LINEBREAK
						wstring str = m_vecSpeech1[ m_iIndexSpeech1 ];

						CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(1)->fontIndex );
						if( NULL == pFont )
							return; 

						D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 840, 512 );
						
						const int CHAT_LINE_WIDTH = (int)vTemp.x;;
					
						int addRow = CWordLineHandler::LineBreakInX2Main(str, pFont, CHAT_LINE_WIDTH, L"", true, false);
						if( -1 == addRow)
							return;
						
						pStatic_Speech->GetString(1)->msg = str;

#else //#ifdef CLIENT_GLOBAL_LINEBREAK
						pStatic_Speech->GetString(1)->msg = m_vecSpeech1[ m_iIndexSpeech1 ];
#endif //CLIENT_GLOBAL_LINEBREAK
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"Alarm_DialogueBox.ogg", false, false ); 
					}
					else
					{
						m_bEndChapter1 = true;

						m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
						m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
					}

					m_fSpeechElapsedTime = 0.f;
					m_iIndexSpeech1++;
				}

				if( true == m_bEndChapter1 )
				{
					m_fSpeechElapsedTime = 100.f;
				}
			}

			if( m_fElapsedTime > 3.5f && m_bStartChapter2 == true )
			{
				const float MAGIC_SPEECH_TIME = 2.5f;
				m_fSpeechElapsedTime += fElapsedTime;
				if( m_fSpeechElapsedTime > MAGIC_SPEECH_TIME )
				{
					if( m_iIndexSpeech2 < (int) m_vecSpeech2.size() )
					{
						m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 1), 0.25f, true );
						m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 1), 0.25f, true );

						CKTDGUIStatic* pStatic_Speech = (CKTDGUIStatic*) m_pDLGSpeechBalloon->GetControl( L"Static_Ending_Speech" );
#ifdef CLIENT_GLOBAL_LINEBREAK
						wstring str = m_vecSpeech2[ m_iIndexSpeech2 ];

						CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(1)->fontIndex );
						if( NULL == pFont )
							return; 

						const int CHAT_LINE_WIDTH = 840;
					
						int addRow = CWordLineHandler::LineBreakInX2Main(str, pFont, CHAT_LINE_WIDTH, L"", true, false);
						if( -1 == addRow)
							return;
						
						pStatic_Speech->GetString(1)->msg = str;
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
						pStatic_Speech->GetString(1)->msg = m_vecSpeech2[ m_iIndexSpeech2 ];
#endif //CLIENT_GLOBAL_LINEBREAK
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"Alarm_DialogueBox.ogg", false, false ); 
					}
					else
					{
						m_bStartChapter2 = false;
						m_bEnd = true;
					}

					m_fSpeechElapsedTime = 0.f;
					m_iIndexSpeech2++;
				}
			}

			if( true == m_bEnd )
			{
				m_bStarted = false;
				m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
				m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
			}
		}
	}
}




#ifdef HENIR_TEST
//////////////////////////////////////////////////////////////////////////
CX2StateDungeonGame::SecretStageEnteringEvent::SecretStageEnteringEvent()
{
	m_pDLGSpeechBalloon = NULL;
	m_pDLGCharacterPicture = NULL;
	m_bStarted = false;
	m_bEnd = false;
	m_fElapsedTime = 0.f;
	m_fSpeechElapsedTime = 0.f;

	m_iIndexSpeech1 = 0;

	m_fElapsedTime_ShowClearEffect = -1.f;
	m_eBossNPCID = CX2UnitManager::NUI_NONE;

	m_bCreatedChallengeParticle = false;
}

CX2StateDungeonGame::SecretStageEnteringEvent::~SecretStageEnteringEvent()
{
	SAFE_DELETE_DIALOG( m_pDLGSpeechBalloon );
	SAFE_DELETE_DIALOG( m_pDLGCharacterPicture );
}

void CX2StateDungeonGame::SecretStageEnteringEvent::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_fElapsedTime_ShowClearEffect >= 0.f )
	{
		m_fElapsedTime_ShowClearEffect += fElapsedTime;
		if( m_fElapsedTime_ShowClearEffect > 0.5f )
		{
			m_fElapsedTime_ShowClearEffect = -1.f;
			Start();
		}
	}

	if( true == m_bStarted )
	{
		m_fElapsedTime += fElapsedTime;


		if( m_fElapsedTime > 2.5f && false == m_bCreatedChallengeParticle )
		{
			m_bCreatedChallengeParticle = true;
			g_pX2Game->GetMajorParticle()->CreateSequence( NULL,  L"Challenge", 0,0,0, 1000, 1000, 1, 1 );
		}


		if( m_fElapsedTime > 3.f && m_pDLGCharacterPicture == NULL )
		{
			m_pDLGCharacterPicture = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Dungeon_Ending_Event_Character.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCharacterPicture );

			m_pDLGCharacterPicture->Move( D3DXVECTOR2(1024-361, 0), D3DXCOLOR(1, 1, 1, 0), 0.01f, true );
			m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 1), 0.25f, false );


			CKTDGUIStatic* pStatic_Character = (CKTDGUIStatic*) m_pDLGCharacterPicture->GetControl( L"Character_Picture" );
			if( NULL != pStatic_Character && NULL != pStatic_Character->GetPicture(0) )
			{
				switch( m_eBossNPCID )
				{
				case CX2UnitManager::NUI_PPORU_GIANT_BLACK:
					{
						const D3DXVECTOR2 vPictureSize = D3DXVECTOR2( 385, 700 );
						const D3DXVECTOR2 vPicturePos = D3DXVECTOR2( 1024, 768 ) - vPictureSize;
						pStatic_Character->GetPicture(0)->SetPos( vPicturePos );
						pStatic_Character->GetPicture(0)->SetSize( vPictureSize );
						pStatic_Character->GetPicture(0)->SetTex_LUA2( "HQ_Boss_Black_Pporu.tga" );
					} break;
				case CX2UnitManager::NUI_WALLY_7TH:
					{
						const D3DXVECTOR2 vPictureSize = D3DXVECTOR2( 512, 638 );
						const D3DXVECTOR2 vPicturePos = D3DXVECTOR2( 1024, 768 ) - vPictureSize;
						pStatic_Character->GetPicture(0)->SetPos( vPicturePos );
						pStatic_Character->GetPicture(0)->SetSize( vPictureSize );
						pStatic_Character->GetPicture(0)->SetTex_LUA2( "HQ_Boss_Wally7.tga" );
					} break;
				case CX2UnitManager::NUI_LIZARDMAN_SHAMANKING_DARK:
					{
						const D3DXVECTOR2 vPictureSize = D3DXVECTOR2( 512, 701 );
						const D3DXVECTOR2 vPicturePos = D3DXVECTOR2( 1024, 768 ) - vPictureSize;
						pStatic_Character->GetPicture(0)->SetPos( vPicturePos );
						pStatic_Character->GetPicture(0)->SetSize( vPictureSize );
						pStatic_Character->GetPicture(0)->SetTex_LUA2( "HQ_Boss_Dark_Kayak.tga" );
					} break;
				case CX2UnitManager::NUI_ELSWORD_PARASITE:
					{
						const D3DXVECTOR2 vPictureSize = D3DXVECTOR2( 512, 768 );
						const D3DXVECTOR2 vPicturePos = D3DXVECTOR2( 1024, 768 ) - vPictureSize;
						pStatic_Character->GetPicture(0)->SetPos( vPicturePos );
						pStatic_Character->GetPicture(0)->SetSize( vPictureSize );
						pStatic_Character->GetPicture(0)->SetTex_LUA2( "HQ_Boss_Alterasia_Type_H.tga" );
					} break;
				}
			}
		}

		if( m_fElapsedTime > 3.25f && m_pDLGSpeechBalloon == NULL )
		{
			m_pDLGSpeechBalloon = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Dungeon_Ending_Event_Speech.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSpeechBalloon );



			CKTDGUIStatic* pStatic_Character_Name = (CKTDGUIStatic*) m_pDLGSpeechBalloon->GetControl( L"Static_Ending_Speech" );
			if( NULL != pStatic_Character_Name && NULL != pStatic_Character_Name->GetString(0) )
			{
				const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( m_eBossNPCID );
				if( NULL != pNPCTemplet )
				{
					pStatic_Character_Name->GetString(0)->msg = pNPCTemplet->m_Name;
				}
				else
				{
					pStatic_Character_Name->GetString(0)->msg = L"";
				}
			}

			m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.01f, true );
			m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 1), 0.25f, false );

			m_iIndexSpeech1 = 0;
			m_fSpeechElapsedTime = 100.f;

			// 메신져 숨기기
			if(g_pData != NULL && g_pData->GetMessenger() != NULL)
			{
				g_pData->GetMessenger()->SetHideWindow(true);
			}
		}


		if( m_fElapsedTime > 3.5f && NULL != m_pDLGCharacterPicture && NULL != m_pDLGSpeechBalloon )
		{

			const float MAGIC_SPEECH_TIME = 2.5f;
			m_fSpeechElapsedTime += fElapsedTime;
			if( m_fSpeechElapsedTime > MAGIC_SPEECH_TIME )
			{
				if( m_iIndexSpeech1 < (int) m_vecSpeech1.size() )
				{
					CKTDGUIStatic* pStatic_Speech = (CKTDGUIStatic*) m_pDLGSpeechBalloon->GetControl( L"Static_Ending_Speech" );
					pStatic_Speech->GetString(1)->msg = m_vecSpeech1[ m_iIndexSpeech1 ];
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Alarm_DialogueBox.ogg", false, false ); 
				}
				else
				{
					m_bEnd = true;

					m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
					m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
				}

				m_fSpeechElapsedTime = 0.f;
				m_iIndexSpeech1++;
			}


			if( true == m_bEnd )
			{
				m_bStarted = false;
				

				// 비밀 스테이지에 입장할지 물어보는 팝업창을 띄운다
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WANT_TO_ENTER_SECRET_STATE,
					CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 10.f, GET_STRING( STR_ID_3745 ), 
					(CKTDXStage*) g_pMain->GetNowState(), 
					CX2StateDungeonGame::DGUCM_WANT_TO_ENTER_SECRET_STAGE_OK, CX2StateDungeonGame::DGUCM_WANT_TO_ENTER_SECRET_STAGE_CANCEL, CX2StateDungeonGame::DGUCM_WANT_TO_ENTER_SECRET_STAGE_OK );
			}
		}
	}
}

#endif HENIR_TEST


//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG

/** @function : SetDungeonClearTexture
@brief : 던전 클리어시에 나와야할 텍스처를 지정
@param : CKTDGUIStatic* pStaticClearTexture_ 출력해야할 UI의 Static 포인터
*/
void CX2StateDungeonGame::DungeonEndingEvent::SetDungeonClearTexture( CKTDGUIStatic* pStaticClearTexture_ )
{
	ASSERT( NULL != g_pX2Game );
	ASSERT( NULL != g_pX2Game->GetMyUnit() );
	
	wstring fileName;
	wstring pieceName;

	CX2GUUser* pMyUser = g_pX2Game->GetMyUnit();

	switch ( pMyUser->GetUnitClass() )
	{
	case CX2Unit::UC_CHUNG_IRON_CANNON:
		{
			// 광폭화 모드이면
			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Chung_Iron_Cannon_Rage.TGA";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		}
		break;

	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
		{
			// 광폭화 모드이면
			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Chung_CFG_Rage.TGA";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		}
		break;

	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
		{
			// 광폭화 모드이면
			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Chung_CSG_Rage.TGA";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		}
		break;
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
		{
			// 광폭화 모드이면
			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Chung_SHG_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		}
		break;
#endif
#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
		{
			// 광폭화 모드이면
			if ( pMyUser->IsFullHyperMode() )
				fileName = L"DDS_Chung_CIP_Clear_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		}
		break;
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		{
			// 광폭화 모드이면
			if ( pMyUser->IsFullHyperMode() )
				fileName = L"DDS_Chung_CDC_Clear_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		}
		break;
#endif CHUNG_SECOND_CLASS_CHANGE
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
		{
			CX2GUChung* pChungUser = static_cast<CX2GUChung*>( pMyUser );

			// 광폭화 모드이면
			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Chung_TT_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		}
		break;
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
		{
			CX2GUAra* pAraUser = static_cast<CX2GUAra*>( pMyUser );

			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Ara_Martial_Artist_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		} break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_LITTLE_HSIEN:
		{
			CX2GUAra* pAraUser = static_cast<CX2GUAra*>( pMyUser );

			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Ara_LITTLE_HSIEN_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		} break;
	case CX2Unit::UC_ARA_SAKRA_DEVANAM:
		{
			CX2GUAra* pAraUser = static_cast<CX2GUAra*>( pMyUser );

			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Ara_Sakra_Devanam_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		} break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	case CX2Unit::UC_ARA_LITTLE_DEVIL:
		{
			CX2GUAra* pAraUser = static_cast<CX2GUAra*>( pMyUser );

			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Ara_Little_Devil_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		} break;
	case CX2Unit::UC_ARA_YAMA_RAJA:
		{
			CX2GUAra* pAraUser = static_cast<CX2GUAra*>( pMyUser );

			if ( pMyUser->IsFullHyperMode() )
				fileName = L"HQ_Dungeon_Clear_Ara_Yama_Raja_Rage.tga";
			else
				CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		} break;
#endif //SERV_ARA_CHANGE_CLASS_SECOND
	default:
		CX2Data::GetCharacterImageName( fileName, pieceName, g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UnitClass, CX2Data::CIT_DungeonClear );
		break;
	}

	if( true == pieceName.empty() )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, fileName.c_str() );
		pStaticClearTexture_->GetPicture(0)->SetTex_LUA2( strFileName.c_str() );
	}
	else
	{
		pStaticClearTexture_->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
	}
}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG



#ifdef CREATE_MONSTER_LIST_TEST
void CreateMonsterListDialog::Update( bool bSortAlphabetical )
{
	if( m_pDLGCreateMonsterList == NULL )
		return;

	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGCreateMonsterList->GetControl( L"ListBox_Monster_ID" );
	if( NULL == pListBox )
		return;


	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"EnumString.lua" );
	if( Info == NULL )
		return;


	vector<wstring> vecNPCIDs;

	wstring wstrEnumString;
	ConvertCharToWCHAR( wstrEnumString, Info->pRealData, Info->size );


	std::wstring::size_type iFoundIndex = wstrEnumString.find( L"NUI_" );
	std::wstring::size_type iEndIndex = wstrEnumString.find( L"\"", iFoundIndex );

	int iCount = 0;
	while( iFoundIndex != -1 )
	{

		if( iCount%2 == 0 )
		{
			wstring wstrNUI = L'\"' + wstrEnumString.substr( iFoundIndex+4, iEndIndex-iFoundIndex-4 ) + L'\"';
			vecNPCIDs.push_back( wstrNUI );
		}
		iCount++;


		iFoundIndex = wstrEnumString.find( L"NUI_", iEndIndex );

		if( iFoundIndex == std::wstring::npos || -1 == iFoundIndex || iFoundIndex == iEndIndex )
			break;

		iEndIndex = wstrEnumString.find( L"\"", iFoundIndex );
	}

	if( true == bSortAlphabetical )
	{
		std::sort( vecNPCIDs.begin(), vecNPCIDs.end() );
	}

	pListBox->RemoveAllItems();
	for( UINT i=0; i<vecNPCIDs.size(); i++ )
	{
		pListBox->AddItem( vecNPCIDs[i].c_str(), (void*) NULL );
	}


	pListBox->SetScrollBarWidth( 26 );
	pListBox->SetScrollBarEndPos();
}

void CreateMonsterListDialog::KeyProcess( CKTDXStage* pStage )
{
	if ( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
		return; 

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_COMMA) == TRUE )
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

void CreateMonsterListDialog::OnCommand( const std::wstring& monsterName )
{
	WCHAR wszText[256] = L"";
	StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"/cm %s", monsterName.c_str() ); 

	if ( NULL != g_pMain->GetGameEdit() )
		g_pMain->GetGameEdit()->ExecCommand( wszText );
}
#endif CREATE_MONSTER_LIST_TEST


#ifdef MONSTER_STATE_LIST_TEST
// void MonsterStateListDialog::Update()
// {
// 	if( m_pDLGMonsterStateList == NULL )
// 		return;
// 
// 	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGMonsterStateList->GetControl( L"ListBox_Monster_ID" );
// 	if( NULL == pListBox )
// 		return;
// 	
// 	if ( NULL == g_pMain->GetGameEdit() )
// 		return;
// 
// 
// 	int npcID = g_pMain->GetGameEdit()->GetLastCreatedMonster();
// 	CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByType(npcID);
// 	if( NULL == pNPC )
// 		return; 
// 
// 	if( (CX2UnitManager::NPC_UNIT_ID)npcID == m_NPCID )
// 		return; 
// 	m_NPCID = (CX2UnitManager::NPC_UNIT_ID)npcID;
// 
// 	m_vecStateName.resize(0); 
// 	std::vector< std::wstring > vecAnimationName;	
// 	pNPC->EnumerateStateAndAnimationName( m_vecStateName, vecAnimationName );
// 	ASSERT( m_vecStateName.size() == vecAnimationName.size() );
// 
// 	pListBox->RemoveAllItems();
// 	for( int i=0; i<(int)vecAnimationName.size(); i++ )
// 	{
// 		vecAnimationName[i] += L"(";
// 		vecAnimationName[i] += m_vecStateName[i];
// 		vecAnimationName[i] += L")";
// 
// 		pListBox->AddItem( vecAnimationName[i].c_str(), (void*)&m_vecStateName[i] );
// 	}
// 
// 	pListBox->SetScrollBarWidth( 26 );
// 	pListBox->SetScrollBarEndPos();
// }
// 
// void MonsterStateListDialog::KeyProcess( CKTDXStage* pStage )
// {
// 	if( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
// 		return; 
// 
// 	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_PERIOD) == TRUE )
// 	{
// 		if( true == IsOpen() )
// 		{
// 			Close();
// 		}
// 		else
// 		{
// 			Open( pStage );
// 		}
// 	}
// }
// 
// void MonsterStateListDialog::OnCommand( const std::wstring& monsterName )
// {
// 	if( NULL != g_pX2Game )
// 	{
// 		g_pX2Game->EnableAllNPCAI( false ); 
// 	}
// 
// 	WCHAR wszText[256] = L"";
// 	StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"/msc %s", monsterName.c_str() ); 
// 
// 	if ( NULL != g_pMain->GetGameEdit() )
// 		g_pMain->GetGameEdit()->ExecCommand( wszText );
// }
#endif MONSTER_STATE_LIST_TEST

/** @function : IsNearPortalLineMap() const
	@brief : 튜토리얼 던전에서 루벤마을로 이동 하는 동작
*/


#ifndef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

void BadAttitudeUserWarningDialog::SetFadeInDlg ( const float fTime_ )		// 해당 Dlg 를 Fade In 한다.
{
	if ( m_VecDlgPictureDataPointer.size() > 0 )			// Dlg 의 각 그림 데이터 포인트가 있다면
	{
		BOOST_FOREACH ( CKTDGUIControl::CPictureData* pPictureData, m_VecDlgPictureDataPointer ) // 각 그림 데이터/에 대해
		{
			pPictureData->SetFadeIn ( fTime_ );	// time 만큼 Fade 실시
		}	
	}

//	float interpolationValue = (6f - fTime) / 6f;
}

void BadAttitudeUserWarningDialog::SetFadeOutDlg ( const float fTime_ )		// 해당 Dlg 를 Fade Out 한다.
{
	if ( m_VecDlgPictureDataPointer.size() > 0 )  // Dlg 의 각 그림 데이터 포인트가 있다면
	{
		BOOST_FOREACH ( CKTDGUIControl::CPictureData* pPictureData, m_VecDlgPictureDataPointer )	// 각 그림 데이터에 대해
		{
			pPictureData->SetFadeOut ( fTime_ );		// time Fade Out 실시
		}		
	}
}

void BadAttitudeUserWarningDialog::Update( float fElapsedTime_ )
{
	if( m_pDLGWarningState == NULL )
		return;

	m_fWarningRemainingTime += fElapsedTime_;		// 생성된 후 존재했던 시간을 계산
	float fFadeValueUsingButton = 1.0f;					// 버튼의 Fade-In, out 에 사용할 값

	if ( m_fWarningRemainingTime > 6.0f )		// 6초 이상이면 다시 리셋 한다
	{
		Init();
	}

	else if ( m_fWarningRemainingTime > 5.0f )	// 5초 이상, 6초 미만이면
	{
		if ( false == GetIsFadeOut() )			// Fade Out 를 했는가?
		{
			SetFadeOutDlg( 1.0f );				// Fade Out 를 실시한다.
			SetIsFadeOut(true);			
		}
		fFadeValueUsingButton = 6.f - m_fWarningRemainingTime;
	}
	else if ( m_fWarningRemainingTime > 1.0f )
	{
		fFadeValueUsingButton = 1.f;
	}

	else if ( m_fWarningRemainingTime > .0f )	// 0초 이상 5초 미만이면
	{
		if ( false == GetIsFadeIn() )			// Fade in 을 했는가?
		{
			SetFadeInDlg( 1.0f );				// Fade in 을 실시한다.
			SetIsFadeIn(true);
		}
		fFadeValueUsingButton = m_fWarningRemainingTime;
	}
	
	m_pButtonExitWarningDlg->SetColor (D3DXCOLOR (1.f, 1.f, 1.f, fFadeValueUsingButton ) );

}



#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2StateDungeonGame::MoveToRubenVillage( float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2StateDungeonGame::MoveToRubenVillage()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if( false == g_pMain->GetIsPlayingTutorial() )
		return;

	if ( NULL != m_pDungeonGame && true == m_pDungeonGame->IsNearPortalLineMap() )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_TimerWaitingPortal.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_TimerWaitingPortal.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if ( m_TimerWaitingPortal.CheckAndResetElapsedTime() )
		{		
			g_pMain->SetIsPlayingTutorial( false );
			m_bLeaveRoomAtTutorial = true;

			g_pMain->SetIsExitingTutorial( true );
			if ( m_bSend_KEGS_LEAVE_ROOM_REQ == false )
				m_pDungeonGame->Handler_EGS_LEAVE_ROOM_REQ();

			m_bSend_KEGS_LEAVE_ROOM_REQ = true;

			if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )				
			{	////튜토리얼 진행방향 가이드 끄기
				if( true == g_pData->GetPlayGuide()->GetActiveNoviceGuide() )
				{
					g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_NOVICE_GUIDE_01, false);
				}
				g_pData->GetPlayGuide()->DeleteQuestGuideParticle();
				g_pData->GetPlayGuide()->DeleteInputGuideParticle();
			}
		}
	}
	else
	{
		m_TimerWaitingPortal.ResetSumOfElapsedTime();
	}

}

void CX2StateDungeonGame::DrawMovingSmallBar()
{
	if( NULL != m_pDungeonGame && NULL != m_pDungeonGame->GetMyUnit() && NULL != m_pDungeonGame->GetLineMap() )
	{
		const int iLastTouchedLineMapIndex = m_pDungeonGame->GetMyUnit()->GetLastTouchLineIndex(true);
		const CKTDGLineMap::LineData* pLastTouchedLineData = m_pDungeonGame->GetLineMap()->GetLineData( iLastTouchedLineMapIndex );
		if( NULL != pLastTouchedLineData )
		{
			D3DXVECTOR3 vPosMovingGage( ( (pLastTouchedLineData->startPos.x + pLastTouchedLineData->endPos.x ) / 2), pLastTouchedLineData->startPos.y + 300, pLastTouchedLineData->startPos.z );

			D3DXVECTOR3 vOut( 0.0f, 0.0f, 0.0f );
			D3DXVec3Project( &vOut, &vPosMovingGage, &g_pKTDXApp->GetViewport(), 
				&g_pKTDXApp->GetProjectionTransform(), &g_pKTDXApp->GetViewTransform(), &g_pKTDXApp->GetWorldTransform() );

			vOut.x -= ( 25 * g_pKTDXApp->GetResolutionScaleX() );
			vOut.y += ( -90 * g_pKTDXApp->GetResolutionScaleY() );

			if( NULL != m_TexDataMovingGageBG.pTexture )
			{
				m_TexDataMovingGageBG.pTexture->SetDeviceTexture();
				DrawFace( vOut.x+1.0f, vOut.y+1.0f, m_TexDataMovingGageBG,
					D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			if( NULL != m_TexDataMovingGage.pTexture )
			{
				vOut.x += ( 13 * g_pKTDXApp->GetResolutionScaleX() );
				vOut.y += ( 25 * g_pKTDXApp->GetResolutionScaleY() );
				const float fNowPercent = m_TimerWaitingPortal.GetSumOfElapsedTime() / m_TimerWaitingPortal.GetTargetTime();
				m_TexDataMovingGage.pTexture->SetDeviceTexture();
				DrawFace( vOut.x, vOut.y, m_TexDataMovingGage,
					D3DCOLOR_ARGB( 255, 255, 255, 255 ), fNowPercent );
			}
		}
	}
}


void CX2StateDungeonGame::DrawFace( const float fX_, const float fY_, const CKTDGUIControl::UITextureData& texData_, 
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
void CX2StateDungeonGame::CreateMovingSmallBar()
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

void CX2StateDungeonGame::DestroyMovingSmallBar()
{
	SAFE_CLOSE( m_TexDataMovingGageBG.pTexture );
	SAFE_CLOSE( m_TexDataMovingGage.pTexture );
}

#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
void CX2StateDungeonGame::AllocateFaultyPlayerWarningDlg ()
{
	// 강퇴 경고 DLG 할당, DLG_Bad_Attribute_Player_Warning.lua 에서 가져옴
	DeallocateFaultyPlayerWarningDlg ();
	m_pDLGFaultyPlayerWarning = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Bad_Attribute_Player_Warning.lua" );
	if ( NULL != m_pDLGFaultyPlayerWarning )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFaultyPlayerWarning );
		vector <CKTDGUIControl::CPictureData*> p_vecPictureData; // 그림 데이터 포인트를 저장해두는 곳
	
		// Dlg 의 Static 을 저장해서 해당 PictureData 를 vector 에 저장해둔다.
		CKTDGUIStatic* pStaticWarningDlg = static_cast <CKTDGUIStatic*> ( m_pDLGFaultyPlayerWarning->GetControl(L"DLG_Bad_Attribute_Player_Warning") );
		if ( NULL != pStaticWarningDlg )
		{
			CKTDGUIControl::CPictureData* pDlgPictureData = pStaticWarningDlg->GetPicture(0);		
			if ( NULL != pDlgPictureData )
				p_vecPictureData.push_back(pDlgPictureData);
		}
	
		// 버튼 의 Static 을 저장해서 해당 PictureData 를 vector 에 저장해둔다.
		// 버튼의 경우, Staitic 이 없으므로 Scirpt 단위에서 Static 을 만들고
		// 해당 Static 을 다시 불러와서 그림을 뽑아와야한다.
		CKTDGUIButton* pWarningDlgExitButton = static_cast <CKTDGUIButton*> ( m_pDLGFaultyPlayerWarning->GetControl(L"_exit") );
		if ( NULL != pWarningDlgExitButton )
		{
			CKTDGUIStatic* pStaticWarningDlgExitButton = pWarningDlgExitButton->GetNormalStatic();
			if ( NULL != pStaticWarningDlgExitButton )
			{
				CKTDGUIControl::CPictureData* pButtonPictureData = pStaticWarningDlgExitButton->GetPicture(0);
				p_vecPictureData.push_back(pButtonPictureData);
			}
		}

		// 각 그림 데이터에 접근하여 SetFlicker 함수를 사용	
		BOOST_FOREACH( CKTDGUIControl::CPictureData* pPictureData, p_vecPictureData )
		{
			if ( NULL != pPictureData )
			{
				pPictureData->SetFlicker( 3.0f, 1.f, 0.f );
			}
		}
	}
}


void CX2StateDungeonGame::DeallocateFaultyPlayerWarningDlg()
{
	SAFE_DELETE_DIALOG ( m_pDLGFaultyPlayerWarning );
	m_bFaultyPlayerWarningDlgExitButtonPressed = false;
}

void CX2StateDungeonGame::ReservingWarningDlgDelete ()
{
	// 원래는 SAFE_DELETE_DIALOG ( m_pDLGFaultyPlayerWarning ); 
	// 가 들어가야 맞는 구문이긴 하나, 버튼 메시지 처리 방식 상 불가능하다.
	// ( 이미 없어진 버튼의 State 를 변경하려고 함, Crash 유발 
	m_bFaultyPlayerWarningDlgExitButtonPressed = true;
}

#endif
#ifdef SERV_EVENT_VALENTINE_DUNGEON 
void CX2StateDungeonGame::Send_EGS_SYNC_DUNGEON_TIMER_NOT()
{
	if( NULL != m_pDungeonGame && 
		NULL != m_pDungeonGame->GetDungeon() &&
		NULL != m_pDungeonGame->GetDungeon()->GetDungeonData() &&
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
		SEnum::DI_EVENT_VALENTINE_DUNGEON_INT == m_pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID
#else
		SEnum::DI_EVENT_VALENTINE_DAY == m_pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
		)
	{
		m_bIsValentineDungeon = true;

		if( NULL == m_pDLGValentineTimer )
		{
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
			m_pDLGValentineTimer = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Valentine_Dungeon_Play_Timer.lua" );
#else
			m_pDLGValentineTimer = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Valentine_Dungeon_Timer.lua" );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGValentineTimer );
			m_pDLGValentineTimer->SetShowEnable(true, true);
		}

		if( true == g_pX2Game->IsHost() )
		{
			g_pData->GetServerProtocol()->SendID( EGS_SYNC_DUNGEON_TIMER_NOT );
		}
	}	
}

bool CX2StateDungeonGame::Handler_EGS_SYNC_DUNGEON_TIMER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SYNC_DUNGEON_TIMER_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_fValentineDungeonRemainTime = static_cast<float>(kEvent.m_iRemainTime);

#ifndef SERV_EVENT_VALENTINE_DUNGEON_INT
#ifdef FIXED_CLEAR_VALENTINE_DUNGEON_WHEN_REMAINING_TIME_ZERO
	if ( m_fValentineDungeonRemainTime <= 0 )		// 남은 시간이 0 이하 라면..게임 끝!
	{
		if( NULL != m_pDungeonGame && 
			NULL != m_pDungeonGame->GetDungeon() && 
			NULL != m_pDungeonGame->GetDungeon()->GetNowStage() &&
			NULL != m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage() )
		{
			if ( m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->GetEndGameDelay() <= 0 )
			{
				m_pDungeonGame->SetWindTeam( CX2Room::TN_RED );
				m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage()->ClearDungeonGame();
			}
		}
	}
#endif // FIXED_CLEAR_VALENTINE_DUNGEON_WHEN_REMAINING_TIME_ZERO
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
	return true;
}
void CX2StateDungeonGame::ValentineDungeonTimer_OnFrameMove( float fElapsedTime_ )
{ 
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	m_fValentineDungeonRemainTime += fElapsedTime_;
#else
	m_fValentineDungeonRemainTime -= fElapsedTime_;
#endif SERV_EVENT_VALENTINE_DUNGEON_INT


	if( m_fValentineDungeonRemainTime < 0 || m_iValentineDungeonStage < 0 )
		return;

	if( NULL != m_pDLGValentineTimer )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGValentineTimer->GetControl(L"Valentine_Dungeon_Timer"));
		if( NULL != pStatic )
		{
#ifndef SERV_EVENT_VALENTINE_DUNGEON_INT
			// 현재 스테이지 계산
			const wstring STAGE_TEXTURE_KEY[] = {	L"valen_stage_number_0",
				L"valen_stage_number_1",
				L"valen_stage_number_2",
				L"valen_stage_number_3",
				L"valen_stage_number_4",
				L"valen_stage_number_5",
				L"valen_stage_number_6",
				L"valen_stage_number_7",
				L"valen_stage_number_8",
				L"valen_stage_number_9" };

			UINT iStage10 = min( m_iValentineDungeonStage / 10, 9 );
			UINT iStage1 = min( m_iValentineDungeonStage % 10, 9 );	

			CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture(1);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", STAGE_TEXTURE_KEY[iStage10].c_str());
			}

			pPicture = pStatic->GetPicture(2);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", STAGE_TEXTURE_KEY[iStage1].c_str());
			}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

			// 남은 시간 계산
			const wstring TIMER_TEXTURE_KEY[] = {	L"valen_time_no_0",
				L"valen_time_no_1",
				L"valen_time_no_2",
				L"valen_time_no_3",
				L"valen_time_no_4",
				L"valen_time_no_5",
				L"valen_time_no_6",
				L"valen_time_no_7",
				L"valen_time_no_8",
				L"valen_time_no_9" };

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
			UINT iRemainMin10 = min( static_cast<UINT>(m_fValentineDungeonRemainTime) / 600, 9 );
			UINT iRemainMin1 = min( ( static_cast<UINT>(m_fValentineDungeonRemainTime) % 600 ) / 60, 9 );
			UINT iRemainSec10 = min( ( static_cast<UINT>(m_fValentineDungeonRemainTime) % 60 ) / 10, 9 );
			UINT iRemainSec1  = min( static_cast<UINT>(m_fValentineDungeonRemainTime) % 10, 9 );

			//시간
			CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture(1);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", TIMER_TEXTURE_KEY[iRemainMin10].c_str());
			}

			pPicture = pStatic->GetPicture(2);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", TIMER_TEXTURE_KEY[iRemainMin1].c_str());
			}

			pPicture = pStatic->GetPicture(3);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", TIMER_TEXTURE_KEY[iRemainSec10].c_str());
			}

			pPicture = pStatic->GetPicture(4);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", TIMER_TEXTURE_KEY[iRemainSec1].c_str());
			}
#else
			UINT iRemainSec10 = min( static_cast<UINT>(m_fValentineDungeonRemainTime) / 10, 9 );	//10의 자리 초.
			UINT iRemainSec1  = min( static_cast<UINT>(m_fValentineDungeonRemainTime) % 10, 9 );	//1의 자리 초

			//시간
			pPicture = pStatic->GetPicture(3);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", TIMER_TEXTURE_KEY[iRemainSec10].c_str());
			}

			pPicture = pStatic->GetPicture(4);
			if( NULL != pPicture )
			{
				pPicture->SetTex(L"DLG_UI_Common_Texture65_NEW.tga", TIMER_TEXTURE_KEY[iRemainSec1].c_str());
			}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
		}
	}
}

void CX2StateDungeonGame::NotfiyCreateValentineCupCake()
{ 
	++m_iValentineDungeonStage;

	if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )		/// 스테이지 변경시 발생되는 안내 문구 이펙트
	{
		m_hStageStartEffect1 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"Vallentine_Stage_Change_P01", 
																  0, 0, 0, 9999, 9999, -1, 1, -1.0f, true, 1.2f, false );

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hStageStartEffect1 )
		{
			CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hStageStartEffect1 );
			if( pParticle != NULL )
			{
				pParticle->SetOverUI( true );
			}
		}

		m_hStageStartEffect2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"Vallentine_Stage_Change_P02", 
																	 0, 0, 0, 9999, 9999, -1, 1, -1.0f, true, 1.2f, false );

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hStageStartEffect2 )
		{
			CKTDGParticleSystem::CParticleEventSequence* pParticle = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hStageStartEffect2 );
			if( pParticle != NULL )
			{
				pParticle->SetOverUI( true );
			}
		}
	}
}
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_CATCH_HACKUSER_INFO
bool CX2StateDungeonGame::Handler_EGS_CATCH_HACKUSER_INFO_NOT( KEGS_CATCH_HACKUSER_INFO_NOT& kEGS_CATCH_HACKUSER_INFO_NOT )
{
	g_pData->GetServerProtocol()->SendPacket( EGS_CATCH_HACKUSER_INFO_NOT, kEGS_CATCH_HACKUSER_INFO_NOT );

	return true;
}
#endif SERV_CATCH_HACKUSER_INFO

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09

/** @function	: Handler_EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT
	@brief		: 보스 몬스터 사망 직후, 어둠의 문 입장신을 위하여 보내지는 패킷에 대한 처리
	@param		: 이벤트 메시지
	@return		: 처리 여부
*/
bool CX2StateDungeonGame::Handler_EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	SetEnterDefenceDungeon( true );		/// 어둠의 문 입장 처리 여부 설정

	const D3DXVECTOR3 vCreatePos( kEvent.m_DieBossPos.x, kEvent.m_DieBossPos.y, kEvent.m_DieBossPos.z );
	
	/// 입장 몬스터 생성
	if ( NULL != g_pX2Game && true == g_pX2Game->IsHost() )
		g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_SEED_OF_DARK_PORTAL, 1, true, vCreatePos, true, 0.f, true, -1, 
								 CX2Room::TN_RED, CX2NPCAI::NAT_NORMAL, -1, false, CX2Room::TN_NONE, CX2GUNPC::NCT_DEFENCE_DUNGEON  );

	/// 어둠의 문 던전 데이터 안의 입장 대사를 받아와서, 엔딩 스피치에 적용
	const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( static_cast<SEnum::DUNGEON_ID>(38300) );
	if( NULL != pDungeonData || pDungeonData->m_vecEndingSpeechSetMap.size() >= 2 )
	{
		const CX2Dungeon::DungeonData::EndingSpeechSetMap& mapSpeech = pDungeonData->m_vecEndingSpeechSetMap[ 1 ];
		std::map< CX2Unit::UNIT_TYPE, CX2Dungeon::DungeonData::EndingSpeechSet >::const_iterator it;
		it = mapSpeech.find( g_pData->GetMyUser()->GetSelectUnit()->GetType() );
		if( it != mapSpeech.end() )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet = it->second;
			if( false == endingSpeechSet.empty() )
			{
				int iRandomSpeechIndex = rand() % (int) endingSpeechSet.size();
				const CX2Dungeon::DungeonData::EndingSpeech& endingSpeech = endingSpeechSet[ iRandomSpeechIndex ];

				CX2StateDungeonGame* pState = static_cast<CX2StateDungeonGame*>( g_pMain->GetNowState() );

				if ( NULL != pState )
					pState->GetDungeonEndingEvent()->m_vecSpeech1 = endingSpeech;
			}
		}
	}

	return true;
}

/** @function	: Handler_EGS_UPDATE_INVENTORY_SLOT_INFO_NOT
	@brief		: 실시간 인벤토리 변경 동작 패킷 처리
	@param		: 이벤트 메시지
	@return		: 처리 여부
*/
bool CX2StateDungeonGame::Handler_EGS_UPDATE_INVENTORY_SLOT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_INVENTORY_SLOT_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pData )
	{
		if ( NULL != g_pData->GetMyUser() &&
			 NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			 g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
		}

		if ( NULL != g_pData->GetUIManager() &&
			 NULL != g_pData->GetUIManager()->GetUIQuickSlot() )
		{
			g_pData->GetUIManager()->GetUIQuickSlot()->ResetQuickSlotUI();
		}
	}

	return true;
}

/** @function	: WaitDefenceDungeonStart
	@brief		: 특정 시간 이후 어둠의 문 스테이트 변경을 처리하는 함수
	@param		: 프레임당 시간
*/
void CX2StateDungeonGame::WaitDefenceDungeonStart( const float fElapsedTime )
{
	if ( false == IsSamef( m_fWaitDefenceDungeonStartTime, -1.f ) )
	{
		m_fWaitDefenceDungeonStartTime -= fElapsedTime;

		if ( 0.f >= m_fWaitDefenceDungeonStartTime && NULL != g_pKTDXApp )
		{
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_GAME, NULL, false );
			m_fWaitDefenceDungeonStartTime = -1.f;
		}
	}
}

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // 적용날짜: 2013-04-22

/** @function	: Handler_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT
	@brief		: 어둠의 문 입장 거부로 인한 던전 퇴장 패킷 처리
	@param		: 이벤트 메시지
	@return		: 처리 여부
*/
bool CX2StateDungeonGame::Handler_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( NULL != g_pMain && NULL != g_pMain->GetNowState() )
	{
		CX2State* pState = static_cast<CX2State*>( g_pMain->GetNowState() );
		if( pState != NULL )
			pState->ReturnToPlaceWhereBeforeDungeonStart();
	}

	//CX2GameUnit* pMyUserUnit = m_pDungeonGame->GetMyUnit();
	//if ( pMyUserUnit != NULL )
	//{
	//	pMyUserUnit->Win();

	//	GetDungeonEndingEvent()->m_fElapsedTime_WinEffect = 0.f;

	//	if( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON ) // result 화면에서 win or lose를 표시하기 위해서
	//	{			
	//		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
	//		if( NULL != pDungeonGame )
	//			pDungeonGame->SetWindTeam( (CX2Room::TEAM_NUM)pMyUserUnit->GetTeam() );
	//	}
	//}

	return true;
}

/** @function	: Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT
	@brief		: 어둠의 문 입장 여부 팝업 생성 패킷 처리
	@param		: 이벤트 메시지
	@return		: 처리 여부
*/
bool CX2StateDungeonGame::Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	/// 입장 여부를 선택하는 팝업 생성
	if ( NULL != g_pMain &&
		 NULL != g_pData &&
		 NULL != g_pData->GetMyUser() &&
		 NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;

		userData.iMyUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();

		g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_ENTER_GATE_OF_DARKNESS,
									   CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 5.f, GET_STRING( STR_ID_24445 ), 
									   static_cast<CKTDXStage*>( g_pMain->GetNowState() ),
									   CX2StateDungeonGame::DGUCM_AGREE_ENTER_GATE_OF_DARKNESS, 
									   CX2StateDungeonGame::DGUCM_NOT_AGREE_ENTER_GATE_OF_DARKNESS, 
									   CX2StateDungeonGame::DGUCM_AGREE_ENTER_GATE_OF_DARKNESS );

	}
	
	return true;
}

/** @function	: Sned_Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT
	@brief		: 어둠의 문 입장 여부 수락 정보 전송
*/
void CX2StateDungeonGame::Sned_Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT( bool bAgree )
{
	KEGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT kPacket;
	kPacket.m_bAgree = bAgree;

	g_pData->GetServerProtocol()->SendPacket( EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT, kPacket );

	/// 입장 여부 선택창 제거
	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	if ( NULL != g_pMain &&
		 NULL != g_pData &&
		 NULL != g_pData->GetMyUser() &&
		 NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();

		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_ENTER_GATE_OF_DARKNESS, userData );
	}

	return;
}

#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES // 오현빈
/** @function	: Handler_EGS_DUNGEON_KILLALLNPC_CHECK_NOT
	@brief		: 살아있는 몬스터가 없는지 체크 하는 것으로, 던전 클리어 했을 때
				  클리어 정보를 호스트 외에 유저에게도 알려주기 위한 처리
*/
bool CX2StateDungeonGame::Handler_EGS_DUNGEON_KILLALLNPC_CHECK_NOT()
{
	CX2DungeonSubStage *pSubStage = NULL;

	if( m_pDungeonGame != NULL && 
		m_pDungeonGame->GetDungeon() != NULL && 
		m_pDungeonGame->GetDungeon()->GetNowStage() != NULL )
	{
		pSubStage = m_pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStage();
	}
	
	// 던전클리어 진행시킨다.
	if( pSubStage != NULL )
		pSubStage->ClearDungeonGame();

	return true;
}
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES


#ifdef PLAY_EMOTION_BY_USER_SELECT
/*virtual */void CX2StateDungeonGame::PlayEmotionByUserSelect()
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

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
bool CX2StateDungeonGame::Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	return m_pDungeonGame->Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( kEvent );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
bool CX2StateDungeonGame::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 인벤토리가 열려 있을 때는 인벤토리의 휠 처리만 하기
	if( NULL != g_pData &&
		NULL != g_pData->GetUIManager() && 
		NULL != g_pData->GetUIManager()->GetUIInventory() &&
		true == g_pData->GetUIManager()->GetUIInventory()->GetIsMouseOver() )
	{
		return true;
	}

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
#endif //DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL


#ifdef REFORM_ENTRY_POINT
void CX2StateDungeonGame::SetLoadingGageBar( int iLoadingPercent )
{
	if( NULL == m_pDLGLoadingFront )
		return;

	CKTDGUIStatic* pStatic_Loading = m_pDLGLoadingFront->GetStatic_LUA( "First_Loading" );
	CKTDGUIControl::CPictureData* pPicture_OrangeBar = pStatic_Loading->GetPicture(0);
#ifdef FIX_FIRST_LOADING
	const float MAGIC_GAGE_BAR_FULL_LENGTH = 1024;
#else
	CKTDGUIControl::CPictureData* pPicture_LeftEdge = pStatic_Loading->GetPicture(1);
	CKTDGUIControl::CPictureData* pPicture_RightEdge = pStatic_Loading->GetPicture(2);
	CKTDGUIControl::CPictureData* pPicture_UpEdge = pStatic_Loading->GetPicture(3);
	CKTDGUIControl::CPictureData* pPicture_DownEdge = pStatic_Loading->GetPicture(4);

	const float MAGIC_GAGE_BAR_FULL_LENGTH = 614;
#endif //FIX_FIRST_LOADING

	const float fNowBarLength = MAGIC_GAGE_BAR_FULL_LENGTH * (float)iLoadingPercent / 100.f;
	pPicture_OrangeBar->SetSizeX( fNowBarLength );

}
#endif //REFORM_ENTRY_POINT
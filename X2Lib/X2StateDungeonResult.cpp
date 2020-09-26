#include "StdAfx.h"
#include ".\x2statedungeonresult.h"


/** @function	CX2StateDungeonResult
	@brief		생성자, 멤버 초기화
*/
CX2StateDungeonResult::CX2StateDungeonResult(void) :
m_pDLGBack( NULL ),
m_pDLGFront( NULL ),
m_pDLGMsgBox( NULL ),
m_fPlayBGM( 0.5f ),
m_bStartTimer( false ),
m_fRemainTime( 4.f ),
m_bSendRefreshRoomReq( false ),
m_fTotalScore( 0.f ),
m_fComboScore( 0.f ),
m_fTechnicalScore( 0.f ),
m_fHour( 0.f ),
m_fMinute( 0.f ),
m_fSec( 0.f ),
m_fDamaged( 0.f ),
m_DungeonResultAniOrder( DRAO_DUNGEON_PICTURE_BG ),
m_bInitUIFail( false ),
m_hParticleElMark( INVALID_PARTICLE_SEQUENCE_HANDLE ),
m_hParticleRank( INVALID_PARTICLE_SEQUENCE_HANDLE ),
m_pSoundElMark( NULL ),
m_pPicCharScore( NULL ),
m_pPicCharEXP( NULL ),
m_pPicCharBonusEXP( NULL ),
m_pPicCharComboAndTechScore( NULL ),
m_pPicCharED( NULL ),
m_bAnimateScoreBG( false ),
m_fStartWaitRemainTime( 2.f ),
m_bUIAnimStart( false ),
m_fMyTotalRankWaitRemainTime( 1.5f ),
m_bUIMyTotalRankStart( false ),
m_fWaitForFadeOutRemainTime( 2.5f ),
m_bWaitForFadeOut( true ),
m_ChoiceItemID( -1 ),
m_fED( 0.f ),
m_fEXP( 0.f ),
m_fBonusEXP( 0.f ),
m_fEXPSize( 0.f ),
m_fAnimatBoxTime( 0.f ),
m_bSendStartRewardReq( false ),
m_bStartRewardBoxSelectNot( false ),
//m_vecRewardXMeshInst;
//m_vecBonusStamp;
//m_vecBonusExpType;
m_fTimeBonusEXPShowEvent( 0.f ),
m_bCheckBonusEXP( false ),
m_bSendedRegHack( false )
//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
#ifdef	SERV_INSERT_GLOBAL_SERVER
, m_bShowNumOfElCrystalsProtected( false )	/// 보호된 엘의 갯수를 출력 할 것인가? (디펜스 던전에서 승리한 경우만 출력)
#endif	SERV_INSERT_GLOBAL_SERVER
//}} kimhc // 2011-04-18 // 국내, 디펜스 던전
, m_bPressEnterKeyOrZKeyForSkip( false )
{
	DialogLog( "\nCX2StateDungeonResult::CX2StateDungeonResult Start\n" );


	if ( InitUI() == false )
	{
		m_bInitUIFail = true;
		m_bSendRefreshRoomReq = true;
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_619 ), this );
		ErrorLogMsg( XEM_ERROR19, "던전 결과창 초기화 실패!" );
		//Handler_EGS_REFRESH_ROOM_REQ();
		Handler_EGS_RESULT_SUCCESS_REQ();
	}

	if( g_pKTDXApp->GetDSManager()->GetInit() == true && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL )
		g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( L"MusicWin.ogg", false );

	m_pSoundElMark		= g_pKTDXApp->GetDeviceManager()->OpenSound( L"ResultStamp.ogg" );

	SoundReady( L"Count.ogg" );
	SoundReady( L"ResultStamp.ogg" );
	SoundReady( L"EXPUp.ogg" );
	SoundReady( L"LevelUp.ogg" );
	SoundReady( L"ResultBonus.ogg" );
	SoundReady( L"LevelUpBGM.ogg" );

	//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", true );

	// 던전결과창에서 커뮤니티창 숨긴다.
	if(g_pData != NULL && g_pData->GetMessenger() != NULL)
	{
		g_pData->GetMessenger()->SetHideWindow(true);
	}
	

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );

	g_pKTDXApp->SkipFrame();

	//CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox", 0,0,0, 0,0,0, 0,0,0 );
	//pMeshInstance->SetAge( 1000 );

    

#ifdef REFORM_UI_SKILLSLOT
	if( NULL != g_pData && 
		NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->SetShow(false);
	}
#endif //REFORM_UI_SKILLSLOT

#ifdef RIDING_SYSTEM
	if( NULL != CX2RidingPetManager::GetInstance() && NULL != CX2RidingPetManager::GetInstance()->GetSkillSlot() )
		CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetShowRidingSkillSlot(false);
#endif //RIDING_SYSTEM

	DialogLog( "\nCX2StateDungeonResult::CX2StateDungeonResult End\n" );
}

CX2StateDungeonResult::~CX2StateDungeonResult(void)
{
	DialogLog( "\nCX2StateDungeonResult::~CX2StateDungeonResult Start\n" );

	g_pMain->ResetDungeonRewardItem();

	SAFE_DELETE_DIALOG( m_pDLGBack );
	SAFE_DELETE_DIALOG( m_pDLGFront );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );

	g_pData->DeleteDungeonResultInfo();

	if ( m_hParticleElMark != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleElMark );

	if ( m_hParticleRank != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleRank );


	SAFE_CLOSE( m_pSoundElMark );


	m_pPicCharScore->Clear();
	SAFE_DELETE( m_pPicCharScore );
	m_pPicCharEXP->Clear();
	SAFE_DELETE( m_pPicCharEXP );
	m_pPicCharBonusEXP->Clear();
	SAFE_DELETE( m_pPicCharBonusEXP );
	m_pPicCharComboAndTechScore->Clear();
	SAFE_DELETE( m_pPicCharComboAndTechScore );
	m_pPicCharED->Clear();
	SAFE_DELETE( m_pPicCharED );

	for ( int i = 0; i < (int)m_vecRewardXMeshInst.size(); i++ )
	{
		CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshInst = m_vecRewardXMeshInst[i];
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( hMeshInst );
	}
	m_vecRewardXMeshInst.clear();

	for ( int i = 0; i < (int)m_vecBonusStamp.size(); i++ )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_vecBonusStamp[i] );
	}

	DialogLog( "\nCX2StateDungeonResult::~CX2StateDungeonResult End\n" );
}



HRESULT CX2StateDungeonResult::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bInitUIFail == true )
		return S_OK;

	if ( g_pData->GetDungeonResultInfo() == NULL )
	{
		if ( m_bSendRefreshRoomReq == false )
		{
			m_bSendRefreshRoomReq = true;
			Handler_EGS_RESULT_SUCCESS_REQ();
		}
	}
	if( NULL != g_pData && NULL != g_pData->GetPlayGuide() )
	{
		g_pData->GetPlayGuide()->OnFrameMove(fTime, fElapsedTime);
	}

	CX2StateCommonBG::OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	if ( m_bStartTimer == true )
		m_fRemainTime -= fElapsedTime;

	if ( m_fRemainTime <= 0.0f && m_bSendRefreshRoomReq == false )
	{
		m_bSendRefreshRoomReq = true;
		
		if ( g_pX2Room->GetBan() == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_553 ) );
			//g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_ROOM, NULL, false );	

			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, NULL, false );	
		}
		else
		{
			//Handler_EGS_REFRESH_ROOM_REQ();
			Handler_EGS_RESULT_SUCCESS_REQ();
		}
	}

	PicCharFrameMove( fTime, fElapsedTime );

	CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetMySlot();
	if( NULL != pSlotData && pSlotData->m_pUnit != NULL && NULL != pSlotData->m_pUnitViewer )
	{
		UnitViewerProcess( pSlotData->m_pUnitViewer );
	}

	if ( m_bUIAnimStart == false )
	{
		m_fStartWaitRemainTime =- fElapsedTime;
	}

	if ( m_fStartWaitRemainTime <= 0.0f )
	{
		m_bUIAnimStart = true;
	}


	if ( m_bUIAnimStart == true )
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGBack ) == false )
		{
			return S_OK;
		}

		if ( false == GetPressEnterKeyOrZKeyForSkip() )	/// 아직 Enter 또는 Z 키를 눌러서 결과창을 스킵 하지 않았으면
		{
			/// Enter 또는 Z 키를 누르는지 체크
#ifdef X2TOOL
			CKTDIDeviceWinKey* pKTDIDevice = g_pKTDXApp->GetDIManager()->Getkeyboard();
#else
			CKTDIDevice* pKTDIDevice = g_pKTDXApp->GetDIManager()->Getkeyboard();
#endif
			if ( pKTDIDevice->GetKeyState( DIK_RETURN ) == TRUE || pKTDIDevice->GetKeyState( DIK_Z ) == TRUE )
				SetPressEnterKeyOrZKeyForSkip( true );	/// 눌렀음
		}

		const int DELTA_FOR_ED_AND_EXP = 33333;	/// ED & EXP
		const int DELTA_FOR_TIME = 35;		/// time
		const int DELTA_FOR_COMBO_AND_TECHNIQUE = 333333;	///	combo & technique
		

		//일단 자기꺼 표시

		int playTimeAllSec = 0;

		if( NULL != g_pData->GetDungeonResultInfo() )
		{
			playTimeAllSec = g_pData->GetDungeonResultInfo()->m_Sec; 
		}		

		int playTimeHour = playTimeAllSec/3600; 
		int	playTimeMinute = ( playTimeAllSec%3600 ) / 60;
		int playTimeSec = ( playTimeAllSec%3600 ) % 60;

#ifdef SERV_HACKING_TOOL_LIST
		// 특정던전을 제외한 던전 클리어 시간이 2분 이하일 경우 핵으로 감지한다. 단 던전 적정렙보다 20렙 이상의 유저는 검사하지 않는다.
		const CX2Dungeon::DungeonData* pDungeonData_Difficulty = NULL;

		if( NULL != g_pData && NULL != g_pData->GetDungeonManager() &&
			NULL != g_pData->GetPartyManager() && NULL != g_pData->GetPartyManager()->GetMyPartyData() )
		{
			int iDungeonID = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID;
			int iDungeonDifficulty = g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty;
			pDungeonData_Difficulty = g_pData->GetDungeonManager()->GetDungeonData( static_cast<SEnum::DUNGEON_ID>( iDungeonID + iDungeonDifficulty) );
		}

		int iDungeonLv = 0;
		if( NULL != pDungeonData_Difficulty )
		{
			iDungeonLv = pDungeonData_Difficulty->m_MinLevel;

			if( pDungeonData_Difficulty->m_MaxLevel > 0 )
			{
				iDungeonLv = pDungeonData_Difficulty->m_MaxLevel;
			}
		}

#ifdef SERV_CATCH_HACKUSER_INFO
		if ( g_pData->GetDungeonResultInfo() == NULL )
		{
			//Handler_EGS_CATCH_HACKUSER_INFO_NOT(3);
			Handler_EGS_CLIENT_QUIT_REQ();
		}
#endif SERV_CATCH_HACKUSER_INFO

		int nPlayer = (int)g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList.size();		
		int iUserLv = g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUnitData().m_Level;

		if( g_pData->GetDungeonResultInfo()->m_bWin == true &&	// 클리어했고
			playTimeAllSec <= 120 &&							// 클리어 소요 시간이 2분 이하이고
			nPlayer == 1 &&										// 솔플이고
			iUserLv <= iDungeonLv + 20 &&						// 던전 적정최고렙보다 20레벨 이상의 유저가 아니면 핵검사
			g_pKTDXApp->GetFindHacking() == false && 
			m_bSendedRegHack == false &&
			g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
		{
			SEnum::DUNGEON_ID eDungeon = g_pData->GetDungeonRoom()->GetDungeonID();			

			// 아래 던전부터 검사하도록 한다.
			if( eDungeon != SEnum::DI_ELDER_HENIR_SPACE &&
// 				eDungeon != SEnum::DI_BESMA_HENIR_SPACE &&
// 				eDungeon != SEnum::DI_ALTERA_HENIR_SPACE &&
// 				eDungeon != SEnum::DI_FEITA_HENIR_SPACE &&
// 				eDungeon != SEnum::DI_VELDER_HENIR_SPACE &&
// 				eDungeon != SEnum::DI_HAMEL_HENIR_SPACE &&
// 				eDungeon != SEnum::DI_BATTLE_SHIP_VELDER &&
// 				eDungeon != SEnum::DI_BATTLE_SHIP_HAMEL &&
				( eDungeon > SEnum::DI_MONSTER_TEST_EXPERT && eDungeon < SEnum::DI_TRAINING_FREE ) )
			{
				// 핵으로 감지
				if(g_pData != NULL && g_pData->GetServerProtocol() != NULL )
				{
					g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );								
					if( g_pData != NULL && g_pData->GetMyUser() != NULL )
						g_pData->GetMyUser()->AccessUserData().hackingUserType = CX2User::HUT_AGREE_HACK_USER;
				}	

#ifdef ADD_COLLECT_CLIENT_INFO			
				g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_21, "", false, true );
#else
				g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_AntiHacking_Dungeon_ClearTime );
#endif
				m_bSendedRegHack = true;
				//g_pKTDXApp->SetFindHacking( true );
			}
		}
#endif

		for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList.size(); i++ )
		{
			CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList[i];

			if ( pDungeonResultUnitInfo->m_UnitUID == g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUID() )
			{
				//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
				switch(m_DungeonResultAniOrder)
				{
				case DRAO_DUNGEON_PICTURE_BG:
					{
						if ( m_bAnimateScoreBG == false )
						{
							m_bAnimateScoreBG = true;
							CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Info" );
							if ( pStaticScoreBG != NULL )
							{
								pStaticScoreBG->SetShow( true );
								pStaticScoreBG->SetOffsetPos( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x - 70, pStaticScoreBG->GetOffsetPos().y ) );
								pStaticScoreBG->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticScoreBG->Move( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x + 70, 
									pStaticScoreBG->GetOffsetPos().y ),
									D3DXCOLOR(1,1,1,1), 0.2f, true );
							}
						}

						else
						{
							CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Info" );

							if ( pStaticScoreBG != NULL && pStaticScoreBG->GetIsMoving() == false )
							{
								m_bAnimateScoreBG = false;

								//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
					#ifdef	SERV_INSERT_GLOBAL_SERVER
								m_DungeonResultAniOrder = DRA0_DUNGEON_DEFENCE_RESULT;
					#else	SERV_INSERT_GLOBAL_SERVER
								m_DungeonResultAniOrder = DRAO_TOTAL_SCORE_BG;
					#endif	SERV_INSERT_GLOBAL_SERVER
								//}} kimhc // 2011-04-18 // 국내, 디펜스 던전
							}
						}
					} break;
					
					//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
		#ifdef	SERV_INSERT_GLOBAL_SERVER
				case DRA0_DUNGEON_DEFENCE_RESULT:
					{
						// 디펜스 던전에서 승리 했으면 보호된 엘의 수정 갯수를 보여줌
						if ( true == GetShowNumOfElCrystalsProtected() )
						{
							if ( false == m_bAnimateScoreBG )
							{
								m_bAnimateScoreBG = true;
								CKTDGUIStatic* pStaticDefenceDungeonResult = static_cast<CKTDGUIStatic*>( m_pDLGBack->GetControl( L"Defence_result" ) );
								if ( pStaticDefenceDungeonResult != NULL )
								{
									pStaticDefenceDungeonResult->SetShow( true );
									pStaticDefenceDungeonResult->SetOffsetPos( D3DXVECTOR2( pStaticDefenceDungeonResult->GetOffsetPos().x - 70, pStaticDefenceDungeonResult->GetOffsetPos().y ) );
									pStaticDefenceDungeonResult->SetColor( D3DXCOLOR(1,1,1,0) );
									pStaticDefenceDungeonResult->Move( D3DXVECTOR2( pStaticDefenceDungeonResult->GetOffsetPos().x + 70, 
										pStaticDefenceDungeonResult->GetOffsetPos().y ),
										D3DXCOLOR(1,1,1,1), 0.2f, true );

								}
							}
							else
							{
								CKTDGUIStatic* pStaticDefenceDungeonResult = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Info" );

								if ( pStaticDefenceDungeonResult != NULL && pStaticDefenceDungeonResult->GetIsMoving() == false )
								{
									m_bAnimateScoreBG = false;
									m_DungeonResultAniOrder = DRAO_TOTAL_SCORE_BG;
								}
							}
						}
						// 디펜스 던전에서 승리하지 못했거나, 디펜스 던전이 아니면
						else
							m_DungeonResultAniOrder = DRAO_TOTAL_SCORE_BG;	// 바로 다음으로 넘어감						
						
					} break;
		#endif	SERV_INSERT_GLOBAL_SERVER
					//}} kimhc // 2011-04-18 // 국내, 디펜스 던전

				case DRAO_TOTAL_SCORE_BG:
					{
						if ( m_bAnimateScoreBG == false )
						{
							m_bAnimateScoreBG = true;
							CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"TotalScore_BG" );
							if ( pStaticScoreBG != NULL )
							{
								pStaticScoreBG->SetShow( true );
								pStaticScoreBG->SetOffsetPos( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x - 70, pStaticScoreBG->GetOffsetPos().y ) );
								pStaticScoreBG->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticScoreBG->Move( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x + 70, 
									pStaticScoreBG->GetOffsetPos().y ),
									D3DXCOLOR(1,1,1,1), 0.2f, true );

							}
						}
						else
						{
							CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"TotalScore_BG" );

							if ( pStaticScoreBG != NULL && pStaticScoreBG->GetIsMoving() == false )
							{
								m_bAnimateScoreBG = false;
#ifdef SERV_DUNGEON_MVP
								m_DungeonResultAniOrder = DRAO_PICKED_MVP_PLAYER;
#else // SERV_DUNGEON_MVP
								m_DungeonResultAniOrder = DRAO_TIME_AND_DAMAGED_SCORE_BG;
#endif // SERV_DUNGEON_MVP
							}
						}


					} break;

				case DRAO_TIME_AND_DAMAGED_SCORE_BG:
					{
						if ( m_bAnimateScoreBG == false )
						{
							m_bAnimateScoreBG = true;
							CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_TIME_AND_DAMAGED_Score_BG" );
							if ( pStaticScoreBG != NULL )
							{
								pStaticScoreBG->SetShow( true );
								pStaticScoreBG->SetOffsetPos( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x - 70, pStaticScoreBG->GetOffsetPos().y ) );
								pStaticScoreBG->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticScoreBG->Move( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x + 70, 
									pStaticScoreBG->GetOffsetPos().y ),
									D3DXCOLOR(1,1,1,1),	0.2f, true );
							}
						}
						else
						{
							CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_TIME_AND_DAMAGED_Score_BG" );

							if ( pStaticScoreBG != NULL &&pStaticScoreBG->GetIsMoving() == false )
							{
								m_bAnimateScoreBG = false;
								m_DungeonResultAniOrder = DRAO_COMBO_SCORE;
								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", true, false );
							}
						}
					} break;
					/*
				case DRAO_DAMAGED_SCORE_BG:
					{
						if ( m_bAnimateScoreBG == false )
						{
							m_bAnimateScoreBG = true;
							CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Damaged_Score_BG" );
							if ( pStaticScoreBG != NULL )
							{
								pStaticScoreBG->SetShow( true );
								pStaticScoreBG->SetOffsetPos( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x - 70, pStaticScoreBG->GetOffsetPos().y ) );
								pStaticScoreBG->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticScoreBG->Move( D3DXVECTOR2( pStaticScoreBG->GetOffsetPos().x + 70, 
								pStaticScoreBG->GetOffsetPos().y ),
								D3DXCOLOR(1,1,1,1), 0.2f, true );
							}
							}
							else
							{
								CKTDGUIStatic* pStaticScoreBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Damaged_Score_BG" );

							if ( pStaticScoreBG != NULL && pStaticScoreBG->GetIsMoving() == false )
							{
								m_bAnimateScoreBG = false;
								m_DungeonResultAniOrder = DRAO_COMBO_SCORE;

								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", true );
							}
						}
					} break;
					*/
				case DRAO_COMBO_SCORE:
					{
						if ( GetPressEnterKeyOrZKeyForSkip() )
							m_fComboScore = static_cast<float>( pDungeonResultUnitInfo->m_nComboScore );
						else
							m_fComboScore += (fElapsedTime * DELTA_FOR_COMBO_AND_TECHNIQUE);

						if ( (int)m_fComboScore >= pDungeonResultUnitInfo->m_nComboScore )
						{
							m_fComboScore = static_cast<float>( pDungeonResultUnitInfo->m_nComboScore );
							m_DungeonResultAniOrder = DRAO_TECHNICAL_SCORE;

							CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Combo_Rank" );
							if ( pStaticRank != NULL  )
							{
								pStaticRank->SetShow( true );

								for ( int i = 0; i < RS_DEAD; i++ )
								{
									pStaticRank->GetPicture( i )->SetShow( false );
								}

								pStaticRank->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticRank->Move( pStaticRank->GetOffsetPos(), D3DXCOLOR(1,1,1,1), 0.5, true );
								pStaticRank->GetPicture( Get_UI_RANK_SCORE( (CX2DungeonRoom::RANK_TYPE)pDungeonResultUnitInfo->m_cComboRank ) )->SetShow( true );
							}

						}
					} break;
				case DRAO_TECHNICAL_SCORE:
					{
						if ( GetPressEnterKeyOrZKeyForSkip() )
							m_fTechnicalScore = static_cast<float>( pDungeonResultUnitInfo->m_nTechnicalScore );
						else
							m_fTechnicalScore += (fElapsedTime * DELTA_FOR_COMBO_AND_TECHNIQUE);

						if ( (int)m_fTechnicalScore >= pDungeonResultUnitInfo->m_nTechnicalScore )
						{
							m_fTechnicalScore = static_cast<float>( pDungeonResultUnitInfo->m_nTechnicalScore );
							m_DungeonResultAniOrder = DRAO_TIME;

							CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Technical_Rank" );
							if ( pStaticRank != NULL  )
							{
								pStaticRank->SetShow( true );
								for ( int i = 0; i < RS_DEAD; i++ )
								{
									pStaticRank->GetPicture( i )->SetShow( false );
								}

								pStaticRank->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticRank->Move( pStaticRank->GetOffsetPos(), D3DXCOLOR(1,1,1,1), 0.5, true );
								pStaticRank->GetPicture( Get_UI_RANK_SCORE( (CX2DungeonRoom::RANK_TYPE)pDungeonResultUnitInfo->m_cTechnicalRank ) )->SetShow( true );	
							}
						}
					} break;
				case DRAO_TIME:
					{
						if ( GetPressEnterKeyOrZKeyForSkip() )
						{
							m_fSec		= static_cast<float>( playTimeSec );
							m_fMinute	= static_cast<float>( playTimeMinute );
							m_fHour		= static_cast<float>( playTimeHour );
						}

						m_fSec += fElapsedTime* DELTA_FOR_TIME;

						if ( m_fSec >= playTimeSec )
						{
							m_fSec = (float)playTimeSec;

							m_fMinute += fElapsedTime * DELTA_FOR_TIME;

							if ( m_fMinute >= playTimeMinute )
							{
								m_fMinute = (float)playTimeMinute;

								m_fHour += fElapsedTime * DELTA_FOR_TIME;

								if ( m_fHour >= playTimeHour )
								{
									m_fHour = (float)playTimeHour;

									m_DungeonResultAniOrder = DRAO_DAMAGED;

									CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Time_Rank" );
									if ( pStaticRank != NULL )
									{
										pStaticRank->SetShow( true );
										for ( int i = 0; i < RS_DEAD; i++ )
										{
											pStaticRank->GetPicture( i )->SetShow( false );
										}

										pStaticRank->SetColor( D3DXCOLOR(1,1,1,0) );
										pStaticRank->Move( pStaticRank->GetOffsetPos(), D3DXCOLOR(1,1,1,1), 0.5, true );
										pStaticRank->GetPicture( Get_UI_RANK_SCORE( (CX2DungeonRoom::RANK_TYPE)pDungeonResultUnitInfo->m_cTimeRank ) )->SetShow( true );	
									}
								}
							}
						}
					} break;
				case DRAO_DAMAGED:
					{
						
						if ( GetPressEnterKeyOrZKeyForSkip() )
							m_fDamaged = static_cast<float>( pDungeonResultUnitInfo->m_nDamaged );
						else
							m_fDamaged += (fElapsedTime * DELTA_FOR_TIME);

						if ( (int)m_fDamaged >= pDungeonResultUnitInfo->m_nDamaged )
						{
							m_fDamaged = (float)pDungeonResultUnitInfo->m_nDamaged;
							m_DungeonResultAniOrder = DRAO_TOTAL_SCORE;

							CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Damaged_Rank" );
							if ( pStaticRank != NULL )
							{
								pStaticRank->SetShow( true );
								for ( int i = 0; i < RS_DEAD; i++ )
								{
									pStaticRank->GetPicture( i )->SetShow( false );
								}

								pStaticRank->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticRank->Move( pStaticRank->GetOffsetPos(), D3DXCOLOR(1,1,1,1), 0.5, true );
								pStaticRank->GetPicture( Get_UI_RANK_SCORE( (CX2DungeonRoom::RANK_TYPE)pDungeonResultUnitInfo->m_cDamagedRank ) )->SetShow( true );	
							}
						}
					} break;
				case DRAO_TOTAL_SCORE:
					{
						//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg" );

						m_fTotalScore += (fElapsedTime * DELTA_FOR_COMBO_AND_TECHNIQUE);

						if ( (int)m_fTotalScore >= pDungeonResultUnitInfo->m_nTotalScore )
						{
							m_fTotalScore = (float)pDungeonResultUnitInfo->m_nTotalScore;
							m_DungeonResultAniOrder = DRAO_TOTAL_SCORE_RANK;

							g_pKTDXApp->GetDeviceManager()->StopSound( L"Count.ogg" );
						}
					} break;
				case DRAO_TOTAL_SCORE_RANK:
					{
						if ( m_bUIMyTotalRankStart == false )
							m_fMyTotalRankWaitRemainTime -= fElapsedTime;

						if ( m_fMyTotalRankWaitRemainTime <= 0.0f )
						{
							m_bUIMyTotalRankStart = true;
						}

						if ( m_bUIMyTotalRankStart == true )
						{
							g_pKTDXApp->GetDeviceManager()->PlaySound( L"ResultStamp.ogg", false, false);

							//m_pParticleElMark	= g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"El_Mark", 0,0,0, 1000, 1000, 1, 1 );

							//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
							switch(pDungeonResultUnitInfo->m_cTotalRank)
							{
								//{{ kimhc // 2011-02-22 // 던전랭크 SS 등급 추가
				#ifdef	DUNGEON_RANK_NEW
							case CX2DungeonRoom::RT_SS:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_SS", 0,0,0, 1000, 1000, 1, 1 );
								} break;
				#endif	DUNGEON_RANK_NEW	
								//}} kimhc // 2011-02-22 // 던전랭크 SS 등급 추가

							case CX2DungeonRoom::RT_S:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_S", 0,0,0, 1000, 1000, 1, 1 );
								} break;
							case CX2DungeonRoom::RT_A:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_A", 0,0,0, 1000, 1000, 1, 1 );
								} break;
							case CX2DungeonRoom::RT_B:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_B", 0,0,0, 1000, 1000, 1, 1 );
								} break;
							case CX2DungeonRoom::RT_C:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_C", 0,0,0, 1000, 1000, 1, 1 );
								} break;
							case CX2DungeonRoom::RT_D:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_D", 0,0,0, 1000, 1000, 1, 1 );
								} break;
							case CX2DungeonRoom::RT_E:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_E", 0,0,0, 1000, 1000, 1, 1 );
								} break;
							case CX2DungeonRoom::RT_F:
								{
									m_hParticleRank = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"Rank_F", 0,0,0, 1000, 1000, 1, 1 );
								} break;
							default:
								break;
							}//}}
							
							m_DungeonResultAniOrder = DRAO_OTHERS_RANK;
						}
					} break;
				case DRAO_OTHERS_RANK:
					{
						int otherUserNum = 0;

						if ( g_pData->GetDungeonResultInfo() != NULL )
						{
							for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList.size(); i++ )
							{
								CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList[i];
								if ( NULL != pDungeonResultUnitInfo )
								{
									if ( g_pData->GetDungeonRoom()->GetMySlot() != NULL && pDungeonResultUnitInfo->m_UnitUID != g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUID() )
									{
										CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( pDungeonResultUnitInfo->m_UnitUID );
										if ( pSlotData != NULL && pSlotData->GetSlotState() != CX2Room::SS_CLOSE && pSlotData->GetSlotState() != CX2Room::SS_EMPTY )
										{
											WCHAR staticResultRank[256] = {0};

											StringCchPrintf( staticResultRank, 256, L"Dungeon_Result_Other%d_Rank", otherUserNum );
											//wsprintf( staticResultRank, L"Dungeon_Result_Other%d_Rank", otherUserNum );

											if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
											{
												//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
												ErrorLogMsg( XEM_ERROR23, "m_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
												m_bStartTimer = true;
												return S_OK;
											}


											CKTDGUIStatic* pStaticRank 
												= static_cast<CKTDGUIStatic*>( m_pDLGBack->GetControl( staticResultRank ) );									
											if ( NULL != pStaticRank )
											{
												pStaticRank->SetShow( true );
												for ( int i = 0; i < RS_DEAD; i++ )
												{
													pStaticRank->GetPicture( i )->SetShow( false );
												}

												pStaticRank->SetColor( D3DXCOLOR(1,1,1,0) );
												pStaticRank->Move( pStaticRank->GetOffsetPos(), D3DXCOLOR(1,1,1,1), 0.5, true );
												pStaticRank->GetPicture( Get_UI_RANK_SCORE( (CX2DungeonRoom::RANK_TYPE)pDungeonResultUnitInfo->m_cTotalRank ) )->SetShow( true );	
												otherUserNum++;
											}
										}

									}	// if
								}	// for
							}	// if
						}

						m_DungeonResultAniOrder = DRAO_WAIT_INFO_FADE_OUT;
					} break;
				case DRAO_WAIT_INFO_FADE_OUT:
					{
						if ( m_bWaitForFadeOut == true )
							m_fWaitForFadeOutRemainTime -= m_fElapsedTime;

						if ( m_fWaitForFadeOutRemainTime <= 0  )
						{
							//		m_DungeonResultAniOrder = DRAO_FIRST_RESULT_INFO_FADE_OUT;
							m_DungeonResultAniOrder = DRAO_ED_BG;

							if ( m_hParticleElMark != INVALID_PARTICLE_SEQUENCE_HANDLE )
								g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleElMark );

							if ( m_hParticleRank != INVALID_PARTICLE_SEQUENCE_HANDLE )
								g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hParticleRank );

							float moveChangeTime = 0.5f;

							CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"TotalScore_BG" );
							pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );
							pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Info" );
							pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );
							pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_TIME_AND_DAMAGED_Score_BG" );
							pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );
							pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Combo_Rank" );
							pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );
							pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Technical_Rank" );
							pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );
							pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Time_Rank" );
							pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );
							pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Damaged_Rank" );
							pStatic->Move( pStatic->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );

							CKTDGUIStatic* pStaticPartyRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Party_Rank" );
							pStaticPartyRank->Move( pStaticPartyRank->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), 0.2f, true );

							//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
				#ifdef	SERV_INSERT_GLOBAL_SERVER
							CKTDGUIStatic* pStaticDefenceDungeonResult = static_cast<CKTDGUIStatic*>( m_pDLGBack->GetControl( L"Defence_result" ) );
							if( NULL != pStaticDefenceDungeonResult )
								pStaticDefenceDungeonResult->Move( pStaticDefenceDungeonResult->GetOffsetPos(), D3DXCOLOR( 1, 1, 1, 0 ), moveChangeTime, true );
				#endif	SERV_INSERT_GLOBAL_SERVER
							//}} kimhc // 2011-04-18 // 국내, 디펜스 던전

							for ( int i = 0; i < 3; i++ )
							{
								//							WCHAR staticResultUnit[256] = {0};
								WCHAR staticResultRank[256] = {0};
								WCHAR staticResultDesc[256] = {0};
								WCHAR staticResultID[256] = {0};
								WCHAR staticResultPCRoom[256] = {0};

								//							wsprintf( staticResultUnit, L"Dungeon_Result_Other%d_Unit", i );

								StringCchPrintf( staticResultRank, 256, L"Dungeon_Result_Other%d_Rank", i );
								StringCchPrintf( staticResultDesc, 256, L"Dungeon_Result_Other%d_Desc", i );
								StringCchPrintf( staticResultID, 256, L"Dungeon_Result_Other%d_ID", i );
								StringCchPrintf( staticResultPCRoom, 256, L"Dungeon_Result_Other%d_PCRoom", i );

								//wsprintf( staticResultRank, L"Dungeon_Result_Other%d_Rank", i );
								//wsprintf( staticResultDesc, L"Dungeon_Result_Other%d_Desc", i );
								//wsprintf( staticResultID, L"Dungeon_Result_Other%d_ID", i );
								//wsprintf( staticResultPCRoom, L"Dungeon_Result_Other%d_PCRoom", i );

								/*
								CKTDGUIStatic* pStaticOtherUnit = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultUnit );
								pStaticOtherUnit->Move( pStaticOtherUnit->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );
								*/
								CKTDGUIStatic* pStaticOtherDesc = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultDesc );
								if ( pStaticOtherDesc != NULL )
									pStaticOtherDesc->Move( pStaticOtherDesc->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );

								CKTDGUIStatic* pStaticOtherRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultRank );
								if ( pStaticOtherDesc != NULL )
									pStaticOtherRank->Move( pStaticOtherRank->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );

								CKTDGUIStatic* pStaticOtherID = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultID );
								if ( pStaticOtherID != NULL )
									pStaticOtherID->Move( pStaticOtherID->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );

								CKTDGUIStatic* pStaticOtherPCRoom = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultPCRoom );
								if ( pStaticOtherPCRoom != NULL )
									pStaticOtherPCRoom->Move( pStaticOtherPCRoom->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );

							}
							/*
							int otherUserNum = 0;

							if ( g_pData->GetDungeonResultInfo() != NULL )
							{
							for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList.size(); i++ )
							{
							CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList[i];
							if ( pDungeonResultUnitInfo == NULL )
							continue;

							if ( g_pData->GetDungeonRoom()->GetMySlot() != NULL && pDungeonResultUnitInfo->m_UnitUID != g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUID() )
							{
							CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( pDungeonResultUnitInfo->m_UnitUID );
							if ( pSlotData == NULL )
							continue;

							if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
							continue;


							WCHAR staticResultUnit[256] = {0};
							WCHAR staticResultRank[256] = {0};
							WCHAR staticResultDesc[256] = {0};

							wsprintf( staticResultUnit, L"Dungeon_Result_Other%d_Unit", otherUserNum );
							wsprintf( staticResultRank, L"Dungeon_Result_Other%d_Rank", otherUserNum );
							wsprintf( staticResultDesc, L"Dungeon_Result_Other%d_Desc", otherUserNum );



							if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
							{
							//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
							ErrorLogMsg( XEM_ERROR23, "m_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
							m_bStartTimer = true;
							return S_OK;
							}


							CKTDGUIStatic* pStaticOtherUnit = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultUnit );
							pStaticOtherUnit->Move( pStaticOtherUnit->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );


							CKTDGUIStatic* pStaticOtherDesc = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultDesc );
							pStaticOtherDesc->Move( pStaticOtherDesc->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );

							CKTDGUIStatic* pStaticOtherRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultRank );
							pStaticOtherRank->Move( pStaticOtherRank->GetOffsetPos(), D3DXCOLOR( 1,1,1,0 ), moveChangeTime, true );

							}
							}
							*/

							/*							
							pSlotData->m_pUnitViewer->SetShowObject( false );


							g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), true );
							g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,500,-1000, 0,0,0 );
							g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( 1.0f );


							CKTDGUIStatic* pStaticReward_Choice_Item_Black = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Reward_Choice_Item_Black" );
							pStaticReward_Choice_Item_Black->SetShow( true );
							pStaticReward_Choice_Item_Black->SetColor( D3DXCOLOR(1,1,1,0) );
							pStaticReward_Choice_Item_Black->Move( pStaticReward_Choice_Item_Black->GetOffsetPos(), D3DXCOLOR(1,1,1,1), 1.0f, true );



							}
							*/

						}
					} break;
				case DRAO_FIRST_RESULT_INFO_FADE_OUT:
					{
						CKTDGUIStatic* pStaticReward_Choice_Item_Black = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Reward_Choice_Item_Black" );

						if ( pStaticReward_Choice_Item_Black->GetIsMoving() == false && m_bSendStartRewardReq == false )
						{
							m_bSendStartRewardReq = true;
							Handler_EGS_START_REWARD_BOX_SELECT_REQ();
						}

						if ( m_bStartRewardBoxSelectNot == true )
						{
							m_DungeonResultAniOrder = DRAO_CHOICE_REWARD;

							int playerNum = 0;
							for ( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
							{
								CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData(i);
								if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
								{
									playerNum++;
								}
							}

							WCHAR buttonChoiceItemNameBuff[256] = {0};

							for ( int i = 0; i < playerNum; i++ )
							{
								StringCchPrintf( buttonChoiceItemNameBuff, 256, L"Reward_Box_%d_%d", playerNum, i );
								//wsprintf( buttonChoiceItemNameBuff, L"Reward_Box_%d_%d", playerNum, i );
								m_pDLGBack->GetControl( buttonChoiceItemNameBuff )->SetEnable( true );
							}

							// 2009.01.19 김태완 : 코드정리 elseif->switch
							switch(playerNum)
							{
							case 1:
								{
									CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  0,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}
								} break;
							case 2:
								{
									CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  -130,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}


									pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  130,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}

								} break;
							case 3:
								{
									CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  -300,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}


									pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  0,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}


									pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  300,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}

								} break;
							case 4:
								{
									CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  -400,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}


									pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  -130,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}


									pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  130,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}


									pMeshInstance = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"RewardItemBox",  400,0,0, 0,0,0, 0,0,0 );
									if( NULL != pMeshInstance )
									{
										m_vecRewardXMeshInst.push_back( pMeshInstance->GetHandle() );							
									}

								} break;							
							default:
								{
									ASSERT( !"Unexpected PlayerNum in DungeonResult : Player more then 4 case or less then 0 case" );
								}
								break;
							}//}}
							

							CKTDGUIStatic* pStaticDungeon_Reward_Choice_Item = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Reward_Choice_Item" );
							pStaticDungeon_Reward_Choice_Item->SetShow(true);
							pStaticDungeon_Reward_Choice_Item->SetColor( D3DXCOLOR(1,1,1,0) );
							pStaticDungeon_Reward_Choice_Item->Move( pStaticDungeon_Reward_Choice_Item->GetOffsetPos(), D3DXCOLOR(1,1,1,1), 0.5f, true );
						}
					} break;
				case DRAO_CHOICE_REWARD:
					{
						//m_DungeonResultAniOrder = DRAO_END;
					} break;
				case DRAO_ANIMATE_BOX:
					{
						m_fAnimatBoxTime += fElapsedTime;

						if ( m_fAnimatBoxTime >= 3.0f )
						{
							for ( int i = 0; i < (int)m_vecRewardXMeshInst.size(); i++ )
							{
								CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshInst = m_vecRewardXMeshInst[i];
								g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( hMeshInst );
							}
							m_vecRewardXMeshInst.clear();


							g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
							g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
								g_pKTDXApp->GetDGManager()->GetFar(), false );


							CKTDGUIStatic* pStaticReward_Choice_Item_Black = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Reward_Choice_Item_Black" );
							pStaticReward_Choice_Item_Black->Move( pStaticReward_Choice_Item_Black->GetOffsetPos(), D3DXCOLOR(1,1,1,0), 1.0f, true );

							CKTDGUIStatic* pStaticDungeon_Reward_Choice_Item = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Reward_Choice_Item" );
							pStaticDungeon_Reward_Choice_Item->Move( pStaticDungeon_Reward_Choice_Item->GetOffsetPos(), D3DXCOLOR(1,1,1,0), 0.5f, true );

							m_DungeonResultAniOrder = DRAO_ED_BG;
						}
					} break;
				case DRAO_ED_BG:
					{
						CKTDGUIStatic* pStaticReward_Choice_Item_Black = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Reward_Choice_Item_Black" );
						if ( pStaticReward_Choice_Item_Black->GetIsMoving() == false )
						{
							if ( m_pDLGFront->GetControl( L"Result_Title" ) != NULL )
								m_pDLGFront->GetControl( L"Result_Title" )->SetShow( false );

							if ( m_pDLGFront->GetControl( L"Reward_Title" ) != NULL )
								m_pDLGFront->GetControl( L"Reward_Title" )->SetShow( true );


							if( NULL != pSlotData && NULL != pSlotData->m_pUnitViewer )
							{
								pSlotData->m_pUnitViewer->SetShowObject( true );

								if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
								{
									pSlotData->m_pUnit->SetIsLevelUp( false );
									//pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLevelUp", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
									pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_LEVEL_UP );

									CKTDGXMeshPlayer::CXMeshInstance* pInst = NULL;
									pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp01_1", 
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().x,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().y - 25.0f,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().z, 
										0,0,0, 0,0,0 );
									pInst->SetDelayTime( 0.3f );
									pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp01",
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().x,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().y - 25.0f,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().z, 
										0,0,0, 0,0,0 );
									pInst->SetDelayTime( 0.3f );
									pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp04",
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().x,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().z, 
										0,0,0, 0,0,0 );
									pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02", 
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
										0,0,0, 0,0,0 );
									pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02", 
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
										0,0,0, 0,0,0 );
									pInst->SetDelayTime( 0.1f );
									pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02",
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
										0,0,0, 0,0,0 );
									pInst->SetDelayTime( 0.2f );
									pInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  L"LobbyLevelUp02",
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().x + 5.0f,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().y,
										pSlotData->m_pUnitViewer->GetMatrix().GetPos().z + 30.0f, 
										0,0,0, 0,0,0 );
									pInst->SetDelayTime( 0.3f );

									g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUp.ogg", false, false );

									m_fPlayBGM = -1;

									//if( g_pKTDXApp->GetDSManager()->GetInit() == true && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL )
									//	g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( L"LevelUpBGM.ogg" );

									g_pKTDXApp->GetDeviceManager()->PlaySound( L"LevelUpBGM.ogg", false, false );

									g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LevelUp", 233, 458, 0, 1000, 1000, 1, 1, 1 );
									CKTDGParticleSystem::CParticleEventSequence* pPartInst = NULL;
									pPartInst = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LobbyLevelUp01", pSlotData->m_pUnitViewer->GetMatrix().GetPos() );
									pPartInst->SetLatency( 2.266f );
								}
								else
								{	
									pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
								}
							}


							CKTDGUIStatic* pStaticEDBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"StaticReward_ED" );
							pStaticEDBG->SetShow( true );
							pStaticEDBG->SetColor( D3DXCOLOR(1,1,1,0) );
							pStaticEDBG->SetOffsetPos( D3DXVECTOR2( pStaticEDBG->GetOffsetPos().x - 70, pStaticEDBG->GetOffsetPos().y ) );
							pStaticEDBG->Move( D3DXVECTOR2( pStaticEDBG->GetOffsetPos().x + 70, pStaticEDBG->GetOffsetPos().y ), D3DXCOLOR(1,1,1,1), 0.2f, true );

							m_DungeonResultAniOrder = DRAO_ITEM_BG;
						}
					} break;
				case DRAO_ITEM_BG:
					{
						CKTDGUIStatic* pStaticReward_Choice_Item_Black = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"StaticReward_ED" );
						if ( pStaticReward_Choice_Item_Black->GetIsMoving() == false )
						{
							CKTDGUIStatic* pStaticItemBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Item_BG" );
							pStaticItemBG->SetShow( true );
							pStaticItemBG->SetColor( D3DXCOLOR(1,1,1,0) );
							pStaticItemBG->SetOffsetPos( D3DXVECTOR2( pStaticItemBG->GetOffsetPos().x - 70, pStaticItemBG->GetOffsetPos().y ) );
							pStaticItemBG->Move( D3DXVECTOR2( pStaticItemBG->GetOffsetPos().x + 70, pStaticItemBG->GetOffsetPos().y ), D3DXCOLOR(1,1,1,1), 0.2f, true );

							int iCount = 7;

							const int iSizeOfPicture = pStaticItemBG->GetPictureNum();
							for( UINT i=0; i < g_pMain->GetDungeonRewardItem().size(); i++ )
							{
								if( iCount > iSizeOfPicture )
									break;

								const KItemInfo& itemInfo = g_pMain->GetDungeonRewardItem().at(i);

                                wstring wTexName = g_pData->GetItemManager()->GetItemTemplet( itemInfo.m_iItemID )->GetShopImage();
								if( g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->IsValidFile( wTexName.c_str() ) == false )
									wTexName = L"HQ_Shop_Ui_Noimage.dds";

								string texName = "";
								ConvertWCHARToChar( texName, wTexName.c_str() );
								ASSERT( NULL != pStaticItemBG->GetPicture(iCount) );
								if( pStaticItemBG->GetPicture(iCount) != NULL )
								{
									pStaticItemBG->GetPicture(iCount)->SetTex_LUA2( texName.c_str() );

									if( itemInfo.m_iQuantity > 1 )
									{
										CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
										pStaticItemBG->AddString( pString );
										pString->fontIndex = XUF_DODUM_15_BOLD;
										pString->color = D3DXCOLOR( 1,1,1,1 );
										pString->outlineColor = D3DXCOLOR( 0,0,0,1 );
										WCHAR buff[256] = {0};
										StringCchPrintf( buff, 256, L"%d", itemInfo.m_iQuantity );
										//wsprintf( buff, L"%d", itemInfo.m_iQuantity );
										pString->msg = buff;
										pString->fontStyle = CKTDGFontManager::FS_SHELL;
										pString->sortFlag = DT_RIGHT;
										pString->pos = D3DXVECTOR2( pStaticItemBG->GetPicture(iCount)->GetPos().x +  pStaticItemBG->GetPicture(iCount)->GetSize().x - 20,
											pStaticItemBG->GetPicture(iCount)->GetPos().y +  pStaticItemBG->GetPicture(iCount)->GetSize().y - 20  );
									}
								}

								iCount++;
							}						

							m_DungeonResultAniOrder = DRAO_EXP_BG;
						}
					} break;
				case DRAO_EXP_BG:
					{
						if ( m_pDLGBack->GetControl( L"Dungeon_Result_Item_BG" )->GetIsMoving() == false )
						{
							CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
							if ( pStaticEXPBG != NULL )
							{
								pStaticEXPBG->SetShow( true );
								pStaticEXPBG->SetColor( D3DXCOLOR(1,1,1,0) );
								pStaticEXPBG->SetOffsetPos( D3DXVECTOR2( pStaticEXPBG->GetOffsetPos().x - 70, pStaticEXPBG->GetOffsetPos().y ) );
								pStaticEXPBG->Move( D3DXVECTOR2( pStaticEXPBG->GetOffsetPos().x + 70, pStaticEXPBG->GetOffsetPos().y ), D3DXCOLOR(1,1,1,1), 0.2f, true );

								//EXP setting

								D3DXVECTOR2 picOrgSize = pStaticEXPBG->GetPicture( (pStaticEXPBG->GetPictureNum() - 1) )->GetOriginalSize();
								CKTDGUIControl::CPictureData* pPictureEXP = pStaticEXPBG->GetPicture( (pStaticEXPBG->GetPictureNum() - 1) );

								int nowExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP;
								int nowBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NowBaseLevelEXP;
								int nextBaseExp = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NextBaseLevelEXP;

								pPictureEXP->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );

								if ( pStaticEXPBG->GetString(0) != NULL )
								{
									WCHAR buff[256] = {0};
									swprintf( buff, L"%d / %d (%.1f%%)", nowExp - nowBaseExp, nextBaseExp - nowBaseExp, (float)(nowExp - nowBaseExp)/(nextBaseExp - nowBaseExp)*100.0f );
									pStaticEXPBG->GetString(0)->msg = buff;
								}
								/*
								int nowExp = pSlotData->m_pUnit->GetPrevEXP();
								int nowBaseExp = pSlotData->m_pUnit->GetPrevNowBaseLevelEXP();
								int nextBaseExp = pSlotData->m_pUnit->GetPrevNextBaseLevelEXP();

								m_fEXPSize = (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x;

								pStaticEXPBG->GetPicture(27)->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );
								pStaticEXPBG->GetPicture(28)->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );

								pStaticEXPBG->GetPicture(27)->SetSizeX( (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x );
								*/


								m_DungeonResultAniOrder = DRAO_ED;

								//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", true );
							}
						}
					} break;
				case DRAO_ED:
					{
						CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
						if ( pStaticEXPBG->GetIsMoving() == false )
						{
							//{{ kimhc // 실시간 엘소드 중 실시간 ED 획득
						#ifdef	REAL_TIME_ELSWORD
							const int iED 
								= ( g_pMain->GetIsPlayingTutorial() ?
								static_cast<int>( pDungeonResultUnitInfo->m_nED ) : g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED - pDungeonResultUnitInfo->m_nOldED );
							
							if ( GetPressEnterKeyOrZKeyForSkip() )
								m_fED = static_cast<float>( iED );
							else
								m_fED += (fElapsedTime * DELTA_FOR_ED_AND_EXP);

							if ( m_fED >= static_cast< float >( iED ) )
							{
								m_fED = static_cast< float >( iED );
								m_DungeonResultAniOrder = DRAO_ITEM;
							}							
						#else	REAL_TIME_ELSWORD
							if ( m_fED >= (float)pDungeonResultUnitInfo->m_nED )
							{
								m_fED = (float)pDungeonResultUnitInfo->m_nED;

								//g_pKTDXApp->GetDeviceManager()->StopSound( L"Count.ogg" );

								m_DungeonResultAniOrder = DRAO_ITEM;

							}
						#endif	REAL_TIME_ELSWORD
							//}} kimhc // 실시간 엘소드 중 실시간 ED 획득
							
							/*
							WCHAR edBuff[256] = {0};
							wsprintf( edBuff, L"%d", (int)m_fED );
							CKTDGUIStatic* pStaticED = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"StaticReward_ED" );
							pStaticED->GetString(0)->msg = edBuff;
							*/
						}
					} break;
				case DRAO_ITEM:
					{
						//////////////////////////////////////////////////////////////////////////
						// At Next EXP Page
						int playerNum = 0;
						for ( int i = 0; i < g_pX2Room->GetSlotNum(); i++ )
						{
							CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData(i);
							if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
							{
								playerNum++;
							}
						}

						//WCHAR partyPayoutBuff[256] = {0};

						//if ( playerNum == 2 )
						//{
						//wsprintf( partyPayoutBuff, L"파티 분배율 : 50%%" );
						//}
						//else if ( playerNum == 3 )
						//{
						//wsprintf( partyPayoutBuff, L"파티 분배율 : 33%%" );
						//}
						//else if ( playerNum == 4 )
						//{
						//wsprintf( partyPayoutBuff, L"파티 분배율 : 25%%" );
						//}

						//CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
						//pStaticEXPBG->GetString(0)->msg = partyPayoutBuff;

						m_DungeonResultAniOrder = DRAO_BONUS_EXP;

						//g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", true );
						//////////////////////////////////////////////////////////////////////////

					} break;
				case DRAO_BONUS_EXP:
					{


						if ( m_bCheckBonusEXP == false )
						{
							m_bCheckBonusEXP = true;

							// here stamp effect
							CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
							if ( pStaticEXPBG != NULL )
							{
								CX2StateDungeonResult::BonusEXPData bonusExpData;
								if ( pDungeonResultUnitInfo->m_nClearBonusEXP > 0 )
								{
									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_CLEAR;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nClearBonusEXP;
									m_vecBonusExpType.push_back( bonusExpData );

								}
								if ( pDungeonResultUnitInfo->m_nRankBonusEXP > 0 )
								{
									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_RANK;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nRankBonusEXP;
									m_vecBonusExpType.push_back( bonusExpData );

								}
								if ( pDungeonResultUnitInfo->m_nPartyBonusEXP > 0 )
								{

									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_PARTY;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nPartyBonusEXP;
									m_vecBonusExpType.push_back( bonusExpData );


								}
								if ( pDungeonResultUnitInfo->m_nMvpBonusEXP > 0 )
								{

									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_MVP;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nMvpBonusEXP;
									m_vecBonusExpType.push_back( bonusExpData );

								}
								if ( pDungeonResultUnitInfo->m_nGameBangBonusEXP > 0 )
								{

									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_GAMEBANG;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nGameBangBonusEXP;
									m_vecBonusExpType.push_back( bonusExpData );

								}
								if ( pDungeonResultUnitInfo->m_nPremiumBonusEXP > 0 )
								{

									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_PREMIUM;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nPremiumBonusEXP;
									m_vecBonusExpType.push_back( bonusExpData );

								}
								if ( pDungeonResultUnitInfo->m_nEventBonusExp > 0 )
								{

									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_EVENT;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nEventBonusExp;
									m_vecBonusExpType.push_back( bonusExpData );
								}
								if ( pDungeonResultUnitInfo->m_nChannelBonusEXP > 0 )
								{

									bonusExpData.bonusExpType = CX2StateDungeonResult::BET_CHANNEL_BONUS;
									bonusExpData.bonusExp = pDungeonResultUnitInfo->m_nChannelBonusEXP;
									m_vecBonusExpType.push_back( bonusExpData );
								}
								

								/*
								if ( g_pData->GetMyUser()->GetIsPCRoom() == true )
								{
								if ( pStaticEXPBG->GetPicture( 23) != NULL )
								pStaticEXPBG->GetPicture( 23 )->SetShow( true );
								}
								else
								{
								if ( pStaticEXPBG->GetPicture( 23) != NULL )
								pStaticEXPBG->GetPicture( 23 )->SetShow( false );
								}
								*/
							}
						}

						//m_DungeonResultAniOrder = DRAO_BONUS_EXP;

						m_fTimeBonusEXPShowEvent += fElapsedTime;

						if ( m_fTimeBonusEXPShowEvent > 0.4f )
						{
							m_fTimeBonusEXPShowEvent = 0.0f;

							if ( m_vecBonusExpType.empty() == true )
								m_DungeonResultAniOrder = DRAO_EXP;
							else
							{
								CX2StateDungeonResult::BonusEXPData bonuseEXPData = m_vecBonusExpType[0];
								ShowBonusEXPEvent( bonuseEXPData.bonusExpType, bonuseEXPData.bonusExp );
								m_vecBonusExpType.erase( m_vecBonusExpType.begin() );
							}
						}
					} break;
				case DRAO_EXP:
					{
						m_fEXP += (fElapsedTime * DELTA_FOR_ED_AND_EXP);

						//{{ kimhc // 실시간 엘소드 중 실시간 경험치 획득
				#ifdef	REAL_TIME_ELSWORD

						const int iEXP 
							= ( g_pMain->GetIsPlayingTutorial() ?
							static_cast<int>( pDungeonResultUnitInfo->GetTotalExp() ) : g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_EXP - pDungeonResultUnitInfo->m_nOldEXP );

						if ( GetPressEnterKeyOrZKeyForSkip() )
							m_fEXP = static_cast<float>( iEXP );
						else
						{
							m_fEXP += (fElapsedTime * DELTA_FOR_ED_AND_EXP);
							g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", false, false );
						}

						if ( m_fEXP >= static_cast< float >( iEXP ) )
						{
							m_fEXP = static_cast< float >( iEXP );

							m_DungeonResultAniOrder = DRAO_EXP_BAR;
						}						

				#else	REAL_TIME_ELSWORD
						if ( m_fEXP < (float)( pDungeonResultUnitInfo->GetTotalExp() ) ) 
							g_pKTDXApp->GetDeviceManager()->PlaySound( L"Count.ogg", false, false );

						if ( m_fEXP >= (float)( pDungeonResultUnitInfo->GetTotalExp() ) )
						{
							m_fEXP = (float)( pDungeonResultUnitInfo->GetTotalExp() );

							m_DungeonResultAniOrder = DRAO_EXP_BAR;
						}
				#endif	REAL_TIME_ELSWORD
						//}} kimhc // 실시간 엘소드 중 실시간 경험치 획득
							
					} break;
				case DRAO_EXP_BAR:
					{
						/*
						//EXP setting
						CKTDGUIStatic* pStaticEXPBG = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
						D3DXVECTOR2 picOrgSize = pStaticEXPBG->GetPicture(27)->GetOriginalSize();

						int nowExp = pSlotData->m_pUnit->GetUnitData().m_EXP;
						int nowBaseExp = pSlotData->m_pUnit->GetUnitData().m_NowBaseLevelEXP;
						int nextBaseExp = pSlotData->m_pUnit->GetUnitData().m_NextBaseLevelEXP;



						m_fEXPSize += (fElapsedTime*X2_MAGIC_MULTY_NUM4);

						float maxSize = 0;
						if ( pSlotData->m_pUnit->GetPrevLevel() == pSlotData->m_pUnit->GetUnitData().m_Level )
						{
						maxSize = (float)(nowExp - nowBaseExp) / (float)( nextBaseExp - nowBaseExp ) * picOrgSize.x;
						}
						else
						{
						maxSize = picOrgSize.x;
						}



						if ( m_fEXPSize >= maxSize )
						{
						m_fEXPSize = maxSize;

						if ( pSlotData->m_pUnit->GetPrevLevel() < pSlotData->m_pUnit->GetUnitData().m_Level )
						{
						m_fEXPSize = 0;
						//pStaticEXPBG->GetPicture(28)->SetSizeX( 0 );
						pSlotData->m_pUnit->SetPrevLevel( pSlotData->m_pUnit->GetPrevLevel() + 1 );
						}
						else
						{
						m_DungeonResultAniOrder = DRAO_END;
						g_pKTDXApp->GetDeviceManager()->StopSound( L"EXPUp.ogg" );
						}
						}

						pStaticEXPBG->GetPicture(27)->SetSizeX( m_fEXPSize );
						*/

						pSlotData->m_pUnit->SetPrevLevel( pSlotData->m_pUnit->GetUnitData().m_Level);

						m_DungeonResultAniOrder = DRAO_END;
					} break;

#ifdef SERV_DUNGEON_MVP
				case DRAO_PICKED_MVP_PLAYER :// MVP UI, Particle 지연 시간이 필요할 지 모르므로 case 로 따로 뺌
				{
					CX2DungeonGame::DungeonResultInfo* pDungeonResultInfo = g_pData->GetDungeonResultInfo();
					
					
					if ( pDungeonResultInfo != NULL )
					{
						UidType m_UidPickedMVPPlayer = pDungeonResultInfo->GetMvpPlayerUID();			
						UidType m_UidMyUser = 0;

						if ( NULL != g_pData->GetDungeonRoom() &&
							 NULL != g_pData->GetDungeonRoom()->GetMySlot() &&
							 NULL != g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit )
								m_UidMyUser = g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUID();
						
						
						if ( m_UidPickedMVPPlayer > 0 && m_UidMyUser > 0 )		// PickedMVPPlayer 는 MVP 로 선택된 캐릭터가 있음!
						{
							if ( m_UidMyUser == m_UidPickedMVPPlayer ) // 내가 MVP 일 때
							{		
#ifdef FIXED_DUNGEON_RESULT_STATE_MVP_PARTICLE // 던전 결과창에서 자기 자신이 MVP 로 선정 시,  RESULT 에 MVP 파티클이 가려지는 문제 수정 kimjh
								

								CKTDGParticleSystem::CParticleEventSequence*  pParticle1 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P01", 300, 70, 0);//, 1000, 1000, 1, 1, 1 );
								if ( NULL != pParticle1 ) 
									pParticle1->SetOverUI( true );		
								CKTDGParticleSystem::CParticleEventSequence*  pParticle2 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P02", 300, 70, 0, 1000, 1000, 1, 1, 1 );
								if ( NULL != pParticle2 ) 
									pParticle2->SetOverUI( true );		
								CKTDGParticleSystem::CParticleEventSequence*  pParticle3 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P03", 300, 70, 0, 1000, 1000, 1, 1, 1 );
								if ( NULL != pParticle3 ) 
									pParticle3->SetOverUI( true );		
								CKTDGParticleSystem::CParticleEventSequence*  pParticle4 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P04", 300, 70, 0, 1000, 1000, 1, 1, 1 );
								if ( NULL != pParticle4 ) 
									pParticle4->SetOverUI( true );		
								CKTDGParticleSystem::CParticleEventSequence*  pParticle5 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P05", 300, 70, 0, 1000, 1000, 1, 1, 1 );
								if ( NULL != pParticle5 ) 
									pParticle5->SetOverUI( true );		

#else // FIXED_DUNGEON_RESULT_STATE_MVP_PARTICLE // 던전 결과창에서 자기 자신이 MVP 로 선정 시,  RESULT 에 MVP 파티클이 가려지는 문제 수정 kimjh
								g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P01", 300, 70, 0);//, 1000, 1000, 1, 1, 1 );
								g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P02", 300, 70, 0, 1000, 1000, 1, 1, 1 );
								g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P03", 300, 70, 0, 1000, 1000, 1, 1, 1 );
								g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P04", 300, 70, 0, 1000, 1000, 1, 1, 1 );
								g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_Self_P05", 300, 70, 0, 1000, 1000, 1, 1, 1 );
#endif // FIXED_DUNGEON_RESULT_STATE_MVP_PARTICLE // 던전 결과창에서 자기 자신이 MVP 로 선정 시,  RESULT 에 MVP 파티클이 가려지는 문제 수정 kimjh
							}
							else					// 다른 유저가 MVP 일 때
							{
								int iOtherUserNum = 0;
								for ( int i = 0; i < static_cast <int> ( pDungeonResultInfo->m_DungeonResultUnitInfoList.size() ); i++ )
								{
									CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = pDungeonResultInfo->m_DungeonResultUnitInfoList[i];
																		
									if ( NULL != g_pData->GetDungeonRoom() &&
										NULL != g_pData->GetDungeonRoom()->GetSlotDataByUnitUID( pDungeonResultUnitInfo->m_UnitUID ) ) 
									{
										if ( pDungeonResultUnitInfo->m_UnitUID == m_UidPickedMVPPlayer )
										{									
											CKTDGUIStatic* pStaticPartyRank = static_cast<CKTDGUIStatic*> ( m_pDLGBack->GetControl( L"Dungeon_Result_Party_Rank" ) );
											// 해당 Player 의 사진이 없으면 뿌리지 않는다.
											if ( pStaticPartyRank != NULL && pStaticPartyRank->GetPicture( static_cast <int> ( iOtherUserNum ) ) != NULL )
											{
												D3DXVECTOR2 PicturePos = pStaticPartyRank->GetPicture( static_cast <int> ( iOtherUserNum ) )->GetPos();
												D3DXVECTOR2 vecCenterPos = pStaticPartyRank->GetPicture( iOtherUserNum )->GetPos();
												D3DXVECTOR2 vecPictureSize = pStaticPartyRank->GetPicture( iOtherUserNum )->GetSize();
												D3DXVECTOR2 vecLeftTopPos = vecCenterPos;
												vecLeftTopPos.x = vecLeftTopPos.x + ( vecPictureSize.x / 2 );
												vecLeftTopPos.y = vecLeftTopPos.y + ( vecPictureSize.y / 2 );

												// 140 ( UI 간격 ) * UI 뿌릴 사진 번호 + 13 ( X 축 고정 값 ), 테스트 목적으로 무조건 1번째 플레이어에게! 
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P01", vecLeftTopPos.x, vecLeftTopPos.y, 0, 1000, 1000, 1, 1, 1 );
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P02", vecLeftTopPos.x, vecLeftTopPos.y, 0, 1000, 1000, 1, 1, 1 );
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P03", vecLeftTopPos.x, vecLeftTopPos.y, 0, 1000, 1000, 1, 1, 1 );
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P04", vecLeftTopPos.x, vecCenterPos.y, 0, 1000, 1000, 1, 1, 1 );
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P05", vecLeftTopPos.x, vecCenterPos.y, 0, 1000, 1000, 1, 1, 1 );
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P06", vecLeftTopPos.x, vecLeftTopPos.y, 0, 1000, 1000, 1, 1, 1 );
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P07", vecLeftTopPos.x, vecLeftTopPos.y, 0, 1000, 1000, 1, 1, 1 );
												g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"MVP_UI_People_P08", vecLeftTopPos.x, vecLeftTopPos.y, 0, 1000, 1000, 1, 1, 1 );
											}
										}
										if ( pDungeonResultUnitInfo->m_UnitUID != m_UidMyUser )
											iOtherUserNum += 1;
									}
								}
							}
						}
						// 일단은 바로 변경, 추후 시간이 필요하면 ElapsedTime 사용해서 지연 시킬 것
						m_DungeonResultAniOrder = DRAO_TIME_AND_DAMAGED_SCORE_BG;	
					}
				}
				break;
#endif // SERV_DUNGEON_MVP
				case DRAO_END:
					{
						if( NULL != pSlotData &&
							NULL != pSlotData->m_pUnitViewer && 
							NULL != pSlotData->m_pUnitViewer->GetXSkinAnim() )
						{
							if ( pSlotData->m_pUnitViewer->GetXSkinAnim()->GetPlayType() == CKTDGXSkinAnim::XAP_ONE_WAIT )
							{
								if ( pSlotData->m_pUnitViewer->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAS_WAIT )
								{
									m_bStartTimer = true;
								}
							}
							else
							{
								m_bStartTimer = true;
							}
						}
					} break;

				default:
					break;

				}//}}

				break;
			}
		}


		PicCharDrawText();

	}
	


	return S_OK;
}

HRESULT CX2StateDungeonResult::OnFrameRender()
{
	if ( m_bInitUIFail == true )
		return S_OK;

	CX2State::OnFrameRender();


	return S_OK;
}

bool CX2StateDungeonResult::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2State::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch( wParam )
	{
		case DRUCM_CHOICE_ITEM_BOX:
			{
				if ( m_DungeonResultAniOrder == DRAO_CHOICE_REWARD )
				{
					CKTDGUIButton* pButtonItem = (CKTDGUIButton*)lParam;
					wstring buttonName = pButtonItem->GetName();
					WCHAR wcharChoiceID = buttonName[ buttonName.size() - 1 ];
					wstring wstrChoiceID = L"";
					wstrChoiceID += wcharChoiceID;
					int choiceID = _wtoi( wstrChoiceID.c_str() );
					m_ChoiceItemID = choiceID;

					Handler_EGS_SELECT_REWARD_BOX_REQ( m_ChoiceItemID );
/*
					if ( m_ChoiceItemID < 0 || m_ChoiceItemID >= (int)m_vecRewardXMeshInst.size() )
					{

					}
					else
					{
						CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = m_vecRewardXMeshInst[m_ChoiceItemID];
						pMeshInst->SetGlobalTime( 1999.9f );
					}


					m_DungeonResultAniOrder = DRAO_ANIMATE_BOX;
					*/
				}	
				
			}
			break;


		case DRUCM_GO_TRAINING_SCHOOL_OK:
			{
				DialogLog("tutorial result --> training school");

				// 훈련소로 바로가기
				//CX2StateAutoChanger::TARGET_DETAIL targetDetail;
				//targetDetail.m_iTrainingSchoolMode = (int) CX2StateTrainingSchool::TSM_TRAINING;
				//g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_TRAINING_SCHOOL, targetDetail );

				// 던전 초심자숲으로 바로가기
				CX2StateAutoChanger::TARGET_DETAIL targetDetail;
				targetDetail.m_iChannelID = (int) 104;		// note!!! 채널번호 일단 하드코딩
				targetDetail.m_iDungeonID = (int) SEnum::DI_EL_FOREST_GATE_NORMAL;
				targetDetail.m_iRoomUID = -1;

				g_pMain->GetStateAutoChanger().StartStateChange( (int)g_pMain->GetNowStateID(), CX2Main::XS_DUNGEON_GAME, targetDetail, 
					g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
			}
			break;

		case DRUCM_GO_TRAINING_SCHOOL_CANCEL:
			{
				g_pMain->SetIsExitingTutorial( true );

				DialogLog("XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_ROOM, NULL, false");
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_553 ) );
				//g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_ROOM, NULL, false );
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, NULL, false );	

			}
			break;

		case DRUCM_ESCAPE_DUNGEON_RESULT_OK:
			{
				CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>( lParam );
				if ( NULL != pControl )
				{
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

#ifdef FIX_TUTORIAL_FAIL_BUG
					if( true == g_pMain->GetIsPlayingTutorial() )
					{
						g_pMain->SetIsPlayingTutorial( false );
						g_pMain->SetIsExitingTutorial( true );
					}
#endif // FIX_TUTORIAL_FAIL_BUG

					if ( NULL != g_pX2Room )
						g_pX2Room->Handler_EGS_LEAVE_ROOM_REQ();
// 					CX2State* pState = static_cast<CX2State*>( g_pMain->GetNowState() );
// 					if ( NULL != pState )
// 						pState->ReturnToPlaceWhereBeforeDungeonStart();
				}

			} break;

		case DRUCM_ESCAPE_DUNGEON_RESULT_CANCEL:
			{
				CKTDGUIControl* pControl = reinterpret_cast<CKTDGUIControl*>( lParam );
				if ( NULL != pControl )
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			} break;

	}

	return false;
}


bool CX2StateDungeonResult::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateCommonBG::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

#ifdef BUFF_TEMPLET_SYSTEM	
#ifdef COME_BACK_REWARD
	if ( NULL !=g_pData->GetPremiumBuffManager() )
	{
		if ( true == g_pData->GetPremiumBuffManager()->UIServerEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif COME_BACK_REWARD
#endif BUFF_TEMPLET_SYSTEM

	switch( wParam )
	{
	//case EGS_REFRESH_ROOM_ACK:
	//	{
	//		return Handler_EGS_REFRESH_ROOM_ACK( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;

	case EGS_RESULT_SUCCESS_ACK:
		{
			return Handler_EGS_RESULT_SUCCESS_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_RESULT_ALL_UNIT_SUCCESS_NOT:
		{
			return Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_START_REWARD_BOX_SELECT_ACK:
		{
			return Handler_EGS_START_REWARD_BOX_SELECT_ACK();
		}
		break;

	case EGS_START_REWARD_BOX_SELECT_NOT:
		{
			return Handler_EGS_START_REWARD_BOX_SELECT_NOT();
		}
		break;

	case EGS_SELECT_REWARD_BOX_ACK:
		{
			return Handler_EGS_SELECT_REWARD_BOX_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_SELECT_REWARD_BOX_NOT:
		{
			return Handler_EGS_SELECT_REWARD_BOX_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_LEAVE_ROOM_ACK:
		{
			return Handler_EGS_LEAVE_ROOM_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_LEAVE_ROOM_NOT:
		{
			return Handler_EGS_LEAVE_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_BAN_USER_NOT:
		{
			g_pX2Room->SetBan( true );
			return true;
		}
		break;

	case EGS_JOIN_ROOM_NOT:
		{
			return Handler_EGS_JOIN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
	}

	return false;
}



bool CX2StateDungeonResult::Handler_EGS_RESULT_SUCCESS_REQ()
{
	DialogLog( "CX2StateDungeonResult::Handler_EGS_RESULT_SUCCESS_REQ Start" );


	KEGS_RESULT_SUCCESS_REQ kPacket;

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	// 맵에 가지고 있는 GamePlayNetworkInfo 를 vector에 담는다.
	if ( NULL != g_pX2Room )
		g_pX2Room->MoveGamePlayNetworkInfoFromMapToVector( kPacket.m_kGamePlayNetworkInfo.m_vecGamePlayNetworkInfo );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

	g_pData->GetServerProtocol()->SendPacket( EGS_RESULT_SUCCESS_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESULT_SUCCESS_ACK );

	DialogLog( "CX2StateDungeonResult::Handler_EGS_RESULT_SUCCESS_REQ End" );

	return true;
}

bool CX2StateDungeonResult::Handler_EGS_RESULT_SUCCESS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_ACK Start" );

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESULT_SUCCESS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_RESULT_SUCCESS_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_ACK End" );

			return true;
		}
		else
		{
			if ( kEvent.m_iOK == NetError::NOT_LEAVE_ROOM_REASON_14 )
			{
				g_pData->GetDungeonRoom()->SetNotEnoughSpirit( true );
			}
		}
	}
	return false;
}

bool CX2StateDungeonResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT Start" );

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

#ifdef FIX_TUTORIAL_FAIL_BUG
	if( true == g_pMain->GetIsPlayingTutorial() )
	{
		g_pMain->SetIsPlayingTutorial( false );
		g_pMain->SetIsExitingTutorial( true );
	}
#endif // FIX_TUTORIAL_FAIL_BUG

	CX2State* pState = (CX2State*)g_pMain->GetNowState();
	if( pState != NULL )
		pState->ReturnToPlaceWhereBeforeDungeonStart();

	DialogLog( "CX2StateDungeonResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT End" );
	return true;
}


bool CX2StateDungeonResult::Handler_EGS_START_REWARD_BOX_SELECT_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_START_REWARD_BOX_SELECT_REQ );
	g_pMain->AddServerPacket( EGS_START_REWARD_BOX_SELECT_ACK );

	return true;
}

bool CX2StateDungeonResult::Handler_EGS_START_REWARD_BOX_SELECT_ACK()
{
	if( g_pMain->DeleteServerPacket( EGS_START_REWARD_BOX_SELECT_ACK ) == true )
	{
		return true;
	}

	return false;
}

bool CX2StateDungeonResult::Handler_EGS_START_REWARD_BOX_SELECT_NOT()
{
	m_bStartRewardBoxSelectNot = true;

	return true;
}

bool CX2StateDungeonResult::Handler_EGS_SELECT_REWARD_BOX_REQ( int boxNum )
{
	KEGS_SELECT_REWARD_BOX_REQ kEGS_SELECT_REWARD_BOX_REQ;
	kEGS_SELECT_REWARD_BOX_REQ.m_BoxNum = boxNum;

	g_pData->GetServerProtocol()->SendPacket( EGS_SELECT_REWARD_BOX_REQ, kEGS_SELECT_REWARD_BOX_REQ );
	g_pMain->AddServerPacket( EGS_SELECT_REWARD_BOX_ACK );

	return true;
}

bool CX2StateDungeonResult::Handler_EGS_SELECT_REWARD_BOX_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_REWARD_BOX_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_SELECT_REWARD_BOX_ACK ) == true )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

bool CX2StateDungeonResult::Handler_EGS_SELECT_REWARD_BOX_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SELECT_REWARD_BOX_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( m_DungeonResultAniOrder == DRAO_CHOICE_REWARD )
	{
		if ( g_pX2Room->GetMySlot() != NULL && g_pX2Room->GetMySlot()->m_pUnit != NULL )
		{
			UidType myUnitUID = g_pX2Room->GetMySlot()->m_pUnit->GetUID();
			if ( kEvent.m_UnitUID == myUnitUID )
			{
				if ( kEvent.m_BoxNum < 0 || kEvent.m_BoxNum >= (int)m_vecRewardXMeshInst.size() )
				{

				}
				else
				{
					
					CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_vecRewardXMeshInst[kEvent.m_BoxNum] );
					if( NULL != pMeshInst )
					{
						pMeshInst->SetGlobalTime( 1999.9f );
					}

					m_DungeonResultAniOrder = DRAO_ANIMATE_BOX;
				}
			}
			else
			{
				if ( kEvent.m_BoxNum < 0 || kEvent.m_BoxNum >= (int)m_vecRewardXMeshInst.size() )
				{

				}
				else
				{
					CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_vecRewardXMeshInst[kEvent.m_BoxNum] );
					if( NULL != pMeshInst )
					{
						pMeshInst->SetGlobalTime( 999.9f );
					}
				}
			}


			return true;
		}
	}

	return false;
	
}

bool CX2StateDungeonResult::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_SELECT_REWARD_BOX_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if ( g_pMain->DeleteServerPacket( EGS_LEAVE_ROOM_ACK ) )
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			CX2State* pState = static_cast<CX2State*>( g_pMain->GetNowState() );
			if ( NULL != pState )
				pState->ReturnToPlaceWhereBeforeDungeonStart();

			return true;
		}
	}

	return false;	
}

bool CX2StateDungeonResult::Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	return g_pX2Room->Handler_EGS_LEAVE_ROOM_NOT( kEvent );
}

bool CX2StateDungeonResult::Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if ( g_pX2Room != NULL )
		g_pX2Room->Handler_EGS_JOIN_ROOM_NOT( kEvent );

	return false;
}

bool CX2StateDungeonResult::InitUI()
{
	DialogLog( "CX2StateDungeonResult::InitUI Start" );

	m_pDLGBack = new CKTDGUIDialog( this, L"DLG_Dungeon_Result_Back.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg(m_pDLGBack);
	if ( m_pDLGBack->GetIsSuccessInitOpenScript() == false )
		return false;


	m_pDLGFront = new CKTDGUIDialog( this, L"DLG_Dungeon_Result_Front.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg(m_pDLGFront);
	if ( m_pDLGFront->GetIsSuccessInitOpenScript() == false )
		return false;

	if ( g_pData->GetDungeonRoom() == NULL || g_pX2Room == NULL )
	{
		ErrorLogMsg( XEM_ERROR26, "g_pData->GetDungeonRoom() == NULL || g_pX2Room == NULL" );
		return false;
	}

	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGBack ) == false )
	{
		ErrorLogMsg( XEM_ERROR27, "m_pDLGBack 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGBack ) == false" );
		//ResultLog << "m_pDLGBack 메모리 침범당함" << fileout;
		m_bStartTimer = true;
		return false;
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"ResultScorePicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharScore = new CKTDGPicChar( L"HQ_RankPC_", pSeq );
	//m_pPicCharScore = new CKTDGPicChar( L"HQ_CPC_", pSeq );
	m_pPicCharScore->SetWidth( 35.0f );

	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"ResultEXPPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharEXP = new CKTDGPicChar( m_pPicCharScore, pSeq );
	m_pPicCharEXP->SetWidth( 30.0f );


	m_pPicCharBonusEXP = new CKTDGPicChar( g_pData->GetPicChar(), g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"ResultBonusEXPPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );
	m_pPicCharBonusEXP->SetWidth( 15 );

	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"ResultScorePicCharComboAndTech", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharComboAndTechScore = new CKTDGPicChar( m_pPicCharScore, pSeq );
	m_pPicCharComboAndTechScore->SetWidth( 18.0f );

	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"ResultED", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharED = new CKTDGPicChar( L"PC_", pSeq );
	m_pPicCharED->SetWidth( 18.0f );


	CKTDGUIStatic* pStaticDungeonPictrue = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Info" );
	if ( pStaticDungeonPictrue != NULL )
	{
		CKTDGUIControl::CPictureData* pPictureData = pStaticDungeonPictrue->GetPicture(2);

		pStaticDungeonPictrue->GetPicture( 3 )->SetShow( false );
		pStaticDungeonPictrue->GetPicture( 4 )->SetShow( false );

			
		if ( g_pData->GetDungeonResultInfo() != NULL )
		{
			if ( g_pData->GetDungeonResultInfo()->m_bWin == true )
			{
				pStaticDungeonPictrue->GetPicture( 3 )->SetShow( true );
			}
			else
			{
				pStaticDungeonPictrue->GetPicture( 4 )->SetShow( true );
			}
		}
		else
		{
			if ( g_pMain->GetTrainingSuccess() == true )
			{
				pStaticDungeonPictrue->GetPicture( 3 )->SetShow( true );
			}
			else
			{
				pStaticDungeonPictrue->GetPicture( 4 )->SetShow( true );
			}
		}


			
		const CX2Dungeon::DungeonData* pDungeonData = 
			g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID );
            
		CKTDGUIControl::UITextureData* pTexture = NULL;
		const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = NULL;

		if( NULL != pDungeonData )
		{
			pTexture = new CKTDGUIControl::UITextureData();

			pTexture->texName = pDungeonData->m_UIData.m_TextureName;
			pTexture->pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexture->texName );

			wstring wstrTexPieceName = pDungeonData->m_UIData.m_PieceName;
			MakeUpperCase( wstrTexPieceName );

			pTexUV = pTexture->pTexture->GetTexUV( wstrTexPieceName );
		}


		if ( NULL != pTexture && NULL != pTexture->pTexture )
		{
				
			if( pTexUV != NULL )
			{
				pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= pTexUV->leftTop;
				pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
				pTexture->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= pTexUV->leftBottom;
				pTexture->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= pTexUV->rightBottom;
				pTexture->SetTextureUV();

				pPictureData->SetTex( pTexture );
			}

			const CX2Dungeon::DungeonData* pDungeonData_Difficulty = 
				g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID) (g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID + g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonDifficulty) );


			// fix!!! 헤니르 시공의 경우에 던전 결과창에서 던전 데이타 정보를 난이도 대신에 도전 모드인지를 참조해야함

			wstring wstrBuff = L"";

			//{{ kimhc // 2011-04-26 // 디펜스 던전
#ifdef	SERV_INSERT_GLOBAL_SERVER
			if ( false == g_pData->GetDungeonManager()->IsDefenceDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) )
#endif	SERV_INSERT_GLOBAL_SERVER
				//}} kimhc // 2011-04-26 // 디펜스 던전
			{
					
				if( NULL != pDungeonData_Difficulty )
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_384, "ii", pDungeonData_Difficulty->m_MinLevel, pDungeonData_Difficulty->m_MaxLevel ) );
				}
			}

			D3DXCOLOR diffColor = g_pData->GetDungeonManager()->GetDifficultyColor( 
				g_pData->GetDungeonRoom()->GetDungeonID(), 
				g_pData->GetDungeonRoom()->GetDifficulty(),
				g_pData->GetSelectUnitLevel() );

			pStaticDungeonPictrue->GetString(0)->color	= diffColor;
			pStaticDungeonPictrue->GetString(0)->msg	= wstrBuff;

			int requireLevel = 0;
			const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)( g_pData->GetDungeonRoom()->GetDungeonID() + g_pData->GetDungeonRoom()->GetDifficulty() ) );
			if ( pDungeonData != NULL )
				requireLevel = pDungeonData->m_MinLevel;

			pStaticDungeonPictrue->GetString(2)->color = diffColor;
			pStaticDungeonPictrue->GetString(3)->color = diffColor;

			//{{ kimhc // 2010.3.22 //	비밀 던전 개편
#ifdef	SERV_SECRET_HELL
			pStaticDungeonPictrue->GetString(2)->msg = L"";
			pStaticDungeonPictrue->GetString(3)->msg = L"";

			switch ( pDungeonData->m_eDungeonType )
			{
			case CX2Dungeon::DT_NORMAL:
				{
					// DI_EL_FOREST_GATE_NORMAL, HARD, EXPERT 가 아니면
					if ( pDungeonData->m_DungeonID < SEnum::DI_EL_FOREST_GATE_NORMAL ||
						pDungeonData->m_DungeonID > SEnum::DI_EL_FOREST_GATE_EXPERT )
					{
						pStaticDungeonPictrue->GetString(2)->msg = GET_STRING( STR_ID_620 );
						pStaticDungeonPictrue->GetString(2)->msg += L":";

						switch ( pDungeonData->m_eDifficulty )
						{
						case CX2Dungeon::DL_NORMAL:
							pStaticDungeonPictrue->GetString(3)->msg = L" 1";
							break;

						case CX2Dungeon::DL_HARD:
							pStaticDungeonPictrue->GetString(3)->msg = L" 2~3";
							break;

						case CX2Dungeon::DL_EXPERT:
						default:
							pStaticDungeonPictrue->GetString(3)->msg = L" 3~4";
							break;
						}

#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
						if( pDungeonData->m_DungeonID == SEnum::DI_EVENT_VALENTINE_DUNGEON_INT )
						{
							pStaticDungeonPictrue->GetString(3)->msg = L" 3~4";
						}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
					}

					switch ( pDungeonData->m_eDifficulty )
					{
					case CX2Dungeon::DL_NORMAL:
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_385 );
						break;

					case CX2Dungeon::DL_HARD:
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_386 );
						break;

					case CX2Dungeon::DL_EXPERT:
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_388 );
						break;
					}

				} break;

			case CX2Dungeon::DT_HENIR:
				{
					if ( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_HENIR_PRACTICE )
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_938 );
					else if ( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_HENIR_CHALLENGE )
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_3747 );

				} break;

			case CX2Dungeon::DT_SECRET:
				{
					if ( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_SECRET_NORMAL )
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_938 );
					else if ( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode == CX2Dungeon::DM_SECRET_HELL )
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_4943 );
							
				} break;

			default:
				break;
			}		
			
#else	SERV_SECRET_HELL
			if ( g_pData->GetDungeonRoom()->GetDifficulty() == 0 )
			{
				pStaticDungeonPictrue->GetString(2)->msg = L"";
				pStaticDungeonPictrue->GetString(3)->msg = L"";
			}
			else if ( g_pData->GetDungeonRoom()->GetDifficulty() == 1 )
			{
				if ( pDungeonData->m_DungeonID == SEnum::DI_EL_FOREST_GATE_NORMAL ||
					pDungeonData->m_DungeonID == SEnum::DI_EL_FOREST_GATE_HARD || 
					pDungeonData->m_DungeonID == SEnum::DI_EL_FOREST_GATE_EXPERT )
				{
					pStaticDungeonPictrue->GetString(2)->msg = L"";
					pStaticDungeonPictrue->GetString(3)->msg = L"";
				}
				else
				{
					pStaticDungeonPictrue->GetString(2)->msg = GET_STRING( STR_ID_620 );
					pStaticDungeonPictrue->GetString(2)->msg += L" : ";
					pStaticDungeonPictrue->GetString(3)->msg = GET_STRING( STR_ID_621 );
				}

			}
			else if ( g_pData->GetDungeonRoom()->GetDifficulty() == 2 )
			{
				if ( pDungeonData->m_DungeonID == SEnum::DI_EL_FOREST_GATE_NORMAL ||
					pDungeonData->m_DungeonID == SEnum::DI_EL_FOREST_GATE_HARD || 
					pDungeonData->m_DungeonID == SEnum::DI_EL_FOREST_GATE_EXPERT )
				{
					pStaticDungeonPictrue->GetString(2)->msg = L"";
					pStaticDungeonPictrue->GetString(3)->msg = L"";
				}
				else
				{
					pStaticDungeonPictrue->GetString(2)->msg = GET_STRING( STR_ID_620 );
					pStaticDungeonPictrue->GetString(2)->msg += L" : ";
					pStaticDungeonPictrue->GetString(3)->msg = GET_STRING( STR_ID_622 );
				}
			}

			if( NULL != pDungeonData && CX2Dungeon::DT_HENIR == pDungeonData->m_eDungeonType )
			{
				switch( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
				{
				default:
				case CX2Dungeon::DM_HENIR_PRACTICE:
					{
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_938 );
					} break;

				case CX2Dungeon::DM_HENIR_CHALLENGE:
					{
						pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_3747 );
					} break;
				}
			}
			else
			{
				if ( g_pData->GetDungeonRoom()->GetDifficulty() == 0 )
					pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_385 );
				else if ( g_pData->GetDungeonRoom()->GetDifficulty() == 1 )
					pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_386 );
				else if ( g_pData->GetDungeonRoom()->GetDifficulty() == 2 )
					pStaticDungeonPictrue->GetString(4)->msg = GET_STRING( STR_ID_388 );

			}
#endif	SERV_SECRET_HELL
			//}} kimhc // 2010.3.22 //	비밀 던전 개편

		}
		else
		{
			pStaticDungeonPictrue->SetShow( false );
			pStaticDungeonPictrue->GetString(0)->msg = L"";
			pStaticDungeonPictrue->GetString(1)->msg = L"";
			pStaticDungeonPictrue->GetString(2)->msg = L"";
			pStaticDungeonPictrue->GetString(3)->msg = L"";
			pStaticDungeonPictrue->GetString(4)->msg = L"";
			SAFE_DELETE( pTexture );
		}

#ifndef NEW_DEFENCE_DUNGEON	//결과 화면에서 지킨 수정의 갯수 표시 않함
		//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
#ifdef	SERV_INSERT_GLOBAL_SERVER
		if ( NULL != pDungeonData) 
		{
			switch ( pDungeonData->m_DungeonID )
			{
			CASE_DEFENCE_DUNGEON
				{
					const CX2DungeonGame::DungeonResultInfo* pDungeonResultInfo
						= g_pData->GetDungeonResultInfo();

					if ( NULL != pDungeonResultInfo && true == pDungeonResultInfo->m_bWin )
					{
						// 디펜스 던전에서 승리 했으므로 보호된 엘의 수정 갯수를 출력
						SetShowNumOfElCrystalsProtected( true );

						// 보호된 작은 엘의 수정 갯수 출력
						CKTDGUIStatic* pStaticDefenceDungeonResult = static_cast<CKTDGUIStatic*>( m_pDLGBack->GetControl( L"Defence_result" ) );
						if ( NULL != pStaticDefenceDungeonResult )
						{	
							// 작은 엘의 수정은 3번 인덱스 부터 시작 한다
							const int MAGIC_START_INDEX_FOR_SMALL_EL_CRYSTALS = 3;
							const int MAGIC_END_INDEX_FOR_SMALL_EL_CRYSTALS = MAGIC_START_INDEX_FOR_SMALL_EL_CRYSTALS + pDungeonResultInfo->m_ucNumOfProtectedSmallCrystals;
								
							for ( int i = MAGIC_START_INDEX_FOR_SMALL_EL_CRYSTALS; i < MAGIC_END_INDEX_FOR_SMALL_EL_CRYSTALS; ++i )
							{
								CKTDGUIControl::CPictureData* pPictureData = pStaticDefenceDungeonResult->GetPicture( i );

								if ( NULL != pPictureData )
									pPictureData->SetShow( true );									
							} // for
						} // if
					} // if
				} // CASE_DEFENCE_DUNGEON
				break;

			default:
				break;
			} // switch
		} // if
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-18 // 국내, 디펜스 던전
#endif NEW_DEFENCE_DUNGEON
	}

	CX2Room::SlotData* pSlotData = g_pData->GetDungeonRoom()->GetMySlot();
	if( NULL != pSlotData && pSlotData->m_pUnit != NULL)
	{
		if( pSlotData->m_pUnitViewer == NULL )
			pSlotData->MakeUnitViewer();
		pSlotData->m_pUnitViewer->SetShowObject( true );

		pSlotData->m_pUnitViewer->SetNowEmotionId(CX2Unit::ET_NONE);

		if ( g_pData->GetDungeonResultInfo() != NULL )
		{			
			if ( g_pData->GetDungeonResultInfo()->m_bWin == true )
			{
				pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
			}
			else
			{
				pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLose", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
			}	
		}
		else //훈련소 결과창을 빨리 만들기 위해 임시로 들어간 코드 .. 고치자 따로 DungeonResultUI 클래스를 만드는게 좋지 안흥ㄹ까?
		{
			if ( g_pMain->GetTrainingSuccess() == true )
			{
				pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
			}
			else
			{
				pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLose", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
			}
		}
	}





	int otherUserNum = 0;

	if ( g_pData->GetDungeonResultInfo() != NULL )
	{
		for ( int i = 0; i < (int)g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList.size(); i++ )
		{
			CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = g_pData->GetDungeonResultInfo()->m_DungeonResultUnitInfoList[i];
			if ( pDungeonResultUnitInfo == NULL )
				continue;

			if ( g_pData->GetDungeonRoom()->GetMySlot() != NULL && pDungeonResultUnitInfo->m_UnitUID != g_pData->GetDungeonRoom()->GetMySlot()->m_pUnit->GetUID() )
			{
				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( pDungeonResultUnitInfo->m_UnitUID );
				if ( pSlotData == NULL )
					continue;

				if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
					continue;

				CKTDGUIStatic* pStaticPartyRank = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_Party_Rank" );


				if ( pStaticPartyRank != NULL && pStaticPartyRank->GetPicture( otherUserNum ) != NULL )
				{
					if ( pSlotData->m_pUnit != NULL )
					{
						pStaticPartyRank->GetPicture( otherUserNum )->SetShow( true );



						wstring fileName;
						wstring pieceName;
						if( true == pDungeonResultUnitInfo->m_bIsDie )
						{
							if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pSlotData->m_pUnit->GetUnitTemplet()->m_UnitClass, CX2Data::CIT_160by160Gray ) )
							{
								pStaticPartyRank->GetPicture( otherUserNum )->SetTex( fileName.c_str(), pieceName.c_str() );
							}
							else
							{
								ASSERT( !"NO" );
							}
						}
						else
						{
							if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pSlotData->m_pUnit->GetUnitTemplet()->m_UnitClass, CX2Data::CIT_160by160 ) )
							{
								pStaticPartyRank->GetPicture( otherUserNum )->SetTex( fileName.c_str(), pieceName.c_str() );
							}
							else
							{
								ASSERT( !"NO" );
							}
						}

					


						//switch( pSlotData->m_pUnit->GetUnitTemplet()->m_UnitClass )
						//{
						//case CX2Unit::UC_ELSWORD_SWORDMAN:
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Elsword1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Elsword2" );

						//	} break;
						//case CX2Unit::UC_ELSWORD_KNIGHT:
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Elsword_Sword" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Elsword_Sword_Gray" );
						//	} break;
						//case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:	// 11
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Elsword_Magic" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Elsword_Magic_Gray" );

						//	} break;

						//case CX2Unit::UC_ARME_VIOLET_MAGE:		// 2
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Aisha1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Aisha2" );
						//	} break;
						//case CX2Unit::UC_ARME_HIGH_MAGICIAN:		// 14
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Aisha_High" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Aisha_High_Gray" );
						//	} break;
						//case CX2Unit::UC_ARME_DARK_MAGICIAN:		// 15
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Aisha_Dark" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Aisha_Dark_Gray" );
						//	} break;

						//case CX2Unit::UC_LIRE_ELVEN_RANGER:		// 3
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Rena1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture19.tga", L"Dungeon_Result_Rena2" );
						//	} break;
						//case CX2Unit::UC_LIRE_COMBAT_RANGER:		// 12
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Rena_Combat" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Rena_Combat_Gray" );
						//	} break;
						//case CX2Unit::UC_LIRE_SNIPING_RANGER:		// 13
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Rena_Sniper" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Rena_Sniper_Gray" );
						//	} break;

						//case CX2Unit::UC_RAVEN_FIGHTER:			// 4
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture57.tga", L"DUNGEON_RESULT_RAVEN1" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_Common_New_Texture57.tga", L"DUNGEON_RESULT_RAVEN2" );
						//	} break;
						//case CX2Unit::UC_RAVEN_SOUL_TAKER:		// 16
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Sword" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Sword_Gray" );
						//	} break;
						//case CX2Unit::UC_RAVEN_OVER_TAKER:		// 17
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Over" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture15.tga", L"Result_Raven_Over_Gray" );
						//	} break;

						//case CX2Unit::UC_EVE_NASOD:				// 5
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Character01.tga", L"EVE_NORMAL" );	
						//		else													  
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Character01.tga", L"EVE_GRAY" );	
						//	} break;
						//case CX2Unit::UC_EVE_EXOTIC_GEAR:			// 18
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Eve_Exotic" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Eve_Exotic_Gray" );
						//	} break;

						//case CX2Unit::UC_EVE_ARCHITECTURE:		// 19
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture16.tga", L"Result_Eve_Arch" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture17.tga", L"Result_Eve_Arch_Gray" );
						//	} break;


						//case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture21.tga", L"L_Knight_Large" );		// 160 x 160
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture21.tga", L"L_Knight_Gray" );
						//	} break;

						//case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
						//	{
						//		if ( pDungeonResultUnitInfo->m_bIsDie == false )
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture21.tga", L"R_Slayer_Large" );
						//		else
						//			pStaticPartyRank->GetPicture( otherUserNum )->SetTex( L"DLG_UI_Common_Texture21.tga", L"R_Slayer_Gray" );

						//	} break;


						//default:
						//	{
						//		ASSERT( !"Unexpected UnitClass" );
						//	} break;
						//}
					}
					
				}
				

//				WCHAR staticResultUnit[256] = {0};

				WCHAR staticResultDesc[256] = {0};
				WCHAR staticResultID[256] = {0};
				WCHAR staticResultPCRoom[256] = {0};

//				wsprintf( staticResultUnit, L"Dungeon_Result_Other%d_Unit", otherUserNum );


				StringCchPrintf( staticResultDesc, 256, L"Dungeon_Result_Other%d_Desc", otherUserNum );
				StringCchPrintf( staticResultID, 256, L"Dungeon_Result_Other%d_ID", otherUserNum );
				StringCchPrintf( staticResultPCRoom, 256, L"Dungeon_Result_Other%d_PCRoom", otherUserNum );


				//wsprintf( staticResultDesc, L"Dungeon_Result_Other%d_Desc", otherUserNum );
				//wsprintf( staticResultID, L"Dungeon_Result_Other%d_ID", otherUserNum );
				//wsprintf( staticResultPCRoom, L"Dungeon_Result_Other%d_PCRoom", otherUserNum );



				if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
				{
					//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
					ErrorLogMsg( XEM_ERROR23, "m_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
					m_bStartTimer = true;
					return S_OK;
				}
/*

				CKTDGUIStatic* pStaticOtherUnit = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultUnit );

				pStaticOtherUnit->SetShow( true );

				for ( int i = 0; i < 3; i++ )
				{
					pStaticOtherUnit->GetPicture(i)->SetShow( false );
				}
				
				if ( pSlotData->m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_SWORDMAN )
				{
					pStaticOtherUnit->GetPicture(0)->SetShow( true );
				}
				else if ( pSlotData->m_pUnit->GetClass() == CX2Unit::UC_ARME_VIOLET_MAGE )
				{
					pStaticOtherUnit->GetPicture(1)->SetShow( true );
				}
				else if ( pSlotData->m_pUnit->GetClass() == CX2Unit::UC_LIRE_ELVEN_RANGER )
				{
					pStaticOtherUnit->GetPicture(2)->SetShow( true );
				}
*/

				if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
				{
					//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
					ErrorLogMsg( XEM_ERROR24, "m_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
					m_bStartTimer = true;
					return S_OK;
				}

				CKTDGUIStatic* pStaticOtherDesc = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultDesc );
				pStaticOtherDesc->SetShow( true );
				WCHAR unitDescBuff[1024] = {0};
				StringCchPrintf( unitDescBuff, 1024, L"LV.%d ", pSlotData->m_pUnit->GetUnitData().m_Level );
				//wsprintf( unitDescBuff, L"LV.%d ", pSlotData->m_pUnit->GetUnitData().m_Level );
				pStaticOtherDesc->GetString(0)->msg = unitDescBuff;

				CKTDGUIStatic* pStaticOtherID = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultID );
				pStaticOtherID->SetShow( true );
				pStaticOtherID->GetString(0)->msg = pSlotData->m_pUnit->GetNickName();

				if ( pSlotData->m_pUnit->GetUnitData().m_bIsGameBang == true )
				{
					CKTDGUIStatic* pStaticOtherPCRoom = (CKTDGUIStatic*)m_pDLGBack->GetControl( staticResultPCRoom );
					pStaticOtherPCRoom->SetShow( true );

				}
				

				otherUserNum++;
			}
		}
	}


	
	

	DialogLog( "CX2StateDungeonResult::InitUI End" );

	return true;
}

void CX2StateDungeonResult::UnitViewerProcess( CX2UnitViewerUI* pViewer )
{
	if ( pViewer == NULL )
	{
		ErrorLogMsg( XEM_ERROR28, "pViewer == NULL" );
		return;
	}

	//pViewer->SetLightPos( 1000, 1000, -200 );
	//pViewer->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pViewer->SetLightPos( -250, 100, -600 );	// 캐릭터뷰어 라이트 위치 변경

	pViewer->GetMatrix().Move( -272, -126, 0 ); 
	pViewer->GetMatrix().Scale( 1.9f,1.9f,1.9f );
	pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));

	pViewer->OnFrameMove( m_fTime, m_fElapsedTime );
}

CX2StateDungeonResult::RANK_SCORE CX2StateDungeonResult::Get_UI_RANK_SCORE( CX2DungeonRoom::RANK_TYPE rankType )
{
	RANK_SCORE rankScore;

	switch( rankType )
	{
	default:
	case CX2DungeonRoom::RT_F:
		rankScore = CX2StateDungeonResult::RS_F;
		break;

	case CX2DungeonRoom::RT_E:
		rankScore = CX2StateDungeonResult::RS_E;
		break;

	case CX2DungeonRoom::RT_D:
		rankScore = CX2StateDungeonResult::RS_D;
		break;

	case CX2DungeonRoom::RT_C:
		rankScore = CX2StateDungeonResult::RS_C;
		break;

	case CX2DungeonRoom::RT_B:
		rankScore = CX2StateDungeonResult::RS_B;
		break;

	case CX2DungeonRoom::RT_A:
		rankScore = CX2StateDungeonResult::RS_A;
		break;

	case CX2DungeonRoom::RT_S:
		rankScore = CX2StateDungeonResult::RS_S;
		break;

	//{{ kimhc // 2011-02-22 // 던전랭크 SS 등급 추가
#ifdef	DUNGEON_RANK_NEW
	case CX2DungeonRoom::RT_SS:
		rankScore = CX2StateDungeonResult::RS_SS;
		break;
#endif	DUNGEON_RANK_NEW	
	//}} kimhc // 2011-02-22 // 던전랭크 SS 등급 추가
	}

	return rankScore;
}


void CX2StateDungeonResult::ShowBonusEXPEvent( CX2StateDungeonResult::BonusEXPType bonusEXPType, int bonusEXP )
{
	wstringstream tempText;
	tempText << L"+" << bonusEXP;

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGBack->GetControl( L"Dungeon_Result_EXP_BG" );
	if ( pStatic != NULL && m_pPicCharBonusEXP != NULL )
	{
		//2부터 클리어, 랭크, 파티, MVP, 피시방, 프리미엄
		int offsetIndex = 2 + (int)bonusEXPType;

		if ( pStatic->GetPicture( offsetIndex ) != NULL )
			pStatic->GetPicture( offsetIndex )->SetShow( true );

		D3DXVECTOR3 tempPos = pStatic->GetDummyPos( (int)bonusEXPType );
		m_pPicCharBonusEXP->DrawText( tempText.str().c_str(),tempPos, D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );
	}
}

void CX2StateDungeonResult::PicCharFrameMove( double fTime, float fElapsedTime )
{
	if ( m_pPicCharScore != NULL )
		m_pPicCharScore->OnFrameMove( fElapsedTime );
	if ( m_pPicCharEXP != NULL )
		m_pPicCharEXP->OnFrameMove( fElapsedTime );
	if ( m_pPicCharComboAndTechScore != NULL )
		m_pPicCharComboAndTechScore->OnFrameMove( fElapsedTime );

	if( m_pPicCharBonusEXP != NULL )
		m_pPicCharBonusEXP->OnFrameMove( fElapsedTime );
}

void CX2StateDungeonResult::PicCharDrawText()
{
	if ( m_pPicCharScore != NULL && m_pPicCharComboAndTechScore != NULL && m_pPicCharED != NULL && m_pPicCharEXP != NULL )
	{
		m_pPicCharScore->Clear();
		m_pPicCharComboAndTechScore->Clear();
		m_pPicCharED->Clear();
		m_pPicCharEXP->Clear();

		if ( m_DungeonResultAniOrder >= DRAO_COMBO_SCORE && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
			m_pPicCharComboAndTechScore->DrawText( (int)m_fComboScore, D3DXVECTOR3(656+26,566,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );

		if ( m_DungeonResultAniOrder >= DRAO_TECHNICAL_SCORE && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
			m_pPicCharComboAndTechScore->DrawText( (int)m_fTechnicalScore, D3DXVECTOR3(877+30,566,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );

		if ( m_DungeonResultAniOrder >= DRAO_TIME && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
		{
			WCHAR buff[256] = {0};
			StringCchPrintf( buff, 256, L"%02d=%02d=%02d", (int)m_fHour, (int)m_fMinute, (int)m_fSec );
			//wsprintf( buff, L"%02d=%02d=%02d", (int)m_fHour, (int)m_fMinute, (int)m_fSec );
			m_pPicCharComboAndTechScore->DrawText( buff, D3DXVECTOR3(731+12,383,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
		}

		if ( m_DungeonResultAniOrder >= DRAO_DAMAGED && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
		{
			WCHAR buff[256] = {0};
			if ( m_fDamaged == 0 )
			{
				StringCchPrintf( buff, 256, L"0" );
				//wsprintf( buff, L"0" );
			}
			else
			{
				StringCchPrintf( buff, 256, L"-%d", (int)m_fDamaged );
				//wsprintf( buff, L"-%d", (int)m_fDamaged );
			}
			m_pPicCharComboAndTechScore->DrawText( buff, D3DXVECTOR3(945,383,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
		}

		if ( m_DungeonResultAniOrder >= DRAO_TOTAL_SCORE && m_DungeonResultAniOrder < DRAO_FIRST_RESULT_INFO_FADE_OUT )
			m_pPicCharScore->DrawText( (int)m_fTotalScore, D3DXVECTOR3(865+47,653,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );

		if ( m_DungeonResultAniOrder >= DRAO_EXP  )
			m_pPicCharEXP->DrawText( (int)m_fEXP, D3DXVECTOR3(930,649,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );


		if ( m_DungeonResultAniOrder >= DRAO_BONUS_EXP  )
		{
			/*
			WCHAR buff[256] = {0};
			if ( m_fBonusEXP == 0 )
			{
			wsprintf( buff, L"0" );
			}
			else
			{
			wsprintf( buff, L"+%d", (int)m_fBonusEXP );
			}

			m_pPicCharScore->DrawText( buff, D3DXVECTOR3(948,582,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
			*/


			//m_pPicCharScore->DrawText( (int)m_fBonusEXP, D3DXVECTOR3(948,582,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
		}

		if ( m_DungeonResultAniOrder >= DRAO_ED )
		{
			m_pPicCharED->DrawText( (int)m_fED, D3DXVECTOR3(640,130,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_RIGHT, 0.00f );
		}

	}
}

bool CX2StateDungeonResult::LastMsgByESC()
{
	g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_22136 ), DRUCM_ESCAPE_DUNGEON_RESULT_OK, 
		g_pMain->GetNowState(), DRUCM_ESCAPE_DUNGEON_RESULT_CANCEL );

	return true;
}



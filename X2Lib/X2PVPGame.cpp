#include "StdAfx.h"
#include ".\x2pvpgame.h"

CX2PVPGame::CX2PVPGame(void)
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	: m_bEscape( true )
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
{
	m_GameType				= CX2Game::GT_PVP;

	m_pPVPRoom				= NULL;
	m_WinTeam				= CX2Room::TN_RED;

	m_bTimeStop				= false;
	m_fRemainPlayTime		= 0.0f;
	m_fFirstRemainPlayTime	= -1.f;
	m_iLastElapsedPlayTime	= 0;

	m_pFontForTime		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL );


#ifdef TODAY_RECORD_TEST
	m_iConsecutiveWinCount	= 0;
	m_iConsecutiveLoseCount = 0;
#endif TODAY_RECORD_TEST

#ifdef DUNGEON_ITEM	
	m_bFirstCreateItem		= false;
	m_iSepcialItemId		= 0;
	m_iSpecialItemUid		= 0;
	m_fCreateItemTimer		= 0.f;
	m_fChangeItemPosTimer	= 0.f;
#endif

//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
	if ( g_pInstanceData != NULL )
	{
		g_pInstanceData->SetReceiveGameStartNotPacket( false );		// packet을 받지 않은 상태로 초기화
		g_pInstanceData->SetSendGameStartReqPacket( false );		// packet을 전송하지 않은 상태로 초기화
	}
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정

	CX2GageManager::GetInstance()->ClearPvpMemberUI();
}

CX2PVPGame::~CX2PVPGame(void)
{
	/*
	Release() 에서 수행
	*/
}

/** @function : Release
	@brief : X2Game의 소멸자에 실행할 것들을 이곳에서 실행함
	(이렇게 따로 만들어 준 이유는 X2Game 소멸자가 X2GameUnit 등을 소멸할 때
	X2GameUnit 등의 소멸자 내부 또는 호출되는 함수 들에 내부에서 다시 X2Game을 참조하는 경우
	가 많아서 다 찾아내기 힘들기에 이렇게 Release로 만들었습니다.
	X2Game이 소멸되는 경우는 현재 Pvp, Dungeon, BattleField의 스테이트가 소멸될 때 이므로
	해당 스테이트의 소멸자에서 Release를 호출 하도록 하였습니다.
	더 좋은 방법이 있으신 분은 알려주세요 - 2012-11-11 (김현철)
*/
/*virtual*/ void CX2PVPGame::Rlease()
{
	m_mapUnitKillNum.clear();
	m_mapUnitDeathNum.clear();
	//g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForTime );

	CX2Game::Release();
}

HRESULT CX2PVPGame::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2Game::OnFrameMove( fTime, fElapsedTime );
	
	if( m_GameState == GS_PLAY )
	{
		if( m_bTimeStop == false )
		{
			if( m_fRemainPlayTime > 0.0f )
				m_fRemainPlayTime -= fElapsedTime;

			if( m_fRemainPlayTime <= 0.0f )
				m_fRemainPlayTime = 0.0f;
		}

#ifdef DUNGEON_ITEM
		if( IsHost() == true )
		{
			if( m_pPVPRoom != NULL && m_pPVPRoom->GetItemMode() == true )
			{			
				if( m_iSepcialItemId > 0 )
				{
					m_fCreateItemTimer = 0.f;

					m_fChangeItemPosTimer += fElapsedTime;
					if( m_fChangeItemPosTimer >= 9.f )
					{
						Handler_EGS_CHANGE_PVP_ITEM_POS_REQ();
						m_fChangeItemPosTimer = 0.f;
					}
				}
				else
				{
					m_fChangeItemPosTimer = 0.f;
				}

				if( m_iSepcialItemId <= 0 )
				{
					m_fCreateItemTimer += fElapsedTime;
					if( m_fCreateItemTimer >= 30.f )
					{						
						// 아이템 없이 30초가 지났으므로 아이템 생성 요청
						// 생성될 위치를 결정한다.
						int iItemPos = GetPositionCreateItem();
						if( iItemPos >= 0)
						{
							// 아이템 생성 요청
							Handler_EGS_CREATE_PVP_ITEM_REQ(iItemPos);
						}
						m_fCreateItemTimer = 0.f;
						m_iSepcialItemId = 1; // 중복 요청을 방지
					}
				}
			}
		}

		for( int i = 0; i <  m_pRoom->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = m_pRoom->GetSlotData( i );

			if( pSlotData == NULL 
				|| pSlotData->m_SlotState == CX2Room::SS_CLOSE
				|| pSlotData->m_SlotState == CX2Room::SS_EMPTY
				|| pSlotData->m_SlotState == CX2Room::SS_WAIT )
				continue;

			if ( false == pSlotData->IsMyUnit() )
			{
				CX2GageManager* pGageManager = CX2GageManager::GetInstance();

				if ( NULL != pGageManager )
				{
					CX2GameUnit* pGameUnitMember 
						= GetUserUnitByUID( pSlotData->m_pUnit->GetUID() );

					pGageManager->UpdatePvpMemberGageData( pSlotData->m_pUnit->GetUID(), pGameUnitMember );
				}
			}
		}

		for( int i = 0; i <  m_pRoom->GetNpcSlotNum(); ++i )
		{
			CX2Room::RoomNpcSlot* pSlotData = m_pRoom->GetNpcSlotDataByIndex( i );

			if( pSlotData == NULL )
				continue;

			CX2GageManager* pGageManager = CX2GageManager::GetInstance();

			if ( NULL != pGageManager )
			{
				CX2GameUnit* pGameUnitMember 
					= GetNPCUnitByUID( static_cast<int>( pSlotData->m_iNpcUid ) );

				pGageManager->UpdatePvpMemberGageData( pSlotData->m_iNpcUid, pGameUnitMember );
			}
		}
#endif

	}

	return S_OK;
}

HRESULT CX2PVPGame::OnFrameRender()
{
	CX2Game::OnFrameRender();

	if ( m_GameState == GS_PLAY )
	{
#ifdef DIALOG_SHOW_TOGGLE
		bool bHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();
		if( bHide == true )
			return S_OK;
#endif

		if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
		{
			WCHAR timeBuf[200] = {0,};
			swprintf( timeBuf, L"%d", (int)GetRemainPlayTime() );
			m_pFontForTime->OutTextXY( 516, 127, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		}
	}

	return S_OK;
}

void CX2PVPGame::GameLoading( CX2Room* pRoom )
{
	m_GameState			= GS_LOADING;
	m_pPVPRoom			= (CX2PVPRoom*)pRoom;
	CX2Game::GameLoading( pRoom );
	m_GameState			= GS_LOADING;
	
	m_fRemainPlayTime	= m_pPVPRoom->GetPlayTimeLimit();

//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
	if ( g_pKTDXApp->GetDeviceManager() != NULL )
		g_pKTDXApp->GetDeviceManager()->ReleaseAllMemoryBuffers();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
}

void CX2PVPGame::WorldLoading()
{
	//월드..
	m_pWorld = g_pData->GetWorldManager()->CreateWorld( (CX2World::WORLD_ID)m_pPVPRoom->GetWorldID() );

#ifdef PORTAL_LINEMAP_TEST
	//{{ 2009.04.24 태완 : CX2DungeonGame::StageLoading 안의 코드를 가져다 붙임
	// note!! dmlee 2008.12.21 일단 포탈 라인맵위치에 포탈 표시 파티클은 여기서 생성하는 걸로 하드 코딩
	if( NULL != m_pWorld &&
		NULL != m_pWorld->GetLineMap() )
	{
		for( int i=0; i<m_pWorld->GetLineMap()->GetNumLineData(); i++ )
		{
			const CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( i );
			if( NULL != pLineData &&
				CKTDGLineMap::LT_POTAL == pLineData->lineType )
			{
				CX2WorldObjectParticle* pParticle = m_pWorld->CreateObjectParticle( GetMajorParticle(), "Peita_Teleport_MagicSquare01" );
				if( NULL != pParticle )
				{
					pParticle->SetParticlePos( ( pLineData->startPos + pLineData->endPos ) * 0.5f + D3DXVECTOR3(0, 3, 0 ) );
					pParticle->SetLayer( XL_EFFECT_0 );
				}
			}
		}
	}
	//}}
#endif PORTAL_LINEMAP_TEST

}

bool CX2PVPGame::DeleteUserUnit( int index )
{
	//전적 정보 삭제
	return CX2Game::DeleteUserUnit( index );
}

void CX2PVPGame::TimeOut()
{
	//타임아웃 이펙트

	//전투종료(강제무적)
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UnitList[i];
		if( pCX2GameUnit != NULL )
			pCX2GameUnit->SetForceInvincible( 9999.0f );
	}
	
}

bool CX2PVPGame::Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT )
{
	if( kEGS_LEAVE_ROOM_NOT.m_iUnitUID == g_pX2Room->GetMySlot()->m_UnitUID )
	{
		ErrorLog( XEM_ERROR119 );
	}

	CX2Game::Handler_EGS_LEAVE_ROOM_NOT( kEGS_LEAVE_ROOM_NOT );

	if( kEGS_LEAVE_ROOM_NOT.m_iUnitUID == g_pX2Room->GetMySlot()->m_UnitUID )
	{
#ifdef SERV_PVP_NEW_SYSTEM
		if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
		{
			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if( pState != NULL )
				pState->ReturnToPlaceWhereBeforeDungeonStart();
		}
		else
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( 437 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
		}

		CX2GageManager::GetInstance()->ClearPvpMemberUI();
#else
		DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false" );
		g_pMain->CreateStateChangeDLG( GET_STRING( 437 ) );
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_LOBBY, NULL, false );
#endif
	}
	else
	{
		CX2GageManager::GetInstance()->RemovePvpMemberUIByUserUid( kEGS_LEAVE_ROOM_NOT.m_iUnitUID );
	}

	return true;
}

bool CX2PVPGame::Handler_EGS_LEAVE_GAME_NOT( KEGS_LEAVE_GAME_NOT& kEGS_LEAVE_GAME_NOT )
{
	if( kEGS_LEAVE_GAME_NOT.m_iUnitUID == g_pX2Room->GetMySlot()->m_UnitUID )
	{
		g_pMain->CreateStateChangeDLG( GET_STRING( 438 ) );
	}

	CX2Game::Handler_EGS_LEAVE_GAME_NOT( kEGS_LEAVE_GAME_NOT );

	if( kEGS_LEAVE_GAME_NOT.m_iUnitUID == g_pX2Room->GetMySlot()->m_UnitUID )
	{
#ifdef SERV_PVP_NEW_SYSTEM
		if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
		{
			CX2State* pState = (CX2State*)g_pMain->GetNowState();
			if( pState != NULL )
				pState->ReturnToPlaceWhereBeforeDungeonStart();
		}
		else
		{
			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false" );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );
		}

		CX2GageManager::GetInstance()->ClearPvpMemberUI();
#else
		DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false" );
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );
#endif
	}
	else
	{
		CX2GageManager::GetInstance()->RemovePvpMemberUIByUserUid( kEGS_LEAVE_GAME_NOT.m_iUnitUID );
	}

	return true;
}

bool CX2PVPGame::Handler_EGS_USER_UNIT_DIE_ACK( KEGS_USER_UNIT_DIE_ACK& kEGS_USER_UNIT_DIE_ACK )
{
#ifndef SERV_PVP_NEW_SYSTEM
	if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		if( NULL != g_pChatBox )
		{
			wstring wstrColor = L"#C4EB2D2";								// 78, 178, 210
			D3DXCOLOR coTextColor(0.30588f, 0.69803f, 0.823529f, 1.f);	// 78, 178, 210

			g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_439, "ii", kEGS_USER_UNIT_DIE_ACK.m_VP, kEGS_USER_UNIT_DIE_ACK.m_EXP ) ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
		}
	}
#endif
	return true;
}

bool CX2PVPGame::Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT )
{
	CX2Game::Handler_EGS_REMAINING_PLAY_TIME_NOT( kEGS_REMAINING_PLAY_TIME_NOT );

	float   fRemainingPlayTime = kEGS_REMAINING_PLAY_TIME_NOT;

	if( m_fGameTime > 5.0f )
	{
#ifdef  ELOG_STATISTICS_NEW_DATA
		m_kGameStatistics.GatherPeriodicStatistics();
#else   ELOG_STATISTICS_NEW_DATA
		if( m_AverageRenderFrameRate <= 0 )
		{
			m_AverageRenderFrameRate = (int)DXUTGetFPS();
		}
		else
		{
			m_AverageRenderFrameRate += (int)DXUTGetFPS();
			m_AverageRenderFrameRate /= 2;
		}
#endif  ELOG_STATISTICS_NEW_DATA

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		CX2Game::StartTcpPingCheck();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	}

	if( GetTimeStop() == false )
	{
		SetRemainPlayTime( fRemainingPlayTime );
		float fElapsedPlayTime = GetFirstRemainPlayTime() - GetRemainPlayTime() ;
		int iElapsedTimeBy50 = (int) ( fElapsedPlayTime / 50.f );
		
		if( fElapsedPlayTime > 249.f &&
			iElapsedTimeBy50 != m_iLastElapsedPlayTime ) 
		{
			m_iLastElapsedPlayTime = iElapsedTimeBy50;


			switch( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
			{
			case KPVPChannelInfo::PCC_PLAY:
			case KPVPChannelInfo::PCC_FREE:
			case KPVPChannelInfo::PCC_TOURNAMENT:
				{
				} break;

			default:
				{
#ifndef SERV_PVP_NEW_SYSTEM
					if( GetMyUnit() != NULL )
					{
						CX2Unit* pMyUnit = GetMyUnit()->GetUnit();
						if( NULL != pMyUnit )
						{
							const CX2PVPResultTable::ResultData* pResultData = 
								g_pData->GetPVPResultTable()->GetTimeBonus( pMyUnit->GetPVPEmblem() );

							if( NULL != pResultData )
							{
								if( NULL != g_pChatBox )
								{
									wstring wstrColor = L"#C4EB2D2";							// 78, 178, 210
									D3DXCOLOR coTextColor(0.30588f, 0.69803f, 0.823529f, 1.f);	// 78, 178, 210

									g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_440, "ii", pResultData->m_VP, pResultData->m_EXP ) ), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
								}
							}
						}
					}
#endif
				} break;
			}

		}
	}
	return true;
}

bool CX2PVPGame::Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT )
{
	CX2Game::Handler_EGS_STATE_CHANGE_RESULT_NOT( kEGS_STATE_CHANGE_RESULT_NOT );
	DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_RESULT, NULL, false" );
	g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_441 ) );
	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_RESULT, NULL, false );

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	m_bEscape = false;

	KOGGamePerformanceCheck::GetInstance()->End();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	return true;
}

/*virtual*/ bool CX2PVPGame::Handler_EGS_END_GAME_NOT( KEGS_END_GAME_NOT& kKEGS_END_GAME_NOT )
{
	CX2Game::Handler_EGS_END_GAME_NOT( kKEGS_END_GAME_NOT );
	TimeStop();

	return true;
}

//bool CX2PVPGame::Handler_EGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT( KEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT& kEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT )
//{
//	g_pData->ResetPVPTeamResultInfo( kEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT );
//
//	return true;
//}
//
//bool CX2PVPGame::Handler_EGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT( KEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT& kEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT )
//{
//	g_pData->ResetPVPDeathResultInfo( kEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT );
//
//	return true;
//}
//
//bool CX2PVPGame::Handler_EGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT( KEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT& kEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT )
//{
//	g_pData->ResetPVPTeamDeathResultInfo( kEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT );
//
//	return true;
//}

bool CX2PVPGame::Handler_EGS_END_GAME_PVP_RESULT_DATA_NOT( KEGS_END_GAME_PVP_RESULT_DATA_NOT& kEGS_END_GAME_PVP_RESULT_DATA_NOT )
{
#ifdef SERV_PVP_NEW_SYSTEM
	bool bWinMyTeam = false;
#endif

	CX2GUUser* pMyUnit = GetMyUnit();
	if( pMyUnit != NULL )
	{
		for( int  i = 0; i < (int)kEGS_END_GAME_PVP_RESULT_DATA_NOT.m_vecPVPUnitInfo.size(); i++ )
		{
			KPVPUnitResultInfo kPVPUnitResultInfo = kEGS_END_GAME_PVP_RESULT_DATA_NOT.m_vecPVPUnitInfo[i];
			if( pMyUnit->GetUnitUID() == kPVPUnitResultInfo.m_UnitUID )
			{
				if( true == kEGS_END_GAME_PVP_RESULT_DATA_NOT.m_bIsDrawn ) 
				{
					pMyUnit->Draw();
				}
				else
				{
					if( kPVPUnitResultInfo.m_bWin == true )
					{
						pMyUnit->Win();
#ifdef SERV_PVP_NEW_SYSTEM
						bWinMyTeam = true;
#endif

#ifdef TODAY_RECORD_TEST
						m_iConsecutiveLoseCount = 0;
						if( NULL != GetMyUnit() && 
							NULL != GetMyUnit()->GetUnit() )
						{
							GetMyUnit()->GetUnit()->AccessUnitData().m_DailyAchievement.m_iWinCount += 1;
							m_iConsecutiveWinCount += 1;

							if( m_iConsecutiveWinCount > GetMyUnit()->GetUnit()->GetUnitData().m_DailyAchievement.m_iMaxConsecutivePVPWinCount )
							{
								GetMyUnit()->GetUnit()->AccessUnitData().m_DailyAchievement.m_iMaxConsecutivePVPWinCount = m_iConsecutiveWinCount;
							}
						}
#endif TODAY_RECORD_TEST
					}
					else
					{
						pMyUnit->Lose();


#ifdef TODAY_RECORD_TEST
						m_iConsecutiveWinCount = 0;

						if( NULL != GetMyUnit() && 
							NULL != GetMyUnit()->GetUnit() )
						{
							GetMyUnit()->GetUnit()->AccessUnitData().m_DailyAchievement.m_iLoseCount += 1;
							m_iConsecutiveLoseCount += 1;

							if( m_iConsecutiveLoseCount > GetMyUnit()->GetUnit()->GetUnitData().m_DailyAchievement.m_iMaxConsecutivePVPLoseCount )
							{
								GetMyUnit()->GetUnit()->AccessUnitData().m_DailyAchievement.m_iMaxConsecutivePVPLoseCount = m_iConsecutiveLoseCount;
							}
						}
#endif TODAY_RECORD_TEST


					}
				}
			}
		}
	}

#ifdef SERV_PVP_NEW_SYSTEM
	if( IsHost() == true && g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
	{
		for(int i=0; i<GetNPCUnitNum(); ++i)
		{
			CX2GUNPC *pNpc = GetNPCUnit(i);
			if( pNpc != NULL && pNpc->IsPvpBot() == true && pNpc->GetNowHp() > 0.f )
			{
				if( kEGS_END_GAME_PVP_RESULT_DATA_NOT.m_bIsDrawn == true )
				{
					pNpc->Draw();
				}
				else if( bWinMyTeam == true )
				{
					pNpc->Lose();
				}
				else
				{
					pNpc->Win();
				}
			}
		}
	}
#endif

	CX2GageManager::GetInstance()->ClearPvpMemberUI();

	g_pData->ResetPVPResultInfo( kEGS_END_GAME_PVP_RESULT_DATA_NOT );

	return true;
}

void CX2PVPGame::SetRemainPlayTime( float fRemainPlayTime )
{
	m_fRemainPlayTime = fRemainPlayTime; 

	if( -1.f == m_fFirstRemainPlayTime )
	{
		m_fFirstRemainPlayTime = fRemainPlayTime;
	}
}


#ifdef DUNGEON_ITEM
int CX2PVPGame::GetPositionCreateItem()
{
	if( m_pWorld == NULL || m_pWorld->GetLineMap() == NULL )
		return -1;

	int iResult = -1;
	int nSizeUser = GetUserUnitListSize();

	CKTDGLineMap *pLineMap = m_pWorld->GetLineMap();
	int nSizeStartPosNum = pLineMap->GetStartPosNum();
	if( nSizeStartPosNum <= 0 )
		return -1;	

	vector<int> vecCreatePosition;
	
	for(int i=0; i<nSizeStartPosNum; ++i)
	{
		bool bExistUser = false;
		D3DXVECTOR3 vPos = pLineMap->GetStartPosition(i);

		// 현재 포지션주위 300내에 존재하는 유저가 있는지 검사한다.
		for(int j=0; j<nSizeUser; ++j)
		{
			CX2GUUser *pUser = GetUserUnit(j);
			if(pUser != NULL)
			{
				if( GetDistance( vPos, pUser->GetPos() ) <= 300.f )
				{
					bExistUser = true;
					break;
				}
			}
		}
		
		if( bExistUser == false )
		{
			// 현재 포지션은 주위 300이내에 유저가 없는 위치이다.
			vecCreatePosition.push_back(i);
		}
	}
	
	UINT nPosition = vecCreatePosition.size();
	if( nPosition > 0 )
	{
		// 선택된 위치중 랜덤으로 아이템 생성위치를 결정한다.
		iResult = rand() % nPosition;
		iResult = vecCreatePosition[iResult];
	}
	else
	{
		// 전체 위치중 랜덤으로 아이템 생성위치를 결정한다.
		iResult = rand() % nSizeStartPosNum;
	}

	return iResult;	
}

void CX2PVPGame::CreateSpecialItem(int itemid, int itemuid, int startpos)
{
	if(m_pDropItemManager != NULL)
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3( 0.f, 0.f, 0.f );
		if( m_pWorld != NULL && m_pWorld->GetLineMap() != NULL )
		{
			vPos = m_pWorld->GetLineMap()->GetStartPosition(startpos);
		}

		m_iSepcialItemId = itemid;
		m_iSpecialItemUid = itemuid;

		m_pDropItemManager->AddDropItem( m_iSepcialItemId, m_iSpecialItemUid, vPos, 0.f, 99999.f );
	}
}

void CX2PVPGame::ChangeSpecialItem(int itemId, int itemuid)
{	
	if(m_pDropItemManager != NULL && m_iSepcialItemId > 0)
	{
		if( m_pDropItemManager->GetDropItemCount() <= 0 )
		{
			m_iSepcialItemId = 0;
			return;
		}

		D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

		m_iSepcialItemId = itemId;
		m_iSpecialItemUid = itemuid;

		m_pDropItemManager->GetDropItemPosition(0, vPos);

		m_pDropItemManager->DeleteAllItem();		
		m_pDropItemManager->AddDropItem( itemId, itemuid, vPos, 0.f, 99999.f );
	}
}

void CX2PVPGame::Handler_EGS_CREATE_PVP_ITEM_REQ(int iCreateIdPos)
{
	KEGS_CREATE_PVP_ITEM_REQ kKEGS_GET_ITEM_REQ;
	
	kKEGS_GET_ITEM_REQ.m_iPositionIndex = iCreateIdPos;

	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_PVP_ITEM_REQ, kKEGS_GET_ITEM_REQ );
	
}
void CX2PVPGame::Handler_EGS_CHANGE_PVP_ITEM_POS_REQ()
{
	if( m_pDropItemManager != NULL && m_iSpecialItemUid > 0 )
	{
		int iItemPos = GetPositionCreateItem();
		if( iItemPos >= 0)
		{
			// 아이템 위치 변경 요청
			KEGS_CHANGE_PVP_ITEM_POS_REQ  kEGS_CHANGE_PVP_ITEM_POS_REQ;

			kEGS_CHANGE_PVP_ITEM_POS_REQ.m_iItemUID = (UidType)m_iSpecialItemUid;
			kEGS_CHANGE_PVP_ITEM_POS_REQ.m_iPos = iItemPos;

			g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PVP_ITEM_POS_REQ , kEGS_CHANGE_PVP_ITEM_POS_REQ );
		}
	}
}

void CX2PVPGame::Handler_EGS_CHANGE_PVP_ITEM_POS_NOT( UidType iItemUid, int iChangePos )
{
	if( m_pDropItemManager != NULL )
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3( 0.f, 0.f, 0.f );
		if( m_pWorld != NULL && m_pWorld->GetLineMap() != NULL )
		{
			vPos = m_pWorld->GetLineMap()->GetStartPosition(iChangePos);
		}

		bool bChanged = m_pDropItemManager->ChangeDropItemPositionByUid( (int)iItemUid, vPos );
		if( bChanged == true )
		{
			m_pDropItemManager->DeleteAllItem();		
			m_pDropItemManager->AddDropItem( m_iSepcialItemId, m_iSpecialItemUid, vPos, 0.f, 99999.f );
		}

		m_fChangeItemPosTimer = 0.f;
	}
}
#endif

//CX2PVPGame::PVPResultInfo::PVPResultInfo( KEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT& kPacket )
//{
//	m_WinTeam		= kPacket.m_cWinTeam;
//
//	for( int i = 0; i < (int)kPacket.m_vecPVPUnitInfo.size(); i++ )
//	{
//		PVPResultUnitInfo* pPVPResultUnitInfo	= new PVPResultUnitInfo();
//		KPVPTeamUnitInfo kPVPUnitInfo			= kPacket.m_vecPVPUnitInfo[i];
//
//		pPVPResultUnitInfo->m_Rank				= 0;
//
//		pPVPResultUnitInfo->m_UnitUid			= kPVPUnitInfo.m_UnitUID;
//		pPVPResultUnitInfo->m_KillNum			= kPVPUnitInfo.m_nKillNum;
//		pPVPResultUnitInfo->m_DieNum			= kPVPUnitInfo.m_nDeathNum;
//		pPVPResultUnitInfo->m_PVPPoint			= kPVPUnitInfo.m_nVSPoint;
//		pPVPResultUnitInfo->m_ED				= kPVPUnitInfo.m_nED;
//		pPVPResultUnitInfo->m_bIsMVP			= kPVPUnitInfo.m_bIsMVP;
//
//		m_PVPResultUnitInfoList.push_back( pPVPResultUnitInfo );
//	}
//}
//
//CX2PVPGame::PVPResultInfo::PVPResultInfo( KEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT& kPacket )
//{
//	for( int i = 0; i < (int)kPacket.m_vecPVPUnitInfo.size(); i++ )
//	{	
//		KPVPDeathSurvivalUnitInfo kPVPUnitInfo = kPacket.m_vecPVPUnitInfo[i];
//
//		CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = new CX2PVPGame::PVPResultUnitInfo();
//
//		pPVPResultUnitInfo->m_UnitUid		= kPVPUnitInfo.m_UnitUID;
//		pPVPResultUnitInfo->m_Rank			= kPVPUnitInfo.m_nRanking;
//		pPVPResultUnitInfo->m_KillNum		= kPVPUnitInfo.m_nKillNum;
//		pPVPResultUnitInfo->m_DieNum		= kPVPUnitInfo.m_nDeathNum;
//		pPVPResultUnitInfo->m_PVPPoint		= kPVPUnitInfo.m_nVSPoint;
//		pPVPResultUnitInfo->m_ED			= kPVPUnitInfo.m_nED;
//		pPVPResultUnitInfo->m_bIsMVP		= kPVPUnitInfo.m_bIsMVP;
//
//		m_PVPResultUnitInfoList.push_back( pPVPResultUnitInfo );
//	}
//}
//
//
//CX2PVPGame::PVPResultInfo::PVPResultInfo( KEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT& kPacket )
//{
//	m_WinTeam		= kPacket.m_cWinTeam;
//
//	for( int i = 0; i < (int)kPacket.m_vecPVPUnitInfo.size(); i++ )
//	{
//		PVPResultUnitInfo* pPVPResultUnitInfo	= new PVPResultUnitInfo();
//		KPVPTeamUnitInfo kPVPUnitInfo			= kPacket.m_vecPVPUnitInfo[i];
//
//		pPVPResultUnitInfo->m_Rank				= 0;
//
//		pPVPResultUnitInfo->m_UnitUid			= kPVPUnitInfo.m_UnitUID;
//		pPVPResultUnitInfo->m_KillNum			= kPVPUnitInfo.m_nKillNum;
//		pPVPResultUnitInfo->m_DieNum			= kPVPUnitInfo.m_nDeathNum;
//		pPVPResultUnitInfo->m_PVPPoint			= kPVPUnitInfo.m_nVSPoint;
//		pPVPResultUnitInfo->m_ED				= kPVPUnitInfo.m_nED;
//		pPVPResultUnitInfo->m_bIsMVP			= kPVPUnitInfo.m_bIsMVP;
//
//		m_PVPResultUnitInfoList.push_back( pPVPResultUnitInfo );
//	}
//}

CX2PVPGame::PVPResultInfo::PVPResultInfo( KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacket )
{
	m_WinTeam		= kPacket.m_cWinTeam;
	m_bIsDrawn		= kPacket.m_bIsDrawn;

	for( int i = 0; i < (int)kPacket.m_vecPVPUnitInfo.size(); i++ )
	{
		PVPResultUnitInfo* pPVPResultUnitInfo	= new PVPResultUnitInfo();
		KPVPUnitResultInfo kPVPUnitInfo			= kPacket.m_vecPVPUnitInfo[i];
		pPVPResultUnitInfo->Set_KPVPUnitResultInfo( kPVPUnitInfo );

		m_PVPResultUnitInfoList.push_back( pPVPResultUnitInfo );
	}

#ifdef SERV_PVP_NEW_SYSTEM
	m_bIsAbnormalEndPlay = kPacket.m_bIsAbnormalEndPlay;
#endif

#ifdef SERV_PVP_REMATCH
	m_iRematchCount = kPacket.m_iRematchCount;
#endif SERV_PVP_REMATCH
}

CX2PVPGame::PVPResultInfo::~PVPResultInfo()
{
	for( int i = 0; i < (int)m_PVPResultUnitInfoList.size(); i++ )
	{
		CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = m_PVPResultUnitInfoList[i];
		SAFE_DELETE( pPVPResultUnitInfo );
	}
	m_PVPResultUnitInfoList.clear();
}

#ifdef CHUNG_SECOND_CLASS_CHANGE
/*virtual*/ void CX2PVPGame::UpdateUnitPointer()
{
	CX2Game::UpdateUnitPointer();
	for( int i = 0; i < (int)m_NPCUnitList.size(); i++ )
	{
		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		if( pCX2GUNPC != NULL )
		{
			//{{ kimhc // 2010.8.7 // 무조건 NPC에게 카메라가 가도록 하는 기능
			if( pCX2GUNPC->GetFocusCameraForce() == true 
				|| ( pCX2GUNPC->GetFocusCamera() == true && pCX2GUNPC->GetStartState() == (int)pCX2GUNPC->GetGameUnitState() ) )
				//}} kimhc // 2010.8.7 // 무조건 NPC에게 카메라가 가도록 하는 기능
			{
				m_optrFocusUnit = pCX2GUNPC;
				ResetFocusUnit();
				return;
			}
			if( m_optrFocusUnit == pCX2GUNPC && pCX2GUNPC->GetStartState() != (int)pCX2GUNPC->GetGameUnitState() )
			{
				if( GetMyUnit() != NULL )
					m_optrFocusUnit = GetMyUnit();
				else
					m_optrFocusUnit.Reset();
				ResetFocusUnit();
				return;
			}
		}
	}
}

#endif

/*virtual*/ void CX2PVPGame::UserUnitDieNot( KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT )
{
	// TO DO..
	// pvp 게임중 내가 누구에게 죽었는지의 정보출력을 만들어 주어야함.
#ifdef SERV_PVP_NEW_SYSTEM
	CX2GameUnit *pKilled = NULL;
	if( kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID > 0 )
		pKilled = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
	else
		pKilled = GetNPCUnitByUID( (int)kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );

	if( pKilled != NULL )
	{
		wstring buffer;

		if( kEGS_USER_UNIT_DIE_NOT.m_KillerNPCUID != -1 )
		{
			//몬스터에게 죽었다
			CX2GUNPC* pKiller = GetNPCUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KillerNPCUID );
			if( pKiller != NULL )
			{
				buffer = GET_REPLACED_STRING( ( STR_ID_191, "LL", pKilled->GetUnitName(), pKiller->GetNPCTemplet().m_Name ) );
			}
			KillUnit( true, kEGS_USER_UNIT_DIE_NOT.m_KillerNPCUID, -1, false, kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
		}
		else
		{
			//유저에게 죽었다
			CX2GameUnit *pKiller = NULL;
			CX2GameUnit *pMDKiller = NULL;

			if( kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID > 0 )
				pKiller = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID );
			else
				pKiller = GetNPCUnitByUID( (int)kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID );
			if( kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID > 0 )
				pKiller = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID );
			else
				pKiller = GetNPCUnitByUID( (int)kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID );

			if( pKilled != NULL )
			{
				if( pKiller != NULL )
				{
					if( pMDKiller != NULL && kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID != kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID )
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_192, "LLL", pKiller->GetUnitName(), pMDKiller->GetUnitName(), pKilled->GetUnitName() ) );
					}
					else
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_193, "LL", pKiller->GetUnitName(), pKilled->GetUnitName() ) );
					}
				}
				else
				{
					if( pMDKiller != NULL && kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID != kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID )
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_193, "LL", pMDKiller->GetUnitName(), pKilled->GetUnitName() ) );
					}
					else
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_194, "L", pKilled->GetUnitName() ) );
					}
				}
			}

			KillUnit( false, kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID, kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID, false, kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
		}

		if( NULL != g_pChatBox )
		{
			wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
			D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77


			g_pChatBox->AddChatLog( buffer.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
		}

#ifdef FIX_ARA_AWAKE_ABILITY_BUG
		ProcessIKillThisUser( kEGS_USER_UNIT_DIE_NOT );
#endif // FIX_ARA_AWAKE_ABILITY_BUG
	}

#else // SERV_PVP_NEW_SYSTEM

	CX2GUUser* pKilled = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
	if( pKilled != NULL )
	{
		wstring buffer;

		// fix!! 이브의 소환 몬스터에게 죽은 경우에 예외 처리 필요. 
		if( kEGS_USER_UNIT_DIE_NOT.m_KillerNPCUID != -1 )
		{
			//몬스터에게 죽었다
			CX2GUNPC* pKiller = GetNPCUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KillerNPCUID );
			if( pKiller != NULL && pKilled->GetUnit() != NULL )
			{
				buffer = GET_REPLACED_STRING( ( STR_ID_191, "SL", pKilled->GetUnit()->GetNickName(), pKiller->GetNPCTemplet().m_Name ) );
			}
			KillUnit( true, kEGS_USER_UNIT_DIE_NOT.m_KillerNPCUID, -1, false, kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
		}
		else
		{
			//유저에게 죽었다
			CX2GUUser* pKiller = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID );
			CX2GUUser* pMDKiller = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID  );
			if ( pKilled->GetUnit() != NULL )
			{
				if( pKiller != NULL && pKiller->GetUnit() != NULL )
				{
					if( pMDKiller != NULL && pMDKiller->GetUnit() != NULL && kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID != kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID )
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_192, "SSS", pKiller->GetUnit()->GetNickName(), pMDKiller->GetUnit()->GetNickName(), pKilled->GetUnit()->GetNickName() ) );
					}
					else
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_193, "SS", pKiller->GetUnit()->GetNickName(), pKilled->GetUnit()->GetNickName() ) );
					}
				}
				else
				{
					if( pMDKiller != NULL && pMDKiller->GetUnit() != NULL && kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID != kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID )
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_193, "SS", pMDKiller->GetUnit()->GetNickName(), pKilled->GetUnit()->GetNickName() ) );
					}
					else
					{
						buffer = GET_REPLACED_STRING( ( STR_ID_194, "S", pKilled->GetUnit()->GetNickName() ) );
					}
				}
			}

			KillUnit( false, kEGS_USER_UNIT_DIE_NOT.m_KillerUserUnitUID, kEGS_USER_UNIT_DIE_NOT.m_MaxDamageKillerUserUnitUID, false, kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
		}

		if( NULL != g_pChatBox )
		{
			wstring wstrColor = L"#CFF3F4D";							// 255, 63, 77
			D3DXCOLOR coTextColor(1.f, 0.24705f, 0.30196f, 1.f);		// 255, 63, 77


			g_pChatBox->AddChatLog( buffer.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, coTextColor, wstrColor );
		}
	}
#endif //SERV_PVP_NEW_SYSTEM
}

#ifdef NEW_CHARACTER_EL
/** @function : GetIsExsitancePartyMemberHPLessThanPer
	@brief : 체력 @1 % 이하인 파티원이 존재 여부 체크
	@param : fHPRate_ (체크할 HP 비율), pGUUserWhoCallFunction_ (이 함수를 호출한 CX2GUUser 포인터로 체크 대상에서 제외 및 같은 파티인지 알아낼 용도)
*/
/*virtual*/ 
bool CX2PVPGame::GetIsExsitancePartyMemberHPLessThanPer( const float fHPRate_, const CX2GUUser* pGUUserWhoCallFunction_ ) const
{
	BOOST_FOREACH( CX2GUUser* pGUUser, m_UserUnitList )
	{
		if ( NULL != pGUUser &&
			pGUUser->GetTeam() == pGUUserWhoCallFunction_->GetTeam() &&
			pGUUser->GetUnitUID() != pGUUserWhoCallFunction_->GetUnitUID() )
		{
			const float fHpPercent = 
				pGUUser->GetNowHp() / pGUUser->GetMaxHp();

			if( fHpPercent > 0.f &&
				fHpPercent <= fHPRate_ )
				return true;
		}
	}

	return false;
}
#endif // NEW_CHARACTER_EL


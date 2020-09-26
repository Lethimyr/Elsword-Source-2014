#include "StdAfx.h"
#include ".\x2pvpgameteamdeath.h"

CX2PVPGameTeamDeath::CX2PVPGameTeamDeath(void)
{
	m_pFontForReBirth			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont(XUF_DODUM_20_BOLD );
	m_pFontForWinKill			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont(XUF_DODUM_20_BOLD );


	m_MaxKillNum			= g_pData->GetPVPRoom()->GetWinKillNum();
	m_fReBirthTime			= 0.0f;
	m_fRemainRebirthTime	= 0.0f;

	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"TeamDeathScore", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharForRedAndBlueKillNum = new CKTDGPicChar( g_pData->GetComboPicCharBig(), pSeq );
	m_pPicCharForRedAndBlueKillNum->SetWidth( 26 );
	//m_pPicCharForRedAndBlueKillNum = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
	//m_pPicCharForRedAndBlueKillNum->SetWidth( 21 );

	m_BeforeRedTeamKill = -1;
	m_BeforeBlueTeamKill = -1;
}

CX2PVPGameTeamDeath::~CX2PVPGameTeamDeath(void)
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
/*virtual*/ void CX2PVPGameTeamDeath::Release()
{
	//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForReBirth );
	//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForWinKill );
	//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForRedAndBlueKillNum );
	m_pPicCharForRedAndBlueKillNum->Clear();
	SAFE_DELETE( m_pPicCharForRedAndBlueKillNum );

	CX2PVPGame::Release();
} 
HRESULT	CX2PVPGameTeamDeath::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2PVPGame::OnFrameMove( fTime, fElapsedTime );

	m_fRemainRebirthTime -= m_fElapsedTime;
	if( m_fRemainRebirthTime < 0.0f )
		m_fRemainRebirthTime = 0.0f; 

#ifndef SERV_PVP_NEW_SYSTEM
	int redTeamVPMax	= 0;
	int blueTeamVPMax	= 0;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser != NULL )
		{
			if( NULL != pCX2GUUser->GetUnit() )
			{
				if( pCX2GUUser->GetTeam() == CX2Room::TN_RED )
				{
					redTeamVPMax += pCX2GUUser->GetUnit()->GetUnitData().m_VSPointMax;
				}
				else if( pCX2GUUser->GetTeam() == CX2Room::TN_BLUE )
				{
					blueTeamVPMax += pCX2GUUser->GetUnit()->GetUnitData().m_VSPointMax;
				}
			}
			else
			{
				DeleteUserUnitByUID( pCX2GUUser->GetUnitUID() );
				ErrorLog( XEM_ERROR113 );
			}
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	int VPMaxGap = redTeamVPMax - blueTeamVPMax;
	if( VPMaxGap > 0 )
	{
		m_TeamVPMaxAdvRed	= 0;
		m_TeamVPMaxAdvBlue	= VPMaxGap;		
	}
	else
	{
		m_TeamVPMaxAdvRed	= -VPMaxGap;
		m_TeamVPMaxAdvBlue	= 0;		
	}
#endif

	if ( m_BeforeRedTeamKill != m_RedTeamKill || m_BeforeBlueTeamKill != m_BlueTeamKill )
	{
		m_pPicCharForRedAndBlueKillNum->Clear();
		wstringstream tempString;
		//tempString << m_BeforeRedTeamKill << L" ; " << m_BeforeBlueTeamKill;
		//m_pPicCharForRedAndBlueKillNum->DrawText( tempString.str().c_str(), D3DXVECTOR3( 520, 212, 0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER );

#ifdef DIALOG_SHOW_TOGGLE
		bool bHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();
		if( bHide == true )
			return S_OK;
#endif
		
		UpdateScore();		
	}

	return S_OK;
}

HRESULT CX2PVPGameTeamDeath::OnFrameRender()
{
	CX2PVPGame::OnFrameRender();

	if( m_GameState == GS_PLAY )
	{
#ifdef DIALOG_SHOW_TOGGLE
		bool bHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();
		if( bHide == true )
			return S_OK;
#endif
		CX2GUUser* pCX2GUUser = GetMyUnit();
		if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
		{
			m_pFontForWinKill->OutTextXY( 516, 43, GET_REPLACED_STRING( ( STR_ID_444, "i", m_MaxKillNum ) ), D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		}

		//swprintf( buf, L"%d : %d", m_RedTeamKill, m_BlueTeamKill );
		//m_pFontForRedAndBlueKillNum->OutTextXY( 512, 171, buf, D3DXCOLOR(1,1,1,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );

#ifdef PVP_SEASON2
		if( GetMyUnit() != NULL && GetMyUnit()->GetRebirthTime() > 0.0f && GetMyUnit()->GetNowHp() <= 0.0f && GetMyUnit()->GetRebirthTime() < 31.0f )
#else
		if( GetMyUnit() != NULL && GetMyUnit()->GetRebirthTime() > 0.0f && GetMyUnit()->GetNowHp() <= 0.0f && GetMyUnit()->GetRebirthTime() < 11.0f )
#endif
		{
			m_pFontForReBirth->OutTextMultiline( 512, 500, GET_REPLACED_STRING( ( STR_ID_442, "i", ( int )GetMyUnit()->GetRebirthTime() ) ), D3DXCOLOR(1,1,1,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), 1.0f, NULL, DT_CENTER );
		}
	}

	return S_OK;
}

void CX2PVPGameTeamDeath::AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex )
{
	CX2PVPGame::AddUserUnit( pSlotData, slotIndex );

	//for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	//{
	//	CX2GUUser* pCX2GUUser = m_UserUnitList[i];
	//	if( pCX2GUUser != NULL )
	//	{
	//		pCX2GUUser->InitPosition( false, i );
	//	}
	//}
}

bool CX2PVPGameTeamDeath::DetermineLastKill()
{
	return false;
}

void CX2PVPGameTeamDeath::UserUnitDieNot( KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT )
{
	CX2PVPGame::UserUnitDieNot( kEGS_USER_UNIT_DIE_NOT );

	if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		CX2GUUser* pKilled = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
		if( pKilled != NULL )
		{
			if( m_bLastKillCheck == false )
			{

//#ifdef PVP_BOSS_COMBAT_TEST
//
//				if( NULL != g_pX2Room )
//				{
//					CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( pKilled->GetUnitUID() );
//					if( NULL != pSlotData )
//					{
//						if( true == pSlotData->GetIsBoss() )
//						{
//							m_bLastKillCheck = true;
//
//							g_pKTDXApp->StopFrame( 80 );
//							g_pKTDXApp->GetDGManager()->ClearScreen( 3 );
//							g_pKTDXApp->SlowFrame( 0.2f, 1.5f );
//
//							SetTimerFocusUnit( pKilled, 1.0f );
//
//							for( int i = 0; i < (int)GetUnitNum(); i++ )
//							{
//								CX2GameUnit* pCX2GameUnit = GetUnit( i );
//								if( pCX2GameUnit != NULL )
//								{
//									pCX2GameUnit->SetForceInvincible( 9999.0f );
//								}
//							}
//						}
//					}
//				}
//
//#endif PVP_BOSS_COMBAT_TEST


				if( m_RedTeamKill == m_MaxKillNum -1 && pKilled->GetTeam() == CX2Room::TN_BLUE )
				{
					m_bLastKillCheck = true;

					g_pKTDXApp->StopFrame( 80 );
					g_pKTDXApp->GetDGManager()->ClearScreen( 5 );
					g_pKTDXApp->SlowFrame( 0.2f, 1.5f );

					SetTimerFocusUnit( pKilled, 1.0f );

					for( int i = 0; i < (int)GetUnitNum(); i++ )
					{
						CX2GameUnit* pCX2GameUnit = GetUnit( i );
						if( pCX2GameUnit != NULL )
							pCX2GameUnit->SetForceInvincible( 9999.0f );
					}
				}
				if( m_BlueTeamKill == m_MaxKillNum -1 && pKilled->GetTeam() == CX2Room::TN_RED )
				{
					m_bLastKillCheck = true;

					g_pKTDXApp->StopFrame( 80 );
					g_pKTDXApp->GetDGManager()->ClearScreen( 5 );
					g_pKTDXApp->SlowFrame( 0.2f, 1.5f );

					SetTimerFocusUnit( pKilled, 1.0f );

					for( int i = 0; i < (int)GetUnitNum(); i++ )
					{
						CX2GameUnit* pCX2GameUnit = GetUnit( i );
						if( pCX2GameUnit != NULL )
							pCX2GameUnit->SetForceInvincible( 9999.0f );
					}
				}
			}		
		}
	}

}

void CX2PVPGameTeamDeath::UpdateScore()
{
	if( CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() )
		return;

	if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
		return;
	

#ifdef MODIFY_PVP_SCORE_UI
	if( NULL != GetMyUnit() )
	{
		CX2Room::TEAM_NUM eTeamNum = static_cast<CX2Room::TEAM_NUM>(GetMyUnit()->GetTeam());
		if( CX2Room::TN_RED == eTeamNum )
		{
			SetScoreTexture( m_RedTeamKill, m_BlueTeamKill, true );
		}
		else
		{
			SetScoreTexture( m_BlueTeamKill, m_RedTeamKill, false );
		}				
	}
	else if( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver )	/// 옵저버 일 땐, 디폴트로 갱신
	{
		SetScoreTexture( m_RedTeamKill, m_BlueTeamKill );
	}
				
#else
	CX2StatePVPGame* pState = (CX2StatePVPGame*) g_pMain->GetNowState();
	if ( NULL != pState && NULL != pState->GetDlgScore() )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pState->GetDlgScore()->GetControl(L"StaticPVPGameState");
		if( NULL != pStatic)
		{
			//일단 안보여주도록 초기화.
			for(int i=3; i<6; i++) // 3~4 : 레드팀 킬수,  5~6: 블루팀 킬수
			{
				pStatic->GetPicture(i)->SetShow(false);
			}


			const wstring MAGIC_TEXTURE_KEY[] = {	L"_NUM_0",
				L"_NUM_1",
				L"_NUM_2",
				L"_NUM_3",
				L"_NUM_4",
				L"_NUM_5",
				L"_NUM_6",
				L"_NUM_7",
				L"_NUM_8",
				L"_NUM_9" };

			//레드 팀.
			int iRedKillTen	= (m_RedTeamKill / 10) % 10;
			int iRedKill	= m_RedTeamKill % 10;;;

			wstring KeyName;
			if( iRedKillTen > 0 )
			{
				KeyName = L"RED" + MAGIC_TEXTURE_KEY[iRedKillTen] ;
				//10의자리
				pStatic->GetPicture(2)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(2)->SetShow(true);
			}
			//1의자리
			KeyName = L"RED" + MAGIC_TEXTURE_KEY[iRedKill] ;
			pStatic->GetPicture(3)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
			pStatic->GetPicture(3)->SetShow(true);


			//블루 팀
			int iBlueKillTen = (m_BlueTeamKill / 10) % 10;
			int iBlueKill	 = m_BlueTeamKill % 10;;;

			if( iBlueKillTen > 0 )
			{
				KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iBlueKillTen] ;
				//10의자리
				pStatic->GetPicture(4)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(4)->SetShow(true);

				//1의자리
				KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iBlueKill] ;
				pStatic->GetPicture(5)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(5)->SetShow(true);
			}
			else
			{
				//1의자리 ( 두자리 수가 아닐 때는 1의 자리 수를 10의 자리 위치로 
				KeyName = L"BLUE" + MAGIC_TEXTURE_KEY[iBlueKill] ;
				pStatic->GetPicture(4)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(4)->SetShow(true);
			}

			m_BeforeRedTeamKill = m_RedTeamKill;
			m_BeforeBlueTeamKill = m_BlueTeamKill;

		}	
	}
#endif //MODIFY_PVP_SCORE_UI
}
#ifdef MODIFY_PVP_SCORE_UI
/** @function : SetScoreTexture
	@brief : 레드팀/블루팀 구분하여 스코어 UI 설정
			(우리팀 스코어가 왼쪽에 위치하도록)
	@param : iMyTeamKill_(우리팀 킬수) iOtherTeamKill_(적군 팀 킬수) bIsRedTeam_(내가 빨간팀인가?)
*/
void CX2PVPGameTeamDeath::SetScoreTexture( int iMyTeamKill_, int iOtherTeamKill_, bool bIsRedTeam_ /*= true*/ )
{
	CX2StatePVPGame* pState = (CX2StatePVPGame*) g_pMain->GetNowState();
	if ( NULL != pState && NULL != pState->GetDlgScore() )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)pState->GetDlgScore()->GetControl(L"StaticPVPGameState");
		if( NULL != pStatic)
		{
			//일단 안보여주도록 초기화.
			for(int i=4; i<8; ++i) // 4~5 : 레드팀 킬수,  6~7: 블루팀 킬수
			{
				if( NULL == pStatic->GetPicture(i) )
					return;

				pStatic->GetPicture(i)->SetShow(false);
			}

			const wstring MAGIC_TEXTURE_KEY[] = {	L"_NUM_0",
				L"_NUM_1",
				L"_NUM_2",
				L"_NUM_3",
				L"_NUM_4",
				L"_NUM_5",
				L"_NUM_6",
				L"_NUM_7",
				L"_NUM_8",
				L"_NUM_9" };

			wstring MyTeamKeyName = L"";
			wstring OtherTeamKeyName = L"";

			if( NULL != pStatic->GetPicture(0) &&
				NULL != pStatic->GetPicture(1) &&
				NULL != pStatic->GetPicture(2) )
			{
				if( true == bIsRedTeam_ ) 
				{
					MyTeamKeyName = L"RED";		//아군팀 레드
					OtherTeamKeyName = L"BLUE"; //적군팀 블루
					pStatic->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", L"PVP_SCORE_BG"); 
					pStatic->GetPicture(1)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", L"PVP_RED"); 
					pStatic->GetPicture(2)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", L"PVP_BLUE");
				}
				else
				{
					MyTeamKeyName = L"BLUE";	//아군팀 블루
					OtherTeamKeyName = L"RED";	//적군팀 레드
					pStatic->GetPicture(0)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", L"PVP_SCORE_BG_2"); 
					pStatic->GetPicture(1)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", L"PVP_BLUE_LEFT"); 
					pStatic->GetPicture(2)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", L"PVP_RED_RIGHT");
				}
			}

		
			//우리 팀 (왼쪽에 표시)
			int iMyTeamKillTen = (iMyTeamKill_ / 10) % 10;
			int iMyTeamKill	= iMyTeamKill_ % 10;;;

			wstring KeyName;
			if( iMyTeamKillTen > 0 )
			{
				KeyName = MyTeamKeyName + MAGIC_TEXTURE_KEY[iMyTeamKillTen] ;
				//10의자리
				pStatic->GetPicture(4)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(4)->SetShow(true);
			}
			//1의자리
			KeyName = MyTeamKeyName + MAGIC_TEXTURE_KEY[iMyTeamKill] ;
			pStatic->GetPicture(5)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
			pStatic->GetPicture(5)->SetShow(true);


			//상대 팀(오른쪽에 표시)
			int iOtherTeamKillTen = (iOtherTeamKill_ / 10) % 10;
			int iOtherTeamKill	 = iOtherTeamKill_ % 10;;;

			if( iOtherTeamKillTen > 0 )
			{
				KeyName = OtherTeamKeyName + MAGIC_TEXTURE_KEY[iOtherTeamKillTen] ;
				//10의자리
				pStatic->GetPicture(6)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
				pStatic->GetPicture(6)->SetShow(true);
			}
			//1의자리 ( 두자리 수가 아닐 때는 1의 자리 수를 10의 자리 위치로 
			KeyName = OtherTeamKeyName + MAGIC_TEXTURE_KEY[iOtherTeamKill] ;
			pStatic->GetPicture(7)->SetTex(L"DLG_UI_Common_Texture65_NEW.TGA", KeyName.c_str());
			pStatic->GetPicture(7)->SetShow(true);

			m_BeforeRedTeamKill = m_RedTeamKill;
			m_BeforeBlueTeamKill = m_BlueTeamKill;
		}
	}
}
#endif //MODIFY_PVP_SCORE_UI
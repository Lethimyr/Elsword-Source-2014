#include "StdAfx.h"
#include ".\x2pvpgamesurvival.h"

CX2PVPGameSurvival::CX2PVPGameSurvival(void)
{
	m_pFontForReBirth		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_20_BOLD );
	//m_pFontForMyAndWinKill	= g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUKFont(L"HY헤드라인M", 60, 1, true );
	m_pFontForWinKill			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );

	m_MaxKillNum			= g_pData->GetPVPRoom()->GetWinKillNum();
	m_fReBirthTime			= 0.0f;
	m_fRemainRebirthTime	= 0.0f;
}

CX2PVPGameSurvival::~CX2PVPGameSurvival(void)
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
/*virtual*/ void CX2PVPGameSurvival::Release()
{
	//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForReBirth );
	//	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFontForWinKill );
	CX2PVPGame::Release();
}

HRESULT	CX2PVPGameSurvival::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2PVPGame::OnFrameMove( fTime, fElapsedTime );

	m_fRemainRebirthTime -= m_fElapsedTime;
	if( m_fRemainRebirthTime < 0.0f )
		m_fRemainRebirthTime = 0.0f; 


	return S_OK;
}

HRESULT CX2PVPGameSurvival::OnFrameRender()
{
	CX2PVPGame::OnFrameRender();

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return S_OK;
#endif

	if( m_GameState == GS_PLAY )
	{
#ifdef PVP_SEASON2
		if( GetMyUnit() != NULL && GetMyUnit()->GetRebirthTime() > 0.0f && GetMyUnit()->GetNowHp() <= 0.0f && GetMyUnit()->GetRebirthTime() < 31.0f )
#else
		if( GetMyUnit() != NULL && GetMyUnit()->GetRebirthTime() > 0.0f && GetMyUnit()->GetNowHp() <= 0.0f && GetMyUnit()->GetRebirthTime() < 11.0f )
#endif
		{
			m_pFontForReBirth->OutTextMultiline( 512, 500, GET_REPLACED_STRING( ( STR_ID_442, "i", ( int )GetMyUnit()->GetRebirthTime() ) ), D3DXCOLOR(1,1,1,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), 1.0f, NULL, DT_CENTER );
		}

		if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
		{
			m_pFontForWinKill->OutTextXY( 512, 95, GET_REPLACED_STRING( ( STR_ID_443, "i", m_MaxKillNum ) ), D3DXCOLOR(1,225/255.0f,90/255.0f,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		}
	}

	return S_OK;
}

void CX2PVPGameSurvival::AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex )
{
	CX2PVPGame::AddUserUnit( pSlotData, slotIndex );

	// fix!! : linemap에 userunitlist 크기 만큼 start pos이 찍혀 있어야 한다. 아니면 뻗거나 오동작
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CSLock locker( m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser != NULL )
		{
			pCX2GUUser->InitPosition( false, i );
		}
	}

}

bool CX2PVPGameSurvival::DetermineLastKill()
{
	return false;
}

void CX2PVPGameSurvival::UserUnitDieNot( KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT )
{
	CX2PVPGame::UserUnitDieNot( kEGS_USER_UNIT_DIE_NOT );


	if( KPVPChannelInfo::PCC_PLAY != g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
	{
		CX2GUUser* pKilled = GetUserUnitByUID( kEGS_USER_UNIT_DIE_NOT.m_KilledUserUnitUID );
		if( pKilled != NULL && m_bLastKillCheck == false )
		{
			for ( int i = 0; i < GetUserUnitListSize(); i++ )
			{
				CX2GUUser* pGUUser = GetUserUnit(i);
				if ( pGUUser == NULL )
					continue;

				if( (int)pGUUser->m_KillUserUnitList.size() == m_MaxKillNum )
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




#include "StdAfx.h"
#include ".\x2combomanager.h"

CX2ComboManager::CX2ComboManager( float fComboIntervalTime )
{	
	m_bMyUnit				= false;
	m_pUnit					= NULL;
	m_ComboNum				= 0;

	m_fTime					= 0.0f;
	m_fEffTime				= 0.0f;
	m_fComboIntervalTime	= fComboIntervalTime;

	m_pPicChar				= NULL;
	m_pPicCharBig			= NULL;

	for ( int i = 0; i < 13; ++i )
		m_hInterjection[i]	= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hComboDamage = INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_fComboDamage = 0.f;
	m_fComboCount = 0;

	m_hScoreBonus			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_pScoreBonusPicChar	= NULL;
	m_ComboScore			= 0;
	m_ComboScoreFull		= 0;
	m_hGoodBonus			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hNiceBonus			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hCoolBonus			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hGreatBonus			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hPerfectBonus			= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_iMyComboDamage = 0;
	m_iMyMaxComboDamage = 0;
	m_pPicCharComboDamage = NULL;
	m_pPicCharComboMaxDamage = NULL;
	
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM	
	m_iUsingSubStageComboScore = 0;		// 서브 스테이지의 콤보 스코어에 사용할 값을 초기화
#endif	// SERV_DUNGEON_FORCED_EXIT_SYSTEM

	ZeroMemory( m_ComboContent, 256 );
}

CX2ComboManager::~CX2ComboManager(void)
{
	m_vecComboCount.clear();

	for ( int i = 0; i < 13; ++i )
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hInterjection[i] );

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hComboDamage );

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hScoreBonus );

	//SAFE_DELETE( m_pPicChar );
	//SAFE_DELETE( m_pPicCharBig );

	//m_pPicChar->Clear();
	m_pPicChar = NULL;
	//m_pPicCharBig->Clear();
	m_pPicCharBig = NULL;
	//m_pScoreBonusPicChar->Clear();
	m_pScoreBonusPicChar = NULL;

	SAFE_DELETE( m_pPicCharComboDamage );
	SAFE_DELETE( m_pPicCharComboMaxDamage );

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hGoodBonus );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hNiceBonus );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hCoolBonus );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hGreatBonus );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hPerfectBonus );
}


#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
int CX2ComboManager::GetSubStageComboScore() const			// 이 함수는 서브 스테이지가 이동 할 때만 호출되어야 합니다.
{
	return m_ComboScoreFull - m_iUsingSubStageComboScore;	// 스테이지의 누적 콤보 스코어에 이번 서브 스테이지를 제외한 누적 콤보 스코어를 빼서 갖고 있는다.
}


void CX2ComboManager::ResetSubStageComboScore() 				// 현재까지 누적된 ComboScore 를 m_iUsingSubStageComboScore 에 대입한다.
{
	m_iUsingSubStageComboScore = m_ComboScoreFull;	
}

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM





void CX2ComboManager::SetMyUnit( bool bMyUnit, CX2GUUser* pUnit )
{
	m_bMyUnit	= bMyUnit;
	m_pUnit		= pUnit;
	if( m_bMyUnit == true )
	{
		wstring		wstrParticle[13] = { L"NewComboGood",		L"NewComboNice",		L"NewComboGreat",		L"NewComboDelux",
										 L"NewComboSuperb",		L"NewComboSuperior",	L"NewComboAwesome",		L"NewComboExcellent",
										 L"NewComboMarvelous",	L"NewComboGorgeous",	L"NewComboHonorable",	L"NewComboMonstrous",	L"NewComboEvil" };

		for ( int i = 0; i < 13; ++i )
		{
			if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hInterjection[i] )
			{
				m_hInterjection[i]	= g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  wstrParticle[i].c_str(), 0,0,0, 0,0 );
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[i] );
				if( NULL != pSeq )
				{
					pSeq->SetDynamicChain( false );
				}
			}
		}

		if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hComboDamage )
		{
			m_hComboDamage = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"NewComboDamage", 0,0,0, 0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hComboDamage );
			if( NULL != pSeq )
			{
				pSeq->SetDynamicChain( false );
			}
		}

		if( m_pPicChar == NULL )
		{
			if( NULL != g_pData->GetComboPicChar() )
			{
				m_pPicChar = g_pData->GetComboPicChar();
				m_pPicChar->Clear();
				g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pPicChar->GetSeq() );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_pPicChar->GetSeq() );
			}

		}
		if( m_pPicCharBig == NULL )
		{
			if( NULL != g_pData->GetComboPicCharBig() )
			{
				m_pPicCharBig = g_pData->GetComboPicCharBig();
				m_pPicCharBig->Clear();
				g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pPicCharBig->GetSeq() );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_pPicCharBig->GetSeq() );
			}
		}


		if( m_pPicCharComboDamage == NULL && g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		{			
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"GameScorePicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
			pSeq->SetDynamicChain( false );
			m_pPicCharComboDamage = new CKTDGPicChar( L"HQ_CPC_", pSeq, L"0123456789" );
			m_pPicCharComboDamage->SetWidth( 15.0f );			
			m_pPicCharComboDamage->Clear();
			g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pPicCharComboDamage->GetSeq() );
			g_pKTDXApp->GetDGManager()->AddObjectChain( m_pPicCharComboDamage->GetSeq() );


			CKTDGParticleSystem::CParticleEventSequence* pSeqMax = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"GameScorePicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
			pSeqMax->SetDynamicChain( false );
			m_pPicCharComboMaxDamage = new CKTDGPicChar( L"HQ_CPC_", pSeqMax, L"0123456789" );
			m_pPicCharComboMaxDamage->SetWidth( 15.0f );
			m_pPicCharComboMaxDamage->SetColor( D3DXCOLOR(1.f, 0.f, 0.f, 1.f) );
			m_pPicCharComboMaxDamage->Clear();
			g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pPicCharComboMaxDamage->GetSeq() );
			g_pKTDXApp->GetDGManager()->AddObjectChain( m_pPicCharComboMaxDamage->GetSeq() );
		}


	}
}

HRESULT CX2ComboManager::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	
	Verify();

	if( m_pPicChar != NULL )
		m_pPicChar->OnFrameMove( fElapsedTime );

		
	if( m_pPicCharComboDamage != NULL )
		m_pPicCharComboDamage->OnFrameMove( fElapsedTime );
	if( m_pPicCharComboMaxDamage != NULL )
		m_pPicCharComboMaxDamage->OnFrameMove( fElapsedTime );

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	{
		if( m_pPicCharComboDamage != NULL && m_pPicCharComboMaxDamage != NULL &&
			m_pPicCharComboDamage->GetSeq() != NULL && m_pPicCharComboMaxDamage->GetSeq() != NULL )
		{				
			m_pPicCharComboDamage->GetSeq()->SetShowObject(false);
			m_pPicCharComboMaxDamage->GetSeq()->SetShowObject(false);
		}
	}
#endif //DIALOG_SHOW_TOGGLE

	if ( m_fTime >= m_fComboIntervalTime )
	{
		if( m_ComboNum > 1 )
			m_vecComboCount.push_back( m_ComboNum );

		m_fTime			= 0.0f;		
		g_pX2Game->AddGameScore( m_ComboScore, 0.5f );
		m_ComboScoreFull += m_ComboScore;

		//{{ JHKang / 강정훈 / 2011.02.11 / 던전 랭크 개선 콤보 점수 개선
#ifndef DUNGEON_RANK_NEW
		if( m_ComboNum >= 40 )
		{
			g_pX2Game->AddGameScore( 300, 0.9f );
			m_ComboScoreFull += 300;
			g_pX2Game->AddGameScore( 800, 1.3f );
			m_ComboScoreFull += 800;
			g_pX2Game->AddGameScore( 1500, 1.7f );
			m_ComboScoreFull += 1500;
			g_pX2Game->AddGameScore( 3500, 2.1f );
			m_ComboScoreFull += 3500;
			g_pX2Game->AddGameScore( 5000, 2.5f );
			m_ComboScoreFull += 5000;
		}
		else if( m_ComboNum >= 25 )
		{
			g_pX2Game->AddGameScore( 300, 0.9f );
			m_ComboScoreFull += 300;
			g_pX2Game->AddGameScore( 800, 1.3f );
			m_ComboScoreFull += 800;
			g_pX2Game->AddGameScore( 1500, 1.7f );
			m_ComboScoreFull += 1500;
			g_pX2Game->AddGameScore( 3500, 2.1f );
			m_ComboScoreFull += 3500;
		}
		else if( m_ComboNum >= 14 )
		{
			g_pX2Game->AddGameScore( 300, 0.9f );
			m_ComboScoreFull += 300;
			g_pX2Game->AddGameScore( 800, 1.3f );
			m_ComboScoreFull += 800;
			g_pX2Game->AddGameScore( 1500, 1.7f );
			m_ComboScoreFull += 1500;
		}
		else if( m_ComboNum >= 8 )
		{
			g_pX2Game->AddGameScore( 300, 0.9f );
			m_ComboScoreFull += 300;
			g_pX2Game->AddGameScore( 800, 1.3f );
			m_ComboScoreFull += 800;
		}
		else if( m_ComboNum >= 4 )
		{
			g_pX2Game->AddGameScore( 300, 0.9f );
			m_ComboScoreFull += 300;
		}
#endif DUNGEON_RANK_NEW
		//}} JHKang / 강정훈 / 2011.02.11 / 던전 랭크 개선 콤보 점수 개선

		m_ComboNum		= 0;
		m_ComboScore	= 0;
		m_fComboDamage	= 0.f;
		m_fComboCount = 0;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        auto SetParticleAgeAndEventTimer = []( CKTDGParticleSystem::CParticle& kParticle )
        {
			kParticle.SetAge( 99998.f );
			kParticle.SetEventTimer( 99998.f );
        };
        auto SetParticleAgeAndEventTimerCond = []( CKTDGParticleSystem::CParticle& kParticle )
        {
            if ( kParticle.GetAge() < 99990.0f )
            {
			    kParticle.SetAge( 99998.f );
			    kParticle.SetEventTimer( 99998.f );
            }
        };
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		if( m_hScoreBonus != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hScoreBonus );
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pSeq->ApplyFunctionToParticles( SetParticleAgeAndEventTimer );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticle*, pParticle, pSeq->m_ParticleList )
				{
					if ( pParticle != NULL )
					{
						pParticle->SetAge( 99998 );
						pParticle->SetEventTimer( 99998 );
					}//if
				}		
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pSeq->ClearAllParticle(); // fix!! 왜 지우지? 
			}
		}

		if( m_hGoodBonus != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hGoodBonus );
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pSeq->ApplyFunctionToParticles( SetParticleAgeAndEventTimerCond );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticle*, pParticle, pSeq->m_ParticleList )
				{
					if( pParticle != NULL && pParticle->GetAge() < 99990.0f )
					{
						pParticle->SetAge( 99998.0f );
						pParticle->SetEventTimer( 99998.0f );	
					}	
				}			
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}
		if( m_hNiceBonus != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hNiceBonus );
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pSeq->ApplyFunctionToParticles( SetParticleAgeAndEventTimerCond );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticle*, particle, pSeq->m_ParticleList )
				{
					if( particle != NULL && particle->GetAge() < 99990.0f )
					{
						particle->SetAge( 99998.0f );
						particle->SetEventTimer( 99998.0f );	
					}	
				}		
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}
		if( m_hCoolBonus != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hCoolBonus );
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pSeq->ApplyFunctionToParticles( SetParticleAgeAndEventTimerCond );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticle*, particle, pSeq->m_ParticleList )
				{
					if( particle != NULL && particle->GetAge() < 99990.0f )
					{
						particle->SetAge( 99998.0f );
						particle->SetEventTimer( 99998.0f );	
					}	
				}			
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}
		if( m_hGreatBonus != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hGreatBonus );
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pSeq->ApplyFunctionToParticles( SetParticleAgeAndEventTimerCond );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticle*, particle, pSeq->m_ParticleList )
				{
					if( particle != NULL && particle->GetAge() < 99990.0f )
					{
						particle->SetAge( 99998.0f );
						particle->SetEventTimer( 99998.0f );	
					}	
				}	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}
		if( m_hPerfectBonus != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hPerfectBonus );
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pSeq->ApplyFunctionToParticles( SetParticleAgeAndEventTimerCond );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticle*, particle, pSeq->m_ParticleList )
				{
					if( particle != NULL && particle->GetAge() < 99990.0f )
					{
						particle->SetAge( 99998.0f );
						particle->SetEventTimer( 99998.0f );
					}	
				}	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}
	}
	else
	{
		m_fTime += fElapsedTime;
	}
	if( m_ComboNum == 0 )
	{
		m_fEffTime += fElapsedTime;
		if( m_fEffTime + m_fComboIntervalTime > 2.0f )
		{
			if( m_hComboDamage != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hComboDamage );
				if( NULL != pSeq )
				{
					pSeq->ClearAllParticle();
				}
				else
				{
					m_hComboDamage = INVALID_PARTICLE_SEQUENCE_HANDLE;
				}
			}

			m_fEffTime = 0.0f;
		}
	}
	else
	{
		m_fEffTime = 0.0f;
	}

	return S_OK;
}


//{{ robobeg : 2008-10-28
//HRESULT CX2ComboManager::OnFrameRender()
//{
//	KTDXPROFILE();
//
//	return S_OK;
//}
//}} robobeg : 2008-10-28
#ifdef ELSWORD_SHEATH_KNIGHT
int CX2ComboManager::AddCombo()
#else
void CX2ComboManager::AddCombo()
#endif ELSWORD_SHEATH_KNIGHT
{
	m_ComboNum++;
	m_fTime = 0.0f;
	m_fEffTime = 0.0f;

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	{
#ifdef DUNGEON_RANK_NEW
		if ( m_ComboNum >= 1 )
		{
			const int MAGIC_NUMBER_SECTION = 5;		// 구간 값
			m_ComboScore = static_cast<int>( ceil( static_cast<double>(static_cast<double>(m_ComboNum) / static_cast<double>(MAGIC_NUMBER_SECTION) ) ) ) * m_ComboNum;
		}
#endif //DUNGEON_RANK_NEW

		if( m_pPicCharComboDamage != NULL && m_pPicCharComboMaxDamage != NULL &&
			m_pPicCharComboDamage->GetSeq() != NULL && m_pPicCharComboMaxDamage->GetSeq() != NULL )
		{				
			m_pPicCharComboDamage->GetSeq()->SetShowObject(false);
			m_pPicCharComboMaxDamage->GetSeq()->SetShowObject(false);
		}

#ifdef ELSWORD_SHEATH_KNIGHT
		return m_ComboNum;
#else
		return;
#endif ELSWORD_SHEATH_KNIGHT
	}
#endif //DIALOG_SHOW_TOGGLE

	if( m_bMyUnit == true )
	{
		if( m_hComboDamage != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hComboDamage );
			if( NULL != pSeq )
			{
				pSeq->ClearAllParticle();
				pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
			}
		}
		if( m_pPicChar != NULL )
		{
			m_pPicChar->Clear();

			const float fFinalDamage = m_pUnit->GetCurrentDamage();
#ifdef ADD_DAMAGE_LOG
			if( NULL != m_pUnit &&
				true == m_pUnit->IsMyUnit() )
			{
				g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_2632, "i", static_cast<int>(fFinalDamage) ) ), KEGS_CHAT_REQ::CPT_SYSTEM );
			}
#endif // ADD_DAMAGE_LOG

			const int iMyLevel = m_pUnit->GetUnitLevel();

			if ( 0 < fFinalDamage && 0 < iMyLevel )
			{
				float fRate = 60.f;

				if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
					fRate = 6.f;

				m_fComboDamage += ceil( fFinalDamage / ( 400.f + ( ( iMyLevel + 5 ) * fRate ) ) );
				m_pPicChar->SetWidth( 30.0f );
				m_pPicChar->DrawText( (int)m_fComboDamage, D3DXVECTOR3(-100, 327, 0), D3DXVECTOR3(1, 0, 0), CKTDGPicChar::AT_RIGHT );
			}
		}
		if ( m_fComboCount < 1 && 5.f <= m_fComboDamage && 10.f > m_fComboDamage )
		{
			if( m_hInterjection[0] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[0] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 1;
			}
		}
		else if ( m_fComboCount < 2 && 10.f <= m_fComboDamage && 15.f > m_fComboDamage )
		{
			if( m_hInterjection[1] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[1] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 2;
			}
		}
		else if ( m_fComboCount < 3 && 15.f <= m_fComboDamage && 20.f > m_fComboDamage )
		{
			if( m_hInterjection[2] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[2] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 3;
			}
		}
		else if ( m_fComboCount < 4 && 20.f <= m_fComboDamage && 30.f > m_fComboDamage )
		{
			if( m_hInterjection[3] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[3] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 4;
			}
		}
		else if ( m_fComboCount < 5 && 30.f <= m_fComboDamage && 40.f > m_fComboDamage )
		{
			if( m_hInterjection[4] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[4] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 5;
			}
		}
		else if ( m_fComboCount < 6 && 40.f <= m_fComboDamage && 55.f > m_fComboDamage )
		{
			if( m_hInterjection[5] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[5] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 6;
			}
		}
		else if ( m_fComboCount < 7 && 55.f <= m_fComboDamage && 70.f > m_fComboDamage )
		{
			if( m_hInterjection[6] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[6] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 7;
			}
		}
		else if ( m_fComboCount < 8 && 70.f <= m_fComboDamage && 85.f > m_fComboDamage )
		{
			if( m_hInterjection[7] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[7] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 8;
			}
		}
		else if ( m_fComboCount < 9 && 85.f <= m_fComboDamage && 100.f > m_fComboDamage )
		{
			if( m_hInterjection[8] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[8] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 9;
			}
		}
		else if ( m_fComboCount < 10 && 100.f <= m_fComboDamage && 125.f > m_fComboDamage )
		{
			if( m_hInterjection[9] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[9] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 10;
			}
		}
		else if ( m_fComboCount < 11 && 125.f <= m_fComboDamage && 150.f > m_fComboDamage )
		{
			if( m_hInterjection[10] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[10] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 11;
			}
		}
		else if ( m_fComboCount < 12 && 150.f <= m_fComboDamage && 200.f > m_fComboDamage )
		{
			if( m_hInterjection[11] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[11] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 12;
			}
		}
		else if ( m_fComboCount < 13 && 200.f <= m_fComboDamage )
		{
			if( m_hInterjection[12] != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				ClearParticleSeqGoodSeries();

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[12] );
				if( NULL != pSeq )
				{
					pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
				}

				m_fComboCount = 13;
			}
		}

		//{{ JHKang / 강정훈 / 2011.02.11 / 던전 랭크 개선 콤보 점수 개선
#ifdef DUNGEON_RANK_NEW
		if ( m_ComboNum >= 1 )
		{
			const int MAGIC_NUMBER_SECTION = 5;		// 구간 값

			m_ComboScore = static_cast<int>( ceil( static_cast<double>(static_cast<double>(m_ComboNum) / static_cast<double>(MAGIC_NUMBER_SECTION) ) ) ) * m_ComboNum;
			
		}
#else
		if( m_ComboNum >= 4 )
		{
			if( m_ComboNum >= 4 && m_ComboNum < 11 )
			{
				m_ComboScore += MultiplyWeights( 50 );
			}
			else if( m_ComboNum >= 11 && m_ComboNum < 21 )
			{
				m_ComboScore += MultiplyWeights( 100 );
			}
			else if( m_ComboNum >= 21 && m_ComboNum < 31 )
			{
				m_ComboScore += MultiplyWeights( 150 );
			}
			else if( m_ComboNum >= 31 && m_ComboNum < 41 )
			{
				m_ComboScore += MultiplyWeights( 200 );
			}
			else if( m_ComboNum >= 41 && m_ComboNum < 51 )
			{
				m_ComboScore += MultiplyWeights( 250 );
			}
			else if( m_ComboNum >= 51 && m_ComboNum < 61 )
			{
				m_ComboScore += MultiplyWeights( 300 );
			}
			else if( m_ComboNum >= 61 && m_ComboNum < 71 )
			{
				m_ComboScore += MultiplyWeights( 350 );
			}
			else if( m_ComboNum >= 71 && m_ComboNum < 81 )
			{
				m_ComboScore += MultiplyWeights( 400 );
			}
			else if( m_ComboNum >= 81 && m_ComboNum < 91 )
			{
				m_ComboScore += MultiplyWeights( 450 );
			}
			else if( m_ComboNum >= 91 )
			{
				m_ComboScore += MultiplyWeights( 500 );
			}

			if( m_pScoreBonusPicChar != NULL )
			{
				m_pScoreBonusPicChar->Clear();
				m_pScoreBonusPicChar->SetWidth( 11.0f );
				if( m_ComboScore < 1000 )
					m_pScoreBonusPicChar->SetColorNo();
				else if( m_ComboScore < 2000 )
					m_pScoreBonusPicChar->SetColor( 0xffffff00 );
				else if( m_ComboScore < 3000 )
					m_pScoreBonusPicChar->SetColor( 0xffff0000 );
				m_pScoreBonusPicChar->DrawText( m_ComboScore, D3DXVECTOR3(140,275,0), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_LEFT );
			}
		}
#endif DUNGEON_RANK_NEW
		//}} JHKang / 강정훈 / 2011.02.11 / 던전 랭크 개선 콤보 점수 개선

		if( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		{
			if( m_pPicCharComboDamage != NULL )
			{
				m_pPicCharComboDamage->Clear();
				m_pPicCharComboDamage->DrawText( m_iMyComboDamage, D3DXVECTOR3(100.f, 650.f, 0.f), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_LEFT );			
			}

			if( m_pPicCharComboMaxDamage != NULL )
			{
				if( m_iMyMaxComboDamage <= m_iMyComboDamage )
				{
					m_pPicCharComboMaxDamage->Clear();
					m_pPicCharComboMaxDamage->DrawText( m_iMyMaxComboDamage, D3DXVECTOR3(100.f, 600.f, 0.f), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_LEFT );
				}
			}

#ifdef DIALOG_SHOW_TOGGLE
			if( m_pPicCharComboDamage != NULL && m_pPicCharComboMaxDamage != NULL &&
				m_pPicCharComboDamage->GetSeq() != NULL && m_pPicCharComboMaxDamage->GetSeq() != NULL )
			{				
				m_pPicCharComboDamage->GetSeq()->SetShowObject(true);
				m_pPicCharComboMaxDamage->GetSeq()->SetShowObject(true);
			}		
#endif
		}
	}
#ifdef ELSWORD_SHEATH_KNIGHT
	return m_ComboNum;
#endif ELSWORD_SHEATH_KNIGHT
}

void CX2ComboManager::Verify()
{
	if( g_pKTDXApp->GetIsNowVeryfy() == true )
	{
		if( m_ComboNum.Verify()				== false
			|| m_ComboScore.Verify()		== false
			|| m_ComboScoreFull.Verify()	== false 
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		    || m_iUsingSubStageComboScore.Verify() == false
#endif 
			)
		{
			g_pKTDXApp->SetFindHacking( true );
		}
	}
}

void CX2ComboManager::ClearParticleSeqGoodSeries()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	for ( int i = 0; i < ARRAY_SIZE(m_hInterjection); ++i )
	{
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hInterjection[i] );
		if ( NULL != pSeq )
			pSeq->ClearAllParticle();
	}
}

/** @function : MultiplyWeights
	@brief : 각 캐릭터별 콤보 가중치를 계산하는 함수\n
	2010-11-15일 부터는 클라이언트에서 가중치를 적용하지 않음
	@param : 가중치가 적용되지 않는 경우 증가해야하는 점수
	@return : 가중치가 적용된 후의 증가량
*/
FORCEINLINE int CX2ComboManager::MultiplyWeights( int iScore_ )
{
// 	switch( m_pUnit->GetUnit()->GetType() )
// 	{
// 	case CX2Unit::UT_ELSWORD:
// 	case CX2Unit::UT_RAVEN:
// 		{
// 			return iScore_;
// 		}
// 		break;
// 
// 	case CX2Unit::UT_EVE:
// 	case CX2Unit::UT_ARME:
// 		{
// 			const float WEIGHT_EVE_AISHA = 0.7f;
// 			return iScore_ * WEIGHT_EVE_AISHA;
// 		}
// 		break;
// 
// 	case CX2Unit::UT_LIRE:
// 		{
// 			const float WEIGHT_RENA = 1.15f;
// 			return iScore_ * WEIGHT_RENA;
// 		}
// 		break;
// 
// 	default:
// 		return iScore_;
// 		break;
// 	}

	return iScore_;
}
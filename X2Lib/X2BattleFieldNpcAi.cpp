#include "StdAfx.h"
#include ".\X2BattleFieldNpcAi.h"


const float		CX2BattleFieldNpcAi::BASIC_INCREASE_HP_PERCENT_BY_SECOND				= 0.002f;
const float		CX2BattleFieldNpcAi::OUT_OF_PATROL_LINE_INCREASE_HP_PERCENT_BY_SECOND	= 0.005f;
const float		CX2BattleFieldNpcAi::OUT_OF_PLAY_LINE_INCREASE_HP_PERCENT_BY_SECOND		= 0.02f;

CX2BattleFieldNpcAi::CX2BattleFieldNpcAi( CX2GUNPC* pMasterNpc_, const CNpcCreatedInfoPtr NpcCreatedInfoPtr_ )
	: CX2GUNPCAI( pMasterNpc_ ), m_bAggressive( NpcCreatedInfoPtr_->GetAggressive() ), m_bCanTargeting( NpcCreatedInfoPtr_->GetAggressive() ), 
	m_bCanPatrol( true ), m_bBackState01( false ), m_bBackState02( false ), m_bArriveAtDest( false ), 
 	m_vecPatrolLineMapIndex( NpcCreatedInfoPtr_->GetPetrolLineIndexList() ), 
	m_ElapsedTimeCheckOnLineMap( 5.0f ), m_ElapsedTimeAfterSuccessToPatrol( 0.0f ), //m_ElapsedTimeUpNowHp( 1.0f ),
 	m_ElapsedTimeComeBackState( 5.0f ), m_vecPlayLineMapIndex( NpcCreatedInfoPtr_->GetPlayLineIndexList() ), 
	m_fIncreaseHpPercentBySecond( BASIC_INCREASE_HP_PERCENT_BY_SECOND ), m_iCheckLineMapAtFirst( -1 )

#ifdef	BATTLE_FIELD_TEST
	, m_uiGroupId( NpcCreatedInfoPtr_->GetGroupId() )
#endif	BATTLE_FIELD_TEST

{
	SetAIType( NAT_FIELD );
}

CX2BattleFieldNpcAi::~CX2BattleFieldNpcAi()
{
}

/*virtual*/ void CX2BattleFieldNpcAi::OnFrameMove( double fTime, float fElapsedTime )
{
	//*
	if ( true == m_bNoBrain )
		return;


#ifdef NEXON_QA_CHEAT_REQ
	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )
	if( true == m_bCanPatrol )
		m_bCanPatrol = ( m_vecPatrolLineMapIndex.size() == 0) ? false : true;
#endif //NEXON_QA_CHEAT_REQ


	if ( false == m_bEnabled )
		return;

	SetMasterUnitData();

	CheckArrivedDestination();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    UpNowHpBySecondProcess( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	UpNowHpBySecondProcess();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// 백스테이트면 Moving만 수행
	if ( GetBackState01() )
		return Moving( fElapsedTime );
	
	if ( GetBackState02() ) 
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_ElapsedTimeComeBackState.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_ElapsedTimeComeBackState.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		if ( m_ElapsedTimeComeBackState.CheckAndResetElapsedTime() )
		{
			// 위치 이동 후 바로 P2P 패킷 전송이 가능 하도록 Direction을 바꿔준다.
			m_pMasterNPC->SetPosition( GetFinalDest(), !IsRight() );
		}
		
		return Moving( fElapsedTime );
	}
	
	// 패트롤 라인맵과 플레이 라인맵의 체크 시간 갱신
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_ElapsedTimeCheckOnLineMap.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_ElapsedTimeCheckOnLineMap.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// 패트롤 중이 아니면 경과시간 갱신
	if ( false == GetNowPatroling() )
    {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_ElapsedTimeAfterSuccessToPatrol.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_ElapsedTimeAfterSuccessToPatrol.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    }

	if ( m_ElapsedTimeCheckOnLineMap.CheckAndResetElapsedTime() )	// 라인맵 체크시간 검사 및 갱신
	{
		// 패트롤 라인맵 위에 있는지 체크
		if ( false == CheckOnPatrolLineMap() )	// 패트롤 라인맵 위에 없다면
		{
			// 플레이 라인맵 위에 있는지 검사
			if ( false == CheckOnPlayLineMap() )	// 플레이 라인맵 위에도 없다면
			{

#ifdef NEXON_QA_CHEAT_REQ
				CX2User* pUser = g_pData->GetMyUser();
				if( (NULL != pUser && pUser->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && true == m_bCanPatrol) || (pUser->GetAuthLevel() < CX2User::XUAL_SPECIAL_USER) )
#endif //NEXON_QA_CHEAT_REQ

				 // Do BackState 02
				if ( Patroling() )
				{
					if ( -1 == m_iCheckLineMapAtFirst)
						m_iCheckLineMapAtFirst = m_pMasterNPC->GetLastTouchLineIndex();

					SetNowPatroling( false );
					SetBackState02( true );
					m_ElapsedTimeComeBackState.ResetSumOfElapsedTime();
					m_pMasterNPC->SetIsComeBackState( true );
					SetIncreaseHpPercentBySecond( OUT_OF_PLAY_LINE_INCREASE_HP_PERCENT_BY_SECOND );
					ResetTarget();
					SetLostTarget( false );	// 타겟을 잃었는지 여부를 초기화
					return Moving( fElapsedTime );
				}
			}
			else	// 플레이 라인맵 위에는 있지만
			{
				if ( null == m_optrTargetGameUnit && GetLostTarget() && false == GetNowPatroling() )	// 타겟을 잃은 경우
				{

#ifdef NEXON_QA_CHEAT_REQ
					CX2User* pUser = g_pData->GetMyUser();
					if( (NULL != pUser && pUser->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && true == m_bCanPatrol) || (pUser->GetAuthLevel() < CX2User::XUAL_SPECIAL_USER) )
#endif //NEXON_QA_CHEAT_REQ

					// Do BackState 01
					if ( Patroling() )
					{
						SetNowPatroling( false );
						SetBackState01( true );
						SetIncreaseHpPercentBySecond( OUT_OF_PATROL_LINE_INCREASE_HP_PERCENT_BY_SECOND );
						ResetTarget();
						SetLostTarget( false );	// 타겟을 잃었는지 여부를 초기화
						return Moving( fElapsedTime );
					}
				}
			}
		}
	}
	
	// 나를 공격한 유닛이 있는 경우 타겟팅
	SetLostTarget( false );	// 타겟을 잃었는지 여부를 초기화
	if ( TargetingGameUnitWhichAttackMe() )
	{
		return Moving( fElapsedTime );	// 무빙으로..
	}

	// 타겟중인 유닛이 없고, 패트롤 가능한 유닛이고 (이곳을 수행 할 때 타겟 중인 것이 없다면 Patrol을 할 것임)
	if ( null == m_optrTargetGameUnit && GetCanPatrol() )
	{

#ifdef NEXON_QA_CHEAT_REQ
		CX2User* pUser = g_pData->GetMyUser();
		if( (NULL != pUser && pUser->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && true == m_bCanPatrol) || (pUser->GetAuthLevel() < CX2User::XUAL_SPECIAL_USER) )
#endif //NEXON_QA_CHEAT_REQ


		// 이전 패트롤을 끝내지 않았으면 다시 패트롤 목적지를 갱신하지 않는다.
		if ( false == GetNowPatroling() && 
            m_ElapsedTimeAfterSuccessToPatrol.CheckElapsedTime() && 
			 Patroling() &&
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			 CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->patrolMoveData.patrolBeginRate ) == true
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
             RandomInt() < GetAIData()->patrolMoveData.patrolBeginRate 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )
		{
			m_ElapsedTimeAfterSuccessToPatrol.ResetSumOfElapsedTime();
			return Moving( fElapsedTime );
		}
	}

	// 루아 Function에 의해 타겟팅을 하는 것이면
	if( true == GetEnableLuaTargetingFunc() && 
		false == GetLuaTargetingFunc().empty() )
	{
		m_fElapsedTimeAfterLastTargeting += fElapsedTime;
		if( m_fElapsedTimeAfterLastTargeting > GetAIData()->targetData.targetInterval )
		{
			m_fElapsedTimeAfterLastTargeting = 0.f;
			//string func = "";
			//ConvertWCHARToChar( func, GetLuaTargetingFunc().c_str() );
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			lua_tinker::call<void>( m_pMasterNPC->GetFunctionLuaState(),  GetLuaTargetingFunc().c_str(), g_pKTDXApp, g_pX2Game, m_pMasterNPC );
//#else	X2OPTIMIZE_NPC_LUASPACE_SHARING
//			lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  func.c_str(), g_pKTDXApp, g_pX2Game, m_pMasterNPC );
//#endif	X2OPTIMIZE_NPC_LUASPACE_SHARING
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	else
	{
		Targeting( fElapsedTime );
	}
	Moving( fElapsedTime );
	//*/
}


/*virtual*/ void CX2BattleFieldNpcAi::Targeting( float fElapsedTime )
{
	// 타겟팅을 할 수 없는 경우에는 리턴 (비선공인데 아직 공격 받은적이 없다거나 등등)
	if ( false == GetCanTargeting()  )
		return;

	ASSERT( NULL != g_pX2Game->GetWorld()->GetLineMap() );

	// 현재 타겟중인 대상이 있는 경우 타겟을 잃어버리진 않는지 검사 및 잃어버리는 조건에 걸린 경우(true 리턴) 타겟을 Reset 함
	if ( CheckAndResetTargetLost() )
	{
		ResetTarget();
		SetLostTarget( true );
		SetFinalDestinationPosAndLineMapIndex( m_pMasterNPC->GetPos(), m_pMasterNPC->GetLastTouchLineIndex() );
		SetCanTargeting( IsAggressive() );	// 타겟을 잃어버린 경우 비선공 몬스터는 Targeting을 중지
		m_fElapsedTimeAfterLastTargeting = GetAIData()->targetData.targetInterval;
	}

	m_fElapsedTimeAfterLastTargeting += fElapsedTime;
	// TargetingInterval을 체크한다.
	if ( m_fElapsedTimeAfterLastTargeting < GetAIData()->targetData.targetInterval )
		return;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fElapsedTimeAfterLastTargeting = 0.0f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// 현재 타겟이 있는 경우 타겟을 유지 할 것인지 처리
	if ( CanPreserveLastTargetUnit() )
		return;		// 무빙

	/////////////////////////// 여기서 부터는 새로운 유닛을 타겟팅 ////////////////////////////////
	
	// 제일 가까이에 있는 유닛들을 vector로 뽑아냄
	GetTargetListInNearRangeOrRange();

	// 무조건 타겟 할 수 있는 거리의 유닛이 없거나(m_vecTargetGameUnitListInNearRange), 
	// GetAIData()->targetData.targetSuccessRate 확률조건에 성공하면 일반적인 Range 내의 유닛들을 타겟 대상으로 삼는다
	if ( !m_vecTargetGameUnitListInRange.empty() )
	{
		if ( m_vecTargetGameUnitListInNearRange.empty() 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            || CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->targetData.targetSuccessRate ) == true 
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            || RandomInt() < GetAIData()->targetData.targetSuccessRate 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )
			SearchingTargetUnit( m_vecTargetGameUnitListInRange );
		else	// 아니면 무조건 Target이 가능한 유닛들을 대상으로 타겟 유닛을 찾는다.
			SearchingTargetUnit( m_vecTargetGameUnitListInNearRange );

		m_vecTargetGameUnitListInRange.resize( 0 );
		m_vecTargetGameUnitListInNearRange.resize( 0 );
	}
	else
		m_fElapsedTimeAfterLastTargeting = 0.0f;

	// 타겟 여기 까지 왔는데도 타겟이 없으면... 패트롤을 해야하는데..
}

/*virtual*/ void CX2BattleFieldNpcAi::LoadAIDataFromLUA( KLuaManager& luaManager )
{
	CX2GUNPCAI::LoadAIDataFromLUA( luaManager );

	// 패트롤 쿨타임 설정
	m_ElapsedTimeAfterSuccessToPatrol.SetTargetTime( GetAIData()->patrolMoveData.patrolCoolingTime );
}

bool CX2BattleFieldNpcAi::Patroling()
{
	const UINT uiSizeofPatrolLineMap = m_vecPatrolLineMapIndex.size();
	ASSERT( 0 != uiSizeofPatrolLineMap );
	if ( 0 == uiSizeofPatrolLineMap )
		return false;

	const int iDestinationLineMapIndexForPatrol = m_vecPatrolLineMapIndex[RandomInt() % uiSizeofPatrolLineMap];

	// 현재 자신이 서 있는 위치와 같으면
	if ( GetLastTouchLineIndex() == iDestinationLineMapIndexForPatrol )
		return false;

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	ASSERT( NULL != pLineMap );

	const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iDestinationLineMapIndexForPatrol );
	ASSERT( NULL != pLineData );
	if ( NULL == pLineData )
		return false;
	
	const float fDistance3SqFromLineStartPos = GetDistance3Sq( pLineData->startPos, GetNowPosition() );
	const float fDistance3SqFromLineEndPos = GetDistance3Sq( pLineData->endPos, GetNowPosition() );
	const float fMoveGap3Sq = pow( static_cast<float>( GetAIData()->chaseMoveData.moveGap ), 2 );
	
	if ( fDistance3SqFromLineStartPos < fMoveGap3Sq && fDistance3SqFromLineEndPos > fMoveGap3Sq )
		SetFinalDestinationPosAndLineMapIndex( pLineData->endPos,	iDestinationLineMapIndexForPatrol );
	else if ( fDistance3SqFromLineEndPos < fMoveGap3Sq && fDistance3SqFromLineStartPos > fMoveGap3Sq )
		SetFinalDestinationPosAndLineMapIndex( pLineData->startPos,	iDestinationLineMapIndexForPatrol );
	else
		SetFinalDestinationPosAndLineMapIndex( D3DXVECTOR3( pLineData->startPos + (pLineData->endPos - pLineData->startPos) * RandomFloat( 0.1f, 0.9f ) ),
		iDestinationLineMapIndexForPatrol );

	SetNowPatroling( true );

	return true;
}

bool CX2BattleFieldNpcAi::TargetingGameUnitWhichAttackMe()
{
	// 자신을 공격한 유닛이 있는지 체크
	if ( null != m_optrAttackerGameUnit )	// 자신을 공격한 유닛이 있으면
	{
		if ( null == m_optrTargetGameUnit	// 기존에 타겟 중인 유닛이 없거나
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            || m_pMasterNPC->EstimateFrameAccumPercent( GetAIData()->targetData.attackTargetRate ) == true
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			|| RandomInt() < GetAIData()->targetData.attackTargetRate 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )	// 공격한 유닛을 타겟할 확률 체크에 성공하면
		{
			m_optrTargetGameUnit = m_optrAttackerGameUnit;	// 타겟팅 함

			ResetAttackerGameUnit();
			
			if ( false == IsAggressive() )	// 비선공 몬스터의 경우 공격을 당햇으면
				SetCanTargeting( true );	// 그 이후 부터는 타겟팅이 가능 하도록 함

			return true;
		}
	}

	return false;	
}

bool CX2BattleFieldNpcAi::CanPreserveLastTargetUnit()
{
	// 타겟중인 유닛은 있는데 이전 타겟을 유지하는 것이 확률에 의해 실패 하면
	if ( null == m_optrTargetGameUnit )
		return false;	// 타겟이 없는 경우

	// 현재의 타겟을 유지할 확률
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->targetData.preserveLastTargetRate ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( RandomInt() < GetAIData()->targetData.preserveLastTargetRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		return true;	// 기존의 타겟을 유지 하므로 다시 타겟팅 할 필요 없음
		
	}

	// 다시 타겟팅을 함, 그러나 ResetTarget은 하지 않음 (다시 타겟팅 대상을 찾는 것일 뿐)
	return false;
}

void CX2BattleFieldNpcAi::GetTargetListInNearRangeOrRange()
{
	const UINT uiListSize = g_pX2Game->GetUserUnitListSize();
	const D3DXVECTOR3& vMyPos = m_pMasterNPC->GetPos();

	for ( UINT i = 0; i < uiListSize; ++i )
	{
		CX2GameUnit* pGameUnit = g_pX2Game->GetUserUnit( i );

		if ( NULL == pGameUnit )
			continue;

		// 유닛이 죽었으면 패스
		if ( CX2GameUnit::GUSI_DIE == pGameUnit->GetGameUnitState() || 0.0f >= pGameUnit->GetNowHp() )
			continue;

		const TargetData& targetData = GetAIData()->targetData;
		const D3DXVECTOR3& vOtherGameUnitPos = pGameUnit->GetPos();
		// 큰것에서 작은것을 빼서 높이 차를 구함
		const float fDistanceHeight = ( vMyPos.y > vOtherGameUnitPos.y ? vMyPos.y - vOtherGameUnitPos.y : vOtherGameUnitPos.y - vMyPos.y );
		if ( fDistanceHeight > targetData.targetHeightLimit )
			continue;

		// 나와 대상 유닛과의 거리의 제곱을 구함
		const float fDistance3Sq = GetDistance3Sq( vMyPos, vOtherGameUnitPos );
		
		// 타겟 가능 거리보다 멀면 패스
		if ( fDistance3Sq > targetData.targetRange * targetData.targetRange )
			continue;

		// 타겟이 가능한 거리에 있는데 유닛이 클로킹 중이고, 내가 스캔을 할 수 없으면
		if ( pGameUnit->GetInvisibility() && false == GetCanScanCloaking( fDistance3Sq
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            , false
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            ) )
			continue;

		// 타겟이 가능 하다면
		m_vecTargetGameUnitListInRange.push_back( pGameUnit );

		// 무조건 타겟이 가능한 거리보다도 가깝다면
		if ( fDistance3Sq < targetData.targetNearRange * targetData.targetNearRange )
			m_vecTargetGameUnitListInNearRange.push_back( pGameUnit );		
	}

}

bool CX2BattleFieldNpcAi::CheckAndResetTargetLost()
{
	// 타겟중인 유닛이 없는 경우에는 false를 리턴
	if ( null == m_optrTargetGameUnit )
		return false;

	// 그외의 경우에는 retrun 시에는 Targeting을 시도 할 수 있도록, m_TargetingInterval을 세팅함

	// 타겟이 죽은 상태면
	if ( CX2GameUnit::GUSI_DIE == m_optrTargetGameUnit->GetGameUnitState() &&
		 0.0f >= m_optrTargetGameUnit->GetNowHp() )
		return true;

	if ( m_vecPlayLineMapIndex.empty() )
		return false;

	// 타겟 중인 유닛이 타겟을 잃어버리는 범위 밖으로 나갔는지 체크
	const float fDistance3Sq = GetDistance3Sq( m_pMasterNPC->GetPos(), m_optrTargetGameUnit->GetPos() );
	
	const int& iTargetLostRange = GetAIData()->targetData.targetLostRange;
	if ( fDistance3Sq > iTargetLostRange * iTargetLostRange )
		return true;

	// 타겟중인 유닛이 클로킹 인데 내가 스캔 할 수 없으면
	if ( m_optrTargetGameUnit->GetInvisibility() && 
		 GetCanScanCloaking( fDistance3Sq
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
         , true
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
         ) )
		return true;

	return false;
}

void CX2BattleFieldNpcAi::SearchingTargetUnit( IN vector<CX2GameUnit*>& vecTargetCandidateUnitList_ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( vecTargetCandidateUnitList_.empty() == true )
        return;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	switch ( GetAIData()->targetData.targetPriority )
	{
	case TP_HIGH_LEVEL_FIRST: 
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// 기존의 Targeting 중인 유닛이 없거나, 기존의 유닛 보다 새로운 유닛의 Level이 높으면
				if ( null == m_optrTargetGameUnit ||
					 IsHigherNewTargetGameUnitLevel( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_LOW_LEVEL_FIRST:
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// 기존의 Targeting 중인 유닛이 없거나, 기존의 유닛 보다 새로운 유닛의 Level이 낮으면
				if ( null == m_optrTargetGameUnit ||
					 false == IsHigherNewTargetGameUnitLevel( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_HIGH_HP_FIRST:
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// 기존의 Targeting 중인 유닛이 없거나, 기존의 유닛 보다 새로운 유닛의 HP가 높으면
				if ( null == m_optrTargetGameUnit ||
					 IsHigherNewTargetGameUnitHp( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_LOW_HP_FIRST:
		{
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// 기존의 Targeting 중인 유닛이 없거나, 기존의 유닛 보다 새로운 유닛의 HP가 낮으면
				if ( null == m_optrTargetGameUnit ||
					false == IsHigherNewTargetGameUnitHp( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_ ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
			}
		} break;

	case TP_NEAR_FIRST:
		{
			float fDistanceSqFromTargetUnit = 0.0f;	// 현재 타겟팅 된 유닛과의 거리
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// 기존의 Targeting 중인 유닛이 없거나, 새로운 타겟이 더 가까이 있으면
				if( null == m_optrTargetGameUnit ||
					IsNearNewTargetGameUnit( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_, fDistanceSqFromTargetUnit ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;				
			}
		} break;

	case TP_FAR_FIRST:
		{
			float fDistanceSqFromTargetUnit = 0.0f;	// 현재 타겟팅 된 유닛과의 거리
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				// 기존의 Targeting 중인 유닛이 없거나, 새로운 타겟이 더 멀리 있으면
				if( null == m_optrTargetGameUnit ||
					IsFarNewTargetGameUnit( m_optrTargetGameUnit.GetObservable(), pNewTargetCandidateGameUnit_, fDistanceSqFromTargetUnit ) )
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;				
			}
		} break;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    default:
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	case TP_RANDOM:
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            float fRate = 0.f;
            BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
            {
                fRate = fRate * 0.5f + 0.5f;
            }
            if ( CX2GUNPC::EstimateFrameOneshotPercent( fRate * 100.f ) == true )
            {
                if ( vecTargetCandidateUnitList_.size() == 1 )
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_.front();
                else
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_[ rand() % vecTargetCandidateUnitList_.size() ];
            }
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				if ( RandomInt() < 50 )
				{
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
					break;
				}
			}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;

	case TP_ONLY_ONE:
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if ( m_optrTargetGameUnit == null )
            {
                if ( vecTargetCandidateUnitList_.size() == 1 )
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_.front();
                else
                    m_optrTargetGameUnit = vecTargetCandidateUnitList_[ rand() % vecTargetCandidateUnitList_.size() ];
            }
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			BOOST_FOREACH( CX2GameUnit* pNewTargetCandidateGameUnit_, vecTargetCandidateUnitList_ )
			{
				if ( null == m_optrTargetGameUnit )
				{
					m_optrTargetGameUnit = pNewTargetCandidateGameUnit_;
					break;
				}
			}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		} break;
	}
}

bool CX2BattleFieldNpcAi::IsHigherNewTargetGameUnitLevel( const CX2GameUnit* pOldTargetGameUnit_, const CX2GameUnit* pNewTargetGameUnit_ )
{
	if( pOldTargetGameUnit_->GetUnitLevel() < pNewTargetGameUnit_->GetUnitLevel() )
		return true;
	else
		return false;
}

bool CX2BattleFieldNpcAi::IsHigherNewTargetGameUnitHp( const CX2GameUnit* pOldTargetGameUnit_, const CX2GameUnit* pNewTargetGameUnit_ )
{
	if( pOldTargetGameUnit_->GetNowHp() < pNewTargetGameUnit_->GetNowHp() )
		return true;
	else
		return false;
}

bool CX2BattleFieldNpcAi::IsNearNewTargetGameUnit( IN const CX2GameUnit* pOldTargetGameUnit_, IN const CX2GameUnit* pNewGameUnit_, IN OUT float& fDistanceSqFromOldTargetUnit_ )
{
	// 새로운 타겟 대상과의 거리를 구한 후
	const float fDistanceSqFromNewTargetGameUnit = GetDistance3Sq( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );
	const float fDistance = GetDistance( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );

	// 기존의 타겟이 없거나, 새로운 타겟 후보가 더 가까우면
	if( fDistanceSqFromNewTargetGameUnit < fDistanceSqFromOldTargetUnit_ )
	{
		fDistanceSqFromOldTargetUnit_ = fDistanceSqFromNewTargetGameUnit;
		return true;
	}
	else
		return false;
}

bool CX2BattleFieldNpcAi::IsFarNewTargetGameUnit( IN const CX2GameUnit* pOldTargetGameUnit_, IN const CX2GameUnit* pNewGameUnit_, IN OUT float& fDistanceSqFromOldTargetUnit_ )
{
	// 새로운 타겟 대상과의 거리를 구한 후
	const float fDistanceSqFromNewTargetGameUnit = GetDistance3Sq( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );
	const float fDistance = GetDistance( m_pMasterNPC->GetPos(), pNewGameUnit_->GetPos() );

	// 기존의 타겟이 없거나, 새로운 타겟 후보가 더 멀면
	if( fDistanceSqFromNewTargetGameUnit > fDistanceSqFromOldTargetUnit_ )
	{
		fDistanceSqFromOldTargetUnit_ = fDistanceSqFromNewTargetGameUnit;
		return true;
	}
	else
		return false;
}

bool CX2BattleFieldNpcAi::CheckOnPatrolLineMap()
{
	const int iLastTouchedLineMapIndex = m_pMasterNPC->GetLastTouchLineIndex();

	BOOST_FOREACH( const int& iPatrolLineMapIndex, m_vecPatrolLineMapIndex )
	{
		// Patrol LineMap 위에 있으면
		if ( iLastTouchedLineMapIndex == iPatrolLineMapIndex )
			return true;
	}

	// Patrol LineMap 위에 없으면
	return false;
}

bool CX2BattleFieldNpcAi::CheckOnPlayLineMap()
{
	const int iLastTouchedLineMapIndex = m_pMasterNPC->GetLastTouchLineIndex();

	BOOST_FOREACH( const int& iPlayLineMapIndex, m_vecPlayLineMapIndex )
	{
		// Play LineMap 위에 있으면
		if ( iLastTouchedLineMapIndex == iPlayLineMapIndex )
			return true;
	}

	// Play LineMap 위에 없으면
	return false;
}

/*virtual*/ void CX2BattleFieldNpcAi::Moving( float fElapsedTime )
{
	CX2GUNPC::STATE_CHANGE_TYPE eSctMessage = CX2GUNPC::SCT_NO_CHANGE;

	if ( null != m_optrTargetGameUnit )
		SetFinalDestinationPosAndLineMapIndex( m_optrTargetGameUnit->GetPos(), m_optrTargetGameUnit->GetLastTouchLineIndex() );

	bool bAiIsRight = false;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	    GetAIMessageAndUnitDirection( fElapsedTime, OUT eSctMessage, OUT bAiIsRight );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		GetAIMessageAndUnitDirection( OUT eSctMessage, OUT bAiIsRight );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// 이전 AI 메시지와 지금 AI 메시지와 다른게 없다면 SCT_NO_CHANGE
	if( m_PrevAIMessage == eSctMessage || CX2GUNPC::SCT_NO_CHANGE == eSctMessage )
	{
		eSctMessage = CX2GUNPC::SCT_NO_CHANGE;
	}
	else	// 이전 AI와 다르다면 (방향전환의 경우 SCT_AI_DIR_CHANGE 가 수행 된다.)
	{
		m_PrevAIMessage = eSctMessage;
		m_pMasterNPC->PushAIMessage( eSctMessage, bAiIsRight );
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::GetAIMessageAndUnitDirection( float fElapsedTime, OUT CX2GUNPC::STATE_CHANGE_TYPE& eSctMessage_, OUT bool& bAilsRight_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::GetAIMessageAndUnitDirection( OUT CX2GUNPC::STATE_CHANGE_TYPE& eSctMessage_, OUT bool& bAilsRight_ )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	ASSERT( NULL != pLineMap );

	// 일단 필드 몬스터는 LineGroup 무시
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	const float& fElapsedTime = g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

// 	m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
// 	if ( m_fElapsedTimeAfterLastRandomJump > GetAIData()->chaseMoveData.jumpInterval )
// 	{
// 		m_fElapsedTimeAfterLastRandomJump = 0.0f;
// 		const float fJumpChance = RandomFloat( 0.0f, 100.0f );
// 
// 		if ( GetFinalDest().y > GetNowPosition().y )
// 		{
// 			if ( fJumpChance < GetAIData()->chaseMoveData.upJumpRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_JUMP;
// 			else if ( 100 - fJumpChance < GetAIData()->chaseMoveData.upDownRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_DOWN;
// 		}
// 		else
// 		{
// 			if( fJumpChance < GetAIData()->chaseMoveData.downJumpRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_JUMP;
// 			else if( 100 - fJumpChance < GetAIData()->chaseMoveData.downDownRate )
// 				eSctMessage_ = CX2GUNPC::SCT_AI_DOWN;
// 		}
// 
// 		return;
// 	}

	bool bIsTargetOnRight = false;

	// 현재위치(m_vPosition)와 타겟이 오른쪽에 있는지(bIsTargetOnRight)를 알아옴
// 	m_PathFinder.FollowTargetPath( GetNowPosition(), GetLastTouchLineIndex(), GetFinalDest(), GetFinalDestLineIndex(),
// 		GetMaxJumpUp(), GetMaxJumpRight(), pLineMap, static_cast<float>( GetAIData()->chaseMoveData.destGap ), 
// 		static_cast<float>( GetAIData()->chaseMoveData.moveGap ), GetFootOnLine(), false, OUT bIsTargetOnRight, 
// 		GetAIData()->chaseMoveData.m_fLineEndDetectRange );
	m_PathFinder.FollowTargetPathInBattleField( this, pLineMap, bIsTargetOnRight );

	if( IsRight() != bIsTargetOnRight )
		m_bIsTargetFront = false;
	else
		m_bIsTargetFront = true;
	
	bAilsRight_ = IsRight();
	switch ( m_PathFinder.GetMoveState() )
	{
	case CX2SimplePathFinder::MS_WAIT:	
		{
			eSctMessage_ = CX2GUNPC::SCT_AI_WAIT;
		} break;

		/// 왼쪽으로 가라고 할때
	case CX2SimplePathFinder::MS_FORCE_LEFT:	
	case CX2SimplePathFinder::MS_LEFT:
		{
			bAilsRight_ = false;

			if( true == IsRight() )	// 현재 오른쪽을 보고 있고
			{
				// 이전 AI 메시지가 wait, walk, dash 중 하나였다면
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{	
					// CHASE_MOVE의 DIR_CHANGE_INTERVAL 체크
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > GetAIData()->chaseMoveData.dirChangeInterval ) 
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						eSctMessage_ = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			// 왼쪽을 보고 있었다거나, 오른쪽을 보고 있었더라도 wait, walk, dash가 아니었다면 walk 하라는 AI를 수행
			eSctMessage_ = CX2GUNPC::SCT_AI_WALK;
		} break;

	case CX2SimplePathFinder::MS_FORCE_RIGHT:
	case CX2SimplePathFinder::MS_RIGHT: 
		{
			bAilsRight_ = true;

			if( false == IsRight() )
			{
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > GetAIData()->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						eSctMessage_ = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			eSctMessage_ = CX2GUNPC::SCT_AI_WALK;			
		} break;

	case CX2SimplePathFinder::MS_JUMP:
		{
			eSctMessage_ = CX2GUNPC::SCT_AI_JUMP;
		} break;

	case CX2SimplePathFinder::MS_DOWN:
		{
			eSctMessage_	= CX2GUNPC::SCT_AI_DOWN;
		} break;

	case CX2SimplePathFinder::MS_JUMP_LEFT:
		{
			bAilsRight_ = false;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_JUMP_RIGHT:
		{
			bAilsRight_ = true;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_LEFT:
		{
			bAilsRight_ = false;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_RIGHT:
		{
			bAilsRight_ = true;
			eSctMessage_	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	default:
		ASSERT( !L"Assert 처리를 해야하나..?" );
		break;
	}	// switch

	// walk, dash 중에 walk, dash로의 상태변화는 CHASE_MOVE의 WALK_INTERVAL 간격으로 갱신
	if( ( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage || CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
		CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage ) &&
		( CX2GUNPC::SCT_AI_WALK == eSctMessage_ || CX2GUNPC::SCT_AI_DASH == eSctMessage_ ) )
	{
		m_fElapsedTimeAfterLastWalkRunUpdate += fElapsedTime;

		/// walkInterval 만큼 경과 됐으면
		if( m_fElapsedTimeAfterLastWalkRunUpdate > GetAIData()->chaseMoveData.walkInterval )
		{	
			m_fElapsedTimeAfterLastWalkRunUpdate = 0.f;

			// 현재위치와 목적한 위치 까지의 거리가 MOVE_SPLIT_RANGE 보다 작으면
			const float fFinalDestDistance3Sq = GetDistance3Sq( GetNowPosition(), GetFinalDest() );
			const float fDistance = GetDistance( GetNowPosition(), GetFinalDest() );

			if( fFinalDestDistance3Sq < GetAIData()->chaseMoveData.moveSplitRange * GetAIData()->chaseMoveData.moveSplitRange )
			{
				// NEAR_WALK_RATE 확률에 따라서
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->chaseMoveData.nearWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < GetAIData()->chaseMoveData.nearWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_WALK;	// 걷던지..
				}
				else
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_DASH;	// 뛰던지..
				}
			}
			else	// MOVE_SPLIT_RANGE 보다 작지 않으면
			{
				// FAR_WALK_RATE 확률에 따라서
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( CX2GUNPC::EstimateFrameOneshotPercent( GetAIData()->chaseMoveData.farWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < GetAIData()->chaseMoveData.farWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_WALK;	// 걷던지..
				}
				else
				{
					eSctMessage_ = CX2GUNPC::SCT_AI_DASH;	// 뛰던지..
				}
			}		
		}
		else
		{
			eSctMessage_ = m_PrevAIMessage;	// 이전의 AI를 그대로 사용
		}
	}
}

void CX2BattleFieldNpcAi::CheckArrivedDestination()
{
	const float fDistance3Sq = GetDistance3Sq( GetNowPosition(), GetFinalDest() );
	
	const int iDestinationGap = GetAIData()->chaseMoveData.destGap;

	if ( fDistance3Sq < iDestinationGap * iDestinationGap )
	{
		// 목적지에 도착
		if ( true == GetBackState01() )
		{
			SetBackState01( false );
			SetIncreaseHpPercentBySecond( BASIC_INCREASE_HP_PERCENT_BY_SECOND );
		}

		if ( true == GetBackState02() )
		{
			SetBackState02( false );
			m_pMasterNPC->SetIsComeBackState( false );
			SetIncreaseHpPercentBySecond( BASIC_INCREASE_HP_PERCENT_BY_SECOND );
// 			SetIncreaseMoveSpeedPercentAtComeBackState( 0.0f );
// 			SetIncreaseAnimSpeedPercentAtComeBackState( 0.0f );
		}

		if ( GetNowPatroling() )
			SetNowPatroling( false );
	}
}

/*virtual*/ void CX2BattleFieldNpcAi::RenderStateParam( WCHAR** wDest_ )
{
// 	int iTarget = 0;
// 	if ( null != m_optrTargetGameUnit )
// 	{
// 		StateLog( L"Target in RenderStateParam" );
// 		iTarget = 1;
// 	}
// 
// 	int iAttacker = 0;
// 	if ( null != m_optrAttackerGameUnit )
// 	{
// 		StateLog( L"Attacker in RenderStateParam" );
// 		iAttacker = 1;
// 	}
// 
// 	int iBackState01 = 0;
// 	if ( true == GetBackState01() )
// 		iBackState01 = 1;
// 
// 	int iBackState02 = 0;
// 	if ( true == GetBackState02() )
// 		iBackState02 = 1;

 	swprintf( *wDest_, L"DestLineIndex: %d",// Target: %d, Attacker: %d, BackState01: %d, BackState02: %d, CheckNum: %d",
 		GetFinalDestLineIndex() );//, iTarget, iAttacker, iBackState01, iBackState02,	m_iCheckLineMapAtFirst );

//	swprintf( *wDest_, L"GroupID: %d", m_uiGroupId );
}

/*virtual*/ void CX2BattleFieldNpcAi::RenderPatrolLineIndex( WCHAR** wDest_ )
{
	swprintf( *wDest_, L"Patrol: " );
	BOOST_FOREACH( int index, m_vecPatrolLineMapIndex)
	{
		WCHAR wSource[10] = { 0, };
		swprintf( wSource, L"%d,", index );
		StrCatW( *wDest_, wSource );
	}
}

/*virtual*/ void CX2BattleFieldNpcAi::RenderPlayLineIndex( WCHAR** wDest_ )
{
	swprintf( *wDest_, L"Play: " );
	BOOST_FOREACH( int index, m_vecPlayLineMapIndex )
	{
		WCHAR wSource[10] = { 0, };
		swprintf( wSource, L"%d,", index );
		StrCatW( *wDest_, wSource );
	}
}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::UpNowHpBySecondProcess( float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2BattleFieldNpcAi::UpNowHpBySecondProcess()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	//m_ElapsedTimeUpNowHp.OnFrameMove();
	//if ( m_ElapsedTimeUpNowHp.CheckAndResetElapsedTime() )
#ifdef FIELD_BOSS_RAID // 보스 NPC는 체력 회복 제거
	if( false == m_pMasterNPC->GetIsBosRaidNPC() )
#endif // FIELD_BOSS_RAID
	{
		const float fMaxHp = m_pMasterNPC->GetMaxHp();
		const float fNowHp = m_pMasterNPC->GetNowHp();
		if ( fNowHp < fMaxHp && 0.0f < fNowHp )
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_pMasterNPC->UpNowHp( fMaxHp * GetIncreaseHpPercentBySecond() * fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_pMasterNPC->UpNowHp( fMaxHp * GetIncreaseHpPercentBySecond() * g_pKTDXApp->GetElapsedTime() );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
	}
}

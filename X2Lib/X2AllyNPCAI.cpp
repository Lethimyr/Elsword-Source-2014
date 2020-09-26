#include "StdAfx.h"
#include ".\x2allynpcai.h"

CX2AllyNPCAI::CX2AllyNPCAI( CX2GUNPC* pMasterNPC ) :
CX2NPCAI( pMasterNPC, CX2NPCAI::NAT_ALLY ), 
m_AllyUnitUID( -1 ), 
m_eAllyUnitType( CX2GameUnit::GUT_USER ),
m_iFinalDestLineIndex( -1 ),
m_vFinalDest( 0, 0, 0 )
, m_fMaxJumpRight( 0.f )
, m_fMaxJumpUp( 0.f )
, m_ChaseMoveData( ChaseMoveData() )
, m_RandomJumpData( RandomJumpData() )
, m_TargetData( TargetData() )
#ifdef EVOKE_TARGETING_BUG_FIX		/// 메뉴얼 타겟팅 추가
,m_bEnableLuaTargetingFunc(false)
,m_strLuaTargetingFunc("")
#endif EVOKE_TARGETING_BUG_FIX
{
}

CX2AllyNPCAI::~CX2AllyNPCAI(void)
{
}

void CX2AllyNPCAI::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( true == m_bNoBrain )
		return; 

	if( false == m_bEnabled )
		return;

	m_fElapsedTimeAfterLastTargeting	+= fElapsedTime;
	
	if( false == RallyToAlly() )
	{
#ifdef EVOKE_TARGETING_BUG_FIX		/// 메뉴얼 타겟팅 대상이 없으면, 스크립트내 메뉴얼 타겟 함수에서 타겟 찾아와서 업데이트
		if( true == m_bEnableLuaTargetingFunc && 
			false == m_strLuaTargetingFunc.empty() )
		{
			m_fElapsedTimeAfterLastTargeting += fElapsedTime;
			if( m_fElapsedTimeAfterLastTargeting > m_TargetData.targetInterval )
			{
				m_fElapsedTimeAfterLastTargeting = 0.f;
				//string func = "";
				//ConvertWCHARToChar( func, m_strLuaTargetingFunc.c_str() );

				lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  m_strLuaTargetingFunc.c_str(), g_pKTDXApp, g_pX2Game, m_pMasterNPC );
			}
			TargetUpdate();
		}
		else
#endif EVOKE_TARGETING_BUG_FIX
			Targeting( fElapsedTime );
	}
	
	Moving( fElapsedTime );
}



void CX2AllyNPCAI::LoadAIDataFromLUA( KLuaManager& luaManager )
{
	CX2NPCAI::LoadAIDataFromLUA( luaManager );
	if( true == m_bNoBrain )
		return;


	if( luaManager.BeginTable( "INIT_AI" ) == true )
	{
		if( luaManager.BeginTable( "ALLY" ) == true )
		{
			LUA_GET_VALUE( luaManager, "FAR_LOST_RANGE",		m_AllyData.allyFarLostRange,	2000.f );
			LUA_GET_VALUE( luaManager, "LOST_RANGE",			m_AllyData.allyLostRange,		1000.f );

			luaManager.EndTable();
		}


		if( luaManager.BeginTable( "TARGET" ) == true )
		{
#ifdef EVOKE_TARGETING_BUG_FIX		/// 메뉴얼 타겟팅 함수 파싱
			LUA_GET_VALUE_UTF8( luaManager, "MANUAL_TARGETING_FUNC",	m_TargetData.strLuaTargetingFunc,		"" );

			if( false == m_TargetData.strLuaTargetingFunc.empty() )
			{
				SetLuaTargetingFunc( m_TargetData.strLuaTargetingFunc );
				SetEnableLuaTargetingFunc( true );
			}
#endif EVOKE_TARGETING_BUG_FIX

			LUA_GET_VALUE_ENUM( luaManager, "TARGET_PRIORITY",		m_TargetData.targetPriority,		TARGET_PRIORITY, TP_RANDOM );

			LUA_GET_VALUE( luaManager, "TARGET_INTERVAL",			m_TargetData.targetInterval,		0 );
			LUA_GET_VALUE( luaManager, "TARGET_NEAR_RANGE",			m_TargetData.targetNearRange,		0 );
			m_TargetData.targetNearRange = min( m_TargetData.targetNearRange, 8000.f);

			LUA_GET_VALUE( luaManager, "TARGET_RANGE",				m_TargetData.targetRange,			0 );
			m_TargetData.targetRange	 = min( m_TargetData.targetRange, 9000.f);

			LUA_GET_VALUE( luaManager, "TARGET_LOST_RANGE",			m_TargetData.targetLostRange,		0 );
			m_TargetData.targetLostRange = min( m_TargetData.targetLostRange, 10000.f);

			LUA_GET_VALUE( luaManager, "TARGET_SUCCESS_RATE",		m_TargetData.targetSuccessRate,		0 );
			LUA_GET_VALUE( luaManager, "ATTACK_TARGET_RATE",		m_TargetData.targetAttakerRate,		0 );
			LUA_GET_VALUE( luaManager, "PRESERVE_LAST_TARGET_RATE", m_TargetData.targetPreserveRate,	0 );

#ifdef ADD_NPC_CONDITION_TABLE
			LUA_GET_VALUE( luaManager, "TARGET_ONLY_OUR_TEAM",		m_TargetData.bTargetOnlyOurTeam,	false );
#endif // ADD_NPC_CONDITION_TABLE


			luaManager.EndTable(); // TARGET
		}
		


		if( luaManager.BeginTable( "CHASE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "DIR_CHANGE_INTERVAL",	m_ChaseMoveData.dirChangeInterval,		0 );

			LUA_GET_VALUE( luaManager, "DEST_GAP",				m_ChaseMoveData.destGap,				0 );
			LUA_GET_VALUE( luaManager, "MOVE_GAP",				m_ChaseMoveData.moveGap,				0 );

			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",		m_ChaseMoveData.dashRange,				0 );			// 추가된 것
			LUA_GET_VALUE( luaManager, "REVERSE_WALK_DASH",		m_ChaseMoveData.bReverseWalkDash,		false );		// 추가된 것
			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",			m_ChaseMoveData.walkDashInterval,		0 );

			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_ChaseMoveData.bStayOnCurrentLineGroup,	false );

#ifdef ADD_NPC_CONDITION_TABLE
			LUA_GET_VALUE( luaManager, "IF_CANNOT_FIND_MOVE_STATE_DO_WAIT",		m_ChaseMoveData.bIfCannotFindMoveStateDoWait, false );
#endif // ADD_NPC_CONDITION_TABLE

			// random jump
			LUA_GET_VALUE( luaManager, "RANDOM_JUMP_ENABLED",	m_RandomJumpData.bEnabled,		false );			// 추가된 것
			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",			m_RandomJumpData.jumpInterval,		0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",			m_RandomJumpData.jumpUpRate,		0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",			m_RandomJumpData.jumpDownRate,		0 );



			luaManager.EndTable(); // CHASE_MOVE
		}

		luaManager.EndTable(); // INIT_AI
	}
}



void CX2AllyNPCAI::SetMasterUnitData()
{
	KTDXPROFILE();

	// todo!! 미리 한번만 계산해서 가지고 있어야, 아니면 대략 계산하거나
	// t = v0/g
	// l = v0*t - 1/2*g*t^2 = 1/2*v0^2/g
	const CX2GameUnit::PhysicParam& physicParam = m_pMasterNPC->GetPhysicParam();	

	float fHalfTimeOnAir;
	if( physicParam.fGAccel != 0.f )
		fHalfTimeOnAir = physicParam.GetJumpSpeed() / physicParam.fGAccel;	
	else
		fHalfTimeOnAir = 0.f;

	m_fMaxJumpUp	= 0.5f * physicParam.GetJumpSpeed() * fHalfTimeOnAir;
	m_fMaxJumpRight = 2.f * physicParam.GetDashSpeed() * fHalfTimeOnAir - physicParam.fReloadAccel * fHalfTimeOnAir * fHalfTimeOnAir;

	if( 0.f != physicParam.GetDashSpeed() && m_fMaxJumpRight < LINE_RADIUS )
	{
		m_fMaxJumpRight = LINE_RADIUS;
	}
}


// 일정 거리 이상 소환사와 거리가 멀어지면 소환사 옆으로 이동한다
bool CX2AllyNPCAI::RallyToAlly()
{
	if ( m_optrAllyGameUnit == null )
		return false;
	
	if( m_optrAllyGameUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE )
		return false;

	if( m_optrAllyGameUnit->GetNowHp() <= 0.f )
		return false;

	if( NULL == m_pMasterNPC )
		return false;

	if( CX2UnitManager::NCT_BASIC != m_pMasterNPC->GetClassType() )
		return false;

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-12
	/// 어둠의 문 소환 몬스터는 복귀 시키지 않는다.
	if ( CX2GUNPC::NCT_DEFENCE_DUNGEON == m_pMasterNPC->GetNPCCreateType() )
		return false;
#endif // SERV_NEW_DEFENCE_DUNGEON

	if( CX2UnitManager::NUI_EVOKE_TRAP_LIRE_MEMO == m_pMasterNPC->GetNPCTemplet().m_nNPCUnitID )
		return false;

	float fDist = GetDistance( m_optrAllyGameUnit->GetPos(), m_pMasterNPC->GetPos() );
	if( fDist > m_AllyData.allyFarLostRange )
	{
		m_pMasterNPC->SetPosition( m_optrAllyGameUnit->GetPos(), true );
		m_optrTargetGameUnit = m_optrAllyGameUnit;

		TargetUpdate();
		return true;
	}
	else if( fDist > m_AllyData.allyLostRange )
	{
		m_optrTargetGameUnit = m_optrAllyGameUnit;

		TargetUpdate();
		return true;
	}

	return false;
}


// 타게팅된 유닛의 UID를 결정한다.
void CX2AllyNPCAI::Targeting( float fElapsedTime )
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;


	// 자신을 공격한 유닛을 타겟팅
	if( m_optrAttackerGameUnit != null &&
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_pMasterNPC->EstimateFrameAccumPercent( (float) m_TargetData.targetAttakerRate ) == true
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		RandomInt() < m_TargetData.targetAttakerRate
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        )
	{
		m_optrTargetGameUnit = m_optrAttackerGameUnit;
		ResetAttackerGameUnit();
		TargetUpdate();
		return;
	}    

	// 이전 타겟을 유지할까요?
	if( null != m_optrTargetGameUnit )
	{
		if ( CX2GameUnit::GUSI_DIE != m_optrTargetGameUnit->GetGameUnitState() )
		{
			// LostRange 거리 밖이면 타겟하던 것을 없앤다
			const float fDistance3Sq = GetDistance3Sq( m_optrTargetGameUnit->GetPos(), m_pMasterNPC->GetPos() );
			if( fDistance3Sq <= m_TargetData.targetLostRange * m_TargetData.targetLostRange )
            {
			    // 거리 내에 있으나 이전 타겟을 확률에 의해 유지 실패하면
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( m_pMasterNPC->EstimateFrameAccumPercent( (float) m_TargetData.targetPreserveRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			    if ( RandomFloat( 0.f, 100.f ) <= m_TargetData.targetPreserveRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    return TargetUpdate();
            }
		}
        ResetTarget();
	}

	if( null != m_optrTargetGameUnit )
	{
		// 타겟을 갱신할 때가 되었나
		if( m_fElapsedTimeAfterLastTargeting < m_TargetData.targetInterval )
		{
			TargetUpdate();
			return;
		}
		else
		{
			m_fElapsedTimeAfterLastTargeting = 0.f;
		}

		// 타겟팅에 성공할 확률 계산
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if( CX2GUNPC::EstimateFrameOneshotPercent( (float) m_TargetData.targetSuccessRate ) == false )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( RandomInt() >= m_TargetData.targetSuccessRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			TargetUpdate();
			return;
		}
	}



	// 새로운 타겟을 찾는다
	// Targeting 관련 변수
	CX2GameUnit*	pTargetPickedGameUnit			= NULL;
	float			fTargetPickedDistance3Sq		= 0.f;
	
	bool			bFoundNearTargetGameUnit		= false;	// 무조건 타겟되는 거리내의 유저를 찾았는지
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_vecpTempGameUnit.resize( 0 );
    float                       fRate = 0.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		CX2GameUnit* pGameUnit = g_pX2Game->GetUnit(i);
		if( NULL == pGameUnit ||
			pGameUnit->GetNowHp() <= 0.f ||
			pGameUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE ||
#ifdef ADD_NPC_CONDITION_TABLE
			( false == m_TargetData.bTargetOnlyOurTeam && pGameUnit->GetTeam() == m_pMasterNPC->GetTeam() ) || // bTargetOnlyOurTeam가 false 일 때 같은 팀이면 타겟 하지 않기.
			( true == m_TargetData.bTargetOnlyOurTeam && pGameUnit->GetTeam() != m_pMasterNPC->GetTeam() ) // bTargetOnlyOurTeam가 true 일 때 같은 팀이 아니면 타겟 하지 않기.
#else
			pGameUnit->GetTeam() == m_pMasterNPC->GetTeam() 
#endif // ADD_NPC_CONDITION_TABLE
			)
		{
			continue;
		}

		switch( pGameUnit->GetGameUnitType() )
		{
		case CX2GameUnit::GUT_NPC:
			{
				CX2GUNPC* pNPC = static_cast<CX2GUNPC*>( pGameUnit );

#ifdef ADD_NPC_CONDITION_TABLE
				if( m_pMasterNPC == pNPC )
					continue;
#endif // ADD_NPC_CONDITION_TABLE
				switch( pNPC->GetNPCTemplet().m_ClassType )
				{
				case CX2UnitManager::NCT_THING_GATE:
				case CX2UnitManager::NCT_THING_BOX:
				case CX2UnitManager::NCT_THING_TRAP:
				case CX2UnitManager::NCT_THING_DEVICE:
				case CX2UnitManager::NCT_THING_CHECKER:
				case CX2UnitManager::NCT_THING_WORLD_BLOCK:
				//{{ JHKang / 강정훈 / 2010/12/07 / 버프 및 디버프 받지 않는 NPC TYPE
#ifdef NEW_SKILL_2010_11
				case CX2UnitManager::NCT_THING_NOBUFF:		/// 버프/디버프에 영향을 받지 않음()
#endif NEW_SKILL_2010_11
				//}} JHKang / 강정훈 / 2010/12/07 / 버프 및 디버프 받지 않는 NPC TYPE
					{
						continue;
					} break;
				}
			} break;
		}
		
		const float fDistance3Sq = GetDistance3Sq( pGameUnit->GetPos(), m_pMasterNPC->GetPos() );

		if( fDistance3Sq > m_TargetData.targetRange * m_TargetData.targetRange )
			continue;

		// 무조건 타겟이 되는 거리안에 있는 유닛이고
		if( fDistance3Sq < m_TargetData.targetNearRange * m_TargetData.targetNearRange )
		{
			// 기존의 타겟이 없거나, 기존 타겟 보다 가까우면
			if ( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPickedDistance3Sq )
			{
				pTargetPickedGameUnit		= pGameUnit;
				fTargetPickedDistance3Sq	= fDistance3Sq;
				bFoundNearTargetGameUnit	= true;	// NearRange 안쪽의 유닛을 찾았다
			}
		}

		// NearRange안에 유닛이 없는 경우에만 (NearRange 안에 유닛이 있는 경우에는 그 유닛을 무조건 타겟함)
		if ( false == bFoundNearTargetGameUnit )
		{
			switch( m_TargetData.targetPriority )
			{
			case TP_HIGH_LEVEL_FIRST:					
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetUnitLevel() > pTargetPickedGameUnit->GetUnitLevel() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_LOW_LEVEL_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetUnitLevel() < pTargetPickedGameUnit->GetUnitLevel() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_HIGH_HP_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() > pTargetPickedGameUnit->GetNowHp() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_LOW_HP_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() < pTargetPickedGameUnit->GetNowHp() )
						pTargetPickedGameUnit = pGameUnit;
				} break;

			case TP_NEAR_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPickedDistance3Sq )
					{
						fTargetPickedDistance3Sq	= fDistance3Sq;
						pTargetPickedGameUnit		= pGameUnit;
					}
				} break;

			case TP_FAR_FIRST:
				{
					if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPickedDistance3Sq )
					{
						fTargetPickedDistance3Sq	= fDistance3Sq;
						pTargetPickedGameUnit		= pGameUnit;
					}
				} break;

			default:
			case TP_RANDOM:
				{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    m_vecpTempGameUnit.push_back( pGameUnit );
                    fRate = fRate * 0.5f + 0.5f;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
					if( RandomFloat( 0.f, 100.f ) < 50.f || NULL == pTargetPickedGameUnit )
					{
						pTargetPickedGameUnit = pGameUnit;
					}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				} break;

#ifdef SEASON3_MONSTER_2010_12
			case TP_ONLY_ONE:
				{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    m_vecpTempGameUnit.push_back( pGameUnit );
                    fRate = 1.f;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
					if( NULL == pTargetPickedGameUnit )
						pTargetPickedGameUnit = pGameUnit;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				} break;
#endif
			}
		}	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( pTargetPickedGameUnit == NULL && m_vecpTempGameUnit.empty() == false
        && CX2GUNPC::EstimateFrameOneshotPercent( fRate * 100.f ) == true )
    {
        if ( m_vecpTempGameUnit.size() == 1 )
            pTargetPickedGameUnit = m_vecpTempGameUnit.front();
        else
        {
            int i = rand()% m_vecpTempGameUnit.size();
            pTargetPickedGameUnit = m_vecpTempGameUnit[ i ];
        }
    }
    m_vecpTempGameUnit.resize( 0 );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	m_optrTargetGameUnit = pTargetPickedGameUnit;

	return TargetUpdate();
}

// 타게팅된 unit uid를 이용해서 목표 위치를 계산한다
void CX2AllyNPCAI::TargetUpdate()
{
	KTDXPROFILE();	

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;

	if ( null == m_optrTargetGameUnit )
	{
		m_iFinalDestLineIndex	= m_pMasterNPC->GetNPCFrameData().syncData.lastTouchLineIndex;
		m_vFinalDest			= m_pMasterNPC->GetPos();
	}
	else
	{
		m_iFinalDestLineIndex	= m_optrTargetGameUnit->GetLastTouchLineIndex();
		m_vFinalDest			= m_optrTargetGameUnit->GetPos();
	}
}



void CX2AllyNPCAI::Moving( float fElapsedTime )
{
	KTDXPROFILE();	

	CX2GUNPC::STATE_CHANGE_TYPE message = CX2GUNPC::SCT_NO_CHANGE;
	CKTDGLineMap* pLineMap	= g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return; 

	bool bNewIsRight = m_pMasterNPC->GetIsRight();
	const int iLastLineIndex = m_pMasterNPC->GetNPCFrameData().syncData.lastTouchLineIndex;


	// 현재 라인그룹을 유지해야하는데 타겟이 다른 라인그룹에 있다면
	if( true == m_ChaseMoveData.bStayOnCurrentLineGroup )
	{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
		CKTDGLineMap::LineGroupID lineGroupCurr = pLineMap->GetLineGroupIncludesLineData( iLastLineIndex );
		CKTDGLineMap::LineGroupID lineGroupDest = pLineMap->GetLineGroupIncludesLineData( m_iFinalDestLineIndex );

		if( lineGroupCurr.IsValid() == false ||
			lineGroupDest.IsValid() == false ||
			lineGroupCurr != lineGroupDest )
		{
			message = CX2GUNPC::SCT_AI_WAIT;
			goto SEND_AI_MESSAGE;
		}
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
		CKTDGLineMap::LineGroup* pLineGroupCurr = pLineMap->GetLineGroupIncludesLineData( pLineMap->GetLineData( iLastLineIndex ) );
		CKTDGLineMap::LineGroup* pLineGroupDest = pLineMap->GetLineGroupIncludesLineData( pLineMap->GetLineData( m_iFinalDestLineIndex ) );

		if( NULL == pLineGroupDest ||
			NULL == pLineGroupCurr ||
			pLineGroupDest != pLineGroupCurr )
		{
			message = CX2GUNPC::SCT_AI_WAIT;
			goto SEND_AI_MESSAGE;
		}
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
	}
	


	// 랜덤 점프
	if( false == m_ChaseMoveData.bStayOnCurrentLineGroup &&
		true == m_RandomJumpData.bEnabled )
	{
		m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
		
		if( m_fElapsedTimeAfterLastRandomJump > m_RandomJumpData.jumpInterval )
		{
			m_fElapsedTimeAfterLastRandomJump = 0.f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if ( CX2GUNPC::EstimateFrameOneshotPercent( (float) m_RandomJumpData.jumpUpRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( RandomInt() < m_RandomJumpData.jumpUpRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			{
				if( rand()%2 == 0 )
				{
					message = CX2GUNPC::SCT_AI_JUMP;
				}
				else
				{
					message = CX2GUNPC::SCT_AI_JUMP_DIR;
				}
				
				goto SEND_AI_MESSAGE;
			}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            else if( CX2GUNPC::EstimateFrameOneshotPercent( (float) m_RandomJumpData.jumpDownRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			else if( RandomInt() < m_RandomJumpData.jumpDownRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			{
				if( rand()%2 == 0 )
				{
					message = CX2GUNPC::SCT_AI_DOWN;
				}
				else
				{
					message = CX2GUNPC::SCT_AI_DOWN_DIR;
				}
				goto SEND_AI_MESSAGE;
			}
		}
	}


	// 목적지로 이동
	bool bIsTargetOnRight;
	const float MAGIC_LINE_END_DETECT_RANGE = 50.f;
	m_PathFinder.FollowTargetPath( /*fElapsedTime,*/ m_pMasterNPC->GetPos(), iLastLineIndex, 
		m_vFinalDest, m_iFinalDestLineIndex, 
		m_fMaxJumpUp, m_fMaxJumpRight, pLineMap, 
		m_ChaseMoveData.destGap, m_ChaseMoveData.moveGap,
		m_pMasterNPC->GetNPCFrameData().unitCondition.bFootOnLine,
		m_ChaseMoveData.bStayOnCurrentLineGroup,  
		bIsTargetOnRight, MAGIC_LINE_END_DETECT_RANGE 
#ifdef ADD_NPC_CONDITION_TABLE
		, m_ChaseMoveData.bIfCannotFindMoveStateDoWait		
#endif // ADD_NPC_CONDITION_TABLE
		);


	// 타겟을 바라보고 서 있는지
	if( m_pMasterNPC->GetIsRight() != bIsTargetOnRight )
	{
		m_bIsTargetFront = false;
	}
	else
	{
		m_bIsTargetFront = true;
	}


	const float fFinalDestDist = GetDistance( m_pMasterNPC->GetPos(), m_vFinalDest );

	
	switch( m_PathFinder.GetMoveState() )
	{
	case CX2SimplePathFinder::MS_WAIT:	
		{
			message = CX2GUNPC::SCT_AI_WAIT;
		} break;

	case CX2SimplePathFinder::MS_FORCE_LEFT:
	case CX2SimplePathFinder::MS_LEFT:
		{
			bNewIsRight = false;

			// 오른쪽을 보고 있다면 방향을 바꿔야 한다
			if( true == m_pMasterNPC->GetIsRight() )
			{
				m_fElapsedTimeAfterLastDirChange += fElapsedTime;
				if( m_fElapsedTimeAfterLastDirChange > m_ChaseMoveData.dirChangeInterval )
				{
					m_fElapsedTimeAfterLastDirChange = 0.f;
					message = CX2GUNPC::SCT_AI_DIR_CHANGE;
					break;
				}
			}

			message = CX2GUNPC::SCT_AI_WALK;
		} break;

	case CX2SimplePathFinder::MS_FORCE_RIGHT:
	case CX2SimplePathFinder::MS_RIGHT: 
		{
			bNewIsRight = true;

			if( false == m_pMasterNPC->GetIsRight() )
			{
				m_fElapsedTimeAfterLastDirChange += fElapsedTime;
				if( m_fElapsedTimeAfterLastDirChange > m_ChaseMoveData.dirChangeInterval )
				{
					m_fElapsedTimeAfterLastDirChange = 0.f;
					message = CX2GUNPC::SCT_AI_DIR_CHANGE;					
					break;
				}
			}

			message = CX2GUNPC::SCT_AI_WALK;			
		} break;

	case CX2SimplePathFinder::MS_JUMP:
		{
			message = CX2GUNPC::SCT_AI_JUMP;
		} break;

	case CX2SimplePathFinder::MS_DOWN:
		{
			message	= CX2GUNPC::SCT_AI_DOWN;
		} break;

	case CX2SimplePathFinder::MS_JUMP_LEFT:
		{
			bNewIsRight = false;

			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_JUMP_RIGHT:
		{
			bNewIsRight = true;

			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_LEFT:
		{
			bNewIsRight = false;

			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_RIGHT:
		{
			bNewIsRight = true;

			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;
	} // end of switch





	// 걷다가 뛰는 상태로 바뀌거나 뛰다가 걷는 상태로, 서있다가 걷거나 뛰는 상태로 바뀔 때 시간 간격을 둔다.
	switch( message )
	{
	case CX2GUNPC::SCT_AI_WALK:
	case CX2GUNPC::SCT_AI_DASH:
		{
			switch( m_PrevAIMessage )
			{
			case CX2GUNPC::SCT_AI_WAIT:
			case CX2GUNPC::SCT_AI_WALK:
			case CX2GUNPC::SCT_AI_DASH:
				{
					m_fElapsedTimeAfterLastWalkRunUpdate += fElapsedTime;
					if( m_fElapsedTimeAfterLastWalkRunUpdate > m_ChaseMoveData.walkDashInterval )
					{	
						m_fElapsedTimeAfterLastWalkRunUpdate = 0.f;

						if( fFinalDestDist < m_ChaseMoveData.dashRange )
						{
							message = CX2GUNPC::SCT_AI_DASH;
						}
						else
						{
							message = CX2GUNPC::SCT_AI_WALK;
						}
					}
					else
					{
						message = m_PrevAIMessage;
					}
				} break;
			}
		} break;
	}


SEND_AI_MESSAGE:
	if( m_PrevAIMessage == message || CX2GUNPC::SCT_NO_CHANGE == message )
	{
		message = CX2GUNPC::SCT_NO_CHANGE;
	}
	else
	{
		m_PrevAIMessage = message;
		m_pMasterNPC->PushAIMessage( message, bNewIsRight );
	}
}


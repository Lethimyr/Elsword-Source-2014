#include "StdAfx.h"
//#include ".\x2petai.h"

#ifdef SERV_PET_SYSTEM


CX2PETAI::CX2PETAI( CX2PET* pPet )
: m_bNoBrain( false )
, m_bEnabled( true ) 
, m_pPet( pPet )
, m_eAIState( NAS_WAIT )
, m_TargetUnitUID( -1 )
, m_fElapsedTimeAfterLastTargeting( 0.f )
, m_fElapsedTimeAfterLastMoving( 0.f )
, m_fElapsedTimeAfterLastRandomJump( 0.f )
, m_fElapsedTimeAfterLastWalkRunUpdate( 0.f )
, m_fElapsedTimeAfterLastDirChange( 0.f )
, m_fElapsedPatrolCoolingTime( 0.f )
#ifdef AI_FLY
, m_fElapsedTimeAfterLastFlyMoveUpdate( 0.f )
, m_bFlyUnit( false )
, m_bNowStateFly( false )
#endif
, m_optrGameUnitAttackMeOrBeAttackedByMe()
{
	m_PrevAIMessage = CX2PET::SCT_NO_CHANGE;

	m_iFinalDestLineIndex = -1;

	m_bLostTarget = false;

	m_bFleeing = false;

	m_bEnableLuaTargetingFunc	= false;
	m_strLuaTargetingFunc		= "";
		
	m_pAIData = new AIData();
	m_bFleeing = false;

	m_bMasterIsRight = true;

	//m_MonsterUid = -1;
	m_MasterUid = -1;

	m_bFootOnLine = false;
	m_bIsRight = false;
	m_bIsTargetFront = false;
	m_fMaxHP = 0;
	m_fMaxJumpRight = 0.f;
	m_fMaxJumpUp = 0.f;
	m_fNowHP = 0.f;
	m_iLastTouchLineIndex = 0;
	m_iPivotPointLineIndex = 0;
}

CX2PETAI::~CX2PETAI(void)
{
	SAFE_DELETE(m_pAIData);
}

void CX2PETAI::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( true == m_bNoBrain )
		return;

	if( m_bEnabled == false )
	{
		//m_TargetUnitUID = -1;
		return;
	}

	CX2PET::STATE_CHANGE_TYPE message = CX2PET::SCT_NO_CHANGE;
	SetMasterUnitData();


	if( g_pX2Game != NULL && g_pX2Game->IsHost() == false )
		return;

	CKTDGLineMap* pLineMap = NULL;

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	}
	else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
	{
		if( g_pTFieldGame != NULL )
		{
			pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
		}
		else if( g_pSquareGame != NULL )
		{
			pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
		}
	}

	if( NULL == pLineMap )
		return;	


	if( true == m_bEnableLuaTargetingFunc && 
		false == m_strLuaTargetingFunc.empty() )
	{
		m_fElapsedTimeAfterLastTargeting += fElapsedTime;
		if( m_fElapsedTimeAfterLastTargeting > m_pAIData->targetData.targetInterval )
		{
			m_fElapsedTimeAfterLastTargeting = 0.f;
			//string func = "";
			//ConvertWCHARToChar( func, strLuaTargetingFunc.c_str() );
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
			lua_tinker::call<void>( m_pPet->GetLuaManager().GetLuaState(),  m_strLuaTargetingFunc.c_str(), g_pKTDXApp, g_pX2Game, m_pPet );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
			lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  func.c_str(), g_pKTDXApp, g_pX2Game, m_pPet );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		}
		TargetUpdate();
	}
	else
	{
		Targeting( fElapsedTime );
	}
	
	Moving( fElapsedTime );
}

void CX2PETAI::LoadAIDataFromLUA( KLuaManager& luaManager )
{
	if( luaManager.BeginTable( "INIT_AI" ) == true )
	{

		if( luaManager.BeginTable( "TARGET" ) == true )
		{
			LUA_GET_VALUE_UTF8( luaManager, "MANUAL_TARGETING_FUNC",	m_pAIData->targetData.strLuaTargetingFunc,		"" );

			if( false == m_pAIData->targetData.strLuaTargetingFunc.empty() )
			{
				SetLuaTargetingFunc( m_pAIData->targetData.strLuaTargetingFunc );
				SetEnableLuaTargetingFunc( true );
			}			

			LUA_GET_VALUE( luaManager, "TARGET_NPC",				m_pAIData->targetData.targetNPC,					false );
			LUA_GET_VALUE_ENUM( luaManager, "TARGET_PRIORITY",		m_pAIData->targetData.targetPriority,				TARGET_PRIORITY, TP_RANDOM );
			LUA_GET_VALUE( luaManager, "TARGET_INTERVAL",			m_pAIData->targetData.targetInterval,				0 );
			LUA_GET_VALUE( luaManager, "TARGET_NEAR_RANGE",			m_pAIData->targetData.targetNearRange,			0 );
			m_pAIData->targetData.targetNearRange = min( m_pAIData->targetData.targetNearRange, 8000);

			LUA_GET_VALUE( luaManager, "TARGET_RANGE",				m_pAIData->targetData.targetRange,				0 );
			m_pAIData->targetData.targetRange = min( m_pAIData->targetData.targetRange, 9000);

			LUA_GET_VALUE( luaManager, "TARGET_LOST_RANGE",			m_pAIData->targetData.targetLostRange,			0 );
			m_pAIData->targetData.targetLostRange = min( m_pAIData->targetData.targetLostRange, 10000);

			LUA_GET_VALUE( luaManager, "TARGET_SUCCESS_RATE",		m_pAIData->targetData.targetSuccessRate,			0 );
			LUA_GET_VALUE( luaManager, "ATTACK_TARGET_RATE",		m_pAIData->targetData.attackTargetRate,			0 );
			LUA_GET_VALUE( luaManager, "PRESERVE_LAST_TARGET_RATE", m_pAIData->targetData.preserveLastTargetRate,		0 );			
			
			luaManager.EndTable(); // TARGET
		}

		if( luaManager.BeginTable( "CHASE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pAIData->chaseMoveData.moveSplitRange,			0 );
			LUA_GET_VALUE( luaManager, "DEST_GAP",			m_pAIData->chaseMoveData.destGap,					0 );
			LUA_GET_VALUE( luaManager, "MOVE_GAP",			m_pAIData->chaseMoveData.moveGap,					0 );
			LUA_GET_VALUE( luaManager, "RUN_GAP",			m_pAIData->chaseMoveData.runGap,					0 );			

			LUA_GET_VALUE( luaManager, "DIR_CHANGE_INTERVAL", m_pAIData->chaseMoveData.dirChangeInterval,		0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pAIData->chaseMoveData.walkInterval,			0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pAIData->chaseMoveData.nearWalkRate,			0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pAIData->chaseMoveData.farWalkRate,				0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pAIData->chaseMoveData.jumpInterval,			0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pAIData->chaseMoveData.upJumpRate,				0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pAIData->chaseMoveData.upDownRate,				0 );
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pAIData->chaseMoveData.downJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pAIData->chaseMoveData.downDownRate,			0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pAIData->chaseMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pAIData->chaseMoveData.bOnlyThisLineGroup,	false );


			luaManager.EndTable(); // CHASE_MOVE
		}

		
#ifdef AI_FLY
		if( luaManager.BeginTable( "FLY_CHASE_MOVE" ) == true )
		{
			SetAIFly(true);

			LUA_GET_VALUE( luaManager, "DEST_HEIGHT_GAP",		m_pAIData->flymovedata.fDestHeight,			0 );
			LUA_GET_VALUE( luaManager, "DEST_LAND_GAP",			m_pAIData->flymovedata.fDestGap,				0 );
			LUA_GET_VALUE( luaManager, "DEST_AREA",				m_pAIData->flymovedata.fDestArea,				0 );
			LUA_GET_VALUE( luaManager, "FLY_MOVE_INTERVAL",		m_pAIData->flymovedata.fFlyMoveInterval,		0 );			
			
			luaManager.EndTable();
		}
#endif	

		luaManager.EndTable(); // INIT_AI
	}
}

void CX2PETAI::SetMasterUnitData()
{
	KTDXPROFILE();

	m_MasterUid = m_pPet->GetMasterUid();

	const CX2PET::PetCondition& pCondition = m_pPet->GetCondition();
	const CX2PET::PetSyncData& pSyncData = m_pPet->GetSyncData();

	m_vPosition = pSyncData.position;
	m_bIsRight = pSyncData.bIsRight;
	m_iLastTouchLineIndex = pSyncData.lastTouchLineIndex;
	m_bFootOnLine = pCondition.bFootOnLine;
	
	// fix!! 미리 한번만 계산해서 가지고 있어야, 아니면 대략 계산하거나
	// t = v0/g
	// l = v0*t - 1/2*g*t^2 = 1/2*v0^2/g
	const CX2PET::PhysicParam& physicParam = m_pPet->GetPhysicParam();	

	float fHalfTimeOnAir;
	if( physicParam.fGAccel != 0.f )
		fHalfTimeOnAir = physicParam.fJumpSpeed / physicParam.fGAccel;	
	else
		fHalfTimeOnAir = 0.f;

	m_fMaxJumpUp = 0.5f * physicParam.fJumpSpeed * fHalfTimeOnAir;
	m_fMaxJumpRight = 2.f * physicParam.fRunSpeed * fHalfTimeOnAir - physicParam.fReloadAccel * fHalfTimeOnAir * fHalfTimeOnAir;

	if( 0.f != physicParam.fRunSpeed && m_fMaxJumpRight < LINE_RADIUS )
	{
		m_fMaxJumpRight = LINE_RADIUS;
	}
}

void CX2PETAI::UpdateJumpSpeed()
{
	const CX2PET::PhysicParam& physicParam = m_pPet->GetPhysicParam();	

	float fHalfTimeOnAir;
	if( physicParam.fGAccel != 0.f )
		fHalfTimeOnAir = physicParam.fJumpSpeed / physicParam.fGAccel;	
	else
		fHalfTimeOnAir = 0.f;

	m_fMaxJumpUp = 0.5f * physicParam.fJumpSpeed * fHalfTimeOnAir;
	m_fMaxJumpRight = 2.f * physicParam.fRunSpeed * fHalfTimeOnAir - physicParam.fReloadAccel * fHalfTimeOnAir * fHalfTimeOnAir;

	if( 0.f != physicParam.fRunSpeed && m_fMaxJumpRight < LINE_RADIUS )
	{
		m_fMaxJumpRight = LINE_RADIUS;
	}
}

void CX2PETAI::Targeting( float fElapsedTime )
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = NULL;
	
	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
        pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	}
	else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
	{
		if( g_pTFieldGame != NULL )
		{
			pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
		}
		else if( g_pSquareGame != NULL )
		{
			pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
		}
	}

	if( NULL == pLineMap )
		return;	
	

	m_TargetUnitUID = m_pPet->GetMasterUid();
    

	//// 타게팅 할 때가 되었는지 알아보자~
	//{
	//	m_fElapsedTimeAfterLastTargeting += fElapsedTime;
	//	if( m_fElapsedTimeAfterLastTargeting < m_pAIData->targetData.targetInterval )
	//		return TargetUpdate();
		m_fElapsedTimeAfterLastTargeting = 0.f;
	//}

	return TargetUpdate();	
}





void CX2PETAI::TargetUpdate()
{
	KTDXPROFILE();	

	CKTDGLineMap* pLineMap = NULL;

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	}
	else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
	{
		if( g_pTFieldGame != NULL )
		{
			pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
		}
		else if( g_pSquareGame != NULL )
		{
			pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
		}
	}

	if( NULL == pLineMap )
		return;	

	

	if( true == m_bLostTarget )
	{
		m_bLostTarget = false;
	}

	int iTargetLineIndex;
	D3DXVECTOR3 vTargetUnitPos;
	bool bTargetUnitOK = false;

	if( g_pX2Game != NULL )
	{
		CX2GameUnit* pGameUnitAttackMeOrBeAttackedByMe = GetGameUnitAttackMeOrBeAttackedByMe();

		if ( NULL != pGameUnitAttackMeOrBeAttackedByMe )
		{
			bTargetUnitOK = true;
			vTargetUnitPos = pGameUnitAttackMeOrBeAttackedByMe->GetPos();
			iTargetLineIndex = pGameUnitAttackMeOrBeAttackedByMe->GetLastTouchLineIndex();
			m_bMasterIsRight = pGameUnitAttackMeOrBeAttackedByMe->GetIsRight();
		}
		else
		{
			CX2GUUser* pGUUser = (CX2GUUser*)g_pX2Game->GetUserUnitByUID( m_TargetUnitUID );
			if( NULL != pGUUser )
			{
				bTargetUnitOK = true;
				vTargetUnitPos = pGUUser->GetPos();
				iTargetLineIndex = pGUUser->GetFrameData().syncData.lastTouchLineIndex;
				m_bMasterIsRight =  pGUUser->GetIsRight();
			}
		}
		
	}
	else if( g_pTFieldGame != NULL )
	{
		CX2SquareUnit* pUser = (CX2SquareUnit*)g_pTFieldGame->GetSquareUnitByUID( m_TargetUnitUID );
		if( NULL != pUser )
		{
			bTargetUnitOK = true;
			vTargetUnitPos = pUser->GetPos();
			iTargetLineIndex = pUser->GetLastTouchLineIndex();
			m_bMasterIsRight =  pUser->GetIsRight();
		}			
	}
	else if( g_pSquareGame != NULL )
	{
		CX2SquareUnit* pUser = (CX2SquareUnit*)g_pSquareGame->GetSquareUnitByUID( m_TargetUnitUID );
		if( NULL != pUser )
		{
			bTargetUnitOK = true;
			vTargetUnitPos = pUser->GetPos();
			iTargetLineIndex = pUser->GetLastTouchLineIndex();
			m_bMasterIsRight =  pUser->GetIsRight();
		}			
	}


	if( false == bTargetUnitOK )
	{
		m_TargetUnitUID = -1;
		m_bLostTarget = true;			
		return;
	}

	m_iFinalDestLineIndex = iTargetLineIndex;
	m_vFinalDest = vTargetUnitPos;	
}

void CX2PETAI::Moving( float fElapsedTime )
{
	KTDXPROFILE();	

	CX2PET::STATE_CHANGE_TYPE message = CX2PET::SCT_NO_CHANGE;

	CKTDGLineMap* pLineMap = NULL;

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	}
	else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
	{
		if( g_pTFieldGame != NULL )
		{
			pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
		}
		else if( g_pSquareGame != NULL )
		{
			pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
		}
	}

	if( NULL == pLineMap )
		return;	


	int iFinalDestLineIndex = m_iFinalDestLineIndex;
	D3DXVECTOR3 vFinalDest	= m_vFinalDest;
	bool bAIIsRight = false;


	bool bStayOnCurrLineGroup = false;	
	D3DXVECTOR3 vDist = D3DXVECTOR3( 0.f, 0.f, 0.f );

	if( false == bStayOnCurrLineGroup )
	{	
	
#ifdef AI_FLY
		if(GetAIFlyNowState() == false)
#endif
		{
			m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
			if( m_fElapsedTimeAfterLastRandomJump > m_pAIData->chaseMoveData.jumpInterval )
			{
				m_fElapsedTimeAfterLastRandomJump = 0.f;
				const float fJumpChance = RandomFloat( 0.f, 100.f );

				if( vFinalDest.y > m_vPosition.y )
				{
					if( fJumpChance < m_pAIData->chaseMoveData.upJumpRate )
					{	
						message = CX2PET::SCT_AI_JUMP;
						goto SEND_AI_MESSAGE;
					}
					else if( 100-fJumpChance < m_pAIData->chaseMoveData.upDownRate )
					{
						message = CX2PET::SCT_AI_DOWN;
						goto SEND_AI_MESSAGE;
					}
				}
				else
				{
					if( fJumpChance < m_pAIData->chaseMoveData.downJumpRate )
					{
						message = CX2PET::SCT_AI_JUMP;
						goto SEND_AI_MESSAGE;
					}
					else if( 100-fJumpChance < m_pAIData->chaseMoveData.downDownRate )
					{
						message = CX2PET::SCT_AI_DOWN;
						goto SEND_AI_MESSAGE;
					}
				}
			}
		}			
	}

//#ifdef SERV_PET_SYSTEM

	// 자 타게팅된 목적지로 이동을 해보자~
	float fDestGap = (float)m_pAIData->chaseMoveData.destGap;
	bool bIsTargetOnRight;
	if( false == m_bFleeing )
	{
#ifdef AI_FLY
		if( GetAIFlyNowState() == true )
		{
			D3DXVECTOR3 vDistPos = vFinalDest;
			if( m_bLostTarget == true )
			{
				vDistPos.y -= m_pAIData->flymovedata.fDestHeight;
			}
			
			m_PathFinder.SimpleFollowTargetPathFly( fElapsedTime, m_vPosition, m_iLastTouchLineIndex, vDistPos, iFinalDestLineIndex,
				m_fMaxJumpUp * 2.f, m_fMaxJumpRight,
				(const float)m_pAIData->flymovedata.fDestHeight, (const float)m_pAIData->flymovedata.fDestArea,
				pLineMap, (const float)m_pAIData->flymovedata.fDestGap, (const float)m_pAIData->chaseMoveData.moveGap,
				m_bFootOnLine, bStayOnCurrLineGroup, bIsTargetOnRight, m_pAIData->chaseMoveData.m_fLineEndDetectRange );
		}
		else
#endif
		{
			m_PathFinder.SimpleFollowTargetPath( fElapsedTime, m_vPosition, m_iLastTouchLineIndex, vFinalDest, iFinalDestLineIndex,
				m_fMaxJumpUp, m_fMaxJumpRight, pLineMap, (const float)fDestGap, (const float)m_pAIData->chaseMoveData.moveGap,
				m_bFootOnLine, bStayOnCurrLineGroup, bIsTargetOnRight, m_pAIData->chaseMoveData.m_fLineEndDetectRange );
		}		
	}
	else
	{
		m_PathFinder.SimpleFollowTargetPath( fElapsedTime, m_vPosition, m_iLastTouchLineIndex, vFinalDest, iFinalDestLineIndex,
			m_fMaxJumpUp, m_fMaxJumpRight, pLineMap, (const float)fDestGap, (const float)m_pAIData->chaseMoveData.moveGap,
			m_bFootOnLine, bStayOnCurrLineGroup, bIsTargetOnRight, m_pAIData->chaseMoveData.m_fLineEndDetectRange );
	}

	if( m_bIsRight != bIsTargetOnRight )
	{
		m_bIsTargetFront = false;
	}
	else
	{
		m_bIsTargetFront = true;
	}
//#endif // PET

	bAIIsRight = m_bIsRight;
	vDist = vFinalDest - m_vPosition;
	const float fFinalDestDist = D3DXVec3LengthSq( &vDist );

	switch( m_PathFinder.GetMoveState() )
	{
	case CX2SimplePathFinder::MS_WAIT:	
		{
			message = CX2PET::SCT_AI_WAIT;
		} break;

	case CX2SimplePathFinder::MS_FORCE_LEFT:
	case CX2SimplePathFinder::MS_LEFT:
		{
			bAIIsRight = false;

			if( true == m_bIsRight )
			{
				if( CX2PET::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2PET::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2PET::SCT_AI_DASH	== m_PrevAIMessage )
				{	
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval 
#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
						&& m_pPet->IsSpeedTime() == false
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET		
						) 
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2PET::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			message = CX2PET::SCT_AI_WALK;
		} break;

	case CX2SimplePathFinder::MS_FORCE_RIGHT:
	case CX2SimplePathFinder::MS_RIGHT: 
		{
			bAIIsRight = true;

			if( false == m_bIsRight )
			{
				if( CX2PET::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2PET::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2PET::SCT_AI_DASH	== m_PrevAIMessage )
				{
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval 
#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
						&& m_pPet->IsSpeedTime() == false
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET
						)
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2PET::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			message = CX2PET::SCT_AI_WALK;			
		} break;

	case CX2SimplePathFinder::MS_JUMP:
		{
			message = CX2PET::SCT_AI_JUMP;
		} break;

	case CX2SimplePathFinder::MS_DOWN:
		{
			message	= CX2PET::SCT_AI_DOWN;
		} break;

	case CX2SimplePathFinder::MS_JUMP_LEFT:
		{
			bAIIsRight = false;
			message	= CX2PET::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_JUMP_RIGHT:
		{
			bAIIsRight = true;
			message	= CX2PET::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_LEFT:
		{
			bAIIsRight = false;
			message	= CX2PET::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_RIGHT:
		{
			bAIIsRight = true;
			message	= CX2PET::SCT_AI_JUMP_DIR;
		} break;

#ifdef AI_FLY
	case CX2SimplePathFinder::MS_FLY_LEFT_FRONT:
	case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_UP:
	case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_DOWN:
	case CX2SimplePathFinder::MS_FLY_LEFT_BACK:
	case CX2SimplePathFinder::MS_FLY_LEFT_BACK_UP:
	case CX2SimplePathFinder::MS_FLY_LEFT_BACK_DOWN:
		{
			bAIIsRight = false;

			if( m_bIsRight == true )
			{
				if( CX2PET::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_FRONT	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_FRONT_UP	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_FRONT_DOWN	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_BACK	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_BACK_UP	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_BACK_DOWN	== m_PrevAIMessage )
				{		
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2PET::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			switch( m_PathFinder.GetMoveState() )
			{
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT:
				message = CX2PET::SCT_AI_FLY_FRONT;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_UP:
				message = CX2PET::SCT_AI_FLY_FRONT_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_DOWN:
				message = CX2PET::SCT_AI_FLY_FRONT_DOWN;
				break;

			case CX2SimplePathFinder::MS_FLY_LEFT_BACK:
				message = CX2PET::SCT_AI_FLY_BACK;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_BACK_UP:
				message = CX2PET::SCT_AI_FLY_BACK_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_BACK_DOWN:
				message = CX2PET::SCT_AI_FLY_BACK_DOWN;
				break;
			default:
				message = CX2PET::SCT_NO_CHANGE;
			}
		}
		break;
	case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT:
	case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_UP:
	case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_DOWN:
	case CX2SimplePathFinder::MS_FLY_RIGHT_BACK:
	case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_UP:
	case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_DOWN:
		{
			bAIIsRight = true;

			if( m_bIsRight == false )
			{
				if( CX2PET::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_FRONT	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_FRONT_UP	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_FRONT_DOWN	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_BACK	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_BACK_UP	== m_PrevAIMessage ||
					CX2PET::SCT_AI_FLY_BACK_DOWN	== m_PrevAIMessage )
				{		
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2PET::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			switch( m_PathFinder.GetMoveState() )
			{
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT:
				message = CX2PET::SCT_AI_FLY_FRONT;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_UP:
				message = CX2PET::SCT_AI_FLY_FRONT_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_DOWN:
				message = CX2PET::SCT_AI_FLY_FRONT_DOWN;
				break;

			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK:
				message = CX2PET::SCT_AI_FLY_BACK;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_UP:
				message = CX2PET::SCT_AI_FLY_BACK_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_DOWN:
				message = CX2PET::SCT_AI_FLY_BACK_DOWN;
				break;
			default:
				message = CX2PET::SCT_NO_CHANGE;
			}
		}
		break;
	case CX2SimplePathFinder::MS_FLY_UP:
		{
			message = CX2PET::SCT_AI_FLY_UP;
		}
		break;
	case CX2SimplePathFinder::MS_FLY_DOWN:
		{
			message = CX2PET::SCT_AI_FLY_DOWN;
		}
		break;
#endif
	} // end of switch

#ifdef AI_FLY	
	if( ( m_PrevAIMessage == CX2PET::SCT_AI_FLY_FRONT || m_PrevAIMessage == CX2PET::SCT_AI_FLY_BACK ||
		m_PrevAIMessage == CX2PET::SCT_AI_FLY_FRONT_UP || m_PrevAIMessage == CX2PET::SCT_AI_FLY_BACK_UP || 
		m_PrevAIMessage == CX2PET::SCT_AI_FLY_FRONT_DOWN || m_PrevAIMessage == CX2PET::SCT_AI_FLY_BACK_DOWN || m_PrevAIMessage == CX2PET::SCT_AI_WAIT ) &&
		( message == CX2PET::SCT_AI_FLY_FRONT || message == CX2PET::SCT_AI_FLY_BACK ||
		message == CX2PET::SCT_AI_FLY_FRONT_UP || message == CX2PET::SCT_AI_FLY_BACK_UP ||
		message == CX2PET::SCT_AI_FLY_FRONT_DOWN || message == CX2PET::SCT_AI_FLY_BACK_DOWN ) )
	{
		m_fElapsedTimeAfterLastFlyMoveUpdate += fElapsedTime;
		if( m_fElapsedTimeAfterLastFlyMoveUpdate > (float)m_pAIData->flymovedata.fFlyMoveInterval )
		{
			m_fElapsedTimeAfterLastFlyMoveUpdate = 0.f;
		}
		else
		{
			message = m_PrevAIMessage;
		}
	}	
#endif




	// walk, dash 중에 walk, dash로의 상태변화는 walk interval 간격으로 갱신
	if( ( CX2PET::SCT_AI_WAIT	== m_PrevAIMessage ||
		CX2PET::SCT_AI_WALK	== m_PrevAIMessage ||
		CX2PET::SCT_AI_DASH	== m_PrevAIMessage ) &&
		( CX2PET::SCT_AI_WALK == message || 
		CX2PET::SCT_AI_DASH == message ) )
	{
		m_fElapsedTimeAfterLastWalkRunUpdate += fElapsedTime;
		if( m_fElapsedTimeAfterLastWalkRunUpdate > (float)m_pAIData->chaseMoveData.walkInterval )
		{	
			m_fElapsedTimeAfterLastWalkRunUpdate = 0.f;

#ifdef CONVERSION_VS
			if( fFinalDestDist >= pow( static_cast<float>(m_pAIData->chaseMoveData.moveSplitRange), 2 ) )
#else CONVERSION_VS
			if( fFinalDestDist >= pow( m_pAIData->chaseMoveData.moveSplitRange, 2 ) )
#endif CONVERSION_VS
			{
#ifdef CONVERSION_VS
				if( fFinalDestDist >= pow( static_cast<float>(m_pAIData->chaseMoveData.runGap), 2 ) )
#else CONVERSION_VS
				if( fFinalDestDist >= pow( m_pAIData->chaseMoveData.runGap, 2 ) )
#endif CONVERSION_VS
				{					
					message = CX2PET::SCT_AI_DASH;				
				}
				else
				{
					if( m_PrevAIMessage != CX2PET::SCT_AI_DASH )
						message = CX2PET::SCT_AI_WALK;
					else
						message = CX2PET::SCT_AI_DASH;
				}
			}
			//else
			//{
			//	message = CX2PET::SCT_AI_WAIT;
			//}
		}
		else
		{
			message = m_PrevAIMessage;
		}
	}

SEND_AI_MESSAGE:

	if( m_PrevAIMessage == message || CX2PET::SCT_NO_CHANGE == message )
	{
		message = CX2PET::SCT_NO_CHANGE;
	}
	else
	{
		m_PrevAIMessage = message;
		m_pPet->PushAIMessage( message, bAIIsRight );
	}
}

#endif
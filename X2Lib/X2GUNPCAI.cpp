#include "StdAfx.h"
#include ".\x2gunpcai.h"

CX2GUNPCAI::CX2GUNPCAI( CX2GUNPC* pMasterNPC ) :
CX2NPCAI( pMasterNPC, CX2NPCAI::NAT_NORMAL )
, m_bNowPatroling( false )
{
	m_PrevAIMessage = CX2GUNPC::SCT_NO_CHANGE;

	m_iFinalDestLineIndex = -1;

	m_bLostTarget = false;
	SetNowPatroling( false );

	m_bFleeing = false;

	m_bEnableLuaTargetingFunc	= false;
	m_strLuaTargetingFunc		= "";

	m_fScanCloakingNearRange	= 0.f;
	m_fScanCloaking				= 0.f;
	m_bScanCloaking				= false;
	//{{ oasis : 김상윤 // 2009-10-7 ////////////////
	m_pMainAIData = new AIData();	//FieldFix: new로 생성할 필요 없음
	m_pSubAIData = new AIData();
	SetAItoMainAIData(); // m_pAIData = m_pMainAIData;
	//}} oasis : 김상윤 // 2009-10-7 ////////////////
#ifdef RIDING_MONSTER
	m_bRideToUnit = false;
#endif RIDING_MONSTER

#ifdef SEASON3_MONSTER_2010_12
	m_bTargetedOnlyOne = false;
#endif
	
	m_bFootOnLine	= false;
	m_bIsRight		= false;
	m_fMaxHP		= 0.f;
	m_fMaxJumpRight	= 0.f;
	m_fMaxJumpUp	= 0.f;
	m_fNowHP		= 0.f;
	m_iLastTouchLineIndex	= -1;
	m_iPivotPointLineIndex	= -1;
}

/*virtual*/ CX2GUNPCAI::~CX2GUNPCAI(void)
{
	//{{ oasis : 김상윤 // 2009-10-7 ////////////////
	SAFE_DELETE(m_pMainAIData);		//FieldFix: new로 생성할 필요 없었음
	SAFE_DELETE(m_pSubAIData);
	//}} oasis : 김상윤 // 2009-10-7 ////////////////
}

void CX2GUNPCAI::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( true == m_bNoBrain )
		return;

	if( false == m_bEnabled )
		return;
	
	CX2GUNPC::STATE_CHANGE_TYPE message = CX2GUNPC::SCT_NO_CHANGE; // FieldFix: 사용안함
	SetMasterUnitData();

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();	//FieldFix: 이것도 가지고 있어도 될듯


	// 루아 Function에 의해 타겟팅을 하는 것이면
	if( true == m_bEnableLuaTargetingFunc && 
		false == m_strLuaTargetingFunc.empty() )
	{
		m_fElapsedTimeAfterLastTargeting += fElapsedTime;
		if( m_fElapsedTimeAfterLastTargeting > m_pAIData->targetData.targetInterval )
		{
			m_fElapsedTimeAfterLastTargeting = 0.f;
			//string func = "";
			//ConvertWCHARToChar( func, m_wstrLuaTargetingFunc.c_str() );
//#ifdef	X2OPTIMIZE_NPC_LUASPACE_SHARING
			lua_tinker::call<void>( m_pMasterNPC->GetFunctionLuaState(),  m_strLuaTargetingFunc.c_str(), g_pKTDXApp, g_pX2Game, m_pMasterNPC );
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
	// 그룹AI가 켜져 있으면
	//{{oasis:김상윤////2009-10-7////
	else if(m_pMasterNPC->GetGroupAICommand_LUA() != CX2GUNPC::GAI_CMD_OFF ) // GROUP AI enable
	{		
		switch ( m_pMasterNPC->GetGroupAICommand_LUA() )
		{
		case CX2GUNPC::GAI_CMD_HOLD:
			{
				CX2GameUnit* pGroupAiTargetGameUnit = g_pX2Game->GetNearestUserUnitInSpecificRange( m_pMasterNPC->GetPos(), 0.0f );
				m_pMasterNPC->SetTargetUnit( pGroupAiTargetGameUnit );
			} break;

		default:
			{
				CX2GameUnit* pGroupAiTargetGameUnit = m_pMasterNPC->GetGroupAITargetGameUnit();
				m_pMasterNPC->SetTargetUnit( pGroupAiTargetGameUnit );
			} break;
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	//}}oasis:김상윤////2009-10-7////
	else	// 그룹 AI를 사용 하는 것이 아니면
	{
		// 현재 타겟이 NPC 이면
		if( true == m_pAIData->targetData.targetNPC )
		{
			TargetingNPC( fElapsedTime );
		}
		else
		{
			Targeting( fElapsedTime );
		}
	}

	Moving( fElapsedTime );
}

void CX2GUNPCAI::LoadAIDataFromLUA( KLuaManager& luaManager )
{

	//FieldFix: BegingTable 안으로 들어가는 것이 좋을듯...
	CX2NPCAI::LoadAIDataFromLUA( luaManager );
	if( true == m_bNoBrain )
		return;


	if( luaManager.BeginTable( "INIT_AI" ) == true )
	{

		if( luaManager.BeginTable( "TARGET" ) == true )
		{
			LUA_GET_VALUE_UTF8( luaManager, "MANUAL_TARGETING_FUNC",	m_pMainAIData->targetData.strLuaTargetingFunc,		"" );

			if( false == m_pMainAIData->targetData.strLuaTargetingFunc.empty() )
			{
				SetLuaTargetingFunc( m_pMainAIData->targetData.strLuaTargetingFunc );
				SetEnableLuaTargetingFunc( true );
			}			

			LUA_GET_VALUE( luaManager, "TARGET_NPC",				m_pMainAIData->targetData.targetNPC,					false );
			LUA_GET_VALUE_ENUM( luaManager, "TARGET_PRIORITY",		m_pMainAIData->targetData.targetPriority,				TARGET_PRIORITY, TP_RANDOM );
			LUA_GET_VALUE( luaManager, "TARGET_INTERVAL",			m_pMainAIData->targetData.targetInterval,				0 );
			LUA_GET_VALUE( luaManager, "TARGET_HEIGHT_LIMIT",		m_pMainAIData->targetData.targetHeightLimit,			100 );
			LUA_GET_VALUE( luaManager, "TARGET_NEAR_RANGE",			m_pMainAIData->targetData.targetNearRange,			0 );
			m_pMainAIData->targetData.targetNearRange = min( m_pMainAIData->targetData.targetNearRange, 8000);

			LUA_GET_VALUE( luaManager, "TARGET_RANGE",				m_pMainAIData->targetData.targetRange,				0 );
			m_pMainAIData->targetData.targetRange	 = min( m_pMainAIData->targetData.targetRange, 9000);

			LUA_GET_VALUE( luaManager, "TARGET_LOST_RANGE",			m_pMainAIData->targetData.targetLostRange,			0 );
			m_pMainAIData->targetData.targetLostRange = min( m_pMainAIData->targetData.targetLostRange, 10000);

			LUA_GET_VALUE( luaManager, "TARGET_SUCCESS_RATE",		m_pMainAIData->targetData.targetSuccessRate,			0.0f );
			LUA_GET_VALUE( luaManager, "ATTACK_TARGET_RATE",		m_pMainAIData->targetData.attackTargetRate,			0.0f );
			LUA_GET_VALUE( luaManager, "PRESERVE_LAST_TARGET_RATE", m_pMainAIData->targetData.preserveLastTargetRate,		0.0f );			

			luaManager.EndTable(); // TARGET
		}

		if( luaManager.BeginTable( "CHASE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pMainAIData->chaseMoveData.moveSplitRange,			0 );
			LUA_GET_VALUE( luaManager, "DEST_GAP",			m_pMainAIData->chaseMoveData.destGap,					0 );
			LUA_GET_VALUE( luaManager, "MOVE_GAP",			m_pMainAIData->chaseMoveData.moveGap,					0 );

			LUA_GET_VALUE( luaManager, "DIR_CHANGE_INTERVAL", m_pMainAIData->chaseMoveData.dirChangeInterval,		0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pMainAIData->chaseMoveData.walkInterval,			0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pMainAIData->chaseMoveData.nearWalkRate,			0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pMainAIData->chaseMoveData.farWalkRate,				0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pMainAIData->chaseMoveData.jumpInterval,			0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pMainAIData->chaseMoveData.upJumpRate,				0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pMainAIData->chaseMoveData.upDownRate,				0 );
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pMainAIData->chaseMoveData.downJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pMainAIData->chaseMoveData.downDownRate,			0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pMainAIData->chaseMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pMainAIData->chaseMoveData.bOnlyThisLineGroup,	false );


			luaManager.EndTable(); // CHASE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pMainAIData->escapeMoveData.moveSplitRange,		0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_GAP",		m_pMainAIData->escapeMoveData.escapeGap,			0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pMainAIData->escapeMoveData.walkInterval,		0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pMainAIData->escapeMoveData.nearWalkRate,		0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pMainAIData->escapeMoveData.farWalkRate,		0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pMainAIData->escapeMoveData.jumpInterval,		0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pMainAIData->escapeMoveData.upJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pMainAIData->escapeMoveData.upDownRate,			0 );			
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pMainAIData->escapeMoveData.downJumpRate,		0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pMainAIData->escapeMoveData.downDownRate,		0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pMainAIData->escapeMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			luaManager.EndTable(); // ESCAPE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_CONDITION" ) == true )
		{
			LUA_GET_VALUE( luaManager, "RATE",			m_pMainAIData->escapeCondition.escapeRate,		0 );
			LUA_GET_VALUE( luaManager, "MY_HP",			m_pMainAIData->escapeCondition.myHP,				0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_RANGE",	m_pMainAIData->escapeCondition.escapeRange,		0.f );
			// 09.04.16 태완 : Escape Condition에 Flage Check 추가
			LUA_GET_VALUE( luaManager, "FLAG_TRUE",		m_pMainAIData->escapeCondition.m_iFlagTrue,		-1 );

			if( m_pMainAIData->escapeCondition.escapeRate  != 0 ||
				m_pMainAIData->escapeCondition.myHP		 != 0 ||
				m_pMainAIData->escapeCondition.escapeRange != 0.f )
			{
				if( m_pMainAIData->escapeCondition.escapeRate == 0 )
					m_pMainAIData->escapeCondition.escapeRate = 100;

				if( m_pMainAIData->escapeCondition.myHP		== 0 )
					m_pMainAIData->escapeCondition.myHP		= 100;

				if( m_pMainAIData->escapeCondition.escapeRange == 0.f )
					m_pMainAIData->escapeCondition.escapeRange = 10000.f;
			}

			luaManager.EndTable(); // ESCAPE_CONDITION
		}

		if( luaManager.BeginTable( "PATROL_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "PATROL_BEGIN_RATE",		m_pMainAIData->patrolMoveData.patrolBeginRate,		0 );
			LUA_GET_VALUE( luaManager, "PATROL_RANGE",			m_pMainAIData->patrolMoveData.patrolRange,			0 );
			LUA_GET_VALUE( luaManager, "PATROL_COOL_TIME",		m_pMainAIData->patrolMoveData.patrolCoolingTime,		0 );
			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pMainAIData->patrolMoveData.bOnlyThisLineGroup,		false );

			luaManager.EndTable(); // PATROL_MOVE
		}

#ifdef AI_FLY
		if( luaManager.BeginTable( "FLY_CHASE_MOVE" ) == true )
		{
			SetAIFly(true);

			LUA_GET_VALUE( luaManager, "DEST_HEIGHT_GAP",		m_pMainAIData->flymovedata.fDestHeight,			0 );
			LUA_GET_VALUE( luaManager, "DEST_LAND_GAP",			m_pMainAIData->flymovedata.fDestGap,				0 );
			LUA_GET_VALUE( luaManager, "DEST_AREA",				m_pMainAIData->flymovedata.fDestArea,				0 );
			LUA_GET_VALUE( luaManager, "FLY_MOVE_INTERVAL",		m_pMainAIData->flymovedata.fFlyMoveInterval,		0 );			

			LUA_GET_VALUE( luaManager, "UNDERWATER_MODE",		m_pMainAIData->flymovedata.bUnderWaterMode,		false );			
			luaManager.EndTable();
		}
#endif	

		luaManager.EndTable(); // INIT_AI
	}
	//{{ oasis : 김상윤 // 2009-10-7 ////////////////
	if( luaManager.BeginTable( "INIT_SUB_AI" ) == true )
	{

		if( luaManager.BeginTable( "TARGET" ) == true )
		{
			LUA_GET_VALUE_UTF8( luaManager, "MANUAL_TARGETING_FUNC",	m_pSubAIData->targetData.strLuaTargetingFunc,		"" );

			if( false == m_pSubAIData->targetData.strLuaTargetingFunc.empty() )
			{
				SetLuaTargetingFunc( m_pSubAIData->targetData.strLuaTargetingFunc );
				SetEnableLuaTargetingFunc( true );
			}			

			LUA_GET_VALUE( luaManager, "TARGET_NPC",				m_pSubAIData->targetData.targetNPC,					false );
			LUA_GET_VALUE_ENUM( luaManager, "TARGET_PRIORITY",		m_pSubAIData->targetData.targetPriority,				TARGET_PRIORITY, TP_RANDOM );
			LUA_GET_VALUE( luaManager, "TARGET_INTERVAL",			m_pSubAIData->targetData.targetInterval,				0 );
			LUA_GET_VALUE( luaManager, "TARGET_HEIGHT_LIMIT",		m_pMainAIData->targetData.targetHeightLimit,			100 );
			LUA_GET_VALUE( luaManager, "TARGET_NEAR_RANGE",			m_pSubAIData->targetData.targetNearRange,			0 );
			m_pSubAIData->targetData.targetNearRange = min( m_pSubAIData->targetData.targetNearRange, 8000);

			LUA_GET_VALUE( luaManager, "TARGET_RANGE",				m_pSubAIData->targetData.targetRange,				0 );
			m_pSubAIData->targetData.targetRange	 = min( m_pSubAIData->targetData.targetRange, 9000);

			LUA_GET_VALUE( luaManager, "TARGET_LOST_RANGE",			m_pSubAIData->targetData.targetLostRange,			0 );
			m_pSubAIData->targetData.targetLostRange = min( m_pSubAIData->targetData.targetLostRange, 10000);

			LUA_GET_VALUE( luaManager, "TARGET_SUCCESS_RATE",		m_pSubAIData->targetData.targetSuccessRate,			0 );
			LUA_GET_VALUE( luaManager, "ATTACK_TARGET_RATE",		m_pSubAIData->targetData.attackTargetRate,			0 );
			LUA_GET_VALUE( luaManager, "PRESERVE_LAST_TARGET_RATE", m_pSubAIData->targetData.preserveLastTargetRate,		0 );			

			luaManager.EndTable(); // TARGET
		}

		if( luaManager.BeginTable( "CHASE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pSubAIData->chaseMoveData.moveSplitRange,			0 );
			LUA_GET_VALUE( luaManager, "DEST_GAP",			m_pSubAIData->chaseMoveData.destGap,					0 );
			LUA_GET_VALUE( luaManager, "MOVE_GAP",			m_pSubAIData->chaseMoveData.moveGap,					0 );

			LUA_GET_VALUE( luaManager, "DIR_CHANGE_INTERVAL", m_pSubAIData->chaseMoveData.dirChangeInterval,		0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pSubAIData->chaseMoveData.walkInterval,			0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pSubAIData->chaseMoveData.nearWalkRate,			0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pSubAIData->chaseMoveData.farWalkRate,				0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pSubAIData->chaseMoveData.jumpInterval,			0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pSubAIData->chaseMoveData.upJumpRate,				0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pSubAIData->chaseMoveData.upDownRate,				0 );
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pSubAIData->chaseMoveData.downJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pSubAIData->chaseMoveData.downDownRate,			0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pSubAIData->chaseMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pSubAIData->chaseMoveData.bOnlyThisLineGroup,	false );


			luaManager.EndTable(); // CHASE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "MOVE_SPLIT_RANGE",	m_pSubAIData->escapeMoveData.moveSplitRange,		0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_GAP",		m_pSubAIData->escapeMoveData.escapeGap,			0 );

			LUA_GET_VALUE( luaManager, "WALK_INTERVAL",		m_pSubAIData->escapeMoveData.walkInterval,		0 );
			LUA_GET_VALUE( luaManager, "NEAR_WALK_RATE",	m_pSubAIData->escapeMoveData.nearWalkRate,		0 );
			LUA_GET_VALUE( luaManager, "FAR_WALK_RATE",		m_pSubAIData->escapeMoveData.farWalkRate,		0 );

			LUA_GET_VALUE( luaManager, "JUMP_INTERVAL",		m_pSubAIData->escapeMoveData.jumpInterval,		0 );
			LUA_GET_VALUE( luaManager, "UP_JUMP_RATE",		m_pSubAIData->escapeMoveData.upJumpRate,			0 );
			LUA_GET_VALUE( luaManager, "UP_DOWN_RATE",		m_pSubAIData->escapeMoveData.upDownRate,			0 );			
			LUA_GET_VALUE( luaManager, "DOWN_JUMP_RATE",	m_pSubAIData->escapeMoveData.downJumpRate,		0 );
			LUA_GET_VALUE( luaManager, "DOWN_DOWN_RATE",	m_pSubAIData->escapeMoveData.downDownRate,		0 );

			LUA_GET_VALUE( luaManager, "LINE_END_RANGE",	m_pSubAIData->escapeMoveData.m_fLineEndDetectRange,	LINE_RADIUS * 1.5f	);

			luaManager.EndTable(); // ESCAPE_MOVE
		}

		if( luaManager.BeginTable( "ESCAPE_CONDITION" ) == true )
		{
			LUA_GET_VALUE( luaManager, "RATE",			m_pSubAIData->escapeCondition.escapeRate,		0 );
			LUA_GET_VALUE( luaManager, "MY_HP",			m_pSubAIData->escapeCondition.myHP,				0 );
			LUA_GET_VALUE( luaManager, "ESCAPE_RANGE",	m_pSubAIData->escapeCondition.escapeRange,		0.f );
			// 09.04.16 태완 : Escape Condition에 Flage Check 추가
			LUA_GET_VALUE( luaManager, "FLAG_TRUE",		m_pSubAIData->escapeCondition.m_iFlagTrue,		-1 );

			if( m_pSubAIData->escapeCondition.escapeRate  != 0 ||
				m_pSubAIData->escapeCondition.myHP		 != 0 ||
				m_pSubAIData->escapeCondition.escapeRange != 0.f )
			{
				if( m_pSubAIData->escapeCondition.escapeRate == 0 )
					m_pSubAIData->escapeCondition.escapeRate = 100;

				if( m_pSubAIData->escapeCondition.myHP		== 0 )
					m_pSubAIData->escapeCondition.myHP		= 100;

				if( m_pSubAIData->escapeCondition.escapeRange == 0.f )
					m_pSubAIData->escapeCondition.escapeRange = 10000.f;
			}

			luaManager.EndTable(); // ESCAPE_CONDITION
		}

		if( luaManager.BeginTable( "PATROL_MOVE" ) == true )
		{
			LUA_GET_VALUE( luaManager, "PATROL_BEGIN_RATE",		m_pSubAIData->patrolMoveData.patrolBeginRate,		0 );
			LUA_GET_VALUE( luaManager, "PATROL_RANGE",			m_pSubAIData->patrolMoveData.patrolRange,			0 );
			LUA_GET_VALUE( luaManager, "PATROL_COOL_TIME",		m_pSubAIData->patrolMoveData.patrolCoolingTime,		0 );
			LUA_GET_VALUE( luaManager, "ONLY_THIS_LINE_GROUP",	m_pSubAIData->patrolMoveData.bOnlyThisLineGroup,		false );

			luaManager.EndTable(); // PATROL_MOVE
		}

#ifdef AI_FLY
		if( luaManager.BeginTable( "FLY_CHASE_MOVE" ) == true )
		{
			SetAIFly(true);

			LUA_GET_VALUE( luaManager, "DEST_HEIGHT_GAP",		m_pSubAIData->flymovedata.fDestHeight,			0 );
			LUA_GET_VALUE( luaManager, "DEST_LAND_GAP",			m_pSubAIData->flymovedata.fDestGap,				0 );
			LUA_GET_VALUE( luaManager, "DEST_AREA",				m_pSubAIData->flymovedata.fDestArea,				0 );
			LUA_GET_VALUE( luaManager, "FLY_MOVE_INTERVAL",		m_pSubAIData->flymovedata.fFlyMoveInterval,		0 );			

			LUA_GET_VALUE( luaManager, "UNDERWATER_MODE",		m_pSubAIData->flymovedata.bUnderWaterMode,		false );			

			luaManager.EndTable();
		}
#endif	

		luaManager.EndTable(); // INIT_SUB_AI
	}
	//}} oasis : 김상윤 // 2009-10-7 ////////////////
}

void CX2GUNPCAI::SetMasterUnitData()
{
	KTDXPROFILE();

	const CX2GUNPC::NPCFrameData& frameData = m_pMasterNPC->GetNPCFrameData();
	//const CX2GUNPC::NPCSyncData& syncData = m_pMasterNPC->GetNPCSyncData();

#ifdef RIDING_MONSTER
	if(m_pMasterNPC->GetRideState() == CX2GUNPC::RS_ON_RIDING && 
		m_pMasterNPC->GetRideType() ==  CX2GUNPC::RT_RIDER )
	{
		const CX2GUNPC::NPCFrameData& ridingFrameData = m_pMasterNPC->GetRideUnit()->GetNPCFrameData();
		m_vPosition = ridingFrameData.syncData.position;	//FieldFix: 포인터 등으로 해당 부분 연결해되 될듯..
		m_bIsRight = ridingFrameData.syncData.bIsRight;
		m_iLastTouchLineIndex = ridingFrameData.syncData.lastTouchLineIndex;
		m_bFootOnLine = ridingFrameData.unitCondition.bFootOnLine;
	}
	else
	{
		m_vPosition = frameData.syncData.position;
		m_bIsRight = frameData.syncData.bIsRight;
		m_iLastTouchLineIndex = frameData.syncData.lastTouchLineIndex;
		m_bFootOnLine = frameData.unitCondition.bFootOnLine;
	}


#else
	m_vPosition = frameData.syncData.position;
	m_bIsRight = frameData.syncData.bIsRight;
	m_iLastTouchLineIndex = frameData.syncData.lastTouchLineIndex;
	m_bFootOnLine = frameData.unitCondition.bFootOnLine;
#endif RIDING_MONSTER

	m_fNowHP = m_pMasterNPC->GetNowHp();
	m_fMaxHP = m_pMasterNPC->GetMaxHp();

	// fix!! 미리 한번만 계산해서 가지고 있어야, 아니면 대략 계산하거나
	// t = v0/g
	// l = v0*t - 1/2*g*t^2 = 1/2*v0^2/g
	const CX2GameUnit::PhysicParam& physicParam = m_pMasterNPC->GetPhysicParam();	

	float fHalfTimeOnAir;
	if( physicParam.fGAccel != 0.f )
		fHalfTimeOnAir = physicParam.GetJumpSpeed() / physicParam.fGAccel;	
	else
		fHalfTimeOnAir = 0.f;

	m_fMaxJumpUp = 0.5f * physicParam.GetJumpSpeed() * fHalfTimeOnAir;
	m_fMaxJumpRight = 2.f * physicParam.GetDashSpeed() * fHalfTimeOnAir - physicParam.fReloadAccel * fHalfTimeOnAir * fHalfTimeOnAir;

	if( 0.f != physicParam.GetDashSpeed() && m_fMaxJumpRight < LINE_RADIUS )
	{
		m_fMaxJumpRight = LINE_RADIUS;
		//ASSERT( !L"RUN_SPEED is too small" );
	}
}

/*virtual*/ void CX2GUNPCAI::Targeting( float fElapsedTime )
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;


	//FieldFix: 부동소수점은 == 연산 되도록 사용하지 말자
	// 그리고 밑의 Cloaking 부분과 연관해서는 뭔가 비효율적인듯
	if( m_fScanCloaking == 0.f && m_fScanCloakingNearRange == 0.f )
	{
		m_bScanCloaking = false;
	}

	/// 타겟팅 Interval 만큼의 시간이 지났는지 임시로 체크 해보고
	float fTime = m_fElapsedTimeAfterLastTargeting + fElapsedTime;

	// 지났으면
	if( fTime >= m_pAIData->targetData.targetInterval )
	{
		m_bScanCloaking = false;
		// 클로킹을 볼 수 있는 확률을 확인
		if( m_fScanCloaking > 0.f && 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            CX2GUNPC::EstimateFrameOneshotPercent( m_fScanCloaking ) == true
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            (float)RandomInt() <= m_fScanCloaking 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            )
		{
			m_bScanCloaking = true;
		}
	}

	// 자신을 공격한 유닛이 있으면 타겟팅
	if( m_optrAttackerGameUnit != null )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if ( m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->targetData.attackTargetRate ) == true )
		{
			SetTargetGameUnit( m_optrAttackerGameUnit.GetObservable() );
        }
		// 공격당했을 때 일정 확률로 도망가자~
		const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;

		if ( NULL != m_pMasterNPC && m_pMasterNPC->GetSiegeMode() )
		{
			ResetAttackerGameUnit();

			if ( null != m_optrTargetGameUnit )
			{
				if( fNowHPRate < m_pAIData->escapeCondition.myHP &&
					m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->escapeCondition.escapeRate ) == true &&
					true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue) )	// 09.04.16 태완
				{
					m_bFleeing = true;
				}

				return TargetUpdate( true );
			}
		}	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if ( RandomFloat( 0.f, 100.f ) < m_pAIData->targetData.attackTargetRate )
		{
			SetTargetGameUnit( m_optrAttackerGameUnit.GetObservable() );

			// 공격당했을 때 일정 확률로 도망가자~
			const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;

			if ( NULL != m_pMasterNPC && m_pMasterNPC->GetSiegeMode() )
			{
				ResetAttackerGameUnit();

				if ( null != m_optrTargetGameUnit )
				{
					if( fNowHPRate < m_pAIData->escapeCondition.myHP &&
						RandomFloat( 0.f, 100.f ) < m_pAIData->escapeCondition.escapeRate &&
						true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue) )	// 09.04.16 태완
					{
						m_bFleeing = true;
					}

					return TargetUpdate();
				}
			}	
		}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	// 도망가는 중인데
	if( true == m_bFleeing )
	{
		m_bFleeing = false;

		// 타겟이 없거나, 타겟이 있는데 도망 거리만큼 멀어졌다면
		if ( null == m_optrTargetGameUnit )
		{ 
			ResetTarget();
		}
 		else if( GetDistance3Sq( m_optrTargetGameUnit->GetPos(), m_vPosition ) > static_cast<float>( m_pAIData->escapeMoveData.escapeGap * m_pAIData->escapeMoveData.escapeGap ) )
 		{
			m_bFleeing = false;
 		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	
	//FieldFix: 무슨 투명화 처리가 이렇게 여러번 되어 있나...
	// 현재 타겟이 투명화 되었다면 타겟을 잃는다, 이브의 투명화 스킬에 대한 처리
	if( null != m_optrTargetGameUnit )
	{
		if ( m_optrTargetGameUnit->GetInvisibility() && false == m_bScanCloaking )
		{
			ResetTarget();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
		
#ifdef EVE_ELECTRA
		if( m_pMasterNPC->GetShowSubBossName() == false && m_pMasterNPC->GetShowBossName() == false && 
			m_pMasterNPC->GetExtraDamagePack().m_FlashBang.m_fTime > 0.0f )
		{
			ResetTarget();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
#endif EVE_ELECTRA

	}

	// 타게팅 할 때가 되었는지 알아보자~
	{
		m_fElapsedTimeAfterLastTargeting += fElapsedTime;

		// targetInterval 시간이 지나지 않았으면 이 중괄포 밑에 부분을 수행할 수 없음
		if( m_fElapsedTimeAfterLastTargeting < m_pAIData->targetData.targetInterval )
        {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        }
		m_fElapsedTimeAfterLastTargeting = 0.f;	// FieldFix: 이건 괄호 다음에 있던지 해야할 듯

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.targetSuccessRate ) == false )
			return TargetUpdate( false );	// 이부분을 보면 TargetUpdate가 Success를 의미하진 않는듯
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( RandomFloat( 0.f, 100.f ) >= m_pAIData->targetData.targetSuccessRate )
			return TargetUpdate();	// 이부분을 보면 TargetUpdate가 Success를 의미하진 않는듯
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	// 이전 타겟을 유지할까요?
	if( false == m_bLostTarget && null != m_optrTargetGameUnit )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if ( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.preserveLastTargetRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if ( RandomFloat( 0.f, 100.f ) < m_pAIData->targetData.preserveLastTargetRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			const float fDistance3Sq = GetDistance3Sq( m_optrTargetGameUnit->GetPos(), m_vPosition );
			if( fDistance3Sq < m_pAIData->targetData.targetLostRange * m_pAIData->targetData.targetLostRange 
				&& CX2GameUnit::GUSI_DIE == m_optrTargetGameUnit->GetGameUnitState() )
            {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            }
			else
				ResetTarget();
		}
	}

	// 자~ 본격적으로 새로운 타겟을 찾아 봅시다~
	
	// Escape 관련 변수
	CX2GameUnit*	pFleePickedGameUnit				= NULL;
	float			fFleePickedDistance3Sq			= 0.f;	

	// Targeting 관련 변수
	CX2GameUnit*	pTargetPickedGameUnit			= NULL;
	float			fTargetPickedDistance3Sq		= 0.f;
	
	bool			bFoundNearTargetGameUnit		= false;	// 무조건 타겟되는 거리내의 유저를 찾았는지

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_vecpTempGameUnit.resize( 0 );
    float                       fRate = 0.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	for( int i = 0; i<g_pX2Game->GetUserUnitListSize(); i++ )
	{
		CX2GameUnit* pGameUnit =  g_pX2Game->GetUserUnit(i);
		if( NULL == pGameUnit || CX2GameUnit::GUSI_DIE == pGameUnit->GetGameUnitState() )
			continue;
		
		if( true == pGameUnit->GetInvisibility() && m_bScanCloaking == false)
			continue;

		if( pGameUnit->GetNowHp() <= 0.f )
			continue;

		const float fDistance3Sq = GetDistance3Sq( pGameUnit->GetPos(), m_vPosition );

		// 도망 중인 상태가 아닌 경우
		if( false == m_bFleeing )
		{
			// 도망가야할 경우를 대비해 
			// escapeRange 안에 있는 유저 중 제일 가까운 유저를 일단 도망 대상으로 설정해 놓는다
			if( fDistance3Sq < m_pAIData->escapeCondition.escapeRange * m_pAIData->escapeCondition.escapeRange )
			{
				if( NULL == pFleePickedGameUnit || fDistance3Sq < fFleePickedDistance3Sq )
				{
					pFleePickedGameUnit		= pGameUnit;
					fFleePickedDistance3Sq	= fDistance3Sq;					
				}
			}
		}

		// targetRange 보다 멀면 타켓 하지 않음
		if( fDistance3Sq > m_pAIData->targetData.targetRange * m_pAIData->targetData.targetRange )
			continue;
		
		// 무조건 타겟이 되는 거리안에 있는 유닛이고
		if( fDistance3Sq < m_pAIData->targetData.targetNearRange * m_pAIData->targetData.targetNearRange )
		{
			// 기존의 타겟이 없거나, 기존 타겟 보다 가까우면
			if ( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPickedDistance3Sq )
			{
				pTargetPickedGameUnit		= pGameUnit;
				fTargetPickedDistance3Sq	= fDistance3Sq;
				bFoundNearTargetGameUnit	= true;	// NearRange 안쪽의 유닛을 찾았다
			}
		}
		

		if ( false == bFoundNearTargetGameUnit )
		{
			switch( m_pAIData->targetData.targetPriority )
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
					if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() > pTargetPickedGameUnit->GetNowHp() )
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
	} // end of for(i)
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

	// escape range 안에 그 유닛을 타겟으로 도망갈지 말지 결정한다

	// NPC가 시즈모드가 아니고 도망가야하는 조건을 만족 하는 경우
	// iPickedFleeTargetIndex에 설정해놓은 대상을 타겟으로 하여 도망 간다
	const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;
	
	if( false == m_pMasterNPC->GetSiegeMode() && 
		NULL != pTargetPickedGameUnit && 
		fNowHPRate <= m_pAIData->escapeCondition.myHP &&
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->escapeCondition.escapeRate ) == true &&
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		RandomFloat( 0.f, 100.f ) <= m_pAIData->escapeCondition.escapeRate &&
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue ) )		// 09.04.16 태완
	{
		m_optrTargetGameUnit = pFleePickedGameUnit;
	}
	else // 그냥 chase 타겟을 쫒아간다
	{
		m_optrTargetGameUnit = pTargetPickedGameUnit;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    return TargetUpdate( false );	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	return TargetUpdate();	
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}


void CX2GUNPCAI::TargetingNPC( float fElapsedTime )
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    // 이 아래 코드는 매 프레임이 아니라 adaptive 하게 실행될 수 있다. 따라서 올바른 누적 확률 계산을 위해서는
    // m_pMasterNPC->EstimateFrameAccumPercent 을 사용한다.
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	// 근처에 유저 유닛이 있으면 그 유닛을 타겟으로 flee
	// 유저 유닛이 없으면 근처에 있는 나소드 중에서 보스, 중간보스, 일반 순서로 피가 적은 애를 타겟으로 이동한다
	// 특정거리까지 가까워지면 힐링 시작~

	// 자신을 공격한 유닛을 타겟으로 flee
	if( m_optrAttackerGameUnit != null )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if ( m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->targetData.attackTargetRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( RandomFloat( 0.f, 100.f ) < m_pAIData->targetData.attackTargetRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_bFleeing = true;
			ResetAttackerGameUnit();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
	}
	
	// 도망가는 중이면 다 도망쳤는지 확인한다
	if( true == m_bFleeing && null != m_optrTargetGameUnit )
	{
		if( GetDistance( m_optrTargetGameUnit->GetPos(), m_vPosition ) > m_pAIData->escapeMoveData.escapeGap * m_pAIData->escapeMoveData.escapeGap )
		{
			ResetTarget();
			m_bFleeing		= false;

		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

	// 타게팅 할 때가 되었는지 알아보자~
	m_fElapsedTimeAfterLastTargeting += fElapsedTime;
	if( m_fElapsedTimeAfterLastTargeting < m_pAIData->targetData.targetInterval )
    {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return TargetUpdate( true );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    }
	m_fElapsedTimeAfterLastTargeting = 0.f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    // 이 아래 코드는 m_fElapsedTimeAfterLastTargeting 이 m_pAIData->targetData.targetInterval 을 지날 때마다
    // 1번씩 trigger 되어 실행된다. 따라서 random 계산할 때는 CX2GUNPC::EstimateFrameOneshotPercent 을 이용하도록 한다
    // <- 기존의 random 계산법과 동일 

    if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.targetSuccessRate ) == false )
        return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( RandomFloat( 0.f, 100.f ) >= m_pAIData->targetData.targetSuccessRate )
		return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


	// 이전 타겟을 유지할까요?
	if( false == m_bLostTarget && null != m_optrTargetGameUnit )
	{
		if ( CX2GameUnit::GUSI_DIE != m_optrTargetGameUnit->GetGameUnitState() )
		{
			// LostRange 거리 밖이면 타겟하던 것을 없앤다
			const float fDistance3Sq = GetDistance3Sq( m_optrTargetGameUnit->GetPos(), m_vPosition );
			if( fDistance3Sq <= m_pAIData->targetData.targetLostRange * m_pAIData->targetData.targetLostRange )
            {
			// 거리 내에 있으나 이전 타겟을 확률에 의해 유지 실패하면
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->targetData.preserveLastTargetRate ) == true )
                    return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if ( RandomFloat( 0.f, 100.f ) <= m_pAIData->targetData.preserveLastTargetRate )
                    return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

            }
		}
		ResetTarget();
	}

	// Escape 관련 변수
	CX2GameUnit*	pFleePickedGameUnit				= NULL;
	float			fFleePickedDistance3Sq			= 0.f;	

	// Targeting 관련 변수
	CX2GameUnit*	pTargetPickedGameUnit			= NULL;
	float			fTargetPicketDistance3Sq		= 0.f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_vecpTempGameUnit.resize( 0 );
    float                       fRate = 0.f;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	for( int i = 0; i < g_pX2Game->GetNPCUnitListSize(); i++ )
	{
		CX2GameUnit* pGameUnit =  g_pX2Game->GetNPCUnit( i );
		if( NULL == pGameUnit || CX2GameUnit::GUSI_DIE == pGameUnit->GetGameUnitState() )
			continue;

		if( true == pGameUnit->GetInvisibility() && m_bScanCloaking == false)
			continue;

		if( pGameUnit->GetNowHp() <= 0.f )
			continue;
		
		const float fDistance3Sq = GetDistance3Sq( pGameUnit->GetPos(), m_vPosition );

		// 특정 거리보다 가까우면 무조건 타겟팅
		if ( fDistance3Sq < m_pAIData->targetData.targetNearRange )
		{
			m_optrTargetGameUnit = pGameUnit;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_vecpTempGameUnit.resize( 0 );
            return TargetUpdate( false );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			return TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}

		// 도망 중인 상태가 아닌 경우
		if( false == m_bFleeing )
		{
			// 도망가야할 경우를 대비해 
			// escapeRange 안에 있는 유저 중 제일 가까운 유저를 일단 도망 대상으로 설정해 놓는다
			if( fDistance3Sq < m_pAIData->escapeCondition.escapeRange * m_pAIData->escapeCondition.escapeRange )
			{
				if( NULL == pFleePickedGameUnit || fDistance3Sq < fFleePickedDistance3Sq )
				{
					pFleePickedGameUnit		= pGameUnit;
					fFleePickedDistance3Sq	= fDistance3Sq;					
				}
			}
		}

		// targetRange 보다 멀면 타켓 하지 않음
		if( fDistance3Sq > m_pAIData->targetData.targetRange * m_pAIData->targetData.targetRange )
			continue;

		switch( m_pAIData->targetData.targetPriority )
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
				if( NULL == pTargetPickedGameUnit || pGameUnit->GetNowHp() > pTargetPickedGameUnit->GetNowHp() )
					pTargetPickedGameUnit = pGameUnit;
			} break;

		case TP_NEAR_FIRST:
			{
				if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPicketDistance3Sq )
				{
					fTargetPicketDistance3Sq	= fDistance3Sq;
					pTargetPickedGameUnit		= pGameUnit;
				}
			} break;

		case TP_FAR_FIRST:
			{
				if( NULL == pTargetPickedGameUnit || fDistance3Sq < fTargetPicketDistance3Sq )
				{
					fTargetPicketDistance3Sq	= fDistance3Sq;
					pTargetPickedGameUnit		= pGameUnit;
				}
			} break;

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
		default:
			ASSERT( !L"Wrong Path!" );
			break;
		}
	} // end of for(i)
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


	// NPC가 시즈모드가 아니고 도망가야하는 조건을 만족 하는 경우
	// iPickedFleeTargetIndex에 설정해놓은 대상을 타겟으로 하여 도망 간다
	const float fNowHPRate = 100.f * m_fNowHP / m_fMaxHP;

	if( false == m_pMasterNPC->GetSiegeMode() && 
		NULL != pTargetPickedGameUnit && 
		fNowHPRate <= m_pAIData->escapeCondition.myHP &&
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->escapeCondition.escapeRate ) == true &&
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		RandomFloat( 0.f, 100.f ) <= m_pAIData->escapeCondition.escapeRate &&
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		true == EscapeFlagCheck( m_pAIData->escapeCondition.m_iFlagTrue ) )		// 09.04.16 태완
	{
		m_optrTargetGameUnit = pFleePickedGameUnit;
	}
	else // 그냥 chase 타겟을 쫒아간다
	{
		m_optrTargetGameUnit = pTargetPickedGameUnit;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    return TargetUpdate( false );	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	return TargetUpdate();	
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
}



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GUNPCAI::TargetUpdate( bool bAccumulate_ )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GUNPCAI::TargetUpdate()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	KTDXPROFILE();	

#ifdef RIDING_MONSTER
	// 다른 NPC를 타야하는 몬스터라면
	// FieldFix: 이럴꺼면 처음부터 그냥 없애지.. 왜 Targeting을 수행 했나...
	if(m_bRideToUnit)
	{
		// 타켓을 없앰
		ResetTarget();
	}

#endif RIDING_MONSTER

	CKTDGLineMap* pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	if( NULL == pLineMap )
		return;

	//{{oasis:김상윤////2009-10-7////
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_FORCED_MOVE &&
		m_pMasterNPC->IsGroupAIGroundTarget() ==  true ) // 지면 타겟
	{
		m_vFinalDest = m_pMasterNPC->GetGroupAITargetPos();
		pLineMap->GetLandPosition(m_vFinalDest, 30.0f, &m_iFinalDestLineIndex);
		return;
	}
	//}}oasis:김상윤////2009-10-7////

	// 타겟이 없다면
	if( null == m_optrTargetGameUnit )
	{
		m_bFleeing = false; // target을 잃으면 도망가는 것도 멈춰야 한다

		// 특정 유닛한테 가야한다면
		if( null != m_optrRallyGameUnit )
		{
			D3DXVECTOR3 vRallyUnitPos( m_optrRallyGameUnit->GetPos() );
			int iRallyLineIndex = m_optrRallyGameUnit->GetLastTouchLineIndex();

			// 거리차이 계산
			float fDistance3Sq = GetDistance3Sq( vRallyUnitPos, m_vPosition );

			// 거리 차이가 CHASE구조체에 정한 DEST_GAP 이하 일때
			if( fDistance3Sq < m_pAIData->chaseMoveData.destGap * m_pAIData->chaseMoveData.destGap )
			{
#ifdef RIDING_MONSTER
				// 현재 자신이 무엇인가를 탈 수 있는 유닛이라면
				if ( m_bRideToUnit )
				{
					//탈때까지 Rally 지속
					if ( null == m_optrRallyGameUnit )
						StopRideToUnit();
				}
				else	// Ride 할 수 있는 유닛이 아니면 목적지에 도착 했으므로
				{
#endif RIDING_MONSTER
					m_optrRallyGameUnit.Reset();

					// 다음 frame에 바로 다시 targeting 할 수 있도록 한다
					m_fElapsedTimeAfterLastTargeting = m_pAIData->targetData.targetInterval + 1.f;
#ifdef RIDING_MONSTER
				}
#endif RIDING_MONSTER
			}			
			else	// 거리차이가 DEST_GAP 이상이면 최종 목적 위치를 업데이트함
			{
				m_iFinalDestLineIndex	= iRallyLineIndex;
				m_vFinalDest			= vRallyUnitPos;					
				return;
			}
		}


		// FieldFix: m_iTargetUnitUID이기 때문에 m_bLostTarget을 여기서 true 시켜주는군...
		// 차라리 lostTarget 함수를 만들어서 그 안에서 m_bLostTarget과 m_iTargetUnitUID 처리를 둘다 해주는게 좋을듯
		if( false == m_bLostTarget )	// 여기 까지 왔다면 타겟을 잃은것?
		{		
			m_bLostTarget = true;
			m_vPivotPoint = m_vPosition;	// 현재 자리를 기점으로 패트롤?
			m_iPivotPointLineIndex = m_iLastTouchLineIndex;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if( bAccumulate_ == true && m_pMasterNPC->EstimateFrameAccumPercent( m_pAIData->patrolMoveData.patrolBeginRate ) == true
                || bAccumulate_ == false && CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->patrolMoveData.patrolBeginRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( RandomFloat( 0.f, 100.f ) < m_pAIData->patrolMoveData.patrolBeginRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				SetNowPatroling( true );
			else
				SetNowPatroling( false );
		}
	}
	else	// 타겟이 있으면
	{
		// 타겟이 있는데 찾아가야할 유닛이 있다면
		if( null != m_optrRallyGameUnit )
		{
			//dbg::clog << L"랠리 끝 - 새 유저 타겟" << dbg::endl;

			// 찾아가지 않도록 함
			m_optrRallyGameUnit.Reset();
		}

		// FieldFix: 타겟이 있는데 타겟을 잃어...이런 경우는 코드상 없어보이고, 없어야 한다.
		if( true == m_bLostTarget )
		{
			m_bLostTarget = false;
			SetNowPatroling( false );
		}

		// 타겟이 있는 경우 최종 목적지를 타겟으로 변경.. (움직이지 않는 몬스터에게는 필요 없겠군..)
		m_iFinalDestLineIndex = m_optrTargetGameUnit->GetLastTouchLineIndex();
		m_vFinalDest = m_optrTargetGameUnit->GetPos();
	}
}

void CX2GUNPCAI::Moving( float fElapsedTime )
{
	KTDXPROFILE();	

	CX2GUNPC::STATE_CHANGE_TYPE message = CX2GUNPC::SCT_NO_CHANGE;
	CKTDGLineMap* pLineMap	= g_pX2Game->GetWorld()->GetLineMap();

	// 원래의 m_iFinalDestLineIndex와 m_vFinalDest를 그대로 가지고 있을 수 있겠군
	int iFinalDestLineIndex = m_iFinalDestLineIndex;
	D3DXVECTOR3 vFinalDest	= m_vFinalDest;
	bool bAIIsRight = false;


	bool bStayOnCurrLineGroup = false;
	if( GetNowPatroling() )	// 패트롤을 한다면
	{
		bStayOnCurrLineGroup = m_pAIData->patrolMoveData.bOnlyThisLineGroup;
	}
	else	// 패트롤을 안한다면
	{
		// 도망가는 것도 아니고, 특정 유닛의 위치로 가는 것도 아니면서, Chase는 현재 라인그룹만 가능 한데 (bOneyThisLineGroup의 기본값은 false)
		if( false == m_bFleeing && null == m_optrRallyGameUnit && 
			true == m_pAIData->chaseMoveData.bOnlyThisLineGroup )
		{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
			CKTDGLineMap::LineGroupID pLineGroupCurr = pLineMap->GetLineGroupIncludesLineData( m_iLastTouchLineIndex );
			CKTDGLineMap::LineGroupID pLineGroupDest = pLineMap->GetLineGroupIncludesLineData( m_iFinalDestLineIndex );
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			CKTDGLineMap::LineGroup* pLineGroupCurr = pLineMap->GetLineGroupIncludesLineData( pLineMap->GetLineData(m_iLastTouchLineIndex ) );
			CKTDGLineMap::LineGroup* pLineGroupDest = pLineMap->GetLineGroupIncludesLineData( pLineMap->GetLineData(m_iFinalDestLineIndex ) );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			// 현재 위치와 목적지의 위치가 서로 다른 라인 그룹이면
			if( pLineGroupCurr != pLineGroupDest )
			{
				// 타겟을 잃도록 하고 현재 위치를 기점으로 패트롤 시킨다.
				ResetTarget();
				m_bLostTarget = true;
				m_vPivotPoint = m_vPosition;
				m_iPivotPointLineIndex = m_iLastTouchLineIndex;
				SetNowPatroling( true );

				// 그리고는 AI_WAIT를 보냄
				message = CX2GUNPC::SCT_AI_WAIT;
				goto SEND_AI_MESSAGE;
			}
		}
	}

	// 특정 유닛에게 가야하는 것도 아닌데 타겟을 잃었으면
	if( null == m_optrRallyGameUnit && true == m_bLostTarget )
	{	
		// 그런데 패트롤 하는 것도 아니라면
		if( false == GetNowPatroling() )
		{
			// 위에서 iFinalDestLineIndex 와 vFinalDest 를 설정해 준 것을 현재 위치로 변경
			iFinalDestLineIndex = m_iLastTouchLineIndex;
			vFinalDest = m_vPosition;
		}
		else	// 패트롤을 한다면
		{			
			bool bOnEdge = false;

			// patrol중에 현재 linegroup에만 있어야 한다면 라인맵의 끝에 있는지 체크
			if( true == bStayOnCurrLineGroup )
			{
				const CKTDGLineMap::LineData* pLineDataTemp = pLineMap->GetLineData( m_iLastTouchLineIndex );
				if( NULL != pLineDataTemp )
				{
					// 오른쪽을 바라보고 있으면 라인의 끝을 체크
					if( true == m_bIsRight &&
						( -1 == pLineDataTemp->nextLine ||
						-2 == pLineDataTemp->nextLine ) )
					{
						float fDist = GetDistance( m_vPosition, pLineDataTemp->endPos );
						if( fDist < LINE_RADIUS )
						{
							bOnEdge = true;
						}
					}
					// 왼쪽을 바라보고 있으면 라인의 시작을 체크
					else if( false == m_bIsRight &&
						( -1 == pLineDataTemp->beforeLine ||
						-2 == pLineDataTemp->beforeLine ) )
					{
						float fDist = GetDistance( m_vPosition, pLineDataTemp->startPos );
						if( fDist < LINE_RADIUS )
						{
							bOnEdge = true;
						}
					}
				}
			}

			// 패트롤 중, 현재 위치와 기점의 위치가 패트롤 가능 거리 이상으로 떨어져 있거나, 현재 라인그룹에만 있어야 하는데 현재 위치가 라인 그룹의 가장자리 이면
			if( GetDistance(m_vPivotPoint, m_vPosition) > m_pAIData->patrolMoveData.patrolRange ||
				true == bOnEdge )
			{
				m_fElapsedPatrolCoolingTime += fElapsedTime;
				if( m_fElapsedPatrolCoolingTime > m_pAIData->patrolMoveData.patrolCoolingTime )	/// 쿨타임이 지났으면
				{		
					// 왜 여기서는 m_fElapsedPatrolCoolingTime를 초기화 시키지 않지?
					if( true == m_bIsRight )	// 오른쪽을 보고 있으면 왼쪽으로~
					{
						iFinalDestLineIndex = -1;
						vFinalDest = m_vPivotPoint + D3DXVECTOR3(-10000,0,0);
					}
					else	// 왼쪽을 보고 있으면 오른쪽으로~
					{
						iFinalDestLineIndex = -1;
						vFinalDest = m_vPivotPoint + D3DXVECTOR3(10000,0,0);
					}
				}
				else	// cool 타임이 지나지 않았으면
				{
					// 현재 위치를 목적지로 함
					iFinalDestLineIndex = m_iLastTouchLineIndex;
					vFinalDest = m_vPosition;
				}				
			}
			else
			{
				m_fElapsedPatrolCoolingTime = 0.f;
				if( true == m_bIsRight )
				{
					iFinalDestLineIndex = -1;
					vFinalDest = m_vPivotPoint + D3DXVECTOR3(10000,0,0);
				}
				else
				{
					iFinalDestLineIndex = -1;
					vFinalDest = m_vPivotPoint + D3DXVECTOR3(-10000,0,0);
				}
			}
		}
	}

	//{{oasis:김상윤////2009-10-7////
	// 그룹AI를 사용하지 않고
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_OFF)
	{
		//}}oasis:김상윤////2009-10-7////
		if( false == bStayOnCurrLineGroup )	// 현재 라인그룹만 패트롤하는게 아니라면
		{
			if( true == m_bFleeing ) // 도망가는 중이면
			{
				// ESCAPE_MOVE의 JUMP_INTERVAL 체크
				m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
				if( m_fElapsedTimeAfterLastRandomJump > m_pAIData->escapeMoveData.jumpInterval )
				{
					m_fElapsedTimeAfterLastRandomJump = 0.f;
					const float fJumpChance = RandomFloat( 0.f, 100.f );
					
					// 목적지의 위치가 현재 위치보다 위에 있으면
					if( vFinalDest.y > m_vPosition.y )
					{
						// 도망갈때의 UP_JUMP_RATE 체크
						if( fJumpChance < m_pAIData->escapeMoveData.upJumpRate )
						{	
							message = CX2GUNPC::SCT_AI_JUMP;
							goto SEND_AI_MESSAGE;
						}
						else if( 100.f-fJumpChance < m_pAIData->escapeMoveData.upDownRate ) // 도망 갈때의 UP_DOWN_RATE 체크
						{
							message = CX2GUNPC::SCT_AI_DOWN;
							goto SEND_AI_MESSAGE;
						}
					}
					else	// 목적지의 위치가 현재 위치보다 아래에 있으면
					{
						// 도망 갈때의 DOWN_JUMP_RATE 체크
						if( fJumpChance < m_pAIData->escapeMoveData.downJumpRate )
						{
							message = CX2GUNPC::SCT_AI_JUMP;
							goto SEND_AI_MESSAGE;
						}
						else if( 100.f-fJumpChance < m_pAIData->escapeMoveData.downDownRate )	 // 도망 갈때의 DOWN_DOWN_RATE 체크
						{
							message = CX2GUNPC::SCT_AI_DOWN;
							goto SEND_AI_MESSAGE;
						}
					}
				}
			}
			else // chase 중이면
			{
#ifdef AI_FLY
				if(GetAIFlyNowState() == false)	// Fly 상태가 아닌 경우에만 실행
#endif
				{
					// FieldFix: 이m_fElapsedTimeAfterLastRandomJump 도 if(!m_bRideToUnit)에 포함되어야 할 듯
					m_fElapsedTimeAfterLastRandomJump += fElapsedTime;
#ifdef RIDING_MONSTER
					if( !m_bRideToUnit)		/// 다른 유닛을 Ride하는 유닛이 아닌경우 에만 
					{
#endif RIDING_MONSTER
						// FieldFix: 위의 Flee 처리와 동일 하므로, 이부분은 템플릿 또는 함수등으로 만드는게 좋을듯...
						// CHASE_MOVE의 JUMP_INTERVAL 체크
						if( m_fElapsedTimeAfterLastRandomJump > m_pAIData->chaseMoveData.jumpInterval )
						{
							m_fElapsedTimeAfterLastRandomJump = 0.f;
							const float fJumpChance = RandomFloat( 0.f, 100.f );

							if( vFinalDest.y > m_vPosition.y )
							{
								if( fJumpChance < m_pAIData->chaseMoveData.upJumpRate )
								{	
									message = CX2GUNPC::SCT_AI_JUMP;
									goto SEND_AI_MESSAGE;
								}
								else if( 100-fJumpChance < m_pAIData->chaseMoveData.upDownRate )
								{
									message = CX2GUNPC::SCT_AI_DOWN;
									goto SEND_AI_MESSAGE;
								}
							}
							else
							{
								if( fJumpChance < m_pAIData->chaseMoveData.downJumpRate )
								{
									message = CX2GUNPC::SCT_AI_JUMP;
									goto SEND_AI_MESSAGE;
								}
								else if( 100-fJumpChance < m_pAIData->chaseMoveData.downDownRate )
								{
									message = CX2GUNPC::SCT_AI_DOWN;
									goto SEND_AI_MESSAGE;
								}
							}
						}
#ifdef RIDING_MONSTER
					}
#endif RIDING_MONSTER
				}			
			}
		}
		//{{oasis:김상윤////2009-10-7////
	}
	//}}oasis:김상윤////2009-10-7////

	// 자 타게팅된 목적지로 이동을 해보자~
	float fDestGap = 0.f;
	//{{oasis:김상윤////2009-10-7////
	float fMoveGap = 0.f;

	// 그룹AI의 GAI_CMD_FORCED_MOVE를 수행 중이면
	if(m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_FORCED_MOVE)
	{
		fMoveGap = 15.f;
		fDestGap = 15.f;
	}
#ifdef RIDING_MONSTER
	else if(m_bRideToUnit == true)	// 다른 유닛을 Ride하는 유닛이면
	{
		fMoveGap = 15.f;
		fDestGap = 15.f;
	}
#endif RIDING_MONSTER
	//}}oasis:김상윤////2009-10-7////
	else	/// 일반 유닛이면
	{
		fDestGap = (float)m_pAIData->chaseMoveData.destGap;	// 스크립트의 DEST_GAP
		//{{oasis:김상윤////2009-10-7////
		fMoveGap = (float)m_pAIData->chaseMoveData.moveGap;	// 스크립트의 MOVE_GAP
		//}}oasis:김상윤////2009-10-7////
	}
	bool bIsTargetOnRight;
	//{{oasis:김상윤////2009-10-7////
	// 도망중이 아니거나, 그룹AI를 실행 중인 경우
	if( false == m_bFleeing || m_pMasterNPC->GetGroupAICommand_LUA() != CX2GUNPC::GAI_CMD_OFF)
	{
		// 원본: if( false == m_bFleeing )
		//}}oasis:김상윤////2009-10-7////
#ifdef AI_FLY
		if( GetAIFlyNowState() == true )	// Fly 상태면
		{
			D3DXVECTOR3 vDistPos = vFinalDest;

			if( m_bLostTarget == true )	// 타겟을 잃은 경우
			{
				// vFinalDest 보다 DEST_HEIGHT_GAP 만큼 위에 있도록 한다.
				vDistPos.y -= m_pAIData->flymovedata.fDestHeight;	 
			}
			
			m_PathFinder.FollowTargetPathFly( /*IN fElapsedTime,*/ m_vPosition, IN m_iLastTouchLineIndex, IN vDistPos, IN iFinalDestLineIndex,
				IN m_fMaxJumpUp * 2.f, IN m_fMaxJumpRight,
				IN (const float)m_pAIData->flymovedata.fDestHeight, IN (const float)m_pAIData->flymovedata.fDestArea,
				pLineMap, IN (const float)m_pAIData->flymovedata.fDestGap, /*IN (const float)m_pAIData->chaseMoveData.moveGap,*/
				/*IN m_bFootOnLine,*/ /*IN bStayOnCurrLineGroup,*/ OUT bIsTargetOnRight, IN m_pAIData->chaseMoveData.m_fLineEndDetectRange );
		}
		else	// FLY 상태가 아니면
#endif
		{
			m_PathFinder.FollowTargetPath( /*fElapsedTime,*/ m_vPosition, m_iLastTouchLineIndex, vFinalDest, iFinalDestLineIndex,
				m_fMaxJumpUp, m_fMaxJumpRight, pLineMap, (const float)fDestGap, (const float)fMoveGap,
				m_bFootOnLine, bStayOnCurrLineGroup, OUT bIsTargetOnRight, m_pAIData->chaseMoveData.m_fLineEndDetectRange );
		}		
	}
	else	// 그룹 AI를 수행하지 않으면서 도망 중이면
	{
		// FLY는 처리 없나?
		// oasis  fDestGap -> 150.f  원본: (const float)fDestGap,
		m_PathFinder.EscapeTargetPath( /*fElapsedTime,*/ m_vPosition, m_iLastTouchLineIndex, vFinalDest, iFinalDestLineIndex,
			m_fMaxJumpUp, m_fMaxJumpRight, pLineMap, (const float)150.f, /*(const float)m_pAIData->chaseMoveData.moveGap,*/
			m_bFootOnLine, /*bStayOnCurrLineGroup,*/ OUT bIsTargetOnRight, m_bIsRight, m_pAIData->chaseMoveData.m_fLineEndDetectRange );
	}

	// 타겟을 보고 있는지 아닌지 판단은.. 몬스터의 현재스테이트의 VIEW_TARGET이 true인 경우에만 구하면 될듯한데...
	if( m_bIsRight != bIsTargetOnRight )
	{
		m_bIsTargetFront = false;
	}
	else
	{
		m_bIsTargetFront = true;
	}


	bAIIsRight = m_bIsRight;
	const float fFinalDestDist = GetDistance( m_vPosition, vFinalDest );

	switch( m_PathFinder.GetMoveState() )
	{
	case CX2SimplePathFinder::MS_WAIT:	
		{
			message = CX2GUNPC::SCT_AI_WAIT;
		} break;

		/// 왼쪽으로 가라고 할때
	case CX2SimplePathFinder::MS_FORCE_LEFT:	
	case CX2SimplePathFinder::MS_LEFT:
		{
			bAIIsRight = false;

			if( true == m_bIsRight )	// 현재 오른쪽을 보고 있고
			{
				// 이전 AI 메시지가 wait, walk, dash 중 하나였다면
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{	
					// CHASE_MOVE의 DIR_CHANGE_INTERVAL 체크
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval ) 
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
				}
			}

			// 왼쪽을 보고 있었다거나, 오른쪽을 보고 있었더라도 wait, walk, dash가 아니었다면 walk 하라는 AI를 수행
			message = CX2GUNPC::SCT_AI_WALK;
		} break;

	case CX2SimplePathFinder::MS_FORCE_RIGHT:
	case CX2SimplePathFinder::MS_RIGHT: 
		{
			bAIIsRight = true;

			if( false == m_bIsRight )
			{
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage )
				{
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;					
						break;
					}
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
			bAIIsRight = false;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_JUMP_RIGHT:
		{
			bAIIsRight = true;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_LEFT:
		{
			bAIIsRight = false;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
		} break;

	case CX2SimplePathFinder::MS_DOWN_RIGHT:
		{
			bAIIsRight = true;
			message	= CX2GUNPC::SCT_AI_JUMP_DIR;
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
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_DOWN	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_DOWN	== m_PrevAIMessage )
				{		
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;		// 방향 전환			
						break;
					}
				}
			}

			switch( m_PathFinder.GetMoveState() )
			{
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT:
				message = CX2GUNPC::SCT_AI_FLY_FRONT;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_UP:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_FRONT_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_DOWN;
				break;

			case CX2SimplePathFinder::MS_FLY_LEFT_BACK:
				message = CX2GUNPC::SCT_AI_FLY_BACK;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_BACK_UP:
				message = CX2GUNPC::SCT_AI_FLY_BACK_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_LEFT_BACK_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_BACK_DOWN;
				break;
			default:
				message = CX2GUNPC::SCT_NO_CHANGE;
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
				if( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_FRONT_DOWN	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_UP	== m_PrevAIMessage ||
					CX2GUNPC::SCT_AI_FLY_BACK_DOWN	== m_PrevAIMessage )
				{		
					m_fElapsedTimeAfterLastDirChange += fElapsedTime;
					if( m_fElapsedTimeAfterLastDirChange > m_pAIData->chaseMoveData.dirChangeInterval )
					{
						m_fElapsedTimeAfterLastDirChange = 0.f;
						message = CX2GUNPC::SCT_AI_DIR_CHANGE;		// 방향전환			
						break;
					}
				}
			}

			switch( m_PathFinder.GetMoveState() )
			{
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT:
				message = CX2GUNPC::SCT_AI_FLY_FRONT;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_UP:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_FRONT_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_FRONT_DOWN;
				break;

			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK:
				message = CX2GUNPC::SCT_AI_FLY_BACK;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_UP:
				message = CX2GUNPC::SCT_AI_FLY_BACK_UP;
				break;
			case CX2SimplePathFinder::MS_FLY_RIGHT_BACK_DOWN:
				message = CX2GUNPC::SCT_AI_FLY_BACK_DOWN;
				break;
			default:
				message = CX2GUNPC::SCT_NO_CHANGE;
			}
		}
		break;
	case CX2SimplePathFinder::MS_FLY_UP:
		{
			message = CX2GUNPC::SCT_AI_FLY_UP;
		}
		break;
	case CX2SimplePathFinder::MS_FLY_DOWN:
		{
			message = CX2GUNPC::SCT_AI_FLY_DOWN;
		}
		break;
#endif
	} // end of switch

	// 이전의 AI가 이동이거나 WAIT 였으면
#ifdef AI_FLY	
	if( ( m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_FRONT || m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_BACK ||
		m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_FRONT_UP || m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_BACK_UP || 
		m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_FRONT_DOWN || m_PrevAIMessage == CX2GUNPC::SCT_AI_FLY_BACK_DOWN || m_PrevAIMessage == CX2GUNPC::SCT_AI_WAIT ) &&
		( message == CX2GUNPC::SCT_AI_FLY_FRONT || message == CX2GUNPC::SCT_AI_FLY_BACK ||
		message == CX2GUNPC::SCT_AI_FLY_FRONT_UP || message == CX2GUNPC::SCT_AI_FLY_BACK_UP ||
		message == CX2GUNPC::SCT_AI_FLY_FRONT_DOWN || message == CX2GUNPC::SCT_AI_FLY_BACK_DOWN ) )
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


	// 그룹AI의 HOLD 인 경우
	//{{oasis:김상윤////2009-10-7////
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_HOLD) // COMMAND == 1. HOLD
	{
		// 방향 전환은 처리
		if(message == CX2GUNPC::SCT_AI_DIR_CHANGE)
		{
			;
		}
		else	// 그 외에는 대기
		{
			message = CX2GUNPC::SCT_AI_WAIT;
		}
		goto SEND_AI_MESSAGE;
	}

	// 그룹AI의 FORCED_MOVE 인 경우 
	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_FORCED_MOVE) // COMMAND == 3. FORCED MOVE
	{	
		if(true == m_pMasterNPC->GetGroupAISelfCommandTransit_LUA())
		{
			message = CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT;
			m_pMasterNPC->PushUnUseAIMessage(CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT);
			m_pMasterNPC->SetGroupAISelfCommandTransit_LUA(false);
		}
		else if( message == CX2GUNPC::SCT_AI_WAIT)
		{
			m_pMasterNPC->SetGroupAISelfCommandTransit_LUA(true);
		}
		else if( message == CX2GUNPC::SCT_AI_WALK)	// AI가 WALK인 경우 DASH로..
		{
			message = CX2GUNPC::SCT_AI_DASH;
		}

		// FieldFix: else 처리는 없네..?

		goto SEND_AI_MESSAGE;
	}

	if( m_pMasterNPC->GetGroupAICommand_LUA() == CX2GUNPC::GAI_CMD_SKILL_A) // COMMAND == 4. SKILL A
	{	
		if(true == m_pMasterNPC->GetGroupAISelfCommandTransit_LUA())
		{
			message = CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT;
			m_pMasterNPC->PushUnUseAIMessage(CX2GUNPC::SCT_AI_SELF_COMMAND_TRANSIT);
			m_pMasterNPC->SetGroupAISelfCommandTransit_LUA(false);
		}
		goto SEND_AI_MESSAGE;
	}
	//}}oasis:김상윤////2009-10-7////


	// FieldFix: 그룹AI의 경우 여기까지 오지도 않겠군
	// walk, dash 중에 walk, dash로의 상태변화는 CHASE_MOVE의 WALK_INTERVAL 간격으로 갱신
	if( ( CX2GUNPC::SCT_AI_WAIT	== m_PrevAIMessage ||
		CX2GUNPC::SCT_AI_WALK	== m_PrevAIMessage ||
		CX2GUNPC::SCT_AI_DASH	== m_PrevAIMessage ) &&
		( CX2GUNPC::SCT_AI_WALK == message || 
		CX2GUNPC::SCT_AI_DASH == message ) )
	{
		m_fElapsedTimeAfterLastWalkRunUpdate += fElapsedTime;

		/// walkInterval 만큼 경과 됐으면
		if( m_fElapsedTimeAfterLastWalkRunUpdate > (float)m_pAIData->chaseMoveData.walkInterval )
		{	
			m_fElapsedTimeAfterLastWalkRunUpdate = 0.f;

			// 현재위치와 목적한 위치 까지의 거리가 MOVE_SPLIT_RANGE 보다 작으면
			if( fFinalDestDist < m_pAIData->chaseMoveData.moveSplitRange )
			{
				// NEAR_WALK_RATE 확률에 따라서
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->chaseMoveData.nearWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < m_pAIData->chaseMoveData.nearWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					message = CX2GUNPC::SCT_AI_WALK;	// 걷던지..
				}
				else
				{
					message = CX2GUNPC::SCT_AI_DASH;	// 뛰던지..
				}
			}
			else	// MOVE_SPLIT_RANGE 보다 작지 않으면
			{
				// FAR_WALK_RATE 확률에 따라서
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                if( CX2GUNPC::EstimateFrameOneshotPercent( m_pAIData->chaseMoveData.farWalkRate ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				if( RandomFloat( 0.f, 100.f ) < m_pAIData->chaseMoveData.farWalkRate )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					message = CX2GUNPC::SCT_AI_WALK;	// 걷던지..
				}
				else
				{
					message = CX2GUNPC::SCT_AI_DASH;	// 뛰던지..
				}
			}		
		}
		else
		{
			message = m_PrevAIMessage;	// 이전의 AI를 그대로 사용
		}
	}

SEND_AI_MESSAGE:

#ifdef AI_FLY	
	// 혼자 안드로메다로 가는 일 없도록 강제 고도조절한다.
	if( GetAIFlyNowState() == true )	// 날고있는 경우
	{
		float fHeightHigh = -9999.f;		// 살아있는 유저들과의 Y포지션 차이중 가장 큰 것
		float fHeightLow = 9999.f;		// 살아있는 유저들과의 Y포지션 차이중 가장 작은것
		for(int i=0; i<g_pX2Game->GetUserUnitListSize(); ++i)
		{			
			CX2GUUser *pUser = g_pX2Game->GetUserUnit(i);
			if(pUser != NULL && pUser->GetNowHp() > 0.f )
			{
				float fHeightDist = m_vPosition.y - pUser->GetPos().y;

				if(fHeightDist > fHeightHigh)
					fHeightHigh = fHeightDist;
				if(fHeightDist < fHeightLow)
					fHeightLow = fHeightDist;
			}
		}

		/// 고도 조절
		if(fHeightHigh >= 1500.f)
			message = CX2GUNPC::SCT_AI_FLY_DOWN;
		else if(fHeightLow <= -1500.f)
			message = CX2GUNPC::SCT_AI_FLY_UP;

		// 물라인맵에서 움직이는 물고기의 경우도 fly로 처리하는데...
		// FLY_CHASE_MOVE 의 UNDERWATER_MODE 가 TRUE 면
		if( m_pAIData != NULL && m_pAIData->flymovedata.bUnderWaterMode == true && m_pMasterNPC != NULL )
		{
			int iLastLine = m_pMasterNPC->GetLastTouchLineIndex();
			CKTDGLineMap* pLineMap	= g_pX2Game->GetWorld()->GetLineMap();
			const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(iLastLine);
			if( pLineData != NULL && pLineData->m_bUnderWater == true )
			{
				// 물라인맵의 높이를 넘지 않도록 조절
				if( message == CX2GUNPC::SCT_AI_FLY_UP || message == CX2GUNPC::SCT_AI_FLY_FRONT_UP || message == CX2GUNPC::SCT_AI_FLY_BACK_UP )
				{
					D3DXVECTOR3 vNpcPos = m_pMasterNPC->GetPos();
					if( vNpcPos.y >= pLineData->m_fWaterHeight - m_pMasterNPC->GetUnitHeight(true) )
						message = CX2GUNPC::SCT_AI_WAIT;
				}				
			}
			else
			{
				message = CX2GUNPC::SCT_AI_WAIT;
			}
		}
	}	
#endif

	// 이전 AI 메시지와 지금 AI 메시지와 다른게 없다면 SCT_NO_CHANGE
	if( m_PrevAIMessage == message || CX2GUNPC::SCT_NO_CHANGE == message )
	{
		message = CX2GUNPC::SCT_NO_CHANGE;
	}
	else	// 이전 AI와 다르다면 (방향전환의 경우 SCT_AI_DIR_CHANGE 가 수행 된다.)
	{
		m_PrevAIMessage = message;
		m_pMasterNPC->PushAIMessage( message, bAIIsRight );
	}
}

#ifdef RIDING_MONSTER
void CX2GUNPCAI::StopRideToUnit()
{
	m_bRideToUnit = false;
	m_optrRallyGameUnit.Reset();

	// 다음 frame에 바로 다시 targeting 할 수 있도록 한다
	m_fElapsedTimeAfterLastTargeting = m_pAIData->targetData.targetInterval + 1.f;

	ResetTarget();
	m_bFleeing = false;
	SetNowPatroling( false );
	m_bLostTarget = false;
}
void CX2GUNPCAI::SetRideToUnit( CX2GameUnit* pRallyGameUnit_ )
{
	m_bRideToUnit = true;		// 다른 유닛을 ride
	SetRallyToUnit( pRallyGameUnit_ );
}
#endif RIDING_MONSTER


void CX2GUNPCAI::SetRallyToUnit( CX2GameUnit* pRallyGameUnit_ )
{
	m_optrRallyGameUnit = pRallyGameUnit_;

	ResetTarget();
	m_bFleeing = false;
	SetNowPatroling( false );
	m_bLostTarget = false;
}


//{{ 09.04.16 태완
bool CX2GUNPCAI::EscapeFlagCheck( int iFlagIndex )
{
	// true면 도망가는 Flag 조건을 만족한다.
	// flag 조건이 지정되지 않은 경우에는 무조건 true어야 한다 (기존 코드와 충돌이 없도록)

	// 0~9 사이가 아니면 조건이 존재하지 않거나 정상적인 index가 아니므로 도망 가능하게 하자
	if( iFlagIndex < 0 || 9 < iFlagIndex )
		return true;
	else
	{
		if( true == m_pMasterNPC->GetFlag_LUA( iFlagIndex ) )
			return true;
	}

	return false;

}
//}}

//{{ oasis907 : 김상윤 [2009.12.9] //
void CX2GUNPCAI::SetAItoMainAIData()
{ 
	m_pAIData = m_pMainAIData; 
	m_bFleeing = false;
}
void CX2GUNPCAI::SetAItoSubAIData() 
{ 
	m_pAIData = m_pSubAIData; 
	//m_bFleeing = false;
}
//}} oasis907 : 김상윤 [2009.12.9] //
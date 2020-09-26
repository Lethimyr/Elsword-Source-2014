#include "StdAfx.h"
#include ".\x2npcai.h"


CX2NPCAI::CX2NPCAI( CX2GUNPC* pOwnerNPC, NPC_AI_TYPE eAIType )
: m_bNoBrain( false )
, m_bEnabled( true ) 
, m_eAIType( eAIType )
, m_pMasterNPC( pOwnerNPC )
//, m_TargetUnitUID( -1 )
//, m_TargetUnitType( CX2GameUnit::GUT_USER )
, m_fElapsedTimeAfterLastTargeting( 0.f )
, m_fElapsedTimeAfterLastRandomJump( 0.f )
, m_fElapsedTimeAfterLastWalkRunUpdate( 0.f )
, m_fElapsedTimeAfterLastDirChange( 0.f )
, m_fElapsedPatrolCoolingTime( 0.f )
// , m_fIncreaseMoveSpeedPercentAtComeBackState( 0.0f )
// , m_fIncreaseAnimSpeedPercentAtComeBackState( 0.0f )
#ifdef AI_FLY
, m_fElapsedTimeAfterLastFlyMoveUpdate( 0.f )
, m_bFlyUnit( false )
, m_bNowStateFly( false )
#endif
, m_PrevAIMessage( CX2GUNPC::SCT_NO_CHANGE )
, m_bFleeing( false )
, m_bIsTargetFront( false )
, m_optrAttackerGameUnit( CX2GameUnitoPtr() )
{
}

CX2NPCAI::~CX2NPCAI(void)
{
}

/*virtual*/ void CX2NPCAI::LoadAIDataFromLUA( KLuaManager& luaManager )
{
	if( luaManager.BeginTable( "INIT_AI" ) == true )
	{
		LUA_GET_VALUE( luaManager, "NO_BRAIN", m_bNoBrain, false );
		luaManager.EndTable();
	}

}


/*virtual*/ void CX2NPCAI::RenderStateParam( WCHAR** wDest_ )
{
	return;
}

/*virtual*/ void CX2NPCAI::RenderPatrolLineIndex( WCHAR** wDest_ )
{
	return;
}

/*virtual*/ void CX2NPCAI::RenderPlayLineIndex( WCHAR** wDest_ )
{
	return;
}

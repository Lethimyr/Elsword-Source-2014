#include "StdAfx.h"
#include ".\x2dungeonroom.h"

CX2DungeonRoom::CX2DungeonRoom()
{
	m_DifficultyLevel	= CX2Dungeon::DL_NORMAL;
	m_DungeonID			= SEnum::DI_NONE;

	SetIntrudeGame( false );

	m_fPlayTimeLimit = 0.f;

//#ifndef NOT_USE_DICE_ROLL
//	m_GetItemType = CX2DungeonRoom::DGIT_RANDOM;
//#endif //NOT_USE_DICE_ROLL

	m_bLvLimit = true;

#ifdef SERV_RANDOM_DAY_QUEST
	m_eDungeonMode = CX2Dungeon::DM_INVALID;
#endif
}

CX2DungeonRoom::~CX2DungeonRoom(void)
{
}

/*virtual*/ void CX2DungeonRoom::Set_KRoomInfo( const KRoomInfo& kRoomInfo_ )
{
	CX2Room::Set_KRoomInfo( kRoomInfo_ );

	m_DifficultyLevel	= (CX2Dungeon::DIFFICULTY_LEVEL)kRoomInfo_.m_DifficultyLevel;
	m_DungeonID			= (SEnum::DUNGEON_ID)kRoomInfo_.m_iDungeonID;
	m_fPlayTimeLimit	= kRoomInfo_.m_fPlayTime;

//#ifndef NOT_USE_DICE_ROLL
//	m_GetItemType		= (CX2DungeonRoom::DUNGEON_GET_ITEM_TYPE)kRoomInfo_.m_cGetItemType;
//#endif //NOT_USE_DICE_ROLL
#ifdef SERV_RANDOM_DAY_QUEST
	m_eDungeonMode		= (CX2Dungeon::DUNGEON_MODE)kRoomInfo_.m_cDungeonMode;
#endif SERV_RANDOM_DAY_QUEST

	// 던전방 레벨제한
	//m_bLvLimit = kRoomInfo.m_bCharLevelLimit;

}

int	CX2DungeonRoom::GetClickSlot( int x, int y )
{
	D3DXVECTOR2 kConvertPos;
	kConvertPos.x = (float)x / g_pKTDXApp->GetResolutionScaleX();
	kConvertPos.y = (float)y / g_pKTDXApp->GetResolutionScaleY();
	D3DXVECTOR2 kConvertSize = g_pKTDXApp->ConvertByResolution( 50.0f, 50.0f );

	if( GetDistance2( kConvertPos.x, kConvertPos.y, 141.0f, 467.0f ) < kConvertSize.x )
	{
		return 0;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 375.0f, 467.0f ) < kConvertSize.x )
	{
		return 1;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 650.0f, 467.0f ) < kConvertSize.x )
	{
		return 2;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 884.0f, 467.0f ) < kConvertSize.x )
	{
		return 3;
	}

	return -1;
}


//{{ 허상형 : [2011/3/14/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
bool CX2DungeonRoom::IsDefenceDungeon( SEnum::DUNGEON_ID eDungeonID )
{
	switch( eDungeonID )
	{
	CASE_DEFENCE_DUNGEON
		return true;
	}

	return false;
}

bool CX2DungeonRoom::IsDefenceDungeon( int iDungeonID )
{
	switch( iDungeonID )
	{
	CASE_DEFENCE_DUNGEON
		return true;
	}

	return false;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 허상형 : [2011/3/14/] //	월드 미션


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef HEAP_BROKEN_BY_ROOM
/*virtual*/ void  CX2DungeonRoom::ApplyRoomPacketData()
{
    CX2Room::ApplyRoomPacketData();
    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_DUNGEON ) );
}
#endif // HEAP_BROKEN_BY_ROOM
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
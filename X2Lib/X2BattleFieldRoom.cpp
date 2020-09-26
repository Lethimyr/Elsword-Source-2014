#include "StdAfx.h"
#include ".\X2BattleFieldRoom.h"

// CX2BattleFieldRoom::CX2BattleFieldRoom( const KEGS_JOIN_BATTLE_FIELD_ACK& kEvent_ )
// {
// 
// }

/*virtual*/ CX2BattleFieldRoom::~CX2BattleFieldRoom(void)
{
}

/*virtual*/ void CX2BattleFieldRoom::Set_KRoomInfo( const KRoomInfo& kRoomInfo_ )
{
	CX2Room::Set_KRoomInfo( kRoomInfo_ );

	m_uiBattleFieldId			= kRoomInfo_.m_iBattleFieldID;
	m_eGetItemType		= (DUNGEON_GET_ITEM_TYPE)kRoomInfo_.m_cGetItemType;
}


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef HEAP_BROKEN_BY_ROOM
/*virtual*/ void CX2BattleFieldRoom::ApplyRoomPacketData()
{
    CX2Room::ApplyRoomPacketData();
    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_BATTLE_FIELD ) );
}
#endif // HEAP_BROKEN_BY_ROOM
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

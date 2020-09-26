#include "StdAfx.h"
#include ".\x2pvproom.h"

CX2PVPRoom::CX2PVPRoom()
{
	m_PVPGameType		= PGT_TEAM;

	m_WinKillNum	= 0;
	m_fPlayTime		= 0.0f;
	m_bCanIntrude	= true;
	m_WorldID		= CX2World::WI_NONE;	
	m_WorldIDForUI	= CX2World::WI_NONE;

#ifdef DUNGEON_ITEM
	m_bItemMode		= true;
#endif
#ifdef SERV_PVP_NEW_SYSTEM
	m_bOfficialPvp = false;
#endif
}

CX2PVPRoom::~CX2PVPRoom(void)
{
}

/*virtual*/ void CX2PVPRoom::Set_KRoomInfo( const KRoomInfo& kRoomInfo_ )
{	
	CX2Room::Set_KRoomInfo( kRoomInfo_ );

	m_PVPGameType	= (CX2PVPRoom::PVP_GAME_TYPE)kRoomInfo_.m_PVPGameType;
	m_WinKillNum	= kRoomInfo_.m_WinKillNum;
	m_fPlayTime		= kRoomInfo_.m_fPlayTime;
	m_bCanIntrude	= kRoomInfo_.m_bCanIntrude;
	m_WorldID		= kRoomInfo_.m_WorldID;
	m_WorldIDForUI  = kRoomInfo_.m_ShowPvpMapWorldID;
#ifdef DUNGEON_ITEM
	m_bItemMode		= kRoomInfo_.m_bIsItemMode;
#endif
}




bool CX2PVPRoom::Handler_EGS_CHANGE_MAP_REQ( int worldID )
{
	CX2PVPRoom::SlotData* pSlotData = GetMySlot();

	if( pSlotData == NULL )
		return false;

	if( IsRoomMaster() == false )
		return false;

	KEGS_CHANGE_MAP_REQ kEGS_CHANGE_MAP_REQ;
	kEGS_CHANGE_MAP_REQ.m_WorldID = worldID;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_MAP_REQ, kEGS_CHANGE_MAP_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_MAP_ACK );

	return true;
}

bool CX2PVPRoom::Handler_EGS_CHANGE_MAP_ACK( KEGS_CHANGE_MAP_ACK& kEGS_CHANGE_MAP_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_CHANGE_MAP_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_CHANGE_MAP_ACK.m_iOK );
	}
	return false;
}

bool CX2PVPRoom::Handler_EGS_CHANGE_MAP_NOT( KEGS_CHANGE_MAP_NOT& kEGS_CHANGE_MAP_NOT )
{
	//m_WorldID = (CX2World::WORLD_ID)kEGS_CHANGE_MAP_NOT.m_WorldID;
	m_WorldIDForUI = kEGS_CHANGE_MAP_NOT.m_WorldID;
	return true;
}

bool CX2PVPRoom::Handler_EGS_CHANGE_SLOT_OPEN_NOT( KEGS_CHANGE_SLOT_OPEN_NOT& kEGS_CHANGE_SLOT_OPEN_NOT )
{
	switch( GetPVPGameType() )
	{
	case PGT_TEAM:
	case PGT_TEAM_DEATH:
		{
			SlotData* pSlotData1 = GetSlotData( kEGS_CHANGE_SLOT_OPEN_NOT.m_SlotIndex );
			SlotData* pSlotData2 = GetSlotData( kEGS_CHANGE_SLOT_OPEN_NOT.m_TeamBalanceSlotIndex );

			if( pSlotData1 == NULL || pSlotData2 == NULL )
				return false;

			pSlotData1->m_SlotState = (SLOT_STATE)kEGS_CHANGE_SLOT_OPEN_NOT.m_SlotState;
			pSlotData2->m_SlotState = (SLOT_STATE)kEGS_CHANGE_SLOT_OPEN_NOT.m_SlotState;
		}
		break;

	case PGT_SURVIVAL:
		{
			SlotData* pSlotData = GetSlotData( kEGS_CHANGE_SLOT_OPEN_NOT.m_SlotIndex );

			if( pSlotData == NULL )
				return false;

			pSlotData->m_SlotState = (SLOT_STATE)kEGS_CHANGE_SLOT_OPEN_NOT.m_SlotState;
		}
		break;
	}

	return true;
}

int	CX2PVPRoom::GetClickSlot( int x, int y )
{
	D3DXVECTOR2 kConvertPos;
	kConvertPos.x = (float)x / g_pKTDXApp->GetResolutionScaleX();
	kConvertPos.y = (float)y / g_pKTDXApp->GetResolutionScaleY();
	D3DXVECTOR2 kConvertSize = g_pKTDXApp->ConvertByResolution( 50.0f, 50.0f );

	if( GetDistance2( kConvertPos.x, kConvertPos.y, 129.0f, 198.0f ) < kConvertSize.x )
	{
		return 0;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 312.0f, 198.0f ) < kConvertSize.x )
	{
		return 1;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 493.0f, 198.0f ) < kConvertSize.x )
	{
		return 2;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 676.0f, 198.0f ) < kConvertSize.x )
	{
		return 3;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 880.0f, 517.0f ) < kConvertSize.x )
	{
		return 4;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 703.0f, 517.0f ) < kConvertSize.x )
	{
		return 5;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 515.0f, 517.0f ) < kConvertSize.x )
	{
		return 6;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 339.0f, 517.0f ) < kConvertSize.x )
	{
		return 7;
	}

	return -1;
}

#ifdef SERV_PET_SYSTEM
bool CX2PVPRoom::GetClickPetSlot( int x, int y )
{
	D3DXVECTOR2 kConvertPos;
	kConvertPos.x = (float)x / g_pKTDXApp->GetResolutionScaleX();
	kConvertPos.y = (float)y / g_pKTDXApp->GetResolutionScaleY();
	D3DXVECTOR2 kConvertSize = g_pKTDXApp->ConvertByResolution( 20.0f, 20.0f );

	int iSlotIndex = -1;

	if( GetDistance2( kConvertPos.x, kConvertPos.y, 129.0f - 60.f , 198.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 0;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 312.0f - 60.f, 198.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 1;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 493.0f - 60.f, 198.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 2;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 676.0f - 60.f, 198.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 3;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 880.0f - 20.f, 517.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 4;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 703.0f - 20.f, 517.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 5;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 515.0f - 20.f, 517.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 6;
	}
	if( GetDistance2( kConvertPos.x, kConvertPos.y, 339.0f - 20.f, 517.0f + 100.f ) < kConvertSize.x )
	{
		iSlotIndex = 7;
	}

	if( GetMySlot() != NULL && GetMySlot()->m_Index == iSlotIndex )
	{
		return true;
	}

	return false;
}
#endif

bool CX2PVPRoom::IsRoomMaster( SlotData* pSlotData )
{
	if( pSlotData == NULL )
	{
		pSlotData = GetMySlot();
		if( pSlotData == NULL )
			return false;
	}

	if( pSlotData->m_bHost == true && GetActiveObserverSlotNum() == 0 )
		return true;

	if( pSlotData->m_bHost == false && pSlotData->m_bObserver == true )
		return true;

	return false;
}






bool CX2PVPRoom::IsValidWinKillNum( char cPvpGameType, char cWinKillNum )
{
	switch( cPvpGameType )
	{
	case PGT_TEAM:
		{
			if( cWinKillNum != 0 )
			{
				return false;
			}
		}
		break;

	case PGT_TEAM_DEATH:
		{
			switch( cWinKillNum )
			{
			case 4:
			case 8:
			case 12:
			case 16:
			case 20:
			case 24:
			case 28:
				break;

			default:
				return false;
			}
		}
		break;

	case PGT_SURVIVAL:
		{
			switch( cWinKillNum )
			{
			case 3:
			case 6:
			case 9:
			case 12:
				break;

			default:
				return false;
			}
		}
		break;

	default:
		{
			//START_LOG( cerr, L"대전 타입이 이상합니다." )
			//	<< BUILD_LOGc( cPvpGameType )
			//	<< END_LOG;

			return false;
		} break;
	}

	return true;
}

bool CX2PVPRoom::IsValidPvpPlayTime( char cPvpGameType, int iPlayTime )
{
	switch( cPvpGameType )
	{
	case PGT_TEAM:
		{
			switch( iPlayTime )
			{
			case 50: // 운영자급
			case 200:
			case 300:
			case 400:
				break;

			default:
				return false;
			}
		}
		break;

	case PGT_TEAM_DEATH:
		{
			switch( iPlayTime )
			{
			case 50: // 운영자급
			case 200:
			case 500:
			case 800:
			case 1100:
				break;

			default:
				return false;
			}
		}
		break;

	case PGT_SURVIVAL:
		{
			switch( iPlayTime )
			{
			case 50:
			case 200:
			case 500:
			case 800:
			case 1100:
				break;

			default:
				return false;
			}
		}
		break;

	default:
		{
			//START_LOG( cerr, L"대전 타입이 이상합니다." )
			//	<< BUILD_LOGc( cPvpGameType )
			//	<< END_LOG;

			return false;
		} break;
	}

	return true;
}

char CX2PVPRoom::GetValidWinKillNum( char cPvpGameType )
{
	switch( cPvpGameType )
	{
	case PGT_TEAM:
		{
			return 0;
		}
		break;

	case PGT_TEAM_DEATH:
		{
			return 4;
		}
		break;

	case PGT_SURVIVAL:
		{
			return 3;
		}
		break;

	default:
		{
			ASSERT( !"NO" );			
		} 
		break;
	}

	return 1;
}

int CX2PVPRoom::GetValidPvpPlayTime( char cPvpGameType )
{
	switch( cPvpGameType )
	{
	case PGT_TEAM:
	case PGT_TEAM_DEATH:
	case PGT_SURVIVAL:
		{
			return 200;
		} break;

	default:
		{
			ASSERT( !"NO" );
		} break;
	}

	return 200;
}

#ifdef DUNGEON_ITEM		
void CX2PVPRoom::SetItemMode(bool bVal)
{
	if( GetPVPGameType() == PGT_SURVIVAL)
	{
		m_bItemMode = true;
	}
	else
	{
		m_bItemMode		= bVal;
	}			
}
bool CX2PVPRoom::GetItemMode()						
{ 
	if( GetPVPGameType() == PGT_SURVIVAL)
	{
		m_bItemMode = true;
	}
	return m_bItemMode; 
}
#endif

#ifdef HEAP_BROKEN_BY_ROOM
/** @function : ApplyRoomPacketData
	@brief : static 변수, RoomPacketData 에 셋팅된 룸정보를 멤버에 적용 시킨다
*/
/*virtual*/ void CX2PVPRoom::ApplyRoomPacketData()
{
	CX2Room::ApplyRoomPacketData();

	m_bOfficialPvp = RoomPacketData.m_bOfficialPvp;

	if ( !RoomPacketData.m_mapPvpNpcInfo.empty() )
		Set_KRoomSlotInfoNpc( RoomPacketData.m_mapPvpNpcInfo );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_PVP ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

}
#endif // HEAP_BROKEN_BY_ROOM
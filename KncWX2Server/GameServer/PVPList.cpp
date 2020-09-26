#include ".\pvplist.h"

NiImplementRootRTTI( KPVPList );

KPVPList::KPVPList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum )
:KRoomList( uiRoomListID, eRoomListType, uiPageNum ),
m_ePVPChannelClass( KPVPChannelInfo::PCC_NONE )
{
}

KPVPList::~KPVPList(void)
{
	m_vecRoomList.clear();
	m_vecTypeRoomList[CXSLRoom::PGT_TEAM].clear();
	m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].clear();
	m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].clear();
}

ImplOstmOperatorW( KPVPList );
ImplToStringW( KPVPList )
{
	return stm_ << GetRTTI()->GetName()
		<< L", PVP Room List ID-" << m_uiRoomListID
		<< std::endl;
}

UidType KPVPList::GetWaitingRoomUID( IN KEGS_QUICK_JOIN_REQ& kReq )
{
	std::vector< KRoomInfoPtr >::iterator vit;
	KRoomInfoPtr spRoomInfo;

	std::vector< UidType > vecRoomUID;

	switch( kReq.m_iOption )
	{
	case CXSLRoom::PGT_TEAM:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_TEAM].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM].end(); ++vit )
			{
				spRoomInfo = ( *vit );
				if( !spRoomInfo )
				{
					START_LOG( cerr, L"룸 정보 포인터 이상." );
					continue;
				}

				if( spRoomInfo->m_RoomState == CXSLRoom::RS_WAIT && ( spRoomInfo->m_MaxSlot > spRoomInfo->m_JoinSlot ) )
				{
					// 아이템전 퀵조인 이면 아이템전 방중에서 고르고
#ifdef DUNGEON_ITEM
					if( kReq.m_bIsItemMode == true )
					{
						if( spRoomInfo->m_bIsItemMode == true )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
					}
					// 아니면 파워전 방중에서 고르자!
					else
					{
						//{{ 2010. 10. 12	최육사	대전방 퀵조인 오류 수정
						if( spRoomInfo->m_bIsItemMode == false )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
						//}}
					}
#else
					vecRoomUID.push_back( spRoomInfo->m_RoomUID );
#endif DUNGEON_ITEM
				}
			}

			int iNumRoom = ( int )vecRoomUID.size();
			if( iNumRoom > 0 )
			{
				int iIndex = rand() % iNumRoom;
				_JIF( iIndex < iNumRoom, iIndex = 0 );
				return vecRoomUID[iIndex];
			}
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
	case CXSLRoom::PGT_SURVIVAL:
		{
			for( vit = m_vecTypeRoomList[kReq.m_iOption].begin(); vit != m_vecTypeRoomList[kReq.m_iOption].end(); ++vit )
			{
				spRoomInfo = ( *vit );
				if( !spRoomInfo )
				{
					START_LOG( cerr, L"룸 정보 포인터 이상." );
					continue;
				}

				if( spRoomInfo->m_MaxSlot > spRoomInfo->m_JoinSlot )
				{
					//vecRoomUID.push_back( spRoomInfo->m_RoomUID );
					// 아이템전 퀵조인 이면 아이템전 방중에서 고르고
#ifdef DUNGEON_ITEM
					if( kReq.m_bIsItemMode == true )
					{
						if( spRoomInfo->m_bIsItemMode == true )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
					}
					// 아니면 파워전 방중에서 고르자!
					else
					{
						//{{ 2010. 10. 12	최육사	대전방 퀵조인 오류 수정
						if( spRoomInfo->m_bIsItemMode == false )
							vecRoomUID.push_back( spRoomInfo->m_RoomUID );
						//}}
					}
#else
					vecRoomUID.push_back( spRoomInfo->m_RoomUID );
#endif DUNGEON_ITEM
				}
			}

			int iNumRoom = ( int )vecRoomUID.size();
			if( iNumRoom > 0 )
			{
				int iIndex = rand() % iNumRoom;
				_JIF( iIndex < iNumRoom, iIndex = 0 );
				return vecRoomUID[iIndex];
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"대전채널 타입이 이상함.!" )
				<< BUILD_LOG( GetID() )
				<< BUILD_LOG( GetType() )
				<< BUILD_LOG( kReq.m_iOption )
#ifdef DUNGEON_ITEM
				<< BUILD_LOG( static_cast<int>(kReq.m_bIsItemMode) )
#endif DUNGEON_ITEM
				<< END_LOG;				
		}
		break;
	}

	return 0;
}

bool KPVPList::AddRoomInfo( KRoomInfoPtr spRoomInfo_ )
{
	// ※추가하려는 방정보가 매니져에서 체크하여 중복이 없다고 보고 체크루틴은 없음.

	// 새로 생긴방을 앞쪽으로 먼저 넣기 위해.
	PreInsertVector( m_vecRoomList, spRoomInfo_ );

	//대전모드일경우 게임타입별 채널관리를 따로 보관해둔다.
	switch( spRoomInfo_->m_PVPGameType )
	{
	case CXSLRoom::PGT_TEAM:
		{
			PreInsertVector( m_vecTypeRoomList[CXSLRoom::PGT_TEAM], spRoomInfo_ );
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		{
			PreInsertVector( m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH], spRoomInfo_ );
		}
		break;

	case CXSLRoom::PGT_SURVIVAL:
		{
			PreInsertVector( m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL], spRoomInfo_ );
		}
		break;

	default:
		{
			START_LOG( cerr, L"대전타입별 방정보 추가가 이상함." )
				<< BUILD_LOGc( spRoomInfo_->m_PVPGameType )
				<< END_LOG;

			return false;
		}
	}

	return true;
}

bool KPVPList::DelRoomInfo( KRoomInfoPtr spRoomInfo_ )
{
	std::vector< KRoomInfoPtr >::iterator vit;

	for( vit = m_vecRoomList.begin(); vit != m_vecRoomList.end(); ++vit )
	{
		if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
		{
			break;
		}
	}

	if( vit == m_vecRoomList.end() )
	{
		START_LOG( cerr, L"삭제하고자 하는 방정보가 없음." )
			<< BUILD_LOG(	spRoomInfo_->m_RoomUID )
			<< BUILD_LOG(	spRoomInfo_->m_uiRoomListID )
			<< BUILD_LOG(	spRoomInfo_->m_RoomName )
			<< BUILD_LOGc(	spRoomInfo_->m_RoomState )
			<< END_LOG;

		return false;
	}
	else
	{
		m_vecRoomList.erase( vit );
	}

	switch( spRoomInfo_->m_PVPGameType )
	{
	case CXSLRoom::PGT_TEAM:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_TEAM].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM].end(); ++vit )
			{
				if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
				{
					break;
				}
			}

			if( vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM].end() )
			{
				m_vecTypeRoomList[CXSLRoom::PGT_TEAM].erase( vit );
			}
			else
			{
				START_LOG( cerr, L"삭제할려는 타입별 대전방이 없음." )
					<< L"CXSLRoom::PGT_TEAM" << dbg::endl
					<< BUILD_LOG( spRoomInfo_->m_RoomUID )
					<< BUILD_LOG( spRoomInfo_->m_uiRoomListID )
					<< BUILD_LOG( spRoomInfo_->m_RoomName )
					<< BUILD_LOGc( spRoomInfo_->m_RoomState )
					<< END_LOG;
			}
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].end(); ++vit )
			{
				if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
				{
					break;
				}
			}

			if( vit != m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].end() )
			{
				m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH].erase( vit );
			}
			else
			{
				START_LOG( cerr, L"삭제할려는 타입별 대전방이 없음." )
					<< L"CXSLRoom::PGT_TEAM_DEATH" << dbg::endl
					<< BUILD_LOG(	spRoomInfo_->m_RoomUID )
					<< BUILD_LOG(	spRoomInfo_->m_uiRoomListID )
					<< BUILD_LOG(	spRoomInfo_->m_RoomName )
					<< BUILD_LOGc(	spRoomInfo_->m_RoomState )
					<< END_LOG;
			}
		}
		break;

	case CXSLRoom::PGT_SURVIVAL:
		{
			for( vit = m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].begin(); vit != m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].end(); ++vit )
			{
				if( spRoomInfo_->m_RoomUID == (*vit)->m_RoomUID )
				{
					break;
				}
			}

			if( vit != m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].end() )
			{
				m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL].erase( vit );
			}
			else
			{
				START_LOG( cerr, L"삭제할려는 타입별 대전방이 없음." )
					<< L"CXSLRoom::PGT_SURVIVAL" << dbg::endl
					<< BUILD_LOG(	spRoomInfo_->m_RoomUID )
					<< BUILD_LOG(	spRoomInfo_->m_uiRoomListID )
					<< BUILD_LOG(	spRoomInfo_->m_RoomName )
					<< BUILD_LOGc(	spRoomInfo_->m_RoomState )
					<< END_LOG;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"삭제할려는 대전 방타입이 이상함.!" )
				<< BUILD_LOGc( spRoomInfo_->m_PVPGameType )
				<< END_LOG;

			return false;
		}
	}

	//매니저에서 지우는 함수(DeleteRoomInfo())를 통해서 들어오기 때문에 참조카운터가 3이며
	//이를 넘기면 안되다.
	if( spRoomInfo_.use_count() > 3 )
	{
		START_LOG( cerr, L"RoomInfo Invalid Ref-count." )
			<< BUILD_LOG( spRoomInfo_.use_count() )
			<< BUILD_LOG( spRoomInfo_->m_RoomName )
			<< BUILD_LOG( spRoomInfo_->m_RoomUID )
			<< BUILD_LOG( GetType() )
			;
	}
	START_LOG( clog, L" Ref-count" )
		<< BUILD_LOG( spRoomInfo_.use_count() );

	return true;
}

bool KPVPList::GetRoomInfoPage( IN KEGS_ROOM_LIST_REQ& kReq_, OUT KEGS_ROOM_LIST_ACK& kAck_ )
{
	std::vector< KRoomInfoPtr >* pvecRoomList;
	pvecRoomList = &m_vecRoomList;

	switch( kReq_.m_cPVPGameType )
	{
	case CXSLRoom::PGT_TEAM:
		pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_TEAM];
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_TEAM_DEATH];
		break;

	case CXSLRoom::PGT_SURVIVAL:
		pvecRoomList = &m_vecTypeRoomList[CXSLRoom::PGT_SURVIVAL];
		break;
	}

	if( pvecRoomList->empty() == true )
	{
		kAck_.m_nTotalPage = 1;
		kAck_.m_nViewPage = 1;

		return true;
	}

	std::vector< KRoomInfoPtr >::iterator vit;

	UINT uiTotalPage = 0;
	uiTotalPage = pvecRoomList->size() / GetPageNum();

	if( uiTotalPage <= 0 )
	{
		uiTotalPage = 1;
	}

	// 마지막 페이지의 룸리스트 개수가 한페이지의 개수보다 적을 경우 페이지를 추가한다.
	if( ((pvecRoomList->size() % GetPageNum()) > 0) && (pvecRoomList->size() > GetPageNum()) )
	{
		++uiTotalPage;
	}

	//요청한 페이지가 마지막 페이지를 넘었을 경우
	//마지막 페이지가 보게될 페이지가 된다.
	if( kReq_.m_nViewPage > uiTotalPage )
	{
		kReq_.m_nViewPage = uiTotalPage;
	}

	int nS = GetPageNum() * ( kReq_.m_nViewPage - 1 );
	int nE = nS + GetPageNum();

	KRoomInfo kRoomInfo;

	kAck_.m_nTotalPage	= uiTotalPage;
	kAck_.m_nViewPage	= kReq_.m_nViewPage;
	for(; nS < nE; ++nS )
	{
		if( nS < (int)pvecRoomList->size() && nS >= 0 )
		{
			kRoomInfo = *((*pvecRoomList)[nS]);
			kAck_.m_vRoomInfo.push_back( kRoomInfo );
		}
		else
		{
			if( nS != (int)pvecRoomList->size() )
			{
				START_LOG( cerr, L"페이지 요청실패.!" )
					<< BUILD_LOG( GetPageNum() )
					<< BUILD_LOG( nS )
					<< BUILD_LOG( nE )
					<< BUILD_LOG( pvecRoomList->size() )
					<< BUILD_LOG( m_vecRoomList.size() )
					<< END_LOG;
			}

			break;
		}
	}

	if( kAck_.m_vRoomInfo.size() > GetPageNum() )
	{
		START_LOG( cerr, L"방정보 개수 이상.!(PVP)" )
			<< BUILD_LOG( kAck_.m_vRoomInfo.size() )
			<< BUILD_LOG( GetPageNum() )
			<< BUILD_LOG( nS )
			<< BUILD_LOG( nE )
			<< BUILD_LOG( pvecRoomList->size() )
			<< BUILD_LOG( (int)m_ePVPChannelClass )
			<< END_LOG;
	}

	return true;
}




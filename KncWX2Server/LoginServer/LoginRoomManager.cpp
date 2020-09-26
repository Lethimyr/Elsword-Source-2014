#include "LoginRoomManager.h"

#include "Room/BaseRoomFSM.h"
#include "Room/RoomFSM.h"
#include "Room/TradeFSM.h"
#include "Room/PersonalShopFSM.h"

//{{ 2011. 04. 06	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY	
	#include "PersonalShopAgency.h"
#endif SERV_PSHOP_AGENCY
//}}
#include "NetError.h"
#include "Enum/Enum.h"

#include "LoginServer.h"

#include "socket/NetCommon.h"		// 릴레이 서버(현재는 룸 서버) IP
#include "LoginNetLayer.h"				// 릴리이 서버(현재는 룸 서버) Port

NiImplementRTTI( KLoginRoomManager, KRoomManager );

ImplToStringW( KLoginRoomManager )
{
	stm_ << L"--------------- Personal Shop Agency Manager -------------------" << std::endl;
	stm_ << TOSTRINGW( GetNumEmptyRoom() );
	stm_ << TOSTRINGW( m_mapRoomList.size() );
	stm_ << TOSTRINGW( m_vecRoomList.size() );
	stm_ << TOSTRINGW( m_dwNextRoomNumber );
	stm_ << TOSTRINGW( m_mapPShopOwnerList.size() );
	//}}

	KTickDumpInfo kInfo;
	m_kRoomManagerTickDump.GetDump( kInfo );

	stm_ << L"-------------- Tick Dump --------------------" << std::endl;
	stm_ << L"MinTick : "		 << kInfo.m_dwMinTick << std::endl;
	stm_ << L"MinTickRegDate : " << kInfo.m_wstrMinTickRegDate << std::endl;
	stm_ << L"MaxTick : "		 << kInfo.m_dwMaxTick << std::endl;
	stm_ << L"MaxTickRegDate : " << kInfo.m_wstrMaxTickRegDate << std::endl;
	stm_ << L"AverageTick : "	 << kInfo.m_iAverageTickCount << std::endl;
	stm_ << L"TotalTickCount : " << kInfo.m_iTotalTickCount << std::endl;
	stm_ << L"PassedTickCount : " << kInfo.m_iPassedTickCount << std::endl;

	//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	stm_ << L"-------------- RoomManager Max Queue Size Dump --------------------" << std::endl;
	m_kRMEventProcess.ToString( stm_ );

	stm_ << L"-------------- Room Max Queue Size Dump --------------------" << std::endl;

	UidType iRoomUID = 0;
	KPerformer::KMaxQueueSize kMaxQueueSizeDump;
	BOOST_TEST_FOREACH( KLoginBaseRoomPtr, spRoom, m_vecRoomList )
	{
		if( spRoom == NULL )
			continue;

        KPerformer::KMaxQueueSize kMaxQueueSize = spRoom->GetMaxQueueSize();
		if( kMaxQueueSize.m_QueueSize > kMaxQueueSizeDump.m_QueueSize )
		{
            kMaxQueueSizeDump = kMaxQueueSize;
			iRoomUID = spRoom->GetUID();
		}
	}	

	stm_ << TOSTRINGW( iRoomUID );
	stm_ << TOSTRINGW( CXSLRoom::GetRoomType( iRoomUID ) );
	stm_ << TOSTRINGW( kMaxQueueSizeDump.m_QueueSize );
	stm_ << TOSTRINGW( kMaxQueueSizeDump.GetMaxQueueSizeRegDate() );
    
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}

	//{{ 2011. 03. 25	최육사	서버 상태 체크
#ifdef SERV_ROOM_TICK_DUMP
	{
		KTickDumpInfo kInfo;
		m_kRoomTickDump.GetDump( CXSLRoom::RT_PSHOP_AGENCY, kInfo );

		stm_ << L"-------------- Tick Dump --------------------" << std::endl;		
		stm_ << L"MaxTick : "		 << kInfo.m_dwMaxTick << std::endl;
		stm_ << L"MaxTickRegDate : " << kInfo.m_wstrMaxTickRegDate << std::endl;
		stm_ << L"AverageTick : "	 << kInfo.m_iAverageTickCount << std::endl;
	}
#endif SERV_ROOM_TICK_DUMP
	//}}

	return stm_;
}

KLoginRoomManager::KLoginRoomManager()
:
m_dfZUTickTime( 0.0 ),
m_dwNextRoomNumber( 1 )
{
}

KLoginRoomManager::~KLoginRoomManager()
{
}

void KLoginRoomManager::Run()
{
	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 20 );       // 0.02s 간격

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

	}

	return;
}

bool KLoginRoomManager::Init()
{
	return KThread::Begin();
}

bool KLoginRoomManager::CreateEmptyRoom()
{
	KLocker lock( m_csEmptyRoomList );

	KNetAddress kAddress;
	kAddress.m_wstrIP = NetCommon::GetLocalIPW();
	kAddress.m_usPort = KNetLayer::GetKObj()->GetNCUDPPort();

	if( m_spRoomFSM == NULL )
	{
		m_spRoomFSM = CreateRoomFSM();
	}

	UidType iRoomUID = 0;
	KLoginBaseRoomPtr spRoom( CreateRoom() );
	if( spRoom == NULL )
	{
		START_LOG( cerr, L"방 생성 실패!" )
			<< END_LOG;
        return false;
	}

	KncUid::SetPureUID( iRoomUID, static_cast<UidType>(m_dwNextRoomNumber) );
	KncUid::SetServerID( iRoomUID, SEnum::PSHOP_AGENCY_PROXY_ID );
	//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	KncUid::SetServerGroupID( iRoomUID, GetKLoginServer()->GetServerGroupID() );
#endif SERV_INTEGRATION
	//}}
	KncUid::SetReservedID( iRoomUID, CXSLRoom::RT_PSHOP_AGENCY );
	spRoom->SetUID( iRoomUID );
	spRoom->SetFSM( m_spRoomFSM );
	spRoom->SetUDPRelayInfo( kAddress );

	// 여기서는 map의 key중복 검사를 할 필요가 없을 것 같다.
	m_mapRoomList[iRoomUID] = spRoom;
	m_vecRoomList.push_back( spRoom );
	m_mapEmptyRoomList[iRoomUID] = iRoomUID;

	// 다음번에 생성할 방 숫자
	++m_dwNextRoomNumber;
	return true;
}

void KLoginRoomManager::Tick()
{
	//////////////////////////////////////////////////////////////////////////
	//{{ 2011. 03. 24	최육사	서버 상태 체크
#ifdef SERV_ROOM_TICK_DUMP
	const DWORD dwLastTickCount = ::GetTickCount();
#endif SERV_ROOM_TICK_DUMP
	//}}		
	//////////////////////////////////////////////////////////////////////////		

	BOOST_TEST_FOREACH( KLoginBaseRoomPtr, spRoom, m_vecRoomList )
	{
		if( spRoom == NULL )
		{
			START_LOG( cerr, L"룸 포인터 이상." )
				<< END_LOG;
			continue;
		}

		spRoom->Tick();
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2011. 03. 24	최육사	서버 상태 체크
#ifdef SERV_ROOM_TICK_DUMP
	const DWORD dwRoomTypeTick = ::GetTickCount() - dwLastTickCount;
	m_kRoomTickDump.CheckOneTickLatency( CXSLRoom::RT_PSHOP_AGENCY, dwRoomTypeTick );
#endif SERV_ROOM_TICK_DUMP
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2010. 03. 08  최육사	TickDumper
#ifdef SERV_ROOM_TICK
	m_kRoomManagerTickDump.CheckOneTickLatency();
#endif SERV_ROOM_TICK
	//}}
	
	//{{ 2010. 06. 17  최육사	스크립트 실시간 패치
#ifdef SERV_REALTIME_SCRIPT
	m_kRMEventProcess.Tick();
#endif SERV_REALTIME_SCRIPT
	//}}
}

#include <KncLua.h>
#include <lua_tinker.h>

void KLoginRoomManager::RegToLua()
{
	//센터서버의 셋팅관련 데이터(Set~~Func)은 config_cn.lua 를 통해 받는다.
	lua_tinker::class_add<KLoginRoomManager>( g_pLua, "KLoginRoomManager" );
	lua_tinker::class_def<KLoginRoomManager>( g_pLua, "dump",						&KLoginRoomManager::Dump );
	lua_tinker::class_def<KLoginRoomManager>( g_pLua, "SetZUTickTime",				&KLoginRoomManager::SetZUTickTime );

	lua_tinker::decl( g_pLua, "RoomManager", this );
}

void KLoginRoomManager::ShutDown()
{
	// 로그 남기기
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;

	KRoomManager::ShutDown();
}

DWORD KLoginRoomManager::GetNumEmptyRoom() const
{
	KLocker lock( m_csEmptyRoomList );

	return ( DWORD )m_mapEmptyRoomList.size();
}

UidType KLoginRoomManager::OpenRoom()
{
	KLocker lock( m_csEmptyRoomList );

	if( m_mapEmptyRoomList.empty() )
	{
		// 빈방이 부족하여 새로 방을 만듭니다!
		LIF( CreateEmptyRoom() );
	}

	std::map< UidType, UidType >::iterator mit;
	for( mit = m_mapEmptyRoomList.begin(); mit != m_mapEmptyRoomList.end(); ++mit )
	{
		KLoginBaseRoomPtr spRoom = Get( mit->first );
		if( spRoom == NULL )
		{
			START_LOG( cerr, L"빈 방 인덱스가 이상함." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}
		else
		{
			// 모든 방 FSM 들은 INIT, CLOSE, WAIT 를 가지며 이들의 값도 같다. 
			if( spRoom->GetStateID() == KBaseRoomFSM::S_INIT )
			{
				break;
			}
			else
			{
				START_LOG( cerr, L"빈 방 리스트에 있지만 방 상태가 이상함." )
					<< BUILD_LOG( spRoom->GetStateID() )
					<< BUILD_LOG( spRoom->GetStateIDString() )
					<< END_LOG;
			}
		}
	}

	if( mit == m_mapEmptyRoomList.end() )
	{
		START_LOG( cerr, L"빈 방을 찾을 수 없음." )
			<< END_LOG;

		return 0;
	}

	UidType iRoomUID = mit->first;
	m_mapEmptyRoomList.erase( mit );

	return iRoomUID;
}

bool KLoginRoomManager::CloseRoom( UidType iRoomUID )
{
	const CXSLRoom::ROOM_TYPE eRoomType = ( CXSLRoom::ROOM_TYPE )KncUid::ExtractReservedID( iRoomUID );
	if( eRoomType < 0 || eRoomType >= CXSLRoom::RT_TOTAL_NUM )
	{
		START_LOG( cerr, L"룸 타입이 이상함." )
			<< BUILD_LOG( eRoomType )
			<< BUILD_LOG( ( int )CXSLRoom::RT_TOTAL_NUM )
			<< END_LOG;

		return false;
	}

	KLocker lock( m_csEmptyRoomList );

	std::map< UidType, UidType >::iterator mit;
	mit = m_mapEmptyRoomList.find( iRoomUID );
	if( mit != m_mapEmptyRoomList.end() )
	{
		START_LOG( cerr, L"빈 방리스트에 넣으려는 ID의 방이 이미 있음." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return false;
	}

	KLoginBaseRoomPtr spRoom = Get( iRoomUID );
	if( !spRoom )
	{
		START_LOG( cerr, L"해당 UID의 방이 없음." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return false;
	}

	// 모든 방 FSM 들은 INIT, CLOSE, WAIT 를 가지며 이들의 값도 같다.
	if( spRoom->GetStateID() != KBaseRoomFSM::S_CLOSE )
	{
		START_LOG( cerr, L"방 상태 이상." )
			<< BUILD_LOG( spRoom->GetStateID() )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return false;
	}

	// 모든 방 FSM 들은 INIT, CLOSE, WAIT 를 가지며 이들의 값도 같다.
	spRoom->StateTransition( KBaseRoomFSM::I_TO_INIT );
	m_mapEmptyRoomList.insert( std::make_pair( iRoomUID, iRoomUID ) );

	return true;
}

//{{ 2010. 06. 17  최육사	오토핵 방지
void KLoginRoomManager::QueueingEventToRoomManager( const KEventPtr& spEvent_ )
{
	m_kRMEventProcess.QueueingEvent( spEvent_ );
}
//}}

void KLoginRoomManager::QueueingEventToRoomByRoomUID( IN const UidType iRoomUID, IN const KEventPtr& spEvent_ )
{
	KLoginBaseRoomPtr spRoom = Get( iRoomUID );
	if( spRoom == NULL )
	{
		START_LOG( cerr, L"존재 하지 않는 방입니다!" )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return;
	}

	spRoom->QueueingEvent( spEvent_ );
}

void KLoginRoomManager::QueueingEventToRoom( const KEventPtr& spEvent_ )
{
	UidType iRoomUID = 0;
	switch( spEvent_->m_usEventID )
	{
	case ERM_OPEN_PSHOP_AGENCY_REQ:
	case ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT:
	case ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ:
		QueueingEventToRoomManager( spEvent_ ); // SimLayer에서 처리하기 위해 LoginRMEventProcess에게 보냅니다.
		return;

	default:
		// 방 하나에만 이벤트가 전달된다고 가정함. GetFirstUID
		iRoomUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	if( iRoomUID <= 0 )
	{
		switch( spEvent_->m_usEventID )
		{
		case ERM_OPEN_PSHOP_AGENCY_REQ:
			{
				KERM_OPEN_PERSONAL_SHOP_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );
				return;
			}
			break;
		case ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT:
			{
				START_LOG( cerr, L"개설 가능한 대리상인 방이 없습니다! 일어나서는 안되는 에러!" )
					<< END_LOG;

			}
			break;
		default:
			START_LOG( cerr, L"이벤트의 목표 방 UID가 이상함." )
				<< BUILD_LOG( iRoomUID )
				<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
				<< END_LOG;
			return;
		}
	}

	KLoginBaseRoomPtr spRoom = Get( iRoomUID );
	if( spRoom == NULL )
	{
		START_LOG( cwarn, L"방 검색 실패." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return;
	}

	spRoom->QueueingEvent( spEvent_ );
}

void KLoginRoomManager::QueueingToAll( int iRoomType, const KEventPtr& spEvent_ )
{
	// 빈 방에는 큐잉하지 않는다.
	std::map< UidType, KLoginBaseRoomPtr >::iterator mitRoom;
	std::map< UidType, UidType >::iterator mitEmptyRoom;

	BOOST_TEST_FOREACH( KLoginBaseRoomPtr, spRoom, m_vecRoomList )
	{
		if( spRoom == NULL )
			continue;

		mitEmptyRoom = m_mapEmptyRoomList.find( spRoom->GetUID() );
		if( mitEmptyRoom == m_mapEmptyRoomList.end() )
		{
			spRoom->QueueingEvent( spEvent_ );
		}
	}
}

void KLoginRoomManager::QueueingToAll( const KEventPtr& spEvent_ )
{
	// 현재 열려 있는 모든 방에 이벤트를 큐잉한다. 빈 방에는 큐잉하지 않는다.	
	QueueingToAll( CXSLRoom::RT_PSHOP_AGENCY, spEvent_ );
}

void KLoginRoomManager::SetZUTickTime( double dfTickTime )
{
	m_dfZUTickTime = dfTickTime;

	START_LOG( cout2, L"Zombie User 체크 시간설정.!" )
		<< BUILD_LOG( m_dfZUTickTime );
}

double KLoginRoomManager::GetZUTickTime()
{
	if( m_dfZUTickTime <= 0.0 || m_dfZUTickTime >= 600.0 )
	{
		return 180.0;
	}

	return m_dfZUTickTime;
}

bool KLoginRoomManager::AddPShopOwner( IN const UidType iUnitUID, IN const UidType iRoomUID )
{
	UidType iCheckRoomUID = 0;
	if( GetPShopAgencyUIDByUnitUID( iUnitUID, iCheckRoomUID ) == true )
	{
		START_LOG( cerr, L"이미 등록되어 있는 유저입니다. 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iCheckRoomUID )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return false;
	}
    
	m_mapPShopOwnerList.insert( std::make_pair( iUnitUID, iRoomUID ) );
	return true;
}

void KLoginRoomManager::DelPShopOwner( IN const UidType iUnitUID )
{
	UidType iCheckRoomUID = 0;
	if( GetPShopAgencyUIDByUnitUID( iUnitUID, iCheckRoomUID ) == false )
	{
		START_LOG( cerr, L"등록되어 있는 유저가 아닙니다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}
    
	m_mapPShopOwnerList.erase( iUnitUID );
}

bool KLoginRoomManager::GetPShopAgencyUIDByUnitUID( IN const UidType iUnitUID, OUT UidType& iRoomUID )
{
	std::map< UidType, UidType >::const_iterator mit;
	mit = m_mapPShopOwnerList.find( iUnitUID );
	if( mit == m_mapPShopOwnerList.end() )
	{
		return false;
	}

	iRoomUID = mit->second;
	return true;
}

void KLoginRoomManager::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID )
{
	SendToGSCharacter( nGS, nTo, usEventID, char() );
}

FSMclassPtr KLoginRoomManager::CreateRoomFSM()
{
	return boost::shared_ptr< KPersonalShopFSM >( new KPersonalShopFSM );
}

KLoginBaseRoomPtr KLoginRoomManager::CreateRoom()
{
	return KPersonalShopAgencyPtr( new KPersonalShopAgency );
}

KLoginBaseRoomPtr KLoginRoomManager::Get( UidType iRoomUID )
{
	std::map< UidType, KLoginBaseRoomPtr >::iterator mit;
	mit = m_mapRoomList.find( iRoomUID );
	if( mit == m_mapRoomList.end() )
	{
		return KLoginBaseRoomPtr();
	}

	return mit->second;
}




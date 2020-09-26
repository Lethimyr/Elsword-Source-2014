#include "NewCnPartyManager.h"
#include "NetError.h"

#include "CnPartyFSM.h"
#include "CnServer.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLPartyManager.h"
#include "Lottery.h"
#include "Enum/Enum.h"

//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// 릴레이 서버(현재는 룸 서버) IP
#include "CnNetLayer.h"				// 릴리이 서버(현재는 룸 서버) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}
//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////


#define CLASS_TYPE KCnPartyManager
ImplPfID( KCnPartyManager, PI_CN_PARTY );

ImplementSingleton( KCnPartyManager );


KCnPartyManager::KCnPartyManager() :
m_ciMaxParty(2000)
{
	lua_tinker::class_add<KCnPartyManager>( g_pLua, "KCnPartyManager" );
	lua_tinker::class_def<KCnPartyManager>( g_pLua, "dump",						&KCnPartyManager::Dump );

	lua_tinker::decl( g_pLua, "PartyManager", this );
}

KCnPartyManager::~KCnPartyManager()
{
}

ImplToStringW( KCnPartyManager )
{
	stm_ << L"-------------- Party Manager --------------" << std::endl
		 << TOSTRINGW( m_mapParty.size() )
		 << TOSTRINGW( m_setSpareUID.size() );

	//stm_ << L"-------------- Tick Dump --------------------" << std::endl;
	//stm_ << L"MaxTick : " << m_kPartyManagerTickDump.GetMaxTick() << std::endl;
	//stm_ << L"MaxTickRegDate : " << m_kPartyManagerTickDump.GetMaxTickRegDate() << std::endl;

	return stm_;
}

void KCnPartyManager::Init()
{
	for( int iIndex = 1; iIndex <= GetMaxParty(); ++iIndex )
	{
		UidType iPartyUID = 0;
		KncUid::SetPureUID( iPartyUID, static_cast<UidType>(iIndex) );
		KncUid::SetServerID( iPartyUID, GetKCnServer()->GetProxyID() );
		//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		KncUid::SetServerGroupID( iPartyUID, GetKCnServer()->GetServerGroupID() );
#endif SERV_INTEGRATION
		//}}

		m_setSpareUID.insert( iPartyUID );

		KCnPartyPtr spParty( new KCnParty( iPartyUID ) );
		if( spParty == NULL )
		{
			START_LOG( cerr, L"KCnPartyPtr new 할당실패." )
				<< BUILD_LOG( iPartyUID )
				<< END_LOG;

			return;
		}

		// FSM설정
		FSMclassPtr spFSMClass = boost::shared_ptr< KCnPartyFSM >( new KCnPartyFSM );
		spParty->SetFSM( spFSMClass );

		// 파티 정보 생성
		m_mapParty.insert( std::make_pair( iPartyUID, spParty ) );
	}
}

void KCnPartyManager::Tick()
{
	// 이벤트를 먼저 처리한뒤..
	KPerformer::Tick();

	// 파티 Tick을 처리하자!
	std::map< UidType, KCnPartyPtr >::iterator mit;
	for( mit = m_mapParty.begin(); mit != m_mapParty.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"파티 포인터 이상" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		mit->second->Tick();
	}

	//{{ 2010. 03. 08  최육사	TickDumper
//#ifdef SERV_ROOM_TICK
//	m_kPartyManagerTickDump.CheckOneTickLatency();
//#endif SERV_ROOM_TICK
	//}}
}

KCnPartyPtr KCnPartyManager::GetParty( IN UidType iPartyUID )
{
	std::map< UidType, KCnPartyPtr >::iterator mit = m_mapParty.find( iPartyUID );
	if( mit == m_mapParty.end() )
	{
		START_LOG( cwarn, L"찾는 파티가 없습니다.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KCnPartyPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"파티 포인터 이상.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KCnPartyPtr();
	}

	return mit->second;
}

//{{ 2010. 01. 27  최육사	방 정보
#ifdef SERV_ROOM_COUNT

void KCnPartyManager::SendPartyCountInfo( bool bOpen )
{
	KERM_ROOM_COUNT_INFO_NOT kNot;
	kNot.m_cRoomType = static_cast<char>(CXSLPartyManager::PE_PARTY_COUNT_TYPE);
	kNot.m_bIsOpen = bOpen;

	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_PARTY, 0, PI_CN_PROXY, 0, anTrace, ERM_ROOM_COUNT_INFO_NOT, kNot );
}

#endif SERV_ROOM_COUNT
//}}

void KCnPartyManager::QueueingEventToParty( const KEventPtr& spEvent_ )
{
	UidType iPartyUID = 0;
	switch( spEvent_->m_usEventID )
	{
	case EPM_CREATE_PARTY_BY_INVITE_REQ:
	case EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ:
	case EPM_REQUEST_REGROUP_PARTY_NOT:
	case EPM_CHECK_INVALID_USER_NOT:
		QueueingEvent( spEvent_ ); // 게임 로직과 관련된 처리는 RoomManager Thread에서 처리해야 하므로 여기서 바로 처리 안하고 event queueing한다.
		return;

	default:
		// 파티 하나에만 이벤트가 전달된다고 가정함. GetFirstUID
		iPartyUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	KCnPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 검색 실패." )
			<< BUILD_LOG( iPartyUID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
		return;
	}

	// 파티에 event전달
	spParty->QueueingEvent( spEvent_ );
}

void KCnPartyManager::QueueingToAll( const KEventPtr& spEvent )
{
	std::map< UidType, KCnPartyPtr >::iterator mit;
	for( mit = m_mapParty.begin(); mit != m_mapParty.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"파티 포인터 이상" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mit->second->QueueingEvent( spEvent );
	}
}

void KCnPartyManager::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
        CASE( EPM_CREATE_PARTY_BY_INVITE_REQ );
		CASE( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ );
		CASE( EPM_REQUEST_REGROUP_PARTY_NOT );
	   _CASE( EPM_CHECK_INVALID_USER_NOT, UidType );

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_REQ )
{
	// 일단 파티 객체를 하나 얻어옵니다.
	KCnPartyPtr spParty = OpenParty();
	if( spParty == NULL )
	{
		START_LOG( cerr, L"생성할 빈파티 객체가 없음." )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		KEPM_CREATE_PARTY_BY_INVITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CREATE_PARTY_BY_INVITE_ACK, kPacket );
		return;
	}

	// 출발지 정보 재 입력
	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	// 해당 파티에 이벤트 큐잉
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_PARTY, anTrace, EPM_CREATE_PARTY_BY_INVITE_REQ, kPacket_ );
	spParty->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	// 일단 파티 객체를 하나 얻어옵니다.
	KCnPartyPtr spParty = OpenParty();
	if( spParty == NULL )
	{
		START_LOG( cerr, L"생성할 빈파티 객체가 없음." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket );
		return;
	}

	// 출발지 정보 재 입력
	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	// 해당 파티에 이벤트 큐잉
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_PARTY, anTrace, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, kPacket_ );
	spParty->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( EPM_REQUEST_REGROUP_PARTY_NOT )
{
	// 일단 파티 객체를 하나 얻어옵니다.
	KCnPartyPtr spParty = OpenParty();
	if( spParty == NULL )
	{
		START_LOG( cerr, L"생성할 빈파티 객체가 없음." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 출발지 정보 재 입력
	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	// 해당 파티에 이벤트 큐잉
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_PARTY, anTrace, EPM_REQUEST_REGROUP_PARTY_NOT, kPacket_ );
	spParty->QueueingEvent( spEvent );
}

_IMPL_ON_FUNC( EPM_CHECK_INVALID_USER_NOT, UidType )
{
	std::map< UidType, KCnPartyPtr >::iterator mit;
	for( mit = m_mapParty.begin(); mit != m_mapParty.end(); ++mit )
	{
		KCnPartyPtr spParty = mit->second;
		if( spParty == NULL )
		{
			START_LOG( cerr, L"파티 포인터 이상" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// 출발지 정보 재 입력
		UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

		// 해당 파티에 이벤트 큐잉
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CN_PARTY, anTrace, EPM_CHECK_INVALID_USER_NOT, kPacket_ );
		spParty->QueueingEvent( spEvent );
	}
}

KCnPartyPtr KCnPartyManager::OpenParty()
{
	std::set< UidType >::iterator sit = m_setSpareUID.begin();
	if( sit == m_setSpareUID.end() )
	{
		START_LOG( cerr, L"파티를 새로 열어야 하는데 빈리스트에 없다.?" )
			<< END_LOG;

		return KCnPartyPtr();
	}

	KCnPartyPtr spParty = GetParty( *sit );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"새로 개설할 파티 객체가 없음." )
			<< END_LOG;

		return KCnPartyPtr();
	}

	m_setSpareUID.erase( sit );
	spParty->Reset();

	//{{ 2010. 01. 27  최육사	방 정보
#ifdef SERV_ROOM_COUNT
	SendPartyCountInfo( true );
#endif SERV_ROOM_COUNT
	//}}

	return spParty;
}

void KCnPartyManager::CloseParty( IN KCnPartyPtr spParty, bool bClosePartyList /*= true*/ )
{
	const UidType iPartyUID = spParty->GetUID();

	std::set< UidType >::iterator sit = m_setSpareUID.find( spParty->GetUID() );
	if( sit != m_setSpareUID.end() )
	{
		START_LOG( cerr, L"파티를 닫을려는데 이미 닫혀있따..?" )
			<< BUILD_LOG( spParty->GetUID() )
			<< END_LOG;
		return;
	}

	m_setSpareUID.insert( spParty->GetUID() );

	// 파티가 삭제 되므로 모든 게임서버의 GSPartyManager에 삭제되는 파티를 알린다.
	spParty->DelPartyInfoInPartyList();

	//그리고 파티 초기화.
	spParty->Reset();

	// 상태 체크
	switch( spParty->GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
		// 파티 열어보지도 못하고 다시 닫는구먼!
		break;

	case KCnPartyFSM::S_MAKING_PARTY:
	case KCnPartyFSM::S_WAIT:
	case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
	case KCnPartyFSM::S_PREPARE_FOR_PLAY:
		{
			// 파티 상태 CLOSE상태로 변경
			spParty->StateTransition( KCnPartyFSM::I_TO_CLOSE );
		}

	case KCnPartyFSM::S_CLOSE:
		{
			// 파티 상태 INIT으로 변경
			spParty->StateTransition( KCnPartyFSM::I_TO_INIT );
		}
		break;

	default:
		{
			START_LOG( cerr, L"파티를 닫으려는데 상태가 이상함. 있을수 없는 에러!" )
				<< BUILD_LOG( spParty->GetStateIDString() )
				<< END_LOG;
		}
	}	

	if( bClosePartyList )
	{
		KEPM_CLOSE_PARTY_LIST_NOT kPacket;
		kPacket.m_iPartyUID = iPartyUID;
		BroadCastGameServer( EPM_CLOSE_PARTY_LIST_NOT, kPacket );
	}

	//{{ 2010. 01. 27  최육사	방 정보
#ifdef SERV_ROOM_COUNT
	SendPartyCountInfo( false );
#endif SERV_ROOM_COUNT
	//}}
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}



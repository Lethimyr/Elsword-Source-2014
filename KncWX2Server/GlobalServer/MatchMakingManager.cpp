#include "MatchMakingManager.h"
#include "NetError.h"

#include "GlobalServer.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "X2Data/XSLRoom.h"
#include "MatchFSM.h"
#include "MatchUserFSM.h"
#include "GlobalScriptRefreshManager.h"
#include "CompareLimitList.h"
//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}


//{{ 2011. 06. 14	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM



//////////////////////////////////////////////////////////////////////////
//{{ 2012. 02. 21	김민성	공식 대전 대기 시간 표시 변경
#ifdef SERV_PVP_MATCH_WAIT_TIME
KMatchCompleteTimeManager::KMatchCompleteTimeManager()
{
}

KMatchCompleteTimeManager::~KMatchCompleteTimeManager()
{
}

void KMatchCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	KMatchTime kCompleteTime;
	kCompleteTime.m_tMatchTime = CTime::GetCurrentTime();
	kCompleteTime.m_iWatiTime = iCompleteTime;

	m_listCompleteTime.push_back( kCompleteTime );

	// 5개가 될때까지 옛날것을 지운다.
	while( CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KMatchCompleteTimeManager::GetMatchMakingEstimatedTime() const
{
	// 한번도 매치가 성공한적이 없다
	if( m_listCompleteTime.empty() )
		return 2000; // 클라이언트에서 30분 이상이면 '알수 없음'으로 표시한다

	int iSumCompleteTime = 0;												// 최근 성공 대기 시간 총 합
	int iEstimatedTime	 = 0;												// 최근 성공 대기 시간 평균
	int	iSumCount		 = 0;												// 계산 추가된 횟수
	CTime tCurr = CTime::GetCurrentTime();									// 현재 시간
	CTime tTwoHourAgo = tCurr - CTimeSpan( 0, 2, 0, 0 );					// 현재 시간에서 두시간 전 시간

	BOOST_TEST_FOREACH( const KMatchTime, kCompleteTime, m_listCompleteTime )
	{
		if( tTwoHourAgo <= kCompleteTime.m_tMatchTime )	// 최근 2시간 이내 라면
		{
			iSumCompleteTime += kCompleteTime.m_iWatiTime;
			++iSumCount;
		}
	}

	if( iSumCount <= 0 )	// 계산에 추가 된 횟수가 없다 -> 매칭 성공 정보는 있지만 2시간이 지난 정보이므로 버리자
	{
		return 2000; // 클라이언트에서 30분 이상이면 '알수 없음'으로 표시한다
	}

	iEstimatedTime = ( iSumCompleteTime / iSumCount );
	const int iMiddleResult = iEstimatedTime % 10; // 10초단위 올림
	if( iMiddleResult > 0 )
	{
		iEstimatedTime = ( iEstimatedTime - iMiddleResult ) + 10;			// 평균값을 올림한다.
	}

	return iEstimatedTime;
}
#else
KMatchCompleteTimeManager::KMatchCompleteTimeManager()
{
}

KMatchCompleteTimeManager::~KMatchCompleteTimeManager()
{
}

void KMatchCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	m_listCompleteTime.push_back( iCompleteTime );

	// 5개가 될때까지 옛날것을 지운다.
	while( COMPLETE_TIME_ENUM::CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KMatchCompleteTimeManager::GetMatchMakingEstimatedTime() const
{
	// 한번도 매치가 성공한적이 없다
	if( m_listCompleteTime.empty() )
		return 10; // 디폴트값 10초

	const int iCompleteCount = static_cast<int>(m_listCompleteTime.size());
	int iSumCompleteTime = 0;

	BOOST_TEST_FOREACH( const int, iTime, m_listCompleteTime )
	{
		iSumCompleteTime += iTime;
	}

	int iEstimatedTime = ( iSumCompleteTime / iCompleteCount );
	const int iMiddleResult = iEstimatedTime % 10; // 10초단위 올림
	if( iMiddleResult > 0 )
	{
		iEstimatedTime = ( iEstimatedTime - iMiddleResult ) + 10;
	}

	return iEstimatedTime;
}
#endif SERV_PVP_MATCH_WAIT_TIME
//}}
//////////////////////////////////////////////////////////////////////////


#define CLASS_TYPE KMatchMakingManager
ImplPfID( KMatchMakingManager, PI_CN_PARTY );

ImplementSingleton( KMatchMakingManager );


FSMclassPtr KMatchMakingManager::ms_spUserFSM = KMatchUserFSMPtr( new KMatchUserFSM );


KMatchMakingManager::KMatchMakingManager() :
m_iLastMatchUID( 0 ),
m_iLastWaitNumber( 0 ),
m_iRematchCount( 0 )
{
	lua_tinker::class_add<KMatchMakingManager>( g_pLua, "KMatchMakingManager" );
	lua_tinker::class_def<KMatchMakingManager>( g_pLua, "Command",					&KMatchMakingManager::Command_LUA );
	lua_tinker::class_def<KMatchMakingManager>( g_pLua, "dump",						&KMatchMakingManager::Dump );

	lua_tinker::decl( g_pLua, "MatchManager", this );
}

KMatchMakingManager::~KMatchMakingManager()
{
}

ImplToStringW( KMatchMakingManager )
{
	stm_ << L"-------------- Match Making Manager --------------" << std::endl
		<< TOSTRINGW( m_iLastMatchUID )
		<< TOSTRINGW( m_mapMatch.size() )
		<< TOSTRINGW( m_iLastWaitNumber )
		<< TOSTRINGW( m_mapMatchWaitUser.size() )
		<< TOSTRINGW( m_mapMatchWaitList.size() )
		<< TOSTRINGW( m_mapMatchWaitParty.size() )
		<< TOSTRINGW( m_iRematchCount )
		;

	for( int iEnum = 0; iEnum < SEnum::MT_MAX; ++iEnum )
	{
		stm_ << L"------------------------------------------" << std::endl
			<< TOSTRINGW( SEnum::GetMatchTypeName( iEnum ) )
			<< TOSTRINGW( m_listMatchWaitList[iEnum].size() )
			<< TOSTRINGW( m_kCompleteTimeManager[iEnum].GetListSize() )
			;
	}

	return stm_;
}

void KMatchMakingManager::Command_LUA( char cType )
{
    START_LOG( cout, L"매치 메이킹 개발자 전용 커맨드!" )
		<< BUILD_LOGc( cType );

	KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT kPacket;
	kPacket.m_cType = cType;
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GLOBAL_MATCH_MAKING, NULL, EGB_MATCH_MAKING_ADMIN_COMMAND_NOT, kPacket );
	QueueingEventToMatch( spEvent );
}

void KMatchMakingManager::Run()
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

void KMatchMakingManager::ShutDown()
{
	End();
}

bool KMatchMakingManager::Init()
{
	return KThread::Begin();
}

void KMatchMakingManager::ClearAllMatchInfo()
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout2, L"[경고!] 모든 매치 정보를 초기화 시킵니다!" );
	//////////////////////////////////////////////////////////////////////////	
	// 매치 종료 처리!
	std::vector< UidType > vecMatchList;
	for( u_int uiMatchType = 0; uiMatchType < SEnum::MT_MAX; ++uiMatchType )
	{
		BOOST_TEST_FOREACH( KMatchPtr, spMatch, m_vecMatch[uiMatchType] )
		{
			if( !spMatch )
				continue;

			vecMatchList.push_back( spMatch->GetMatchUID() );
		}
	}

	BOOST_TEST_FOREACH( const UidType, iMatchUID, vecMatchList )
	{
		DestroyMatch( iMatchUID );
	}


	//////////////////////////////////////////////////////////////////////////
	// 대기자 리스트 종료 처리
	std::vector< UidType > vecUnitUIDLIst;
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMatchWaitUser.begin(); mit != m_mapMatchWaitUser.end(); ++mit )
	{
		vecUnitUIDLIst.push_back( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDLIst )
	{
		UnRegMatchWaitListByUnitUID( iUnitUID, NetError::NOT_LEAVE_MATCH_WAIT_REASON_01 );
	}
}

void KMatchMakingManager::Tick()
{
	// MatchMakingManager이벤트를 먼저 처리한뒤..
	KPerformer::Tick();

	std::vector< UidType > vecDeleteMatchUIDList;

	// 매치 Tick을 처리하자!	
	for( u_int uiMatchType = 0; uiMatchType < SEnum::MT_MAX; ++uiMatchType )
	{
		BOOST_TEST_FOREACH( KMatchPtr, spMatch, m_vecMatch[uiMatchType] )
		{
			if( spMatch == NULL )
				continue;

			spMatch->Tick();

			// 마지막 상태가 S_CLOSE라면 종료 리스트에 MatchUID값 넣자!
			if( spMatch->GetStateID() == KMatchFSM::S_CLOSE )
			{
				vecDeleteMatchUIDList.push_back( spMatch->GetUID() );
			}
		}
	}

	// 매치 종료 처리!
	BOOST_TEST_FOREACH( const UidType, iMatchUID, vecDeleteMatchUIDList )
	{
        DestroyMatch( iMatchUID );
	}

	// 매치 메이킹
	for( u_int uiMatchType = 0; uiMatchType < SEnum::MT_MAX; ++uiMatchType )
	{
		//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
#else
		// 이미 생성된 매치들을 위한 매치 메이킹
		MatchMakingForCreatedMatch( static_cast<SEnum::MATCH_TYPE>(uiMatchType) );
#endif SERV_2012_PVP_SEASON2
		//}}

		// 대기중인 대기자들을 위한 매치 메이킹
		MatchMakingForWaitUser( static_cast<SEnum::MATCH_TYPE>(uiMatchType) );
	}
}

KMatchPtr KMatchMakingManager::MakeMatch( IN const SEnum::MATCH_TYPE eMatchType )
{
	KMatchPtr spMatch( new KMatch( ++m_iLastMatchUID, eMatchType ) );
	if( spMatch == NULL )
	{
		START_LOG( cerr, L"KCnPartyPtr new 할당실패." )
			<< BUILD_LOG( m_iLastMatchUID )
			<< END_LOG;

		return KMatchPtr();
	}

	// FSM설정
	FSMclassPtr spFSMClass = boost::shared_ptr< KMatchFSM >( new KMatchFSM );
	spMatch->SetFSM( spFSMClass );
	return spMatch;
}

KMatchUserPtr KMatchMakingManager::MakeUser( IN const SEnum::MATCH_TYPE eMatchType, IN const KMatchUserInfo& kInfo )
{
	KMatchUserPtr spMatchUser( new KMatchUser( kInfo ) );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"매치 유저 생성 실패!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	spMatchUser->SetFSM( ms_spUserFSM );
	spMatchUser->SetMatchType( eMatchType );
	return spMatchUser;
}

KMatchUserPtr KMatchMakingManager::GetMatchWaitUserByUnitUID( IN const UidType iUnitUID )
{
	std::map< UidType, KMatchUserPtr >::iterator mit = m_mapMatchWaitUser.find( iUnitUID );
	if( mit == m_mapMatchWaitUser.end() )
	{
		START_LOG( cwarn, L"찾는 매치유저가 없습니다.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 유저 포인터 이상.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	return mit->second;
}

KMatchUserWaitListPtr KMatchMakingManager::GetMatchWaitListByWaitNumber( IN const int iWaitNumber )
{
	std::map< int, KMatchUserWaitListPtr >::iterator mit = m_mapMatchWaitList.find( iWaitNumber );
	if( mit == m_mapMatchWaitList.end() )
	{
		START_LOG( cwarn, L"찾는 매치 대기자 리스트가 없습니다.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 매치 대기자 리스트 포인터 이상.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	return mit->second;
}

KMatchUserWaitListPtr KMatchMakingManager::GetMatchWaitListByPartyUID( IN const UidType iPartyUID )
{
	std::map< UidType, KMatchUserWaitListPtr >::iterator mit = m_mapMatchWaitParty.find( iPartyUID );
	if( mit == m_mapMatchWaitParty.end() )
	{
		START_LOG( cwarn, L"찾는 매치 대기자 리스트가 없습니다.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 매치 대기자 리스트 포인터 이상.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	return mit->second;
}

KMatchPtr KMatchMakingManager::GetMatch( IN UidType iMatchUID )
{
	std::map< UidType, KMatchPtr >::iterator mit = m_mapMatch.find( iMatchUID );
	if( mit == m_mapMatch.end() )
	{
		START_LOG( cwarn, L"찾는 매치가 없습니다.!" )
			<< BUILD_LOG( iMatchUID )
			<< END_LOG;

		return KMatchPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 포인터 이상.!" )
			<< BUILD_LOG( iMatchUID )
			<< END_LOG;

		return KMatchPtr();
	}

	return mit->second;
}

KMatchPtr KMatchMakingManager::CreateMatch( IN const SEnum::MATCH_TYPE eMatchType, IN const std::vector< KMatchUserInfo >& vecMatchUserInfo )
{
	KMatchPtr spMatch = MakeMatch( eMatchType );
	if( spMatch == NULL )
	{
		START_LOG( cerr, L"매치 생성 실패!" )
			<< BUILD_LOG( eMatchType )
			<< END_LOG;

		return KMatchPtr();
	}

	// 매치 정보 초기화
	spMatch->Enter( vecMatchUserInfo );

	// 매치 등록
	m_mapMatch.insert( std::make_pair( spMatch->GetUID(), spMatch ) );
	m_vecMatch[eMatchType].push_back( spMatch );

	// 매치가 처음 생성되면 매치 메이킹 상태이다!
	spMatch->StateTransition( KMatchFSM::I_TO_MATCH_MAKING );
    return spMatch;
}

void KMatchMakingManager::DestroyMatch( IN const UidType iMatchUID )
{
	KMatchPtr spMatch = GetMatch( iMatchUID );
	if( spMatch == NULL )
	{
		START_LOG( cerr, L"삭제하려는 매치 정보가 없습니다!" )
			<< BUILD_LOG( iMatchUID )
			<< END_LOG;
		return;
	}

	const SEnum::MATCH_TYPE eMatchType = spMatch->GetMatchType();	

	std::vector< KMatchPtr >::iterator vit;
	for( vit = m_vecMatch[eMatchType].begin(); vit != m_vecMatch[eMatchType].end(); ++vit )
	{
		KMatchPtr spMatch = *vit;
		if( spMatch == NULL )
			continue;

		if( spMatch->GetUID() == iMatchUID )
			break;
	}

	if( vit != m_vecMatch[eMatchType].end() )
	{
		m_vecMatch[eMatchType].erase( vit );
	}

	m_mapMatch.erase( iMatchUID );
}

void KMatchMakingManager::QueueingEventToMatch( IN const KEventPtr& spEvent_ )
{
	UidType iMatchUID = 0;
	switch( spEvent_->m_usEventID )
	{
	case EGB_REQUEST_MATCH_MAKING_REQ:
	case EGB_CANCEL_MATCH_MAKING_REQ:
	case EPM_CHECK_INVALID_USER_NOT:
	case ESR_ORDER_TO_REFRESH_MANAGER_REQ:
		QueueingEvent( spEvent_ ); // 게임 로직과 관련된 처리는 RoomManager Thread에서 처리해야 하므로 여기서 바로 처리 안하고 event queueing한다.
		return;

	default:
		iMatchUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	KMatchPtr spMatch = GetMatch( iMatchUID );
	if( spMatch == NULL )
	{
		START_LOG( cwarn, L"매치 검색 실패." )
			<< BUILD_LOG( iMatchUID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
		return;
	}

	// 매치에 event전달
	spMatch->QueueingEvent( spEvent_ );
}

void KMatchMakingManager::QueueingToAll( IN const KEventPtr& spEvent )
{
	std::map< UidType, KMatchPtr >::iterator mit;
	for( mit = m_mapMatch.begin(); mit != m_mapMatch.end(); ++mit )
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

bool KMatchMakingManager::RegMatchWaitList( IN const SEnum::MATCH_TYPE eType, IN const std::vector< KMatchUserInfo >& vecMatchUserList, IN const bool bRegFrontOfList )
{
	// 1. 매치 인원 수가 맞는지 체크!
	if( KMatch::GetMatchPlayerCount( eType ) < (int)vecMatchUserList.size() )
	{
		START_LOG( cerr, L"매치 대기자 리스트 생성 실패!" )
			<< BUILD_LOG( eType )
			<< BUILD_LOG( KMatch::GetMatchPlayerCount( eType ) )
			<< BUILD_LOG( vecMatchUserList.size() )
			<< END_LOG;
		return false;
	}

	// 2. 매치 대기자 리스트를 생성 합니다.
	KMatchUserWaitListPtr spWaitList( new KMatchUserWaitList( ++m_iLastWaitNumber ) );
	if( spWaitList == NULL )
	{
		START_LOG( cerr, L"매치 대기자 리스트 생성 실패!" )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return false;
	}
	
	// 3. 매치 대기자 리스트에 신청자들을 하나씩 넣습니다!
	BOOST_TEST_FOREACH( const KMatchUserInfo, kInfo, vecMatchUserList )
	{
		// 이미 등록된 캐릭터 인지 검사
		if( IsExistInWaitListByUnitUID( kInfo.m_iUnitUID ) == true )
		{
			START_LOG( cerr, L"이미 매치에 등록되어있는 유저!" )
				<< BUILD_LOG( eType )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

		// 이미 등록된 파티인지 검사
		if( kInfo.m_iPartyUID != 0 )
		{
			if( IsExistInWaitListByPartyUID( kInfo.m_iPartyUID ) == true )
			{
				START_LOG( cerr, L"이미 등록된 파티 정보 입니다! 절대 일어나서는 안되는 에러!" )
					<< BUILD_LOG( eType )
					<< BUILD_LOG( kInfo.m_iPartyUID )
					<< END_LOG;
				return false;
			}
		}

        KMatchUserPtr spMatchUser = MakeUser( eType, kInfo );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 생성 중 매치 유저 생성 실패!" )
				<< BUILD_LOG( eType )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

		// 대기자 리스트에 매치 유저를 등록하자!
		spWaitList->AddMatchUser( spMatchUser );

		// 전체 유저 리스트에도 등록하자!
		m_mapMatchWaitUser.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser ) );
	}

	if( spWaitList->GetNumMember() == 0 )
	{
		START_LOG( cerr, L"대기자 리스트를 생성하지 못하였습니다." )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return false;
	}

	// 4. 해당 타입의 대기자리스트에 넣습니다!
	if( bRegFrontOfList )
	{
		// 재신청이라면 리스트 최상위에 넣자!
		m_listMatchWaitList[eType].push_front( spWaitList );
	}
	else
	{
		// 새로운 신청이라면 리스트 최하위에 넣자!
		m_listMatchWaitList[eType].push_back( spWaitList );
	}

	// 5. 대기번호를 key로하는 컨테이너에도 넣습니다!
	m_mapMatchWaitList.insert( std::make_pair( spWaitList->GetMatchWaitNumber(), spWaitList ) );

	// 6. PartyUID가 존재한다면?
	const UidType iPartyUID = spWaitList->GetPartyUID();
	if( iPartyUID != 0 )
	{
		// 이미 등록된 파티인지는 위에서 이미 다 검사 했다!
		m_mapMatchWaitParty.insert( std::make_pair( iPartyUID, spWaitList ) );
	}
	
	return true;
}

bool KMatchMakingManager::UnRegMatchWaitListByUnitUID( IN const UidType iUnitUID, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KMatchUserPtr spMatchUser = GetMatchWaitUserByUnitUID( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"대기자 리스트 등록해제를 하려고 하는데 유저를 찾지 못하였습니다." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	// 매치 타입을 먼저 얻자!	
	const SEnum::MATCH_TYPE eMatchType = static_cast<SEnum::MATCH_TYPE>(spMatchUser->GetMatchType());
	if( eMatchType < 0  ||  eMatchType > SEnum::MT_MAX )
	{
		START_LOG( cerr, L"매치 타입이 이상합니다! 절대 일어날수 없는 에러!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( eMatchType )
			<< BUILD_LOG( iUnRegReason )
			<< END_LOG;
		return false;
	}

	// 해당 매치 정보 삭제
    m_mapMatchWaitUser.erase( iUnitUID );

	// 대기자 리스트에서도 삭제
	std::list< KMatchUserWaitListPtr >::iterator lit = m_listMatchWaitList[eMatchType].begin();
	for( ; lit != m_listMatchWaitList[eMatchType].end(); ++lit )
	{
		if( (*lit) == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다" )
				<< END_LOG;
			continue;
		}

        if( (*lit)->IsExistMatchUser( iUnitUID ) )
		{
            (*lit)->DelMatchUser( iUnitUID );
			break;
		}
	}

	if( lit != m_listMatchWaitList[eMatchType].end() )
	{
		if( (*lit)->GetNumMember() == 0 )
		{
			const int iMatchWaitNumber = (*lit)->GetMatchWaitNumber();
			m_listMatchWaitList[eMatchType].erase( lit );
            m_mapMatchWaitList.erase( iMatchWaitNumber );
		}
	}
	else
	{
		START_LOG( cerr, L"대기자 리스트에 매치유저가 없다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}

	// 취소 처리를 날리자!
	KEGS_UNREG_MATCH_WAIT_LIST_NOT kPacketNot;
	kPacketNot.m_iReason = iUnRegReason;
	kPacketNot.m_wstrCancelUnitNickName = wstrCancelUnitNickName;
	spMatchUser->SendToGSCharacter( EGB_UNREG_MATCH_WAIT_LIST_NOT, kPacketNot );

	// 매치가 성공해서 대기자 리스트에서 빠지는거라면 매치 성공 통계를 남기자!
	if( iUnRegReason == NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 )
	{
		m_kCompleteTimeManager[eMatchType].AddCompleteTime( static_cast<int>(spMatchUser->GetMatchCompleteSeconds()) );
	}
	return true;
}

bool KMatchMakingManager::UnRegMatchWaitListByWaitNumber( IN const int iWaitNumber, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KMatchUserWaitListPtr spMatchWaitList = GetMatchWaitListByWaitNumber( iWaitNumber );
	if( spMatchWaitList == NULL )
	{
		START_LOG( cerr, L"대기자 리스트 등록해제를 하려고 하는데 리스트를 찾지 못하였습니다." )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;
		return false;
	}

	// 파티UID값을 얻어내서 파티컨테이너부터 삭제하자!
	const UidType iPartyUID = spMatchWaitList->GetPartyUID();
	if( iPartyUID != 0 )
	{
		if( IsExistInWaitListByPartyUID( iPartyUID ) == false )
		{
			START_LOG( cerr, L"PartyUID로 대기자리스트를 찾지 못했다? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iWaitNumber )
				<< BUILD_LOG( NetError::GetErrStr( iUnRegReason ) )
				<< BUILD_LOG( wstrCancelUnitNickName )
				<< BUILD_LOG( iPartyUID )
				<< END_LOG;
			// 따로 처리할건 없겠군..
		}
		else
		{
			// 파티 컨테이너에서 삭제!
			m_mapMatchWaitParty.erase( iPartyUID );
		}
	}

	// 캐릭터 리스트를 얻어서 하나씩 등록해제하자!
	std::vector< UidType > vecUnitUIDList;
	spMatchWaitList->GetUnitUIDList( vecUnitUIDList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDList )
	{
		UnRegMatchWaitListByUnitUID( iUnitUID, iUnRegReason, wstrCancelUnitNickName );
	}
	
	return true;
}

bool KMatchMakingManager::UnRegMatchWaitListByPartyUID( IN const UidType iPartyUID, IN const int iUnRegReason, IN const UidType iRequestCancelUnitUID )
{
	KMatchUserWaitListPtr spWaitList = GetMatchWaitListByPartyUID( iPartyUID );
	if( spWaitList == NULL )
	{
		START_LOG( cerr, L"이탈 요청한 PartyUID가 등록되어 있지 않네?" )
			<< BUILD_LOG( iPartyUID )
			<< BUILD_LOG( NetError::GetErrStr( iUnRegReason ) )
			<< BUILD_LOG( iRequestCancelUnitUID )
			<< END_LOG;
		return false;
	}

	// 해당 PartyUID에 맞는 캐릭터가 있었는지 보자!
	KMatchUserPtr spCancelMatchUser = spWaitList->GetMatchUser( iRequestCancelUnitUID );
	if( spCancelMatchUser == NULL )
	{
		START_LOG( cerr, L"이탈 요청한 PartyUID에 해당되는 캐릭터가 없네? 이건 처리하면 안되겠다." )
			<< BUILD_LOG( iPartyUID )
			<< BUILD_LOG( NetError::GetErrStr( iUnRegReason ) )
			<< BUILD_LOG( iRequestCancelUnitUID )
			<< END_LOG;
		return false;
	}
	
	return UnRegMatchWaitListByWaitNumber( spWaitList->GetMatchWaitNumber(), iUnRegReason, spCancelMatchUser->GetNickName() );
}

//{{ 2012. 09. 15	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchMakingManager::FindTheOpponent( IN const SEnum::MATCH_TYPE& eType,
										   IN const KMatchUserWaitListPtr& spFrontMatchWaitUser,
										   IN const std::list<KMatchUserWaitListPtr>& listMatchMakingWaitList,
										   OUT std::vector<int>& vecSelectedWaitUserList )
{
	const int CHECK_WAIT_USER_COUNT = SiKPvpMatchManager()->GetCheckWaitUserCount();
	int iCheckListIndex = 0;

	vecSelectedWaitUserList.clear();

	// listSortedWaitUserList를 +- 200, +- 400, ArrangeUser로 분할
	std::list<KMatchUserWaitListPtr> listUnderTheRatingGab200;
	std::list<KMatchUserWaitListPtr> listUnderTheRatingGab400;
	std::list<KMatchUserWaitListPtr> listUnderTheRatingGabArrange;

	const int iUpperRating200 = 200;
	const int iUpeerRating400 = 400;
	
	// 매치 대기자 리스트의 멤버가 모두 F랭크인지 검사!
	const bool bIsAllMemberBeginner = spFrontMatchWaitUser->IsAllPlayersBeginner();

	BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spTargetUserList, listMatchMakingWaitList )
	{
		// 요만큼만 체크하자!
		if( iCheckListIndex > CHECK_WAIT_USER_COUNT )
			break;

		++iCheckListIndex;

		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		// 매치 메이킹 준비가 되었는지 확인한다!
		if( spTargetUserList->IsPossibleMatchMakingBegin() == false )
			continue;
		
		int iOptimalMatchPoint = 0;

		// 최적의 매치 조건인지 체크!
		if( spFrontMatchWaitUser->CheckOptimalMatch( spTargetUserList, iOptimalMatchPoint ) == false )
			continue;

		// 상대가 배치 랭커라면,
		if(	spTargetUserList->IsAllPlayersArrangeRank() == true )
		{
			// Rating 400 이내의
			if( iOptimalMatchPoint <= iUpeerRating400 )
			{
				listUnderTheRatingGabArrange.push_back( spTargetUserList );
			}
		}
		else
		{
			if( iOptimalMatchPoint <= iUpperRating200 )
			{
				listUnderTheRatingGab200.push_back( spTargetUserList );
			}
			else if( iOptimalMatchPoint <= iUpeerRating400 )
			{
				listUnderTheRatingGab400.push_back( spTargetUserList );
			}
		}
	}

	// 체크된 매치 인원 수
	int iSumMatchPlayerCount = spFrontMatchWaitUser->GetNumMember();
	int iMinRating = spFrontMatchWaitUser->GetRatingWeightAverage();
	int iMaxRating = spFrontMatchWaitUser->GetRatingWeightAverage();

	int i2NumPartyCount = ( spFrontMatchWaitUser->GetNumMember() == 2 ) ? 1 : 0;
	if( _CheckTheMemberNum( eType, bIsAllMemberBeginner, listUnderTheRatingGab200, iSumMatchPlayerCount, iMinRating, iMaxRating, i2NumPartyCount, vecSelectedWaitUserList ) == true )
	{
		return true;
	}

	if( _CheckTheMemberNum( eType, bIsAllMemberBeginner, listUnderTheRatingGab400, iSumMatchPlayerCount, iMinRating, iMaxRating, i2NumPartyCount, vecSelectedWaitUserList ) == true )
	{
		return true;
	}

	if( _CheckTheMemberNum( eType, bIsAllMemberBeginner, listUnderTheRatingGabArrange, iSumMatchPlayerCount, iMinRating, iMaxRating, i2NumPartyCount, vecSelectedWaitUserList ) == true )
	{
		return true;
	}

	return false;
}

bool KMatchMakingManager::_CheckTheMemberNum( IN const SEnum::MATCH_TYPE& eType,
											  IN const bool bIsAllMemberBeginner,
											  IN std::list<KMatchUserWaitListPtr>& listMatchMakingWaitList,
											  IN OUT int& iSumMatchPlayerCount,
											  IN OUT int& iMinRating,
											  IN OUT int& iMaxRating,
											  IN OUT int& i2NumPartyCount,
											  OUT std::vector<int>& vecSelectedWaitUserList )
{
	const int MATCH_TOTAL_PLAYER_COUNT = KMatch::GetMatchTotalPlayerCount( eType );
	const int GAP_MIN_MAX_RATING = SiKPvpMatchManager()->GetGapMinMaxRating();

	// 정리된 리스트를 바탕으로 매치 인원에 맞추어 추려내자!
	while( listMatchMakingWaitList.empty() == false )
	{
		// 랜덤 유저 매칭
		std::list<KMatchUserWaitListPtr>::iterator it = listMatchMakingWaitList.begin();
		for( int i=0, iMax = ( rand() % listMatchMakingWaitList.size() ); i < iMax; ++i )
		{
			++it;
		}

		KMatchUserWaitListPtr spTargetUserList = *it;
		listMatchMakingWaitList.erase( it );
		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		// 사람수에 맞는지 체크!
		if( ( iSumMatchPlayerCount + spTargetUserList->GetNumMember() ) > MATCH_TOTAL_PLAYER_COUNT )
			continue;

		// 2인으로 신청한 유저들은 반드시 같은 팀으로 정해주어야 하기 때문에
		// 하나의 매칭에서 2인 파티는 최대 2파티만이 가능하다.
		// 3인 파티의 경우에는 전체 매칭 인원 체크에서 검사하고 있으며,
		// 1인 파티의 경우에는 제한이 없어야 한다.
		if( spTargetUserList->GetNumMember() == 2 )
		{
			if( i2NumPartyCount == 2 )
			{
				continue;
			}
			else if( 2 < i2NumPartyCount )
			{
				START_LOG( cerr, L"하나의 매칭에서 2인 파티의 수가 이미 2개를 넘어섰습니다.")
					<< BUILD_LOG( i2NumPartyCount )
					<< END_LOG;
				return false;
			}
			++i2NumPartyCount;
		}

		// 매치 대기자 모두 F랭크면 다른 유저도 F랭크로만 구성된 대기자 리스트를 매치 시킨다!
		if( bIsAllMemberBeginner != spTargetUserList->IsAllPlayersBeginner() )
		{
			continue;
		}

		// 매치 대상자들의 최소 rating과 최대 rating간의 차이가 너무 큰건 아닌지 체크한다!
		int iCheckMinRating = iMinRating;
		int iCheckMaxRating = iMaxRating;

		if( iMinRating > spTargetUserList->GetRatingWeightAverage() )
		{
			iCheckMinRating = spTargetUserList->GetRatingWeightAverage();
		}
		else if( iMaxRating < spTargetUserList->GetRatingWeightAverage() )
		{
			iCheckMaxRating = spTargetUserList->GetRatingWeightAverage();
		}

		if( abs( iCheckMaxRating - iCheckMinRating ) > GAP_MIN_MAX_RATING )
		{
			continue;
		}

		iMinRating = iCheckMinRating;
		iMaxRating = iCheckMaxRating;

		iSumMatchPlayerCount += spTargetUserList->GetNumMember();
		vecSelectedWaitUserList.push_back( spTargetUserList->GetMatchWaitNumber() );

		// 모든 인원이 다 채워졌으면 이탈!
		if( iSumMatchPlayerCount == MATCH_TOTAL_PLAYER_COUNT )
			return true;
	}

	// 모든 인원이 준비되었는지 인원수를 체크하자!
	if( iSumMatchPlayerCount == MATCH_TOTAL_PLAYER_COUNT )
	{
		return true;
	}
	return false;
}
#else
void KMatchMakingManager::MatchMakingForCreatedMatch( IN const SEnum::MATCH_TYPE eType )
{
	const int MATCH_TOTAL_PLAYER_COUNT = KMatch::GetMatchTotalPlayerCount( eType );

	//////////////////////////////////////////////////////////////////////////	
    // 먼저 매치들을 가지고 비어있는 매치를 채우고 매치를 시작시키자!
	BOOST_TEST_FOREACH( KMatchPtr, spMatch, m_vecMatch[eType] )
	{
		if( spMatch == NULL )
		{
			START_LOG( cerr, L"매치 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		// 1. 매치 메이킹 상태인 매치만 체크한다!
		if( spMatch->GetStateID() != KMatchFSM::S_MATCH_MAKING )
			continue;

		// 2. 모든 멤버들이 F랭크인지 검사
		const bool bIsAllMemberBeginner = spMatch->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F );

		int iBestOptimalMatchPoint = INT_MAX; // 최적의 매치 조건 등급
		KMatchUserWaitListPtr spSelectedWaitList;

		// 3. 해당 매치에 들어갈만한 최적의 대기자를 찾아서 집어 넣자!
		BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spWaitList, m_listMatchWaitList[eType] )
		{
			if( spWaitList == NULL )
			{
				START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다." )
					<< END_LOG;
				continue;
			}

			// 매치 메이킹 준비가 되었는지 확인한다!
			if( spWaitList->IsPossibleMatchMakingBegin() == false )
				continue;

			// 인원수가 맞는지 체크하자!
			if( ( spMatch->GetNumMember() + spWaitList->GetNumMember() ) > MATCH_TOTAL_PLAYER_COUNT )
				continue;

			// 매치의 멤버들이 모두 F랭크면 다른 멤버도 F랭크여야만 한다.
			if( bIsAllMemberBeginner )
			{
				if( spWaitList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == false )
					continue;
			}
			else
			{
				if( spWaitList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == true )
					continue;
			}

			int iOptimalMatchPoint = 0;

			// 최적의 매치 조건인지 체크!
			if( spMatch->CheckOptimalMatch( spWaitList, iOptimalMatchPoint ) == false )
				continue;

			// 그렇다면 저장!
			if( iOptimalMatchPoint < iBestOptimalMatchPoint )
			{
				iBestOptimalMatchPoint = iOptimalMatchPoint;
                spSelectedWaitList = spWaitList;
			}
		}

		// 선택된 최적의 매치 입장 유저가 없다면 패스!
		if( spSelectedWaitList == NULL )
			continue;

		// 매치에 입장할 유저 정보 정리!
		std::vector< KMatchUserInfo > vecEnterMatchUser;
		spSelectedWaitList->GetMatchUserInfoList( vecEnterMatchUser );

		// 대기자 리스트에서 제거!
		LIF( UnRegMatchWaitListByWaitNumber( spSelectedWaitList->GetMatchWaitNumber(), NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 ) );

		// 매치에 입장!
		if( spMatch->Enter( vecEnterMatchUser ) == false )
		{
			START_LOG( cerr, L"여기까지 왔는데 입장 실패됐다. 먼가 잘못되었군.." )
				<< BUILD_LOG( spMatch->GetMatchUID() )
				<< BUILD_LOG( vecEnterMatchUser.size() )
				<< END_LOG;
			continue;
		}

		// 최종적으로 매치가 성사되었는지 확인하자!
		if( spMatch->CheckMatchMakingSuccess() == false )
		{
            START_LOG( clog, L"매치 성사 실패!" )
				<< BUILD_LOG( spMatch->GetMatchUID() )
				<< END_LOG;
		}
		else
		{
			// 재 매치 성공 카운트!
			++m_iRematchCount;
		}
	}
}
#endif SERV_2012_PVP_SEASON2
//}}

void KMatchMakingManager::MatchMakingForWaitUser( IN const SEnum::MATCH_TYPE eType )
{
	//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	// 필요로 하는 함수 내부에서 사용한다.
	const int MATCH_MAKING_BEGIN_COUNT = SiKPvpMatchManager()->GetMatchMakingBeginCount();
#else
	//////////////////////////////////////////////////////////////////////////	
	// 그다음은 남은 대기자들을 가지고 새로운 매치를 만들어내자!	
	const int MATCH_TOTAL_PLAYER_COUNT = KMatch::GetMatchTotalPlayerCount( eType );
	const int MATCH_MAKING_BEGIN_COUNT = SiKPvpMatchManager()->GetMatchMakingBeginCount();
	const int CHECK_WAIT_USER_COUNT = SiKPvpMatchManager()->GetCheckWaitUserCount();
	const int GAP_MIN_MAX_RATING = SiKPvpMatchManager()->GetGapMinMaxRating();
#endif SERV_2012_PVP_SEASON2
	//}}

	// 대기자 리스트를 복사해오자!
	std::list< KMatchUserWaitListPtr > listMatchMakingWaitList = m_listMatchWaitList[eType];	

	// 대기 순으로 매치 메이킹을 한다!
	const u_int uiTotalListSize = m_listMatchWaitList[eType].size();
	u_int uiLoopCount = 0;

	while( listMatchMakingWaitList.empty() == false )
	{
		++uiLoopCount;
		if( uiLoopCount > uiTotalListSize )
		{
			START_LOG( cerr, L"잘못된 동작을 하였습니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( uiLoopCount )
				<< BUILD_LOG( uiTotalListSize )
				<< END_LOG;
			break;
		}


		//////////////////////////////////////////////////////////////////////////
		// 1. 매치를 만들수 있는 인원수보다 대기자수가 적을 경우 더이상 체크할 필요가 없다!
		//////////////////////////////////////////////////////////////////////////
		if( (int)listMatchMakingWaitList.size() < MATCH_MAKING_BEGIN_COUNT )
			break;


		//////////////////////////////////////////////////////////////////////////		
		// 2. 대기자 리스트에서 젤 처음것을 하나 빼낸다.
		//////////////////////////////////////////////////////////////////////////		
		KMatchUserWaitListPtr spFrontMatchWaitUser = listMatchMakingWaitList.front();
		listMatchMakingWaitList.pop_front();
		if( spFrontMatchWaitUser == NULL )
		{
			START_LOG( cerr, L"매치 대기자 리스트 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}


		//////////////////////////////////////////////////////////////////////////		
		// 3. 매치 메이킹을 시작해도 되는지 검사!
		//////////////////////////////////////////////////////////////////////////		
		if( spFrontMatchWaitUser->IsPossibleMatchMakingBegin() == false )
			continue;

		//{{ 2012. 09. 16	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		// 체크된 매치 인원 수
		/*
		최소 매칭 인원 제한 4명으로 설정한 뒤 4명의 인원이 매칭을 신청하면, 당연히 4명 중에서 매칭이 이루어질 거라고
		생각했지만, 다음과 같은 부분 때문에 사실은 가장 마지막에 신청한 유저를 제외하고 매칭하도록 동작하고 있었다.

		// 매치 메이킹 준비가 되었는지 확인한다!
		if( spTargetUserList->IsPossibleMatchMakingBegin() == false )
			continue;

		매치 메이킹 신청 후 일정 시간이 지나야만 매칭 후보에 들어갈 수 있다는 조건이다.
		*/
		std::vector< int > vecSelectedWaitUserList;
		if( FindTheOpponent( eType, spFrontMatchWaitUser, listMatchMakingWaitList, vecSelectedWaitUserList ) == false )
		{
			continue;
		}
#else
		//////////////////////////////////////////////////////////////////////////		
		// 4. 매치 대기자 리스트의 멤버가 모두 F랭크인지 검사!
		//////////////////////////////////////////////////////////////////////////		
		const bool bIsAllMemberBeginner = spFrontMatchWaitUser->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F );


		//////////////////////////////////////////////////////////////////////////
		// 4. 최적의 매치 조건 순으로 정렬한다.
		//////////////////////////////////////////////////////////////////////////
		KCompareLimitList< KMatchUserWaitListPtr > listOptimalMatchWaitList( CHECK_WAIT_USER_COUNT ); // 최적의 대기자 리스트를 저장!
		int iCheckListIndex = 0;
		
		BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spTargetUserList, listMatchMakingWaitList )
		{
			// 요만큼만 체크하자!
			if( iCheckListIndex > CHECK_WAIT_USER_COUNT )
				break;

			++iCheckListIndex;

			if( spTargetUserList == NULL )
			{
				START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다." )
					<< END_LOG;
				continue;
			}

			// 매치 메이킹 준비가 되었는지 확인한다!
			if( spTargetUserList->IsPossibleMatchMakingBegin() == false )
				continue;

			// 매치 대기자 모두 F랭크면 다른 유저도 F랭크로만 구성된 대기자 리스트를 매치 시킨다!
			if( bIsAllMemberBeginner )
			{
				if( spTargetUserList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == false )
                    continue;
			}
			else
			{
				if( spTargetUserList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == true )
					continue;
			}

			int iOptimalMatchPoint = 0;

			// 최적의 매치 조건인지 체크!
			if( spFrontMatchWaitUser->CheckOptimalMatch( spTargetUserList, iOptimalMatchPoint ) == false )
				continue;

            // 그렇다면 저장!
			listOptimalMatchWaitList.IfUpperValuePushBack( iOptimalMatchPoint, spTargetUserList );
		}


		//////////////////////////////////////////////////////////////////////////		
		// 5. 정렬된 리스트를 바탕으로 매치 인원에 맞추어 추려내자!
		//////////////////////////////////////////////////////////////////////////		
		std::list< KMatchUserWaitListPtr > listSortedWaitUserList;
		listOptimalMatchWaitList.GetValueList( listSortedWaitUserList );


		// 체크된 매치 인원 수		
		std::vector< int > vecSelectedWaitUserList;
		int iSumMatchPlayerCount = spFrontMatchWaitUser->GetNumMember();
		int iMinRating = spFrontMatchWaitUser->GetRatingAverage();
		int iMaxRating = spFrontMatchWaitUser->GetRatingAverage();
		
		BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spTargetUserList, listSortedWaitUserList )
		{
			if( spTargetUserList == NULL )
			{
				START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다." )
					<< END_LOG;
				continue;
			}

			// 사람수에 맞는지 체크!
			if( ( iSumMatchPlayerCount + spTargetUserList->GetNumMember() ) > MATCH_TOTAL_PLAYER_COUNT )
				continue;

			iSumMatchPlayerCount += spTargetUserList->GetNumMember();
			vecSelectedWaitUserList.push_back( spTargetUserList->GetMatchWaitNumber() );

			if( iMinRating > spTargetUserList->GetRatingAverage() )
				iMinRating = spTargetUserList->GetRatingAverage();

			if( iMaxRating < spTargetUserList->GetRatingAverage() )
				iMaxRating = spTargetUserList->GetRatingAverage();

			// 모든 인원이 다 채워졌으면 이탈!
			if( iSumMatchPlayerCount == MATCH_TOTAL_PLAYER_COUNT )
				break;
		}


		//////////////////////////////////////////////////////////////////////////
		// 6. 모든 인원이 준비되었는지 인원수를 체크하자!
		//////////////////////////////////////////////////////////////////////////
		if( iSumMatchPlayerCount < MATCH_TOTAL_PLAYER_COUNT )
		{
			continue;
		}


		//////////////////////////////////////////////////////////////////////////
		// 7. 매치 대상자들의 최소 rating과 최대 rating간의 차이가 너무 큰건 아닌지 체크한다!
		//////////////////////////////////////////////////////////////////////////
		const int iGapMinMaxRating = ::abs( iMaxRating - iMinRating );
		if( iGapMinMaxRating > GAP_MIN_MAX_RATING )
		{
			continue;
		}
#endif SERV_2012_PVP_SEASON2
		//}}

		//////////////////////////////////////////////////////////////////////////
		// 8. 최적의 매치 조건 순으로 인원수를 맞추어 하나의 매치를 완성시킨다.
		//////////////////////////////////////////////////////////////////////////
		{
			std::vector< KMatchUserInfo > vecSourceUserInfo;
			spFrontMatchWaitUser->GetMatchUserInfoList( vecSourceUserInfo );

			// 대기자 리스트에서 매치 요청자 삭제!
			if( UnRegMatchWaitListByWaitNumber( spFrontMatchWaitUser->GetMatchWaitNumber(), NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 ) == false )
			{
				START_LOG( cerr, L"대기자 리스트에서 매치 요청자 삭제 실패! 일어 나서는 안되는 에러!" )
					<< BUILD_LOG( eType )
					<< BUILD_LOG( spFrontMatchWaitUser->GetMatchWaitNumber() )
					<< END_LOG;
				break;
			}

			// 새로운 매치 생성!
			KMatchPtr spNewMatch = CreateMatch( eType, vecSourceUserInfo );
			if( spNewMatch == NULL )
			{
				START_LOG( cerr, L"새로운 매치 생성 실패!" )
					<< BUILD_LOG( eType )
					<< END_LOG;
				break;
			}

			// 적합한 유저들 매치에 입장 처리!
			BOOST_TEST_FOREACH( const int, iMatchWaitNumber, vecSelectedWaitUserList )
			{
                KMatchUserWaitListPtr spWaitList = GetMatchWaitListByWaitNumber( iMatchWaitNumber );
				if( spWaitList == NULL )
				{
					START_LOG( cerr, L"대기자 리스트가 존재하지 않는다! 여기와서 이러면 어떻해.. 일어나면 안되는 에러!" )
						<< BUILD_LOG( eType )
						<< BUILD_LOG( iMatchWaitNumber )
						<< END_LOG;
					break;
				}

				std::vector< KMatchUserInfo > vecNewEnterUser;
				spWaitList->GetMatchUserInfoList( vecNewEnterUser );

				// 대기자 리스트에서 삭제하자!
				if( UnRegMatchWaitListByWaitNumber( iMatchWaitNumber, NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 ) == false )
				{
					START_LOG( cerr, L"대기자 리스트에서 매치 요청자 삭제 실패!" )
						<< BUILD_LOG( eType )
						<< BUILD_LOG( spWaitList->GetMatchWaitNumber() )
						<< END_LOG;
					break;
				}

				// 매치에 입장 시키자!
				if( spNewMatch->Enter( vecNewEnterUser ) == false )
				{
					START_LOG( cerr, L"추가인원 입장 실패! 정상에러인가?" )
						<< BUILD_LOG( vecNewEnterUser.size() )
						<< END_LOG;
					break;
				}

				// 뽑아놓은 리스트에서 삭제
				std::list< KMatchUserWaitListPtr >::iterator litD = listMatchMakingWaitList.begin();
				for( ; litD != listMatchMakingWaitList.end(); ++litD )
				{
					if( (*litD) == NULL )
					{
						START_LOG( cerr, L"포인터값이 이상합니다." )
							<< END_LOG;
						continue;
					}

					if( (*litD)->GetMatchWaitNumber() == iMatchWaitNumber )
						break;
				}

				if( litD != listMatchMakingWaitList.end() ) 
				{
					listMatchMakingWaitList.erase( litD );
				}
			}

			// 최종적으로 매치가 성사되었는지 확인하자!
			if( spNewMatch->CheckMatchMakingSuccess() == false )
			{
				START_LOG( clog, L"매칭이 실패했다!" )
					<< BUILD_LOG( spNewMatch->GetMatchUID() )
					<< END_LOG;
			}
		}
	}
}

void KMatchMakingManager::ProcessEvent( IN const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
        CASE( EGB_REQUEST_MATCH_MAKING_REQ );
		CASE( EGB_CANCEL_MATCH_MAKING_REQ );	   
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
		CASE_NOPARAM( EGB_CHECK_INVALID_MATCH_NOT );
		CASE( EGB_MATCH_MAKING_ADMIN_COMMAND_NOT );

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EGB_REQUEST_MATCH_MAKING_REQ )
{
	// 매치 메이킹!
	KEGS_REQUEST_MATCH_MAKING_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	SEnum::MATCH_TYPE eMatchType;
	if( KMatch::GetMatchTypeByMatchInfo( kPacket_.m_cPvpGameType, kPacket_.m_cPvpPlayerCount, eMatchType ) == false )
	{
		START_LOG( cerr, L"잘못된 매치 요청 정보입니다!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_15;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_MATCH_MAKING_ACK, kPacket );
		return;
	}

	// 매치 대기자 리스트에 등록!
	if( RegMatchWaitList( eMatchType, kPacket_.m_vecMatchUserInfo, kPacket_.m_bRegFrontOfList ) == false )
	{
		START_LOG( cerr, L"매치 대기자 리스트에 등록이 실패하였습니다!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( eMatchType )
			<< BUILD_LOG( kPacket_.m_vecMatchUserInfo.size() )
			<< BUILD_LOG( kPacket_.m_bRegFrontOfList )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_15;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_MATCH_MAKING_ACK, kPacket );
		return;
	}

	// 결과 전송
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_MATCH_MAKING_ACK, kPacket );

	// 매치 성공에 대해서 알림
	BOOST_TEST_FOREACH( const KMatchUserInfo&, kInfo, kPacket_.m_vecMatchUserInfo )
	{
		KEGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT kPacketNot;
		kPacketNot.m_iEstimatedTime = m_kCompleteTimeManager[eMatchType].GetMatchMakingEstimatedTime();
		SendToGSCharacter( kInfo.m_iGSUID, kInfo.m_iUnitUID, EGB_REG_MATCH_WAIT_LIST_SUCCESS_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_REQ )
{	
	// 파티 소속이면 같은 파티원도 함께 등록 해제!
	if( kPacket_.m_iPartyUID != 0 )
	{
		if( UnRegMatchWaitListByPartyUID( kPacket_.m_iPartyUID, kPacket_.m_iReason, FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"PartyUID로 대기자 리스트에 삭제 실패!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< END_LOG;
		}
	}

	// 대기자 리스트에 등록되어 있는지 확인한다!
	const bool bExistInWaitList = IsExistInWaitListByUnitUID( FIRST_SENDER_UID );
	if( bExistInWaitList == true )
	{
        if( UnRegMatchWaitListByUnitUID( FIRST_SENDER_UID, kPacket_.m_iReason ) == false )
		{
			START_LOG( cerr, L"대기자 리스트에 삭제 실패!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}

		if( kPacket_.m_iPartyUID != 0 )
		{
			START_LOG( cerr, L"이미 PartyUID로 삭제 했을텐데.. 아직 있네? 머지?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< END_LOG;
		}
		
		if( kPacket_.m_iMatchUID != 0 )
		{
			START_LOG( cerr, L"대기자 리스트에 등록되어있는데 MatchUID값이 유효하다! 오류인듯?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iMatchUID )
				<< END_LOG;
		}
	}

	// 매치 정보가 존재하지 않는다면
	if( kPacket_.m_iMatchUID == 0 )
	{
		// 시스템이 보낸 패킷인 경우 ACK 전송 안함.
		if( kPacket_.m_bSendBySystem == true )
			return;

		KEGS_CANCEL_MATCH_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_MATCH_MAKING_ACK, kPacket );
		return;
	}

	KMatchPtr spMatch = GetMatch( kPacket_.m_iMatchUID );
	if( spMatch != NULL )
	{
		// 매치에 event전달
		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };
		spEvent->SetData( PI_GLOBAL_MATCH_MAKING, anTrace, EGB_CANCEL_MATCH_MAKING_REQ, kPacket_ );
		spMatch->QueueingEvent( spEvent );
	}
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_PVP_MATCH_MANAGER, KPvpMatchManager );

	default:
		{
			START_LOG( cerr, L"이쪽으로 오면 안되는 타입인데?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}
}

IMPL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_MATCH_NOT )
{
    START_LOG( cout, L"GlobalUser객체가 소멸되어 현재 매치 신청 또는 매치들을 모두 삭제처리합니다!" )	
		<< END_LOG;

	ClearAllMatchInfo();
}

IMPL_ON_FUNC( EGB_MATCH_MAKING_ADMIN_COMMAND_NOT )
{
    START_LOG( cout, L"개발자 커맨드에 의한 동작 시작!" )
		<< BUILD_LOGc( kPacket_.m_cType );

	switch( kPacket_.m_cType )
	{
	case KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT::CT_DUMP:		
		{

		}
		break;

	case KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT::CT_CLEAR:
		{
            ClearAllMatchInfo();
		}
		break;

	default:
		{
			START_LOG( cerr, L"잘못된 커맨드 입니다!" )
				<< BUILD_LOGc( kPacket_.m_cType )
				<< END_LOG;
		}
		break;
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}



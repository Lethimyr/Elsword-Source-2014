#include "AutoPartyMakingProcess.h"
#include "NetError.h"

#include "GlobalServer.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "X2Data/XSLRoom.h"
#include "AutoPartyFSM.h"
#include "AutoPartyUserFSM.h"
//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#else
	#include "AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}

#include "X2Data/XSLDungeon.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM



class KAutoPartyUidDispenser
{
public:
	KAutoPartyUidDispenser()
	{
		m_iLastUID = 0;
	}
	~KAutoPartyUidDispenser()
	{
	}

	UidType GetNewAutoPartyUID()
	{
		return ++m_iLastUID;
	}

private:
	UidType		m_iLastUID;
};


static KAutoPartyUidDispenser g_autoPartyUidDispenser;


//////////////////////////////////////////////////////////////////////////
KAutoPartyCompleteTimeManager::KAutoPartyCompleteTimeManager()
{
}

KAutoPartyCompleteTimeManager::~KAutoPartyCompleteTimeManager()
{
}

//{{ 던전 자동 매칭 예상 시간 수정 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
void KAutoPartyCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	KAutoPartyTime kTemp;
	kTemp.m_tAutoPartyTime = CTime::GetCurrentTime();
	kTemp.m_iWatiTime = iCompleteTime;	

	m_listCompleteTime.push_back( kTemp );

	// 12개가 될때까지 옛날것을 지운다.
	while( CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KAutoPartyCompleteTimeManager::GetAutoPartyMakingEstimatedTime() const
{
	// 한번도 매치가 성공한적이 없다
	if( m_listCompleteTime.empty() )
		return 2000; // 클라이언트에서 30분 이상이면 '알수 없음'으로 표시한다

	int iSumCompleteTime = 0;												// 최근 성공 대기 시간 총 합
	int iEstimatedTime	 = 0;												// 최근 성공 대기 시간 평균
	int	iSumCount		 = 0;												// 계산 추가된 횟수
	CTime tCurr = CTime::GetCurrentTime();									// 현재 시간
	CTime tTwoHourAgo = tCurr - CTimeSpan( 0, 2, 0, 0 );					// 현재 시간에서 두시간 전 시간

	BOOST_TEST_FOREACH( const KAutoPartyTime, kCompleteTime, m_listCompleteTime )
	{
		if( tTwoHourAgo <= kCompleteTime.m_tAutoPartyTime )	// 최근 2시간 이내 라면
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

void KAutoPartyCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	m_listCompleteTime.push_back( iCompleteTime );

	// 5개가 될때까지 옛날것을 지운다.
	while( COMPLETE_TIME_ENUM::CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KAutoPartyCompleteTimeManager::GetAutoPartyMakingEstimatedTime() const
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
#endif SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
//}}



bool KAutoPartyMakingProcess::GetListTypeByLevelCondition( IN const u_char ucLevel, OUT int& iListType ) const
{
	iListType = 0;

	BOOST_TEST_FOREACH( const SLevelCondition&, sCondition, m_vecLevelCondition )
	{
		if( sCondition.m_iBeginLevel > ucLevel  ||  sCondition.m_iEndLevel < ucLevel )
			continue;
        
		iListType = sCondition.m_iListType;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

UidType KAutoPartyMakingProcess::ExtractDungeonIDAtWaitNumber( IN const UidType iUID_ )
{
    return ( iUID_ >> 40 ) & ( UidType )0x0000000000ffffff;
}

UidType KAutoPartyMakingProcess::ExtractDungeonModeAtWaitNumber( IN const UidType iUID_ )
{
	return ( iUID_ >> 32 ) & ( UidType )0x00000000000000ff;
}

void KAutoPartyMakingProcess::SetDungeonIDAtWaitNumber( UidType& iDestUID_, IN const UidType iSrcUID_ )
{
	iDestUID_ = ( iDestUID_ & 0x000000ffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x0000000000ffffff ) << 40 ) & 0xffffff0000000000 );
}

void KAutoPartyMakingProcess::SetDungeonModeAtWaitNumber( UidType& iDestUID_, IN const UidType iSrcUID_ )
{
	iDestUID_ = ( iDestUID_ & 0xffffff00ffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x00000000000000ff ) << 32 ) & 0x000000ff00000000 );
}

//////////////////////////////////////////////////////////////////////////


FSMclassPtr KAutoPartyMakingProcess::ms_spUserFSM = KAutoPartyUserFSMPtr( new KAutoPartyUserFSM );

KAutoPartyMakingProcess::KAutoPartyMakingProcess( IN const KAutoPartyMakingProcessCondition& kInfo ) :
m_iLastWaitNumber( 0 ),

m_iDungeonMode( kInfo.m_iDungeonMode ),
m_bActivatedAutoPartyBonus( false )
{
	if( m_iDungeonMode == CXSLDungeon::DM_SECRET_NORMAL || m_iDungeonMode == CXSLDungeon::DM_SECRET_HELL )
	{
		// 비던
		m_iDungeonID = kInfo.m_iDungeonIDWithDif;
		m_iDifficultyLevel = 0;
	}
	else
	{	
		// 일반 던전
		m_iDungeonID = ( ( kInfo.m_iDungeonIDWithDif / 10 ) * 10 );
		m_iDifficultyLevel = ( kInfo.m_iDungeonIDWithDif % 10 );
	}

	AddLevelCondition( kInfo.m_iBeginLevel, kInfo.m_iEndLevel );

	// 최초로 생성되는 시점에 LastWaitNumber값을 처리한다.
	SetDungeonIDAtWaitNumber( m_iLastWaitNumber, m_iDungeonID + m_iDifficultyLevel );
	SetDungeonModeAtWaitNumber( m_iLastWaitNumber, m_iDungeonMode );
}

KAutoPartyMakingProcess::~KAutoPartyMakingProcess()
{
}

void KAutoPartyMakingProcess::ClearAllAutoPartyInfo()
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout2, L"[경고!] 모든 자동 파티 정보를 초기화 시킵니다!" );
	//////////////////////////////////////////////////////////////////////////	
	
	//////////////////////////////////////////////////////////////////////////
	// 대기자 리스트 종료 처리
	std::vector< UidType > vecWaitNumberLIst;
	std::map< UidType, KAutoPartyUserWaitListPtr >::const_iterator mit;	
	for( mit = m_mapAutoPartyWaitList.begin(); mit != m_mapAutoPartyWaitList.end(); ++mit )
	{
		vecWaitNumberLIst.push_back( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iWaitNumber, vecWaitNumberLIst )
	{
		UnRegAutoPartyWaitListByWaitNumber( iWaitNumber, NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01 );
	}
}

void KAutoPartyMakingProcess::Tick()
{
	// 현재 보너스 지급 상태인데
	if( IsActivatedAutoPartyBonus() )
	{
		// 보너스 시간이 아니다!
		if( IsAutoPartyBonusTime() == false )
		{
			// 보너스 지급을 정지!
			SetActivatedAutoPartyBonus( false );

			// 모든 서버에게 알린다.
			KEGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT kPacketNot;
			kPacketNot.m_iDungeonIDWithDif = GetDungeonIDWithDif();
			kPacketNot.m_cDungeonMode = GetDungeonMode();
			kPacketNot.m_bActivatedBonus = IsActivatedAutoPartyBonus();
			BroadCastGameServer( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, kPacketNot );
		}
	}
	// 현재 보너스 지급 상태가 아닌데
	else
	{
		// 보너스 지급 시간이다!
		if( IsAutoPartyBonusTime() )
		{
			// 보너스 지급을 시작!
			SetActivatedAutoPartyBonus( true );

			// 모든 서버에게 알린다
			KEGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT kPacketNot;
			kPacketNot.m_iDungeonIDWithDif = GetDungeonIDWithDif();
			kPacketNot.m_cDungeonMode = GetDungeonMode();
			kPacketNot.m_bActivatedBonus = IsActivatedAutoPartyBonus();
			BroadCastGameServer( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, kPacketNot );
		}
	}
}

void KAutoPartyMakingProcess::AddLevelCondition( IN const int iBeginLevel, IN const int iEndLevel )
{
    SLevelCondition sInfo;
	sInfo.m_iBeginLevel = iBeginLevel;
	sInfo.m_iEndLevel = iEndLevel;
	sInfo.m_iListType = static_cast<int>(m_vecLevelCondition.size()); // 현재 컨테이너의 size값이 ListType이 된다.
	m_vecLevelCondition.push_back( sInfo );
}

//{{ 2012. 11. 21	최육사		대기인원 표시 레벨 구간으로 수정
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
int	KAutoPartyMakingProcess::GetWaitUserCountByListType( IN const int iListType ) const
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapAutoPartyWaitUserCount.find( iListType );
	if( mit == m_mapAutoPartyWaitUserCount.end() )
	{		
		return 0;
	}

	return mit->second;
}

int	KAutoPartyMakingProcess::GetWaitUserCountByWaitNumber( IN const UidType iWaitNumber )
{
    KAutoPartyUserWaitListPtr spWaitList = GetAutoPartyWaitListByWaitNumber( iWaitNumber );
	if( IS_NULL( spWaitList ) )
	{
		START_LOG( cwarn, L"존재 하지 않는 대기리스트 입니다." )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;
		return 0;
	}

	return GetWaitUserCountByListType( spWaitList->GetListType() );
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
//}}

// WaitNumber
UidType	KAutoPartyMakingProcess::NewWaitNumber()
{
    return ++m_iLastWaitNumber;
}

//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
KAutoPartyPtr KAutoPartyMakingProcess::MakeAutoParty( IN const KAutoPartyConditionInfo& kInfo )
{
	KAutoPartyConditionInfo kConditionInfo;
	kConditionInfo.m_iDungeonID = kInfo.m_iDungeonID;
	kConditionInfo.m_cDifficultyLevel = kInfo.m_cDifficultyLevel;
	kConditionInfo.m_cGetItemType = static_cast<char>(CXSLRoom::DGIT_PERSON);
	kConditionInfo.m_cDungeonMode = kInfo.m_cDungeonMode;
	kConditionInfo.m_cLimitPlayerCount = SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	const UidType iAutoPartyUID = g_autoPartyUidDispenser.GetNewAutoPartyUID();

	KAutoPartyPtr spAutoParty( new KAutoParty( iAutoPartyUID, kConditionInfo ) );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"KAutoParty new 할당실패." )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// FSM설정
	FSMclassPtr spFSMClass = boost::shared_ptr< KAutoPartyFSM >( new KAutoPartyFSM );
	spAutoParty->SetFSM( spFSMClass );
	return spAutoParty;
}
#else
KAutoPartyPtr KAutoPartyMakingProcess::MakeAutoParty()
{
	KAutoPartyConditionInfo kConditionInfo;
	kConditionInfo.m_iDungeonID = GetDungeonID();
	kConditionInfo.m_cDifficultyLevel = GetDifficultyLevel();
	kConditionInfo.m_cGetItemType = static_cast<char>(CXSLRoom::DGIT_PERSON);
	kConditionInfo.m_cDungeonMode = GetDungeonMode();
	kConditionInfo.m_cLimitPlayerCount = SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	const UidType iAutoPartyUID = g_autoPartyUidDispenser.GetNewAutoPartyUID();

	KAutoPartyPtr spAutoParty( new KAutoParty( iAutoPartyUID, kConditionInfo ) );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"KAutoParty new 할당실패." )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// FSM설정
	FSMclassPtr spFSMClass = boost::shared_ptr< KAutoPartyFSM >( new KAutoPartyFSM );
	spAutoParty->SetFSM( spFSMClass );
	return spAutoParty;
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

KAutoPartyUserPtr KAutoPartyMakingProcess::MakeUser( IN const KAutoPartyUserInfo& kInfo )
{
	KAutoPartyUserPtr  spAutoPartyUser( new KAutoPartyUser( kInfo ) );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"자동 파티 유저 생성 실패!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr();
	}

	spAutoPartyUser->SetFSM( ms_spUserFSM );
	return spAutoPartyUser;
}

KAutoPartyUserPtr KAutoPartyMakingProcess::GetAutoPartyWaitUserByUnitUID( IN const UidType iUnitUID )
{
	std::map< UidType, KAutoPartyUserPtr  >::iterator mit = m_mapAutoPartyWaitUser.find( iUnitUID );
	if( mit == m_mapAutoPartyWaitUser.end() )
	{
		START_LOG( cwarn, L"찾는 매치유저가 없습니다.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr ();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 유저 포인터 이상.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr();
	}

	return mit->second;
}

KAutoPartyUserWaitListPtr KAutoPartyMakingProcess::GetAutoPartyWaitListByWaitNumber( IN const UidType iWaitNumber )
{
	std::map< UidType, KAutoPartyUserWaitListPtr >::iterator mit = m_mapAutoPartyWaitList.find( iWaitNumber );
	if( mit == m_mapAutoPartyWaitList.end() )
	{
		START_LOG( cwarn, L"찾는 매치 대기자 리스트가 없습니다.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KAutoPartyUserWaitListPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 매치 대기자 리스트 포인터 이상.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KAutoPartyUserWaitListPtr();
	}

	return mit->second;
}

//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
KAutoPartyPtr KAutoPartyMakingProcess::CreateAutoParty( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo, IN const KAutoPartyConditionInfo& kInfo )
{
	KAutoPartyPtr spAutoParty = MakeAutoParty( kInfo );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"매치 생성 실패!" )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// 자동 파티 정보 초기화
	spAutoParty->Enter( vecAutoPartyUserInfo );

	// 자동 파티가 처음 생성되면 자동 파티 메이킹 상태이다!
	spAutoParty->StateTransition( KAutoPartyFSM::I_TO_AUTO_PARTY_MAKING );
	return spAutoParty;
}
#else
KAutoPartyPtr KAutoPartyMakingProcess::CreateAutoParty( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo )
{
	KAutoPartyPtr spAutoParty = MakeAutoParty();
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"매치 생성 실패!" )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	// 자동 파티 정보 초기화
	spAutoParty->Enter( vecAutoPartyUserInfo );

	// 자동 파티가 처음 생성되면 자동 파티 메이킹 상태이다!
	spAutoParty->StateTransition( KAutoPartyFSM::I_TO_AUTO_PARTY_MAKING );
	return spAutoParty;
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
bool KAutoPartyMakingProcess::RegAutoPartyWaitList( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList, 
												    IN const KAutoPartyConditionInfo& kAutoPartyCondition, 
													OUT UidType& iWaitNumber, 
													IN const bool bRegFrontOfList /*= false*/ )
#else
bool KAutoPartyMakingProcess::RegAutoPartyWaitList( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList, 
												    OUT UidType& iWaitNumber, 
												    IN const bool bRegFrontOfList /*= false*/ )
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}
{
	// 초기화
	iWaitNumber = 0;

	// 1. 자동 파티 인원 수가 맞는지 체크!
	if( SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT < (int)vecAutoPartyUserList.size() )
	{
		START_LOG( cerr, L"매치 대기자 리스트 생성 실패!" )
			<< BUILD_LOG( SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
			<< BUILD_LOG( vecAutoPartyUserList.size() )
			<< END_LOG;
		return false;
	}

	// 2. 자동 파티 대기자 리스트를 생성 합니다.
	KAutoPartyUserWaitListPtr spWaitList( new KAutoPartyUserWaitList( NewWaitNumber() ) );
	if( spWaitList == NULL )
	{
		START_LOG( cerr, L"매치 대기자 리스트 생성 실패!" )
			<< END_LOG;
		return false;
	}
	
	// 3. 자동 파티 대기자 리스트에 신청자들을 하나씩 넣습니다!
	BOOST_TEST_FOREACH( const KAutoPartyUserInfo&, kInfo, vecAutoPartyUserList )
	{
		// 이미 등록된 캐릭터 인지 검사
		if( IsExistInWaitListByUnitUID( kInfo.m_iUnitUID ) == true )
		{
			START_LOG( cerr, L"이미 매치에 등록되어있는 유저!" )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

        KAutoPartyUserPtr spAutoPartyUser = MakeUser( kInfo );
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 생성 중 매치 유저 생성 실패!" )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

		// 대기자 리스트에 자동 파티 유저를 등록하자!
		spWaitList->AddAutoPartyUser( spAutoPartyUser );

		// 전체 유저 리스트에도 등록하자!
		m_mapAutoPartyWaitUser.insert( std::make_pair( spAutoPartyUser->GetCharUID(), spAutoPartyUser ) );
	}

	if( spWaitList->GetNumMember() == 0 )
	{
		START_LOG( cerr, L"대기자 리스트를 생성하지 못하였습니다." )
			<< END_LOG;
		return false;
	}

	// 4. ListType값을 구하자!
	int iListType = 0;
	//{{ 2012. 12. 04 던전 매칭 레벨 평균 레벨로 변경 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_LEVEL
	if( GetListTypeByLevelCondition( spWaitList->GetAverageUnitLevel(), iListType ) == false )
	{
		START_LOG( cwarn, L"리스트 타입을 구하지 못했습니다!" )			
			<< BUILD_LOGc( spWaitList->GetAverageUnitLevel() )
			<< BUILD_LOGc( GetDungeonMode() )
			<< BUILD_LOG( GetDungeonIDWithDif() )
			<< END_LOG;

		// 일단 0으로 넣자!
		spWaitList->SetListType( 0 );
	}
#else
	if( GetListTypeByLevelCondition( spWaitList->GetLevelForListType(), iListType ) == false )
	{
		START_LOG( cerr, L"리스트 타입을 구하지 못했습니다!" )			
			<< BUILD_LOG( spWaitList->GetLevelForListType() )
			<< BUILD_LOG( GetDungeonMode() )
			<< BUILD_LOG( GetDungeonIDWithDif() )
			<< END_LOG;

		// 일단 0으로 넣자!
		spWaitList->SetListType( 0 );
	}
#endif SERV_AUTO_PARTY_MATCHING_LEVEL
	//}
	else
	{
        spWaitList->SetListType( iListType );
	}

	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	// 5. 자동 파티 신청 조건 저장!
	spWaitList->SetAutoPartyCondition( kAutoPartyCondition );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	// 6. 해당 타입의 대기자리스트에 넣습니다!
	if( bRegFrontOfList )
	{
		// 재신청이라면 리스트 최상위에 넣자!
		m_listAutoPartyWaitList.push_front( spWaitList );
	}
	else
	{
		// 새로운 신청이라면 리스트 최하위에 넣자!
		m_listAutoPartyWaitList.push_back( spWaitList );
	}

	// 7. 대기번호를 key로하는 컨테이너에도 넣습니다!
	m_mapAutoPartyWaitList.insert( std::make_pair( spWaitList->GetWaitNumber(), spWaitList ) );
	
	iWaitNumber = spWaitList->GetWaitNumber();


	// 8. 자동 파티 보너스 적용!
	if( IsActivatedAutoPartyBonus() == true  ||  IsActivatedDelayTimeBonus() == true )
	{
		spWaitList->SetAutoPartyBonus( true );
	}

	//{{ 2012. 11. 21	최육사		대기인원 표시 레벨 구간으로 수정
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	// 9. 대기자 수 카운트를 올리자!
	IncreaseAutoPartyWaitUserCount( spWaitList->GetListType(), spWaitList->GetNumMember() );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}
	
	return true;
}

bool KAutoPartyMakingProcess::UnRegAutoPartyWaitListByUnitUID( IN const UidType iUnitUID, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KAutoPartyUserPtr spAutoPartyUser = GetAutoPartyWaitUserByUnitUID( iUnitUID );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"대기자 리스트 등록해제를 하려고 하는데 유저를 찾지 못하였습니다." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	// 해당 매치 정보 삭제
    m_mapAutoPartyWaitUser.erase( iUnitUID );

	// 대기자 리스트에서도 삭제
	std::list< KAutoPartyUserWaitListPtr >::iterator lit = m_listAutoPartyWaitList.begin();
	for( ; lit != m_listAutoPartyWaitList.end(); ++lit )
	{
		if( (*lit) == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다" )
				<< END_LOG;
			continue;
		}

        if( (*lit)->IsExistAutoPartyUser( iUnitUID ) )
		{
            (*lit)->DelAutoPartyUser( iUnitUID );
			break;
		}
	}

	if( lit != m_listAutoPartyWaitList.end() )
	{
		if( (*lit)->GetNumMember() == 0 )
		{
			const UidType iAutoPartyWaitNumber = (*lit)->GetWaitNumber();
			m_listAutoPartyWaitList.erase( lit );
            m_mapAutoPartyWaitList.erase( iAutoPartyWaitNumber );
		}
	}
	else
	{
		START_LOG( cerr, L"대기자 리스트에 매치유저가 없다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}

	// 취소 처리를 날리자!
	KEGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT kPacketNot;
	kPacketNot.m_iReason = iUnRegReason;
	kPacketNot.m_wstrCancelUnitNickName = wstrCancelUnitNickName;
	spAutoPartyUser->SendToGSCharacter( EGB_UNREG_AUTO_PARTY_WAIT_LIST_NOT, kPacketNot );

	// 자동 파티가 성공해서 대기자 리스트에서 빠지는거라면 자동파티 성공 통계를 남기자!
	if( iUnRegReason == NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 )
	{
		m_kCompleteTimeManager.AddCompleteTime( static_cast<int>(spAutoPartyUser->GetAutoPartyCompleteSeconds()) );
	}
	return true;
}

bool KAutoPartyMakingProcess::UnRegAutoPartyWaitListByWaitNumber( IN const UidType iWaitNumber, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KAutoPartyUserWaitListPtr spAutoPartyWaitList = GetAutoPartyWaitListByWaitNumber( iWaitNumber );
	if( spAutoPartyWaitList == NULL )
	{
		START_LOG( cerr, L"대기자 리스트 등록해제를 하려고 하는데 리스트를 찾지 못하였습니다." )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;
		return false;
	}

	//{{ 2012. 11. 21	최육사		대기인원 표시 레벨 구간으로 수정
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	// 대기자 수 카운트를 내리자!
	DecreaseAutoPartyWaitUserCount( spAutoPartyWaitList->GetListType(), spAutoPartyWaitList->GetNumMember() );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}

	// 캐릭터 리스트를 얻어서 하나씩 등록해제하자!
	std::vector< UidType > vecUnitUIDList;
	spAutoPartyWaitList->GetUnitUIDList( vecUnitUIDList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDList )
	{
		UnRegAutoPartyWaitListByUnitUID( iUnitUID, iUnRegReason, wstrCancelUnitNickName );
	}
	
	return true;
}

void KAutoPartyMakingProcess::MakingAutoPartyProcessing( OUT std::vector< KAutoPartyPtr >& vecMadeAutoParty )
{
	vecMadeAutoParty.clear();

	//////////////////////////////////////////////////////////////////////////	
	// 그다음은 남은 대기자들을 가지고 새로운 매치를 만들어내자!
	const int AUTO_PARTY_MAKING_BEGIN_COUNT = SiKAutoPartyScriptManager()->GetAutoPartyMakingBeginCount();
	const int CHECK_WAIT_USER_COUNT = SiKAutoPartyScriptManager()->GetCheckWaitUserCount();

	// 대기자 리스트를 복사해오자!
	std::list< KAutoPartyUserWaitListPtr > listAutoPartyMakingWaitList = m_listAutoPartyWaitList;

	// 대기 순으로 매치 메이킹을 한다!
	const u_int uiTotalListSize = listAutoPartyMakingWaitList.size();
	u_int uiLoopCount = 0;

	while( listAutoPartyMakingWaitList.empty() == false )
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
		// 1. 자동 파티를 만들수 있는 인원수보다 대기자수가 적을 경우 더이상 체크할 필요가 없다!
		//////////////////////////////////////////////////////////////////////////
		if( (int)listAutoPartyMakingWaitList.size() < AUTO_PARTY_MAKING_BEGIN_COUNT )
			break;


		//////////////////////////////////////////////////////////////////////////
		// 2. 대기자 리스트에서 젤 처음것을 하나 빼낸다.
		//////////////////////////////////////////////////////////////////////////
		KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser = listAutoPartyMakingWaitList.front();
		listAutoPartyMakingWaitList.pop_front();
		if( spFrontAutoPartyWaitUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 대기자 리스트 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}


		//////////////////////////////////////////////////////////////////////////
		// 3. 대기 시간을 얻어 낸다.
		//////////////////////////////////////////////////////////////////////////
		const double fElapsedWaitTime = spFrontAutoPartyWaitUser->ElapsedWaitTime();
		

		//////////////////////////////////////////////////////////////////////////
		// 4. 최적의 조건 순으로 정렬하여 대상자 리스트를 뽑아 낸다.
		//////////////////////////////////////////////////////////////////////////
		KCompareLimitList< KAutoPartyUserWaitListPtr > listOptimalAutoPartyWaitList( CHECK_WAIT_USER_COUNT ); // 최적의 대기자 리스트를 저장!
		
		// 최적의 조건 순으로 CHECK_WAIT_USER_COUNT수 만큼만 리스트를 뽑아 내자!
		AutoPartyProcess_GetWaitUserListByOptimalConditionSort( spFrontAutoPartyWaitUser, fElapsedWaitTime, listAutoPartyMakingWaitList, listOptimalAutoPartyWaitList );


		//////////////////////////////////////////////////////////////////////////
		// 5. 정렬된 리스트를 바탕으로 총 파티 인원에 맞추어 추려내자!
		//////////////////////////////////////////////////////////////////////////
		std::list< KAutoPartyUserWaitListPtr > listSortedWaitUserList;
		listOptimalAutoPartyWaitList.GetValueList( listSortedWaitUserList );

		// 체크된 매치 인원 수
		std::vector< UidType > vecSelectedWaitUserList;
		int iSumAutoPartyPlayerCount;
		//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		char iDifficulty;
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		//}}

		// 자동 파티가 될 대기자번호 리스트를 얻자!
		AutoPartyProcess_GetWaitNumberListForCompleteMakeAutoParty( spFrontAutoPartyWaitUser, 
																	listSortedWaitUserList, 
																	vecSelectedWaitUserList, 
																	iSumAutoPartyPlayerCount
																	//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
															#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																	, iDifficulty
															#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																	//}}
																	);


		//////////////////////////////////////////////////////////////////////////
		// 6. 해당 유저의 대기 시간에 따라 바로 시작할지 파티 모집을 좀 더 기다릴지 결정 한다.
		//////////////////////////////////////////////////////////////////////////

		// 아직 대기시간이 많이 안지났으므로 최대 파티 모집 인원을 체크하자!
		if( fElapsedWaitTime < SiKAutoPartyScriptManager()->GetDirectGameStartWaitTime() )
		{
			//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
			const KAutoPartyConditionInfo& kAutoPartyConditionInfo = spFrontAutoPartyWaitUser->GetAutoPartyCondition();
			int iDungeon = kAutoPartyConditionInfo.m_iDungeonID + static_cast<int>( iDifficulty );

			if( iSumAutoPartyPlayerCount < SiKAutoPartyScriptManager()->GetAutoPartyMatchingSuccessNum( iDungeon ) )
				continue;
#else
			if( iSumAutoPartyPlayerCount < SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
				continue;
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
			//}}
		}
		//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		else
		{
			if( SiKAutoPartyScriptManager()->IsCheckAllMemberDirectGameStartTime() == true )
			{
				// 모집 인원이 있는경우만 체크
				if( vecSelectedWaitUserList.empty() == false )
				{
					// 모집된 파티원들 모두 시간이 지났는지 검사!
					bool bDirectGameStart = false;

					BOOST_TEST_FOREACH( const UidType, iAutoPartyWaitNumber, vecSelectedWaitUserList )
					{
						KAutoPartyUserWaitListPtr spWaitList = GetAutoPartyWaitListByWaitNumber( iAutoPartyWaitNumber );
						if( IS_NULL( spWaitList ) )
						{
							START_LOG( cerr, L"대기자 리스트가 존재하지 않는다! 여기와서 이러면 어떻해.. 일어나면 안되는 에러!" )
								<< BUILD_LOG( iAutoPartyWaitNumber )
								<< END_LOG;
							break;
						}

						if( spWaitList->ElapsedWaitTime() < SiKAutoPartyScriptManager()->GetDirectGameStartWaitTime() )
						{
							bDirectGameStart = false;
							break;
						}

						bDirectGameStart = true;
					}

					// 아직 시간이 안된 유저가 존재한다.
					if( bDirectGameStart == false )
						continue;
				}
			}			
		}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}


		//////////////////////////////////////////////////////////////////////////
		// 7. 최적의 파티 조건 순으로 인원수를 맞추어 하나의 파티를 완성시킨다.
		//////////////////////////////////////////////////////////////////////////
		{
			std::vector< KAutoPartyUserInfo > vecSourceUserInfo;
			spFrontAutoPartyWaitUser->GetAutoPartyUserInfoList( vecSourceUserInfo );

			// 대기자 리스트에서 매치 요청자 삭제!
			if( UnRegAutoPartyWaitListByWaitNumber( spFrontAutoPartyWaitUser->GetWaitNumber(), NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 ) == false )
			{
				START_LOG( cerr, L"대기자 리스트에서 매치 요청자 삭제 실패! 일어 나서는 안되는 에러!" )
					<< BUILD_LOG( spFrontAutoPartyWaitUser->GetWaitNumber() )
					<< END_LOG;
				break;
			}

			// 새로운 자동파티 생성!
			//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			KAutoPartyPtr spNewAutoParty = CreateAutoParty( vecSourceUserInfo, spFrontAutoPartyWaitUser->GetAutoPartyCondition() );
#else
			KAutoPartyPtr spNewAutoParty = CreateAutoParty( vecSourceUserInfo );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			//}}
			if( spNewAutoParty == NULL )
			{
				START_LOG( cerr, L"새로운 매치 생성 실패!" )
					<< END_LOG;
				break;
			}

			// 적합한 유저들 자동 파티에 입장 처리!
			BOOST_TEST_FOREACH( const UidType, iAutoPartyWaitNumber, vecSelectedWaitUserList )
			{
                KAutoPartyUserWaitListPtr spWaitList = GetAutoPartyWaitListByWaitNumber( iAutoPartyWaitNumber );
				if( spWaitList == NULL )
				{
					START_LOG( cerr, L"대기자 리스트가 존재하지 않는다! 여기와서 이러면 어떻해.. 일어나면 안되는 에러!" )
						<< BUILD_LOG( iAutoPartyWaitNumber )
						<< END_LOG;
					break;
				}

				std::vector< KAutoPartyUserInfo > vecNewEnterUser;
				spWaitList->GetAutoPartyUserInfoList( vecNewEnterUser );

				//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
				// 더 높은 난이도를 선택한다!
				if( spNewAutoParty->GetDifficultyLevel() < spWaitList->GetDifficultyLevel() )
				{
					spNewAutoParty->SetDifficultyLevel( spWaitList->GetDifficultyLevel() );
				}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
				//}}

				// 대기자 리스트에서 삭제하자!
				if( UnRegAutoPartyWaitListByWaitNumber( iAutoPartyWaitNumber, NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 ) == false )
				{
					START_LOG( cerr, L"대기자 리스트에서 자동 파티 요청자 삭제 실패!" )
						<< BUILD_LOG( spWaitList->GetWaitNumber() )
						<< END_LOG;
					break;
				}

				// 자동 파티에 입장 시키자!
				if( spNewAutoParty->Enter( vecNewEnterUser ) == false )
				{
					START_LOG( cerr, L"추가인원 입장 실패! 정상에러인가?" )
						<< BUILD_LOG( vecNewEnterUser.size() )
						<< END_LOG;
					break;
				}

				// 뽑아놓은 리스트에서 삭제
				std::list< KAutoPartyUserWaitListPtr >::iterator litD = listAutoPartyMakingWaitList.begin();
				for( ; litD != listAutoPartyMakingWaitList.end(); ++litD )
				{
					if( (*litD) == NULL )
					{
						START_LOG( cerr, L"포인터값이 이상합니다." )
							<< END_LOG;
						continue;
					}

					if( (*litD)->GetWaitNumber() == iAutoPartyWaitNumber )
						break;
				}

				if( litD != listAutoPartyMakingWaitList.end() ) 
				{
					listAutoPartyMakingWaitList.erase( litD );
				}
			}

			// 최종적으로 자동파티가 성사되었는지 확인하자!
			if( spNewAutoParty->CheckAutoPartyMakingSuccess() == false )
			{
				START_LOG( clog, L"자동 파티 성공 처리에 실패했다!" )
					<< BUILD_LOG( spNewAutoParty->GetAutoPartyUID() )
					<< END_LOG;
			}
			else
			{
				// 자동 파티가 생성 성공되었다!
				vecMadeAutoParty.push_back( spNewAutoParty );
			}
		}
	}
}

void KAutoPartyMakingProcess::AutoPartyProcess_GetWaitUserListByOptimalConditionSort( IN const KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser,
																					  IN const double fElapsedWaitTime,
																					  IN const std::list< KAutoPartyUserWaitListPtr >& listAutoPartyMakingWaitList, 																					  
																					  OUT KCompareLimitList< KAutoPartyUserWaitListPtr >& listOptimalAutoPartyWaitList )
{
	// 만약 대기 시간이 동일 유닛 타입 포함 시간보다 크다면 파티 매칭시에 같은 유닛 타입도 매칭 되도록 한다.
	const bool bSameUnitTypeWaitTimeOver = ( fElapsedWaitTime > SiKAutoPartyScriptManager()->GetSameUnitTypeWaitTime() );
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	const bool bSameDifficultyLevelWaitTimeOver = ( fElapsedWaitTime > SiKAutoPartyScriptManager()->GetSameDifficultyLevelWaitTime() );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	const u_int uiMaxSize = listOptimalAutoPartyWaitList.MaxSize();

	BOOST_TEST_FOREACH( KAutoPartyUserWaitListPtr, spTargetUserList, listAutoPartyMakingWaitList )
	{
		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		// (1) 다른 난이도의 매칭에 대한 조건 ( 우선순위가 높다 )
		if( bSameDifficultyLevelWaitTimeOver == false )
		{
			// 다른 난이도라면 매칭 대상에 포함되지 않는다.
			if( spFrontAutoPartyWaitUser->CheckSameDifficultyLevel( spTargetUserList ) == false )
				continue;
		}
		else
		{
			if( SiKAutoPartyScriptManager()->IsCheckAllMemberSameDifficultyLevelTime() == true )
			{
				// 타겟 대기자 리스트도 시간이 지났는가?
				if( spTargetUserList->ElapsedWaitTime() < SiKAutoPartyScriptManager()->GetSameDifficultyLevelWaitTime() )
					continue;
			}
		}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}

		// (2) 같은 ListType인지 검사! ( 레벨 간격 체크 )
		if( spTargetUserList->GetListType() != spFrontAutoPartyWaitUser->GetListType() )
			continue;

		// (3) 다른 타입의 캐릭터 매칭에 대한 조건
		if( bSameUnitTypeWaitTimeOver == false )
		{
			// 같은 유닛 타입이면 대상이 될수 없다!
			if( spFrontAutoPartyWaitUser->CheckSameUnitType( spTargetUserList ) == true )
				continue;
		}
		//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		else
		{
			if( SiKAutoPartyScriptManager()->IsCheckAllMemberSameUnitTypeTime() == true )
			{
				// 타겟 대기자 리스트도 시간이 지났는가?
				if( spTargetUserList->ElapsedWaitTime() < SiKAutoPartyScriptManager()->GetSameUnitTypeWaitTime() )
					continue;
			}
		}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}

		// 그렇다면 저장!
		listOptimalAutoPartyWaitList.PushBack( spTargetUserList );

		// 리스트가 가득 찼는지 확인하자!
		if( listOptimalAutoPartyWaitList.Size() >= uiMaxSize )
			break;
	}
}

void KAutoPartyMakingProcess::AutoPartyProcess_GetWaitNumberListForCompleteMakeAutoParty( IN const KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser,
																						  IN const std::list< KAutoPartyUserWaitListPtr >& listSortedWaitUserList,
																						  OUT std::vector< UidType >& vecSelectedWaitUserList,
																						  OUT int& iSumAutoPartyPlayerCount
																						  //{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
																				#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																						  , OUT char& iDifficulty
																				#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																						  //}}
																						  )
{
	vecSelectedWaitUserList.clear();

	iSumAutoPartyPlayerCount = spFrontAutoPartyWaitUser->GetNumMember();
	//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	iDifficulty = spFrontAutoPartyWaitUser->GetDifficultyLevel();
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

	BOOST_TEST_FOREACH( KAutoPartyUserWaitListPtr, spTargetUserList, listSortedWaitUserList )
	{
		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"대기자 리스트 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		// 총 인원 수가 딱 4명으로 들어맞아야 한다!
		if( ( iSumAutoPartyPlayerCount + spTargetUserList->GetNumMember() ) > SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
			continue;

		iSumAutoPartyPlayerCount += spTargetUserList->GetNumMember();
		vecSelectedWaitUserList.push_back( spTargetUserList->GetWaitNumber() );

		//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		if( iDifficulty < spTargetUserList->GetDifficultyLevel() )
		{
			iDifficulty = spTargetUserList->GetDifficultyLevel();
		}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		//}}

		// 모든 인원이 다 채워졌으면 이탈!
		if( iSumAutoPartyPlayerCount == SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT )
			break;
	}
}

bool KAutoPartyMakingProcess::IsActivatedDelayTimeBonus()
{
	if( m_vecDelayBonusTimer.empty() )
		return false;

	std::vector< boost::timer >::iterator vit;
	for( vit = m_vecDelayBonusTimer.begin(); vit != m_vecDelayBonusTimer.end(); ++vit )
	{
		if( vit->elapsed() < 5.0 )
			return true;
	}
	
	m_vecDelayBonusTimer.clear();
	return false;
}

bool KAutoPartyMakingProcess::IsAutoPartyBonusTime() const
{
	return( m_tGameStartLastTime.elapsed() > SiKAutoPartyScriptManager()->GetAutoPartyBonusTime() );
}

void KAutoPartyMakingProcess::SetActivatedAutoPartyBonus( IN const bool bActivate )
{
	const bool bBefore = m_bActivatedAutoPartyBonus;
	m_bActivatedAutoPartyBonus = bActivate;

	if( bBefore == true  &&  bActivate == false )
	{
		m_vecDelayBonusTimer.push_back( boost::timer() );
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 11. 21	최육사		대기인원 표시 레벨 구간으로 수정
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
void KAutoPartyMakingProcess::IncreaseAutoPartyWaitUserCount( IN const int iListType, IN const int iCount )
{
	std::map< int, int >::iterator mit;
	mit = m_mapAutoPartyWaitUserCount.find( iListType );
	if( mit == m_mapAutoPartyWaitUserCount.end() )
	{
		m_mapAutoPartyWaitUserCount.insert( std::make_pair( iListType, iCount ) );
		return;
	}

	// 증가!
	mit->second += iCount;
}

void KAutoPartyMakingProcess::DecreaseAutoPartyWaitUserCount( IN const int iListType, IN const int iCount )
{
	std::map< int, int >::iterator mit;
	mit = m_mapAutoPartyWaitUserCount.find( iListType );
	if( mit == m_mapAutoPartyWaitUserCount.end() )
	{
		m_mapAutoPartyWaitUserCount.insert( std::make_pair( iListType, 0 ) );
		return;
	}

	// 감소!
	mit->second -= iCount;

	if( mit->second < 0 )
	{
		START_LOG( cerr, L"어떻게 0보다 작게 나올수 있지?" )
			<< BUILD_LOG( mit->second )
			<< END_LOG;

		mit->second = 0;
	}
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
//}}



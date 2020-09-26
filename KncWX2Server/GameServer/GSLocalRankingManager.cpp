#include "GSLocalRankingManager.h"

//{{ 2013. 3. 15	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
#include "GameSysVal/GameSysVal.h"
#include "NetError.h"

#define CLASS_TYPE  KGSLocalRankingManager
ImplPfID( KGSLocalRankingManager, PI_NULL );
ImplementSingleton( KGSLocalRankingManager );

KGSLocalRankingManager::KGSLocalRankingManager()
: m_bInit( false )
, m_iUID( 0 )
{
}

void KGSLocalRankingManager::clear( void )
{
	m_bInit	= false;
	m_iUID	= 0;
	m_kResetTimer.restart();

	m_mapRankerTab.clear();

	m_mapRankerUserInfo.clear();
	m_mapRankerUnitInfo.clear();

	m_mapBackUpUnitInfo.clear();
	m_mapBackUpUIDInfo.clear();
	m_mapBackUpUserInfo.clear();
}

bool KGSLocalRankingManager::Begin( void )
{
	SendToLoginServer( ELG_LOCAL_RANKING_SYSTEM_INFO_CHECK_NOT, m_iUID );
	m_kResetTimer.restart();
	return KThread::Begin();
}

void KGSLocalRankingManager::ShutDown( void )
{
	KThread::End();
}

void KGSLocalRankingManager::Init( IN const int iOption
								 , IN const int iUID
								 , IN const std::map<UidType, KLocalRankingUnitInfo>& mapRankerUnitInfo
								 , IN const std::map<UidType, UidType>& mapRankerUIDInfo
								 , IN const std::map<UidType, KLocalRankingUserInfo>& mapRankerUserInfo
								 )
{
	if( m_iUID != iUID )
	{
		clear();

		// 시스템 정보 세팅
		m_bInit = true;
		m_iUID	= iUID;
	}

	// insert
	m_mapBackUpUnitInfo.insert( mapRankerUnitInfo.begin(), mapRankerUnitInfo.end() );
	m_mapBackUpUIDInfo.insert( mapRankerUIDInfo.begin(), mapRankerUIDInfo.end() );
	m_mapBackUpUserInfo.insert( mapRankerUserInfo.begin(), mapRankerUserInfo.end() );

	if( iOption == NetError::NET_OK )
	{
		// 랭킹 정보 세팅
		std::map<UidType, KLocalRankingUnitInfo>::const_iterator it;
		for( it = m_mapBackUpUnitInfo.begin(); it != m_mapBackUpUnitInfo.end(); ++it )
		{
			std::map<UidType, UidType>::const_iterator itUIDInfo = m_mapBackUpUIDInfo.find( it->first );
			if( itUIDInfo == m_mapBackUpUIDInfo.end() )
			{
				continue;
			}

			std::map<UidType, KLocalRankingUserInfo>::const_iterator itUserInfo = m_mapBackUpUserInfo.find( itUIDInfo->second );
			if( itUserInfo == m_mapBackUpUserInfo.end() )
			{
				continue;
			}

			SomethingChanged( m_iUID, itUserInfo->second, it->second );
		}

		START_LOG( cout, L"로컬 랭킹 시스템 Init" )
			<< BUILD_LOG( m_iUID );
	}
}

void KGSLocalRankingManager::Tick( void )
{
	KPerformer::Tick();

	if( 60.0 < m_kResetTimer.elapsed() )
	{
		if( CheckInit() == true )
		{
			// 랭킹 사이즈 체크 ( 축소 작업 )
			SizeProcess();
		}

		SendToLoginServer( ELG_LOCAL_RANKING_SYSTEM_INFO_CHECK_NOT, m_iUID );

		m_kResetTimer.restart();
	}
}

void KGSLocalRankingManager::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

	switch( spEvent_->m_usEventID )
	{
		_CASE( EGS_LOCAL_RANKING_INQUIRY_REQ, KLocalRankingInquiryData );
		CASE( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_CHECK_NOT );
		CASE( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT );
		CASE( ELG_LOCAL_RANKING_RESET_NOT );
		CASE( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT );

	default:
		dbg::cerr << L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr();
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

void KGSLocalRankingManager::Run( void )
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}
}

void KGSLocalRankingManager::SendToLoginServer( unsigned short usEventID ) const
{
	SendToLoginServer( usEventID, char() );
}

void KGSLocalRankingManager::BroadCastAllGSUser( unsigned short usEventID ) const
{
	BroadCastAllGSUser( usEventID, char() );
}

void KGSLocalRankingManager::SendToGSUser( UidType nTo_, unsigned short usEventID ) const
{
	SendToGSUser( nTo_, usEventID, char() );
}

int KGSLocalRankingManager::CheckChangedInfo( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const
{
	bool bDataChanged = false;

	std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		// 카테고리가 바뀌거나 공개/비공개가 전환될 때 카테고리 정보 처리를 한다.
		if( ( SEnum::GetCategoryIndex( itUser->second.m_kInfo.m_iCategory ) != SEnum::GetCategoryIndex( kUserInfo.m_iCategory ) )
		 || ( SEnum::CheckFlag( itUser->second.m_kInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) != SEnum::CheckFlag( kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) )
		 )
		{
			// 카테고리 정보가 수정되었다.
			return LRM_CATEGORY_CHANGED;
		}
		else
		{
			bDataChanged |= !( itUser->second.m_kInfo == kUserInfo );
		}
	}

	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		if( itUnit->second.m_kInfo.m_iPoint[SEnum::LRMTC_SPIRIT] != kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] )
		{
			return LRM_SPIRIT_CHANGED;
		}
		else if( itUnit->second.m_kInfo.m_iPoint[SEnum::LRMTC_AP] != kUnitInfo.m_iPoint[SEnum::LRMTC_AP] )
		{
			return LRM_AP_CHANGED;
		}
		else
		{
			bDataChanged |= !( itUnit->second.m_kInfo == kUnitInfo );
		}
	}
	else
	{
		if( ( 0 < kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] ) || ( 0 < kUnitInfo.m_iPoint[SEnum::LRMTC_AP] ) )
			return LRM_INSERT_DATA;
		else
			return LRM_NONE;
	}

	if( bDataChanged == true )
	{
		return LRM_DATA_CHANGED;
	}

	return LRM_NONE;
}

bool KGSLocalRankingManager::SomethingChanged( IN const int iUID
											 , IN const KLocalRankingUserInfo& kUserInfo
											 , IN const KLocalRankingUnitInfo& kUnitInfo
											 )
{
	if( CheckInit() == false )
	{
		// 초기화되지 않은 상태
		return false;
	}

	if( CheckUID( iUID ) == false )
	{
		// UID 불일치
		return false;
	}

	// 무엇이 바뀌었는가?
	const int iChangeFlag = CheckChangedInfo( kUserInfo, kUnitInfo );
	switch( iChangeFlag )
	{
	case LRM_CATEGORY_CHANGED:
		{
			// 1. 유저 카테고리 정보가 바뀌었다.
			// 유저 유닛 리스트 사본을 가져오고,
			std::set< UidType > setUnitList;
			std::map< UidType, std::set< UidType > >::const_iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
			if( itUnitList != m_mapUserUnitList.end() )
			{
				setUnitList = itUnitList->second;
			}

			// 현재 받은 UnitUID를 제거한 뒤,
			setUnitList.erase( kUnitInfo.m_iUnitUID );

			KLocalRankingUserInfo kPrevUserInfo;
			KLocalRankingUnitInfo kPrevUnitInfo;

			if( GetUserInfo( kUserInfo.m_iUserUID, kPrevUserInfo ) == false )
			{
				return false;
			}

			// 사본에 대해 루프를 돌며, 모든 유닛에 대해 RankingProcess를 수행한다.
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setUnitList )
			{
				// UnitInfo의 사본을 사용해야 한다. ( RankingProcess 내부에서 삽입 삭제가 발생할테니... )
				if( GetUnitInfo( iUnitUID, kPrevUnitInfo ) == false )
				{
					continue;
				}

				RankingProcess( iChangeFlag, kPrevUserInfo, kPrevUnitInfo, kUserInfo, kPrevUnitInfo );
			}

			if( GetUnitInfo( kUnitInfo.m_iUnitUID, kPrevUnitInfo ) == true )
			{
				RankingProcess( iChangeFlag, kPrevUserInfo, kPrevUnitInfo, kUserInfo, kUnitInfo );
			}
		}
		break;

	case LRM_SPIRIT_CHANGED:
	case LRM_AP_CHANGED:
		{
			// 2. 유닛 ELP 정보가 바뀌었다.
			// RankingProcess를 수행한다.
			KLocalRankingUserInfo kPrevUserInfo;
			KLocalRankingUnitInfo kPrevUnitInfo;

			if( GetUserInfo( kUserInfo.m_iUserUID, kPrevUserInfo ) == false )
			{
				return false;
			}

			if( GetUnitInfo( kUnitInfo.m_iUnitUID, kPrevUnitInfo ) == false )
			{
				return false;
			}

			RankingProcess( iChangeFlag, kPrevUserInfo, kPrevUnitInfo, kUserInfo, kUnitInfo );
		}
		break;

	case LRM_INSERT_DATA:
		RankingProcess( iChangeFlag, kUserInfo, kUnitInfo, kUserInfo, kUnitInfo );
		break;

	case LRM_DATA_CHANGED:
		// 3. 기타 정보가 바뀌었다.
		// 정보를 쓴다.
		DataUpdate( kUserInfo, kUnitInfo );
		break;

	default:
		// 아무것도 하지 않는다.
		return false;
	}

	return true;
}

void KGSLocalRankingManager::RankingProcess( IN const int iChangeFlag
										   , IN const KLocalRankingUserInfo& kPrevUserInfo
										   , IN const KLocalRankingUnitInfo& kPrevUnitInfo
										   , IN const KLocalRankingUserInfo& kUserInfo
										   , IN const KLocalRankingUnitInfo& kUnitInfo
										   )
{
	const int iPrevCategoryIndex = SEnum::GetCategoryIndex( kPrevUserInfo.m_iCategory );
	const int iNextCategoryIndex = ( SEnum::CheckFlag( kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) == true ) ? SEnum::LRLC_NONE : SEnum::GetCategoryIndex( kUserInfo.m_iCategory );

	int iMin;
	int iMax;

	if( ( iChangeFlag == LRM_CATEGORY_CHANGED ) || ( iChangeFlag == LRM_INSERT_DATA ) )
	{
		iMin = SEnum::LRMTC_NONE + 1;
		iMax = SEnum::LRMTC_MAX;
	}
	else if( iChangeFlag == LRM_SPIRIT_CHANGED )
	{
		iMin = SEnum::LRMTC_SPIRIT;
		iMax = iMin + 1;
	}
	else if( iChangeFlag == LRM_AP_CHANGED )
	{
		iMin = SEnum::LRMTC_AP;
		iMax = iMin + 1;
	}

	for( int i = iMin; i < iMax; ++ i )
	{
		// 랭크 처리
		Remove( i, iPrevCategoryIndex, kPrevUserInfo, kPrevUnitInfo );
		Add( i, iNextCategoryIndex, kUnitInfo.m_iPoint[i], kUserInfo, kUnitInfo );
	}
}

void KGSLocalRankingManager::DataInsert( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	// 유저 정보 처리
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser == m_mapRankerUserInfo.end() )
	{
		KLocalRankingUserInfoCounter kUserInfoCount;
		itUser = m_mapRankerUserInfo.insert( std::map<UidType, KLocalRankingUserInfoCounter>::value_type( kUserInfo.m_iUserUID, kUserInfoCount ) ).first;
	}

	itUser->second.m_kInfo = kUserInfo;
	++itUser->second.m_iCounter;

	// 유닛 정보 처리
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit == m_mapRankerUnitInfo.end() )
	{
		KLocalRankingUnitInfoCounter kUnitInfoCount;
		itUnit = m_mapRankerUnitInfo.insert( std::map<UidType, KLocalRankingUnitInfoCounter>::value_type( kUnitInfo.m_iUnitUID, kUnitInfoCount ) ).first;
	}

	itUnit->second.m_iUserUID	= kUserInfo.m_iUserUID;
	itUnit->second.m_kInfo		= kUnitInfo;
	++itUnit->second.m_iCounter;

	// 유저와 유닛 정보 이어주기
	std::map< UidType, std::set< UidType > >::iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
	if( itUnitList == m_mapUserUnitList.end() )
	{
		std::set<UidType> setUnitList;
		itUnitList = m_mapUserUnitList.insert( std::map< UidType, std::set< UidType > >::value_type( kUserInfo.m_iUserUID, setUnitList ) ).first;
	}

	itUnitList->second.insert( kUnitInfo.m_iUnitUID );
}

void KGSLocalRankingManager::DataRemove( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	int iRemainUserInfoCount = 0;

	// 유저 정보 처리
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		if( 1 < itUser->second.m_iCounter )
		{
			--itUser->second.m_iCounter;
			iRemainUserInfoCount = itUser->second.m_iCounter;
		}
		else
		{
			m_mapRankerUserInfo.erase( itUser );
		}
	}

	// 유닛 정보 처리
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		if( 1 < itUnit->second.m_iCounter )
		{
			--itUnit->second.m_iCounter;
		}
		else
		{
			m_mapRankerUnitInfo.erase( itUnit );
		}
	}

	// 유저와 유닛 정보 정리하기
	if( 0 < iRemainUserInfoCount )
	{
		std::map< UidType, std::set< UidType > >::iterator itUnitList = m_mapUserUnitList.find( kUserInfo.m_iUserUID );
		if( itUnitList != m_mapUserUnitList.end() )
		{
			itUnitList->second.erase( kUnitInfo.m_iUnitUID );
		}
	}
	else
	{
		m_mapUserUnitList.erase( kUserInfo.m_iUserUID );
	}
}

void KGSLocalRankingManager::DataUpdate( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo )
{
	// 유저 정보 수정
	std::map<UidType, KLocalRankingUserInfoCounter>::iterator itUser = m_mapRankerUserInfo.find( kUserInfo.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		itUser->second.m_kInfo = kUserInfo;
	}

	// 유닛 정보 수정
	std::map<UidType, KLocalRankingUnitInfoCounter>::iterator itUnit = m_mapRankerUnitInfo.find( kUnitInfo.m_iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		itUnit->second.m_kInfo = kUnitInfo;
	}
}

bool KGSLocalRankingManager::Remove( IN const int iMainTabIndex
								   , IN int iCategory
								   , IN const KLocalRankingUserInfo& kUserInfo
								   , IN const KLocalRankingUnitInfo& kUnitInfo
								   )
{
	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		return false;
	}

	iCategory = SEnum::GetCategoryIndex( iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return false;
	}

	return Remove( itRanker->second, kUserInfo, kUnitInfo );
}

bool KGSLocalRankingManager::Remove( IN RankMMAP& mmapRanker
								   , IN const KLocalRankingUserInfo& kUserInfo
								   , IN const KLocalRankingUnitInfo& kUnitInfo
								   )
{
	RankMMAP::iterator it;
	for( it = mmapRanker.begin(); it != mmapRanker.end(); ++it )
	{
		if( it->second == kUnitInfo.m_iUnitUID )
		{
			break;
		}
	}

	// 해당하는 정보가 없다면 루틴 종료
	if( it == mmapRanker.end() )
	{
		return false;
	}

	// 랭크에서 제거
	mmapRanker.erase( it );

	DataRemove( kUserInfo, kUnitInfo );

	return true;
}

bool KGSLocalRankingManager::Add( IN const int iMainTabIndex
								, IN int iCategory
								, IN const int iAccumulatedPoint
								, IN const KLocalRankingUserInfo& kUserInfo
								, IN const KLocalRankingUnitInfo& kUnitInfo
								)
{
	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		return false;
	}

	iCategory = SEnum::GetCategoryIndex( iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		std::pair< std::map< std::pair< int, int >, RankMMAP >::iterator, bool> pairResult = m_mapRankerTab.insert( std::map< std::pair< int, int >, RankMMAP >::value_type( std::make_pair( iMainTabIndex, iCategory ), RankMMAP() ) );
		if( pairResult.second == false )
		{
			return false;
		}

		itRanker = pairResult.first;
	}

	return Add( itRanker->second, iAccumulatedPoint, kUserInfo, kUnitInfo );
}

bool KGSLocalRankingManager::Add( IN RankMMAP& mmapRanker
								, IN const int iAccumulatedPoint
								, IN const KLocalRankingUserInfo& kUserInfo
								, IN const KLocalRankingUnitInfo& kUnitInfo
								)
{
	if( SEnum::GetCategoryIndex( kUserInfo.m_iCategory ) == SEnum::LRLC_NONE )
	{
		return false;
	}

	if( iAccumulatedPoint <= 0 )
	{
		return false;
	}

	if( static_cast<size_t>( SiKGameSysVal()->GetLocalRankingNumOfRanker() ) <= mmapRanker.size() )
	{
		// 누군가를 쫓아내야 랭커에 들어갈 수 있는 상태이다.
		RankMMAP::reverse_iterator rit = mmapRanker.rbegin();
		if( rit != mmapRanker.rend() )
		{
			if( iAccumulatedPoint < rit->first )
			{
				// 랭킹에 들 수 없는 ELP이다.
				return false;
			}
		}
	}

	// 동일한 점수라면, 나중에 유입된 대상을 높은 랭크에 배치 시킨다.
	//RankMMAP::iterator it = mmapRanker.find( iAccumulatedPoint );
	//mmapRanker.insert( it, RankMMAP::value_type( iAccumulatedPoint, kUnitInfo.m_iUnitUID ) );
	mmapRanker.insert( RankMMAP::value_type( iAccumulatedPoint, kUnitInfo.m_iUnitUID ) );

	// 고유 정보 처리
	DataInsert( kUserInfo, kUnitInfo );

	return true;
}


void KGSLocalRankingManager::SizeProcess( void )
{
	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker;
	for( itRanker = m_mapRankerTab.begin(); itRanker != m_mapRankerTab.end(); ++itRanker )
	{
		_SizeProcess( itRanker->second );
	}
}

void KGSLocalRankingManager::_SizeProcess( IN OUT RankMMAP& mmapRanker )
{
	// 몇자리나 넘쳤는지 확인한다.
	int iGab = mmapRanker.size() - SiKGameSysVal()->GetLocalRankingNumOfRanker();
	if( iGab <= 0 )
	{
		return;
	}

	// 끝에서부터 넘친 수만큼 제거한다.
	for( int i = 0; i < iGab; ++i )
	{
		RankMMAP::reverse_iterator rit = mmapRanker.rbegin();
		if( rit == mmapRanker.rend() )
		{
			break;
		}

		const UidType iUnitUID = rit->second;

		// UnitInfo 구하기
		std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( iUnitUID );
		if( itUnit == m_mapRankerUnitInfo.end() )
		{
			continue;
		}
		const KLocalRankingUnitInfo kUnitInfo = itUnit->second.m_kInfo;

		// UserInfo 구하기
		std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
		if( itUser == m_mapRankerUserInfo.end() )
		{
			continue;
		}
		const KLocalRankingUserInfo kUserInfo = itUser->second.m_kInfo;

		// 제거 처리
		Remove( mmapRanker, kUserInfo, kUnitInfo );
	}
}

bool KGSLocalRankingManager::RankingCheck( IN const KLocalRankingUserInfo& kUserInfo, IN const KLocalRankingUnitInfo& kUnitInfo ) const
{
	if( SEnum::CheckFlag( kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) == true )
	{
		return false;
	}

	const int iCategory = SEnum::GetCategoryIndex( kUserInfo.m_iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	for( int i = 0; i < SEnum::LRMTC_MAX; ++i )
	{
		if( _RankingCheck( i, iCategory, kUnitInfo.m_iPoint[i] ) == true )
		{
			return true;
		}
	}
	
	return false;
}

bool KGSLocalRankingManager::_RankingCheck( IN const int iMainTabIndex
										  , IN int iCategory
										  , IN const int iAccumulatedPoint
										  ) const
{
	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		return false;
	}

	iCategory = SEnum::GetCategoryIndex( iCategory );
	if( iCategory == SEnum::LRLC_NONE )
	{
		return false;
	}

	std::map< std::pair< int, int >, RankMMAP >::const_iterator itRanker = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return true;
	}

	const RankMMAP& mmapRanker = itRanker->second;

	int iRank = 1;
	RankMMAP::const_iterator it;
	for( it = mmapRanker.begin(); it != mmapRanker.end(); ++it )
	{
		if( it->first <= iAccumulatedPoint )
		{
			break;
		}

		++iRank;
	}

	return iRank <= SiKGameSysVal()->GetLocalRankingNumOfRanker();
}

void KGSLocalRankingManager::GetRankerList( IN OUT UINT& nViewPage
										  , IN const char cMainTabIndex
										  , IN const byte byteFilter
										  , IN const std::set<UidType>& setFriendList
										  , OUT UINT& nTotalPage
										  , OUT std::vector<KLocalRankingRankerInfo>& vecRankerList
										  , OUT KLocalRankingRankerInfo& kPrevRankerInfo
										  , IN OUT KLocalRankingRankerInfo& kMyRankInfo
										  )
{
	if( ( cMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= cMainTabIndex ) )
	{
		START_LOG( cerr, L"잘못된 cMainTabIndex 입니다." )
			<< BUILD_LOG( cMainTabIndex )
			<< END_LOG;
		return;
	}

	const int iCategory = SEnum::GetCategoryIndex( kMyRankInfo.m_kUserInfo.m_iCategory );

	if( iCategory == SEnum::LRLC_NONE )
	{
		return;
	}

	std::map< std::pair< int, int >, RankMMAP >::iterator itRanker = m_mapRankerTab.find( std::make_pair( static_cast<int>( cMainTabIndex ), iCategory ) );
	if( itRanker == m_mapRankerTab.end() )
	{
		return;
	}

	const int iMax = 2;
	int iIndex = 0;
	RankMMAP* mmapRanker = &itRanker->second;
	RankMMAP mmapStorage[iMax];
	RankMMAP* mmapFiltered = mmapStorage + iIndex;

	if( SEnum::CheckFlag( byteFilter, SEnum::LRF_FRIEND ) == true )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			if( setFriendList.find( it->second ) != setFriendList.end() )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}
		
		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		//mmapFiltered->clear();
	}

	if( SEnum::CheckFlag( byteFilter, SEnum::LRF_UNIT ) == true )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( it->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			if( itUnit->second.m_kInfo.m_cUnitClass == kMyRankInfo.m_kUnitInfo.m_cUnitClass )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}
		
		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		mmapFiltered->clear();
	}

	if( SEnum::CheckFlag( byteFilter, SEnum::LRF_GENDER_MALE ) == true )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( it->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
			if( itUser == m_mapRankerUserInfo.end() )
			{
				continue;
			}

			if( SEnum::CheckFlag( itUser->second.m_kInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == true )
			{
				continue;
			}

			if( itUser->second.m_kInfo.m_byteGender == 1 )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}

		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		mmapFiltered->clear();
	}
	else if( SEnum::CheckFlag( byteFilter, SEnum::LRF_GENDER_FEMALE ) == true )
	{
		for( RankMMAP::const_iterator it = mmapRanker->begin(); it != mmapRanker->end(); ++it )
		{
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( it->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
			if( itUser == m_mapRankerUserInfo.end() )
			{
				continue;
			}

			if( SEnum::CheckFlag( itUser->second.m_kInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == true )
			{
				continue;
			}

			if( itUser->second.m_kInfo.m_byteGender == 2 )
			{
				mmapFiltered->insert( RankMMAP::value_type( it->first, it->second ) );
			}
		}

		if( iMax <= ++iIndex )
			iIndex = 0;

		mmapRanker		= mmapFiltered;
		mmapFiltered	= mmapStorage + iIndex;
		mmapFiltered->clear();
	}

	_GetRankerList( *mmapRanker
		, nViewPage
		, nTotalPage
		, vecRankerList
		, kPrevRankerInfo
		, kMyRankInfo
		);
}

void KGSLocalRankingManager::_GetRankerList( IN const RankMMAP& mmapRanker
										   , IN OUT UINT& nViewPage
										   , OUT UINT& nTotalPage
										   , OUT std::vector<KLocalRankingRankerInfo>& vecRankerList
										   , OUT KLocalRankingRankerInfo& kPrevRankerInfo
										   , IN OUT KLocalRankingRankerInfo& kMyRankInfo
										   ) const
{
	const int iNumOfPageUser	= 7;
	const int iNumOfRanker		= SiKGameSysVal()->GetLocalRankingNumOfRanker();
	
	// 전체 페이지 수 구하기
	int iTotalNum = min( static_cast<int>( mmapRanker.size() ), iNumOfRanker );
	nTotalPage = iTotalNum / iNumOfPageUser;
	if( 0 < ( iTotalNum % iNumOfPageUser ) )
	{
		++nTotalPage;
	}

	// nViewPage의 유저 리스트 구하기
	UINT nNowPage	= 1;
	int iRank		= 0;
	std::vector< std::pair< UidType, int > > vecUnitList;

	RankMMAP::const_iterator itRanker;
	for( itRanker = mmapRanker.begin(); itRanker != mmapRanker.end(); ++itRanker )
	{
		++iRank;

		if( iNumOfPageUser == vecUnitList.size() )
		{
			++nNowPage;
			if( nViewPage < nNowPage )
			{
				break;
			}
			vecUnitList.clear();
		}
		vecUnitList.push_back( std::make_pair( itRanker->second, iRank ) );
	}

	// 구한 페이지 정보 세팅
	nViewPage = min( nViewPage, nNowPage );

	// 유저 유닛 정보 구하기
	std::vector< std::pair< UidType, int > >::const_iterator itUnit;
	for( itUnit = vecUnitList.begin(); itUnit != vecUnitList.end(); ++itUnit )
	{
		KLocalRankingRankerInfo kInfo;
		kInfo.m_iRank = itUnit->second;
		GetUserUnitInfo( itUnit->first, kInfo.m_kUserInfo, kInfo.m_kUnitInfo, true );
		vecRankerList.push_back( kInfo );
	}

	// 내 직전 유저 및 내 랭킹 정보 구하기
	iRank = mmapRanker.size();
	const int iSize	= mmapRanker.size();
	RankMMAP::const_reverse_iterator rit = mmapRanker.rbegin();

	if( iNumOfRanker < iSize )
	{
		const int iCount = iSize - iNumOfRanker;
		for( int i = 0; i < iCount; ++i )
		{
			if( rit == mmapRanker.rend() )
			{
				break;
			}
			++rit;
			--iRank;
		}
	}

	// 마지막 유저 정보 저장
	kPrevRankerInfo.m_iRank					= iRank;
	kPrevRankerInfo.m_kUnitInfo.m_iUnitUID	= rit->second;

	// 내 정보 찾기
	for( ; rit != mmapRanker.rend(); ++rit )
	{
		if( rit->second == kMyRankInfo.m_kUnitInfo.m_iUnitUID )
		{
			break;
		}
		--iRank;
	}

	// 내 랭크 체크
	if( rit != mmapRanker.rend() )
	{
		// 나는 랭커이다. 나와 내 직전 유저는 누군가?
		kMyRankInfo.m_iRank = iRank;

		// 내 직전 유저 체크
		if( ++rit != mmapRanker.rend() )
		{
			kPrevRankerInfo.m_iRank					= --iRank;
			kPrevRankerInfo.m_kUnitInfo.m_iUnitUID	= rit->second;
		}
	}

	GetUserUnitInfo( kPrevRankerInfo.m_kUnitInfo.m_iUnitUID, kPrevRankerInfo.m_kUserInfo, kPrevRankerInfo.m_kUnitInfo, true );
}

void KGSLocalRankingManager::GetUserUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUserInfo& kUserInfo , OUT KLocalRankingUnitInfo& kUnitInfo, IN const bool bUserFilter /*= false */) const
{
	kUserInfo.clear();
	kUnitInfo.clear();

	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( iUnitUID );
	if( itUnit != m_mapRankerUnitInfo.end() )
	{
		kUnitInfo = itUnit->second.m_kInfo;
	}

	std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator itUser = m_mapRankerUserInfo.find( itUnit->second.m_iUserUID );
	if( itUser != m_mapRankerUserInfo.end() )
	{
		const KLocalRankingUserInfo& kSrcUserInfo = itUser->second.m_kInfo;
		KLocalRankingUserInfo& kDstUserInfo = kUserInfo;

		if( bUserFilter == true )
		{
			kDstUserInfo.m_iUserUID		= kSrcUserInfo.m_iUserUID;
			kDstUserInfo.m_byteFilter	= kSrcUserInfo.m_byteFilter;
			if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) == false )
			{
				kDstUserInfo.m_iCategory = kSrcUserInfo.m_iCategory;
			}

			if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_BIRTH ) == false )
			{
				kDstUserInfo.m_iBirth = kSrcUserInfo.m_iBirth;
			}

			if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_PROFILE ) == false )
			{
				kDstUserInfo.m_wstrProfile = kSrcUserInfo.m_wstrProfile;
			}

			if( SEnum::CheckFlag( kSrcUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == false )
			{
				kDstUserInfo.m_byteGender = kSrcUserInfo.m_byteGender;
			}
		}
		else
		{
			kDstUserInfo = kSrcUserInfo;
		}
	}
}

bool KGSLocalRankingManager::GetUserInfo( IN const UidType iUserUID, OUT KLocalRankingUserInfo& kInfo ) const
{
	std::map<UidType, KLocalRankingUserInfoCounter>::const_iterator it = m_mapRankerUserInfo.find( iUserUID );
	if( it != m_mapRankerUserInfo.end() )
	{
		kInfo = it->second.m_kInfo;
		return true;
	}

	return false;
}

bool KGSLocalRankingManager::GetUnitInfo( IN const UidType iUnitUID, OUT KLocalRankingUnitInfo& kInfo ) const
{
	std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator it = m_mapRankerUnitInfo.find( iUnitUID );
	if( it != m_mapRankerUnitInfo.end() )
	{
		kInfo = it->second.m_kInfo;
		return true;
	}

	return false;
}

_IMPL_ON_FUNC( EGS_LOCAL_RANKING_INQUIRY_REQ, KLocalRankingInquiryData )
{
	KEGS_LOCAL_RANKING_INQUIRY_ACK kPacket;
	kPacket.m_nViewPage		= kPacket_.m_nViewPage;
	kPacket.m_cMainTabIndex	= kPacket_.m_cMainTabIndex;
	kPacket.m_cSubTabIndex	= kPacket_.m_cSubTabIndex;
	kPacket.m_byteFilter	= kPacket_.m_byteFilter;
	kPacket.m_kMyRankInfo	= kPacket_.m_kMyRankInfo;

	if( CheckInit() == false )
	{
		kPacket.m_iOK = NetError::ERR_LOCALRANKING_06;
		SendToGSUser( FIRST_SENDER_UID, EGS_LOCAL_RANKING_INQUIRY_ACK, kPacket );
		return;
	}

	GetRankerList( kPacket.m_nViewPage
				 , kPacket.m_cMainTabIndex
				 , kPacket.m_byteFilter
				 , kPacket_.m_setFriendList
				 , kPacket.m_nTotalPage
				 , kPacket.m_vecRankerList
				 , kPacket.m_kPrevRankerInfo
				 , kPacket.m_kMyRankInfo
				 );

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSUser( FIRST_SENDER_UID, EGS_LOCAL_RANKING_INQUIRY_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_CHECK_NOT )
{
	// 준비가 되어 있는가?
	if( CheckInit() == false )
	{
		return;
	}

	// UID를 검사하자!
	if( CheckUID( kPacket_.m_iUID ) == false )
	{
		return;
	}

	// 진입 혹은 퇴출
	if( ( IsRankingUser( kPacket_.m_kUserInfo.m_iUserUID ) == true ) || ( RankingCheck( kPacket_.m_kUserInfo, kPacket_.m_kUnitInfo ) == true ) )
	{
		// 로그인 서버에 전송한다.
		SendToLoginServer( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT )
{
	SomethingChanged( kPacket_.m_iUID, kPacket_.m_kUserInfo, kPacket_.m_kUnitInfo );
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_RESET_NOT )
{
	Init( kPacket_.m_iOK
		, kPacket_.m_iUID
		, kPacket_.m_mapRankerUnitInfo
		, kPacket_.m_mapRankerUIDInfo
		, kPacket_.m_mapRankerUserInfo
		);

	// 마지막 패킷인가?
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 모든 유저들에게 브로드캐스팅
		BroadCastAllGSUser( ELG_LOCAL_RANKING_RESET_NOT );
	}
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT )
{
	const int iMainTabIndex	= kPacket_.m_iMainTabIndex;
	const int iSubTabIndex	= kPacket_.m_iSubTabIndex;

	if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) || ( SEnum::LRMTC_MAX <= iMainTabIndex ) )
	{
		START_LOG( cerr, L"유효하지 않은 iMainTabIndex 입니다." )
			<< BUILD_LOG( iMainTabIndex )
			<< END_LOG;
		return;
	}

	if( ( iSubTabIndex <= SEnum::LRLC_NONE ) || ( SEnum::LRLC_MAX <= iSubTabIndex ) )
	{
		START_LOG( cerr, L"유효하지 않은 iSubTabIndex 입니다." )
			<< BUILD_LOG( iSubTabIndex )
			<< END_LOG;
		return;
	}

	START_LOG( cout, L"로컬 랭킹 시스템 조사 시작" );

	std::map< std::pair< int, int >, RankMMAP >::const_iterator itTab = m_mapRankerTab.find( std::make_pair( iMainTabIndex, iSubTabIndex ) );
	if( itTab != m_mapRankerTab.end() )
	{
		START_LOG( cout, L"탭" )
			<< BUILD_LOG( iMainTabIndex )
			<< BUILD_LOG( iSubTabIndex );

		const RankMMAP& mmapRanker = itTab->second;
		RankMMAP::const_iterator itRank;
		int	iRank = 0;
		const int iMainTabIndex = itTab->first.first;
		for( itRank = mmapRanker.begin(); itRank != mmapRanker.end(); ++itRank )
		{
			++iRank;

			// 유닛 정보가 정상적으로 존재하는가?
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnit = m_mapRankerUnitInfo.find( itRank->second );
			if( itUnit == m_mapRankerUnitInfo.end() )
			{
				START_LOG( cout2, L"로컬 랭킹 유닛 정보 이상 유저" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second );
				continue;
			}

			// 유저 정보가 정상적으로 존재하는가?
			if( m_mapRankerUserInfo.find( itUnit->second.m_iUserUID ) == m_mapRankerUserInfo.end() )
			{
				START_LOG( cout2, L"로컬 랭킹 유저 정보 이상 유저" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second );
				continue;
			}

			// 내 다음 랭커가 존재하는가?
			RankMMAP::const_iterator itRankNext = itRank;
			if( ++itRankNext == mmapRanker.end() )
			{
				continue;
			}

			// 랭크 테이블에서의 랭킹 포인트가 순위에 맞게 들어가 있는가?
			if( itRank->first < itRankNext->first )
			{
				START_LOG( cout2, L"로컬 랭킹 포인트 정보 이상 유저" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second );

				START_LOG( cout2, L"다음 유저" )
					<< BUILD_LOG( iRank + 1 )
					<< BUILD_LOG( itRankNext->first )
					<< BUILD_LOG( itRankNext->second );
				continue;
			}

			// 내 다음 랭커의 유닛 정보가 존재하는가?
			std::map<UidType, KLocalRankingUnitInfoCounter>::const_iterator itUnitNext = m_mapRankerUnitInfo.find( itRankNext->second );
			if( itUnitNext == m_mapRankerUnitInfo.end() )
			{
				continue;
			}

			// 메인 카테고리 인덱스가 유효한가?
			if( ( iMainTabIndex <= SEnum::LRMTC_NONE ) ||
				( SEnum::LRMTC_MAX <= iMainTabIndex )
				)
			{
				continue;
			}

			// 유닛 정보에서의 랭킹 포인트가 순위에 맞게 들어가 있는가?
			if( itUnit->second.m_kInfo.m_iPoint[iMainTabIndex] < itUnitNext->second.m_kInfo.m_iPoint[iMainTabIndex] )
			{
				START_LOG( cout2, L"로컬 랭킹 포인트 정보 이상 유저" )
					<< BUILD_LOG( iRank )
					<< BUILD_LOG( itRank->first )
					<< BUILD_LOG( itRank->second )
					<< BUILD_LOG( itUnit->second.m_kInfo.m_iPoint[iMainTabIndex] );

				START_LOG( cout2, L"다음 유저" )
					<< BUILD_LOG( iRank + 1 )
					<< BUILD_LOG( itRankNext->first )
					<< BUILD_LOG( itRankNext->second )
					<< BUILD_LOG( itUnitNext->second.m_kInfo.m_iPoint[iMainTabIndex] );
				continue;
			}
		}
	}

	START_LOG( cout, L"로컬 랭킹 시스템 조사 종료" );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
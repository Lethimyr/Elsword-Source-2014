#include "LoginSimLayer.h"
#include "BaseServer.h"
#include "buffmanager.h"
#include "X2data/XSLBuffManager.h"

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM

ImplementSingleton( KBuffManager );

ImplToStringW( KBuffManager )
{
	return stm_;
}

KBuffManager::KBuffManager(void)
{
}

KBuffManager::~KBuffManager(void)
{
}


bool KBuffManager::AddUnitBuffInfo( IN const UidType& iUnitUID, IN const std::vector< KBuffInfo >& vecBuffInfo )
{
	//////////////////////////////////////////////////////////////////////////
	{
		// 버프 정보 검증
		BOOST_TEST_FOREACH( const KBuffInfo&, kTestBuff, vecBuffInfo )
		{
			if( kTestBuff.m_kFactorInfo.m_vecBuffBehaviorFactors.size() > 10 )
			{
				START_LOG( cerr, "m_vecBuffBehaviorFactors의 원소 갯수가 10개를 넘었다!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( kTestBuff.m_kFactorInfo.m_vecBuffBehaviorFactors.size() )
					<< END_LOG;
			}

			if( kTestBuff.m_kFactorInfo.m_vecBuffFinalizerFactors.size() > 10 )
			{
				START_LOG( cerr, "m_vecBuffFinalizerFactors의 원소 갯수가 10개를 넘었다!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( kTestBuff.m_kFactorInfo.m_vecBuffFinalizerFactors.size() )
					<< END_LOG;
			}

			BOOST_TEST_FOREACH( const KBuffBehaviorFactor&, kBuffBFactor, kTestBuff.m_kFactorInfo.m_vecBuffBehaviorFactors )
			{
				if( kBuffBFactor.m_vecValues.size() > 10 )
				{
					START_LOG( cerr, "KBuffBehaviorFactor의 m_vecValues원소 갯수가 10개를 넘었다!" )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( kBuffBFactor.m_vecValues.size() )
						<< END_LOG;
				}
			}

			BOOST_TEST_FOREACH( const KBuffFinalizerFactor&, kBuffFFactor, kTestBuff.m_kFactorInfo.m_vecBuffFinalizerFactors )
			{
				if( kBuffFFactor.m_vecValues.size() > 10 )
				{
					START_LOG( cerr, "KBuffFinalizerFactor의 m_vecValues원소 갯수가 10개를 넘었다!" )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( kBuffFFactor.m_vecValues.size() )
						<< END_LOG;
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	std::map< UidType, KUnitBuffInfo >::iterator mit = m_mapUnitBuffList.find( iUnitUID );
	if( mit == m_mapUnitBuffList.end() )
	{
		KUnitBuffInfo kNewUnitBuffInfo;

		// 최초 입력되는 것이므로 시간을 저장하자.
		CTime tCurr = CTime::GetCurrentTime();
		kNewUnitBuffInfo.m_iInsertTime = tCurr.GetTime();
		BOOST_TEST_FOREACH( const KBuffInfo&, kNewBuffInfo, vecBuffInfo )
		{
			kNewUnitBuffInfo.m_mapBuffInfo.insert( std::make_pair( kNewBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, kNewBuffInfo ) );
		}

		// 유닛의 정보가 없으니 새로 추가하자
		m_mapUnitBuffList.insert( std::make_pair( iUnitUID, kNewUnitBuffInfo ) );

		if( AddRenewBuffTime( iUnitUID, kNewUnitBuffInfo.m_iInsertTime ) == false )
		{
			// 시간 추가 실패
			// 결과적으로 버프를 추가 하지 못한건 아니니깐 실패 처리는 하지 않는다.
		}
		return true;
	}
	
	// 유닛의 정보가 존재한다.
	KUnitBuffInfo& kUnitBuffInfo = mit->second;

	// 옛날 버프중에 같은게 있는지 찾아 본다.
	BOOST_TEST_FOREACH( const KBuffInfo&, kNewBuff, vecBuffInfo )
	{
		std::map< int, KBuffInfo >::iterator mitBI;
		mitBI = kUnitBuffInfo.m_mapBuffInfo.find( kNewBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( mitBI == kUnitBuffInfo.m_mapBuffInfo.end() )
		{
			// 없는 버프는 추가한다.
			kUnitBuffInfo.m_mapBuffInfo.insert( std::make_pair( kNewBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, kNewBuff ) );
		}
		else
		{
			// 정보 갱신
			mitBI->second = kNewBuff;
		}
	}

	// 버프 갱신 되었으므로 시간도 갱신한다.
	CTime tCurr = CTime::GetCurrentTime();
	__int64 iOldTime = kUnitBuffInfo.m_iInsertTime;
	__int64 iNewTime = tCurr.GetTime();

	if( AddRenewBuffTime( iUnitUID, iNewTime, iOldTime ) == true )
	{
		kUnitBuffInfo.m_iInsertTime = iNewTime;
	}
	else
	{
		// 시간 추가 실패
		// 결과적으로 버프를 추가 하지 못한건 아니니깐 실패 처리는 하지 않는다.
	}

	return true;
}

bool KBuffManager::DeleteUnitBuffInfo( IN UidType& iUnitUID, IN std::vector<int>& vecBuffID )
{
	std::map< UidType, KUnitBuffInfo >::iterator mit = m_mapUnitBuffList.find( iUnitUID );
	if( mit == m_mapUnitBuffList.end() )
	{
		START_LOG( clog, L"존재하지 않는 Unit 정보를 지우려고 합니다." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}
	else
	{
		BOOST_TEST_FOREACH( const int, iBuffID, vecBuffID )
		{
			mit->second.m_mapBuffInfo.erase( iBuffID );
		}
		
		if( static_cast<int>(mit->second.m_mapBuffInfo.size()) <= 0 )
		{			
			// 시간 정보 지우기
			DeleteBuffTime( mit->second.m_iInsertTime, mit->first );

			// 버프 정보 지우기
			m_mapUnitBuffList.erase( mit );
		}
	}

	return true;
}

#ifdef _CONVERT_VS_2010
bool KBuffManager::DeleteUnitAllBuffInfo( IN UidType iUnitUID )
#else
bool KBuffManager::DeleteUnitAllBuffInfo( IN UidType& iUnitUID )
#endif _CONVERT_VS_2010
{
	std::map< UidType, KUnitBuffInfo >::iterator mit = m_mapUnitBuffList.find( iUnitUID );
	if( mit != m_mapUnitBuffList.end() )
	{
		START_LOG( clog, L"존재하지 않는 Unit 정보를 지우려고 합니다." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}
	else
	{
		// 시간 정보 지우기
		DeleteBuffTime( mit->second.m_iInsertTime, mit->first );

		// 버프 정보 지우기
		m_mapUnitBuffList.erase( mit );
	}

	return true;
}

bool KBuffManager::AddRenewBuffTime( IN const UidType& iUnitUID, IN const __int64& iNewTime, IN const __int64 iOleTime /* = 0*/ )
{
	// 기존의 시간의 값이 입력되어 있었다.
	if( iOleTime > 0 )
	{
		// 기존 정보를 찾아서 지우자
		std::map< __int64, std::vector<UidType> >::iterator mit = m_mapUnitTime.find( iOleTime );
		if( mit != m_mapUnitTime.end() )
		{
			std::vector<UidType>::iterator vit = mit->second.begin();
			for( ; vit != mit->second.end() ; ++vit )
			{
				if( iUnitUID == (*vit) )
				{
					// 삭제하자
					mit->second.erase( vit );

					break;
				}
			}
		}
		else
		{
			START_LOG( clog, L"있다고 했는데 없다...?" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( iNewTime )
				<< BUILD_LOG( iOleTime )
				<< END_LOG;

			return false;
		}
	}

	std::map< __int64, std::vector<UidType> >::iterator mit = m_mapUnitTime.find( iNewTime );
	if( mit != m_mapUnitTime.end() )
	{
		BOOST_TEST_FOREACH( UidType, iUID, mit->second )
		{
			if( iUID == iUnitUID )
			{
				START_LOG( cerr, L"입력된 데이터가 있는데?" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;

				// 있다면 추가 안해도 된다. 실패는 아님
				return true;
			}
		}

		mit->second.push_back( iUnitUID );
	}
	else
	{
		std::vector<UidType> vecNew;
		vecNew.push_back( iUnitUID );

		m_mapUnitTime.insert( std::make_pair( iNewTime, vecNew ) );
	}

	return true;
}

void KBuffManager::DeleteBuffTime( IN __int64& iTime, IN UidType iUnitUID )
{
	// 기존 정보를 찾아서 지우자
	std::map< __int64, std::vector<UidType> >::iterator mit = m_mapUnitTime.find( iTime );
	if( mit != m_mapUnitTime.end() )
	{
		std::vector<UidType>::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end() ; ++vit )
		{
			if( iUnitUID == (*vit) )
			{
				// 삭제하자
				mit->second.erase( vit );

				break;
			}
		}
	}
	else
	{
		START_LOG( clog, L"있다고 했는데 없다...?" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iTime )
			<< END_LOG;
	}
}

bool KBuffManager::GetUnitAllBuffInfo( IN UidType& iUnitUID, OUT KUnitBuffInfo& kUnitBuffInfo )
{
	std::map< UidType, KUnitBuffInfo >::iterator mit = m_mapUnitBuffList.find( iUnitUID );
	if( mit != m_mapUnitBuffList.end() )
	{
		// 데이터를 요청 했다는 것은 접속을 했으니 유닛 정보 시간을 갱신 시켜야 한다.
		CTime tCurr = CTime::GetCurrentTime();
		__int64 iOldTime = mit->second.m_iInsertTime;
		__int64 iNewTime = tCurr.GetTime();
		
		if( AddRenewBuffTime( iUnitUID, iNewTime, iOldTime ) == true )
		{
			mit->second.m_iInsertTime = iNewTime;
		}
		else
		{
			// 시간 추가 실패
			// 결과적으로 버프를 추가 하지 못한건 아니니깐 실패 처리는 하지 않는다.
		}

		kUnitBuffInfo = mit->second;
	}
	else
	{
		START_LOG( clog, L"데이터가 없다." )
			<< BUILD_LOG( iUnitUID )
			<<END_LOG;
		return false;
	}

	return true;
}

void KBuffManager::CheckEndBuff()
{
	// 현재 시간
	CTime tCurr = CTime::GetCurrentTime();
	// 삭제 기준 시간
	CTimeSpan tSpan = CTimeSpan( 0, CXSLBuffManager::UBKT_UNIT_RESET_TIME_HOUR, 0, 0);
	CTime tDeleteCurr;
	tDeleteCurr = tCurr - tSpan;

	// 삭제할 unit 을 저장 해둘 컨테이너
	std::set< UidType > setUnitList;

	// 현재 시간보다 낮은 시간의 유저를 찾아서 지운다. - 시간 별로 정렬 되므로 처음 부터 검색
	std::map< __int64, std::vector<UidType> >::iterator mit = m_mapUnitTime.begin();
	for( ; mit != m_mapUnitTime.end() ; ++mit )
	{
		if( mit->first > tDeleteCurr.GetTime() )
		{
			START_LOG( clog, L"삭제 되어야 할 시간보다 크므로 다음에 지운다." )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( tDeleteCurr.GetTime() )
				<< END_LOG;
			break;
		}

		BOOST_TEST_FOREACH( UidType, iUnitUID, mit->second )
		{
			setUnitList.insert( iUnitUID );
		}
	}

	std::set< UidType >::iterator sit = setUnitList.begin();
	for( ; sit != setUnitList.end() ; ++sit )
	{
#ifdef _CONVERT_VS_2010
		DeleteUnitAllBuffInfo( *sit );
#else
		DeleteUnitAllBuffInfo( (*sit) );
#endif _CONVERT_VS_2010
	}
}


#endif SERV_SERVER_BUFF_SYSTEM
//}

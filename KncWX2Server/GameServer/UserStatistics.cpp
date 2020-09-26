#include "Enum/Enum.h"
#include "GSUser.h"
#include ".\userstatistics.h"
//{{ 2011. 01. 18	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	#include "GameSysVal/GameSysVal.h"
#endif SERV_CHAR_LOG
//}}


KUserStatistics::KUserStatistics(void)
:
m_bIsNormalUser( true ) // 기본적으로 통계적용되도록 초기화. -> 운영자는 false값
{
	//{{ 2007. 11. 19  최육사  유저 플레이 타임 통계
	m_iStatisticsPlayTime[PT_TOTAL_PLAY_TIME]	= 0;
	m_iStatisticsPlayTime[PT_DUNGEON_PLAY_TIME]	= 0;
	m_iStatisticsPlayTime[PT_PVP_PLAY_TIME]		= 0;
	//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
	m_iStatisticsPlayTime[PT_SPIRIT_PLAY_TIME]	= 0;
	m_iStatisticsPlayTime[PT_FIELD_PLAY_TIME]	= 0;
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}

	//}}
	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	for( int iIndex = 0; iIndex < AGCT_MAX; ++iIndex )
	{
		m_iAccountGameCount[iIndex].SetValue( 0 );
	}
	m_bIsAccountCount = false;
#endif SERV_ACCOUNT_COUNT
	//}}

	Clear();
}

KUserStatistics::~KUserStatistics(void)
{
}

void KUserStatistics::SetIsNormalUser( char cAuthLevel )
{
	m_bIsNormalUser = ( cAuthLevel == SEnum::UAL_NORMAL || cAuthLevel == SEnum::UAL_BLOCKED_1 );
}

void KUserStatistics::GetCharacterStatistics( std::map< std::pair< int, int >, std::vector< int > >& mapPvpStatistics,
											  std::map< std::pair< int, int >, std::vector< int > >& mapDungeonStatistics,
											  std::map< std::pair< int, int >, std::vector< int > >& mapTCenterStatistics,
											  std::map< std::pair< int, int >, std::vector< int > >& mapEtcStatistics 
#ifdef SERV_USER_STATISTICS_RENEWAL
											  , std::map< std::pair< int, int >, std::vector< int > >& mapEDStatistics 
#endif //SERV_USER_STATISTICS_RENEWAL
											  )
{
	mapPvpStatistics	 = m_mapPvpStatistics;
	mapDungeonStatistics = m_mapDungeonStatistics;
	mapTCenterStatistics = m_mapTCenterStatistics;
	mapEtcStatistics	 = m_mapEtcStatistics;
#ifdef SERV_USER_STATISTICS_RENEWAL
	mapEDStatistics		= m_mapEDStatistics;
#endif //SERV_USER_STATISTICS_RENEWAL
}

void KUserStatistics::IncreaseCount( int iTableID, int iSubTableID , int iColumn, int iCount )
{
	// 운영자급 이상이라면 통계 포함 안됨
	if( m_bIsNormalUser == false )
		return;

	UserStatisticsKey key( iTableID, iSubTableID );

	// 업데이트할 데이터를 vector로 변환
	std::vector< int > vecCount;
	for( int i = 0; i < iColumn; i++ )
	{
		vecCount.push_back( 0 );
	}
	vecCount.push_back( iCount );

	switch( iTableID )
	{
	case USTable_PvpModeCount:
	case USTable_PvpPlayTime:
		IncreaseCount( m_mapPvpStatistics ,key, vecCount );
		break;

	case USTable_Dungeon:
		IncreaseCount( m_mapDungeonStatistics, key, vecCount );
		break;

	case USTable_TrainingCenter:
		IncreaseCount( m_mapTCenterStatistics, key, vecCount );
		break;

	case USTable_EtcData:
		IncreaseCount( m_mapEtcStatistics, key, vecCount );
		break;
#ifdef SERV_USER_STATISTICS_RENEWAL
	case USTable_EDData:
		IncreaseCount( m_mapEDStatistics, key, vecCount );
		break;
#endif //SERV_USER_STATISTICS_RENEWAL
	default:
		START_LOG( cerr, L"유저 통계 : 존재하지않는 테이블ID입니다." )
			<< BUILD_LOG( iTableID )
			<< END_LOG;

		break;
	}
}

void KUserStatistics::IncreaseCount( std::map< std::pair< int, int >, std::vector< int > >& mapStatistics, UserStatisticsKey& key, std::vector< int >& vecCount )
{
	// 통계 데이터 업데이트
	UserStatisticsIterator mit = mapStatistics.find( key );
	if( mit == mapStatistics.end() )
	{
		mapStatistics.insert( std::make_pair( key, vecCount ) );
	}
	else
	{
		std::vector< int >::const_iterator vitSrc = vecCount.begin();
		std::vector< int >::iterator vitDest = mit->second.begin();

		while( vitSrc != vecCount.end() )
		{
			if( vitDest == mit->second.end() )
			{
				while( vitSrc != vecCount.end() )
				{
					mit->second.push_back( *vitSrc );
					vitSrc++;
				}
			}
			else
			{
				( *vitDest ) += ( *vitSrc );
				vitSrc++;
				vitDest++;
			}
		}
	}
}

void KUserStatistics::Clear()
{
    m_mapPvpStatistics.clear();
	m_mapDungeonStatistics.clear();
	m_mapTCenterStatistics.clear();
	m_mapEtcStatistics.clear();
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_mapEDStatistics.clear();
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2010. 06. 30  최육사	근성도 통계
#ifdef SERV_SPIRIT_STATISTICS
	m_iUsedSpirit = 0;
#endif SERV_SPIRIT_STATISTICS
	//}}
	//{{ 2011. 01. 16	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	for( int iIndex = 0; iIndex < CGCT_MAX; ++iIndex )
	{
		m_iCharGameCount[iIndex].SetValue( 0 );
	}
	m_wstrUnitCreateDate.clear();
	m_bIsCharCount = false;
#endif SERV_CHAR_LOG
	//}}
}

void KUserStatistics::AddPlayTime( PLAY_TIME ePT, int iTotalPlayTime )
{
	if( iTotalPlayTime < 0  ||  iTotalPlayTime > 86400 ) // 24시간
	{
		START_LOG( cwarn, L"비 정상적인 대전 플레이 타임!" )
			<< BUILD_LOG( ePT )
			<< BUILD_LOG( iTotalPlayTime )
			<< END_LOG;

		// 유저 통계는 누적데이터이기 때문에 쓰레기값이 들어가면 안된다.
		return; 
	}

	m_iStatisticsPlayTime[ePT] += iTotalPlayTime;
}

//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
void KUserStatistics::InitAccountCountInfo( IN const std::wstring& wstrAccountRegDate, IN const std::map< int, int >& mapAccCountInfo )
{
	for( int iIndex = 0; iIndex < AGCT_MAX; ++iIndex )
	{
		std::map< int, int >::const_iterator mit;
		mit = mapAccCountInfo.find( iIndex );
		if( mit == mapAccCountInfo.end() )
			continue;

		m_iAccountGameCount[iIndex].SetValue( mit->second );
	}

	// 계정 생성 날짜 저장
	m_wstrAccountRegDate = wstrAccountRegDate;

	CTime tAccountRegDate;
	if( KncUtil::ConvertStringToCTime( wstrAccountRegDate, tAccountRegDate ) == false )
	{
		START_LOG( cerr, L"계정 생성 날짜 정보가 이상합니다. 변환 실패!" )
			<< BUILD_LOG( wstrAccountRegDate )
			<< END_LOG;

		std::wstring wstrFailedDate = L"2000-01-01 00:00:00";
		LIF( KncUtil::ConvertStringToCTime( wstrFailedDate, tAccountRegDate ) );
	}

	m_bIsAccountCount = ( tAccountRegDate > SiKGameSysVal()->GetAccountStatisticsStartDate() );
}

void KUserStatistics::IncreaseAccountCount( IN const ACCOUNT_GAME_COUNT_TYPE eCountType, IN const int iCount /*= 1*/ )
{
	if( IsAccountCount() == false )
		return;

	if( eCountType < 0  ||  eCountType > AGCT_MAX )
	{
		START_LOG( cerr, L"정의 되지 않은 값입니다!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return;
	}

	m_iAccountGameCount[eCountType] += iCount;
}

int KUserStatistics:: GetAccountCount( IN const ACCOUNT_GAME_COUNT_TYPE eCountType ) const
{
	if( eCountType < 0  ||  eCountType > AGCT_MAX )
	{
		START_LOG( cerr, L"정의 되지 않은 값입니다!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return 0;
	}

	return m_iAccountGameCount[eCountType];
}

void KUserStatistics::GetAccountDBUpdateInfo( OUT std::map< int, int >& mapAccCountInfo )
{
	mapAccCountInfo.clear();

	if( IsAccountCount() == false )
		return;

	for( int iIndex = 0; iIndex < AGCT_MAX; ++iIndex )
	{
		if( m_iAccountGameCount[iIndex].IsChanged() == false )
			continue;

		const int iChangedValue = m_iAccountGameCount[iIndex].GetChangeValue();
		m_iAccountGameCount[iIndex].SetValue( iChangedValue );

		mapAccCountInfo.insert( std::make_pair( iIndex, iChangedValue ) );
	}
}
#endif SERV_ACCOUNT_COUNT
//}}

//{{ 2011. 01. 16	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
void KUserStatistics::InitCharacterCountInfo( IN const std::map< int, int >& mapCharCountInfo, IN const std::wstring& wstrUnitCreateDate )
{
	for( int iIndex = 0; iIndex < CGCT_MAX; ++iIndex )
	{
		std::map< int, int >::const_iterator mit;
		mit = mapCharCountInfo.find( iIndex );
		if( mit == mapCharCountInfo.end() )
			continue;

		m_iCharGameCount[iIndex].SetValue( mit->second );
	}

	// 캐릭터 생성 날짜 저장
	m_wstrUnitCreateDate = wstrUnitCreateDate;

	CTime tUnitCreateDate;
	if( KncUtil::ConvertStringToCTime( wstrUnitCreateDate, tUnitCreateDate ) == false )
	{
        START_LOG( cerr, L"캐릭터 생성 날짜 정보가 이상합니다. 변환 실패!" )
			<< BUILD_LOG( wstrUnitCreateDate )
			<< END_LOG;

		std::wstring wstrFailedDate = L"2000-01-01 00:00:00";
		LIF( KncUtil::ConvertStringToCTime( wstrFailedDate, tUnitCreateDate ) );
	}

	m_bIsCharCount = ( tUnitCreateDate > SiKGameSysVal()->GetCharStatisticsStartDate() );
}

void KUserStatistics::IncreaseCharacterCount( IN const CHAR_GAME_COUNT_TYPE eCountType, IN const int iCount /*= 1*/ )
{
	if( IsCharCount() == false )
		return;

    if( eCountType < 0  ||  eCountType > CGCT_MAX )
	{
		START_LOG( cerr, L"정의 되지 않은 값입니다!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return;
	}

	m_iCharGameCount[eCountType] += iCount;
}

int	KUserStatistics::GetCharacterCount( IN const CHAR_GAME_COUNT_TYPE eCountType ) const
{
	if( eCountType < 0  ||  eCountType > CGCT_MAX )
	{
		START_LOG( cerr, L"정의 되지 않은 값입니다!" )
			<< BUILD_LOG( eCountType )
			<< END_LOG;
		return 0;
	}

    return m_iCharGameCount[eCountType];
}

void KUserStatistics::GetDBUpdateInfo( OUT std::map< int, int >& mapCharCountInfo )
{
	mapCharCountInfo.clear();

	if( IsCharCount() == false )
		return;

	for( int iIndex = 0; iIndex < CGCT_MAX; ++iIndex )
	{
		if( m_iCharGameCount[iIndex].IsChanged() == false )
			continue;

		const int iChangedValue = m_iCharGameCount[iIndex].GetChangeValue();
		m_iCharGameCount[iIndex].SetValue( iChangedValue );

		mapCharCountInfo.insert( std::make_pair( iIndex, iChangedValue ) );
	}    
}

void KUserStatistics::SetLastClearDungeonID( IN const int iDungeonID )
{
    m_iCharGameCount[CGCT_LAST_PLAY_DUNGEON_ID] += ( iDungeonID - m_iCharGameCount[CGCT_LAST_PLAY_DUNGEON_ID] );
}

#endif SERV_CHAR_LOG
//}}

//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
bool KUserStatistics::IsFirstCharSelectToday( IN const std::wstring& wstrUnitLastLoginDate )
{
	std::wstring wstrModifiedLastLoginDate;
	const bool bIsFirstSelect = ( GetCharacterCount( KUserStatistics::CGCT_SELECT_CHAR ) == 0 );
	if( bIsFirstSelect )
	{
		wstrModifiedLastLoginDate = L"2000-01-01 00:00:00";
	}
	else
	{
		wstrModifiedLastLoginDate = wstrUnitLastLoginDate;
	}

	CTime tCurTime = CTime::GetCurrentTime();
	CTime tLastLoginTime;
	if( KncUtil::ConvertStringToCTime( wstrModifiedLastLoginDate, tLastLoginTime ) == false )
	{
		START_LOG( cerr, L"마지막 캐릭터 선택시간 변환 실패!" )
			<< BUILD_LOG( wstrModifiedLastLoginDate )
			<< BUILD_LOG( wstrUnitLastLoginDate )
			<< END_LOG;
		return false;
	}
	
	CTime tCurToday = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 0, 0, 0 );
	CTime tLastLoginDay = CTime( tLastLoginTime.GetYear(), tLastLoginTime.GetMonth(), tLastLoginTime.GetDay(), 0, 0, 0 );

	// 오늘 날짜와 마지막 로그인 날짜를 비교했는데 오늘보다 더 옜날에 로그인 했으면 true를 반환!
	return ( tCurToday > tLastLoginDay );
}
#endif SERV_DAILY_CHAR_FIRST_SELECT
//}}

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
bool KUserStatistics::IsFirstUserLoginToday( IN const UidType& UserUID, IN const std::wstring& wstrUserLastLoginDate )
{
	CTime tCurr = CTime::GetCurrentTime();
	CTime tLastLoginTime;
	if( KncUtil::ConvertStringToCTime( wstrUserLastLoginDate, tLastLoginTime ) == false )
	{
		START_LOG( clog, L"이전 로그인 시간 변환 실패_없을 수도 있다!" )
			<< BUILD_LOG( UserUID )
			<< BUILD_LOG( wstrUserLastLoginDate )
			<< END_LOG;
		return false;
	}

	CTime tCurToday = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), 0, 0, 0 );
	CTime tLastLoginDay = CTime( tLastLoginTime.GetYear(), tLastLoginTime.GetMonth(), tLastLoginTime.GetDay(), 0, 0, 0 );

	// 오늘 날짜와 마지막 로그인 날짜를 비교했는데 오늘보다 더 옜날에 로그인 했으면 true를 반환!
	return ( tCurToday > tLastLoginDay );

}
#endif SERV_SECOND_SECURITY
//}}

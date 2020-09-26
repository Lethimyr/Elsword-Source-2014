#include ".\henirranking.h"
#include <algorithm>
#include "Enum/Enum.h"
//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	#include "LoginServer.h"
#endif SERV_INTEGRATION
//}}

KHenirRanking::KHenirRanking(void) :
m_uiLastRank( 0 ),
m_bIsRankingChange( false ),
m_iRankingType( -1 ),
m_iRefreshWeekOfMonth( 0 ),
m_iRefreshDayOfWeek( 0 ),
m_iRefreshHour( 0 )
{
}

KHenirRanking::~KHenirRanking(void)
{
}

void KHenirRanking::SetRefreshTime( const int iRankingType, const int iWeekOfMonth, const int iDayOfWeek, const int iHour )
{
	m_iRankingType = iRankingType;
	m_iRefreshWeekOfMonth = iWeekOfMonth;
	m_iRefreshDayOfWeek = iDayOfWeek;
	m_iRefreshHour = iHour;

	switch( m_iRankingType )
	{
	case SEnum::RT_DAY_RANKING:
		{
			// 일별 랭킹 초기화 시간 설정
			CTime tCurTime	 = CTime::GetCurrentTime();
			m_tRefreshTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), iHour, 0, 0 );
			if( m_tRefreshTime < tCurTime )
			{
				// 이미 지나간 시간이라면 다음날로 설정
				m_tRefreshTime += CTimeSpan( 1, 0, 0, 0 );
			}

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"일간 랭킹 갱신 일자 설정!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_WEEK_RANKING:
		{				
			CTime tCurTime = CTime::GetCurrentTime();

			// 매주 무슨요일 몇시
			CTime tResetTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), iHour, 0, 0 );

			int iIncreaseDayCount = iDayOfWeek - tResetTime.GetDayOfWeek(); // 해당 요일까지 몇일 부족한지 계산			
			tResetTime += CTimeSpan( iIncreaseDayCount, 0, 0, 0 );			// 부족한 요일만큼 더하자!

			if( tResetTime < tCurTime )
			{
				// ResetTime이 오늘보다 과거라면 일주일을 더한다
				tResetTime += CTimeSpan( 7, 0, 0, 0 );
			}

			// 갱신 시각 설정
			m_tRefreshTime = tResetTime;

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"주간 랭킹 갱신 일자 설정!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_MONTH_RANKING:
		{			
			CTime tCurTime = CTime::GetCurrentTime();

			// 그달의 몇번째주 무슨요일 몇시
			CTime tResetTime = GetDayOfWeekInMonth( tCurTime.GetYear(), tCurTime.GetMonth(), iWeekOfMonth, iDayOfWeek, iHour );

			// 지금 보다 과거인가?
			if( tResetTime < tCurTime )
			{
				// 한달 다음으로 돌리고..
				int iYear = tResetTime.GetYear();
				int iNextMonth = tResetTime.GetMonth() + 1;
				if( iNextMonth > 12 )
				{
					++iYear; // 다음해
					iNextMonth = 1; // 1월
				}

				tResetTime = GetDayOfWeekInMonth( iYear, iNextMonth, iWeekOfMonth, iDayOfWeek, iHour );
			}

			// 갱신 시각 설정
			m_tRefreshTime = tResetTime;

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"월간 랭킹 갱신 일자 설정!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	case SEnum::RT_HERO_RANKING:
		{			
			CTime tCurTime = CTime::GetCurrentTime();

			// 그달의 몇번째주 무슨요일 몇시
			CTime tResetTime = GetDayOfWeekInMonth( tCurTime.GetYear(), tCurTime.GetMonth(), iWeekOfMonth, iDayOfWeek, iHour );

			// 지금 보다 과거인가?
			if( tResetTime < tCurTime )
			{
				// 한달 다음으로 돌리고..
				int iYear = tResetTime.GetYear();
				int iNextMonth = tResetTime.GetMonth() + 1;
				if( iNextMonth > 12 )
				{
					++iYear; // 다음해
					iNextMonth = 1; // 1월
				}

				tResetTime = GetDayOfWeekInMonth( iYear, iNextMonth, iWeekOfMonth, iDayOfWeek, iHour );
			}

			// 갱신 시각 설정
			m_tRefreshTime = tResetTime;

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"영웅 랭킹 갱신 일자 설정!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;
#endif // SERV_HENIR_RENEWAL_2013

	default:
		{
            START_LOG( cerr, L"랭킹 타입이 이상합니다. 갱신날짜 설정 실패!" )
				<< BUILD_LOG( m_iRankingType )
				<< END_LOG;
		}
		break;
	}
}

void KHenirRanking::NextRefreshTimeSet()
{
	// 다음 갱신 시각을 결정!
	switch( m_iRankingType )
	{
	case SEnum::RT_DAY_RANKING:
		{
			m_tRefreshTime += CTimeSpan( 1, 0, 0, 0 );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"일간 랭킹 갱신 일자를 다음 갱신 날짜로 수정 완료!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_WEEK_RANKING:
		{
			m_tRefreshTime += CTimeSpan( 7, 0, 0, 0 );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"주간 랭킹 갱신 일자를 다음 갱신 날짜로 수정 완료!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case SEnum::RT_MONTH_RANKING:
		{
			int iYear = m_tRefreshTime.GetYear();
			int iNextMonth = m_tRefreshTime.GetMonth() + 1;
			if( iNextMonth > 12 )
			{
				++iYear; // 다음해
				iNextMonth = 1; // 1월
			}

			m_tRefreshTime = GetDayOfWeekInMonth( iYear, iNextMonth, m_iRefreshWeekOfMonth, m_iRefreshDayOfWeek, m_iRefreshHour );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"월간 랭킹 갱신 일자를 다음 갱신 날짜로 수정 완료!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	case SEnum::RT_HERO_RANKING:
		{
			int iYear = m_tRefreshTime.GetYear();
			int iNextMonth = m_tRefreshTime.GetMonth() + 1;
			if( iNextMonth > 12 )
			{
				++iYear; // 다음해
				iNextMonth = 1; // 1월
			}

			m_tRefreshTime = GetDayOfWeekInMonth( iYear, iNextMonth, m_iRefreshWeekOfMonth, m_iRefreshDayOfWeek, m_iRefreshHour );

			//////////////////////////////////////////////////////////////////////////
			CStringW strRefreshTime = (CStringW)m_tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			START_LOG( cout, L"영웅 랭킹 갱신 일자를 다음 갱신 날짜로 수정 완료!" )
				<< BUILD_LOG( strRefreshTime.GetBuffer() );
			//////////////////////////////////////////////////////////////////////////
		}
		break;
#endif // SERV_HENIR_RENEWAL_2013

	default:
		{
			START_LOG( cerr, L"랭킹 타입이 이상합니다. 갱신날짜 설정 실패!" )
				<< BUILD_LOG( m_iRankingType )
				<< END_LOG;
		}
		break;
	}
}

void KHenirRanking::SetRewardBeginTime( const std::wstring& wstrBeginTime )
{
	std::wstring wstrTemp = wstrBeginTime;

	if( KncUtil::ConvertStringToCTime( wstrTemp, m_tRewardBeginTime ) == false )
	{
        START_LOG( cerr, L"랭킹 시작시각 등록 실패!" )
			<< BUILD_LOG( m_iRankingType )
			<< BUILD_LOG( wstrBeginTime )
			<< END_LOG;
	}
	else
	{
		START_LOG( cout, L"랭킹 시작시각 등록 성공!" )
			<< BUILD_LOG( m_iRankingType )
			<< BUILD_LOG( wstrBeginTime );
	}
}

#ifdef SERV_HENIR_RANKING_GROUP_FIX
bool KHenirRanking::RankingSort( const UidType& iUnitUID )
{
	bool bRankOK = true;

	// 일단 좋은 기록 순으로 정렬
	std::sort( m_vecRankingInfo.begin(), m_vecRankingInfo.end(), &KHenirRankingInfo::RankCompare );

	// 정렬된 랭킹정보에 랭크 매기자! ( 동일한 기록이면 같은 랭크 )
	int iOverLastRankCount = 0;
	{	
		KHenirRankingInfo kBeforeRank;

		for( unsigned int ui = 0; ui < m_vecRankingInfo.size(); ++ui )
		{
			KHenirRankingInfo& kRankInfo = m_vecRankingInfo[ui];

			// 공동 랭크 검사
			if( KHenirRankingInfo::IsSameRank( kRankInfo, kBeforeRank ) )
			{
				kRankInfo.m_iRank = kBeforeRank.m_iRank;
			}
			else
			{
				kRankInfo.m_iRank = static_cast<int>(ui + 1);
			}

			kBeforeRank = kRankInfo;

			// 마지막 랭크를 넘었다면 카운트
			if( kRankInfo.m_iRank > static_cast<int>(m_uiLastRank) )
			{
				++iOverLastRankCount;
			}
		}
	}

	// 마지막 랭크보다 낮은 랭크들은 뒤에서부터 자른다
	for( int iIndex = 0; iIndex < iOverLastRankCount; ++iIndex )
	{
		const KHenirRankingInfo& kOverLastRank = m_vecRankingInfo.back();
		if( iUnitUID == kOverLastRank.m_iUnitUID )
			bRankOK = false;
		m_vecRankingInfo.pop_back();
	}

	return bRankOK;
}
#endif SERV_HENIR_RANKING_GROUP_FIX

void KHenirRanking::RankingSort()
{
	// 일단 좋은 기록 순으로 정렬
	std::sort( m_vecRankingInfo.begin(), m_vecRankingInfo.end(), &KHenirRankingInfo::RankCompare );

	// 정렬된 랭킹정보에 랭크 매기자! ( 동일한 기록이면 같은 랭크 )
	int iOverLastRankCount = 0;
	{	
		KHenirRankingInfo kBeforeRank;

		for( unsigned int ui = 0; ui < m_vecRankingInfo.size(); ++ui )
		{
			KHenirRankingInfo& kRankInfo = m_vecRankingInfo[ui];

			// 공동 랭크 검사
			if( KHenirRankingInfo::IsSameRank( kRankInfo, kBeforeRank ) )
			{
				kRankInfo.m_iRank = kBeforeRank.m_iRank;
			}
			else
			{
				kRankInfo.m_iRank = static_cast<int>(ui + 1);
			}

			kBeforeRank = kRankInfo;

			// 마지막 랭크를 넘었다면 카운트
			if( kRankInfo.m_iRank > static_cast<int>(m_uiLastRank) )
			{
				++iOverLastRankCount;
			}
		}
	}

	// 마지막 랭크보다 낮은 랭크들은 뒤에서부터 자른다
	for( int iIndex = 0; iIndex < iOverLastRankCount; ++iIndex )
	{
		m_vecRankingInfo.pop_back();
	}
}

CTime KHenirRanking::GetDayOfWeekInMonth( int iYear, int iMonth, int iWeekOfMonth, int iDayOfWeek, int iHour )
{
	// 그달의 첫번째날 구하기
	CTime tFirstDayOfMonth = CTime( iYear, iMonth, 1, iHour, 0, 0 );

	// 그달의 첫번째 해당 요일 구하기
	int iIncreaseDayCount = iDayOfWeek - tFirstDayOfMonth.GetDayOfWeek();
	if( iIncreaseDayCount < 0 )
	{
		iIncreaseDayCount = 7 + iIncreaseDayCount;
	}
	tFirstDayOfMonth += CTimeSpan( iIncreaseDayCount, 0, 0, 0 );

	// 그달의 몇번째주 무슨요일의 CTime 구하기
	CTime tDayOfMonth = tFirstDayOfMonth + CTimeSpan( ( 7 * ( iWeekOfMonth - 1 ) ), 0, 0, 0 );

	return tDayOfMonth;
}

bool KHenirRanking::CheckNewRecord( IN const KHenirRankingInfo& kNewRankingInfo )
{
	std::vector< UidType >::const_iterator vitFIND;
	std::vector< KHenirRankingInfo >::reverse_iterator vitTR;

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	// 0. 해당 서버군 유저인지 확인한다!
	if( KncUid::ExtractServerGroupID( kNewRankingInfo.m_iUnitUID ) != GetKLoginServer()->GetServerGroupID() )
	{
		START_LOG( cerr, L"해당 서버군 유저가 아니라서 랭킹 등록 실패!" )
			<< BUILD_LOG( kNewRankingInfo.m_iUnitUID )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kNewRankingInfo.m_iUnitUID ) )
			<< BUILD_LOG( GetKLoginServer()->GetServerGroupID() )
			<< END_LOG;

		return false;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// 1. 기존의 랭킹리스트에 자신의 기록이 있는지 찾아보자!
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = m_vecRankingInfo.begin(); vitFind != m_vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_iUnitUID != kNewRankingInfo.m_iUnitUID )
			continue;

		if( KHenirRankingInfo::RankCompare( kNewRankingInfo, *vitFind ) == false )
		{
			// 기존의 자신의 기록과 비교해보니 더 낮은 기록이면 무시한다
			return false;
		}
		else
		{
			// 더 높은 기록이면 기록을 덮어씌우자!
			*vitFind = kNewRankingInfo;

			// 랭킹 소트로..
			goto end_proc;
		}
	}

	// 2. 현재 랭킹정보리스트가 100위보다 작다면..?
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	if( m_vecRankingInfo.size() < ( m_uiLastRank + 3 ) )	// 공동 100위가 존재할 수 있으므로 일단 랭킹 정보 갯수를 103개까지 저장해둔다.
#else
	if( m_vecRankingInfo.size() < m_uiLastRank )
#endif SERV_HENIR_RANKING_GROUP_FIX
	{
        m_vecRankingInfo.push_back( kNewRankingInfo );

		// 랭킹 소트로..
		goto end_proc;
	}

	// 3. 마지막 랭크와 비교하여 100위안에 드는지 찾아본다	
	vitTR = m_vecRankingInfo.rbegin();
	if( vitTR == m_vecRankingInfo.rend() )
	{
		START_LOG( cerr, L"랭킹 리스트가 이상함" )
			<< BUILD_LOG( kNewRankingInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	const KHenirRankingInfo& kOldRankingInfo = *vitTR;
	if( KHenirRankingInfo::RankCompare( kNewRankingInfo, kOldRankingInfo ) == false )
	{
		// 100위안에 들지 못했다!
		return false;
	}

	// 4. 100위안에 드는지 확인되었으니 랭킹리스트에 넣고 소트하자
	m_vecRankingInfo.push_back( kNewRankingInfo );

end_proc:
	// 5. 랭킹 소트
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	if( RankingSort(kNewRankingInfo.m_iUnitUID) == false )
		return false;
#else
	RankingSort();
#endif SERV_HENIR_RANKING_GROUP_FIX

	// 6. 랭킹 정보 refresh 예약
	SetRankingChanged( true );
	return true;
}

bool KHenirRanking::DeleteRecord( IN const std::wstring& wstrNickName )
{
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = m_vecRankingInfo.begin(); vitFind != m_vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_wstrNickName == wstrNickName )
			break;
	}

	if( vitFind == m_vecRankingInfo.end() )
	{
		START_LOG( cout, L"랭킹에서 지울 유저를 찾지 못했다!" )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( m_iRankingType )
			<< BUILD_LOG( m_vecRankingInfo.size() )
			<< END_LOG;

		return false;
	}

	START_LOG( cout2, L"----- 랭킹 삭제 정보 -----" )
		<< BUILD_LOG( vitFind->m_iRank )
		<< BUILD_LOG( vitFind->m_iStageCount )
		<< BUILD_LOG( vitFind->m_ulPlayTime )
		<< BUILD_LOG( vitFind->m_tRegDate )
		<< BUILD_LOG( vitFind->m_iUnitUID )
		<< BUILD_LOG( vitFind->m_wstrNickName )
		<< BUILD_LOGc( vitFind->m_cUnitClass )
		<< BUILD_LOGc( vitFind->m_ucLevel );

	// 찾았다! 지우자!
	m_vecRankingInfo.erase( vitFind );

	// 랭킹 소트
	RankingSort();

	// 랭킹 정보 refresh 예약
	SetRankingChanged( true );
	return true;
}

bool KHenirRanking::DeleteRecord( IN const UidType iUnitUID )
{
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = m_vecRankingInfo.begin(); vitFind != m_vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_iUnitUID == iUnitUID )
			break;
	}

	if( vitFind == m_vecRankingInfo.end() )
		return false;

	// 찾았다! 지우자!
	m_vecRankingInfo.erase( vitFind );

	// 랭킹 소트
	RankingSort();

	// 랭킹 정보 refresh 예약
	SetRankingChanged( true );
	return true;
}

void KHenirRanking::SetRankingInfo( const std::vector< KHenirRankingInfo >& vecRankingInfo )
{
	m_vecRankingInfo.clear();
    m_vecRankingInfo = vecRankingInfo;

	// 랭킹 소트
	RankingSort();
}

void KHenirRanking::ClearRanking()
{
	m_vecRankingInfo.clear();
	SetRankingChanged( true ); // 게임서버로 브로드 캐스팅 되도록 flag값 변경
}



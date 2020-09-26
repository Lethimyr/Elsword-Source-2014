#include ".\gshenirranking.h"

KGSHenirRanking::KGSHenirRanking(void)
{
}

KGSHenirRanking::~KGSHenirRanking(void)
{
}

//{{ 2009. 10. 27  최육사	캐릭터삭제
bool KGSHenirRanking::IsInRankingUser( UidType iUnitUID ) const
{
	std::vector< KHenirRankingInfo >::const_iterator vit;
	for( vit = m_vecRankingInfo.begin(); vit != m_vecRankingInfo.begin(); ++vit )
	{
        if( vit->m_iUnitUID == iUnitUID )
			return true;
	}

	return false;
}
//}}

bool KGSHenirRanking::CheckNewRecord( IN const KHenirRankingInfo& kNewRank ) const
{
#ifdef SERV_HENIR_RANKING_GROUP_FIX	// 103위 까지 다 넣음
	if( m_vecRankingInfo.size() >= m_uiLastRank + 3 )
#else
	if( m_vecRankingInfo.size() >= m_uiLastRank )
#endif
	{
		const KHenirRankingInfo& kOldRank = m_vecRankingInfo.back();

		if( !KHenirRankingInfo::RankCompare( kNewRank, kOldRank ) )
			return false; // 순위권 안에 들지 못했음.
	}

	// 순위권 안에 들었음.
	return true;
}

void KGSHenirRanking::UpdateRankingInfo( IN const std::vector< KHenirRankingInfo >& vecRankingInfo )
{
	m_vecRankingInfo.clear();
	m_vecRankingInfo = vecRankingInfo;
}

void KGSHenirRanking::GetRankingInfo( IN OUT UINT& uiViewPage, OUT UINT& uiTotalPage, OUT std::vector< KHenirRankingInfo >& vecRankingInfo ) const
{
	vecRankingInfo.clear();

	_JIF( uiViewPage > 0, return; );

	if( m_vecRankingInfo.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return;
	}

	uiTotalPage = m_vecRankingInfo.size() / PAGE_PER_COUNT;
	if( ( m_vecRankingInfo.size() % PAGE_PER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * PAGE_PER_COUNT ) - PAGE_PER_COUNT;
	if( m_vecRankingInfo.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // 마지막 페이지
		uiBeginIndex = ( uiViewPage * PAGE_PER_COUNT ) - PAGE_PER_COUNT;

		if( m_vecRankingInfo.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"일어날 수 없는 에러!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecRankingInfo.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KHenirRankingInfo >::const_iterator vit = m_vecRankingInfo.begin();
	vit += uiBeginIndex;

	while( vit != m_vecRankingInfo.end() )
	{
		vecRankingInfo.push_back( *vit );

		if( vecRankingInfo.size() >= PAGE_PER_COUNT )
			break;

		++vit;
	}
}




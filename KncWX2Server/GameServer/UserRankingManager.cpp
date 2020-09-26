#include "GSUser.h"
#include ".\UserRankingManager.h"
#include "GSRankingManager.h"


KUserRankingManager::KUserRankingManager(void) :
m_bHenirRankingInited( false )
, m_bHenirRankingChanged( false )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
, m_bHenirHeroRankingInited( false )
, m_bHenirHeroRankingChanged( false )
#endif // SERV_HENIR_RENEWAL_2013
{	
}

KUserRankingManager::~KUserRankingManager(void)
{
}

void KUserRankingManager::clear()
{
	m_bHenirRankingInited = false;
	m_bHenirRankingChanged = false;
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
	m_bHenirHeroRankingInited = false;
	m_bHenirHeroRankingChanged = false;
#endif // SERV_HENIR_RENEWAL_2013
}

void KUserRankingManager::Init( IN const std::vector< KHenirRankingInfo >& vecHenirRanking
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
	, IN const std::vector< KHenirRankingInfo >& vecHenirHeroRanking
#endif // SERV_HENIR_RENEWAL_2013
	)
{
	std::vector< KHenirRankingInfo >::const_iterator vit = vecHenirRanking.begin();
	if( vit != vecHenirRanking.end() )
	{
        m_kHenirRanking = *vit;
        m_bHenirRankingInited = true;
	}

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
	vit = vecHenirHeroRanking.begin();
	if( vit != vecHenirHeroRanking.end() )
	{
		m_kHenirHeroRanking = *vit;
		m_bHenirHeroRankingInited = true;
	}
#endif // SERV_HENIR_RENEWAL_2013
}

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
void KUserRankingManager::GetUpdateInfo( OUT std::vector< KHenirRankingInfo >& vecHenirRanking, OUT std::vector< KHenirRankingInfo >& vecHenirHeroRanking )
{
	vecHenirRanking.clear();
	vecHenirHeroRanking.clear();

	if( m_bHenirRankingInited  &&  m_bHenirRankingChanged )
	{
		vecHenirRanking.push_back( m_kHenirRanking );
	}

	if( m_bHenirHeroRankingInited  &&  m_bHenirHeroRankingChanged )
	{
		vecHenirHeroRanking.push_back( m_kHenirHeroRanking );
	}
}
#else // SERV_HENIR_RENEWAL_2013
void KUserRankingManager::GetUpdateInfo( OUT std::vector< KHenirRankingInfo >& vecHenirRanking )
{
	vecHenirRanking.clear();

	if( m_bHenirRankingInited  &&  m_bHenirRankingChanged )
	{
		vecHenirRanking.push_back( m_kHenirRanking );
	}
}
#endif // SERV_HENIR_RENEWAL_2013

void KUserRankingManager::GetRankingInfo( IN KGSUserPtr spUser, IN const KEGS_GET_RANKING_INFO_REQ& kReq, OUT KEGS_GET_RANKING_INFO_ACK& kAck )
{
	switch( kReq.m_iRankingType )
	{
	case SEnum::RT_DAY_RANKING:
	case SEnum::RT_WEEK_RANKING:
	case SEnum::RT_MONTH_RANKING:
		{
			if( m_bHenirRankingInited )
			{
                kAck.m_vecMyHenirRanking.push_back( m_kHenirRanking );
			}
		}
		break;

	case SEnum::RT_DUNGEON_RANKING:
		{
			KDungeonRankingInfo kRankingInfo;
			if( SiKGSRankingManager()->GetDungeonRankingByUnitUID( spUser->GetCharUID(), kRankingInfo ) == true )
			{
				kAck.m_vecMyDungeonRanking.push_back( kRankingInfo );
			}
		}
		break;

	case SEnum::RT_PVP_RANKING:
		{
			KPvpRankingInfo kRankingInfo;
			if( SiKGSRankingManager()->GetPvpRankingByUnitUID( spUser->GetCharUID(), kRankingInfo ) == true )
			{
				kAck.m_vecMyPvpRanking.push_back( kRankingInfo );
			}
		}
		break;

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
	case SEnum::RT_HERO_RANKING:
		if( m_bHenirHeroRankingInited == true )
		{
			kAck.m_vecMyHenirRanking.push_back( m_kHenirHeroRanking );
		}
		break;
#endif // SERV_HENIR_RENEWAL_2013

	default:
		{
			START_LOG( cerr, L"존재하지 않는 RankingType입니다" )
				<< BUILD_LOG( kReq.m_iRankingType )
				<< END_LOG;
		}
	}
}

void KUserRankingManager::SetRankingInfo( IN const KHenirRankingInfo& kInfo )
{
	m_kHenirRanking = kInfo;
	m_bHenirRankingInited = true;
	m_bHenirRankingChanged = true;
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
	if( kInfo.QualificationForHeroRank() == true )
	{
		m_kHenirHeroRanking = kInfo;
		m_bHenirHeroRankingInited = true;
		m_bHenirHeroRankingChanged = true;
	}
#endif // SERV_HENIR_RENEWAL_2013
}



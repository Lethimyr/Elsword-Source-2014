#include "UserLocalRankingInfo.h"

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
#include "GSLocalRankingManager.h"
#include "..\Common\GameSysVal\GameSysVal.h"
KUserLocalRankingInfo::KUserLocalRankingInfo()
: m_iUID( 0 )
{
	for( int i = 0; i < SEnum::LRMTC_MAX; ++i )
	{
		m_iPoint[i].SetMaxValue( 2000000000 );
		m_iPoint[i].SetValue( 0 );
	}
}

void KUserLocalRankingInfo::GetFilteredUserInfo( OUT KLocalRankingUserInfo& kInfo ) const
{
	kInfo.m_iUserUID	= m_kLocalRankingUserInfo.m_iUserUID;
	kInfo.m_byteFilter	= m_kLocalRankingUserInfo.m_byteFilter;

	if( SEnum::CheckFlag( m_kLocalRankingUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) == false )
	{
		kInfo.m_iCategory = m_kLocalRankingUserInfo.m_iCategory;
	}

	if( SEnum::CheckFlag( m_kLocalRankingUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_BIRTH ) == false )
	{
		kInfo.m_iBirth = m_kLocalRankingUserInfo.m_iBirth;
	}

	if( SEnum::CheckFlag( m_kLocalRankingUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_PROFILE ) == false )
	{
		kInfo.m_wstrProfile = m_kLocalRankingUserInfo.m_wstrProfile;
	}

	if( SEnum::CheckFlag( m_kLocalRankingUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == false )
	{
		kInfo.m_byteGender = m_kLocalRankingUserInfo.m_byteGender;
	}
}

bool KUserLocalRankingInfo::CheckUpdate( void ) const
{
	if( m_wstrUpdated.empty() == true )
	{
		return true;
	}

	CTime tUpdated;
	if( KncUtil::ConvertStringToCTime( m_wstrUpdated, tUpdated ) == false )
	{
		return false;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();
	CTime tToday( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 6, 0, 0 );

	if( tToday <= tUpdated )
	{
		return false;
	}

	return true;
}

bool KUserLocalRankingInfo::UpdateTimeCheck( void )
{
	if( m_kElpUpdateGap.elapsed() < SiKGameSysVal()->GetLocalRankingElpUpdateGap() )
	{
		return false;
	}

	m_kElpUpdateGap.restart();
	return true;
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
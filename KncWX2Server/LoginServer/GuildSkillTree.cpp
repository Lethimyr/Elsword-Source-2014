#include ".\guildskilltree.h"
#include "NetError.h"

// 스킬 개편
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27

KGuildSkillTree::KGuildSkillTree(void) :
m_iGuildCSPoint( 0 ),
m_iMaxGuildCSPoint( 0 ),
m_tGuildCSPointEndDate( 0 )
{
}

KGuildSkillTree::~KGuildSkillTree(void)
{
}

void KGuildSkillTree::UpdateGuildSkillTree( IN const KGuildSkillInfo& kInfo )
{
	m_mapSkillTree.clear();

	SetGuildSPoint( kInfo.m_iGuildSPoint );
	SetGuildCSPoint( kInfo.m_iGuildCSPoint );
	SetMaxGuildCSPoint( kInfo.m_iMaxGuildCSPoint );
	SetGuildCSPointEndDate( kInfo.m_wstrGuildCSPointEndDate );

	for( u_int ui = 0; ui < kInfo.m_vecGuildSkill.size(); ++ui )
	{
		const KGuildSkillData& kGuildSkillData = kInfo.m_vecGuildSkill[ui];

		if( m_mapSkillTree.find( kGuildSkillData.m_iSkillID ) != m_mapSkillTree.end() )
		{
			START_LOG( cerr, L"중복된 스킬을 보유하고 있음." )
				<< BUILD_LOG( kGuildSkillData.m_iSkillID )
				<< BUILD_LOG( kGuildSkillData.m_cSkillLevel )
				<< BUILD_LOG( kGuildSkillData.m_cSkillCSPoint )
				<< END_LOG;

			continue;
		}

		int iSkillLevel = static_cast<int>(kGuildSkillData.m_cSkillLevel);

		// 스킬의 최고레벨보다 높은 스킬이 있다면 최고레벨로 보정
		const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( kGuildSkillData.m_iSkillID );
		if( NULL != pSkillTreeTemplet )
		{
			if( static_cast<int>(kGuildSkillData.m_cSkillLevel) > pSkillTreeTemplet->m_iMasterSkillLevel )
			{
				START_LOG( cerr, L"최고레벨보다 높은 레벨의 스킬이 DB에 있음" )
					<< BUILD_LOG( kGuildSkillData.m_iSkillID )
					<< BUILD_LOGc( kGuildSkillData.m_cSkillLevel )
					<< BUILD_LOG( pSkillTreeTemplet->m_iMasterSkillLevel )
					<< BUILD_LOGc( kGuildSkillData.m_cSkillCSPoint )
					<< END_LOG;

				iSkillLevel = pSkillTreeTemplet->m_iMasterSkillLevel;
			}
		}

		// 이미 존재하면 삽입되지 않고 존재하지 않으면 삽입된다.
		GuildSkill kSkillData( kGuildSkillData.m_cSkillLevel, kGuildSkillData.m_cSkillCSPoint );
		m_mapSkillTree.insert( std::make_pair( kGuildSkillData.m_iSkillID, kSkillData ) );
	}
}

void KGuildSkillTree::GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo ) const
{
	kInfo.m_vecGuildSkill.clear();

	GuildSkillDataMap::const_iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		KGuildSkillData kGuildSkillData;
		kGuildSkillData.m_iSkillID = mit->first;
		kGuildSkillData.m_cSkillLevel = mit->second.m_iSkillLevel;
		kGuildSkillData.m_cSkillCSPoint = mit->second.m_iSkillCSPoint;
		kInfo.m_vecGuildSkill.push_back( kGuildSkillData );
	}

	kInfo.m_iGuildSPoint = GetGuildSPoint();
	kInfo.m_iGuildCSPoint = GetGuildCSPoint();
	kInfo.m_iMaxGuildCSPoint = GetMaxGuildCSPoint();
	kInfo.m_wstrGuildCSPointEndDate = m_wstrGuildCSPointEndDate;
}

void KGuildSkillTree::ResetGuildSkillTree()
{
	m_mapSkillTree.clear();
}

int KGuildSkillTree::GetSkillLevel( int iSkillID ) const
{
	GuildSkillDataMap::const_iterator mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		return mit->second.m_iSkillLevel;
	}

	return 0;
}

bool KGuildSkillTree::SetGuildCSPointEndDate( IN const std::wstring& wstrEndDate )
{
	if( wstrEndDate.empty() )
		return false;

	if( KncUtil::ConvertStringToCTime( wstrEndDate, m_tGuildCSPointEndDate ) == false )
		return false;

	m_wstrGuildCSPointEndDate = wstrEndDate;
	return true;
}

bool KGuildSkillTree::SetSkillLevelAndCSP( int iSkillID, int iSkillLevel, int iSkillCSPoint )
{
	SET_ERROR( NET_OK );

	int iCheckSkillLevel = iSkillLevel;
	if( 0 == iSkillLevel )
	{
		iCheckSkillLevel = 1;
	}
	else if( iSkillLevel < 0 )
	{
		SET_ERROR( ERR_SKILL_20 );

		return false;
	}

	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( iSkillID );

	if( NULL == pSkillTemplet )
	{
		SET_ERROR( ERR_SKILL_19 );
		return false;
	}

	if( iSkillLevel < iSkillCSPoint )
	{
		SET_ERROR( ERR_SKILL_20 );
		return false;
	}

	if( iSkillLevel > SiCXSLSkillTree()->GetMasterGuildSkillLevel( iSkillID ) )
	{
		SET_ERROR( ERR_SKILL_21 );
		return false;
	}

	GuildSkillDataMap::iterator mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		GuildSkill& kGuildSkillData = mit->second;
		kGuildSkillData.m_iSkillLevel	= iSkillLevel;
		kGuildSkillData.m_iSkillCSPoint	= iSkillCSPoint;
	}
	else 
	{
		m_mapSkillTree[ iSkillID ] = GuildSkill( iSkillLevel, iSkillCSPoint );
	}

	return true;
}

bool KGuildSkillTree::IsMasterSkillLevel( IN int iSkillID )
{
	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( iSkillID );
	if( NULL == pSkillTemplet )
	{
		START_LOG( cerr, L"길드 스킬이 최고 레벨인지 확인하려는데 스킬 템플릿이 NULL" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}

	GuildSkillDataMap::const_iterator mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		const GuildSkill& kGuildSkillData = mit->second;
		int iMasterSkillLevel = SiCXSLSkillTree()->GetMasterGuildSkillLevel( iSkillID );
		if( kGuildSkillData.m_iSkillLevel > iMasterSkillLevel )
		{
			START_LOG( cerr, L"길드 스킬이 최고 레벨인지 확인하려는데 스킬 레벨이 최고레벨보다 높다" )
				<< BUILD_LOG( iSkillID )
				<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
				<< BUILD_LOG( iMasterSkillLevel )
				<< END_LOG;

			return true;
		}
		else if( kGuildSkillData.m_iSkillLevel == iMasterSkillLevel )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	return false;
}

bool KGuildSkillTree::IsAllPrecedingSkillMasterLevel( int iSkillID )
{
	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( iSkillID );
	if( NULL == pSkillTreeTemplet )
	{
		START_LOG( cerr, L"스킬트리 템플릿이 없음_preceding" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}

	if( pSkillTreeTemplet->m_iPrecedingSkill > 0 )
	{
		if( IsAllPrecedingSkillMasterLevel( pSkillTreeTemplet->m_iPrecedingSkill ) == false )
		{
			return false;
		}
	}

	return true;
}

bool KGuildSkillTree::IsAllFollowingSkillLevelZero( int iSkillID )
{
	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( iSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		START_LOG( cerr, L"스킬트리 템플릿이 없음_following" )			
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}

	for( u_int ui = 0; ui < pSkillTreeTemplet->m_vecFollowingSkill.size(); ++ui )
	{
		if( GetSkillLevel( pSkillTreeTemplet->m_vecFollowingSkill[ui] ) > 0 )
			return false;
	}

	return true;
}

// 이 스킬을 배우기에 충분할 만큼 SP를 소모했는지
bool KGuildSkillTree::IsTierOpened( int iSkillID )
{
	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( iSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		START_LOG( cerr, L"스킬트리 템플릿이 없음_IsTierOpened" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}


	const int MAGIC_SKILL_POINT_PER_TIER = 5;
	int iUsedSPoint = 0;
	int iUsedCSPoint = 0;
	CalcUsedSPointAndCSPoint( iUsedSPoint, iUsedCSPoint );
	if( iUsedSPoint + iUsedCSPoint < pSkillTreeTemplet->m_iTier * MAGIC_SKILL_POINT_PER_TIER )
	{
		return false;
	}

	return true;
}

// tier별로 소모된 누적 SP+CSP를 계산해준다
void KGuildSkillTree::CalcCumulativeUsedSPointOnEachTier( OUT std::vector< int >& vecTierSPoint )
{	
	int iMaxTierIndex = 0;
	for( GuildSkillDataMap::const_iterator mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{		
		const GuildSkill& kGuildSkillData = mit->second;

		// 스킬 레벨값이 유효한지 검사		
		if( kGuildSkillData.m_iSkillLevel <= 0 )
			continue;

		const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( mit->first );
		if( pSkillTreeTemplet == NULL )
			continue;

		if( iMaxTierIndex < pSkillTreeTemplet->m_iTier )
			iMaxTierIndex = pSkillTreeTemplet->m_iTier;
	}

	vecTierSPoint.resize(0);
	for( int i = 0; i <= iMaxTierIndex; ++i )
	{
		vecTierSPoint.push_back( 0 );
	}

	for( GuildSkillDataMap::const_iterator mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const GuildSkill& kGuildSkillData = mit->second;

		// 스킬 레벨값이 유효한지 검사
		if( kGuildSkillData.m_iSkillLevel <= 0 )
			continue;

		const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( mit->first );
		if( pSkillTreeTemplet == NULL )
			continue;

		//{{ 2010. 8. 17	최육사	스킬 되돌리기 버그 수정
		vecTierSPoint[ pSkillTreeTemplet->m_iTier ] += ( kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint );
	}

	int iCumulativeTierSPoint = 0;
	for( u_int ui = 0; ui < vecTierSPoint.size(); ++ui )
	{
		iCumulativeTierSPoint += vecTierSPoint[ui];
		vecTierSPoint[ui] = iCumulativeTierSPoint;
	}
}

bool KGuildSkillTree::IsCashSkillPointExpired()
{
	if( 0 == m_iMaxGuildCSPoint )
		return true;

	return false;
}

void KGuildSkillTree::CalcExpireCashSkillPoint( OUT int& iGuildSPoint, OUT int& iRetrievedSPoint, OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData )
{
	vecModifiedGuildSkillData.clear();
	iRetrievedSPoint = 0;		

	GuildSkillDataMap::const_iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{		
		const GuildSkill& kGuildSkillData = mit->second;

		if( kGuildSkillData.m_iSkillCSPoint > 0 )
		{
			iRetrievedSPoint += kGuildSkillData.m_iSkillCSPoint;
			const u_char ucNewSkillLevel = static_cast<u_char>( kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint );

			vecModifiedGuildSkillData.push_back( KGuildSkillData( static_cast<short>(mit->first), ucNewSkillLevel, 0 ) );
		}
	}

	// 스킬트리에 소모했던 CSP 와 남아있는 CSP 를 더해서 구매시의 CSP를 빼면 돌려줄 SP 수치가 계산된다
	iRetrievedSPoint += GetGuildCSPoint() - GetMaxGuildCSPoint();

	// 되돌릴 SP를 현재 SP와 합하여 DB에 업데이트!
	iGuildSPoint = GetGuildSPoint() + iRetrievedSPoint;
}

void KGuildSkillTree::ExpireCashSkillPoint( OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData )
{
	vecModifiedGuildSkillData.clear();

	// 실제로 cash skill point로 획득한 스킬을 되돌린다.
	GuildSkillDataMap::iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const int iSkillID = mit->first;
		GuildSkill& kGuildSkillData = mit->second;

		if( kGuildSkillData.m_iSkillCSPoint > 0 )
		{
			const u_char ucNewSkillLevel = static_cast<u_char>( kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint );

			kGuildSkillData.m_iSkillLevel = ucNewSkillLevel;
			kGuildSkillData.m_iSkillCSPoint = 0;
		}

		vecModifiedGuildSkillData.push_back( KGuildSkillData( static_cast<short>(mit->first), kGuildSkillData.m_iSkillLevel, 0 ) );
	}

	SetMaxGuildCSPoint( 0 );
	SetGuildCSPoint( 0 );
}

bool KGuildSkillTree::GetSkillLevelAndCSP( IN int iSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint )
{
	GuildSkillDataMap::const_iterator mit;
	mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		const GuildSkill& kGuildSkillData = mit->second;
		iSkillLevel		= kGuildSkillData.m_iSkillLevel;
		iSkillCSPoint	= kGuildSkillData.m_iSkillCSPoint;
		return true;
	}
	else
	{
		iSkillLevel		= 0;
		iSkillCSPoint	= 0;
	}

	return false;
}

void KGuildSkillTree::CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint )
{
	iSPoint = 0;
	iCSPoint = 0;

	GuildSkillDataMap::const_iterator mit;

	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const int iSkillID = mit->first;
		const GuildSkill& kGuildSkillData = mit->second;

		if( kGuildSkillData.m_iSkillLevel > 0 )
		{
			if( kGuildSkillData.m_iSkillCSPoint > kGuildSkillData.m_iSkillLevel )
			{
				START_LOG( cerr, L"스킬 레벨 < 스킬 CSP" )
					<< BUILD_LOG( iSkillID )
					<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
					<< BUILD_LOG( kGuildSkillData.m_iSkillCSPoint )
					<< END_LOG;
			}

			iSPoint		+= kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint;
			iCSPoint	+= kGuildSkillData.m_iSkillCSPoint;
		}
		else if( kGuildSkillData.m_iSkillLevel == 0 )
		{
			if( kGuildSkillData.m_iSkillCSPoint > 0 )
			{
				START_LOG( cwarn, L"스킬 레벨이 0이하인데 SkillCSP가 0보다 큼!" )
					<< BUILD_LOG( iSkillID )
					<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
					<< BUILD_LOG( kGuildSkillData.m_iSkillCSPoint )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cwarn, L"스킬 레벨이 0보다 작다!" )
				<< BUILD_LOG( iSkillID )
				<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
				<< BUILD_LOG( kGuildSkillData.m_iSkillCSPoint )
				<< END_LOG;
		}
	}
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
#ifdef GUILD_SKILL_TEST

KGuildSkillTree::KGuildSkillTree(void) :
m_iGuildCSPoint( 0 ),
m_iMaxGuildCSPoint( 0 ),
m_tGuildCSPointEndDate( 0 )
{
}

KGuildSkillTree::~KGuildSkillTree(void)
{
}

void KGuildSkillTree::UpdateGuildSkillTree( IN const KGuildSkillInfo& kInfo )
{
	m_mapSkillTree.clear();

	SetGuildSPoint( kInfo.m_iGuildSPoint );
	SetGuildCSPoint( kInfo.m_iGuildCSPoint );
	SetMaxGuildCSPoint( kInfo.m_iMaxGuildCSPoint );
	SetGuildCSPointEndDate( kInfo.m_wstrGuildCSPointEndDate );

	for( u_int ui = 0; ui < kInfo.m_vecGuildSkill.size(); ++ui )
	{
		const KGuildSkillData& kGuildSkillData = kInfo.m_vecGuildSkill[ui];

		if( m_mapSkillTree.find( kGuildSkillData.m_iSkillID ) != m_mapSkillTree.end() )
		{
			START_LOG( cerr, L"중복된 스킬을 보유하고 있음." )
				<< BUILD_LOG( kGuildSkillData.m_iSkillID )
				<< BUILD_LOG( kGuildSkillData.m_cSkillLevel )
				<< BUILD_LOG( kGuildSkillData.m_cSkillCSPoint )
				<< END_LOG;

			continue;
		}

		int iSkillLevel = static_cast<int>(kGuildSkillData.m_cSkillLevel);

		// 스킬의 최고레벨보다 높은 스킬이 있다면 최고레벨로 보정
		const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( kGuildSkillData.m_iSkillID );
		if( NULL != pSkillTreeTemplet )
		{
			if( static_cast<int>(kGuildSkillData.m_cSkillLevel) > pSkillTreeTemplet->m_iMaxSkillLevel )
			{
				START_LOG( cerr, L"최고레벨보다 높은 레벨의 스킬이 DB에 있음" )
					<< BUILD_LOG( kGuildSkillData.m_iSkillID )
					<< BUILD_LOGc( kGuildSkillData.m_cSkillLevel )
					<< BUILD_LOG( pSkillTreeTemplet->m_iMaxSkillLevel )
					<< BUILD_LOGc( kGuildSkillData.m_cSkillCSPoint )
					<< END_LOG;

				iSkillLevel = pSkillTreeTemplet->m_iMaxSkillLevel;
			}
		}

		// 이미 존재하면 삽입되지 않고 존재하지 않으면 삽입된다.
		GuildSkill kSkillData( kGuildSkillData.m_cSkillLevel, kGuildSkillData.m_cSkillCSPoint );
		m_mapSkillTree.insert( std::make_pair( kGuildSkillData.m_iSkillID, kSkillData ) );
	}
}

void KGuildSkillTree::GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo ) const
{
	kInfo.m_vecGuildSkill.clear();

	GuildSkillDataMap::const_iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
        KGuildSkillData kGuildSkillData;
		kGuildSkillData.m_iSkillID = mit->first;
		kGuildSkillData.m_cSkillLevel = mit->second.m_iSkillLevel;
		kGuildSkillData.m_cSkillCSPoint = mit->second.m_iSkillCSPoint;
		kInfo.m_vecGuildSkill.push_back( kGuildSkillData );
	}

	kInfo.m_iGuildSPoint = GetGuildSPoint();
	kInfo.m_iGuildCSPoint = GetGuildCSPoint();
	kInfo.m_iMaxGuildCSPoint = GetMaxGuildCSPoint();
	kInfo.m_wstrGuildCSPointEndDate = m_wstrGuildCSPointEndDate;
}

void KGuildSkillTree::ResetGuildSkillTree()
{
    m_mapSkillTree.clear();
}

int KGuildSkillTree::GetSkillLevel( int iSkillID ) const
{
	GuildSkillDataMap::const_iterator mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		return mit->second.m_iSkillLevel;
	}

	return 0;
}

bool KGuildSkillTree::SetGuildCSPointEndDate( IN const std::wstring& wstrEndDate )
{
	if( wstrEndDate.empty() )
		return false;

	if( KncUtil::ConvertStringToCTime( wstrEndDate, m_tGuildCSPointEndDate ) == false )
		return false;

	m_wstrGuildCSPointEndDate = wstrEndDate;
	return true;
}

bool KGuildSkillTree::SetSkillLevelAndCSP( int iSkillID, int iSkillLevel, int iSkillCSPoint )
{
	SET_ERROR( NET_OK );

	int iCheckSkillLevel = iSkillLevel;
	if( 0 == iSkillLevel )
	{
		iCheckSkillLevel = 1;
	}
	else if( iSkillLevel < 0 )
	{
		SET_ERROR( ERR_SKILL_20 );

		return false;
	}

	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( iSkillID, iCheckSkillLevel );

	if( NULL == pSkillTemplet )
	{
		SET_ERROR( ERR_SKILL_19 );
		return false;
	}

	if( iSkillLevel < iSkillCSPoint )
	{
		SET_ERROR( ERR_SKILL_20 );
		return false;
	}

	if( iSkillLevel > SiCXSLSkillTree()->GetMaxGuildSkillLevel( iSkillID ) )
	{
		SET_ERROR( ERR_SKILL_21 );
		return false;
	}

	GuildSkillDataMap::iterator mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		GuildSkill& kGuildSkillData = mit->second;
		kGuildSkillData.m_iSkillLevel	= iSkillLevel;
		kGuildSkillData.m_iSkillCSPoint	= iSkillCSPoint;
	}
	else 
	{
		m_mapSkillTree[ iSkillID ] = GuildSkill( iSkillLevel, iSkillCSPoint );
	}

	return true;
}

bool KGuildSkillTree::IsMaxSkillLevel( IN int iSkillID )
{
	const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( iSkillID, 1 );
	if( NULL == pSkillTemplet )
	{
		START_LOG( cerr, L"길드 스킬이 최고 레벨인지 확인하려는데 스킬 템플릿이 NULL" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}

	GuildSkillDataMap::const_iterator mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		const GuildSkill& kGuildSkillData = mit->second;
		int iMaxSkillLevel = SiCXSLSkillTree()->GetMaxGuildSkillLevel( iSkillID );
		if( kGuildSkillData.m_iSkillLevel > iMaxSkillLevel )
		{
			START_LOG( cerr, L"길드 스킬이 최고 레벨인지 확인하려는데 스킬 레벨이 최고레벨보다 높다" )
				<< BUILD_LOG( iSkillID )
				<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
				<< BUILD_LOG( iMaxSkillLevel )
				<< END_LOG;

			return true;
		}
		else if( kGuildSkillData.m_iSkillLevel == iMaxSkillLevel )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	return false;
}

bool KGuildSkillTree::IsAllPrecedingSkillMaxLevel( int iSkillID )
{
	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( iSkillID );
	if( NULL == pSkillTreeTemplet )
	{
		START_LOG( cerr, L"스킬트리 템플릿이 없음_preceding" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}


	for( u_int ui = 0; ui < pSkillTreeTemplet->m_vecPrecedingSkill.size(); ++ui )
	{
		int iPrecedingSkillID = pSkillTreeTemplet->m_vecPrecedingSkill[ui];
		if( IsMaxSkillLevel( iPrecedingSkillID ) == false )
		{
			return false;
		}
	}

	return true;
}

bool KGuildSkillTree::IsAllFollowingSkillLevelZero( int iSkillID )
{
	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( iSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		START_LOG( cerr, L"스킬트리 템플릿이 없음_following" )			
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}

	for( u_int ui = 0; ui < pSkillTreeTemplet->m_vecFollowingSkill.size(); ++ui )
	{
		if( GetSkillLevel( pSkillTreeTemplet->m_vecFollowingSkill[ui] ) > 0 )
			return false;
	}

	return true;
}

// 이 스킬을 배우기에 충분할 만큼 SP를 소모했는지
bool KGuildSkillTree::IsTierOpened( int iSkillID )
{
	const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( iSkillID );
	if( pSkillTreeTemplet == NULL )
	{
		START_LOG( cerr, L"스킬트리 템플릿이 없음_IsTierOpened" )
			<< BUILD_LOG( iSkillID )
			<< END_LOG;

		return false;
	}


	const int MAGIC_SKILL_POINT_PER_TIER = 5;
	int iUsedSPoint = 0;
	int iUsedCSPoint = 0;
	CalcUsedSPointAndCSPoint( iUsedSPoint, iUsedCSPoint );
	if( iUsedSPoint + iUsedCSPoint < pSkillTreeTemplet->m_iTier * MAGIC_SKILL_POINT_PER_TIER )
	{
		return false;
	}

	return true;
}

// tier별로 소모된 누적 SP+CSP를 계산해준다
void KGuildSkillTree::CalcCumulativeUsedSPointOnEachTier( OUT std::vector< int >& vecTierSPoint )
{	
	int iMaxTierIndex = 0;
	for( GuildSkillDataMap::const_iterator mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{		
		const GuildSkill& kGuildSkillData = mit->second;

		// 스킬 레벨값이 유효한지 검사		
		if( kGuildSkillData.m_iSkillLevel <= 0 )
			continue;

		const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( mit->first );
		if( pSkillTreeTemplet == NULL )
			continue;

		if( iMaxTierIndex < pSkillTreeTemplet->m_iTier )
			iMaxTierIndex = pSkillTreeTemplet->m_iTier;
	}

	vecTierSPoint.resize(0);
	for( int i = 0; i <= iMaxTierIndex; ++i )
	{
		vecTierSPoint.push_back( 0 );
	}

	for( GuildSkillDataMap::const_iterator mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const GuildSkill& kGuildSkillData = mit->second;

		// 스킬 레벨값이 유효한지 검사
		if( kGuildSkillData.m_iSkillLevel <= 0 )
			continue;
		
		const CXSLSkillTree::SkillTreeTemplet* pSkillTreeTemplet = SiCXSLSkillTree()->GetGuildSkillTreeTemplet( mit->first );
		if( pSkillTreeTemplet == NULL )
			continue;

		//{{ 2010. 8. 17	최육사	스킬 되돌리기 버그 수정
#ifdef SERV_RESET_SKILL_BUG_FIX
        vecTierSPoint[ pSkillTreeTemplet->m_iTier ] += ( kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint );
#else
		vecTierSPoint[ pSkillTreeTemplet->m_iTier ] += kGuildSkillData.m_iSkillLevel;
#endif SERV_RESET_SKILL_BUG_FIX
		//}}
	}

	int iCumulativeTierSPoint = 0;
	for( u_int ui = 0; ui < vecTierSPoint.size(); ++ui )
	{
		iCumulativeTierSPoint += vecTierSPoint[ui];
		vecTierSPoint[ui] = iCumulativeTierSPoint;
	}
}

bool KGuildSkillTree::IsCashSkillPointExpired()
{
	if( 0 == m_iMaxGuildCSPoint )
		return true;

	return false;
}

void KGuildSkillTree::CalcExpireCashSkillPoint( OUT int& iGuildSPoint, OUT int& iRetrievedSPoint, OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData )
{
	vecModifiedGuildSkillData.clear();
	iRetrievedSPoint = 0;		

	GuildSkillDataMap::const_iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{		
		const GuildSkill& kGuildSkillData = mit->second;

		if( kGuildSkillData.m_iSkillCSPoint > 0 )
		{
			iRetrievedSPoint += kGuildSkillData.m_iSkillCSPoint;
			const u_char ucNewSkillLevel = static_cast<u_char>( kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint );

			vecModifiedGuildSkillData.push_back( KGuildSkillData( static_cast<short>(mit->first), ucNewSkillLevel, 0 ) );
		}
	}

	// 스킬트리에 소모했던 CSP 와 남아있는 CSP 를 더해서 구매시의 CSP를 빼면 돌려줄 SP 수치가 계산된다
	iRetrievedSPoint += GetGuildCSPoint() - GetMaxGuildCSPoint();

	// 되돌릴 SP를 현재 SP와 합하여 DB에 업데이트!
	iGuildSPoint = GetGuildSPoint() + iRetrievedSPoint;
}

void KGuildSkillTree::ExpireCashSkillPoint( OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData )
{
	vecModifiedGuildSkillData.clear();

	// 실제로 cash skill point로 획득한 스킬을 되돌린다.
	GuildSkillDataMap::iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const int iSkillID = mit->first;
		GuildSkill& kGuildSkillData = mit->second;

		if( kGuildSkillData.m_iSkillCSPoint > 0 )
		{
			const u_char ucNewSkillLevel = static_cast<u_char>( kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint );

			kGuildSkillData.m_iSkillLevel = ucNewSkillLevel;
			kGuildSkillData.m_iSkillCSPoint = 0;
		}

		vecModifiedGuildSkillData.push_back( KGuildSkillData( static_cast<short>(mit->first), kGuildSkillData.m_iSkillLevel, 0 ) );
	}

	SetMaxGuildCSPoint( 0 );
	SetGuildCSPoint( 0 );
}

bool KGuildSkillTree::GetSkillLevelAndCSP( IN int iSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint )
{
	GuildSkillDataMap::const_iterator mit;
	mit = m_mapSkillTree.find( iSkillID );
	if( mit != m_mapSkillTree.end() )
	{
		const GuildSkill& kGuildSkillData = mit->second;
		iSkillLevel		= kGuildSkillData.m_iSkillLevel;
		iSkillCSPoint	= kGuildSkillData.m_iSkillCSPoint;
		return true;
	}
	else
	{
		iSkillLevel		= 0;
		iSkillCSPoint	= 0;
	}

	return false;
}

void KGuildSkillTree::CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint )
{
	iSPoint = 0;
	iCSPoint = 0;

	GuildSkillDataMap::const_iterator mit;

	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const int iSkillID = mit->first;
		const GuildSkill& kGuildSkillData = mit->second;

		if( kGuildSkillData.m_iSkillLevel > 0 )
		{
			if( kGuildSkillData.m_iSkillCSPoint > kGuildSkillData.m_iSkillLevel )
			{
				START_LOG( cerr, L"스킬 레벨 < 스킬 CSP" )
					<< BUILD_LOG( iSkillID )
					<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
					<< BUILD_LOG( kGuildSkillData.m_iSkillCSPoint )
					<< END_LOG;
			}

			iSPoint		+= kGuildSkillData.m_iSkillLevel - kGuildSkillData.m_iSkillCSPoint;
			iCSPoint	+= kGuildSkillData.m_iSkillCSPoint;
		}
		else if( kGuildSkillData.m_iSkillLevel == 0 )
		{
			if( kGuildSkillData.m_iSkillCSPoint > 0 )
			{
				START_LOG( cwarn, L"스킬 레벨이 0이하인데 SkillCSP가 0보다 큼!" )
					<< BUILD_LOG( iSkillID )
					<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
					<< BUILD_LOG( kGuildSkillData.m_iSkillCSPoint )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cwarn, L"스킬 레벨이 0보다 작다!" )
				<< BUILD_LOG( iSkillID )
				<< BUILD_LOG( kGuildSkillData.m_iSkillLevel )
				<< BUILD_LOG( kGuildSkillData.m_iSkillCSPoint )
				<< END_LOG;
		}
	}
}
#endif GUILD_SKILL_TEST
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013





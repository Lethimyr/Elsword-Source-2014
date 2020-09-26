#include "UserDungeonManager.h"
//{{ 2012. 04. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLDungeonManager.h"
	#include "GSAutoPartyManager.h"
	#include "X2Data/XSLMapData.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_PC_BANG_TYPE
	#include "GameSysVal/GameSysVal.h"
#endif SERV_PC_BANG_TYPE

//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST


KUserDungeonManager::KUserDungeonManager(void)
{
	Clear();
}

KUserDungeonManager::~KUserDungeonManager(void)
{
}

void KUserDungeonManager::Clear()
{
	m_iHenirRewardCount				= 0;				// 헤니르 보상 가능 횟수 (일반)
	m_iHenirRewardPremiumCount		= 0;				// 헤니르 보상 가능 횟수 (PC방)
	m_iHenirRewardEventCount		= 0;				// 헤니르 보상 가능 횟수 (이벤트) 횟수
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	m_iHenirChallengeRewardCount		= 0;				// 헤니르 도전 모드 보상 가능 횟수 (일반)
	m_iHenirChallengeRewardPremiumCount	= 0;				// 헤니르 도전 모드 보상 가능 횟수 (PC방)
	m_iHenirChallengeRewardEventCount	= 0;				// 헤니르 도전 모드 보상 가능 횟수 (이벤트) 횟수
#endif // SERV_HENIR_RENEWAL_2013

	m_iHenirRewardEventLimitCount	= 0;				// 헤니르 보상 가능 횟수 (이벤트) 제한
	m_bUnLimitedHenirRewardEvnet	= false;			// 헤니르 보상 가능 횟수 (이벤트) 무제한

	m_bIsPcBang						= false;
#ifdef SERV_PC_BANG_TYPE
	m_iPcBangType					= -1;
#endif SERV_PC_BANG_TYPE
	//{{ 2012. 02. 08	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_iAutoPartyWaitNumber			= 0;
	m_iAutoPartyUID					= 0;
	m_kDungeonGameInfo.Clear();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
}

void KUserDungeonManager::Init( IN const bool bUnlimitedEvent			// 무제한 인가?
							  , IN const int iLimitEvent				// Event 추가 횟수 MAX
							  , IN const int iNormalCnt					// 보상 받은 횟수 일반
							  , IN const int iPremiumCnt				// 보상 받은 횟수 pc방
							  , IN const int iEventCnt					// 보상 받은 횟수 이벤트
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
							  , IN const int iChallengeNormalCnt
							  , IN const int iChallengePremiumCnt
							  , IN const int iChallengeEventCnt
#endif // SERV_HENIR_RENEWAL_2013
							  , IN const bool bIsPcBang					// PC방인지 정보 넣기
#ifdef SERV_PC_BANG_TYPE
								, IN const int iPcBangType				// PC방 타입
#endif SERV_PC_BANG_TYPE							  
							  )
{
	if( ( iLimitEvent < 0 )
		|| ( iNormalCnt < 0 )
		|| ( iPremiumCnt < 0 )
		|| ( iEventCnt < 0 )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
		|| ( iChallengeNormalCnt < 0 )
		|| ( iChallengePremiumCnt < 0 )
		|| ( iChallengeEventCnt < 0 )
#endif // SERV_HENIR_RENEWAL_2013
		)
	{
		START_LOG( cerr, L"헤니르 보상 획득 - 0 보다 작은 값으로 초기화 한다?!")
			<< BUILD_LOG( bUnlimitedEvent )
			<< BUILD_LOG( iLimitEvent )
			<< BUILD_LOG( iNormalCnt )
			<< BUILD_LOG( iPremiumCnt )
			<< BUILD_LOG( iEventCnt )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
			<< BUILD_LOG( iChallengeNormalCnt )
			<< BUILD_LOG( iChallengePremiumCnt )
			<< BUILD_LOG( iChallengeEventCnt )
#endif // SERV_HENIR_RENEWAL_2013
			<< END_LOG;
	}

	m_iHenirRewardCount				= iNormalCnt;
	m_iHenirRewardPremiumCount		= iPremiumCnt;
	m_iHenirRewardEventCount		= iEventCnt;
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	m_iHenirChallengeRewardCount		= iChallengeNormalCnt;
	m_iHenirChallengeRewardPremiumCount	= iChallengePremiumCnt;
	m_iHenirChallengeRewardEventCount	= iChallengeEventCnt;
#endif // SERV_HENIR_RENEWAL_2013

	m_iHenirRewardEventLimitCount	= iLimitEvent;
	m_bUnLimitedHenirRewardEvnet	= bUnlimitedEvent;

	m_bIsPcBang						= bIsPcBang;
#ifdef SERV_PC_BANG_TYPE
	m_iPcBangType					= iPcBangType;
#endif SERV_PC_BANG_TYPE
}

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
bool KUserDungeonManager::IncreaseHenirRewardCount( IN const char cDungeonMode )
{
	// 보상 획득 감소 순서
	// PC방 > Event > Normal

	int	iHENIR_REWARD_LIMIT_COUNT	= 0;
	int* pHenirRewardCount			= NULL;		// 헤니르 보상 사용한 횟수 (일반)
	int* pHenirRewardPremiumCount	= NULL;		// 헤니르 보상 사용한 횟수 (PC방)
	int* pHenirRewardEventCount		= NULL;		// 헤니르 보상 사용한 횟수 (이벤트) 횟수

	switch( cDungeonMode )
	{
	case CXSLDungeon::DM_HENIR_PRACTICE:
		iHENIR_REWARD_LIMIT_COUNT	= HRLC_NORMAL;
		pHenirRewardCount			= &m_iHenirRewardCount;
		pHenirRewardPremiumCount	= &m_iHenirRewardPremiumCount;
		pHenirRewardEventCount		= &m_iHenirRewardEventCount;
		break;

	case CXSLDungeon::DM_HENIR_CHALLENGE:
		iHENIR_REWARD_LIMIT_COUNT	= HRLC_NORMAL_CHALLENGE;
		pHenirRewardCount			= &m_iHenirChallengeRewardCount;
		pHenirRewardPremiumCount	= &m_iHenirChallengeRewardPremiumCount;
		pHenirRewardEventCount		= &m_iHenirChallengeRewardEventCount;
		break;

	default:
		START_LOG( cerr, L"잘못된 DungeonMode 정보입니다." )
			<< BUILD_LOGc( cDungeonMode )
			<< END_LOG;
		return false;
	}

	if( m_bUnLimitedHenirRewardEvnet == true )		// 헤니르 무제한 이벤트 중이면
	{
		return true;
	}

	// 1. PC 방
	if( IsPremiumUser()== true )
	{
#ifdef SERV_PC_BANG_TYPE
		int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
		if( *pHenirRewardPremiumCount < iAdditionalHenirRewardCount )
#else SERV_PC_BANG_TYPE
		if( *pHenirRewardPremiumCount < HRLC_PREMIUM )
#endif SERV_PC_BANG_TYPE
		{
			*pHenirRewardPremiumCount += 1;
			return true;
		}
	}

	// 2. Event
	if( *pHenirRewardEventCount < m_iHenirRewardEventLimitCount )
	{
		*pHenirRewardEventCount += 1;
		return true;
	}

	// 3. Normal
	if( *pHenirRewardCount < iHENIR_REWARD_LIMIT_COUNT )
	{
		*pHenirRewardCount += 1;
		return true;
	}

	return false;
}
#else
bool KUserDungeonManager::IncreaseHenirRewardCount()
{
	if( m_bUnLimitedHenirRewardEvnet == true )		// 헤니르 무제한 이벤트 중이면
	{
		return true;
	}

	// 보상 획득 감소 순서
	// PC방 > Event > Normal

	// 1. PC 방
	if( IsPremiumUser()== true )
	{
#ifdef SERV_PC_BANG_TYPE
		int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
		if( m_iHenirRewardPremiumCount < iAdditionalHenirRewardCount )
#else SERV_PC_BANG_TYPE
		if( m_iHenirRewardPremiumCount < HRLC_PREMIUM )
#endif SERV_PC_BANG_TYPE
		{
			m_iHenirRewardPremiumCount += 1;

			return true;
		}
	}
	
	// 2. Event
	if( m_iHenirRewardEventCount < m_iHenirRewardEventLimitCount )
	{
		m_iHenirRewardEventCount += 1;

		return true;
	}

	// 3. Normal
	if( m_iHenirRewardCount < HRLC_NORMAL )
	{
		m_iHenirRewardCount += 1;

		return true;
	}

	return false;
}
#endif // SERV_HENIR_RENEWAL_2013

void KUserDungeonManager::GetHenirRewardCountInfo( IN KEGS_HENIR_REWARD_COUNT_NOT& kNot )
{
	kNot.m_bUnLimited	= m_bUnLimitedHenirRewardEvnet;
	kNot.m_iEvent		= GetPossibleHenirRewardEventCount();
	kNot.m_iPremium		= GetPossibleHenirRewardPremiumCount();
	kNot.m_iNormal		= GetPossibleHenirRewardNormalCount();
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	kNot.m_iChallengeNormal		= GetPossibleHenirChallengeRewardNormalCount();
	kNot.m_iChallengePremium	= GetPossibleHenirChallengeRewardPremiumCount();
	kNot.m_iChallengeEvent		= GetPossibleHenirChallengeRewardEventCount();
#endif // SERV_HENIR_RENEWAL_2013
	kNot.m_iEventMAX	= m_iHenirRewardEventLimitCount;

#ifdef SERV_PC_BANG_TYPE
	int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
	kNot.m_iPremiumMAX	= ( IsPremiumUser() == true )  ? iAdditionalHenirRewardCount : 0;
#else SERV_PC_BANG_TYPE
	kNot.m_iPremiumMAX	= ( IsPremiumUser() == true )  ? HRLC_PREMIUM : 0;
#endif SERV_PC_BANG_TYPE
}

int KUserDungeonManager::GetPossibleHenirRewardNormalCount()
{ 
	if( (HRLC_NORMAL - m_iHenirRewardCount) < 0 )
	{
		return 0;
	}

	return HRLC_NORMAL - m_iHenirRewardCount; 
}

int KUserDungeonManager::GetPossibleHenirRewardEventCount()
{
	if( (m_iHenirRewardEventLimitCount - m_iHenirRewardEventCount) < 0 )
	{
		return 0;
	}

	return m_iHenirRewardEventLimitCount - m_iHenirRewardEventCount; 
}

int KUserDungeonManager::GetPossibleHenirRewardPremiumCount()
{
	if( IsPremiumUser() == true )
	{
#ifdef SERV_PC_BANG_TYPE
		int iAdditionalHenirRewardCount = SiKGameSysVal()->GetAdditionalHenirRewardCount( GetPcBangType() );
		if( (iAdditionalHenirRewardCount - m_iHenirRewardPremiumCount) < 0 )
		{
			return 0;
		}

		return iAdditionalHenirRewardCount - m_iHenirRewardPremiumCount;
#else SERV_PC_BANG_TYPE
		if( (HRLC_PREMIUM - m_iHenirRewardPremiumCount) < 0 )
		{
			return 0;
		}

		return HRLC_PREMIUM - m_iHenirRewardPremiumCount;
#endif SERV_PC_BANG_TYPE
	}
	
	return 0; 
}

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
int KUserDungeonManager::GetPossibleHenirChallengeRewardNormalCount( void ) const
{
	const int iResult = HRLC_NORMAL_CHALLENGE - m_iHenirChallengeRewardCount;

	if( iResult < 0 )
	{
		return 0;
	}

	return iResult; 
}

int KUserDungeonManager::GetPossibleHenirChallengeRewardEventCount( void ) const
{
	const int iResult = m_iHenirRewardEventLimitCount - m_iHenirChallengeRewardEventCount;

	if( iResult < 0 )
	{
		return 0;
	}

	return iResult; 
}

int KUserDungeonManager::GetPossibleHenirChallengeRewardPremiumCount( void ) const
{
	if( IsPremiumUser() == true )
	{
		const int iResult = HRLC_PREMIUM - m_iHenirChallengeRewardPremiumCount;
		if( iResult < 0 )
		{
			return 0;
		}

		return iResult;
	}

	return 0;
}
#endif // SERV_HENIR_RENEWAL_2013

#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2012. 04. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KUserDungeonManager::SetDungeonGameInfo( IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode )
{
    m_kDungeonGameInfo.m_iDungeonID = iDungeonID;
	m_kDungeonGameInfo.m_cDifficulty = cDifficulty;
	m_kDungeonGameInfo.m_cDungeonMode = cDungeonMode;
}

bool KUserDungeonManager::IsPlayingNowHenirChallengeMode() const
{
	if( m_kDungeonGameInfo.m_iDungeonID == 0 )
		return false;

	const CXSLDungeon::DUNGEON_TYPE eDungeonType = SiCXSLDungeonManager()->GetDungeonType( m_kDungeonGameInfo.GetDungeonIDWithDif() );
	if( eDungeonType != CXSLDungeon::DT_HENIR )
	{
		// 헤니르 시공이 아니면 false
		return false;
	}

	//CXSLDungeon::HENIR_DUNGEON_MODE eHenirDungeonMode = CXSLDungeon::HDM_INVALID;
	//if( !SiKPartyManager()->GetHenirDungeonMode( GetPartyUID(), eHenirDungeonMode ) )
	//{
	//	START_LOG( cerr, L"던전 모드를 얻을 수 없습니다! 있을수 없는 에러!" )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( GetPartyUID() )
	//		<< END_LOG;

	//	return false;
	//}

	// 헤니르 시공 도전모드 여부
	return ( (CXSLDungeon::DUNGEON_MODE)m_kDungeonGameInfo.m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE );
}

void KUserDungeonManager::GetAutoPartyBonusDungeonList( IN const int iVillageMapID, OUT std::map< int, KAutoPartyDungeonBonusInfo >& mapAutoPartyBonusList )
{
	const CXSLMapData::SLocalMapInfo* pLocalMapIDList = SiCXSLMapData()->GetLocalMapList( iVillageMapID );
	if( IS_NULL( pLocalMapIDList ) )
	{
		START_LOG( cerr, L"로컬맵 리스트 정보를 얻을수 없습니다!" )
			<< BUILD_LOG( iVillageMapID )
			<< END_LOG;
		return;
	}

	BOOST_TEST_FOREACH( const KDungeonGameSimpleInfo&, kInfo, pLocalMapIDList->m_vecDungeonInfo )
	{
		KAutoPartyDungeonBonusInfo kAutoPartyBonusInfo;
		kAutoPartyBonusInfo.m_cDungeonMode = kInfo.m_cDungeonMode;
		kAutoPartyBonusInfo.m_iDungeonID = kInfo.m_iDungeonID;
		kAutoPartyBonusInfo.m_bActivatedBonus = SiKGSAutoPartyManager()->IsAutoPartyBonus( kInfo.m_cDungeonMode, kInfo.m_iDungeonID );
		mapAutoPartyBonusList.insert( std::make_pair( kInfo.m_iDungeonID, kAutoPartyBonusInfo ) );
	}
}

void KUserDungeonManager::GetAutoPartyBonusDungeonList( IN const std::vector< KDungeonGameSimpleInfo >& vecDungeonList, OUT std::map< int, KAutoPartyDungeonBonusInfo >& mapAutoPartyBonusList )
{
	BOOST_TEST_FOREACH( const KDungeonGameSimpleInfo&, kInfo, vecDungeonList )
	{
		KAutoPartyDungeonBonusInfo kAutoPartyBonusInfo;
		kAutoPartyBonusInfo.m_cDungeonMode = kInfo.m_cDungeonMode;
		kAutoPartyBonusInfo.m_iDungeonID = kInfo.m_iDungeonID;
		kAutoPartyBonusInfo.m_bActivatedBonus = SiKGSAutoPartyManager()->IsAutoPartyBonus( kInfo.m_cDungeonMode, kInfo.m_iDungeonID );
		mapAutoPartyBonusList.insert( std::make_pair( kInfo.m_iDungeonID, kAutoPartyBonusInfo ) );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}



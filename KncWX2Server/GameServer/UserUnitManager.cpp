#include "GSUser.h"
#include ".\userunitmanager.h"



//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KUserUnitManager::KUserUnitManager(void)
{
	Clear();
}

KUserUnitManager::~KUserUnitManager(void)
{
}

void KUserUnitManager::Clear()
{
	m_kGamePlayStatusContainer.Clear();
	m_vecReservedReturnToFieldInfo.clear();

	//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
	m_bEnterCashShop = false;
#endif SERV_VISIT_CASH_SHOP
	//}}
}

void KUserUnitManager::Init( IN OUT KGamePlayStatusContainer& kNewGamePlayStatus, IN const KRoomUserInfo& kRoomUserInfo )
{
	// DB에서 가져온 Status정보가 비어있을경우 [캐릭터 최초 생성]
	KGamePlayStatus kGamePlayStatus;
	if( kNewGamePlayStatus.Get( kGamePlayStatus ) == false )
	{
		kGamePlayStatus.m_iMaxMP = 300;
		kGamePlayStatus.m_iCurMP = 300;

		// 최대 HP정보는 배틀필드 기준으로 계산된 정보를 넣어주자!
		kGamePlayStatus.m_iMaxHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
		kGamePlayStatus.m_iCurHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
	}
	// DB에서 가져온 Status정보가 비어있지 않을 경우
	else
	{
#ifdef SERV_CREATE_UNIT_INIT_STAT	// 적용날짜: 2013-07-11
		if( kGamePlayStatus.m_iMaxHP <= 0 )		// maxhp 가 0 이하는 말이 안된다.
#else
		if( kGamePlayStatus.m_iCurHP == -1 )
#endif	// SERV_CREATE_UNIT_INIT_STAT
		{
			kGamePlayStatus.m_iMaxMP = 300;
			kGamePlayStatus.m_iCurMP = 300;

			// 최대 HP정보는 배틀필드 기준으로 계산된 정보를 넣어주자!
			kGamePlayStatus.m_iMaxHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
			kGamePlayStatus.m_iCurHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
		}
		else
		{
			// 최대 HP정보는 배틀필드 기준으로 계산된 정보를 넣어주자!
			kGamePlayStatus.m_iMaxHP = kRoomUserInfo.m_kGameStat.m_iBaseHP;
		}
	}    

	// DB에서 가져온 플레이 상태값을 저장한다.
	m_kGamePlayStatusContainer.Set( kGamePlayStatus );

	// 클라에도 보낼 정보에도 적용하자.
	kNewGamePlayStatus.Set( kGamePlayStatus );

	{
		// HP & MP도 저장하자!
		m_kPartyMemberStatus.m_fHpPercent = static_cast<float>(kGamePlayStatus.m_iCurHP) / static_cast<float>(kGamePlayStatus.m_iMaxHP) * 100.f;
		m_kPartyMemberStatus.m_fMpPercent = static_cast<float>(kGamePlayStatus.m_iCurMP) / static_cast<float>(kGamePlayStatus.m_iMaxMP) * 100.f;
	}
}

void KUserUnitManager::SetGamePlayStatusAndCheckChangedHpMp( IN const KGamePlayStatus& kNewGamePlayStatus, OUT bool& bChangedHpMp, OUT KPartyMemberStatus& kHpMpInfo )
{
	bChangedHpMp = false;

	//////////////////////////////////////////////////////////////////////////	
	// 기존 정보가 없다!
	KGamePlayStatus kOldStatusInfo;
    if( m_kGamePlayStatusContainer.Get( kOldStatusInfo ) == false )
	{        
		// Hp Mp정보가 변경되었다고 전달!
		bChangedHpMp = true;
	}
	else
	{
		// Hp Mp가 변경되었는지 확인
        if( kOldStatusInfo.m_iMaxHP != kNewGamePlayStatus.m_iMaxHP  ||
			kOldStatusInfo.m_iCurHP != kNewGamePlayStatus.m_iCurHP  ||
			kOldStatusInfo.m_iMaxMP != kNewGamePlayStatus.m_iMaxMP  ||
			kOldStatusInfo.m_iCurMP != kNewGamePlayStatus.m_iCurMP )
		{
			// Hp Mp정보가 변경되었다!
			bChangedHpMp = true;
		}
	}

	// 저장하자!
	m_kGamePlayStatusContainer.Set( kNewGamePlayStatus );

	//////////////////////////////////////////////////////////////////////////	
	// Hp Mp 퍼센티지를 구하자!
	if( bChangedHpMp )
	{
		kHpMpInfo.m_fHpPercent = static_cast<float>(kNewGamePlayStatus.m_iCurHP) / static_cast<float>(kNewGamePlayStatus.m_iMaxHP) * 100.f;
		kHpMpInfo.m_fMpPercent = static_cast<float>(kNewGamePlayStatus.m_iCurMP) / static_cast<float>(kNewGamePlayStatus.m_iMaxMP) * 100.f;

		// 저장하자!
		m_kPartyMemberStatus = kHpMpInfo;
	}
}

void KUserUnitManager::ReservedReturnToFieldInfo( IN const KReturnToFieldInfo& kInfo )
{
    if( m_vecReservedReturnToFieldInfo.empty() == false )
	{
		START_LOG( cerr, L"필드 복귀 정보를 예약하려는데 이미 예약되어있다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( m_vecReservedReturnToFieldInfo.size() )
			<< END_LOG;
	}

	// 일단 예약정보 초기화!
	m_vecReservedReturnToFieldInfo.clear();

	// 그리고 예약!
	m_vecReservedReturnToFieldInfo.push_back( kInfo );
}

bool KUserUnitManager::GetAndDeleteResevedReturnToFieldInfo( OUT KReturnToFieldInfo& kInfo )
{
	if( m_vecReservedReturnToFieldInfo.empty() )
		return false;

    BOOST_TEST_FOREACH( const KReturnToFieldInfo&, kReservedInfo, m_vecReservedReturnToFieldInfo )
	{
		kInfo = kReservedInfo;
		break;
	}

	m_vecReservedReturnToFieldInfo.clear();
	return true;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}




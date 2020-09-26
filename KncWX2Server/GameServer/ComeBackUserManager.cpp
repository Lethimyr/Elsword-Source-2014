
#include "ComeBackUserManager.h"
#include "GameServer.h"
#include "RewardTable.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
KComeBackUserManager::KComeBackUserManager(void)
{
}

KComeBackUserManager::~KComeBackUserManager(void)
{
}

void KComeBackUserManager::Clear()
{
	SetIsComeBackFirst( false );
	SetIsComeBackUser( false );
	SetComeBackRewardLevel( 0 );
	SetIsUnitSelect( false );
}

void KComeBackUserManager::SetComeBackBuffEndTime( IN std::wstring& wstrComeBackBuffEndDate )
{
	if( KncUtil::ConvertStringToCTime( wstrComeBackBuffEndDate, m_tComeBackBuffEnd ) == false )
	{
		m_tComeBackBuffEnd = CTime( 0 );
		START_LOG( cerr, L"시간 변환 실패!!" )
			<< BUILD_LOG( wstrComeBackBuffEndDate )
			<< END_LOG;
	}
}

bool KComeBackUserManager::GiveComeBackUserRewardCube( OUT int& iRewardItem, OUT CTime& tBuffEnd, OUT int& iRewardPeriod )
{
	CTime tCurr = CTime::GetCurrentTime();
	iRewardItem = 0;
	iRewardPeriod = 0;

	switch( GetComeBackRewardLevel() )
	{
	case KGameSysVal::CBS_ONE:
		{
			CTimeSpan tSpan = CTimeSpan( KGameSysVal::CBSD_ONE , 0, 0, 0 );
			tCurr += tSpan;
			iRewardPeriod = KGameSysVal::CBSD_ONE;

			iRewardItem = KRewardTable::ERI_COME_BACK_USER_ITEM_CUBE_ONE;
		}break;
	case KGameSysVal::CBS_TWO:
		{
			CTimeSpan tSpan = CTimeSpan( KGameSysVal::CBSD_TWO, 0, 0, 0 );
			tCurr += tSpan;
			iRewardPeriod = KGameSysVal::CBSD_TWO;

			iRewardItem = KRewardTable::ERI_COME_BACK_USER_ITEM_CUBE_TWO;
		}break;
	case KGameSysVal::CBS_THREE:
		{
			CTimeSpan tSpan = CTimeSpan( KGameSysVal::CBSD_THREE, 0, 0, 0 );
			tCurr += tSpan;
			iRewardPeriod = KGameSysVal::CBSD_THREE;

			iRewardItem = KRewardTable::ERI_COME_BACK_USER_ITEM_CUBE_THREE;
		}break;
	default:
		{
			START_LOG( cerr, L"복귀유저 보상 등급이 이상합니다." )
				<< BUILD_LOG( iRewardItem )
				<< BUILD_LOG( GetComeBackRewardLevel() )
				<< END_LOG;
			return false ;
		}
	}
	std::wstring wsrtBuffEnd = tBuffEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ));
	SetComeBackBuffEndTime( wsrtBuffEnd );
	tBuffEnd = tCurr;
	return true;
}

bool KComeBackUserManager::CheckComeBackBuffEnd()
{
	CTime tCurr = CTime::GetCurrentTime();

	if( GetIsComeBackUser() == true && m_tComeBackBuffEnd < tCurr )	// 휴면 유저이고  복귀 보상 버프가 종료 되었다
	{
		// 휴면 유저 버프 종료 저장
		SetIsComeBackUser( false );

		return true;
	}

	return false;
}

#endif SERV_COME_BACK_USER_REWARD
//}} 
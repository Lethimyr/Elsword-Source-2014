#include "CnPartyFeverManager.h"
#include "X2Data/XSLPartyManager.h"
#include "X2Data/XSLBattleFieldManager.h"


KCnPartyFeverManager::KCnPartyFeverManager()
{
	Reset();
}

KCnPartyFeverManager::~KCnPartyFeverManager()
{
}

void KCnPartyFeverManager::Reset()
{
	m_sFeverPoint = 0;
}

short KCnPartyFeverManager::GetFeverPoint( IN const int iNumMember ) const
{
	return SiCXSLPartyManager()->GetMaxFeverPointByNumMember( GetRealFeverPoint(), iNumMember );
}

KCnPartyFeverManager::UPDATE_FEVER_POINT KCnPartyFeverManager::UpdateFeverPoint( IN const int iFeverPoint, IN const int iNumMember, OUT int& iResultFeverPoint )
{
	iResultFeverPoint = 0;

	// 파티 멤버 수 구하기
	if( iNumMember == 1 )
	{	
		// 파티 멤버가 한명이면 아무런 동작을 하지 않는다.
		return UFP_NONE;
	}

	SetRealFeverPoint( iFeverPoint );

	if( GetRealFeverPoint() < 0 )
		SetRealFeverPoint( 0 );

	// fever 발동 조건을 만족하는지 체크
	if( GetRealFeverPoint() >= SiCXSLPartyManager()->GetActivateFeverPoint() )
	{
		// 4800을 만족하면 다시 4000으로 초기화 된다.
		SetRealFeverPoint( SiCXSLPartyManager()->GetMaxFeverPointByNumMember( GetRealFeverPoint(), iNumMember ) );

		// fever 발동!
		iResultFeverPoint = GetRealFeverPoint();
		return UFP_ACTIVE_FEVER;
	}
	else
	{
		// 현재 fever수치 보내기
		if( GetRealFeverPoint() < FEVER_100_PERCENT_POINT )
		{
			iResultFeverPoint = GetRealFeverPoint();
		}
		else
		{
			// 4000이상이더라도 클라이언트에서는 UI상으로 4000까지만 출력해야함.
			iResultFeverPoint = FEVER_100_PERCENT_POINT;
		}
		return UFP_CHANGE_FEVER;
	}
}

int KCnPartyFeverManager::CalcIncreaseFeverPoint( IN const int iNumMember, IN const char cRoomType, IN const short sSubStageNpcCount, IN const bool bComeBackUserInParty )
{
	if( sSubStageNpcCount == 0 )
	{
		// 몬스터가 하나도 없는 곳에서는 fever수치가 증가하지 않는다.
		return 0;
	}

	int iIncreasePartyFever = SiCXSLPartyManager()->GetIncreasePartyFever( iNumMember );

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	if( bComeBackUserInParty == true )
	{
		iIncreasePartyFever += static_cast<int>(iIncreasePartyFever * 0.5f); // 휴면 유저 보상
	}
#endif SERV_COME_BACK_USER_REWARD
	//}}

	// 배틀필드에서 잡은 몬스터라면 배틀필드 상수값을 곱하자!
	if( cRoomType == CXSLRoom::RT_BATTLE_FIELD )
	{
		//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
		float fMultipliedFeverByBattleFieldFactor = static_cast<float>(iIncreasePartyFever) * SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor();
#else
		float fMultipliedFeverByBattleFieldFactor = static_cast<float>(iIncreasePartyFever) * SiCXSLBattleFieldManager()->GetBattleFieldFactor();
#endif SERV_FIELD_ED_EXP_FACTOR
		//}
		if( fMultipliedFeverByBattleFieldFactor > 0  &&  fMultipliedFeverByBattleFieldFactor < 1 )
		{
			fMultipliedFeverByBattleFieldFactor = 1.f;
		}

		iIncreasePartyFever = static_cast<int>( fMultipliedFeverByBattleFieldFactor );
	}

	// 파티 피버 증가
	const int iIncreaseResult = iIncreasePartyFever / sSubStageNpcCount;
	return iIncreaseResult;
}



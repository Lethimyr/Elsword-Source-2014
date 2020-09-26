#include "UserRidingPet.h"

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20

#include "X2Data/XSLRidingPetManager.h"

KUserRidingPet::KUserRidingPet( IN KRidingPetInfo kInfo, IN const bool bCreated )
{
	CTime tCurrentDate = CTime::GetCurrentTime();

	const CXSLRidingPetManager::RidingPetTemplet* pRidingPetTemplet = SiCXSLRidingPetManager()->GetRidingPetTemplet( kInfo.m_usRindingPetID );
	if( pRidingPetTemplet == NULL )
	{
		kInfo.m_fStamina = 0.0f;
	}
	else
	{
		//										TimeGap								/			1차는데 필요한 시간						*  탈 것별 회복 상수
		kInfo.m_fStamina += ( tCurrentDate.GetTime() - kInfo.m_iLastUnSummonDate ) / ( SiCXSLRidingPetManager()->GetRecoveryTimePerOne() * pRidingPetTemplet->m_fStaminaRecoveryRate );
		
		if( ( pRidingPetTemplet->m_usMaxStamina < kInfo.m_fStamina ) ||
			( kInfo.m_fStamina < 0.0f )
			)
		{
			kInfo.m_fStamina = pRidingPetTemplet->m_usMaxStamina;
		}
	}

	if( bCreated == true )
	{
		kInfo.m_iLastUnSummonDate = tCurrentDate.GetTime();
	}

	SetData( kInfo, bCreated );
}

KUserRidingPet::~KUserRidingPet(void)
{
}

bool KUserRidingPet::EnableCheck( void ) const
{
	return ( SiCXSLRidingPetManager()->GetEnableStamina() <= m_kInfo.m_fStamina );
}

bool KUserRidingPet::IsExpirationRidingPet( void ) const
{
	if( m_kInfo.m_wstrDestroyDate.empty() == true )
	{
		return false;
	}

	CTime tDestroyDate;
	if( KncUtil::ConvertStringToCTime( m_kInfo.m_wstrDestroyDate, tDestroyDate ) == false )
	{
		START_LOG( cerr, L"기간제 펫 만료 일자 오류" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kInfo.m_wstrDestroyDate )
			<< END_LOG;
		return false;
	}

	return tDestroyDate <= CTime::GetCurrentTime();
}

bool KUserRidingPet::CheckRidingPetStamina( IN const float fStamina )
{
	const CXSLRidingPetManager::RidingPetTemplet* pRidingPetTemplet = SiCXSLRidingPetManager()->GetRidingPetTemplet( m_kInfo.m_usRindingPetID );
	if( pRidingPetTemplet == NULL )
	{
		return false;
	}
	
	return ( fStamina <= pRidingPetTemplet->m_usMaxStamina );
}
#endif	// SERV_RIDING_PET_SYSTM

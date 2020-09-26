#include "GSFieldBossManager.h"

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-14	// 박세훈
#include <Windows.h>

ImplementSingleton( KGSFieldBossManager );

KGSFieldBossManager::KGSFieldBossManager( void )
	: m_tReservedTimeForInitRequest( 0 )
	, m_tCheckTerm( 0 )
	, m_tPortalAppearanceTime( 0 )
	, m_tRemainPortalTime( 0 )
	, m_tRemainCoolTime( 0 )
	, m_iPortalAppearanceMap( 0 )
	, m_iPortalDestination( 0 )
	, m_byteState( 0 )
{
}

KGSFieldBossManager::~KGSFieldBossManager( void )
{
}

bool KGSFieldBossManager::CheckTerm( IN const __time64_t tCurrentTime )
{
	// 1분마다 체크하자
	if( ( tCurrentTime - m_tCheckTerm ) <= 60 )
	{
		return false;
	}

	m_tCheckTerm = tCurrentTime;
	return true;
}

void KGSFieldBossManager::SetInfo( IN const __time64_t tPortalAppearanceTime
								 , IN const __time64_t tRemainPortalTime
								 , IN const __time64_t tRemainCoolTime
								 , IN const int iPortalAppearanceMap
								 , IN const int iPortalDestination
								 , IN const bool bPortalOpen
								 , IN const bool bCoolTime
								 )
{
	m_tPortalAppearanceTime	= tPortalAppearanceTime;
	m_tRemainPortalTime		= tRemainPortalTime;
	m_tRemainCoolTime		= tRemainCoolTime;
	m_iPortalAppearanceMap	= iPortalAppearanceMap;
	m_iPortalDestination	= iPortalDestination;
	( bPortalOpen ) ? ( m_byteState |= GFBS_PORTAL_OPEN ) : ( m_byteState &= ~GFBS_PORTAL_OPEN );
	SetCoolTimeState( bCoolTime );
}
#endif // SERV_BATTLE_FIELD_BOSS

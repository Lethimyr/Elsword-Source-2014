#pragma once

#include "ServerDefine.h"

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-14	// 박세훈
#include <Windows.h>
#include <KNCSingleton.h>
#include <time.h>

// 필드 보스 시스템은 대부분의 코드를 외부에 두도록 해보자 2013-10-31//	박세훈

class KGSFieldBossManager
{
	DeclareSingleton( KGSFieldBossManager );

	enum GS_FIELD_BOSS_STATE
	{
		GFBS_PORTAL_OPEN	= ( 1 << 0 ),
		GFBS_COOL_TIME		= ( 1 << 1 ),
		GFBS_CONNECTED		= ( 1 << 2 ),	// GlobalServer와 연결이 되어 있다면, 종료 정보도 GlobalServer로부터 받는다. 만약 연결이 끊어졌다면, 전달 받은 시간을 통해서 종료 체크를 한다
	};

public:
	void		SetReservedTimeForInitRequest( IN const __time64_t tReservedTimeForInitRequest )	{	m_tReservedTimeForInitRequest = tReservedTimeForInitRequest;	}
	__time64_t	GetReservedTimeForInitRequest( void ) const	{	return m_tReservedTimeForInitRequest;	}
	bool		CheckTerm( IN const __time64_t tCurrentTime );
	void		SetConnected( const bool bConnected )	{	( bConnected ) ? ( m_byteState |= GFBS_CONNECTED ) : ( m_byteState &= ~GFBS_CONNECTED );	}

	void		SetInfo( IN const __time64_t tPortalAppearanceTime
					   , IN const __time64_t tRemainPortalTime
					   , IN const __time64_t tRemainCoolTime
					   , IN const int iPortalAppearanceMap
					   , IN const int iPortalDestination
					   , IN const bool bPortalOpen
					   , IN const bool bCoolTime
					   );

	void		ClosePortal( void ){	m_byteState &= ~GFBS_PORTAL_OPEN;	}
	void		SetCoolTimeState( IN const bool bValue )	{	( bValue ) ? ( m_byteState |= GFBS_COOL_TIME ) : ( m_byteState &= ~GFBS_COOL_TIME );	}

	__time64_t	GetPortalAppearanceTime( void ) const	{	return m_tPortalAppearanceTime;	}
	int			GetPortalAppearanceMap( void ) const	{	return m_iPortalAppearanceMap;	}
	int			GetPortalDestination( void ) const	{	return m_iPortalDestination;	}
	__time64_t	GetRemainPortalTime( void ) const	{	return m_tRemainPortalTime;	}
	__time64_t	GetRemainCoolTime( void ) const	{	return m_tRemainCoolTime;	}

	bool		IsPortalOpen( void ) const	{	return ( m_byteState & GFBS_PORTAL_OPEN ) == GFBS_PORTAL_OPEN;	}
	bool		IsCoolTime( void ) const	{	return ( m_byteState & GFBS_COOL_TIME ) == GFBS_COOL_TIME;	}
	bool		IsConnected( void ) const	{	return ( m_byteState & GFBS_CONNECTED ) == GFBS_CONNECTED;	}

private:
	KGSFieldBossManager( void );
	~KGSFieldBossManager( void );

private:
	__time64_t	m_tReservedTimeForInitRequest;	// Global Server의 FieldBossManager 정보를 얻어오기 위해 사용
	__time64_t	m_tCheckTerm;

	__time64_t	m_tPortalAppearanceTime;		// Local Machine Time
	__time64_t	m_tRemainPortalTime;			// 포탈이 닫힐 때까지 남은 시간 ( 종료 시각으로 처리해도 된다 )
	__time64_t	m_tRemainCoolTime;				// 재시작까지 남은 시간 ( 종료 시각으로 처리해도 된다 )
	int			m_iPortalAppearanceMap;			// 포탈이 생성된 배틀 필드 ID
	int			m_iPortalDestination;			// 포탈이 연결된 보스 필드 ID
	byte		m_byteState;
};

DefSingletonInline( KGSFieldBossManager );
#endif // SERV_BATTLE_FIELD_BOSS

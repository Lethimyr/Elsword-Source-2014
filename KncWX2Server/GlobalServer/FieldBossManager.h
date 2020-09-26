#pragma once

#include "ServerDefine.h"

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-14	// 박세훈
#include <Windows.h>
#include <KNCSingleton.h>
#include <map>

// 필드 보스 시스템은 대부분의 코드를 외부에 두도록 해보자 2013-10-31//	박세훈

class KFieldBossManager
{
	DeclareSingleton( KFieldBossManager );

	enum FIELD_BOSS_STATE
	{
		FBS_PORTAL_OPEN	= ( 1 << 0 ),
		FBS_COOL_TIME	= ( 1 << 1 ),
	};

public:
	void	UpdateTotalDangerousValue( IN const int iPortalAppearanceMap, IN const byte byteValue );
	void	ClearTotalDangerousValue( void ){	m_mapTotalDangerousValue.clear();	}
	byte	GetTotalDangerousValue( IN const int iPortalAppearanceMap ) const;
	byte	GetTotalDangerousValue( void ) const;

	void		OpenPortal( IN const int iPortalAppearanceMap, IN const int iPortalDestination, IN const __time64_t tPortalAppearanceTime );
	void		ClosePortal( void ){	m_byteState &= ~FBS_PORTAL_OPEN;	}
	void		SetCoolTimeState( IN const bool bValue )	{	( bValue ) ? ( m_byteState |= FBS_COOL_TIME ) : ( m_byteState &= ~FBS_COOL_TIME );	}

	__time64_t	GetPortalAppearanceTime( void ) const	{	return m_tPortalAppearanceTime;	}
	int			GetPortalAppearanceMap( void ) const	{	return m_iPortalAppearanceMap;	}
	int			GetPortalDestination( void ) const	{	return m_iPortalDestination;	}
	
	bool		CheckTerm( IN const __time64_t tCurrentTime );
	bool		IsPortalOpen( void ) const	{	return ( m_byteState & FBS_PORTAL_OPEN ) == FBS_PORTAL_OPEN;	}
	bool		IsCoolTime( void ) const	{	return ( m_byteState & FBS_COOL_TIME ) == FBS_COOL_TIME;	}

private:
	KFieldBossManager( void );
	~KFieldBossManager( void );

	typedef	std::map<int, byte>	TYPE_MAP_TOTAL_DANGEROUS_VALUE_FIELD;	// iVillageMapID, byteValue
	TYPE_MAP_TOTAL_DANGEROUS_VALUE_FIELD	m_mapTotalDangerousValue;

	__time64_t	m_tCheckTerm;
	__time64_t	m_tPortalAppearanceTime;
	int			m_iPortalAppearanceMap;
	int			m_iPortalDestination;

	byte		m_byteState;
};

DefSingletonInline( KFieldBossManager );
#endif // SERV_BATTLE_FIELD_BOSS

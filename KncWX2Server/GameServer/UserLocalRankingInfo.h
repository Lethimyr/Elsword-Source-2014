#pragma once
#include "ServerDefine.h"

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
#include <WinSock2.h>	// windows.h redefinition 오류 대처용
#include <dbg/dbg.hpp>
#include "CommonPacket.h"
#include "..\Common\CacheData.h"
#include "..\Common\Enum\Enum.h"
#include "boost\timer.hpp"

class KUserLocalRankingInfo
{
public:
	KUserLocalRankingInfo();
	~KUserLocalRankingInfo(){}

	bool	InitCheck( IN const UidType iUserUID, IN const UidType iUnitUID ) const{	return ( m_kLocalRankingUserInfo.m_iUserUID == iUserUID ) && ( m_kLocalRankingUnitInfo.m_iUnitUID == iUnitUID );	}
	bool	InitCheckUserInfo( IN const UidType iUserUID ) const{	return ( m_kLocalRankingUserInfo.m_iUserUID == iUserUID );	}
	void	InitInfo( IN const int iUID, IN const KLocalRankingUserInfo& kUserInfo, IN const std::wstring& wstrUpdated, IN const KLocalRankingUnitInfo& kUnitInfo )
	{
		m_iUID					= iUID;
		m_kLocalRankingUserInfo	= kUserInfo;
		m_wstrUpdated			= wstrUpdated;
		m_kLocalRankingUnitInfo	= kUnitInfo;

		for( int i = 0; i < SEnum::LRMTC_MAX; ++i )
		{
			m_iPoint[i].SetValue( kUnitInfo.m_iPoint[i] );
		}
	}

	void	InitUserInfo( IN const KLocalRankingUserInfo& kInfo, IN const std::wstring& wstrUpdated )
	{
		m_kLocalRankingUserInfo = kInfo;
		m_wstrUpdated			= wstrUpdated;
	}

	void	ResetUnitInfo( void ){	m_kLocalRankingUnitInfo.clear();	}
	
	void	GetUserInfo( OUT KLocalRankingUserInfo& kInfo ) const{	kInfo = m_kLocalRankingUserInfo;	}
	void	GetUnitInfo( OUT KLocalRankingUnitInfo& kInfo ) const{	kInfo = m_kLocalRankingUnitInfo;	}
	void	GetFilteredUserInfo( OUT KLocalRankingUserInfo& kInfo ) const;

	void	GetLastUpdateDate( OUT std::wstring& wstrLastUpdateDate ){	wstrLastUpdateDate	= m_wstrUpdated;	}
	int		GetUID( void ) const{	return m_iUID;	}

	bool	CheckUpdate( void ) const;

	void	IncreasedUnitInfo( IN const int iIncreasedSpirit, IN const int iIncreasedAP )
	{
		m_iPoint[SEnum::LRMTC_SPIRIT]	+= iIncreasedSpirit;
		m_iPoint[SEnum::LRMTC_AP]		+= iIncreasedAP;

		for( int i = 0; i < SEnum::LRMTC_MAX; ++i )
		{
			m_kLocalRankingUnitInfo.m_iPoint[i] = m_iPoint[i];
		}
	}

	void	GetValue( OUT int& iChangedSpirit, OUT int& iChangedAP ) const
	{
		iChangedSpirit	= m_iPoint[SEnum::LRMTC_SPIRIT];
		iChangedAP		= m_iPoint[SEnum::LRMTC_AP];
	}

	void	GetChangedValue( OUT int& iChangedSpirit, OUT int& iChangedAP )
	{
		iChangedSpirit	= m_iPoint[SEnum::LRMTC_SPIRIT].GetChangeValue();
		iChangedAP		= m_iPoint[SEnum::LRMTC_AP].GetChangeValue();

		m_iPoint[SEnum::LRMTC_SPIRIT].SetValue( m_iPoint[SEnum::LRMTC_SPIRIT] );
		m_iPoint[SEnum::LRMTC_AP].SetValue( m_iPoint[SEnum::LRMTC_AP] );
	}

	void	AdjustInitValue( OUT int& iChangedSpirit, OUT int& iChangedAP )
	{
		m_iPoint[SEnum::LRMTC_SPIRIT].AdjustInitValue( m_iPoint[SEnum::LRMTC_SPIRIT].GetInitValue() - iChangedSpirit );
		m_iPoint[SEnum::LRMTC_AP].AdjustInitValue( m_iPoint[SEnum::LRMTC_AP].GetInitValue() - iChangedAP );
	}

	bool	IsValidCategory( void ) const	{ return ( SEnum::LRLC_NONE < m_kLocalRankingUserInfo.m_iCategory );	}
	bool	UpdateTimeCheck( void );
	bool	IsFirstTimeInput( void ) const	{	return m_wstrUpdated.empty();	}
	void	SetCharacterLevel( IN const u_char ucLevel ){	m_kLocalRankingUnitInfo.m_ucLevel = ucLevel;	}

private:
	int										m_iUID;						// 초기화 프로세스를 위한 UID이다. Unit 정보에만 관여한다.
	std::wstring							m_wstrUpdated;
	boost::timer							m_kElpUpdateGap;

	KCacheDataMaxLimit< int, __int64 >		m_iPoint[SEnum::LRMTC_MAX];

	KLocalRankingUserInfo					m_kLocalRankingUserInfo;
	KLocalRankingUnitInfo					m_kLocalRankingUnitInfo;
};
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
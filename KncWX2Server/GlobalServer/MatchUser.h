#pragma once

#include "FSM/support_FSM.h"
#include "MatchUserFSM.h"
#include "Event.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "X2Data/XSLUnit.h"


//{{ 2011. 06. 14	ÃÖÀ°»ç	´ëÀü °³Æí
#ifdef SERV_PVP_NEW_SYSTEM


#define KMatchUserPtr KMatchUser*
#undef KMatchUserPtr
SmartPointer( KMatchUser );


class KMatchUser
{
	DeclareFSM_MT;
public:
	enum TIMER_ENUM
	{
		TE_MATCH_MAKING_BEGIN = 0,
		TE_MATCH_COMPLETE,

		TE_MAX,
	};

public:
    KMatchUser( IN const KMatchUserInfo& kInfo );
    virtual ~KMatchUser();
	
	UidType		GetCharUID() const		{ return m_kMatchUserInfo.m_iUnitUID; }
	const std::wstring& GetNickName() const { return m_kMatchUserInfo.m_wstrNickName; }
	UidType		GetGSUID() const		{ return m_kMatchUserInfo.m_iGSUID; }
	const KMatchUserInfo& GetMatchUserInfo() const { return m_kMatchUserInfo; }
	UidType		GetPartyUID() const		{ return m_kMatchUserInfo.m_iPartyUID; }

	//{{ 2012. 06. 20	¹Ú¼¼ÈÆ	2012 ´ëÀü ½ÃÁð2
#ifdef SERV_2012_PVP_SEASON2
	char		GetCurrentRank( void ) const	{ return m_kMatchUserInfo.m_cCurrentRank;	}
	bool		IsArrangeRank( void ) const	{ return m_kMatchUserInfo.m_iOfficialMatchCnt < 10;	}
	char		IsBeginner( void ) const	{ return GetRating() == 0;	}
#endif SERV_2012_PVP_SEASON2
	//}}

	int			GetRating() const		{ return m_kMatchUserInfo.m_iRating; }
	int			GetRPoint() const		{ return m_kMatchUserInfo.m_iRPoint; }
	int			GetMatchWaitTime() const{ return m_kMatchUserInfo.m_iMatchWaitTime; }
	const std::set< short >& GetSelectedPvpMap() const { return m_setSelectedPvpMap; }
	bool		IsPvpPowerMode() const	{ return m_bPvpPowerMode; }
	bool		IsPvpItemMode() const	{ return m_bPvpItemMode; }
	double		GetMatchCompleteSeconds() const { return m_tTimer[TE_MATCH_COMPLETE].elapsed(); }
	CXSLUnit::PVP_RANK GetPvpRank() const;
	
	//double		GetExpectPercentageOfVictories() const;	// ±â´ë ½Â·ü

	void		SetMatchType( IN char cMatchType )	{ m_cMatchType = cMatchType; }
	char		GetMatchType() const	{ return m_cMatchType; }

	bool		IsPossibleMatchMakingBegin() const;
	
	bool		Accept();
	bool		IsAccepted();

	void		UpdatePvpGameMode( IN const bool bPvpPowerMode, IN const bool bPvpItemMode );
	void		UpdateSelectedPvpMap( IN const std::set< short >& setPvpMapInfo );

	void		ClearRoomUserInfo()		{ m_vecRoomUserInfo.clear(); }	
	void		UpdateRoomUserInfo( IN const KRoomUserInfo& kRoomUserInfo );
	bool		GetRoomUserInfo( OUT KRoomUserInfo& kRoomUserInfo );
	bool		IsEmptyRoomUserInfo() const { return m_vecRoomUserInfo.empty(); }

public:
	template < class T > void SendToGSCharacter( unsigned short usEventID, const T& data );
	void SendToGSCharacter( unsigned short usEventID );

private:
	char							m_cMatchType;
    KMatchUserInfo					m_kMatchUserInfo;
	std::vector< KRoomUserInfo >	m_vecRoomUserInfo;
	bool							m_bPvpPowerMode;
	bool							m_bPvpItemMode;
	std::set< short >				m_setSelectedPvpMap;
	boost::timer					m_tTimer[TE_MAX];
};

template < class T >
void KMatchUser::SendToGSCharacter( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetGSUID(), -1 };
	KncSend( PI_GLOBAL_MATCH_MAKING, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, GetCharUID(), anTrace, usEventID, data );
}

#endif SERV_PVP_NEW_SYSTEM
//}}



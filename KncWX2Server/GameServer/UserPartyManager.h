#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include <boost/timer.hpp>


//{{ 2011. 12. 20	최육사	랜덤큐브 DB실시간 적용
#ifdef SERV_BATTLE_FIELD_SYSTEM



class KUserPartyManager
{
public:
	struct SWaitPartyUserInfo
	{
		UidType			m_iWaitUnitUID;
		boost::timer	m_tPartyWaitTime;

		SWaitPartyUserInfo()
		{
			m_iWaitUnitUID = 0;
		}
	};

public:
	KUserPartyManager(void);
	~KUserPartyManager(void);

	void		Clear();

	UidType		GetPartyUID() const							{ return m_iPartyUID; }
	void		SetPartyUID( UidType iUID )					{ m_iPartyUID = iUID; }
	UidType		GetCandidatePartyUID() const				{ return m_iCandidatePartyUID; }
	void		SetCandidatePartyUID( UidType iUID )		{ m_iCandidatePartyUID = iUID; }
	UidType		GetQuickJoinPartyUID() const				{ return m_iQuickJoinPartyUID; }
	void		SetQuickJoinPartyUID( UidType iUID )		{ m_iQuickJoinPartyUID = iUID; }
	//{{ 2010. 02. 09  최육사	빠른파티 기능개선
//#ifdef SERV_QUICK_PARTY_UPGRADE	
	void		AddKickedPartyUID( IN UidType iPartyUID );
	bool		CheckKickedPartyUID( IN UidType iPartyUID ) { return ( m_mapKickedPartyUIDList.find( iPartyUID ) != m_mapKickedPartyUIDList.end() ); }
	void		CheckExpiredKickedPartyUID();
//#endif SERV_QUICK_PARTY_UPGRADE
	//}}

	void		ClearWaitPartyUser()						{ m_vecWaitPartyUserList.clear(); }
	void		CheckWaitPartyUser();
	bool		RegWaitPartyUser( IN const UidType iUnitUID );
	bool		UnregWaitPartyUser( IN const UidType iUnitUID );
	bool		IsWaitPartyUser( IN const UidType iUnitUID ) const;
	bool		IsEmptyWaitPartyUser() const				{ return m_vecWaitPartyUserList.empty(); }

private:
	UidType								m_iPartyUID;
	UidType								m_iCandidatePartyUID;
	UidType								m_iQuickJoinPartyUID;
	//{{ 2010. 02. 09  최육사	빠른파티 기능개선
//#ifdef SERV_QUICK_PARTY_UPGRADE
	std::map< UidType, boost::timer >	m_mapKickedPartyUIDList;
//#endif SERV_QUICK_PARTY_UPGRADE
	//}}
	std::vector< SWaitPartyUserInfo >	m_vecWaitPartyUserList;
};



#endif SERV_BATTLE_FIELD_SYSTEM
//}}



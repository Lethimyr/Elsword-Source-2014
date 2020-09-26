#pragma once
#include "KncUidType.h"
#include <boost/timer.hpp>
#include <vector>
#include <map>
#include "CommonPacket.h"


//{{ 2007. 8. 9  최육사  커뮤니티옵션 클래스 }}
struct KBlackListUnit;
class KUserGameOption
{
public:

	KUserGameOption(void);
	~KUserGameOption(void);
	
	// Deny State
	bool IsDenyFriendShip()							{ return ( m_cDenyFriendShip == KDenyOptions::DOS_ON ); }
	bool IsDenyParty()								{ return ( m_cDenyParty == KDenyOptions::DOS_ON ); }
	bool IsDenyTrade()								{ return ( m_cDenyPersonalTrade == KDenyOptions::DOS_ON ); }
	bool IsDenyInviteGuild()						{ return ( m_cDenyInviteGuild == KDenyOptions::DOS_ON ); }
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	bool IsDenyRequestCouple()						{ return ( m_cDenyRequestCouple == KDenyOptions::DOS_ON ); }
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	const KDenyOptions GetDenyOptions();
	bool SetDenyOptions( const KDenyOptions& kDenyOptions );
		
	// Chat Black List functions
	bool IsAddChatBlackListPossible()				{ return ( m_mapChatBlackList.size() < 30 ); }
	void SetChatBlackList( const std::vector< KChatBlackListUnit >& vecBlackList );
	void AddChatBlackListUnit( UidType iUnitUID, std::wstring& wstrNickName );
	void DelChatBlackListUnit( UidType iUnitUID, std::wstring& wstrNickName );
	bool IsChatBlackListUnit( UidType iUnitUID )	{ return ( m_mapChatBlackList.find( iUnitUID ) != m_mapChatBlackList.end() ); }

private:
	char								m_cDenyFriendShip;
	char								m_cDenyInviteGuild;
	char								m_cDenyParty;
	char								m_cDenyPersonalTrade;
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	char								m_cDenyRequestCouple;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
    char                                m_cDenyInvitePractivePVP;
	//{{ 2008. 1. 30  최육사  채팅 블랙 리스트
	std::map< UidType, std::wstring >	m_mapChatBlackList;
	//}}
};


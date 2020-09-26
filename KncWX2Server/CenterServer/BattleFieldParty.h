#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "RoomUser.h"


//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KBattleFieldParty
{
public:
    KBattleFieldParty();
    virtual ~KBattleFieldParty();

	void Clear();    
	void UpdatePartyMemberList( IN const std::vector< UidType >& vecInfo );

	bool IsPartyMember( IN const UidType iUnitUID ) const	{ return ( m_setPartyMemberList.find( iUnitUID ) != m_setPartyMemberList.end() ); }
	int GetNumMember() const								{ return static_cast<int>(m_setPartyMemberList.size()); }
	const std::set< UidType >& GetPartyMemberList() const	{ return m_setPartyMemberList; }
	void GetPartyMemberList( OUT std::vector< UidType >& vecMemberList ) const;

	bool JoinPartyMember( IN const UidType iUnitUID );
	bool LeavePartyMember( IN const UidType iUnitUID );
    
private:
    std::set< UidType >			m_setPartyMemberList;
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}



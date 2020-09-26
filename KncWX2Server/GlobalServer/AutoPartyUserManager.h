#pragma once

#include "AutoPartyUser.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


SmartPointer( KAutoPartyUserFSM );


class KAutoPartyUserManager
{
public:
    KAutoPartyUserManager();
    virtual ~KAutoPartyUserManager();

	void	Init( IN const KAutoPartyConditionInfo& kInfo );

	// 초기화 및 삭제
	bool	Enter( IN const KAutoPartyUserInfo& kInfo );
	bool	Enter( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList );
    void	Reset();
	bool	DeleteUser( IN const UidType iUnitUID );

	// 정보 얻기
    KAutoPartyUserPtr GetUser( IN const UidType iUnitUID );
	void	GetAutoPartyUserList( OUT std::vector< UidType >& vecUserList );
    int		GetNumMember() const;
	int		GetLimitPlayerCount() const				{ return static_cast<int>(m_kConditionInfo.m_cLimitPlayerCount); }
	bool	IsExist( IN const UidType iUnitUID )	{ return ( m_mapMember.find( iUnitUID ) != m_mapMember.end() ); }
 
	// 수락
	bool	Accept( IN const UidType iUnitUID );
	bool	IsAllPlayersAccept();

	// Regroup Party
	void	ClearAllPlayerRegroupPartyInfo();
	bool	UpdateRegroupPartyInfo( IN const KPartyUserInfo& kPartyUserInfo, IN const KPartyRoomUserInfo& kPartyRoomUserInfo );
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	bool	GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo, OUT UidType& iRegroupPartyHostUnitUID );
#else
	bool	GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo );
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	bool	IsAllPlayersUpdatedRegroupPartyInfo();

	//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
	void	GetAutoPartyCancelUserList( OUT std::vector< UidType >& vecCancelUserList );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
	//}

protected:
	KAutoPartyConditionInfo					m_kConditionInfo;
    std::map< UidType, KAutoPartyUserPtr >	m_mapMember;
	static FSMclassPtr						ms_spUserFSM;
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}



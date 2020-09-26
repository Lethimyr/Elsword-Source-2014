#pragma once
#include "GSBattleFieldRoom.h"

// class
#define KBattleFieldListPtr KBattleFieldList*
#undef KBattleFieldListPtr
SmartPointer( KBattleFieldList );


//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KBattleFieldList
{
public:
	KBattleFieldList();
	virtual ~KBattleFieldList(void);

	KGSBattleFieldRoomPtr GetRoomInfo( IN const UidType iRoomUID, IN const bool bErrorLog = false );
	bool	IsExist( IN const UidType iRoomUID )			{ return m_mapList.find( iRoomUID ) != m_mapList.end(); }
	bool	AddRoomInfo( IN const KBattleFieldRoomInfo& kInfo );
	bool	DelRoomInfo( IN const UidType iRoomUID );
	//{{ 2012. 11. 20	최육사		배틀필드 방 정리 기능 추가
#ifdef SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
	void	DelRoomInfoByCnServerUID( IN const UidType iCnServerUID );
#endif SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
	//}}
	bool	UpdateRoomInfo( IN const KBattleFieldRoomInfo& kInfo );
	
	// 조건에 따라 가장 적절한 필드로 이동하기 위한 함수
	bool	GetRoomUIDForJoinBattleField( IN const UidType iUnitUID, IN const UidType iTargetRoomUID, IN const UidType iPartyUID, OUT UidType& iJoinBFRoomUID );

protected:
	UidType GetBattleFieldRoomUID_MaxPartyMemberCount( IN const UidType iUnitUID, IN const UidType iPartyUID );
	UidType GetBattleFieldRoomUID_TargetRoom( IN const UidType iTargetRoomUID );
	UidType GetBattleFieldRoomUID_AllRandom();

protected:
	std::vector< KGSBattleFieldRoomPtr >		m_vecList;
	std::map< UidType, KGSBattleFieldRoomPtr >	m_mapList;
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}
#pragma once

// class
#define KGSBattleFieldRoomPtr KGSBattleFieldRoom*
#undef KGSBattleFieldRoomPtr
SmartPointer( KGSBattleFieldRoom );


//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KGSBattleFieldRoom
{
public:
	KGSBattleFieldRoom( const KBattleFieldRoomInfo& kInfo );
	virtual ~KGSBattleFieldRoom(void);
		
	UidType GetRoomUID() const									{ return m_kRoomInfo.m_RoomUID; }
	int		GetPartyMemberCountByPartyUID( IN const UidType iPartyUID );
	int		GetMaxUserCount() const								{ return static_cast<int>(m_kRoomInfo.m_MaxSlot); }
	int		GetJoinUserCount() const							{ return static_cast<int>(m_kRoomInfo.m_mapUnitUIDPartyUID.size()); }
	int		GetJoinUserCountWithReservedUserCount() const		{ return m_iJoinUserCountWithReservedUserCount; }
	bool	IsJoinUser( IN const UidType iUnitUID ) const		{ return ( m_kRoomInfo.m_mapUnitUIDPartyUID.find( iUnitUID ) != m_kRoomInfo.m_mapUnitUIDPartyUID.end() ); }
	bool	IsExistParty( IN const UidType iPartyUID )			{ return ( m_kRoomInfo.m_mapPartyList.find( iPartyUID ) != m_kRoomInfo.m_mapPartyList.end() ); }

	void	UpdateRoomInfo( IN const KBattleFieldRoomInfo& kInfo );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-06	// 박세훈
	int		GetBossFieldState( void ) const	{	return m_kRoomInfo.m_byteBossFieldState;	}
#endif // SERV_BATTLE_FIELD_BOSS

protected:
	int		CalcJoinUserCountWithReservedUserCount() const;

protected:
	KBattleFieldRoomInfo	m_kRoomInfo;
	int						m_iJoinUserCountWithReservedUserCount;
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}
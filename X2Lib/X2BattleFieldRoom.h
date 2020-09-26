#pragma once

class CX2BattleFieldRoom : public CX2Room
{
public:

// 	enum DUNGEON_GET_ITEM_TYPE
// 	{
// 		DGIT_NONE	= 0,
// 		DGIT_RANDOM,
// 		DGIT_PERSON,
// 		DGIT_END,
// 	};

// 	struct BattleFieldRoomSimpleInfo : public RoomSimpleInfo
// 	{
// 		CX2Dungeon::DIFFICULTY_LEVEL	m_DifficultyLevel;
// 		SEnum::DUNGEON_ID			m_DungeonID;				
// 
// 		BattleFieldRoomSimpleInfo()
// 		{
// 			m_DifficultyLevel	= CX2Dungeon::DL_NORMAL;
// 			m_DungeonID			= SEnum::DI_NONE;
// 		}
// 
// 		virtual void Set_KRoomInfo( const KRoomInfo* pData )
// 		{
// 			RoomSimpleInfo::Set_KRoomInfo( pData );
// 
// 			m_DifficultyLevel	= (CX2Dungeon::DIFFICULTY_LEVEL)pData->m_DifficultyLevel;
// 			m_DungeonID			= (SEnum::DUNGEON_ID)pData->m_iDungeonID;
// 		}
// 	};

//private:
	CX2BattleFieldRoom() 
		: m_uiBattleFieldId( SEnum::VMI_INVALID )
		, m_eGetItemType( CX2Room::DGIT_RANDOM ) 
	{};

public:
	
	//CX2BattleFieldRoom( const KEGS_JOIN_BATTLE_FIELD_ACK& kEvent_ );
	virtual ~CX2BattleFieldRoom(void);

	UINT							GetBattleFieldId() const { return m_uiBattleFieldId; }
	void							SetBattleFieldId( const UINT uiBattleFieldId_ ) { m_uiBattleFieldId = uiBattleFieldId_; }

	virtual void					Set_KRoomInfo( const KRoomInfo& kRoomInfo_ );
	virtual int						GetClickSlot( int x, int y ) { return 0; }

	
	DUNGEON_GET_ITEM_TYPE			GetGetItemType() const { return m_eGetItemType; }
	void							SetGetItemType( DUNGEON_GET_ITEM_TYPE val) { m_eGetItemType = val; }

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef HEAP_BROKEN_BY_ROOM
	virtual void ApplyRoomPacketData();
#endif // HEAP_BROKEN_BY_ROOM
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


protected:
	UINT							m_uiBattleFieldId;
	DUNGEON_GET_ITEM_TYPE			m_eGetItemType;
};


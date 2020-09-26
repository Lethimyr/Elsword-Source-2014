#pragma once
#include "x2room.h"


class CX2DungeonRoom : public CX2Room
{
	public:
		
		enum RANK_TYPE
		{
			RT_NONE		= 0,
			RT_F,
			RT_E,
			RT_D,
			RT_C,
			RT_B,
			RT_A,
			RT_S,
			//{{ kimhc // 2011-02-22 // 던전랭크 SS 등급 추가
			RT_SS
			//}} kimhc // 2011-02-22 // 던전랭크 SS 등급 추가
		};

//#ifndef NOT_USE_DICE_ROLL
//		enum DUNGEON_GET_ITEM_TYPE
//		{
//			DGIT_NONE	= 0,
//			DGIT_RANDOM,
//			DGIT_PERSON,
//			DGIT_END,
//		};
//#endif //NOT_USE_DICE_ROLL

		struct DungeonRoomSimpleInfo : public RoomSimpleInfo
		{
			CX2Dungeon::DIFFICULTY_LEVEL	m_DifficultyLevel;
			SEnum::DUNGEON_ID			m_DungeonID;				
#ifdef SERV_RANDOM_DAY_QUEST
			CX2Dungeon::DUNGEON_MODE		m_eDungeonMode;
#endif SERV_RANDOM_DAY_QUEST

			DungeonRoomSimpleInfo()
			{
				m_DifficultyLevel	= CX2Dungeon::DL_NORMAL;
				m_DungeonID			= SEnum::DI_NONE;
#ifdef SERV_RANDOM_DAY_QUEST
				m_eDungeonMode		= CX2Dungeon::DM_INVALID; 
#endif SERV_RANDOM_DAY_QUEST
			}

			virtual void Set_KRoomInfo( const KRoomInfo* pData )
			{
				RoomSimpleInfo::Set_KRoomInfo( pData );

				m_DifficultyLevel	= (CX2Dungeon::DIFFICULTY_LEVEL)pData->m_DifficultyLevel;
				m_DungeonID			= (SEnum::DUNGEON_ID)pData->m_iDungeonID;
#ifdef SERV_RANDOM_DAY_QUEST
				m_eDungeonMode		= (CX2Dungeon::DUNGEON_MODE)pData->m_cDungeonMode;
#endif SERV_RANDOM_DAY_QUEST
			}
		};


	public:
		CX2DungeonRoom();
		virtual ~CX2DungeonRoom(void);

		CX2Dungeon::DIFFICULTY_LEVEL	GetDifficulty()		{ return m_DifficultyLevel;		}
		SEnum::DUNGEON_ID			GetDungeonID() const	{ return m_DungeonID;			}

		void							SetDifficulty( CX2Dungeon::DIFFICULTY_LEVEL difficultyLevel )	{ m_DifficultyLevel		= difficultyLevel;	}
		void							SetDungeonID( SEnum::DUNGEON_ID dungeonID )				{ m_DungeonID			= dungeonID;		}

#ifdef SERV_RANDOM_DAY_QUEST
		CX2Dungeon::DUNGEON_MODE		GetDungeonMode()		{ return m_eDungeonMode;		}
		void							SetDungeonMode( CX2Dungeon::DUNGEON_MODE dungeonMode )	{ m_eDungeonMode		= dungeonMode;	}
#endif SERV_RANDOM_DAY_QUEST

		void							SetDungeonEX( const WCHAR* pMsg ) { m_DungeonEx = pMsg; }
		wstring							GetDungeonEX() { return m_DungeonEx; } 

		virtual void					Set_KRoomInfo( const KRoomInfo& kRoomInfo_ );

		virtual int						GetClickSlot( int x, int y );

		float							GetPlayTimeLimit() const { return m_fPlayTimeLimit; }
		void							SetPlayTimeLimit( float fTime ) { m_fPlayTimeLimit = fTime; }

//#ifndef NOT_USE_DICE_ROLL
//		DUNGEON_GET_ITEM_TYPE			GetDungeonGetItemType() { return m_GetItemType; }
//		void							SetGetItemType(CX2DungeonRoom::DUNGEON_GET_ITEM_TYPE val) { m_GetItemType = val; }
//#endif //NOT_USE_DICE_ROLL

		bool							GetCharLvLimit() { return m_bLvLimit; }
		void							SetCharLvLimit(bool val) { m_bLvLimit = val; }

		//{{ 허상형 : [2011/3/14/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
		bool							IsDefenceDungeon( SEnum::DUNGEON_ID eDungeonID );
		bool							IsDefenceDungeon( int iDungeonID );
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 허상형 : [2011/3/14/] //	월드 미션

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef HEAP_BROKEN_BY_ROOM
	    virtual void                    ApplyRoomPacketData();
#endif // HEAP_BROKEN_BY_ROOM
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	protected:
		CX2Dungeon::DIFFICULTY_LEVEL	m_DifficultyLevel;
		SEnum::DUNGEON_ID			m_DungeonID;
#ifdef SERV_RANDOM_DAY_QUEST
		CX2Dungeon::DUNGEON_MODE		m_eDungeonMode;
#endif SERV_RANDOM_DAY_QUEST

		wstring							m_DungeonEx;//던전 설명

		float							m_fPlayTimeLimit;		

//#ifndef NOT_USE_DICE_ROLL
//		DUNGEON_GET_ITEM_TYPE			m_GetItemType;
//#endif //NOT_USE_DICE_ROLL
		
		// 던전방 레벨제한
		bool							m_bLvLimit;
		
};


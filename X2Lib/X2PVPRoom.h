#pragma once

class CX2PVPRoom : public CX2Room
{
	public:
		enum PVP_GAME_TYPE
		{
			PGT_TEAM = 0,
			PGT_TEAM_DEATH,
			PGT_SURVIVAL,
			PGT_TAG,
		};

		enum TEAM_DEATH_WIN_KILL_NUM_LIMIT
		{
			DTWKNL_MIN = 4,
			DTWKNL_MAX = 28,
		};

		enum SURVIVAL_DEATH_WIN_KILL_NUM_LIMIT
		{
			SDWKNL_MIN = 3,
			SDWKNL_MAX = 12,
		};

		enum TEAM_PLAY_TIME_LIMIT
		{
			TPTL_MIN = 200,
			TPTL_MAX = 400,
		};

		enum DEATH_MATCH_PLAY_TIME_LIMIT
		{
			DMPTL_MIN = 200,
			DMPTL_MAX = 1100,
		};



#ifdef SERVER_PVP_BASE_DEFENCE_TEST

		struct PVPBaseDefenceData
		{
			UidType m_StatueOfRedUID;
			UidType m_StatueOfBlueUID;

			PVPBaseDefenceData()
				: m_StatueOfRedUID( -1 )
				, m_StatueOfBlueUID( -1 )
			{
			}

		};

		struct PVPGameOption
		{
			bool m_bBaseDefence;
			PVPBaseDefenceData m_BaseDefenceData;

			bool m_bBossCombat;

			PVPGameOption() 
				: m_bBaseDefence( false )
				, m_bBossCombat( false )
			{
			}
		};

#endif SERVER_PVP_BASE_DEFENCE_TEST





		struct PVPRoomSimpleInfo : public RoomSimpleInfo
		{
			PVP_GAME_TYPE				m_PVPGameType;
			int							m_WinKillNum;
			bool						m_bCanIntrude;
			CX2World::WORLD_ID			m_WorldID;
#ifdef DUNGEON_ITEM
			bool						m_bItemMode;
#endif

			PVPRoomSimpleInfo()
			{
				m_PVPGameType	= PGT_TEAM;
				m_WinKillNum	= 0;
				m_bCanIntrude	= true;
				m_WorldID		= CX2World::WI_NONE;
#ifdef DUNGEON_ITEM
				m_bItemMode		= true;
#endif
			}
			virtual void Set_KRoomInfo( const KRoomInfo* pData )
			{
				RoomSimpleInfo::Set_KRoomInfo( pData );

				m_PVPGameType		= (PVP_GAME_TYPE)pData->m_PVPGameType;
				m_WinKillNum		= pData->m_WinKillNum;
				m_bCanIntrude		= pData->m_bCanIntrude;
				m_WorldID			= (CX2World::WORLD_ID)pData->m_WorldID;
#ifdef DUNGEON_ITEM
				m_bItemMode			= pData->m_bIsItemMode;
#endif
			}
		};


	public:
		CX2PVPRoom();
		virtual ~CX2PVPRoom(void);
		
		PVP_GAME_TYPE		GetPVPGameType()							{ return m_PVPGameType;			}
		int					GetPVPGameType_LUA()						{ return (int)m_PVPGameType; }
		
		int					GetWinKillNum()								{ return m_WinKillNum;			}
		float				GetPlayTimeLimit()							{ return m_fPlayTime;			}
		bool				GetCanIntrude()								{ return m_bCanIntrude;			}
		int					GetWorldID()								{ return m_WorldID;				}
		int					GetWorldIDForUI()							{ return m_WorldIDForUI;		}

		void				SetPVPGameType( PVP_GAME_TYPE gameType )	{ m_PVPGameType		= gameType;		}
		void				SetWinKillNum( int killNum )				{ m_WinKillNum		= killNum;		}
		void				SetPlayTime( float fPlayTime )				{ m_fPlayTime		= fPlayTime;	}
		void				SetCanIntrude( bool bCanIntrude )			{ m_bCanIntrude		= bCanIntrude;	}
		void				SetWorldID( int worldID )					{ m_WorldID			= worldID;		}
		void				SetWorldIDForUI( int worldID )				{ m_WorldIDForUI	= worldID;		}

#ifdef DUNGEON_ITEM		
		void				SetItemMode(bool bVal);
		bool				GetItemMode();
#endif

		virtual void		Set_KRoomInfo( const KRoomInfo& kRoomInfo_ );

		virtual bool		Handler_EGS_CHANGE_SLOT_OPEN_NOT( KEGS_CHANGE_SLOT_OPEN_NOT& kEGS_CHANGE_SLOT_OPEN_NOT );
		

		bool				Handler_EGS_CHANGE_MAP_REQ( int worldID );
		bool				Handler_EGS_CHANGE_MAP_ACK( KEGS_CHANGE_MAP_ACK& kEGS_CHANGE_MAP_ACK );
		bool				Handler_EGS_CHANGE_MAP_NOT( KEGS_CHANGE_MAP_NOT& kEGS_CHANGE_MAP_NOT );

		virtual int			GetClickSlot( int x, int y );
#ifdef SERV_PET_SYSTEM
		bool				GetClickPetSlot( int x, int y );
#endif
		bool				IsRoomMaster( SlotData* pSlotData = NULL );

		//{{ 2009. 10. 22  최육사	대전방 버그수정
		static bool			IsValidWinKillNum( char cPvpGameType, char cWinKillNum );
		static bool			IsValidPvpPlayTime( char cPvpGameType, int iPlayTime );
		//}}

		static char			GetValidWinKillNum( char cPvpGameType );
		static int			GetValidPvpPlayTime( char cPvpGameType );

#ifdef SERVER_PVP_BASE_DEFENCE_TEST
		CX2PVPRoom::PVPGameOption& GetPVPGameOption() { return m_PVPGameOption; }
#endif SERVER_PVP_BASE_DEFENCE_TEST
				
#ifdef SERV_PVP_NEW_SYSTEM
		bool IsOfficialPvp() { return m_bOfficialPvp; }
		void SetOfficialPvp(bool bVal) { m_bOfficialPvp = bVal; }
#endif

#ifdef HEAP_BROKEN_BY_ROOM
		virtual void ApplyRoomPacketData();
#endif // HEAP_BROKEN_BY_ROOM


	protected:
		PVP_GAME_TYPE				m_PVPGameType;

		int							m_WinKillNum;
		float						m_fPlayTime;
		bool						m_bCanIntrude;
		int							m_WorldID;
		int							m_WorldIDForUI;

#ifdef DUNGEON_ITEM
		bool							m_bItemMode;
#endif
				
#ifdef SERVER_PVP_BASE_DEFENCE_TEST
		PVPGameOption m_PVPGameOption; 
#endif SERVER_PVP_BASE_DEFENCE_TEST

#ifdef SERV_PVP_NEW_SYSTEM
		bool m_bOfficialPvp;
#endif
};





























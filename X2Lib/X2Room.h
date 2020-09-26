#pragma once
/**@file X2Room.h
   @breif Room 헤더 파일 
*/

#define PING_TIME_TO_BE_INITIALIZED 99999

/** @class CX2Room
	@brief Room 클래스  
	@date 
*/
class CX2Room
{
	public:
		/// ROOM 형식
		enum ROOM_TYPE
		{
            RT_INVALID = -1,		/// 없음
			RT_PVP,					/// 대전
			RT_DUNGEON,				/// 던전
			RT_SQUARE,				/// 광장
			RT_TRADE,				/// 거래
			RT_TRAININGCENTER,		/// 자유 훈련
			RT_PERSONAL_SHOP,		/// 개인 상점
            RT_ARCADE,				/// 아케이드 모드 // FieldFix: 삭제 해야함

			RT_BATTLE_FIELD,		/// 배틀 필드

            RT_TOTAL_NUM,			/// 전체 개수
		};

		/// ROOM 상태
		enum ROOM_STATE
		{
			RS_INIT		= 1,		/// 초기
			RS_CLOSED,				/// 닫혀 있음
			RS_WAIT,				/// 대기
			RS_TIME_COUNT,			/// 대전, 던전 3, 2, 1 시작 하기 카운트 상태
			RS_LOADING,				/// 불러오는 중
			RS_PLAY,				/// 게임 중
			RS_RESULT,				/// 결과창
			S_RETURN_TO_FIELD,		/// 던전 대전을 끝내고 마을로 돌아가는 중간 상태
		};

		/// 채팅 형태
		enum ROOM_CHAT_TYPE
		{
			RCT_ALL = 0,			/// 전체
			RCT_TEAM,				/// 팀
			RCT_WHISPER,			/// 귓속말
		};

		/// ROOM 기본 정보
		struct RoomSimpleInfo
		{
			UidType		m_RoomUID;		/// ROOM UID
			wstring		m_RoomName;		/// ROOM 이름
			ROOM_STATE	m_RoomState;	/// ROOM 상태

			bool		m_bPublic;		/// 공용
			int			m_MaxSlot;		/// 최대 슬롯
			int			m_JoinSlot;		/// 들어온 슬롯

			/// 생성자
			RoomSimpleInfo()
			{
				m_RoomUID		= 0;
				m_RoomState		= RS_INIT;

				m_bPublic		= true;
				m_MaxSlot		= 0;
				m_JoinSlot		= 0;
			}
			/// ROOM 정보 설정
			virtual void Set_KRoomInfo( const KRoomInfo* pData )
			{
				m_RoomUID			= pData->m_RoomUID;
				m_RoomName			= pData->m_RoomName;
				m_RoomState			= (ROOM_STATE)pData->m_RoomState;

				m_bPublic			= pData->m_bPublic;
				m_MaxSlot			= pData->m_MaxSlot;
				m_JoinSlot			= pData->m_JoinSlot;
			}
		};

		/// 팀 번호
		enum TEAM_NUM
		{
			TN_NONE		= -1,	/// 없음
			TN_RED		=  0,	/// 레드
			TN_BLUE		=  1,	/// 블루
			TN_MONSTER	=  2,	/// 몬스터
		};

		/// 
		enum DUNGEON_GET_ITEM_TYPE
		{
			DGIT_NONE	= 0,
			DGIT_RANDOM,
			DGIT_PERSON,
			DGIT_END,
		};

		enum SLOT_STATE
		{
			SS_EMPTY	= 1,
			SS_CLOSE,
			SS_WAIT,
			SS_LOADING,
			SS_PLAY,
		};

		// FieldFix: 엘소드를 제외한 이차전직에만 사용중, 꾸준한 관리가 안되고 있으므로 삭제하는게 좋을지도..
		enum ROOM_MOTION
		{
			RM_WAIT1 = 0,
			RM_WAIT2,
			RM_END,
		};

		struct SlotData
		{
			int							m_Index;
			SLOT_STATE					m_SlotState;
			
			int							m_TeamNum;
			UidType						m_UnitUID;
			DWORD						m_PingTime;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            DWORD                       m_adwPingSendTime[5];
            unsigned                    m_uPingSendCount;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            float                       m_fAvgPingTime;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            float                       m_fAvgPingTimeSqr;
            float                       m_fAvgPingStd;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            DWORD                       m_dwPingCount;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

			float						m_fElapsedTimeAfterJoinRoom;

			

			CX2Unit*					m_pUnit;
			CX2UnitViewerUI*			m_pUnitViewer;

//#ifdef PVP_BOSS_COMBAT_TEST
//			bool						m_bIsBoss;
//			
//#endif PVP_BOSS_COMBAT_TEST


#ifdef TAG_MATCH_TEST
			CX2Unit*					m_pSubUnit;
#endif TAG_MATCH_TEST

#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
			UINT						m_uiUserPacketCountToReceive;		/// 받은 유저 패킷 수
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM

#ifdef SERV_PVP_NEW_SYSTEM
			
			UidType						m_iNpcUid;
			int							m_iNpcId;
			wstring						m_wstrNpcName;
			int							m_iNpcRating;
			bool						m_bNpc;
#endif

#ifdef PVP_SEASON2
			char						m_cRank;
#endif

			bool						m_bHost;
			bool						m_bMySlot;
			bool						m_bReady;
			bool						m_bPitIn;
			bool						m_bTrade;
			bool						m_bObserver;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			UidType						m_RelationshipTargetUID;
#endif // 

			SlotData()
			{
				m_Index			= 0;
				m_SlotState		= SS_CLOSE;
				m_bHost			= false;
				m_bMySlot		= false;
				m_bReady		= false;
				m_bPitIn		= false;
				m_bTrade		= false;
				m_TeamNum		= TN_RED;
				m_UnitUID		= 0;
				m_PingTime		= PING_TIME_TO_BE_INITIALIZED;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                ZeroMemory( m_adwPingSendTime, sizeof(m_adwPingSendTime) );
                m_uPingSendCount = 0;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                m_fAvgPingTime  = 0.f;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                m_fAvgPingTimeSqr = 0.f;
                m_fAvgPingStd = 0.f;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                m_dwPingCount   = 0;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
				m_fElapsedTimeAfterJoinRoom	= 0.f;
				m_bObserver		= false;

				m_pUnit			= NULL;
				m_pUnitViewer	= NULL;

//#ifdef PVP_BOSS_COMBAT_TEST
//				m_bIsBoss		= false;
//#endif PVP_BOSS_COMBAT_TEST


#ifdef TAG_MATCH_TEST
				m_pSubUnit		= NULL;
#endif TAG_MATCH_TEST

#ifdef SERV_PVP_NEW_SYSTEM
				m_bNpc			= false;
				m_iNpcUid		= 0;
				m_iNpcId		= 0;
				m_wstrNpcName	= L"";
				m_iNpcRating		= 0;
#endif

#ifdef PVP_SEASON2
				m_cRank = 0;
#endif

#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
				m_uiUserPacketCountToReceive = 0;		/// 받은 유저 패킷 수
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM


#ifdef ADDED_RELATIONSHIP_SYSTEM
				m_RelationshipTargetUID = 0;
#endif // ADDED_RELATIONSHIP_SYSTEM

			}
			virtual ~SlotData()
			{
				Free();
			}
			
			virtual void Free();

            CX2Unit*    GetUnit()   { return m_pUnit; }

			bool	Set_KRoomSlotInfo( const KRoomSlotInfo& kRoomSlotInfo );
			bool	Set_KRoomSlotInfoOfMine( const KRoomSlotInfo& kRoomSlotInfo_ );
			bool	Set_KRoomSlotInfoOfOthers( const KRoomSlotInfo& kRoomSlotInfo_ );
			void	Set_KRoomSlotInfoInCloseOrEmptyRoomstate();

			void	CheckMakeUnitViewer();
			void	MakeUnitViewer();
			bool	IsMyUnit() const;
			bool	GetIsHost() { return m_bHost; }
			bool	GetIsReady() { return m_bReady; }
			bool	GetIsPitIn() { return m_bPitIn; }
			int		GetSlotState() const { return (int)m_SlotState; }
			int		GetTeamNum()	{ return m_TeamNum; }
//#ifdef PVP_BOSS_COMBAT_TEST
//			bool GetIsBoss() const { return m_bIsBoss; }
//			void SetIsBoss(const bool val) { m_bIsBoss = val; }
//#endif PVP_BOSS_COMBAT_TEST

#ifdef OBSERVER_DISABLE_IP
			bool	IsObserver()	{ return m_bObserver;	}
#endif OBSERVER_DISABLE_IP

#ifdef SERV_NEW_PVPROOM_PROCESS
			bool	GetIsObserver() { return m_bObserver; }
#endif SERV_NEW_PVPROOM_PROCESS

#ifdef RIDING_SYSTEM
			void				SetOrClearRidingPetInfo( CX2Unit* pUnit_, const KRoomSlotInfo& kRoomSlotInfo_ );
#endif	// RIDING_SYSTEM
		};

#ifdef SERV_PVP_NEW_SYSTEM
		struct RoomNpcSlot
		{
			int			m_iNpcId;
			UidType		m_iNpcUid;
			int			m_iLevel;
			int			m_iRating;
			wstring		m_wstrNpcName;
#ifdef PVP_SEASON2
			char		m_cRank;
#endif

			float		m_fBaseHP;
			float		m_fAtkPhysic;
			float		m_fAtkMagic;
			float		m_fDefPhysic;
			float		m_fDefMagic;

			RoomNpcSlot()
			{
				m_iNpcId = 0;
				m_iNpcUid = 0;
				m_iLevel = 0;
				m_iRating = 0;
				m_wstrNpcName = L"";
#ifdef PVP_SEASON2
				m_cRank = 0;
#endif

				m_fBaseHP = 0.f;
				m_fAtkPhysic = 0.f;
				m_fAtkMagic = 0.f;
				m_fDefPhysic = 0.f;
				m_fDefMagic = 0.f;
			}
		};
#endif


	public:
		CX2Room();
		virtual ~CX2Room(void);

		void				ConnectRelayServer( const WCHAR* pIP, int port );

		HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		void				P2PPacketHandler();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		bool				P2PPacketHandler( CKTDNUDP::RecvData* pRecvData );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
		void				TestPing();

		UidType				GetRoomUID()								{ return m_RoomUID;						}
		const WCHAR*		GetRoomName()								{ return m_RoomName.c_str();			}
		ROOM_TYPE			GetRoomType()								{ return m_RoomType;					}
		ROOM_STATE			GetRoomState()								{ return m_RoomState;					}
		int					GetRoomState_LUA()							{ return (int)m_RoomState;				}
		bool				GetRoomPublic()								{ return m_bPublic;						}

		void				SetRoomUID( UidType roomUID )				{ m_RoomUID		= roomUID;				}
		void				SetRoomName( WCHAR* pName )					{ m_RoomName	= pName;				}
#ifdef SERV_NEW_PVPROOM_PROCESS
		void				SetRoomName( wstring wstrName )				{ m_RoomName	= wstrName;				}
#endif SERV_NEW_PVPROOM_PROCESS
		void				SetRoomState( ROOM_STATE roomState )		{ m_RoomState	= roomState;			}
		void				SetRoomPublic( bool bPublic )				{ m_bPublic		= bPublic;				}

		SlotData*			GetObserverSlotData( int index );
		SlotData*			GetObserverSlotDataByUnitUID( UidType unitUID );
		SlotData*			GetObserverSlotDataByNickName( const WCHAR* pNickName );
		SlotData*			GetSlotData( int index );
		SlotData*			GetSlotDataByUnitUID( UidType unitUID );
		SlotData*			GetSlotDataByNickName( const WCHAR* pNickName );

#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
        void                ResetSlotPingSendsAll();
        bool                ResetSlotPingSendByUnitUID( UidType unitUID );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

		int					GetUserNum();
		int					GetSlotNum()								{ return (int)  m_SlotDataList.size();	}
		int					GetObserverSlotNum()						{ return (int)  m_ObserverSlotDataList.size();	}
		int					GetActiveObserverSlotNum();
		int					GetTeamNum( int teamNum, bool bGame );
		int					GetIndexByUnitUID( UidType unitUid );
		void				SetIntrudeGame( bool bIntrude )				{ m_bIntrudeGame = bIntrude;			}
		bool				GetIntrudeGame()							{ return m_bIntrudeGame;				}

		SlotData*			GetHostSlot();
		SlotData*			GetMySlot();

		virtual void		Set_KRoomInfo( const KRoomInfo& kRoomInfo_ );
		virtual void		Set_KRoomSlotInfo( const KRoomSlotInfo& kRoomSlotInfo_ );
		virtual void		Set_KRoomSlotInfoList( const std::vector<KRoomSlotInfo>& vecSlot_ );
		virtual void		Set_KRoomObserverSlotInfoList( std::vector<KRoomSlotInfo>& vecSlot );

		void				CheckMakeUnitViewer( SlotData* pSlotData_ );
		void				MatchingMySlotData( const std::vector<KRoomSlotInfo>& vecSlot_ );

#ifdef SERV_PVP_NEW_SYSTEM
		void				Set_KRoomSlotInfoNpc( std::map< UidType, KPvpNpcInfo > &mapNpcInfo );
		void				DeleteNpcSlot();
		std::vector< CX2Room::RoomNpcSlot >& GetNpcSlot() { return m_vecNpcSlot; }
		CX2Room::RoomNpcSlot *GetNpcSlotData(UidType iUid);
		bool				IsNpcSlot(UidType iNpcUid);
		int					GetNpcSlotNum()	{ return (int)  m_vecNpcSlot.size();	}
		CX2Room::RoomNpcSlot *GetNpcSlotDataByIndex( int iIndex_ );
#endif	

		virtual bool		Handler_EGS_JOIN_ROOM_NOT( KEGS_JOIN_ROOM_NOT& kEGS_JOIN_ROOM_NOT );

		virtual bool		Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason = NetError::NOT_LEAVE_ROOM_REASON_00 );
		virtual bool		Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK );
		virtual bool		Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT );

		virtual bool		Handler_EGS_LEAVE_GAME_REQ();
		virtual bool		Handler_EGS_LEAVE_GAME_ACK( KEGS_LEAVE_GAME_ACK& kEGS_LEAVE_GAME_ACK );
		virtual bool		Handler_EGS_LEAVE_GAME_NOT( KEGS_LEAVE_GAME_NOT& kEGS_LEAVE_GAME_NOT );

		virtual bool		Handler_EGS_CHANGE_TEAM_REQ( CX2Room::TEAM_NUM teamNum );
		virtual bool		Handler_EGS_CHANGE_TEAM_ACK( KEGS_CHANGE_TEAM_ACK& kEGS_CHANGE_TEAM_ACK );
		virtual bool		Handler_EGS_CHANGE_TEAM_NOT( KEGS_CHANGE_TEAM_NOT& kEGS_CHANGE_TEAM_NOT );

		virtual bool		Handler_EGS_CHANGE_READY_REQ( );
		virtual bool		Handler_EGS_CHANGE_READY_ACK( KEGS_CHANGE_READY_ACK& kEGS_CHANGE_READY_ACK );
		virtual bool		Handler_EGS_CHANGE_READY_NOT( KEGS_CHANGE_READY_NOT& kEGS_CHANGE_READY_NOT );

		virtual bool		Handler_EGS_CHANGE_PITIN_REQ( );
		virtual bool		Handler_EGS_CHANGE_PITIN_ACK( KEGS_CHANGE_PITIN_ACK& kEGS_CHANGE_PITIN_ACK );
		virtual bool		Handler_EGS_CHANGE_PITIN_NOT( KEGS_CHANGE_PITIN_NOT& kEGS_CHANGE_PITIN_NOT );

		virtual bool		Handler_EGS_CHANGE_SLOT_OPEN_REQ( int slotIndex );
		virtual bool		Handler_EGS_CHANGE_SLOT_OPEN_ACK( KEGS_CHANGE_SLOT_OPEN_ACK& kEGS_CHANGE_SLOT_OPEN_ACK );
		virtual bool		Handler_EGS_CHANGE_SLOT_OPEN_NOT( KEGS_CHANGE_SLOT_OPEN_NOT& kEGS_CHANGE_SLOT_OPEN_NOT );


		//virtual bool		Handler_EGS_END_GAME_NOT( KEGS_END_GAME_NOT& kKEGS_END_GAME_NOT );

		virtual bool		Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT& kEGS_CHANGE_EQUIPPED_ITEM_NOT );

		virtual bool		Handler_EGS_BAN_USER_REQ( UidType unitUID );
		virtual bool		Handler_EGS_BAN_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		// oasis907 : 김상윤 [2009.11.19] //
		bool Handler_EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT( KEGS_CHANGE_SKILL_INFO_IN_ROOM_NOT& kEvent_ );

		//{{ kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
#ifdef	SERV_CHOOSE_FASTEST_HOST
		void				Send_EGS_UDP_PING_SCORE_NOT( const DWORD dwAveragePingTime_ );	/// 다른 유저들과의 핑 평균 시간
		DWORD				GetAveragePingTimeForGameEdit() const { return m_dwAveragePingTimeForGameEdit; }
		void				SetAveragePingTimeForGameEdit( const DWORD dwAveragePingTimeForGameEdit_ ) { m_dwAveragePingTimeForGameEdit = dwAveragePingTimeForGameEdit_; }
#endif	SERV_CHOOSE_FASTEST_HOST
		//}} kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드

		void				PlayMotionByID( ROOM_MOTION motionID, CX2UnitViewerUI* pUnitViewerUI );

		virtual bool		GetBan() { return m_bBan; }
		virtual void		SetBan( bool bBan ) { m_bBan = bBan; }

		virtual bool		GetNotEnoughSpirit() { return m_bNotEnoughSpirit; }
		virtual void		SetNotEnoughSpirit( bool bCheck ) { m_bNotEnoughSpirit = bCheck; }

		virtual int			GetClickSlot( int x, int y ) = NULL;
		bool				GetRecievePitInNot() { return m_bRecievePitInNot; }

		wstring				GetCenterServerIP() const { return m_wstrCenterServerIP; }
		void				SetCenterServerIP( const WCHAR* val) { m_wstrCenterServerIP = val; }
		
		int					GetNumOfPlayers() const { return m_iNumOfPlayers; }
		void				SetNumOfPlayers(int val) { m_iNumOfPlayers = val; }

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		bool				GetCheckUDPGameCountStart()	{return m_bCheckGameStart; }
		void				SetCheckUDPGameCountStart(bool _bGameStart) { m_bCheckGameStart = _bGameStart; }
#endif//UDP_CAN_NOT_SEND_USER_KICK

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		void				MoveGamePlayNetworkInfoFromMapToVector( OUT vector<KGamePlayNetworkInfo>& vecGamePlayNetworkInfo_ );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO


		void				ClearSlotDataList()
		{
			for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
			{
				SlotData* pSlotData = m_SlotDataList[i];
				SAFE_DELETE( pSlotData );
			}
			m_SlotDataList.clear();
		}

		void				ClearObserverSlotDataList()
		{
			for( int i = 0; i < (int)m_ObserverSlotDataList.size(); i++ )
			{
				SlotData* pSlotData = m_ObserverSlotDataList[i];
				SAFE_DELETE( pSlotData );
			}
			m_ObserverSlotDataList.clear();
		}

#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
		wstring				GetStringNetworkInfo( const SlotData* pSlotData_ );
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-11
		bool				Handler_EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT( KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT& kEvent_ );
#endif // SERV_NEW_DEFENCE_DUNGEON

	protected:
		void				NetworkProcess();	// UDP 클래스 안에 Peer정보를 유지 갱신하고, 일정간격으로 Ping 테스트를 시도함

		//{{ kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
#ifdef	SERV_CHOOSE_FASTEST_HOST
		bool				GetAverageTestPingTimeToOthers( OUT DWORD& dwAveragePingTime_ );	/// 다른 유저들과의 핑 평균 시간
#endif	SERV_CHOOSE_FASTEST_HOST
		//}} kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		void				InsertGamePlayNetworkInfo( const SlotData* pSlotData_ );
		void				UpdateGamePlayNetworkInfo( const CKTDNUDP::Peer* pPeer_ );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

	public:
		
		struct ROOM_PACKET_DATA
		{
			KRoomInfo					m_RoomInfo;
			vector<KRoomSlotInfo>		m_vecSlot;
			vector<KRoomSlotInfo>		m_vecObserverSlot;
			map<UidType, KPvpNpcInfo>	m_mapPvpNpcInfo;
			wstring						m_wstrCNIP;
			bool						m_bOfficialPvp;
			bool						m_bSetNewData;		/// 초기화 되었는가?

			ROOM_PACKET_DATA()
			{
				Initialize();
			}

			~ROOM_PACKET_DATA()
			{
				Initialize();
			}

			void Initialize()
			{
				m_RoomInfo.Initialize();
				m_vecSlot.resize( 0 );
				m_vecObserverSlot.resize( 0 );
				m_mapPvpNpcInfo.clear();
				m_wstrCNIP.clear();
				m_bOfficialPvp = false;
				m_bSetNewData = false;
			}
		};

		static ROOM_PACKET_DATA		RoomPacketData;

		static void InitializeRoomPacketData();
		static void SetRoomPacketData( const KRoomInfo& roomInfo_, const vector<KRoomSlotInfo>& vecSlot_,
			const vector<KRoomSlotInfo>& vecObserverSlot_, const wstring& wstrCNIP_ );
		static bool IsNewDataSet() { return RoomPacketData.m_bSetNewData; }
		static void SetPvpNpcSlotInfo( const map<UidType, KPvpNpcInfo>& mapPvpNpcSlotInfo_ );
		static void SetOfficialPvp( const bool bOfficialPvp_ );

#ifdef	HEAP_BROKEN_BY_ROOM
		virtual void ApplyRoomPacketData();
#endif	HEAP_BROKEN_BY_ROOM		

	protected:
	
		

		ROOM_TYPE					m_RoomType;

		UidType						m_RoomUID;
		wstring						m_RoomName;		
		ROOM_STATE					m_RoomState;

		float						m_fElapsedTime;

		bool						m_bPublic;
		vector<SlotData*>			m_SlotDataList;
		vector<SlotData*>			m_ObserverSlotDataList;

		bool						m_bIntrudeGame;
		float						m_fPingTestTime;

		bool						m_bBan;
		wstring						m_RelayServerIP;
		int							m_RelayServerPort;

		bool						m_bNotEnoughSpirit;

		bool						m_bRecievePitInNot;
		wstring						m_wstrCenterServerIP;

		int							m_iNumOfPlayers;

#ifdef SERV_PVP_NEW_SYSTEM
		std::vector< RoomNpcSlot >	m_vecNpcSlot;
#endif

#ifdef UDP_CAN_NOT_SEND_USER_KICK
		bool						m_bCheckGameStart;
#endif//UDP_CAN_NOT_SEND_USER_KICK

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		map<UidType, KGamePlayNetworkInfo>	m_mapGamePlayNetworkInfo;	/// 유닛별 p2p 및 relay 관련 정보
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

	//{{ kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드
#ifdef	SERV_CHOOSE_FASTEST_HOST
		DWORD						m_dwAveragePingTimeForGameEdit;		/// 게임Edit에서 임시로 변경하는 Ping값
#endif	SERV_CHOOSE_FASTEST_HOST
	//}} kimhc // 2012-10-16 // 핑이 빠른 유저를 호스트로 변경하는 코드

#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
public:
    void    SetSkipPingSendCheck( bool bSet )   { m_bSkipPingSendCheck = bSet; }
    bool    GetSkipPingSendCheck() const        { return m_bSkipPingSendCheck; }

private:
        bool                        m_bSkipPingSendCheck;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
public:
	void ForceHostForGameEdit( bool bEnable ) { m_bForceHostForGameEdit = bEnable; }

private:
	bool m_bForceHostForGameEdit;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
};
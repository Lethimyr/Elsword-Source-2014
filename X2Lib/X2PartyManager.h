#pragma once

class CX2PartyManager
{
public:
	struct PartyMemberData
	{
		UidType						m_iUnitUID;
		wstring						m_wstrNickName;
		CX2Unit::UNIT_CLASS			m_eUnitClass; 
		int							m_iUnitLevel;
		bool						m_bPartyLeader;

#ifdef SERV_CHANNEL_PARTY
		int							m_iChannelID;
#endif SERV_CHANNEL_PARTY
		// 내 파티인 경우에 유효한 정보			
		bool						m_bGameReady;
		CX2Unit::CONNECT_UNIT_STATE	m_eState;
		int							m_iStateCode;		// 필드인 경우 map id를 담고 있음
		D3DXVECTOR3					m_vPosition;		// 필드에서 캐릭터의 위치
#ifdef SERV_CHANNEL_PARTY
		bool						m_bComeBackUser;
#endif SERV_CHANNEL_PARTY
		//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		wstring						m_IP;
		int							m_Port;
		wstring					    m_InternalIP;
		int							m_InternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
		
//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		int							m_iEquippedTitleID;
#endif	SERV_TOONILAND_CHANNELING_EVENT
//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트			

		PartyMemberData()
		{
			Init();
		}

		void Init()
		{
			m_iUnitUID		= -1;
			m_iUnitLevel	= 0;
			m_wstrNickName	= L"";
			m_eUnitClass	= CX2Unit::UC_NONE;
			m_bPartyLeader	= false;
			m_bGameReady	= false;
			m_eState		= CX2Unit::CUS_NONE;
			m_iStateCode	= -1;
			m_vPosition		= D3DXVECTOR3( 0, 0, 0 );
#ifdef SERV_CHANNEL_PARTY
			m_iChannelID	= 0;
#endif SERV_CHANNEL_PARTY
			
#ifdef COME_BACK_REWARD
			m_bComeBackUser	= false;
#endif
			//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
			m_InternalPort	= 0;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
			//}}
	
//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
			m_iEquippedTitleID = 0;
#endif	SERV_TOONILAND_CHANNELING_EVENT
//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트			

		}
	};
	
	struct PartyData
	{
		//{{ 2010. 03. 03  최육사	채널 통합 파티
		UidType					m_iPartyUID;			// note!! 0이하의 값이면 유효하지 않은 파티, 즉 파티가 없다.
		//}}
		wstring					m_wstrPartyTitle;
		int						m_iDungeonID;
		int 					m_iDungeonDifficulty;
		int						m_iPartyMemberCount;
		int						m_iMaxPartyMemberCount;
		bool					m_bPublic;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		bool					m_bIsLimitLevel;
#endif SERV_DUNGEON_DIFFICULTY_REVISION

#ifdef COME_BACK_REWARD
		bool					m_bComeBackPower;
#endif

		//{{ kimhc // 2011-08-08 // 혈맹의 끝 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		bool					m_bHaveTooniLandEventTitle;	/// 혈맹의 끈 칭호를 가진 유저가 있으면..
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // 혈맹의 끝 이벤트

#ifdef GRACE_OF_EL
		// m_bHaveTooniLandEventTitle 로 사용함.
#endif

		CX2Room::DUNGEON_GET_ITEM_TYPE m_eItemLootingMode;
		vector<PartyMemberData>	m_vecPartyMemberData;
#ifdef HENIR_TEST
		int 					m_iDungeonMode;	//CX2Dungeon::DUNGEON_MODE
		int						m_iPartyFever;
#endif HENIR_TEST


		UidType					m_iPartyLeaderUID;

#ifdef SERV_PVP_NEW_SYSTEM
		bool					m_bPvpParty;
		char					m_pvpMode;	// 0:팀매치, 1:팀데스매치
#endif

		PartyData()
		{
			m_vecPartyMemberData.reserve( 4 );

			Init();
		}

		void Init()
		{
			m_iPartyUID				= -1;
			m_wstrPartyTitle		= GET_STRING( STR_ID_0 );
			m_iDungeonID			= 0;
			m_iDungeonDifficulty	= 0;
			m_iPartyMemberCount		= 0;
			m_iMaxPartyMemberCount	= 4;
			m_iPartyLeaderUID		= -1;
			m_bPublic				= true;
			m_bIsLimitLevel			= true;
			m_eItemLootingMode		= CX2Room::DGIT_RANDOM;

#ifdef HENIR_TEST
			m_iDungeonMode		= 0; // CX2Dungeon::DM_HENIR_PRACTICE;
			m_iPartyFever			= 0;
#endif HENIR_TEST
			
#ifdef COME_BACK_REWARD
			m_bComeBackPower		= false;
#endif

			//{{ kimhc // 2011-08-08 // 혈맹의 끝 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
			m_bHaveTooniLandEventTitle		= false;	/// 혈맹의 끈 칭호를 가진 유저가 있으면..
#endif	SERV_TOONILAND_CHANNELING_EVENT
			//}} kimhc // 2011-08-08 // 혈맹의 끝 이벤트

#ifdef SERV_PVP_NEW_SYSTEM
			m_bPvpParty		= false;
			m_pvpMode		= 0;
#endif

			ClearPartyMemberData();
		}


		void Set_KPartyInfo( const KPartyInfo& kPartyInfo )
		{
			m_iPartyUID				= kPartyInfo.m_iPartyUID;
			m_wstrPartyTitle		= kPartyInfo.m_wstrPartyName;
			m_iDungeonID			= kPartyInfo.m_iDungeonID;
			m_iDungeonDifficulty	= (int) kPartyInfo.m_DifficultyLevel;
			m_iPartyMemberCount		= (int) kPartyInfo.m_JoinSlot;
			m_iMaxPartyMemberCount	= (int) kPartyInfo.m_MaxSlot;
			m_bPublic				= kPartyInfo.m_bPublic;
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
			m_bIsLimitLevel			= kPartyInfo.m_bCheckLowLevel;
#endif SERV_DUNGEON_DIFFICULTY_REVISION

#ifdef COME_BACK_REWARD
			m_bComeBackPower		= kPartyInfo.m_bComeBackBuff;
#endif
			m_eItemLootingMode		= (CX2Room::DUNGEON_GET_ITEM_TYPE) kPartyInfo.m_cGetItemType;

#ifdef HENIR_TEST
			m_iDungeonMode		= (int) kPartyInfo.m_cDungeonMode;
			m_iPartyFever			= (int) kPartyInfo.m_sPartyFever;
#endif HENIR_TEST

#ifdef SERV_PVP_NEW_SYSTEM
		if( kPartyInfo.m_cPartyType == KPartyInfo::PT_PVP_PARTY )
			m_bPvpParty = true;
		else
			m_bPvpParty = false;
		m_pvpMode = kPartyInfo.m_cPVPGameType;
#endif
		}

		void UpdatePartyLeaderUID()
		{
			for( UINT i=0; i<m_vecPartyMemberData.size(); i++ )
			{
				const PartyMemberData& memberData = m_vecPartyMemberData[i];
				if( true == memberData.m_bPartyLeader )
				{
					m_iPartyLeaderUID = memberData.m_iUnitUID;
				}
			}
		}
		

		int GetPartyMemberCount() { return (int) m_vecPartyMemberData.size(); }
		PartyMemberData* GetPartyMemberData( const int index, UidType exceptUnitUID = -1 );
		PartyMemberData* GetPartyMemberData( const UidType iUnitUID );
		PartyMemberData* GetPartyMemberData( const wstring& nickName );



		// 내 파티원 정보
		void AddPartyMemberData( const KPartyUserInfo& kPartyUserInfo )
		{
			PartyMemberData partyMemberData;

			partyMemberData.m_wstrNickName	= kPartyUserInfo.m_wstrNickName;
			partyMemberData.m_iUnitLevel    = (int) kPartyUserInfo.m_ucLevel;
			partyMemberData.m_eUnitClass    = (CX2Unit::UNIT_CLASS) kPartyUserInfo.m_cUnitClass;
			partyMemberData.m_bPartyLeader	= kPartyUserInfo.m_bHost;
			partyMemberData.m_bGameReady	= kPartyUserInfo.m_bReady;
			partyMemberData.m_eState		= (CX2Unit::CONNECT_UNIT_STATE) kPartyUserInfo.m_cState;
			partyMemberData.m_iStateCode	= kPartyUserInfo.m_iStateCode;
			partyMemberData.m_iUnitUID		= kPartyUserInfo.m_iUnitUID;
#ifdef SERV_CHANNEL_PARTY
			partyMemberData.m_iChannelID	= kPartyUserInfo.m_iChannelID;
#endif SERV_CHANNEL_PARTY
#ifdef COME_BACK_REWARD
			partyMemberData.m_bComeBackUser	= kPartyUserInfo.m_bComeBack;
#endif
			//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
			partyMemberData.m_IP			= kPartyUserInfo.m_wstrIP;
			partyMemberData.m_Port			= kPartyUserInfo.m_usPort;
			partyMemberData.m_InternalIP	= kPartyUserInfo.m_wstrInternalIP;
			partyMemberData.m_InternalPort	= kPartyUserInfo.m_usInternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
			//}}
			//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
			//partyMemberData.m_iEquippedTitleID = kPartyUserInfo.m_iEquippedTitleID;
#endif	SERV_TOONILAND_CHANNELING_EVENT
//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트			

			AddPartyMemberData( partyMemberData );
		}

		// 파티리스트에서 다른 파티의 멤버 정보
		void AddPartyMemberData( const KPartyListUserInfo& kPartyListUserInfo )
		{
			PartyMemberData partyMemberData;

			partyMemberData.m_wstrNickName	= kPartyListUserInfo.m_wstrNickName;
			partyMemberData.m_iUnitLevel    = (int) kPartyListUserInfo.m_ucLevel;
			partyMemberData.m_eUnitClass    = (CX2Unit::UNIT_CLASS) kPartyListUserInfo.m_cUnitClass;
			partyMemberData.m_bPartyLeader	= kPartyListUserInfo.m_bHost;
#ifdef SERV_CHANNEL_PARTY
			partyMemberData.m_eState		= (CX2Unit::CONNECT_UNIT_STATE) kPartyListUserInfo.m_cState;
			partyMemberData.m_iStateCode	= kPartyListUserInfo.m_iStateCode;
			partyMemberData.m_iChannelID	= kPartyListUserInfo.m_iChannelID;
#endif SERV_CHANNEL_PARTY

#ifdef SERV_INTEGRATION
			//{{ oasis907 : 김상윤 [2010.5.18] // 던전 대전 서버군 통합
			partyMemberData.m_iUnitUID		= kPartyListUserInfo.m_iUnitUID;
			//}}
#endif SERV_INTEGRATION

			AddPartyMemberData( partyMemberData );
		}

#ifdef COME_BACK_REWARD
		void AddPartyMemberData( UidType iUnitUID, const wstring& nickName, CX2Unit::UNIT_CLASS eUnitClass, 
			int unitLevel, bool bPartyLeader, bool bGameReady, char cState, int iStateCode, int iChannelID, bool bComeBack_ )
#else
		void AddPartyMemberData( UidType iUnitUID, const wstring& nickName, CX2Unit::UNIT_CLASS eUnitClass, 
			int unitLevel, bool bPartyLeader, bool bGameReady, char cState, int iStateCode, int iChannelID )
#endif
		{
			PartyMemberData partyMemberData;

			partyMemberData.m_iUnitUID      = iUnitUID;
			partyMemberData.m_wstrNickName  = nickName;
			partyMemberData.m_eUnitClass    = eUnitClass;
			partyMemberData.m_iUnitLevel    = unitLevel;
			partyMemberData.m_bPartyLeader  = bPartyLeader;
			partyMemberData.m_bGameReady	= bGameReady;
#ifdef SERV_CHANNEL_PARTY
			partyMemberData.m_eState		= (CX2Unit::CONNECT_UNIT_STATE) cState;
			partyMemberData.m_iStateCode	= iStateCode;
			partyMemberData.m_iChannelID	= iChannelID;
#endif SERV_CHANNEL_PARTY
#ifdef COME_BACK_REWARD
			partyMemberData.m_bComeBackUser	= bComeBack_;
#endif

			AddPartyMemberData( partyMemberData );
		}

		void AddPartyMemberData( const PartyMemberData& partyMemberData )
		{
			if( true == partyMemberData.m_bPartyLeader )
			{
				m_iPartyLeaderUID = partyMemberData.m_iUnitUID;
			}

			m_vecPartyMemberData.push_back( partyMemberData );
		}

		bool RemovePartyMemberData( const int index )
		{
			if( index < 0 || index >= (int)m_vecPartyMemberData.size() )
				return false;
			
			m_vecPartyMemberData.erase( m_vecPartyMemberData.begin() + index );
			return true;
		}

		bool RemovePartyMemberData( const UidType iUnitUID )
		{
			for( UINT i=0; i< m_vecPartyMemberData.size() ; i++ )
			{
				if( iUnitUID == m_vecPartyMemberData[i].m_iUnitUID )
				{
					m_vecPartyMemberData.erase( m_vecPartyMemberData.begin() + i );
					return true;
				}
			}

			return false;
		}

		void ClearPartyMemberData()
		{
			m_vecPartyMemberData.resize(0);
		}

	};
public:
	CX2PartyManager(void);
	virtual ~CX2PartyManager(void);

	CX2PartyManager::PartyData* GetMyPartyData() { return &m_MyPartyData; }
	CX2PartyManager::PartyMemberData* GetMyPartyMemberData();
    
	bool DoIHaveParty();
	bool AmIPartyLeader();
	bool IsMyPartyMember(UidType iUid);
	bool IsAllReady();

	bool IsDungeonInLocal( int iLocalMapID, int iDungeonID );
	bool IsRankableArcadeDungeon( int iDungeonID );

	void CancleReady();

	std::map<wstring, bool> GetHeroRecruitDungeonList();
	bool IsHeroRecruitDungeon(int iDungeoID );
	void UpdateSelectedDungeon();

	//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	HRESULT OnFrameMove( double fTime, float fElapsedTime );
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

public:
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CLOSE_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_JOIN_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_PARTY_REQ( int iReason );
	bool Handler_EGS_LEAVE_PARTY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	
	bool Handler_EGS_INVITE_PARTY_REQ( UidType inviteUID, const wstring& wstrInviteName_, int iServerGroupID_ = -1 );
	bool Handler_EGS_INVITE_PARTY_REQ( const wstring& wstrInviteName_, int iServerGroupID_ = -1 );

	bool Handler_EGS_INVITE_PARTY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_INVITE_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	//{{ 2010. 03. 10  최육사	채널 통합 파티 - PartyUID는 UidType이어야함.
#ifdef SERV_INVITE_PARTY_REPLY_REASON 
	bool Send_EGS_INVITE_PARTY_REPLY_NOT( char Reason, UidType partyUID );
#else 
	bool Send_EGS_INVITE_PARTY_REPLY_NOT( bool bAccept, UidType partyUID );
#endif
	//}}
	bool Handler_EGS_INVITE_PARTY_DENY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool Handler_EGS_PARTY_CHANGE_DUNGEON_REQ( int iDungeonID, int iDifficultyLevel, int iDungeonMode );
	bool Handler_EGS_PARTY_CHANGE_DUNGEON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PARTY_CHANGE_DUNGEON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_UPDATE_PARTY_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_QUICK_START_DUNGEON_GAME_REQ( const CX2PartyManager::PartyData* pPartyData_ );
	bool Handler_EGS_QUICK_START_DUNGEON_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
	bool Handler_EGS_PARTY_GAME_START_REQ();
	bool Handler_EGS_PARTY_GAME_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PARTY_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-07
	bool Handler_EGS_DEFENCE_DUNGEON_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// 해당 Not가 오면, 어둠의 문 던전으로 이동한다.
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ( bool bLimit );
	bool Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	
	bool Handler_EGS_PARTY_CHANGE_HOST_REQ( UidType unitUID );
	bool Handler_EGS_PARTY_CHANGE_HOST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PARTY_CHANGE_HOST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool Handler_EGS_PARTY_BAN_USER_REQ( UidType unitUID );
	bool Handler_EGS_PARTY_BAN_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PARTY_BAN_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ( CX2Room::DUNGEON_GET_ITEM_TYPE eMode );
	bool Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_PARTY_CHANGE_NUM_OF_PER_REQ( int iMaxSlot );
	bool Handler_EGS_PARTY_CHANGE_NUM_OF_PER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PARTY_CHANGE_NUM_OF_PER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef HENIR_TEST
	bool Handler_EGS_PARTY_FEVER_POINT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_PARTY_FEVER_STATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif HENIR_TEST

#ifdef SERV_PVP_NEW_SYSTEM
	bool ChangePartyType(bool bPvp, char cPvpMode = 0);
	bool Handler_EGS_CHANGE_PARTY_TYPE_REQ( const CX2PartyManager::PartyData& partyData );
	bool Handler_EGS_CHANGE_PARTY_TYPE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_PARTY_TYPE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	UidType GetMatchUid() { return m_iMatchUID; }
	void SetMatchUid(UidType iMatchUid) { m_iMatchUID = iMatchUid; }	
	void SetProcessPvpMatch(bool bVal) { m_bProcessPvpMatch = bVal; }
	bool GetProcessPvpMatch() const { return m_bProcessPvpMatch; }
	void SetProcessDungeonMatch(bool bVal) { m_bProcessDungeonMatch = bVal; }
	bool GetProcessDungeonMatch() const { return m_bProcessDungeonMatch; }

	void RemovePvpPartyMsg() 
	{ 
		SAFE_DELETE_DIALOG_HANDLE(m_pDlgPvpPartyMsg); 
		m_pDlgPvpPartyMsg = NULL;
	}

	bool Handler_EGS_REQUEST_MATCH_MAKING_REQ(char cPlayerNum, char cPvpMode);
	bool Handler_EGS_REQUEST_MATCH_MAKING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CANCEL_MATCH_MAKING_REQ();
	bool Handler_EGS_CANCEL_MATCH_MAKING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UNREG_MATCH_WAIT_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_MATCH_MAKING_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_MATCH_MAKING_SUCCESS_REPLY_NOT( bool bStartAgree );
	bool Handler_EGS_GAME_START_PVP_MATCH_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ( char cPvpType );
	bool Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_MATCH_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef SERV_NEW_EVENT_TYPES
	bool Handler_EGS_UPDATE_DUNGEON_STATUS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_NEW_EVENT_TYPES
	void SetGameStartFlag(bool bVal) { m_bStartGame = bVal; }

	bool Handler_EGS_PARTY_GAME_START_OPINION_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Send_EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT( const bool bAccept_ );
	bool Handler_EGS_PARTY_GAME_START_FAILED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void RememberComeBackInfoBeforeInstanceGameStart();		/// 던전 또는 대전 시작 전에 다시 필드로 돌아올때 필요한 정보를 기억

	void Handler_EGS_AUTO_PARTY_DUNGEON_GAME_REQ();
	bool Handler_EGS_AUTO_PARTY_DUNGEON_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	bool Handler_EGS_AUTO_PARTY_WAIT_USER_COUNT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW

	void Handler_EGS_CANCEL_AUTO_PARTY_MAKING_REQ();
	bool Handler_EGS_CANCEL_AUTO_PARTY_MAKING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Handler_EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT( const bool bAccept_ );

	bool Handler_EGS_AUTO_PARTY_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool Handler_EGS_REGROUP_PARTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Send_EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT();

protected:
	//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	void	NetworkProcess( IN const float fElapsedTime );
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

#ifdef NEW_MAIL_LOG
	void AddClosePartyLog( const string& strPacketName_, int iReason_ );
#endif // NEW_MAIL_LOG

private:
	PartyData			m_MyPartyData;

	CKTDGUIDialogType   m_pDLGPartyQuickJoin;

#ifdef SERV_PVP_NEW_SYSTEM
	bool				m_bProcessPvpMatch;
	UidType				m_iMatchUID;
	CKTDGUIDialogType	m_pDlgPvpPartyMsg;
#endif
	bool				m_bProcessDungeonMatch;

	std::map<int,KAutoPartyDungeonBonusInfo>	m_mapAutoPartyDungeonBonusInfo;
	int											m_iCurrentDungeonIDWithDifficulty;

	bool				m_bChangeMap;
	bool				m_bStartGame;
};

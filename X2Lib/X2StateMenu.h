#pragma once

class CX2StateMenu : public CX2StateCommonBG
{
	public:
		enum STATE_MENU_UI_CUSTOM_MSG
		{
			SMUCM_MENU = 100,
			SMUCM_MY_INFO,
			SMUCM_CASH_SHOP,
			SMUCM_OPTION,
			SMUCM_BACK,
			SMUCM_TUTORIAL,
			SMUCM_STATE_CHANGE_UNIT_SELECT,

			SMUCM_UNIT_ANIMATION_DOWN,
			SMUCM_UNIT_ANIMATION_UP,

			SMUCM_MY_INFO_MY_STATE,
			SMUCM_MY_INFO_INVENTORY,
			SMUCM_MY_INFO_QUEST,

			SMUCM_MY_INFO_INVENTORY_WINDOW_CHANGED,			// remove!! 

			SMUCM_CHANGE_VP,

			SMUCM_STATE_CHANGE_SERVER_SELECT,

			SMUCM_PARTY,

			SMUCM_FIND_USER,
			SMUCM_FIND_USER_OK,
			SMUCM_FIND_USER_CANCEL,

			SMUCM_USER_INFO_OK,
			SMUCM_USER_INFO_PLAY_TOGETHER,
			SMUCM_USER_INFO_REFRESH,

			SMUCM_MY_INFO_SKILL_TREE,

			SMUCM_INVITE_NOT_OK,
			SMUCM_INVITE_NOT_CANCEL,

			SMUCM_FIND_USER_SELECT,

			SMUCM_SELL_ITEM_NUM_CHANGE,
			SMUCM_SELL_ITEM_NUM_OK,
			SMUCM_SELL_ITEM_NUM_CANCEL,

			SMUCM_STATE_CHANGE_PVP_LOBBY,
			SMUCM_STATE_CHANGE_LOCAL_MAP,				// fix!! remove
			SMUCM_STATE_CHANGE_VILLAGE,
			SMUCM_STATE_CHANGE_ARCADE_LOBBY,			// fix!! remove

			SMUCM_CHANGE_SELL_MOUSE_CURSOR,
			SMUCM_CHANGE_REPAIR_MOUSE_CURSOR,

			SMUCM_COUPON_BOX,
			SMUCM_COUPON_INPUT_BOX,

			//이거 그냥 State로 옮겨야 하지 않을까?
			SMUCM_CHANGE_DECOMPOSITION_MOUSE_CURSOR,
			SMUCM_CHANGE_ENCHANT_MOUSE_CURSOR,
			SMUCM_CHANGE_SOCKET_MOUSE_CURSOR,

			SMUCM_VIEW_STUDENT_CANDIDATE_LIST,

			SMUCM_USER_INFO_FRIEND_REQ,
			SMUCM_USER_INFO_PARTY_INVITE,

			SMUCM_PRESENT_ARRIVAL_CLOSE,

			SMUCM_CHANGE_ATTRIB_ENCHANT_MOUSE_CURSOR,
			SMUCM_POSTBOX,

			SMUCM_CHANGE_EXCHANGE_ITEM_MOUSE_CURSOR,		// 아이템 보상교환 커서

//#ifdef NEW_VILLAGE_UI
			SMUCM_COMMUNITY,	
			SMUCM_INVENTORY,
			SMUCM_SYSTEM,
//#endif
			SMUCM_EXIT_SYSTEM,

			SMUCM_LEAVE_DUNGEON_LOUNGE_OK,
			SMUCM_LEAVE_DUNGEON_LOUNGE_CANCEL,
			SMUCM_LEAVE_PARTY_OK,
			SMUCM_LEAVE_PARTY_CANCEL,

			SMUCM_OPEN_ELLIOS_GUIDE,
			SMUCM_SKILL_ICON_MOUSE_OVER,
			SMUCM_SKILL_ICON_MOUSE_OUT,
			SMUCM_OPEN_ELLIOS_GUIDE_GO_TO_PAGE,

			SMUCM_PET_LIST,

			//{{ kimhc // 2011-04-23 // 디펜스던전, 월드버프UI
			SMUCM_WORLD_BUFF,
			//}} kimhc // 2011-04-23 // 디펜스던전, 월드버프UI
			SMUCM_SECURITY,
			SMUCM_PREMIUM_BUFF,		/// 프리미엄 버프(휴면 복귀, PC 방)

			SMUCM_DUNGEON_MATCH_GAME_ACCEPT,
			SMUCM_DUNGEON_MATCH_GAME_DENY,
			SMUCM_DISABLE_QUICK_SLOT_MOUSE_UP, // 비활성화 슬롯 확장
			SMUCM_DISABLE_SKILL_SLOT_MOUSE_UP,
//#ifdef REFORM_UI_MAINUI
			SMUCM_GUILD, //커뮤니티와 길드창을 분리 예정으로 버튼 먼저 분리.
//#endif //REFORM_UI_MAINUI
//#ifdef REFORM_UI_NOTICE
			SMUCM_NOTICE_CLOSE,
//#endif //REFORM_UI_NOTICE
//#ifdef REFORM_UI_PLAYGUIDE
			SMUCM_PLAYGUIDE_CLOSE,
//#endif //REFORM_UI_PLAYGUIDE
			SMUCM_OPEN_KEYPAD,
//#ifdef SERV_RECOMMEND_LIST_EVENT
			SMUCM_FRIEND_RECOMMEND_OPEN,
			SMUCM_FRIEND_RECOMMEND_OK,
			SMUCM_FRIEND_RECOMMEND_NEXT,
			SMUCM_FRIEND_RECOMMEND_CLOSE,
//#endif //SERV_RECOMMEND_LIST_EVENT
//#ifdef FIELD_BOSS_RAID  // 레이드 필드 내 ESC 처리
			SMUCM_RAID_FIELD_EXIT_OK,
			SMUCM_RAID_FIELD_EXIT_CANCEL,
//#endif // FIELD_BOSS_RAID //  레이드 필드 내 ESC 처리
//#ifdef SHOW_WEB_ADVERTISEMENT
			SMUCM_SHOW_ADVERTISEMENT,
//#endif SHOW_WEB_ADVERTISEMENT
//#ifdef SERV_ADD_WARP_BUTTON
			SMUCM_WARP_BUTTON,
			SMUCM_WARP_DEST_CLOSE,
			SMUCM_WARP_DEST_SELECT,
			SMUCM_WARP_DEST,
			SMUCM_WARP_DEST_OK,
//#endif // SERV_ADD_WARP_BUTTON
//#ifdef SERV_CHINA_SPIRIT_EVENT
			SMUCM_USE_SPIRIT_REWARD_RAGE,
			SMUCM_USE_SPIRIT_REWARD_PEACE,
//#endif SERV_CHINA_SPIRIT_EVENT
//#ifdef CLIENT_COUNTRY_US
			SMUCM_FACEBOOK_MOVE,
			SMUCM_STEAM_COMMUNITY_MOVE,
//#endif //CLIENT_COUNTRY_US
//#ifdef SERV_NEW_YEAR_EVENT_2014
			SMUCM_COMPLETE_SPECIAL_MISSION,
			SMUCM_COMPLETE_SUPPORT_MISSION,
			SMUCM_COMPLETE_NEW_YEAR_MISSION,
			SMUCM_COMPLETE_NEW_YEAR_MISSION_OK,
			SMUCM_COMPLETE_NEW_YEAR_MISSION_CANCEL,
			SMUCM_NEW_YEAR_2014_UI_TOGGLE,
			SMUCM_NEW_YEAR_2014_UI_CLOSE,
//#endif //SERV_NEW_YEAR_EVENT_2014
//#ifdef SERV_CHANNELING_AERIA
			SMUCM_SHOW_AERIA_USER_SUPPORT,
//#endif //SERV_CHANNELING_AERIA
//#ifdef SERV_ELESIS_UPDATE_EVENT
			SMUCM_VIEW_NOTE,
//#endif SERV_ELESIS_UPDATE_EVENT
//#ifdef SERV_EVENT_CHECK_POWER
			SMUCM_TOGGLE_CHECK_POWER_GUIDE_PAGE,
			SMUCM_TOGGLE_CHECK_POWER_RESULT_PAGE,
			SMUCM_START_CHECK_POWER,
//#endif SERV_EVENT_CHECK_POWER
//#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
			SMUCM_USE_CHUNG_GIVE_ITEM_ONE,
			SMUCM_USE_CHUNG_GIVE_ITEM_TWO,
			SMUCM_USE_CHUNG_GIVE_ITEM_TREE,
//#endif SERV_EVENT_CHUNG_GIVE_ITEM
//#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
			SMUCM_USE_COBO_EVET_BUTTON,
//#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
//#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
			SMUCM_USE_ADAMS_EVET_BUTTON = 203,
//#endif ALWAYS_EVENT_ADAMS_UI_SHOP
//#ifdef SERV_4TH_ANNIVERSARY_EVENT
			SMUCM_TOGGLE_EVENT = 204,
			SMUCM_4TH_EVENT_BUTTON,
			SMUCM_4TH_EVENT_OK,
			SMUCM_4TH_EVENT_CANCEL,
//#endif SERV_4TH_ANNIVERSARY_EVENT
		};

		
		struct PresentArrival
		{
#ifdef SERV_GLOBAL_BILLING
			KBillProductInfo	m_kBillProductInfo;
#else
			KNXBTProductInfo	m_KNXBTProductInfo;
#endif SERV_GLOBAL_BILLING
			wstring				m_SenderNickName;
			wstring				m_PresentMsg;

			PresentArrival& operator=(const PresentArrival& rhs) 
			{ 
				if ( this == &rhs ) 
				{ 
					return *this; 
				}

#ifdef SERV_GLOBAL_BILLING
				m_kBillProductInfo		= rhs.m_kBillProductInfo;
#else
				m_KNXBTProductInfo		= rhs.m_KNXBTProductInfo;
#endif SERV_GLOBAL_BILLING
				m_SenderNickName		= rhs.m_SenderNickName;
				m_PresentMsg			= rhs.m_PresentMsg;

				return *this; 
			} 
		};

	public:
		CX2StateMenu(void);
		virtual ~CX2StateMenu(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		wstring GetWstrFindUserName() const { return m_wstrFindUserName; }
		void SetWstrFindUserName(wstring val) { m_wstrFindUserName = val; }

		virtual void OnChangedInventory() {};
		virtual void OnResetStateAndEquip();

		virtual bool Handler_EGS_SEARCH_UNIT_REQ( const WCHAR* wszNickName );
		virtual bool Handler_EGS_SEARCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool Handler_EGS_GET_CONNECTION_UNIT_INFO_REQ( const WCHAR* wszNickName );
		virtual bool Handler_EGS_GET_CONNECTION_UNIT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef SERV_ADVERTISEMENT_EVENT
		virtual bool Handler_EGS_GET_ADVERTISEMENT_EVENT_REQ();
		virtual bool Handler_EGS_GET_ADVERTISEMENT_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_ADVERTISEMENT_EVENT

		CX2Inventory::SORT_TYPE GetInventorySortType( const WCHAR* pRadioButtonName );




		bool ToggleSystemMenu(bool bShow = true);		
		void SetShowMenu(bool val) { m_bShowMenu = val; }
		bool GetShowSystemMenu() { return m_bShowSystemMenu; }
		bool GetShowMsgBox();
		virtual void ToggleCashShop();


		//{{ kimhc // 2011-04-22 // 디펜스던전 월드 버프 UI
#ifdef	SERV_INSERT_GLOBAL_SERVER
		void AddStringRemainTime( OUT wstringstream& wstreamDesc_, IN const int iRemainTimeWorldBuff_ );
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-22 // 디펜스던전 월드 버프 UI

		//{{ JHKang / 강정훈 / 2011.6.26 / 프리미엄 버프 UI
#ifdef COME_BACK_REWARD
		void UpdateGuideDescPremiumBuffUI();
#endif
		//}}

		//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
		inline void SetShowTooniLandEvent( const bool bShow_ );
		inline void UpdateGuideDescTooniLandEvent();
#endif	SERV_TOONILAND_CHANNELING_EVENT
		//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

#ifdef GRACE_OF_EL
		FORCEINLINE void SetShowGraceOfEl( IN const bool bShow_ );
		FORCEINLINE void UpdateGuideDescGraceOfEl();
#endif

#ifdef SERV_COEXISTENCE_FESTIVAL_ROOMBUFF
		void SetShowFestivalRoomBuff( bool bVal );
#endif //SERV_COEXISTENCE_FESTIVAL_ROOMBUFF

#ifdef SERV_EVENT_RETURN_USER_MARK
		void OpenElliosGuide( int iTargetPage );
#endif //RASING_HERO_EVENT

#ifdef BUFF_TEMPLET_SYSTEM
		void UpdateGuideDescBuffTemplet( CX2PremiumBuffIconPtr pPremiumBuffPtr );
		int  ConvertTimeForBuffTemplet( wstring wstrEndTime );
		void SetShowBuffTempletIcon( bool bVal );
#endif BUFF_TEMPLET_SYSTEM

		void ShowMainMenuButton();
		void UpdateMenuPosistion();	//우측하단 메뉴 위치 갱신
		void UpdateMenuString();
		inline void SetGuideString( CKTDGUIButton* pButton_, const WCHAR* pGuideDesc_ )
		{
			if( NULL != pButton_ )
			{
				pButton_->SetGuideDesc(pGuideDesc_);
			}
		}
#ifdef MODIFY_DUNGEON_STAGING
		bool GetShowUI() const { return m_bShowUI; }
		void SetShowUI(bool bVal){m_bShowUI = bVal;}
#endif //MODIFY_DUNGEON_STAGING
	
#ifdef MODIFY_ACCEPT_QUEST
		void SetShowNewQuest(bool bVal);
#endif // MODIFY_ACCEPT_QUEST

#ifdef SERV_ADD_WARP_BUTTON
		bool GetShowWarpDest();
		void SetCurrentVillageWarpIndex( int iVal_ ) { m_iCurrentVillageWarpIndex = iVal_; }
		void ClickWarpButton( bool bIsEdConsumption_ );
		void SetShowWarpDest(bool val);
#endif // SERV_ADD_WARP_BUTTON

	protected:

		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		
#ifndef NEW_MESSENGER
		virtual bool Handler_EGS_KNM_TOGETHER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_KNM_INVITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_KNM_INVITE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif NEW_MESSENGER

#ifdef SERV_NAVER_CHANNELING
		bool		Handler_EGS_GET_NAVER_ACCESS_TOKEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_NAVER_CHANNELING

#ifdef SERV_GLOBAL_BILLING
		virtual bool Handler_EGS_BILL_PRODUCT_INFO_REQ();
		virtual bool Handler_EGS_BILL_PRODUCT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#else // SERV_GLOBAL_BILLING
		virtual bool Handler_EGS_CASH_PRODUCT_INFO_REQ();
		virtual bool Handler_EGS_CASH_PRODUCT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		
#endif // SERV_GLOBAL_BILLING

		bool Handler_EGS_CASH_PRODUCT_INFO_CHANGED_NOT();

				//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
		bool Handler_EGS_CLEAR_TEMP_INVENTORY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	REAL_TIME_ELSWORD
				//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

		bool Handler_EGS_ADD_ON_STAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_ADD_ON_STAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		
		bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Hander_EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_ADD_WARP_BUTTON
		bool Handler_EGS_WARP_BY_BUTTON_REQ( int iWarpDest_ );
		bool Handler_EGS_WARP_BY_BUTTON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 

		bool Handler_EGS_UPDATE_WARP_VIP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 

		void UseWarpPopup(bool bIsEdConsumption_);
		void InitWarpList();
		void AddWarpList(CKTDGUIContolList *pControlList, const int row, const int iVillageId);
		void CreateWarpDest();

		wstring GetExpirationDateDesc( __int64 trTime_ );
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_CHINA_SPIRIT_EVENT
		bool Handler_EGS_USE_SPIRIT_REWARD_REQ();
		bool Handler_EGS_USE_SPIRIT_REWARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		bool Handler_EGS_EVENT_COBO_DUNGEON_FIELD_REQ( bool bTemp );
		bool Handler_EGS_EVENT_COBO_DUNGEON_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		void ShowCoboEventUI(float TempTime);
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_NEW_YEAR_EVENT_2014
		bool Handler_EGS_2013_EVENT_MISSION_COMPLETE_REQ( IN bool bLevelUpEvent );
		bool Handler_EGS_2014_EVENT_MISSION_COMPLETE_REQ();
		bool Handler_EGS_2013_EVENT_MISSION_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
		bool Handler_EGS_2014_EVENT_MISSION_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		void SetUsedCashShop(bool bUsed){ m_UsedCashShop = bUsed;}
		bool GetUsedCashShop(void){ return m_UsedCashShop; }
#endif //ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef SERV_CONTENT_MANAGER_INT
		bool Handler_EGS_CASH_SHOP_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
#endif //SERV_CONTENT_MANAGER_INT

#ifdef SERV_EVENT_CHECK_POWER
		bool Handler_EGS_START_CHECK_POWER_REQ( bool bStart );
		bool Handler_EGS_START_CHECK_POWER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_UPDATE_CHECK_POWER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHECK_POWER_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		bool Handler_EGS_EVENT_CHUNG_GIVE_ITEM_REQ( int iChoice );
		bool Handler_EGS_EVENT_CHUNG_GIVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		void ShowChungGiveItem_UI(void);
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		void SetShowAdamsUI(void);
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		bool Handler_EGS_4TH_ANNIV_EVENT_REWARD_REQ( IN int iSelectedIndex );
		bool Handler_EGS_4TH_ANNIV_EVENT_REWARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_4TH_ANNIVERSARY_EVENT

		virtual bool Handler_EGS_CHAR_LEVEL_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_DECREASE_ENDURANCE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool ProcessStateChangeUnitSelectWithOptionWindow();
		virtual bool ProcessStateChangeServerSelectWithOptionWindow();

		bool HandleMsgByESCImp();

		//virtual bool ClickMenuBackButton() = 0;
		virtual bool ToggleLobbyMenuWindow();

		virtual void SetShowStateDLG( bool bShow ) = 0;

				void UpdateUserInfoUI( KEGS_SEARCH_UNIT_ACK &kEvent );

		virtual bool InitUI();
		virtual bool InitETC();

				void PopUpMenuClose( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

				void OpenFindUserDLG();
	
				
				void SetShowDlgMenu(bool val);
		virtual void SetShowMenu();
		virtual bool ShortCutKeyProcess();
		virtual void ChatProcess();

		virtual void RefreshMenuInfo();
				void UpdateExpGageBar();
	
		virtual void ChangeByToggleCashShop();		

				void CheckPresentArrival();			

				// 09.03.24 김태완
				void CheckMyInfoChange();


				void OnClickedMenuSkillButton();

		void DisplayLevelUpEffect( CX2GUUser* pGUUser_ );

		bool CameraZoomInAndOutKeyProcess();

#ifdef SERV_RECOMMEND_LIST_EVENT
		void OpenRecommendMsgBox( bool _bOpen );
		bool Handler_EGS_RECOMMEND_USER_REQ( const WCHAR* wszNickName, bool bCancel = false );
		bool Handler_EGS_RECOMMEND_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_RECOMMEND_LIST_EVENT

	protected:
#ifdef SERV_NEW_YEAR_EVENT_2014
		CKTDGUIDialogType			m_pDLG2014Confirm;
#endif SERV_NEW_YEAR_EVENT_2014

		CKTDGUIDialogType			m_pDLGMenu;

		CKTDGUIDialogType			m_pDLGMenuWindow;
		CKTDGUIDialogType			m_pDLGFindUser;
		CKTDGUIDialogType			m_pDLGUserInfo;

		CKTDGUIDialogType			m_pDLGF1Help;


		bool					m_bShowWindowMenu;
		bool					m_bShowSystemMenu;

		CX2CashShop*			m_pCashShop;
		
		wstring					m_wstrFindUserName;
		UidType					m_FindUserUid;

		CKTDGUIDialogType			m_pDLGInviteNot;
		wstring					m_InviterName;

		
		// 유저 찾기
		CX2Unit::CONNECT_UNIT_STATE m_eSearchUnitConnectionState;
		int m_iSearchUnitChannelID;
		int m_iSearchUnitDungeonID;
		UidType m_iSearchUnitRoomUID;

		


		float					m_fTimeRefreshMenuInfo;

		int						m_CashItemPage;
		CKTDGUIDialogType			m_pDLGCashItemUpdate;

		CKTDGUIDialogType			m_pDLGPresentArrival;


		CKTDGUIDialogType			m_pDLGSystem;
		
		bool					m_bShowMenu;


		CKTDGUIDialogType			m_pDLGLeavePartyOrLounge;

		CKTDGUIDialogType			m_pDLGGuideBookButton;
		CX2Book					m_Book;

		//{{ 09.03.24 김태완 : 우하단 돈/VP 변화 정보 표시
		CKTDGPicChar*			m_pPicCharMenuPlusInfo;
		CKTDGPicChar*			m_pPicCharMenuPlusInfoRed;
		int						m_TotalExp;
		int						m_TotalED;		
		int						m_TotalLevel;
#ifdef SERV_PVP_NEW_SYSTEM
		int						m_TotalAP;
#else
		int						m_TotalVP;
#endif
		//}}
#ifdef SERV_EPAY_SYSTEM
		boost::timer                m_EpayTimerCheck;
#endif //SERV_EPAY_SYSTEM
		float					m_fMenuMaxRemainTime; //메인메뉴가 유지 될 수 있는 최대 시간
		bool					m_bShowMainMenuButton;  //마우스 동작 없을 때 메인메뉴 안보이도록 하기 위한 변수
		vector<CKTDGUIButton*>  m_vecpButtonMainMenu; 
#ifdef MODIFY_DUNGEON_STAGING
		bool					m_bShowUI;
#endif //MODIFY_DUNGEON_STAGING

#ifdef SERV_RECOMMEND_LIST_EVENT
		CKTDGUIDialogType		m_pDLGFriendRecommendPopUp;
#endif //SERV_RECOMMEND_LIST_EVENT

#ifdef SERV_ADD_WARP_BUTTON
		CKTDGUIDialogType			m_pDLGWarpDestination;
		std::vector<CKTDGUIButton*> m_vecWarpListButton;
		int							m_iSelectedWarpDest;
		SHORT						m_SumDelta;
		int							m_iSelectedWarpDestIndex;//현재 위치와 거리를 알기 위해 인덱스 저장
		int							m_iCurrentVillageWarpIndex; //현재 위치 인덱스로 저장
		bool						m_bIsEdConsumptionWarp;		//워프아이템 ED소모여부
		float						m_fTimeCheckWarpVip;	// Vip 시간 체크는 1분 단위로 합니다.
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		CKTDGUIDialogType			m_pDLGChungItemUI;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		CKTDGUIDialogType			m_pDLGCoboEventUI;
		CKTDGUIDialogType			m_pDLGCoboEventCountUI;
		int							m_iElapsedTimeStage; //스타트 버튼을 누르고 흐른 시간
		CTime						m_tRemaindTimeDisCount;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		CKTDGUIDialogType			m_pDLGAdamsEventShopUI;
		bool						m_UsedCashShop;
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		CKTDGUIDialogType			m_pDLG4thConfirm;
		int							m_iSelectedButtonIndex;
#endif //SERV_4TH_ANNIVERSARY_EVENT
};





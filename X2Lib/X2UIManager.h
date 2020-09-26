#ifdef NEW_VILLAGE_UI

#pragma once

class CX2UIManager
{
public:
// Comment : 08.12.12 김태완
// 	UIManager에 의해 관리되는 UI들의 제작시 제약 사항
//	void UIClosed(UI_MENU menuInx);
// 	void UIOpened(UI_MENU menuInx);
// 	실제로 UI가 열리고 닫히는 시점에 UIOpened 또는 UIClosed를 호출해 줘야 합니다.
// 	UI 레이어 설정 함수가 있어야 합니다. ( SortUIByIndex 내부에서 호출하게 연결 )
// 	왼쪽 위에 위치해서 위치가 조절되어야 하는 UI들은 위치 조절 함수가 구현되어야 합니다. ( SetDLGPosition에서 호출 )
// 
// 	UI 추가시 주의해야 하는 점
// 	-새 UI를 enum CX2UIManager::UI_MENU에 추가 (순서 조심할 것)
// 	- UIManager의 msgproc, uicustomeventproc, uiservereventproc에 추가
// 	- ToggleUI, SortUIbyIndex, GetShow에 추가
// 	-  왼쪽 위에 위치해서 우선순위에 따라 위치가 조절되어야 하는 UI들은
// 	- GetDLGSize, SetDLGPosition에 추가
// 내가 왜 그냥 UI 클래스 하나 만들지 않았을까 나는 한없이 어리석었도다 ...


	// enum의 순서를 메뉴 우선순위 순으로 정렬
    enum UI_MENU
    {
		UI_MENU_HIGHEST = 0,
		
		UI_MENU_QUICK_SLOT,
#ifdef REFORM_UI_SKILLSLOT
		UI_SKILL_SLOT,
#endif //REFORM_UI_SKILLSLOT
		
		UI_MENU_UPPER,

		UI_MENU_PERSONAL_TRADE,
		//UI_MENU_PARTY_LIST,
		//UI_MENU_LOCAL_MAP,
		UI_MENU_QUEST_RECEIVE,
		UI_MENU_SHOP,
		UI_MENU_MANUFACTURE,
#ifdef POSTBOX_NOTICE

		UI_MENU_POSTBOX_NOTICE,
#endif POSTBOX_NOTICE

#ifdef DEF_TRADE_BOARD
		// oasis907 : 김상윤 [2010.3.12] //  개인 상점 검색 시스템
		UI_MENU_PERSONAL_SHOP_BOARD,
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD
		// oasis907 : 김상윤 [2009.11.24] // 길드 게시판
		UI_MENU_GUILD_BOARD,
#endif GUILD_BOARD

		//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
		UI_MENU_RANKING_INFO,
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		UI_MENU_ITEM_EXCHANGE_SHOP,
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		UI_MENU_PRIVATE_BANK,
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행

		UI_MENU_PERSONAL_SHOP,
        UI_MENU_CHARINFO,
#ifdef SERV_PET_SYSTEM
		UI_MENU_PET_LIST,
#endif
		UI_MENU_SKILL,
		UI_MENU_COMBO_TREE,
#ifdef SERV_EPIC_QUEST
		UI_MENU_QUEST_NEW,
#endif SERV_EPIC_QUEST
        UI_MENU_QUEST,
		UI_MENU_WATCH,

		UI_MENU_LU,	// 왼쪽 위에 떠야 하는 하위창들(파티, 캐릭터..)등은 이것보다 위로 그 외는 아래로. 
					// UI_MENU_UPPER ~ UI_MENU_LU 사이에 있는 UI들은 열고 닫힐 때 위치가 정렬된다.
					// UI_MENU_LU 아래쪽에 있는 UI들은 위치정렬은 되지 않는다. (상하관계 정렬은 된다)

		//{{ kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD

		UI_TEMP_INVENTORY,		// enum.lua에는 없음(DLG_Item_Box.lua)
								// MENU라 하기에 애매해서 MENU 단어를 앞에 붙이지 않았음
#endif	REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

#ifdef SERV_SKILL_NOTE
		UI_SKILL_NOTE,
#endif

		UI_MENU_INVEN,
        UI_MENU_COMMUNITY,
        UI_MENU_OPTION,

		UI_MENU_LOWEST,

		UI_MENU_PARTY,
		UI_MENU_GUILD,	//길드창과 커뮤니티창을 분리할 예정이므로 버튼 먼저 분리

		//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
		UI_MENU_RESOLVE,
#endif SERV_MULTI_RESOLVE
		//}}
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		UI_MENU_SYNTHESIS,
#endif SERV_SYNTHESIS_AVATAR
		//}}

#ifdef ADDED_RELATIONSHIP_SYSTEM
		UI_MENU_RELATION,
#endif // ADDED_RELATIONSHIP_SYSTEM


#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
		UI_MENU_HERO,
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		UI_MENU_WEDDING_EVENT_LETTER,
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef RIDING_SYSTEM
		UI_RIDING_SKILL_SLOT,
#endif //RIDING_SYSTEM

        UI_MENU_END,
    };

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	enum UI_TIME_CONTROL_ITME_TYPE
	{
		UTCIT_SHOP					= 0,
		UTCIT_MANUFACTURE			= 1,
		UTCIT_EXCHANGE_SHOP			= 2,
	};
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

public:
    CX2UIManager();
	void ResetUIManager();
    ~CX2UIManager();

    virtual bool    UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual bool    UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
    virtual HRESULT OnFrameRender();
	
	void OnStateChange();
	void OnFieldChange();
    
	bool IsOpenedWindow();
	bool GetShow(UI_MENU menuInx);
	bool ToggleUI(CX2UIManager::UI_MENU menuInx, UidType arg = NULL);
    bool ToggleUI(CX2UIManager::UI_MENU menuInx, bool forOpen, UidType arg = NULL);
	void CloseAll();
	void CloseAllLUDlg();		// LU : 왼쪽 위
	void CloseAllNPCDlg();
	//bool CloseByEsc();			// 위에서부터 하나씩 닫는다.  창이 하나라도 닫혔으면 true, 더 닫힐 창이 없는 상태면 false
	bool GetShowNpcShop();

	bool GetDLGFlag(){return m_bFlag;}
	void SetDLGFlag(bool val){m_bFlag = val;}
	
	
	//bool IsStateMenu();
		
	// 특정 UI가 열렸다/닫혔다를 신호해 줘서 위치관계를 재정렬하게 한다.
	// UIManager를 거치지 않고 열고 닫는 경우(REQ/ACK 등 단순히 ToggleUI->SetShow path를 사용하지 못하는 경우) 에 활용하자.
	void UIClosed(UI_MENU menuInx);
	void UIOpened(UI_MENU menuInx);

	// Toggle이 동작하지 않게 해 주는 Method!
	bool ToggleEnableCondition(UI_MENU menuInx, bool forOpen);
	
	// UI 정렬을 위한 내부 method
	void SetUILayerIndex(UI_MENU menuInx, bool bAdd /* true면 추가, false면 삭제 */ );
    void SortUIbyLayerIndex();

	// 특정상황에서 특정 UI를 diasble 시킨다 : 파티창 작동 안하게 하려고
	void DisableDialogOnCondition();


	void CreateUICharInfo();
	CX2UICharInfo* GetUICharInfo();
	
	void CreateUIInventory();
	CX2UIInventory* GetUIInventory();
	
	void CreateUIPersonalShop();
	CX2UIPersonalShop* GetUIPersonalShop();

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	void DestroyPeronalShopUI();
#endif //SERV_UPGRADE_TRADE_SYSTEM
	
	void CreateUIPersonalTrade();
	CX2UIPersonalTrade* GetUIPersonalTrade();
	
	void CreateUIShop();
	CX2UIShop* GetUIShop();
	
	void CreateUIManufacture();
	CX2UIManufacture* GetUIManufacture();
	
	void CreateUIQuickSlot();
	CX2UIQuickSlot* GetUIQuickSlot();
	
#ifdef SERV_EPIC_QUEST
	void CreateUIQuestNew();
	CX2UIQuestNew* GetUIQuestNew();
#else
	void CreateUIQuest();
	CX2UIQuest* GetUIQuest();
	
	void CreateUIQuestReceive();
	CX2UIQuestReceive* GetUIQuestReceive();
#endif SERV_EPIC_QUEST		
	void CreateUIWatch();
	CX2UIWatch*	GetUIWatch();	

#ifdef SERV_PET_SYSTEM
	void CreatePetList();
	CX2UIPetInfo* GetUIPetList();
#endif

#ifdef NEW_SKILL_TREE_UI
	void CreateUISkillTree();

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		CX2UISkillTreeNew* GetUISkillTree();
	#else // UPGRADE_SKILL_SYSTEM_2013
		CX2UISkillTree* GetUISkillTree();
	#endif // UPGRADE_SKILL_SYSTEM_2013
	
#endif

#ifdef COMBO_TREE_UI_TEST
	void CreateUIComboTree();
	CX2UIComboTree* GetUIComboTree();
#endif COMBO_TREE_UI_TEST
	
#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
	void CreateUIHeroMatch();
	CX2UIHeroMatch* GetUIHeroMatch();
#endif //SERV_HERO_PVP_MANAGE_LIST

	//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	void CreateUITempInventory();
	CX2UITempInventory* GetUITempInventory() const;
	void DestoryUITempInventory();
#endif	REAL_TIME_ELSWORD
	//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리


#ifdef POSTBOX_NOTICE
	// oasis907 : 김상윤 [2010.3.12] // 개인 거래 상점 검색 시스템
	void CreateUIPostBoxWarning();
	CKTDGUIDialog* GetUIPostBoxWarning() const;
#endif POSTBOX_NOTICE

#ifdef DEF_TRADE_BOARD
	// oasis907 : 김상윤 [2010.3.12] // 개인 거래 상점 검색 시스템
	void CreateUIPersonalShopBoard();
	CX2UIPersonalShopBoard* GetUIPersonalShopBoard() const;
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD
	// oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	void CreateUIGuildBoard();
	CX2UIGuildBoard* GetUIGuildBoard() const;
#endif GUILD_BOARD

	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	void CreateUIRankingInfo();
	CX2UIRankingInfo* GetUIRankingInfo() const;
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	void						CreateUIItemExchangeShop();
	CX2UIItemExchangeShop*		GetUIItemExchangeShop() const;
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	void						CreateUIPrivateBank();
	CX2UIPrivateBank*			GetUIPrivateBank() const;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행

#ifdef SERV_SKILL_NOTE
	void						CreateUISkillNote();
	CX2UISkillNote*				GetUISkillNote() const;
#endif
	
	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	void						CreateUIResolveItem();
	CX2UIResolveItem*				GetUIResolveItem() const;
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	void						CreateUISynthesisItem();
	CX2UISynthesisItem*				GetUISynthesisItem() const;
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef SERV_PET_SYSTEM
	bool PetInventoryOpenCondition();
	bool PetInventoryCloseCondition(UI_MENU menuInx);
#ifdef ADD_PET_INVENTORY_BUTTON
	bool PetInventoryForceOpenCondition();
	void PetInventoryForceCloseOtherDlg();
#endif ADD_PET_INVENTORY_BUTTON
#endif SERV_PET_SYSTEM

	void SetShowUI(bool bVal, bool bForce = false);//, bool AllowReset = true);
	bool GetShowUI() const {return m_bShowUI;}	

	void Handler_EGS_WATCH_UNIT_REQ(UidType uid);
	bool Handler_EGS_WATCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	bool Handler_EGS_BUY_UI_SETTING_REQ(int iHouseID, int iTimeControlItemType);
	bool Handler_EGS_BUY_UI_SETTING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_GET_TIME_CONTROL_ITME_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	std::set<int> GetBanBuyItemList() { return m_setBanBuyItem; }
	std::set<int> GetBanManufactureItemList() { return m_setBanManufactureItem; }
	std::set<int> GetBanExchangeItemList() { return m_setBanExchangeItem; }
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_MANUFACTURE_PERIOD_FIX
	bool Handler_EGS_MANUFACTURE_PERIOD_SETTING_REQ( int iHouseID );
	bool Handler_EGS_MANUFACTURE_PERIOD_SETTING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_MANUFACTURE_PERIOD_FIX


#ifdef OTHER_SERVER_GROUP_WATCH
	void OpenWatchUIByOtherServerGroupUnitUid(UidType UnitUid_);
	bool IsSameServerGroupByUnitUid( UidType UnitUid_ );
#endif //OTHER_SERVER_GROUP_WATCH

#ifdef REFORM_UI_SKILLSLOT
	//스킬슬롯, 퀵슬롯, 메뉴버튼을 각각 따로 관리하기 위해 추가
	void SetShowSkillSlot(bool val);
	bool GetShowSkillslot() const { return m_bShowSkillSlot; }

	void SetShowQuickSlot(bool val) { m_bShowQuickSlot = val;}
	bool GetShowQuickSlot() const { return m_bShowQuickSlot; }
#endif //REFORM_UI_SKILLSLOT

#ifdef RIDING_SYSTEM
	void SetShowRidingPetSkillSlot(bool val);
	bool GetShowRidingPetSkillslot() const { return m_bShowRidingPetSkillSlot; }
#endif //RIDING_SYSTEM

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	bool CreateWeddingEventLetter();
	void CreateWeddingXmesh( int Select_);
	void ClearWeaddingXmesh();
	void CreateWeddingCaution();
	void SetShowLetterGuide( const bool _show );

	void Handler_EGS_SEND_LOVE_LETTER_EVENT_REQ();
	bool Handler_EGS_SEND_LOVE_LETTER_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	CX2UIPersonalShop* GetPersonalShop() { return m_pPersonalShop; }
#endif //SERV_UPGRADE_TRADE_SYSTEM
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	bool GetbShowAdamsShop(void)
	{
		return m_bShowAdamsShop;
	}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
private:
	//void SetPosLUDialog(UI_MENU openDLG, bool forOpen);
	void SetPosLUDialogOnOpen(UI_MENU openDLG);
	void SetPosLUDialogOnClose(UI_MENU closeDLG);
	D3DXVECTOR2 GetDLGSize(UI_MENU DLG);
	void SetDLGPosition(UI_MENU DLG, D3DXVECTOR2 vec);
	void SetNowDragItemReturn();

	void SetShowMinimapUI(bool bVal);
	void SetShowGageUI(bool bVal);
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	public :
	void SetShowQucikQuest(bool bVal);
	private:
#else
	void SetShowQucikQuest(bool bVal);
#endif 
	void SetShowPlayGuide(bool bVal);
	void SetShowFeverUI(bool bVal);
	void SetShowCinematicUI(bool bVal, float fTime_ = 0.0f );
	void SetShowResurrectionStone(bool bVal);
	void SetShowMenu(bool bVal);

#ifdef ADDED_RELATIONSHIP_SYSTEM
	public :
		void SetShowSkillDesc(bool bVal);

	private :
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	public :
	void SetShowPartyMenu(bool bVal);
	private:
#else
	void SetShowPartyMenu(bool bVal);
#endif 
	void SetShowMonsterIndicator(bool bVal);
	bool					m_bFlag;

	CX2UICharInfo*			m_pCharInfo;
	CX2UIInventory*			m_pInventory;
	CX2UIWatch*				m_pWatch;
	CX2UIPersonalShop*		m_pPersonalShop;
	CX2UIPersonalTrade*		m_pPersonalTrade;
	CX2UIShop*				m_pShop;
	CX2UIManufacture*		m_pManufacture;
	CX2UIQuickSlot*			m_pQuickSlot;

#ifdef SERV_EPIC_QUEST
	CX2UIQuestNew*			m_pQuestNew;
#else
	CX2UIQuestReceive*		m_pQuestReceive;
	CX2UIQuest*				m_pQuest;
#endif SERV_EPIC_QUEST

#ifdef SERV_PET_SYSTEM
	CX2UIPetInfo*			m_pPetList;
#endif

#ifdef NEW_SKILL_TREE_UI

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		CX2UISkillTreeNew*		m_pSkillTree;
	#else // UPGRADE_SKILL_SYSTEM_2013
		CX2UISkillTree*			m_pSkillTree;
	#endif // UPGRADE_SKILL_SYSTEM_2013
	
#endif

#ifdef COMBO_TREE_UI_TEST
	CX2UIComboTree*			m_pComboTreeUI;
#endif COMBO_TREE_UI_TEST

	//{{ kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 
#ifdef	REAL_TIME_ELSWORD
	CX2UITempInventory*		m_pTempInventory;	// 던전 내에서 인벤토리가 가득 찼을 경우 생기는 임시 공간
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 

#ifdef POSTBOX_NOTICE
	CKTDGUIDialog* m_pPostBoxWarning;
#endif POSTBOX_NOTICE
#ifdef DEF_TRADE_BOARD
	CX2UIPersonalShopBoard* m_pPersonalShopBoard;
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD
	// oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	CX2UIGuildBoard*		m_pGuildBoard;	
#endif GUILD_BOARD

	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	CX2UIRankingInfo*		m_pRankingInfo;		// 게시판, BILLBOARD(헤니르 시공, 대전, 레벨 랭킹)
#endif	RANKING_INFO_UI
	//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	CX2UIItemExchangeShop*	m_pItemExchangeShop;	// 아이템 교환 (헤니르 시공 업데이트)
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	CX2UIPrivateBank*		m_pPrivateBank;			// 개인 은행
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행

#ifdef SERV_SKILL_NOTE
	CX2UISkillNote*			m_pSkillNote;
#endif

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	CX2UIResolveItem*		m_pResolveItem;
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	CX2UISynthesisItem*		m_pSynthesisItem;
#endif SERV_SYNTHESIS_AVATAR
	//}}

#ifdef SERV_HERO_PVP_MANAGE_LIST
	CX2UIHeroMatch*			m_pHeroMatch;
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	float					m_fWeddingClose;
	CKTDGUIDialogType		m_pDlgWeddingEventLetter;
	CKTDGUIDialogType		m_pDlgWeddingCaution;
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshHandleWedding;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

	CX2UIManager::UI_MENU				m_LUMenu1st;
	CX2UIManager::UI_MENU				m_LUMenu2nd;

	vector<CX2UIManager::UI_MENU>		m_vLayer;	
	bool								m_bShowUI;

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	std::set<int>				m_setBanBuyItem;
	std::set<int>				m_setBanManufactureItem;
	std::set<int>				m_setBanExchangeItem;
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef REFORM_UI_SKILLSLOT
	//스킬슬롯, 퀵슬롯, 메뉴버튼을 각각 따로 관리하기 위해 추가
	bool								m_bShowSkillSlot;
	bool								m_bShowQuickSlot;
#endif //REFORM_UI_SKILLSLOT

#ifdef RIDING_SYSTEM
	bool								m_bShowRidingPetSkillSlot;
#endif //RIDING_SYSTEM

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	bool								m_bShowAdamsShop;
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
};

#endif

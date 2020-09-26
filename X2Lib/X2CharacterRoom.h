#pragma once

//class CX2StateMenu;
//class CX2CharacterRoom : public CX2ItemSlotManager
//{
//	public:
//
//		enum CHARACTER_ROOM_MSG
//		{
//			CRM_QUICK_SELL_ITEM = 1000,
//			CRM_REPAIR_ITEM,
//			CRM_OPEN_RANDOM_ITEM,
//
//			CRM_SKILL_SLOT_UNEQUIP_1_OVER,
//			CRM_SKILL_SLOT_UNEQUIP_2_OVER,
//			CRM_SKILL_SLOT_UNEQUIP_3_OVER,
//
//			CRM_SKILL_SLOT_UNEQUIP_4_OVER,
//			CRM_SKILL_SLOT_UNEQUIP_5_OVER,
//			CRM_SKILL_SLOT_UNEQUIP_6_OVER,
//
//			CRM_SKILL_SLOT_UNEQUIP_OUT,
//
//			CRM_ALL_EQUIPPING_ITEM_REPAIR,
//			CRM_ALL_EQUIPPING_ITEM_REPAIR_OK,
//			CRM_ALL_EQUIPPING_ITEM_REPAIR_CANCEL,
//
//			CRM_QUICK_SELL_ITEM_CANCEL,
//
//			CRM_RESOLVE_ITEM_OK,
//			CRM_RESOLVE_ITEM_CANCEL,
//
//			CRM_SKILL_JOB_LEVEL_1,
//			CRM_SKILL_JOB_LEVEL_2,
//
//			CRM_ENCHANT_ITEM_OK,
//			CRM_SPECIAL_ENCHANT_ITEM_OK,
//			CRM_ENCHANT_ITEM_CANCEL,
//
//			CRM_RESOLVE_ITEM_RESULT_OK,
//			
//			CRM_ENCHANT_ITEM_RESULT_OK,
//
//			CRM_RESOLVE_ITEM_RESULT_PREV_PAGE,
//			CRM_RESOLVE_ITEM_RESULT_NEXT_PAGE,
//
//			CRM_RESALE_ITEM_NUM_CHECK,
//			CRM_RESALE_ITEM_NUM_UNCHECK,
//
//			CRM_SOCKET_ITEM_CLOSE,
//			CRM_SOCKET_ITEM_PUSH_NORMAL_MAGIC_STONE,
//			CRM_SOCKET_ITEM_PUSH_RARE_MAGIC_STONE,
//			CRM_SOCKET_ITEM_REMOVE_MAGIC_STONE,
//
//			CRM_SOCKET_ITEM_FEE_CHECK_OK,
//
//			CRM_CLOSE_CUBE_RESULT,
//
//			CRM_INVEN_PAGE_PREV,
//			CRM_INVEN_PAGE_NEXT,
//
//			CRM_ATTRIB_ENCHANT_ITEM_CANCEL,
//			CRM_ATTRIB_ENCHANT_ITEM_OK,
//			CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT,
//
//			CRM_EL_CHANGER_OK,
//			CRM_EL_CHANGER_CANCEL,
//			CRM_EL_CHANGER_CHANGE_ITEM_COUNT,
//
//            CRM_CANCELLATION_TITLE,
//            CRM_LIST_TITLE,
//
//			CRM_ATTRIB_ENCHANT_ITEM_OK_REP,
//			CRM_ATTRIB_ENCHANT_ITEM_REMOVE_SLOT_REP,
//
//			CRM_USE_ENCHANT_ADJUVANT,					// 강화 보조제 체크박스
//			CRM_EXCHANGE_OLD_ITEM_OK,					// 구 아이템 보상교환 
//
//			CRM_ITEM_RECOVERY_OK,						// 아이템 복구
//
//			CRM_ATTRACTION_ITEM_OK,
//			CRM_ATTRACTION_ITEM_CANCLE,
//			CRM_ATTRACTION_ITEM_RESULT_OK,
//
//			CRM_USE_COUPON_ITEM_OK,
//			CRM_USE_COUPON_ITEM_CANCLE,
//
//			CRM_OPEN_RANDOM_ITEM_CANCLE,
//			CRM_ATTRACTION_ITEM_DUMMY,					// ESC 흡수용 : 아무것도 안 하는 메시지
//		};
//
//		enum CHARACTER_ROOM_STATE
//		{
//			CRS_INVENTORY = 0,
//			CRS_QUEST,
//			CRS_SKILL_TREE,
//		};
//
//		
//
//	public:
//		CX2CharacterRoom( CKTDXStage* pNowStage, const WCHAR* pFileName );
//		virtual ~CX2CharacterRoom(void);
//
//		HRESULT OnFrameMove( double fTime, float fElapsedTime );
//		bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//
//		bool GetCheckOperationCondition();
//
//		// handler
//		bool Handler_EGS_CHANGE_INVENTORY_SLOT_ACK( KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK& kEvent );
//		bool Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_DELETE_ITEM_REQ();
//		bool Handler_EGS_DELETE_ITEM_ACK( KEGS_DELETE_ITEM_ACK& kEvent );
//
//		bool Handler_EGS_SELL_ITEM_REQ();
//		bool Handler_EGS_SELL_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_REPAIR_ITEM_REQ();
//		bool Handler_EGS_REPAIR_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_GET_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//		//bool Handler_EGS_GET_SKILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_CHANGE_SKILL_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//		bool Handler_EGS_CHANGE_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		//{{ 2007. 10. 8  최육사  랜덤 아이템 핸들러
//		void PlayGetRandomItem();
//		bool Handler_EGS_OPEN_RANDOM_ITEM_REQ();
//		bool Handler_EGS_OPEN_RANDOM_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		
//		//}}
//
//		//////////////////////////////////////////////////////////////////////////
//
//		bool Handler_EGS_RESOLVE_ITEM_REQ();
//		bool Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//
//		bool Handler_EGS_ENCHANT_ITEM_REQ( bool bSpecialEnchant );
//		bool Handler_EGS_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_SOCKET_ITEM_REQ( int socketIndex, bool bIsRareMagicStone, bool bIsRemoveSocketOption );
//		bool Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( int slotNum, int enchantID, bool bDebug );
//		bool Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_IDENTIFY_ITEM_REQ();
//		bool Handler_EGS_IDENTIFY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_ITEM_EXCHANGE_REQ();
//		bool Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//
//
//		void SetNowStage( CKTDXStage* pNowStage );
//		void SetOpen( bool bOpen );
//		bool GetOpen() { return m_bOpen; } 
//
//		void SetReserveCharRoomState( CHARACTER_ROOM_STATE charRoomState ) { m_NowCharacterRoomState = charRoomState; }
//		CHARACTER_ROOM_STATE GetCharRoomState() { return m_NowCharacterRoomState; }
//
//		bool SetCharacterRoomState( CHARACTER_ROOM_STATE characterRoomState );
//		bool ChangeInventoryTab( CX2Inventory::SORT_TYPE sortType );
//		bool SetInventorySort( CX2Inventory::SORT_TYPE sortType, int pageNum = 1 );
//
//		void SetUnitClicked( bool bClicked );
//
//		void SetNowDragItemReturn();
//
//		void ResourcePreLoad();
//		void ClearDeviceList();
//
//		bool IsInEquippedItemSlotReact( D3DXVECTOR2 mousePos );
//		
//		bool GetPlayGetRandomItem() { return m_bPlayGetRandomItem; }
//
//		CX2UnitViewerUI* GetUnitViewer() { return m_pUnitViewerUI; }
//
//		
//		// 스킬 트리 관련
//		CX2SkillTreeUI* GetSkillTreeUI() const { return m_pSkillTreeUI; }
//		CKTDGUIDialog* GetDLGSkillSlot() const { return m_pDLGSkillSlot; }
//		void ResetSkillSlotUI();	
//
//		void CreatLevelupEffect();
//		
//		// 되팔기 관련
//		void SellItemNumChange( int iNum );			// 몇개 올리고 내릴것인지?
//		void DecideSellItemNum( bool bCheck );	// 결정 혹은 취소
//
//		// All Equipping Item Repair
//		void OpenDLGAllEquippingItemRepair();
//		void AllEquippingItemRepairREQ();
//
//		void ResetUnitViewerUI();
//
//		void PlayWarningAllReady();
//		void StopWarningAllReady();
//		
//		
//		void ResetStat();
//		bool UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo, bool bInventoryUIUpdate = true );
//
//        // 인벤내 사용아이템
//        bool Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid);
//        bool Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );       
//            
//#ifdef TITLE_SYSTEM
//        // 타이틀 이름 설정
//        void SetTitleName();
//        void SetTitleNew(bool val);
//#endif
//
//	protected:
//
//		
//		void SetCharacterRoomInventoryState();
//		void SetCharacterRoomQuestState();
//		void SetCharacterRoomSkillTreeState();
//
//		bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem );
//		bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID );
//		
//		//캐릭터 룸 관련
//		void ResetNowEquipUI();
//		void ResetCharRoomEDnCashUInVP();
//		void UpdateLevel();				// UI에 level 정보를 갱신
//		void RemoveNoEnduranceNowEquip(); //내구도 없는 아이템 벗기기
//		void ResetResurrectionStoneUI();
//		void ResetSpirit();
//
//		//인벤 관련
//		void ResetRoomStateToInventory( UidType iItemUID = -1 );		//캐릭터 룸에 있는 아이템 클릭시 해당 인벤토리면 인벤토리 탭이 나옴
//		void UpdateSP();				// UI에 SP 정보를 갱신
//		void UpdateExpGageBar();		// UI에 EXP gage 정보를 갱신
//
//
//		//슬롯 관련
//		bool MouseDown( D3DXVECTOR2 mousePos );
//		bool MouseUp( D3DXVECTOR2 mousePos );
//
//		void MouseMove( D3DXVECTOR2 mousePos );
//		bool MouseRButtonUp( D3DXVECTOR2 mousePos );
//
//		bool OnDropItemAnyToEquip( D3DXVECTOR2 mousePos );
//		bool OnDropItemAnyToInven( D3DXVECTOR2 mousePos );
//		void OnDropItemAnyToOther( D3DXVECTOR2 mousePos );
//
//		bool OnRClickedItemInEquip( D3DXVECTOR2 mousePos );
//		bool OnRClickedItemInInven( D3DXVECTOR2 mousePos );
//
//		void DrawSlotMouseOverImage();
//
//
//		wstring GetSlotItemDesc();
//
//		//리소스 로딩 관련
//		void ResourcePreLoad( CX2Inventory::SORT_TYPE sortType, vector< CKTDXDevice* >& vecReadyDevice );
//
//		bool XSkinMeshReady( const WCHAR* pFileName );
//		bool XMeshReady( const WCHAR* pFileName );
//		bool TextureReady( const WCHAR* pFileName );
//		bool SoundReady( const WCHAR* pFileName );
//		void DeviceReady( CKTDXDevice* pDevice ){ m_pReadyDeviceList.push_back( pDevice ); }
//
//
//
//		//기타
//		void UnitHandling( double fTime, float fElapsedTime );
//
//		bool IsInUnitClickReact( D3DXVECTOR2 mousePos );
//
//
//		wstring GetRadioButtonNameByInvenSortType( CX2Inventory::SORT_TYPE sortType );
//
//		void UpdateOpenRandomItemAck();
//
//		//되팔기 관련
//		void OpenSellWindow( CX2SlotItem* pSlot );
//		void OpenItemSellNumDLG( UidType sellItemUID );
//
//		//수리 관련
//		void OpenRepairWindow( CX2SlotItem* pSlot );
//
//		//분해 관련
//		void OpenDecompositionWindow( CX2SlotItem* pSlot ); 
//		void ChangeDecompositionPage( bool bNextPage );
//		void OpenDecompositionResultWindow( std::map< int, int >& mapInsertedItem, 
//											std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
//											bool bJackpot, bool bIdentifyResult = false );
//
//		
//	
//
//		//강화 관련
//		int GetEnchantStoneLevel( int iItemLevel );
//		void UpdateEnchantWindow();
//		void OpenEnchantWindow( CX2SlotItem* pSlot );
//		void OpenEnchantResultWindow( KEGS_ENCHANT_ITEM_ACK& kEGS_ENCHANT_ITEM_ACK );
//
//		//소켓 관련
//		void OpenSocketWindow( CX2SlotItem* pSlot );
//		void ResetSocketWindow();
//		void OpenSocketCheatWindow();
//		void CloseSocketCheatWindow();
//
//		//속성 강화 관련
//		void OpenAtrribEnchantWindow( CX2SlotItem* pSlot );
//		void ResetAtrribEnchantWindow();
//
//		//엘 감정, 되돌리기
//		void OpenElChangerWindow( bool bIdentify, CX2Item* pItem );
//		void AdjustElChangerItemCount( const WCHAR* wszAdjustCommand );
//
//		// 아이템 보상 교환
//		void OpenExchangeOldItemWindow( CX2SlotItem* pSlot );
//
//
//
//
//		void CheckMyInfoChange();
//
//		int GetQuickSlotNumLayer() { return m_SlotManagerLayer + 3; }
//		int GetItemDescLayer() { return m_SlotManagerLayer + 4; }
//
//		void ResetInvenPage( int nowPage, int maxPage );
//		void ResetInvenPageUI();
//		void NextInvenPage();
//		void PrevInvenPage();      
//
//        // 인벤내 사용가능한 아이템인지 검사
//        bool CheckUseItem(CX2Item *pItem);
//
//		int GetRequiredEnchantAdjuvantItemID( int iItemLevel );
//#ifdef ITEM_RECOVERY_TEST
//		int GetRequiredRecoveryStoneID( int iItemLevel );
//		bool Handler_EGS_RESTORE_ITEM_REQ( UidType RecoveryItemUid );
//		bool Handler_EGS_RESTORE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//		void PlayGetAttractionItem();
//		void UpdateOpenAttractionItemAck();
//		bool IsEnchantCoupon( int ItemID );
//		bool IsEnchantCouponUseable(int CouponItemID, int TargetEnchantLevel );
//
//		bool Handler_EGS_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID );
//		bool Handler_EGS_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//#endif
//
//	private:
//		
//		bool					m_bOpen;
//
//		CKTDGUIDialog*			m_pDLGUnitStateTopMenu;
//		CKTDGUIDialog*			m_pDLGMyInfoFront;
//
//		CKTDGUIDialog*			m_pDLGMyInfoInventory;
//
//		CKTDGUIDialog*			m_pDialogToRemoveItem;
//		CKTDGUIDialog*			m_pDLGToRepair;
//
//		CKTDGUIDialog*			m_pDLGMyInfoQuickSlotNumber;
//
//		CKTDGUIDialog*			m_pDLGSkillSlot;
//
//		//{{ 2007. 10. 8  최육사  랜덤 아이템 다이얼로그 포인터
//		CKTDGUIDialog*			m_pDLGRandomItem;
//		//}}
//		CKTDGUIDialog*			m_pDLGResolveItem;
//		CKTDGUIDialog*			m_pDLGEnchantItem;
//		CKTDGUIDialog*			m_pDLGResolveItemResult;
//		CKTDGUIDialog*			m_pDLGEnchantItemResult;
//
//		CKTDGUIDialog*			m_pDLGSocketItem;
//		CKTDGUIDialog*			m_pDLGSocketItemPushOk;
//
//		CKTDGUIDialog*			m_pDLGAttribEnchantItem;
//
//		CKTDGUIDialog*			m_pDLGElChanger;
//
//		CKTDGUIDialog*			m_pDLGWarningAllReady;
//
//		CKTDGUIDialog*			m_pDLGSocketItemCheat;
//
//		CKTDGUIDialog*			m_pDLGCubeCheck;
//
//		CKTDGUIDialog*			m_pDLGSocketItemGuide;
//		CKTDGUIDialog*			m_pDLGEnchantItemGuide;
//		CKTDGUIDialog*			m_pDLGAttribItemGuide;
//
//		CX2SkillTreeUI*			m_pSkillTreeUI;
//		
//		CX2UnitViewerUI*		m_pUnitViewerUI;
//		float					m_fRemainRotY;
//		float					m_fMouseSensitivity;
//		bool					m_bRotateReturn;
//
//		D3DXVECTOR2				m_UnitClickPos;
//		D3DXVECTOR2				m_UnitClickSize;
//
//
//		CX2StateMenu*			m_pStateMenu;
//	
//
//
//		CHARACTER_ROOM_STATE	m_NowCharacterRoomState;
//
//		CKTDXDeviceTexture*		m_pTextureImpossibleEqip;
//
//		D3DXVECTOR2				m_MyInfoSlotItemReactLeftTop;
//		D3DXVECTOR2				m_MyInfoSlotItemReactRightBottom;
//
//		CX2Inventory::SORT_TYPE m_NowInventorySortType;
//		int						m_NowInvenSortTypePageNum;
//		int						m_NowInvenSortTypePageMaxNum;
//
////		CX2Stat*				m_pStat;
////		CX2Stat*				m_pStatEqip;
////	CX2Stat				    m_Stat;
////	CX2Stat				    m_StatEqip;
//
//		vector<CKTDXDevice*>	m_pReadyDeviceList;
//
//		D3DXVECTOR2				m_GarbageBoxPos;
//		D3DXVECTOR2				m_GarbageBoxSize;
//
//		D3DXVECTOR2				m_RepairBoxPos;
//		D3DXVECTOR2				m_RepairBoxSize;
//
//		int						m_TempSortType;
//		int						m_TempSlotID;
//
//		UidType					m_TempItemUID;
//
//
//
//
//		UidType					m_QuickSellItemUID;
//		
//		//{{ 2007. 10. 8  최육사  랜덤 아이템 임시 저장
//		UidType					m_iRandomItemUID;
//		int						m_RandomItemID;
//		//}}
//
//		UidType					m_DecompositionItemUID;
//		UidType					m_EnchantItemUID;
//		bool					m_bUseEnchantAdjuvant;				// 강화 보조제 사용여부
//
//		UidType					m_SocketItemUID;
//		int						m_SocketSlotIndex;
//		bool					m_bSocketRareMagicStone;
//		bool					m_bSocketRemove;
//
//		UidType					m_AttribEnchantItemUID;
//
//		UidType					m_ElChangerItemUID;
//		int						m_ElChangerItemCount;
//
//		int									m_DecompositionWindowNowPageNum;
//		std::map< int, int >                m_mapResolveResultInsertedItem;
//
//		CKTDGUIDialog*									m_pDLGGetRandomItemBG;
//		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstRandomBox;
//		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstRandomBoxKey;
//
//		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqKeyTrace;
//
//		bool					m_bPlayGetRandomItem;
//		float					m_fPlayTime;
//		float					m_fPlayMaxTime;
//
//		D3DXVECTOR2				m_vQuickSlotWindowPos;
//		D3DXVECTOR2				m_vQuickSlotWindowSize;
//
//		CKTDGUIDialog*			m_pDLGSelectSellNumWindowPopup;
//		UidType					m_SellItemUID;
//		int						m_SellItemNum;
//		static bool				m_sbCheckQuestionSellNum;
//
//		KEGS_OPEN_RANDOM_ITEM_ACK m_TempOpenRandomItemAck;
//
//		CKTDGUIDialog*			m_pDLGAllEquippingItemRepair;
//		vector< UidType >		m_vecAllEquippingItem;
//
//		CKTDGPicChar*			m_pPicCharMenuPlusInfo;
//		CKTDGPicChar*			m_pPicCharMenuPlusInfoRed;
//		int						m_TotalExp;
//		int						m_TotalED;
//		int						m_TotalVP;
//		int						m_TotalLevel;
//
//		int						m_AttribEnchantSlotID;
//		int						m_AttribEnchantID;
//
//#ifdef ITEM_RECOVERY_TEST
//		UidType					m_RecoveryItemUid;
//#endif
//#ifdef ATTRACTION_ITEM_TEST
//		bool					m_bOpenedAttractionItem;
//		bool					m_bPlayAttractionItem;
//		CKTDGUIDialog*									m_pDLGOpenAttraction;
//		CKTDGUIDialog*									m_pDLGAttractionResult;
//		CKTDGUIDialog*									m_pDLGGetAttractionItemBG;
//
//		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstMeltMachine;
//		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstMeltMachineStart;
//
//		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSpreadLight;
//		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEndLight;
//
//		// 강화권관련
//		bool					m_bUseCoupon;
//		UidType					m_CouponUid;
//		UidType					m_CouponTargetUid;
//#endif
//#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
//		bool					m_bUsingRandomItemAutoOpen;
//		int						m_OpenedRandomItemCount;
//		CKTDGUIDialog*			m_pDLGMsgBox;
//		std::map< int, int >	m_MapAutoRandomItemGet;
//#endif
//};

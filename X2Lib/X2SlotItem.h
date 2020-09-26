#pragma once

class CX2SlotItem : public CX2SlotManager::CX2Slot
{
	public:
		CX2SlotItem(void);
		virtual ~CX2SlotItem(void);

		virtual void ResetPos();

		CX2Inventory::SORT_TYPE GetSortType() { return m_SortType; }
		void SetSortType( CX2Inventory::SORT_TYPE sortType ) { m_SortType = sortType; }

		int GetSlotID() { return m_SlotID; }
		void SetSlotID( int slotID ) { m_SlotID = slotID; }

		bool GetFashion() { return m_bFashion; }
		CX2Unit::EQIP_POSITION GetEquipPos() { return m_EquipPos; }

		wstring GetSlotDesc() { return m_SlotDesc; }

		void SetItemUID( UidType ItemUID ) { m_ItemUID = ItemUID; }
		UidType GetItemUID() { return m_ItemUID; }

		int GetItemTID() { return m_ItemTID; }
		void SetItemTID( int val ) { m_ItemTID = val; }

		bool CreateItemUI( CX2Item* pItem, int itemQuantity = -1 );
		bool CreateItemUI( int itemID, int itemQuantity = -1, bool bUseImpossibleImg = false, bool bQuantityStr = false );
		bool CreateItemUI( 
            const CX2Item::ItemTemplet* pItemTemplet, 
            int itemQuantity = -1, bool bUseImpossibleImg = false, bool bQuantityStr = false  );

		bool CreateMaterialItemUI( 
            const CX2Item::ItemTemplet* pItemTemplet, 
            int itemQuantity, int needItemQuantity );

		bool IsCreatedItemUI() { return m_pDLGSlot != NULL; }
		bool IsResetItemUI();

		bool ResetItemUI( CX2Item* pItem );
		bool ResetItemUI( int itemID, int itemQuantity = -1, bool bUseImpossibleImg = false, bool bQuantityStr = false  );
		
		bool LostItemUI();

		void DestroyItemUI();

#ifdef SERV_GLOBAL_BILLING
		void SetKBillOrderInfo( KBillOrderInfo& kKBillOrderInfo );
		KBillOrderInfo& GetKBillOrderInfo() { return m_KBillOrderInfo; }
#else // SERV_GLOBAL_BILLING
		void SetKNXBTOrderInfo( KNXBTOrderInfo& kKNXBTOrderInfo );
		KNXBTOrderInfo& GetKNXBTOrderInfo() { return m_KNXBTOrderInfo; }
#endif // SERV_GLOBAL_BILLING


		void CreateRibbonPicture( 
            const CX2Item::ItemTemplet* pItemTemplet, 
            CKTDGUIStatic* pStatic );
		void SetShowImpossiblePicture(bool bShow);

		//{{ kimhc // 2009-08-26 // 아이템 봉인 아이콘
#ifdef	SEAL_ITEM
		void CreateSealedStatic();				// 봉인아이콘Picture를 가진 static 생성
		void SetShowSealedImage( bool bShow );	// 봉인아이콘을 보일것인가 말것인가?
#endif	SEAL_ITEM
		//}} kimhc // 2009-08-26 // 아이템 봉인 아이콘

#ifdef SERV_SOCKET_NEW
		//{{ oasis907 : 김상윤 [2010.5.11] // 소켓 강화 사용중인 아이템 표시
		void CreateSocketUseStatic();				
		void SetShowSocketUseImage( UidType Uid );
		//}}
#endif SERV_SOCKET_NEW
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
		void SetShowExchangeSelectImage( bool bShow );
		void SetShowExchangeUseImage( int itemID );
		void ExchangeReadyEffectStart();
		void ExchangeReadyEffectEnd();
#endif SERV_ITEM_EXCHANGE_NEW

#ifdef QUEST_REWARD_PERIOD
		int GetPeriod() { return m_Period; }
#endif QUEST_REWARD_PERIOD
#ifdef SERV_EXCHANGE_PERIOD_ITEM
		void SetPeriod( IN int iPeriod ) { m_Period = iPeriod; }
#endif //SERV_EXCHANGE_PERIOD_ITEM

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		void CreateSellWaitingStatic();						/// 개인 상점에 등록중인 아이템 표시 아이콘 생성
		void SetShowWaitingSellImage( IN bool bShow_ );		/// 개인 상점에 등록중인 아이템 표시 아이콘 활성 여부
#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef PACKAGEITEM_SET_NOT_EQUIP_ITEM
		bool CheckPackageitemSetNotEquipItem( const CX2Item::ItemTemplet* pItemTemplet);
#endif PACKAGEITEM_SET_NOT_EQUIP_ITEM

	protected:

		void LuaGetValue( KLuaManager& luaManager );

		CKTDGUIDialogType CreateItemUIDialog();

		CX2Inventory::SORT_TYPE m_SortType;

		bool					m_bFashion;
		CX2Unit::EQIP_POSITION	m_EquipPos;

		wstring					m_SlotDesc;

		UidType					m_ItemUID;
		int						m_ItemTID;

		bool					m_bReset;

#ifdef QUEST_REWARD_PERIOD
		int						m_Period;	//몇일 짜리 기간제 아이템인가?
#endif QUEST_REWARD_PERIOD
		
#ifdef SERV_GLOBAL_BILLING
		KBillOrderInfo			m_KBillOrderInfo;
#else // SERV_GLOBAL_BILLING
		KNXBTOrderInfo			m_KNXBTOrderInfo; //원래는 넥슨쪽 캐시 아이템 슬롯으로 상속받아서 써야하는데 시간이 없으니까 일단 요로코롬
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hEffectExchangeReady;
#endif SERV_ITEM_EXCHANGE_NEW

};

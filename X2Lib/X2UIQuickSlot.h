#ifdef NEW_VILLAGE_UI

#pragma once

// StateMenu에서 분리시킴
class CX2UIQuickSlot : public CX2UIDragable
{
public:
	CX2UIQuickSlot( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIQuickSlot(void);

	
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool KeyEventProcess();

	//virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool GetShow(){ return m_bShow; }
	void SetShow(bool val);
	bool GetEnable(){ return m_bEnable; }

#ifdef SERV_NEW_DEFENCE_DUNGEON
	void SetEnable(bool val, bool bDirectEnable = false);
#else //SERV_NEW_DEFENCE_DUNGEON
	void SetEnable(bool val);
#endif //SERV_NEW_DEFENCE_DUNGEON

	void SetLayer(X2_DIALOG_LAYER layer);

	void ResetQuickSlotUI();
	CX2SlotItem* GetEmptyslot();


	bool GetWaiting_EGS_USE_QUICK_SLOT_ACK() const { return m_bWaiting_EGS_USE_QUICK_SLOT_ACK; }
#ifdef SPECIAL_USE_ITEM
	void SetWaiting_EGS_USE_QUICK_SLOT_ACK( bool bWaiting_EGS_USE_QUICK_SLOT_ACK ) { m_bWaiting_EGS_USE_QUICK_SLOT_ACK = bWaiting_EGS_USE_QUICK_SLOT_ACK; }
#endif SPECIAL_USE_ITEM

	//{{ kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체
#ifdef REAL_TIME_ELSWORD
	void UpdateCoolTime( int itemID );
	bool ResetCoolTime();				// 퀵슬롯 내의 모든 아이템의 쿨타임 초기화
#endif REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 실시간 퀵슬롯 교체

	//{{ kimhc // 2009-08-08 //	은행에 있는 item, drag & drop 때 UIDragable에 있는 것을 그대로 사용할 시에
	//							에러발생되므로 재정의 함
#ifdef	PRIVATE_BANK
	//virtual void	DrawSlotMouseOverImage();
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-08 // 은행에 있는 item, drag & drop 때 UIDragable에 있는 것을 그대로 사용할 시에
	//							에러발생되므로 재정의 함

#ifdef	SERV_EXPAND_QUICK_SLOT
	void SetExpandQuickSlot( int iMaxNumOfSlot );
#endif  SERV_EXPAND_QUICK_SLOT

	void InitMyQuickSlotCoolTimeFromGageManager( const map<int, int>& mapQuickSlotCoolTime_ );
	void GetQuickSlotStringByIndex( IN int iIndex_, OUT wstring& strQuickSlot_ ) const;
	void UpdateSlotKey();
	

#ifdef CREATE_ALLY_NPC_BY_MONSTER
	void UpdateCoolTimeByGroupID( int iCoolTimeGroupID );
#endif //CREATE_ALLY_NPC_BY_MONSTER

#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool GetIsSummonCardSlot() const { return m_bIsSummonCardSlot; }	/// 어둠의 문 몬스터 카드용 퀵슬롯 여부 반환
	void SetIsSummonCardSlot(bool val) { m_bIsSummonCardSlot = val;}	/// 어둠의 문 몬스터 카드용 퀵슬롯 여부 설정
#endif //SERV_NEW_DEFENCE_DUNGEON

protected:
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem );
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID );
	bool Handler_EGS_USE_QUICK_SLOT_REQ( int slotID );
	bool Handler_EGS_USE_QUICK_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_USE_QUICK_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );
	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos );
	virtual bool MouseDown(D3DXVECTOR2 mousePos);
#ifdef NEW_ITEM_NOTICE
	virtual wstring GetSlotItemDesc();
#endif //NEW_ITEM_NOTICE

#ifdef SERV_NEW_DEFENCE_DUNGEON
	void SetSlotItemIcon( vector< CX2Slot* >& m_SlotList, const CX2Inventory& kInventory, CKTDGUIStatic* pStatic );			/// 퀵슬롯 아이콘 설정 함수
	void SetSummonMonsterItemIcon( vector< CX2Slot* >& m_SlotList, const CX2Inventory& kInventory, CKTDGUIStatic* pStatic );	/// 몬스터 소환 카드 슬롯 아이콘 설정 함수

	void InputSummonCardSlotKey( const CX2Inventory& kInventory, CX2Item** pQuickItem, int& slotID );		/// 몬스터 소환 카드 슬롯 키보드 입력 함수
	void InputQuickSlotKey( const CX2Inventory& kInventory, CX2Item** pQuickItem, int& slotID );			/// 퀵슬롯 키보드 입력 함수

	bool Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ( int slotID );											/// 몬스터 카드 퀵슬롯 사용 요청 함수
	bool Handler_EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// 몬스터 카드 퀵슬롯 사용 응답 함수
#endif //SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
	void OnFrameMove_CoolTime( CX2Item* pItem_, CX2SlotItem* pSlotItem_ );
#endif //SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP

private:

	bool				m_bShow;
	bool				m_bEnable;
	CKTDGUIDialogType		m_pDLGQuickSlot;

	bool						m_bWaiting_EGS_USE_QUICK_SLOT_ACK;	

	// 오현빈 // SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
	// 상기 디파인 작업으로 int를 ItemID에서 GroupID로 변경함
	map < int, CKTDXTimer >		m_mapQuickSlotItemCoolTime;

#ifdef NEW_ITEM_NOTICE
	CKTDGUIDialogType		m_pDLGItemHighLight;
#endif //NEW_ITEM_NOTICE

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-03-25
	bool					m_bIsSummonCardSlot;		/// 어둠의 문 몬스터 카드용 퀵슬롯 여부
#endif //SERV_NEW_DEFENCE_DUNGEON
};

#endif

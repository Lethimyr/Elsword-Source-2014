#ifdef NEW_VILLAGE_UI

#pragma once

#define MAX_INVENTORY_SIZE 24

//{{ kimhc // 실시간 엘소드 중 실시간 장비 교체 및 아바타 교체
#ifdef	REAL_TIME_ELSWORD
namespace	_CONST_UIINVENTORY_
{
	const	int		g_iEquippedNormalWeaponSlotID		=		9;
	const	int		g_iEquippedFashionWeaponslotID		=		10;
}
#endif	REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 실시간 장비 교체 및 아바타 교체

//class CX2StateMenu;
class CX2UIInventory : public CX2UIDragable
{
public:
//	enum SORT_TYPE
//	{
//		ST_NONE = 0,
//		ST_EQUIP,
//		ST_ACCESSORY,
//		ST_MATERIAL,
//		ST_SPECIAL,
//		ST_QUEST,
//		ST_QUICK_SLOT, //소비성
//		ST_AVARTA,
//		ST_REPOSITORY,
//		ST_E_EQUIP,
//		ST_E_SKILL,
//		ST_E_QUICK_SLOT,
//		ST_END,
//	};

	enum UI_INVENTORY_MSG
	{
		UIM_EXIT = 11000,
		UIM_EQUIP,
		UIM_ACCESSORY,
		UIM_QUICK_SLOT,
		UIM_MATERIAL,
		UIM_QUEST,
		UIM_SPECIAL,
		UIM_AVATAR,
		UIM_PREV_PAGE,
		UIM_NEXT_PAGE,	

#ifdef SERV_REFORM_INVENTORY_TEST
		UIM_PAGE_1,
		UIM_PAGE_2,
		UIM_PAGE_3,
//#ifdef SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		UIM_PAGE_4,
//#endif SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR	
		UIM_OVER_PAGE_1,
		UIM_OVER_PAGE_2,
		UIM_OVER_PAGE_3,
//#ifdef SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR
		UIM_OVER_PAGE_4,
//#endif SERV_REFORM_INVENTORY_AVATAR_CHARTER_FOUR	
#endif SERV_REFORM_INVENTORY_TEST

		UIM_OPEN_RANDOM_ITEM = 11100,
		UIM_CLOSE_RANDOM_ITEM_RESULT,
		UIM_OPEN_RANDOM_ITEM_NEW,
		UIM_EXIT_RANDOM_ITEM_NEW,

		UIM_CREATE_PERSONAL_SHOP = 11200,

		UIM_RESOLVE_ITEM_OK			= 11300,
		UIM_RESOLVE_ITEM_CANCLE,
		UIM_RESOLVE_ITEM_RESULT_OK,
		UIM_RESOLVE_ITEM_RESULT_PREV_PAGE,
		UIM_RESOLVE_ITEM_RESULT_NEXT_PAGE,
		//{{ 2011.03.16   임규수 아바타 분해 시스템
//#ifdef SERV_MULTI_RESOLVE
		UIM_RESOLVE_ITEM_OPEN,
//#endif SERV_MULTI_RESOLVE

		UIM_SOCKET_DLG_CLOSE				= 11400,
		UIM_SOCKET_NORMAL_STONE,
		UIM_SOCKET_RARE_STONE,
		UIM_SOCKET_REMOVE,
		UIM_SOCKET_FEE_OK,

		UIM_EL_CHANGER_EXIT		= 11500,
		UIM_EL_CHANGER_NUM_PLUS,
		UIM_EL_CHANGER_NUM_MINUS,
		UIM_EL_CHANGER_NUM_UPDATE,
		UIM_EL_CHANGER_OK,
		UIM_EL_CHANGER_RESULT_OK,

		UIM_ITEM_RECOVERY_OK = 11600,
		UIM_ITEM_USE_WARP,		
		UIM_SKILL_PLUS_ITEM_USE,

//#ifdef SERV_TREASURE_BOX_ITEM
		UIM_TREASURE_BOX_CANCLE			= 11650,
//#endif SERV_TREASURE_BOX_ITEM

		UIM_ATTRACTION_ITEM_OK			= 11700,
		UIM_ATTRACTION_ITEM_CANCLE,
		UIM_ATTRACTION_ITEM_RESULT_OK,

		UIM_USE_COUPON_ITEM_OK,
		UIM_USE_COUPON_ITEM_CANCLE,

		UIM_OPEN_RANDOM_ITEM_CANCLE,
		UIM_ATTRACTION_ITEM_DUMMY,					// ESC 흡수용 : 아무것도 안 하는 메시지

		UIM_SELL_CATEGORY_ALL,
		UIM_DELETE_ITEM,

//#ifdef SERV_ATTRIBUTE_CHARM
		UIM_USE_ATTRIBUTE_CHARM_COUPON_ITEM_OK,
//#endif SERV_ATTRIBUTE_CHARM

		//{{ 2012. 02. 21	김민성	인벤토리 정렬
//#ifdef SERV_SORT_CATEGORY_ITEM 
		UIM_SORT_CATEGORY_ITEM,
//#endif SERV_SORT_CATEGORY_ITEM 
		//}}

		//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
//#ifdef REAL_TIME_ELSWORD

		UIM_REGISTER_QUANTITY_CONFIRM = 11750,
		UIM_REGISTER_QUANTITY_CANCEL,
		UIM_REGISTER_QUANTITY_PLUS,
		UIM_REGISTER_QUANTITY_MINUS,
		UIM_REGISTER_QUANTITY_UPDATE,
		UIM_DELETE_ITEM_CONFIRM_OK,
		UIM_DELETE_ITEM_CONFIRM_CANCEL,

//#endif	REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

		UIM_CHANGE_NICK_NAME_OK			= 11800,
		UIM_SKILL_INIT_OK,

		UIM_ITEM_USE_TITLE, 		


		UIM_ITEM_USE_SKILL_UNSEAL,

		//{{ kimhc // 2009-08-19 // 봉인 관련 다이얼로그
		UIM_SEAL_ITEM_CONFIRM_OK,
		UIM_SEAL_ITEM_CONFIRM_CANCEL,
		UIM_UNSEAL_ITEM_CONFIRM_OK,
		UIM_UNSEAL_ITEM_CONFIRM_CANCEL,
		//}} kimhc // 2009-08-19 // 봉인 관련 다이얼로그

		UIM_WARP_DEST_CLOSE,
		UIM_WARP_DEST_SELECT,
		UIM_WARP_DEST,	
		UIM_GUILD_SKILL_INIT_OK,

		UIM_PC_BANG,

		UIM_SKILL_NOTE,
		UIM_UNKNOWN_TO_IDENTIFY, // 엘 속성 판별

//#ifdef SERV_PET_SYSTEM
		UIM_PET_FOOD,	// 펫 먹이주기
		UIM_PET_FOOD_OK,
//#ifdef ADD_PET_INVENTORY_BUTTON
		UIM_PET_INVENTORY_OPEN,
//#endif ADD_PET_INVENTORY_BUTTON
//#endif
//#ifdef BUFF_TEMPLET_SYSTEM
		UIM_ITEM_USE_DEFENCE_DUNGEON_POTION,
//#endif BUFF_TEMPLET_SYSTEM
//#ifdef SERV_PSHOP_AGENCY
		UIM_CHECK_PSHOP_OK,
//#endif
		UIM_WARP_DEST_OK,
//#ifdef PET_DROP_ITEM_PICKUP
		UIM_PET_DROP_ITEM_PICK_UP_OK,
//#endif //PET_DROP_ITEM_PICKUP
//#ifdef SPT_NONE_ITEM_DELETE
		UIM_SPTNONE_ITEM_DELETE_OK,
		UIM_SPTNONE_ITEM_DELETE_CANCEL,
//#endif //SPT_NONE_ITEM_DELETE
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //미라클큐브
		UIM_USE_RESTORE_ITEM_EVALUATION_OK,
		UIM_USE_RESTORE_ITEM_EVALUATION_CANCEL,
		UIM_USE_EVALUATE_ITEM_OK,
		UIM_USE_EVALUATE_ITEM_CANCEL,
		UIM_EXCHANGE_NEW_ITEM_OK,
		UIM_EXCHANGE_NEW_ITEM_CANCEL,
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
//#ifdef RIDING_ITEM_POPUP_CHECK
		UIM_RIDING_ITEM_USE_OK,
//#endif //RIDING_ITEM_POPUP_CHECK

//#ifdef FINALITY_SKILL_SYSTEM //JHKang
		UIM_EL_BREAK_EXIT,
		UIM_EL_BREAK_NUM_PLUS,
		UIM_EL_BREAK_NUM_MINUS,
		UIM_EL_BREAK_NUM_UPDATE,
		UIM_EL_BREAK_OK,
		UIM_EL_BREAK_RESULT_OK,
//#endif //FINALITY_SKILL_SYSTEM

//#ifdef GOOD_ELSWORD //JHKang
		UIM_EXPAND_INVENTORY_CASH,
		UIM_EXPAND_INVENTORY_ED,
		UIM_EXPAND_INVENTORY_ED_BUY_CONFIRM,
		UIM_EXPAND_INVENTORY_ED_BUY_CANCEL,
//#endif //GOOD_ELSWORD
//#ifdef FIELD_BOSS_RAID // 소켓 슬롯 확장
		UIM_USE_ADD_SOCKET_SLOT_OK		= 11841,
		UIM_USE_ADD_SOCKET_SLOT_CANCEL	= 11842,
//#endif // FIELD_BOSS_RAID
//#ifdef REFORM_SKILL_NOTE_UI
		UIM_BUY_SKILL_NOTE_GUIDE_MOUSE_UP = 11843,
//#endif // REFORM_SKILL_NOTE_UI
//#ifdef SERV_GOLD_TICKET
		UIM_USE_GOLD_TICKET,
//#endif SERV_GOLD_TICKET
//#ifdef SERV_TOUR_TICKET_EVENT
		UIM_USE_TOUR_TICKET,
//#endif SERV_TOUR_TICKET_EVENT
//#ifdef SERV_RELATIONSHIP_EVENT_INT
		UIM_USE_PROPOSE_ITEM,
		UIM_USE_PROPOSE_ITEM_OK,
		UIM_USE_PROPOSE_ITEM_CANCLE,
		UIM_USE_DIVORCE_ITEM,
//#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
		UIM_PET_HALLOWEEN_TRANSFORM_POSION_OK,
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
#ifdef SERV_RECRUIT_EVENT_BASE
		UIM_RECOMMEND_OK,
		UIM_RECOMMEND_CANCEL,
#endif SERV_RECRUIT_EVENT_BASE
#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
		UIM_USE_EXPAND_INVENTORY_OK,
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		UIM_UNLIMITED_SECOND_CHANGE_JOB_CLOSE,
		UIM_UNLIMITED_SECOND_CHANGE_JOB_SELECT,
		UIM_UNLIMITED_SECOND_CHANGE_JOB_DEST,	
		UIM_UNLIMITED_SECOND_CHANGE_JOB_OK,
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
//#ifdef SERV_RECRUIT_EVENT_BASE
//		UIM_RECOMMEND_OK,
//		UIM_RECOMMEND_CANCEL,
//#endif SERV_RECRUIT_EVENT_BASE
//#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
//		UIM_USE_EXPAND_INVENTORY_OK,
//#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
	};

	enum RANDOM_ITEM_OPEN_EVENT_TYPE
	{
		RIOET_NONE,
		RIOET_ICE_HEATER,			// 가열기
		RIOET_MONSTER_CARD_SET,		// 헤니르 시공 관련 몬스터 카드 셋트
#ifdef APRIL_5TH_TREE_DAY_EVENT
		RIOET_APRIL_5TH_TREE_DAY_EVENT,		// 식목일 이벤트
#endif
		RIOET_GOLD_ICE_HEATER,		// 골드 얼음 조각상 가열기
//#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		RIOET_MINI_ICE_HEATER,		// 미니 얼음 조각상 가열기
//#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
//#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
		RIOET_ALL_IN_ONE_ICE_HEATER, // 올인원 얼음 조각상 가열기
//#endif SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
#ifdef RIDING_SYSTEM
		RIOET_RIDING_PET_GACHA,  //탈 것 가챠
#endif RIDING_SYSTEM

#ifdef SERV_HALLOWEEN_ICE_HEATER
		RIOET_HALLOWEEN_ICE_HEATER,
#endif //SERV_HALLOWEEN_ICE_HEATER

		//{{100416 김정협 트레져 박스 추가
//#ifdef 	SERV_TREASURE_BOX_ITEM
		RIOET_TREASURE_BOX_ITEM,		// 트레져 박스
//#endif 	SERV_TREASURE_BOX_ITEM
		//}}100416 김정협 트레져 박스 추가

//#ifdef CUBE_OPEN_IMAGE_MANAGER
		RIOET_EVENT_CUBE,			// 이벤트 큡
//#endif CUBE_OPEN_IMAGE_MANAGER
	};

	//kimhc // 2009-08-12 // 랜덤큐브를 열때 다이얼로그를 다르게 처리해야할 몬스터 카드 아이템
	enum MONSTER_CARD_SET_ID
	{
		MONSTER_CARD_SET_RUBEN	= 69353,
		MONSTER_CARD_SET_ELDER,
		MONSTER_CARD_SET_BESMA,
		MONSTER_CARD_SET_PEITA,
		MONSTER_CARD_SET_ALTERA,
		MONSTER_CARD_SET_SECRET,
		MONSTER_CARD_SET_RUBEN_GOLD_EDITION,
		MONSTER_CARD_SET_ELDER_GOLD_EDITION,
		MONSTER_CARD_SET_BESMA_GOLD_EDITION,
		MONSTER_CARD_SET_PEITA_GOLD_EDITION,
		MONSTER_CARD_SET_ALTERA_GOLD_EDITION,
		MONSTER_CARD_SET_SECRET_GOLD_EDITION,
		MONSTER_CARD_SET_ELIOS,
		MONSTER_CARD_SET_ELIOS_GOLD_EDITION,
		MONSTER_CARD_SET_ELIOS_ALL_EDITION,

	};
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	//오현빈 // 2012-03-07 // 획득한 아이템 결과 확인 창
	enum ITEM_OBTAIN_RESULT_TYPE
	{
		IORT_NONE,
		IORT_CUBE_OPEN,			//큐브 오픈
		IORT_RANDOM_EXCHANGE,	//랜덤 교환
		IORT_ITEM_RESOLVE,		//아이템 분해
	};
#endif RANDOM_EXCHANGE_RESULT_VIEW

#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD		/// 얼음 조각상 가열기 타이밍
	enum ATTRACTION_ITEM_TIME_INFO
	{
		AITI_NONE,
		AITI_START_PARTICLE_1,	/// 첫번째 파티클 생성 시간
		AITI_START_PARTICLE_2,	/// 두번째 파티클 생성 시간
		AITI_START_PARTICLE_3,	/// 세번째 파티클 생성 시간
		AITI_END_PARTICLE,		/// 종료 파티클 생성 시간
		AITI_END_TIME,			/// 종료 시간
		AITI_END,
	};
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD

#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	enum ATTRACTION_ITEM_PARTICLE_NAME_INFO
	{
		AIPNI_NONE,
		AIPNI_START_PARTICLE_1,	/// 첫번째 파티클 이름
		AIPNI_START_PARTICLE_2,	/// 두번째 파티클 이름
		AIPNI_START_PARTICLE_3,	/// 세번째 파티클 이름
		AIPNI_START_PARTICLE_4,	/// 세번째 파티클 이름
		AIPNI_START_PARTICLE_5,	/// 세번째 파티클 이름
		AIPNI_END_PARTICLE_1,	/// 종료 파티클 이름
		AIPNI_END_PARTICLE_2,	/// 종료 파티클 이름
		AIPNI_END,
	};
#endif // SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

#ifdef FIX_ICE_HEATER_EVENT
	
	struct IceHeaterEventData
	{
		wstring m_wstrWaitUIEffectSetName;			/// 가열기 대기 상태 이펙트셋 이름
		wstring m_wstrPlayUIEffectSetName;			/// 가열기 재생 상태 이펙트 셋 이름

		wstring m_wstrWaitMeshPlayerName;			/// 가열기 메시 객체 이름
		wstring m_wstrPlayMeshPlayerName;			/// 가열기 메시 객체 이름

		wstring m_wstrMainDialogTitleTexture;		/// 가열기 메인 다이얼로그 타이틀 텍스처 이름
		wstring m_wstrMainDialogTitleTextureKey;	/// 가열기 메인 다이얼로그 타이틀 텍스처 키

		wstring m_wstrResultDialogTitleTexture;		/// 가열기 결과 다이얼로그 타이틀 텍스처 이름
		wstring m_wstrResultDialogTitleTextureKey;	/// 가열기 결과 다이얼로그 타이틀 텍스처 키

		float	m_fCautionIconOffSetPositionX;		///	가열기 다이얼로그에 표시될 느낌표 아이콘의 오프셋 X 위치
		float	m_fCautionIconOffSetPositionY;		///	가열기 다이얼로그에 표시될 느낌표 아이콘의 오프셋 Y 위치

		float	m_fCautionStringOffSetPositionX;	/// 가열기 다이얼로그의 느낌표 아이콘 옆에 표시될 스트링의 오프셋 X 위치
		float	m_fCautionStringOffSetPositionY;	/// 가열기 다이얼로그의 느낌표 아이콘 옆에 표시될 스트링의 오프셋 Y 위치

		wstring	m_wstrIceHeaterPlaySound;			/// 가열기 가동 재생음

		int		m_iCautionStringNum;				/// 가열기 주의 안내 문구 스트링 아이디

		float	m_fPlayTime;						/// 재생 시간 ( 가열기 가동 ~ 종료까지 시간 )

		IceHeaterEventData()
		: m_wstrWaitUIEffectSetName( L"" )
		, m_wstrPlayUIEffectSetName( L"" )
		, m_wstrWaitMeshPlayerName( L"" )
		, m_wstrPlayMeshPlayerName( L"" )
		, m_wstrMainDialogTitleTexture( L"" )
		, m_wstrMainDialogTitleTextureKey( L"" )
		, m_wstrResultDialogTitleTexture( L"" )
		, m_wstrResultDialogTitleTextureKey( L"" )
		, m_fCautionIconOffSetPositionX( 0.f )
		, m_fCautionIconOffSetPositionY( 0.f )
		, m_fCautionStringOffSetPositionX( 0.f )
		, m_fCautionStringOffSetPositionY( 0.f )
		, m_wstrIceHeaterPlaySound( L"" )
		, m_iCautionStringNum( 0 )
		, m_fPlayTime( 0.f )
		{}

	};

#endif FIX_ICE_HEATER_EVENT

public:

	CX2UIInventory( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIInventory(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL

	bool GetShow() { return m_bShow; }
	void SetShow(bool val);

	void SetLayer(X2_DIALOG_LAYER layer);
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition(D3DXVECTOR2 vec, bool bAbsolute = true );

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	// 오현빈 // 2012/03/02 // 획득 결과창을 출력하기 위한 아이템 획득 타입을 설정하는 함수(큐브오픈, 랜덤아이템교환, 아이템 분해)
	void SetItemObtainResultType(CX2UIInventory::ITEM_OBTAIN_RESULT_TYPE eType){m_eItemObtainResultType = eType;}

#if defined( SERV_MULTI_RESOLVE ) || defined( EXCHANGE_OPEN_IMAGE )
	CX2UIInventory::ITEM_OBTAIN_RESULT_TYPE GetItemObtainResultType(){ return m_eItemObtainResultType; }
#endif // SERV_MULTI_RESOLVE

#endif RANDOM_EXCHANGE_RESULT_VIEW

#ifdef SERV_SOCKET_NEW
	// oasis907 : 김상윤 [2010.4.6] // 
	CX2UISocketItem* GetUISocketItem(){ return m_pUISocketItem; }		
#endif SERV_SOCKET_NEW

	// 인벤 관련
	bool ChangeInventoryTab( CX2Inventory::SORT_TYPE sortType );
	bool SetInventorySort( CX2Inventory::SORT_TYPE sortType, int pageNum = 1 );
	bool ChangeInventoryTabByUid( UidType iItemUID );
	bool UpdateInventorySlot();

	void UpdateUIButton();

#ifndef	SERV_REFORM_INVENTORY_TEST
	void ResetInvenPage( int nowPage, int maxPage );
	void ResetInvenPageUI();
	void NextInvenPage();
	void PrevInvenPage();
#endif SERV_REFORM_INVENTORY_TEST

	CX2Inventory::SORT_TYPE GetSortType(){ return m_NowInventorySortType; }
	int GetNowInvenSortTypePageNum();
	bool UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo, bool bInventoryUIUpdate = true );

	// 인포머 쪽에서 알려주는 새로운 아이템여부 설정
	void SetNewItem(bool val) { m_bIsNewItem = val;}
	bool GetNewItem() { return m_bIsNewItem; }

	bool GetOverMouseInven();

	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem );
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID );

	//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
	bool Handler_EGS_SORT_CATEGORY_ITEM_REQ( CX2Inventory::SORT_TYPE NowInventorySortType );
	bool Handler_EGS_SORT_CATEGORY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SORT_CATEGORY_ITEM 
	//}}


	// 인벤내 사용아이템
#ifdef BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid, UidType iTempCode = 0, int iItemID = 0);
#else  //BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(UidType iUid, int iTempCode = 0);
#endif //BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_USE_ITEM_IN_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );       

#ifdef SERV_SHARING_BANK_TEST
	//{{ 지헌 : 은행간 교환 가능한 아이템인지 체크하는 함수
	bool CheckCanMoveShareItem(CX2Item* pFromItem, CX2Item* pToItem, KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ &kMoveInfo);
	//}}
	bool Handler_EGS_UPDATE_ITEM_POSITION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SHARING_BANK_TEST

#ifdef SERV_TREASURE_BOX_ITEM
	bool GetPlayTreasureBox() 
	{ //배경화면은 항상 켜져있다.
		if(m_pDLGGetAttractionItemBG != NULL)
			return m_pDLGGetAttractionItemBG->GetShow(); 		
		return false;
	}
#endif SERV_TREASURE_BOX_ITEM

	//{{ kimhc // 실시간 엘소드 중 아이템 버리기
#ifdef REAL_TIME_ELSWORD
	bool GetOpenDeleteItemConfirmDLG();
	bool GetOpenRegisterQuantityDLG();
#endif
	//{{ kimhc // 실시간 엘소드 중 아이템 버리기

#ifdef ATTRACTION_ITEM_TEST
	bool GetPlayAttractionItem() 
	{ 
		if(m_pDLGOpenAttraction != NULL)
			return m_pDLGOpenAttraction->GetShow(); 
		return false;
	}
#endif

#ifdef FREE_WARP
	void InitWarpList();
	void AddWarpList(CKTDGUIContolList *pControlList, const int row, const int iVillageId);
	void CreateWarpDest();
	void SetShowWarpDest(bool val);
	bool GetShowWarpDest();
#endif

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	void InitSecondJobList();
	bool AddSecondJobList(CKTDGUIContolList *pControlList, const int row, const CX2Unit::UNIT_CLASS eSecondJobId);
	void CreateSecondJobDest();
	void SetShowSecondJobDest(bool val);
	bool GetShowSecondJobDest();
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

	//kimhc // 2010-02-03 // PC방에서는 PC 탭이 먼저 보이하기 위해, 특수한 경우가 아니라면 이 함수는 sortype만 변경 하므로 SetInventorySort()를 쓰기 바람
	void SetSortType( CX2Inventory::SORT_TYPE eSortType ) { m_NowInventorySortType = eSortType; }

#ifdef SERV_SKILL_NOTE
	void HideSkillNote(bool bVal);
	void SetEnableSkillNote(bool bVal);
#endif
#ifdef REFORM_SKILL_NOTE_UI // 기술의 노트 버튼 제거하지 않고 비활성화로 변경
	void SetEnableBuySkillNote( bool bVal );
	void RegisterMemo( const CX2Item::ItemTemplet* pItemTemplet_, UidType uiMemoItemUID_ );
#endif // REFORM_SKILL_NOTE_UI


#ifdef RANDOM_CUBE_UI_NEW
	CKTDGUIDialogType GetDLGResolveItemResult(){ return m_pDLGResolveItemResult; }
#endif RANDOM_CUBE_UI_NEW

#ifdef SERV_PET_SYSTEM
	void FeedPet();
	void SetEnableFeedPet(bool bVal);

	void SetPetFoodSlot(CX2SlotItem* pSlotItem){ m_pPetFoodSlot = pSlotItem; }

	void OpenPetInventory(); // oasis907 : 김상윤 [2010.9.7] // 펫 인벤토리
	CX2UIPetInventory* GetUIPetInventory(){ return m_pUIPetInventory; }
	void SetAutoFeed(bool bAutoFeed){ m_bAutoFeed = bAutoFeed; }
	bool GetAutoFeed(){ return m_bAutoFeed; }

#ifdef ADD_PET_INVENTORY_BUTTON
	bool IsPetInventoryExist();
	void UpdateButtonPetInventory();
#endif ADD_PET_INVENTORY_BUTTON
#endif
	//오현빈 // 2012-03-06 // 랜덤아이템 교환 결과 창 출력을 위해 CX2UIItemExChangeShop 에서 사용 할 수 있도록 public으로 이동
	void OpenResolveResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
		bool bJackpot );

#ifdef JUNK_AVATAR
	bool AuthDeleteItem( UidType DeleteItemUid_, int Quantity_  )
	{
		return Handler_EGS_DELETE_ITEM_REQ( DeleteItemUid_, Quantity_ );
	}
#endif
	void SetEDString(int iED_);
	void SetAPString(int iAP_);

#ifdef EXCHANGE_OPEN_IMAGE
	void OpenRandomItem( int iItemID, ITEM_OBTAIN_RESULT_TYPE eIORT );
#endif EXCHANGE_OPEN_IMAGE

#ifdef FIX_ICE_HEATER_EVENT
	void	OpenScriptFile( const WCHAR* szScriptFileName_ );
	void	AddIceHeaterEventInfo_LUA();
#endif // FIX_ICE_HEATER_EVENT

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	const CX2Inventory::SORT_TYPE GetInventorySortType() { return m_NowInventorySortType; }

	/// 외부 참조를 위해, Public으로 설정
	bool	UpdatePageUI( int iPage = 1 );

	/// /// 현재 카테고리 페이지 수 반환 ( 외부 참조를 위해, Public으로 설정 )
	int		GetNowInventoryPageNum( CX2Inventory::SORT_TYPE InventorySortType_ ) const { return m_NowInventoryPageNum[ InventorySortType_ - 1 ]; }
#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool	GetIsMouseOver();
#endif // CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	void	UseTransformPetItem();
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

protected:
#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	//오현빈 // 2012-03-06 // 큐브오픈, 랜덤아이템 교환으로 획득한 아이템에 대한 결과창 초기화
	void InitItemObtainResult(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem );
	//오현빈 // 2012-03-06 // 아이템 분해로 획득한 아이템에 대한 결과창 초기화
	void InitItemResolveResult(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem );
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	void InitItemObtainResult2(const WCHAR* pFileName, std::map< int, int >& mapInsertedItem, int iSlotNum = RESOLVE_RESULT_ITEM_LIST_MAX_SLOT_NUM );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
#endif RANDOM_EXCHANGE_RESULT_VIEW
	//{{ kimhc // 2009-08-10 // 은행
#ifdef	PRIVATE_BANK
	bool Handler_EGS_EXPAND_BANK_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		// 은행 슬롯 확장
#endif	PRIVATE_BANK	
	//}} kimhc // 2009-08-10 // 은행

	bool Handler_EGS_CHANGE_INVENTORY_SLOT_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 인벤내 사용가능한 아이템인지 검사
	bool CheckUseItem(CX2Item *pItem);


	//{{ 2007. 10. 8  최육사  랜덤 아이템 핸들러
	bool Handler_EGS_OPEN_RANDOM_ITEM_REQ();
	bool Handler_EGS_OPEN_RANDOM_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		
	//}}
	void UpdateOpenRandomItemAck();
	void UpdateOpenMonsterCardSet();

#ifdef RANDOM_CUBE_UI_NEW
	void UpdateOpenRandomItemAckNew();
#endif RANDOM_CUBE_UI_NEW
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
	void UpdateOpenMiniRandomItemAckNew();
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

	//엘 감정, 되돌리기
	void OpenElChangerWindow( bool bIdentify, CX2Item* pItem );
	void UpdateElChangerDLG( bool bReadIME );
	void OpenElChangerResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo );
	bool Handler_EGS_IDENTIFY_ITEM_REQ();
	bool Handler_EGS_IDENTIFY_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 분해
	void OpenResolveWindow( CX2SlotItem* pSlot );
	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	void OpenResolveWindow();
#endif SERV_MULTI_RESOLVE
	//}}
	void ChangeResolveResultPage( bool bNextPage );
	//오현빈 // 2012-03-06 // 랜덤아이템 교환 결과 창 출력을 위해 CX2UIItemExChangeShop 에서 사용 할 수 있도록 public으로 이동
// 	void OpenResolveResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo,
// 							      bool bJackpot );
	bool Handler_EGS_RESOLVE_ITEM_REQ();
	bool Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


#ifdef SERV_SOCKET_NEW
// oasis907 : 김상윤 [2010.4.19] // 
	bool OpenNewSocketWindow( CX2SlotItem* pSlot );
#else
	// 소켓
	void OpenSocketWindow( CX2SlotItem* pSlot );
	void ResetSocketWindow();
	void OpenSocketCheatWindow();
	void CloseSocketCheatWindow();

	bool Handler_EGS_SOCKET_ITEM_REQ( int socketIndex, bool bIsRareMagicStone, bool bIsRemoveSocketOption );
	bool Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SOCKET_NEW

#ifdef SERV_GOLD_TICKET
	bool handler_EGS_CHARGE_POINT_NOT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_GOLD_TICKET

	//슬롯 관련
	virtual bool MouseDown( D3DXVECTOR2 mousePos );
	virtual bool MouseUp( D3DXVECTOR2 mousePos );

	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );

	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos );

	void DrawSlotMouseOverImage();
	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );

	// 아이템 복구 관련
#ifdef ITEM_RECOVERY_TEST
	void GetRequiredRecoveryStoneID( IN const int iItemLevel_, OUT vector<int>& vecRecoveryItemIdList_ );
	void GetRequiredArmorOnlyRecoveryStoneID( IN const int iItemLevel_, OUT vector<int>& vecRecoveryItemIdList_ );
	void CheckRequiredRecoveryStoneInInventory( IN OUT vector<int>& vecRecoveryItemIdList_ );
	bool Handler_EGS_RESTORE_ITEM_REQ( UidType RecoveryItemUid );
	bool Handler_EGS_RESTORE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef ATTRACTION_ITEM_TEST
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD		/// 미니 얼음 조각상 가열기 처리를 위해, 오픈 이벤트 타입 추가
	void PlayGetAttractionItem( RANDOM_ITEM_OPEN_EVENT_TYPE eRandomItemEventType = RIOET_ICE_HEATER );
#else  SERV_SERV_MINI_RANDOM_CUBE_REWARD
	void PlayGetAttractionItem();
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
	void PlayGetAttractionItemGold();
	void UpdateOpenAttractionItemAck();
	bool IsEnchantCouponUseable(int CouponItemID, int TargetEnchantLevel );
	bool IsAttachItem( int ItemID );
	bool IsDefenceOnlyCoupon( int ItemID );

	bool Handler_EGS_ENCHANT_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID );
	bool Handler_EGS_ENCHANT_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_ATTRIBUTE_CHARM
	bool IsAttributeItem( int ItemID );
	wstring GetAttributeItemExtraDamageName( int itemTID );
	bool Handler_EGS_ATTRIB_ATTACH_ITEM_REQ( UidType CouponItemUID, UidType TargetItemUID );
	bool Handler_EGS_ATTRIB_ATTACH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_ATTRIBUTE_CHARM

#endif

#ifdef APRIL_5TH_TREE_DAY_EVENT			// 식목일 이벤트 관련 코드
	void UpdateOpenTreeDayItemAck();
	bool IsAprilEventRandomItem( int ItemID );
#endif

		//{{100416 김정협 트레져 박스 추가
#ifdef 	SERV_TREASURE_BOX_ITEM
	void UpdateOpenTreasureBoxItemAck();
	bool IsTreasureBoxRandomItem( int ItemID );
#endif 	SERV_TREASURE_BOX_ITEM
		//}}100416 김정협 트레져 박스 추가
	//}}
#ifdef	SERV_SHARING_BANK_TEST
	bool IsSealItem( CX2Item* pItem, int &iCanSealCount );	//	봉인 해야하는 아이템인지 체크
#endif	SERV_SHARING_BANK_TEST
		
	bool IsMonsterCardSetItem( int iItemID );

#ifdef AUTH_DELETE_ITEM

	//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
	bool Handler_EGS_DELETE_ITEM_REQ( UidType DeleteItemUid, int Quantity  );
#else REAL_TIME_ELSWORD
	bool Handler_EGS_DELETE_ITEM_REQ( UidType DeleteItemUid );
#endif REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

	bool Handler_EGS_DELETE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

	//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
	void OpenDeleteItemConfirmDLG();
	void OpenRegisterQuantityDLG( D3DXVECTOR2 pos );
	void UpdateRegisterQuantityDLG( bool bReadIME );
#endif REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

	//{{ kimhc // 2009-08-19 // 봉인시키는 아이템을 오른쪽 클릭 시
#ifdef	SEAL_ITEM
	void RClickItemForSeal( UidType itemUID );
	bool CanSealUpThisItem( CX2Item* pItem ) const;
	void ResetItemIDForSealAndCursorState();

	bool Handler_EGS_SEAL_ITEM_REQ( UidType itemUID );
	bool Handler_EGS_SEAL_ITEM_ACK( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UNSEAL_ITEM_REQ( UidType itemUID );
	bool Handler_EGS_UNSEAL_ITEM_ACK( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#endif	SEAL_ITEM
	//}} kimhc // 2009-08-19 // 봉인시키는 아이템을 오른쪽 클릭 시

	bool CheckRadioButtonBySortType( CX2Inventory::SORT_TYPE eType );


#ifdef	SERV_REFORM_INVENTORY_TEST
	void	SetShowInventoryPageUI(int iMaxPageNum = 0 );

#ifndef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	/// 외부 참조를 위해, Public으로 이동
	bool	UpdatePageUI( int iPage = 1 );
#endif //SERV_UPGRADE_TRADE_SYSTEM

	int		GetPageNumBySlotID( int iSlotID );

	int		GetMaxLineNumPerCategory( CX2Inventory::SORT_TYPE InventorySortType_ );	/// 현제 카테고리의 최대 슬롯 줄 수 반환

#ifndef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	/// 외부 참조를 위해, Public으로 이동 ( 현재 카테고리 페이지 수 반환 )
	int		GetNowInventoryPageNum( CX2Inventory::SORT_TYPE InventorySortType_ ) const { return m_NowInventoryPageNum[ InventorySortType_ - 1 ]; }
#endif //SERV_UPGRADE_TRADE_SYSTEM

	/// 현재 카테고리 페이지 수 설정
	void	SetNowInventoryPageNum( CX2Inventory::SORT_TYPE InventorySortType_, int iPageNum_ ) { m_NowInventoryPageNum[ InventorySortType_ - 1 ] = iPageNum_; }
#endif	SERV_REFORM_INVENTORY_TEST

#ifdef SERV_TOUR_TICKET_EVENT
	void	ShowTourEventDlg();
#endif SERV_TOUR_TICKET_EVENT

	//{{ 2012. 10. 08  할로윈 가열기
#ifdef SERV_HALLOWEEN_ICE_HEATER
	void UpdateOpenHalloWeenItemAck();
#endif SERV_HALLOWEEN_ICE_HEATER

protected:
	void UseDefenceDungeonPotion( const UidType uidItem_, const int iItemId_ );
#ifdef FREE_WARP
	void UseWarpItem( CX2SlotItem* pItemSlot_, bool bIsEdConsumption_ );
	void UseWarpPopup(bool bIsEdConsumption_);
#endif //FREE_WARP

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	void UseUSCJobItem( CX2SlotItem* pItemSlot_);
	void UseUSCJobPopup();
#endif //SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef PET_DROP_ITEM_PICKUP
	void SetPetAutoLooting( UidType uiPetUID_ );
#endif //PET_DROP_ITEM_PICKUP

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //미라클큐브
	bool IsRestoreEvaluateItem( const int ItemID_ ) const;
	void UseRestoreEvaluteItem( UidType iConsumeItemUID_ );

	bool Handler_EGS_RESTORE_ITEM_EVALUATE_REQ();
	bool Handler_EGS_RESTORE_ITEM_EVALUATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_ITEM_EVALUATE_REQ();
	bool Handler_EGS_ITEM_EVALUATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_ITEM_CONVERT_REQ( UidType iItemUID, int iQuantity );
	bool Handler_EGS_ITEM_CONVERT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05


#ifdef ADDED_ENCHANT_COUPON_HAVE_LEVEL_LIMIT    // 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적
	bool CheckLevelIfLevelLimitEnchantCoupon ( const int iCouponItemID_ , const int iTargetItemLevel_ );
#endif // ADDED_ENCHANT_ITEM_HAVE_LEVEL_LIMIT // 김종훈 / 13-07-03 / 레벨 제한을 가진 강화의 부적

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	void OpenElBreakWindow( IN CX2Item* pItem_ );
	void UpdateElBreakDLG( IN const bool bReadIME_ );
	bool Handler_EGS_ITEM_EXTRACT_REQ();
	bool Handler_EGS_ITEM_EXTRACT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //FINALITY_SKILL_SYSTEM

#ifdef GOOD_ELSWORD //JHKang
#ifndef NO_GOOD_ELSWORD_INT
	bool	Handler_EGS_GET_NEXT_INVENTORY_ED_REQ();
	bool	Handler_EGS_GET_NEXT_INVENTORY_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_EXPAND_INVENTORY_ED_REQ();
	bool	Handler_EGS_EXPAND_INVENTORY_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif NO_GOOD_ELSWORD_INT
#endif //GOOD_ELSWORD

#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
	bool IsAddSocketSlotItem( const int ItemID_ ) const;
	void UseAddSocketSlotItem( UidType iConsumeItemUID_ );

	void Handler_EGS_SOCKET_EXPAND_ITEM_REQ();
	bool Handler_EGS_SOCKET_EXPAND_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // ADD_SOCKET_SLOT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	void Handler_EGS_USE_PROPOSE_ITEM_REQ( UidType iItemUID, wstring wstrNickName );
	bool Handler_EGS_USE_PROPOSE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Handler_EGS_USE_DIVORCE_ITEM_REQ();
	bool Handler_EGS_USE_DIVORCE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_CUBE_IN_ITEM_MAPPING
	bool Handler_EGS_CUBE_IN_ITEM_MAPPING_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_RECRUIT_EVENT_BASE
	bool Handler_EGS_USE_RECRUIT_TICKET_REQ( UidType iUid );
	bool Handler_EGS_USE_RECRUIT_TICKET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_REGISTER_RECRUITER_REQ( const WCHAR* wszNickname, bool bCancel = false );
	bool Handler_EGS_REGISTER_RECRUITER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OpenRecommendMsgBox( bool bOpen );
#endif SERV_RECRUIT_EVENT_BASE

private:
	CX2Inventory::SORT_TYPE m_NowInventorySortType;

#ifdef	SERV_REFORM_INVENTORY_TEST
	int						m_NowInventoryPageNum[7];
#else	SERV_REFORM_INVENTORY_TEST
	int						m_NowInvenSortTypePageNum;
	int						m_NowInvenSortTypePageMaxNum;
#endif	SERV_REFORM_INVENTORY_TEST

	//CX2Inventory*			m_pInventory;
	D3DXVECTOR2				m_MovedPosition;

	CKTDGUIDialogType			m_pDLGUIInventory;
	CKTDGUIDialogType			m_pDLGResolveItem;
	CKTDGUIDialogType			m_pDLGResolveItemResult;
	CKTDGUIDialogType			m_pDLGSocketItem;
	CKTDGUIDialogType			m_pDLGSocketItemPushOk;
	CKTDGUIDialogType			m_pDLGSocketItemCheat;
	CKTDGUIDialogType			m_pDLGCubeCheck;
	CKTDGUIDialogType			m_pDLGElChanger;
	CKTDGUIDialogType			m_pDLGElChangerResult;

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	CKTDGUIDialogType			m_pDLGElBreak;			/// 엘 분해기
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_PSHOP_AGENCY
	CKTDGUIDialogType			m_pDLGCheckPShop;
#endif

#ifdef SERV_SOCKET_NEW
	CX2UISocketItem*			m_pUISocketItem;
#endif SERV_SOCKET_NEW

	//{{ kimhc // 실시간 엘소드 중 수량성 아이템 버리기
#ifdef REAL_TIME_ELSWORD
	CKTDGUIDialogType			m_pDLGItemEnroll;
	CKTDGUIDialogType			m_pDLGDeleteItemConfirm;
	int							m_iItemEnrollNum;			// 버릴 아이템의 수량
	UidType						m_EnrollItemUid;			// 버릴 아이템의 UidType
#endif REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 수량성 아이템 버리기

	//{{ kimhc // 2009-08-19 // 봉인의 인장 사용할 때 확인 창
#ifdef	SEAL_ITEM
	CKTDGUIDialogType			m_pDLGSealItemConfirm;
	CX2SlotItem*				m_pSlotItemToBeSealed;				// 봉인될 slotItem
	UidType						m_itemUIDForSeal;					// 사용한 고양이 발도장의 UID
	UidType						m_itemUIDToBeUnSealed;				// 봉인이 풀려야할 UID
	//}} kimhc // 2009-08-19 // 봉인의 인장 사용할 때 확인 창
#endif	SEAL_ITEM


	//{{ 2007. 10. 8  최육사  랜덤 아이템 다이얼로그 포인터
	CKTDGUIDialogType			m_pDLGRandomItem;
	//}}

	// 09.04.01 태완 : 랜덤아이템 연출 관련 코드 정리
	// 확인창을 여는 시점(우클릭이 된 시점)에 m_eRandomItemEventType의 값을 정의하고,
	// 확인창이 취소되는 시점 또는 연출 다 끝나고 확인 누를 때 m_eRandomItemEventType의 값을 NONE으로 다시 바꿔준다.
	RANDOM_ITEM_OPEN_EVENT_TYPE	m_eRandomItemEventType;

	UidType					m_iRandomItemUID;
	int						m_RandomItemID;

	bool					m_bShow;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	KEGS_OPEN_RANDOM_ITEM_ACK m_TempOpenRandomItemAck;

	// 감정 / 환원 관련
	UidType					m_ElChangerItemUID;
	int						m_ElChangerItemCount;

	// 분해관련
	UidType					m_DecompositionItemUID;
	int									m_DecompositionWindowNowPageNum;
	std::map< int, int >                m_mapResolveResultInsertedItem;

	// 소켓관련
	UidType					m_SocketItemUID;
	int						m_SocketSlotIndex;
	bool					m_bSocketRareMagicStone;
	bool					m_bSocketRemove;


#ifdef ATTRACTION_ITEM_TEST
	float					m_fPlayTime;

	//bool					m_bOpenedAttractionItem;
	bool					m_bPlayAttractionItem;
	CKTDGUIDialogType									m_pDLGOpenAttraction;
	CKTDGUIDialogType									m_pDLGAttractionResult;
	CKTDGUIDialogType									m_pDLGGetAttractionItemBG;	

	CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstMeltMachine;
	CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMeshInstMeltMachineStart;

	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSpreadLight;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEndLight;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEndLight2;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEndLight3;

	// 강화권관련
	UidType					m_CouponUid;
	UidType					m_CouponTargetUid;
#endif

#ifdef APRIL_5TH_TREE_DAY_EVENT			// 식목일 이벤트 관련 코드
	CKTDGUIDialogType			m_pDLGTreeDay1;
	CKTDGUIDialogType			m_pDLGTreeDay2;
	bool					m_bPlayingTreedayEvent;
	bool					m_bAprilSoundPlayed;

#endif

	//{{100416 김정협 트레져 박스 추가
#ifdef 	SERV_TREASURE_BOX_ITEM
	CKTDGUIDialogType			m_pDLGTreasureBox1;
	CKTDGUIDialogType			m_pDLGTreasureBox2;
	CKTDGUIDialogType			m_pDLGTreasureBox3;
	CKTDGUIDialogType			m_pDLGTreasureBox4;
	CKTDGUIDialogType			m_pDLGTreasureBox5;
	CKTDGUIDialogType			m_pDLGTreasureBox6;
	CKTDGUIDialogType			m_pDLGTreasureBox7;
	CKTDGUIDialogType			m_pDLGTreasureBox8;
	CKTDGUIDialogType			m_pDLGTreasureBox9;
	CKTDGUIDialogType			m_pDLGTreasureBox_White;

	bool						m_bPlayingTreasureBox;
	bool						m_bTreasureBoxSoundPlayed;

#ifdef TREASURE_BOX_ITEM_THIN
	int							m_TreasureBoxSPItemID;
#endif TREASURE_BOX_ITEM_THIN
#endif 	SERV_TREASURE_BOX_ITEM
	//}}100416 김정협 트레져 박스 추가

#ifdef ITEM_RECOVERY_TEST
	UidType					m_RecoveryItemUid;
#endif

#ifdef AUTH_RANDOM_ITEM_AUTO_OPEN
	bool					m_bUsingRandomItemAutoOpen;
	int						m_OpenedRandomItemCount;
	CKTDGUIDialogType			m_pDLGAutoOpenMsgBox;
	std::map< int, int >	m_MapAutoRandomItemGet;
#endif

#ifdef RANDOM_CUBE_UI_NEW
	CKTDGUIDialogType		m_pDLGCubeOpenEffect;
	float					m_fCubeOpenTime;
	bool					m_bCubeJustOpen;
	bool					m_bCubeOpen;
	std::map< int, int >	m_mapCubeItemGet;
#endif RANDOM_CUBE_UI_NEW
	UidType					m_WarpItem;

#ifdef BUFF_TEMPLET_SYSTEM
	UidType					m_DefencedungeonPotion;
	int						m_DefencedungeonPotionID;
#endif BUFF_TEMPLET_SYSTEM

	// 새로운 아이템이 있는지에 대한 여부를 설정
	bool					m_bIsNewItem;

	UidType					m_UseItemTempUID;
	UidType					m_UsedItemUID;
	UidType					m_TitleItemUID;

#ifdef FREE_WARP
	CKTDGUIDialogType			m_pDLGWarpDestination;
	std::vector<CKTDGUIButton*> m_vecWarpListButton;
	int							m_iSelectedWarpDest;
	SHORT						m_SumDelta;

	int							m_iSelectedWarpDestIndex;//현재 위치와 거리를 알기 위해 인덱스 저장
	int							m_iCurrentVillageWarpIndex; //현재 위치 인덱스로 저장
	bool						m_bIsEdConsumptionWarp;		//워프아이템 ED소모여부
#endif

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	CKTDGUIDialogType			m_pDLGUSCJob;
	std::vector<CKTDGUIButton*> m_vecUSCJobButton;
	int							m_iSelectedUSCJob;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	UidType						m_UseTransformdItemUID;
	UidType						m_UseTransformItemPetUID;
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

	CKTDGUIDialogType		m_pDLGIdentifyStone;

#ifdef SERV_PET_SYSTEM
	CX2UIPetInventory*		m_pUIPetInventory;
	bool					m_bAutoFeed;

	CX2SlotItem*			m_pPetFoodSlot;
#endif

#ifdef RANDOM_EXCHANGE_RESULT_VIEW
	ITEM_OBTAIN_RESULT_TYPE	m_eItemObtainResultType; 
#endif RANDOM_EXCHANGE_RESULT_VIEW

#ifdef NEW_ITEM_NOTICE
	CKTDGUIDialogType		m_pDLGItemHighLight;
#endif //NEW_ITEM_NOTICE
#ifdef FIX_RANDOM_CUBE_OPEN_RESULT_BUG
	bool					m_bProcessCubeOpen;			//큐브 오픈 동작 처리중 여부
#endif //FIX_RANDOM_CUBE_OPEN_RESULT_BUG

#ifdef	SERV_SHARING_BANK_TEST
public:
	KEGS_CHANGE_INVENTORY_SLOT_ITEM_REQ	m_kTempChangeSlotInfo;
#endif	SERV_SHARING_BANK_TEST

#ifdef CUBE_OPEN_IMAGE_MANAGER
	bool					m_bHideCubeOpneBar;			// 큐브 오픈 동장에서 진행 바 표시여부
	CKTDGUIDialogType		m_pDLGEventCubeResultImage;					// 결과 이미지 UI
#endif // CUBE_OPEN_IMAGE_MANAGER

#ifdef SPT_NONE_ITEM_DELETE
	bool					m_bIsSptNoneItem;
	CKTDGUIDialogType		m_pSptNoneItemDelete;
#endif //SPT_NONE_ITEM_DELETE

#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
	int						m_iUseOpenRandomItemID;		// 마우스 오른 클릭으로 열었던 아이템 큐브 ID 임시 저장
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM

#ifdef SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
	bool					m_bIsAllInOne;
#endif //SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD
#ifdef RIDING_SYSTEM
	bool					m_bIsRidingGacha;
#endif //RIDING_SYSTEM

#ifdef SERV_HALLOWEEN_ICE_HEATER
	bool					m_bIsHalloweenIceHeater;
#endif //SERV_HALLOWEEN_ICE_HEATER

#ifdef FIX_ICE_HEATER_EVENT

	map<RANDOM_ITEM_OPEN_EVENT_TYPE, IceHeaterEventData*>	m_mapIceHeaterEventData;		/// 가열기 재생 정보값 저장 맵
	IceHeaterEventData*										m_pNowIceHeaterEventData;		/// 현재 사용중인 가열기 재생 정보값
	CX2EffectSet::Handle									m_hUIEffectSetWait;		/// 현재 가열기에서 사용중인 대기 UIEffectSet 핸들
	CX2EffectSet::Handle									m_hUIEffectSetPlay;		/// 현재 가열기에서 사용중인 재생 UIEffectSet 핸들
#endif // FIX_ICE_HEATER_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //미라클큐브
	UidType					m_iConsumeItemUID;
	UidType					m_iTargetItemUID;
	int						m_iConsumeItemQuantity;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef RIDING_ITEM_POPUP_CHECK
	UidType					m_iRidingItemUid;
#endif //RIDING_ITEM_POPUP_CHECK

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	UidType		m_ElBreakItemUID;
	int			m_ElBreakItemCount;
#endif //FINALITY_SKILL_SYSTEM

#ifdef GOOD_ELSWORD //JHKang
	int					m_iConfirmED;
	CKTDGUIDialogType	m_pDLGBuyConfirm;
#endif //GOOD_ELSWORD

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	float		m_fLeftMouseWhellCoolTime;
#endif // CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef SERV_TOUR_TICKET_EVENT
	CKTDGUIDialogType		m_pDLGTourEventDlg;
	bool					m_bPlayTourEvent;
	float					m_fPlayTourEventDlgTime;
#endif SERV_TOUR_TICKET_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	CKTDGUIDialogType		m_pDLGMarryEventNickName;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
	CKTDGUIDialogType		m_pDLGRecommend;
#endif SERV_RECRUIT_EVENT_BASE
};

#endif

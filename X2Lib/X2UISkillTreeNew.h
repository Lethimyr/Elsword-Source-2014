#ifdef UPGRADE_SKILL_SYSTEM_2013

#pragma once

class CX2EquippedSkillSlotData;
class CX2SkillTreeSlotData;

class CX2UISkillTreeNew
{
public:

	enum SKILL_TREE_UI_CUSTOM_MSG
	{
		STUCM_EXIT	= 29000,
	
		STUCM_BUTTON_SKILL_LEVEL_PLUS,
		STUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOVER,
		STUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOUT,

		STUCM_BUTTON_SKILL_LEVEL_MINUS,
		STUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOVER,
		STUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOUT,

		STUCM_BUTTON_SKILL_MASTER_MOUSEOVER,
		STUCM_BUTTON_SKILL_MASTER_MOUSEOUT,

		STUCM_EQUIPSLOT_DROPPED,
		STUCM_EQUIPSLOT_RMOUSEUP,
		STUCM_EQUIPSLOT_MOUSEIN,
		STUCM_EQUIPSLOT_MOUSEOUT,	

		STUCM_SKILLTREESLOT_RMOUSEUP,
		STUCM_SKILLTREESLOT_MOUSEIN,
		STUCM_SKILLTREESLOT_MOUSEOUT,

		STUCM_SKILL_RESET_MODE_CHANGE,
		STUCM_SKILL_RESET_MODE_CHANGE_MOUSEOVER,
		STUCM_SKILL_RESET_MODE_CHANGE_MOUSEOUT,

		STUCM_CONTROLLIST_SCROLL,

		STUCM_NAVI_LEFT_CLICKED,
		STUCM_NAVI_DRAGGING,

		STUCM_INGAME_ICON_MOUSE_OVER,
		STUCM_INGAME_ICON_MOUSE_OUT,

		STUCM_FIRST_GUIDE_CLOSE,
		STUCM_FIRST_GUIDE_DONOTSHOW,

		STUCM_CSP_GUIDE_MOUSEIN,
		STUCM_CSP_GUIDE_MOUSEOUT,
		STUCM_SKILLTREESLOTB_GUIDE_MOUSEIN,
		STUCM_SKILLTREESLOTB_GUIDE_MOUSEOUT,

		STUCM_UNSEAL_SKILL_CANCLE,
		STUCM_UNSEAL_SKILL_BUY,

		STUCM_OPEN_COMBO_TREE,

		STUCM_INGAME_ELSWORD_WSS_MOUSE_OVER,
		STUCM_INGAME_ELSWORD_WSS_MOUSE_OUT,

//#ifdef GUIDE_SKILL_SLOTB
		STUCM_SKILLTREESLOTB_BUY_GUIDE_MOUSEIN,
		STUCM_SKILLTREESLOTB_BUY_GUIDE_MOUSEOUT,
//#endif GUIDE_SKILL_SLOTB
//#ifdef GUIDE_SKILL_LEVEL_MINUS
		STUCM_BUTTON_SKILL_LEVEL_MINUS_GUIDE_MOUSEOVER,
		STUCM_BUTTON_SKILL_LEVEL_MINUS_GUIDE_MOUSEOUT, 
//#endif GUIDE_SKILL_LEVEL_MINUS

		STUCM_BUTTON_SKILL_LEVEL_PLUS_RESULT,		/// 결정하기 버튼 눌렀을 때
		STUCM_BUTTON_SKILL_LEVEL_PLUS_CANCEL,		/// 취소 버튼을 눌렀을 때

		STUCM_CHANGE_SKILL_LEVEL_PLUS,				/// 스킬 습득 처리
		STUCM_BUTTON_SKILL_LEVEL_UPGRADE,			/// 스킬 강화 버튼을 눌렀을 때
		STUCM_BUTTON_EXIT,							/// 닫기 버튼을 눌렀을 때
		STUCM_EXIT_WITH_SKILL_LEVEL_PLUS,			/// 닫으면서 스킬 레벨 증가 처리

		STUCM_BUTTON_ENABLE_INIT_SKILL,				/// 스킬 트리 내 초기화 버튼
		STUCM_BUTTON_INIT_SKILL,					/// 스킬 슬롯 초기화 버튼
		STUCM_INIT_SKILL,							/// 스킬 슬롯 초기화 처리 ( 0으로 )
		STUCM_INIT_SKILL_ONE_POINT,					/// 스킬 슬롯 초기화 처리 ( 1로 )
#ifdef SKILL_PAGE_SYSTEM //JHKang
		STUCM_SKILL_PAGE1,			/// 페이지 1 선택 (미리보기)
		STUCM_SKILL_PAGE2,			/// 페이지 2 선택 (미리보기)
		STUCM_SKILL_PAGE3,			/// 페이지 3 선택 (미리보기)
		STUCM_ADD_SKILL_PAGE_ED,	/// 페이지 추가 ED
		STUCM_ADD_SKILL_PAGE_CASH,	/// 페이지 추가 CASH
		STUCM_DECIDE_SKILL_PAGE,	/// 페이지 결정
		STUCM_BUY_CONFIRM,			/// 페이지 구입
		STUCM_BUY_CANCEL,			/// 페이지 구입 취소

		STUCM_DECIDE_WITH_SKILL_LEVEL_PLUS,			/// 스킬 값 및 페이지 변경
		STUCM_CANCEL_DECIDE_WITH_SKILL_LEVEL_PLUS,	/// 스킬 값 적용 취소 및 변경
#endif //SKILL_PAGE_SYSTEM
	};

	/// 스킬 트리에 있는 스킬 슬롯 타입
	enum SKILL_TREE_SLOT_TYPE
	{
		STST_NONE				= -1,
		STST_SELECT_SKILL_LEFT	= 0,	/// 2지선다 왼쪽
		STST_SELECT_SKILL_RIGHT	= 1,	/// 2지선다 오른쪽
		STST_SINGLE_SKILL		= 2,	/// 가장 우측 싱글 스킬 
		STST_END				= 3,
	};

	/// 스킬 트리에 있는 스킬 슬롯의 버튼 타입
	enum SKILL_TREE_SLOT_BUTTON_TYPE
	{
		STSBT_NONE					= -1,
		STSBT_LEARN_SKILL			= 0,	/// 스킬 습득 버튼
		STSBT_UPGRADE_SKILL			= 1,	/// 스킬 강화 버튼
		STSBT_CAN_NOT_LEARN_SKILL	= 2,	/// 스킬 습득 불가 버튼
		STSBT_INIT_SKILL			= 3,	/// 스킬 초기화 버튼
		STSBT_END					= 5,
	};

	struct SkillSlotUI
	{
		CKTDGUISlot*	m_pSlot;					/// 스킬 슬롯
		CKTDGUIStatic*	m_pStaticBG;				/// 배경 백판
		CKTDGUIStatic*	m_pStaticSkillBG;			/// 스킬 백판
		CKTDGUIStatic*	m_pStaticSkillLock;			/// 잠김 스킬
		CKTDGUIButton*	m_pButtonSkillInit;			/// 스킬 초기화 버튼
		CKTDGUIButton*	m_pButtonSkillDesc;			/// 스킬 툴팁 표시용 버튼
		CKTDGUIButton*	m_pButtonSkillLearn;		/// 스킬 습득 버튼
		CKTDGUIStatic*	m_pStaticSkillMaster;		/// 스킬 마스터 이미지
		CKTDGUIStatic*	m_pStaticSkillNoLearn;		/// 스킬 습득 불가 이미지
		CKTDGUIStatic*	m_pStaticStringRequireSP;	/// 요구 SP
		bool			m_bSelectSkillBlind;		/// 2지선다 스킬중, 선택되지 않아 블라인드 처리해야 할 여부
		bool			m_bSkillLineBlind;			/// 조건이 충족되지 않아 잠겨버린 스킬 라인 처리 여부
		

		SkillSlotUI()
		{
			m_pSlot						= NULL;
			m_pStaticBG					= NULL;
			m_pStaticSkillBG			= NULL;
			m_pStaticSkillLock			= NULL;
			m_pButtonSkillInit			= NULL;
			m_pButtonSkillDesc			= NULL;
			m_pButtonSkillLearn			= NULL;
			m_pStaticSkillMaster		= NULL;
			m_pStaticSkillNoLearn		= NULL;
			m_pStaticStringRequireSP	= NULL;
			m_bSelectSkillBlind			= false;
			m_bSkillLineBlind			= false;
		}

	public:
		void SetSkillLineBlind(bool val) { m_bSkillLineBlind = val; }

		// 소멸자는 필요없다 : 전부 Dialog 소멸자에서 사라짐
	};

	struct SkillPositionKey
	{
		int	m_iRow;		/// 가로 위치
		int	m_iCol;		/// 세로 위치

		SkillPositionKey() :
		m_iRow( 0 ),
		m_iCol( 0 )
		{
		}

		SkillPositionKey( int iRow, int iCol )
		{
			m_iRow = iRow;
			m_iCol = iCol;
		}

		bool operator < ( const SkillPositionKey& rhs ) const 
		{

			if( m_iRow < rhs.m_iRow )
				return true;
			else if( m_iRow > rhs.m_iRow )
				return false;

			if( m_iCol < rhs.m_iCol )
				return true;

			
			return false;
		}
	};

public:

	CX2UISkillTreeNew( CKTDXStage* pNowStage );
	virtual ~CX2UISkillTreeNew(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool GetShow() { return m_bShow; }
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vec );

	void InitSkillTreeUI();
	void UpdateSPInfo();
	
	void InitializeSkillEquipSlot();
	CX2EquippedSkillSlotData* GetEquippedSlotData( int index, bool bSlotB = false );
#ifdef REFORM_UI_SKILLSLOT
	CKTDGUISlot* GetEquippedSlot( int index, bool bSlotB = false );
#endif //REFORM_UI_SKILLSLOT
	void UpdateSkillEquipedSlotUI();
	
	void CreateSkillTree( CX2Unit::UNIT_CLASS eUnitClass );
	void UpdateSkillTree();
	void UpdateSkillTree( CX2SkillTree::SKILL_ID eSkillID );			// Tree에서 해당 Slot만 업데이트한다
	void UpdateBlind();
#ifdef SKILL_PAGE_SYSTEM //JHKang
	void UpdateUI( bool bEquipped, bool bSP = true, bool bSkillTree = false, bool bSkillPage_ = false );
	void BuySkillPageDialog( const int iED_ );
#else //SKILL_PAGE_SYSTEM
	void UpdateUI( bool bEquipped, bool bSP = true, bool bSkillTree = false );
#endif //SKILL_PAGE_SYSTEM
    
	void CreateSlotUIPreset( SkillSlotUI& UISet, int eSkillID, int iCol );

	const map<CX2SkillTree::SKILL_ID, SkillSlotUI>& GetMapSkillSlotUISet(){ return m_mapSkillSlotUISet; }

	bool GetRemoveMode(){ return m_bSkillRemoveMode; }
	void InitEquippedSkillSlotData();

	void UpdateSkillCoolTime( float fElapsedTime );
	//void UpdateSkillSlotPosition();
	bool GetSkillSlotSwap()const {return m_bSkillSlotSwap;}
	void SwapSkillSlot(bool bVal, bool bForce = false);
	void SetOffsetPosSkillSLotStatic(const D3DXVECTOR2& vPos_, const D3DXVECTOR2& vAddPos_ = D3DXVECTOR2(0,0));
	void SetOffsetStatic( const WCHAR* strControl_, const D3DXVECTOR2& vPos_);

	//////////////////////////////////////////////////////////////////////////

	//** fix!! 스킬 봉인해제 다이얼로그를 여는 함수. 추가 작업 필요합니다.
	void OpenSkillUnsealInformDLG( const CX2SkillTree::SkillTemplet* pSkillTemplet, D3DXVECTOR2 SlotCenterPos );	

	void OpenDLGSkillUnsealed( bool bOpen );
	void UpdateDLGSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID );


	
	//////////////////////////////////////////////////////////////////////////
	void GetPrecedingSkillDesc( OUT wstring& wstrDesc, CX2SkillTree::SKILL_ID ePrecedingSkill ) const;
	void ShowSkillDesc( bool bShow, CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE, D3DXVECTOR2 pos = D3DXVECTOR2(0,0), 
						D3DXVECTOR2 size = D3DXVECTOR2(0,0), bool bEquipped = false );
	void GetSkillEffectiveDesc( OUT wstring& wstrDesc, IN int iSkillLevel, IN int iIncreasekillLevel, IN const CX2SkillTree::SkillTemplet* pSkillTemplet ) const;
	void GetSkillTitleDesc( OUT wstring& wstrTitleDesc, IN const CX2SkillTree::SkillTemplet* pSkillTemplet, 
							OUT const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet,IN int iSkillLevel, IN int iIncreasekillLevel ) const;
	void GetSkillSubDesc( IN const CX2SkillTree::SkillTemplet* pSkillTemplet, IN const int iTotalSkillLevel, OUT wstring& wstrSubDesc ) const;
	void GetSkillUnSealedDesc( OUT wstring& wstrDesc, IN const CX2SkillTree::SKILL_ID eSkillID ) const;

	void ShowUIDesc( bool bShow, wstring wstr = L"", D3DXVECTOR2 pos = D3DXVECTOR2(0,0), 
					 CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT = CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT,
					 D3DXCOLOR coTextColor = D3DXCOLOR(0,0,0,1), D3DXCOLOR coBackColor = D3DXCOLOR(1, 0.96f, 0.6f, 1 ) );
	//wstring GetExpirationDateDesc( const wstring& wstrExpirationDate );	// kimhc // KTDXCommonFunc로 옮김 // 2009-06-16
	
	//////////////////////////////////////////////////////////////////////////

//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
	bool ProcessSkillTreeSlotRMouseUp( LPARAM lParam );	// 스킬트리에서 마우스 우클릭 했을 때의 처리
	bool ProcessEquipSlotDropped( LPARAM lParam );		// 장착슬롯으로 드래그앤 드랍 했을 때의 처리
	bool ProcessEquipSlotRMouseUp( LPARAM lParam );		// 장착슬롯에서 마우스 우클릭 했을 때의 처리

// 	bool CheckIsReady();					// 현재 레디 상태이면 true, 레디 상태가 아니면 false 를 리턴
// 	void ShowOkAndCancelMsgForUnReady( LPARAM lParam, SKILL_TREE_UI_CUSTOM_MSG eSTUCM ); // 레디 상태를 풀겠냐는 ok, cancel 메시지 박스, 인자는 장착하려는 UICustomMSG에서 넘겨준 lParam값(슬롯)	

	//void ProcessChangeEquipSlotAfterUnReady();
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정

#ifdef REFORM_UI_SKILLSLOT 
	CKTDGUIDialogType	GetDLGGameSkillSLot() {return m_pDLGGameSkillSlot; }
	bool				IsExpandSkillSlot()	{return m_bIsExpandSkillSlot; }
	bool				GetShowEquipSkillGuide() {return m_bShowEquipSkillGuide; }
#endif //REFORM_UI_SKILLSLOT

	void UpdateSkillSlotString();

#ifdef SKILL_SLOT_UI_TYPE_B
	void				SetSkillUIType( bool bIsTypeA_ );
	bool				GetIsSkillUiTypeA() const { return m_bIsSkillUITypeA; }
	void				SwapSkillSlotTypeB( bool bVal_, bool bForce_= false);
	void				SwapSkillSlotTypeA( bool bVal_, bool bForce_= false);
	void				SetOffsetPosEtcSkillSlotUI();
#endif //SKILL_SLOT_UI_TYPE_B

	/// 현재 스킬창에서 스킬 레벨을 올리고 있는가
	bool GetNowLearnSkill() const { return m_bNowLearnSkill; }

	/// 현재 스킬창에서 스킬 레벨을 초기화 하고 있는가
	bool GetNowInitSkill() const { return m_bNowInitSkill; }

	bool													IsLearnBeforeSelectSkill( const int iNowTierIndex );						/// 이전 단계의 2지선다 스킬을 배웠는지 여부를 반환

	const int												GetNotLearnTierIndex();														/// 배우지 못한 최상위 스킬 라인 반환
	const int												GetNotEnoughLevelTierIndex( const int iLevel );								/// 배울 수 없는 레벨의 최상위 스킬 라인 반환
	const map<SkillPositionKey,	CX2SkillTree::SKILL_ID>&	GetMapSkillPosition() { return m_mapSkillPosition; }						/// 위치에 따른 스킬 아이디 저장 컨테이너 반환
	const CKTDGUIDialogType									GetDLGUISkillTree() { return m_pDLGUISkillTree; }							/// 스킬 트리 다이얼로그 객체 반환
	const int												GetSkillLevelInSkillWimdow( const CX2SkillTree::SKILL_ID eSkillID ) const;	/// 스킬창 내 스킬 아이디 반환
	const CX2SkillTree::SKILL_ID							GetAnotherSelectSkillID( CX2SkillTree::SKILL_ID eCheckSkillID ) const;		/// 2지선다 스킬 중 반대편에 있는 스킬 아이디를 반환
	const int												GetRequireSkillLineLevel( int iSkillLineTierIndex ) const					/// 스킬 Tire에 따른 스킬 습득 요구 레벨
	{ 
		if ( iSkillLineTierIndex >= ARRAY_SIZE( OPEN_SKILL_TREE_LEVEL ) )
		{
			/// 각 라인별 습득 레벨 배열보다 큰 라인 인덱스를 참조하려고 해요!!!
			ASSERT( !L"NowSkillLineLevel Is Large Then OPEN_SKILL_TREE_LEVEL Array Size!!!" );
			return 0;
		}

		return OPEN_SKILL_TREE_LEVEL[iSkillLineTierIndex];
	}

private:
#ifdef ELSWORD_WAY_OF_SWORD
	void					DrawElswordWSSDesc( D3DXVECTOR2 vDrawPos_ );
#endif ELSWORD_WAY_OF_SWORD

	void					CreateSkillTreePortrait( CKTDGUIContolList* pControlList );									/// 스킬 트리 초상화 및 구분선 설정 함수

	void					UpdateAtMoveScrollBar();																	/// 스크롤 바가 움직일 때 변경되어야 할 정보를 처리하는 함수

	void					SetSelectSkillSlotPosition();																/// 2지선다 스킬의 백판 변경 함수
	void					SetDivisionLine( CKTDGUIContolList* pControlList );											/// 구분선 설정 함수
	void					SetLearnSkillInfo( CX2SkillTree::SKILL_ID eSkillID );										/// 스킬 습득을 위한 KGetSkillInfo 값 설정 함수
	void					SetBlindSelectSkill();																		/// 2지선다 스킬 중 선택되지 않은 스킬에 대한 블라인드 설정
	void					SetSkillSlotButton( CKTDGUIButton* pSlotButton, SKILL_TREE_SLOT_BUTTON_TYPE eButtonType );	/// 스킬 트리의 스킬 슬롯 버튼 설정 함수
	void					SetUnitClassImageAndName(	CKTDGUIStatic* pStatic,											/// 스킬 트리 초상화의 전직 이미지 및 전직명 설정 함수
														CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_ARME_VOID_PRINCESS, 
														CX2Unit::UNIT_CLASS_TYPE UnitClassLevel = CX2Unit::UCT_BASIC_CLASS );
#ifdef SKILL_PAGE_SYSTEM //JHKang
	void UpdateSkillPageUI();
#endif //SKILL_PAGE_SYSTEM

private:
	bool					m_bShow;
	bool					m_bSkillRemoveMode;
	bool					m_bClassInfoVaild;
	//D3DXVECTOR2				m_MovedPosition;
	//D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGUISkillTree;   
	CKTDGUIDialogType			m_pDLGSkillToolTip;	
	CKTDGUIDialogType			m_pDLGSkillUnsealInform;	
	CKTDGUIDialogType			m_pDLGSkillUnsealed;		// 스킬 봉인해제 완료 팝업
#ifdef ELSWORD_WAY_OF_SWORD
	CKTDGUIDialogType			m_pDLGElswordWSSDesc;
#endif ELSWORD_WAY_OF_SWORD


	map<CX2SkillTree::SKILL_ID, SkillSlotUI>		m_mapSkillSlotUISet;
	map<SkillPositionKey, CX2SkillTree::SKILL_ID>	m_mapSkillPosition;

	const CX2UserSkillTree*		m_pUserSkillTree;
		
	// 마우스 휠 관련
	SHORT				m_SumDelta;
	int					m_iPrevSP;	// UpdateSP를 해 줄때 이전 SP 상태와 비교해서 Tree Update가 필요할지 결정한다

	// Navigation 관련
	float				m_fNaviMoveRange;

	// ToolTip : TalkBox 이용해서.
	CX2TalkBoxManager*		m_pTalkBoxManager;
	bool					m_bUsingCSP;

#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
	CKTDGUIDialogType		m_pDLGFirstGuide;
#endif

#ifdef REFORM_UI_SKILLSLOT
	bool					m_bIsExpandSkillSlot;
	CKTDGUIDialogType		m_pDLGGameSkillSlot;
	bool					m_bShowEquipSkillGuide;
#endif //REFORM_UI_SKILLSLOT

	bool					m_bSkillSlotSwap;			/// 스킬 슬롯 스왑 시도

#ifdef SKILL_SLOT_UI_TYPE_B
	bool					m_bIsSkillUITypeA;			/// 스킬 슬롯 타입
#endif //SKILL_SLOT_UI_TYPE_B

	int						m_iMaxSkillRowNum;			/// 최대 스킬 배열 높이

	bool					m_bNowLearnSkill;			/// 현재 스킬창에서 스킬 레벨을 올리고 있는가
	bool					m_bNowInitSkill;			/// 현재 스킬창에서 스킬 레벨을 초기화 하고 있는가

	CX2SkillTree::SKILL_ID	m_eCheckSkillID;			/// 메시지 이벤트를 통해 변경될 스킬 아이디

	bool					m_bReplacedEsCounter;		/// 빈틈없는 반격용 하드 코딩 코드

#ifdef SKILL_PAGE_SYSTEM //JHKang
	CKTDGUIDialogType	m_pDLGBuyConfirm;	/// 스킬 페이지 확장 다이얼로그
	int					m_iConfirmED;		/// 스킬 페이지 확장시 요구 금액
#endif //SKILL_PAGE_SYSTEM
};

//////////////////////////////////////////////////////////////////////////

class CX2SkillTreeSlotData : public CKTDGUISlotData
{

public:

	enum SKILL_TREE_SLOT_ADD_PICTURE_TYPE
	{
		STSAPT_DISABLE = 0,		/// 어두운 배경
		STSAPT_EQUIPPED,		/// 장착중 
		STSAPT_SEALED,			/// 봉인 마크
		STSAPT_LEVEL,			/// 스킬 레벨
		STSAPT_CANLEARN,		/// 습득 가능
		STSAPT_SELECT,			/// 선택 가능

		STSAPT_END,
	};

	CX2SkillTreeSlotData();
	virtual ~CX2SkillTreeSlotData();

	virtual void UpdateUI( CKTDGUISlot* pSlot );
	void ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_TREE_SLOT_ADD_PICTURE_TYPE eType, int val = -1 );

	//CX2SkillTree::SkillTreeTemplet*	pSkillTemplet;
	CX2SkillTree::SKILL_ID m_eSkillID;
};

class CX2EquippedSkillSlotData : public CKTDGUISlotData
{
	enum SKILL_EQUIPPED_SLOT_ADD_PICTURE_TYPE
	{
		SESAPT_DISABLE = 0,
		SESAPT_LEVEL,

		STSAPT_END,
	};

public:
	CX2EquippedSkillSlotData();
	CX2EquippedSkillSlotData(int index, bool bSlotb );
	virtual ~CX2EquippedSkillSlotData();

	int GetSlotID();		// 0~8로 계산되는 ID를 돌려준다
	void ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_EQUIPPED_SLOT_ADD_PICTURE_TYPE eType, int val = -1, bool bIsSkillLevelUpByItem = false );

	virtual void UpdateUI( CKTDGUISlot* pSlot );
	CX2SkillTree::SKILL_ID m_eSkillID;
	int	m_iIndex;
	bool m_bSlotb;



};

#endif // UPGRADE_SKILL_SYSTEM_2013
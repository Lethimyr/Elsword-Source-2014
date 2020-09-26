#pragma once

#ifdef SERV_EPIC_QUEST
namespace		_CONST_UIQUESTNEW_INFO_
{
	const	UINT		g_iNumQuestSlotPerPage		= 13;		// 한 페이지 당 나타낼 수 있는 최대 퀘스트 정보 갯수
	const	int			g_iRewardSlotNum			= 3;		// 보상 슬롯 갯수

	const	int			g_iMaxNumOngoingQuest		= 6;

	const	float		g_fQuickQuestMinimizeSizeY	= 22.f;

}





//class CX2StateMenu;
class CX2UIQuestNew : public CX2ItemSlotManager
{
	enum UI_QUEST_NEW_CUSTOM_MSG
	{
		UQNCM_EXIT				= 41000,
		UQNCM_QUEST_SELECT,		//퀘스트 선택
		UQNCM_QUEST_ACCEPT,		//퀘스트 수락
		UQNCM_QUEST_COMPLETE,	//퀘스트 완료
		UQNCM_QUEST_GIVE_UP,	//퀘스트 포기
		UQNCM_QUEST_GIVE_UP_CONFIRM, //퀘스트 포기_확인

		UQNCM_QUESTCLEAR_OK,

		UQNCM_SCROOL_UP,		//스크롤
		UQNCM_SCROOL_DOWN,

		UQNCM_REWARD_CHECK_ITEM,	//보상
		UQNCM_REWARD_UNCHECK_ITEM,
		UQNCM_REWARD_MOUSE_OVER,
		UQNCM_REWARD_MOUSE_OUT,

		UQNCM_NAVI_LCLICKED,
		UQNCM_NAVI_DRAGGING,

		UQNCM_QUEST_ACCEPT_CONFIRM,
		UQNCM_QUEST_ACCEPT_CANCLE,
		UQNCM_QUEST_GIVE_UP_CANCLE,

		UQNCM_QUEST_ADMIN_COMPLETE,

		UQNCM_QUEST_DESC_UP,		//사용하지 않는 항목
		UQNCM_QUEST_DESC_DOWN,		//사용하지 않는 항목

		UQNCM_EXIT2,				//사용하지 않는 항목

		UQNCM_CATEGORY_SELECT,
		UQNCM_BACK_TO_QUEST_LIST,	//목록으로

		UQNCM_QUICK_TO_EXPAND,
		UQNCM_QUICK_TO_MINIMIZE,
		UQNCM_QUICK_EXPAND_MOUSE_OVER,
		UQNCM_QUICK_EXPAND_MOUSE_OUT,

		UQNCM_SELECT_QUEST_NORMAL_TAB,
		UQNCM_SELECT_QUEST_SCENARIO_TAB,

		UQNCM_QUEST_REMIND,
		UQNCM_CHECK_QUICK_QUEST,
		UQNCM_QUEST_SELECT_MOUSE_OVER,
		UQNCM_QUICK_TO_DETAIL,
		UQNCM_QUEST_COMPLETE_MOUSE_OVER,
		UQNCM_TOOLTIP_MOUSE_OUT,

//#ifdef REFORM_QUEST
		UQNCM_SELECT_QUEST_EVENT_TAB,
		UQNCM_QUEST_INSTANTLY_COMPLETE,
//#endif //REFORM_QUEST

	};

	struct QuestListSlot
	{
		int					m_QuestID;
		CKTDGUIRadioButton*	m_pButton;
		CKTDGUIStatic*		m_pStatic;
	};


public:

	CX2UIQuestNew( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIQuestNew(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	bool GetShowClearDLG(){ return m_bShowClearDLG; }
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vec );
	

	void ShowNextButton(bool val);



	//////////////////////////////////////////////////////////////////////////

	bool Handler_EGS_TALK_WITH_NPC_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID );
	bool Handler_EGS_TALK_WITH_NPC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_NEW_QUEST_REQ( int iQuestID );
	bool Handler_EGS_NEW_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_NEW_QUEST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_GIVE_UP_QUEST_REQ( int questID );
#ifdef SERV_GATHER_GIVE_UP_QUEST 
	bool Handler_EGS_GATHER_GIVE_UP_QUEST_REQ( std::vector< int >& vecGatherQuestID );
#endif // SERV_GATHER_GIVE_UP_QUEST
	bool Handler_EGS_GIVE_UP_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID );
	bool Handler_EGS_QUEST_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ADMIN_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID );

#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool Handler_EGS_ALL_COMPLETED_QUEST_COMPLETE_REQ( const KEGS_ALL_COMPLETED_QUEST_COMPLETE_REQ& kPacketNot );
	bool Handler_EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_ENTER_FIELD_QUEST_CLEAR

#ifdef SERV_POINT_COUNT_SYSTEM
	bool Handler_EGS_QUEST_POINT_COUNT_SYSTEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_POINT_COUNT_SYSTEM

	//////////////////////////////////////////////////////////////////////////

	void SetNPCID(CX2UnitManager::NPC_UNIT_ID val){	m_eNPCID = val; }
	//void UpdateQuestList( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
	//void UpdateQuestUI( bool bUpdateNavi = true );
	void UpdateNavi();
	//void ResetQuestUI();
	//void UncheckAllQuestList(); // X


	//void UpdateQuestDescPage(); // X


	//void UpdateNPCQuestDesc(); // X

	void SelectQuest( int SelectedQuestID );
	//void SelectNextQuest( bool bNext ); // X

	bool GetThereIsSelectionReward() const ;
	void OpenQuestRewardWindow( int iQuestID, bool bEnableSelection );
	void CloseQuestRewardWindow();
#ifdef QUEST_REWARD_PERIOD
	void OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID, int socketID = 0, int Period = 0 );
#else
	void OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID, int socketID = 0 );
#endif QUEST_REWARD_PERIOD
	bool CanIAcceptQuest( int iQuestID );

	// 미니맵 퀘스트 알림이 관련
	void UpdateNewQuestNotice();
	bool IsNearVillage( int CurrentVillageID, int TargetVillageID );	// 퀘스트 알리미에서 "PC가 속한 지역"인지를 확인하는 함수.

	// 마우스 휠 관련
	bool MouseOnQuestListArea( D3DXVECTOR2 mousePos );
	//bool MouseOnQuestCommentArea( D3DXVECTOR2 mousePos ); // X
#ifdef REFORM_QUEST
	void SetOpenEpicCategory();
	void InitNewQuestList(){m_setNewEventQuestIDList.clear();}
	void NotificationByQuickQuestInfoChange(const int iQuestID_, bool bIsSubQuestComplete_ = false);
	void PlayQuestCompleteSound( bool bIsSubQuestComplete_ = true);
#endif //REFORM_QUEST

protected:
	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );
	bool TargetQuestisOnScreenList( int TargetQuestID );

private:
	int						m_iQuestPerPage;


	bool					m_bShow;
	bool					m_bShowClearDLG;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGUIQuestReceive;   
	CKTDGUIDialogType			m_pDLGUIQuestClear;
	CKTDGUIDialogType			m_pDLGQuestRewardItemInfo;

	CKTDGUIDialogType			m_pDLGMsgBox;


	CKTDGUICheckBox*					m_pCheckListQuickQuest;
	CKTDGUICheckBox*					m_pCheckDetailQuickQuest;


	CX2UnitManager::NPC_UNIT_ID m_eNPCID;


	//int					m_iTopQuestSlotIndex;
	vector<int>			m_vecPrevNotAssignedQuestID;		// 새로 갱신된 퀘스트 리스트가 있는지 확인하기 위해서
	vector<int>			m_vecNotAssignedQuestID;
	vector<int>			m_vecAvailableQuestID;
	vector<QuestListSlot>			m_vQuestList;

	int					m_iPickedQuestID;

	map<int, bool >		m_mapNewQuest;					// 새로 생긴 퀘스트 목록

	bool				m_bAdminQuestComplete;
	bool				m_bLastRewardWasSelection;

	// 마우스 휠 관련
	SHORT				m_SumDelta;
	std::multimap<int, int>	m_mulmapNearVillageData;

	vector<wstring>		m_vecQuestDesc;
	int					m_iQuestDescIndex; // 필요X
	int					m_iQuestDescLinePerPage; // 필요X
	D3DXVECTOR2			m_vQuestDescSize; // 필요X

	// 퀘스트 보상 관련..
	map< int, int >		m_mapItemIDAndSocketID;			// 설마 같은 아이템을 나눠서 주는 그런 일은 없겠지
	
	// kimhc / /2009-12-18 // 선택 보상 아이템 선택하는 룰 변경
	BYTE				m_byNumOfItemThatICanGet;		// 가질 수 있는 선택 보상 아이템 갯수



public:

	enum QUEST_UI_MODE
	{
		QUM_QUEST,
		QUM_QUEST_RECEIVE,
		QUM_EPIC_QUEST,
		QUM_QUEST_DETAIL,
		QUM_EVENT_QUEST,
#ifdef ACCEPT_QUEST_TAB_FIX
		QUM_EVENT_QUEST_RECEIVE,
#endif // ACCEPT_QUEST_TAB_FIX
	};


	enum QUEST_STATE_SORT
	{
		QSS_COMPLETE_EPIC,
		QSS_COMPLETE,
		QSS_WAIT_EPIC,
		QSS_FAILED,
		QSS_ONGOING,
		QSS_AVAILABLE,
		QSS_END,
	};

	enum QUEST_TYPE_SORT
	{
		QTS_EPIC,
		QTS_CONTEST,
		QTS_EVENT,
		QTS_PCBANG,
		QTS_CHANGE_JOB,
		QTS_SKILL,
		QTS_NORMAL,
	};

	enum REPEAT_TYPE_SORT
	{
		RTS_NORMAL,
		RTS_REPEAT,
		RTS_DAY,
	};

	struct UIQuickQuestInfo
	{
		CKTDGUIDialogType	m_pDLGUIQuickQuest;
		bool				m_bIsExpand;

		float				m_fExpandSizeY;

		int					m_QuestID;
		int					m_DungeonID;
		QUEST_STATE_SORT	m_eQuestState;
		QUEST_TYPE_SORT		m_eQuestType;
		REPEAT_TYPE_SORT	m_eRepeatType;

		CKTDGUIButton*		m_pButtonMinimize;
		CKTDGUIButton*		m_pButtonExpand;
#ifdef REFORM_QUEST
		CKTDGUIButton*		m_pButtonComplete;
#endif //REFORM_QUEST
		CKTDGUIStatic*		m_pStaticExpandBGNormal;
		CKTDGUIStatic*		m_pStaticExpandBGOver;

		CKTDGUIStatic*		m_pStaticQuestTitle;
		CKTDGUIStatic*		m_pStaticQuestInfo;

		UIQuickQuestInfo()
		{

			m_QuestID		= -1;
			m_DungeonID		= 0;

			m_eQuestState	= CX2UIQuestNew::QSS_AVAILABLE;
			m_eQuestType	= CX2UIQuestNew::QTS_NORMAL;
			m_eRepeatType	= CX2UIQuestNew::RTS_NORMAL;
			m_bIsExpand = false;
			m_fExpandSizeY	= 0.f;

			m_pButtonMinimize		= NULL;
			m_pButtonExpand			= NULL;
#ifdef REFORM_QUEST
			m_pButtonComplete		= NULL;
#endif //REFORM_QUEST
			m_pStaticExpandBGNormal = NULL;
			m_pStaticExpandBGOver	= NULL;

			m_pStaticQuestTitle		= NULL;
			m_pStaticQuestInfo		= NULL;
		}
	};

	struct UIQuestSlotInfo
	{	
		bool				m_bIsCategory;
		bool				m_bIsCategoryOpen;
		int					m_QuestID;
		int					m_DungeonID;
		int					m_iPlayLevel;
		QUEST_STATE_SORT	m_eQuestState;
		QUEST_TYPE_SORT		m_eQuestType;
		REPEAT_TYPE_SORT	m_eRepeatType;

		CKTDGUIButton*		m_pButtonCategory;
		CKTDGUIStatic*		m_pStaticCategory;
		CKTDGUIRadioButton*	m_pButtonQuest;
		CKTDGUIStatic*		m_pStaticQuest;
		int					m_iQuestIndex;

		UIQuestSlotInfo()
		{
			m_bIsCategory	= false;
			m_bIsCategoryOpen = false;
			m_QuestID		= -1;
			m_DungeonID		= 0;
			m_iPlayLevel	= 0;

			m_eQuestState = CX2UIQuestNew::QSS_AVAILABLE;
			m_eQuestType = CX2UIQuestNew::QTS_NORMAL;
			m_eRepeatType = CX2UIQuestNew::RTS_NORMAL;

			m_pButtonCategory = NULL;
			m_pStaticCategory = NULL;
			m_pButtonQuest = NULL;
			m_pStaticQuest = NULL;
			m_iQuestIndex = 0;
		}
	};

	struct QuestInfo
	{
		bool m_bIsCategory;
	
		int m_iID;
		int m_iEpicGroupID;
		int m_iAfterQuestID;
		QUEST_STATE_SORT m_eQuestState;
		QUEST_TYPE_SORT m_eQuestType;
		REPEAT_TYPE_SORT m_eRepeatType;
		int m_iConditionLevel;
		int m_iPlayLevel;
		int m_iFairLevel;

		QuestInfo()
		{
			m_bIsCategory = false;
			m_iID = -1;
			m_iEpicGroupID = -1;
			m_iAfterQuestID = -1;
			m_eQuestState = CX2UIQuestNew::QSS_AVAILABLE;
			m_eQuestType = CX2UIQuestNew::QTS_NORMAL;
			m_eRepeatType = CX2UIQuestNew::RTS_NORMAL;
			m_iConditionLevel = 0;
			m_iPlayLevel = 0;
			m_iFairLevel = 0;

		}
	};
	

	struct EpicQuestInfoSort : public std::binary_function< QuestInfo, QuestInfo, bool >
	{
		bool operator()( const QuestInfo& _Left, const QuestInfo& _Right ) const
		{
			if(_Left.m_iEpicGroupID == _Right.m_iEpicGroupID)
			{
				if(_Left.m_iID == _Right.m_iID)
				{
					return true;
				}
				else if(_Left.m_iID > _Right.m_iID)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(_Left.m_iEpicGroupID > _Right.m_iEpicGroupID)
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};



	struct QuestInfoSort : public std::binary_function< QuestInfo, QuestInfo, bool >
	{
		bool operator()( const QuestInfo& _Left, const QuestInfo& _Right ) const
		{
			if(_Left.m_eQuestState == _Right.m_eQuestState)
			{
				if(_Left.m_eQuestType == _Right.m_eQuestType)
				{
					if(_Left.m_eRepeatType == _Right.m_eRepeatType)
					{
						if(_Left.m_iConditionLevel == _Right.m_iConditionLevel)
						{
							if(_Left.m_iID == _Right.m_iID)
							{
								return true;
							}
							else if(_Left.m_iID < _Right.m_iID)
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						else if(_Left.m_iConditionLevel < _Right.m_iConditionLevel)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else if(_Left.m_eRepeatType < _Right.m_eRepeatType)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if(_Left.m_eQuestType < _Right.m_eQuestType)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(_Left.m_eQuestState < _Right.m_eQuestState)
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};

	//퀵퀘스트 정렬용
	struct QuickQuestInfoSort : public std::binary_function< QuestInfo, QuestInfo, bool >
	{
		bool operator()( const QuestInfo& _Left, const QuestInfo& _Right ) const
		{
			if(_Left.m_eQuestState == _Right.m_eQuestState)
			{
				if(_Left.m_iFairLevel == _Right.m_iFairLevel)
				{
					if(_Left.m_iID == _Right.m_iID)
					{
						return true;
					}
					else if(_Left.m_iID < _Right.m_iID)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if(_Left.m_iFairLevel < _Right.m_iFairLevel)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else if(_Left.m_eQuestState < _Right.m_eQuestState)
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};


	// oasis907 : 김상윤 [2010.6.21] // 
	// UIQuestListInfo는 Quest UI 상에 뿌려지는 한 칸의 리스트 요소이다.
	// 하나의 Quest 정보가 될수도, 아무런 내용을 담지 않은 카테고리가 될 수도 있다.
	UIQuestSlotInfo		m_vecUIQuestSlotInfo[_CONST_UIQUESTNEW_INFO_::g_iNumQuestSlotPerPage];

	vector<QuestInfo>	m_vecOnGoingQuestInfo;
	vector<QuestInfo>	m_vecAvailQuestInfo;
#ifdef REFORM_QUEST
	vector<QuestInfo>	m_vecOnGoingEventQuestInfo;
	std::map<int, bool> m_mapEpicQuestProgressInfo;	//에픽퀘스트 카테고리 별 진행 여부 <EpicGroupID, bIsProgress>
	std::set<int>		m_setNewEventQuestIDList;	//새롭게 자동 수락된 퀘스트 목록
#endif //REFORM_QUEST


	vector<QuestInfo>	m_vecEpicQuestInfo;
	vector<QuestInfo>	m_vecQuestInfoList;
	vector<QuestInfo>	m_vecQuickQuest;		//퀵퀘스트 정렬용 벡터


	vector<int>			m_vecOnGoingEpicQuestID;

	CKTDGUIDialogType	m_pDLGUIQuestList;

	CKTDGUIDialogType	m_pDLGUIQuestDetail;

	UIQuickQuestInfo	m_vecUIQuickQuest[_CONST_UIQUESTNEW_INFO_::g_iMaxNumOngoingQuest];

	int					m_iMaxLengthQuestList;
	
	QUEST_UI_MODE		m_eNowQuestUIMode;

	bool				m_bIsOpenAvailCategory;
	bool				m_bIsOpenOngoingCategory;
	bool				m_bIsOpenEventCategory;
	int					m_iOpenEpicCategory;

	std::set<int>		m_setOpenQuickQuestID;



	// 퀘스트 창 열었을 때 탭 상태 우선 순위에 쓰이는 인자
	bool				m_bIsTalkingQuestNPC;
	bool				m_bAvailableQuestNPCHas;
	bool				m_bCompleteQuestInEpicTab;
	bool				m_bCompleteQuestInNormalTab;
	bool				m_bOngoingQuestInEpicTab;
	bool				m_bOngoingQuestInNormalTab;
	
	int					m_iCompleteEpicCategory;
	int					m_iOngoingEpicCategory;

	int					m_iSizeQuestInfoList;

	int					m_iTopQuestSlotIndex;

	QUEST_UI_MODE		m_eLastQuestUIMode;

	int					m_iSelectedQuestID;
	int					m_iSelectedQuestSlotIndex;

	// 스크립트에서 파싱하는 상수
	int			m_iLineWidthQuestClearCondition;
	int			m_iLineWidthNPCQuestDescription;

#ifdef GUIDE_QUICK_QUEST_COMPLETE
	int			m_iShowQuickQuest;	// 오현빈 // 2012-05-14 // 보여지는 퀵퀘스트 갯수
#endif //GUIDE_QUICK_QUEST_COMPLETE
	// ToolTip : TalkBox 이용해서.
	CX2TalkBoxManager*		m_pTalkBoxManager;

	CKTDGFontManager::CUKFont* m_pFontQuestClearCondition;
	CKTDGFontManager::CUKFont* m_pFontNPCQuestDescription;

	CKTDGFontManager::CUKFont* m_pFontQuickQuestClearCondition;

	wstring GetLocalMapQuestDesc( std::vector<QuestInfo>& vecQuestInfo , int iDungeonID);
	bool GetLocalMapQuestDesc(int iDungeonID, OUT wstring& QuestDesc);
	bool	GetPartyUIQuestIcon( std::vector<QuestInfo>& vecQuestInfo, int iDungeonID, int iDungeonDifficulty, int iDungeonMode);
	bool	GetPartyUIQuestIcon( int iDungeonID, int iDungeonDifficulty, int iDungeonMode);



	wstring CreateStringQuestClearCondition();
	wstring CreateStringNPCQuestDescription();

	void ProcessQuestDesc( const wstring& wstrDesc, const int iLineWidth, CKTDGFontManager::CUKFont* pFont );

	void UpdateQuestClearCondition();
	void UpdateNPCQuestDescription();

	void ShowUIDesc( bool bShow, wstring wstr = L"", D3DXVECTOR2 pos = D3DXVECTOR2(0,0), 
		CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT = CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT,
		D3DXCOLOR coTextColor = D3DXCOLOR(0,0,0,1), D3DXCOLOR coBackColor = D3DXCOLOR(1, 0.96f, 0.6f, 1 ) );


	void UpdateDetailQuestTitle();
	void UpdateRewardInfo(); // EXP, SP, ED 스트링

	void InitRewardItemSlot();
	void UpdateRewardItemSlot();

	void UpdateQuestButton();

	bool IsTalkingQuestNPC();
	void SetTalkingQuestNPC(bool bVal, CX2UnitManager::NPC_UNIT_ID NpcID = CX2UnitManager::NUI_NONE);

	void ReturnToQuestListUI();

	void SetNowQuestUIMode(QUEST_UI_MODE _eNowQuestUIMode);

	bool IsStateQuickQueskFilter();
	bool IsStateSubQuestCanClear( int iSubQuestID, int iNowDungeonID );


	QUEST_TYPE_SORT ConvertToQuestTypeSort( int _eQuestType );
	REPEAT_TYPE_SORT ConvertToRepeatTypeSort( int _eQuestRepeatType );
#ifdef MODIFY_QUEST_UI
	QUEST_STATE_SORT ConvertToStateTypeSort( int eQuestStateType_ );
#endif //MODIFY_QUEST_UI

	int GetQuestPictureNumber(QUEST_TYPE_SORT _eQuestTypeSort, REPEAT_TYPE_SORT _eRepeatTypeSort);
	int GetQuestPictureNumber(QUEST_STATE_SORT _eQuestStateSort);

	bool GetUnitQuest( std::vector<int>& vecQuestID );
	bool GetCompleteEpicQuest( std::vector<int>& vecQuestID );

	void UpdateAvailQuestInfo( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
	void UpdateOngoingQuestInfo();
	void UpdateUIQuestSlotInfo( UINT iQuestSlotInfoIndex,  QuestInfo& QuestInfo );

	void SetUIQuestSlotInfoToCategory( UINT iQuestSlotInfoIndex,  QuestInfo& QuestInfo );
	void SetUIQuestSlotInfoToEpicCategory( UINT iQuestSlotInfoIndex,  QuestInfo& QuestInfo );

	void EpicQuestSort( std::vector<QuestInfo>& vecEpicQuestInfo);


	void InitUIQuestSlotInfo();
	void HideUIQuestSlotInfo();

	QUEST_UI_MODE DetermineUIModeWithTabPriority();
	void ResetTabPriorityFactor();


	void ResetQuestList(QUEST_UI_MODE eQuestUIMode);
	void ResetQuestUI( bool bUpdateNavi = true, bool bResetExpand = false, bool bIgnoreTabPriority = true);


	void NewEpicStartEffect();
	//외부 인터페이스 UIQuest
	// UpdateQuickQuestDLG
	// GetShowQuickQuestDLG
	// UpdateQuestUI

	void FocusQuest( bool bDown );

	wstring GetNPCFaceTextureKey(CX2UnitManager::NPC_UNIT_ID NpcID);


	void MinimizeAllUIQuickQuest();
	void MinimizeAllUIQuickQuestWithExpandInfo();

	void ExpandAllUIQuickQuest();
	void ExpandAllUIQuickQuestWithExpandInfo();


	void ExpandUIQuickQuest(int iSelectedUIQuickQuestIndex, bool bMouseOver = true);
	void MinimizeUIQuickQuest(int iSelectedUIQuickQuestIndex, bool bPreserveExpandInfo = false);
#ifdef GUIDE_QUICK_QUEST_COMPLETE
	void MouseOverUIQuickQuest(int iSelectedUIQuickQuestIndex, D3DXVECTOR2 vPos);
#else
	void MouseOverUIQuickQuest(int iSelectedUIQuickQuestIndex);
#endif //GUIDE_QUICK_QUEST_COMPLETE
	void MouseOutUIQuickQuest(int iSelectedUIQuickQuestIndex);


	bool GetOverMouseQuest();
	bool GetShowQuickQuestDLG();
	void SetShowQuickQuestDLG( bool forOpen );
	void UserShowQuickQuestDLG( bool forOpen );
	void InitUserShowQuickQuestDLG( bool forOpen );
	void UpdateQuickQuestDLG(bool bResetExpand = false, bool bUpdateOngoingQuest = true); //오현빈 // 2012-10-02 // ResetQuestUI에서 호출되었을 경우에는 UpdateOngoingQuestInfo중복 수행 하지 않도록 인자 추가(bUpdateOngoingQuest)
	void ToggleUserShowQuickQuestDLG()
	{ 
#ifdef FIX_UI_OFF_TAB_OFF
		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
			return;
#endif //FIX_UI_OFF_TAB_OFF
		m_bUserShowQuickQuestDLG = !m_bUserShowQuickQuestDLG; 
		UpdateQuickQuestDLG();UserShowQuickQuestDLG(m_bUserShowQuickQuestDLG); ShowUIDesc(false); 
	}
	bool	m_bShowQuickQuestDLG;	// 시스템 상 강제로 행해지는 퀵퀘스트창의 온오프
	bool	m_bUserShowQuickQuestDLG; // 유저가 원하는 퀵퀘스트창의 온오프

#ifdef REFORM_QUEST
	bool GetSubQuestDungeonIDList( IN int iSubQuestID, OUT vector<int>& vecDungeonID_ );
	void ClearQuestCompleteParticle();
#else
	int GetSubQuestDungeonID( int iSubQuestID );
#endif //REFORM_QUEST

	bool SetUIQuickQuestSlotInfo(int iQuestSlotInfoIndex, QuestInfo& QuestInfo );

#ifdef REFORM_NOVICE_GUIDE
	void SetNoviceGuideStep();
#endif //REFORM_NOVICE_GUIDE

#ifdef GUIDE_QUEST_MONSTER
	bool GetIsQuestMonster( CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ ){ return (m_setQuestNPCList.find(eNPCUnitID_) != m_setQuestNPCList.end()); }
#endif //GUIDE_QUEST_MONSTER

	void UpdateQuestMonster();
private:
#ifdef REFORM_QUEST
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hQuestCompleteParticle;
#endif //REFORM_QUEST

#ifdef GUIDE_QUEST_MONSTER
	void SetQuestMonsterByQuestInfo( const vector<QuestInfo>& vecQuestInfo_ );
	std::set<CX2UnitManager::NPC_UNIT_ID>	m_setQuestNPCList;
#endif //GUIDE_QUEST_MONSTER

#ifdef DAY_OF_THE_WEEK_QUEST
	// 요일 퀘스트 유효셩 여부 체크.
	// 요일 퀘스트가 아니라면 무조건 treu 반환.
	bool GetIsValidDayTheWeekQuest( int iQuestID_ ) ;
#endif //DAY_OF_THE_WEEK_QUEST
};
#endif SERV_EPIC_QUEST

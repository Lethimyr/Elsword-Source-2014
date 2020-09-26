#ifndef SERV_EPIC_QUEST
#pragma once

//class CX2StateMenu;
class CX2UIQuestReceive : public CX2ItemSlotManager
{
	enum UI_QUEST_RECEIVE_CUSTOM_MSG
	{
		UQRCM_EXIT				= 24000,
		UQRCM_QUEST_SELECT,
		UQRCM_QUEST_ACCEPT,
		UQRCM_QUEST_COMPLETE,
		UQRCM_QUEST_GIVE_UP,
		UQRCM_QUEST_GIVE_UP_CONFIRM,

		UQRCM_QUESTCLEAR_OK,

		UQRCM_SCROOL_UP,
		UQRCM_SCROOL_DOWN,

		UQRCM_REWARD_CHECK_ITEM,
		UQRCM_REWARD_UNCHECK_ITEM,
		UQRCM_REWARD_MOUSE_OVER,
		UQRCM_REWARD_MOUSE_OUT,

		UQRCM_NAVI_LCLICKED,
		UQRCM_NAVI_DRAGGING,

		UQRCM_QUEST_ACCEPT_CONFIRM,
		UQRCM_QUEST_ACCEPT_CANCLE,
		UQRCM_QUEST_GIVE_UP_CANCLE,

		UQRCM_QUEST_ADMIN_COMPLETE,

		UQRCM_QUEST_DESC_UP,
		UQRCM_QUEST_DESC_DOWN,

	};

	struct QuestListSlot
	{
		int					m_QuestID;
		CKTDGUIRadioButton*	m_pButton;
		CKTDGUIStatic*		m_pStatic;
	};


public:

	CX2UIQuestReceive( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIQuestReceive(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
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
	bool Handler_EGS_GIVE_UP_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID );
	bool Handler_EGS_QUEST_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ADMIN_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID );

	//////////////////////////////////////////////////////////////////////////

	void SetNPCID(CX2UnitManager::NPC_UNIT_ID val){	m_eNPCID = val; }
	void UpdateQuestList( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
	void UpdateQuestUI( bool bUpdateNavi = true );
	void UpdateNavi();
	void ResetQuestUI();
	void UncheckAllQuestList();


	void UpdateQuestDesc( const wstring& wstrDesc );
	void UpdateQuestDescPage();
	wstring CreateQuestDesc();
	void UpdateNPCQuestDesc();

	void SelectQuest( int SelectedQuestID );
	void SelectNextQuest( bool bNext );
	void SetRewardItemSlot();

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
	bool MouseOnQuestCommentArea( D3DXVECTOR2 mousePos );

protected:
	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );
	bool TargetQuestisOnScreenList( int TargetQuestID );

private:
	int						m_iQuestPerPage;
	int						m_iRewardSlotNum;

	bool					m_bShow;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGUIQuestReceive;   
	CKTDGUIDialogType			m_pDLGUIQuestClear;
	CKTDGUIDialogType			m_pDLGQuestRewardItemInfo;

	CKTDGUIDialogType			m_pDLGMsgBox;

	CX2UnitManager::NPC_UNIT_ID m_eNPCID;


	int					m_iTopIndex;
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
	int					m_iQuestDescIndex;
	int					m_iQuestDescLinePerPage;
	D3DXVECTOR2			m_vQuestDescSize;

	// 퀘스트 보상 관련..
	map< int, int >		m_mapItemIDAndSocketID;			// 설마 같은 아이템을 나눠서 주는 그런 일은 없겠지
	
	// kimhc / /2009-12-18 // 선택 보상 아이템 선택하는 룰 변경
	BYTE				m_byNumOfItemThatICanGet;		// 가질 수 있는 선택 보상 아이템 갯수

};
#endif SERV_EPIC_QUEST

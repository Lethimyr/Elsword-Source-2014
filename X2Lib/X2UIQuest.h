#ifndef SERV_EPIC_QUEST
#pragma once

//class CX2StateMenu;
class CX2UIQuest : public CX2ItemSlotManager
{
	enum QUEST_UI_CUSTOM_MESSAGE
	{
		QUCM_EXIT					= 25000,
		QUCM_SELECT_QUEST,
		QUCM_CATEGORY_BUTTON,
		QUCM_LIST_UP,
		QUCM_LIST_DOWN,
		QUCM_COMMENT_UP,
		QUCM_COMMENT_DOWN,
		QUCM_QUEST_GIVEUP,
		QUCM_QUEST_GIVEUP_CONFIRM,
		QUCM_QUEST_GIVEUP_CANCLE,

		QUCM_LIST_NAVI_LCLICKED,
		QUCM_LIST_NAVI_DRAGGING,
		
		QUCM_COMMENT_NAVI_LCLICKED,
		QUCM_COMMENT_NAVI_DRAGGING,

		QUCM_QUICK_QUEST_CHECKBOX,
		QUCM_QUICK_QUEST_SIZE_BUTTON,

	};
	
	// 정렬된 퀘스트 + 카테고리 리스트를 담은 struct. 벡터에 들어갑니다.
	struct MyQuestList
	{
		bool				m_bIsCategory;
		bool				m_bIsOpen;
		int					m_QuestID;
		int					m_DungeonID;

		MyQuestList()
		{
			m_bIsCategory	= false;
			m_bIsOpen		= false;
			m_QuestID		= -1;
			m_DungeonID		= 0;
		}
	};

	// 버튼 + 스태틱 정보를 담은 struct. 벡터에 들어갑니다.
	struct MyQuestListSlot
	{		
		CKTDGUIButton*		m_pCategoryButton;
		CKTDGUIStatic*		m_pCategoryStatic;
		CKTDGUIRadioButton*	m_pListButton;
		CKTDGUIStatic*		m_pListStatic;
		int					m_iQuestIndex;
		MyQuestListSlot()
		{
			m_pListButton = NULL;
			m_pListStatic = NULL;
			m_pCategoryButton = NULL;
			m_pCategoryStatic = NULL;
			m_iQuestIndex = 0;
		}
	};
public:

	CX2UIQuest( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIQuest(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vec );
	

	void ResetQuestUI( bool bResetTopIndex = true );
	void ClearQuestSlotList();
	void UpdateQuestList();
	void UpdateQuestUI( bool bUpdateNavi = true );
	void UpdateQuestListNavi();
	
	void UpdateQuestDesc( const wstring& wstrDesc );
	void UpdateQuestDescPage( bool bUpdateNavi = true );
	void UpdateQuestCommentNavi();
	wstring CreateQuestDesc();	
	
	void SelectQuest( int SelectedQuestID );
	void SetRewardItemSlot();
	void SelectCategory( int SelectedCategoryDID, bool forOpen );

	void SetNextTopIndex(bool isUp);

	// 아이콘 받기~
// 	wstring GetVillageIconFileName( int LocalMapID );
// 	wstring GetVillageIconKey( int LocalMapID );

	// 마우스 휠 관련
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool MouseOnQuestListArea( D3DXVECTOR2 mousePos );
	bool MouseOnQuestCommentArea( D3DXVECTOR2 mousePos );



	// 퀵퀘스트 관련
	void SetShowQuickQuestDLG( bool forOpen );
	bool GetShowQuickQuestDLG();
	void UpdateQuickQuestDLG();
	void OpenUpQuickQuesstDLG( bool forOpen );
	bool GetQuickQuestDLGOpenedUP(){ return m_bQuickQuestOpenedUP; }

	// 서브퀘스트 ID에서 해당 퀘스트가 수행되는 던전 ID를 구한다 ( 없으면 DI_END return )
	int GetSubQuestDungeonID( int iSubQuestID );

	bool GetOverMouseQuest();

	// 보상아이템 관련 예외처리
	virtual wstring GetSlotItemDesc();

private:
	int						m_iQuestPerPage;
	int						m_iRewardSlotNum;

	int						m_iQuestListSize;
	int						m_iQuestListUnderTop;
	
	bool					m_bShow;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGUIQuest;   
	CKTDGUIDialogType			m_pDLGMsgBox;

	vector<MyQuestListSlot>			m_vQuestListSlot;
	vector<MyQuestList>				m_vQuestList;
	int						m_iTopIndex;
	int						m_iPickedQuestID;
	int						m_iQuestNum;

	// 마우스 휠 관련
	SHORT				m_SumDelta;

	// 퀘스트 정보 관련
	D3DXVECTOR2			m_vQuestDescSize;				// 퀘스트 설명 글자가 나올 창의 크기, 글자 부분만
	vector<wstring>		m_vecQuestDesc;					// 선택된 퀘스트의 설명
	int					m_iQuestDescIndex;
	int					m_iQuestDescLinePerPage;


	// 퀵 퀘스트 창 관련
	CKTDGUIDialogType			m_pDLGQuickQuest;
	bool					m_bQuickQuestOpenedUP;
	bool					m_bQuickQuestOpen;

	// 퀘스트 보상 관련..
	map< int, int >		m_mapItemIDAndSocketID;			// 설마 같은 아이템을 나눠서 주는 그런 일은 없겠지

};
#endif SERV_EPIC_QUEST

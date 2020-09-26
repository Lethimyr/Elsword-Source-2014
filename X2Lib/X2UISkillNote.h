#ifdef SERV_SKILL_NOTE

#pragma once
class CX2UISkillNote
{
public:
	enum UI_SKILL_NOTE_MSG
	{
		USNM_EXIT = 39000,		
		USNM_REGISTER_VIEW_SLOT,
		USNM_SLOT_PAGE_LEFT,
		USNM_SLOT_PAGE_RIGHT,
		USNM_REGISTER_OK,
		USNM_REGISTER_CANCEL,
		USNM_PAGE_PREV,
		USNM_PAGE_NEXT,

		USNM_USE_MEMO,						// 메모 사용
		USNM_BUY_MEMO,						// 메모 구입
		USNM_HAS_MEMO_MOUSE_OVER,			// 보유 메모 설명
		USNM_NOT_HAS_MEMO_MOUSE_OVER,		// 미보유 메모 설명
	};

public:

	CX2UISkillNote( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UISkillNote(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SetShow(bool val);
	bool GetShow(){ return m_bShow; }
	
	bool OpenDialog();

	void SetLayer(X2_DIALOG_LAYER layer);

	void RegisterMemo(UidType iMemoId);
	void SetRegisterMemo(bool bVal) { m_bRegisterMemo = bVal; }
	bool GetRegisterMemo() { return m_bRegisterMemo; }
    
	void UpdateMemoSlot(int iPage = 0);
	void UpdateMemo(bool bVal, char cPage);	

	char GetUpdatePage() { return m_iUpdatePage; }
#ifndef REFORM_SKILL_NOTE_UI // 메모 즉시 등록되도록 변경
	bool GetUpdateMemo() { return m_bUpdateMemo; }
	float GetWriteMemoTime() { return m_fWriteMemoTime; }
#endif // REFORM_SKILL_NOTE_UI
	void ResetSkillNote();


#ifdef REFORM_SKILL_NOTE_UI
	// 소지 여부 갱신. (기술의 노트가 열려진 상태에서만 불려 질 수 있다 )
	void UpdateMemoList_MemoItemNum();	
	
	void ResetMemoListDLG();
#endif // REFORM_SKILL_NOTE_UI

private:	
	void ViewMemo(int iPage);
	void Handler_EGS_REG_SKILL_NOTE_MEMO_REQ(char iPage);	
	void CheckRegisterMemo();
	void UpdateMemoEffect();	
    wstring ReplaceDesc_(const wchar_t* pwstrOri, const wchar_t* pwstrPattern, const wchar_t* pwstrReplace);

#ifdef REFORM_SKILL_NOTE_UI
	// 메모 설명
	void GetMemoDesc( OUT wstring& wstrMemoDesc_, int iMemoItemID_ = 0, bool bHas_ = false);


	// 메모 리스트 관련
	void				ResetMemoList();
	void				ResetMemoListUI();

	void				SetMemoListData_MemoData( CKTDGUIStatic *pStaticMemo_, int iSkillID_, int iMemoItemID_, int iMemoItemNum_ );
	void				SetMemoListData_MemoButton( CKTDGUIButton *pButton_, int iMemoItemID_, int iMemoItemNum_ );
#endif // REFORM_SKILL_NOTE_UI

private:
	char					m_iRegisterPage;
	char					m_iCurrentPage;	
	char					m_iUpdatePage;
	bool					m_bShow;
	bool					m_bRegisterMemo;
	UidType					m_iRegisterMemo;
	int						m_iViewPage;

	CKTDGUIDialogType		m_pDLGSkillNote;
	CKTDGUIDialogType		m_pDLGQuery;
	CKTDGUIDialogType		m_pDLGInfoBox;
#ifndef REFORM_SKILL_NOTE_UI // 메모 즉시 등록되도록 변경
	CKTDGUIDialogType		m_pDLGWrittingNote;
	float					m_fWriteMemoTime;
	bool					m_bUpdateMemo;	
	std::vector<wstring>	m_vecSlotLv;
#endif // REFORM_SKILL_NOTE_UI

	CKTDGParticleSystem::CParticleEventSequenceHandle m_hParticle1;	
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hParticle2;	

#ifdef REFORM_SKILL_NOTE_UI
	SHORT					m_SumDelta;
	/* #NOTE : 기술의 노트 UI와 분리 시킴.
	분리 시킨 이유  
	//1. 컨트롤 리스트내부 컨트롤을 캐릭터가 변경되면 제거해야 하는데, 제거가 불편 함.	
	//2. 마우스 오버시 휠 동작해야되는데, 현재 마우스 오버는 다이얼로그 단위로만 설정 가능 함.	*/	
	CKTDGUIDialogType		m_pDLGMemoList;	
#endif // REFORM_SKILL_NOTE_UI

};

#endif

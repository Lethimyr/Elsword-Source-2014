#pragma once


class CX2TutorSystem
{
public:
	enum TUTOR_SYSTEM_UI_CUSTOM_MSG
	{
		TSUCM_VIEW_CANDIDATE_AFTER_A_WEEK	= 1700, 
		TSUCM_CLOSE_STUDENT_CANDIDATE_LIST,
		TSUCM_REFRESH_STUDENT_CANDIDATE_LIST,
		TSUCM_PREV_PAGE_STUDENT_CANDIDATE_LIST,
		TSUCM_NEXT_PAGE_STUDENT_CANDIDATE_LIST,

		TSUCM_ACCEPT_TUTOR_YES,				// 사제요청 응답
		TSUCM_ACCEPT_TUTOR_NO,

		TSUCM_REQUEST_STUDENT,				// 사제관계 요청하기

		TSUCM_STUDENT_CANDIDATE_1,			// 사제 후보 선택
		TSUCM_STUDENT_CANDIDATE_2,
		TSUCM_STUDENT_CANDIDATE_3,
		TSUCM_STUDENT_CANDIDATE_4,
		TSUCM_STUDENT_CANDIDATE_5,

		TSUCM_REFUSE_TUTOR_REQUEST_FOR_A_WEEK, 
	};

	struct TutorUnit
	{
		UidType			m_UnitUID;
		int				m_iLevel;
		std::wstring	m_wstrNickName;
		bool			m_bIsOnline;

		TutorUnit()
		{
			m_UnitUID = -1;
			m_iLevel = 0;
			m_wstrNickName = L"";
			m_bIsOnline = false;
		}
		TutorUnit( const TutorUnit& t ) { *this = t; }    // copy constructor
		TutorUnit& operator=( const TutorUnit& t )        // assign operator
		{
			m_UnitUID = t.m_UnitUID;
			m_iLevel = t.m_iLevel;
			m_wstrNickName = t.m_wstrNickName;
			m_bIsOnline = t.m_bIsOnline;

			return *this;
		}
	};


	struct TutorOption
	{
		int		m_iLastViewYDayPossibleStudentList;		// 일주일간 보지 않기로 설정 되어있을 때 마지막으로 본 날짜
		bool	m_bViewAfterAWeek;						// 일주일간 보지 않기

		bool	m_bRefuseTutorForAWeek;					// 일주일간 사제요청 거절하기
		int		m_iLastRefuseYDay;						// 일주일간 사제요청 거절했던 날

		TutorOption()
		{
			m_bViewAfterAWeek = false;
			m_iLastViewYDayPossibleStudentList = -1;

			m_bRefuseTutorForAWeek = false;
			m_iLastRefuseYDay = -1;
		};
	};


	struct TutorInviting
	{
	public:
		CKTDGUIDialogType			m_pDLGInviting;
		wstring					m_wstrInviteeName;
		float					m_fTimeLeftForWaitingReply;

	public:
		TutorInviting( wstring& wstrInviteeName, float fTimeLeft, CKTDXStage* pNowStage );
		~TutorInviting();
		void OnFrameMove( double fTime, float fElapsedTime );
	};


	struct TutorInvited
	{
	public:
		CKTDGUIDialogType			m_pDLGInvited;
		UidType					m_InviterUnitUID;
		wstring					m_wstrInviterName;
		float					m_fTimeLeftForReplyInvite;

	public:
		TutorInvited( UidType InviterUnitUID, wstring& wstrInviterName, float fTimeLeft, int YesMsgID, int NoMsgID, CKTDXStage* pNowStage );
		~TutorInvited();
		void OnFrameMove( double fTime, float fElapsedTime );
	};
	

public:
	CX2TutorSystem( CKTDXStage* pStage );
	virtual ~CX2TutorSystem(void);

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DEL_TUTORIAL_REQ( UidType unitUID );
	bool Handler_EGS_DEL_TUTORIAL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DEL_TUTORIAL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_REQUEST_TUTORIAL_REQ( UidType unitUID );
	bool Handler_EGS_REQUEST_TUTORIAL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_REQUEST_TUTORIAL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Send_Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( bool bAccept, UidType receiverUnitUID );
	bool Handler_EGS_REQUEST_TUTORIAL_REPLY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_TUTORIAL_STUDENT_LIST_REQ( int iPageIndex );
	bool Handler_EGS_TUTORIAL_STUDENT_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



	bool OpenScriptFile( const WCHAR* pFileName );
	bool SaveScriptFile( const WCHAR* pFileName );

	void OpenStudentCandidateList( bool bOpen );
	void UpdateStudentCandidateListPage(); 

	void SetLastViewDateToday( wstring wstrNickName );
	bool Elapsed7DaysFromLastViewDate( wstring wstrNickName );
	bool Elapsed1DaysFromLastViewDate( wstring wstrNickName );

	void SetLastRefuseDateToday( wstring wstrNickName );
	bool Elapsed7DaysFromLastRefuseDate( wstring wstrNickName );


public:
	CKTDXStage* GetStage() const { return m_pStage; }
	void SetStage(CKTDXStage* val);

	bool GetViewAfterAWeek( wstring wstrNickName )
	{
		return m_mapTutorOption[wstrNickName].m_bViewAfterAWeek;
	}
	
	void SetViewAfterAWeek( wstring wstrNickName, bool val) 
	{
		m_mapTutorOption[wstrNickName].m_bViewAfterAWeek = val;
	}

	bool GetBeenUnitSelect() const { return m_bBeenUnitSelect; }
	void SetBeenUnitSelect(bool val) { m_bBeenUnitSelect = val; }

	void OnFrameMove( double fTime, float fElapsedTime );

	int GetNumberOfStudent() { return (int)m_mapTutorUnit.size(); }
	void ResetTutorStudent( vector<KTutorialDBUnitInfo>& vecStudent );


	const map< UidType, TutorUnit >& GetMapTutorStudent() const { return m_mapTutorUnit; }
	const TutorUnit* GetTutorUnit( UidType unitUID ) const;

	void SetTutorInviteeName(wstring val) { m_wstrInviteeName = val; }


private:		
	CKTDXStage*					m_pStage;
	CKTDGUIDialogType				m_pDLGStudentCandidateList;

	vector< TutorUnit >			m_vecStudentCandidate;
	map< UidType, TutorUnit >	m_mapTutorUnit;				// 스승 혹은 제자 목록
	map< wstring, TutorOption >	m_mapTutorOption;			// 캐릭터 이름 : 사제 UI 옵션

	bool						m_bBeenUnitSelect;			// 유닛 선택창으로 이동했을 경우에만 true로


	TutorInviting*				m_pTutorInviting;
	vector< TutorInvited* >		m_vecpTutorInvited;
	wstring						m_wstrInviteeName; 


	int					m_iMaxStudentCandidatePage;
	int					m_iNowStudentCandidatePage;

	UidType				m_iPickedStudentCandidateUID;
	wstring				m_wstrPickedStudentCandidateName;

	CKTDXTimer			m_TimerRefreshCandidateList;
};

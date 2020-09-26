//{{ 2011.9.16 이준호 반디캠 작업
#ifdef	BANDICAM_RECORDING

#pragma once


// namespace		_CONST_UIRANKING_INFO_
// {
// 	const	int		g_iNumRankingPerPage		= 5;		// 한 페이지 당 나타낼 수 있는 랭킹 갯수
// 	const	int		g_iLimitRankingToDisplay	= 2000;		// 이 뒤의 순위는 알수없음 으로 표시
// 	const	int		g_iRankLimit				= 3;		// 순위는 3자리 까지만
// 	const	int		g_iMaxPageLimit				= 20;
// }

class CX2UIRecordVideo
{
public:

	enum UI_RECORD_VIDEO_CUSTOM_MSG
	{
		URVCM_EXIT				= 	52000,
		URVCM_RECORD_START,
		URVCM_RECORD_PAUSE,
		URVCM_RECORD_STOP,
		URVCM_OPEN_OPTION,
		URVCM_CLOSE_OPTION,
	};


public:
	CX2UIRecordVideo( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	~CX2UIRecordVideo();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );

	//정지, 해제 키 입력으로 왔을 때 처리 함수 
	void				PauseKeyHandling();	
	CKTDGUIDialogType	GetDialog()	{	return m_pDlgRecordVideo;	}
private:
	void				InitUIRecordVideo();
	void				ReadyCloseUIRecordVideo();

	//이 상태들은 상태가 변했을때 보여야할 Ui를 의미한다 이준호
	void				UI_RecdingStart();
	void				UI_RecdingPause();
	void				UI_RecdingStop();
	

	bool				m_bPause;

private:
	bool								m_bShow;
	const CKTDXStage* const				m_pNowStage;
	CKTDGUIDialogType					m_pDlgRecordVideo;

	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;

	CKTDGUIButton*	m_pButtonExit;				// 레코딩 창 닫는 버튼
	CKTDGUIButton*	m_pButtonRecordStart;		// 레코딩 시작 버튼
	CKTDGUIButton*	m_pButtonRecordPause;		// 레코딩 일시 정지 및 재시작 버튼
	CKTDGUIButton*	m_pButtonRecordStop;		// 레코딩 스톱 버튼
	CKTDGUIButton*	m_pButtonRecordRec;			// 레코딩 레코드
	//CKTDGUIButton*	m_pButtonOpenOption;		// 옵션창 여는 버튼
	//CKTDGUIButton*	m_pButtonCloseOption;		// 옵션창 닫는 버튼

//	CKTDGUIStatic*						m_pStaticHenirInfo;

};

#endif	BANDICAM_RECORDING
//}}

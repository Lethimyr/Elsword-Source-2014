#pragma once


// CSkinDialog dialog

class CSkinDialog : public CDialog
{
public:
	CSkinDialog(UINT nResourceID, CWnd* pParent = NULL);   // standard constructor

	enum	{	CLICKED_BOX_NONE=0,
				CLICKED_BOX_MINIMIZE,
				CLICKED_BOX_MAXIMIZE,
				CLICKED_BOX_RESTORE,
				CLICKED_BOX_EXIT
			};

// Attributes
public:
	CRgn		m_rgnWnd;			// 윈도우 영역

	CBrush		m_CtlColorBrush;

	int			m_nClickedBox;		// 현재 클릭된 박스 정보

	BOOL		m_bTracking;		// 현재 마우스 박스위에 있는지 아닌지 상태 플래그

	BOOL		m_bDraging;			// 윈도우가 마우스 드래그로 이동 중인지 아닌지의 상태 플래그
	BOOL		m_bSizeCursor;		// 커서모양이 사이즈조절 모양인지 아닌지의 상태 플래그

	BOOL		m_bActive;

	COLORREF	m_cfText;			// 텍스트 색
	COLORREF	m_cfMask;			// 마스크 색

	// 비트맵 사이즈
	SIZE		m_sizeCapL;			// 윈도우 캡션 왼쪽
	SIZE		m_sizeCapM;			// 윈도우 캡션 중앙
	SIZE		m_sizeCapR;			// 윈도우 캡션 오른쪽
	SIZE		m_sizeFrmL;			// 윈도우 프레임 왼쪽
	SIZE		m_sizeFrmLB;		// 윈도우 프레임 왼쪽아래
	SIZE		m_sizeFrmB;			// 윈도우 프레임 아래
	SIZE		m_sizeFrmR;			// 윈도우 프레임 오른쪽
	SIZE		m_sizeFrmRB;		// 윈도우 프레임 오른쪽아래
	SIZE		m_sizeBox;			// 윈도우 최대/최소 박스 사이즈

	// 비트맵 핸들
	HBITMAP		m_hBmpCapL;			// 윈도우 캡션 왼쪽
	HBITMAP		m_hBmpCapM;			// 윈도우 캡션 중앙
	HBITMAP		m_hBmpCapR;			// 윈도우 캡션 오른쪽
	HBITMAP		m_hBmpFrmL;			// 윈도우 프레임 왼쪽
	HBITMAP		m_hBmpFrmLB;		// 윈도우 프레임 왼쪽아래
	HBITMAP		m_hBmpFrmB;			// 윈도우 프레임 아래
	HBITMAP		m_hBmpFrmR;			// 윈도우 프레임 오른쪽
	HBITMAP		m_hBmpFrmRB;		// 윈도우 프레임 오른쪽 아래

	HBITMAP		m_hBmpMinimize;		// 윈도우 최소 박스
	HBITMAP		m_hBmpMaximize;		// 윈도우 최대 박스
	HBITMAP		m_hBmpRestore;		// 윈도우 복귀 박스
	HBITMAP		m_hBmpExit;			// 윈도우 종료 박스

	// 아이콘 핸들
	HICON		m_hIcon;

// Operations
public:
	void		DrawFrame();
	void		SetCtlColor(COLORREF crCtlColor);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

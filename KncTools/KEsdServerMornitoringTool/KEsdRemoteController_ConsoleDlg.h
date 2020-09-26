#pragma once
#include "afxwin.h"
#include "2DPushGraph.h"
#include "ListCtrlEx.h"
#include "Label.h"



// CKEsdRemoteController_ConsoleDlg 대화 상자
class CKEsdRemoteController_ConsoleDlg : public CDialog
{
	enum AUTH_LEVEL
	{
		AL_NORMAL = 0,
		AL_SUPER,
		AL_ADMIN,
	};

	enum COL_INDEX
	{
		//CI_NONE		= 0,
		CI_CHECK_BOX	= 0,
		CI_YESTERDAY_COLOR,
		CI_TODAY_COLOR,
		CI_SERVER_NAME,
		CI_IP,
		CI_CONNECT,
		CI_CHANNEL_SERVER,
		CI_LOGIN_SERVER,
		CI_CENTER_SERVER,
		CI_GAME_SERVER,		
		CI_NOWTIME_CCU,
	};

	enum CCU
	{
        CCU_TIME_GAP = 3,
	};

	// 생성
public:
	CKEsdRemoteController_ConsoleDlg(CWnd* pParent = NULL);	// 표준 생성자

	// 대화 상자 데이터
	enum { IDD = IDD_KESDREMOTECONTROLLER_CONSOLE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


	// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void SetAuthLevel( int iAuthLevel ) { m_iAuthLevel = iAuthLevel; }
	void InitEsdListCtrl();
	void UpdateEsdListCtrl( int iServerGroupID, bool bIsRemoveAllItems = false, bool bForceRefresh = false );
	void InsertEsdListCtrl( KESDServerInfo& kInfo );
	void UpdateEsdListCtrl( int iIdx, KESDServerInfo& kInfo );
	void UpdateEsdListSelected();
	void ResetAllServerChecked();
	void CheckingCheckBox();

private:
	void DrawLineYesterdayGraph();
	void DrawLineTodayGraph();
	void ChangeTabInfo( int iTabIndex );

protected:
	int  m_iTotalCCU;
	int	 m_iGuestIDTotalCCU;
	int	 m_iAuthLevel;
	bool m_bEsdListUpdateLock;
	C2DPushGraph m_PushGraph;
	CListCtrlEx m_listCtrlEx;
	CEdit m_editTotalCCU;
	CEdit m_editTopCCU;
	CEdit m_editGuestIDTotalCCU;
	CEdit m_editGuestIDTopCCU;
	CTabCtrl m_tabCtrl;
	CLabel m_yesTotal;
	CLabel m_todayTotal;
	CStatic m_totalStatic;
	CStatic m_topStatic;
	CStatic m_GuestIDStatic;

	// 서버군 ID
	int m_iServerGroupID;

	// 그래프 데이터
	std::map< int, std::vector< CCUInfo > > m_mapYesterdayGraph;
	std::map< int, std::vector< CCUInfo > > m_mapTodayGraph;
	std::map< int, std::vector< CCUInfo > > m_mapGuestUserGraph;
	typedef std::map< int, std::vector< CCUInfo > >::iterator GraphIterator;

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonLaunch();
	afx_msg void OnBnClickedButtonPatch();
	afx_msg void OnBnClickedButtonShutdown();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnNMClickListEsd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListEsd(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonLaunchCo();
	CButton m_dateInfoBtn;
	CButton m_centerOnlyBtn;
	afx_msg void OnMainExit();
	afx_msg void OnAccountAccountdialog();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	CButton m_yesterdayCheck;
	CButton m_todayCheck;
	CButton m_totalCheck;
	CButton m_averageCheck;
	CButton m_guestIDCheck;
	afx_msg void OnBnClickedButtonLogin2();
	afx_msg void OnBnClickedTotalCheck();
	afx_msg void OnBnClickedAllCheck();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMainOption();	
	afx_msg void OnBnClickedButtonDateInfo();
	afx_msg void OnBnClickedConnectButton();
	afx_msg void OnBnClickedAverageCheck();
//	afx_msg void OnHdnFilterBtnClickListEsd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickListEsd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedGuestCheck();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonHsbUpdate();
	afx_msg void OnBnClickedButtonSvnUpdate();
	afx_msg void OnBnClickedButtonWindowsUpdate();
	afx_msg void OnBnClickedButtonVirusCheck();
	afx_msg void OnBnClickedButtonDbIdpw();
};

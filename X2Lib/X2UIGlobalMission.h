#pragma once

#ifdef SERV_GLOBAL_MISSION_MANAGER

class CX2UIGlobalMission
{
public:

	CX2UIGlobalMission();
	~CX2UIGlobalMission();

	void SetShowGlobalMissionDlg( bool bShow );
	CKTDGUIDialogType GetGlobalMissionDlg() { return m_pDlgGlobalMissionDlg; }
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void UpdateNumOfNowClearCount( int iNumOfNowClearCount );
	int GetNumOfNowClearCount() const { return m_iNumOfNowClearCount; }
	void SetNumOfNowClearCount( int iNumOfNowClearCount ) { m_iNumOfNowClearCount = iNumOfNowClearCount; }

	void UpdateNumOfClearCount( int iNumOfClearCount );
	int GetNumOfClearCount() const { return m_iNumOfClearCount; }
	void SetNumOfClearCount( int iNumOfClearCount ) { m_iNumOfClearCount = iNumOfClearCount; }
private:
	int							m_iNumOfNowClearCount;	// 현재 클리어 카운트
	int							m_iNumOfClearCount;
	CKTDGUIDialogType			m_pDlgGlobalMissionDlg;	// UI
};

#endif	SERV_GLOBAL_MISSION_MANAGER

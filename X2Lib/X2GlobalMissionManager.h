#pragma once

#ifdef SERV_GLOBAL_MISSION_MANAGER

class CX2UIGlobalMission;

class CX2GlobalMissionManager
{
public:
	~CX2GlobalMissionManager();
	CX2GlobalMissionManager();

	void Init();

	CX2UIGlobalMission* GetUIGlobalMission() const { return m_pUIGlobalMission; }
	bool IsActive();

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HRESULT OnFrameMove( double fTime, float fElapsedTime );

	bool Handler_EGS_GLOBAL_MISSION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool CheckShowGlobalMissionUI();
private:
	CX2UIGlobalMission* m_pUIGlobalMission;
	KGlobalMissionInfo m_kNowGlobalMissionInfo;
};

#endif SERV_GLOBAL_MISSION_MANAGER

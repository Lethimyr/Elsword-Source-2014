#pragma once

//////////////////////////////////////////////////////////////////////////
//## Main stage
//## Desc : 엔진에 등록되어 사용될 메인 프레임
//##		- 컨트롤 데이터를 보관 & 시나리오 엔진으로의 기능 전송
//////////////////////////////////////////////////////////////////////////

enum DLG_HWND
{
	DH_RESOURCE = 0,

	DH_ACTION,
	DH_TEX_IO,
	DH_TEX_02,

	DH_ACTION_LIST,

	DH_END,
};

class CX2ScenarioToolStage : public CKTDXStage
{
public:
	CX2ScenarioToolStage(void);
	virtual ~CX2ScenarioToolStage(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	void	CreateDlg();
	// 다이얼로그 및 해당 컨트롤 핸들 제공 함수들..
	void	SetDlgHwnd( DLG_HWND dlgType, HWND hWnd );
	HWND	GetDlgHwnd( DLG_HWND dlgType );
	HWND	GetDlgItem( DLG_HWND dlgType, int iDlgItem );
	void	HideActionSubDlg();
	void	ShowActionSubDlg(int iSelSubDlg);

private:

	CKTDGScenarioManager		m_cScenarioManager;

private:

	HWND	m_hWnd[DH_END];
};

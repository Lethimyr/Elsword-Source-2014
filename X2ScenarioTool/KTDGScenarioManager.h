#pragma once

//////////////////////////////////////////////////////////////////////////
//## Scenario manager
//## Des : 시나리오 기능의 메인 엔진이며 랜더링 & 액션 기능을 제공.
//////////////////////////////////////////////////////////////////////////
class CKTDGScenarioManager
{
public:
	CKTDGScenarioManager(void);
	~CKTDGScenarioManager(void);

	virtual	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual	HRESULT OnFrameRender( bool bFront = false );

	virtual HRESULT OnFrameRenderTopModal();

	virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual	HRESULT OnResetDevice();
	virtual	HRESULT OnLostDevice();

private:
	//CKTDXDeviceTexture*	m_pTestTex;
	_SCENARIO_OBJECT_::CTextureList	m_RenderTexList;
};

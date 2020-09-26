#pragma once

//////////////////////////////////////////////////////////////////////////
//## Main stage
//## Desc : 엔진에 등록되어 사용될 랜더 프레임
//##		- 컨트롤 데이터를 보관 & EventScene 엔진으로의 기능 전송
//////////////////////////////////////////////////////////////////////////

class CRenderFrm : public CKTDXStage
{
public:
	CRenderFrm(void);
	virtual ~CRenderFrm(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	void	SetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType, HWND hWnd );
	HWND	GetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType );
	HWND	GetDlgItem( _EST_OBJECT_::DLG_HWND dlgType, int iDlgItem );

	CEventScriptParsing * GetEventScriptParsing(){ return &m_EventScriptParsing; }

	CX2EventScene * GetX2EventScene(){ return &m_EventScene; }

	void			SetLastPlaySceneID(WCHAR* wszID){m_wstrLastPlaySceneID = wszID;}
	std::wstring	GetLastPlaySceneID(){return m_wstrLastPlaySceneID;}

public:
	//# 툴 옵션기능

	//## 랜더링 화면 더블클릭시 EventList 에서 랜더링된 리스트에 포커스 맞추기
	void	SetFocusPlaySceneID();

private:
	HWND	m_hWnd[_EST_OBJECT_::DH_END];
	CEventScriptParsing	m_EventScriptParsing;

	CX2EventScene	m_EventScene;

	std::wstring m_wstrLastPlaySceneID;
};

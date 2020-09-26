#pragma once

#include <vector>
using namespace std;

class CNewDialogTool	: public CKTDXStage
{
public:
	CNewDialogTool(void);
	~CNewDialogTool(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void	OpenScript( const WCHAR* scriptFilename = L"DialogTool.lua" );

private:

	KLuaManager*			m_pLuaManager;

};


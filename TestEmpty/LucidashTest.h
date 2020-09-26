#pragma once

#include <vector>
//#include <iostream>

class CLucidashTest : public CKTDXStage
{
	public:
		CLucidashTest(void);
		virtual ~CLucidashTest(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:
		CX2UnitManager m_UnitManger;
		vector<int> Datas;
		//CX2DungeonManager m_DungeonManager;
};

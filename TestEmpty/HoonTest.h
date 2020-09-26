#pragma once

#include <vector>

class CHoonTest : public CKTDXStage
{
	private:
		std::vector< int > EmergenceNumber;

	public:
		CHoonTest(void);
		virtual ~CHoonTest(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:
		CX2UnitManager m_UnitManger;
		//CX2DungeonManager m_DungeonManager;
};

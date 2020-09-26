#pragma once

#include "../X2Lib/X2SMTPMail.h"



class CItemViewer : public CKTDXStage
{
	public:

		enum VIEWER_DLG_MESSAGE
		{
			VDM_PREV = 0,
			VDM_NEXT,
			VDM_PRINT,
		};

	public:

		CItemViewer(void);
		virtual ~CItemViewer(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:

		void ResetUI();

		bool OpenScriptFile( const WCHAR* pFileName );
		

		int								m_NowViewPage;
		int								m_NowViewMaxPage;
		int								m_NowPrintPage;
		int								m_QuantityPerPage;

		CKTDGUIDialogType					m_pDialog;
		CKTDGFontManager::CKTDGFont*	m_pFont;

		CX2ItemManager*					m_pItemManager;

		vector< CKTDGUIDialogType >		m_vecDLG;
};

#pragma once

class CX2StateCommonBG : public CX2StateOption
{
	public:
		CX2StateCommonBG(void);
		virtual ~CX2StateCommonBG(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

		void SetOpenCommonBG( bool bCheck );

	protected:

		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool SetCommonBG();
		void BGEffectFrameMove( double fTime, float fElapsedTime );

		virtual void SetShowCommonBG();

		CKTDGUIDialogType			m_pDLGCommonBG;
		bool					m_bShowCommonBG;

		float					m_fCommonBGEffectTime;
		float					m_fCommonBGEffectCoolTime;
};

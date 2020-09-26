#pragma once

class CForfeitTest : public CKTDXStage
{
public:
	enum STATE_TEST_UI_CUSTOM_MSG
	{
		STUCM_RADIO1SET		= 0,
		STUCM_RADIO2SET		= 1,
		STUCM_RADIO3SET		= 2,

		STUCM_CHECK1SET		= 11,
		STUCM_CHECK2SET		= 12,
		STUCM_CHECK3SET		= 13,
		STUCM_CHECK4SET		= 14,

		STUCM_CHECK1UNSET	= 21, 
		STUCM_CHECK2UNSET	= 22,
		STUCM_CHECK3UNSET	= 23,
		STUCM_CHECK4UNSET	= 24,

        STUCM_IME			= 30,

		STUCM_BUTTONON		= 40,

		STUCM_COMBO_SELECT	= 50,

		STUCM_LIST_SELECT	= 60,

	};

	enum STATE_TEST_MODE{
		STM_BG1 = 0,
		STM_BG2,
		STM_BG3,
	};

public:
	CForfeitTest(void);
	~CForfeitTest(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
	
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	//CKTDGUIDialogType	m_pDLGTestBack;
	CKTDGUIDialogType	m_pDLGTestFront;

	STATE_TEST_MODE	m_NowTestMode;
	bool			m_bIsCheckBox1On;
	bool			m_bIsCheckBox2On;
	bool			m_bIsCheckBox3On;
	bool			m_bIsCheckBox4On;

	wstring			m_IMEBox;

};

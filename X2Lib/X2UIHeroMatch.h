#pragma once

class CX2UIHeroMatch
{

public:
	CX2UIHeroMatch();
	~CX2UIHeroMatch();

	enum X2_HERO_MATCH_UI_MSG
	{
		XHMUM_CLOSE			= 56000,
		XHMUM_ADD_USER,
		XHMUM_DELETE_USER,
		XHMUM_SHOW_COMMAND,
		XHMUM_CLOSE_ADD_USER,
		XHMUM_ADD_USER_OK,
		XHMUM_SHOW_MENU,
		XHMUM_WHISPER,
	};

	struct HeroMatchUserListControls
	{
		CKTDGUICheckBox*		m_pCheckBoxSelect;
		CKTDGUIStatic*			m_pStaticNickName;
		CKTDGUIButton*			m_pButtonNickName;
		CKTDGUIStatic*			m_pStaticServerName;
		CKTDGUIRadioButton*		m_pRadioButtonLogIn;
		CKTDGUIRadioButton*		m_pRadioButtonChannelLogIn;

		HeroMatchUserListControls() :
		m_pCheckBoxSelect( NULL ),
		m_pStaticNickName( NULL ),
		m_pButtonNickName( NULL ),
		m_pStaticServerName( NULL ),
		m_pRadioButtonLogIn( NULL ),
		m_pRadioButtonChannelLogIn( NULL )
		{}
	};

	void					InitUI();

	bool					UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool					UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool					MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void					SetOpen( bool bCheck );
	bool					GetShow(){ return m_bShow; }
private:
	std::vector< HeroMatchUserListControls > m_vecHeroMatchUserListControls;

	bool					Handler_EGS_ADMIN_HERO_PVP_USER_LIST_REQ();
	bool					Handler_EGS_ADMIN_HERO_PVP_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool					Handler_EGS_ADMIN_HERO_PVP_USER_ADD_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool					Handler_EGS_ADMIN_HERO_PVP_USER_ADD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool					Handler_EGS_ADMIN_HERO_PVP_USER_REMOVE_REQ();
	bool					Handler_EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool					Handler_EGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool					Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ();

	void					UpdateUserList();
	void					ClearList();
	void					ToggleHelpList();

	CKTDXStage*				m_pStage;
	CKTDGUIDialogType		m_pDlgHeroMatch;
	CKTDGUIDialogType		m_pDlgHeroAdd;
	CKTDGUIDialogType		m_pDlgHeroPopup;

	bool					m_bShow;

	short					m_SumDelta;

	UINT					m_iInfoMaxNum;

	UINT					m_iPickedUser;

	std::vector< KHeroPvpUserInfo >	m_vecUserList;
};
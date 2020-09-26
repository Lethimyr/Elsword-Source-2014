#pragma once

class CX2StateLogin : public CX2State
{
	public:
		enum STATE_LOGIN_UI_CUSTOM_MSG
		{
			SLUCM_LOGIN = 0,
			SLUCM_CHANGE_STATE_MEMBER_JOIN,

			SLUCM_CONNECT_SERVER,				// fix!! 사용하는 곳이 없네요 

			SLUCM_AUTO_LOGIN,
			SLUCM_NOT_AUTO_LOGIN,

			SLUCM_NEXON_KOREA_LOGIN,
			SLUCM_NEXON_KOREA_LOGIN_COMPLETE,
//#ifdef _HOMEPAGE_MOVE_
			SLUCM_HOMEPAGE_MOVE,
			SLUCM_REGISTER_MOVE,
			SLUCM_FACEBOOK_MOVE,
			SLUCM_STEAM_COMMUNITY_MOVE,
//#endif _HOMEPAGE_MOVE_
//#ifdef RULE_AGREEMENT
			SLUCM_RULE_AGREEMENT,
//#ifdef NEW_RULE_AGREEMENT
			SLUCM_NEW_RULE_AGREEMENT,
//#endif NEW_RULE_AGREEMENT
//#endif RULE_AGREEMENT
//#ifdef SAVE_LOGIN_ID
			SLUCM_REMEMBER_ID_CHECK_ON,
			SLUCM_REMEMBER_ID_CHECK_OFF,
//#endif SAVE_LOGIN_ID
//#ifdef SERV_PUBLISHER_MATRIXKEY
			SLUCM_MATRIX_NUMBER_MOUSE_OVER,
			SLUCM_MATRIX_NUMBER_MOUSE_OUT,
			SLUCM_MATRIX_NUMBER_MOUSE_UP,
			SLUCM_MATRIX_OK,
			SLUCM_MATRIX_CLEAR,
//#endif SERV_PUBLISHER_MATRIXKEY
//#ifdef SERV_PUBLISHER_OTP
			SLUCM_OTP_ON,
			SLUCM_OTP_OFF,
//#endif SERV_PUBLISHER_OTP
//#ifdef LOGIN_KEYBOARD_SECURITY
			SLUCM_KEYBOARD,
			SLUCM_KEYBOARD_KEY,
			SLUCM_KEYBOARD_ENTER,
			SLUCM_KEYBOARD_SHIFT,
			SLUCM_KEYBOARD_CTRL,
			SLUCM_KEYBOARD_BACK,
			SLUCM_KEYBOARD_EDIT_FOCUSOUT,
			SLUCM_KEYBOARD_CLOSE,
//#endif LOGIN_KEYBOARD_SECURITY
//#ifdef USE_ACCOUNT_DOMAIN
			SLUCM_DOMAIN_TYPE_PLAYID		=	30, 
			SLUCM_DOMAIN_TYPE_TCG			=	31, 
//#endif USE_ACCOUNT_DOMAIN
		};

	public:
		CX2StateLogin(void);
		virtual ~CX2StateLogin(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		
	protected:
		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void RetrieveUserIDnPassword();
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		void StateChangeLoading();
		void InitManualLogin();

//{{ kimhc // 2010.5.11 // 사내에서 자동 로그인 할 수 있는 기능
#ifdef	AUTO_LOGIN_IN_HOUSE
		void ReadIDAndPassword();
#endif	AUTO_LOGIN_IN_HOUSE
//}} kimhc // 2010.5.11 // 사내에서 자동 로그인 할 수 있는 기능

		CKTDGUIDialogType	m_pDLGLoginBack;
		CKTDGUIDialogType	m_pDLGLoginFront;
		
#ifdef NEW_LOGIN_PROCESS
#ifdef ADD_SERVER_GROUP
		bool OpenScriptServerGroupFile();
#endif //ADD_SERVER_GROUP
		bool TryVerifyAccount();
		bool ConnectToChannelServer();
#ifdef CLIENT_DIRECT_CONNECT_AUTH_SERVER
		bool VerifyPublisherAccoutByDirect(OUT std::string& str);
#endif // CLIENT_DIRECT_CONNECT_AUTH_SERVER
		bool Handler_ECH_VERIFY_ACCOUNT_REQ();
		bool Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // NEW_LOGIN_PROCESS

#ifdef SERV_LOGIN_MAC_ADDRESS
		std::wstring GetMACAddress() const;
#endif SERV_LOGIN_MAC_ADDRESS

#ifdef SERV_GLOBAL_AUTH
		bool Handler_ECH_SECURITY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_ECH_SECURITY_AUTH_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef SERV_PUBLISHER_OTP
		bool Handler_SLUCM_OTP_ON( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_OTP_OFF( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		std::wstring			m_wstrOTP;
#endif SERV_PUBLISHER_OTP
#ifdef SERV_PUBLISHER_MATRIXKEY
		bool Handler_SLUCM_MATRIX_OK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_MATRIX_NUMBER( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_MATRIX_CLEAR( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void CreateMatrixPopup();
		void ResetMatrix(const std::wstring& wstrCoordinate);
		void FrameMoveMatrixPopup(float fElapsedTime);

		LPCWSTR GetMatrixString();

		virtual bool HandleMsgByESCImp();

		CKTDGUIDialogType		m_pDLGMatrix;
		bool					m_bDeleteMatrixDialog;
		CKTDGUIButton*			m_pNumButton[10];
		D3DXVECTOR2				m_vCenter;
		float					m_fRadius;
		float					m_fButtonSize;
		float					m_fRotation;
		bool					m_bRotate;
		UINT					m_nInput;
		std::wstring			m_wstrMatrixCoordinate;
		std::wstring			m_wstrMatrix;
#endif SERV_PUBLISHER_MATRIXKEY
#endif SERV_GLOBAL_AUTH

#ifdef LOGIN_KEYBOARD_SECURITY
		CKTDGUIControl* GetLastFocusedControl();
		void AppendKeyValue(const std::wstring& wstrValue);

		bool Handler_SLUCM_KEYBOARD( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_KEYBOARD_CLOSE( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); //2011.10.20 lygan_조성욱 // 중국 로그인 UI 변경 관련
		bool Handler_SLUCM_KEYBOARD_KEY( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_KEYBOARD_ENTER( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_KEYBOARD_SHIFT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_KEYBOARD_BACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_KEYBOARD_EDIT_FOCUSOUT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		CKTDGUIDialogType						m_pDLGKeyboard;
		CKTDGUIControl*							m_pLastFocusedControl;
		bool									m_bKeyboardShift;

		bool									m_bKeyBoardUse;

		std::vector<std::wstring>				m_vecKeyList;
		std::map<std::wstring, std::wstring>		m_mapNormalKeyValue;
		std::map<std::wstring, std::wstring>		m_mapShiftedKeyValue;
#endif LOGIN_KEYBOARD_SECURITY
#ifdef	USE_ACCOUNT_DOMAIN
		bool Handler_SLUCM_DOMAIN_TYPE_PLAYID( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_SLUCM_DOMAIN_TYPE_TCG( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	USE_ACCOUNT_DOMAIN
#ifdef SAVE_LOGIN_ID
		bool SaveID();
		bool LoadID();
		bool DeleteSavedID();
#endif SAVE_LOGIN_ID

#ifdef RULE_AGREEMENT
		void OpenRuleAgreementDlg();
#ifdef NEW_RULE_AGREEMENT
		void OpenNewRuleAgreementDlg();
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
//{{ 2013.03.13 조효진	회원가입 웹페이지를 버튼을 누르면 띄워줌
#ifdef MEMBER_JOIN_FROM_WEB
		std::wstring GetStrMemberJoinWeb();
#endif MEMBER_JOIN_FROM_WEB
//}}
#ifdef FIRST_SCREEN_TEST

		CKTDXDeviceTexture*		m_pTexutureBG;

#endif // FIRST_SCREEN_TEST

#ifdef RULE_AGREEMENT
		CKTDGUIDialogType	m_pDLGRuleAggrement;
		CKTDGUIDialogType	m_pDLGBackGround;
#ifdef NEW_RULE_AGREEMENT
		CKTDGUIDialogType	m_pDLGNewRuleAggrement;
		CKTDGUIDialogType	m_pDLGNewBackGround;
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
};

#pragma once

class CX2StateJoin : public CX2State
{
	public:
		enum STATE_JOIN_UI_CUSTOM_MSG
		{
			SJUCM_MEMBER_JOIN = 0,
			SJUCM_BACK_TO_LOGIN,
		};

	public:
		CX2StateJoin(void);
		virtual ~CX2StateJoin(void);



	protected:
		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//bool UIServerTimeOutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		bool ServerConnectReq( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool ServerConnectAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//bool ServerConnectTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool NewUserJoinReq( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool NewUserJoinAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		//bool NewUserJoinTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		bool JoinErrorCheck();

		CKTDGUIDialogType	m_pDLGJoinBack;
		CKTDGUIDialogType	m_pDLGJoinFront;

		wstring			m_UserID;
		wstring			m_UserPassword;
		wstring			m_UserName;
		//wstring			m_UserNickName;
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
		bool			m_bServerG;
		bool			m_bServerS;
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
};

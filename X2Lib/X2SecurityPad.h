#pragma once

#ifdef SERV_SECOND_SECURITY
class CX2SecurityPad
{
public:
	enum SECURITY_PAD_UI_MSG
	{
		SPUM_DIAL_NUMBER_0			= 49000,
		SPUM_DIAL_NUMBER_1,
		SPUM_DIAL_NUMBER_2,
		SPUM_DIAL_NUMBER_3,
		SPUM_DIAL_NUMBER_4,
		SPUM_DIAL_NUMBER_5,
		SPUM_DIAL_NUMBER_6,
		SPUM_DIAL_NUMBER_7,
		SPUM_DIAL_NUMBER_8,
		SPUM_DIAL_NUMBER_9,
		SPUM_DIAL_NUMBER_10,
		SPUM_DIAL_NUMBER_11,
		SPUM_EXIT,
		SPUM_OK,
		SPUM_CANCEL,
		SPUM_DELETE_PASSWORD_VERIFY_OK,
// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, A使見AO ╳取﹌／A﹌O ╳芋使帚“╳i, kimjh
		SPUM_CHANGE_STATE_CHANGE_PASSWORD,
		SPUM_CHANGE_STATE_RELEASE_PASSWORD,
// #endif // REFORM_ENTRY_POINT	// 13-11-11, A使見AO ╳取﹌／A﹌O ╳芋使帚“╳i, kimjh
	};

	enum SECURITY_PAD_STATE
	{
		SPS_CLOSE,
		SPS_GAME_START,
		SPS_CREATE_PASSWORD,
		SPS_CHANGE_PASSWORD,
		SPS_RELEASE_PASSWORD,
// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, A使見AO ╳取﹌／A﹌O ╳芋使帚“╳i, kimjh
		SPC_SELECT_STATE,
// #endif // REFORM_ENTRY_POINT	// 13-11-11, A使見AO ╳取﹌／A﹌O ╳芋使帚“╳i, kimjh
	};


public:
	CX2SecurityPad();
	virtual ~CX2SecurityPad();

	bool GetShow(){ return m_bShow; }
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh A使見AO ╳取﹌／A﹌O ╳芋使帚“╳i
	void SetShow(bool bShow, SECURITY_PAD_STATE eState = SPS_CLOSE, bool bIsDeleteOldDialog = true );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh A使見AO ╳取﹌／A﹌O ╳芋使帚“╳i
	void SetShow(bool bShow, SECURITY_PAD_STATE eState = SPS_CLOSE );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh A使見AO ╳取﹌／A﹌O ╳芋使帚“╳i

	void ShuffleNumber();

	void ClearInputPassword();
	void ProcessPassword();
	void UpdateUIMarble();

	bool GetUseSecondPW(){ return m_bUseSecondPW; }
	void SetUseSecondPW(bool val){ m_bUseSecondPW = val; }

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_SECOND_SECURITY_K3R_AUTH
	bool Handler_ECH_SECURITY_AUTH_REQ();
	bool Handler_ECH_SECURITY_AUTH_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SECOND_SECURITY_K3R_AUTH

	bool Handler_EGS_AUTH_SECOND_SECURITY_REQ(); // Phase Count 1
	bool Handler_EGS_AUTH_SECOND_SECURITY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CREATE_SECOND_SECURITY_REQ(); // Phase Count 2
	bool Handler_EGS_CREATE_SECOND_SECURITY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DELETE_SECOND_SECURITY_REQ(); // Phase Count 1
	bool Handler_EGS_DELETE_SECOND_SECURITY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DELETE_SECOND_SECURITY_VERIFY_REQ();
	bool Handler_EGS_DELETE_SECOND_SECURITY_VERIFY_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_COMPARE_SECOND_SECURITY_PW_REQ(); // Phase Count 3
	bool Handler_EGS_COMPARE_SECOND_SECURITY_PW_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_SECOND_SECURITY_PW_REQ(); // Phase Count 3
	bool Handler_EGS_CHANGE_SECOND_SECURITY_PW_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, A帚AO ▽╱AＤ ﹉昆芋i, kimjh
	void AutoAuthoritySecondSecurity ();
#endif // REFORM_ENTRY_POINT	// 13-11-11, A帚AO ▽╱AＤ ﹉昆芋i, kimjh
#ifdef FIX_REFORM_ENTRY_POINT_2ND // ▽eA取E芋, A帚AO ▽╱AＤ ﹉昆芋i 卹oA５ 2A㊣
	bool GetIsAutoAuthoritySecondSecurityBegin() const { return m_bIsAutoAuthorityPassworBegin; }
#endif // FIX_REFORM_ENTRY_POINT_2TH // ▽eA取E芋, A帚AO ▽╱AＤ ﹉昆芋i 卹oA５ 2A㊣

#ifdef FIX_REFORM_ENTRY_POINT_7TH		// ▽eA取E芋, A帚AO ▽╱AＤ ﹉昆芋i 7A㊣, kimjh
	bool Handler_EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ(); // Phase Count 3
	bool Handler_EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool Handler_EGS_GET_SECOND_SECURITY_INFO_REQ ();
	bool Handler_EGS_GET_SECOND_SECURITY_INFO_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // FIX_REFORM_ENTRY_POINT_7TH	// ▽eA取E芋, A帚AO ▽╱AＤ ﹉昆芋i 7A㊣, kimjh


protected:
	CKTDGUIDialogType		    m_pDLGSecurityPad;
	CKTDGUIStatic*				m_pStaticMemo;				

	bool						m_bShow;
	bool						m_bUseSecondPW;
	int							m_iButtonSequence[12];
	WCHAR						m_cPassword[7];
	wstring						m_wstrPassword_Temp1;
	wstring						m_wstrPassword_Temp2;
	wstring						m_wstrPassword_Delete;
	int							m_iPasswordIndex;
	SECURITY_PAD_STATE			m_eState;
	int							m_iPhase;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, A帚AO ▽╱AＤ ﹉昆芋i, kimjh
	wstring						m_wstrAutoAuthorityPassword;
#endif // REFORM_ENTRY_POINT	// 13-11-11, A帚AO ▽╱AＤ ﹉昆芋i, kimjh

#ifdef FIX_REFORM_ENTRY_POINT_2ND // ▽eA取E芋, A帚AO ▽╱AＤ ﹉昆芋i 卹oA５ 2A㊣
	bool						m_bIsAutoAuthorityPassworBegin;
#endif // FIX_REFORM_ENTRY_POINT_2TH // ▽eA取E芋, A帚AO ▽╱AＤ ﹉昆芋i 卹oA５ 2A㊣	

};
#endif SERV_SECOND_SECURITY
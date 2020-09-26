/**@file	X2DeparturePrevention.h
   @breif	이탈 유저 방지 시스템 
*/

#pragma once

#ifdef DEPARTURE_PREVENTION_SYSTEM

/** @class : CX2PremuimBuff
	@brief : 버프를 받을 유저에게 버프 적용 및 화면에 UI 표시
	@date  : 2011/05/31
*/
class CX2DeparturePrevention
{
public:
	// public variable start
	enum STATE_DEPARTURE_PREVENTION_PREV_STATE
	{
		SDPPS_NONE				= 0,
		SDPPS_EXIT				= 1,
		SDPPS_CHARACTER_SELECT	= 2,
		SDPPS_SERVER_SELECT		= 3,
	};

	enum STATE_DEPARTURE_PREVENTION_UI_CUSTOM_MSG
	{
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_EXIT	= 51000,	/// 이탈 유저 보상 선택 대화상자 나가기 이벤트
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_ONE		= 51001,	/// 첫 번째 보상 선택
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_TWO		= 51002,	/// 두 번째 보상 선택
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_THREE	= 51003,	/// 세 번째 보상 선택
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_FOUR	= 51004,	/// 네 번째 보상 선택
		SDPRUCM_DEPARTURE_PREVENTION_REWARD_FIVE	= 51005,	/// 다섯 번째 보상 선택

		SDPRUCM_DEPARTURE_PREVENTION_NOREWARD_EXIT	= 51006,	/// 보상 받지않고 종료

		SDPRUCM_DEPARTURE_PREVENTION_RESULT_OK		= 51007,	/// 보상 완료(게임으로 돌아감)
	};
	// public variable start

public:
	// public function start
	CX2DeparturePrevention(void);
	~CX2DeparturePrevention(void);

	//{{ 이벤트 처리
	bool UICustomEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	//{{ 프레임 당 처리
	HRESULT OnFrameMove( IN double fTime, IN float fElapsedTime );
	//}}

	//{{ 서버와 통신
	bool Handler_EGS_OUT_USER_RETAINING_REQ();
	bool Handler_EGS_OUT_USER_RETAINING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_RETAINING_SELECT_REWARD_REQ( IN int iVal_ );
	bool Handler_EGS_RETAINING_SELECT_REWARD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool UIServerEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	void SetPrevState( IN STATE_DEPARTURE_PREVENTION_PREV_STATE eVal_ );
	// public function end

private:
	// private function start
	void Clear();
	void UpdateRewardUI( IN KEGS_OUT_USER_RETAINING_ACK &kEvent_ );
	void QuitGame();
	// private function end

private:
	// private variable start
	CKTDGUIDialogType	m_pDLGLeaveUserReward;		/// 이탈 유저 보상 선택 대화상자
	CKTDGUIDialogType	m_pDLGLeaveUserResult;		/// 이탈 유저 보상 선택 결과 대화상자
	CKTDGUIDialogType	m_pMsgBoxReQuitGame;		/// 받지 않고 게임 종료

	STATE_DEPARTURE_PREVENTION_PREV_STATE m_ePrevState;		/// 이탈 유저 확인 전 요청 상태
	// private variable start
};

#endif


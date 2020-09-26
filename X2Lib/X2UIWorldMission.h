//{{ 허상형 : [2011/3/22/] //	월드 미션
#pragma once

#ifdef SERV_INSERT_GLOBAL_SERVER

const int REWARD_SUCCESS_ITEM_ID	= 60001797;	//	성공시 지급되는 아이템
const int REWARD_FAIL_ITEM_ID		= 60001798;	//	실패시 시간에따라 조각으로 지급되는 아이템

class CX2UIWorldMission
{
public:
	enum	WORLD_MISSION_UI_MSG
	{
		WMUM_REWARD_CLOSE	= 46200,
	};

	CX2UIWorldMission();
	~CX2UIWorldMission();

	//bool SetShowRewardDlg( bool bEnable );
	void SetShowTimeDlg( bool bEnable );
	//bool SetShowEXPBonusDlg( bool bEnable );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//  	void SetReservedRewardItem( bool bEnable, int iItemID = 0, int iQuantity = 0 );
//  	bool GetReservedRewardItem()	{ return m_bReservedReward; }

	void SetForceUpdateCrystalUI(){m_bForceUpdate = true;}

	void UpdateTimeDlg( int iTime );

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-15
	void UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ = 0, int iNumOfMaxCrystalsProtected_ = 1 );
#else // SERV_NEW_DEFENCE_DUNGEON
	void UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ );
#endif // SERV_NEW_DEFENCE_DUNGEON

	//int GetNumOfCrystalsProtected() const { return m_iNumOfCrystalsProtected; }
	//void SetNumOfCrystalsProtected( int iNumOfCrystalsProtected_ ) { m_iNumOfCrystalsProtected = iNumOfCrystalsProtected_; }

	void SetShowCrystalPictureData( CKTDGUIStatic* pStaticTime_, int iIndexOfColor_, bool bShow_ );

	CKTDGUIDialogType			GetTimeDlg()	{ return m_pDlgTime; }
	//CKTDGUIDialogType			GetEXPBonusDlg()	{ return m_pDlgEXPBonus; }

	bool GetEnableTimeDlg() const { return m_bEnableTimeDlg; }
	void SetEnableTimeDlg( bool bEnableTimeDlg_ ) { m_bEnableTimeDlg = bEnableTimeDlg_; }

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-27
	bool GetIsFlickerDanger() const { return m_bIsFlickerDanger; }		/// 어둠의 문 월드 미션 UI의 Danger 마크가 깜빡이는 여부 반환
	void SetIsFlickerDanger(bool val) { m_bIsFlickerDanger = val; }		/// 어둠의 문 월드 미션 UI의 Danger 마크가 깜빡이는 여부 설정
#endif // SERV_NEW_DEFENCE_DUNGEON

private:
	//CKTDGUIDialogType			m_pDlgReward;
	CKTDGUIDialogType			m_pDlgTime;
	//CKTDGUIDialogType			m_pDlgEXPBonus;

// 	bool						m_bReservedReward;
// 	int							m_iRewardItemID;
// 	int							m_iRewardQuantity;
	int							m_iNumOfCrystalsProtected;	// 현재까지 지켜진 크리스탈의 갯수
	bool						m_bForceUpdate;
	bool						m_bEnableTimeDlg;

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-03-27
	bool						m_bIsFlickerDanger;			/// 어둠의 문 월드 미션 UI의 Danger 마크가 깜빡이는 여부
#endif // SERV_NEW_DEFENCE_DUNGEON
	
};

#endif	SERV_INSERT_GLOBAL_SERVER
//}} 허상형 : [2011/3/22/] //	월드 미션

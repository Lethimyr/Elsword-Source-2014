#pragma once

class CX2CharPopupMenu : public CX2PopupUIBase
{   	
public:
	enum USER_MENU_UI_MSG
	{		
		UMUI_INFO = 17000,	/// 유저정보
		UMUI_WATCH,			/// 엿보기
		UMUI_WHISPER,		/// 귓속말
		UMUI_FRIEND,		/// 친구요청
		UMUI_DISCIPLE,		/// 사제요청
		UMUI_LEADER,		/// 파티장임명
		UMUI_PARTY,			/// 파티요청
		UMUI_INVITE,		/// 파티초대
		UMUI_OUT,			/// 강퇴
		UMUI_TRADE,			/// 개인거래
		//{{ kimhc // 2009-09-29 // 길드 관련 메시지 추가
		UMUI_INVITE_GUILD_MEMBER,	/// 길드초대
		//}} kimhc // 2009-09-29 // 길드 관련 메시지 추가
		UMUI_INFO_SIMPLE,
		UMUI_MOVE_TO_PARTY,	/// 파티 소속필드로 이동
		UMUI_INPUT_PARTY,	/// 초대할 캐릭터명 입력
		UMUI_DROP_OUT,		/// 파티 탈퇴
		UMUI_INPUT_EXIT,	/// 캐릭 초대 다이얼로그 종료
		UMUI_INVITE_PARTY_NAME,	/// 캐릭터 이름으로 파티 초대
		UMUI_SELECT_INVITE_MEMBER_SEVER,//초대할 파티원 서버
		UMUI_EXIT,			/// 종료
//#ifdef ADDED_RELATIONSHIP_SYSTEM
		UMUI_COUPLE,		// 커플 초대
		UMUI_SUMMON,
//#endif // ADDED_RELATIONSHIP_SYSTEM

	};

	enum USER_MENU
	{
		UM_NONE = 0,
		UM_NORMAL_NORMAL,		// 일반->일반
		UM_NORAML_PARTY,		// 일반->파티원
		UM_NORMAL_PARTYL,		// 일반->파티장
		UM_PARTY_NORMAL,		// 파티원->일반
		UM_PARTY_PARTY,			// 파티원->파티원
		UM_PARTY_PARTYL,		// 파티원->파티장
		UM_PARTYL_NORMAL,		// 파티장->일반
		UM_PARTYL_PARTY,		// 파티장->파티원
	};

    CX2CharPopupMenu();
    virtual ~CX2CharPopupMenu();

	//void SetStage( CKTDXStage* pNowState  );
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShowMenu() const { return m_bShow; }	
	virtual void OpenUserPopupMenu( UidType iUnitUID, bool bPartyMember = false );
	virtual void ClosePopupMenu();

	//CKTDGUIDialogType GetDialog() { return m_pDlgMenu; }

private:
	void SetUnit( UidType uidUnitUID, bool bPartyMember = false );
	void SetMode( USER_MENU eMode, bool bPartyMember = false );
	bool SetPopupMenu( UidType iUnitUID, bool bPartyMember = false );
	void OnPartyInput( bool bOn_ );
	void SetOnPopup();

protected:    
    //CX2State*				m_pNowState;
    CKTDGUIDialogType			m_pDlgMenu;

private:	
	//int						m_nMenuCount;
	//CX2Unit*				m_pUnit;
	UidType					m_iUid;
	wstring					m_wstrName;
	int						m_iUnitLevel;
	CX2Unit::UNIT_CLASS		m_eUnitClass;

	//bool							m_bShow;

	CKTDGUIControl::CPictureData*	m_pPicMiddle1;
	CKTDGUIControl::CPictureData*	m_pPicBottom1;
	CKTDGUIControl::CPictureData*	m_pPicMiddle2;
	CKTDGUIControl::CPictureData*	m_pPicBottom2;
    
	CKTDGUIControl::CPictureData*	m_pPicCharacter;		/// 캐릭터 이미지

	CKTDGUIControl::CPictureData*	m_pPicLevelTen[10];		/// 레벨 십단위
	CKTDGUIControl::CPictureData*	m_pPicLevelOne[10];		/// 레벨 일단위

	CKTDGUIStatic*					m_pStaticName;			/// 캐릭터 이름

	CKTDGUIButton*					m_pButtonSimpleInfo;	/// 유저정보(심플)
	CKTDGUIButton*					m_pButtonUserInfo;		/// 유저정보(살펴보기)
	CKTDGUIButton*					m_pButtonWatch;			/// 엿보기
	CKTDGUIButton*					m_pButtonwhisper;		/// 귓속말
	CKTDGUIButton*					m_pButtonfriend;		/// 친구요청
	CKTDGUIButton*					m_pButtondisciple;		/// 사제요청
	CKTDGUIButton*					m_pButtonleader;		/// 파티장임명
	CKTDGUIButton*					m_pButtonparty;			/// 파티요청
	CKTDGUIButton*					m_pButtoninvite;		/// 파티초대
	CKTDGUIButton*					m_pButtonout;			/// 강퇴
	CKTDGUIButton*					m_pButtontrade;			/// 개인거래

	//{{ 허상형 : [2009/9/18] //	길드 초대 버튼
	CKTDGUIButton*					m_pButtonInviteGuild;	///	길드초대
#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIButton*					m_pButtonInviteCouple;	///	커플초대
	CKTDGUIButton*					m_pButtonSummonCouple;	///	커플소환
#endif // ADDED_RELATIONSHIP_SYSTEM
};

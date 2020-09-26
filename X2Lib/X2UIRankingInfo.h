//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI

#pragma once

namespace		_CONST_UIRANKING_INFO_
{
	const	int		g_iNumRankingPerPage		= 5;		// 한 페이지 당 나타낼 수 있는 랭킹 갯수
	const	int		g_iLimitRankingToDisplay	= 2000;		// 이 뒤의 순위는 알수없음 으로 표시
	const	int		g_iRankLimit				= 3;		// 순위는 3자리 까지만
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	const	int		g_iMaxPageLimit				= 21;
#else // SERV_HENIR_RANKING_GROUP_FIX
	const	int		g_iMaxPageLimit				= 20;
#endif // SERV_HENIR_RANKING_GROUP_FIX
}

#ifdef SERV_LOCAL_RANKING_SYSTEM
namespace		_CONST_UIRANKING_FRIEND_INFO_
{
	const	int		g_iNumRankingPerPage		= 8;		//한 페이지당 나타내는 랭킹 갯수
	const	int		g_iRankLimit				= 4;		//순위는 4자리까지
	const	int		g_iMaxPageLimit				= 143;		//최대 페이지 갯수
	const	int		g_iLimitRanking				= 1000;		//총 1000위까지의 순위만 받는다.
	const	int		g_iLimitStrongGage			= 6;		//근성도 포인트는 최대 6자리까지
}
#endif //SERV_LOCAL_RANKING_SYSTEM

class CX2UIRankingInfo
{
public:

	enum UI_RANKING_INFO_CUSTOM_MSG
	{
		URICM_EXIT							= 31000,
		URICM_SELECT_HENIR_TAP,
		URICM_SELECT_PVP_TAP,
		URICM_SELECT_LEVEL_TAP,
		URICM_SELECT_DAY_RADIO_BUTTON,
		URICM_SELECT_WEEK_RADIO_BUTTON,
		URICM_SELECT_MONTH_RADIO_BUTTON,
		URICM_PUSH_PREV_PAGE,
		URICM_PUSH_NEXT_PAGE,

//{{ 허상형 : [2009/8/21] //	신학기 이벤트 탭
#ifdef NEW_TERM_EVENT
		URICM_SELECT_EVENT_TAP,

		URICM_SELECT_EVENT_HENIR_BUTTON,
		URICM_SELECT_EVENT_DUNGEON_BUTTON,
		URICM_SELECT_EVENT_KILL_BUTTON,
		URICM_SELECT_EVENT_DEATH_BUTTON,	
#endif	//	NEW_TERM_EVENT
//}} 허상형 : [2009/8/21] //	신학기 이벤트 탭

//#ifdef SERV_LOCAL_RANKING_SYSTEM
		URICM_SELECT_WEEK_PVP_TAP			= 31014,
		URICM_SELECT_DUNGEON_FIELD_TAP,
		URICM_SELECT_AREA_RADIO_BUTTON,
		URICM_SELECT_GUILD_RADIO_BUTTON,
		URICM_SELECT_FRINED_CHECK,
		URICM_PAGE_SEARCH_FOCUS,
		URICM_PAGE_SEARCH_ENTER,
		URICM_OPEN_POPUP_MENU,
		URICM_POPUP_SELECT_CHARINFO,
		URICM_POPUP_SELECT_FRINED,
		URICM_POPUP_SELECT_WHISPER,
		URICM_PAGE_SERACH_CHANGE,
		URICM_SELECT_CHARACTER_CHECK,
		URICM_DUMMY_SELECT,		//아무것도 선택 안함. 길드버튼 비활성화를 위해 추가
		URICM_DBCLK_CHAINFO,
		URICM_SELECT_MALE_CHECK,
		URICM_SELECT_FEMALE_CHECK,
// #ifdef ADDED_RELATIONSHIP_SYSTEM
		URICM_INVITE_COUPLE,
// #endif ADDED_RELATIONSHIP_SYSTEM
//#endif //SERV_LOCAL_RANKING_SYSTEM

//#ifdef NEW_HENIR_DUNGEON
		URICM_SELECT_HERO_BUTTON	= 31032,
//#endif // NEW_HENIR_DUNGEON

	};

	////////////////////////////////////////////////////
	// 오현빈 // 2013-09-23 // 값 추가 하려면 Enum.h의 RANKING_TYPE과 맞춰야 함
	enum RANKING_TYPE 
	{
		RT_DAY_RANKING		= 0,
		RT_WEEK_RANKING,
		RT_MONTH_RANKING,
		RT_LEVEL_RANKING,			// enum.h의 RANKING_TYPE의 RT_DUNGEON_RANKING과 같음
		RT_PVP_RANKING,

		//{{ 허상형 : [2009/8/20] //	신학기 이벤트 관련 enum
#ifdef NEW_TERM_EVENT
		RT_EVENT_DUNGEON,
		RT_EVENT_HENIR,
		RT_EVENT_PVP_KILL,
		RT_EVENT_PVP_DIE,
#endif	//	NEW_TERM_EVENT
		//}} 허상형 : [2009/8/20] //	신학기 이벤트 관련 enum

#ifdef SERV_LOCAL_RANKING_SYSTEM
		RT_WEEK_PVP_TAP,
		RT_DUNGEON_FIELD_TAP,
		RT_AREA,
		RT_GUILD,
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef NEW_HENIR_DUNGEON
		RT_HERO_RANKING = 9,
#endif // NEW_HENIR_DUNGEON
		RT_MAX_NUM,
	};

#ifdef SERV_LOCAL_RANKING_SYSTEM
	struct ColumnForRankFriend
	{
		u_char			ucLevel;				// 유닛 레벨
		byte			byteGender;				// 유저 성별
		int				UnitClass;				// 유닛 클래스
		int				iArea;					// 지역
		__time64_t		iBirth;					// 생일
		UidType			m_iUserUID;				// 유닛 UID
		int				m_iRanking;				// 순위
		CKTDGUIStatic*	m_pStaticRanking;		// 순위 string
		CKTDGUIStatic*	m_pStaticClass;			// 클래스 아이콘
		CKTDGUIStatic*	m_pStaticNickName;		// 닉네임 string
		CKTDGUIStatic*	m_pStaticGender;		// 성별 아이콘
		CKTDGUIStatic*	m_pStaticPoint;			// 근성도 수치 or 주간 누적 AP 수치 텍스처
		CKTDGUIButton*	m_pButtonPopUp;			// 오른쪽 클릭시 팝업 출력
		wstring			m_wstsrIntroduce;		// 자기 소개
		ColumnForRankFriend() : m_wstsrIntroduce(L"")
		{
			byteGender			= 0;
			UnitClass			= 0;
			m_iUserUID			= 0;
			m_iRanking			= 0;
			m_pStaticRanking	= NULL;
			m_pStaticClass		= NULL;
			m_pStaticNickName	= NULL;
			m_pStaticGender		= NULL;
			m_pStaticPoint		= NULL;
			m_pButtonPopUp		= NULL;
		}
	};

	struct CommonMenuCategory
	{
		CKTDGUIStatic*			m_pStaticCommonInfo;
		CKTDGUIRadioButton*		m_pRadioArea;
		CKTDGUIRadioButton*		m_pRadioGuild;
		CKTDGUICheckBox*		m_pCheckFrield;
		CKTDGUICheckBox*		m_pCheckCharacter;
		CKTDGUICheckBox*		m_pCheckMale;
		CKTDGUICheckBox*		m_pCheckFemale;

		CommonMenuCategory()
		{
			m_pStaticCommonInfo	= NULL;
			m_pRadioArea		= NULL;
			m_pRadioGuild		= NULL;
			m_pCheckFrield		= NULL;	
			m_pCheckCharacter	= NULL;
			m_pCheckMale		= NULL;
			m_pCheckFemale		= NULL;
		}
	};

	enum BAR_SELECT //백그라운드에서 Bar(가로줄, 세로줄) 선택
	{
		BS_OLDBAR		= 0,
		BS_NEWBAR,
//#ifdef LOCAL_RANKING_UPDATE
		BS_NONE,
//#endif //LOCAL_RANKING_UPDATE
	};

	enum GENDER_SELECT //성별 선택
	{
		GS_NONE			= 0,
		GS_MALE,
		GS_FEMALE,
	};
#endif //SERV_LOCAL_RANKING_SYSTEM

	struct ColumnForRank
	{
		int				m_iRankingForOthers;				// 다른 사람들의 순위
		CKTDGUIStatic*	m_pStaticRanking;					// 유저의 랭킹string(유저는 string, 다른 유저는 picture 3개)
		CKTDGUIStatic*	m_pStaticClass;						// 유저의 클래스 아이콘
		CKTDGUIStatic*	m_pStaticNickName;					// 닉네임string
		
		CKTDGUIStatic*	m_pStaticClearStage;				// 유저의 클리어스테이지string
		CKTDGUIStatic*	m_pStaticPlayTime;					// 걸린시간string 용
		CKTDGUIButton*	m_pButtonRegistredDate;				// 헤니르 시공의 기록이 등록된 날짜(마우스 업 시 출력)

		CKTDGUIStatic*	m_pStaticPvpEmblem;					// 유저의 대전 등급
		CKTDGUIStatic*	m_pStaticWin;						// 유저의 승을 나타내는 string
		CKTDGUIStatic*	m_pStaticLose;						// 유저의 패를 나타내는 string
		CKTDGUIStatic*	m_pStaticExp;						// 유저의 경험치string

		ColumnForRank() 
		{
			m_iRankingForOthers				= 0;
			m_pStaticRanking				= NULL;
            m_pStaticNickName				= NULL;
			m_pStaticClass					= NULL;
	
			m_pStaticClearStage				= NULL;
			m_pStaticPlayTime				= NULL;
			m_pButtonRegistredDate			= NULL;

			m_pStaticPvpEmblem				= NULL;
			m_pStaticWin					= NULL;
			m_pStaticLose					= NULL;
			m_pStaticExp					= NULL;
		}
	};

public:
	CX2UIRankingInfo( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	~CX2UIRankingInfo();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );

	//{{ 허상형 : [2009/8/20] //	이벤트 컨트롤 활성화 함수
#ifdef NEW_TERM_EVENT
	void				InitUIRankingEventControl( bool bShow );
#endif	//	NEW_TERM_EVENT
	//}} 허상형 : [2009/8/20] //	이벤트 컨트롤 활성화 함수


private:
	void				InitUIRankingInfo();
	void				InitMyColumn();
	void				InitOthersColumn();

	void				SetShowControlsByType( RANKING_TYPE type, bool bShow );
	void				SetShowMyInfoByType( RANKING_TYPE type, bool bShow );
	void				SetShowOthersInfoByType( ColumnForRank* const pColumnForOthersRank, RANKING_TYPE type, bool bShow );

	void				UpdatePageUI() const;
	std::wstring		ConvertSecToPlayTimeStr( UINT uiSec ) const;
	std::wstring		ConvertTimeToRegisterDateStr( __int64 tRegisterDate );
	void				DestoryUIRankingInfo();
	void				SetCharIcon( CKTDGUIStatic* const pStaticClass, char cUnitClass, int iLevel );
	void				SetLevelIcon( CKTDGUIControl::CPictureData* const pPictureData, int num );
	void				SetRankNumImage( CKTDGUIStatic* const pStaticRankNumImg, int iRankNum );
	void				SetNumToPictureForRank( CKTDGUIControl::CPictureData* const pPictureData, int num );


	bool				Handler_EGS_GET_RANKING_INFO_REQ();
	bool				Handler_EGS_GET_RANKING_INFO_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
#ifdef SERV_LOCAL_RANKING_SYSTEM
	#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		 //인자는 로컬랭킹에 대한 로그를 남기기 위해 추가. 게시판에서 버튼을 클릭하여 호출됐을경우만 true
		bool				Handler_EGS_LOCAL_RANKING_INQUIRY_REQ( bool IsBoardButton = false );
	#else
		bool				Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
	#endif //SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
	
	bool				Handler_EGS_LOCAL_RANKING_INQUIRY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void				ChangeBar( BAR_SELECT _barSelect );
	void				InitMyColumnFriend();
	void				InitOthersColumnFriend();
	void				SetShowInfoByType_F( ColumnForRankFriend* const pColumnForOthersRank, bool bShow );
	void				SetRank( CKTDGUIStatic* _static, int _rank );
	void				SetPoint( CKTDGUIStatic* _static, int _point );
	void				SetGender( CKTDGUIStatic* _static, GENDER_SELECT _select );
#endif //SERV_LOCAL_RANKING_SYSTEM

private:
	bool								m_bShow;
	const CKTDXStage* const				m_pNowStage;
	CKTDGUIDialogType					m_pDlgRankingInfo;

	RANKING_TYPE						m_nowRankingInfoType;				// 현재 보여줘야 할 랭킹 정보 타입(헤니르 일일, 대전 등)
	UINT								m_uiNowPage;						// 현재 보여지는 페이지 수
	UINT								m_uiMaxPage;						// 현재 보여줄 수 있는 최대 페이지 수
	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;

	CKTDGUIStatic*						m_pStaticCommonInfo;		//예전에 공통으로 쓰이던 세부항목을 저장해둠(헤니르,시즌대전)
	CKTDGUIStatic*						m_pStaticHenirInfo;
	CKTDGUIStatic*						m_pStaticPvpInfo;
	CKTDGUIStatic*						m_pStaticLevelInfo;

	CKTDGUIRadioButton*					m_pRadioButtonDaily;
	CKTDGUIRadioButton*					m_pRadioButtonWeekly;
	CKTDGUIRadioButton*					m_pRadioButtonMonthly;
#ifdef NEW_HENIR_DUNGEON
	CKTDGUIRadioButton*					m_pRadioButtonHero;
#endif // NEW_HENIR_DUNGEON
	//{{ 허상형 : [2009/8/20] //	신학기 이벤트 관련 컨트롤
#ifdef NEW_TERM_EVENT
	CKTDGUIStatic*						m_pStaticEventInfo;
	CKTDGUIRadioButton*					m_pRadioButtonEventHenir;
	CKTDGUIRadioButton*					m_pRadioButtonEventDungeon;
	CKTDGUIRadioButton*					m_pRadioButtonEventKill;
	CKTDGUIRadioButton*					m_pRadioButtonEventDeath;
#endif	//NEW_TERM_EVENT
	//}} 허상형 : [2009/8/20] //	신학기 이벤트 관련 컨트롤

	ColumnForRank						m_columnForOthersRank[_CONST_UIRANKING_INFO_::g_iNumRankingPerPage];
	ColumnForRank						m_columnForMyRank;

#ifdef SERV_LOCAL_RANKING_SYSTEM
	RANKING_TYPE						m_nowRankingInfoTypeSub;

	CKTDGUIDialogType					m_pDlgRankingPopUp;

	ColumnForRankFriend					m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage]; //수치 수정
	ColumnForRankFriend					m_columnForMyFriendRank;

	CommonMenuCategory					m_sCommonMenu;			//새로 생긴 공통으로 쓰이는 세부항목(던전&필드,주간대전)
	CKTDGUIStatic*						m_pStaticDnFd;			//던전&필드에서만 쓰이는 세부항목
	CKTDGUIStatic*						m_pStaticWeekPvp;		//주간대전에서만 쓰이는 세부항목
	CKTDGUIStatic*						m_pStaticNotice;		//상단 글씨, 느낌표

	CKTDGUIIMEEditBox*					m_IMEEditPageSearch;	//페이지 검색

	byte								Is_Filter_Check;		//현재 필터가 체크된 상태인가
	int									m_iPickedUser;			//선택되서 팝업 메뉴가 띄워진 유저

	CX2ProfileManager::Profile			m_sProfile;

#endif //SERV_LOCAL_RANKING_SYSTEM
};

#endif	RANKING_INFO_UI
//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
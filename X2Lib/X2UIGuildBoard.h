#ifdef	GUILD_BOARD

#pragma once

namespace		_CONST_UIGUILDBOARD_INFO_
{
	const	int		g_iNumAdvertisePerPage		= 6;		// 한 페이지 당 나타낼 수 있는 광고 갯수
	const	int		g_iNumJoinListPerPage		= 6;		// 한 페이지 당 나타낼 수 있는 가입 신청 갯수
	const	int		g_iGuildLevelLimit			= 3;		// 길드 레벨은 3자리 까지만
}

class CX2UIGuildBoard
{
public:


	enum UI_GUILD_BOARD_CUSTOM_MSG
	{
		UGBCM_EXIT							= 37000,
		UGBCM_JOIN_LIST_EXIT,
		UGBCM_ADVERTISE_EXIT,
		UGBCM_SELECT_GUILD_AD_TAP,
		UGBCM_SELECT_GUILD_NUM_MEMBER,
		UGBCM_SELECT_GUILD_LEVEL,
		UGBCM_SELECT_GUILD_NEW,
		UGBCM_SELECT_GUILD_HOT,
		UGBCM_SELECT_ADVERTISE_ED,
		UGBCM_PUSH_PREV_PAGE,
		UGBCM_PUSH_NEXT_PAGE,
		UGBCM_PUSH_JOIN,
		UGBCM_PUSH_JOIN_MESSAGE_OK,
		UGBCM_PUSH_JOIN_MESSAGE_CANCEL,
		UGBCM_VERIFY_JOIN_MESSAGE_OK,
		UGBCM_PUSH_SKILL,
		UGBCM_PUSH_ADVERTISE,
		UGBCM_PUSH_ADVERTISE_MESSAGE_OK,
		UGBCM_PUSH_ADVERTISE_MESSAGE_CANCEL,
		UGBCM_VERIFY_ADVERTISE_MESSAGE_OK,
		UGBCM_MODIFY_ADVERTISE_MESSAGE_OK,
		UGBCM_PUSH_JOIN_LIST,	
		UGBCM_PUSH_PREV_PAGE_JOIN_LIST,
		UGBCM_PUSH_NEXT_PAGE_JOIN_LIST,
		UGBCM_PUSH_ACCEPT,	
		UGBCM_PUSH_REMOVE,
		UGBCM_SELECT_JOIN_LIST,
		UGBCM_SELECT_REG_DATE,
	};
	//{{ oasis907 : 김상윤 [2010.2.16] // 
	enum SORT_TYPE
	{
		ST_REG_DATE = 0,
		ST_NEW_GUILD,
		ST_HOT_GUILD,
		ST_MEMBER_COUNT,
		ST_GUILD_LEVEL,
		ST_MAX_NUM,
	};
	//}

	//{{ oasis907 : 김상윤 [2009.12.28] //
	struct UserSkillData
	{
		int m_iSkillLevel;
		int m_iSkillCSPoint;

		UserSkillData() : 
		m_iSkillLevel( 0 ),
			m_iSkillCSPoint( 0 )
		{
		}

		UserSkillData( int iLevel, int iSkillCSPoint )
		{
			m_iSkillLevel = iLevel;
			m_iSkillCSPoint = iSkillCSPoint;
		}
	};
	//}} oasis907 : 김상윤 [2009.12.28] //


	struct ColumnForAdvertise
	{
		CKTDGUIStatic* m_pStaticGuildName;					// 길드 이름
		CKTDGUIStatic* m_pStaticGuildLevel;				// 길드 레벨
		CKTDGUIStatic* m_pStaticAdvertiseMessage;				// 길드 광고 메시지

		CKTDGUIButton* m_pButtonDescGuild;				// 길드 정보 툴팁
		CKTDGUIButton* m_pButtonDescMessage;				// 길드 광고 메시지 툴팁

		CKTDGUIButton* m_pButtonJoin;	
		CKTDGUIButton* m_pButtonSkill;	

		CKTDGUIStatic* m_pStaticButtonJoin;
		CKTDGUIStatic* m_pStaticButtonSkill;

		int m_iGuildUID;				

		KGuildAdInfo m_KGuildAdInfo;

		ColumnForAdvertise() 
		{
            m_pStaticGuildName = NULL;
			m_pStaticGuildLevel = NULL;
			m_pStaticAdvertiseMessage = NULL;

			m_pButtonDescGuild = NULL;	
			m_pButtonDescMessage = NULL;			

			m_pButtonJoin = NULL;	
			m_pButtonSkill = NULL;	

			m_pStaticButtonJoin = NULL;	
			m_pStaticButtonSkill = NULL;	

			m_iGuildUID = 0;
		}
	};

	


	struct ColumnForJoinList
	{
		CKTDGUIDialogType m_pCharacterIcon;

		UidType m_iUnitUID;

		CKTDGUIRadioButton* m_pRadioJoinList;
		CKTDGUIStatic* m_pStaticJoinList;				
		CKTDGUIButton* m_pButtonDescMessage;				// 가입 신청 메시지 툴팁


		KApplyJoinGuildInfo m_KApplyJoinGuildInfo;


		ColumnForJoinList() 
		{
			m_pCharacterIcon = NULL;
			
			m_pRadioJoinList = NULL;
			m_pStaticJoinList = NULL;
			m_pButtonDescMessage = NULL;

			m_iUnitUID = 0;

		}
	};




public:
	CX2UIGuildBoard( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	~CX2UIGuildBoard();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );


//{{ oasis907 : 김상윤 [2009.12.18] //
	CX2UIGuildSkillTreeInfo* GetUIGuildSkillTreeInfo() { return m_pGuildSkillTreeInfo; }
	// oasis907 : 김상윤 [2009.12.28] //
	void				SetRequiredGuildSkill( const std::vector<KGuildSkillData>& vecSkillList );




private:
	void				InitUIGuildBoard();
	void				InitColumnAdvertise();

	void				SetShowControlsByType( SORT_TYPE type, bool bShow );

	void				SetShowColumn( ColumnForAdvertise* const pColumnForAdvertise, SORT_TYPE type, bool bShow );
	void				SetShowColumn( ColumnForJoinList* const pColumnForJoinList, bool bShow, UINT uiListIndex ); // 가입 신청 목록


	void				UpdatePageUI() const;	  // 길드 게시판의 페이지 번호
	void				UpdatePageJoinUI() const; // 가입 신청 목록의 페이지 번호

	void				UpdateButtonUI(bool bCanShowApplyList); // 길드광고등록, 신청목록보기 버튼. 
	void				UpdateJoinListButtonUI(UINT uiJoinListIndex); // 가입신청목록의 가입 수락, 삭제하기 버튼


	// 활성화, 비활성화와 이에 따른 버튼의 그래픽 처리.
	void				SetEnableButtonAccept(bool bEnable); // 가입 수락 버튼 
	void				SetEnableButtonRemove(bool bEnable); // 삭제하기 버튼
	void				SetEnableButtonAdvertise(bool bEnable); // 광고 등록 버튼
	void				SetEnableButtonJoinList(bool bEnable); // 신청 목록 버튼

	std::wstring		ConvertTimeToRegisterDateStr( __int64 tRegisterDate );
	wstring				CutString( CKTDGUIStatic* pSeedStatic, int iWidth, const WCHAR* wszString, const WCHAR* wszReplaceString = L"");

	void				SetNumImage( CKTDGUIStatic* const pStaticNumImg, int iRankNum );
	void				SetNumToPicture( CKTDGUIControl::CPictureData* const pPictureData, int num );

	// oasis907 : 김상윤 [2009.12.29] // 가입 신청 목록
	void				InitColumnJoinList();

public:

	int					GetGuildSkillLevel( CX2SkillTree::SKILL_ID eSkillID );
	bool				IsMaxGuildSkillLevel( IN CX2SkillTree::SKILL_ID eSkillID );
	bool				GetGuildSkillLevelAndCSP( IN CX2SkillTree::SKILL_ID eSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint );
	void				CalcUsedGuildSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint );
	bool				CanILearnThisSkill( CX2SkillTree::SKILL_ID eSkillID, bool bCheckSP = true );





	bool				Handler_EGS_GET_GUILD_AD_LIST_REQ();
	bool				Handler_EGS_GET_GUILD_AD_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_GET_GUILD_SKILL_IN_BOARD_REQ();
	bool				Handler_EGS_GET_GUILD_SKILL_IN_BOARD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_REGISTRATION_GUILD_AD_REQ( wstring &wstrMessage ) const;
	bool				Handler_EGS_REGISTRATION_GUILD_AD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_MODIFY_REG_GUILD_AD_REQ( wstring &wstrMessage ) const;
	bool				Handler_EGS_MODIFY_REG_GUILD_AD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



	bool				Handler_EGS_APPLY_JOIN_GUILD_REQ( wstring &wstrMessage, bool bOldApplyDelete );
	bool				Handler_EGS_APPLY_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
	bool				Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_ACCEPT_JOIN_GUILD_REQ();
	bool				Handler_EGS_ACCEPT_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_DELETE_APPLY_JOIN_GUILD_REQ();
	bool				Handler_EGS_DELETE_APPLY_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool				SetShowGuildAdvertiseMessageDlg(bool bEnable);
	bool				SetShowGuildJoinMessageDlg(bool bEnable);




private:
	bool								m_bShow;

	bool								m_bOldApplyDelete;
	bool								m_bRegMyGuildAd; 

	std::wstring						m_wstrLastMessage;
	std::wstring						m_wstrLastADMessage;

	const CKTDXStage* const				m_pNowStage;
	CKTDGUIDialogType					m_pDlgGuildBoard;
									


	SORT_TYPE							m_nowSortType;				// 현재 보여줘야 할 랭킹 정보 타입(헤니르 일일, 대전 등)
	UINT								m_uiNowPage;						// 현재 보여지는 페이지 수
	UINT								m_uiMaxPage;						// 현재 보여줄 수 있는 최대 페이지 수

	UINT								m_uiNowPageJoinList;					// 현재 보여지는 페이지 수 (가입 신청 목록)
	UINT								m_uiMaxPageJoinList;					// 현재 보여줄 수 있는 최대 페이지 수(가입 신청 목록)

	UINT								m_uiListIndexForJoin;				 // 길드 가입 신청 버튼 리스트 인덱스
	UINT								m_uiListIndexForSkill;				// 길드 스킬 보기 버튼 리스트 인덱스


	UINT								m_uiJoinListIndex;			 // 길드 가입 신청 버튼 리스트 인덱스


	short								m_sAdvertisePeriod;					

	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;

	

	CKTDGUIRadioButton*					m_pRadioGuildNumMember;
	CKTDGUIRadioButton*					m_pRadioGuildLevel;
	CKTDGUIRadioButton*					m_pRadioGuildNew;
	CKTDGUIRadioButton*					m_pRadioGuildHot;
	//{{ oasis907 : 김상윤 [2010.2.16] // 
	CKTDGUIRadioButton*					m_pRadioRegDate;


	CKTDGUIButton*						m_pButtonAdvertise;	
	CKTDGUIButton*						m_pButtonJoinList;

	CKTDGUIButton*						m_pButtonAccept;	
	CKTDGUIButton*						m_pButtonRemove;	


	ColumnForAdvertise					m_columnForAdvertise[_CONST_UIGUILDBOARD_INFO_::g_iNumAdvertisePerPage];
	ColumnForJoinList					m_columnForJoinList[_CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage]; // 가입 신청 목록

	// oasis907 : 김상윤 [2009.12.18] //
	CX2UIGuildSkillTreeInfo*			m_pGuildSkillTreeInfo;

	// oasis907 : 김상윤 [2009.12.29] //
	CKTDGUIDialogType					m_pDlgJoinList;
	CKTDGUIDialogType					m_pDlgGuildAdvertiseMessage;
	CKTDGUIDialogType					m_pDlgGuildJoinMessage;



	typedef std::map< CX2SkillTree::SKILL_ID, UserSkillData > SkillDataMap;				// <skill id, UserSkillData>
	SkillDataMap						m_mapGuildSkillRequired;


public:
	//{{ oasis907 : 김상윤 [2009.12.28] //
	KProtectedType<int>					m_iGuildSPoint;				// 가지고 있는 스킬 포인트
	KProtectedType<int>					m_iGuildCSPoint;				// 가지고 있는 캐시 스킬 포인트
	KProtectedType<int>					m_iMaxGuildCSPoint; 
	std::wstring						m_wstrGuildCSPointEndDate;
	//}} oasis907 : 김상윤 [2009.12.28] //



//}} oasis907 : 김상윤 [2009.12.28] //


};

#endif	GUILD_BOARD
#pragma once

//{{ 허상형 : [2009/9/15] //	길드매니저 클래스 선언

namespace		_CONST_GUILD_MANAGER_
{
	const BYTE		g_byGuildAdminMagicNumber	= 10;		// 길드원 10명당 길드 관리자 1명
	const USHORT	g_usMaxNumOfGuildMembers	= 100;		// 확장 가능한 최대 길드 인원
	const int		g_iMaxGuildHonorPoint		= 1600;		// 길드 명예포인트 최고치
	// oasis907 : 김상윤 [2009.12.9] //  enum GUILD_USER_GRADE 
	const UCHAR		g_ucLimitGuildGrade         = 6;    // 길드 스킬 영향 받지 못하는 등급 커트라인

}

class CX2UIGuild;

#ifdef GUILD_MANAGEMENT
class CX2GuildManager{

public:
	enum	GUILD_UI_MESSAGE
	{
		GUM_GUILD_NAME_OK			= 35001,
		GUM_GUILD_NAME_CANCEL,
		GUM_GUILD_INVITE_MEMBER_OK,				// 길드 초대 확인
		GUM_GUILD_INVITE_MEMBER_CANCEL,			// 길드 초대 취소
		GUM_GUILD_JOIN_OK,						// 초대에 의한 길드 가입 확인
		GUM_GUILD_JOIN_CANCEL,					// 초대에 의한 길드 가입 취소

		GUM_GUILD_INFO_LEAVE_BUTTON,			// 길드 정보탭에서 길드탈퇴 버튼
		GUM_GUILD_INFO_DESTROY_BUTTON,			// 길드 정보탭에서 길드해체 버튼
		GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON,	// 길드 메시지 변경 버튼

		GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_OK,		//	길드 메시지 변경 Dialog 확인 버튼
		GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_CANCEL,	//	길드 메시지 변경 Dialog 취소 버튼
		GUM_GUILD_INFO_CHANGE_MESSAGE_FINAL_OK,			//	길드 메시지 변경 최종 확인

		GUM_GUILD_INFO_CLOSE,					//	길드 정보 Dialog 닫기

		GUM_GUILD_LEAVE_OK,					// 길드 탈퇴 확인
		GUM_GUILD_LEAVE_CANCEL,				// 길드 탈퇴 취소

		GUM_GUILD_DESTROY_OK,				// 길드 해체 확인
		GUM_GUILD_DESTROY_CANCEL,			// 길드 해체 취소

		GUM_GUILD_INFO_TAB,					//	길드 정보 탭
		GUM_GUILD_MEMBER_INFO_TAB,			//	길드원 정보 탭
		GUM_GUILD_SKILL_TAB,				//	길드 스킬 탭

		GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE,			//	인사말변경
		GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_OK,		//	인사말변경 Dialog Ok
		GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_CANCEL,	//	인사말변경 Dialog Cancel

		GUM_GUILD_MEMBER_INFO_PAGE_LEFT,	//	길드원 정보 페이지 다운
		GUM_GUILD_MEMBER_INFO_PAGE_RIGHT,	//	길드원 정보 페이지 업

		GUM_GUILD_NEW_GUILD_MASTER_OK,		//	새로운 길드 마스터 임명 Dialog 확인 버튼
		GUM_GUILD_NEW_GUILD_MASTER_RE_OK,		//	새로운 길드 마스터 임명 Dialog 재확인 버튼
		GUM_GUILD_NEW_GUILD_MASTER_RE_CANCEL,	//	새로운 길드 마스터 임명 Dialog 재확인 취소 버튼

		GUM_GUILD_MEMBER_INFO_DELETE01,		//	길드원 인사말 삭제
		GUM_GUILD_MEMBER_INFO_DELETE02,
		GUM_GUILD_MEMBER_INFO_DELETE03,
		GUM_GUILD_MEMBER_INFO_DELETE04,
		GUM_GUILD_MEMBER_INFO_DELETE05,
		GUM_GUILD_MEMBER_INFO_DELETE06,
		GUM_GUILD_MEMBER_INFO_DELETE07,
		GUM_GUILD_MEMBER_INFO_DELETE08,

		GUM_GUILD_MEMBER_INFO_DELETE_OK,	//	길드원 메시지 삭제 확인버튼

		GUM_GUILD_CREATE_OK,
		
		GUM_GUILD_NOTICE_OK,				//	길드 완료 팝업 확인 버튼

#ifdef SERV_GUILD_CHANGE_NAME
		GUM_GUILD_NAME_CHANGE_CHECK_OK,			//  길드 이름 변경 체크 확인 버튼
		GUM_GUILD_NAME_CHANGE_CHECK_CANCEL,		//  길드 이름 변경 체크 취소 버튼
		GUM_GUILD_NAME_CHANGE_OK,				//  길드 이름 변경 확인 버튼
		GUM_GUILD_NAME_CHANGE_CNACEL,			//  길드 이름 변경 취소 버튼
#endif //SERV_GUILD_CHANGE_NAME
	};


	//{{ 허상형 : [2009/9/19] //	길드 계급 설정
	enum	GUILD_USER_GRADE
	{
		GUG_INVALID		= 0,
		GUG_MASTER,				//	마스터
		GUG_SYSOP,				//	관리자
		GUG_OLD_USER,			//	원로 길드원
		GUG_VIP_USER,			//	우수 길드원
		GUG_NORMAL_USER,		//	일반 길드원
		GUG_NEW_USER,			//	신입 길드원
	};
	//}} 허상형 : [2009/9/19] //	길드 계급 설정

	//	UI MSG
	enum	GUILD_CHANGE_GRADE_UI_MSG
	{
		GCGUM_MASTER	= 35101,
		GCGUM_SYSOP,
		GCGUM_OLD_USER,
		GCGUM_VIP_USER,
		GCGUM_NORMAL_USER,
		GCGUM_NEW_USER,
	};

	//{{ kimhc // 2009-09-18 // 길드 정보 구조체
	struct GuildInfo
	{
		UidType				m_iGuildUID;
		std::wstring		m_wstrGuildName;
		UINT				m_uiGuildEXP;
		std::wstring		m_wstrGuildMessage;
		std::wstring		m_wstrFoundingDay;			// 길드 창단일
		USHORT				m_usMaxNumOfGuildMember;
		USHORT				m_usNowNumOfGuildMember;
		std::wstring		m_wstrGuildMasterNickName;
		BYTE				m_byMaxNumOfGuildAdmin;		// 길드 관리자 최대 수

		BYTE				m_byGuildLevel;				//{{ 허상형 : [2009/9/28] //	레벨 추가

		GuildInfo() :
		m_iGuildUID( 0 ),
		m_wstrGuildName(),
		m_uiGuildEXP( 0 ),
		m_wstrGuildMessage(),
		m_wstrFoundingDay(),
		m_usMaxNumOfGuildMember( 0 ),
		m_usNowNumOfGuildMember( 0 ),
		m_wstrGuildMasterNickName(),
		m_byMaxNumOfGuildAdmin( 0 ),
		m_byGuildLevel( 0 )
		{}

		GuildInfo( const KGuildInfo& kGuildInfo ) :
		m_iGuildUID( kGuildInfo.m_iGuildUID ),
		m_wstrGuildName( kGuildInfo.m_wstrGuildName.c_str() ),
		m_uiGuildEXP( kGuildInfo.m_iGuildEXP ),
		m_wstrGuildMessage( kGuildInfo.m_wstrGuildMessage.c_str() ),
		m_wstrFoundingDay( kGuildInfo.m_wstrFoundingDay.c_str() ),
		m_usMaxNumOfGuildMember( kGuildInfo.m_usMaxNumMember ),
		m_usNowNumOfGuildMember( 0 ),
		m_wstrGuildMasterNickName(),
		m_byMaxNumOfGuildAdmin( 0 ),//( kGuildInfo.m_usMaxNumMember / _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber ) - 1 ),
		m_byGuildLevel( kGuildInfo.m_ucGuildLevel )
		{
		}

		void UpdateMaxNumOfGuildAdmin()
		{
			BYTE byMaxNum = 0;
			m_byMaxNumOfGuildAdmin = m_usNowNumOfGuildMember / _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber;

			if ( (m_usNowNumOfGuildMember % _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber) != 0 )
				m_byMaxNumOfGuildAdmin++;
		}
	};
	//}} kimhc // 2009-09-18 // 길드 정보 구조체

	//{{ kimhc // 2009-10-31 // 길드 경험치 정보
	struct GuildEXP
	{
		int				m_iNeedEXP;
		int				m_iTotalEXP;

		GuildEXP()
		{
			m_iNeedEXP		= 0;
			m_iTotalEXP		= 0;
		}

		/*GuildEXP( int iNeedExp, int iTotalExp )
		{
			m_iTotalEXP		= iNeedExp;
			m_iTotalEXP		= iTotalExp;
		}*/
	};
	//}} kimhc // 2009-10-31 // 길드 경험치 정보

	//{{ 허상형 : [2009/10/11] //	길드원 정보 구조체(인사말 변경에 사용)
	struct GuildMemberInfo{
		UidType	m_UID;		//	길드원 UID
		UCHAR	ucGrade;		//	길드원 등급
		char	m_cUnitClass;	//	길드원 클래스
		UCHAR	m_ucLevel;		//	길드원 레벨

		GuildMemberInfo() : m_UID(0), ucGrade(GUG_INVALID), m_cUnitClass(0), m_ucLevel(0) {}
	};
	
	//}} 허상형 : [2009/10/11] //	길드원 정보 구조체(인사말 변경에 사용)

public:
	CX2GuildManager();
	~CX2GuildManager();

	HRESULT			OnFrameMove( double fTime, float fElapsedTime );
	bool			UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool			UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool			DidJoinGuild() const { return m_pMyGuildInfo != NULL ? true : false; }		// 길드에 가입 되어져 있는가?
	std::wstring	GetWStringGuildMemberGrade( BYTE byMemberGrade ) const;
	void			SetUseGuildItemUID( UidType iUID ) { m_iUseGuildItemUID = iUID; }
	bool			IsMyGuildUser( UidType iUid ) const { return m_setUidOfGuildMembers.find( iUid ) != m_setUidOfGuildMembers.end() ? true : false;	} //	자기 길드원인가?
	bool			CanDestroyGuild() const;
	bool			CanInviteMember() const;
	void			UpdateGuildTabInCommunityUI() const;
	void			UpdateNowNumOfGuildMember( UidType iUnitUID, bool bIsAdd = true );	// 길드원 추가/삭제

	GuildInfo*	GetGuildInfo() const { return m_pMyGuildInfo; } 
	GuildInfo*	GetMyGuildInfo()	{	return m_pMyGuildInfo;	}
	wstring		GetMyGuildName()	{	return ( m_pMyGuildInfo == NULL ) ? L"" : m_pMyGuildInfo->m_wstrGuildName; }
	wstring		GetDisplayMyGuildName();
	wstring		ConvertDisplayGuildName( const wstring &wstrGuildName );	//	길드명 사이에 []를 넣어준다
	void		SafeDeleteGuildInfo();
	bool		IsMyUID( UidType iMyUID ) const;
	D3DXCOLOR	GetGradeColor( GUILD_USER_GRADE eGrade ) const;
	UidType		GetSelectedUserUID() {	return m_iSelectedUserUID;	}
	void		SetVecGuildMemberInfo( std::vector< GuildMemberInfo > vecGuildMemberInfo ) {	m_vecGuildMemberInfo = vecGuildMemberInfo;	}
	bool		GetShownGuildMessage() { return m_bIsShownGuildMessage;	}
	void		SetShownGuildMessage( bool bShown ) { m_bIsShownGuildMessage = bShown;	}
	std::vector< GuildMemberInfo >	GetVecGuildMemberInfo() {	return m_vecGuildMemberInfo;	}

	void		SetGuildInfo( const KGuildInfo &kInfo )
	{
		if( m_pMyGuildInfo == NULL )
		{
			m_pMyGuildInfo = new GuildInfo( kInfo );
		}
		else
		{
			m_pMyGuildInfo->m_iGuildUID = kInfo.m_iGuildUID;
			m_pMyGuildInfo->m_wstrGuildName = kInfo.m_wstrGuildName.c_str();
			m_pMyGuildInfo->m_uiGuildEXP = kInfo.m_iGuildEXP;
			m_pMyGuildInfo->m_wstrGuildMessage = kInfo.m_wstrGuildMessage.c_str();
			m_pMyGuildInfo->m_wstrFoundingDay = kInfo.m_wstrFoundingDay.c_str();
			m_pMyGuildInfo->m_usMaxNumOfGuildMember = kInfo.m_usMaxNumMember;
			m_pMyGuildInfo->m_byMaxNumOfGuildAdmin = ( m_pMyGuildInfo->m_usMaxNumOfGuildMember / _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber ) - 1;
			m_pMyGuildInfo->m_byGuildLevel = kInfo.m_ucGuildLevel;




		}

		//	상형 : 클래스 멤버 변수 접근은 구조체 생성자에서 할 수 없어 일단 밖으로 빼놓음.
		m_pMyGuildInfo->m_usNowNumOfGuildMember	= m_setUidOfGuildMembers.size();
		m_pMyGuildInfo->UpdateMaxNumOfGuildAdmin();
	}
	
	void		SetGuildInfo( const KEGS_GUILD_INFO_NOT &kPacket )
	{
		SetGuildInfo( kPacket.m_kGuildInfo );

		m_pMyGuildInfo->m_wstrGuildMasterNickName = kPacket.m_wstrGuildMasterName;
		m_pMyGuildInfo->UpdateMaxNumOfGuildAdmin();
	}
	
	//	핸들러
	bool	Handler_EGS_CREATE_GUILD_REQ( const wstring& wszGuildName ) const;
	bool	Handler_EGS_CREATE_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_GUILD_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_REQ( UidType iUID, int iGrade );
	bool	Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_GUILD_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_INVITE_GUILD_REQ( UidType unitUID ) const;	// 길드 초대
	bool	Handler_EGS_INVITE_GUILD_REQ( const wstring& wstrNickName ) const;
	bool	Handler_EGS_INVITE_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) const;

	bool	Handler_EGS_INVITE_GUILD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_INVITE_GUILD_REPLY_REQ( int iGuildUID, int iReasonForDeny );
	bool	Handler_EGS_INVITE_GUILD_REPLY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) const;
	bool	Handler_EGS_INVITE_GUILD_REPLY_DENY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_KICK_GUILD_MEMBER_REQ( UidType iUnitUID ) const;
	bool	Handler_EGS_KICK_GUILD_MEMBER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_CHANGE_GUILD_MESSAGE_REQ( wstring &wstrMessage ) const;
	bool	Handler_EGS_CHANGE_GUILD_MESSAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_DISBAND_GUILD_REQ() const;
	bool	Handler_EGS_DISBAND_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_GET_GUILD_USER_LIST_REQ();
	bool	Handler_EGS_GET_GUILD_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ( wstring &wstrMessage, UidType iUid = 0 ) const;
	bool	Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_JOIN_GUILD_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//{{ oasis907 : 김상윤 [2009.12.17] //
	bool	Handler_RESERVED_EGS_INVITE_GUILD_NOT( KEGS_INVITE_GUILD_NOT kEvent);
//}} oasis907 : 김상윤 [2009.12.17] //

#ifdef SERV_GUILD_CHANGE_NAME
	bool Handler_EGS_CHANGE_GUILD_NAME_CHECK_REQ( wstring wstrGuildName );
	bool Handler_EGS_CHANGE_GUILD_NAME_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_GUILD_NAME_REQ( int iOk ) ;
	bool Handler_EGS_CHANGE_GUILD_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_GUILD_CHANGE_NAME

public:
	CX2UIGuild *GetUIGuild() const { return m_pUIGuild; }
	USHORT		GetNowNumOfGuildMember() const { return m_pMyGuildInfo != NULL ? m_pMyGuildInfo->m_usNowNumOfGuildMember : 0; }
	USHORT		GetMaxNumOfGuildMember() const { return m_pMyGuildInfo != NULL ? m_pMyGuildInfo->m_usMaxNumOfGuildMember : 0; }
	bool		GetShowUIGuildInfo() const;

	const GuildEXP&	GetGuildExpData( BYTE byNowGuildLevel ) const;	
	BYTE			GetLimitGuildLevel() const { return m_byLimitGuildLevel; }
#ifdef GUILD_SKILL
	UCHAR			GetLimitGuildGrade() const { return m_ucLimitGuildGrade; } // oasis907 : 김상윤 [2009.11.18] //
#endif GUILD_SKILL
	bool			OpenScriptFile( const wstring wstrFileName );
	void			SetLimitGuildLevel_LUA( BYTE byLimitGuildLevel );
	void			AddGuildEXP_LUA( BYTE byGuildLevel, int iNeedEXP, int iTotalEXP );
	UINT			GetNowGuildExp( BYTE byNowGuildLevel ) const;
	UINT			GetNextLevelNeedExp( BYTE byNowGuildLevel ) const;
	UINT			GetTotalGuildExp( BYTE byNowGuildLevel ) const;
	int				GetNowMyGuildHonorPoint( BYTE byNowGuildLevel ) const;

	void			EmptyFuncUsedParsing_LUA() {};


	


private:
	GuildInfo*								m_pMyGuildInfo;
	std::set< UidType >						m_setUidOfGuildMembers;		// 길드원들의 UID
	
	std::map< CKTDGUIDialogType,int >		m_mapGuildInvitationDlg;	// vec< pair< Dialog, 길드UID > > // 유저를 초대 중인...
	std::set< int >							m_setGuildUIDForInvitation;	// 유저를 초대 중인 길드의 UID


	UidType									m_iUseGuildItemUID;			//	사용한 아이템의 UID(창단주문서, 길드원 확장 아이템 등에 이용)
	CKTDGUIDialogType						m_pDlgChangeGuildMessage;	//	길드명 변경 최종 확인용 Dialog객체

	CKTDGUIDialogType						m_pDlgNewMaster;			//	길드 마스터 물려줄때 확인 Dlg
	CKTDGUIDialogType						m_pDlgNewMasterRe;			//	길드 마스터 물려줄때 재확인 Dlg

	wstring									m_wstrSelectedUserName;	//	선택된 케릭터이름
	UidType									m_iSelectedUserUID;		//	선택된 케릭터UID

	std::vector< GuildMemberInfo >			m_vecGuildMemberInfo;	//	길드원 정보 구조체

	wstring									m_wstrCreateGuildName;	//	만들 길드 이름
	wstring									m_wstrSenderNickName;	// 나에게 길드 초대를 한 유저의 닉네임

	bool									m_bIsShownGuildMessage;	//	길드 메시지를 보여줬는지 여부

	std::map< BYTE, GuildEXP >				m_mapGuildExpTable;		// 길드 경험치 테이블
	BYTE									m_byLimitGuildLevel;	// 길드 만렙 제한
#ifdef GUILD_SKILL
	UCHAR									m_ucLimitGuildGrade; // oasis907 : 김상윤 [2009.11.18] // 길드 스킬의 영향을 받지 못하는 제한 등급
#endif GUILD_SKILL

	CKTDGUIDialogType						m_DlgKickByMaster;		// 강퇴 당했다는 팝업
public:
	CX2UIGuild	*m_pUIGuild;
	
};

#endif	//	GUILD_MANAGEMENT
//}} 허상형 : [2009/9/15] //	길드매니저 클래스 선언

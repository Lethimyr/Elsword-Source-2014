//{{ 허상형 : [2009/9/8] //	길드 UI 클래스
#pragma once

namespace		_CONST_UI_GUILD_
{
	const BYTE		g_ShownPlayersPerPage	= 8;		// 1 페이지당 표시되는 플레이어 숫자

	const int		g_GuildLevelBarSizeX = 296;			//	길드 레벨 경험치 바 길이(UI크기 수정 시 함께 수정)
	const int		g_GuildHornorBarSizeX = 296;		//	길드 명예 포인트 바 길이(UI크기 수정 시 함께 수정)
}

#ifdef GUILD_MANAGEMENT

class CX2UIGuild
{
public:
	enum	GUILD_INFO_TAB_TYPE
	{
		GIT_CURRENT_TAB,
		GIT_GUILD_INFO,
		GIT_GUILD_MEMBER_INFO,
		GIT_GUILD_SKILL,
	};

	struct	GuildInfoControl	//	길드 탭 컨트롤
	{
		map<wstring, CKTDGUIControl *> mapControl;
		CKTDGUIDialogType	pDialog;

		GuildInfoControl() : pDialog( NULL )	{}
        GuildInfoControl( CKTDGUIDialogType dialog ) : pDialog( dialog )	{}

		~GuildInfoControl()	{	mapControl.clear();	};

		bool Insert( wstring name )
		{
			if( pDialog == NULL )	return false;
			CKTDGUIControl *control = pDialog->GetControl( name.c_str() );
			
			if(	control != NULL )
			{
				mapControl.insert( std::pair<wstring, CKTDGUIControl *>( name, control ) );
			}

			return true;
		}

		CKTDGUIControl *GetControl( wstring name )
		{
			if( pDialog == NULL )	return NULL;
			return pDialog->GetControl( name.c_str() );
		}

		void SetParentControl( CKTDGUIDialogType dialog ) { pDialog = dialog; }
		void Clear() { pDialog = NULL;	mapControl.clear(); }

		void SetEnable( wstring name, bool bEnable )
		{
			if( mapControl.find(name) != mapControl.end() )
			{
				mapControl[name]->SetEnable( bEnable );
			}
		}
		void SetEnable( bool bEnable )
		{
			map<wstring, CKTDGUIControl *>::iterator it;
			for(it = mapControl.begin(); it != mapControl.end(); it++)
			{
				it->second->SetEnable( bEnable );
			}
		}

		void SetShow( wstring name, bool bEnable )
		{
			if( mapControl.find(name) != mapControl.end() )
			{
				mapControl[name]->SetShow( bEnable );
			}
		}

		void SetShow( bool bEnable )
		{
			map<wstring, CKTDGUIControl *>::iterator it;
			for(it = mapControl.begin(); it != mapControl.end(); it++)
			{
				it->second->SetShow( bEnable );
			}
		}
	
		void SetShowEnable( wstring name, bool bShow, bool bEnable )		{	SetShow( name, bShow );	SetEnable( name, bEnable );		}
		void SetShowEnable( bool bShow, bool bEnable )		{		SetShow( bShow );		SetEnable( bEnable );	}
	};

private:
	CKTDGUIDialogType			m_pDlgCreateGuild;
	CKTDGUIDialogType			m_pDlgCreateGuildNotice;
	CKTDGUIDialogType			m_pDlgChangeGradeMenu;
	CKTDGUIDialogType			m_pDlgGuildInfo;
	CKTDGUIDialogType			m_pDlgInviteGuild;
	CKTDGUIDialogType			m_pDlgChangeGuildMessage;
	CKTDGUIDialogType			m_pDlgChangeGuildMemberMessage;

	CKTDGUIDialogType			m_pGuildMemberIcon[8];

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-09 //// 
	CX2UIGuildSkillTree*				m_pGuildSkillTree;
	//}} oasis907 : 김상윤 //// 2009-11-09 //// 
#endif GUILD_SKILL

#ifdef SERV_GUILD_CHANGE_NAME
	CKTDGUIDialogType			m_pDlgChangeGuildName;
#endif //CHANGE_GUILD_NAME
	char						m_cGuildInfoTabType;	//	현재 길드 탭 타입

	GuildInfoControl*			m_pGuildInfoControl;
	GuildInfoControl*			m_pGuildMemberInfoControl;

	char						m_cNowMemberInfoPage;		//	현재 멤버 정보 페이지
	char						m_cMaxMemberInfoPage;		//	최대 멤버 정보 페이지
public:
	CX2UIGuild();
	~CX2UIGuild();

	CKTDGUIDialogType			GetDlgCreateGuild() {	return m_pDlgCreateGuild; }
	CKTDGUIDialogType			GetDlgInviteGuildMember() { return m_pDlgInviteGuild; }
	CKTDGUIDialogType			GetDlgChangeGuildMessage() { return m_pDlgChangeGuildMessage; }
	CKTDGUIDialogType			GetDlgChangeGuildMemberMessage() { return m_pDlgChangeGuildMemberMessage; }
	CKTDGUIDialogType			GetDlgGuildInfo() { return m_pDlgGuildInfo; };

	GuildInfoControl*			GetGuildInfoControl() { return m_pGuildInfoControl; }
#ifdef SERV_GUILD_CHANGE_NAME
	CKTDGUIDialogType			GetDlgChangeGuildName() {	return m_pDlgChangeGuildName; }
	bool		SetShowChangeGuildNameDlg( bool bEnable );
#endif //CHANGE_GUILD_NAME
	bool		SetShowCreateGuildDlg( bool bEnable, UidType iUID = 0 );
	bool		SetShowCreateGuildNoticeDlg( bool bEnable, const WCHAR *pGuildName = L"", const WCHAR *pGuildMasterName = L"" );
	bool		SetShowInviteGuildMemberDlg( bool bEnable );
	bool		SetShowChangeGradeMenu( bool bEnable, int iTargetGrade = CX2GuildManager::GUG_INVALID );	//	iTargetGrade = 등급을 바꾸려는 대상의 Grade
	bool		SetShowGuildInfo( bool bEnable );
	bool		SetShowChangeGuildMessageDlg( bool bEnable );
	bool		SetShowChangeGuildMemberMessageDlg( bool bEnable );

	bool		SetGuildInfoTab( char cTab = GIT_CURRENT_TAB );

	bool		GetShowGuildInfo() const;
	char		GetGuildInfoTabType() const { return m_cGuildInfoTabType; }

	void		ResetGuildInfoControl();
	void		ResetGuildMemberInfoControl();

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-09 //// 
	CX2UIGuildSkillTree* GetUIGuildSkillTree() { return m_pGuildSkillTree; }

	void		CreateUIGuildSkillTree();

	bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//}} oasis907 : 김상윤 //// 2009-11-09 //// 
#endif GUILD_SKILL

	void		UpdateGuildMemberInfo( const KEGS_GET_GUILD_USER_LIST_ACK &kEvent );

	char		GetNowMemberInfoPage() { return m_cNowMemberInfoPage; }
	void		SetNowMemberInfoPage( char cPage ) { m_cNowMemberInfoPage = cPage; }
	char		GetMaxMemberInfoPage() { return m_cMaxMemberInfoPage; }
	void		SetMaxMemberInfoPage( char cPage ) { m_cMaxMemberInfoPage = cPage; }

	bool		SetFieldUserIcon(int i, bool bShow);
	void		ClearUserIcon(int i);

	bool		UpdateGuildLevelBar( int iCurrent, int iMax );	// Level이라기 보다는 경험치 Bar 임
	bool		UpdateGuildHonorBar( int iCurrent, int iMax );
	bool		UpdateGuildLevelStr();

	//	wszString의 문자열의 길이를 구하고(pSeedStatic 컨트롤 기준) wszReplaceString이 있을 경우 뒤에 붙여준다. 예) L"..."
	wstring		CutString( CKTDGUIStatic* pSeedStatic, int iWidth, const WCHAR* wszString, const WCHAR* wszReplaceString = L"");
};

#endif	//	GUILD_MANAGEMENT
//}} 허상형 : [2009/9/8] //	길드 UI 클래스
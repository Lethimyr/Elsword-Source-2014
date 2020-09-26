#pragma once

class CX2ProfileManager
{
public:
	enum PROFILE_MANAGER_UI_MSG
	{
		PMUM_CLOSE			= 57000,
		PMUM_LOCATION1,
		PMUM_SAVE,
		PMUM_CANCEL,
		PMUM_PROFILE_CHANGE,
		PMUM_SAVE_OK,
		PMUM_PASSWORD_OK,
		PMUM_PASSWORD_CANCLE,
	};
	
	enum PROFILE_MANAGER_GENDER
	{
		PMG_NONE			= 0,
		PMG_MALE,
		PMG_FEMALE,
	};

	enum PROFILE_MANAGER_AREA
	{
		PMA_SEOUL		= 1,
		PMA_BUSAN,				
		PMA_DAEGU,				
		PMA_INCHEON,	
		PMA_GWANGJU,			
		PMA_DAEJEON,		
		PMA_ULSAN,
		PMA_SEJONG,
		PMA_GYEONGGIDO,
		PMA_GANGWONDO,
		PMA_CHUNGBUK,
		PMA_CHUNGNAM,
		PMA_JEONBUK,
		PMA_JEONNAM,
		PMA_GYEONGBUK,
		PMA_GYEONGNAM,
		PMA_JEJU,
	};

	struct Profile
	{
		u_char		level;
		int			UnitClass;
		wstring		nickname;
		wstring		introduce;
		__time64_t	birthYear;
		int			area;
		byte		gender;
		byte		filter;

		Profile():introduce( L"" ),nickname( L"" )
		{
			level		= -1;
			UnitClass	= -1;
			birthYear	= -1;
			area		= -1;
			gender		= -1;
			filter		= 0;
		}
	};

	struct Area
	{
		wstring UpperArea;
		wstring UnderArea;

		Area():UpperArea(L""),UnderArea(L"")
		{

		}
	};

	CX2ProfileManager(void);
	~CX2ProfileManager(void);

	bool	Handler_EGS_LOCAL_RANKING_USER_INFO_READ_REQ( const wstring& _password );

	bool	UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void	OpenProfile( bool _changebutton );
	void	CloseProfile();
	void	OpenProfileInput();
	void	CloseProfileInput();
	void	OpenRankup( KEGS_LOCAL_RANKING_RANK_UP_NOT& kEvent );
	void	CloseRankup();
	void	ShowRankUpFade( float _elapsedtime );
	void	CloseAll();
	void	ClosePassword();

	bool	GetShow(){ return m_bShow; }
	bool	GetShowRankup(){ return m_bShowRankup; }
	void	SetShowRankup( bool _show ){ m_bShowRankup = _show; }
	bool	GetIsProfileSave(){ return m_bIsProfileSave; }
	void	SetIsProfileSave( bool _save ){ m_bIsProfileSave = _save; }
	bool	GetIsLoadComplete(){ return m_bIsLoadComplete; }
	void	SetIsLoadComplete( bool _complete ){ m_bIsLoadComplete = _complete; }
	float	GetRankupTime(){ return m_fRankupFadeTime; }

	vector<Area>&	GetArea(){ return m_vecArea; }
	Profile*		GetProfile(){ return &m_sProfile; }
	void			SetProfile( const Profile* _profile );
	void			SetDlgCharInfo( CKTDGUIDialogType _dlg ){ m_pDlgCharInfo = _dlg; }
private:
	void	Create_Password();
	void	CreateProfileUI();
	void	CreateProfileInputUI();
	void	CreateRankUpUI();
	bool	SaveInformation();
	void	SetStrongGage( bool _me, int _value );
	void	SetRankTexture( CKTDGUIStatic* _static, int _rank );
	void	ClearRankUpPaticle();
	void	SetAreaCategoryTexture( CKTDGUIControl::CPictureData* _picture, char _category, bool _updown );
	void	SetLocationInfo();

	bool	Handler_EGS_LOCAL_RANKING_USER_INFO_READ_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_LOCAL_RANKING_USER_INFO_WRITE_REQ();
	bool	Handler_EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_LOCAL_RANKING_RANK_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	CX2State*					m_pNowState;
	CKTDGUIDialogType			m_pDlgProfileInput;
	CKTDGUIDialogType			m_pDlgProfile;
	CKTDGUIDialogType			m_pDlgRankUp;
	CKTDGUIDialogType			m_pDlgCharInfo;
	CKTDGUIDialogType			m_pDlgPassword;

	bool						m_bShow;
	bool						m_bShowRankup;
	bool						m_bIsProfileSave;
	bool						m_bIsLoadComplete;
	bool						m_bIsOpenSelect;   //true면 입력창, false면 출력창이 열린다.

	float						m_fRankupFadeTime;

	Profile						m_sProfile;

	wstring						m_wstrPassword;

	vector< Area >				m_vecArea;

	static const __time64_t			MINYEAR; //1900 매년 바꿔줘야 하나...
	static const __time64_t			MAXYEAR; //2013
	static const float				RANKUP_UI_PLAY_TIME; //5.f 실시간피드백UI 노출 시간

	static const int				BUSAN_OFFSET;
	static const int				DAEGU_OFFSET;
	static const int				INCHEON_OFFSET;
	static const int				GWANGJU_OFFSET;
	static const int				DAEJEON_OFFSET;
	static const int				ULSAN_OFFSET;
	static const int				GYEONGGIDO_OFFSET;
	static const int				GANGWONDO_OFFSET;
	static const int				CHUNGBUK_OFFSET;
	static const int				CHUNGNAM_OFFSET;
	static const int				JEONBUK_OFFSET;
	static const int				JEONNAM_OFFSET;
	static const int				GYEONGBUK_OFFSET;
	static const int				GYEONGNAM_OFFSET;
	static const int				JEJU_OFFSET;

	//static const int				BUSAN_NUM;
	//static const int				DAEGU_NUM;
	//static const int				INCHEON_NUM;
	//static const int				GWANGJU_NUM;
	//static const int				DAEJEON_NUM;
	//static const int				ULSAN_NUM;
	//static const int				GYEONGGIDO_NUM;
	//static const int				GANGWONDO_NUM;
	//static const int				CHUNGBUK_NUM;
	//static const int				CHUNGNAM_NUM;
	//static const int				JEONBUK_NUM;
	//static const int				JEONNAM_NUM;
	//static const int				GYEONGBUK_NUM;
	//static const int				GYEONGNAM_NUM;
	//static const int				JEJU_NUM;

	CKTDGParticleSystem::CParticleEventSequence* m_Seq1;
	CKTDGParticleSystem::CParticleEventSequence* m_Seq2;
	CKTDGParticleSystem::CParticleEventSequence* m_Seq3;

};
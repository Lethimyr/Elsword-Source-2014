/**@file	X2PremiumBuff.h
   @breif	휴면 복귀 유저 버프 및 기타 특수 버프 관리 
*/

#pragma once

#ifdef COME_BACK_REWARD

#ifdef BUFF_TEMPLET_SYSTEM

class CX2PremiumBuffTemplet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2PremiumBuffTemplet> CX2PremiumBuffTempletPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef shared_ptr<CX2PremiumBuffTemplet> CX2PremiumBuffTempletPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2PremiumBuffTemplet
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:

	enum BUFF_ID
	{
		BI_NONE							= 0,
		BI_GATHERING_OF_HERO,			// 영웅의 집결 ( 자동파티버프 )
		BI_REST_FOR_REGROUP,			// 재집결을 위한 휴식
		BI_WARM_EL_AURA,				// 따뜻한 엘의 기운 (마을 버프)
		BI_PROTECTION_OF_NUT,			// 누트의 가호     ( 비약, 임시 BuffID )
		BI_PROTECTION_OF_PUTA,			// 프타의 가호     ( 비약, 임시 BuffID )
		BI_PROTECTION_OF_LAHELL,		// 라헬의 가호     ( 비약, 임시 BuffID )
		BI_PROTECTION_OF_JAKIELL,		// 자키엘의 가호   ( 비약, 임시 BuffID )
		BI_BELSSING_OF_SERAPHIM,		// 세라핌의 축복   ( 영약, 임시 BuffID )
		BI_BELSSING_OF_GEV,				// 게브의 축복	  ( 영약, 임시 BuffID )
		BI_BELSSING_OF_AMON,			// 아몬의 축복	  ( 영약, 임시 BuffID )
		BI_BELSSING_OF_CRONOS,			// 크로노스의 축복 ( 영약, 임시 BuffID )
		BI_SAVE_CRYSTAL_BUFF_LEVEL_1,	// 어둠의 문 개인 버프 레벨 4
		BI_SAVE_CRYSTAL_BUFF_LEVEL_2,	// 어둠의 문 개인 버프 레벨 4
		BI_SAVE_CRYSTAL_BUFF_LEVEL_3,	// 어둠의 문 개인 버프 레벨 4
		BI_SAVE_CRYSTAL_BUFF_LEVEL_4,	// 어둠의 문 개인 버프 레벨 4
		BI_VITALITY_OF_EL,				// 어둠의 문 엘의 수정 버프
		BI_STRENGTHENING_BODY,			// 강화된 신체 ( 비약 사용 제한 디버프 )
	};

	//버프 적용 가능한 룸 타입
	enum USING_ROOM_TYPE
	{
		URT_NONE						= -1,
		URT_PVP,						
		URT_DUNGEON,
		URT_SQUARE,
		URT_TRADE,
		URT_TRAININGCENTER,
		URT_PERSONAL_SHOP,
		URT_ARCADE,
		URT_PSHOP_AGENCY,
	};

	~CX2PremiumBuffTemplet()
	{   
		m_vUsingRoomType.clear();
		m_vecSocketOptions.clear();
	}

	static CX2PremiumBuffTempletPtr CreatePremiumBuffTemplet() 
	{
		return CX2PremiumBuffTempletPtr( new CX2PremiumBuffTemplet );
	}

	CX2PremiumBuffTemplet::BUFF_ID GetPremiumBuffID() const { return m_ePremiumBuffID; }
	void SetPremiumBuffID( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ ) { m_ePremiumBuffID = ePremiumBuffID_; }

	UINT GetStringID() const { return m_uiStringID; }
	void SetStringID( const UINT uiStringID_ ) { m_uiStringID = uiStringID_; }

	float GetBuffPeriodSecond() const { return m_fPremiumBuffPeriodSecond; };
	void  SetBuffPeriodSecond( const float m_fPremiumBuffPeriodSecond_ ) { m_fPremiumBuffPeriodSecond = m_fPremiumBuffPeriodSecond_; };

	void PushUsingRoomTypeList( const CX2PremiumBuffTemplet::USING_ROOM_TYPE m_vecSocketOptions_ ) { m_vecSocketOptions.push_back( m_vecSocketOptions_ ); }
	const vector<CX2PremiumBuffTemplet::USING_ROOM_TYPE>& GetUsingRoomTypeList() { return m_vUsingRoomType; }

	void PushBackSocketOption( const int iSocketOption_ ) { m_vecSocketOptions.push_back( iSocketOption_ ); }
	const vector<int>& GetSocketOptionsList() { return m_vecSocketOptions; }

	bool ParsingScript( IN KLuaManager& luaManager_ );

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:
	CX2PremiumBuffTemplet()
		: m_ePremiumBuffID( BI_NONE ), 
		  m_uiStringID( 0 ),
		  m_fPremiumBuffPeriodSecond( 0 )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
          , m_uRefCount( 0 )
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	CX2PremiumBuffTemplet::BUFF_ID					m_ePremiumBuffID;
	UINT											m_uiStringID;
	float											m_fPremiumBuffPeriodSecond;
	vector<CX2PremiumBuffTemplet::USING_ROOM_TYPE>	m_vUsingRoomType;
	vector<int>										m_vecSocketOptions;	
};

IMPLEMENT_INTRUSIVE_PTR( CX2PremiumBuffTemplet );


class CX2PremiumBuffIcon;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2PremiumBuffIcon> CX2PremiumBuffIconPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2PremiumBuffIcon> CX2PremiumBuffIconPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2PremiumBuffIcon
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:

	static CX2PremiumBuffIconPtr CreatePremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, UINT uiIndex_,
		const WCHAR* pFileName_, const WCHAR* pKeyName_, wstring wstrEndTime, bool m_bUsingIcon );

	UINT	GetIndex() const { return m_uiIndex; }
	void	SetIndex( const UINT uiIndex_ ) { m_uiIndex = uiIndex_; }
	void	ChangeBuffIconPosition( const UINT uiNewIndex_ );
	wstring GetBuffEndTime() const { return m_wstrEndTime; }
	bool	GetUsingIcon() const { return m_bUsingIcon; }

	CX2PremiumBuffTemplet::BUFF_ID GetPremiumBuffID() const { return m_ePremiumBuffID; }
	CKTDGUIDialogType& GetBuffIconDialogType() { return m_pDlgBuffIcon; }

	~CX2PremiumBuffIcon();

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

protected:
	CX2PremiumBuffIcon( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, UINT uiIndex_, wstring wstrEndTime, bool bUsingIcon ) 
		: m_ePremiumBuffID( ePremiumBuffID_ ), m_pDlgBuffIcon( NULL ), m_uiIndex( uiIndex_ ), m_wstrEndTime( wstrEndTime ), m_bUsingIcon( bUsingIcon )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
	void CreateBuffIcon( const WCHAR* pFileName_, const WCHAR* pKeyName_ );
	void UpdateBuffIconPosition( const UINT uiIndexGap_ );
	void SetBuffDesc();

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	CX2PremiumBuffTemplet::BUFF_ID	m_ePremiumBuffID;
	CKTDGUIDialogType				m_pDlgBuffIcon;
	UINT							m_uiIndex;
	wstring							m_wstrEndTime;	

	bool							m_bUsingIcon;

	const static float				POSITION_X;
	const static float				POSITION_Y;
	const static UINT				NUMBER_OF_ROW	= 5;
	const static UINT				SIZE_OF_ICON	= 31;
};

IMPLEMENT_INTRUSIVE_PTR( CX2PremiumBuffIcon );

#endif BUFF_TEMPLET_SYSTEM


/** @class : CX2PremuimBuff
	@brief : 버프를 받을 유저에게 버프 적용 및 화면에 UI 표시
	@date  : 2011/05/31
*/
class CX2PremiumBuffManager
{
public:
#pragma region enumulation
	enum STATE_COMBE_BACK_UI_CUSTOM_MSG
	{
		SCBUCM_WELCOME_OK = 50000,
		SCBUCM_COME_BACK_BUFF_DESC_OK,
	};
#pragma endregion enumulation

#pragma region public function
	CX2PremiumBuffManager();
	~CX2PremiumBuffManager();

	//{{ 휴면 유저 확인
	bool IsComeBackUser() const;
	void SetComeBackUser( IN bool bComeBackUser_ );
	INT64 GetComeBackBuffEnd() const;
	//}}

	//{{ 휴면 유저 처음 복귀
	bool IsDescComplete() const;
	bool IsFirstComeBack() const;
	bool IsFirstComeBackBuffDesc() const;
	void SetFirstComeBack( IN bool bFirstComeBack_ );
	void SetShowFirstComBackUI( IN bool bShow_ );
	//}}

	//{{ PC방 접속 시
	bool IsPremiumPC() const;
	void SetPremiumPC( IN bool bFirstComeBack_ );
	//}}

#ifdef SERV_PC_BANG_TYPE
	bool IsPcBangAvatar();
	void SetPcBangReward( KPcBangReward kPcBangReward );
	wstring GetPcBangBuffString();
#endif SERV_PC_BANG_TYPE

	//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	bool IsTooniLandChannelingEvent() const { return m_bTooniLandChannelingEvent; }
	void SetTooniLandChannelingEvent( bool bTooniLandChannelingEvent_ ) { m_bTooniLandChannelingEvent = bTooniLandChannelingEvent_; }
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

	//{{ 휴면 유저 버프 UI 관련
	bool GetShowPremiumBuffUI() const;
	bool SetShowPremiumBuffUI( IN bool bShow_ );
	void UpdateGuideDescPremiumBuffUI();
	//}}

	//{{ 휴면 유저 정보 출력 관련
	bool GetViewBuffChat() const;
	void SetViewBuffChat( IN bool bShow_ );
	//}}

	//{{ 파티 내에 휴면 복귀 유저
	bool IsComeBackUserInParty() const;
	void SetComeBackUserInParty( IN bool bShow_ );
	//}}

	//{{ 이벤트 처리
	bool UICustomEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	//{{ 서버와 통신
#ifdef BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	bool Handler_EGS_UPDATE_BUFF_INFO_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
#endif BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_COME_BACK_BUFF_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	bool Handler_EGS_COME_BACK_USER_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	bool UIServerEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	//{{ 프레임 당 처리
	HRESULT OnFrameMove( IN double fTime, IN float fElapsedTime );
	//}

#ifdef GRACE_OF_EL
	bool IsGraceOfEl() const;
	void SetGraceOfEl( IN bool bGraceOfEl_ );
#endif
#ifdef BUFF_TEMPLET_SYSTEM
	void InsertPremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, wstring wstrEndTime );
	void ErasePremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ );
	bool GetFileAndKeyName( IN const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, OUT const WCHAR** ppFileName_, OUT const WCHAR** ppKeyName_, OUT bool* bUsingIcon_ );
	void UpdatePremiumBuffIconPosition();

	void OpenScriptFile( const wchar_t* wszScriptFileName_ );
	void AddBuffInfo_LUA();

	CX2PremiumBuffTempletPtr GetPremiumBuffTempletPtr( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ );	

	void GetSocketOptions( OUT vector<int>& vecBuffSocetOptions_ );
	void GetSocketOptions( IN const vector<int>& vecBuffIdList_, OUT vector<int>& vecBuffSocetOptions_ );
	
	vector<CX2PremiumBuffIconPtr> GetPremiumBuffList() { return m_vecPremiumBuffList; }

	bool GetShowBuffTempletUI() { return m_bBuffTempletUI; }
	void SetShowBuffTempletUI( bool bShow ) { m_bBuffTempletUI = bShow; }

#endif BUFF_TEMPLET_SYSTEM
#pragma endregion public function

private:
#pragma region private function
	void Clear();
#ifdef SERV_PC_BANG_TYPE
	wstring _GetLastString( wstring wstr )
	{
		unsigned int uiFound = wstr.find_last_of( L'\n' );
		unsigned int uiLastLineSize = wstr.size() - uiFound;

		if( uiLastLineSize > 15 )
			return L"\n";
		else
			return L", ";
	}
#endif SERV_PC_BANG_TYPE

#pragma endregion private function

private:
#pragma region private member
	bool m_bComeBackUser;			/// 휴면 복귀 유저 여부(버프 아이콘 및 필드 타이틀)
	bool m_bChatViewFirst;			/// 채팅 창에 표시되는 버프 내용
	bool m_bDescComplete;			/// 첫 접속 시 혜택 설명 UI 확인(창 닫기)
	bool m_bFirstComeBack;			/// 첫 접속 여부(첫 알림 UI 관련)
	bool m_bFirstComeBackBuffDesc;	/// 첫 접속 시 혜택 설명 UI
	bool m_bPremiumPC;				/// PC방 유저 여부
	bool m_bPremiumBuffUI;			/// 휴면, PC방 등 버프 아이콘 표시 여부
	bool m_bComeBackuserInParty;	/// 파티에 휴면 복귀 유저 포함되었는지 여부
		
	//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	bool m_bTooniLandChannelingEvent;	/// 투니랜드 채널링 이벤트 동안 신규 계정을 생성한 유저
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

#ifdef GRACE_OF_EL
	bool m_bGraceOfEl;			/// 버블파이터 특공대에게 적용되는 "엘의 가호" 버프 여부
#endif

	CKTDGUIDialogType	m_pDLGWelcomeComeBackUser;		/// 휴면 복귀 유저 환영 메시지 다이얼로그
	CKTDGUIDialogType	m_pDLGComeBackUserBuffDesc;		/// 휴면 복귀 유저 버프 설명 다이얼로그

	INT64	m_iComeBackBuffEnd;		/// 휴면 버프 남은 시간

#ifdef BUFF_TEMPLET_SYSTEM
	vector<CX2PremiumBuffIconPtr>		m_vecPremiumBuffList;

	typedef	map<CX2PremiumBuffTemplet::BUFF_ID, CX2PremiumBuffTempletPtr> PremiumBuffTempletMap;
	PremiumBuffTempletMap				m_mapPremiumBuffTempletMap;
	bool								m_bBuffTempletUI;/// 버프 템플릿 표시 여부
#endif BUFF_TEMPLET_SYSTEM

#ifdef SERV_PC_BANG_TYPE
	KPcBangReward						m_kPcBangReward;
#endif SERV_PC_BANG_TYPE

#pragma endregion private member
};

#endif

#pragma once
#ifdef ADDED_RELATIONSHIP_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2OfficiantManger
// 주례 관련된 내용 관리.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2OfficiantManger
{
public:

	/// 주례 행동 양식
	struct OfficiantBehavior
	{
		float									m_fTalkDelayTime;	// 다음 대사 시작 전 지연 시간
		int										m_iTalkStrID;
		wstring									m_wstrMotionName;
		CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE		m_eAnimPlayType;

		OfficiantBehavior():
		m_fTalkDelayTime(0.f),
		m_iTalkStrID(-1),
		m_wstrMotionName(L""),
		m_eAnimPlayType(CKTDGXSkinAnim::XAP_LOOP)
		{}
	};

	/// 주례 템플릿
	struct OfficiantTemplet
	{
		wstring							m_wstrMeshTempletName;	// 메쉬템플릿 이름
		vector<OfficiantBehavior*>		m_vecOfficiantBehavior;	// 주례 행동 양식
		D3DXVECTOR3						m_vTalkBoxOffsetPos;	// 말풍선 위치 offset
		SEnum::WEDDING_OFFICIANT_TYPE	m_eOfficantType;		// 주례사 타입
		float							m_fCameraDurationTime;	// 카메라 지속 시간

		OfficiantTemplet():
		m_vTalkBoxOffsetPos(0,0,0),
		m_eOfficantType( SEnum::WOT_NPC_END ),
		m_fCameraDurationTime(0.f)
		{}

		OfficiantTemplet& operator=( const OfficiantTemplet& rhs )
		{
			m_wstrMeshTempletName	= rhs.m_wstrMeshTempletName;
			m_vTalkBoxOffsetPos		= rhs.m_vTalkBoxOffsetPos;
			m_eOfficantType			= rhs.m_eOfficantType;
			m_fCameraDurationTime	= rhs.m_fCameraDurationTime;

			BOOST_FOREACH( OfficiantBehavior* pOfficiantBehavior, rhs.m_vecOfficiantBehavior )
			{
				OfficiantBehavior* pTempOfficiantBehavior = new OfficiantBehavior;
				*pTempOfficiantBehavior = *pOfficiantBehavior;

				m_vecOfficiantBehavior.push_back( pTempOfficiantBehavior );
			}
			
			return *this;
		}
	};

public:
	CX2OfficiantManger();
	virtual ~CX2OfficiantManger(void);
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool AddOfficiantTemplet_LUA();

	bool CreateOfficiant();
	void StartOfficiantsMessage();

	void SetOfficiant( SEnum::WEDDING_OFFICIANT_TYPE eOfficiantType_ ) { m_eCurrentOfficiantType = eOfficiantType_;}
	void SetOfficiantPos( const D3DXVECTOR3& vPos_, const D3DXVECTOR3& vCameraEye_, const D3DXVECTOR3& vCameraLookAt_);
	
private:

	void ChangeOfficiantMotion( const WCHAR* pMotionName_, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE eAnimPlayType_ );
	void PlayOfficiantsMessage( int iStrID_ );
	void PlayOfficiantCamera( const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt, float fTrackingTime );

private:
	std::map<SEnum::WEDDING_OFFICIANT_TYPE, OfficiantTemplet*> m_mapOfficiantTemplet;		// 전체 주례 정보.

	SEnum::WEDDING_OFFICIANT_TYPE			m_eCurrentOfficiantType;	// 현재 주례 타입.	
	OfficiantTemplet*						m_pCurrentOfficiant;		// 현재 주례 객체
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hOfficiantMesh;			// 주례 메쉬 핸들

	bool									m_bIsPlayOfficiantsMessage; // 주례사 시작 여부
	float									m_fOfficiantTime;			// 주례사 시간

	
	D3DXVECTOR3								m_vPos;			// 주례 위치
	D3DXVECTOR3								m_vCameraEye;	// 주례 진행하는 동안 이동 할 카메라 위치	
	D3DXVECTOR3								m_vCameraLootAt;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingHallManager
// 예식장에 관련된 내용 관리
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2WeddingHallManager
{	
public:
	struct WeddingHallTemplet
	{
		// 결혼식 참석자 시작 위치
		int							m_iBrideStartPos;
		int							m_iGroomStartPos;
		vector<int>					m_vecWeddingGuestStartPos;

		// 신랑,신부의 주례사 시작 이후 이동 될 위치
		int							m_iBrideOfficiantStartPos;
		int							m_iGroomOfficiantStartPos;

		// 주례 위치
		D3DXVECTOR3					m_vOffciantPos;
		D3DXVECTOR3					m_vOffciantCameraEye;		// 주례 진행 카메라
		D3DXVECTOR3					m_vOffciantCmaeraLookAt;	

		D3DXCOLOR					m_vColor;					//공지 색상
		D3DXCOLOR					m_vOutlineColor; 

		CX2World::WORLD_ID			m_eWorldID;
		SEnum::WEDDING_HALL_TYPE	m_eWeddingHallType;

		WeddingHallTemplet():
		m_iBrideStartPos(0),
		m_iGroomStartPos(0),
		m_iBrideOfficiantStartPos(0),
		m_iGroomOfficiantStartPos(0),
		m_vOffciantPos(0,0,0),
		m_vOffciantCameraEye(0,0,0),
		m_vOffciantCmaeraLookAt(0,0,0),
		m_eWorldID(CX2World::WI_NONE),
		m_eWeddingHallType(SEnum::WHT_CASH_A),
		m_vColor(1,1,1,1),
		m_vOutlineColor(0,0,0,1)
		{}
	};

public:
	CX2WeddingHallManager();
	virtual ~CX2WeddingHallManager();
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

	bool AddWeddingHallTemplet_LUA();
	void SetWeddingHall( SEnum::WEDDING_HALL_TYPE eHallType_ );

	// 현재 웨딩홀의 정보 얻는 함수
	CX2World::WORLD_ID GetCurrentWorldID() const;
	void GetOfficiantPos( OUT D3DXVECTOR3& vPos_ ) const;
	void GetOfficiantsMessageEye( OUT D3DXVECTOR3& vEye_ ) const;
	void GetOfficiantsMessageLookAt( OUT D3DXVECTOR3& vLookAt_ ) const;
	UINT GetStartPosIndexByWeddingStatus( SEnum::WEDDING_STATUS eWeddingStatus_ ) const;
	UINT GetOfficiantStartPosIndexByWeddingStatus( SEnum::WEDDING_STATUS eWeddingStatus_ ) const;

	void GetNoticeColor( OUT D3DXCOLOR& vColor_){ if( NULL != m_pCurrentWeddingHallTemplet) vColor_ = m_pCurrentWeddingHallTemplet->m_vColor; }
	void GetNoticeOutlineColor( OUT D3DXCOLOR& vOutlineColor_){ if( NULL != m_pCurrentWeddingHallTemplet) vOutlineColor_ = m_pCurrentWeddingHallTemplet->m_vOutlineColor; }

	void GetQuestStartPosList( OUT vector<int>& vecStartPos_ );
private:
	std::map<SEnum::WEDDING_HALL_TYPE, WeddingHallTemplet*> m_mapWeddingHallTemplet;
	SEnum::WEDDING_HALL_TYPE								m_eCurrentWeddingHallType;

	WeddingHallTemplet*										m_pCurrentWeddingHallTemplet;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingBehavior
// 결혼식 진행에 필요한 행동 정보 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2WeddingBehavior
{
public:
	// 이펙트 출력 위치 타입
	enum WEDDING_EFFECTSET_POS_TYPE
	{
		WEPT_CUSTOM,	// 사용자 지정 위치
		WEPT_GROOM,		// 신랑 위치
		WEPT_BRIDE,		// 신부 위치
	};
	// 유저 위치 이동 타입
	enum WEDDING_POS_MOVE_TYPE
	{
		WPMT_WEDDING_START,
		WPMT_OFFICIANT_START,
	};

	// 공지
	struct Behavior_Notice
	{
		float m_fStartTime;
		float m_fDurationTime;
		int   m_iStrID;
		bool  m_bEndNotice;
	};

	// 위치 이동
	struct Behavior_Move
	{
		float m_fStartTime;
		WEDDING_POS_MOVE_TYPE eWeddingPosMove;
	};

	// 사운드 출력
	struct Behavior_Sound
	{
		float m_fStartTime;
		wstring m_wstrSoundFileName;
		float m_fDurationTime;
	};

	// 지속시간동안 출력 후 제거되어야 하는 사운드 관리
	struct Behavior_DurationTimeSound
	{
		// 포인터의 할당해제 관리는 KTDXDeviceManager클래스에서 처리하고 있음.
		CKTDXDeviceSound* m_pSound;
		float			  m_fTime;
		float			  m_fDurationTime;
		
		Behavior_DurationTimeSound():
		m_pSound(NULL),
		m_fTime(0.f),
		m_fDurationTime(0.f)
		{}
	};

	struct Behavior_Effectset
	{
		float m_fStartTime;
		wstring m_wstrEffectSetName;
		D3DXVECTOR3 m_vPos;
		WEDDING_EFFECTSET_POS_TYPE m_ePosType;
	};

public:
	CX2WeddingBehavior();
	~CX2WeddingBehavior();
	void OpenScript( KLuaManager& luaManager_ );

	vector<Behavior_Notice>		m_vecBehavior_Notice;
	vector<Behavior_Move>		m_vecBehavior_Move;
	vector<Behavior_Sound>		m_vecBehavior_Sound;
	vector<Behavior_Effectset>	m_vecBehavior_Effectset;
	
	vector<Behavior_DurationTimeSound> m_vecBehavior_DurationTimeSound; // 지속시간동안 출력 후 제거되어야 하는 사운드 관리
private:
	void CheckTimeError( float fBehaviorTime_, float fPreBehabiorTime_ );
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingManager
// 주례, 예식장을 관리하는 클래스와 함께 결혼식의 전반적인 동작을 관리.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2WeddingManager
{	
public:
	struct NoticeInfo
	{
		wstring wstrNotice;
		float	m_fElapseTime; // 보여준 시간
		float   m_fMaxShowTime;   // 보여줘야 할 시간

		NoticeInfo():
		wstrNotice(L""),
		m_fElapseTime(0.f),
		m_fMaxShowTime(0.f)
		{}
	};
public:
	CX2WeddingManager();
	virtual ~CX2WeddingManager(void);
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	HRESULT	OnFrameRender();

	/// 내부 매니져클래스에 접근하기 위한 랩핑함수
	bool OpenScriptFile( const WCHAR* pOfficiantFileName_,  
						const WCHAR* pWeddingHallFileName_,
						const WCHAR* pWeddingManagerFileName_);

	bool AddOfficiantTemplet_LUA();
	bool AddWeddingHallTemplet_LUA();

	void SetOfficiant( SEnum::WEDDING_OFFICIANT_TYPE eOfficiantType_ );
	void SetWeddingHallAndOfficiantInfo( SEnum::WEDDING_HALL_TYPE eHallType_ );

	// 게임 객체 생성 할 때 필요한 유닛 위치 설정 함수
	CX2World::WORLD_ID	GetCurrentWorldID() const { return m_eCurrentWorldID; }
	UINT				GetStartPosIndexByWeddingStatus( IN SEnum::WEDDING_STATUS eWeddingStatus_ ) const;
	
	/// 웨딩 시작
	void				StartWedding();
private:

	void OnFrameMove_Notice();
	void OnFrameMove_Move();
	void OnFrameMove_Sound();
	void OnFrameMove_Effectset();
	void OnFrameMove_SoundStop( float fElapsedTime_ );

private:	
	bool					m_bIsPlayWedding;
	float					m_fWeddingTime;

	CX2OfficiantManger*		m_pOfficiantManager;
	CX2WeddingHallManager*	m_pWeddingHallManager;
	CX2WeddingBehavior		m_WeddingBehavior;

	CX2World::WORLD_ID		m_eCurrentWorldID;


	NoticeInfo				m_NoticeInfo;
	

};//class CX2WeddingManager


#endif //ADDED_RELATIONSHIP_SYSTEM
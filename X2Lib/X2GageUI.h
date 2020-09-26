#pragma once

#define D3DFVF_DRAWFACE_RHW_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define ONE_CHARGE 100.0f
#define TWO_CHARGE 200.0f
#define THREE_CHARGE 300.0f
#define FULL_HYPER_COUNT 3	/// 3각성을 할 수 있는 각성 구슬 갯수


class CX2GameUnit;
class CX2RidingPet;
class CX2GageUI;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2GageUI> CX2GageUIPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2GageUI> CX2GageUIPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2GageUI 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{

public:

	// 이중 하나라도 set(1) 인 상태면 UI를 보이지 않도록 함
// 	enum GAGE_UI_FACTOR_SHOW	/// UI를 보이거나 안보이게 하는 요소
// 	{
// 		GUFS_ENABLE_SCREEN_SHOT_MODE = 0,	/// 스샷 모드에 의해 감춰짐
// 		GUFS_STATE_CHANGE,					/// 마을->필드 등 상태변경에 의해 감춰지는 경우
// 		GUFS_FAR_DISTANCE,					/// 거리가 멀어서 안보이는 경우
// 		GUFS_EVE_FLASH_BANG,				/// 이브 일렉트라 스킬 (포톤 플레어)
// 		GUFS_END,							/// 전체 갯수 및 끝을 나타냄
// 	};

	enum PVP_GAME_UI_BAR
	{
		PGUB_MY_MP = 0,
		PGUB_MY_ACTIVE_MP,	/// Detonation(기폭)
		PGUB_MY_SOUL,		/// HyperGage(각성)
	};

	enum PVP_GAME_UI_WAKE_ORB
	{
		PGUWO_ORB1 = 0,
		PGUWO_ORB2,
		PGUWO_ORB3,
		PGUWO_ORB_CHUNG,
		PGUWO_ORB_RAVEN,
		PGUWO_ORB_ARA,
#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어 해외팀 수정
		PGUWO_ORB_EVE,
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
	};

	enum WAY_OF_SWORD_DESTRUCTION_PICTURE
	{
		WOSDP_BG = 0,			// 배경
		WOSDP_POINT_BAR,		// 검의길 수치
		WOSDP_DETONATION_CHARGE,// 기폭 수치
		WOSDP_GAGE_EFFECT,		// 게이지 이펙트
		WOSDP_EMPTY_BEAD,		// 빈 구슬
		WOSDP_FULL_BEAD,		// 꽉 찬 구슬
	};



	
#ifdef BUFF_ICON_UI
	struct BuffIcon
	{
		BUFF_TEMPLET_ID					eBuffID;
		wstring							szTextureFileName;
		wstring							szTextureKeyName;
		int								iBuffName;
		int								iBuffDesc;
		bool							bNew;

		BuffIcon( BUFF_TEMPLET_ID eBuffID_, const wstring& FileName_, const wstring& KeyName_, const int BuffName_, const int BuffDesc_ ):
		eBuffID(eBuffID_), szTextureFileName(FileName_), szTextureKeyName(KeyName_), iBuffName(BuffName_), iBuffDesc(BuffDesc_), bNew(true)
		{}
	};
#endif //BUFF_ICON_UI

public:
	CX2GageUI() : m_fAlpha( 0.0f ), m_bShow( true )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
	
	virtual ~CX2GageUI();

	virtual void			InitUI(){};	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ ) = NULL;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove() = NULL;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual	void			OnFrameRender() {};

	virtual void			CrashMyStateGageUI( const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, D3DXCOLOR changeColor ) { ASSERT( !L"Crash Base Function is not available" ); }
	virtual void			SetHpTexture( const WCHAR* pTexName_ ) { ASSERT( !L"Hp Base function is not available"); }
	virtual void			SetMpTexture( const WCHAR* pTexName_ ) { ASSERT( !L"Mp Base function is not available"); }

	virtual bool			GetShow() const { return m_bShow; }
	virtual void			SetShow( const bool bShow_ ) { m_bShow = bShow_; }

	virtual float			GetAlpha() const { return m_fAlpha; }
	virtual void			SetAlpha( const float fAlpha_ ) { m_fAlpha = fAlpha_; }

	//void					SetOwnerGameUnit( CX2GameUnit* pOwnerGameUnit_ ) { m_optrGameUnit = pOwnerGameUnit_; }

#ifdef BUFF_ICON_UI
	virtual	void			PushBuff( const BuffIcon& BuffIconInfo_, bool bIsDeBuff_){};
	virtual	void			EraseBuff(BUFF_TEMPLET_ID eBuffID_, bool bIsDeBuff_){};
	virtual	void			ClearBuffList(){};
	virtual	void			UpdateBuffIcon(){};
	virtual void			NotifyDurationTime5sec(BUFF_TEMPLET_ID eBuffID_){};
	virtual void			InitBuffIconFlicker(BUFF_TEMPLET_ID eBuffID_) {};
#endif //BUFF_ICON_UI	

#ifdef DISPLAY_BUFF_DURATION_TIME
	virtual void SetDurationTime( BUFF_TEMPLET_ID eBuffID_, int iTime_){}
#endif // DISPLAY_BUFF_DURATION_TIME

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	float					m_fAlpha;			/// HP와 MP의 게이지에 사용(검은배경, 각 게이지의 색)		
	bool					m_bShow;

	//std::bitset<GUFS_END>		m_bitsetGageUiFactorShow;	/// 한개라도 set(1)인 비트가 있으면 UI가 보이지 않도록 함(처음엔 모두 0으로 시작)

};

IMPLEMENT_INTRUSIVE_PTR( CX2GageUI )

#define KIND_OF_HP_GAGE_BAR_COLOR 7
#define MAX_NUM_OF_POST_FIX_COLOR 5

class CX2BossGageUI;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BossGageUI> CX2BossGageUIPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BossGageUI> CX2BossGageUIPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2BossGageUI : public CX2GageUI
{
public:
	CX2BossGageUI( CX2GameUnit* pGameUnit_ );
	virtual ~CX2BossGageUI();

	virtual void			InitUI();
	void					SetBossGageTexture( const WCHAR* wszFaceTexName_, const WCHAR* wszFaceTexPieceName_ );
	void					ResetBossUIPos();
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	
	virtual void			SetShow( const bool bShow_ )
	{ 
		if ( NULL != m_pDLGBossGage )
		{
			CX2GageUI::SetShow( bShow_ );
			m_pDLGBossGage->SetShow( bShow_ );
		}
	}

	USHORT GetNumOfGage() const { return m_usNumOfGage; }
	void SetNumOfGage( const USHORT usNumOfGage_ ) { m_usNumOfGage = usNumOfGage_; }
	void SetNumberIcon( CKTDGUIControl::CPictureData* pPictureData_, const int iNum_ );

	bool IsOwnerGameUnit( const UidType uidBoss_ );
// 	{
// 		if ( null != m_optrGameUnit || uidBoss_ != m_optrGameUnit->GetUnitUID() )
// 			return false;
// 		else
// 			return true;
// 	}
public:
	// 기본 길이
	const static float MAGIC_LEFT_HP_BAR_LENGTH;// = 11.f;
	const static float MAGIC_RIGHT_HP_BAR_LENGTH;// = 16.f;
	const static float MAGIC_HP_BAR_LENGTH;// = 626.f + MAGIC_LEFT_HP_BAR_LENGTH + MAGIC_RIGHT_HP_BAR_LENGTH;

	const static WCHAR WCHAR_HP_BAR_POST_FIX[KIND_OF_HP_GAGE_BAR_COLOR][MAX_NUM_OF_POST_FIX_COLOR];// = { L"_V", L"_PB" };

private:
	CX2GameUnitoPtr		m_optrGameUnit;
	CKTDGUIDialogType	m_pDLGBossGage;
	vector<wstring>		m_vecHpBarColor;
	//wstring				m_wstrHPBar[KIND_OF_HP_GAGE_BAR_COLOR];		/// HP Bar 색상 키 값

	USHORT				m_usNumOfGage;		/// 게이지의 갯수
#ifdef SKILL_SLOT_UI_TYPE_B
	bool				m_bSkillSlotUITypeA;
#endif //SKILL_SLOT_UI_TYPE_B
};

class CX2SmallGageUI : public CX2GageUI 
{

public:
	struct DRAWFACE_RHW_VERTEX
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};

	CX2SmallGageUI( CX2GameUnit* pGameUnit_ );
	virtual ~CX2SmallGageUI();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual	void			OnFrameRender();

	void					SetHpTexture( const WCHAR* pTexName_ );
	void					SetMpTexture( const WCHAR* pTexName_ );

protected:
	void					DrawFace( float fX_, float fY_, float fWidth_, float fHeight_, D3DCOLOR color_ /* = 0xffffffff */);

private:
	void					DrawSmallGage();
	void					DrawHpGage( const D3DXVECTOR3& vOut_ );
	void					DrawMpGage( const D3DXVECTOR3& vOut_ );

	CX2GameUnitoPtr					m_optrGameUnit;
	CKTDGStateManager::KStateID		m_RenderStateID;	/// 인터페이스 출력을 위한 2D 출력함수
	CKTDXDeviceTexture*				m_pHPSmallBar;
	CKTDXDeviceTexture*				m_pMPSmallBar;
	float							m_fMPGageAlpha;		/// MP 게이지의 Fade In, Out에 사용
};

class CX2MySmallGageUI : public CX2GageUI
{
public:
	CX2MySmallGageUI( CX2GameUnit* pGameUnit_ );
	virtual ~CX2MySmallGageUI();

	virtual void			InitUI();
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	
	virtual bool			GetShow() const;
	virtual void			SetShow( const bool bShow_ );

	void					CreateAirGage();
	void					DestroyAirGage();
	void					UpdateAirGage();

private:
	CX2GameUnitoPtr					m_optrGameUnit;
	CKTDGUIDialogType				m_pDlgAirGage;
};

class CX2StatusGageUI : public CX2GageUI
 {
public:
	CX2StatusGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
		: CX2GageUI(), m_pGageData( pGageData_ ), m_eOwnerGameUnitClass( eOwnerUnitClass_ )
	{}

	virtual ~CX2StatusGageUI();
	
	//virtual void			CrashMyStateGageBar( const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, D3DXCOLOR changeColor );
	CX2Unit::UNIT_CLASS		GetOwnerUnitClass() const { return m_eOwnerGameUnitClass; }
	void					SetOwnerUnitClass( CX2Unit::UNIT_CLASS eOwnerGameUnitClass_ ) { m_eOwnerGameUnitClass = eOwnerGameUnitClass_; }

#ifdef BUFF_ICON_UI
	virtual	void			PushBuff( const BuffIcon& BuffIconInfo_, bool bIsDeBuff_);
	virtual	void			EraseBuff(BUFF_TEMPLET_ID eBuffID_, bool bIsDeBuff_);
	virtual	void			ClearBuffList();
	virtual	void			UpdateBuffIcon(){}
	virtual void			NotifyDurationTime5sec(BUFF_TEMPLET_ID eBuffID_);
	virtual void			InitBuffIconFlicker(BUFF_TEMPLET_ID eBuffID_);
#endif //BUFF_ICON_UI	

protected:
	virtual void			SetCharacterImage( const CX2Unit::UNIT_CLASS eGameUnitClass_ ) = NULL;
	void	UpdateGageForUV( IN CKTDGUIStatic* pStaticBar_, IN int iPicNum, IN const WCHAR* pUVName_, 
							 IN float fPercent_, IN bool isWidth_ );

#ifdef BUFF_ICON_UI
	void	SetBuffIconStatic( const BuffIcon& BuffIconInfo_, int iIndex_, bool bIsDebuff_, const D3DXVECTOR2& vSize_ );
#endif //BUFF_ICON_UI

protected:
	CX2GageData*				m_pGageData;
	CX2Unit::UNIT_CLASS			m_eOwnerGameUnitClass;
#ifdef BUFF_ICON_UI
	CKTDGUIDialogType			m_pDlgBuffIcon;		//버프, 디버프 아이콘

	//출력 순서를 보장해주기 위해 vector로 작업
	vector<BuffIcon>			m_vecBuffList;		//현재 걸려있는 버프 리스트
	vector<BuffIcon>			m_vecDebuffList;	//현재 걸려있는 디버프 리스트
#endif //BUFF_ICON_UI
};

class CX2MyGageUI : public CX2StatusGageUI 
{
public:

public:
	CX2MyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );
	virtual ~CX2MyGageUI();

	virtual void			SetShow( const bool bShow_ );
	virtual void			InitUI();
	void					InitStatusUI();
	virtual void			InitWakeOrb();
	
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			CrashMyStateGageUI( const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, D3DXCOLOR changeColor );
	void					SetOnPopup(bool bVal){m_bOnPopUp = bVal;}
	bool					GetOnPopup() const { return m_bOnPopUp; }
#ifdef BUFF_ICON_UI
	virtual	void			UpdateBuffIcon();
	void					SetBuffIconGuideDesc();
#endif //BUFF_ICON_UI	
	virtual void			SetCharacterImage( const CX2Unit::UNIT_CLASS eGameUnitClass_ );
#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG
	virtual void			ResetGageUIEtc( const CX2Unit::UNIT_CLASS eGameUnitClass_ ){}
#endif // FIX_CHUNG_GAGE_UI_UPDATE_BUG

#ifdef DISPLAY_BUFF_DURATION_TIME
	virtual void SetDurationTime( BUFF_TEMPLET_ID eBuffID_, int iTime_);
#endif // DISPLAY_BUFF_DURATION_TIME
protected:		
	// 오현빈 // 엘리시스 섬멸 기폭 구현을 위해 가상함수로 변경
	virtual void			UpdateMpAndHyperAndDetonationGage();
	virtual void			UpdateDetonationGage( CKTDGUIStatic* pStaticStateBar_ );	/// 기폭 게이지

	void					UpdateHyperModeOrb();
	void					UpdateHpGage();
	void					UpdateMpGageAndString( CKTDGUIStatic* pStaticStateBar_ );
	void					UpdateMpGuidePoint();
	virtual void			UpdateHyperGage( CKTDGUIStatic* pStaticStateBar_ );			/// 애드의 DP 시스템을 위해, 가상 함수로 변경
	

	void					UpdateHyperGageString( const float fNowHyper_, const float fMaxHyper_ );
	virtual void			UpdateHyperModeRemainTime();								/// 애드의 DP 시스템을 위해, 가상 함수로 변경
	void					UpdatePartyLeaderUI();
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	bool					IsMyPlayerFaulty () const { return m_bIsMyPlayerFaulty; }
	void					IsMyPlayerFaulty (bool val_) { m_bIsMyPlayerFaulty = val_; }

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM


	void					UpdateInfoString();

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	void				    UpdateTradeBlockDesc( std::wstring& wstrBuffDesc );
#endif // SERV_NEW_UNIT_TRADE_LIMIT

protected:
	CKTDGUIDialogType			m_pDLGMyUnit;
	bool	m_bOnPopUp;
	bool	m_bIsShow;
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	bool	m_bIsMyPlayerFaulty;
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

};

class CX2ChungMyGageUI : public CX2MyGageUI
{
public:
	CX2ChungMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );

	virtual void			InitUI();
	virtual void			InitWakeOrb();
	void					InitCannonBallUI();

#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	
	void					UpdateCharacterImage();
	void					UpdateCannonBallCountUI();

	void					InitNumOfVerticalOfCannonBall( const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );

	bool					GetUseExtraCannonBallGage() const { return m_bUseExtraCannonBallGage; }
	void					SetUseExtraCannonBallGage( const bool bUseExtraCannonBallGage_ ) { m_bUseExtraCannonBallGage = bUseExtraCannonBallGage_; }

#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG
	virtual void			ResetGageUIEtc( const CX2Unit::UNIT_CLASS eGameUnitClass_ ){ InitNumOfVerticalOfCannonBall(eGameUnitClass_); InitCannonBallUI();}
#endif // FIX_CHUNG_GAGE_UI_UPDATE_BUG
protected:
	int						GetNumOfVerticalOfCannonBall() const { return m_iNumOfVerticalOfCannonBall; }
	void					SetNumOfVerticalOfCannonBall( const int iNumOfVerticalOfCannonBall_ ) { m_iNumOfVerticalOfCannonBall = iNumOfVerticalOfCannonBall_; }

	void					GetBerserkModeMyStateImage( OUT wstring& wstrFileName, OUT wstring& wstrPieceName, IN const CX2Unit::UNIT_CLASS eClassType );

private:
	int						m_iNumOfVerticalOfCannonBall;	
	bool					m_bUseExtraCannonBallGage;
	
};

class CX2ElswordMyGageUI : public CX2MyGageUI
{
public:
	CX2ElswordMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
		: CX2MyGageUI( pGageData_, eOwnerUnitClass_ ),
		m_pStaticElswordVigor( NULL ), m_pStaticElswordDest( NULL ), m_bShowWspParticle( true )
	{
		m_hSeqVigorEffect[0]	= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqVigorEffect[1]	= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqVigorEffect[2]	= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqDestEffect[0]		= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqDestEffect[1]		= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqDestEffect[2]		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	}

	virtual ~CX2ElswordMyGageUI();

	virtual void			InitUI();
	virtual void			SetShow( const bool bShow_ );
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	void InitUIElswordWSP();
	void StopUIEffectElswordWS();
	void PlayUIEffectDestruction();
	void PlayUIEffectVigor();	
	void UIFrameMoveElswordWSP();
	void UpdateUIElswordWSP();

protected:
	bool GetShowWspParticle() const { return m_bShowWspParticle; }
	void SetShowWspParticle( const bool bShowWspParticle_ );

private:
	CKTDGUIStatic*										m_pStaticElswordVigor;
	CKTDGUIStatic*										m_pStaticElswordDest;
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqVigorEffect[3];
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDestEffect[3];

	bool												m_bShowWspParticle;
	
};

class CX2RavenMyGageUI : public CX2MyGageUI 
{
public:
	CX2RavenMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );
	virtual ~CX2RavenMyGageUI();

	virtual void			InitWakeOrb();
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void					UpdateHyperOrb();

private:
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hSeqHyperBall;
};

class CX2PartyMemberGageUI : public CX2StatusGageUI
{

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템

public :
	enum BAN_VOTE_BUTTON_TYPE	// 투표 버튼 타입
	{ 
		BVBT_NONE = 0,  		// 던전 진행 중, 
		BVBT_NEED_VOTE,			// 투표 해야하는 상태
		BVBT_COMPLETE_VOTE,		// 투표 완료
//		BVBT_FAULTY_USER_VOTE,	// 불량 유저 상태
	};

#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템




public:
	CX2PartyMemberGageUI( const KPartyUserInfo& kPartyUserInfo_, CX2GageData* pGageData_, const UINT uiPositionIndex_ ) 
		: CX2StatusGageUI( pGageData_, static_cast<CX2Unit::UNIT_CLASS>( kPartyUserInfo_.m_cUnitClass ) ),
		m_pDLGOtherUnit( NULL ), m_wstrNickName( kPartyUserInfo_.m_wstrNickName ),
		m_uiLevel( kPartyUserInfo_.m_ucLevel ), m_uidGameUnit( kPartyUserInfo_.m_iUnitUID ), 
		m_uiPositionIndex( uiPositionIndex_ ), 
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템
		m_iVoteButtonType ( BVBT_NONE ),		// PartyMemberGageUI 의 강퇴 버튼 타입
		m_pButtonVotePlayer ( NULL ),			// 해당 버튼의 Dlg 포인터
		m_pButtonVoteComplete ( NULL ),			// 해당 버튼의 Dlg 포인터
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템
		
		m_bOnPopUp( false )
#ifdef SKILL_SLOT_UI_TYPE_B
		, m_bIsSkillUITypeA ( false )
#endif //SKILL_SLOT_UI_TYPE_B
	{}

	virtual ~CX2PartyMemberGageUI();

	virtual void			InitUI();	
	void					SetLevelString( const UINT uiLevel_ );
	void					SetNickNameString( const wstring& wstrNickName_ );
	void					SetPosition( const UINT uiPositionIndex_ );

	virtual void			SetShow( const bool bShow_ );

#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	
	virtual void			CrashMyStateGageUI( const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, D3DXCOLOR changeColor ) { ASSERT( !L"Crash Base Function is not available" ); }
	virtual void			SetHpTexture( const WCHAR* pTexName_ ) { ASSERT( !L"Hp Base function is not available"); }
	virtual void			SetMpTexture( const WCHAR* pTexName_ ) { ASSERT( !L"Mp Base function is not available"); }

	void					UpdateGageDataFromGameUnit();
	void					UpdateResurrectShortKey();
	void					UpdatePartyLeaderUI();

	UINT					GetPositionIndex() const { return m_uiPositionIndex; }
	void					SetPositionIndex( const UINT uiPosIndex_ ) 
	{ 
		m_uiPositionIndex = uiPosIndex_; 
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		if( NULL != m_pButtonVotePlayer )
		{
			if( false == m_pButtonVotePlayer->SetDummyInt(0, m_uiPositionIndex ) )
				m_pButtonVotePlayer->AddDummyInt( m_uiPositionIndex );
		}
#endif //SERV_DUNGEON_FORCED_EXIT_SYSTEM
	}
	
	//CX2Unit::UNIT_CLASS GetOwnerUnitClass() const { return m_eOwnerGameUnitClass; }
	//void SetOwnerUnitClass( CX2Unit::UNIT_CLASS eOwnerGameUnitClass_ ) { m_eOwnerGameUnitClass = eOwnerGameUnitClass_; }

	bool					GetOnPopup() const { return m_bOnPopUp; }

#ifdef BUFF_ICON_UI
	virtual		void		UpdateBuffIcon();
#endif //BUFF_ICON_UI	

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템
	void					UpdateFaultyPlayerUI ( const bool b_faultyValue_ );											// 투표 버튼의 UI 를 Update 함
	void					SetShowRelationVoteButtons ( const bool bVote_, const bool bComplete_ );		// 투표 버튼과 투표 완료 버튼의 보여주기 설정을 함	
	BAN_VOTE_BUTTON_TYPE	GetVoteButtonType () const { return m_iVoteButtonType; }				// 투표 버튼의 상태를 반환
	void					SetVoteButtonType ( const BAN_VOTE_BUTTON_TYPE iVoteButtonType_ ) { m_iVoteButtonType = iVoteButtonType_; }	// 투표 버튼의 상태를 설정
	CKTDGUIButton *			GetVoteReadyButton () const;				// 투표 준비 버튼을 반환
	CKTDGUIButton *			GetVoteCompleteButton () const { return m_pButtonVoteComplete; }		// 투표 완료 버튼을 반환
	const wstring &			GetNicknameThisPartyMemeber() const { return m_wstrNickName; }		// 해당 GageUI 를 갖고 있는 소유주의 아이디를 반환함
	const UidType &			GetUidTypeThisPartyMember () const { return m_uidGameUnit; }			// 해당 GageUI 를 갖고 있는 소유주의 UID 를 반환함 
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템


protected:
	virtual		void CX2PartyMemberGageUI::SetCharacterImage( const CX2Unit::UNIT_CLASS eGameUnitClass_ );



protected:
	CKTDGUIDialogType		m_pDLGOtherUnit;

private:
	wstring					m_wstrNickName;
	UINT					m_uiLevel;
	UidType					m_uidGameUnit;
	UINT					m_uiPositionIndex;

	bool	m_bOnPopUp;
#ifdef SKILL_SLOT_UI_TYPE_B
	bool					m_bIsSkillUITypeA;
#endif //SKILL_SLOT_UI_TYPE_B.

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM		// 13-01-15 / 던전 플레이 중, 불량 유저 강제 퇴장 시스템
	CKTDGUIButton *			m_pButtonVotePlayer;			// 강퇴하기 버튼
	CKTDGUIButton *			m_pButtonVoteComplete;			// 강퇴완료 버튼
	BAN_VOTE_BUTTON_TYPE	m_iVoteButtonType;				// 현재 버튼의 타입, 0 = 아무 상태도 아님, 1 = 강퇴 버튼 입력 받는 중, 2 = 입력 받기 완료, 3 = 불량 유저 판별
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM


};

class CX2PVPPlayerGageUI : public CX2StatusGageUI
{
public:
	CX2PVPPlayerGageUI( const CX2Room::SlotData& pPvpSlot_, CX2GageData* pGageData_, const UINT uiPositionIndex_, char cRank_ )
		: CX2StatusGageUI( pGageData_, pPvpSlot_.m_pUnit->GetClass() ), m_wstrNickName( pPvpSlot_.m_pUnit->GetNickName() ),
		m_uiLevel( pPvpSlot_.m_pUnit->GetPrevLevel() ), m_uiPositionIndex( uiPositionIndex_ ), m_uiMyTeam( pPvpSlot_.m_TeamNum ),
		m_cRank(cRank_), m_iNpcId(-1)
#ifdef SKILL_SLOT_UI_TYPE_B
		, m_bIsSkillUITypeA ( false )
#endif //SKILL_SLOT_UI_TYPE_B
	{}


	CX2PVPPlayerGageUI( const CX2Room::RoomNpcSlot& npcSlotData_, CX2GageData* pGageData_, const UINT uiPositionIndex_, char cRank_ )
		: CX2StatusGageUI( pGageData_, CX2Unit::UC_NONE ), m_wstrNickName( npcSlotData_.m_wstrNpcName ),
		m_uiLevel( -1 ), m_uiPositionIndex( uiPositionIndex_ ), m_uiMyTeam(  CX2Room::TN_BLUE ),
		m_cRank(cRank_), m_iNpcId( npcSlotData_.m_iNpcId )
#ifdef SKILL_SLOT_UI_TYPE_B
		, m_bIsSkillUITypeA ( false )
#endif //SKILL_SLOT_UI_TYPE_B
	{}
	
	virtual ~CX2PVPPlayerGageUI();

	virtual void	InitUI();
	void			SetLevelString( IN const UINT uiLevel_ );
	void			SetNickNameString( IN const wstring& wstrNickName );
	void			SetPosition( IN const UINT uiPositionIndex_ );
	virtual void	SetShow( const bool bShow_ );

#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void			UpdatePvpRank();

	virtual void	CrashMyStateGageUI( const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, D3DXCOLOR changeColor )
	{
		ASSERT( !L"Crash Base Function is not available" );
	}
	virtual void	SetHpTexture( const WCHAR* pTexName_ )
	{
		ASSERT( !L"Hp Base function is not available");
	}
	virtual void	SetMpTexture( const WCHAR* pTexName_ )
	{
		ASSERT( !L"Mp Base function is not available");
	}

	void	UpdateGageDataFromGameUnit();
	UINT	GetPositionIndex() const
	{
		return m_uiPositionIndex;
	}
	void	SetPositionIndex( const UINT uiPosIndex_ )
	{
		m_uiPositionIndex	= uiPosIndex_;
	}

	void	SetRankImage();

#ifdef BUFF_ICON_UI
	virtual	void			UpdateBuffIcon();
#endif //BUFF_ICON_UI

protected:
	virtual void CX2PVPPlayerGageUI::SetCharacterImage( const CX2Unit::UNIT_CLASS eGameUnitClass_ );


protected:
	CKTDGUIDialogType		m_pDLGOtherUnit;

private:
	wstring		m_wstrNickName;		/// 캐릭터명
	UINT		m_uiLevel;			/// 레벨
	UINT		m_uiPositionIndex;	/// 포지션 지정 및 카운팅 변수
	UINT		m_uiMyTeam;			/// RED, BLUE, NONE 팀 구별
	char		m_cRank;
	int			m_iNpcId;			/// 대전NPC초상화 설정을 위한 NPCUid
#ifdef SKILL_SLOT_UI_TYPE_B
	bool					m_bIsSkillUITypeA;
#endif //SKILL_SLOT_UI_TYPE_B
};

#pragma region CX2AraMyGageUI
/** @class : CX2AraMyGageUI
	@brief : 아라의 기력 UI
	@date  : 2012/11/30
*/
class CX2AraMyGageUI : public CX2MyGageUI
{
public:
	CX2AraMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );

	virtual void	InitUI();
	virtual void	InitWakeOrb();
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	
	void	InitForcePowerUI();
	void	UpdateCharacterImage();
	void	UpdateForcePowerUI();
		
protected:
	void	GetBerserkModeMyStateImage( OUT wstring& wstrFileName, OUT wstring& wstrPieceName, IN const CX2Unit::UNIT_CLASS eClassType );
};
#pragma endregion 클래스

#ifdef NEW_CHARACTER_EL
class CX2ElesisMyGageUI : public CX2MyGageUI
{
public:
	CX2ElesisMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ )
		: CX2MyGageUI( pGageData_, eOwnerUnitClass_ ),
		m_pStaticElswordVigor( NULL ), m_pStaticElswordDest( NULL ), m_bShowWspParticle( true )
	{
		m_hSeqVigorEffect[0]	= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqVigorEffect[1]	= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqVigorEffect[2]	= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqDestEffect[0]		= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqDestEffect[1]		= INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hSeqDestEffect[2]		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	}

	virtual ~CX2ElesisMyGageUI();

	virtual void			InitUI();
	virtual void			SetShow( const bool bShow_ );
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	void InitUIElswordWSP();
	void StopUIEffectElswordWS();
	void PlayUIEffectDestruction();
	void PlayUIEffectVigor();	
	void UIFrameMoveElswordWSP();
	void UpdateUIElswordWSP();

protected:
	bool GetShowWspParticle() const { return m_bShowWspParticle; }
	void SetShowWspParticle( const bool bShowWspParticle_ );

	virtual void UpdateMpAndHyperAndDetonationGage();
	virtual void UpdateDetonationGage( CKTDGUIStatic* pStaticStateBar_ );	/// 기폭 게이지

private:
	CKTDGUIStatic*										m_pStaticElswordVigor;
	CKTDGUIStatic*										m_pStaticElswordDest;
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqVigorEffect[3];
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqDestEffect[3];

	bool												m_bShowWspParticle;

};
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
class CX2AddMyGageUI : public CX2MyGageUI
{
public:
	CX2AddMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );

	virtual void			InitUI();
	virtual void			InitWakeOrb();				/// 각성 구슬 UI 초기화
	void					InitDPGageUI();				/// DP 게이지 UI 초기화
	void					InitMutationCountUI();		/// 변이 수치 UI 초기화
	void					InitFormationModeConsumeDPGuide();	/// 구성 모드 전환량 표시 가이드

#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	void					UpdateMutationCountUI();	/// 변이 수치 UI 갱신
	virtual void			UpdateHyperGage( CKTDGUIStatic* pStaticStateBar_ );			/// 애드의 DP 시스템을 위해, 가상 함수로 변경
	virtual void			UpdateMpAndHyperAndDetonationGage();

#ifdef FIX_CHUNG_GAGE_UI_UPDATE_BUG
	virtual void			ResetGageUIEtc( const CX2Unit::UNIT_CLASS eGameUnitClass_ )
	{ 
		InitMutationCountUI(); 
		InitDPGageUI(); 
	}
#endif // FIX_CHUNG_GAGE_UI_UPDATE_BUG

protected:
	/// 전직에 따른 각성시 초상화 이미지 설정
	void					UpdateDPGage();
	void					UpdateDPGuidePoint();

	virtual	void			UpdateHyperModeRemainTime();
};
#endif //SERV_9TH_NEW_CHARACTER


#ifdef ADD_RENA_SYSTEM //김창한
/** @class : CX2RenaMyGageUI
	@brief : 레나의 NF UI
	@date  : 2012/11/30
*/
class CX2RenaMyGageUI : public CX2MyGageUI
{
public:
	CX2RenaMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );

	virtual void	InitUI();
#ifdef      X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void			OnFrameMove( float fElapsedTime_ );
#else       X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void			OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	
	void	InitNaturalForceUI();
	void	UpdateNaturalForceUI();
};
#endif //ADD_RENA_SYSTEM


#ifdef ADD_EVE_SYSTEM_2014		// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
class CX2EveMyGageUI : public CX2MyGageUI
{
public:
	CX2EveMyGageUI( CX2GageData* pGageData_, const CX2Unit::UNIT_CLASS eOwnerUnitClass_ );
	virtual void			InitWakeOrb();
};
#endif // ADD_EVE_SYSTEM_2014	// 김종훈, 2014 - 이브 추가 시스템, 나소드 코어
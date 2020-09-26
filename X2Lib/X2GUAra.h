#pragma once
/**
	@file	X2GUAra.h
	@author	JHKang
	@brief	아라 클래스가 정의된 파일
	@since  2012-11-06
	
	@date	JHKang on 2012-11-14,
			- h 및 cpp 파일 작업 재개
	@date	JHKang on 2012-11-18,
			- 추가 주석문 추가( file 설명 )
	@data	JHKang on 2012-11-28,
			- 기본 콤보 23종 완료
	@data	JHKang on 2012-11-28,
			- 특수 이동기 제작( 능공허도, 2단 점프 )
	@data	JHKang on 2012-11-29,
			- 스킬 3종 제작 임시( 바람 쐐기, 빨래 널기, 호랑이 발톱 )
	@data	JHKang on 2012-12-06,
			- 기력 시스템 추가 및 수정
	@data	JHKang on 2012-12-07
			- 스킬 캔슬에 따른 모션 분할, 상태 증가
	@data	JHKang on 2013-01-04
			- 아라 1-1, 1-2차 전직 시작( 패시브 : 번득이는 기지, 창술의 극의 )
	@data	JHKang on 2013-01-08
			- 아라 1-1, 1-2차 전직 ( 액티브 : 맹호 1식 - 바위 깨기 )
	@data	JHKang on 2013-01-10
			- 아라 1-1, 1-2차 전직 ( 스페셜 액티브 : 용아 4식 - 달빛 베기 )
	@data	JHKang on 2013-01-11
			- 아라 3각성 아이템 교체 관련 수정
	@data	JHKang on 2013-01-15
			- 아라 1-1, 1-2차 전직 ( 스페셜 액티브 캐시 3필 : 비연 )
	@data	JHKang on 2013-01-17
			- 아라 1-1, 1-2차 전직 ( 액티브 : 맹호 3식 지축 )
	@data	JHKang on 2013-03-28
			- 아라 광폭화 모드일 때 한 벌 아바타 오류 수정 ( FIX_ARA_ONE_PIECE_FOR_BERSERK )
	@data	JHKang on 2013-06-04
			- 아라 스킬 개편( 신속, 철산고, 용추, 천격, 만월 베기, 맹호의 힘, 호신강기, 재빠른 기상 )
	@data	JHKang on 2013-06-20
			- 아라 스킬 개편( 제압 기력 소모 및 데미지 공식 변경 )
	@data	JHKang on 2013-07-02
			- 낙화 상태 분할
*/

#ifdef ARA_CHARACTER_BASE

#pragma region CX2GUAra
/** @class : CX2GUAra
	@brief : 2012 신 캐릭터 "아라" 클래스
	@date  : 2012-11-06
	@desc  : 긴 창을 사용하는 캐릭터, 구미호의 힘을 사용
*/
class CX2GUAra : public CX2GUUser
{
	#pragma region public 
public:
#pragma region enum
	enum ARA_STATE_ID
	{
	#pragma region STATE_ID
		ASI_BASE = USI_END + 1,	/// 아라 State ID

		ASI_HYPER_WAIT,			/// 각성일 때 대기 상태
		ASI_BERSERK_MODE,		/// 3각성 상태
		
		ASI_DASH_JUMP_POWER_LANDING,		/// 빠르게 착지하기

		ASI_STANDUP_ROLLING_FRONT_FRONT,	/// 앞에서 앞으로 구르며 일어나기
		//////////////////////////////////////////////////////////////////////5
		ASI_STANDUP_ROLLING_FRONT_BACK,		/// 앞에서 뒤로 구르며 일어나기
		ASI_STANDUP_ROLLING_BACK_FRONT,		/// 뒤에서 앞으로 구르며 일어나기
		ASI_STANDUP_ROLLING_BACK_BACK,		/// 뒤에서 뒤로 구르며 일어나기

		ASI_STANDUP_ATTACK_FRONT,			/// 전방 공격하면서 일어나기
		ASI_STANDUP_ATTACK_BACK,			/// 후방 공격하면서 일어나기
		//////////////////////////////////////////////////////////////////////10
		ASI_WALL_LANDING,		/// 착지
		ASI_AIRIAL_DASH,		/// 공중 달리기_능공허도
		ASI_DOUBLE_JUMP_FRONT,	/// 이단 점프_앞
		ASI_DOUBLE_JUMP_UP,		/// 이단 점프_위

		ASI_COMBO_Z,			/// 콤보 Z
		//////////////////////////////////////////////////////////////////////15
		ASI_COMBO_ZZ,			/// 콤보 ZZ
		ASI_COMBO_ZZZ,			/// 콤보 ZZZ
		ASI_COMBO_ZZZZ,			/// 콤보 ZZZZ
		ASI_COMBO_ZZZXStart,	/// 콤보 ZZZXStart
		ASI_COMBO_ZZZXCharge,	/// 콤보 ZZZXCharge
		//////////////////////////////////////////////////////////////////////20
		ASI_COMBO_ZZZXAttack,	/// 콤보 ZZZXAttack
		ASI_COMBO_ZZX,			/// 콤보 ZZX
		
		ASI_COMBO_X,			/// 콤보 X
		ASI_COMBO_XX,			/// 콤보 XX
		ASI_COMBO_XXX,			/// 콤보 XXX
		//////////////////////////////////////////////////////////////////////25
		ASI_COMBO_XXXloop,		/// 콤보 XXXloop
		ASI_COMBO_XXXloopX,		/// 콤보 XXXloopX
		ASI_COMBO_XXdownX,		/// 콤보 XXdownX
		ASI_COMBO_XXdownXX,		/// 콤보 XXdownXX_맹호 1식

		ASI_DASH_COMBO_Z,				/// 대쉬 콤보 Z
		//////////////////////////////////////////////////////////////////////30
		ASI_DASH_COMBO_ZpushX_CHARGE,	/// 대쉬 콤보 ZpushX 충전
		ASI_DASH_COMBO_ZpushX_ATTACK,	/// 대쉬 콤보 ZpushX 공격
		ASI_DASH_COMBO_X,				/// 대쉬 콤보 X

		ASI_JUMP_ATTACK_Z,			/// 점프 공격 Z
		ASI_JUMP_ATTACK_X,			/// 점프 공격 X
		//////////////////////////////////////////////////////////////////////35
		ASI_DASH_JUMP_COMBO_Z,		/// 대쉬 점프 콤보 Z
		ASI_DASH_JUMP_COMBO_ZZ,		/// 대쉬 점프 콤보 ZZ
		ASI_DASH_JUMP_COMBO_ZZZ,	/// 대쉬 점프 콤보 ZZZ
		ASI_DASH_JUMP_COMBO_X,		/// 대쉬 점프 콤보 X

		ASI_SA_ARA_WIND_WEDGE_START,		/// 연풍설_바람 쐐기_낭아 2식 첫번째 공격
		//////////////////////////////////////////////////////////////////////40
		ASI_SA_ARA_WIND_WEDGE_END,			/// 연풍설_바람 쐐기_낭아 2식 두번째 공격
		
		ASI_SA_ARA_SHADOW_RISING_JUMP,		/// 벽력뢰_천둥 꽃망울-점프
		ASI_SA_ARA_SHADOW_RISING_LANDING,	/// 벽력뢰_천둥 꽃망울-착지 공격
		ASI_SA_ARA_SHADOW_RISING_END,		/// 벽력뢰_천둥 꽃망울-마지막 공격

		ASI_SA_ARA_PULLING_PIERCE_START,	/// 천의_빨래 널기 첫번째 공격
		//////////////////////////////////////////////////////////////////////45
		ASI_SA_ARA_PULLING_PIERCE_END,		/// 천의_빨래 널기 두번째 공격

		ASI_SA_ARA_TEMPEST_DANCE_START,		/// 선풍_휘모리 첫번째 공격
		ASI_SA_ARA_TEMPEST_DANCE_END,		/// 선풍_휘모리 두번째 공격

		ASI_SA_ARA_SHADOW_KNOT_START,		/// 결영_그림자 매듭_귀살 3식_시작
		ASI_SA_ARA_SHADOW_KNOT_WAIT,		/// 결영_그림자 매듭_귀살 3식_대기
		//////////////////////////////////////////////////////////////////////50
		ASI_SA_ARA_SHADOW_KNOT_CATCH,		/// 결영_그림자 매듭_귀살 3식_무기 받기
		ASI_SA_ARA_SHADOW_KNOT_END,			/// 결영_그림자 매듭_귀살 3식_종료
		
		ASI_SA_ARA_FALLING_DRAGON,		/// 강룡추_구름 떨구기

		ASI_A_ARA_TIGER_CLAW_START,		/// 호랑이 발톱_맹호 2식_첫번째 공격
		ASI_A_ARA_TIGER_CLAW_END,		/// 호랑이 발톱_맹호 2식_두번째 공격
		//////////////////////////////////////////////////////////////////////55

		ASI_A_ARA_FORCE_SPEAR,			/// 기공창

		ASI_A_ARA_GAIN_FORCE,			/// 흡공
	#pragma endregion 노전직

	#pragma region 첫 번째 전직
#ifdef ARA_CHANGE_CLASS_FIRST
		//아라 1차 전직 - 소선(Little Hsien)
		ASI_COMBO_ALH_ZZXX,					/// 콤보ZZXX
		ASI_COMBO_ALH_ZZXX_LANDING,			/// 콤보ZZXX 랜딩
		ASI_DASH_COMBO_ALH_ZZ,				/// 대쉬 콤보 ZZ
		//////////////////////////////////////////////////////////////////////60
		ASI_JUMP_ATTACK_ALH_ZZ,				/// 점프 공격 ZZ

		ASI_STANDUP_ATTACK_FRONT_NO_DOWN,	/// 쓰러뜨리지 않는 전방 공격하면서 일어나기
		ASI_STANDUP_ATTACK_BACK_NO_DOWN,	/// 쓰러뜨리지 않는 후방 공격하면서 일어나기

		ASI_P_ALH_QUICK_STAND_FRONT_FRONT,	/// 재빠른 기상
		ASI_P_ALH_QUICK_STAND_FRONT_BACK,
		//////////////////////////////////////////////////////////////////////65
		ASI_P_ALH_QUICK_STAND_BACK_FRONT,
		ASI_P_ALH_QUICK_STAND_BACK_BACK,
		
		ASI_A_ALH_BREAKING,					/// 이화접목 대기
		ASI_A_ALH_BREAKING_ATTACK,			/// 이화접목 공격(반격)
		ASI_A_ALH_BREAKING_AVOIDANCE,		/// 이화접목 회피
		//////////////////////////////////////////////////////////////////////70
		ASI_A_ALH_ROCK_CRASH_START,		/// 바위 깨기
		ASI_A_ALH_ROCK_CRASH_END,

		ASI_SA_ALH_DOUBLE_COLLISION_START,	/// 팔괘장 첫번째 공격
		ASI_SA_ALH_DOUBLE_COLLISION_END,	/// 팔괘장 두번째 공격

		ASI_SA_ALH_MOONLIGHT_SLASH_READY,		/// 달빛 베기 시작
		//////////////////////////////////////////////////////////////////////75
		ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE,		/// 달빛 베기 충전
		ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK,	/// 달빛 베기(초승달)
		ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK,	/// 달빛 베기(보름달)

		ASI_SA_ALH_POWER_OF_WHITER_TIGER,	/// 맹호격
		ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING,
		//////////////////////////////////////////////////////////////////////80		
		ASI_SA_ALH_PEERLESSNESS_SLASH_START,	/// 무쌍참
		ASI_SA_ALH_PEERLESSNESS_SLASH_END,

		ASI_SA_ALH_DESCENT_DRAGON,				/// 용아 최종오의 - 폭쇄

		//아라 2차 전직 - 제천(Sakra Devanam)
		ASI_DASH_JUMP_COMBO_ASD_ZX,		/// 대쉬 점프 ZX
		ASI_DOUBLE_JUMP_FRONT_COMBO_ASD_Z_BACK,	/// 이단 점프 콤보 Z Back
		//////////////////////////////////////////////////////////////////////85
		ASI_P_ASD_QUICKEN_GUARD,		/// 번득이는 기지 모션
		ASI_P_ASD_QUICKEN_GUARD_SMALL,	/// 번득이는 기지 SMALL_DAMAGE 모션
		
		ASI_SA_ASD_TURBULENT_WAVE_START,	/// 맹호 4식 - 노도
		ASI_SA_ASD_TURBULENT_WAVE_LOOP,	
		ASI_SA_ASD_TURBULENT_WAVE_END,	
		//////////////////////////////////////////////////////////////////////90
		ASI_SA_ALH_SWALLOW_ASSAULT_READY,	/// 비연 시작
		ASI_SA_ALH_SWALLOW_ASSAULT_ATTACK,	/// 비연 공격
		ASI_SA_ALH_SWALLOW_ASSAULT_FINISH,	/// 비연 종료
		ASI_SA_ALH_SWALLOW_ASSAULT_ESCAPE,	/// 비연 탈출
		ASI_A_ASD_LOW_BRANDISH_SLICE,		/// 지축 슬라이딩
		//////////////////////////////////////////////////////////////////////95
		ASI_A_ASD_LOW_BRANDISH_KICK,		/// 지축 킥
		ASI_SA_ASD_VOID_BREAK,				/// 최종오의 맹호 파천
		ASI_SA_ASD_SUPPRESSION,				/// 제압
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		ASI_A_ASD_FALLING_BLOSSOM_FINISH,	/// 낙화
#endif //UPGRADE_SKILL_SYSTEM_2013
		//////////////////////////////////////////////////////////////////////100
#endif //ARA_CHANGE_CLASS_FIRST
	#pragma endregion 1-1, 1-2차 전직

	#pragma region 두 번째 전직
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		ASI_COMBO_ALD_ZZXX,
		ASI_COMBO_ALD_ZZZFrontZ,
		ASI_DASH_JUMP_COMBO_ALD_ZX,
		ASI_DASH_JUMP_COMBO_ALD_ZXX,
		ASI_DASH_JUMP_COMBO_AYR_XZ,
		//////////////////////////////////////////////////////////////////////105
		ASI_DASH_COMBO_AYR_ZZ,
		ASI_A_AYR_HIGH_SPEED,				/// 아라 1차 전직 소마 - 쾌속
		ASI_A_ALD_WOLF_CLAW,				/// 아라 1차 전직 소마 - 낭아 3식 늑대발톱
		ASI_A_ALD_WOLF_CLAW_FINISH,			/// 아라 1차 전직 소마 - 낭아 3식 늑대발톱 마무리
		ASI_SA_ALD_HELL_OF_TORNADO_FINISH,	/// 아라 1차 전직 소마 - 연환지옥
		//////////////////////////////////////////////////////////////////////110
		ASI_SA_ALD_WOLF_SPEAR,				/// 아라 1차 전직 소마 - 낭아 4식 늑대이빨
		ASI_SA_ALD_WOLF_SPEAR_FINISH,		/// 아라 1차 전직 소마 - 낭아 4식 늑대이빨 마무리
		ASI_SA_ALD_WOLF_BREAK,				/// 아라 1차 전직 소마 - 낭아 최종오의 - 극쇄
		ASI_SA_ALD_FINGER_BULLET,			/// 아라 1차 전직 소마 - 나찰 1식 흡혼
		ASI_SA_ALD_FINGER_BULLET_FINISH,	/// 아라 1차 전직 소마 - 나찰 1식 흡혼 마무리
		//////////////////////////////////////////////////////////////////////115
		ASI_SA_ALD_ENERGY_BEAD,				/// 아라 1차 전직 소마 - 나찰 2식 연환장
		ASI_SA_ALD_ENERGY_BEAD_FINISH,		/// 아라 1차 전직 소마 - 나찰 2식 연환장 마무리
		ASI_SA_ALD_PRISON_SPEAR,			/// 아라 1차 전직 소마 - 나찰 3식 창살감옥
		ASI_SA_ALD_PRISON_SPEAR_FINISH,		/// 아라 1차 전직 소마 - 나찰 3식 창살감옥 마무리
		ASI_A_AYR_STEAL_SOUL,				/// 아라 2차 전직 명왕 - 나찰 4식 탈혼
		//////////////////////////////////////////////////////////////////////120
		ASI_A_AYR_STEAL_SOUL_FINISH,		/// 아라 2차 전직 명왕 - 나찰 4식 탈혼 마무리
		ASI_SA_AYR_OPEN_THE_HELL,			/// 아라 2차 전직 명왕 - 나찰 최종오의 - 연옥
		ASI_SA_AYR_ENERGY_VOID_START,		/// 아라 2차 전직 명왕 - 초진공장 준비
		ASI_SA_AYR_ENERGY_VOID_LOOP,		/// 아라 2차 전직 명왕 - 초진공장 차지
		ASI_SA_AYR_ENERGY_VOID_END,			/// 아라 2차 전직 명왕 - 초진공장 마무리
		//////////////////////////////////////////////////////////////////////120
		ASI_SA_AYR_SUPPRESSION_ENERGY,		/// 아라 2차 전직 명왕 - 제압 - 기
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	#pragma endregion 2-1, 2-2차 전직

	};
	#pragma endregion ARA_STATE_ID

	#pragma region enum
	enum ARA_MAJOR_PARTICLE_INSTANCE_ID
	{
		ARA_MAJOR_PII_BASE,
		ARA_MAJOR_PII_END,
	};
	#pragma endregion ARA_MAJOR_PARTICLE_INSTANCE_ID

	#pragma region enum
	enum ARA_MINOR_PARTICLE_INSTANCE_ID
	{
		ARA_MINOR_PII_BASE,
		ARA_MINOR_PII_END,
	};
	#pragma endregion ARA_MINOR_PARTICLE_INSTANCE_ID

	#pragma region enum
	enum ARA_MAJOR_MESH_INSTANCE_ID
	{
		ARA_MAJOR_MII_BASE,
		ARA_MAJOR_MII_END,
	};
	#pragma endregion ARA_MAJOR_MESH_INSTANCE_ID

	#pragma region class
	/** @class : CForcePowerSystem
		@brief : 기력 시스템
		@since  2012-11-06
		@date	JHKang on 2012-11-30,
				- 설계 및 디자인
	*/
	class CForcePowerSystem
	{
	private:
		int			m_iUsedForcePowerAtThisState;	/// 현재 스테이트에서 소모한 기력의 갯수
		
	public:
		CForcePowerSystem( void ) : m_iUsedForcePowerAtThisState( 0 )
		{}

		int GetUsedForcePowerAtThisState() const
		{
			return m_iUsedForcePowerAtThisState;
		}

		void SetUsedForcePowerAtThisState( int iUsedForcePowerAtThisState_)
		{
			m_iUsedForcePowerAtThisState = iUsedForcePowerAtThisState_;
		}

	};
	#pragma endregion CForcePowerSystem

	#pragma region struct
	/** @struct SMoonLightSlashData
		@brief  달빛 베기 데이터
		@since  2012-11-06
		@date	JHKang on 2013-1-14,
				- 레피-와일드 차지 참조
	*/
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    struct sMoonLightSlashData;
    typedef boost::intrusive_ptr<sMoonLightSlashData> sMoonRightSlashDataPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::shared_ptr<sMoonLightSlashData> sMoonRightSlashDataPtr;		/// 스마트 포인터 타입
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	struct sMoonLightSlashData : boost::noncopyable
	{
	private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		float	m_fChargingTime;		/// 충전 시간
		float	m_fPowerRate;			/// 충전 시간에 따른 데미지 비율
		bool	m_bSlotB;				/// A/B Slot 확인
		bool*	m_pChargeKey;			/// 차지 키 확인
		
		int						m_iSlotID;			/// 장착된 스킬슬롯을 알아내기 위한 용도
		CX2EffectSet::Handle	m_hChargeEffect;	/// 충전 시 발생하는 이펙트
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hNewAttackBox;		/// 무기 Attack라인 변경

		sMoonLightSlashData() : m_fChargingTime( 0.f ), m_fPowerRate( 0.f ), m_bSlotB( false ),
			m_pChargeKey( NULL ), m_iSlotID( 0 ),
			m_hChargeEffect( INVALID_EFFECTSET_HANDLE ), m_hNewAttackBox( INVALID_MESH_INSTANCE_HANDLE )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{}
		
	public:

		static sMoonRightSlashDataPtr CreateMoonLightSlashData() { return sMoonRightSlashDataPtr( new sMoonLightSlashData ); }

		float	GetChargingTime() const { return m_fChargingTime; }
		void	SetChargingTime( float fChargingTime ) { m_fChargingTime = fChargingTime; }

		float	GetPowerRate() const { return m_fPowerRate; }
		void	SetPowerRate( float fPowerRate ) { m_fPowerRate = fPowerRate; }

		int GetSlotID() const { return m_iSlotID; }
		void SetSlotID( int iStateID ) { m_iSlotID = iStateID; }

		bool	GetSlotB() const { return m_bSlotB; }
		void	SetSlotB( bool bSlotB_ ) { m_bSlotB = bSlotB_; }

		bool	GetChargeKey() const { return (NULL != m_pChargeKey ? *m_pChargeKey : false); }
		void	SetChargeKey( bool* pChargeKey_ ) { m_pChargeKey = pChargeKey_; }

		CX2EffectSet::Handle GetHandleEffectSet() const { return m_hChargeEffect; }
		CX2EffectSet::Handle& GetHandleReferenceEffectSet() { return m_hChargeEffect; }
		void SetHandleEffectSet( CX2EffectSet::Handle hHandle_ ) { m_hChargeEffect = hHandle_; }
		
		CKTDGXMeshPlayer::CXMeshInstanceHandle GetHandleAttackMesh() const { return m_hNewAttackBox; }
		CKTDGXMeshPlayer::CXMeshInstanceHandle& GetHandleRefAttackMesh() { return m_hNewAttackBox; }
		void SetHandleAttackMesh( CKTDGXMeshPlayer::CXMeshInstanceHandle hHandle_ ) { m_hNewAttackBox = hHandle_; }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        void    AddRef()    {   ++m_uRefCount; }
        void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	};
	#pragma endregion SMoonLightSlashData

	#pragma region SLockOnSystem 

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    struct SLockOnSystem;
    typedef boost::intrusive_ptr<SLockOnSystem> sLockOnSystemPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    typedef boost::shared_ptr<SLockOnSystem> sLockOnSystemPtr;		/// 스마트 포인터 타입
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	struct SLockOnSystem : boost::noncopyable
	{
	private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		bool		m_bLockOnUnit;		/// LockOn 성공 여부
		float		m_fPvpRange;		/// 대전 범위
		float		m_fPveRange;		/// 던전 범위
		D3DXVECTOR3	m_vDestPos;			/// 상대 위치
		D3DXVECTOR3	m_vSafePos;			/// 최근 이동한 가장 안전한 위치
		UidType		m_BeforeTargetUID;	/// 이전에 타겟된 유닉의 UID
		
		SLockOnSystem() : m_bLockOnUnit( false ), m_fPvpRange( 0.f ), m_fPveRange( 0.f ), m_BeforeTargetUID( -1 )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{
			INIT_VECTOR3( m_vDestPos,	0.f, 0.f, 0.f );
			INIT_VECTOR3( m_vSafePos,	0.f, 0.f, 0.f );
		}

	public:

		static sLockOnSystemPtr CreateLockOnSystem() { return sLockOnSystemPtr( new SLockOnSystem ); }

		bool	CheckLockOnUnit() { return m_bLockOnUnit; }
		void	ResetLockOnUnit() { m_bLockOnUnit = false; }
		
		float	GetPvpRange() { return m_fPvpRange; }
		void	SetPvpRange( float fRange_ ) { m_fPvpRange = fRange_; }
		
		float	GetPveRange() { return m_fPveRange; }
		void	SetPveRange( float fRange_ ) { m_fPveRange = fRange_; }

		D3DXVECTOR3	GetDestPos() { return m_vDestPos; }
		
		D3DXVECTOR3 GetSafePos() { return m_vSafePos; }
		void		SetSafePos( D3DXVECTOR3 vPos_ ) { m_vSafePos = vPos_; }

		void ProcessLockOn( CX2GUAra* myUnit );

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        void    AddRef()    {   ++m_uRefCount; }
        void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	};
	#pragma endregion 
		
	#pragma endregion Enum, Struct, Class

	#pragma region private
private:
	CX2GUAra( int iUnitIndex_, int iTeamNum_, IN lua_State* pScriptLuaState_, int iFrameBufferNum_, IN CX2Unit* pUnit_ );
	virtual ~CX2GUAra();

#ifdef RIDING_SYSTEM
	virtual bool CanRidingState();
#endif //RIDING_SYSTEM

	#pragma endregion function

	#pragma region protected
protected:
	virtual void	Init( bool bUseTeamPos_, int iStartPosIndex_ );
	virtual void	ParseCommonRandomState();
	virtual void	InitState();

	void			InitStateID();
	void			InitStateCommon();
	void			InitStateByUnitClass();
	virtual void	InitComponent();
	void			InitPromotionEquipList();

	virtual void	InitAquiredActiveSkillState( const int iSkillID_ );
	virtual void	InitPassiveSkillState();
	virtual void	SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, 
		const int iSlotIndex_, const bool bSlotB_ );
	virtual void	SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ );
	void			SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet, ARA_STATE_ID eStateID );
	void			SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ );

	virtual void	CommonStateStartFuture();
	virtual void	CommonStateStart();

	virtual void	CommonFrameMoveFuture();
	virtual void	CommonFrameMove();

	virtual void	CommonStateEndFuture();
	virtual void	CommonStateEnd();

	virtual RENDER_HINT	CommonRender_Prepare();

	virtual bool	SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition
		= CX2SkillTree::ASUT_GROUND );
	virtual void	NoStateChangeActionFrameMove();
	virtual float	GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

	void			LoadPromotionItemID( IN KLuaManager& luaManager_ );

	void			ShowActiveSkillCutInAndLightByBerserkMode( float fTimeToShow_, bool bOnlyLight_ = false );

	virtual CX2GageData*	CreateGageData();
	CX2AraGageData*			GetAraGageData() { return static_cast<CX2AraGageData*>( GetGageData() ); }
	const CX2AraGageData*	GetAraGageData() const { return static_cast<const CX2AraGageData*>( GetGageData() ); }

	void			StateChangeAirialDashIfPossible();

	__forceinline void	UpForcePower( int iUpCount_ );
	__forceinline bool	CanUseForcePower();
	__forceinline bool	IsEmptyForcePower();
	__forceinline bool	IsFullForcePower();

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	bool	UpForcePowerAtThisAnimationTime( float fAnimationTimeNeededToChargeForcePower_, int iUpCount_ );
	bool	UseForcePowerAtThisAnimationTime( float fAnimationTimeNeededToUseForcePower_, int iUpCount_ );
#else // SERV_ARA_CHANGE_CLASS_SECOND
	void	UpForcePowerAtThisAnimationTime( float fAnimationTimeNeededToChargeForcePower_, int iUpCount_ );
	void	UseForcePowerAtThisAnimationTime( float fAnimationTimeNeededToUseForcePower_, int iUpCount_ );
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	
	void	UseForcePower( int iCount_ );
	void	ConsumeForcePower( int iCount_ );

	void	SetForcePower( int iForcePower_ );
	void	SetMaxForcePower( int iMaxForcePower_ )  { GetAraGageData()->SetMaxForcePower( iMaxForcePower_ ); };

	UCHAR	GetBasicForcePower() const { return m_ucBasicForcePower; }
	void	SetBasicForcePower( UCHAR ucBasicForcePower_ ) { m_ucBasicForcePower = ucBasicForcePower_; }

	void	ResetForcePowerWhenRebirthInPVPGame( int iNumOfMyTeamKill_, int iNumOfEnmyTeamKill_ );

	inline void	ChangeAnimTail();

	bool	CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );
#ifdef FIX_SKILL_CANCEL_ARA
	void	StateChangeSpecialAttack( IN const CX2SkillTree::SKILL_ID eSkillId_ );
#else
	void	StateChangeSpecialAttack( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
#endif

	virtual	bool	ShouldResetNotBusyTimer();

	bool GetAddItemAndRemoveItemToViewListBerserk( OUT CX2EqipPtr& pEquipPtrToRemove_, IN OUT CX2Item** ppItemToAddInViewList_,
		IN CX2Item* const pItemToRemoveInViewList_, OUT int& iIndexInViewListForRemove_ );
	void		AddBerserkEquipToViewList( CX2Item* const pItem );
	void		RemoveBerserkEquipFromViewList( CX2EqipPtr pEquipPtrToRemove, int index	);

	void		MoveAraSafePosition()
	{
		D3DXVECTOR3 vPos = m_sLockOnSystemPtr->GetSafePos();
		vPos.y += LINE_RADIUS * 1.5f;
		m_FrameDataFuture.syncData.position = vPos;

		StateChange( ASI_SA_ALH_SWALLOW_ASSAULT_ESCAPE );
	}

	virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

#pragma endregion function

	#pragma region public
public:
	static CX2GUAra*	CreateGUAra( int iUnitIndex_, int iTeamNum_, IN lua_State* pScriptLuaState_, int iFrameBufferNum_,
		IN CX2Unit* pUnit_ );

	static void			AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );

	virtual void	ReInit( bool bTeamPos_ = false, int iStartPosIndex_ = -1 );
	virtual void	DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ );

	virtual void	Win();
	virtual void	Lose();
	virtual void	Draw();

	virtual void	InitEffect();
	virtual void	InitEnchantWeaponEffectUnitSpecific();
	
	void			InitializeAraMajorParticleArray();
	virtual void	CreateMajorParticleArray();
	static void		AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
	CKTDGParticleSystem::CParticleEventSequence* SetAraMajorParticleByEnum( ARA_MAJOR_PARTICLE_INSTANCE_ID eVal_,
																			wstring wstrParticleName_, int iDrawCount_ = -1 );
	__forceinline CKTDGParticleSystem::CParticleEventSequenceHandle GetHandleAraMajorParticleByEnum( ARA_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const;
	__forceinline void SetHandleAraMajorParticleByEnum( ARA_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ );
	void			DeleteAraMajorParticle();

	void			InitializeAraMinorParticleArray();
	virtual void	CreateMinorParticleArray();
	static void		AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
	CKTDGParticleSystem::CParticleEventSequence* SetAraMinorParticleByEnum( ARA_MINOR_PARTICLE_INSTANCE_ID eVal_, 
		wstring wstrParticleName_, int iDrawCount_ = -1 );
	__forceinline CKTDGParticleSystem::CParticleEventSequenceHandle GetHandleAraMinorParticleByEnum( ARA_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const;
	__forceinline void SetHandleAraMinorParticleByEnum( ARA_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ );
	void			DeleteAraMinorParticle();
	virtual	void	DeleteMinorParticle();

	void			InitializeMajorMeshArray();
	void			DeleteMajorMesh();
	CKTDGXMeshPlayer::CXMeshInstanceHandle GetHandleAraMajorMeshByEnum( ARA_MAJOR_MESH_INSTANCE_ID eVal_ ) const;
	void SetHandleAraMajorMeshByEnum( ARA_MAJOR_MESH_INSTANCE_ID eVal_, CKTDGXMeshPlayer::CXMeshInstanceHandle hHandle_ );

	virtual bool	SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
	virtual void	COMMON_BUFF_FrameMove();

	virtual void	NotifyShowObjectChanged();
	virtual void	NotifyDGManagerChanged( CKTDGManager& refInManager_ );
	void			SetShowPromotionEquips( bool bShow_ );
	virtual void	SetShowViewList( bool bShow_ );	
	void			SetShowOriginalViewList( bool bShow_ );
	void			ReplaceWeapon( CX2EqipPtr pWeaponPtr_ );
	virtual bool	UpdateEquipViewListInfo( CX2Item* pItemToAddInViewList, CX2Item* const pItemToRemoveInViewList );
	virtual void	UpdateEquipCollisionData();
	#ifdef FIX_ARA_ONE_PIECE_FOR_BERSERK
	virtual void UpdateNotRenderingOtherEquipList();
	#endif //FIX_ARA_ONE_PIECE_FOR_BERSERK

	virtual bool	IsFullHyperMode() const;
	virtual void	SetFullHyperMode( const bool bBerserkMode_ );

	virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );

	virtual void	ApplyHyperModeBuff();
	virtual void	HyperModeBuffEffectStart();
	virtual void	HyperModeBuffEffectEnd();

	virtual void	SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void	UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void	ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );
	virtual void	ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ );

	virtual void	ChargeMpAndEtcInTrainingGame();

	__forceinline int	GetForcePower();

	virtual void	DoProcessWhenKillEnemy( const CX2GameUnit* pDefender_ );

#ifdef ARA_CHANGE_CLASS_FIRST
	__forceinline int	GetMaxForcePower();

	float			GetBlockingRate();
	float			GetTurnigRate();

	virtual float GetReduceDebuffDurationRate(BUFF_TEMPLET_ID eBuffTempletID_);		/// 아라 1차 전직 소선 - 패시브 기력 활성화 : 디버프 유지 시간 감소
#endif

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
	float			GetSkillCoolTimeValue( const CX2SkillTree::SKILL_ID eSkillID_, const float fCoolTimeLeft_ );	/// 메모 등으로 인한 쿨타임 변경값 설정 함수
	USHORT			GetActualFPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const USHORT usForceGainValue_ );	/// 메모 등으로 인한 기력 소비량 설정 함수
	USHORT			GetActualFPGainCount( const CX2SkillTree::SKILL_ID eSkillID_, const USHORT usForceGainValue_ );	/// 메모 등으로 인한 기력 전환량 설정 함수
#endif // ADD_ARA_MEMO

	virtual int GetComboZStateID() const { return ASI_COMBO_Z; }
	virtual int GetComboXStateID() const { return ASI_COMBO_X; }

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	/// 기탄 데미지 이펙트 생성 함수
	void			CreateEnergeBullet( IN const float vRotZOffSet_ = 0.f, IN const bool bFireRightHand_ = true );
	/// 아라 2차 전직 명왕 - 회광반조 동작 처리
#ifdef FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE // 김태환
	bool			ProcessResurrection( IN const int iSkillLevel_ );
#else //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE
	void			SetResurrection( IN const int iSkillLevel_ );
#endif //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE
	/// 이혈공을 사용할 수 있는 상태인지 검사
	bool			CanUseConversionHealth( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN const int iSkillLevel_ );
	/// AttackResult에 추가 동작시킬 함수 설정
	void			SetAttackResultDelegate();
	/// 타격시 추가되는 기력량 설정
	void			UpNowForcePowerHitOthers( const CX2DamageManager::DamageData& damageData_ );
	/// 타격된 대상의 객체 포인터 저장
	void			AddHitUnitInfoByDamageData( const CX2DamageManager::DamageData& damageData_ );
	/// 데미지 처리시 추가로 관련 수치 설정하는 함수( GetFinalDamage )
	virtual void	ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, 
													 OUT float& fAttackerMagicDamage_, OUT float& fForceDownValue_ );
	///  MP 부족시, 헛타 생성
#ifndef SERV_9TH_NEW_CHARACTER // 김태환
	/// 다른 캐릭터들 전부 똑같은 함수를 쓰고 있으니, X2GUUser로 옮기자.
	virtual void		CreateNotEnoughMPEffect( D3DXVECTOR3 vPos, float fDegreeX, float fDegreeY, float fDegreeZ );
#endif // SERV_9TH_NEW_CHARACTER

	/// 스킬 슬롯 스테이트 아이디에 해당하는 스테이트 아이디 반환
	virtual void GetStateIDBySecretSkillTriggerStateID( IN OUT int& iStateID_ ) const;

	/// 해당 스킬이 스테이트가 없는 스킬인지 검사
	bool IsNoStageChangeSkill( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )  const;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

//#ifdef FIX_GAIN_FORCE_BUG
	virtual void		AttackResultByType( CX2DamageManager::DamageData &pDamageData );
//#endif // FIX_GAIN_FORCE_BUG

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	virtual const int GetHypetMpdeStateID();
#endif //SKILL_CANCEL_BY_HYPER_MODE

	#pragma endregion function

	#pragma region protected

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// FP Cancel 이 가능한지 여부, 종전 boolean 값 쓰던 부분에 Skill ID 체크하는 부분을 추가함
	bool GetAvailableFPCancel ( CX2SkillTree::SKILL_ID m_eSkillID ) const 
	{
		return ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID && CX2SkillTree::SI_A_ALD_REFLECTION != m_eSkillID );	
	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

protected:
	#pragma region state set
	void DieFrontStartFuture();
	void DieFrontStart();
	void DieFrontFrameMove();
	void DieFrontEventProcess();
	
	void DieBackStartFuture();
	void DieBackStart();
	void DieBackFrameMove();
	void DieBackEventProcess();
	#pragma endregion ASI_DIE_FRONT, ASI_DIE_BACK

	#pragma region state Set
	void WaitStartFuture();
	void WaitStart();
	void WaitEventProcess();

	void HyperWaitStartFuture();
	void HyperWaitStart();
	void HyperWaitEventProcess();
	void HyperWaitStateEnd();
	void PlayAnimationHyperWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ );

	void WalkEventProcess();

	void JumpReadyEventProcess();
	void JumpUpEventProcess();
	void JumpDownEventProcess();
	void JumpLandingStart();
	void JumpLandingEventProcess();
	#pragma endregion ASI_WAIT, WALK, JUMP_READY, JUMP_UP, JUMP_DOWN, JUMP_LANDING

	#pragma region state set
	void DashStart();
	void DashEventProcess();
	
	void DashEndStartFuture();
	void DashEndFrameMove();
	void DashEndEventProcess();

	void DashJumpStartFuture();
	void DashJumpStart();
	void DashJumpFrameMoveFuture();
	void DashJumpEventProcess();
	void DashJumpEndFuture();

	void DashJumpLandingEventProcess();

	void DashJumpPowerLandingStartFuture();
	void DashJumpPowerLandingStart();
	void DashJumpPowerLandingEventProcess();
	#pragma endregion ASI_DASH, DASH_END, DASH_JUMP, DASH_JUMP_LANDING, POWER_LANDING
			
	#pragma region state set
	void DamageSmallEventProcess();
	
	void DamageBigStart();
	void DamageBigEventProcess();

	void DamageDownFrontFrameMove();
	void DamageDownFrontEventProcess();
	
	void DamageDownBackFrameMove();
	void DamageDownBackEventProcess();

	void DamageAirDownLandingEventProcess();
	#pragma endregion DAMAGE

	#pragma region state set
	void HyperModeFrameMove();
#ifdef MODIFY_RIDING_PET_AWAKE
	void RidingHyperModeFrameMove();
	void CommonHyperModeFrameMove( float fTime1_, float fTime2_, float fTime3_, bool bSound_ = false );
#endif // MODIFY_RIDING_PET_AWAKE

	void StandUpRollingEventProcess();
	void StandUpRollingEnd();

	void StandUpAttackEventProcess();
	void StandUpAttackEnd();
	#pragma endregion ASI_HYPER_MODE, ASI_STANDUP_ROLLING, ASI_STANDUP_ATTACK

	#pragma region state set
	void ComboZEventProcess();
	void ComboZZEventProcess();
	void ComboZZZFrameMove();
	void ComboZZZEventProcess();
	void ComboZZZZEventProcess();
	void ComboZZZXStartEventProcess();
	void ComboZZZXChargeStateStart();
	void ComboZZZXChargeFrameMove();
	void ComboZZZXChargeEventProcess();
	void ComboZZZXChargeStateEnd();
	void ComboZZZXAttackFrameMove();
	void ComboZZZXAttackEventProcess();
	void ComboZZZXAttackStateEnd();
	void ComboZZXEventProcess();
	#pragma endregion COMBO_Z

	#pragma region state set
	void ComboXEventProcess();
	void ComboXXEventProcess();
	void ComboXXXStateStart();
	void ComboXXXEventProcess();
	void ComboXXXloopStateStart();
	void ComboXXXloopFrameMove();
	void ComboXXXloopEventProcess();
	void ComboXXXloopXStateStart();
	void ComboXXXloopXFrameMove();
	void ComboXXXloopXEventProcess();
	void ComboXXXloopXStateEnd();
	void ComboXXdownXFrameMove();
	void ComboXXdownXEventProcess();
	void ComboXXdownXXEventProcess();
	#pragma endregion COMBO_X

	#pragma region state set
	void ComboDashZFrameMove();
	void ComboDashZEventProcess();
	void ComboDashZpushX_ChargeStateStart();
	void ComboDashZpushX_ChargeFrameMove();
	void ComboDashZpushX_ChargeEventProcess();
	void ComboDashZpushX_ChargeStateEnd();
	void ComboDashZpushX_AttackFrameMove();
	void ComboDashZpushX_AttackEventProcess();
	void ComboDashZpushX_AttackStateEnd();
	void ComboDashXEventProcess();
	#pragma endregion DASH_COMBO

	#pragma region state set
	void JumpAttackZFrameMoveFuture();
	void JumpAttackZEventProcess();
	void JumpAttackXFrameMoveFuture();
	void JumpAttackXFrameMove();
	void JumpAttackXEventProcess();
	void DashJumpComboZEventProcess();
	void DashJumpComboZZEventProcess();
	void DashJumpComboZZZFrameMove();
	void DashJumpComboZZZEventProcess();
	void DashJumpComboXEventProcess();
	#pragma endregion JUMP_ATTACK

	#pragma region state set
	void AirialDashStartFuture();
	void AirialDashStart();
	void AirialDashFrameMoveFuture();
	void AirialDashFrameMove();
	void AirialDashEventProcess();
	void AirialDashEndFuture();
	void DoubleJumpStartFuture();
	void DoubleJumpFrontEventProcess();
	void DoubleJumpUpEventProcess();
	void DoubleJumpBackEventProcess();
	#pragma endregion special command

	#pragma region state skill set

	void ASI_SA_ARA_WIND_WEDGE_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ARA_WIND_WEDGE_START_Init();
	void ASI_SA_ARA_WIND_WEDGE_START_FrameMove();
	void ASI_SA_ARA_WIND_WEDGE_START_EventProcess();

	void ASI_SA_ARA_WIND_WEDGE_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ARA_WIND_WEDGE_END_Init();
	void ASI_SA_ARA_WIND_WEDGE_END_FrameMove();
	void ASI_SA_ARA_WIND_WEDGE_END_EventProcess();

	void ASI_SA_ARA_SHADOW_RISING_JUMP_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ARA_SHADOW_RISING_JUMP_Init();
	void ASI_SA_ARA_SHADOW_RISING_JUMP_FrameMove();
	void ASI_SA_ARA_SHADOW_RISING_JUMP_EventProcess();

	void ASI_SA_ARA_SHADOW_RISING_LANDING_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ARA_SHADOW_RISING_LANDING_Init();
	void ASI_SA_ARA_SHADOW_RISING_LANDING_FrameMove();
	void ASI_SA_ARA_SHADOW_RISING_LANDING_EventProcess();

	void ASI_SA_ARA_SHADOW_RISING_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ARA_SHADOW_RISING_END_Init();
	void ASI_SA_ARA_SHADOW_RISING_END_FrameMove();
	void ASI_SA_ARA_SHADOW_RISING_END_EventProcess();

	void ASI_SA_ARA_PULLING_PIERCE_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ARA_PULLING_PIERCE_START_Init();
	void ASI_SA_ARA_PULLING_PIERCE_START_FrameMove();
	void ASI_SA_ARA_PULLING_PIERCE_START_EventProcess();

	void ASI_SA_ARA_PULLING_PIERCE_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ARA_PULLING_PIERCE_END_Init();
	void ASI_SA_ARA_PULLING_PIERCE_END_FrameMove();
	void ASI_SA_ARA_PULLING_PIERCE_END_EventProcess();

	void ASI_SA_ARA_TEMPEST_DANCE_START_Init();
	void ASI_SA_ARA_TEMPEST_DANCE_START_FrameMove();
	void ASI_SA_ARA_TEMPEST_DANCE_START_EventProcess();
	void ASI_SA_ARA_TEMPEST_DANCE_END_Init();
	void ASI_SA_ARA_TEMPEST_DANCE_END_FrameMove();
	void ASI_SA_ARA_TEMPEST_DANCE_END_EventProcess();

	void ASI_SA_ARA_SHADOW_KNOT_START_Init();
	void ASI_SA_ARA_SHADOW_KNOT_START_StateStart();
	void ASI_SA_ARA_SHADOW_KNOT_START_FrameMove();
	void ASI_SA_ARA_SHADOW_KNOT_START_EventProcess();
	void ASI_SA_ARA_SHADOW_KNOT_WAIT_Init();
	void ASI_SA_ARA_SHADOW_KNOT_WAIT_FrameMove();
	void ASI_SA_ARA_SHADOW_KNOT_WAIT_EventProcess();
	void ASI_SA_ARA_SHADOW_KNOT_WAIT_StateEnd();
	void ASI_SA_ARA_SHADOW_KNOT_CATCH_Init();
	void ASI_SA_ARA_SHADOW_KNOT_CATCH_EventProcess();
	void ASI_SA_ARA_SHADOW_KNOT_END_Init();
	void ASI_SA_ARA_SHADOW_KNOT_END_FrameMove();
	void ASI_SA_ARA_SHADOW_KNOT_END_EventProcess();
	
	void ASI_SA_ARA_FALLING_DRAGON_Init();
	void ASI_SA_ARA_FALLING_DRAGON_FrameMove();
	void ASI_SA_ARA_FALLING_DRAGON_EventProcess();

	void ASI_A_ARA_TIGER_CLAW_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ARA_TIGER_CLAW_START_Init();
	void ASI_A_ARA_TIGER_CLAW_START_StateStart();
	void ASI_A_ARA_TIGER_CLAW_START_FrameMove();
	void ASI_A_ARA_TIGER_CLAW_START_EventProcess();
	void ASI_A_ARA_TIGER_CLAW_START_StateEnd();

	void ASI_A_ARA_TIGER_CLAW_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ARA_TIGER_CLAW_END_Init();
	void ASI_A_ARA_TIGER_CLAW_END_FrameMove();
	void ASI_A_ARA_TIGER_CLAW_END_EventProcess();

	void ASI_A_ARA_FORCE_SPEAR_Init();
	void ASI_A_ARA_FORCE_SPEAR_FrameMove();
	void ASI_A_ARA_FORCE_SPEAR_EventProcess();

	void ASI_A_ARA_GAIN_FORCE_Init();
	void ASI_A_ARA_GAIN_FORCE_FrameMove();
	void ASI_A_ARA_GAIN_FORCE_EventProcess();

	#pragma region Little Hsien
	void ASI_SA_ALH_MOONLIGHT_SLASH_READY_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALH_MOONLIGHT_SLASH_READY_Init();
	void ASI_SA_ALH_MOONLIGHT_SLASH_READY_StateStart();
	void ASI_SA_ALH_MOONLIGHT_SLASH_READY_StateStartFuture();
	void ASI_SA_ALH_MOONLIGHT_SLASH_READY_FrameMove();
	void ASI_SA_ALH_MOONLIGHT_SLASH_READY_EventProcess();
	
	void ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_Init();
	void ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_FrameMove();
	void ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_EventProcess();
	
	void ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_Init();
	void ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateStart();
	void ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_FrameMove();
	void ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateEnd();
	void ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_EventProcess();

	void ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_Init();
	void ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateStart();
	void ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_FrameMove();
	void ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateEnd();
	void ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_EventProcess();

	void ASI_SA_ALH_SWALLOW_ASSAULT_Ready_Init();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateStart();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Ready_FrameMove();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateEnd();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Ready_EventProcess();

	void ASI_SA_ALH_SWALLOW_ASSAULT_Attack_Init();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateStart();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Attack_FrameMove();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Attack_EventProcess();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateEnd();

	void ASI_SA_ALH_SWALLOW_ASSAULT_Finish_Init();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Finish_StateStart();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Finish_FrameMove();
	void ASI_SA_ALH_SWALLOW_ASSAULT_Finish_EventProcess();
	
	void ASI_SA_ALH_SWALLOW_ASSAULT_Escape_EventProcess();

	void ASI_A_ALH_ROCK_CRASH_FALLING_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ALH_ROCK_CRASH_START_Init();
	void ASI_A_ALH_ROCK_CRASH_START_StateStart();
	void ASI_A_ALH_ROCK_CRASH_START_FrameMove();
	void ASI_A_ALH_ROCK_CRASH_START_EventProcess();
	void ASI_A_ALH_ROCK_CRASH_START_StateEnd();

	void ASI_A_ALH_ROCK_CRASH_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ALH_ROCK_CRASH_END_Init();
	void ASI_A_ALH_ROCK_CRASH_END_FrameMove();
	void ASI_A_ALH_ROCK_CRASH_END_EventProcess();
	#pragma endregion 1-1

	#pragma region SAKRA_DEVANAM
	void ASI_A_ASD_LOW_BRANDISH_SLIDING_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ASD_LOW_BRANDISH_SLIDING_Init();
	void ASI_A_ASD_LOW_BRANDISH_SLIDING_StateStart();
	void ASI_A_ASD_LOW_BRANDISH_SLIDING_FrameMove();
	void ASI_A_ASD_LOW_BRANDISH_SLIDING_EventProcess();
	void ASI_A_ASD_LOW_BRANDISH_SLIDING_StateEnd();

	void ASI_A_ASD_LOW_BRANDISH_KICK_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ASD_LOW_BRANDISH_KICK_Init();
	void ASI_A_ASD_LOW_BRANDISH_KICK_FrameMove();
	void ASI_A_ASD_LOW_BRANDISH_KICK_EventProcess();

	void ASI_P_ASD_QUICKEN_GUARD_StateStart();
	void ASI_P_ASD_QUICKEN_GUARD_EventProcess();

	void ASI_SA_ASD_VOID_BREAK_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ASD_VOID_BREAK_Init();
	void ASI_SA_ASD_VOID_BREAK_FrameMove();
	void ASI_SA_ASD_VOID_BREAK_EventProcess();
	#pragma endregion 1-2

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	#pragma region 스킬 개편
	void ASI_A_ARA_Quick_Piercing_EventProcess();	// 천격
	void ASI_A_ARA_Quick_Piercing_FrameMove();

	void ASI_A_ARA_Shoulder_Blow_Init();			// 철산고
	void ASI_A_ALH_Shoulder_Blow_FrameMove();
	void ASI_A_ALH_Shoulder_Blow_EventProcess();

	void ASI_A_ASD_Falling_Blossom_Start_Init();			// 낙화
	void ASI_A_ASD_Falling_Blossom_Start_StateStart();
	void ASI_A_ASD_Falling_Blossom_Start_EventProcess();

	void ASI_A_ASD_Falling_Blossom_Finish_Init();			// 낙화
	void ASI_A_ASD_Falling_Blossom_Finish_EventProcess();

	void ASI_A_ARA_Quick_Stab_Init();				// 신속
	void ASI_A_ARA_Quick_Stab_FrameMove();
	void ASI_A_ARA_Quick_Stab_EventProcess();
	#pragma endregion 신속, 철산고, 용추, 천격, 만월베기, 맹호의 힘, 호신강기
#endif //UPGRADE_SKILL_SYSTEM_2013
	
	#pragma endregion passive, active, special active skill
	
#ifdef ARA_CHANGE_CLASS_FIRST
	void ASI_COMBO_ALH_ZZXX_EventProcess();					//COMBO_ZZXX
	void ASI_COMBO_ALH_ZZXX_LANDING_EventProcess();			//COMBO_ZZXX
	void ASI_DASH_COMBO_ALH_ZZ_EventProcess();				//DASH_COMBO_ZZ
	void ASI_JUMP_ATTACK_ALH_ZZ_EventProcess();				//JUMP_ATTACK_ZZ

	void ASI_DASH_JUMP_COMBO_ASD_ZX_EventProcess();			//DASH_JUMP_COMBO_ZX
	void ASI_DOUBLE_JUMP_FRONT_COMBO_ASD_Z_BACK_EventProcess();//DOUBLE_JUMP_FRONT_COMBO_Z_BACK

	void ASI_A_ALH_BREAKING_Init();							//이화접목 대기
	void ASI_A_ALH_BREAKING_FrameMove();
	void ASI_A_ALH_BREAKING_EventProcess();
	void ASI_A_ALH_BREAKING_ATTACK_Init();					//이화접목 공격
	void ASI_A_ALH_BREAKING_ATTACK_FrameMove();
	void ASI_A_ALH_BREAKING_ATTACK_EventProcess();
	void ASI_A_ALH_BREAKING_ATTACK_EndFuture();

	void ASI_SA_ALH_DOUBLE_COLLISION_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALH_DOUBLE_COLLISION_START_Init();
	void ASI_SA_ALH_DOUBLE_COLLISION_START_Start();
	void ASI_SA_ALH_DOUBLE_COLLISION_START_FrameMove();
	void ASI_SA_ALH_DOUBLE_COLLISION_START_EventProcess();

	void ASI_SA_ALH_DOUBLE_COLLISION_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALH_DOUBLE_COLLISION_END_FrameMove();
	void ASI_SA_ALH_DOUBLE_COLLISION_END_EventProcess();

	void ASI_SA_ASD_TURBULENT_WAVE_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ );			// 맹호 4식 - 노도
	void ASI_SA_ASD_TURBULENT_WAVE_START_Init();			// 맹호 4식 - 노도
	void ASI_SA_ASD_TURBULENT_WAVE_START_Start();
	void ASI_SA_ASD_TURBULENT_WAVE_START_FrameMove();
	void ASI_SA_ASD_TURBULENT_WAVE_START_End();
	void ASI_SA_ASD_TURBULENT_WAVE_START_EventProcess();

	void ASI_SA_ASD_TURBULENT_WAVE_LOOP_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ASD_TURBULENT_WAVE_LOOP_FrameMove();
	void ASI_SA_ASD_TURBULENT_WAVE_LOOP_EventProcess();
	void ASI_SA_ASD_TURBULENT_WAVE_LOOP_End();

	void ASI_SA_ASD_TURBULENT_WAVE_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ASD_TURBULENT_WAVE_END_Init();
	void ASI_SA_ASD_TURBULENT_WAVE_END_FrameMove();
	void ASI_SA_ASD_TURBULENT_WAVE_END_EventProcess();

	void ASI_SA_ASD_SUPPRESSION_Init();						// 제압
	void ASI_SA_ASD_SUPPRESSION_StateStart();
	void ASI_SA_ASD_SUPPRESSION_FrameMove();
	void ASI_SA_ASD_SUPPRESSION_EventProcess();

	void ASI_SA_ALH_POWER_OF_WHITER_TIGER_Init();			/// 맹호격
	void ASI_SA_ALH_POWER_OF_WHITER_TIGER_FrameMove();
	void ASI_SA_ALH_POWER_OF_WHITER_TIGER_EventProcess();
	void ASI_SA_ALH_POWER_OF_WHITER_TIGER_End();
	void ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING_FrameMove();
	void ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING_EventProcess();

	void ASI_SA_ALH_PEERLESSNESS_SLASH_START_Init();			/// 무쌍참
	void ASI_SA_ALH_PEERLESSNESS_SLASH_START_Start();
	void ASI_SA_ALH_PEERLESSNESS_SLASH_START_FrameMove();
	void ASI_SA_ALH_PEERLESSNESS_SLASH_START_EventProcess();
	void ASI_SA_ALH_PEERLESSNESS_SLASH_START_End();
	void ASI_SA_ALH_PEERLESSNESS_SLASH_END_FrameMove();
	void ASI_SA_ALH_PEERLESSNESS_SLASH_END_EventProcess();

	void ASI_SA_ALH_DESCENT_DRAGON_CreateState( UserUnitStateData& stateData_, const int iStateId_ );			/// 용아 최종오의 - 폭쇄
	void ASI_SA_ALH_DESCENT_DRAGON_Init();			/// 용아 최종오의 - 폭쇄
	void ASI_SA_ALH_DESCENT_DRAGON_FrameMove();
	void ASI_SA_ALH_DESCENT_DRAGON_EventProcess();

	void SetInvisibility(bool bVal_);
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

	#pragma region 2-1, 2-2 Combo State
	/// DASH_JUMP_COMBO_ZX
	void ASI_DASH_JUMP_COMBO_ALD_ZX_FrameMove();
	void ASI_DASH_JUMP_COMBO_ALD_ZX_EventProcess();

	/// DASH_JUMP_COMBO_ALD_ZXX
	void ASI_DASH_JUMP_COMBO_ALD_ZXX_FrameMove();
	void ASI_DASH_JUMP_COMBO_ALD_ZXX_EventProcess();

	/// COMBO_ZZXX
	void ASI_COMBO_ALD_ZZXX_FrameMove();
	void ASI_COMBO_ALD_ZZXX_EventProcess();

	/// COMBO_ZZZFrontZ
	void ASI_COMBO_ALD_ZZZFrontZ_FrameMove();
	void ASI_COMBO_ALD_ZZZFrontZ_EventProcess();

	/// DASH_JUMP_COMBO_XZ
	void ASI_DASH_JUMP_COMBO_AYR_XZ_StateStartFuture();
	void ASI_DASH_JUMP_COMBO_AYR_XZ_FrameMove();
	void ASI_DASH_JUMP_COMBO_AYR_XZ_EventProcess();

	/// DASH_COMBO_ZZX
	void ASI_DASH_COMBO_AYR_ZZ_FrameMove();
	void ASI_DASH_COMBO_AYR_ZZ_EventProcess();
	#pragma endregion 2-1, 2-2 커맨드 스킬

	#pragma region 2-1, 2-2 Active
	/// 낭아 3식 - 늑대발톱
	void ASI_A_ALD_WOLF_CLAW_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ALD_WOLF_CLAW_Init();
	void ASI_A_ALD_WOLF_CLAW_StateStart();
	void ASI_A_ALD_WOLF_CLAW_EventProcess();
	void ASI_A_ALD_WOLF_CLAW_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ALD_WOLF_CLAW_FINISH_Init();
	void ASI_A_ALD_WOLF_CLAW_FINISH_EventProcess();

	/// 이혈공
	void ASI_A_ALD_CONVERSION_HEALTH_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ALD_CONVERSION_HEALTH_Init();
	void ASI_A_ALD_CONVERSION_HEALTH_FrameMove();
	void ASI_A_ALD_CONVERSION_HEALTH_EventProcess();

	/// 이기공
	void ASI_A_ALD_CONVERSION_MANA_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_ALD_CONVERSION_MANA_Init();
	void ASI_A_ALD_CONVERSION_MANA_FrameMove();
	void ASI_A_ALD_CONVERSION_MANA_EventProcess();

	/// 쾌속
	void ASI_A_AYR_HIGH_SPEED_READY_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_AYR_HIGH_SPEED_READY_EventProcess();
	void ASI_A_AYR_HIGH_SPEED_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_AYR_HIGH_SPEED_Init();
	void ASI_A_AYR_HIGH_SPEED_StateStartFuture();
	void ASI_A_AYR_HIGH_SPEED_EventProcess();
	void ASI_A_AYR_HIGH_SPEED_StateEndFuture();
	#pragma endregion 2-1, 2-2 액티브 스킬

	#pragma region 2-1, 2-2 Special Active
	/// 기공파
	void ASI_SA_ALD_ENERGY_WAVE_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_ENERGY_WAVE_Init();
	void ASI_SA_ALD_ENERGY_WAVE_EventProcess();

	/// 연환지옥
	void ASI_SA_ALD_HELL_OF_TORNADO_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_HELL_OF_TORNADO_Init();
	void ASI_SA_ALD_HELL_OF_TORNADO_StateStart();
	void ASI_SA_ALD_HELL_OF_TORNADO_EventProcess();
	void ASI_SA_ALD_HELL_OF_TORNADO_FINISH_CreateState(  UserUnitStateData& stateData_, const int iStateId_  );
	void ASI_SA_ALD_HELL_OF_TORNADO_FINISH_Init();
	void ASI_SA_ALD_HELL_OF_TORNADO_FINISH_EventProcess();

	/// 낭아 4식 - 늑대이빨
	void ASI_SA_ALD_WOLF_SPEAR_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_WOLF_SPEAR_Init();
	void ASI_SA_ALD_WOLF_SPEAR_StateStart();
	void ASI_SA_ALD_WOLF_SPEAR_EventProcess();
	void ASI_SA_ALD_WOLF_SPEAR_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_WOLF_SPEAR_FINISH_Init();
	void ASI_SA_ALD_WOLF_SPEAR_FINISH_EventProcess();

	/// 나찰 1식 - 흡혼
	void ASI_SA_ALD_FINGER_BULLET_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_FINGER_BULLET_Init();
	void ASI_SA_ALD_FINGER_BULLET_StateStart();
	void ASI_SA_ALD_FINGER_BULLET_EventProcess();
	void ASI_SA_ALD_FINGER_BULLET_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_FINGER_BULLET_FINISH_Init();
	void ASI_SA_ALD_FINGER_BULLET_FINISH_FrameMove();
	void ASI_SA_ALD_FINGER_BULLET_FINISH_EventProcess();

	/// 나찰 2식 - 연환장
	void ASI_SA_ALD_ENERGY_BEAD_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_ENERGY_BEAD_Init();
	void ASI_SA_ALD_ENERGY_BEAD_StateStart();
	void ASI_SA_ALD_ENERGY_BEAD_EventProcess();
	void ASI_SA_ALD_ENERGY_BEAD_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_ENERGY_BEAD_FINISH_Init();
	void ASI_SA_ALD_ENERGY_BEAD_FINISH_EventProcess();

	/// 나찰 3식 - 창살감옥
	void ASI_SA_ALD_PRISON_SPEAR_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_PRISON_SPEAR_Init();
	void ASI_SA_ALD_PRISON_SPEAR_StateStart();
	void ASI_SA_ALD_PRISON_SPEAR_EventProcess();
	void ASI_SA_ALD_PRISON_SPEAR_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_PRISON_SPEAR_FINISH_Init();
	void ASI_SA_ALD_PRISON_SPEAR_FINISH_EventProcess();

	/// 나찰 4식 - 탈혼
	void ASI_A_AYR_STEAL_SOUL_CreateState( OUT UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_AYR_STEAL_SOUL_Init();
	void ASI_A_AYR_STEAL_SOUL_StateStart();
	void ASI_A_AYR_STEAL_SOUL_EventProcess();
	void ASI_A_AYR_STEAL_SOUL_FINISH_CreateState( OUT UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_A_AYR_STEAL_SOUL_FINISH_Init();
	void ASI_A_AYR_STEAL_SOUL_FINISH_FrameMove();
	void ASI_A_AYR_STEAL_SOUL_FINISH_EventProcess();

	/// 초진공장
	void ASI_SA_AYR_ENERGY_VOID_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_AYR_ENERGY_VOID_START_Init();
	void ASI_SA_AYR_ENERGY_VOID_START_StateStartFuture();
	void ASI_SA_AYR_ENERGY_VOID_START_StateStart();
	void ASI_SA_AYR_ENERGY_VOID_START_EventProcess();
	
	void ASI_SA_AYR_ENERGY_VOID_LOOP_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_AYR_ENERGY_VOID_LOOP_Init();
	void ASI_SA_AYR_ENERGY_VOID_LOOP_StateStart();
	void ASI_SA_AYR_ENERGY_VOID_LOOP_EventProcess();

	void ASI_SA_AYR_ENERGY_VOID_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_AYR_ENERGY_VOID_END_Init();
	void ASI_SA_AYR_ENERGY_VOID_END_StateStart();
	void ASI_SA_AYR_ENERGY_VOID_END_FrameMove();
	void ASI_SA_AYR_ENERGY_VOID_END_EventProcess();

	/// 제압 - 기
	void ASI_SA_AYR_SUPPRESSION_ENERGY_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_AYR_SUPPRESSION_ENERGY_Init();
	void ASI_SA_AYR_SUPPRESSION_ENERGY_EventProcess();

	/// 낭아 최종오의 - 극쇄
	void ASI_SA_ALD_WOLF_BREAK_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_ALD_WOLF_BREAK_Init();
	void ASI_SA_ALD_WOLF_BREAK_StateStart();
	void ASI_SA_ALD_WOLF_BREAK_FrameMove();
	void ASI_SA_ALD_WOLF_BREAK_EventProcess();

	/// 나찰 최종오의 - 연옥
	void ASI_SA_AYR_OPEN_THE_HELL_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_SA_AYR_OPEN_THE_HELL_Init();
	void ASI_SA_AYR_OPEN_THE_HELL_FrameMove();
	void ASI_SA_AYR_OPEN_THE_HELL_EventProcess();

	/// 대규환지옥
	void ASI_HA_AYR_ENERGY_CANNON_CreateState( UserUnitStateData& stateData_, const int iStateId_ );
	void ASI_HA_AYR_ENERGY_CANNON_Init();
	void ASI_HA_AYR_ENERGY_CANNON_EventProcess();
	#pragma endregion 2-1, 2-2 스페셜 액티브 스킬

#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FINALITY_SKILL_SYSTEM //김창한
	//아라 제천 궁극기 - 분기 등천
	void ASI_HA_ASD_THOUSANDS_BLOSSOMS_Init();
	void ASI_HA_ASD_THOUSANDS_BLOSSOMS_EventProcess();
#endif //FINALITY_SKILL_SYSTEM	#pragma endregion state function

	#pragma region protected
protected:
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_ahAraMajorParticleInstance[ARA_MAJOR_PII_END];	/// 메이저 파티클 핸들
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_ahAraMinorParticleInstance[ARA_MINOR_PII_END];	/// 마이너 파티클 핸들
	CKTDGXMeshPlayer::CXMeshInstanceHandle				m_ahAraMajorMeshInstance[ARA_MAJOR_MII_END];		/// 메이저 메쉬 핸들

	vector<CX2EqipPtr>	m_vecPromotionEquipList;		/// 외형 장비 리스트로 각성 구슬 3개를 소모한 각성을 했을때 보여진다.
	CX2EqipPtr			m_ptrPromotionEquipTail;		/// 아라 각성시 나오는 구미호 꼬리

	vector<int>			m_vecPromotionItemIDList;		/// 프로모션 장비의 ID 리스트
	vector<int>			m_vecPromotionItemIDList2;		/// 
	bool				m_bBerserkMode;					/// 광폭화 상태

	const int	MAX_ANIMATION_LOOP_COUNT_AT_THIS_STATE;	/// 현재 상태에서 최대로 Loop 될 수있는 애니메이션 횟수

	float		m_fDamageDataChangeTime;	/// DamageDate 변경 시간 1
	float		m_fDamageDataChangeTime2;	/// DamageData 변경 시간 2
	
	USHORT		m_uiComboXXXLoopCount;		/// XXXLoop 콤보 카운트 ( 최대 3회 )
	float		m_fZpushXChargeTime;		/// Dash_Combo_ZpushX_Charge 시간( 최대 2.0초 )
	float		m_fAirialDashTime;			/// 능공허도 시간( 이후 패시브로 증가될 수 있음 )
	USHORT		m_usArialCount;				/// 능공허도 횟수
	USHORT		m_usDoubleJumpCount;		/// 이단점프 횟수

	UCHAR		m_ucBasicForcePower;		/// 기본 기력치
	CForcePowerSystem	m_ForcePowerSystem;	/// 기력 시스템
	D3DXVECTOR2	m_vFPCancel;				/// 기력 취소 시간
	bool		m_bFPCancel;				/// 기력으로 취소
	USHORT		m_usForceGainCount;			/// 스킬 캔슬을 통한 기력 획득량
	USHORT		m_usBeforeForceGainCount;	/// m_usForceGainCount이 캔슬한 스킬로 갱신되는 문제로 이전 값 저장
	
	const CX2SkillTree::SkillTemplet*	m_pSkillTemplet;	/// 스킬 템플릿 포인터
	int		m_iPressedSkillSlotIndex;
	float	m_fMPConsume;

	float	m_fShadowKnotTime;								/// 그림자 매듭 Wait 타임
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffectHandle	    m_hShadowKnotWeaponMesh;	/// 그림자 매듭 무기
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect*	m_pShadowKnotWeaponMesh;	/// 그림자 매듭 무기
//#ifndef FIX_GAIN_FORCE_BUG
//	CX2DamageEffect::CEffect*	m_pGainForceDamaggeEffect;	/// 흡공에 의한 타격을 확인하기 위함
//#endif	FIX_GAIN_FORCE_BUG	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	float	m_fForceSpearDamageRel;		/// 기공창 숙련 패시브 데미지 증가량
	USHORT	m_usForceSpearPiercing;		/// 기공창 숙련 패시브 관통 횟수
#endif //UPGRADE_SKILL_SYSTEM_2013
	
	float		m_fZZZXChargeTime;				/// ZZZXChargeTime 시간( 최대 2.0초 )

	CX2EffectSet::Handle	m_hPullinPierce;	/// 빨래 널기 핸들 삭제용

	float		m_fPullingDragonPowerRate;		/// 강룡추 공격력

#ifdef ARA_CHANGE_CLASS_FIRST
	bool		m_bAbleQuickStand;				/// 재빠른 기상 습득 여부
	int			m_iGainMpByFpConversion;		/// 기력 대비 얻을 수 있는 MP량(패시브 : 충만한 힘)
	float		m_fPreRenderParamColorA;		/// 무쌍참으로 렌더 끄기 전 임시 저장하는 기존 알파값		
	float		m_fRenderParamColorA;			/// 무쌍참으로 렌더 끌 때 페이드 아웃 시키기 위한 알파값

	float	m_fBlockingRate;					/// 1-2차 번득이는 기지 막기 비율
	float	m_fTurningRate;						/// 1-2차 번득이는 기지 뒤 돌아맞기 비율

	bool				m_bBrutalPiercing;				/// 창술의 극의 스킬 여부
	CX2BuffFactorPtr	m_pBrutalPiercingBuffFactor;	/// 창술의 극의 디버프 Buff Factor

	float	m_fReduceEDTDurationRate;			/// 아라 1차 전직 소선 - 패시브 기력 활성화 : 디버프 시간 감소율

	sMoonRightSlashDataPtr	m_sMoonRightSlashDataPtr;		/// 달빛베기 데이터

	CX2EffectSet::Handle	m_hTempEffectSet;	/// 이펙트셋 끄는 시점을 컨트롤하기 위한 임시 핸들
	
    sLockOnSystemPtr                m_sLockOnSystemPtr;		/// 추적 시스템 Smart Ptr

#ifdef	VERIFY_STAT_BY_BUFF
	PROTECT_VECTOR3				m_vPreTransScale;	/// 무쌍참으로 유저 스케일 변경 시킬 때, 버프에 의한 스케일 변경 값 저장
#else	// VERIFY_STAT_BY_BUFF
	D3DXVECTOR3					m_vPreTransScale;	/// 무쌍참으로 유저 스케일 변경 시킬 때, 버프에 의한 스케일 변경 값 저장
#endif	// VERIFY_STAT_BY_BUFF

#endif

#ifdef FIX_SKILL_CANCEL_ARA
	CX2SkillTree::SKILL_ID	m_eSkillID;		/// 스킬 아이디
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	float	m_fCurrentSpeed;	/// 보름달 베기 충전 시간 감소치
	bool	m_bMoonSlash;		/// 만월 베기 패시브
	float	m_fSuppresionPowerRate;		/// 제압 공격력
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	float		m_fUseForcePowerTime;						/// 기력 소모 되는 시간
	int			m_iUseForcePowerCount;						/// 기력 소모량

	float		m_fAddForcePowerTime;						/// 기력 증가 되는 시간
	int			m_iAddForcePowerCount;						/// 기력 증가량

	float		m_fIncreaseEnergeBulletSize;				/// 아라 1차 전직 소마 - 기탄 강화를 통해 증가되는 기탄 크기
	float		m_fIncreaseEnergeBulletDamage;				/// 아라 1차 전직 소마 - 기탄 강화를 통해 증가되는 기탄 공격력

	float		m_fIncreaseEnergeBulletRange;				/// 아라 1차 전직 소마 - 기력 강화를 통해 증가되는 기탄 사거리
	float		m_fIncreaseEnergeBulletForceDownValue;		/// 아라 1차 전직 소마 - 기력 강화를 통해 증가되는 다운 수치 감소량

	std::vector<CX2GameUnitoPtr> m_vecHitUnitPointer;		/// 아라 1차 전직 소마 - 흡공에 피격된 유닛 포인터 저장 객체

	float		m_fResurrectionLimitHPValue;				/// 아라 2차 전직 명왕 - 회광반조 제한 체력 수치
	float		m_fResurrectionCoolTime;					/// 아라 2차 전직 명왕 - 회광반조 재발동 시간

	float		m_fHighSpeedValue;							/// 아라 2차 전직 명왕 - 쾌속 진행 시간

	int			m_iEnergeVoidChargeValue;					/// 아라 2차 전직 명왕 - 초진공장 차지 단계
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle   m_hEnergeVoidDamageEffect;	/// 아라 2차 전직 명왕 - 초진공장 공격 이펙트
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect* m_pEnergeVoidDamageEffect;	/// 아라 2차 전직 명왕 - 초진공장 공격 이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	bool		m_bEnergeVoidChargeEnd;						/// 아라 2차 전직 명왕 - 초진공장 차지 종료 여부

	float		m_fHowlingDamageRel;						/// 늑대의 울부짖음 데미지 배율
	float		m_fHowlingActiveRate;						/// 늑대의 울부짖음 발동 확률

	float		m_fOpenTheHellIncreaseDamage;				/// 지옥 초래를 통한 연옥 데미지 증가율

	int		m_iWolfBreakeCount;								/// 낭아 최종오의 - 극쇄 발사 카운트
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	#pragma endregion variable

//#ifdef FIX_GAIN_FORCE_BUG
	CX2DamageEffect::CEffectHandle m_hGainForceIntakeEffect;	// 흡공 기력 회수용 데미지 이펙트의 핸들
	int		m_iGainForceDamageTimeNow;	// 흡공 기력 회복을 위한 변수,(타격 횟수 저장)
//#endif // FIX_GAIN_FORCE_BUG
};
#pragma endregion 클래스

IMPLEMENT_INTRUSIVE_PTR( CX2GUAra::SLockOnSystem );
IMPLEMENT_INTRUSIVE_PTR( CX2GUAra::sMoonLightSlashData );

#endif

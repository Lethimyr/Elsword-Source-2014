#pragma once

//{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG

/** @class : CX2GUChung
	@brief : 2010-12-23 에 패치될 캐릭터 청 클래스
	@date  : 2010/11/18
*/
class CX2GUChung : public CX2GUUser
{
public:

	enum CHUNG_STATE_ID
	{
		CSI_BASE = USI_END	+ 1,			/// 청만의 스테이트 중 시작 ID
		
		//CSI_POWER_DOWN,			/// 점프 상태에서 DOWN 화살표를 눌렀을 때
		CSI_POWER_DOWN_LANDING,		/// 점프 상태에서 DOWN 화살표에 의해서 착지 했을 때의 스테이트

		CSI_STANDUP_ROLLING_FRONT_FRONT,
		CSI_STANDUP_ROLLING_FRONT_BACK,
		CSI_STANDUP_ROLLING_BACK_FRONT,
		CSI_STANDUP_ROLLING_BACK_BACK,

		CSI_STANDUP_ATTACK_FRONT,
		CSI_STANDUP_ATTACK_BACK,

		CSI_WALL_LANDING,

		CSI_CHARGE_CANNON_BALL,				/// 캐논볼을 모으는 모션
		
		CSI_GUARD,
		CSI_GUARD_ATTACK_Z,
		CSI_GUARD_ATTACK_X,

		CSI_COMBO_Z,
		CSI_COMBO_ZZ,
		CSI_COMBO_ZZZ,

		CSI_COMBO_ZZZZ,

		CSI_COMBO_ZZZX_READY,
		CSI_COMBO_ZZZX_CANNON_BALL_SHOT,	/// 캐논볼이 2개 이상있을 때 발사하는 모션 (1개있을때는 실행되지 않고 바로 finish가 실행됨)
		CSI_COMBO_ZZZX_FINISH,				/// 캐논볼 1개 이상 있을 때 발사 하는 마지막 모션

		CSI_COMBO_ZZdownZ,
		CSI_COMBO_ZZdownZZ,

		CSI_COMBO_X,
		CSI_COMBO_XX,
		CSI_COMBO_XXX,
		
		CSI_COMBO_XXZ,

		CSI_COMBO_XXZX_CANNON_BALL_SHOT,	/// 캐논볼이 2개 이상있을 때 발사하는 모션 (1개있을때는 실행되지 않고 바로 finish가 실행됨)
		CSI_COMBO_XXZX_FINISH,				/// 캐논볼 1개 이상 있을 때 발사 하는 마지막 모션

		CSI_COMBO_DASH_Z,

		CSI_ATTACK_DASH_X,

		CSI_ATTACK_JUMP_Z,
		CSI_ATTACK_JUMP_X,

		CSI_COMBO_DASH_JUMP_Z,
		CSI_COMBO_DASH_JUMP_ZZ,
		CSI_COMBO_DASH_JUMP_ZZLanding,
		CSI_COMBO_DASH_JUMP_ZZLanding_CANNON_BALL_SHOT,
		
		CSI_ATTACK_DASH_JUMP_X,

		CSI_STANDUP_CANNON_ATTACK_FRONT,
		CSI_STANDUP_CANNON_ATTACK_BACK,

		//{{ kimhc // 2011.1.4 // 청 1차 전직
		// 퓨리가디언(FG)
		CSI_CFG_COMBO_ZZX,
		CSI_CFG_COMBO_DASH_ZZ,
		CSI_CFG_COMBO_DASH_ZZZ,
		CSI_CFG_COMBO_DASH_JUMP_ZX,
		CSI_CFG_COMBO_DASH_JUMP_ZXX,
		CSI_CFG_COMBO_DASH_JUMP_ZXXLanding,
		//}} kimhc // 2011.1.4 // 청 1차 전직

		///////////////////////////////////////////////////////
		//{{ 장훈 : 2011-01-12 : 청 1차 전직 슈팅가디언
		CSI_CSG_COMBO_XFRONTX_PULL_SUBWEAPON,
		CSI_CSG_COMBO_XFRONTX_FIRE,
		CSI_CSG_COMBO_XFRONTXX,

		CSI_CSG_COMBO_DASH_XX,
		CSI_CSG_COMBO_DASH_XXX,

		CSI_CSG_COMBO_DASH_JUMP_X,
		CSI_CSG_COMBO_DASH_JUMP_XX,
		CSI_CSG_COMBO_DASH_JUMP_XX_Landing,

		//}} 장훈 : 2011-01-12 : 청 1차 전직 슈팅가디언
		///////////////////////////////////////////////////////

		//{{ JHKang / 강정훈 / 2011.6.14 / 청 2차 전직 데들리 체이서
#ifdef CHUNG_SECOND_CLASS_CHANGE
		CSI_CDC_COMBO_X,
		CSI_CDC_COMBO_XX,
		
		CSI_CDC_COMBO_XZ,
		CSI_CDC_COMBO_XZZ,

		CSI_CDC_COMBO_ZZX,
		CSI_CDC_COMBO_ZZXX,
		CSI_CDC_COMBO_ZZXXX,
#endif
		//}}

		///////////////////////// 스킬 ////////////////////////////////////////////////////
		
		// 디토네이션
		CSI_SI_SA_CHUNG_DETONATION_LOOP,
		CSI_SI_SA_CHUNG_DETONATION_END,
		// 루나틱 블로우
		CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION,
		CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION_FINISH,

		//{{장훈 : 2010-12-15 : Gigantic impact(specialactive)
		CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_AIRFLY,
		CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_AIRFLY_HYPER,
		CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_LANDING,
		CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_LANDING_HYPER,
		//}} 장훈 : 2010-12-15 : Gigantic impact(specialactive)

		//{{ kimhc // 2011.1.4 // 청 1차 전직
		// 루나틱퓨리(퓨리가디언)
		CSI_SI_SA_CFG_LUNATIC_FURY_RUN,
		CSI_SI_SA_CFG_LUNATIC_FURY_ATTACK,
		CSI_SI_SA_CFG_LUNATIC_FURY_BOMBING,
		CSI_SI_SA_CFG_LUNATIC_FURY_END,
		CSI_SI_SA_CFG_LUNATIC_FURY_END_HYPER,
		
		// 리프어택(퓨리가디언)
		CSI_SI_A_CFG_LEAP_ATTACK_JUMP,
		CSI_SI_A_CFG_LEAP_ATTACK_JUMP_DOWN,
		CSI_SI_A_CFG_LEAP_ATTACK_LANDING,

		// 가드(퓨리가디언)
		CSI_SI_A_CFG_GUARD_WAIT,
		CSI_SI_A_CFG_GUARD_END,
		CSI_SI_A_CFG_GUARD_DAMAGE,		// 가드 스킬을 쓴 상태에서 데미지를 받는 경우

		// 듀얼버스터(슈팅가디언)
		CSI_SI_SA_CSG_DUAL_BUSTER_LANDING,

		// 슈팅스타(슈팅가디언)
		CSI_SI_SA_CSG_SHOOTING_STAR_ATTACK,
		CSI_SI_SA_CSG_SHOOTING_STAR_END,
		//}} kimhc // 2011.1.4 // 청 1차 전직

		//{{ 장훈 : 2011-01-18 : 액티브 스킬 - 럼블샷/헤드샷
		CSI_SI_A_CSG_RUMBLE_SHOT,
		CSI_SI_A_CSG_HEAD_SHOT,
		//}} 장훈 : 2011-01-18 : 액티브 스킬 - 럼블샷/헤드샷

		//{{ kimhc // 2011-3-16 // 2011-04 에 패치될 청 캐시 스킬 중 슈팅 가디언 2필, 카밋크래셔
		// 카밋크래셔(슈팅가디언)
		CSI_SI_SA_CSG_COMET_CRASHER_LOOP,
		CSI_SI_SA_CSG_COMET_CRASHER_FINISH,
		//}} kimhc // 2011-3-16 // 2011-04 에 패치될 청 캐시 스킬 중 슈팅 가디언 2필, 카밋크래셔

		//{{ JHKang / 강정훈 / 2011-6-17 / 청 2차 전직 데들리 체이서 레그 샷, 불렛 블릿츠, 아틸러리 스트라이크
		CSI_SI_A_CDC_LEG_SHOT,

		CSI_SI_SA_CDC_BULLET_BLITZ,

		CSI_SI_SA_CDC_ARTILLERY_STRIKE_READY,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_FIRE,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CANCEL,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_END,

		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_IDLE,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_STRIKE,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_LEFT,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_RIGHT,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_UP,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_DOWN,

		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_UP_LEFT,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_UP_RIGHT,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_DOWN_LEFT,
		CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_DOWN_RIGHT,
		//}}

		CSI_CIP_COMBO_POWER_DOWN_LANDING_Z,
		CSI_CIP_COMBO_POWER_DOWN_LANDING_ZZ,
		CSI_CIP_COMBO_DASH_XX,
		CSI_CIP_COMBO_DASH_XXX,
		CSI_CIP_COMBO_DASH_XXXX,
		CSI_CIP_COMBO_DASH_XXXZ,
		CSI_SI_A_CIP_SUDDEN_BURSTER,
		CSI_SI_SA_CIP_BURST_WOLF,
		CSI_SI_SA_CIP_LAND_DEMOLISHER_DOWN,
		CSI_SI_SA_CIP_LAND_DEMOLISHER_LANDING,
		
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		CSI_CHG_COMBO_X,
		CSI_CHG_COMBO_XX,

		CSI_CHG_COMBO_XZ,

		CSI_CHG_DASH_COMBO_ZZ,
		CSI_CHG_DASH_COMBO_ZZX,

		CSI_CHG_DASH_JUMP_COMBO_ZX,

		CSI_SI_A_SIEGE_SHELLING_READY,
		CSI_SI_A_SIEGE_SHELLING_READYLOOP,
		CSI_SI_A_SIEGE_SHELLING_X,
		CSI_SI_A_SIEGE_SHELLING_Z_FINISH,
		CSI_SI_A_SIEGE_SHELLING_X_FINISH,

		CSI_P_CHG_CANNONEER_WITHSTANDING_SIEGE,
		CSI_P_CHG_CANNONEER_WITHSTANDING_WAIT,

		CSI_A_CHG_ELASTIC_BOMB,
		CSI_SA_CHG_WONDER_WALL,
		CSI_SA_CHG_DREAD_CHASE_START,
		CSI_SA_CHG_DREAD_CHASE_LOOP,
		CSI_SA_CHG_DREAD_CHASE_END,

		CSI_SA_CHG_BIGBANG_STREAM,
		CSI_SA_CHG_CARPET_BOMBING,

		CSI_SI_A_CHG_CHAOS_CANNON_START,
		CSI_SI_A_CHG_CHAOS_CANNON_WAIT,
		CSI_SI_A_CHG_CHAOS_CANNON_WALK,
		CSI_SI_A_CHG_CHAOS_CANNON_JUMP_READY,
		CSI_SI_A_CHG_CHAOS_CANNON_JUMP_UP,
		CSI_SI_A_CHG_CHAOS_CANNON_JUMP_DOWN,
		CSI_SI_A_CHG_CHAOS_CANNON_JUMP_LANDIG,
		CSI_SI_A_CHG_CHAOS_CANNON_01,
		CSI_SI_A_CHG_CHAOS_CANNON_02,
		CSI_SI_A_CHG_CHAOS_CANNON_03,
		CSI_SI_A_CHG_CHAOS_CANNON_GUARD,
		CSI_SI_A_CHG_CHAOS_CANNON_END,

		CSI_A_CHG_MARK_OF_COMMANDER,
#endif

#ifdef ADDITIONAL_MEMO
		CSI_SI_A_CFG_GUARD_READY_MEMO,
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		CSI_CTT_COMBO_ZZX,
		CSI_CTT_COMBO_ZZXX,
		CSI_CTT_DASH_COMBO_XX,
		CSI_CTT_MOBILE_ARMORED_MODE_READY,
		CSI_CTT_MOBILE_ARMORED_MODE_LOOP,
		CSI_CTT_MOBILE_ARMORED_MODE_LOOP_FOWARD,
		CSI_CTT_MOBILE_ARMORED_MODE_LOOP_BACKWARD,
		CSI_CTT_MOBILE_ARMORED_MODE_Z,
		CSI_CTT_MOBILE_ARMORED_MODE_X,
		CSI_CTT_MOBILE_ARMORED_MODE_UNREADY,
		CSI_A_CTT_REMOTE_MORTAR,		/// 청 2차 전직 택티컬 트루퍼 - 박격포
		CSI_SI_SA_CTT_CROSS_FIRE,
		CSI_SA_CTT_TACTICAL_FIELD,		/// 청 2차 전직 택티컬 트루퍼 - 택티컬 필드
		CSI_CTT_COMBO_DASH_X,
#endif SERV_CHUNG_TACTICAL_TROOPER

//#ifdef UPGRADE_SKILL_SYSTEM_2013 // 오현빈
		CSI_A_CHUNG_IMPACT_DETONATION_LOOP,	// 임팩트 디토네이션
		CSI_A_CHUNG_IMPACT_DETONATION_FINAL,
		CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_DOWN,	// 랜드 디몰리셔 - 어스퀘이크
		CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_LANDING,
//#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
		CSI_A_CDC_RAPID_CANNON_LOOP,		// 래피드 캐논 루프
		CSI_A_CDC_RAPID_CANNON_FINAL,		// 래피드 캐논 마지막 샷

		CSI_A_CIP_ARTILLERY_NOVA_FIRE,		// 아틸러리 노바 발사
#endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈

	};

	enum CHUNG_MAJOR_PARTICLE_INSTANCE_ID
	{
		CHUNG_MAJOR_PII_BASE,		/// 청만 쓰는 메이저 파티클( 임시로 넣어 높음 )
		CHUNG_MAJOR_PII_END,
	};

	enum CHUNG_MINOR_PARTICLE_INSTANCE_ID
	{
		CHUNG_MINOR_PII_BASE,		/// 청만 쓰는 마이너 파티클( 임시로 넣어 높음 )
		CHUNG_MINOR_PII_END,
	};

	///////////////////////////////////////////////////////
	//{{ 장훈 : 2011-01-15 : 청이 쓰는 메이져 메쉬 인스턴스
	enum CHUNG_MAJOR_MESH_INSTANCE_ID
	{
		CHUNG_MAJOR_MII_BASE,
		CHUNG_MAJOR_MII_CSG_COMBO_XFRONTX_PULL_SUBWEAPON_MINI_GUN,
		CHUNG_MAJOR_MII_CSG_COMBO_XFRONTX_FIRE_MINI_GUN,
		CHUNG_MAJOR_MII_CSG_COMBO_XFRONTXX_MINI_GUN,

		CHUNG_MAJOR_MII_CSG_COMBO_DASH_XX_MINI_GUN,
		CHUNG_MAJOR_MII_CSG_COMBO_DASH_XXX_MINI_GUN,

		CHUNG_MAJOR_MII_CSG_COMBO_DASH_JUMP_X_MINI_GUN,
		CHUNG_MAJOR_MII_CSG_COMBO_DASH_JUMP_XX_MINI_GUN,
		CHUNG_MAJOR_MII_CSG_COMBO_DASH_JUMP_XX_LANDING_MINI_GUN,

		CHUNG_MAJOR_MII_SI_A_CSG_HEAD_SHOT_MINI_GUN,

#ifdef CHUNG_SECOND_CLASS_CHANGE
		CHUNG_MAJOR_MII_CDC_COMBO_X_MINI_GUN_L,
		
		CHUNG_MAJOR_MII_CDC_COMBO_XZ_MINI_GUN_L,
		CHUNG_MAJOR_MII_CDC_COMBO_XZ_MINI_GUN_R,
		
		CHUNG_MAJOR_MII_CDC_COMBO_XZZ_MINI_GUN_L,
		
		CHUNG_MAJOR_MII_CDC_COMBO_ZZXX_MINI_GUN_L,
		CHUNG_MAJOR_MII_CDC_COMBO_ZZXX_MINI_GUN_R,

		CHUNG_MAJOR_MII_SI_A_CDC_LEG_SHOT_MINI_GUN_R,

		CHUNG_MAJOR_MII_SI_SA_CDC_BULLET_BLITZ_MINI_GUN_L,
#endif
		CHUNG_MAJOR_MII_END,
	};
	//}} 장훈 : 2011-01-15 : 청이 쓰는 메이져 메쉬 인스턴스

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
	// 아틸러리 스트라이크 타입
	enum CHUNG_ARTILLERY_STIKE_TYPE  
	{
		CAST_NONE = 0,			
		CAST_MISSILE_SHOWER,	// 미슬 샤워
		CAST_QUANTUM_BALLISTA,	// 퀀텀 발리스타
	};

#endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈

	struct GUARD_STATE_INFO
	{
		CHUNG_STATE_ID	m_eStateId;
		D3DXVECTOR2		m_vecGuardTimer;
	};

	///////////////////////////////////////////////////////

	/** @class : CGuardSystem
		@brief : 청이 사용하는 가드 시스템 클래스 
	*/
	class CGuardSystem
	{
	private:
		int		m_iGuardPointRemain;				/// 남아있는 가드 할 수 있는 횟수
		int		m_iMaxGuardPoint;					/// 가드할 수 있는 최대 횟수
		bool	m_bDamaged;							/// 현재 데미지를 받은 상태인지 에 대한 플래그
		int		m_iDamageReducePercentGuardState;	/// 가드 스테이트에서 감소될 데미지의 Percent
				
	public:
		static const int INITIAL_MAX_GUARD_POINT = 6;

	public:
		CGuardSystem() : m_iGuardPointRemain( INITIAL_MAX_GUARD_POINT ), 
			m_iMaxGuardPoint( INITIAL_MAX_GUARD_POINT ), m_bDamaged( false ), m_iDamageReducePercentGuardState( 0 )
		{}

		int GetGuardPointRemain() const { return m_iGuardPointRemain; }
		__forceinline void SetGuardPointRemain( int iGuardPointRemain_ )
		{ 
			m_iGuardPointRemain = iGuardPointRemain_; 

			if ( 0 > m_iGuardPointRemain )
				m_iGuardPointRemain = 0;
		}

		int GetMaxGuardPoint() const { return m_iMaxGuardPoint; }
		void SetMaxGuardPoint( int iGuardPointAtEachState_ ) { m_iMaxGuardPoint = iGuardPointAtEachState_; }
		
		// 남아있는 가드 포인트를 각 스테이트에서 가드할 수 있는 횟수로 다시 채움
		void ChargeGuardPointRemain() { SetGuardPointRemain( GetMaxGuardPoint() ); }

		int GetDamageReducePercentGuardState() const { return m_iDamageReducePercentGuardState; }
		void SetDamageReducePercentGuardState( int iDamageReducePercentGuardState_ ) { m_iDamageReducePercentGuardState = iDamageReducePercentGuardState_; }
	};

	/** @class : CCannonBallSystem
		@brief : 청이 사용하는 캐논볼 시스템 클래스
		@date  : 2010/12/05
	*/
	class CCannonBallSystem
	{
	private:
		float	m_fTimeNeededToChangeCannonBallChargeState;	/// 캐논볼 차지 상태로 넘어가기 위해 필요한, Wait상태에서 지속해야 하는 시간
		float	m_fTimeElapsedAtWaitState;					/// Wait 상태에서 다른 상태로 바뀌지 않고 지속된 시간
		int		m_iUsedCannonballCountAtThisState;			/// 현재 스테이트에서 소모한 CannonBall의 갯수
		
	public:
		CCannonBallSystem() : m_fTimeNeededToChangeCannonBallChargeState( 0.5f ),
			m_fTimeElapsedAtWaitState( 0.0f ), m_iUsedCannonballCountAtThisState( 0 )
		{}

		float GetTimeNeededToChangeCannonBallChargeState() const { return m_fTimeNeededToChangeCannonBallChargeState; }	// CannonBallChaarge 상태로 넘어가기 위해 지속해야할 시간
		void SetTimeNeededToChangeCannonBallChargeState(float fTimeNeededToChangeCannonBallChargeState_) { m_fTimeNeededToChangeCannonBallChargeState = fTimeNeededToChangeCannonBallChargeState_; } // CannonBallChaarge 상태로 넘어가기 위해 지속해야할 시간

		float GetTimeElapsedAtWaitState() const { return m_fTimeElapsedAtWaitState; }	// Wait 상태에서 지속한 시간
		void SetTimeElapsedAtWaitState(float fTimeElapsedAtWaitState_) { m_fTimeElapsedAtWaitState = fTimeElapsedAtWaitState_; }	// Wait 상태에서 지속한 시간

		int GetUsedCannonballCountAtThisState() const { return m_iUsedCannonballCountAtThisState; }	// 현재 스테이트에서 소모한 CannonBall의 갯수
		void SetUsedCannonballCountAtThisState( int iUsedCannonballCountAtThisState_) { m_iUsedCannonballCountAtThisState = iUsedCannonballCountAtThisState_; }

	};

	//{{ kimhc // 2011.1.11 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	/** @class : CFuryGuardianGuardSkill
	@brief : 청 1차 전직 퓨리가디언이 사용하는 Guard 액티브스킬의 클래스
	@date  : 2011/01/11
	*/

    class   CFuryGuardianGuardSkill;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    typedef boost::intrusive_ptr<CFuryGuardianGuardSkill> CFuryGuardianGuardSkillPtr;	/// 퓨리가디언이 사용하는 Guard 액티브스킬 클래스의 스마트포인터Type
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::shared_ptr<CFuryGuardianGuardSkill> CFuryGuardianGuardSkillPtr;	/// 퓨리가디언이 사용하는 Guard 액티브스킬 클래스의 스마트포인터Type
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	class CFuryGuardianGuardSkill : boost::noncopyable
	{
	private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		CHUNG_STATE_ID	m_eSkillSlotID_Guard;			/// 퓨리가디언의 Guard가 어느 스킬슬롯에 장착되어 있는지를 담는 변수
		const bool*		m_pGuardKey;					/// 퓨리가디언의 Guard가 장착되어 있는 스킬슬롯의 Key(A, S, D, C 등)	
		bool			m_bGuardEquippedAtSlotB;		/// 퓨리가디언의 Guard가 B슬롯에 장착되어 있는지를 담는 변수
		int				m_iDamageReducePercent;			/// 퓨리가디언의 Guard 상태에서 데미지 감소율
		CFuryGuardianGuardSkill() : m_eSkillSlotID_Guard( CSI_BASE ),
			m_pGuardKey( NULL ), m_bGuardEquippedAtSlotB( false ), m_iDamageReducePercent( 0 )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{}

	public:
		static CFuryGuardianGuardSkillPtr   CreateFuryGuardianGuardSkill() { return CFuryGuardianGuardSkillPtr( new CFuryGuardianGuardSkill ); }

		CX2GUChung::CHUNG_STATE_ID GetSkillSlotID_Guard() const { return m_eSkillSlotID_Guard; }	// 퓨리가디언의 Guard가 어느 스킬슬롯에 장착되어 있는지를 담는 변수값을 얻어옴
		void SetSkillSlotID_Guard(CX2GUChung::CHUNG_STATE_ID eSkillSlotID_Guard_) { m_eSkillSlotID_Guard = eSkillSlotID_Guard_; } // 퓨리가디언의 Guard가 어느 스킬슬롯에 장착되어 있는지를 담는 변수값을 Set함

		bool GetGuardKeyValue() const { return *m_pGuardKey; }	// 퓨리가디언의 Guard가 장착되어 있는 스킬슬롯의 Key(A, S, D, C 등)	의 값을 얻어옴 (눌렸는지 안눌렸는지)
		void SetGuardKeyPointer( const bool* pGuardKey_ ) { m_pGuardKey = pGuardKey_; }	// 퓨리가디언의 Guard가 장착되어 있는 스킬슬롯의 Key(A, S, D, C 등)	포인터를 Set함

		bool GetGuardEquippedAtSlotB() const { return m_bGuardEquippedAtSlotB; }	// 퓨리가디언의 Guard가 B슬롯에 장착되어 있는지를 얻어옴
		void SetGuardEquippedAtSlotB(bool bGuardEquippedAtSlotB_) { m_bGuardEquippedAtSlotB = bGuardEquippedAtSlotB_; }

		int GetDamageReducePercent() const { return m_iDamageReducePercent; }
		void SetDamageReducePercent(int iDamageReducePercent_) { m_iDamageReducePercent = iDamageReducePercent_; }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        void    AddRef()    {   ++m_uRefCount; }
        void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	};


#endif	CHUNG_FIRST_CLASS_CHANGE
	//}} kimhc // 2011.1.11 // 청 1차 전직

#ifdef CHUNG_SECOND_CLASS_CHANGE
	int GetShot() const { return m_iShot; }
	void SetShot( int iNPCId_ ) { m_iShot = iNPCId_; }
#endif
#ifdef RIDING_SYSTEM
	virtual bool CanRidingState();
#endif //RIDING_SYSTEM
	
private:
	CX2GUChung( int iUnitIndex_, int iTeamNum_, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		int iFrameBufferNum_, IN CX2Unit* pUnit_ );
	virtual ~CX2GUChung();

protected:
	virtual void		Init( bool bUseTeamPos_, int iStartPosIndex_ );	// 청만 가지고 있는 멤버들 초기화
	virtual void		ParseCommonRandomState();	// 실행할 Win, Lose, Start 상태 등을 랜덤하게 Load 하기 위한 함수
	virtual void		InitState();				// 모든 상태를 초기화
	
	void				InitStateID();				// 기본적인 State의 ID를 초기화 (walk, dash, jump 등)
	void				InitStateCommon();			// 공통으로 가지고 있는 State(하지만 스테이트 명은 다름)을 초기화
	void				InitStateByUnitClass();		// 전직 별로 다른 스테이트를 초기화
	virtual void		InitComponent();			// 캐릭터 스크립트의 INIT_COMPONENT 테이블을 로드함

	virtual void		InitPassiveSkillState();	// 패시브 스킬 초기화
	virtual void		SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ );	// 장착한 스킬 상태 셋팅
	virtual void		SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ );	// 장착한 스킬의 레벨에 따라 각 스킬에 해당하는 상태의 PowerRate등을 셋팅
	void				SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet, CHUNG_STATE_ID eStateID );	// 장착한 스킬의 레벨등이 변경될 때 같이 변경되어야 할 스테이트 데이타

	virtual void		CommonStateStartFuture();	// 스테이트가 변경될 때 마다 한번씩 실행되는 함수로 해당 스테이트의 lua 테이블을 읽어옴. 단, Future는 LocalUnit만 실행
	virtual void		CommonStateStart();	// 스테이트가 변경될 때 마다 한번씩 실행되는 함수로 해당 스테이트의 lua 테이블을 읽어옴

	virtual void		CommonFrameMoveFuture();
	virtual void		CommonFrameMove();

	virtual void		CommonStateEndFuture();
	virtual void		CommonStateEnd();

	virtual RENDER_HINT	CommonRender_Prepare();
	
	virtual bool		SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );
	virtual void		NoStateChangeActionFrameMove();

	virtual float		GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;
	
	void				LoadPersonalInfo( IN KLuaManager& luaManager_ );	// 청만이 가지고 있는 파싱할 정보 등을 로드함
	void				LoadStateIDCanBeChangedGuardState( IN KLuaManager& luaManager_ );	// 가드 스테이트로 넘어갈 수 있는 스테이트 ID 리스트를 스크립트로 부터 읽어들임
	void				LoadPromotionItemID( IN KLuaManager& luaManager_ );


	bool				IsThisStateCanBeChangedToGuardStateOnHit();	// 타격을 받았을 때 Guard 상태로 변경 될 수 있는 스테이트 인가?
	
	void				ChangeAnimationChargeCannonBallNow();
	void				ChangeAnimationChargeCannonBallFuture();

#ifdef BALANCE_PATCH_20120329

#ifdef SERV_CHUNG_TACTICAL_TROOPER		/// 택티컬 필드로 캐논볼 충전시, 리로드 마스터리 적용 해제
	__forceinline void UpCannonBallCount( int iUpCount_, bool bForce = false, bool bNoCheckReloadMastery = false );
#else SERV_CHUNG_TACTICAL_TROOPER
	__forceinline void UpCannonBallCount( int iUpCount_, bool bForce = false );	// 캐논볼의 카운트를 한개 올린다.
#endif SERV_CHUNG_TACTICAL_TROOPER

#else
	__forceinline void UpCannonBallCount( int iUpCount_ );	// 캐논볼의 카운트를 한개 올린다.
#endif
	void UpCannonBallCountAtThisAnimationTime( float fAnimationTimeNeededToChargeCannonBall_, int iUpCount_ );	// 정해진 애니메이션 타임에 캐논볼 카운트를 한개 올린다. (m_pXSkinAnim의 animation 시간을 체크함)
	void UseCannonAtThisAnimationTime( float fAnimationTimeNeededToUseCannonBall_, int iUpCount_ );	// 정해진 애니메이션 타임에 캐논볼 카운트를 한개 소모한다. (m_pXSkinAnim의 animation 시간을 체크함)

	void				UseCannon( int iCount_ );	// 캐논을 사용 할 때 호출 해야한다.
	void				ConsumeCannonBallCount( int iCount_  );	// 캐논볼을 전달한 인자 만큼 소모한다.

	__forceinline bool	CanUseCannon();
	__forceinline bool	IsEmptyCannonBallCount();
	__forceinline bool	IsFullCannonBallCount();

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	__forceinline int	GetTotalCannonBallCount();
	void				SetTotalCannonBallCount( int iTotalCannonBallCount_ );
	
	__forceinline int	GetExtraCannonBallCount();
	void				SetExtraCannonBallCount( int iExtraCannonBallCount_ ) 
	{ 
		GetChungGageData()->SetCannonBallChanged( true );
		GetChungGageData()->SetNowCannonBallCountEx( iExtraCannonBallCount_ ); 
	}

	void				SetMaxExtraCannonBallCount( int iMaxExtraCannonCount_ ) { GetChungGageData()->SetMaxCannonBallCountEx( iMaxExtraCannonCount_ ); }
#endif SERV_CHUNG_TACTICAL_TROOPER

	void				SetCannonBallCount( int iCannonBallCount_ ) 
	{ 
		GetChungGageData()->SetCannonBallChanged( true );
		GetChungGageData()->SetNowCannonBallCount( iCannonBallCount_ ); 
	}
	void				SetMaxCannonBallCount( int iMaxCannonCount_ ) { GetChungGageData()->SetMaxCannonBallCount( iMaxCannonCount_ ); }
	
	CX2ChungGageData*	GetChungGageData() { return static_cast<CX2ChungGageData*>( GetGageData() ); }
	const CX2ChungGageData*	GetChungGageData() const { return static_cast<const CX2ChungGageData*>( GetGageData() ); }
	void				InitPromotionEquipList();

// 	CX2OldGageManager::CCannonBallUIPtr GetCannonBallUIPtr() 
// 	{ 
// 		ASSERT( 0 < m_GageManager.GetCannonBallUIPtr().use_count() );
// 		return m_GageManager.GetCannonBallUIPtr(); 
// 	}

	void ShowActiveSkillCutInAndLightByBerserkMode( float fTimeToShow_, bool bOnlyLight_ = false );	// 전달해준 시간에 광폭화모드에 따라 스킬 컷인과 light를 출력함

	virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

	// 리로드 캐논에 의해서 충전될 캐논볼의 갯수 (리로드캐논은 기본 스킬이므로 모두 가지고 있음)
	UCHAR GetCannonBallCountToBeUpUsingReLoadCannon() const { return m_ucCannonBallCountToBeUpUsingReLoadCannon; }
	void SetCannonBallCountToBeUpUsingReLoadCannon(UCHAR ucCannonBallCountToBeUpUsingReLoadCannon_ ) { m_ucCannonBallCountToBeUpUsingReLoadCannon = ucCannonBallCountToBeUpUsingReLoadCannon_; }
	UCHAR GetCannonBallCountToBeUpUsingReLoadCannon() { return m_ucCannonBallCountToBeUpUsingReLoadCannon; }

	UCHAR GetBasicCannonBallCount() const { return m_ucBasicCannonBallCount; }
	void SetBasicCannonBallCount( UCHAR ucBasicCannonBallCount_ ) { m_ucBasicCannonBallCount = ucBasicCannonBallCount_; }

	void ResetCannonBallCountWhenRebirthInPVPGame( int iNumOfMyTeamKill_, int iNumOfEnmyTeamKill_ );

	void ShowOnomatopoeiaParticle( const WCHAR* wszWeaponBoneName_, const WCHAR* wszParticleName_ );	// '쾅' 하는 의성어 파티클을 출력

	float GetDamageRelByCannonBallMastery() const { return m_fDamageRelByCannonBallMastery; }
	void SetDamageRelByCannonBallMastery( float fDamageRelByCannonBallMastery_ ) { m_fDamageRelByCannonBallMastery = fDamageRelByCannonBallMastery_; }

//{{ kimhc // 2011.1.14 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	float GetForceDownRelAtMeleeAttack() const { return m_fForceDownRelAtMeleeAttack; }	// 근접 공격 포스다운 수치 조정
	void SetForceDownRelAtMeleeAttack(float fForceDownRelAtMeleeAttack_) { m_fForceDownRelAtMeleeAttack = fForceDownRelAtMeleeAttack_; }

	float GetProbabilityBigDamageAtGuardMastery() const { return m_fProbabilityBigDamageAtGuardMastery; }	// 가드마스터리에 의해 GuardX등의 공격이 big데미지를 줄 확률
	void SetProbabilityBigDamageAtGuardMastery(float Val_) { m_fProbabilityBigDamageAtGuardMastery = Val_; }

	void InitRemodelMagazinePassiveSkill(); // 탄창/약실 개조 패시브 스킬 Init함수

	float GetIncreaseRangeSubWeaponShot() const { return m_fIncreaseRangeSubWeaponShot; }	// 청 1차 전직, 슈팅 가디언의 서브웨폰 사거리 증가율(강선개조 패시브)
	void SetIncreaseRangeSubWeaponShot(float fIncreaseRateSubWeaponShot_) { m_fIncreaseRangeSubWeaponShot = fIncreaseRateSubWeaponShot_; }

	virtual float GetCriticalValue( float fDefenderCritical_ = 0.f );

	float GetCriticalDamageRate() const { return m_fCriticalDamageRate; }	// 청 1차 전직, 슈팅 가디언의 강선개조에 의한 Critical 시 데미지 증가
	void SetCriticalDamageRate(float fCriticalDamageRate_) { m_fCriticalDamageRate = fCriticalDamageRate_; }
	float CalculateHpPercentUpRateByDamageDataHpUp( float fDamageDataHpPercentUp_ );

	void SetReduceGuardDamageAtHeavyStanceFromGuardSkill();	// 퓨리가디언의 가드 스킬에 의해서 증가된 값을 헤비스텐스의 데미지 감소율을 셋팅
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.14 // 청 1차 전직

	///////////////////////////////////////////////////////
	//{{ 장훈 : 2011-01-19
#ifndef SERV_9TH_NEW_CHARACTER // 김태환
	/// 다른 캐릭터들 전부 똑같은 함수를 쓰고 있으니, X2GUUser로 옮기자.
	virtual void		CreateNotEnoughMPEffect( D3DXVECTOR3 vPos, float fDegreeX, float fDegreeY, float fDegreeZ );
#endif // SERV_9TH_NEW_CHARACTER
	//}} 장훈 : 2011-01-19
	///////////////////////////////////////////////////////
#ifdef CHUNG_SECOND_CLASS_CHANGE
	float GetProbabilityIronWill() const { return m_fProbabilityIronWill; }
	void SetProbabilityIronWill(float Val_) { m_fProbabilityIronWill = Val_; }
#endif CHUNG_SECOND_CLASS_CHANGE

#ifdef CHUNG_SECOND_CLASS_CHANGE
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	void SetSelfDestructSummonedNPC( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE, float fDeleteTime = 3.f );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	void SetSelfDestructSummonedNPC( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#endif

	virtual CX2GageData*	CreateGageData();

#ifdef BALANCE_DEADLY_CHASER_20130214
	void SetExtraDamageMiniGun( CX2DamageManager::DamageData& DamageData_, float fRate_ ); // 
#endif //BALANCE_DEADLY_CHASER_20130214

public:
	static CX2GUChung*	CreateGUChung( int iUnitIndex_, int iTeamNum_, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		int iFrameBufferNum_, IN CX2Unit* pUnit_ )	// 팩토리 함수
	{
		return new CX2GUChung( iUnitIndex_, iTeamNum_, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			iFrameBufferNum_, pUnit_ );
	}


#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	static void			AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	virtual void		ReInit( bool bTeamPos_ = false, int iStartPosIndex_ = -1 );	// 게임내에서 부활석 등에 의해 다시 게임을 시작할 때 사용됨
	
	virtual void		DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ );	// 피격됐을 때 실행되는 함수
	
	virtual void		Win();	// 승리모션
	virtual void		Lose();	// 패배모션
	virtual void		Draw();	// 비겼을 때의 모션

	virtual void		InitEffect();	// 캐릭터가 쓰는 이펙트 중 공통으로 쓰는 이펙트(대쉬 할때 나오는 연기 등) 및 각 캐릭터 마다 쓰는 이펙트 초기화
	virtual void		InitEnchantWeaponEffectUnitSpecific();	// 각 캐릭터가 쓰는 무기 의 인첸트 파티클 초기화 (블레이즈, 프로즌, 포이즌은 여기서 하고 그외의 것은 다른곳에서 함)

	void				InitializeChungMajorParticleArray();	// 메이저 파티클 배열을 초기화
	virtual void		CreateMajorParticleArray();		// 메이저 파티클 배열을 생성
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	static void			AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CKTDGParticleSystem::CParticleEventSequence* SetChungMajorParticleByEnum( CHUNG_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 
	ParticleEventSequenceHandle	GetHandleChungMajorParticleByEnum( CHUNG_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const // 청만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴
	{
		ASSERT( CHUNG_MAJOR_PII_END > eVal_ && CHUNG_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		return m_ahChungMajorParticleInstance[eVal_];
	}

	void				SetHandleChungMajorParticleByEnum( CHUNG_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ ) // 청만 쓰는 메이저 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함
	{
		ASSERT( CHUNG_MAJOR_PII_END > eVal_ && CHUNG_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		m_ahChungMajorParticleInstance[eVal_] = hHandle_;
	}
	void				DeleteChungMajorParticle();

	void				InitializeChungMinorParticleArray();	// 공통적으로 쓰는 마이너 파티클 배열을 초기화
	virtual void		CreateMinorParticleArray();		// 각 캐릭터들만 쓰는 마이너 파티클 배열을 초기화
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	static void			AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	CKTDGParticleSystem::CParticleEventSequence* SetChungMinorParticleByEnum( CHUNG_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴
	ParticleEventSequenceHandle	GetHandleChungMinorParticleByEnum( CHUNG_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const	// 청만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴
	{
		ASSERT( CHUNG_MINOR_PII_END > eVal_ && CHUNG_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		return m_ahChungMinorParticleInstance[eVal_];
	}

	void				SetHandleChungMinorParticleByEnum( CHUNG_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )	// 청만 쓰는 마이너 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함
	{
		ASSERT( CHUNG_MINOR_PII_END > eVal_ && CHUNG_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
		m_ahChungMinorParticleInstance[eVal_] = hHandle_;
	}
	///////////////////////////////////////////////////////
	//{{ 장훈 : 2011-01-15 : 청이 쓰는 메이져 메쉬 인스턴스
	void InitializeMajorMeshArray();	// 메이저 파티클 배열을 초기화
	void DeleteMajorMesh();
	CKTDGXMeshPlayer::CXMeshInstanceHandle GetHandleChungMajorMeshByEnum( CHUNG_MAJOR_MESH_INSTANCE_ID eVal_ ) const
	{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        ASSERT( CHUNG_MAJOR_MII_END > eVal_ && CHUNG_MAJOR_MESH_INSTANCE_ID(0) <= eVal_ );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		ASSERT( CHUNG_MAJOR_MII_END > eVal_ && INVALID_MESH_INSTANCE_HANDLE < eVal_ );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		return m_ahChungMajorMeshInstance[eVal_];
	}
	void SetHandleChungMajorMeshByEnum( CHUNG_MAJOR_MESH_INSTANCE_ID eVal_, CKTDGXMeshPlayer::CXMeshInstanceHandle hHandle_ );
	//}} 장훈 : 2011-01-15 : 청이 쓰는 메이져 메쉬 인스턴스
	///////////////////////////////////////////////////////
	void				DeleteChungMinorParticle();

	virtual	void		DeleteMinorParticle();

	virtual bool		SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );	// 필살기 중 스테이트 변경이 없는 것 수행
	virtual void		COMMON_BUFF_FrameMove();

	virtual void		NotifyShowObjectChanged();		
	virtual void		NotifyDGManagerChanged( CKTDGManager& refInManager_ );		// DGManager에 등록
	void				SetShowPromotionEquips( bool bShow_ );
	virtual void		SetShowViewList( bool bShow_ );	
	void				SetShowOriginalViewList( bool bShow_ );
	void				ReplaceWeapon( CX2EqipPtr pWeaponPtr_ );	// 광폭화 모드와 관련하여 무기 변경
	virtual bool		UpdateEquipViewListInfo( CX2Item* pItemToAddInViewList, CX2Item* const pItemToRemoveInViewList );
	virtual void		UpdateEquipCollisionData();
	
	virtual bool		IsFullHyperMode() const { return GetChungGageData()->GetBerserkMode(); }
	virtual void		SetFullHyperMode( const bool bBerserkMode_ )
	{
		GetChungGageData()->SetBerserkModeChanged( true );
		GetChungGageData()->SetBerserkMode( bBerserkMode_ ); 
	}

	__forceinline int	GetCannonBallCount();

	virtual		HRESULT OnFrameMove( double fTime, float fElapsedTime );
	
	virtual void ChargeMpAndEtcInTrainingGame();

	virtual void		ApplyHyperModeBuff();
	virtual void		HyperModeBuffEffectStart();
	virtual void		HyperModeBuffEffectEnd();

	virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ );

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	void				CreateRemoteMortar();							/// 박격포 소환 함수
	void				InsertRemoteMortarNPCUID( UidType iNPCUID ) { m_iRemoteMortarNPCUID = iNPCUID; }	/// 박격포 아이디 추가 함수
	void				DeleteRemoteMortarNPCUID() { m_iRemoteMortarNPCUID = -1; }							/// 박격포 아이디 삭제 함수
	UidType				GetRemoteMortarNPCUID() { return m_iRemoteMortarNPCUID; }							/// 박격포 아이디 반환 함수

	void				SupplyCannonBallCount( int SupplyCount );		/// 택티컬 필드에서 보급될 케논볼
#endif SERV_CHUNG_TACTICAL_TROOPER

	virtual float		GetFinalDamageToBeChangedBySkill( float fFinalDamage_ );

	virtual int GetComboZStateID() const { return CSI_COMBO_Z; }
	virtual int GetComboXStateID() const { return CSI_COMBO_X; }
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//							스테이트 함수 시작
	///////////////////////////////////////////////////////////////////////////////////////////////////////
protected:

	// CSI_DIE_FRONT
	void DieFrontStartFuture();
	void DieFrontStart();
	void DieFrontFrameMove();
	void DieFrontEventProcess();

	// CSI_DIE_BACK
	void DieBackStartFuture();
	void DieBackStart();
	void DieBackFrameMove();
	void DieBackEventProcess();

	// CSI_WAIT
	void WaitStartFuture();
	void WaitStart();
	void WaitFrameMoveFuture();
	void WaitEventProcess();

	// CSI_WALK
	void WalkEventProcess();

	// CSI_JUMP_READY
	void JumpReadyEventProcess();

	// CSI_JUMP_UP
	void JumpUpEventProcess();

	// CSI_JUMP_DOWN
	void JumpDownEventProcess();

	// CSI_JUMP_LANDING
	void JumpLandingEventProcess();

	// CSI_DASH
	void DashEventProcess();

	//CSI_DASH_END
	void DashEndStartFuture();
	void DashEndFrameMove();
	void DashEndEventProcess();

	// CSI_DASH_JUMP
	void DashJumpStartFuture();
	void DashJumpStart();
	void DashJumpFrameMoveFuture();
	void DashJumpEventProcess();
	void DashJumpEndFuture();

	// CSI_DASH_JUMP_LANDING
	void DashJumpLandingEventProcess();

	// CSI_POWER_DOWN_LANDING
	void PowerLandingStartFuture();
	void PowerDownLandingStart();
	void PowerDownLandingEventProcess();

	// CSI_HYPER_MODE
	void HyperModeFrameMove();
#ifdef MODIFY_RIDING_PET_AWAKE
	void RidingHyperModeFrameMove();
	void CommonHyperModeFrameMove( float fTime1_, float fTime2_, float fTime3_, bool bSound_ = false );
#endif // MODIFY_RIDING_PET_AWAKE
	// CSI_DAMAGE_SMALL_FRONT and CSI_DAMAGE_SMALL_BACK
	void DamageSmallEventProcess();

	// CSI_DAMAGE_BIG_FRONT and CSI_DAMAGE_BIG_BACK
	void DamageBigStart();
	void DamageBigEventProcess();

	// CSI_DAMAGE_DOWN_FRONT
	void DamageDownFrontFrameMove();
	void DamageDownFrontEventProcess();

	// CSI_DAMAGE_DOWN_BACK
	void DamageDownBackFrameMove();
	void DamageDownBackEventProcess();

	// CSI_DAMAGE_AIR_DOWN_LANDING
	void DamageAirDownLandingEventProcess();

	//CSI_STANDUP_ROLLING
	void StandUpRollingEventProcess();
	void StandUpRollingEnd();

	//CSI_STANDUP_ATTACK
	void StandUpAttackEventProcess();
	void StandUpAttackEnd();

	// CSI_CHARGE_CANNON_BALL
	void ChargeCannonBallStart();
	void ChargeCannonBallStartFuture();
	void ChargeCannonBallFrameMove();
	void ChargeCannonBallEventProcess();
	
	// CSI_GUARD
	void GuardStart();
	void GuardEventProcess();
	
	// CSI_GUARD_ATTACK_Z
	void GuardAttackZEventProcess();

    // CSI_GUARD_ATTACK_X
	void GuardAttackXInit();
	void GuardAttackXFrameMove();
	void GuardAttackXEventProcess();

	// CSI_COMBO_Z
	void ComboZEventProcess();
	// CSI_COMBO_ZZ
	void ComboZZEventProcess();
	// CSI_COMBO_ZZZ
	void ComboZZZEventProcess();
	// CSI_COMBO_ZZZZ
	void ComboZZZZEventProcess();

	// CSI_COMBO_ZZZX_READY
	void ComboZZZXReadyEventProcess();

	// CSI_COMBO_ZZZX_CANNON_BALL_SHOT
	void ComboZZZXCannonBallShotInit();
	void ComboZZZXCannonBallShotStartFuture();
	void ComboZZZXCannonBallShotFrameMove();
	void ComboZZZXCannonBallShotEventProcess();

	// CSI_COMBO_ZZZX_FINISH
	void ComboZZZXFinishInit();
	void ComboZZZXFinishStartFuture();
	void ComboZZZXFinishFrameMove();
	void ComboZZZXFinishEventProcess();

	// CSI_COMBO_ZZdownZ
	void ComboZZdownZEventProcess();
	// CSI_COMBO_ZZdownZZ
	void ComboZZdownZZEventProcess();

	// CSI_COMBO_X
	void ComboXEventProcess();
	// CSI_COMBO_XX
	void ComboXXInit();

	void ComboXXEventProcess();
	// CSI_COMBO_XXX
	void ComboXXXInit();
	void ComboXXXFrameMove();
	void ComboXXXEventProcess();
	// CSI_COMBO_XXZ
	void ComboXXZEventProcess();

	// CSI_COMBO_XXZX_CANNON_BALL_SHOT
	void ComboXXZXCannonBallShotInit();
	void ComboXXZXCannonBallShotFrameMove();
	void ComboXXZXCannonBallShotEventProcess();

	// CSI_COMBO_XXZX_FINISH
	void ComboXXZXFinishInit();
	void ComboXXZXFinishFrameMove();
	void ComboXXZXFinishEventProcess();

	// CSI_COMBO_DASH_Z
	void ComboDashZEventProcess();
	
	// CSI_ATTACK_DASH_X
	void AttackDashXInit();
	void AttackDashXFrameMove();
	void AttackDashXEventProcess();
	void AttackDashXEndFuture();
	
	// CSI_ATTACK_JUMP_Z
	void AttackJumpZEventProcess();
	void AttackJumpZFrameMoveFuture();
    // CSI_ATTACK_JUMP_X
	void AttackJumpXEventProcess();
	void AttackJumpXFrameMoveFuture();
    
	// CSI_COMBO_DASH_JUMP_Z
	void ComboDashJumpZEventProcess();
	// CSI_COMBO_DASH_JUMP_ZZ
	void ComboDashJumpZZEventProcess();

	// CSI_COMBO_DASH_JUMP_ZZLanding
	void ComboDashJumpZZLandingEventProcess();

	// CSI_COMBO_DASH_JUMP_ZZLanding_CANNON_BALL_SHOT
	void ComboDashJumpZZLandingCannonBallShotInit();
	
	void ComboDashJumpZZLandingCannonBallShotEventProcess();
    
	// CSI_ATTACK_DASH_JUMP_X
	void AttackDashJumpXInit();
	void AttackDashJumpXStartFuture();
	void AttackDashJumpXFrameMove();
	void AttackDashJumpXFrameMoveFuture();
	void AttackDashJumpXEventProcess();

	// CSI_STANDUP_CANNON_ATTACK_FRONT && BACK
	void StandUpCannonAttackInit();
	void StandUpCannonAttackFrameMove();
	void StandUpCannonAttackEventProcess();
	void StandUpCannonAttackEnd();

	// CSI_STANDUP_CANNON_ATTACK_BACK
	void StandUpCannonAttackEndFuture();

//{{ kimhc // 2011.1.4 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	// CSI_CFG_COMBO_ZZX (퓨리가디언)

	void CFGComboZZXEventProcess();
	
	// CSI_COMBO_DASH_Z (퓨리 가디언 사용)
	void CFGComboDashZEventProcess();

	// CSI_CFG_COMBO_DASH_ZZ
	void CFGComboDashZZEventProcess();

	// CSI_CFG_COMBO_DASH_ZZZ
	void CFGComboDashZZZEventProcess();

	// CSI_CFG_COMBO_DASH_JUMP_ZX
	void CFGComboDashJumpZXEventProcess();
	
	// CSI_CFG_COMBO_DASH_JUMP_ZXX
	void CFGComboDashJumpZXXStart();
	void CFGComboDashJumpZXXEventProcess();
		
	// CSI_CFG_COMBO_DASH_JUMP_ZXXLanding
	void CFGComboDashJumpZXXLandingEventProcess();

	///////////////////////////////////////////////////////
	//{{ 장훈 : 2011-01-12 : 청 1차 전직 슈팅가디언

	// CSI_CSG_COMBO_XFRONTX_PULL_SUBWEAPON,
	void CSGComboXFrontXPullSubweaponInit();
	void CSGComboXFrontXPullSubweaponFrameMove();
	void CSGComboXFrontXPullSubweaponEventProcess();
	void CSGComboXFrontXPullSubweaponEnd();
	// CSI_CSG_COMBO_XFRONTX_FIRE,
	void CSGComboXFrontXFireInit();
	void CSGComboXFrontXFireFrameMove();
	void CSGComboXFrontXFireEventProcess();
	void CSGComboXFrontXFireEnd();
	// CSI_CSG_COMBO_XFRONTXX,
	void CSGComboXFrontXXInit();
	void CSGComboXFrontXXFrameMove();
	void CSGComboXFrontXXEventProcess();
	void CSGComboXFrontXXEnd();

	// CSI_CSG_COMBO_DASH_XX,
	void CSGComboDashXXInit();
	void CSGComboDashXXFrameMove();
	void CSGComboDashXXEventProcess();
	void CSGComboDashXXEnd();
	// CSI_CSG_COMBO_DASH_XXX,
	void CSGComboDashXXXInit();
	void CSGComboDashXXXFrameMove();
	void CSGComboDashXXXEventProcess();
	void CSGComboDashXXXEnd();

	// CSI_CSG_COMBO_DASH_JUMP_X,
	void CSGComboDashJumpXInit();
	void CSGComboDashJumpXFrameMove();
	void CSGComboDashJumpXEventProcess();
	void CSGComboDashJumpXEnd();
	// CSI_CSG_COMBO_DASH_JUMP_XX,
	void CSGComboDashJumpXXInit();
	void CSGComboDashJumpXXFrameMove();
	void CSGComboDashJumpXXEventProcess();
	void CSGComboDashJumpXXEnd();
	// CSI_CSG_COMBO_DASH_JUMP_XX_Landing,
	void CSGComboDashJumpXXLandingInit();
	void CSGComboDashJumpXXLandingFrameMove();
	void CSGComboDashJumpXXLandingEventProcess();
	void CSGComboDashJumpXXLandingEnd();
	//}} 장훈 : 2011-01-12 : 청 1차 전직 슈터가디언
	///////////////////////////////////////////////////////

#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.4 // 청 1차 전직
	
	//{{ JHKang / 강정훈 / 2011.6.14 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
	// CSI_CDC_COMBO_X
	void CDCComboXInit();
	void CDCComboXFrameMove();
	void CDCComboXEventProcess();
	void CDCComboXEnd();
	// CSI_CDC_COMBO_XX
	void CDCComboXXInit();
	void CDCComboXXFrameMove();
	void CDCComboXXEventProcess();
	// CSI_CDC_COMBO_XZ
	void CDCComboXZInit();
	void CDCComboXZFrameMove();
	void CDCComboXZEventProcess();
	void CDCComboXZEnd();
	// CSI_CDC_COMBO_XZZ
	void CDCComboXZZInit();
	void CDCComboXZZFrameMove();
	void CDCComboXZZEventProcess();
	// CSI_CDC_COMBO_ZZX
	void CDCComboZZXEventProcess();
	// CSI_CDC_COMBO_ZZXX
	void CDCComboZZXXInit();
	void CDCComboZZXXFrameMove();
	void CDCComboZZXXEventProcess();
	void CDCComboZZXXEnd();
	// CSI_CDC_COMBO_ZZXXX
	void CDCComboZZXXXInit();
	void CDCComboZZXXXFrameMove();
	void CDCComboZZXXXEventProcess();
#endif
	//}}

#ifdef CHUNG_SECOND_CLASS_CHANGE
	// oasis907 : 김상윤 [2011.6.15] 청 2차 전직 - 아이언 팔라딘
	// CSI_CIP_COMBO_POWER_DOWN_LANDING_Z
	void CIPComboPowerDownLandingZInit();

	void CIPComboPowerDownLandingZEventProcess();
	void CIPComboPowerDownLandingZEnd();

	// CSI_CIP_COMBO_POWER_DOWN_LANDING_ZZ
	void CIPComboPowerDownLandingZZInit();

	void CIPComboPowerDownLandingZZEventProcess();
	void CIPComboPowerDownLandingZZEnd();

	// CSI_CIP_COMBO_DASH_XX
	void CIPComboDashXXInit();
	void CIPComboDashXXFrameMove();
	void CIPComboDashXXEventProcess();
	void CIPComboDashXXEnd();
	// CSI_CIP_COMBO_DASH_XXX,
	void CIPComboDashXXXInit();
	void CIPComboDashXXXFrameMove();
	void CIPComboDashXXXEventProcess();
	void CIPComboDashXXXEnd();
	// CSI_CIP_COMBO_DASH_XXXX
	void CIPComboDashXXXXInit();

	void CIPComboDashXXXXEventProcess();
	void CIPComboDashXXXXEnd();
	// CSI_CIP_COMBO_DASH_XXXZ
	void CIPComboDashXXXZInit();
	void CIPComboDashXXXZFrameMove();
	void CIPComboDashXXXZEventProcess();
	void CIPComboDashXXXZEnd();

	// 랜드디몰리셔 콤보에 붙는 이펙트 위해 한곳에 몰아넣음
	// 제작 다 되면 원위치로 갈 예정

	void ComboZFrameMove();
	void ComboZZFrameMove();
	void ComboZZZFrameMove();
	void ComboZZZZFrameMove();


	void ComboZZdownZZFrameMove();

	void ComboXFrameMove();


	void CFGComboDashZFrameMove();
	void CIPComboDashXXXXFrameMove();

	void PowerDownLandingFrameMove();
	void CIPComboPowerDownLandingZFrameMove();
	void CIPComboPowerDownLandingZZFrameMove();

	void ComboDashJumpZZLandingFrameMove();


	void StandUpAttackFrontFrameMove();
	void StandUpAttackBackFrameMove();
	void GuardAttackZFrameMove();


#endif CHUNG_SECOND_CLASS_CHANGE

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	void CHG_ComboXFrameMove();
	void CHG_ComboXEventProcess();
	void CHG_ComboXXFrameMove();
	void CHG_ComboXXEventProcess();
	
	void CHG_ComboDashZEventProcess();
	void CHG_ComboDashZZFrameMove();
	void CHG_ComboDashZZEventProcess();
	void CHG_ComboDashZZXFrameMove();
	void CHG_ComboDashZZXEventProcess();

	void CHG_ComboDashJumpZXFrameMove();
	void CHG_ComboDashJumpZXEventProcess();
	
	void CSI_SI_A_SIEGE_SHELLING_Ready_StateStart();
	void CSI_SI_A_SIEGE_SHELLING_Ready_FrameMove();
	void CSI_SI_A_SIEGE_SHELLING_Ready_EventProcess();	
	void CSI_SI_A_SIEGE_SHELLING_ReadyLoop_FrameMove();
	void CSI_SI_A_SIEGE_SHELLING_ReadyLoop_EventProcess();
	void CSI_SI_A_SIEGE_SHELLING_X_FrameMove();
	void CSI_SI_A_SIEGE_SHELLING_X_EventProcess();
	void CSI_SI_A_SIEGE_SHELLING_X_Finish_FrameMove();
	void CSI_SI_A_SIEGE_SHELLING_X_Finish_EventProcess();
	void CSI_SI_A_SIEGE_SHELLING_Z_Finish_EventProcess();
	void CSI_P_CHG_CANNONEER_WITHSTANDING_FrameMove();
	void CSI_P_CHG_CANNONEER_WITHSTANDING_SIEGE_EventProcess();
	void CSI_P_CHG_CANNONEER_WITHSTANDING_WAIT_EventProcess();

	void CSI_A_CHG_ELASTIC_BOMB_FrameMove();
	void CSI_A_CHG_ELASTIC_BOMB_EventProcess();
	void CSI_A_CHG_MARK_OF_COMMANDER_Init();
	void CSI_A_CHG_MARK_OF_COMMANDER_FrameMove();
	void CSI_A_CHG_MARK_OF_COMMANDER_EventProcess();
	void CSI_SA_CHG_WONDER_WALL_FrameMove();
	void CSI_SA_CHG_WONDER_WALL_EventProcess();
	void CSI_SA_CHG_DREAD_CHASE_START_StateStart();
	void CSI_SA_CHG_DREAD_CHASE_START_FrameMove();
	void CSI_SA_CHG_DREAD_CHASE_START_EventProcess();
	void CSI_SA_CHG_DREAD_CHASE_LOOP_FrameMove();
	void CSI_SA_CHG_DREAD_CHASE_LOOP_EventProcess();
	void CSI_SA_CHG_DREAD_CHASE_END_FrameMove();
	void CSI_SA_CHG_DREAD_CHASE_END_EventProcess();
	void CSI_SA_CHG_BIGBANG_STREAM_FrameMove();
	void CSI_SA_CHG_BIGBANG_STREAM_EventProcess();
	void CSI_SA_CHG_CARPET_BOMBING_FrameMove();
	void CSI_SA_CHG_CARPET_BOMBING_EventProcess();
	void DoCarpetBombing();

	void CSI_SI_A_CHG_CHAOS_CANNON_START_FrameMove();
	void CSI_SI_A_CHG_CHAOS_CANNON_START_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_WAIT_FrameMove();
	void CSI_SI_A_CHG_CHAOS_CANNON_WAIT_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_WALK_FrameMove();
	void CSI_SI_A_CHG_CHAOS_CANNON_WALK_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_JUMP_READY_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_JUMP_UP_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_JUMP_DOWN_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_JUMP_LANDIG_FrameMove();
	void CSI_SI_A_CHG_CHAOS_CANNON_JUMP_LANDIG_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_END_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_GUARD_EventProcess();
	void CSI_SI_A_CHG_CHAOS_CANNON_ATTACK_EventProcess();
#endif
	
	// 본섭 때문에 디파인 밖으로 역시 제작 다되면 원위치로 갈예정  <-- 이거 언제 원위치로 갈거임?
	void ComboZZdownZFrameMove();
	void ComboXXFrameMove();
	void ComboDashJumpZZLandingCannonBallShotFrameMove();
	void CFGComboZZXFrameMove();

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	/// CSI_CTT_COMBO_ZZX (택티컬 트루퍼)
	void CTT_ComboZZXFrameMove();
	void CTT_ComboZZXEventProcess();

	/// CSI_CTT_COMBO_ZZXX (택티컬 트루퍼)
	void CTT_ComboZZXXFrameMove();
	void CTT_ComboZZXXEventProcess();

	void CTT_DashComboXXFrameMove();
	void CTT_DashComboXXEventProcess();

	void CTT_MobileArmoredModeReadyStateStart();
	void CTT_MobileArmoredModeReadyFrameMove();
	void CTT_MobileArmoredModeReadyEventProcess();	
	void CTT_MobileArmoredModeReadyStateEnd();

	void CTT_MobileArmoredModeLoopFowardStateStart();
	void CTT_MobileArmoredModeLoopFowardFrameMove();
	void CTT_MobileArmoredModeLoopFowardEventProcess();
	void CTT_MobileArmoredModeLoopFowardStateEnd();

	void CTT_MobileArmoredModeLoopStateStart();
	void CTT_MobileArmoredModeLoopFrameMove();
	void CTT_MobileArmoredModeLoopEventProcess();
	void CTT_MobileArmoredModeLoopStateEnd();

	void CTT_MobileArmoredModeLoopBackwardStateStart();
	void CTT_MobileArmoredModeLoopBackwardFrameMove();
	void CTT_MobileArmoredModeLoopBackwardEventProcess();
	void CTT_MobileArmoredModeLoopBackwardStateEnd();


	void CTT_MobileArmoredModeUnreadyStateStart();
	void CTT_MobileArmoredModeUnreadyFrameMove();
	void CTT_MobileArmoredModeUnreadyEventProcess();
	void CTT_MobileArmoredModeUnreadyStateEnd();

	void CTT_MobileArmoredModeZStateStartFuture();
	void CTT_MobileArmoredModeZStateStart();
	void CTT_MobileArmoredModeZFrameMove();
	void CTT_MobileArmoredModeZEventProcess();
	void CTT_MobileArmoredModeZStateEnd();

	void CTT_MobileArmoredModeXStateStartFuture();
	void CTT_MobileArmoredModeXStateStart();
	void CTT_MobileArmoredModeXFrameMove();
	void CTT_MobileArmoredModeXEventProcess();
	void CTT_MobileArmoredModeXStateEnd();
#endif SERV_CHUNG_TACTICAL_TROOPER


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// 여기서 부터 ACTIVE, SPECIAL ACTIVE 스킬//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// IRON_CANNON 스페셜 액티브, 디토네이션
	void CSI_SI_SA_CHUNG_DETONATION_START_StartFuture();
	void CSI_SI_SA_CHUNG_DETONATION_START_FrameMove();
	void CSI_SI_SA_CHUNG_DETONATION_START_EventProcess();

	void CSI_SI_SA_CHUNG_DETONATION_LOOP_Init();
	void CSI_SI_SA_CHUNG_DETONATION_LOOP_StartFuture();
	void CSI_SI_SA_CHUNG_DETONATION_LOOP_FrameMove();
	void CSI_SI_SA_CHUNG_DETONATION_LOOP_EventProcess();

	void CSI_SI_SA_CHUNG_DETONATION_END_Init();
	void CSI_SI_SA_CHUNG_DETONATION_END_StartFuture();
	void CSI_SI_SA_CHUNG_DETONATION_END_FrameMove();
	void CSI_SI_SA_CHUNG_DETONATION_END_EventProcess();

	// IRON_CANNON 스페셜 액티브, 루나틱 블로우
	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_START_FrameMove();
	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_START_EventProcess();

	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION_Init();
	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION_FrameMove();
	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION_EventProcess();

	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION_FINISH_Init();
	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION_FINISH_FrameMove();
	void CSI_SI_SA_CHUNG_LUNATIC_BLOW_EXPLOSION_FINISH_EventProcess();

	// IRON_CANNON 액티브, 리로드캐논
	void CSI_SI_A_CHUNG_RELOAD_CANNON_Init();
	void CSI_SI_A_CHUNG_RELOAD_CANNON_Start();
	void CSI_SI_A_CHUNG_RELOAD_CANNON_FrameMove();
	void CSI_SI_A_CHUNG_RELOAD_CANNON_EventProcess();


	//////////////////////////////////////////////
	//{{ 장훈 : 2010-12-15 : Gigantic impact(specialactive)
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_START_Init();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_START_StateStartFuture();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_START_FrameMove();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_START_FrameMoveFuture();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_START_EventProcess();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_START_StateEnd();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_START_StateEndFuture();
	
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_AIRFLY_Init();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_AIRFLY_FrameMove();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_AIRFLY_FrameMoveFuture();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_AIRFLY_EventProcess();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_AIRFLY_StateEnd();

	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_LANDING_Init();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_LANDING_FrameMove();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_LANDING_EventProcess();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_LANDING_Hyper_EventProcess();
	void CSI_SI_SA_CHUNG_GIGANTIC_IMPACT_LANDING_StateEnd();
	//}} 장훈 : 2010-12-15 : Gigantic impact(specialactive)
	///////////////////////////////////////////

	// IRON_CANNON 스페셜 액티브, 브루탈 스윙
	void CSI_SI_SA_CHUNG_BRUTAL_SWING_Init();
	void CSI_SI_SA_CHUNG_BRUTAL_SWING_StartFuture();
	void CSI_SI_SA_CHUNG_BRUTAL_SWING_FrameMove();
	void CSI_SI_SA_CHUNG_BRUTAL_SWING_EventProcess();

	// IRON_CANNON 스페셜 액티브, 스케어 체이스
	void CSI_SI_SA_CHUNG_SCARE_CHASE_Init();
	void CSI_SI_SA_CHUNG_SCARE_CHASE_FrameMove();
	void CSI_SI_SA_CHUNG_SCARE_CHASE_EventProcess();

	// 에이밍 샷	
	void CSI_SI_A_CHUNG_AIMMING_SHOT_Init();
	void CSI_SI_A_CHUNG_AIMMING_SHOT_StateStart();
	void CSI_SI_A_CHUNG_AIMMING_SHOT_FrameMove();
	void CSI_SI_A_CHUNG_AIMMING_SHOT_EventProcess();
	void CSI_SI_A_CHUNG_AIMMING_SHOT_StateEnd();
	
	//{{ oasis907 : 김상윤 [2010.12.18] // 아겔다마
	void CSI_SI_SA_CHUNG_ACELDAMA_Init();
	void CSI_SI_SA_CHUNG_ACELDAMA_StateStart();
	void CSI_SI_SA_CHUNG_ACELDAMA_FrameMove();
	void CSI_SI_SA_CHUNG_ACELDAMA_EventProcess();
	void CSI_SI_SA_CHUNG_ACELDAMA_StateEnd();
	//}} oasis907 : 김상윤 [2010.12.18] // 아겔다마

//{{ kimhc // 2011.1.8 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	// 스틸엣지 (퓨리가디언)
	void CSI_SI_SA_CFG_STEEL_EDGE_Init();
	void CSI_SI_SA_CFG_STEEL_EDGE_FrameMove();
	void CSI_SI_SA_CFG_STEEL_EDGE_EventProcess();

	// 아이언 하울링 (퓨리가디언)
	void CSI_SI_SA_CFG_IRON_HOWLING_Init();
	void CSI_SI_SA_CFG_IRON_HOWLING_FrameMove();
	void CSI_SI_SA_CFG_IRON_HOWLING_EventProcess();

	// 루나틱 퓨리 (퓨리가디언)
	void CSI_SI_SA_CFG_LUNATIC_FURY_READY_Init();
	void CSI_SI_SA_CFG_LUNATIC_FURY_READY_FrameMove();
	void CSI_SI_SA_CFG_LUNATIC_FURY_READY_EventProcess();

	void CSI_SI_SA_CFG_LUNATIC_FURY_RUN_FrameMove();
	void CSI_SI_SA_CFG_LUNATIC_FURY_RUN_EventProcess();

	void CSI_SI_SA_CFG_LUNATIC_FURY_ATTACK_FrameMove();
	void CSI_SI_SA_CFG_LUNATIC_FURY_ATTACK_EventProcess();

	void CSI_SI_SA_CFG_LUNATIC_FURY_BOMBING_FrameMove();
	void CSI_SI_SA_CFG_LUNATIC_FURY_BOMBING_EventProcess();

	void CSI_SI_SA_CFG_LUNATIC_FURY_END_FrameMove();
	void CSI_SI_SA_CFG_LUNATIC_FURY_END_EventProcess();

	// 리프어택 (퓨리가디언)
	void CSI_SI_A_CFG_LEAP_ATTACK_READY_Init();
	void CSI_SI_A_CFG_LEAP_ATTACK_READY_FrameMove();
	void CSI_SI_A_CFG_LEAP_ATTACK_READY_EventProcess();

	void CSI_SI_A_CFG_LEAP_ATTACK_JUMP_Start();
	void CSI_SI_A_CFG_LEAP_ATTACK_JUMP_FrameMove();
	void CSI_SI_A_CFG_LEAP_ATTACK_JUMP_EventProcess();

	void CSI_SI_A_CFG_LEAP_ATTACK_JUMP_DOWN_Start();
	void CSI_SI_A_CFG_LEAP_ATTACK_JUMP_DOWN_FrameMove();
	void CSI_SI_A_CFG_LEAP_ATTACK_JUMP_DOWN_EventProcess();

	void CSI_SI_A_CFG_LEAP_ATTACK_LANDING_Start();
	void CSI_SI_A_CFG_LEAP_ATTACK_LANDING_FrameMove();
	void CSI_SI_A_CFG_LEAP_ATTACK_LANDING_EventProcess();

	// 가드 (퓨리가디언)
	void CSI_SI_A_CFG_GUARD_READY_Init();
	void CSI_SI_A_CFG_GUARD_READY_StartFuture();
	void CSI_SI_A_CFG_GUARD_READY_FrameMove();
	void CSI_SI_A_CFG_GUARD_READY_EventProcess();

	void CSI_SI_A_CFG_GUARD_WAIT_Start();
	void CSI_SI_A_CFG_GUARD_WAIT_FrameMove();
	void CSI_SI_A_CFG_GUARD_WAIT_EventProcess();
	void CSI_SI_A_CFG_GUARD_WAIT_End();

	void CSI_SI_A_CFG_GUARD_END_FrameMove();
	void CSI_SI_A_CFG_GUARD_END_EventProcess();
	void CSI_SI_A_CFG_GUARD_END_End();

	void CSI_SI_A_CFG_GUARD_DAMAGE_Start();
	void CSI_SI_A_CFG_GUARD_DAMAGE_FrameMove();
	void CSI_SI_A_CFG_GUARD_DAMAGE_EventProcess();
	void CSI_SI_A_CFG_GUARD_DAMAGE_End();

	// 듀얼버스터 (슈팅가디언)
	void CSI_SI_SA_CSG_DUAL_BUSTER_Init();
	void CSI_SI_SA_CSG_DUAL_BUSTER_FrameMove();
	void CSI_SI_SA_CSG_DUAL_BUSTER_EventProcess();
	
	void CSI_SI_SA_CSG_DUAL_BUSTER_LANDING_FrameMove();
	void CSI_SI_SA_CSG_DUAL_BUSTER_LANDING_EventProcess();

	// 헤비레일건 (슈팅가디언)
	void CSI_SI_SA_CSG_HEAVY_RAILGUN_Init();
	void CSI_SI_SA_CSG_HEAVY_RAILGUN_FrameMove();
	void CSI_SI_SA_CSG_HEAVY_RAILGUN_EventProcess();

	// 슈팅스타 (슈팅가디언)
	void CSI_SI_SA_CSG_SHOOTING_STAR_READY_Init();
	void CSI_SI_SA_CSG_SHOOTING_STAR_READY_FrameMove();
	void CSI_SI_SA_CSG_SHOOTING_STAR_READY_EventProcess();

	void CSI_SI_SA_CSG_SHOOTING_STAR_ATTACK_Start();
	void CSI_SI_SA_CSG_SHOOTING_STAR_ATTACK_FrameMove();
	void CSI_SI_SA_CSG_SHOOTING_STAR_ATTACK_EventProcess();

	void CSI_SI_SA_CSG_SHOOTING_STAR_END_EventProcess();

	// 럼블샷(슈팅가디언)
	//{{ 장훈 : 2011-01-18 : 액티브 스킬 - 럼블샷/헤드샷
	void CSI_SI_A_CSG_RUMBLE_SHOT_Init();
	void CSI_SI_A_CSG_RUMBLE_SHOT_FrameMove();
	void CSI_SI_A_CSG_RUMBLE_SHOT_EventProcess();

	void CSI_SI_A_CSG_HEAD_SHOT_Init();
	void CSI_SI_A_CSG_HEAD_SHOT_FrameMove();
	void CSI_SI_A_CSG_HEAD_SHOT_EventProcess();
	//}} 장훈 : 2011-01-18 : 액티브 스킬 - 럼블샷/헤드샷
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.8 // 청 1차 전직

//{{ kimhc // 2011-3-16 // 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	// 카밋크래셔(슈팅가디언)
	void CSI_SI_SA_CSG_COMET_CRASHER_READY_Init();
	void CSI_SI_SA_CSG_COMET_CRASHER_READY_FrameMove();
	void CSI_SI_SA_CSG_COMET_CRASHER_READY_EventProcess();

	void CSI_SI_SA_CSG_COMET_CRASHER_LOOP_FrameMove();
	void CSI_SI_SA_CSG_COMET_CRASHER_LOOP_EventProcess();

	void CSI_SI_SA_CSG_COMET_CRASHER_FINISH_FrameMove();
	void CSI_SI_SA_CSG_COMET_CRASHER_FINISH_EventProcess();

	// 샤프슈터신드롬(슈팅가디언)
	void CSI_SI_SA_CSG_SHARPSHOOTER_SYNDROME_Init();
	void CSI_SI_SA_CSG_SHARPSHOOTER_SYNDROME_FrameMove();
	void CSI_SI_SA_CSG_SHARPSHOOTER_SYNDROME_FrameMoveFuture();
	void CSI_SI_SA_CSG_SHARPSHOOTER_SYNDROME_EventProcess();

	// 판데모니움(퓨리가디언)
	void CSI_SI_SA_CFG_PANDEMONIUM_Init();
	void CSI_SI_SA_CFG_PANDEMONIUM_FrameMove();
	void CSI_SI_SA_CFG_PANDEMONIUM_EventProcess();

	// 칼라볼그페인(퓨리가디언)
	void CSI_SI_SA_CFG_PAIN_OF_CALADBOLG_Init();
	void CSI_SI_SA_CFG_PAIN_OF_CALADBOLG_FrameMove();
	void CSI_SI_SA_CFG_PAIN_OF_CALADBOLG_EventProcess();
#endif	CASH_SKILL_FOR_CHUNG_2011_04
//}} kimhc // 2011-3-16 // 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
	// oasis907 : 김상윤 [2011.6.16] 청 2차 전직 - 아이언 팔라딘
	void CSI_SI_A_CIP_SUDDEN_BURSTER_Init();
	void CSI_SI_A_CIP_SUDDEN_BURSTER_FrameMoveFuture();
	void CSI_SI_A_CIP_SUDDEN_BURSTER_FrameMove();
	void CSI_SI_A_CIP_SUDDEN_BURSTER_EventProcess();
	
	void CSI_SI_SA_CIP_BURST_WOLF_Init();
	void CSI_SI_SA_CIP_BURST_WOLF_StateStart();
	void CSI_SI_SA_CIP_BURST_WOLF_FrameMove();
	void CSI_SI_SA_CIP_BURST_WOLF_EventProcess();
	void CSI_SI_SA_CIP_BURST_WOLF_StateEnd();


	void CSI_SI_SA_CIP_LAND_DEMOLISHER_START_StartFuture();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_START_FrameMoveFuture();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_START_FrameMove();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_START_EventProcess();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_START_EndFuture();

	void CSI_SI_SA_CIP_LAND_DEMOLISHER_DOWN_StartFuture();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_DOWN_FrameMoveFuture();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_DOWN_FrameMove();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_DOWN_EventProcess();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_DOWN_EndFuture();

	void CSI_SI_SA_CIP_LAND_DEMOLISHER_LANDING_Init();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_LANDING_StateStart();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_LANDING_FrameMove();
	void CSI_SI_SA_CIP_LAND_DEMOLISHER_LANDING_EventProcess();


#endif CHUNG_SECOND_CLASS_CHANGE




#ifdef CHUNG_SECOND_CLASS_CHANGE
	//{{ JHKang / 강정훈 / 2011.6.19 / 레그 샷
	void CSI_SI_A_CDC_LEG_SHOT_Init();
	void CSI_SI_A_CDC_LEG_SHOT_Start();
	void CSI_SI_A_CDC_LEG_SHOT_FrameMove();
	void CSI_SI_A_CDC_LEG_SHOT_EventProcess();
	//}}
	//{{ JHKang / 강정훈 / 2011.6.20 / 아틸러리 스트라이크
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_READY_Init();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_READY_Start();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_READY_FrameMove();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_READY_EventProcess();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_StateEnd();
	
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_EventProcess();

	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_IDLE_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_STRIKE_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_STRIKE_StateEnd();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_LEFT_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_RIGHT_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_UP_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_DOWN_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_UP_LEFT_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_UP_RIGHT_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_DOWN_LEFT_StateStart();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CONTROL_DOWN_RIGHT_StateStart();
	
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_FIRE_Start();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_FIRE_FrameMove();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_FIRE_EventProcess();

	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CANCEL_Start();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_CANCEL_EventProcess();

	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_END_EventProcess();

	void ArtilleryStrikeControl_EventProcess();
	//}}

	//{{ JHKang / 강정훈 / 2011.7.1 / 불렛 블릿치
	void CSI_SI_SA_CDC_BULLET_BLITZ_Init();
	void CSI_SI_SA_CDC_BULLET_BLITZ_StateStart();
	void CSI_SI_SA_CDC_BULLET_BLITZ_FrameMove();
	void CSI_SI_SA_CDC_BULLET_BLITZ_EventProcess();
	#ifdef BALANCE_DEADLY_CHASER_20130214
	void CSI_SI_SA_CDC_BULLET_BLITZ_HYPER_StateStart();
	void CSI_SI_SA_CDC_BULLET_BLITZ_HYPER_FrameMove();
	#endif //BALANCE_DEADLY_CHASER_20130214
	//}}
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	void CSI_SI_SA_CTT_CROSS_FIRE_Init();
	void CSI_SI_SA_CTT_CROSS_FIRE_StartFuture();
	void CSI_SI_SA_CTT_CROSS_FIRE_FrameMove();
	void CSI_SI_SA_CTT_CROSS_FIRE_EventProcess();
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 오현빈
	// 임팩트 디토네이션
	void CSI_SA_CHUNG_IMPACT_DETONATION_READY_StartFuture();
	void CSI_SA_CHUNG_IMPACT_DETONATION_READY_EventProcess();

	void CSI_SA_CHUNG_IMPACT_DETONATION_LOOP_Init();
	void CSI_SA_CHUNG_IMPACT_DETONATION_LOOP_StartFuture();
	void CSI_SA_CHUNG_IMPACT_DETONATION_LOOP_FrameMove();
	void CSI_SA_CHUNG_IMPACT_DETONATION_LOOP_EventProcess();

	void CSI_SA_CHUNG_IMPACT_DETONATION_FINAL_StartFuture();
	void CSI_SA_CHUNG_IMPACT_DETONATION_FINAL_FrameMove();
	void CSI_SA_CHUNG_IMPACT_DETONATION_FINAL_EventProcess();

	// 아틸러리 노바
	void CSI_A_CIP_ARTILLERY_NOVA_CHARGE_Init();
	void CSI_A_CIP_ARTILLERY_NOVA_CHARGE_StartFuture();
	void CSI_A_CIP_ARTILLERY_NOVA_CHARGE_FrameMove();
	void CSI_A_CIP_ARTILLERY_NOVA_CHARGE_EventProcess();

	void CSI_A_CIP_ARTILLERY_NOVA_FIRE_FrameMove();
	void CSI_A_CIP_ARTILLERY_NOVA_FIRE_EventProcess();

	// 판데모니움 피어
	void CSI_SI_SA_CFG_PANDEMONIUM_FEAR_Init();
	void CSI_SI_SA_CFG_PANDEMONIUM_FEAR_FrameMove();
	void CSI_SI_SA_CFG_PANDEMONIUM_FEAR_EventProcess();

	
	// 랜드 디몰리셔 - 어스 퀘이크
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_START_StartFuture();
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_START_FrameMoveFuture();
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_START_FrameMove();
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_START_EventProcess();
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_START_EndFuture();

	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_DOWN_StartFuture();
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_DOWN_EventProcess();

	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_LANDING_Init();
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_LANDING_FrameMove();
	void CSI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE_LANDING_EventProcess();

	// 더블 파이어
	void CSI_A_CSG_DOUBLE_FIRE_Init();
	void CSI_A_CSG_DOUBLE_FIRE_FrameMove();
	void CSI_A_CSG_DOUBLE_FIRE_EventProcess();

	// 일래스틱 밤 - 그리네이드 런처
	void CSI_A_CHG_ELASTIC_BOMB_LAUNCHER_Init ();
	void CSI_A_CHG_ELASTIC_BOMB_LAUNCHER_FrameMove();
	void CSI_A_CHG_ELASTIC_BOMB_LAUNCHER_EventProcess();

#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈

	// 매그넘 샷
	void CSI_A_CSG_MAGNUM_SHOT_Init ();
	void CSI_A_CSG_MAGNUM_SHOT_FrameMove ();
	void CSI_A_CSG_MAGNUM_SHOT_EventProcess ();

	// 래피드 캐논
	void CSI_A_CDC_RAPID_CANNON_READY_StartFuture();
	void CSI_A_CDC_RAPID_CANNON_READY_Start();
	void CSI_A_CDC_RAPID_CANNON_READY_EventProcess();

	void CSI_A_CDC_RAPID_CANNON_LOOP_Init();
	void CSI_A_CDC_RAPID_CANNON_LOOP_StartFuture();
	void CSI_A_CDC_RAPID_CANNON_LOOP_FrameMove();
	void CSI_A_CDC_RAPID_CANNON_LOOP_EventProcess();

	void CSI_A_CDC_RAPID_CANNON_FINAL_StartFuture();
	void CSI_A_CDC_RAPID_CANNON_FINAL_FrameMove();
	void CSI_A_CDC_RAPID_CANNON_FINAL_EventProcess();


	/// 아틸러리 스트라이크 퀀텀 발리스타, Start 만 있음
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_QUANTUM_BALLISTA_READY_Init();
	void CSI_SI_SA_CDC_ARTILLERY_STRIKE_QUANTUM_BALLISTA_READY_Start();

	/// 택티컬 필드 레이드
	void CSI_SA_CTT_TACTICAL_FIELD_RAID_Init();
	void CSI_SA_CTT_TACTICAL_FIELD_RAID_FrameMove();
	void CSI_SA_CTT_TACTICAL_FIELD_RAID_EventProcess();

	/// 지원 폭격
	void CSI_A_CTT_BOMBARD_SERVICE_Init ();
	void CSI_A_CTT_BOMBARD_SERVICE_FrameMove ();
	void CSI_A_CTT_BOMBARD_SERVICE_EventProcess ();


	// 아틸러리 스트라이크 변경, 기존 Anim 에서 카펫 바밍과 같은 Style 로 변경
	void DoArtilleryStrikeFire();		// 아틸러리 스트라이크 미사일 발사!
	void ResetArtilleryStrikeData ();	// ArtilleryStike 관련 데이터를 초기화 시킴
										// 쿨타임, 유지 시간 등은 스킬 템플릿에서 얻어오므로
										// 실제 사용되는 코드만 초기화 
	// 아틸러리 스트라이크 데이터 설정
	void SetArtilleryStrikeData ( CHUNG_ARTILLERY_STIKE_TYPE eType_ );		
#endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
	//{{ kimhc // 2011-04-13 // 청 가드 버그 테스트
#ifdef	GUARD_BUG_TEST
	//virtual void RenderName();
	UINT m_uiCountToCallSetDamageReducePercentFuncInCommonStateStart;
	UINT m_uiCountToCallSetDamageReducePercentFuncInCommonStateEnd;	
	wstring m_wstrFunctionName;
	bool m_bVerifyGuardSystem;
#endif	GUARD_BUG_TEST
	//}} kimhc // 2011-04-13 // 청 가드 버그 테스트

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	/// 박격포(택티컬 커맨더)
	void CSI_A_CTT_REMOTE_MORTAR_Init();
	void CSI_A_CTT_REMOTE_MORTAR_StateStart();			
	void CSI_A_CTT_REMOTE_MORTAR_FrameMove();
	void CSI_A_CTT_REMOTE_MORTAR_EventProcess();

	/// 크로스 파이어(택티컬 커맨더)

	/// 택티컬 필드(택티컬 커맨더)
	void CSI_SA_CTT_TACTICAL_FIELD_Init();
	void CSI_SA_CTT_TACTICAL_FIELD_StateStart();			
	void CSI_SA_CTT_TACTICAL_FIELD_FrameMove();
	void CSI_SA_CTT_TACTICAL_FIELD_EventProcess();

#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef FINALITY_SKILL_SYSTEM //JHKang
#pragma region SI_FS_CIP_DOOM_STRIKER
	void CSI_HA_CIP_DOOM_STRIKER_Init();
#pragma endregion 둠 스트라이커 - 궁극기

#pragma region SI_FS_CDC_OUTRAGE_STRIKE
	void CSI_HA_CDC_OUTRAGE_STRIKE_Init();
#pragma endregion 아웃 레이지 스트라크 - 궁극기
#endif //FINALITY_SKILL_SYSTEM

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
	// 새틀라이트 레인
	void CSI_HA_CTT_SATELITE_RAIN_Init ();
	void CSI_HA_CTT_SATELITE_RAIN_FrameMove();		
	void CSI_HA_CTT_SATELITE_RAIN_EventProcess();

	void DoSateliteRain ();		// 세틀라이트 레인 실행 함수
	void ResetSateliteRain ();	// 세틀라이트 레인 관련 데이터 초기화
	void SateliteRainFrameMove ();	// 세틀라이트 레인, CommonFrameMove 
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템



	virtual void ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	void ChaosCannonAttackStateChange ( int iOffset_ );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	
#ifdef ADD_MEMO_1ST_CLASS //김창한
	virtual float GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const;
#endif //ADD_MEMO_1ST_CLASS

protected:

	CKTDGParticleSystem::CParticleEventSequenceHandle		m_ahChungMajorParticleInstance[CHUNG_MAJOR_PII_END];
	CKTDGParticleSystem::CParticleEventSequenceHandle		m_ahChungMinorParticleInstance[CHUNG_MINOR_PII_END];
	///////////////////////////////////////////////////////
	//{{ 장훈 : 2011-01-15 : 청이 쓰는 메이져 메쉬 인스턴스
	CKTDGXMeshPlayer::CXMeshInstanceHandle					m_ahChungMajorMeshInstance[CHUNG_MAJOR_MII_END];
	//}} 장훈 : 2011-01-15 : 청이 쓰는 메이져 메쉬 인스턴스
	///////////////////////////////////////////////////////

	CGuardSystem			m_GuardSystem;			/// 가드 시스템에 대한 정보
	CCannonBallSystem		m_CannonBallSystem;		/// 캐논볼 시스템에 대한 정보

	vector<GUARD_STATE_INFO> m_vecStateIDCanBeChangedToGuardState;	/// 가드 스테이트로 넘어갈 수 있는 스테이트 ID 리스트

	vector<CX2EqipPtr> m_vecPromotionEquipList;		/// 외형 장비 리스트로 각성 구슬 3개를 소모한 각성을 했을때 보여진다.
	vector<int> m_vecPromotionItemIDList;			/// 프로모션 장비의 ID 리스트
	bool		m_bBerserkMode;						/// 최대 갯수의 각성 구슬을 가지고 각성한 상태인가?(광폭화)		
	const int	MAX_ANIMATION_LOOP_COUNT_AT_THIS_STATE;	/// 현재 상태에서 최대로 Loop 될 수있는 애니메이션 횟수
	UCHAR		m_ucCannonBallCountToBeUpUsingReLoadCannon;	/// 리로드 캐논에 의해서 충전될 캐논볼의 갯수 (리로드캐논은 기본 스킬이므로 모두 가지고 있음)
	float		m_fDamageRelByCannonBallMastery;	/// 리로드 마스터리에 의해서 증가할 리로드캐논의 데미지 배율
	UCHAR		m_ucBasicCannonBallCount;			/// 기본 캐논볼 갯수

	bool		m_bAimingTarget;
	D3DXVECTOR3 m_vAimingTargetPos;
	D3DXVECTOR3 m_vAimingTargetLandPos;

	//{{ kimhc // 2011.1.11 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	CFuryGuardianGuardSkillPtr	m_GuardForFuryGuardianPtr;		/// 청 1차 전직, 퓨리 가디언의 Guard 스킬
	float						m_fForceDownRelAtMeleeAttack;	/// 근접 공격인 경우 패시브 스킬등에 의한 ForceDown 수치 조정
	float						m_fProbabilityBigDamageAtGuardMastery;		/// 가드마스터리에서 BigDamage를 줄 확률	
	float						m_fIncreaseRangeSubWeaponShot;	/// 청 1차 전직, 슈팅 가디언의 서브웨폰 사거리 증가율(강선개조 패시브)
	float						m_fCriticalDamageRate;			/// 청 1차 전직, 슈팅 가디언의 강선개조에 의한 Critical 시 데미지 증가
#endif	CHUNG_FIRST_CLASS_CHANGE
	//}} kimhc // 2011.1.11 // 청 1차 전직

#ifdef BALANCE_PATCH_20110303
	int							m_iCountbyCsgComboDashJumpXXMiniGun;
#endif BALANCE_PATCH_20110303

#ifdef CHUNG_SECOND_CLASS_CHANGE
	float						m_fProbabilityIronWill;
	bool						m_bApplyLandDemolisher;
	CX2EffectSet::Handle		m_hLandDemolisherAfterEffect;
	float						m_fLandDemolisherPowerRate;
	float						m_fRemainIronWillRenderEffectTime;
	int							m_iDebuffTransitNum;
#endif CHUNG_SECOND_CLASS_CHANGE


#ifdef CHUNG_SECOND_CLASS_CHANGE
	CKTDXTimer	m_TimerCheckInvalidShot;		/// 아틸러리 스트라이크 타이머
	//CX2GUNPC*	m_pShot;						/// 포격 커서 NPC
	int			m_iShot;						/// 포격 커서 NPC UID
	bool		m_bIsSummoningShot;				/// 소환 확인
	CKTDXTimer	m_TimerShotControl;				/// Shot 타이머
	short		m_iArtilleryStrikePlayCount;	/// 포격 개수
	D3DXVECTOR3	m_vLastShotPos;					/// 마지막 위치
	D3DXVECTOR3 m_vLastShotDir;					/// 마지막 방향 벡터

	int			m_sBulletCount;					/// 불렛 블릿츠 탄환 개수
	
	float		m_fDecreaseRate;				/// 강렬한 쇼타임 감소율(MP, 쿨타임)
	CKTDXTimer	m_timerSummoned;				/// 소환된 몬스터 시간
	float		m_fSummonedLifeTime;			/// 소환된 몬스터들 살아있는 시간
#endif

#ifdef SKILL_BALANCE_20110728
	bool		m_bIncreaseCritical;			/// 강선 개조에 의한 크리티컬 적용 여부
#endif

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	int			m_iSiegeShellingCount;
	float		m_fSiegeShellingDistance;
	float		m_fSiegeShellingDamageRel;
	float		m_fSiegeShellingUserBallRate;
	float		m_fInsertCannonBallRel;
	float		m_fBeShotCannonBallRel;
	float		m_fBeShotCannonBallRelTime;
	float		m_fWithStandingRate;
	float		m_fWithStandingPowerRate;
	int			m_iChargeCannonBall;
	int			m_iDreadChaseCount;
	int			m_iCarpetBombingCount;
	int			m_iCarpetBombingSmallCount;
	float		m_fCarpetBombingTime;
	bool		m_bCarpetBombing;
	D3DXVECTOR3	m_vCarpetBombingPos;
	D3DXVECTOR3 m_vCarpetBombingDir;
	D3DXVECTOR3 m_vCarpetBombingRot;
	float		m_fChaosCannonTime;
	CX2EffectSet::Handle		m_hChaosCannonEffect;	
	CKTDXDeviceSound	*m_pChaosCannonSound;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	bool		m_bMobileArmoredModeEnable;
	bool		m_bMobileArmoredModeXFired;
	bool		m_bInStateMobileArmoredMode;
	int			m_iExtraCannonBallReloadCount;

	float		m_fMobileArmoredModeDistance;
	float		m_fMobileArmoredModeDamageRel;

	float		m_fMobileArmoredModeStepDustElapsedTime;
	float		m_fMobileArmoredModeFireXCoolTime;
	float		m_fMobileArmoredModeFireZCoolTime;
	CX2EffectSet::Handle	m_hMobileArmoredModeEffect;	

	UidType		m_iRemoteMortarNPCUID;			/// 설치된 박격포 아이디
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef BALANCE_DEADLY_CHASER_20130214
	float		m_fCannonBallExtraChargeRate;	/// 캐논볼 추가 장전 확률
	float		m_fMiniGunHeadShotRate;			/// 권총 공격에 헤드샷 발동 확률
#endif //BALANCE_DEADLY_CHASER_20130214


#ifdef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
	CHUNG_ARTILLERY_STIKE_TYPE		m_eArtilleryStrikeType;		// 아틸러리 스트라이크 변경, 사용하겠는가? 타입은?	
	float		m_fRateGasPistonSystemAutoRecharge;				// 가스 피스톤 시스템, 캐논볼 사용 시 자동 장전 확률
	int			m_iArtilleryStrikeNowPlayCount;					// 아틸러리 스트라이크, 현재 몇개째?
	CKTDXTimer	m_TimerCheckArtilleryStrikeFire;				// 아틸러리 스트라이크 발사 타이머
	bool		m_bArtilleryStrikeFire;							// 아틸러리 스트라이크 발사?
	float		m_fArtilleryStrikePowerRate;					// 아틸러리 스트라이크, 쏘고 나서 1초 후 데미지 이펙트가 떨어져 PowerRate 를 받아오지 못함으로 
																// PowerRate 를 따로 저장해둔다.
	int			m_iBombardServiceTarget;						// 지원 폭격 타켓 UID
	CX2EffectSet::Handle	 m_hBombardServiceEffect;			// 지원 폭격 대미지 이펙트
	int			m_iFireCount;									// 발사 횟수
	float		m_fChargeTime;									// 차지 시간
	float		m_fPassiveCriticalRateChung;					// 크리티컬 증가값
#endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
	CX2EffectSet::Handle			m_hSateliteRainBigMissileSummonEffect;		// 새틀라이트 레인 이펙트
	CKTDXTimer						m_TimerCheckSateliteRain;	// 새틀라이트 레인 이펙트 타이머
	bool							m_bSateliteRainFire;		// 새틀라이트 레인 발사?
	float							m_fSateliteRainPowerRate;	// 새틀라이트 레인 쏘고 나서 1초 후 데미지 이펙트가 떨어져 PowerRate 를 받아오지 못함으로 
																// PowerRate 를 따로 저장해둔다.
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle   		m_hCEffectSateliteRain;		// 새틀라이트 레인 이펙트
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect*		m_pCEffectSateliteRain;		// 새틀라이트 레인 이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	double							m_fSateliteRainPhase;		// 새틀라이트 레인 진행 중인 상태, 1초 마다 한번씩
	bool							m_bIsFireSateliteRainBigMissile; // 새틀라이트 레인, 큰 미사일 발사 했는가?
	
	D3DXVECTOR3						m_vSateliteRainSummonMagicZonePosition;		// 새틀라이트 레인, 큰 미사일 발사하기 전 마법진 좌표
	D3DXVECTOR3						m_vSateliteRainSummonMagicZoneRotate;		// 새틀라이트 레인, 마법진 회전값
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	float		m_fArtilleryStrikeCreateDamageEffectGapTime;				// 아틸러리 스트라이크 미슬 샤워의 데미지 이펙트 생성 시간 Gap, 미슬 샤워 오류 수정으로 인해 추가됨
	float		m_fSiegeShellingIncraseMPRel;									// 탄두 개조 패시브, MP 증가량 추가
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편


};

IMPLEMENT_INTRUSIVE_PTR(  CX2GUChung::CFuryGuardianGuardSkill );

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG

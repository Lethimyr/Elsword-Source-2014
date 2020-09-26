#pragma once


#ifdef NEW_CHARACTER_EL

class CX2GUEL : public CX2GUUser
{
public:
	enum EL_STATE_ID
	{
		//전 캐릭터 공통 스테이트
		ESI_BASE = USI_END + 1,
		ESI_STANDUP_ROLLING_FRONT_FRONT,
		ESI_STANDUP_ROLLING_FRONT_BACK,
		ESI_STANDUP_ROLLING_BACK_FRONT,
		ESI_STANDUP_ROLLING_BACK_BACK,
		ESI_STANDUP_ATTACK_FRONT,
		ESI_STANDUP_ATTACK_BACK,
		ESI_STANDUP_ATTACK_FRONT_NO_DOWN,
		ESI_STANDUP_ATTACK_BACK_NO_DOWN,
		ESI_DASH_JUMP_POWER_LANDING,
		ESI_WALL_LANDING,

		//EL 스테이트
		///////////////////////////
		ESI_COMBO_Z,			/// 콤보 Z
		ESI_COMBO_ZZ,			/// 콤보 ZZ
		ESI_COMBO_ZZZ,			/// 콤보 ZZZ
		ESI_COMBO_ZZZZ,			/// 콤보 ZZZZ
		ESI_COMBO_ZZZX,			/// 콤보 ZZZX

		ESI_COMBO_X,			/// 콤보 X
		ESI_COMBO_XX,			/// 콤보 XX
		ESI_COMBO_XXX,			/// 콤보 XXX
		ESI_COMBO_XXZ,			/// 콤보 XXZ

		ESI_JUMP_ATTACK_Z,		/// 점프 공격 Z
		ESI_JUMP_ATTACK_X,		/// 점프 공격 X

		ESI_DASH_COMBO_Z,		/// 대쉬 콤보 Z
		ESI_DASH_COMBO_ZZ,		/// 대쉬 콤보 ZZ

		ESI_DASH_COMBO_X,		/// 대쉬 콤보 X

		ESI_DASH_JUMP_COMBO_Z,	/// 대쉬 점프 콤보 Z
		ESI_DASH_JUMP_COMBO_ZZ,	/// 대쉬 점프 콤보 ZZ
		ESI_DASH_JUMP_COMBO_ZZZ,/// 대쉬 점프 콤보 ZZZ

		ESI_DASH_JUMP_COMBO_X,	/// 대쉬 점프 콤보 X
		ESI_DASH_JUMP_COMBO_XX, /// 대쉬 점프 콤보 XX
		ESI_DASH_JUMP_COMBO_XX_LANDING, /// 대쉬 점프 콤보 XX Landing

		ESI_QUICK_STANDUP_ATTACK_FRONT,	// 기회의 포착 STAND_FRONT
		ESI_QUICK_STANDUP_ATTACK_BACK,	// 기회의 포착 STAND_BACK 


		ESI_CAN_NOT_ATTACK_JUMP_DOWN,		/// 공격 할 수 없는 점프 다운 추가
		///////////////////////////			
		
		ESI_SWORD_WAIT,			/// 검의 길 대기

/////////////////////////////////////////////////////////////////////////////////////////////////
// 나이트
		ESI_SI_A_EL_DODGE_AND_SLASH,			/// 닷지&슬래시
		ESI_SI_A_EL_DODGE_AND_SLASH_SECOND,		/// 닷지&슬래시 2회 입력
		ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z,	/// 닷지&슬래시 Z공격

		ESI_SI_A_EL_LEAP_CRASH,					// 도약
		ESI_SI_A_EL_LEAP_CRASH_JUMP,
		ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING,

		ESI_SI_A_EL_RUSHING_SWORD,				/// 러싱 소드
		ESI_SI_A_EL_RUSHING_SWORD_2nd,
		ESI_SI_A_EL_RUSHING_SWORD_3rd,

		ESI_SI_A_EL_KICK,						/// 발차기

		ESI_SI_A_EL_MEGA_SLASH,					/// 메가 슬래시

		ESI_SI_A_EL_SONIC_ASSAULT_STAB,			/// 소닉 어썰트 - 스탭
		ESI_SI_A_EL_SONIC_ASSAULT_STING,		/// 소닉 어썰트 - 스팅

		ESI_SI_SA_EL_WILD_SHOCK,				/// 와일드 쇼크

		ESI_SI_SA_EL_POWER_BURSTER,				/// 파워 버스터

		ESI_SI_SA_EL_MEGA_BURSTER,				/// 메가 버스터

		ESI_SI_SA_EL_UNLIMITED_BLADE,			/// 언리미티드 블레이드
/////////////////////////////////////////////////////////////////////////////////////////////////
// 1차 공용
		ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE,		/// 극기 - 강(强)
		ESI_SA_A_EL_ENDURANCE_POWER_ACTIVE_MEMO,/// 극기 - 강(强) 메모

/////////////////////////////////////////////////////////////////////////////////////////////////
// 세이버 나이트

		ESI_DASH_COMBO_SSK_ZZ,					/// 대시 콤보 ZZ
		ESI_DASH_COMBO_SSK_ZZZ,					/// 대시 콤보 ZZZ
		ESI_DASH_COMBO_SSK_ZZX,					/// 대시 콤보 ZZX
		ESI_DASH_JUMP_COMBO_SSK_XX,				/// 대시 점프 콤보 XX
		ESI_DASH_JUMP_COMBO_SSK_XXX,			/// 대시 점프 콤보 XXX

		ESI_SI_A_SSK_CROSS_SLASH,				/// 크로스 슬래시

		ESI_SI_A_SSK_POWER_BREAK,				/// 파워 브레이크
		ESI_SI_A_SSK_HEAVY_STUNNER,				/// 헤비 스터너

		ESI_SI_SA_SSK_SPIRAL_BLAST,				/// 스파이럴 블래스트
		
		ESI_SI_SA_SSK_EXTINCTION_START,			/// 세이버 - 익스팅션
		ESI_SI_SA_SSK_EXTINCTION_CHARGE,		
		ESI_SI_SA_SSK_EXTINCTION_ATTACK,
		ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP,
		ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN,

		ESI_SI_SA_SSK_VICTORIOUS_SWORD,			/// 승리의 검

		ESI_SI_SA_SSK_JUDGEMENT_DRIVE,			/// 저지먼트 드라이브

/////////////////////////////////////////////////////////////////////////////////////////////////
// 파이로 나이트

		ESI_DASH_COMBO_SPK_ZX,					/// 대시 콤보 ZX
		ESI_DASH_COMBO_SPK_ZZ,					/// 대시 콤보 ZZZ
		ESI_DASH_COMBO_SPK_ZZZ,					/// 대시 콤보 ZZZ
		ESI_DASH_JUMP_COMBO_SPK_XZ,				/// 대시 점프 콤보 XZ
		ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK,	/// 대시 콤보 ZX 추가 공격

		ESI_SI_A_SPK_SWORD_ERUPTION,			/// 소드 이럽션

		ESI_SI_A_SPK_BIG_BURST,					/// 대폭살

		ESI_SI_SA_SPK_BURST_RISING,				/// 버스트 라이징
		
		ESI_SI_SA_SPK_BURST_WAVE,				/// 버스트 웨이브

		ESI_SI_SA_SPK_SWORDFIRE,				/// 소드 파이어

		ESI_SI_SA_SPK_INFERNAL_BLADE,			/// 인페르날 블레이드

		ESI_SI_SA_SPK_ETERNAL_FIRE,				/// 이터널 파이어

/////////////////////////////////////////////////////////////////////////////////////////////////
// 그랜드 마스터

// #ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터
		// 커맨드
		ESI_DASH_JUMP_COMBO_SGM_XZ,					/// 대시 점프 콤보 XZ
		ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP,			/// 대시 점프 콤보 XZ 다운
		ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN,			/// 대시 점프 콤보 XZ 다운

		ESI_COMBO_SGM_XX_DOWN_X_START,				/// 콤보 XX 다운 X 시작
		ESI_COMBO_SGM_XX_DOWN_X_LOOP,				/// 콤보 XX 다운 X 루프
		ESI_COMBO_SGM_XX_DOWN_X_END,				/// 콤보 XX 다운 X 끝

		ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED,		/// 상급 기사 숙련, 대미지를 입어 앞으로 뒤를 때 공격 입력 시, 해당 State 로 전환
		ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED,	/// 상급 기사 숙련, 대미지를 입어 뒤로 뒤를 때 공격 입력 시, 해당 State 로 전환
		
		// 액티브
		ESI_A_SGM_PROVOKE,							/// 도발 시작 
		ESI_A_SGM_PROVOKE_REVENGE,					/// 도발 성공 후 반격

		ESI_A_SGM_DRASTIC_CHARGE_START,				/// 드래스틱 차지 시작 
		ESI_A_SGM_DRASTIC_CHARGE_LOOP,				/// 드래스틱 차지 루프
		ESI_A_SGM_DRASTIC_CHARGE_END,				/// 드래스틱 차지 끝 

		ESI_A_SGM_SNAP_CRASHER_START,					/// 스냅 크래셔, 공중에서 사용
		ESI_A_SGM_SNAP_CRASHER_LOOP,					/// 스냅 크래셔, 공중에서 사용 후 내려옴
		ESI_A_SGM_SNAP_CRASHER_END,						/// 스냅 크래셔, 공중에서 사용 후 내려와서 땅 찍기
		ESI_A_SGM_SNAP_CRASHER_GROUND,					/// 스냅 크래셔, 지상에서 사용

		// 스페셜 액티브
		ESI_SA_SGM_SONIC_BLADE,						/// 소닉 블레이드

		ESI_SA_SGM_CRITICAL_DIVE,					/// 크리티컬 다이브
		ESI_SA_SGM_CRITICAL_DIVE_HIT,				/// 크리티컬 다이브 사용 중 적이 맞았을 때

		ESI_SA_SGM_JUGGERNAUT_BUSTER_START,			/// 저거넛 버스터, 시작
		ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP,			/// 저거넛 버스터, 루프
		ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST,		/// 저거넛 버스터, 마지막 루프
		ESI_SA_SGM_JUGGERNAUT_BUSTER_END,			/// 저거넛 버스터, 끝

		ESI_SA_SGM_WAR_PRELUDE,						/// 전장의 숨결

		// 하이퍼 액티브
		ESI_HA_SGM_SWORD_OF_RELICS,					/// 소드 오브 렐릭스	

// #endif // SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터

/////////////////////////////////////////////////////////////////////////////////////////////////
// 블레이징 하트

		//커맨드
		ESI_COMBO_SBH_XZ,							/// 콤보 XZ
		ESI_COMBO_SBH_XZZ,							/// 콤보 XZZ
		ESI_COMBO_SBH_XZZZ,							/// 콤보 XZZZ
		ESI_DASH_COMBO_SBH_X,						/// 대시 콤보 X
		ESI_DASH_COMBO_SBH_XX,						/// 대시 콤보 XX
		ESI_DASH_COMBO_SBH_XXX,						/// 대시 콤보 XXX
		ESI_DASH_COMBO_SBH_XXXZ,					///대시 콤보 X 중 Z

		//액티브
		ESI_SI_A_SBH_FLAME_SWORD,					/// 홍련의 검
		ESI_SI_A_SBH_FLAME_SWORD_FINISH,			/// 홍련의 검 연결동작
		ESI_SI_A_SBH_FIREWORKS,						/// 불놀이
		ESI_SI_A_SBH_SCALET_ROSE,					/// 불꽃 장미
		ESI_SI_A_SBH_SCALET_ROSE_SUCCESS,			/// 불꽃 장미 타격 성공

		//스폐셜 액티브
		ESI_SI_SA_SBH_SCALDIC_SWORD,				/// 스칼딕 소드
		ESI_SI_SA_SBH_WALL_OF_PROMINENCE,			/// 불꽃의 결계
		ESI_SI_SA_SBH_BLAZE_WING,					/// 블레이즈 윙
		ESI_SI_SA_SBH_BLAZING_DANCE_START,			/// 블레이징 댄스 시작
		ESI_SI_SA_SBH_BLAZING_DANCE_1ST,			/// 블레이징 댄스 첫번째 타격
		ESI_SI_SA_SBH_BLAZING_DANCE_2ND,			/// 블레이징 댄스 두번째 타격
		ESI_SI_SA_SBH_BLAZING_DANCE_3RD,			/// 블레이징 댄스 세번째 타격
		ESI_SI_SA_SBH_BLAZING_DANCE_4TH,			/// 블레이징 댄스 네번째 타격
		ESI_SI_SA_SBH_BLAZING_DANCE_FINISH,			/// 블레이징 댄스 마지막 타격

		//하이퍼 액티브
		ESI_SI_HA_SBH_ANCIENT_FIRE,					/// 에인션트 파이어

/////////////////////////////////////////////////////////////////////////////////////////////////

	};
	class CFirstStrikeData							// 엘리시스 그랜드 마스터 패시브, 선수 필승 관련 데이터
	{
		private :
			float	m_fFirstStrikeAddStopTimeDef;			// 엘리시스 그랜드 마스터 패시브, 선수 필승 ( 커맨드 경직 시간 증가 ) 
			float   m_fFirstStrikeAddDamage;				// 엘리시스 그랜드 마스터 패시브, 선수 필승 ( 커맨드 데미지 증가 ) 
			float   m_fFirstStrikeAddIncreaseMPRate;		// 엘리시스 그랜드 마스터 패시브, 선수 필승 ( 커맨드 MP 회수율 증가) 
		public :
			CFirstStrikeData ( void ) : m_fFirstStrikeAddStopTimeDef ( 0 ),	
				m_fFirstStrikeAddDamage ( 0 ),	m_fFirstStrikeAddIncreaseMPRate ( 0 ) {}

			CFirstStrikeData ( const float fAddStopTime_,  const float fAddDamage_, const float fAddMPRate_ ) :
				m_fFirstStrikeAddStopTimeDef ( fAddStopTime_ ), m_fFirstStrikeAddDamage ( fAddDamage_ ), m_fFirstStrikeAddIncreaseMPRate ( fAddMPRate_ ) {}					

			void SetFirstStrikeData ( const float fAddStopTime_, const float fAddDamage_, const float fAddMPRate_ )
			{
				m_fFirstStrikeAddStopTimeDef	 = fAddStopTime_;
				m_fFirstStrikeAddDamage			 = fAddDamage_;
				m_fFirstStrikeAddIncreaseMPRate	 = fAddMPRate_;
			}

			float GetFirstStrikeAddStopTimeDef ( void ) const		{ return m_fFirstStrikeAddStopTimeDef; }
			float GetFirstStrikeAddDamage ( void ) const			{ return m_fFirstStrikeAddDamage > 0 ? m_fFirstStrikeAddDamage : 1.f; }
			float GetFirstStrikeAddIncreaseMPRate ( void ) const	{ return m_fFirstStrikeAddIncreaseMPRate > 0 ? m_fFirstStrikeAddIncreaseMPRate : 1.f; }
	};


private:
	CX2GUEL( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit );
	virtual ~CX2GUEL(void);

protected:

	virtual void Init( bool bUseTeamPos_, int iStartPosIndex_ );
	virtual void ParseCommonRandomState();
	virtual void InitState();
	virtual void InitComponent();
	virtual void InitPassiveSkillState();
	virtual void SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ );
	virtual void SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ );
				 
	virtual void CommonStateStartFuture();
	virtual void CommonStateStart();
				 
	virtual void CommonFrameMoveFuture();
	virtual void CommonFrameMove();
				 
	virtual void CommonStateEndFuture();
	virtual void CommonStateEnd();

	virtual void DetonationStateChange( CX2DamageManager::DamageData* pDamageData );

	virtual RENDER_HINT	CommonRender_Prepare();

	virtual bool SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ = CX2SkillTree::ASUT_GROUND );
	virtual void NoStateChangeActionFrameMove();
	virtual float GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

	virtual float GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const;

	virtual CX2GageData* CreateGageData();

	virtual	bool ShouldResetNotBusyTimer();

	void InitStateID();
	void InitStateCommon();
	void InitStateByUnitClass();

	void SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, EL_STATE_ID eStateID_ );
	void SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ );


	void ShowActiveSkillCutInAndLight( float fTimeToShow_, bool bOnlyLight_ = false );
	virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

	bool CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );
	void StateChangeSpecialAttack( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
#ifdef FIX_NO_STATE_SKILL_BUG
	virtual bool IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ );
#endif // FIX_NO_STATE_SKILL_BUG

	CX2ElesisGageData* GetELGageData() { return static_cast<CX2ElesisGageData*>( GetGageData() ); }
	const CX2ElesisGageData* GetELGageData() const { return static_cast<const CX2ElesisGageData*>( GetGageData() ); }
////////////////////////////////////////////////
//검의 길
#ifdef ELSWORD_WAY_OF_SWORD
	void SwordWaitStartFuture();
	void SwordWaitStart();
	void SwordWaitEventProcess();
	void PlayAnimationSwordWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ );	

	void WayOfSwordStateChange();
	void GaleBuffFrameMove();

	bool CanSpecialReactProcess( CX2DamageManager::REACT_TYPE eReActType_ );
#endif // ELSWORD_WAY_OF_SWORD

	float GetFireBallPowerRate() const { return m_fFireBallPowerRate; }
	void SetFireBallPowerRate(float val) { m_fFireBallPowerRate = val; }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 공통 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ESI_DIE_FRONT
	void DieFrontStartFuture();
	void DieFrontStart();
	void DieFrontFrameMove();
	void DieFrontEventProcess();

	//ESI_DIE_BACK
	void DieBackStartFuture();
	void DieBackStart();
	void DieBackFrameMove();
	void DieBackEventProcess();

	//ESI_WAIT
	void WaitStartFuture();
	void WaitStart();
	void WaitEventProcess();

	//ESI_WALK
	void WalkEventProcess();

	//ESI_JUMP_READY
	void JumpReadyEventProcess();

	//ESI_JUMP_UP
	void JumpUpEventProcess();

	//ESI_JUMP_DOWN
	void JumpDownEventProcess();

	//ESI_JUMP_LANDING
	void JumpLandingStartFuture();
	void JumpLandingEventProcess();

	//ESI_DASH
	void DashEventProcess();

	//ESI_DASH_END
	void DashEndStartFuture();
	void DashEndFrameMove();
	void DashEndEventProcess();

	//ESI_DASH_JUMP
	void DashJumpStartFuture();
	void DashJumpStart();
	void DashJumpFrameMoveFuture();
	void DashJumpEventProcess();
	void DashJumpEndFuture();

	//ESI_DASH_JUMP_LANDING
	void DashJumpLandingEventProcess();
	void DashJumpLandingStartFuture();

	//ESI_DAMAGE_SMALL_FRONT
	//USI_DAMAGE_SMALL_BACK
	void DamageSmallEventProcess();

	//ESI_DAMAGE_BIG_FRONT
	//ESI_DAMAGE_BIG_BACK
	void DamageBigStart();
	void DamageBigEventProcess();

	//ESI_DAMAGE_DOWN_FRONT
	void DamageDownFrontFrameMove();
	void DamageDownFrontEventProcess();

	//ESI_DAMAGE_DOWN_BACK
	void DamageDownBackFrameMove();
	void DamageDownBackEventProcess();

	//ESI_DAMAGE_AIR_DOWN_LANDING
	void DamageAirDownLandingEventProcess();

	//ESSI_STANDUP_ROLLING_FRONT_FRONT
	void StandUpRollingFrontFrontEventProcess();
	void StandUpRollingFrontFrontEnd();

	//ESSI_STANDUP_ROLLING_FRONT_BACK
	void StandUpRollingFrontBackEventProcess();
	void StandUpRollingFrontBackEnd();

	//ESSI_STANDUP_ROLLING_BACK_FRONT
	void StandUpRollingBackFrontEventProcess();
	void StandUpRollingBackFrontEnd();

	//ESSI_STANDUP_ROLLING_BACK_BACK
	void StandUpRollingBackBackEventProcess();
	void StandUpRollingBackBackEnd();

	//ESI_STANDUP_ATTACK_FRONT
	void StandUpAttackFrontEventProcess();
	void StandUpAttackFrontEnd();

	//ESI_STANDUP_ATTACK_BACK
	void StandUpAttackBackEventProcess();
	void StandUpAttackBackEnd();
	void StandUpAttackBackStateEndFuture();

	//ESI_STANDUP_ATTACK_FRONT_NO_DOWN
	void StandUpAttackFrontNoDownEventProcess();
	void StandUpAttackFrontNoDownEnd();

	//ESI_STANDUP_ATTACK_BACK_NO_DOWN
	void StandUpAttackBackNoDownEventProcess();
	void StandUpAttackBackNoDownEnd();

	//ESI_DASH_JUMP_POWER_LANDING
	void DashJumpPowerLandingStartFuture();
	void DashJumpPowerLandingStart();
	void DashJumpPowerLandingEventProcess();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 공통 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ComboZEventProcess();
	void ComboZZEventProcess();

	void ComboZZZEventProcess();
	void ComboZZZZEventProcess();
	void ComboZZZXEventProcess();

	void ComboXEventProcess();
	void ComboXXEventProcess();
	void ComboXXXEventProcess();
	void ComboXXZEventProcess();

	void JumpAttackZEventProcess();
	void JumpAttackZFrameMoveFuture();

	void JumpAttackXEventProcess();
	void JumpAttackXFrameMoveFuture();

	void DashComboZEventProcess();
	void DashComboZZEventProcess();

	void DashComboXEventProcess();

	void DashJumpComboZEventProcess();
	void DashJumpComboZZEventProcess();
	void DashJumpComboZZZEventProcess();

	void DashJumpComboXEventProcess();
	void DashJumpComboXXEventProcess();
	void DashJumpComboXXLandingEventProcess();

#ifdef ADD_CAN_NOT_ATTACK_JUMP_DOWN
	void ESI_CAN_NOT_ATTACK_JUMP_DOWN_EventProcess();
#endif // ADD_CAN_NOT_ATTACK_JUMP_DOWN
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 나이트 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ESI_SI_A_EL_DODGE_AND_SLASH_StartFuture();			// 닷지 & 슬래시
	void ESI_SI_A_EL_DODGE_AND_SLASH_Init();
	void ESI_SI_A_EL_DODGE_AND_SLASH_FrameMove();
	void ESI_SI_A_EL_DODGE_AND_SLASH_EventProcess();
	void ESI_SI_A_EL_DODGE_AND_SLASH_SECOND_StartFuture();
	void ESI_SI_A_EL_DODGE_AND_SLASH_ATTACK_Z_EventProcess();

	void ESI_SI_A_EL_LEAP_CRASH_Init();						// 도약
	void ESI_SI_A_EL_LEAP_CRASH_EventProcess();
	void ESI_SI_A_EL_LEAP_CRASH_JUMP_EventProcess();
	void ESI_SI_A_EL_LEAP_CRASH_JUMP_LANDING_EventProcess();

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	void ESI_SI_A_EL_RUSHING_SWORD_StateStart ();
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	void ESI_SI_A_EL_RUSHING_SWORD_StartFuture();			// 러싱 소드
	void ESI_SI_A_EL_RUSHING_SWORD_Init();
	void ESI_SI_A_EL_RUSHING_SWORD_EventProcess();
	void ESI_SI_A_EL_RUSHING_SWORD_2nd_EventProcess();

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	void ESI_SI_A_EL_RUSHING_SWORD_2nd_StartFuture();
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	void ESI_SI_A_EL_RUSHING_SWORD_3rd_EventProcess();

	void ESI_SI_A_EL_KICK_Init();							// 발차기
	void ESI_SI_A_EL_KICK_Start();
	void ESI_SI_A_EL_KICK_EventProcess();
	
	void ESI_SI_A_EL_MEGA_SLASH_Init();						// 메가 슬래시
	void ESI_SI_A_EL_MEGA_SLASH_EventProcess();

	void ESI_SI_A_EL_SONIC_ASSAULT_STAB_Init();				// 소닉 어썰트 - 스탭
	void ESI_SI_A_EL_SONIC_ASSAULT_STAB_EventProcess();

	void ESI_SI_A_EL_SONIC_ASSAULT_STING_Init();			// 소닉 어썰트 - 스팅
	void ESI_SI_A_EL_SONIC_ASSAULT_STING_EventProcess();

	void ESI_SI_SA_EL_WILD_SHOCK_Init();					// 와일드 쇼크
	void ESI_SI_SA_EL_WILD_SHOCK_EventProcess();

	void ESI_SI_SA_EL_POWER_BURSTER_Init();					// 파워 버스터	
	void ESI_SI_SA_EL_POWER_BURSTER_EventProcess();

	void ESI_SI_SA_EL_MEGA_BURSTER_Init();					// 메가 버스터
	void ESI_SI_SA_EL_MEGA_BURSTER_EventProcess();

	void ESI_SI_SA_EL_UNLIMITED_BLADE_Init();				// 언리미티드 블레이드
	void ESI_SI_SA_EL_UNLIMITED_BLADE_EventProcess();

	void ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_EventProcess();	// 극기 - 강(强)
	void ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_Init();
	void ESI_SI_A_EL_ENDURANCE_POWER_ACTIVE_StateStart();
	void DoStateEndurancePowerRage(bool bIsWayOfSword_ );
	void ProcessEndurancePowerSkill();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 세이버 나이트 스테이트 함수
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ESI_DASH_COMBO_SSK_ZZ_EventProcess();				/// DASH_COMBO_ZZ
	void ESI_DASH_COMBO_SSK_ZZZ_EventProcess();				/// DASH_COMBO_ZZZ
	void ESI_DASH_COMBO_SSK_ZZX_EventProcess();				/// DASH_COMBO_ZZX
	void ESI_DASH_JUMP_COMBO_SSK_XX_EventProcess();			/// DASH_JUMP_COMBO_XX
	void ESI_DASH_JUMP_COMBO_SSK_XXX_EventProcess();		/// DASH_JUMP_COMBO_XXX

	void ESI_SI_A_SSK_CROSS_SLASH_Init();					// 크로스 슬래시
	void ESI_SI_A_SSK_CROSS_SLASH_EventProcess();

	void ESI_SI_A_SSK_POWER_BREAK_Init();					// 파워 브레이크
	void ESI_SI_A_SSK_POWER_BREAK_EventProcess();

	void ESI_SI_A_SSK_HEAVY_STUNNER_Init();					// 헤비 스터너
	void ESI_SI_A_SSK_HEAVY_STUNNER_EventProcess();

	void ESI_SI_SA_SSK_SPIRAL_BLAST_Init();					// 스파이럴 블래스트
	void ESI_SI_SA_SSK_SPIRAL_BLAST_EventProcess();

	void ESI_SI_SA_SSK_EXTINCTION_START_Init();				// 세이버 - 익스팅션
	void ESI_SI_SA_SSK_EXTINCTION_START_Start();
	void ESI_SI_SA_SSK_EXTINCTION_START_StartFuture();
	void ESI_SI_SA_SSK_EXTINCTION_START_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMove();
	void ESI_SI_SA_SSK_EXTINCTION_CHARGE_FrameMoveFuture();
	void ESI_SI_SA_SSK_EXTINCTION_CHARGE_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_ATTACK_StartFuture();		// 발사 스테이트
	void ESI_SI_SA_SSK_EXTINCTION_ATTACK_FrameMove();		
	void ESI_SI_SA_SSK_EXTINCTION_ATTACK_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMove();	// 각도 조절
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_UP_FrameMoveFuture();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMove();	
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_EventProcess();
	void ESI_SI_SA_SSK_EXTINCTION_DEGREE_DOWN_FrameMoveFuture();
	void InitExtinction();									// 익스팅션에 사용되는 변수 초기화
	void Extinction_FrameMoveFuture();						// 익스팅션 공통 프레임무브
	void CreateExctionUIEffect();
	void DestoryExctionUIEffect();

	void ESI_SI_SA_SSK_VICTORIOUS_SWORD_Init();				// 승리의 검
	void ESI_SI_SA_SSK_VICTORIOUS_SWORD_EventProcess();
	void ESI_SI_SA_SSK_VICTORIOUS_SWORD_FrameMove();

	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_Init();				// 저지먼트 드라이브
#ifdef ADD_MEMO_1ST_CLASS //김창한
	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStartFuture();
	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_StateStart();
#endif //ADD_MEMO_1ST_CLASS
	void ESI_SI_SA_SSK_JUDGEMENT_DRIVE_EventProcess();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 파이로 나이트 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ESI_DASH_COMBO_SPK_ZX_EventProcess();				// DASH_COMBO_ZX
	void ESI_DASH_COMBO_SPK_ZX_FrameMove();
	void ESI_DASH_COMBO_SPK_ZZ_EventProcess();				// DASH_COMBO_ZZ
	void ESI_DASH_COMBO_SPK_ZZZ_EventProcess();				// DASH_COMBO_ZZZ
	void ESI_DASH_COMBO_SPK_ZZZ_FrameMove();
	void ESI_DASH_JUMP_COMBO_SPK_XZ_EventProcess();			// DASH_JUMP_COMBO_XZ
	void ESI_DASH_JUMP_COMBO_SPK_XZ_FrameMove();
	void ESI_DASH_JUMP_COMBO_SPK_XZ_StartFurture();
	void ESI_DASH_JUMP_COMBO_SPK_XZ_REATTACK_StartFurture();

	void ESI_SI_A_SPK_SWORD_ERUPTION_Init();				// 소드 이럽션
	void ESI_SI_A_SPK_SWORD_ERUPTION_EventProcess();
	
	void ESI_SI_A_SPK_BIG_BURST_Init();						// 대폭살
	void ESI_SI_A_SPK_BIG_BURST_EventProcess(); 

	void ESI_SI_SA_SPK_BURST_RISING_Init();					// 버스트 라이징
	void ESI_SI_SA_SPK_BURST_RISING_EventProcess();

	void ESI_SI_SA_SPK_BURST_WAVE_Init();					// 버스트 웨이브
	void ESI_SI_SA_SPK_BURST_WAVE_EventProcess();

	void ESI_SI_SA_SPK_SWORDFIRE_Init();					// 소드 파이어
	void ESI_SI_SA_SPK_SWORDFIRE_EventProcess();

	void ESI_SI_SA_SPK_INFERNAL_BLADE_Init();				// 인페르날 블레이드
	void ESI_SI_SA_SPK_INFERNAL_BLADE_EventProcess();

	void ESI_SI_SA_SPK_ETERNAL_FIRE_Init();					// 이터널 파이어
	void ESI_SI_SA_SPK_ETERNAL_FIRE_EventProcess();

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 그랜드 마스터 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ESI_DASH_JUMP_COMBO_SGM_XZ_Init ();						// 대시 점프 콤보 XZ
	void ESI_DASH_JUMP_COMBO_SGM_XZ_EventProcess();			 	

	void ESI_DASH_JUMP_COMBO_SGM_XZ_LOOP_EventProcess();			// 대시 점프 콤보 XZ 루프
			
	void ESI_DASH_JUMP_COMBO_SGM_XZ_DOWN_EventProcess();			// 대시 점프 콤보 XZ 다운
	

	void ESI_COMBO_SGM_XX_DOWN_X_START_Init ();						// 콤보 XX 다운 X 시작
	void ESI_COMBO_SGM_XX_DOWN_X_START_FrameMove();
	void ESI_COMBO_SGM_XX_DOWN_X_START_StateStart ();					
	void ESI_COMBO_SGM_XX_DOWN_X_START_EventProcess();			 

	void ESI_COMBO_SGM_XX_DOWN_X_LOOP_EventProcess();				// 콤보 XX 다운 X 루프

	void ESI_COMBO_SGM_XX_DOWN_X_END_EventProcess();				// 콤보 XX 다운 X 끝


	void ESI_QUICK_STANDUP_ATTACK_FRONT_ADVANCED_EventProcess();	// 상급 기사 숙련, 대미지를 입어 뒤로 뒤를 때 공격 입력 시, 해당 State 로 전환

	void ESI_QUICK_STANDUP_ATTACK_BACK_ADVANCED_EventProcess();		// 상급 기사 숙련, 대미지를 입어 앞으로 뒤를 때 공격 입력 시, 해당 State 로 전환


	void ESI_A_SGM_PROVOKE_Init ();									// 도발 시작
	void ESI_A_SGM_PROVOKE_StateStart ();
	void ESI_A_SGM_PROVOKE_EventProcess();			 
	void ESI_A_SGM_PROVOKE_StateEnd();

	void ESI_A_SGM_PROVOKE_REVENGE_EventProcess();					// 도발 성공 후 반격


	void ESI_A_SGM_DRASTIC_CHARGE_START_Init ();					// 드래스틱 차지 시작 
	void ESI_A_SGM_DRASTIC_CHARGE_START_StateStart ();
	void ESI_A_SGM_DRASTIC_CHARGE_START_FrameMove();
	void ESI_A_SGM_DRASTIC_CHARGE_START_EventProcess();			 
	void ESI_A_SGM_DRASTIC_CHARGE_START_StartFurture();

	void ESI_A_SGM_DRASTIC_CHARGE_LOOP_EventProcess();				// 드래스틱 차지 루프
	void ESI_A_SGM_DRASTIC_CHARGE_LOOP_FrameMove();

	void ESI_A_SGM_DRASTIC_CHARGE_END_StateStart ();				// 드래스틱 차지 끝 
	void ESI_A_SGM_DRASTIC_CHARGE_END_EventProcess();			 


	void ESI_A_SGM_SNAP_CRASHER_START_Init ();						// 스냅 크래셔, 공중에서 사용
	void ESI_A_SGM_SNAP_CRASHER_START_EventProcess();			 
	void ESI_A_SGM_SNAP_CRASHER_START_StateEnd();

	void ESI_A_SGM_SNAP_CRASHER_LOOP_StateStart ();					// 스냅 크래셔, 공중에서 사용 후 내려옴
	void ESI_A_SGM_SNAP_CRASHER_LOOP_EventProcess();			 

	void ESI_A_SGM_SNAP_CRASHER_END_StateStart();					// 스냅 크래셔, 공중에서 사용 후 내려와서 땅 찍기
	void ESI_A_SGM_SNAP_CRASHER_END_EventProcess();			 
	

	void ESI_SA_SGM_SONIC_BLADE_Init ();							// 소닉 블레이드
	void ESI_SA_SGM_SONIC_BLADE_EventProcess();			 
	

	void ESI_SA_SGM_CRITICAL_DIVE_Init ();							// 크리티컬 다이브
	void ESI_SA_SGM_CRITICAL_DIVE_EventProcess();			 

	void ESI_SA_SGM_CRITICAL_DIVE_HIT_EventProcess();				// 크리티컬 다이브 사용 중 적이 맞았을 때
	

	void ESI_SA_SGM_JUGGERNAUT_BUSTER_START_Init ();				// 저거넛 버스터, 시작
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_START_StateStart();			
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_START_EventProcess();			 
		
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_EventProcess();			// 저거넛 버스터, 루프
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_FrameMove();
	
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_EventProcess();		// 저거넛 버스터, 루프 끝
	void ESI_SA_SGM_JUGGERNAUT_BUSTER_LOOP_LAST_StartEnd();

	void ESI_SA_SGM_JUGGERNAUT_BUSTER_END_EventProcess();			// 저거넛 버스터, 끝
	

	void ESI_SA_SGM_WAR_PRELUDE_Init ();							// 전장의 숨결
	void ESI_SA_SGM_WAR_PRELUDE_EventProcess();			 
	

	void ESI_HA_SGM_SWORD_OF_RELICS_Init ();						// 소드 오브 렐릭스
	void ESI_HA_SGM_SWORD_OF_RELICS_EventProcess();			 
// #endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 블레이징 하트 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void ESI_COMBO_SBH_XZ_EventProcess();					// 콤보 XZ
	void ESI_COMBO_SBH_XZ_FrameMove();
	void ESI_COMBO_SBH_XZ_Init();

	void ESI_COMBO_SBH_XZZ_EventProcess();					// 콤보 XZZ
	void ESI_COMBO_SBH_XZZ_FrameMove();
	void ESI_COMBO_SBH_XZZ_Init();

	void ESI_COMBO_SBH_XZZZ_EventProcess();					// 콤보 XZZZ
	void ESI_COMBO_SBH_XZZZ_FrameMove();
	void ESI_COMBO_SBH_XZZZ_Init();

	void ESI_DASH_COMBO_SBH_X_EventProcess();				// 대시 콤보 X
	void ESI_DASH_COMBO_SBH_X_FrameMove();
	void ESI_DASH_COMBO_SBH_X_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_X_Init();

	void ESI_DASH_COMBO_SBH_XX_EventProcess();				// 대시 콤보 XX	
	void ESI_DASH_COMBO_SBH_XX_FrameMove();
	void ESI_DASH_COMBO_SBH_XX_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_XX_Init();

	void ESI_DASH_COMBO_SBH_XXX_EventProcess();				// 대시 콤보 XXX
	void ESI_DASH_COMBO_SBH_XXX_FrameMove();
	void ESI_DASH_COMBO_SBH_XXX_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_XXX_Init();

	void ESI_DASH_COMBO_SBH_XXXZ_EventProcess();			// 대시 콤보 X 중 Z
	void ESI_DASH_COMBO_SBH_XXXZ_FrameMove();
	void ESI_DASH_COMBO_SBH_XXXZ_FrameMoveFuture();
	void ESI_DASH_COMBO_SBH_XXXZ_Init();

	void ESI_SI_A_SBH_FLAME_SWORD_Init();					// 홍련의 검
	void ESI_SI_A_SBH_FLAME_SWORD_EventProcess();
	void ESI_SI_A_SBH_FLAME_SWORD_FINISH_EventProcess();

	void ESI_SI_A_SBH_FIREWORKS_Init();						// 불놀이
	void ESI_SI_A_SBH_FIREWORKS_EventProcess();				

	void ESI_SI_A_SBH_SCALET_ROSE_Init();					// 불꽃 장미
	void ESI_SI_A_SBH_SCALET_ROSE_EventProcess();
	void ESI_SI_A_SBH_SCALET_ROSE_FrameMove();
	void ESI_SI_A_SBH_SCALET_ROSE_StateEnd();
	void ESI_SI_A_SBH_SCALET_ROSE_SUCCESS_EventProcess();

	void ESI_SI_SA_SBH_SCALDIC_SWORD_Init();				// 스칼딕 소드
	void ESI_SI_SA_SBH_SCALDIC_SWORD_EventProcess();		

	void ESI_SI_SA_SBH_WALL_OF_PROMINENCE_Init();			// 불꽃의 결계
	void ESI_SI_SA_SBH_WALL_OF_PROMINENCE_EventProcess();
	void ESI_SI_SA_SBH_WALL_OF_PROMINENCE_FrameMove();

	void ESI_SI_SA_SBH_BLAZE_WING_Init();					// 블레이즈 윙
	void ESI_SI_SA_SBH_BLAZE_WING_EventProcess();

	void ESI_SI_SA_SBH_BLAZING_DANCE_START_Init();				// 블레이징 댄스
	void ESI_SI_SA_SBH_BLAZING_DANCE_START_StateStart();
	void ESI_SI_SA_SBH_BLAZING_DANCE_START_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_1ST_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_1ST_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_2ND_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_2ND_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_3RD_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_3RD_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_4TH_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_4TH_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StartFuture();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_EventProcess();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_StateEnd();
	void ESI_SI_SA_SBH_BLAZING_DANCE_FINISH_FrameMoveFuture();
	
	void ESI_SI_HA_SBH_ANCIENT_FIRE_Init();					// 에인션트 파이어
	void ESI_SI_HA_SBH_ANCIENT_FIRE_EventProcess();
	void ESI_SI_HA_SBH_ANCIENT_FIRE_FrameMove();
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	//~ protected 함수
public:
	static CX2GUEL* CreateGUEL( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit );

	static void			AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );

	// 탈 것
	virtual bool CanRidingState();

	virtual void ReInit( bool bTeamPos = false, int iStartPosIndex = -1 );
	virtual void DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ );

	virtual void Win();
	virtual void Lose();
	virtual void Draw();

	virtual void InitEffect();

	virtual void CreateMajorParticleArray();
	virtual void CreateMinorParticleArray();

	static void AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );

	virtual bool SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
	virtual void COMMON_BUFF_FrameMove();

	virtual void HyperModeFrameMove();
#ifdef MODIFY_RIDING_PET_AWAKE
	void RidingHyperModeFrameMove();
	void CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ = false );
#endif //MODIFY_RIDING_PET_AWAKE
	virtual void HyperModeBuffEffectStart();

	virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );

	virtual void InitEnchantWeaponEffectUnitSpecific();

	virtual int GetComboZStateID() const { return ESI_COMBO_Z; }
	virtual int GetComboXStateID() const { return ESI_COMBO_X; }
	////////////////////////////////////////////////
	//검의 길
#ifdef ELSWORD_WAY_OF_SWORD
	virtual int			GetWayOfSwordState() const;
	void SetWayOfSwordState( const int iWayOfSwordState_ );

	virtual float		GetWayOfSwordPoint() const;
	virtual void		SetWayOfSwordPoint( float fWayOfSwordPoint_ );

	void				Start_SwordmanVigor();
	void				End_SwordmanVigor();
	void				Start_SwordmanDestruction();
	void				End_SwordmanDestruction();

	virtual float GetFinalDamage_WayOfSword( CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ );
	virtual void SetCriticalPercent_WayOfSword( IN CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_,
		OUT float& fCriticalPercent_);
#endif // ELSWORD_WAY_OF_SWORD
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	const bool GetUnitClassLine( UNIT_CLASS_LINE UnitClassLine_ );
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

	////////////////////////////////////////////////////
	/// 패시브 관련 함수
	// 기사의 숙명 패시브 처리 프레임 무브
	void ChivalryProc_FrameMove( );

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	//// 불꽃의 낙인 패시브 ////
	void StigmaOfFireProc_FrameMove();													// 불꽃의 낙인 재사용시간 체크 프레임 무브	
	void ApplyDebuffStigmaOfFire( const CX2DamageManager::DamageData& damageData_ );	// 불꽃의 낙인 공격 타입을 체크해서 버프를 거는 Delegate 함수
	
	//// 홍련의 검 ////
	void ApplyDebuffFlameSword( const CX2DamageManager::DamageData& damageData_ );		// 홍련의 검 공격 타입을 체크해서 버프를 거는 Delegate 함수

	//// 불꽃 개화 ////
	void FireBlossomsProc_FrameMove();													// 불꽃 개화 재사용시간 체크 프레임 무브

	//// 블레이징 댄스 ////
	void BlazingDanceFindTargetAndMove();												// 타겟 위치를 찾아 이동시키는 함수.
	const bool BlazindDanceCheckTargetPosition( CX2GameUnit* pUnit_, const CKTDGLineMap::LineData* pLineData_ );			// 적절한 타겟 위치인지 체크하는 함수.

	CFirstStrikeData & GetFirstStrikeData ( void ) { return m_cFirstStrikeData; }

	bool  IsFirstStrikeRelateState ( void )
	{	
		switch ( GetNowStateID() )
		{
			case ESI_COMBO_Z: 
			case ESI_COMBO_X :
			case ESI_JUMP_ATTACK_Z :
			case ESI_JUMP_ATTACK_X :
			case ESI_DASH_COMBO_Z :
			case ESI_DASH_COMBO_X :
			case ESI_DASH_JUMP_COMBO_Z : 
			case ESI_DASH_JUMP_COMBO_X : 
				return true;

			default:
				return false;
		}
		return false;
	}

	bool GetIsProvokeStateChange () const	 { return m_bIsProvokeStateChange; }
	void SetIsProvokeStateChange ( bool val ) { m_bIsProvokeStateChange = val; }

	virtual void ChangeDamageData ();
	
	void DrasticChargeProc_FrameMove ();			// 드래스틱 차지 이펙트
	void XX_DOWN_XProc_FrameMove ();				// XX 밑 X 이펙트
	void SnapCrasherProc_FrameMove ();			// 드래스틱 차지 이펙트
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef MODIFY_SET_DAMAGE_DATA
	virtual	void SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_ ,OUT float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA


	//~ Public 함수
protected:
	// 스킬 사용 관련
	const CX2SkillTree::SkillTemplet*	m_pSkillTemplet;	/// 스킬 템플릿 포인터
	int		m_iPressedSkillSlotIndex;
	float	m_fMPConsume;

#ifdef ELSWORD_WAY_OF_SWORD
	int		m_iPreComboCount;
#endif // ELSWORD_WAY_OF_SWORD

	// 닷지&슬래시 재사용 횟수
	UINT m_uiDodgeAndSlashMaxReuseCount;	// 최대 재사용 횟수
	UINT m_uiDodgeAndSlashReuseCount;		// 현재 남은 재사용 횟수

	/////////////////////////////////////////////////////
	/// 패시브 스킬 관련

	// 파이로 나이트 - 불의 신체
	float m_fFireBallPowerRate; /// 파이어볼 강화 배율. 기본값 1.f

	// 세이버 나이트 - 기사의 숙명
	float m_fChivalryBuffCheckTime;		// 기사의 숙명 버프 적용 체크 타임

	UINT m_uiChivalryAttackBuffFactorID;  // 공격력 버프 팩터 ID
	UINT m_uiChivalryDefenceBuffFactorID; // 방어력 버프 팩터 ID
	UINT m_uiChivalryLevel;				  // 기사의 숙명 스킬 레벨

	// 엘리시스 공통 - 극기 - 강(强)
	bool m_bIsActiveEndurancePower;		/// 활성화 여부

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	// 블레이징 하트 - 불꽃의 낙인
	float m_fStigmaOfFireCoolTime;		// 스킬 쿨타임
	bool m_bIsStigmaOfFire;				// 활성화 여부
	UINT m_uiSkillLevelStigmaOfFire;	// 스킬 레벨

	// 블레이징 하트 - 불꽃 개화 
	BUFF_TEMPLET_ID m_eFireBlossomsSaveDebuffID;	// 해제 할 디버프 ID
	float m_fFireBlossomsCoolTime;					// 스킬 쿨타임
	float m_fFireBlossomsCheckTime;					// 스킬 체크타임. 스킬 레벨에 따라 달라짐.
	bool m_bIsFireBlossoms;							// 활성화 여부
	UINT m_uiFireBlossomsSkillLevel;				// 스킬 레벨

	// 블레이징 하트 - 꺼지지 않는 불꽃
	bool m_bIsLearnUnextinguishableFire;			// 배웠는지 여부
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

	/////////////////////////////////////////////////////
	/// 액티브, 스페셜 액티브 스킬 관련


	// 세이버 - 익스팅션
	float m_fExtinctionRemainTime;		/// 익스팅션 유지 시간 체크
	float m_fExtinctionDegreeZ;			/// 익스팅션 발사 각도

	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hExctionBG;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hExtinctionArrow;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CKTDGParticleSystem::CParticleHandle				m_hExtinctionArrowParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticle*						m_pExtinctionArrow;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	// 승리의 검
	bool m_bIsApplyVictoriousSword;		/// 승리의 검 적용 여부. 닷지&슬래시 관련 효과 적용하기 위한 변수
	float m_fDodgeAndSlashCoolTimeDecreaseRate; /// 닷지&슬래시 쿨타임 감소 비율

	// ESI_DASH_JUMP_COMBO_SPK_XZ
	bool m_bIsDashAttackPossible;		// 대시 공격 불가 상태 // 대점ZX 연속 사용 하는 것을 막기 위한 코드
	UINT m_uiSPKXZReattakCount;
	//~ protected 변수

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

	// 엘리시스 그랜드 마스터 대시 점프 콤보 XZ 
	// 엘리시스 그랜드 마스터 콤보 XX 밑 X
	int		m_iComboXXdownXNowLoopCount;				// 현재 카운트 변수
	int		m_iComboXXdownXMaxLoopCount;				// 최대 카운트 변수

	// 엘리시스 그랜드 마스터 패시브, 상급 기사 숙련
	bool	m_bAbleQuickStand;							// 재빠른 기상 가능 여부
	bool	m_bAbleQuickStandAttack;					// 재빠른 기상 후 기회의 포착 가능 여부

	// 엘리시스 그랜드 마스터 패시브, 검의 왈츠 	
	int		m_iRushingSwordAddLoopValue;				// 추가된 반복 횟수
	int		m_iRushingSwordNowLoopValue;				// 현재 반복 횟수
	int		m_iDecreaseMPRushingSword;					// 시전 MP 감소
	
	// 엘리시스 그랜드 마스터 패시브, 선수 필승
	CFirstStrikeData  m_cFirstStrikeData;				// 선수 필승용 구조체
	
	// 엘리시스 그랜드 마스터 액티브, 드래스틱 차지
	float	m_fDrasticChargeNowLoopTime;				// 루프 시간
	float	m_fDrasticChargeLoopDecreaseMPValue;		// MP 소모량

	// 엘리시스 그랜드 마스터 액티브 도발
	bool	m_bIsProvokeStateChange;					// 도발 상태로 변경해야 하는가?, GUuser 의 DamageReact 에서 해당 Boolean 값을 체크한 뒤 
														// CX2GUEl 의 DamageReact 에서 상태를 변경한다

	// 엘리시스 그랜드 마스터 액티브, 스냅 크래셔
	CX2EffectSet::Handle	m_hSnapCresherLoop;	// 스냅크래셔 루프 이펙트
	CX2EffectSet::Handle	m_hSnapCreaherLoopAttack; // 스냅 크래셔 루프 어택 이펙트 ( 문제사항이 생겨 루프 이펙트에서 데미지만 따로 분리함 )


	// 엘리시스 그랜드 마스터 콤보, XX 밑 X
	CX2EffectSet::Handle	m_hXXDownXLoop;				// XX 밑 X 루프 이펙트 핸들

	// 엘리시스 그랜드 마스터 액티브, 드래스틱 차지
	CX2EffectSet::Handle	m_hDrasticChargeLoop;				// 드래스틱 차지 루프 이펙트 핸들, 몸에 붙는 동그라미
	CX2EffectSet::Handle	m_hDrasticChargeGroundFire;			// 드래스틱 차지 루프 이펙트 핸들, 칼에 붙는 불

	// 엘리시스 그랜드 마스터 스액, 저거넛 버스터
	float	m_fJuggernautBusterNowLoopTime;				// 현재 루프 시간
	bool	m_bJuggernautBusterIsStartDirectionRight;	// 사용 했을 때의 방향



	// 블레이징 하트 - 불꽃 장미
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffectHandle       m_hSBHScaletRoseDamageEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CX2DamageEffect::CEffect*	m_pSBHScaletRoseDamageEffect;			// 이펙트 임시 저장
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	// 블레이징 하트 - 블레이징 댄스
	std::vector<UidType>		m_vecBlazingDanceTargetUid;				// 타겟 Unit Uid
	bool						m_bIsThereBlazingDanceTarget;			// 공격할 타겟이 있는지 체크
	bool						m_bIsBlazingDanceLastAttack;			// 마지막 타격인지 체크(방향 전환을 위해)
	D3DXVECTOR3					m_vec3BlazingDanceTargetPos;			// 마지막 타겟 위치(방향 전환을 위해)

	// 블레이징 하트 - 홍련의 검
	bool						m_bIsFrameSword;						// 활성화 여부
	UINT						m_uiSkillLevelFlameSword;				// 스킬 레벨

	// 블레이징 하트 - 파이어 월
	float						m_fWallOfProminenceLifeTime;			// 파이어 월 유지 시간

	// 블레이징 하트 - 불꽃의 낙인 
	float						m_fStigmaOfFireDamageValue;				// 레벨에 따른 폭발 데미지 증가량. 패시브 스킬이라 따로 저장.

	// 블레이징 하트 - 대시 콤보 시작 딜레이 용
	float						m_fDashComboStartDelay;					// 대시 콤보의 마지막( XXX나 XXXZ ) 후에 다시 대시 콤보를 시작하기 까지의 시간

	// 블레이징 하트 - 에인션트 파이어 
	float						m_fAncientFireRenderTime;				// 렌더값 변하는 시간

#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

public:

	//~ Public 변수
};

#endif // NEW_CHARACTER_EL
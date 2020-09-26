#pragma once

class CX2GUElsword_SwordMan : public CX2GUUser
{
	public:
		enum ELSWORD_SWORDMAN_STATE_ID
		{
			ESSI_BASE = USI_END + 1,
			
			ESSI_DASH_JUMP_POWER_LANDING,

			ESSI_STANDUP_ROLLING_FRONT_FRONT,
			ESSI_STANDUP_ROLLING_FRONT_BACK,
			ESSI_STANDUP_ROLLING_BACK_FRONT,
			ESSI_STANDUP_ROLLING_BACK_BACK,

			ESSI_STANDUP_ATTACK_FRONT,
			ESSI_STANDUP_ATTACK_BACK,

			ESSI_REVENGE_WAIT,
			ESSI_REVENGE_ATTACK,

			ESSI_COMBO_Z1,
			ESSI_COMBO_Z2,
			ESSI_COMBO_Z3,
			ESSI_COMBO_Z4,
			ESSI_COMBO_Z4a,

			ESSI_COMBO_X1,
			ESSI_COMBO_X2,
			ESSI_COMBO_X3,
			ESSI_COMBO_X3a,

			ESSI_DASH_COMBO_Z1,
			ESSI_DASH_COMBO_Z2,
			ESSI_DASH_COMBO_Z3,
			ESSI_DASH_COMBO_Z3a,

			ESSI_DASH_ATTACK_X,

			ESSI_JUMP_ATTACK_Z,
			ESSI_JUMP_ATTACK_X,

			ESSI_DASH_JUMP_COMBO_Z1,
			ESSI_DASH_JUMP_COMBO_Z2,
			ESSI_DASH_JUMP_COMBO_Z3,
			ESSI_DASH_JUMP_COMBO_Z3_LANDING,

			ESSI_DASH_JUMP_COMBO_X1,
			ESSI_DASH_JUMP_COMBO_X2,
			ESSI_DASH_JUMP_COMBO_X2a,
			ESSI_DASH_JUMP_COMBO_X2_LANDING,
			ESSI_DASH_JUMP_COMBO_X3,

			ESSI_STANDUP_ATTACK_FRONT_NO_DOWN,
			ESSI_STANDUP_ATTACK_BACK_NO_DOWN,

			ESSI_SA_ESK_WINDMILL,
			ESSI_SA_ESK_WINDMILL_HYPER,
			ESSI_SA_ESK_WINDMILL_END,
			ESSI_SA_ESK_WINDMILL_END_HYPER,
			
			ESSI_SA_EMK_SWORD_FIRE_LANDING,
			ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER,

			ESSI_SA_AIR_SLASH_SECOND,				
			ESSI_SA_AIR_SLASH_SECOND_HYPER,


			// 엘소드 2차 전직 콤보
			ESSI_ELK_ZZZfrontZ,
			ESSI_ELK_XXX,
			ESSI_ELK_XXXX,

			ESSI_ERS_XZ,
			ESSI_ERS_XZZ,
			ESSI_ERS_DASH_JUMP_XZ,
			ESSI_ERS_DASH_JUMP_XZZ,



			ESSI_A_ELK_ROLLING_SMASH, 
			ESSI_A_ELK_ROLLING_SMASH_HYPER,
			ESSI_A_ELK_ROLLING_SMASH_LANDING,
			ESSI_A_ELK_ROLLING_SMASH_LANDING_HYPER,
			
			ESSI_SWORD_WAIT,
			ESSI_DESTRUCTION_RAGE,

#ifdef ELSWORD_SHEATH_KNIGHT
			ESSI_ETK_COMBO_ZZZ,
			ESSI_ETK_COMBO_ZZZZ,
			ESSI_ETK_COMBO_ZZZZZ,
			ESSI_ETK_COMBO_XX,
			ESSI_ETK_COMBO_XXX_LOOP,
			ESSI_ETK_COMBO_XXXZ,
			ESSI_ETK_COMBO_XXX_END,
			ESSI_ETK_DASH_COMBO_ZZ,

			ESSI_SI_A_ES_ROLLING,
			ESSI_SI_A_ES_ROLLING_END,
			ESSI_SI_SA_ETK_HARSH_CHASER,
			ESSI_SI_SA_ETK_HARSH_CHASER_PULL,
			ESSI_SI_SA_ETK_MAELSTORM_RAGE,
			ESSI_SI_SA_ETK_FINAL_STRIKE,
			ESSI_SI_SA_ETK_SWORD_FALL,
			ESSI_SI_SA_ETK_CRESCENT_CUT,

#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_ELSWORD_INFINITY_SWORD
			ESSI_EIS_DASH_COMBO_XX,
			ESSI_EIS_DASH_COMBO_XXX,
			ESSI_EIS_DASHJUMP_COMBO_XXX,
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			ESSI_SI_A_ES_DEFENCE_READY,
			ESSI_SI_A_ES_DEFENCE_WAIT,
			ESSI_SI_A_ES_DEFENCE_END,
			ESSI_SI_A_ES_DEFENCE_DAMAGE,
			ESSI_SI_P_ESK_AUTO_GUARD,
#endif //UPGRADE_SKILL_SYSTEM_2013

			//////////////////////////////////////////////////////////////////////////
			// 위쪽에 추가해주세요~ 이 아래는 테스트로 추가된 enum 입니다



			ESSI_TRANSFORMED, 

			ESSI_LINK_JUMP,

			ESSI_CLIMB_WAIT,
			ESSI_CLIMB_FRONT,
			ESSI_CLIMB_UP,
			ESSI_CLIMB_UP_FRONT,
			ESSI_CLIMB_DOWN,
			ESSI_CLIMB_DOWN_FRONT,


			ESSI_GRAB_FRONT,
			ESSI_PICK_UP,
			ESSI_THROW_FORWARD,
			ESSI_THROW_UPSIDE,


			ESSI_THROW_WOODEN_PIECE,

			ESSI_MACHINE_GUN_RIDE,
			ESSI_MACHINE_GUN_WAIT,
			ESSI_MACHINE_GUN_WALK,
			ESSI_MACHINE_GUN_ATTACK,
			ESSI_MACHINE_GUN_LEAVE,
			ESSI_MACHINE_GUN_JUMP_UP,
			ESSI_MACHINE_GUN_JUMP_DOWN,

			ESSI_SUMMON_MAGIC_SPEAR,

			ESSI_LEAP,


//#ifdef WALL_JUMP_TEST
			ESSI_WALL_LANDING,			
//#endif WALL_JUMP_TEST

//#ifdef PVP_BOSS_COMBAT_TEST
//			ESSI_FROZEN,
//#endif PVP_BOSS_COMBAT_TEST
		};
		
		enum ELSWORD_MAJOR_PARTICLE_INSTANCE_ID
		{
			ELSWORD_MAJOR_PII_BASE,			// kimhc // 현재는 쓰이는 것이 없어서 임시로 넣어놓음
			ELSWORD_MAJOR_PII_END,
		};

		enum ELSWORD_MINOR_PARTICLE_INSTANCE_ID
		{
			ELSWORD_MINOR_PII_REVENGE_WAIT,
			ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE,
			ELSWORD_MINOR_PII_IMPACT_BLUE,
			ELSWORD_MINOR_PII_STR_KUAANG,
			ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_RED,
			ELSWORD_MINOR_PII_IMPACT_TICK_RED,
			ELSWORD_MINOR_PII_MAGNUM_BLADE_FIRE_WAVE,

			ELSWORD_MINOR_PII_END,
		};

#ifdef ELSWORD_SHEATH_KNIGHT
		struct ThirdBladeMatrix
		{
			D3DXMATRIX		PastCombineMatrix;
			float			fPastTime;
			ThirdBladeMatrix()
			{
				fPastTime	= 0.f;
			}
		};


		struct ThirdBladeDamageData
		{
			CX2DamageManager::DamageData	m_DamageData;
			float							m_fAttackTimeStart;
			float							m_fAttackTimeEnd;
			ThirdBladeDamageData()
			{
				m_fAttackTimeStart		= 0.f;
				m_fAttackTimeEnd		= 0.f;
			}
		};

#endif ELSWORD_SHEATH_KNIGHT



#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한

        class   CElswordGuardSkill;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        typedef boost::intrusive_ptr<CElswordGuardSkill> CElswordGuardSkillPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		typedef boost::shared_ptr<CElswordGuardSkill> CElswordGuardSkillPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		class CElswordGuardSkill : boost::noncopyable
		{
		private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			ELSWORD_SWORDMAN_STATE_ID	m_eSkillSlotID_Guard;			/// 엘소드의 Guard가 어느 스킬슬롯에 장착되어 있는지를 담는 변수
			const bool*					m_pGuardKey;					/// 엘소드의 Guard가 장착되어 있는 스킬슬롯의 Key(A, S, D, C 등)	
			bool						m_bGuardEquippedAtSlotB;		/// 엘소드의 Guard가 B슬롯에 장착되어 있는지를 담는 변수
			int							m_iDamageReducePercent;			/// 엘소드의 Guard 상태에서 데미지 감소율
			CElswordGuardSkill() : m_eSkillSlotID_Guard( ESSI_BASE ),
				m_pGuardKey( NULL ), m_bGuardEquippedAtSlotB( false ), m_iDamageReducePercent( 0 )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			{}

		public:
			static CElswordGuardSkillPtr    CreateElswordGuardSkill() { return CElswordGuardSkillPtr( new CElswordGuardSkill ); }

			CX2GUElsword_SwordMan::ELSWORD_SWORDMAN_STATE_ID GetSkillSlotID_Guard() const { return m_eSkillSlotID_Guard; }	// 엘소드의 Guard가 어느 스킬슬롯에 장착되어 있는지를 담는 변수값을 얻어옴
			void SetSkillSlotID_Guard(CX2GUElsword_SwordMan::ELSWORD_SWORDMAN_STATE_ID eSkillSlotID_Guard_) { m_eSkillSlotID_Guard = eSkillSlotID_Guard_; } // 엘소드의 Guard가 어느 스킬슬롯에 장착되어 있는지를 담는 변수값을 Set함

			bool GetGuardKeyValue() const { return *m_pGuardKey; }	// 엘소드의 Guard가 장착되어 있는 스킬슬롯의 Key(A, S, D, C 등)	의 값을 얻어옴 (눌렸는지 안눌렸는지)
			void SetGuardKeyPointer( const bool* pGuardKey_ ) { m_pGuardKey = pGuardKey_; }	// 엘소드의 Guard가 장착되어 있는 스킬슬롯의 Key(A, S, D, C 등)	포인터를 Set함

			bool GetGuardEquippedAtSlotB() const { return m_bGuardEquippedAtSlotB; }	// 엘소드의 Guard가 B슬롯에 장착되어 있는지를 얻어옴
			void SetGuardEquippedAtSlotB(bool bGuardEquippedAtSlotB_) { m_bGuardEquippedAtSlotB = bGuardEquippedAtSlotB_; }

			int GetDamageReducePercent() const { return m_iDamageReducePercent; }
			void SetDamageReducePercent(int iDamageReducePercent_) { m_iDamageReducePercent = iDamageReducePercent_; }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            void    AddRef()    {   ++m_uRefCount; }
            void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		};


#endif //UPGRADE_SKILL_SYSTEM_2013


	private:
		CX2GUElsword_SwordMan( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit );
		virtual ~CX2GUElsword_SwordMan(void);

		//{{ kimhc // 2010.11.2 // 로드나이트 - 고통 억제
#ifdef	NEW_SKILL_2010_11
		void				ReducePain( CX2SkillTree::SKILL_TYPE eSkillType_, const IN CX2UserSkillTree& cUserSkillTree_ );
#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.2 // 로드나이트 - 고통 억제


	public:
		static CX2GUElsword_SwordMan* CreateGUElsword( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit )
		{
			return new CX2GUElsword_SwordMan( unitIndex, teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
				frameBufferNum, pUnit );
		};

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void			AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ );
		static void			AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ );
		static void			AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		

		virtual void		ReInit( bool bRandom = false, int startPosIndex = -1 );


		virtual void		DamageReact( CX2DamageManager::DamageData* pDamageData );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
#ifdef ARA_CHANGE_CLASS_FIRST
		virtual void		DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack = -1, int iAvoidance = -1 );
#else
		virtual void		DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack = -1 );
#endif //ARA_CHANGE_CLASS_FIRST

		virtual float		GetFinalDamageToBeChangedByActiveSkill( float fFinalDamage_ );
		void				ApplyBuffToHarshSlayer( const CX2DamageManager::DamageData& damageData_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef BALANCE_PATCH_20131107
		//각 유닛마다 Enchant가 적용될 때 셋팅해야 하는 함수 실행.
		virtual void				SetSpecificValueByEnchant();
#endif //BALANCE_PATCH_20131107

		virtual void		Win();
		virtual void		Lose();
		virtual void		Draw();
		

		virtual void		InitEffect();
		virtual void		InitEnchantWeaponEffectUnitSpecific();


		void				InitializeElswordMajorParticleArray();	// 메이저 파티클 배열을 초기화 // kimhc // 2010.11.5 
		virtual void		CreateMajorParticleArray();		// 메이저 파티클 배열을 생성 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void			AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetElswordMajorParticleByEnum( ELSWORD_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		ParticleEventSequenceHandle	GetHandleElswordMajorParticleByEnum( ELSWORD_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const // 엘소드만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		{
			ASSERT( ELSWORD_MAJOR_PII_END > eVal_ && ELSWORD_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahElswordMajorParticleInstance[eVal_];
		}

		ParticleEventSequenceHandle& GetHandleReferenceElswordMajorParticleByEnum( ELSWORD_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) // 엘소드만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		{
			ASSERT( ELSWORD_MAJOR_PII_END > eVal_ && ELSWORD_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahElswordMajorParticleInstance[eVal_];
		}

		void				SetHandleElswordMajorParticleByEnum( ELSWORD_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ ) // 엘소드만 쓰는 메이저 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함 // kimhc // 2010.11.5 
		{
			ASSERT( ELSWORD_MAJOR_PII_END > eVal_ && ELSWORD_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahElswordMajorParticleInstance[eVal_] = hHandle_;
		}
		void				DeleteElswordMajorParticle();
		

		void				InitializeElswordMinorParticleArray();	// 공통적으로 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
		virtual void		CreateMinorParticleArray();		// 각 캐릭터들만 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void			AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetElswordMinorParticleByEnum( ELSWORD_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		ParticleEventSequenceHandle	GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const	// 엘소드만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴	// kimhc // 2010.11.5 
		{
			ASSERT( ELSWORD_MINOR_PII_END > eVal_ && ELSWORD_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahElswordMinorParticleInstance[eVal_];
		}

		ParticleEventSequenceHandle& GetHandleReferenceElswordMinorParticleByEnum( ELSWORD_MINOR_PARTICLE_INSTANCE_ID eVal_ ) // 엘소드만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴	// kimhc // 2010.11.5 
		{
			ASSERT( ELSWORD_MINOR_PII_END > eVal_ && ELSWORD_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahElswordMinorParticleInstance[eVal_];
		}

		void				SetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )	// 엘소드만 쓰는 마이너 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함	// kimhc // 2010.11.5 
		{
			ASSERT( ELSWORD_MINOR_PII_END > eVal_ && ELSWORD_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahElswordMinorParticleInstance[eVal_] = hHandle_;
		}
		void				DeleteElswordMinorParticle();

		virtual void		DeleteMinorParticle();

		virtual bool		SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet );
		
		virtual float	GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

#ifdef NEW_SKILL_2010_11
		//{{ oasis907 : 김상윤 [2010.11.1] // 룬 슬레이어 - 마법 저항 수련
		float				GetMagicReflectRate();
		//}}
#endif NEW_SKILL_2010_11

		CX2ElswordGageData*	GetElswordGageData() 
		{ 
			ASSERT( NULL != GetGageData() );
			return static_cast<CX2ElswordGageData*>( GetGageData() ); 
		}
		const CX2ElswordGageData*	GetElswordGageData() const 
		{ 
			ASSERT( NULL != GetGageData() );
			return static_cast<const CX2ElswordGageData*>( GetGageData() ); 
		}

#ifdef ELSWORD_WAY_OF_SWORD
		virtual int			GetWayOfSwordState() const;
		void SetWayOfSwordState( const int iWayOfSwordState_ );

		virtual float		GetWayOfSwordPoint() const;
		virtual void		SetWayOfSwordPoint( float fWayOfSwordPoint_ );
		float				GetAnimSpeedUpByVigor(){ return m_fAnimSpeedUpByVigor; }

		void				Start_SwordmanVigor();
		void				End_SwordmanVigor();
		void				Start_SwordmanDestruction();
		void				End_SwordmanDestruction();

#endif ELSWORD_WAY_OF_SWORD
#ifdef ELSWORD_SHEATH_KNIGHT
		void				AddComboToBrutalSlayer( const CX2DamageManager::DamageData& damageData_ );
#endif ELSWORD_SHEATH_KNIGHT
#ifdef RIDING_SYSTEM
		virtual bool CanRidingState();
#endif //RIDING_SYSTEM

#ifdef ELSWORD_WAY_OF_SWORD
		virtual float		GetFinalDamage_WayOfSword( CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ );
#endif //ELSWORD_WAY_OF_SWORD

		virtual int GetComboZStateID() const { return ESSI_COMBO_Z1; }
		virtual int GetComboXStateID() const { return ESSI_COMBO_X1; }

	protected:
		virtual void ParseCommonRandomState();

		virtual void		InitState();


		void InitStateID();
		void InitStateCommon();


		virtual void		InitComponent();	
		
		/// 각 유닛마다 특정 버프가 실행 또는 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
		virtual void		SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void		UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );

		virtual void		InitPassiveSkillState();
		virtual void		SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ );
		virtual void		SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ );


		virtual void		CommonStateStartFuture();
		virtual void		CommonStateStart();
		virtual void		CommonFrameMoveFuture();
		virtual void		CommonFrameMove();
		virtual void		CommonStateEndFuture();
		virtual void		CommonStateEnd();
//{{ robobeg : 2008-10-28
		//virtual void		CommonRender();
        virtual RENDER_HINT CommonRender_Prepare();
//}} robobeg : 2008-10-28

		virtual bool		SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );
		virtual void		NoStateChangeActionFrameMove();
		
		void				ProcessEnduranceSkill();		/// 극기 스킬 처리
		
		virtual	bool		ShouldResetNotBusyTimer();
		virtual void		HyperModeBuffEffectStart();

	protected:
		bool				m_bReAttackZ1Right;
		float				m_fReAttackZ1Time;
		float				m_fReDashJumpXTime;

		float				m_fAttackPowerRateFireBall;

		
		float					m_fWindMillDuration;
		CX2EffectSet::Handle	m_hWindMill;
		CX2EffectSet::Handle	m_hWindMillLand;
		CKTDXDeviceSound*		m_pSoundWindMill;

		CX2EffectSet::Handle	m_hSwordFire;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle   m_hRuneSlayerDashJumpXZ;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect* m_pRuneSlayerDashJumpXZ;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2EffectSet::Handle m_hRuneSlayerComboXZ;
		CX2EffectSet::Handle m_hSwordEnchant;



#ifdef SKILL_CASH_10_TEST
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle			    m_hDoubleSlash;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*			m_pDoubleSlash;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif
		
#ifdef NEW_SKILL_2010_11
		//{{ oasis907 : 김상윤 [2010.11.1] // 룬 슬레이어 - 마법 저항 수련
		float								m_fMagicReflectRate;	// 마법 반사 확률
		//}}
#endif NEW_SKILL_2010_11

        /** 아래의 CKTDGXSkinAnim* 타입의 멤버들은 내부적으로 관리하는 객체이므로,
            smart pointer를 사용하지 않는다.
            - jintaeks on 2009-01-12, 16:26 */


#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST

		//{{AFX
		CKTDGXSkinAnim*						m_pMegaSlash;
		CKTDGXSkinAnim*						m_pMegaSlashBackDust;

		CKTDGXSkinAnim*						m_pFlameGager;
		CKTDGXSkinAnim*						m_pFlameGagerLand;

		CKTDGXSkinAnim*						m_pMagnumBlade;
		CKTDGXSkinAnim*						m_pMagnumBladeFire;
		//}}AFX

#endif NO_MORE_EFFECT_HARD_CODING_TEST


		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshInstAirSlash1;
		CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshInstFatalFury;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqAssualtSlashRing;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqAssualtSlashDust;

		// UI Major
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqHandFire;

		D3DXVECTOR3										m_StartPosToLeap;
		D3DXVECTOR3										m_DirVecToLeap;
		float											m_fVerticalSpeedToLeap;
		float											m_fParallelSpeedToLeap;
		float											m_fHeightToLeap;
		int												m_DegreeToLeap;
		float											m_fTimeToLeap;
		float											m_fElapsedTimeToLeap;

#ifdef TEST_MACRO_COMBO
		CKTDXTimer		m_TimerStartRun;
#endif TEST_MACRO_COMBO


		float m_fRollingSmashDuration;
		bool m_bIsMagicKnightWinMotion2; 
		bool m_bERS_DASH_JUMP_XZ_Rune_Created;
		bool m_bERS_COMBO_XZ_Rune_Created;

#ifdef SERV_SKILL_NOTE
		bool m_bFirstAttackSuccess;		
#endif

#ifdef ELSWORD_WAY_OF_SWORD
		CX2EffectSet::Handle							m_hEffectVigor;
		CX2EffectSet::Handle							m_hEffectDestruction;
		float											m_fDestructionRageTime;
		float											m_fAnimSpeedUpByVigor;
#endif ELSWORD_WAY_OF_SWORD

#ifdef ELSWORD_SHEATH_KNIGHT
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hThirdBladeSheathMesh;

		float											m_fDamageDataChangeTime;
		float											m_fDamageDataChangeTime2;

		int												m_iETKComboXXXLoopCount;

		D3DXVECTOR3										m_vFinalStrikeBlackHolePos;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle   						m_hFinalStrikeBlackHole;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*						m_FinalStrikeBlackHole;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		D3DXVECTOR3										m_vSwordFallSheathPos;

		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeFirst;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeSecond;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeThird;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeFourth;
#ifdef ADDITIONAL_MEMO
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeFirst_Memo;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeSecond_Memo;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeThird_Memo;
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSwordFallBladeFourth_Memo;
#endif

		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hSecondBladeSheath;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle                           m_hDamageEffectHarshChaserBlade;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*						m_pDamageEffectHarshChaserBlade;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		
		float											m_fRollingHpConsumeRel;
		float											m_fRollingSpeedRel;
		float											m_fHarshChaserProjectiveRange;
		float											m_fMaelStormEffectiveTime;
		

		bool											m_bCompactCounterEnable;
		float											m_fCompactCounterCoolTime;
		
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_ELSWORD_INFINITY_SWORD
		float											m_fDashStateTime;
		float											m_fMindOfFighterConsumeMpRate;
		int												m_iBrutalSlayerActivatedLevel;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle                           m_hDamageSwordBlasting[3];
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*						m_pDamageSwordBlasting[3];
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif

#ifdef BALANCE_RUNE_SLAYER_20130214
		//버프 템플릿에서 다른 무기 이펙트를 지정했다면,  강화 이펙트 보여 주지 않도록 하기 위해 추가
		bool											m_bShowWeaponEnchantEffect; // 무기 강화 이펙트 출력 여부.
#endif //BALANCE_RUNE_SLAYER_20130214

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		CElswordGuardSkillPtr							m_GuardForElswordPtr;				//막기
		float											m_fAutoGuardRate;					//오토 가드 확률
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle						    m_hDamageEffectPiercingSword;		//피어싱 소드 이펙트 핸들
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*						m_pDamageEffectPiercingSword;		//피어싱 소드 이펙트 핸들
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		float											m_fPiercingSwordProjectileRange;	//피어싱 소드 사거리
		float											m_fRuneProjectileRangeIncPercent;	//룬 사거리 증가 비율(룬 마스터리)
		float											m_fRuneProjectileSizeIncPercent;	//룬 크기 증가 비율(룬 마스터리)	
		bool											m_bIsGuardMotion;					//현재 가드 모션을 취하고 있는지 체크
		UCHAR											m_ucSaveDebuffCount;				//역전의 인내 디버프 걸릴경우를 알아내기 위한 변수
		bool											m_bAutoGuardBlocking;				//오토 가드 체크 값
		float											m_fIncreaseCoolTimeRel;		//강렬한 일격 스킬로 인해 추가되는 쿨타임
		float											m_fActiveSkillAddPowerRate;			//강렬한 일격 스킬로 인해 증가되는 액티브 스킬의 공격력
		CX2EffectSet::Handle							m_hEffectSwordShield;				//소드 실드 이펙트 핸들
		float											m_fSwordShieldEffectiveTime;		//소드 실드 이펙트 유지 시간
#endif //UPGRADE_SKILL_SYSTEM_2013 

#ifdef BALANCE_PATCH_20131107
		int												m_iSkillLevelInduranceOfRevenge; //역전의 인내 스킬 레벨 값.
#endif //BALANCE_PATCH_20131107

	private:

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahElswordMajorParticleInstance[ELSWORD_MAJOR_PII_END];
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_ahElswordMinorParticleInstance[ELSWORD_MINOR_PII_END];

	
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//							스테이트 함수 시작

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

	protected:	

#ifdef	SERV_ELSWORD_INFINITY_SWORD
		void LightingStepFrameMove();	/// 전광석화 버프 적용
#endif	SERV_ELSWORD_INFINITY_SWORD

		//ESSI_DIE_FRONT
		void DieFrontStartFuture();
		void DieFrontStart();
		void DieFrontFrameMove();
		void DieFrontEventProcess();

		//ESSI_DIE_BACK
		void DieBackStartFuture();
		void DieBackStart();
		void DieBackFrameMove();
		void DieBackEventProcess();

		void WinFrameMove();
		//void WinCameraMove();

		//ESSI_WAIT
		void WaitStartFuture();
		void WaitStart();
		//void WaitFrameMoveFuture();
		void WaitEventProcess();

		//ESSI_WALK
#ifdef TEST_MACRO_COMBO
		void WalkStart();
#endif TEST_MACRO_COMBO
		void WalkEventProcess();

		//ESSI_JUMP_READY
		void JumpReadyEventProcess();

		//ESSI_JUMP_UP
		void JumpUpEventProcess();

		//ESSI_JUMP_DOWN
		void JumpDownEventProcess();

		//ESSI_JUMP_LANDING
		void JumpLandingEventProcess();

		//ESSI_DASH
		void DashEventProcess();

		//ESSI_DASH_END
		void DashEndStartFuture();
		void DashEndStart();
		void DashEndFrameMove();
		void DashEndEventProcess();

		//ESSI_DASH_JUMP
		void DashJumpStartFuture();
		void DashJumpStart();
		void DashJumpFrameMoveFuture();
		void DashJumpEventProcess();
		void DashJumpEndFuture();

		//ESSI_DASH_JUMP_LANDING
		void DashJumpLandingEventProcess();

		//ESSI_DASH_JUMP_POWER_LANDING
		void DashJumpPowerLandingStartFuture();
		void DashJumpPowerLandingStart();
		void DashJumpPowerLandingEventProcess();

		//ESSI_HYPER_MODE
		void HyperModeFrameMove();

#ifdef MODIFY_RIDING_PET_AWAKE
		void RidingHyperModeFrameMove();
		void CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ = false );
#endif // MODIFY_RIDING_PET_AWAKE

		////ESSI_DAMAGE_GROGGY
		//void ESSI_DAMAGE_GROGGY_FrameMoveFuture();
		//void ESSI_DAMAGE_GROGGY_EventProcess();

		//ESSI_DAMAGE_SMALL_FRONT
		void DamageSmallFrontEventProcess();

		//ESSI_DAMAGE_SMALL_BACK
		void DamageSmallBackEventProcess();

		//ESSI_DAMAGE_BIG_FRONT
		void DamageBigFrontStart();
		void DamageBigFrontEventProcess();

		//ESSI_DAMAGE_BIG_BACK
		void DamageBigBackStart();
		void DamageBigBackEventProcess();

		//ESSI_DAMAGE_DOWN_FRONT
		void DamageDownFrontFrameMove();
		void DamageDownFrontEventProcess();

		//ESSI_DAMAGE_DOWN_BACK
		void DamageDownBackFrameMove();
		void DamageDownBackEventProcess();

		//ESSI_DAMAGE_AIR_DOWN_LANDING
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

		void StandUpAttackFrontEventProcess();
		void StandUpAttackFrontEnd();

		void StandUpAttackBackEventProcess();
		void StandUpAttackBackEnd();

		//ESSI_REVENGE_WAIT
		void RevengeWaitStart();
		void RevengeWaitFrameMoveFuture();
		void RevengeWaitFrameMove();
		void RevengeWaitEventProcess();

		//ESSI_REVENGE_ATTACK
		void RevengeAttackStart();
		void RevengeAttackFrameMoveFuture();
		void RevengeAttackFrameMove();
		void RevengeAttackCameraMove();
		void RevengeAttackEventProcess();

		//ESSI_COMBO_Z1
		void ComboZ1FrameMoveFuture();
		void ComboZ1FrameMove();
		void ComboZ1EventProcess();

		//ESSI_COMBO_Z2
		void ComboZ2FrameMove();
		void ComboZ2EventProcess();

		//ESSI_COMBO_Z3
		void ComboZ3FrameMove();
		void ComboZ3EventProcess();

		//ESSI_COMBO_Z4
		void ComboZ4FrameMoveFuture();
		void ComboZ4FrameMove();
		void ComboZ4EventProcess();

		//ESSI_COMBO_Z4a
		void ComboZ4aFrameMove();
		void ComboZ4aEventProcess();

		//ESSI_COMBO_X1
		void ComboX1FrameMoveFuture();
		void ComboX1FrameMove();
		void ComboX1EventProcess();

		//ESSI_COMBO_X2
		void ComboX2FrameMoveFuture();
		void ComboX2FrameMove();
		void ComboX2EventProcess();

		//ESSI_COMBO_X3
		void ComboX3FrameMoveFuture();
		void ComboX3FrameMove();
		void ComboX3EventProcess();

		//ESSI_COMBO_X3a
		void ComboX3aFrameMove();
		void ComboX3aEventProcess();

		//ESSI_DASH_COMBO_Z1
		void DashComboZ1FrameMove();
		void DashComboZ1EventProcess();

		//ESSI_DASH_COMBO_Z2
		void DashComboZ2StartFuture();
		void DashComboZ2FrameMove();
		void DashComboZ2EventProcess();

		//ESSI_KNIGHT_DASH_COMBO_Z2
		void ESSI_KNIGHT_DASH_COMBO_Z2_StateStartFuture();
		void ESSI_KNIGHT_DASH_COMBO_Z2_EventProcess();

		//ESSI_KNIGHT_DASH_COMBO_Z3
		void ESSI_KNIGHT_DASH_COMBO_Z3_StateStartFuture();
		void ESSI_KNIGHT_DASH_COMBO_Z3_EventProcess();

		//ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3
		void ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3_StateStartFuture();
		void ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3_EventProcess();

		//ESSI_KNIGHT_DASH_COMBO_Z3a
		void ESSI_KNIGHT_DASH_COMBO_Z3a_StateStartFuture();
		void ESSI_KNIGHT_DASH_COMBO_Z3a_EventProcess();

		//ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a
		void ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_FrameMove();
		void ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_EventProcess();
		void ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_StateEndFuture();
		

		

		//ESSI_DASH_ATTACK_X
		void DashAttackXStartFuture();
		void DashAttackXFrameMove();
		void DashAttackXEventProcess();

		//ESSI_JUMP_ATTACK_Z
		void JumpAttackZFrameMoveFuture();
		void JumpAttackZFrameMove();
		void JumpAttackZEventProcess();

		//ESSI_JUMP_ATTACK_X
		void JumpAttackXFrameMoveFuture();
		void JumpAttackXFrameMove();
		void JumpAttackXEventProcess();

		//ESSI_DASH_JUMP_COMBO_Z1
		void DashJumpComboZ1FrameMove();
		void DashJumpComboZ1EventProcess();

		//ESSI_DASH_JUMP_COMBO_Z2
		void DashJumpComboZ2FrameMove();
		void DashJumpComboZ2EventProcess();

		//ESSI_DASH_JUMP_COMBO_Z3
		void DashJumpComboZ3FrameMove();
		void DashJumpComboZ3EventProcess();

		//ESSI_DASH_JUMP_COMBO_Z3_LANDING
		void DashJumpComboZ3LandingStartFuture();
		void DashJumpComboZ3LandingStart();
		void DashJumpComboZ3LandingEventProcess();

		//ESSI_DASH_JUMP_COMBO_X1
		void DashJumpComboX1FrameMove();
		void DashJumpComboX1EventProcess();

		//ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X1
		//void ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X1_EventProcess();

		//ESSI_DASH_JUMP_COMBO_X2
		void DashJumpComboX2FrameMoveFuture();
		void DashJumpComboX2FrameMove();
		void DashJumpComboX2EventProcess();
		void DashJumpComboX2End();

		//ESSI_DASH_JUMP_COMBO_X2_LANDING
		void DashJumpComboX2LandingStartFuture();
		void DashJumpComboX2LandingStart();
		void DashJumpComboX2LandingEventProcess();

		//ESSI_KNIGHT_DASH_JUMP_COMBO_X2
		void ESSI_KNIGHT_DASH_JUMP_COMBO_X2_FrameMoveFuture();
		void ESSI_KNIGHT_DASH_JUMP_COMBO_X2_FrameMove();
		void ESSI_KNIGHT_DASH_JUMP_COMBO_X2_EventProcess();
		void ESSI_KNIGHT_DASH_JUMP_COMBO_X2_End();

		//ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a
		void ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_FrameMoveFuture();
		void ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_FrameMove();
		void ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_EventProcess();

		//ESSI_KNIGHT_DASH_JUMP_COMBO_X3
		void ESSI_KNIGHT_DASH_JUMP_COMBO_X3_EventProcess();

	
		////////////////////////////////////////////////////////////////////////
		//ESSI_SI_ES_POWER_ATTACK
		void ESSI_SI_ES_POWER_ATTACK_FrameMoveFuture();
		void ESSI_SI_ES_POWER_ATTACK_FrameMove();
		void ESSI_SI_ES_POWER_ATTACK_EventProcess();

		//ESSI_SI_ES_POWER_ATTACK_HYPER
		void ESSI_SI_ES_POWER_ATTACK_HYPER_FrameMoveFuture();
		void ESSI_SI_ES_POWER_ATTACK_HYPER_FrameMove();
		void ESSI_SI_ES_POWER_ATTACK_HYPER_EventProcess();

		//ESSI_SI1_ES_MEGA_SLASH
		void ESSI_SI1_ES_MEGA_SLASH_Init();
		void ESSI_SI1_ES_MEGA_SLASH_FrameMoveFuture();
		void ESSI_SI1_ES_MEGA_SLASH_FrameMove();
		void ESSI_SI1_ES_MEGA_SLASH_CameraMove();
		void ESSI_SI1_ES_MEGA_SLASH_EventProcess();

		//ESSI_SI1_ES_MEGA_SLASH_HYPER
		void ESSI_SI1_ES_MEGA_SLASH_HYPER_FrameMoveFuture();
		void ESSI_SI1_ES_MEGA_SLASH_HYPER_FrameMove();
		void ESSI_SI1_ES_MEGA_SLASH_HYPER_CameraMove();
		void ESSI_SI1_ES_MEGA_SLASH_HYPER_EventProcess();


		//ESSI_SI2_ES_FLAME_GEYSER
		void ESSI_SI2_ES_FLAME_GEYSER_Init();
		void ESSI_SI2_ES_FLAME_GEYSER_Start();
		//void ESSI_SI2_ES_FLAME_GEYSER_FrameMoveFuture();
		void ESSI_SI2_ES_FLAME_GEYSER_FrameMove();
		void ESSI_SI2_ES_FLAME_GEYSER_CameraMove();
		void ESSI_SI2_ES_FLAME_GEYSER_EventProcess();

		//ESSI_SI2_ES_FLAME_GEYSER_HYPER
		void ESSI_SI2_ES_FLAME_GEYSER_HYPER_Start();
		//void ESSI_SI2_ES_FLAME_GEYSER_HYPER_FrameMoveFuture();
		void ESSI_SI2_ES_FLAME_GEYSER_HYPER_FrameMove();
		void ESSI_SI2_ES_FLAME_GEYSER_HYPER_CameraMove();
		void ESSI_SI2_ES_FLAME_GEYSER_HYPER_EventProcess();


		//ESSI_SI3_ES_UNLIMITED_BLADE_LV1
		void ESSI_SI3_ES_UNLIMITED_BLADE_Init();
		void ESSI_SI3_ES_UNLIMITED_BLADE_FrameMoveFuture();
		void ESSI_SI3_ES_UNLIMITED_BLADE_FrameMove();
		void ESSI_SI3_ES_UNLIMITED_BLADE_CameraMove();
		void ESSI_SI3_ES_UNLIMITED_BLADE_EventProcess();

		//ESSI_SI3_ES_UNLIMITED_BLADE_HYPER
		void ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_Init();
		void ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_FrameMoveFuture();
		void ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_FrameMove();
		void ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_CameraMove();
		void ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_EventProcess();



		//ESSI_SI1_ES_AIR_SLASH
		void ESSI_SI1_ES_AIR_SLASH_Init();
		void ESSI_SI1_ES_AIR_SLASH_FrameMoveFuture();
		void ESSI_SI1_ES_AIR_SLASH_FrameMove();
		void ESSI_SI1_ES_AIR_SLASH_CameraMove();
		void ESSI_SI1_ES_AIR_SLASH_EventProcess();

		//ESSI_SI1_ES_AIR_SLASH_HYPER
		void ESSI_SI1_ES_AIR_SLASH_HYPER_FrameMoveFuture();
		void ESSI_SI1_ES_AIR_SLASH_HYPER_FrameMove();
		void ESSI_SI1_ES_AIR_SLASH_HYPER_CameraMove();
		void ESSI_SI1_ES_AIR_SLASH_HYPER_EventProcess();

		//ESSI_SI1_ES_AIR_SLASH_NEXT
		void ESSI_SI1_ES_AIR_SLASH_NEXT_FrameMove();
		void ESSI_SI1_ES_AIR_SLASH_NEXT_EventProcess();

		//ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT
		void ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT_FrameMove();
		void ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT_EventProcess();

		//ESSI_SI1_ES_ASSAULT_SLASH
		void ESSI_SI1_ES_ASSAULT_SLASH_Init();
		void ESSI_SI1_ES_ASSAULT_SLASH_Start();
		void ESSI_SI1_ES_ASSAULT_SLASH_FrameMoveFuture();
		void ESSI_SI1_ES_ASSAULT_SLASH_FrameMove();
		void ESSI_SI1_ES_ASSAULT_SLASH_CameraMove();
		void ESSI_SI1_ES_ASSAULT_SLASH_EventProcess();

		//ESSI_SI1_ES_ASSAULT_SLASH_HYPER
		void ESSI_SI1_ES_ASSAULT_SLASH_HYPER_FrameMoveFuture();
		void ESSI_SI1_ES_ASSAULT_SLASH_HYPER_FrameMove();
		void ESSI_SI1_ES_ASSAULT_SLASH_HYPER_CameraMove();
		void ESSI_SI1_ES_ASSAULT_SLASH_HYPER_EventProcess();


		//{{ dmlee 2009.6.2 obsolete!! 사용하지 않는 필살기
		////ESSI_SI2_ES_LUNA_BLADE
		//void ESSI_SI2_ES_LUNA_BLADE_Init();
		//void ESSI_SI2_ES_LUNA_BLADE_FrameMoveFuture();
		//void ESSI_SI2_ES_LUNA_BLADE_FrameMove();
		//void ESSI_SI2_ES_LUNA_BLADE_CameraMove();
		//void ESSI_SI2_ES_LUNA_BLADE_EventProcess();

		////ESSI_SI2_ES_LUNA_BLADE_HYPER
		//void ESSI_SI2_ES_LUNA_BLADE_HYPER_FrameMoveFuture();
		//void ESSI_SI2_ES_LUNA_BLADE_HYPER_FrameMove();
		//void ESSI_SI2_ES_LUNA_BLADE_HYPER_CameraMove();
		//void ESSI_SI2_ES_LUNA_BLADE_HYPER_EventProcess();
		//}} dmlee 2009.6.2 obsolete!! 사용하지 않는 필살기

		//ESSI_SI2_ES_FATAL_FURY
		void ESSI_SI2_ES_FATAL_FURY_Init();
		void ESSI_SI2_ES_FATAL_FURY_FrameMoveFuture();
		void ESSI_SI2_ES_FATAL_FURY_FrameMove();
		void ESSI_SI2_ES_FATAL_FURY_CameraMove();
		void ESSI_SI2_ES_FATAL_FURY_EventProcess();

		//ESSI_SI2_ES_FATAL_FURY_HYPER
		void ESSI_SI2_ES_FATAL_FURY_HYPER_FrameMoveFuture();
		void ESSI_SI2_ES_FATAL_FURY_HYPER_FrameMove();
		void ESSI_SI2_ES_FATAL_FURY_HYPER_CameraMove();
		void ESSI_SI2_ES_FATAL_FURY_HYPER_EventProcess();


		//ESSI_SI2_ES_SPIRAL_BLAST
		void ESSI_SI2_ES_SPIRAL_BLAST_Init();
		void ESSI_SI2_ES_SPIRAL_BLAST_FrameMoveFuture();
		void ESSI_SI2_ES_SPIRAL_BLAST_FrameMove();
		void ESSI_SI2_ES_SPIRAL_BLAST_CameraMove();
		void ESSI_SI2_ES_SPIRAL_BLAST_EventProcess();

		//ESSI_SI2_ES_SPIRAL_BLAST_HYPER
		void ESSI_SI2_ES_SPIRAL_BLAST_HYPER_FrameMoveFuture();
		void ESSI_SI2_ES_SPIRAL_BLAST_HYPER_FrameMove();
		void ESSI_SI2_ES_SPIRAL_BLAST_HYPER_CameraMove();
		void ESSI_SI2_ES_SPIRAL_BLAST_HYPER_EventProcess();


		//ESSI_SI3_ES_TRIPLE_GEYSER
		void ESSI_SI3_ES_TRIPLE_GEYSER_Init();
		void ESSI_SI3_ES_TRIPLE_GEYSER_FrameMoveFuture();
		void ESSI_SI3_ES_TRIPLE_GEYSER_FrameMove();
		void ESSI_SI3_ES_TRIPLE_GEYSER_CameraMove();
		void ESSI_SI3_ES_TRIPLE_GEYSER_EventProcess();

		//ESSI_SI3_ES_TRIPLE_GEYSER_HYPER
		void ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_FrameMoveFuture();
		void ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_FrameMove();
		void ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_CameraMove();
		void ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_EventProcess();


		//ESSI_SI3_ES_ARMAGEDDON_BLADE
		void ESSI_SI3_ES_ARMAGEDDON_BLADE_Init();
		void ESSI_SI3_ES_ARMAGEDDON_BLADE_FrameMove();
		void ESSI_SI3_ES_ARMAGEDDON_BLADE_EventProcess();

		//ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER
		void ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER_FrameMove();
		void ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER_EventProcess();

		//ESSI_SI1_ES_WIND_BLADE
		void ESSI_SI1_ES_WIND_BLADE_Init();
		void ESSI_SI1_ES_WIND_BLADE_FrameMove();
		void ESSI_SI1_ES_WIND_BLADE_EventProcess();

		//ESSI_SI1_ES_WIND_BLADE_HYPER
		void ESSI_SI1_ES_WIND_BLADE_HYPER_FrameMove();
		void ESSI_SI1_ES_WIND_BLADE_HYPER_EventProcess();

		//ESSI_SI2_ES_RISING_SLASH
		void ESSI_SI2_ES_RISING_SLASH_Init();
		void ESSI_SI2_ES_RISING_SLASH_FrameMove();
		void ESSI_SI2_ES_RISING_SLASH_EventProcess();

		//ESSI_SI2_ES_RISING_SLASH_HYPER
		void ESSI_SI2_ES_RISING_SLASH_HYPER_FrameMove();
		void ESSI_SI2_ES_RISING_SLASH_HYPER_EventProcess();

		//ESSI_SI3_ES_STORM_BLADE
		void ESSI_SI3_ES_STORM_BLADE_Init();
		void ESSI_SI3_ES_STORM_BLADE_FrameMove();
		void ESSI_SI3_ES_STORM_BLADE_EventProcess();

		//ESSI_SI3_ES_STORM_BLADE_HYPER
		void ESSI_SI3_ES_STORM_BLADE_HYPER_FrameMove();
		void ESSI_SI3_ES_STORM_BLADE_HYPER_EventProcess();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한

		//ESSI_A_ELK_IMPACT_SMASH
		void ESSI_A_ELK_IMPACT_SMASH_Init();
		void ESSI_A_ELK_IMPACT_SMASH_EventProcess();

		//ESSI_A_ES_SWORD_WAVE
		void ESSI_A_ES_SWORD_WAVE_Init();
		void ESSI_A_ES_SWORD_WAVE_EventProcess();

		//ESSI_SI_A_ES_DEFENCE
		void ESSI_SI_A_ES_DEFENCE_READY_StartFuture();
		void ESSI_SI_A_ES_DEFENCE_READY_EventProcess();

		void ESSI_SI_A_ES_DEFENCE_WAIT_Start();
		void ESSI_SI_A_ES_DEFENCE_WAIT_FrameMove();
		void ESSI_SI_A_ES_DEFENCE_WAIT_EventProcess();

		void ESSI_SI_A_ES_DEFENCE_END_Start();
		void ESSI_SI_A_ES_DEFENCE_END_EventProcess();
		
		void ESSI_SI_A_ES_DEFENCE_DAMAGE_FrameMove();
		void ESSI_SI_A_ES_DEFENCE_DAMAGE_EventProcess();
		
		//ESSI_SI_P_ESK_AUTO_GUARD
		void ESSI_SI_P_ESK_AUTO_GUARD_Start();
		void ESSI_SI_P_ESK_AUTO_GUARD_EventProcess();
		void ESSI_SI_P_ESK_AUTO_GUARD_End();

		//ESSI_SI_SA_EMK_RISING_SLASH_WAVE
		void ESSI_SI_SA_EMK_RISING_SLASH_WAVE_Init();
		void ESSI_SI_SA_EMK_RISING_SLASH_WAVE_EventProcess();
		void ESSI_SI_SA_EMK_RISING_SLASH_WAVE_FrameMove();
		void ESSI_SI_SA_EMK_RISING_SLASH_WAVE_HYPER_FrameMove();

		//ESSI_SI_A_ERS_CRITICAL_SWORD
		void ESSI_SI_A_ERS_CRITICAL_SWORD_Init();
		void ESSI_SI_A_ERS_CRITICAL_SWORD_FrameMove();
		void ESSI_SI_A_ERS_CRITICAL_SWORD_EventProcess();

		//ESSI_SI_A_ERS_RUNE_OF_FIRE
		void ESSI_SI_A_ERS_RUNE_OF_FIRE_Init();
		void ESSI_SI_A_ERS_RUNE_OF_FIRE_EventProcess();
		void ESSI_SI_A_ERS_RUNE_OF_FIRE_FrameMove();

		//ESSI_SI_A_ERS_RUNE_OF_ICE
		void ESSI_SI_A_ERS_RUNE_OF_ICE_Init();
		void ESSI_SI_A_ERS_RUNE_OF_ICE_EventProcess();
		void ESSI_SI_A_ERS_RUNE_OF_ICE_FrameMove();

		//ESSI_SI_A_ETK_PIERCING_SWORD
		void ESSI_SI_A_ETK_PIERCING_SWORD_Init();
		void ESSI_SI_A_ETK_PIERCING_SWORD_EventProcess();
		void ESSI_SI_A_ETK_PIERCING_SWORD_StateStart();
		void ESSI_SI_A_ETK_PIERCING_SWORD_FrameMove();
		void ESSI_SI_A_ETK_PIERCING_SWORD_End();

		//ESSI_SI_A_EIS_FATAL_SLAP
		void ESSI_SI_A_EIS_FATAL_SLAP_Init();
		void ESSI_SI_A_EIS_FATAL_SLAP_EventProcess();

		//ESSI_A_ETK_SWORD_SHIELD
		void ESSI_A_ETK_SWORD_SHIELD_Init();
		void ESSI_A_ETK_SWORD_SHIELD_StateStart();
		void ESSI_A_ETK_SWORD_SHIELD_FrameMove();
		void ESSI_A_ETK_SWORD_SHIELD_EventProcess();

		//ESSI_A_ESK_WEAPON_BREAK
		void ESSI_A_ESK_WEAPON_BREAK_Init();
#ifndef ADD_MEMO_1ST_CLASS //김창한
		void ESSI_A_ESK_WEAPON_BREAK_StateStart();
#endif //ADD_MEMO_1ST_CLASS
		void ESSI_A_ESK_WEAPON_BREAK_EventProcess();
		
#endif //UPGRADE_SKILL_SYSTEM_2013 

		//COMMON_BUFF
		virtual void COMMON_BUFF_FrameMove();
#ifndef SPECIAL_USE_ITEM
		//ESSI_THROW_WOODEN_PIECE
		void THROW_WOODEN_PIECE_FrameMove();
#endif SPECIAL_USE_ITEM
		//ESSI_LEAP
		void ESSI_LEAP_StartFuture();
		void ESSI_LEAP_FrameMoveFuture();
		void ESSI_LEAP_EventProcess();
		void ESSI_LEAP_End();




		void ESSI_SI_A_ES_KICK_Start();
		void ESSI_SI_A_ES_KICK_EventProcess();

		
		void StandUpAttackFrontNoDownEventProcess();
		void StandUpAttackFrontNoDownEnd();

		void StandUpAttackBackNoDownEventProcess();
		void StandUpAttackBackNoDownEnd();




	void ESSI_SA_ESK_WINDMILL_START_Init();
	void ESSI_SA_ESK_WINDMILL_START_StateStart();
	void ESSI_SA_ESK_WINDMILL_START_FrameMove();
	void ESSI_SA_ESK_WINDMILL_START_EventProcess();
	void ESSI_SA_ESK_WINDMILL_START_StateEnd();

	void ESSI_SA_ESK_WINDMILL_START_HYPER_StateStart();
	void ESSI_SA_ESK_WINDMILL_START_HYPER_FrameMove();
	void ESSI_SA_ESK_WINDMILL_START_HYPER_EventProcess();
	void ESSI_SA_ESK_WINDMILL_START_HYPER_StateEnd();


	void ESSI_SA_ESK_WINDMILL_FrameMove();
	void ESSI_SA_ESK_WINDMILL_EventProcess();
	void ESSI_SA_ESK_WINDMILL_StateEnd();
	void ESSI_SA_ESK_WINDMILL_HYPER_FrameMove();
	void ESSI_SA_ESK_WINDMILL_HYPER_EventProcess();
	void ESSI_SA_ESK_WINDMILL_HYPER_StateEnd();


	void ESSI_SA_ESK_WINDMILL_END_FrameMove();
	void ESSI_SA_ESK_WINDMILL_END_EventProcess();
	void ESSI_SA_ESK_WINDMILL_END_HYPER_FrameMove();
	void ESSI_SA_ESK_WINDMILL_END_HYPER_EventProcess();
		
	void ESSI_SA_EMK_SWORD_FIRE_Init();
	void ESSI_SA_EMK_SWORD_FIRE_StartFuture();
	void ESSI_SA_EMK_SWORD_FIRE_FrameMoveFuture();
	void ESSI_SA_EMK_SWORD_FIRE_FrameMove();
	void ESSI_SA_EMK_SWORD_FIRE_EventProcess();
	void ESSI_SA_EMK_SWORD_FIRE_EndFuture();

	void ESSI_SA_EMK_SWORD_FIRE_HYPER_Init();
	void ESSI_SA_EMK_SWORD_FIRE_HYPER_StartFuture();
	void ESSI_SA_EMK_SWORD_FIRE_HYPER_FrameMoveFuture();
	void ESSI_SA_EMK_SWORD_FIRE_HYPER_FrameMove();
	void ESSI_SA_EMK_SWORD_FIRE_HYPER_EventProcess();
	void ESSI_SA_EMK_SWORD_FIRE_HYPER_EndFuture();
	
	void ESSI_SA_EMK_SWORD_FIRE_LANDING_FrameMove();
	void ESSI_SA_EMK_SWORD_FIRE_LANDING_EventProcess();
	void ESSI_SA_EMK_SWORD_FIRE_LANDING_End();

	void ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_FrameMove();
	void ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_EventProcess();
	void ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_End();

	//ESSI_SA_EMK_BIG_BURST
	void ESSI_SA_EMK_BIG_BURST_Init();
	void ESSI_SA_EMK_BIG_BURST_FrameMoveFuture();
	void ESSI_SA_EMK_BIG_BURST_FrameMove();
	void ESSI_SA_EMK_BIG_BURST_EventProcess();
	void ESSI_SA_EMK_BIG_BURST_End();



	void ESSI_A_ESK_ARMOR_BREAK_Init();
	void ESSI_A_ESK_ARMOR_BREAK_StateStart();
	void ESSI_A_ESK_ARMOR_BREAK_FrameMove();
	void ESSI_A_ESK_ARMOR_BREAK_EventProcess();
	void ESSI_A_ESK_ARMOR_BREAK_StateEnd();

	void ESSI_A_ESK_ARMOR_BREAK_HYPER_FrameMove();
	void ESSI_A_ESK_ARMOR_BREAK_HYPER_EventProcess();
	void ESSI_A_ESK_ARMOR_BREAK_HYPER_StateEnd();









#ifdef SKILL_CASH_10_TEST

	void ESSI_A_ESK_PHOENIX_TALON_Init();
	void ESSI_A_ESK_PHOENIX_TALON_StateStart();
	void ESSI_A_ESK_PHOENIX_TALON_FrameMove();
	void ESSI_A_ESK_PHOENIX_TALON_Hyper_FrameMove();
	void ESSI_A_ESK_PHOENIX_TALON_EventProcess();
	void ESSI_A_ESK_PHOENIX_TALON_StateEnd();
	

	void ESSI_SA_ESK_DOUBLE_SLASH_FrameMove();
	void ESSI_SA_ESK_DOUBLE_SLASH_EventProcess();
	void ESSI_SA_ESK_DOUBLE_SLASH_StateEnd();

	void ESSI_SA_ESK_DOUBLE_SLASH_HYPER_FrameMove();
#endif	//	SKILL_CASH_10_TEST







	// 2차 전직
	void ESSI_ELK_ZZZfrontZ_FrameMove();
	void ESSI_ELK_ZZZfrontZ_EventProcess();
	void ESSI_ELK_XXX_FrameMove();
	void ESSI_ELK_XXX_EventProcess();
	void ESSI_ELK_XXXX_FrameMove();
	void ESSI_ELK_XXXX_EventProcess();


	void ESSI_ERS_XZ_StateStartFuture();
	void ESSI_ERS_XZ_FrameMoveFuture();
	void ESSI_ERS_XZ_FrameMove();
	void ESSI_ERS_XZ_EventProcess();
	void ESSI_ERS_XZZ_FrameMove();
	void ESSI_ERS_XZZ_EventProcess();


	void ESSI_ERS_DASH_JUMP_XZ_StateStartFuture();
	void ESSI_ERS_DASH_JUMP_XZ_StateEndFuture();
	void ESSI_ERS_DASH_JUMP_XZ_FrameMoveFuture();
	void ESSI_ERS_DASH_JUMP_XZ_FrameMove();
	void ESSI_ERS_DASH_JUMP_XZ_EventProcess();


	void ESSI_ERS_DASH_JUMP_XZZ_FrameMove();
	void ESSI_ERS_DASH_JUMP_XZZ_EventProcess();




	void ESSI_SA_ELK_SONIC_BLADE_Init();
	void ESSI_SA_ELK_SONIC_BLADE_StateStart();
	void ESSI_SA_ELK_SONIC_BLADE_FrameMove();
	void ESSI_SA_ELK_SONIC_BLADE_EventProcess();
	void ESSI_SA_ELK_SONIC_BLADE_StateEnd();

	void ESSI_SA_ELK_SONIC_BLADE_HYPER_StateStart();
	void ESSI_SA_ELK_SONIC_BLADE_HYPER_FrameMove();
	void ESSI_SA_ELK_SONIC_BLADE_HYPER_EventProcess();
	void ESSI_SA_ELK_SONIC_BLADE_HYPER_StateEnd();


	void ESSI_SA_ERS_SPLASH_EXPLOSION_Init();
	void ESSI_SA_ERS_SPLASH_EXPLOSION_StateStart();
	void ESSI_SA_ERS_SPLASH_EXPLOSION_FrameMove();
	//void ESSI_SA_ERS_SPLASH_EXPLOSION_EventProcess();
	void ESSI_SA_ERS_SPLASH_EXPLOSION_StateEnd();

	void ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_StateStart();
	void ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_FrameMove();
	//void ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_EventProcess();
	void ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_StateEnd();


	void ESSI_A_ELK_ROLLING_SMASH_READY_Init();
	void ESSI_A_ELK_ROLLING_SMASH_READY_StateStart();
	void ESSI_A_ELK_ROLLING_SMASH_READY_FrameMove();
	void ESSI_A_ELK_ROLLING_SMASH_READY_EventProcess();
	void ESSI_A_ELK_ROLLING_SMASH_READY_StateEnd();

	void ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_StateStart();
	void ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_FrameMove();
	void ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_EventProcess();
	void ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_StateEnd();



	void ESSI_A_ELK_ROLLING_SMASH_StateStartFuture();
	void ESSI_A_ELK_ROLLING_SMASH_FrameMove();	
	void ESSI_A_ELK_ROLLING_SMASH_EventProcess();	
	void ESSI_A_ELK_ROLLING_SMASH_End();			

	void ESSI_A_ELK_ROLLING_SMASH_HYPER_StateStartFuture();
	void ESSI_A_ELK_ROLLING_SMASH_HYPER_EventProcess();





	void ESSI_A_ERS_SWORD_ENCHANT_Init();
	void ESSI_A_ERS_SWORD_ENCHANT_FrameMove();
	void ESSI_A_ERS_SWORD_ENCHANT_EventProcess();
	
//{{ JHKang / 강정훈 / 2010/10/31 / 룬 슬레이어 루나 블레이드 추가
#ifdef NEW_SKILL_2010_11
	/** 
		@function
			- ESSI_SA_ERS_LUNA_BLADE_Init
			- ESSI_SA_ERS_LUNA_BLADE_Start
			- ESSI_SA_ERS_LUNA_BLADE_FrameMove
			- ESSI_SA_ERS_LUNA_BLADE_EventProcess
			- ESSI_SA_ERS_LUNA_BLADE_HYPER_Start
			- ESSI_SA_ERS_LUNA_BLADE_HYPER_FrameMove
			- ESSI_SA_ERS_LUNA_BLADE_HYPER_EventProcess
		@brief
			- 관련된 리소스 로딩
			- 설정 정보 가져오기
			- 애니메이션 조절
			- 이벤트 설정
	*/
	void ESSI_SA_ERS_LUNA_BLADE_Init();
	void ESSI_SA_ERS_LUNA_BLADE_FrameMove();
	void ESSI_SA_ERS_LUNA_BLADE_EventProcess();
#endif NEW_SKILL_2010_11
//}} JHKang / 강정훈 / 2010/10/31 / 룬 슬레이어 루나 블레이드 추가
#ifdef NEW_SKILL_2010_11
	//{{ oasis907 : 김상윤 [2010.11.5] // 로드 나이트 - 샌드 스톰
	void ESSI_SA_ELK_SAND_STORM_FrameMoveFuture();
	void ESSI_SA_ELK_SAND_STORM_FrameMove();
	void ESSI_SA_ELK_SAND_STORM_EventProcess();
	
	//}} oasis907 : 김상윤 [2010.11.5] // 로드 나이트 - 샌드 스톰
#endif NEW_SKILL_2010_11

#ifdef ELSWORD_SHEATH_KNIGHT
	void ESSI_ETK_COMBO_ZZZ_StateStart();
	void ESSI_ETK_COMBO_ZZZ_FrameMove();
	void ESSI_ETK_COMBO_ZZZ_EventProcess();
	void ESSI_ETK_COMBO_ZZZ_StateEnd();
	void ESSI_ETK_COMBO_ZZZZ_FrameMove();
	void ESSI_ETK_COMBO_ZZZZ_EventProcess();
	void ESSI_ETK_COMBO_ZZZZ_StateEnd();

	void ESSI_ETK_COMBO_ZZZZZ_StateStart();
	void ESSI_ETK_COMBO_ZZZZZ_FrameMove();
	void ESSI_ETK_COMBO_ZZZZZ_EventProcess();
	void ESSI_ETK_COMBO_ZZZZZ_StateEnd();

	void ESSI_ETK_COMBO_XX_StateStart();
	void ESSI_ETK_COMBO_XX_FrameMove();
	void ESSI_ETK_COMBO_XX_EventProcess();
	void ESSI_ETK_COMBO_XX_StateEnd();

	void ESSI_ETK_COMBO_XXX_LOOP_StateStart();
	void ESSI_ETK_COMBO_XXX_LOOP_FrameMove();
	void ESSI_ETK_COMBO_XXX_LOOP_EventProcess();
	void ESSI_ETK_COMBO_XXX_LOOP_StateEnd();

	void ESSI_ETK_COMBO_XXXZ_FrameMove();
	void ESSI_ETK_COMBO_XXXZ_EventProcess();
	void ESSI_ETK_COMBO_XXXZ_StateEnd();

	void ESSI_ETK_COMBO_XXX_END_StateStart();
	void ESSI_ETK_COMBO_XXX_END_FrameMove();
	void ESSI_ETK_COMBO_XXX_END_EventProcess();
	void ESSI_ETK_COMBO_XXX_END_StateEnd();

	void ESSI_ETK_DASH_COMBO_ZZ_FrameMove();
	void ESSI_ETK_DASH_COMBO_ZZ_EventProcess();
	void ESSI_ETK_DASH_COMBO_ZZ_StateEnd();

	void ESSI_SI_A_ES_ROLLING_StateStart();
	void ESSI_SI_A_ES_ROLLING_StateStartFuture();	
	void ESSI_SI_A_ES_ROLLING_FrameMove();	
	void ESSI_SI_A_ES_ROLLING_EventProcess();	

	void ESSI_SI_A_ES_ROLLING_END_StateStart();	
	void ESSI_SI_A_ES_ROLLING_END_FrameMove();	
	void ESSI_SI_A_ES_ROLLING_END_EventProcess();	

	void ESSI_SI_SA_ETK_HARSH_CHASER_StateStart();
	void ESSI_SI_SA_ETK_HARSH_CHASER_FrameMove();	
	void ESSI_SI_SA_ETK_HARSH_CHASER_EventProcess();
	void ESSI_SI_SA_ETK_HARSH_CHASER_End();

	void ESSI_SI_SA_ETK_HARSH_CHASER_PULL_FrameMove();	
	void ESSI_SI_SA_ETK_HARSH_CHASER_PULL_EventProcess();	

	void CompactCounterAttackStart();
	void CompactCounterAttackStartFuture();
	void CompactCounterAttackFrameMoveFuture();
	void CompactCounterAttackFrameMove();
	void CompactCounterAttackEventProcess();
	void CompactCounterAttackStateEnd();

	void CompactCounterGuardAttackStart();
	void CompactCounterGuardAttackStartFuture();
	void CompactCounterGuardAttackFrameMoveFuture();
	void CompactCounterGuardAttackFrameMove();
	void CompactCounterGuardAttackCameraMove();
	void CompactCounterGuardAttackEventProcess();
	void CompactCounterGuardAttackStateEnd();

	void ESSI_SI_SA_ETK_MAELSTORM_RAGE_Init();
	void ESSI_SI_SA_ETK_MAELSTORM_RAGE_StartFuture();
	void ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMoveFuture();
	void ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMove();
	void ESSI_SI_SA_ETK_MAELSTORM_RAGE_EventProcess();
	void ESSI_SI_SA_ETK_MAELSTORM_RAGE_EndFuture();

	void ESSI_SI_SA_ETK_FINAL_STRIKE_Init();
	void ESSI_SI_SA_ETK_FINAL_STRIKE_FrameMove();
	void ESSI_SI_SA_ETK_FINAL_STRIKE_EventProcess();
	void ESSI_SI_SA_ETK_FINAL_STRIKE_End();

	void ESSI_SI_SA_ETK_SWORD_FALL_Init();
	void ESSI_SI_SA_ETK_SWORD_FALL_FrameMove();
	void ESSI_SI_SA_ETK_SWORD_FALL_EventProcess();
	void ESSI_SI_SA_ETK_SWORD_FALL_End();

	void ESSI_SI_SA_ETK_CRESCENT_CUT_Init();
	void ESSI_SI_SA_ETK_CRESCENT_CUT_FrameMove();
	void ESSI_SI_SA_ETK_CRESCENT_CUT_EventProcess();
	void ESSI_SI_SA_ETK_CRESCENT_CUT_End();

	void ESSI_SI_SA_ETK_PHANTOM_SWORD_Init();
	void ESSI_SI_SA_ETK_PHANTOM_SWORD_FrameMove();
	void ESSI_SI_SA_ETK_PHANTOM_SWORD_EventProcess();
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_ELSWORD_INFINITY_SWORD
	void ESSI_EIS_DASH_COMBO_XX_EventProcess();
	void ESSI_EIS_DASH_COMBO_XXX_EventProcess();
	void ESSI_EIS_DASHJUMP_COMBO_XX_EventProcess();
	void ESSI_EIS_DASHJUMP_COMBO_XXX_FrameMove();
	void ESSI_EIS_DASHJUMP_COMBO_XXX_EventProcess();
	void ESSI_EIS_DASHJUMP_COMBO_XXX_End();
	void ESSI_SI_A_EIS_SPIRAL_STING_FrameMove();
	void ESSI_SI_A_EIS_SPIRAL_STING_EventProcess();
	void ESSI_SI_SA_EIS_SWORD_BLASTING_FrameMove();
	void ESSI_SI_SA_EIS_SWORD_BLASTING_EventProcess();
	void ESSI_SI_SA_EIS_RAGE_CUTTER_FrameMove();
	void ESSI_SI_SA_EIS_RAGE_CUTTER_EventProcess();
#endif //SERV_ELSWORD_INFINITY_SWORD


#ifdef ELSWORD_WAY_OF_SWORD
	void SwordWaitStartFuture();
	void SwordWaitStart();
	void SwordWaitEventProcess();
	void PlayAnimationSwordWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ );	

	void WayOfSwordStateChange();
	void DestructionRageFrameMove();

	void DoStateDestructionRage();
	void DestructionRageStateStart();
	void DestructionRageStateFrameMove();
	void DestructionRageStateEventProcess();
#endif ELSWORD_WAY_OF_SWORD

#ifdef GRAPPLING_TEST
	void ESSI_DAMAGE_GRAPPLED_FRONT_StartFuture();
	void ESSI_DAMAGE_GRAPPLED_FRONT_Start();
	void ESSI_DAMAGE_GRAPPLED_FRONT_FrameMoveFuture();
	void ESSI_DAMAGE_GRAPPLED_FRONT_EventProcess();
	void ESSI_DAMAGE_GRAPPLED_FRONT_EndFuture();
	void ESSI_DAMAGE_GRAPPLED_FRONT_End();


	void ESSI_DAMAGE_GRAPPLED_BACK_StartFuture();
	void ESSI_DAMAGE_GRAPPLED_BACK_FrameMoveFuture();
	void ESSI_DAMAGE_GRAPPLED_BACK_EventProcess();
	void ESSI_DAMAGE_GRAPPLED_BACK_End();


	void ESSI_GRAB_FRONT_StartFuture();
	void ESSI_GRAB_FRONT_Start();
	void ESSI_GRAB_FRONT_FrameMoveFuture();
	void ESSI_GRAB_FRONT_EventProcess();
	void ESSI_GRAB_FRONT_End();



	void ESSI_THROW_FORWARD_StartFuture();
	void ESSI_THROW_FORWARD_FrameMoveFuture();
	void ESSI_THROW_FORWARD_EventProcess();
	void ESSI_THROW_FORWARD_EndFuture();
	void ESSI_THROW_FORWARD_End();

#endif GRAPPLING_TEST

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	#pragma region SI_FS_ELK_Gigantic_Slash
	void ESSI_HA_ELK_Gigantic_Slash_Init();
	#pragma endregion 기간틱 슬래시 : 궁극기

	#pragma region SI_FS_ERS_Shining_Rune_Buster
	void ESSI_HA_ERS_Shining_Rune_Buster_Init();
	void ESSI_HA_ERS_Shining_Rune_Buster_EventProcess();

	#pragma endregion 샤이닝 룬 버스트 : 궁극기

	#pragma region SI_FS_EIS_Blade_Rain
	void ESSI_HA_EIS_Blade_Rain_Init();
	void ESSI_HA_EIS_Blade_Rain_EventProcess();
	#pragma endregion 블레이드 레인 : 궁극기

#endif //FINALITY_SKILL_SYSTEM

	void ShowActiveSkillCutInAndLight( const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ = false );
	virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

	virtual CX2GageData*	CreateGageData();
};


#ifdef UPGRADE_SKILL_SYSTEM_2013

IMPLEMENT_INTRUSIVE_PTR( CX2GUElsword_SwordMan::CElswordGuardSkill );

#endif  UPGRADE_SKILL_SYSTEM_2013
#pragma once

class CX2GURaven : public CX2GUUser
{
	public:
		enum RAVEN_STATE_ID
		{
			RSI_BASE = USI_END + 1,
		
			RSI_DASH_JUMP_POWER_LANDING,

			RSI_STANDUP_ROLLING_FRONT_FRONT,
			RSI_STANDUP_ROLLING_FRONT_BACK,
			RSI_STANDUP_ROLLING_BACK_FRONT,
			RSI_STANDUP_ROLLING_BACK_BACK,

			RSI_STANDUP_ATTACK_FRONT,
			RSI_STANDUP_ATTACK_BACK,

			RSI_SUPER_DASH_READY,
			RSI_SUPER_DASH,
			RSI_SUPER_DASH_END,

			RSI_COMBO_Z,
			RSI_COMBO_ZZ,
			RSI_COMBO_ZZZ,
			RSI_COMBO_ZZZZ,

			RSI_COMBO_ZZZX,
			RSI_COMBO_ZZX,
			RSI_COMBO_ZZXX,

			RSI_COMBO_X,
			RSI_COMBO_XX,
			RSI_COMBO_XXX,

			RSI_DASH_COMBO_Z,
			RSI_DASH_COMBO_ZZ,
			RSI_DASH_COMBO_X,

			RSI_DASH_JUMP_COMBO_Z,
			RSI_DASH_JUMP_COMBO_ZZ,
			RSI_DASH_JUMP_COMBO_ZZ_Landing,
			RSI_DASH_JUMP_COMBO_X,

			RSI_JUMP_COMBO_Z,
			RSI_JUMP_COMBO_X,

			RSI_SOUL_TAKER_COMBO_ZZZupZ,
			RSI_SOUL_TAKER_COMBO_XXZ,		// Z 8연타 준비
			RSI_SOUL_TAKER_COMBO_XXZZ8,		// Z 8연타
			RSI_SOUL_TAKER_COMBO_XXZZ8Z,	// Z 8연타후에
			RSI_SOUL_TAKER_DASH_COMBO_ZZZ,
			
			RSI_OVER_TAKER_COMBO_XXdownX,
			RSI_OVER_TAKER_COMBO_XXdownXdownX,
			RSI_OVER_TAKER_COMBO_XXdownXdownXdownX,
			RSI_OVER_TAKER_COMBO_XXfrontX,

			RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX,
			RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING, 
			RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX,

//#ifdef WALL_JUMP_TEST
			RSI_WALL_LANDING,
//#endif WALL_JUMP_TEST

			RSI_SHADOW_STEP_THRUST,

#ifdef UPGRADE_RAVEN
			RSI_PARRYING_SMALL,
			RSI_PARRYING,
#endif

			RSI_STANDUP_ATTACK_FRONT_NO_DOWN,
			RSI_STANDUP_ATTACK_BACK_NO_DOWN,
			



#ifdef SKILL_30_TEST			
			RSI_SA_ROT_VALKYRIESJAVELIN_LANDING,
			RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_LANDING,
#endif SKILL_30_TEST

#ifdef RAVEN_SECOND_CLASS_CHANGE
			RRF_DASH_JUMP_COMBO_XX,
			RRF_DASH_JUMP_COMBO_XX_LANDING,
			RRF_COMBO_ZZXFRONTX,
			RRF_COMBO_ZZXFRONTXX,

			RBM_COMBO_XZ,
			RBM_COMBO_XZZ,
			RBM_COMBO_XZZZ,			
			RBM_COMBO_ZZZX_EXPLOSION,
			RBM_COMBO_ZZZXZ,

			SI_SA_RBM_WOLF_FANG_STARTLANDING,
			SI_SA_RBM_WOLF_FANG_STARTLANDING_HYPER,
			SI_SA_RBM_WOLF_FANG_HIT,
			SI_SA_RBM_WOLF_FANG_HIT_HYPER,
			SI_SA_RBM_WOLF_FANG_HITLANDING,
#endif
			//{{ JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가
#ifdef NEW_SKILL_2010_11
			RSI_SHADOW_STEP,		/// 긴급 탈출을 위한 섀도우 스텝 상태 추가
#endif NEW_SKILL_2010_11
			//}} JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가

//{{ kimhc // 2010.11.11 // 레피-와일드차지
#ifdef	NEW_SKILL_2010_11

			RSI_SA_RRF_WILD_CHARGING,			/// 레피-와일드차지에서 기를 모으고 있는 상태
			RSI_SA_RRF_WILD_CHARGE_DASH,		/// 레피_와이들차지에서 앞으로 나가면써 때리는 상태
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.11 // 레피-와일드차지

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
			RSI_SHADOW_STEP_THRUST_ENHANCE,		/// 블마-쉐도우 스탭 중 x 키 눌러서 쉐도우 스러스트
			RSI_P_RRF_SHADOW_PUNISHER,			/// 레피-쉐도우 스탭 중 x 키 눌러서 쉐도우 퍼니셔
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef RAVEN_WEAPON_TAKER
			//RSI_COMBO_X,
			RSI_WEAPON_TAKER_COMBO_XX,
			RSI_WEAPON_TAKER_COMBO_XXX,
			RSI_WEAPON_TAKER_COMBO_XXX_LOOP,
			RSI_WEAPON_TAKER_COMBO_XXX_FINISH,

			//RSI_COMBO_ZZZX,
			RSI_WEAPON_TAKER_COMBO_ZZZXX,
			RSI_WEAPON_TAKER_COMBO_ZZZdownX,
			RSI_WEAPON_TAKER_COMBO_ZZZdownXX,

			RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX,
			RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX,

			RSI_A_RWT_BARRAGE_ATTACK_STAND,
			RSI_A_RWT_BARRAGE_ATTACK_AIR,

			RSI_SA_RWT_BURSTING_BLADE_STAND,
			RSI_SA_RWT_BURSTING_BLADE_AIR,

			RSI_SA_RWT_REVOLVER_CANNON_LOOP,
			RSI_SA_RWT_REVOLVER_CANNON_FINISH,
			RSI_SA_RWT_REVOLVER_CANNON_END,
			RSI_SA_RWT_REVOLVER_CANNON_OVER_END,

			RSI_SA_RWT_HELLFIRE_GATLING_LOOP,
			RSI_SA_RWT_HELLFIRE_GATLING_END,
			RSI_SA_RWT_HELLFIRE_GATLING_OVER_END,

			RSI_SA_RWT_GIGA_PROMINENCE_LOOP,
			RSI_SA_RWT_GIGA_PROMINENCE_FINISH,
			RSI_SA_RWT_GIGA_PROMINENCE_END,
			RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH,
			RSI_SA_RWT_GIGA_PROMINENCE_OVER_END,
#endif RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
			//Dash Jump Combo XX
			RVC_DASH_JUMP_COMBO_X,
			RVC_DASH_JUMP_COMBO_XX_START,
			RVC_DASH_JUMP_COMBO_XX_LOOP,
			RVC_DASH_JUMP_COMBO_XX_FINISH,

			//Dash Combo ZZZZ
			RVC_DASH_COMBO_ZZZ,
			RVC_DASH_COMBO_ZZZPushZ_READY,
			RVC_DASH_COMBO_ZZZPushZ,
			RVC_DASH_COMBO_ZZZZ_FINISH,

			//Deadly Raid
			RVC_SI_SA_DEADLY_RAID_LOOP,
			RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT,
			RVC_SI_SA_DEADLY_RAID_LOOP_END,
			RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT,
			RVC_SI_SA_DEADLY_RAID_FINISH,
			RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT,
			RVC_SI_SA_DEADLY_RAID_END,

			//Ignition Crow
			RVC_SI_SA_IGNITION_CROW_FIRE,
			RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT,
			RVC_SI_SA_IGNITION_CROW_OVERHEAT_END,

			//Shadow Back Slide
			RVC_SI_P_SHADOW_BACK_SLIDE,
			RVC_SI_P_SHADOW_BACK_SLIDE_END,
			RVC_SI_P_SHADOW_STEP_FINISH,

			//Shadow Buster
			RVC_SI_P_SHADOW_BUSTER,

			//Bleeding Slicer
			RVC_SI_A_BLEEDING_SLICER_GROUND,
			RVC_SI_A_BLEEDING_SLICER_FLY,

			//Harpoon Spear Fire
			RSI_SA_RWT_HARPOON_SPEAR_FIRE,

			//Harpoon Spear Fire Overheat
			RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT,

#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

			// 화염인
			RSI_A_RWT_FLAME_SWORD_GROUND,
			RSI_A_RWT_FLAME_SWORD_FLY,

			// 리볼버 캐논 HE
			RSI_SA_RWT_REVOLVER_CANNON_HE,
			RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP,
			RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH,
			RSI_SA_RWT_REVOLVER_CANNON_HE_OVER_END,
			RSI_SA_RWT_REVOLVER_CANNON_HE_END,

			// 이그니션 크로우 인시너레이션
			RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE,
			RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT,
			RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END,

			// 리미트 크러셔
			RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK,
			RSI_A_RRF_LIMIT_CRUSHER_AVOID_ATTACK,

#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef FINALITY_SKILL_SYSTEM //김창한
			RSI_HA_RBM_EXTREM_BLADE_READY,			//익스트림 블레이드 FX 공격 준비
			RSI_HA_RBM_EXTREM_BLADE_ATTACK,			//익스트림 블레이드 FX 공격
			RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH,	//익스트림 블레이드 FX 피니쉬 공격
#endif //FINALITY_SKILL_SYSTEM

			//////////////////////////////////////////////////////////////////////////
			// 위쪽에 추가해주세요~ 이 아래는 테스트로 추가된 enum 입니다


			RSI_TRANSFORMED,

//#ifdef PVP_BOSS_COMBAT_TEST
//			RSI_FROZEN,
//#endif PVP_BOSS_COMBAT_TEST

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			RSI_A_RBM_SONIC_SLASH_FINISH,
#endif // UPGRADE_SKILL_SYSTEM_2013

		};

		enum RAVEN_MAJOR_PARTICLE_INSTANCE_ID
		{
			RAVEN_MAJOR_PII_GIGA_DRIVE_WIND_FIRST,
			RAVEN_MAJOR_PII_GIGA_DRIVE_WIND_SECOND,
			RAVEN_MAJOR_PII_GIGA_DRIVE_WIND_THIRD,
			RAVEN_MAJOR_PII_END,
		};

		enum RAVEN_MINOR_PARTICLE_INSTANCE_ID
		{
			RAVEN_MINOR_PII_BASE,		// kimhc // 현재는 쓰이는 것이 없어서 임시로 넣어놓음
			RAVEN_MINOR_PII_END,
		};

		struct ArcEnemyData
		{
			D3DXVECTOR3					m_vOffsetPos;
			D3DXVECTOR3					m_vOffsetRotate;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffectHandle       m_hEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CX2DamageEffect::CEffect*	m_pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			ArcEnemyData();
			~ArcEnemyData();
		};

		/** @struct : WildChargeData
			@brief : 레피-와일드차지 스킬에서 필요로 하는 데이타를 모아 놓은 구조체
			@date  : 2010/11/16
		*/
        struct  WildChargeData;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        typedef boost::intrusive_ptr<WildChargeData> WildChargeDataPtr;	/// WildChargeData 구조체의 스마트 포인터 타입
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		typedef boost::shared_ptr<WildChargeData> WildChargeDataPtr;	/// WildChargeData 구조체의 스마트 포인터 타입
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		struct WildChargeData : boost::noncopyable
		{
		private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

			float m_fHpEachWildChargeFrame;				/// 와일드 차지에서 차지 중일 때 이전 프레임의 HP를 저장할 용도(현재 프레임의 소모된 HP를 알아내기 위해 사용)
			float m_fTimeAfterFullCharging;				/// 와일드차지가 풀차지 된 후의 경과 시간
			float m_fPowerRateWildCharging;				/// 와일드차지에서 차지된 파워레이트(데미지배율)
			RAVEN_STATE_ID m_eSlotID_WildCharge;		/// 와일드차지가 장착된 스킬슬롯을 알아내기 위한 용도( 스킬 슬롯이 스테이트중 하나로 로 구성 되어 있음 )
			bool m_bSlotB;								/// 와일드차지가 장착된 슬롯이 A인지, B인지 구분하기 위한용도
			bool* m_pChargeKey;							/// 와일드 차지의 키가 눌려있는지 알아내기 위안 용도 (메모리를 생성하는 것이 아니므로 해제하지 않는다.)
			CX2EffectSet::Handle m_hHandleWildCharingEffectSet;	/// 충전중을 표시하는 이펙트 핸들
			
			WildChargeData() : m_fHpEachWildChargeFrame( 0.f ), m_fTimeAfterFullCharging( 0.f ),
				m_fPowerRateWildCharging( 0.f ), m_eSlotID_WildCharge( RSI_BASE ), m_bSlotB( false ),
				m_pChargeKey( NULL ), m_hHandleWildCharingEffectSet( INVALID_EFFECTSET_HANDLE )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			{}

		public:

			static WildChargeDataPtr  CreateWildChargeData() { return WildChargeDataPtr( new WildChargeData ); }

			float GetHpEachWildChargeFrame() const { return m_fHpEachWildChargeFrame; }
			void SetHpEachWildChargeFrame(float fHpEachWildChargeFrame_) { m_fHpEachWildChargeFrame = fHpEachWildChargeFrame_; }

			float GetTimeAfterFullCharging() const { return m_fTimeAfterFullCharging; }
			void SetTimeAfterFullCharging(float fTimeAfterFullCharging_) { m_fTimeAfterFullCharging = fTimeAfterFullCharging_; }

			float GetPowerRateWildCharging() const { return m_fPowerRateWildCharging; }
			void SetPowerRateWildCharging(float fPowerRateWildCharging_) { m_fPowerRateWildCharging = fPowerRateWildCharging_; }

			RAVEN_STATE_ID GetSlotID_WildCharge() const { return m_eSlotID_WildCharge; }

			/** @function : SetSlotID_WildCharge
				@brief : 현재 스킬이 어떤 슬롯에 있는지를 저장하기 위한 함수(스킬은 각 슬롯이 하나의 상태로 되어 있어 상태 ID를 인자값으로 전달해 준다.)
				@param : 상태 ID
			*/
			void SetSlotID_WildCharge(RAVEN_STATE_ID eStateID_) { m_eSlotID_WildCharge = eStateID_; }

			bool GetSlotB() const { return m_bSlotB; }
			void SetSlotB(bool bSlotB_) { m_bSlotB = bSlotB_; }

			bool GetChargeKey() const { return *m_pChargeKey; }
			void SetChargeKeyPointer(bool* pChargeKey_) { m_pChargeKey = pChargeKey_; }

			CX2EffectSet::Handle GetHandleWildCharingEffectSet() const { return m_hHandleWildCharingEffectSet; }
			CX2EffectSet::Handle& GetHandleReferenceWildCharingEffectSet() { return m_hHandleWildCharingEffectSet; }
			void SetHandleWildCharingEffectSet(CX2EffectSet::Handle hHandle_) { m_hHandleWildCharingEffectSet = hHandle_; }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            void    AddRef()    {   ++m_uRefCount; }
            void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		};


	private:
		CX2GURaven( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit );
		virtual ~CX2GURaven(void);


	public:
		static CX2GURaven* CreateGURaven( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit )
		{
			return new CX2GURaven( unitIndex, teamNum, 
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
		virtual void		AttackResultPlayEffect();

		virtual void		Win();
		virtual void		Lose();
		virtual void		Draw();


		virtual void		InitEffect();
		virtual void		InitEnchantWeaponEffectUnitSpecific();
		
		void				InitializeRavenMajorParticleArray();	// 메이저 파티클 배열을 초기화 // kimhc // 2010.11.5 
		virtual void		CreateMajorParticleArray();		// 메이저 파티클 배열을 생성 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void			AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetRavenMajorParticleByEnum( RAVEN_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		ParticleEventSequenceHandle	GetHandleRavenMajorParticleByEnum( RAVEN_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const // 캐릭터만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		{
			ASSERT( RAVEN_MAJOR_PII_END > eVal_ && RAVEN_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRavenMajorParticleInstance[eVal_];
		}
		ParticleEventSequenceHandle& GetHandleReferenceRavenMajorParticleByEnum( RAVEN_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) // 캐릭터만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들의 레퍼런스 하나를 얻어옴 // kimhc // 2010.11.5 
		{
			ASSERT( RAVEN_MAJOR_PII_END > eVal_ && RAVEN_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRavenMajorParticleInstance[eVal_];
		}

		void				SetHandleRavenMajorParticleByEnum( RAVEN_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ ) // 캐릭터만 쓰는 메이저 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함 // kimhc // 2010.11.5 
		{
			ASSERT( RAVEN_MAJOR_PII_END > eVal_ && RAVEN_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahRavenMajorParticleInstance[eVal_] = hHandle_;
		}
		void				DeleteRavenMajorParticle();


		void				InitializeRavenMinorParticleArray();	// 공통적으로 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
		virtual void		CreateMinorParticleArray();		// 각 캐릭터들만 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void			AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetRavenMinorParticleByEnum( RAVEN_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		ParticleEventSequenceHandle	GetHandleRavenMinorParticleByEnum( RAVEN_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const	// 캐릭터만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴	// kimhc // 2010.11.5 
		{
			ASSERT( RAVEN_MINOR_PII_END > eVal_ && RAVEN_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRavenMinorParticleInstance[eVal_];
		}
		ParticleEventSequenceHandle& GetHandleReferenceRavenMinorParticleByEnum( RAVEN_MINOR_PARTICLE_INSTANCE_ID eVal_ ) // 캐릭터만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들의 레퍼런스 하나를 얻어옴	// kimhc // 2010.11.5 
		{
			ASSERT( RAVEN_MINOR_PII_END > eVal_ && RAVEN_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRavenMinorParticleInstance[eVal_];
		}
		void				SetHandleRavenMinorParticleByEnum( RAVEN_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )	// 캐릭터만 쓰는 마이너 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함	// kimhc // 2010.11.5 
		{
			ASSERT( RAVEN_MINOR_PII_END > eVal_ && RAVEN_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahRavenMinorParticleInstance[eVal_] = hHandle_;
		}
		void				DeleteRavenMinorParticle();

		virtual	void		DeleteMinorParticle();

		virtual bool		SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet );
		virtual float		GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

#ifdef UPGRADE_RAVEN
		virtual void		Init( bool bUseTeamPos_, int iStartPosIndex_ );	
		void				DoHyper(bool bFullHyper);
		int					GetNasodBall() { return m_iNaosdBallLv; }
		int					GetNasodLvByGuage() { return (int)(m_fRageGauge / 20.f); }
		bool				GetParrying() { return m_bParrying; }
		bool				CanParrying() { return m_bCanParrying; }		
		float				GetParryingTime() { return m_fParryingTime; }
		bool				CanParryingState();
		void				AddRageGuage(float fVal);
		float				GetRageGuage() { return m_fRageGauge; }	
		bool				IsAttackNasodBall() 
		{
			if( m_bDelayAttackNasodBall == false && m_bAttackNasodBall == false )
			{
				return false;
			}

			return true;
		}
#endif

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		bool GetIsShadowStepIcon() { return m_bIsShadowStepIcon; }		/// 섀도우 스텝 아이콘 적용 여부		
		bool GetActiveShadowStep() { return m_bActiveShadowStepCombo; }	/// 섀도우 스텝 계열 동작중 여부
		float GetEDTBleedingTime() { return m_fEDTBleedingTime; }		/// 치명상 유지 시간 반환
#endif SERV_RAVEN_VETERAN_COMMANDER

		virtual bool		IsFullHyperMode() const { return m_bHyperAura; }
		virtual void		SetFullHyperMode( const bool bFullHyperMode_ ) { m_bHyperAura = bFullHyperMode_; }

		virtual void ApplyHyperModeBuff();
		virtual void HyperModeBuffEffectStart();
		virtual void HyperModeBuffEffectEnd();
		virtual void AddUnitSlashData( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ );

	protected:
		virtual void ParseCommonRandomState();
		virtual void		InitState();
		void InitStateID();
		void InitStateCommon();
		void InitStateSwordTaker();
		void InitStateOverTaker();
#ifdef RAVEN_WEAPON_TAKER
		void InitStateWeaponTaker();
#endif RAVEN_WEAPON_TAKER

		virtual void		InitComponent();	

		virtual void		InitPassiveSkillState();
		virtual void		SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ );
		virtual void		SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_);




		virtual void		CommonStateStartFuture();
		virtual void		CommonStateStart();
		virtual void		CommonFrameMoveFuture();
		virtual void		CommonFrameMove();
		virtual void		CommonEventProcess();
		virtual void		CommonStateEndFuture();
		virtual void		CommonStateEnd();
//{{ robobeg : 2008-10-28
		//virtual void		CommonRender();
        virtual RENDER_HINT CommonRender_Prepare();
//}} robobeg : 2008-10-28

		virtual bool		SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );	
		virtual void		NoStateChangeActionFrameMove();

		virtual void		ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );
		virtual void		ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ );

		void				ClearArcEnemyData();

#ifndef NEW_SKILL_TREE
		void				CommonBuffAddStat( CX2SkillTree::SKILL_ID eSkillID, float fTimeScale );		// CX2GUUser::BuffProcess( eSkillID, false ); 로 통합
#endif

//{{ kimhc // 2010.11.3 // 레피 - 승화된 분노
#ifdef	NEW_SKILL_2010_11
		virtual float		GetHyperModeTime() const;
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.3 // 레피 - 승화된 분노

#ifdef UPGRADE_RAVEN
		void				ClearNasodBall();
		void				CreateNasodBall();
		void				DoParrying();
		void				RestoreNasodBall();
		virtual void		AttackResultByType( CX2DamageManager::DamageData &pDamageData );
		void				ChangePowerRateNasodBall();
		void				AttackNasodBall( );
		void				DoFrameMoveNasodBall();				
#endif // UPGRADE_RAVEN

		void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );

		//레이븐 용병의 처신술 패시브(SI_P_RWT_BEHAVIOR_OF_MERCENARY) 효과를 적용시키기 위해 추가한 함수
		virtual float GetReduceDebuffDurationRate(BUFF_TEMPLET_ID eBuffTempletID_);

		virtual int GetComboZStateID() const { return RSI_COMBO_Z; }
		virtual int GetComboXStateID() const { return RSI_COMBO_X; }
	protected:
		bool								m_bReAttackZ1Right;
		float								m_fReAttackZ1Time;
		float								m_fReDashJumpXTime;
		bool								m_bZZZX;

#ifdef OLD_HAND_SLASH_TRACE
		//{{AFX
		CKTDGSlashTrace*					m_pHandSlashTrace;
		CKTDGSlashTrace*					m_pHandSlashTraceTip;
		//}}AFX
#endif OLD_HAND_SLASH_TRACE

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCannonBlade;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMagnumBlaster1;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMesh_MagnumBlaster2;


		//Major
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPowerAssault[3];
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSevenBurst[3];

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqBloodyAccel;		

		//Minor
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSonicStab;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSonicStab1;


		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshFlyingImpact;

		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshBerserkerBlade;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshBerserkerBlade1;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshBerserkerBlade2;
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshBerserkerBlade3;


		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshGuardianStrike1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqGuardianStrike1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqGuardianStrike2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqGuardianStrike3;

		int												m_iSuccessiveHit_OneZ;
		std::vector< ArcEnemyData* >					m_vecArcEnemyData;
		CKTDXTimer										m_TimerFlyingImpact;
#ifdef BALANCE_PATCH_20110303
		float											m_fArcEnemyEachTime;
#endif
		




		float			m_fShadowStepTimeLeft;
		bool			m_bEnableShadowStepThrust;
		float			m_fAttackPowerRateShadowStepThrust;
		float			m_fMPConsumeShadowStepThrust;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		bool			m_bEnableShadowStepThrustEnhance;			// 블마용 쉐도우 쓰러스트 사용 가능 여부
		float			m_fAttackPowerRateShadowStepThrustEnhance;	// 블마용 쉐도우 쓰러스트 공격력

		float			m_fChanceOfExplosiveNasodHand;				// 레피용 폭발하는 나소드 핸드 확률		
		bool			m_bEnableShadowStepPunisher;				// 레피용 쉐도우 퍼니셔 사용 가능 여부
		float			m_fAttackPowerRateShadowStepPunisher;		// 레피용 쉐도우 퍼니셔 공격력

		float			m_fOverheatDamageLimitedRate;				// 베커용 오버히트 데미지 제한 퍼센테이지 ( 용병의 승리전략 ) 
		float			m_fAddOverHeatBuffDurationTime;				// 베커용 오버히트 지속시간 증가 ( 과열모드 작동! )
		int				m_iAddOverHeatLevel;						// 베커용 오버히트 레벨 설정 ( 과열모드 작동!) 
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈


#ifdef RAVEN_SECOND_CLASS_CHANGE
		int				m_iBlackHoleHitCount;
		float			m_fBlackHoleTime;
		CX2EffectSet::Handle m_hBlackHoleEffectSet;
		
		CX2DamageManager::DAMAGE_TYPE	m_eDamageTypeRaven;
		float							m_fChangeDamageRelRaven;
		float							m_fPassiveCriticalRateRaven;

		D3DXVECTOR3	m_vXCrashPos;
#endif
#ifdef RAVEN_WEAPON_TAKER
		int				m_iRWTMaxComboLoopCount;	/// 키를 누를 때마다 증가하고 이 횟수가 오버히트 가능 횟수보다 커지면 오버히트 발동 // kimhc // 2013-07-04
		int				m_iRWTComboLoopCount;		/// loop 가능 스테이트가 몇번이나 loop 됬는지를 나타냄 (오버히트 발동 시 이 횟수와 m_iRWTMaxComboLoopCount를 비교한다.) // kimhc // 2013-07-04
		int				m_iOverheatKeyPressTime;
		bool			m_bOverheated;
		bool			m_bShowedOverheat;

		float			m_fAirSlicerDamageRel;
		float			m_fOverheatDamageRate;
		float			m_fReduceEDTDurationRate;
		float			m_fOverheatBuffTime;

		// 버스팅블레이드 타격메쉬
		CKTDGXMeshPlayer::CXMeshInstanceHandle
						m_hBurstingBladeAttackBox;

		// 기가 프로미넌스 위치 선정을 위한 컨테이너
		std::vector< char >
						m_vecLeftSpot;
		std::vector< char >
						m_vecRightSpot;
#endif RAVEN_WEAPON_TAKER

#ifdef BALANCE_PATCH_20120329
		CX2EffectSet::Handle	m_hEffectDashJumpComboX;
#endif

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		int													m_iRVCComboLoopCount;					/// 연발 콤보 반복 횟수
		int													m_iRVCComboLoopInputCount;				/// 연발 콤보 반복 입력 횟수

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle   							m_hEffectFrameThrow;					/// 대시 zzzz용 화염 방사 이펙트
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*							m_pEffectFrameThrow;					/// 대시 zzzz용 화염 방사 이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		bool												m_bEndFrameThrow;						/// 화염 방사 동작 정지 여부 조사

		bool												m_bEnableShadowBackSlide;				/// 섀도우 백 슬라이드 패시브 적용 여부
		float												m_fShadowStepCooTime;					/// 섀도우 스텝 쿨타임
		bool												m_bActiveShadowStepCombo;				/// 섀도우 스텝 계열 동작중 여부
		bool												m_bIsShadowStepIcon;					/// 섀도우 스텝 아이콘 적용중 여부
		int													m_iShadowBackSlideLevel;				/// 섀도우 백 슬라이드 스킬 레벨
		float												m_fShadowBusterPowerRate;				/// 섀도우 버스터 파워레이트
		float												m_fShadowBackSlideTimeLeft;				/// 섀도우 백 슬라이드 유지 시간
		float												m_fShadowStepFinishTimeLeft;			/// 마지막 섀도우 스텝 유지 시간

		bool												m_bEnableSurvivalTechniqueOfMercenary;	/// 용병의 생존술 패시브 적용 여부
		bool												m_bActiveSurvivalTechniqueOfMercenary;	/// 용병의 생존술 발생 여부
		
		float												m_fLimitOverHeatHP;						/// 오버 히트 가능한 HP 제한치

		bool												m_bShowedOverheat1;						/// 단계적 오버히트용 이모티콘 생성 확인 객체
		bool												m_bShowedOverheat2;						/// 단계적 오버히트용 이모티콘 생성 확인 객체
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqOverHeatIcon;						/// 단계적 오버히트용 이모티콘 핸들
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqOverHeatOverlapIcon;				/// 단계적 오버히트 중첩 이모티콘 핸들

		float												m_fEDTBleedingTime;						/// 치명상 유지 시간


		bool												m_bIsHyperDeadlyRaid;					/// 현재 데들리 레이드 각성 여부

		float												m_fDamageDataChangeTime;
		float												m_fDamageDataChangeTime1;
		float												m_fDamageDataChangeTime2;

		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hOverHeatObject;						/// 오버 히트 발동시 생성되는 구조물

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffectHandle							    m_hEffectOverHeatFire1;					/// 오버 히트 발동시 생성되는 화염
		CX2DamageEffect::CEffectHandle							    m_hEffectOverHeatFire2;					/// 오버 히트 발동시 생성되는 화염
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*							m_pEffectOverHeatFire1;					/// 오버 히트 발동시 생성되는 화염
		CX2DamageEffect::CEffect*							m_pEffectOverHeatFire2;					/// 오버 히트 발동시 생성되는 화염
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef FINALITY_SKILL_SYSTEM //김창한
		float												m_fExtremBladeDelay;					/// 익스트림 블레이드 히트 간격
#endif //FINALITY_SKILL_SYSTEM

	private:

		CKTDGParticleSystem::CParticleEventSequenceHandle		m_ahRavenMajorParticleInstance[RAVEN_MAJOR_PII_END];
		CKTDGParticleSystem::CParticleEventSequenceHandle		m_ahRavenMinorParticleInstance[RAVEN_MINOR_PII_END];

	protected:
		//RSI_DIE_FRONT
		void RSI_DIE_FRONT_StartFuture();
		void RSI_DIE_FRONT_Start();
		void RSI_DIE_FRONT_FrameMove();
		void RSI_DIE_FRONT_EventProcess();

		//RSI_DIE_BACK
		void RSI_DIE_BACK_StartFuture();
		void RSI_DIE_BACK_Start();
		void RSI_DIE_BACK_FrameMove();
		void RSI_DIE_BACK_EventProcess();

		//RSI_WAIT
		void RSI_WAIT_StartFuture();
		void RSI_WAIT_Start();
		void RSI_WAIT_EventProcess();

		//RSI_WALK
#ifdef BALANCE_BLADE_MASTER_20130117	/// 캔슬시 마법체 소멸시킬 수 있도록 종료 함수 추가
		void RSI_WALK_Start();
#endif BALANCE_BLADE_MASTER_20130117
		void RSI_WALK_EventProcess();

		//RSI_JUMP_READY
		void RSI_JUMP_READY_EventProcess();

		//RSI_JUMP_UP
		void RSI_JUMP_UP_EventProcess();

		//RSI_JUMP_DOWN
		void RSI_JUMP_DOWN_EventProcess();

		//RSI_JUMP_LANDING
		void RSI_JUMP_LANDING_EventProcess();

		//RSI_DASH
		void RSI_DASH_EventProcess();

		//RSI_DASH_END
		void RSI_DASH_END_StartFuture();
		void RSI_DASH_END_Start();
		void RSI_DASH_END_FrameMove();
		void RSI_DASH_END_EventProcess();

		//RSI_DASH_JUMP
		void RSI_DASH_JUMP_StartFuture();
		void RSI_DASH_JUMP_Start();
		void RSI_DASH_JUMP_FrameMoveFuture();
		void RSI_DASH_JUMP_EventProcess();
		void RSI_DASH_JUMP_EndFuture();

		//RSI_DASH_JUMP_LANDING
		void RSI_DASH_JUMP_LANDING_EventProcess();

		//RSI_DASH_JUMP_POWER_LANDING
		void RSI_DASH_JUMP_POWER_LANDING_StartFuture();
		void RSI_DASH_JUMP_POWER_LANDING_Start();
		void RSI_DASH_JUMP_POWER_LANDING_EventProcess();

		//RSI_HYPER_MODE
		void HyperModeFrameMove();
#ifdef MODIFY_RIDING_PET_AWAKE
		void RidingHyperModeFrameMove();
		void CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ = false );
#endif // MODIFY_RIDING_PET_AWAKE

		//RSI_DAMAGE_GROGGY
		//void RSI_DAMAGE_GROGGY_FrameMoveFuture();
		//void RSI_DAMAGE_GROGGY_EventProcess();

		//RSI_DAMAGE_SMALL_FRONT
		void RSI_DAMAGE_SMALL_FRONT_EventProcess();

		//RSI_DAMAGE_SMALL_BACK
		//{{ JHKang / 강정훈 / 2010/11/02 / 긴급탈출 패시브
#ifdef NEW_SKILL_2010_11
		void RSI_DAMAGE_SMALL_BACK_Start();
#endif NEW_SKILL_2010_11
		//}} JHKang / 강정훈 / 2010/11/02 / 긴급탈출 패시브

		//RSI_DAMAGE_SMALL_BACK
		void RSI_DAMAGE_SMALL_BACK_EventProcess();

		//RSI_DAMAGE_BIG_FRONT
		void RSI_DAMAGE_BIG_FRONT_Start();
		void RSI_DAMAGE_BIG_FRONT_EventProcess();

		//RSI_DAMAGE_BIG_BACK
		void RSI_DAMAGE_BIG_BACK_Start();
		void RSI_DAMAGE_BIG_BACK_EventProcess();

		//RSI_DAMAGE_DOWN_FRONT
		void RSI_DAMAGE_DOWN_FRONT_FrameMove();
		void RSI_DAMAGE_DOWN_FRONT_EventProcess();

		//RSI_DAMAGE_DOWN_BACK
		void RSI_DAMAGE_DOWN_BACK_FrameMove();
		void RSI_DAMAGE_DOWN_BACK_EventProcess();

		//RSI_DAMAGE_AIR_DOWN_LANDING
		void RSI_DAMAGE_AIR_DOWN_LANDING_EventProcess();

		//RSI_STANDUP_ROLLING_FRONT_FRONT
		void RSI_STANDUP_ROLLING_FRONT_FRONT_EventProcess();
		void RSI_STANDUP_ROLLING_FRONT_FRONT_End();

		//RSI_STANDUP_ROLLING_FRONT_BACK
		void RSI_STANDUP_ROLLING_FRONT_BACK_EventProcess();
		void RSI_STANDUP_ROLLING_FRONT_BACK_End();

		//RSI_STANDUP_ROLLING_BACK_FRONT
		void RSI_STANDUP_ROLLING_BACK_FRONT_EventProcess();
		void RSI_STANDUP_ROLLING_BACK_FRONT_End();

		//RSI_STANDUP_ROLLING_BACK_BACK
		void RSI_STANDUP_ROLLING_BACK_BACK_EventProcess();
		void RSI_STANDUP_ROLLING_BACK_BACK_End();

		//RSI_STANDUP_ATTACK_FRONT
		void RSI_STANDUP_ATTACK_FRONT_FrameMove();
		void RSI_STANDUP_ATTACK_FRONT_EventProcess();
		void RSI_STANDUP_ATTACK_FRONT_End();

		//RSI_STANDUP_ATTACK_BACK
		void RSI_STANDUP_ATTACK_BACK_FrameMove();
		void RSI_STANDUP_ATTACK_BACK_EventProcess();
		void RSI_STANDUP_ATTACK_BACK_End();

		//RSI_SUPER_DASH_READY
#ifdef SERV_SKILL_NOTE
		void RSI_SUPER_DASH_READY_StateStart();
#endif
		void RSI_SUPER_DASH_READY_EventProcess();

		//RSI_SUPER_DASH
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		// 버닝 러쉬
		void RSI_SUPER_DASH_Init();		
		void RSI_SUPER_DASH_StateStart();
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		void RSI_SUPER_DASH_FrameMoveFuture();
		void RSI_SUPER_DASH_EventProcess();

		//RSI_SUPER_DASH_END
		void RSI_SUPER_DASH_END_FrameMove();
		void RSI_SUPER_DASH_END_EventProcess();

		//RSI_COMBO_Z
		void RSI_COMBO_Z_EventProcess();

		//RSI_COMBO_ZZ
		void RSI_COMBO_ZZ_EventProcess();

		//RSI_COMBO_ZZZ
		void RSI_COMBO_ZZZ_EventProcess();

		//RSI_COMBO_ZZZZ
		void RSI_COMBO_ZZZZ_FrameMoveFuture();
		void RSI_COMBO_ZZZZ_EventProcess();

		//RSI_COMBO_ZZZX
		void RSI_COMBO_ZZZX_Start();
		void RSI_COMBO_ZZZX_FrameMove();
		void RSI_COMBO_ZZZX_EventProcess();

		//RSI_COMBO_ZZX
		void RSI_COMBO_ZZX_EventProcess();

		//RSI_COMBO_ZZXX
		void RSI_COMBO_ZZXX_FrameMoveFuture();
		void RSI_COMBO_ZZXX_EventProcess();

		//RSI_COMBO_X
		void RSI_COMBO_X_EventProcess();

		//RSI_COMBO_XX
		void RSI_COMBO_XX_EventProcess();

		//RSI_COMBO_XXX
		void RSI_COMBO_XXX_FrameMove();
		void RSI_COMBO_XXX_EventProcess();

		//RSI_DASH_COMBO_Z
		void RSI_DASH_COMBO_Z_EventProcess();

		//RSI_DASH_COMBO_ZZ
		void RSI_DASH_COMBO_ZZ_EventProcess();

		//RSI_DASH_COMBO_X
		void RSI_DASH_COMBO_X_FrameMove();
		void RSI_DASH_COMBO_X_EventProcess();

		//RSI_DASH_JUMP_COMBO_Z
		void RSI_DASH_JUMP_COMBO_Z_EventProcess();

		//RSI_DASH_JUMP_COMBO_ZZ
		void RSI_DASH_JUMP_COMBO_ZZ_StateStart();
		void RSI_DASH_JUMP_COMBO_ZZ_FrameMoveFuture();
		void RSI_DASH_JUMP_COMBO_ZZ_EventProcess();

		//RSI_DASH_JUMP_COMBO_ZZ_Landing
		void RSI_DASH_JUMP_COMBO_ZZ_Landing_StartFuture();
		void RSI_DASH_JUMP_COMBO_ZZ_Landing_EventProcess();

		//RSI_DASH_JUMP_COMBO_X
		void RSI_DASH_JUMP_COMBO_X_Init();
		void RSI_DASH_JUMP_COMBO_X_Start();
		void RSI_DASH_JUMP_COMBO_X_FrameMoveFuture();
		void RSI_DASH_JUMP_COMBO_X_FrameMove();
		void RSI_DASH_JUMP_COMBO_X_EventProcess();

		//RSI_JUMP_COMBO_Z
		void RSI_JUMP_COMBO_Z_FrameMoveFuture();
		void RSI_JUMP_COMBO_Z_EventProcess();

		//RSI_JUMP_COMBO_X
		void RSI_JUMP_COMBO_X_StateStart();
		void RSI_JUMP_COMBO_X_FrameMoveFuture();		
		void RSI_JUMP_COMBO_X_EventProcess();




		
		
		//RSI_SI1_MAXIMUM_CANNON
		void RSI_SI1_MAXIMUM_CANNON_Init();
		void RSI_SI1_MAXIMUM_CANNON_FrameMove();
		void RSI_SI1_MAXIMUM_CANNON_EventProcess();
		//RSI_SI1_MAXIMUM_CANNON_HYPER
		void RSI_SI1_MAXIMUM_CANNON_HYPER_FrameMove();
		void RSI_SI1_MAXIMUM_CANNON_HYPER_EventProcess();
	
		
		//RSI_SI2_POWER_ASSAULT
		void RSI_SI2_POWER_ASSAULT_Init();
		void RSI_SI2_POWER_ASSAULT_FrameMoveFuture();
		void RSI_SI2_POWER_ASSAULT_FrameMove();
		void RSI_SI2_POWER_ASSAULT_EventProcess();
		//RSI_SI2_POWER_ASSAULT_HYPER	
		void RSI_SI2_POWER_ASSAULT_HYPER_FrameMoveFuture();
		void RSI_SI2_POWER_ASSAULT_HYPER_FrameMove();
		void RSI_SI2_POWER_ASSAULT_HYPER_EventProcess();


		//RSI_SI1_EARTH_BREAKER
		void RSI_SI1_EARTH_BREAKER_Init();
		void RSI_SI1_EARTH_BREAKER_FrameMove();
		void RSI_SI1_EARTH_BREAKER_EventProcess();
		//RSI_SI1_EARTH_BREAKER_HYPER
		void RSI_SI1_EARTH_BREAKER_HYPER_FrameMove();
		void RSI_SI1_EARTH_BREAKER_HYPER_EventProcess();


		//RSI_SI2_CANNON_BLADE
		void RSI_SI2_CANNON_BLADE_Init();
		void RSI_SI2_CANNON_BLADE_FrameMove();
		void RSI_SI2_CANNON_BLADE_EventProcess();
		//RSI_SI2_CANNON_BLADE_HYPER
		void RSI_SI2_CANNON_BLADE_HYPER_FrameMove();
		void RSI_SI2_CANNON_BLADE_HYPER_EventProcess();


#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		//RSI_SI3_MAGNUM_BLASTER
		void RSI_SI3_MAGNUM_BLASTER_Init();
		void RSI_SI3_MAGNUM_BLASTER_FrameMoveFuture();
		void RSI_SI3_MAGNUM_BLASTER_FrameMove();
		void RSI_SI3_MAGNUM_BLASTER_EventProcess();
		void RSI_SI3_MAGNUM_BLASTER_End();
		//RSI_SI3_MAGNUM_BLASTER_HYPER
		void RSI_SI3_MAGNUM_BLASTER_HYPER_FrameMoveFuture();
		void RSI_SI3_MAGNUM_BLASTER_HYPER_FrameMove();
		void RSI_SI3_MAGNUM_BLASTER_HYPER_EventProcess();
		void RSI_SI3_MAGNUM_BLASTER_HYPER_End();
#endif //UPGRADE_SKILL_SYSTEM_2013

		//RSI_SI3_SEVEN_BURST
		void RSI_SI3_SEVEN_BURST_Init();
		void RSI_SI3_SEVEN_BURST_FrameMoveFuture();
		void RSI_SI3_SEVEN_BURST_FrameMove();
		void RSI_SI3_SEVEN_BURST_EventProcess();
		//RSI_SI3_SEVEN_BURST_HYPER
		void RSI_SI3_SEVEN_BURST_HYPER_FrameMoveFuture();
		void RSI_SI3_SEVEN_BURST_HYPER_FrameMove();
		void RSI_SI3_SEVEN_BURST_HYPER_EventProcess();
	
		//COMMON_BUFF
		virtual void COMMON_BUFF_FrameMove();

		void RSI_SOUL_TAKER_DASH_COMBO_ZZ_EventProcess();

		void RSI_SOUL_TAKER_COMBO_ZZZupZ_FrameMoveFuture();
		void RSI_SOUL_TAKER_COMBO_ZZZupZ_EventProcess();
	
		void RSI_SOUL_TAKER_COMBO_XXZ_EventProcess();

#ifdef BALANCE_BLADE_MASTER_20130117	/// 슈퍼아머 색상 변경 때문에 추가
		void RSI_SOUL_TAKER_COMBO_XXZZ8_StateStart();
#endif BALANCE_BLADE_MASTER_20130117
		void RSI_SOUL_TAKER_COMBO_XXZZ8_FrameMove();
		void RSI_SOUL_TAKER_COMBO_XXZZ8_EventProcess();
		void RSI_SOUL_TAKER_COMBO_XXZZ8Z_EventProcess();

		void RSI_SOUL_TAKER_DASH_COMBO_ZZZ_EventProcess();



		void RSI_OVER_TAKER_COMBO_XXdownX_FrameMoveFuture();
		void RSI_OVER_TAKER_COMBO_XXdownX_EventProcess();

		void RSI_OVER_TAKER_COMBO_XXdownXdownX_FrameMoveFuture();
		void RSI_OVER_TAKER_COMBO_XXdownXdownX_EventProcess();

		void RSI_OVER_TAKER_COMBO_XXdownXdownXdownX_FrameMoveFuture();
		void RSI_OVER_TAKER_COMBO_XXdownXdownXdownX_EventProcess();
		

		void RSI_OVER_TAKER_COMBO_XXfrontX_Init();
		void RSI_OVER_TAKER_COMBO_XXfrontX_FrameMove();
		void RSI_OVER_TAKER_COMBO_XXfrontX_EventProcess();


		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_FrameMoveFuture();
		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_FrameMove();
		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_EventProcess();

		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_StartFuture();
		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_Start();
		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_EventProcess();

		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX_FrameMove();
		void RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX_EventProcess();

#ifdef RAVEN_SECOND_CLASS_CHANGE
		void RRF_DASH_JUMP_COMBO_X_Init();
		void RRF_DASH_JUMP_COMBO_X_Start();
		void RRF_DASH_JUMP_COMBO_X_FrameMoveFuture();
		void RRF_DASH_JUMP_COMBO_X_FrameMove();
		void RRF_DASH_JUMP_COMBO_X_EventProcess();

		void RRF_DASH_JUMP_COMBO_XX_Init();
		void RRF_DASH_JUMP_COMBO_XX_FrameMoveFuture();
		void RRF_DASH_JUMP_COMBO_XX_FrameMove();
		void RRF_DASH_JUMP_COMBO_XX_EventProcess();

		void RRF_DASH_JUMP_COMBO_XX_LANDING_Start();
		void RRF_DASH_JUMP_COMBO_XX_LANDING_FrameMove();
		void RRF_DASH_JUMP_COMBO_XX_LANDING_EventProcess();	

		void RRF_COMBO_ZZXFRONTX_Start();
		void RRF_COMBO_ZZXFRONTX_FrameMoveFuture();
		void RRF_COMBO_ZZXFRONTX_FrameMove();
		void RRF_COMBO_ZZXFRONTX_EventProcess();
		void RRF_COMBO_ZZXFRONTX_End();

		void RRF_COMBO_ZZXFRONTXX_FrameMove();
		void RRF_COMBO_ZZXFRONTXX_EventProcess();

		void RBM_Combo_XZ_EventProcess();
		void RBM_Combo_XZZ_EventProcess();
		void RBM_Combo_XZZZ_FrameMoveFuture();
		void RBM_Combo_XZZZ_EventProcess();
		
		void RBM_COMBO_ZZZX_Init();
#ifdef BALANCE_BLADE_MASTER_20130117	/// 캔슬시 마법체 소멸시킬 수 있도록 종료 함수 추가
		void RBM_COMBO_ZZZX_StateStart();
#endif BALANCE_BLADE_MASTER_20130117
		void RBM_COMBO_ZZZX_FrameMove();
		void RBM_COMBO_ZZZX_EventProcess();
		void RBM_COMBO_ZZZXZ_Start();
		void RBM_COMBO_ZZZXZ_EventProcess();		
#ifdef BALANCE_BLADE_MASTER_20130117	/// 캔슬시 마법체 소멸시킬 수 있도록 종료 함수 추가
		void RBM_COMBO_ZZZX_EXPLOSION_StateStart();
#endif BALANCE_BLADE_MASTER_20130117

		void RBM_COMBO_ZZZX_EXPLOSION_FrameMove();
		void RBM_COMBO_ZZZX_EXPLOSION_EventProcess();

		void SI_SA_RRF_X_CRASH_Init();
		void SI_SA_RRF_X_CRASH_FrameMove();
		void SI_SA_RRF_X_CRASH_EventProcess();

		void SI_A_RBM_ONE_FLASH_Init();
		void SI_A_RBM_ONE_FLASH_StateStart();
		void SI_A_RBM_ONE_FLASH_EventProcess();

		void SI_SA_RBM_WOLF_FANG_START_Init();
		void SI_SA_RBM_WOLF_FANG_START_StateStart();
		void SI_SA_RBM_WOLF_FANG_START_FrameMove();
		void SI_SA_RBM_WOLF_FANG_START_EventProcess();		
		void SI_SA_RBM_WOLF_FANG_STARTLANDING_EventProcess();
		void SI_SA_RBM_WOLF_FANG_HIT_EventProcess();
		void SI_SA_RBM_WOLF_FANG_HITLANDING_EventProcess();
#endif

		// 필살기
		void RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_Init();
		void RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_FrameMove();
		void RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_EventProcess();
		void RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_HYPER_FrameMove();
		void RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_HYPER_EventProcess();


		void RSI_SI2_SOUL_TAKER_FLYING_IMPACT_Init();
		void RSI_SI2_SOUL_TAKER_FLYING_IMPACT_FrameMove();
		void RSI_SI2_SOUL_TAKER_FLYING_IMPACT_EventProcess();
		void RSI_SI2_SOUL_TAKER_FLYING_IMPACT_End();
		void RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_FrameMove();
		void RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_EventProcess();
		void RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_End();


	
		void RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_FrameMoveFuture();
		void RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_FrameMove();
		void RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_EventProcess();

		void RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_FrameMoveFuture();
		void RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_FrameMove();
		void RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_EventProcess();



		// 필살기
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_Init();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateStartFuture();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateStart();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_FrameMove();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_EventProcess();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateEnd();

		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateStartFuture();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateStart();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_FrameMove();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_EventProcess();
		void RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateEnd();

		

		void RSI_SI2_OVER_TAKER_ARC_ENEMY_Init();
		void RSI_SI2_OVER_TAKER_ARC_ENEMY_FrameMove();
		void RSI_SI2_OVER_TAKER_ARC_ENEMY_EventProcess();
		void RSI_SI2_OVER_TAKER_ARC_ENEMY_HYPER_FrameMove();
		void RSI_SI2_OVER_TAKER_ARC_ENEMY_HYPER_EventProcess();
#ifdef BALANCE_BLADE_MASTER_20130117	/// 플라잉 임팩트 전진 기능 추가
#endif BALANCE_BLADE_MASTER_20130117


		void RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_Init();
		void RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_StateStartFuture();
		void RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_FrameMove();
		void RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_EventProcess();

		void RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_StateStartFuture();
		void RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_FrameMove();
		void RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_EventProcess();

		void RSI_SI_A_RF_SHADOW_STEP_Init();
		void RSI_SI_A_RF_SHADOW_STEP_StartFuture();
		void RSI_SI_A_RF_SHADOW_STEP_Start();
		void RSI_SI_A_RF_SHADOW_STEP_FrameMove();
		void RSI_SI_A_RF_SHADOW_STEP_EventProcess();
		void RSI_SI_A_RF_SHADOW_STEP_End();



		void RSI_SHADOW_STEP_THRUST_Init();
		void RSI_SHADOW_STEP_THRUST_Start();
		void RSI_SHADOW_STEP_THRUST_EventProcess();



		void RSI_STANDUP_ATTACK_FRONT_NO_DOWN_FrameMove();
		void RSI_STANDUP_ATTACK_FRONT_NO_DOWN_EventProcess();
		void RSI_STANDUP_ATTACK_FRONT_NO_DOWN_End();


		void RSI_STANDUP_ATTACK_BACK_NO_DOWN_FrameMove();
		void RSI_STANDUP_ATTACK_BACK_NO_DOWN_EventProcess();
		void RSI_STANDUP_ATTACK_BACK_NO_DOWN_End();





#ifdef SKILL_30_TEST

		void RSI_SA_RST_SHOCKWAVE_Init();
		void RSI_SA_RST_SHOCKWAVE_StateStart();
		void RSI_SA_RST_SHOCKWAVE_FrameMove();				
		void RSI_SA_RST_SHOCKWAVE_EventProcess();				
		void RSI_SA_RST_SHOCKWAVE_StateEnd();

		void RSI_SA_RST_SHOCKWAVE_HYPER_Init();
		void RSI_SA_RST_SHOCKWAVE_HYPER_StateStart();
		void RSI_SA_RST_SHOCKWAVE_HYPER_FrameMove();		
		void RSI_SA_RST_SHOCKWAVE_HYPER_EventProcess();	
		void RSI_SA_RST_SHOCKWAVE_HYPER_StateEnd();

		void RSI_SA_ROT_VALKYRIESJAVELIN_Init();
		void RSI_SA_ROT_VALKYRIESJAVELIN_StateStart();
		void RSI_SA_ROT_VALKYRIESJAVELIN_FrameMove();				
		void RSI_SA_ROT_VALKYRIESJAVELIN_EventProcess();		
		void RSI_SA_ROT_VALKYRIESJAVELIN_StateEnd();

		void RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_Init();
		void RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_StateStart();
		void RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_FrameMove();		
		void RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_EventProcess();	
		void RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_StateEnd();
	


		void RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_FrameMove();
		void RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_EventProcess();

		void RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_HYPER_FrameMove();
		void RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_HYPER_EventProcess();





		void RSI_A_RST_CUT_TENDON_Init();
		void RSI_A_RST_CUT_TENDON_StateStart();
		void RSI_A_RST_CUT_TENDON_EventProcess();
		
		void RSI_A_ROT_WEAPON_BREAK_Init();
		void RSI_A_ROT_WEAPON_BREAK_StateStart();
		void RSI_A_ROT_WEAPON_BREAK_EventProcess();
		
		void RSI_A_ROT_WEAPON_BREAK_HYPER_EventProcess();
		
#endif SKILL_30_TEST

#ifdef SKILL_CASH_10_TEST
		void RSI_A_RST_BLOODY_ACCEL_Init();
		void RSI_A_RST_BLOODY_ACCEL_FrameMove();
		void RSI_A_RST_BLOODY_ACCEL_EventProcess();
		
		void RSI_A_ROT_NUCLEAR_Init();
		void RSI_A_ROT_NUCLEAR_FrameMove();
		void RSI_A_ROT_NUCLEAR_EventProcess();
		void RSI_A_ROT_NUCLEAR_Hyper_FrameMove();
		void RSI_A_ROT_NUCLEAR_Hyper_EventProcess();
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
		void SI_A_RRF_GROUND_IMPACT_Init();
		void SI_A_RRF_GROUND_IMPACT_FrameMove();
		void SI_A_RRF_GROUND_IMPACT_EventProcess();
#endif

		//{{ JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가
#ifdef NEW_SKILL_2010_11
		/** @function RBM_EMERGENCY_ESCAPE
			@brief 패시브 실행
		*/
		void RBM_EMERGENCY_ESCAPE();

		/** @function RBM_EMERGENCY_ESCAPE_StartFuture
			@brief 섀도우 스텝과는 별개로 실행
		*/
		void RBM_EMERGENCY_ESCAPE_StartFuture();
#endif NEW_SKILL_2010_11
		//}} JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가

//{{ kimhc // 2010.11.10 // 블마-기가드라이브, 레피-와일드차지
#ifdef	NEW_SKILL_2010_11
		
		// 블마-기가드라이브
		void RSI_SA_RBM_GIGA_DRIVE_Init();
		void RSI_SA_RBM_GIGA_DRIVE_FrameMove();
		void RSI_SA_RBM_GIGA_DRIVE_EventProcess();

		// 레피-와일드차지 시작 스테이트
		void RSI_SA_RRF_WILD_CHARGE_READY_Init();
		void RSI_SA_RRF_WILD_CHARGE_READY_StateStart();
		void RSI_SA_RRF_WILD_CHARGE_READY_StateStartFuture();
		void RSI_SA_RRF_WILD_CHARGE_READY_FrameMove();
		void RSI_SA_RRF_WILD_CHARGE_READY_EventProcess();

		// 레피-와일드차지 기모으는 스테이트
		void RSI_SA_RRF_WILD_CHARGING_StateStart();
		void RSI_SA_RRF_WILD_CHARGING_FrameMove();
		void RSI_SA_RRF_WILD_CHARGING_EventProcess();
		void RSI_SA_RRF_WILD_CHARGING_StateEnd();

		// 레피-와일드차지 앞으로 돌진하는 스테이트
		void RSI_SA_RRF_WILD_CHARGE_DASH_FrameMove();
		void RSI_SA_RRF_WILD_CHARGE_DASH_EventProcess();

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.10 // 블마-기가드라이브, 레피-와일드차지

#ifdef UPGRADE_RAVEN
		void RSI_PARRYING_StartFuture();
		void RSI_PARRYING_Start();
		void RSI_PARRYING_EventProcess();
		void RSI_PARRYING_StateEnd();
#endif

#ifdef RAVEN_WEAPON_TAKER
		void RSI_RWT_LOSE2_FrameMove();

		void RWT_COMBO_X_Init();
		void RWT_COMBO_X_FrameMove();
		void RWT_COMBO_X_EventProcess();
		void RSI_WEAPON_TAKER_COMBO_XX_FrameMoveFuture();
		void RSI_WEAPON_TAKER_COMBO_XX_EventProcess();

		void RSI_WEAPON_TAKER_COMBO_XXX_StateStart();
		void RSI_WEAPON_TAKER_COMBO_XXX_FrameMove();
		void RSI_WEAPON_TAKER_COMBO_XXX_EventProcess();

		void RSI_WEAPON_TAKER_COMBO_XXX_LOOP_StateStart();
		void RSI_WEAPON_TAKER_COMBO_XXX_LOOP_FrameMove();
		void RSI_WEAPON_TAKER_COMBO_XXX_LOOP_EventProcess();
		void RSI_WEAPON_TAKER_COMBO_XXX_LOOP_StateEnd();

		void RSI_WEAPON_TAKER_COMBO_XXX_FINISH_FrameMove();
		void RSI_WEAPON_TAKER_COMBO_XXX_FINISH_EventProcess();

		void RWT_COMBO_ZZZX_Start();
		void RWT_COMBO_ZZZX_FrameMove();
		void RWT_COMBO_ZZZX_EventProcess();
		void RWT_COMBO_ZZZX_EndFuture();
		void RSI_WEAPON_TAKER_COMBO_ZZZXX_FrameMoveFuture();
		void RSI_WEAPON_TAKER_COMBO_ZZZXX_EventProcess();
		void RSI_WEAPON_TAKER_COMBO_ZZZdownX_FrameMove();
		void RSI_WEAPON_TAKER_COMBO_ZZZdownX_EventProcess();
		void RSI_WEAPON_TAKER_COMBO_ZZZdownXX_FrameMove();
		void RSI_WEAPON_TAKER_COMBO_ZZZdownXX_EventProcess();

		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_StateStart();
		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_FrameMove();
		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_EventProcess();
		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_StateEnd();

		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_StateStart();
		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_FrameMove();
		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_EventProcess();
		void RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_StateEnd();


		void RSI_A_RF_BREAKING_FIST_Init();
		void RSI_A_RF_BREAKING_FIST_StateStart();
		void RSI_A_RF_BREAKING_FIST_FrameMove();
		void RSI_A_RF_BREAKING_FIST_EventProcess();
		void RSI_A_RF_BREAKING_FIST_StateEnd();
#ifdef BALANCE_BLADE_MASTER_20130117	/// 캔슬시 움직임 멈추기 위해 추가
		void RSI_A_RF_BREAKING_FIST_StateEndFuture();
#endif BALANCE_BLADE_MASTER_20130117

		void RSI_A_RWT_BARRAGE_ATTACK_Init();
		void RSI_A_RWT_BARRAGE_ATTACK_StateStartFuture();
		void RSI_A_RWT_BARRAGE_ATTACK_STAND_EventProcess();
		void RSI_A_RWT_BARRAGE_ATTACK_STAND_FrameMove();
		void RSI_A_RWT_BARRAGE_ATTACK_AIR_EventProcess();
		void RSI_A_RWT_BARRAGE_ATTACK_AIR_FrameMove();
#ifdef RAVEN_LIMIT_AIR_COMBO		/// 공중 채공 콤보 제한
		void RSI_A_RWT_BARRAGE_ATTACK_AIR_FrameMoveFutrue();
#endif RAVEN_LIMIT_AIR_COMBO

		void RSI_SA_RWT_BURSTING_BLADE_Init();
		void RSI_SA_RWT_BURSTING_BLADE_StateStartFuture();
		void RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateStart();
		void RSI_SA_RWT_BURSTING_BLADE_ATTACK_FrameMove();
		void RSI_SA_RWT_BURSTING_BLADE_ATTACK_EventProcess();
		void RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateEnd();
#ifdef RAVEN_LIMIT_AIR_COMBO		/// 공중 채공 콤보 제한
		void RSI_SA_RWT_BURSTING_BLADE_ATTACK_AIR_FrameMoveFuture();
		void RSI_SA_RWT_BURSTING_BLADE_ATTACK_AIR_EventProcess();
#endif RAVEN_LIMIT_AIR_COMBO

		void RSI_SA_RWT_REVOLVER_CANNON_Init();
		void RSI_SA_RWT_REVOLVER_CANNON_StateStart();
		void RSI_SA_RWT_REVOLVER_CANNON_FrameMove();
		void RSI_SA_RWT_REVOLVER_CANNON_EventProcess();
		void RSI_SA_RWT_REVOLVER_CANNON_StateEnd();
		void RSI_SA_RWT_REVOLVER_CANNON_LOOP_StateStart();
		void RSI_SA_RWT_REVOLVER_CANNON_LOOP_FrameMove();
		void RSI_SA_RWT_REVOLVER_CANNON_LOOP_EventProcess();
		void RSI_SA_RWT_REVOLVER_CANNON_LOOP_StateEnd();
		void RSI_SA_RWT_REVOLVER_CANNON_FINISH_EventProcess();
		void RSI_SA_RWT_REVOLVER_CANNON_END_EventProcess();

		void RSI_SA_RWT_HARPOON_SPEAR_Init();
		void RSI_SA_RWT_HARPOON_SPEAR_StateStart();
		void RSI_SA_RWT_HARPOON_SPEAR_FrameMove();
		void RSI_SA_RWT_HARPOON_SPEAR_EventProcess();
		void RSI_SA_RWT_HARPOON_SPEAR_StateEnd();

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		/// 일반
		void RSI_SA_RWT_HARPOON_SPEAR_FIRE_EventProcess();

		/// 오버 히트
		void RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_StateStart();
		void RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_FrameMove();
		void RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_EventProcess();
#endif SERV_RAVEN_VETERAN_COMMANDER

		void RSI_SA_RWT_HELLFIRE_GATLING_Init();
		void RSI_SA_RWT_HELLFIRE_GATLING_StateStart();
		void RSI_SA_RWT_HELLFIRE_GATLING_FrameMove();
		void RSI_SA_RWT_HELLFIRE_GATLING_EventProcess();
		void RSI_SA_RWT_HELLFIRE_GATLING_StateEnd();
		void RSI_SA_RWT_HELLFIRE_GATLING_LOOP_StateStart();
		void RSI_SA_RWT_HELLFIRE_GATLING_LOOP_FrameMove();
		void RSI_SA_RWT_HELLFIRE_GATLING_LOOP_EventProcess();
		void RSI_SA_RWT_HELLFIRE_GATLING_LOOP_StateEnd();
		void RSI_SA_RWT_HELLFIRE_GATLING_END_EventProcess();

		void RSI_SA_RWT_GIGA_PROMINENCE_Init();
		void RSI_SA_RWT_GIGA_PROMINENCE_StateStart();
		void RSI_SA_RWT_GIGA_PROMINENCE_FrameMove();
		void RSI_SA_RWT_GIGA_PROMINENCE_EventProcess();
		void RSI_SA_RWT_GIGA_PROMINENCE_StateEnd();
		void RSI_SA_RWT_GIGA_PROMINENCE_LOOP_StateStart();
		void RSI_SA_RWT_GIGA_PROMINENCE_LOOP_EventProcess();
		void RSI_SA_RWT_GIGA_PROMINENCE_LOOP_FrameMove();
		void RSI_SA_RWT_GIGA_PROMINENCE_LOOP_StateEnd();
		void RSI_SA_RWT_GIGA_PROMINENCE_FINISH_FrameMove();
		void RSI_SA_RWT_GIGA_PROMINENCE_FINISH_EventProcess();
		void RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_StateStart();
		void RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_FrameMove();
		void RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_EventProcess();
		void RSI_SA_RWT_GIGA_PROMINENCE_END_EventProcess();

		void RSI_SA_RWT_GIGA_PROMINENCE_OVER_END_StateStart();
		void RSI_SA_RWT_GIGA_PROMINENCE_OVER_END_EventProcess();

		void _GetGigaProminencePosition( OUT bool& bRight, OUT float& fDistance );

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		void _ShowOverheatIcon( int iSize = 0 );
#else  SERV_RAVEN_VETERAN_COMMANDER
		void _ShowOverheatIcon();
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		bool _CanOverheat( bool bShowMassage = true );
#else  SERV_RAVEN_VETERAN_COMMANDER
		bool _CanOverheat();
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		void _BurnOverheatHP( const CX2DamageManager::DamageData& AttDamageData, float fDecreaseHPRate = -1.f, float fDecreaseHPRate2 = 1.f );
#else  SERV_RAVEN_VETERAN_COMMANDER
		void _BurnOverheatHP( const CX2DamageManager::DamageData& AttDamageData );
#endif SERV_RAVEN_VETERAN_COMMANDER

		void _SetOverheat( bool bOn = true, float fDuration = -1.f );

#endif RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		/// RVC_DASH_JUMP_COMBO_X				- 일반적인 공격
		void RVC_DASH_JUMP_COMBO_X_Init();
		void RVC_DASH_JUMP_COMBO_X_Start();
		void RVC_DASH_JUMP_COMBO_X_FrameMoveFuture();
		void RVC_DASH_JUMP_COMBO_X_FrameMove();
		void RVC_DASH_JUMP_COMBO_X_EventProcess();

		/// RVC_DASH_JUMP_COMBO_XX_START		- 오버 히트 시작
		void RVC_DASH_JUMP_COMBO_XX_START_StateStart();
		void RVC_DASH_JUMP_COMBO_XX_START_FrameMoveFuture();
		void RVC_DASH_JUMP_COMBO_XX_START_FrameMove();
		void RVC_DASH_JUMP_COMBO_XX_START_EventProcess();
		void RVC_DASH_JUMP_COMBO_XX_START_StateEnd();

		/// RVC_DASH_JUMP_COMBO_XX_LOOP		- 오버 히트 연사중
		void RVC_DASH_JUMP_COMBO_XX_LOOP_StateStart();
		void RVC_DASH_JUMP_COMBO_XX_LOOP_FrameMoveFuture();
		void RVC_DASH_JUMP_COMBO_XX_LOOP_FrameMove();
		void RVC_DASH_JUMP_COMBO_XX_LOOP_EventProcess();
		void RVC_DASH_JUMP_COMBO_XX_LOOP_StateEnd();

		/// RVC_DASH_JUMP_COMBO_XX_FINISH		- 오버 히트 막타
		void RVC_DASH_JUMP_COMBO_XX_FINISH_StateStart();
		void RVC_DASH_JUMP_COMBO_XX_FINISH_FrameMoveFuture();
		void RVC_DASH_JUMP_COMBO_XX_FINISH_FrameMove();
		void RVC_DASH_JUMP_COMBO_XX_FINISH_EventProcess();
		void RVC_DASH_JUMP_COMBO_XX_FINISH_StateEnd();

		/// RVC_DASH_COMBO_ZZZ				- 오버 히트 전 돌아서는 동작
		void RVC_DASH_COMBO_ZZZ_StateStart();
		void RVC_DASH_COMBO_ZZZ_EventProcess();
		void RVC_DASH_COMBO_ZZZ_StateEnd();

		/// RVC_DASH_COMBO_ZZZPushZ_READY		- 화염 방사 전 준비 동작
		void RVC_DASH_COMBO_ZZZPushZ_READY_StateStart();
		void RVC_DASH_COMBO_ZZZPushZ_READY_EventProcess();
		void RVC_DASH_COMBO_ZZZPushZ_READY_StateEnd();

		/// RVC_DASH_COMBO_ZZZPushZ			- 오버 히트 ( 화염 방사 )
		void RVC_DASH_COMBO_ZZZPushZ_StateStart();
		void RVC_DASH_COMBO_ZZZPushZ_FrameMove();
		void RVC_DASH_COMBO_ZZZPushZ_EventProcess();
		void RVC_DASH_COMBO_ZZZPushZ_StateEnd();

		/// RVC_DASH_COMBO_ZZZZ_FINISH		- 오버 히트 마무리 동작
		void RVC_DASH_COMBO_ZZZZ_FINISH_EventProcess();

		/// RVC_SI_SA_DEADLY_RAID
		void RVC_SI_SA_DEADLY_RAID_Init();
		void RVC_SI_SA_DEADLY_RAID_StateStart();
		void RVC_SI_SA_DEADLY_RAID_FrameMove();
		void RVC_SI_SA_DEADLY_RAID_EventProcess();

		/// RVC_SI_SA_DEADLY_RAID_LOOP
		void RVC_SI_SA_DEADLY_RAID_LOOP_StateStart();
		void RVC_SI_SA_DEADLY_RAID_LOOP_FrameMove();
		void RVC_SI_SA_DEADLY_RAID_LOOP_EventProcess();

		/// RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT
		void RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_StateStart();
		void RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_FrameMove();
		void RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_EventProcess();

		/// RVC_SI_SA_DEADLY_RAID_LOOP_END
		void RVC_SI_SA_DEADLY_RAID_LOOP_END_StateStart();
		void RVC_SI_SA_DEADLY_RAID_LOOP_END_FrameMove();
		void RVC_SI_SA_DEADLY_RAID_LOOP_END_EventProcess();

		/// RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT
		void RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_StateStart();
		void RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_FrameMove();
		void RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_EventProcess();
		
		/// RVC_SI_SA_DEADLY_RAID_FINISH
		void RVC_SI_SA_DEADLY_RAID_FINISH_StateStart();
		void RVC_SI_SA_DEADLY_RAID_FINISH_FrameMove();
		void RVC_SI_SA_DEADLY_RAID_FINISH_EventProcess();

		/// RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT
		void RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_StateStart();
		void RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_FrameMove();
		void RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_EventProcess();

		/// RVC_SI_SA_DEADLY_RAID_END
		void RVC_SI_SA_DEADLY_RAID_END_EventProcess();

		/// RVC_SI_SA_IGNITION_CROW
		void RVC_SI_SA_IGNITION_CROW_Init();
		void RVC_SI_SA_IGNITION_CROW_StateStart();
		void RVC_SI_SA_IGNITION_CROW_FrameMove();
		void RVC_SI_SA_IGNITION_CROW_EventProcess();
		void RVC_SI_SA_IGNITION_CROW_StateEnd();

		/// RVC_SI_SA_IGNITION_CROW
		void RVC_SI_SA_IGNITION_CROW_FIRE_FrameMove();
		void RVC_SI_SA_IGNITION_CROW_FIRE_EventProcess();

		void RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_StateStart();
		void RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_FrameMove();
		void RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_EventProcess();

		void RVC_SI_SA_IGNITION_CROW_OVERHEAT_END_EventProcess();

		/// RVC_SI_P_SHADOW_BACK_SLIDE
		void RVC_SI_P_SHADOW_BACK_SLIDE_Init();
		void RVC_SI_P_SHADOW_BACK_SLIDE_StartFuture();
		void RVC_SI_P_SHADOW_BACK_SLIDE_Start();
		void RVC_SI_P_SHADOW_BACK_SLIDE_FrameMove();
		void RVC_SI_P_SHADOW_BACK_SLIDE_EventProcess();
		void RVC_SI_P_SHADOW_BACK_SLIDE_End();

		/// RVC_SI_P_SHADOW_STEP_FINISH
		void RVC_SI_P_SHADOW_STEP_FINISH_Init();
		void RVC_SI_P_SHADOW_STEP_FINISH_StartFuture();
		void RVC_SI_P_SHADOW_STEP_FINISH_Start();
		void RVC_SI_P_SHADOW_STEP_FINISH_FrameMove();
		void RVC_SI_P_SHADOW_STEP_FINISH_EventProcess();
		void RVC_SI_P_SHADOW_STEP_FINISH_End();

		/// RVC_SI_P_SHADOW_BACK_SLIDE_END
		void RVC_SI_P_SHADOW_BACK_SLIDE_END_FrameMoveFuture();
		void RVC_SI_P_SHADOW_BACK_SLIDE_END_EventProcess();

		/// RVC_SI_P_SHADOW_BUSTER
		void RVC_SI_P_SHADOW_BUSTER_Init();
		void RVC_SI_P_SHADOW_BUSTER_Start();
		void RVC_SI_P_SHADOW_BUSTER_FrameMove();
		void RVC_SI_P_SHADOW_BUSTER_EventProcess();

		/// RVC_SI_A_BLEEDING_SLICE
		void RVC_SI_A_BLEEDING_SLICER_Init();
		void RVC_SI_A_BLEEDING_SLICER_StateStart();
		void RVC_SI_A_BLEEDING_SLICER_EventProcess();

		/// RVC_SI_A_BLEEDING_SLICER_GROUND
		void RVC_SI_A_BLEEDING_SLICER_GROUND_StateStart();
		void RVC_SI_A_BLEEDING_SLICER_GROUND_FrameMove();
		void RVC_SI_A_BLEEDING_SLICER_GROUND_EventProcess();
		void RVC_SI_A_BLEEDING_SLICER_GROUND_StateEnd();

		/// RVC_SI_A_BLEEDING_SLICER_AIR
		void RVC_SI_A_BLEEDING_SLICER_AIR_StateStart();
		void RVC_SI_A_BLEEDING_SLICER_AIR_FrameMove();
		void RVC_SI_A_BLEEDING_SLICER_AIR_FrameMoveFuture();
		void RVC_SI_A_BLEEDING_SLICER_AIR_EventProcess();
		void RVC_SI_A_BLEEDING_SLICER_AIR_StateEnd();

		/// 데들리 레이드
		bool	PossibleDeadlyRaidLoop();											/// 데들리 레이드 오버 히트 가능 여부 및 이모티콘 설정
		bool	ChangeDeadlyRaidLoop();												/// 데들리 레이드 반복 State 변경 가능 여부
		wstring DeadlyRaidLoopDamageEffectName();									/// 데들리 레이드 반복 State 에서 사용할 DamageEffect 이름 반환

		/// 섀도우 백 슬라이딩
		void SetShadowStepAttackSkill( RAVEN_STATE_ID eStateID = RSI_SHADOW_STEP_THRUST );			/// 섀도우 스텝,백 슬라이드 도중 공격 선택 제어
		void InitShadowStep();														/// 섀도우 스텝 계열 초기화 함수

		/// 오버 히트 발생시 생성 구조물
		void CreateOverHeatObject();
		void DeleteOverHeatObject();
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef SKILL_FOR_CANCEL
		bool CheckPressedSkillForCancel();
#endif	SKILL_FOR_CANCEL

		void ShowActiveSkillCutInAndLight( const WCHAR* szBoneName_, const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ = false );
		virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

		void CheckAndDoReversalSoldier();
		void CheckAndDoServivalTechniqueOfMercenary();

//{{ kimhc // 2010.11.12 // 레피-와일드차지
#ifdef	NEW_SKILL_2010_11
		WildChargeDataPtr m_WildChargeDataPtr; /// 와일드차지 스킬 데이터
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.12 // 레피-와일드차지

#ifdef UPGRADE_RAVEN		
		bool							m_bLockOnNpc;
		UidType							m_iLockOnUid;

		bool							m_bParrying;
		bool							m_bCanParrying;
		float							m_fParryingTime;
		bool							m_bAttackNasodBall;
		bool							m_bDelayAttackNasodBall;
		float							m_fDelayAttackTime;
		int								m_iNaosdBallLv;
		float							m_fNasodBallAttackTime;
		KProtectedType<float>			m_fRageGauge;
		ArcEnemyData*					m_pNasodBall;
		bool							m_bHyperAura;
		CX2EffectSet::Handle			m_hHyperAuraEffectset;
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hSeqHyperBall;
#ifndef FIX_AFTER_IMAGE
		CKTDGXSkinAfterImage*							m_pAfterImageNasodBall;
#endif
#endif
		float							m_fOldHp;

#ifdef BALANCE_BLADE_MASTER_20130117
		RAVEN_STATE_ID					m_eRavenCurrentStage;		/// 블레이드 마스터 ZZZXZZZ 콤보 캔슬시 클라간 동기화를 맞추기 위해, 캔슬시 스테이트를 저장
#endif BALANCE_BLADE_MASTER_20130117

#ifdef UPGRADE_SKILL_SYSTEM_2013
		/// RSI_A_RF_MEGADRILL_BREAK 메가드릴 브레이크
		void RSI_A_RF_MEGADRILL_BREAK_Init();
		void RSI_A_RF_MEGADRILL_BREAK_FrameMoveFuture();
		void RSI_A_RF_MEGADRILL_BREAK_FrameMove();
		void RSI_A_RF_MEGADRILL_BREAK_EventProcess();
		void RSI_A_RF_MEGADRILL_BREAK_End();

		/// SI_A_RBM_SONIC_SLASH 소닉 슬래시
		void RSI_A_RBM_SONIC_SLASH_LOOP_Init();
		void RSI_A_RBM_SONIC_SLASH_LOOP_StateStart();
		void RSI_A_RBM_SONIC_SLASH_LOOP_FrameMove();
		void RSI_A_RBM_SONIC_SLASH_LOOP_EventProcess();
		void RSI_A_RBM_SONIC_SLASH_FINISH_EventProcess();

		/// RSI_SA_RST_SHOCKWAVE_FRONT 쇼크 웨이브 - 속
		void RSI_SA_RST_SHOCKWAVE_FRONT_Init();
		void RSI_SA_RST_SHOCKWAVE_FRONT_StateStart();
		void RSI_SA_RST_SHOCKWAVE_FRONT_FrameMove();
		void RSI_SA_RST_SHOCKWAVE_FRONT_EventProcess();
		void RSI_SA_RST_SHOCKWAVE_FRONT_StateEnd();

		/// RSI_SA_RST_SHOCKWAVE_FRONT 기가 드라이브 - 폭
		void RSI_SA_RBM_GIGA_DRIVE_BURST_Init();
		void RSI_SA_RBM_GIGA_DRIVE_BURST_StateStart();
		void RSI_SA_RBM_GIGA_DRIVE_BURST_FrameMove();
		void RSI_SA_RBM_GIGA_DRIVE_BURST_EventProcess();
		void RSI_SA_RBM_GIGA_DRIVE_BURST_StateEnd();

		/// RSI_SA_ROT_CHARGED_BOLT_BLOOD 차지드 볼트 - 쏜즈 블러드
		void RSI_SA_ROT_CHARGED_BOLT_BLOOD_Init();
		void RSI_SA_ROT_CHARGED_BOLT_BLOOD_FrameMove();
		void RSI_SA_ROT_CHARGED_BOLT_BLOOD_EventProcess();


		// RSI_A_RWT_FLAME_SWORD 화염인, 김종훈
		void RSI_A_RWT_FLAME_SWORD_Init ();
		void RSI_A_RWT_FLAME_SWORD_StateStart ();
		void RSI_A_RWT_FLAME_SWORD_EventProcess ();


		// RSI_A_RWT_FLAME_SWORD 화염인 지상에서 사용, 김종훈
		void RSI_A_RWT_FLAME_SWORD_GROUND_StateStart ();
		void RSI_A_RWT_FLAME_SWORD_GROUND_EventProcess ();
		
		// RSI_A_RWT_FLAME_SWORD 화염인 공중에서 사용, 김종훈
		void RSI_A_RWT_FLAME_SWORD_AIR_StateStart ();
		void RSI_A_RWT_FLAME_SWORD_AIR_FrameMoveFuture ();
		void RSI_A_RWT_FLAME_SWORD_AIR_EventProcess ();

		// RSI_A_RRF_LIMIT_CRUSHER 리미트 크러셔, 김종훈
		void RSI_A_RRF_LIMIT_CRUSHER_CHARGE_Init();
		void RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateStartFuture();
		void RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateStart();
		void RSI_A_RRF_LIMIT_CRUSHER_CHARGE_EventProcess();
		void RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateEnd();

		void RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK_EventProcess ();

		void RSI_A_RRF_LIMIT_CRUSHER_AVOID_ATTACK_EventProcess ();

		// RSI_A_RRF_HELL_DIVE 헬 다이브, 김종훈
		void RSI_A_RRF_HELL_DIVE_Init ();
		void RSI_A_RRF_HELL_DIVE_FrameMove ();
		void RSI_A_RRF_HELL_DIVE_EventProcess ();

		// RSI_A_RVC_NAPALM_GRENADE 네이팜 그레네이드, 김종훈
		void RSI_A_RVC_NAPALM_GRENADE_Init ();
		void RSI_A_RVC_NAPALM_GRENADE_EventProcess ();
		void RSI_A_RVC_NAPALM_GRENADE_FrameMove ();
		
		// RSI_SA_RWT_REVOLVER_CANNON_HE 리볼버 캐논 HE, 김종훈
		void RSI_SA_RWT_REVOLVER_CANNON_HE_Init ();
		void RSI_SA_RWT_REVOLVER_CANNON_HE_StateStart ();
		void RSI_SA_RWT_REVOLVER_CANNON_HE_EventProcess ();
		void RSI_SA_RWT_REVOLVER_CANNON_HE_FrameMove ();

		// RSI_SA_RWT_REVOLVER_CANNON_HE 리볼버 캐논 HE 루프, 김종훈
		void RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_StateStart();
		void RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_FrameMove ();
		void RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_EventProcess ();
		void RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH_EventProcess ();

		// RSI_SA_RWT_REVOLVER_CANNON_HE 리볼버 캐논 HE 엔드, 김종훈

		void RSI_SA_RWT_REVOLVER_CANNON_HE_END_EventProcess ();
		
		/// RVC_SI_SA_IGNITION_CROW_INCINERATION 이그니션 크로우 인시너레이션, 김종훈
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_Init();
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_StateStart();
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_FrameMove();
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_EventProcess();

		/// RVC_SI_SA_IGNITION_CROW_INCINERATION 이그니션 크로우 인시너레이션 발사, 김종훈
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_EventProcess();

		/// RVC_SI_SA_IGNITION_CROW_INCINERATION 이그니션 크로우 인시너레이션 오버히트 발사, 김종훈
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_StateStart();
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_FrameMove();
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_EventProcess();

		/// RVC_SI_SA_IGNITION_CROW_INCINERATION 이그니션 크로우 인시너레이션 오버히트 종료, 김종훈
		void RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END_EventProcess();

		//  RSI_P_RBM_SHADOW_STEP_THRUST 쉐도우 쓰러스트, 김종훈
		// 블마 쉐도우 쓰러스트용 State 추가
		// 기존 Shadow Step 과는 다른 것에 유의!
		// 해당 스킬 배운 상태에서 X 버튼을 누르면 나간다.
		void RSI_P_RBM_SHADOW_STEP_THRUST_Init();
		void RSI_P_RBM_SHADOW_STEP_THRUST_Start();
		void RSI_P_RBM_SHADOW_STEP_THRUST_EventProcess();

		void RSI_P_RRF_SHADOW_PUNISHER_Init ();
		void RSI_P_RRF_SHADOW_PUNISHER_Start ();
		void RSI_P_RRF_SHADOW_PUNISHER_EventProcess ();

		void RSI_A_ROT_ARMOR_BREAK_Init();
		void RSI_A_ROT_ARMOR_BREAK_EventProcess();
		

		void SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ );
		void SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet, RAVEN_STATE_ID eStateID );
#endif UPGRADE_SKILL_SYSTEM_2013


#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
		// 인페르날 암스 
		void RSI_HA_RRF_INFERNAL_ARM_Init();
		void RSI_HA_RRF_INFERNAL_ARM_EventProcess();

		// 익스트림 블레이드 FX
		void RSI_HA_RBM_EXTREM_BLADE_READY_Init();
		void RSI_HA_RBM_EXTREM_BLADE_READY_EventProcess();

		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_Init();
		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_StateStart();
		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_FrameMove(); 
		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_EventProcess();
		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_StateEnd();

		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_Init();
		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_StartFuture();
		void RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_EventProcess();

		#pragma region SI_HA_RVC_BURNING_BUSTER
		void RSI_HA_RVC_BURNING_BUSTER_Init();
		#pragma endregion 버닝 버스터 - 궁극기


		void SetInvisibility(bool bVal_);
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
};

IMPLEMENT_INTRUSIVE_PTR( CX2GURaven::WildChargeData );
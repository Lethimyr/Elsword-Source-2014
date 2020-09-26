#pragma once

class CX2GULire_ElvenRanger : public CX2GUUser
{
	public:
		enum LIRE_ELVENRANGER_STATE_ID
		{
			LESI_BASE = USI_END + 1,
			
			LESI_DOUBLE_JUMP,
			LESI_DASH_DOUBLE_JUMP,
			LESI_DOUBLE_JUMP_ATTACK_Z,
			LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP,
			LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X,
			LESI_DOUBLE_JUMP_ATTACK_Z_LANDING,

			LESI_CHARGE_MP,

			LESI_STANDUP_ROLLING_FRONT_FRONT,
			LESI_STANDUP_ROLLING_FRONT_BACK,
			LESI_STANDUP_ROLLING_BACK_FRONT,
			LESI_STANDUP_ROLLING_BACK_BACK,

			LESI_STANDUP_ATTACK_FRONT,
			LESI_STANDUP_ATTACK_BACK,

			LESI_SIEGE_READY,
			LESI_SIEGE_WAIT,
			LESI_SIEGE_ATTACK,
			LESI_SIEGE_ATTACK_X,
			LESI_SIEGE_END,

			LESI_COMBO_Z1,
			LESI_COMBO_Z2,
			LESI_COMBO_Z3,
			LESI_COMBO_Z4,
			LESI_COMBO_Z4a,
			LESI_COMBO_Z4UP,

			LESI_COMBO_X1,
			LESI_COMBO_X2,
			LESI_COMBO_X3,
			LESI_COMBO_X4,
			LESI_COMBO_X4UP,
			LESI_COMBO_X4DOWN,

			LESI_DASH_COMBO_Z1,
			LESI_COMBAT_RANGER_DASH_COMBO_Z2,
			LESI_DASH_COMBO_Z2,
			LESI_DASH_ATTACK_X,
			LESI_SNIPING_RANGER_DASH_COMBO_X2,
#ifndef BALANCE_GRAND_ARCHER_20121213
			LESI_SNIPING_RANGER_DASH_COMBO_X3,
			LESI_SNIPING_RANGER_DASH_COMBO_X3_UP,
			LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN,
#endif //BALANCE_GRAND_ARCHER_20121213

			LESI_JUMP_ATTACK_Z,
			LESI_JUMP_ATTACK_X,

			LESI_DASH_JUMP_ATTACK_Z,
			LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2,
			LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3,

			LESI_DASH_JUMP_ATTACK_Z_LANDING,

			LESI_DASH_JUMP_COMBO_X1,
			LESI_DASH_JUMP_COMBO_X2,
			LESI_DASH_JUMP_COMBO_X3,
			LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3,

			LESI_SI_A_LE_REFLEX_MAGIC,

//#ifdef WALL_JUMP_TEST
			LESI_WALL_LANDING,
//#endif WALL_JUMP_TEST

			LESI_SA_LCR_SHARPFALL_LANDING,

#ifdef SKILL_CASH_10_TEST
			LESI_SA_LCR_VIOLENT_ATTACK,
			LESI_SA_LCR_VIOLENT_ATTACK_HYPER,
			LESI_SA_LCR_VIOLENT_ATTACK_LANDING,
			LESI_SA_LCR_VIOLENT_ATTACK_LANDING_HYPER,
#endif


			// 레나 2차전직 콤보
			LESI_LWS_ZZfrontZ,
			LESI_LWS_ZZfrontZZ,
			LESI_LWS_ZZfrontZZZ, 
			LESI_LWS_ZZfrontZZZ_Landing,

			//LESI_LWS_DASH_X,
			LESI_LWS_DASH_XZ,
			LESI_LWS_DASH_XZZ,

#ifdef BALANCE_GRAND_ARCHER_20121213
			LESI_LGA_ZZX,
			LESI_LGA_ZZXX,
#else
			LESI_LGA_ZZZX,
			LESI_LGA_ZZZXX,
#endif //BALANCE_GRAND_ARCHER_20121213
			LESI_LGA_DASH_JUMP_Z,
			LESI_LGA_DASH_JUMP_ZX,



			LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH,	// 타격후에 피격자를 통과해서 뒤로 미끄러져 들어가는 상태
			LESI_SA_LWS_SLIDE_KICK_FINISH,			// 
			LESI_SA_LWS_SLIDE_KICK_CANCEL,

#ifdef	SERV_TRAPPING_RANGER_TEST
			LESI_COMBO_RTR_ZZZ,
			LESI_COMBO_RTR_ZZZ_DZ,
			LESI_COMBO_RTR_ZZZZ,
			LESI_COMBO_RTR_ZZZZZ,

			LESI_DASH_COMBO_RTR_ZX,

			LESI_DASH_JUMP_ATTACK_RTR_Z,
			LESI_DASH_JUMP_ATTACK_RTR_ZZ,

			LESI_SA_RTR_FATALITY_SUCCESS,
			LESI_SA_RTR_FATALITY_FAIL,

			LESI_SI_A_LE_RISING_FALCON_LANDING,
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER			//레나 2차 전직 나이트 와쳐
			//ZXXX 콤보
			LESI_COMBO_RNW_ZZX,
			LESI_COMBO_RNW_ZZXX,
			LESI_COMBO_RNW_ZZXXX,

			//→→  →↑ZZ 콤보
			LESI_RNW_ACCEL_DASH,
			LESI_RNW_ACCEL_DASH_JUMP,
			LESI_RNW_ACCEL_DASH_JUMP_LANDING,
			LESI_RNW_ACCEL_DASH_JUMP_Z,
			LESI_RNW_ACCEL_DASH_JUMP_ZZ,

			//퓨리어스 인게이지
			LESI_SA_RNW_FURIOUS_ENGAGE,

			//지연의 신호탄
			LESI_A_RNW_START_OF_DELAYED_FIRING,
			//지연의 신호탄 - 전체 폭파
			LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE,
#endif SERV_RENA_NIGHT_WATCHER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			LESI_A_LGA_RAPID_SHOT_READY,					/// 래피드 샷 준비
			LESI_A_LGA_RAPID_SHOT_ATTACK,					/// 래피드 샷 공격
			LESI_A_LGA_RAPID_SHOT_FINISH,					/// 래피드 샷 마무리(마지막 화살 한발을 발사)
			LESI_P_LE_AGILE_MOVEMENT_FRONT_FRONT,			/// 재빠른 몸놀림 
			LESI_P_LE_AGILE_MOVEMENT_FRONT_BACK,
			LESI_P_LE_AGILE_MOVEMENT_BACK_FRONT,
			LESI_P_LE_AGILE_MOVEMENT_BACK_BACK,
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
			LESI_HA_LWS_SPIRAL_STIKE_SPINING,
			LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK,
			
			LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT,			
			LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING,

			LESI_HA_LNW_INNOCENT_START,	//이노센트 기 모으며 공격 시작
			LESI_HA_LNW_INNOCENT_END,	//이노센트 칼로 찌르며 마무리
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템

#ifdef ADD_RENA_SYSTEM //김창한
			LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX,
#endif //ADD_RENA_SYSTEM

			//////////////////////////////////////////////////////////////////////////
			// 위쪽에 추가해주세요~ 이 아래는 테스트로 추가된 enum 입니다


			LESI_TRANSFORMED,



//#ifdef PVP_BOSS_COMBAT_TEST
//			LESI_FROZEN,
//#endif PVP_BOSS_COMBAT_TEST


		};

		enum RENA_MAJOR_PARTICLE_INSTANCE_ID
		{
			RENA_MAJOR_PII_WIND_SNEAKER_IMPACT_LEAF,
			RENA_MAJOR_PII_PHOENIX_START_WAVE,
			RENA_MAJOR_PII_END,
		};

		enum RENA_MINOR_PARTICLE_INSTANCE_ID
		{
			RENA_MINOR_PII_IMPACT_TICK_BLUE,
			RENA_MINOR_PII_END,
		};

		struct WindSneakerData
		{
			float m_fTimeLeft;
			bool m_bEnlargedFootAttackBox; // 발에 붙은 어택박스 크기를 확대한 상태인지
			float m_fMPCost;
			//float m_fAttackPowerRate;
			//float m_fForceDownRate;

			WindSneakerData()
			: m_fTimeLeft( 0.f )
			, m_bEnlargedFootAttackBox( false )
			, m_fMPCost( 8.f )
			//, m_fAttackPowerRate( 0.f )
			//, m_fForceDownRate( 0.f )
			{
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_fTimeLeft > 0.f )
				{
					m_fTimeLeft -= fElapsedTime;
				}
			}
		};


		// 스킬에 의해 화살공격의 수치가 변경되는 것과 관련된 데이타
		struct ArrowData
		{
			float m_fAddBackSpeedX;
#ifdef NEW_SKILL_2010_11
			// oasis907 : 김상윤 [2010.11.4] // 그랜드 아처 - 진동 사격(패시브)
			float m_fVibrationShootingInvokeRate;
			float m_fVibrationShootingDamageRate;
#endif NEW_SKILL_2010_11
			ArrowData()
			: m_fAddBackSpeedX( 0.f )
#ifdef NEW_SKILL_2010_11
			// oasis907 : 김상윤 [2010.11.4] // 그랜드 아처 - 진동 사격(패시브)
			, m_fVibrationShootingInvokeRate( 0.f )
			, m_fVibrationShootingDamageRate( 0.f )
#endif NEW_SKILL_2010_11
			{
			}
		};

		struct ChargeMPData
		{
			float m_fChargeStateMP;			// 차지 상태에서 MP 차는 속도 증가 수치

			ChargeMPData()
			: m_fChargeStateMP( 0.f )
			{
			}
		};

#ifdef NEW_SKILL_2010_11
		//{{ oasis907 : 김상윤 [2010.11.3] // 윈드 스니커 - 자연과의 친화(패시브)
		struct FriendshipNatureData : public TimeLimited
		{
			bool  m_bCanActivate;
			float m_fInvokeRate;
			float m_fTimeElapsed;

			FriendshipNatureData()
			{
				m_bCanActivate			= true;
				m_fInvokeRate			= 0.f;
				m_fTimeElapsed			= 15.f;
			}
		
			bool CanActivate(){ return m_bCanActivate; }

			virtual void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_bCanActivate == false && m_fTimeElapsed < 15.f )
				{
					m_fTimeElapsed += fElapsedTime;
				}
				else
				{
					m_bCanActivate = true;
					m_fTimeElapsed = 0.f;
				}
				TimeLimited::OnFrameMove(fTime, fElapsedTime);
			}
		};
		//}} oasis907 : 김상윤 [2010.11.3] // 윈드 스니커 - 자연과의 친화(패시브)
#endif NEW_SKILL_2010_11

#ifdef	SERV_TRAPPING_RANGER_TEST
		enum FATALITY_RATING_RANK
		{
			FRR_LUCKY = 0,
			FRR_NORMAL,
			FRR_POOR,
			FRR_FAIL,
		};

		struct FatalityData{
			FATALITY_RATING_RANK	m_eRating;
			float					m_fPowerRate;

			FatalityData()
			{
				Init();
			}

			void Init()
			{
				m_eRating		= FRR_FAIL;
				m_fPowerRate	= 0.f;
			}

			float GetPowerRate()	{ return m_fPowerRate; }
		};
#endif SERV_TRAPPING_RANGER_TEST


#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한

        struct  RapidShotData;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        typedef boost::intrusive_ptr<RapidShotData> RapidShotDataPtr;	/// RapidShotData 구조체의 스마트 포인터 타입
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		typedef boost::shared_ptr<RapidShotData> RapidShotDataPtr;	/// RapidShotData 구조체의 스마트 포인터 타입
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		struct RapidShotData : boost::noncopyable
		{
		private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			LIRE_ELVENRANGER_STATE_ID m_eSlotID_RapidShot;		/// 래피드 샷이 장착된 스킬슬롯을 알아내기 위한 용도( 스킬 슬롯이 스테이트중 하나로 로 구성 되어 있음 )
			bool m_bSlotB;										/// 래피드 샷이 장착된 슬롯이 A인지, B인지 구분하기 위한용도
			bool* m_pSkillKey;									/// 래피드 샷의 키가 눌려있는지 알아내기 위안 용도 (메모리를 생성하는 것이 아니므로 해제하지 않는다.)
			float m_fTimeAfterStart;							/// 래피드 샷 스킬 시작 후 지난 시간.

			RapidShotData() : m_eSlotID_RapidShot( LESI_BASE ), m_bSlotB( false ), m_pSkillKey( NULL ), m_fTimeAfterStart( 0.f )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			{}

		public:

			static RapidShotDataPtr  CreateRapidShotData() { return RapidShotDataPtr( new RapidShotData ); }
			LIRE_ELVENRANGER_STATE_ID GetSlotID_RapidShot() const { return m_eSlotID_RapidShot; }
			void SetSlotID_RapidShot(LIRE_ELVENRANGER_STATE_ID eStateID_) { m_eSlotID_RapidShot = eStateID_; }

			void SetTimeAfterStart( float fTimeAfterStart_ ){ m_fTimeAfterStart = fTimeAfterStart_; }
			const float GetTimeAfterStart(){ return m_fTimeAfterStart; }

			bool GetSlotB() const { return m_bSlotB; }
			void SetSlotB(bool bSlotB_) { m_bSlotB = bSlotB_; }

			bool GetSkillKey() const { return *m_pSkillKey; }
			void SetSkillKeyPointer(bool* pSkillKey_) { m_pSkillKey = pSkillKey_; }

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            void    AddRef()    {   ++m_uRefCount; }
            void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		};
#endif //UPGRADE_SKILL_SYSTEM_2013

	private:
		CX2GULire_ElvenRanger( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit );
		virtual ~CX2GULire_ElvenRanger(void);


	public:
		static CX2GULire_ElvenRanger* CreateGULire( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			int frameBufferNum, CX2Unit* pUnit )
		{
			return new CX2GULire_ElvenRanger( unitIndex, teamNum, 
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
		
		virtual void		Win();
		virtual void		Lose();
		virtual void		Draw();

		virtual void		InitEffect();
		virtual void		InitEnchantWeaponEffectUnitSpecific();

		void				InitializeLireMajorParticleArray();	// 메이저 파티클 배열을 초기화 // kimhc // 2010.11.5 
		virtual void		CreateMajorParticleArray();		// 메이저 파티클 배열을 생성 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void			AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetRenaMajorParticleByEnum( RENA_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		
		ParticleEventSequenceHandle	GetHandleRenaMajorParticleByEnum( RENA_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const // 캐릭터만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		{
			ASSERT( RENA_MAJOR_PII_END > eVal_ && RENA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRenaMajorParticleInstance[eVal_];
		}

		ParticleEventSequenceHandle& GetHandleReferenceRenaMajorParticleByEnum( RENA_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) // 캐릭터만 쓰는 메이저 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		{
			ASSERT( RENA_MAJOR_PII_END > eVal_ && RENA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRenaMajorParticleInstance[eVal_];
		}

		void				SetHandleRenaMajorParticleByEnum( RENA_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ ) // 캐릭터만 쓰는 메이저 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함 // kimhc // 2010.11.5 
		{
			ASSERT( RENA_MAJOR_PII_END > eVal_ && RENA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahRenaMajorParticleInstance[eVal_] = hHandle_;
		}
		void				DeleteLireMajorParticle();


		void				InitializeLireMinorParticleArray();	// 공통적으로 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
		virtual void		CreateMinorParticleArray();		// 각 캐릭터들만 쓰는 마이너 파티클 배열을 초기화 // kimhc // 2010.11.5 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void			AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		CKTDGParticleSystem::CParticleEventSequence* SetRenaMinorParticleByEnum( RENA_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// 각 캐릭터들만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴 // kimhc // 2010.11.5 
		
		ParticleEventSequenceHandle	GetHandleRenaMinorParticleByEnum( RENA_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const	// 캐릭터만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴	// kimhc // 2010.11.5 
		{
			ASSERT( RENA_MINOR_PII_END > eVal_ && RENA_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRenaMinorParticleInstance[eVal_];
		}

		ParticleEventSequenceHandle& GetHandleReferenceRenaMinorParticleByEnum( RENA_MINOR_PARTICLE_INSTANCE_ID eVal_ ) // 캐릭터만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴	// kimhc // 2010.11.5 
		{
			ASSERT( RENA_MINOR_PII_END > eVal_ && RENA_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			return m_ahRenaMinorParticleInstance[eVal_];
		}

		void				SetHandleRenaMinorParticleByEnum( RENA_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )	// 캐릭터만 쓰는 마이너 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함	// kimhc // 2010.11.5 
		{
			ASSERT( RENA_MINOR_PII_END > eVal_ && RENA_MINOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
			m_ahRenaMinorParticleInstance[eVal_] = hHandle_;
		}
		void				DeleteLireMinorParticle();

		virtual void		DeleteMinorParticle();

#ifdef SKILL_30_TEST

		bool MakeRoomForEntangleTrap( int iMaxEntangleTrapCount, bool bForce );
		void CreateEntangleTrap();
		void AddEntangleTrap( CX2GUNPC* pEntangle );
		float GetEntangleTrapAttackDuration() const { return m_fEntangleTrapAttackDuration; }

#endif SKILL_30_TEST

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		//엘드랏실의 분노
		bool MakeRoomForAngerOfElfTrap( int iMaxAngerOfElfTrapCount, bool bForce );
		void CreateAngerOfElfTrap();
		void AddAngerOfElfTrap( CX2GUNPC* pAngerOfElfTrap );
		void DeleteAngerOfElfTrapUID( UidType iNPCUID );
		void SetSiegeModeEffectiveValue( IN const int iSkillLevel );		/// 시즈 모드를 통해 증가되는 수치 설정
#endif //UPGRADE_SKILL_SYSTEM_2013

		virtual bool		SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet );
		virtual float		GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

#ifdef ADD_MEMO_1ST_CLASS //김창한
		const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION GetSkillUseCondition(const CX2SkillTree::SkillTemplet* pSkillTemplet_);
#endif //ADD_MEMO_1ST_CLASS

#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.3] // 윈드 스니커 - 자연과의 친화(패시브)
		FriendshipNatureData* GetFriendshipNatureData() { return m_pFriendshipNatureData; }
#endif NEW_SKILL_2010_11

#ifdef SKILL_BALANCE_20110728
		bool IsNatureForce() { return m_bNatureForcePossible; }
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
		void ProcessVPP();

		void SetFatalityData();

		void CreateEvoke( const int MAX_COUNT );
		void InsertEvokeNpc( CX2GameUnit* pGameUnitEvoke_ );
		void DeleteEvokeNPCUID( UidType iNPCUID );
#endif	SERV_TRAPPING_RANGER_TEST

		virtual void UpNowMpPerHitOthers( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ );	// 근접공격 성공시 엠회수 처리

		virtual void HyperModeBuffEffectStart();
		virtual void AddUnitSlashData( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ );
#ifdef RIDING_SYSTEM
		virtual bool CanRidingState();
#endif //RIDING_SYSTEM

		virtual int GetComboZStateID() const { return LESI_COMBO_Z1; }
		virtual int GetComboXStateID() const { return LESI_COMBO_X1; }

#ifdef ADD_RENA_SYSTEM //김창한
		void	ConsumeNaturalForce( int iCount_ );				//자연의 기운 소모
		__forceinline void	UpNaturalForce( int iUpCount_ );	//자연의 기운 획득
		__forceinline bool	IsEmptyNaturalForce();				//자연의 기운이 하나도 없는지 체크
		__forceinline int	GetNaturalForceCount();				//현재 자연의 기운 갯수 반환

		virtual void	SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void	UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
		virtual void	AdjustDamageDataBeforeDamageReact( CX2DamageManager::DamageData* pDamageData );

		//인자로 들어온 값이 m_vecSaveData에 저장된 값 중에 일치하는 것이 있는지 체크하는 함수 
		virtual bool CheckDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ );
		//예외적으로 데미지 이펙트를 2개 이상 생성하는 콤보에 대한 예외처리 2014.01.28
		bool CheckDamageRelateComboData( const CX2DamageManager::DamageRelateSkillData sData_ );
		//m_vecSaveData 값 중에 인자로 들어온 값과 일치하는 것을 삭제하는 함수
		virtual void DeleteDamageRelateSkillData( const CX2DamageManager::DamageRelateSkillData sData_ );

		const float GetNaturalForceChargeValue(){ return m_fNaturalForceValueCharge; }
#endif //ADD_RENA_SYSTEM

	protected:
		virtual void ParseCommonRandomState();
		void				InitComponent();
		void				InitState();

		void InitStateID();
		void InitStateCommon();


		void				InitDevice();

#ifdef ADD_RENA_SYSTEM //김창한
		virtual void			AttackResultByType( CX2DamageManager::DamageData &pDamageData );
		void					SetNaturalForceEffect();	//자연의 기운 이펙트 설정
		void					UpdateNaturalForceEffect();	//자연의 기운 갯수가 달라진것을 체크해서 SetNaturalForceEffect함수 호출

		virtual void Init( bool bUseTeamPos_, int iStartPosIndex_ );
		virtual CX2GageData*	CreateGageData();
		CX2RenaGageData*		GetRenaGageData() { return static_cast<CX2RenaGageData*>( GetGageData() ); }
		const CX2RenaGageData*	GetRenaGageData() const { return static_cast<const CX2RenaGageData*>( GetGageData() ); }

		void	SetNaturalForceCount( const int iNaturalForceCount_ );	//자연의 기운 갯수를 설정
		void	SetNaturalForceBuff( const bool bBuffOn_ );				//자연의 기운 버프를 키고 끄는 함수
		const bool	GetNaturalForceBuff();								//현재 자연의 기운 버프가 적용된 상태인지 반환
		__forceinline bool	IsFullNaturalForce();						//자연의 기운이 최대치인지 체크

		bool	IsHyperActiveSkill( CX2SkillTree::SKILL_ID eSkillId_ );	//하이퍼 액티브인지 체크하는 함수
#endif //ADD_RENA_SYSTEM

		virtual void		InitEquippedActiveSkillState( bool bOnlySkillLevel = false );
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

#ifndef SERV_9TH_NEW_CHARACTER // 김태환
		/// 다른 캐릭터들 전부 똑같은 함수를 쓰고 있으니, X2GUUser로 옮기자.
		virtual void		CreateNotEnoughMPEffect( D3DXVECTOR3 vPos, float fDegreeX, float fDegreeY, float fDegreeZ );
#endif // SERV_9TH_NEW_CHARACTER

#ifdef NEW_SKILL_2010_11
		// oasis907 : 김상윤 [2010.11.4] // 윈드 스니커 - 자연과의 친화(패시브)
		void				ProcessFriendshipNature();
		void				ReInitFriendshipNature();
#endif NEW_SKILL_2010_11

#ifdef	SERV_TRAPPING_RANGER_TEST
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		void				StateInitTrapBlade();
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		void				CreateTrapBlade();
		void				DeleteTrapBlade();

		bool				IsUseTrapBalde()	{ return m_bUseTrapBlade; }
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef MODIFY_SET_DAMAGE_DATA
		virtual	void				SetDamageExceptionProcess( CX2DamageManager::DamageData* pDamageData_ ,OUT float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA
		
#ifdef BALANCE_GRAND_ARCHER_20121213
		void CreateGuideArrow( bool bMemo_, bool bHyperMode_ = false);
#endif //BALANCE_GRAND_ARCHER_20121213

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
		void SetInvisibility ( bool bVa_ );
		virtual void		ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템

		bool											m_bDownForce;
		float											m_fChangeMPTime;
		bool											m_bReAttackZ1Right;
		float											m_fReAttackZ1Time;

		int												m_JumpXCount;
		float											m_fJumpXTime;




		float m_fArrowRangeRate;				// 화살이 날아가는 거리 비율
		float m_fMPConsumeRateSiegeArrow;		// 시즈모드에서 화살 마나소모 비율~
		float m_fDamageRelSiegeArrow;			// 시즈모드 파워 업그레이드.

		float m_fAttackPowerRateArrow;
		int	  m_iAddHitCountArrow;		
		float m_fHitGapArrow;
		int	  m_nBackShotCount;					// 백덤블링샷 화살수
		float m_fHitAddMp;						// 관통화살 마나 회수량설정

#ifdef SKILL_30_TEST
		float m_fEntangleTrapAttackDuration;			// 인탱글 트랩이 유닛을 잡고 있는 시간	
		int m_MaxEntangleTrapCount;
		std::vector<UidType> m_vecEntagleTrapUID;

		int m_iDoubleJumpCount;
#endif SKILL_30_TEST
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		//엘드랏실의 분노
		int						m_MaxAngerOfElfTrapCount;
		std::vector<UidType>	m_vecAngerOfElfTrapUID;
		float					m_fSiegeDamageByPassive;	/// 시즈 모드 데미지 증가값
#endif //UPGRADE_SKILL_SYSTEM_2013

		ArrowData m_ArrowData;
		ChargeMPData m_ChargeMPData;

#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.3] // 윈드 스니커 - 자연과의 친화(패시브)
		FriendshipNatureData* m_pFriendshipNatureData;
#endif NEW_SKILL_2010_11

        /** 아래의 CKTDGXSkinAnim*타입의 멤버들은 내부적으로 관리하는 객체이므로
            smart pointer를 사용하지 않는다.
            - jintaeks on 2009-01-12, 16:26 */
		CKTDGXSkinAnim*									m_pMPChargeA;
		CKTDGXSkinAnim*									m_pMPChargeB;
		CKTDGXSkinAnim*									m_pMPChargeC;

		float											m_fMPChargeWaitTime;
		float											m_fMPChargeSoundTime;
		D3DXVECTOR3										m_PerfectStormPos;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMPEnergy;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMPEnergyCenter;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMPChargeWave;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMPChargeFeather;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMPCharge2Up;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMPCharge2UpHigh;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqMPChargePlus;			// SI_P_LGA_COMMUNE_OF_NATURE 스킬 있을 때 차지 이펙트

		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshSpinningKick1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSpinningKick1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSpinningKick2;

#ifndef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 다이브 킥 보밍 관련 소스 삭제 ( 캐릭터 스크립트 내 이펙트 셋으로 이관 )
		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hMeshDiveKickBombing;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	



		CX2EffectSet::Handle	m_hWindSneakerFoot;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle   m_hDESlideKick;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect* m_pDESlideKick;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE



		int m_iArrowMeshPlayerIndex; // 화살 데미지 이펙트에서 어떤 메시 플레이어를 사용할지

#ifdef SKILL_BALANCE_20110728
		bool m_bNatureForcePossible;
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
		FatalityData										m_sFatalityData;

		CKTDGXMeshPlayer::CXMeshInstanceHandle				m_hTrapBlade;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffectHandle							    m_hEffectTrapArrow;
		CX2DamageEffect::CEffectHandle							    m_hEffectCallOfRuin;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*							m_pEffectTrapArrow;
		CX2DamageEffect::CEffect*							m_pEffectCallOfRuin;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		float												m_fDamageDataChangeTime;
#ifdef SERV_RENA_NIGHT_WATCHER
		float												m_fDamageDataChangeTime1;
#endif SERV_RENA_NIGHT_WATCHER
		float												m_fDamageDataChangeTime2;

		bool												m_bUseTrapBlade;

		std::vector<CX2GameUnitoPtr>						m_vecOptrEvoke;
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef BALANCE_PATCH_20120329
		bool												m_bIsSiegeSkill;
		char												m_iSiegeAttackCount;
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		bool												m_bAccelDashEventFlag;					/// 일정시간 이상 대시를 할 때, 액셀 대시를 사용할 수 있다.( 임시 )

		bool												m_bUsePriorPlannedBlow;					/// 계산된 일격 케릭 내 적용 여부
		float												m_fPriorPlannedBlowDownValue;			/// 계산된 일격 다운 수치 감소율
		float												m_fPriorPlannedBlowIncreaseDamage;		/// 계산된 일격 증가 데미지 값
		bool												m_bActivePriorPlannedBlow;				/// 계산된 일격 효과 발동 여부
		bool												m_bActivePriorPlannedBlowAttack;		/// 계산된 일격이 적용된 공격 판별
		bool												m_bActivePriorPlannedBlowWalkCancelChange;		
		int													m_iPriorPlannedBlowHitCount;			/// 계산된 일격 콤보 누적 값
		bool												m_bPossibleHitCount;					/// 계산된 일격 콤보 누적 값 연산 가능 여부( 한 State에 여러명을 Hit 시켜도 한번만 Count 되어야 한다. )
		float												m_fPriorPlannedBlowTime;				/// 계산된 일격 콤보 누적 값 초기화 시간( 콤보수가 누적되지 않으면 5초 후 콤보 수 초기화 )
		CX2EffectSet::Handle								m_hPriorPlannedBlowEffect;				/// 계산된 일격 적용시 사용되는 이펙트
		int													m_iNowPriorPlannedBlowLevel;			/// 현재 적용되어 있는 계산된 일격 이펙트 레벨
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		float												m_fPriorPlannedBlowIncreaseHPRate;		/// 계산된 일격 HP 회복량 ( 퍼센티지 )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		bool												m_bActiveStartOfDelayedFiring;			/// 지연의 신호탄 활성 여부
		float												m_fStartOfDelayedFiringIncreaseDamage;	/// 지연의 신호탄 추가 데미지
		float												m_fStartOfDelayedFiringCoolTime;		/// 지연의 신호탄 유지 시간
		CX2EffectSet::Handle								m_hActiveStartOfDelayedFiring;			/// 지연의 신호탄 유지중 확인 이펙트
		float												m_fStartOfDelayedFiringPowerRate;		/// 지연의 신호탄

		bool												m_bShowTrapBladeToWinMotion;			/// 승리시 에렌딜 표시

		int													m_iIndexNum;							/// 지연의 신호탄 데미지 이펙트 인덱스 생성용
#endif SERV_RENA_NIGHT_WATCHER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		RapidShotDataPtr						            m_RapidShotDataPtr;						/// 래피드 샷 스킬 데이터
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffectHandle							    m_hEffectTrapArrowFungus;				/// 트래핑 애로우 - 펑거스
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect*							m_pEffectTrapArrowFungus;				/// 트래핑 애로우 - 펑거스
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		bool												m_bAbleAgileMovement;					/// 재빠른 몸놀림을 배웠는가
#endif //UPGRADE_SKILL_SYSTEM_2013

	private:

#ifdef ADD_MEMO_1ST_CLASS //김창한
		wstring													m_wstrShootingMagumRandomArrow; //메모 작업. 슈팅 매그넘 랜덤 화살 이펙트 이름.
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_RENA_SYSTEM //김창한
		KProtectedType<float>									m_fNaturalForceValueCharge;	//자연의 기운 충전 값(충전값이 5가 되면 자연의 기운 하나 생성)

		CX2EffectSet::Handle									m_hNaturalForceEffect;		//자연의 기운 이펙트 핸들
		int														m_iBeforeNaturalForceCount;	//이펙트 변경을 위하여, 이전 NF값 저장

		CX2SkillTree::SKILL_ID									m_eSaveStateSkillId;	//여러 state로 이루어진 스킬인지 확인을 위해 skillid 저장
#endif //ADD_RENA_SYSTEM

		CKTDGParticleSystem::CParticleEventSequenceHandle		m_ahRenaMajorParticleInstance[RENA_MAJOR_PII_END];
		CKTDGParticleSystem::CParticleEventSequenceHandle		m_ahRenaMinorParticleInstance[RENA_MINOR_PII_END];

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
		float		m_fSpiralStrikeRemainingTime;
		float		m_fPreRenderParamColorA;		/// 렌더 끄기 전 임시 저장하는 기존 알파값		
		float		m_fRenderParamColorA;			/// 렌더 끌 때 페이드 아웃 시키기 위한 알파값
		CX2EffectSet::Handle		m_hSpiralStrikeMiddleEffect;			// 스파이럴 스트라이크, 미들 이펙트 핸들
		CX2EffectSet::Handle		m_hSpiralStrikeMiddleLightEffect;		// 스파이럴 스트라이크, 미들 이펙트 핸들
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        vector<CX2DamageEffect::CEffectHandle>				m_vecCryotronBoltDamageEffect;				// 크레이오트론 볼트, 타켓 저장용
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		vector<CX2DamageEffect::CEffect*>				m_vecCryotronBoltDamageEffect;				// 크레이오트론 볼트, 타켓 저장용
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//							스테이트 함수 시작

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

	public:
		//LESI_DIE_FRONT
		void DieFrontStartFuture();
		void DieFrontStart();
		void DieFrontFrameMove();
		void DieFrontEventProcess();

		//LESI_DIE_BACK
		void DieBackStartFuture();
		void DieBackStart();
		void DieBackFrameMove();
		void DieBackEventProcess();

		//LESI_WAIT
		void WaitStartFuture();
		void WaitStart();
		void WaitFrameMoveFuture();
		void WaitEventProcess();

		//LESI_WALK
		void WalkEventProcess();
#ifdef ADD_RENA_SYSTEM //김창한
		void WalkStart();
#endif //ADD_RENA_SYSTEM

		//LESI_JUMP_READY
		void JumpReadyEventProcess();

		//LESI_JUMP_UP
		void JumpUpEventProcess();

		//LESI_JUMP_DOWN
		void JumpDownEventProcess();

		//LESI_JUMP_LANDING
		void JumpLandingEventProcess();

		//LESI_DASH
#ifdef SERV_RENA_NIGHT_WATCHER
		void LESI_DASH_StateStartFuture();
		void LESI_DASH_FrameMoveFuture();
#endif SERV_RENA_NIGHT_WATCHER
		void DashEventProcess();
#ifdef ADD_RENA_SYSTEM //김창한
		void DashStart();
#endif //ADD_RENA_SYSTEM

		//LESI_DASH_END
		void DashEndStartFuture();
		void DashEndStart();
		void DashEndFrameMoveFuture();
		void DashEndFrameMove();
		void DashEndEventProcess();

		//LESI_DASH_JUMP
		void DashJumpStartFuture();
		void DashJumpStart();
		void DashJumpFrameMoveFuture();
		void DashJumpEventProcess();
		void DashJumpEndFuture();

		//LESI_DASH_JUMP_LANDING
		void DashJumpLandingEventProcess();

		//LESI_DOUBLE_JUMP
		void DoubleJumpStartFuture();
		void DoubleJumpFrameMoveFuture();
		void DoubleJumpEventProcess();

		//LESI_DASH_DOUBLE_JUMP
		void DashDoubleJumpStartFuture();
		void DashDoubleJumpFrameMoveFuture();
		void DashDoubleJumpEventProcess();

		//LESI_DOUBLE_JUMP_ATTACK_Z
		void DoubleJumpAttackZStartFuture();
		void DoubleJumpAttackZFrameMove();
		void DoubleJumpAttackZEventProcess();

		//LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z
		void LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_EventProcess();

		//LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP
		void LESI_COMBAT_RANGER_LESI_DOUBLE_JUMP_ATTACK_Z_UP_EventProcess();

		//LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X
		void LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_FrameMoveFuture();
		void LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_FrameMove();
		void LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_X_EventProcess();
#ifdef ADD_RENA_SYSTEM //김창한
		void LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_StateStart();
		void LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_FrameMove();
		void LESI_SNIPING_RANGER_DOUBLE_JUMP_ATTACK_XX_EventProcess();
#endif //ADD_RENA_SYSTEM

		//LESI_DOUBLE_JUMP_ATTACK_Z_LANDING
		void DoubleJumpAttackZLandingStartFuture();
		void DoubleJumpAttackZLandingStart();
		void DoubleJumpAttackZLandingEventProcess();

		//LESI_CHARGE_MP
		void ChargeMPStartFuture();
		void ChargeMPStart();
		void ChargeMPFrameMove();
		void ChargeMPEventProcess();
		void ChargeMPEndFuture();
		void ChargeMPEnd();

		//LESI_HYPER_MODE
		void HyperModeInit();
		void HyperModeFrameMove();
#ifdef MODIFY_RIDING_PET_AWAKE
		void RidingHyperModeInit();
		void RidingHyperModeFrameMove();
		void CommonHyperModeFrameMove(float fTime1_, float fTime2_, bool bSound_ = false);
#endif // MODIFY_RIDING_PET_AWAKE
		
		////LESI_DAMAGE_GROGGY
		//void LESI_DAMAGE_GROGGY_FrameMoveFuture();
		//void LESI_DAMAGE_GROGGY_EventProcess();

		//LESI_DAMAGE_SMALL_FRONT
		void DamageSmallFrontEventProcess();

		//LESI_DAMAGE_SMALL_BACK
		void DamageSmallBackEventProcess();

		//LESI_DAMAGE_BIG_FRONT
		void DamageBigFrontStart();
		void DamageBigFrontEventProcess();

		//LESI_DAMAGE_BIG_BACK
		void DamageBigBackStart();
		void DamageBigBackEventProcess();

		//LESI_DAMAGE_DOWN_FRONT
		void DamageDownFrontFrameMove();
		void DamageDownFrontEventProcess();

		//LESI_DAMAGE_DOWN_BACK
		void DamageDownBackFrameMove();
		void DamageDownBackEventProcess();

		//LESI_DAMAGE_AIR_DOWN_LANDING
		void DamageAirDownLandingInit();
		void DamageAirDownLandingEventProcess();

		//LESI_STANDUP_ROLLING_FRONT_FRONT
		void StandUpRollingFrontFrontEventProcess();
		void StandUpRollingFrontFrontEndFuture();
		void StandUpRollingFrontFrontEnd();

		//LESI_STANDUP_ROLLING_FRONT_BACK
		void StandUpRollingFrontBackEventProcess();
		void StandUpRollingFrontBackEndFuture();
		void StandUpRollingFrontBackEnd();

		//LESI_STANDUP_ROLLING_BACK_FRONT
		void StandUpRollingBackFrontEventProcess();
		void StandUpRollingBackFrontEndFuture();
		void StandUpRollingBackFrontEnd();

		//LESI_STANDUP_ROLLING_BACK_BACK
		void StandUpRollingBackBackEventProcess();
		void StandUpRollingBackBackEndFuture();
		void StandUpRollingBackBackEnd();

		//LESI_STANDUP_ATTACK_FRONT
		void StandUpAttackFrontFrameMove();
		void StandUpAttackFrontEventProcess();
		void StandUpAttackFrontEnd();

		//LESI_STANDUP_ATTACK_BACK
		void StandUpAttackBackFrameMove();
		void StandUpAttackBackEventProcess();
		void StandUpAttackBackEnd();

		//LESI_SIEGE_READY
		void SiegeReadyStateStartFuture();
		void SiegeReadyStateStart();
		void SiegeReadyEventProcess();

		//LESI_SIEGE_WAIT
		void SiegeWaitStateStart();
		void SiegeWaitFrameMove();
		void SiegeWaitEventProcess();

		//LESI_SIEGE_ATTACK
		void SiegeAttackFrontStateStartFuture();
		void SiegeAttackFrontStateStart();
		void SiegeAttackFrontFrameMove();
		void SiegeAttackEventProcess();

		//LESI_SIEGE_ATTACK_X
		void SiegeAttackXFrontStateStartFuture();
		void SiegeAttackXFrontStateStart();
		void SiegeAttackXFrontFrameMove();
		void SiegeAttackXEventProcess();

		//LESI_SIEGE_END
		void SiegeEndEventProcess();

		//LESI_COMBO_Z1
		void ComboZ1FrameMoveFuture();
		void ComboZ1FrameMove();
		void ComboZ1EventProcess();

		//LESI_COMBO_Z2
		void ComboZ2FrameMoveFuture();
		void ComboZ2FrameMove();
		void ComboZ2EventProcess();

		//LESI_COMBO_Z3
		void ComboZ3FrameMoveFuture();
		void ComboZ3FrameMove();
		void ComboZ3EventProcess();

		//LESI_COMBO_Z4
		void ComboZ4FrameMoveFuture();
		void ComboZ4FrameMove();
		void ComboZ4EventProcess();

		//LESI_COMBO_Z4a
		//void ComboZ4aFrameMoveFuture();
		void ComboZ4aFrameMove();
		void ComboZ4aEventProcess();
		//void ComboZ4aEndFuture();

		//LESI_COMBO_Z4UP
		void ComboZ4UpFrameMoveFuture();
		void ComboZ4UpFrameMove();
		void ComboZ4UpEventProcess();

		//LESI_COMBO_X1
		void ComboX1FrameMoveFuture();
		void ComboX1FrameMove();
		void ComboX1EventProcess();

		//LESI_COMBO_X2
		void ComboX2FrameMoveFuture();
		void ComboX2FrameMove();
		void ComboX2EventProcess();

		//LESI_COMBO_X3
		void ComboX3FrameMoveFuture();
		void ComboX3FrameMove();
		void ComboX3EventProcess();

		//LESI_COMBO_X4
		void ComboX4FrameMoveFuture();
		void ComboX4FrameMove();
		void ComboX4EventProcess();
		
		//LESI_COMBO_X4UP
		void ComboX4UpFrameMoveFuture();
		void ComboX4UpFrameMove();
		void ComboX4UpEventProcess();

		//LESI_COMBO_X4DOWN
		void ComboX4DownFrameMoveFuture();
		void ComboX4DownFrameMove();
		void ComboX4DownEventProcess();

		//LESI_DASH_COMBO_Z1
		void DashComboZ1StartFuture();
		void DashComboZ1FrameMove();
		void DashComboZ1EventProcess();

		//LESI_COMBAT_RANGER_DASH_COMBO_Z1
		void LESI_COMBAT_RANGER_DASH_COMBO_Z1_StartFuture();
		void LESI_COMBAT_RANGER_DASH_COMBO_Z1_EventProcess();

		//LESI_COMBAT_RANGER_DASH_COMBO_Z2
		void LESI_COMBAT_RANGER_DASH_COMBO_Z2_StartFuture();
		void LESI_COMBAT_RANGER_DASH_COMBO_Z2_EventProcess();

		//LESI_DASH_COMBO_Z2
		void DashComboZ2StartFuture();
		void DashComboZ2FrameMove();
		void DashComboZ2EventProcess();

		//LESI_DASH_ATTACK_X
		void DashAttackXStartFuture();
		void DashAttackXFrameMove();
		void DashAttackXEventProcess();
		void DashAttackXEndFuture();

		//LESI_SNIPING_RANGER_DASH_COMBO_X1
		void LESI_SNIPING_RANGER_DASH_COMBO_X1_StartFuture();
		void LESI_SNIPING_RANGER_DASH_COMBO_X1_FrameMove();
		void LESI_SNIPING_RANGER_DASH_COMBO_X1_EventProcess();
		void LESI_SNIPING_RANGER_DASH_COMBO_X1_EndFuture();

		//LESI_SNIPING_RANGER_DASH_COMBO_X2
		void LESI_SNIPING_RANGER_DASH_COMBO_X2_FrameMove();
		void LESI_SNIPING_RANGER_DASH_COMBO_X2_EventProcess();
#ifndef BALANCE_GRAND_ARCHER_20121213
		//LESI_SNIPING_RANGER_DASH_COMBO_X3
		void LESI_SNIPING_RANGER_DASH_COMBO_X3_FrameMove();
		void LESI_SNIPING_RANGER_DASH_COMBO_X3_EventProcess();

		//LESI_SNIPING_RANGER_DASH_COMBO_X3_UP
		void LESI_SNIPING_RANGER_DASH_COMBO_X3_UP_FrameMove();
		void LESI_SNIPING_RANGER_DASH_COMBO_X3_UP_EventProcess();

		//LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN
		void LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN_FrameMove();
		void LESI_SNIPING_RANGER_DASH_COMBO_X3_DOWN_EventProcess();
#endif //BALANCE_GRAND_ARCHER_20121213
		//LESI_JUMP_ATTACK_Z
		void JumpAttackZFrameMoveFuture();
		void JumpAttackZFrameMove();
		void JumpAttackZEventProcess();

		//LESI_JUMP_ATTACK_X
		void JumpAttackXStartFuture();
		void JumpAttackXFrameMoveFuture();
		void JumpAttackXFrameMove();
		void JumpAttackXEventProcess();

		//LESI_DASH_JUMP_ATTACK_Z
		//void DashJumpAttackZStartFuture();
		//void DashJumpAttackZFrameMoveFuture();
		//void DashJumpAttackZFrameMove();
		void DashJumpAttackZEventProcess();

		//LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z1
		void LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z1_EventProcess();
		//LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2
		void LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z2_EventProcess();
		//LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3
		void LESI_COMBAT_RANGER_DASH_JUMP_COMBO_Z3_EventProcess();

		//LESI_DASH_JUMP_ATTACK_Z
		void DashJumpAttackZLandingStartFuture();
		void DashJumpAttackZLandingStart();
		void DashJumpAttackZLandingEventProcess();

		//LESI_DASH_JUMP_COMBO_X1
		void DashJumpComboX1FrameMoveFuture();
		void DashJumpComboX1FrameMove();
		void DashJumpComboX1EventProcess();

		//LESI_DASH_JUMP_COMBO_X2
		void DashJumpComboX2FrameMoveFuture();
		void DashJumpComboX2FrameMove();
		void DashJumpComboX2EventProcess();

		//LESI_DASH_JUMP_COMBO_X3
#ifdef ADD_RENA_SYSTEM // 김태환
		void DashJumpComboX3StateStart();
#endif // ADD_RENA_SYSTEM
		void DashJumpComboX3FrameMoveFuture();
		void DashJumpComboX3FrameMove();
		void DashJumpComboX3EventProcess();

		//LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2
		void LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_FrameMoveFuture();
		void LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_FrameMove();
		void LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X2_EventProcess();

		//LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3
		void LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_FrameMoveFuture();
		void LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_FrameMove();
		void LESI_SNIPING_RANGER_DASH_JUMP_COMBO_X3_EventProcess();

		void ShowSiegeFailGuide();


		////////////////////////////////////////////////////////////////////////
		//LESI_SI_LE_POWER_ATTACK
		void LESI_SI_LE_POWER_ATTACK_FrameMoveFuture();
		void LESI_SI_LE_POWER_ATTACK_FrameMove();
		void LESI_SI_LE_POWER_ATTACK_EventProcess();

		//LESI_SI_LE_POWER_ATTACK_HYPER
		void LESI_SI_LE_POWER_ATTACK_HYPER_FrameMoveFuture();
		void LESI_SI_LE_POWER_ATTACK_HYPER_FrameMove();
		void LESI_SI_LE_POWER_ATTACK_HYPER_EventProcess();

		//LESI_SI1_LE_PIERCE_ARROW
		void LESI_SI1_LE_PIERCE_ARROW_Init();
		void LESI_SI1_LE_PIERCE_ARROW_FrameMoveFuture();
		void LESI_SI1_LE_PIERCE_ARROW_FrameMove();
		void LESI_SI1_LE_PIERCE_ARROW_CameraMove();
		void LESI_SI1_LE_PIERCE_ARROW_EventProcess();

		//LESI_SI1_LE_PIERCE_ARROW_HYPER
		void LESI_SI1_LE_PIERCE_ARROW_HYPER_Init();
		void LESI_SI1_LE_PIERCE_ARROW_HYPER_FrameMoveFuture();
		void LESI_SI1_LE_PIERCE_ARROW_HYPER_FrameMove();
		void LESI_SI1_LE_PIERCE_ARROW_HYPER_CameraMove();
		void LESI_SI1_LE_PIERCE_ARROW_HYPER_EventProcess();

		//LESI_SI2_LE_DIVISION_BLAST
		void LESI_SI2_LE_DIVISION_BLAST_Init();
		void LESI_SI2_LE_DIVISION_BLAST_Start();
		void LESI_SI2_LE_DIVISION_BLAST_FrameMoveFuture();
		void LESI_SI2_LE_DIVISION_BLAST_FrameMove();
		void LESI_SI2_LE_DIVISION_BLAST_CameraMove();
		void LESI_SI2_LE_DIVISION_BLAST_EventProcess();

		//LESI_SI2_LE_DIVISION_BLAST_HYPER
		void LESI_SI2_LE_DIVISION_BLAST_HYPER_Init();
		void LESI_SI2_LE_DIVISION_BLAST_HYPER_Start();
		void LESI_SI2_LE_DIVISION_BLAST_HYPER_FrameMoveFuture();
		void LESI_SI2_LE_DIVISION_BLAST_HYPER_FrameMove();
		void LESI_SI2_LE_DIVISION_BLAST_HYPER_CameraMove();
		void LESI_SI2_LE_DIVISION_BLAST_HYPER_EventProcess();

		//LESI_SI3_LE_PHOENIX_STRIKE
		void LESI_SI3_LE_PHOENIX_STRIKE_Init();
		void LESI_SI3_LE_PHOENIX_STRIKE_FrameMoveFuture();
		void LESI_SI3_LE_PHOENIX_STRIKE_FrameMove();
		void LESI_SI3_LE_PHOENIX_STRIKE_CameraMove();
		void LESI_SI3_LE_PHOENIX_STRIKE_EventProcess();

		//LESI_SI3_LE_PHOENIX_STRIKE_HYPER
		void LESI_SI3_LE_PHOENIX_STRIKE_HYPER_Init();
		void LESI_SI3_LE_PHOENIX_STRIKE_HYPER_FrameMoveFuture();
		void LESI_SI3_LE_PHOENIX_STRIKE_HYPER_FrameMove();
		void LESI_SI3_LE_PHOENIX_STRIKE_HYPER_CameraMove();
		void LESI_SI3_LE_PHOENIX_STRIKE_HYPER_EventProcess();


		//LESI_SI1_LE_ASSAULT_KICK
		void LESI_SI1_LE_ASSAULT_KICK_Init();
		void LESI_SI1_LE_ASSAULT_KICK_FrameMoveFuture();
		void LESI_SI1_LE_ASSAULT_KICK_FrameMove();
		void LESI_SI1_LE_ASSAULT_KICK_CameraMove();
		void LESI_SI1_LE_ASSAULT_KICK_EventProcess();

		//LESI_SI1_LE_ASSAULT_KICK_HYPER
		void LESI_SI1_LE_ASSAULT_KICK_HYPER_FrameMoveFuture();
		void LESI_SI1_LE_ASSAULT_KICK_HYPER_FrameMove();
		void LESI_SI1_LE_ASSAULT_KICK_HYPER_CameraMove();
		void LESI_SI1_LE_ASSAULT_KICK_HYPER_EventProcess();

		//LESI_SI1_LE_HUMMING_WIND
		void LESI_SI1_LE_HUMMING_WIND_Init();
		void LESI_SI1_LE_HUMMING_WIND_FrameMoveFuture();
		void LESI_SI1_LE_HUMMING_WIND_FrameMove();
		void LESI_SI1_LE_HUMMING_WIND_CameraMove();
		void LESI_SI1_LE_HUMMING_WIND_EventProcess();

		//LESI_SI1_LE_HUMMING_WIND_HYPER
		void LESI_SI1_LE_HUMMING_WIND_HYPER_FrameMoveFuture();
		void LESI_SI1_LE_HUMMING_WIND_HYPER_FrameMove();
		void LESI_SI1_LE_HUMMING_WIND_HYPER_CameraMove();
		void LESI_SI1_LE_HUMMING_WIND_HYPER_EventProcess();


		//LESI_SI2_LE_GUIDED_ARROW
		void LESI_SI2_LE_GUIDED_ARROW_Init();
		void LESI_SI2_LE_GUIDED_ARROW_FrameMoveFuture();
		void LESI_SI2_LE_GUIDED_ARROW_FrameMove();
		void LESI_SI2_LE_GUIDED_ARROW_CameraMove();
		void LESI_SI2_LE_GUIDED_ARROW_EventProcess();

		//LESI_SI2_LE_GUIDED_ARROW_HYPER
		void LESI_SI2_LE_GUIDED_ARROW_HYPER_FrameMoveFuture();
		void LESI_SI2_LE_GUIDED_ARROW_HYPER_FrameMove();
		void LESI_SI2_LE_GUIDED_ARROW_HYPER_CameraMove();
		void LESI_SI2_LE_GUIDED_ARROW_HYPER_EventProcess();


		//LESI_SI2_LE_PERFECT_STORM
		void LESI_SI2_LE_PERFECT_STORM_Init();
		void LESI_SI2_LE_PERFECT_STORM_FrameMoveFuture();
		void LESI_SI2_LE_PERFECT_STORM_FrameMove();
		void LESI_SI2_LE_PERFECT_STORM_CameraMove();
		void LESI_SI2_LE_PERFECT_STORM_EventProcess();

		//LESI_SI2_LE_PERFECT_STORM_HYPER
		void LESI_SI2_LE_PERFECT_STORM_HYPER_FrameMoveFuture();
		void LESI_SI2_LE_PERFECT_STORM_HYPER_FrameMove();
		void LESI_SI2_LE_PERFECT_STORM_HYPER_CameraMove();
		void LESI_SI2_LE_PERFECT_STORM_HYPER_EventProcess();


		//LESI_SI3_LE_AERO_TORNADO
		void LESI_SI3_LE_AERO_TORNADO_Init();
		void LESI_SI3_LE_AERO_TORNADO_FrameMoveFuture();
		void LESI_SI3_LE_AERO_TORNADO_FrameMove();
		void LESI_SI3_LE_AERO_TORNADO_CameraMove();
		void LESI_SI3_LE_AERO_TORNADO_EventProcess();

		//LESI_SI3_LE_AERO_TORNADO_HYPER
		void LESI_SI3_LE_AERO_TORNADO_HYPER_FrameMoveFuture();
		void LESI_SI3_LE_AERO_TORNADO_HYPER_FrameMove();
		void LESI_SI3_LE_AERO_TORNADO_HYPER_CameraMove();
		void LESI_SI3_LE_AERO_TORNADO_HYPER_EventProcess();


		//LESI_SI1_LCR_SPINNING_KICK
		void LESI_SI1_LCR_SPINNING_KICK_Init();
		void LESI_SI1_LCR_SPINNING_KICK_FrameMoveFuture();
		void LESI_SI1_LCR_SPINNING_KICK_FrameMove();
		void LESI_SI1_LCR_SPINNING_KICK_EventProcess();

		//LESI_SI1_LCR_SPINNING_KICK_HYPER
		void LESI_SI1_LCR_SPINNING_KICK_HYPER_FrameMoveFuture();
		void LESI_SI1_LCR_SPINNING_KICK_HYPER_FrameMove();
		void LESI_SI1_LCR_SPINNING_KICK_HYPER_EventProcess();


		//LESI_SI2_LCR_CRESCENT_KICK
		void LESI_SI2_LCR_CRESCENT_KICK_Init();
		void LESI_SI2_LCR_CRESCENT_KICK_FrameMove();
		void LESI_SI2_LCR_CRESCENT_KICK_EventProcess();

		//LESI_SI2_LCR_CRESCENT_KICK_HYPER
		void LESI_SI2_LCR_CRESCENT_KICK_HYPER_FrameMove();
		void LESI_SI2_LCR_CRESCENT_KICK_HYPER_EventProcess();


		//LESI_SI3_LCR_DIVE_KICK_BOMBING
		void LESI_SI3_LCR_DIVE_KICK_BOMBING_Init();
		void LESI_SI3_LCR_DIVE_KICK_BOMBING_FrameMoveFuture();
		void LESI_SI3_LCR_DIVE_KICK_BOMBING_FrameMove();
		void LESI_SI3_LCR_DIVE_KICK_BOMBING_EventProcess();

		//LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER
		void LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_FrameMoveFuture();
		void LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_FrameMove();
		void LESI_SI3_LCR_DIVE_KICK_BOMBING_HYPER_EventProcess();


		//LESI_SI3_LSR_CRAZY_SHOT
		void LESI_SI3_LSR_CRAZY_SHOT_Init();
		void LESI_SI3_LSR_CRAZY_SHOT_FrameMove();
		void LESI_SI3_LSR_CRAZY_SHOT_EventProcess();

		//LESI_SI3_LSR_CRAZY_SHOT_HYPER
		void LESI_SI3_LSR_CRAZY_SHOT_HYPER_FrameMove();
		void LESI_SI3_LSR_CRAZY_SHOT_HYPER_EventProcess();

		//COMMON_BUFF
		virtual void COMMON_BUFF_FrameMove();

		// SI_A_LE_BACKJUMP_SHOT
		void LESI_SI_A_LE_BACKJUMP_SHOT_Init();
		void LESI_SI_A_LE_BACKJUMP_SHOT_Start();
		void LESI_SI_A_LE_BACKJUMP_SHOT_EventProcess();
		void LESI_SI_A_LE_BACKJUMP_SHOT_FrameMove();
		void LESI_SI_A_LE_BACKJUMP_SHOT_FrameMoveFuture();
		void LESI_SI_A_LE_BACKJUMP_SHOT_EndFuture();		

		// LESI_SI_A_LE_REFLEX_MAGIC
		void LESI_SI_A_LE_REFLEX_MAGIC_READY_EventProcess();

		void LESI_SI_A_LE_REFLEX_MAGIC_Init();
		void LESI_SI_A_LE_REFLEX_MAGIC_Start();		
		void LESI_SI_A_LE_REFLEX_MAGIC_EventProcess();		

#ifdef SKILL_30_TEST
		void LESI_SA_LCR_SHARPFALL_Init();
		void LESI_SA_LCR_SHARPFALL_FrameMove();
		void LESI_SA_LCR_SHARPFALL_EventProcess();
		void LESI_SA_LCR_SHARPFALL_HYPER_FrameMove();
		void LESI_SA_LCR_SHARPFALL_HYPER_EventProcess();


		void LESI_SA_LCR_SHARPFALL_LANDING_FrameMove();	
		void LESI_SA_LCR_SHARPFALL_LANDING_EventProcess();	


		void LESI_SA_LSR_WINDWARD_Init();
		void LESI_SA_LSR_WINDWARD_StateStart();
		void LESI_SA_LSR_WINDWARD_FrameMove();
		void LESI_SA_LSR_WINDWARD_EventProcess();
		void LESI_SA_LSR_WINDWARD_StateEnd();


		void LESI_SA_LSR_WINDWARD_HYPER_StateStart();
		void LESI_SA_LSR_WINDWARD_HYPER_FrameMove();
		void LESI_SA_LSR_WINDWARD_HYPER_EventProcess();
		void LESI_SA_LSR_WINDWARD_HYPER_StateEnd();
	
		
		void LESI_SA_LSR_ENTANGLE_Init();
		void LESI_SA_LSR_ENTANGLE_StateStart();
		void LESI_SA_LSR_ENTANGLE_StateStartFuture();
		void LESI_SA_LSR_ENTANGLE_FrameMove();
		void LESI_SA_LSR_ENTANGLE_EventProcess();
		void LESI_SA_LSR_ENTANGLE_StateEnd();

		void LESI_SA_LCR_LOW_KICK_Init();
#ifndef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 로우킥 메모가 미들킥으로 이전되면서, Start 스테이트 제거
		void LESI_SA_LCR_LOW_KICK_StateStart();
#endif // UPGRADE_SKILL_SYSTEM_2013 
		void LESI_SA_LCR_LOW_KICK_FrameMove();
		void LESI_SA_LCR_LOW_KICK_EventProcess();
#endif SKILL_30_TEST

#ifdef SKILL_CASH_10_TEST
		void LESI_SA_LCR_VIOLENT_ATTACK_READY_Init();
		void LESI_SA_LCR_VIOLENT_ATTACK_READY_StateStart();
		void LESI_SA_LCR_VIOLENT_ATTACK_READY_FrameMove();
		void LESI_SA_LCR_VIOLENT_ATTACK_READY_Hyper_FrameMove();
		void LESI_SA_LCR_VIOLENT_ATTACK_READY_FrameMoveFuture();
		void LESI_SA_LCR_VIOLENT_ATTACK_READY_EventProcess();
		void LESI_SA_LCR_VIOLENT_ATTACK_READY_StateEnd();
		
		void LESI_SA_LCR_VIOLENT_ATTACK_Init();
		void LESI_SA_LCR_VIOLENT_ATTACK_FrameMove();
		void LESI_SA_LCR_VIOLENT_ATTACK_EventProcess();
		void LESI_SA_LCR_VIOLENT_ATTACK_StateEnd();

		void LESI_SA_LCR_VIOLENT_ATTACK_LANDING_Init();
		void LESI_SA_LCR_VIOLENT_ATTACK_LANDING_FrameMove();
		void LESI_SA_LCR_VIOLENT_ATTACK_LANDING_EventProcess();
		void LESI_SA_LCR_VIOLENT_ATTACK_LANDING_Hyper_EventProcess();
		void LESI_SA_LCR_VIOLENT_ATTACK_LANDING_StateEnd();
		
		//{{ kimhc // 2009-09-19 // 궁그닐 스킬
		void LESI_SA_LSR_GUNGNIR_Init();
		void LESI_SA_LSR_GUNGNIR_StateStart();
		void LESI_SA_LSR_GUNGNIR_FrameMove();
		void LESI_SA_LSR_GUNGNIR_EventProcess();
		void LESI_SA_LSR_GUNGNIR_StateEnd();

		void LESI_SA_LSR_GUNGNIR_HYPER_StateStart();
		void LESI_SA_LSR_GUNGNIR_HYPER_FrameMove();
		void LESI_SA_LSR_GUNGNIR_HYPER_EventProcess();
		void LESI_SA_LSR_GUNGNIR_HYPER_StateEnd();
#endif








//#ifdef RENA_SECOND_CLASS_CHANGE


		void LESI_LWS_ZZfrontZ_FrameMoveFuture();
		void LESI_LWS_ZZfrontZ_EventProcess();
		
		
		void LESI_LWS_ZZfrontZZ_FrameMoveFuture();
		void LESI_LWS_ZZfrontZZ_EventProcess();

		void LESI_LWS_ZZfrontZZZ_FrameMoveFuture();
		void LESI_LWS_ZZfrontZZZ_EventProcess();
		void LESI_LWS_ZZfrontZZZ_Landing_EventProcess();

		void LESI_LWS_DASH_X_EventProcess();

		//void LESI_LWS_DASH_XZ_FrameMoveFuture();
		void LESI_LWS_DASH_XZ_EventProcess();
		void LESI_LWS_DASH_XZZ_EventProcess();

#ifdef BALANCE_GRAND_ARCHER_20121213
		void LESI_LGA_ZZX_FrameMove();
		void LESI_LGA_ZZX_EventProcess();

#ifdef ADD_RENA_SYSTEM //김창한
		void LESI_LGA_ZZXX_StateStart();
#endif //ADD_RENA_SYSTEM
		void LESI_LGA_ZZXX_FrameMove();
		void LESI_LGA_ZZXX_EventProcess();
#else
		void LESI_LGA_ZZZX_FrameMove();
		void LESI_LGA_ZZZX_EventProcess();

		void LESI_LGA_ZZZXX_FrameMove();
		void LESI_LGA_ZZZXX_EventProcess();
#endif//BALANCE_GRAND_ARCHER_20121213

		void LESI_LGA_DASH_JUMP_Z_EventProcess();
		void LESI_LGA_DASH_JUMP_ZX_FrameMoveFuture();
		void LESI_LGA_DASH_JUMP_ZX_FrameMove();
		void LESI_LGA_DASH_JUMP_ZX_EventProcess();
	
		

		void LESI_SA_LWS_SLIDE_KICK_Init();
		void LESI_SA_LWS_SLIDE_KICK_FrameMove();
		void LESI_SA_LWS_SLIDE_KICK_EventProcess();
		//void LESI_SA_LWS_SLIDE_KICK_HYPER_FrameMove();
		//void LESI_SA_LWS_SLIDE_KICK_HYPER_EventProcess();
		void LESI_SA_LWS_SLIDE_KICK_StateEnd();

		void LESI_SA_LWS_SLIDE_KICK_CANCEL_EventProcess();


		//void LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH_FrameMove();
		void LESI_SA_LWS_SLIDE_KICK_PASS_THROUGH_EventProcess();

		//void LESI_SA_LWS_SLIDE_KICK_FINISH_FrameMove();
		//void LESI_SA_LWS_SLIDE_KICK_FINISH_HYPER_FrameMove();



		void LESI_SA_LGA_FREEZING_ARROW_Init();
		void LESI_SA_LGA_FREEZING_ARROW_FrameMove();			
		void LESI_SA_LGA_FREEZING_ARROW_HYPER_FrameMove();		

		void LESI_A_LWS_NATURE_FORCE_Init();
		void LESI_A_LWS_NATURE_FORCE_StateStart();
		void LESI_A_LWS_NATURE_FORCE_HYPER_StateStart();

		void LESI_A_LGA_STIGMA_ARROW_Init();
		void LESI_A_LGA_STIGMA_ARROW_FrameMove();


#ifdef NEW_SKILL_2010_11

		//LESI_SI3_LE_AERO_STRAFE
		void LESI_SA_LGA_AERO_STRAFE_Init();
		void LESI_SA_LGA_AERO_STRAFE_FrameMoveFuture();
		void LESI_SA_LGA_AERO_STRAFE_FrameMove();
		void LESI_SA_LGA_AERO_STRAFE_CameraMove();
		void LESI_SA_LGA_AERO_STRAFE_EventProcess();
		void LESI_SA_LGA_AERO_STRAFE_StateEndFurtue();

		//LESI_SI3_LE_AERO_STRAFE_HYPER
		void LESI_SA_LGA_AERO_STRAFE_HYPER_Init();
		void LESI_SA_LGA_AERO_STRAFE_HYPER_FrameMoveFuture();
		void LESI_SA_LGA_AERO_STRAFE_HYPER_FrameMove();
		void LESI_SA_LGA_AERO_STRAFE_HYPER_CameraMove();
		void LESI_SA_LGA_AERO_STRAFE_HYPER_EventProcess();

#endif NEW_SKILL_2010_11
//#endif RENA_SECOND_CLASS_CHANGE

#ifdef NEW_SKILL_2010_11
		//{{ oasis907 : 김상윤 [2010.11.5] // 윈드 스니커 - 아이레린나
		void LESI_SA_LWS_AIRELINNA_Init();
		void LESI_SA_LWS_AIRELINNA_FrameMove();

		//}}
#endif NEW_SKILL_2010_11

#ifdef	SERV_TRAPPING_RANGER_TEST
		//LESI_SA_RTR_EVOKE
		void LESI_SA_RTR_EVOKE_StateStart();
		void LESI_SA_RTR_EVOKE_Init();
		void LESI_SA_RTR_EVOKE_FrameMove();
		void LESI_SA_RTR_EVOKE_StateEnd();
		void LESI_SA_RTR_EVOKE_HYPER_FrameMove();

		//LESI_A_RTR_EXPLOSION_TRAP
		void LESI_A_RTR_EXPLOSION_TRAP_Init();
		void LESI_A_RTR_EXPLOSION_TRAP_StartFuture();
		void LESI_A_RTR_EXPLOSION_TRAP_Start();
		void LESI_A_RTR_EXPLOSION_TRAP_FrameMoveFuture();
		void LESI_A_RTR_EXPLOSION_TRAP_FrameMove();
		void LESI_A_RTR_EXPLOSION_TRAP_EventProcess();
		void LESI_A_RTR_EXPLOSION_TRAP_EndFuture();

		//LESI_SA_RTR_TRAPING_ARROW
		void LESI_SA_RTR_TRAPING_ARROW_StateStart();
		void LESI_SA_RTR_TRAPING_ARROW_FrameMoveFuture();
		void LESI_SA_RTR_TRAPING_ARROW_FrameMove();
		void LESI_SA_RTR_TRAPING_ARROW_HYPER_FrameMove();
		void LESI_SA_RTR_TRAPING_ARROW_EventProcess();
		void LESI_SA_RTR_TRAPING_ARROW_StateEnd();

		//LESI_SA_RTR_KARMA
		void LESI_SA_RTR_KARMA_Init();
		void LESI_SA_RTR_KARMA_StateStart();
		void LESI_SA_RTR_KARMA_FrameMove();
		void LESI_SA_RTR_KARMA_HYPER_FrameMove();
		void LESI_SA_RTR_KARMA_EventProcess();
		void LESI_SA_RTR_KARMA_StateEnd();

		//LESI_SA_RTR_FATALITY
		void LESI_SA_RTR_FATALITY_Init();
		void LESI_SA_RTR_FATALITY_StateStart();
		void LESI_SA_RTR_FATALITY_StateStartFuture();
		void LESI_SA_RTR_FATALITY_FrameMoveFuture();
		void LESI_SA_RTR_FATALITY_FrameMove();
		void LESI_SA_RTR_FATALITY_HYPER_FrameMove();
		void LESI_SA_RTR_FATALITY_EventProcess();
		void LESI_SA_RTR_FATALITY_StateEnd();
		void LESI_SA_RTR_FATALITY_StateEndFuture();

		//LESI_SA_RTR_CALL_OF_RUIN
		void LESI_SA_RTR_CALL_OF_RUIN_Init();
		void LESI_SA_RTR_CALL_OF_RUIN_StateStart();
		void LESI_SA_RTR_CALL_OF_RUIN_FrameMoveFuture();
		void LESI_SA_RTR_CALL_OF_RUIN_FrameMove();
		void LESI_SA_RTR_CALL_OF_RUIN_HYPER_FrameMove();
		void LESI_SA_RTR_CALL_OF_RUIN_EventProcess();
		void LESI_SA_RTR_CALL_OF_RUIN_StateEnd();

		//LESI_A_LE_RISING_FALCON
		void LESI_A_LE_RISING_FALCON_Init();
		void LESI_A_LE_RISING_FALCON_FrameMoveFuture();
		void LESI_A_LE_RISING_FALCON_FrameMove();
		void LESI_A_LE_RISING_FALCON_EventProcess();


		void LESI_COMBO_RTR_ZZZ_StateStart();
		void LESI_COMBO_RTR_ZZZ_FrameMove();
		void LESI_COMBO_RTR_ZZZ_EventProcess();
		void LESI_COMBO_RTR_ZZZ_StateEnd();

		void LESI_COMBO_RTR_ZZZ_DZ_StateStart();
		void LESI_COMBO_RTR_ZZZ_DZ_FrameMove();
		void LESI_COMBO_RTR_ZZZ_DZ_EventProcess();
		void LESI_COMBO_RTR_ZZZ_DZ_StateEnd();

		void LESI_COMBO_RTR_ZZZZ_StateStart();
		void LESI_COMBO_RTR_ZZZZ_FrameMove();
		void LESI_COMBO_RTR_ZZZZ_EventProcess();
		void LESI_COMBO_RTR_ZZZZ_StateEnd();

		void LESI_COMBO_RTR_ZZZZZ_StateStart();
		void LESI_COMBO_RTR_ZZZZZ_FrameMove();
		void LESI_COMBO_RTR_ZZZZZ_EventProcess();
		void LESI_COMBO_RTR_ZZZZZ_StateEnd();

		void LESI_JUMP_ATTACK_RTR_Z_StateStart();
		void LESI_JUMP_ATTACK_RTR_Z_FrameMove();
		void LESI_JUMP_ATTACK_RTR_Z_FrameMoveFuture();
		void LESI_JUMP_ATTACK_RTR_Z_EventProcess();
		void LESI_JUMP_ATTACK_RTR_Z_StateEnd();

		void LESI_DASH_COMBO_RTR_ZX_StateStart();
		void LESI_DASH_COMBO_RTR_ZX_FrameMove();
		void LESI_DASH_COMBO_RTR_ZX_EventProcess();
		void LESI_DASH_COMBO_RTR_ZX_StateEnd();
		void LESI_DASH_COMBO_RTR_ZX_StateEndFuture();

		void LESI_DASH_JUMP_ATTACK_RTR_Z_StateStart();
		void LESI_DASH_JUMP_ATTACK_RTR_Z_FrameMove();
		void LESI_DASH_JUMP_ATTACK_RTR_Z_EventProcess();
		void LESI_DASH_JUMP_ATTACK_RTR_Z_StateEnd();

		void LESI_DASH_JUMP_ATTACK_RTR_ZZ_StateStart();
		void LESI_DASH_JUMP_ATTACK_RTR_ZZ_FrameMove();
		void LESI_DASH_JUMP_ATTACK_RTR_ZZ_EventProcess();
		void LESI_DASH_JUMP_ATTACK_RTR_ZZ_StateEnd();

		void LESI_SA_RTR_FATALITY_SUCCESS_EventProcess();
		void LESI_SA_RTR_FATALITY_FAIL_EventProcess();

		void LESI_SI_A_LE_RISING_FALCON_LANDING_EventProcess();
#endif	SERV_TRAPPING_RANGER_TEST
		
		void ShowActiveSkillCutInAndLight( const WCHAR* szBoneName_, const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ = false );
		virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

#ifdef BALANCE_PATCH_20120329
		void SetSiegeModeCoolTime();

		//LESI_COMBO_X2
		void SnipingRangerComboX2FrameMove();
		void SnipingRangerComboX2EventProcess();

		//LESI_COMBO_X3
		void SnipingRangerComboX3FrameMoveFuture();
		void SnipingRangerComboX3FrameMove();
		void SnipingRangerComboX3EventProcess();

		//LESI_COMBO_X4
		void SnipingRangerComboX4FrameMove();
		void SnipingRangerComboX4EventProcess();
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		//ZXXXX 콤보
		//LESI_COMBO_RNW_ZZX,
		void LESI_COMBO_RNW_ZZX_StateStart();
		void LESI_COMBO_RNW_ZZX_FrameMove();
		void LESI_COMBO_RNW_ZZX_EventProcess();
		void LESI_COMBO_RNW_ZZX_StateEnd();

		//LESI_COMBO_RNW_ZZXX,
		void LESI_COMBO_RNW_ZZXX_StateStart();
		void LESI_COMBO_RNW_ZZXX_FrameMove();
		void LESI_COMBO_RNW_ZZXX_EventProcess();
		void LESI_COMBO_RNW_ZZXX_StateEnd();

		//LESI_COMBO_RNW_ZZXXX,
		void LESI_COMBO_RNW_ZZXXX_StateStart();
		void LESI_COMBO_RNW_ZZXXX_FrameMove();
		void LESI_COMBO_RNW_ZZXXX_EventProcess();
		void LESI_COMBO_RNW_ZZXXX_StateEnd();

		//순간 가속 콤보
		//LESI_RNW_ACCEL_DASH
		void LESI_RNW_ACCEL_DASH_StateStartFuture();
		void LESI_RNW_ACCEL_DASH_StateStart();
		void LESI_RNW_ACCEL_DASH_FrameMoveFuture();
		void LESI_RNW_ACCEL_DASH_EventProcess();

		//LESI_RNW_ACCEL_DASH_JUMP
		void LESI_RNW_ACCEL_DASH_JUMP_StateStartFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_StateStart();
		void LESI_RNW_ACCEL_DASH_JUMP_FrameMoveFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_StateEndFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_EventProcess();

		//LESI_RNW_ACCEL_DASH_JUMP_LANDING
		void LESI_RNW_ACCEL_DASH_JUMP_LANDING_StateStartFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_LANDING_StateStart();
		void LESI_RNW_ACCEL_DASH_JUMP_LANDING_EventProcess();


		//LESI_RNW_ACCEL_DASH_JUMP_Z
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_StateStartFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_FrameMove();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_FrameMoveFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_EventProcess();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_Z_StateEndFuture();

		//LESI_RNW_ACCEL_DASH_JUMP_ZZ
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_StateStartFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_FrameMove();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_FrameMoveFuture();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_EventProcess();
		void LESI_RNW_ACCEL_DASH_JUMP_ATTACK_ZZ_StateEndFuture();

		//퓨리어스 인게이지
		//LESI_SA_RNW_FURIOUS_ENGAGE
		void LESI_SA_RNW_FURIOUS_ENGAGE_StateStart();
		void LESI_SA_RNW_FURIOUS_ENGAGE_FrameMoveFuture();
		void LESI_SA_RNW_FURIOUS_ENGAGE_FrameMove();
		void LESI_SA_RNW_FURIOUS_ENGAGE_CameraMove();
		void LESI_SA_RNW_FURIOUS_ENGAGE_EventProcess();

		//지연의 신호탄
		//LESI_A_RNW_START_OF_DELAYED_FIRING
		void LESI_A_RNW_START_OF_DELAYED_FIRING_Init();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_StateStart();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_StateStartFuture();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMoveFuture();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_FrameMove();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_StateEnd();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_EventProcess();

		//지연의 신호탄 - 전체 폭파
		//LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE
		void LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_FrameMove();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_StateStart();
		void LESI_A_RNW_START_OF_DELAYED_FIRING_ACTIVE_EventProcess();

		//글라이딩 스트라이크
		//LESI_SA_RNW_GLIDING_STRIKE
		void LESI_SA_RNW_GLIDING_STRIKE_Init();
		void LESI_SA_RNW_GLIDING_STRIKE_StateStartFuture();
		void LESI_SA_RNW_GLIDING_STRIKE_FrameMoveFuture();
		void LESI_SA_RNW_GLIDING_STRIKE_FrameMove();
		void LESI_SA_RNW_GLIDING_STRIKE_StateEndFuture();
		void LESI_SA_RNW_GLIDING_STRIKE_EventProcess();

		void SetPriorPlannedBlow();					/// 계산된 일격 증가 및 연산 함수
		void EndPriorPlannedBlow();					/// 계산된 일격 초기화 및 해제 함수
		void CheckPriorPlannedBlow();				/// 계산된 일격 적용 판단 함수

		void InsertPriorPlannedBlowEffect( int iActiveNum );	/// 계산된 일격 이펙트 생성 함수
		void DeletePriorPlannedBlowEffect();		/// 계산된 일격 이펙트 소멸 함수

		/// 지연의 신호탄 적용 여부 제어
		bool GetActiveStartOfDelayedFiring() const { return m_bActiveStartOfDelayedFiring; }
		void SetActiveStartOfDelayedFiring( bool bActive ) { m_bActiveStartOfDelayedFiring = bActive; }

		/// 지연의 신호탄
		void UpdateStartOfDelayedFiring( bool bInsertUnit = true );																			/// 공격자가 발사한 지연의 신호탄 정보 갱신
		void DeleteStartOfDelayedFiring( vector<CX2DamageManager::StartOfDelayedFiringData*>& vArrowData, int iAttackerIndex, bool bDeleteUnitArrow );	/// 가장 나중에 발사한 화살 삭제 함수
		void SetStartOfDelayedFiringOrderNum( vector<CX2DamageManager::StartOfDelayedFiringData*>& vArrowData );							/// 발사 순서 설정
		bool CreateStartOfDelayedFiringDataVector( OUT vector<CX2DamageManager::StartOfDelayedFiringData*>& pData, int iAttackerIndex );	/// 공격자 인덱스가 적용시킨 모든 화살 객체를 벡터에 넣어서 반환
		bool CreateEntireExplosionDataVector( OUT vector<CX2DamageManager::StartOfDelayedFiringData*>& pData );								/// 전체 폭발 대상 객체 정보 벡터 생성
		void EntireExplosion();																												/// 지연의 신호탄 전체 폭발
		void ClearStartOfDelayedFiringData();																								/// 사망 혹은 이탈시, 해당 유저가 적용한 지연의 신호탄 정보 삭제
		int	 CreateDamageEffectIndex();																										/// 폭발의 화살 DamageEffect의 Index 생성 함수
#endif SERV_RENA_NIGHT_WATCHER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		//LESI_A_LCR_MIDDLE_KICK
		void LESI_A_LCR_MIDDLE_KICK_Init();
		void LESI_A_LCR_MIDDLE_KICK_FrameMove();
		void LESI_A_LCR_MIDDLE_KICK_EventProcess();

		//LESI_A_LWS_BACK_KICK
		void LESI_A_LWS_BACK_KICK_Init();
		void LESI_A_LWS_BACK_KICK_EventProcess();

		//LESI_A_LWS_HIGH_KICK
		void LESI_A_LWS_HIGH_KICK_Init();
		void LESI_A_LWS_HIGH_KICK_EventProcess();

		//LESI_SA_LWS_AIRELINNA_SYLPH
		void LESI_SA_LWS_AIRELINNA_SYLPH_Init();
		void LESI_SA_LWS_AIRELINNA_SYLPH_FrameMove();

		//LESI_A_LSR_SHOOTING_MAGNUM
		void LESI_A_LSR_SHOOTING_MAGNUM_Init();
		void LESI_A_LSR_SHOOTING_MAGNUM_FrameMove();
		void LESI_A_LSR_SHOOTING_MAGNUM_EventProcess();

		//LESI_A_LGA_ARC_SHOT
		void LESI_A_LGA_ARC_SHOT_Init();
		void LESI_A_LGA_ARC_SHOT_FrameMove();
		void LESI_A_LGA_ARC_SHOT_EventProcess();

		//LESI_A_LGA_RAPID_SHOT
		void LESI_A_LGA_RAPID_SHOT_READY_Init();
		void LESI_A_LGA_RAPID_SHOT_READY_StartFuture();
		void LESI_A_LGA_RAPID_SHOT_READY_EventProcess();
		void LESI_A_LGA_RAPID_SHOT_ATTACK_EventProcess();
		void LESI_A_LGA_RAPID_SHOT_ATTACK_FrameMove();
		void LESI_A_LGA_RAPID_SHOT_FINISH_EventProcess();
		void LESI_A_LGA_RAPID_SHOT_FINISH_FrameMove();

		//LESI_A_LGA_FREEZING_BOLT
		void LESI_A_LGA_FREEZING_BOLT_Init();
		void LESI_A_LGA_FREEZING_BOLT_FrameMove();

		//LESI_A_LGA_WIND_BLAST
		void LESI_A_LGA_WIND_BLAST_Init();
		void LESI_A_LGA_WIND_BLAST_FrameMove();

		//LESI_SA_RTR_TRAPPING_ARROW_FUNGUS
		void LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_Init();
		void LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_StateStart();
		void LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_FrameMove();
		void LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_EventProcess();
		void LESI_SA_RTR_TRAPPING_ARROW_FUNGUS_StateEnd();

		//LESI_A_RNW_THORNS_TRAP
		void LESI_A_RNW_THORNS_TRAP_Init();
		void LESI_A_RNW_THORNS_TRAP_StartFuture();
		void LESI_A_RNW_THORNS_TRAP_FrameMoveFuture();
		void LESI_A_RNW_THORNS_TRAP_FrameMove();
		void LESI_A_RNW_THORNS_TRAP_EventProcess();
		void LESI_A_RNW_THORNS_TRAP_EndFuture();

		//LESI_P_RNW_SEED_OF_RUIN
		void LESI_P_RNW_SEED_OF_RUIN_Init();
		void LESI_P_RNW_SEED_OF_RUIN_StartFuture();
		void LESI_P_RNW_SEED_OF_RUIN_FrameMoveFuture();
		void LESI_P_RNW_SEED_OF_RUIN_FrameMove();
		void LESI_P_RNW_SEED_OF_RUIN_EventProcess();
		void LESI_P_RNW_SEED_OF_RUIN_EndFuture();

		//LESI_A_RNW_ANGER_OF_ELF
		void LESI_A_RNW_ANGER_OF_ELF_Init();
		void LESI_A_RNW_ANGER_OF_ELF_StateStart();
		void LESI_A_RNW_ANGER_OF_ELF_FrameMove();
		void LESI_A_RNW_ANGER_OF_ELF_EventProcess();
		void LESI_A_RNW_ANGER_OF_ELF_StateEnd();

		//LESI_A_RNW_ROSEBUSH_TRAP
		void LESI_A_RNW_ROSEBUSH_TRAP_Init();
		void LESI_A_RNW_ROSEBUSH_TRAP_StartFuture();
		void LESI_A_RNW_ROSEBUSH_TRAP_FrameMoveFuture();
		void LESI_A_RNW_ROSEBUSH_TRAP_FrameMove();
		void LESI_A_RNW_ROSEBUSH_TRAP_EventProcess();
		void LESI_A_RNW_ROSEBUSH_TRAP_EndFuture();

		//LESI_A_LWS_ASSAULT_IMPACT
		void LESI_A_LWS_ASSAULT_IMPACT_Init();
		void LESI_A_LWS_ASSAULT_IMPACT_EventProcess();

		//LESI_A_LTR_ENTANGLE
		void LESI_A_LTR_ENTANGLE_Init();
		void LESI_A_LTR_ENTANGLE_FrameMove();
		void LESI_A_LTR_ENTANGLE_EventProcess();

		void SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet, LIRE_ELVENRANGER_STATE_ID eStateID );
		void SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템

		// 윈스 궁극기 스파이럴 스트라이크

		// 공격 전 준비 모션
		void LESI_HA_LWS_SPIRAL_STIKE_READY_Init();
		void LESI_HA_LWS_SPIRAL_STIKE_READY_StateStart();
		void LESI_HA_LWS_SPIRAL_STIKE_READY_EventProcess();

		void LESI_HA_LWS_SPIRAL_STIKE_SPINING_StateStart();
		void LESI_HA_LWS_SPIRAL_STIKE_SPINING_FrameMove();
		void LESI_HA_LWS_SPIRAL_STIKE_SPINING_EventProcess();
		void LESI_HA_LWS_SPIRAL_STIKE_SPINING_StateEnd();
		// 이펙트 완료하고 난 후 마지막 모션
		void LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_Init();
		void LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_FrameMove();
		void LESI_HA_LWS_SPIRAL_STIKE_FLYING_ATTACK_EventProcess();

		// 그아 궁극기 크레이아트론 볼트
		// 크레이아트론 볼트 에서 슈터 데스티니로 이름 변경되었습니다. 13.07.28, kimjh

		void LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_Init();
		void LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_StateStart();
		void LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_FrameMove();
		void LESI_HA_LGA_CRYOTRON_BOLT_GUIDE_SHOT_EventProcess();

		void LESI_HA_LGA_CRYOTRON_BOLT_CHARGE_SHOT_EventProcess();

		void LESI_HA_LGA_CRYOTRON_BOLT_DOWNLANDING_EventProcess();


		// 나이트 와쳐 궁극기 - 이노센트
		void LESI_HA_LNW_INNOCENT_START_Init();			//기모으며 시작
		void LESI_HA_LNW_INNOCENT_START_EventProcess();
		void LESI_HA_LNW_INNOCENT_END_Init();			//칼로 찌르며 마무리
		void LESI_HA_LNW_INNOCENT_END_StateStart();
		void LESI_HA_LNW_INNOCENT_END_EventProcess();
		void LESI_HA_LNW_INNOCENT_END_StateEnd();
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템		
		
		typedef	srutil::delegate0<void>	DelegateFriendshipOfNature;
		DelegateFriendshipOfNature		m_delegateFriendshipOfNature;
};

IMPLEMENT_INTRUSIVE_PTR( CX2GULire_ElvenRanger::RapidShotData );
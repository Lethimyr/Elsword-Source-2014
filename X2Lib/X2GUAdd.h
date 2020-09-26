#pragma once

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )

/// 애드의 구성 모드를 설정할 수 있는지
#define ELSE_IF_CAN_CHANGE_FORMATION_MODE( hyperModeState ) \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	true == m_InputData.oneLCtr && true == GetPossibleChangeFormationMode() ) \
	{ StateChange( hyperModeState ); }

#define IF_CAN_CHANGE_FORMATION_MODE( hyperModeState ) \
	if( false == IsThereEffectiveBlackHoleDamageEffect() && \
	true == m_InputData.oneLCtr && true == GetPossibleChangeFormationMode() ) \
	{ StateChange( hyperModeState ); }

/// 애드의 구성 모드를 해제할 수 있는지
#define ELSE_IF_CAN_RELEASE_FORMATION_MODE \
	else if( true == m_InputData.oneLCtr && true == GetIsFormationMode() )  \
	{ m_FrameDataFuture.syncData.nowAction = UAI_RELEASE_FORMATION_MODE; }

/// 애드의 나소드 장갑 모드를 사용할 수 있는지
#define ELSE_IF_CAN_NASOD_ARMOR_ATTACK( iEventFlagIndex, fInputTimingStart, fInputTiming, fChangeTiming, bIsFormationMode, uidTargetUID ) \
	else if( m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] == true && \
			 m_pXSkinAnimFuture->GetNowAnimationTime() > (float)(fChangeTiming) && (UidType)uidTargetUID > 0 ) \
			{ StateChange( CX2GUAdd::ANSI_COMBO_NASOD_ARMOR_READY ); m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = false; } \
	else if( ( true == m_InputData.oneZ || true == m_InputData.oneX ) && \
			 m_pXSkinAnimFuture->GetNowAnimationTime() < (float)(fInputTiming) && \
			 m_pXSkinAnimFuture->GetNowAnimationTime() >= (float)(fInputTimingStart) && \
			 NULL != GetUnit() && CX2Unit::UC_ADD_NASOD_RULER != GetUnit()->GetClass() && \
			 true == (bool)bIsFormationMode ) \
			{ m_FrameDataFuture.stateParam.bEventFlagList[(int)(iEventFlagIndex)] = true; }

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
/// 애드 전용 스킬 캔슬 기능
#define SKILL_CANCEL_AFTER_ONLY_ADD( time ) \
	else if( (time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) && \
	SpecialAttackEventProcess() == true ) \
	{ } \
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && \
		true == m_InputData.oneLCtr && true == GetPossibleChangeFormationMode() && \
		(time) >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() > (time) ) \
	{ StateChange( USI_HYPER_MODE ); }
#endif // SKILL_CANCEL_BY_HYPER_MODE

class CX2GUAdd : public CX2GUUser
{
public:
	/// 나소드 아머 공격 타겟 정보
	struct NasodArmorAttackTargetData
	{
		UidType m_uidTargetUID;
		bool	m_bIsNPC;

		NasodArmorAttackTargetData() 
		: m_uidTargetUID( 0 )
		, m_bIsNPC( false )
		{}

		void SetTargetData( IN const UidType uidTargetUID, IN const bool bIsNPC )
		{
			m_uidTargetUID	= uidTargetUID;
			m_bIsNPC		= bIsNPC;
		}

		const CX2GameUnit* GetTargetUnit();

		void ResetTargetData()
		{
			m_uidTargetUID	= 0;
			m_bIsNPC		= false;
		}
	};

	/// 다이너모 마그네트론 소환 정보 ( 다이너모 구성 - 마그네트론 )
	struct MagnetronDamageEffectData
	{
		CX2DamageEffect::CEffectHandle									m_hLinkEffectHandle;	/// 데미지 이펙트 핸들
		std::pair<CX2DamageEffect::CEffectHandle, CX2DamageEffect::CEffectHandle>	m_hPylonHandle;			/// 연결된 두 파일런의 데미지 이펙트 핸들

		MagnetronDamageEffectData() 
		{
			m_hLinkEffectHandle = INVALID_DAMAGE_EFFECT_HANDLE;
			m_hPylonHandle		= std::make_pair( INVALID_DAMAGE_EFFECT_HANDLE, INVALID_DAMAGE_EFFECT_HANDLE );
		}

		MagnetronDamageEffectData( CX2DamageEffect::CEffectHandle hLinkEffectHandle_, std::pair<CX2DamageEffect::CEffectHandle, CX2DamageEffect::CEffectHandle> hPylonHandle_ ) 
		{
			m_hLinkEffectHandle = hLinkEffectHandle_;
			m_hPylonHandle		= hPylonHandle_;
		}
	};

	/// 다이너모 파일런 소환 정보 ( 다이너모 구성 - 파일런 )
	struct PylonDamageEffectData
	{
		CX2DamageEffect::CEffectHandle	m_EffectHandle;			/// 데미지 이펙트 핸들
		CX2DamageEffect::CEffectHandle	m_BuffEffectLHandle;		/// 버프용 이펙트 핸들
		float					m_fRemainTime;		/// 유지 시간

		PylonDamageEffectData() 
		: m_EffectHandle( INVALID_DAMAGE_EFFECT_HANDLE )
		, m_BuffEffectLHandle( INVALID_DAMAGE_EFFECT_HANDLE )
		, m_fRemainTime( 0.f )
		{}

		PylonDamageEffectData( CX2DamageEffect::CEffectHandle hEffectHandle_, CX2DamageEffect::CEffectHandle hBuffEffectHandle_, float  fRemainTime_ ) 
		{
			m_EffectHandle		= hEffectHandle_;
			m_BuffEffectLHandle	= hBuffEffectHandle_;
			m_fRemainTime		= fRemainTime_;
		}
	};

	/// 다이너모 파일런 연결 이펙트 객체 정보 ( 다이너모 구성 - 파일런 )
	struct PylonLinkDamageEffectData
	{
		CX2DamageEffect::CEffectHandle									m_hLinkEffectHandle;	/// 연결 데미지 이펙트 핸들
		std::pair<CX2DamageEffect::CEffectHandle, CX2DamageEffect::CEffectHandle>	m_hPylonHandle;			/// 연결된 두 파일런의 데미지 이펙트 핸들

		PylonLinkDamageEffectData() 
		{
			m_hLinkEffectHandle = INVALID_DAMAGE_EFFECT_HANDLE;
			m_hPylonHandle		= std::make_pair( INVALID_DAMAGE_EFFECT_HANDLE, INVALID_DAMAGE_EFFECT_HANDLE );
		}

		PylonLinkDamageEffectData( CX2DamageEffect::CEffectHandle hLinkEffectHandle_, std::pair<CX2DamageEffect::CEffectHandle, CX2DamageEffect::CEffectHandle> PylonHandle_ ) 
		{
			m_hLinkEffectHandle = hLinkEffectHandle_;
			m_hPylonHandle		= PylonHandle_;
		}
	};

	/// 다이너모 마그네트론 연결 이펙트 객체 정보 ( 다이너모 구성 - 마그네트론 )
	struct MagnetronData
	{
		vector<CX2DamageEffect::CEffectHandle>	m_vecMagnetronDamageEffectData;	/// 마그네트론 데미지 이펙트 저장 객체
		float									m_fFireReadyTime;				/// 마그네트론 발사 간격
		
		MagnetronData() 
			: m_fFireReadyTime( 0.f )
		{
			m_vecMagnetronDamageEffectData.reserve( 0 );
		}

		/// 스테이시스 필드 정보 초기화 함수
		void ResetMagnetronData();
	};

	/// 스테이시스 필드 피격 및 타격 수치 저장용 구조체
	struct StasisfieldData
	{
		float					m_fRemainTime;		/// 스테이시스 필드 유지 시간
		float					m_fHitValue;		/// 누적 타격치
		float					m_fDamageValue;		/// 누적 피격치

		float					m_fChangeRate;		/// 누적 피격/타격 수치에 의한 결과 값
		CX2EffectSet::Handle	m_hLoopEffectSet;	/// 필드 유지 이펙트 셋
		CX2EffectSet::Handle	m_hCountEffectSet;	/// 카운트 다운 이펙트 셋

		StasisfieldData() 
		: m_fRemainTime( 0.f )
		, m_fHitValue( 0.f )
		, m_fDamageValue( 0.f )
		, m_fChangeRate(1.f)
		, m_hLoopEffectSet( INVALID_EFFECTSET_HANDLE )
		, m_hCountEffectSet( INVALID_EFFECTSET_HANDLE )
		{}

		/// 스테이시스 필드 정보 초기화 함수
		void ResetStasisfieldData();
		
		/// 스테이시스 필드 동작 여부
		const bool GetIsActiveStasisfield() const  { return ( 0.f < m_fRemainTime ); }
	};

	/// 위상 변화 관련 정보
	struct PhaseShiftData
	{
		float					m_fEffectLifeTime;
		float					m_fReturnMPConsume;		/// 위상 변화 복귀 동작 MP 소비량
		D3DXVECTOR3				m_vUserReturnPos;		/// 유저 복귀 위치
		CX2DamageEffect::CEffectHandle m_hPhasShiftDamageEffect;

		// 초기화 관련
		PhaseShiftData();
		void InitPhaseEffectData();
		void InitResturnPosValue();
		// 유효화 검사
		bool IsValidReturnPosValue() const;
		bool GetIsActivePhaseShift() const{	return IsValidReturnPosValue();}
		// 슬롯 아이콘 갱신
		void UpdateSkillSlotUI();
	};

	/// 입자탄 - 과충전기 관련 정보
	struct ChargedImpulsarData
	{
		bool	m_bEnableChargedImpulsar;	/// 입자탄 - 과충전기 패시브 적용 여부

		// 증가 가능한 최대 배율. 스크립트에서 설정 함.
		float	m_fIncreaseLifeTimeMaxRate;		
		float	m_fIncreaseScaleMaxRate;		
		float	m_fIncreaseCurveSpeedMaxRate;
		float	m_fIncreaseDamageRate;

		ChargedImpulsarData() 
		{
			m_bEnableChargedImpulsar	= false;
			m_fIncreaseLifeTimeMaxRate		= 1.f;
			m_fIncreaseScaleMaxRate			= 1.f;
			m_fIncreaseCurveSpeedMaxRate	= 1.f;
			m_fIncreaseDamageRate			= 1.f;
		}

		void ResetChargedImpulsarData()
		{
			m_bEnableChargedImpulsar	= false;
			m_fIncreaseLifeTimeMaxRate		= 1.f;
			m_fIncreaseScaleMaxRate			= 1.f;
			m_fIncreaseCurveSpeedMaxRate	= 1.f;
			m_fIncreaseDamageRate			= 1.f;
		}
	};

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

	/// 환상 추적자 타겟 관련 정보
	struct FantasyTracerTargetInfo
	{
		bool	m_bEnableChargedImpulsar;	/// 입자탄 - 과충전기 패시브 적용 여부

		float						m_fRemainTime;		/// 유지 시간
		NasodArmorAttackTargetData	m_NATargetData;		/// 타겟 정보
		CX2SkillTree::SKILL_ID		m_eSkillID;			/// 나소드 콤보를 발동 시킬 스킬 아이디
		bool						m_bIsUseData;		/// 해당 타겟 정보 사용 여부

		FantasyTracerTargetInfo() 
		{
			m_fRemainTime	= 0.f;
			m_NATargetData	= NasodArmorAttackTargetData();
			m_eSkillID		= CX2SkillTree::SI_NONE;
			m_bIsUseData	= false;
		}

		FantasyTracerTargetInfo( float fRemainTime, NasodArmorAttackTargetData NATargetData, CX2SkillTree::SKILL_ID eSkillID ) 
		{
			m_fRemainTime	= fRemainTime;
			m_NATargetData	= NATargetData;
			m_eSkillID		= eSkillID;
			m_bIsUseData	= false;
		}

		void ResetFantasyTracerTargetInfo()
		{
			m_fRemainTime	= 0.f;
			m_NATargetData	= NasodArmorAttackTargetData();
			m_eSkillID		= CX2SkillTree::SI_NONE;
			m_bIsUseData	= false;
		}
	};
	
	/// 더스트 아나토미 타겟 정보
	struct DustAnatomyTargetInfo
	{
		CX2DamageEffect::CEffectHandle	m_eLinkEffect;			/// 연결 이펙트 핸들
		UidType							m_uidTargetUID;			/// 타겟 대상
		bool							m_bIsNPC;				/// 타겟 NPC 여부
		wstring							m_wstrCreateBoneName;	/// 생성 본 위치

		DustAnatomyTargetInfo() 
		{
			m_eLinkEffect			= INVALID_DAMAGE_EFFECT_HANDLE;
			m_uidTargetUID			= 0;
			m_bIsNPC				= false;
			m_wstrCreateBoneName	= L"";
		}

		DustAnatomyTargetInfo( CX2DamageEffect::CEffectHandle eLinkEffect, UidType uidTargetUID, bool bIsNPC, wstring wstrCreateBoneName ) 
		{
			m_eLinkEffect			= eLinkEffect;
			m_uidTargetUID			= uidTargetUID;
			m_bIsNPC				= bIsNPC;
			m_wstrCreateBoneName	= wstrCreateBoneName;
		}

		CX2GameUnit*	GetTargetUnit();
	};

	/// 리버스 리액터 타겟 정보
	struct ReverseReactorTargetInfo
	{
		float							m_fMaxTransitionDamage;		/// 전이할 수 있는 최대 데미지
		float							m_fCumulativeDamage;		/// 누적 데미지
		float							m_fRemainTime;				/// 유지 시간
		float							m_fTransitionDamageRate;	/// 전이 데미지 배율
		UidType							m_uidTargetUID;				/// 타겟 UID
		bool							m_bIsNPC;					/// 타겟 NPC 여부
		CX2DamageEffect::CEffectHandle	m_eLinkEffect;				/// 연결 이펙트 핸들

		ReverseReactorTargetInfo() 
		{
			m_fMaxTransitionDamage	= 0.f;
			m_fCumulativeDamage		= 0.f;
			m_fRemainTime			= 0.f;
			m_fTransitionDamageRate	= 0.f;
			m_uidTargetUID			= 0;
			m_bIsNPC				= false;
			m_eLinkEffect			= INVALID_DAMAGE_EFFECT_HANDLE;
		}

		ReverseReactorTargetInfo( float fMaxTransitionDamage, float fCumulativeDamage, float fRemainTime, float fTransitionDamageRate, 
								  UidType uidTargetUID, bool bIsNPC, CX2DamageEffect::CEffectHandle eLinkEffect ) 
		{
			m_fMaxTransitionDamage	= fMaxTransitionDamage;
			m_fCumulativeDamage		= fCumulativeDamage;
			m_fRemainTime			= fRemainTime;
			m_fTransitionDamageRate	= fTransitionDamageRate;
			m_uidTargetUID			= uidTargetUID;
			m_bIsNPC				= bIsNPC;
			m_eLinkEffect			= eLinkEffect;
		}

		CX2GameUnit*	GetTargetUnit();
		void			ResetReverseReactorTargetInfo();
	};

	/// 퀵실버 프렌지 관련 정보
	struct QuicksilverFrenzyData
	{
		bool	m_bEnableQuicksilverFrenzy;		/// 적용 여부
		float	m_fFireIntervalTime;			/// 발사 간격
		float	m_fPowerRate;					/// 데미지 배율

		QuicksilverFrenzyData() 
		{
			m_bEnableQuicksilverFrenzy	= false;
			m_fFireIntervalTime			= 0.f;
			m_fPowerRate				= 1.f;
		}

		void ResetQuicksilverFrenzyData()
		{
			m_bEnableQuicksilverFrenzy	= false;
			m_fFireIntervalTime			= 0.f;
			m_fPowerRate				= 1.f;
		}
	};

	/// 사이킥 스톰 관련 정보
	struct PsychicStormData
	{
		float					m_fIncreaseSize;		/// 증가되는 크기
		float					m_fLoopTime;			/// 반복 시간
		float					m_fFireGap;				/// 발사 간격
		CX2EffectSet::Handle	eLoopEffectHandle;		/// 루프 이펙트 핸들

		PsychicStormData() 
		{
			m_fIncreaseSize		= 0.f;
			m_fLoopTime			= 0.f;
			m_fFireGap			= 0.f;
			eLoopEffectHandle	= INVALID_EFFECTSET_HANDLE;
		}

		void ResetPsychicStormData();
	};
#endif //SERV_ADD_LUNATIC_PSYKER

	enum ADD_NASODRULER_STATE_ID
	{
		/// 전 캐릭터 공통 스테이트
		ANSI_BASE = USI_END + 1,

		ANSI_AIR_DASH,						/// 공중 대시

		ANSI_HYPER_WAIT,					/// 각성 대기 상태

		ANSI_STAND_UP_ROLLING_FRONT_FRONT,	/// FRONT DOWN 상태에서 앞으로 이동하며 기상
		ANSI_STAND_UP_ROLLING_FRONT_BACK,	/// FRONT DOWN 상태에서 뒤로 돌며 기상
		ANSI_STAND_UP_ROLLING_BACK_FRONT,	/// BACK DOWN 상태에서 앞으로 돌며 기상
		ANSI_STAND_UP_ROLLING_BACK_BACK,	/// BACK DOWN 상태에서 뒤로 이동하며 기상

		ANSI_STAND_UP_ATTACK_FRONT,			/// FRONT DOWN 상태에서 기상 공격
		ANSI_STAND_UP_ATTACK_BACK,			/// BACK DOWN 상태에서 기상 공격

		ANSI_DASH_JUMP_POWER_LANDING,		/// 최대 속도 낙하 착지

		ANSI_LEVITATION_WAIT,				/// 제자리 공중 부양

		ANSI_WALL_LANDING,					/// ???


		/// 기본 전직
		//////////////////////////////////////////////////////////////////////////

		/// 기본 콤보
		ANSI_COMBO_Z,						/// 콤보 Z
		ANSI_COMBO_ZZ,						/// 콤보 ZZ
		ANSI_COMBO_ZZZ,						/// 콤보 ZZZ
		ANSI_COMBO_ZZZZ,					/// 콤보 ZZZZ
		ANSI_COMBO_ZZZX,					/// 콤보 ZZZX
		ANSI_COMBO_ZZX,						/// 콤보 ZZX
		ANSI_COMBO_ZZXX,					/// 콤보 ZZXX

		ANSI_COMBO_X,						/// 콤보 X
		ANSI_COMBO_XX,						/// 콤보 XX
		ANSI_COMBO_XXX,						/// 콤보 XXX
		ANSI_COMBO_XXZ,						/// 콤보 XXZ
		ANSI_COMBO_XXZZ_START,				/// 콤보 XXZZ 시작
		ANSI_COMBO_XXZZ_LOOP,				/// 콤보 XXZZ 반복
		ANSI_COMBO_XXZZ_FINISH,				/// 콤보 XXZZ 마무리

		ANSI_JUMP_ATTACK_Z,					/// 점프 공격 Z
		ANSI_JUMP_ATTACK_X,					/// 점프 공격 X

		ANSI_DASH_COMBO_Z,					/// 대쉬 콤보 Z
		ANSI_DASH_COMBO_ZZ,					/// 대쉬 콤보 ZZ
		ANSI_DASH_COMBO_ZZZ,				/// 대쉬 콤보 ZZz

		ANSI_DASH_COMBO_X,					/// 대쉬 콤보 X
		ANSI_DASH_COMBO_XX,					/// 대쉬 콤보 XX
		ANSI_DASH_COMBO_XXX,				/// 대쉬 콤보 XXx

		ANSI_DASH_JUMP_COMBO_Z,				/// 대쉬 점프 콤보 Z
		ANSI_DASH_JUMP_COMBO_ZZ,			/// 대쉬 점프 콤보 ZZ
		ANSI_DASH_JUMP_COMBO_ZZZ,			/// 대쉬 점프 콤보 ZZZ
		ANSI_DASH_JUMP_COMBO_X,				/// 대쉬 점프 콤보 X
		ANSI_DASH_JUMP_COMBO_XX_START,		/// 대쉬 점프 콤보 XXZZ 시작
		ANSI_DASH_JUMP_COMBO_XX_CHARGE,		/// 대쉬 점프 콤보 XXZZ 반복
		ANSI_DASH_JUMP_COMBO_XX_FINISH,		/// 대쉬 점프 콤보 XXZZ 마무리


		/// 1-1차 사이킥 트레이서
		//////////////////////////////////////////////////////////////////////////

		/// 나소드 배리어
		ANSI_DP_BARRIER_ACTIVE,				/// DP 베리어 가동

		/// 나소드 아머 콤보
		ANSI_COMBO_NASOD_ARMOR_READY,		/// 나소드 아머 공격 준비
		ANSI_COMBO_NASOD_ARMOR_ATTACK,		/// 나소드 아머 공격
		ANSI_COMBO_NASOD_ARMOR_LANDING,		/// 나소드 아머 착지
		ANSI_COMBO_NASOD_ARMOR_Z,			/// 나소드 아머 콤보 Z
		ANSI_COMBO_NASOD_ARMOR_ZZ,			/// 나소드 아머 콤보 ZZ
		ANSI_COMBO_NASOD_ARMOR_ZZZ,			/// 나소드 아머 콤보 ZZZZ
		ANSI_COMBO_NASOD_ARMOR_ZZX,			/// 나소드 아머 콤보 ZZX
		ANSI_COMBO_NASOD_ARMOR_ZZXX,		/// 나소드 아머 콤보 ZZXX
		ANSI_COMBO_NASOD_ARMOR_X,			/// 나소드 아머 콤보 X
		ANSI_COMBO_NASOD_ARMOR_XX,			/// 나소드 아머 콤보 XX
		ANSI_COMBO_NASOD_ARMOR_XXX,			/// 나소드 아머 콤보 XXX

		/// 스폐셜 액티브
		ANSI_SA_APT_STASIS_FIELD_ACTIVE,	/// 스테이시스 필드 발동 스테이트

		ANSI_SA_APT_PULSE_CANNON_LOOP,		/// 펄스 캐논 차지
		ANSI_SA_APT_PULSE_CANNON_FINISH,	/// 펄스 캐논 발사

		/// 액티브
		ANSI_A_AN_PHASE_SHIFT_BLINK,		/// 위상 변화 복귀 스테이트

		ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY,		/// 입자탄 과충전기 준비
		ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE,		/// 입자탄 과충전기 차지
		ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE,		/// 입자탄 과충전기 발사


		/// 1-2차 루나틱 사이커
		//////////////////////////////////////////////////////////////////////////

		/// 나소드 아머 콤보
		ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_START,		/// 나소드 아머 콤보 ZZFrontZ 준비
		ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP,		/// 나소드 아머 콤보 ZZFrontZ 반복
		ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX,			/// 나소드 아머 콤보 ZZFrontZX
		ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionX_LOOP,	/// 나소드 아머 콤보 XDirectionX 반복
		ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ,		/// 나소드 아머 콤보 XDirectionXZ

		ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK,				/// 퀵실버 액셀 후진

		/// 스폐셜 액티브
		ANSI_SA_ALP_PSYCHIC_STORM_LOOP,		/// 사이킥 스톰 반복
		ANSI_SA_ALP_PSYCHIC_STORM_FINISH,	/// 사이킥 스톰 마지막 타격
	};


	/// 특수한 상황에서의 렌더 설정 상태
	enum CUSTOM_RENDER_STATE
	{
		CRS_NONE						= 0,
		CRS_COMBO_DASH_X				= 1,	/// 대시 X 투명
		CRS_QUICKSILVER_FRENZY_BUFF		= 2,	/// 퀵실버 프렌지 버프 적용 효과
		CRS_END							= 3,
	};

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 출력 교환기 현재 상황 ( DP가 50% 이상인가, 미만인가 )
	enum POWER_EXCHANGER_STATE
	{
		PES_NONE			= 0,
		PES_HALF_UP			= 1,	/// DP 50% 이상
		PES_HALF_DOWN		= 2,	/// DP 50% 미만
		PES_END				= 3,
	};
#endif //SERV_ADD_LUNATIC_PSYKER

/// private Function

#pragma region private Function

private:

	CX2GUAdd( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit );
	virtual ~CX2GUAdd(void);

/// private Function End


/// protected Function Start

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

	virtual void CommonEventProcess();

	virtual void DetonationStateChange( CX2DamageManager::DamageData* pDamageData );

	virtual RENDER_HINT	CommonRender_Prepare();

	virtual bool SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ = CX2SkillTree::ASUT_GROUND );
	virtual void NoStateChangeActionFrameMove();
	virtual float GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;

	float GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const;

	virtual CX2GageData* CreateGageData();

	virtual	bool ShouldResetNotBusyTimer();

	void InitStateID();
	void InitStateCommon();
	void InitStateByUnitClass();

	void SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, ADD_NASODRULER_STATE_ID eStateID_ );
	void SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ );


	void ShowActiveSkillCutInAndLight( float fTimeToShow_, bool bOnlyLight_ = false );
	virtual void ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );

	bool CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND );
	void StateChangeSpecialAttack( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );

	void PlayLoopSound( const WCHAR* pSoundFileName_, bool bLoop_, bool b3D_ );
	void StopLoopSound();

	virtual float GetChangeDamageRateByAttackerSkill( const CX2DamageManager::DamageData* pDamageData_  ) const;

#ifdef FIX_NO_STATE_SKILL_BUG
	virtual bool IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ );
#endif // FIX_NO_STATE_SKILL_BUG

#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
	virtual void DieDamageEffectProc( const CX2DamageManager::DamageData& pDamageData_ );
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE

	virtual void ChargeMpAndEtcInTrainingGame();
	/// 공통 스테이트
	//////////////////////////////////////////////////////////////////////////

	/// ANSI_DIE_FRONT
	void DieFrontStartFuture();
	void DieFrontStart();
	void DieFrontFrameMove();
	void DieFrontEventProcess();

	/// ANSI_DIE_BACK
	void DieBackStartFuture();
	void DieBackStart();
	void DieBackFrameMove();
	void DieBackEventProcess();

	/// ANSI_WAIT
	void WaitStartFuture();
	void WaitStart();
	void WaitEventProcess();

	/// ANSI_HYPER_WAIT
	void HyperWaitStartFuture();
	void HyperWaitStart();
	void HyperWaitEventProcess();

	/// ANSI_WALK
	void WalkEventProcess();

	/// ANSI_JUMP_READY
	void JumpReadyEventProcess();

	/// ANSI_JUMP_UP
	void JumpUpEventProcess();

	/// ANSI_JUMP_DOWN
	void JumpDownEventProcess();

	/// ANSI_JUMP_LANDING
	void JumpLandingStartFuture();
	void JumpLandingEventProcess();

	/// ANSI_DASH
	void DashEventProcess();

	/// ANSI_DASH_END
	void DashEndStartFuture();
	void DashEndFrameMove();
	void DashEndEventProcess();

	/// ANSI_AIR_DASH
	void AirDashEventProcess();
	void AirDashStateEndFuture();

	/// ANSI_DASH_JUMP
	void DashJumpStartFuture();
	void DashJumpStart();
	void DashJumpFrameMoveFuture();
	void DashJumpEventProcess();
	void DashJumpEndFuture();

	/// ANSI_DASH_JUMP_LANDING
	void DashJumpLandingEventProcess();
	void DashJumpLandingStartFuture();

	/// ANSI_LEVITATION_WAIT
	void LevitationWaitFrameMove();
	void LevitationWaitEventProcess();

	/// ANSI_DAMAGE_SMALL_FRONT
	/// USI_DAMAGE_SMALL_BACK
	void DamageSmallEventProcess();

	/// ANSI_DAMAGE_BIG_FRONT
	/// ANSI_DAMAGE_BIG_BACK
	void DamageBigStart();
	void DamageBigEventProcess();

	/// ANSI_DAMAGE_DOWN_FRONT
	void DamageDownFrontFrameMove();
	void DamageDownFrontEventProcess();

	/// ANSI_DAMAGE_DOWN_BACK
	void DamageDownBackFrameMove();
	void DamageDownBackEventProcess();

	/// ANSI_DAMAGE_AIR_DOWN_LANDING
	void DamageAirDownLandingEventProcess();

	/// ANSI_STAND_UP_ROLLING_FRONT_FRONT
	void StandUpRollingFrontFrontEventProcess();
	void StandUpRollingFrontFrontEnd();

	/// ANSI_STAND_UP_ROLLING_FRONT_BACK
	void StandUpRollingFrontBackEventProcess();
	void StandUpRollingFrontBackEnd();

	/// ANSI_STAND_UP_ROLLING_BACK_FRONT
	void StandUpRollingBackFrontEventProcess();
	void StandUpRollingBackFrontEnd();

	/// ANSI_STAND_UP_ROLLING_BACK_BACK
	void StandUpRollingBackBackEventProcess();
	void StandUpRollingBackBackEnd();

	/// ANSI_STAND_UP_ATTACK_FRONT
	void StandUpAttackFrontEventProcess();
	void StandUpAttackFrontEnd();

	/// ANSI_STAND_UP_ATTACK_BACK
	void StandUpAttackBackEventProcess();
	void StandUpAttackBackEnd();
	void StandUpAttackBackStateEndFuture();

	/// ANSI_DASH_JUMP_POWER_LANDING
	void DashJumpPowerLandingStartFuture();
	void DashJumpPowerLandingStart();
	void DashJumpPowerLandingEventProcess();

	/// 구성 모드
	void FormationModeStart();

	/// ANSI_NP_BARRIER_ACTIVE
	void NasodBarrierActiveStateStartFuture();
	void NasodBarrierActiveEventProcess();
	
	/// ANSI_COMBO_NASOD_ARMOR_READY,
	void NasodArmorReadyEventProcess();

	/// ANSI_COMBO_NASOD_ARMOR_ATTACK,
	void NasodArmorAttackStartFuture();
	void NasodArmorAttackEventProcess();

	///	ANSI_COMBO_NASOD_ARMOR_LANDING,
	void NasodArmorLandingEventProcess();
	void NasodArmorLandingEndFuture();

	/// ANSI_COMBO_NASOD_ARMOR_Z
	void NasodArmorComboZEventProcess();

	/// ANSI_COMBO_NASOD_ARMOR_ZZ
	void NasodArmorComboZZEventProcess();

	/// ANSI_COMBO_NASOD_ARMOR_ZZZ
	void NasodArmorComboZZZEventProcess();

	/// ANSI_COMBO_NASOD_ARMOR_ZZX
	void NasodArmorComboZZXEventProcess();

	/// ANSI_COMBO_NASOD_ARMOR_ZZXX
	void NasodArmorComboZZXXEventProcess();

	/// ANSI_COMBO_NASOD_ARMOR_X
	void NasodArmorComboXEventProcess();
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	void NasodArmorComboXStateEndFuture();
#endif //SERV_ADD_LUNATIC_PSYKER

	/// ANSI_COMBO_NASOD_ARMOR_XX
	void NasodArmorComboXXEventProcess();

	/// ANSI_COMBO_NASOD_ARMOR_XXX
	void NasodArmorComboXXXEventProcess();

	
	///기본 커맨드
	//////////////////////////////////////////////////////////////////////////

	/// ANSI_COMBO_Z
	void ComboZEventProcess();

	/// ANSI_COMBO_ZZ
	void ComboZZEventProcess();

	/// ANSI_COMBO_ZZZ
	void ComboZZZEventProcess();

	/// ANSI_COMBO_ZZZZ
	void ComboZZZZEventProcess();

	/// ANSI_COMBO_ZZZX
	void ComboZZZXEventProcess();

	/// ANSI_COMBO_ZZX
	void ComboZZXEventProcess();

	/// ANSI_COMBO_ZZXX
	void ComboZZXXEventProcess();

	/// ANSI_COMBO_X
	void ComboXEventProcess();

	/// ANSI_COMBO_XX
	void ComboXXEventProcess();

	/// ANSI_COMBO_XXX
	void ComboXXXEventProcess();

	/// ANSI_COMBO_XXZ
	void ComboXXZEventProcess();

	/// ANSI_COMBO_XXZZ_START
	void ComboXXZZStartEventProcess();
	void ComboXXZZStartStateEndFuture();

	/// ANSI_COMBO_XXZZ_LOOP
	void ComboXXZZLoopStateStart();
	void ComboXXZZLoopEventProcess();
	void ComboXXZZLoopStateEnd();

	/// ANSI_COMBO_XXZZ_FINISH
	void ComboXXZZFinishStateStartFuture();
	void ComboXXZZFinishStateStart();
	void ComboXXZZFinishEventProcess();

	/// ANSI_JUMP_ATTACK_Z
	void JumpComboZEventMoveFuture();
	void JumpComboZEventProcess();

	/// ANSI_JUMP_ATTACK_X
	void JumpComboXEventMoveFuture();
	void JumpComboXEventProcess();

	/// ANSI_DASH_COMBO_Z
	void DashComboZFrameMoveFuture();
	void DashComboZEventProcess();

	/// ANSI_DASH_COMBO_ZZ
	void DashComboZZEventProcess();

	/// ANSI_DASH_COMBO_ZZZ
	void DashComboZZZEventProcess();

	/// ANSI_DASH_COMBO_X
	void DashComboXStateStart();
	void DashComboXFrameMove();
	void DashComboXEventProcess();
	void DashComboXStateEnd();
	void DashComboXStateEndFuture();

	/// ANSI_DASH_COMBO_XX
	void DashComboXXEventProcess();	

	/// ANSI_DASH_COMBO_XXX
	void DashComboXXXEventProcess();	

	/// ANSI_DASH_JUMP_COMBO_Z
	void DashJumpComboZEventProcess();	

	/// ANSI_DASH_JUMP_COMBO_ZZ
	void DashJumpComboZZEventProcess();	

	/// ANSI_DASH_JUMP_COMBO_ZZZ
	void DashJumpComboZZZEventProcess();	

	/// ANSI_DASH_JUMP_COMBO_X
	void DashJumpComboXEventProcess();	

	/// ANSI_DASH_JUMP_COMBO_XX_START
	void DashJumpComboXXStartEventProcess();
	void DashJumpComboXXStartStateEndFuture();

	/// ANSI_DASH_JUMP_COMBO_XX_CHARGE
	void DashJumpComboXXLoopStateStart();	
	void DashJumpComboXXLoopFrameMove();
	void DashJumpComboXXLoopEventProcess();	
	void DashJumpComboXXLoopStateEnd();	

	/// ANSI_DASH_JUMP_COMBO_XX_FINISH
	void DashJumpComboXXFinishStateStartFuture();	
	void DashJumpComboXXFinishFrameMove();	
	void DashJumpComboXXFinishEventProcess();	


	/// 스패셜 액티브
	//////////////////////////////////////////////////////////////////////////

	/// ANSI_SA_AN_VOID_BREAKER
	void ANSI_SA_AN_VOID_BREAKER_Init();
	void ANSI_SA_AN_VOID_BREAKER_FrameMove();
	void ANSI_SA_AN_VOID_BREAKER_EventProcess();

	/// ANSI_SA_AN_PARTICLE_PRISM
	void ANSI_SA_AN_PARTICLE_PRISM_Init();
	void ANSI_SA_AN_PARTICLE_PRISM_FrameMove();
	void ANSI_SA_AN_PARTICLE_PRISM_EventProcess();

	/// ANSI_SA_AN_ENERGY_BOOM
	void ANSI_SA_AN_ENERGY_BOOM_Init();
	void ANSI_SA_AN_ENERGY_BOOM_FrameMove();
	void ANSI_SA_AN_ENERGY_BOOM_EventProcess();

	/// ANSI_SA_AN_PARTICLE_ACCELERATOR
	void ANSI_SA_AN_PARTICLE_ACCELERATOR_Init();
	void ANSI_SA_AN_PARTICLE_ACCELERATOR_FrameMove();
	void ANSI_SA_AN_PARTICLE_ACCELERATOR_EventProcess();

	/// ANSI_SA_AN_NEUTRON_BOMB
	void ANSI_SA_AN_NEUTRON_BOMB_Init();
	void ANSI_SA_AN_NEUTRON_BOMB_EventProcess();

	/// ANSI_SA_APT_PULSE_CANNON
	void ANSI_SA_APT_PULSE_CANNON_READY_Init();
	void ANSI_SA_APT_PULSE_CANNON_READY_StateStartFuture();
	void ANSI_SA_APT_PULSE_CANNON_READY_StateStart();
	void ANSI_SA_APT_PULSE_CANNON_READY_EventProcess();

	void ANSI_SA_APT_PULSE_CANNON_LOOP_FrmaeMove();
	void ANSI_SA_APT_PULSE_CANNON_LOOP_EventProcess();

	void ANSI_SA_APT_PULSE_CANNON_FINISH_EventProcess();

	/// ANSI_SA_APT_PULSE_CANNON
	void ANSI_SA_APT_EMP_SHOCK_Init();
	void ANSI_SA_APT_EMP_SHOCK_FrameMove();
	void ANSI_SA_APT_EMP_SHOCK_EventProcess();

	/// ANSI_SA_APT_QUAKE_BUSTER
	void ANSI_SA_APT_QUAKE_BUSTER_Init();
	void ANSI_SA_APT_QUAKE_BUSTER_EventProcess();
	void ANSI_SA_APT_QUAKE_BUSTER_FrameMove();
	void ANSI_SA_APT_QUAKE_BUSTER_HYPER_EventProcess();

	/// ANSI_SA_APT_STASIS_FIELD
	void ANSI_SA_APT_STASIS_FIELD_Init();
	void ANSI_SA_APT_STASIS_FIELD_StateStart();
	void ANSI_SA_APT_STASIS_FIELD_FrameMove();
	void ANSI_SA_APT_STASIS_FIELD_EventProcess();

	void ANSI_SA_APT_STASIS_FIELD_ACTIVE_StateStart();
	void ANSI_SA_APT_STASIS_FIELD_ACTIVE_FrameMove();
	void ANSI_SA_APT_STASIS_FIELD_ACTIVE_EventProcess();

	/// ANSI_SA_APT_CONQUEROR 퀀커러
	void ANSI_SA_APT_CONQUEROR_Init();
	void ANSI_SA_APT_CONQUEROR_EventProcess();
	void ANSI_SA_APT_CONQUEROR_FrameMove();


	/// 액티브
	//////////////////////////////////////////////////////////////////////////

	/// SI_A_AN_PULSE_BULLET
	void ANSI_A_AN_PULSE_BULLET_Init();
	void ANSI_A_AN_PULSE_BULLET_EventProcess();

	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_StateStartFuture();
	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_EventProcess();
	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_StateEndFuture();

	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_StateStart();
	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_FrameMove();
	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_EventProcess();

	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_StateStartFuture();
	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_FrameMove();
	void ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_EventProcess();

	/// SI_A_AN_MIND_BREAK
	void ANSI_A_AN_MIND_BREAK_Init();
	void ANSI_A_AN_MIND_BREAK_FrmaeMove();
	void ANSI_A_AN_MIND_BREAK_EventProcess();

	/// SI_A_AN_PHASE_SHIFT
	void ANSI_A_AN_PHASE_SHIFT_Init();
	void ANSI_A_AN_PHASE_SHIFT_FrameMove();
	void ANSI_A_AN_PHASE_SHIFT_EventProcess();

	void ANSI_A_AN_PHASE_SHIFT_BLINK_StateStart();
	void ANSI_A_AN_PHASE_SHIFT_BLINK_EventProcess();

	/// SI_A_APT_MAGNETRON_DYNAMO
	void ANSI_A_APT_MAGNETRON_DYNAMO_Init();
	void ANSI_A_APT_MAGNETRON_DYNAMO_FrameMove();
	void ANSI_A_APT_MAGNETRON_DYNAMO_EventProcess();

	/// SI_A_APT_DESOLVER_DYNAMO
	void ANSI_A_APT_DESOLVER_DYNAMO_Init();
	void ANSI_A_APT_DESOLVER_DYNAMO_EventProcess();

	/// SI_A_APT_PYLON_DYNAMO
	void ANSI_A_APT_PYLON_DYNAMO_Init();
	void ANSI_A_APT_PYLON_DYNAMO_FrameMove();
	void ANSI_A_APT_PYLON_DYNAMO_EventProcess();

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_START
	void NasodArmorComboZZFrontZStartEventProcess();

	/// ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP
	void NasodArmorComboZZFrontZLoopStateStartFuture();
	void NasodArmorComboZZFrontZLoopStateStart();
	void NasodArmorComboZZFrontZLoopEventProcess();

	/// ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX
	void NasodArmorComboZZFrontZXEventProcess();

	/// ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionX_LOOP
	void NasodArmorComboXDirectionXLoopEventProcess();
	void NasodArmorComboXDirectionXLoopStateEndFuture();

	/// ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ
	void NasodArmorComboXDirectionXZEventProcess();


	/// SA_ALP_REVERSE_REACTOR
	void ANSI_SA_ALP_REVERSE_REACTOR_Init();
	void ANSI_SA_ALP_REVERSE_REACTOR_StateStart();
	void ANSI_SA_ALP_REVERSE_REACTOR_FrameMove();
	void ANSI_SA_ALP_REVERSE_REACTOR_EventProcess();

	/// SA_ALP_QUICKSILVER_ACCEL
	void ANSI_SA_ALP_QUICKSILVER_ACCEL_Init();
	void ANSI_SA_ALP_QUICKSILVER_ACCEL_FrameMove();
	void ANSI_SA_ALP_QUICKSILVER_ACCEL_EventProcess();
	void ANSI_SA_ALP_QUICKSILVER_ACCEL_StateEnd();

	/// SA_ALP_QUICKSILVER_ACCEL_BACK
	void ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_FrameMove();
	void ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_EventProcess();
	void ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_StateEnd();

	/// SA_ALP_QUICKSILVER_FRENZY
	void ANSI_SA_ALP_QUICKSILVER_FRENZY_Init();
	void ANSI_SA_ALP_QUICKSILVER_FRENZY_FrameMove();
	void ANSI_SA_ALP_QUICKSILVER_FRENZY_EventProcess();

	/// SA_ALP_DUST_ANATOMY
	void ANSI_SA_ALP_DUST_ANATOMY_Init();
	void ANSI_SA_ALP_DUST_ANATOMY_FrameMove();
	void ANSI_SA_ALP_DUST_ANATOMY_EventProcess();
	void ANSI_SA_ALP_DUST_ANATOMY_StateEnd();

	/// SA_ALP_PSYCHIC_STORM
	void ANSI_SA_ALP_PSYCHIC_STORM_Init();
	void ANSI_SA_ALP_PSYCHIC_STORM_StateStartFuture();
	void ANSI_SA_ALP_PSYCHIC_STORM_StateStart();
	void ANSI_SA_ALP_PSYCHIC_STORM_EventProcess();

	void ANSI_SA_ALP_PSYCHIC_STORM_LOOP_StateStart();
	void ANSI_SA_ALP_PSYCHIC_STORM_LOOP_FrameMove();
	void ANSI_SA_ALP_PSYCHIC_STORM_LOOP_EventProcess();

	void ANSI_SA_ALP_PSYCHIC_STORM_FINISH_FrameMove();
	void ANSI_SA_ALP_PSYCHIC_STORM_FINISH_EventProcess();

	/// A_ALP_PSIONIC_BEAT
	void ANSI_A_ALP_PSIONIC_BEAT_Init();
	void ANSI_A_ALP_PSIONIC_BEAT_EventProcess();

	/// A_ALP_TWIRL_RUSH
	void ANSI_A_ALP_TWIRL_RUSH_Init();
	void ANSI_A_ALP_TWIRL_RUSH_EventProcess();

	/// HA_ALP_DOOMS_DAY
	void ANSI_HA_ALP_DOOMS_DAY_Init();
	void ANSI_HA_ALP_DOOMS_DAY_StateStart();
	void ANSI_HA_ALP_DOOMS_DAY_FrameMove();
	void ANSI_HA_ALP_DOOMS_DAY_EventProcess();
	void ANSI_HA_ALP_DOOMS_DAY_StateEnd();
#endif //SERV_ADD_LUNATIC_PSYKER

/// protected Function End


/// public Function Start

public:

	static CX2GUAdd*	CreateGUAdd( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit );

	static void			AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );

	// 탈 것
	virtual bool		CanRidingState();

	virtual void		RidingWaitEventProcess();
	virtual void		RidingWalkEventProcess();
	virtual void		RidingJumpLandingEventProcess();
	virtual void		RidingDashEventProcess();
	virtual void		RidingDashEndEventProcess();

	virtual void		ReInit( bool bTeamPos = false, int iStartPosIndex = -1 );
	virtual void		DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ );

	virtual void		Win();
	virtual void		Lose();
	virtual void		Draw();

	virtual void		InitEffect();

	virtual void		CreateMajorParticleArray();
	virtual void		CreateMinorParticleArray();

	static void			AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );
	static void			AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ );

	virtual bool		SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
	virtual void		COMMON_BUFF_FrameMove();

	virtual void		HyperModeFrameMove();

#ifdef MODIFY_RIDING_PET_AWAKE
	void				RidingHyperModeFrameMove();
	void				CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ = false );
#endif //MODIFY_RIDING_PET_AWAKE

	virtual void		HyperModeBuffEffectStart();

	virtual void		SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void		UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );
	virtual void		ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );

	virtual void		InitEnchantWeaponEffectUnitSpecific();

	void				AttackResultByType( CX2DamageManager::DamageData &pDamageData );

#ifdef MODIFY_SET_DAMAGE_DATA
	virtual	void		SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_ ,OUT float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA

	virtual void		ApplyHyperModeBuff();

	/// 동기화 관련 처리
	const SHORT			GetAbilitySyncData();
	void				SetAbilitySyncData( IN const SHORT sAbilitySyncData_ );
	
	/// 특정 스테이트 변경 관련 처리
	void				PlayAnimationWaitStartByAdd( IN CKTDGXSkinAnim* pXSkinAnim_, IN const FrameData& refFrameData_ );
	virtual bool		StateChange( int state, bool bDirectChange = false );
	virtual void		SetCustomAnimName( IN const UserUnitStateData& UserStateData, IN OUT wstring& wstrAnimName );
	
	/// 게이지 관련
	CX2AddGageData*		GetAddGageData() const;

	/// 특정 스킬에 대한 처리
	const bool			SpecialAttackCustomStateChange( IN const CX2SkillTree::SKILL_ID eSkillID_ );
	const float			GetCustomSkillCoolTime( IN const float fCoolTime_, IN const CX2SkillTree::SKILL_ID eSkillID_ );


#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	const bool			CheckSkillSpecialUse( IN const CX2SkillTree::SKILL_ID eSkillID_  );
#endif //SERV_ADD_LUNATIC_PSYKER
	
#ifdef ADD_MEMO_1ST_CLASS //김창한
	const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION GetSkillUseCondition( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ );
#endif //ADD_MEMO_1ST_CLASS

	/// 변이 수치
	const int			GetMutationCount();																		/// 변이 수치 반환 함수
	void				SetChangedMutationCount( IN const bool bChangedMutationCount_ );						/// 변이 수치 갱신 여부 설정
	virtual void		SetMutationCount( IN const int iMutationCount_ );										/// 변이 수치 대입 함수
	void				UpMutationCount( IN const int iMutationCount_  );										/// 변이 수치 연산 함수

	/// DP 수치
	const float			GetMaxDPValue();																		/// 최대 DP 수치 반환 함수
	const float			GetDPValue();																			/// DP 수치 반환 함수
	virtual void		SetDPValue( IN const float fDPValue_ );													/// DP 수치 대입 함수
	virtual void		UpNowDPValue( IN const float fDPValue_, IN const float fChangeValueRate_ = 1.f );		/// DP 수치 연산 함수
	virtual void		UpNowDPValueByDamage( IN const float fDamage_ );										/// 피격시 DP 변동 처리
	virtual void		UpNowDPValueByHit( IN const CX2DamageManager::DamageData& DamageData_ );				/// 타격시 DP 변동 처리
	void				UpNowDPValueByUseMP( IN const float fUseMP );											/// MP 사용시 DP 변동 처리
	const float			GetActualDPConsume( IN const CX2UserSkillTree::SkillSlotData* pSkillSlotData_ );				/// DP 소모량
	const int			GetMutationPointConsume( IN const CX2UserSkillTree::SkillSlotData* pSkillSlotData_ );	/// 변이 포인트 소모량
	bool				IsDPConsumptionSkill( IN const CX2SkillTree::SKILL_ID eSkillID_ ) const;				/// 해당 스킬이 DP를 사용하는 스킬인지 검사
	const bool			ConsumeMPorDP( IN const float fConsumeMP, IN const float fConsumeDP );					/// MP 혹은 DP 소비 검사 함수
	const float			GetDPRateByType( IN const CX2DamageManager::DYNAMO_PARTICLE_RATE_TYPE eDPRateType );	/// DP 배율 타입에 따른 배율 값
	const float 		GetUseDPByFormationMode();		 														/// 구성 모드에 사용되는 DP 량
	const float			GetEffectiveIncreaseDPValue();															/// DP 획득량 증가값 반환
	const float			GetEffectiveDecreaseDPConsume();														/// DP 소모량 감소값 반환
	virtual const bool	GetUpDPByAttackThisFrame() { return m_bSetUpDPByAttackThisFrame; }						/// 타격시 한 프레임에 DP 획득된 여부 Flag 반환
	virtual void		SetUpDPByAttackThisFrame( IN const bool bval_ ) { m_bSetUpDPByAttackThisFrame = bval_; }/// 타격시 한 프레임에 DP 획득된 여부 Flag 설정
	virtual const bool	GetUpDPByDamageThisFrame() { return m_bSetUpDPByAttackThisFrame; }						/// 피격시 한 프레임에 DP 획득된 여부 Flag 반환
	virtual void		SetUpDPByDamageThisFrame( IN const bool bval_ ) { m_bSetUpDPByDamageThisFrame = bval_; }/// 피격시 한 프레임에 DP 획득된 여부 Flag 설정


	/// 구성 모드
	virtual const bool	GetIsFormationMode() const;																/// 구성 모드 적용 여부 반환 함수
	virtual void		SetIsFormationMode( IN const bool bIsFormationMode_ );									/// 구성 모드 적용 여부 설정 함수
	virtual float		GetHyperModeTime() const { return 9999999.f; }											/// 각성 시간 반환 ( 애드는 각성 지속 시간이 무한이다. )
	const bool			GetPossibleChangeFormationMode( IN const bool bOutputInformation = true );				/// 구성 모드 사용 가능 여부 반환
	const float			GetChangeFormationCoolTime();															/// 구성 모드 전환 쿨타임 반환
	void				SetChangeFormationCoolTime( IN const float fCoolTime_ );								/// 구성 모드 전환 쿨타임 설정

	bool				IsPossibleChangeNasodBarrier( IN const CX2DamageManager::DamageData* pDamageData_ );	/// 나소드 배리어 실행 처리 함수
	void				SetNasodArmorAttackTarget( IN const CX2DamageManager::DamageData& pDamageData_ );		/// 나소드 아머 공격 타겟팅 설정 함수

	/// 다이너모 구성 - 파일런
	void				AddPylonLinkDamageEffect( IN CX2DamageEffect::CEffect* pAddPylon_ );					/// 파일런간 연결 데미지 이펙트 추가 함수
	void				DeletePylonLinkDamageEffect( IN CX2DamageEffect::CEffectHandle hDeletePylonHandle_ );	/// 파일런간 연결 데미지 이펙트 삭제 함수
	void				ProcessPylonDamageEffect();																/// 파일런 유지 시간 제어 함수
	bool				GetSummonPylonPos( OUT D3DXVECTOR3& fSummonPos );										/// 파일런 소환 가능한 위치 반환 함수
	void				ResetPylonData();																		/// 파일런 관련 모든 정보 및 이펙트 초기화
	void				ProcessNonstopSummonPylon();															/// 파일런 연속 소환 관련 제어
	const bool			GetPossibleNonstopSummonPylon() const;													/// 파일런 연속 소환 가능 여부

	/// 다이너모 구성 - 마그네트론
	void				ProcessMagnetronDamageEffect();															/// 마그네트론 위치 제어 함수
	void				AttackMagnetronDamageEffect( IN const CX2DamageManager::DamageData& pDamageData_ );		/// 마그네트론 타겟팅 설정 함수

	/// 스테이시스 필드
	const StasisfieldData&	GetStasisfieldData() { return m_StasisfieldData; }									/// 스테이시스 필드 정보값 반환 함수
	const bool			GetIsActiveStasisfield() { return m_StasisfieldData.GetIsActiveStasisfield(); }			/// 스테이시스 필드 적용 여부 반환 함수
	virtual void		SetStasisfieldHitValue( IN const float fHitValue_ );									/// 스테이시스 필드 타격치 설정 함수
	virtual void		SetStasisfieldDamageValue( IN const float fDamageValue_ );								/// 스테이시스 필드 피격치 설정 함수
	void				ProcessStasisField();																	/// 스테이시스 필드 매 프레임 처리 함수
	void				EndStasisFieldByTime();																	/// 유지 시간이 지나서, 스테이시스 필드 종료 되었을 때 동작 처리 함수

	/// 위상 변화
	const float			GetPhaseShiftMPConsume() const { return m_PhaseShiftData.m_fReturnMPConsume; }
	const bool			GetIsActivePhaseShift() const { return m_PhaseShiftData.GetIsActivePhaseShift(); }
	void				InitPhaseShift();

	/// 미립자 폭풍
	void				CreateDustStormToAttackResult( const CX2DamageManager::DamageData& damageData_ );
	void				CreateDustStormToDamageReact( const CX2DamageManager::DamageData& damageData_ );
	void				CreateDustStormDamageEffect( const float fDamageRate_, const D3DXVECTOR3& vPos_ );
	void				ProcessDustStormCoolTime();

	// 파티클 프리즘
	void				DestroyMinePrismNPC() const;
	CX2EffectSet::Handle GetParticlePrismHandle() const { return m_hParticlePrsim; }

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	// 초월자의 신체
	void				ProcessBodyOfTranscendence( IN OUT CX2DamageManager::DamageData* pDamageData_ );

	// 출력 교환기
	void				ProcessPowerExchanger();

	// 환상 추적자
	const bool			GetIsFantasyTracerTargetSkillID( IN const CX2SkillTree::SKILL_ID eSkillID_ )  const;
	void				SetUseFantasyTracerTarget( IN const CX2SkillTree::SKILL_ID eSkillID_ );
	void				ProcessFantasyTracerTargetInfo();

	void				SetDustAnatomyTarget( IN const CX2DamageManager::DamageData& pDamageData_ );

	// 리버스 리액터
	void				ProcessReverseReactorTargetInfo();
	virtual void		SetReverseReactorTransitionDamage( IN const float fFinalDamage_ );

	/// 퀵실버 프렌지
	void				ProcessQuicksilverFrenzy();
	void				AttackQuicksilverFrenzy( IN const CX2DamageManager::DamageData& pDamageData_, IN const bool bIsAttack_ = false );
	const D3DXCOLOR 	SetQuicksilverFrenzyFlickerColor( IN const D3DXCOLOR& coRenderColor_ );
#endif //SERV_ADD_LUNATIC_PSYKER

/// public Function End
	

/// protected Valuable Start

protected:

	const CX2SkillTree::SkillTemplet*	m_pSkillTemplet;
	int		m_iPressedSkillSlotIndex;
	float	m_fMPConsume;				/// MP 소모량
	float	m_fDPConsume;				/// DP 소모량
	int		m_iMutationCountConsume;	/// 변이 게이지 소모량

#ifdef ELSWORD_WAY_OF_SWORD
	int		m_iPreComboCount;
#endif // ELSWORD_WAY_OF_SWORD

	bool	m_bEnableAirDash;					/// 공중 대시 가능 여부
	bool	m_bEnableLevitationWait;			/// 공중 부양 가능 여부

	int		m_iComboLoopCount;					/// 반복 콤보에 대한 반복수

	float	m_fRenderParamColorA;				/// 캐릭터 알파값 설정

	float	m_fChargeTime;						/// 충전 시간
	int		m_iChargeCount;						/// 충전 횟수

	float	m_fChangeFormationCoolTime;			/// 구성 모드 전환 쿨타임

	float	m_fDPRateBySpecialActive;			/// 스페셜 액티브 공격일 때의 DP 획득량 배율 
	float	m_fDPRateByActive;					/// 액티브 공격일 때의 DP 획득량 배율
	float	m_fDPRateByCommand;					/// 커맨드 공격일 때의 DP 획득량 배율
	float	m_fDPRateByNasodArmor;				/// 나소드 아머 공격일 때의 DP 획득량 배율

	float	m_fDPRateByFormationModeAttack;		/// 구성모드중 타격시 DP 소모량 변동 배율
	float	m_fDPRateByFormationModeHit;		/// 구성모드중 피격시 DP 획득량 변동 배율
	float	m_fDPRateByFormationModeUseMP;		/// 구성모드중 MP 사용시 DP 획득량 변동 배율
	
	float	m_fDPRateByNoFormationModeAttack;	/// 구성모드가 아닐 때 타격시 DP 획득량 변동 배율
	float	m_fUseDPByNasodBarrier;				/// 나소드 배리어 발동시 DP 소모량 수치

	float	m_fUseDPByEnableFormationMode;		/// 구성 모드로 전환을 위해 보유해야 하는 최소 DP 수치
	float	m_fUseDPByFormationMode;			/// 구성 모드로 전환시 DP 소모량 수치

	bool	m_bPossibleNasodArmorModeState;		/// 나소드 배리어 발동 가능한 스테이트 인가

	bool	m_bSetUpDPByAttackThisFrame;		/// 타격시 한 프레임에 한번만 DP 획득 되도록 설정
	bool	m_bSetUpDPByDamageThisFrame;		/// 피격시 한 프레임에 한번만 DP 획득 되도록 설정


	// 파티클 프리즘
	CX2EffectSet::Handle m_hParticlePrsim;		/// 장막 몬스터 죽을 때 같이 죽이기 위해 핸들 저장

	// Loop 스테이트의 사운드 종료를 위한 변수
	CKTDXDeviceSound*	m_pLoopSound;

	/// 특수한 상황에서의 렌더 설정 상태
	CUSTOM_RENDER_STATE	m_eCustomRenderState;

//////////////////////////////////////////////////////////////////////////
// 패시브 스킬 관련
	// 다이너모 연구-해체
	float	m_fGainDPValueChangeRate;			/// 획득량 증가 배율
	float	m_fNotUsingDPSkillDamageRate;		/// 비 DP 스킬 데미지 배율

	// 다이너모 연구-구성
	float	m_fUsingDPValueChangeRate;			/// 소모량 감소 배율
	float	m_fUsingDPSkillDamageRate;			/// DP 스킬 데미지 배율

	// 나소드 아머 모드
	float	m_fNaCommandDamageRate;				/// 나소드 아머 커맨드 데미지 배율
	float	m_fDPValueRateByActiveFormationMode;/// 구성 모드로 전환 시 소모되는 DP량을 감소 시키는 배율

	// 무한의 도서관
	int		m_iLibraryOfLimitlessLevel;			/// 무한의 도서관 버프 적용을 위한 레벨 값
	
	// 미립자 폭풍
	float	m_fDustStormRateByHit;				///  타격시 발동 확률
	float	m_fDustStormRateByDamage;			///  파격시 발동 확률
	float	m_fDustStormCoolTime;				///  발동 쿨타임


#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

	// 초월자의 신체
	float	m_fBodyOfTranscendenceActiveRate;	/// 발동 확률
	float	m_fBodyOfTranscendenceMoveSpeed;	/// 이동 속도 증가치

	// 출력 교환기
	bool								m_bEnablePowerExchanger;				/// 적용 여부
	POWER_EXCHANGER_STATE				m_ePowerExchangerState;					/// 현재 상황

	/// 환상 추적자
	bool								m_bEnableFantasyTracer;					/// 적용 여부
	float								m_fFantasyTracerUpMPRateByFormationMode;/// 구성 모드 전환시 회복 MP 배율
	vector<FantasyTracerTargetInfo>		m_vecFantasyTracerTargetInfo;			/// 타겟 정보 컨테이너
	CX2SkillTree::SKILL_ID				m_eNasodArmorAttackActiveSkillID;		/// 나소드 아머 공격을 시전한 스킬 아이디
	CX2SkillTree::SKILL_ID				m_eNasodArmorLandingActiveSkillID;		/// 나소드 아머 착지을 시전한 스킬 아이디

#endif //SERV_ADD_LUNATIC_PSYKER

//////////////////////////////////////////////////////////////////////////

	NasodArmorAttackTargetData			m_NasodArmorAttackTargetData;		/// 나소드 아머 공격 타겟 정보

	/// 다이너모 구성 - 파일런
	float								m_fNonstopSummonPylonCoolTime;		/// 파일런 연속 소환 쿨타임
	UINT								m_uiNonstopSummonPylonCount;		/// 파일런 연속 소환 갯수
	vector<PylonLinkDamageEffectData>	m_vecPylonLinkDamageEffectData;		/// 다이너모 파일런 연결 데미지 이펙트 정보 컨테이너
	vector<PylonDamageEffectData>		m_vecSummonPylonDamageEffectData;	/// 소환된 다이너모 파일런 데미지 이펙트 정보 컨테이너

	/// 다이너모 구성 - 마그네트론
	MagnetronData						m_MagnetronData;					/// 마그네트론 관련 객체

	/// 스테이시스 필드
	StasisfieldData						m_StasisfieldData;					/// 스테이시스 필드 수치 관련 객체

	/// 위상 변화
	PhaseShiftData						m_PhaseShiftData;					/// 위상 변화 관련 객체

	/// 입자탄
	ChargedImpulsarData					m_ChargedImpulsarData;				/// 입자탄 - 과충전기 객체

	/// 펄스 캐논
	vector<int>							m_vecPulseBulletFireIndex;			/// 펄스 캐논 발사 인덱스 저장 객체

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 더스트 아나토미
	vector<DustAnatomyTargetInfo>		m_vecDustAnatomyTargetInfo;			/// 더스트 아나토미 연결 이펙트용 타겟 정보

	/// 리버스 리액터
	ReverseReactorTargetInfo			m_ReverseReactorTargetInfo;			/// 리버스 리액터 관련 객체

	/// 퀵실버 프렌지
	QuicksilverFrenzyData				m_QuicksilverFrenzyData;			/// 퀵실버 프렌지 관련 객체
	float								m_fQuicksilverFrenzyFlickerTime;	/// 반짝이는 시간

	/// 퀵실버 액셀
	CX2DamageEffect::CEffectHandle		m_eQuicksilverAccelLinkEffect;		/// 퀵실버 액셀 연결 이펙트

	/// 사이킥 스톰
	PsychicStormData					m_PsychicStormData;					/// 사이킥 스톰 관련 객체

	/// 둠스 데이
	vector<CX2DamageEffect::CEffectHandle> m_vecDoomsDayLinkEffect;			/// 연결 이펙트 저장 객체
#endif //SERV_ADD_LUNATIC_PSYKER

/// protected Valuable End

};

#endif // SERV_9TH_NEW_CHARACTER
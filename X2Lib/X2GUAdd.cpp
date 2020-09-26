#include "StdAfx.h"
#include "X2GUAdd.h"

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )

namespace _CONST_ADD_
{
	/// 콤보 XXZZ 반복수
	const float COMBO_XXZZ_MIN_COUNT						= 3.f;		/// 최소 반복수
	const float COMBO_XXZZ_MAX_COUNT						= 10.f;		/// 최대 반복수

	/// 대시 점프 콤보 XX 차지 시간
	const float DASH_JUMP_COMBO_X2_MAX_CHARGE_TIME			= 3.f;		/// 최대 차지 시간
	const float DASH_JUMP_COMBO_X2_MAX_REMAIN_TIME			= 3.f;		/// 최대 유지 시간

	/// 마그네트론
	const int	MAGNETRON_MAX_NUM							= 3;		/// 최대 개수
	const float	MAGNETRON_FIRE_READY_TIME					= 0.3f;		/// 발사 간격

	/// 펄스 캐논 
	const int	PULSE_CANNON_MIN_FIRE_COUNT					= 4;		/// 최소 발사수
	const int	PULSE_CANNON_MAX_FIRE_COUNT					= 8;		/// 최대 발사수
	const float	PULSE_CANNON_CONSUME_MP_VALUE				= 25.f;		/// 최대 발사수
	const float	PULSE_CANNON_CONSUME_DP_VALUE				= 500.f;	/// 최대 발사수
	
	/// 입자탄 관련 상수
	const float PULSE_BULLET_MAX_CHARGE_TIME				= 5.f;		/// 입자탄 5초간 유지 가능하지만, 충전 시간은 1초까지만 인정 해 줌
	const float PULSE_BULLET_MAX_APPLY_CHARGE_TIME			= 1.f;
	const float PULSE_BULLET_BASE_CURVE_SPEED				= 5.f;

	/// 파일런
	const float PYLON_REMAIN_TIME							= 20.f;		/// 유지 시간
	const float PYLON_NONESTOP_SUMMON_COOL_TIME				= 5.f;		/// 연속 소환 쿨타임
	const UINT  PYLON_NONESTOP_SUMMON_LIMIT_NUM				= 3;		/// 연속 소환 객체 제한 수

	/// EMP 쇼크 관련
	const float EMP_SHOCK_NEAR_RANGE						= 700.f;	/// 직접 타격 범위
	const float EMP_SHOCK_FAR_RANGE							= 1200.f;	/// 간접 타격 범위

	/// 공중 부양
	const float LEVITATION_CONSUME_MP_PER_SECOND			= 2.f;		/// 초당 MP 소모량

	/// 나소드 아머
	const float NASOD_ARMOR_TARGET_RANGE					= 1000.f;	/// 타겟팅 범위
	const float NASOD_ARMOR_TARGET_HEIGHT_RANGE				= 500.f;	/// 타겟팅 높이 범위

	/// 파티클 엑셀러레이터
	const int	PARTICLE_ACCELERATOR_MAX_USE_MUTATION_COUNT	= 3;		/// 최대 변이 게이지 소모수

	/// 퀀커러
	const int	CONQUEROR_MAX_USE_MUTATION_COUNT			= 3;		/// 최대 변이 게이지 소모수

	/// 보이드 브레이커
	const int	VOID_BREAKER_MAX_USE_MUTATION_COUNT			= 3;		/// 최대 변이 게이지 소모수
	
	/// 스테이시스 필드
	const float STASIS_FIELD_REMAIN_TIME					= 15.f;		/// 유지 시간

	/// 미립자 폭풍
	const float DUST_STORM_FIRE_COOL_TIME					= 0.1f;		/// 쿨타임
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 나소드 아머 콤보 ZZFrontZ_LOOP
	const int	NASOD_ARMOR_ZZFrontZ_LOOP_MIN_COUNT			= 1;		/// 최소 반복수
	const int	NASOD_ARMOR_ZZFrontZ_LOOP_MAX_COUNT			= 10;		/// 최대 반복수

	/// 나소드 아머 콤보 XDirectionX_LOOP
	const int	NASOD_ARMOR_XDirectionX_LOOP_MAX_COUNT		= 6;		/// 최대 반복수

	/// 환상 추적자
	const float	FANTASY_TRACER_TARGET_REMAIN_TIME			= 2.f;		/// 타겟 유지 시간
	const float	FANTASY_TRACER_TARGET_DELETE_READY_TIME		= 1.f;		/// 삭제 전 대기 시간

	/// 리버스 리액터
	const float	REVERSE_REACTOR_ENABLE_ATTACK_RANGE			= 1200.f;	/// 공격 범위
	const float	REVERSE_REACTOR_ENABLE_ATTACK_RANGE_PVP		= 700.f;	/// 공격 범위 ( 대전 )
	const float	REVERSE_REACTOR_LINK_RANGE					= 1200.f;	/// 연결 범위

	/// 퀵실버 프렌지
	const float	QUICKSILVER_FRENZY_FIRE_INTERVAL_TIME		= 0.3f;		/// 발사 간격
	const float	QUICKSILVER_FRENZY_FLICKER_INTERVAL_TIME	= 1.f;		/// 버프 적용시 반짝이는 간격

	/// 사이킥 스톰
	const float	PSYCHIC_STORM_DEFAULT_LOOP_TIME				= 0.8f;		/// 기본발동 시간
	const float	PSYCHIC_STORM_ATTACK_INTERVAL_TIME			= 0.105f;	/// 공격 간격
	const int	PSYCHIC_STORM_MAX_USE_MUTATION_COUNT		= 3;		/// 최대 변이 게이지 소모수
#endif //SERV_ADD_LUNATIC_PSYKER
}

static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] =
{
	{	/// 애드 나소드 룰러, 사이킥 트레이서
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Add.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Add.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Add.dds"),
	},

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	{	/// 애드 나소드 루나틱 사이커
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Add_ALP.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Add_ALP.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Add_ALP.dds"),
	}
#endif //SERV_ADD_LUNATIC_PSYKER
};

/*static */CX2GUAdd* CX2GUAdd::CreateGUAdd( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit )
{
	return new CX2GUAdd( unitIndex, teamNum, pScriptLuaState, frameBufferNum, pUnit );
};

CX2GUAdd::CX2GUAdd( int unitIndex, int teamNum, lua_State*   pScriptLuaState, int frameBufferNum, CX2Unit* pUnit )
: CX2GUUser( unitIndex, teamNum, pScriptLuaState, frameBufferNum, pUnit )
, m_iPressedSkillSlotIndex( -1 )
, m_bEnableAirDash( true )
, m_bEnableLevitationWait( true )
, m_fDPRateBySpecialActive( 1.f )
, m_fDPRateByActive( 1.f )
, m_fDPRateByCommand( 1.f )
, m_fDPRateByNasodArmor( 1.f )
, m_fDPRateByFormationModeAttack ( 0.f )
, m_fDPRateByFormationModeHit ( 0.f )
, m_fDPRateByFormationModeUseMP ( 0.f )
, m_fDPRateByNoFormationModeAttack ( 0.f )
, m_fUseDPByEnableFormationMode( 0.f )
, m_fUseDPByFormationMode ( 0.f )
, m_fUseDPByNasodBarrier ( 0.f )
, m_fGainDPValueChangeRate( 1.f )
, m_fUsingDPValueChangeRate( 1.f )
, m_fUsingDPSkillDamageRate( 1.f )
, m_fNotUsingDPSkillDamageRate( 1.f )
, m_iComboLoopCount( 0 )
, m_fChargeTime( 0.f )
, m_iChargeCount( 0 )
, m_fChangeFormationCoolTime( 0.f )
, m_fRenderParamColorA( 1.f )
, m_fNaCommandDamageRate( 1.f )
, m_fDPValueRateByActiveFormationMode( 1.f )
, m_iLibraryOfLimitlessLevel( 0 ) 
, m_fDustStormRateByHit(1.f)
, m_fDustStormRateByDamage(1.f)
, m_fDustStormCoolTime(0.f)
, m_bPossibleNasodArmorModeState( false )
, m_hParticlePrsim( INVALID_EFFECTSET_HANDLE )
, m_fMPConsume( 0.f )
, m_fDPConsume( 0.f )
, m_iMutationCountConsume( 0 )
, m_pLoopSound(NULL)
, m_bSetUpDPByAttackThisFrame( false )
, m_bSetUpDPByDamageThisFrame( false )
, m_fNonstopSummonPylonCoolTime( 0.f )
, m_uiNonstopSummonPylonCount( 0 )
, m_eCustomRenderState( CRS_NONE )
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
, m_fBodyOfTranscendenceActiveRate( 0.f )
, m_fBodyOfTranscendenceMoveSpeed( 1.f )
, m_bEnablePowerExchanger( false )
, m_ePowerExchangerState( PES_NONE )
, m_bEnableFantasyTracer( false )
, m_fFantasyTracerUpMPRateByFormationMode( 0.f )
, m_eNasodArmorAttackActiveSkillID( CX2SkillTree::SI_NONE )
, m_eNasodArmorLandingActiveSkillID( CX2SkillTree::SI_NONE )
, m_fQuicksilverFrenzyFlickerTime( 0.f )
#endif //SERV_ADD_LUNATIC_PSYKER
{
	m_fThrowStartTime		= 0.66f;
	m_vThrowStartPosOffset	= D3DXVECTOR3(40.f, 130.f, 0.f);
	m_vThrowStartVelocity	= D3DXVECTOR3(2000.f, 100.f, 0.f);

	m_vecPylonLinkDamageEffectData.resize( 0 );
	m_vecSummonPylonDamageEffectData.resize( 0 );

	/// 펄스 캐논 발사 인덱스 저장
	for( int i = 1; i <= 6; ++i )
	{
		m_vecPulseBulletFireIndex.push_back( i );
	}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	m_vecFantasyTracerTargetInfo.resize( 0 );
	m_vecDustAnatomyTargetInfo.resize( 0 );
#endif //SERV_ADD_LUNATIC_PSYKER

}

CX2GUAdd::~CX2GUAdd(void)
{
	StopLoopSound();
}

/*static*/ void CX2GUAdd::AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}

/*static*/ void CX2GUAdd::AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}

/*static*/ void CX2GUAdd::AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	CX2GUAdd::AppendMajorParticleToDeviceList( listInOut_ );
	CX2GUAdd::AppendMinorParticleToDeviceList( listInOut_ );
	CX2GameUnit::Weapon::AppendEnchantParticleForSwordToDeviceList( listInOut_, eDamageType_ );
}

/*virtual */bool CX2GUAdd::CanRidingState()
{
	if ( GetNowStateID() == CX2GUUser::USI_WAIT || GetNowStateID() == CX2GUUser::USI_WALK || 
		GetNowStateID() == CX2GUUser::USI_DASH || GetNowStateID() == CX2GUUser::USI_DASH_END || 
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT || GetNowStateID() == CX2GUUser::USI_RIDING_WALK ||
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT_HABIT || GetNowStateID() == CX2GUUser::USI_RIDING_DASH ||
		GetNowStateID() == CX2GUUser::USI_RIDING_DASH_END || GetNowStateID() == CX2GUAdd::ANSI_HYPER_WAIT )
		return true;

	return false;
}

/*virtual*/ void CX2GUAdd::Init( bool bUseTeamPos_, int iStartPosIndex_ )
{
	CX2GUUser::Init( bUseTeamPos_, iStartPosIndex_ );

	SetMaxSoul( MAX_DP_GAGE_VALUE );

	SetNeedToUpdateWeaponEffect( true );
	UpdateEquipCollisionData();

	/// 구성 모드 해제
	if ( NULL != GetAddGageData() )
		GetAddGageData()->SetIsFormationMode( false );

	/// DP, 구성 모드 구슬 개수, 변이 게이지 설정
	if ( IsMyUnit() )
	{
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				// TODO :  다른 유저들은 InitComponent 처리함.
				// 오현빈//빠른 처리를 위해 이렇게 작업 했지만,
				//		   SetHyperModeCount를 ADD에 맞게 수정해야 될 거라고 생각 함. 
				
				/// 분노의 반지 착용시, DP를 갖고 시작 한다.
				if( true == GetHasHyperModeItem() )
				{
					SetDPValue( 2500.f );
					SetHasHyperModeItem(false);
				}
				else
				{
					SetDPValue( 0.f );
				}

				SetMutationCount( 0 );
			} break;

		default:
			{
				if ( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
				{
					SetDPValue( 0.f );
					SetMutationCount( 0 );
				}
			} break;
		}

		/// DP 및 변이 수치 동기화
		m_FrameDataFuture.syncData.m_CannonBallCount = GetAbilitySyncData();
	}
}
/*virtual*/ void CX2GUAdd::ParseCommonRandomState()
{
	if( true == m_LuaManager.BeginTable( "INIT_COMMON_RANDOM_STATE" ) )
	{
		const char* tableName = "";
		switch( m_pUnit->GetClass() )
		{
		case CX2Unit::UC_ADD_NASOD_RULER:
			{
				tableName = "ADD_NASOD_RULER";
			} break;
		case CX2Unit::UC_ADD_PSYCHIC_TRACER:
			{
				tableName = "ADD_PSYCHIC_TRACER";
			} break;
	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		case CX2Unit::UC_ADD_LUNATIC_PSYKER:
			{
				tableName = "ADD_LUNATIC_PSYKER";
			} break;
	#endif //SERV_ADD_LUNATIC_PSYKER
		default:
			{
				ASSERT( !"NO" );
			} break;
		}

		CX2GUUser::ParseCommonRandomState(tableName);
		m_LuaManager.EndTable(); // INIT_COMMON_RANDOM_STATE
	}
}
/*virtual*/ void CX2GUAdd::InitState()
{
	switch ( GetUnitClass() )
	{
	case CX2Unit::UC_ADD_NASOD_RULER:
	case CX2Unit::UC_ADD_PSYCHIC_TRACER:
		{
			m_iSkillCutInSetIndex = 0;
		} break;
	default:
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
		{
			m_iSkillCutInSetIndex = 1;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
	case CX2Unit::UC_NONE:
		{
			ASSERT( !L"Unit Class Is fault!" );
			m_iSkillCutInSetIndex = 0;
		} break;
	}

	InitStateID();
	InitStateCommon();
	InitStateByUnitClass();

	std::string strStartTableNameUTF8;
	std::string strWinTableNameUTF8;
	std::string strLoseTableNameUTF8;

	InitStateCommonRandom( strStartTableNameUTF8, strWinTableNameUTF8, strLoseTableNameUTF8 );

	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_START;
	m_LuaManager.MakeTableReference( strStartTableNameUTF8.c_str(), stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WIN;
	m_LuaManager.MakeTableReference( strWinTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, WinStateStart );	
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_LOSE;
	m_LuaManager.MakeTableReference( strLoseTableNameUTF8.c_str(), stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, LoseStateStart );	
	m_StateList[stateData.stateID] = stateData;

	InitPassiveSkillState();
	InitEquippedActiveSkillState();

	CX2GUUser::InitState();
}
/*virtual*/ void CX2GUAdd::InitComponent()
{
	CX2GUUser::InitComponent();

	/// DP 획득 및 소모 관련 수치 설정
	if( m_LuaManager.BeginTable( "INIT_COMPONENT" ) == true )
	{
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_SPECIAL_ACTIVE",			m_fDPRateBySpecialActive,			0.0f );	/// 스페셜 액티브 공격일 때의 DP 변동량 배율 
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_ACTIVE",					m_fDPRateByActive,					0.0f );	/// 액티브 공격일 때의 DP 변동량 배율
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_COMMAND",					m_fDPRateByCommand,					0.0f );	/// 커맨드 공격일 때의 DP 변동량 배율
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_NASOD_ARMOR",				m_fDPRateByNasodArmor,				0.0f );	/// 나소드 아머 공격일 때의 DP 변동량 배율
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_FORMATION_MODE_ATTACK",	m_fDPRateByFormationModeAttack,		0.0f );	/// 구성모드중 타격시 DP 변동 배율
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_FORMATION_MODE_HIT",		m_fDPRateByFormationModeHit,		0.0f );	/// 구성모드중 피격시 DP 변동 배율
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_FORMATION_MODE_USE_MP",	m_fDPRateByFormationModeUseMP,		0.0f );	/// 구성모드중 MP 사용시 DP 변동 배율 ( 스페셜 액티브 이상 )
		LUA_GET_VALUE( m_LuaManager, "DP_RATE_BY_NO_FORMATION_MODE_ATTACK",	m_fDPRateByNoFormationModeAttack,	0.0f );	/// 구성모드 해제중 타격시 DP 변동 배율
		LUA_GET_VALUE( m_LuaManager, "USE_DP_BY_ENABLE_FORMATION_MODE",		m_fUseDPByEnableFormationMode,		0.0f );	/// 구성 모드 활성 가능 요구 DP 수치
		LUA_GET_VALUE( m_LuaManager, "USE_DP_BY_FORMATION_MODE",			m_fUseDPByFormationMode,			0.0f );	/// 구성 모드 전환시 소모 DP 수치
		LUA_GET_VALUE( m_LuaManager, "USE_DP_BY_NASOD_BARRIER",				m_fUseDPByNasodBarrier,				0.0f );	/// 나소드 배리어 발동시 소모 DP 수치
		LUA_GET_VALUE( m_LuaManager, "CHANGE_FORMATION_MODE_COOL_TIME",		m_fChangeFormationCoolTime,			0.0f );	/// 구성 모드 전환 쿨타임

		m_LuaManager.EndTable();
	}
}
/*virtual*/ void CX2GUAdd::InitPassiveSkillState()
{
	CX2GUUser::InitPassiveSkillState();
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
 	CX2SkillTree* const pSkillTree = g_pData->GetSkillTree();

	SetAddMagicAttackCriticalDamageRate(0.f);
	SetAddMagicAttackCriticalRate(0.f);
	SetAddAnimSpeedFactor(0.f);

	/// 기본 전직 공통 패시브
	//////////////////////////////////////////////////////////////////////////
	{
		/// 다이너모 구성 - 과충전기
		{
			m_ChargedImpulsarData.ResetChargedImpulsarData();

			const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_AN_CHARGED_IMPULSAR;
			const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
			if ( 0 < iSkillLevel )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
				if( NULL != pSkillTemplet )
				{
					m_ChargedImpulsarData.m_bEnableChargedImpulsar = true;		/// 과충전기 기능 활성

					m_ChargedImpulsarData.m_fIncreaseLifeTimeMaxRate	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillLevel );
					m_ChargedImpulsarData.m_fIncreaseScaleMaxRate		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillLevel );
					m_ChargedImpulsarData.m_fIncreaseCurveSpeedMaxRate	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_CURVE_SPEED_REL, iSkillLevel );
					m_ChargedImpulsarData.m_fIncreaseDamageRate			= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}

		/// 다이너모 연구 - 해체
		{
			m_fGainDPValueChangeRate = 1.f;
			m_fNotUsingDPSkillDamageRate = 1.f;

			const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_AN_RESEARCH_DYNAMO_DISSOLUTION;
			const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
			if ( 0 < iSkillLevel )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
				if( NULL != pSkillTemplet )
				{
					m_fGainDPValueChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DP_GAIN_REL, iSkillLevel );
					m_fNotUsingDPSkillDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}

		/// 다이너모 연구 - 구성
		{
			m_fUsingDPValueChangeRate = 1.f;
			m_fUsingDPSkillDamageRate = 1.f;

			const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_AN_RESEARCH_DYNAMO_COMPOSITION;
			const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
			if ( 0 < iSkillLevel )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
				if( NULL != pSkillTemplet )
				{
					m_fUsingDPValueChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DP_USE_REL, iSkillLevel );
					m_fUsingDPSkillDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
				}
			}
		}

		/// 위상 변화
		{
			m_PhaseShiftData.m_fReturnMPConsume = 0.f;
			m_PhaseShiftData.m_fEffectLifeTime = 0.f;

			const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_A_AN_PHASE_SHIFT;
			const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
			if ( 0 < iSkillLevel )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
				if( NULL != pSkillTemplet )
				{
					m_PhaseShiftData.m_fReturnMPConsume = 
						pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_BURN_ABS, iSkillLevel );

					m_PhaseShiftData.m_fEffectLifeTime = 
						pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillLevel );
				}
			}
		}

	}

	// 나소드 아머 모드
	m_fNaCommandDamageRate = 1.f;
	m_fDPValueRateByActiveFormationMode = 1.f;
	// 무한의 도서관
	m_iLibraryOfLimitlessLevel = 0;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 초월자의 신체
	m_fBodyOfTranscendenceActiveRate		= 0.f;		/// 발동 확률 초기화
	m_fBodyOfTranscendenceMoveSpeed			= 1.f;		/// 이동 속도 증가률 초기화

	/// 출력 교환기
	m_ePowerExchangerState					= PES_NONE;	/// 출력 교환기 상태 초기화
	m_bEnablePowerExchanger					= false;	/// 적용 여부 초기화

	/// 환상 추적자
	m_bEnableFantasyTracer					= false;	/// 적용 여부 초기화
	m_fFantasyTracerUpMPRateByFormationMode = 0.f;		/// 구성 모드 전환시 회복 MP 배율 초기화
#endif //SERV_ADD_LUNATIC_PSYKER

	switch( GetUnitClass() )
	{
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
		{
			/// 초월자의 신체
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_ALP_BODY_OF_TRANSCENDENCE;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet )
					{
						/// 발동 확률
						m_fBodyOfTranscendenceActiveRate	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel );

						/// 이동 속도 증가치
						m_fBodyOfTranscendenceMoveSpeed		= max( 1.f, pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MOVE_SPEED, iSkillLevel ) );
					}
				}
			}

			/// 출력 교환기
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_ALP_POWER_EXCHANGER;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					m_bEnablePowerExchanger = true;	/// 출력 교환기 적용 여부 설정
				}
				else	/// 만약 출력 교환기 버프 적용중에 스킬 초기화 할때를 대비하여, 버프도 같이 지워주자.
				{
					EraseBuffTempletFromGameUnit( BTI_BUFF_POWER_EXCHANGER_HALF_UP );
					EraseBuffTempletFromGameUnit( BTI_BUFF_POWER_EXCHANGER_HALF_DOWN );
				}
			}

			/// 환상 추적자
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_ALP_FANTASY_TRACER;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet )
					{
						m_bEnableFantasyTracer					= true;	/// 환상 추적자 적용 여부 설정
						/// 구성 모드 전환시 회복 MP 배율 설정
						m_fFantasyTracerUpMPRateByFormationMode = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel );
					}
				}
			}
		}/// 1차 전직 패시브를 검사하기 위해, break 구문 제거

#endif //SERV_ADD_LUNATIC_PSYKER

	case CX2Unit::UC_ADD_PSYCHIC_TRACER:
		{
			/// 나소드 아머 모드
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_APT_NASOD_ARMOR_MODE;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet )
					{
						m_fNaCommandDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
						m_fDPValueRateByActiveFormationMode = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DP_USE_REL, iSkillLevel );
					}
				}
			}

			/// 무한의 도서관
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_APT_LIBRARY_OF_LIMITLESS;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				m_iLibraryOfLimitlessLevel = iSkillLevel;
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet )
					{
						// m_SkillRelatedData의 초기화는 CX2GUUser::InitPassiveSkillState()에서 수행.
						const float fAddForceDownRate = CalculateIncreasingRate(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_REL, iSkillLevel ));
						m_SkillRelatedData.SetForceDownValueRateOfMeleeAttack( m_SkillRelatedData.GetForceDownValueRateOfMeleeAttack() + fAddForceDownRate );
						m_SkillRelatedData.SetForceDownValueRateOfRangeAttack( m_SkillRelatedData.GetForceDownValueRateOfRangeAttack() + fAddForceDownRate );
					}
				}
			}

			/// 미립자 폭풍
			{
				const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_P_APT_DUST_STORM;
				const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
				if ( 0 < iSkillLevel )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
					if( NULL != pSkillTemplet )
					{
						InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUAdd, CreateDustStormToAttackResult ) );
						InsertDelegateProcessWithDamageDataToDamageReact( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUAdd, CreateDustStormToDamageReact ) );

						m_fDustStormRateByHit = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_MAGIC_FIRST_REL, iSkillLevel );
						m_fDustStormRateByDamage = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_MAGIC_SECOND_REL, iSkillLevel );
					}
				}
			}
		} break;
	default:
		break;
	}
}
/*virtual*/ void CX2GUAdd::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ )
{
	if ( iSlotIndex_ < 0 || iSlotIndex_ >= ARRAY_SIZE( _CONST_GUUSER_::aNormalSkillStateID ) )
		return;

	int iNormalStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aNormalSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aNormalSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& normalStateData = m_StateList[ iNormalStateID ];
	normalStateData.Init();
	normalStateData.stateID = iNormalStateID;

	int iHyperStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aHyperSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aHyperSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& hyperStateData  = m_StateList[ iHyperStateID ];
	hyperStateData.Init();	
	hyperStateData.m_bHyperState = true;
	hyperStateData.stateID = iHyperStateID;

	const CX2SkillTree::SkillTemplet* pSkillTempletEquipped = GetEquippedActiveSkillTemplet( eSkillID_ );
	if( NULL == pSkillTempletEquipped )
		return;

	/// 모든 캐릭터 공통 스킬 설정
	//////////////////////////////////////////////////////////////////////////
	switch( pSkillTempletEquipped->m_eType )
	{	
	case CX2SkillTree::ST_BUFF:
		{	// 버프
			m_LuaManager.MakeTableReference( "ANSI_COMMON_BUFF", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, COMMON_BUFF_FrameMove );	
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	

			m_LuaManager.MakeTableReference( "ANSI_COMMON_BUFF", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

			return;
		} break;
	case CX2SkillTree::ST_ACTIVE:
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
		{
			// ST_ACTIVE, ST_SPECIAL_ACTIVE는 아래 구문에서 수행
		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
		{
			switch( eSkillID_ )
			{
			case CX2SkillTree::SI_ETC_WS_COMMON_LOVE:
				{
					m_LuaManager.MakeTableReference( "ANSI_THROW_ITEM", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	

					m_LuaManager.MakeTableReference( "ANSI_THROW_ITEM", hyperStateData.stateID );
					hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
					hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
				} break;
			}
			return;
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		{
			return;
		} break;
	}

	switch ( eSkillID_ )
	{
	/// 스페셜 액티브
	//////////////////////////////////////////////////////////////////////////

	/// 보이드 브레이커
	case CX2SkillTree::SI_SA_AN_VOID_BREAKER:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_AN_VOID_BREAKER", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_VOID_BREAKER_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_VOID_BREAKER_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_VOID_BREAKER_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_AN_VOID_BREAKER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

		/// 파티클 프리즘
	case CX2SkillTree::SI_SA_AN_PARTICLE_PRISM:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_AN_PARTICLE_PRISM", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_PARTICLE_PRISM_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_PARTICLE_PRISM_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_PARTICLE_PRISM_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_AN_PARTICLE_PRISM", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	/// 에너지 붐
	case CX2SkillTree::SI_SA_AN_ENERGY_BOOM:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_AN_ENERGY_BOOM", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_ENERGY_BOOM_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_ENERGY_BOOM_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_ENERGY_BOOM_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_AN_ENERGY_BOOM", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	/// 파티클 액셀러레이터
	case CX2SkillTree::SI_SA_AN_PARTICLE_ACCELERATOR:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_AN_PARTICLE_ACCELERATOR", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_PARTICLE_ACCELERATOR_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_PARTICLE_ACCELERATOR_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_PARTICLE_ACCELERATOR_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_AN_PARTICLE_ACCELERATOR", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	/// 뉴트란 밤
	case CX2SkillTree::SI_SA_AN_NEUTRON_BOMB:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_AN_NEUTRON_BOMB", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_NEUTRON_BOMB_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_AN_NEUTRON_BOMB_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_AN_NEUTRON_BOMB", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	/// 펄스 캐논
	case CX2SkillTree::SI_SA_APT_PULSE_CANNON:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_APT_PULSE_CANNON_READY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_PULSE_CANNON_READY_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_PULSE_CANNON_READY_StateStartFuture );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_PULSE_CANNON_READY_StateStart );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_PULSE_CANNON_READY_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_APT_PULSE_CANNON_READY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= ANSI_SA_APT_PULSE_CANNON_LOOP;
				m_LuaManager.MakeTableReference( "ANSI_SA_APT_PULSE_CANNON_LOOP", stateData.stateID );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_PULSE_CANNON_LOOP_FrmaeMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_PULSE_CANNON_LOOP_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID				= ANSI_SA_APT_PULSE_CANNON_FINISH;
				m_LuaManager.MakeTableReference( "ANSI_SA_APT_PULSE_CANNON_FINISH", stateData.stateID );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_PULSE_CANNON_FINISH_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

	/// EMP 쇼크
	case CX2SkillTree::SI_SA_APT_EMP_SHOCK:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_APT_EMP_SHOCK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_EMP_SHOCK_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_EMP_SHOCK_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_EMP_SHOCK_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_APT_EMP_SHOCK", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	/// 퀘이크 버스터
	case CX2SkillTree::SI_SA_APT_QUAKE_BUSTER:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_APT_QUAKE_BUSTER", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_QUAKE_BUSTER_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_QUAKE_BUSTER_EventProcess );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_QUAKE_BUSTER_FrameMove );

			m_LuaManager.MakeTableReference( "ANSI_SA_APT_QUAKE_BUSTER_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_QUAKE_BUSTER_HYPER_EventProcess );
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
		} break;

	/// 스테이시스 필드
	case CX2SkillTree::SI_SA_APT_STASIS_FIELD:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_APT_STASIS_FIELD", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_STASIS_FIELD_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_STASIS_FIELD_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_STASIS_FIELD_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_STASIS_FIELD_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_APT_STASIS_FIELD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= ANSI_SA_APT_STASIS_FIELD_ACTIVE;
				m_LuaManager.MakeTableReference( "ANSI_SA_APT_STASIS_FIELD_ACTIVE", stateData.stateID );
				stateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_STASIS_FIELD_ACTIVE_StateStart );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_STASIS_FIELD_ACTIVE_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_STASIS_FIELD_ACTIVE_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

	/// 퀀커러
	case CX2SkillTree::SI_SA_APT_CONQUEROR:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_APT_CONQUEROR", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_CONQUEROR_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_CONQUEROR_EventProcess );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_APT_CONQUEROR_FrameMove );

			m_LuaManager.MakeTableReference( "ANSI_SA_APT_CONQUEROR_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
		} break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		/// 리버스 리액터
	case CX2SkillTree::SI_SA_ALP_REVERSE_REACTOR:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_REVERSE_REACTOR", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_REVERSE_REACTOR_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_REVERSE_REACTOR_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_REVERSE_REACTOR_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_REVERSE_REACTOR_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_REVERSE_REACTOR", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

		/// 퀵실버 액셀
	case CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_QUICKSILVER_ACCEL", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_ACCEL_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_ACCEL_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_ACCEL_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_ACCEL_StateEnd );

			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_QUICKSILVER_ACCEL", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEnd				= normalStateData.StateEnd;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK;
				m_LuaManager.MakeTableReference( "ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK", stateData.stateID );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_EventProcess );
				stateData.StateEnd				= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_StateEnd );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

		/// 퀵실버 프렌지
	case CX2SkillTree::SI_SA_ALP_QUICKSILVER_FRENZY:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_QUICKSILVER_FRENZY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_FRENZY_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_FRENZY_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_QUICKSILVER_FRENZY_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_QUICKSILVER_FRENZY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

		/// 더스트 아나토미
	case CX2SkillTree::SI_SA_ALP_DUST_ANATOMY:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_DUST_ANATOMY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_DUST_ANATOMY_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_DUST_ANATOMY_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_DUST_ANATOMY_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_DUST_ANATOMY_StateEnd );

			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_DUST_ANATOMY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEnd				= normalStateData.StateEnd;
		} break;

		/// 사이킥 스톰
	case CX2SkillTree::SI_SA_ALP_PSYCHIC_STORM:
		{
			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_PSYCHIC_STORM", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_StateStartFuture );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_StateStart );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_SA_ALP_PSYCHIC_STORM", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= ANSI_SA_ALP_PSYCHIC_STORM_LOOP;
				m_LuaManager.MakeTableReference( "ANSI_SA_ALP_PSYCHIC_STORM_LOOP", stateData.stateID );
				stateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_LOOP_StateStart );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_LOOP_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_LOOP_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID				= ANSI_SA_ALP_PSYCHIC_STORM_FINISH;
				m_LuaManager.MakeTableReference( "ANSI_SA_ALP_PSYCHIC_STORM_FINISH", stateData.stateID );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_FINISH_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_SA_ALP_PSYCHIC_STORM_FINISH_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}

		} break;
#endif //SERV_ADD_LUNATIC_PSYKER


	/// 하이퍼 액티브
	//////////////////////////////////////////////////////////////////////////
		/// 퀵실버 프렌지
	case CX2SkillTree::SI_HA_ALP_DOOMS_DAY:
		{
			m_LuaManager.MakeTableReference( "ANSI_HA_ALP_DOOMS_DAY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_HA_ALP_DOOMS_DAY_Init );
			
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_HA_ALP_DOOMS_DAY_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_HA_ALP_DOOMS_DAY_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_HA_ALP_DOOMS_DAY_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, ANSI_HA_ALP_DOOMS_DAY_StateEnd );

			m_LuaManager.MakeTableReference( "ANSI_HA_ALP_DOOMS_DAY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEnd				= normalStateData.StateEnd;
		} break;


	/// 액티브
	//////////////////////////////////////////////////////////////////////////

	/// 입자탄
	case CX2SkillTree::SI_A_AN_PULSE_BULLET:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_AN_PULSE_BULLET", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_AN_PULSE_BULLET", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY;
				m_LuaManager.MakeTableReference( "ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY", stateData.stateID );
				stateData.StateStartFuture		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_StateStartFuture );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_EventProcess );
				stateData.StateEndFuture		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_StateEndFuture );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID				= ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE;
				m_LuaManager.MakeTableReference( "ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE", stateData.stateID );
				stateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_StateStart );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID				= ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE;
				m_LuaManager.MakeTableReference( "ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE", stateData.stateID );
				stateData.StateStartFuture		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_StateStartFuture );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

		/// 정신 붕괴
	case CX2SkillTree::SI_A_AN_MIND_BREAK:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_AN_MIND_BREAK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_MIND_BREAK_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_MIND_BREAK_FrmaeMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_MIND_BREAK_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_AN_MIND_BREAK", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

		/// 위상 변화
	case CX2SkillTree::SI_A_AN_PHASE_SHIFT:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_AN_PHASE_SHIFT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PHASE_SHIFT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PHASE_SHIFT_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PHASE_SHIFT_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_AN_PHASE_SHIFT", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= ANSI_A_AN_PHASE_SHIFT_BLINK;
				m_LuaManager.MakeTableReference( "ANSI_A_AN_PHASE_SHIFT_BLINK", stateData.stateID );
				stateData.StateStart			= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PHASE_SHIFT_BLINK_StateStart );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_AN_PHASE_SHIFT_BLINK_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

		/// 다이너모 구성 - 마그네트론
	case CX2SkillTree::SI_A_APT_MAGNETRON_DYNAMO:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_APT_MAGNETRON_DYNAMO", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_MAGNETRON_DYNAMO_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_MAGNETRON_DYNAMO_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_MAGNETRON_DYNAMO_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_APT_MAGNETRON_DYNAMO", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

		/// 다이너모 구성 - 디졸버
	case CX2SkillTree::SI_A_APT_DESOLVER_DYNAMO:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_APT_DESOLVER_DYNAMO", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_DESOLVER_DYNAMO_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_DESOLVER_DYNAMO_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_APT_DESOLVER_DYNAMO_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_DESOLVER_DYNAMO_EventProcess );
		} break;

		/// 다이너모 구성 - 파일런
	case CX2SkillTree::SI_A_APT_PYLON_DYNAMO:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_APT_PYLON_DYNAMO", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_PYLON_DYNAMO_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_PYLON_DYNAMO_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_APT_PYLON_DYNAMO_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_APT_PYLON_DYNAMO", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		/// 사이오닉 비트
	case CX2SkillTree::SI_A_ALP_PSIONIC_BEAT:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_ALP_PSIONIC_BEAT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_ALP_PSIONIC_BEAT_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_ALP_PSIONIC_BEAT_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_ALP_PSIONIC_BEAT", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

		/// 트월 러시
	case CX2SkillTree::SI_A_ALP_TWIRL_RUSH:
		{
			m_LuaManager.MakeTableReference( "ANSI_A_ALP_TWIRL_RUSH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUAdd, ANSI_A_ALP_TWIRL_RUSH_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUAdd, ANSI_A_ALP_TWIRL_RUSH_EventProcess );

			m_LuaManager.MakeTableReference( "ANSI_A_ALP_TWIRL_RUSH_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
	}
}

/** @function	: SetEquippedSkillLevel
	@brief		: 스킬 내 추가되는 스테이트에 대한 스킬 레벨 연동
	@param		: 스킬 아이디, 모든 스테이트 연동 여부
*/
/*virtual*/ void CX2GUAdd::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
	/// 스테이시스 필드
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_APT_STASIS_FIELD == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_APT_STASIS_FIELD );

		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_SA_APT_STASIS_FIELD_ACTIVE );
		}
	}

	/// 위상 변화
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AN_PHASE_SHIFT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AN_PHASE_SHIFT );

		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_A_AN_PHASE_SHIFT_BLINK );
		}
	}

	/// 입자탄
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AN_PULSE_BULLET == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AN_PULSE_BULLET );

		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY );
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE );
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE );
		}
	}

	/// 펄스 캐논
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_APT_PULSE_CANNON == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_APT_PULSE_CANNON );

		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_SA_APT_PULSE_CANNON_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_SA_APT_PULSE_CANNON_FINISH );
		}
	}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 퀵실버 액셀
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL );

		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK );
		}
	}

	/// 사이킥 스톰
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALP_PSYCHIC_STORM == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALP_PSYCHIC_STORM );

		if( NULL != pSkillTemplet )
		{
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_SA_ALP_PSYCHIC_STORM_LOOP );
			SetEquippedSkillLevelStateData( pSkillTemplet, ANSI_SA_ALP_PSYCHIC_STORM_FINISH );
		}
	}
#endif //SERV_ADD_LUNATIC_PSYKER
}
			 
/*virtual*/ void CX2GUAdd::CommonStateStartFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateStartFuture();

	/// 나소드 배리어를 시전할 수 있는지 여부
	LUA_GET_VALUE( m_LuaManager, "POSSIBLE_NASOD_BARRIER", m_bPossibleNasodArmorModeState, false );

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 나소드 배리어를 시전할 수 있는 클래스 인지 여부
	if ( true == m_LuaManager.BeginTable( "POSSIBLE_NASOD_BARRIER_BY_UNIT_CLASS" ) )
	{
		int	iIndex		= 1;
		int	iUnitClass	= 0;

		while( true == m_LuaManager.GetValue( iIndex, iUnitClass ) )
		{
			CX2Unit::UNIT_CLASS eUnitClass = static_cast<CX2Unit::UNIT_CLASS>( iUnitClass );

			if ( GetUnitClass() == eUnitClass )
			{	
				m_bPossibleNasodArmorModeState = true;
				break;
			}

			++iIndex;
		}

		m_LuaManager.EndTable();
	}
#endif //SERV_ADD_LUNATIC_PSYKER
}
/*virtual*/ void CX2GUAdd::CommonStateStart()
{
	CX2GUUser::CommonStateStart();
}
			 
/*virtual*/ void CX2GUAdd::CommonFrameMoveFuture()
{
	KTDXPROFILE();

	/// 땅에 닿았다면, 공중 대시 초기화
	if( true == IsOnSomethingFuture() && false == m_bEnableAirDash )
		m_bEnableAirDash = true;

	/// 땅에 닿았다면, 공중 부양 초기화
	if( true == IsOnSomethingFuture() && false == m_bEnableLevitationWait )
		m_bEnableLevitationWait = true;

	/// DP 및 변이 수치 동기화
	m_FrameDataFuture.syncData.m_CannonBallCount = GetAbilitySyncData();

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 환상 추적자 타겟 정보 갱신
	ProcessFantasyTracerTargetInfo();
#endif //SERV_ADD_LUNATIC_PSYKER

	CX2GUUser::CommonFrameMoveFuture();
}

/*virtual*/ void CX2GUAdd::CommonFrameMove()
{
	KTDXPROFILE();
	CX2GUUser::CommonFrameMove();

	/// 타 클라이언트의 DP 및 변이 수치 동기화
	if ( false == IsMyUnit() )
	{
		SetAbilitySyncData( static_cast<USHORT>( m_FrameDataNow.syncData.m_CannonBallCount ) );
	}

	/// 다이너모 구성 - 파일런 유지시간 연산
	ProcessPylonDamageEffect();
	/// 다이너모 구성 - 파일런 연속 소환 관련 쿨타임 제어
	ProcessNonstopSummonPylon();

	/// 다이너모 구성 - 마그네트론 위치 제어 함수
	ProcessMagnetronDamageEffect();

	/// 스테이시스 필드 쿨타임 제어 함수
	ProcessStasisField();

	/// 미립자 폭풍 쿨타임 연산
	ProcessDustStormCoolTime();

	/// 프레임당 한번의 DP 회복 및 소모 처리 Flag
	SetUpDPByAttackThisFrame( false );
	SetUpDPByDamageThisFrame( false );

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 출력 교환기 DP량 검사 및 버프 적용 함수
	ProcessPowerExchanger();

	/// 리버스 리액터 기능 갱신 함수
	ProcessReverseReactorTargetInfo();

	/// 퀵실버 프렌지 발사 간격 갱신 함수
	ProcessQuicksilverFrenzy();
#endif //SERV_ADD_LUNATIC_PSYKER
}
			 
/*virtual*/ void CX2GUAdd::CommonStateEndFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEndFuture();
}

/*virtual*/ void CX2GUAdd::CommonStateEnd()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEnd();
}

/*virtual*/ void CX2GUAdd::CommonEventProcess()
{
	KTDXPROFILE();

	/// 구성 모드일 때 DP 게이지가 0이 되면,  구성 모드 풀어 주세요.
	if ( true == GetIsFormationMode() && 0.f >= GetDPValue() )
	{
		SetIsFormationMode( false );
		m_FrameDataFuture.syncData.nowAction = UAI_RELEASE_FORMATION_MODE;
	}

	CX2GUUser::CommonEventProcess();
}

/*virtual*/ void CX2GUAdd::DetonationStateChange( CX2DamageManager::DamageData* pDamageData )
{
	CX2GUUser::DetonationStateChange( pDamageData );
}
			 
/*virtual*/ RENDER_HINT	CX2GUAdd::CommonRender_Prepare()
{
	KTDXPROFILE();

	return CX2GUUser::CommonRender_Prepare();
}
			 
/*virtual*/ bool CX2GUAdd::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ /*= CX2SkillTree::ASUT_GROUND */)
{
	KTDXPROFILE();

	if ( false == CanUseSpecialAttack( eActiveSkillUseCondition_ ) )
		return false;

	if( NULL == m_pSkillTemplet )
	{
		return false;
	}
	else
	{
		StateChangeSpecialAttack( m_pSkillTemplet );
	}

	return true;

}
/** @function	: NoStateChangeActionFrameMove
	@brief		: 스테이트의 변화가 없는 스킬 사용 시 수행되는 FrameMove
*/
/*virtual*/ void CX2GUAdd::NoStateChangeActionFrameMove()
{
	switch( m_FrameDataNow.syncData.nowAction )
	{
	/// 애드 구성 모드 해제
	case UAI_RELEASE_FORMATION_MODE:
		{
			EraseBuffTempletFromGameUnit( BTI_FORMATION_MODE );
			EraseBuffTempletFromGameUnit( BTI_BUFF_LIBRARY_OF_LIMITLESS_MANA_RECOVERY );
			SetRemainHyperModeTime( 0.f );
			/// UpMutationCount( 1 );	해제시 변이 포인트 획득 제거

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			/// 초월자의 신체 - 구성 모드중 이동 속도 증가 해제
			m_AdditionalWalkSpeed.SetPassiveMultiple( m_AdditionalWalkSpeed.GetPassiveMultiple() / m_fBodyOfTranscendenceMoveSpeed );
			m_AdditionalDashSpeed.SetPassiveMultiple( m_AdditionalDashSpeed.GetPassiveMultiple() / m_fBodyOfTranscendenceMoveSpeed );
#endif //SERV_ADD_LUNATIC_PSYKER
		} break;
	case UAI_ACTIVE_STASIS_FIELD:
		{
			if ( true == IsMyUnit() && true == m_StasisfieldData.GetIsActiveStasisfield() )
				StateChange( ANSI_SA_APT_STASIS_FIELD_ACTIVE );
		} break;
	default:
		break;
	}
}

/*virtual*/ float CX2GUAdd::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = 0.f;

	/// 비각성시 MP를 쓰고, 각성시 DP를 쓰는 스킬이 아닐때만 MP 소모 설정
	const float fUseDPSkillAtFormationMode = GetSpecialAbilityValue( CX2SkillTree::SA_DP_USE_REL, eSkillID_ );

	if ( 0.f >= fUseDPSkillAtFormationMode || false == GetIsFormationMode() )
	{
		fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );

		switch ( eSkillID_ )
		{
		case CX2SkillTree::SI_SA_APT_STASIS_FIELD:			/// 스테이시스 필드
			{
				/// 스테이시스 필드 동작중이면, MP 소모 없이 방출 스테이트로 전환 하자
				if ( true == m_StasisfieldData.GetIsActiveStasisfield() )
					fMpConsumption = 0;
			} break;
		case CX2SkillTree::SI_A_AN_PHASE_SHIFT:				/// 위상 변화
			{
				if ( true == m_PhaseShiftData.IsValidReturnPosValue() )			/// 위상 변화 복귀시, 일정 MP를 소모 하자
					fMpConsumption = GetPhaseShiftMPConsume();
	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환	
				/// 환상 추적자 발동시, MP 소모를 없애자.
				else if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PHASE_SHIFT ) )
					fMpConsumption = 0.f;
	#endif //SERV_ADD_LUNATIC_PSYKER
			} break;

	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		case CX2SkillTree::SI_A_AN_PULSE_BULLET:			/// 입자탄
			{
				/// 환상 추적자 발동시, MP 소모를 없애자.
				if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PULSE_BULLET ) )
					fMpConsumption = 0.f;
			} break;

		case CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL:		/// 퀵실버 액셀
			{
				/// 퀵실버 액셀 후진시에는 MP 10만 소비
				if ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == GetFutureStateSkillID())
					fMpConsumption = 10.f;
			} break;
	#endif //SERV_ADD_LUNATIC_PSYKER
		}
	}

	return fMpConsumption;
}
float CX2GUAdd::GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel ) const
{
	if( NULL == pSkillTemplet_ )
		return 0.f;

	float fSkillCoolTime = pSkillTemplet_->GetSkillCoolTimeValue( iSkillLevel ) ;

	switch ( pSkillTemplet_->m_eID )
	{
	case CX2SkillTree::SI_SA_APT_STASIS_FIELD:			/// 스테이시스 필드
		{
			/// 스테이시스 필드 동작중
			if ( true == m_StasisfieldData.GetIsActiveStasisfield() )
				return 0.f;
		} break;
	case CX2SkillTree::SI_A_AN_PHASE_SHIFT:				/// 위상 변화
		{
			/// 위상 변화 복귀시
			if ( true == m_PhaseShiftData.IsValidReturnPosValue() )
				return 0.f;
	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			/// 환상 추적자 발동
			else if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PHASE_SHIFT ) )
				return 0.f;
	#endif //SERV_ADD_LUNATIC_PSYKER
		} break;
	case CX2SkillTree::SI_A_APT_PYLON_DYNAMO:			/// 다이너모 구성 - 파일런
		{
			/// 연속 소환 가능한 조건이라면, 쿨타임 설정 않함
			if ( true == GetPossibleNonstopSummonPylon() )
				return 0.f;
		} break;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2SkillTree::SI_A_AN_PULSE_BULLET:			/// 입자탄
		{
			/// 환상 추적자 발동
			if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PULSE_BULLET ) )
				return 0.f;
		} break;

	case CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL:		/// 퀵실버 액셀
		{
			/// 퀵실버 액셀 후진시에는 쿨타임 적용 하지 않는다.
			if ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == GetFutureStateSkillID() )
				return 0.f;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
	default:
		break;
	}

	return max( 0.f, fSkillCoolTime );
}
/*virtual*/ CX2GageData* CX2GUAdd::CreateGageData()
{
	if ( true == IsMyUnit() )
	{
		const CX2AddGageData* pAddrGageData = 
			static_cast<const CX2AddGageData*>( CX2GageManager::GetInstance()->GetMyGageData() );

		if ( NULL != pAddrGageData )
			return new CX2AddGageData( *pAddrGageData );
		else
			return new CX2AddGageData();
	}
	else
		return new CX2AddGageData();
}

/*virtual*/	bool CX2GUAdd::ShouldResetNotBusyTimer()
{	
	if ( false == CX2GUUser::ShouldResetNotBusyTimer() )
		return false;
	else if ( ANSI_HYPER_WAIT == m_FrameDataFuture.syncData.nowState )
		return false;
	else if ( ANSI_LEVITATION_WAIT == m_FrameDataFuture.syncData.nowState )
		return false;

	return true; // 바쁜 상태 라면 리셋 해야 함
}

void CX2GUAdd::InitStateID()
{
	m_CommonState.m_Wait	= USI_WAIT;
}

/** @function	: InitStateCommon
	@brief		: 모든 캐릭터가 공통적으로 쓰는 상태 및 전직에 상관없이 공통으로 쓰는 상태 초기화
*/
void CX2GUAdd::InitStateCommon()
{
	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "ANSI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DieBackEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "ANSI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "ANSI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, WaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, WaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, WaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_HYPER_WAIT;
	m_LuaManager.MakeTableReference( "ANSI_HYPER_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, HyperWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, HyperWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, HyperWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "ANSI_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, WalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "ANSI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, JumpReadyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "ANSI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, JumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ANSI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, JumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ANSI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, JumpLandingStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, JumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "ANSI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "ANSI_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_AIR_DASH;
	m_LuaManager.MakeTableReference( "ANSI_AIR_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, AirDashEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, AirDashStateEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAdd, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_LEVITATION_WAIT;
	m_LuaManager.MakeTableReference( "ANSI_LEVITATION_WAIT", stateData.stateID );
	stateData.OnFrameMove	= SET_CB_FUNC( CX2GUAdd, LevitationWaitFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, LevitationWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ANSI_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, FormationModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DamageDownFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DamageDownFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DamageDownBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DamageDownBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DamageAirDownLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "ANSI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "ANSI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "ANSI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "ANSI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "ANSI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "ANSI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ANSI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;


#ifdef RIDING_SYSTEM
	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "ANSI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOnEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
#ifdef FIX_SCORPION_SPECIAL_SKILL_BUG
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
#endif // FIX_SCORPION_SPECIAL_SKILL_BUG
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_SPECIAL_MOVE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEndFutrue );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEnd );
	m_StateList[stateData.stateID] = stateData;
#endif //RIDING_SYSTEM

#ifdef MODIFY_RIDING_PET_AWAKE
	stateData.Init();
	stateData.stateID			= USI_RIDING_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ANSI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, FormationModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, RidingHyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingHyperModeEventProcess );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingHyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingHyperModeEnd );
	m_StateList[stateData.stateID] = stateData;
#endif // MODIFY_RIDING_PET_AWAKE

	stateData.Init();
	stateData.stateID			= ANSI_STAND_UP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, StandUpRollingFrontFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, StandUpRollingFrontFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_STAND_UP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "ANSI_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, StandUpRollingFrontBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, StandUpRollingFrontBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_STAND_UP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, StandUpRollingBackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, StandUpRollingBackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_STAND_UP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "ANSI_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, StandUpRollingBackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, StandUpRollingBackBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_STAND_UP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "ANSI_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, StandUpAttackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, StandUpAttackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_STAND_UP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "ANSI_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, StandUpAttackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, StandUpAttackBackEnd );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, StandUpAttackBackStateEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DashJumpPowerLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DashJumpPowerLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpPowerLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "ANSI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;

//=====================================================================================
// 공격 스테이트

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_Z;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_ZZZZ;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_ZZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboZZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_ZZZX;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_ZZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboZZZXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_ZZX;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_ZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboZZXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_ZZXX;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_ZZXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboZZXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_X;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_XX;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_XXX;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboXXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_XXZ;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_XXZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboXXZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_XXZZ_START;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_XXZZ_START", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboXXZZStartEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, ComboXXZZStartStateEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_XXZZ_LOOP;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_XXZZ_LOOP", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, ComboXXZZLoopStateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboXXZZLoopEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, ComboXXZZLoopStateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_COMBO_XXZZ_FINISH;
	m_LuaManager.MakeTableReference( "ANSI_COMBO_XXZZ_FINISH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, ComboXXZZFinishStateStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, ComboXXZZFinishStateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, ComboXXZZFinishEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ANSI_JUMP_ATTACK_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAdd, JumpComboZEventMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, JumpComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "ANSI_JUMP_ATTACK_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAdd, JumpComboXEventMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, JumpComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "ANSI_DASH_COMBO_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAdd, DashComboZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ANSI_DASH_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ANSI_DASH_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_COMBO_X;
	m_LuaManager.MakeTableReference( "ANSI_DASH_COMBO_X", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DashComboXStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DashComboXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashComboXEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, DashComboXStateEnd );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, DashComboXStateEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_COMBO_XX;
	m_LuaManager.MakeTableReference( "ANSI_DASH_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashComboXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_COMBO_XXX;
	m_LuaManager.MakeTableReference( "ANSI_DASH_COMBO_XXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashComboXXXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_COMBO_XX_START;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_COMBO_XX_START", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXStartEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXStartStateEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_COMBO_XX_CHARGE;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_COMBO_XX_CHARGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXLoopStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXLoopFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXLoopEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXLoopStateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ANSI_DASH_JUMP_COMBO_XX_FINISH;
	m_LuaManager.MakeTableReference( "ANSI_DASH_JUMP_COMBO_XX_FINISH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXFinishStateStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXFinishFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, DashJumpComboXXFinishEventProcess );
	m_StateList[stateData.stateID] = stateData;
}

void CX2GUAdd::InitStateByUnitClass()
{
	UserUnitStateData stateData;
	switch ( GetUnitClass() )
	{
	case CX2Unit::UC_ADD_NASOD_RULER:
		{

		} break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
		{
			stateData.Init();
			stateData.stateID			= ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_START;
			m_LuaManager.MakeTableReference( "ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_START", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZFrontZStartEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP;
			m_LuaManager.MakeTableReference( "ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZFrontZLoopStateStartFuture );
			stateData.StateStart		= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZFrontZLoopStateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZFrontZLoopEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX;
			m_LuaManager.MakeTableReference( "ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZFrontZXEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionX_LOOP;
			m_LuaManager.MakeTableReference( "ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionX_LOOP", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboXDirectionXLoopEventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboXDirectionXLoopStateEndFuture );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ;
			m_LuaManager.MakeTableReference( "ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboXDirectionXZEventProcess );
			m_StateList[stateData.stateID] = stateData;
		} /// 1차 전직 구문도 타기 위해, Break 제거

#endif //SERV_ADD_LUNATIC_PSYKER

	case CX2Unit::UC_ADD_PSYCHIC_TRACER:
		{
			stateData.Init();
			stateData.stateID			= ANSI_DP_BARRIER_ACTIVE;
			m_LuaManager.MakeTableReference( "ANSI_DP_BARRIER_ACTIVE", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, NasodBarrierActiveStateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodBarrierActiveEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_READY;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_READY", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorReadyEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_ATTACK;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_ATTACK", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAdd, NasodArmorAttackStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorAttackEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_LANDING;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_LANDING", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorLandingEventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, NasodArmorLandingEndFuture );

			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_Z;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_Z", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_ZZ;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_ZZZ;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_ZZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZZEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_ZZX;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_ZZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZXEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_ZZXX;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_ZZXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboZZXXEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_X;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_X", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboXEventProcess );
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboXStateEndFuture );
#endif //SERV_ADD_LUNATIC_PSYKER
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_XX;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_XX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboXXEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ANSI_COMBO_NASOD_ARMOR_XXX;
			m_LuaManager.MakeTableReference( "ANSI_COMBO_NASOD_ARMOR_XXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAdd, NasodArmorComboXXXEventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
	default:
		ASSERT(!"GetUnitClass Error");
		break;
	}
}


void CX2GUAdd::SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, ADD_NASODRULER_STATE_ID eStateID_ )
{
	if ( NULL != pSkillTemplet_ )
		SetSkillLevelStateData( pSkillTemplet_, m_StateList[eStateID_] );
}

void CX2GUAdd::SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ )
{
	if( NULL == pSkillTemplet_ )
		return;

	if ( NULL == GetUnit() )
		return;

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨

	stateData_.m_SPLevel	= iSkillTempletLevel;
	stateData_.m_fPowerRate	= pSkillTemplet_->GetSkillPowerRateValue( iSkillTempletLevel );
	stateData_.m_eSkillID	= pSkillTemplet_->m_eID;
}
void CX2GUAdd::ShowActiveSkillCutInAndLight( float fTimeToShow_, bool bOnlyLight_ /*= false*/ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTimeToShow_ ) == true && EventCheck( fTimeToShow_, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( GetShowCutInAndChangeWorldColor() && GetShowActiveSkillShow() )
		{
			ActiveSkillShow( L"Dummy2_Lhand", s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].fileName.c_str(),
				s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].vSize,
				s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].vPosition, IsHyperState(), bOnlyLight_ );
		}
	}	
}
/*virtual*/ void CX2GUAdd::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ )
{
	ShowActiveSkillCutInAndLight( fTimeToShow_, bOnlyLight_ );
}
bool CX2GUAdd::CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition/* = CX2SkillTree::ASUT_GROUND*/ )
{
	KTDXPROFILE();

	if( NULL != GetUnit() )
	{
		const CX2UserSkillTree& cUserSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;	// 유저가 배운 스킬 트리

		m_iPressedSkillSlotIndex = -1;
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;

		// 사용한 스킬의 스킬슬롯 데이터와 그 인덱스를 얻어옴
		if ( false == CommonSpecialAttackEventProcess( cUserSkillTree, pSkillSlotData, m_iPressedSkillSlotIndex ) )
			return false;

		if( NULL == pSkillSlotData )
			return false;

		// 사용한 스킬
		const CX2SkillTree::SkillTemplet* pSkillTemplet 
			= GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );

		if( NULL == pSkillTemplet )
			return false;

		const int iSkillTempletLevel = max( 1, cUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

		// 스킬을 사용할 수 있는 조건인지 판단( ex: 공중에서는 사용 할 수 없는 필살기 등)
		const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseConditionBySkillTemplet = GetSkillUseCondition( pSkillTemplet );

		if( false == CheckSkillUseCondition( eActiveSkillUseCondition, eActiveSkillUseConditionBySkillTemplet ) )
			return false;	

		//소환 필살기 종류는 게임이 끝난 상태에서 사용할 수 없다.
		if( false == CheckSummonSpecialAttack( pSkillTemplet->m_eID ) )
			return false;

		// 공식대전에서 사용 가능한 스킬인지 검사
		if( false == CheckSkillUsePVPOfficial( pSkillTemplet->m_eID ) )
			return false;

#ifdef FIX_NO_STATE_SKILL_BUG
		// 탈 것 탑승 상태에서 사용 가능한 스킬인지 검사
		if( false == IsValideSkillRidingOn( pSkillTemplet->m_eID)  )
			return false;
#endif // FIX_NO_STATE_SKILL_BUG

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		// 특수한 상황에서 발동할 수 있는 스킬인지 검사
		if( false == CheckSkillSpecialUse( pSkillTemplet->m_eID ) )
			return false;
#endif //SERV_ADD_LUNATIC_PSYKER

		// 엠피 소모량을 얻어옴
		m_fMPConsume			= GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );

		// DP 소모량을 얻어옴			( 기존의 기력 컬럼 사용 )
		m_fDPConsume			= GetActualDPConsume( pSkillSlotData );

		// 변이 포인트 소모량을 얻어옴	( 기존의 캐논볼 컬럼 사용 )
		m_iMutationCountConsume = GetMutationPointConsume( pSkillSlotData );

		const float fCoolTimeMax = GetActualCoolTime( pSkillTemplet, iSkillTempletLevel );
		
		float fCoolTimeLeft = min( fCoolTimeMax, pSkillSlotData->m_fCoolTimeLeft );

		bool bSlotB		= (m_iPressedSkillSlotIndex > 3) ? true : false;
		int iSlotIndex	= (m_iPressedSkillSlotIndex > 3) ? m_iPressedSkillSlotIndex-4 : m_iPressedSkillSlotIndex;

#ifndef _SERVICE_
		if( false == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
#endif _SERVICE_
			//#endif _SERVICE_
		{
			if( 0.f < fCoolTimeLeft )
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_226 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}

			/// MP 소모 검사
			if ( GetNowMp() < m_fMPConsume )
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}

			/// DP 소모 검사 ( 구성 모드 중이라면, DP를 사용하지 않는다. 비각성때는 MP, 각성시에만 DP를 쓰는 스킬 제외 )
			if ( GetDPValue() < m_fDPConsume )
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_29420 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}

			/// 변이 포인트 소모 검사
			if ( GetMutationCount() < m_iMutationCountConsume )
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_29421 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}

#ifdef FINALITY_SKILL_SYSTEM //JHKang
			if ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
			{
				const int iItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

				if( iItemNum <= 0 )
				{
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_26119 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
					return false;
				}
			}
#endif //FINALITY_SKILL_SYSTEM

			/// 다이너모 구성 - 파일런 소환 가능 여부 검사
			if ( pSkillTemplet->m_eID == CX2SkillTree::SI_A_APT_PYLON_DYNAMO &&
				false == GetSummonPylonPos( D3DXVECTOR3( 0.f, 0.f, 0.f ) ) )
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_29419 ), D3DXCOLOR(1,1,1,1),
					D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}
		}

		/// 스테이지 내 모든 몬스터 사망 혹은 스테이지 종료시 위상 변화 순간 이동 사용 불가
		if ( pSkillTemplet->m_eID == CX2SkillTree::SI_A_AN_PHASE_SHIFT &&
			 true == m_PhaseShiftData.IsValidReturnPosValue() &&
			 NULL != g_pX2Game &&
			 ( true == g_pX2Game->GetGameEnd() || true == g_pX2Game->GetLastKillCheck() ) )
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_226 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}

		if( NULL == pSkillTemplet )		/// pSkillTemplet을 위에서 새로 대입하기 때문에, NULL처리 추가
			return false;

		m_pSkillTemplet = pSkillTemplet;

		return true;
	}
	else
	{
		return false;
	}

	return false;
}

void CX2GUAdd::StateChangeSpecialAttack( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	if( NULL != GetUnit() && NULL != pSkillTemplet_ )
	{
		CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;// 유저가 배운 스킬 트리
		
		CountUsedSkill( m_iPressedSkillSlotIndex );

		/// MP 소비
			UpNowMp( -m_fMPConsume );

		/// DP 소비
			UpNowDPValue( -m_fDPConsume );

		/// 변이 포인트 소비
		UpMutationCount( -m_iMutationCountConsume );

		/// 스페셜 액티브 이면서 DP 소비량이 없을 때 DP사용량에 따라 DP 증가 처리
		if ( CX2SkillTree::ST_SPECIAL_ACTIVE == pSkillTemplet_->m_eType && 
			 0.f >= pSkillTemplet_->m_usFPConsumtion  )
			UpNowDPValueByUseMP( m_fMPConsume );


#ifdef FINALITY_SKILL_SYSTEM //JHKang
		if ( pSkillTemplet_->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
		{
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

			if ( NULL != pItem )
				g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
		}
#endif //FINALITY_SKILL_SYSTEM

		const int	iSkillTempletLevel	= max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨

		/// 설정을 위한 쿨타임 반환 함수, 특정 조건에 대하여 쿨타임 설정을 제외하기 위해 따로 구분
		const float fResultCoolTime		= GetCustomSkillCoolTime( GetActualCoolTime( pSkillTemplet_, iSkillTempletLevel), pSkillTemplet_->m_eID );

		if( false == IsSamef( -1.f, fResultCoolTime ))
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet_->m_eID, fResultCoolTime );

		if( false == SpecialAttackNoStageChange( pSkillTemplet_ ) )
		{
			/// 특정 스킬에 대한 스테이트 변경 검사
			if ( false == SpecialAttackCustomStateChange( pSkillTemplet_->m_eID ) )
			{
				/// 따로 변경해 줄 스테이트가 없다면, 일반적인 로직 진행
				if( false == GetIsFormationMode() )
				{
					if( m_iPressedSkillSlotIndex > 3 ) //슬롯 B일때
					{
						StateChange( _CONST_GUUSER_::aNormalSkillStateIDSlotB[m_iPressedSkillSlotIndex-4] );
					}
					else
					{
						StateChange( _CONST_GUUSER_::aNormalSkillStateID[m_iPressedSkillSlotIndex] );
					}
				}
				else
				{
					if( m_iPressedSkillSlotIndex > 3 ) //슬롯 B일때
					{
						StateChange( _CONST_GUUSER_::aHyperSkillStateIDSlotB[m_iPressedSkillSlotIndex-4] );
					}
					else
					{
						StateChange( _CONST_GUUSER_::aHyperSkillStateID[m_iPressedSkillSlotIndex] );
					}
				}
			}
		}

		m_iNowSpecialAttack = m_iPressedSkillSlotIndex + 1;
		m_bSpecialAttackEventProcessedAtThisFrame = true;
	}

}
/*virtual*/float CX2GUAdd::GetChangeDamageRateByAttackerSkill( const CX2DamageManager::DamageData* pDamageData_ ) const
{
	float fAtkDamageRate = 1.f;

	switch( pDamageData_->m_DPRateType )
	{
	case CX2DamageManager::DPRT_SPECIAL_ACTIVE:
	case CX2DamageManager::DPRT_ACTIVE:
		{
			// 스킬의 DP사용 여부에 따라 데미지 배율 적용
			if( true == IsDPConsumptionSkill( pDamageData_->m_eUsedSkillID ) )
				fAtkDamageRate *= m_fUsingDPSkillDamageRate;
			else  
				fAtkDamageRate *= m_fNotUsingDPSkillDamageRate;
		} break;
	case CX2DamageManager::DPRT_NASOD_ARMOR:
		{
			// 나소드 아머 커맨드 데미지 배율 적용
			fAtkDamageRate *= m_fNaCommandDamageRate;
		} break;
	default:
		break;
	}

	return fAtkDamageRate;
}
/*virtual*/ void CX2GUAdd::ReInit( bool bTeamPos /*= false*/, int iStartPosIndex /*= -1*/ )
{
	CX2GUUser::ReInit( bTeamPos, iStartPosIndex );

	if ( IsMyUnit() )
	{
//		오현빈//대전에서 부활 했을 때 초기화되면 안되기 때문에 제거
// 		switch ( g_pX2Game->GetGameType() )
// 		{
// 		case CX2Game::GT_PVP:
// 			{
// 				SetDPValue( 0.f );
// 				SetFormationModeCount( 0 );
// 				SetMutationCount( 0 );
// 			} break;
// 
// 		default:
// 			{
// 				if ( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
// 				{
// 					SetDPValue( 0.f );
// 					SetFormationModeCount( 0 );
// 					SetMutationCount( 0 );
// 				}
// 			} break;
// 		}

		/// DP 및 변이 수치 동기화
		m_FrameDataFuture.syncData.m_CannonBallCount = GetAbilitySyncData();

		/// 다이너모 파일런 및 연결 데미지 이펙트 정보 초기화
		ResetPylonData();

		/// 마그네트론 관련 객체 초기화
		m_MagnetronData.ResetMagnetronData();

		/// 스테이시스 필드 정보 초기화
		m_StasisfieldData.ResetStasisfieldData();

		/// 위상 변화 초기화
		m_PhaseShiftData.InitPhaseEffectData();
		m_PhaseShiftData.InitResturnPosValue();
		m_PhaseShiftData.UpdateSkillSlotUI();

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		/// 환상 추적자 스킬 아이디 초기화
		m_eNasodArmorAttackActiveSkillID = CX2SkillTree::SI_NONE;

		/// 리버스 리액터 관련 객체 초기화
		m_ReverseReactorTargetInfo.ResetReverseReactorTargetInfo();

		/// 퀵실버 프렌지 관련 객체 초기화
		m_QuicksilverFrenzyData.ResetQuicksilverFrenzyData();
#endif //SERV_ADD_LUNATIC_PSYKER
	}
}
/*virtual*/ void CX2GUAdd::DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ )
{
	KTDXPROFILE();

	if ( NULL == pDamageData_ )
		return;

	/// 만약 DP 베리어가 가동 되었다면 피격 모션을 취하지 않으며, 데미지를 입지 않는다.
	if ( true == IsPossibleChangeNasodBarrier( pDamageData_ ) )
	{
		SetDamageReducePercent( 100 );		/// 데미지 감소율 100% 설정
		CX2GUUser::DamageReact( pDamageData_ );
		SetDamageReducePercent( -100 );		/// 데미지 감소율 복구
	}
	else
	{
		CX2GUUser::DamageReact( pDamageData_ );
		CX2GUUser::DamageReactStateChange( pDamageData_ );
	}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 퀵실버 프랜지 공격 처리
	AttackQuicksilverFrenzy( *pDamageData_ );
#endif //SERV_ADD_LUNATIC_PSYKER

	StopLoopSound();
}
/*virtual*/ void CX2GUAdd::Win()
{
	CX2GUUser::Win();
	StateChange( GetWinStateID() );
}
/*virtual*/ void CX2GUAdd::Lose()
{
	CX2GUUser::Lose();

	if ( true == IsMyUnit() && 0.f < GetNowHp() )
	{
		StateChange( GetLoseStateID() );
	}
}
/*virtual*/ void CX2GUAdd::Draw()
{
	CX2GUUser::Draw();
	StateChange( GetWinStateID() );
}
/*virtual*/ void CX2GUAdd::InitEffect()
{
	CX2GUUser::InitEffect();

	CreateMajorParticleArray();
	CreateMinorParticleArray();
}
/*virtual*/ void CX2GUAdd::InitEnchantWeaponEffectUnitSpecific()
{
	BOOST_TEST_FOREACH( Weapon*, pWeapon, m_vecpWeapon )
	{
		ASSERT( NULL != pWeapon );
		pWeapon->InitEnchantParticleForBow();
	}
}

/** @function	: AttackResultByType
	@brief		: 공격시 추가 기능 처리 함수
	@param		: 데미지 데이터
*/
void CX2GUAdd::AttackResultByType( CX2DamageManager::DamageData &pDamageData )
{
	CX2GUUser::AttackResultByType( pDamageData );

	/// 나소드 장갑 콤보용 타겟 정보 설정
	if ( true == IsMyUnit() )
		SetNasodArmorAttackTarget( pDamageData );

	/// 다이너모 구성 - 마그네트론 공격 처리
	AttackMagnetronDamageEffect( pDamageData );

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 더스트 아나토미 타겟 정보 설정
	SetDustAnatomyTarget( pDamageData );

	/// 퀵실버 프랜지 공격 처리
	AttackQuicksilverFrenzy( pDamageData, true );
#endif //SERV_ADD_LUNATIC_PSYKER
}

/*virtual*/ void CX2GUAdd::CreateMajorParticleArray()
{
	CX2GUUser::CreateMajorParticleArray();
	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	// small damage 타격을 줬을 때 나오는 파티클
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT,			L"DamageImpact",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE,		L"DamageImpactCore",		2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH,		L"DamageImpactSlash",		2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	// AttackResult의 switch case에서 default:로 처리되는 타격에 대한 파티클 (현재는 small 이외의 파티클)
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RED,		L"DamageImpactRed",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RING_RED,	L"DamageImpactRingRed",		1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE_RED,	L"DamageImpactCoreRed",		1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH_RED,	L"DamageImpactSlashRed",	1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
}
/*virtual*/ void CX2GUAdd::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();
}
/*static*/ void CX2GUAdd::AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMajorParticle() == NULL )
		return;

	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpact" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactCore" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactSlash" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactRingRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactCoreRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"DamageImpactSlashRed" );
}
/*static*/ void CX2GUAdd::AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{

}
/*virtual*/ bool CX2GUAdd::SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	if( NULL == pSkillTemplet_ )
		return false;

	switch( pSkillTemplet_->m_eID )
	{
	case CX2SkillTree::SI_SA_APT_STASIS_FIELD:
		{
			if ( 0.f < m_StasisfieldData.m_fRemainTime )
				m_FrameDataFuture.syncData.nowAction = UAI_ACTIVE_STASIS_FIELD;
			else
				return false;
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}
/*virtual*/ void CX2GUAdd::COMMON_BUFF_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.2f ) && true == EventCheck( 0.2f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		PlayCommonBuffMinorParticle();
		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 0.25f ) && true == EventCheck( 0.25f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		int	iSkillSlotIndex = 0;
		bool bSlotB = false;

		if ( true == GetSkillSlotIndexUsed( iSkillSlotIndex, bSlotB ) )
		{
			const CX2Unit::UnitData& pUnitData = GetUnit()->GetUnitData();

			const CX2UserSkillTree::SkillSlotData* pSkillSlotData = pUnitData.m_UserSkillTree.GetSkillSlot( iSkillSlotIndex, bSlotB );
			CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE;

			if( NULL != pSkillSlotData )
				eSkillID = pSkillSlotData->m_eID;

			ProcessAcceleraterBuff( eSkillID, IsHyperState() );
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::FormationModeStart()
{
	CommonStateStart();

	if ( NULL != m_ptrRidingPet )
		m_ptrRidingPet->StateChange( m_ptrRidingPet->m_WaitState );

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 구성 모드 전환에 필요한 DP량
	const float fUseDPByFormationMode = GetUseDPByFormationMode();

	UpNowDPValue( -fUseDPByFormationMode );

	/// 환상 추적자 적용중이라면, 소모한 DP의 일정량을 MP로 치환
	if ( true == m_bEnableFantasyTracer )
	{
		/// 실제 DP는 UI내 표기된 수치보다 20배 크므로, 20을 나눠 준다.
		const float fUpMPValue = fUseDPByFormationMode * m_fFantasyTracerUpMPRateByFormationMode / 20.f;

		UpNowMp( fUpMPValue );	/// MP 회복
		
		/// MP 회복량 표시
		if ( NULL != g_pX2Game )
			g_pX2Game->DisplayAddMPInformation( fUpMPValue, GetPos(), GetDirVector() );
	}

	/// 초월자의 신체 - 구성 모드중 이동 속도 증가 설정
	m_AdditionalWalkSpeed.SetPassiveMultiple( m_AdditionalWalkSpeed.GetPassiveMultiple() * m_fBodyOfTranscendenceMoveSpeed );
	m_AdditionalDashSpeed.SetPassiveMultiple( m_AdditionalDashSpeed.GetPassiveMultiple() * m_fBodyOfTranscendenceMoveSpeed );
#else //SERV_ADD_LUNATIC_PSYKER
	/// DP( 각성 수치 ) 소모
	UpNowDPValue( -GetUseDPByFormationMode() );
#endif //SERV_ADD_LUNATIC_PSYKER

	/// 변이 게이지 1 추가
	UpMutationCount( 1 );

	SetRemainHyperModeTime( 99999.f );

	m_bHyperModeUsed							= true;
	m_iLastStateHyperModeCount					= m_FrameDataNow.syncData.m_HyperModeCount;
	m_iHyperModeCountUsed						= m_FrameDataNow.syncData.m_HyperModeCount;
	m_FrameDataNow.syncData.m_HyperModeCount	= 0;
}

/*virtual*/ void CX2GUAdd::HyperModeFrameMove()
{
#ifdef MODIFY_RIDING_PET_AWAKE
	CommonHyperModeFrameMove( 0.05f, 0.46f );
#else
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

	if( m_pXSkinAnim->EventTimer( 0.05f ) == true && EventCheck(0.05f, false) == true )
	{		
		ShowMinorParticleHyperModeChange();
	}

	if( m_pXSkinAnim->EventTimer( 0.46f ) == true && EventCheck(0.46f, false) == true )
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();
	}
	CommonFrameMove();
#endif // MODIFY_RIDING_PET_AWAKE
}
/*virtual*/ void CX2GUAdd::HyperModeBuffEffectStart()
{
	CX2GUUser::HyperModeBuffEffectStart();
}
/** @function	: SetSpecificValueByBuffTempletID
	@brief		: 각 유닛마다 특정 버프가 실행 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 설정 등...)
	@param		: 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUAdd::SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

 	switch ( eBuffTempletId_ )
 	{
	case BTI_FORMATION_MODE:
		{
			/// 구성 모드 설정
			SetIsFormationMode( true );

			if ( GetUnitClass() != CX2Unit::UC_ADD_NASOD_RULER )
				SetBuffFactorToGameUnitByBuffFactorID( BFI_NASOD_ARMOR );
		} break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		/// 퀵실버 프렌지
	case BTI_BUFF_QUICKSILVER_FRENZY:
		{
			m_QuicksilverFrenzyData.ResetQuicksilverFrenzyData();
			m_QuicksilverFrenzyData.m_bEnableQuicksilverFrenzy = true;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER

 	default:
 		break;
 	}
}
/** @function	: UnSetSpecificValueByBuffTempletID
	@brief		: 각 유닛마다 특정 버프가 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 해제 됬다는 플래그 설정 등...)
	@param		: 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUAdd::UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_FORMATION_MODE:
		{
			/// 구성 모드 해제
			SetIsFormationMode( false );
			
			if ( GetUnitClass() != CX2Unit::UC_ADD_NASOD_RULER )
				ReserveToFinishBuffTempletFromGameUnit( BTI_NASOD_ARMOR );
		} break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		/// 퀵실버 프렌지
	case BTI_BUFF_QUICKSILVER_FRENZY:
		{
			m_QuicksilverFrenzyData.ResetQuicksilverFrenzyData();
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER

	default:
		break;
	}
}

/** @function	: ApplyRenderParam
	@brief		: 버프에 의한 renderparam과 함께 추가적으로 해비스텐스에 대한 renderparam을 설정하는 함수
	@param		: SkinAnim의 RenderParam 포인터(pRenderParam_)
*/
/*virtual*/ void CX2GUAdd::ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
#ifdef RIDING_SYSTEM
	if ( m_bPassDash )
		return;
#endif //RIDING_SYSTEM

	switch( m_eCustomRenderState )
	{
	case CRS_COMBO_DASH_X:
		{
			m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON;
			m_RenderParam.outLineColor.a	= m_fRenderParamColorA;
			m_RenderParam.color.a			= 0.f;
			m_RenderParam.bAlphaBlend		= true;
		} break;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CRS_QUICKSILVER_FRENZY_BUFF:
		{
			m_RenderParam.renderType	= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
			m_RenderParam.outLineColor	= D3DXCOLOR( 1.f, 1.f, 1.f, 1.f );
			m_RenderParam.color			= SetQuicksilverFrenzyFlickerColor( D3DXCOLOR( 0.5f, 0.3f, 1.f, 1.f ) );
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
	default:
		{
			CX2GameUnit::ApplyRenderParam( pRenderParam_ );
		} break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스테이트 함수
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ANSI_DIE_FRONT
void CX2GUAdd::DieFrontStartFuture()
{
	CommonStateStartFuture();
}
void CX2GUAdd::DieFrontStart()
{
	CommonStateStart();
	DyingStart();
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
}
void CX2GUAdd::DieFrontFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}
void CX2GUAdd::DieFrontEventProcess()
{
	if ( GetShowObject() == false && m_pXSkinAnim->IsAnimationEnd() == true )
	{
		StateChange( GUSI_DIE );
	}
}

//ANSI_DIE_BACK
void CX2GUAdd::DieBackStartFuture()
{
	CommonStateStartFuture();
}
void CX2GUAdd::DieBackStart()
{
	CommonStateStart();
	DyingStart();
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
}
void CX2GUAdd::DieBackFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}
void CX2GUAdd::DieBackEventProcess()
{
	if ( GetShowObject() == false  && m_pXSkinAnim->IsAnimationEnd() == true )
	{
		StateChange( GUSI_DIE );
	}
}

//ANSI_WAIT
void CX2GUAdd::WaitStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationWaitStartByAdd( m_pXSkinAnimFuture, m_FrameDataFuture );
}
void CX2GUAdd::WaitStart()
{
	CommonStateStart();
	PlayAnimationWaitStartByAdd( m_pXSkinAnim.get(), m_FrameDataNow );

	m_CommonState.m_Wait = USI_WAIT;

	/// 나소드 아머 콤보 타겟 정보 초기화
	m_NasodArmorAttackTargetData.ResetTargetData();
}
void CX2GUAdd::WaitEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ANSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ANSI_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess() )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
				LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
				&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if ( true == GetIsFormationMode() )
	{
		StateChange( ANSI_HYPER_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	CommonEventProcess();
}

//ANSI_HYPER_WAIT
void CX2GUAdd::HyperWaitStartFuture()
{
	CommonStateStartFuture();
}
void CX2GUAdd::HyperWaitStart()
{
	CommonStateStart();

	/// 나소드 아머 콤보 타겟 정보 초기화
	m_NasodArmorAttackTargetData.ResetTargetData();

	m_CommonState.m_Wait = ANSI_HYPER_WAIT;
}
void CX2GUAdd::HyperWaitEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ANSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ANSI_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess() )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_RELEASE_FORMATION_MODE
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if ( false == GetIsFormationMode() )
	{
		StateChange( USI_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	CommonEventProcess();
}

//ANSI_WALK
void CX2GUAdd::WalkEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ANSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ANSI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureRight == false && m_InputData.pureLeft == false )
	{
		StateChange( USI_WAIT );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

//ANSI_JUMP_READY
void CX2GUAdd::JumpReadyEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() || true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}

	CommonEventProcess();
}

//ANSI_JUMP_UP
void CX2GUAdd::JumpUpEventProcess()
{
	if( ( m_InputData.oneDoubleRight == true || m_InputData.oneDoubleLeft == true ) &&
		true == m_bEnableAirDash )
	{
		StateChange( ANSI_AIR_DASH );
		m_bEnableAirDash = false;
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( ( m_InputData.oneUp || true == m_InputData.oneDoubleUp ) && true == m_bEnableLevitationWait )
	{
		StateChange( ANSI_LEVITATION_WAIT );
		m_bEnableLevitationWait = false;
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ANSI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ANSI_JUMP_ATTACK_X );
	}
	else if( 0.0f >= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_DOWN );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

//ANSI_JUMP_DOWN
void CX2GUAdd::JumpDownEventProcess()
{
	if( ( m_InputData.oneDoubleRight == true || m_InputData.oneDoubleLeft == true ) &&
		true == m_bEnableAirDash )
	{
		StateChange( ANSI_AIR_DASH );
		m_bEnableAirDash = false;
	}
	else if ( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( ( m_InputData.oneUp || true == m_InputData.oneDoubleUp ) && true == m_bEnableLevitationWait )
	{
		StateChange( ANSI_LEVITATION_WAIT );
		m_bEnableLevitationWait = false;
	}
	else if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ANSI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ANSI_JUMP_ATTACK_X );
	}
	else if ( 0.0f <= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_DOWN );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

//ANSI_JUMP_LANDING
void CX2GUAdd::JumpLandingStartFuture()
{
	CommonStateStartFuture();
}
void CX2GUAdd::JumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChange( USI_WALK );
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ANSI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ANSI_COMBO_X );
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( USI_WAIT, false );
	}

	CommonEventProcess();
}

//ANSI_DASH
void CX2GUAdd::DashEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		if( true == m_bEnableAirDash && ( true == m_InputData.pureRight || true == m_InputData.pureLeft ) )
		{
			StateChange( ANSI_AIR_DASH );

			m_bEnableAirDash = false;
		}
		else
		{
			StateChange( USI_DASH_JUMP );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ANSI_DASH_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ANSI_DASH_COMBO_X );
	}
	else if ( true == SpecialAttackEventProcess() )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE
	else if( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_InputData.pureLeft == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChange( USI_DASH_END );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_DASH_JUMP );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

//ANSI_AIR_DASH
void CX2GUAdd::AirDashEventProcess()
{
	if( 0.45 < m_FrameDataFuture.unitCondition.fStateTime )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ANSI_DASH_JUMP_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ANSI_DASH_JUMP_COMBO_X );
	}
	else if ( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	else if( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( m_InputData.pureRight == true && m_InputData.pureLeft == true )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_DASH_JUMP );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

void CX2GUAdd::AirDashStateEndFuture()
{
	if ( 800.f < m_PhysicParam.nowSpeed.x )
		m_PhysicParam.nowSpeed.x = 800.f;

	CommonStateEndFuture();
}

//ANSI_DASH_END
void CX2GUAdd::DashEndStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GUAdd::DashEndFrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true )
		CreateStepDust();

	CommonFrameMove();
}
void CX2GUAdd::DashEndEventProcess()
{
	// DashEnd에서 공격 키를 눌렀을 때 대시 공격인지, 일반 공격인지를 정하기 위한 const 값. 
	const float TRIGGER_TIME_FACTOR_DASH_ATTACK = 0.3f;

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ANSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ANSI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true)
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.oneLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChangeDashIfPossible();
	}
	else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

//ANSI_DASH_JUMP
void CX2GUAdd::DashJumpStartFuture()
{
	CommonStateStartFuture();

	const float MAGIC_NUMBER_MULTIPLY_GRAVITY_ACCEL = 1.2f;

	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * MAGIC_NUMBER_MULTIPLY_GRAVITY_ACCEL;
	m_bDownForce					= false;

}
void CX2GUAdd::DashJumpStart()
{
	CommonStateStart();

	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
		CreateStepDust();
}
void CX2GUAdd::DashJumpFrameMoveFuture()
{
	const float MAGIC_NUMBER_MULTIPLY_DASH_SPEED = 1.6f;

	if( m_InputData.pureRight == true && m_FrameDataNow.syncData.bIsRight	== true )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * MAGIC_NUMBER_MULTIPLY_DASH_SPEED;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataNow.syncData.bIsRight	== false )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * MAGIC_NUMBER_MULTIPLY_DASH_SPEED;
	}

	CommonFrameMoveFuture();
}
void CX2GUAdd::DashJumpEventProcess()
{
	bool &bCanDashJump = m_FrameDataFuture.stateParam.bEventFlagList[0];

	if( true == IsOnSomethingFuture() )
	{
		
		if ( true == m_bDownForce )
		{
			StateChange( ANSI_DASH_JUMP_POWER_LANDING );
			m_bDownForce = false;
		} // if
		else
		{
			if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			{
				StateChangeDashIfPossible();
			}
			else
				StateChange( USI_DASH_JUMP_LANDING );

			m_bDownForce = false;
		}
	}
	else if( m_InputData.oneUp && true == m_bEnableAirDash )
	{
		StateChange( ANSI_AIR_DASH );
		m_bEnableAirDash = false;
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		StateChange( ANSI_WALL_LANDING );
		m_bDisableGravity = true;
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ANSI_DASH_JUMP_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ANSI_DASH_JUMP_COMBO_X );
	}
	else if( true == m_InputData.pureRight )
	{
		if( false == m_FrameDataFuture.syncData.bIsRight )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( 0.0f < m_PhysicParam.nowSpeed.y )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	else if( true == m_InputData.pureLeft )
	{
		if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			if( GetJumpSpeed() < m_PhysicParam.nowSpeed.y )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( 0.0f < m_PhysicParam.nowSpeed.y )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	else if ( ( m_InputData.oneDoubleRight == true || m_InputData.oneDoubleLeft == true ) &&
			  true == m_bEnableAirDash )
	{
		StateChange( ANSI_AIR_DASH );
		m_bEnableAirDash = false;
	}
	else if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
		bCanDashJump = false;
	}

	CommonEventProcess();
}
void CX2GUAdd::DashJumpEndFuture()
{
	CommonStateEndFuture();
	m_PhysicParam.fDownAccel	= 0.0f;
}

//ANSI_DASH_JUMP_LANDING
void CX2GUAdd::DashJumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ANSI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ANSI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_DASH);
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_READY );
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
void CX2GUAdd::DashJumpLandingStartFuture()
{
	CX2GUUser::DashJumpLandingStartFuture();
}

/// ANSI_LEVITATION_WAIT
void CX2GUAdd::LevitationWaitFrameMove()
{
	/// 소비량 = 초당 MP 2 * 스테이트 유지시간
	const float fConsumeMP = _CONST_ADD_::LEVITATION_CONSUME_MP_PER_SECOND * m_fElapsedTime * m_FrameDataNow.unitCondition.fStateTime;

	FlushMp( fConsumeMP );

	CommonFrameMove();
}

void CX2GUAdd::LevitationWaitEventProcess()
{
	if( true == m_InputData.oneDown || 1.f > GetNowMp() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( 0.f >= GetNowHp() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ANSI_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ANSI_JUMP_ATTACK_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( ( m_InputData.oneDoubleRight == true || m_InputData.oneDoubleLeft == true ) &&
			 true == m_bEnableAirDash )
	{
		StateChange( ANSI_AIR_DASH );
		m_bEnableAirDash = false;
	}
	else if( m_InputData.oneRight == true || m_InputData.oneLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( m_InputData.oneDown == true )
	{
		if( NULL != g_pX2Game && 
			NULL != g_pX2Game->GetWorld() &&
			NULL != g_pX2Game->GetWorld()->GetLineMap() &&
			true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}

	CommonEventProcess();
}

//ANSI_DAMAGE_SMALL_FRONT
//ANSI_DAMAGE_SMALL_BACK
void CX2GUAdd::DamageSmallEventProcess()
{
	if( m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y > 200.0f )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_DAMAGE_AIR_SMALL );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}

//ANSI_DAMAGE_BIG_FRONT
void CX2GUAdd::DamageBigStart()
{
	CommonStateStart();
	CreateStepDust();
}
void CX2GUAdd::DamageBigEventProcess()
{
	float fGapFromUnitToLineMap = m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y;
	const float ACCEPTABLE_GAP = 200.0f;	// 이것보다 거리가 크면 공중에 있는 상태임

	if( ACCEPTABLE_GAP < fGapFromUnitToLineMap )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_DAMAGE_AIR_SMALL );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( 0.93f < m_pXSkinAnimFuture->GetNowAnimationTime() )
		{
			StateChange( USI_WAIT );

		}
	}

	CommonEventProcess();
}

//ANSI_DAMAGE_DOWN_FRONT
void CX2GUAdd::DamageDownFrontFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimerOneshot( 0.19f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.19f ) && true == EventCheck( 0.19f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		CreateStepDust();

	CommonFrameMove();
}
void CX2GUAdd::DamageDownFrontEventProcess()
{
	bool& bChangeStandUpAttackFront			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingFrontFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingFrontBack	= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackFront		= m_FrameDataFuture.stateParam.bEventFlagList[4];
	
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	// 넘어진 후
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		if( true == bChangeStandUpAttackFront )
		{
			StateChange( ANSI_STAND_UP_ATTACK_FRONT );
		}
		else if ( true == bChangeStandUpBigAttackFront )
		{
			StateChange( ANSI_STAND_UP_ATTACK_FRONT );
		}
		else if( true == bChangeStandUpRollingFrontFront )
		{
			StateChange( ANSI_STAND_UP_ROLLING_FRONT_FRONT );
		}
		else if( true == bChangeStandUpRollingFrontBack )
		{
			StateChange( ANSI_STAND_UP_ROLLING_FRONT_BACK );
		}
		else
		{
			StateChange( USI_DAMAGE_STANDUP_FRONT );
		}
	}
	else if( 0.33f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( true == m_InputData.oneZ )
		{	
			bChangeStandUpAttackFront		= true;
			bChangeStandUpRollingFrontFront	= false;
			bChangeStandUpRollingFrontBack	= false;
			bChangeStandUpBigAttackFront	= false;

		}
		else if ( true == m_InputData.oneX )
		{
			bChangeStandUpAttackFront		= false;
			bChangeStandUpRollingFrontFront	= false;
			bChangeStandUpRollingFrontBack	= false;
			bChangeStandUpBigAttackFront	= true;
		}
		// 우측 방향을 향하고 있는 경우에
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 앞으로 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontBack	= false;
				bChangeStandUpRollingFrontFront	= true;
				bChangeStandUpBigAttackFront	= false;

			}
			// 좌측 화살표 키보드를 누른 경우
			else if( true == m_InputData.pureLeft )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontFront	= false;
				bChangeStandUpRollingFrontBack	= true;
				bChangeStandUpBigAttackFront	= false;

			}
		}
		// 좌측 방향을 향하고 있는 경우에
		else
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontFront	= false;
				bChangeStandUpRollingFrontBack	= true;
				bChangeStandUpBigAttackFront	= false;

			}
			else if( true == m_InputData.pureLeft )
			{	
				// 앞을 롤링
				bChangeStandUpAttackFront		= false;
				bChangeStandUpRollingFrontBack	= false;
				bChangeStandUpRollingFrontFront	= true;
				bChangeStandUpBigAttackFront	= false;
			}
		}
	}

	CommonEventProcess();
}

//ANSI_DAMAGE_DOWN_BACK
void CX2GUAdd::DamageDownBackFrameMove()
{
	if( true == m_pXSkinAnim->EventTimer( 0.32f ) )
		CreateStepDust();

	CommonFrameMove();
}
void CX2GUAdd::DamageDownBackEventProcess()
{
	bool& bChangeStandUpAttackBack			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingBackFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingBackBack		= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackBack		= m_FrameDataFuture.stateParam.bEventFlagList[4];

	// 넘어졌는데 밑에 라인맵 등이 없는 경우
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	// 넘어진 후
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		// Z 키를 눌렀을 때
		if( true == bChangeStandUpAttackBack )
		{
			StateChange( ANSI_STAND_UP_ATTACK_BACK );
		}
		// X 키를 눌렀을 때
		else if ( true == bChangeStandUpBigAttackBack )
		{
			StateChange( ANSI_STAND_UP_ATTACK_BACK );
		}
		else if( true == bChangeStandUpRollingBackFront )
		{
			StateChange( ANSI_STAND_UP_ROLLING_BACK_FRONT );
		}
		else if( true == bChangeStandUpRollingBackBack )
		{
			StateChange( ANSI_STAND_UP_ROLLING_BACK_BACK );
		}
		else
		{
			StateChange( USI_DAMAGE_STANDUP_BACK );
		}
	}

	else if( 0.33f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( true == m_InputData.oneZ )
		{	
			bChangeStandUpAttackBack		= true;
			bChangeStandUpRollingBackFront	= false;
			bChangeStandUpRollingBackBack	= false;
			bChangeStandUpBigAttackBack		= false;
		}
		else if ( true == m_InputData.oneX )
		{
			bChangeStandUpAttackBack		= false;
			bChangeStandUpRollingBackFront	= false;
			bChangeStandUpRollingBackBack	= false;
			bChangeStandUpBigAttackBack		= true;
		}
		// 우측 방향을 향하고 있는 경우에
		else if( true == m_FrameDataFuture.syncData.bIsRight )
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 앞으로 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackBack	= false;
				bChangeStandUpRollingBackFront	= true;
				bChangeStandUpBigAttackBack		= false;

			}
			// 좌측 화살표 키보드를 누른 경우
			else if( true == m_InputData.pureLeft )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackFront	= false;
				bChangeStandUpRollingBackBack	= true;
				bChangeStandUpBigAttackBack		= false;

			}
		}
		// 좌측 방향을 향하고 있는 경우에
		else
		{
			// 우측 화살표 키보드를 누른 경우
			if( true == m_InputData.pureRight )
			{	
				// 뒤로 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackFront	= false;
				bChangeStandUpRollingBackBack	= true;
				bChangeStandUpBigAttackBack		= false;

			}
			else if( true == m_InputData.pureLeft )
			{	
				// 앞을 롤링
				bChangeStandUpAttackBack		= false;
				bChangeStandUpRollingBackBack	= false;
				bChangeStandUpRollingBackFront	= true;
				bChangeStandUpBigAttackBack		= false;
			}
		}
	}

	CommonEventProcess();
}

//ANSI_DAMAGE_AIR_DOWN_LANDING
void CX2GUAdd::DamageAirDownLandingEventProcess()
{
	DamageDownFrontEventProcess();
}

//ANSI_STANDUP_ROLLING_FRONT_FRONT
void CX2GUAdd::StandUpRollingFrontFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChange( USI_WALK );
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
void CX2GUAdd::StandUpRollingFrontFrontEnd()
{
	CommonStateEnd();

	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

//ANSI_STANDUP_ROLLING_FRONT_BACK
void CX2GUAdd::StandUpRollingFrontBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChange( USI_WALK );
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
void CX2GUAdd::StandUpRollingFrontBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

//ANSI_STANDUP_ROLLING_BACK_FRONT
void CX2GUAdd::StandUpRollingBackFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChange( USI_WALK );
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
void CX2GUAdd::StandUpRollingBackFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

//ANSI_STANDUP_ROLLING_BACK_BACK
void CX2GUAdd::StandUpRollingBackBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChange( USI_WALK );
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
void CX2GUAdd::StandUpRollingBackBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

//ANSI_STANDUP_ATTACK_FRONT
void CX2GUAdd::StandUpAttackFrontEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChange( USI_WALK );
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}
void CX2GUAdd::StandUpAttackFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

//ANSI_STANDUP_ATTACK_BACK
void CX2GUAdd::StandUpAttackBackEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChange( USI_WALK );
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}
void CX2GUAdd::StandUpAttackBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}
void CX2GUAdd::StandUpAttackBackStateEndFuture()
{
	m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
	CommonStateEndFuture();
}

//ANSI_DASH_JUMP_POWER_LANDING
void CX2GUAdd::DashJumpPowerLandingStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x /= 2.0f;
}
void CX2GUAdd::DashJumpPowerLandingStart()
{
	CommonStateStart();
	CreateStepDust();
}
void CX2GUAdd::DashJumpPowerLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneRight == true || m_InputData.oneLeft == true )
	{
		StateChange( USI_WALK );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 6.0f )
	{
		if( m_InputData.oneZ == true )
		{
			StateChange( ANSI_COMBO_Z );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( ANSI_COMBO_X );
		}
		else if( SpecialAttackEventProcess() == true )
		{
		}
		else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureUp == true )
		{
			StateChange( USI_JUMP_READY );
		}
		//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 10.f, ESSI_REVENGE_WAIT )
		else if( m_InputData.oneDown == true 
			&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
			LINE_RADIUS, 
			&m_FrameDataFuture.unitCondition.landPosition, 
			&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}

		else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


#pragma region 커맨드 함수

/// ANSI_COMBO_Z
void CX2GUAdd::ComboZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_COMBO_ZZ )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_ZZ
void CX2GUAdd::ComboZZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_COMBO_ZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_ZZX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_ZZZ
void CX2GUAdd::ComboZZZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_COMBO_ZZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_ZZZX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_ZZZZ
void CX2GUAdd::ComboZZZZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_ZZZX
void CX2GUAdd::ComboZZZXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}

/// ANSI_COMBO_ZZX
void CX2GUAdd::ComboZZXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_ZZXX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_ZZXX
void CX2GUAdd::ComboZZXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_X
void CX2GUAdd::ComboXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_XX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_XX
void CX2GUAdd::ComboXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_XXX )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, 
								 m_InputData.oneZ == true, ANSI_COMBO_XXZ )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_XXX
void CX2GUAdd::ComboXXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_XXZ
void CX2GUAdd::ComboXXZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_COMBO_XXZZ_START )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_XXZZ_START
void CX2GUAdd::ComboXXZZStartEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if(  0.456f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		StateChange( ANSI_COMBO_XXZZ_LOOP );
	}

	CommonEventProcess();
}

void CX2GUAdd::ComboXXZZStartStateEndFuture()
{
	CommonStateEndFuture();
	/// 스테이트 변경 즉시 사운드를 종료시켜 주기 위해 코드에서 처리
	PlayLoopSound(L"Add_Effect05.ogg", true, false );
}

/// ANSI_COMBO_XXZZ_LOOP
void CX2GUAdd::ComboXXZZLoopStateStart()
{
	CommonStateStart();

	SetNoDetonation( true );
}

void CX2GUAdd::ComboXXZZLoopEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		++ m_iComboLoopCount;

		/// 최소 반복 횟수를 만족하지 못하거나, 최대 반복 횟수 안에서 키를 누르고 있을 때 반복 스테이트로 전환
		if ( _CONST_ADD_::COMBO_XXZZ_MIN_COUNT - 1 > m_iComboLoopCount || 
			 ( _CONST_ADD_::COMBO_XXZZ_MAX_COUNT - 2 >= m_iComboLoopCount && true == m_FrameDataFuture.stateParam.bEventFlagList[0]) )
			StateChange( ANSI_COMBO_XXZZ_LOOP );
		else
			StateChange( ANSI_COMBO_XXZZ_FINISH );
	}
	ELSE_IF_SET_EVENT_FLAG( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, true == m_InputData.oneZ )

	CommonEventProcess();
}

void CX2GUAdd::ComboXXZZLoopStateEnd()
{
	CommonStateEnd();

	SetNoDetonation( false );
}

/// ANSI_COMBO_XXZZ_FINISH
void CX2GUAdd::ComboXXZZFinishStateStartFuture()
{
	CommonStateStartFuture();

	m_iComboLoopCount = 0;	/// 콤보 반복수 초기화

	/// 차징 사운드 종료
	StopLoopSound();

	CommonStateStartFuture();
}

void CX2GUAdd::ComboXXZZFinishStateStart()
{
	CommonStateStart();
}

void CX2GUAdd::ComboXXZZFinishEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_JUMP_ATTACK_Z
void CX2GUAdd::JumpComboZEventMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( m_InputData.pureDown == true && m_InputData.pureUp == false )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonFrameMoveFuture();
}

void CX2GUAdd::JumpComboZEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_JUMP_ATTACK_X
void CX2GUAdd::JumpComboXEventMoveFuture()
{
	if( m_InputData.pureDown == true && m_InputData.pureUp == false )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonFrameMoveFuture();
}

void CX2GUAdd::JumpComboXEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_COMBO_Z
void CX2GUAdd::DashComboZFrameMoveFuture()
{
	if( m_InputData.pureDown == true && m_InputData.pureUp == false )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonFrameMoveFuture();
}

void CX2GUAdd::DashComboZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_DASH_COMBO_ZZ )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_COMBO_ZZ
void CX2GUAdd::DashComboZZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_DASH_COMBO_ZZZ )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_COMBO_ZZZ
void CX2GUAdd::DashComboZZZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_COMBO_X
void CX2GUAdd::DashComboXStateStart()
{
	m_eCustomRenderState	= CRS_NONE;

	CommonStateStart();
}

void CX2GUAdd::DashComboXFrameMove()
{
	CommonFrameMove();

	/// 특정 애니메이션 시간 동안은 숨기자!

	if( 0.085f <= m_pXSkinAnim->GetNowAnimationTime() && 0.43f > m_pXSkinAnim->GetNowAnimationTime() )
	{
		m_eCustomRenderState	= CRS_COMBO_DASH_X;
	}
}

void CX2GUAdd::DashComboXEventProcess()
{
	if ( false == IsOnSomethingFuture() && 0.45f <= m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_DASH_COMBO_XX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

void CX2GUAdd::DashComboXStateEnd()
{
	m_fRenderParamColorA	= 1.f;		/// 알파값 초기화

	CommonStateEnd();
}

void CX2GUAdd::DashComboXStateEndFuture()
{
	/// 뒤로 돌아 주세요!
	CommonStateStartFuture();

	m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;
}

/// ANSI_DASH_COMBO_XX
void CX2GUAdd::DashComboXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_DASH_COMBO_XXX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_COMBO_XXX
void CX2GUAdd::DashComboXXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_JUMP_COMBO_Z
void CX2GUAdd::DashJumpComboZEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_DASH_JUMP_COMBO_ZZ )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_JUMP_COMBO_ZZ
void CX2GUAdd::DashJumpComboZZEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneZ == true, ANSI_DASH_JUMP_COMBO_ZZZ )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_JUMP_COMBO_ZZZ
void CX2GUAdd::DashJumpComboZZZEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_JUMP_COMBO_X
void CX2GUAdd::DashJumpComboXEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_DASH_JUMP_COMBO_XX_START )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_DASH_JUMP_COMBO_XX_START
void CX2GUAdd::DashJumpComboXXStartEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ANSI_DASH_JUMP_COMBO_XX_CHARGE );
	}

	CommonEventProcess();
}

void CX2GUAdd::DashJumpComboXXStartStateEndFuture()
{
	CommonStateEndFuture();

	/// 차징 사운드 재생
	PlayLoopSound( L"Add_Effect10.ogg", true, false );
}

/// ANSI_DASH_JUMP_COMBO_XX_CHARGE
void CX2GUAdd::DashJumpComboXXLoopStateStart()
{
	CommonStateStart();

	m_fChargeTime = 0.f;	/// 충전 시간 초기화
	SetNoDetonation( true );
}

void CX2GUAdd::DashJumpComboXXLoopFrameMove()
{
	m_fChargeTime += m_fElapsedTime;

	/// 차지 연산값 설정 ( 최대 차지 연산값 제한 )
	const float fChargeValue = ( _CONST_ADD_::DASH_JUMP_COMBO_X2_MAX_CHARGE_TIME > m_fChargeTime ) 
							   ? _CONST_ADD_::DASH_JUMP_COMBO_X2_MAX_CHARGE_TIME : m_fChargeTime;

	CommonFrameMove();
}

void CX2GUAdd::DashJumpComboXXLoopEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	/// X를 누르고 있지 않거나, 최대 유지 시간을 넘으면 발사 스테이트로 이동
	else if ( false == m_InputData.pureX || _CONST_ADD_::DASH_JUMP_COMBO_X2_MAX_REMAIN_TIME <= m_FrameDataNow.unitCondition.fStateTime )
	{
		StateChange( ANSI_DASH_JUMP_COMBO_XX_FINISH );
	}

	CommonEventProcess();
}

void CX2GUAdd::DashJumpComboXXLoopStateEnd()
{
	CommonStateEnd();

	SetNoDetonation( false );
}

/// ANSI_DASH_JUMP_COMBO_XX_FINISH
void CX2GUAdd::DashJumpComboXXFinishStateStartFuture()
{
	CommonStateStartFuture();

	/// 차징 사운드 종료
	StopLoopSound();
}

void CX2GUAdd::DashJumpComboXXFinishFrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
	{
		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		{
			float fChargeRate = 1.f;		/// 차지 배율

			/// 완충이라면, 100% 증가로 설정
			if ( _CONST_ADD_::DASH_JUMP_COMBO_X2_MAX_REMAIN_TIME <= m_fChargeTime )
				fChargeRate = 2.f;
			else	/// 완충이 아니라면, 최대 75% 증가로 설정
				fChargeRate = 1.f + ( 0.25f * m_fChargeTime );

			/// 추가 데미지 배율
			const float fPowerRate = GetPowerRate() * fChargeRate;

			CX2DamageEffect::CEffect* pDamageEffect = 
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_AN_Dash_Jump_Combo_XX_Bullet", fPowerRate, GetBonePos( L"EF01" ), 
															  GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			
			/// 발사체 크기 설정
			if ( NULL != pDamageEffect && NULL != pDamageEffect->GetMainEffect() )
				pDamageEffect->SetScale( pDamageEffect->GetMainEffect()->GetScale() * fChargeRate );
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::DashJumpComboXXFinishEventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_NP_BARRIER_ACTIVE
void CX2GUAdd::NasodBarrierActiveStateStartFuture()
{
	/// DP 소모
	UpNowDPValue( - m_fUseDPByNasodBarrier );

	CommonStateStartFuture();
}

void CX2GUAdd::NasodBarrierActiveEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_READY,
void CX2GUAdd::NasodArmorReadyEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ANSI_COMBO_NASOD_ARMOR_ATTACK );
	}
		
	/// 캔슬되면 환상 추적자 이상 동작 한다... 기획적으로도 캔슬 구간은 없다고 했으니, 지워 버리자
#ifndef SERV_ADD_LUNATIC_PSYKER // 김태환
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //SERV_ADD_LUNATIC_PSYKER

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_ATTACK,
void CX2GUAdd::NasodArmorAttackStartFuture()
{
	CommonStateStartFuture();

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	bool		bIsNPC			= m_NasodArmorAttackTargetData.m_bIsNPC;		/// NPC 인 여부
	UidType		uidTargetUID	= m_NasodArmorAttackTargetData.m_uidTargetUID;	/// 타겟 UID

	/// 스킬을 통한 나소드 아머 콤보를 사용중일 땐, 해당 정보 객체를 사용하자
	if ( CX2SkillTree::SI_NONE != m_eNasodArmorAttackActiveSkillID )
	{
		bool bIsSetTargetData = false;	/// 타겟 설정 여부

		/// 타겟 정보 검사 및 설정
		BOOST_FOREACH( FantasyTracerTargetInfo& TargetInfo, m_vecFantasyTracerTargetInfo )
		{
			if ( m_eNasodArmorAttackActiveSkillID == TargetInfo.m_eSkillID )
			{
				bIsNPC			= TargetInfo.m_NATargetData.m_bIsNPC;
				uidTargetUID	= TargetInfo.m_NATargetData.m_uidTargetUID;

				bIsSetTargetData = true;
			}
		}

		/// 타겟 설정 실패시, 패스
		if ( false == bIsSetTargetData )
			return;
	}
#else // SERV_ADD_LUNATIC_PSYKER
	const bool			bIsNPC			= m_NasodArmorAttackTargetData.m_bIsNPC;		/// NPC 인 여부
	const UidType		uidTargetUID	= m_NasodArmorAttackTargetData.m_uidTargetUID;	/// 타겟 UID
#endif // SERV_ADD_LUNATIC_PSYKER

	const CX2GameUnit* pUnit = NULL;		/// 타겟 대상

	if ( true == bIsNPC )	/// NPC일 때
	{
		CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( static_cast<int>( uidTargetUID ) );

		if ( NULL != pNPC )
			pUnit = static_cast<CX2GameUnit*>( pNPC );		/// 타겟 설정
	}
	else					/// 유저일 때
	{
		CX2GUUser* pUser = g_pX2Game->GetUserUnitByUID( uidTargetUID );

		if ( NULL != pUser )
			pUnit = static_cast<CX2GameUnit*>( pUser );		/// 타겟 설정
	}

	/// 죽거나 같은 팀이거나 무적이면, 제외
	if ( NULL == pUnit ||
		 0.f >= pUnit->GetNowHp() ||
		 pUnit->GetTeam() == GetTeam() ||
		 true == pUnit->GetInvincible() )
		return;

	/// 유효한 라인맵인지 검사
	if ( NULL == g_pX2Game ||
		 NULL == g_pX2Game->GetWorld() ||
		 NULL == g_pX2Game->GetWorld()->GetLineMap() )
		 return;

	const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( pUnit->GetLastTouchLineIndex() );

	if( NULL == pLineData )
		return;

	if( pLineData->lineType != CKTDGLineMap::LT_NORMAL || pLineData->bEnable == false ) //라인맵이 유효한지 체크
		return;

#ifdef FIX_AISHA_TELEPORT_POSITON
	if( pUnit->GetPos().y < pLineData->startPos.y )
		return; 
#endif //FIX_AISHA_TELEPORT_POSITON

	/// 모든 조건을 충족하므로, 위치 대입
	D3DXVECTOR3 vOffSetMovePos;		/// OffSet을 대입한 순간 이동 위치
	
	/// 뒤로 OffSet 만큼 위치의 라인맵 존재 여부 검사, 실패하면 그냥 OffSet 없이 재자리에 설정
	if ( true == g_pX2Game->GetWorld()->GetLineMap()->GetFixedDistancePositionForWeaponTaker( &pUnit->GetPos(), 100.f, !GetIsRight(), vOffSetMovePos ) )
	{
		vOffSetMovePos.y = pUnit->GetPos().y;

		m_FrameDataFuture.syncData.position = vOffSetMovePos;
	}
	else
	{
		m_FrameDataFuture.syncData.position	= pUnit->GetPos();
	}
}

void CX2GUAdd::NasodArmorAttackEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( ANSI_COMBO_NASOD_ARMOR_LANDING );
	}

	/// 캔슬되면 환상 추적자 이상 동작 한다... 기획적으로도 캔슬 구간은 없다고 했으니, 지워 버리자
#ifndef SERV_ADD_LUNATIC_PSYKER // 김태환
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#endif //SERV_ADD_LUNATIC_PSYKER

	CommonEventProcess();
}

///	ANSI_COMBO_NASOD_ARMOR_LANDING,
void CX2GUAdd::NasodArmorLandingEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
								 m_InputData.oneZ == true, ANSI_COMBO_NASOD_ARMOR_Z )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_NASOD_ARMOR_X )

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

void CX2GUAdd::NasodArmorLandingEndFuture()
{
	/// 타겟 유닛
	const CX2GameUnit* pTargetUnit = m_NasodArmorAttackTargetData.GetTargetUnit();

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 스킬을 통한 나소드 아머 콤보를 사용중일 땐, 해당 정보 객체를 사용하자
	if ( CX2SkillTree::SI_NONE != m_eNasodArmorAttackActiveSkillID )
	{
		bool bIsSetTargetData = false;	/// 타겟 설정 여부

		/// 타겟 정보 검사 및 설정
		BOOST_FOREACH( FantasyTracerTargetInfo& TargetInfo, m_vecFantasyTracerTargetInfo )
		{
			if ( m_eNasodArmorAttackActiveSkillID == TargetInfo.m_eSkillID )
			{
				pTargetUnit		= TargetInfo.m_NATargetData.GetTargetUnit();

				bIsSetTargetData = true;
			}
		}

		/// 공격시 사용할 타겟 스킬 아이디 초기화
		m_eNasodArmorAttackActiveSkillID = CX2SkillTree::SI_NONE;

		/// 타겟 설정 실패시, 패스
		if ( false == bIsSetTargetData )
			return;
	}
#endif //SERV_ADD_LUNATIC_PSYKER

	/// 타겟의 방향을 바라보도록 설정
	if ( NULL != pTargetUnit )
	{
		const bool	bRight	= IsRightTarget( pTargetUnit->GetPos() );	/// 타겟이 있는 방향
		bool		bFlip	= false;									/// 방향 전환 여부

		/// 타겟의 방향을 바라보고 있는지 검사
		if( ( false == bRight	&& true		== GetIsRight() ) ||
			( true	== bRight	&& false	== GetIsRight() ) )
		{
			bFlip = true;
		}

		/// 타겟을 바라보고 있지 않다면, 방향 전환 수행
		if ( true == bFlip )
			m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
	}

	CommonStateEndFuture();
}

/// ANSI_COMBO_NASOD_ARMOR_Z
void CX2GUAdd::NasodArmorComboZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
								 m_InputData.oneZ == true, ANSI_COMBO_NASOD_ARMOR_ZZ )

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_ZZ
void CX2GUAdd::NasodArmorComboZZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
								( CX2Unit::UC_ADD_LUNATIC_PSYKER == GetUnitClass() || CX2Unit::UC_ADD_LUNATIC_PSYKER == GetUnitClass() ) &&
								m_InputData.oneZ == true && 
								( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ||
								( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) )
								, ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_START )
#endif //SERV_ADD_LUNATIC_PSYKER

	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
								 m_InputData.oneZ == true, ANSI_COMBO_NASOD_ARMOR_ZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_NASOD_ARMOR_ZZX )

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_ZZZ
void CX2GUAdd::NasodArmorComboZZZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_ZZX
void CX2GUAdd::NasodArmorComboZZXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_NASOD_ARMOR_ZZXX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_ZZXX
void CX2GUAdd::NasodArmorComboZZXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_X
void CX2GUAdd::NasodArmorComboXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
								 ( CX2Unit::UC_ADD_LUNATIC_PSYKER == GetUnitClass() || CX2Unit::UC_ADD_LUNATIC_PSYKER == GetUnitClass() ) &&
								 m_InputData.oneX == true && 
								 ( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ||
								   ( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) )
								, ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionX_LOOP )
#endif //SERV_ADD_LUNATIC_PSYKER

	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_NASOD_ARMOR_XX )

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
void CX2GUAdd::NasodArmorComboXStateEndFuture()
{
	m_iComboLoopCount		= 0;			/// 콤보 카운트 초기화

	CommonStateEndFuture();
}
#endif //SERV_ADD_LUNATIC_PSYKER

/// ANSI_COMBO_NASOD_ARMOR_XX
void CX2GUAdd::NasodArmorComboXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
								 m_InputData.oneX == true, ANSI_COMBO_NASOD_ARMOR_XXX )
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/// ANSI_COMBO_NASOD_ARMOR_XXX
void CX2GUAdd::NasodArmorComboXXXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		m_PhysicParam.nowSpeed.x = 550.f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

#pragma endregion //커맨드 함수


#pragma region 스패셜 액티브

/// 보이드 브레이커
void CX2GUAdd::ANSI_SA_AN_VOID_BREAKER_Init()
{
	TextureReadyInBackground(L"ADD_CYCLE_COA02.tga");
	TextureReadyInBackground(L"VOID_BREAKER_A.tga");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.x");
	XMeshReadyInBackground(L"ADD_ShockWave003.Y");
	XMeshReadyInBackground(L"ADD_MAGNETRON_DYNAMO_Spark2.Y");
	XMeshReadyInBackground(L"Condense_Light01.dds");
}

void CX2GUAdd::ANSI_SA_AN_VOID_BREAKER_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
	{
		D3DXVECTOR3 vPos	= GetBonePos( L"EF01" );
		vPos.x				+= 30.f;

		float fPowerRate = GetPowerRate();
		float fScaleRate = 1.f;
		/// 변이 포인트가 있다면, 공격력과 크기를 증가 시키자
		if ( 0 < GetMutationCount() && true == GetIsFormationMode() )
		{
			/// 변이 포인트 소모량 계산
			const int iUseMutationCount = min( GetMutationCount(), _CONST_ADD_::CONQUEROR_MAX_USE_MUTATION_COUNT );

			/// 공격력 수정
			const float fSADamageValue	= GetSpecialAbilityValue( CX2SkillTree::SA_DAMAGE_REL, CX2SkillTree::SI_SA_AN_VOID_BREAKER ) - 1.f;
			const float fDamageValue	= static_cast<float>( ( fSADamageValue * iUseMutationCount ) + 1.f );
			fPowerRate *= fDamageValue;

			// 크기 수정
			const float fSAScaleValue	= GetSpecialAbilityValue( CX2SkillTree::SA_SIZE_ABS, CX2SkillTree::SI_SA_AN_VOID_BREAKER ) - 1.f ;
			const float fSizeValue		= static_cast<float>( ( fSAScaleValue * iUseMutationCount ) + 1.f );
			fScaleRate *= fSizeValue;

			UpMutationCount( -iUseMutationCount );	/// 변이 포인트 소진
		}

		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_AN_Void_Breaker_Projectile", fPowerRate, 
																				vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		/// 크기 수정
		if( false == IsSamef(fScaleRate,1.f) &&
			NULL != pDamageEffect )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMesh = pDamageEffect->GetMainEffect();
			if ( NULL != pMesh )
			{
				pMesh->SetScale( pMesh->GetScale() * fScaleRate );
			}
		}

	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_AN_VOID_BREAKER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 파티클 프리즘
void CX2GUAdd::ANSI_SA_AN_PARTICLE_PRISM_Init()
{
	XSkinMeshReadyInBackground(L"AN_PARTICLE_PRISM_Dummy01.X");
	XMeshReadyInBackground(L"AN_PARTICLE_PRISM_04.Y");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Add_Gravity_Ball.tga");
	TextureReadyInBackground(L"ADD_CYCLE_COA01.Tga");
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
}

void CX2GUAdd::ANSI_SA_AN_PARTICLE_PRISM_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
	{
		DestroyMinePrismNPC();
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hParticlePrsim );

		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		{
			float fRotateRadianY = D3DXToRadian( GetRotateDegree().y ) + 1.57f;	/// 회전값

			/// 앞 NPC 소환
			D3DXVECTOR3 vStartNPCPos = GetPos();
			vStartNPCPos.z += 50.f * cos( fRotateRadianY );
			vStartNPCPos.x += 50.f * sin( fRotateRadianY );
				
			int iHyperMode = true == IsHyperState() ? 1 : 0;
			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_PARTICLE_PRISM_START, iHyperMode, true, vStartNPCPos, 
				GetIsRight(), 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>(GetTeam()), CX2NPCAI::NAT_ALLY, GetUnitUID() );

			/// 뒤 NPC 소환
			D3DXVECTOR3 vEndNPCPos		= GetPos();

#ifdef FIX_EFFECT_SCALE_BY_UNIT_SCALE // 김태환
			/// 유닛 크기에 따른 간격 조정
			const float	fOffSetLength	= 650.f * m_vTransScale.x;

			vEndNPCPos.z += fOffSetLength * cos( fRotateRadianY );
			vEndNPCPos.x += fOffSetLength * sin( fRotateRadianY );
#else //FIX_EFFECT_SCALE_BY_UNIT_SCALE // 김태환
			vEndNPCPos.z += 650.f * cos( fRotateRadianY );
			vEndNPCPos.x += 650.f * sin( fRotateRadianY );
#endif //FIX_EFFECT_SCALE_BY_UNIT_SCALE // 김태환

			g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_PARTICLE_PRISM_END, iHyperMode, true, vEndNPCPos, 
				GetIsRight(), 0.f, true, -1, static_cast<CX2Room::TEAM_NUM>(GetTeam()), CX2NPCAI::NAT_ALLY, GetUnitUID() );


			// 이펙트 생성
			{
				/// 변이 포인트가 있다면, 디버프 효과 강화
				bool bUasMutationPoint = false;
				if ( 0 < GetMutationCount() && true == GetIsFormationMode() )
				{
					bUasMutationPoint = true;
					SetMutationCount( GetMutationCount() - 1);
				}		

				m_hParticlePrsim =
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_AN_PARTICLE_PRISM", this,
					NULL, bUasMutationPoint, GetPowerRate(), -1.f, D3DXVECTOR3(1,1,1),
					true, vStartNPCPos, GetRotateDegree(), GetDirVector() );
			}

		}
	}
	CommonFrameMove();
}
void CX2GUAdd::ANSI_SA_AN_PARTICLE_PRISM_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}
/** @function : IsExistingMinePrismNPC
	@brief : 내 소유의 프리즘 NPC 소멸시키기
*/
void CX2GUAdd::DestroyMinePrismNPC() const
{
	for( int i=0; i < g_pX2Game->GetNPCUnitListSize(); ++i)
	{
		CX2GUNPC* pNPC = g_pX2Game->GetNPCUnit(i);
		if( NULL != pNPC )
		{
			UidType uiOwnerUnitUID = pNPC->GetOwnerGameUnitUID();
			if( -1 != uiOwnerUnitUID && 
				GetUnitUID() == uiOwnerUnitUID )
			{
				switch(pNPC->GetNPCTemplet().m_nNPCUnitID)
				{
				case CX2UnitManager::NUI_PARTICLE_PRISM_START:
				case CX2UnitManager::NUI_PARTICLE_PRISM_END:
					{
						pNPC->SetNowHp(0.f);
					} break;
				default:
					break;
				}
			}
		}
	}
}


/// 에너지 붐
void CX2GUAdd::ANSI_SA_AN_ENERGY_BOOM_Init()
{
	TextureReadyInBackground(L"ADD_CYCLE_COA01.tga" );
	TextureReadyInBackground(L"ADD_CYCLE_COA02.tga" );
	TextureReadyInBackground(L"ADD_CYCLE_COA03.tga" );
	TextureReadyInBackground(L"Add_ENERGY_BOOM_A.tga" );
	XMeshReadyInBackground( L"Up_Impact01.Y" );
	XMeshReadyInBackground( L"ADD_Effect_Line02.Y" );
	XSkinMeshReadyInBackground( L"ADD_Effect_Shock_01.x" );
	XSkinMeshReadyInBackground( L"ADD_ENERGY_BOOM_Stone.X" );
	XSkinMeshReadyInBackground( L"ADD_ENERGY_BOOM_Stone02.X" );
	XSkinMeshReadyInBackground( L"ADD_ENERGY_BOOM_A.X" );
	XMeshReadyInBackground( L"ADD_ShockWave003.Y" );
	TextureReadyInBackground(L" Black_Circle01.dds" );
	TextureReadyInBackground(L" Black_Light.dds" );
	TextureReadyInBackground(L" EffectSet_Riding_Dark_HAMELLING_Skill_C03.tga" );
	TextureReadyInBackground(L" add_EFFECT_Cycle_Blur01.dds" );
}

void CX2GUAdd::ANSI_SA_AN_ENERGY_BOOM_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.527f ) )
	{
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01" );

		/// 실제 데미지 이펙트
		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SI_SA_AN_Energy_Boom_Projectile", GetPowerRate(), 
												  vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		/// 변이 포인트가 있다면, 사거리를 증가 시키자
		if ( 0 < GetMutationCount() && NULL != pDamageEffect && true == GetIsFormationMode() )
		{
			/// 사정 거리 수정
			CKTDGXMeshPlayer::CXMeshInstance* pMesh = pDamageEffect->GetMainEffect();

			if ( NULL != pMesh )
			{
				const float fSARangeValue	= GetSpecialAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, CX2SkillTree::SI_SA_AN_ENERGY_BOOM );
				const float fIncreaseRange	= pMesh->GetMaxLifeTime() * fSARangeValue;

				pMesh->SetMaxLifeTime( fIncreaseRange );
			}

			UpMutationCount( -1 );	/// 변이 포인트 소모
		}
	}

	CommonFrameMove();
}
void CX2GUAdd::ANSI_SA_AN_ENERGY_BOOM_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 파티클 액셀러레이터
void CX2GUAdd::ANSI_SA_AN_PARTICLE_ACCELERATOR_Init()
{
	XSkinMeshReadyInBackground(L"ADD_PARTICLE_ACCELERATOR_DUMMY01.X");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	XMeshReadyInBackground( L"ADD_PARTICLE_ACCELERATOR_SPARK01.Y" );
	XSkinMeshReadyInBackground( L"SA_AEM_CHAIN_LIGHTNING_P01.X" );
}

void CX2GUAdd::ANSI_SA_AN_PARTICLE_ACCELERATOR_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 1.35f ) )
	{
		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		{
			float fPowerRate = GetPowerRate();

			/// 변이 게이지가 있다면, 데미지 증가 처리 ( 3개 까지 )
			if ( 0 < GetMutationCount() && true == GetIsFormationMode() )
			{
				/// 변이 포인트 소모량 계산
				const int iUseMutationCount = min( GetMutationCount(), _CONST_ADD_::PARTICLE_ACCELERATOR_MAX_USE_MUTATION_COUNT );

				const float fIncreaseDamageRate = GetSpecialAbilityValue( CX2SkillTree::SA_DAMAGE_REL, CX2SkillTree::SI_SA_AN_PARTICLE_ACCELERATOR ) - 1.f;
				fPowerRate *= ( ( fIncreaseDamageRate * iUseMutationCount ) + 1.f ) ;
				UpMutationCount( -iUseMutationCount );
			}

			D3DXVECTOR3 vPos = GetBonePos( L"Bip01_Spine1" );

			/// 실제 데미지 이펙트
			CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SA_AN_Particle_Accelerator_Attack", fPowerRate, 
																					vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

			/// 효과음 이펙트 출력
			SetEffectSoundParticle( GetBonePos( L"Bip01" ), L"ADD_STR_Kwagwang" );
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_AN_PARTICLE_ACCELERATOR_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 뉴트란 밤
void CX2GUAdd::ANSI_SA_AN_NEUTRON_BOMB_Init()
{
	TextureReadyInBackground(L"ADD_CYCLE_COA01.tga");
	TextureReadyInBackground(L"ADD_Effect_CIRCLE01.tga");
	TextureReadyInBackground(L"Add_Gravity_Ball.tga");
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"NEUTRON_BOMB_Impact.tga");
	TextureReadyInBackground(L"PlusHPMP.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}

void CX2GUAdd::ANSI_SA_AN_NEUTRON_BOMB_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 펄스 캐논
void CX2GUAdd::ANSI_SA_APT_PULSE_CANNON_READY_Init()
{
	XSkinMeshReadyInBackground(L"CHUNG_Cannon_Effect01.X");
	XSkinMeshReadyInBackground(L"CHUNG_Cannon_Effect02.X");
	XSkinMeshReadyInBackground(L"CHUNG_Cannon_Effect03.X");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Add_Gravity_Ball.tga");
}

void CX2GUAdd::ANSI_SA_APT_PULSE_CANNON_READY_StateStartFuture()
{
	/// 키 입력 제어 객체 초기화
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );

	CommonStateStartFuture();
}

void CX2GUAdd::ANSI_SA_APT_PULSE_CANNON_READY_StateStart()
{
	/// 펄스 캐논 발사 인덱스 설정
	vector<int> vecTempIndex = m_vecPulseBulletFireIndex;
	m_vecPulseBulletFireIndex.resize( 0 );

	/// 1부터 6까지의 값을 임의로 정렬한다. ( Sync를 위해, 직접 기능을 만들어 쓰자... )
	for( int i = static_cast<int>( vecTempIndex.size() ); 0 < i; --i )
	{
		const int iRandomNumb = RandomInt( 1, i );

		if( static_cast<UINT>( iRandomNumb ) > vecTempIndex.size() )
			continue;

		m_vecPulseBulletFireIndex.push_back( vecTempIndex[iRandomNumb -1] );

		vecTempIndex.erase( vecTempIndex.begin() + iRandomNumb - 1 );
	}

	m_iComboLoopCount = 0;	/// 반복 카운트 초기화

	CommonStateStart();
}

void CX2GUAdd::ANSI_SA_APT_PULSE_CANNON_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ANSI_SA_APT_PULSE_CANNON_LOOP );
	}

	CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_APT_PULSE_CANNON_LOOP_FrmaeMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
	{
		/// 펄스 캐논 발사 인덱스 설정 ( 7발째 이상은 0번 인덱스부터 )
		int iWeaponIndex = ( 6 <= m_iComboLoopCount ) ? m_iComboLoopCount - 6 : m_iComboLoopCount;

		/// 벡터 인덱스를 넘어간다면, 0으로 설정
		if ( m_vecPulseBulletFireIndex.size() < static_cast<UINT>( iWeaponIndex + 1 ) )
			iWeaponIndex = 0;

		/// 무기 본에 따른 위치 설정
		WCHAR buf[256] = {0,};

		StringCchPrintf( buf, 255, L"Weapon0%d", m_vecPulseBulletFireIndex[ iWeaponIndex ] );	/// Weapon01 ~ Weapon06

		D3DXVECTOR3 vPos = GetBonePos( buf );


		/// 데미지 이펙트 생성
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SA_APT_Pulse_Cannon_Projectile", GetPowerRate(), vPos, 


			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_APT_PULSE_CANNON_LOOP_EventProcess()
{
	/// 키 입력 제어 객체에서 슬롯이 변경 되어 있는지 검사
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		++m_iComboLoopCount;

		const bool bIsUnPushKey		= m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData );

		if ( _CONST_ADD_::PULSE_CANNON_MIN_FIRE_COUNT > m_iComboLoopCount || 
			 ( _CONST_ADD_::PULSE_CANNON_MAX_FIRE_COUNT > m_iComboLoopCount && false == bIsUnPushKey && 
			   true == ConsumeMPorDP( _CONST_ADD_::PULSE_CANNON_CONSUME_MP_VALUE, _CONST_ADD_::PULSE_CANNON_CONSUME_DP_VALUE ) ) )
		{
			StateChange( ANSI_SA_APT_PULSE_CANNON_LOOP );
		}
		else
		{
			StateChange( ANSI_SA_APT_PULSE_CANNON_FINISH );
		}
	}

	CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_APT_PULSE_CANNON_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// EMP 쇼크
void CX2GUAdd::ANSI_SA_APT_EMP_SHOCK_Init()
{
	TextureReadyInBackground(L"ADD_Effect_Triangle01.Tga");
	TextureReadyInBackground(L"Arme_Aging_Gate01.tga");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"EFFECT_Cycle_LensFlare01.dds");
}

void CX2GUAdd::ANSI_SA_APT_EMP_SHOCK_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.961f ) )
	{
		if ( NULL != g_pX2Game && NULL != GetUnit() )
		{
			/// 스킬 레벨 반환
			const CX2UserSkillTree& userSkillTree	= GetUnit()->GetUnitData().m_UserSkillTree;
			const int				iSkillLevel		= userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_APT_EMP_SHOCK );

			BOOST_FOREACH( CX2GameUnit* pGameUnit, g_pX2Game->GetUnitList() )
			{
				if ( NULL == pGameUnit )
					continue;

				/// 같은 팀이면 패스
				if ( GetTeam() == pGameUnit->GetTeam() )
					continue;


				/// NPC일 때, 일반 적 NPC가 아니라면 패스
				if( GUT_NPC == pGameUnit->GetGameUnitType() )
				{
					CX2GUNPC* pNPC = static_cast<CX2GUNPC*>( pGameUnit );

					if ( NULL == pNPC || CX2UnitManager::NCT_BASIC != pNPC->GetNPCTemplet().m_ClassType )
						continue;
				}

				/// 범위에 따른 디버프 설정
				const float fDistance = GetDistance( GetPos(), pGameUnit->GetPos() );

				if ( _CONST_ADD_::EMP_SHOCK_NEAR_RANGE >= fDistance )		/// 직접 타격 범위
					pGameUnit->SetBuffFactorToGameUnitByBuffFactorID( BFI_DEBUFF_EMP_SHOCK_NEAR, iSkillLevel );
				else if ( _CONST_ADD_::EMP_SHOCK_FAR_RANGE >= fDistance )	/// 간접 타격 범위
					pGameUnit->SetBuffFactorToGameUnitByBuffFactorID( BFI_DEBUFF_EMP_SHOCK_FAR, iSkillLevel );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_APT_EMP_SHOCK_EventProcess()
{
	bool& bUseEmpShokOnLine = m_FrameDataFuture.stateParam.bEventFlagList[1];

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( false == bUseEmpShokOnLine && true == IsOnSomethingFuture() )
			StateChange( USI_JUMP_LANDING );
		else
			StateChange( USI_WAIT );
	}
	else if( 0.3 > m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		bUseEmpShokOnLine = IsOnSomethingFuture();
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 퀘이크 버스터
void CX2GUAdd::ANSI_SA_APT_QUAKE_BUSTER_Init()
{
	TextureReadyInBackground(L"ADD_CYCLE_COA01.Tga");
	TextureReadyInBackground(L"ADD_Effect_Triangle01.Tga");
	TextureReadyInBackground(L"ADD_ShockWave.dds");
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}

void CX2GUAdd::ANSI_SA_APT_QUAKE_BUSTER_EventProcess()
{
	if ( true == IsOnSomethingFuture() && 1.2f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}
void CX2GUAdd::ANSI_SA_APT_QUAKE_BUSTER_FrameMove()
{
	CommonFrameMove();
	
	const float fEventTimer = ( true == IsHyperState() ) ? 1.f: 0.6f;
	if( m_pXSkinAnim->EventTimerOneshot( fEventTimer ) )
	{
		float fPowerRate = GetPowerRate();
		if ( 0 < GetMutationCount() && true == GetIsFormationMode() )
		{
			const float fChangePowerRate = 
				GetSpecialAbilityValue( CX2SkillTree::SA_DAMAGE_REL, CX2SkillTree::SI_SA_APT_QUAKE_BUSTER );

			if( 0 < fChangePowerRate )
				fPowerRate *= fChangePowerRate;

			// 퀘이크 버스트는 변이포인트 1개 사용
			SetMutationCount( GetMutationCount() - 1 );
		}	

		if( true == IsHyperState() )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Quake_Buster_Hyper", this, NULL, IsHyperState(), fPowerRate );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Quake_Buster", this, NULL, IsHyperState(), fPowerRate );
	}
}
void CX2GUAdd::ANSI_SA_APT_QUAKE_BUSTER_HYPER_EventProcess()
{
	if ( true == IsOnSomethingFuture() && 1.4f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 스테이시스 필드
void CX2GUAdd::ANSI_SA_APT_STASIS_FIELD_Init()
{
	TextureReadyInBackground(L"ADD_ShockWave.dds");
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	TextureReadyInBackground(L"ADD_CYCLE_COA03.tga");
	TextureReadyInBackground(L"ADD_Dark_Cycle_Attack_2.tga");
	TextureReadyInBackground(L"Arme_Aging_Gate01.tga");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"EFFECT_Cycle_LensFlare01.dds");
}

void CX2GUAdd::ANSI_SA_APT_STASIS_FIELD_StateStart()
{
	/// 스테이시스 필드 활성 및 유지 시간 설정
	m_StasisfieldData.m_fRemainTime	= _CONST_ADD_::STASIS_FIELD_REMAIN_TIME;

	/// 스킬 아이콘 갱신
	if ( IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI();

	CommonStateStart();
}

void CX2GUAdd::ANSI_SA_APT_STASIS_FIELD_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
	{
		/// 카운트 다운용 이펙트 설정
		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() )
		{
			CX2EffectSet::Handle hLoopEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_APT_STASIS_FIELD_Loop", this, 
				NULL, false, -1.f, _CONST_ADD_::STASIS_FIELD_REMAIN_TIME );

			if ( INVALID_EFFECTSET_HANDLE != hLoopEffectSet )
				m_StasisfieldData.m_hLoopEffectSet = hLoopEffectSet;
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_APT_STASIS_FIELD_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_APT_STASIS_FIELD_ACTIVE_StateStart()
{
	/// 스테이시스 유지 시간 초기화 
	/// ( 유지 시간만 초기화 하는 이유는, 데미지 이펙트 생성 전 까지 피격 및 타격 누적값을 유지해야 하기 때문에 )
	m_StasisfieldData.m_fRemainTime = 0.f;

	/// 피격/타격에 의한 수치 조절 배율 -> 범위 : 100~150%
	/// 수식 : ( 1 + min( 쌓인 데미지/자신의 최대HP , 0.5) )				
	const float fTotalValue = m_StasisfieldData.m_fHitValue + m_StasisfieldData.m_fDamageValue;
	m_StasisfieldData.m_fChangeRate = ( 1.f + min( fTotalValue / GetMaxHp(), 0.5f ) );

	/// 스킬 아이콘 갱신
	if ( IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI();

	CommonStateStart();
}

void CX2GUAdd::ANSI_SA_APT_STASIS_FIELD_ACTIVE_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
	{
		// 이펙트셋 생성
		CX2EffectSet::Handle hEffectSet =
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_APT_Stasis_Field_Activate", this);
		CX2EffectSet::EffectSetInstance* pEffectSet =
			g_pX2Game->GetEffectSet()->GetEffectSetInstance(hEffectSet);
		if( NULL != pEffectSet )
		{

			D3DXVECTOR3 vScale = pEffectSet->m_vScale * m_StasisfieldData.m_fChangeRate;
			pEffectSet->SetEffectScale( vScale, true );
		}
	}
	else if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
	{
		// 데미지 이펙트 생성
		const float fPowerRate = GetPowerRate() * m_StasisfieldData.m_fChangeRate;
		CX2DamageEffect::CEffect* pDamageEffect = 
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_APT_Stasis_Field_Activate", fPowerRate, 
			GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		if( NULL != pDamageEffect )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMesh = pDamageEffect->GetMainEffect();
			if( NULL != pMesh )
			{
				D3DXVECTOR3 vScale = pMesh->GetScale() * m_StasisfieldData.m_fChangeRate ;
				pMesh->SetScale( vScale );
			}
		}
		m_StasisfieldData.ResetStasisfieldData();
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_APT_STASIS_FIELD_ACTIVE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 퀀커러
void CX2GUAdd::ANSI_SA_APT_CONQUEROR_Init()
{
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_Spark2.Y" );
	XMeshReadyInBackground( L"ADD_PARTICLE_ACCELERATO_CYCLE1.Y" );
	XMeshReadyInBackground( L"ADD_PARTICLE_ACCELERATOR_SPARK02.Y" );
	XSkinMeshReadyInBackground(L"ADD_CONQUERER_DUMMY01.X");
}

void CX2GUAdd::ANSI_SA_APT_CONQUEROR_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}
/** @function : ANSI_SA_APT_CONQUEROR_FrameMove
	@brief : 각성 시 주먹 공격, 비각성 시 유도 미사일 공격
*/
void CX2GUAdd::ANSI_SA_APT_CONQUEROR_FrameMove()
{
	CommonFrameMove();

	if( m_pXSkinAnim->EventTimerOneshot( 1.3f ) )
	{
		if( true == GetIsFormationMode() )
		{
			float fPowerRate = GetPowerRate();
			/// 변이 포인트가 있다면, 공격력 증가 시키자
			if ( 0 < GetMutationCount() )
			{
				/// 변이 포인트 소모량 계산
				const int iUseMutationCount = min( GetMutationCount(), _CONST_ADD_::CONQUEROR_MAX_USE_MUTATION_COUNT );

				const float fSADamageValue	= GetSpecialAbilityValue( CX2SkillTree::SA_DAMAGE_REL, CX2SkillTree::SI_SA_APT_CONQUEROR ) - 1.f;
				const float fDamageValue	= static_cast<float>( ( fSADamageValue * iUseMutationCount ) + 1.f );
				fPowerRate *= fDamageValue;

				UpMutationCount( -iUseMutationCount );	/// 변이 포인트 소진
			}		

			if ( NULL != g_pX2Game->GetDamageEffect() )
			{
				D3DXVECTOR3 vPos;
				GetBonePos( L"Bip01", vPos );
				vPos.x += ( true == GetIsRight() ) ? 300.f : -300.f;
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SA_APT_Conqueror_Hyper", fPowerRate, 
					vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			}

		} 
		// 노각성 : 유도미사일
		else
		{
			int iProjectileCount = 12;

			/// 변이 포인트가 있다면, 발사체 수량 증가 시키자 -> 각성시에만 사용
			/*if ( 0 < GetMutationCount() )
			{
				/// 변이 포인트 소모량 계산
				const int iUseMutationCount = min( GetMutationCount(), _CONST_ADD_::CONQUEROR_MAX_USE_MUTATION_COUNT );

				const float fProjectileBaseCount = GetSpecialAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, CX2SkillTree::SI_SA_APT_CONQUEROR );
				iProjectileCount += static_cast<int>(fProjectileBaseCount) * iUseMutationCount;

				SetMutationCount( -iUseMutationCount );	/// 변이 포인트 소진
			}*/

			if ( NULL != g_pX2Game->GetDamageEffect() )
			{
				D3DXVECTOR3 vPos;
				GetBonePos( L"Bip01", vPos );
				vPos.x += ( true == GetIsRight() ) ? 200.f : -200.f;

				for( int i = 0; i < iProjectileCount; ++i )
				{
					D3DXVECTOR3 vRotateDegree = GetRotateDegree();
					vRotateDegree.y += -135.f + GetRandomInt(i) * 2;
					vRotateDegree.z += -135.f + GetRandomInt( iProjectileCount+i ) * 2;

					CX2DamageEffect::CEffect* pDamageEffect = 
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SA_APT_Conqueror_Projectile", GetPowerRate(), 
						vPos, vRotateDegree, GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

					SetLockOn( pDamageEffect, i, CX2DamageEffect::LOT_UID_VECTOR );
				}
			}
		}
	}
}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

/// ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_START
void CX2GUAdd::NasodArmorComboZZFrontZStartEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		m_iComboLoopCount = 0;	/// 콤보 반복 카운트 초기화
		StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}

/// ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP
void CX2GUAdd::NasodArmorComboZZFrontZLoopStateStartFuture()
{
	CommonStateStartFuture();

	/// 반복 횟수에 따라 애니메이션 속도 증가
	if ( 0 < m_iComboLoopCount )
	{
		const float fChangeSpeedValue = m_fAnimSpeedFuture + ( 0.1f * m_iComboLoopCount );
		ResetAnimSpeed( m_fAnimSpeedFuture, true, true, fChangeSpeedValue );
	}
}

void CX2GUAdd::NasodArmorComboZZFrontZLoopStateStart()
{
	CommonStateStart();

	/// 반복 횟수에 따라 애니메이션 속도 증가
	if ( 0 < m_iComboLoopCount )
	{
		const float fChangeSpeedValue = 1.f + ( 0.1f * m_iComboLoopCount );
		ResetAnimSpeed( fChangeSpeedValue, true, false, fChangeSpeedValue );
	}
}

void CX2GUAdd::NasodArmorComboZZFrontZLoopEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		++ m_iComboLoopCount;

		/// DP 없으면, 마무리 동작
		if ( 0.f >= GetDPValue() )
			StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX );
		/// 최소 반복 횟수를 만족하지 못하면, Loop 진행
		else if ( _CONST_ADD_::NASOD_ARMOR_ZZFrontZ_LOOP_MIN_COUNT > m_iComboLoopCount )
			StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP );
		/// 최대 반복 횟수 미만 이라면
		else if ( _CONST_ADD_::NASOD_ARMOR_ZZFrontZ_LOOP_MAX_COUNT > m_iComboLoopCount )
		{
			/// Z 키 입력이 있다면, Loop 진행
			if ( true == m_FrameDataFuture.stateParam.bEventFlagList[0] )
				StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZ_LOOP );
			/// 입력 없으면 마무리 동작
			else
				StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX );
		}
		else
			StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX );
	}
	/// 반복 횟수 미만 이면서 X 키 누르면 바로 마무리 동작
	else if ( _CONST_ADD_::NASOD_ARMOR_ZZFrontZ_LOOP_MIN_COUNT <= m_iComboLoopCount &&
		_CONST_ADD_::NASOD_ARMOR_ZZFrontZ_LOOP_MAX_COUNT >  m_iComboLoopCount &&
		true == m_InputData.oneX )
		StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX );
	ELSE_IF_SET_EVENT_FLAG( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, true == m_InputData.oneZ )

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}

/// ANSI_ALP_COMBO_NASOD_ARMOR_ZZFrontZX
void CX2GUAdd::NasodArmorComboZZFrontZXEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}

/// ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionX_LOOP
void CX2GUAdd::NasodArmorComboXDirectionXLoopEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		++ m_iComboLoopCount;

		/// DP 없으면, 마무리 동작
		if ( 0.f >= GetDPValue() )
			StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ );
		/// 최대 반복 횟수 미만 이라면
		else if ( _CONST_ADD_::NASOD_ARMOR_XDirectionX_LOOP_MAX_COUNT > m_iComboLoopCount )
		{
			/// X 키 입력이 있다면, Loop 진행
			if ( true == m_FrameDataFuture.stateParam.bEventFlagList[0] )
				StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionX_LOOP );
			/// 입력 없으면 마무리 동작
			else
				StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ );
		}
		else
			StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ );
	}
	/// 반복 횟수 미만 이면서 Z 키 누르면 바로 마무리 동작
	else if ( _CONST_ADD_::NASOD_ARMOR_XDirectionX_LOOP_MAX_COUNT >  m_iComboLoopCount &&
		true == m_InputData.oneZ )
		StateChange( ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ );
	ELSE_IF_SET_EVENT_FLAG( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, true == m_InputData.oneX )

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

void CX2GUAdd::NasodArmorComboXDirectionXLoopStateEndFuture()
{
	m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;

	CommonStateEndFuture();
}

/// ANSI_ALP_COMBO_NASOD_ARMOR_XDirectionXZ
void CX2GUAdd::NasodArmorComboXDirectionXZEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}


/// 리버스 리액터
void CX2GUAdd::ANSI_SA_ALP_REVERSE_REACTOR_Init()
{
	TextureReadyInBackground(L"ADD_CYCLE_COA01.tga");
	TextureReadyInBackground(L"ADD_Effect_Triangle01.Tga");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	TextureReadyInBackground(L"ADD_ShockWave.dds");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
	TextureReadyInBackground(L"Black_Light.dds");
	XMeshReadyInBackground( L"ADD_STASIS_FIELD_Spark.Y" );
	XMeshReadyInBackground( L"ADD_PHASE_SHIFT_ATTACK01.Y" );
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"ADD_Reverse_Reactor_Link01.X");
}

void CX2GUAdd::ANSI_SA_ALP_REVERSE_REACTOR_StateStart()
{
	m_ReverseReactorTargetInfo.ResetReverseReactorTargetInfo();

	CommonStateStart();
}

void CX2GUAdd::ANSI_SA_ALP_REVERSE_REACTOR_FrameMove()
{
	if ( m_pXSkinAnim->EventTimerOneshot( 0.31f ) )
	{
		/// 타겟팅 대상 찾기 ( 아이샤 에너지 드레인 복붙 )
		float fDistance = ( CX2Game::GT_PVP == g_pX2Game->GetGameType() ) ? _CONST_ADD_::REVERSE_REACTOR_ENABLE_ATTACK_RANGE_PVP
																		  : _CONST_ADD_::REVERSE_REACTOR_ENABLE_ATTACK_RANGE;
		CX2GameUnit* pFrontNearestUnit = NULL;

		for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
		{
			CX2GameUnit *pUnit = g_pX2Game->GetUnit( i );

			if ( NULL == pUnit )
				continue;
			if( g_pX2Game->IsValidUnit( pUnit ) == false )
				continue;
			if( GetTeam() == pUnit->GetTeam() )
				continue;
			if( pUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE || 
				pUnit->GetNowHp() <= 0.f ||
				pUnit->GetInvincible() == true )
				continue;					

			if( g_pX2Game->IsFront( (CX2GameUnit*)this, pUnit ) == false )
				continue;

			float fOffset = 0.f;
			if(pUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC)
			{
				CX2GUNPC *pNpc = static_cast<CX2GUNPC*>( pUnit );
				if( NULL == pNpc || CX2UnitManager::NCT_BASIC != pNpc->GetNPCTemplet().m_ClassType )
					continue;

				fOffset = ( pNpc->GetOrgUnitSize().x + pNpc->GetOrgUnitSize().y ) / 4.f;
			}					

			D3DXVECTOR3 vDistance = pUnit->GetPos() - GetPos();

			float fDistanceUnit = D3DXVec3Length( &vDistance ) - fOffset;

			if( fDistance >= fDistanceUnit )
			{
				fDistance = fDistanceUnit;
				pFrontNearestUnit = pUnit;
			}
		}

		if ( NULL != pFrontNearestUnit && NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		{
			/// 연결 데미지 이펙트 설정
			D3DXVECTOR3 vMyPos	=  GetBonePos( L"Bip01" );
			vMyPos.y			+= 25.f;

			D3DXVECTOR3	vEnemyPos;

			if ( false == pFrontNearestUnit->GetBonePos( L"Bip01", vEnemyPos ) )
			{
				vEnemyPos	=  pFrontNearestUnit->GetPos();
				vEnemyPos.y	+= 100.f;
			}
			else
			{
				vEnemyPos.y	+= 25.f;
			}

			/// 타겟 위치에 폭발 이펙트 생성
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_ALP_Reverse_Reactor", GetPowerRate(), 
														  vEnemyPos, GetRotateDegree(), GetRotateDegree() );

			/// 링크 이펙트 생성
			CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_Reverse_Reactor_Link", GetPowerRate(), 
																							  vMyPos, GetRotateDegree(), GetRotateDegree() );

			if ( NULL != pEffect )
			{
				/// 이펙트 연결 설정
				pEffect->SetLinkMainEffectByPos( vMyPos, vEnemyPos );

				/// 연결 사운드 출력
				if ( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDeviceManager() )
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Add_Reverse_Reactor_Connect.ogg" );

		#ifdef ADD_BALANCE_PATCH_140123 // 김태환
				/// 대전 배율 ( 대전시에는, 전이 데미지 및 최대 전이량을 감소 시켜준다. )
				const float fPVPRate = ( CX2Game::GT_PVP == g_pX2Game->GetGameType() ) ? 0.1f : 1.f;

				/// 최대 전이 데미지량
				const float fMaxTransitionDamage	= GetMaxHp() * fPVPRate;
				/// 전이 데미지 배율
				const float fTransitionDamageRate	= GetSpecialAbilityValue( CX2SkillTree::SA_DAMAGE_REL, CX2SkillTree::SI_SA_ALP_REVERSE_REACTOR ) * fPVPRate;
				/// 유지 시간
				const float fRemainTime				= GetSpecialAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, CX2SkillTree::SI_SA_ALP_REVERSE_REACTOR );
		#else //ADD_BALANCE_PATCH_140123
				/// 최대 전이 데미지량
				const float fMaxTransitionDamage	= ( CX2Game::GT_PVP == g_pX2Game->GetGameType() ) ? ( GetMaxHp() * 0.1f ) : GetMaxHp();
				/// 유지 시간
				const float fRemainTime				= GetSpecialAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, CX2SkillTree::SI_SA_ALP_REVERSE_REACTOR );
				/// 전이 데미지 배율
				const float fTransitionDamageRate	= GetSpecialAbilityValue( CX2SkillTree::SA_DAMAGE_REL, CX2SkillTree::SI_SA_ALP_REVERSE_REACTOR );
		#endif //ADD_BALANCE_PATCH_140123

				const bool	bIsNPC					= ( CX2GameUnit::GUT_NPC == pFrontNearestUnit->GetGameUnitType() ) ? true : false;

				/// 정보 저장
				m_ReverseReactorTargetInfo = ReverseReactorTargetInfo( fMaxTransitionDamage, 0.f, fRemainTime, fTransitionDamageRate, 
																	   pFrontNearestUnit->GetUnitUID(), bIsNPC, pEffect->GetHandle() );

				/// 디버프 적용
				pFrontNearestUnit->SetBuffFactorToGameUnitByBuffFactorID( BFI_DEBUFF_PLASMA_LINK );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_ALP_REVERSE_REACTOR_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
	GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 퀵실버 액셀
void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_ACCEL_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	XSkinMeshReadyInBackground(L"ADD_PSYCHIC_STORM_01.X");
	XMeshReadyInBackground( L"ADD_PHASE_SHIFT_ATTACK01.Y" );
		
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_ACCEL_FrameMove()
{
	/// 강타 이펙트 생성
	if ( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
	{
		/// 막타 강타 공격
		wstring wstrImpactDamageEffectName = L"DamageEffect_ALP_Quicksilver_Accel_Impact";

		/// 변이 포인트가 있으면, 스턴 데미지 이펙트 발생
		if ( true == GetIsFormationMode() && 0 < GetMutationCount() )
		{
			wstrImpactDamageEffectName.clear();
			wstrImpactDamageEffectName = L"DamageEffect_ALP_Quicksilver_Accel_Impact_Stun";

			UpMutationCount( -1 );
		}

		D3DXVECTOR3 vPos = GetIsRight() ? ( GetBonePos( L"Bip01" ) + ( GetDirVector() * 250.f ) ): 
										  ( GetBonePos( L"Bip01" ) - ( GetDirVector() * 250.f ) );
		vPos.y += 30.f;

		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrImpactDamageEffectName.c_str(), GetPowerRate(), 
														  vPos, GetRotateDegree(), GetRotateDegree() );
	}
	/// 보여주기용 링크 이펙트 설정
	else if( m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
	{
		CX2DamageEffect::CEffect* pEffect = 
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_APT_Pylon_Additional_Shock", GetPowerRate(), 
			GetBonePos( L"Bip01_L_Finger2" ), GetRotateDegree(), GetRotateDegree() );

		if ( NULL != pEffect )
			m_eQuicksilverAccelLinkEffect = pEffect->GetHandle();
	}
	/// 보여주기용 링크 이펙트 해제
	else if( m_pXSkinAnim->EventTimerOneshot( 0.85f ) )
	{
		if ( INVALID_DAMAGE_EFFECT_HANDLE != m_eQuicksilverAccelLinkEffect )
			g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_eQuicksilverAccelLinkEffect );
	}

	/// 보여주기용 링크 이펙트 연결
	if ( INVALID_DAMAGE_EFFECT_HANDLE != m_eQuicksilverAccelLinkEffect &&
		 0.25f <= m_pXSkinAnim->GetNowAnimationTime() &&
		 0.85f > m_pXSkinAnim->GetNowAnimationTime() )
	{
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_eQuicksilverAccelLinkEffect );

		if ( NULL != pEffect )
		{
			D3DXVECTOR3 vEffectPos = GetBonePos( L"Bip01" );	/// 이펙트가 있을 위치

			float fRotateRadianY = D3DXToRadian( GetRotateDegree().y ) + 1.57f;	/// 회전값
			vEffectPos.z += 150.f * cos( fRotateRadianY );
			vEffectPos.x += 150.f * sin( fRotateRadianY );
			vEffectPos.y += 45.f;

			pEffect->SetLinkMainEffectByPos( GetBonePos( L"Bip01_L_Finger2" ), vEffectPos );
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_ACCEL_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	/// 후진 가능 여부 검사
	else if( false == m_FrameDataFuture.stateParam.bEventFlagList[0] &&
			 m_fEventTime[1].keyInputStart < m_pXSkinAnimFuture->GetNowAnimationTime() && 
			  m_fEventTime[1].keyInputEnd >= m_pXSkinAnimFuture->GetNowAnimationTime() &&
			 true == SpecialAttackEventProcess() )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
	GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_ACCEL_StateEnd()
{
	if ( INVALID_DAMAGE_EFFECT_HANDLE != m_eQuicksilverAccelLinkEffect && 
		NULL != g_pX2Game && 
		NULL != g_pX2Game->GetDamageEffect() )
	{
		g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_eQuicksilverAccelLinkEffect );
	}

	CommonStateEnd();
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_FrameMove ()
{
	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
	{
		/// 강타 이펙트 생성
		if ( m_pXSkinAnim->EventTimerOneshot( 0.596f ) )
		{
			/// 막타 강타 공격
			wstring wstrImpactDamageEffectName = L"DamageEffect_ALP_Quicksilver_Accel_Impact";

			/// 변이 포인트가 있으면, 스턴 데미지 이펙트 발생
			if ( true == GetIsFormationMode() && 0 < GetMutationCount() )
			{
				wstrImpactDamageEffectName.clear();
				wstrImpactDamageEffectName = L"DamageEffect_ALP_Quicksilver_Accel_Impact_Stun";

				UpMutationCount( -1 );
			}

			D3DXVECTOR3 vPos = GetIsRight() ? ( GetBonePos( L"Bip01" ) + ( GetDirVector() * 100.f ) ): 
				( GetBonePos( L"Bip01" ) - ( GetDirVector() * 100.f ) );
			vPos.y += 30.f;

			CX2DamageEffect::CEffect* pEffect = 
				g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrImpactDamageEffectName.c_str(), GetPowerRate(), 
				vPos, GetRotateDegree(), GetRotateDegree() );
		}
		/// 보여주기용 링크 이펙트 설정
		else if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
		{
			CX2DamageEffect::CEffect* pEffect = 
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_APT_Pylon_Additional_Shock", GetPowerRate(), 
				GetBonePos( L"Bip01_L_Finger2" ), GetRotateDegree(), GetRotateDegree() );

			if ( NULL != pEffect )
				m_eQuicksilverAccelLinkEffect = pEffect->GetHandle();
		}
		/// 보여주기용 링크 이펙트 해제
		else if( m_pXSkinAnim->EventTimerOneshot( 0.57f ) )
		{
			if ( INVALID_DAMAGE_EFFECT_HANDLE != m_eQuicksilverAccelLinkEffect )
				g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_eQuicksilverAccelLinkEffect );
		}

		/// 보여주기용 링크 이펙트 연결
		if ( INVALID_DAMAGE_EFFECT_HANDLE != m_eQuicksilverAccelLinkEffect &&
			 0.0f <= m_pXSkinAnim->GetNowAnimationTime() &&
			 0.57f > m_pXSkinAnim->GetNowAnimationTime() )
		{
			CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_eQuicksilverAccelLinkEffect );

			if ( NULL != pEffect )
			{
				D3DXVECTOR3 vEffectPos = GetBonePos( L"Bip01" );	/// 이펙트가 있을 위치

				float fRotateRadianY = D3DXToRadian( GetRotateDegree().y ) + 1.57f;	/// 회전값
				vEffectPos.z += 150.f * cos( fRotateRadianY );
				vEffectPos.x += 150.f * sin( fRotateRadianY );
				vEffectPos.y += 45.f;

				pEffect->SetLinkMainEffectByPos( GetBonePos( L"Bip01_L_Finger2" ), vEffectPos );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_EventProcess ()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
	GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK_StateEnd()
{
	if ( INVALID_DAMAGE_EFFECT_HANDLE != m_eQuicksilverAccelLinkEffect && 
		 NULL != g_pX2Game && 
		 NULL != g_pX2Game->GetDamageEffect() )
	{
		g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_eQuicksilverAccelLinkEffect );
	}

	CommonStateEnd();
}

/// 퀵실버 프렌지
void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_FRENZY_Init()
{
	TextureReadyInBackground(L"GlowBall_G01.tga");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
	XMeshReadyInBackground( L"ADD_PARTICLE_ACCELERATOR_SPARK02.Y" );
	XMeshReadyInBackground( L"ADD_QUICKSILVER_FRENZY_SPARK01.Y" );
	
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_FRENZY_FrameMove()
{
	if ( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
	{
		UINT uiSkillLevel = static_cast<UINT>( GetSkillLevelValue( CX2SkillTree::SI_SA_ALP_QUICKSILVER_FRENZY ) );
		SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_QUICKSILVER_FRENZY, uiSkillLevel );

		/// 스킬 시전 당시의 데미지 배율 설정
		m_QuicksilverFrenzyData.m_fPowerRate = GetPowerRate();
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_ALP_QUICKSILVER_FRENZY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
	GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 더스트 아나토미
void CX2GUAdd::ANSI_SA_ALP_DUST_ANATOMY_Init()
{
	TextureReadyInBackground(L"ADD_CYCLE_COA01.tga");
	XSkinMeshReadyInBackground(L"ADD_Dust_Anatomy_Start_DUMMY01.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"EFFECT_Cycle_LensFlare01.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blr01.dds");
	XSkinMeshReadyInBackground(L"ADD_Dust_Anatomy_Spark01.X");
	TextureReadyInBackground(L"Black_Light.dds");
	XMeshReadyInBackground( L"ADD_Effect_Explosion_01.Y" );
	XMeshReadyInBackground( L"ADD_GroundShockWave001.Y" );
	XMeshReadyInBackground( L"ADD_STASIS_FIELD_Spark.Y" );
}

void CX2GUAdd::ANSI_SA_ALP_DUST_ANATOMY_FrameMove()
{
	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
	{
		/// 연결 이펙트 생성
		if ( m_pXSkinAnim->EventTimerOneshot( 0.23f ) )
		{
			if ( NULL !=  g_pX2Game->GetEffectSet() )
			{
				vector<DustAnatomyTargetInfo>::iterator vIt = m_vecDustAnatomyTargetInfo.begin();

				while( vIt != m_vecDustAnatomyTargetInfo.end() )
				{
					CX2GameUnit* pUnit = vIt->GetTargetUnit();

					/// 죽어 있으면, 해당 유닛 연결 정보 제거
					if ( NULL == pUnit || 0.f >= pUnit->GetNowHp() )
					{
						vIt = m_vecDustAnatomyTargetInfo.erase( vIt );

						continue;
					}

					const D3DXVECTOR3& vMyPos = GetBonePos( vIt->m_wstrCreateBoneName.c_str() );

					CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Dust_Anatomy_Link_Effect", 
						GetPowerRate(), vMyPos, GetRotateDegree(), GetRotateDegree() );

					if ( NULL == pEffect )
						continue;

					const D3DXVECTOR3&	vEnemyPos = pUnit->GetBonePos( L"Bip01" );

					pEffect->SetLinkMainEffectByPos( vMyPos, vEnemyPos );

					/// 이펙트 핸들 저장
					vIt->m_eLinkEffect = pEffect->GetHandle();

					/// 보여주기용 이펙트 생성
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_SA_ALP_Dust_Anatomy_Explosion_1nd", pUnit );

					++vIt;
				}
			}
		}
		/// 연결 이펙트 제거
		else if ( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
		{
			BOOST_FOREACH( DustAnatomyTargetInfo TargetInfo, m_vecDustAnatomyTargetInfo )
			{
				g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( TargetInfo.m_eLinkEffect );
			}

			m_vecDustAnatomyTargetInfo.resize( 0 );
		}

		/// 연결 이펙트 갱신
		if ( 0.11f < m_pXSkinAnim->GetNowAnimationTime() && 0.9f >= m_pXSkinAnim->GetNowAnimationTime() )
		{
			vector<DustAnatomyTargetInfo>::iterator vIt = m_vecDustAnatomyTargetInfo.begin();

			while( vIt != m_vecDustAnatomyTargetInfo.end() )
			{
				CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( vIt->m_eLinkEffect );

				if ( NULL != pEffect )
				{
					CX2GameUnit* pUnit = vIt->GetTargetUnit();

					/// 죽어 있으면, 해당 유닛 연결 정보 제거
					if ( NULL == pUnit || 0.f >= pUnit->GetNowHp() )
					{
						vIt = m_vecDustAnatomyTargetInfo.erase( vIt );

						continue;
					}

					const D3DXVECTOR3&	vMyPos		= GetBonePos( vIt->m_wstrCreateBoneName.c_str() );
					const D3DXVECTOR3&	vEnemyPos	= pUnit->GetBonePos( L"Bip01" );

					pEffect->SetLinkMainEffectByPos( vMyPos, vEnemyPos );
				}

				++vIt;
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_ALP_DUST_ANATOMY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

		CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_ALP_DUST_ANATOMY_StateEnd()
{
	m_vecDustAnatomyTargetInfo.resize( 0 );

	CommonStateEnd();
}

/// 사이킥 스톰
void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_Init()
{
	TextureReadyInBackground(L"ADD_CENTER_LIGHT01.dds");
	XSkinMeshReadyInBackground(L"ADD_PSYCHIC_STORM_DUMMY01.X");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"ADD_PSYCHIC_STORM_DUMMY02.X");	
	XSkinMeshReadyInBackground( L"ADD_PSYCHIC_STORM_01.X" );
	XMeshReadyInBackground(L"ADD_PARTICLE_ACCELERATOR_SPARK02.Y");
	XMeshReadyInBackground(L"ADD_ShockWave003.Y");
	XMeshReadyInBackground(L"ADD_PARTICLE_ACCELERATO_CYCLE1.Y");
	XMeshReadyInBackground(L"ADD_PSYCHIC_STORM_03.Y");
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_StateStartFuture()
{
	m_PsychicStormData.ResetPsychicStormData();

	m_PsychicStormData.m_fLoopTime = _CONST_ADD_::PSYCHIC_STORM_DEFAULT_LOOP_TIME;	/// 반복 시간 초기화

	CommonStateStartFuture();
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_StateStart()
{
	/// 초기화 함수를 Future 과 Now 두번 불러도 좋다. 
	/// 어짜피 Future에서 설정된 LoopTime은 Now 가 오기 전에 Loop가 끝나는 것을 막기 위한 처리 이기 때문이다.
	m_PsychicStormData.ResetPsychicStormData();

	float& fLoopTime		= m_PsychicStormData.m_fLoopTime;					/// 반복 시간
	fLoopTime				= _CONST_ADD_::PSYCHIC_STORM_DEFAULT_LOOP_TIME;

	float& fIncreaseSize	= m_PsychicStormData.m_fIncreaseSize;				/// 이펙트 크기

	if ( true == GetIsFormationMode() )
	{
		/// 각성시 반복 시간 추가
		fLoopTime += static_cast<float>( GetSpecialAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME_HYPER, 
			CX2SkillTree::SI_SA_ALP_PSYCHIC_STORM ) );

		/// 변이 포인트가 있다면, 반복 시간, 이펙트 크기 추가
		if ( 0 < GetMutationCount() )
		{
			/// 변이 포인트 소모량 계산
			const int iUseMutationCount = min( GetMutationCount(), _CONST_ADD_::PSYCHIC_STORM_MAX_USE_MUTATION_COUNT );

			/// 추가 반복 시간 설정
			fLoopTime += static_cast<float>( iUseMutationCount *
				GetSpecialAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, CX2SkillTree::SI_SA_ALP_PSYCHIC_STORM ) );

			/// 추가 크기 설정
			m_PsychicStormData.m_fIncreaseSize *= static_cast<float>( iUseMutationCount * 
				GetSpecialAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, CX2SkillTree::SI_SA_ALP_PSYCHIC_STORM ) );

			UpMutationCount( -iUseMutationCount );	/// 변이 포인트 소모
		}
	}

	CommonStateStart();
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ANSI_SA_ALP_PSYCHIC_STORM_LOOP );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

		CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_LOOP_StateStart()
{
	/// 보여주기용 이펙트 생성
	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() )
	{
		/// 이펙트 생성 및 핸들 저장
		CX2EffectSet::Handle hPsychicStorm		= g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_SA_ALP_Psychic_Storm_Loop", this );
		m_PsychicStormData.eLoopEffectHandle	= hPsychicStorm;

		/// 이펙트 크기 지정
		const float fIncreaseSize = m_PsychicStormData.m_fIncreaseSize;

		if ( 1.f < fIncreaseSize )
		{
			CX2EffectSet::EffectSetInstance * phPsychicStorm = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hPsychicStorm );

			if ( NULL !=  phPsychicStorm )
			{
				const float fIncreaseSize = m_PsychicStormData.m_fIncreaseSize;
				phPsychicStorm->SetEffectScale( D3DXVECTOR3( fIncreaseSize, fIncreaseSize, fIncreaseSize ), true );
			}
		}
	}

	CommonStateStart();
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_LOOP_FrameMove()
{
	float& fFireGap = m_PsychicStormData.m_fFireGap;	/// 이펙트 생성 간격

	float dfhdjkfhjksdhkjf = m_pXSkinAnim->GetNowAnimationTime();

	if( fFireGap <= m_pXSkinAnim->GetNowAnimationTime() )
	{
		fFireGap += _CONST_ADD_::PSYCHIC_STORM_ATTACK_INTERVAL_TIME;	/// 다음 생성 간격 연산

		/// 크기값을 적용하여 공격 이펙트 생성
		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		{
			D3DXVECTOR3 vPos = GetBonePos( L"Bip01" );
			vPos.y += 150.f;

			CX2DamageEffect::CEffect* pCEffect 
				= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_ALP_Psychic_Storm_Loop", GetPowerRate(), vPos, 
																GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

			/// 이펙트 크기 지정
			const float fIncreaseSize = m_PsychicStormData.m_fIncreaseSize;

			if ( 1.f < fIncreaseSize && NULL != pCEffect && NULL != pCEffect->GetMainEffect() )
			{
				const float fIncreaseSize = m_PsychicStormData.m_fIncreaseSize;
				pCEffect->SetScale( pCEffect->GetMainEffect()->GetScale() * fIncreaseSize ) ;
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_LOOP_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true ||
		m_PsychicStormData.m_fLoopTime <= m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		StateChange( ANSI_SA_ALP_PSYCHIC_STORM_FINISH );
	}

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

		CommonEventProcess();
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_FINISH_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
	{
		m_PsychicStormData.ResetPsychicStormData();
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_SA_ALP_PSYCHIC_STORM_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

		CommonEventProcess();
}

#endif //SERV_ADD_LUNATIC_PSYKER

#pragma endregion //스패셜 액티브


#pragma region 액티브

/// 입자탄
void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_Init()
{
	TextureReadyInBackground(L"ADD_Effect_CIRCLE01.tga");
	TextureReadyInBackground(L"Add_Gravity_Ball.tga");
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"ADD_Effect_Triangle01.Tga");
}
void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 입자탄 - 과충전기
void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_StateStartFuture()
{
	CommonStateStartFuture();
	/// 키 입력 제어 객체 초기화
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );
}

void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE );
	}

	CommonEventProcess();
}

void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY_StateEndFuture()
{
	CommonStateEndFuture();
	
	/// 스테이트 변경 즉시 사운드를 종료시켜 주기 위해 코드에서 처리
	PlayLoopSound(L"ADD_PULSE_BULLET_CHARGE_01.ogg", false, false );
}


void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_StateStart()
{
	CommonStateStart();
	m_fChargeTime = 0.f;
}
void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_FrameMove()
{
	CommonFrameMove();
	m_fChargeTime += m_fElapsedTime;
}
void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_CHARGE_EventProcess()
{
	/// 키 입력 제어 객체에서 슬롯이 변경 되어 있는지 검사
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();

	if ( _CONST_ADD_::PULSE_BULLET_MAX_CHARGE_TIME <= m_fChargeTime || 
		true == m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData ) )
	{
		StateChange( ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE );
	}

	CommonEventProcess();
}


void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_StateStartFuture()
{
	CommonStateStartFuture();
	StopLoopSound();
}

/** @function : ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_FrameMove
	@brief : 과충전기 패시브의 영향을 받아, 입자탄의 이펙트의 속성을 충전량 만큼 변경 시킴
			 현재는 지속시간, 크기, 데미지,  유도 설정 후 커브 스피드 값 조절 처리 함
*/
void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
	{
		D3DXVECTOR3 vPos;
		GetBonePos( L"EF01", vPos );
		vPos.x += ( true == GetIsRight() ) ? 70.f : -70.f;

		// 증가 배율 0~1값
		const float fIncreaseChargeRate = (min( _CONST_ADD_::PULSE_BULLET_MAX_APPLY_CHARGE_TIME, m_fChargeTime )) /_CONST_ADD_::PULSE_BULLET_MAX_APPLY_CHARGE_TIME ;

		// 증가 가능한 최대 배율 : 2, 충전에 의한 배율 값 : 0. 5 일때
		// -> (2-1)*0.5 +1 = 1.5
		float fPowerRate = GetPowerRate();
		{
			const float fDamageRate = (m_ChargedImpulsarData.m_fIncreaseDamageRate-1) * fIncreaseChargeRate + 1;
			if( 0 < fDamageRate )
				fPowerRate *= fDamageRate;
		}

		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_AN_Pulse_Bullet_Ball", fPowerRate,
																				vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		if ( NULL != pDamageEffect )
		{
			SetLockOn( pDamageEffect, 0, CX2DamageEffect::LOT_NEARST_UID_VECTOR );
			pDamageEffect->SetLockOnType( CX2DamageEffect::LOT_NEARST_UID_VECTOR );
			
			m_fChargeTime = 0.f;

			// 록온 관련 정보 설정
			const float fIncreaseCurveSpeedRate = (m_ChargedImpulsarData.m_fIncreaseCurveSpeedMaxRate-1) * fIncreaseChargeRate + 1;
			const float fCurveSpeed = _CONST_ADD_::PULSE_BULLET_BASE_CURVE_SPEED * fIncreaseCurveSpeedRate;
			pDamageEffect->GetLockOnData().m_fCurveSpeed			= fCurveSpeed;
			pDamageEffect->GetLockOnData().m_fTimeUnlockFromStart	= 0.2f;
			pDamageEffect->GetLockOnData().m_bIsOnlyFront			= true;

			/// 차지 시간에 따른 각 수치 설정
			CKTDGXMeshPlayer::CXMeshInstance* pMesh = pDamageEffect->GetMainEffect();
			if( NULL != pMesh )
			{
				// 증가 가능한 최대 배율 : 2, 충전에 의한 배율 값 : 0. 5 일때
				// -> (2-1)*0.5 +1 = 1.5

				// 라이프타임
				const float fIncreaseLifeRate = (m_ChargedImpulsarData.m_fIncreaseLifeTimeMaxRate-1) * fIncreaseChargeRate + 1;
				const float fLifeTime = pMesh->GetMaxLifeTime() * fIncreaseLifeRate;
				pMesh->SetMaxLifeTime( fLifeTime );

				// 스케일
				const float fScaleRate = (m_ChargedImpulsarData.m_fIncreaseScaleMaxRate-1) * fIncreaseChargeRate + 1;
				pMesh->SetScale( fScaleRate, fScaleRate, fScaleRate );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_FIRE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 정신 붕괴
void CX2GUAdd::ANSI_A_AN_MIND_BREAK_Init()
{
	XSkinMeshReadyInBackground(L"ADD_MIND_BREAK_Dynamo2.X");
	XSkinMeshReadyInBackground( L"ADD_MIND_BREAK_Dynamo.X" );
	XMeshReadyInBackground( L"ADD_ShockWave003.Y" );
	TextureReadyInBackground( L"ADD_Effect_Triangle01.Tga" );
	XSkinMeshReadyInBackground( L"ADD_NEUTRON_BOMB_B.X" );
}

void CX2GUAdd::ANSI_A_AN_MIND_BREAK_FrmaeMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.271f ) )
	{
		/// 타겟팅 대상 찾기 ( 아이샤 에너지 드레인 복붙 )
		float			fDistance				= 700.f;
		CX2GameUnit*	pFrontNearestUnit		= NULL;

//		// 나중에 사정거리 SA 값으로 받아와야 할 필요가 있으면 주석 해제해주세요!
// 		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ABM_ENERGY_DRAIN );
// 		if( NULL != pSkillTemplet )
// 		{
// 			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
// 			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
// 			fDistance = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillTempletLevel );
// 		}
		for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
		{
			CX2GameUnit *pUnit = g_pX2Game->GetUnit( i );

			if ( NULL == pUnit )
				continue;
			if( g_pX2Game->IsValidUnit( pUnit ) == false )
				continue;
			if( GetTeam() == pUnit->GetTeam() )
				continue;
			if( pUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE || pUnit->GetNowHp() <= 0.f )
				continue;					

			if( g_pX2Game->IsFront( (CX2GameUnit*)this, pUnit ) == false )
				continue;

			float fOffset = 0.f;
			if(pUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC)
			{
				CX2GUNPC *pNpc = static_cast<CX2GUNPC*>( pUnit );
				if( NULL == pNpc || CX2UnitManager::NCT_BASIC != pNpc->GetNPCTemplet().m_ClassType )
					continue;

				fOffset = ( pNpc->GetOrgUnitSize().x + pNpc->GetOrgUnitSize().y ) / 4.f;
			}					

			D3DXVECTOR3 vDistance = pUnit->GetPos() - GetPos();

			float fDistanceUnit = D3DXVec3Length( &vDistance ) - fOffset;

			if( fDistance >= fDistanceUnit )
			{
				fDistance = fDistanceUnit;
				pFrontNearestUnit = pUnit;
			}
		}

		if ( NULL != pFrontNearestUnit )
		{
			/// 스킬 레벨 반환
			const CX2UserSkillTree& userSkillTree	= GetUnit()->GetUnitData().m_UserSkillTree;
			const int				iSkillLevel		= userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AN_MIND_BREAK );

			/// 대상의 머리에 강제로 디버프 박아 버림
			pFrontNearestUnit->SetBuffFactorToGameUnitByBuffFactorID( BFI_DEBUFF_MIND_BREAK, iSkillLevel );

			/// 대상의 머리에 강제로 데미지 이펙트 박아 버림
			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() && g_pX2Game->GetEffectSet() )
			{
				D3DXVECTOR3 vHeadPos = pFrontNearestUnit->GetHeadBonePos();

				/// 타격용 데미지 이펙트
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_SI_A_AN_MIND_BREAK", GetPowerRate(), 
					vHeadPos, GetRotateDegree(),GetRotateDegree() );

				/// 디스플레이용 이펙트셋
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_A_AN_MIND_BREAK", pFrontNearestUnit );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_A_AN_MIND_BREAK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 위상 변화
void CX2GUAdd::ANSI_A_AN_PHASE_SHIFT_Init()
{
	XMeshReadyInBackground(L"ADD_Effect_CIRCLE_03_1.Y");
	XMeshReadyInBackground(L"ADD_PHASE_SHIFT_BLINK06.Y");
	XMeshReadyInBackground(L"ADD_MAGNETRON_DYNAMO_Spark.Y");
	XMeshReadyInBackground(L"ADD_MAGNETRON_DYNAMO_Spark2.Y");
	XSkinMeshReadyInBackground(L"ADD_PHASE_SHIFT_SPARK01.X");
	XSkinMeshReadyInBackground(L"ADD_PHASE_SHIFT_LOOP01.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"add_EFFECT_Cycle_Blur01.dds");
	TextureReadyInBackground(L"ADD_ShockWave.dds");
	TextureReadyInBackground(L"Black_Light.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
}
void CX2GUAdd::ANSI_A_AN_PHASE_SHIFT_FrameMove()
{
	CommonFrameMove();

	if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
	{
		/// 유지되는 데미지 이펙트 생성
		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		{
			// 기존 이펙트가 있다면 강제로 터뜨리기
			if( INVALID_DAMAGE_EFFECT_HANDLE != m_PhaseShiftData.m_hPhasShiftDamageEffect )
			{
				CX2DamageEffect::CEffect* pEffect = 
					g_pX2Game->GetDamageEffect()->GetInstance( m_PhaseShiftData.m_hPhasShiftDamageEffect );
				if( NULL != pEffect )
				{
					CKTDGXMeshPlayer::CXMeshInstance* pMesh = pEffect->GetMainEffect();
					if( NULL != pMesh )
						pMesh->SetMaxLifeTime(0.1f);

					pEffect->GetDamageData().m_eUsedSkillID = CX2SkillTree::SI_NONE;
					m_PhaseShiftData.InitPhaseEffectData();
				}
			}

			// 새로 생성
			{
				CX2DamageEffect::CEffect* pEffect = 
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Add_Phase_Shift", 
					GetPowerRate(), GetPos(), GetRotateDegree(),GetRotateDegree() );

				if( NULL != pEffect )
				{
					// 돌아갈 위치 저장
					m_PhaseShiftData.m_vUserReturnPos = GetPos();

					// 데미지 이펙트 핸들 저장
					m_PhaseShiftData.m_hPhasShiftDamageEffect = pEffect->GetHandle();

					// 슬롯 UI 변경
					if ( true == IsMyUnit() ) 
						m_PhaseShiftData.UpdateSkillSlotUI();

					// 이펙트 지속시간 설정
					CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pEffect->GetMainEffect();
					pMeshInstance->SetMaxLifeTime( m_PhaseShiftData.m_fEffectLifeTime );
				}
			}
		}
	}
}

void CX2GUAdd::ANSI_A_AN_PHASE_SHIFT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}
/** @function : ANSI_A_AN_PHASE_SHIFT_BLINK
	@brief : 설치해둔 위상변화 이펙트와 유저와의 위치 값 변경
*/
void CX2GUAdd::ANSI_A_AN_PHASE_SHIFT_BLINK_StateStart()
{
	CommonStateStart();

	if( NULL != g_pX2Game && 
		NULL != g_pX2Game->GetDamageEffect() )
	{
		CX2DamageEffect::CEffect* pEffect =
			g_pX2Game->GetDamageEffect()->GetInstance( m_PhaseShiftData.m_hPhasShiftDamageEffect );

		if( NULL != pEffect )
		{
			pEffect->SetPos( GetPos() );
		}
	}

	// # 순서 보장을 위해 Futrue에서 하지 않고 IsMyUnit으로 검사 함
	//   ( 순서 보장 : 이펙트가 먼저 옮겨간 다음, 유저 위치를 옮겨야 함)
	if( true == IsMyUnit() &&
	    true == m_PhaseShiftData.IsValidReturnPosValue() )
	{
// 		// 방향 변경
//		(기획적으로 방향 변경하지 않도록 제거했지만 코드 유지 해 둠..)
// 		const D3DXVECTOR3 vDistancePos = m_PhaseShiftData.m_vUserReturnPos -m_FrameDataFuture.syncData.position;
// 		if( (GetDirVector().x * vDistancePos.x) < 0.f ||
// 			(GetDirVector().z * vDistancePos.z) < 0.f )
// 		{
// 			m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
// 		}
		
		// 위치 변경
		m_FrameDataFuture.syncData.position = m_PhaseShiftData.m_vUserReturnPos;
		m_FrameDataNow.syncData.position = m_PhaseShiftData.m_vUserReturnPos;
		m_PhaseShiftData.InitResturnPosValue();

		if ( IsMyUnit() )
			m_PhaseShiftData.UpdateSkillSlotUI();
	}
}
void CX2GUAdd::ANSI_A_AN_PHASE_SHIFT_BLINK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 다이너모 구성 - 마그네트론
void CX2GUAdd::ANSI_A_APT_MAGNETRON_DYNAMO_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_100x50x50_center.X" );
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_Spark.Y" );
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_Spark2.Y" );
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_CYCLE1.Y" );
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_CYCLE2.Y" );
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_MESH1.Y" );
}

void CX2GUAdd::ANSI_A_APT_MAGNETRON_DYNAMO_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
	{
		/// 마그네트론 정보 초기화
		m_MagnetronData.ResetMagnetronData();

		for( int i = 0; i < _CONST_ADD_::MAGNETRON_MAX_NUM; ++i )
		{
			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
			{
				D3DXVECTOR3 vPos = GetBonePos( L"Bip01" );
				D3DXVECTOR3 vRot = GetRotateDegree();

				vRot.y	+= static_cast<float>( ( 360.f / _CONST_ADD_::MAGNETRON_MAX_NUM ) * i );

				CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"DamageEffect_SA_APT_Magnetron_Dynamo_Start", 
																				  GetPowerRate(), vPos, vRot, vRot, m_FrameDataNow.unitCondition.landPosition.y );

				if ( NULL != pEffect )
					m_MagnetronData.m_vecMagnetronDamageEffectData.push_back( pEffect->GetHandle() );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_A_APT_MAGNETRON_DYNAMO_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 다이너모 구성 - 디졸버
void CX2GUAdd::ANSI_A_APT_DESOLVER_DYNAMO_Init()
{
	XSkinMeshReadyInBackground(L"ADD_DESOLVER_DYNAMO_Start_DUMMY01.X");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XMeshReadyInBackground( L"ADD_GroundShockWave001.Y" );
	XMeshReadyInBackground( L"ADD_Effect_Explosion_02.Y" );
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_CYCLE2.Y" );
	XMeshReadyInBackground( L"ADD_ShockWave003.Y" );
	TextureReadyInBackground( L"Black_Light.dds" );
}

void CX2GUAdd::ANSI_A_APT_DESOLVER_DYNAMO_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

/// 다이너모 구성 - 파일런
void CX2GUAdd::ANSI_A_APT_PYLON_DYNAMO_Init()
{
	XSkinMeshReadyInBackground(L"DummyAttackBox_100x50x50_center.X");
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_Spark.Y" );
	XMeshReadyInBackground( L"ADD_MAGNETRON_DYNAMO_Spark2.Y" );
	TextureReadyInBackground( L"ADD_CYCLE_COA01.Tga" );
	TextureReadyInBackground( L"Black_Light.dds" );
	TextureReadyInBackground( L"add_EFFECT_Cycle_Blur01.dds" );
}

void CX2GUAdd::ANSI_A_APT_PYLON_DYNAMO_FrameMove()
{
	if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
	{
		D3DXVECTOR3 vSummonPos = GetPos();
			
		vSummonPos.y += 110.f;

		/// 파일런 데미지 이펙트
		CX2DamageEffect::CEffect* pEffect		= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_APT_Pylon_Dynamo", GetPowerRate(), 
			vSummonPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		/// 버프용 데미지 이펙트
		CX2DamageEffect::CEffect* pBuffEffect	= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Pylon_Buff", GetPowerRate(), 
			vSummonPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		if ( NULL != pEffect && NULL != pBuffEffect )
		{
			AddPylonLinkDamageEffect( pEffect );

			/// 소환한 파일런이 하나도 없으면, 연속 소환 쿨타임 설정
			if ( 0 == m_uiNonstopSummonPylonCount )
				m_fNonstopSummonPylonCoolTime = _CONST_ADD_::PYLON_NONESTOP_SUMMON_COOL_TIME;
			
			/// 연속 소환 검사를 위한 파일런 개체수 증가
			++m_uiNonstopSummonPylonCount;

			/// 소환된 파일런 UID 저장
			m_vecSummonPylonDamageEffectData.push_back( PylonDamageEffectData( pEffect->GetHandle(), pBuffEffect->GetHandle(), 
				_CONST_ADD_::PYLON_REMAIN_TIME ) );

			/// 소환한 파일런이 연속 소환 제한수를 넘었다면, 연속 소환 쿨타임 및 카운트 제거
			if ( _CONST_ADD_::PYLON_NONESTOP_SUMMON_LIMIT_NUM <=  m_uiNonstopSummonPylonCount )
			{
				m_fNonstopSummonPylonCoolTime	= 0.f;
				m_uiNonstopSummonPylonCount		=	0;

			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_A_APT_PYLON_DYNAMO_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
									GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

	CommonEventProcess();
}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

/// 사이오닉 비트
void CX2GUAdd::ANSI_A_ALP_PSIONIC_BEAT_Init()
{
	TextureReadyInBackground(L"ADD_CYCLE_COA01.tga");
	TextureReadyInBackground(L"ADD_Effect_Triangle01.Tga");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
}

void CX2GUAdd::ANSI_A_ALP_PSIONIC_BEAT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

		CommonEventProcess();
}

/// 트월 러시
void CX2GUAdd::ANSI_A_ALP_TWIRL_RUSH_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"GlowBall_G01.tga");
	XMeshReadyInBackground( L"ADD_PARTICLE_ACCELERATOR_SPARK02.Y" );
	XMeshReadyInBackground( L"ADD_ShockWave003.Y" );
	XMeshReadyInBackground( L"ADD_PARTICLE_ACCELERATO_CYCLE1.Y" );
	XMeshReadyInBackground( L"ADD_PSYCHIC_STORM_03.Y" );
	XSkinMeshReadyInBackground(L"ADD_PSYCHIC_STORM_01.X");
}

void CX2GUAdd::ANSI_A_ALP_TWIRL_RUSH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

		CommonEventProcess();
}

#endif //SERV_ADD_LUNATIC_PSYKER

#pragma endregion //액티브

#pragma region 하이퍼 스페셜 액티브

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

/// 둠스 데이
void CX2GUAdd::ANSI_HA_ALP_DOOMS_DAY_Init()
{

}

void CX2GUAdd::ANSI_HA_ALP_DOOMS_DAY_StateStart()
{
	m_vecDoomsDayLinkEffect.resize( 0 );	/// 링크 이펙트 초기화

	CommonStateStart();
}

void CX2GUAdd::ANSI_HA_ALP_DOOMS_DAY_FrameMove()
{
	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
	{
		/// 이펙트 생성
		if( m_pXSkinAnim->EventTimerOneshot( 0.33f ) )
		{
			/// 더미 본 이펙트
			CX2DamageEffect::CEffect* pCEffectDummy	= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_ALP_Dooms_Day_Dummy_effect", GetPowerRate(), 
				GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			/// 링크 이펙트 1
			CX2DamageEffect::CEffect* pCEffectLink1 = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_ALP_Dooms_Day_Link_effect", GetPowerRate(), 
				GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			/// 링크 이펙트 2
			CX2DamageEffect::CEffect* pCEffectLink2 = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_ALP_Dooms_Day_Link_effect", GetPowerRate(), 
				GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

			if ( NULL != pCEffectDummy && NULL != pCEffectLink1 && NULL != pCEffectLink2 )
			{
				m_vecDoomsDayLinkEffect.push_back( pCEffectDummy->GetHandle() );
				m_vecDoomsDayLinkEffect.push_back( pCEffectLink1->GetHandle() );
				m_vecDoomsDayLinkEffect.push_back( pCEffectLink2->GetHandle() );
			}
		}
		/// 이펙트 제거
		else if( m_pXSkinAnim->EventTimerOneshot( 4.294f ) )
		{
			BOOST_FOREACH( CX2DamageEffect::CEffectHandle hHandle , m_vecDoomsDayLinkEffect )
			{
				g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( hHandle );
			}

			m_vecDoomsDayLinkEffect.resize( 0 );
		}

		/// 이펙트 링크 위치 갱신
		if ( false == m_vecDoomsDayLinkEffect.empty() )
		{
			vector<CX2DamageEffect::CEffectHandle>::iterator vIt = m_vecDoomsDayLinkEffect.begin();

			CX2DamageEffect::CEffect* pCEffectDummy = g_pX2Game->GetDamageEffect()->GetInstance( *vIt );
			CX2DamageEffect::CEffect* pCEffectLink1 = g_pX2Game->GetDamageEffect()->GetInstance( *( vIt + 1 ) );
			CX2DamageEffect::CEffect* pCEffectLink2 = g_pX2Game->GetDamageEffect()->GetInstance( *( vIt + 2 ) );

			if ( NULL != pCEffectDummy && NULL != pCEffectDummy->GetMainEffect() && 
				 NULL != pCEffectLink1 && NULL != pCEffectLink2 )
			{
				CKTDGXMeshPlayer::CXMeshInstance* pDummyMesh = pCEffectDummy->GetMainEffect();

				if ( NULL != pDummyMesh )
				{
					const D3DXVECTOR3 vLinkPos1_1 = pDummyMesh->GetBonePos( L"Dummy02" );
					const D3DXVECTOR3 vLinkPos1_2 = pDummyMesh->GetBonePos( L"Dummy04" );
					pCEffectLink1->SetLinkMainEffectByPos( vLinkPos1_1, vLinkPos1_2 );

					const D3DXVECTOR3 vLinkPos2_1 = pDummyMesh->GetBonePos( L"Dummy03" );
					const D3DXVECTOR3 vLinkPos2_2 = pDummyMesh->GetBonePos( L"Dummy05" );
					pCEffectLink2->SetLinkMainEffectByPos( vLinkPos2_1, vLinkPos2_2 );
				}
			}
		}
	}

	CommonFrameMove();
}

void CX2GUAdd::ANSI_HA_ALP_DOOMS_DAY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER_ONLY_ADD( m_fSkillCancelAfter )
#else // SKILL_CANCEL_BY_HYPER_MODE
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif //SKILL_CANCEL_BY_HYPER_MODE
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		ELSE_IF_CAN_NASOD_ARMOR_ATTACK( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, 
		GetIsFormationMode(), m_NasodArmorAttackTargetData.m_uidTargetUID )

		CommonEventProcess();
}

void CX2GUAdd::ANSI_HA_ALP_DOOMS_DAY_StateEnd()
{
	/// 만약 이펙트 남아있다면, 지워주자
	if ( false == m_vecDoomsDayLinkEffect.empty() )
	{
		BOOST_FOREACH( CX2DamageEffect::CEffectHandle hHandle , m_vecDoomsDayLinkEffect )
		{
			g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( hHandle );
		}
	}

	CommonStateEnd();
}

#endif //SERV_ADD_LUNATIC_PSYKER

#pragma endregion //하이퍼 스페셜 액티브

#ifdef MODIFY_SET_DAMAGE_DATA
/*virtual*/void CX2GUAdd::SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_ ,OUT float& fPowerRate)
{
	CX2GUUser::SetDamageExceptionProcess(pDamageData_, fPowerRate);

	/// 초월자의 신체 적용 설정
	ProcessBodyOfTranscendence( pDamageData_ );
}
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef MODIFY_RIDING_PET_AWAKE
void CX2GUAdd::RidingHyperModeFrameMove()
{
	CommonHyperModeFrameMove( 0.f, 0.1f, true );
}
void CX2GUAdd::CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ /*= false*/ )
{
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( fTime1_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTime1_ ) == true && EventCheck(fTime1_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange( L"Bip01_Spine1" );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( fTime2_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTime2_ ) == true && EventCheck(fTime2_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

		if ( NULL !=g_pX2Game->GetMinorParticle()  )
		{
			D3DXVECTOR3 pos = GetBonePos( L"Bip01_Spine1" );

			CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = 
				g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
			PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z,  5, 10,  3 );

			CKTDGParticleSystem::CParticleEventSequence* pSeqWhiteShockWave = 
				g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_WHITE_SHOCK_WAVE ) );
			PlaySequenceByTriggerCount( pSeqWhiteShockWave, pos, 10, 10,  1 );

			CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTick = 
				g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_IMPACT_TICK ) );
			PlaySequenceByTriggerCount( pSeqImpactTick, pos,  200, 200,  30 );
		}

		/// 효과음 이펙트 출력
		SetEffectSoundParticle( GetBonePos( L"Bip01" ), L"ADD_STR_ToKang" );
#else // SERV_ADD_LUNATIC_PSYKER
		ShowMinorParticleHyperModeShock( L"Bip01_Spine1" );
#endif // SERV_ADD_LUNATIC_PSYKER

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();

		if( true == bSound_ )
		{
			PlaySound( L"Energy.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}
	CommonFrameMove();
}
#endif //MODIFY_RIDING_PET_AWAKE


/** @function	: GetPossibleChangeFormationMode
	@brief		: 구성 모드 사용 가능 여부 반환
	@param		: 안내 메시지 표시 여부
	@return		: 사용 가능 여부
*/
const bool CX2GUAdd::GetPossibleChangeFormationMode( IN const bool bOutputInformation /*= true*/ )
{ 
	if ( NULL == g_pX2Game  )
		return false;

	/// 구성 모드 중이라면, 패스
	if ( true == GetIsFormationMode() )
		return false;

	/// DP 검사
	if ( m_fUseDPByEnableFormationMode > GetDPValue() )
	{
		/// DP 부족하면, 안내 텍스트 표시
		if ( true == bOutputInformation  )
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_29420 ), D3DXCOLOR(1,1,1,1),
			D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );

		return false;
	}

	/// 쿨타임 검사
	if ( GetChangeFormationCoolTime() > 0.f )
	{
		/// 쿨타임 부족하면, 안내 텍스트 표시
		if ( true == bOutputInformation  )
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_29939 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );

		return false;
	}

	return true;
}

/** @function	: GetChangeFormationCoolTime
	@brief		: 구성 모드 전환 쿨타임 반환
*/
const float CX2GUAdd::GetChangeFormationCoolTime()
{
	return ( NULL != GetAddGageData() ) ? GetAddGageData()->GetChangeFormationCoolTime() : 0.f;
}

/** @function	: SetChangeFormationCoolTime
	@brief		: 구성 모드 전환 쿨타임 설정
	@param		: 설정할 쿨타임
*/
void CX2GUAdd::SetChangeFormationCoolTime( IN const float fCoolTime_ )
{
	if ( NULL != GetAddGageData() )
	{
		GetAddGageData()->SetChangeFormationCoolTime( fCoolTime_ );
	}
}

/** @function	: GetAddGageData
	@brief		: 애드 전용 게이지 데이터 반환
	@return		: 게이지 데이터
*/
CX2AddGageData* CX2GUAdd::GetAddGageData() const
{ 
	return static_cast<CX2AddGageData*>( m_pGageData ); 
}

/** @function	: SpecialAttackCustomStateChange
	@brief		: 특정 스킬에 대한 스테이트 변경 처리
	@param		: 스킬 아이디
	@return		: 시전 가능 여부
*/
const bool CX2GUAdd::SpecialAttackCustomStateChange( IN const CX2SkillTree::SKILL_ID eSkillID_ )
{
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	switch ( eSkillID_ )
	{
	case CX2SkillTree::SI_A_AN_PHASE_SHIFT:		/// 위상 변화 위치 복귀 스테이트
		{
			if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PHASE_SHIFT ) && true == GetIsFormationMode() )
			{
				SetUseFantasyTracerTarget( CX2SkillTree::SI_A_AN_PHASE_SHIFT );	/// 사용 처리

				StateChange( ANSI_COMBO_NASOD_ARMOR_READY );

				return true;
			}
			else if ( true == m_PhaseShiftData.IsValidReturnPosValue() )
			{
				StateChange( ANSI_A_AN_PHASE_SHIFT_BLINK );
				return true;
			}
		} break;
	case CX2SkillTree::SI_A_AN_PULSE_BULLET:	/// 입자탄 - 과충전기용 스테이트
		{
			if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PULSE_BULLET ) && true == GetIsFormationMode() )
			{
				SetUseFantasyTracerTarget( CX2SkillTree::SI_A_AN_PULSE_BULLET );	/// 사용 처리

				StateChange( ANSI_COMBO_NASOD_ARMOR_READY );

				return true;
			}
			// 입자탄 - 과충전기 패시브 적용 시 과충전 스테이트로 변경
			else if ( true == m_ChargedImpulsarData.m_bEnableChargedImpulsar )
			{
				StateChange( ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY );
				return true;
			}

		} break;
	case CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL:
		{
			/// 퀵실버 액셀 후진 전환
			if ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == GetFutureStateSkillID() )
			{
				StateChange( ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK );
				return true;
			}
		} break;
	}
#else // SERV_ADD_LUNATIC_PSYKER
	switch ( eSkillID_ )
	{
	case CX2SkillTree::SI_A_AN_PHASE_SHIFT:		/// 위상 변화 위치 복귀 스테이트
		{
			if ( true == m_PhaseShiftData.IsValidReturnPosValue() )
			{
				StateChange( ANSI_A_AN_PHASE_SHIFT_BLINK );
				return true;
			}
		} break;
	case CX2SkillTree::SI_A_AN_PULSE_BULLET:	/// 입자탄 - 과충전기용 스테이트
		{
			// 입자탄 - 과충전기 패시브 적용 시 과충전 스테이트로 변경
			if ( true == m_ChargedImpulsarData.m_bEnableChargedImpulsar )
			{
				StateChange( ANSI_A_AN_PULSE_BULLET_CHARGED_IMPULSAR_READY );
				return true;
			}

		} break;
	}
#endif // SERV_ADD_LUNATIC_PSYKER
	
	return false;
}

/** @function	: GetCustomSkillCoolTime
	@brief		: 특정 스킬에 대한 스테이트 변경 처리
	@param		: 기존 쿨타임, 스킬 아이디
	@return		: 최종 스킬 쿨타임
*/
const float	CX2GUAdd::GetCustomSkillCoolTime( IN const float fCoolTime_, IN const CX2SkillTree::SKILL_ID eSkillID_ )
{
	float fCoolTime = fCoolTime_;

	switch ( eSkillID_ )
	{
	case CX2SkillTree::SI_SA_APT_STASIS_FIELD:		/// 스테이시스 필드
		{
			if ( true == m_StasisfieldData.GetIsActiveStasisfield() )
			{
				fCoolTime = -1.f;
			}
		} break;
	case CX2SkillTree::SI_A_AN_PHASE_SHIFT:			/// 위상 변화
		{
			if ( INVALID_DAMAGE_EFFECT_HANDLE != m_PhaseShiftData.m_hPhasShiftDamageEffect )
			{
				fCoolTime = -1.f;
			}
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			/// 위상 변화 복귀 혹은 환상 추적자 발동시엔 쿨타임 설정 제외
			else if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PHASE_SHIFT ) )
			{
				fCoolTime = -1.f;
			}
#endif // SERV_ADD_LUNATIC_PSYKER
	
		} break;
	case CX2SkillTree::SI_A_APT_PYLON_DYNAMO:		/// 다이너모 구성 - 파일런
		{
			/// 연속 소환 가능한 조건이라면, 쿨타임 설정 않함
			if ( true == GetPossibleNonstopSummonPylon() )
			{
				fCoolTime = -1.f;
			}
		} break;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2SkillTree::SI_A_AN_PULSE_BULLET:		/// 입자탄
		{
			/// 환상 추적자 발동시엔 쿨타임 설정 제외
			if ( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PULSE_BULLET ) )
				fCoolTime = -1.f;
		} break;

	case CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL:	/// 퀵실버 액셀
		{
			/// 퀵실버 액셀 후진시에는 쿨타임 적용 하지 않는다.
			if ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == GetFutureStateSkillID() )
				fCoolTime = -1.f;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return fCoolTime;
}

/** @function	: SetCustomAnimName
	@brief		: 특정 상태일 때, 애니메이션을 바꾸는 함수
	@param		: 애니메이션 이름
*/
/*virtual*/ void CX2GUAdd::SetCustomAnimName( IN const UserUnitStateData& UserStateData, IN OUT wstring& wstrAnimName )

{
	switch( UserStateData.stateID )
	{
	case USI_DASH:
	case ANSI_AIR_DASH:
		{
			/// 각성중 대시중일 때
			if ( true == GetIsFormationMode() )
				wstrAnimName = L"Formation_Dash";
		} break;
	case USI_DASH_END:
		{
			/// 각성중 대시 종료할 때
			if ( true == GetIsFormationMode() )
				wstrAnimName = L"Formation_DashEnd";
		} break;
	}
}

#ifdef ADD_MEMO_1ST_CLASS //김창한
const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION CX2GUAdd::GetSkillUseCondition( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = pSkillTemplet_->m_eActiveSkillUseCondtion;

	switch (pSkillTemplet_->m_eID)
	{
	case CX2SkillTree::SI_SA_AHM_CHAIN_LIGHTNING:
		{
			if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO17) == true )
				eActiveSkillUseCondition = CX2SkillTree::ASUT_AIR;
		} break;
	case CX2SkillTree::SI_SA_APT_STASIS_FIELD:
		{
			// 스테이시스 필드 예외처리
			if( true == m_StasisfieldData.GetIsActiveStasisfield() &&
				CX2SkillTree::SI_SA_APT_STASIS_FIELD != m_FutureStateData.m_eSkillID )
				eActiveSkillUseCondition = CX2SkillTree::ASUT_ANYTIME_EXCEPT_AIR;
		} break;
	}

	return eActiveSkillUseCondition;
}
#endif //ADD_MEMO_1ST_CLASS

/** @function	: ConsumeMPorDP
	@brief		: MP 혹은 DP 소비 검사 함수
	@param		: MP 소비량, DP 소비량
	@return		: 부족함 없이 정상 소비 되었는지 여부
*/
const bool CX2GUAdd::ConsumeMPorDP( IN const float fConsumeMP, IN const float fConsumeDP )
{
	if ( true == GetIsFormationMode() )
	{
		if ( GetDPValue() <= fConsumeDP )
			return false;
		else
			UpNowDPValue( -fConsumeDP );
	}
	else
	{
		return FlushMp( fConsumeMP );
	}

	return true;
}

/** @function	: GetDPRateByType
	@brief		: DP 배율 타입에 따른 배율 값
	@param		: DP 배율 타입
	@return		: 배율 값
*/
const float CX2GUAdd::GetDPRateByType( IN const CX2DamageManager::DYNAMO_PARTICLE_RATE_TYPE eDPRateType )
{
	switch ( eDPRateType )
	{
	case CX2DamageManager::DPRT_SPECIAL_ACTIVE:	/// 스페셜 액티브
		{
			return m_fDPRateBySpecialActive;
		} break;
	case CX2DamageManager::DPRT_ACTIVE:			/// 액티브
		{
			return m_fDPRateByActive;
		} break;
	case CX2DamageManager::DPRT_COMMAND:		/// 커맨드
		{
			return m_fDPRateByCommand;
		} break;
	case CX2DamageManager::DPRT_NASOD_ARMOR:	/// 나소드 아머
		{
			/// 각성중이 아니면, 배율을 0으로 주자
			return ( true == GetIsFormationMode() ) ? m_fDPRateByNasodArmor : 0.f;
		} break;
	default:
		{
			/// 타입이 들어있지 않거나, 이상한 값이 들어 있어요!
			//ASSERT( ! "Unknown DYNAMO_PARTICLE_RATE_TYPE!!!" );
			return 0.f;
		} break;
	}
}

/** @function	: GetMutationCount
	@brief		: 변이 수치 반환 함수
	@return		: 변이 수치
*/
const int CX2GUAdd::GetMutationCount()
{
	return ( NULL != GetAddGageData() ) ? GetAddGageData()->GetMutationCount() : 0;
}

/** @function	: SetChangedMutationCount
	@brief		: 변이 수치 갱신 여부 설정
	@param		: 변이 수치 갱신 여부
*/
void CX2GUAdd::SetChangedMutationCount( IN const bool bChangedMutationCount_ ) 
{ 
	if ( NULL != GetAddGageData() )
	{
		GetAddGageData()->SetChangedMutationCount( bChangedMutationCount_ );
	}
}

/** @function	: SetMutationCount
	@brief		: 변이 수치 대입 함수
	@param		: 변이 수치
*/
/*virtual*/ void CX2GUAdd::SetMutationCount( IN const int iMutationCount_ ) 
{ 
	if ( NULL != GetAddGageData() )
	{
		GetAddGageData()->SetChangedMutationCount( true );
		GetAddGageData()->SetMutationCount( iMutationCount_ ); 
	}
}

/** @function	: UpMutationCount
	@brief		: 변이 수치 연산 함수
	@param		: 변이 수치
*/
void CX2GUAdd::UpMutationCount( IN const int iMutationCount_  )
{
	if ( NULL != GetAddGageData() )
	{
		int iNowMutationCount = GetAddGageData()->GetMutationCount() + iMutationCount_ ;

		if ( MAX_MUTATION_COUNT_VALUE < iNowMutationCount )
			iNowMutationCount = MAX_MUTATION_COUNT_VALUE;
		else if ( 0 >= iNowMutationCount )
			iNowMutationCount = 0;

		GetAddGageData()->SetChangedMutationCount( true );
		GetAddGageData()->SetMutationCount( iNowMutationCount ); 
	}
}

/** @function	: GetMaxDPValue
	@brief		: 최대 DP 수치 반환 함수
	@return		: 최대 DP 수치
*/
const float	CX2GUAdd::GetMaxDPValue()
{ 
	return ( NULL != GetAddGageData() ? GetAddGageData()->GetMaxDPValue() : 0.f ); 
}

/** @function	: GetDPValue
	@brief		: DP 수치 반환 함수
	@return		: DP 수치
*/
const float CX2GUAdd::GetDPValue() 
{ 
	return ( NULL != GetAddGageData() ? GetAddGageData()->GetDPValue() : 0.f ); 
}

/** @function	: SetDPValue
	@brief		: DP 수치 대입 함수
	@param		: DP 수치
*/
/*virtual*/ void CX2GUAdd::SetDPValue( IN const float fDPValue_ ) 
{ 
	if ( NULL != GetAddGageData() )
		GetAddGageData()->SetDPValue( fDPValue_ ); 
}

/** @function	: UpNowDPValue
	@brief		: DP 수치 연산 함수
				  소켓, 스킬 효과에 대한 증감 적용
				  
				  증감 효과에 의해 값이 뻥튀기 되는 문제를 수정하기 위해, 인자로 rate값 받음

				  기존 : fNowDPValue * 각충속 * 길드스킬 * 패시브스킬
				 		  1000      *  1.5   *    1.5   * 1.5 -> 3.37배 증가
				  변경 : fNowDPValue * ( 1 + (각충속 - 1) + (길드스킬-1) + (패시브스킬-1) )
				 		  1000      * ( 1 + 0.5  + 0.5 + 0.5 ) -> 2.5배 증가

	@param		: fDPValue_(DP 수치), fChangeValueRate_ ( 값 변경할 배율 값 )
*/
/*virtual*/ void CX2GUAdd::UpNowDPValue( IN const float fDPValue_, IN const float fChangeValueRate_ /*= 1.f */ )
{
	if( 0 == fDPValue_ )
		return;

	float		fNowDPValue = fDPValue_;			/// 설정 시킬 DP 수치
	const float	fMaxNPValue = GetMaxDPValue();		/// 최대 DP 수치

	// DP획득
	// 패시브 스킬(다이너모 연구), 소켓 효과(각성충전속도), 길드 스킬 영향 적용
	if ( 0.f < fNowDPValue )		
	{
		float fChangeRate = 0.f;
		{
			if ( NULL != g_pData &&
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit()  )
			{
				// 소켓 및 패시브 스킬로 인한 DP 획득량 증가값
				const float fChangeRate_BySocketAndPassive = GetEffectiveIncreaseDPValue();

				// 인자로 받은 기타 효과 ( ex 20% 증가 : 1.2 -> 0.2f )
				const float fChangeRate_ByArg = fChangeValueRate_ - 1.f;

				// ex ( 1 + 0.5f + 0.15f + 0.2f = 1.85f )
				fChangeRate = 1.f + ( fChangeRate_BySocketAndPassive + fChangeRate_ByArg );
			}
		}
		fNowDPValue *= fChangeRate;
	}
	// DP 소모
	// 패시브 스킬(다이너모 연구), 소켓 효과(각성지속시간) 영향 적용
	else if( 0.f > fNowDPValue )	
	{
		float fChangeRate = 0.f;
		{
			// 소켓 및 패시브 스킬로 인한 DP 소모량 감소값
			const float fChangeRate_BySocketAndPassive = GetEffectiveDecreaseDPConsume();

			// 인자로 받은 기타 효과 ( ex -10%감소 : 0.9 -> 0.1)
			const float fChangeRate_ByArg = 1.f - fChangeValueRate_;
			
			// ( ex  1.f - 0.06f - 0.02f - 0.1f = 0.82f )
			fChangeRate = 1.f - fChangeRate_BySocketAndPassive - fChangeRate_ByArg;
		}
		fNowDPValue *= fChangeRate;
	}

	/// 현재 DP 수치와 연산
	fNowDPValue += GetDPValue();

	/// DP 최대 및 최소치 재한
	if ( fMaxNPValue < fNowDPValue )
		fNowDPValue = fMaxNPValue;
	else if ( 0.f >= fNowDPValue )
		fNowDPValue = 0.f;

	SetDPValue( fNowDPValue );
}

/** @function	: UpNowDPValueByDamage
	@brief		: 피격시 DP 변동 처리
	@param		: 데미지
*/
/*virtual*/ void CX2GUAdd::UpNowDPValueByDamage( IN const float fDamage_ )
{
	/// 구성 모드 중일땐, 제외
	if ( true == GetIsFormationMode() )
		return;

	/// 현재 체력 비율
	float fNowDPValue = fDamage_ / GetMaxHp();

	/// 자신의 최대 체력보다 더 큰 데미지가 들어왔을 때, 1배율로 고정
	if ( 1.f < fNowDPValue )
		fNowDPValue = 1.f;

	/// 피격 시 DP획득량 = ( 입은 피해량 / 총HP량 ) * 2400
	fNowDPValue = fNowDPValue * m_fDPRateByFormationModeHit;
	

	// 스킬에 의한 증가 값 적용
	float fIncreaseValueRateBySkill = 1.f;
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2UserSkillTree& cUserSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;		// 유저가 배운 스킬 트리

		fIncreaseValueRateBySkill = cUserSkillTree.GetIncreaseGainSoulRateByDamage();			// 피격시 증가되는 각성충전속도
	}

	UpNowDPValue( fNowDPValue, fIncreaseValueRateBySkill );
}

/** @function	: UpNowDPValueByHit
	@brief		: 타격시 DP 변동 처리
	@param		: 데미지 데이터
*/
/*virtual*/ void CX2GUAdd::UpNowDPValueByHit( IN const CX2DamageManager::DamageData& DamageData_ )
{
	const float fPowerRate		= GetPowerRate();	/// 기본 데미지 배율
	const float fSkillTypeRate	= GetDPRateByType( DamageData_.m_DPRateType );	/// 스킬 타입에 따른 배율
	float		fDamageRate		= 0.f;				/// 데미지 타입에 따른 배율

	/// 데미지 타입에 따른 배율 설정
	switch ( DamageData_.damageType )
	{
	case CX2DamageManager::DT_PHYSIC:	fDamageRate = DamageData_.damage.fPhysic; break;
	case CX2DamageManager::DT_MAGIC:	fDamageRate = DamageData_.damage.fMagic; break;
	case CX2DamageManager::DT_MIX:		fDamageRate = ( DamageData_.damage.fPhysic + DamageData_.damage.fPhysic ) / 2.f; break;
	}

	float fNowDPValue = 0.f;		/// 적용시킬 DP 수치

	/// 구성 모드 여부에 따라, 수치 연산
	if ( false == GetIsFormationMode() )
		/// 타격 시 DP획득량 = 데미지 배율 * 파워레이트 * 20 * 스킬TYPE상수 (커맨드2, 액티브1, 스페셜액티브0)
		fNowDPValue += fPowerRate * fDamageRate * m_fDPRateByNoFormationModeAttack * fSkillTypeRate;
	else
		/// 타격 시 DP소모량 = 데미지 배율 * 파워레이트 * 20 * 스킬TYPE상수 (커맨드2, 액티브1, 스페셜액티브0)
		fNowDPValue -= fPowerRate * fDamageRate * m_fDPRateByFormationModeAttack * fSkillTypeRate;

	// 스킬에 의한 증가 값 적용
	float fIncreaseValueRateBySkill = 1.f;
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2UserSkillTree& cUserSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;	// 유저가 배운 스킬 트리

		fIncreaseValueRateBySkill = cUserSkillTree.GetIncreaseGainSoulRateByHit();			// 타격시 증가되는 각성충전속도
	}

	UpNowDPValue( fNowDPValue, fIncreaseValueRateBySkill );
}

/** @function	: UpNowDPValueByUseMP
	@brief		: MP 사용시 DP 변동 처리
	@param		: MP 사용량
*/
void CX2GUAdd::UpNowDPValueByUseMP( IN const float fUseMP )
{
	/// 구성 모드 중일땐, 제외
	if ( true == GetIsFormationMode() )
		return;

	/// MP소모시 DP획득량 = 사용한MP * 4.8
	const float fNowMPValue = fUseMP * m_fDPRateByFormationModeUseMP;

	UpNowDPValue( fNowMPValue );
}

/** @function	: 해당 스킬에 대한 DP 소모량 반환
	@brief		: GetActualDPConsume
	@param		: 스킬 정보
	@return		: DP 소모량
*/
const float CX2GUAdd::GetActualDPConsume( IN const CX2UserSkillTree::SkillSlotData* pSkillSlotData_ )
{
	float fDPConsume = 0.f;		/// DP 소모량

	/// 비각성시 MP를 쓰고, 각성시 DP를 쓰는 스킬일 때만 DP 소모 설정
	if ( true == GetIsFormationMode() )
	{
		/// 비각성시 MP를 쓰고, 각성시 DP를 쓰는 스킬일 때만 DP 소모 설정
		const float fUseDPSkillAtFormationMode = GetSpecialAbilityValue( CX2SkillTree::SA_DP_USE_REL, pSkillSlotData_->m_eID );

		if ( 0.f < fUseDPSkillAtFormationMode )
		{
			/// 소모 MP의 배율로 DP 소모 ( UI에 표시된 DP에 비해 실 DP는 20배 크다 )
			const float fMPConsume = CX2GUUser::GetActualMPConsume( pSkillSlotData_->m_eID, GetSkillLevelValue( pSkillSlotData_->m_eID ) );

			fDPConsume = fUseDPSkillAtFormationMode * fMPConsume * 20.f;
		}
	}
	else
	{
		/// DP는 기력 인자를 사용한다.
		if ( NULL != pSkillSlotData_ )
			fDPConsume = static_cast<float>( pSkillSlotData_->m_usFPConsumption );

		switch ( pSkillSlotData_->m_eID )
		{
		case CX2SkillTree::SI_A_AN_PHASE_SHIFT:
			{
				/// 위상 변화 복귀시, DP소모하지 않기
				if ( true == m_PhaseShiftData.IsValidReturnPosValue() )
					fDPConsume = 0.f;
	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				/// 환상 추적자 발동시, DP소모하지 않기
				else if( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PHASE_SHIFT ) )
					fDPConsume = 0.f;
	#endif //SERV_ADD_LUNATIC_PSYKER
			} break;
	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		case CX2SkillTree::SI_A_AN_PULSE_BULLET:
			{
				/// 환상 추적자 발동시, DP소모하지 않기
				if( true == GetIsFantasyTracerTargetSkillID( CX2SkillTree::SI_A_AN_PULSE_BULLET ) )
					fDPConsume = 0.f;
			} break;

		case CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL:
			{
				/// 퀵실버 액셀 후진시에는 DP소모하지 않기
				if ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == GetFutureStateSkillID())
					fDPConsume = 0.f;
			} break;
	#endif //SERV_ADD_LUNATIC_PSYKER
		default:
			break;
		}
	}

	return fDPConsume;
}

/** @function	: 해당 스킬에 대한 변이 포인트 소모량 반환
	@brief		: GetMutationPointConsume
	@param		: 스킬 정보
	@return		: 변이 포인트 소모량
*/
const int CX2GUAdd::GetMutationPointConsume( IN const CX2UserSkillTree::SkillSlotData* pSkillSlotData_ )
{
	int iMutationPointConsume = 0;		/// 변이 포인트 소모량

	/// 변이 포인트는 캐논볼 인자를 사용한다.
	if ( NULL != pSkillSlotData_ )
		iMutationPointConsume = static_cast<int>( pSkillSlotData_->m_usCBConsumption );

	return iMutationPointConsume;
}

/** @function	: GetIsFormationMode
	@brief		: 구성 모드 적용 여부 반환 함수
	@return		: 구성 모드 적용 여부
*/
/*virtual*/ const bool CX2GUAdd::GetIsFormationMode() const
{ 
	return ( NULL != GetAddGageData() ? GetAddGageData()->GetIsFormationMode() : false ); 
}

/** @function	: SetIsFormationMode
	@brief		: 구성 모드 적용 여부 설정 함수
	@param		: 구성 모드 적용 여부
*/
/*virtual*/ void CX2GUAdd::SetIsFormationMode( IN const bool bIsFormationMode_ )
{
	if ( NULL != GetAddGageData() )
		GetAddGageData()->SetIsFormationMode( bIsFormationMode_ );

	/// 구성 모드 쿨타임 설정
	if ( true == bIsFormationMode_ )
		SetChangeFormationCoolTime( m_fChangeFormationCoolTime );
}

/** @function	: IsPossibleChangeNasodBarrier
	@brief		: 나소드 배리어 실행 처리 함수
	@return		: 나소드 배리어 실행 여부
*/
bool CX2GUAdd::IsPossibleChangeNasodBarrier(  IN const CX2DamageManager::DamageData* pDamageData_ )
{
	if ( NULL != pDamageData_ )
	{
		/// 나소드 배리어 발동 가능한 스테이트 이며 DP가 남아 있고, 더미 데미지가 아니라면 배리어 발동
		if ( true == m_bPossibleNasodArmorModeState && 
			 true == GetIsFormationMode() &&
			 0.f < GetDPValue() &&
			 CX2DamageManager::RT_DUMMY_DAMAGE != pDamageData_->reActType )
		{
			/// DP 베리어 스테이트로 전환
			StateChange( ANSI_DP_BARRIER_ACTIVE );

			return true;
		}
	}

	return false;
}

/** @function	: PlayAnimationWaitStartByAdd
	@brief		: 상황에 따른 Wait 모션 처리
	@param		: 애니메이션 객체, 프레임 데이터
*/
void CX2GUAdd::PlayAnimationWaitStartByAdd( IN CKTDGXSkinAnim* pXSkinAnim_, IN const FrameData& refFrameData_ )
{
	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType;
	LUA_GET_VALUE_ENUM ( m_LuaManager, "PLAY_TYPE", playType, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE, CKTDGXSkinAnim::XAP_ONE_WAIT );

	// FATAL_HP_PERCENT 보다 HP가 적으면 fatal 애니메이션이 실행
	const float FATAL_HP_PERCENT = 0.25f;

	if( GetMaxHp() * FATAL_HP_PERCENT >= refFrameData_.syncData.fNowHP )
	{
		bool bTransition = true;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );

		if( false == refFrameData_.unitCondition.bTransitionChange || false == bTransition )
			pXSkinAnim_->ChangeAnim( L"Fatal", false );
		else
			pXSkinAnim_->ChangeAnim( L"Fatal", true );
	}
	else
	{
		bool bTransition = true;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );

		if( false == refFrameData_.unitCondition.bTransitionChange || false == bTransition )
		{
			pXSkinAnim_->ChangeAnim( L"Wait", false );
		}
		else
		{
			pXSkinAnim_->ChangeAnim( L"Wait", true );
		}
	}

	pXSkinAnim_->Play( playType );
}

/** @function	: IsDPConsumptionSkill
	@brief		: 해당 스킬이 DP를 사용하는 스킬인지 검사
				/// ***각성시에만 DP를 소모하더라도, DP소모하는 스킬로 판단 함
	@param		: 스킬 아이디
	@return		: DP 스킬 여부
*/
bool CX2GUAdd::IsDPConsumptionSkill( IN const CX2SkillTree::SKILL_ID eSkillID_ ) const
{
	// ID를 지정하지 않으면, DP 소모하지 않는 스킬로 판단 함
	if( CX2SkillTree::SI_NONE == eSkillID_)
		return false;

	if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID_ );

		if ( NULL == pSkillTemplet )
			return false;

		/// DP 소모량을 받아와서, 소모량이 존재한다면 DP를 사용하는 스킬로 판단
		const float fDPConsumptionValue = pSkillTemplet->GetSkillDPConsumptionValue();

		if ( 0 < fDPConsumptionValue )
			return true;
	}

	return false;	/// DP를 소모하는 스킬이 아니네요
}
/** @function : GetUseDPByFormationMode
	@brief : 구성 모드로 전환시 소모되는 DP 수치
*/
const float CX2GUAdd::GetUseDPByFormationMode() 
{
	ASSERT( 0.f < m_fDPValueRateByActiveFormationMode );

	// 소켓 및 패시브 스킬로 인한 DP 소모량 감소값
	const float fConsumeDP = m_fUseDPByFormationMode * ( m_fDPValueRateByActiveFormationMode + GetEffectiveDecreaseDPConsume() );

	return fConsumeDP;
}

/** @function	: GetEffectiveIncreaseDPValue
	@brief		: DP 획득량 증가값 반환
	@return		: DP 획득량 증가값
*/
const float CX2GUAdd::GetEffectiveIncreaseDPValue()
{
	// 소켓 효과(각성충전속도) ( ex 50%증가 : 0.5f )		/// 애드는 절반만 적용 된다.  ( 소켓 적용시 절반 깎으므로, 여기에선 절반 처리 제외 )
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// 버프 적용 구조로 변경

	const float fChangeRate_BySocket = GetHyperChargeSpeed();
	
#else // SERV_ADD_LUNATIC_PSYKER
	const float fChangeRate_BySocket = CX2SocketItem::GetFinalHyperGageChargeSpeedPercent( GetSocketData().m_fHyperModeChargeRate, GetUnitLevel() ) * 0.5f;
#endif // SERV_ADD_LUNATIC_PSYKER

	// 패시브 스킬(다이너모 연구) ( ex 15%증가 : 1.15 -> 0.15f )
	const float fChangeRate_ByPassiveSkill = m_fGainDPValueChangeRate - 1.f;

	return fChangeRate_BySocket + fChangeRate_ByPassiveSkill;
}

/** @function	: GetEffectiveDecreaseDPConsume
	@brief		: DP 소모량 감소값 반환
	@return		: DP 소모량 감소값
*/
const float	CX2GUAdd::GetEffectiveDecreaseDPConsume()
{
	// 소켓 효과(각성지속시간)  (ex 6%증가 : 6 -> 0.06)		/// 애드는 절반만 적용 된다. ( 소켓 적용시 절반 깎으므로, 여기에선 절반 처리 제외 )
	const float fChangeRate_BySocket = GetSocketData().m_fHyperModeTime * 0.01f;

	// 패시브 스킬(다이너모 연구) (ex -2%감소 : 0.98 -> 0.02)
	const float fChangeRate_Passive = 1.f - m_fUsingDPValueChangeRate;
	return fChangeRate_BySocket + fChangeRate_Passive;
}

/** @function	: StateChange
	@brief		: 스테이트 변경 함수
	@param		: 스테이트 아이디, 즉시 스테이트 변경 처리 여부
	@return		: 변경 성공 여부
*/
/*virtual*/ bool CX2GUAdd::StateChange( int state, bool bDirectChange /*= false*/ )
{
	/// 대기 상태일 때, 구성 모드 중이라면 구성 모드 대기 상태로 설정하자.
	if ( USI_WAIT == state )
	{
		if ( true ==  GetIsFormationMode() )
			state = ANSI_HYPER_WAIT;
	}

	return CX2GUUser::StateChange( state, bDirectChange );
}

/** @function : ApplyHyperModeBuff
	@brief : 각성버프 적용
*/
/*virtual*/ void CX2GUAdd::ApplyHyperModeBuff()
{
	SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_FORMATION_MODE ), 1 );

	// 무한의 도서관 버프 적용
	if( 0 < m_iLibraryOfLimitlessLevel )
		SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_LIBRARY_OF_LIMITLESS_MANA_RECOVERY ), m_iLibraryOfLimitlessLevel );

#ifdef HAMEL_SECRET_DUNGEON // 김태환
	/// 각성시 적용할 버프가 있는지 검사
	if ( false == m_SocketData.m_vecInfoByUseHyperMode.empty() &&
		GUT_USER == GetGameUnitType() )
	{
		CX2GUUser* pGUUser = static_cast<CX2GUUser*>( this );

		/// 버프 적용
		if ( NULL != pGUUser )
		{
#ifdef ADJUST_SECRET_ITEM_OPTION //김창한
			if( pGUUser->GetSocketOptionHyperCoolTime() <= 0.f )
#endif //ADJUST_SECRET_ITEM_OPTION
				pGUUser->ApplyBuffByUseHyperMode();
		}
	}
#endif // HAMEL_SECRET_DUNGEON
}

/** @function	: GetAbilitySyncData
	@brief		: 동기화 시킬 정보값 반환 함수
	@return		: 동기화 시킬 정보값 ( DP, 변이 수치 )
*/
const SHORT CX2GUAdd::GetAbilitySyncData()
{
	SHORT sAbilitySyncData = 0;

	/// 변이 수치 대입 ( 변이 수치는 절대 3 비트를 넘어서면 않돼요! )
	sAbilitySyncData |= static_cast<SHORT>( GetMutationCount() );

	/// DP 수치 대입을 위해, 이동
	sAbilitySyncData = sAbilitySyncData << 13;

	/// DP 수치 대입 ( DP는 절대 13 비트를 넘어서면 않돼요! )
	sAbilitySyncData |= static_cast<SHORT>( GetDPValue() );

	return sAbilitySyncData;
}

/** @function	: SetAbilitySyncData
	@brief		: 정보값 동기화 값을 실제 값에 적용하는 함수
	@param		: 정보값 동기화 값
*/
void CX2GUAdd::SetAbilitySyncData( IN const SHORT sAbilitySyncData_ )
{
	/// DP 수치 설정 ( 3비트 )
	const float fNPValue		= static_cast<float>( sAbilitySyncData_ & 0x1FFF ) ;
	SetDPValue( fNPValue );

	/// 변이 수치 설정 ( 13비트 )
	const int iMutationCount	= static_cast<int>( ( sAbilitySyncData_ >> 13 ) & 0x0007 );
	SetMutationCount( iMutationCount );
}

/** @function	: AddPylonLinkDamageEffect
	@brief		: 파일런간 연결 데미지 이펙트 생성 함수
	@param		: 추가할 파일런 데미지 이펙트 핸들
*/
void CX2GUAdd::AddPylonLinkDamageEffect( IN CX2DamageEffect::CEffect* pAddPylon_ )
{
	if ( NULL == pAddPylon_ )
		return;

	const CX2DamageEffect::CEffectHandle hAddPylonHandle = pAddPylon_->GetHandle();

	/// 설치되어 있는 파일런 검사
	BOOST_FOREACH( const PylonDamageEffectData& AnotherPylonData, m_vecSummonPylonDamageEffectData )
	{
		bool bIsLink = false;		/// 다른 파일런과 데미지 이펙트 연결 여부

		/// 현재 생성된 연결 데미지 이펙트 검사
		BOOST_FOREACH( const PylonLinkDamageEffectData& LinkData, m_vecPylonLinkDamageEffectData )
		{
			const CX2DamageEffect::CEffectHandle iStartLinkPylonHandle	= LinkData.m_hPylonHandle.first;	/// 연결된 파일런 핸들
			const CX2DamageEffect::CEffectHandle iEndLinkPylonHandle	= LinkData.m_hPylonHandle.second;	/// 연결된 반대편 파일런 핸들

			/// 검사하려는 두 파일런 간 연결된 데미지 이펙트인지 검사
			if ( ( hAddPylonHandle					== iStartLinkPylonHandle	|| hAddPylonHandle					== iEndLinkPylonHandle ) &&
				( AnotherPylonData.m_EffectHandle	== iStartLinkPylonHandle	|| AnotherPylonData.m_EffectHandle	== iEndLinkPylonHandle ) )
			{
				bIsLink = true;		/// 연결 되었어요.
				break;
			}
		}

		/// 연결 되어 있지 않다면, 연결용 데미지 이펙트 생성
		if ( false == bIsLink )
		{
			/// 파일런이 존재하지 않거나, 죽어있으면 다음 파일런을 검사
			if ( NULL == g_pX2Game || NULL == g_pX2Game->GetDamageEffect() )
				continue;

			CX2DamageEffect::CEffect* pAnotherEffect = g_pX2Game->GetDamageEffect()->GetInstance( AnotherPylonData.m_EffectHandle );

			if ( NULL == pAnotherEffect )
				continue;
			
			/// 두 이펙트 사이 거리 검사
			float fDistance = GetDistance( pAddPylon_->GetPos(), pAnotherEffect->GetPos() );

			if ( 0 >= fDistance )
				fDistance = 1.f;

			/// 특정 거리보다 멀면 다음 파일런을 검사
			if ( 1000.f	< fDistance )
				continue;

			/// Main Mesh 반환
			CKTDGXMeshPlayer::CXMeshInstance* pAddMeshInst		= pAddPylon_->GetMainEffect();
			CKTDGXMeshPlayer::CXMeshInstance* pAnotherMeshInst	= pAnotherEffect->GetMainEffect();

			if ( NULL == pAddMeshInst || NULL == pAnotherMeshInst )
				continue;

			const D3DXVECTOR3	vAddPylonPos	= pAddMeshInst->GetBonePos(L"Dummy01");	/// 위치값
			const D3DXVECTOR3	vAddPylonRot	= pAddMeshInst->GetRotateDegree();					/// 회전값

			/// 연결 데미지 이펙트 생성
			CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_APT_Pylon_Additional_Shock", 
				GetPowerRate(), vAddPylonPos, vAddPylonRot, vAddPylonRot );

			if ( NULL == pEffect )
				continue;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			const D3DXVECTOR3 vAnotherPylonPos = pAnotherMeshInst->GetBonePos(L"Dummy01");

			/// 두 파일런 간 데미지 이펙트 매시 연결 설정 ( 셰도우 링커 참고 )
			pEffect->SetLinkMainEffectByPos( vAddPylonPos, vAnotherPylonPos, fDistance );
#else // SERV_ADD_LUNATIC_PSYKER


			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pEffect->GetMainEffect();		/// 연결 데미지 이펙트의 Main Mesh

			if ( NULL == pMeshInst )
				continue;

			/// 두 파일런 간 데미지 이펙트 매시 연결 설정 ( 셰도우 링커 참고 )
			const D3DXVECTOR3	vAnotherPylonPos = pAnotherMeshInst->GetBonePos(L"Dummy01");

			D3DXVECTOR3			vResultPos( vAnotherPylonPos.x - vAddPylonPos.x,
				vAnotherPylonPos.y - vAddPylonPos.y,
				vAnotherPylonPos.z - vAddPylonPos.z );

			vResultPos = GetDirVecToDegree( vResultPos );
			pMeshInst->SetRotateDegree( vResultPos );
			pMeshInst->SetMoveAxisAngleDegree( vResultPos );
			pMeshInst->SetScale( fDistance / 900.f, 1.f, 1.f );
#endif // SERV_ADD_LUNATIC_PSYKER
			
			/// 생성한 데미지 이펙트 저장
			m_vecPylonLinkDamageEffectData.push_back( PylonLinkDamageEffectData( pEffect->GetHandle(), std::make_pair( hAddPylonHandle, AnotherPylonData.m_EffectHandle ) ) );
		}
	}
}

/** @function	: DeletePylonLinkDamageEffect
	@brief		: 파일런간 연결 데미지 이펙트 삭제 함수
	@param		: 삭제할 파일런 데미지 이펙트 핸들
*/
void CX2GUAdd::DeletePylonLinkDamageEffect( IN CX2DamageEffect::CEffectHandle hDeletePylonHandle_ )
{
	/// 현재 생성된 연결 데미지 이펙트 검사
	vector<PylonLinkDamageEffectData>::const_iterator vItLinkData = m_vecPylonLinkDamageEffectData.begin();

	while( vItLinkData != m_vecPylonLinkDamageEffectData.end() )
	{
		PylonLinkDamageEffectData LinkData = *vItLinkData;

		/// 삭제한 파일런과 연결된 데미지 이펙트가 있다면, 해제
		if ( hDeletePylonHandle_ == LinkData.m_hPylonHandle.first || hDeletePylonHandle_ == LinkData.m_hPylonHandle.second )
		{
			if ( NULL != g_pX2Game &&
				 NULL != g_pX2Game->GetDamageEffect() )
			{
				g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( LinkData.m_hLinkEffectHandle );
			}

			/// 저장된 연결 정보 삭제
			vItLinkData = m_vecPylonLinkDamageEffectData.erase( vItLinkData );
		}
		else
			++vItLinkData;
	}
}

/** @function	: ProcessPylonDamageEffect
	@brief		: 파일런 유지 시간 제어 함수
*/
void CX2GUAdd::ProcessPylonDamageEffect()
{
	vector<PylonDamageEffectData>::iterator vItSummonPylon = m_vecSummonPylonDamageEffectData.begin();

	while( vItSummonPylon != m_vecSummonPylonDamageEffectData.end() )
	{
		PylonDamageEffectData& PylonData = *vItSummonPylon;

		float& fRemainTime = PylonData.m_fRemainTime;
			
		fRemainTime -= m_fElapsedTime;

		if ( 0.f >= fRemainTime && NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		{
			/// 링크된 데미지 이펙트 정리
			DeletePylonLinkDamageEffect( PylonData.m_EffectHandle );

			/// 파일런 및 버프용 데미지 이펙트 제거
			g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( PylonData.m_EffectHandle );
			g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( PylonData.m_BuffEffectLHandle );

			vItSummonPylon = m_vecSummonPylonDamageEffectData.erase( vItSummonPylon );
		}
		else
			++vItSummonPylon;
	}
}

/** @function	: GetSummonPylonPos
	@brief		: 파일런 소환 가능한 위치 반환 함수
	@param		: 위치값
	@return		: 정상 여부
*/
bool CX2GUAdd::GetSummonPylonPos( OUT D3DXVECTOR3& fSummonPos )
{
	if ( NULL != g_pX2Game &&
		 NULL != g_pX2Game->GetWorld() &&
		 NULL != g_pX2Game->GetWorld()->GetLineMap() &&
		 NULL != g_pX2Game->GetDamageEffect() )

	{
		/// 검사할 소환 위치 Offset
		float fCheckOffsetPosition[3] = { 100.f, 0.f, -100.f };

		for ( int i = 0 ; i < ARRAY_SIZE( fCheckOffsetPosition ) ; ++i )
		{
			bool bPossibleSummon = true;		/// 소환 가능 여부

			/// 소환 위치 Offset 연산
			D3DXVECTOR3 fMovePosition = GetIsRight() ? GetPos() + ( GetDirVector() * fCheckOffsetPosition[i] ) : 
													   GetPos() - ( GetDirVector() * fCheckOffsetPosition[i] );

			/// 현재 설치된 파일런들의 영역과 겹치지 않는지 검사
			BOOST_FOREACH( PylonDamageEffectData PylonData, m_vecSummonPylonDamageEffectData )
			{
				CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( PylonData.m_EffectHandle );

				if ( NULL != pEffect && 250.f >= GetDistance( fMovePosition, pEffect->GetPos() ) )
				{
					bPossibleSummon = false;	/// 겹치는 영역이 있어요!
					break;
				}
			}

			/// 다른 파일런들과 겹치는 영역이 없다면, 해당 위치 반환
			if ( true == bPossibleSummon )
			{
				fSummonPos = fMovePosition;
				return true;
			}
		}
	}

	return false;
}

/** @function	: ResetPylonData
	@brief		: 파일런 관련 모든 정보 및 이펙트 초기화
*/
void CX2GUAdd::ResetPylonData()
{
	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
	{
		BOOST_FOREACH( PylonDamageEffectData PylonData, m_vecSummonPylonDamageEffectData )
		{
			g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( PylonData.m_EffectHandle );
		}

		BOOST_FOREACH( PylonLinkDamageEffectData LinkData, m_vecPylonLinkDamageEffectData )
		{
			g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( LinkData.m_hLinkEffectHandle);
		}

		m_vecSummonPylonDamageEffectData.resize( 0 );
		m_vecPylonLinkDamageEffectData.resize( 0 );
	}
}

/** @function	: ProcessNonstopSummonPylon
	@brief		: 파일런 연속 소환 관련 제어
*/
void CX2GUAdd::ProcessNonstopSummonPylon()
{
	if ( 0.f < m_fNonstopSummonPylonCoolTime )
	{
		m_fNonstopSummonPylonCoolTime -= m_fElapsedTime;

		/// 쿨타임 만료시, 쿨타임 및 연속 소환 카운트 초기화
		if ( 0.f >= m_fNonstopSummonPylonCoolTime )
		{
			m_fNonstopSummonPylonCoolTime	= 0.f;
			m_uiNonstopSummonPylonCount		= 0;
		}
	}
}

/** @function	: GetPossibleNonstopSummonPylon
	@brief		: 파일런 연속 소환 가능 여부
*/
const bool CX2GUAdd::GetPossibleNonstopSummonPylon() const
{
	/// 연속 소환 제한수를 넘지 않고, 연속 소환 쿨타임이 남아있다면 소환 가능
	if ( 1 <= m_uiNonstopSummonPylonCount && 
		_CONST_ADD_::PYLON_NONESTOP_SUMMON_LIMIT_NUM > m_uiNonstopSummonPylonCount &&
		0.f < m_fNonstopSummonPylonCoolTime )
		return true;

	return false;
}

/** @function	: SetNasodArmorAttackTarget
	@brief		: 나소드 아머 공격 타겟팅 설정 함수
	@param		: 데미지 데이터
*/
void CX2GUAdd::SetNasodArmorAttackTarget( IN const CX2DamageManager::DamageData& pDamageData_ )
{
	if( NULL != g_pX2Game &&
		0.f  >= pDamageData_.m_fHpPercentUp &&
		null != pDamageData_.optrDefenderGameUnit &&
		CX2DamageManager::AT_UNIT == pDamageData_.defenderType )
	{
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		/// 환상 추적자 패시브로 인하여, 나소드 아머 공격 타겟팅 구문 로직 변경

		NasodArmorAttackTargetData	NAAttackTargetData		= NasodArmorAttackTargetData();	/// 빈 타겟 객체
		bool						bIsSuccessSetTargetData	= false;	/// 타겟 정보 기록 여부

		/// 타겟 정보 설정
		if( CX2GameUnit::GUT_USER == pDamageData_.optrDefenderGameUnit->GetGameUnitType() )
		{
			NAAttackTargetData.SetTargetData( pDamageData_.optrDefenderGameUnit->GetUnitUID(), false );

			bIsSuccessSetTargetData = true;		/// 기록 성공
		}
		else if( CX2GameUnit::GUT_NPC == pDamageData_.optrDefenderGameUnit->GetGameUnitType() )
		{
			CX2GUNPC *pNpc = static_cast<CX2GUNPC *>( pDamageData_.optrDefenderGameUnit.GetObservable() );

			if( pNpc != NULL && CX2UnitManager::NCT_BASIC == pNpc->GetNPCTemplet().m_ClassType )
			{
				NAAttackTargetData.SetTargetData( pDamageData_.optrDefenderGameUnit->GetUnitUID(), true );
				bIsSuccessSetTargetData = true;	/// 기록 성공
			}
		}

		/// 타겟 정보 기록 실패 했다면, 패스
		if ( false == bIsSuccessSetTargetData )
			return;

		const CX2SkillTree::SKILL_ID eSkillID = pDamageData_.m_eUsedSkillID;	/// 스킬 아이디

		switch ( eSkillID )
		{
		//////////////////////////////////////////////////////////////////////////
		/// 특정 스킬에 대한 타겟 정보 설정
		case CX2SkillTree::SI_A_AN_PULSE_BULLET:	/// 입자탄
		case CX2SkillTree::SI_A_AN_PHASE_SHIFT:		/// 위상 변화
			{
				/// 구성 모드 중일때만 설정
				if ( true == GetIsFormationMode() )
				{
					vector<FantasyTracerTargetInfo>::iterator vIt = m_vecFantasyTracerTargetInfo.begin();

					/// 만약 같은 ID의 타겟 정보가 들어있다면, 지워버리자
					for ( vIt ; vIt != m_vecFantasyTracerTargetInfo.end(); ++vIt )
					{
						if ( vIt->m_eSkillID == eSkillID )
						{
							m_vecFantasyTracerTargetInfo.erase( vIt );
							break;	/// 지웠으니 탈출
						}
					}

					/// 타겟 유지 시간 설정 ( 실제 유지 시간 + 삭제 대기 시간 )
					const float fRemainTime = _CONST_ADD_::FANTASY_TRACER_TARGET_REMAIN_TIME + _CONST_ADD_::FANTASY_TRACER_TARGET_DELETE_READY_TIME;

					/// 환상 추적자 타겟 정보 설정
					FantasyTracerTargetInfo FTTargetInfo = FantasyTracerTargetInfo( fRemainTime, NAAttackTargetData, eSkillID );

					/// 정보 삽입
					m_vecFantasyTracerTargetInfo.push_back( FantasyTracerTargetInfo( FTTargetInfo ) );

					/// 아이콘 갱신
					if ( NULL != g_pX2Game )
						g_pX2Game->UpdateSkillSlotUI();
				}
			} break;
		//////////////////////////////////////////////////////////////////////////
		/// 그 외 ( 일반 타겟 정보 설정 )
		default:
			{
				const D3DXVECTOR3 vMyPos		= GetPos();
				const D3DXVECTOR3 vTargetPos	= pDamageData_.optrDefenderGameUnit->GetPos();

				/// 타겟팅 범위 검사
				if ( _CONST_ADD_::NASOD_ARMOR_TARGET_RANGE < GetDistance( vMyPos, vTargetPos ) )
					return;

				/// 대전일 경우, 추가로 높이 범위 검사
				if ( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
				{
					const D3DXVECTOR3 vMyBonePos		= GetBonePos( L"Bip01" );
					const D3DXVECTOR3 vTargetBonePos	= pDamageData_.optrDefenderGameUnit->GetBonePos( L"Bip01" );

					if ( _CONST_ADD_::NASOD_ARMOR_TARGET_HEIGHT_RANGE < abs( vMyBonePos.y - vTargetBonePos.y ) )
						return;
				}

				/// 이전 타겟 정보에 대입
				m_NasodArmorAttackTargetData = NAAttackTargetData;
			}
		}

#else // SERV_ADD_LUNATIC_PSYKER
		const D3DXVECTOR3 vMyPos		= GetPos();
		const D3DXVECTOR3 vTargetPos	= pDamageData_.optrDefenderGameUnit->GetPos();

		/// 타겟팅 범위 검사
		if ( _CONST_ADD_::NASOD_ARMOR_TARGET_RANGE < GetDistance( vMyPos, vTargetPos ) )
			return;

		/// 대전일 경우, 추가로 높이 범위 검사
		if ( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
		{
			const D3DXVECTOR3 vMyBonePos		= GetBonePos( L"Bip01" );
			const D3DXVECTOR3 vTargetBonePos	= pDamageData_.optrDefenderGameUnit->GetBonePos( L"Bip01" );

			if ( _CONST_ADD_::NASOD_ARMOR_TARGET_HEIGHT_RANGE < abs( vMyBonePos.y - vTargetBonePos.y ) )
				return;
		}

		/// 타겟 정보 설정
		if( CX2GameUnit::GUT_USER == pDamageData_.optrDefenderGameUnit->GetGameUnitType() )
		{
			m_NasodArmorAttackTargetData.SetTargetData( pDamageData_.optrDefenderGameUnit->GetUnitUID(), false );
		}
		else if( CX2GameUnit::GUT_NPC == pDamageData_.optrDefenderGameUnit->GetGameUnitType() )
		{
			CX2GUNPC *pNpc = static_cast<CX2GUNPC *>( pDamageData_.optrDefenderGameUnit.GetObservable() );

			if( pNpc != NULL && CX2UnitManager::NCT_BASIC == pNpc->GetNPCTemplet().m_ClassType )
				m_NasodArmorAttackTargetData.SetTargetData( pDamageData_.optrDefenderGameUnit->GetUnitUID(), true );
		}
#endif // SERV_ADD_LUNATIC_PSYKER
		
	}
}

/** @function	: ProcessMagnetronDamageEffect
	@brief		: 마그네트론 위치 제어 함수
*/
void CX2GUAdd::ProcessMagnetronDamageEffect()
{
	vector<CX2DamageEffect::CEffectHandle>& vecMagnetronDamageEffectData = m_MagnetronData.m_vecMagnetronDamageEffectData;

	if ( true == vecMagnetronDamageEffectData.empty() || 
		 NULL == g_pX2Game || 
		 NULL == g_pX2Game->GetDamageEffect() )
		return;

	vector<CX2DamageEffect::CEffectHandle>::iterator vIt = vecMagnetronDamageEffectData.begin();

	while( vIt != vecMagnetronDamageEffectData.end() )
	{
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( *vIt );

		/// 유저 뒤 특정 위치로 이동
		if ( NULL != pEffect )
		{
			pEffect->SetPos( GetBonePos( L"Bip01" ) );
			++vIt;
		}
		else
			vIt = vecMagnetronDamageEffectData.erase( vIt );
	}

	/// 마그네트론 발사 간격 연산
	float& fFireReadyTime = m_MagnetronData.m_fFireReadyTime;

	if ( 0.f < fFireReadyTime )
	{
		fFireReadyTime -= m_fElapsedTime;

		if ( 0.f >= fFireReadyTime )
			fFireReadyTime = 0.f;
	}
}

/** @function	: SetMagnetronDamageEffectTarget
	@brief		: 다이너모 구성 - 마그네트론 타겟 설정
	@param		: 데미지 데이터
*/
void CX2GUAdd::AttackMagnetronDamageEffect( IN const CX2DamageManager::DamageData& pDamageData_ )
{
	if( 0.f < m_MagnetronData.m_fFireReadyTime ||
		true == m_MagnetronData.m_vecMagnetronDamageEffectData.empty() || 
		NULL == g_pX2Game ||
		NULL == g_pX2Game->GetDamageEffect() ||
		null == pDamageData_.optrDefenderGameUnit ||
		CX2DamageManager::AT_UNIT != pDamageData_.defenderType ||
		CX2DamageManager::AT_SPECIAL == pDamageData_.attackType||
		CX2DamageManager::RT_DUMMY_DAMAGE == pDamageData_.reActResult )
		return;

	/// 마그네트론 관련 이펙트라면, 동작 중지 시키자
	if ( INVALID_DAMAGE_EFFECT_HANDLE != pDamageData_.hAttackerEffect )
	{
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( pDamageData_.hAttackerEffect );

		if ( NULL != pEffect )
		{
			const wstring wstrDamageEffectName = pEffect->GetName();

			if ( wstrDamageEffectName == L"DamageEffect_APT_Magnetron_Dynamo_Explosion" )
				return;
		}
	}

	/// 저장되어 있는 마그네트론 이펙트 중 가장 첫번째 선택
	vector<CX2DamageEffect::CEffectHandle>::iterator vIt = m_MagnetronData.m_vecMagnetronDamageEffectData.begin();

	CX2DamageEffect::CEffectHandle hEffectHandle = *vIt;

	CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( hEffectHandle );

	if( pEffect == NULL ) 
		return;

	/// 해당 데미지 이펙트의 Main Mesh 반환
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pEffect->GetMainEffect();

	if( pMeshInst == NULL ) 
		return;

	/// 발사 위치 반환
	const D3DXVECTOR3 vFirePos = pMeshInst->GetBonePos( L"Dummy02" );

	/// 발사 이펙트 생성
	CX2DamageEffect::CEffect* pFireEffect = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"DamageEffect_SA_APT_Magnetron_Dynamo_Projectile", GetPowerRate(), vFirePos, 
		GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	/// 타겟팅 정보 설정
	if ( NULL != pFireEffect )
	{
		CX2DamageEffect::LockOnData& FireLockOnData = pFireEffect->GetLockOnData();

		if( CX2GameUnit::GUT_USER == pDamageData_.optrDefenderGameUnit->GetGameUnitType() )
			FireLockOnData.m_LockOnUnitUID = pDamageData_.optrDefenderGameUnit->GetUnitUID();
		else if( CX2GameUnit::GUT_NPC == pDamageData_.optrDefenderGameUnit->GetGameUnitType() )
			FireLockOnData.m_LockOnNPCUID = static_cast<int>( pDamageData_.optrDefenderGameUnit->GetUnitUID() );
	}

	/// 이펙트 제거 ( 잔상 없이 바로! )
	g_pX2Game->GetDamageEffect()->DestroyInstanceHandleSilently( hEffectHandle );

	m_MagnetronData.m_vecMagnetronDamageEffectData.erase( vIt );

	/// 발사 간격 설정
	m_MagnetronData.m_fFireReadyTime = _CONST_ADD_::MAGNETRON_FIRE_READY_TIME;
}

/** @function	: SetStasisfieldHitValue
	@brief		: 스테이시스 필드 타격치 설정 함수
	@param		: 타격치
*/
/*virtual*/ void CX2GUAdd::SetStasisfieldHitValue( IN const float fHitValue_ )
{
	if ( true == m_StasisfieldData.GetIsActiveStasisfield() )
		m_StasisfieldData.m_fHitValue += fHitValue_;
}

/** @function	: SetStasisfieldDamageValue
	@brief		: 스테이시스 필드 피격치 설정 함수
	@param		: 피격치
*/
/*virtual*/ void CX2GUAdd::SetStasisfieldDamageValue( IN const float fDamageValue_ )
{
	if ( true == m_StasisfieldData.GetIsActiveStasisfield() )
		m_StasisfieldData.m_fDamageValue += fDamageValue_;
}

/** @function	: ProcessStasisField
	@brief		: 스테이시스 필드 매 프레임 처리 함수
*/
void CX2GUAdd::ProcessStasisField()
{
	if ( 0.f < m_StasisfieldData.m_fRemainTime )
	{
		m_StasisfieldData.m_fRemainTime -= m_fElapsedTime;

		/// 5초 이하로 남았는데 카운트 다운 이펙트가 생성된 적 없으면, 이펙트 생성
		if ( 5.f >= m_StasisfieldData.m_fRemainTime && 
			INVALID_EFFECTSET_HANDLE == m_StasisfieldData.m_hCountEffectSet )
		{
			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() )
			{
				CX2EffectSet::Handle hCountEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_APT_STASIS_FIELD_Count_Down", this, 
					NULL, false, -1.f, _CONST_ADD_::STASIS_FIELD_REMAIN_TIME );

				m_StasisfieldData.m_hCountEffectSet = hCountEffectSet;
			}
		}

		/// 스테이시스 필드 종료
		if ( 0.f >=  m_StasisfieldData.m_fRemainTime )
			EndStasisFieldByTime();
	}
}
/** @function : CreateDustStormToAttackResult
	@brief : #1. 타격에 의해 미립자 이펙트 생성(미립자 폭풍)
			 #2. AttackResult 함수에서 수행 됨
			 #3. 생성조건 : AT_SPECIAL 아니고, 생성한 damagedata의 공격력이 0이 아닐 때 확률에 의해 생성 
*/
void CX2GUAdd::CreateDustStormToAttackResult( const CX2DamageManager::DamageData& damageData_ )
{
	if( CX2DamageManager::AT_SPECIAL != damageData_.attackType && 0.f >= m_fDustStormCoolTime )
	{	
		float fRandVal = GetRandomFloat();;
		if( ( m_fDustStormRateByHit  * damageData_.m_fRateModifier) > fRandVal )
		{
			// # 미립자를 생성한 데미지의 공격력 배율 얻기
			float fDamageRate = 0.f;
			switch ( damageData_.damageType )
			{
			case CX2DamageManager::DT_PHYSIC:	
				fDamageRate = damageData_.damage.fPhysic;
				break;
			case CX2DamageManager::DT_MAGIC:	
				fDamageRate = damageData_.damage.fMagic; 
				break;
			case CX2DamageManager::DT_MIX:		
				fDamageRate = ( damageData_.damage.fPhysic + damageData_.damage.fPhysic ) / 2.f; 
				break;
			default:
				fDamageRate = 1.f;
				break;				
			}

			CreateDustStormDamageEffect(fDamageRate, damageData_.impactPoint );
		}
	}
}
/** @function : CreateDustStormToDamageReact
	@brief : 피격에 의해 미립자 이펙트 생성(미립자 폭풍)
	@brief : #1. 피격에 의해 미립자 이펙트 생성(미립자 폭풍)
			 #2. DamageReact 함수에서 수행 됨
			 #3. 생성조건 : AT_SPECIAL 아니고, 생성한 damagedata의 공격력이 0이 아닐 때 확률에 의해 생성 
*/
void CX2GUAdd::CreateDustStormToDamageReact( const CX2DamageManager::DamageData& damageData_ )
{
	if( CX2DamageManager::AT_SPECIAL != damageData_.attackType && 0.f >= m_fDustStormCoolTime )
	{	
		float fRandVal = GetRandomFloat();;
		if( ( m_fDustStormRateByDamage * damageData_.m_fRateModifier) > fRandVal )
		{
			// # 미립자를 생성한 데미지의 공격력 배율 얻기
			float fDamageRate = 0.f;
			switch ( damageData_.damageType )
			{
			case CX2DamageManager::DT_PHYSIC:
				fDamageRate = damageData_.damage.fPhysic;
				break;
			case CX2DamageManager::DT_MAGIC:
				fDamageRate = damageData_.damage.fMagic; 
				break;
			case CX2DamageManager::DT_MIX:
				fDamageRate = ( damageData_.damage.fPhysic + damageData_.damage.fPhysic ) / 2.f; 
				break;
			default:
				fDamageRate = 1.f;
				break;				
			}

			CreateDustStormDamageEffect(fDamageRate, damageData_.impactPoint );
		}
	}
}

/** @function	: CreateDustStormDamageEffect
	@brief		: 미립자 폭풍 데미지 이펙트 생성 함수
	@param		: 데미지 배율, 생성 위치
*/
void CX2GUAdd::CreateDustStormDamageEffect( const float fDamageRate_, const D3DXVECTOR3& vPos_ )
{
	if( false == IsSamef( fDamageRate_, 0.f ))
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_ADD_DUST_STORM", fDamageRate_, 
			vPos_, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		m_fDustStormCoolTime = _CONST_ADD_::DUST_STORM_FIRE_COOL_TIME;	/// 쿨타임 설정
	}
}

/** @function	: ProcessDustStormCoolTime
	@brief		: 미립자 폭풍 쿨타임 연산 함수
*/
void CX2GUAdd::ProcessDustStormCoolTime()
{
	if ( 0.f < m_fDustStormCoolTime )
	{
		m_fDustStormCoolTime -= m_fElapsedTime;

		if ( 0.f >= m_fDustStormCoolTime )
			m_fDustStormCoolTime = 0.f;
	}
}

/** @function	: EndStasisFieldByTime
	@brief		: 유지 시간이 지나서, 스테이시스 필드 종료 되었을 때 동작 처리 함수
*/
void CX2GUAdd::EndStasisFieldByTime()
{
	if ( NULL != g_pData && g_pData->GetSkillTree() && NULL != GetUnit() )
	{
		CX2UserSkillTree& userSkillTree = GetUnit()->AccessUnitData().m_UserSkillTree;
		CX2SkillTree* const pSkillTree = g_pData->GetSkillTree();

		const CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_SA_APT_STASIS_FIELD;

		const int iSkillLevel = userSkillTree.GetSkillLevel( eSkillID );
		if ( 0 < iSkillLevel )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( eSkillID );
			if( NULL != pSkillTemplet )
			{
				/// 피격 및 타격치 합산
				const float fTotalValue = m_StasisfieldData.m_fHitValue + m_StasisfieldData.m_fDamageValue;
				/// 시전 MP
				const float fMPConsume	= GetActualMPConsume( pSkillTemplet->m_eID, iSkillLevel );
				/// MP 회복량 = 시전 MP * ( 0.3333 + min( 쌓인 데미지/자신의 최대HP , 0.3333) )
				const float fIncreaseMPValue = fMPConsume * ( 0.3333f + min( fTotalValue / GetMaxHp(), 0.3333f ) );

				/// MP 회복
				UpNowMp( fIncreaseMPValue );

				/// MP 회복 수치 표시
				if ( NULL != g_pX2Game )
					g_pX2Game->DisplayAddMPInformation( fIncreaseMPValue, GetPos(), GetDirVector() );
			}
		}
	}

	/// 사운드 재생
	PlaySound( L"AddVoice_Stasis_Field03.ogg" );
	PlaySound( L"Add_StasisField2.ogg" );

	/// 스테이시스 필드 초기화
	m_StasisfieldData.ResetStasisfieldData();

	/// 스킬 아이콘 갱신
	if ( IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI();
}

/** @function	: ResetMagnetronData
	@brief		: 마그네트론 정보 초기화 함수
*/
void CX2GUAdd::MagnetronData::ResetMagnetronData()
{
	m_fFireReadyTime = 0.f;

	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
	{
		BOOST_FOREACH( CX2DamageEffect::CEffectHandle hMagnetronHandle, m_vecMagnetronDamageEffectData )
		{
			g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( hMagnetronHandle );
		}
	}
}

/** @function	: ResetStasisfieldData
	@brief		: 스테이시스 필드 정보 초기화 함수
*/
void CX2GUAdd::StasisfieldData::ResetStasisfieldData()
{
	m_fRemainTime			= 0.f;
	m_fHitValue				= 0.f;
	m_fDamageValue			= 0.f;

	if ( NULL != g_pX2Game &&
		 NULL != g_pX2Game->GetEffectSet() )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hLoopEffectSet );
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hCountEffectSet );
	}
}

//////////////////////////////////////////////////////////////////////////
// 위상변화 관리 데이터 함수
CX2GUAdd::PhaseShiftData::PhaseShiftData()
{
	m_fEffectLifeTime		= 0.f;
	m_fReturnMPConsume		= 0.f;
	m_vUserReturnPos		= D3DXVECTOR3( 0.f, 0.f, 0.f );
	m_hPhasShiftDamageEffect= INVALID_DAMAGE_EFFECT_HANDLE;
}
void CX2GUAdd::PhaseShiftData::InitPhaseEffectData()
{
	m_hPhasShiftDamageEffect = INVALID_DAMAGE_EFFECT_HANDLE;
}
void CX2GUAdd::PhaseShiftData::InitResturnPosValue()
{
	m_vUserReturnPos = D3DXVECTOR3(0.f,0.f,0.f);
}
/** @function : IsValidReturnPosValue
	@brief : 유저가 복귀할 위치값이 유효한지 검사
*/
bool CX2GUAdd::PhaseShiftData::IsValidReturnPosValue() const
{
	if( INVALID_DAMAGE_EFFECT_HANDLE != m_hPhasShiftDamageEffect ) 
	{
		const float fPosValue = m_vUserReturnPos.x + m_vUserReturnPos.y + m_vUserReturnPos.z;
		return !IsSamef(fPosValue,0.f);
	}
	else
		return false;
}
/** @function : UpdateSkillSlotUI
	@brief : 위상변화 스킬 슬롯 아이콘 변경 내용 갱신.
			 (호출 위치를 파악하기 쉽도록 함수 추가 함)
*/
void CX2GUAdd::PhaseShiftData::UpdateSkillSlotUI()
{
// 스킬 아이콘 변경 시점.
//#case1 설치 이펙트 설치 했을 때
//#case2 설치 이펙트가 지속시간에 의해 제거되었을 때
//#case3 유저가 설치 이펙트 위치로 이동 했을 때

	g_pX2Game->UpdateSkillSlotUI();
}
#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
/** @function : DieDamageEffectProc
	@brief : 데미지 이펙트로부터 불려지는 Callback 함수
			 데미지 이펙트를 생성한 유닛 객체에게만 불려 짐 
*/
void CX2GUAdd::DieDamageEffectProc( const CX2DamageManager::DamageData& pDamageData_ )
{
	switch( pDamageData_.m_eUsedSkillID )
	{
	case CX2SkillTree::SI_A_AN_PHASE_SHIFT:
		{ // 위상변화 대기 이펙트 사망 시 쿨타임 처리
			if( true == pDamageData_.m_bNoDamage )
			{
				// 관리중에 있는 이펙트가 있을 때만 처리
				if( INVALID_DAMAGE_EFFECT_HANDLE != m_PhaseShiftData.m_hPhasShiftDamageEffect )
				{
					m_PhaseShiftData.InitResturnPosValue();
					m_PhaseShiftData.InitPhaseEffectData();
					if ( IsMyUnit() )
						m_PhaseShiftData.UpdateSkillSlotUI();
				}
			}
		} break;
	default:
		break;
	}
}
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
void CX2GUAdd::PlayLoopSound( const WCHAR* pSoundFileName_, bool bLoop_, bool b3D_ )
{
	if( NULL != g_pKTDXApp && 
		NULL != g_pKTDXApp->GetDeviceManager() )
	{
		// TODO: 루프 사운드 처리 할 다른 방법을 찾아 봐야 할 듯 함.
		//		 현재는 루프 사운드가 중첩 될 수 없다고 가정하고 작업 함.
		StopLoopSound();

		m_pLoopSound = 
			g_pKTDXApp->GetDeviceManager()->PlaySound( pSoundFileName_, false, false );		
	}
}

/** @function : StopLoopSound
	@brief : 일부 Loop 스테이트의 사운드를 종료 시키는 함수.
			 해당 스테이트의 정상종료와, 피격, 유닛 소멸 시 처리한다.
*/
void CX2GUAdd::StopLoopSound()
{
	if( NULL != m_pLoopSound )
	{
		m_pLoopSound->Stop();
		m_pLoopSound = NULL;
	}
}

#ifdef FIX_NO_STATE_SKILL_BUG
/** @function : IsValideSkillRidingOn
	@brief : 탈 것 탑승중에 사용 할 수 있는 스킬인지 체크.
	@return : 사용 가능 : true 사용 불가 falle
*/
/*virtual*/ bool CX2GUAdd::IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ )
{
	if( true == GetRidingOn() )
	{
		switch( eSkill_ID_ ) 
		{
		case CX2SkillTree::SI_SA_APT_STASIS_FIELD: // 스테이시스 필드
			{
				return false;
			} break;
		default:
			return true;
			break;
		}
	}
	else
	{
		return true;
	}
}
#endif // FIX_NO_STATE_SKILL_BUG


/*virtual*/ void CX2GUAdd::ChargeMpAndEtcInTrainingGame()
{
	CX2GUUser::ChargeMpAndEtcInTrainingGame();

	CX2AddGageData* pAddGageData = static_cast<CX2AddGageData*>( GetGageData() );
	if( NULL != pAddGageData )
	{
		SetDPValue( pAddGageData->GetMaxDPValue() );
	}
}

void CX2GUAdd::RidingWaitEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( false == GetRidingOn() )
	{
		StateChange( USI_RIDING_OFF );
	}
	else if ( true == RidingPetSpecialAttackEventProcess() )
	{
	}
	else if ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if ( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_RIDING_WALK );
	}
	else if ( true == m_InputData.pureUp )	// 점프
	{
		StateChange( USI_RIDING_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( true == m_InputData.oneDown &&	
		true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
		&m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( CheckElapsedTimeRidingWaitHabit() &&
		NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && m_ptrRidingPet->GetXSkinAnimFuturePtr()->IsAnimationEnd() )
	{
		StateChange( USI_RIDING_WAIT_HABIT );
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_RIDING_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE( USI_RIDING_HYPER_MODE );

	CommonEventProcess();
}

void CX2GUAdd::RidingWalkEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( false == GetRidingOn() )
	{
		StateChange( USI_RIDING_OFF );
	}
	else if ( true == RidingPetSpecialAttackEventProcess() )
	{
		TRACE( L"Walk RidingPetSpecialAttackEventProcess" );
	}
	else if ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		TRACE( L"Walk m_InputData.pureDoubleRight m_InputData.pureDoubleLeft" );
		StateChangeDashIfPossible();
	}
	else if ( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		TRACE( L"Walk m_InputData.pureRight m_InputData.pureLeft" );
		StateChange( USI_RIDING_WAIT );

		TRACE( L"RidingWalkStart Wait\n" );
	}
	else if ( true == m_InputData.pureUp )
	{
		TRACE( L"Walk m_InputData.pureUp" );
		StateChange( USI_RIDING_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( true == m_InputData.oneDown &&
		true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
		&m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_RIDING_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE( USI_RIDING_HYPER_MODE );

	CommonEventProcess();
}

void CX2GUAdd::RidingJumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( false == GetRidingOn() )
	{
		StateChange( USI_RIDING_OFF );
	}
	else if( NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && m_ptrRidingPet->GetXSkinAnimFuturePtr()->IsAnimationEnd() )
	{
		StateChange( USI_RIDING_WAIT );
	}
	else if ( true == RidingPetSpecialAttackEventProcess() )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_RIDING_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_RIDING_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( true == m_InputData.oneDown &&
		true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
		&m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}	
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_RIDING_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE( USI_RIDING_HYPER_MODE );

	CommonEventProcess();
}

void CX2GUAdd::RidingDashEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		if ( NULL != m_ptrRidingPet && GetAirDashCount() < 1 )
		{
			if ( CX2RidingPet::RSM_AIR_DASH == m_ptrRidingPet->GetInfo().m_eSpecialMove || 
				CX2RidingPet::RSM_PASS_DASH == m_ptrRidingPet->GetInfo().m_eSpecialMove )
				StateChange( USI_RIDING_SPECIAL_MOVE );
		}
		else
		{
			StateChange( USI_RIDING_DASH_JUMP );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
	}
	else if ( false == GetRidingOn() )
	{
		StateChange( USI_RIDING_OFF );
	}
	else if ( true == RidingPetSpecialAttackEventProcess() )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( USI_RIDING_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	else if( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( USI_RIDING_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_InputData.pureLeft == true )
	{
		StateChange( USI_RIDING_DASH_END );
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		StateChange( USI_RIDING_WALK );
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		StateChange( USI_RIDING_WALK );
	}
	else if( true == m_InputData.oneDown && 
		true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
		&m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_RIDING_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}

	if ( NULL != m_ptrRidingPet && CX2RidingPet::RSM_BOOST_DASH == m_ptrRidingPet->GetInfo().m_eSpecialMove &&
		1.0f <= m_FrameDataFuture.unitCondition.fStateTime )
	{
		StateChange( USI_RIDING_SPECIAL_MOVE );
	}
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_RIDING_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE( USI_RIDING_HYPER_MODE );

	CommonEventProcess();
}

void CX2GUAdd::RidingDashEndEventProcess()
{
	const float TRIGGER_TIME_FACTOR_DASH_ATTACK = 0.3f;

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if ( false == GetRidingOn() )
	{
		StateChange( USI_RIDING_OFF );
	}
	else if ( true == RidingPetSpecialAttackEventProcess() )
	{
	}
	else if( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneRight && true == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.oneLeft && false == m_FrameDataFuture.syncData.bIsRight )
	{
		StateChangeDashIfPossible();
	}
	else if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_RIDING_WALK );
	}
	else if( true == m_InputData.oneDown &&
		true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
		&m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_RIDING_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_RIDING_JUMP_UP );
	}
	else if( NULL != m_ptrRidingPet && NULL != m_ptrRidingPet->GetXSkinAnimFuturePtr() && m_ptrRidingPet->GetXSkinAnimFuturePtr()->IsAnimationEnd() )
	{	
		StateChange( USI_RIDING_WAIT );
	}	
	ELSE_IF_CAN_CHANGE_FORMATION_MODE( USI_RIDING_HYPER_MODE )
	ELSE_IF_CAN_RELEASE_FORMATION_MODE( USI_RIDING_HYPER_MODE );

	CommonEventProcess();
}

void CX2GUAdd::InitPhaseShift()
{ 
	if( INVALID_DAMAGE_EFFECT_HANDLE != m_PhaseShiftData.m_hPhasShiftDamageEffect )
	{
		CX2DamageEffect::CEffect* pEffect = 
			g_pX2Game->GetDamageEffect()->GetInstance( m_PhaseShiftData.m_hPhasShiftDamageEffect );
		if( NULL != pEffect )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMesh = pEffect->GetMainEffect();
			if( NULL != pMesh )
				pMesh->SetMaxLifeTime(0.1f);

			pEffect->GetDamageData().m_eUsedSkillID = CX2SkillTree::SI_NONE;
			m_PhaseShiftData.InitPhaseEffectData();
		}
	}

	m_PhaseShiftData.InitPhaseEffectData(); 
	m_PhaseShiftData.InitResturnPosValue(); 
	m_PhaseShiftData.UpdateSkillSlotUI(); 
}

/** @function	: GetTargetUnit
	@brief		: 나소드 아머에 타겟팅 된 유닛 반환
	@return		: 타겟팅 유닛 객체
*/
const CX2GameUnit* CX2GUAdd::NasodArmorAttackTargetData::GetTargetUnit()
{
	const CX2GameUnit* pGameUnit = NULL;

	if ( 0 != m_uidTargetUID )
	{
		if ( true == m_bIsNPC )	/// NPC일 때
		{
			CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( static_cast<int>( m_uidTargetUID ) );

			if ( NULL != pNPC )
				pGameUnit = static_cast<CX2GameUnit*>( pNPC );		/// GameUnit 객체 반환
		}
		else					/// 유저일 때
		{
			CX2GUUser* pUser = g_pX2Game->GetUserUnitByUID( m_uidTargetUID );

			if ( NULL != pUser )
				pGameUnit = static_cast<CX2GameUnit*>( pUser );		/// GameUnit 객체 반환
		}
	}

	return pGameUnit;
}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환

/** @function	: ProcessBodyOfTranscendence
	@brief		: 초월자의 신체 공격 검사 및 수행 함수
	@param		: 데미지 데이터
*/
void CX2GUAdd::ProcessBodyOfTranscendence( IN OUT CX2DamageManager::DamageData* pDamageData_ )
{
	/// NULL 포인터면 패스
	if ( NULL == pDamageData_ )
		return;

	/// 나소드 아머 콤보가 아니면, 패스
	if ( CX2DamageManager::DPRT_NASOD_ARMOR != pDamageData_->m_DPRateType )
		return;

	/// 발동 확률 검사 ( 패시브를 찍지 않았다면, 확률은 0% )
	if ( m_fBodyOfTranscendenceActiveRate >= RandomFloat() )
	{
		pDamageData_->m_bIgnoreDefence		= true;	/// 방어력 무시
		pDamageData_->m_bIgnoreStateDefence	= true;	/// 가드 무시
	}
}

/** @function	: ProcessPowerExchanger
	@brief		: 출력 교환기 버프 설정 함수
*/
void CX2GUAdd::ProcessPowerExchanger()
{
	/// 출력 교환기 않찍었으면 패스
	if ( false == m_bEnablePowerExchanger )
		return;

	/// 현재 DP량에 따른 출력 교환기 스테이트 ( 현재 DP가 50% 이상인가? 미만인가? )
	const POWER_EXCHANGER_STATE eNowPowerExchangerState = ( GetMaxDPValue() * 0.5f <=  GetDPValue() ) ? PES_HALF_UP : PES_HALF_DOWN;

	/// 내가 설정되어 있는 출력 교환기 스테이트와 다를 때
	if ( m_ePowerExchangerState != eNowPowerExchangerState )
	{
		/// 출력 교환기 스테이트 갱신
		m_ePowerExchangerState = eNowPowerExchangerState;


		if ( NULL != GetUnit() )
		{
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
			const int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALP_POWER_EXCHANGER );

			if ( PES_HALF_UP == eNowPowerExchangerState )		/// 50% 이상일 때
			{
				/// 자체적으로 버프에서 BFI_BUFF_POWER_EXCHANGER_HALF_DOWN을 지우고 적용됨
				SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_POWER_EXCHANGER_HALF_UP, iSkillLevel );
			}
			else		/// 50% 미만일 때
			{
				/// 자체적으로 버프에서 BFI_BUFF_POWER_EXCHANGER_HALF_UP을 지우고 적용됨
				SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_POWER_EXCHANGER_HALF_DOWN, iSkillLevel );
			}
		}
	}
}

/** @function	: GetIsFantasyTracerTargetSkillID
	@brief		: 해당 스킬의 타겟 정보가 있는지 검사
	@param		: 스킬 아이디
	@return		: 타겟 정보 존재 여부
*/
const bool CX2GUAdd::GetIsFantasyTracerTargetSkillID( IN const CX2SkillTree::SKILL_ID eSkillID_ ) const
{
	/// 환상 추적자 적용시에만 검사
	if ( true == GetIsFormationMode() && true == m_bEnableFantasyTracer )
	{
		BOOST_FOREACH( FantasyTracerTargetInfo TargetInfo, m_vecFantasyTracerTargetInfo )
		{
			if ( eSkillID_ == TargetInfo.m_eSkillID && false == TargetInfo.m_bIsUseData )
				return true;
		}
	}

	return false;
}

/** @function	: SetUseFantasyTracerTarget
	@brief		: 해당 스킬 아이디의 타겟 정보 사용 처리 설정
	@param		: 스킬 아이디
*/
void CX2GUAdd::SetUseFantasyTracerTarget( IN const CX2SkillTree::SKILL_ID eSkillID_ )
{
	BOOST_FOREACH( FantasyTracerTargetInfo& TargetInfo, m_vecFantasyTracerTargetInfo )
	{
		if ( eSkillID_ == TargetInfo.m_eSkillID )
		{
			TargetInfo.m_bIsUseData				= true;						/// 사용 처리
			m_eNasodArmorAttackActiveSkillID	= eSkillID_;				/// 공격시 사용할 스킬 아이디 저장
			m_eNasodArmorLandingActiveSkillID	= CX2SkillTree::SI_NONE;	/// 착지시 사용할 스킬 아이디 초기화

			/// 스킬 아이콘 갱신
			if ( NULL != g_pX2Game )
				g_pX2Game->UpdateSkillSlotUI();
		}
	}
}

/** @function	: ProcessFantasyTracerTargetInfo
	@brief		: 환상 추적자 타겟 정보 갱신 함수
*/
void CX2GUAdd::ProcessFantasyTracerTargetInfo()
{
	vector<FantasyTracerTargetInfo>::iterator vIt = m_vecFantasyTracerTargetInfo.begin();

	while ( vIt != m_vecFantasyTracerTargetInfo.end() )
	{
		float& fRemainTime = vIt->m_fRemainTime;

		fRemainTime -= m_fElapsedTime;

		/// 삭제 유지시간 도달시, 해당 타겟 정보 사용 처리
		if ( _CONST_ADD_::FANTASY_TRACER_TARGET_DELETE_READY_TIME >= fRemainTime && false == vIt->m_bIsUseData )
		{
			vIt->m_bIsUseData = true;

			/// 스킬 아이콘 갱신
			if ( NULL != g_pX2Game )
				g_pX2Game->UpdateSkillSlotUI();
		}

		/// 시간 종료된 것은 지워주자
		if ( 0.f >= fRemainTime )
		{
			vIt = m_vecFantasyTracerTargetInfo.erase( vIt );

			/// 스킬 아이콘 갱신
			if ( NULL != g_pX2Game )
				g_pX2Game->UpdateSkillSlotUI();
		}
		else
			++vIt;
	}
}

/** @function	: SetDustAnatomyTarget
	@brief		: 더스트 아나토미 타겟 설정
	@param		: 데미지 데이터
*/
void CX2GUAdd::SetDustAnatomyTarget( IN const CX2DamageManager::DamageData& pDamageData_ )
{
	if ( null == pDamageData_.optrDefenderGameUnit || 
		 CX2SkillTree::SI_SA_ALP_DUST_ANATOMY != pDamageData_.m_eUsedSkillID )
		return;

	/// 범위 내 유닛들을 타겟 정보에 삽입
	if ( NULL != g_pX2Game && false == g_pX2Game->GetUnitList().empty() )
	{
		CX2GameUnit* pUnit = pDamageData_.optrDefenderGameUnit.GetObservable();

		if ( NULL != pUnit )
		{
			/// 타겟 방향에 따라 이펙트 붙일 손 위치 지정
			wstring wstrCreateBoneName = L"Bip01";

			if ( true == GetIsRight() )
				wstrCreateBoneName = IsRightTarget( pUnit->GetPos() ) ? L"Bip01_L_Finger2" : L"Bip01_R_Finger2";
			else
				wstrCreateBoneName = IsRightTarget( pUnit->GetPos() ) ? L"Bip01_R_Finger2" : L"Bip01_L_Finger2";

			bool bIsNPC = ( CX2GameUnit::GUT_NPC == pUnit->GetGameUnitType() ) ? true : false;

			DustAnatomyTargetInfo TargetInfo = DustAnatomyTargetInfo( INVALID_DAMAGE_EFFECT_HANDLE, pUnit->GetUnitUID(), bIsNPC, wstrCreateBoneName );

			m_vecDustAnatomyTargetInfo.push_back( TargetInfo );
		}
	}
}

/** @function	: GetTargetUnit
	@brief		: 더스트 아나토미 타겟 반환 함수
	@return		: 타겟 객체
*/
CX2GameUnit* CX2GUAdd::DustAnatomyTargetInfo::GetTargetUnit()
{
	if ( 0 < m_uidTargetUID && NULL != g_pX2Game )
	{
		BOOST_FOREACH( CX2GameUnit* pUnit, g_pX2Game->GetUnitList() )
		{
			if ( NULL == pUnit )
				continue;

			bool bIsNPC = ( CX2GameUnit::GUT_NPC == pUnit->GetGameUnitType() ) ? true : false;

			/// NPC 대상 검사
			if ( true == m_bIsNPC )
			{
				if ( true == bIsNPC && pUnit->GetUnitUID() == m_uidTargetUID )
					return pUnit;
			}
			/// User 대상 검사
			else
			{
				if ( false == bIsNPC && pUnit->GetUnitUID() == m_uidTargetUID )
					return pUnit;
			}
		}
	}

	return NULL;
}

/** @function	: GetTargetUnit
	@brief		: 리버스 리엑터 타겟 반환 함수
	@return		: 타겟 객체
*/
CX2GameUnit* CX2GUAdd::ReverseReactorTargetInfo::GetTargetUnit()
{
	if ( 0 < m_uidTargetUID && NULL != g_pX2Game )
	{
		BOOST_FOREACH( CX2GameUnit* pUnit, g_pX2Game->GetUnitList() )
		{
			if ( NULL == pUnit )
				continue;

			bool bIsNPC = ( CX2GameUnit::GUT_NPC == pUnit->GetGameUnitType() ) ? true : false;

			/// NPC 대상 검사
			if ( true == m_bIsNPC )
			{
				if ( true == bIsNPC && pUnit->GetUnitUID() == m_uidTargetUID )
					return pUnit;
			}
			/// User 대상 검사
			else
			{
				if ( false == bIsNPC && pUnit->GetUnitUID() == m_uidTargetUID )
					return pUnit;
			}
		}
	}

	return NULL;
}

/** @function	: ResetReverseReactorTargetInfo
	@brief		: 리버스 리액터 정보 객체 초기화 함수
*/
void CX2GUAdd::ReverseReactorTargetInfo::ResetReverseReactorTargetInfo()
{
	m_fMaxTransitionDamage	= 0.f;
	m_fCumulativeDamage		= 0.f;
	m_fRemainTime			= 0.f;
	m_fTransitionDamageRate = 0.f;

	CX2GameUnit* pTargetUnit = GetTargetUnit();

	if ( NULL != pTargetUnit )
		pTargetUnit->EraseBuffTempletFromGameUnit( BTI_DEBUFF_PLASMA_LINK );

	m_uidTargetUID = 0;

	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetDamageEffect() )
		g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_eLinkEffect );

	/// 이펙트 해제 사운드 출력
	if ( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDeviceManager() )
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Add_Reverse_Reactor_ConnectCancel.ogg" );
}

/** @function	: ProcessReverseReactorTargetInfo
	@brief		: 리버스 리액터 정보 갱신 함수
*/
void CX2GUAdd::ProcessReverseReactorTargetInfo()
{
	float& fRemainTime = m_ReverseReactorTargetInfo.m_fRemainTime;

	if ( 0.f < fRemainTime )
	{
		fRemainTime -= m_fElapsedTime;

		/// 유지 시간 만료시, 기능 해제
		if ( 0.f >= fRemainTime )
		{
			fRemainTime = 0.f;

			m_ReverseReactorTargetInfo.ResetReverseReactorTargetInfo();
		}
		/// 링크 이펙트 갱신
		else
		{
			CX2GameUnit* pUnit = m_ReverseReactorTargetInfo.GetTargetUnit();

			if ( NULL == pUnit || 0.f >= pUnit->GetNowHp() )
			{
				m_ReverseReactorTargetInfo.ResetReverseReactorTargetInfo();
				return;
			}
			
			D3DXVECTOR3	vMyPos	= GetBonePos( L"Bip01" );
			vMyPos.y			+= 25.f;

			D3DXVECTOR3	vEnemyPos;

			if ( false == pUnit->GetBonePos( L"Bip01", vEnemyPos ) )
			{
				vEnemyPos	=  pUnit->GetPos();
				vEnemyPos.y	+= 100.f;
			}
			else
			{
				vEnemyPos.y	+= 25.f;
			}

			/// 연결 범위 이상으로 멀어지면, 스킬 종료
			if ( _CONST_ADD_::REVERSE_REACTOR_LINK_RANGE < GetDistance( vMyPos, vEnemyPos ) )
				m_ReverseReactorTargetInfo.ResetReverseReactorTargetInfo();
			/// 이펙트 연결 갱신
			else
			{
				CX2DamageEffect::CEffect*	pEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_ReverseReactorTargetInfo.m_eLinkEffect );

				if ( NULL != pEffect )
					pEffect->SetLinkMainEffectByPos( vMyPos, vEnemyPos );
			}
		}
	}
}

/** @function	: SetReverseReactorTransitionDamage
	@brief		: 리버스 리액터 전이 데미지 처리 함수
	@param		: 피격 데미지
*/
/*virtual*/ void CX2GUAdd::SetReverseReactorTransitionDamage( IN const float fFinalDamage_ )
{
	if ( 0.f < m_ReverseReactorTargetInfo.m_fRemainTime )
	{
		CX2GameUnit* pUnit = m_ReverseReactorTargetInfo.GetTargetUnit();

		if ( NULL != g_pX2Game && NULL != pUnit && 0.f < pUnit->GetNowHp() )
		{
			float		fTransitionDamage		= m_ReverseReactorTargetInfo.m_fTransitionDamageRate * fFinalDamage_;	/// 전이 시킬 데미지
			float&		fCumulativeDamage		= m_ReverseReactorTargetInfo.m_fCumulativeDamage;		/// 누적 데미지
			const float fMaxTransitionDamage	= m_ReverseReactorTargetInfo.m_fMaxTransitionDamage;	/// 최대 허용 전이 데미지

			fCumulativeDamage += fTransitionDamage;		/// 데미지 누적

			/// 최대 허용 전이 데미지 수치를 넘어갈 수 없도록 조정
			if ( fMaxTransitionDamage < fCumulativeDamage )
			{
				float fOverDamage = fCumulativeDamage - fMaxTransitionDamage;

				fTransitionDamage -= fOverDamage;

				/// 기능 정지
				m_ReverseReactorTargetInfo.ResetReverseReactorTargetInfo();
			}

			/// 체력 감소 및 감소치 표기
			pUnit->UpNowHp( - fTransitionDamage );
			g_pX2Game->DisplayDamageInformation( fTransitionDamage, pUnit->GetHeadBonePos(), pUnit->GetDirVector() );
		}

	}
}

/** @function	: ProcessQuicksilverFrenzy
	@brief		: 퀵실버 프렌지 발사 간격 연산
	@param		: 
	@return		: 
*/
void CX2GUAdd::ProcessQuicksilverFrenzy()
{
	if ( false == m_QuicksilverFrenzyData.m_bEnableQuicksilverFrenzy )
	{
		m_eCustomRenderState = CRS_NONE;
		return;
	}
	else
	{
		m_eCustomRenderState = CRS_QUICKSILVER_FRENZY_BUFF;

		float& fFireIntervalTime = m_QuicksilverFrenzyData.m_fFireIntervalTime;

		if ( 0.f < fFireIntervalTime )
		{
			fFireIntervalTime -= m_fElapsedTime;

			if ( 0.f >= fFireIntervalTime )
				fFireIntervalTime = 0.f;
		}
	}
}

/** @function	: AttackQuicksilverFrenzy
	@brief		: 피격 혹은 타격시 퀵실버 프렌지 발사체 생성 및 록온
	@param		: 데미지 정보
*/
void CX2GUAdd::AttackQuicksilverFrenzy( IN const CX2DamageManager::DamageData& pDamageData_, IN const bool bIsAttack_ /*= false*/ )
{
	/// 버프 미적용 이거나 특수 공격 혹은 발사 간격이 남아있으면, 발사체 생성 패스
	if ( false == m_QuicksilverFrenzyData.m_bEnableQuicksilverFrenzy ||
		 CX2DamageManager::AT_SPECIAL == pDamageData_.attackType ||
		 0.f < m_QuicksilverFrenzyData.m_fFireIntervalTime )
		return;

	/// 각 상황에 따른 타겟 대상 설정
	CX2GameUnit* pTargetUnit = ( true == bIsAttack_ ) ? pDamageData_.optrDefenderGameUnit.GetObservable() : pDamageData_.optrAttackerGameUnit.GetObservable();

	if ( NULL != pTargetUnit )
	{
		D3DXVECTOR3 vRotateDegree = GetRotateDegree();

		/// 발사체 생성
		CX2DamageEffect::CEffect* pEffect = 
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_ALP_Quicksilver_Frenzy_Projectile", m_QuicksilverFrenzyData.m_fPowerRate, 
														  GetBonePos( L"Bip01" ), GetRotateDegree(), GetRotateDegree(), 
														  m_FrameDataNow.unitCondition.landPosition.y );
		/// 록온 타겟 설정
		if ( NULL != pEffect )
		{
			CX2DamageEffect::LockOnData* pLockOnData = &pEffect->GetLockOnData();

			if( CX2GameUnit::GUT_USER == pTargetUnit->GetGameUnitType() )
				pLockOnData->m_LockOnUnitUID = pTargetUnit->GetUnitUID();
			else
				pLockOnData->m_LockOnNPCUID = static_cast<int>( pTargetUnit->GetUnitUID() );
		}

		///발사 간격 설정
		m_QuicksilverFrenzyData.m_fFireIntervalTime = _CONST_ADD_::QUICKSILVER_FRENZY_FIRE_INTERVAL_TIME;
	}
}

/** @function	: SetQuicksilverFrenzyFlickerColor
	@brief		: 퀵실버 프렌지 버프 적용 효과 컬러값 설정
	@param		: 변경 시킬 컬러값
*/
const D3DXCOLOR CX2GUAdd::SetQuicksilverFrenzyFlickerColor( IN const D3DXCOLOR& coRenderColor_ )
{
	const float fFlickerGap = _CONST_ADD_::QUICKSILVER_FRENZY_FLICKER_INTERVAL_TIME;	/// 반짝이는 간격

	D3DXCOLOR coResultColor( 1.f, 1.f, 1.f, 1.f );

	/// 반짝이는 시간 갱신
	m_fQuicksilverFrenzyFlickerTime += m_fElapsedTime;

	if ( m_fQuicksilverFrenzyFlickerTime >= fFlickerGap )
		m_fQuicksilverFrenzyFlickerTime = 0.f;

	/// 반짝이는 간격에 대한 진행률
	float fFlickerRate = m_fQuicksilverFrenzyFlickerTime / fFlickerGap;

	/// 진행률이 절반 이상이면, 색상 감소
	if ( ( fFlickerGap / 2 ) < m_fQuicksilverFrenzyFlickerTime )
	{
		fFlickerRate = ( fFlickerRate - 0.5f ) * 2.f;	/// 0 ~ 1.f 로 진행률 변환

		coResultColor.r = max( 1.f - ( ( 1.f - coRenderColor_.r ) * fFlickerRate ), coRenderColor_.r );
		coResultColor.g = max( 1.f - ( ( 1.f - coRenderColor_.g ) * fFlickerRate ), coRenderColor_.g );
		coResultColor.b = max( 1.f - ( ( 1.f - coRenderColor_.b ) * fFlickerRate ), coRenderColor_.b );
	}
	///  진행률이 절반 이하면, 색상 증가
	else
	{
		fFlickerRate *= 2.f;	/// 0 ~ 1.f 로 진행률 변환

		coResultColor.r = min( coRenderColor_.r + ( ( 1.f - coRenderColor_.r ) * fFlickerRate ), 1.f );
		coResultColor.g = min( coRenderColor_.g + ( ( 1.f - coRenderColor_.g ) * fFlickerRate ), 1.f );
		coResultColor.b = min( coRenderColor_.b + ( ( 1.f - coRenderColor_.b ) * fFlickerRate ), 1.f );
	}

	return coResultColor;
}

/** @function	: CheckSkillSpecialUse
	@brief		: 특수한 스킬에 대하여, 사용 가능 여부를 검사
	@param		: 스킬 아이디
	@return		: 스킬 사용 가능 여부
*/
const bool CX2GUAdd::CheckSkillSpecialUse( IN const CX2SkillTree::SKILL_ID eSkillID_  )
{
	/// 퀵실버 액셀 후진 시전 가능한 시간중에 다른 스킬을 사용하려 하면, 못쓰게 막자.
	if ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == GetFutureStateSkillID() )
	{
		if ( ANSI_SA_ALP_QUICKSILVER_ACCEL_BACK != GetFutureStateID() )
		{
			if ( m_fEventTime[1].keyInputStart < m_pXSkinAnimFuture->GetNowAnimationTime() &&
				m_fEventTime[1].keyInputEnd >= m_pXSkinAnimFuture->GetNowAnimationTime() )
			{
				return  ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == eSkillID_ ) ? true : false;
			}
			else
			{
				return  ( CX2SkillTree::SI_SA_ALP_QUICKSILVER_ACCEL == eSkillID_ ) ? false :  true;
			}
		}
			return false;
	}

	return true;
}

/** @function	: ResetPsychicStormData
	@brief		: 사이킥 스톰 객체 초기화 함수
*/
void CX2GUAdd::PsychicStormData::ResetPsychicStormData()
{
	m_fIncreaseSize		= 0.f;
	m_fLoopTime			= 0.f;
	m_fFireGap			= 0.f;

	if ( NULL != g_pX2Game && NULL != g_pX2Game->GetEffectSet() )
		g_pX2Game->GetEffectSet()->DestroyInstanceHandle( eLoopEffectHandle );
}

#endif //SERV_ADD_LUNATIC_PSYKER

#endif //SERV_9TH_NEW_CHARACTER
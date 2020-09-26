#include "StdAfx.h"
#include ".\x2guraven.h"

#ifdef RAVEN_WEAPON_TAKER



namespace _CONST_RAVEN_
{
	const float OVERHEAT_HP_LIMIT = 0.25f;
	const float MAGIC_NUMBER_MULTIPLY_LINE_RADIUS = 1.5f;
	const float DEFAULT_OVERHEAT_HP_PENALTY = 0.2f;
	const float OVERHEAT_DAMAGE_BUFF_RATE = 1.1f;

	const int OVERHEAT_KEY_PRESS_TIME = 5;


	// XXX루프콤보 1~11
	const int MIN_COMBO_XXX_LOOP_COUNT = 1;
	const int MAX_COMBO_XXX_LOOP_COUNT = 11;

	const int MIN_REVOLVER_CANNON_LOOP_COUNT = 4;
	const int MAX_REVOLVER_CANNON_LOOP_COUNT = 6;

	const int MIN_HELLFIRE_GATLING_LOOP_COUNT = 17;
	const int MAX_HELLFIRE_GATLING_LOOP_COUNT = 27;

	const int MAX_GIGA_PROMINENCE_LOOP_COUNT = 8;

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	/// Dash Jump XXX 콤보 카운트
	const int MAX_DASH_JUMP_COMBO_XX_LOOP_COUNT = 9;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	const int MAX_DASH_COMBO_ZZZPushZ_LOOP_COUNT = 20;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	const int MAX_DASH_COMBO_ZZZPushZ_LOOP_COUNT = 10;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	/// 섀도우 스텝 최대 동작 횟수
	const int MAX_ACTIVE_SHADOW_STEP = 2;

	/// 데들리 레이드 오버 히트 입력 수
	const int DEADLY_RAID_OVERHEAT_INPUT_NUM_1 = 3;
	const int DEADLY_RAID_OVERHEAT_INPUT_NUM_2 = 5;
	const int DEADLY_RAID_OVERHEAT_INPUT_NUM_3 = 7;

	/// 하푼 스피어 오버 히트 입력 횟수
	const int HARPOON_SPEAR_OVERHEAT_KEY_PRESS_TIME = 4;
#endif SERV_RAVEN_VETERAN_COMMANDER
}

#endif  RAVEN_WEAPON_TAKER



#ifdef SERV_APRIL_FOOLS_DAY
static const CX2GUUser::SkillCutIn s_SkillCutInSetFoolsDay = CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Gura.dds");
#endif //APRIL_FOOLS_DAY

// 필살기 쓸 때 cut in image 
static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] = 
{
	// 레이븐, 소드테이커, 오버테이커
	{		
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_1.dds"),// D3DXVECTOR2(654.f, 654.f),	D3DXVECTOR3(207.f, 442.f, 0.f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_2.dds"),// D3DXVECTOR2(570.f, 570.0f), D3DXVECTOR3(228.f, 484.f, 0.f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_3.dds"),// D3DXVECTOR2(712.f, 712.f),	D3DXVECTOR3(217.f, 429.f, 0.f) ),
	},

#ifdef RAVEN_SECOND_CLASS_CHANGE
	// 블레이드 마스터
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Blade_Master_1.dds"),// D3DXVECTOR2( 656.f, 656.f ), D3DXVECTOR3( 208.f, 444.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Blade_Master_1.dds"),// D3DXVECTOR2( 656.f, 656.f ), D3DXVECTOR3( 208.f, 444.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Blade_Master_1.dds"),// D3DXVECTOR2( 656.f, 656.f ), D3DXVECTOR3( 208.f, 444.f, 0.f ) ),
	},

	// 레크리스 피스트
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Reckless_Fist_1.dds"),// D3DXVECTOR2( 661.f, 661.f ), D3DXVECTOR3( 290.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Reckless_Fist_1.dds"),// D3DXVECTOR2( 661.f, 661.f ), D3DXVECTOR3( 290.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Reckless_Fist_1.dds"),// D3DXVECTOR2( 661.f, 661.f ), D3DXVECTOR3( 290.f, 452.f, 0.f ) ),
	},
#endif

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	// 베테랑 커맨더
		{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Veteran_Commander_1.dds"),// D3DXVECTOR2( 661.f, 661.f ), D3DXVECTOR3( 330.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Veteran_Commander_1.dds"),// D3DXVECTOR2( 661.f, 661.f ), D3DXVECTOR3( 330.f, 452.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Raven_Veteran_Commander_1.dds"),// D3DXVECTOR2( 661.f, 661.f ), D3DXVECTOR3( 330.f, 452.f, 0.f ) ),
	},
#endif SERV_RAVEN_VETERAN_COMMANDER
};

// dmlee 2008.09.15
// 레이븐 소드테이커, 오버테이커의 2필인 아크에너미와 플라잉임팩트의 이름이 바뀌어서 코드와 필살기 이름이 서로 맞지 않습니다. 주의해주세요. ^^
// XXfrontX, XXdownXdownXdownX도 입력키가 바뀌어서 실제 입력과 상태 이름이 다릅니다. 주의해 주세요. ^^


CX2GURaven::CX2GURaven( int unitIndex, int teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const WCHAR* pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD 
	int frameBufferNum, CX2Unit* pUnit )
: CX2GUUser( unitIndex, teamNum, 
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	pScriptFileName, 
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD 
frameBufferNum, pUnit ) 
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
, 	m_bEnableShadowStepThrustEnhance ( false ),			// 쉐도우 스러스트 사용 가능 여부
	m_fAttackPowerRateShadowStepThrustEnhance ( 1.f ),	// 쉐도우 스러스트 공격력
	m_fChanceOfExplosiveNasodHand ( .1f ),				// 폭발적인 나소드 핸드 패시브 발동 확률, 레피만 가져야함
	m_bEnableShadowStepPunisher ( false ),				// 레피용 쉐도우 퍼니셔 사용 가능 여부
	m_fAttackPowerRateShadowStepPunisher ( 1.f ),		// 레피용 쉐도우 퍼니셔 공격력
	m_fOverheatDamageLimitedRate ( 0.f ),				// 베커용 오버히트 데미지 제한 퍼센테이지 ( 용병의 승리전략 ) 
	m_fAddOverHeatBuffDurationTime ( 0.f ),				// 베커용 오버히트 지속시간 증가 ( 과열모드 작동! )
	m_iAddOverHeatLevel ( 0 )							// 베커용 오버히트 레벨 설정	 ( 과열모드 작동! ) 
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
#ifdef FINALITY_SKILL_SYSTEM //김창한
	, m_fExtremBladeDelay( 0.001f )
#endif //FINALITY_SKILL_SYSTEM
{

	m_bReAttackZ1Right					= false;
	m_fReAttackZ1Time					= 0.0f;
	m_fReDashJumpXTime					= 0.0f;
	m_bZZZX								= false;

#ifdef OLD_HAND_SLASH_TRACE
//{{AFX
	m_pHandSlashTrace					= NULL;
	m_pHandSlashTraceTip				= NULL;
//}}AFX
#endif OLD_HAND_SLASH_TRACE

	m_hSeqCannonBlade		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMagnumBlaster1	= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hMesh_MagnumBlaster2	= INVALID_MESH_INSTANCE_HANDLE;




	InitializeRavenMajorParticleArray();
	InitializeRavenMinorParticleArray();

	m_hSeqBloodyAccel						= INVALID_PARTICLE_SEQUENCE_HANDLE;

	for( int i=0; i<ARRAY_SIZE( m_hSeqPowerAssault ); i++ )
	{
		m_hSeqPowerAssault[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}

	for( int i=0; i<ARRAY_SIZE( m_hSeqSevenBurst ); i++ )
	{
		m_hSeqSevenBurst[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
	

	m_hSeqSonicStab						= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqSonicStab1					= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hMeshFlyingImpact					= INVALID_MESH_INSTANCE_HANDLE;

	m_hMeshBerserkerBlade				= INVALID_MESH_INSTANCE_HANDLE;
	m_hMeshBerserkerBlade1				= INVALID_MESH_INSTANCE_HANDLE;
	m_hMeshBerserkerBlade2				= INVALID_MESH_INSTANCE_HANDLE;
	m_hMeshBerserkerBlade3				= INVALID_MESH_INSTANCE_HANDLE;

	m_hMeshGuardianStrike1				= INVALID_MESH_INSTANCE_HANDLE;

	m_hSeqGuardianStrike1				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqGuardianStrike2				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqGuardianStrike3				= INVALID_PARTICLE_SEQUENCE_HANDLE;


	m_iSuccessiveHit_OneZ				= 0;


	m_fShadowStepTimeLeft = 0.f;
	m_bEnableShadowStepThrust = false;
	m_fAttackPowerRateShadowStepThrust = 1.f;
	m_fMPConsumeShadowStepThrust = 0.f;


#ifdef RAVEN_SECOND_CLASS_CHANGE
	m_iBlackHoleHitCount = 0;
	m_fBlackHoleTime = 0.f;
	m_hBlackHoleEffectSet = INVALID_EFFECTSET_HANDLE;

	m_eDamageTypeRaven = CX2DamageManager::DT_PHYSIC;
	m_fChangeDamageRelRaven = 1.f;
	m_fPassiveCriticalRateRaven = 0.f;
#endif

#ifdef UPGRADE_RAVEN
	m_bLockOnNpc = false;
	m_iLockOnUid = -1;

	m_bCanParrying = false;
	m_bParrying = false;
	m_fParryingTime = 0.f;
	m_bAttackNasodBall = false;
	m_bDelayAttackNasodBall = false;
	m_fDelayAttackTime = 0.f;
	m_iNaosdBallLv = 0;
	m_fRageGauge = 0.f;
	m_fNasodBallAttackTime = 0.f;
	m_pNasodBall = NULL;
	m_bHyperAura = false;
	m_hHyperAuraEffectset = INVALID_EFFECTSET_HANDLE;
	m_hSeqHyperBall = INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifndef FIX_AFTER_IMAGE
	m_pAfterImageNasodBall = NULL;
#endif
#endif
#ifdef SPECIAL_USE_ITEM
	m_fThrowStartTime		= 0.8f;
	m_vThrowStartPosOffset	= D3DXVECTOR3(40.f, 130.f, 0.f);
	m_vThrowStartVelocity	= D3DXVECTOR3(1600.f, 200.f, 0.f);
#endif SPECIAL_USE_ITEM

#ifdef RAVEN_WEAPON_TAKER
	m_iRWTMaxComboLoopCount = 0;
	m_iRWTComboLoopCount = 0;
	m_iOverheatKeyPressTime = 0;
	m_bOverheated = false;
	m_bShowedOverheat = false;
	m_fAirSlicerDamageRel = 0.f;
	m_fOverheatDamageRate = 1.0f;
	m_fReduceEDTDurationRate = 1.0f;
	m_fOverheatBuffTime = 0.f;

	m_hBurstingBladeAttackBox = INVALID_MESH_INSTANCE_HANDLE;
#endif RAVEN_WEAPON_TAKER

#ifdef BALANCE_PATCH_20120329
	m_hEffectDashJumpComboX	= INVALID_EFFECTSET_HANDLE;
#endif
#ifdef BALANCE_PATCH_20110303
	m_fArcEnemyEachTime = 0.f;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	m_iRVCComboLoopCount		= 0;							/// 연발 콤보 반복 횟수
	m_iRVCComboLoopInputCount	= 0;							/// 연발 콤보 반복 입력 횟수

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectFrameThrow			= INVALID_DAMAGE_EFFECT_HANDLE;							/// 대시 zzzz용 화염 방사 동작 정지 여부 조사 방사 이펙트
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectFrameThrow			= NULL;							/// 대시 zzzz용 화염 방사 동작 정지 여부 조사 방사 이펙트
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_bEndFrameThrow			= false;						/// 화염 방사 동작 정지 여부 조사

	m_bEnableShadowBackSlide	= false;						/// 섀도우 백 슬라이드 패시브 적용 여부
	m_fShadowStepCooTime		= 0.f;							/// 섀도우 스텝 쿨타임
	m_bActiveShadowStepCombo	= false;						/// 섀도우 스텝 사용중 여부
	m_bIsShadowStepIcon			= true;							/// 섀도우 스텝 아이콘 적용중 여부
	m_iShadowBackSlideLevel		= 0;							/// 섀도우 백 슬라이드 스킬 레벨
	m_fShadowBusterPowerRate	= 0.5f;							/// 섀도우 버스터 파워레이트
	m_fShadowBackSlideTimeLeft	= 0.f;							/// 섀도우 백 슬라이드 유지 시간
	m_fShadowStepFinishTimeLeft	= 0.f;							/// 마지막 섀도우 스텝 유지 시간

	m_bEnableSurvivalTechniqueOfMercenary = false;				/// 용병의 생존술 패시브 적용 여부
	m_bActiveSurvivalTechniqueOfMercenary = false;				/// 용병의 생존술 발생 여부
	
	m_fLimitOverHeatHP			= 0.1f;							/// 오버 히트 가능한 HP 제한치


	m_bShowedOverheat1			= false;						/// 단계적 오버히트용 이모티콘 생성 확인 객체
	m_bShowedOverheat2			= false;						/// 단계적 오버히트용 이모티콘 생성 확인 객체
	m_hSeqOverHeatIcon			= INVALID_PARTICLE_SEQUENCE_HANDLE;		/// 단계적 오버히트용 이모티콘 핸들
	m_hSeqOverHeatOverlapIcon	= INVALID_PARTICLE_SEQUENCE_HANDLE;		/// 단계적 오버히트 중첩 이모티콘 핸들

	m_fEDTBleedingTime			= 0.f;							/// 치명상 유지 시간

	m_bIsHyperDeadlyRaid		= false;						/// 현재 데들리 레이드 각성 여부

	m_fDamageDataChangeTime		= 0.f;
	m_fDamageDataChangeTime1	= 0.f;
	m_fDamageDataChangeTime2	= 0.f;

	m_hOverHeatObject			= INVALID_MESH_INSTANCE_HANDLE;	/// 오버 히트 발동시 생성되는 구조물

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_hEffectOverHeatFire1		= INVALID_DAMAGE_EFFECT_HANDLE;							/// 오버 히트 발동시 생성되는 화염
	m_hEffectOverHeatFire2		= INVALID_DAMAGE_EFFECT_HANDLE;							/// 오버 히트 발동시 생성되는 화염
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectOverHeatFire1		= NULL;							/// 오버 히트 발동시 생성되는 화염
	m_pEffectOverHeatFire2		= NULL;							/// 오버 히트 발동시 생성되는 화염
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#endif SERV_RAVEN_VETERAN_COMMANDER

	m_fOldHp = 0.0f;

#ifdef BALANCE_BLADE_MASTER_20130117
	m_eRavenCurrentStage		= RSI_BASE;		/// 블레이드 마스터 ZZZXZZZ 콤보 캔슬시 클라간 동기화를 맞추기 위해, 캔슬시 스테이트를 저장
#endif BALANCE_BLADE_MASTER_20130117
}

CX2GURaven::~CX2GURaven(void)
{
	ClearArcEnemyData();
#ifdef UPGRADE_RAVEN
	ClearNasodBall();
	m_fRageGauge = 0.f;
	SetFullHyperMode( false );
	if( m_hHyperAuraEffectset != INVALID_EFFECTSET_HANDLE )
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hHyperAuraEffectset );
	if( m_hSeqHyperBall != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle(m_hSeqHyperBall);
	}
#endif

#ifdef BALANCE_PATCH_20120329
	if( m_hEffectDashJumpComboX != INVALID_EFFECTSET_HANDLE )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectDashJumpComboX );
	}
#endif

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqSonicStab );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqSonicStab1 );

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqGuardianStrike1 );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqGuardianStrike2 );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqGuardianStrike3 );


#ifdef OLD_HAND_SLASH_TRACE
//{{AFX
	SAFE_DELETE_KTDGOBJECT( m_pHandSlashTrace );
	SAFE_DELETE_KTDGOBJECT( m_pHandSlashTraceTip );

//}}AFX
#endif OLD_HAND_SLASH_TRACE

	DeleteRavenMajorParticle();
	DeleteRavenMinorParticle();

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqBloodyAccel );	

	for( int i=0; i<3; i++ )
	{
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqPowerAssault[i] );
	}

	for( int i=0; i<3; i++ )
	{
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqSevenBurst[i] );
	}

#ifdef RAVEN_SECOND_CLASS_CHANGE
	m_iBlackHoleHitCount = 0;
	m_fBlackHoleTime = 0.f;
	if( m_hBlackHoleEffectSet != INVALID_EFFECTSET_HANDLE )
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hBlackHoleEffectSet );
#endif

#ifdef RAVEN_WEAPON_TAKER
	g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hBurstingBladeAttackBox );
#endif RAVEN_WEAPON_TAKER

	m_fOldHp = 0.0f;
	
#ifdef SERV_RAVEN_VETERAN_COMMANDER
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffectFrameThrow			= INVALID_DAMAGE_EFFECT_HANDLE;	
	m_hEffectOverHeatFire1 = INVALID_DAMAGE_EFFECT_HANDLE;	
	m_hEffectOverHeatFire2 = INVALID_DAMAGE_EFFECT_HANDLE;	
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffectFrameThrow			= NULL;
	m_pEffectOverHeatFire1 = NULL;
	m_pEffectOverHeatFire2 = NULL;

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hOverHeatObject );

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqOverHeatIcon );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqOverHeatOverlapIcon );
#endif SERV_RAVEN_VETERAN_COMMANDER
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GURaven::AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GURaven::ReInit( bool bRandom, int startPosIndex )
{
	CX2GUUser::ReInit( bRandom, startPosIndex );
	m_fReDashJumpXTime		= 0.0f;

#ifdef UPGRADE_RAVEN
	m_bLockOnNpc = false;
	m_iLockOnUid = -1;

	m_bParrying = false;
	m_bCanParrying = false;
	m_fParryingTime = 0.f;
	m_bAttackNasodBall = false;
	m_bDelayAttackNasodBall = false;
	m_fDelayAttackTime = 0.f;
	m_iNaosdBallLv = 0;
	m_fRageGauge = 0.f;
	m_fNasodBallAttackTime = 0.f;
	
	if( IsMyUnit() )
	{
		m_FrameDataFuture.syncData.m_CannonBallCount = GetNasodBall();
	}
#endif


}

void CX2GURaven::DamageReact( CX2DamageManager::DamageData* pDamageData )
{
	KTDXPROFILE();
	
#ifdef RAVEN_WEAPON_TAKER
	// 상처, 출혈(미구현), 블라인드, 연무블라인드, 스턴, 혼란, 공황, 프레스, 동작둔화, 이동둔화 지속시간 감소
	CX2DamageManager::EXTRA_DAMAGE_TYPE& eEDT = pDamageData->m_ExtraDamage.m_ExtraDamageType;
	if( eEDT == CX2DamageManager::EDT_LEG_WOUND ||
#ifdef SERV_RAVEN_VETERAN_COMMANDER
		eEDT == CX2DamageManager::EDT_BLEEDING ||
#endif SERV_RAVEN_VETERAN_COMMANDER
		eEDT == CX2DamageManager::EDT_BLIND ||
		eEDT == CX2DamageManager::EDT_SMOKE_BLIND ||
		eEDT == CX2DamageManager::EDT_STUN ||
		eEDT == CX2DamageManager::EDT_REVERSE_LEFT_RIGHT ||
		eEDT == CX2DamageManager::EDT_PANIC ||
		eEDT == CX2DamageManager::EDT_Y_PRESSED ||
		eEDT == CX2DamageManager::EDT_DULL ||
		eEDT == CX2DamageManager::EDT_LEG_SHOT )
	{
		pDamageData->m_ExtraDamage.m_fTime *= m_fReduceEDTDurationRate;
	}
#endif RAVEN_WEAPON_TAKER


	CX2GUUser::DamageReact( pDamageData );
	CX2GUUser::DamageReactStateChange( pDamageData );

	if( IsDamagedReaction(pDamageData) == true &&
		pDamageData->reActResult != CX2DamageManager::RT_DUMMY_DAMAGE  // 이브 더스트오라에 맞을경우 zzzx콤보 상태이상 수정
	#ifdef DUNGEON_ITEM
			&& pDamageData->m_fHpPercentUp <= 0.f 
			&& pDamageData->m_fMpPercentUp <= 0.f 
	#endif
			)
		{
			m_fBlackHoleTime = 0.f;
			m_iBlackHoleHitCount = 0;
#ifdef BALANCE_BLADE_MASTER_20130117
			m_eRavenCurrentStage = RBM_COMBO_ZZZXZ;		/// 블레이드 마스터 ZZZXZZZ 콤보 캔슬시 클라간 동기화를 맞추기 위해, 캔슬시 스테이트를 저장
#endif BALANCE_BLADE_MASTER_20130117
		}
}

/*virtual*/ void CX2GURaven::AttackResultPlayEffect()
{
	CX2GUUser::AttackResultPlayEffect();

#ifdef BALANCE_PATCH_20110303
	if( m_fArcEnemyEachTime > 0.f )
		return;
#endif

	if( CX2DamageManager::RT_DUMMY_DAMAGE != m_DamageData.reActResult )
	{
		if( true == m_vecArcEnemyData.empty() )
			return;

		ArcEnemyData* pData = m_vecArcEnemyData[0];
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( pData == NULL )
        {
            ClearArcEnemyData();
            return;
        }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	
		/// 아크 에너미 데이터 널처리 추가
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( pData->m_hEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( pData->m_pEffect ) ? pData->m_pEffect : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGXMeshPlayer::CXMeshInstance* pMainEffect = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL;
		if( pMainEffect != NULL ) 
		{
			if( CX2DamageManager::AT_UNIT == m_DamageData.defenderType )
			{
				if( null != m_DamageData.optrDefenderGameUnit )
				{
					CX2DamageEffect::LockOnData* pLockOnData = &pEffect->GetLockOnData();
					if( CX2GameUnit::GUT_USER == m_DamageData.optrDefenderGameUnit->GetGameUnitType() )
					{
						pLockOnData->m_LockOnUnitUID = m_DamageData.optrDefenderGameUnit->GetUnitUID();
					}
					else if( CX2GameUnit::GUT_NPC == m_DamageData.optrDefenderGameUnit->GetGameUnitType() )
					{
						pLockOnData->m_LockOnNPCUID = static_cast<int>( m_DamageData.optrDefenderGameUnit->GetUnitUID() );
					}
						
					D3DXVECTOR3 vDirVec = GetDirVector();
					if( true == GetIsRight() )
						vDirVec *= -1;
					
					pMainEffect->SetPos( pMainEffect->GetPos() + vDirVec * 150.f + D3DXVECTOR3( 0, 0, 0 ) );
					pEffect->SetAttackTime( D3DXVECTOR2( 0, 9999 ) );
					pMainEffect->SetDirSpeed( 3000.f );
					pMainEffect->SetNowLifeTime( 0.f );	
					pMainEffect->SetMaxLifeTime( 0.35f );
					pEffect->SetLandPos( 0.f );

					PlaySound( L"Raven_OT_SP2_FlyingImpact3.ogg" );

					// kimhc // 2010-06-21 // 메모리해제 없이 erase 하는 부분에 메모리 해제 추가
					pData->m_hEffect = INVALID_DAMAGE_EFFECT_HANDLE; // m_pEffect는 데미지이펙트에서 자동으로 메모리 해제 해줌
					SAFE_DELETE( pData );
					m_vecArcEnemyData.erase( m_vecArcEnemyData.begin() );

#ifdef BALANCE_PATCH_20110303
					m_fArcEnemyEachTime = 0.3f;
#endif
				}
			}
		}
		else
		{
			//{{ kimhc // 2010.6.21 // 메모리해제 없이 erase 하는 부분에 메모리 해제 추가
			ClearArcEnemyData();
			//}} kimhc // 2010.6.21 // 메모리해제 없이 erase 하는 부분에 메모리 해제 추가
				
		}
	}
}

void CX2GURaven::Win()
{
	CX2GUUser::Win();

#ifdef RIDING_SYSTEM
	if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
		StateChange( USI_RIDING_WAIT_HABIT );
	else
		StateChange( USI_WIN );
#else
	StateChange( USI_WIN );
#endif //RIDING_SYSTEM
}

void CX2GURaven::Lose()
{
	CX2GUUser::Lose();

	if( IsMyUnit() == true && 
		GetNowHp() > 0.f )
	{
#ifdef RIDING_SYSTEM
		if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
			StateChange( USI_RIDING_WAIT );
		else
			StateChange( USI_LOSE );
#else
		StateChange( USI_LOSE );
#endif //RIDING_SYSTEM
	}
}

void CX2GURaven::Draw()
{
	CX2GUUser::Draw();

#ifdef RIDING_SYSTEM
	if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
		StateChange( USI_RIDING_WAIT_HABIT );
	else
		StateChange( USI_WIN );
#else
	StateChange( USI_WIN );
#endif //RIDING_SYSTEM
}



void CX2GURaven::ParseCommonRandomState()
{
	if( true == m_LuaManager.BeginTable( "INIT_COMMON_RANDOM_STATE" ) )
	{

		const char* tableName = "";
		switch( GetUnit()->GetClass() )
		{
		case CX2Unit::UC_RAVEN_FIGHTER:
			{
				tableName = "RAVEN_FIGHTER";
			} break;

		case CX2Unit::UC_RAVEN_SOUL_TAKER:
			{
				tableName = "RAVEN_SOUL_TAKER";
			} break;


		case CX2Unit::UC_RAVEN_OVER_TAKER:
			{
				tableName = "RAVEN_OVER_TAKER";
			} break;

#ifdef RAVEN_WEAPON_TAKER
		case CX2Unit::UC_RAVEN_WEAPON_TAKER:
			{
				tableName = "RAVEN_WEAPON_TAKER";
			} break;
#endif RAVEN_WEAPON_TAKER

#ifdef RAVEN_SECOND_CLASS_CHANGE
		case CX2Unit::UC_RAVEN_BLADE_MASTER:
			{
				tableName = "RAVEN_BLADE_MASTER";
			} break;


		case CX2Unit::UC_RAVEN_RECKLESS_FIST:
			{
				tableName = "RAVEN_RECKLESS_FIST";
			} break;
#endif

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
			{
				tableName = "RAVEN_VETERAN_COMMANDER";
			} break;
#endif SERV_RAVEN_VETERAN_COMMANDER

		default:
			{
				ASSERT( !"NO" );
			} break;
		}

		CX2GUUser::ParseCommonRandomState(tableName);

		m_LuaManager.EndTable(); // INIT_COMMON_RANDOM_STATE
	}
}


void CX2GURaven::InitState()
{
	ASSERT( NULL != GetUnit() );

	const CX2Unit::UnitData* pUnitData = &GetUnit()->GetUnitData();
	
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_FIGHTER:
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
	case CX2Unit::UC_RAVEN_OVER_TAKER:
#ifdef RAVEN_WEAPON_TAKER
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
#endif RAVEN_WEAPON_TAKER
		{
			m_iSkillCutInSetIndex = 0;
		} break;
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
		{
			m_iSkillCutInSetIndex = 1;
		} break;
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
		{
			m_iSkillCutInSetIndex = 2;
		} break;
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			m_iSkillCutInSetIndex = 3;
		} break;
#endif SERV_RAVEN_VETERAN_COMMANDER
	}

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
		{
			m_iSkillCutInSetSubIndex = 1;
		} break;
	case CX2Unit::UC_RAVEN_OVER_TAKER:
		{
			m_iSkillCutInSetSubIndex = 2;
		} break;
	default:
		{
			m_iSkillCutInSetSubIndex = 0;
		}
		break;
	}

	
	InitStateID();

	// 공통으로 쓰는 상태
	InitStateCommon();



	// 공통으로 쓰는 랜덤한 상태 start, win, lose 상태 초기화
	std::string tableNameStartUTF8;
	std::string tableNameWinUTF8;
	std::string tableNameLoseUTF8;
	InitStateCommonRandom( tableNameStartUTF8, tableNameWinUTF8, tableNameLoseUTF8 );


	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_START;
	m_LuaManager.MakeTableReference( tableNameStartUTF8.c_str(), stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WIN;
	m_LuaManager.MakeTableReference( tableNameWinUTF8.c_str(), stateData.stateID );
#ifdef SERV_PET_SYSTEM
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, WinStateStart );	
#endif
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_LOSE;
	m_LuaManager.MakeTableReference( tableNameLoseUTF8.c_str(), stateData.stateID );
#ifdef SERV_PET_SYSTEM
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, LoseStateStart );	
#endif
#ifdef RAVEN_WEAPON_TAKER
	if( tableNameLoseUTF8 == "RSI_RWT_LOSE2" )
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_RWT_LOSE2_FrameMove );
#endif RAVEN_WEAPON_TAKER
	m_StateList[stateData.stateID] = stateData;


	
	//m_StateList[stateData.stateID] = stateData;  // ??? 왜 한번더 insert하지?


	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
#endif
		{
			InitStateSwordTaker();
		} break;

	case CX2Unit::UC_RAVEN_OVER_TAKER:
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
#endif
		{
			InitStateOverTaker();
		} break;
#ifdef RAVEN_WEAPON_TAKER
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
#endif SERV_RAVEN_VETERAN_COMMANDER
		{
			InitStateWeaponTaker();
		} break;
#endif RAVEN_WEAPON_TAKER
	}

	// 특정 클래스에만 있거나 특정 클래스만 기본 클래스와 다른 상태
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_FIGHTER:
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
	case CX2Unit::UC_RAVEN_OVER_TAKER:
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
#endif
#ifdef RAVEN_WEAPON_TAKER
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
#endif RAVEN_WEAPON_TAKER
		{
			stateData.Init();
			stateData.stateID			= RSI_DASH_JUMP_COMBO_X;
			m_LuaManager.MakeTableReference( "RSI_DASH_JUMP_COMBO_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_X_Init );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_X_Start );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_X_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;

#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
		{
			stateData.Init();
			stateData.stateID			= RSI_DASH_JUMP_COMBO_X;
			m_LuaManager.MakeTableReference( "RRF_DASH_JUMP_COMBO_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_X_Init );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_X_Start );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_X_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;			
		}
		break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
		case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			stateData.Init();
			stateData.stateID			= RVC_DASH_JUMP_COMBO_X;
			m_LuaManager.MakeTableReference( "RVC_DASH_JUMP_COMBO_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_X_Init );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_X_Start );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_X_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
#endif SERV_RAVEN_VETERAN_COMMANDER
	}

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_FIGHTER:
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
	case CX2Unit::UC_RAVEN_OVER_TAKER:
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
#endif
		{
			stateData.Init();
			stateData.stateID			= RSI_COMBO_ZZZX;
			m_LuaManager.MakeTableReference( "RSI_COMBO_ZZZX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZZX_Start );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
		{
			stateData.Init();
			stateData.stateID			= RSI_COMBO_ZZZX;
			m_LuaManager.MakeTableReference( "RBM_COMBO_ZZZX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZX_Init );
#ifdef BALANCE_BLADE_MASTER_20130117
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZX_StateStart );
#endif BALANCE_BLADE_MASTER_20130117
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
#endif
#ifdef RAVEN_WEAPON_TAKER
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
#endif SERV_RAVEN_VETERAN_COMMANDER
		{
			stateData.Init();
			stateData.stateID			= RSI_COMBO_ZZZX;
			m_LuaManager.MakeTableReference( "RWT_COMBO_ZZZX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RWT_COMBO_ZZZX_Start );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RWT_COMBO_ZZZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RWT_COMBO_ZZZX_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GURaven, RWT_COMBO_ZZZX_EndFuture );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
#endif RAVEN_WEAPON_TAKER
	}
	

#ifdef RAVEN_WEAPON_TAKER
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
#endif SERV_RAVEN_VETERAN_COMMANDER
		{
			stateData.Init();
			stateData.stateID			= RSI_COMBO_X;
			m_LuaManager.MakeTableReference( "RWT_COMBO_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RWT_COMBO_X_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RWT_COMBO_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RWT_COMBO_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
	default:
		{
			stateData.Init();
			stateData.stateID			= RSI_COMBO_X;
			m_LuaManager.MakeTableReference( "RSI_COMBO_X", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
	}
#endif RAVEN_WEAPON_TAKER


#ifdef RAVEN_SECOND_CLASS_CHANGE
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
		{
			stateData.Init();
			stateData.stateID			= RBM_COMBO_XZ;
			m_LuaManager.MakeTableReference( "RBM_COMBO_XZ", stateData.stateID );			
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RBM_Combo_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RBM_COMBO_XZZ;
			m_LuaManager.MakeTableReference( "RBM_COMBO_XZZ", stateData.stateID );			
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RBM_Combo_XZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RBM_COMBO_XZZZ;
			m_LuaManager.MakeTableReference( "RBM_COMBO_XZZZ", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RBM_Combo_XZZZ_FrameMoveFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RBM_Combo_XZZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;
			
			stateData.Init();
			stateData.stateID			= RBM_COMBO_ZZZXZ;
			m_LuaManager.MakeTableReference( "RBM_COMBO_ZZZXZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZXZ_Start );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZXZ_EventProcess );			
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RBM_COMBO_ZZZX_EXPLOSION;
			m_LuaManager.MakeTableReference( "RBM_COMBO_ZZZX_EXPLOSION", stateData.stateID );
#ifdef BALANCE_BLADE_MASTER_20130117	/// 캔슬시 마법체 소멸시킬 수 있도록 종료 함수 추가
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZX_EXPLOSION_StateStart );
#endif BALANCE_BLADE_MASTER_20130117
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZX_EXPLOSION_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RBM_COMBO_ZZZX_EXPLOSION_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
		{
			stateData.Init();
			stateData.stateID			= RRF_DASH_JUMP_COMBO_XX;
			m_LuaManager.MakeTableReference( "RRF_DASH_JUMP_COMBO_XX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_XX_Init );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_XX_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_XX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RRF_DASH_JUMP_COMBO_XX_LANDING;
			m_LuaManager.MakeTableReference( "RRF_DASH_JUMP_COMBO_XX_LANDING", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_XX_LANDING_Start );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_XX_LANDING_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RRF_DASH_JUMP_COMBO_XX_LANDING_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RRF_COMBO_ZZXFRONTX;
			m_LuaManager.MakeTableReference( "RRF_COMBO_ZZXFRONTX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RRF_COMBO_ZZXFRONTX_Start );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RRF_COMBO_ZZXFRONTX_FrameMoveFuture );			
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RRF_COMBO_ZZXFRONTX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RRF_COMBO_ZZXFRONTX_EventProcess );	
			stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RRF_COMBO_ZZXFRONTX_End );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RRF_COMBO_ZZXFRONTXX;
			m_LuaManager.MakeTableReference( "RRF_COMBO_ZZXFRONTXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RRF_COMBO_ZZXFRONTXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RRF_COMBO_ZZXFRONTXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			stateData.Init();
			stateData.stateID			= RVC_DASH_JUMP_COMBO_XX_START;
			m_LuaManager.MakeTableReference( "RVC_DASH_JUMP_COMBO_XX_START", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_START_StateStart );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_START_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_START_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_START_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RVC_DASH_JUMP_COMBO_XX_LOOP;
			m_LuaManager.MakeTableReference( "RVC_DASH_JUMP_COMBO_XX_LOOP", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_LOOP_StateStart );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_LOOP_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_LOOP_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_LOOP_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RVC_DASH_JUMP_COMBO_XX_FINISH;
			m_LuaManager.MakeTableReference( "RVC_DASH_JUMP_COMBO_XX_FINISH", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_FINISH_StateStart );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_FINISH_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_FINISH_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_FINISH_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RVC_DASH_JUMP_COMBO_XX_FINISH_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RVC_DASH_COMBO_ZZZ;
			m_LuaManager.MakeTableReference( "RVC_DASH_COMBO_ZZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZ_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RVC_DASH_COMBO_ZZZPushZ_READY;
			m_LuaManager.MakeTableReference( "RVC_DASH_COMBO_ZZZPushZ_READY", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZPushZ_READY_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZPushZ_READY_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZPushZ_READY_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RVC_DASH_COMBO_ZZZPushZ;
			m_LuaManager.MakeTableReference( "RVC_DASH_COMBO_ZZZPushZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZPushZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZPushZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZPushZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZPushZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= RVC_DASH_COMBO_ZZZZ_FINISH;
			m_LuaManager.MakeTableReference( "RVC_DASH_COMBO_ZZZZ_FINISH", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_DASH_COMBO_ZZZZ_FINISH_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
#endif SERV_RAVEN_VETERAN_COMMANDER
	}
#endif

#ifdef SERV_RAVEN_VETERAN_COMMANDER

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			stateData.Init();
			stateData.stateID			= RSI_DASH_COMBO_ZZ;
			m_LuaManager.MakeTableReference( "RSI_SOUL_TAKER_DASH_COMBO_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_DASH_COMBO_ZZ_EventProcess );
			m_StateList[ stateData.stateID ] = stateData;
		} break;
	}

#endif SERV_RAVEN_VETERAN_COMMANDER

	InitPassiveSkillState();
	InitEquippedActiveSkillState();
	

	CX2GUUser::InitState();
}

void CX2GURaven::InitStateID()
{
	m_CommonState.m_Wait	= USI_WAIT;
	m_ChargeMpState			= USI_WAIT;

#ifdef TRANSFORMER_TEST
	m_CommonState.m_Transformed = RSI_TRANSFORMED;
#endif TRANSFORMER_TEST




}

void CX2GURaven::InitStateCommon()
{

	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "RSI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_Start );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "RSI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_DIE_BACK_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DIE_BACK_Start );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DIE_BACK_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DIE_BACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	
	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "RSI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );	// 기존에 빠진듯 하여 추가함
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "RSI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_WAIT_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_WAIT_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WAIT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "RSI_WALK", stateData.stateID );
#ifdef BALANCE_BLADE_MASTER_20130117
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_WALK_Start );
#endif BALANCE_BLADE_MASTER_20130117
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WALK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "RSI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "RSI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "RSI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "RSI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "RSI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "RSI_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_END_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DASH_END_Start );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DASH_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "RSI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_Start );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_EventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_EndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "RSI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
#ifdef CONVERSION_VS
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
#else CONVERSION_VS
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, JumpLandingStart );
#endif CONVERSION_VS
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "RSI_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_POWER_LANDING_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_POWER_LANDING_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_POWER_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( "RSI_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
#endif HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_SMALL_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_SMALL_BACK", stateData.stateID );
	//{{ JHKang / 강정훈 / 2010/11/02 / 긴급탈출
#ifdef NEW_SKILL_2010_11
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_SMALL_BACK_Start );
#endif NEW_SKILL_2010_11
	//}} JHKang / 강정훈 / 2010/11/02 / 긴급탈출
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_SMALL_BACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_BIG_FRONT_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_BIG_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_BIG_BACK_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_BIG_BACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_DOWN_FRONT_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_DOWN_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_DOWN_BACK_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_DOWN_BACK_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DAMAGE_AIR_DOWN_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "RSI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_FRONT_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_FRONT_FRONT_End );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_FRONT_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_FRONT_BACK_End );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_BACK_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_BACK_FRONT_End );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_BACK_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ROLLING_BACK_BACK_End );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_FRONT_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_FRONT_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_FRONT_End );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_BACK_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_BACK_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_BACK_End );
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ATTACK_FRONT_NO_DOWN;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ATTACK_FRONT_NO_DOWN", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_FRONT_NO_DOWN_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_FRONT_NO_DOWN_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_FRONT_NO_DOWN_End );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_STANDUP_ATTACK_BACK_NO_DOWN;
	m_LuaManager.MakeTableReference( "RSI_STANDUP_ATTACK_BACK_NO_DOWN", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_BACK_NO_DOWN_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_BACK_NO_DOWN_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_STANDUP_ATTACK_BACK_NO_DOWN_End );
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= RSI_COMBO_Z;
	m_LuaManager.MakeTableReference( "RSI_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "RSI_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "RSI_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_COMBO_ZZZZ;
	m_LuaManager.MakeTableReference( "RSI_COMBO_ZZZZ", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZZZ_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	

	stateData.Init();
	stateData.stateID			= RSI_COMBO_ZZX;
	m_LuaManager.MakeTableReference( "RSI_COMBO_ZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_COMBO_ZZXX;
	m_LuaManager.MakeTableReference( "RSI_COMBO_ZZXX", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZXX_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_ZZXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifndef RAVEN_WEAPON_TAKER
	stateData.Init();
	stateData.stateID			= RSI_COMBO_X;
	m_LuaManager.MakeTableReference( "RSI_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif RAVEN_WEAPON_TAKER

	stateData.Init();
	stateData.stateID			= RSI_COMBO_XX;
	m_LuaManager.MakeTableReference( "RSI_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_COMBO_XXX;
	m_LuaManager.MakeTableReference( "RSI_COMBO_XXX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_COMBO_XXX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "RSI_DASH_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_DASH_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "RSI_DASH_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_COMBO_ZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;





	stateData.Init();
	stateData.stateID			= RSI_DASH_COMBO_X;
	m_LuaManager.MakeTableReference( "RSI_DASH_COMBO_X", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DASH_COMBO_X_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_DASH_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "RSI_DASH_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_DASH_JUMP_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "RSI_DASH_JUMP_COMBO_ZZ", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_ZZ_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_ZZ_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_ZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_DASH_JUMP_COMBO_ZZ_Landing;
	m_LuaManager.MakeTableReference( "RSI_DASH_JUMP_COMBO_ZZ_Landing", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_ZZ_Landing_StartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DASH_JUMP_COMBO_ZZ_Landing_EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "RSI_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_COMBO_Z_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_COMBO_Z_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "RSI_JUMP_COMBO_X", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_JUMP_COMBO_X_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_COMBO_X_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_JUMP_COMBO_X_EventProcess );
	m_StateList[stateData.stateID] = stateData;



#ifdef WALL_JUMP_TEST
	stateData.Init();
	stateData.stateID			= RSI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "RSI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;
#endif WALL_JUMP_TEST





	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "RSI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "RSI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef SPECIAL_USE_ITEM
	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "RSI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;
#endif SPECIAL_USE_ITEM

#ifdef RIDING_SYSTEM
	#pragma region RSI_RIDING_START
	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "RSI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_ON
	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "RSI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOnEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_OFF
	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "RSI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_WAIT_HABIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "RSI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_WAIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "RSI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_WALK
	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "RSI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
	
	#pragma region RSI_RIDING_JUMP_UP
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "RSI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "RSI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "RSI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_DASH
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "RSI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_DASH_END
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "RSI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_DASH_JUMP
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "RSI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_DASH_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "RSI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_DAMAGE_FRONT
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "RSI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_DAMAGE_BACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "RSI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_DIE
	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "RSI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_Start );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_DIE_FRONT_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "RSI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_JUMP_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "RSI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_ATTACK_X
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "RSI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_SPECIAL_ATTACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "RSI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region RSI_RIDING_SPECIAL_MOVE
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "RSI_RIDING_SPECIAL_MOVE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEndFutrue );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingSpecialMoveEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
#endif //RIDING_SYSTEM

#ifdef MODIFY_RIDING_PET_AWAKE
	stateData.Init();
	stateData.stateID			= USI_RIDING_HYPER_MODE;
	m_LuaManager.MakeTableReference( "RSI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RidingHyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingHyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingHyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingHyperModeEnd );
#endif HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;

#endif // MODIFY_RIDING_PET_AWAKE

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "RSI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "RSI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "RSI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef UPGRADE_RAVEN
	stateData.Init();
	stateData.stateID			= RSI_PARRYING_SMALL;
	m_LuaManager.MakeTableReference( "RSI_PARRYING_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_StateEnd ); 
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_PARRYING;
	m_LuaManager.MakeTableReference( "RSI_PARRYING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_StartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_PARRYING_StateEnd );	
	m_StateList[stateData.stateID] = stateData;
#endif



//#ifdef PVP_BOSS_COMBAT_TEST
//
//	stateData.Init();
//	stateData.stateID			= RSI_FROZEN;
//	m_LuaManager.MakeTableReference( "RSI_FROZEN", stateData.stateID );
//	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, Frozen_StateStart ); 
//	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, Frozen_EventProcess );
//	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, Frozen_StateEnd ); 
//	m_StateList[stateData.stateID] = stateData;
//
//	m_FrozenState = RSI_FROZEN;
//
//#endif PVP_BOSS_COMBAT_TEST

#ifdef TRANSFORMER_TEST

	stateData.Init();
	stateData.stateID			= RSI_TRANSFORMED;
	m_LuaManager.MakeTableReference( "RSI_TRANSFORMED", stateData.stateID );
	m_StateList[stateData.stateID] = stateData;


#endif TRANSFORMER_TEST


}

void CX2GURaven::InitStateSwordTaker()
{
	UserUnitStateData stateData;


	stateData.Init();
	stateData.stateID			= RSI_SOUL_TAKER_COMBO_ZZZupZ;
	m_LuaManager.MakeTableReference( "RSI_SOUL_TAKER_COMBO_ZZZupZ", stateData.stateID );		
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_COMBO_ZZZupZ_FrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_COMBO_ZZZupZ_EventProcess );					
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_SOUL_TAKER_COMBO_XXZ;
	m_LuaManager.MakeTableReference( "RSI_SOUL_TAKER_COMBO_XXZ", stateData.stateID );			
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_COMBO_XXZ_EventProcess );					
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_SOUL_TAKER_COMBO_XXZZ8;
	m_LuaManager.MakeTableReference( "RSI_SOUL_TAKER_COMBO_XXZZ8", stateData.stateID );	
#ifdef BALANCE_BLADE_MASTER_20130117		/// 슈퍼아머 색상 변경 때문에 추가
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_COMBO_XXZZ8_StateStart );
#endif BALANCE_BLADE_MASTER_20130117
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_COMBO_XXZZ8_FrameMove );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_COMBO_XXZZ8_EventProcess );					
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_SOUL_TAKER_COMBO_XXZZ8Z;
	m_LuaManager.MakeTableReference( "RSI_SOUL_TAKER_COMBO_XXZZ8Z", stateData.stateID );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_COMBO_XXZZ8Z_EventProcess );					
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= RSI_SOUL_TAKER_DASH_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "RSI_SOUL_TAKER_DASH_COMBO_ZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_DASH_COMBO_ZZZ_EventProcess );
	m_StateList[stateData.stateID] = stateData;


#ifndef SERV_RAVEN_VETERAN_COMMANDER		/// 베테랑 커맨더와 공통으로 사용하기 위해 위치 이동
	stateData.Init();
	stateData.stateID			= RSI_DASH_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "RSI_SOUL_TAKER_DASH_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SOUL_TAKER_DASH_COMBO_ZZ_EventProcess );
	m_StateList[ stateData.stateID ] = stateData;
#endif SERV_RAVEN_VETERAN_COMMANDER
}

void CX2GURaven::InitStateOverTaker()
{
	UserUnitStateData stateData;


	stateData.Init();
	stateData.stateID			= RSI_OVER_TAKER_COMBO_XXdownX;
	m_LuaManager.MakeTableReference( "RSI_OVER_TAKER_COMBO_XXdownX", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXdownX_FrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXdownX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_OVER_TAKER_COMBO_XXdownXdownX;
	m_LuaManager.MakeTableReference( "RSI_OVER_TAKER_COMBO_XXdownXdownX", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXdownXdownX_FrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXdownXdownX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_OVER_TAKER_COMBO_XXdownXdownXdownX;
	m_LuaManager.MakeTableReference( "RSI_OVER_TAKER_COMBO_XXdownXdownXdownX", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXdownXdownXdownX_FrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXdownXdownXdownX_EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_OVER_TAKER_COMBO_XXfrontX;
	m_LuaManager.MakeTableReference( "RSI_OVER_TAKER_COMBO_XXfrontX", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXfrontX_Init );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXfrontX_FrameMove );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_COMBO_XXfrontX_EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX;
	m_LuaManager.MakeTableReference( "RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_FrameMoveFuture );	
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING;
	m_LuaManager.MakeTableReference( "RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_StartFuture );	
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX_EventProcess);
	m_StateList[stateData.stateID] = stateData;


}


#ifdef RAVEN_WEAPON_TAKER
void CX2GURaven::InitStateWeaponTaker()
{
	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_COMBO_XX;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_COMBO_XX", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XX_FrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_COMBO_XXX;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_COMBO_XXX", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_COMBO_XXX_LOOP;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_COMBO_XXX_LOOP", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_LOOP_StateStart );	
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_LOOP_FrameMove );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_LOOP_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_LOOP_StateEnd );	
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_COMBO_XXX_FINISH;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_COMBO_XXX_FINISH", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_FINISH_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_XXX_FINISH_EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_COMBO_ZZZXX;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_COMBO_ZZZXX", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_ZZZXX_FrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_ZZZXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_COMBO_ZZZdownX;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_COMBO_ZZZdownX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_ZZZdownX_FrameMove );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_ZZZdownX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_COMBO_ZZZdownXX;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_COMBO_ZZZdownXX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_ZZZdownXX_FrameMove );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_COMBO_ZZZdownXX_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_EventProcess );
	stateData.StateEnd	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_StateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX;
	m_LuaManager.MakeTableReference( "RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_StateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_FrameMove );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_StateEnd );
	m_StateList[stateData.stateID] = stateData;
}
#endif RAVEN_WEAPON_TAKER

/*virtual*/ void CX2GURaven::InitPassiveSkillState()
{
	CX2GUUser::InitPassiveSkillState();


	m_bEnableShadowStepThrust = false;
	m_fAttackPowerRateShadowStepThrust = 1.f;
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RF_SHADOW_PIERCING, true );

	if( iSkillLevel > 0 )	// 내가 배운 스킬에 대해서만 Skill level up 효과가 적용된다.
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RF_SHADOW_PIERCING );

		if ( NULL == GetUnit()  )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		if( NULL != pSkillTemplet )
		{
			m_bEnableShadowStepThrust = true;
			m_fAttackPowerRateShadowStepThrust *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
			m_fMPConsumeShadowStepThrust = pSkillTemplet->GetSkillMPConsumptionValue( iSkillLevel );
		}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RF_THRUST );

	if( iSkillLevel > 0 )	// 내가 배운 스킬에 대해서만 Skill level up 효과가 적용된다.
	{
	#ifdef PVP_SEASON2
		iSkillLevel += GetSkillLevelUpNum( CX2SkillTree::SI_P_RF_THRUST );		// 패시브 스킬인데 액티브 스킬처럼 사용되는 것이라서 skill level up option에 포함시켜서 예외처리

		iSkillLevel = min( iSkillLevel, 10 );
	#else
		iSkillLevel += GetSkillLevelUpNum( CX2SkillTree::SI_P_RF_SHADOW_PIERCING );		// 패시브 스킬인데 액티브 스킬처럼 사용되는 것이라서 skill level up option에 포함시켜서 예외처리
	#endif
		
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RF_THRUST, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			m_bEnableShadowStepThrust = true;
			m_fAttackPowerRateShadowStepThrust *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
			m_fMPConsumeShadowStepThrust = pSkillTemplet->m_fMPConsumption;
		}
#endif //UPGRADE_SKILL_SYSTEM_2013

		//{{ JHKang / 강정훈 / 2010/12/01 / 긴급탈출 기능 추가로 인한 초기화 위치 변경(섀도우 스텝이 스킬 사용 슬롯에 없을 때 문제 발생)
#ifdef NEW_SKILL_2010_11
		// NOTE!! 섀도우 피어싱 예외 처리
		{
			UserUnitStateData stateData;

			stateData.Init();
			stateData.m_SPLevel			= 1;
			stateData.m_fPowerRate		= 1.f;	// note!!! 데미지 계산이 다른 active 필살기와 다르게 구현되어있으므로 주의. passive skill 의 데미지 증가 처리는 InitPassiveSkillState()에서.
			stateData.m_eSkillID		= CX2SkillTree::SI_A_RF_SHADOW_STEP;
			stateData.stateID			= RSI_SHADOW_STEP_THRUST;
			m_LuaManager.MakeTableReference( "RSI_SHADOW_STEP_THRUST", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SHADOW_STEP_THRUST_Init );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SHADOW_STEP_THRUST_Start );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SHADOW_STEP_THRUST_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
#endif NEW_SKILL_2010_11
		//}} JHKang / 강정훈 / 2010/12/01 / 긴급탈출 기능 추가로 인한 초기화 위치 변경
	}


#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	m_bEnableShadowStepThrustEnhance = false;		// 블마용 쉐도우 쓰러스트
	m_fAttackPowerRateShadowStepThrustEnhance = 1.f;
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 쉐도우 쓰러스트
	int iShadowThrustSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RBM_SHADOW_THRUST, true );
	if( iShadowThrustSkillLevel > 0 )
	{
		// 일반 레이븐의 공통 기술인 쉐도우 피어싱 ( SI_P_RF_THRUST ) 이
		// 새로 추가된 블마의 쉐도우 스러스트 ( SI_P_RBM_SHADOW_THRUST ) 와
		// 스킬 ID 가 비슷한 것에 유의!
		const CX2SkillTree::SkillTemplet* pSkillTempletEnhanceThrust = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RBM_SHADOW_THRUST );
		if( NULL != pSkillTempletEnhanceThrust )
		{
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			m_bEnableShadowStepThrustEnhance = true;
			m_fAttackPowerRateShadowStepThrustEnhance *= pSkillTempletEnhanceThrust->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iShadowThrustSkillLevel );
			m_fMPConsumeShadowStepThrust = pSkillTempletEnhanceThrust->GetSkillMPConsumptionValue( iShadowThrustSkillLevel );

			UserUnitStateData stateData;

			stateData.Init();
			stateData.m_SPLevel			= 1;
			stateData.m_fPowerRate		= 1.f;	// note!!! 데미지 계산이 다른 active 필살기와 다르게 구현되어있으므로 주의. passive skill 의 데미지 증가 처리는 InitPassiveSkillState()에서.
			stateData.m_eSkillID		= CX2SkillTree::SI_A_RF_SHADOW_STEP;
			stateData.stateID			= RSI_SHADOW_STEP_THRUST_ENHANCE;
			m_LuaManager.MakeTableReference( "RSI_SHADOW_STEP_THRUST_ENHANCE", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_P_RBM_SHADOW_STEP_THRUST_Init );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_P_RBM_SHADOW_STEP_THRUST_Start );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_P_RBM_SHADOW_STEP_THRUST_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		// 위 ( 쉐도우 피어싱) 와 같은 이유로 예외 처리
	}
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈



#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	m_bEnableShadowStepPunisher		= false;	 // 레피용 쉐도우 퍼니셔 사용 가능 여부
	m_fAttackPowerRateShadowStepPunisher	= 1.f;		 // 레피용 쉐도우 퍼니셔 공격력

	// 쉐도우 퍼니셔
	int iShadowPunisherSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RRF_SHADOW_PUNISHER, true );
	if( iShadowPunisherSkillLevel > 0 )
	{
		
		const CX2SkillTree::SkillTemplet* pSkillTempletShadowPunisher = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RRF_SHADOW_PUNISHER );
		if( NULL != pSkillTempletShadowPunisher )
		{
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			m_bEnableShadowStepPunisher = true;
			m_fAttackPowerRateShadowStepPunisher *= pSkillTempletShadowPunisher->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iShadowPunisherSkillLevel );

			UserUnitStateData stateData;


			stateData.Init();
			stateData.m_SPLevel			= 1;
			stateData.m_fPowerRate		= 1.f;	// note!!! 데미지 계산이 다른 active 필살기와 다르게 구현되어있으므로 주의. passive skill 의 데미지 증가 처리는 InitPassiveSkillState()에서.
			stateData.m_eSkillID		= CX2SkillTree::SI_A_RF_SHADOW_STEP;
			stateData.stateID			= RSI_P_RRF_SHADOW_PUNISHER;
			m_LuaManager.MakeTableReference( "RSI_P_RRF_SHADOW_PUNISHER", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_P_RRF_SHADOW_PUNISHER_Init );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_P_RRF_SHADOW_PUNISHER_Start );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_P_RRF_SHADOW_PUNISHER_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		// 위 ( 쉐도우 피어싱, 쓰러스트) 와 같은 이유로 예외 처리
	}
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 폭발하는 나소드 핸드
	m_fChanceOfExplosiveNasodHand = 0.f;

	int iExplosiveNasodHandSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RRF_EXPLOSIVE_NASOD_HAND, true );
	if( iExplosiveNasodHandSkillLevel > 0 )
	{		
		const CX2SkillTree::SkillTemplet* pSkillTempletExplosiveNasodHand = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RRF_EXPLOSIVE_NASOD_HAND );
		if( NULL != pSkillTempletExplosiveNasodHand )
		{
			m_fChanceOfExplosiveNasodHand = pSkillTempletExplosiveNasodHand->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iExplosiveNasodHandSkillLevel );
		}
	}
	// 폭발하는 나소드 핸드 효과를 받기 위해서는
	// 해당 DAMAGE_DATA 에 CREATE_DAMAGE_EFFECT_NAME 를 넣어준다.

	SetHitCreateDamageEffectRate ( m_fChanceOfExplosiveNasodHand ); // 확률 설정
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef RAVEN_SECOND_CLASS_CHANGE	
	m_eDamageTypeRaven = CX2DamageManager::DT_PHYSIC;
	m_fChangeDamageRelRaven = 1.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RRF_BURNING_NASOD_HAND, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RRF_BURNING_NASOD_HAND );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RRF_BURNING_NASOD_HAND );
		if( NULL != pSkillTemplet )
		{
			int damageType = 0;
			damageType = (int) pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_TYPE_CHANGE, iSkillLevel );
			switch(damageType)
			{
			case 1:				
				m_eDamageTypeRaven = CX2DamageManager::DT_PHYSIC;
				break;
			case 2:				
				m_eDamageTypeRaven = CX2DamageManager::DT_MAGIC;
				break;
			default:
				break;
			}
			m_fChangeDamageRelRaven = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RRF_BURNING_NASOD_HAND, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			int damageType = 0;
			damageType = (int) pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_TYPE_CHANGE );
			switch(damageType)
			{
			case 1:				
				m_eDamageTypeRaven = CX2DamageManager::DT_PHYSIC;
				break;
			case 2:				
				m_eDamageTypeRaven = CX2DamageManager::DT_MAGIC;
				break;
			default:
				break;
			}
			m_fChangeDamageRelRaven = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	m_fPassiveCriticalRateRaven = 0.f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RST_BLADE_MASTERY, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RST_BLADE_MASTERY );
		if( NULL != pSkillTemplet )
		{
			m_fPassiveCriticalRateRaven = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_CRITICAL_RATE_ABS, iSkillLevel );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RBM_BLADE_MASTERY );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RBM_BLADE_MASTERY, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			m_fPassiveCriticalRateRaven = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_CRITICAL_RATE_ABS );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
#endif
	//{{ JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가
#ifdef NEW_SKILL_2010_11
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RBM_EMERGENCY_ESCAPE, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RBM_EMERGENCY_ESCAPE );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
		//{{ JHKang / 강정훈 / 2010/11/01 / 섀도우 스텝 상태로 만들어서 StateChange 함수로 호출 가능하도록 만들기
		{
			UserUnitStateData stateData;

			stateData.Init();
			stateData.m_SPLevel			= 1;
			stateData.stateID			= RSI_SHADOW_STEP;
			m_LuaManager.MakeTableReference( "RSI_SI_A_RF_SHADOW_STEP", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_Init );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RBM_EMERGENCY_ESCAPE_StartFuture );
			stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_Start );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_End );
			m_StateList[stateData.stateID] = stateData;
		}
		//}}
	}
#endif NEW_SKILL_2010_11
	//}} JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가

//{{ kimhc // 2010.11.3 // 레피 - 승화된 분노
#ifdef	NEW_SKILL_2010_11
 	const CX2SkillTree::SkillTemplet* pSkillTempletSpritualizedFury = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RRF_SPIRITUALIZED_FURY );
 
 	if ( NULL != pSkillTempletSpritualizedFury )
 	{
 		SetCanChangeHyperModeInHyperModeState( true );
 	}
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.3 // 레피 - 승화된 분노


#ifdef RAVEN_WEAPON_TAKER
	const CX2SkillTree::SkillTemplet* pSkillTemplet;

	// 에어슬라이서 공중 사용 데미지 보너스
	m_fAirSlicerDamageRel = 0.f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RVC_AIR_SLICER );
	if( NULL != pSkillTemplet )
	{
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// 스킬 레벨
		m_fAirSlicerDamageRel = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillTempletLevel );
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RWT_AIR_SLICER );
	if( NULL != pSkillTemplet )
		m_fAirSlicerDamageRel = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
#endif //UPGRADE_SKILL_SYSTEM_2013

	// 용병의 처신술
	m_fOverheatDamageRate = 1.0f;
	m_fReduceEDTDurationRate = 1.0f;
	pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RWT_BEHAVIOR_OF_MERCENARY );
	if( NULL != pSkillTemplet )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// 스킬 레벨
	
		// 오버히트 데미지량 감소
		m_fOverheatDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_OVERHEAT_HP, iSkillTempletLevel );

		// 상태이상 지속효과 감소
		m_fReduceEDTDurationRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REDUCE_EDT_TIME_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		// 오버히트 데미지량 감소
		m_fOverheatDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_OVERHEAT_HP );

		// 상태이상 지속효과 감소
		m_fReduceEDTDurationRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REDUCE_EDT_TIME_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}


#endif RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	/// 레이븐 2차 전직 베테랑 커맨더 - 치명상
	const CX2SkillTree::SkillTemplet* pSkillTempletBleedingSlicer = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_A_RVC_BLEEDING_SLICER );

	if ( NULL != pSkillTempletBleedingSlicer )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTempletBleedingSlicer->m_eID, true ) );	/// 스킬 레벨
	
		m_fEDTBleedingTime = pSkillTempletBleedingSlicer->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );	/// 유지 시간
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_fEDTBleedingTime = pSkillTempletBleedingSlicer->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );	/// 유지 시간
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

	// 레이븐 2차 전직 베테랑 커맨더 - 용병의 생존전략	
	m_bEnableSurvivalTechniqueOfMercenary = false;

	const CX2SkillTree::SkillTemplet* pSkillTempletSurvivalTechniqueOfMercenary = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY );

	if ( NULL != pSkillTempletSurvivalTechniqueOfMercenary )
	{
		m_bEnableSurvivalTechniqueOfMercenary = true;
		/// 오버 히트 체력 제한 보정

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTempletSurvivalTechniqueOfMercenary->m_eID, true ) );	/// 스킬 레벨
	
		m_fLimitOverHeatHP		= _CONST_RAVEN_::OVERHEAT_HP_LIMIT * pSkillTempletSurvivalTechniqueOfMercenary->GetSkillAbilityValue( CX2SkillTree::SA_OVERHEAT_HP, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_fLimitOverHeatHP		= _CONST_RAVEN_::OVERHEAT_HP_LIMIT * pSkillTempletSurvivalTechniqueOfMercenary->GetSkillAbilityValue( CX2SkillTree::SA_OVERHEAT_HP );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		InsertDelegateProcessToCommonFrameMove( SET_DELEGATE_PROCESS( CX2GURaven, CheckAndDoServivalTechniqueOfMercenary ) );
	}

	// 레이븐 2차 전직 베테랑 커맨더 - 섀도우 백 슬라이드
	m_bEnableShadowBackSlide = false;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RVC_SHADOW_BACK_SLIDE, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RVC_SHADOW_BACK_SLIDE );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
		m_iShadowBackSlideLevel = 1;	/// 섀도우 백 슬라이드 습득시 쿨타임 1초 감소

		m_bEnableShadowBackSlide = true;

		UserUnitStateData stateData;


		// 섀도우 백 슬라이드

		stateData.Init();
		//stateData.m_SPLevel			 = 1;
		stateData.stateID				 = RVC_SI_P_SHADOW_BACK_SLIDE;
		m_LuaManager.MakeTableReference( "RVC_SI_P_SHADOW_BACK_SLIDE", stateData.stateID );
		stateData.StateStartFuture		 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BACK_SLIDE_StartFuture );
		stateData.StateStart			 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BACK_SLIDE_Start );
		stateData.OnFrameMove			 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BACK_SLIDE_FrameMove );
		stateData.OnEventProcess		 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BACK_SLIDE_EventProcess );
		stateData.StateEnd				 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BACK_SLIDE_End );
		m_StateList[stateData.stateID] = stateData;

		
		// 섀도우 백 슬라이드 종료 모션

		stateData.Init();
		//stateData.m_SPLevel			 = 1;
		stateData.stateID				 = RVC_SI_P_SHADOW_BACK_SLIDE_END;
		m_LuaManager.MakeTableReference( "RVC_SI_P_SHADOW_BACK_SLIDE_END", stateData.stateID );
		stateData.OnFrameMoveFuture		 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BACK_SLIDE_END_FrameMoveFuture );
		stateData.OnEventProcess		 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BACK_SLIDE_END_EventProcess );
		m_StateList[stateData.stateID] = stateData;


		// 섀도우 스텝 피니시

		stateData.Init();
		//stateData.m_SPLevel			 = 1;
		stateData.stateID				 = RVC_SI_P_SHADOW_STEP_FINISH;
		m_LuaManager.MakeTableReference( "RSI_SI_A_RF_SHADOW_STEP", stateData.stateID );
		stateData.StateInit				 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_STEP_FINISH_Init );
		stateData.StateStartFuture		 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_STEP_FINISH_StartFuture );
		stateData.StateStart			 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_STEP_FINISH_Start );
		stateData.OnFrameMove			 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_STEP_FINISH_FrameMove );
		stateData.OnEventProcess		 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_STEP_FINISH_EventProcess );
		stateData.StateEnd				 = SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_STEP_FINISH_End );
		m_StateList[stateData.stateID] = stateData;


		// 섀도우 버스터

		stateData.Init();
		//stateData.m_SPLevel			= 1;
		//stateData.m_fPowerRate		= 1.f;
		//stateData.m_eSkillID		= CX2SkillTree::SI_A_RF_SHADOW_STEP;
		stateData.stateID			= RVC_SI_P_SHADOW_BUSTER;
		m_LuaManager.MakeTableReference( "RVC_SI_P_SHADOW_BUSTER", stateData.stateID );
		stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BUSTER_Init );
		stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BUSTER_Start );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BUSTER_FrameMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_P_SHADOW_BUSTER_EventProcess );
		m_StateList[stateData.stateID] = stateData;

		pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RVC_SHADOW_BACK_SLIDE );

		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// 스킬 레벨
	
			m_fShadowBusterPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillTempletLevel );	/// 섀도우 버스터 파워레이트
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fShadowBusterPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );	/// 섀도우 버스터 파워레이트
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#endif SERV_RAVEN_VETERAN_COMMANDER

	/// 역전의 용사
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	const CX2SkillTree::SkillTemplet* pReversalSoldierSkillTemplet 
		= userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RBM_REVERSAL_SOLDIER );
#else //UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pReversalSoldierSkillTemplet 
		= userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_COMMON_REVERSAL_SOLDIER );
#endif //UPGRADE_SKILL_SYSTEM_2013
	if ( NULL != pReversalSoldierSkillTemplet )
		InsertDelegateProcessToCommonFrameMove( SET_DELEGATE_PROCESS( CX2GURaven, CheckAndDoReversalSoldier ) );


#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 섬세한 검술, 동속 증가
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RST_EXQUISITE_SWORDMANSHIP, true );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RST_EXQUISITE_SWORDMANSHIP );
		if( NULL != pSkillTemplet )
		{
			float fAddAnimSpeedFactor = CalculateIncreasingRate( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ANI_SPEED, iSkillLevel ) );
			SetAddAnimSpeedFactor( fAddAnimSpeedFactor );
		}
	}
	else
	{
		SetAddAnimSpeedFactor( 0.f );
	}


	// 용병의 승리 전략, 일정 HP % 에서 오버히트 데미지를 입지 않게 해준다.
	// GetFinalDamage 전, HP 까지는 곳에서 예외 처리
	m_fOverheatDamageLimitedRate = 0.f;
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RVC_WINNABLE_TECHNIQUE_OF_MERCENARY, true );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RVC_WINNABLE_TECHNIQUE_OF_MERCENARY );
		if( NULL != pSkillTemplet )
		{
			float fLimitOverHeatHP_TechniqueOfMercenary = 
				_CONST_RAVEN_::OVERHEAT_HP_LIMIT * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_OVERHEAT_HP, iSkillLevel );

			m_fLimitOverHeatHP = min ( m_fLimitOverHeatHP, fLimitOverHeatHP_TechniqueOfMercenary );
			m_fOverheatDamageLimitedRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGED_REL, iSkillLevel );			
		}
	}

	// 과열모드 작동!
	// 오버히트 버프의 공격력 증가량을 증가시키고
	// 지속 시간을 늘려준다.

	m_fAddOverHeatBuffDurationTime = 0;
	m_iAddOverHeatLevel = 0;
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RVC_OVERHEAT_MODE, true );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RVC_OVERHEAT_MODE );
		if( NULL != pSkillTemplet )
		{
			m_iAddOverHeatLevel			   = iSkillLevel;
			m_fAddOverHeatBuffDurationTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillLevel );			
		}
	}
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈



}



/*virtual*/ void CX2GURaven::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ )
{
	if ( iSlotIndex_ < 0 || iSlotIndex_ >= ARRAY_SIZE( _CONST_GUUSER_::aNormalSkillStateID ) )
		return;

	int iNormalStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aNormalSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aNormalSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& normalStateData = m_StateList[ iNormalStateID ];
	normalStateData.Init();
	normalStateData.stateID = iNormalStateID;

	int iHyperStateID = ( bSlotB_ == false ? _CONST_GUUSER_::aHyperSkillStateID[iSlotIndex_] : _CONST_GUUSER_::aHyperSkillStateIDSlotB[iSlotIndex_] );
	UserUnitStateData& hyperStateData = m_StateList[ iHyperStateID ];
	hyperStateData.Init();	
	hyperStateData.m_bHyperState = true;
	hyperStateData.stateID = iHyperStateID;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillID_ );
	if( NULL == pSkillTemplet )
		return;

	switch( pSkillTemplet->m_eType )
	{	
		case CX2SkillTree::ST_BUFF:
			{	// 버프 필살기
				m_LuaManager.MakeTableReference( "RSI_SI1_COMMON_BUFF", normalStateData.stateID );
				normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, COMMON_BUFF_FrameMove );	
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	

				m_LuaManager.MakeTableReference( "RSI_SI1_COMMON_BUFF_HYPER", hyperStateData.stateID );
				hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, COMMON_BUFF_FrameMove );	
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );

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
						m_LuaManager.MakeTableReference( "RSI_THROW_ITEM", normalStateData.stateID );
						normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	
						normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	

						m_LuaManager.MakeTableReference( "RSI_THROW_ITEM", hyperStateData.stateID );
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// 여기서 부터 ACTIVE, SPECIAL ACTIVE 스킬//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	switch( eSkillID_ )
	{
	default:
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_RF_MAXIMUM_CANNON:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RF_MAXIMUM_CANNON:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "RSI_SI1_MAXIMUM_CANNON", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI1_MAXIMUM_CANNON_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_MAXIMUM_CANNON_FrameMove );
			//normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI1_MAXIMUM_CANNON_CameraMove );		
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_MAXIMUM_CANNON_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI1_MAXIMUM_CANNON_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_MAXIMUM_CANNON_HYPER_FrameMove );
			//hyperStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI1_MAXIMUM_CANNON_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_MAXIMUM_CANNON_HYPER_EventProcess );

		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_RF_EARTH_BREAKER:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RF_EARTH_BREAKER:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "RSI_SI1_EARTH_BREAKER", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI1_EARTH_BREAKER_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_EARTH_BREAKER_FrameMove );
			//normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI1_EARTH_BREAKER_CameraMove );		
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_EARTH_BREAKER_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI1_EARTH_BREAKER_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_EARTH_BREAKER_HYPER_FrameMove );
			//hyperStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI1_EARTH_BREAKER_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_EARTH_BREAKER_HYPER_EventProcess );
		} break;


	case CX2SkillTree::SI_SA_RF_POWER_ASSAULT:
		{
			m_LuaManager.MakeTableReference( "RSI_SI2_POWER_ASSAULT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_FrameMove );
			//normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_CameraMove );		
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_EventProcess );

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 파워 어설트, 밸런스 패치 이후 Hyper State 삭제
			m_LuaManager.MakeTableReference( "RSI_SI2_POWER_ASSAULT", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_Init );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_FrameMove );
			//hyperStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_EventProcess );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			m_LuaManager.MakeTableReference( "RSI_SI2_POWER_ASSAULT_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_HYPER_FrameMove );
			//hyperStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_POWER_ASSAULT_HYPER_EventProcess );

#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		} break;

	case CX2SkillTree::SI_SA_RF_CANNON_BLADE:
		{
			m_LuaManager.MakeTableReference( "RSI_SI2_CANNON_BLADE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI2_CANNON_BLADE_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_CANNON_BLADE_FrameMove );	
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_CANNON_BLADE_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI2_CANNON_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_CANNON_BLADE_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_CANNON_BLADE_HYPER_EventProcess );

		} break;

#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case CX2SkillTree::SI_SA_RF_MAGNUM_BLASTER:
		{
			m_LuaManager.MakeTableReference( "RSI_SI3_MAGNUM_BLASTER", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_End );

			m_LuaManager.MakeTableReference( "RSI_SI3_MAGNUM_BLASTER_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_HYPER_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GURaven, RSI_SI3_MAGNUM_BLASTER_HYPER_End );

		} break;
#endif //UPGRADE_SKILL_SYSTEM_2013

	case CX2SkillTree::SI_SA_RF_SEVEN_BURST:
		{
			m_LuaManager.MakeTableReference( "RSI_SI3_SEVEN_BURST", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI3_SEVEN_BURST_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI3_SEVEN_BURST_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_SEVEN_BURST_FrameMove );
			//normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI2_SEVEN_BURST_CameraMove );		
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_SEVEN_BURST_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI3_SEVEN_BURST_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI3_SEVEN_BURST_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_SEVEN_BURST_HYPER_FrameMove );
			//hyperStateData.OnCameraMove		= SET_CB_FUNC( CX2GURaven, RSI_SI2_SEVEN_BURST_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_SEVEN_BURST_HYPER_EventProcess );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_RST_BERSERKER_BLADE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RST_BERSERKER_BLADE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "RSI_SI1_SOUL_TAKER_BERSERKER_BLADE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_HYPER_EventProcess );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_RBM_FLYING_IMPACT:		// 주의!! : 아크에너미와 플라잉임팩트 코드 내용이 서로 바뀌어있음, 이름 바꿔야함
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RST_FLYING_IMPACT:		// 주의!! : 아크에너미와 플라잉임팩트 코드 내용이 서로 바뀌어있음, 이름 바꿔야함
#endif //UPGRADE_SKILL_SYSTEM_2013
		{											// 원래 이름대로 바꿈 ( 2013. 01. 09 )
			m_LuaManager.MakeTableReference( "RSI_SI2_SOUL_TAKER_FLYING_IMPACT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI2_SOUL_TAKER_FLYING_IMPACT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_SOUL_TAKER_FLYING_IMPACT_FrameMove );	
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_SOUL_TAKER_FLYING_IMPACT_EventProcess );	
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SI2_SOUL_TAKER_FLYING_IMPACT_End );


			m_LuaManager.MakeTableReference( "RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GURaven, RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_End );

		} break;

	case CX2SkillTree::SI_SA_RST_HYPER_SONIC_STAB:
		{
			m_LuaManager.MakeTableReference( "RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB", normalStateData.stateID );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER", hyperStateData.stateID );				
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_EventProcess );

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_ROT_CHARGED_BOLT_HEDGHOG:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ROT_CHARGED_BOLT:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "RSI_SI1_OVER_TAKER_CHARGED_BOLT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateStartFuture );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_EventProcess );	
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateEnd );

			m_LuaManager.MakeTableReference( "RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER", hyperStateData.stateID );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateStartFuture );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GURaven, RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateEnd );

		} break;

	case CX2SkillTree::SI_SA_ROT_ARC_ENEMY:		// 주의!! : 아크에너미와 플라잉임팩트 코드 내용이 서로 바뀌어있음, 이름 바꿔야함
		{										// 원래 이름대로 바꿈 ( 2013. 01. 09 )
			m_LuaManager.MakeTableReference( "RSI_SI2_OVER_TAKER_ARC_ENEMY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI2_OVER_TAKER_ARC_ENEMY_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_OVER_TAKER_ARC_ENEMY_FrameMove );	
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_OVER_TAKER_ARC_ENEMY_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI2_OVER_TAKER_ARC_ENEMY_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI2_OVER_TAKER_ARC_ENEMY_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI2_OVER_TAKER_ARC_ENEMY_HYPER_EventProcess );
		} break;

	case CX2SkillTree::SI_SA_ROT_GUARDIAN_STRIKE:
		{
			m_LuaManager.MakeTableReference( "RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_StateStartFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER", hyperStateData.stateID );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GURaven, RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_StateStartFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_A_RF_BURNING_RUSH:
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
			// 버닝 러쉬 State 를 기존 3개에서 1개로 수정 ( 대쉬만 사용 )
			m_LuaManager.MakeTableReference( "RSI_SUPER_DASH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_Init );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_StateStart );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			normalStateData.OnFrameMoveFuture   = SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_FrameMoveFuture );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_EventProcess );	

			m_LuaManager.MakeTableReference( "RSI_SUPER_DASH", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_Init );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_StateStart );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			hyperStateData.OnFrameMoveFuture   = SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_FrameMoveFuture );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_EventProcess );	

			hyperStateData.m_bHyperState = true;

#else // UPGRADE_SKILL_SYSTEM_2013
			m_LuaManager.MakeTableReference( "RSI_SUPER_DASH_READY", normalStateData.stateID );
	#ifdef SERV_SKILL_NOTE
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_READY_StateStart );
	#endif
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_READY_EventProcess );

			m_LuaManager.MakeTableReference( "RSI_SUPER_DASH_READY", hyperStateData.stateID );
	#ifdef SERV_SKILL_NOTE
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_READY_StateStart );
	#endif
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_READY_EventProcess );




			// note!! 버닝 러시 예외 처리
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= RSI_SUPER_DASH;
				m_LuaManager.MakeTableReference( "RSI_SUPER_DASH", stateData.stateID );
				stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_FrameMoveFuture );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_EventProcess );
				m_StateList[stateData.stateID] = stateData;


				stateData.Init();
				stateData.stateID				= RSI_SUPER_DASH_END;
				m_LuaManager.MakeTableReference( "RSI_SUPER_DASH_END", stateData.stateID );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_END_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SUPER_DASH_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}

#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈


		} break;


	case CX2SkillTree::SI_A_RF_SHADOW_STEP:
		{
			m_LuaManager.MakeTableReference( "RSI_SI_A_RF_SHADOW_STEP", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_StartFuture );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_Start );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_End );



			m_LuaManager.MakeTableReference( "RSI_SI_A_RF_SHADOW_STEP", hyperStateData.stateID );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_StartFuture );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_Start );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GURaven, RSI_SI_A_RF_SHADOW_STEP_End );


			//{{ JHKang / 강정훈 / 2010/12/01 /  긴급탈출 기능 추가로 인한 초기화 위치 변경(섀도우 스텝이 스킬 사용 슬롯에 없을 때 문제 발생)
#ifndef NEW_SKILL_2010_11
			// NOTE!! 섀도우 피어싱 예외 처리
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.m_SPLevel			= 1;
				stateData.m_fPowerRate		= 1.f;	// note!!! 데미지 계산이 다른 active 필살기와 다르게 구현되어있으므로 주의. passive skill 의 데미지 증가 처리는 InitPassiveSkillState()에서.
				stateData.m_eSkillID		= CX2SkillTree::SI_A_RF_SHADOW_STEP;
				stateData.stateID			= RSI_SHADOW_STEP_THRUST;
				m_LuaManager.MakeTableReference( "RSI_SHADOW_STEP_THRUST", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SHADOW_STEP_THRUST_Init );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SHADOW_STEP_THRUST_Start );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SHADOW_STEP_THRUST_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
#endif NEW_SKILL_2010_11
			//}} JHKang / 강정훈 / 2010/12/01 / 긴급탈출 기능 추가로 인한 초기화 위치 변경

		} break;



#ifdef SKILL_30_TEST

	case CX2SkillTree::SI_SA_RST_SHOCKWAVE:
		{
			m_LuaManager.MakeTableReference( "RSI_SA_RST_SHOCKWAVE", normalStateData.stateID ); 			
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_StateEnd );


			m_LuaManager.MakeTableReference( "RSI_SA_RST_SHOCKWAVE_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_HYPER_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_HYPER_StateEnd );

		} break;


	case CX2SkillTree::SI_SA_ROT_VALKYRIESJAVELIN:
		{
			m_LuaManager.MakeTableReference( "RSI_SA_ROT_VALKYRIESJAVELIN", normalStateData.stateID ); 	
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_StateEnd );


			m_LuaManager.MakeTableReference( "RSI_SA_ROT_VALKYRIESJAVELIN_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_StateEnd );


			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_ROT_VALKYRIESJAVELIN_LANDING;
				m_LuaManager.MakeTableReference( "RSI_SA_ROT_VALKYRIESJAVELIN_LANDING", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_EventProcess );										
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_LANDING;
				m_LuaManager.MakeTableReference( "RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_LANDING", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_HYPER_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_HYPER_EventProcess );									
				m_StateList[stateData.stateID] = stateData;
			}

		} break;


	case CX2SkillTree::SI_A_RST_CUT_TENDON:
		{

			m_LuaManager.MakeTableReference( "RSI_A_RST_CUT_TENDON", normalStateData.stateID ); 	
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_RST_CUT_TENDON_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_RST_CUT_TENDON_StateStart );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RST_CUT_TENDON_EventProcess );				
			
			m_LuaManager.MakeTableReference( "RSI_A_RST_CUT_TENDON_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;


	case CX2SkillTree::SI_A_ROT_WEAPON_BREAK:
		{


			m_LuaManager.MakeTableReference( "RSI_A_ROT_WEAPON_BREAK", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_WEAPON_BREAK_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_WEAPON_BREAK_StateStart );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_WEAPON_BREAK_EventProcess );				
			

			m_LuaManager.MakeTableReference( "RSI_A_ROT_WEAPON_BREAK_HYPER", hyperStateData.stateID );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_WEAPON_BREAK_StateStart );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_WEAPON_BREAK_EventProcess );	
			


		} break;

#endif SKILL_30_TEST

#ifdef SKILL_CASH_10_TEST

	case CX2SkillTree::SI_SA_RST_BLOODY_ACCEL:
		{
			m_LuaManager.MakeTableReference( "RSI_A_RST_BLOODY_ACCEL", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RST_BLOODY_ACCEL_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_RST_BLOODY_ACCEL_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RST_BLOODY_ACCEL_EventProcess );
			
			m_LuaManager.MakeTableReference( "RSI_A_RST_BLOODY_ACCEL_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_SA_ROT_NUCLEAR:
		{
			m_LuaManager.MakeTableReference( "RSI_A_ROT_NUCLEAR", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_NUCLEAR_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_NUCLEAR_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_NUCLEAR_EventProcess );

			m_LuaManager.MakeTableReference( "RSI_A_ROT_NUCLEAR_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_NUCLEAR_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_NUCLEAR_Hyper_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_NUCLEAR_Hyper_EventProcess );

		} break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_RF_GROUND_IMPACT:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_RRF_GROUND_IMPACT:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "SI_A_RRF_GROUND_IMPACT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_A_RRF_GROUND_IMPACT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, SI_A_RRF_GROUND_IMPACT_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_A_RRF_GROUND_IMPACT_EventProcess );

			m_LuaManager.MakeTableReference( "SI_A_RRF_GROUND_IMPACT", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_A_RRF_GROUND_IMPACT_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, SI_A_RRF_GROUND_IMPACT_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_A_RRF_GROUND_IMPACT_EventProcess );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_RRF_X_CRASH:		
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RRF_X_CRASH:		
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "SI_SA_RRF_X_CRASH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_SA_RRF_X_CRASH_Init );		
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, SI_SA_RRF_X_CRASH_FrameMove );				
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_SA_RRF_X_CRASH_EventProcess );

			m_LuaManager.MakeTableReference( "SI_SA_RRF_X_CRASH_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_SA_RRF_X_CRASH_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, SI_SA_RRF_X_CRASH_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_SA_RRF_X_CRASH_EventProcess );
		} break;

	case CX2SkillTree::SI_A_RBM_ONE_FLASH:		
		{
			m_LuaManager.MakeTableReference( "SI_A_RBM_ONE_FLASH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_A_RBM_ONE_FLASH_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, SI_A_RBM_ONE_FLASH_StateStart );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_A_RBM_ONE_FLASH_EventProcess );

			m_LuaManager.MakeTableReference( "SI_A_RBM_ONE_FLASH", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_A_RBM_ONE_FLASH_Init );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, SI_A_RBM_ONE_FLASH_StateStart );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_A_RBM_ONE_FLASH_EventProcess );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_RBM_WOFL_FANG:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RBM_WOFL_FANG:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "SI_SA_RBM_WOLF_FANG_START", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_FrameMove );				
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_EventProcess );

			m_LuaManager.MakeTableReference( "SI_SA_RBM_WOLF_FANG_START", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_Init );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_START_EventProcess );
			hyperStateData.m_bHyperState		= true;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= SI_SA_RBM_WOLF_FANG_STARTLANDING;
				m_LuaManager.MakeTableReference( "SI_SA_RBM_WOLF_FANG_STARTLANDING", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_STARTLANDING_EventProcess );										
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= SI_SA_RBM_WOLF_FANG_STARTLANDING_HYPER;
				m_LuaManager.MakeTableReference( "SI_SA_RBM_WOLF_FANG_STARTLANDING", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_STARTLANDING_EventProcess );										
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= SI_SA_RBM_WOLF_FANG_HIT;
				m_LuaManager.MakeTableReference( "SI_SA_RBM_WOLF_FANG_HIT", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_HIT_EventProcess );									
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= SI_SA_RBM_WOLF_FANG_HIT_HYPER;
				m_LuaManager.MakeTableReference( "SI_SA_RBM_WOLF_FANG_HIT_HYPER", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_HIT_EventProcess );									
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= SI_SA_RBM_WOLF_FANG_HITLANDING;
				m_LuaManager.MakeTableReference( "SI_SA_RBM_WOLF_FANG_HITLANDING", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, SI_SA_RBM_WOLF_FANG_HITLANDING_EventProcess );									
				m_StateList[stateData.stateID] = stateData;
			}

		} break;
#endif

		//{{ kimhc // 2010.11.10 // 블마-기가드라이브, 레피-와일드차지
#ifdef	NEW_SKILL_2010_11

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_RBM_GIGA_DRIVE_LIMITER:		// 블마-기가드라이브
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RBM_GIGA_DRIVE:		// 블마-기가드라이브
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "RSI_SA_RBM_GIGA_DRIVE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_Init );		
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_FrameMove );				
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_EventProcess );

			m_LuaManager.MakeTableReference( "RSI_SA_RBM_GIGA_DRIVE_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_EventProcess );
		} break;

	case CX2SkillTree::SI_SA_RRF_WILD_CHARGE:		// 레피-와일드차지
		{
			m_LuaManager.MakeTableReference( "RSI_SA_RRF_WILD_CHARGE_READY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGE_READY_Init );		
			normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGE_READY_StateStart );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGE_READY_StateStartFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGE_READY_FrameMove );				
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGE_READY_EventProcess );

			m_LuaManager.MakeTableReference( "RSI_SA_RRF_WILD_CHARGE_READY", hyperStateData.stateID );
			//hyperStateData.stateID				= normalStateData.stateID;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RRF_WILD_CHARGING;
				m_LuaManager.MakeTableReference( "RSI_SA_RRF_WILD_CHARGING", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGING_StateStart );	
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGING_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGING_EventProcess );										
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGING_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RRF_WILD_CHARGE_DASH;
				m_LuaManager.MakeTableReference( "RSI_SA_RRF_WILD_CHARGE_DASH", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGE_DASH_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RRF_WILD_CHARGE_DASH_EventProcess );									
				m_StateList[stateData.stateID] = stateData;
			}

			// 와일드차지가 상태 ID (ex: RSI_SPECIAL_ATTACK1a 등)
			// slotB 주의!

#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			if ( 0 > m_WildChargeDataPtr.use_count() )
				m_WildChargeDataPtr.reset();
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

			m_WildChargeDataPtr = WildChargeData::CreateWildChargeData();
			m_WildChargeDataPtr->SetSlotID_WildCharge( RAVEN_STATE_ID( iNormalStateID ) );
			m_WildChargeDataPtr->SetSlotB( bSlotB_ );
		} break;

#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.10 // 블마-기가드라이브, 레피-와일드차지


#ifdef RAVEN_WEAPON_TAKER
	case CX2SkillTree::SI_A_RF_BREAKING_FIST:
		{
			m_LuaManager.MakeTableReference( "RSI_A_RF_BREAKING_FIST", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_StateEnd );
#ifdef BALANCE_BLADE_MASTER_20130117	/// 캔슬시 움직임 멈추기 위해 추가
			normalStateData.StateEndFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_StateEndFuture );
#endif BALANCE_BLADE_MASTER_20130117


			m_LuaManager.MakeTableReference( "RSI_A_RF_BREAKING_FIST", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_StateEnd );
#ifdef BALANCE_BLADE_MASTER_20130117	/// 캔슬시 움직임 멈추기 위해 추가
			hyperStateData.StateEndFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RF_BREAKING_FIST_StateEndFuture );
#endif BALANCE_BLADE_MASTER_20130117
		} break;

	case CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK:
		{
			// 2012.01.05 박진웅. 웨폰테이커 작업
			// RSI_A_RWT_BARRAGE_ATTACK는 분기를 위한 가상 스테이트
			// RSI_A_RWT_BARRAGE_ATTACK_STAND는 지상에서 사용될 스테이트
			// RSI_A_RWT_BARRAGE_ATTACK_AIR는 공중에서 사용될 스테이트
			m_LuaManager.MakeTableReference( "RSI_A_RWT_BARRAGE_ATTACK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_StateStartFuture );
				
			m_LuaManager.MakeTableReference( "RSI_A_RWT_BARRAGE_ATTACK", hyperStateData.stateID );
			hyperStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_StateStartFuture );
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_A_RWT_BARRAGE_ATTACK_STAND;
				m_LuaManager.MakeTableReference( "RSI_A_RWT_BARRAGE_ATTACK_STAND", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_STAND_EventProcess );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_STAND_FrameMove );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_A_RWT_BARRAGE_ATTACK_AIR;
				m_LuaManager.MakeTableReference( "RSI_A_RWT_BARRAGE_ATTACK_AIR", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_AIR_EventProcess );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_AIR_FrameMove );
#ifdef RAVEN_LIMIT_AIR_COMBO		/// 공중 무한 채공 방지용
					stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_BARRAGE_ATTACK_AIR_FrameMoveFutrue );
#endif RAVEN_LIMIT_AIR_COMBO
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::SI_SA_RWT_BURSTING_BLADE:
		{
			// 2012.01.05 박진웅. 웨폰테이커 작업
			// RSI_SA_RWT_BURSTING_BLADE는 분기를 위한 가상 스테이트
			// RSI_A_RWT_BURSTING_BLADE_STAND는 지상에서 사용될 스테이트
			// RSI_A_RWT_BURSTING_BLADE_AIR는 공중에서 사용될 스테이트
			// 추가로 함수에서 할 행동은 같다고 보아 등록한 함수명은 같으며, 가상 스테이트와의 구별을 위해 _ATTACK 라고 붙여주었습니다.
			m_LuaManager.MakeTableReference( "RSI_SA_RWT_BURSTING_BLADE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_StateStartFuture );
				
			m_LuaManager.MakeTableReference( "RSI_SA_RWT_BURSTING_BLADE", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_Init );
			hyperStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_StateStartFuture );
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_BURSTING_BLADE_STAND;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_BURSTING_BLADE_STAND", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateStart );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_EventProcess );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_FrameMove );
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_BURSTING_BLADE_AIR;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_BURSTING_BLADE_AIR", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateStart );
#ifdef RAVEN_LIMIT_AIR_COMBO			/// 공중 채공 콤보 방지용
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_AIR_EventProcess );
				stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_AIR_FrameMoveFuture );
#else  RAVEN_LIMIT_AIR_COMBO
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_EventProcess );
#endif RAVEN_LIMIT_AIR_COMBO
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_FrameMove );
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateEnd );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_ORDNANCE_PENETRATOR:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_StateEnd );

			m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_StateEnd );
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_LOOP;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_LOOP", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_LOOP_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_LOOP_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_LOOP_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_LOOP_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_FINISH;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_FINISH", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_LOOP_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_LOOP_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_FINISH_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_END;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_OVER_END;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_OVER_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::SI_SA_RWT_HARPOON_SPEAR:
		{
			m_LuaManager.MakeTableReference( "RSI_SA_RWT_HARPOON_SPEAR", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_StateEnd );

			m_LuaManager.MakeTableReference( "RSI_SA_RWT_HARPOON_SPEAR", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_StateEnd );

#ifdef SERV_RAVEN_VETERAN_COMMANDER		/// 오버 히트 추가로 인한 발사 스테이트 분리
				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= RSI_SA_RWT_HARPOON_SPEAR_FIRE;
					m_LuaManager.MakeTableReference( "RSI_SA_RWT_HARPOON_SPEAR_FIRE", stateData.stateID );				/// 일반 발사
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_FIRE_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT;
					m_LuaManager.MakeTableReference( "RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT", stateData.stateID );		/// 오버 히트 발사
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}
#endif SERV_RAVEN_VETERAN_COMMANDER
		} break;
	case CX2SkillTree::SI_SA_RWT_HELLFIRE_GATLING:
		{
			m_LuaManager.MakeTableReference( "RSI_SA_RWT_HELLFIRE_GATLING", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_StateEnd );

			m_LuaManager.MakeTableReference( "RSI_SA_RWT_HELLFIRE_GATLING", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_StateEnd );
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_HELLFIRE_GATLING_LOOP;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_HELLFIRE_GATLING_LOOP", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_LOOP_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_LOOP_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_LOOP_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_LOOP_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_HELLFIRE_GATLING_END;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_HELLFIRE_GATLING_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_HELLFIRE_GATLING_OVER_END;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_HELLFIRE_GATLING_OVER_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_HELLFIRE_GATLING_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	case CX2SkillTree::SI_SA_RWT_GIGA_PROMINENCE:
		{
			m_LuaManager.MakeTableReference( "RSI_SA_RWT_GIGA_PROMINENCE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_StateEnd );

			m_LuaManager.MakeTableReference( "RSI_SA_RWT_GIGA_PROMINENCE", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_StateEnd );

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_GIGA_PROMINENCE_LOOP;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_GIGA_PROMINENCE_LOOP", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_LOOP_StateStart );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_LOOP_EventProcess );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_LOOP_FrameMove );
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_LOOP_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_GIGA_PROMINENCE_FINISH;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_GIGA_PROMINENCE_FINISH", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_FINISH_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_FINISH_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_GIGA_PROMINENCE_FINISH", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_GIGA_PROMINENCE_END;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_GIGA_PROMINENCE_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= RSI_SA_RWT_GIGA_PROMINENCE_OVER_END;
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_GIGA_PROMINENCE_OVER_END", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_OVER_END_StateStart );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_GIGA_PROMINENCE_OVER_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				}
			} break;
#endif RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		case CX2SkillTree::SI_SA_RVC_DEADLY_RAID:
			{
				m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_Init );
				normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_StateStart );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_EventProcess );	

				m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID", hyperStateData.stateID );
				hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_StateStart );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_EventProcess );

				{
					UserUnitStateData stateData;

					stateData.Init();		/// 일반 타격
					stateData.stateID			= RVC_SI_SA_DEADLY_RAID_LOOP;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID_LOOP", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();		/// 오버 히트 타격
					stateData.stateID			= RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();		/// 막타 전 일반 타격
					stateData.stateID			= RVC_SI_SA_DEADLY_RAID_LOOP_END;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID_LOOP_END", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_END_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_END_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_END_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();		/// 막타 전 오버 히트 타격
					stateData.stateID			= RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();		/// 막타
					stateData.stateID			= RVC_SI_SA_DEADLY_RAID_FINISH;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID_FINISH", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FINISH_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FINISH_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FINISH_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();		/// 오버 히트 막타
					stateData.stateID			= RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();		/// 오버 히트 마무리 동작
					stateData.stateID			= RVC_SI_SA_DEADLY_RAID_END;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_DEADLY_RAID_END", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_DEADLY_RAID_END_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}
			} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			case CX2SkillTree::SI_SA_RVC_IGNITION_CROW_NAPALM:
#else //UPGRADE_SKILL_SYSTEM_2013
			case CX2SkillTree::SI_SA_RVC_IGNITION_CROW:
#endif //UPGRADE_SKILL_SYSTEM_2013
			{
				m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_Init );
				normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_StateStart );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_EventProcess );	
				normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_StateEnd );

				m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW", hyperStateData.stateID );
				hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_StateStart );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_EventProcess );
				hyperStateData.StateEnd				= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_StateEnd );

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= RVC_SI_SA_IGNITION_CROW_FIRE;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_FIRE", stateData.stateID );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_FIRE_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_FIRE_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RVC_SI_SA_IGNITION_CROW_OVERHEAT_END;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_OVERHEAT_END", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_OVERHEAT_END_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}
			} break;


#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		case CX2SkillTree::SI_A_ROT_ARMOR_BREAK:
			{
				// 아머 브레이커 추가
				m_LuaManager.MakeTableReference( "RSI_A_ROT_ARMOR_BREAK", normalStateData.stateID ); 				
				normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_ARMOR_BREAK_Init );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_ARMOR_BREAK_EventProcess );				
				
				m_LuaManager.MakeTableReference( "RSI_A_ROT_ARMOR_BREAK", hyperStateData.stateID ); 				
				hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_ARMOR_BREAK_Init );
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_ROT_ARMOR_BREAK_EventProcess );				
				
				hyperStateData.m_bHyperState = true;
			} break;

		case CX2SkillTree::SI_SA_RVC_IGNITION_CROW_INCINERATION:
			{
				// 이그니션 크로우 인시너레이션
				m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_INCINERATION", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_Init );
				normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_StateStart );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_EventProcess );	

				m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_INCINERATION", hyperStateData.stateID );
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_Init );
				hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_StateStart );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_EventProcess );	

				hyperStateData.m_bHyperState = true;

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END;
					m_LuaManager.MakeTableReference( "RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}
			} break;

		// 리볼버 캐논 HE
		case CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_HIGH_EXPLOSIVE_SHELL:
			{
				m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_HE", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_Init );
				normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_StateStart );
				normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_FrameMove );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_EventProcess );

				m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_HE", hyperStateData.stateID );
				hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_Init );
				hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_StateStart );
				hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_FrameMove );
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_EventProcess );

				hyperStateData.m_bHyperState = true;

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP;
					m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH;
					m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH", stateData.stateID );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				
					stateData.Init();
					stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_HE_END;
					m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_HE_END", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_END_EventProcess );
					m_StateList[stateData.stateID] = stateData;
					
					stateData.Init();
					stateData.stateID			= RSI_SA_RWT_REVOLVER_CANNON_HE_OVER_END;
					m_LuaManager.MakeTableReference( "RSI_SA_RWT_REVOLVER_CANNON_HE_OVER_END", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_SA_RWT_REVOLVER_CANNON_HE_END_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}
			} break;
		// 리미트 크러셔
		case CX2SkillTree::SI_A_RRF_LIMIT_CRUSHER:
			{
				m_LuaManager.MakeTableReference( "RSI_A_RRF_LIMIT_CRUSHER_CHARGE", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_LIMIT_CRUSHER_CHARGE_Init );
				normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateStartFuture );
				normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateStart );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_LIMIT_CRUSHER_CHARGE_EventProcess );	
				normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateEnd );

				m_LuaManager.MakeTableReference( "RSI_A_RRF_LIMIT_CRUSHER_CHARGE", hyperStateData.stateID );
				hyperStateData.StateInit			= normalStateData.StateInit;
				hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;	
				hyperStateData.m_bHyperState = true;

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK;
					m_LuaManager.MakeTableReference( "RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RSI_A_RRF_LIMIT_CRUSHER_AVOID_ATTACK;
					m_LuaManager.MakeTableReference( "RSI_A_RRF_LIMIT_CRUSHER_AVOID_ATTACK", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_LIMIT_CRUSHER_AVOID_ATTACK_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}
			}
			break;

		// 헬 다이브
		case CX2SkillTree::SI_A_RRF_HELL_DIVE:
			{
				m_LuaManager.MakeTableReference( "RSI_A_RRF_HELL_DIVE", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_HELL_DIVE_Init );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_HELL_DIVE_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_HELL_DIVE_EventProcess );	

				m_LuaManager.MakeTableReference( "RSI_A_RRF_HELL_DIVE", hyperStateData.stateID );
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_HELL_DIVE_Init );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_HELL_DIVE_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RRF_HELL_DIVE_EventProcess );	
				hyperStateData.m_bHyperState = true;
			}
			break;

		// 네이팜 그레네이드
		case CX2SkillTree::SI_A_RVC_NAPALM_GRENADE :
			{
				m_LuaManager.MakeTableReference( "RSI_A_RVC_NAPALM_GRENADE", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_RVC_NAPALM_GRENADE_Init );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RVC_NAPALM_GRENADE_EventProcess );
				normalStateData.OnFrameMove	= SET_CB_FUNC( CX2GURaven, RSI_A_RVC_NAPALM_GRENADE_FrameMove );

				m_LuaManager.MakeTableReference( "RSI_A_RVC_NAPALM_GRENADE", hyperStateData.stateID );
				hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_RVC_NAPALM_GRENADE_Init );
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RVC_NAPALM_GRENADE_EventProcess );
				hyperStateData.OnFrameMove	= SET_CB_FUNC( CX2GURaven, RSI_A_RVC_NAPALM_GRENADE_FrameMove );

				hyperStateData.m_bHyperState = true;
			}
			break;
		
		// 화염인
		case CX2SkillTree::SI_A_RWT_FLAME_SWORD :
			{
				m_LuaManager.MakeTableReference( "RSI_A_RWT_FLAME_SWORD", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_Init );
				normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_StateStart );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_EventProcess );
				m_LuaManager.MakeTableReference( "RSI_A_RWT_FLAME_SWORD", hyperStateData.stateID );
				hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_Init );
				hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_StateStart );
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_EventProcess );
				hyperStateData.m_bHyperState = true;

				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RSI_A_RWT_FLAME_SWORD_GROUND;
				m_LuaManager.MakeTableReference( "RSI_A_RWT_FLAME_SWORD_GROUND", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_GROUND_StateStart );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_GROUND_EventProcess );
				m_StateList[stateData.stateID] = stateData;


				stateData.Init();
				stateData.stateID			= RSI_A_RWT_FLAME_SWORD_FLY;
				m_LuaManager.MakeTableReference( "RSI_A_RWT_FLAME_SWORD_FLY", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_AIR_StateStart );	
				stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_AIR_FrameMoveFuture );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_A_RWT_FLAME_SWORD_AIR_EventProcess );		
				m_StateList[stateData.stateID] = stateData;
			}
			break;

		case CX2SkillTree::SI_SA_RST_SHOCKWAVE_FRONT :
			{
				/// 쇼크 웨이브 - 속
				m_LuaManager.MakeTableReference( "RSI_SA_RST_SHOCKWAVE_FRONT", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_Init );
				normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_StateStart );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_FrameMove );				
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_EventProcess );				
				normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_StateEnd );

				m_LuaManager.MakeTableReference( "RSI_SA_RST_SHOCKWAVE_FRONT", hyperStateData.stateID );
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_Init );
				hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_StateStart );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_FrameMove );				
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_EventProcess );				
				hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_SA_RST_SHOCKWAVE_FRONT_StateEnd );

				hyperStateData.m_bHyperState = true;
			}
			break;

		case CX2SkillTree::SI_A_RBM_SONIC_SLASH :
			{
				/// 소닉 슬래시
				m_LuaManager.MakeTableReference( "RSI_A_RBM_SONIC_SLASH_LOOP", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_Init );
				normalStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_StateStart );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_EventProcess );
				
				m_LuaManager.MakeTableReference( "RSI_A_RBM_SONIC_SLASH_LOOP", hyperStateData.stateID );
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_Init );
				hyperStateData.StateStart			= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_StateStart );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_LOOP_EventProcess );
				hyperStateData.m_bHyperState = true;

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID				= RSI_A_RBM_SONIC_SLASH_FINISH;
					m_LuaManager.MakeTableReference( "RSI_A_RBM_SONIC_SLASH_FINISH", stateData.stateID );				/// 일반 발사
					stateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RBM_SONIC_SLASH_FINISH_EventProcess );
					m_StateList[stateData.stateID]	= stateData;
				}
			}
			break;

		case CX2SkillTree::SI_SA_RBM_GIGA_DRIVE_SEISMIC_TREMOR :
			{
				/// 기가 드라이브 - 폭
				m_LuaManager.MakeTableReference( "RSI_SA_RBM_GIGA_DRIVE_BURST", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_BURST_Init );		
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_BURST_FrameMove );				
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_BURST_EventProcess );
				m_LuaManager.MakeTableReference( "RSI_SA_RBM_GIGA_DRIVE_BURST", hyperStateData.stateID );
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_BURST_Init );		
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_BURST_FrameMove );				
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_RBM_GIGA_DRIVE_BURST_EventProcess );
				hyperStateData.m_bHyperState = true;
			}
			break;

		case CX2SkillTree::SI_A_ROT_CHARGED_BOLT_BLOOD :
			{
				m_LuaManager.MakeTableReference( "RSI_SA_ROT_CHARGED_BOLT_BLOOD", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_CHARGED_BOLT_BLOOD_Init );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_CHARGED_BOLT_BLOOD_FrameMove );				
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_CHARGED_BOLT_BLOOD_EventProcess );
				
				m_LuaManager.MakeTableReference( "RSI_SA_ROT_CHARGED_BOLT_BLOOD", hyperStateData.stateID );
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_CHARGED_BOLT_BLOOD_Init );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_CHARGED_BOLT_BLOOD_FrameMove );				
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_SA_ROT_CHARGED_BOLT_BLOOD_EventProcess );

				hyperStateData.m_bHyperState = true;
			}
			break;
		case CX2SkillTree::SI_A_RF_MEGADRILL_BREAK :
			{
				m_LuaManager.MakeTableReference( "RSI_A_RF_MEGADRILL_BREAK", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_Init );
				normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_FrameMoveFuture );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_EventProcess );
				normalStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_End );
				
				m_LuaManager.MakeTableReference( "RSI_A_RF_MEGADRILL_BREAK", hyperStateData.stateID );
				hyperStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_Init );
				hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_FrameMoveFuture );
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_EventProcess );
				hyperStateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_A_RF_MEGADRILL_BREAK_End );
				hyperStateData.m_bHyperState = true;
			}
			break;
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		
		case CX2SkillTree::SI_A_RVC_BLEEDING_SLICER:
			{
			
				m_LuaManager.MakeTableReference( "RVC_SI_A_BLEEDING_SLICER", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_Init );
				normalStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_StateStart );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_EventProcess );

				m_LuaManager.MakeTableReference( "RVC_SI_A_BLEEDING_SLICER", hyperStateData.stateID );
				hyperStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_Init );
				hyperStateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_StateStart );
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_EventProcess );


				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= RVC_SI_A_BLEEDING_SLICER_GROUND;
				m_LuaManager.MakeTableReference( "RVC_SI_A_BLEEDING_SLICER_GROUND", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_GROUND_StateStart );	
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_GROUND_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_GROUND_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_GROUND_StateEnd );	
				m_StateList[stateData.stateID] = stateData;


				stateData.Init();
				stateData.stateID			= RVC_SI_A_BLEEDING_SLICER_FLY;
				m_LuaManager.MakeTableReference( "RVC_SI_A_BLEEDING_SLICER_FLY", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_AIR_StateStart );	
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_AIR_FrameMove );	
				stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_AIR_FrameMoveFuture );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_AIR_EventProcess );		
				stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RVC_SI_A_BLEEDING_SLICER_AIR_StateEnd );	
				m_StateList[stateData.stateID] = stateData;

			} break;
#endif SERV_RAVEN_VETERAN_COMMANDER
#ifdef FINALITY_SKILL_SYSTEM //김창한
		case CX2SkillTree::SI_HA_RBM_EXTREM_BLADE:
			{
				m_LuaManager.MakeTableReference( "RSI_HA_RBM_EXTREM_BLADE_READY", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_READY_Init );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_READY_EventProcess );

				m_LuaManager.MakeTableReference( "RSI_HA_RBM_EXTREM_BLADE_READY", hyperStateData.stateID );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_READY_EventProcess );

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= RSI_HA_RBM_EXTREM_BLADE_ATTACK;
					m_LuaManager.MakeTableReference( "RSI_HA_RBM_EXTREM_BLADE_ATTACK", stateData.stateID );
					stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_Init );
					stateData.StateStart		= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_EventProcess );
					stateData.StateEnd			= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_StateEnd );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH;
					m_LuaManager.MakeTableReference( "RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH", stateData.stateID );
					stateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_Init );
					stateData.StateStartFuture	= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_StartFuture );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GURaven, RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}

			} break;

		case CX2SkillTree::SI_HA_RRF_INFERNAL_ARM:
			{
				m_LuaManager.MakeTableReference( "RSI_HA_RRF_INFERNAL_ARM", normalStateData.stateID );
				normalStateData.StateInit			= SET_CB_FUNC( CX2GURaven, RSI_HA_RRF_INFERNAL_ARM_Init );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_HA_RRF_INFERNAL_ARM_EventProcess );	

				m_LuaManager.MakeTableReference( "RSI_HA_RRF_INFERNAL_ARM", hyperStateData.stateID );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GURaven, RSI_HA_RRF_INFERNAL_ARM_EventProcess );
			} break;

		#pragma region SI_HA_RVC_BURNING_BUSTER
		case CX2SkillTree::SI_HA_RVC_BURNING_BUSTER:
			{
				m_LuaManager.MakeTableReference( "RSI_HA_RVC_BURNING_BUSTER", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GURaven, RSI_HA_RVC_BURNING_BUSTER_Init );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );

				m_LuaManager.MakeTableReference( "RSI_HA_RVC_BURNING_BUSTER", hyperStateData.stateID );
				hyperStateData.m_bHyperState	= true;
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
			} break;
		#pragma endregion 버닝 버스터 - 궁극기
#endif //FINALITY_SKILL_SYSTEM
	}
}



/*virtual*/ void CX2GURaven::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_RF_BURNING_RUSH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RF_BURNING_RUSH );
		if( NULL != pSkillTemplet )
		{	
			
			UserUnitStateData& stateDataDash = m_StateList[ RSI_SUPER_DASH ];
			stateDataDash.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataDash.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataDash.m_eSkillID = pSkillTemplet->m_eID;
			
			UserUnitStateData& stateDataDashEnd = m_StateList[ RSI_SUPER_DASH_END ];
			stateDataDashEnd.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataDashEnd.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataDashEnd.m_eSkillID = pSkillTemplet->m_eID;
	
			float fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, pSkillTemplet->m_iSkillLevel );
		
		}
	}
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SKILL_30_TEST
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ROT_VALKYRIESJAVELIN  == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ROT_VALKYRIESJAVELIN );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			{
				UserUnitStateData& normalStateData = m_StateList[ RSI_SA_ROT_VALKYRIESJAVELIN_LANDING ];
				normalStateData.m_SPLevel =iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_LANDING ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}	
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ RSI_SA_ROT_VALKYRIESJAVELIN_LANDING ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_LANDING ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}	
	#endif // UPGRADE_SKILL_SYSTEM_2013	
		}
	}
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_RBM_WOFL_FANG == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RBM_WOFL_FANG == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RBM_WOFL_FANG );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RBM_WOFL_FANG );
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			{
				UserUnitStateData& normalStateData = m_StateList[ SI_SA_RBM_WOLF_FANG_HIT ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ SI_SA_RBM_WOLF_FANG_HIT_HYPER ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}	
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ SI_SA_RBM_WOLF_FANG_HIT ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ SI_SA_RBM_WOLF_FANG_HIT_HYPER ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}	
	#endif // UPGRADE_SKILL_SYSTEM_2013	
		}
	}
#endif

//{{ kimhc // 2010.11.15 // 레피-와일드차지
#ifdef	NEW_SKILL_2010_11
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RRF_WILD_CHARGE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RRF_WILD_CHARGE );
		if ( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			{
				UserUnitStateData& normalStateData = m_StateList[ RSI_SA_RRF_WILD_CHARGING ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}		

			{
				UserUnitStateData& normalStateData = m_StateList[ RSI_SA_RRF_WILD_CHARGE_DASH ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ RSI_SA_RRF_WILD_CHARGING ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}		

			{
				UserUnitStateData& normalStateData = m_StateList[ RSI_SA_RRF_WILD_CHARGE_DASH ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013		
		}
	}
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.15 // 레피-와일드차지


#ifdef RAVEN_WEAPON_TAKER
	// 연무 폭파 
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataStand = m_StateList[ RSI_A_RWT_BARRAGE_ATTACK_STAND ];
			stateDataStand.m_SPLevel = iSkillTempletLevel;
			stateDataStand.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataStand.m_eSkillID = pSkillTemplet->m_eID;

			// 앞서 저장해 둔 에어슬라이서 데미지 보정을 추가
			UserUnitStateData& stateDataAir = m_StateList[ RSI_A_RWT_BARRAGE_ATTACK_AIR ];
			stateDataAir.m_SPLevel = iSkillTempletLevel;
			stateDataAir.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel ) * m_fAirSlicerDamageRel;
			stateDataAir.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataStand = m_StateList[ RSI_A_RWT_BARRAGE_ATTACK_STAND ];
			stateDataStand.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataStand.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataStand.m_eSkillID = pSkillTemplet->m_eID;

			// 앞서 저장해 둔 에어슬라이서 데미지 보정을 추가
			UserUnitStateData& stateDataAir = m_StateList[ RSI_A_RWT_BARRAGE_ATTACK_AIR ];
			stateDataAir.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataAir.m_fPowerRate = pSkillTemplet->m_fPowerRate * m_fAirSlicerDamageRel;
			stateDataAir.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	// 버스팅 블레이드
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RWT_BURSTING_BLADE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RWT_BURSTING_BLADE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataStand = m_StateList[ RSI_SA_RWT_BURSTING_BLADE_STAND ];
			stateDataStand.m_SPLevel = iSkillTempletLevel;
			stateDataStand.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataStand.m_eSkillID = pSkillTemplet->m_eID;

			// 앞서 저장해 둔 에어슬라이서 데미지 보정을 추가
			UserUnitStateData& stateDataAir = m_StateList[ RSI_SA_RWT_BURSTING_BLADE_AIR ];
			stateDataAir.m_SPLevel = iSkillTempletLevel;
			stateDataAir.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel ) * m_fAirSlicerDamageRel;
			stateDataAir.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataStand = m_StateList[ RSI_SA_RWT_BURSTING_BLADE_STAND ];
			stateDataStand.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataStand.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataStand.m_eSkillID = pSkillTemplet->m_eID;

			// 앞서 저장해 둔 에어슬라이서 데미지 보정을 추가
			UserUnitStateData& stateDataAir = m_StateList[ RSI_SA_RWT_BURSTING_BLADE_AIR ];
			stateDataAir.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataAir.m_fPowerRate = pSkillTemplet->m_fPowerRate * m_fAirSlicerDamageRel;
			stateDataAir.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	// 리볼버 캐논
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_ORDNANCE_PENETRATOR == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_ORDNANCE_PENETRATOR );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON );
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_LOOP ];
			stateDataLoop.m_SPLevel = iSkillTempletLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_FINISH ];
			stateDataFinish.m_SPLevel = iSkillTempletLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_LOOP ];
			stateDataLoop.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_FINISH ];
			stateDataFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	// 헬파이어 개틀링
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RWT_HELLFIRE_GATLING == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RWT_HELLFIRE_GATLING );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateData = m_StateList[ RSI_SA_RWT_HELLFIRE_GATLING_LOOP ];
			stateData.m_SPLevel = iSkillTempletLevel;
			stateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateData.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateData = m_StateList[ RSI_SA_RWT_HELLFIRE_GATLING_LOOP ];
			stateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateData.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	// 기가 프로미넌스
	
	// 헬파이어 개틀링
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RWT_GIGA_PROMINENCE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RWT_GIGA_PROMINENCE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_GIGA_PROMINENCE_LOOP ];
			stateDataLoop.m_SPLevel = iSkillTempletLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_GIGA_PROMINENCE_FINISH ];
			stateDataFinish.m_SPLevel = iSkillTempletLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataOverFinish = m_StateList[ RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH ];
			stateDataOverFinish.m_SPLevel = iSkillTempletLevel;
			stateDataOverFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataOverFinish.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_GIGA_PROMINENCE_LOOP ];
			stateDataLoop.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_GIGA_PROMINENCE_FINISH ];
			stateDataFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataOverFinish = m_StateList[ RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH ];
			stateDataOverFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataOverFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataOverFinish.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#endif RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	/// 데들리 레이드
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RVC_DEADLY_RAID == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RVC_DEADLY_RAID );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataLoop = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP ];
			stateDataLoop.m_SPLevel = iSkillTempletLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataLoopOverheat = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT ];
			stateDataLoopOverheat.m_SPLevel = iSkillTempletLevel;
			stateDataLoopOverheat.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoopOverheat.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataLoopEnd = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP_END ];
			stateDataLoopEnd.m_SPLevel = iSkillTempletLevel;
			stateDataLoopEnd.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoopEnd.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataLoopEndOverHeat = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT ];
			stateDataLoopEndOverHeat.m_SPLevel = iSkillTempletLevel;
			stateDataLoopEndOverHeat.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoopEndOverHeat.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RVC_SI_SA_DEADLY_RAID_FINISH ];
			stateDataFinish.m_SPLevel = iSkillTempletLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinishOverHeat = m_StateList[ RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT ];
			stateDataFinishOverHeat.m_SPLevel = iSkillTempletLevel;
			stateDataFinishOverHeat.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFinishOverHeat.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataLoop = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP ];
			stateDataLoop.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataLoopOverheat = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT ];
			stateDataLoopOverheat.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoopOverheat.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoopOverheat.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataLoopEnd = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP_END ];
			stateDataLoopEnd.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoopEnd.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoopEnd.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataLoopEndOverHeat = m_StateList[ RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT ];
			stateDataLoopEndOverHeat.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoopEndOverHeat.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoopEndOverHeat.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RVC_SI_SA_DEADLY_RAID_FINISH ];
			stateDataFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinishOverHeat = m_StateList[ RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT ];
			stateDataFinishOverHeat.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinishOverHeat.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFinishOverHeat.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	/// 치명상
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_RVC_BLEEDING_SLICER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RVC_BLEEDING_SLICER);
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataLoop = m_StateList[ RVC_SI_A_BLEEDING_SLICER_GROUND ];
			stateDataLoop.m_SPLevel = iSkillTempletLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RVC_SI_A_BLEEDING_SLICER_FLY ];
			stateDataFinish.m_SPLevel = iSkillTempletLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel ) * m_fAirSlicerDamageRel;		/// 에어 슬라이서 추가 데미지 보정
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataLoop = m_StateList[ RVC_SI_A_BLEEDING_SLICER_GROUND ];
			stateDataLoop.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RVC_SI_A_BLEEDING_SLICER_FLY ];
			stateDataFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate * m_fAirSlicerDamageRel;		/// 에어 슬라이서 추가 데미지 보정
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	/// 하픈 스피어
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RWT_HARPOON_SPEAR == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RWT_HARPOON_SPEAR );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_HARPOON_SPEAR_FIRE ];
			stateDataLoop.m_SPLevel = iSkillTempletLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT ];
			stateDataFinish.m_SPLevel = iSkillTempletLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_HARPOON_SPEAR_FIRE ];
			stateDataLoop.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT ];
			stateDataFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	/// 이그니션 크로우
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RVC_IGNITION_CROW_NAPALM == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RVC_IGNITION_CROW == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RVC_IGNITION_CROW_NAPALM );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RVC_IGNITION_CROW );
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataFire = m_StateList[ RVC_SI_SA_IGNITION_CROW_FIRE ];
			stateDataFire.m_SPLevel =iSkillTempletLevel;
			stateDataFire.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFire.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFireOverHeat = m_StateList[ RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT ];
			stateDataFireOverHeat.m_SPLevel = iSkillTempletLevel;
			stateDataFireOverHeat.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFireOverHeat.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataFire = m_StateList[ RVC_SI_SA_IGNITION_CROW_FIRE ];
			stateDataFire.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFire.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFire.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFireOverHeat = m_StateList[ RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT ];
			stateDataFireOverHeat.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFireOverHeat.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFireOverHeat.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#endif SERV_RAVEN_VETERAN_COMMANDER


#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈


	/// 화염인
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_RWT_FLAME_SWORD == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RWT_FLAME_SWORD);
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_A_RWT_FLAME_SWORD_GROUND ];
			stateDataLoop.m_SPLevel = iSkillTempletLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_A_RWT_FLAME_SWORD_FLY ];
			stateDataFinish.m_SPLevel = iSkillTempletLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel ) * m_fAirSlicerDamageRel;		/// 에어 슬라이서 추가 데미지 보정
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_A_RWT_FLAME_SWORD_GROUND ];
			stateDataLoop.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_A_RWT_FLAME_SWORD_FLY ];
			stateDataFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate * m_fAirSlicerDamageRel;		/// 에어 슬라이서 추가 데미지 보정
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}



	// 리볼버 캐논 HE
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_HIGH_EXPLOSIVE_SHELL == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_HIGH_EXPLOSIVE_SHELL );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP ];
			stateDataLoop.m_SPLevel = iSkillTempletLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH ];
			stateDataFinish.m_SPLevel = iSkillTempletLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	
			UserUnitStateData& stateDataOverEnd = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_HE_OVER_END ];
			stateDataOverEnd.m_SPLevel = iSkillTempletLevel;
			stateDataOverEnd.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataOverEnd.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataEnd = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_HE_END ];
			stateDataEnd.m_SPLevel = iSkillTempletLevel;
			stateDataEnd.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataEnd.m_eSkillID = pSkillTemplet->m_eID;



	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataLoop = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP ];
			stateDataLoop.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataLoop.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataLoop.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFinish = m_StateList[ RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH ];
			stateDataFinish.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFinish.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFinish.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}


	/// 이그니션 크로우 이그니션
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_RVC_IGNITION_CROW_INCINERATION == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RVC_IGNITION_CROW_INCINERATION );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateDataFire = m_StateList[ RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE ];
			stateDataFire.m_SPLevel = iSkillTempletLevel;
			stateDataFire.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFire.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFireOverHeat = m_StateList[ RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT ];
			stateDataFireOverHeat.m_SPLevel = iSkillTempletLevel;
			stateDataFireOverHeat.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFireOverHeat.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFireOverHeatEnd = m_StateList[ RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END ];
			stateDataFireOverHeatEnd.m_SPLevel = iSkillTempletLevel;
			stateDataFireOverHeatEnd.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateDataFireOverHeatEnd.m_eSkillID = pSkillTemplet->m_eID;


			    

	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateDataFire = m_StateList[ RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE ];
			stateDataFire.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFire.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFire.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& stateDataFireOverHeat = m_StateList[ RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT ];
			stateDataFireOverHeat.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			stateDataFireOverHeat.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			stateDataFireOverHeat.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	if( true == bChangeAll_ || CX2SkillTree::SI_A_RBM_SONIC_SLASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RBM_SONIC_SLASH );
		SetEquippedSkillLevelStateData( pSkillTemplet, RSI_A_RBM_SONIC_SLASH_FINISH );
	}

	if( true == bChangeAll_ || CX2SkillTree::SI_A_RRF_LIMIT_CRUSHER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RRF_LIMIT_CRUSHER );
		SetEquippedSkillLevelStateData( pSkillTemplet, RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK );
		SetEquippedSkillLevelStateData( pSkillTemplet, RSI_A_RRF_LIMIT_CRUSHER_AVOID_ATTACK );
	}

#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
#ifdef FINALITY_SKILL_SYSTEM //김창한
	//익스트림 블레이드
	if ( true == bChangeAll_ || CX2SkillTree::SI_HA_RBM_EXTREM_BLADE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_HA_RBM_EXTREM_BLADE );
		SetEquippedSkillLevelStateData( pSkillTemplet, RSI_HA_RBM_EXTREM_BLADE_ATTACK );
		SetEquippedSkillLevelStateData( pSkillTemplet, RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH );
	}
#endif //FINALITY_SKILL_SYSTEM
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GURaven::AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

void CX2GURaven::InitComponent()
{
	CX2GUUser::InitComponent();

	if( m_LuaManager.BeginTable( "INIT_COMPONENT" ) == true )
	{
		float fSlashTraceDisableTime;
		float fSlashTraceTipDisableTime;
		LUA_GET_VALUE( m_LuaManager, "SLASH_TRACE_DISABLE_TIME",		fSlashTraceDisableTime,		0.4f );
		LUA_GET_VALUE( m_LuaManager, "SLASH_TRACE_TIP_DISABLE_TIME",	fSlashTraceTipDisableTime,	0.5f );
		int slashTraceDrawCount;
		int slashTraceTipDrawCount;
		LUA_GET_VALUE( m_LuaManager, "SLASH_TRACE_DRAW_COUNT",			slashTraceDrawCount,		1 );
		LUA_GET_VALUE( m_LuaManager, "SLASH_TRACE_TIP_DRAW_COUNT",		slashTraceTipDrawCount,		3 );

	

#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
		AddUnitSlashData( CX2UnitSlashTraceManager::STC_NONE );
#endif UNIT_SLASH_TRACE_MANAGER_TEST

		m_LuaManager.EndTable();
	}

	m_fOldHp = GetMaxHp();
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GURaven::AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	CX2GURaven::AppendMajorParticleToDeviceList( listInOut_ );
	CX2GURaven::AppendMinorParticleToDeviceList( listInOut_ );
	CX2GameUnit::Weapon::AppendEnchantParticleForSwordToDeviceList( listInOut_, eDamageType_ );
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

void CX2GURaven::InitEffect()
{
	CX2GUUser::InitEffect();

	CreateMajorParticleArray();
	CreateMinorParticleArray();
}




/*virtual*/ void CX2GURaven::InitEnchantWeaponEffectUnitSpecific() 
{
	for( UINT i=0; i<m_vecpWeapon.size(); ++i )
	{
		Weapon* pWeapon = m_vecpWeapon[i];
		pWeapon->InitEnchantParticleForSword();
	}
}

void CX2GURaven::DeleteRavenMajorParticle()
{
	CX2GUUser::DeleteGUUserMajorParticle();

	CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();

	for ( int index = 0; index < RAVEN_MAJOR_PII_END; index++ )
	{
		pMajorParticleSystem->DestroyInstanceHandle( m_ahRavenMajorParticleInstance[index] );
	}
}

void CX2GURaven::DeleteRavenMinorParticle()
{
	CX2GUUser::DeleteGUUserMinorParticle();
	
	CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMinorParticle();

	for ( int index = 0; index < RAVEN_MINOR_PII_END; index++ )
	{
		pMajorParticleSystem->DestroyInstanceHandle( m_ahRavenMinorParticleInstance[index] );
	}
}

void CX2GURaven::InitializeRavenMajorParticleArray()
{
	CX2GUUser::InitializeGUUserMajorParticleArray();

	for ( int index = 0; index < RAVEN_MAJOR_PII_END; index++ )
	{
		m_ahRavenMajorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GURaven::AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
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
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD



/*virtual*/ void CX2GURaven::CreateMajorParticleArray()
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
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RED,			L"DamageImpactRed",		2 );
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

CKTDGParticleSystem::CParticleEventSequence* CX2GURaven::SetRavenMajorParticleByEnum( RAVEN_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ /*= -1*/ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleRavenMajorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), 0,0,0, 0, 0, iDrawCount_, 0 );

		SetHandleRavenMajorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleRavenMajorParticleByEnum( eVal_) );

	return pSeq;
}

void CX2GURaven::InitializeRavenMinorParticleArray()
{
	CX2GUUser::InitializeGUUserMinorParticleArray();

	for ( int index = 0; index < RAVEN_MINOR_PII_END; index++ )
	{
		m_ahRavenMinorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GURaven::AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}

#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

/*virtual*/ void CX2GURaven::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();
}

CKTDGParticleSystem::CParticleEventSequence* CX2GURaven::SetRavenMinorParticleByEnum( RAVEN_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ /*= -1*/ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleRavenMinorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), 0,0,0, 0, 0, iDrawCount_, 0 );

		SetHandleRavenMinorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleRavenMinorParticleByEnum( eVal_) );

	return pSeq;
}

void CX2GURaven::CommonStateStartFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateStartFuture();
}

void CX2GURaven::CommonStateStart()
{
	KTDXPROFILE();

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME",				m_fDamageDataChangeTime,			0.f );
	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME1",			m_fDamageDataChangeTime1,			0.f );
	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME2",			m_fDamageDataChangeTime2,			0.f );
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef RAVEN_SECOND_CLASS_CHANGE
	bool bChangeDamageType = false;
	ResetDamageType();
	LUA_GET_VALUE( m_LuaManager, "BURNING_NASOD_HAND_POSSIBLE",	bChangeDamageType,	false );
	if( bChangeDamageType == true )
	{
		ChangeDamageType(m_eDamageTypeRaven, m_fChangeDamageRelRaven);
	}

#ifndef BALANCE_BLADE_MASTER_20130117		/// 크리티컬 증가 패시브를 모든 공격에 적용

	bool bPassiveCriticalRate = false;
	ResetPassiveCriticalRate();
	LUA_GET_VALUE( m_LuaManager, "BLADE_MASTERY",	bPassiveCriticalRate,	false );
	if( bPassiveCriticalRate == true )

#endif  BALANCE_BLADE_MASTER_20130117
	{
		SetPassiveCriticalRate(m_fPassiveCriticalRateRaven);
	}

#endif

	CX2GUUser::CommonStateStart();
}

void CX2GURaven::CommonFrameMoveFuture()
{
	KTDXPROFILE();
	m_fReAttackZ1Time -= m_fElapsedTime;
	if( m_fReAttackZ1Time < 0.0f )
		m_fReAttackZ1Time = 0.0f;

	m_fReDashJumpXTime -= m_fElapsedTime;
	if( m_fReDashJumpXTime < 0.0f )
		m_fReDashJumpXTime = 0.0f;

	CX2GUUser::CommonFrameMoveFuture();

#ifdef UPGRADE_RAVEN
	m_FrameDataFuture.syncData.m_CannonBallCount = GetNasodBall();
#endif
}

void CX2GURaven::CommonFrameMove()
{
	KTDXPROFILE();

	CX2GUUser::CommonFrameMove();

#ifdef OLD_HAND_SLASH_TRACE
//{{AFX

	bool bHandSlash = false;
	LUA_GET_VALUE( m_LuaManager, "HAND_SLASH_TRACE", bHandSlash, false );

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_bEffect == true )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	{
		m_pHandSlashTrace->SetShowObject( true );
		m_pHandSlashTraceTip->SetShowObject( true );

		m_pHandSlashTrace->OnFrameMove( m_fTime, m_fElapsedTime );
		m_pHandSlashTrace->GetMatrix().Move( GetMatrix().GetPos() );

		m_pHandSlashTraceTip->OnFrameMove( m_fTime, m_fElapsedTime );
		m_pHandSlashTraceTip->GetMatrix().Move( GetMatrix().GetPos() );
	}
	else
	{
		m_pHandSlashTrace->SetShowObject( false );
		m_pHandSlashTraceTip->SetShowObject( false );
	}

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_bEffect == true )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	{
		D3DXVECTOR3 up = GetBonePos( L"ATTACK_SPHERE1_Lhand1" );
		D3DXVECTOR3 down = GetBonePos( L"Bip01_L_Hand" );
		D3DXVECTOR3 dir	= up - down;
		D3DXVec3Normalize( &dir, &dir );
		up += dir * 50.0f;

		if( up != D3DXVECTOR3(0.f,0.f,0.f) && down != D3DXVECTOR3(0.f,0.f,0.f) )
		{
			D3DXVECTOR3 dirTip	= down - up;
			D3DXVec3Normalize( &dirTip, &dirTip );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            D3DXVECTOR3 tipDown = up + dirTip * GetNowSlashTraceTipWide();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			D3DXVECTOR3 tipDown = up + dirTip * m_FrameDataNow.stateParam.fSlashTraceTipWide;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

			if( bHandSlash == true )
			{
				if( GetRemainHyperModeTime() > 0.0f )
				{
					m_pHandSlashTrace->AddSlashData( up, down, 0xccff3333 );
					m_pHandSlashTraceTip->AddSlashData( up, tipDown, 0xffff3333 );
				}
				else
				{
					m_pHandSlashTrace->AddSlashData( up, down, 0xcc5555ff );
					m_pHandSlashTraceTip->AddSlashData( up, tipDown, 0xff5555ff );
				}
			}
			else
			{
				if( GetRemainHyperModeTime() > 0.0f )
				{
					m_pHandSlashTrace->AddSlashData( up, down, 0x00ff3333 );
					m_pHandSlashTraceTip->AddSlashData( up, tipDown, 0x00ff3333 );
				}
				else
				{
					m_pHandSlashTrace->AddSlashData( up, down, 0x005555ff );
					m_pHandSlashTraceTip->AddSlashData( up, tipDown, 0x005555ff );
				}
			}
		}
	}
//}}AFX
#endif OLD_HAND_SLASH_TRACE




	D3DXVECTOR3 vDirVec = GetDirVector();
	D3DXVECTOR3 vZVec = GetZVector();
	D3DXVec3Normalize( &vDirVec, &vDirVec );

	for( UINT i=0; i<m_vecArcEnemyData.size(); i++ )
	{
		ArcEnemyData* pData = m_vecArcEnemyData[i];
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( NULL != pData && INVALID_DAMAGE_EFFECT_HANDLE != pData->m_hEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL != pData && NULL != pData->m_pEffect )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( pData->m_hEffect ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( pData->m_pEffect ) ? pData->m_pEffect : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{
				CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pEffect->GetMainEffect();
				if( NULL != pMeshInst )
				{
					D3DXVECTOR3 vTargetPos = GetPos();
					if( true == GetIsRight() )
					{
						vTargetPos += pData->m_vOffsetPos.x * vDirVec;
						vTargetPos.y += pData->m_vOffsetPos.y;
						vTargetPos += pData->m_vOffsetPos.z * vZVec;
					}
					else
					{
						vTargetPos -= pData->m_vOffsetPos.x * vDirVec;
						vTargetPos.y += pData->m_vOffsetPos.y;
						vTargetPos += pData->m_vOffsetPos.z * vZVec;
					}

					float coeff = 0.1f;
					switch( i )
					{
					default:
					case 0:
						{
							coeff = 0.05f;
						} break;
					case 1:
						{
							coeff = 0.06f;
						} break;
					case 2:
						{
							coeff = 0.07f;
						} break;
					case 3:
						{
							coeff = 0.08f;
						} break;
					}

					D3DXVECTOR3 vRotateDegree = GetRotateDegree() + pData->m_vOffsetRotate;
					pMeshInst->SetRotateDegree( vRotateDegree );
					pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );

					vTargetPos = vTargetPos * coeff + pMeshInst->GetPos() * ( 1.f-coeff );
					pMeshInst->SetPos( vTargetPos );

				}
			}
			else
			{
				//{{ kimhc // 2010.6.21 // 메모리해제 없이 erase 하는 부분에 메모리 해제 추가
				ClearArcEnemyData();
				//m_vecArcEnemyData.resize(0);
				//}} kimhc // 2010.6.21 // 메모리해제 없이 erase 하는 부분에 메모리 해제 추가
				
				break;
			}
		}		
	}

#ifdef UPGRADE_RAVEN
	
	if( GetHyperModeCount() == 3 )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

		if( m_hSeqHyperBall == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			D3DXVECTOR3 vHyperBallPos = D3DXVECTOR3(74.f * g_pKTDXApp->GetResolutionScaleX(), 60.f *g_pKTDXApp->GetResolutionScaleY(), 0.f);
			m_hSeqHyperBall = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"raven_powrUp_UI_01", vHyperBallPos );
			if( g_pX2Game->GetMajorParticle()->GetInstanceSequence(m_hSeqHyperBall ) != NULL )
			{
				pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence(m_hSeqHyperBall );
				pSeq->SetOverUI( true );
			}					
		}
		else
		{
			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence(m_hSeqHyperBall);
			if( pSeq != NULL )
			{
				D3DXVECTOR3 vHyperBallPos = D3DXVECTOR3(74.f * g_pKTDXApp->GetResolutionScaleX(), 60.f *g_pKTDXApp->GetResolutionScaleY(), 0.f);
				pSeq->SetPosition( vHyperBallPos );
			}				
		}

#ifdef DIALOG_SHOW_TOGGLE
		if( pSeq != NULL )
		{
			if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
				pSeq->SetShowObject(false);
			else
				pSeq->SetShowObject(true);
		}
#endif
	}
	else
	{
		if( m_hSeqHyperBall != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pX2Game->GetMajorParticle()->DestroyInstanceHandle(m_hSeqHyperBall);
		}	
	}

	DoFrameMoveNasodBall();
#endif

#ifdef BALANCE_PATCH_20110303
	if( m_fArcEnemyEachTime > 0.f )
	{
		m_fArcEnemyEachTime -= m_fElapsedTime;
	}
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
	if( GetNowHp() <= 0.f )
	{
		m_fBlackHoleTime = 0.f;
	}

	if( m_fBlackHoleTime > 0.f )
	{
		m_fBlackHoleTime -= m_fElapsedTime;

		if( m_fBlackHoleTime <= 0.f )
		{
			//m_iBlackHoleHitCount = 0;
			m_fBlackHoleTime = 0.f;
	#ifdef BALANCE_BLADE_MASTER_20130117
			m_eRavenCurrentStage = RSI_BASE;
	#endif BALANCE_BLADE_MASTER_20130117
			if( m_hBlackHoleEffectSet != INVALID_EFFECTSET_HANDLE )
				g_pX2Game->GetEffectSet()->StopEffectSet( m_hBlackHoleEffectSet );
		}
	}
	else
	{
		//m_iBlackHoleHitCount = 0;
		if( m_hBlackHoleEffectSet != INVALID_EFFECTSET_HANDLE )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hBlackHoleEffectSet );
	}
	
	
#endif

	// 오버히트 버프 시간 감소
#ifdef RAVEN_WEAPON_TAKER
	if( m_fOverheatBuffTime > 0.f )
	{
		m_fOverheatBuffTime -= m_fElapsedTime;

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		if( m_fOverheatBuffTime <= 0.f )
			DeleteOverHeatObject();
#endif SERV_RAVEN_VETERAN_COMMANDER
	}
	else
	{
		m_fOverheatBuffTime = 0.f;
	}
#endif RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	/// 레이븐 2차 전직 베테랑 커멘더 - 용병의 생존술
	if( true == m_bEnableSurvivalTechniqueOfMercenary && 0.5f >= GetNowHp() / GetMaxHp() )
	{
		if( false == m_bActiveSurvivalTechniqueOfMercenary )
		{
			m_bActiveSurvivalTechniqueOfMercenary = true;			/// 용병의 생존술 설정
			const CX2SkillTree::SkillTemplet* pSkillTempletSurvivalTechniqueOfMercenary 
				= GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY );

			if ( NULL != pSkillTempletSurvivalTechniqueOfMercenary && !pSkillTempletSurvivalTechniqueOfMercenary->m_vecBuffFactorPtr.empty() )
			{
				m_bEnableSurvivalTechniqueOfMercenary = true;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit()  )
					return;

				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTempletSurvivalTechniqueOfMercenary->m_eID, true ) );	/// 스킬 레벨
		
				/// 오버 히트 체력 제한 보정
				m_fLimitOverHeatHP		= _CONST_RAVEN_::OVERHEAT_HP_LIMIT * 
					pSkillTempletSurvivalTechniqueOfMercenary->GetSkillAbilityValue( CX2SkillTree::SA_OVERHEAT_HP, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				/// 오버 히트 체력 제한 보정
				m_fLimitOverHeatHP		= _CONST_RAVEN_::OVERHEAT_HP_LIMIT * pSkillTempletSurvivalTechniqueOfMercenary->GetSkillAbilityValue( CX2SkillTree::SA_OVERHEAT_HP );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				SetBuffFactorToGameUnit( pSkillTempletSurvivalTechniqueOfMercenary, 0 );
			}
		}
	}
	else
	{
		m_bActiveSurvivalTechniqueOfMercenary = false;				/// 용병의 생존술 비활성
	}

	/// 오버 히트 발생시 생성되는 구조물 위치 갱신
	if( m_hOverHeatObject != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hOverHeatObject );

		if( NULL != pMeshInst )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Forearm" );
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
		}
		else
		{
			m_hOverHeatObject = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	/// 오버 히트 발생시 생성되는 구조물 위치 갱신
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* m_pEffectOverHeatFire1 = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectOverHeatFire1 ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL != m_pEffectOverHeatFire1 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		D3DXVECTOR3 vPosition = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Forearm" );

		m_pEffectOverHeatFire1->SetPos( vPosition );
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* m_pEffectOverHeatFire2 = g_pX2Game->GetDamageEffect()->GetInstance( m_hEffectOverHeatFire2 ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL != m_pEffectOverHeatFire2 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		D3DXVECTOR3 vPosition = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		m_pEffectOverHeatFire2->SetPos( vPosition );
	}
#endif SERV_RAVEN_VETERAN_COMMANDER
}


/*virtual*/ void CX2GURaven::CommonEventProcess()
{
	KTDXPROFILE();
	CX2GUUser::CommonEventProcess();

	//{{ kimhc // 2010.10.5 //
#ifdef SKILL_FOR_CANCEL
	if ( true == CheckPressedSkillForCancel() )
		SpecialAttackEventProcess();
#endif SKILL_FOR_CANCEL
	//}} kimhc // 2010.10.5 //
}


void CX2GURaven::CommonStateEndFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEndFuture();
}

void CX2GURaven::CommonStateEnd()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEnd();
}

//{{ robobeg : 2008-10-28
//void CX2GURaven::CommonRender()
/*virtual*/
RENDER_HINT CX2GURaven::CommonRender_Prepare()
//}} robobeg : 2008-10-28
{
	KTDXPROFILE();
	if( m_fShadowStepTimeLeft > 0.f )
	{
		if( GetRemainHyperModeTime() > 0.0f )
		{
			m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
			m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_RED;
			m_RenderParam.outLineColor		= D3DXCOLOR(0, 0, 0, 0);
		}
		else
		{
			m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
			m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
			m_RenderParam.outLineColor		= D3DXCOLOR(0, 0, 0, 0);
		}
	}
	else
	{
// 		if( GetRemainHyperModeTime() > 0.0f )
// 		{
// 			m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
// 			m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_RED;
// 			m_RenderParam.outLineColor		= 0xffff5511;
// 		}
// 		else
// 		{
// 			m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
// 			m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
// 			m_RenderParam.outLineColor		= 0xffffffff;
// 		}
	}

//{{ robobeg : 2008-10-28
	//CX2GUUser::CommonRender();
    return CX2GUUser::CommonRender_Prepare();
//}} robobeg : 2008-10-28
}

/*virtual*/ bool CX2GURaven::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition /*= CX2SkillTree::ASUT_ONLY_ON_LINE*/ )
{
	KTDXPROFILE();

	const int INVALID_SKILL_SLOT_INDEX = -1;

	int iPressedSkillSlotIndex = INVALID_SKILL_SLOT_INDEX;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;
	CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree; // 유저가 배운 스킬 트리

	if ( false == CommonSpecialAttackEventProcess( accessUserSkillTree, pSkillSlotData, iPressedSkillSlotIndex ) )
		return false;

	if( NULL == pSkillSlotData )
		return false;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );
	if( NULL == pSkillTemplet )
		return false;

#ifdef RAVEN_WEAPON_TAKER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	const CX2SkillTree::SkillTemplet* pAirSlicerSkillTemplet 
		= accessUserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RVC_AIR_SLICER );
	if ( NULL != pAirSlicerSkillTemplet )
	{
		const int iAirSlicerLevel = max( 1, accessUserSkillTree.GetSkillLevel( pAirSlicerSkillTemplet->m_eID, true ) );	/// 스킬 레벨

		// 기존 에어슬라이서 레벨별 제한 조건을 모두 1로 변경
		if ( NULL != pAirSlicerSkillTemplet && 
			 iAirSlicerLevel >= 1 &&
			 ( pSkillTemplet->m_eID == CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK ||
			   pSkillTemplet->m_eID == CX2SkillTree::SI_SA_RWT_BURSTING_BLADE ||
			   pSkillTemplet->m_eID == CX2SkillTree::SI_A_RVC_BLEEDING_SLICER ||
			   pSkillTemplet->m_eID == CX2SkillTree::SI_A_RWT_FLAME_SWORD ) &&
			 ( eActiveSkillUseCondition == CX2SkillTree::ASUT_AIR || eActiveSkillUseCondition == CX2SkillTree::ASUT_AIR_ONLY ) )
		{
			eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND;
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pAirSlicerSkillTemplet 
		= accessUserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RWT_AIR_SLICER );

	if( NULL != pAirSlicerSkillTemplet &&
		( pSkillTemplet->m_eID == CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK && pAirSlicerSkillTemplet->m_iSkillLevel >= 1 ||
		pSkillTemplet->m_eID == CX2SkillTree::SI_SA_RWT_BURSTING_BLADE && pAirSlicerSkillTemplet->m_iSkillLevel >= 3 ) &&
		( eActiveSkillUseCondition == CX2SkillTree::ASUT_AIR || eActiveSkillUseCondition == CX2SkillTree::ASUT_AIR_ONLY ) )
	{
		eActiveSkillUseCondition = CX2SkillTree::ASUT_GROUND;
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#endif RAVEN_WEAPON_TAKER

#ifdef ADD_MEMO_1ST_CLASS //김창한
	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet->m_eActiveSkillUseCondtion ) )
#else //ADD_MEMO_1ST_CLASS
	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet ) )
#endif //ADD_MEMO_1ST_CLASS
		return false;

	// 공식대전에서 사용 가능한 스킬인지 검사
	if( false == CheckSkillUsePVPOfficial( pSkillTemplet->m_eID ) )
		return false;

#ifdef UPGRADE_RAVEN
	if( m_bParrying == true && pSkillTemplet->m_eID != CX2SkillTree::SI_A_RF_SHADOW_STEP )
	{		
		return false;
	}
#endif

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	if( true == m_bActiveShadowStepCombo )			/// 섀도우 스텝 계열 동작중
	{
		if( true != m_bEnableShadowBackSlide  )		/// 섀도우 백 슬라이드 패시브를 않찍었다면 패스
			return false;

		if( pSkillTemplet->m_eID != CX2SkillTree::SI_A_RF_SHADOW_STEP )		/// 셰도우 스텝 이외의 스킬을 사용하면 패스
			return false;

		if( m_iRVCComboLoopCount > _CONST_RAVEN_::MAX_ACTIVE_SHADOW_STEP )	/// 최대 적용 횟수 이상일 때 패스
			return false;

		if( false == FlushMp( 5.f ) )					/// 엠피 없어도 패스
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );

			return false;
		}

#ifdef BALANCE_BLADE_MASTER_20130117
		if( RBM_COMBO_ZZZXZ == m_eRavenCurrentStage )	/// 만약 캔슬시 블레이드 마스터 ZZZXZ라면, 마법체 소멸 시켜주자
		{
			m_fBlackHoleTime		= 0.f;
			m_iBlackHoleHitCount	= 0;
			m_eRavenCurrentStage	= RSI_BASE;
		}
#endif BALANCE_BLADE_MASTER_20130117

		return true;
	}
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

	float fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
	float fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, pSkillTemplet->m_iSkillLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013


#ifdef TOGGLE_UNLIMITED_SKILL_USE
#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ )
	if( false == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_OPERATOR ) || false == g_pMain->IsUnlimitedSkillUse() )
#endif //defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ )
#else //TOGGLE_UNLIMITED_SKILL_USE
#ifndef _SERVICE_
	if( false == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
#endif _SERVICE_
#endif //TOGGLE_UNLIMITED_SKILL_USE
	{
		if( pSkillSlotData->m_fCoolTimeLeft > 0.f )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_226 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}

		if ( GetNowMp() < fMPConsume )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}

#ifdef FINALITY_SKILL_SYSTEM //JHKang
		if ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
		{
			const int iItemNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetNumItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

			if( iItemNum <= 0 
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
				&& false == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence()
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
				)
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_26119 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
			}
		}
#endif //FINALITY_SKILL_SYSTEM
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	/// 스킬창에서 스킬 습득중이면, 스킬 사용 불가
	if (	NULL != g_pData &&
		NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() &&
		true == g_pData->GetUIManager()->GetUISkillTree()->GetNowLearnSkill() )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_25110 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}
#endif // UPGRADE_SKILL_SYSTEM_2013

	//{{ JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련
#ifdef DUNGEON_RANK_NEW
	CountUsedSkill( iPressedSkillSlotIndex );
#endif DUNGEON_RANK_NEW
	//}} JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	float fSkillCoolTime = pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
	float fSkillCoolTime = pSkillTemplet->m_fSkillCoolTime;
#endif // UPGRADE_SKILL_SYSTEM_2013
	
#ifdef ADDITIONAL_MEMO
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if( pSkillTemplet->m_eID == CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_ORDNANCE_PENETRATOR && GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO15 ) == true )
#else //UPGRADE_SKILL_SYSTEM_2013
	if( pSkillTemplet->m_eID == CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON && GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO15 ) == true )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{		
		fSkillCoolTime -= 2.f;
		if( fSkillCoolTime < 0.f )
			fSkillCoolTime = 0.f;
	}
#endif

	UpNowMp( -fMPConsume );

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	if ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
	{
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		if ( true == g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence() )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ();
		else
		{
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

		if ( NULL != pItem )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
	}
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_RAVEN_VETERAN_COMMANDER		//섀도우 스텝, 섀도우 백 슬라이드는 사용 횟수 한계 혹은 모션 종료, 공격 스킬 사용 전까진 쿨타임이 돌지 않는다.
	if(  pSkillTemplet->m_eID != CX2SkillTree::SI_A_RF_SHADOW_STEP || true != m_bEnableShadowBackSlide )
#endif SERV_RAVEN_VETERAN_COMMANDER
	{
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, fSkillCoolTime );
	}

#ifdef UPGRADE_RAVEN
	// 스페셜 어택에 대해서 본노게이지 누적
	if( pSkillTemplet->m_eType == CX2SkillTree::ST_SPECIAL_ACTIVE && GetRemainHyperModeTime() > 0.f && IsMyUnit() == true )
	{
		m_fRageGauge += ( fMPConsume / 3.f );
		if( m_fRageGauge > 100.f )
			m_fRageGauge = 100.f;
	}
#endif

	if( false == SpecialAttackNoStageChange( pSkillTemplet ) )
	{
		if( GetRemainHyperModeTime() <= 0.f )
		{
			if( iPressedSkillSlotIndex > 3 ) //슬롯 B일때
			{
				StateChange( _CONST_GUUSER_::aNormalSkillStateIDSlotB[iPressedSkillSlotIndex-4] );
			}
			else
			{
				StateChange( _CONST_GUUSER_::aNormalSkillStateID[iPressedSkillSlotIndex] );
			}
		}
		else
		{
			if( iPressedSkillSlotIndex > 3 ) //슬롯 B일때
			{
				StateChange( _CONST_GUUSER_::aHyperSkillStateIDSlotB[iPressedSkillSlotIndex-4] );
			}
			else
			{
				StateChange( _CONST_GUUSER_::aHyperSkillStateID[iPressedSkillSlotIndex] );
			}
		}
	}

	m_eSpecialAttackKeyPressed = (SPECIAL_ATTACK_KEY_PRESSED) (iPressedSkillSlotIndex + 1);

	m_iNowSpecialAttack = iPressedSkillSlotIndex + 1;
	m_bSpecialAttackEventProcessedAtThisFrame = true;

#ifdef BALANCE_BLADE_MASTER_20130117
	if( RBM_COMBO_ZZZXZ == m_eRavenCurrentStage )	/// 만약 캔슬시 블레이드 마스터 ZZZXZ라면, 마법체 소멸 시켜주자
	{
		m_fBlackHoleTime		= 0.f;
		m_iBlackHoleHitCount	= 0;
		m_eRavenCurrentStage	= RSI_BASE;
	}
#endif BALANCE_BLADE_MASTER_20130117

#ifdef SERV_SKILL_USE_SUBQUEST
	Send_SKILL_USE_REQ( pSkillTemplet->m_eID );
#endif SERV_SKILL_USE_SUBQUEST

	return true;
	
}



// 상태변화가 없는 필살기는 하드코딩해서 예외 처리 한다
/*virtual*/bool CX2GURaven::SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet )
{
	if ( pSkillTemplet == NULL )
		return false;

	return false;
}




/*virtual*/void CX2GURaven::NoStateChangeActionFrameMove()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//							스테이트 함수 시작

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//RSI_DIE_FRONT
void CX2GURaven::RSI_DIE_FRONT_StartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GURaven::RSI_DIE_FRONT_Start()
{
	CommonStateStart();
	DyingStart();

	m_pXSkinAnim->SetPlaySpeed( 0.5f );



	ClearArcEnemyData();
#ifdef UPGRADE_RAVEN
	ClearNasodBall();
	m_fRageGauge = 0.f;
#endif
}

void CX2GURaven::RSI_DIE_FRONT_FrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GURaven::RSI_DIE_FRONT_EventProcess()
{
//{{ robobeg : 2008-10-21
	//if( m_bShow == false && m_pXSkinAnim->IsAnimationEnd() == true )
    if( GetShowObject() == false && m_pXSkinAnim->IsAnimationEnd() == true )
//}} robobeg : 2008-10-21
	{
		StateChange( GUSI_DIE );
	}
}


//RSI_DIE_BACK
void CX2GURaven::RSI_DIE_BACK_StartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GURaven::RSI_DIE_BACK_Start()
{
	CommonStateStart();
	DyingStart();

	m_pXSkinAnim->SetPlaySpeed( 0.5f );



	ClearArcEnemyData();
#ifdef UPGRADE_RAVEN
	ClearNasodBall();
	m_fRageGauge = 0.f;
#endif
}

void CX2GURaven::RSI_DIE_BACK_FrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.32f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GURaven::RSI_DIE_BACK_EventProcess()
{
//{{ robobeg : 2008-10-21
	//if( m_bShow == false && m_pXSkinAnim->IsAnimationEnd() == true )
    if ( GetShowObject() == false && m_pXSkinAnim->IsAnimationEnd() == true )
//}} robobeg : 2008-10-21
	{
		StateChange( GUSI_DIE );
	}
}

//RSI_WAIT
void CX2GURaven::RSI_WAIT_StartFuture()
{
	CommonStateStartFuture();
	PlayAnimationWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );

}

void CX2GURaven::RSI_WAIT_Start()
{
#ifdef BALANCE_BLADE_MASTER_20130117
	if( RBM_COMBO_ZZZXZ == m_eRavenCurrentStage )	/// 만약 캔슬시 블레이드 마스터 ZZZXZ라면, 마법체 소멸 시켜주자
	{
		m_fBlackHoleTime		= 0.f;
		m_iBlackHoleHitCount	= 0;
		m_eRavenCurrentStage	= RSI_BASE;
	}
#endif BALANCE_BLADE_MASTER_20130117

	CommonStateStart();
	PlayAnimationWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
}

void CX2GURaven::RSI_WAIT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( RSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_COMBO_X );
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
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 30.f, RSI_SUPER_DASH_READY )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

#ifdef BALANCE_BLADE_MASTER_20130117
void CX2GURaven::RSI_WALK_Start()
{
	if( RBM_COMBO_ZZZXZ == m_eRavenCurrentStage )	/// 만약 캔슬시 블레이드 마스터 ZZZXZ라면, 마법체 소멸 시켜주자
	{
		m_fBlackHoleTime		= 0.f;
		m_iBlackHoleHitCount	= 0;
		m_eRavenCurrentStage	= RSI_BASE;
	}

	CommonStateStart();
}
#endif BALANCE_BLADE_MASTER_20130117

void CX2GURaven::RSI_WALK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( RSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
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
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 30.f, RSI_SUPER_DASH_READY )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}


//RSI_JUMP_READY
void CX2GURaven::RSI_JUMP_READY_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( RSI_JUMP_COMBO_Z );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_JUMP_COMBO_X );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true
		|| m_InputData.pureRight == true
		|| m_InputData.pureLeft == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_JUMP_UP_EventProcess()
{
	if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		if( GetEnableDash() == true )
		{
			StateChange( USI_DASH_JUMP );
		}
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( RSI_JUMP_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_JUMP_COMBO_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_PhysicParam.nowSpeed.y <= 0.0f )
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

void CX2GURaven::RSI_JUMP_DOWN_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef WALL_JUMP_TEST
	else if( m_FrameDataFuture.unitCondition.bFootOnWall == true &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( RSI_WALL_LANDING );
	}
#endif WALL_JUMP_TEST
	else if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		if( GetEnableDash() == true )
		{
			StateChange( USI_DASH_JUMP );
		}
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( RSI_JUMP_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_JUMP_COMBO_X );
	}
#ifdef NEW_SKILL_TREE
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
#endif NEW_SKILL_TREE
	else if( m_PhysicParam.nowSpeed.y >= 0.0f )
	{
		StateChange( USI_JUMP_UP );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

void CX2GURaven::RSI_JUMP_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( RSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_COMBO_X );
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
		StateChange( USI_WALK );
	}
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 30.f, RSI_SUPER_DASH_READY )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( USI_WAIT, false );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_DASH_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( RSI_DASH_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_DASH_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 30.f, RSI_SUPER_DASH_READY )
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( m_InputData.pureRight == false && m_InputData.pureLeft == false )
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
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}


//RSI_DASH_END
void CX2GURaven::RSI_DASH_END_StartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GURaven::RSI_DASH_END_Start()
{
#ifdef BALANCE_BLADE_MASTER_20130117
	if( RBM_COMBO_ZZZXZ == m_eRavenCurrentStage )	/// 만약 캔슬시 블레이드 마스터 ZZZXZ라면, 마법체 소멸 시켜주자
	{
		m_fBlackHoleTime		= 0.f;
		m_iBlackHoleHitCount	= 0;
		m_eRavenCurrentStage	= RSI_BASE;
	}
#endif BALANCE_BLADE_MASTER_20130117

	CommonStateStart();

}

void CX2GURaven::RSI_DASH_END_FrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true )
	{
		CreateStepDust();

	}

	CommonFrameMove();
}

void CX2GURaven::RSI_DASH_END_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 8.0f )
		{
			StateChange( RSI_COMBO_Z );
		}
		else
		{
			StateChange( RSI_DASH_COMBO_Z );
		}
	}
	else if( m_InputData.oneX == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 8.0f )
		{
			StateChange( RSI_COMBO_X );
		}
		else
		{
			StateChange( RSI_DASH_COMBO_X );
		}
	}
	else if( SpecialAttackEventProcess() == true)
	{
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	//3키 대시
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
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 30.f, RSI_SUPER_DASH_READY )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, 
		LINE_RADIUS, 
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
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


//RSI_DASH_JUMP
void CX2GURaven::RSI_DASH_JUMP_StartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * 1.2f;
	m_bDownForce					= false;
}

void CX2GURaven::RSI_DASH_JUMP_Start()
{
	CommonStateStart();


	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
	{
		CreateStepDust();

	}
}

void CX2GURaven::RSI_DASH_JUMP_FrameMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataNow.syncData.bIsRight	== true )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.6f;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataNow.syncData.bIsRight	== false )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.6f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_DASH_JUMP_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( USI_DASH_JUMP );
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
			m_bDownForce = false;
		}
		else if( m_bDownForce == true )
		{
			StateChange( RSI_DASH_JUMP_POWER_LANDING );
			m_bDownForce = false;
		}
		else
		{
			if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
			{
				StateChangeDashIfPossible();
			}
			else
				StateChange( USI_DASH_JUMP_LANDING );
			m_bDownForce = false;
		}
	}
#ifdef WALL_JUMP_TEST
	else if( m_FrameDataFuture.unitCondition.bFootOnWall == true &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( RSI_WALL_LANDING );
	}
#endif WALL_JUMP_TEST
	else if( m_InputData.oneZ == true )
	{
		StateChange( RSI_DASH_JUMP_COMBO_Z );
	}
	else if( m_InputData.oneX == true && m_fReDashJumpXTime <= 0.0f )
	{
#ifdef SERV_RAVEN_VETERAN_COMMANDER
		if( CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnit()->GetUnitData().m_UnitClass )
			StateChange( RVC_DASH_JUMP_COMBO_X );
		else
			StateChange( RSI_DASH_JUMP_COMBO_X );
#else SERV_RAVEN_VETERAN_COMMANDER
		StateChange( RSI_DASH_JUMP_COMBO_X );
#endif SERV_RAVEN_VETERAN_COMMANDER
	}
#ifdef NEW_SKILL_TREE
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
#endif NEW_SKILL_TREE
	else if( m_InputData.pureRight == true )
	{
		if( m_FrameDataFuture.syncData.bIsRight == false )
		{
			if( m_PhysicParam.nowSpeed.y > GetJumpSpeed() )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( m_PhysicParam.nowSpeed.y > 0.0f )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	else if( m_InputData.pureLeft == true )
	{
		if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_PhysicParam.nowSpeed.y > GetJumpSpeed() )
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			if( m_PhysicParam.nowSpeed.y > 0.0f )
				StateChange( USI_JUMP_UP );
			else
				StateChange( USI_JUMP_DOWN );
		}
	}
	if( m_InputData.oneUp == true && m_PhysicParam.nowSpeed.y <= 0.0f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	if( m_InputData.oneDown == true )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
		m_bDownForce = true;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM


	CommonEventProcess();
}

void CX2GURaven::RSI_DASH_JUMP_EndFuture()
{
	CommonStateEnd();
	m_PhysicParam.fDownAccel	= 0.0f;
}

void CX2GURaven::RSI_DASH_JUMP_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	if( m_InputData.oneZ == true )
	{
		StateChange( RSI_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( RSI_COMBO_X );
	}
	else if( SpecialAttackEventProcess() == true )
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
	else if( m_InputData.pureUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 30.f, RSI_SUPER_DASH_READY )
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

	CommonEventProcess();
}


//RSI_DASH_JUMP_POWER_LANDING
void CX2GURaven::RSI_DASH_JUMP_POWER_LANDING_StartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x /= 2.0f;
}

void CX2GURaven::RSI_DASH_JUMP_POWER_LANDING_Start()
{
	CommonStateStart();


	CreateStepDust();
}

void CX2GURaven::RSI_DASH_JUMP_POWER_LANDING_EventProcess()
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
			StateChange( RSI_COMBO_Z );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( RSI_COMBO_X );
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
		//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 30.f, RSI_SUPER_DASH_READY )
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



//RSI_HYPER_MODE
void CX2GURaven::HyperModeFrameMove()
{
#ifdef MODIFY_RIDING_PET_AWAKE
	CommonHyperModeFrameMove( 0.05f, 0.83f );
#else
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	//g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.05f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.05f ) == true && EventCheck(0.05f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.83f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.83f ) == true && EventCheck(0.83f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
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

////RSI_DAMAGE_GROGGY
//void CX2GURaven::RSI_DAMAGE_GROGGY_FrameMoveFuture()
//{
//	if( m_fGroggyTime > 0.0f )
//	{
//		m_fGroggyTime -= m_fElapsedTime;
//		if( m_fGroggyTime < 0.0f )
//			m_fGroggyTime = 0.0f;
//	}
//	CommonFrameMoveFuture();
//}
//
//void CX2GURaven::RSI_DAMAGE_GROGGY_EventProcess()
//{
//	if( m_fGroggyTime <= 0.0f )
//	{
//		StateChange( RSI_WAIT );
//	}
//	CommonEventProcess();
//}

//RSI_DAMAGE_SMALL_FRONT
void CX2GURaven::RSI_DAMAGE_SMALL_FRONT_EventProcess()
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

//{{ JHKang / 강정훈 / 2010/11/02
#ifdef NEW_SKILL_2010_11
void CX2GURaven::RSI_DAMAGE_SMALL_BACK_Start()
{
	CommonStateStart();

	RBM_EMERGENCY_ESCAPE();
}
#endif NEW_SKILL_2010_11
//}} JHKang / 강정훈 / 2010/11/02


//RSI_DAMAGE_SMALL_BACK
void CX2GURaven::RSI_DAMAGE_SMALL_BACK_EventProcess()
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


//RSI_DAMAGE_BIG_FRONT
void CX2GURaven::RSI_DAMAGE_BIG_FRONT_Start()
{
	CommonStateStart();

	CreateStepDust();

}

void CX2GURaven::RSI_DAMAGE_BIG_FRONT_EventProcess()
{
	if( m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y > 200.0f )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.93f )
		{
			StateChange( USI_WAIT );
		}
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


//RSI_DAMAGE_BIG_BACK
void CX2GURaven::RSI_DAMAGE_BIG_BACK_Start()
{
	CommonStateStart();

	CreateStepDust();

	//{{ JHKang / 강정훈 / 2010/11/01 / 긴급탈출 패시브 추가
#ifdef NEW_SKILL_2010_11
	RBM_EMERGENCY_ESCAPE();
#endif NEW_SKILL_2010_11
	//}} JHKang / 강정훈 / 2010/11/01 / 긴급탈출 패시브 추가
}

void CX2GURaven::RSI_DAMAGE_BIG_BACK_EventProcess()
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


//RSI_DAMAGE_DOWN_FRONT
void CX2GURaven::RSI_DAMAGE_DOWN_FRONT_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.19f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.19f ) == true && EventCheck( 0.19f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		CreateStepDust();

	}

	CommonFrameMove();
}

void CX2GURaven::RSI_DAMAGE_DOWN_FRONT_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[4] == true )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( RSI_STANDUP_ATTACK_FRONT_NO_DOWN );
			}
			else
			{
				StateChange( RSI_STANDUP_ATTACK_FRONT );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( RSI_STANDUP_ATTACK_FRONT );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( RSI_STANDUP_ROLLING_FRONT_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( RSI_STANDUP_ROLLING_FRONT_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_FRONT );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
	{
		if( m_InputData.oneX == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= true;
		}
		else if( m_InputData.oneZ == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= true;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
		}
		else if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
		else
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_DAMAGE_DOWN_BACK_FrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.32f ) == true )
	{
		CreateStepDust();

	}

	CommonFrameMove();
}

void CX2GURaven::RSI_DAMAGE_DOWN_BACK_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[4] == true )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( RSI_STANDUP_ATTACK_BACK_NO_DOWN ); 
			}
			else
			{
				StateChange( RSI_STANDUP_ATTACK_BACK );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( RSI_STANDUP_ATTACK_BACK );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( RSI_STANDUP_ROLLING_BACK_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( RSI_STANDUP_ROLLING_BACK_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_BACK );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66 )
	{
		if( m_InputData.oneX == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= true;
		}
		else if( m_InputData.oneZ == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= true;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
		}
		else if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
		else
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
	}

	CommonEventProcess();
}

//RSI_DAMAGE_AIR_DOWN_LANDING
void CX2GURaven::RSI_DAMAGE_AIR_DOWN_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[4] == true )
		{
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( RSI_STANDUP_ATTACK_FRONT_NO_DOWN );
			}
			else
			{
				StateChange( RSI_STANDUP_ATTACK_FRONT );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( RSI_STANDUP_ATTACK_FRONT );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( RSI_STANDUP_ROLLING_FRONT_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( RSI_STANDUP_ROLLING_FRONT_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_FRONT );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
	{
		if( m_InputData.oneX == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= true;
		}
		else if( m_InputData.oneZ == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= true;
			m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
		}
		else if( m_FrameDataFuture.syncData.bIsRight == true )
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
		else
		{
			if( m_InputData.pureRight == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
			else if( m_InputData.pureLeft == true )
			{	
				m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
				m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
				m_FrameDataFuture.stateParam.bEventFlagList[4]	= false;
			}
		}
	}

	CommonEventProcess();
}

//RSI_STANDUP_ROLLING_FRONT_FRONT
void CX2GURaven::RSI_STANDUP_ROLLING_FRONT_FRONT_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_STANDUP_ROLLING_FRONT_FRONT_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


//RSI_STANDUP_ROLLING_FRONT_BACK
void CX2GURaven::RSI_STANDUP_ROLLING_FRONT_BACK_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


void CX2GURaven::RSI_STANDUP_ROLLING_FRONT_BACK_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



//RSI_STANDUP_ROLLING_BACK_FRONT
void CX2GURaven::RSI_STANDUP_ROLLING_BACK_FRONT_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_STANDUP_ROLLING_BACK_FRONT_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



//RSI_STANDUP_ROLLING_BACK_BACK
void CX2GURaven::RSI_STANDUP_ROLLING_BACK_BACK_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_STANDUP_ROLLING_BACK_BACK_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


//RSI_STANDUP_ATTACK_FRONT
void CX2GURaven::RSI_STANDUP_ATTACK_FRONT_FrameMove()
{

	CommonFrameMove();
}

void CX2GURaven::RSI_STANDUP_ATTACK_FRONT_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_STANDUP_ATTACK_FRONT_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


//RSI_STANDUP_ATTACK_BACK
void CX2GURaven::RSI_STANDUP_ATTACK_BACK_FrameMove()
{

	CommonFrameMove();
}

void CX2GURaven::RSI_STANDUP_ATTACK_BACK_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_STANDUP_ATTACK_BACK_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

#ifdef SERV_SKILL_NOTE


void CX2GURaven::RSI_SUPER_DASH_READY_StateStart()
{
	CommonStateStart();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO5 ) == true )
	{   
		ResetAnimSpeed( m_fAnimSpeed, true, true, 2.0f );
	}
}

#endif

void CX2GURaven::RSI_SUPER_DASH_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.3f )
	{
		StateChange( RSI_SUPER_DASH );	
	}

	CommonEventProcess();
}

//RSI_SUPER_DASH

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
// 버닝 러쉬
#pragma region 버닝 러쉬
void CX2GURaven::RSI_SUPER_DASH_Init()
{
	XSkinMeshReadyInBackground( L"ST_DarkFireBall_Dummy.X" );

	TextureReadyInBackground( L"Arme_Critical.dds" );
	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
}
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
void CX2GURaven::RSI_SUPER_DASH_StateStart()
{
	CommonStateStart();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO5 ) == true )
	{   
		CX2Unit* pUnit = GetUnit();
		if ( NULL != pUnit )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_RF_BURNING_RUSH );
			if ( NULL != pSkillTemplet )
			{
				CX2UserSkillTree& accessUserSkillTree = pUnit->AccessUnitData().m_UserSkillTree;
				const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
				
				accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) * 2 );
			}
		}
	}
	
}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

void CX2GURaven::RSI_SUPER_DASH_FrameMoveFuture()
{

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
#ifndef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 메모 효과 변경으로 인해 구문 삭제 

	// 메모 적용 시, 대쉬 속도 2배로 변경
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO5 ) == true )
	{   
			ResetAnimSpeed( m_fAnimSpeed, true, true, 1.5f * 1.5f );
	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
#else // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.25f;
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈	


	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SUPER_DASH_EventProcess()
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
// 버닝 러쉬 스킬 변경으로 인해 해당 이벤트 프로세스 삭제
#ifdef SERV_SKILL_NOTE
	float MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC = 40.f;
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO5 ) == true )
	{
		MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC = 10.f;
	}
#else
	const float MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC = 40.f;
#endif
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();


#else //UPGRADE_SKILL_SYSTEM_2013

	switch( m_eSpecialAttackKeyPressed )
	{
	case SAKP_A:
		{
			if( m_InputData.pureA == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		}
		break;

	case SAKP_S:
		{
			if( m_InputData.pureS == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		} break;

	case SAKP_D:
		{
			if( m_InputData.pureD == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		} break;

	case SAKP_C:
		{
			if( m_InputData.pureC == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		} break;
	case SAKP_Q:
		{
			if( m_InputData.pureQ == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		} break;

	case SAKP_W:
		{
			if( m_InputData.pureW == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		} break;

	case SAKP_E:
		{
			if( m_InputData.pureE == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		} break;

	case SAKP_R:
		{
			if( m_InputData.pureR == false ||
				FlushMp( MAGIC_SUPER_DASH_MP_CONSUME_PER_SEC * m_fElapsedTime ) == false )
			{
				StateChange( RSI_SUPER_DASH_END );
			}
		} break;
	}

#ifdef SERV_SKILL_NOTE
	if( SpecialAttackEventProcess() == true )
	{		
	}
#endif

#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	

	CommonEventProcess();
}


//RSI_SUPER_DASH_END
void CX2GURaven::RSI_SUPER_DASH_END_FrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true )
	{
		CreateStepDust();

	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SUPER_DASH_END_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}	
#ifdef SERV_SKILL_NOTE
	else if( SpecialAttackEventProcess() == true )
	{		
	}
#endif
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	

	CommonEventProcess();
}

#pragma  endregion 버닝 러쉬
//RSI_COMBO_Z
void CX2GURaven::RSI_COMBO_Z_EventProcess()
{
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.29f )
	{
		StateChange( RSI_COMBO_ZZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.3f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
		{
			m_fCanNotAttackTime = 0.1f;
			StateChange( USI_WAIT );
		}
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
		m_fCanNotAttackTime = 0.1f;
	}
	CommonEventProcess();
}

//RSI_COMBO_ZZ
void CX2GURaven::RSI_COMBO_ZZ_EventProcess()
{
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	//else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	//{
	//	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.7f )
	//	{
	//		StateChangeDashIfPossible();
	//	}
	//}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.39f )
	{
		StateChange( RSI_COMBO_ZZZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.39f )
	{
		StateChange( RSI_COMBO_ZZX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.39f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.39f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.1f;
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_COMBO_ZZZ_EventProcess()
{
	m_bReAttackZ1Right = m_FrameDataFuture.syncData.bIsRight;
	m_fReAttackZ1Time = 0.2f;

	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.59f )
	{
		StateChange( RSI_COMBO_ZZZZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.59f )
	{
		StateChange( RSI_COMBO_ZZZX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.3f )
	{
#ifdef RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		if( ( CX2Unit::UC_RAVEN_WEAPON_TAKER == GetUnit()->GetUnitData().m_UnitClass || 
			CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnit()->GetUnitData().m_UnitClass ) &&
#else  SERV_RAVEN_VETERAN_COMMANDER
		if( CX2Unit::UC_RAVEN_WEAPON_TAKER == GetUnit()->GetUnitData().m_UnitClass &&
#endif SERV_RAVEN_VETERAN_COMMANDER
			( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ||
			m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) )
			StateChange( RSI_COMBO_ZZZX );

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		else if( CX2Unit::UC_RAVEN_WEAPON_TAKER == GetUnit()->GetUnitData().m_UnitClass || 
			CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnit()->GetUnitData().m_UnitClass )
#else  SERV_RAVEN_VETERAN_COMMANDER
		else if( CX2Unit::UC_RAVEN_WEAPON_TAKER == GetUnit()->GetUnitData().m_UnitClass )
#endif SERV_RAVEN_VETERAN_COMMANDER
			StateChange( RSI_WEAPON_TAKER_COMBO_ZZZdownX );
#endif RAVEN_WEAPON_TAKER
	}
#ifdef RAVEN_SECOND_CLASS_CHANGE
	else if( (CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass || CX2Unit::UC_RAVEN_BLADE_MASTER == GetUnit()->GetUnitData().m_UnitClass) &&
#else
	else if( CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass &&
#endif
		m_FrameDataFuture.stateParam.bEventFlagList[2] == true && 
		m_pXSkinAnimFuture->GetNowAnimationTime() > 0.59f )
	{
		StateChange( RSI_SOUL_TAKER_COMBO_ZZZupZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
#ifdef RAVEN_SECOND_CLASS_CHANGE
	else if( (CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass || CX2Unit::UC_RAVEN_BLADE_MASTER == GetUnit()->GetUnitData().m_UnitClass) &&
#else
	else if( CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass &&
#endif
		m_InputData.oneZ == true && m_InputData.pureUp == true && 
		m_pXSkinAnimFuture->GetNowAnimationTime() < 0.59f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.59f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.59f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
		{
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//RSI_COMBO_ZZZZ
void CX2GURaven::RSI_COMBO_ZZZZ_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed() * 1.3f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_COMBO_ZZZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.1f )
		{
			StateChange( USI_WAIT );
		}
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//RSI_COMBO_ZZZX
void CX2GURaven::RSI_COMBO_ZZZX_Start()
{
	CommonStateStart();
	m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
	m_DamageData.bCanRevenge	= false;

	if( FlushMp( 10.0f ) == true )
		m_bZZZX = true;
	else
		m_bZZZX = false;
}

void CX2GURaven::RSI_COMBO_ZZZX_FrameMove()
{
	if( m_bZZZX == true )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.566f ) && EventCheck( 0.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 20.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 20.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y += 10.0f;
			CKTDGParticleSystem::CParticleEventSequence* pSeq;
			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire01", pos );
			pSeq->SetAddRotate( GetRotateDegree() );
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.57f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.57f ) && EventCheck( 0.57f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 5.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 5.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y += 20.0f;
			CKTDGParticleSystem::CParticleEventSequence* pSeq;
			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire02", pos );
			pSeq->SetAddRotate( GetRotateDegree() );
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.7f ) && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 20.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 20.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y += 10.0f;
			CKTDGParticleSystem::CParticleEventSequence* pSeq;
			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire01", pos );
			pSeq->SetAddRotate( GetRotateDegree() );
			pSeq->SetAxisAngle( GetRotateDegree() );

			pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 5.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 5.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y += 20.0f;
			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire02", pos );
			pSeq->SetAddRotate( GetRotateDegree() );
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.83f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.83f ) && EventCheck( 0.83f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 20.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 20.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y += 10.0f;
			CKTDGParticleSystem::CParticleEventSequence* pSeq;
			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire01", pos );
			pSeq->SetAddRotate( GetRotateDegree() );
			pSeq->SetAxisAngle( GetRotateDegree() );

			pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 5.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 5.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y += 20.0f;
			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire02", pos );
			pSeq->SetAddRotate( GetRotateDegree() );
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
		//kimc 위의 코드 중복된것으로 판단되어 주석 처리함
// 		else if( m_pXSkinAnim->EventTimer( 0.83f ) && EventCheck( 0.83f, false ) == true )
// 		{
// 			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
// 			if( GetIsRight() == true )
// 				pos += 20.0f * m_FrameDataNow.unitCondition.dirVector;
// 			else
// 				pos -= 20.0f * m_FrameDataNow.unitCondition.dirVector;
// 			pos.y += 10.0f;
// 			CKTDGParticleSystem::CParticleEventSequence* pSeq;
// 			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire01", pos );
// 			pSeq->SetAddRotate( GetRotateDegree() );
// 			pSeq->SetAxisAngle( GetRotateDegree() );
// 
// 			pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
// 			if( GetIsRight() == true )
// 				pos += 5.0f * m_FrameDataNow.unitCondition.dirVector;
// 			else
// 				pos -= 5.0f * m_FrameDataNow.unitCondition.dirVector;
// 			pos.y += 20.0f;
// 			pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_ComboZ4a_GunFire02", pos );
// 			pSeq->SetAddRotate( GetRotateDegree() );
// 			pSeq->SetAxisAngle( GetRotateDegree() );
// 		}

		if( m_pXSkinAnim->GetNowAnimationTime() > 0.566f && m_pXSkinAnim->GetNowAnimationTime() < 1.0f )
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CKTDXCollision::CollisionData data;
			data.m_fRadius = 100.0f;

			if( GetIsRight() == true )
				pos += 50.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 50.0f * m_FrameDataNow.unitCondition.dirVector;
			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat );
			mat._41 = pos.x;
			mat._42 = pos.y;
			mat._43 = pos.z;
			data.m_pCombineMatrix = &mat;
			m_DamageData.pCollisionData = &data;

			if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
			{
				m_FrameDataNow.unitCondition.bHit = true;
			}
		}

		if( EventTimer( 1.01f, false ) == true )
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CKTDXCollision::CollisionData data;
			data.m_fRadius = 100.0f;

			if( GetIsRight() == true )
				pos += 20.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 20.0f * m_FrameDataNow.unitCondition.dirVector;
			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat );
			mat._41 = pos.x;
			mat._42 = pos.y;
			mat._43 = pos.z;
			data.m_pCombineMatrix = &mat;

			m_DamageData.pCollisionData = &data;
			m_DamageData.backSpeed.x	= GetDashSpeed() * 2.0f;
			m_DamageData.backSpeed.y	= GetJumpSpeed() * 0.3f;
			m_DamageData.bReAttack		= false;
			m_DamageData.hitUnitList.resize(0);


			if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
			{
				m_FrameDataNow.unitCondition.bHit = true;
			}
			m_bZZZX = false;
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

		
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
		if( pSeq != NULL )
		{
			PlaySequenceByTriggerCount( pSeq, pos, 100, 100, 1 );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_COMBO_ZZZX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true 
		|| m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.35f )
		{
			StateChange( USI_WAIT );
		}
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//RSI_COMBO_ZZX
void CX2GURaven::RSI_COMBO_ZZX_EventProcess()
{
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
#ifdef RAVEN_SECOND_CLASS_CHANGE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f &&
		GetUnit()->GetClass() == CX2Unit::UC_RAVEN_RECKLESS_FIST &&
		( ( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) || 
		( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ) )
	{
		StateChange( RRF_COMBO_ZZXFRONTX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#endif 
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
	{
		StateChange( RSI_COMBO_ZZXX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.6f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.0f )
		{
			StateChange( USI_WAIT );
		}
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

//RSI_COMBO_ZZXX
void CX2GURaven::RSI_COMBO_ZZXX_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_COMBO_ZZXX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.7f )
		{
			StateChange( USI_WAIT );
		}
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//RSI_COMBO_X
void CX2GURaven::RSI_COMBO_X_EventProcess()
{
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.53f )
	{
		StateChange( RSI_COMBO_XX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#ifdef RAVEN_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_( 1, 0.48f, 0.48f, m_InputData.oneZ == true && GetUnit()->GetClass() == CX2Unit::UC_RAVEN_BLADE_MASTER, RBM_COMBO_XZ )
#endif
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.53f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

//RSI_COMBO_XX
void CX2GURaven::RSI_COMBO_XX_EventProcess()
{
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}



	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.55f )
	{
#ifdef RAVEN_SECOND_CLASS_CHANGE
		if( CX2Unit::UC_RAVEN_OVER_TAKER == GetUnit()->GetUnitData().m_UnitClass || CX2Unit::UC_RAVEN_RECKLESS_FIST == GetUnit()->GetUnitData().m_UnitClass )
#else
		if( CX2Unit::UC_RAVEN_OVER_TAKER == GetUnit()->GetUnitData().m_UnitClass )
#endif
		{
			if( m_InputData.pureDown == true )
				StateChange( RSI_OVER_TAKER_COMBO_XXfrontX );
			else if( m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureRight == true )
				StateChange( RSI_OVER_TAKER_COMBO_XXdownX );
			else if( m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureLeft == true )
				StateChange( RSI_OVER_TAKER_COMBO_XXdownX );
			else
				StateChange( RSI_COMBO_XXX );
		}
		else 
		{
			StateChange( RSI_COMBO_XXX );
		}

		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;

	}
#ifdef RAVEN_SECOND_CLASS_CHANGE
	else if( (CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass || CX2Unit::UC_RAVEN_BLADE_MASTER == GetUnit()->GetUnitData().m_UnitClass) &&
#else
	else if( CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass &&
#endif	
		m_FrameDataFuture.stateParam.bEventFlagList[1] == true
		&& m_pXSkinAnimFuture->GetNowAnimationTime() > 0.55f )
	{
		m_iSuccessiveHit_OneZ = 0;
		StateChange( RSI_SOUL_TAKER_COMBO_XXZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.55f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
#ifdef RAVEN_SECOND_CLASS_CHANGE
	else if( (CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass || CX2Unit::UC_RAVEN_BLADE_MASTER == GetUnit()->GetUnitData().m_UnitClass) && 
#else
	else if( CX2Unit::UC_RAVEN_SOUL_TAKER == GetUnit()->GetUnitData().m_UnitClass && 
#endif
		m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.55f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_COMBO_XXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.16f ) && EventCheck( 0.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 7.0f ) == true )
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			if( GetIsRight() == true )
				pos += 35.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 35.0f * m_FrameDataNow.unitCondition.dirVector;

			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Smoke_Raven_ComboX03", pos );
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Smoke_Raven_ComboX01", pos );
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_ComboX02", pos );

			UpDownCrashCamera( 20.0f, 0.3f );

			CKTDXCollision::CollisionData data;
			data.m_fRadius = 110.0f;
			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat );
			mat._41 = pos.x;
			mat._42 = pos.y;
			mat._43 = pos.z;
			data.m_pCombineMatrix = &mat;

			m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
			m_DamageData.bCanRevenge	= false;

			m_DamageData.pCollisionData = &data;
			m_DamageData.hitUnitList.resize(0);


			if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
			{
				m_FrameDataNow.unitCondition.bHit = true;
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				PlaySequenceByTriggerCount( pSeq, pos, 100, 100, 1 );
				pSeq->SetAxisAngle( GetRotateDegree() );
				pSeq->SetAddRotate( GetRotateDegree() );
			}
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_COMBO_XXX_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.0f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

//RSI_DASH_COMBO_Z
void CX2GURaven::RSI_DASH_COMBO_Z_EventProcess()
{
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.397f
		&& true == IsOnSomethingFuture() )
	{
		StateChange( RSI_DASH_COMBO_ZZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.397f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
		{
			m_fCanNotAttackTime = 0.1f;
			StateChange( USI_WAIT );
		}
	}


	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//RSI_DASH_COMBO_ZZ
void CX2GURaven::RSI_DASH_COMBO_ZZ_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.2f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//RSI_DASH_COMBO_X
void CX2GURaven::RSI_DASH_COMBO_X_FrameMove()
{
#ifdef BALANCE_BLADE_MASTER_20130117		/// 스크립트에서 DAMAGE_DATA_NEXT로 분리
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) == true && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit	= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );	
			m_LuaManager.EndTable();		
		}
#else  BALANCE_BLADE_MASTER_20130117
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.63f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.63f ) == true && EventCheck( 0.63f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		m_DamageData.backSpeed.x		= GetDashSpeed() * 1.5f;
		m_DamageData.fCameraCrashGap	= 15.0f;
		m_DamageData.fCameraCrashTime	= 0.5f;
		m_DamageData.fStopTimeDef		= 0.7f;
#endif  BALANCE_BLADE_MASTER_20130117

		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_DashAttackX01", pos );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_DashAttackX02", pos );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_DashAttackX03", pos );
		UpDownCrashCamera( 20.0f, 0.5f );
	}
	CommonFrameMove();
}

void CX2GURaven::RSI_DASH_COMBO_X_EventProcess()
{
	
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.1f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//RSI_DASH_JUMP_COMBO_Z
void CX2GURaven::RSI_DASH_JUMP_COMBO_Z_EventProcess()
{
	if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_InputData.oneX == true &&
#ifdef RAVEN_SECOND_CLASS_CHANGE
		( CX2Unit::UC_RAVEN_OVER_TAKER == GetUnit()->GetUnitData().m_UnitClass || CX2Unit::UC_RAVEN_RECKLESS_FIST == GetUnit()->GetUnitData().m_UnitClass
#else
		( CX2Unit::UC_RAVEN_OVER_TAKER == GetUnit()->GetUnitData().m_UnitClass
#endif
#ifdef RAVEN_WEAPON_TAKER
		|| CX2Unit::UC_RAVEN_WEAPON_TAKER == GetUnit()->GetUnitData().m_UnitClass
#endif RAVEN_WEAPON_TAKER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
		|| CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnit()->GetUnitData().m_UnitClass
#endif SERV_RAVEN_VETERAN_COMMANDER
		) )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.2f )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( RSI_DASH_JUMP_COMBO_ZZ );
		}
#ifdef RAVEN_SECOND_CLASS_CHANGE
		else if( (CX2Unit::UC_RAVEN_OVER_TAKER == GetUnit()->GetUnitData().m_UnitClass || CX2Unit::UC_RAVEN_RECKLESS_FIST == GetUnit()->GetUnitData().m_UnitClass) &&
#else
		else if( CX2Unit::UC_RAVEN_OVER_TAKER == GetUnit()->GetUnitData().m_UnitClass &&
#endif
			m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX );
		}
#ifdef RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		else if( (CX2Unit::UC_RAVEN_WEAPON_TAKER == GetUnit()->GetUnitData().m_UnitClass ||
				  CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnit()->GetUnitData().m_UnitClass ) &&
#else  SERV_RAVEN_VETERAN_COMMANDER
		else if( CX2Unit::UC_RAVEN_WEAPON_TAKER == GetUnit()->GetUnitData().m_UnitClass &&
#endif SERV_RAVEN_VETERAN_COMMANDER
			m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX );
		}
#endif RAVEN_WEAPON_TAKER
	}

	CommonEventProcess();
}

//RSI_DASH_JUMP_COMBO_ZZ
void CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ_StateStart()
{
	CommonStateStart();

#ifdef BOUNDING_COMBO_TEST
	m_DamageData.reActType = CX2DamageManager::RT_BOUNDING;
	m_DamageData.backSpeed.y = -2300.f;
#endif BOUNDING_COMBO_TEST

}


void CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.23f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.23f ) == true && EventCheck( 0.23f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 2.0f;
	}
	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( RSI_DASH_JUMP_COMBO_ZZ_Landing );
	}

	CommonEventProcess();
}


//RSI_DASH_JUMP_COMBO_ZZ_Landing
void CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ_Landing_StartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed.x /= 2.0f;
}

void CX2GURaven::RSI_DASH_JUMP_COMBO_ZZ_Landing_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 3.0f )
	{
		if( m_InputData.oneZ == true )
		{
			StateChange( RSI_COMBO_Z );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( RSI_COMBO_X );
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
			StateChange( USI_JUMP_READY );
		}
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

//RSI_DASH_JUMP_COMBO_X
void CX2GURaven::RSI_DASH_JUMP_COMBO_X_Init()
{
	XSkinMeshReadyInBackground( L"Hand_FireImpact01.X" );
}

void CX2GURaven::RSI_DASH_JUMP_COMBO_X_Start()
{
	CommonStateStart();
	m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
	m_DamageData.bCanRevenge	= false;
}

void CX2GURaven::RSI_DASH_JUMP_COMBO_X_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.21f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.4f )
	{
		if( m_PhysicParam.nowSpeed.y < 0.0f )
			m_PhysicParam.nowSpeed.y = 0.0f;
	}
	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_DASH_JUMP_COMBO_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef RAVEN_WEAPON_TAKER
		if ( FlushMp( 5.0f ) == true )
#else RAVEN_WEAPON_TAKER
		if ( FlushMp( 10.0f ) == true )
#endif RAVEN_WEAPON_TAKER
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 10.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 10.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y -= 10.0f;

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_DASH_JUMP_X", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree() );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 vAngle = GetRotateDegree();
			vAngle.z -= 50.f;


			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				PlaySequenceByTriggerCount( pSeq, pos, 100, 100, 1 );
				pSeq->SetAxisAngle( vAngle );
				pSeq->SetAddRotate( vAngle );
			}
		}
		
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_DASH_JUMP_COMBO_X_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

//RSI_JUMP_COMBO_Z
void CX2GURaven::RSI_JUMP_COMBO_Z_FrameMoveFuture()
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

void CX2GURaven::RSI_JUMP_COMBO_Z_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}


//RSI_JUMP_COMBO_X

void CX2GURaven::RSI_JUMP_COMBO_X_StateStart()
{
	CommonStateStart();

#ifdef BOUNDING_COMBO_TEST
	m_DamageData.reActType = CX2DamageManager::RT_BOUNDING;
	m_DamageData.backSpeed.y = -1800.f;
#endif BOUNDING_COMBO_TEST

}



void CX2GURaven::RSI_JUMP_COMBO_X_FrameMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( m_InputData.pureDown == true )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonFrameMoveFuture();
}



void CX2GURaven::RSI_JUMP_COMBO_X_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			m_fCanNotJumpTime = 0.2f;
		}
		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}






//RSI_SI1_MAXIMUM_CANNON
void CX2GURaven::RSI_SI1_MAXIMUM_CANNON_Init()
{
	XSkinMeshReadyInBackground( L"MaximumCanon_Mesh01.X" );
	XMeshReadyInBackground( L"Maximumcanon_Mesh02.Y" );
}

void CX2GURaven::RSI_SI1_MAXIMUM_CANNON_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		if( 0.f != vDirVec.y )
		{
			vDirVec.y = 0.f;
			D3DXVec3Normalize( &vDirVec, &vDirVec );
		}

		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -30.f;
		else
			vPos = vBonePos + vDirVec * 30.f;
		vPos.y -= 7.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Raven_MaximumCanon02", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		if( true == bIsRight )
			vPos = GetPos() + vDirVec * 200.f;
		else
			vPos = GetPos() + vDirVec * -200.f;
		vPos.y += 3.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Raven_MaximumCanon03", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -30.f;
		else
			vPos = vBonePos + vDirVec * 30.f;
		vPos.y -= 10.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Raven_MaximumCanon04", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		vPos = vBonePos;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_MaximumCanon05", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}



		if( true == bIsRight )
			vPos = vBonePos + vDirVec * 95.f;
		else
			vPos = vBonePos + vDirVec * -95.f;
		vPos.y -= 7.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_MaximumCanon06", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -30.f;
		else
			vPos = vBonePos + vDirVec * 30.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_MAXIMUM_CANNON", GetPowerRate(), vPos, 
															GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.55f ) == true && EventCheck( 1.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 데미지 이펙트 생성 위치를 120에서 180으로 조정
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * 180.f;
		else
			vPos = vBonePos + vDirVec * -180.f;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * 120.f;
		else
			vPos = vBonePos + vDirVec * -120.f;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		vPos.y += 15.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_MAXIMUM_CANNON_FLY", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_SI1_MAXIMUM_CANNON_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}



//RSI_SI1_MAXIMUM_CANNON_HYPER
void CX2GURaven::RSI_SI1_MAXIMUM_CANNON_HYPER_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		if( 0.f != vDirVec.y )
		{
			vDirVec.y = 0.f;
			D3DXVec3Normalize( &vDirVec, &vDirVec );
		}

		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -30.f;
		else
			vPos = vBonePos + vDirVec * 30.f;
		vPos.y -= 7.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Raven_MaximumCanon02", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		if( true == bIsRight )
			vPos = GetPos() + vDirVec * 200.f;
		else
			vPos = GetPos() + vDirVec * -200.f;
		vPos.y += 3.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Raven_MaximumCanon03", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -30.f;
		else
			vPos = vBonePos + vDirVec * 30.f;
		vPos.y -= 10.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Raven_MaximumCanon04", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		vPos = vBonePos;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_MaximumCanon05", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}



		if( true == bIsRight )
			vPos = vBonePos + vDirVec * 95.f;
		else
			vPos = vBonePos + vDirVec * -95.f;
		vPos.y -= 7.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_MaximumCanon06", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}


		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -30.f;
		else
			vPos = vBonePos + vDirVec * 30.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_MAXIMUM_CANNON_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.55f ) == true && EventCheck( 1.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();
		D3DXVECTOR3 vPos;

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편		
		// 데미지 이펙트 생성 위치 변경
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * 180.f;
		else
			vPos = vBonePos + vDirVec * -180.f;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * 120.f;
		else
			vPos = vBonePos + vDirVec * -120.f;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		vPos.y += 15.f;

		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_MAXIMUM_CANNON_FLY_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}

	

	CommonFrameMove();
}

void CX2GURaven::RSI_SI1_MAXIMUM_CANNON_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}





//RSI_SI2_POWER_ASSAULT
void CX2GURaven::RSI_SI2_POWER_ASSAULT_Init()
{
	TextureReadyInBackground( L"PowerAssault01.dds" );
}

void CX2GURaven::RSI_SI2_POWER_ASSAULT_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.35f ) == true && EventCheck( 0.35f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 1500.f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.9f ) == true && EventCheck( 0.9f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 0.f;
		m_PhysicParam.nowSpeed.x = 1000.f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SI2_POWER_ASSAULT_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.08f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.08f ) == true && EventCheck( 0.08f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqPowerAssault[0] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Raven_PowerAssault01", D3DXVECTOR3(0, 0, 0) );
		m_hSeqPowerAssault[1] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Raven_PowerAssault02", D3DXVECTOR3(0, 0, 0) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.466f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.466f ) == true && EventCheck( 0.466f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqPowerAssault[2] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Raven_PowerAssault03", D3DXVECTOR3(0, 0, 0) );
	}

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPowerAssault[0] )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPowerAssault[0] );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE2_Lhand2" );
			D3DXVECTOR3 vDirVec = GetDirVector();
			bool bIsRight = GetIsRight();

			D3DXVECTOR3 vZVec;
			vZVec.y = 0.f;
			vZVec.z = vDirVec.x;
			vZVec.x = -vDirVec.z;
			D3DXVec3Normalize( &vZVec, &vZVec );

			D3DXVECTOR3 vPos;
			if( true == bIsRight )
				vPos = vBonePos + vDirVec * -10.f;
			else
				vPos = vBonePos + vDirVec * 10.f;
			vPos += vZVec * -30.f;

			pSeq->SetPosition( vPos );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );

		}
		else
		{
			m_hSeqPowerAssault[0]  = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}

	}


	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPowerAssault[1] )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPowerAssault[1] );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE2_Lhand2" );

			pSeq->SetPosition( vBonePos );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqPowerAssault[1]  = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}




	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPowerAssault[2] )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPowerAssault[2] );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE2_Lhand2" );
			D3DXVECTOR3 vDirVec = GetDirVector();
			bool bIsRight = GetIsRight();

			D3DXVECTOR3 vPos;
			if( true == bIsRight )
				vPos = vBonePos + vDirVec * 200.f;
			else
				vPos = vBonePos + vDirVec * -200.f;

			pSeq->SetPosition( vPos );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqPowerAssault[2]  = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

//#ifndef BALANCE_BLADE_MASTER_20130117		/// 데미지 이펙트로 변경
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		m_DamageData.reActType 			= CX2DamageManager::RT_FLY;
		m_DamageData.backSpeed 			= D3DXVECTOR2( 1500.f, 1000.f );
		m_DamageData.attackType			= CX2DamageManager::AT_SPECIAL;
		
		m_DamageData.fCameraCrashGap	= 5.f;
		m_DamageData.fCameraCrashTime	= 0.2f;
		m_DamageData.bReAttack			= false;
		//m_DamageData.fHitGap			= 0.1f;
	}
//#endif BALANCE_BLADE_MASTER_20130117

	CommonFrameMove();
}

void CX2GURaven::RSI_SI2_POWER_ASSAULT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}



//RSI_SI2_POWER_ASSAULT_HYPER
void CX2GURaven::RSI_SI2_POWER_ASSAULT_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.35f ) == true && EventCheck( 0.35f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 1500.f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.9f ) == true && EventCheck( 0.9f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 0.f;
		m_PhysicParam.nowSpeed.x = 1000.f;
	}

	CommonFrameMoveFuture();
}


void CX2GURaven::RSI_SI2_POWER_ASSAULT_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.08f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.08f ) == true && EventCheck( 0.08f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqPowerAssault[0] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Raven_PowerAssault01", D3DXVECTOR3(0, 0, 0) );
		m_hSeqPowerAssault[1] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Raven_PowerAssault02", D3DXVECTOR3(0, 0, 0) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.466f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.466f ) == true && EventCheck( 0.466f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqPowerAssault[2] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Raven_PowerAssault03", D3DXVECTOR3(0, 0, 0) );
	}


	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPowerAssault[0] )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPowerAssault[0] );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE2_Lhand2" );
			D3DXVECTOR3 vDirVec = GetDirVector();
			bool bIsRight = GetIsRight();

			D3DXVECTOR3 vZVec;
			vZVec.y = 0.f;
			vZVec.z = vDirVec.x;
			vZVec.x = -vDirVec.z;
			D3DXVec3Normalize( &vZVec, &vZVec );

			D3DXVECTOR3 vPos;
			if( true == bIsRight )
				vPos = vBonePos + vDirVec * -10.f;
			else
				vPos = vBonePos + vDirVec * 10.f;
			vPos += vZVec * -30.f;

			pSeq->SetPosition( vPos );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );

		}
		else
		{
			m_hSeqPowerAssault[0]  = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}

	}


	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPowerAssault[1] )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPowerAssault[1] );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE2_Lhand2" );

			pSeq->SetPosition( vBonePos );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqPowerAssault[1]  = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}




	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPowerAssault[2] )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPowerAssault[2] );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE2_Lhand2" );
			D3DXVECTOR3 vDirVec = GetDirVector();
			bool bIsRight = GetIsRight();

			D3DXVECTOR3 vPos;
			if( true == bIsRight )
				vPos = vBonePos + vDirVec * 200.f;
			else
				vPos = vBonePos + vDirVec * -200.f;

			pSeq->SetPosition( vPos );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		else
		{
			m_hSeqPowerAssault[2]  = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

//#ifndef BALANCE_BLADE_MASTER_20130117		/// 데미지 이펙트로 변경
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		m_DamageData.reActType 			= CX2DamageManager::RT_FLY;
		m_DamageData.backSpeed 			= D3DXVECTOR2( 1500.f, 1000.f );
		m_DamageData.attackType			= CX2DamageManager::AT_SPECIAL;
		
		m_DamageData.fCameraCrashGap	= 5.f;
		m_DamageData.fCameraCrashTime	= 0.2f;
		m_DamageData.bReAttack			= false;
		//m_DamageData.fHitGap			= 0.1f;

	}
//#endif BALANCE_BLADE_MASTER_20130117


	CommonFrameMove();
}

void CX2GURaven::RSI_SI2_POWER_ASSAULT_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}



//RSI_SI3_SEVEN_BURST
void CX2GURaven::RSI_SI3_SEVEN_BURST_Init()
{
	TextureReadyInBackground( L"DarkSmoke01.dds" );
}

void CX2GURaven::RSI_SI3_SEVEN_BURST_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 2.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 2.633f ) == true && EventCheck( 2.633f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = -700.f;
	}

	CommonFrameMoveFuture();

}

void CX2GURaven::RSI_SI3_SEVEN_BURST_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.833f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.833f ) == true && EventCheck( 0.833f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst05", vBonePos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.033f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.033f ) == true && EventCheck( 1.033f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.233f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.233f ) == true && EventCheck( 1.233f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.433f ) == true && EventCheck( 1.433f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.633f ) == true && EventCheck( 1.633f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.833f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.833f ) == true && EventCheck( 1.833f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.f ) == true && EventCheck( 2.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqSevenBurst[0] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst04", D3DXVECTOR3(0, 0, 0) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.3f ) == true && EventCheck( 2.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqSevenBurst[1] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst04", D3DXVECTOR3(0, 0, 0) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.566f ) == true && EventCheck( 2.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 35.f;
		else
			vPos = vBonePos + GetDirVector() * -35.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_2", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.633f ) == true && EventCheck( 2.633f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -60.f;
		else
			vPos = vBonePos + vDirVec * 60.f;
		vPos.y += 10.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_SevenBurst07", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_MaximumCanon05", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		m_hSeqSevenBurst[2] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst04", D3DXVECTOR3(0, 0, 0) );

		UpDownCrashCameraNoReset( 5.0f, 1.f );
	}


	for( int i=0; i<3; i++ )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqSevenBurst[i] )
		{

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSevenBurst[i]  );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE1_Lhand" );

				pSeq->SetPosition( vBonePos );
				pSeq->SetAxisAngle( GetRotateDegree() );
				pSeq->SetAddRotate( GetRotateDegree() );
			}
			else
			{
				m_hSeqSevenBurst[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SI3_SEVEN_BURST_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

//RSI_SI3_SEVEN_BURST_HYPER
void CX2GURaven::RSI_SI3_SEVEN_BURST_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 2.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 2.633f ) == true && EventCheck( 2.633f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = -700.f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SI3_SEVEN_BURST_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.833f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.833f ) == true && EventCheck( 0.833f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst05", vBonePos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.033f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.033f ) == true && EventCheck( 1.033f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.233f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.233f ) == true && EventCheck( 1.233f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.433f ) == true && EventCheck( 1.433f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.633f ) == true && EventCheck( 1.633f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.833f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.833f ) == true && EventCheck( 1.833f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 50.f;
		else
			vPos = vBonePos + GetDirVector() * -50.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_1_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.f ) == true && EventCheck( 2.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqSevenBurst[0] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst04", D3DXVECTOR3(0, 0, 0) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.3f ) == true && EventCheck( 2.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqSevenBurst[1] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst04", D3DXVECTOR3(0, 0, 0) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.566f ) == true && EventCheck( 2.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + GetDirVector() * 35.f;
		else
			vPos = vBonePos + GetDirVector() * -35.f;
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SEVEN_BURST_2_HYPER", GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.633f ) == true && EventCheck( 2.633f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vBonePos = GetBonePos( L"Dummy2_Lhand" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = vBonePos + vDirVec * -60.f;
		else
			vPos = vBonePos + vDirVec * 60.f;
		vPos.y += 10.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_SevenBurst07", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_MaximumCanon05", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		m_hSeqSevenBurst[2] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_SevenBurst04", D3DXVECTOR3(0, 0, 0) );

		UpDownCrashCameraNoReset( 5.0f, 1.f );
	}



	for( int i=0; i<3; i++ )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqSevenBurst[i] )
		{

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSevenBurst[i] );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE1_Lhand" );

				pSeq->SetPosition( vBonePos );
				pSeq->SetAxisAngle( GetRotateDegree() );
				pSeq->SetAddRotate( GetRotateDegree() );
			}
			else
			{
				m_hSeqSevenBurst[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_SI3_SEVEN_BURST_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}



// RSI_SI1_EARTH_BREAKER
void CX2GURaven::RSI_SI1_EARTH_BREAKER_Init()
{
	XSkinMeshReadyInBackground( L"EarthBreaker_Mesh01.X" );
	XMeshReadyInBackground( L"Thief_Fat_JumpAttackA_Stone02.Y" );
}

void CX2GURaven::RSI_SI1_EARTH_BREAKER_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈	
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vZVec;
		vZVec.y = 0.f;
		vZVec.z = vDirVec.x;
		vZVec.x = -vDirVec.z;
		D3DXVec3Normalize( &vZVec, &vZVec );

		D3DXVECTOR3 vEffPos;
		if( true == bIsRight )
			vEffPos = GetPos() + vDirVec * 40.f;
		else
			vEffPos = GetPos() + vDirVec * -40.f;
		vEffPos += vZVec * -45.f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_EarthBreaker01_Mesh", vEffPos, GetRotateDegree(), GetRotateDegree() );
	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.433f ) == true && EventCheck( 0.433f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 10.0f, 0.4f );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = GetPos() + vDirVec * 45.f;
		else
			vPos = GetPos() + vDirVec * -45.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"RespawnLanding_P02", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Stone_Raven_EarthBreaker02", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Stone_Raven_EarthBreaker03", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
		
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO2 ) == true && GetRandomFloat() <= 0.7f)
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_INVISIBLE_MEMO", GetPowerRate(), vPos + D3DXVECTOR3(0, 50, 0), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_INVISIBLE", GetPowerRate(), vPos + D3DXVECTOR3(0, 50, 0), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_INVISIBLE", GetPowerRate(), vPos + D3DXVECTOR3(0, 50, 0), 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif

		//D3DXVECTOR3 vLeftPos = vPos + vDirVec * -150.f;
		//D3DXVECTOR3 vRightPos = vPos + vDirVec * 150.f;

		//vLeftPos.y	+= 250.f;
		//vRightPos.y += 250.f;

		
		//g_pX2Game->GetDamageEffect()->CreateInstance( (CKTDGObject*) this,  (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE", GetPowerRate(), vLeftPos, 
		//	GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );


		D3DXVECTOR3 vEffPos = GetPos() + D3DXVECTOR3( 0, 250, 0 );

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO2 ) == true && GetRandomFloat() <= 0.7f )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE_MEMO", GetPowerRate(), vEffPos, 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE", GetPowerRate(), vEffPos, 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE", GetPowerRate(), vEffPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 10.0f, 0.4f );

		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = GetPos() + vDirVec * 45.f;
		else
			vPos = GetPos() + vDirVec * -45.f;

		D3DXVECTOR3 vLeftPos = vPos + vDirVec * -150.f;
		D3DXVECTOR3 vRightPos = vPos + vDirVec * 150.f;

		vLeftPos.y += 5.f;
		vRightPos.y += 5.f;

		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"DownSmoke", vLeftPos, 100, 100, 1, 7 );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"DownSmoke", vRightPos, 100, 100, 1, 7 );

		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"GroundShockWave", vLeftPos, 100, 100, 1, 1 );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"GroundShockWave", vRightPos, 100, 100, 1, 1 );

	}

	CommonFrameMove();

}

void CX2GURaven::RSI_SI1_EARTH_BREAKER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

// RSI_SI1_EARTH_BREAKER_HYPER
void CX2GURaven::RSI_SI1_EARTH_BREAKER_HYPER_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈	
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vZVec;
		vZVec.y = 0.f;
		vZVec.z = vDirVec.x;
		vZVec.x = -vDirVec.z;
		D3DXVec3Normalize( &vZVec, &vZVec );

		D3DXVECTOR3 vEffPos;
		if( true == bIsRight )
			vEffPos = GetPos() + vDirVec * 40.f;
		else
			vEffPos = GetPos() + vDirVec * -40.f;
		vEffPos += vZVec * -45.f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_EarthBreaker01_Mesh", vEffPos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.433f ) == true && EventCheck( 0.433f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 10.0f, 0.4f );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = GetPos() + vDirVec * 45.f;
		else
			vPos = GetPos() + vDirVec * -45.f;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"RespawnLanding_P02", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Stone_Raven_EarthBreaker02", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Stone_Raven_EarthBreaker03", vPos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO2 ) == true && GetRandomFloat() <= 0.7f )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_INVISIBLE_MEMO", GetPowerRate(), vPos + D3DXVECTOR3(0, 50, 0), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_INVISIBLE", GetPowerRate(), vPos + D3DXVECTOR3(0, 50, 0), 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_INVISIBLE", GetPowerRate(), vPos + D3DXVECTOR3(0, 50, 0), 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif

		//D3DXVECTOR3 vLeftPos = vPos + vDirVec * -150.f;
		//D3DXVECTOR3 vRightPos = vPos + vDirVec * 150.f;

		//vLeftPos.y	+= 250.f;
		//vRightPos.y += 250.f;

		//g_pX2Game->GetDamageEffect()->CreateInstance( (CKTDGObject*) this,  (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE", GetPowerRate(), vLeftPos, 
		//	GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		D3DXVECTOR3 vEffPos = GetPos() + D3DXVECTOR3( 0, 250, 0 );

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO2 ) == true && GetRandomFloat() <= 0.7f )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE_MEMO", GetPowerRate(), vEffPos, 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE", GetPowerRate(), vEffPos, 
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_EARTH_BREAKER_STONE", GetPowerRate(), vEffPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 10.0f, 0.4f );

		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == bIsRight )
			vPos = GetPos() + vDirVec * 45.f;
		else
			vPos = GetPos() + vDirVec * -45.f;

		D3DXVECTOR3 vLeftPos = vPos + vDirVec * -150.f;
		D3DXVECTOR3 vRightPos = vPos + vDirVec * 150.f;

		vLeftPos.y += 5.f;
		vRightPos.y += 5.f;

		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"DownSmoke", vLeftPos, 100, 100, 1, 7 );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"DownSmoke", vRightPos, 100, 100, 1, 7 );

		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"GroundShockWave", vLeftPos, 100, 100, 1, 1 );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"GroundShockWave", vRightPos, 100, 100, 1, 1 );

	}

	CommonFrameMove();

}

void CX2GURaven::RSI_SI1_EARTH_BREAKER_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}


// RSI_SI2_CANNON_BLADE
void CX2GURaven::RSI_SI2_CANNON_BLADE_Init()
{
	TextureReadyInBackground( L"Raven_CanonBlade01.dds" );
}

void CX2GURaven::RSI_SI2_CANNON_BLADE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqCannonBlade = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_MaximumCanon05", GetBonePos( L"Bip01_L_Hand" ) );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO1 ) == true )
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE_MEMO", GetPowerRate(), GetBonePos( L"Bip01" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE_MEMO", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		}
		else
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE", GetPowerRate(), GetBonePos( L"Bip01" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}

	if( m_hSeqCannonBlade != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqCannonBlade );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"ATTACK_SPHERE1_Lhand1" ) );
		}
		else
		{
			m_hSeqCannonBlade = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SI2_CANNON_BLADE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

// RSI_SI2_CANNON_BLADE_HYPER
void CX2GURaven::RSI_SI2_CANNON_BLADE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSeqCannonBlade = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_MaximumCanon05", GetBonePos( L"Bip01_L_Hand" ) );
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO1 ) == true )
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE_MEMO", GetPowerRate(), GetBonePos( L"Bip01" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE_MEMO", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		
		}
		else
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE", GetPowerRate(), GetBonePos( L"Bip01" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_CANNON_BLADE", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
	}

	if( m_hSeqCannonBlade != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqCannonBlade );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"ATTACK_SPHERE1_Lhand1" ) );
		}
		else
		{
			m_hSeqCannonBlade = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SI2_CANNON_BLADE_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
//RSI_SI3_MAGNUM_BLASTER
void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_Init()
{
	XSkinMeshReadyInBackground( L"MagnumBlaster_Mesh01.X" );
	TextureReadyInBackground( L"MagnumBlaster01_Mesh.tga" );
}

void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.27f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.27f ) == true && EventCheck( 0.27f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.7f ) == true && EventCheck( 0.7f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 1.2f ) == true && EventCheck( 1.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.1f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		m_DamageData.bReAttack = true;
		m_DamageData.fHitGap = 0.1f;
		SetEnableAttackBox( L"Lhand1", true );
		SetEnableAttackBox( L"Sword", false );
		m_DamageData.backSpeed.x = 0.0f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_L_Hand" );
		m_hSeqMagnumBlaster1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_MagnumBlaster06", vBonePos );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_MagnumBlaster01_Mesh", vBonePos, GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMesh_MagnumBlaster2 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.7f ) == true && EventCheck( 2.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE1_Lhand1" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + vDirVec * 40.f;
		else
			vPos = vBonePos + vDirVec * -40.f;

		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_MagnumBlaster04", vPos );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_MagnumBlaster05", vPos );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.6f ) == true && EventCheck( 2.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_MAGNUM_BLASTER", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}


	if( m_hSeqMagnumBlaster1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqMagnumBlaster1 );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"Bip01_L_Hand" ) );
		}
		else
		{
			m_hSeqMagnumBlaster1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}


	if( m_hMesh_MagnumBlaster2 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMesh_MagnumBlaster2 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_L_Hand" ) );
		}
		else
		{
			m_hMesh_MagnumBlaster2 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_End()
{
	SetEnableAttackBox( L"Lhand1", false );
	SetEnableAttackBox( L"Sword", true );
	CommonStateEnd();
}

//RSI_SI3_MAGNUM_BLASTER_HYPER
void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.27f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.27f ) == true && EventCheck( 0.27f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.7f ) == true && EventCheck( 0.7f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 1.2f ) == true && EventCheck( 1.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.1f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		
		m_DamageData.bReAttack = true;
		m_DamageData.fHitGap = 0.1f;
		SetEnableAttackBox( L"Lhand1", true );
		SetEnableAttackBox( L"Sword", false );
		m_DamageData.backSpeed.x = 0.0f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_L_Hand" );
		m_hSeqMagnumBlaster1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_MagnumBlaster06", vBonePos );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_MagnumBlaster01_Mesh", vBonePos, GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMesh_MagnumBlaster2 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.7f ) == true && EventCheck( 2.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"ATTACK_SPHERE1_Lhand1" );
		D3DXVECTOR3 vDirVec = GetDirVector();
		bool bIsRight = GetIsRight();

		D3DXVECTOR3 vPos;
		if( true == GetIsRight() )
			vPos = vBonePos + vDirVec * 40.f;
		else
			vPos = vBonePos + vDirVec * -40.f;

		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_MagnumBlaster04", vPos );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_MagnumBlaster05", vPos );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.6f ) == true && EventCheck( 2.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_MAGNUM_BLASTER", GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}



	if( m_hSeqMagnumBlaster1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqMagnumBlaster1 );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"Bip01_L_Hand" ) );
		}
		else
		{
			m_hSeqMagnumBlaster1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}




	if( m_hMesh_MagnumBlaster2 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMesh_MagnumBlaster2 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_L_Hand" ) );
		}
		else
		{
			m_hMesh_MagnumBlaster2 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SI3_MAGNUM_BLASTER_HYPER_End()
{
	SetEnableAttackBox( L"Lhand1", false );
	SetEnableAttackBox( L"Sword", true );
	CommonStateEnd();
}
#endif //UPGRADE_SKILL_SYSTEM_2013

/** @function : COMMON_BUFF_FrameMove
@brief : 공통적으로 쓰는 버프 스킬의 FrameMove
*/
/*virtual*/ void CX2GURaven::COMMON_BUFF_FrameMove()
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 // 공통 스킬 개편, 김종훈
	// 각성 인지 아닌지에 따라 World색 변경
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.004f, 0 );
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.83f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.83f ) == true && EventCheck( 0.83f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		PlayCommonBuffMinorParticle();
		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.85f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.85f ) == true && EventCheck( 0.85f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		int	iSkillSlotIndex = 0;
		bool bSlotB = false;

		if ( true == GetSkillSlotIndexUsed( iSkillSlotIndex, bSlotB ) )
		{
			const CX2Unit::UnitData* pUnitData = &GetUnit()->GetUnitData();

			const CX2UserSkillTree::SkillSlotData* pSkillSlotData = pUnitData->m_UserSkillTree.GetSkillSlot( iSkillSlotIndex, bSlotB );
			CX2SkillTree::SKILL_ID eSkillID = CX2SkillTree::SI_NONE;

			if( NULL != pSkillSlotData )
				eSkillID = pSkillSlotData->m_eID;

			ProcessAcceleraterBuff( eSkillID, IsHyperState() );
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SOUL_TAKER_DASH_COMBO_ZZ_EventProcess()
{
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f 
		&& true == IsOnSomethingFuture() && CX2Unit::UC_RAVEN_VETERAN_COMMANDER != GetUnit()->GetUnitData().m_UnitClass )
#else  SERV_RAVEN_VETERAN_COMMANDER
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f
		&& true == IsOnSomethingFuture() )
#endif SERV_RAVEN_VETERAN_COMMANDER

	{
		StateChange( RSI_SOUL_TAKER_DASH_COMBO_ZZZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.5f && 
			 CX2Unit::UC_RAVEN_VETERAN_COMMANDER != GetUnit()->GetUnitData().m_UnitClass )
#else  SERV_RAVEN_VETERAN_COMMANDER
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.5f )
#endif SERV_RAVEN_VETERAN_COMMANDER
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	ELSE_IF_STATE_CHANGE_ON_( 1, 100.f, 0.43f, m_InputData.oneZ && CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnit()->GetUnitData().m_UnitClass, RVC_DASH_COMBO_ZZZ )
#endif SERV_RAVEN_VETERAN_COMMANDER
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
		{
			m_fCanNotAttackTime = 0.1f;	
			StateChange( USI_WAIT );
		}
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


void CX2GURaven::RSI_SOUL_TAKER_DASH_COMBO_ZZZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.84f )
		{
			m_fCanNotAttackTime = 0.1f;	
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


void CX2GURaven::RSI_SOUL_TAKER_COMBO_ZZZupZ_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.5f ) == true && EventCheck( 0.5f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.y = 1500.f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SOUL_TAKER_COMBO_ZZZupZ_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_JUMP_LANDING );		
	}	

	CommonEventProcess();
}



void CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZ_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	else if( m_iSuccessiveHit_OneZ >= 2 && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.87f )
	{
		m_iSuccessiveHit_OneZ = 0;
		StateChange( RSI_SOUL_TAKER_COMBO_XXZZ8 );
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.87f )
	{
		m_iSuccessiveHit_OneZ++;
	}

	CommonEventProcess();
}

#ifdef BALANCE_BLADE_MASTER_20130117		/// 슈퍼아머 색상 변경 때문에 추가
void CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8_StateStart()
{
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}
#endif BALANCE_BLADE_MASTER_20130117

void CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8_FrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.047f ) == true /*&& EventCheck( 0.047f, false ) == true*/ )
	{
		PlaySound( L"Raven_Slash1.ogg" );
	}
	else if( m_pXSkinAnim->EventTimer( 0.276f ) == true /*&& EventCheck( 0.276f, false ) == true*/ )
	{
		PlaySound( L"Raven_Slash2.ogg" );
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8_EventProcess()
{

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->GetNowPlayCount() > 4 )
	{
		StateChange( RSI_SOUL_TAKER_COMBO_XXZZ8Z );
	}
#ifndef BALANCE_BLADE_MASTER_20130117		/// Z연타 콤보는 입력에 상관없이 무조건 발동
	else if( m_iSuccessiveHit_OneZ < 2 && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.33f )	// Z를 연타하지 않으면 바로 콤보모션 끝내기
	{
		StateChange( RSI_SOUL_TAKER_COMBO_XXZZ8Z );	
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.35f )
	{
		m_iSuccessiveHit_OneZ = 0;
	}		
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.33f )
	{
		m_iSuccessiveHit_OneZ++;
	}
#endif BALANCE_BLADE_MASTER_20130117

	CommonEventProcess();
}

void CX2GURaven::RSI_SOUL_TAKER_COMBO_XXZZ8Z_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_BALANCE_PATCH

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.43f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.43f && SpecialAttackEventProcess() == true )			
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
#endif
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();

}


void CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownX_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.38f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.38f ) == true && EventCheck( 0.38f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = 300.f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )			
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && 
		m_pXSkinAnimFuture->GetNowAnimationTime() > 0.57f )
	{
		StateChange( RSI_OVER_TAKER_COMBO_XXdownXdownX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureRight == true &&
		m_pXSkinAnimFuture->GetNowAnimationTime() < 0.57f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_InputData.oneX == true && m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureLeft == true &&
		m_pXSkinAnimFuture->GetNowAnimationTime() < 0.57f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownX_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.46f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.46f ) == true && EventCheck( 0.46f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = 300.f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )			
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && 
		m_pXSkinAnimFuture->GetNowAnimationTime() > 0.733f )
	{
		StateChange( RSI_OVER_TAKER_COMBO_XXdownXdownXdownX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureRight == true &&
		m_pXSkinAnimFuture->GetNowAnimationTime() < 0.733f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_InputData.oneX == true && m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureLeft == true &&
		m_pXSkinAnimFuture->GetNowAnimationTime() < 0.733f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownXdownX_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.42f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.42f ) == true && EventCheck( 0.42f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = 0.f;		
	}

	CommonFrameMoveFuture();
}


void CX2GURaven::RSI_OVER_TAKER_COMBO_XXdownXdownXdownX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}



void CX2GURaven::RSI_OVER_TAKER_COMBO_XXfrontX_Init()
{
	XSkinMeshReadyInBackground( L"Raven_Iron_Attack01.X" );
}

void CX2GURaven::RSI_OVER_TAKER_COMBO_XXfrontX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += 50.f * GetDirVector();
		}
		else
		{
			vPos -= 50.f * GetDirVector();
		}

		CKTDGParticleSystem::CParticleEventSequence* pSeq;
		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Raven_attackX3Front_Particle01", vPos );
		if( NULL != pSeq )
		{
			//pSeq->SetAddRotate( GetRotateDegree() );
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.3666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.3666f ) && EventCheck( 0.3666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 7.0f ) == true )
		{
			D3DXVECTOR3 vPos = GetPos();

			if( true == GetIsRight() )
			{
				vPos += 60.f * GetDirVector();
			}
			else
			{
				vPos -= 60.f * GetDirVector();
			}

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_OVER_TAKER_XX_FRONT_X", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
		}
		else
		{
			D3DXVECTOR3 vPos = GetPos();
			if( true == GetIsRight() )
			{
				vPos += 60.f * GetDirVector();
			}
			else
			{
				vPos -= 60.f * GetDirVector();
			}
			vPos.y += 50.f;


			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				D3DXVECTOR3 angle = GetRotateDegree();
				angle.z = 45.0f;

				PlaySequenceByTriggerCount( pSeq, vPos, 100, 100, 1 );

				pSeq->SetAxisAngle( angle );
				pSeq->SetAddRotate( angle );
			}
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.4333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.4333f ) && EventCheck( 0.4333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 5.0f, 0.15f );	
	}

	CommonFrameMove();
}


void CX2GURaven::RSI_OVER_TAKER_COMBO_XXfrontX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true	
		|| m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.85f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.267f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.267f ) == true && EventCheck( 0.267f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed = D3DXVECTOR2( 0, 0 );
		m_PhysicParam.passiveSpeed = D3DXVECTOR2( 2200.f, -2200.f * 1.2f );
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_FrameMove()
{
	CommonFrameMove();
}

void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING );
	}

	CommonEventProcess();
}






void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_StartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed = D3DXVECTOR2( 0.f, 0.f );
	m_PhysicParam.passiveSpeed = D3DXVECTOR2( 0.f, 0.f );
}

void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_Start()
{
	CommonStateStart();

	UpDownCrashCamera( 20.0f, 0.3f );	
	CreateStepDust();

}

void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZX_LANDING_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.1f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.3f )
	{
		if( m_InputData.oneX == true )
		{
			StateChange( RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX );
		}
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.4f )
	{
		if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChange( USI_WALK );
		}
		else if( m_InputData.pureUp == true )
		{
			StateChange( USI_JUMP_READY );
		}
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


void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX_FrameMove()
{
	CommonFrameMove();
}

void CX2GURaven::RSI_OVER_TAKER_DASH_JUMP_COMBO_ZXX_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


// 필살기
void CX2GURaven::RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_Init()
{
	XSkinMeshReadyInBackground( L"BerserKerBlade02.X" );
	XSkinMeshReadyInBackground( L"BerserKerBlade03.X" );
	XSkinMeshReadyInBackground( L"BerserKerBlade04.X" );

	TextureReadyInBackground( L"BerserkerBlade01.tga" );
	TextureReadyInBackground( L"BerserkerBlade02.tga" );
}

void CX2GURaven::RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

	wstring wstrBerserkerBlade = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE";
	wstring wstrBerserkerBladeDown = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE_DOWN";
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO9 ) == true )
	{
		wstrBerserkerBlade = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE_MEMO";
		wstrBerserkerBladeDown = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE_DOWN_MEMO";
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * -50.f;
		}
		else
		{
			vPos -= GetDirVector() * -50.f;
		}

		CX2DamageEffect::CEffect* pEffectBerseker = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrBerserkerBlade.c_str(), GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		if( NULL != pEffectBerseker )
		{
			m_hMeshBerserkerBlade = pEffectBerseker->GetMainEffectHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6333f ) == true && EventCheck( 0.6333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
		if( true == GetIsRight() )
		{
			vBonePos += GetDirVector() * 10.f;
		}
		else
		{
			vBonePos -= GetDirVector() * 10.f;
		}

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade1 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade04", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade2 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.966f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.966f ) == true && EventCheck( 0.966f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade3 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * 180.f;
		}
		else
		{
			vPos -= GetDirVector() * 180.f;
		}
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Particle_BerserkerBlade01", vPos );

		UpDownCrashCamera( 10.0f, 0.5f );	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7333f ) == true && EventCheck( 0.7333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * -50.f;
		}
		else
		{
			vPos -= GetDirVector() * -50.f;
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrBerserkerBlade.c_str(), GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8666f ) == true && EventCheck( 0.8666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * -50.f;
		}
		else
		{
			vPos -= GetDirVector() * -50.f;
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrBerserkerBladeDown.c_str(), GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}


	if( m_hMeshBerserkerBlade1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade1 );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
			if( true == GetIsRight() )
			{
				vBonePos += GetDirVector() * 10.f;
			}
			else
			{
				vBonePos -= GetDirVector() * 10.f;
			}

			pMeshInst->SetPos( vBonePos );
		}
		else
		{
			m_hMeshBerserkerBlade1 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	if( m_hMeshBerserkerBlade2 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade2 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade2 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	if( m_hMeshBerserkerBlade3 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade3 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade3 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}



	if( m_hMeshBerserkerBlade != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vPos = GetPos();
			if( true == GetIsRight() )
			{
				vPos += GetDirVector() * -50.f;
			}
			else
			{
				vPos -= GetDirVector() * -50.f;
			}

			pMeshInst->SetPos( vPos );
		}
		else
		{
			m_hMeshBerserkerBlade = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_HYPER_FrameMove()
{

// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

	wstring wstrBerserkerBlade = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE";
	wstring wstrBerserkerBladeDown = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE_DOWN";
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO9 ) == true )
	{
		wstrBerserkerBlade = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE_MEMO";
		wstrBerserkerBladeDown = L"RAVEN_SOUL_TAKER_BERSERKER_BLADE_DOWN_MEMO";
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * -50.f;
		}
		else
		{
			vPos -= GetDirVector() * -50.f;
		}

		CX2DamageEffect::CEffect* pEffectBerseker = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrBerserkerBlade.c_str(), GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		if( NULL != pEffectBerseker )
		{
			m_hMeshBerserkerBlade = pEffectBerseker->GetMainEffectHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6333f ) == true && EventCheck( 0.6333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
		if( true == GetIsRight() )
		{
			vBonePos += GetDirVector() * 10.f;
		}
		else
		{
			vBonePos -= GetDirVector() * 10.f;
		}

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade1 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade04", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade2 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.966f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.966f ) == true && EventCheck( 0.966f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade3 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * 180.f;
		}
		else
		{
			vPos -= GetDirVector() * 180.f;
		}
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Particle_BerserkerBlade01", vPos );

		UpDownCrashCamera( 10.0f, 0.5f );	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7333f ) == true && EventCheck( 0.7333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * -50.f;
		}
		else
		{
			vPos -= GetDirVector() * -50.f;
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrBerserkerBlade.c_str(), GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8666f ) == true && EventCheck( 0.8666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos += GetDirVector() * -50.f;
		}
		else
		{
			vPos -= GetDirVector() * -50.f;
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrBerserkerBladeDown.c_str(), GetPowerRate(), vPos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

	}


	if( m_hMeshBerserkerBlade1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade1 );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
			if( true == GetIsRight() )
			{
				vBonePos += GetDirVector() * 10.f;
			}
			else
			{
				vBonePos -= GetDirVector() * 10.f;
			}

			pMeshInst->SetPos( vBonePos );
		}
		else
		{
			m_hMeshBerserkerBlade1 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	if( m_hMeshBerserkerBlade2 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade2 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade2 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	if( m_hMeshBerserkerBlade3 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade3 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade3 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}



	if( m_hMeshBerserkerBlade != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vPos = GetPos();
			if( true == GetIsRight() )
			{
				vPos += GetDirVector() * -50.f;
			}
			else
			{
				vPos -= GetDirVector() * -50.f;
			}

			pMeshInst->SetPos( vPos );
		}
		else
		{
			m_hMeshBerserkerBlade = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	CommonFrameMove();

}

void CX2GURaven::RSI_SI1_SOUL_TAKER_BERSERKER_BLADE_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SI2_SOUL_TAKER_FLYING_IMPACT_Init()
{
	XSkinMeshReadyInBackground( L"BerserkerBlade_Weapon.X" );

	TextureReadyInBackground( L"BerserkerBlade01.tga" );
	TextureReadyInBackground( L"BerserkerBlade02.tga" );
}

void CX2GURaven::RSI_SI2_SOUL_TAKER_FLYING_IMPACT_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshFlyingImpact );

#ifdef SERV_SKILL_NOTE
		wstring wstrEffectName = L"RAVEN_SOUL_TAKER_FLYING_IMPACT";
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO3 ) == true )
		{
			wstrEffectName = L"RAVEN_SOUL_TAKER_FLYING_IMPACT_MEMO";
		}
		CX2DamageEffect::CEffect* pEffectFlyingImpact = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrEffectName.c_str(), GetPowerRate(), D3DXVECTOR3(0,0,0), 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else
		CX2DamageEffect::CEffect* pEffectFlyingImpact = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SOUL_TAKER_ARC_ENEMY", GetPowerRate(), D3DXVECTOR3(0,0,0), 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif
		
		if( NULL != pEffectFlyingImpact )
		{
            if ( CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pEffectFlyingImpact->GetMainEffect() )
			{
				m_hMeshFlyingImpact = pMeshInstance->GetHandle();
				pMeshInstance->SetBoundingRadius( 0.f );
			}
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.8f ) == true && EventCheck( 2.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( m_hMeshFlyingImpact != INVALID_MESH_INSTANCE_HANDLE )
		{
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshFlyingImpact );
		}
	}



	if( m_hMeshFlyingImpact != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshFlyingImpact );
		if( NULL != pMeshInst )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
			/// 기존의 동작은 Dummy1_Rhand의 행렬을 가져와서 적용하기 때문에, pMeshInst의 값이 적용되지 않는다. ( 스케일 값 )
			/// 그렇기 때문에 pMeshInst의 스케일 값을 행렬로 변환하여 곱해준다면, 회전되는 행렬에 매시 스케일을 적용할 수 있다.
			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat );
			D3DXMatrixScaling( &mat, pMeshInst->GetSize().x, pMeshInst->GetSize().y, pMeshInst->GetSize().z );
			pMeshInst->SetUseDXMatrix( mat * pMultiAnimFrame->combineMatrix );
#else  BALANCE_ELEMENTAL_MASTER_20130117
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
#endif BALANCE_ELEMENTAL_MASTER_20130117
		}	
		else
		{
			m_hMeshFlyingImpact = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	CommonFrameMove();

}

void CX2GURaven::RSI_SI2_SOUL_TAKER_FLYING_IMPACT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}




void CX2GURaven::RSI_SI2_SOUL_TAKER_FLYING_IMPACT_End()
{
	CommonStateEnd();

	if( m_hMeshFlyingImpact != INVALID_MESH_INSTANCE_HANDLE )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshFlyingImpact );
	}
}




void CX2GURaven::RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshFlyingImpact );

#ifdef SERV_SKILL_NOTE
		wstring wstrEffectName = L"RAVEN_SOUL_TAKER_FLYING_IMPACT";
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO3 ) == true )
		{
			wstrEffectName = L"RAVEN_SOUL_TAKER_FLYING_IMPACT_MEMO";
		}
		CX2DamageEffect::CEffect* pEffectFlyingImpact = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, wstrEffectName.c_str(), GetPowerRate(), D3DXVECTOR3(0,0,0), 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else
		CX2DamageEffect::CEffect* pEffectFlyingImpact = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_SOUL_TAKER_ARC_ENEMY", GetPowerRate(), D3DXVECTOR3(0,0,0), 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif


		if( NULL != pEffectFlyingImpact )
		{
			if ( CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pEffectFlyingImpact->GetMainEffect() )
			{
				m_hMeshFlyingImpact = pMeshInstance->GetHandle();
				pMeshInstance->SetBoundingRadius( 0.f );
			}
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 2.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 2.8f ) == true && EventCheck( 2.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( m_hMeshFlyingImpact != INVALID_MESH_INSTANCE_HANDLE )
		{
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshFlyingImpact );
		}
	}




	if( m_hMeshFlyingImpact != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshFlyingImpact );
		if( NULL != pMeshInst )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
			/// 기존의 동작은 Dummy1_Rhand의 행렬을 가져와서 적용하기 때문에, pMeshInst의 값이 적용되지 않는다. ( 스케일 값 )
			/// 그렇기 때문에 pMeshInst의 스케일 값을 행렬로 변환하여 곱해준다면, 회전되는 행렬에 매시 스케일을 적용할 수 있다.
			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat );
			D3DXMatrixScaling( &mat, pMeshInst->GetSize().x, pMeshInst->GetSize().y, pMeshInst->GetSize().z );
			pMeshInst->SetUseDXMatrix( mat * pMultiAnimFrame->combineMatrix );
#else  BALANCE_ELEMENTAL_MASTER_20130117
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
#endif BALANCE_ELEMENTAL_MASTER_20130117
		}	
		else
		{
			m_hMeshFlyingImpact = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	CommonFrameMove();
}






void CX2GURaven::RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}


void CX2GURaven::RSI_SI2_SOUL_TAKER_FLYING_IMPACT_HYPER_End()
{
	CommonStateEnd();

	if( m_hMeshFlyingImpact != INVALID_MESH_INSTANCE_HANDLE )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshFlyingImpact );
	}
}



void CX2GURaven::RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.7f ) == true && EventCheck( 0.7f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 1000.f;			
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.f )
	{
		m_PhysicParam.passiveSpeed.x = 0.f;
	}

	CommonFrameMoveFuture();
}


void CX2GURaven::RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7666f ) == true && EventCheck( 0.7666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Footsteps" );

		m_hSeqSonicStab = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"White_Particle_HyperSonicStab02", vBonePos );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}


		vBonePos = GetBonePos( L"Bip01_R_Hand" );

		m_hSeqSonicStab1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Trace_HyperSonicStab03", vBonePos );
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab1 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
		if( true == GetIsRight() )
		{
			vBonePos += GetDirVector() * 10.f;
		}
		else
		{
			vBonePos -= GetDirVector() * 10.f;
		}

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade1 = pMeshInst->GetHandle();
		}


		g_pKTDXApp->GetDGManager()->ClearScreen( 10 );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.875f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.875f ) == true && EventCheck( 0.875f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade04", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade2 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade3 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.05f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.05f ) == true && EventCheck( 1.05f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 10.0f, 0.2f );	
	}




	if( m_hSeqSonicStab != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Footsteps" );
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqSonicStab = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}




	if( m_hSeqSonicStab1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab1 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_R_Hand" );
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqSonicStab1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}





	if( m_hMeshBerserkerBlade1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade1 );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
			if( true == GetIsRight() )
			{
				vBonePos += GetDirVector() * 10.f;
			}
			else
			{
				vBonePos -= GetDirVector() * 10.f;
			}

			pMeshInst->SetPos( vBonePos );
		}
		else
		{
			m_hMeshBerserkerBlade1 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	if( m_hMeshBerserkerBlade2 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade2 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade2 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	if( m_hMeshBerserkerBlade3 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade3 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade3 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	CommonFrameMove();

}

void CX2GURaven::RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_FrameMoveFuture()
{	

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.7f ) == true && EventCheck( 0.7f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.passiveSpeed.x = 1000.f;		
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.f )
	{
		m_PhysicParam.passiveSpeed.x = 0.f;
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7666f ) == true && EventCheck( 0.7666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Footsteps" );

		m_hSeqSonicStab = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"White_Particle_HyperSonicStab02", vBonePos );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}


		vBonePos = GetBonePos( L"Bip01_R_Hand" );

		m_hSeqSonicStab1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Trace_HyperSonicStab03", vBonePos );
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab1 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
		if( true == GetIsRight() )
		{
			vBonePos += GetDirVector() * 10.f;
		}
		else
		{
			vBonePos -= GetDirVector() * 10.f;
		}

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade1 = pMeshInst->GetHandle();
		}

		g_pKTDXApp->GetDGManager()->ClearScreen( 10 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.875f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.875f ) == true && EventCheck( 0.875f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade04", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade2 = pMeshInst->GetHandle();
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_SoulTaker_Sp1_BerserkerBlade03", 
			vBonePos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshBerserkerBlade3 = pMeshInst->GetHandle();
		}	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.05f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.05f ) == true && EventCheck( 1.05f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 10.0f, 0.2f );	
	}


	if( m_hSeqSonicStab != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Footsteps" );
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqSonicStab = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}




	if( m_hSeqSonicStab1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSonicStab1 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_R_Hand" );
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqSonicStab1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}




	if( m_hMeshBerserkerBlade1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade1 );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Neck" );
			if( true == GetIsRight() )
			{
				vBonePos += GetDirVector() * 10.f;
			}
			else
			{
				vBonePos -= GetDirVector() * 10.f;
			}

			pMeshInst->SetPos( vBonePos );
		}
		else
		{
			m_hMeshBerserkerBlade1 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	if( m_hMeshBerserkerBlade2 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade2 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade2 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	if( m_hMeshBerserkerBlade3 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshBerserkerBlade3 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Neck" ) );
		}
		else
		{
			m_hMeshBerserkerBlade3 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_SI3_SOUL_TAKER_HYPERSONIC_STAB_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}



void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_Init()
{
	TextureReadyInBackground( L"Raven_Overtaker_SP1a_ChargedBolt.tga" );
	TextureReadyInBackground( L"Raven_Overtaker_SP1a_ChargedBolt01.tga" );
	XSkinMeshReadyInBackground( L"Raven_OverTaker_SP1a_ChargedBolt.X" );
}

void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateStart()
{
	CommonStateStart();

//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	m_bDisableGravity = true;
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST
}


void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateStartFuture()
{
	CommonStateStartFuture();

//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	//m_PhysicParam.passiveSpeed.x = 0.f;
//	//m_PhysicParam.passiveSpeed.y = 0.f;
//	m_PhysicParam.nowSpeed.x = 0.f;
//	//m_PhysicParam.nowSpeed.y = 0.f;
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST

}

void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈
	// 차지드 볼트가 액티브로 변경됨
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // 청 스킬 개편, 김종훈

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vRotDegree = GetRotateDegree();
		D3DXVECTOR3 vPos = GetPos();

		if( false == GetIsRight() )
		{
			vRotDegree.y += 180.f;
			vPos += GetDirVector() * 30.f;
		}

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO10 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_CHARGED_BOLT_MEMO", GetPowerRate(), vPos, 
				vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_CHARGED_BOLT", GetPowerRate(), vPos, 
				vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos -= GetDirVector() * 20.f;
		}
		else
		{
			vPos += GetDirVector() * 20.f;
		}
		vPos.y += 50.f;

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_Overtaker_Sp1a_ChargedBolt02", vPos );
		pSeq->SetAxisAngle( GetRotateDegree() );
		pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 4.f, 0.3333f );
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	else if( m_pXSkinAnimFuture->EventTimer( 1.2f ) == true && EventCheck( 1.2f, true ) == true )
//	{
//		m_bDisableGravity = false;
//	}
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST

	CommonEventProcess();
}

void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_StateEnd()
{
	CommonStateEnd();

//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	m_bDisableGravity = false;
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST
}



void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateStartFuture()
{
	CommonStateStartFuture();

//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	//m_PhysicParam.passiveSpeed.x = 0.f;
//	//m_PhysicParam.passiveSpeed.y = 0.f;
//	m_PhysicParam.nowSpeed.x = 0.f;
//	//m_PhysicParam.nowSpeed.y = 0.f;
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST
}


void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateStart()
{
	CommonStateStart();

//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	m_bDisableGravity = true;
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST

}


void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈	
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vRotDegree = GetRotateDegree();
		D3DXVECTOR3 vPos = GetPos();

		if( false == GetIsRight() )
		{
			vRotDegree.y += 180.f;
			vPos += GetDirVector() * 30.f;
		}

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO10 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_CHARGED_BOLT_MEMO", GetPowerRate(), vPos, 
				vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_CHARGED_BOLT", GetPowerRate(), vPos, 
				vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
		{
			vPos -= GetDirVector() * 20.f;
		}
		else
		{
			vPos += GetDirVector() * 20.f;
		}
		vPos.y += 50.f;

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_Overtaker_Sp1a_ChargedBolt02", vPos );
		pSeq->SetAxisAngle( GetRotateDegree() );
		pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 4.f, 0.3333f );
	}

	CommonFrameMove();	

}


void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	else if( m_pXSkinAnimFuture->EventTimer( 1.2f ) == true && EventCheck( 1.2f, true ) == true )
//	{
//		m_bDisableGravity = false;
//	}
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST

	CommonEventProcess();
}


void CX2GURaven::RSI_SI1_OVER_TAKER_CHARGED_BOLT_HYPER_StateEnd()
{
	CommonStateEnd();

//#ifdef RAVEN_AIR_SPECIAL_ATTACK_TEST
//	m_bDisableGravity = false;
//#endif RAVEN_AIR_SPECIAL_ATTACK_TEST

}


void CX2GURaven::RSI_SI2_OVER_TAKER_ARC_ENEMY_Init()
{
	TextureReadyInBackground( L"Raven_Overtaker_SP2_FlyingImpact.tga" );
	XSkinMeshReadyInBackground( L"Raven_OT_SP2_FlyingImpact_Effect.X" );

	XSkinMeshReadyInBackground( L"Raven_OverTaker_SP2a_FlyingImpact_Start_01.X" );
	XSkinMeshReadyInBackground( L"Raven_OverTaker_SP2a_FlyingImpact_Start_02.X" );
	XSkinMeshReadyInBackground( L"Raven_OverTaker_SP2a_FlyingImpact_Start_03.X" );
	XSkinMeshReadyInBackground( L"Raven_OverTaker_SP2a_FlyingImpact_Start_04.X" );

}

void CX2GURaven::RSI_SI2_OVER_TAKER_ARC_ENEMY_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6666f ) == true && EventCheck( 0.6666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_01", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_02", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_03", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.1333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1333f ) == true && EventCheck( 1.1333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_04", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ClearArcEnemyData();

		D3DXVECTOR3 vRotDegree = GetRotateDegree();
		D3DXVECTOR3 vDirVec = GetDirVector();
		D3DXVECTOR3 vZVec = GetZVector();
		vDirVec.y = 0.f;

		D3DXVECTOR3 vPos = GetPos();
		if( false == GetIsRight() )
		{
			vRotDegree.y += 180.f;
		}

		for( int i=0; i<4; i++ )
		{
			CX2DamageEffect::CEffect* pSpear = NULL;

#ifdef SERV_SKILL_NOTE
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO4 ) == true )
			{
				pSpear = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_ARC_ENEMY_MEMO", GetPowerRate(), vPos, 
					vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
			else
			{
				pSpear = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_ARC_ENEMY", GetPowerRate(), vPos, 
					vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
#else
			pSpear = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_FLYING_IMPACT", GetPowerRate(), vPos, 
				vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
            CKTDGXMeshPlayer::CXMeshInstance* pSpearMeshInstance = pSpear->GetMainEffect();
            if ( pSpearMeshInstance != NULL )
			    pSpearMeshInstance->GetXSkinAnim()->Reset( (float) (rand()%10) * 0.1f );

			if( NULL != pSpear )
			{
				ArcEnemyData* pData = new ArcEnemyData;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pData->m_hEffect = pSpear->GetHandle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pData->m_pEffect = pSpear;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

				switch( i )
				{
				default:
				case 0:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -202*0.9f, 72*1.5f, 21 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, 0 );
					} break;

				case 1:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -198*0.9f, 117*1.5f, 21 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, -17 );
					} break;

				case 2:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -184*0.9f, 159*1.5f, 22 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, -36 );
					} break;

				case 3:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -149*0.9f, 192*1.5f, 21 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, -54 );
					} break;
				}

                if ( pSpearMeshInstance != NULL )
                {
				    pSpearMeshInstance->SetMoveAxisAngleDegree( GetRotateDegree() );
				    pSpearMeshInstance->SetRotateDegree( GetRotateDegree() + pData->m_vOffsetRotate );
                }

				D3DXVECTOR3 vTargetPos = GetPos(); 
				if( true == GetIsRight() )
				{
					vTargetPos += pData->m_vOffsetPos.x * vDirVec;
					vTargetPos.y += pData->m_vOffsetPos.y;
					vTargetPos += pData->m_vOffsetPos.z * vZVec;
				}
				else
				{
					vTargetPos -= pData->m_vOffsetPos.x * vDirVec;
					vTargetPos.y += pData->m_vOffsetPos.y;
					vTargetPos += pData->m_vOffsetPos.z * vZVec;
				}

                if ( pSpearMeshInstance != NULL )
				    pSpearMeshInstance->SetPos( vTargetPos );

				m_vecArcEnemyData.push_back( pData );
			}
		}

		m_TimerFlyingImpact.restart();
#ifdef BALANCE_PATCH_20110303
		m_fArcEnemyEachTime = 0.f;
#endif

	}

	CommonFrameMove();

}


void CX2GURaven::RSI_SI2_OVER_TAKER_ARC_ENEMY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SI2_OVER_TAKER_ARC_ENEMY_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6666f ) == true && EventCheck( 0.6666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_01", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_02", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_03", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.1333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1333f ) == true && EventCheck( 1.1333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
		pMeshInst  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_Overtaker_Sp2a_FlyingImpact_Start_04", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ClearArcEnemyData();

		D3DXVECTOR3 vRotDegree = GetRotateDegree();
		D3DXVECTOR3 vDirVec = GetDirVector();
		D3DXVECTOR3 vZVec = GetZVector();
		vDirVec.y = 0.f;

		D3DXVECTOR3 vPos = GetPos();
		if( false == GetIsRight() )
		{
			vRotDegree.y += 180.f;
		}

		for( int i=0; i<4; i++ )
		{
			CX2DamageEffect::CEffect* pSpear = NULL;
#ifdef SERV_SKILL_NOTE
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO4 ) == true )
			{
				pSpear = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_ARC_ENEMY_MEMO_HYPER", GetPowerRate(), vPos, 
					vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
			else
			{
				pSpear = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_ARC_ENEMY_HYPER", GetPowerRate(), vPos, 
					vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
			}
#else
			pSpear = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_FLYING_IMPACT_HYPER", GetPowerRate(), vPos, 
				vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );
#endif
			ASSERT( pSpear != NULL );
            CKTDGXMeshPlayer::CXMeshInstance* pSpearMeshInstance = pSpear->GetMainEffect();
            if ( pSpearMeshInstance != NULL )
			    pSpearMeshInstance->GetXSkinAnim()->Reset( (float) (rand()%10) * 0.1f );

			if( NULL != pSpear )
			{
				ArcEnemyData* pData = new ArcEnemyData;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pData->m_hEffect = pSpear->GetHandle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pData->m_pEffect = pSpear;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

				switch( i )
				{
				default:
				case 0:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -202*0.9f, 72*1.5f, 21 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, 0 );
					} break;

				case 1:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -198*0.9f, 117*1.5f, 21 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, -17 );
					} break;

				case 2:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -184*0.9f, 159*1.5f, 22 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, -36 );
					} break;

				case 3:
					{
						pData->m_vOffsetPos = D3DXVECTOR3( -149*0.9f, 192*1.5f, 21 );
						pData->m_vOffsetRotate = D3DXVECTOR3( 0, 0, -54 );
					} break;
				}

                if ( pSpearMeshInstance != NULL )
                {
				    pSpearMeshInstance->SetMoveAxisAngleDegree( GetRotateDegree() );
				    pSpearMeshInstance->SetRotateDegree( GetRotateDegree() + pData->m_vOffsetRotate );
                }

				D3DXVECTOR3 vTargetPos = GetPos(); 
				if( true == GetIsRight() )
				{
					vTargetPos += pData->m_vOffsetPos.x * vDirVec;
					vTargetPos.y += pData->m_vOffsetPos.y;
					vTargetPos += pData->m_vOffsetPos.z * vZVec;
				}
				else
				{
					vTargetPos -= pData->m_vOffsetPos.x * vDirVec;
					vTargetPos.y += pData->m_vOffsetPos.y;
					vTargetPos += pData->m_vOffsetPos.z * vZVec;
				}

                if ( pSpearMeshInstance != NULL )
				    pSpearMeshInstance->SetPos( vTargetPos );

				m_vecArcEnemyData.push_back( pData );
			}
		}

		m_TimerFlyingImpact.restart();
#ifdef BALANCE_PATCH_20110303
		m_fArcEnemyEachTime = 0.f;
#endif

	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SI2_OVER_TAKER_ARC_ENEMY_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_Init()
{
	XSkinMeshReadyInBackground( L"GuardianStrike01.X" );
	XSkinMeshReadyInBackground( L"GuardianStrike02.X" );

	TextureReadyInBackground( L"Explosion_Fire01.dds" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"PinLight01.tga" );
	TextureReadyInBackground( L"Raven_GuardianStrike01.tga" );
}

void CX2GURaven::RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_StateStartFuture()
{
	m_FrameDataFuture.syncData.bIsRight = true;

	CommonStateStartFuture();
}


void CX2GURaven::RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Spine1" );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_OverTaker_Sp3_GuardianStrike02", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshGuardianStrike1 = pMeshInst->GetHandle();
		}

		vBonePos -= 50.f * GetZVector();
		m_hSeqGuardianStrike1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Light_GuardianStrike02", vBonePos );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike1 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}

		vBonePos -= 50.f * GetZVector();
		m_hSeqGuardianStrike2 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Light_GuardianStrike03", vBonePos );
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike2 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}

		UpDownCrashCameraNoReset( 4.f, 0.53f );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.67f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.67f ) == true && EventCheck( 0.67f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );
		if( true == GetIsRight() )
		{
			vBonePos += 20.f * GetDirVector();
		}
		else
		{
			vBonePos -= 20.f * GetDirVector();
		}
		vBonePos += 50.f * GetZVector();
		vBonePos.y -= 30.f;

#ifndef ADD_MEMO_1ST_CLASS //김창한
		//스크립트로 이동.
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_GUARDIAN_STRIKE", GetPowerRate(), vBonePos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_GuardianStrike01", vBonePos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}

		m_hSeqGuardianStrike3 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Light_GuardianStrike04", vBonePos );
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike3 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );

		}

		UpDownCrashCameraNoReset( 20.f, 2.27f );
	}


	
	if( m_hMeshGuardianStrike1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshGuardianStrike1 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Spine1" ) );
		}
		else
		{
			m_hMeshGuardianStrike1 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	if( m_hSeqGuardianStrike1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike1 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Spine1" );
			vBonePos -= 50.f * GetZVector();
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqGuardianStrike1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}


	if( m_hSeqGuardianStrike2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike2 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Spine1" );
			vBonePos -= 100.f * GetZVector();
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqGuardianStrike2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	if( m_hSeqGuardianStrike3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike3 );
		if( NULL != pSeq )
		{

			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );
			if( true == GetIsRight() )
			{
				vBonePos += 20.f * GetDirVector();
			}
			else
			{
				vBonePos -= 20.f * GetDirVector();
			}
			vBonePos += 50.f * GetZVector();
			vBonePos.y -= 30.f;

			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqGuardianStrike3 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}


void CX2GURaven::RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_StateStartFuture()
{
	m_FrameDataFuture.syncData.bIsRight = true;

	CommonStateStartFuture();
}

void CX2GURaven::RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Spine1" );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_OverTaker_Sp3_GuardianStrike02", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshGuardianStrike1 = pMeshInst->GetHandle();
		}


		vBonePos -= 50.f * GetZVector();
		m_hSeqGuardianStrike1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Light_GuardianStrike02", vBonePos );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike1 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}

		vBonePos -= 50.f * GetZVector();
		m_hSeqGuardianStrike2 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Light_GuardianStrike03", vBonePos );
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike2 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}

		UpDownCrashCameraNoReset( 4.f, 0.53f );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.67f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.67f ) == true && EventCheck( 0.67f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );
		if( true == GetIsRight() )
		{
			vBonePos += 20.f * GetDirVector();
		}
		else
		{
			vBonePos -= 20.f * GetDirVector();
		}
		vBonePos += 50.f * GetZVector();
		vBonePos.y -= 30.f;

#ifndef ADD_MEMO_1ST_CLASS //김창한
		//스크립트로 이동
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_OVER_TAKER_GUARDIAN_STRIKE_HYPER", GetPowerRate(), vBonePos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_GuardianStrike01", vBonePos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}

		m_hSeqGuardianStrike3 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Light_GuardianStrike04", vBonePos );
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike3 );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );

		}

		UpDownCrashCameraNoReset( 25.f, 2.27f );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.87f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.87f ) == true && EventCheck( 0.87f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		vPos.y += 130.f;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_GuardianStrike05", vPos );
		//pSeq->SetAddRotate( GetRotateDegree() );
		pSeq->SetAxisAngle( GetRotateDegree() );
		pSeq->SetVelocity( D3DXVECTOR3( 670, 0, 0 ) );


		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_GuardianStrike05", vPos );
		//pSeq->SetAddRotate( GetRotateDegree() );
		pSeq->SetAxisAngle( GetRotateDegree() );
		pSeq->SetVelocity( D3DXVECTOR3( 0, 670, 0 ) );

		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_GuardianStrike05", vPos );
		//pSeq->SetAddRotate( GetRotateDegree() );
		pSeq->SetAxisAngle( GetRotateDegree() );
		pSeq->SetVelocity( D3DXVECTOR3( -670, 0, 0 ) );


		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_GuardianStrike05", vPos );
		//pSeq->SetAddRotate( GetRotateDegree() );
		pSeq->SetAxisAngle( GetRotateDegree() );
		pSeq->SetVelocity( D3DXVECTOR3( 0, -670, 0 ) );
	}




	if( m_hMeshGuardianStrike1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshGuardianStrike1 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_Spine1" ) );
		}
		else
		{
			m_hMeshGuardianStrike1 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}










	if( m_hSeqGuardianStrike1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike1 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Spine1" );
			vBonePos -= 50.f * GetZVector();
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqGuardianStrike1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}


	if( m_hSeqGuardianStrike2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike2 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_Spine1" );
			vBonePos -= 100.f * GetZVector();
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqGuardianStrike2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	if( m_hSeqGuardianStrike3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqGuardianStrike3 );
		if( NULL != pSeq )
		{

			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );
			if( true == GetIsRight() )
			{
				vBonePos += 20.f * GetDirVector();
			}
			else
			{
				vBonePos -= 20.f * GetDirVector();
			}
			vBonePos += 50.f * GetZVector();
			vBonePos.y -= 30.f;

			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqGuardianStrike3 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}



	CommonFrameMove();

}
void CX2GURaven::RSI_SI3_OVER_TAKER_GUARDIAN_STRIKE_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();

}

void CX2GURaven::ClearArcEnemyData()
{
	for( UINT i=0; i<m_vecArcEnemyData.size(); i++ )
	{
		ArcEnemyData* pData = m_vecArcEnemyData[i];
		SAFE_DELETE( pData );
	}

	m_vecArcEnemyData.resize(0);
}

#ifdef UPGRADE_RAVEN

void CX2GURaven::Init( bool bUseTeamPos_, int iStartPosIndex_ )
{
	CX2GUUser::Init( bUseTeamPos_, iStartPosIndex_ );

	m_bLockOnNpc = false;
	m_iLockOnUid = -1;

	m_bParrying = false;
	m_bCanParrying = false;
	m_fParryingTime = 0.f;
	m_bAttackNasodBall = false;
	m_bDelayAttackNasodBall = false;
	m_fDelayAttackTime = 0.f;
	m_iNaosdBallLv = 0;
	m_fRageGauge = 0.f;
	m_fNasodBallAttackTime = 0.f;
	
	if( IsMyUnit() )
	{
		m_FrameDataFuture.syncData.m_CannonBallCount = GetNasodBall();
	}
}


void CX2GURaven::DoHyper(bool bFullHyper)
{
}

void CX2GURaven::ClearNasodBall()
{
#ifndef FIX_AFTER_IMAGE
	if( m_pAfterImageNasodBall != NULL )
		SAFE_DELETE_KTDGOBJECT( m_pAfterImageNasodBall );
#endif

	if( m_pNasodBall != NULL )
		SAFE_DELETE( m_pNasodBall );

	m_bLockOnNpc = false;
	m_iLockOnUid = -1;

	m_pNasodBall = NULL;
	m_iNaosdBallLv = 0;
	m_fNasodBallAttackTime = 0.f;
	m_bAttackNasodBall = false;	
	m_bDelayAttackNasodBall = false;
	m_fDelayAttackTime = 0.f;
#ifndef FIX_AFTER_IMAGE
	m_pAfterImageNasodBall = NULL;
#endif
}

void CX2GURaven::CreateNasodBall()
{
	if( m_iNaosdBallLv <= 0 )
	{
		ClearNasodBall();
		return;
	}

	if( m_pNasodBall == NULL )
	{		
		m_pNasodBall = new ArcEnemyData;
		if( m_pNasodBall != NULL )
			m_pNasodBall->m_vOffsetPos = D3DXVECTOR3( -180*0.9f, 42*3.f, 0.f );		
	}
    if ( m_pNasodBall == NULL )
        return;

	bool bNewNasodBall = false;
		
	if( 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        ( m_pNasodBall->m_hEffect == INVALID_DAMAGE_EFFECT_HANDLE
          || g_pX2Game->GetDamageEffect()->IsLiveInstanceHandle( m_pNasodBall->m_hEffect ) == false ) 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		(m_pNasodBall->m_pEffect == NULL || g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pNasodBall->m_pEffect ) == false) 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        )
	{
		D3DXVECTOR3 vRotDegree = GetRotateDegree();
		D3DXVECTOR3 vDirVec = GetDirVector();
		D3DXVECTOR3 vZVec = GetZVector();
		vDirVec.y = 0.f;

		D3DXVECTOR3 vPos = GetPos();
		if( false == GetIsRight() )
		{
			vRotDegree.y += 180.f;
		}

		CX2DamageEffect::CEffect *pNasodBall = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"RAVEN_NASOD_BALL", GetPowerRate(), vPos, 
			vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y );

		if( pNasodBall == NULL )
		{
			ClearNasodBall();
			return;
		}

		bNewNasodBall = true;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_pNasodBall->m_hEffect = pNasodBall->GetHandle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pNasodBall->m_pEffect = pNasodBall;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        CKTDGXMeshPlayer::CXMeshInstance* pNasodBallMeshInstance = pNasodBall->GetMainEffect();
        if ( pNasodBallMeshInstance != NULL )
        {
		    pNasodBallMeshInstance->SetMoveAxisAngleDegree( GetRotateDegree() );
		    pNasodBallMeshInstance->SetRotateDegree( GetRotateDegree() + m_pNasodBall->m_vOffsetRotate );
        }

		D3DXVECTOR3 vTargetPos = GetPos(); 
		if( true == GetIsRight() )
		{
			vTargetPos += m_pNasodBall->m_vOffsetPos.x * vDirVec;
			vTargetPos.y += m_pNasodBall->m_vOffsetPos.y;
			vTargetPos += m_pNasodBall->m_vOffsetPos.z * vZVec;
		}
		else
		{
			vTargetPos -= m_pNasodBall->m_vOffsetPos.x * vDirVec;
			vTargetPos.y += m_pNasodBall->m_vOffsetPos.y;
			vTargetPos += m_pNasodBall->m_vOffsetPos.z * vZVec;
		}

        if ( pNasodBallMeshInstance != NULL )
		    pNasodBallMeshInstance->SetPos( vTargetPos );
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = m_pNasodBall->m_pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CKTDGXMeshPlayer::CXMeshInstance*  pNasodBallMeshInstance = ( pNasodBallEffect != NULL ) ? pNasodBallEffect->GetMainEffect() : NULL;
	if( NULL != pNasodBallMeshInstance )
	{
		D3DXVECTOR3 vEffectPos = pNasodBallMeshInstance->GetPos();
		g_pX2Game->GetEffectSet()->PlayEffectSetWithCustomPos_LUA( "EffectSet_BEED_UP", (CX2GameUnit*)this, vEffectPos, GetRotateDegree() );

		pNasodBallMeshInstance->SetOutlineColor( D3DXCOLOR(1.f, 0.333f, 0.0745f, 1.f) );

		switch( m_iNaosdBallLv )
		{
		case 1:
			{
				pNasodBallMeshInstance->SetScale(1.f, 1.f, 1.f);
			}
			break;
		case 2:
			{
				pNasodBallMeshInstance->SetScale(1.3f, 1.3f, 1.3f);
			}
			break;
		case 3:
			{
				pNasodBallMeshInstance->SetScale(1.6f, 1.6f, 1.6f);
			}
			break;
		case 4:
			{
				pNasodBallMeshInstance->SetScale(1.9f, 1.9f, 1.9f);
			}
			break;
		case 5:
			{
				pNasodBallMeshInstance->SetScale(2.2f, 2.2f, 2.2f);
			}
			break;
		default:
			break;
		}

#ifndef FIX_AFTER_IMAGE
	    if( m_pAfterImageNasodBall == NULL && bNewNasodBall == true )
	    {
		    m_pAfterImageNasodBall = CKTDGXSkinAfterImage::CreateSkinAfterImage( pNasodBallMeshInstance->GetXSkinAnim(), 10, XL_EFFECT_0 );
		    if( m_pAfterImageNasodBall != NULL )
		    {
			    m_pAfterImageNasodBall->SetAfterImageColor( D3DXCOLOR(1.f, 0.608f, 0.482f, 1.f) );
			    m_pAfterImageNasodBall->AddMesh( pNasodBallMeshInstance->GetXSkinMesh() );
			    m_pAfterImageNasodBall->OnFrameMove(m_fTime, m_fElapsedTime);
			    m_pAfterImageNasodBall->Enable();
		    }	
	    }
#endif

	}

}
bool CX2GURaven::CanParryingState()
{
	if( m_FrameDataNow.unitCondition.bFootOnLine == false )
		return false;

	switch( GetNowStateID() )
	{
	case CX2GUUser::USI_JUMP_UP:		
	case CX2GUUser::USI_JUMP_DOWN:
	case CX2GUUser::USI_DASH_JUMP:
		return false;
	default:
		break;
	}
	if(	GetNowStateID() >= CX2GUUser::USI_DAMAGE_GROGGY && GetNowStateID() <= CX2GUUser::USI_DAMAGE_GRAPPLED_FRONT )
		return false;

	switch( GetNowStateID() )
	{
	case CX2GURaven::RSI_COMBO_ZZZX:
	case CX2GURaven::RBM_COMBO_ZZZXZ:
	case CX2GURaven::RBM_COMBO_ZZZX_EXPLOSION:
		return false;
	default:
		break;
	}

	return true;
}

void CX2GURaven::DoParrying()
{
	if( m_pNasodBall == NULL )
        return;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pNasodBall->m_pEffect ) ? m_pNasodBall->m_pEffect : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		D3DXVECTOR3 vDirVec = GetDirVector();
		if( GetIsRight() == false )
			vDirVec *= -1;

        CKTDGXMeshPlayer::CXMeshInstance* pNasodBallMeshInstance = pNasodBallEffect->GetMainEffect();
        if ( pNasodBallMeshInstance != NULL )
        {
		    pNasodBallMeshInstance->SetScale(1.3f, 1.3f, 1.3f);
		    pNasodBallMeshInstance->ChangeAnim(L"Guard", CKTDGXSkinAnim::XAP_ONE_WAIT, 1.f);

		    pNasodBallMeshInstance->SetOutlineColor( D3DXCOLOR(0.968f, 0.443f, 0.f, 1.f) );
        }
		pNasodBallEffect->SetAttackTime( D3DXVECTOR2( 0, 0 ) );

#ifdef VERIFY_STAT_BY_BUFF
		const PROTECT_VECTOR3 &vScale = GetScaleByUnit();
#else	// VERIFY_STAT_BY_BUFF
		const D3DXVECTOR3 &vScale = GetScaleByUnit();
#endif // VERIFY_STAT_BY_BUFF

		D3DXVECTOR3 vOffset = vScale.x * vDirVec * 100.f;
		float fOffsetY = vScale.y * 170.f;

        if ( pNasodBallMeshInstance != NULL )
        {
		    pNasodBallMeshInstance->SetPos( GetPos() +  vOffset + D3DXVECTOR3( 0.f, fOffsetY, 0.f ) );				
		    pNasodBallMeshInstance->SetDirSpeed( 0.f );						
        }
		pNasodBallEffect->SetLandPos( 0.f );	

		PlaySound(L"Raven_NasodCore_Trans_Parry.ogg");
		
		m_fRageGauge -= 20.f;
		if( m_fRageGauge < 0.f )
			m_fRageGauge = 0.f;		

		--m_iNaosdBallLv;
		if( m_iNaosdBallLv < 0 )
			m_iNaosdBallLv = 0;
	}
}

void CX2GURaven::RestoreNasodBall()
{
	CreateNasodBall();

    if ( m_pNasodBall == NULL )
        return;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect );
    if ( pNasodBallEffect == NULL )
        return;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = m_pNasodBall->m_pEffect;
	if( pNasodBallEffect == NULL )
		return;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	m_pNasodBall->m_vOffsetPos = D3DXVECTOR3( -180*0.9f, 42*3.f, 0.f);

	CX2DamageEffect::LockOnData* pLockOnData = &pNasodBallEffect->GetLockOnData();				
	pLockOnData->m_LockOnUnitUID = GetUnitUID();
	pLockOnData->m_fCurveSpeed = 10.f;
	pLockOnData->m_fUnLockTime = 9999.f;
    CKTDGXMeshPlayer::CXMeshInstance* pNasodBallEffectMeshInstance = pNasodBallEffect->GetMainEffect();
	pNasodBallEffect->SetAttackTime( D3DXVECTOR2( 0, 0 ) );
    if ( pNasodBallEffectMeshInstance != NULL )
    {
	    pNasodBallEffectMeshInstance->ChangeAnim(L"Wait", CKTDGXSkinAnim::XAP_LOOP, 1.f);
	    pNasodBallEffectMeshInstance->SetDirSpeed( 0.f );	
	    pNasodBallEffectMeshInstance->SetOutlineColor( D3DXCOLOR(1.f, 0.333f, 0.0745f, 1.f) );
	    switch( m_iNaosdBallLv )
	    {
	    case 1:
		    {
			    pNasodBallEffectMeshInstance->SetScale(1.f, 1.f, 1.f);			
			
		    }
		    break;
	    case 2:
		    {
			    pNasodBallEffectMeshInstance->SetScale(1.3f, 1.3f, 1.3f);
		    }
		    break;
	    case 3:
		    {
			    pNasodBallEffectMeshInstance->SetScale(1.6f, 1.6f, 1.6f);
		    }
		    break;
	    case 4:
		    {
			    pNasodBallEffectMeshInstance->SetScale(1.9f, 1.9f, 1.9f);
		    }
		    break;
	    case 5:
		    {
			    pNasodBallEffectMeshInstance->SetScale(2.2f, 2.2f, 2.2f);
		    }
		    break;
	    default:
		    break;
	    }
    }



#ifndef FIX_AFTER_IMAGE
	if( m_pAfterImageNasodBall != NULL )
		m_pAfterImageNasodBall->SetAfterImageColor( D3DXCOLOR(1.f, 0.608f, 0.482f, 1.f) );
#endif
}

void CX2GURaven::AttackResultByType( CX2DamageManager::DamageData &pDamageData )
{
	CX2GUUser::AttackResultByType( pDamageData );

	if( GetRemainHyperModeTime() <= 0.f )
		return;

	if( pDamageData.attackType != CX2DamageManager::AT_SPECIAL )
	{
		// 충전
		if( IsMyUnit() == true && IsAttackNasodBall() == false )
			m_fRageGauge += ( max( pDamageData.damage.fPhysic, pDamageData.damage.fMagic ) * pDamageData.m_fRateModifier * 4.f );

		if( m_fRageGauge > 100.f )
			m_fRageGauge = 100.f;
	}	
	
// 	if( pDamageData.pDefenderUnit == NULL || pDamageData.pDefenderUnit->GetNowHp() <= 0.f )
// 		return;

	if( null != pDamageData.optrDefenderGameUnit && pDamageData.optrDefenderGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		CX2GUNPC *pNpc = (CX2GUNPC *)pDamageData.optrDefenderGameUnit.GetObservable();
		if( pNpc != NULL && pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
			return;
	}
	


	if( false == ( m_bAttackNasodBall == false && m_bDelayAttackNasodBall == false &&
		pDamageData.m_eDamageTrigger == CX2DamageManager::DTT_RAVEN_RAGE_ATTACK &&
        m_bParrying == false &&	
		m_pNasodBall != NULL ) )
        return;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pNasodBall->m_pEffect ) ? m_pNasodBall->m_pEffect : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE		
	{
		// 발사		
		m_bDelayAttackNasodBall = true;
		m_fDelayAttackTime = 0.f;
		m_fNasodBallAttackTime = 0.f;
		
		if( CX2DamageManager::AT_UNIT == pDamageData.defenderType )
		{
			if( null != pDamageData.optrDefenderGameUnit )
			{				
				if( CX2GameUnit::GUT_USER == pDamageData.optrDefenderGameUnit->GetGameUnitType() )
				{
					m_bLockOnNpc = false;
					m_iLockOnUid = pDamageData.optrDefenderGameUnit->GetUnitUID();
				}
				else if( CX2GameUnit::GUT_NPC == pDamageData.optrDefenderGameUnit->GetGameUnitType() )
				{
					m_bLockOnNpc = true;
					m_iLockOnUid = pDamageData.optrDefenderGameUnit->GetUnitUID();
				}
			}
		}		
        CKTDGXMeshPlayer::CXMeshInstance* pNasodBallEffectMeshInstance = pNasodBallEffect->GetMainEffect();
        if ( pNasodBallEffectMeshInstance != NULL )
		    pNasodBallEffectMeshInstance->SetDirSpeed( 0.f );
		switch( m_iNaosdBallLv )
		{
		case 1:			
            if ( pNasodBallEffectMeshInstance == NULL )
                break;
			pNasodBallEffectMeshInstance->ChangeAnim(L"Attack1", CKTDGXSkinAnim::XAP_ONE_WAIT, 1.f);
			pNasodBallEffectMeshInstance->SetScale(1.f, 1.f, 1.f);			
			break;
		case 2:	
            if ( pNasodBallEffectMeshInstance == NULL )
                break;
			pNasodBallEffectMeshInstance->ChangeAnim(L"Attack2", CKTDGXSkinAnim::XAP_ONE_WAIT, 1.f);	
			pNasodBallEffectMeshInstance->SetScale(1.6f, 1.6f, 1.6f);
			break;
		case 3:
            if ( pNasodBallEffectMeshInstance == NULL )
                break;
			pNasodBallEffectMeshInstance->ChangeAnim(L"Attack3", CKTDGXSkinAnim::XAP_ONE_WAIT, 1.f);
			pNasodBallEffectMeshInstance->SetScale(1.4f, 1.4f, 1.4f);
			break;
		case 4:
            if ( pNasodBallEffectMeshInstance == NULL )
                break;
			pNasodBallEffectMeshInstance->ChangeAnim(L"Attack4", CKTDGXSkinAnim::XAP_ONE_WAIT, 1.f);
			pNasodBallEffectMeshInstance->SetScale(1.6f, 1.6f, 1.6f);
			break;
		case 5:
            if ( pNasodBallEffectMeshInstance == NULL )
                break;
			pNasodBallEffectMeshInstance->ChangeAnim(L"Attack5", CKTDGXSkinAnim::XAP_ONE_WAIT, 1.f);
			pNasodBallEffectMeshInstance->SetScale(1.5f, 1.5f, 1.5f);
			break;
		default:
			RestoreNasodBall();
			return;
		}	

#ifndef FIX_AFTER_IMAGE
		if( m_pAfterImageNasodBall != NULL )		
			m_pAfterImageNasodBall->SetAfterImageColor( 0xffff0000 );
#endif

		PlaySound( L"Raven_NasodCore_Trans_Attack.ogg" );
	}
}

void CX2GURaven::ChangePowerRateNasodBall()
{
	float fPhysic = 1.f;
	float fMagic = 1.f;
	float fBallPowerRate = 1.f;
	switch( m_iNaosdBallLv )
	{
	case 1:
		fBallPowerRate = 2.2f;				
		break;
	case 2:
		fBallPowerRate = 4.4f;
		break;
	case 3:
		fBallPowerRate = 6.6f;
		break;
	case 4:
		fBallPowerRate = 8.8f;
		break;
	case 5:
		fBallPowerRate = 11.f;
		break;
	default:
		RestoreNasodBall();
		m_bAttackNasodBall = false;
		m_bDelayAttackNasodBall = false;			
		return;
	}	

    if ( m_pNasodBall == NULL )
        return;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = m_pNasodBall->m_pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if (pNasodBallEffect == NULL )
        return;
	pNasodBallEffect->GetDamageData().damage.fPhysic = fPhysic * fBallPowerRate;
	pNasodBallEffect->GetDamageData().damage.fMagic = fMagic * fBallPowerRate;
}

void CX2GURaven::AttackNasodBall( )
{
    if ( m_pNasodBall == NULL )
        return;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pNasodBall->m_pEffect ) ? m_pNasodBall->m_pEffect : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CKTDGXMeshPlayer::CXMeshInstance* pNasodBallEffectMeshInstance = ( pNasodBallEffect != NULL ) ? pNasodBallEffect->GetMainEffect() : NULL )
	{
		CX2DamageEffect::LockOnData* pLockOnData = &pNasodBallEffect->GetLockOnData();
//#ifdef FIX_AFTER_IMAGE
//		//if ( NULL != pLockOnData )
//		{
//			pLockOnData->m_fCurveSpeed = 30.f;
//			pLockOnData->m_fUnLockTime = 0.06f;
//		}
//#else
		pLockOnData->m_fCurveSpeed = 30.f;
		pLockOnData->m_fUnLockTime = 0.06f;
//#endif

		if( (m_iLockOnUid >= 0 || m_iLockOnUid < -1) )
		{			
			if( m_bLockOnNpc == true )
			{
				CX2GUNPC *pNpc = g_pX2Game->GetNPCUnitByUID( (int)m_iLockOnUid );
				if( pNpc == NULL || pNpc->GetNowHp() <= 0.f )
				{
					m_bDelayAttackNasodBall = false;
					m_fDelayAttackTime = 0.f;
					m_bAttackNasodBall = false;
					m_iLockOnUid = -1;
					RestoreNasodBall();
					return;
				}

				pLockOnData->m_LockOnNPCUID = (int)m_iLockOnUid;
			}
			else
			{
				CX2GUUser *pUser = g_pX2Game->GetUserUnitByUID( m_iLockOnUid );
				if( pUser == NULL || pUser->GetNowHp() <= 0.f )
				{
					m_bDelayAttackNasodBall = false;
					m_fDelayAttackTime = 0.f;
					m_bAttackNasodBall = false;
					m_iLockOnUid = -1;
					RestoreNasodBall();
					return;
				}

				pLockOnData->m_LockOnUnitUID = m_iLockOnUid;
			}			
			
			D3DXVECTOR3 vDirVec = GetDirVector();
			if( true == GetIsRight() )
				vDirVec *= -1;

			pNasodBallEffectMeshInstance->SetPos( pNasodBallEffectMeshInstance->GetPos() + vDirVec * 150.f + D3DXVECTOR3( 0, 0, 0 ) );
			pNasodBallEffect->SetAttackTime( D3DXVECTOR2( 0, 9999 ) );

			pNasodBallEffect->SetLandPos( 0.f );
			pNasodBallEffectMeshInstance->SetDirSpeed( 3600.f );				
						
			ChangePowerRateNasodBall();

			m_bAttackNasodBall = true;
			m_bDelayAttackNasodBall = false;

			PlaySound( L"Raven_NasodCore_Attack.ogg" );
				
		}
	}
}

void CX2GURaven::DoFrameMoveNasodBall()
{	
	if( GetRemainHyperModeTime() > 0.f )
	{
		if( m_fParryingTime > 0.f )
		{
			m_fParryingTime -= m_fElapsedTime;
			if( m_fParryingTime < 0.f )
			{
				m_fParryingTime = 0.f;
				m_bCanParrying = false;

				//D3DXVECTOR3 vPos = pNasodBallEffect->GetMainEffect()->GetPos();
				//g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_PVP_BOT_CATASTROPHE_LASER", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );			
			}
		}

#ifndef FIX_AFTER_IMAGE
		if( m_pNasodBall != NULL )
		{
			CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshPlayer = INVALID_MESH_INSTANCE_HANDLE;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pNasodBall->m_pEffect ) ? m_pNasodBall->m_pEffect : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_pAfterImageNasodBall != NULL && pNasodBallEffect != NULL )
			{
				hMeshPlayer = pNasodBallEffect->GetMainEffectHandle();
				CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( hMeshPlayer );
				
				if( pMeshInst != NULL && pMeshInst->GetState() == CKTDGXMeshPlayer::IS_PLAY &&
					pMeshInst->GetXSkinAnim() == m_pAfterImageNasodBall->GetXSkinAnim() && 
					g_pX2Game->GetMajorXMeshPlayer()->IsLiveInstanceHandle( hMeshPlayer ) == true )
				{
					m_pAfterImageNasodBall->OnFrameMove( m_fTime, m_fElapsedTime );
				}
				else
				{
					if( m_pAfterImageNasodBall != NULL )
						SAFE_DELETE_KTDGOBJECT( m_pAfterImageNasodBall );
				}
			}
			else
			{
				if( m_pAfterImageNasodBall != NULL )
					SAFE_DELETE_KTDGOBJECT( m_pAfterImageNasodBall );
			}
		}
#endif

		if( m_bDelayAttackNasodBall == true && m_bParrying == false )
		{
			m_fDelayAttackTime += m_fElapsedTime;
			if( m_fDelayAttackTime >= 0.3f )
			{
				AttackNasodBall();
			}
		}

		if( GetParrying() == false )
		{
			int iNasodBall = 0;
			if( IsMyUnit() == true )
				iNasodBall = (int)(m_fRageGauge / 20.f);
			else
				iNasodBall = m_FrameDataNow.syncData.m_CannonBallCount;

			// 나소드볼 유지관리
			if( m_iNaosdBallLv != iNasodBall )
			{
				// 재생성
				if( m_iNaosdBallLv == 0 )
					PlaySound( L"Raven_NasodCore_Appear.ogg" );
				else if( m_iNaosdBallLv > iNasodBall )
					PlaySound( L"Raven_NasodCore_Shrink.ogg" );
				else
					PlaySound( L"Raven_NasodCore_Grow.ogg" );

				m_iNaosdBallLv = iNasodBall;
				if( m_iNaosdBallLv > 5 )
					m_iNaosdBallLv = 5;
				CreateNasodBall();

				if( IsAttackNasodBall() == true )
					ChangePowerRateNasodBall();
			}
		}		

		D3DXVECTOR3 vDirVec = GetDirVector();

		if( m_bAttackNasodBall == false && m_bDelayAttackNasodBall == false && GetParrying() == false &&
			m_pNasodBall != NULL && 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_pNasodBall->m_hEffect != INVALID_DAMAGE_EFFECT_HANDLE
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_pNasodBall->m_pEffect != NULL 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pNasodBall->m_pEffect ) ? m_pNasodBall->m_pEffect : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{
				D3DXVECTOR3 vZVec = GetZVector();
				D3DXVec3Normalize( &vDirVec, &vDirVec );

				CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pNasodBallEffect->GetMainEffect();
				if( NULL != pMeshInst )
				{
					D3DXVECTOR3 vTargetPos = GetPos();
					if( true == GetIsRight() )
					{
						vTargetPos += m_pNasodBall->m_vOffsetPos.x * vDirVec;
						vTargetPos.y += m_pNasodBall->m_vOffsetPos.y;
						vTargetPos += m_pNasodBall->m_vOffsetPos.z * vZVec;
					}
					else
					{
						vTargetPos -= m_pNasodBall->m_vOffsetPos.x * vDirVec;
						vTargetPos.y += m_pNasodBall->m_vOffsetPos.y;
						vTargetPos += m_pNasodBall->m_vOffsetPos.z * vZVec;
					}

					float coeff = 0.05f;
					D3DXVECTOR3 vRotateDegree = GetRotateDegree() + m_pNasodBall->m_vOffsetRotate;
					pMeshInst->SetRotateDegree( vRotateDegree );
					pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );

					vTargetPos = vTargetPos * coeff + pMeshInst->GetPos() * ( 1.f-coeff );
					pMeshInst->SetPos( vTargetPos );

				}
			}
			else
			{				
				if( IsMyUnit() == true )
				{
					m_fRageGauge = m_fRageGauge - (m_iNaosdBallLv * 20.f);
				}
				ClearNasodBall();				
			}			
		}

		if( m_bParrying == true && m_pNasodBall != NULL  )
		{			
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* pNasodBallEffect = m_pNasodBall->m_pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CKTDGXMeshPlayer::CXMeshInstance* pNasodBallEffectMeshInstance = ( pNasodBallEffect != NULL ) ? pNasodBallEffect->GetMainEffect() : NULL )
            {
			    if( GetIsRight() == false )
				    vDirVec *= -1;			

    #ifdef VERIFY_STAT_BY_BUFF
			    const PROTECT_VECTOR3 &vScale = GetScaleByUnit();
    #else	// VERIFY_STAT_BY_BUFF
			    const D3DXVECTOR3 &vScale = GetScaleByUnit();
    #endif // VERIFY_STAT_BY_BUFF

			    D3DXVECTOR3 vOffset = vScale.x * vDirVec * 70.f;
			    float fOffsetY = vScale.y * 100.f;

			    pNasodBallEffectMeshInstance->SetPos( GetPos() +  vOffset + D3DXVECTOR3( 0.f, fOffsetY, 0.f ) );		
            }
		}

		m_fNasodBallAttackTime += m_fElapsedTime;
#ifdef FIX_AFTER_IMAGE
		if( m_pNasodBall != NULL && 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_pNasodBall->m_hEffect != INVALID_DAMAGE_EFFECT_HANDLE &&
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_pNasodBall->m_pEffect != NULL && 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_bAttackNasodBall == true && m_bDelayAttackNasodBall == false && m_fNasodBallAttackTime >= 1.2f )
#else
		if( m_bAttackNasodBall == true && m_bDelayAttackNasodBall == false && m_fNasodBallAttackTime >= 1.2f )
#endif
		{
			m_bLockOnNpc = false;
			m_iLockOnUid = -1;

			m_bAttackNasodBall = false;
			m_fDelayAttackTime = 0.f;
			m_fNasodBallAttackTime = 0.f;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_pNasodBall->m_hEffect ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CX2DamageEffect::CEffect* pNasodBallEffect = g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pNasodBall->m_pEffect ) ? m_pNasodBall->m_pEffect : NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{
#ifndef FIX_AFTER_IMAGE
				if( m_pNasodBall != NULL && pNasodBallEffect != NULL )
#endif
				{
					if( pNasodBallEffect->GetDamageTimeNow() > 0 )
					{
						// 타격한 경우이므로 정상적으로 제거
						if( IsMyUnit() == true )
						{
							m_fRageGauge = m_fRageGauge - (m_iNaosdBallLv * 20.f);
						}
						
						ClearNasodBall();						
					}
					else
					{
						// 타격하지 못한경우 wait상태로 복귀
						RestoreNasodBall();
					}
				}				
			}
            else
			{
				// 타격한 경우이므로 정상적으로 제거
				if( IsMyUnit() == true )
				{
					m_fRageGauge = m_fRageGauge - (m_iNaosdBallLv * 20.f);
				}
				ClearNasodBall();
			}				
		}
	}	
}

void CX2GURaven::AddRageGuage(float fVal)
{
	m_fRageGauge = m_fRageGauge + fVal;
	if( m_fRageGauge > 100.f )
		m_fRageGauge = 100.f;
	else if( m_fRageGauge < 0.f )
		m_fRageGauge = 0.f;
}

#endif //UPGRAD_RAVEN

//{{ kimhc // 2010.11.3 // 레피 - 승화된 분노
#ifdef	NEW_SKILL_2010_11

/*virtual*/ float CX2GURaven::GetHyperModeTime() const
{
	/// 강제각성 시간이 주어졌으면
	if ( GetForceHyperModeTime() > 0.0f )
		return CX2GUUser::GetHyperModeTime();	/// 상위클래스의 GetHyperModeTime() 사용
	else if ( GetRemainHyperModeTime() > 0.0f )	/// 재각성이면
	{
		float fTotalTime = 0.0f;

		const CX2UserSkillTree& cUserSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const CX2SkillTree::SkillTemplet* pSkillTempletSpritualizedFury 
			= cUserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_RRF_SPIRITUALIZED_FURY );

		if ( NULL != pSkillTempletSpritualizedFury )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillTempletLevel = max( 1, cUserSkillTree.GetSkillLevel( pSkillTempletSpritualizedFury->m_eID, true ) );	/// 스킬 레벨
	
			fTotalTime = pSkillTempletSpritualizedFury->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			fTotalTime = pSkillTempletSpritualizedFury->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

		return fTotalTime;

		//return fTotalTime;
	}
	else
		return CX2GUUser::GetHyperModeTime();	/// 일반적인 각성
}

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.3 // 레피 - 승화된 분노

void CX2GURaven::RSI_SI_A_RF_SHADOW_STEP_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Shadow_Step_Mesh01.X" );
}


void CX2GURaven::RSI_SI_A_RF_SHADOW_STEP_StartFuture()
{
	CommonStateStartFuture();

	m_fShadowStepTimeLeft = 0.f;



	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RF_SHADOW_STEP );
	if( NULL != pSkillTemplet )
	{
		ASSERT( m_PhysicParam.passiveSpeed.x > 0.f );
		if( 0.f != m_PhysicParam.passiveSpeed.x )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			m_fShadowStepTimeLeft = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS, iSkillTempletLevel ) * 
									100.f / m_PhysicParam.passiveSpeed.x;
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fShadowStepTimeLeft = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS ) * 100.f / m_PhysicParam.passiveSpeed.x;
	#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_NOTE			
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO6 ) == true )
			{
				m_PhysicParam.passiveSpeed.x *= 1.3f;				
			}
#endif
		}

	}
}

void CX2GURaven::RSI_SI_A_RF_SHADOW_STEP_Start()
{
	CommonStateStart();

	SetAlphaObject( true );

	if ( false == IsMyUnit() && false == IsNullGageUI() )
		m_pGageUI->SetAlpha( 0.0f );
	
	m_RenderParam.bAlphaBlend		= true;
	m_RenderParam.color.a			= 0.1f;

	EnableAfterImage();

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	if( true == m_bEnableShadowBackSlide )
	{
		//스킬 쿨타임 자체 연산
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RF_SHADOW_STEP );

		if( NULL != pSkillTemplet && true == m_bEnableShadowBackSlide )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			m_fShadowStepCooTime = pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel );		/// 처음 동작시 쿨타임을 저장해 놓고, 자체 연산한다.
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fShadowStepCooTime = pSkillTemplet->m_fSkillCoolTime;		/// 처음 동작시 쿨타임을 저장해 놓고, 자체 연산한다.
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

		m_iRVCComboLoopCount	 = 1;		/// 섀도우 스텝 동작 횟수
		m_bActiveShadowStepCombo = true;	/// 섀도우 스텝 사용중 여부
		m_bIsShadowStepIcon		 = false;	/// 섀도우 백 슬라이드 아이콘으로 설정

		if ( IsMyUnit() )
			g_pX2Game->UpdateSkillSlotUI();
	}
#endif SERV_RAVEN_VETERAN_COMMANDER
}

void CX2GURaven::RSI_SI_A_RF_SHADOW_STEP_FrameMove()
{
	if( m_FrameDataNow.syncData.bFrameStop == false )
	{
		if( m_fShadowStepTimeLeft > 0.f )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_fShadowStepTimeLeft -= GetElapsedTime();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_fShadowStepTimeLeft -= g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( m_fShadowStepTimeLeft < 0.f )
				m_fShadowStepTimeLeft = 0.f;
		}
	}

	CommonFrameMove();
}



void CX2GURaven::RSI_SI_A_RF_SHADOW_STEP_EventProcess()
{
	if( m_fShadowStepTimeLeft <= 0.f )
	{
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			InitShadowStep();

			m_bActiveShadowStepCombo = false;
#endif SERV_RAVEN_VETERAN_COMMANDER
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChangeDashIfPossible();
		}
	}

#ifdef SERV_RAVEN_VETERAN_COMMANDER

	else if( true == m_bEnableShadowBackSlide && true == SpecialAttackEventProcess() )
	{
		m_bActiveShadowStepCombo = false;	/// 섀도우 스텝 사용중 여부

		StateChange( RVC_SI_P_SHADOW_BACK_SLIDE );
	}

	else if( true == m_InputData.pureZ )
	{
		m_bActiveShadowStepCombo = false;	/// 섀도우 스텝 사용중 여부

		SetShadowStepAttackSkill();
	}

	else if( true == m_InputData.pureX )
	{
		m_bActiveShadowStepCombo = false;	/// 섀도우 스텝 사용중 여부

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

		// 각각에 맞는 쉐도우 스탭 스킬이 발동!
		if ( true == m_bEnableShadowStepPunisher )
		{
			SetShadowStepAttackSkill( RSI_P_RRF_SHADOW_PUNISHER );
		}
		else if ( true == m_bEnableShadowStepThrustEnhance )
		{
			SetShadowStepAttackSkill( RSI_SHADOW_STEP_THRUST_ENHANCE );
		}
		else if ( true == m_bEnableShadowBackSlide )			   
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		{
			SetShadowStepAttackSkill( RVC_SI_P_SHADOW_BUSTER );
		}
	
	}
#else  SERV_RAVEN_VETERAN_COMMANDER

	else if( m_InputData.pureZ == true || m_InputData.pureX == true )
	{
		if( m_FrameDataFuture.unitCondition.fStateTime > 0.2f )
		{
			if( true == m_bEnableShadowStepThrust )
			{
				if( FlushMp( m_fMPConsumeShadowStepThrust ) == true )
				{
					StateChange( RSI_SHADOW_STEP_THRUST );
				}
				else
				{
#ifdef ALWAYS_SCREEN_SHOT_TEST
					if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
					{
						g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
					}
#else
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif ALWAYS_SCREEN_SHOT_TEST
				}
			}
		}
	}

#endif SERV_RAVEN_VETERAN_COMMANDER

	CommonEventProcess();
}




void CX2GURaven::RSI_SI_A_RF_SHADOW_STEP_End()
{
	CommonStateEnd();

	SetAlphaObject( false );
	if ( false == IsMyUnit() && false == IsNullGageUI() )
		m_pGageUI->SetAlpha( 255.0f );
	
	m_RenderParam.bAlphaBlend		= false;
	m_RenderParam.color.a			= 1.f;

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	if( true == m_bActiveShadowStepCombo ) /// State 변경이 일어나지 않았다면, 아이콘 초기화 한다.
		InitShadowStep();

#endif SERV_RAVEN_VETERAN_COMMANDER
	m_fShadowStepTimeLeft = 0.f;
	DisableAfterImage();
}


void CX2GURaven::RSI_SHADOW_STEP_THRUST_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Thrust_Mesh01.X" );
}



void CX2GURaven::RSI_SHADOW_STEP_THRUST_Start()
{
	CommonStateStart();

	m_NowStateData.m_fPowerRate = m_fAttackPowerRateShadowStepThrust;

	m_DamageData.damage.fPhysic					*= m_fAttackPowerRateShadowStepThrust;
	m_DamageData.damage.fMagic					*= m_fAttackPowerRateShadowStepThrust;		
	m_DamageData.m_ExtraDamage.m_DamagePerSec	*= m_fAttackPowerRateShadowStepThrust;

#ifdef SERV_SKILL_NOTE

#ifdef BALANCE_BLADE_MASTER_20130117		/// 메모가 적용되어 있으면, 무조건 않넘어 뜨리도록 수정
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO7 ) == true )
#else  BALANCE_BLADE_MASTER_20130117
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO7 ) == true && GetRandomFloat() <= 0.6f)
#endif BALANCE_BLADE_MASTER_20130117
	{
		m_DamageData.reActType = CX2DamageManager::RT_BIG_DAMAGE;
	}
#endif
}

void CX2GURaven::RSI_SHADOW_STEP_THRUST_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}





void CX2GURaven::RSI_STANDUP_ATTACK_FRONT_NO_DOWN_FrameMove()
{

	CommonFrameMove();
}

void CX2GURaven::RSI_STANDUP_ATTACK_FRONT_NO_DOWN_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_STANDUP_ATTACK_FRONT_NO_DOWN_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


void CX2GURaven::RSI_STANDUP_ATTACK_BACK_NO_DOWN_FrameMove()
{

	CommonFrameMove();
}

void CX2GURaven::RSI_STANDUP_ATTACK_BACK_NO_DOWN_EventProcess()
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
			StateChangeDashIfPossible();
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_STANDUP_ATTACK_BACK_NO_DOWN_End()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

#ifdef SKILL_FOR_CANCEL
bool CX2GURaven::CheckPressedSkillForCancel()
{
	if( true == IsThereEffectiveBlackHoleDamageEffect() )
	{
		return false;
	}

	if( true == m_bSpecialAttackEventProcessedAtThisFrame )
		return false;

	int iPressedSlot= -1;
	
	if( true == m_InputData.oneA )
		iPressedSlot = 0;
	else if( true == m_InputData.oneS )
		iPressedSlot = 1;
	else if( true == m_InputData.oneD )
		iPressedSlot = 2;
	else if( true == m_InputData.oneC )
		iPressedSlot = 3;
	else if( true == m_InputData.oneQ )
		iPressedSlot = 4;
	else if( true == m_InputData.oneW )
		iPressedSlot = 5;
	else if( true == m_InputData.oneE )
		iPressedSlot = 6;
	else if( true == m_InputData.oneR )
		iPressedSlot = 7;
	if ( iPressedSlot == -1 )
		return false;

	bool bSlotB = (iPressedSlot > 3) ? true : false;
	int iSlotIndex = (iPressedSlot > 3) ? iPressedSlot-4 : iPressedSlot;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( iSlotIndex, bSlotB );	

	if( pSkillSlotData == NULL )
		return false;	

	switch ( pSkillSlotData->m_eID )
	{
	case CX2SkillTree::SI_A_RST_CUT_TENDON:
		return true;
	default:
		break;
	}

	return false;
}
#endif	SKILL_FOR_CANCEL

#ifdef SKILL_30_TEST

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh02.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh03.X" );
	TextureReadyInBackground( L"Whitecircle02.dds" );
	TextureReadyInBackground( L"GroundShockWave02.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
}
void CX2GURaven::RSI_SA_RST_SHOCKWAVE_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

}


void CX2GURaven::RSI_SA_RST_SHOCKWAVE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}


void CX2GURaven::RSI_SA_RST_SHOCKWAVE_HYPER_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh02.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh03.X" );
	TextureReadyInBackground( L"Whitecircle02.dds" );
	TextureReadyInBackground( L"GroundShockWave02.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
}
void CX2GURaven::RSI_SA_RST_SHOCKWAVE_HYPER_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_HYPER_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}


void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ROT_VKJ_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ROT_VKJ_Spear_Mesh01.X" );
}
	
void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

}


void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		float fUpPower = -1.f;
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO21 ) == true )
		{
			D3DXVECTOR3 vecMyPos = GetPos();
			D3DXVECTOR3 vecLandPos = GetLandPosition();
			
			fUpPower = floor( (vecMyPos.y - vecLandPos.y) * 0.01f );
			fUpPower = (fUpPower * 0.02f);
			fUpPower = min( fUpPower, 0.1f );

			fUpPower = fUpPower + GetPowerRate();
		}
		CX2EffectSet::Handle hGroundImpact = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RAVEN_VJAVELIN", this, NULL, false, fUpPower );
		m_vecEffectSetToDeleteOnDamageReact.push_back(hGroundImpact);
#else //ADD_MEMO_1ST_CLASS
		CX2EffectSet::Handle hGroundImpact = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RAVEN_VJAVELIN", this );
		m_vecEffectSetToDeleteOnDamageReact.push_back(hGroundImpact);
#endif //ADD_MEMO_1ST_CLASS
		
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.48f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.48f ) == true && EventCheck( 0.48f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;			
		m_PhysicParam.fDownAccel = m_PhysicParam.fGAccel * 2.f;
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( RSI_SA_ROT_VALKYRIESJAVELIN_LANDING );
	}
	CommonEventProcess();
}


void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}
	
void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ROT_VKJ_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ROT_VKJ_Spear_Mesh01.X" );
}

void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

}

void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		float fUpPower = -1.f;
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO21 ) == true )
		{
			D3DXVECTOR3 vecMyPos = GetPos();
			D3DXVECTOR3 vecLandPos = GetLandPosition();

			fUpPower = floor( (vecMyPos.y - vecLandPos.y) * 0.01f );
			fUpPower = (fUpPower * 0.02f);
			fUpPower = min( fUpPower, 0.1f );

			fUpPower = fUpPower + GetPowerRate();
		}
		CX2EffectSet::Handle hGroundImpact = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RAVEN_VJAVELIN", this, NULL, true, fUpPower );
		m_vecEffectSetToDeleteOnDamageReact.push_back(hGroundImpact);
#else //ADD_MEMO_1ST_CLASS
		CX2EffectSet::Handle hGroundImpact = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RAVEN_VJAVELIN", this );
		m_vecEffectSetToDeleteOnDamageReact.push_back(hGroundImpact);
#endif //ADD_MEMO_1ST_CLASS

	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.48f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.48f ) == true && EventCheck( 0.48f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;			
		m_PhysicParam.fDownAccel = m_PhysicParam.fGAccel * 2.f;
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_LANDING );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_HYPER_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}

void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_FrameMove()
{

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_HYPER_FrameMove()
{

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_ROT_VALKYRIESJAVELIN_LANDING_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}









void CX2GURaven::RSI_A_RST_CUT_TENDON_Init()
{
	TextureReadyInBackground( L"CutTendon_Slash01.dds" );
}

void CX2GURaven::RSI_A_RST_CUT_TENDON_StateStart()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RST_CUT_TENDON );
	if ( NULL != pSkillTemplet && pSkillTemplet->m_vecBuffFactorPtr.size() > 1 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( NULL == GetUnit()  )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
		
		if ( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO11 ) )
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[1]->GetClonePtr( iSkillTempletLevel ) );
		else
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
		if ( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO11 ) )
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[1] );
		else
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
}
	
void CX2GURaven::RSI_A_RST_CUT_TENDON_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_A_ROT_WEAPON_BREAK_Init()
{
	TextureReadyInBackground( L"Break_Impact01.dds" );
}

void CX2GURaven::RSI_A_ROT_WEAPON_BREAK_StateStart()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ROT_WEAPON_BREAK );

	if ( NULL != pSkillTemplet && pSkillTemplet->m_vecBuffFactorPtr.size() > 1 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( NULL == GetUnit()  )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

		if ( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO12 ) )
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[1]->GetClonePtr( iSkillTempletLevel ) );
		else
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
		if ( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO12 ) )
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[1] );
		else
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );	
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
}

void CX2GURaven::RSI_A_ROT_WEAPON_BREAK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}
#endif SKILL_30_TEST

#ifdef SKILL_CASH_10_TEST
void CX2GURaven::RSI_A_RST_BLOODY_ACCEL_Init()
{
	XSkinMeshReadyInBackground( L"Raven_Bloody_Accel_Blade_Mesh01.X" );
	XMeshReadyInBackground( L"Wind_Liner02.Y" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"Heal01.tga" );
	TextureReadyInBackground( L"Blood_Bouble01.dds" );
}

void CX2GURaven::RSI_A_RST_BLOODY_ACCEL_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.f ) == true && EventCheck( 1.f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RST_BLOODY_ACCEL );
		if( NULL != pSkillTemplet )
		{
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO19 ) == true )
				SetBuffFactorToGameUnit( pSkillTemplet, 1 );
			else
#endif //ADD_MEMO_1ST_CLASS
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}

		D3DXVECTOR3 vBonePos = GetWeaponBonePos( 0, L"TRACE_END0", 0 );
		m_hSeqBloodyAccel = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"SpinLiner_Raven_Bloody_Accel01", vBonePos );
	}

	if( m_hSeqBloodyAccel != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqBloodyAccel );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetWeaponBonePos( 0, L"TRACE_END0", 0 );
			vBonePos.y += 30.f;

			D3DXVECTOR3 vBoneRot = D3DXVECTOR3(0.f, 0.f, 0.f);			
			GetFrameRotateDegree(&vBoneRot, m_pXSkinAnim->GetCloneFrame(L"Dummy1_Rhand"));				
			vBoneRot.x += 90.f;

			pSeq->SetPosition( vBonePos );
			pSeq->SetAxisAngle( vBoneRot );
			pSeq->SetAddRotate( vBoneRot );				
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_A_RST_BLOODY_ACCEL_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}
		
void CX2GURaven::RSI_A_ROT_NUCLEAR_Init()
{
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Nuclear.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Bottom_Light.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Explosion_Light.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Center_Body.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Center_Head.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Center_Body_A.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Center_Head_A.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Mushroom_Body.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Mushroom_Head.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Mushroom_Head.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Dust01.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ROT_NUCLEAR_Effect_Dust02.X" );
}

void CX2GURaven::RSI_A_ROT_NUCLEAR_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.5f ) == true && EventCheck( 2.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 40.f, 1.6f );
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_A_ROT_NUCLEAR_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 5.33f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_A_ROT_NUCLEAR_Hyper_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.5f ) == true && EventCheck( 2.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 40.f, 1.6f );
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_A_ROT_NUCLEAR_Hyper_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}

#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE

void CX2GURaven::RRF_DASH_JUMP_COMBO_X_Init()
{
	XSkinMeshReadyInBackground( L"Hand_FireImpact01.X" );
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_X_Start()
{
	CommonStateStart();
	m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
	m_DamageData.bCanRevenge	= false;
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_X_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.21f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.7f )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.26f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.29f )
		{			
			m_PhysicParam.nowSpeed.y = 1200.0f;
		}
		else
		{
			if( m_PhysicParam.nowSpeed.y < 0.0f )
				m_PhysicParam.nowSpeed.y = 0.0f;
		}			
	}
		
	CommonFrameMoveFuture();
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.26f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.26f ) && EventCheck( 0.26f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 10.0f ) == true )
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			if( GetIsRight() == true )
				pos += 10.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 10.0f * m_FrameDataNow.unitCondition.dirVector;
			pos.y -= 10.0f;

#ifdef BALANCE_PATCH_20120329
			if( m_hEffectDashJumpComboX != INVALID_EFFECTSET_HANDLE )
				g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectDashJumpComboX );
				
			m_hEffectDashJumpComboX = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RRF_DashJumpX", (CX2GameUnit*)this, (CX2GameUnit*)this );
			m_vecEffectSetToDeleteOnDamageReact.push_back( m_hEffectDashJumpComboX );
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RRF_DASH_JUMP_X", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree() );
#endif

			UpDownCrashCamera( 10.0f, 0.2f );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 vAngle = GetRotateDegree();
			vAngle.z -= 50.f;


			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				PlaySequenceByTriggerCount( pSeq, pos, 100, 100, 1 );
				pSeq->SetAxisAngle( vAngle );
				pSeq->SetAddRotate( vAngle );
			}
		}

	}

	CommonFrameMove();
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_X_EventProcess()
{
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.39f
		&& IsOnSomethingFuture() == false )
	{
		StateChange( RRF_DASH_JUMP_COMBO_XX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.39f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
		else
		{
			StateChange( USI_JUMP_DOWN );
		}
	}

		

	CommonEventProcess();
}


void CX2GURaven::RRF_DASH_JUMP_COMBO_XX_Init()
{
	//XSkinMeshReadyInBackground( L"Hand_FireImpact01.X" );
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_XX_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.54f )
	{
		if( m_PhysicParam.nowSpeed.y < 0.0f )
			m_PhysicParam.nowSpeed.y = 0.0f;
	}
	CommonFrameMoveFuture();
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_XX_FrameMove()
{
		
	CommonFrameMove();
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_XX_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( RRF_DASH_JUMP_COMBO_XX_LANDING );
	}

	CommonEventProcess();
}

void CX2GURaven::RRF_DASH_JUMP_COMBO_XX_LANDING_Start()
{
	CommonStateStart();

	CreateStepDust();
}
void CX2GURaven::RRF_DASH_JUMP_COMBO_XX_LANDING_FrameMove()
{
	CommonFrameMove();
}
void CX2GURaven::RRF_DASH_JUMP_COMBO_XX_LANDING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}		
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	IF_SKILL_CANCEL_AFTER( 0.4f )
	ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( 0.4f )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
#else // SKILL_CANCEL_BY_HYPER_MODE
	ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( 0.4f )
	{
		if( SpecialAttackEventProcess() == true )
		{
		}
		else if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE

	CommonEventProcess();
}

void CX2GURaven::RRF_COMBO_ZZXFRONTX_Start()
{
	CommonStateStart();
	// To do...
}
void CX2GURaven::RRF_COMBO_ZZXFRONTX_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.27f ) 
	{			
		m_PhysicParam.nowSpeed.x = 0.f;
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.33f )
	{
		m_PhysicParam.nowSpeed.x = 0.f;
		m_PhysicParam.passiveSpeed.x = 0.f;
	}

	CommonFrameMoveFuture();
}
void CX2GURaven::RRF_COMBO_ZZXFRONTX_FrameMove()
{
	// To do...

	CommonFrameMove();
}
void CX2GURaven::RRF_COMBO_ZZXFRONTX_EventProcess()
{
	// To do...		
	if( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || 
		( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}

	if(  m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f && m_FrameDataFuture.stateParam.bEventFlagList[1] == true)
	{
		StateChange( USI_WAIT );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	else if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}				
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.57f, 0.57f, m_InputData.oneX == true && GetUnit()->GetClass() == CX2Unit::UC_RAVEN_RECKLESS_FIST, RRF_COMBO_ZZXFRONTXX )

	CommonEventProcess();
}
void CX2GURaven::RRF_COMBO_ZZXFRONTX_End()
{
	CommonStateEnd();
	
	m_PhysicParam.nowSpeed.x = 0.f;
	m_PhysicParam.passiveSpeed.x = 0.f;
}

void CX2GURaven::RRF_COMBO_ZZXFRONTXX_FrameMove()
{
	// To do...

	CommonFrameMove();
}
void CX2GURaven::RRF_COMBO_ZZXFRONTXX_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	else if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}		

	CommonEventProcess();
}

void CX2GURaven::RBM_Combo_XZ_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	else if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}		
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.63f, 0.63f, m_InputData.oneZ == true && GetUnit()->GetClass() == CX2Unit::UC_RAVEN_BLADE_MASTER, RBM_COMBO_XZZ )

	CommonEventProcess();
}
void CX2GURaven::RBM_Combo_XZZ_EventProcess()
{
	// To do...
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 )	

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}		
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( SpecialAttackEventProcess() == true )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}			
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.66f, 0.66f, m_InputData.oneZ == true && GetUnit()->GetClass() == CX2Unit::UC_RAVEN_BLADE_MASTER, RBM_COMBO_XZZZ )

	CommonEventProcess();
}
void CX2GURaven::RBM_Combo_XZZZ_FrameMoveFuture()
{
	/*if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.43f )
	{			
		m_PhysicParam.nowSpeed.y = 0.0f;
	}*/
	CommonFrameMoveFuture();
}
void CX2GURaven::RBM_Combo_XZZZ_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			//m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_JUMP_LANDING );
		}
	}		

	CommonEventProcess();
}

void CX2GURaven::RBM_COMBO_ZZZX_Init()
{
	TextureReadyInBackground( L"Gate_Light02_Gray.dds" );
	TextureReadyInBackground( L"SpreadLight02_Gray.dds" );
	TextureReadyInBackground( L"GuideArrow01.dds" );
	TextureReadyInBackground( L"Condense_Pulse02.dds" );
}

#ifdef BALANCE_BLADE_MASTER_20130117
void CX2GURaven::RBM_COMBO_ZZZX_StateStart()
{
	m_iBlackHoleHitCount = 0;
	m_fBlackHoleTime	 = 0.f;
	m_eRavenCurrentStage = RBM_COMBO_ZZZXZ;		/// 블레이드 마스터 ZZZXZZZ 콤보 캔슬시 클라간 동기화를 맞추기 위해, 캔슬시 스테이트를 저장

	CommonStateStart();
}
#endif BALANCE_BLADE_MASTER_20130117

void CX2GURaven::RBM_COMBO_ZZZX_FrameMove()
{
	// To do...
		
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.24f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( m_hBlackHoleEffectSet != INVALID_EFFECTSET_HANDLE )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hBlackHoleEffectSet );

		m_iBlackHoleHitCount = 1;
		m_fBlackHoleTime = (m_pXSkinAnim->GetMaxAnimationTime() - 0.24f) + 1.f;
		m_hBlackHoleEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RBM_BLACKHOLE", this );	

		UpDownCrashCamera( 5.0f, 0.2f );
	}		
		
	CommonFrameMove();
}
void CX2GURaven::RBM_COMBO_ZZZX_EventProcess()
{
	// To do...
	if( m_iBlackHoleHitCount > 0 && m_fBlackHoleTime <= 0.9f )
	{
		m_iBlackHoleHitCount = 0;
		m_fBlackHoleTime = 1.0f;
		StateChange(RBM_COMBO_ZZZX_EXPLOSION);
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.94f, 0.94f, m_InputData.oneZ == true && 
		GetUnit()->GetClass() == CX2Unit::UC_RAVEN_BLADE_MASTER, RBM_COMBO_ZZZXZ )
	else if( false == IsOnSomethingFuture() )
	{
		m_fBlackHoleTime = 0.f;
		m_iBlackHoleHitCount = 0;
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}		

#ifdef BALANCE_BLADE_MASTER_20130117
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif BALANCE_BLADE_MASTER_20130117

	CommonEventProcess();
}
void CX2GURaven::RBM_COMBO_ZZZXZ_Start()
{
	CommonStateStart();
	// To do...		
	if(m_iBlackHoleHitCount-1 >= 4)
	{			
		m_iBlackHoleHitCount = 0;
		m_fBlackHoleTime = 1.0f;
		StateChange(RBM_COMBO_ZZZX_EXPLOSION);

		m_FrameDataFuture.syncData.bIsRight			= !m_FrameDataFuture.syncData.bIsRight;
		m_FrameDataFuture.unitCondition.dirDegree.y	+= 180.0f;
	}
	else
	{
		++m_iBlackHoleHitCount;		
		m_fBlackHoleTime = 0.69f + 0.55f;
	}		

#ifdef BALANCE_BLADE_MASTER_20130117
	m_eRavenCurrentStage = RBM_COMBO_ZZZXZ;		/// 블레이드 마스터 ZZZXZZZ 콤보 캔슬시 클라간 동기화를 맞추기 위해, 캔슬시 스테이트를 저장
#endif BALANCE_BLADE_MASTER_20130117

}
void CX2GURaven::RBM_COMBO_ZZZXZ_EventProcess()
{
	// To do...
	if( false == IsOnSomethingFuture() )
	{
		m_fBlackHoleTime = 0.f;
		m_iBlackHoleHitCount = 0;
		StateChange( USI_JUMP_DOWN );
		m_PhysicParam.nowSpeed.x = 0.f;
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;


	}		
	else if( m_iBlackHoleHitCount > 0 && m_fBlackHoleTime <= 0.4f && m_FrameDataFuture.stateParam.bEventFlagList[0] == false )
	{
		m_iBlackHoleHitCount = 0;
		m_fBlackHoleTime = 1.0f;
		StateChange(RBM_COMBO_ZZZX_EXPLOSION);
	}
	else if( m_iBlackHoleHitCount - 1 >= 4 && m_fBlackHoleTime <= 0.4f )
	{
		m_iBlackHoleHitCount = 0;
		m_fBlackHoleTime = 1.0f;
		StateChange(RBM_COMBO_ZZZX_EXPLOSION);
	}		
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.69f && m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
	{
		StateChange( RBM_COMBO_ZZZXZ );
	}	

#ifdef BALANCE_BLADE_MASTER_20130117
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif BALANCE_BLADE_MASTER_20130117

	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 0.69f && m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}		

	CommonEventProcess();
}	

#ifdef BALANCE_BLADE_MASTER_20130117
void CX2GURaven::RBM_COMBO_ZZZX_EXPLOSION_StateStart()
{
	m_eRavenCurrentStage = RBM_COMBO_ZZZXZ;		/// 블레이드 마스터 ZZZXZZZ 콤보 캔슬시 클라간 동기화를 맞추기 위해, 캔슬시 스테이트를 저장

	CommonStateStart();
}
#endif BALANCE_BLADE_MASTER_20130117

void CX2GURaven::RBM_COMBO_ZZZX_EXPLOSION_FrameMove()
{
	// To do...

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 10.0f, 0.2f );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//if( m_hBlackHoleEffectSet != INVALID_EFFECTSET_HANDLE )
		//	g_pX2Game->GetEffectSet()->StopEffectSet( m_hBlackHoleEffectSet );

		m_iBlackHoleHitCount = 0;
		m_fBlackHoleTime = 0.f;			
	}

	CommonFrameMove();
}
void CX2GURaven::RBM_COMBO_ZZZX_EXPLOSION_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
#ifdef BALANCE_BLADE_MASTER_20130117
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#else  BALANCE_BLADE_MASTER_20130117
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f ) 
	{
		if( SpecialAttackEventProcess() == true )
		{
		}
	}
#endif BALANCE_BLADE_MASTER_20130117

	CommonEventProcess();
}



void CX2GURaven::SI_A_RRF_GROUND_IMPACT_Init()
{
	XSkinMeshReadyInBackground( L"Raven_RRF_Ground_Impact_AttackBox01.X" );
	XMeshReadyInBackground( L"Thief_Fat_JumpAttackA_Stone02.Y" );
	XMeshReadyInBackground( L"Up_Impact01.Y" );
	TextureReadyInBackground( L"stone.dds" );			
}

void CX2GURaven::SI_A_RRF_GROUND_IMPACT_FrameMove()
{
	// To do...

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.466f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.466f ) == true && EventCheck( 0.466f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		float fScale = 1.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet(CX2SkillTree::SI_A_RF_GROUND_IMPACT);
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet(CX2SkillTree::SI_A_RRF_GROUND_IMPACT);
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			fScale = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			fScale = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

		CX2EffectSet::Handle hGroundImpact = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RAVEN_GROUND_IMPACT", this, NULL, false, -1.f, 01.f, D3DXVECTOR3(fScale, fScale, fScale) );
// 			if( hGroundImpact != INVALID_EFFECTSET_HANDLE )
// 			{				
// 				CX2EffectSet::EffectSetInstance *pInst = g_pX2Game->GetEffectSet()->GetEffectSetInstance(hGroundImpact);				
// 				pInst->SetEffectScale( D3DXVECTOR3(fScale, fScale, fScale) );
// 			}		

		//g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( 20.0f, 0.2f );
	}		

	CommonFrameMove();
		
}
void CX2GURaven::SI_A_RRF_GROUND_IMPACT_EventProcess()
{
	// To do...
	if( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true) || 
		( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false) )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if(  m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.f && m_FrameDataFuture.stateParam.bEventFlagList[0] == true)
	{
		StateChange( USI_WAIT );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		

	CommonEventProcess();
}

void CX2GURaven::SI_SA_RRF_X_CRASH_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XSkinMeshReadyInBackground( L"Raven_RRF_SI_SA_X_Crash_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Raven_RRF_SI_SA_X_Crash_Mesh02.X" );
	XSkinMeshReadyInBackground( L"Raven_RRF_SI_SA_X_Crash_Mesh03.X" );
	TextureReadyInBackground( L"smoke02.dds" );	
	TextureReadyInBackground( L"DarkSmoke01.dds" );	
}
void CX2GURaven::SI_SA_RRF_X_CRASH_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 1 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.166f ) == true && EventCheck( 1.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_vXCrashPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
	}		
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.31f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.31f ) == true && EventCheck( 1.31f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		if( IsHyperState() == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RRF_X_CRASH_NODAMAGE_HYPER", GetPowerRate(), m_vXCrashPos, GetRotateDegree(), GetRotateDegree() );				
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RRF_X_CRASH_NODAMAGE", GetPowerRate(), m_vXCrashPos, GetRotateDegree(), GetRotateDegree() );				
		}
			
	}
		
	CommonFrameMove();
}
void CX2GURaven::SI_SA_RRF_X_CRASH_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		

	CommonEventProcess();
}

void CX2GURaven::SI_A_RBM_ONE_FLASH_Init()
{
}
void CX2GURaven::SI_A_RBM_ONE_FLASH_StateStart()
{
	CommonStateStart();
	// To do...		
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet(CX2SkillTree::SI_A_RBM_ONE_FLASH);
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit()  )
		return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		int iDamageTimes = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		int iDamageTimes = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		if( iDamageTimes > 0 )
		{
			ChangeDamageTimes( iDamageTimes );
		}
	}
		
}
void CX2GURaven::SI_A_RBM_ONE_FLASH_EventProcess()
{
	// To do...

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		
	ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( 1.f )
	{
		if( SpecialAttackEventProcess() == true )
		{
		}
		else if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::SI_SA_RBM_WOLF_FANG_START_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XSkinMeshReadyInBackground( L"Raven_RSM_SI_SA_Wolf_Fang_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Raven_RSM_SI_SA_Wolf_Fang_Mesh02.X" );
	XSkinMeshReadyInBackground( L"Raven_RSM_SI_SA_Wolf_Fang_Mesh03.X" );
	TextureReadyInBackground( L"ColorBallGray.dds" );
	TextureReadyInBackground( L"GroundShockWave02.dds" );
}
void CX2GURaven::SI_SA_RBM_WOLF_FANG_START_StateStart()
{
	CommonStateStart();
}
void CX2GURaven::SI_SA_RBM_WOLF_FANG_START_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.001f, 1 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈	

	CommonFrameMove();
}
void CX2GURaven::SI_SA_RBM_WOLF_FANG_START_EventProcess()
{
	// To do...
	if( GetAttackSuccess(false) == true )
	{
		if( IsHyperState() == true )
			StateChange(SI_SA_RBM_WOLF_FANG_HIT_HYPER);
		else
			StateChange(SI_SA_RBM_WOLF_FANG_HIT);
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( IsOnSomethingFuture() == true )
		{
			if( IsHyperState() == true )
				StateChange( SI_SA_RBM_WOLF_FANG_STARTLANDING_HYPER );
			else
				StateChange( SI_SA_RBM_WOLF_FANG_STARTLANDING );
		}
	}		

	CommonEventProcess();
}	
void CX2GURaven::SI_SA_RBM_WOLF_FANG_STARTLANDING_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			//StateChange( RSI_WAIT );
			StateChange( USI_DASH );
		}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( CX2SkillTree::SI_A_RBM_WOFL_FANG );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( CX2SkillTree::SI_SA_RBM_WOFL_FANG );
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillSlotData )
		{
			float fMPConsume = pSkillSlotData->m_fMPConsumption / 2.f;
			UpNowMp(fMPConsume);

			if( NULL != g_pData->GetPicCharBlue() )
			{
				WCHAR wszText[64] = L"";
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", (int)fMPConsume );

				D3DXVECTOR3 pos = GetPos();
				pos.y += 50.f;

				g_pData->GetPicCharBlue()->DrawText( wszText, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
			}
		}	
	}
	else if( GetAttackSuccess(false) == true )
	{
		if( IsHyperState() == true )
			StateChange(SI_SA_RBM_WOLF_FANG_HIT_HYPER);
		else
			StateChange(SI_SA_RBM_WOLF_FANG_HIT);
	}

	CommonEventProcess();
}
void CX2GURaven::SI_SA_RBM_WOLF_FANG_HIT_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( IsOnSomethingFuture() == true )
		{
			StateChange( SI_SA_RBM_WOLF_FANG_HITLANDING );
		}
	}		

	CommonEventProcess();
}
void CX2GURaven::SI_SA_RBM_WOLF_FANG_HITLANDING_EventProcess()
{
	// To do...
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		

	CommonEventProcess();
}
#endif

//{{ JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가
#ifdef NEW_SKILL_2010_11
void CX2GURaven::RBM_EMERGENCY_ESCAPE()
{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경

	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RBM_EMERGENCY_ESCAPE, true );

	/// 긴급 탈출 스킬 확인
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RBM_EMERGENCY_ESCAPE );

		if ( NULL != pSkillTemplet )
		{
			/// 발동 확율
			if ( GetRandomInt() < pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel ) * 100.f )
			{
				//{{ 마나 확인 (SpecialAttackEventProcess 함수 참조)
				float fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, iSkillLevel );

				if ( GetNowMp() < fMPConsume )
				{
#ifdef ALWAYS_SCREEN_SHOT_TEST
					if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
					{
						return;
					}
#endif ALWAYS_SCREEN_SHOT_TEST
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
					return;
				}

				StateChange( RSI_SHADOW_STEP );

				UpNowMp( -fMPConsume );
				//}}
			}
		}
	}

	#else // UPGRADE_SKILL_SYSTEM_2013

	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RBM_EMERGENCY_ESCAPE );

	/// 긴급 탈출 스킬 확인
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RBM_EMERGENCY_ESCAPE, iSkillLevel );

		/// 발동 확율
		if ( GetRandomInt() < pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE ) * 100.f )
		{
			//{{ 마나 확인 (SpecialAttackEventProcess 함수 참조)
			float fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, pSkillTemplet->m_iSkillLevel );

			if ( GetNowMp() < fMPConsume )
			{
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return;
			}

			StateChange( RSI_SHADOW_STEP );

			UpNowMp( -fMPConsume );
			//}}
		}
	}

	#endif // UPGRADE_SKILL_SYSTEM_2013
}

void CX2GURaven::RBM_EMERGENCY_ESCAPE_StartFuture()
{
	CommonStateStartFuture();

	m_fShadowStepTimeLeft = 0.f;
		
	CX2SkillTree::SkillTemplet* pSkillTemplet = NULL;

	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_RF_SHADOW_STEP );

	// 섀도우 스텝 스킬 포인트가 0일 때
	if ( 0 == iSkillLevel )
		iSkillLevel = 1;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경

	#ifdef PVP_SEASON2
		pSkillTemplet =  const_cast<CX2SkillTree::SkillTemplet*>( g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_RF_SHADOW_STEP ) );
	#else
		pSkillTemplet =  const_cast<CX2SkillTree::SkillTemplet*>( g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_RF_SHADOW_STEP, iSkillLevel ) );
	#endif

		if( NULL != pSkillTemplet )
		{
			ASSERT( m_PhysicParam.passiveSpeed.x > 0.f );
			if( 0.f != m_PhysicParam.passiveSpeed.x )
			{
				m_fShadowStepTimeLeft = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS, 
					min( iSkillLevel + GetSkillLevelUpNum( CX2SkillTree::SI_A_RF_SHADOW_STEP ), MAX_LIMITED_SKILL_LEVEL ) ) * 100.f / m_PhysicParam.passiveSpeed.x;

	#ifdef SERV_SKILL_NOTE			
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO6 ) == true )
				{
					m_PhysicParam.passiveSpeed.x *= 1.3f;				
				}
	#endif
			}
	}

#else // UPGRADE_SKILL_SYSTEM_2013

#ifdef PVP_SEASON2
	pSkillTemplet =  const_cast<CX2SkillTree::SkillTemplet*>( g_pData->GetSkillTree()->GetSkillTemplet(
		CX2SkillTree::SI_A_RF_SHADOW_STEP, min( iSkillLevel + GetSkillLevelUpNum( CX2SkillTree::SI_A_RF_SHADOW_STEP ), 10 ) ) );
#else
	pSkillTemplet =  const_cast<CX2SkillTree::SkillTemplet*>( g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_RF_SHADOW_STEP, iSkillLevel ) );
#endif

	if( NULL != pSkillTemplet )
	{
		ASSERT( m_PhysicParam.passiveSpeed.x > 0.f );
		if( 0.f != m_PhysicParam.passiveSpeed.x )
		{
			m_fShadowStepTimeLeft = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS ) * 100.f / m_PhysicParam.passiveSpeed.x;

#ifdef SERV_SKILL_NOTE			
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO6 ) == true )
			{
				m_PhysicParam.passiveSpeed.x *= 1.3f;				
			}
#endif
		}
	}

#endif // UPGRADE_SKILL_SYSTEM_2013
}
#endif NEW_SKILL_2010_11
//}} JHKang / 강정훈 / 2010/10/31 / 긴급탈출 패시브 추가


//{{ kimhc // 2010.11.10 // 블마-기가드라이브, 레피-와일드 차지
#ifdef	NEW_SKILL_2010_11
	/** @function : CX2GURaven::RSI_SA_RBM_GIGA_DRIVE_Init()
	@brief : 블라-기가드라이브에서 쓰일 파일들을 미리 로드 함
	@param : void
	@return : void
*/
void CX2GURaven::RSI_SA_RBM_GIGA_DRIVE_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_SA_Gungnir_Mesh05.X" );
	XSkinMeshReadyInBackground( L"Arme_ChargeMp1.X" );
	XMeshReadyInBackground( L"Up_Impact01.Y" );
	XMeshReadyInBackground( L"Thief_Fat_JumpAttackA_Stone02.Y" );
	TextureReadyInBackground( L"ColorBallFire.dds" );	
}

void CX2GURaven::RSI_SA_RBM_GIGA_DRIVE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 1.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 1.1f ) == true && EventCheck( 1.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//CX2UserSkillTree& cUserSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTempletGigaDrive = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RBM_GIGA_DRIVE_LIMITER );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTempletGigaDrive = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_RBM_GIGA_DRIVE );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetBuffFactorToGameUnit( pSkillTempletGigaDrive, 0 );		
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RBM_GIGA_DRIVE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		

	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGE_READY_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_Double_Slash_Mesh004.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Thrust_Mesh01.X" );
	XSkinMeshReadyInBackground( L"SpinningKick02.X" );

	TextureReadyInBackground( L"CenterLight_Gray01.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
	TextureReadyInBackground( L"Lire_01_SpinningKick05.tga" );
	TextureReadyInBackground( L"WC_01.tga" );
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGE_READY_StateStart()
{
	CommonStateStart();

	m_WildChargeDataPtr->SetTimeAfterFullCharging( 0.0f );
	m_WildChargeDataPtr->SetPowerRateWildCharging( 0.0f );
	// 충전중에 소모된 HP를 알아내기 위해 충전 전의 HP를 저장
	m_WildChargeDataPtr->SetHpEachWildChargeFrame( GetNowHp() );
}


void CX2GURaven::RSI_SA_RRF_WILD_CHARGE_READY_StateStartFuture()
{
	CommonStateStartFuture();

	// m_eSlotID_WildCharge에 담긴 normal 스테이트가 무엇이냐에 따라서, 차지하는 키가 무엇인지 알아냄
	switch ( m_WildChargeDataPtr->GetSlotID_WildCharge() )
	{
	case USI_SPECIAL_ATTACK_1:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureA );
		break;

	case USI_SPECIAL_ATTACK_2:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureS );
		break;

	case USI_SPECIAL_ATTACK_3:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureD );
		break;

	case USI_SPECIAL_ATTACK_4:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureC );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_1:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureQ );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_2:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureW );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_3:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureE );
		break;

	case USI_SPECIAL_ATTACK_SLOTB_4:
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureR );
		break;

	default:
		ASSERT( !L"RSI_SA_RRF_WILD_CHARGE_READY_StateStartFuture" );
		m_WildChargeDataPtr->SetChargeKeyPointer( &m_InputData.pureA );
		break;
	}
}


void CX2GURaven::RSI_SA_RRF_WILD_CHARGE_READY_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_WildChargeDataPtr->SetHandleWildCharingEffectSet( g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RF_WILD_CHARGE_CHARGING", this ) );
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGE_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_SA_RRF_WILD_CHARGING );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGING_StateStart()
{
	CommonStateStart();

	const int DAMAGE_REDUCE_PERCENT_WILD_CHARGING_STATE = 30;

	SetDamageReducePercent( GetDamageReducePercent() + DAMAGE_REDUCE_PERCENT_WILD_CHARGING_STATE );
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGING_FrameMove()
{
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
	const float MAX_CHARGE_POWER_RATE = 10.0f;

	// m_fPowerRateWildCharging가 100%가 모이지 않았으면 if를 실행함 (10.0f 로 판단한 이유는 아래에 있음)
	if ( MAX_CHARGE_POWER_RATE > m_WildChargeDataPtr->GetPowerRateWildCharging() )
	{
		// 현재의 HP
		float fHpNow		= GetNowHp();
		// 소모된 HP양
		float fHpDecresed	= m_WildChargeDataPtr->GetHpEachWildChargeFrame() - fHpNow;
		// 소모된 HP% = (이전 HP - 현재 HP / 최대 HP ) * 100
		float fHpPercentDecreased = 0.0;

		if ( 0 < fHpDecresed ) // 소모된 HP가 있으면
		{
			fHpPercentDecreased = fHpDecresed / GetMaxHp() * 100;
		}
		
		// 기획에서는 충전된 m_fPowerRateWildCharging = ( 소모된HP% + 충전시간(초) ) * 10 이라 할때
		// GetPowerRateWildCharging() 가 100 보다 크면 3초 후에 발동 되기를 원함
		// 위의 얘기는 GetPowerRateWildCharging() 의 계산에서 10을 곱하지 않고 '10보다 크다' 를 판단하는 것과 같다.

		// 지금 프레임에서 차지된 양을 이전까지 차지되었던 양에다가 더함
		// 위에서 100 이아닌 10으로 판단하고 10을 곱하지 않음
		m_WildChargeDataPtr->SetPowerRateWildCharging( m_WildChargeDataPtr->GetPowerRateWildCharging() + fHpPercentDecreased + m_fElapsedTime ); 
		
		// 현재 프레임의 HP를 저장 (다음 프레임에서 이전 프레임의 HP 정보로 활용)
		m_WildChargeDataPtr->SetHpEachWildChargeFrame( fHpNow );
	}
	else		// 풀차지가 되었으면
	{
		const float EPSILON_TIME = 0.005f;

		// 차지가 맥스 된 이펙트 출력
		if ( m_WildChargeDataPtr->GetTimeAfterFullCharging() >= 0.0f &&
			 m_WildChargeDataPtr->GetTimeAfterFullCharging() <= EPSILON_TIME )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_WILD_CHARGE_MAX", this );
			PlaySound( L"Raven_WildCharge_ChargeFull.ogg" );
		}
			 
		// 풀차지가 된 후로부터 몇초가 지났는지를 계산함
		m_WildChargeDataPtr->SetTimeAfterFullCharging( m_WildChargeDataPtr->GetTimeAfterFullCharging() + m_fElapsedTime );
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGING_EventProcess()
{
	const float MAX_TIME_AFTER_FULL_CHARGING = 3.0f;

	// 기를 모으는 키에서 손을 뗏다거나
	// 풀차지 후 MAX_TIME_AFTER_FULL_CHARGING 만큼의 시간이 지났거나
	// 스킬슬롯(A, B)이 바뀌었으면
	if ( false == m_WildChargeDataPtr->GetChargeKey() || 
		 MAX_TIME_AFTER_FULL_CHARGING < m_WildChargeDataPtr->GetTimeAfterFullCharging() ) 
		StateChange( RSI_SA_RRF_WILD_CHARGE_DASH );
	
	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGING_StateEnd()
{
	CommonStateEnd();
	
	const int DAMAGE_REDUCE_PERCENT_WILD_CHARGING_STATE = 30;
	SetDamageReducePercent( GetDamageReducePercent() - DAMAGE_REDUCE_PERCENT_WILD_CHARGING_STATE );

	g_pX2Game->GetEffectSet()->StopEffectSet( m_WildChargeDataPtr->GetHandleReferenceWildCharingEffectSet() );
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGE_DASH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const float MAX_CHARGE_POWER_RATE = 10.0f;
		if ( MAX_CHARGE_POWER_RATE < m_WildChargeDataPtr->GetPowerRateWildCharging() )
			m_WildChargeDataPtr->SetPowerRateWildCharging( MAX_CHARGE_POWER_RATE );

		float fPowerRateWildCharge = GetPowerRate();

		// 100이 아니라 10으로 나눈 (0.1을 곱함) 이유는 RSI_SA_RRF_WILD_CHARGING_FrameMove() 에서 10을 덜 곱했기 때문임
		fPowerRateWildCharge += GetPowerRate() * ( m_WildChargeDataPtr->GetPowerRateWildCharging() * 0.1f );

		bool bIsHyperMode = false;

		if ( 0.0f < GetRemainHyperModeTime() )
			bIsHyperMode = true;

	 	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RF_WILD_CHARGE_CHARGE_ATTACK", this, NULL,
			bIsHyperMode, fPowerRateWildCharge );
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RRF_WILD_CHARGE_DASH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		

	CommonEventProcess();
}

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.10 // 블마-기가드라이브, 레피-와일드 차지

#ifdef UPGRADE_RAVEN
void CX2GURaven::RSI_PARRYING_StartFuture()
{
	CommonStateStartFuture();
	
}
void CX2GURaven::RSI_PARRYING_Start()
{
	CommonStateStart();

	// To do...			
 	int iRand = GetRandomInt(0) % 2;
 	if( iRand == 0 )
 	{
 		m_pXSkinAnim.get()->ChangeAnim( L"GuardSmallFront", false );		
 	}

	m_bParrying = true;
	m_bCanParrying = true;
	m_fParryingTime = 0.5f;
	
	m_bDelayAttackNasodBall = false;
	m_bAttackNasodBall = false;
	m_fDelayAttackTime = 0.f;
	m_fNasodBallAttackTime = 0.f;

	// 패링동작
	DoParrying();
}
void CX2GURaven::RSI_PARRYING_EventProcess()
{	
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.3f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.3f ) 
	{
		if( SpecialAttackEventProcess() == true )
		{
		}
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE

	CommonEventProcess();
}

void CX2GURaven::RSI_PARRYING_StateEnd()
{
	CommonStateEnd();

	m_bParrying = false;
	m_bCanParrying = false;
	m_fParryingTime = 0.f;
	RestoreNasodBall();
}
#endif

//////////////////////////////////////////////////////////////////////////
CX2GURaven::ArcEnemyData::ArcEnemyData()
{
	m_vOffsetRotate = D3DXVECTOR3(0, 0, 0);
	m_vOffsetPos = D3DXVECTOR3( 0, 0, 0 );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hEffect = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pEffect = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

CX2GURaven::ArcEnemyData::~ArcEnemyData()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( INVALID_DAMAGE_EFFECT_HANDLE != m_hEffect )
		g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL != m_pEffect )
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_pEffect );
    m_pEffect = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}





#ifdef RAVEN_WEAPON_TAKER
void CX2GURaven::RSI_RWT_LOSE2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.55f ) == true && EventCheck( 1.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 20.0f, 0.3f );
	}
	CommonFrameMove();
}

// 레이븐 웨폰 테이커 새 커맨드
void CX2GURaven::RWT_COMBO_X_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"High_Explosive_Shell00.X" );
	XSkinMeshReadyInBackground( L"X_Combo2_TRACE.X" );
	XSkinMeshReadyInBackground( L"RWT_Gun_Fire_Impact01.Y" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"High_Explosive_Shell00.X" );
	XSkinMeshReady( L"X_Combo2_TRACE.X" );
	XSkinMeshReady( L"RWT_Gun_Fire_Impact01.Y" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}
void CX2GURaven::RWT_COMBO_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.321f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.321f ) == true && EventCheck( 0.321f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp(2.0f) == true )
		{
			//포쏘기
			D3DXVECTOR3 vDirVector = GetDirVector();
			D3DXVECTOR3 vPos = GetBonePos( L"Bip01_L_Hand" );
			D3DXVECTOR3 vOffset = ( vDirVector * 50.f );
			vPos += GetIsRight() ? -vOffset : vOffset;

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RWT_COMBO_X_SHELL",
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				PlaySequenceByTriggerCount( pSeq, pos, 100, 100, 1 );
				pSeq->SetAxisAngle( GetRotateDegree() );
				pSeq->SetAddRotate( GetRotateDegree() );
			}
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RWT_COMBO_X_EventProcess()
{
	// 이벤트 플래그가 무엇을 나타내는지 알기 쉽게 하도록
	bool &bCanChangeStateComboXX			= m_FrameDataFuture.stateParam.bEventFlagList[0];

	// 발밑에 라인맵이 없으면
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_RAVEN_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	// 라인맵 위에 있고 대시도 하지 않았으면
	else 
	{
		const float LOWER_BOUND_INPUT_TIME_COMBO = 0.1f;	// 이 시간 이후부터 콤보 입력 가능
		const float UPPER_BOUND_INPUT_TIME_COMBO = 0.702f;	// 이 시간 까지 콤보 입력 가능
		// 콤보입력이 가능한 시점
		if( LOWER_BOUND_INPUT_TIME_COMBO <= m_pXSkinAnimFuture->GetNowAnimationTime() &&
			UPPER_BOUND_INPUT_TIME_COMBO >= m_pXSkinAnimFuture->GetNowAnimationTime() )
		{
			// X 키를 눌렀으면 XX 콤보로
			if( true == m_InputData.oneX )
			{
				bCanChangeStateComboXX = true;
			}
		}

		const float TRIGGER_TIME_COMBO_X	= 0.702f;	// Combo_X의 분기점
		if( TRIGGER_TIME_COMBO_X < m_pXSkinAnimFuture->GetNowAnimationTime() )
		{				
			// XX 콤보 실행
			if( true == bCanChangeStateComboXX )
			{
				StateChange( RSI_WEAPON_TAKER_COMBO_XX );
			}
		}

		// 아무 입력 없이 그냥 애니메이션이 끝났으면
		if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XX_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XX_EventProcess()
{
	// 이벤트 플래그가 무엇을 나타내는지 알기 쉽게 하도록
	bool &bCanChangeStateComboXXX			= m_FrameDataFuture.stateParam.bEventFlagList[0];

	const float UPPER_BOUND_INPUT_TIME_COMBO = 0.737f;	// 이 시간 까지 콤보 입력 가능
	// 콤보입력이 가능한 시점
	if( UPPER_BOUND_INPUT_TIME_COMBO >= m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// X 키를 눌렀으면 XXX 콤보로
		if( true == m_InputData.oneX )
		{
			bCanChangeStateComboXXX = true;
		}
	}

	const float TRIGGER_TIME_COMBO_X	= 0.737f;	// Combo_X의 분기점
	if( TRIGGER_TIME_COMBO_X <= m_pXSkinAnimFuture->GetNowAnimationTime() )
	{				
		// XXX 콤보 실행, 땅에 있을 때만
		if( true == bCanChangeStateComboXXX &&
			true == IsOnSomethingFuture() )
		{
			StateChange( RSI_WEAPON_TAKER_COMBO_XXX );
		}
	}

	// 아무 입력 없이 그냥 애니메이션이 끝났으면
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		// 발밑에 라인맵이 없으면
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_RAVEN_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}

	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.466f;		// 이 시간 이후부터 캔슬 입력 가능
	// 캔슬입력이 가능한 시점
	if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// 대시 캔슬
		if( CAN_DASH_CANCEL )
		{			
			StateChangeDashIfPossible();
			bCanChangeStateComboXXX = false;
		}
		// 스킬 캔슬
		ELSE_IF_ASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		{
			if( false == IsOnSomethingFuture() )
				SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR );
			else
				SpecialAttackEventProcess();
			bCanChangeStateComboXXX = false;
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_StateStart()
{
	CommonStateStart();

	_SetOverheat( false );
	m_bShowedOverheat = false;
	m_iRWTComboLoopCount = 0;
	m_iRWTMaxComboLoopCount = 1;
}

void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_FrameMove()
{
	CommonFrameMove();
}

void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_EventProcess()
{
	// 발밑에 라인맵이 없으면
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_RAVEN_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else
	{
		// 별일 없이 종료
		if( true == m_pXSkinAnimFuture->IsAnimationEnd())
		{				
			StateChange( USI_WAIT );
		}

		const float TRIGGER_TIME_COMBO_X	= 0.660f;	// Combo_X의 분기점
		if( TRIGGER_TIME_COMBO_X <= m_pXSkinAnimFuture->GetNowAnimationTime() )
		{				
			// XXX_LOOP 콤보 실행
			StateChange( RSI_WEAPON_TAKER_COMBO_XXX_LOOP );
		}
		else
		{
			// 키 입력시에도 미리 오버히트 검사를 해 둡니다.
			if( true == m_InputData.oneX )
			{
				if( m_iRWTMaxComboLoopCount < _CONST_RAVEN_::MAX_COMBO_XXX_LOOP_COUNT && _CanOverheat() )
					++m_iRWTMaxComboLoopCount;
			}
		}
	}

	CommonEventProcess();
}

#ifdef SERV_RAVEN_VETERAN_COMMANDER
void CX2GURaven::_ShowOverheatIcon( int iSize/*= 0*/ )
#else  SERV_RAVEN_VETERAN_COMMANDER
void CX2GURaven::_ShowOverheatIcon()
#endif SERV_RAVEN_VETERAN_COMMANDER
{
	D3DXVECTOR3 vDirVector = GetDirVector();
	D3DXVECTOR3 vPos( GetHeadBonePos() );
	D3DXVECTOR3 vOffset = ( vDirVector * 140.f );
	vPos += GetIsRight() ? vOffset : -vOffset;
	vPos.y += 60.0f;

#ifdef SERV_RAVEN_VETERAN_COMMANDER

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqOverHeatIcon )				/// 기존에 있던 오버 히트 이모티콘은 해제
	{
		CKTDGParticleSystem::CParticleEventSequence* pEmoticon = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqOverHeatIcon );

		if( NULL !=  pEmoticon )
		{
			pEmoticon->SetEmitRate( 0, 0 );
			pEmoticon->ClearAllParticle();
		}

		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqOverHeatIcon );
	}

	/// 오버 히트 이모티콘 새로 생성
	m_hSeqOverHeatIcon = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*)this, L"WeaponTaker_Overheat", vPos );

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqOverHeatIcon )
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqOverHeatIcon );


	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqOverHeatOverlapIcon )		/// 기존에 있던 오버 히트 중첩 이모티콘은 해제
	{
		CKTDGParticleSystem::CParticleEventSequence* pEmoticon = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqOverHeatOverlapIcon );

		if( NULL !=  pEmoticon )
		{
			pEmoticon->SetEmitRate( 0, 0 );
			pEmoticon->ClearAllParticle();
		}

		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqOverHeatOverlapIcon );
	}

	D3DXVECTOR3 vOffsetOverlap = ( vDirVector * 50.f );
	vPos += vOffsetOverlap;
	vPos.y += 20.0f;

	/// 데들리 레이드 단계적 오버 히트 중첩 이모티콘 생성
	if( 3 == iSize )
		m_hSeqOverHeatIcon = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*)this, L"Overlap_EDT_Emoticon_3_OverHeat", vPos );
	else if ( 2 == iSize )
		m_hSeqOverHeatIcon = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*)this, L"Overlap_EDT_Emoticon_2_OverHeat", vPos );
	else if ( 1 == iSize )
		m_hSeqOverHeatIcon = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*)this, L"Overlap_EDT_Emoticon_1_OverHeat", vPos );

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqOverHeatIcon )
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqOverHeatIcon );

#else  SERV_RAVEN_VETERAN_COMMANDER
	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"WeaponTaker_Overheat", vPos);
#endif SERV_RAVEN_VETERAN_COMMANDER

	UpDownCrashCamera( 30.0f, 0.5f );
}
#ifdef SERV_RAVEN_VETERAN_COMMANDER
bool CX2GURaven::_CanOverheat( bool bShowMassage/*= true*/ )
#else  SERV_RAVEN_VETERAN_COMMANDER
bool CX2GURaven::_CanOverheat()
#endif SERV_RAVEN_VETERAN_COMMANDER
{
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	float LimitHP = _CONST_RAVEN_::OVERHEAT_HP_LIMIT;

	/// 용병의 생존술 - 오버 히트 체력 한계 증가 or
	/// 용병의 승리전략
	if( (true == m_bEnableSurvivalTechniqueOfMercenary && true == m_bActiveSurvivalTechniqueOfMercenary)

#ifdef UPGRADE_SKILL_SYSTEM_2013	/// 김현철 // 2013-07-1 // 용병의 승리전략을 배웠을 시에도 적용 되도록 수정
		|| m_fOverheatDamageLimitedRate > 0.0f 
#endif // UPGRADE_SKILL_SYSTEM_2013
		)
		LimitHP = m_fLimitOverHeatHP;

	if( GetNowHp() / GetMaxHp() >= LimitHP )
		return true;

	if( true == bShowMassage )
	{
		g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, 
			GET_STRING( STR_ID_15437 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
	}
#else  SERV_RAVEN_VETERAN_COMMANDER
	if( GetNowHp() / GetMaxHp() >= _CONST_RAVEN_::OVERHEAT_HP_LIMIT )
		return true;

	g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, 
		GET_STRING( STR_ID_15437 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif SERV_RAVEN_VETERAN_COMMANDER

	return false;
}

#ifdef SERV_RAVEN_VETERAN_COMMANDER		//HP 감소율을 추가( 데들리 레이드 )
void CX2GURaven::_BurnOverheatHP( const CX2DamageManager::DamageData& AttDamageData, float fDecreaseHPRate /*= -1.f*/, float fDecreaseHPRate2 /*= 1.f*/ )
#else  SERV_RAVEN_VETERAN_COMMANDER
void CX2GURaven::_BurnOverheatHP( const CX2DamageManager::DamageData& AttDamageData )
#endif SERV_RAVEN_VETERAN_COMMANDER
{
	//if( pAttDamageData == NULL )
	//	return;

	CX2DamageManager::DamageData kOverheatDamageData = AttDamageData;
#ifdef SERV_RAVEN_VETERAN_COMMANDER		//인자로 받은 감소율에 따라, 받는 데미지 연산
	if( -1.f == fDecreaseHPRate )
		fDecreaseHPRate = _CONST_RAVEN_::DEFAULT_OVERHEAT_HP_PENALTY;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 현재 HP 가 용병의 승리전략 제한 HP 보다 낮으면 0 으로 설정한다.
	if( GetNowHp() / GetMaxHp() < ( m_fOverheatDamageLimitedRate ) )
	{
		fDecreaseHPRate = 0;
	}
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈


	kOverheatDamageData.damage.fMagic *= m_fOverheatDamageRate * fDecreaseHPRate * fDecreaseHPRate2;
	kOverheatDamageData.damage.fPhysic *= m_fOverheatDamageRate * fDecreaseHPRate * fDecreaseHPRate2;

#else  SERV_RAVEN_VETERAN_COMMANDER
	kOverheatDamageData.damage.fMagic *= m_fOverheatDamageRate * _CONST_RAVEN_::DEFAULT_OVERHEAT_HP_PENALTY;
	kOverheatDamageData.damage.fPhysic *= m_fOverheatDamageRate * _CONST_RAVEN_::DEFAULT_OVERHEAT_HP_PENALTY;
#endif SERV_RAVEN_VETERAN_COMMANDER
	kOverheatDamageData.optrDefenderGameUnit = this;
	kOverheatDamageData.m_fCriticalRate = 0.f;

//	ModifyFinalDamageByGameUnitType( &kOverheatDamageData );
	float fFinalDamage = GetFinalDamage( &kOverheatDamageData );

	//// 뎀감. 모든 계산을 거친 최종 대미지에 옵션 수치화의 데미지 감소를 적용 (snatch에 영향을 주지 않음)
	//if ( 0.0f < m_SocketData.m_fAdditionalDefenceValue )
	//{
	//	fFinalDamage -= fFinalDamage * CX2SocketItem::GetFinalAdditionalDefencePercent( m_SocketData.m_fAdditionalDefenceValue, GetUnitLevel() );
	//}

	//// 추뎀
	//const float fAdditionalAttackValue = kOverheatDamageData.pAttackerUnit->GetAdditionalAttackDamage( &kOverheatDamageData );
	//if ( 0.0f < fAdditionalAttackValue )
	//{
	//	fFinalDamage += fAdditionalAttackValue * m_fOverheatDamageRate * _CONST_RAVEN_::DEFAULT_OVERHEAT_HP_PENALTY;
	//}

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	if( true == m_bEnableSurvivalTechniqueOfMercenary )		/// 용병의 생존술 적용 상태에서 오버 히트 데미지가 남은 체력보다 클 때, 체력 1 남김
	{
		if( 0.f >= GetNowHp() - fFinalDamage )
			fFinalDamage = GetNowHp() - 1.f;
	}
#endif SERV_RAVEN_VETERAN_COMMANDER

	// 데미지 적용
	if( IsMyUnit() == true )
	{
		UpNowHp( -fFinalDamage );
	}

#ifndef SERV_RAVEN_VETERAN_COMMANDER		/// 주의 : 베테랑 커맨더 디파인이 없을 때
	// 1레벨 화상 5초간 적용
	kOverheatDamageData.m_ExtraDamage.m_ExtraDamageType = CX2DamageManager::EDT_FIRE;
	kOverheatDamageData.m_ExtraDamage.m_fTime = 5.f;
	kOverheatDamageData.m_ExtraDamage.m_iLevel = 1;
	Process_ExtraDamage( &kOverheatDamageData, kOverheatDamageData.m_ExtraDamage );
#endif SERV_RAVEN_VETERAN_COMMANDER
}

void CX2GURaven::_SetOverheat( bool bOn /*= true*/, float fDuration /*= -1.f*/ )
{
	m_bOverheated = bOn;

	if( fDuration > 0.f )
	{
		m_fOverheatBuffTime = fDuration;
		
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		m_fOverheatBuffTime += m_fAddOverHeatBuffDurationTime; // 과열모드 작동! 버프 시간 증가	
		if ( m_iAddOverHeatLevel > 0 )
			SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_OVER_HEAT_ENHANCE ), m_iAddOverHeatLevel );
		else
			SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_OVER_HEAT ) );
#else
		SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_OVER_HEAT ) );
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	}

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	if( true == bOn )
		CreateOverHeatObject();
#endif SERV_RAVEN_VETERAN_COMMANDER
}

void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_LOOP_StateStart()
{
	++m_iRWTComboLoopCount;
	if( m_iRWTComboLoopCount > _CONST_RAVEN_::MIN_COMBO_XXX_LOOP_COUNT )
		_SetOverheat( true, 5.f );
	else
		_SetOverheat( false );

	CommonStateStart();
}

void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_LOOP_FrameMove()
{
	CommonFrameMove();

	if( m_bOverheated )
	{
		m_FrameDataNow.stateParam.bSuperArmor = true;
		m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
	}

	// 데미지 이펙트로 공격할 시점
	const float fAttackTime = 0.11f;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fAttackTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fAttackTime ) == true && EventCheck( fAttackTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( m_bOverheated )
		{
			CX2DamageEffect::CEffect* pEffect =
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RWT_COMBO_XXX_LOOP_OVERHEAT",
				GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

			if( pEffect != NULL )
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				_BurnOverheatHP( pEffect->GetDamageData(), -1.f, 0.5f );
#else  SERV_RAVEN_VETERAN_COMMANDER
				_BurnOverheatHP( pEffect->GetDamageData() );
#endif SERV_RAVEN_VETERAN_COMMANDER
		}
		else
		{
			CX2DamageEffect::CEffect* pEffect =
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RWT_COMBO_XXX_LOOP",
				GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
	}
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_LOOP_EventProcess()
{
	// 키 입력시에도 미리 오버히트 검사를 해 둡니다.
	if( m_InputData.oneX == true && _CanOverheat() )
	{
		if( m_iRWTMaxComboLoopCount < _CONST_RAVEN_::MAX_COMBO_XXX_LOOP_COUNT )
			++m_iRWTMaxComboLoopCount;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_iRWTMaxComboLoopCount <= m_iRWTComboLoopCount )
		{
			StateChange( RSI_WEAPON_TAKER_COMBO_XXX_FINISH );
		}
		else
		{
			StateChange( RSI_WEAPON_TAKER_COMBO_XXX_LOOP );
		}
	}
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#endif SERV_RAVEN_VETERAN_COMMANDER

	CommonEventProcess();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_LOOP_StateEnd()
{
	CommonStateEnd();
}

void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_FINISH_FrameMove()
{
	CommonFrameMove();

	if( m_bOverheated )
	{
		m_FrameDataNow.stateParam.bSuperArmor = true;
		m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
	}
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_XXX_FINISH_EventProcess()
{
	// 이 시간 이후부터 캔슬 입력 가능
	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.440f;

	// 발밑에 라인맵이 없으면
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_RAVEN_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	// 스킬 캔슬
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
#else  SERV_RAVEN_VETERAN_COMMANDER
	SKILL_CANCEL_AFTER( 0.67f )
#endif SERV_RAVEN_VETERAN_COMMANDER
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RWT_COMBO_ZZZX_Start()
{
	CommonStateStart();
}
void CX2GURaven::RWT_COMBO_ZZZX_FrameMove()
{
	CommonFrameMove();
}
void CX2GURaven::RWT_COMBO_ZZZX_EventProcess()
{
	// 이벤트 플래그가 무엇을 나타내는지 알기 쉽게 하도록
	bool &bCanChangeStateComboZZZXX			= m_FrameDataFuture.stateParam.bEventFlagList[0];

	const float LOWER_BOUND_INPUT_TIME_COMBO = 0.533f;	// 이 시간부터 콤보 입력 가능
	// 콤보입력이 가능한 시점
	if( LOWER_BOUND_INPUT_TIME_COMBO < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// X 키를 눌렀으면 ZZZXX 콤보로
		if( true == m_InputData.oneX )
		{
			bCanChangeStateComboZZZXX = true;
		}
	}

	const float TRIGGER_TIME_COMBO_X	= 0.567f;	// Combo_X의 분기점
	if( TRIGGER_TIME_COMBO_X < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{				
		// ZZZXX 콤보 실행, 땅에 있을 때만
		if( true == bCanChangeStateComboZZZXX &&
			true == IsOnSomethingFuture() )
		{
			StateChange( RSI_WEAPON_TAKER_COMBO_ZZZXX );
		}
	}

	// 아무 입력 없이 그냥 애니메이션이 끝났으면
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		// 발밑에 라인맵이 없으면
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_RAVEN_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}


	// 이 시간 이후부터 캔슬 입력 가능
	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.60f;
	if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// 대시 캔슬
		if( CAN_DASH_CANCEL )
		{			
			StateChangeDashIfPossible();
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RWT_COMBO_ZZZX_EndFuture()
{
	CommonStateEndFuture();

	m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;
}

void CX2GURaven::RSI_WEAPON_TAKER_COMBO_ZZZXX_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_ZZZXX_EventProcess()
{
	// 발밑에 라인맵이 없으면
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_RAVEN_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;

		m_PhysicParam.nowSpeed.x = 0;
		m_PhysicParam.passiveSpeed.x = -1;
	}
	// 라인맵 위에 있고 대시도 하지 않았으면
	else 
	{
		const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.200f;		// 이 시간 이후부터 캔슬 입력 가능
		// 캔슬입력이 가능한 시점
		if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
		{
			// 대시 캔슬
			if( CAN_DASH_CANCEL )
			{			
				StateChangeDashIfPossible();
			}
		}

		// 아무 입력 없이 그냥 애니메이션이 끝났으면
		if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_ZZZdownX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.335f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.335f ) == true && EventCheck( 0.335f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp(2.0f) == true )
		{
			//포쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RWT_COMBO_X_SHELL",
				GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				PlaySequenceByTriggerCount( pSeq, pos, 100, 100, 1 );
				pSeq->SetAxisAngle( GetRotateDegree() );
				pSeq->SetAddRotate( GetRotateDegree() );
			}
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_ZZZdownX_EventProcess()
{
	// 이벤트 플래그가 무엇을 나타내는지 알기 쉽게 하도록
	bool &bCanChangeStateComboZZZdownXX			= m_FrameDataFuture.stateParam.bEventFlagList[0];

	const float LOWER_BOUND_INPUT_TIME_COMBO = 0.533f;	// 이 시간부터 콤보 입력 가능
	// 콤보입력이 가능한 시점
	if( LOWER_BOUND_INPUT_TIME_COMBO < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// X 키를 눌렀으면 ZZZdownXX 콤보로
		if( true == m_InputData.oneX )
		{
			bCanChangeStateComboZZZdownXX = true;
		}
	}

	const float TRIGGER_TIME_COMBO_X = 0.739f;	// Combo_X의 분기점
	if( TRIGGER_TIME_COMBO_X < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{				
		// ZZZdownXX 콤보 실행, 땅에 있을 때만
		if( true == bCanChangeStateComboZZZdownXX &&
			true == IsOnSomethingFuture() )
		{
			StateChange( RSI_WEAPON_TAKER_COMBO_ZZZdownXX );
		}
	}


	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.45f;			// 이 시간 이후부터 캔슬 입력 가능
	// 캔슬입력이 가능한 시점
	if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// 대시 캔슬
		if( CAN_DASH_CANCEL )
		{			
			StateChangeDashIfPossible();
			bCanChangeStateComboZZZdownXX = false;
		}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		SKILL_CANCEL_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
#else // SKILL_CANCEL_BY_HYPER_MODE
		ELSE_IF_ASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		{
			if( true == SpecialAttackEventProcess() )
				bCanChangeStateComboZZZdownXX = false;
		}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	}


	// 아무 입력 없이 그냥 애니메이션이 끝났으면
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		// 발밑에 라인맵이 없으면
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_RAVEN_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_ZZZdownXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.225f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.225f ) == true && EventCheck( 0.225f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp(2.0f) == true )
		{
			//포쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RWT_COMBO_ZZZ_DOWN_XX_SHELL",
				GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				PlaySequenceByTriggerCount( pSeq, pos, 100, 100, 1 );
				pSeq->SetAxisAngle( GetRotateDegree() );
				pSeq->SetAddRotate( GetRotateDegree() );
			}
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_WEAPON_TAKER_COMBO_ZZZdownXX_EventProcess()
{
	// 이 시간 이후부터 캔슬 입력 가능
	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.85f;
	if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// 이동 캔슬
		if( CAN_WALK_CANCEL )
		{			
			StateChange( USI_WALK );
		}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		SKILL_CANCEL_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
#else // SKILL_CANCEL_BY_HYPER_MODE
		ELSE_IF_ASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		{
			SpecialAttackEventProcess();
		}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		

	CommonEventProcess();
}

void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	//m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	//m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}
void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_FrameMove()
{
	CommonFrameMove();
}
void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_EventProcess()
{
	// 이벤트 플래그가 무엇을 나타내는지 알기 쉽게 하도록
	bool& bCanChangeStateDashJumpComboZXX = m_FrameDataFuture.stateParam.bEventFlagList[0];

	// 분기점
	const float TRIGGER_TIME_RWT_COMBO_DASH_JUMP_ZXX = 0.635f;
	// 이 시간 이후부터 캔슬 입력 가능
	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.420f;		

	if( TRIGGER_TIME_RWT_COMBO_DASH_JUMP_ZXX < m_pXSkinAnimFuture->GetNowAnimationTime()  )
	{
		// 발 아래에 라인맵이 있으면 착지
		if ( false == IsOnSomethingFuture() )
		{
			if( bCanChangeStateDashJumpComboZXX )
				StateChange( RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX );
			else
				m_bDisableGravity = false;
		}
		else
		{
			StateChange( USI_JUMP_LANDING );
		}
	} // if
	// 캔슬입력이 가능한 시점
	else if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( false ) {}
		ELSE_IF_ASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		{
			if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
				bCanChangeStateDashJumpComboZXX = false;
		}

	}
	else if ( true == m_InputData.oneX )
	{
		bCanChangeStateDashJumpComboZXX = true;
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZX_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}


void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_StateStart()
{
	CommonStateStart();
}
void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_FrameMove()
{
	CommonFrameMove();
}
void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_EventProcess()
{
	// 이 시간 이후부터 캔슬 입력 가능
	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.240f;

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
	}
	else if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( false ) {}
		ELSE_IF_ASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		{
			SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR );
		}	
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_WEAPON_TAKER_DASH_JUMP_COMBO_ZXX_StateEnd()
{
	CommonStateEnd();
}



// 레이븐 새 액티브 브레이킹 피스트 - 박진웅
void CX2GURaven::RSI_A_RF_BREAKING_FIST_Init()
{
}

void CX2GURaven::RSI_A_RF_BREAKING_FIST_StateStart()
{
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}


void CX2GURaven::RSI_A_RF_BREAKING_FIST_FrameMove()
{

	CommonFrameMove();
}

void CX2GURaven::RSI_A_RF_BREAKING_FIST_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

#ifdef BALANCE_BLADE_MASTER_20130117
	ELSE_IF_ZX_PRESSED_AFTER( m_fAttackCancelAfter )
	{
		StateChange( USI_WAIT );
	}
#else  BALANCE_BLADE_MASTER_20130117
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.821f )
		{
			StateChange( USI_WAIT );
		}
	}
#endif BALANCE_BLADE_MASTER_20130117

	CommonEventProcess();
}


void CX2GURaven::RSI_A_RF_BREAKING_FIST_StateEnd()
{
	CommonStateEnd();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = true;
}

#ifdef BALANCE_BLADE_MASTER_20130117
void CX2GURaven::RSI_A_RF_BREAKING_FIST_StateEndFuture()
{
	m_PhysicParam.nowSpeed.x = 0.f;

	CommonStateEndFuture();
}
#endif BALANCE_BLADE_MASTER_20130117


// 레이븐1차 웨폰테이커 연무폭파 - 박진웅
void CX2GURaven::RSI_A_RWT_BARRAGE_ATTACK_Init()
{
	XSkinMeshReadyInBackground( L"RWT_BARRAGE_ATTACK_TRACE_Air.X" );
	XSkinMeshReadyInBackground( L"RWT_BARRAGE_ATTACK_TRACE_Stand.X" );
}
void CX2GURaven::RSI_A_RWT_BARRAGE_ATTACK_StateStartFuture()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( RSI_A_RWT_BARRAGE_ATTACK_AIR );
	}
	else
	{
		StateChange( RSI_A_RWT_BARRAGE_ATTACK_STAND );
	}
	CommonStateStartFuture();
}
void CX2GURaven::RSI_A_RWT_BARRAGE_ATTACK_STAND_EventProcess()
{
	// 이 시간 이후부터 캔슬 입력 가능
	const float LOWER_BOUND_INPUT_TIME_CANCEL = 0.93f;
	if( LOWER_BOUND_INPUT_TIME_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		// 이동 캔슬
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{			
			StateChange( USI_WALK );
		}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		SKILL_CANCEL_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
#else // SKILL_CANCEL_BY_HYPER_MODE
		ELSE_IF_ASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		{
			SpecialAttackEventProcess();
		}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_A_RWT_BARRAGE_ATTACK_STAND_FrameMove()
{
	// 지상에서 딜레이는 0.4초
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		// 지상에서의 이펙트 오프셋
		D3DXVECTOR3 vDirVector = GetDirVector();
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01_L_Hand" );
		D3DXVECTOR3 vOffset = ( vDirVector * 25.f );
		vPos += GetIsRight() ? vOffset : -vOffset;
		vPos += D3DXVECTOR3( 0.f, 25.f, 0.f );

		CX2DamageEffect::CEffect* pDE = NULL;

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO13 ) == true )
		{
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_BARRAGE_ATTACK_STAND_MEMO", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
		}
		else
		{
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_BARRAGE_ATTACK_STAND", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
		}
		if( NULL != pDE )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK );
			if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			{
				if ( NULL == GetUnit()  )
					return;

				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

				pDE->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
			}
#else //UPGRADE_SKILL_SYSTEM_2013
				pDE->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_A_RWT_BARRAGE_ATTACK_AIR_EventProcess()
{
#ifdef RAVEN_LIMIT_AIR_COMBO		/// 공중 채공 콤보 제한
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( USI_JUMP_LANDING );
	}
	SKILL_CANCEL_AFTER_FLY( m_fSkillCancelAfterFly )
#else  RAVEN_LIMIT_AIR_COMBO
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_JUMP_LANDING );
	}
#endif RAVEN_LIMIT_AIR_COMBO

	CommonEventProcess();
}
void CX2GURaven::RSI_A_RWT_BARRAGE_ATTACK_AIR_FrameMove()
{
	// 공중0.1초
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		// 공중에서의 이펙트 오프셋
		D3DXVECTOR3 vDirVector = GetDirVector();
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01_L_Hand" );
		D3DXVECTOR3 vOffset = ( vDirVector * 75.f );
		vPos += GetIsRight() ? vOffset : -vOffset;
		vPos += D3DXVECTOR3( 0.f, -75.f, 0.f );

		CX2DamageEffect::CEffect* pDE = NULL;

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO13 ) == true )
		{
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_BARRAGE_ATTACK_AIR_MEMO", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
		}
		else
		{
			pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_BARRAGE_ATTACK_AIR", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
		}

		if( NULL != pDE )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK );
			if( NULL != pSkillTemplet )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RWT_BARRAGE_ATTACK );
				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				{
					if ( NULL == GetUnit()  )
						return;

					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

					pDE->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
				}
#else //UPGRADE_SKILL_SYSTEM_2013
					pDE->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
		}
	}

	CommonFrameMove();
}

#ifdef RAVEN_LIMIT_AIR_COMBO			/// 공중 채공 콤보 제한
void CX2GURaven::RSI_A_RWT_BARRAGE_ATTACK_AIR_FrameMoveFutrue()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )		/// 애니메이션 종료 후에는 마지막 모션 그대로 떨어진다.
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}

		if( m_InputData.pureDown == true )
		{
			m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		}
	}

	CommonFrameMoveFuture();
}
#endif RAVEN_LIMIT_AIR_COMBO


// 레이븐1차 웨폰테이커 버스팅 블레이드 - 박진웅
void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_Init()
{
	XSkinMeshReadyInBackground( L"BURSTING_BLADE_2Hit_Damage.X" );
	XSkinMeshReadyInBackground( L"BURSTING_BLADE_2Hit_SwordFire.X" );
	XSkinMeshReadyInBackground( L"RWT_BURSTING_BLADE_AttackDummy.X" );
	XSkinMeshReadyInBackground( L"SI_SA_RWT_BURSTING_BLADE_OverHit_Damage.X" );
	XSkinMeshReadyInBackground( L"SI_SA_RWT_BURSTING_BLADE_OverHit_SwordFire.X" );
}
void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_StateStartFuture()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( RSI_SA_RWT_BURSTING_BLADE_AIR );
	}
	else
	{
		StateChange( RSI_SA_RWT_BURSTING_BLADE_STAND );
	}
	CommonStateStartFuture();
}

void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateStart()
{
	CommonStateStart();

	// 버스팅블레이드가 이미 존재하면 삭제
	if( INVALID_MESH_INSTANCE_HANDLE != m_hBurstingBladeAttackBox )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hBurstingBladeAttackBox );
	}

	// 이후 생성
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"BurstingBladeAttackBox", 0,0,0, 0,0,0, 0,0,0 );
	if( pMeshInst != NULL )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		m_hBurstingBladeAttackBox = pMeshInst->GetHandle();
		pMeshInst->SetBoundingRadius( 0.0f );

		CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
		pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
		pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );

		SetSubAttackList( pMeshInst->GetXSkinAnim() );
	}

#ifdef ADDITIONAL_MEMO
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO14 ) == true )
	{
		m_DamageData.m_ExtraDamage.Init();
		m_DamageData.m_ExtraDamage.m_ExtraDamageType = CX2DamageManager::EDT_FIRE;
		m_DamageData.m_ExtraDamage.m_fTime = 10.f;
		m_DamageData.m_ExtraDamage.m_iLevel = 0;
		m_DamageData.m_ExtraDamage.m_fRate = 1.f;
		m_DamageData.m_ExtraDamage.m_fPureRate = 0.5f;
		m_DamageData.m_ExtraDamage.m_bIgnoreResist = true;
	}			
#endif
}

void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_ATTACK_FrameMove()
{
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈	ChangeWorldColorByHyperMode();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.8f ) && true == EventCheck( 0.8f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen )
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit	= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();

#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO14 ) == true )
			{
				m_DamageData.m_ExtraDamage.Init();
				m_DamageData.m_ExtraDamage.m_ExtraDamageType = CX2DamageManager::EDT_FIRE;
				m_DamageData.m_ExtraDamage.m_fTime = 10.f;
				m_DamageData.m_ExtraDamage.m_iLevel = 0;
				m_DamageData.m_ExtraDamage.m_fRate = 1.f;
				m_DamageData.m_ExtraDamage.m_fPureRate = 0.5f;
				m_DamageData.m_ExtraDamage.m_bIgnoreResist = true;
			}			
#endif
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 1.2f ) && true == EventCheck( 1.2f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen )
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_THIRD" );
			m_LuaManager.EndTable();

#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO14 ) == true )
			{
				m_DamageData.m_ExtraDamage.Init();
				m_DamageData.m_ExtraDamage.m_ExtraDamageType = CX2DamageManager::EDT_FIRE;
				m_DamageData.m_ExtraDamage.m_fTime = 10.f;
				m_DamageData.m_ExtraDamage.m_iLevel = 0;
				m_DamageData.m_ExtraDamage.m_fRate = 1.f;
				m_DamageData.m_ExtraDamage.m_fPureRate = 0.5f;
				m_DamageData.m_ExtraDamage.m_bIgnoreResist = true;
			}			
#endif
		}
	}

	if( m_hBurstingBladeAttackBox != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hBurstingBladeAttackBox );
		if( NULL != pMeshInst )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
		}
		else
		{
			m_hBurstingBladeAttackBox = INVALID_MESH_INSTANCE_HANDLE;
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_ATTACK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_JUMP_LANDING );
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_ATTACK_StateEnd()
{
	m_SubAttackListSet.clear();

	// 버스팅블레이드 삭제
	if( INVALID_MESH_INSTANCE_HANDLE != m_hBurstingBladeAttackBox )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hBurstingBladeAttackBox );
	}

	CommonStateEnd();
}

#ifdef RAVEN_LIMIT_AIR_COMBO		/// 공중 채공 콤보 제한
void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_ATTACK_AIR_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )		/// 애니메이션 종료 후에는 마지막 모션 그대로 떨어진다.
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}

		if( m_InputData.pureDown == true )
		{
			m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		}
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_SA_RWT_BURSTING_BLADE_ATTACK_AIR_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( USI_JUMP_LANDING );
	}
	SKILL_CANCEL_AFTER_FLY( m_fSkillCancelAfterFly )

		CommonEventProcess();
}
#endif RAVEN_LIMIT_AIR_COMBO

// 레이븐1차 웨폰테이커 리볼버 캐논 - 박진웅
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_Init()
{
	XSkinMeshReadyInBackground( L"High_Explosive_Shell02.X" );
	XSkinMeshReadyInBackground( L"Damage_Effect_Dummy.X" );
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_StateStart()
{
	CommonStateStart();

	_SetOverheat( false );
	m_bShowedOverheat = false;
	m_iRWTComboLoopCount = 0;
	m_iRWTMaxComboLoopCount = _CONST_RAVEN_::MIN_REVOLVER_CANNON_LOOP_COUNT;
	m_iOverheatKeyPressTime = 0;
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
			++m_iOverheatKeyPressTime;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_SA_RWT_REVOLVER_CANNON_LOOP );
	}
	else
	{
	}
	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_StateEnd()
{
	CommonStateEnd();
}


void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_LOOP_StateStart()
{
	++m_iRWTComboLoopCount;
	if( m_iRWTComboLoopCount > _CONST_RAVEN_::MIN_REVOLVER_CANNON_LOOP_COUNT )
		_SetOverheat( true, 5.f );
	else
		_SetOverheat( false );

	CommonStateStart();
}
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_LOOP_FrameMove()
{
	// 이벤트 플래그가 무엇을 나타내는지 알기 쉽게 하도록
	bool &bOverheat = m_FrameDataFuture.stateParam.bEventFlagList[0];

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( m_iRWTComboLoopCount == _CONST_RAVEN_::MIN_REVOLVER_CANNON_LOOP_COUNT + 1 )
			PlaySound( L"RavenVoice_Shout12.ogg" );

		//포쏘기
		D3DXVECTOR3 vDirVector = GetDirVector();
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01_L_Hand" );
		D3DXVECTOR3 vOffset = ( vDirVector * 50.f );
		vPos += GetIsRight() ? -vOffset : vOffset;

		float fPowerRate = GetPowerRate();
#ifdef ADDITIONAL_MEMO
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO15 ) == true )
			fPowerRate *= 0.8f;
#endif

		if( m_bOverheated )
		{
			CX2DamageEffect::CEffect* pEffect;
			if( m_iRWTComboLoopCount >= m_iRWTMaxComboLoopCount )
			{
				pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_FINAL_SHELL_OVERHEAT",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			}
			else
			{
				pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_SHELL_OVERHEAT",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			}

			if( pEffect != NULL )
				_BurnOverheatHP( pEffect->GetDamageData() );
		}
		else
		{
			if( m_iRWTComboLoopCount >= m_iRWTMaxComboLoopCount )
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_FINAL_SHELL",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_SHELL",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			}
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_LOOP_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
		{
			++m_iOverheatKeyPressTime;

			if( m_iOverheatKeyPressTime >= _CONST_RAVEN_::OVERHEAT_KEY_PRESS_TIME )
			{
				if( false == m_bShowedOverheat )
					_ShowOverheatIcon();
				m_bShowedOverheat = true;

				m_iRWTMaxComboLoopCount = _CONST_RAVEN_::MAX_REVOLVER_CANNON_LOOP_COUNT;
			}
		}

	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		// 한발이 모자라도 넘어가자-피니시에서 한방 쏘니까
		if( m_iRWTComboLoopCount + 1 >= m_iRWTMaxComboLoopCount )
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_FINISH );
		}
		else
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_LOOP );
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_LOOP_StateEnd()
{
	CommonStateEnd();
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_bOverheated )
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_OVER_END );
		}
		else
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_END );
		}
	}
}
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
}





// 레이븐1차 웨폰테이커 하푼 스피어 - 박진웅
void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_Init()
{
	XSkinMeshReadyInBackground( L"High_Explosive_Shell03.X" );
	XSkinMeshReadyInBackground( L"HARPOON_SPEAR_Light_Mesh.X" );
}

void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	m_iRVCComboLoopInputCount = 1;		/// 오버 히트 입력 카운트
	m_bShowedOverheat		  = false;	/// 오버 히트 아이콘 적용 여부
#endif SERV_RAVEN_VETERAN_COMMANDER
}

void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );
	
#ifndef SERV_RAVEN_VETERAN_COMMANDER		/// 주의 : 베테랑 커맨더 디파인 없을 때
	// 0.75초에 중력을 다시 켠다.
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.75f ) == true && EventCheck( 0.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}
#endif SERV_RAVEN_VETERAN_COMMANDER


	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_EventProcess()
{
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
			++m_iRVCComboLoopInputCount;

		if( m_iRVCComboLoopInputCount >= _CONST_RAVEN_::HARPOON_SPEAR_OVERHEAT_KEY_PRESS_TIME )
		{
			if( false == m_bShowedOverheat )
			{
				_ShowOverheatIcon();
				m_bShowedOverheat = true;
			}
		}
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == m_bShowedOverheat )		/// 오버 히트 발사
			StateChange( RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT );
		else								/// 일반 발사
			StateChange( RSI_SA_RWT_HARPOON_SPEAR_FIRE );
	}
#else  SERV_RAVEN_VETERAN_COMMANDER
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true ) 
	{
		StateChange( USI_JUMP_DOWN );
	}
#endif SERV_RAVEN_VETERAN_COMMANDER

	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_StateEnd()
{
	m_bDisableGravity = false;

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	m_iRVCComboLoopInputCount	= 0;		/// 오버 히트 입력 카운트
#endif SERV_RAVEN_VETERAN_COMMANDER
	CommonStateEnd();
}


#ifdef SERV_RAVEN_VETERAN_COMMANDER
/// 일반
void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_FIRE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_JUMP_DOWN );
	}

	CommonEventProcess();
}

/// 오버 히트
void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_StateStart()
{
	_SetOverheat( true, 5.f );

	CommonStateStart();
}
void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		/// 오버 히트 데미지 처리
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_HARPOON_SPEAR_SHELL_OVERHEAT_PENALTY",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );

		if( NULL != pEffect )
		{
			_BurnOverheatHP( pEffect->GetDamageData() );

			g_pX2Game->GetDamageEffect()->DestroyInstance( pEffect );
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_RWT_HARPOON_SPEAR_FIRE_OVERHEAT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_JUMP_DOWN );
	}

	CommonEventProcess();
}
#endif SERV_RAVEN_VETERAN_COMMANDER


// 레이븐1차 웨폰테이커 헬파이어 개틀링 - 박진웅
void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_Init()
{
	XSkinMeshReadyInBackground( L"raven_SA_hellFire_attackSphere.X" );
}

void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_StateStart()
{
	CommonStateStart();

	_SetOverheat( false );
	m_bShowedOverheat = false;
	m_iRWTComboLoopCount = 0;
	m_iRWTMaxComboLoopCount = _CONST_RAVEN_::MIN_HELLFIRE_GATLING_LOOP_COUNT;
	m_iOverheatKeyPressTime = 0;
}

void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );


	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
			++m_iOverheatKeyPressTime;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_SA_RWT_HELLFIRE_GATLING_LOOP );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_StateEnd()
{
	CommonStateEnd();
}


void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_LOOP_StateStart()
{
	++m_iRWTComboLoopCount;
	if( m_iRWTComboLoopCount > _CONST_RAVEN_::MIN_HELLFIRE_GATLING_LOOP_COUNT )
		_SetOverheat( true, 5.f );
	else
		_SetOverheat( false );

	CommonStateStart();
}
void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_LOOP_FrameMove()
{
	bool bFire = false;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Hellfire_Gatling", (CX2GameUnit*)this, (CX2GameUnit*)this );
		bFire = true;
	}

	if( m_iRWTComboLoopCount >= 4 && m_iRWTComboLoopCount <= 8 )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.09f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.09f ) == true && EventCheck( 0.09f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			bFire = true;
	}
	else if( m_iRWTComboLoopCount >= 9 )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if( m_pXSkinAnim->EventTimerOneshot( 0.06f ) || m_pXSkinAnim->EventTimerOneshot( 0.12f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.06f ) == true && EventCheck( 0.06f, false ) == true ||
			m_pXSkinAnim->EventTimer( 0.12f ) == true && EventCheck( 0.12f, false ) == true ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			bFire = true;
	}

	if( bFire )
	{
		if( m_iRWTComboLoopCount == _CONST_RAVEN_::MIN_HELLFIRE_GATLING_LOOP_COUNT + 1 )
			PlaySound( L"RavenVoice_Shout12.ogg" );

		//포쏘기
		D3DXVECTOR3 vDirVector = GetDirVector();
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01_L_Hand" );
		D3DXVECTOR3 vOffset = ( vDirVector * 50.f );
		vPos += GetIsRight() ? -vOffset : vOffset;

		if( m_bOverheated )
		{
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO29 ) == true )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_HELLFIRE_GATLING_BULLET_OVERHEAT_MEMO",
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), GetYPos() );
			else
#endif //ADD_MEMO_1ST_CLASS
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_HELLFIRE_GATLING_BULLET_OVERHEAT",
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), GetYPos() );

			// 오버히트에 사용될 데미지 재현을 위한 임시 이펙트
			CX2DamageEffect::CEffect* pEffect = NULL;

#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO29 ) == true )
				pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_HELLFIRE_GATLING_IMPACT_OVERHEAT_MEMO",
				GetPowerRate(), D3DXVECTOR3( 0.f, -1000.f, 0.f ), GetRotateDegree(), GetRotateDegree(), GetYPos() );
			else
#endif //ADD_MEMO_1ST_CLASS
				pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_HELLFIRE_GATLING_IMPACT_OVERHEAT",
				GetPowerRate(), D3DXVECTOR3( 0.f, -1000.f, 0.f ), GetRotateDegree(), GetRotateDegree(), GetYPos() );
			if( pEffect != NULL )
				_BurnOverheatHP( pEffect->GetDamageData() );

			g_pX2Game->GetDamageEffect()->DestroyInstance( pEffect );
		}
		else
		{
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO29 ) == true )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_HELLFIRE_GATLING_BULLET_MEMO",
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), GetYPos() );
			else
#endif //ADD_MEMO_1ST_CLASS
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_HELLFIRE_GATLING_BULLET",
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), GetYPos() );
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_LOOP_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
		{
			++m_iOverheatKeyPressTime;

			if( m_iOverheatKeyPressTime >= _CONST_RAVEN_::OVERHEAT_KEY_PRESS_TIME )
			{
				if( false == m_bShowedOverheat )
					_ShowOverheatIcon();
				m_bShowedOverheat = true;

				m_iRWTMaxComboLoopCount = _CONST_RAVEN_::MAX_HELLFIRE_GATLING_LOOP_COUNT;
			}
		}
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_iRWTComboLoopCount >= m_iRWTMaxComboLoopCount )
		{
			if( m_bOverheated )
				StateChange( RSI_SA_RWT_HELLFIRE_GATLING_OVER_END );
			else
				StateChange( RSI_SA_RWT_HELLFIRE_GATLING_END );
		}
		else
		{
			StateChange( RSI_SA_RWT_HELLFIRE_GATLING_LOOP );
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_LOOP_StateEnd()
{
	CommonStateEnd();
}

void CX2GURaven::RSI_SA_RWT_HELLFIRE_GATLING_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
}



// 레이븐1차 웨폰테이커 기가 프로미넌스 - 박진웅
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_Init()
{
	XSkinMeshReadyInBackground( L"PROMINENCE_stone01.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_Light_Mesh01.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_Final_ATTACK_SPHERE.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_Flame_ATTACK_SPHERE.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_pillar_fire01.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_pillar_fire02.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_pillar_fire03.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_PowerWave.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_Smoke_ATTACK_SPHERE.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_smoke01.X" );
	XSkinMeshReadyInBackground( L"RWT_GIGA_PROMINENCE_smoke02.X" );
}

void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_StateStart()
{
	CommonStateStart();

	_SetOverheat( false );
	m_bShowedOverheat = false;
	m_iRWTComboLoopCount = 0;
	m_iOverheatKeyPressTime = 0;

	m_vecLeftSpot.clear();
	m_vecRightSpot.clear();
}

void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );


	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
			++m_iOverheatKeyPressTime;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_SA_RWT_GIGA_PROMINENCE_LOOP );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_StateEnd()
{
	CommonStateEnd();
}


void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_LOOP_StateStart()
{
	CommonStateStart();

	++m_iRWTComboLoopCount;
}

void CX2GURaven::_GetGigaProminencePosition( OUT bool& bRight, OUT float& fDistance )
{
	if( m_vecRightSpot.empty() )
	{
		m_vecLeftSpot.push_back( 1 );
		m_vecLeftSpot.push_back( 2 );
		m_vecLeftSpot.push_back( 3 );
		m_vecRightSpot.push_back( 1 );
		m_vecRightSpot.push_back( 2 );
		m_vecRightSpot.push_back( 3 );
	}

	int iSpot = 0;
	bRight = ( m_vecLeftSpot.size() < m_vecRightSpot.size() ) ? true : false;
	if( true == bRight )
	{
		int iVectorIndex = GetRandomInt() % m_vecRightSpot.size();;
		iSpot = m_vecRightSpot[iVectorIndex];
		m_vecRightSpot.erase( m_vecRightSpot.begin() + iVectorIndex );
	}
	else
	{
		int iVectorIndex = GetRandomInt() % m_vecRightSpot.size();;
		iSpot = m_vecLeftSpot[iVectorIndex];
		m_vecLeftSpot.erase( m_vecLeftSpot.begin() + iVectorIndex );
	}
	
	if( iSpot == 1 )
		fDistance = 150.f;
	else if( iSpot == 2 )
		fDistance = 375.f;
	else if( iSpot == 3 )
		fDistance = 600.f;
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_LOOP_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) || m_pXSkinAnim->EventTimerOneshot( 0.24f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true ||
		m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bRight = false;
		float fDistance = 0.f;
		_GetGigaProminencePosition( bRight, fDistance );
		
		D3DXVECTOR3 vPos;
		if( g_pX2Game->GetWorld()->GetLineMap()->GetFixedDistancePositionForWeaponTaker( &GetPos(), fDistance, bRight, vPos ) )
		{
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true )
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Giga_Prominence_Flame_MEMO",
				this, NULL, false, GetPowerRate(), -1.f, D3DXVECTOR3( 1, 1, 1 ), true, vPos, GetRotateDegree(), GetDirVector() );
			else
#endif //ADD_MEMO_1ST_CLASS
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Giga_Prominence_Flame",
					this, NULL, false, GetPowerRate(), -1.f, D3DXVECTOR3( 1, 1, 1 ), true, vPos, GetRotateDegree(), GetDirVector() );
		}
		else
		{
			// 해당 위치에 라인맵이 없을 때
			D3DXVECTOR3 vNewLinemapPos;
			int iNewLineIndex;

			if( true == g_pX2Game->GetWorld()->GetLineMap()->GetFixedDistancePositionInAllLine( GetDirVector(), GetPos(), fDistance, bRight, vNewLinemapPos, iNewLineIndex ) )
			{
				#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true )
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Giga_Prominence_Flame_MEMO",
				this, NULL, false, GetPowerRate(), -1.f, D3DXVECTOR3( 1, 1, 1 ), true, vPos, GetRotateDegree(), GetDirVector() );
			else
#endif //ADD_MEMO_1ST_CLASS
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Giga_Prominence_Flame",
					this, NULL, false, GetPowerRate(), -1.f, D3DXVECTOR3( 1, 1, 1 ), true, vNewLinemapPos, GetRotateDegree(), GetDirVector() );
			}
		}
		
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_LOOP_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
		{
			++m_iOverheatKeyPressTime;
		}
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_iRWTComboLoopCount >= _CONST_RAVEN_::MAX_GIGA_PROMINENCE_LOOP_COUNT )
		{
			if( m_iOverheatKeyPressTime >= _CONST_RAVEN_::OVERHEAT_KEY_PRESS_TIME )
				StateChange( RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH );
			else
				StateChange( RSI_SA_RWT_GIGA_PROMINENCE_FINISH );
		}
		else
		{
			StateChange( RSI_SA_RWT_GIGA_PROMINENCE_LOOP );
		}
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_LOOP_StateEnd()
{
	CommonStateEnd();
}

void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_FINISH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		// 왼쪽
		CX2DamageEffect::CEffect* pEffect =
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH_STONE",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );
        if ( CKTDGXMeshPlayer::CXMeshInstance* pLeftMeshInstance = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			pLeftMeshInstance->SetDirSpeed( -pLeftMeshInstance->GetDirSpeed() );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);

		pEffect =
#endif //ADD_MEMO_1ST_CLASS
		// 오른쪽
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH_STONE",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );

#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);
#endif //ADD_MEMO_1ST_CLASS
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		// 왼쪽공격
		CX2DamageEffect::CEffect* pEffect =
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );
        if ( CKTDGXMeshPlayer::CXMeshInstance* pLeftMeshInstance = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			pLeftMeshInstance->SetDirSpeed( -pLeftMeshInstance->GetDirSpeed() );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);

		pEffect =
#endif //ADD_MEMO_1ST_CLASS
		// 오른쪽
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);
#endif //ADD_MEMO_1ST_CLASS
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_SA_RWT_GIGA_PROMINENCE_END );
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_StateStart()
{
	_ShowOverheatIcon();
	_SetOverheat( true, 5.f );

	CommonStateStart();
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		PlaySound( L"RavenVoice_Shout12.ogg" );

		// 연무
		CX2DamageEffect::CEffect* pEffect =
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH_SMOKE",
			GetPowerRate(), GetBonePos( L"Bip01_L_Hand" ), GetRotateDegree(), GetRotateDegree() );

		// 오버히트 데미지
		if( pEffect )
			_BurnOverheatHP( pEffect->GetDamageData() );

#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);
#endif //ADD_MEMO_1ST_CLASS
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		// 왼쪽
		CX2DamageEffect::CEffect* pEffect =
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH_STONE",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );
        if ( CKTDGXMeshPlayer::CXMeshInstance* pLeftMeshInstance = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			pLeftMeshInstance->SetDirSpeed( -pLeftMeshInstance->GetDirSpeed() );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);

		pEffect =
#endif //ADD_MEMO_1ST_CLASS
		// 오른쪽
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH_STONE",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);
#endif //ADD_MEMO_1ST_CLASS
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		// 왼쪽공격
		CX2DamageEffect::CEffect* pEffect =
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH_OVERHEAT",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );
        if ( CKTDGXMeshPlayer::CXMeshInstance* pLeftMeshInstance = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			pLeftMeshInstance->SetDirSpeed( -pLeftMeshInstance->GetDirSpeed() );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);

		pEffect =
#endif //ADD_MEMO_1ST_CLASS
		// 오른쪽
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_GIGA_PROMINENCE_FINISH_OVERHEAT",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), GetYPos() );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO30 ) == true && pEffect != NULL )
			pEffect->SetScale(1.1f, 1.1f, 1.1f);
#endif //ADD_MEMO_1ST_CLASS
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_OVER_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_SA_RWT_GIGA_PROMINENCE_OVER_END );
	}

	CommonEventProcess();
}
void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_OVER_END_StateStart()
{
	_SetOverheat( false );
	CommonStateStart();
}


void CX2GURaven::RSI_SA_RWT_GIGA_PROMINENCE_OVER_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

#endif RAVEN_WEAPON_TAKER

void CX2GURaven::DeleteMinorParticle()
{
	CX2GUUser::DeleteMinorParticle();

	DeleteRavenMinorParticle();
}

#ifdef SERV_RAVEN_VETERAN_COMMANDER
/// RVC_DASH_JUMP_COMBO_X
void CX2GURaven::RVC_DASH_JUMP_COMBO_X_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"Hand_FireImpact01.X" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"Hand_FireImpact01.X" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_X_Start()
{
	m_iRVCComboLoopCount		= 1;	//첫 발사 카운트

	m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
	m_DamageData.bCanRevenge	= false;

	CommonStateStart();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_X_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )		/// 애니메이션 종료 후에는 마지막 모션 그대로 떨어진다.
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}

		if( m_InputData.pureDown == true )
		{
			m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		}
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.21f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.7f )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.26f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.29f )
		{			
			m_PhysicParam.nowSpeed.y = 700.0f;
		}
		else
		{
			if( m_PhysicParam.nowSpeed.y < 0.0f )
				m_PhysicParam.nowSpeed.y = 0.0f;
		}			
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.26f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.26f ) && EventCheck( 0.26f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDir = GetRotateDegree();
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		vDir.z -= 45.f;

		if( FlushMp( 10.0f ) == true )
		{ 
			//발사 이펙트
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Dash_Jump_Fire", (CX2GameUnit*)this, (CX2GameUnit*)this );

			//소이탄 데미지 이펙트
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DASH_JUMP_X_BULLET",
				GetPowerRate(), vPos, vDir, vDir, m_FrameDataNow.unitCondition.landPosition.y );

			g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( 10.0f, 0.2f );
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
			if( pSeq != NULL )
			{
				PlaySequenceByTriggerCount( pSeq, vPos, 100, 100, 1 );
				pSeq->SetAxisAngle( vDir );
				pSeq->SetAddRotate( vDir );
			}
		}

	}

	CommonFrameMove();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_X_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0])
		{
			StateChange( RVC_DASH_JUMP_COMBO_XX_START );
		}
		else if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
	}
	else if( true == m_InputData.oneX )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	SKILL_CANCEL_AFTER_FLY( m_fSkillCancelAfterFly )

	CommonEventProcess();
}

/// RVC_DASH_JUMP_COMBO_XX_START
void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_START_StateStart()
{
	CommonStateStart();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_START_FrameMove()
{
	CommonFrameMove();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_START_FrameMoveFuture()
{
	if( m_PhysicParam.nowSpeed.y < 0.0f )
		m_PhysicParam.nowSpeed.y = 0.0f;

	CommonFrameMoveFuture();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] && true == _CanOverheat() )
		{
			StateChange( RVC_DASH_JUMP_COMBO_XX_LOOP );

			//_SetOverheat( true, 5.f );
		}
		else
		{
			StateChange( RVC_DASH_JUMP_COMBO_XX_FINISH );
		}
	}
	else if( true == m_InputData.oneX )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonEventProcess();
}
void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_START_StateEnd()
{
	CommonStateEnd();
}

/// RVC_DASH_JUMP_COMBO_XX_LOOP
void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_LOOP_StateStart()
{
	++m_iRVCComboLoopCount;	/// 발사 카운트 증가

	CommonStateStart();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_LOOP_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.006f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.006f ) && EventCheck( 0.006f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDir = GetRotateDegree();
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		/// 소이탄 발사 각도 설정
		vDir.z -= 42.5f;
		vDir.z += ( ( 0.7f - RandomFloat( 1.5f ) ) * 10.f );		//임의의 각도로 발사

		/// 소이탄 데미지 이펙트
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DASH_JUMP_X_BULLET_LOOP",
											GetPowerRate(), vPos, vDir, vDir, m_FrameDataNow.unitCondition.landPosition.y );

		/// 소이탄 오버 히트 데미지 
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DASH_JUMP_Xloop_BULLET_HIT",
											GetPowerRate(), vPos, vDir, vDir, m_FrameDataNow.unitCondition.landPosition.y );

		if( NULL != pEffect )
		{
			_BurnOverheatHP( pEffect->GetDamageData(), -1.f, 0.5f );

			_SetOverheat( true, 5.f );

			g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( pEffect );
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_LOOP_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_LOOP_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		/// 키 입력을 받았고, 오버 히트 가능한 상태일 때 발동
		if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] && true == _CanOverheat() )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;

			if( m_iRVCComboLoopCount >= _CONST_RAVEN_::MAX_DASH_JUMP_COMBO_XX_LOOP_COUNT )
				StateChange( RVC_DASH_JUMP_COMBO_XX_FINISH );	/// 총 발사수가 9발 이상일 때, Finish로 변경
			else
				StateChange( RVC_DASH_JUMP_COMBO_XX_LOOP );		/// 아니면 Loop로 변경
		}
		else
		{
			StateChange( RVC_DASH_JUMP_COMBO_XX_FINISH );		/// 무조건 종료시에는 Finish 모션으로 한발 발사 한다.
		}
	}
	else if( true == m_InputData.oneX )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	SKILL_CANCEL_AFTER_FLY( m_fSkillCancelAfterFly )

	CommonEventProcess();
}
void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_LOOP_StateEnd()
{
	CommonStateEnd();
}

/// RVC_DASH_JUMP_COMBO_XX_FINISH
void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_FINISH_StateStart()
{
	++m_iRVCComboLoopCount;	/// 발사 카운트 증가

	CommonStateStart();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_FINISH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.025f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.025f ) && EventCheck( 0.025f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDir = GetRotateDegree();
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		/// 소이탄 발사 각도 설정
		vDir.z -= 45.f;

		/// 소이탄 데미지 이펙트
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DASH_JUMP_X_BULLET", GetPowerRate(), vPos, vDir, vDir, m_FrameDataNow.unitCondition.landPosition.y );

		g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( 10.0f, 0.2f );
	}

	CommonFrameMove();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_FINISH_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = -800.0f;
		m_PhysicParam.nowSpeed.y = 1200.0f;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )		/// 애니메이션 종료 후에는 마지막 모션 그대로 떨어진다.
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}

		if( m_InputData.pureDown == true )
		{
			m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
	}
	SKILL_CANCEL_AFTER_FLY( m_fSkillCancelAfterFly )

	CommonEventProcess();
}
void CX2GURaven::RVC_DASH_JUMP_COMBO_XX_FINISH_StateEnd()
{
	m_iRVCComboLoopCount = 0;

	CommonStateEnd();
}

/// RVC_DASH_COMBO_ZZZ
void CX2GURaven::RVC_DASH_COMBO_ZZZ_StateStart()
{
	CommonStateStart();

	SetNoDetonation( true );		/// 기폭 사용 제한
}

void CX2GURaven::RVC_DASH_COMBO_ZZZ_EventProcess()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.501f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.501f ) == true && EventCheck( 0.501f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( true == m_InputData.pureZ )
			StateChange( RVC_DASH_COMBO_ZZZPushZ_READY );

	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GURaven::RVC_DASH_COMBO_ZZZ_StateEnd()
{
	SetNoDetonation( false );

	CommonStateEnd();
}

/// RVC_DASH_COMBO_ZZZ
void CX2GURaven::RVC_DASH_COMBO_ZZZPushZ_READY_StateStart()
{
	CommonStateStart();

	SetNoDetonation( true );		/// 기폭 사용 제한
}

void CX2GURaven::RVC_DASH_COMBO_ZZZPushZ_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RVC_DASH_COMBO_ZZZPushZ );
	}

	CommonEventProcess();
}

void CX2GURaven::RVC_DASH_COMBO_ZZZPushZ_READY_StateEnd()
{
	SetNoDetonation( false );

	CommonStateEnd();
}

/// RVC_DASH_COMBO_ZZZPushZ
void CX2GURaven::RVC_DASH_COMBO_ZZZPushZ_StateStart()
{
	CommonStateStart();

	SetNoDetonation( true );		/// 기폭 사용 제한

	m_iRVCComboLoopCount = 0;		/// 발사 카운트 초기화

	m_bEndFrameThrow	 = false;	/// 10발 발사한 여부 조사

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hEffectFrameThrow != INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL !=m_pEffectFrameThrow )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        g_pX2Game->GetDamageEffect()->DestroyInstanceHandleSilently( m_hEffectFrameThrow );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( m_pEffectFrameThrow );
		m_pEffectFrameThrow = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	}

	if( true == _CanOverheat() )
		_SetOverheat( true, 5.f );	
	else
		m_bEndFrameThrow = true;			/// 화염 방사 종료
}

void CX2GURaven::RVC_DASH_COMBO_ZZZPushZ_FrameMove()
{
	int fCheckTime = static_cast<int>( m_pXSkinAnim->GetNowAnimationTime() / 0.2f );

	if( fCheckTime >= m_iRVCComboLoopCount )		/// 0.2초 마다 발생
	{
		if( 0 == m_iRVCComboLoopCount )				/// 최초 진입시 화염 방사 이펙트 생성
		{
			if( true == m_bEndFrameThrow )
				return;

			D3DXVECTOR3 vDir = GetRotateDegree();
			D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

			if( GetIsRight() == true )
				vPos += 30.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				vPos -= 30.0f * m_FrameDataNow.unitCondition.dirVector;

			/// 화염 방사 데미지 이펙트
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_hEffectFrameThrow == INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( NULL == m_pEffectFrameThrow )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                CX2DamageEffect::CEffect* 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				m_pEffectFrameThrow = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_RVC_FRAME_THROW",
					GetPowerRate(), vPos, vDir, vDir, m_FrameDataNow.unitCondition.landPosition.y );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_hEffectFrameThrow = ( m_pEffectFrameThrow != NULL ) ? m_pEffectFrameThrow->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( m_hEffectFrameThrow != INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( NULL != m_pEffectFrameThrow )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			CX2DamageEffect::CEffect* pCEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_RVC_FRAME_THROW_OVERHEAT_PENALTY",
												 GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );		/// 오버 히트 데미지용 이펙트

			if( NULL != pCEffect )
			{
				_BurnOverheatHP( pCEffect->GetDamageData(), -1.f, 0.5f );		/// 오버 히트 설정

				g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( pCEffect );
			}

			_SetOverheat( true, 5.f );	/// 오버 히트 발동 가능시 오버 히트 버프 유지 시간 갱신
		}

		++m_iRVCComboLoopCount;				/// 발사 카운트 증가
	}

	
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 웨폰 테이커, 베테랑 커맨더의 ZZZ State 의 Loop 를 2배 증가
	if( false == m_bEndFrameThrow && _CONST_RAVEN_::MAX_DASH_COMBO_ZZZPushZ_LOOP_COUNT < m_iRVCComboLoopCount )		
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	if( false == m_bEndFrameThrow && 10 < m_iRVCComboLoopCount )	
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		m_bEndFrameThrow = true;		/// 화염 방사 종료

	CommonFrameMove();
}

void CX2GURaven::RVC_DASH_COMBO_ZZZPushZ_EventProcess()
{
	if( 0.3f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( false == m_InputData.pureZ )						/// z키를 떼면 정지
			StateChange( RVC_DASH_COMBO_ZZZZ_FINISH );
	}

	if( false == _CanOverheat( false ) || true == m_bEndFrameThrow )	/// 사용할 조건이 않될 때 정지
		StateChange( RVC_DASH_COMBO_ZZZZ_FINISH );
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		m_bEndFrameThrow = false;

		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( RVC_DASH_COMBO_ZZZZ_FINISH );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GURaven::RVC_DASH_COMBO_ZZZPushZ_StateEnd()
{
	/// 해당 State 종료시 화염 방사 이펙트 소멸 설정
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hEffectFrameThrow != INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL != m_pEffectFrameThrow && true == g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pEffectFrameThrow ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hEffectFrameThrow );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_pEffectFrameThrow );
        m_pEffectFrameThrow = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		m_bEndFrameThrow	= false;
	}

	m_iRVCComboLoopCount = 0;	/// 발사 카운트 초기화

	SetNoDetonation( false );

	CommonStateEnd();
}

void CX2GURaven::RVC_DASH_COMBO_ZZZZ_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/// 레이븐 2차 전직 베테랑 커맨더 - 데들리 레이드
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_Init()
{
	/// Nothing
}
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_StateStart()
{
	m_iRVCComboLoopCount	  = 1;	/// 발사 카운트 초기화
	m_iRVCComboLoopInputCount = 1;	/// 발사 입력 카운트 초기화
	m_bIsHyperDeadlyRaid	  = false;	/// 현재 데들리 레이드 각성 여부

	m_bShowedOverheat		  = false;
	m_bShowedOverheat1		  = false;
	m_bShowedOverheat2		  = false;

	CommonStateStart();
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_FrameMove()
{
	CommonFrameMove();

	m_bIsHyperDeadlyRaid = IsHyperState();

// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		if( GetIsRight() == true )
			vPos += 100.0f * m_FrameDataNow.unitCondition.dirVector;
		else
			vPos -= 100.0f * m_FrameDataNow.unitCondition.dirVector;

#ifdef VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetVec3ScaleByUnit(), true, vPos);
#else	// VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetScaleByUnit(), true, vPos);
#endif // VERIFY_STAT_BY_BUFF


		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DEADLY_RAID", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
	}
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		++m_iRVCComboLoopInputCount;		/// 스킬 키 입력 시 누적 입력 카운트 증가

		PossibleDeadlyRaidLoop();			/// 오버 히트 이모티콘 표시 연산
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == ChangeDeadlyRaidLoop() )
			StateChange( RVC_SI_SA_DEADLY_RAID_LOOP );				/// 일반 반복 공격
		else
			StateChange( RVC_SI_SA_DEADLY_RAID_LOOP_END );			/// 일반 막타 전 공격
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/// RVC_SI_SA_DEADLY_RAID_LOOP
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_StateStart()
{
	++m_iRVCComboLoopCount;

	CommonStateStart();
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_FrameMove()
{
	CommonFrameMove();
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	if( m_bIsHyperDeadlyRaid == true )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xffff0000 );
	}
	else 
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	}	
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		if( GetIsRight() == true )
			vPos += 100.0f * m_FrameDataNow.unitCondition.dirVector;
		else
			vPos -= 100.0f * m_FrameDataNow.unitCondition.dirVector;

#ifdef VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetVec3ScaleByUnit(), true, vPos);
#else	// VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetScaleByUnit(), true, vPos);
#endif // VERIFY_STAT_BY_BUFF


		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DEADLY_RAID", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
	}
}
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_EventProcess()
{
	if( 0.21 > m_pXSkinAnim->GetNowAnimationTime() )
	{
		if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
			m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
			m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
			m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
			m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
			m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
			m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
			m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
		{
			++m_iRVCComboLoopInputCount;		/// 스킬 키 입력 시 누적 입력 카운트 증가

			PossibleDeadlyRaidLoop();			/// 오버 히트 이모티콘 표시 연산
		}
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( 2 >= m_iRVCComboLoopCount )
		{
			if( true == ChangeDeadlyRaidLoop() )
				StateChange( RVC_SI_SA_DEADLY_RAID_LOOP );				/// 반복 공격
			else
				StateChange( RVC_SI_SA_DEADLY_RAID_LOOP_END );			/// 일반 막타 전 공격
		}
		else
		{
			if( true == ChangeDeadlyRaidLoop() )
				StateChange( RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT );		/// 오버 히트 반복 공격
			else
				StateChange( RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT );	/// 오버 히트 막타 전 공격
		}
	}

	CommonEventProcess();
}

/// RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_StateStart()
{
	++m_iRVCComboLoopCount;

	_SetOverheat( true, 5.f );

	CommonStateStart();
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_FrameMove()
{
	CommonFrameMove();
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	if( m_bIsHyperDeadlyRaid == true )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xffff0000 );
	}
	else 
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	}	
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		if( GetIsRight() == true )
			vPos += 100.0f * m_FrameDataNow.unitCondition.dirVector;
		else
			vPos -= 100.0f * m_FrameDataNow.unitCondition.dirVector;

#ifdef VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetVec3ScaleByUnit(), true, vPos);
#else	// VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetScaleByUnit(), true, vPos);
#endif // VERIFY_STAT_BY_BUFF


		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DEADLY_RAID_LARGE", 
			GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );

		if( NULL != pEffect )
			_BurnOverheatHP( pEffect->GetDamageData(), 0.2f );
	}
}
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_OVERHEAT_EventProcess()
{
	if( 0.21 > m_pXSkinAnim->GetNowAnimationTime() )
	{
		if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
			m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
			m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
			m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
			m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
			m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
			m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
			m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
		{
			++m_iRVCComboLoopInputCount;		/// 스킬 키 입력 시 누적 입력 카운트 증가

			PossibleDeadlyRaidLoop();			/// 오버 히트 이모티콘 표시 연산
		}
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT );		/// 오버 히트 막타 전 공격
	}

	CommonEventProcess();
}

/// RVC_SI_SA_DEADLY_RAID_LOOP_END
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_END_StateStart()
{
	++m_iRVCComboLoopCount;

	CommonStateStart();
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_END_FrameMove()
{
	CommonFrameMove();
// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	if( m_bIsHyperDeadlyRaid == true )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xffff0000 );
	}
	else 
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	}	
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		if( GetIsRight() == true )
			vPos += 100.0f * m_FrameDataNow.unitCondition.dirVector;
		else
			vPos -= 100.0f * m_FrameDataNow.unitCondition.dirVector;

#ifdef VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetVec3ScaleByUnit(), true, vPos);
#else	// VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetScaleByUnit(), true, vPos);
#endif // VERIFY_STAT_BY_BUFF


		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DEADLY_FINISH_READY", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
	}
}
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( 2 >= m_iRVCComboLoopCount )
			StateChange( RVC_SI_SA_DEADLY_RAID_FINISH );				/// 반복 공격
		else
			StateChange( RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT );		/// 오버 히트 막타 전 공격
	}

	CommonEventProcess();
}

/// RVC_SI_SA_DEADLY_RAID_LOOP_END
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_StateStart()
{
	++m_iRVCComboLoopCount;

	_SetOverheat( true, 5.f );

	CommonStateStart();
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_FrameMove()
{
	CommonFrameMove();

// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	if( m_bIsHyperDeadlyRaid == true )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xffff0000 );
	}
	else 
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	}	
// #endif // UPGRADE_SKILL_SYSTEM_2013
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		if( GetIsRight() == true )
			vPos += 100.0f * m_FrameDataNow.unitCondition.dirVector;
		else
			vPos -= 100.0f * m_FrameDataNow.unitCondition.dirVector;

#ifdef VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetVec3ScaleByUnit(), true, vPos);
#else
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetScaleByUnit(), true, vPos);
#endif // VERIFY_STAT_BY_BUFF


		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DEADLY_FINISH_READY_LARGE", 
			GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );

		if( NULL != pEffect )
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 오버 히트 횟수 증가에 따른 소모 hp 값 조절 ( 기존 0.2 ~ 0.4 -> 0.2 고정
			_BurnOverheatHP( pEffect->GetDamageData(), 0.2f );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			if( 4 >= m_iRVCComboLoopCount )
				_BurnOverheatHP( pEffect->GetDamageData(), 0.2f );
			else
				_BurnOverheatHP( pEffect->GetDamageData(), 0.4f );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			
		}
	}
}
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_LOOP_END_OVERHEAT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT );	/// 오버 히트 막타 전 공격
	}

	CommonEventProcess();
}

/// RVC_SI_SA_DEADLY_RAID_FINISH
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_FINISH_StateStart()
{
	++m_iRVCComboLoopCount;

	CommonStateStart();
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_FINISH_FrameMove()
{
	CommonFrameMove();

// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거	
	if( m_bIsHyperDeadlyRaid == true )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xffff0000 );
	}
	else 
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	}	
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		if( GetIsRight() == true )
			vPos += 100.0f * m_FrameDataNow.unitCondition.dirVector;
		else
			vPos -= 100.0f * m_FrameDataNow.unitCondition.dirVector;

#ifdef VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid_Big", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetVec3ScaleByUnit(), true, vPos);
#else	// VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid_Big", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetScaleByUnit(), true, vPos);
#endif // VERIFY_STAT_BY_BUFF


		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DEADLY_RAID_FINISH", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );
	}
}
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/// RVC_SI_SA_DEADLY_RAID_FINISH
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_StateStart()
{
	++m_iRVCComboLoopCount;

	_SetOverheat( true, 5.f );

	CommonStateStart();
}

void CX2GURaven::RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_FrameMove()
{
	CommonFrameMove();

// #ifndef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	// 화면 암전 및 캐릭터 컷인 제거
	if( m_bIsHyperDeadlyRaid == true )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xffff0000 );
	}
	else 
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	}	
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

		if( GetIsRight() == true )
			vPos += 100.0f * m_FrameDataNow.unitCondition.dirVector;
		else
			vPos -= 100.0f * m_FrameDataNow.unitCondition.dirVector;

#ifdef VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid_Big", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetVec3ScaleByUnit(), true, vPos);
#else	// VERIFY_STAT_BY_BUFF
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Deadly_Raid_Big", (CX2GameUnit*)this, (CX2GameUnit*)this, false, -1.f, -1.f, GetScaleByUnit(), true, vPos);
#endif // VERIFY_STAT_BY_BUFF


		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_DEADLY_RAID_FINISH_LARGE", 
			GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree() );

		if( NULL != pEffect )
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 오버 히트 횟수 증가에 따른 소모 hp 값 조절 ( 기존 0.2 ~ 0.6 -> 0.2 고정
			_BurnOverheatHP( pEffect->GetDamageData(), 0.2f );
#else  // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			if( 2 >= m_iRVCComboLoopCount )
				_BurnOverheatHP( pEffect->GetDamageData(), 0.2f );
			if( 4 >= m_iRVCComboLoopCount )
				_BurnOverheatHP( pEffect->GetDamageData(), 0.4f );
			else
				_BurnOverheatHP( pEffect->GetDamageData(), 0.6f );

#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		}
	}
}
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_FINISH_OVERHEAT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RVC_SI_SA_DEADLY_RAID_END );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/// RVC_SI_SA_DEADLY_RAID_FINISH
void CX2GURaven::RVC_SI_SA_DEADLY_RAID_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_END );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

		CommonEventProcess();
}

/// 데들리 레이드 반복 State 변경 가능 여부 반환
bool CX2GURaven::PossibleDeadlyRaidLoop()
{
	if( 1 >= m_iRVCComboLoopCount )
	{
		if( _CONST_RAVEN_::DEADLY_RAID_OVERHEAT_INPUT_NUM_1 <= m_iRVCComboLoopInputCount && true == _CanOverheat() )	//누적 입력수 및 오버 히트 가능 여부 검사
		{
			if( false == m_bShowedOverheat )
			{
				_ShowOverheatIcon( 1 );
				m_bShowedOverheat = true;

				return true;
			}
		}
	}

	if( 2 >= m_iRVCComboLoopCount )
	{
		if( _CONST_RAVEN_::DEADLY_RAID_OVERHEAT_INPUT_NUM_2 <= m_iRVCComboLoopInputCount && true == _CanOverheat() )	//누적 입력수 및 오버 히트 가능 여부 검사
		{
			if( false == m_bShowedOverheat1 )
			{
				_ShowOverheatIcon( 2 );
				m_bShowedOverheat1 = true;

				return true;
			}
		}
	}

	if( 3 >= m_iRVCComboLoopCount )
	{
		if( _CONST_RAVEN_::DEADLY_RAID_OVERHEAT_INPUT_NUM_3 <= m_iRVCComboLoopInputCount && true == _CanOverheat() )	//누적 입력수 및 오버 히트 가능 여부 검사
		{
			if( false == m_bShowedOverheat2 )
			{
				_ShowOverheatIcon( 3 );
				m_bShowedOverheat2 = true;

				return true;
			}
		}
	}

	return false;
}

bool CX2GURaven::ChangeDeadlyRaidLoop()
{
	if( 1 == m_iRVCComboLoopCount && true == m_bShowedOverheat )
	{
		return true;
	}

	if( 2 == m_iRVCComboLoopCount && true == m_bShowedOverheat1 )
	{
		return true;

	}

	if( 3 == m_iRVCComboLoopCount && true == m_bShowedOverheat2 )
	{
		return true;

	}

	return false;
}

/// 레이븐 2차 전직 베테랑 커맨더 - 이그니션 크로우
void CX2GURaven::RVC_SI_SA_IGNITION_CROW_Init()
{
	
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_StateStart()
{
	m_iRVCComboLoopInputCount  = 0;
	m_bShowedOverheat		   = false;

	CommonStateStart();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );
	CommonFrameMove();
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_EventProcess()
{
	if( 2.01f > m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
			m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
			m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
			m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
			m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
			m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
			m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
			m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
		{
			++m_iRVCComboLoopInputCount;		/// 스킬 키 입력 시 누적 입력 카운트 증가

			if( _CONST_RAVEN_::OVERHEAT_KEY_PRESS_TIME <= m_iRVCComboLoopInputCount && true == _CanOverheat() )
			{
				if( false == m_bShowedOverheat )
					_ShowOverheatIcon();
				m_bShowedOverheat = true;
			}
		}
	}


	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == m_bShowedOverheat )
			StateChange( RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT );
		else
			StateChange( RVC_SI_SA_IGNITION_CROW_FIRE );
	}

	CommonEventProcess();
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_StateEnd()
{
	CommonStateEnd();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_FIRE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_IGNITION_CROW", 
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_FIRE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_StateStart()
{
	_SetOverheat( true, 5.f );

	CommonStateStart();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_IGNITION_CROW_OVERHEAT", 
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		/// 오버 히트 데미지 처리
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_IGNITION_CROW_OVERHEAT_PENALTY",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );

		if( NULL != pEffect )
		{
			_BurnOverheatHP( pEffect->GetDamageData() );

			g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( pEffect );
		}
	}

	CommonFrameMove();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_FIRE_OVERHEAT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
			StateChange( RVC_SI_SA_IGNITION_CROW_OVERHEAT_END );
	}

	CommonEventProcess();
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_OVERHEAT_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{		
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/// 레이븐 2차 전직 베테랑 커맨더 - 섀도우 백 슬라이드
void CX2GURaven::RVC_SI_P_SHADOW_BACK_SLIDE_StartFuture()
{
	m_fShadowBackSlideTimeLeft = 0.f;


	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RF_SHADOW_STEP );
	if( NULL != pSkillTemplet )
	{
		//ASSERT( m_PhysicParam.passiveSpeed.x > 0.f );
		if( 0.f != m_PhysicParam.passiveSpeed.x )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			int iShadowBackSlideRnage = static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS, iSkillTempletLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
			int iShadowBackSlideRnage = static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS ) );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			
			m_fShadowBackSlideTimeLeft = iShadowBackSlideRnage * 100.f / ( m_PhysicParam.passiveSpeed.x );
		}
	}

	CommonStateStartFuture();
}

void CX2GURaven::RVC_SI_P_SHADOW_BACK_SLIDE_Start()
{
	SetAlphaObject( true );
	m_GageManager.SetAlpha( 0.f );

	m_RenderParam.bAlphaBlend		= true;
	m_RenderParam.color.a			= 0.1f;

	if ( m_pAfterImage != NULL )
		m_pAfterImage->SetAfterImageColor( 0xffcccccc );

	m_bActiveShadowStepCombo = true;	/// 섀도우 스텝 사용중 여부
	m_bIsShadowStepIcon		 = false;	/// 섀도우 스텝 아이콘으로 설정

	++m_iRVCComboLoopCount;

	CommonStateStart();
}

void CX2GURaven::RVC_SI_P_SHADOW_BACK_SLIDE_FrameMove()
{
	if( m_FrameDataNow.syncData.bFrameStop == false )
	{
		if( m_fShadowBackSlideTimeLeft > 0.f )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_fShadowBackSlideTimeLeft -= GetElapsedTime();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_fShadowBackSlideTimeLeft -= g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( m_fShadowBackSlideTimeLeft < 0.f )
				m_fShadowBackSlideTimeLeft = 0.f;
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RVC_SI_P_SHADOW_BACK_SLIDE_EventProcess()
{
	if( m_fShadowBackSlideTimeLeft <= 0.f )
	{
		InitShadowStep();

		m_bActiveShadowStepCombo = false;

		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( RVC_SI_P_SHADOW_BACK_SLIDE_END );
		}
	}
	else if( SpecialAttackEventProcess() == true )
	{
		m_bActiveShadowStepCombo = false;

		StateChange( RVC_SI_P_SHADOW_STEP_FINISH );
	}

	else if( m_InputData.pureZ == true )
	{
		m_bActiveShadowStepCombo = false;

		SetShadowStepAttackSkill();
	}

	else if( m_InputData.pureX == true )
	{
		m_bActiveShadowStepCombo = false;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		// 쉐도우 스탭 백 슬라이드 중..
		// 혹시 모르니 다른 클래스의 쉐도우 스탭 파생기도 추가 해뒀다.
		if ( true == m_bEnableShadowStepPunisher )
		{
			SetShadowStepAttackSkill( RSI_P_RRF_SHADOW_PUNISHER );
		}
		if ( true == m_bEnableShadowStepThrustEnhance )
		{
			SetShadowStepAttackSkill( RSI_SHADOW_STEP_THRUST_ENHANCE );
		}
		else
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		{
			SetShadowStepAttackSkill( RVC_SI_P_SHADOW_BUSTER );
		}
	}
	
	CommonEventProcess();
}

void CX2GURaven::RVC_SI_P_SHADOW_BACK_SLIDE_End()
{
	SetAlphaObject( false );
	m_GageManager.SetAlpha( 255.f ); 

	m_RenderParam.bAlphaBlend		= false;
	m_RenderParam.color.a			= 1.f;

	if( NULL != m_pAfterImage )
	{
		m_pAfterImage->Disable();
	}

	m_fShadowBackSlideTimeLeft = 0.f;

	if( true == m_bActiveShadowStepCombo ) /// State 변경이 일어나지 않았다면, 아이콘 초기화 한다.
		InitShadowStep();

	CommonStateEnd();
}

/// 레이븐 2차 전직 베테랑 커맨더 - 섀도우 백 슬라이드 종료 모션
void CX2GURaven::RVC_SI_P_SHADOW_BACK_SLIDE_END_FrameMoveFuture()
{
	//m_PhysicParam.nowSpeed.x = -250.f;

	CommonFrameMoveFuture();
}

void CX2GURaven::RVC_SI_P_SHADOW_BACK_SLIDE_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

/// 레이븐 2차 전직 베테랑 커맨더 - 섀도우 스텝 피니시
void CX2GURaven::RVC_SI_P_SHADOW_STEP_FINISH_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Shadow_Step_Mesh01.X" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"Raven_SI_A_RF_Shadow_Step_Mesh01.X" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}

void CX2GURaven::RVC_SI_P_SHADOW_STEP_FINISH_StartFuture()
{
	CommonStateStartFuture();

	m_fShadowStepFinishTimeLeft = 0.f;



	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RF_SHADOW_STEP );
	if( NULL != pSkillTemplet )
	{
		ASSERT( m_PhysicParam.passiveSpeed.x > 0.f );
		if( 0.f != m_PhysicParam.passiveSpeed.x )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit()  )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			m_fShadowStepFinishTimeLeft = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS, iSkillTempletLevel ) * 
											100.f / m_PhysicParam.passiveSpeed.x;
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fShadowStepFinishTimeLeft = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS ) * 100.f / m_PhysicParam.passiveSpeed.x;
	#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_NOTE			
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO6 ) == true )
			{
				m_PhysicParam.passiveSpeed.x *= 1.3f;				
			}
#endif
		}

	}
}

void CX2GURaven::RVC_SI_P_SHADOW_STEP_FINISH_Start()
{
	CommonStateStart();

	SetAlphaObject( true );
	m_GageManager.SetAlpha( 0.f );

	m_RenderParam.bAlphaBlend		= true;
	m_RenderParam.color.a			= 0.1f;

	if ( m_pAfterImage != NULL )
		m_pAfterImage->SetAfterImageColor( 0xffcccccc );

		m_bActiveShadowStepCombo = true;	/// 섀도우 스텝 사용중 여부

		++m_iRVCComboLoopCount;
}

void CX2GURaven::RVC_SI_P_SHADOW_STEP_FINISH_FrameMove()
{
	if( m_FrameDataNow.syncData.bFrameStop == false )
	{
		if( m_fShadowStepFinishTimeLeft > 0.f )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_fShadowStepFinishTimeLeft -= GetElapsedTime();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_fShadowStepFinishTimeLeft -= g_pKTDXApp->GetElapsedTime();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( m_fShadowStepFinishTimeLeft < 0.f )
				m_fShadowStepFinishTimeLeft = 0.f;
		}
	}

	CommonFrameMove();
}

void CX2GURaven::RVC_SI_P_SHADOW_STEP_FINISH_EventProcess()
{
	if( m_fShadowStepFinishTimeLeft <= 0.f )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChangeDashIfPossible();
		}
	}
	else if( true == m_InputData.pureZ )
	{
		SetShadowStepAttackSkill();
	}

	else if( true == m_InputData.pureX )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		// 쉐도우 스탭 파생기		
		if ( true == m_bEnableShadowStepPunisher )
		{
			SetShadowStepAttackSkill( RSI_P_RRF_SHADOW_PUNISHER );
		}
		else if ( true == m_bEnableShadowStepThrustEnhance )
		{
			SetShadowStepAttackSkill( RSI_SHADOW_STEP_THRUST_ENHANCE );
		}
		else
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		{
			SetShadowStepAttackSkill( RVC_SI_P_SHADOW_BUSTER );
		}
	}
	CommonEventProcess();
}

void CX2GURaven::RVC_SI_P_SHADOW_STEP_FINISH_End()
{
	CommonStateEnd();

	SetAlphaObject( false );
	m_GageManager.SetAlpha( 255.f ); 

	m_RenderParam.bAlphaBlend		= false;
	m_RenderParam.color.a			= 1.f;

	if( NULL != m_pAfterImage )
	{
		m_pAfterImage->Disable();
	}

	m_fShadowStepFinishTimeLeft = 0.f;

	InitShadowStep();
}

/// 레이븐 2차 전직 베테랑 커맨더 - 섀도우 버스터
void CX2GURaven::RVC_SI_P_SHADOW_BUSTER_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Thrust_Mesh01.X" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"Raven_SI_A_RF_Thrust_Mesh01.X" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}



void CX2GURaven::RVC_SI_P_SHADOW_BUSTER_Start()
{
	CommonStateStart();

	m_NowStateData.m_fPowerRate = m_fShadowBusterPowerRate;
}

void CX2GURaven::RVC_SI_P_SHADOW_BUSTER_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		/// 네이팜탄 생성
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Raven_Shadow_Buster", (CX2GameUnit*)this, (CX2GameUnit*)this );
	}

	CommonFrameMove();
}

void CX2GURaven::RVC_SI_P_SHADOW_BUSTER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GURaven::SetShadowStepAttackSkill( RAVEN_STATE_ID eStateID /*= RSI_SHADOW_STEP_THRUST*/ )
{
	if( m_FrameDataFuture.unitCondition.fStateTime > 0.2f )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		if ( true == m_bEnableShadowStepPunisher && RSI_P_RRF_SHADOW_PUNISHER == eStateID )
		{
			if( true == FlushMp( 10.f ) )
				StateChange( eStateID );
			else
			{
#ifdef ALWAYS_SCREEN_SHOT_TEST
				if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
				{
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				}
#else
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif ALWAYS_SCREEN_SHOT_TEST
			}
		}

		else if ( true == m_bEnableShadowStepThrustEnhance && RSI_SHADOW_STEP_THRUST_ENHANCE == eStateID)
		{
			if( true == FlushMp( 10.f ) )
				StateChange( eStateID );
			else	
			{
#ifdef ALWAYS_SCREEN_SHOT_TEST
				if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
				{
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				}
#else
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif ALWAYS_SCREEN_SHOT_TEST
			}
		}
		else
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
		{
			if( true == m_bEnableShadowStepThrust )	/// 섀도우 피어싱을 배움
			{
	#ifdef BALANCE_BLADE_MASTER_20130117			/// 섀도우 피어싱은 MP를 소모하지 않음
				if( true == m_bEnableShadowBackSlide && RSI_SHADOW_STEP_THRUST != eStateID )	/// 섀도우 백 슬라이드를 배웠고, 섀도우 피어싱 사용중이 아닐 때
				{
					if( true == FlushMp( 10.f ) )
						StateChange( eStateID );
					else
					{
#ifdef ALWAYS_SCREEN_SHOT_TEST
						if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
						{
							g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
						}
#else
						g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif ALWAYS_SCREEN_SHOT_TEST
					}
				}
				else
				{
					StateChange( RSI_SHADOW_STEP_THRUST );
				}
	#else  BALANCE_BLADE_MASTER_20130117
				if( true == FlushMp( 10.f ) )
				{
					if( true == m_bEnableShadowBackSlide )
						StateChange( eStateID );
					else
						StateChange( RSI_SHADOW_STEP_THRUST );
				}
				else
				{
#ifdef ALWAYS_SCREEN_SHOT_TEST
					if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false)
					{
						g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
					}
#else
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
#endif ALWAYS_SCREEN_SHOT_TEST
				}
	#endif BALANCE_BLADE_MASTER_20130117
			}
		}
		InitShadowStep();		/// 섀도우 스텝 쿨타임 적용 및 초기화
	}
}

void CX2GURaven::InitShadowStep()
{
	if( 0.f < m_fShadowStepCooTime )
	{
		m_fShadowStepCooTime -= m_iShadowBackSlideLevel;		/// 섀도우 백 슬라이드 습득시 쿨타임 1초 감소
		CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;

		accessUserSkillTree.SetSkillCoolTimeLeft( CX2SkillTree::SI_A_RF_SHADOW_STEP, m_fShadowStepCooTime );
	}

	m_fShadowStepCooTime	 = 0.f;			/// 쿨타임 초기화
	m_iRVCComboLoopCount	 = 0;			/// 섀도우 스텝 동작 횟수
	m_bIsShadowStepIcon		 = true;		/// 섀도우 스텝 아이콘으로 설정
	m_bActiveShadowStepCombo = false;		/// 섀도우 스텝 계열 동작 종료

	if ( IsMyUnit() )
		g_pX2Game->UpdateSkillSlotUI();		/// 아이콘 변경
}

/// RVC_SI_A_BLEEDING_SLICE
void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_Init()
{

}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_StateStart()
{
	CommonStateStart();
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( RVC_SI_A_BLEEDING_SLICER_FLY );
	}
	else
	{
		StateChange( RVC_SI_A_BLEEDING_SLICER_GROUND );
	}

	CommonEventProcess();
}

/// RVC_SI_A_BLEEDING_SLICER_GROUND
void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_GROUND_StateStart()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RVC_BLEEDING_SLICER );
	if ( NULL != pSkillTemplet && pSkillTemplet->m_vecBuffFactorPtr.size() >= 1 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( NULL == GetUnit()  )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
		
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	// 버스팅블레이드가 이미 존재하면 삭제
// 	if( INVALID_MESH_INSTANCE_HANDLE != m_hBurstingBladeAttackBox )
// 	{
// 		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hBurstingBladeAttackBox );
// 	}
// 
// 	// 이후 생성
// 	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"BleedingSlicerAttackBox_Ground", 0,0,0, 0,0,0, 0,0,0 );
// 	if( pMeshInst != NULL )
// 	{
//#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
//        pMeshInst->SetPerFrameSimulation( true );
//#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
// 		m_hBurstingBladeAttackBox = pMeshInst->GetHandle();
// 		pMeshInst->SetBoundingRadius( 0.0f );
// 
// 		CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
// 
// 		pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
// 		pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );
// 
// 		SetSubAttackList( pMeshInst->GetXSkinAnim() );
// 	}
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_GROUND_FrameMove()
{
// 	if( m_pXSkinAnim->EventTimer( 0.23f ) == true && EventCheck( 0.23f, false ) == true )
// 	{
// 		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
// 
// 		if( GetIsRight() == true )
// 			vPos.x -= 30.f;
// 		else
// 			vPos.x += 30.f;
// 
// 		CKTDGParticleSystem::CParticleEventSequence* pSeq;
// 		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"RVC_A_bleedingSlicer_P01", vPos );
// 
// 		if( NULL != pSeq )
// 		{
// 			pSeq->SetAxisAngle( GetRotateDegree() );
// 			pSeq->SetAddRotate( GetRotateDegree() );
// 		}
// 	}
// 
// 	if( m_hBurstingBladeAttackBox != INVALID_MESH_INSTANCE_HANDLE )
// 	{
// 		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hBurstingBladeAttackBox );
// 		if( NULL != pMeshInst )
// 		{
// 			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
// 			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
// 		}
// 		else
// 		{
// 			m_hBurstingBladeAttackBox = INVALID_MESH_INSTANCE_HANDLE;
// 		}
// 	}

	CommonFrameMove();
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_GROUND_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_GROUND_StateEnd()
{
// 	m_SubAttackListSet.clear();
// 
// 	// 버스팅블레이드 삭제
// 	if( INVALID_MESH_INSTANCE_HANDLE != m_hBurstingBladeAttackBox )
// 	{
// 		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hBurstingBladeAttackBox );
// 	}

	CommonStateEnd();
}

/// RVC_SI_A_BLEEDING_SLICER_FLY
void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_AIR_StateStart()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RVC_BLEEDING_SLICER );
	if ( NULL != pSkillTemplet && pSkillTemplet->m_vecBuffFactorPtr.size() >= 1 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	
		if ( NULL == GetUnit()  )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	//버스팅블레이드가 이미 존재하면 삭제
// 	if( INVALID_MESH_INSTANCE_HANDLE != m_hBurstingBladeAttackBox )
// 	{
// 		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hBurstingBladeAttackBox );
// 	}
// 	
// 	// 이후 생성
// 	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"BleedingSlicerAttackBox_Air", 0,0,0, 0,0,0, 0,0,0 );
// 	if( pMeshInst != NULL )
// 	{
//#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        //pMeshInst->SetPerFrameSimulation( true );
//#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
// 		m_hBurstingBladeAttackBox = pMeshInst->GetHandle();
// 		pMeshInst->SetBoundingRadius( 0.0f );
// 	
// 		CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
// 		pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
// 		pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );
// 	
// 		SetSubAttackList( pMeshInst->GetXSkinAnim() );
// 	}
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_AIR_FrameMove()
{
// 	if( m_pXSkinAnim->EventTimer( 0.23f ) == true && EventCheck( 0.23f, false ) == true )
// 	{
// 		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
// 
// 		if( GetIsRight() == true )
// 			vPos.x -= 30.f;
// 		else
// 			vPos.x += 30.f;
// 
// 		CKTDGParticleSystem::CParticleEventSequence* pSeq;
// 		pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"RVC_A_bleedingSlicer_Air_P01", vPos );
// 
// 		if( NULL != pSeq )
// 		{
// 			pSeq->SetAxisAngle( GetRotateDegree() );
// 			pSeq->SetAddRotate( GetRotateDegree() );
// 		}
// 	}
// 
// 	if( m_hBurstingBladeAttackBox != INVALID_MESH_INSTANCE_HANDLE )
// 	{
// 		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hBurstingBladeAttackBox );
// 		if( NULL != pMeshInst )
// 		{
// 			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
// 			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
// 		}
// 		else
// 		{
// 			m_hBurstingBladeAttackBox = INVALID_MESH_INSTANCE_HANDLE;
// 		}
// 	}

	CommonFrameMove();
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_AIR_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}

		if( m_InputData.pureDown == true )
		{
			m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		}
	}

	CommonFrameMoveFuture();
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_AIR_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER_FLY( m_fSkillCancelAfterFly )

	CommonEventProcess();
}

void CX2GURaven::RVC_SI_A_BLEEDING_SLICER_AIR_StateEnd()
{
	m_SubAttackListSet.clear();

	// 버스팅블레이드 삭제
	if( INVALID_MESH_INSTANCE_HANDLE != m_hBurstingBladeAttackBox )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hBurstingBladeAttackBox );
	}

	CommonStateEnd();
}

void CX2GURaven::CreateOverHeatObject()
{
	if( CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnitClass() )		/// 베테랑 커맨더만 구조물 생성
	{
		if( INVALID_MESH_INSTANCE_HANDLE != m_hOverHeatObject )		/// 이미 생성되어 있다면 중복 생성하지 않는다.
			return;

		/// 오버 히트 구조물 생성
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this, L"RVC_OverHeat_Object_Start", GetPos(), GetRotateDegree(), GetRotateDegree() );

		if( pMeshInst != NULL )
		{
			m_hOverHeatObject = pMeshInst->GetHandle();
			pMeshInst->SetBoundingRadius( 0.0f );

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Forearm" );
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
			pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );

			PlaySound( L"WT_XXXXX_3.ogg" );
		}
	}

	/// 오버 히트 이펙트 생성 ( 팔목, 팔뚝 두곳에서 생성 )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( INVALID_DAMAGE_EFFECT_HANDLE == m_hEffectOverHeatFire1 )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL == m_pEffectOverHeatFire1 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Forearm" );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect*
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pEffectOverHeatFire1 = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_OVERHEAT_EFFECT",
			GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hEffectOverHeatFire1 = ( m_pEffectOverHeatFire1 != NULL ) ? m_pEffectOverHeatFire1->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( INVALID_DAMAGE_EFFECT_HANDLE == m_hEffectOverHeatFire2 )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL == m_pEffectOverHeatFire2 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		D3DXVECTOR3 vPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect*
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pEffectOverHeatFire2 = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_OVERHEAT_EFFECT",
			GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hEffectOverHeatFire2 = ( m_pEffectOverHeatFire2 != NULL ) ? m_pEffectOverHeatFire2->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}
}
void CX2GURaven::DeleteOverHeatObject()
{
	if( CX2Unit::UC_RAVEN_VETERAN_COMMANDER == GetUnitClass() )		/// 베테랑 커맨더만 구조물 생성
	{
		if( INVALID_MESH_INSTANCE_HANDLE != m_hOverHeatObject )
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hOverHeatObject );

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this, L"RVC_OverHeat_Object_End", GetPos(), GetRotateDegree(), GetRotateDegree() );

		if( pMeshInst != NULL )
		{
			m_hOverHeatObject = pMeshInst->GetHandle();
			pMeshInst->SetBoundingRadius( 0.0f );

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Forearm" );
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
			pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );

			PlaySound( L"WT_ZZZXX_2.ogg" );
		}
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( INVALID_DAMAGE_EFFECT_HANDLE != m_hEffectOverHeatFire1 )
		g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hEffectOverHeatFire1 );
	if( INVALID_DAMAGE_EFFECT_HANDLE != m_hEffectOverHeatFire2 )
		g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hEffectOverHeatFire2 );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL != m_pEffectOverHeatFire1 )
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_pEffectOverHeatFire1 );
	if( NULL != m_pEffectOverHeatFire2 )
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_pEffectOverHeatFire2 );
    m_pEffectOverHeatFire1 = NULL;
    m_pEffectOverHeatFire2 = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}
#endif SERV_RAVEN_VETERAN_COMMANDER

void CX2GURaven::ShowActiveSkillCutInAndLight( const WCHAR* szBoneName_, const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ /*= false */ )
{	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( fTimeToShow_ ) && true == EventCheck( fTimeToShow_, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( GetShowCutInAndChangeWorldColor() && GetShowActiveSkillShow() )
#ifdef SERV_APRIL_FOOLS_DAY
			( true == m_bIsFoolsDay )?
			ActiveSkillShow( szBoneName_, s_SkillCutInSetFoolsDay.fileName.c_str(), s_SkillCutInSetFoolsDay.vSize, s_SkillCutInSetFoolsDay.vPosition, IsHyperState(), bOnlyLight_ ) :
#endif //APRIL_FOOLS_DAY
			ActiveSkillShow( szBoneName_, s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].fileName.c_str(), s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].vSize, s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].vPosition, IsHyperState(), bOnlyLight_ );
	}
}

/** @function : CheckAndDoReversalSoldier
	@brief : 레클리스피스트 역전의 용사 패시브 처리
*/
void CX2GURaven::CheckAndDoReversalSoldier()
{
	const float fMaxHp = GetMaxHp();

	if ( (m_fOldHp / fMaxHp) > 0.5f &&		/// HP가 50% 이하로 내려갔을때!
		 (GetNowHp() / fMaxHp) < 0.5f )
	{
		CX2Unit* pUnit = GetUnit();
		if ( NULL != pUnit )
		{
			const CX2UserSkillTree& cUserSkillTree = pUnit->GetUnitData().m_UserSkillTree;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			const int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RBM_REVERSAL_SOLDIER, true );

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RBM_REVERSAL_SOLDIER );
				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					SetBuffFactorToGameUnit( pSkillTemplet, 0 );
			}
#else //UPGRADE_SKILL_SYSTEM_2013
			const int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_REVERSAL_SOLDIER );

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_REVERSAL_SOLDIER, iSkillLevel );
				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					SetBuffFactorToGameUnit( pSkillTemplet, 0 );
			}
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
	}
	m_fOldHp = GetNowHp();
}


/** @function : CheckAndDoServivalTechniqueOfMercenary
	@brief : 베테랑 커맨더 용병의 생존 전략
*/
void CX2GURaven::CheckAndDoServivalTechniqueOfMercenary()
{
	const float fMaxHp = GetMaxHp();

	/// kimh // 필드간 이동, 필드<->던전 이동간에 2번 적용될 것으로 예상 되나
	/// 버프가 교체의 속성을 띠고 있으므로 문제 없음
	if ( (m_fOldHp / fMaxHp) > 0.5f &&		/// HP가 50% 이하로 내려갔을때!
		(GetNowHp() / fMaxHp) < 0.5f )
	{
		CX2Unit* pUnit = GetUnit();
		if ( NULL != pUnit )
		{
			const CX2UserSkillTree& cUserSkillTree = pUnit->GetUnitData().m_UserSkillTree;
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
			const int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			
			if( iSkillLevel > 0 )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY );
	#else // UPGRADE_SKILL_SYSTEM_2013
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY, iSkillLevel );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					SetBuffFactorToGameUnit( pSkillTemplet, 0 );
			}
		}
	}
	m_fOldHp = GetNowHp();
}

/** @function : ApplyHyperModeBuff
	@brief : 각성버프 적용
*/
/*virtual*/ void CX2GURaven::ApplyHyperModeBuff()
{
	CX2GameUnit::ApplyHyperModeBuff();

	if ( IsFullHyperMode() )	/// 풀각성 시작시 나오는 사운드
		PlaySound(L"Raven_ElectricAura_Appear.ogg");
}

/** @function : HyperModeBuffEffectStart
	@brief : 각성 시 나와야하는 이펙트들 시작
*/
/*virtual*/ void CX2GURaven::HyperModeBuffEffectStart()
{
	CX2GUUser::HyperModeBuffEffectStart();

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
		m_hHyperBoostRFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightBlack",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
		m_hHyperBoostLFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftBlack",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
		m_hHyperBoostRArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightBlack",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
		m_hHyperBoostLArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftBlack",	0,0,0, 0, 0 );

	if ( IsFullHyperMode() )
	{
		if( INVALID_EFFECTSET_HANDLE == m_hHyperAuraEffectset )
			m_hHyperAuraEffectset = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RAVEN_HYPER_AURA", (CX2GameUnit*)this, (CX2GameUnit*)this );
	}

	/// kimhc // 클라이언트간에 m_fRageGauge 수치 싱크는 맞지 않으나, 나소드볼 수치는 p2p로 맞추고 있으므로 괜찮을 듯.
	m_fRageGauge = min( m_fRageGauge + 40.0f, 100.0f );	
}

/** @function : HyperModeBuffEffectEnd
	@brief : 각성 종료시 각성이펙트 해제를 위해 수행되는 함수
*/
/*virtual*/ void CX2GURaven::HyperModeBuffEffectEnd()
{
	CX2GUUser::HyperModeBuffEffectEnd();

	/// 풀각성일 때 해제해야 할 것들
	if ( IsFullHyperMode() )
	{
		if( m_hHyperAuraEffectset != INVALID_EFFECTSET_HANDLE )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hHyperAuraEffectset );
			PlaySound(L"Raven_ElectricAura_Disappear.ogg");
		}
		SetFullHyperMode( false );
	}

	/// 남은 RageGage가 있으면 HP와 MP를 증가시킨다
	if( IsMyUnit() == true && GetNowHp() > 0.f && m_fRageGauge > 0.f )
	{			
		UpNowHp( GetMaxHp() * (m_fRageGauge * 0.0005f) );
		UpNowMp( m_fRageGauge );
	}

	if( m_iNaosdBallLv > 0 )
		ClearNasodBall();

	m_fRageGauge = 0.f;
	m_fParryingTime = 0.f;
	m_bCanParrying = false;
	
}

/** @function : CX2GetActualMPConsume
	@brief : 스킬의 MP 소모량을 얻어옴
	@param : 스킬의 ID, 사용한 스킬의 레벨
	@return : 소모될 MP 수치
*/
/*virtual*/ float CX2GURaven::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );

	switch ( eSkillID_ )
	{
	case CX2SkillTree::SI_SA_RF_POWER_ASSAULT:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO8 ) == true )
				fMpConsumption -= 20.f;
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_ORDNANCE_PENETRATOR:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO15 ) == true )
				fMpConsumption -= 50.f;
		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈
	case CX2SkillTree::SI_A_RF_BURNING_RUSH:	//  버닝 러시 마나 감소
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO5 ) == true )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 메모 효과 변경 -> 메모 착용 시, 마나 소모 절반 감소 및 쿨타임 2배 증가
				fMpConsumption = fMpConsumption / 2.f;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				fMpConsumption -= 10.f;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				
		} break;
#endif // UPGRADE_SKILL_SYSTEM_2013 // Raven 스킬 개편, 김종훈	

#ifdef ADD_MEMO_1ST_CLASS //김창한
	case CX2SkillTree::SI_A_RST_BERSERKER_BLADE:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO9 ) == true )
				fMpConsumption *= 1.1f;
		} break;

	case CX2SkillTree::SI_SA_RWT_REVOLVER_CANNON_HIGH_EXPLOSIVE_SHELL:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO28 ) == true )
				fMpConsumption *= 1.1f;
		} break;
#endif //ADD_MEMO_1ST_CLASS

	default:
		break;
	}

	if ( 0.0f > fMpConsumption )
		fMpConsumption = 0.f;

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT	
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillID_ );
	float fMpDecreaseRate = 1.0f;
	if( NULL != pSkillTemplet )
		fMpDecreaseRate  =  g_pData->GetMyUser()->GetSelectUnit()->GetSkillMpDecreaseRate(eSkillID_, pSkillTemplet->m_eType);

	return fMpConsumption * fMpDecreaseRate;
#else SERV_BALANCE_FINALITY_SKILL_EVENT
	return fMpConsumption;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
}

/** @function : AddUnitSlashData
	@brief : 유닛에 트레이스 붙이기 (실행은 아님)
	@param : 트레이스 종류(eSlashTraceCondition_)
*/
/*virtual*/ void CX2GURaven::AddUnitSlashData( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ )
{
#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
	if( NULL == m_pUnitSlashTraceManager )
		m_pUnitSlashTraceManager = CX2UnitSlashTraceManager::CreateUnitSlashTraceManager( this, GetXSkinAnimPtr() );

	if( NULL != m_pUnitSlashTraceManager )
	{
		const int vertexNum = (IsMyUnit() ? 400 : 150);

		switch ( eSlashTraceCondition_ )
		{
		case CX2UnitSlashTraceManager::STC_NONE:
			{
				m_pUnitSlashTraceManager->AddSlashTraceData( L"ATTACK_SPHERE1_Lhand1", L"Bip01_L_Hand", 
					D3DXCOLOR(0xcc5555ff), D3DXCOLOR(0xccff3333), D3DXCOLOR(0xff5555ff), D3DXCOLOR(0xffff3333),
					vertexNum, 0.4f, 1, 
					vertexNum, 0.5f, 3, 
					false, 0.f, L"" );
			} break;

		default:
			break;
		}
	}
#endif UNIT_SLASH_TRACE_MANAGER_TEST
}

/** @function : SetSpecificValueByBuffTempletID
	@brief : 각 유닛마다 특정 버프가 실행 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 설정 등...)
	@param : 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GURaven::SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_BLOODY_WEAPON:
		{
			Weapon* pWeapon = m_vecpWeapon[0];
			if ( NULL != pWeapon )
				pWeapon->SetTraceColor(true, 0xccff0000, 0xffff0000);
		} break;

	default:
		break;
	}
}

/** @function : UnSetSpecificValueByBuffTempletID
	@brief : 각 유닛마다 특정 버프가 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 해제 됬다는 플래그 설정 등...)
	@param : 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GURaven::UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_BLOODY_WEAPON:
		{
			Weapon* pWeapon = m_vecpWeapon[0];
			if ( NULL != pWeapon )
				pWeapon->SetTraceColor(false, 0xcc5555ff, 0xcc5555ff);
		} break;

	default:
		break;
	}
}

/** @function : ApplyRenderParam
	@brief : 버프에 의한 renderparam과 함께 추가적으로 해비스텐스에 대한 renderparam을 설정하는 함수
	@param : SkinAnim의 RenderParam 포인터(pRenderParam_)

*/
/*virtual*/ void CX2GURaven::ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
#ifdef RIDING_SYSTEM
	if ( m_bPassDash )
		return;
#endif //RIDING_SYSTEM

	if( m_fShadowStepTimeLeft > 0.f )
		return;
#ifdef FINALITY_SKILL_SYSTEM //김창한
	else if( true == m_bInvisibility )
	{
		pRenderParam_->renderType		= CKTDGXRenderer::RT_CARTOON;
		pRenderParam_->outLineColor.a	= 0;
		m_RenderParam.color.a			= 0;
	}
#endif //FINALITY_SKILL_SYSTEM
	else
		CX2GameUnit::ApplyRenderParam( pRenderParam_ );
}

/*virtual*/ void CX2GURaven::ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
	if( GetRemainHyperModeTime() > 0.0f )
	{
		pRenderParam_->renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
		pRenderParam_->cartoonTexType	= CKTDGXRenderer::CTT_RED;
		pRenderParam_->outLineColor		= D3DXCOLOR( 1.f, 0.694f, 0.737f, 1.f );
	}
	else
	{
		pRenderParam_->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		pRenderParam_->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
		pRenderParam_->outLineColor		= 0xffffffff;
	}
}

float CX2GURaven::GetReduceDebuffDurationRate(BUFF_TEMPLET_ID eBuffTempletID_)
{
	float fReduceDebuffDurationRate = 1.f;

	switch( eBuffTempletID_ )
	{
	case BTI_DEBUFF_WOUND:				//상처(컷텐던)
	case BTI_DEBUFF_PRESS:
	case BTI_DEBUFF_BLIND:				//시야 방해(명중률 감소, 샌드 스톰 따위)
	case BTI_DEBUFF_STUN:
	case BTI_DEBUFF_PANIC:				//공황 (판데모니움)
	case BTI_DEBUFF_MOVE_JUMP_SLOWDOWN:	//이동 둔화 (레그샷 따위)
	case BTI_DEBUFF_CONFUSION:
	case BTI_DEBUFF_MOTION_SLOWDOWN:	//동작 둔화(로우킥 따위)
	case BTI_DEBUFF_BLIND_SMOKE:		//연무 (회피 감소)
	case BTI_DEBUFF_BLEEDING:			//출혈
	case BTI_DEBUFF_SUPPRESSION:		//제압
	case BTI_DEBUFF_PANDEMONIUM_FEAR:	//판데모니움 - 피어
		{
			fReduceDebuffDurationRate = m_fReduceEDTDurationRate;
		} break;
	default:
		break;
	}

	return fReduceDebuffDurationRate;
}

#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환

/// RSI_A_RF_MEGADRILL_BREAK 메가드릴 브레이크
void CX2GURaven::RSI_A_RF_MEGADRILL_BREAK_Init()
{
	XSkinMeshReadyInBackground( L"MagnumBlaster_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Lire_SI_A_Low_Kick_Mesh02.X" );
	XSkinMeshReadyInBackground( L"Lire_SpecialAttackPierceArrow1a.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Shadow_Step_Mesh01.X" );
	XSkinMeshReadyInBackground( L"SpinningKick02.X" );
		
	TextureReadyInBackground( L"MagnumBlaster01_Mesh.tga" );
	TextureReadyInBackground( L"raven_beed_hit_circle.dds" );
	TextureReadyInBackground( L"raven_beed_hit_spark.DDS" );
	TextureReadyInBackground( L"state_potion_flare.DDS" );
	TextureReadyInBackground( L"ColorBallGray.dds" );	
}

void CX2GURaven::RSI_A_RF_MEGADRILL_BREAK_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.27f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.27f ) == true && EventCheck( 0.27f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.7f ) == true && EventCheck( 0.7f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 1.2f ) == true && EventCheck( 1.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.2f;
	}
	CommonFrameMoveFuture();
}

void CX2GURaven::RSI_A_RF_MEGADRILL_BREAK_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

		m_DamageData.bReAttack = true;
		m_DamageData.fHitGap = 0.1f;
		SetEnableAttackBox( L"Lhand1", true );
		SetEnableAttackBox( L"Sword", false );
		m_DamageData.backSpeed.x = 0.0f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) == true && EventCheck( m_fDamageDataChangeTime, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType			= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit	= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );	
			m_LuaManager.EndTable();		
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01_L_Hand" );
		m_hSeqMagnumBlaster1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Flare_Raven_MagnumBlaster06", vBonePos );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Raven_MagnumBlaster01_Mesh", vBonePos, GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMesh_MagnumBlaster2 = pMeshInst->GetHandle();
		}
	}

	if( m_hSeqMagnumBlaster1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqMagnumBlaster1 );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetBonePos( L"Bip01_L_Hand" ) );
		}
		else
		{
			m_hSeqMagnumBlaster1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}


	if( m_hMesh_MagnumBlaster2 != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMesh_MagnumBlaster2 );
		if( NULL != pMeshInst )
		{
			pMeshInst->SetPos( GetBonePos( L"Bip01_L_Hand" ) );
		}
		else
		{
			m_hMesh_MagnumBlaster2 = INVALID_MESH_INSTANCE_HANDLE;
		}
	}


	CommonFrameMove();
}

void CX2GURaven::RSI_A_RF_MEGADRILL_BREAK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	CommonEventProcess();
}

void CX2GURaven::RSI_A_RF_MEGADRILL_BREAK_End()
{
	SetEnableAttackBox( L"Lhand1", false );
	SetEnableAttackBox( L"Sword", true );
	CommonStateEnd();
}


/// RSI_A_RBM_SONIC_SLASH 소닉 슬래시
void CX2GURaven::RSI_A_RBM_SONIC_SLASH_LOOP_Init()
{
	XSkinMeshReadyInBackground( L"RBM_SONIC_SLASH_TRACE.X" );
	TextureReadyInBackground( L"PET_SILVERFOX_MIHOEffect02.tga" );
}


/// RSI_A_RBM_SONIC_SLASH 소닉 슬래시
void CX2GURaven::RSI_A_RBM_SONIC_SLASH_LOOP_StateStart()
{
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}

void CX2GURaven::RSI_A_RBM_SONIC_SLASH_LOOP_FrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.047f ) == true /*&& EventCheck( 0.047f, false ) == true*/ )
	{
		PlaySound( L"Raven_Slash1.ogg" );
	}
	else if( m_pXSkinAnim->EventTimer( 0.276f ) == true /*&& EventCheck( 0.276f, false ) == true*/ )
	{
		PlaySound( L"Raven_Slash2.ogg" );
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_A_RBM_SONIC_SLASH_LOOP_EventProcess()
{

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->GetNowPlayCount() > 6 )
	{
		StateChange( RSI_A_RBM_SONIC_SLASH_FINISH );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_A_RBM_SONIC_SLASH_FINISH_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}


/// RSI_SA_RST_SHOCKWAVE_FRONT 쇼크 웨이브 - 속
void CX2GURaven::RSI_SA_RST_SHOCKWAVE_FRONT_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh01.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh02.X" );
	XSkinMeshReadyInBackground( L"Raven_SI_SA_ShockWave_Blade_Mesh03.X" );

	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );

	TextureReadyInBackground( L"Arme_Damage_Ring.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );
	TextureReadyInBackground( L"Whitecircle02.dds" );

	TextureReadyInBackground( L"GroundShockWave02.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_FRONT_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_FRONT_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_FRONT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RST_SHOCKWAVE_FRONT_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}

/// RSI_SA_RBM_GIGA_DRIVE 기가 드라이브 - 폭
void CX2GURaven::RSI_SA_RBM_GIGA_DRIVE_BURST_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_SA_Gungnir_Mesh05.X" );
	XSkinMeshReadyInBackground( L"Arme_ChargeMp1.X" );
	XSkinMeshReadyInBackground( L"rena_A_evoke_Dummy.X" );
	XSkinMeshReadyInBackground( L"Arme_ChargeMp1.X" );

	XMeshReadyInBackground( L"Raven_BM_Giga_Wave02.y" );
	XMeshReadyInBackground( L"Raven_BM_Giga_Wave01.Y" );
	XMeshReadyInBackground( L"Thief_Fat_JumpAttackA_Stone02.Y" );

	
	TextureReadyInBackground( L"GroundShockWave02.dds" );
	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );	
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );	
	TextureReadyInBackground( L"Arme_Critical.dds" );	

	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"ColorBallFire.dds" );	
}

void CX2GURaven::RSI_SA_RBM_GIGA_DRIVE_BURST_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );
	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RBM_GIGA_DRIVE_BURST_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/// SI_SA_ROT_CHARGED_BOLT_BLOOD 차지드 볼트 - 쏜즈 블러드
void CX2GURaven::RSI_SA_ROT_CHARGED_BOLT_BLOOD_Init()
{
	TextureReadyInBackground( L"Raven_Overtaker_SP1a_ChargedBolt.tga" );
	TextureReadyInBackground( L"Raven_Overtaker_SP1a_ChargedBolt01.tga" );
	XSkinMeshReadyInBackground( L"Raven_OverTaker_SP1a_ChargedBolt.X" );
}

void CX2GURaven::RSI_SA_ROT_CHARGED_BOLT_BLOOD_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 0 );
	CommonFrameMove();
}

void CX2GURaven::RSI_SA_ROT_CHARGED_BOLT_BLOOD_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	CommonEventProcess();
}


// 스킬 개편 2013, 레이븐 레크리스 피스트 리미트 크러셔, 김종훈
void CX2GURaven::RSI_A_RRF_LIMIT_CRUSHER_CHARGE_Init()
{
	TextureReadyInBackground( L"soket_earth_quake_smoke01.DDS" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.DDS" );
	TextureReadyInBackground( L"soket_earth_quake_stone01.dds" );

	XSkinMeshReadyInBackground( L"Land_Dimolisher_M01.X" );
	XSkinMeshReadyInBackground( L"Land_Dimolisher_M03.X" );
	XSkinMeshReadyInBackground( L"Land_Dimolisher_M05.X" );
}

// 스킬 개편 2013, 레이븐 레크리스 피스트 리미트 크러셔, 김종훈
void CX2GURaven::RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateStartFuture()
{
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );

	CommonStateStartFuture();
}

void CX2GURaven::RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateStart()
{
	m_bIsLimitCrusherCharge = true;

	CommonStateStart();
}

void CX2GURaven::RSI_A_RRF_LIMIT_CRUSHER_CHARGE_EventProcess()
{
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();

	/// 모션이 끝나거나, 일정 모션이 지난 이후로 스킬 키를 놓았다면
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true ||
		0.45f <= m_pXSkinAnimFuture->GetNowAnimationTime() && true == m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData ) )
	{
		StateChange( RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK );
	}
	else if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	CommonEventProcess();
}

void CX2GURaven::RSI_A_RRF_LIMIT_CRUSHER_CHARGE_StateEnd()
{
	m_bIsLimitCrusherCharge = false;

	CommonStateEnd();
}

void CX2GURaven::RSI_A_RRF_LIMIT_CRUSHER_REVENGE_ATTACK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GURaven::RSI_A_RRF_LIMIT_CRUSHER_AVOID_ATTACK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// 스킬 개편 2013, 레이븐 레크리스 피스트 헬 다이브, 김종훈
void CX2GURaven::RSI_A_RRF_HELL_DIVE_Init()
{	
	XSkinMeshReadyInBackground( L"Raven_RRF_SI_SA_X_Crash_Mesh01.x" );
		
		
	TextureReadyInBackground( L"Dark_Smoke.dds" );
	TextureReadyInBackground( L"Flame_REVOLVER.dds" );
	TextureReadyInBackground( L"Particle_Blur.DDS" );
	TextureReadyInBackground( L"Nasod_King_Laser03.dds" );
	TextureReadyInBackground( L"ColorBallFire.dds" );
	TextureReadyInBackground( L"Nasod_King_Laser03.dds" );

	TextureReadyInBackground( L"star2.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );

}

void CX2GURaven::RSI_A_RRF_HELL_DIVE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.166f ) == true && EventCheck( 1.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_vXCrashPos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
	}		
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.31f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.31f ) == true && EventCheck( 1.31f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_RRF_HELL_DIVE_LAST_PUNCH", GetPowerRate(), m_vXCrashPos, GetRotateDegree(), GetRotateDegree() );				
	}

	CommonFrameMove();
}

void CX2GURaven::RSI_A_RRF_HELL_DIVE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}		

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// 스킬 개편 2013, 레이븐 웨폰 테이커 화염인 Initialize, 김종훈
void CX2GURaven::RSI_A_RWT_FLAME_SWORD_Init()
{
	XSkinMeshReadyInBackground( L"RWT_BARRAGE_ATTACK_TRACE_Stand.X" );
	XSkinMeshReadyInBackground( L"RVC_SA_DeadlyRaid_circleLine.X" );

	TextureReadyInBackground( L"ColorBallFire.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"RVC_SA_DeadlyRaid_line.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
	TextureReadyInBackground( L"Dark_Smoke.dds" );
	TextureReadyInBackground( L"Flame_REVOLVER.dds" );
	TextureReadyInBackground( L"Nasod_King_Laser03.dds" );
}

void CX2GURaven::RSI_A_RWT_FLAME_SWORD_StateStart()
{
	CommonStateStart();
}

void CX2GURaven::RSI_A_RWT_FLAME_SWORD_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( RSI_A_RWT_FLAME_SWORD_FLY );
	}
	else
	{
		StateChange( RSI_A_RWT_FLAME_SWORD_GROUND );
	}

	CommonEventProcess();
}
// 스킬 개편 2013, 레이븐 웨폰 테이커 화염인 지상 Ver., 김종훈
void CX2GURaven::RSI_A_RWT_FLAME_SWORD_GROUND_StateStart ()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RWT_FLAME_SWORD );
	if ( NULL != pSkillTemplet && pSkillTemplet->m_vecBuffFactorPtr.size() >= 1 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( NULL == GetUnit()  )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
		
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
}

void CX2GURaven::RSI_A_RWT_FLAME_SWORD_GROUND_EventProcess ()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

// 스킬 개편 2013, 레이븐 웨폰 테이커 화염인 공중 Ver., 김종훈
void CX2GURaven::RSI_A_RWT_FLAME_SWORD_AIR_StateStart ()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_RWT_FLAME_SWORD );
	if ( NULL != pSkillTemplet && pSkillTemplet->m_vecBuffFactorPtr.size() >= 1 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( NULL == GetUnit()  )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
}

void CX2GURaven::RSI_A_RWT_FLAME_SWORD_AIR_FrameMoveFuture ()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = m_PhysicParam.GetWalkSpeed();
		}

		if( m_InputData.pureDown == true )
		{
			m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		}
	}

	CommonFrameMoveFuture();
}
void CX2GURaven::RSI_A_RWT_FLAME_SWORD_AIR_EventProcess ()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER_FLY( m_fSkillCancelAfterFly )

	CommonEventProcess();
}


// 스킬 개편 2013, 레이븐 베테랑 커맨더 네이팜 그레네이드, 김종훈
void CX2GURaven::RSI_A_RVC_NAPALM_GRENADE_Init ()
{
	XSkinMeshReadyInBackground( L"RVC_C_ShadowBackSlide_grenade.X" );
	XSkinMeshReadyInBackground( L"Lire_SI_A_Low_Kick_Mesh02.X" );
	XSkinMeshReadyInBackground( L"SI_A_RF_MEGADRILL_BREAK_M01.X" );
	XSkinMeshReadyInBackground( L"EffectSet_RBM_HYPER_STEP_EXPLO_M01.X" );
	XSkinMeshReadyInBackground( L"SI_SA_RRF_WILD_CHARGE05.X" );

	TextureReadyInBackground( L"Circle_Light01.DDS" );
	TextureReadyInBackground( L"Steam_BP.DDS" );
	TextureReadyInBackground( L"Explosion_Fire01.DDS" );
	TextureReadyInBackground( L"Dark_Smoke.dds" );
	TextureReadyInBackground( L"secretVelder_perpit_fire_flame.dds" );	

	TextureReadyInBackground( L"WhitePointSmall.dds" );
	TextureReadyInBackground( L"Circle_Light01.DDS" );	
}

void CX2GURaven::RSI_A_RVC_NAPALM_GRENADE_EventProcess ()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}


void CX2GURaven::RSI_A_RVC_NAPALM_GRENADE_FrameMove ()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.60f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.60f ) == true && EventCheck( 0.60f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_NAPALM_GRENADE_BULLET_1", 
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_NAPALM_GRENADE_BULLET_2", 
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_NAPALM_GRENADE_BULLET_3", 
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
	CommonFrameMove();
}


// 스킬 개편 2013, 레이븐 웨폰 테이커 리볼버 캐논 HE, 김종훈
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_Init()
{
	TextureReadyInBackground ( L"Steam_BP.dds" );
	TextureReadyInBackground ( L"AeroTornado04.DDS" );
	TextureReadyInBackground ( L"Arme_Critical.dds" );

	XSkinMeshReadyInBackground( L"High_Explosive_Shell02.X" );
	XSkinMeshReadyInBackground( L"Damage_Effect_Dummy.X" );
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_StateStart()
{
	CommonStateStart();

	_SetOverheat( false );
	m_bShowedOverheat = false;
	m_iRWTComboLoopCount = 0;
	m_iRWTMaxComboLoopCount = _CONST_RAVEN_::MIN_REVOLVER_CANNON_LOOP_COUNT;
	m_iOverheatKeyPressTime = 0;
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_FrameMove()
{	
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );
	CommonFrameMove();
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
			++m_iOverheatKeyPressTime;
	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP );
	}
	else
	{
	}
	CommonEventProcess();
}


// 스킬 개편 2013, 레이븐 웨폰 테이커 리볼버 캐논 HE Loop, 김종훈
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_StateStart()
{
	++m_iRWTComboLoopCount;
	if( m_iRWTComboLoopCount > _CONST_RAVEN_::MIN_REVOLVER_CANNON_LOOP_COUNT )
		_SetOverheat( true, 5.f );
	else
		_SetOverheat( false );

	CommonStateStart();
}
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_FrameMove()
{
	// 이벤트 플래그가 무엇을 나타내는지 알기 쉽게 하도록
	bool &bOverheat = m_FrameDataFuture.stateParam.bEventFlagList[0];

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( m_iRWTComboLoopCount == _CONST_RAVEN_::MIN_REVOLVER_CANNON_LOOP_COUNT + 1 )
			PlaySound( L"RavenVoice_Shout12.ogg" );

		//포쏘기
		D3DXVECTOR3 vDirVector = GetDirVector();
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01_L_Hand" );
		D3DXVECTOR3 vOffset = ( vDirVector * 50.f );
		vPos += GetIsRight() ? -vOffset : vOffset;

		float fPowerRate = GetPowerRate();

		if( m_bOverheated )
		{
			if( m_iRWTComboLoopCount >= m_iRWTMaxComboLoopCount )
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_HE_FINAL_SHELL_OVERHEAT",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

				/// 오버 히트 데미지 처리
				CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_HE_SHELL_IMPACT_BIG",
					GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
				if( NULL != pEffect )
				{
					_BurnOverheatHP( pEffect->GetDamageData() );
					g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( pEffect );
				}
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_HE_SHELL_OVERHEAT",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
				/// 오버 히트 데미지 처리
				CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_HE_SHELL_IMPACT",
					GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
				if( NULL != pEffect )
				{
					_BurnOverheatHP( pEffect->GetDamageData() );
					g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( pEffect );
				}
			}

		}
		else
		{
			if( m_iRWTComboLoopCount >= m_iRWTMaxComboLoopCount )
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_HE_FINAL_SHELL",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RAVEN_REVOLVER_CANNON_HE_SHELL",
					fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			}
		}
	}

	CommonFrameMove();
}
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP_EventProcess()
{
	if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
		m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
		m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
		m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
		m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
		m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
		m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
		m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
	{
		if( _CanOverheat() )
		{
			++m_iOverheatKeyPressTime;

			if( m_iOverheatKeyPressTime >= _CONST_RAVEN_::OVERHEAT_KEY_PRESS_TIME )
			{
				if( false == m_bShowedOverheat )
					_ShowOverheatIcon();
				m_bShowedOverheat = true;

				m_iRWTMaxComboLoopCount = _CONST_RAVEN_::MAX_REVOLVER_CANNON_LOOP_COUNT;
#ifdef ADD_MEMO_1ST_CLASS //김창한
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_RAVEN_MEMO28 ) == true )
					++m_iRWTMaxComboLoopCount;
#endif //ADD_MEMO_1ST_CLASS
			}
		}

	}

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		// 한발이 모자라도 넘어가자-피니시에서 한방 쏘니까
		if( m_iRWTComboLoopCount + 1 >= m_iRWTMaxComboLoopCount )
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH );
		}
		else
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_HE_LOOP );
		}
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_bOverheated )
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_HE_OVER_END );
		}
		else
		{
			StateChange( RSI_SA_RWT_REVOLVER_CANNON_HE_END );
		}
	}
	CommonEventProcess ();
}

// 스킬 개편 2013, 레이븐 웨폰 테이커 리볼버 캐논 HE End, 김종훈
void CX2GURaven::RSI_SA_RWT_REVOLVER_CANNON_HE_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

}


/// 레이븐 2차 전직 베테랑 커맨더 - 이그니션 크로우
void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_Init()
{
	XSkinMeshReadyInBackground( L"IGNITION_CROW_BIRD_D.x" );
	XSkinMeshReadyInBackground( L"rena_A_evoke_Dummy.X" );
	
	TextureReadyInBackground( L"Flame_REVOLVER.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"RVC_SA_DeadlyRaid_line.dds" );
	TextureReadyInBackground( L"RVC_SA_DeadlyRaid_spark.dds" );
	TextureReadyInBackground( L"WhitePointSmall.dds" );
	TextureReadyInBackground( L"secretVelder_perpit_fire_flame.dds" );
	TextureReadyInBackground( L"Feather.DDS" );
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_StateStart()
{
	m_iRVCComboLoopInputCount  = 0;
	m_bShowedOverheat		   = false;

	CommonStateStart();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.04f, 2 );
	CommonFrameMove();
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_EventProcess()
{
	if( 2.01f > m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( m_eSpecialAttackKeyPressed == SAKP_A && m_InputData.oneA == true ||
			m_eSpecialAttackKeyPressed == SAKP_S && m_InputData.oneS == true ||
			m_eSpecialAttackKeyPressed == SAKP_D && m_InputData.oneD == true ||
			m_eSpecialAttackKeyPressed == SAKP_C && m_InputData.oneC == true ||
			m_eSpecialAttackKeyPressed == SAKP_Q && m_InputData.oneQ == true ||
			m_eSpecialAttackKeyPressed == SAKP_W && m_InputData.oneW == true ||
			m_eSpecialAttackKeyPressed == SAKP_E && m_InputData.oneE == true ||
			m_eSpecialAttackKeyPressed == SAKP_R && m_InputData.oneR == true )
		{
			++m_iRVCComboLoopInputCount;		/// 스킬 키 입력 시 누적 입력 카운트 증가

			if( _CONST_RAVEN_::OVERHEAT_KEY_PRESS_TIME <= m_iRVCComboLoopInputCount && true == _CanOverheat() )
			{
				if( false == m_bShowedOverheat )
					_ShowOverheatIcon();
				m_bShowedOverheat = true;
			}
		}
	}


	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == m_bShowedOverheat )
			StateChange( RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT );
		else
			StateChange( RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE );
	}

	CommonEventProcess();
}


void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_StateStart()
{
	_SetOverheat( true, 5.f );

	CommonStateStart();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		/// 오버 히트 데미지 처리
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_IGNITION_CROW_INCINERATION_OVERHEAT_PENALTY",
			GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );

		if( NULL != pEffect )
		{
			_BurnOverheatHP( pEffect->GetDamageData() );

			g_pX2Game->GetDamageEffect()->DestroyInstanceSilently( pEffect );
		}
	}

	CommonFrameMove();
};

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_FIRE_OVERHEAT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END );
	}

	CommonEventProcess();
}

void CX2GURaven::RVC_SI_SA_IGNITION_CROW_INCINERATION_OVERHEAT_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{		
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}


void CX2GURaven::RSI_P_RBM_SHADOW_STEP_THRUST_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Thrust_Mesh01.X" );
}



void CX2GURaven::RSI_P_RBM_SHADOW_STEP_THRUST_Start()
{
	CommonStateStart();

	m_DamageData.damage.fPhysic					*= m_fAttackPowerRateShadowStepThrustEnhance;
	m_DamageData.damage.fMagic					*= m_fAttackPowerRateShadowStepThrustEnhance;		
	m_DamageData.m_ExtraDamage.m_DamagePerSec	*= m_fAttackPowerRateShadowStepThrustEnhance;

}

void CX2GURaven::RSI_P_RBM_SHADOW_STEP_THRUST_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}



void CX2GURaven::RSI_P_RRF_SHADOW_PUNISHER_Init()
{
	XSkinMeshReadyInBackground( L"Raven_SI_A_RF_Thrust_Mesh01.X" );
}



void CX2GURaven::RSI_P_RRF_SHADOW_PUNISHER_Start()
{
	CommonStateStart();

	m_NowStateData.m_fPowerRate = m_fAttackPowerRateShadowStepPunisher;

	m_DamageData.damage.fPhysic					*= m_fAttackPowerRateShadowStepPunisher;
	m_DamageData.damage.fMagic					*= m_fAttackPowerRateShadowStepPunisher;		
	m_DamageData.m_ExtraDamage.m_DamagePerSec	*= m_fAttackPowerRateShadowStepPunisher;

}

void CX2GURaven::RSI_P_RRF_SHADOW_PUNISHER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	CommonEventProcess();
}

void CX2GURaven::RSI_A_ROT_ARMOR_BREAK_Init()
{
	TextureReadyInBackground( L"Break_Impact01.dds" );
}


void CX2GURaven::RSI_A_ROT_ARMOR_BREAK_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
		{
			StateChange( USI_WAIT );
		}
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/** @function	: SetEquippedSkillLevelStateData
	@brief		: 장착한 스킬의 레벨등이 변경될 때 같이 변경되어야 할 스테이트 데이타
	@param		: const CX2SkillTree::SkillTemplet* pSkillTemplet, ARA_STATE_ID eStateID
*/
void CX2GURaven::SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet, RAVEN_STATE_ID eStateID )
{
	if ( NULL != pSkillTemplet )
		SetSkillLevelStateData( pSkillTemplet, m_StateList[eStateID] );
}
void CX2GURaven::SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ )
{
	if ( NULL == GetUnit()  )
		return;
	
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨
	
	stateData_.m_SPLevel	= iSkillTempletLevel;
	stateData_.m_fPowerRate	= pSkillTemplet_->GetSkillPowerRateValue( iSkillTempletLevel );
	stateData_.m_eSkillID	= pSkillTemplet_->m_eID;
}
#endif UPGRADE_SKILL_SYSTEM_2013

/*virtual*/ void CX2GURaven::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ )
{
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", fTimeToShow_, m_iSkillCutInSetSubIndex, bOnlyLight_ );
}



#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
void CX2GURaven::RSI_HA_RRF_INFERNAL_ARM_Init()
{	
	TextureReadyInBackground(L"DarkGate_G_Baby_Potion2.dds");
	TextureReadyInBackground(L"aisha_SA_MorningStar_circle.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"Sprriggan_Light_Ball_02.Tga");
	TextureReadyInBackground(L"Sprriggan_Light_Ball_01.Tga");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Spark02_BCMI.dds");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SlashLight.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"RRF_Reckless_Fiste_Spark.tga");
	TextureReadyInBackground(L"elsword_active_swordFall_downSpark.DDS");
	TextureReadyInBackground(L"Pa_Ran_SpaD_Smoke02.DDS");
	TextureReadyInBackground(L"Steam_BP.dds");
	TextureReadyInBackground(L"RVC_SA_DeadlyRaid_spark.dds");
	TextureReadyInBackground(L"Explosion_Sphere.dds");
	TextureReadyInBackground(L"Condense_Pulse01.dds");
	XMeshReadyInBackground(L"Ring_Plane.Y");
	XSkinMeshReadyInBackground(L"RRF_Infernal_Arms_Effect.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	
}
void CX2GURaven::RSI_HA_RRF_INFERNAL_ARM_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


// 익스트림 블레이드 FX
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_READY_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"COMET_CRASHER_Explosion_02.tga");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"GuideArrow01.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	XSkinMeshReadyInBackground(L"Raven_SI_A_RF_Shadow_Step_Mesh01.X");
	XSkinMeshReadyInBackground(L"Raven_SI_SA_ShockWave_Blade_Mesh01.X");
	XSkinMeshReadyInBackground(L"Sprriggan_AirCurren_02.X");
	XSkinMeshReadyInBackground(L"Sprriggan_MagicAttackB.X");
	TextureReadyInBackground(L"Whitecircle02.dds");
}
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( RSI_HA_RBM_EXTREM_BLADE_ATTACK );
	}

	CommonEventProcess();
}

void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_Init()
{
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	TextureReadyInBackground(L"Condense_pulse01.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"Sprriggan_Light_Ball_02.Tga");
	TextureReadyInBackground(L"Sprriggan_Light_Ball_01.Tga");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"PET_SILVERFOX_MIHOEffect02.tga");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Star.dds");
	TextureReadyInBackground(L"SlashLight.dds");
	TextureReadyInBackground(L"COMET_CRASHER_Explosion_02.tga");
	XMeshReadyInBackground(L"Ring_Plane.Y");
	XSkinMeshReadyInBackground(L"SA_RBM_Extrem_RAVEN_Motion01.X");	
}
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_StateStart()
{
	m_fExtremBladeDelay = 0.0f;
	CommonStateStart();
}
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_FrameMove()
{
	m_fExtremBladeDelay += m_fElapsedTime;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.01f ) && true == EventCheck( 0.01f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		SetInvisibility( true );
		TransformScale( PROTECT_VECTOR3( 0.01f, 0.01f, 0.01f ) );

		CX2EffectSet::Handle hEffectSset =
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SA_RBM_Extrem_Blade_Middle", this, NULL, false, GetPowerRate() );

		D3DXVECTOR3 vMyPos = GetPos();
		D3DXVECTOR3 vDestPos = vMyPos;
		vDestPos.y += 350;
		m_FrameDataFuture.syncData.position = vDestPos;
	}
	
	CommonFrameMove();
}
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_EventProcess()
{
	if( m_fExtremBladeDelay > 1.f )
	{
		StateChange( RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH );
	}
	CommonEventProcess();
}
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_StateEnd()
{
	SetInvisibility(false);
	
	TransformScale( GetChangeUnitScaleByBuff() );

	m_fExtremBladeDelay = 0.0f;
	CommonStateEnd();
}

void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_Init()
{
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"COMET_CRASHER_Explosion_02.tga");
	TextureReadyInBackground(L"Colorballgray.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Explosion_Light02.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"GuideArrow01.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh03.X");
	TextureReadyInBackground(L"PET_SILVERFOX_MIHOEffect02.tga");
	TextureReadyInBackground(L"PulseWave01.dds");
	XSkinMeshReadyInBackground(L"Sprriggan_AirCurren_02.X");
	TextureReadyInBackground(L"WhiteImage.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"Whitecircle02.dds");
}
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_StartFuture()
{
	D3DXVECTOR3 vMyPos = GetPos();
	D3DXVECTOR3 vDestPos = vMyPos;
	vDestPos.y -= 350;
	m_FrameDataFuture.syncData.position = vDestPos;

	CommonStateStartFuture();
}
void CX2GURaven::RSI_HA_RBM_EXTREM_BLADE_ATTACK_FINISH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}	

	CommonEventProcess();
}
void CX2GURaven::SetInvisibility(bool bVal_)
{
	m_bInvisibility					= bVal_;
	m_bAbsoluteInvisibility			= bVal_;

	m_RenderParam.bAlphaBlend = bVal_;

	if( false == m_bInvisibility )
		m_RenderParam.color.a = 1.f;

	m_RenderParam.bAlphaBlend = m_bInvisibility;
	SetAlphaObject( m_bInvisibility );

	m_pXSkinAnim->SetLayer(1);
	// 장비에 부착된 파티클
	BOOST_TEST_FOREACH( CX2EqipPtr, pEquip, m_ViewEqipList )
	{
		if( pEquip != NULL )
			pEquip->SetShowAttachedParticle( !m_bInvisibility );
	}

	// 무기 속성 인챈트 파티클
	SetShowEnchantWeaponParticle( !m_bInvisibility );
	// 게이지 바
	if ( false == IsMyUnit() && false == IsNullGageUI() )
	{
		if( true == m_bInvisibility )
			m_pGageUI->SetAlpha( 0.f );
		else
			m_pGageUI->SetAlpha( 255.f );
	}

	// 각성 상태라면 각성 파티클 켜고/끄기
	if( GetRemainHyperModeTime() > 0.f )
	{
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostRFoot )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_RFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRFoot );
			if( NULL != pSeq_RFoot )
				pSeq_RFoot->SetShowObject( !m_bInvisibility );
		}
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostLFoot )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_LFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLFoot );
			if( NULL != pSeq_LFoot )
				pSeq_LFoot->SetShowObject( !m_bInvisibility );
		}
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostRArm )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_RArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRArm );
			if( NULL != pSeq_RArm )
				pSeq_RArm->SetShowObject( !m_bInvisibility );
		}
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hHyperBoostLArm )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq_LArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLArm );
			if( NULL != pSeq_LArm )
				pSeq_LArm->SetShowObject( !m_bInvisibility );
		}
	}
}

#pragma region SI_HA_RVC_BURNING_BUSTER
void CX2GURaven::RSI_HA_RVC_BURNING_BUSTER_Init()
{
	TextureReadyInBackground(L"Condense_Pulse01.dds");
	XSkinMeshReadyInBackground(L"Elsword_SI_SA_Sonic_Blade_Mesh01.X");
	TextureReadyInBackground(L"Explosion_Sphere.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"IGNITION_CROW_BIRD.X");
	TextureReadyInBackground(L"RRF_Reckless_Fiste_Spark.tga");
	TextureReadyInBackground(L"RVC_SA_DeadlyRaid_spark.dds");
	XSkinMeshReadyInBackground(L"SpiralBlast01.X");
	TextureReadyInBackground(L"Steam_BP.dds");
	XSkinMeshReadyInBackground(L"VANGUARD_MagicAttack_Explosion_03.X");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	XSkinMeshReadyInBackground(L"aisha_active_energySpurt_circle.X");
	XSkinMeshReadyInBackground(L"elsword_SA_SwordBlasting_circle.X");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
}
#pragma endregion 버닝 버스터 - 궁극기
#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템


#ifdef MODIFY_RIDING_PET_AWAKE
void CX2GURaven::RidingHyperModeFrameMove()
{
	CommonHyperModeFrameMove(0.f, 0.1f, true);
}
void CX2GURaven::CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ /*= false*/  )
{
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	//g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTime1_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTime1_ ) == true && EventCheck( fTime1_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTime2_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTime2_ ) == true && EventCheck( fTime2_, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();

		if( true == bSound_ )
		{
			PlaySound( L"Energy.ogg" );
			PlaySound( L"Raven_Slash1.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}
	CommonFrameMove();
}
#endif // MODIFY_RIDING_PET_AWAKE
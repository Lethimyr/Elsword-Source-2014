#include "StdAfx.h"
#include "X2GUAra.h"

/**
	@file	X2GUAra.cpp
	@author	JHKang
	@brief	아라 클래스 구현부 정의
	@since  2012-11-06
*/
#ifdef ARA_CHARACTER_BASE

#pragma region 아라에 사용될 상수 정의
namespace _CONST_ARA_
{
	const float MAGIC_NUMBER_MULTIPLY_LINE_RADIUS = 1.5f;
	const float MAGIC_MAX_AIRIAL_DASH_COUNT = 1;
	const float MAGIC_MAX_DOUBLE_JUMP_COUNT = 1;

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
	const float			FORCE_SPEAR_MEMO_ADDITIONAL_RANGE_RATE	= 1.33f;				/// 기공창 메모로 인한 사거리 증가값 ( 오차 값을 감안하여, 33% 설정 )
	const D3DXVECTOR3	GAIN_FORCE_MEMO_ADDITIONAL_RANGE_RATE( 1.35f, 1.35f, 1.35f );	/// 흡공 메모로 인한 데미지 이펙트 크기 증가값 ( 20% 증가인데 커졌다는 느낌이 잘 않들어서, 1.35배로 증가 )
	const int			ROCK_CRASH_MEMO_RESUCE_DAMAGE			= 50;					/// 바위 깨기 메모로 인한 데미지 감소율

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	const float			ENERGY_BULLET_MP_CONSUMPTION			=  4.f;					/// 아라 1차 전직 소마, 2차 전직 명왕 기탄 MP 소모량
	const float			ENERGY_VOID_ONE_CHARGE_TIME				=  0.4f;				/// 아라 2차 전직 명왕 초진공장 차지 한번당 유지시간
	const float			ENERGY_VOID_ONE_CHARGE_MP_CONSUMPTION	=  25.f;				/// 아라 2차 전직 명왕 초진공장 차지 한번당 소모 MP
	const int			ENERGY_VOID_MAX_CHARGE_VALUE			=  10;					/// 아라 1차 전직 명왕 초진공장 최대 차지 횟수
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#endif // ADD_ARA_MEMO
}
#pragma endregion namespace

#pragma region 필살기 사용 시 Cut in image
static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] =
{
	{ /// 노전직, // 1차전직 ( 소선 )
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_normal.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_Rage.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_Rage.dds"),
	}, 

#ifdef ARA_CHANGE_CLASS_FIRST //임시
	{ /// 제천
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_SD_normal.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_SD_Rage.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_SD_Rage.dds"),
	},
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	{ /// 야마 라자
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_YR_normal.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_YR_Rage.dds"),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_YR_Rage.dds"),
	}
#endif //SERV_ARA_CHANGE_CLASS_SECOND

};

#ifdef SERV_APRIL_FOOLS_DAY
static const CX2GUUser::SkillCutIn s_SkillCutInSetFoolsDay = CX2GUUser::SkillCutIn( L"HQ_CutIn_Ara_Gura.dds");
#endif //APRIL_FOOLS_DAY
#pragma endregion 

#pragma region 생성자
/** @function 	: CX2GUAra
	@brief 		: Ara 유저 유닛을 생성한다.
	
	@param iUnitIndex_			: 유닛의 인덱스 번호
	@param iTeamNum_			: 팀 번호
	@param pWstrScriptFileName_	: 스크립트 파일명
	@param iFrameBufferNum_		: 프레임 버퍼 번호
	@param pUnit				: Unit 포인트
*/
CX2GUAra::CX2GUAra( int iUnitIndex_, int iTeamNum_, IN lua_State* pScriptLuaState_, int iFrameBufferNum_, IN CX2Unit* pUnit_ )
	: CX2GUUser( iUnitIndex_, iTeamNum_, pScriptLuaState_, iFrameBufferNum_, pUnit_ )
	, m_bBerserkMode( false ), MAX_ANIMATION_LOOP_COUNT_AT_THIS_STATE(5)
	, m_fDamageDataChangeTime( 0.f ), m_fDamageDataChangeTime2( 0.f ), m_uiComboXXXLoopCount( 0 ), m_fZpushXChargeTime( 0.f )
	, m_fAirialDashTime( 0.f ), m_usArialCount( 0 ), m_usDoubleJumpCount( 0 ), m_bFPCancel( false )
	, m_usForceGainCount( 0 ), m_usBeforeForceGainCount( 0 ), m_pSkillTemplet( NULL ), m_iPressedSkillSlotIndex( -1 )
	, m_fMPConsume( 0.f )	

	, m_fShadowKnotTime( 0.f )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    , m_hShadowKnotWeaponMesh( INVALID_DAMAGE_EFFECT_HANDLE )	
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    , m_pShadowKnotWeaponMesh( NULL )
/*#ifndef FIX_GAIN_FORCE_BUG	
	, m_pGainForceDamaggeEffect( NULL )
#endif	FIX_GAIN_FORCE_BUG	*/	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	, m_fForceSpearDamageRel( 1.f ), m_usForceSpearPiercing( 0 )
#endif //UPGRADE_SKILL_SYSTEM_2013
	, m_fZZZXChargeTime( 0.f ), m_hPullinPierce( NULL )
	, m_fPullingDragonPowerRate( 1.f )
#ifdef ARA_CHANGE_CLASS_FIRST
	, m_bAbleQuickStand( false ) ,m_iGainMpByFpConversion( 0 ), m_fPreRenderParamColorA (1.f), m_fRenderParamColorA( 1.f )
	, m_fBlockingRate( 0.f ), m_fTurningRate( 0.f ), m_bBrutalPiercing( false )
	, m_fReduceEDTDurationRate ( 1.f ), m_hTempEffectSet( INVALID_EFFECTSET_HANDLE )
	, m_vPreTransScale(1,1,1)
#endif
#ifdef FIX_SKILL_CANCEL_ARA
	, m_eSkillID( CX2SkillTree::SI_NONE )
#endif
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	, m_fCurrentSpeed( 1.f ), m_bMoonSlash( false )
	, m_fSuppresionPowerRate( 1.f )
#endif //UPGRADE_SKILL_SYSTEM_2013
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	, m_fIncreaseEnergeBulletSize( 1.f )
	, m_fIncreaseEnergeBulletDamage( 1.f )
	, m_fIncreaseEnergeBulletRange( 1.f )
	, m_fIncreaseEnergeBulletForceDownValue( 0.f )
	, m_fResurrectionCoolTime( 0.f )
	, m_fHighSpeedValue( 0.f )
	, m_fUseForcePowerTime( -1.f )
	, m_iUseForcePowerCount( 0 )
	, m_fAddForcePowerTime( -1.f )
	, m_iAddForcePowerCount( 0 )
	, m_iEnergeVoidChargeValue ( 0 )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    , m_hEnergeVoidDamageEffect( INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	, m_pEnergeVoidDamageEffect( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	, m_bEnergeVoidChargeEnd( false )
	, m_fHowlingDamageRel( 1.0f )
	, m_fHowlingActiveRate( 0.f )
	, m_iWolfBreakeCount( 0 )
	, m_fOpenTheHellIncreaseDamage( 1.f )
#endif // SERV_ARA_CHANGE_CLASS_SECOND
//#ifdef FIX_GAIN_FORCE_BUG
	, m_iGainForceDamageTimeNow(0)
	, m_hGainForceIntakeEffect(INVALID_DAMAGE_EFFECT_HANDLE)
//#endif // FIX_GAIN_FORCE_BUG
{
	InitializeAraMajorParticleArray();
	InitializeAraMinorParticleArray();
	InitializeMajorMeshArray();

	m_fThrowStartTime		= 0.666f;
	m_vThrowStartPosOffset	= D3DXVECTOR3(40.f, 130.f, 0.f);
	m_vThrowStartVelocity	= D3DXVECTOR3(1600.f, 200.f, 0.f);

	INIT_VECTOR2( m_vFPCancel, -1.f, -1.f )
}
#pragma endregion

#pragma region 생성자 팩토리
/** @function 	: CreateGUAra
	@brief 		: 생성자 팩토리
*/
/*static*/ CX2GUAra* CX2GUAra::CreateGUAra( int iUnitIndex_, int iTeamNum_, IN lua_State* pScriptLuaState_,
											int iFrameBufferNum_, IN CX2Unit* pUnit_ )
{
	return new CX2GUAra( iUnitIndex_, iTeamNum_, pScriptLuaState_, iFrameBufferNum_, pUnit_ );
}
#pragma endregion

#pragma region 소멸자
/** @function 	: 소멸자
	@brief 		: 메모리 해제
*/
/*virtual*/ CX2GUAra::~CX2GUAra( void )
{
	DeleteAraMajorParticle();
	DeleteAraMinorParticle();
	DeleteMajorMesh();
}
#pragma endregion

#pragma region Init
/** @function 	: Init
	@brief 		: 아라 관련 멤버들 초기화 및 스크립트 파싱

	@param buseTeamPos	 : 팀 위치 사용 여부
	@param startPosIndex : 시작 위치 인덱스
*/
/*virtual*/ void CX2GUAra::Init( bool bUseTeamPos_, int iStartPosIndex_ )
{
	CX2GUUser::Init( bUseTeamPos_, iStartPosIndex_ );

	SetBasicForcePower( 2 );
	LoadPromotionItemID( m_LuaManager );
	InitPromotionEquipList();
	SetFullHyperMode( false );
	SetShowViewList( true );
	SetNeedToUpdateWeaponEffect( true );
	UpdateEquipCollisionData();
	
	// 기력 관련 코드가 들어가야 함
	SetMaxForcePower( min( GetUnitLevel() / 10, 6 ) + 4 );

	if ( true == IsMyUnit() )
		m_FrameDataFuture.syncData.m_CannonBallCount = GetForcePower();
}
#pragma endregion

#pragma region ReInit
/** @function 	: ReInit
	@brief 		: 게임 내에서 부활석 등에 의해 다시 게임을 시작할 때 처리해줘야 하는 부분(아라)

	@param buseTeamPos	 : 팀 위치 사용 여부
	@param startPosIndex : 시작 위치 인덱스
*/
/*virtual*/ void CX2GUAra::ReInit( bool bTeamPos_ /* = false */, int iStartPosIndex_ /* = -1 */ )
{
	SetFullHyperMode( false );
	CX2GUUser::ReInit( bTeamPos_, iStartPosIndex_ );

	#pragma region 기력 초기화
	switch ( g_pX2Game->GetGameType() )
	{
	default:
	case CX2Game::GT_DUNGEON:
		{
			// 현재 기력치와 기본 생성 기력치 비교 값 변경
			if ( GetForcePower() < GetBasicForcePower() )
				SetForcePower( GetBasicForcePower() );
		} break;
		
	case CX2Game::GT_PVP:
		{
			switch ( GetTeam() )
			{
			case CX2Room::TN_RED:
				{
					ResetForcePowerWhenRebirthInPVPGame( g_pX2Game->GetRedTeamKill(), g_pX2Game->GetBlueTeamKill() );
				} break;

			case CX2Room::TN_BLUE:
				{
					ResetForcePowerWhenRebirthInPVPGame( g_pX2Game->GetBlueTeamKill(), g_pX2Game->GetRedTeamKill() );
				} break;

			default:
				{
					SetForcePower( GetBasicForcePower() );
				} break;
			} // switch ( GetTeam() )
		} // switch ( g_pX2Game->GetGameType() )
	}
	#pragma endregion switch

	if ( IsMyUnit() )
	{
		// 기력 싱크 맞추기
		SetMaxForcePower( min( GetUnitLevel() / 10, 6 ) + 4 );
		m_FrameDataFuture.syncData.m_CannonBallCount= GetForcePower();
	}

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	/// 만약 호신강기 사용 쿨타임이 끝나지 않았다면, 안내 디버프 생성
	if ( 0.f < m_fResurrectionCoolTime )
		SetBuffFactorToGameUnitByBuffFactorID( BFI_DEBUFF_SELF_PROTECTION_FORTITUDE );

#endif // SERV_ARA_CHANGE_CLASS_SECOND
}
#pragma endregion

#pragma region InitStateID
/** @function : InitStateID
	@brief : 특정 상태들의 ID를 상위클래스의 멤버변수에 대입함
*/
/*virtual*/ void CX2GUAra::InitStateID()
{
	m_CommonState.m_Wait	= USI_WAIT;
}
#pragma endregion

#pragma region InitState
/** @function	: InitState
	@brief		: 전직별로 상태 초기화
*/
/*virtual*/ void CX2GUAra::InitState()
{
	switch ( GetUnitClass() )
	{
	case CX2Unit::UC_NONE:
		{
			ASSERT( !L"Unit Class Is fault!" );
			m_iSkillCutInSetIndex = 0;
		} break;

	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_LITTLE_HSIEN: // 소선 - 아라 1차 전직
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	case CX2Unit::UC_ARA_LITTLE_DEVIL: // 소마 - 아라 2_1차 전직
#endif //SERV_ARA_CHANGE_CLASS_SECOND
		{
			m_iSkillCutInSetIndex = 0;
		} break;
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_SAKRA_DEVANAM: // 제천 - 아라 2차 전직
		{
			m_iSkillCutInSetIndex = 1;
		} break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	case CX2Unit::UC_ARA_YAMA_RAJA: // 명왕 - 아라 2_2차 전직
		{
			m_iSkillCutInSetIndex = 2;
		} break;
#endif //SERV_ARA_CHANGE_CLASS_SECOND
	}

	InitStateID();
	InitStateCommon();
	InitStateByUnitClass();

	std::string strStartTableNameUTF8;
	std::string strWinTableNameUTF8;
	std::string strLoseTableNameUTF8;

	InitStateCommonRandom( strStartTableNameUTF8, strWinTableNameUTF8, strLoseTableNameUTF8 );

	#pragma region 상태
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
	#pragma endregion 데이터 생성

	InitPassiveSkillState();
	InitEquippedActiveSkillState();

	CX2GUUser::InitState();
}
#pragma endregion

#pragma region InitStateCommon
/** @function	: InitStateCommon
	@brief		: 모든 캐릭터가 공통적으로 쓰는 상태 및 전직에 상관없이 공통으로 쓰는 상태 초기화
*/
void CX2GUAra::InitStateCommon()
{
	UserUnitStateData stateData;

	#pragma region Base State
	#pragma region ASI_DIE_FRONT
	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "ASI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DIE_BACK
	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "ASI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DieBackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_START_INTRUDE
	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "ASI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

#pragma region ASI_WAIT
	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "ASI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, WaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, WaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, WaitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_HYPER_WAIT
	stateData.Init();
	stateData.stateID			= ASI_HYPER_WAIT;
	m_LuaManager.MakeTableReference( "ASI_HYPER_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, HyperWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, HyperWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, HyperWaitEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, HyperWaitStateEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_WALK
	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "ASI_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, WalkEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_JUMP_READY
	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "ASI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, JumpReadyEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_JUMP_UP
	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "ASI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, JumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ASI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, JumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ASI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, JumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH
	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "ASI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_END
	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "ASI_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, DashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_JUMP
	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ASI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAra, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAra, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_HYPER_MODE
	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ASI_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_BERSERK_MODE
	stateData.Init();
	stateData.stateID			= ASI_BERSERK_MODE;
	m_LuaManager.MakeTableReference( "ASI_BERSERK_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_GROGGY
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_SMALL_FRONT
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_SMALL_BACK
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DamageSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_BIG_FRONT
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_BIG_BACK
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DamageBigStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DamageBigEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_DOWN_FRONT
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, DamageDownFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DamageDownFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_DOWN_BACK
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, DamageDownBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DamageDownBackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_STANDUP_FRONT
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_STANDUP_BACK
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_SMALL
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_DOWN
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_DOWN_INVINCIBLE
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_DOWN_LANDING
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DamageAirDownLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_FALL
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_UP
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_FLY_FRONT
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_AIR_FLY_BACK
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DAMAGE_REVENGE
	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "ASI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_STANDUP_ROLLING_FRONT_FRONT
	stateData.Init();
	stateData.stateID			= ASI_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "ASI_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_STANDUP_ROLLING_FRONT_BACK
	stateData.Init();
	stateData.stateID			= ASI_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "ASI_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_STANDUP_ROLLING_BACK_FRONT
	stateData.Init();
	stateData.stateID			= ASI_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "ASI_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_STANDUP_ROLLING_BACK_BACK
	stateData.Init();
	stateData.stateID			= ASI_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "ASI_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_STANDUP_ATTACK_FRONT
	stateData.Init();
	stateData.stateID			= ASI_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "ASI_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpAttackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpAttackEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_STANDUP_ATTACK_BACK
	stateData.Init();
	stateData.stateID			= ASI_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "ASI_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpAttackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpAttackEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_JUMP_POWER_LANDING
	stateData.Init();
	stateData.stateID			= ASI_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DashJumpPowerLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DashJumpPowerLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashJumpPowerLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion 

	#pragma region ASI_WALL_LANDING
	stateData.Init();
	stateData.stateID			= ASI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "ASI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_PEPPER_RUN_READY
	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "ASI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_PEPPER_RUN
	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "ASI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_PEPPER_RUN_END
	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "ASI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_PEPPER_RUN_JUMP_UP
	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "ASI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_PEPPER_RUN_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ASI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_THROW_ITEM
	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "ASI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma endregion 기본 상태들

	#ifdef RIDING_SYSTEM
	#pragma region ASI_RIDING_START
	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "ASI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_ON
	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "ASI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOnEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_OFF
	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "ASI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_WAIT_HABIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "ASI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_WAIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "ASI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_WALK
	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "ASI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
	
	#pragma region ASI_RIDING_JUMP_UP
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "ASI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ASI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ASI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_DASH
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "ASI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_DASH_END
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "ASI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_DASH_JUMP
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ASI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_DASH_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ASI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_DAMAGE_FRONT
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "ASI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_DAMAGE_BACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "ASI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_DIE
	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "ASI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ASI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_JUMP_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ASI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_ATTACK_X
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "ASI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_SPECIAL_ATTACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "ASI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_RIDING_SPECIAL_MOVE
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "ASI_RIDING_SPECIAL_MOVE", stateData.stateID );
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
	m_LuaManager.MakeTableReference( "ASI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, RidingHyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingHyperModeEventProcess );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingHyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingHyperModeEnd );
	m_StateList[stateData.stateID] = stateData;
#endif // MODIFY_RIDING_PET_AWAKE


	#pragma region Combo State

	#pragma region ASI_AIRIAL_DASH
	stateData.Init();
	stateData.stateID			= ASI_AIRIAL_DASH;
	m_LuaManager.MakeTableReference( "ASI_AIRIAL_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, AirialDashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, AirialDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAra, AirialDashFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, AirialDashFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, AirialDashEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAra, AirialDashEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DOUBLE_JUMP_FRONT
	stateData.Init();
	stateData.stateID			= ASI_DOUBLE_JUMP_FRONT;
	m_LuaManager.MakeTableReference( "ASI_DOUBLE_JUMP_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DoubleJumpStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DoubleJumpFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DOUBLE_JUMP_UP
	stateData.Init();
	stateData.stateID			= ASI_DOUBLE_JUMP_UP;
	m_LuaManager.MakeTableReference( "ASI_DOUBLE_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, DoubleJumpStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DoubleJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
	
	#pragma region ASI_COMBO_Z
	stateData.Init();
	stateData.stateID			= ASI_COMBO_Z;
	m_LuaManager.MakeTableReference( "ASI_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_ZZ
	stateData.Init();
	stateData.stateID			= ASI_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ASI_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_ZZZ
	stateData.Init();
	stateData.stateID			= ASI_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ASI_COMBO_ZZZ", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboZZZFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZZZEventProcess );
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrByStateID( ASI_COMBO_ZZZ );
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_ZZZZ
	stateData.Init();
	stateData.stateID			= ASI_COMBO_ZZZZ;
	m_LuaManager.MakeTableReference( "ASI_COMBO_ZZZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_ZZZXStart
	stateData.Init();
	stateData.stateID			= ASI_COMBO_ZZZXStart;
	m_LuaManager.MakeTableReference( "ASI_COMBO_ZZZXStart", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZZZXStartEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_ZZZXCharge
	stateData.Init();
	stateData.stateID			= ASI_COMBO_ZZZXCharge;
	m_LuaManager.MakeTableReference( "ASI_COMBO_ZZZXCharge", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ComboZZZXChargeStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboZZZXChargeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZZZXChargeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, ComboZZZXChargeStateEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_ZZZXAttack
	stateData.Init();
	stateData.stateID			= ASI_COMBO_ZZZXAttack;
	m_LuaManager.MakeTableReference( "ASI_COMBO_ZZZXAttack", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboZZZXAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZZZXAttackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, ComboZZZXAttackStateEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_ZZX
	stateData.Init();
	stateData.stateID			= ASI_COMBO_ZZX;
	m_LuaManager.MakeTableReference( "ASI_COMBO_ZZX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboZZXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_X
	stateData.Init();
	stateData.stateID			= ASI_COMBO_X;
	m_LuaManager.MakeTableReference( "ASI_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_XX
	stateData.Init();
	stateData.stateID			= ASI_COMBO_XX;
	m_LuaManager.MakeTableReference( "ASI_COMBO_XX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboXXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_XXX
	stateData.Init();
	stateData.stateID			= ASI_COMBO_XXX;
	m_LuaManager.MakeTableReference( "ASI_COMBO_XXX", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ComboXXXStateStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboXXXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_XXXloop
	stateData.Init();
	stateData.stateID			= ASI_COMBO_XXXloop;
	m_LuaManager.MakeTableReference( "ASI_COMBO_XXXloop", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ComboXXXloopStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboXXXloopFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboXXXloopEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_XXXloopX
	stateData.Init();
	stateData.stateID			= ASI_COMBO_XXXloopX;
	m_LuaManager.MakeTableReference( "ASI_COMBO_XXXloopX", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ComboXXXloopXStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboXXXloopXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboXXXloopXEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, ComboXXXloopXStateEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_XXdownX
	stateData.Init();
	stateData.stateID			= ASI_COMBO_XXdownX;
	m_LuaManager.MakeTableReference( "ASI_COMBO_XXdownX", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboXXdownXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboXXdownXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_COMBO_XXdownXX
	stateData.Init();
	stateData.stateID			= ASI_COMBO_XXdownXX;
	m_LuaManager.MakeTableReference( "ASI_COMBO_XXdownXX", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboXXdownXXEventProcess );
	stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrByStateID( ASI_COMBO_XXdownXX );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_COMBO_Z
	stateData.Init();
	stateData.stateID			= ASI_DASH_COMBO_Z;
	m_LuaManager.MakeTableReference( "ASI_DASH_COMBO_Z", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboDashZFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboDashZEventProcess );
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrByStateID( ASI_DASH_COMBO_Z );
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_COMBO_ZpushX_CHARGE
	stateData.Init();
	stateData.stateID			= ASI_DASH_COMBO_ZpushX_CHARGE;
	m_LuaManager.MakeTableReference( "ASI_DASH_COMBO_ZpushX_CHARGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ComboDashZpushX_ChargeStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboDashZpushX_ChargeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboDashZpushX_ChargeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, ComboDashZpushX_ChargeStateEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_COMBO_ZpushX_ATTACK
	stateData.Init();
	stateData.stateID			= ASI_DASH_COMBO_ZpushX_ATTACK;
	m_LuaManager.MakeTableReference( "ASI_DASH_COMBO_ZpushX_ATTACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ComboDashZpushX_AttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboDashZpushX_AttackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, ComboDashZpushX_AttackStateEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_COMBO_X
	stateData.Init();
	stateData.stateID			= ASI_DASH_COMBO_X;
	m_LuaManager.MakeTableReference( "ASI_DASH_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ComboDashXEventProcess );
	stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrByStateID( ASI_DASH_COMBO_X );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_JUMP_ATTACK_Z
	stateData.Init();
	stateData.stateID			= ASI_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ASI_JUMP_ATTACK_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAra, JumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, JumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_JUMP_ATTACK_X
	stateData.Init();
	stateData.stateID			= ASI_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "ASI_JUMP_ATTACK_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUAra, JumpAttackXFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, JumpAttackXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, JumpAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_JUMP_COMBO_Z
	stateData.Init();
	stateData.stateID			= ASI_DASH_JUMP_COMBO_Z;
	m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_Z", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashJumpComboZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_JUMP_COMBO_ZZ
	stateData.Init();
	stateData.stateID			= ASI_DASH_JUMP_COMBO_ZZ;
	m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_ZZ", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashJumpComboZZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_JUMP_COMBO_ZZZ
	stateData.Init();
	stateData.stateID			= ASI_DASH_JUMP_COMBO_ZZZ;
	m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_ZZZ", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, DashJumpComboZZZFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashJumpComboZZZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ASI_DASH_JUMP_COMBO_X
	stateData.Init();
	stateData.stateID			= ASI_DASH_JUMP_COMBO_X;
	m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_X", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, DashJumpComboXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma endregion X, Z 콤보 상태들		
}
#pragma endregion

#pragma region InitStateByUnitClass
/** @function	: InitStateByUnitClass
	@brief		: 전직 별로 다른 스테이틀 초기화
*/
void CX2GUAra::InitStateByUnitClass()
{
	UserUnitStateData stateData;

	switch ( GetUnitClass() )
	{
	default:
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
		break;
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_SAKRA_DEVANAM: // 제천 전용 State case문 (break 추가하면 안됨)
		{
			stateData.Init();
			stateData.stateID			= ASI_DASH_JUMP_COMBO_ASD_ZX;
			m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_ASD_ZX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_ASD_ZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_DOUBLE_JUMP_FRONT_COMBO_ASD_Z_BACK;
			m_LuaManager.MakeTableReference( "ASI_DOUBLE_JUMP_FRONT_COMBO_ASD_Z_BACK", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_DOUBLE_JUMP_FRONT_COMBO_ASD_Z_BACK_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
	case CX2Unit::UC_ARA_LITTLE_HSIEN: // 소선, 제천 State (case UC_ARA_SAKRA_DEVANAM: break 추가하면 안됨)
		{
			stateData.Init();
			stateData.stateID			= ASI_COMBO_ALH_ZZXX;
			m_LuaManager.MakeTableReference( "ASI_COMBO_ALH_ZZXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_COMBO_ALH_ZZXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_COMBO_ALH_ZZXX_LANDING;
			m_LuaManager.MakeTableReference( "ASI_COMBO_ALH_ZZXX_LANDING", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_COMBO_ALH_ZZXX_LANDING_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_DASH_COMBO_ALH_ZZ;
			m_LuaManager.MakeTableReference( "ASI_DASH_COMBO_ALH_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_DASH_COMBO_ALH_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_JUMP_ATTACK_ALH_ZZ;
			m_LuaManager.MakeTableReference( "ASI_JUMP_ATTACK_ALH_ZZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_JUMP_ATTACK_ALH_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			//재빠른 기상
			stateData.Init();
			stateData.stateID			= ASI_P_ALH_QUICK_STAND_FRONT_FRONT;
			m_LuaManager.MakeTableReference( "ASI_P_ALH_QUICK_STAND_FRONT_FRONT", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_P_ALH_QUICK_STAND_FRONT_BACK;
			m_LuaManager.MakeTableReference( "ASI_P_ALH_QUICK_STAND_FRONT_BACK", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_P_ALH_QUICK_STAND_BACK_FRONT;
			m_LuaManager.MakeTableReference( "ASI_P_ALH_QUICK_STAND_BACK_FRONT", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_P_ALH_QUICK_STAND_BACK_BACK;
			m_LuaManager.MakeTableReference( "ASI_P_ALH_QUICK_STAND_BACK_BACK", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
			m_StateList[stateData.stateID] = stateData;	
	
		#pragma region 번득이는 기지
			stateData.Init();
			stateData.stateID			= ASI_P_ASD_QUICKEN_GUARD;
			m_LuaManager.MakeTableReference( "ASI_P_ASD_QUICKEN_GUARD", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_P_ASD_QUICKEN_GUARD_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_P_ASD_QUICKEN_GUARD_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_P_ASD_QUICKEN_GUARD_SMALL;
			m_LuaManager.MakeTableReference( "ASI_P_ASD_QUICKEN_GUARD_SMALL", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_P_ASD_QUICKEN_GUARD_StateStart );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_P_ASD_QUICKEN_GUARD_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		#pragma endregion 

			stateData.Init();
			stateData.stateID			= ASI_STANDUP_ATTACK_FRONT_NO_DOWN;
			m_LuaManager.MakeTableReference( "ASI_STANDUP_ATTACK_FRONT_NO_DOWN", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpAttackEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpAttackEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ASI_STANDUP_ATTACK_BACK_NO_DOWN;
			m_LuaManager.MakeTableReference( "ASI_STANDUP_ATTACK_BACK_NO_DOWN", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpAttackEventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpAttackEnd );
			m_StateList[stateData.stateID] = stateData;

		} break;
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CX2Unit::UC_ARA_YAMA_RAJA:
		{

		#pragma region ASI_DASH_JUMP_COMBO_AYR_XZ
			stateData.Init();
			stateData.stateID			= ASI_DASH_JUMP_COMBO_AYR_XZ;
			m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_AYR_XZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_AYR_XZ_StateStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_AYR_XZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_AYR_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		#pragma endregion

		#pragma region ASI_DASH_COMBO_AYR_ZZ
			stateData.Init();
			stateData.stateID			= ASI_DASH_COMBO_AYR_ZZ;
			m_LuaManager.MakeTableReference( "ASI_DASH_COMBO_AYR_ZZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_DASH_COMBO_AYR_ZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_DASH_COMBO_AYR_ZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		#pragma endregion

		}
	case CX2Unit::UC_ARA_LITTLE_DEVIL:
		{

		#pragma region ASI_COMBO_ALD_ZZXX
			stateData.Init();
			stateData.stateID			= ASI_COMBO_ALD_ZZXX;
			m_LuaManager.MakeTableReference( "ASI_COMBO_ALD_ZZXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_COMBO_ALD_ZZXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_COMBO_ALD_ZZXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		#pragma endregion

		#pragma region ASI_COMBO_ALD_ZZZFrontZ
			stateData.Init();
			stateData.stateID			= ASI_COMBO_ALD_ZZZFrontZ;
			m_LuaManager.MakeTableReference( "ASI_COMBO_ALD_ZZZFrontZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_COMBO_ALD_ZZZFrontZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_COMBO_ALD_ZZZFrontZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		#pragma endregion

		#pragma region ASI_DASH_JUMP_COMBO_ALD_ZX
			stateData.Init();
			stateData.stateID			= ASI_DASH_JUMP_COMBO_ALD_ZX;
			m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_ALD_ZX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_ALD_ZX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_ALD_ZX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		#pragma endregion

		#pragma region ASI_DASH_JUMP_COMBO_ALD_ZXX
			stateData.Init();
			stateData.stateID			= ASI_DASH_JUMP_COMBO_ALD_ZXX;
			m_LuaManager.MakeTableReference( "ASI_DASH_JUMP_COMBO_ALD_ZXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_ALD_ZXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_DASH_JUMP_COMBO_ALD_ZXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		#pragma endregion

		} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	}
}
#pragma endregion

#pragma region InitPassiveSkillState
/** @function	: InitPassiveSkillState
	@brief		: 습득한 패시브 스킬에 따라 초기화
*/
/*virtual*/ void CX2GUAra::InitPassiveSkillState()
{
	CX2GUUser::InitPassiveSkillState();

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	/// AttackResult에 추가될 함수들 설정
	SetAttackResultDelegate();
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

	const CX2SkillTree* const pSkillTree = g_pData->GetSkillTree();

	#pragma region 기공창 숙련
#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	const int iSkillLevelForceSpearMastery = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ARA_FORCE_SPEAR_MASTERY );
	m_fForceSpearDamageRel = 0.f;
	m_usForceSpearPiercing = 0;
	if ( 0 < iSkillLevelForceSpearMastery )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ARA_FORCE_SPEAR_MASTERY,
			iSkillLevelForceSpearMastery );

		if( NULL != pSkillTemplet )
		{
			m_fForceSpearDamageRel = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
			m_usForceSpearPiercing = static_cast<USHORT>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_PIERCING ) );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013	
	#pragma endregion

	switch( GetUnitClass() )
	{
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
		break;
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_SAKRA_DEVANAM: // 제천 전용 State case문 (break 추가하면 안됨)
		{
			#pragma region 번득이는 기지
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillLevelQuickenGuard = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ASD_QUICKEN_GUARD, true );

			m_fTurningRate = m_fBlockingRate = 0.f;
			if ( 0 < iSkillLevelQuickenGuard )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ASD_QUICKEN_GUARD );

				if( NULL != pSkillTemplet )
				{
					m_fTurningRate = m_fBlockingRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevelQuickenGuard );
				}
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			const int iSkillLevelQuickenGuard = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ASD_QUICKEN_GUARD );

			m_fTurningRate = m_fBlockingRate = 0.f;
			if ( 0 < iSkillLevelQuickenGuard )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ASD_QUICKEN_GUARD,
					iSkillLevelQuickenGuard );

				if( NULL != pSkillTemplet )
				{
					m_fTurningRate = m_fBlockingRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE );
				}
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
			#pragma endregion

			#pragma region 창술의 극의
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillLevelBrutalPiercing = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ASD_BRUTAL_PIERCING, true );

			m_bBrutalPiercing = false;
			if ( 0 < iSkillLevelBrutalPiercing )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ASD_BRUTAL_PIERCING );

				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
				{
					m_pBrutalPiercingBuffFactor = pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillLevelBrutalPiercing );

					if ( NULL != m_pBrutalPiercingBuffFactor )
					{
						if ( g_pX2Game != NULL && ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD ) )
							m_pBrutalPiercingBuffFactor->SetRate( pSkillTemplet->m_vecBuffFactorPtr[0]->GetRate( iSkillLevelBrutalPiercing) * 2.f, iSkillLevelBrutalPiercing );

						m_bBrutalPiercing = true;
					}
				}
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			const int iSkillLevelBrutalPiercing = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ASD_BRUTAL_PIERCING );

			m_bBrutalPiercing = false;
			if ( 0 < iSkillLevelBrutalPiercing )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ASD_BRUTAL_PIERCING,
					iSkillLevelBrutalPiercing );

				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
				{
					m_pBrutalPiercingBuffFactor = pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr();

					if ( NULL != m_pBrutalPiercingBuffFactor )
					{
						if ( g_pX2Game != NULL && ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD ) )
							m_pBrutalPiercingBuffFactor->SetRate( pSkillTemplet->m_vecBuffFactorPtr[0]->GetRate() * 2.f );

						m_bBrutalPiercing = true;
					}
				}
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
			#pragma endregion

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			#pragma region SI_P_ASD_FULLMOON_SLASH
			const int iSkillLevelFullMoonSlash = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ASD_FULLMOON_SLASH, true );
			m_fCurrentSpeed = 1.f;
			m_bMoonSlash = false;
			if ( 0 < iSkillLevelFullMoonSlash )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ASD_FULLMOON_SLASH );

				if( NULL != pSkillTemplet )
				{
					m_fCurrentSpeed = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ANI_SPEED, iSkillLevelFullMoonSlash );
					m_bMoonSlash = true;
				}
			}
			#pragma endregion 만월 베기
			
			#pragma region SI_P_ALH_QUICK_STAND
			int iSkillLevelQuickStand = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALH_QUICK_STAND, true );
			if( iSkillLevelQuickStand > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_QUICK_STAND );
				if( NULL != pSkillTemplet )
				{
					const float fMoveSpeed = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MOVE_SPEED, iSkillLevelQuickStand );
#ifdef FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정
					m_AdditionalWalkSpeed.SetPassiveMultiple( m_AdditionalWalkSpeed.GetPassiveMultiple() * fMoveSpeed );
					m_AdditionalDashSpeed.SetPassiveMultiple( m_AdditionalDashSpeed.GetPassiveMultiple() * fMoveSpeed );
#else // FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정
					m_PhysicParam.fWalkSpeed *= fMoveSpeed;
					m_PhysicParam.fRunSpeed *= fMoveSpeed;
#endif // FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정

				}
			}
			#pragma endregion 재빠른 기상
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		
	case CX2Unit::UC_ARA_LITTLE_HSIEN:  // 소선, 제천 State (case UC_ARA_SAKRA_DEVANAM: break 추가하면 안됨)
		{
#ifndef SERV_ARA_CHANGE_CLASS_SECOND // 김태환 ( 재빠른 기상은 소마, 명왕도 사용하기 때문에, 따로 분리 )
			//재빠른 기상
			m_bAbleQuickStand = false;
			int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALH_QUICK_STAND, true );
			if( iSkillLevel > 0 )
			{
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_QUICK_STAND );

				if( NULL != pSkillTemplet )
				{
					m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent += 
						static_cast<int>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel ) * 100.f );
				}
		#else // UPGRADE_SKILL_SYSTEM_2013
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_QUICK_STAND, iSkillLevel );

				if( NULL != pSkillTemplet )
				{
					m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent += 
						static_cast<int>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE ) * 100.f );
				}
		#endif // UPGRADE_SKILL_SYSTEM_2013
				m_bAbleQuickStand = true;
			}
#else // SERV_ARA_CHANGE_CLASS_SECOND
			int iSkillLevel = 0;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

			//충만한 힘
			m_iGainMpByFpConversion = 0;
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALH_FILLED_POWER, true );

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_FILLED_POWER );

				if( NULL != pSkillTemplet )
				{
					m_iGainMpByFpConversion = static_cast<int>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillLevel ) );
				}
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALH_FILLED_POWER );

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_FILLED_POWER, iSkillLevel );

				if( NULL != pSkillTemplet )
				{
					m_iGainMpByFpConversion = static_cast<int>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL ) );
				}
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013

			//이화접목

			SetCanAvoidance( (0 < userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ALH_BREAKING )) ? true : false );

			#pragma region 기력 활성화
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillLevelVigorEnergize = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALH_VIGOR_ENERGIZE, true );

			if ( 0 < iSkillLevelVigorEnergize )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_VIGOR_ENERGIZE  );

				if( NULL != pSkillTemplet )
				{
					// 디버프 지속시간 감소
					m_fReduceEDTDurationRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REDUCE_EDT_TIME_REL, iSkillLevelVigorEnergize );
				}
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			const int iSkillLevelVigorEnergize = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALH_VIGOR_ENERGIZE );

			if ( 0 < iSkillLevelVigorEnergize )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_VIGOR_ENERGIZE,
					iSkillLevelVigorEnergize );

				if( NULL != pSkillTemplet )
				{
					// 디버프 지속시간 감소
					m_fReduceEDTDurationRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REDUCE_EDT_TIME_REL );
				}
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
			#pragma endregion
		}
		break;
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		case CX2Unit::UC_ARA_YAMA_RAJA:
			{
				#pragma region 늑대의 울부짖음
				const int iSkillLevelHowling = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AYR_HOWLING, true );

				m_fHowlingDamageRel 	= 1.0f;
				m_fHowlingActiveRate	= 0.f;

				if ( 0 < iSkillLevelHowling )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AYR_HOWLING );

					if( NULL != pSkillTemplet )
					{

						/// 늑대의 울부짖음 증가 데미지 설정
						/// 아래 변수를 선언해서 한 것은 삭제 예정 (아직 SkillTemplet에 해당 키워드가 추가되지 않아서 임시로 이렇게 했음)
						const float fDamageRel = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevelHowling );
						m_fHowlingDamageRel		= ( fDamageRel > 1.0f ? fDamageRel : 1.0f );
						/// 늑대의 울부짖음 발동 확률
						m_fHowlingActiveRate	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevelHowling );
					}
				}
				#pragma endregion
			}

		case CX2Unit::UC_ARA_LITTLE_DEVIL:	// 소마, 명왕 State (case UC_ARA_YAMA_RAJA: break 추가하면 안됨)
			{
				#pragma region 기탄 강화
				const int iSkillLevelEnergyBulletReinforce = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AYR_ENERGY_BULLET_REINFORCE, true );

				m_fIncreaseEnergeBulletSize		= 1.f;		/// 기탄 크기
				m_fIncreaseEnergeBulletDamage	= 1.f;		/// 기탄 데미지

				if ( 0 < iSkillLevelEnergyBulletReinforce )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AYR_ENERGY_BULLET_REINFORCE );

					if( NULL != pSkillTemplet )
					{
						/// 기탄 크기 증가
						m_fIncreaseEnergeBulletSize		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillLevelEnergyBulletReinforce );
						/// 기탄 데미지 증가
						m_fIncreaseEnergeBulletDamage	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevelEnergyBulletReinforce );
					}
				}
				#pragma endregion

				#pragma region 기력 강화
				const int iSkillLevelEnergeReinforce = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALD_ENERGY_REINFORCE, true );

				m_fIncreaseEnergeBulletRange			= 1.f;	/// 기탄 사거리		
				m_fIncreaseEnergeBulletForceDownValue	= 0.f;	/// 기탄 다운 수치


				if ( 0 < iSkillLevelEnergeReinforce )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALD_ENERGY_REINFORCE );

					if( NULL != pSkillTemplet )
					{
						/// 기탄 사거리 증가
						m_fIncreaseEnergeBulletRange			= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SCOPE_ABS, iSkillLevelEnergeReinforce );
						/// 기탄 다운 수치 감소
						m_fIncreaseEnergeBulletForceDownValue	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_ABS, iSkillLevelEnergeReinforce );
					}
				}
				#pragma endregion
				
				#pragma region 지옥 초래
				const int iSkillLevelHellGateOpen = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AYR_HELL_GATE_OPEN, true );

				m_fOpenTheHellIncreaseDamage = 1.f;		/// 공격력 증가치

				if ( 0 < iSkillLevelHellGateOpen )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AYR_HELL_GATE_OPEN );

					if( NULL != pSkillTemplet )
					{
						/// 공격력 증가 설정
						m_fOpenTheHellIncreaseDamage	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevelHellGateOpen );
					}
				}
				#pragma endregion
			} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	}


#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환 ( 재빠른 기상은 소마, 명왕도 사용하기 때문에, 따로 분리 )

	/// 전직 공통 사용 스킬
#pragma region 재빠른 기상
	m_bAbleQuickStand = false;
	int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALH_QUICK_STAND, true );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ALH_QUICK_STAND );

		if( NULL != pSkillTemplet )
		{
			m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent += 
				static_cast<int>(pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel ) * 100.f );
		}
		m_bAbleQuickStand = true;
	}
#pragma endregion

#endif // SERV_ARA_CHANGE_CLASS_SECOND


#ifdef ARA_CHANGE_CLASS_FIRST

#endif
}
#pragma endregion

#pragma region SetEquippedSkillFunc
/** @function	: SetEquippedSkillFunc
	@brief		: 슬롯에 장착한 스킬에 따라 해당 스킬 스테이트 초기화
*/
/*virtual*/ void CX2GUAra::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_,
																 const bool bSlotB_ )
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

	const CX2SkillTree::SkillTemplet* pSkillTempletEquipped = GetEquippedActiveSkillTemplet( eSkillID_ );
	if( NULL == pSkillTempletEquipped )
		return;

	switch( pSkillTempletEquipped->m_eType )
	{	
	#pragma region 버프 스킬
	case CX2SkillTree::ST_BUFF:
		{	// 버프 필살기
			m_LuaManager.MakeTableReference( "ASI_COMMON_BUFF", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, COMMON_BUFF_FrameMove );	
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	

			m_LuaManager.MakeTableReference( "ASI_COMMON_BUFF", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

			return;
		} break;
	#pragma endregion
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
					m_LuaManager.MakeTableReference( "ASI_THROW_ITEM", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	

					m_LuaManager.MakeTableReference( "ASI_THROW_ITEM", hyperStateData.stateID );
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

	#pragma region ACTIvE, SPECIAL ACTIvE 스킬
	switch ( eSkillID_ )
	{
	default:
	#pragma region SI_SA_ARA_WIND_WEDGE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AM_WIND_WEDGE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_WIND_WEDGE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			ASI_SA_ARA_WIND_WEDGE_START_CreateState( normalStateData, normalStateData.stateID );
			ASI_SA_ARA_WIND_WEDGE_START_CreateState( hyperStateData, hyperStateData.stateID );
			
			UserUnitStateData	subStateData;
			ASI_SA_ARA_WIND_WEDGE_END_CreateState( subStateData, ASI_SA_ARA_WIND_WEDGE_END );
			m_StateList[subStateData.stateID] = subStateData;
		} break;
	#pragma endregion

	#pragma region SI_SA_ARA_SHADOW_RISING
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AM_SHADOW_RISING:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_SHADOW_RISING_JUMP:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			ASI_SA_ARA_SHADOW_RISING_JUMP_CreateState( normalStateData, normalStateData.stateID );
			ASI_SA_ARA_SHADOW_RISING_JUMP_CreateState( hyperStateData, hyperStateData.stateID );

			{
				UserUnitStateData	subStateData;
				ASI_SA_ARA_SHADOW_RISING_LANDING_CreateState( subStateData, ASI_SA_ARA_SHADOW_RISING_LANDING );
				m_StateList[subStateData.stateID] = subStateData;
			}

			{
				UserUnitStateData	subStateData;
				ASI_SA_ARA_SHADOW_RISING_END_CreateState( subStateData, ASI_SA_ARA_SHADOW_RISING_END );
				m_StateList[subStateData.stateID] = subStateData;
			}
			
		} break;
	#pragma endregion

	#pragma region SI_SA_ARA_PULLING_PIERCE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_AM_PULLING_PIERCE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_PULLING_PIERCE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			ASI_SA_ARA_PULLING_PIERCE_START_CreateState( normalStateData, normalStateData.stateID );
			ASI_SA_ARA_PULLING_PIERCE_START_CreateState( hyperStateData, hyperStateData.stateID );

			{
				UserUnitStateData	subStateData;
				ASI_SA_ARA_PULLING_PIERCE_END_CreateState( subStateData, ASI_SA_ARA_PULLING_PIERCE_END );
				m_StateList[subStateData.stateID] = subStateData;
			}
		} break;
	#pragma endregion

	#pragma region SI_SA_ARA_TEMPEST_DANCE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_AM_TEMPEST_DANCE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_TEMPEST_DANCE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "ASI_SA_ARA_TEMPEST_DANCE_START", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_TEMPEST_DANCE_START_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_TEMPEST_DANCE_START_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_TEMPEST_DANCE_START_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_SA_ARA_TEMPEST_DANCE_START", hyperStateData.stateID );
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ASI_SA_ARA_TEMPEST_DANCE_END;
				m_LuaManager.MakeTableReference( "ASI_SA_ARA_TEMPEST_DANCE_END", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_TEMPEST_DANCE_END_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_TEMPEST_DANCE_END_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_TEMPEST_DANCE_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;

			}
		} break;
	#pragma endregion

	#pragma region SI_SA_ARA_FALLING_DRAGON
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_AM_FALLING_DRAGON:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_FALLING_DRAGON:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "ASI_SA_ARA_FALLING_DRAGON", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_FALLING_DRAGON_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_FALLING_DRAGON_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_FALLING_DRAGON_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_SA_ARA_FALLING_DRAGON", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion

	#pragma region SI_SA_ARA_SHADOW_KNOT
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_AM_SHADOW_KNOT:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_SHADOW_KNOT:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_KNOT_START", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_START_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_START_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_START_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_START_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_KNOT_START", hyperStateData.stateID );
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ASI_SA_ARA_SHADOW_KNOT_WAIT;
				m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_KNOT_WAIT", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_WAIT_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_WAIT_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_WAIT_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_WAIT_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ASI_SA_ARA_SHADOW_KNOT_CATCH;
				m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_KNOT_CATCH", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_CATCH_Init );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_CATCH_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ASI_SA_ARA_SHADOW_KNOT_END;
				m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_KNOT_END", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_END_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_END_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_KNOT_END_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	#pragma endregion
#ifdef ARA_CHANGE_CLASS_FIRST
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_ALH_DOUBLE_COLLISION: //용아 3식 - 팔괘장
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ALH_DOUBLE_COLLISION: //용아 3식 - 팔괘장
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			ASI_SA_ALH_DOUBLE_COLLISION_START_CreateState( normalStateData, normalStateData.stateID );
			ASI_SA_ALH_DOUBLE_COLLISION_START_CreateState( hyperStateData, hyperStateData.stateID );

			{
				UserUnitStateData	subStateData;

				ASI_SA_ALH_DOUBLE_COLLISION_END_CreateState( subStateData, ASI_SA_ALH_DOUBLE_COLLISION_END );
				m_StateList[subStateData.stateID] = subStateData;			 	
			}
		} break;
	
	#pragma region SI_SA_ASD_TURBULENT_WAVE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_ASD_TURBULENT_WAVE: // 맹호 4식 - 노도
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ASD_TURBULENT_WAVE: // 맹호 4식 - 노도
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			ASI_SA_ASD_TURBULENT_WAVE_START_CreateState( normalStateData, normalStateData.stateID );
			ASI_SA_ASD_TURBULENT_WAVE_START_CreateState( hyperStateData, hyperStateData.stateID );
			
			{
				UserUnitStateData subStateData;
				ASI_SA_ASD_TURBULENT_WAVE_LOOP_CreateState( subStateData, ASI_SA_ASD_TURBULENT_WAVE_LOOP );
				m_StateList[subStateData.stateID] = subStateData;
			}

			{
				UserUnitStateData subStateData;
				ASI_SA_ASD_TURBULENT_WAVE_END_CreateState( subStateData, ASI_SA_ASD_TURBULENT_WAVE_END );
				m_StateList[subStateData.stateID] = subStateData;
			}			
		} break;
	#pragma endregion 

	#pragma region SI_SA_ASD_SUPPRESSION
	case CX2SkillTree::SI_SA_ASD_SUPPRESSION: // 아라 2차 전직 제천 - 제압
		{
			m_LuaManager.MakeTableReference( "ASI_SA_ASD_SUPPRESSION", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_SUPPRESSION_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_SUPPRESSION_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_SUPPRESSION_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_SUPPRESSION_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_SA_ASD_SUPPRESSION", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion

	#pragma region SI_SA_ALH_MOONLIGHT_SLASH
	case CX2SkillTree::SI_SA_ALH_MOONLIGHT_SLASH:	// 달빛 베기
		{
			ASI_SA_ALH_MOONLIGHT_SLASH_READY_CreateState( normalStateData, normalStateData.stateID );
			ASI_SA_ALH_MOONLIGHT_SLASH_READY_CreateState( hyperStateData, hyperStateData.stateID );

			{
				UserUnitStateData subStateData;
				ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_CreateState( subStateData, ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE );
				m_StateList[subStateData.stateID] = subStateData;
			}

			{
				UserUnitStateData subStateData;
				ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_CreateState( subStateData, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK );
				m_StateList[subStateData.stateID] = subStateData;
			}

			{
				UserUnitStateData subStateData;
				ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_CreateState( subStateData, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK );
				m_StateList[subStateData.stateID] = subStateData;
			}

		} break;
	#pragma endregion

		case CX2SkillTree::SI_SA_ALH_WHITE_TIGER: // 맹호격
		{
			m_LuaManager.MakeTableReference( "ASI_SA_ALH_POWER_OF_WHITER_TIGER", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_POWER_OF_WHITER_TIGER_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_POWER_OF_WHITER_TIGER_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_POWER_OF_WHITER_TIGER_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_POWER_OF_WHITER_TIGER_End );

			m_LuaManager.MakeTableReference( "ASI_SA_ALH_POWER_OF_WHITER_TIGER", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
			hyperStateData.StateEnd			= normalStateData.StateEnd;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING;
				m_LuaManager.MakeTableReference( "ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
		case CX2SkillTree::SI_SA_ALH_PEERLESSNESS_SLASH: // 무쌍참
			{
				m_LuaManager.MakeTableReference( "ASI_SA_ALH_PEERLESSNESS_SLASH_START", normalStateData.stateID );
				normalStateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_PEERLESSNESS_SLASH_START_Start );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_PEERLESSNESS_SLASH_START_Init );
				normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_PEERLESSNESS_SLASH_START_FrameMove );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_PEERLESSNESS_SLASH_START_EventProcess );
				normalStateData.StateEnd		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_PEERLESSNESS_SLASH_START_End );

				m_LuaManager.MakeTableReference( "ASI_SA_ALH_PEERLESSNESS_SLASH_START", hyperStateData.stateID );
				hyperStateData.stateID			= normalStateData.stateID;
				hyperStateData.StateStart		= normalStateData.StateStart;
				hyperStateData.StateInit		= normalStateData.StateInit;
				hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
				hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
				hyperStateData.StateEnd			= normalStateData.StateEnd;

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= ASI_SA_ALH_PEERLESSNESS_SLASH_END;
					m_LuaManager.MakeTableReference( "ASI_SA_ALH_PEERLESSNESS_SLASH_END", stateData.stateID );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_PEERLESSNESS_SLASH_END_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_PEERLESSNESS_SLASH_END_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}
			} break;

	#pragma region SI_SA_ALH_SWALLOW_ASSAULT
		case CX2SkillTree::SI_SA_ALH_SWALLOW_ASSAULT: // 비연
			{
				m_LuaManager.MakeTableReference( "ASI_SA_ALH_SWALLOW_ASSAULT_READY", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Ready_Init );
				normalStateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateStart );
				normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Ready_FrameMove );
				normalStateData.StateEnd		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateEnd );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Ready_EventProcess );

				m_LuaManager.MakeTableReference( "ASI_SA_ALH_SWALLOW_ASSAULT_READY", hyperStateData.stateID );
				hyperStateData.StateInit		= normalStateData.StateInit;
				hyperStateData.StateStart		= normalStateData.StateStart;
				hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
				hyperStateData.StateEnd			= normalStateData.StateEnd;
				hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

				{
					UserUnitStateData stateData;

					stateData.Init();
					stateData.stateID			= ASI_SA_ALH_SWALLOW_ASSAULT_ATTACK;
					m_LuaManager.MakeTableReference( "ASI_SA_ALH_SWALLOW_ASSAULT_ATTACK", stateData.stateID );
					stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Attack_Init );
					stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Attack_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Attack_EventProcess );
					stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateEnd );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= ASI_SA_ALH_SWALLOW_ASSAULT_FINISH;
					m_LuaManager.MakeTableReference( "ASI_SA_ALH_SWALLOW_ASSAULT_FINISH", stateData.stateID );
					stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Finish_Init );
					stateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Finish_StateStart );
					stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Finish_FrameMove );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Finish_EventProcess );
					m_StateList[stateData.stateID] = stateData;

					stateData.Init();
					stateData.stateID			= ASI_SA_ALH_SWALLOW_ASSAULT_ESCAPE;
					m_LuaManager.MakeTableReference( "ASI_SA_ALH_SWALLOW_ASSAULT_ESCAPE", stateData.stateID );
					stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_SWALLOW_ASSAULT_Escape_EventProcess );
					m_StateList[stateData.stateID] = stateData;
				}

#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR 
				if ( 0 > m_sLockOnSystemPtr.use_count() )
					m_sLockOnSystemPtr.reset();
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR 

				m_sLockOnSystemPtr = SLockOnSystem::CreateLockOnSystem();
			} break;
	#pragma endregion 
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef FINALITY_SKILL_SYSTEM //김창한
		#pragma region SI_HA_ASD_THOUSANDS_BLOSSOMS
		case CX2SkillTree::SI_HA_ASD_THOUSANDS_BLOSSOMS: //아라 제천 궁극기 - 분기 등천
		{
			m_LuaManager.MakeTableReference( "ASI_HA_ASD_THOUSANDS_BLOSSOMS", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_HA_ASD_THOUSANDS_BLOSSOMS_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_HA_ASD_THOUSANDS_BLOSSOMS_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_HA_ASD_THOUSANDS_BLOSSOMS", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
		#pragma endregion 
#endif //FINALITY_SKILL_SYSTEM

	#pragma region SI_A_ARA_TIGER_CLAW
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			case CX2SkillTree::SI_A_AM_TIGER_CLAW:
#else //UPGRADE_SKILL_SYSTEM_2013
			case CX2SkillTree::SI_A_ARA_TIGER_CLAW:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			ASI_A_ARA_TIGER_CLAW_START_CreateState( normalStateData, normalStateData.stateID );
			ASI_A_ARA_TIGER_CLAW_START_CreateState( hyperStateData, hyperStateData.stateID );
			
			UserUnitStateData subStateData;
			ASI_A_ARA_TIGER_CLAW_END_CreateState( subStateData, ASI_A_ARA_TIGER_CLAW_END );
			m_StateList[subStateData.stateID] = subStateData;
		} break;
	#pragma endregion

	#pragma region SI_A_ARA_GAIN_FORCE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AM_GAIN_FORCE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_ARA_GAIN_FORCE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "ASI_A_ARA_GAIN_FORCE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_GAIN_FORCE_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_GAIN_FORCE_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_GAIN_FORCE_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_A_ARA_GAIN_FORCE", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion

	#pragma region SI_A_ARA_FORCE_SPEAR
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AM_FORCE_SPEAR:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_ARA_FORCE_SPEAR:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "ASI_A_ARA_FORCE_SPEAR", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_FORCE_SPEAR_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_FORCE_SPEAR_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_FORCE_SPEAR_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_A_ARA_FORCE_SPEAR", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateStartFuture;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion

#ifdef ARA_CHANGE_CLASS_FIRST 
	case CX2SkillTree::SI_A_ALH_BREAKING: //이화접목
		{
			m_LuaManager.MakeTableReference( "ASI_A_ALH_BREAKING", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_BREAKING_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_BREAKING_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_BREAKING_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_A_ALH_BREAKING", hyperStateData.stateID );
			hyperStateData.StateInit		= normalStateData.StateInit;
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
			hyperStateData.StateEnd			= normalStateData.StateEnd;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ASI_A_ALH_BREAKING_ATTACK;
				m_LuaManager.MakeTableReference( "ASI_A_ALH_BREAKING_ATTACK", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_BREAKING_ATTACK_Init );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_BREAKING_ATTACK_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_BREAKING_ATTACK_EventProcess );
				stateData.StateEndFuture	= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_BREAKING_ATTACK_EndFuture );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ASI_A_ALH_BREAKING_AVOIDANCE;
				m_LuaManager.MakeTableReference( "ASI_A_ALH_BREAKING_AVOIDANCE", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, StandUpRollingEventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GUAra, StandUpRollingEnd );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

	#pragma region SI_A_ALH_ROCK_CRASH
	case CX2SkillTree::SI_A_ALH_ROCK_CRASH:
		{
			ASI_A_ALH_ROCK_CRASH_FALLING_START_CreateState( normalStateData, normalStateData.stateID );
			ASI_A_ALH_ROCK_CRASH_FALLING_START_CreateState( hyperStateData, hyperStateData.stateID );
			
			UserUnitStateData subStateData;
			ASI_A_ALH_ROCK_CRASH_END_CreateState( subStateData, ASI_A_ALH_ROCK_CRASH_END );
			m_StateList[subStateData.stateID] = subStateData;
			
		} break;
	#pragma endregion 바위 깨기

	#pragma region SI_A_ASD_LOW_BRANDISH
	case CX2SkillTree::SI_A_ASD_LOW_BRANDISH:
		{
			ASI_A_ASD_LOW_BRANDISH_SLIDING_CreateState( normalStateData, normalStateData.stateID );
			ASI_A_ASD_LOW_BRANDISH_SLIDING_CreateState( hyperStateData, hyperStateData.stateID );

			UserUnitStateData subStateData;
			ASI_A_ASD_LOW_BRANDISH_KICK_CreateState( subStateData, ASI_A_ASD_LOW_BRANDISH_KICK );
			m_StateList[subStateData.stateID] = subStateData;
		} break;
	#pragma endregion 지축
#endif	//ARA_CHANGE_CLASS_FIRST

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	#pragma region SI_A_ARA_QUICK_PIERCING
	case CX2SkillTree::SI_A_AM_QUICK_PIERCING:
		{
			m_LuaManager.MakeTableReference( "ASI_A_ARA_QUICK_PIERCING", normalStateData.stateID );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_Quick_Piercing_EventProcess );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_Quick_Piercing_FrameMove );

			m_LuaManager.MakeTableReference( "ASI_A_ARA_QUICK_PIERCING", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
		} break;
	#pragma endregion 신속

	#pragma region SI_A_ARA_SHOULDER_BLOW
	case CX2SkillTree::SI_A_AM_SHOULDER_BLOW:
		{
			m_LuaManager.MakeTableReference( "ASI_A_ARA_SHOULDER_BLOW", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_Shoulder_Blow_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_Shoulder_Blow_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_Shoulder_Blow_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_A_ARA_SHOULDER_BLOW", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion 철산고

	#pragma region SI_A_ASD_DRAGON_FALLING
	case CX2SkillTree::SI_A_ASD_DRAGON_FALLING:
		{
			m_LuaManager.MakeTableReference( "ASI_A_ASD_FALLING_BLOSSOM_Start", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_Falling_Blossom_Start_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_Falling_Blossom_Start_StateStart );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_Falling_Blossom_Start_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_A_ASD_FALLING_BLOSSOM_Start", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ASI_A_ASD_FALLING_BLOSSOM_FINISH;
				m_LuaManager.MakeTableReference( "ASI_A_ASD_FALLING_BLOSSOM_Finish", stateData.stateID );
				stateData.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_Falling_Blossom_Finish_Init );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_Falling_Blossom_Finish_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	#pragma endregion 낙화

	#pragma region SI_A_ARA_QUICK_STAB
	case CX2SkillTree::SI_A_AM_QUICK_STAB:
		{
			m_LuaManager.MakeTableReference( "ASI_A_ARA_QUICK_STAB", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_Quick_Stab_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_Quick_Stab_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_Quick_Stab_EventProcess );

			m_LuaManager.MakeTableReference( "ASI_A_ARA_QUICK_STAB", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion 천격
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	#pragma region SI_A_ALD_WOLF_CLAW
	case CX2SkillTree::SI_A_ALD_WOLF_CLAW:
		{
			ASI_A_ALD_WOLF_CLAW_CreateState( normalStateData, normalStateData.stateID );
			ASI_A_ALD_WOLF_CLAW_CreateState( hyperStateData, hyperStateData.stateID );

			UserUnitStateData subStateData;
			ASI_A_ALD_WOLF_CLAW_FINISH_CreateState( subStateData, ASI_A_ALD_WOLF_CLAW_FINISH );
			m_StateList[subStateData.stateID] = subStateData;
		} break;
	#pragma endregion 늑대발톱

	#pragma region SI_A_ALD_CONVERSION_HEALTH
		case CX2SkillTree::SI_A_ALD_CONVERSION_HEALTH:
		{
			ASI_A_ALD_CONVERSION_HEALTH_CreateState( normalStateData, normalStateData.stateID );
			ASI_A_ALD_CONVERSION_HEALTH_CreateState( hyperStateData, hyperStateData.stateID );
		} break;
	#pragma endregion 이혈공

	#pragma region SI_A_ALD_CONVERSION_MANA
		case CX2SkillTree::SI_A_ALD_CONVERSION_MANA:
			{
				ASI_A_ALD_CONVERSION_MANA_CreateState( normalStateData, normalStateData.stateID );
				ASI_A_ALD_CONVERSION_MANA_CreateState( hyperStateData, hyperStateData.stateID );
			} break;
	#pragma endregion 이기공

	#pragma region SI_A_AYR_HIGH_SPEED
		case CX2SkillTree::SI_A_AYR_HIGH_SPEED:
			{
				ASI_A_AYR_HIGH_SPEED_READY_CreateState( normalStateData, normalStateData.stateID );
				ASI_A_AYR_HIGH_SPEED_READY_CreateState( hyperStateData, hyperStateData.stateID );

				UserUnitStateData subStateData;
				ASI_A_AYR_HIGH_SPEED_CreateState( subStateData, ASI_A_AYR_HIGH_SPEED );
				m_StateList[subStateData.stateID] = subStateData;
			} break;
	#pragma endregion 쾌속

	#pragma region SI_SA_ALD_ENERGY_WAVE
		case CX2SkillTree::SI_SA_ALD_ENERGY_WAVE:
			{
				ASI_SA_ALD_ENERGY_WAVE_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_ALD_ENERGY_WAVE_CreateState( hyperStateData, hyperStateData.stateID );
			} break;
	#pragma endregion 기공파

	#pragma region SI_SA_ALD_HELL_OF_TORNADO
		case CX2SkillTree::SI_SA_ALD_HELL_OF_TORNADO:
			{
				ASI_SA_ALD_HELL_OF_TORNADO_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_ALD_HELL_OF_TORNADO_CreateState( hyperStateData, hyperStateData.stateID );

				UserUnitStateData subStateData;
				ASI_SA_ALD_HELL_OF_TORNADO_FINISH_CreateState( subStateData, ASI_SA_ALD_HELL_OF_TORNADO_FINISH );
				m_StateList[subStateData.stateID] = subStateData;
			} break;
	#pragma endregion 연환지옥

	#pragma region SI_SA_ALD_WOLF_SPEAR
		case CX2SkillTree::SI_SA_ALD_WOLF_SPEAR:
			{
				ASI_SA_ALD_WOLF_SPEAR_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_ALD_WOLF_SPEAR_CreateState( hyperStateData, hyperStateData.stateID );

				UserUnitStateData subStateData;
				ASI_SA_ALD_WOLF_SPEAR_FINISH_CreateState( subStateData, ASI_SA_ALD_WOLF_SPEAR_FINISH );
				m_StateList[subStateData.stateID] = subStateData;
			} break;
	#pragma endregion 늑대이빨

	#pragma region SI_SA_ALD_FINGER_BULLET
		case CX2SkillTree::SI_SA_ALD_FINGER_BULLET:
			{
				ASI_SA_ALD_FINGER_BULLET_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_ALD_FINGER_BULLET_CreateState( hyperStateData, hyperStateData.stateID );

				UserUnitStateData subStateData;
				ASI_SA_ALD_FINGER_BULLET_FINISH_CreateState( subStateData, ASI_SA_ALD_FINGER_BULLET_FINISH );
				m_StateList[subStateData.stateID] = subStateData;
			} break;
	#pragma endregion 나찰 1식 - 흡혼

	#pragma region SI_SA_ALD_ENERGY_BEAD
		case CX2SkillTree::SI_SA_ALD_ENERGY_BEAD:
			{
				ASI_SA_ALD_ENERGY_BEAD_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_ALD_ENERGY_BEAD_CreateState( hyperStateData, hyperStateData.stateID );

				UserUnitStateData subStateData;
				ASI_SA_ALD_ENERGY_BEAD_FINISH_CreateState( subStateData, ASI_SA_ALD_ENERGY_BEAD_FINISH );
				m_StateList[subStateData.stateID] = subStateData;
			} break;
	#pragma endregion 나찰 2식 - 연환장

	#pragma region SI_SA_ALD_PRISON_SPEAR
		case CX2SkillTree::SI_SA_ALD_PRISON_SPEAR:
			{
				ASI_SA_ALD_PRISON_SPEAR_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_ALD_PRISON_SPEAR_CreateState( hyperStateData, hyperStateData.stateID );

				UserUnitStateData subStateData;
				ASI_SA_ALD_PRISON_SPEAR_FINISH_CreateState( subStateData, ASI_SA_ALD_PRISON_SPEAR_FINISH );
				m_StateList[subStateData.stateID] = subStateData;
			} break;
	#pragma endregion 나찰 3식 - 창살감옥

	#pragma region SI_A_AYR_STEAL_SOUL
		case CX2SkillTree::SI_A_AYR_STEAL_SOUL:
			{
				ASI_A_AYR_STEAL_SOUL_CreateState( normalStateData, normalStateData.stateID );
				ASI_A_AYR_STEAL_SOUL_CreateState( hyperStateData, hyperStateData.stateID );

				UserUnitStateData subStateData;
				ASI_A_AYR_STEAL_SOUL_FINISH_CreateState( subStateData, ASI_A_AYR_STEAL_SOUL_FINISH );
				m_StateList[subStateData.stateID] = subStateData;
			} break;
	#pragma endregion 나찰 4식 - 탈혼

	#pragma region SI_SA_AYR_ENERGY_VOID
		case CX2SkillTree::SI_SA_AYR_ENERGY_VOID:
			{
				ASI_SA_AYR_ENERGY_VOID_START_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_AYR_ENERGY_VOID_START_CreateState( hyperStateData, hyperStateData.stateID );
				{
					UserUnitStateData subStateData;
					subStateData.Init();
					ASI_SA_AYR_ENERGY_VOID_LOOP_CreateState( subStateData, ASI_SA_AYR_ENERGY_VOID_LOOP );
					m_StateList[subStateData.stateID] = subStateData;
					
					subStateData.Init();
					ASI_SA_AYR_ENERGY_VOID_END_CreateState( subStateData, ASI_SA_AYR_ENERGY_VOID_END );
					m_StateList[subStateData.stateID] = subStateData;
				}
			}
			break;
	#pragma endregion 초진공장

	#pragma region SI_SA_AYR_SUPPRESSION_ENERGY
		case CX2SkillTree::SI_SA_AYR_SUPPRESSION_ENERGY:
			{
				ASI_SA_AYR_SUPPRESSION_ENERGY_CreateState( normalStateData, normalStateData.stateID );
				ASI_SA_AYR_SUPPRESSION_ENERGY_CreateState( hyperStateData, hyperStateData.stateID );
			} break;
	#pragma endregion 제압 - 기

	#pragma region SI_SA_AYR_SUPPRESSION_ENERGY
		case CX2SkillTree::SI_HA_AYR_ENERGY_CANNON:
			{
				ASI_HA_AYR_ENERGY_CANNON_CreateState( normalStateData, normalStateData.stateID );
				ASI_HA_AYR_ENERGY_CANNON_CreateState( hyperStateData, hyperStateData.stateID );
			} break;
	#pragma endregion 대규환지옥

#pragma region SI_A_ALD_REFLECTION
		case CX2SkillTree::SI_A_ALD_REFLECTION:
			{
				// 20130812 // 김현철 // 반탄공은 스테이트가 없어서 사운드 레디를 이곳에서 시킴
				SoundReadyInBackground( L"Elsword_Endurance.ogg" );
			} break;
#pragma endregion 반탄공
			
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	}
	
	#pragma endregion 
}
#pragma endregion

#pragma region SetEquippedSkillLevel
/** @function	: SetEquippedSkillLevel
	@brief		: 각 캐릭터의 스킬 슬롯 스테이트의 스킬ID, 스킬 레벨, 파워레이트 등을 스테이트 데이터에 초기화\n
				  특정 스킬이 두개 이상의 스테이트로 구성된 경우 이곳에 추가하는 것을 고려해야함
	@param		: const CX2SkillTree::SKILL_ID eSkillID_ (변경할 스킬의 ID), const bool bChangeAll_ (true일 경우 모든 스킬 변경)
*/
/*virtual*/ void CX2GUAra::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AM_WIND_WEDGE == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ARA_WIND_WEDGE == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AM_WIND_WEDGE );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ARA_WIND_WEDGE );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ARA_WIND_WEDGE_END );
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AM_SHADOW_RISING == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ARA_SHADOW_RISING_JUMP == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AM_SHADOW_RISING );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ARA_SHADOW_RISING_JUMP );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ARA_SHADOW_RISING_LANDING );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ARA_SHADOW_RISING_END );
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_AM_PULLING_PIERCE == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ARA_PULLING_PIERCE == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AM_PULLING_PIERCE );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ARA_PULLING_PIERCE );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ARA_PULLING_PIERCE_END );
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AM_TIGER_CLAW == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ARA_TIGER_CLAW == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AM_TIGER_CLAW );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ARA_TIGER_CLAW );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ARA_TIGER_CLAW_START );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ARA_TIGER_CLAW_END );
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_AM_TEMPEST_DANCE == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ARA_TEMPEST_DANCE == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AM_TEMPEST_DANCE );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ARA_TEMPEST_DANCE );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ARA_TEMPEST_DANCE_END );
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_AM_SHADOW_KNOT == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ARA_SHADOW_KNOT == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AM_SHADOW_KNOT );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ARA_SHADOW_KNOT );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ARA_SHADOW_KNOT_END );
	}
#ifdef ARA_CHANGE_CLASS_FIRST
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ALH_DOUBLE_COLLISION == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALH_DOUBLE_COLLISION == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ALH_DOUBLE_COLLISION );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALH_DOUBLE_COLLISION );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_DOUBLE_COLLISION_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ALH_BREAKING == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ALH_BREAKING );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ALH_BREAKING_ATTACK );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ALH_ROCK_CRASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ALH_ROCK_CRASH );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ALH_ROCK_CRASH_START );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ALH_ROCK_CRASH_END );
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ASD_TURBULENT_WAVE == eSkillID_ )
#else //UPGRADE_SKILL_SYSTEM_2013
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ASD_TURBULENT_WAVE == eSkillID_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ASD_TURBULENT_WAVE );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ASD_TURBULENT_WAVE );
#endif //UPGRADE_SKILL_SYSTEM_2013

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ASD_TURBULENT_WAVE_START );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ASD_TURBULENT_WAVE_LOOP );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ASD_TURBULENT_WAVE_END );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ASD_VOID_BREAK );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALH_WHITE_TIGER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALH_WHITE_TIGER );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALH_PEERLESSNESS_SLASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALH_PEERLESSNESS_SLASH );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_PEERLESSNESS_SLASH_END );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALH_MOONLIGHT_SLASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALH_MOONLIGHT_SLASH );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK );
#ifdef FIX_ARA_SKILL_BUG
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_DESCENT_DRAGON );
#endif //FIX_ARA_SKILL_BUG
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALH_SWALLOW_ASSAULT == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALH_SWALLOW_ASSAULT );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_SWALLOW_ASSAULT_ATTACK );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALH_SWALLOW_ASSAULT_FINISH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ASD_LOW_BRANDISH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ASD_LOW_BRANDISH );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ASD_LOW_BRANDISH_SLICE );
		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ASD_LOW_BRANDISH_KICK );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ASD_SUPPRESSION == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ASD_SUPPRESSION );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ASD_SUPPRESSION );
	}
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ASD_DRAGON_FALLING == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ASD_DRAGON_FALLING );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ASD_FALLING_BLOSSOM_FINISH );
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AYR_HIGH_SPEED == eSkillID_ )			/// 아라 1차 전직 소마 - 쾌속
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AYR_HIGH_SPEED );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_AYR_HIGH_SPEED );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ALD_WOLF_CLAW == eSkillID_ )				/// 아라 1차 전직 소마 - 낭아 3식 늑대발톱
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ALD_WOLF_CLAW );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_ALD_WOLF_CLAW_FINISH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALD_HELL_OF_TORNADO == eSkillID_ )		/// 아라 1차 전직 소마 - 연환지옥
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALD_HELL_OF_TORNADO );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALD_HELL_OF_TORNADO_FINISH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALD_WOLF_SPEAR == eSkillID_ )			/// 아라 1차 전직 소마 - 낭아 4식 늑대이빨
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALD_WOLF_SPEAR );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALD_WOLF_SPEAR );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALD_WOLF_SPEAR_FINISH );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALD_WOLF_BREAK );		///  낭아 최종오의 - 극쇄
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALD_FINGER_BULLET == eSkillID_ )		/// 아라 1차 전직 소마 - 나찰 1식 흡혼
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALD_FINGER_BULLET );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALD_FINGER_BULLET_FINISH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALD_ENERGY_BEAD == eSkillID_ )			/// 아라 1차 전직 소마 - 나찰 2식 연환장
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALD_ENERGY_BEAD );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALD_ENERGY_BEAD_FINISH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ALD_PRISON_SPEAR == eSkillID_ )			/// 아라 1차 전직 소마 - 나찰 3식 창살감옥
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALD_PRISON_SPEAR );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_ALD_PRISON_SPEAR_FINISH );
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AYR_STEAL_SOUL == eSkillID_ )			/// 아라 2차 전직 명왕 - 나찰 4식 탈혼
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AYR_STEAL_SOUL );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_A_AYR_STEAL_SOUL_FINISH );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_AYR_OPEN_THE_HELL );	/// 나찰 최종오의 - 연옥
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_AYR_ENERGY_VOID == eSkillID_ )			/// 아라 2차 전직 명왕 - 초진공장
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AYR_ENERGY_VOID );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_AYR_ENERGY_VOID_LOOP );

		SetEquippedSkillLevelStateData( pSkillTemplet, ASI_SA_AYR_ENERGY_VOID_END );
	}

#endif // SERV_ARA_CHANGE_CLASS_SECOND

}
#pragma endregion

#pragma region SetEquippedSkillLevelStateData
/** @function	: SetEquippedSkillLevelStateData
	@brief		: 장착한 스킬의 레벨등이 변경될 때 같이 변경되어야 할 스테이트 데이타
	@param		: const CX2SkillTree::SkillTemplet* pSkillTemplet, ARA_STATE_ID eStateID
*/
void CX2GUAra::SetEquippedSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet, ARA_STATE_ID eStateID )
{
	if ( NULL != pSkillTemplet )
		SetSkillLevelStateData( pSkillTemplet, m_StateList[eStateID] );
}
#pragma endregion

void CX2GUAra::SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경

	if ( NULL == GetUnit() )
		return;
	
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨
	
	stateData_.m_SPLevel	= iSkillTempletLevel;
	stateData_.m_fPowerRate	= pSkillTemplet_->GetSkillPowerRateValue( iSkillTempletLevel );

#else // UPGRADE_SKILL_SYSTEM_2013

	stateData_.m_SPLevel	= pSkillTemplet_->m_iSkillLevel;
	stateData_.m_fPowerRate	= pSkillTemplet_->m_fPowerRate;

#endif // UPGRADE_SKILL_SYSTEM_2013
	
	stateData_.m_eSkillID	= pSkillTemplet_->m_eID;
}

#pragma region InitComponent
/** @function	: InitComponent
	@brief		: 캐릭터 스크립트의 INIT_COMPONENT 테이블을 로드함
*/
/*virtual*/ void CX2GUAra::InitComponent()
{
	CX2GUUser::InitComponent();

	if ( IsMyUnit() )
	{
		ASSERT( NULL != g_pX2Game );
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				static_cast<CX2AraGageData*>(m_pGageData)->SetBerserkMode( false );
				static_cast<CX2AraGageData*>(m_pGageData)->SetBerserkModeChanged( false );
				static_cast<CX2AraGageData*>(m_pGageData)->SetNowForcePower( 2 );
				static_cast<CX2AraGageData*>(m_pGageData)->SetForcePowerChanged( true );
			} break;

		default:
			{
				if ( g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
				{
					static_cast<CX2AraGageData*>(m_pGageData)->SetBerserkMode( false );
					static_cast<CX2AraGageData*>(m_pGageData)->SetBerserkModeChanged( false );
					static_cast<CX2AraGageData*>(m_pGageData)->SetNowForcePower( 2 );
					static_cast<CX2AraGageData*>(m_pGageData)->SetForcePowerChanged( true );
				}		
			} break;
		}
	}
}
#pragma endregion

#pragma region ParseCommonRandomState
/** @function	: ParseCommonRandomState
	@brief		: Start, Win, Lose 스테이트를 랜덤하게 로드한다
	@param		: 각 랜덤한 start, win, lose 스테이트가 들어있는 직업별 테이블명
*/
/*virtual*/ void CX2GUAra::ParseCommonRandomState()
{
	if( true == m_LuaManager.BeginTable( "INIT_COMMON_RANDOM_STATE" ) )
	{

		const char* tableName = "";
		switch( GetUnit()->GetClass() )
		{
		case CX2Unit::UC_ARA_MARTIAL_ARTIST:
			{
				tableName = "ARA_MARTIAL_ARTIST";
			} break;
#ifdef ARA_CHANGE_CLASS_FIRST
		case CX2Unit::UC_ARA_LITTLE_HSIEN:
			{
				tableName = "ARA_LITTLE_HSIEN";
			} break;
		case CX2Unit::UC_ARA_SAKRA_DEVANAM:
			{
				tableName = "ARA_SAKRA_DEVANAM";
			} break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		case CX2Unit::UC_ARA_LITTLE_DEVIL:
			{
				tableName = "ARA_LITTLE_DEVIL";
			} break;
		case CX2Unit::UC_ARA_YAMA_RAJA:
			{
				tableName = "ARA_YAMA_RAJA";
			} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
		default:
			{
				ASSERT( !"NO" );
			} break;
		}

		CX2GUUser::ParseCommonRandomState(tableName);

		m_LuaManager.EndTable(); // INIT_COMMON_RANDOM_STATE
	}
}
#pragma endregion

#pragma region InitEffect
/** @function	: InitEffect
	@brief		: 멤버로 가지고 있는 이펙트를 생성
*/
/*virtual*/ void CX2GUAra::InitEffect()
{
	CX2GUUser::InitEffect();

	CreateMajorParticleArray();
	CreateMinorParticleArray();
}
#pragma endregion

#pragma region InitEnchantWeaponEffectUnitSpecific
/** @function	: InitEnchantWeaponEffectUnitSpecific
	@brief		: 각 캐릭터가 쓰는 인텐트 파티클 초기화
*/
/*virtual*/ void CX2GUAra::InitEnchantWeaponEffectUnitSpecific()
{
	BOOST_TEST_FOREACH( Weapon*, pWeapon, m_vecpWeapon )
	{
		ASSERT( NULL != pWeapon );
		pWeapon->InitEnchantParticleForBow();	// 임시
	}
}
#pragma endregion

#pragma region InitializeAraMajorParticleArray
/** @function	: InitializeAraMajorParticleArray
	@brief		: 멤버로 가지고 있는 메이저 파티클 핸들 배열 초기화
*/
void CX2GUAra::InitializeAraMajorParticleArray()
{
	CX2GUUser::InitializeGUUserMajorParticleArray();

	for ( int index = 0; index < ARA_MAJOR_PII_END; index++ )
	{
		m_ahAraMajorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}
#pragma endregion

#pragma region CreateMajorParticleArray
/** @function	: CreateMajorParticleArray
	@brief		: 멤버로 가지고 있는 메이저 파티클 생성
*/
/*virtual*/ void CX2GUAra::CreateMajorParticleArray()
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
#pragma endregion

#pragma region AppendMajorParticleToDeviceList
/*static*/ void CX2GUAra::AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
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
#pragma endregion

#pragma region SetAraMajorParticleByEnum
/** @function	: SetAraMajorParticleByEnum
	@brief		: 파티클명과 일치하는 메이저 파티클을 생성한 후 배열에 해당 파티클 핸들이 없으면, 핸들을 배열에 저장, 그리고 그 인스턴스를 리턴해줌
	@param		: 파티클 Instance ID, 파티클 명, DrawCount
	@return		: 생성된 또는 이미 존재하는 파티클의 인스턴스(CKTDGParticleSystem::CParticleEventSequence*)
*/
CKTDGParticleSystem::CParticleEventSequence* CX2GUAra::SetAraMajorParticleByEnum( ARA_MAJOR_PARTICLE_INSTANCE_ID eVal_,
																			wstring wstrParticleName_, int iDrawCount_ /*= -1*/ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleAraMajorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,
											wstrParticleName_.c_str(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 0, 0, iDrawCount_, 0 );

		SetHandleAraMajorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAraMajorParticleByEnum( eVal_) );

	return pSeq;
}
#pragma endregion

#pragma region DeleteAraMajorParticle
/** @function	: DeleteMajorParticle
	@brief		: 멤버로 가지고 메이저 파티클 이펙트 제거
*/
void CX2GUAra::DeleteAraMajorParticle()
{
	CX2GUUser::DeleteGUUserMajorParticle();

	CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();

	for ( int index = 0; index < ARA_MAJOR_PII_END; index++ )
	{
		pMajorParticleSystem->DestroyInstanceHandle( m_ahAraMajorParticleInstance[index] );
	}
}
#pragma endregion

#pragma region InitializeAraMinorParticleArray
/** @function : InitializeAraMinorParticleArray
	@brief : 멤버로 가지고 있는 마이너 파티클 핸들 배열 초기화
*/
void CX2GUAra::InitializeAraMinorParticleArray()
{
	CX2GUUser::InitializeGUUserMinorParticleArray();

	for ( int index = 0; index < ARA_MINOR_PII_END; index++ )
	{
		m_ahAraMinorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}
#pragma endregion

#pragma region CreateMinorParticleArray
/** @function : CreateMinorParticleArray
	@brief : 멤버로 가지고 있는 마이저 파티클 생성
*/
/*virtual*/ void CX2GUAra::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();
}
#pragma endregion

#pragma region AppendMinorParticleToDeviceList
/** @function 	: AppendMinorParticleToDeviceList
	@brief 		: 
*/
/*static*/ void CX2GUAra::AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}
#pragma endregion 

#pragma region SetAraMinorParticleByEnum
/** @function	: SetAraMinorParticleByEnum
	@brief		: 파티클명과 일치하는 마이너 파티클을 생성한 후 배열에 해당 파티클 핸들이 없으면, 핸들을 배열에 저장, 그리고 그 인스턴스를 리턴해줌
	@param		: 파티클 Instance ID, 파티클 명, DrawCount
	@return		: 생성된 또는 이미 존재하는 파티클의 인스턴스(CKTDGParticleSystem::CParticleEventSequence*)
*/
CKTDGParticleSystem::CParticleEventSequence* CX2GUAra::SetAraMinorParticleByEnum( ARA_MINOR_PARTICLE_INSTANCE_ID eVal_,
																			wstring wstrParticleName_, int iDrawCount_ /*= -1*/ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleAraMinorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this, 
											wstrParticleName_.c_str(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 0, 0, iDrawCount_, 0 );

		SetHandleAraMinorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAraMinorParticleByEnum( eVal_) );

	return pSeq;
}
#pragma endregion

#pragma region DeleteAraMinorParticle
/** @function	: DeleteAraMinorParticle
	@brief		: 멤버로 가지고 마이너 파티클 이펙트 제거
*/
void CX2GUAra::DeleteAraMinorParticle()
{
	CX2GUUser::DeleteGUUserMinorParticle();

	CKTDGParticleSystem* pMinorParticleSystem = g_pX2Game->GetMinorParticle();

	for ( int index = 0; index < ARA_MINOR_PII_END; index++ )
	{
		pMinorParticleSystem->DestroyInstanceHandle( m_ahAraMinorParticleInstance[index] );
	}	
}
#pragma endregion

#pragma region InitializeMajorMeshArray
/** @function	: InitializeMajorMeshArray
	@brief		: 멤버로 가지고 있는 메이져 메쉬 핸들 배열 초기화
*/
void CX2GUAra::InitializeMajorMeshArray()
{
	for ( int index = 0; index < ARA_MAJOR_MII_END; ++index )
	{
		m_ahAraMajorMeshInstance[index] = INVALID_MESH_INSTANCE_HANDLE;
	}
}
#pragma endregion

#pragma region SetHandleAraMajorMeshByEnum
/** @function	: SetHandleAraMajorMeshByEnum
	@brief		: 메쉬플레이 핸들을 저장훈 배열에 저장, 저장할려는 핸들이 INVALID 일경우 메쉬플레이 DESTROY 후 배열초기화
	@param		: Mesh Instance ID, Mesh Instance Handle
*/
void CX2GUAra::SetHandleAraMajorMeshByEnum( ARA_MAJOR_MESH_INSTANCE_ID eVal_, CKTDGXMeshPlayer::CXMeshInstanceHandle hHandle_ )
{
	if( INVALID_MESH_INSTANCE_HANDLE == hHandle_ && INVALID_MESH_INSTANCE_HANDLE != GetHandleAraMajorMeshByEnum( eVal_ ) )
	{
		CKTDGXMeshPlayer::CXMeshInstanceHandle hHandleAraMesh = GetHandleAraMajorMeshByEnum( eVal_ );

		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle(hHandleAraMesh);
	}

	m_ahAraMajorMeshInstance[eVal_] = hHandle_;
}
#pragma endregion

#pragma region DeleteMajorMesh
/** @function	: DeleteMajorMesh
	@brief		: 멤버로 가지고 메이져 이펙트 제거
*/
void CX2GUAra::DeleteMajorMesh()
{
	CKTDGXMeshPlayer* pMajorMeshSystem = g_pX2Game->GetMajorXMeshPlayer();

	for ( int index = 0; index < ARA_MAJOR_MII_END; index++ )
	{
#ifdef  X2OPTIMIZE_ARA_MESH_DESTROY_BUG_FIX
        pMajorMeshSystem->DestroyInstanceHandle( m_ahAraMajorMeshInstance[index] );
#else   X2OPTIMIZE_ARA_MESH_DESTROY_BUG_FIX
		pMajorMeshSystem->DestroyInstanceHandle( m_ahAraMinorParticleInstance[index] );
#endif  X2OPTIMIZE_ARA_MESH_DESTROY_BUG_FIX
	}	
}
#pragma endregion

#pragma region CommonStateStartFuture
/** @function	: CommonStateStartFuture
	@brief		: 모든 스테이트의 StartFuture에서 호출되는 함수
*/
void CX2GUAra::CommonStateStartFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateStartFuture();
}
#pragma endregion

#pragma region CommonStateStart
/** @function	: CommonStateStart
	@brief		: 모든 스테이트의 Start에서 호출되는 함수
*/
void CX2GUAra::CommonStateStart()
{
	KTDXPROFILE();
	
	LUA_GET_VALUE( m_LuaManager, "DAMAGEDATA_CHANGE_TIME", m_fDamageDataChangeTime, 0.f );
	LUA_GET_VALUE( m_LuaManager, "DAMAGEDATA_CHANGE_TIME2", m_fDamageDataChangeTime2, 0.f );

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	/// 해당 시간이 설정되어 있으면, 그 시간에 기력 사용 처리를 한다.
	m_fUseForcePowerTime	= -1.f;
	m_iUseForcePowerCount	= 0;

	if( m_LuaManager.BeginTable( "USE_FORCE_POWER" ) == true )
	{		
		LUA_GET_VALUE( m_LuaManager, 1, m_fUseForcePowerTime, -1.f  );	/// 기력 사용 시점
		LUA_GET_VALUE( m_LuaManager, 2, m_iUseForcePowerCount, 0  );	/// 기력 사용량

		m_LuaManager.EndTable();
	}

	/// 해당 시간이 설정되어 있으면, 그 시간에 기력 증가 처리를 한다.
	m_fAddForcePowerTime	= -1.f;
	m_iAddForcePowerCount	= 0;

	if( m_LuaManager.BeginTable( "ADD_FORCE_POWER" ) == true )
	{		
		LUA_GET_VALUE( m_LuaManager, 1, m_fAddForcePowerTime, -1.f  );	/// 기력 증가 시점
		LUA_GET_VALUE( m_LuaManager, 2, m_iAddForcePowerCount, 0  );	/// 기력 증가량

		m_LuaManager.EndTable();
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	if( m_LuaManager.BeginTable( "FP_CANCEL" ) == true )
	{		
		LUA_GET_VALUE( m_LuaManager, 1, m_vFPCancel.x, - 1.f  );
		LUA_GET_VALUE( m_LuaManager, 2, m_vFPCancel.y, - 1.f  );

		m_LuaManager.EndTable();
	}

	ChangeAnimTail();

	//m_bFPCancel = false;

	CX2GUUser::CommonStateStart();

	if ( true == m_DamageData.m_bMeleeAttack && true == m_bBrutalPiercing && NULL != m_pBrutalPiercingBuffFactor )
		m_DamageData.PushBuffFactor( m_pBrutalPiercingBuffFactor );

	m_ForcePowerSystem.SetUsedForcePowerAtThisState( 0 );
}
#pragma endregion

#pragma region CommonFrameMoveFuture
/** @function	: CommonFrameMoveFuture
	@brief		: 모든 스테이트의 FrameMoveFuture에서 호출되는 함수
*/
void CX2GUAra::CommonFrameMoveFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonFrameMoveFuture();

	// 기력 동기화 코드
	m_FrameDataFuture.syncData.m_CannonBallCount= GetForcePower();

	if ( NULL == m_FutureStateData.m_ptrComboAndSkillChainInfo )
		m_ComboAndSkillChainSystem.ResetChaining();
	else
	{
		m_FutureStateData.m_ptrComboAndSkillChainInfo->SetChaining( true );
		m_FutureStateData.m_ptrComboAndSkillChainInfo->ResetChainingFromNext();
		m_ComboAndSkillChainSystem.ResetOtherListChaining( m_FutureStateData.m_ptrComboAndSkillChainInfo );
	}
		
}
#pragma endregion

#pragma region CommonFrameMove
/** @function	: CommonFrameMove
	@brief		: 모든 스테이트의 FrameMove에서 호출되는 함수
*/
void CX2GUAra::CommonFrameMove()
{
	KTDXPROFILE();
	CX2GUUser::CommonFrameMove();

	if ( false == IsMyUnit() )
	{
		// 기력 관련
		SetForcePower( m_FrameDataNow.syncData.m_CannonBallCount );
	} // if

	// 광폭화 모드에 쓰이는 장비들 frameMove 시켜줌
	BOOST_TEST_FOREACH( CX2EqipPtr, pPromotionEquipPtr, m_vecPromotionEquipList )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		pPromotionEquipPtr->SetRenderParam( m_fTime, m_fElapsedTime, m_pXSkinAnim->GetRenderParam() );
		pPromotionEquipPtr->OnFrameMove( m_fTime, m_fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		pPromotionEquipPtr->SetRenderParam( m_fTime, m_fElapsedTime * m_AdvanceTimeCount, m_pXSkinAnim->GetRenderParam() );
		pPromotionEquipPtr->OnFrameMove( m_fTime, m_fElapsedTime * m_AdvanceTimeCount );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	/// 아라 2차 전직 명왕 - 회광반조 쿨타임 갱신
	if ( 0.f < m_fResurrectionCoolTime )
	{
		m_fResurrectionCoolTime -= m_fElapsedTime;

		/// 쿨타임 완료 되었으면, 회광반조 사용 불가 안내 디버프 제거
		if ( 0.f >= m_fResurrectionCoolTime )
			EraseBuffTempletFromGameUnit( BTI_DEBUFF_SELF_PROTECTION_FORTITUDE );
	}

	/// 기력 사용 시간이 설정되어 있다면, 해당 시간에 기력 사용
	if ( false == IsSamef( m_fUseForcePowerTime, -1.f ) )
	{
		if ( true == UseForcePowerAtThisAnimationTime( m_fUseForcePowerTime, m_iUseForcePowerCount ) )
			m_fUseForcePowerTime = -1.f;
	}

	/// 기력 증가 시간이 설정되어 있다면, 해당 시간에 기력 증가
	if ( false == IsSamef( m_fAddForcePowerTime, -1.f ) )
	{
		if ( true == UpForcePowerAtThisAnimationTime( m_fAddForcePowerTime, m_iAddForcePowerCount ) )
			m_fAddForcePowerTime = -1.f;
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND
}
#pragma endregion

#pragma region CommonStateEndFuture
/** @function	: CommonStateEndFuture
	@brief		: 모든 스테이트의 EndFuture에서 호출되는 함수
*/
void CX2GUAra::CommonStateEndFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEndFuture();
}
#pragma endregion

#pragma region CommonStateEnd
/** @function	: CommonStateEnd
	@brief		: 모든 스테이트의 End에서 호출되는 함수
*/
void CX2GUAra::CommonStateEnd()
{	
	KTDXPROFILE();
	CX2GUUser::CommonStateEnd();
}
#pragma endregion

#pragma region CommonRender_Prepare
/** @function	: CommonRender_Prepare
	@brief		: 모든 스테이트의 End에서 호출되는 함수
*/
RENDER_HINT CX2GUAra::CommonRender_Prepare()
{
	KTDXPROFILE();

	return CX2GUUser::CommonRender_Prepare();
}
#pragma endregion

#pragma region SepcialAttackEventProcess
/** @function	: SepcialAttackEventProcess
	@brief		: 스킬이 정상적으로 사용될 수 있는 지 판단, 스킬사용에 필요한 처리 및 스테이트 변경
	@param		: 스킬을 사용할 수 있는 환경 조건
	@return		: bool (스킬이 정상적으로 사용되었는가를 리턴)
*/
/*virtual*/ bool CX2GUAra::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition /*= CX2SkillTree::ASUT_ONLY_ON_LINE*/ )
{
	if ( false == CanUseSpecialAttack( eActiveSkillUseCondition ) )
		return false;
#ifdef FIX_SKILL_CANCEL_ARA
	if ( CX2SkillTree::SI_NONE != m_eSkillID )
		StateChangeSpecialAttack( m_eSkillID );
#else
	if ( NULL != m_pSkillTemplet )
		StateChangeSpecialAttack( m_pSkillTemplet );
#endif

#ifdef SERV_SKILL_USE_SUBQUEST
	Send_SKILL_USE_REQ( m_eSkillID );
#endif SERV_SKILL_USE_SUBQUEST

	return true;
}
#pragma endregion

#pragma region CanUseSpecialAttack
/** @function 	: CanUseSpecialAttack
	@brief 		: 
*/
bool CX2GUAra::CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition /*= CX2SkillTree::ASUT_ONLY_ON_LINE*/ )
{
	KTDXPROFILE();

	int iPressedSkillSlotIndex = -1;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;
	const CX2UserSkillTree& cUserSkillTree =  GetUnit()->GetUnitData().m_UserSkillTree;	// 유저가 배운 스킬 트리

	// 사용한 스킬의 스킬슬롯 데이터와 그 인덱스를 얻어옴
	if ( false == CommonSpecialAttackEventProcess( cUserSkillTree, pSkillSlotData, iPressedSkillSlotIndex ) )
		return false;

	if( NULL == pSkillSlotData )
		return false;

	// 사용한 스킬
	//m_pSkillTemplet = NULL;
	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );

	if( NULL == pSkillTemplet )
		return false;

	// 스킬을 사용할 수 있는 조건인지 판단( ex: 공중에서는 사용 할 수 없는 필살기 등)
#ifdef ADD_MEMO_1ST_CLASS //김창한
	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet->m_eActiveSkillUseCondtion ) )
#else //ADD_MEMO_1ST_CLASS
	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet ) )
#endif //ADD_MEMO_1ST_CLASS
		return false;

	//소환 필살기 종류는 게임이 끝난 상태에서 사용할 수 없다.
	if( false == CheckSummonSpecialAttack( pSkillTemplet->m_eID ) )
		return false;

	// 공식대전에서 사용 가능한 스킬인지 검사
	if( false == CheckSkillUsePVPOfficial( pSkillTemplet->m_eID ) )
		return false;
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if ( NULL == GetUnit() )
		return false;
	
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	// 이혈공 스킬을 사용할 수 있는 상태인지 판단
	if ( CX2SkillTree::SI_A_ALD_CONVERSION_HEALTH == pSkillTemplet->m_eID )
	{
		if ( false == CanUseConversionHealth( pSkillTemplet, iSkillTempletLevel ) )
		{
			/// 체력이 부족하여 사용할 수 없다면, 안내 스트링 표시
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_26868 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	// 엠피 소모량을 얻어옴
	m_fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );

	float fCoolTimeLeft = pSkillSlotData->m_fCoolTimeLeft;
#else // UPGRADE_SKILL_SYSTEM_2013
	// 엠피 소모량을 얻어옴
	m_fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, pSkillTemplet->m_iSkillLevel );
	float fCoolTimeLeft = pSkillSlotData->m_fCoolTimeLeft;
#endif // UPGRADE_SKILL_SYSTEM_2013

	bool bSlotB = (iPressedSkillSlotIndex > 3) ? true : false;
	int iSlotIndex = (iPressedSkillSlotIndex > 3) ? iPressedSkillSlotIndex-4 : iPressedSkillSlotIndex;

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
	/// 해당 스킬의 기력 소비량 ( 메모 효과 적용 )
	USHORT usFPConsumptionNeeded = GetActualFPConsume( pSkillTemplet->m_eID, cUserSkillTree.GetFPConsumption( iSlotIndex, bSlotB ) );

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	/// 해당 스킬의 기력 전환량 ( 메모 효과 적용 ), ( No State Change 스킬이라면, 기력 전환을 막기 위해 0으로 설정 )
	m_usForceGainCount =	false == IsNoStageChangeSkill( pSkillTemplet ) ?
							GetActualFPGainCount( pSkillTemplet->m_eID, cUserSkillTree.GetFPGainCount( iSlotIndex, bSlotB ) ) : 0;
#else // SERV_ARA_CHANGE_CLASS_SECOND
	/// 해당 스킬의 기력 전환량 ( 메모 효과 적용 )
	m_usForceGainCount = GetActualFPGainCount( pSkillTemplet->m_eID, cUserSkillTree.GetFPGainCount( iSlotIndex, bSlotB ) );
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	
#else  // ADD_ARA_MEMO
	USHORT usFPConsumptionNeeded = cUserSkillTree.GetFPConsumption( iSlotIndex, bSlotB );

	m_usForceGainCount = cUserSkillTree.GetFPGainCount( iSlotIndex, bSlotB );
#endif // ADD_ARA_MEMO

	/// 코드 정리를 위해 임시 블럭 처리
	{
		m_ComboAndSkillChainSystem.ResetPtrNowUseChainInfo();

		const UserUnitStateData& stateDataPressed = GetPressedSkillState( iPressedSkillSlotIndex );
		if ( NULL != stateDataPressed.m_ptrComboAndSkillChainInfo &&
			stateDataPressed.m_ptrComboAndSkillChainInfo->GetChaining() )
		{
			CX2ComboAndSkillChainInfoPtr ptrInfo 
				= stateDataPressed.m_ptrComboAndSkillChainInfo->GetNotYetChainedInfo();

			if ( NULL != ptrInfo )
			{
				switch ( ptrInfo->GetComboAndSkillChainType() )
				{
				case CASCT_SKILL:
					{
						const CX2SkillTree::SKILL_ID eSkillIdChained 
							= static_cast<CX2SkillTree::SKILL_ID>( ptrInfo->GetSkillID() );
						pSkillTemplet = GetEquippedActiveSkillTemplet( eSkillIdChained );
						pSkillSlotData = cUserSkillTree.GetSkillSlot( eSkillIdChained );

						/// 연쇄 스킬을 장착 하고 있는 경우
						if ( NULL != pSkillTemplet && NULL != pSkillSlotData )
						{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
							m_fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
							m_fMPConsume = GetActualMPConsume( pSkillTemplet->m_eID, pSkillTemplet->m_iSkillLevel );
	#endif // UPGRADE_SKILL_SYSTEM_2013
							
							fCoolTimeLeft = pSkillSlotData->m_fCoolTimeLeft;
							usFPConsumptionNeeded = pSkillTemplet->m_usFPConsumtion;
							m_usForceGainCount = pSkillTemplet->m_usFPGainCount;
							m_ComboAndSkillChainSystem.SetPtrNowUseChainInfo( ptrInfo );								
						}
						else	/// 연쇄 스킬을 장착 하고 있지 않은 경우
						{
							pSkillTemplet = cUserSkillTree.GetUserSkillTemplet( eSkillIdChained );
							if ( NULL != pSkillTemplet )	/// 장착은 안했어도 스킬을 배운 경우
							{
				#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
								if ( NULL == GetUnit() )
									return false;
	
								const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
								const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
								m_fMPConsume = pSkillTemplet->GetSkillMPConsumptionValue( iSkillTempletLevel );

				#else // UPGRADE_SKILL_SYSTEM_2013
								m_fMPConsume = pSkillTemplet->m_fMPConsumption;
				#endif // UPGRADE_SKILL_SYSTEM_2013
								fCoolTimeLeft = 0.0f;
								usFPConsumptionNeeded = pSkillTemplet->m_usFPConsumtion;
								m_usForceGainCount = pSkillTemplet->m_usFPGainCount;
								m_ComboAndSkillChainSystem.SetPtrNowUseChainInfo( ptrInfo );								
							}
							/// 안 배운 경우에는 수행 무시
						}
					} break;

				default:
					{
						m_ComboAndSkillChainSystem.ResetChaining();
						ASSERT( !L"wrong path" );
					}
					break;
				}
			}

		}
	}

	//m_pSkillTemplet = pSkillTemplet;

	if( NULL == pSkillTemplet )		/// pSkillTemplet을 위에서 새로 대입하기 때문에, NULL처리 추가
		return false;

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
		if( 0.f < fCoolTimeLeft )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_226 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
		}

		if ( GetNowMp() < m_fMPConsume )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2549 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				return false;
		}		

		if ( GetForcePower() < usFPConsumptionNeeded )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_21303 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
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
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
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
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_25110 ), D3DXCOLOR(1,1,1,1),
				D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			return false;
		}
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	if ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
	{
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		if( true ==  g_pData->GetMyUser()->GetSelectUnit()->IsInfinityElEssence() )
		{
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ();
		}
		else
		{
			CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

			if ( NULL != pItem )
				g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
		}
#else
		CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( CX2EnchantItem::ATI_HYPER_SKILL_STONE );

		if ( NULL != pItem )
			g_pX2Game->Handler_EGS_USE_FINALITY_SKILL_REQ( pItem->GetItemData().m_ItemUID );
#endif SERV_BALANCE_FINALITY_SKILL_EVENT
	}
#endif //FINALITY_SKILL_SYSTEM

#ifdef FIX_SKILL_CANCEL_ARA
	m_eSkillID = pSkillTemplet->m_eID;
	m_iPressedSkillSlotIndex = iPressedSkillSlotIndex;
	
	if ( pSkillTemplet->m_eID == CX2SkillTree::SI_SA_ALH_MOONLIGHT_SLASH )
#else
	m_pSkillTemplet = pSkillTemplet;

	if ( NULL != m_pSkillTemplet && m_pSkillTemplet->m_eID == CX2SkillTree::SI_SA_ALH_MOONLIGHT_SLASH )
#endif
	{
		if ( NULL == m_sMoonRightSlashDataPtr )
			m_sMoonRightSlashDataPtr = sMoonLightSlashData::CreateMoonLightSlashData();

		if ( NULL != m_sMoonRightSlashDataPtr )
		{
			const int iNormalStateID = ( false == bSlotB ? _CONST_GUUSER_::aNormalSkillStateID[iSlotIndex] : _CONST_GUUSER_::aNormalSkillStateIDSlotB[iSlotIndex] );

			m_sMoonRightSlashDataPtr->SetSlotID( iNormalStateID );
			m_sMoonRightSlashDataPtr->SetSlotB( bSlotB );
		}
	}

	return true;
}
#pragma endregion

#pragma region StateChangeSpecialAttack
/** @function 	: StateChangeSpecialAttack
	@brief 		: 
*/
#ifdef FIX_SKILL_CANCEL_ARA
void CX2GUAra::StateChangeSpecialAttack( IN const CX2SkillTree::SKILL_ID eSkillId_ )
{
	DXUTTRACE( L"stateChange %d\n", eSkillId_ );


	CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;

	// 던전 랭크 개선 관련
	CountUsedSkill( m_iPressedSkillSlotIndex );

	// MP 소모 시킴
	UpNowMp( -m_fMPConsume );

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( eSkillId_ );

	if( NULL != pSkillTemplet )
	{
		// 스킬템플릿에서 지정한 쿨타임을 스킬을 사용 했음으로 적용함
#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22			메모 등으로 인한 쿨타임 변경값 설정

		const CX2SkillTree::SKILL_ID	eSkillID				= pSkillTemplet->m_eID;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int	iSkillTempletLevel	= max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		const float	fSkillCoolTimeValue	= GetSkillCoolTimeValue( eSkillID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const float						fSkillCoolTimeValue		= GetSkillCoolTimeValue( eSkillID, pSkillTemplet->m_fSkillCoolTime );
	#endif // UPGRADE_SKILL_SYSTEM_2013

		accessUserSkillTree.SetSkillCoolTimeLeft( eSkillID, fSkillCoolTimeValue );

#else  // ADD_ARA_MEMO
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
#endif // ADD_ARA_MEMO

		if ( NULL != m_ComboAndSkillChainSystem.GetPtrNowUseChainInfo() )
		{
			CX2ComboAndSkillChainInfoPtr ptrInfo 
				= m_ComboAndSkillChainSystem.GetPtrNowUseChainInfo();

			DXUTTRACE( L"chain %d, chained %d\n", ptrInfo->GetStateIdMatchedToChain(), ptrInfo->GetStateIdChained() );

			if( GetRemainHyperModeTime() <= 0.f )
				StateChange( ( 0 != ptrInfo->GetStateIdMatchedToChain() ? ptrInfo->GetStateIdMatchedToChain() : ptrInfo->GetStateIdChained() ) );
			else
				StateChange( ( 0 != ptrInfo->GetHyperStateIdMatchedToChain() ? ptrInfo->GetHyperStateIdMatchedToChain() : ptrInfo->GetStateIdChained() ) );
		}
		else
		{
			// state가 변경되지 말아야하는 스킬이 아니라면 스테이트 변경
			if( false == SpecialAttackNoStageChange( pSkillTemplet ) )
			{
				if( GetRemainHyperModeTime() <= 0.f )
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
		//오현빈//2013-01-21//일부 기력 전환 스킬 사용 시, 기력 중첩 회복 되는 경우가 있어서 초기화 추가
		m_bFPCancel = false;
	}
}
#else
void CX2GUAra::StateChangeSpecialAttack( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	const CX2UserSkillTree& cUserSkillTree =  GetUnit()->GetUnitData().m_UserSkillTree;

	// 던전 랭크 개선 관련
	CountUsedSkill( m_iPressedSkillSlotIndex );

	// MP 소모 시킴
	UpNowMp( -m_fMPConsume );
		
	// 스킬템플릿에서 지정한 쿨타임을 스킬을 사용 했음으로 적용함
	cUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet_->m_eID, pSkillTemplet_->m_fSkillCoolTime );

	if ( NULL != m_ComboAndSkillChainSystem.GetPtrNowUseChainInfo() )
	{
		CX2ComboAndSkillChainInfoPtr ptrInfo 
			= m_ComboAndSkillChainSystem.GetPtrNowUseChainInfo();

		if( GetRemainHyperModeTime() <= 0.f )
			StateChange( ( 0 != ptrInfo->GetStateIdMatchedToChain() ? ptrInfo->GetStateIdMatchedToChain() : ptrInfo->GetStateIdChained() ) );
		else
			StateChange( ( 0 != ptrInfo->GetHyperStateIdMatchedToChain() ? ptrInfo->GetHyperStateIdMatchedToChain() : ptrInfo->GetStateIdChained() ) );
	}
	else
	{
		// state가 변경되지 말아야하는 스킬이 아니라면 스테이트 변경
		if( false == SpecialAttackNoStageChange( pSkillTemplet_ ) )
		{
			if( GetRemainHyperModeTime() <= 0.f )
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
	//오현빈//2013-01-21//일부 기력 전환 스킬 사용 시, 기력 중첩 회복 되는 경우가 있어서 초기화 추가
	m_bFPCancel = false; 
}
#endif
#pragma endregion 

#pragma region SpecialAttackNoStageChange
/** @function	: SpecialAttackNoStageChange
	@brief		: 스테이트가 변경되지 말아야하는 스킬인지를 체크 하고 처리함
	@param		: IN const CX2SkillTree::SkillTemplet* (사용한 스킬의 스킬 템플릿 포인터)
	@return		: 스테이트 변경 여부
*/
/*virtual*/ bool CX2GUAra::SpecialAttackNoStageChange( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ )
{
	switch( pSkillTemplet_->m_eID )
	{

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CX2SkillTree::SI_A_ALD_REFLECTION:		/// 아라 1차 전직 소마 - 반탄공
		{
			m_FrameDataFuture.syncData.nowAction = UAI_REFLECTION;
		} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	case CX2SkillTree::SI_NONE:
	default:
		{
			return false;
		} break;

	}
	return true;
}
#pragma endregion

#pragma region COMMON_BUFF_FrameMove
/** @function	: COMMON_BUFF_FrameMove
	@brief		: 공통적으로 쓰는 버프 스킬의 FrameMove
*/
/*virtual*/ void CX2GUAra::COMMON_BUFF_FrameMove()
{

#ifndef UPGRADE_SKILL_SYSTEM_2013 // 공통 스킬 개편, 김종훈
	// 각성 인지 아닌지에 따라 World색 변경
	ChangeWorldColorByHyperMode();		
	ShowActiveSkillCutInAndLightByBerserkMode(  0.004f, false  );
#endif // UPGRADE_SKILL_SYSTEM_2013 // 공통 스킬 개편, 김종훈	
	

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
#pragma endregion

#pragma region InitPromotionEquipList
/** @function	: InitPromotionEquipList
	@brief		: 프로모션 장비 리스트를 초기화 한다.
*/
void CX2GUAra::InitPromotionEquipList()
{
	// 장비된 아이템 ID를 얻어온다.
	BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_ViewEqipList )
	{
		if ( pEquipPtr != NULL )
		{
			switch ( pEquipPtr->GetEqipPos() )
			{
			case CX2Unit::EP_WEAPON_HAND:
			case CX2Unit::EP_DEFENCE_BODY:
			case CX2Unit::EP_DEFENCE_LEG:
			case CX2Unit::EP_DEFENCE_HAND:
			case CX2Unit::EP_DEFENCE_FOOT:
				m_vecPromotionItemIDList2.push_back( pEquipPtr->GetItemID() );
				break;
			}
		}
	}

	m_vecPromotionEquipList.reserve( m_vecPromotionItemIDList.size() + m_vecPromotionItemIDList2.size() );

	BOOST_TEST_FOREACH( int, iItemID, m_vecPromotionItemIDList )
	{
		CX2EqipPtr pEquipPtr = CX2ItemManager::CreateDummyEquip( iItemID, GetUnit(), m_pXSkinAnim );
		pEquipPtr->SetShowObject( false );

		if ( pEquipPtr != NULL )
		{
			pEquipPtr->SetManagerObject( this );			
			m_vecPromotionEquipList.push_back( pEquipPtr );

			if ( CX2Unit::EP_AC_LEG == pEquipPtr->GetEqipPos() )
				m_ptrPromotionEquipTail = pEquipPtr;
		} //if
	}

	BOOST_TEST_FOREACH( int, iItemID, m_vecPromotionItemIDList2 )
	{
		CX2EqipPtr pEquipPtr = CX2ItemManager::CreateDummyEquip( iItemID, GetUnit(), m_pXSkinAnim );
		pEquipPtr->SetShowObject( false );

		if ( pEquipPtr != NULL )
		{
			pEquipPtr->SetManagerObject( this );			
			m_vecPromotionEquipList.push_back( pEquipPtr );
		} //if
	}

	SetShowOriginalViewList( false );
	SetShowPromotionEquips( true );
}
#pragma endregion

#pragma region ShowActiveSkillCutInAndLight
/** @function	: ShowActiveSkillCutInAndLight
	@brief		: 전달해준 시간에 광폭화모드에 따라 스킬 컷인과 light를 출력함
	@param		: float fTimeToShow_, bool bOnlyLight_ (
*/
/*virtual*/ void CX2GUAra::ShowActiveSkillCutInAndLightByBerserkMode( float fTimeToShow_, bool bOnlyLight_ /*= false*/ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( fTimeToShow_ ) == true && EventCheck( fTimeToShow_, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( GetShowCutInAndChangeWorldColor() && GetShowActiveSkillShow() )
		{
			// 광폭화 상태면
			if ( IsFullHyperMode() )
#ifdef SERV_APRIL_FOOLS_DAY
				( true == m_bIsFoolsDay )?
				ActiveSkillShow( L"Dummy2_Lhand", s_SkillCutInSetFoolsDay.fileName.c_str(),
				s_SkillCutInSetFoolsDay.vSize,
				s_SkillCutInSetFoolsDay.vPosition, IsHyperState(), bOnlyLight_ ) :
#endif //APRIL_FOOLS_DAY
				ActiveSkillShow( L"Dummy2_Lhand", s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[1].fileName.c_str(),
								 s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[1].vSize,
								 s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[1].vPosition, IsHyperState(), bOnlyLight_ );
			else
#ifdef SERV_APRIL_FOOLS_DAY
				( true == m_bIsFoolsDay )?
				ActiveSkillShow( L"Dummy2_Lhand", s_SkillCutInSetFoolsDay.fileName.c_str(),
				s_SkillCutInSetFoolsDay.vSize,
				s_SkillCutInSetFoolsDay.vPosition, IsHyperState(), bOnlyLight_ ) :
#endif //APRIL_FOOLS_DAY
				ActiveSkillShow( L"Dummy2_Lhand", s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].fileName.c_str(),
								 s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].vSize,
								 s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[0].vPosition, IsHyperState(), bOnlyLight_ );
		}
	}	
}
#pragma endregion

#pragma region NoStateChangeActionFrameMove
/** @function	: NoStateChangeActionFrameMove
	@brief		: 스테이트의 변화가 없는 스킬 사용 시 수행되는 FrameMove
*/
/*virtual*/ void CX2GUAra::NoStateChangeActionFrameMove()
{
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	switch( m_FrameDataNow.syncData.nowAction )
	{
	case UAI_REFLECTION:	/// 아라 1차 전직 소마 - 반탄공
		{
			if ( NULL != GetUnit() && g_pX2Game->GetEffectSet() && false == GetRidingOn() )
			{
				/// 스킬 레벨 반환
				const int iSkillLevel = max( GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ALD_REFLECTION ), 1 );

				/// 반탄공 버프 생성
#ifdef ADD_MEMO_1ST_CLASS //김창한
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_ARA_MEMO21) == true )
					SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_SI_A_ALD_REFLECTION_MEMO, iSkillLevel );
				else
#endif //ADD_MEMO_1ST_CLASS
				SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_SI_A_ALD_REFLECTION, iSkillLevel );

				/// 반탄공 이펙트 생성
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_A_ALD_REFLECTION", this );

				/// 반탄공 사운드
				PlaySound( L"Elsword_Endurance.ogg" );
				/// 기력 1 소모
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				UseForcePower( 2 );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				UseForcePower( 1 );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			}
		} break;
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND
}
#pragma endregion

#pragma region GetActualMPConsume
/** @function 	: GetActualMPConsume
	@brief 		: 마나 사용량 반환
*/
/*virtual*/ float CX2GUAra::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
	switch( eSkillID_ )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_AM_SHADOW_KNOT:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_SHADOW_KNOT:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			/// 메모 : 비전 그림자 창술 (그림자매듭) - MP 소모량 100 감소
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO5 ) )
				fMpConsumption -= 100.f;
		} break;

#ifdef ADD_MEMO_1ST_CLASS //김창한
		case CX2SkillTree::SI_A_ASD_DRAGON_FALLING:
			{
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO10 ) == true )
					fMpConsumption *= 0.9f;
			} break;

		case CX2SkillTree::SI_A_ALD_CONVERSION_MANA:
			{
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO15 ) == true )
					fMpConsumption *= 0.9f;
			} break;
#endif //ADD_MEMO_1ST_CLASS

	}
#endif // ADD_ARA_MEMO

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
#pragma endregion

#pragma region LoadPromotionItemID
/** @function	: LoadPromotionItemID
	@brief		: 스크립트에서 각 클래스에 해당하는 프로모션 장비 ID를 얻어옴
	@param		: KLuaManager의 레퍼런스
*/
void CX2GUAra::LoadPromotionItemID( IN KLuaManager& luaManager_ )
{
	if ( true == luaManager_.BeginTable( "PROMOTION_ITEM_ID_LIST" ) )
	{
		bool bOpenTable = false;
		switch ( GetUnit()->GetClass() )
		{
			case CX2Unit::UC_ARA_MARTIAL_ARTIST:
			{
				bOpenTable = luaManager_.BeginTable( "ARA_MARTIAL_ARTIST" );
			} break;
#ifdef ARA_CHANGE_CLASS_FIRST
			case CX2Unit::UC_ARA_LITTLE_HSIEN:
				{
					bOpenTable = luaManager_.BeginTable( "ARA_LITTLE_HSIEN" );
				} break;
			case CX2Unit::UC_ARA_SAKRA_DEVANAM:
				{
					bOpenTable = luaManager_.BeginTable( "ARA_SAKRA_DEVANAM" );
				} break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
			case CX2Unit::UC_ARA_LITTLE_DEVIL:
				{
					bOpenTable = luaManager_.BeginTable( "ARA_LITTLE_DEVIL" );
				} break;
			case CX2Unit::UC_ARA_YAMA_RAJA:
				{
					bOpenTable = luaManager_.BeginTable( "ARA_YAMA_RAJA" );
				} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
			default:
			{
				ASSERT( !L"Wrong Path" );
				bOpenTable = luaManager_.BeginTable( "ARA_MARTIAL_ARTIST" );
			} break;
		}

		if ( true == bOpenTable )
		{
			int iIndex = 1;
			int iItemID = 0;

			do 
			{
				LUA_GET_VALUE( luaManager_, iIndex++, iItemID, 0 );
				
				if ( 0 != iItemID )
					m_vecPromotionItemIDList.push_back( iItemID );

			} while ( iItemID != 0 );

			luaManager_.EndTable();
		} // if

		luaManager_.EndTable();
	}
}
#pragma endregion

#pragma region NotifyShowObjectChanged
/** @function	: NotifyShowObjectChanged
	@brief		: "아라"가 보이거나 또는 보이지 않게 될때 같이 보이거나 보이지 않아야 할 것들을 처리
*/
/*virtual*/ void CX2GUAra::NotifyShowObjectChanged()
{
	CX2GUUser::NotifyShowObjectChanged();

	// 청이 보이지 않는다면
	SetShowViewList( GetShowObject() );
	UpdateEquipCollisionData();
}
#pragma endregion

#pragma region NotifyDGManagerChanged
/** @function	: NotifyDGManagerChanged
	@brief		: "아라"가 DGManager에 등록 또는 삭제 될때 같이 등록 되거나 삭제되어야 할 것들을 처리함
	@param		: CKTDGManager& refInManager_
*/
/*virtual*/ void CX2GUAra::NotifyDGManagerChanged( CKTDGManager& refInManager_ )		// DGManager에 등록
{
	CX2GUUser::NotifyDGManagerChanged( refInManager_ );

	if ( IsRecursivelyManagedByDGManager() )
	{
		BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_vecPromotionEquipList )
		{
			if ( pEquipPtr != NULL )
				refInManager_.AddObjectChain( pEquipPtr.get() );
		}
	}
	else
	{
		BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_vecPromotionEquipList )
		{
			if ( pEquipPtr != NULL )
				refInManager_.RemoveObjectChain( pEquipPtr.get() );
		}
	}
}
#pragma endregion

#pragma region SetShowPromotionEquips
/** @function	: SetShowPromotionEquips
	@brief		: 프로모션 장비를 보여줄지 여부를 지정
	@param		: bool bShow_
*/
void CX2GUAra::SetShowPromotionEquips( bool bShow_ )
{
	// 프로모션 장비만 보여야 함
	BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_vecPromotionEquipList )
	{
		if ( pEquipPtr != NULL )
		{
			pEquipPtr->SetShowObject( bShow_ );

			if ( true == bShow_ && CX2Unit::EP_WEAPON_HAND == pEquipPtr->GetEqipPos() )
			{
				ReplaceWeapon( pEquipPtr );
			}
		}
	}
}
#pragma endregion

#pragma region SetShowViewList
/** @function	: SetShowViewList
	@brief		: 광폭화 인지 아닌지에 따라 어느 장비를 보여줄지 를 결정함
	@param		: bool bShow_
*/
/*virtual*/ void CX2GUAra::SetShowViewList( bool bShow_ )
{
	// 광폭화 모드 면
	if ( IsFullHyperMode() )
	{
		SetShowOriginalViewList( false );
		SetShowPromotionEquips( bShow_ );
	}
	// 광폭화 모드가 아니면
	else
	{
		SetShowPromotionEquips( false );
		SetShowOriginalViewList( bShow_ );
	}	
}
#pragma endregion

#pragma region SetShowOriginalViewList
/** @function	: SetShowOriginalViewList
	@brief		: ViewList의 장비를 보여줌
	@param		: bool bShow_ (보여줄지 여부 지정)
*/
void CX2GUAra::SetShowOriginalViewList( bool bShow_ )
{
	BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquipPtr, m_ViewEqipList )
	{
		if ( pViewEquipPtr != NULL )
		{
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
			if ( false != bShow_ || CX2Unit::EP_ONEPIECE_FASHION != pViewEquipPtr->GetEqipPos() )
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
			pViewEquipPtr->SetShowObject( bShow_ );

			if ( true == bShow_ && CX2Unit::EP_WEAPON_HAND == pViewEquipPtr->GetEqipPos() )
			{
				ReplaceWeapon( pViewEquipPtr );
			}
		}
	}
}
#pragma endregion

#pragma region ReplaceWeapon
/** @function	: ReplaceWeapon
	@brief		: 광폭화등을 시행하거나 끝났을 때 무기 변경
	@param		: CX2EqipPtr pWeaponPtr_ ( pWeaponPtr_로 바뀐다 )
*/
void CX2GUAra::ReplaceWeapon( CX2EqipPtr pWeaponPtr_ )
{
	CX2GameUnit::Weapon* pOldWeapon = m_vecpWeapon[0];

	if ( NULL != pOldWeapon &&
		 pOldWeapon->m_pUserWeaponEquip != NULL &&
		 pOldWeapon->m_pUserWeaponEquip != pWeaponPtr_ &&
		 pOldWeapon->m_pUserWeaponEquip->GetItem() != NULL )
	{
		int iEnchantLevel = pOldWeapon->m_pUserWeaponEquip->GetEnchantLevel();
        const CX2Item::ItemTemplet* pOldItemTemplet = pOldWeapon->m_pUserWeaponEquip->GetItem()->GetItemTemplet();

		pOldWeapon->m_pUserWeaponEquip.reset();
		SAFE_DELETE_KTDGOBJECT( pOldWeapon );
		m_vecpWeapon.resize(0);

		// 강화 이펙트를 위해 Enchant level 변경
		pWeaponPtr_->ChangeEnchantLevel( iEnchantLevel );
		CreateWeapon( pOldItemTemplet, pWeaponPtr_ );
	}
}
#pragma endregion

#pragma region UpdateEquipViewListInfo
/** @function	: UpdateEquipViewListInfo
	@brief		: 장비의 장착, 탈착, 그리고 광폭화 여부에 따라 ViewList를 갱신함
	@param		: CX2Item* pItemToAddInViewList(추가될 장비), CX2Item* const pItemToRemoveInViewList(탈착될 장비)
	@return		: 함수의 성공 여부
*/
/*virtual*/ bool CX2GUAra::UpdateEquipViewListInfo( CX2Item* pItemToAddInViewList, CX2Item* const pItemToRemoveInViewList )
{
	CX2EqipPtr pEquipPtrToRemove;
	int iIndexInViewListForRemove_ = 0;
	bool isNullAddList = true;

	if ( NULL != pItemToAddInViewList )
		isNullAddList = false;

	if ( false == GetAddItemAndRemoveItemToViewList( pEquipPtrToRemove, &pItemToAddInViewList, pItemToRemoveInViewList,
		 iIndexInViewListForRemove_ ) )
		return false;

	if ( pEquipPtrToRemove.get() != NULL )
	{
		RemoveEquipFromViewList( pEquipPtrToRemove, iIndexInViewListForRemove_ );
	}

	if ( pItemToAddInViewList != NULL )
	{
		AddEquipToViewList( pItemToAddInViewList );
	}

	CX2EqipPtr pBerserkEquipToRemove;

	if ( true == isNullAddList )
		pItemToAddInViewList = NULL;

	if ( false == GetAddItemAndRemoveItemToViewListBerserk( pBerserkEquipToRemove, &pItemToAddInViewList, pItemToRemoveInViewList,
		iIndexInViewListForRemove_ ) )
		return false;

	if ( NULL != pBerserkEquipToRemove.get() )
	{
		RemoveBerserkEquipFromViewList( pBerserkEquipToRemove, iIndexInViewListForRemove_ );
	}

	if ( pItemToAddInViewList != NULL )
	{
		AddBerserkEquipToViewList( pItemToAddInViewList );
	}

	// 이곳을 사용하는 루틴에서 UpdateEquipCollisionData()를 호출하기 때문에 
	// 여기서는 호출 할 필요가 없다. 
	return true;
}
#pragma endregion

#pragma region UpdateEquipCollisionData
/** @function	: UpdateEquipCollisionData
	@brief		: 현재 보이는 장비에 대한 충돌 데이터를 갱신한다.
*/
/*virtual*/ void CX2GUAra::UpdateEquipCollisionData()
{
	// 광폭화 모드 이면
	if ( IsFullHyperMode() )
	{
		UpdateNotRenderingOtherEquipList();
		CX2GUUser::UpdateEquipCollisionData( m_vecPromotionEquipList );
	}
	// 일반 모드 이면
	else
	{
		CX2GUUser::SetShowViewList( true );
		CX2GUUser::UpdateNotRenderingOtherEquipList();
		CX2GUUser::UpdateEquipCollisionData( m_ViewEqipList );
	}

	////// 어택 박스 왜 끌까?
	SetEnableAttackBox( L"Rfoot", false );
	SetEnableAttackBox( L"Lfoot", false );
	SetEnableAttackBox( L"Lhand2", false );		
}
#pragma endregion

#pragma region UpdateNotRenderingOtherEquipList
#ifdef FIX_ARA_ONE_PIECE_FOR_BERSERK
/*virtual*/ void CX2GUAra::UpdateNotRenderingOtherEquipList()
{
	if ( IsFullHyperMode() )
	{
		#ifdef FACE_OFF_MONSTER_HEAD_TEST
		SetHideNoRenderable(false);
		#endif FACE_OFF_MONSTER_HEAD_TEST

		#ifdef ONE_PIECE_AVATAR
		// oasis907 : 김상윤 [2011.8.22] 한벌 상의 아바타
		bool bOnePieceAvatarCheck = false;

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		CX2EqipPtr pNoRenderFactorEquipBody = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_ONEPIECE_FASHION );
#else //SERV_NEW_ONE_PIECE_AVATAR_SLOT
		CX2EqipPtr pNoRenderFactorEquipBody = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_DEFENCE_BODY );
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
		
		if( pNoRenderFactorEquipBody != NULL )
		{
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
			if ( IsOnePieceAvatarRenderHair( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					if( IsOnePieceAvatarWithNotRenderHairItem( pViewEquip->GetItemID() ) == true )
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_AC_FACE2:
						case CX2Unit::EP_DEFENCE_LEG:
						case CX2Unit::EP_DEFENCE_HAND:
						case CX2Unit::EP_DEFENCE_FOOT:
						case CX2Unit::EP_DEFENCE_BODY:
							{
								pViewEquip->SetShowObject( false );
								bOnePieceAvatarCheck = true;
							}
							break;

						default:
							break;
						}
					}
					else
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_DEFENCE_LEG:
						case CX2Unit::EP_DEFENCE_HAND:
						case CX2Unit::EP_DEFENCE_FOOT:
						case CX2Unit::EP_DEFENCE_BODY:
							{
								pViewEquip->SetShowObject( false );
								bOnePieceAvatarCheck = true;
							}
							break;

						default:
							break;
						}
					}
				}
			}
#endif SERV_NEW_ONE_PIECE_AVATAR_SLOT
			if ( IsOnePieceAvatar( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAIR:
					case CX2Unit::EP_DEFENCE_LEG:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_DEFENCE_FOOT:
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
					case CX2Unit::EP_DEFENCE_BODY:
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
						{
							pViewEquip->SetShowObject( false );
							bOnePieceAvatarCheck = true;
						}
						break;

					default:
						break;
					}
				}
			}
			if ( IsOnePieceAvatarNotRenderFace1( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAIR:
					case CX2Unit::EP_DEFENCE_LEG:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_DEFENCE_FOOT:
					case CX2Unit::EP_AC_FACE1:
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
					case CX2Unit::EP_DEFENCE_BODY:
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
						{
							pViewEquip->SetShowObject( false );
							bOnePieceAvatarCheck = true;
						}
						break;

					default:
						break;
					}
				}
			}
			if ( IsOnePieceAvatarNotRenderFace1Face2( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAIR:
					case CX2Unit::EP_DEFENCE_LEG:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_DEFENCE_FOOT:
					case CX2Unit::EP_AC_FACE1:
					case CX2Unit::EP_AC_FACE2:
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
					case CX2Unit::EP_DEFENCE_BODY:
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT					
						{
							pViewEquip->SetShowObject( false );
							bOnePieceAvatarCheck = true;
						}
						break;

					default:
						break;
					}
				}
			}
			if ( IsOnePieceAvatarNotRenderFace( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				SetHideNoRenderable(true);
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAIR:
					case CX2Unit::EP_DEFENCE_LEG:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_DEFENCE_FOOT:
					case CX2Unit::EP_AC_FACE2:
					case CX2Unit::EP_AC_FACE3:
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
					case CX2Unit::EP_DEFENCE_BODY:
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
						{
							pViewEquip->SetShowObject( false );
							bOnePieceAvatarCheck = true;
						}
						break;

					default:
						break;
					}
				}
			}
			/// 한벌 아바타 외에 헤어와 모든 악세서리를 꺼야 하는 아이템
			if ( IsOnePieceAvatarNotRenderHairAndAllAccessory( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAIR:
					case CX2Unit::EP_DEFENCE_LEG:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_DEFENCE_FOOT:
					case CX2Unit::EP_AC_FACE1:
					case CX2Unit::EP_AC_FACE2:
					//case CX2Unit::EP_AC_FACE3:	/// 요청으로 인한 얼굴 (하) 제외
					case CX2Unit::EP_AC_BODY:
					case CX2Unit::EP_AC_ARM:
					case CX2Unit::EP_AC_NECKLESS:
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
					case CX2Unit::EP_DEFENCE_BODY:
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
						{
							pViewEquip->SetShowObject( false );
							bOnePieceAvatarCheck = true;
						}
						break;

					default:
						break;
					}
				}
			}
			if ( IsOnePieceAvatarRenderingHairAndFace1( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_LEG:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_DEFENCE_FOOT:
						{
							pViewEquip->SetShowObject( false );
						}
						break;

					default:
						break;
					}
				}
			}
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
			// 얼굴 상, 중, 하 제외 모든 악세, 장비의 렌더를 끔
			if ( IsOnePieceAvatarOnlyRenderFace1Face2Face3Equip( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAIR:
					case CX2Unit::EP_DEFENCE_LEG:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_DEFENCE_FOOT:
					case CX2Unit::EP_AC_BODY:
					case CX2Unit::EP_AC_ARM:
					case CX2Unit::EP_AC_NECKLESS:
						{
							pViewEquip->SetShowObject( false );
							bOnePieceAvatarCheck = true;
						}
						break;

					default:
						break;
					}
				}
			}
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT			
			if ( IsOnePieceAvatarWithRenderBodyItem( pNoRenderFactorEquipBody->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAIR:
					case CX2Unit::EP_DEFENCE_HAND:
					case CX2Unit::EP_AC_FACE1:
					case CX2Unit::EP_AC_FACE2:
					//case CX2Unit::EP_AC_FACE3:	/// 요청으로 인한 얼굴 (하) 제외
					case CX2Unit::EP_AC_NECKLESS:
						{
							pViewEquip->SetShowObject( false );
							bOnePieceAvatarCheck = true;
						}
						break;

					default:
						break;
					}
				}
			}
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT					
		}
		#endif ONE_PIECE_AVATAR

		#ifdef FACE_OFF_MONSTER_HEAD_TEST
		CX2EqipPtr pNoRenderFactorEquipACArm = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_AC_ARM );
		if( pNoRenderFactorEquipACArm != NULL )
		{
			if ( IsNoRenderHandEquip( pNoRenderFactorEquipACArm->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_DEFENCE_HAND:
						{
							if ( false == IsFullHyperMode() )
							pViewEquip->SetShowObject( false );
						}
						break;

					default:
						break;
					}
				}
			}
		}
		#endif FACE_OFF_MONSTER_HEAD_TEST

		//CX2Item* pItem = GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_AC_FACE1, false );
		// EP_AC_FACE1 장착위치에 있는 아이템의 id를 얻어옴
		CX2EqipPtr pNoRenderFactorEquip = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_AC_FACE1 );

		if ( pNoRenderFactorEquip == NULL )
			return;
		else
		{
			// kimhc // 이 이상 안보여야 할 분류가 달라 진다면
			// 안보여야할 부위 및 그런 기능을 가지고 있는 아이템을 스크립트로 빼야할듯
			// 현재는 헤어만 안보이게 하는 아이템과 헤어, 얼굴 악세(중,하)를 안보이게 하는 아이템이 같은 위치 이므로
			// 한개만 착용 할 수 있음
			if ( IsNoRenderHairFace2Face3Equip( pNoRenderFactorEquip->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_AC_FACE2:
					case CX2Unit::EP_AC_FACE3:
						pViewEquip->SetShowObject( false );
						break;

					default:
						break;
					}
				}
			}

			if ( IsNoRenderHairFace2Equip( pNoRenderFactorEquip->GetItemID() ) == true )
			{
				BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
				{
					switch ( pViewEquip->GetEqipPos() )
					{
					case CX2Unit::EP_AC_FACE2:
						pViewEquip->SetShowObject( false );
						break;

					default:
						break;
					}
				}
			}


			else if ( IsNoRenderHair( pNoRenderFactorEquip->GetItemID() ) == true )
			{
				#ifdef ONE_PIECE_AVATAR
				if( bOnePieceAvatarCheck == true )
				{
					BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_AC_FACE1:
						case CX2Unit::EP_AC_FACE2:
							pViewEquip->SetShowObject( false );
							break;

						default:
							break;
						}
					}
				}
				else
				#endif ONE_PIECE_AVATAR
				{
					BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_DEFENCE_HAIR:
							if ( false == IsFullHyperMode() )
								pViewEquip->SetShowObject( false );
							break;

						default:
							break;
						}
					}
				}
			}
		#ifdef FACE_OFF_MONSTER_HEAD_TEST
			else if ( IsNoRenderHairFaceFace2Face3Equip( pNoRenderFactorEquip->GetItemID() ) == true )
			{
				#ifdef ONE_PIECE_AVATAR
				if(bOnePieceAvatarCheck == true)
				{
					BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_AC_FACE1:
							pViewEquip->SetShowObject( false );
							break;

						default:
							break;
						}
					}
				}
				else
				#endif ONE_PIECE_AVATAR
				{
					SetHideNoRenderable(true);
					BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_AC_FACE2:
						case CX2Unit::EP_AC_FACE3:
							pViewEquip->SetShowObject( false );
							break;
						default:
							break;
						}
					}
				}
			}
		#endif FACE_OFF_MONSTER_HEAD_TEST
		}

		pNoRenderFactorEquip = GetEquipPtrFromEquipViewListByEquipPosition( CX2Unit::EP_AC_FACE2 );

		if ( pNoRenderFactorEquip != NULL )
		{
			if ( IsNoRenderHairFaceFace1Face3Equip( pNoRenderFactorEquip->GetItemID() ) == true )
			{
#ifdef ONE_PIECE_AVATAR
				if(bOnePieceAvatarCheck == true)
				{
					BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_AC_FACE2:
							pViewEquip->SetShowObject( false );
							break;

						default:
							break;
						}
					}
				}
#endif ONE_PIECE_AVATAR
				else
				{
					SetHideNoRenderable(true);
					BOOST_TEST_FOREACH( CX2EqipPtr, pViewEquip, m_vecPromotionEquipList )
					{
						switch ( pViewEquip->GetEqipPos() )
						{
						case CX2Unit::EP_AC_FACE1:
						case CX2Unit::EP_AC_FACE3:
							pViewEquip->SetShowObject( false );
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}
#endif //FIX_ARA_ONE_PIECE_FOR_BERSERK
#pragma endregion 렌더링 하지 않아도 되는 장비 리스트 갱신

#pragma region GetAddItemAndRemoveItemToViewListBerserk
bool CX2GUAra::GetAddItemAndRemoveItemToViewListBerserk( OUT CX2EqipPtr& pEquipPtrToRemove_, IN OUT CX2Item** ppItemToAddInViewList_,
	IN CX2Item* const pItemToRemoveInViewList_, OUT int& iIndexInViewListForRemove_ )
{
	if (*ppItemToAddInViewList_ != NULL )		// 장착할et 장비가 있는 경우
	{	
		const CX2Item::ItemTemplet* pItemTempletForItemToAdd = NULL;
		pItemTempletForItemToAdd = (*ppItemToAddInViewList_)->GetItemTemplet();

		if ( pItemTempletForItemToAdd == NULL )
		{
			ASSERT( false );
			return false;
		}

		for ( size_t numViewEquipInUser = 0; numViewEquipInUser < m_vecPromotionEquipList.size(); numViewEquipInUser++ )
		{
			if ( pItemTempletForItemToAdd->GetEqipPosition() == m_vecPromotionEquipList[numViewEquipInUser]->GetEqipPos() )
			{
				// 장착할 장비가 Normal이고 해당 위치에 이미 Fashion 템이 있는 경우에는 기존의 아이템을 View에서 제거 하지 추가도 하지 않음
				if (  pItemTempletForItemToAdd->GetFashion() == false && GetUnit()->IsFashionEquipInThePosition( pItemTempletForItemToAdd->GetEqipPosition() ) == true )
				{
					*ppItemToAddInViewList_ = NULL;
				}
				else 
				{
					pEquipPtrToRemove_ = m_vecPromotionEquipList[numViewEquipInUser];
					// 이부분에서 remove 해야할듯
				}
				iIndexInViewListForRemove_ = numViewEquipInUser;
				break;
			}
		}		
	}
	else if ( pItemToRemoveInViewList_ != NULL )		// 장비를 해제만 했을 경우
	{
		const CX2Item::ItemTemplet* pItemTempletForItemToRemove = NULL;
		pItemTempletForItemToRemove = pItemToRemoveInViewList_->GetItemTemplet();

		if ( pItemTempletForItemToRemove == NULL)
		{
			ASSERT( false );
			return false;
		}

		for ( size_t numViewEquipInUser = 0; numViewEquipInUser < m_vecPromotionEquipList.size(); numViewEquipInUser++ )
		{
			if ( pItemTempletForItemToRemove->GetEqipPosition() == m_vecPromotionEquipList[numViewEquipInUser]->GetEqipPos() )
			{
				switch ( pItemTempletForItemToRemove->GetFashion() )
				{
				case true:	// 해제 할 장비가 패션 템인 경우
					{
						pEquipPtrToRemove_ = m_vecPromotionEquipList[numViewEquipInUser];	// 해제할 장비

						// 해제한 곳에 노말 장비가 있는 경우
						if ( GetUnit()->IsNormalEquipInThePosition( pItemTempletForItemToRemove->GetEqipPosition() ) == true )
						{
							CX2Item* pNormalEquip	= NULL;

							pNormalEquip	= GetUnit()->GetInventory().GetEquippingItemByEquipPos( pItemTempletForItemToRemove->GetEqipPosition(), false );

							if ( pNormalEquip == NULL )
								return false;

							if ( pNormalEquip->GetItemData().m_PeriodType != CX2Item::PT_ENDURANCE		// 내구도가 없는 장비거나
								|| pNormalEquip->GetItemData().m_Endurance > 0)		// 내구도가 0이하가 아니라면 노말 장비를 넣어줌
								*ppItemToAddInViewList_ = pNormalEquip;
							else
								*ppItemToAddInViewList_ = GetUnit()->CreateBasicEquip( pItemTempletForItemToRemove->GetEqipPosition() );

						}
						else	// 해제한 곳에 노말 장비도 없는 경우
						{
							*ppItemToAddInViewList_ = GetUnit()->CreateBasicEquip( pItemTempletForItemToRemove->GetEqipPosition() );
						}
					}
					break;

				case false: // 해제 할 아이템이 노말 템인 경우
					{
						// 해제한 곳에 패션 템도 없는 경우
						if (  GetUnit()->IsFashionEquipInThePosition( pItemTempletForItemToRemove->GetEqipPosition() ) == false )
						{
							pEquipPtrToRemove_		= m_vecPromotionEquipList[numViewEquipInUser];	// 해제할 장비
							*ppItemToAddInViewList_	= GetUnit()->CreateBasicEquip( pItemTempletForItemToRemove->GetEqipPosition() );
						}
					}
					break;
				}

				iIndexInViewListForRemove_ = numViewEquipInUser;
				break;	// for문에 대한
			}
		} // for ( size_t numViewEquipInUser = 0; numViewEquipInUser < m_ViewEqipList.size(); numViewEquipInUser++ )
	}

	return true;
}
#pragma endregion

#pragma region AddBerserkEquipToViewList
void CX2GUAra::AddBerserkEquipToViewList( CX2Item* const pItem )
{
	if ( pItem == NULL || pItem->GetItemTemplet() == NULL )
		return;

	const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
	short ePos = pItemTemplet->GetEqipPosition();

	if ( CX2Unit::EP_WEAPON_HAND != ePos && CX2Unit::EP_DEFENCE_BODY != ePos && CX2Unit::EP_DEFENCE_LEG != ePos &&
		CX2Unit::EP_DEFENCE_HAND != ePos && CX2Unit::EP_DEFENCE_FOOT != ePos )
		return;

	CX2EqipPtr	pCX2EquipPtr;
	int			enchantLevel = 0;

	enchantLevel = pItem->GetItemData().m_EnchantLevel;
    bool bBasicEquip = GetUnit()->IsBasicEquip( pItemTemplet->GetItemID() );

	if( pItemTemplet->GetUseCondition() == CX2Item::UC_ANYONE )
	{
		pCX2EquipPtr = CX2Eqip::CreateEquip( pItem, m_pXSkinAnim, enchantLevel, bBasicEquip, true, GetUnit()->GetType() );
	}
	else
	{
		pCX2EquipPtr = CX2Eqip::CreateEquip( pItem, m_pXSkinAnim, enchantLevel, bBasicEquip, true );
	}

	pCX2EquipPtr->SetManagerObject( this );
	// 새로운 장비에 특수능력이 붙어 있었다면 특수능력 부여해 주기

	if ( IsRecursivelyManagedByDGManager() )
		g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2EquipPtr.get() );

	m_vecPromotionEquipList.push_back( pCX2EquipPtr );

	if ( false == IsFullHyperMode() )
		return;

	int						weaponIndex		= 0;

	switch ( pItemTemplet->GetEqipPosition() )
	{
	case CX2Unit::EP_WEAPON_HAND:
		weaponIndex = 0;				// InitEqip 부분에서 0과 1로 구분되어 있음
		break;

	case CX2Unit::EP_WEAPON_SECOND:
		weaponIndex = 1;
		break;
	}

	// 새로운 장비가 Weapon 일 경우
	switch ( pItemTemplet->GetEqipPosition() )
	{
	case CX2Unit::EP_WEAPON_HAND:
	case CX2Unit::EP_WEAPON_SECOND:
		{
			Weapon* pWeapon = Weapon::CreateWeapon( *this );
			ASSERT( pWeapon != NULL );

			pWeapon->m_iWeaponIndex			= weaponIndex;
			pWeapon->m_pUserWeaponEquip		= pCX2EquipPtr;
			pWeapon->m_bUseSlashTrace		= true;


#ifdef ITEM_SLASH_TRACE_COLOR_TEST
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            pItemTemplet->GetSlashTraceColors( pWeapon->m_coSlashTrace, pWeapon->m_coSlashTraceTip
                , pWeapon->m_coSlashTraceHyper, pWeapon->m_coSlashTraceTipHyper, CX2Unit::UT_ARA );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
			pWeapon->m_coSlashTrace			= pItemTemplet->m_coSlashTrace;
			pWeapon->m_coSlashTraceHyper	= pItemTemplet->m_coSlashTraceHyper;
			pWeapon->m_coSlashTraceTip		= pItemTemplet->m_coSlashTraceTip;
			pWeapon->m_coSlashTraceTipHyper	= pItemTemplet->m_coSlashTraceTipHyper;
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
#endif ITEM_SLASH_TRACE_COLOR_TEST


			if( NULL != g_pX2Game &&
				NULL != g_pX2Game->GetMajorParticle() )
			{
                unsigned uNumParticle = pItemTemplet->GetNumAttachedParticleData();
				for( size_t numParticle = 0; numParticle < uNumParticle; numParticle++ )
				{
                    const wchar_t*  pwszParticleName = L"";
                    const wchar_t*  pwszBoneName = L"";
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                    pItemTemplet->GetAttachedParticleData( numParticle, &pwszParticleName, &pwszBoneName );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					const CX2Item::AttachedParticleData& particleData = pItemTemplet->m_vecAttachedParticleData[numParticle];
                    pwszParticleName = particleData.wstrParticleName.c_str();
                    pwszBoneName = particleData.wstrBoneName.c_str();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  pwszParticleName, D3DXVECTOR3( 0, 0, 0 ) );
					if( NULL != pSeq )
					{
						AttachWeaponParticle( pWeapon->m_iWeaponIndex, true, pwszBoneName, pSeq->GetHandle() );
					}
				}
			}

			m_vecpWeapon.push_back( pWeapon );
		}		
		break;	// switch ( pItemTemplet->GetEqipPosition() )
	}
}
#pragma endregion 

#pragma region RemoveBerserkEquipFromViewList
void CX2GUAra::RemoveBerserkEquipFromViewList( CX2EqipPtr pEquipPtrToRemove, int index )
{
	CX2Unit::EQIP_POSITION ePos = pEquipPtrToRemove->GetEqipPos();

	CX2Unit::EQIP_POSITION	equipPosition	= pEquipPtrToRemove->GetEqipPos();
	int						weaponIndex		= 0;
	
	if ( CX2Unit::EP_WEAPON_HAND != ePos && CX2Unit::EP_DEFENCE_BODY != ePos && CX2Unit::EP_DEFENCE_LEG != ePos &&
		CX2Unit::EP_DEFENCE_HAND != ePos && CX2Unit::EP_DEFENCE_FOOT != ePos )
		return;
	
	m_vecPromotionEquipList.erase( m_vecPromotionEquipList.begin() + index );

	if ( false == IsFullHyperMode() )
		return;

	switch ( equipPosition )
	{
	case CX2Unit::EP_WEAPON_HAND:
		weaponIndex = 0;				// InitEqip 부분에서 0과 1로 구분되어 있음
		break;

	case CX2Unit::EP_WEAPON_SECOND:
		weaponIndex = 1;
		break;
	}

	if ( false == IsFullHyperMode() )
		return;

	switch ( equipPosition )
	{
	case CX2Unit::EP_WEAPON_HAND:		// pWeapon->m_iWeaponIndex이 0인것
	case CX2Unit::EP_WEAPON_SECOND:

		for ( size_t numWeapon = 0; numWeapon < m_vecpWeapon.size(); numWeapon++ )								
		{
			Weapon* pWeapon	= m_vecpWeapon[numWeapon];

			if ( pWeapon->m_iWeaponIndex == weaponIndex )
			{										
				m_vecpWeapon.erase( m_vecpWeapon.begin() + numWeapon );
				Weapon::DeleteKTDGObject( pWeapon );

				--numWeapon;	// added by wonpok, 20100318
			}
		}

		break;	// switch
	}
}
#pragma endregion 

#pragma region SLockOnSystem::ProcessLockOn
void CX2GUAra::SLockOnSystem::ProcessLockOn( CX2GUAra* myUnit )
{
	int iSize = 0;
	float fDistance = m_fPveRange * m_fPveRange;
	vector<CX2GameUnit*> vecAreaUnit;
	D3DXVECTOR3 pos = myUnit->GetPos();
	
	for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
	{
		CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
		if( pUnit->GetTeam() == myUnit->GetTeam() )
			continue;

#ifdef FIELD_BOSS_RAID // 유도 / 타겟 지정에 대한 예외
		if( true == pUnit->GetIsBosRaidNPC() )
			continue;
#endif // FIELD_BOSS_RAID

		if( pUnit->GetInvincible() == true )
			continue;

		if( pUnit->GetNowHp() <= 0.f )
			continue;

		if( pUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
		{
			CX2GUNPC* pNPC = (CX2GUNPC*) pUnit;
			switch( pNPC->GetNPCTemplet().m_ClassType )
			{
			case CX2UnitManager::NCT_THING_TRAP:
			case CX2UnitManager::NCT_THING_BOX:
			case CX2UnitManager::NCT_THING_GATE:
			case CX2UnitManager::NCT_THING_CHECKER:
				{
					continue;
				} break;
			}
		}

		if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
			fDistance = m_fPvpRange * m_fPvpRange;

		if( GetDistance3Sq( pos, pUnit->GetPos() ) < fDistance )
		{
			vecAreaUnit.push_back( pUnit );
		}
	}

	iSize = vecAreaUnit.size();

	if ( 1 < iSize )
	{
		int iCount = 0;

		BOOST_FOREACH( CX2GameUnit* pUnit, vecAreaUnit )
		{
			if ( m_BeforeTargetUID == pUnit->GetUnitUID() )
				vecAreaUnit.erase( vecAreaUnit.begin() + iCount );

			++iCount;
		}
	}

	iSize = vecAreaUnit.size();

	if ( 0 < iSize )
	{
		int iRandValue = myUnit->GetRandomInt() % iSize;
		
		CX2GameUnit* pUnit = vecAreaUnit.at( iRandValue );

		if ( NULL != pUnit )
		{
			m_vDestPos = pUnit->GetPos();
			m_BeforeTargetUID = pUnit->GetUnitUID();


			if( NULL != g_pX2Game->GetWorld() && NULL != g_pX2Game->GetWorld()->GetLineMap() )
			{
				D3DXVECTOR3 outPos;
				int			lineIndex = 0;

				if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_vDestPos, LINE_RADIUS, &outPos, &lineIndex, false ) == true )
				{
					const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( lineIndex );
					if( NULL != pLineData )
					{
						if( pLineData->lineType == CKTDGLineMap::LT_NORMAL && pLineData->bEnable == true )
							SetSafePos( m_vDestPos );
					}
				}
			}
			
			m_bLockOnUnit = true;
		}
		
	}
	else
	{
		m_bLockOnUnit = false;
	}
}
#pragma endregion 

#pragma region OnFrameMove
/** @function	: OnFrameMove
	@brief		: 아라의 프레임 무브 함수 ( 무기의 이펙트를 업데이트 할 필요가 있으면 업데이트함 )
	@param		: double fTime, float fElapsedTime 
	@return		: HRESULT
*/
/*virtual*/ HRESULT CX2GUAra::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2GUUser::OnFrameMove( fTime, fElapsedTime );

	if ( GetNeedToUpdateWeaponEffect() )
	{
		UpdateWeaponEnchantEffectAndExtraDamageType();
		SetNeedToUpdateWeaponEffect( false );
	}

	return S_OK;
}
#pragma endregion

#pragma region DamageReact
/** @function 	: DamageReact
	@brief 		: 피격 당했을 때의 처리 부분

	@param pDamageDate	: DamageDate 포인터
*/
/*virtual*/ void CX2GUAra::DamageReact( OUT CX2DamageManager::DamageData* pDamageData_ )
{
	KTDXPROFILE();

#ifdef ARA_CHANGE_CLASS_FIRST
	/// 아라 1차 전직 소선 - 기력 활성화
	CX2DamageManager::EXTRA_DAMAGE_TYPE& eEDT = pDamageData_->m_ExtraDamage.m_ExtraDamageType;

	switch( eEDT )		/// 해당 EDT( 인첸트 속성 )에 대한 지속시간 감소
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_POISON:
	case CX2DamageManager::EDT_ENCHANT_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN:

	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_MASTER_PIERCING:

	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_MASTER_PIERCING:

	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:

	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SNATCH:
		{
			pDamageData_->m_ExtraDamage.m_fTime *= m_fReduceEDTDurationRate;
		} break;
	}
#endif ARA_CHANGE_CLASS_FIRST

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
	/// 메모 효과 변경으로 인하여 사용되지 않지만, 추후 스킬 개편시 사용 예정
	/*switch( GetNowStateSkillID() )
	{
	case CX2SkillTree::SI_A_ALH_ROCK_CRASH:	/// 바위깨기
		{
			/// 메모 : 바위로 바위 치기(맹호 1식 : 바위깨기) - 피해 데미지 50% 감소
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO6 ) )
			{
				int iReduceDamagePercent  = GetDamageReducePercent() + _CONST_ARA_::ROCK_CRASH_MEMO_RESUCE_DAMAGE;

				if( 100 < iReduceDamagePercent )
					iReduceDamagePercent = 100;

				SetDamageReducePercent( iReduceDamagePercent );
			}
		} break;
	}*/
#endif // ADD_ARA_MEMO

	CX2GUUser::DamageReact( pDamageData_ );

#ifdef ARA_CHANGE_CLASS_FIRST
	CX2GUUser::DamageReactStateChange( pDamageData_, ASI_A_ALH_BREAKING_ATTACK, ASI_A_ALH_BREAKING_AVOIDANCE );
#else
	CX2GUUser::DamageReactStateChange( pDamageData_ );
#endif //ARA_CHANGE_CLASS_FIRST
}
#pragma endregion

#pragma region Win
/** @function 	: Win
	@brief 		: 승리했을 때의 상태로 변경
*/
/*virtual*/ void CX2GUAra::Win()
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
#pragma endregion

#pragma region Lose
/** @function 	: Lose
	@brief 		: 패배했을 때의 상태로 변경
*/
/*virtual*/ void CX2GUAra::Lose()
{
	CX2GUUser::Lose();

	if ( true == IsMyUnit() &&
		0.f < GetNowHp() )
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
#pragma endregion

#pragma region Draw
/** @function	: Draw
	@brief		: 비겼을 때의 상태로 변경
*/
/*virtual*/ void CX2GUAra::Draw()
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
#pragma endregion

#pragma region DeleteMinorParticle
/** @function 	: DeleteMinorParticle
	@brief 		: Minor Particle 메모리 제거
*/
void CX2GUAra::DeleteMinorParticle()
{
	CX2GUUser::DeleteMinorParticle();

	DeleteAraMinorParticle();
}
#pragma endregion

#pragma region IsFullHyperMode
/** @function 	: IsFullHyperMode
	@brief 		: 3각성 여부
*/
/*virtual*/bool CX2GUAra::IsFullHyperMode() const
{
	return GetAraGageData()->GetBerserkMode();
}
#pragma endregion

#pragma region SetFullHyperMode
/** @function 	: SetFullHyperMode
	@brief 		: 3각성 설정
*/
/*virtual*/ void CX2GUAra::SetFullHyperMode( const bool bBerserkMode_ )
{
	GetAraGageData()->SetBerserkModeChanged( true );
	GetAraGageData()->SetBerserkMode( bBerserkMode_ );
}
#pragma endregion

#pragma region ApplyHyperModeBuff
/** @function	: ApplyHyperModeBuff
	@brief		: 각성버프 적용
*/
/*virtual*/ void CX2GUAra::ApplyHyperModeBuff()
{
	CX2GameUnit::ApplyHyperModeBuff();

	//if ( IsFullHyperMode() )	/// 풀각성 시작시 나오는 이펙트
		//g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ARA_BERSERK_MODE_START", this );
}
#pragma endregion

#pragma region HyperModeBuffEffectStart
/** @function	: HyperModeBuffEffectStart
	@brief		: 각성 시 나와야하는 이펙트들 시작
*/
/*virtual*/ void CX2GUAra::HyperModeBuffEffectStart()
{
	// 광폭화
	if ( IsFullHyperMode() )
	{
		SetShowViewList( true );
		// BerserkMode의 Set을 지정해 준 후에 UpdateEquipCollisionData()를 호출해야 한다.
		UpdateEquipCollisionData();
		// 무기에 붙는 이펙트 등을 갱신
		SetNeedToUpdateWeaponEffect( true );
		
 		BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_vecPromotionEquipList )
 		{
 			pEquipPtr->ChangeHyperMode();
 		}
	}
	else
	{
		if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
			m_hHyperBoostRFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightOrange",	0, 0, 0, 0, 0);

		if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
			m_hHyperBoostLFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftOrange",	0, 0, 0, 0, 0);

		if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
			m_hHyperBoostRArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightOrange",	0, 0, 0, 0, 0);

		if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
			m_hHyperBoostLArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftOrange",	0, 0, 0, 0, 0);

		BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_ViewEqipList )
		{
			pEquipPtr->ChangeHyperMode();
		}
	}
}
#pragma endregion

#pragma region HyperModeBuffEffectEnd
/** @function	: HyperModeBuffEffectEnd
	@brief		: 각성 종료시 각성이펙트 해제를 위해 수행되는 함수
*/
/*virtual*/ void CX2GUAra::HyperModeBuffEffectEnd()
{
	CX2GUUser::HyperModeBuffEffectEnd();

	// 광폭화 모드인데, 각성시간이 다 되었으면
	if ( IsFullHyperMode() )
	{
		BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_vecPromotionEquipList )
		{
			pEquipPtr->ChangeNormalMode();
		}

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_CHUNG_BERSERK_MODE_START", this );

		SetFullHyperMode( false );
		SetShowViewList( true );
		UpdateEquipCollisionData();
		SetNeedToUpdateWeaponEffect( true );
		
		BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_vecPromotionEquipList )
		{
			pEquipPtr->ChangeNormalMode();
		}
	}
	else
	{
		BOOST_TEST_FOREACH( CX2EqipPtr, pEquipPtr, m_ViewEqipList )
		{
			pEquipPtr->ChangeNormalMode();
		}
	}
}
#pragma endregion

#pragma region SetSpecificValueByBuffTempletID
/** @function	: SetSpecificValueByBuffTempletID
	@brief		: 각 유닛마다 특정 버프가 실행 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 설정 등...)
	@param		: 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUAra::SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_HYPER_MODE:
		{
			if ( IsFullHyperMode() )		/// 자기 자신 뿐만이 아니라 상대방의 클라이언트에서도 판단 해야 하므로 Now를 비교함
				SetBuffFactorToGameUnitByBuffFactorID( BFI_ARA_FULL_HYPER_MODE );
		} break;

	default:
		break;
	}
}
#pragma endregion

#pragma region UnSetSpecificValueByBuffTempletID
/** @function	: UnSetSpecificValueByBuffTempletID
	@brief		: 각 유닛마다 특정 버프가 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 해제 됬다는 플래그 설정 등...)
	@param		: 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUAra::UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_HYPER_MODE:
		{
			ReserveToFinishBuffTempletFromGameUnit( BTI_ARA_FULL_HYPER_MODE );
		} break;

	default:
		break;
	}
}
#pragma endregion
/** @function : ApplyRenderParam
	@brief : 버프에 의한 renderparam과 함께 추가적으로 해비스텐스에 대한 renderparam을 설정하는 함수
	@param : SkinAnim의 RenderParam 포인터(pRenderParam_)

*/
/*virtual*/ void CX2GUAra::ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
#ifdef RIDING_SYSTEM
	if ( m_bPassDash )
		return;
#endif //RIDING_SYSTEM

#ifdef ARA_CHANGE_CLASS_FIRST
	if( true == m_bInvisibility )
	{
		pRenderParam_->renderType		= CKTDGXRenderer::RT_CARTOON;
		pRenderParam_->outLineColor.a	= m_fRenderParamColorA;
		m_RenderParam.color.a			= m_fRenderParamColorA;
		m_RenderParam.bAlphaBlend = true;
	}
	else 
#endif //ARA_CHANGE_CLASS_FIRST
		CX2GameUnit::ApplyRenderParam( pRenderParam_ );
}

#pragma region ApplyRenderParamOnEmptyRenderParamByBuff
/** @function 	: ApplyRenderParamOnEmptyRenderParamByBuff
	@brief 		: 
*/
/*virtual*/ void CX2GUAra::ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
	if( GetRemainHyperModeTime() > 0.0f )
	{
		pRenderParam_->renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
		pRenderParam_->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
		pRenderParam_->outLineColor		= 0xffcc9933;	// 주황색
	}
	else
	{
		pRenderParam_->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		pRenderParam_->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
		pRenderParam_->outLineColor		= 0xffcc9933;
	}
	
	SetUseWorldColor( true );
}
#pragma endregion

#pragma region ChargeMpAndEtcInTrainingGame
/** @function 	: ChargeMpAndEtcInTrainingGame
	@brief 		: ??
*/
/*virtual*/ void CX2GUAra::ChargeMpAndEtcInTrainingGame()
{
	CX2GUUser::ChargeMpAndEtcInTrainingGame();
}
#pragma endregion

#pragma region GetHandleAraMajorParticleByEnum
/** @function 	: GetHandleAraMajorParticleByEnum
	@brief 		: "아라"만 사용하는 파티클 핸들을 반환
*/
__forceinline CKTDGParticleSystem::CParticleEventSequenceHandle CX2GUAra::GetHandleAraMajorParticleByEnum( ARA_MAJOR_PARTICLE_INSTANCE_ID eVal_ ) const
{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    ASSERT( ARA_MAJOR_PII_END > eVal_ && ARA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	ASSERT( ARA_MAJOR_PII_END > eVal_ && INVALID_PARTICLE_SEQUENCE_HANDLE < eVal_ );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	return m_ahAraMajorParticleInstance[eVal_];
}
#pragma endregion

#pragma region SetHandleAraMajorParticleByEnum
/** @function 	: SetHandleAraMajorParticleByEnum
	@brief 		: "아라"만 사용하는 메이저 파티클 핸들 값 지정
*/
__forceinline void CX2GUAra::SetHandleAraMajorParticleByEnum( ARA_MAJOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )
{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    ASSERT( ARA_MAJOR_PII_END > eVal_ && ARA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	ASSERT( ARA_MAJOR_PII_END > eVal_ && INVALID_PARTICLE_SEQUENCE_HANDLE < eVal_ );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	m_ahAraMajorParticleInstance[eVal_] = hHandle_;
}
#pragma endregion

#pragma region GetHandleAraMinorParticleByEnum
/** @function 	: GetHandleAraMinorParticleByEnum
	@brief 		: 아라만 쓰는 마이너 파티클 중 ENUM 값에 해당하는 파티클 핸들 하나를 얻어옴
*/
__forceinline CKTDGParticleSystem::CParticleEventSequenceHandle	CX2GUAra::GetHandleAraMinorParticleByEnum( ARA_MINOR_PARTICLE_INSTANCE_ID eVal_ ) const
{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    ASSERT( ARA_MAJOR_PII_END > eVal_ && ARA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	ASSERT( ARA_MAJOR_PII_END > eVal_ && INVALID_PARTICLE_SEQUENCE_HANDLE < eVal_ );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	return m_ahAraMinorParticleInstance[eVal_];
}
#pragma endregion

#pragma region SetHandleAraMinorParticleByEnum
/** @function 	: SetHandleAraMinorParticleByEnum
	@brief 		: 청만 쓰는 마이너 파티클 핸들 중 ENUM 값에 해당하는 핸들을 셋팅함
*/
__forceinline void CX2GUAra::SetHandleAraMinorParticleByEnum( ARA_MINOR_PARTICLE_INSTANCE_ID eVal_, ParticleEventSequenceHandle hHandle_ )
{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    ASSERT( ARA_MAJOR_PII_END > eVal_ && ARA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	ASSERT( ARA_MAJOR_PII_END > eVal_ && INVALID_PARTICLE_SEQUENCE_HANDLE < eVal_ );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	m_ahAraMinorParticleInstance[eVal_] = hHandle_;
}
#pragma endregion

#pragma region GetHandleAraMajorMeshByEnum
/** @function 	: GetHandleAraMajorMeshByEnum
	@brief 		: 메이저 메쉬 핸들 얻기
*/
CKTDGXMeshPlayer::CXMeshInstanceHandle CX2GUAra::GetHandleAraMajorMeshByEnum( ARA_MAJOR_MESH_INSTANCE_ID eVal_ ) const
{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    ASSERT( ARA_MAJOR_PII_END > eVal_ && ARA_MAJOR_PARTICLE_INSTANCE_ID(0) <= eVal_ );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	ASSERT( ARA_MAJOR_PII_END > eVal_ && INVALID_PARTICLE_SEQUENCE_HANDLE < eVal_ );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	return m_ahAraMajorMeshInstance[eVal_];
}
#pragma endregion

#pragma region CX2AraGageData
/*virtual*/ CX2GageData* CX2GUAra::CreateGageData()
{
	if ( IsMyUnit() )
	{
		const CX2AraGageData* pAraGageData 
			= static_cast<const CX2AraGageData*>( CX2GageManager::GetInstance()->GetMyGageData() );

		if ( NULL != pAraGageData )
			return new CX2AraGageData( *pAraGageData );
		else
			return new CX2AraGageData();
	}
	else
		return new CX2AraGageData();
}
#pragma endregion

#pragma region AppendDeviceToDeviceList
/** @function 	: AppendDeviceToDeviceList
	@brief 		: 어떤 기능을 하는 함수일까?
*/
/*static*/ void CX2GUAra::AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}
#pragma endregion

#pragma region AppendComponentToDeviceList
/** @function 	: AppendComponentToDeviceList
	@brief 		: 어떤 기능을 하는 함수일까?
*/
/*static*/ void CX2GUAra::AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}
#pragma endregion

#pragma region AppendEffectToDeviceList
/** @function 	: AppendEffectToDeviceList
	@brief 		: 
*/
void CX2GUAra::AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	CX2GUAra::AppendMajorParticleToDeviceList( listInOut_ );
	CX2GUAra::AppendMinorParticleToDeviceList( listInOut_ );
	CX2GameUnit::Weapon::AppendEnchantParticleForSwordToDeviceList( listInOut_, eDamageType_ );
}
#pragma endregion

#pragma region May the force be with you
/** @function 	: UpForcePower
	@brief 		: 기력 칸을 채움
*/
__forceinline void CX2GUAra::UpForcePower( int iUpCount_ )
{
#ifdef ARA_CHANGE_CLASS_FIRST
	if( 0 < m_iGainMpByFpConversion )
	{	
		// 변경 할 FP
		int iChangeForcePower = GetForcePower() + iUpCount_ ; 
		// 초과되는 FP
		const int iExceedFpCount = iChangeForcePower - GetMaxForcePower() ; 
		if( 0 < iExceedFpCount )
		{
			//초과되는 FP는 MP로 전환
			UpNowMp( static_cast<float>(iExceedFpCount * m_iGainMpByFpConversion) );
		}
		SetForcePower( iChangeForcePower );
	}
	else
#endif //ARA_CHANGE_CLASS_FIRST	
	{
		SetForcePower( GetForcePower() + iUpCount_ );
	}
}

/** @function 	: CanUseForcePower
	@brief 		: 기력을 사용할 수 있는지 확인
*/
__forceinline bool CX2GUAra::CanUseForcePower()
{
	return ( false == IsEmptyForcePower() );
}

/** @function 	: IsEmptyForcePower
	@brief 		: 기력이 비었는지 확인
*/
__forceinline bool CX2GUAra::IsEmptyForcePower()
{
	return GetAraGageData()->IsEmptyForcePower();
}

/** @function 	: IsFullForcePower
	@brief 		: 기력이 가득 찼는지 확인
*/
__forceinline bool CX2GUAra::IsFullForcePower()
{
	return GetAraGageData()->IsFullForcePower();
}

/** @function 	: UpForcePowerAtThisAnimationTime
	@brief 		: 해당 애니메이션 시간에 기력 칸을 채움
*/
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
bool CX2GUAra::UpForcePowerAtThisAnimationTime( float fAnimationTimeNeededToChargeForcePower_, int iUpCount_ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( fAnimationTimeNeededToChargeForcePower_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( fAnimationTimeNeededToChargeForcePower_ ) && true == EventCheck( fAnimationTimeNeededToChargeForcePower_, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpForcePower( iUpCount_ );
		return true;
	}

	return false;
}
#else // SERV_ARA_CHANGE_CLASS_SECOND
void CX2GUAra::UpForcePowerAtThisAnimationTime( float fAnimationTimeNeededToChargeForcePower_, int iUpCount_ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( fAnimationTimeNeededToChargeForcePower_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( fAnimationTimeNeededToChargeForcePower_ ) && true == EventCheck( fAnimationTimeNeededToChargeForcePower_, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpForcePower( iUpCount_ );
	}
}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

/** @function 	: UseForcePowerAtThisAnimationTime
	@brief 		: 해당 애니메이션 시간에 기력을 사용
*/
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
bool CX2GUAra::UseForcePowerAtThisAnimationTime( float fAnimationTimeNeededToUseForcePower_, int iUpCount_ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( fAnimationTimeNeededToUseForcePower_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( fAnimationTimeNeededToUseForcePower_ ) && true == EventCheck( fAnimationTimeNeededToUseForcePower_, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UseForcePower( iUpCount_ );
		return true;
	}

	return false;
}
#else // SERV_ARA_CHANGE_CLASS_SECOND
void CX2GUAra::UseForcePowerAtThisAnimationTime( float fAnimationTimeNeededToUseForcePower_, int iUpCount_ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( fAnimationTimeNeededToUseForcePower_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( fAnimationTimeNeededToUseForcePower_ ) && true == EventCheck( fAnimationTimeNeededToUseForcePower_, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UseForcePower( iUpCount_ );
	}
}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

/** @function 	: UseForcePower
	@brief 		: 기력을 사용함
*/
void CX2GUAra::UseForcePower( int iCount_ )
{
	ConsumeForcePower( iCount_ );
	m_ForcePowerSystem.SetUsedForcePowerAtThisState( m_ForcePowerSystem.GetUsedForcePowerAtThisState() + iCount_ );
}

/** @function 	: ConsumeForcePower
	@brief 		: 기력을 소모함
*/
void CX2GUAra::ConsumeForcePower( int iCount_ )
{
	SetForcePower( GetForcePower() - iCount_ );
}

/** @function 	: SetForcePower
	@brief 		: 기력을 설정
*/
void CX2GUAra::SetForcePower( int iForcePower_ )
{
	GetAraGageData()->SetForcePowerChanged( true );
	GetAraGageData()->SetNowForcePower( iForcePower_ ); 
}

/** @function	: GetForcePower
	@brief		: 기력 확인
*/
__forceinline int CX2GUAra::GetForcePower() 
{ 
	return GetAraGageData()->GetNowForcePower(); 
}

#ifdef ARA_CHANGE_CLASS_FIRST
__forceinline int CX2GUAra::GetMaxForcePower()
{
	return GetAraGageData()->GetMaxForcePower();
}
#endif //ARA_CHANGE_CLASS_FIRST

/** @function : DoProcessWhenKillEnemy
	@brief : 적을 죽였을 때 처리해야하는 부분 수행하는 함수
	@param : 죽은 유닛의 포인터
*/
void CX2GUAra::DoProcessWhenKillEnemy( const CX2GameUnit* pDefender_ )
{
	if ( NULL != g_pX2Game && IsFullHyperMode() )	/// 3각성 일때 적을 죽였으면
	{
		SetNowHp( GetNowHp() + GetMaxHp() * 0.03f );	/// HP 채우고z
		SetForcePower( GetForcePower() + 1 );			/// 기력 채움

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_KILL_ENEMY_WHEN_FULL_HYPER_MODE", this );

	}
}

/** @function 	: ResetForcePowerWhenRebirthInPVPGame
	@brief 		: 
*/
void CX2GUAra::ResetForcePowerWhenRebirthInPVPGame( int iNumOfMyTeamKill_, int iNumOfEnmyTeamKill_ )
{
	// 대전에서 죽었다가 부활하는 경우
	// 비기거나 지고 있고
	if ( iNumOfMyTeamKill_ <= iNumOfEnmyTeamKill_ )
	{		
		if ( GetForcePower() < GetBasicForcePower() )
			SetForcePower( GetBasicForcePower() );
	}
	// 아군이 이기고있으면 있으면
	else
	{
		SetForcePower( GetBasicForcePower() );		
	}

	// 이기고 있는데 기본 갯수보다 많으면 그냥 놔둠
}
#pragma endregion 기력 관련 함수

#pragma region ShouldResetNotBusyTimer
/** @function 	: ShouldResetNotBusyTimer
	@brief 		: 광폭화 모드일 때 바쁜 상태아니도록 수정
*/
bool CX2GUAra::ShouldResetNotBusyTimer()
{
	if ( false == CX2GUUser::ShouldResetNotBusyTimer() )
		return false;
	else if ( ASI_HYPER_WAIT == m_FrameDataFuture.syncData.nowState )
		return false;
	else
		return true;	// 리셋 해야 함
}
#pragma endregion 바쁜 상태 확인

/** @function : ChangeAnimTail
	@brief : 구미호 꼬리 장비의 애니메이션 변경
*/
void CX2GUAra::ChangeAnimTail()
{
	wstring wstrAnimNameTail;
	LUA_GET_VALUE( m_LuaManager, "ANIM_NAME_TAIL", wstrAnimNameTail, L"Wait" );

	bool bTransitionTail;
	LUA_GET_VALUE( m_LuaManager, "TRANSITION_TAIL", bTransitionTail, false );

	if ( NULL != m_ptrPromotionEquipTail )
		m_ptrPromotionEquipTail->ChangeAnim( wstrAnimNameTail.c_str(), bTransitionTail );
}

#pragma region state

#pragma region DieFront
/** @function	: DieFrontStartFuture
	@brief		: 앞에서 공격받아 죽는 상태의 StartFuture 함수
*/
void CX2GUAra::DieFrontStartFuture()
{
#ifdef RIDING_SYSTEM
	if ( true == GetRidingOn() && NULL != m_ptrRidingPet )
		m_ptrRidingPet->StateChangeFuture( m_ptrRidingPet->m_DyingState );
#endif //RIDING_SYSTEM

	CommonStateStartFuture();

	const float PLAY_SPEED = 0.5f; 
	m_pXSkinAnimFuture->SetPlaySpeed( PLAY_SPEED );
}

/** @function	: DieFrontStart
	@brief		: 앞에서 공격받아 죽는 상태의 Start 함수
*/
void CX2GUAra::DieFrontStart()
{
	CommonStateStart();
	DyingStart();

	const float PLAY_SPEED = 0.5f; 
	m_pXSkinAnim->SetPlaySpeed( PLAY_SPEED );
}

/** @function	: DieFrontFrameMove
	@brief		: 앞에서 공격받아 죽는 상태의 FrameMove 함수
*/
void CX2GUAra::DieFrontFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

/** @function	: DieFrontEventProcess
	@brief		: 앞에서 공격받아 죽는 상태의 EventProcess 함수
*/
void CX2GUAra::DieFrontEventProcess()
{
	if ( false == GetShowObject() && true == m_pXSkinAnim->IsAnimationEnd() )
		StateChange( GUSI_DIE );
}
#pragma endregion

#pragma region DieBack
/** @function	: DieBackStartFuture
	@brief		: 뒤에서 공격받아 죽는 상태의 StartFuture 함수
*/
void CX2GUAra::DieBackStartFuture()
{
	CommonStateStartFuture();

	const float PLAY_SPEED = 0.5f; 
	m_pXSkinAnimFuture->SetPlaySpeed( PLAY_SPEED );
}

/** @function	: DieBackStart
	@brief		: 뒤에서 공격받아 죽는 상태의 StartFuture 함수
*/
void CX2GUAra::DieBackStart()
{
	CommonStateStart();
	DyingStart();

	const float PLAY_SPEED = 0.5f; 
	m_pXSkinAnim->SetPlaySpeed( PLAY_SPEED );
}

/** @function	: DieBackFrameMove
	@brief		: 뒤에서 공격받아 죽는 상태의 StartFuture 함수
*/
void CX2GUAra::DieBackFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

/** @function : DieBackEventProcess
	@brief : 뒤에서 공격받아 죽는 상태의 StartFuture 함수
*/
void CX2GUAra::DieBackEventProcess()
{
	if ( false == GetShowObject() && true == m_pXSkinAnim->IsAnimationEnd() )
		StateChange( GUSI_DIE );
}
#pragma endregion

#pragma region Wait
/** @function	: WaitStartFuture
	@brief		: Wait 상태의 StartFuture() 함수
*/
void CX2GUAra::WaitStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
}

/** @function	: Wait 상태의 StartFuture() 함수
	@brief		: HP에 따라 Wait or Fatal 모션을 플레이함
*/
void CX2GUAra::WaitStart()
{
	CommonStateStart();
	PlayAnimationWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
	
	m_usArialCount = 0;
	m_usDoubleJumpCount = 0;
}

/** @function	: WaitEventProcess
	@brief		: HP에 따라 Wait or Fatal 모션을 플레이함
*/
void CX2GUAra::WaitEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	} // if
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_COMBO_X );
	}
	else if ( true == SpecialAttackEventProcess() )
	{
	} // else if
	else if ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if ( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_WALK );
	}
	else if ( true == m_InputData.pureUp )	// 점프
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && m_InputData.oneLCtr == true
			 && m_FrameDataFuture.syncData.m_HyperModeCount > 0
			 && ( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) )
	{
		if ( 3 <= m_FrameDataFuture.syncData.m_HyperModeCount )
			StateChange( ASI_BERSERK_MODE );
		else
			StateChange( USI_HYPER_MODE );
	}
	//ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )	// 각성
	else if( true == m_InputData.oneDown &&	
			 true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
																   &m_FrameDataFuture.unitCondition.landPosition, 
																   &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( true == IsFullHyperMode() && m_FrameDataNow.unitCondition.fStateTime >= 0.1f )
	{
		StateChange( ASI_HYPER_WAIT );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	CommonEventProcess();
}
#pragma endregion

#pragma region Hyper Wait
/** @function 	: HyperWaitStartFuture
	@brief 		: 
*/
void CX2GUAra::HyperWaitStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationHyperWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
}

/** @function 	: HyperWaitStart
	@brief 		: 
*/
void CX2GUAra::HyperWaitStart()
{
	CommonStateStart();
	PlayAnimationHyperWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );

	m_usArialCount = 0;
	m_usDoubleJumpCount = 0;

	m_CommonState.m_Wait = ASI_HYPER_WAIT;
}

/** @function 	: HyperWaitEventProcess
	@brief 		: 
*/
void CX2GUAra::HyperWaitEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	} // if
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_COMBO_X );
	}
	else if ( true == SpecialAttackEventProcess() )
	{
	} // else if
	else if ( true == m_InputData.pureDoubleRight || true == m_InputData.pureDoubleLeft )
	{
		StateChangeDashIfPossible();
	}
	else if ( true == m_InputData.pureRight || true == m_InputData.pureLeft )
	{
		StateChange( USI_WALK );
	}
	else if ( true == m_InputData.pureUp )	// 점프
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && m_InputData.oneLCtr == true
		&& m_FrameDataFuture.syncData.m_HyperModeCount > 0
		&& ( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) )
	{
		if ( 3 <= m_FrameDataFuture.syncData.m_HyperModeCount )
			StateChange( ASI_BERSERK_MODE );
		else
			StateChange( USI_HYPER_MODE );
	}
	//ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )	// 각성
	else if( true == m_InputData.oneDown &&	
		true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( false == IsFullHyperMode() )
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

/** @function 	: HyperWaitStateEnd
	@brief 		: 
*/
void CX2GUAra::HyperWaitStateEnd()
{
	m_CommonState.m_Wait = USI_WAIT;

	CommonStateEnd();
}

/** @function 	: PlayAnimationHyperWaitStart
	@brief 		: 
*/
void CX2GUAra::PlayAnimationHyperWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ )
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
			if ( true == IsFullHyperMode() )
				pXSkinAnim_->ChangeAnim( L"BoostPowerWait", false );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", false );
		}
		else
		{
			if ( true == IsFullHyperMode() )
				pXSkinAnim_->ChangeAnim( L"BoostPowerWait", true );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", true );
		}
	}

	pXSkinAnim_->Play( playType );
}
#pragma endregion 

#pragma region Walk
/** @function	: WalkEventProcess
	@brief		: Walk 상태의 EventProcess
*/
void CX2GUAra::WalkEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( true == SpecialAttackEventProcess() )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_COMBO_X );
	}
	else if ( false == m_InputData.pureRight && false == m_InputData.pureLeft )
	{
		StateChange( USI_WAIT );
	}
	else if ( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_UP );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && m_InputData.oneLCtr == true
		&& m_FrameDataFuture.syncData.m_HyperModeCount > 0
		&& ( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) )
	{
		if ( 3 <= m_FrameDataFuture.syncData.m_HyperModeCount )
			StateChange( ASI_BERSERK_MODE );
		else
			StateChange( USI_HYPER_MODE );
	}
	//ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( true == m_InputData.oneDown &&
			 true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
																   &m_FrameDataFuture.unitCondition.landPosition, 
																   &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}
#pragma endregion

#pragma region Jump
/** @function	: JumpReadyEventProcess
	@brief		: 점프하기 바로 직전의 상태 EventProcess
*/
void CX2GUAra::JumpReadyEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
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

/** @function	: JumpUpEventProcess
	@brief		: JumpUp 상태에서의 EventProcess( StateChange를 판단 )
*/
void CX2GUAra::JumpUpEventProcess()
{
	if( true == m_InputData.pureDoubleLeft || true == m_InputData.pureDoubleRight )
	{
		StateChangeAirialDashIfPossible();
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_JUMP_ATTACK_X );
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

	if ( true == m_InputData.oneUp )
	{
		StateChange( ASI_DOUBLE_JUMP_UP );
	}

	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
	}

	CommonEventProcess();
}

/** @function	: JumpDownEventProcess
	@brief		: 하강하는 상태에서의 EventProcess( StateChange를 판단 )
*/
void CX2GUAra::JumpDownEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		if( true == m_bDownForce )
		{
			StateChange( ASI_DASH_JUMP_POWER_LANDING );
			m_bDownForce = false;
			m_pComboGuide->ShowComboCommand(L"J", L"D");
		}
		else
		{
			StateChange( USI_JUMP_LANDING );
			m_bDownForce = false;
		}
	}
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall && ( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ASI_WALL_LANDING );
	}
	else if ( true == m_InputData.pureDoubleLeft || true == m_InputData.pureDoubleRight )
	{
		StateChangeAirialDashIfPossible();
	}
	else if ( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_JUMP_ATTACK_Z );
		m_pComboGuide->ShowComboCommand( L"A", L"AXZ" );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_JUMP_ATTACK_X );
		m_pComboGuide->ShowComboCommand( L"A", L"AXX" );
	}
	else if ( 0.0f <= m_PhysicParam.nowSpeed.y )
	{
		StateChange( USI_JUMP_UP );
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	
	if ( true == m_InputData.oneUp )
	{
		StateChange( ASI_DOUBLE_JUMP_UP );
	}

	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
	}

	CommonEventProcess();
}

/** @function 	: JumpLandingStart
	@brief 		: AirialDash Count 초기화
*/
void CX2GUAra::JumpLandingStart()
{
	CommonStateStart();
	CreateStepDust();

	m_usArialCount = 0;
	m_usDoubleJumpCount = 0;
}

/** @function	: JumpLandingEventProcess
	@brief		: 착지 상태에서의 EventProcess( StateChange를 판단 )
*/
void CX2GUAra::JumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
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
		StateChange( ASI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_COMBO_X );
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
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && m_InputData.oneLCtr == true
		&& m_FrameDataFuture.syncData.m_HyperModeCount > 0
		&& ( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) )
	{
		if ( 3 <= m_FrameDataFuture.syncData.m_HyperModeCount )
			StateChange( ASI_BERSERK_MODE );
		else
			StateChange( USI_HYPER_MODE );
	}
	//ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( true == m_InputData.oneDown &&
			 true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
																   &m_FrameDataFuture.unitCondition.landPosition, 
																   &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( USI_WAIT, false );
	}

	CommonEventProcess();
}
#pragma endregion

#pragma region Dash
/** @function 	: DashStart
	@brief 		: 능공허도 초기화
*/
void CX2GUAra::DashStart()
{
	CommonStateStart();
	CreateStepDust();
	m_usArialCount = 0;
	m_usDoubleJumpCount = 0;
}

/** @function	: DashEventProcess
	@brief		: Dash 상태에서의 EventProcess( StateChange를 판단 )
*/
void CX2GUAra::DashEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChangeAirialDashIfPossible();
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ASI_DASH_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ASI_DASH_COMBO_X );
	}
	else if ( true == SpecialAttackEventProcess() )
	{
	}
	else if( true == m_InputData.oneUp )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && m_InputData.oneLCtr == true
		&& m_FrameDataFuture.syncData.m_HyperModeCount > 0
		&& ( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) )
	{
		if ( 3 <= m_FrameDataFuture.syncData.m_HyperModeCount )
			StateChange( ASI_BERSERK_MODE );
		else
			StateChange( USI_HYPER_MODE );
	}
	//ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
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
	else if( true == m_InputData.oneDown &&
			 true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
																   &m_FrameDataFuture.unitCondition.landPosition, 
																   &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}

/** @function	: DashEndStartFuture
	@brief		: Dash 종료시 스피드 0로 초기화
*/
void CX2GUAra::DashEndStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x = GetDashSpeed();
	m_bApplyMotionOffset = false;
}

/** @function	: DashEndFrameMove
	@brief		: Dash 종료 시 먼지 출력
*/
void CX2GUAra::DashEndFrameMove()
{
	if( true == m_pXSkinAnim->EventTimer( 0.4f ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

/** @function	: DashEndEventProcess
	@brief		: Dash 가 종료되는 상태에서의 EventProcess( StateChange를 판단 )
*/
void CX2GUAra::DashEndEventProcess()
{
	// DashEnd에서 공격 키를 눌렀을 때 대시 공격인지, 일반 공격인지를 정하기 위한 const 값. 
	const float TRIGGER_TIME_FACTOR_DASH_ATTACK = 0.3f;

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( true == m_InputData.oneZ )
	{
		if ( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() * TRIGGER_TIME_FACTOR_DASH_ATTACK )
		{
			StateChange( ASI_COMBO_Z );
		}
		else
		{
			StateChange( ASI_DASH_COMBO_Z );
		}
	}
	else if ( true == m_InputData.oneX )
	{
		if ( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() * TRIGGER_TIME_FACTOR_DASH_ATTACK )
		{
			StateChange( ASI_COMBO_X );
		}
		else
		{
			StateChange( ASI_DASH_COMBO_X );
		}
	}
	else if( SpecialAttackEventProcess() == true)
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
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.oneDown &&
			 true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
																   &m_FrameDataFuture.unitCondition.landPosition, 
																   &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_JUMP_READY );
	}
	else if( false == IsThereEffectiveBlackHoleDamageEffect() && m_InputData.oneLCtr == true
		&& m_FrameDataFuture.syncData.m_HyperModeCount > 0
		&& ( GetRemainHyperModeTime() <= 0.0f || true == GetCanChangeHyperModeInHyperModeState() ) )
	{
		if ( 3 <= m_FrameDataFuture.syncData.m_HyperModeCount )
			StateChange( ASI_BERSERK_MODE );
		else
			StateChange( USI_HYPER_MODE );
	}
	//ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
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
#pragma endregion

#pragma region DashJump
/** @function	: DashJumpStartFuture
	@brief		: 캐릭터의 하강 가속도를 중력 가속도로, DownForce 플래그(점프 상태에서 아래방향 키보드를 눌렀는가) 초기화
*/
void CX2GUAra::DashJumpStartFuture()
{
	CommonStateStartFuture();

	const float MAGIC_NUMBER_MULTIPLY_GRAVITY_ACCEL = 1.2f;

	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * MAGIC_NUMBER_MULTIPLY_GRAVITY_ACCEL;
	m_bDownForce					= false;
}

/** @function	: DashJumpStart
	@brief		: 대시 점프 시작 시 라인맵에 닿아 있는 상태라면 먼지 파티클 출력
*/
void CX2GUAra::DashJumpStart()
{
	CommonStateStart();

	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
	{
		CreateStepDust();
	}
}

/** @function	: DashJumpFrameMoveFuture
	@brief		: 현재 달리고 있는 방향으로 대시 점프 했을 시에 앞으로 좀더 멀리 나가게 함
*/
void CX2GUAra::DashJumpFrameMoveFuture()
{
	const float MAGIC_NUMBER_MULTIPLY_DASH_SPEED = 1.6f;

	if( true == m_InputData.pureRight && true == m_FrameDataNow.syncData.bIsRight )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * MAGIC_NUMBER_MULTIPLY_DASH_SPEED;
	}
	else if( true == m_InputData.pureLeft && false == m_FrameDataNow.syncData.bIsRight )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * MAGIC_NUMBER_MULTIPLY_DASH_SPEED;
	}

	CommonFrameMoveFuture();
}

/** @function	: DashJumpEventProcess
	@brief		: Dash Jump 상태에서의 EventProcess( StateChange를 판단 )
*/
void CX2GUAra::DashJumpEventProcess()
{
	bool &bCanDashJump = m_FrameDataFuture.stateParam.bEventFlagList[0];
	
	if ( false == IsOnSomethingFuture() )
	{
		if( true == m_InputData.pureDoubleLeft || true == m_InputData.pureDoubleRight )
		{
			StateChangeAirialDashIfPossible();
		}
	}
	if( true == IsOnSomethingFuture() )
	{
		// 위쪽 화살표를 한번 더 눌렀으면
		if( true == bCanDashJump )
		{
			StateChange( USI_DASH_JUMP );	// 계속해서 대시 점프
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
			m_bDownForce = false;
		}
		else if ( true == m_bDownForce )
		{
			StateChange( ASI_DASH_JUMP_POWER_LANDING );
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
	else if( true == m_FrameDataFuture.unitCondition.bFootOnWall &&
			 ( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ASI_WALL_LANDING );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_DASH_JUMP_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_DASH_JUMP_COMBO_X );
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

	if ( true == m_InputData.pureLeft )
	{
		if ( true == m_InputData.oneUp )
			if ( false == m_FrameDataFuture.syncData.bIsRight )
			{
				StateChange( ASI_DOUBLE_JUMP_FRONT );
			}
	}
	else if ( true == m_InputData.pureRight )
	{
		if ( true == m_InputData.oneUp )
			if ( true == m_FrameDataFuture.syncData.bIsRight )
			{
				StateChange( ASI_DOUBLE_JUMP_FRONT );
			}
	}

	if( true == m_InputData.oneUp  && 0.0f >= m_PhysicParam.nowSpeed.y  )
	{
		bCanDashJump = true;
	}
	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
		bCanDashJump = false;
	}

	CommonEventProcess();
}

/** @function	: DashJumpEndFuture
	@brief		: 하강 가속도 초기화
*/
void CX2GUAra::DashJumpEndFuture()
{
	CommonStateEndFuture();
	m_PhysicParam.fDownAccel	= 0.0f;
}

/** @function	: DashJumpLandingEventProcess
	@brief		: Dash Jump 의 착지 상태에서의 EventProcess( StateChange를 판단 ) 
*/
void CX2GUAra::DashJumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_COMBO_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_COMBO_X );
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
		StateChange( USI_WALK );
	}
	else if( true == m_InputData.pureUp )
	{
		StateChange( USI_JUMP_READY );
	}
	else if( true == m_InputData.oneDown &&
		true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
		&m_FrameDataFuture.unitCondition.landPosition, 
		&m_FrameDataFuture.syncData.lastTouchLineIndex ) )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}		
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#pragma endregion

#pragma region PowerLanding
/** @function	: PowerLandingStartFuture
	@brief		: Dash Jump Power Landing 시작시 값 셋팅
*/
void CX2GUAra::DashJumpPowerLandingStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x *= 0.5f;
}

/** @function	: DashJumpPowerLandingStart
	@brief		: 착지할 때 먼지 이펙트 출력
*/
void CX2GUAra::DashJumpPowerLandingStart()
{
	CommonStateStart();
	CreateStepDust();
	m_usArialCount = 0;
	m_usDoubleJumpCount = 0;
}

/** @function	: DashJumpPowerLandingEventProcess
	@brief		: Dash Jump Power Landing 상태에서의 EventProcess( StateChange를 판단 ) 
*/
void CX2GUAra::DashJumpPowerLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if( true == m_InputData.oneRight || true == m_InputData.oneLeft )
	{
		StateChange( USI_WALK );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 6.0f )
	{
		if( m_InputData.oneZ == true )
		{
			StateChange( ASI_COMBO_Z );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( ASI_COMBO_X );
		}
		if( true == SpecialAttackEventProcess() )
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
		else if( true == m_InputData.pureUp )
		{
			StateChange( USI_JUMP_READY );
		}
		else if( true == m_InputData.oneDown &&
				 true == g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS,
																	   &m_FrameDataFuture.unitCondition.landPosition,
																	   &m_FrameDataFuture.syncData.lastTouchLineIndex ) )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}
#pragma endregion

#pragma region HyperMode
/** @function	: HyperModeFrameMove
	@brief		: 각성 상태에서 표현되야하는 파티클 출력 및 각성 구슬 카운트 수행
*/
void CX2GUAra::HyperModeFrameMove()
{
#ifdef MODIFY_RIDING_PET_AWAKE
	CommonHyperModeFrameMove( 0.693f, 0.759f, 1.287f );
#else
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.693f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.693f ) && true == EventCheck( 0.693f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
else if( true == m_pXSkinAnim->EventTimerOneshot( 0.759f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 0.759f ) && true == EventCheck( 0.759f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();

		ApplyHyperModeBuff();
	}
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( 1.287f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 1.287f ) && true == EventCheck( 1.287f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == IsFullHyperMode() )
			PlaySound( L"AraVoice_BoostPower.ogg" );		
	}

	CommonFrameMove();
#endif // MODIFY_RIDING_PET_AWAKE
}
#pragma endregion

#pragma region DamageSmall
/** @function : DamageSmallEventProcess
	@brief : 앞 또는 뒤에서 small 타격을 받았을 때의 EventProcess
*/
void CX2GUAra::DamageSmallEventProcess()
{
	float fGapFromUnitToLineMap = m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y;
	const float ACCEPTABLE_GAP = 200.0f;
	
	if( ACCEPTABLE_GAP < fGapFromUnitToLineMap )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_DAMAGE_AIR_SMALL );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#pragma endregion

#pragma region DamageBig
/** @function	: DamageBigStart
	@brief		: 앞 또는 뒤에서 Big 타격을 받았을 때 먼지 파티클 출력
*/
void CX2GUAra::DamageBigStart()
{
	CommonStateStart();
	CreateStepDust();
}

/** @function	: DamageBigEventProcess
	@brief		: 앞 또는 뒤에서 Big 타격을 받았을 때의 EventProcess
*/
void CX2GUAra::DamageBigEventProcess()
{
	float fGapFromUnitToLineMap = m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y;
	const float ACCEPTABLE_GAP = 200.0f;	// 이것보다 거리가 크면 공중에 있는 상태임
	
	if( ACCEPTABLE_GAP < fGapFromUnitToLineMap )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_DAMAGE_AIR_SMALL );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
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
#pragma endregion

#pragma region DamageDown
/** @function : DamageDownFrontFrameMove
	@brief : 앞에서 공격 받아 넘어 진 상태의 FrameMove
*/
void CX2GUAra::DamageDownFrontFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.19f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.19f ) && true == EventCheck( 0.19f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

/** @function : DamageDownFrontEventProcess
	@brief : 앞에서 공격 받아 넘어 진 상태의 EventProcess
*/
void CX2GUAra::DamageDownFrontEventProcess()
{
	bool& bChangeStandUpAttackFront			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingFrontFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingFrontBack	= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackFront		= m_FrameDataFuture.stateParam.bEventFlagList[4];
#ifdef ARA_CHANGE_CLASS_FIRST
	//재빠른 기상 배웠다면 스테이트에서 지정 한 시간으로 변경
	float fInputStartTime = ( true == m_bAbleQuickStand ) ? m_fEventTime[0].keyInputStart : 0.66f;
#endif //ARA_CHANGE_CLASS_FIRST
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS ;
	}
	// 넘어진 후
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		if( true == bChangeStandUpAttackFront )
		{
			StateChange( ASI_STANDUP_ATTACK_FRONT );
		}
		else if ( true == bChangeStandUpBigAttackFront )
		{
#ifdef ARA_CHANGE_CLASS_FIRST
			//재빠른 기상 확률에 의해 쓰러뜨리지 않는 공격으로 변환
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( ASI_STANDUP_ATTACK_FRONT_NO_DOWN );
			}
			else
#endif //ARA_CHANGE_CLASS_FIRST
			{
				StateChange( ASI_STANDUP_ATTACK_FRONT );
			}
		}
		else if( true == bChangeStandUpRollingFrontFront )
			StateChange( ASI_STANDUP_ROLLING_FRONT_FRONT );
		else if( true == bChangeStandUpRollingFrontBack )
			StateChange( ASI_STANDUP_ROLLING_FRONT_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_FRONT );
	}
#ifdef ARA_CHANGE_CLASS_FIRST
	// 오현빈 // 2013-01-10 // 소선,제천의 경우에만 true == m_bAbleQuickStand가 성립하기 때문에 직업 체크는 하지 않음.
	ELSE_IF_STATE_CHANGE_ON_EX_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontFront, ASI_P_ALH_QUICK_STAND_FRONT_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingFrontBack, ASI_STANDUP_ROLLING_FRONT_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_( 7, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpAttackFront, ASI_STANDUP_ATTACK_FRONT )
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
#else
	else if( 0.66f < m_pXSkinAnimFuture->GetNowAnimationTime() )
#endif //ARA_CHANGE_CLASS_FIRST
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

/** @function : DamageDownBackFrameMove
	@brief : 뒤에서 공격 받아 넘어 진 상태의 FrameMove
*/
void CX2GUAra::DamageDownBackFrameMove()
{
	if( true == m_pXSkinAnim->EventTimer( 0.32f ) )
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

/** @function : DamageDownBackEventProcess
	@brief : 뒤에서 공격 받아 넘어 진 상태의 EventProcess
*/
void CX2GUAra::DamageDownBackEventProcess()
{
	bool& bChangeStandUpAttackBack			= m_FrameDataFuture.stateParam.bEventFlagList[1];
	bool& bChangeStandUpRollingBackFront	= m_FrameDataFuture.stateParam.bEventFlagList[2];
	bool& bChangeStandUpRollingBackBack		= m_FrameDataFuture.stateParam.bEventFlagList[3];
	bool& bChangeStandUpBigAttackBack		= m_FrameDataFuture.stateParam.bEventFlagList[4];
#ifdef ARA_CHANGE_CLASS_FIRST
	//재빠른 기상 배웠다면 스테이트에서 지정 한 시간으로 변경
	float fInputStartTime = ( true == m_bAbleQuickStand ) ? m_fEventTime[0].keyInputStart : 0.66f;
#endif //ARA_CHANGE_CLASS_FIRST
	// 넘어졌는데 밑에 라인맵 등이 없는 경우
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS ;
	}
	// 넘어진 후
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{	
		// Z 키를 눌렀을 때
		if( true == bChangeStandUpAttackBack )
		{
			StateChange( ASI_STANDUP_ATTACK_BACK );
		}
		// X 키를 눌렀을 때
		else if ( true == bChangeStandUpBigAttackBack )
		{
#ifdef ARA_CHANGE_CLASS_FIRST
			//재빠른 기상 확률에 의해 쓰러뜨리지 않는 공격으로 변환
			if( GetRandomInt() < m_SkillRelatedData.m_iNoDownDamageAtStandUpAttackPercent )
			{
				StateChange( ASI_STANDUP_ATTACK_BACK_NO_DOWN );
			}
			else
#endif //ARA_CHANGE_CLASS_FIRST
			{
				StateChange( ASI_STANDUP_ATTACK_BACK );
			}				
		}
		else if( true == bChangeStandUpRollingBackFront )
			StateChange( ASI_STANDUP_ROLLING_BACK_FRONT );
		else if( true == bChangeStandUpRollingBackBack )
			StateChange( ASI_STANDUP_ROLLING_BACK_BACK );
		else
			StateChange( USI_DAMAGE_STANDUP_BACK );
	}
#ifdef ARA_CHANGE_CLASS_FIRST
	// 오현빈 // 2013-01-10 // 소선,제천의 경우에만 true == m_bAbleQuickStand가 성립하기 때문에 직업 체크는 하지 않음.
	ELSE_IF_STATE_CHANGE_ON_EX_( 5, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingBackFront, ASI_P_ALH_QUICK_STAND_BACK_FRONT )
	ELSE_IF_STATE_CHANGE_ON_EX_( 6, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpRollingBackBack, ASI_P_ALH_QUICK_STAND_BACK_BACK )
	ELSE_IF_STATE_CHANGE_ON_EX_( 7, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_bAbleQuickStand && true == bChangeStandUpAttackBack, ASI_STANDUP_ATTACK_BACK )
	else if( fInputStartTime < m_pXSkinAnimFuture->GetNowAnimationTime() )
#else
	else if( 0.66f < m_pXSkinAnimFuture->GetNowAnimationTime() )
#endif //ARA_CHANGE_CLASS_FIRST
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

/** @function	: DamageAirDownLandingEventProcess
	@brief		: 공중에서 다운 공격 받아 땅에 닿은 상태의 EventProcess
*/
void CX2GUAra::DamageAirDownLandingEventProcess()
{
	DamageDownFrontEventProcess();
}
#pragma endregion

#pragma region StandUp
/** @function	: StandUpRollingEventProcess
	@brief		: 구르면서 일어서는 상태의 EventProcess
*/
void CX2GUAra::StandUpRollingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );

		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
		{
			StateChangeDashIfPossible();
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

/** @function	: StandUpRollingEnd
	@brief		: 구르면서 일어서는 상태의 End
*/
void CX2GUAra::StandUpRollingEnd()
{
	CommonStateEnd();

	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
	/// 메모 : 물 흐르듯이 (이화접목) - 회피율 50% 증가
	if( ASI_A_ALH_BREAKING_AVOIDANCE == GetNowStateID() &&
		true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO7 ) )	/// 이화 접목 회피중이고, 메모 적용중일 때
	{
		SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_BREAKING_MEMO ) );
	}
#endif // ADD_ARA_MEMO
}

/** @function	: StandUpAttackEventProcess
	@brief		: 일어서면서 때리는 상태의 EventProcess
*/
void CX2GUAra::StandUpAttackEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );

			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		{
			if( true == m_InputData.pureRight || true == m_InputData.pureLeft )
			{
				StateChangeDashIfPossible();
			}
			else
				StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

/** @function	: StandUpAttackEnd
	@brief		: 일어서면서 때리는 상태의 End
*/
void CX2GUAra::StandUpAttackEnd()
{
	CommonStateEnd();

	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}
#pragma endregion

#pragma region AirialDash
/** @function 	: AirialDashStartFuture
	@brief 		: 능공허도 중력 무시
*/
void CX2GUAra::AirialDashStartFuture()
{
	CommonStateStartFuture();

	m_bDownForce = false;
	m_bDisableGravity = true;
	// 패시브에 의해서 시간이 바꿜 수 있음
	m_fAirialDashTime = 0.f;
	++m_usArialCount;
}

/** @function 	: AirialDashStart
	@brief 		: 
*/
void CX2GUAra::AirialDashStart()
{
	CommonStateStart();

	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
		CreateStepDust();
}

/** @function 	: AirialDashFrameMoveFuture
	@brief 		: 시간 처리
*/
void CX2GUAra::AirialDashFrameMoveFuture()
{
	m_fAirialDashTime += m_fElapsedTime;

	if( false == m_InputData.pureDown && true == m_InputData.pureRight && true == m_FrameDataNow.syncData.bIsRight )
	{
		m_PhysicParam.nowSpeed.x = m_PhysicParam.passiveSpeed.x;
		m_PhysicParam.nowSpeed.y = max( 200.f - ( m_fAirialDashTime * 500), -1200.f );
	}
	else if( false == m_InputData.pureDown && true == m_InputData.pureLeft && false == m_FrameDataNow.syncData.bIsRight )
	{
		m_PhysicParam.nowSpeed.x = m_PhysicParam.passiveSpeed.x;
		m_PhysicParam.nowSpeed.y = max( 200.f - ( m_fAirialDashTime * 500), -1200.f );
	}
	else
	{
		StateChange( USI_DASH_JUMP );
	}
	
	CommonFrameMoveFuture();
}

/** @function 	: AirialDashFrameMove
	@brief 		: 
*/
void CX2GUAra::AirialDashFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Airial_Dash", this );
	}

	CommonFrameMove();
}

/** @function 	: AirialDashEventProcess
	@brief 		: 능공허도에서 이어지는 이벤트 처리
*/
void CX2GUAra::AirialDashEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( USI_DASH_JUMP );
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
			m_bDownForce = false;
		}
		else
		{
			StateChange( USI_DASH );
			m_bDownForce = false;
		}

	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ASI_DASH_JUMP_COMBO_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ASI_DASH_JUMP_COMBO_X );
	}

	if ( true == m_InputData.pureLeft )
	{
		if ( true == m_InputData.oneUp )
			if ( false == m_FrameDataFuture.syncData.bIsRight )
			{
				StateChange( ASI_DOUBLE_JUMP_FRONT );
			}
	}
	else if ( true == m_InputData.pureRight )
	{
		if ( true == m_InputData.oneUp )
			if ( true == m_FrameDataFuture.syncData.bIsRight )
			{
				StateChange( ASI_DOUBLE_JUMP_FRONT );
			}
	}

	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Airial_Dash", this );

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

	CommonEventProcess();
}

/** @function 	: AirialDashEndFuture
	@brief 		: 
*/
void CX2GUAra::AirialDashEndFuture()
{
	CommonStateEndFuture();

	//if( m_PhysicParam.nowSpeed.x > GetDashSpeed() )
		//m_PhysicParam.nowSpeed.x = GetDashSpeed();

	m_bDisableGravity = false;
}
#pragma endregion

#pragma region Double Jump
/** @function 	: DoubleJumpStartFuture
	@brief 		: 
*/
void CX2GUAra::DoubleJumpStartFuture()
{
	CommonStateStartFuture();

	m_bDownForce = false;
	++m_usDoubleJumpCount;
}

/** @function 	: DoubleJumpFrontEventProcess
	@brief 		: 
*/
void CX2GUAra::DoubleJumpFrontEventProcess()
{
	bool &bCanDashJump = m_FrameDataFuture.stateParam.bEventFlagList[0];

	if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
#ifdef ARA_CHANGE_CLASS_FIRST
		if( CX2Unit::UC_ARA_SAKRA_DEVANAM == GetUnitClass() )
		{
			if( (true == GetIsRight() && true == m_InputData.pureLeft) || // 뒤로 가는 방향키를 입력 했다면
				( false == GetIsRight() && true == m_InputData.pureRight ) )
			{
				StateChange( ASI_DOUBLE_JUMP_FRONT_COMBO_ASD_Z_BACK );
			}
			else
			{
				StateChange( ASI_DASH_JUMP_COMBO_Z );
			}
		}
		else
#endif //ARA_CHANGE_CLASS_FIRST
		{
			StateChange( ASI_DASH_JUMP_COMBO_Z );
		}
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_DASH_JUMP_COMBO_X );
	}
	else if ( true == IsOnSomethingFuture() )
	{
		// 위쪽 화살표를 한번 더 눌렀으면
		if( true == bCanDashJump )
		{
			StateChange( USI_DASH_JUMP );	// 계속해서 대시 점프
			m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
			m_bDownForce = false;
		}
		else if ( true == m_bDownForce )
		{
			StateChange( ASI_DASH_JUMP_POWER_LANDING );
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

	if( true == m_InputData.oneUp  && 0.0f >= m_PhysicParam.nowSpeed.y  )
	{
		bCanDashJump = true;
	}
	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
		bCanDashJump = false;
	}
}

/** @function 	: DoubleJumpUpEventProcess
	@brief 		: 
*/
void CX2GUAra::DoubleJumpUpEventProcess()
{
	if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_JUMP_ATTACK_X );
	}
	else if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	
	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
	}

	CommonEventProcess();
}

/** @function 	: DoubleJumpBackEventProcess
	@brief 		: 
*/
void CX2GUAra::DoubleJumpBackEventProcess()
{
	if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if ( true == m_InputData.oneZ )
	{
		StateChange( ASI_JUMP_ATTACK_Z );
	}
	else if ( true == m_InputData.oneX )
	{
		StateChange( ASI_JUMP_ATTACK_X );
	}
	else if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}

	if( true == m_InputData.oneDown )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed * 2;
		m_bDownForce = true;
	}
}
#pragma endregion 

#pragma region ComboZ
/** @function	: ComboZEventProcess
	@brief		: 상태변경 여부를 판단, 수행( ComboZZ )
*/
void CX2GUAra::ComboZEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
								 m_InputData.oneZ == true, ASI_COMBO_ZZ )

	CommonEventProcess();
}
#pragma endregion 81

#pragma region ComboZZ
/** @function	: ComboZZEventProecss
	@brief		: 상태변경 여부를 판단, 수행( ComboZZZ, ComboZZX )
*/
void CX2GUAra::ComboZZEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ASI_COMBO_ZZZ )
		ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneX == true, ASI_COMBO_ZZX )

		CommonEventProcess();
}
#pragma endregion 82

#pragma region ComboZZZ
/** @function 	: ComboZZZFrameMove
	@brief 		: 데미지 데이터 변경
*/
void CX2GUAra::ComboZZZFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && true == EventCheck( m_fDamageDataChangeTime, false ) )
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
		}
	}

	CommonFrameMove();
}

/** @function	: ComboZZZEventProecss
	@brief		: 상태변경 여부를 판단, 수행( ComboZZZZ )
*/
void CX2GUAra::ComboZZZEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneZ == true && ( CX2Unit::UC_ARA_LITTLE_DEVIL == GetUnitClass() || CX2Unit::UC_ARA_YAMA_RAJA == GetUnitClass() ) &&
		( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ||
		( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) )
		, ASI_COMBO_ALD_ZZZFrontZ )
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ASI_COMBO_ZZZZ )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneX == true, ASI_COMBO_ZZZXStart )

	BWALK_CANCEL_AFTER( m_fBWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 83

#pragma region ComboZZZZ
/** @function	: ComboZZZZEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboZZZZEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
#pragma endregion 84

#pragma region ComboZZZX
/** @function 	: ComboZZZXStartEventProcess
	@brief 		: 
*/
void CX2GUAra::ComboZZZXStartEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ASI_COMBO_ZZZXCharge );
	}
}

/** @function 	: ComboZZZXChargeStateStart
	@brief 		: 
*/
void CX2GUAra::ComboZZZXChargeStateStart()
{
	CommonStateStart();

	m_fZZZXChargeTime = 0.f;
	SetNoDetonation( true );
}

/** @function 	: ComboZZZXFrameMove
	@brief 		: 해당 프레임에 변경될 사항 처리
*/
void CX2GUAra::ComboZZZXChargeFrameMove()
{
	CommonFrameMove();

	if ( 2.f >= m_fZZZXChargeTime )
		m_fZZZXChargeTime += m_fElapsedTime;

	if ( 2.f <= m_fZZZXChargeTime )
		m_fZZZXChargeTime = 2.f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.495f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.495f ) && true == EventCheck( 0.495f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ZpushX_Charge", this, NULL,
			false, GetPowerRate() );
		if ( INVALID_EFFECTSET_HANDLE != hEffectSet )
			m_vecEffectSetToDeleteOnStateEnd.push_back( hEffectSet );
	}
}

/** @function	: ComboZZZXEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboZZZXChargeEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( false == m_InputData.pureX || true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ASI_COMBO_ZZZXAttack );
	}

	CommonEventProcess();
}

/** @function 	: ComboZZZXChargeStateEnd
	@brief 		: 
*/
void CX2GUAra::ComboZZZXChargeStateEnd()
{
	CommonStateEnd();

	SetNoDetonation( false );
}

/** @function 	: ComboZZZXAttackFrameMove
	@brief 		: 
*/
void CX2GUAra::ComboZZZXAttackFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( m_pXSkinAnim->EventTimerOneshot( 0.331f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.331f ) == true && EventCheck( 0.331f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.damage.fPhysic = m_DamageData.damage.fPhysic * ( 1.f + min( m_fZZZXChargeTime, 2.f ) );

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ZpushX_Attack", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();

	if ( 2.f <= m_fZZZXChargeTime )
	{
		m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
		m_FrameDataNow.stateParam.bSuperArmor = true;
	}
}

/** @function	: ComboZZZXAttackEventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboZZZXAttackEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/** @function 	: ComboZZZXAttackStateEnd
	@brief 		: 
*/
void CX2GUAra::ComboZZZXAttackStateEnd()
{
	CommonStateEnd();

	m_FrameDataNow.stateParam.bSuperArmorNotRed = true;
	m_FrameDataNow.stateParam.bSuperArmor = false;
}
#pragma endregion 

#pragma region ComboZZX
/** @function	: ComboZZEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboZZXEventProcess()
{
	if ( 0.462f < m_pXSkinAnim->GetNowAnimationTime() && true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef ARA_CHANGE_CLASS_FIRST
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && 
		( CX2Unit::UC_ARA_LITTLE_HSIEN == GetUnitClass() || CX2Unit::UC_ARA_SAKRA_DEVANAM == GetUnitClass() ), 
		ASI_COMBO_ALH_ZZXX )
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		true == m_InputData.oneX && 
		( CX2Unit::UC_ARA_LITTLE_DEVIL == GetUnitClass() || CX2Unit::UC_ARA_YAMA_RAJA == GetUnitClass() ), 
		ASI_COMBO_ALD_ZZXX )
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	
	CommonEventProcess();
}
#pragma endregion 86

#pragma region ComboX
/** @function	: ComboXEventProecss
	@brief		: 상태변경 여부를 판단, 수행( ComboXX )
*/
void CX2GUAra::ComboXEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true, ASI_COMBO_XX )

	CommonEventProcess();
}
#pragma endregion 87

#pragma region ComboXX
/** @function	: ComboXXEventProecss
	@brief		: 상태변경 여부를 판단, 수행( ComboXXX, ComboXXdownX )
*/
void CX2GUAra::ComboXXEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( true == m_FrameDataFuture.stateParam.bEventFlagList[0] 
			  && m_pXSkinAnimFuture->GetNowAnimationTime() > m_fEventTime[0].stateChange )
	{
		StateChange( ASI_COMBO_XXX );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if ( true == m_FrameDataFuture.stateParam.bEventFlagList[1] 
			  && m_pXSkinAnimFuture->GetNowAnimationTime() > m_fEventTime[0].stateChange )
	{
		StateChange( ASI_COMBO_XXdownX );
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if ( m_pXSkinAnimFuture->GetNowAnimationTime() < m_fEventTime[0].keyInputEnd
			  && m_pXSkinAnimFuture->GetNowAnimationTime() >= m_fEventTime[0].keyInputStart )
	{
		if ( true == m_InputData.oneX )
		{
			if ( true == m_InputData.pureDown )
				m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
			else
				m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
	}
	
	CommonEventProcess();
}
#pragma endregion 88

#pragma region ComboXXX
/** @function 	: ComboXXXStateStart
	@brief 		: XXXLoopCount 초기화
*/

void CX2GUAra::ComboXXXStateStart()
{
	m_uiComboXXXLoopCount = 0;
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}
/** @function	: ComboXXXEventProecss
	@brief		: 상태변경 여부를 판단, 수행( ComboXXXloop )
*/
void CX2GUAra::ComboXXXEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneDoubleX == true, ASI_COMBO_XXXloop )
		
	CommonEventProcess();
}
#pragma endregion 89

#pragma region ComboXXXloop
/** @function 	: ComboXXXloopStateStart
	@brief 		: 
*/
void CX2GUAra::ComboXXXloopStateStart()
{
	++m_uiComboXXXLoopCount;
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}

/** @function 	: ComboXXXloopFrameMove
	@brief 		: 
*/
void CX2GUAra::ComboXXXloopFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && true == EventCheck( m_fDamageDataChangeTime, false ) )
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
		}
	}

	CommonFrameMove();
}

/** @function	: ComboXXXloopEventProecss
	@brief		: 상태변경 여부를 판단, 수행( ComboXXXloopX )
*/
void CX2GUAra::ComboXXXloopEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ASI_COMBO_XXXloopX );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true && m_uiComboXXXLoopCount < 2, ASI_COMBO_XXXloop )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
#pragma endregion 90

#pragma region ComboXXXloopX
/** @function 	: ComboXXXloopXStateStart
	@brief 		: 슈퍼 아머 외곽선 켜기
*/
void CX2GUAra::ComboXXXloopXStateStart()
{
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}

/** @function 	: ComboXXXloopXFrameMove
	@brief 		: DamageData 변경
*/
void CX2GUAra::ComboXXXloopXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && true == EventCheck( m_fDamageDataChangeTime, false ) )
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
		}		
	}

	CommonFrameMove();
}

/** @function	: ComboXXXloopXEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboXXXloopXEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	
	CommonEventProcess();
}

/** @function 	: ComboXXXloopXStateEnd
	@brief 		: 
*/
void CX2GUAra::ComboXXXloopXStateEnd()
{
	CommonStateEnd();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = true;
}
#pragma endregion 91

#pragma region ComboXXdownX
/** @function 	: ComboXXdownXFrameMove
	@brief 		: DamageData 변경
*/
void CX2GUAra::ComboXXdownXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && true == EventCheck( m_fDamageDataChangeTime, false ) )
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
		}
	}

	CommonFrameMove();
}

/** @function	: ComboEventProecss
	@brief		: 상태변경 여부를 판단, 수행( ComboXXdownXX )
*/
void CX2GUAra::ComboXXdownXEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true, ASI_COMBO_XXdownXX )

	CommonEventProcess();
}
#pragma endregion 92

#pragma region ComboXXdownXX
/** @function	: ComboEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboXXdownXXEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
#ifdef FIX_SKILL_CANCEL_ARA

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	else if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	else if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	{
		StateChangeSpecialAttack( m_eSkillID );
	}
#else
	else if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
	{
		StateChangeSpecialAttack( m_pSkillTemplet );
	}
#endif

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}
#pragma endregion 93

#pragma region ComboDashZ
/** @function 	: ComboDashZFrameMove
	@brief 		: 데미지 변경
*/
void CX2GUAra::ComboDashZFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && true == EventCheck( m_fDamageDataChangeTime, false ) )
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
		}		
	}

	CommonFrameMove();
}

/** @function	: ComboDashZEventProcess
	@brief		: 상태변경 여부를 판단, 수행( ComboZpushX_CHARGE )
*/
void CX2GUAra::ComboDashZEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	/*if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}*/
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.pureX == true, ASI_DASH_COMBO_ZpushX_CHARGE )

#ifdef ARA_CHANGE_CLASS_FIRST
		ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneZ == true &&
		( CX2Unit::UC_ARA_LITTLE_HSIEN == GetUnitClass() || CX2Unit::UC_ARA_SAKRA_DEVANAM == GetUnitClass() ), 
		ASI_DASH_COMBO_ALH_ZZ )
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		m_InputData.oneZ == true && CX2Unit::UC_ARA_YAMA_RAJA == GetUnitClass(), ASI_DASH_COMBO_AYR_ZZ )
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	CommonEventProcess();
}
#pragma endregion 94

#pragma region ComboDashZpushX_Charge
/** @function 	: ComboDashZpushX_ChargeStateStart
	@brief 		: 충전 시간 초기화, 마나 브레이크 끄기
*/
void CX2GUAra::ComboDashZpushX_ChargeStateStart()
{
	CommonStateStart();

	m_fZpushXChargeTime = 0.f;
	SetNoDetonation( true );
}

/** @function 	: ComboDashZpushX_ChargeFrameMove
	@brief 		: 충전 시간 갱신 및 2초에 강제 종료
*/
void CX2GUAra::ComboDashZpushX_ChargeFrameMove()
{
	CommonFrameMove();

	if ( 2.f >= m_fZpushXChargeTime )
		m_fZpushXChargeTime += m_fElapsedTime;

	if ( 2.f <= m_fZpushXChargeTime )
		m_fZpushXChargeTime = 2.f;
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.495f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.495f ) && true == EventCheck( 0.495f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ZpushX_Charge", this, NULL,
																					false, GetPowerRate() );
		if ( INVALID_EFFECTSET_HANDLE != hEffectSet )
			m_vecEffectSetToDeleteOnStateEnd.push_back( hEffectSet );
	}
}

/** @function	: ComboDashZpushX_ChargeEventProcess
	@brief		: 상태변경 여부를 판단, 수행( DashComboZpushX_ATTACK )
*/
void CX2GUAra::ComboDashZpushX_ChargeEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	else if ( false == m_InputData.pureX || true == m_pXSkinAnim->IsAnimationEnd() )
	{
		StateChange( ASI_DASH_COMBO_ZpushX_ATTACK );
	}
	
	CommonEventProcess();
}

/** @function 	: ComboDashZpushX_ChargeStateEnd
	@brief 		: 마나 브레이크 켜기
*/
void CX2GUAra::ComboDashZpushX_ChargeStateEnd()
{
	CommonStateEnd();

	SetNoDetonation( false );
}
#pragma endregion 95

#pragma region ComboDashZpushX_Attack
/** @function 	: ComboDashZpushX_AttackFrameMove
	@brief 		: 
*/
void CX2GUAra::ComboDashZpushX_AttackFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.damage.fPhysic = m_DamageData.damage.fPhysic * ( 1.f + min( m_fZpushXChargeTime, 2.f ) );

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ZpushX_Attack", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();

	if ( 2.f <= m_fZpushXChargeTime )
	{
		m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
		m_FrameDataNow.stateParam.bSuperArmor = true;
	}
}

/** @function	: ComboDashZpushX_AttackEventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboDashZpushX_AttackEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

/** @function 	: ComboDashZpushX_AttackStateEnd
	@brief 		: 
*/
void CX2GUAra::ComboDashZpushX_AttackStateEnd()
{
	CommonStateEnd();

	m_FrameDataNow.stateParam.bSuperArmorNotRed = true;
	m_FrameDataNow.stateParam.bSuperArmor = false;
}
#pragma endregion 96

#pragma region ComboDashX
/** @function	: ComboDashXEventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ComboDashXEventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	else if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	else if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107	
	{
		StateChangeSpecialAttack( m_eSkillID );
	}
#else
	else if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
	{
		StateChangeSpecialAttack( m_pSkillTemplet );
	}
#endif
	
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}
#pragma endregion 97

#pragma region JumpAttackZ
/** @function 	: JumpAttackZFrameMoveFuture
	@brief 		: 방향 키로 스피드 조절
*/
void CX2GUAra::JumpAttackZFrameMoveFuture()
{
	// 현재 진행 방향 쪽으로 키보드를 누르면 앞으로 좀 더 나아감
	if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
		( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( true == m_InputData.pureDown && false == m_InputData.pureUp )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonFrameMoveFuture();
}

/** @function	: JumpAttackZEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::JumpAttackZEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		bool &bPureDown = m_FrameDataFuture.stateParam.bEventFlagList[0];

		if ( true == bPureDown )
		{
			m_fCanNotJumpTime = 0.2f;
		}

		StateChange( USI_JUMP_LANDING );
	}
#ifdef ARA_CHANGE_CLASS_FIRST
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true && ( CX2Unit::UC_ARA_LITTLE_HSIEN == GetUnitClass() || CX2Unit::UC_ARA_SAKRA_DEVANAM == GetUnitClass() ), 
		ASI_JUMP_ATTACK_ALH_ZZ )
#endif //ARA_CHANGE_CLASS_FIRST 

	CommonEventProcess();
}
#pragma endregion 98

#pragma region JumpAttackX
/** @function 	: JumpAttackXFrameMoveFuture
	@brief 		: 방향 키로 스피드 조절
*/
void CX2GUAra::JumpAttackXFrameMoveFuture()
{
	// 현재 진행 방향 쪽으로 키보드를 누르면 앞으로 좀 더 나아감
	if( ( true == m_InputData.pureRight && true == m_FrameDataFuture.syncData.bIsRight ) ||
		( true == m_InputData.pureLeft && false == m_FrameDataFuture.syncData.bIsRight ) )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	if( true == m_InputData.pureDown && false == m_InputData.pureUp )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	CommonFrameMoveFuture();
}

/** @function 	: JumpAttackXFrameMove
	@brief 		: Damage Data 변경
*/
void CX2GUAra::JumpAttackXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.396f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.396f ) && true == EventCheck( 0.396f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.reActType		= CX2DamageManager::RT_DOWN;
		m_DamageData.backSpeed.y	= -2000.f;
	}

	CommonFrameMove();
}

/** @function	: JumpAttackXEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::JumpAttackXEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		bool &bPureDown = m_FrameDataFuture.stateParam.bEventFlagList[0];
		if ( true == bPureDown )
		{
			m_fCanNotJumpTime = 0.2f;
		}

		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}
#pragma endregion 99

#pragma region DashJumpComboZ
/** @function	: DashJumpComboZEventProcess
	@brief		: 상태변경 여부를 판단, 수행( DashJumpComboZZ )
*/
void CX2GUAra::DashJumpComboZEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ASI_DASH_JUMP_COMBO_ZZ )
#ifdef ARA_CHANGE_CLASS_FIRST
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		m_InputData.oneX == true && CX2Unit::UC_ARA_SAKRA_DEVANAM == GetUnitClass(), ASI_DASH_JUMP_COMBO_ASD_ZX )
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
	m_InputData.oneX == true && ( CX2Unit::UC_ARA_LITTLE_DEVIL == GetUnitClass() || CX2Unit::UC_ARA_YAMA_RAJA == GetUnitClass() ), 
	ASI_DASH_JUMP_COMBO_ALD_ZX )
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	CommonEventProcess();
}
#pragma endregion 100

#pragma region DashJumpComboZZ
/** @function	: DashJumpComboZZEventProecss
	@brief		: 상태변경 여부를 판단, 수행( DashJumpComboZZZ )
*/
void CX2GUAra::DashJumpComboZZEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true, ASI_DASH_JUMP_COMBO_ZZZ )

	CommonEventProcess();
}
#pragma endregion 101

#pragma region DashJumpComboZZZ
/** @function 	: DashJumpComboZZZFrameMove
	@brief 		: Damage Data 변경
*/
void CX2GUAra::DashJumpComboZZZFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.462f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.462f ) && true == EventCheck( 0.462f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.reActType		= CX2DamageManager::RT_FLY;
		m_DamageData.backSpeed.y	= 1250.f;
	}

	CommonFrameMove();
}

/** @function	: DashJumpComboZZZEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::DashJumpComboZZZEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}
#pragma endregion 102

#pragma region DashJumpComboX
/** @function	: DashJumpComboXEventProecss
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::DashJumpComboXEventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneZ == true && CX2Unit::UC_ARA_YAMA_RAJA == GetUnitClass(), ASI_DASH_JUMP_COMBO_AYR_XZ )
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	CommonEventProcess();
}
#pragma endregion 103

#pragma region StateChangeAirialDashIfPossible
void CX2GUAra::StateChangeAirialDashIfPossible()
{
	if( true == GetEnableDash() && _CONST_ARA_::MAGIC_MAX_AIRIAL_DASH_COUNT > m_usArialCount )
		StateChange( ASI_AIRIAL_DASH );
	else
		StateChange( USI_JUMP_DOWN );
}
#pragma endregion 

#pragma endregion functions

#pragma region special active skill
#pragma region WIND_WEDGE


void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ARA_WIND_WEDGE_START", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_WIND_WEDGE_START_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_WIND_WEDGE_START_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_WIND_WEDGE_START_EventProcess );
}

/** @function 	: ASI_SA_ARA_WIND_WEDGE_START_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_START_Init()
{
	XMeshReadyInBackground( L"ARA_SA_WindWedge_spark.Y" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"ARA_SA_WindWedge_spark.dds" );
	TextureReadyInBackground( L"ARA_SA_WindWedge_windLine.dds" );
}

/** @function 	: ASI_SA_ARA_WIND_WEDGE_START_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_START_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) && CanUseForcePower() )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) && CanUseForcePower() )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
		/// 메모 : 흐름을 타는 창술 (맹호 2식 : 바람쐐기) - 기력 소모량 1 감소
		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO2 ) )
			UseForcePower( 1 );
		else
#endif // ADD_ARA_MEMO
			UseForcePower( 2 );
	}
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.495f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.495f ) && true == EventCheck( 0.495f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Wind_Wedge", this );
		m_usBeforeForceGainCount = m_usForceGainCount;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.495f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.495f ) && true == EventCheck( 0.495f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Wind_Wedge_1st", this, NULL, false, GetPowerRate() );
	}
		
	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_WIND_WEDGE_START_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_SA_ARA_WIND_WEDGE_END );
		}
	}
	
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ARA_WIND_WEDGE_END", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_WIND_WEDGE_END_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_WIND_WEDGE_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_WIND_WEDGE_END_EventProcess );
}

/** @function 	: ASI_SA_ARA_WIND_WEDGE_END_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_END_Init()
{
	XMeshReadyInBackground( L"ARA_SA_WindWedge_spark.Y" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"ARA_SA_WindWedge_spark.dds" );
	TextureReadyInBackground( L"ARA_SA_WindWedge_windLine.dds" );
}

/** @function 	: ASI_SA_ARA_WIND_WEDGE_END_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Wind_Wedge_Final", this );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.015f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.015f ) && true == EventCheck( 0.015f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Wind_Wedge_2nd", this, NULL, false, GetPowerRate() );
	}
	
	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_WIND_WEDGE_END_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_WIND_WEDGE_END_EventProcess()
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
#pragma endregion 바람 쐐기

#pragma region PULLING_PIERCE

void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ARA_PULLING_PIERCE_START", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_PULLING_PIERCE_START_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_PULLING_PIERCE_START_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_PULLING_PIERCE_START_EventProcess );
}

/** @function 	: ASI_SA_ARA_PULLING_PIERCE_START_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_START_Init()
{
	XMeshReadyInBackground( L"ARA_SA_WindWedge_spark.Y" );
	XSkinMeshReadyInBackground( L"ARA_SA_PullingPierce_circle.X" );
	XSkinMeshReadyInBackground( L"elsword_SA_SwordBlastiong_slash.X" );
	XSkinMeshReadyInBackground( L"aisha_SA_SCREWDRIVER_circle.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"ARA_SA_PullingPierce_circle.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_particle.dds" );
}

/** @function 	: ASI_SA_ARA_PULLING_PIERCE_START_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_START_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.396f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.396f ) && true == EventCheck( 0.396f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Pulling_Pierce", this );
		UpForcePower( 1 );
		m_usBeforeForceGainCount = m_usForceGainCount;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.462f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.462f ) && true == EventCheck( 0.462f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
		bool bIsMemoAttack = false;		/// 메모 적용 여부

		/// 메모 : 초천공술 (용아 2식 : 빨래널기) - 크리티컬 확률 25% 증가, 방패 몬스터 가드 무시
		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO4 ) )
			bIsMemoAttack = true;

			m_hPullinPierce = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Pulling_Pierce_1st", 
								this, NULL, bIsMemoAttack, GetPowerRate() );
#else  // ADD_ARA_MEMO
			m_hPullinPierce = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Pulling_Pierce_1st", 
								this, NULL, false, GetPowerRate() );
#endif // ADD_ARA_MEMO
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.759f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.759f ) && true == EventCheck( 0.759f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( INVALID_EFFECTSET_HANDLE != m_hPullinPierce )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hPullinPierce );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_PULLING_PIERCE_START_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_SA_ARA_PULLING_PIERCE_END );
		}
	}
	
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}
}

void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_ ;
	m_LuaManager.MakeTableReference( "ASI_SA_ARA_PULLING_PIERCE_END", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_PULLING_PIERCE_END_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_PULLING_PIERCE_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_PULLING_PIERCE_END_EventProcess );
}

/** @function 	: ASI_SA_ARA_PULLING_PIERCE_END_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_END_Init()
{
	XMeshReadyInBackground( L"ARA_SA_WindWedge_spark.Y" );
	XSkinMeshReadyInBackground( L"ARA_SA_PullingPierce_circle.X" );
	XSkinMeshReadyInBackground( L"aisha_SA_SCREWDRIVER_circle.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"ARA_SA_PullingPierce_circle.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_particle.dds" );
}

/** @function 	: ASI_SA_ARA_PULLING_PIERCE_END_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Pulling_Pierce_Final", this );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.066f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.066f ) && true == EventCheck( 0.066f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Pulling_Pierce_2nd", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_PULLING_PIERCE_END_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_PULLING_PIERCE_END_EventProcess()
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
#pragma endregion 빨래 널기

#pragma region FALLING_DRAGON
/** @function 	: ASI_SA_ARA_FALLING_DRAGON_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_FALLING_DRAGON_Init()
{
	XMeshReadyInBackground( L"ARA_SA_FallingDragon_sparkHigh.Y" );
	XMeshReadyInBackground( L"ARA_SA_FallingDragon_spark.Y" );
	TextureReadyInBackground( L"ARA_SA_FallingDragon_circleSpark.dds" );
	TextureReadyInBackground( L"ARA_SA_FallingDragon_donw.dds" );
	TextureReadyInBackground( L"ARA_SA_FallingDragon_spark.dds" );
	TextureReadyInBackground( L"ARA_SA_FallingDragon_sparkHigh.dds" );
}

/** @function 	: ASI_SA_ARA_FALLING_DRAGON_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_FALLING_DRAGON_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if ( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) && CanUseForcePower() )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) && CanUseForcePower() )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_fPullingDragonPowerRate = GetPowerRate() * ( 1 + ( GetForcePower() - 5 ) * 0.2f );
		
		UseForcePower( GetForcePower() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 1.320f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 1.320f ) && true == EventCheck( 1.320f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Falling_Dragon", this, NULL, IsHyperState(),
			m_fPullingDragonPowerRate );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_FALLING_DRAGON_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_FALLING_DRAGON_EventProcess()
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
#pragma endregion 강룡추_구름 떨구기

#pragma region SHADOW_RISING

void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_JUMP_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_RISING_JUMP", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_JUMP_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_JUMP_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_JUMP_EventProcess );
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_JUMP_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_JUMP_Init()
{
	XMeshReadyInBackground( L"Split_Light01.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Badak.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Badak_BALL.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack_Aura.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack02.X" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Cloud.dds" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_DarkFlame.DDS" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_blackSmoke.dds" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_blackAura.DDS" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_AURA.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Hall.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact01.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact02.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact04.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact05.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Smoke.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"smoke02.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_JUMP_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_JUMP_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Shadow_Rising_Jump", this );
		UpForcePower( 1 );
		m_usBeforeForceGainCount = m_usForceGainCount;
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_JUMP_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_JUMP_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true && IsOnSomethingFuture() == true )
	{
		StateChange( ASI_SA_ARA_SHADOW_RISING_LANDING );
	}

	CommonEventProcess();
}


void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_LANDING_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_RISING_LANDING", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_LANDING_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_LANDING_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_LANDING_EventProcess );
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_LANDING_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_LANDING_Init()
{
	XMeshReadyInBackground( L"Split_Light01.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Badak.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Badak_BALL.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack_Aura.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack02.X" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Cloud.dds" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_DarkFlame.DDS" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_blackSmoke.dds" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_blackAura.DDS" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_AURA.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Hall.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact01.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact02.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact04.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact05.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Smoke.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"smoke02.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_LANDING_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_LANDING_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Shadow_Rising_Landing", this );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_LANDING_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_LANDING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_SA_ARA_SHADOW_RISING_END );
		}
	}
	
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime() 
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}


void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ARA_SHADOW_RISING_END", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_END_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ARA_SHADOW_RISING_END_EventProcess );
}


/** @function 	: ASI_SA_ARA_SHADOW_RISING_END_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_END_Init()
{
	XMeshReadyInBackground( L"Split_Light01.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Badak.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Badak_BALL.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack_Aura.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_RISING_Attack02.X" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Cloud.dds" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_DarkFlame.DDS" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_blackSmoke.dds" );
	TextureReadyInBackground( L"Pa_Ran_SpaD_blackAura.DDS" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_AURA.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Hall.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact01.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact02.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact04.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Impact05.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Smoke.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"smoke02.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_END_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Shadow_Rising_End", this );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_SHADOW_RISING_END_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_RISING_END_EventProcess()
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
#pragma endregion 벽력뢰_천둥 꽃망울

#pragma region TEMPEST
/** @function 	: ASI_SA_ARA_TEMPEST_DANCE_START_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_TEMPEST_DANCE_START_Init()
{
	XMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.Y" );
	XMeshReadyInBackground( L"Lire_Leaf01.Y" );
	XMeshReadyInBackground( L"Wind_Liner02.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_IMPACT.X" );
	TextureReadyInBackground( L"stone.dds" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_particle.dds" );
}

/** @function 	: ASI_SA_ARA_TEMPEST_DANCE_START_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_TEMPEST_DANCE_START_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Tempest_Dance_Start", this );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		UseForcePower( 3 );
#else //UPGRADE_SKILL_SYSTEM_2013
		UpForcePower( 1 );
#endif //UPGRADE_SKILL_SYSTEM_2013
		m_usBeforeForceGainCount = m_usForceGainCount;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.231f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.231f ) && true == EventCheck( 0.231f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_1st", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.429f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.429f ) && true == EventCheck( 0.429f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_2nd", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.561f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.561f ) && true == EventCheck( 0.561f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_2nd", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.957f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.957f ) && true == EventCheck( 0.957f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_2nd", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 1.056f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 1.056f ) && true == EventCheck( 1.056f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_2nd", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 1.155f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 1.155f ) && true == EventCheck( 1.155f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_2nd", this, NULL, false, GetPowerRate() );
	}
	
	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_TEMPEST_DANCE_START_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_TEMPEST_DANCE_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			StateChangeSpecialAttack( m_pSkillTemplet );
			UpForcePower( m_usBeforeForceGainCount );
		}
#endif
		else
		{
			StateChange( ASI_SA_ARA_TEMPEST_DANCE_END );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack()  )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

/** @function 	: ASI_SA_ARA_TEMPEST_DANCE_END_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_TEMPEST_DANCE_END_Init()
{
	XMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.Y" );
	XMeshReadyInBackground( L"Lire_Leaf01.Y" );
	XMeshReadyInBackground( L"Wind_Liner02.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_IMPACT.X" );
	TextureReadyInBackground( L"stone.dds" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"Spin_Liner02_Gray.dds" );
	TextureReadyInBackground( L"CTT_A_REMOTE_MORTAR_shellingPointLight.dds" );
	TextureReadyInBackground( L"SpinWind01.dds" );
}

/** @function 	: ASI_SA_ARA_TEMPEST_DANCE_END_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_TEMPEST_DANCE_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Tempest_Dance_End", this );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.099f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.099f ) && true == EventCheck( 0.099f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_Final_1st", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.297f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.297f ) && true == EventCheck( 0.297f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tempest_Dance_Final_2nd", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_TEMPEST_DANCE_END_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_TEMPEST_DANCE_END_EventProcess()
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
#pragma endregion 선풍_휘모리

#pragma region SHADOW_KNOT
/** @function 	: ASI_SA_ARA_SHADOW_KNOT_START_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_START_Init()
{
	XMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.Y" );
	XMeshReadyInBackground( L"Split_Light01.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_HIGH.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_KNOT.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE03.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SD_pa_source.X" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"state_potion_flare.dds" );
	TextureReadyInBackground( L"stone_AirShip.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_KNOT_KNOT_FINAL.dds" );
	TextureReadyInBackground( L"SD_pa_source.tga" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_AURA.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_TATOO.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_HIGHLIGHT.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_DIFFUSE.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_Attack.dds" );
	TextureReadyInBackground( L"RTW_Flame_Buster.dds" );
	TextureReadyInBackground( L"raven_beed_hit_spark.DDS" );
	TextureReadyInBackground( L"Ara_Black_LEAF.dds" );
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_START_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_START_StateStart()
{
	CommonStateStart();

	m_usBeforeForceGainCount = m_usForceGainCount;

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
	/// 메모 : 비전 그림자 창술 (그림자매듭) - MP 150 + 기력 3 사용
	if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO5 ) )
		UseForcePower( 3 );
#endif // ADD_ARA_MEMO
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_START_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_START_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ARA_Shadow_Knot_Start", this );

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
		/// 메모 : 비전 그림자 창술 (그림자매듭) - 첫 시전에 대한 기력 획득 해제
		if( false == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO5 ) )
			UpForcePower( 1 );
#endif // ADD_ARA_MEMO
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.667f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.667f ) && true == EventCheck( 0.667f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		/* Frame 크래쉬 다음에 테스트 필요
		CKTDXDeviceXSkinMesh* pSkinMesh = NULL;
		CKTDXDeviceXET* pXET = NULL;

		for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
		{
			CX2EqipPtr pCX2Eqip = m_ViewEqipList[i];
			if ( pCX2Eqip == NULL )
				continue;

			if( CX2Unit::EP_WEAPON_HAND != pCX2Eqip->GetEqipPos() )
				continue;

			if( NULL != pCX2Eqip->GetItem() &&
				NULL != pCX2Eqip->GetItem()->GetItemTemplet() )
			{
				if( true == pCX2Eqip->GetItem()->GetItemTemplet()->GetFashion() )
				{
					pSkinMesh = pCX2Eqip->GetNormalModel(0);
					pXET = pCX2Eqip->GetNormalTexChangeXET();
					break;
				}
				else
				{
					pSkinMesh = pCX2Eqip->GetNormalModel(0);
					pXET = pCX2Eqip->GetNormalTexChangeXET();
				}
			}
		}
		*/

		D3DXVECTOR3 vPos = GetBonePos( L"Bip01_Footsteps" );

		//vPos.y += 150.f;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_hShadowKnotWeaponMesh = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_pShadowKnotWeaponMesh = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            (CX2GameUnit*) this,
			L"DAMAGE_EFFECT_SHADOW_KNOT_WEAPON", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(),
			m_FrameDataNow.unitCondition.landPosition.y );


		/*if( NULL != m_pShadowKnotWeaponMesh && NULL != m_pShadowKnotWeaponMesh->GetMainEffect()
			&& NULL != m_pShadowKnotWeaponMesh->GetMainEffect()->GetXSkinAnim() )
		{
			CKTDGXSkinAnim* pSkinAnim = m_pShadowKnotWeaponMesh->GetMainEffect()->GetXSkinAnim();

			if( NULL != pSkinMesh )
			{
				pSkinAnim->RemoveModelXSkinMesh( pSkinAnim->GetAnimXSkinMesh() );
				pSkinAnim->AddModelXSkinMesh( pSkinMesh, NULL, NULL, pXET, false );
			}
		}*/
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_START_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		m_fShadowKnotTime = m_fDamageDataChangeTime;
		StateChange( ASI_SA_ARA_SHADOW_KNOT_WAIT );
	}

	CommonEventProcess();
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_WAIT_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_WAIT_Init()
{
	XMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.Y" );
	XMeshReadyInBackground( L"Split_Light01.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_HIGH.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_KNOT.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE03.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SD_pa_source.X" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"state_potion_flare.dds" );
	TextureReadyInBackground( L"stone_AirShip.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_KNOT_KNOT_FINAL.dds" );
	TextureReadyInBackground( L"SD_pa_source.tga" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_AURA.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_TATOO.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_HIGHLIGHT.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_DIFFUSE.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_Attack.dds" );
	TextureReadyInBackground( L"RTW_Flame_Buster.dds" );
	TextureReadyInBackground( L"raven_beed_hit_spark.DDS" );
	TextureReadyInBackground( L"Ara_Black_LEAF.dds" );
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_WAIT_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_WAIT_FrameMove()
{
	if ( 0.f < m_fShadowKnotTime )
		m_fShadowKnotTime -= m_fElapsedTime;

	if ( 0.f > m_fShadowKnotTime )
		m_fShadowKnotTime = 0.f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ARA_Shadow_Knot_Wait", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_WAIT_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_WAIT_EventProcess()
{
	if ( 0.f >= m_fShadowKnotTime )
		StateChange( ASI_SA_ARA_SHADOW_KNOT_CATCH );

	if ( 0.5f >= m_fShadowKnotTime && 0.033f < m_fShadowKnotTime )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pShadowKnotWeaponMesh 
            = g_pX2Game->GetDamageEffect()->GetInstance( m_hShadowKnotWeaponMesh ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pShadowKnotWeaponMesh = m_pShadowKnotWeaponMesh )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			const CKTDGXMeshPlayer::CXMeshInstance* pMeshInstanceShadowKnot 
				= pShadowKnotWeaponMesh->GetMainEffect();

			if ( NULL != pMeshInstanceShadowKnot )
			{
				float fDistance 
					= GetDistance( GetPos(), pMeshInstanceShadowKnot->GetPos() );

				if ( 250.f >= fDistance )
					StateChange( ASI_SA_ARA_SHADOW_KNOT_CATCH );
			}
		}
	}

	CommonEventProcess();
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_WAIT_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_WAIT_StateEnd()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pShadowKnotWeaponMesh 
        = g_pX2Game->GetDamageEffect()->GetInstance( m_hShadowKnotWeaponMesh ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pShadowKnotWeaponMesh = m_pShadowKnotWeaponMesh )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstanceShadowKnotWeapon 
			= pShadowKnotWeaponMesh->GetMainEffect();

		if ( NULL != pMeshInstanceShadowKnotWeapon )
			pMeshInstanceShadowKnotWeapon->SetNowLifeTime( pMeshInstanceShadowKnotWeapon->GetMaxLifeTime() );
	}
	
	CommonStateEnd();
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_CATCH_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_CATCH_Init()
{
	XMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.Y" );
	XMeshReadyInBackground( L"Split_Light01.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_HIGH.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_KNOT.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE03.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SD_pa_source.X" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"state_potion_flare.dds" );
	TextureReadyInBackground( L"stone_AirShip.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_KNOT_KNOT_FINAL.dds" );
	TextureReadyInBackground( L"SD_pa_source.tga" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_AURA.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_TATOO.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_HIGHLIGHT.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_DIFFUSE.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_Attack.dds" );
	TextureReadyInBackground( L"RTW_Flame_Buster.dds" );
	TextureReadyInBackground( L"raven_beed_hit_spark.DDS" );
	TextureReadyInBackground( L"Ara_Black_LEAF.dds" );
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_CATCH_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_CATCH_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_SA_ARA_SHADOW_KNOT_END );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;

		TRACE ( L"m_usBeforeForceGainCount = %d\n", m_usBeforeForceGainCount );
	}

	CommonEventProcess();
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_END_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_END_Init()
{
	XMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_Ground.Y" );
	XMeshReadyInBackground( L"Split_Light01.Y" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_HIGH.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_TEMPEST_DANCE_BLADE_KNOT.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE_TATOO02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE01.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE02.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE03.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SA_ARA_SHADOW_KNOT_BLADE04.X" );
	XSkinMeshReadyInBackground( L"SD_pa_source.X" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"state_potion_flare.dds" );
	TextureReadyInBackground( L"stone_AirShip.dds" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_KNOT_KNOT_FINAL.dds" );
	TextureReadyInBackground( L"SD_pa_source.tga" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_AURA.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_TATOO.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_HIGHLIGHT.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_DIFFUSE.dds" );
	TextureReadyInBackground( L"SA_ARA_SHDOW_KNOT_BLADE_Attack.dds" );
	TextureReadyInBackground( L"RTW_Flame_Buster.dds" );
	TextureReadyInBackground( L"raven_beed_hit_spark.DDS" );
	TextureReadyInBackground( L"Ara_Black_LEAF.dds" );
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_END_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ARA_Shadow_Knot_End", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.726f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.726f ) && true == EventCheck( 0.726f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Shadow_Knot_Final", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ARA_SHADOW_KNOT_END_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ARA_SHADOW_KNOT_END_EventProcess()
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
#pragma endregion 그림자 매듭

#ifdef ARA_CHANGE_CLASS_FIRST
#pragma region MOONLIGHT_SLASH

void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_READY_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALH_MOONLIGHT_SLASH_READY", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_READY_Init );
	stateData_.StateStart			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_READY_StateStart );
	stateData_.StateStartFuture	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_READY_StateStartFuture );
	stateData_.OnFrameMove			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_READY_FrameMove );
	stateData_.OnEventProcess		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_READY_EventProcess );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_Init
	@brief 		: 리소스 백그라운 로딩
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_READY_Init()
{
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XSkinMeshReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
	TextureReadyInBackground( L"Ara_Orange_Light.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE01.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE02.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.tga" );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_READY_StateStart()
{
	CommonStateStart();

	if ( NULL != m_sMoonRightSlashDataPtr )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		m_pXSkinAnim->SetPlaySpeed( m_fCurrentSpeed );
#endif //UPGRADE_SKILL_SYSTEM_2013
		
		m_sMoonRightSlashDataPtr->SetChargingTime( 0.f );
		m_sMoonRightSlashDataPtr->SetPowerRate( 0.f );
#ifdef FIX_ARA_SKILL_BUG
		UpForcePower( m_usForceGainCount );
#endif //FIX_ARA_SKILL_BUG
	}
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_Ready_StateStartFuture
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_READY_StateStartFuture()
{
	CommonStateStartFuture();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	m_pXSkinAnimFuture->SetPlaySpeed( m_fCurrentSpeed );
#endif //UPGRADE_SKILL_SYSTEM_2013

	if ( NULL != m_sMoonRightSlashDataPtr )
	{
		switch ( m_sMoonRightSlashDataPtr->GetSlotID() )
		{
		case USI_SPECIAL_ATTACK_1:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureA );
			break;

		case USI_SPECIAL_ATTACK_2:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureS );
			break;

		case USI_SPECIAL_ATTACK_3:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureD );
			break;

		case USI_SPECIAL_ATTACK_4:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureC );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_1:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureQ );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_2:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureW );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_3:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureE );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_4:
			m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureR );
			break;

		default:
			//ASSERT( !L"ASI_SA_ALH_MOONLIGHT_SLASh__READY_StateStartFuture" );
			//m_sMoonRightSlashDataPtr->SetChargeKey( &m_InputData.pureA );
			m_sMoonRightSlashDataPtr->SetChargeKey( NULL );
			break;
		}
	}
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_READY_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != m_sMoonRightSlashDataPtr )
			m_sMoonRightSlashDataPtr->SetHandleEffectSet( g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Charge", this ) );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_2Second", this );
	}

	if ( NULL != m_sMoonRightSlashDataPtr )
#ifdef BALANCE_PATCH_20131107
	{
		const float fElapsedAnimationTime = m_pXSkinAnim->GetNowAnimationTime() - m_pXSkinAnim->GetBeforeAnimationTime();
		m_sMoonRightSlashDataPtr->SetChargingTime( m_sMoonRightSlashDataPtr->GetChargingTime() + fElapsedAnimationTime );
	}
#else //BALANCE_PATCH_20131107
		m_sMoonRightSlashDataPtr->SetChargingTime( m_sMoonRightSlashDataPtr->GetChargingTime() + m_fElapsedTime );
#endif //BALANCE_PATCH_20131107

	CommonFrameMove();
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE );
	}
	else if ( NULL != m_sMoonRightSlashDataPtr && 0.66 <= m_sMoonRightSlashDataPtr->GetChargingTime()  )
	{
		if ( false == m_sMoonRightSlashDataPtr->GetChargeKey() )
		{	
			if ( 1.5f <= m_sMoonRightSlashDataPtr->GetChargingTime() )
				StateChange( ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK );
			else
				StateChange( ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK );
		}
	}

	CommonEventProcess();
}

void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_EventProcess );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_Charge_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_Init()
{
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XSkinMeshReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
	TextureReadyInBackground( L"Ara_Orange_Light.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE01.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE02.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.tga" );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_Charge_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_FrameMove()
{
	CommonFrameMove();

	if ( NULL != m_sMoonRightSlashDataPtr )
#ifdef BALANCE_PATCH_20131107
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			m_sMoonRightSlashDataPtr->SetChargingTime( m_sMoonRightSlashDataPtr->GetChargingTime() + m_fElapsedTime );
		}
		else
		{
			const float fElapsedAnimationTime = m_pXSkinAnim->GetNowAnimationTime() - m_pXSkinAnim->GetBeforeAnimationTime();
			m_sMoonRightSlashDataPtr->SetChargingTime( m_sMoonRightSlashDataPtr->GetChargingTime() + fElapsedAnimationTime );
		}
	}
#else //BALANCE_PATCH_20131107
		m_sMoonRightSlashDataPtr->SetChargingTime( m_sMoonRightSlashDataPtr->GetChargingTime() + m_fElapsedTime );
#endif //BALANCE_PATCH_20131107

}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_Charge_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_EventProcess()
{
	CommonEventProcess();

	if ( NULL != m_sMoonRightSlashDataPtr )
	{
		if ( 5.0f <= m_sMoonRightSlashDataPtr->GetChargingTime() || false == m_sMoonRightSlashDataPtr->GetChargeKey() )
		{
			StateChange( ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK );
		}
	}
}


void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateStart );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_FrameMove );
	stateData_.StateEnd			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateEnd );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_EventProcess );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_Init
	@brief 		: 리소스 백그라운 로딩
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_Init()
{
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XSkinMeshReadyInBackground( L"SA_ALH_MOONLIGHT_SLASH_BLADE01.X" );
	XSkinMeshReadyInBackground( L"SA_ALH_MOONLIGHT_SLASH_BLADE02.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
	TextureReadyInBackground( L"Ara_Orange_Light.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE01.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE02.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.tga" );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateStart()
{
	CommonStateStart();

	if( NULL != m_sMoonRightSlashDataPtr )
		m_sMoonRightSlashDataPtr->SetPowerRate( floor( m_sMoonRightSlashDataPtr->GetChargingTime() ) * 0.1f );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_New", this );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.12f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.12f ) == true && EventCheck( 0.12f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != m_sMoonRightSlashDataPtr )
		{
			float fPowerRate = GetPowerRate() + m_sMoonRightSlashDataPtr->GetPowerRate();

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
			bool bIsMemoAttack = false;		/// 메모 적용 여부

			/// 메모 : /// 진 달빛베기 (용아 4식 : 달빛베기) - 공격력 33% 증가
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO9 ) )
				bIsMemoAttack = true;

			/// 기존 코드에 각성에 대한 이펙트 출력이 설정되어 있지만 실제로 존재하고 있지는 않아서, 메모용으로 사용
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( m_bMoonSlash )
				m_sMoonRightSlashDataPtr->SetHandleEffectSet(
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Crescent_Attack_FullMoon", this, NULL, bIsMemoAttack, fPowerRate ) );
			else
				m_sMoonRightSlashDataPtr->SetHandleEffectSet(
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Crescent_Attack", this, NULL, bIsMemoAttack, fPowerRate ) );
#else //UPGRADE_SKILL_SYSTEM_2013
			m_sMoonRightSlashDataPtr->SetHandleEffectSet(
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Crescent_Attack", this, NULL, bIsMemoAttack, fPowerRate ) );
#endif //UPGRADE_SKILL_SYSTEM_2013

#else  // ADD_ARA_MEMO
			m_sMoonRightSlashDataPtr->SetHandleEffectSet(
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Crescent_Attack", this, NULL, IsHyperState(), fPowerRate ) );
#endif // ADD_ARA_MEMO
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 0.275f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 0.275f ) == true && EventCheck( 0.275f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != m_sMoonRightSlashDataPtr )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_sMoonRightSlashDataPtr->GetHandleReferenceEffectSet() );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_StateEnd()
{
	if ( NULL != m_sMoonRightSlashDataPtr )
	{
		m_sMoonRightSlashDataPtr->SetChargingTime( 0.f );
		m_sMoonRightSlashDataPtr->SetPowerRate( 0.f );
	}

	CommonStateEnd();
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_EventProcess()
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

void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateStart );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_FrameMove );
	stateData_.StateEnd			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateEnd );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_EventProcess );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_Init()
{
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XSkinMeshReadyInBackground( L"SA_ALH_MOONLIGHT_SLASH_BLADE01.X" );
	XSkinMeshReadyInBackground( L"SA_ALH_MOONLIGHT_SLASH_BLADE02.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
	TextureReadyInBackground( L"Ara_Orange_Light.dds" );
	TextureReadyInBackground( L"Explosion_Light02.dds" );
	TextureReadyInBackground( L"WhiteCircle02.dds" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE01.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE02.tga" );
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.tga" );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateStart()
{
	CommonStateStart();

	if( NULL != m_sMoonRightSlashDataPtr )
		m_sMoonRightSlashDataPtr->SetPowerRate( floor( m_sMoonRightSlashDataPtr->GetChargingTime() - 2.f ) * 0.1f );
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Full", this );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.033f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.033f ) == true && EventCheck( 0.033f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != m_sMoonRightSlashDataPtr )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			float fPowerRate = GetPowerRate() * ( 1.f + m_sMoonRightSlashDataPtr->GetPowerRate() );
#else //UPGRADE_SKILL_SYSTEM_2013
			float fPowerRate = GetPowerRate() + m_sMoonRightSlashDataPtr->GetPowerRate();
#endif //UPGRADE_SKILL_SYSTEM_2013


#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
			bool bIsMemoAttack = false;		/// 메모 적용 여부

			/// 메모 : /// 진 달빛베기 (용아 4식 : 달빛베기) - RT_FLY에서 RT_BIG_DAMAGE로 변경
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO9 ) )
				bIsMemoAttack = true;

			/// 기존 코드에 각성에 대한 이펙트 출력이 설정되어 있지만 실제로 존재하고 있지는 않아서, 메모용으로 사용
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( m_bMoonSlash )
				m_sMoonRightSlashDataPtr->SetHandleEffectSet(
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Fullmoon_Attack_FullMoon", this, NULL, bIsMemoAttack, fPowerRate ) );
			else
				m_sMoonRightSlashDataPtr->SetHandleEffectSet(
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Fullmoon_Attack", this, NULL, bIsMemoAttack, fPowerRate ) );
#else //UPGRADE_SKILL_SYSTEM_2013
			m_sMoonRightSlashDataPtr->SetHandleEffectSet(
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Fullmoon_Attack", this, NULL, bIsMemoAttack, fPowerRate ) );
#endif //UPGRADE_SKILL_SYSTEM_2013
#else  // ADD_ARA_MEMO
			m_sMoonRightSlashDataPtr->SetHandleEffectSet(
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_MoonLight_Slash_Fullmoon_Attack", this, NULL, IsHyperState(), fPowerRate ) );
#endif // ADD_ARA_MEMO
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 0.300f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 0.300f ) == true && EventCheck( 0.300f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != m_sMoonRightSlashDataPtr )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_sMoonRightSlashDataPtr->GetHandleReferenceEffectSet() );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_StateEnd()
{
	if ( NULL != m_sMoonRightSlashDataPtr )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_sMoonRightSlashDataPtr->GetHandleReferenceEffectSet() );

		m_sMoonRightSlashDataPtr->SetChargingTime( 0.f );
		m_sMoonRightSlashDataPtr->SetPowerRate( 0.f );
	}

	CommonStateEnd();
}

/** @function 	: ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_EventProcess()
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
#pragma endregion 달빛 베기

#pragma region SWALLOW_ASSAULT
/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Ready_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Ready_Init()
{
	XMeshReadyInBackground( L"ALH_SWALLOW_ASSAULT_BASE.Y" );
	XMeshReadyInBackground( L"Wind_Liner02.Y" );
	XMeshReadyInBackground( L"Ara_Orange_Leaf.Y" );
	XSkinMeshReadyInBackground( L"SA_ALH_MOONLIGHT_SLASH_BLADE02.X" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"A_ALH_ROCK_Strong.dds" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateStart()
{
	CommonStateStart();
	
	m_sLockOnSystemPtr->SetPveRange( 1200.f );
	m_sLockOnSystemPtr->SetPvpRange( 700.f );
	m_sLockOnSystemPtr->SetSafePos( GetPos() );
	m_sLockOnSystemPtr->ProcessLockOn( this );
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Ready_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Ready_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UseForcePower( 1 );
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Swallow_Assault_Ready", this );
	}

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.818f && m_pXSkinAnim->GetNowAnimationTime() < 1.020f )
	{
		SetInvisibility( true );

		m_fRenderParamColorA -= m_fElapsedTime * 10.f;
		if( m_fRenderParamColorA < 0.0f )
			m_fRenderParamColorA = 0.0f;
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Ready_StateEnd()
{
	CommonStateEnd();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Ready_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Ready_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if ( true == m_sLockOnSystemPtr->CheckLockOnUnit() )
		{
			m_sLockOnSystemPtr->ResetLockOnUnit();
			StateChange( ASI_SA_ALH_SWALLOW_ASSAULT_ATTACK );
		}
		else
		{
			SetInvisibility( false );
			StateChange( ASI_SA_ALH_SWALLOW_ASSAULT_FINISH );
		}
	}

	CommonEventProcess();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Attack_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Attack_Init()
{
	XMeshReadyInBackground( L"Ring_Plane.Y" );
	XMeshReadyInBackground( L"AssaultSlash02.Y" );
	XMeshReadyInBackground( L"Wind_Liner02.Y" );
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XMeshReadyInBackground( L"ALH_PEERLESSNESS_SHADOW01.Y" );
	XMeshReadyInBackground( L"ALH_PEERLESSNESS_SHADOW02.Y" );
	XMeshReadyInBackground( L"ALH_PEERLESSNESS_SHADOW03.Y" );
	XMeshReadyInBackground( L"ALH_PEERLESSNESS_SHADOW04.Y" );
	XMeshReadyInBackground( L"ALH_PEERLESSNESS_SHADOW05.Y" );
	XMeshReadyInBackground( L"ALH_PEERLESSNESS_SHADOW06.Y" );
	XSkinMeshReadyInBackground( L"SA_ALH_MOONLIGHT_SLASH_BLADE02.X" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_particle.dds" );
	TextureReadyInBackground( L"Sander_Aelo_Light.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
	TextureReadyInBackground( L"RingAlpha.dds" );
	TextureReadyInBackground( L"ARA_SA_WindWedge_windLine.dds" );
	TextureReadyInBackground( L"Ara_Orange_Light.dds" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"Condense_Pulse02.dds" );
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateStart()
{
	CommonStateStart();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Attack_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Attack_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_FrameDataFuture.stateParam.bCanPushUnit = false;
		m_FrameDataFuture.stateParam.bCanPassUnit = true;

		D3DXVECTOR3 vMyDir = m_FrameDataNowBefore.unitCondition.dirVector;
		D3DXVECTOR3 vMyPos = GetPos();
		D3DXVECTOR3 vDestPos = m_sLockOnSystemPtr->GetDestPos();
		D3DXVECTOR3 vTargetDir =  vDestPos - vMyPos;
		float		fDot = 0.f;
						
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        vTargetDir.y = 0.f;
        fDot = D3DXVec3Dot( &vMyDir, &vTargetDir );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
		D3DXVec3Normalize( &vMyDir, &vMyDir );
		D3DXVec3Normalize( &vTargetDir, &vTargetDir );
		fDot = D3DXVec3Dot( &vMyDir, &vTargetDir );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

		if ( fDot < 0.0f )
		{
			m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
			m_FrameDataFuture.unitCondition.dirDegree.y	+= 180.0f;
			m_FrameDataFuture.unitCondition.bAttackerFront = true;
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.198f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.198f ) && true == EventCheck( 0.198f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDestPos = m_sLockOnSystemPtr->GetDestPos();
		vDestPos.y += 20.f;

		m_FrameDataFuture.syncData.position = vDestPos;
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Swallow_Assault_Attack", this );
		
		int iRand = GetRandomInt() % 6;
		WCHAR wszText[64] = L"";
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"Effectset_Swallow_Assault_Shadow0%d", iRand );
		g_pX2Game->GetEffectSet()->PlayEffectSet( wszText, this, NULL, IsHyperState(), -1.f, -1.f, D3DXVECTOR3( 1.f, 1.f, 1.f ),
			true, vDestPos );
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Attack_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Attack_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			if( NULL != g_pX2Game->GetWorld() && NULL != g_pX2Game->GetWorld()->GetLineMap() )
			{
				D3DXVECTOR3 outPos;
				int			lineIndex = 0;

				if( NULL != m_sLockOnSystemPtr )
				{
					if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( GetPos(), LINE_RADIUS, &outPos, &lineIndex, false ) == true )
					{
						const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( lineIndex );

						if( NULL != pLineData )
						{	// 라인맵이 있지만 LT_NORMAL이 아닐 경우나 활성화되어 있지 않을 경우
							if( pLineData->lineType != CKTDGLineMap::LT_NORMAL || pLineData->bEnable == false )
								MoveAraSafePosition();
						}
						else
						{	// 라인맵이 없을 경우
							MoveAraSafePosition();
						}
					}
					else	/// 현재 위치 아래에 라인맵이 없을 때
					{
						MoveAraSafePosition();
					}
				}
			}

			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else if ( 0 < GetForcePower() )
		{
			m_sLockOnSystemPtr->ProcessLockOn( this );

			if ( true == m_sLockOnSystemPtr->CheckLockOnUnit() )
			{
				UseForcePower( 1 );
				StateChange( ASI_SA_ALH_SWALLOW_ASSAULT_ATTACK );
			}
			else
			{
				SetInvisibility( false );
				StateChange( ASI_SA_ALH_SWALLOW_ASSAULT_FINISH );
			}
		}
		else
		{
			StateChange( ASI_SA_ALH_SWALLOW_ASSAULT_FINISH );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Attack_StateEnd()
{
	SetInvisibility( false );

	CommonStateEnd();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Finish_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Finish_Init()
{
	XMeshReadyInBackground( L"Ring_Plane.Y" );
	XMeshReadyInBackground( L"AssaultSlash02.Y" );
	XMeshReadyInBackground( L"Wind_Liner02.Y" );
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XSkinMeshReadyInBackground( L"SA_ALH_MOONLIGHT_SLASH_BLADE02.X" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_particle.dds" );
	TextureReadyInBackground( L"Sander_Aelo_Light.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.dds" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
	TextureReadyInBackground( L"RingAlpha.dds" );
	TextureReadyInBackground( L"ARA_SA_WindWedge_windLine.dds" );
	TextureReadyInBackground( L"Ara_Orange_Light.dds" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"Condense_Pulse02.dds" );
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Finish_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Finish_StateStart()
{
	CommonStateStart();

	if( NULL != g_pX2Game->GetWorld() && NULL != g_pX2Game->GetWorld()->GetLineMap() )
	{
		D3DXVECTOR3 outPos;
		int			lineIndex = 0;

		if( NULL != m_sLockOnSystemPtr )
		{
			if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( GetPos(), LINE_RADIUS, &outPos, &lineIndex, false ) == true )
			{
				const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( lineIndex );

				if( NULL != pLineData )
				{	// 라인맵이 있지만 LT_NORMAL이 아닐 경우나 활성화되어 있지 않을 경우
					if( pLineData->lineType != CKTDGLineMap::LT_NORMAL || pLineData->bEnable == false )
						MoveAraSafePosition();
				}
				else
				{	// 라인맵이 없을 경우
					MoveAraSafePosition();
				}
			}
			else	/// 현재 위치 아래에 라인맵이 없을 때
			{
				MoveAraSafePosition();
			}
		}
	}
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Finish_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Finish_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		SetInvisibility( false );
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Swallow_Assault_Finish", this );
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Swallow_Assault_Attack", this );
	}	

	CommonFrameMove();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Finish_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Finish_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/** @function 	: ASI_SA_ALH_SWALLOW_ASSAULT_Escape_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ALH_SWALLOW_ASSAULT_Escape_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	else if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_WAIT );
	}
	
	CommonEventProcess();
}

#pragma endregion 비연

#endif //ARA_CHANGE_CLASS_FIRST
#pragma endregion functions

#pragma region active skill
#pragma region TIGER_CLAW

void CX2GUAra::ASI_A_ARA_TIGER_CLAW_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ARA_TIGER_CLAW_START", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_START_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_START_StateStart );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_START_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_START_EventProcess );
	stateData_.StateEnd			= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_START_StateEnd );
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_START_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_START_Init()
{
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XSkinMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Blade2.X" );
	XSkinMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Blade3.X" );
	TextureReadyInBackground( L"SI_A_Ston_Cutter_Blade.dds" );
	TextureReadyInBackground( L"SI_A_Ston_Cutter_Ground_Wave.dds" );
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_START_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_START_StateStart()
{
	CommonStateStart();

	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;

	m_usBeforeForceGainCount = m_usForceGainCount;
#ifdef ADD_MEMO_1ST_CLASS //김창한
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO11 ) == true )
		++m_usBeforeForceGainCount;
#endif //ADD_MEMO_1ST_CLASS
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_START_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_START_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.396f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.396f ) && true == EventCheck( 0.396f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Tiger_Claw", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.561f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.561f ) && true == EventCheck( 0.561f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tiger_Claw_1st", this, NULL, false, GetPowerRate() );
	}
	
	CommonFrameMove();
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_START_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_A_ARA_TIGER_CLAW_END );
		}
	}
	
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_START_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_START_StateEnd()
{
	CommonStateEnd();

	m_FrameDataNow.stateParam.bSuperArmorNotRed = true;
}


void CX2GUAra::ASI_A_ARA_TIGER_CLAW_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ARA_TIGER_CLAW_END", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_END_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ARA_TIGER_CLAW_END_EventProcess );
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_END_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_END_Init()
{
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	XSkinMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Blade3.X" );
	TextureReadyInBackground( L"SI_A_Ston_Cutter_Blade.dds" );
	TextureReadyInBackground( L"SI_A_Ston_Cutter_Ground_Wave.dds" );
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_END_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Tiger_Claw_Final", this, NULL, false, GetPowerRate() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.165f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.165f ) && true == EventCheck( 0.165f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ATTACK_BOX_Tiger_Claw_2nd", this, NULL, false, GetPowerRate() );
	}
	
	CommonFrameMove();
}

/** @function 	: ASI_A_ARA_TIGER_CLAW_END_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_TIGER_CLAW_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 호랑이 발톱

#pragma region FORCE_SPEAR
/** @function 	: ASI_A_ARA_FORCE_SPEAR_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_FORCE_SPEAR_Init()
{
	XSkinMeshReadyInBackground( L"elsword_SA_SwordBlastiong_slash.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"elsword_active_crecentCut_crecent.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"Force_Spear_round.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
}

/** @function 	: ASI_A_ARA_FORCE_SPEAR_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_FORCE_SPEAR_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.198f ) && CanUseForcePower() )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.198f ) && true == EventCheck( 0.198f, false ) && CanUseForcePower() )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UseForcePower( 1 );

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
		/// 메모 : 영혼 꿰기 (기공창) - 기공창의 사거리 증가
		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO3 ) )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Force_Spear_Memo", this );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Force_Spear", this );
#else  // ADD_ARA_MEMO
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Force_Spear", this );
#endif // ADD_ARA_MEMO

		D3DXVECTOR3 vPos = GetWeaponBonePos( 0, L"Dummy3_Weapon", 0 );

		if ( true == GetIsRight() )
			vPos = vPos - GetDirVector() * 25.f;
		else
			vPos = vPos + GetDirVector() * 25.f;
		
		vPos.y -= 25.f;
		
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AM_FORCE_SPEAR );
		if( NULL != pSkillTemplet )
		{
			USHORT usForceSpearPiercing = 0;
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AM_FORCE_SPEAR ) );	/// 스킬 레벨
	
			usForceSpearPiercing = static_cast<USHORT>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_PIERCING, iSkillTempletLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
			float fForceSpearDamageRel = 0.f;
			fForceSpearDamageRel = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
			usForceSpearPiercing = static_cast<USHORT>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_PIERCING ) );
	#endif // UPGRADE_SKILL_SYSTEM_2013

			CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this,
				L"DAMAGE_ARA_FORCE_SPEAR", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(),
				m_FrameDataNow.unitCondition.landPosition.y );

			// 기공창 숙련 적용, 관통 횟수를 이용해 여부 확인
			if ( NULL != pDamageEffect && 0 < usForceSpearPiercing )
			{
				pDamageEffect->SetDamageTime( pDamageEffect->GetDamageTime() + usForceSpearPiercing );

			#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
				/// 메모 : 영혼 꿰기 (기공창) - 기공창의 사거리 증가
				if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO3 ) )
				{
					CKTDGXMeshPlayer::CXMeshInstance* pMainEffect =  pDamageEffect->GetMainEffect();

					if( NULL != pMainEffect )
						pMainEffect->SetMaxLifeTime( pMainEffect->GetMaxLifeTime() * _CONST_ARA_::FORCE_SPEAR_MEMO_ADDITIONAL_RANGE_RATE );
				}
			#endif // ADD_ARA_MEMO
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this,
			L"DAMAGE_ARA_FORCE_SPEAR", GetPowerRate() * m_fForceSpearDamageRel, vPos, GetRotateDegree(), GetRotateDegree(),
			m_FrameDataNow.unitCondition.landPosition.y );

		// 기공창 숙련 적용, 관통 횟수를 이용해 여부 확인
		if ( NULL != pDamageEffect && 0 < m_usForceSpearPiercing )
		{
			pDamageEffect->SetDamageTime( pDamageEffect->GetDamageTime() + m_usForceSpearPiercing );

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
			/// 메모 : 영혼 꿰기 (기공창) - 기공창의 사거리 증가
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO3 ) )
			{
				CKTDGXMeshPlayer::CXMeshInstance* pMainEffect =  pDamageEffect->GetMainEffect();

				if( NULL != pMainEffect )
					pMainEffect->SetMaxLifeTime( pMainEffect->GetMaxLifeTime() * _CONST_ARA_::FORCE_SPEAR_MEMO_ADDITIONAL_RANGE_RATE );
			}
#endif // ADD_ARA_MEMO
		}
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	CommonFrameMove();
}

/** @function 	: ASI_A_ARA_FORCE_SPEAR_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_FORCE_SPEAR_EventProcess()
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
#pragma endregion 기공창

#pragma region GAIN_FORCE
/** @function 	: ASI_A_ARA_GAIN_FORCE_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_GAIN_FORCE_Init()
{
	XSkinMeshReadyInBackground( L"SI_A_ARA_GAIN_FORCE_KNOT.X" );
	TextureReadyInBackground( L"Heal01.dds" );
	TextureReadyInBackground( L"SI_A_ARA_GAIN_FORCE_NASUN_GATE_DEST.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
	TextureReadyInBackground( L"SM_SP_ShockWave.tga" );
	TextureReadyInBackground( L"SI_SA_ARA_SHADOW_RISING_Cloud.dds" );
	TextureReadyInBackground( L"Heal01.dds" );
}

/** @function 	: ASI_A_ARA_GAIN_FORCE_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_GAIN_FORCE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_usBeforeForceGainCount = m_usForceGainCount;

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
		D3DXVECTOR3 vGainForceSize( 1.f, 1.f, 1.f );	/// 이펙트 크기

		/// 메모 : 끝 없는 갈망 II (흡공) - 흡공 크기 증가 ( 이펙트 크기 )
		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO1 ) )
			vGainForceSize = _CONST_ARA_::GAIN_FORCE_MEMO_ADDITIONAL_RANGE_RATE;

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ARA_Gain_Force", this, NULL, false, GetPowerRate(), -1.f, vGainForceSize );
#else  // ADD_ARA_MEMO
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ARA_Gain_Force", this, NULL, false, GetPowerRate() );
#endif // ADD_ARA_MEMO
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( 0.066f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 0.066f ) && true == EventCheck( 0.066f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01" );
		
		if ( true == GetIsRight() )
			vPos = vPos + GetDirVector() * 180.f;
		else
			vPos = vPos - GetDirVector() * 180.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AM_GAIN_FORCE );
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ARA_GAIN_FORCE );
#endif //UPGRADE_SKILL_SYSTEM_2013
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AM_GAIN_FORCE ) );	/// 스킬 레벨
	
			float fDrainRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS, iSkillTempletLevel );
			USHORT usPiercing = static_cast<USHORT>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_PIERCING, iSkillTempletLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
			float fDrainRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS );
			USHORT usPiercing = static_cast<USHORT>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_PIERCING ) );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			
			// 마나 데미지
//#ifdef FIX_GAIN_FORCE_BUG
			m_iGainForceDamageTimeNow = 0;
			m_hGainForceIntakeEffect = INVALID_DAMAGE_EFFECT_HANDLE;
			//코드가 너무 지저분해지는 것을 막기 위해 지역변수이지만 접두어 m_ 유지 함
			CX2DamageEffect::CEffect* m_pGainForceDamaggeEffect = 
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_SI_A_ARA_GAIN_FORCE_INTAKE", 
				fDrainRate, vPos, GetRotateDegree(), GetRotateDegree(),	m_FrameDataNow.unitCondition.landPosition.y );

			if( NULL != m_pGainForceDamaggeEffect )
				m_hGainForceIntakeEffect = m_pGainForceDamaggeEffect->GetHandle();
//#else
//			m_pGainForceDamaggeEffect = NULL;
//			m_pGainForceDamaggeEffect = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, 
//				L"DAMAGE_EFFECT_SI_A_ARA_GAIN_FORCE_INTAKE", fDrainRate, vPos, GetRotateDegree(), GetRotateDegree(),
//				m_FrameDataNow.unitCondition.landPosition.y );
//#endif // FIX_GAIN_FORCE_BUG

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
			// 기본 데미지
			CX2DamageEffect::CEffect* pGainForceNormalDamage = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, 
				L"DAMAGE_EFFECT_SI_A_ARA_GAIN_FORCE", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

			if ( NULL != m_pGainForceDamaggeEffect )
			{
				if( 0 < usPiercing )
					m_pGainForceDamaggeEffect->SetDamageTime( usPiercing );

				/// 메모 : 끝 없는 갈망 II (흡공) - 흡공 크기 증가 ( 타격 범위 )
				if( NULL != pGainForceNormalDamage && true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO1 ) )
				{
					m_pGainForceDamaggeEffect->SetScale( _CONST_ARA_::GAIN_FORCE_MEMO_ADDITIONAL_RANGE_RATE );
					pGainForceNormalDamage->SetScale( _CONST_ARA_::GAIN_FORCE_MEMO_ADDITIONAL_RANGE_RATE );
				}
			}
#else  // ADD_ARA_MEMO
			if ( NULL != m_pGainForceDamaggeEffect && 0 < usPiercing )
				m_pGainForceDamaggeEffect->SetDamageTime( usPiercing );

			// 기본 데미지
			g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"DAMAGE_EFFECT_SI_A_ARA_GAIN_FORCE",
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // ADD_ARA_MEMO
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( 0.297f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 0.297f ) && true == EventCheck( 0.297f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
//#ifdef FIX_GAIN_FORCE_BUG
		if( 0 < m_iGainForceDamageTimeNow )
		{
			UpForcePower( m_usForceGainCount * m_iGainForceDamageTimeNow );
			m_iGainForceDamageTimeNow = 0;
			m_hGainForceIntakeEffect = INVALID_DAMAGE_EFFECT_HANDLE;
		}
//#else
//		if ( NULL != m_pGainForceDamaggeEffect && 0 < m_pGainForceDamaggeEffect->GetDamageTimeNow() )
//		{
//			UpForcePower( m_usForceGainCount * m_pGainForceDamaggeEffect->GetDamageTimeNow() );
//		}
//#endif // FIX_GAIN_FORCE_BUG

	}

	CommonFrameMove();
}

/** @function 	: ASI_A_ARA_GAIN_FORCE_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_GAIN_FORCE_EventProcess()
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

	CommonEventProcess();
}
#pragma endregion 흡공

#ifdef ARA_CHANGE_CLASS_FIRST
//아라 1차 전직 - 소선(Little Hsien)
//소선 커맨드
void CX2GUAra::ASI_COMBO_ALH_ZZXX_EventProcess() //ZZXX
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( ASI_COMBO_ALH_ZZXX_LANDING );
	}

	CommonEventProcess();
}
void CX2GUAra::ASI_COMBO_ALH_ZZXX_LANDING_EventProcess() //ZZXX
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GUAra::ASI_DASH_COMBO_ALH_ZZ_EventProcess() //ZZ
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GUAra::ASI_JUMP_ATTACK_ALH_ZZ_EventProcess() //ZZ
{
	if ( true == IsOnSomethingFuture() )
	{
		bool &bPureDown = m_FrameDataFuture.stateParam.bEventFlagList[0];

		if ( true == bPureDown )
		{
			m_fCanNotJumpTime = 0.2f;
		}

		StateChange( USI_JUMP_LANDING );
	}

	CommonEventProcess();
}

//소선 액티브
void CX2GUAra::ASI_A_ALH_BREAKING_Init() //이화접목 대기
{
	TextureReadyInBackground( L"SI_A_ALH_BREAKING_Jin.dds" );	
}
void CX2GUAra::ASI_A_ALH_BREAKING_FrameMove() //이화접목 대기
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Breaking", this );
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_A_ALH_BREAKING_EventProcess() //이화접목 대기
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		
		StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
void CX2GUAra::ASI_A_ALH_BREAKING_ATTACK_Init() //이화접목 공격
{
	XSkinMeshReadyInBackground( L"SI_A_ALH_BREAKING_Ground_Wave.x" );
	XSkinMeshReadyInBackground( L"SI_A_ALH_BREAKING_Traiectory.x" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );	
}
void CX2GUAra::ASI_A_ALH_BREAKING_ATTACK_FrameMove() //이화접목 공격
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.23f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.23f ) && true == EventCheck( 0.23f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Breaking_Attack", this );

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
		/// 메모 : 물 흐르듯이 (이화접목) - 회피율 50% 증가
		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO7 ) )
		{
			SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_BREAKING_MEMO ) );
		}
#endif // ADD_ARA_MEMO
	}
	CommonFrameMove();
}
void CX2GUAra::ASI_A_ALH_BREAKING_ATTACK_EventProcess() //이화접목 공격
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

	CommonEventProcess();
}
void CX2GUAra::ASI_A_ALH_BREAKING_ATTACK_EndFuture() //이화접목 공격
{
	CommonStateEndFuture();
	m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
}

void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALH_DOUBLE_COLLISION_START", stateData_.stateID );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DOUBLE_COLLISION_START_Start );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DOUBLE_COLLISION_START_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DOUBLE_COLLISION_START_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DOUBLE_COLLISION_START_EventProcess );
}

//소선 스페셜 액티브
void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_START_Init() //팔괘장_START
{
	XMeshReadyInBackground( L"AssaultSlash02.y" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"Lire_Kick_Impact01.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_particle.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"RingAlpha.dds" );
	TextureReadyInBackground( L"SA_ALH_DOUBLE_COLLISION_Rune_Base.tga" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"Sander_Aelo_Light.dds" );

}
void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_START_Start() //팔괘장_START
{
	CommonStateStart();

	m_usBeforeForceGainCount = m_usForceGainCount;
}
void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_START_FrameMove() //팔괘장_START
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Double_Collision_01", this );
			UpForcePower( 1 );	// 스킬 사용 기본 기력 회복
			m_usBeforeForceGainCount = m_usForceGainCount;
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.5f ) && true == EventCheck( 0.5f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22
		bool bIsMemoAttack = false;		/// 메모 적용 여부

		/// 메모 : 정신을 잃게 만드는 타격 II (용아 3식 : 팔괘장) - 공격력 20% 증가 및 마나 브레이크 해제
		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO8 ) )
			bIsMemoAttack = true;

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Double_Collision_02", this, NULL, bIsMemoAttack, GetPowerRate() );
#else  // ADD_ARA_MEMO
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Double_Collision_02", this, NULL, false, GetPowerRate() );
#endif // ADD_ARA_MEMO
	}

	
	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_START_EventProcess() //팔괘장_START
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			StateChangeSpecialAttack( m_pSkillTemplet );
			UpForcePower( m_usBeforeForceGainCount ); // 기력 캔슬 기력 회복
		}
#endif
		else
		{
			StateChange( ASI_SA_ALH_DOUBLE_COLLISION_END );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALH_DOUBLE_COLLISION_END", stateData_.stateID );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DOUBLE_COLLISION_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DOUBLE_COLLISION_END_EventProcess );
}
void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_END_FrameMove() //팔괘장_END
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Double_Collision_03", this );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.5f ) && true == EventCheck( 0.5f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_Double_Collision_04", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ALH_DOUBLE_COLLISION_END_EventProcess() //팔괘장_END
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

	CommonEventProcess();
}

//아라 2차 전직 - 제천(Sakra Devanam)

void CX2GUAra::ASI_DASH_JUMP_COMBO_ASD_ZX_EventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( ASI_COMBO_ALH_ZZXX_LANDING );
	}
	else if ( m_fAttackCancelAfter <= m_pXSkinAnim->GetNowAnimationTime() )
	{
		if ( true == m_InputData.oneZ )
		{
			StateChange( ASI_JUMP_ATTACK_Z );
		}
		else if ( true == m_InputData.oneX )
		{
			StateChange( ASI_JUMP_ATTACK_X );
		}
	}
	CommonEventProcess();
}
void CX2GUAra::ASI_DOUBLE_JUMP_FRONT_COMBO_ASD_Z_BACK_EventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
	else if ( m_fAttackCancelAfter <= m_pXSkinAnim->GetNowAnimationTime() )
	{
		if ( true == m_InputData.oneZ )
		{
			StateChange( ASI_JUMP_ATTACK_Z );
		}
		else if ( true == m_InputData.oneX )
		{
			StateChange( ASI_JUMP_ATTACK_X );
		}
	}
	CommonEventProcess();
}

void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ASD_TURBULENT_WAVE_START", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_START_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_START_Start );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_START_FrameMove );
	stateData_.StateEnd			= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_START_End );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_START_EventProcess );
}

void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_START_Init() // 맹호 4식 - 노도
{
	XSkinMeshReadyInBackground( L"SI_SA_ASD_TURBULENT_WAVE_Wave.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_TURBULENT_WAVE_Stone.X" );
	XMeshReadyInBackground( L"SI_SA_ASD_TURBULENT_WAVE_Stone_D.y" );	
	TextureReadyInBackground( L"WhitePoint.dds");
	TextureReadyInBackground( L"Lire_Kick_Impact01.dds");
	TextureReadyInBackground( L"ARA_SA_FallingDragon_sparkHigh.dds");
	TextureReadyInBackground( L"Arme_Ring2.dds");
	TextureReadyInBackground( L"Sander_Aelo_Light.dds");
	TextureReadyInBackground( L"SI_SA_ASD_TURBULENT_WAVE_Tiger.dds");
	TextureReadyInBackground( L"CDC_LEG_SHOT_GUNFIRE.dds");
	TextureReadyInBackground( L"Colorballgray.dds");
	TextureReadyInBackground( L"stone.dds");
	TextureReadyInBackground( L"A_ALH_ROCK_CRASH_Tiger.tga" );	
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_START_Start() // 맹호 4식 - 노도
{
	CommonStateStart();
	m_usBeforeForceGainCount = m_usForceGainCount;
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_START_FrameMove()// 맹호 4식 - 노도
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		UpForcePower(1);
		m_hTempEffectSet = 
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Start", this );	
		m_vecEffectSetToDeleteOnDamageReact.push_back(m_hTempEffectSet);
		m_vecEffectSetToDeleteOnDie.push_back(m_hTempEffectSet);
	}

	//바닥 돌무더기 생성
	if( true == IsOnSomethingFuture() )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.402f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( true == m_pXSkinAnim->EventTimer( 0.402f ) && true == EventCheck( 0.402f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.602f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( true == m_pXSkinAnim->EventTimer( 0.602f ) && true == EventCheck( 0.602f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.802f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( true == m_pXSkinAnim->EventTimer( 0.802f ) && true == EventCheck( 0.802f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 1.002f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( true == m_pXSkinAnim->EventTimer( 1.002f ) && true == EventCheck( 1.002f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 1.202f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( true == m_pXSkinAnim->EventTimer( 1.202f ) && true == EventCheck( 1.202f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_START_EventProcess()// 맹호 4식 - 노도
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ASI_SA_ASD_TURBULENT_WAVE_LOOP );
	}
	CommonEventProcess();
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_START_End()
{
	CommonStateEnd();
	
	if( false == m_pXSkinAnim->IsAnimationEnd() )
	{
		if ( INVALID_EFFECTSET_HANDLE != m_hTempEffectSet )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hTempEffectSet );

	}
}


void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_LOOP_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ASD_TURBULENT_WAVE_LOOP", stateData_.stateID );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_LOOP_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_LOOP_EventProcess );
	stateData_.StateEnd			= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_LOOP_End );
}

void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_LOOP_FrameMove()// 맹호 4식 - 노도
{
	//바닥 돌무더기 생성
	if( true == IsOnSomethingFuture() )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.051f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( true == m_pXSkinAnim->EventTimer( 0.051f ) && true == EventCheck( 0.051f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.251f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( true == m_pXSkinAnim->EventTimer( 0.251f ) && true == EventCheck( 0.251f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.451f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( true == m_pXSkinAnim->EventTimer( 0.451f ) && true == EventCheck( 0.451f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_Stream", this, NULL, false, GetPowerRate() );
		}
	}
	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_LOOP_EventProcess()// 맹호 4식 - 노도
{	
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_SA_ASD_TURBULENT_WAVE_END );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_LOOP_End()// 맹호 4식 - 노도
{	
	CommonStateEnd();
	if ( INVALID_EFFECTSET_HANDLE != m_hTempEffectSet )
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hTempEffectSet );
}

void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ASD_TURBULENT_WAVE_END", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_END_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_TURBULENT_WAVE_END_EventProcess );
}

void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_END_Init()// 맹호 4식 - 노도
{
	XSkinMeshReadyInBackground( L"SI_SA_ASD_TURBULENT_WAVE_Stone_A.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_TURBULENT_WAVE_Stone_B.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_TURBULENT_WAVE_Stone_C.X" );
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_END_FrameMove()// 맹호 4식 - 노도
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( true == IsOnSomethingFuture() )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Turbulent_Wave_End", this, NULL, false, GetPowerRate() );
		}
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ASD_TURBULENT_WAVE_END_EventProcess()// 맹호 4식 - 노도
{
	if ( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.974f && false == IsOnSomethingFuture())
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

#pragma region ASI_SA_ASD_SUPPRESSION
/** @function 	: ASI_SA_ASD_SUPPRESSION_Init
	@brief 		: 아라 2차 전직 제천 - 제압
*/
void CX2GUAra::ASI_SA_ASD_SUPPRESSION_Init()
{
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Divide.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave_A.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave_B.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Sk_Dummy.X" );
	XSkinMeshReadyInBackground( L"Triple_W_PropertyA01.X" );
	XSkinMeshReadyInBackground( L"rena_A_evoke_Dummy.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds");
	TextureReadyInBackground( L"Arme_Critical2.dds");
	TextureReadyInBackground( L"Arme_SpecialAttack1.dds");
	TextureReadyInBackground( L"ColorBallFire.dds");
	TextureReadyInBackground( L"ColorBallGray.dds");
	TextureReadyInBackground( L"Condense_Light01.dds");
	TextureReadyInBackground( L"Explosion_Light_Ring01.dds");
	TextureReadyInBackground( L"Gate_Light_Arua.dds");
	TextureReadyInBackground( L"Gate_Light2.dds");
	TextureReadyInBackground( L"GuideArrow01.dds");
	TextureReadyInBackground( L"soket_earth_quake_smoke02.dds");
}

/** @function 	: ASI_SA_ASD_SUPPRESSION_StateStart
	@brief 		: 아라 2차 전직 제천 - 제압
*/
void CX2GUAra::ASI_SA_ASD_SUPPRESSION_StateStart()
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	UseForcePower( 6 );		/// 기력 소비
#endif //UPGRADE_SKILL_SYSTEM_2013

	CommonStateStart();
}

/** @function 	: ASI_SA_ASD_SUPPRESSION_FrameMove
	@brief 		: 아라 2차 전직 제천 - 제압
*/
void CX2GUAra::ASI_SA_ASD_SUPPRESSION_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

    /// 전조 이펙트 생성을 위한 더미 데미지 이펙트 생성
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )		
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )		
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		// 오현빈 // 사내에서 기력 없이 사용 할 때 ASSERT발생 문제 수정
		m_fSuppresionPowerRate = max(1.f, GetPowerRate() * GetForcePower());

		UseForcePower( GetForcePower() );
#endif //UPGRADE_SKILL_SYSTEM_2013
		D3DXVECTOR3 vFirePos = GetPos();

		/// y축은 방향 벡터 연산 제외
		if( true == GetIsRight() )
		{
			vFirePos.x += GetDirVector().x * 500.f;
			vFirePos.z += GetDirVector().z * 500.f;
		}
		else
		{
			vFirePos.x -= GetDirVector().x * 500.f;
			vFirePos.z -= GetDirVector().z * 500.f;
		}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ASD_SUPPRESSION_READY_GROUND", m_fSuppresionPowerRate, vFirePos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else //UPGRADE_SKILL_SYSTEM_2013
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ASD_SUPPRESSION_READY_GROUND", GetPowerRate(), vFirePos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
    /// 실제 타격 데미지 이펙트 생성
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 1.15f ) )	
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 1.15f ) && true == EventCheck( 1.15f, false ) )	
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vFirePos = GetPos();

		if( true == GetIsRight() )
			vFirePos += GetDirVector() * 500.f;
		else
			vFirePos -= GetDirVector() * 500.f;

		vFirePos.y += 700.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		CX2DamageEffect::CEffect* pDE = 
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ASD_SUPPRESSION_DOWN_FORCE", m_fSuppresionPowerRate,
				vFirePos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else //UPGRADE_SKILL_SYSTEM_2013
		CX2DamageEffect::CEffect* pDE = 
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ASD_SUPPRESSION_DOWN_FORCE",  GetPowerRate(), vFirePos, 
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifndef BALANCE_PATCH_20131107
		if( NULL != pDE )		/// 스킬 레벨에 따른 버프 펙터 삽입
		{
			CX2DamageManager::DamageData& damageData = pDE->GetDamageData();
			//if( NULL != pDamageData )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ASD_SUPPRESSION );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
				{
					if ( NULL == GetUnit() )
						return;

					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_ASD_SUPPRESSION ) );	/// 스킬 레벨

					damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ), false ); /// 데미지 이펙트에 제압 디버프를 추가 ( 프레스 디버프, 제압 디버프 두가지 적용 )
				}
#else //UPGRADE_SKILL_SYSTEM_2013
				if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )	/// 데미지 이펙트에 제압 디버프를 추가 ( 프레스 디버프, 제압 디버프 두가지 적용 )
					damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0], false );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
		}
#endif //BALANCE_PATCH_20131107
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ASD_SUPPRESSION_EventProcess
	@brief 		: 아라 2차 전직 제천 - 제압
*/
void CX2GUAra::ASI_SA_ASD_SUPPRESSION_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	CommonEventProcess();
}
#pragma endregion 제압

void CX2GUAra::ASI_SA_ALH_POWER_OF_WHITER_TIGER_Init()
{
	XSkinMeshReadyInBackground(L"Ara_Dash_Combo_X_M01.X");
	XMeshReadyInBackground( L"SI_A_ARA_STONE_CUTTER_Ground_Wave.Y" );
	TextureReadyInBackground( L"Arme_Ring2.dds");
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds");
	TextureReadyInBackground( L"Ara_Orange_Light.dds");
	TextureReadyInBackground( L"Explosion_Light02.dds");
	TextureReadyInBackground( L"WhiteCircle02.DDS");
	TextureReadyInBackground( L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.tga");
	TextureReadyInBackground( L"ALH_POWER_OF_WHITE_TIGER.TGA");
	TextureReadyInBackground( L"A_ALH_ROCK_CRASH_Tiger.tga");
	TextureReadyInBackground( L"Particle_Blur.DDS");
	TextureReadyInBackground( L"Explosion_Sphere.dds");
	TextureReadyInBackground( L"WhiteCircle02.DDS");
}
void CX2GUAra::ASI_SA_ALH_POWER_OF_WHITER_TIGER_FrameMove() // 맹호격
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		UpForcePower( 1 );
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Power_of_WhiteTiger_Start", this );	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if ( true == m_pXSkinAnim->EventTimerOneshot( 0.8f ) ) 
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if ( true == m_pXSkinAnim->EventTimer( 0.8f ) && true == EventCheck( 0.8f, false ) ) 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hTempEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Power_of_WhiteTiger_BlackHole", this );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if ( true == m_pXSkinAnim->EventTimerOneshot( 1.f ) ) 
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if ( true == m_pXSkinAnim->EventTimer( 1.f ) && true == EventCheck( 1.f, false ) ) 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( true == GetIsRight() ) 
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Power_of_WhiteTiger_Face", this );		
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Power_of_WhiteTiger_Face_Left", this );		
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ALH_POWER_OF_WHITER_TIGER_EventProcess() // 맹호격
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() &&
		true == IsOnSomethingFuture() )
	{
		StateChange( ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING );
	}
	CommonEventProcess();
}
void CX2GUAra::ASI_SA_ALH_POWER_OF_WHITER_TIGER_End()
{
	CommonStateEnd();

	if ( INVALID_EFFECTSET_HANDLE != m_hTempEffectSet )
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hTempEffectSet );
}
void CX2GUAra::ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING_FrameMove() // 맹호격
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{				
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Power_of_WhiteTiger_Landing", this, NULL, false, GetPowerRate() );

		if( true == GetIsRight() ) 
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Power_of_WhiteTiger_Face", this );		
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Power_of_WhiteTiger_Face_Left", this );		

		CX2DamageEffect::CEffect* pDE = 
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ALH_POWER_OF_WHITE_TIGER_BUFF", 
				GetPowerRate(), GetBonePos(L"Bip01"), GetRotateDegree(), GetRotateDegree(), 
				m_FrameDataNow.unitCondition.landPosition.y );
		
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO12 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ALH_POWER_OF_WHITE_TIGER_MEMO_DEBUFF", 
				GetPowerRate(), GetBonePos(L"Bip01"), GetRotateDegree(), GetRotateDegree(), 
				m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS

		if( NULL != pDE )		/// 스킬 레벨에 따른 버프 펙터 삽입
		{
			CX2DamageManager::DamageData& damageData = pDE->GetDamageData();
			//if( NULL != pDamageData )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ALH_WHITE_TIGER );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
				{
					if ( NULL == GetUnit() )
						return;

					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_ALH_WHITE_TIGER ) );	/// 스킬 레벨

					damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
				}
#else //UPGRADE_SKILL_SYSTEM_2013
				if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
				
			}
		}
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ALH_POWER_OF_WHITER_TIGER_LANDING_EventProcess() // 맹호격
{	
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GUAra::ASI_SA_ALH_PEERLESSNESS_SLASH_START_Init() // 무쌍참
{
	TextureReadyInBackground( L"SA_ALH_PEERLESSNESS_SLASH.tga" );
	TextureReadyInBackground( L"elsword_active_crecentCut_crecent.dds" );
	TextureReadyInBackground( L"title_StarLight_Smash_spark.dds" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_particle.dds" );
	TextureReadyInBackground( L"Sander_Aelo_Light.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"Condense_Light01.dds" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.DDS" );
	TextureReadyInBackground( L"SM_MagicAttakcB_Smoke.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"Ara_Orange_LEAF.dds" );
	TextureReadyInBackground( L"ARA_SA_WindWedge_windLine.dds" );
	XMeshReadyInBackground( L"Wind_Liner02.Y" );
	XMeshReadyInBackground( L"Ring_Plane.y" );
	XMeshReadyInBackground( L"SA_ALH_PEERLESSNESS_SHADOW.Y" );
	XMeshReadyInBackground( L"SA_ALH_PEERLESSNESS_SHADOW02.Y" );
	XMeshReadyInBackground( L"SA_ALH_PEERLESSNESS_SHADOW03.Y" );
}
void CX2GUAra::ASI_SA_ALH_PEERLESSNESS_SLASH_START_Start()
{
	CommonStateStart();
	m_usBeforeForceGainCount = m_usForceGainCount;
}
void CX2GUAra::ASI_SA_ALH_PEERLESSNESS_SLASH_START_FrameMove() // 무쌍참
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );
	
	const float fInvisibleStartTime = 0.55f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		UpForcePower( 1 );				
		CX2EffectSet::Handle hEffectSset =
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Peerlessness_Slash_Start", this );	
		m_vecEffectSetToDeleteOnDie.push_back(hEffectSset);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( fInvisibleStartTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( fInvisibleStartTime ) && true == EventCheck( fInvisibleStartTime, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		SetInvisibility(true);
		m_fRenderParamColorA = 0.f;
		m_vPreTransScale = (true == IsChangedGameUnitScaleByBuff()) ? m_vTransScale : D3DXVECTOR3(1,1,1) ;
		
#ifdef VERIFY_STAT_BY_BUFF
		TransformScale( PROTECT_VECTOR3( 0.01f, 0.01f, 0.01f ) );
#else	// VERIFY_STAT_BY_BUFF
		TransformScale( D3DXVECTOR3( 0.01f, 0.01f, 0.01f ) );
#endif // VERIFY_STAT_BY_BUFF

		CX2EffectSet::Handle hEffectSset =
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Peerlessness_Slash_Attack_Effect", this, NULL, false, GetPowerRate() );
		m_vecEffectSetToDeleteOnDie.push_back(hEffectSset);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.710f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 0.710f ) && true == EventCheck( 0.710f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CX2EffectSet::Handle hEffectSset =
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Peerlessness_Slash_Attack", this, NULL, false, GetPowerRate() );
		m_vecEffectSetToDeleteOnDie.push_back(hEffectSset);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 1.532f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 1.532f ) && true == EventCheck( 1.532f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		SetInvisibility(false);
		m_fRenderParamColorA = 1.f;
		TransformScale( m_vPreTransScale );
	}
	CommonFrameMove();

	m_pUnitShadow->SetShowObject(!m_bInvisibility);
}
void CX2GUAra::ASI_SA_ALH_PEERLESSNESS_SLASH_START_EventProcess() // 무쌍참
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
			StateChange( ASI_SA_ALH_PEERLESSNESS_SLASH_END );
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() && true == IsOnSomethingFuture() ) //바닥에서만 기력 캔슬 할 수 있도록 수정
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}
void CX2GUAra::ASI_SA_ALH_PEERLESSNESS_SLASH_START_End() // 무쌍참
{
	CommonStateEnd();
	SetInvisibility(false);
	m_fRenderParamColorA = 1.f;
	TransformScale( m_vPreTransScale );
}
void CX2GUAra::ASI_SA_ALH_PEERLESSNESS_SLASH_END_FrameMove() // // 무쌍참
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Peerlessness_Slash_End", this );			
	}	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.138f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 0.138f ) && true == EventCheck( 0.138f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Peerlessness_Slash_Last_Attack", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ALH_PEERLESSNESS_SLASH_END_EventProcess() // // 무쌍참
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_JUMP_DOWN );
	}
	CommonEventProcess();
}

void CX2GUAra::SetInvisibility(bool bVal_)
{
	m_bInvisibility					= bVal_;
	m_bAbsoluteInvisibility			= bVal_;

	if( false == m_bInvisibility )
		m_RenderParam.color.a = m_fPreRenderParamColorA;

	m_fPreRenderParamColorA = m_RenderParam.color.a;
	m_fRenderParamColorA	= 1.f;
	
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


void CX2GUAra::ASI_SA_ALH_DESCENT_DRAGON_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	//폭쇄
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALH_DESCENT_DRAGON", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DESCENT_DRAGON_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DESCENT_DRAGON_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALH_DESCENT_DRAGON_EventProcess );
}

void CX2GUAra::ASI_SA_ALH_DESCENT_DRAGON_Init() /// 용아 최종오의 - 폭쇄
{
	XSkinMeshReadyInBackground(L"SI_SA_ALH_DESCENT_DRAGON_Stone_A.X");
	XSkinMeshReadyInBackground(L"SI_SA_ALH_DESCENT_DRAGON_Stone_B.X");
	XMeshReadyInBackground(L"SI_SA_ALH_DESCENT_DRAGON_MiniStone_A.y");
	XMeshReadyInBackground(L"SI_SA_ALH_DESCENT_DRAGON_Prickle.y");
	XMeshReadyInBackground(L"Up_Light01_Mesh.Y");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
}
void CX2GUAra::ASI_SA_ALH_DESCENT_DRAGON_FrameMove() /// 용아 최종오의 - 폭쇄
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		if ( true == GetIsRight() )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ALH_Descent_Dragon", this );
		else
#ifdef VERIFY_STAT_BY_BUFF
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ALH_Descent_Dragon", this, NULL, IsHyperState(), -1.f, -1.f, 
			GetVec3ScaleByUnit(), true, GetPos(), GetRotateDegree() + D3DXVECTOR3( 0, 180, 0 ), GetDirVector() );
		
#else	// VERIFY_STAT_BY_BUFF
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ALH_Descent_Dragon", this, NULL, IsHyperState(), -1.f, -1.f, 
			GetScaleByUnit(), true, GetPos(), GetRotateDegree() + D3DXVECTOR3( 0, 180, 0 ), GetDirVector() );
#endif // VERIFY_STAT_BY_BUFF
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( 0.725f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( 0.725f ) && true == EventCheck( 0.725f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*)this, L"DAMAGE_EFFECT_ALH_DESCENT_DRAGON", 
			GetPowerRate(), GetBonePos(L"Bip01"), GetRotateDegree(), GetRotateDegree(),
			m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}
void CX2GUAra::ASI_SA_ALH_DESCENT_DRAGON_EventProcess() /// 용아 최종오의 - 폭쇄
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}
	CommonEventProcess();
}
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef ARA_CHANGE_CLASS_FIRST
#pragma region ROCK_CRASH


void CX2GUAra::ASI_A_ALH_ROCK_CRASH_FALLING_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ALH_ROCK_CRASH_FALLING_START", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_START_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_START_StateStart );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_START_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_START_EventProcess );
	stateData_.StateEnd		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_START_StateEnd );
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_START_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_START_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"A_ALH_ROCK_CRASH_Tiger.tga" );
	TextureReadyInBackground( L"A_ALH_ROCK_Strong.dds" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_START_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_START_StateStart()
{
	CommonStateStart();
	
	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_START_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_START_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Rock_Crash_Start", this, NULL,
			false, GetPowerRate() );

		if ( INVALID_EFFECTSET_HANDLE != hEffectSet )
			m_vecEffectSetToDeleteOnStateEnd.push_back( hEffectSet );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( 0.275f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 0.275f ) && true == EventCheck( 0.275f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetBonePos( L"Bip01" );

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-27

		/// 메모 : 바위로 바위 치기(맹호 1식 : 바위깨기) - 크리티컬 40% 증가
		bool bIsMemoAttack = false;

		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO6 ) )
			bIsMemoAttack = true;

		m_hTempEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ALH_DOUBLE_COLLISION_ATTACKBOX", this, NULL,
			bIsMemoAttack, GetPowerRate() );

#else // ADD_ARA_MEMO

		m_hTempEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ALH_DOUBLE_COLLISION_ATTACKBOX", this, NULL,
			false, GetPowerRate() );

#endif // ADD_ARA_MEMO

	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( true == m_pXSkinAnim->EventTimerOneshot( 0.655f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( true == m_pXSkinAnim->EventTimer( 0.655f ) && true == EventCheck( 0.655f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( INVALID_EFFECTSET_HANDLE != m_hTempEffectSet )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hTempEffectSet );
	}

	CommonFrameMove();
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_START_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_A_ALH_ROCK_CRASH_END );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_START_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_START_StateEnd()
{
	if ( INVALID_EFFECTSET_HANDLE != m_hTempEffectSet )
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hTempEffectSet );

	CommonStateEnd();

	m_PhysicParam.nowSpeed.x = 700.f;
}

void CX2GUAra::ASI_A_ALH_ROCK_CRASH_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ALH_ROCK_CRASH_END", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_END_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALH_ROCK_CRASH_END_EventProcess );
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_END_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_END_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"A_ALH_ROCK_CRASH_Tiger.tga" );
	TextureReadyInBackground( L"A_ALH_ROCK_Strong.dds" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_END_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-27

		/// 메모 : 바위로 바위 치기(맹호 1식 : 바위깨기) - 크리티컬 40% 증가
		bool bIsMemoAttack = false;

		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO6 ) )
			bIsMemoAttack = true;

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Rock_Crash_End", this, NULL, bIsMemoAttack, GetPowerRate() );

#else // ADD_ARA_MEMO

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Rock_Crash_End", this, NULL, false, GetPowerRate() );

#endif // ADD_ARA_MEMO

	}

	CommonFrameMove();
}

/** @function 	: ASI_A_ALH_ROCK_CRASH_END_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_ROCK_CRASH_END_EventProcess()
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

	CommonEventProcess();
}
#pragma endregion 바위 깨기

#pragma region LOW_BRANDISH

void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_SLIDING_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID	= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ASD_LOW_BRANDISH_SLICE", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_SLIDING_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_SLIDING_StateStart );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_SLIDING_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_SLIDING_EventProcess );
	stateData_.StateEnd		= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_SLIDING_StateEnd );
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_SLIDING_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_SLIDING_Init()
{
	XSkinMeshReadyInBackground( L"SI_A_ASD_LOW_BRANDISH_Cut_A.X" );
	XSkinMeshReadyInBackground( L"SI_A_ASD_LOW_BRANDISH_Cut_B.X" );
	XSkinMeshReadyInBackground( L"WindWard_XMesh01.X" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.dds" );
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_SLIDING_StateStart
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_SLIDING_StateStart()
{
	CommonStateStart();
	
	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_SLIDING_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_SLIDING_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Low_Brandish_Sliding_Kick", this, NULL, false, GetPowerRate() );

		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ASD_LOW_BRANDISH );
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );	/// 자신한테 거는 버프
	}

	CommonFrameMove();
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_SLIDING_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_SLIDING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef FIX_SKILL_CANCEL_ARA
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
#else
		if ( true == m_bFPCancel && NULL != m_pSkillTemplet )
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_pSkillTemplet );
		}
#endif
		else
		{
			StateChange( ASI_A_ASD_LOW_BRANDISH_KICK );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_SLIDING_StateEnd
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_SLIDING_StateEnd()
{
	CommonStateEnd();
}

void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_KICK_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ASD_LOW_BRANDISH_KICK", stateData_.stateID );
	stateData_.StateInit			= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_KICK_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_KICK_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ASD_LOW_BRANDISH_KICK_EventProcess );
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_KICK_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_KICK_Init()
{
	XMeshReadyInBackground( L"SI_A_ASD_LOW_BRANDISH_Kick.y" );
	XSkinMeshReadyInBackground( L"WindWard_XMesh01.X" );
	XSkinMeshReadyInBackground( L"SI_A_ASD_LOW_BRANDISH_Cut_B.X" );
	TextureReadyInBackground( L"Colorballgray.dds" );
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_KICK_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_KICK_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_Low_Brandish_Kick", this );
	}

	CommonFrameMove();
}

/** @function 	: ASI_A_ASD_LOW_BRANDISH_KICK_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ASD_LOW_BRANDISH_KICK_EventProcess()
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

	CommonEventProcess();
}
#pragma endregion 지축

#pragma region VOID_BREAK


void CX2GUAra::ASI_SA_ASD_VOID_BREAK_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ASD_VOID_BREAK", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_VOID_BREAK_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_VOID_BREAK_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ASD_VOID_BREAK_EventProcess );
}

/** @function 	: ASI_SA_ASD_VOID_BREAK_Init
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ASD_VOID_BREAK_Init()
{
	XSkinMeshReadyInBackground( L"Lire_SI_SA_Gungnir_Mesh05.X" );
	TextureReadyInBackground( L"ALH_POWER_OF_WHITE_TIGER.tga" );
	TextureReadyInBackground( L"A_ALH_ROCK_CRASH_Tiger.tga" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	TextureReadyInBackground( L"ColorBallRed2.dds" );
	TextureReadyInBackground( L"Condense_Pulse01.dds" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"waldo_trock_specialAttackB_cry_waveLine.dds" );
}

/** @function 	: ASI_SA_ASD_VOID_BREAK_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ASD_VOID_BREAK_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLightByBerserkMode( 0.001f );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef FIX_ARA_SKILL_BUG
		bool bHyper = false;
		if ( GetRemainHyperModeTime() > 0.f )
			bHyper = true;

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ASD_Void_Break", this, NULL, bHyper, GetPowerRate() );
#else //FIX_ARA_SKILL_BUG
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ASD_Void_Break", this, NULL, false, GetPowerRate() );
#endif //FIX_ARA_SKILL_BUG

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		CX2DamageEffect::CEffect* pDE = 
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ALH_POWER_OF_WHITE_TIGER_BUFF", 
			GetPowerRate(), GetBonePos(L"Bip01"), GetRotateDegree(), GetRotateDegree(), 
			m_FrameDataNow.unitCondition.landPosition.y );

		if( NULL != pDE )		/// 스킬 레벨에 따른 버프 펙터 삽입
		{
			CX2DamageManager::DamageData& damageData = pDE->GetDamageData();
			//if( NULL != pDamageData )
			{
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
				CX2SkillTree* pSkillTree = g_pData->GetSkillTree();
				const int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ASD_POWER_OF_TIGER, true );

				if ( iSkillLevel > 0 && NULL != pSkillTree )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ASD_POWER_OF_TIGER  );

					if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					{
						CX2BuffFactorPtr ptrBuffFactorClone = pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillLevel );
						SetBuffFactorToGameUnit( *ptrBuffFactorClone );
						damageData.PushBuffFactor( ptrBuffFactorClone );
					}
				}
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	CommonFrameMove();
}

/** @function 	: ASI_SA_ASD_VOID_BREAK_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_SA_ASD_VOID_BREAK_EventProcess()
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
#pragma endregion 맹호 최종오의 맹호 파천

#endif //ARA_CHANGE_CLASS_FIRST

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
#pragma region SI_A_ARA_QUICK_PIERCING
/** @function 	: ASI_A_ARA_Quick_Piercing_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_Quick_Piercing_EventProcess()
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

	CommonEventProcess();
}

void CX2GUAra::ASI_A_ARA_Quick_Piercing_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime ) && true == EventCheck( m_fDamageDataChangeTime, false ) )
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
		}
	}

	CommonFrameMove();
}
#pragma endregion 천격

#pragma region SI_A_ARA_SHOULDER_BLOW
/** @function 	: ASI_A_ARA_Shoulder_Blow_Init
	@brief 		: 
*/
void CX2GUAra::ASI_A_ARA_Shoulder_Blow_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"A_ALH_ROCK_CRASH_Tiger.tga" );
	TextureReadyInBackground( L"A_ALH_ROCK_Strong.dds" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
}

/** @function 	: ASI_A_ALH_Shoulder_Blow_FrameMove
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_Shoulder_Blow_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.001f ) && true == EventCheck( 0.001f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		UseForcePower( 2 );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		UseForcePower( 3 );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		bool bHyper = false;
		if ( GetRemainHyperModeTime() > 0.f )
			bHyper = true;

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_A_ARA_SHOULDER_BLOW", this, NULL, bHyper, GetPowerRate() );
	}

	CommonFrameMove();
}

/** @function 	: ASI_A_ALH_Shoulder_Blow_EventProcess
	@brief 		: 
*/
void CX2GUAra::ASI_A_ALH_Shoulder_Blow_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 철산고

#pragma region ASI_A_ASD_Falling_Blossom
/** @function 	: ASI_A_ASD_Falling_Blossom_Start_Init
	@brief 		: 스킬 개편 - 낙화
*/
void CX2GUAra::ASI_A_ASD_Falling_Blossom_Start_Init()
{
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Divide.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave_A.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave_B.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Sk_Dummy.X" );
	XSkinMeshReadyInBackground( L"Triple_W_PropertyA01.X" );
	XSkinMeshReadyInBackground( L"rena_A_evoke_Dummy.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds");
	TextureReadyInBackground( L"Arme_Critical2.dds");
	TextureReadyInBackground( L"Arme_SpecialAttack1.dds");
	TextureReadyInBackground( L"ColorBallFire.dds");
	TextureReadyInBackground( L"ColorBallGray.dds");
	TextureReadyInBackground( L"Condense_Light01.dds");
	TextureReadyInBackground( L"Explosion_Light_Ring01.dds");
	TextureReadyInBackground( L"Gate_Light_Arua.dds");
	TextureReadyInBackground( L"Gate_Light2.dds");
	TextureReadyInBackground( L"GuideArrow01.dds");
	TextureReadyInBackground( L"soket_earth_quake_smoke02.dds");
}

void CX2GUAra::ASI_A_ASD_Falling_Blossom_Start_StateStart()
{
	CommonStateStart();

	UpForcePower( 1 );
	m_usBeforeForceGainCount = m_usForceGainCount;
}

void CX2GUAra::ASI_A_ASD_Falling_Blossom_Start_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
		{
			StateChange( ASI_A_ASD_FALLING_BLOSSOM_FINISH );
		}
	}

	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

void CX2GUAra::ASI_A_ASD_Falling_Blossom_Finish_Init()
{
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Divide.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave_A.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Wave_B.X" );
	XSkinMeshReadyInBackground( L"SI_SA_ASD_SUPPRESSION_Sk_Dummy.X" );
	XSkinMeshReadyInBackground( L"Triple_W_PropertyA01.X" );
	XSkinMeshReadyInBackground( L"rena_A_evoke_Dummy.X" );
	TextureReadyInBackground( L"Arme_Ring2.dds");
	TextureReadyInBackground( L"Arme_Critical2.dds");
	TextureReadyInBackground( L"Arme_SpecialAttack1.dds");
	TextureReadyInBackground( L"ColorBallFire.dds");
	TextureReadyInBackground( L"ColorBallGray.dds");
	TextureReadyInBackground( L"Condense_Light01.dds");
	TextureReadyInBackground( L"Explosion_Light_Ring01.dds");
	TextureReadyInBackground( L"Gate_Light_Arua.dds");
	TextureReadyInBackground( L"Gate_Light2.dds");
	TextureReadyInBackground( L"GuideArrow01.dds");
	TextureReadyInBackground( L"soket_earth_quake_smoke02.dds");
}

void CX2GUAra::ASI_A_ASD_Falling_Blossom_Finish_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 낙화

#pragma region SI_A_ARA_QUICK_STAB
/** @function 	: ASI_A_ARA_Quick_Stab_Init
	@brief 		: 스킬 개편 - 신속
*/
void CX2GUAra::ASI_A_ARA_Quick_Stab_Init()
{
	XSkinMeshReadyInBackground( L"elsword_SA_SwordBlastiong_slash.X" );
	XSkinMeshReadyInBackground( L"aisha_SA_SCREWDRIVER_circle.X" );
	TextureReadyInBackground( L"elsword_SA_SwordBlastiong_slash.dds");
	TextureReadyInBackground( L"elsword_active_crecentCut_crecent.dds");
}

void CX2GUAra::ASI_A_ARA_Quick_Stab_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		UseForcePower( 2 );	
#else BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		UseForcePower( 3 );
		//이펙트 수정되며 스크립트에서 출력하도록 수정.
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Ara_ZpushX_Attack", this, NULL, false, GetPowerRate() );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}

	CommonFrameMove();
}

void CX2GUAra::ASI_A_ARA_Quick_Stab_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
#ifndef BALANCE_PATCH_20131107 //김창한
	// 발밑에 라인맵이 없으면
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}
#endif //BALANCE_PATCH_20131107
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 신속

#endif //UPGRADE_SKILL_SYSTEM_2013

#pragma endregion functions

#pragma region passive skill
#ifdef ARA_CHANGE_CLASS_FIRST
#pragma region QUICKEN_GUARD
/** @function 	: 
	@brief 		: 
*/
void CX2GUAra::ASI_P_ASD_QUICKEN_GUARD_StateStart()
{
	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_CSI_CIP_IRON_WILL", this );

	CommonStateStart();
}
/** @function 	: ASI_P_ASD_QUICKEN_GUARD_EventProcess
	@brief 		: 번득이는 기지 EventProcess
*/
void CX2GUAra::ASI_P_ASD_QUICKEN_GUARD_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
		{
			StateChange( USI_WAIT );
		}
	}
	
	CommonEventProcess();
}

/** @function 	: GetBlockingRate, GetTurnigRate
	@brief 		: 번득이는 기지 패시브 막기 비율 및 돌아맞기 비율 값 확인
*/
float CX2GUAra::GetBlockingRate()
{
	return m_fBlockingRate;
}

float CX2GUAra::GetTurnigRate()
{
	return m_fTurningRate;
}
#pragma endregion 번득이는 기지

#pragma region VIGOR_ENERGIZE
/** @function 	: GetReduceDebuffDurationRate
	@brief 		: 기력 활성화 디버프 시간 감소
*/
float CX2GUAra::GetReduceDebuffDurationRate(BUFF_TEMPLET_ID eBuffTempletID_)
{
	float fReduceDebuffDurationRate = 1.f;

#ifdef HAMEL_SECRET_DUNGEON // 김태환
	if ( NULL != g_pX2Game )
	{
		/// 해로운 효과라고 정의된 공식적인 디버프 리스트에 해당 버프가 포함되어 있다면, 버프 시간 감소
		BOOST_FOREACH( BUFF_TEMPLET_ID eBuffTempletID, g_pX2Game->GetOfficiallyDebuffList() )
		{
			if ( eBuffTempletID_ == eBuffTempletID )
				fReduceDebuffDurationRate = m_fReduceEDTDurationRate;
		}
	}
#else // HAMEL_SECRET_DUNGEON
	switch( eBuffTempletID_ )
	{
	case BTI_DEBUFF_FIRE:					/// 화상
	case BTI_DEBUFF_POISON:					/// 중독
	case BTI_DEBUFF_SHOCK:					/// 단죄 ( 쇼크 )
	case BTI_DEBUFF_STUN:					/// 스턴
	case BTI_DEBUFF_COLD:					/// 동상
	case BTI_DEBUFF_FROZEN:					/// 빙결
	case BTI_DEBUFF_WOUND:					/// 상처 (컷텐던)
	case BTI_DEBUFF_BLEEDING:				/// 출혈
	case BTI_DEBUFF_MOTION_SLOWDOWN:		/// 동작 둔화 (로우킥 따위)
	case BTI_DEBUFF_MOVE_JUMP_SLOWDOWN:		/// 이동 둔화
	case BTI_DEBUFF_BLIND:					/// 시야 방해 (명중률 감소, 샌드 스톰 따위)
	case BTI_DEBUFF_FLASH_BANG:				/// 섬광탄 (일렉트라 포톤 플레어)
	case BTI_DEBUFF_PRESS:					/// 압착
	case BTI_DEBUFF_BLIND_SMOKE:			/// 연무 (회피 감소)
	case BTI_DEBUFF_BIND:					/// 동작 정지
	case BTI_DEBUFF_CURSE:					/// 저주
	case BTI_DEBUFF_CONFUSION:				/// 혼란
	case BTI_DEBUFF_PANIC:					/// 공황 (판데모니움)
	case BTI_DEBUFF_ARMOR_BREAK:			/// 무기 파괴
	case BTI_DEBUFF_MAGIC_DEFFENCE_DROP:	/// 마력의 사슬
	case BTI_DEBUFF_WEAPON_BREAK:			/// 방어구 파괴
	case BTI_DEBUFF_ENTANGLE:				/// 인탱글
	case BTI_DEBUFF_LEG_SPASM:				/// 다리 부상
	case BTI_DEBUFF_DEATH_SENTENCE:			/// 죽음의 선고
	case BTI_DEBUFF_AGING:					/// 노화
	case BTI_DEBUFF_STIGMA:					/// 낙인의 사격
	case BTI_DEBUFF_MARK_OF_COMMANDER:		/// 지휘관의 표식
	case BTI_DEBUFF_WATER_HOLD:				/// 물의 결계
	case BTI_DEBUFF_SIDE_EFFECT_MEDICINE:	/// 대두 ( 물약 부작용 )
	case BTI_DEBUFF_SIDE_EFFECT_CREST:		/// 쇠약의 문장 ( 문장 부작용 )
	case BTI_DEBUFF_ANGER_OF_DRYAD:			/// 드라이어드의 분노
	case BTI_DEBUFF_SUPPRESSION:			/// 제압
		{
			fReduceDebuffDurationRate = m_fReduceEDTDurationRate;
		} break;
	default:
		break;
	}
#endif // HAMEL_SECRET_DUNGEON

	return fReduceDebuffDurationRate;
}
#pragma endregion 기력 활성화

#endif //ARA_CHANGE_CLASS_FIRST
#pragma endregion functions

#ifdef FINALITY_SKILL_SYSTEM //김창한
void CX2GUAra::ASI_HA_ASD_THOUSANDS_BLOSSOMS_Init()
{
	XSkinMeshReadyInBackground(L"ALH_MOONLIGHT_SLASH_BLADE_FULL_BASE.X");
	XSkinMeshReadyInBackground(L"ALH_MOONLIGHT_SLASH_BLADE_FULL_LIGHT.X");
	TextureReadyInBackground(L"ALH_MOONLIGHT_SLASH_MOON.tga");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"SI_SA_ALH_MOONLIGHT_SLASH_MOON.tga");
	TextureReadyInBackground(L"star2.dds");
	TextureReadyInBackground(L"Arme_Damage_Ring.dds");
	TextureReadyInBackground(L"Arme_Damage_Center.dds");
	TextureReadyInBackground(L"Arme_Critical.dds");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"ASD_1000_slash_01.tga");
	XSkinMeshReadyInBackground(L"elsword_SA_SwordBlastiong_slash.X");
}
void CX2GUAra::ASI_HA_ASD_THOUSANDS_BLOSSOMS_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

#pragma region 2-1, 2-2 Combo State

#pragma region ComboZZXX
/** @function	: ASI_COMBO_ALD_ZZXX_FrameMove
	@brief		: 기탄 데미지 이펙트에 대한 설정
*/
void CX2GUAra::ASI_COMBO_ALD_ZZXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimerOneshot( 0.24f ) == true ||
		m_pXSkinAnim->EventTimerOneshot( 0.39f ) == true ||
		m_pXSkinAnim->EventTimerOneshot( 0.67f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( ( m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true ) ||
		( m_pXSkinAnim->EventTimer( 0.39f ) == true && EventCheck( 0.349f, false ) == true ) ||
		( m_pXSkinAnim->EventTimer( 0.67f ) == true && EventCheck( 0.67f, false ) == true ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateEnergeBullet( -37.f );
	}

	CommonFrameMove();
}

/** @function	: ASI_COMBO_ALD_ZZXX_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_COMBO_ALD_ZZXX_EventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 101

#pragma region ComboZZZFrontZ
/** @function	: ASI_COMBO_ALD_ZZZFrontZ_FrameMove
	@brief		: 기탄 데미지 이펙트에 대한 설정
*/
void CX2GUAra::ASI_COMBO_ALD_ZZZFrontZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.34f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.34f ) == true && EventCheck( 0.34f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		CreateEnergeBullet();

	CommonFrameMove();
}

/** @function	: ASI_COMBO_ALD_ZZZFrontZ_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_COMBO_ALD_ZZZFrontZ_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 102

#pragma region ComboDashZX
/** @function	: ASI_DASH_JUMP_COMBO_ALD_ZX_FrameMove
	@brief		: 기탄 데미지 이펙트에 대한 설정
*/
void CX2GUAra::ASI_DASH_JUMP_COMBO_ALD_ZX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.27f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.27f ) == true && EventCheck( 0.27f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		CreateEnergeBullet( -45.f );

	CommonFrameMove();
}

/** @function	: ASI_DASH_JUMP_COMBO_ALD_ZX_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_DASH_JUMP_COMBO_ALD_ZX_EventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		m_InputData.oneX == true && ( CX2Unit::UC_ARA_LITTLE_DEVIL == GetUnitClass() || CX2Unit::UC_ARA_YAMA_RAJA == GetUnitClass() ), 
		ASI_DASH_JUMP_COMBO_ALD_ZXX )

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 103

#pragma region ComboDashZXX
/** @function	: ASI_DASH_JUMP_COMBO_ALD_ZXX_FrameMove
	@brief		: 기탄 데미지 이펙트에 대한 설정
*/
void CX2GUAra::ASI_DASH_JUMP_COMBO_ALD_ZXX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimerOneshot( 0.24f ) == true ||
		m_pXSkinAnim->EventTimerOneshot( 0.39f ) == true ||
		m_pXSkinAnim->EventTimerOneshot( 0.67f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( ( m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true ) ||
		( m_pXSkinAnim->EventTimer( 0.39f ) == true && EventCheck( 0.349f, false ) == true ) ||
		( m_pXSkinAnim->EventTimer( 0.67f ) == true && EventCheck( 0.67f, false ) == true ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateEnergeBullet( -45.f );
	}

	CommonFrameMove();
}

/** @function	: ASI_DASH_JUMP_COMBO_ALD_ZXX_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_DASH_JUMP_COMBO_ALD_ZXX_EventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 104

#pragma region ComboDashJumpXZ
/** @function	: ASI_DASH_JUMP_COMBO_AYR_XZ_StateStartFuture
	@brief		: 방향 전환 설정
*/
void CX2GUAra::ASI_DASH_JUMP_COMBO_AYR_XZ_StateStartFuture()
{
	CommonStateStartFuture();

	m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;
}

/** @function	: ASI_DASH_JUMP_COMBO_AYR_XZ_FrameMove
	@brief		: 기탄 데미지 이펙트에 대한 설정
*/
void CX2GUAra::ASI_DASH_JUMP_COMBO_AYR_XZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimerOneshot( 0.24f ) == true ||
		m_pXSkinAnim->EventTimerOneshot( 0.39f ) == true ||
		m_pXSkinAnim->EventTimerOneshot( 0.67f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( ( m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true ) ||
		( m_pXSkinAnim->EventTimer( 0.39f ) == true && EventCheck( 0.349f, false ) == true ) ||
		( m_pXSkinAnim->EventTimer( 0.67f ) == true && EventCheck( 0.67f, false ) == true ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateEnergeBullet( -45.f );
	}

	CommonFrameMove();
}

/** @function	: ASI_DASH_JUMP_COMBO_AYR_XZ_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_DASH_JUMP_COMBO_AYR_XZ_EventProcess()
{
	if ( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 105

#pragma region ComboZZX
/** @function	: ASI_DASH_COMBO_AYR_ZZ_FrameMove
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_DASH_COMBO_AYR_ZZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.186f ) == true ||
		m_pXSkinAnim->EventTimerOneshot( 0.326f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( ( m_pXSkinAnim->EventTimer( 0.186f ) == true && EventCheck( 0.186f, false ) == true ) ||
		( m_pXSkinAnim->EventTimer( 0.326f ) == true && EventCheck( 0.326f, false ) == true ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateEnergeBullet( 0.f, true );	/// 오른손에서 발사
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if ( m_pXSkinAnim->EventTimerOneshot( 0.263f) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if ( m_pXSkinAnim->EventTimer( 0.263f ) == true && EventCheck( 0.263f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateEnergeBullet( 0.f, false );	/// 왼손에서 발사
	}

	CommonFrameMove();
}

/** @function	: ASI_DASH_COMBO_AYR_ZZ_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_DASH_COMBO_AYR_ZZ_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
	}
	else if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 106

#pragma endregion 2-1, 2-2 커맨드 스킬


#pragma region 2-1, 2-2 Active

#pragma region SI_A_ALD_WOLF_CLAW
/** @function	: ASI_A_ALD_WOLF_CLAW_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_ALD_WOLF_CLAW_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ALD_WOLF_CLAW", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_WOLF_CLAW_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_WOLF_CLAW_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_WOLF_CLAW_EventProcess );
}

/** @function	: ASI_A_ALD_WOLF_CLAW_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_A_ALD_WOLF_CLAW_Init()
{
	TextureReadyInBackground(L"Ara_Orange_LEAF.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"Steam_BP.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"state_potion_flare.DDS");
	TextureReadyInBackground(L"stone.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"Ran_flame02.tga");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Impact05.dds");

	XSkinMeshReadyInBackground(L"A_ALD_WOLF_CLAW_START_TRACE.X");
	XSkinMeshReadyInBackground(L"A_ALD_WOLF_CLAW_START_Projecticle.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}

/** @function	: ASI_A_ALD_WOLF_CLAW_StateStart
	@brief		: 기력 전환 수행
*/
void CX2GUAra::ASI_A_ALD_WOLF_CLAW_StateStart()
{
	CommonStateStart();

	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function	: ASI_A_ALD_WOLF_CLAW_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_ALD_WOLF_CLAW_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
			StateChange( ASI_A_ALD_WOLF_CLAW_FINISH );
	}

	/// 기력 캔슬 검사
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

/** @function	: ASI_A_ALD_WOLF_CLAW_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_ALD_WOLF_CLAW_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ALD_WOLF_CLAW_FINISH", stateData_.stateID );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_WOLF_CLAW_FINISH_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_WOLF_CLAW_FINISH_EventProcess );
}

/** @function	: ASI_A_ALD_WOLF_CLAW_FINISH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_A_ALD_WOLF_CLAW_FINISH_Init()
{
	TextureReadyInBackground(L"Ara_Black_LEAF.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Cloud.DDS");
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"soket_earth_quake_stone01.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Ran_flame02.tga");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Impact05.dds");

	XSkinMeshReadyInBackground(L"A_ALD_WOLF_CLAW_END_TRACE.X");
	XSkinMeshReadyInBackground(L"A_ALD_WOLF_CLAW_END_Projecticle.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}

/** @function	: ASI_A_ALD_WOLF_CLAW_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_ALD_WOLF_CLAW_FINISH_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 낭아 3식 - 늑대발톱

#pragma region SI_A_ALD_CONVERSION_HEALTH
/** @function	: ASI_A_ALD_WOLF_CLAW_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_HEALTH_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ALD_CONVERSION_HEALTH", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_CONVERSION_HEALTH_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_CONVERSION_HEALTH_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_CONVERSION_HEALTH_EventProcess );
}

/** @function	: ASI_A_ALD_CONVERSION_HEALTH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_HEALTH_Init()
{
	TextureReadyInBackground(L"A_ALD_CONVERSION_HEALTH_MAP02.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"SpreadLight01.dds");
	TextureReadyInBackground(L"aisha_active_guillotinePress_circleParticle.dds");

	XSkinMeshReadyInBackground(L"Elsword_SI_A_RS_Sword_Enchant_Mesh01.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
}

/** @function	: ASI_A_ALD_CONVERSION_HEALTH_FrameMove
	@brief		: HP를 통해 기력 획득 처리를 하는 함수
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_HEALTH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.2f ) && true == EventCheck( 0.2f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != g_pData->GetSkillTree() &&
			 NULL != GetUnit() )
		{
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;					/// 유저 스킬 트리

			const int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ALD_CONVERSION_HEALTH );	/// 이혈공 스킬 레벨

			/// 이혈공 스킬 템플릿
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ALD_CONVERSION_HEALTH );

			if ( NULL != pSkillTemplet )
			{
				/// 추가될 기력량
				const int	iAddForcePower	= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillLevel ) );
				/// 소모될 HP 계산
				const float fHPRate			= GetMaxHp() * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_HP_REL_TO_MP_ABS_1, iSkillLevel );

				SetNowHp( GetNowHp() - fHPRate );		/// HP 감소 처리

				SetForcePower( GetForcePower() + iAddForcePower );	/// 기력 증가
			}
		}
	}

	CommonFrameMove();
}

/** @function	: ASI_A_ALD_CONVERSION_HEALTH_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_HEALTH_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 이혈공

#pragma region SI_A_ALD_CONVERSION_MANA
/** @function	: ASI_A_ALD_CONVERSION_MANA_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_MANA_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_ALD_CONVERSION_MANA", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_CONVERSION_MANA_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_CONVERSION_MANA_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_ALD_CONVERSION_MANA_EventProcess );
}

/** @function	: ASI_A_ALD_CONVERSION_MANA_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_MANA_Init()
{
	TextureReadyInBackground(L"CDC_AS_FIRE_LIGHT.tga");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"Smoke.dds");
	TextureReadyInBackground(L"SpreadLight01.dds");
	TextureReadyInBackground(L"aisha_active_guillotinePress_circleParticle.dds");

	XSkinMeshReadyInBackground(L"Elsword_SI_A_RS_Sword_Enchant_Mesh01.X");
	XSkinMeshReadyInBackground(L"Mega_Slash_Trace_Dummy.x");
}

/** @function	: ASI_A_ALD_CONVERSION_MANA_FrameMove
	@brief		: MP를 통해 기력 획득 처리를 하는 함수
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_MANA_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.2f ) && true == EventCheck( 0.2f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != g_pData->GetSkillTree() &&
			 NULL != GetUnit() )
		{
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;					/// 유저 스킬 트리

			const int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ALD_CONVERSION_MANA );	/// 이혈공 스킬 레벨

			/// 이기공 스킬 템플릿
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ALD_CONVERSION_MANA );

			if ( NULL != pSkillTemplet )
			{
				/// 추가될 기력량
				const int	iAddForcePower	= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillLevel ) );
				/// 소모될 MP 계산
				const float fMPRate = GetMaxMp() * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillLevel );

				SetNowMp( GetNowMp() - fMPRate );		/// MP 감소 처리

				SetForcePower( GetForcePower() + iAddForcePower );	/// 기력 증가
			}
		}
	}

	CommonFrameMove();
}

/** @function	: ASI_A_ALD_CONVERSION_MANA_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_ALD_CONVERSION_MANA_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 이기공

#pragma region SI_A_AYR_HIGH_SPEED
/** @function	: ASI_A_AYR_HIGH_SPEED_READY_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_AYR_HIGH_SPEED_READY_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID = iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_AYR_HIGH_SPEED_READY", stateData_.stateID );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_HIGH_SPEED_READY_EventProcess );
}

/** @function	: ASI_A_AYR_HIGH_SPEED_READY_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_AYR_HIGH_SPEED_READY_EventProcess()
{
	if ( 0.1f <= m_FrameDataFuture.unitCondition.fStateTime )		/// 모션 끝날때 까지 조작이 없다면, 보고있는 방향으로 진행
	{
			StateChange( ASI_A_AYR_HIGH_SPEED );
	}
	else if ( m_InputData.oneRight)		/// 오른쪽을 눌렀을 때
	{
		m_FrameDataFuture.syncData.bIsRight	= true;		/// 방향 설정

		if ( false ==GetIsRight() )		/// 만약 반대 방향을 보고 있었다면, 180도 회전
			m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;

		StateChange( ASI_A_AYR_HIGH_SPEED );
	}
	else if ( m_InputData.oneLeft)		/// 왼쪽을 눌렀을 때
	{
		m_FrameDataFuture.syncData.bIsRight	= false;	/// 방향 설정

		if ( true ==GetIsRight() )		/// 만약 반대 방향을 보고 있었다면, 180도 회전
			m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;

		StateChange( ASI_A_AYR_HIGH_SPEED );
	}

	CommonEventProcess();
}

/** @function	: ASI_A_AYR_HIGH_SPEED_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_AYR_HIGH_SPEED_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_AYR_HIGH_SPEED", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_HIGH_SPEED_Init );
	stateData_.StateStartFuture	= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_HIGH_SPEED_StateStartFuture );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_HIGH_SPEED_EventProcess );
	stateData_.StateEndFuture	= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_HIGH_SPEED_StateEndFuture );
}

/** @function	: ASI_A_AYR_HIGH_SPEED_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_A_AYR_HIGH_SPEED_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
}

/** @function	: SI_A_AYR_HIGH_SPEED_READY_StateStartFuture
	@brief		: 진행 속도 연산
*/
void CX2GUAra::ASI_A_AYR_HIGH_SPEED_StateStartFuture()
{
	CommonStateStartFuture();

	if ( NULL != g_pData &&
		 NULL != g_pData->GetSkillTree() &&
		 NULL != GetUnit() )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_AYR_HIGH_SPEED );

		if ( NULL != pSkillTemplet )
		{
			const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AYR_HIGH_SPEED );

			/// 속도 증가값 설정
			m_fHighSpeedValue = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REMOTE_SPEED_ABS, iSkillLevel );

			if ( 0 >= m_fHighSpeedValue )
				m_fHighSpeedValue = 1.f;

			m_PhysicParam.passiveSpeed.x *= m_fHighSpeedValue;
		}
	}
}

/** @function	: ASI_A_AYR_HIGH_SPEED_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_AYR_HIGH_SPEED_EventProcess()
{
	if ( m_fEventTime[0].keyInputStart  <= m_FrameDataFuture.unitCondition.fStateTime )
	{
		if ( false == IsOnSomethingFuture() )
			StateChange( ASI_AIRIAL_DASH );
		else
			StateChange( USI_DASH );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/** @function	: ASI_A_AYR_HIGH_SPEED_StateEndFuture
	@brief		: 가속 상태 정지
*/
void CX2GUAra::ASI_A_AYR_HIGH_SPEED_StateEndFuture()
{
	CommonStateEndFuture();

	m_PhysicParam.passiveSpeed.x	= 0.f;
	m_PhysicParam.nowSpeed.x		= 1200.f;
}
#pragma endregion 쾌속

#pragma endregion 2-1, 2-2 액티브 스킬


#pragma region 2-1, 2-2 Special Active

#pragma region SI_SA_ALD_ENERGY_WAVE
/** @function	: ASI_SA_ALD_ENERGY_WAVE_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_WAVE_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_ENERGY_WAVE", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_WAVE_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_WAVE_EventProcess );
}

/** @function	: ASI_SA_ALD_ENERGY_WAVE_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_WAVE_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP03.dds");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP04.dds");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP06.dds");
	TextureReadyInBackground(L"SA_SPK_SWORDFIRE_WAVE.dds");
	TextureReadyInBackground(L"SA_SPK_SWORDFIRE_WAVE2.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Hall.dds");
	TextureReadyInBackground(L"Steam_Bp.dds");
	TextureReadyInBackground(L"Lire_Kick_Impact01.dds");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Cloud.DDS");
	TextureReadyInBackground(L"SpreadLight01.dds");

	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"SA_AYR_ORB_KI_BALL.X");
}

/** @function	: ASI_SA_ALD_ENERGY_WAVE_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_WAVE_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;

		StateChange( USI_JUMP_DOWN );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}
#pragma endregion 기공파

#pragma region SI_SA_ALD_HELL_OF_TORNADO
/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_HELL_OF_TORNADO_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_HELL_OF_TORNADO", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_HELL_OF_TORNADO_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_HELL_OF_TORNADO_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_HELL_OF_TORNADO_EventProcess );
}

/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_StateStart
	@brief		: 기력 전환 수행
*/
void CX2GUAra::ASI_SA_ALD_HELL_OF_TORNADO_StateStart()
{
	CommonStateStart();

	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_HELL_OF_TORNADO_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Lire_Kick_Impact01.dds");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Cloud.DDS");
	TextureReadyInBackground(L"SpreadLight01.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP03.dds");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP04.dds");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP06.dds");
	TextureReadyInBackground(L"SA_SPK_SWORDFIRE_WAVE.dds");
	TextureReadyInBackground(L"SA_SPK_SWORDFIRE_WAVE2.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Hall.dds");
	TextureReadyInBackground(L"Steam_Bp.dds");

	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"SA_AYR_ORB_KI_BALL.X");
}

/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_HELL_OF_TORNADO_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
			StateChange( ASI_SA_ALD_HELL_OF_TORNADO_FINISH );
	}

	/// 기력 캔슬 검사
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}

/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_FINISH_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_HELL_OF_TORNADO_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_HELL_OF_TORNADO_FINISH", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_HELL_OF_TORNADO_FINISH_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_HELL_OF_TORNADO_FINISH_EventProcess );
}

/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_FINISH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_HELL_OF_TORNADO_FINISH_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Lire_Kick_Impact01.dds");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Cloud.DDS");
	TextureReadyInBackground(L"SpreadLight01.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP03.dds");
	TextureReadyInBackground(L"SA_AHD_ENERGY_WAVE_PROJECTILE_MAP04.dds");
	TextureReadyInBackground(L"SA_SPK_SWORDFIRE_WAVE2.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Hall.dds");

	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"SA_AYR_ORB_KI_BALL.X");
}

/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_FINISH_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_HELL_OF_TORNADO_FINISH_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
			StateChange( USI_JUMP_DOWN );
		}
		else
			StateChange( USI_WAIT );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 연환지옥

#pragma region SI_SA_ALD_WOLF_SPEAR
/** @function	: ASI_SA_ALD_WOLF_SPEAR_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_WOLF_SPEAR_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_WOLF_SPEAR", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_SPEAR_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_SPEAR_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_SPEAR_EventProcess );
}

/** @function	: ASI_SA_ALD_WOLF_SPEAR_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_WOLF_SPEAR_Init()
{
	XSkinMeshReadyInBackground(L"ALD_SPEAR_Weapon.X");
}

/** @function	: ASI_SA_ALD_WOLF_SPEAR_StateStart
	@brief		: 기력 전환 수행
*/
void CX2GUAra::ASI_SA_ALD_WOLF_SPEAR_StateStart()
{
	CommonStateStart();

	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function	: ASI_SA_ALD_WOLF_SPEAR_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_WOLF_SPEAR_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
			StateChange( ASI_SA_ALD_WOLF_SPEAR_FINISH );
	}

	/// 기력 캔슬 검사
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	CommonEventProcess();
}

/** @function	: ASI_SA_ALD_WOLF_SPEAR_FINISH_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_WOLF_SPEAR_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_WOLF_SPEAR_FINISH", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_SPEAR_FINISH_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_SPEAR_FINISH_EventProcess );
}

/** @function	: ASI_SA_ALD_WOLF_SPEAR_FINISH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_WOLF_SPEAR_FINISH_Init()
{
	XSkinMeshReadyInBackground(L"ALD_SPEAR_Weapon.X");

}
/** @function	: ASI_SA_ALD_HELL_OF_TORNADO_FINISH_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_WOLF_SPEAR_FINISH_EventProcess()
{
	CommonEventProcess();

	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
			StateChange( USI_JUMP_DOWN );
		}
		else
			StateChange( USI_WAIT );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
}
#pragma endregion 낭아 4식 - 늑대이빨

#pragma region SI_SA_ALD_WOLF_SPEAR
/** @function	: ASI_SA_ALD_FINGER_BULLET_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_FINGER_BULLET", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_FINGER_BULLET_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_FINGER_BULLET_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_FINGER_BULLET_EventProcess );
}

/** @function	: ASI_SA_ALD_FINGER_BULLET_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_Init()
{
	TextureReadyInBackground(L"Force_Spear_round.dds");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SI_A_ARA_GAIN_FORCE_NASUN_GATE_DEST.dds");
	TextureReadyInBackground(L"Whitecircle02.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Hall.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Impact05.dds");

	XSkinMeshReadyInBackground(L"Bust_Wolf_M03.X");
	XSkinMeshReadyInBackground(L"SA_ALD_FINGER_BULLET_Attack_Box_START.X");
}

/** @function	: ASI_SA_ALD_FINGER_BULLET_Init
	@brief		: 흡혼에 의해 타격된 유닛 정보 초기화
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_StateStart()
{
	CommonStateStart();

	/// 흡공에 의해 타격된 대상 저장 객체 초기화
	m_vecHitUnitPointer.clear();

	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function	: ASI_SA_ALD_FINGER_BULLET_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
			StateChange( ASI_SA_ALD_FINGER_BULLET_FINISH );
	}

	/// 기력 캔슬 검사
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/** @function	: ASI_SA_ALD_FINGER_BULLET_FINISH_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ )

{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_FINGER_BULLET_FINISH", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_FINGER_BULLET_FINISH_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_FINGER_BULLET_FINISH_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_FINGER_BULLET_FINISH_EventProcess );
}

/** @function	: ASI_SA_ALD_FINGER_BULLET_FINISH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_FINISH_Init()
{
	XSkinMeshReadyInBackground(L"SA_ALD_FINGER_BULLET_Attack_Box_END.X");
}

/** @function	: ASI_SA_ALD_FINGER_BULLET_FINISH_FrameMove
	@brief		: 흡혼에 피격된 적을 넘어뜨리는 동작 수행
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_FINISH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( m_fEventTime[0].keyInputStart ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( m_fEventTime[0].keyInputStart ) && true == EventCheck( m_fEventTime[0].keyInputStart, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		BOOST_FOREACH( CX2GameUnitoPtr pGameUnit, m_vecHitUnitPointer )
		{
			/// 죽지 않은 유닛일 때
			if ( null != pGameUnit && 0 < pGameUnit->GetNowHp() )
			{
				/// 충돌 처리를 제외하기 때문에, 다운 공격으로 강제 설정
				m_DamageData.reActResult = CX2DamageManager::RT_DOWN;

				/// 디펜더 유닛 추가
				m_DamageData.optrDefenderGameUnit = pGameUnit;

				/// 강제로 데미지 설정
				pGameUnit->DamageReact( &m_DamageData );

				/// 이동 불가 버프 제거
				EraseBuffTempletFromGameUnit( BTI_DEBUFF_NEVER_MOVE );
			}
		}

		m_vecHitUnitPointer.clear();
	}

	CommonFrameMove();
}

/** @function	: ASI_SA_ALD_FINGER_BULLET_FINISH_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_FINGER_BULLET_FINISH_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 나찰 1식 - 흡혼

#pragma region SI_SA_ALD_WOLF_SPEAR
/** @function	: ASI_SA_ALD_ENERGY_BEAD_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_BEAD_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_ENERGY_BEAD", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_BEAD_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_BEAD_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_BEAD_EventProcess );
}

/** @function	: ASI_SA_ALD_ENERGY_BEAD_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_BEAD_Init()
{
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	TextureReadyInBackground(L"secretVelder_Unohound_fire.dds");

	XSkinMeshReadyInBackground(L"SA_AYR_ORB_KI_BALL.X");
}


/** @function	: ASI_SA_ALD_ENERGY_BEAD_StateStart
	@brief		: 기력 전환 수행
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_BEAD_StateStart()
{
	CommonStateStart();

	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function	: ASI_SA_ALD_ENERGY_BEAD_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_BEAD_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
			StateChange( ASI_SA_ALD_ENERGY_BEAD_FINISH );
	}

	/// 기력 캔슬 검사
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}

/** @function	: ASI_SA_ALD_ENERGY_BEAD_FINISH_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_BEAD_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_ENERGY_BEAD_FINISH", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_BEAD_FINISH_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_BEAD_FINISH_EventProcess );
}

/** @function	: ASI_SA_ALD_ENERGY_BEAD_FINISH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_BEAD_FINISH_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Cloud.DDS");
	TextureReadyInBackground(L"secretVelder_Unohound_fire.dds");

	XSkinMeshReadyInBackground(L"SA_AYR_ORB_KI_BALL.X");
}

/** @function	: ASI_SA_ALD_ENERGY_BEAD_FINISH_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_ENERGY_BEAD_FINISH_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 나찰 2식 - 연환장

#pragma region SI_SA_ALD_PRISON_SPEAR
/** @function	: ASI_SA_ALD_PRISON_SPEAR_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_PRISON_SPEAR_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_PRISON_SPEAR", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_PRISON_SPEAR_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_PRISON_SPEAR_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_PRISON_SPEAR_EventProcess );
}

/** @function	: ASI_SA_ALD_PRISON_SPEAR_StateStart
	@brief		: 기력 전환 수행
*/
void CX2GUAra::ASI_SA_ALD_PRISON_SPEAR_StateStart()
{
	CommonStateStart();

	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function	: ASI_SA_ALD_PRISON_SPEAR_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_PRISON_SPEAR_Init()
{
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_KNOT_Trace.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Cloud.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Hall.dds");
	TextureReadyInBackground(L"secretVelder_alchemystBoss_magicAttackB_thunder.DDS");

	XSkinMeshReadyInBackground(L"aisha_active_energySpurt_circle.X");
	XSkinMeshReadyInBackground(L"ALD_SPEAR_Weapon.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}

/** @function	: ASI_SA_ALD_PRISON_SPEAR_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_PRISON_SPEAR_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
			StateChange( ASI_SA_ALD_PRISON_SPEAR_FINISH );
	}

	/// 기력 캔슬 검사
	if( false == m_bFPCancel && m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime()
		&& m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() && true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
}

/** @function	: ASI_SA_ALD_PRISON_SPEAR_FINISH_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_PRISON_SPEAR_FINISH_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_PRISON_SPEAR_FINISH", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_PRISON_SPEAR_FINISH_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_ENERGY_BEAD_FINISH_EventProcess );
}

/** @function	: ASI_SA_ALD_PRISON_SPEAR_FINISH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_PRISON_SPEAR_FINISH_Init()
{
	XSkinMeshReadyInBackground(L"ALD_SPEAR_Weapon.X");
	XSkinMeshReadyInBackground(L"aisha_active_energySpurt_circle.X");

	TextureReadyInBackground(L"secretVelder_alchemystBoss_magicAttackB_thunder.DDS");
}

/** @function	: ASI_SA_ALD_PRISON_SPEAR_FINISH_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_PRISON_SPEAR_FINISH_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 나찰 3식 - 창살감옥

#pragma region SI_A_AYR_STEAL_SOUL
/** @function	: ASI_A_AYR_STEAL_SOUL_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_CreateState( OUT UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_AYR_STEAL_SOUL", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_STEAL_SOUL_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_STEAL_SOUL_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_STEAL_SOUL_EventProcess );
}

/** @function	: ASI_A_AYR_STEAL_SOUL_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_Init()
{
	TextureReadyInBackground(L"Ara_Black_LEAF.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	XSkinMeshReadyInBackground(L"Effect_ElSword_MegaSlashBackDust.X");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	TextureReadyInBackground(L"Whitecircle03.dds");
}

/** @function	: ASI_A_AYR_STEAL_SOUL_StateStart
	@brief		: 탈혼에 의해 타격된 유닛 정보 초기화
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_StateStart()
{
	CommonStateStart();

	/// 흡공에 의해 타격된 대상 저장 객체 초기화
	m_vecHitUnitPointer.clear();

	m_usBeforeForceGainCount = m_usForceGainCount;
}

/** @function	: ASI_A_AYR_STEAL_SOUL_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == GetAvailableFPCancel ( m_eSkillID ) )	/// 기력 캔슬 동작
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if ( true == m_bFPCancel && CX2SkillTree::SI_NONE != m_eSkillID )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		{
			UpForcePower( m_usBeforeForceGainCount );
			StateChangeSpecialAttack( m_eSkillID );
		}
		else
			StateChange( ASI_A_AYR_STEAL_SOUL_FINISH );
	}
	/// 기력 캔슬 검사
	else  if( false == m_bFPCancel && 
			m_vFPCancel.x <= m_pXSkinAnimFuture->GetNowAnimationTime() &&
			m_vFPCancel.y > m_pXSkinAnimFuture->GetNowAnimationTime() &&
			true == CanUseSpecialAttack() )
	{
		m_bFPCancel = true;
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

/** @function	: ASI_A_AYR_STEAL_SOUL_FINISH_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_FINISH_CreateState( OUT UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_A_AYR_STEAL_SOUL_FINISH", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_STEAL_SOUL_FINISH_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_STEAL_SOUL_FINISH_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_A_AYR_STEAL_SOUL_FINISH_EventProcess );
}

/** @function	: ASI_A_AYR_STEAL_SOUL_FINISH_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_FINISH_Init()
{
	XSkinMeshReadyInBackground(L"A_AYR_STEAL_SOUL_SHADOW_E.X");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"Ran_flame02.tga");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"elswrod_demolition_aura.dds");
}

/** @function	: ASI_A_AYR_STEAL_SOUL_FINISH_FrameMove
	@brief		: 탈혼에 피격된 적을 넘어뜨리는 동작 수행
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_FINISH_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.577f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.577f ) && true == EventCheck( 0.577f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		BOOST_FOREACH( CX2GameUnitoPtr pGameUnit, m_vecHitUnitPointer )
		{
			/// 죽지 않은 유닛일 때
			if ( null != pGameUnit && 0 < pGameUnit->GetNowHp() )
			{
				/// 충돌 처리를 제외하기 때문에, 다운 공격으로 강제 설정
				m_DamageData.reActResult = CX2DamageManager::RT_DOWN;

				/// 디펜더 유닛 추가
				m_DamageData.optrDefenderGameUnit = pGameUnit;

				/// 강제로 데미지 설정
				pGameUnit->DamageReact( &m_DamageData );
			}
		}

		m_vecHitUnitPointer.clear();
	}

	CommonFrameMove();
}

/** @function	: SI_A_AYR_STEAL_SOU_FINISHL_FINISH_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_A_AYR_STEAL_SOUL_FINISH_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 나찰 4식 - 탈혼

#pragma region SI_SA_AYR_ENERGY_VOID
/** @function	: ASI_SA_AYR_ENERGY_VOID_START_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_START_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_AYR_ENERGY_VOID_START", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_START_Init );
	stateData_.StateStartFuture	= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_START_StateStartFuture );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_START_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_START_EventProcess );
}

/** @function	: ASI_SA_AYR_ENERGY_VOID_START_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_START_Init()
{
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SpreadLight01.dds");
	TextureReadyInBackground(L"aisha_active_guillotinePress_circleParticle.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
}

/** @function	: ASI_SA_AYR_ENERGY_VOID_START_StateStartFuture
	@brief		: 키 입력 제어 객체 초기화
*/

void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_START_StateStartFuture()
{
	/// 키 입력 제어 객체 초기화
	m_sCurrentSkillKeyManager.InitCurrentSkillKeyManager( m_iNowSpecialAttack - 1 );

	CommonStateStartFuture();
}

/** @function	: ASI_SA_AYR_ENERGY_VOID_START_StateStart
	@brief		: 초진공장 사용 준비 상태의 데미지 이펙트 제거 및 기력 소모, 차지 시간 설정 등 초기화 작업
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_START_StateStart()
{
	CommonStateStart();
	m_iEnergeVoidChargeValue	= 0;			/// 차지 단계 초기화
	m_bEnergeVoidChargeEnd		= false;		/// 차지 종료 여부 초기화
}

/** @function	: ASI_SA_AYR_ENERGY_VOID_START_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_START_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
		StateChange( ASI_SA_AYR_ENERGY_VOID_LOOP );
	
	CommonEventProcess();
}

/** @function	: ASI_SA_AYR_ENERGY_VOID_LOOP_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_LOOP_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_AYR_ENERGY_VOID_LOOP", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_LOOP_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_LOOP_StateStart );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_LOOP_EventProcess );
}

/** @function	: SI_SA_AYR_ENERGY_VOID_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_LOOP_Init()
{
	TextureReadyInBackground(L"Break_Impact01.dds");
	TextureReadyInBackground(L"SI_A_ARA_GAIN_FORCE_NASUN_GATE.dds");
	TextureReadyInBackground(L"SI_A_ARA_GAIN_FORCE_NASUN_GATE_DEST.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Cloud.dds");
	TextureReadyInBackground(L"SpreadLight02.dds");
	TextureReadyInBackground(L"ThunderBall02.dds");
	TextureReadyInBackground(L"ThunderBall03.dds");
	TextureReadyInBackground(L"aisha_SA_MorningStar_circle.dds");


	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}

/** @function	: SI_SA_AYR_ENERGY_VOID_FrameMove
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_LOOP_StateStart()
{
	/// 차지 4단계를 초과 하면서 사용할 수 있는 기력량이 부족할 때
	if ( 3 < m_iEnergeVoidChargeValue &&
		0 >= GetForcePower() )
	{
		m_bEnergeVoidChargeEnd = true;		/// 종료 설정

		return;
	}

	/// 이펙트 생성 처리
	if ( NULL != g_pX2Game->GetDamageEffect() )
	{
		wstring wstrEffectName = L"";		/// 보여주기용 데미지 이펙트 이름

		/// 각 단계에 따른 보여주기용 데미지 이펙트 이름 설정
		switch ( m_iEnergeVoidChargeValue )
		{
		case 0: wstrEffectName = L"DamageEffect_AYR_Energy_Void_Effect_1st"; break;		/// 1단계 이펙트 생성
		case 4: wstrEffectName = L"DamageEffect_AYR_Energy_Void_Effect_2nd"; break;		/// 2단계 이펙트 생성
		case 7: wstrEffectName = L"DamageEffect_AYR_Energy_Void_Effect_3rd"; break;		/// 3단계 이펙트 생성
		}

		/// 생성 위치 설정
		D3DXVECTOR3 vPos = GetIsRight() ? GetPos() + ( GetDirVector() * 500.f ) : GetPos() + ( GetDirVector() * -500.f );

		vPos.y += 100.f;

		/// 보여주기용 데미지 이펙트 생성
		if ( false == wstrEffectName.empty() )
		{
			/// 만약 살아있는 이펙트가 있다면, 죽임
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_hEnergeVoidDamageEffect != INVALID_DAMAGE_EFFECT_HANDLE )
                g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hEnergeVoidDamageEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if ( NULL != m_pEnergeVoidDamageEffect && true == g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pEnergeVoidDamageEffect ) )
				g_pX2Game->GetDamageEffect()->DestroyInstance( m_pEnergeVoidDamageEffect );
            m_pEnergeVoidDamageEffect = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			/// 데미지 이펙트 생성 ( 보여주기용 )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hEnergeVoidDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pEnergeVoidDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                this, wstrEffectName.c_str(), 
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(),
				m_FrameDataNow.unitCondition.landPosition.y );
		}

		/// 실제 데미지 ( 연속되는 타격을 위해, 차지 한번 한 만큼 살아있는 데미지 이펙트 생성 )
		if ( 3 < m_iEnergeVoidChargeValue )		/// 기력을 소비한 추가 타격
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_AYR_Energy_Void_Attack", 
				GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(),
				m_FrameDataNow.unitCondition.landPosition.y );

			UseForcePower( 1 );	/// 기력 소비
		}
		else									/// MP를 소비한 기본 타격
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_AYR_Energy_Void_Default_Attack", 
			GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(),
			m_FrameDataNow.unitCondition.landPosition.y );
	}

	++ m_iEnergeVoidChargeValue;		/// 차지값 갱신

	CommonStateStart();
}

/** @function	: SI_SA_AYR_ENERGY_VOID_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_LOOP_EventProcess()
{
	/// 키 입력 제어 객체에서 슬롯이 변경 되어 있는지 검사
	m_sCurrentSkillKeyManager.CheckSkillSlotSwap();

	/// 모션이 반복 시점을 넘었을 때
	if ( 0.4f <= m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		/// 차지 4단계 이상이고 키 입력을 해제 하였거나, 최대 반복 횟수에 도달되었다면 스테이트로 전환
		if ( ( 3 < m_iEnergeVoidChargeValue &&
			   true == m_sCurrentSkillKeyManager.IsUnpressedCurrentSkillKey( m_InputData ) ) || 
			 _CONST_ARA_::ENERGY_VOID_MAX_CHARGE_VALUE <= m_iEnergeVoidChargeValue )
		{
			StateChange( ASI_SA_AYR_ENERGY_VOID_END );
		}
		else
		{
			StateChange( ASI_SA_AYR_ENERGY_VOID_LOOP );
		}
	}
	/// 종료 조건 충족되면, 마무리 스테이트로 전환
	else if ( true == m_bEnergeVoidChargeEnd )
	{
		StateChange( ASI_SA_AYR_ENERGY_VOID_END );
	}

	CommonEventProcess();
}



/** @function	: SI_SA_AYR_ENERGY_VOID_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_END_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_AYR_ENERGY_VOID_END", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_END_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_END_StateStart );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_END_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_ENERGY_VOID_END_EventProcess );
}

/** @function	: ASI_SA_AYR_ENERGY_VOID_END_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_END_Init()
{
	TextureReadyInBackground(L"ASD_1000_slash_01.tga");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SA_AYR_ENERGY_CANNON_Map01.dds");
	TextureReadyInBackground(L"SpreadLight01.dds");

	XSkinMeshReadyInBackground(L"Bust_Wolf_M03.X");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}
/** @function	: ASI_SA_AYR_ENERGY_VOID_END_StateStart
	@brief		: 살아있는 차지 이펙트가 있다면, 죽임
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_END_StateStart()
{
	CommonStateStart();

	/// 만약 살아있는 이펙트가 있다면, 죽임
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hEnergeVoidDamageEffect != INVALID_DAMAGE_EFFECT_HANDLE )
        g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hEnergeVoidDamageEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if ( NULL != m_pEnergeVoidDamageEffect && true == g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pEnergeVoidDamageEffect ) )
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_pEnergeVoidDamageEffect );
    m_pEnergeVoidDamageEffect = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}
/** @function	: ASI_SA_AYR_ENERGY_VOID_END_FrameMove
	@brief		: 폭발 이펙트 생성
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
if( true == m_pXSkinAnim->EventTimerOneshot( 0.08f ) &&
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.08f ) && true == EventCheck( 0.08f, false ) && 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		NULL != g_pData->GetSkillTree() )
	{
		/// 초진공장 스킬 템플릿 반환
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_SA_AYR_ENERGY_VOID );

		if ( NULL != pSkillTemplet && 
			 NULL != g_pX2Game->GetEffectSet() &&
			 NULL != GetUnit() )
		{
			const CX2UserSkillTree&		userSkillTree	= GetUnit()->GetUnitData().m_UserSkillTree;		/// 유저 스킬 트리
			const int					iSkillLevel		= userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_AYR_ENERGY_VOID );	/// 스킬 레벨

			/// 기본 데미지 배율에 차지 단계만큼 곱해준다. ( 차지 단계 * 차지 단계별 데미지 배율 )
			const float fChargePowerRate = max( m_iEnergeVoidChargeValue * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGED_REL, iSkillLevel ), 1.f );

			/// 폭발 이펙트 생성
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI_SA_AHD_ENERGY_VOID_FX_Hit", this, NULL, IsHyperState(),
				GetPowerRate() * fChargePowerRate );
		}
	}
	
	CommonFrameMove();
}

/** @function	: ASI_SA_AYR_ENERGY_VOID_END_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_AYR_ENERGY_VOID_END_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

#pragma endregion 초진공장

#pragma region SI_SA_AYR_SUPPRESSION_ENERGY
/** @function	: ASI_SA_AYR_SUPPRESSION_ENERGY_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_AYR_SUPPRESSION_ENERGY_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_AYR_SUPPRESSION_ENERGY", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_SUPPRESSION_ENERGY_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_SUPPRESSION_ENERGY_EventProcess );
}

/** @function	: ASI_SA_AYR_SUPPRESSION_ENERGY_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_AYR_SUPPRESSION_ENERGY_Init()
{
	TextureReadyInBackground(L"ASD_1000_slash_01.tga");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Force_Spear_round.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"Light_Impact01.dds");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"PET_GOLDFOX_MIHOEffect02.tga");
	TextureReadyInBackground(L"RVC_SA_DeadlyRaid_line.dds");
	TextureReadyInBackground(L"SA_AHD_SUPPRESSION_ENERGY_P01.dds");
	TextureReadyInBackground(L"SA_AHD_SUPPRESSION_ENERGY_P08.tga");
	TextureReadyInBackground(L"SA_AYR_ENERGY_CANNON_Map03.dds");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Hall.dds");
	TextureReadyInBackground(L"SpreadLight01.dds");
	TextureReadyInBackground(L"secretVelder_alchemystBoss_magicAttackE_wave.dds");

	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
}

/** @function	: ASI_SA_AYR_SUPPRESSION_ENERGY_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_AYR_SUPPRESSION_ENERGY_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 제압 - 기

#pragma region SI_SA_ALD_WOLF_BREAK
/** @function	: ASI_SA_ALD_WOLF_BREAK_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_ALD_WOLF_BREAK_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_ALD_WOLF_BREAK", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_BREAK_Init );
	stateData_.StateStart		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_BREAK_StateStart );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_BREAK_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_ALD_WOLF_BREAK_EventProcess );
}

/** @function	: ASI_SA_ALD_WOLF_BREAK_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_ALD_WOLF_BREAK_Init()
{
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"SI_SA_ARA_SHADOW_RISING_Hall.dds");
	TextureReadyInBackground(L"WhitePoint.dds");

	XSkinMeshReadyInBackground(L"Ald_Wolf_Break_Dummy.X");
}

/** @function	: ASI_SA_ALD_WOLF_BREAK_StateStart
	@brief		: 발사체 생성 카운트 초기화
*/
void CX2GUAra::ASI_SA_ALD_WOLF_BREAK_StateStart()
{
	CommonStateStart();

	m_iWolfBreakeCount = 0;
}

/** @function	: ASI_SA_ALD_WOLF_BREAK_FrameMove
	@brief		: 발사체 생성
*/
void CX2GUAra::ASI_SA_ALD_WOLF_BREAK_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimerOneshot( 1.1f ) ||
		 m_pXSkinAnim->EventTimerOneshot( 1.35f ) ||
		 m_pXSkinAnim->EventTimerOneshot( 1.63f ) ||
		 m_pXSkinAnim->EventTimerOneshot( 2.f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( ( true == m_pXSkinAnim->EventTimer( 1.1f ) && true == EventCheck( 1.1f, false ) ) ||
		 ( true == m_pXSkinAnim->EventTimer( 1.35f ) && true == EventCheck( 1.35f, false ) ) ||
		 ( true == m_pXSkinAnim->EventTimer( 1.63f ) && true == EventCheck( 1.63f, false ) ) ||
		 ( true == m_pXSkinAnim->EventTimer( 2.f ) && true == EventCheck( 2.f, false ) ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		wstring wstrEffectName = L"DamageEffect_SA_ALD_WOLF_Break";		/// 발사체 이름 설정
		float fPowerRate = 1.0f;

		/// 발사 횟수에 따른 발사체 이름 설정
		switch ( m_iWolfBreakeCount )
		{
		case 1:
			{
				wstrEffectName = L"DamageEffect_SA_ALD_WOLF_Break2";
			} break;
		case 3:
			{
				wstrEffectName = L"DamageEffect_SA_ALD_WOLF_Break3";
			} break;
		}

		/// 만약에 패시브 늑대의 울부짖음이 적용중이라면, 특정 확률로 커다란 늑대를 발사한다.
		if ( m_fHowlingActiveRate > GetRandomFloat() )
		{
			wstrEffectName += L"_Special";
			fPowerRate = m_fHowlingDamageRel;	/// 커다란 늑대에만 패시브에 의한 데미지 증가
		}

		/// 이펙트 생성
		if ( false == wstrEffectName.empty() )
		{
			/// 생성 위치 설정
			D3DXVECTOR3 vPos = GetIsRight() ? GetBonePos( L"Bip01" ) + ( GetDirVector() * 100.f ) : 
											  GetBonePos( L"Bip01" ) + ( GetDirVector() * -100.f );

			/// 데미지 이펙트 생성
			g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrEffectName.c_str(), 
				GetPowerRate() * fPowerRate, vPos, GetRotateDegree(), GetRotateDegree(),
				m_FrameDataNow.unitCondition.landPosition.y );
		}

		++ m_iWolfBreakeCount;		/// 카운트 증가
	}
}

/** @function	: ASI_SA_ALD_WOLF_BREAK_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_ALD_WOLF_BREAK_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#pragma endregion 낭아 최종오의 - 극쇄

#pragma region SI_SA_AYR_OPEN_THE_HELL
/** @function	: ASI_SA_AYR_OPEN_THE_HELL_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_SA_AYR_OPEN_THE_HELL_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_SA_AYR_OPEN_THE_HELL", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_OPEN_THE_HELL_Init );
	stateData_.OnFrameMove		= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_OPEN_THE_HELL_FrameMove );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_SA_AYR_OPEN_THE_HELL_EventProcess );
}

/** @function	: ASI_SA_AYR_OPEN_THE_HELL_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_SA_AYR_OPEN_THE_HELL_Init()
{
	TextureReadyInBackground(L"Ara_Black_LEAF.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"SA_AYR_ENERGY_CANNON_Map02.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"aisha_active_guillotinePress_circleParticle.dds");

	XSkinMeshReadyInBackground(L"SA_AYR_OPEN_THE_HELL_BADAK.X");
	XSkinMeshReadyInBackground(L"SA_AYR_ORB_KI_BALL.X");
	XSkinMeshReadyInBackground(L"aisha_active_energySpurt_circle.X");
}

/** @function	: ASI_SA_AYR_OPEN_THE_HELL_FrameMove
	@brief		: 패시브 지옥 초래 적용시, 기력을 흡수하는 데미지 이펙트 생성
*/
void CX2GUAra::ASI_SA_AYR_OPEN_THE_HELL_FrameMove()
{
	CommonFrameMove();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( 0.01f ) && true == EventCheck( 0.01f, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( NULL != g_pX2Game->GetEffectSet() )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"SA_AYR_OPEN_THE_HELL_ALL", this, NULL, IsHyperState(), GetPowerRate() * m_fOpenTheHellIncreaseDamage );
		}
	}
}

/** @function	: ASI_SA_AYR_OPEN_THE_HELL_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_SA_AYR_OPEN_THE_HELL_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );

	}

	CommonEventProcess();
}
#pragma endregion 나찰 최종오의 - 연옥

#pragma region SI_HA_AYR_ENERGY_CANNON
/** @function	: ASI_HA_AYR_ENERGY_CANNON_CreateState
	@brief		: 해당 스킬 아이디에 대한 스테이트 정보 설정
	@param		: State 데이터, State 아이디
*/
void CX2GUAra::ASI_HA_AYR_ENERGY_CANNON_CreateState( UserUnitStateData& stateData_, const int iStateId_ )
{
	stateData_.Init();
	stateData_.stateID			= iStateId_;
	m_LuaManager.MakeTableReference( "ASI_HA_AYR_ENERGY_CANNON", stateData_.stateID );
	stateData_.StateInit		= SET_CB_FUNC( CX2GUAra, ASI_HA_AYR_ENERGY_CANNON_Init );
	stateData_.OnEventProcess	= SET_CB_FUNC( CX2GUAra, ASI_HA_AYR_ENERGY_CANNON_EventProcess );
}

/** @function	: ASI_HA_AYR_ENERGY_CANNON_Init
	@brief		: 필요한 리소스 선로딩
*/
void CX2GUAra::ASI_HA_AYR_ENERGY_CANNON_Init()
{
	TextureReadyInBackground(L"Ara_Orange_LEAF.dds");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"GroundShockWave.dds");
	TextureReadyInBackground(L"RTW_Flame_Buster.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"stone_AirShip.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"SA_AYR_ENERGY_CANNON_Map04.dds");
	TextureReadyInBackground(L"SI_SA_ALH_MOONLIGHT_SLASH_BLADE01.tga");

	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"SA_AYR_ORB_KI_BALL.X");
	XSkinMeshReadyInBackground(L"SA_ENERGY_CANNON_DUMMY.X");
}

/** @function	: ASI_HA_AYR_ENERGY_CANNON_EventProcess
	@brief		: 상태변경 여부를 판단, 수행
*/
void CX2GUAra::ASI_HA_AYR_ENERGY_CANNON_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * _CONST_ARA_::MAGIC_NUMBER_MULTIPLY_LINE_RADIUS;
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#pragma endregion 대규환지옥

#pragma endregion 2-1, 2-2 스페셜 액티브 스킬

#endif // SERV_ARA_CHANGE_CLASS_SECOND


#endif	/// ARA_CHARACTER_BASE

void CX2GUAra::InitAquiredActiveSkillState( const int iSkillID_ )
{
	if ( NULL != GetUnit() )
	{
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const CX2SkillTree::SkillTemplet* pSkillTemplet = 
			userSkillTree.GetUserSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( iSkillID_ ) );
		if ( NULL != pSkillTemplet )
		{
			switch ( iSkillID_ )
			{
			case CX2SkillTree::SI_A_ALH_ROCK_CRASH:
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ALH_ROCK_CRASH_START];
						ASI_A_ALH_ROCK_CRASH_FALLING_START_CreateState( stateData, ASI_A_ALH_ROCK_CRASH_START );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}						

					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ALH_ROCK_CRASH_END];
						ASI_A_ALH_ROCK_CRASH_END_CreateState( stateData, ASI_A_ALH_ROCK_CRASH_END );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				case CX2SkillTree::SI_A_AM_TIGER_CLAW:
#else //UPGRADE_SKILL_SYSTEM_2013
				case CX2SkillTree::SI_A_ARA_TIGER_CLAW:
#endif //UPGRADE_SKILL_SYSTEM_2013
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ARA_TIGER_CLAW_START];
						ASI_A_ARA_TIGER_CLAW_START_CreateState( stateData, ASI_A_ARA_TIGER_CLAW_START );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ARA_TIGER_CLAW_END];
						ASI_A_ARA_TIGER_CLAW_END_CreateState( stateData, ASI_A_ARA_TIGER_CLAW_END );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

			case CX2SkillTree::SI_A_ASD_LOW_BRANDISH:
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ASD_LOW_BRANDISH_SLICE];
						ASI_A_ASD_LOW_BRANDISH_SLIDING_CreateState( stateData, ASI_A_ASD_LOW_BRANDISH_SLICE );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ASD_LOW_BRANDISH_KICK];
						ASI_A_ASD_LOW_BRANDISH_KICK_CreateState( stateData, ASI_A_ASD_LOW_BRANDISH_KICK );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			case CX2SkillTree::SI_A_ASD_TURBULENT_WAVE: // 맹호 4식 - 노도
#else //UPGRADE_SKILL_SYSTEM_2013
			case CX2SkillTree::SI_SA_ASD_TURBULENT_WAVE: // 맹호 4식 - 노도
#endif //UPGRADE_SKILL_SYSTEM_2013
				{
					//// 오의를 발동 시키는 스테이트의 경우에는 모든 스테이트에 stateData.m_ptrComboAndSkillChainInfo 셋팅을 해야 함
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ASD_TURBULENT_WAVE_START];
						ASI_SA_ASD_TURBULENT_WAVE_START_CreateState( stateData, ASI_SA_ASD_TURBULENT_WAVE_START );				
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ASD_TURBULENT_WAVE_LOOP];
						ASI_SA_ASD_TURBULENT_WAVE_LOOP_CreateState( stateData, ASI_SA_ASD_TURBULENT_WAVE_LOOP );				
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ASD_TURBULENT_WAVE_END];
						ASI_SA_ASD_TURBULENT_WAVE_END_CreateState( stateData, ASI_SA_ASD_TURBULENT_WAVE_END );				
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ASD_VOID_BREAK];
						ASI_SA_ASD_VOID_BREAK_CreateState( stateData, ASI_SA_ASD_VOID_BREAK );				
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				case CX2SkillTree::SI_SA_AM_PULLING_PIERCE:
#else //UPGRADE_SKILL_SYSTEM_2013
				case CX2SkillTree::SI_SA_ARA_PULLING_PIERCE:
#endif //UPGRADE_SKILL_SYSTEM_2013
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ARA_PULLING_PIERCE_START];
						ASI_SA_ARA_PULLING_PIERCE_START_CreateState( stateData, ASI_SA_ARA_PULLING_PIERCE_START );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ARA_PULLING_PIERCE_END];
						ASI_SA_ARA_PULLING_PIERCE_END_CreateState( stateData, ASI_SA_ARA_PULLING_PIERCE_END );
						SetSkillLevelStateData( pSkillTemplet, stateData );						
					}

				} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				case CX2SkillTree::SI_A_ALH_DOUBLE_COLLISION:
#else //UPGRADE_SKILL_SYSTEM_2013
				case CX2SkillTree::SI_SA_ALH_DOUBLE_COLLISION:
#endif //UPGRADE_SKILL_SYSTEM_2013
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALH_DOUBLE_COLLISION_START];
						ASI_SA_ALH_DOUBLE_COLLISION_START_CreateState( stateData, ASI_SA_ALH_DOUBLE_COLLISION_START );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALH_DOUBLE_COLLISION_END];
						ASI_SA_ALH_DOUBLE_COLLISION_END_CreateState( stateData, ASI_SA_ALH_DOUBLE_COLLISION_END );
						SetSkillLevelStateData( pSkillTemplet, stateData );						
					}
				} break;

			case CX2SkillTree::SI_SA_ALH_MOONLIGHT_SLASH:
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALH_MOONLIGHT_SLASH_READY];
						ASI_SA_ALH_MOONLIGHT_SLASH_READY_CreateState( stateData, ASI_SA_ALH_MOONLIGHT_SLASH_READY);
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE];
						ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE_CreateState( stateData, ASI_SA_ALH_MOONLIGHT_SLASH_CHARGE );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK];
						ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK_CreateState( stateData, ASI_SA_ALH_MOONLIGHT_SLASH_CRESCENT_ATTACK );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK];
						ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK_CreateState( stateData, ASI_SA_ALH_MOONLIGHT_SLASH_FULLMOON_ATTACK );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}
								
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALH_DESCENT_DRAGON];
						ASI_SA_ALH_DESCENT_DRAGON_CreateState( stateData, ASI_SA_ALH_DESCENT_DRAGON );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}

					if ( NULL != m_sMoonRightSlashDataPtr )
						m_sMoonRightSlashDataPtr.reset();

					m_sMoonRightSlashDataPtr = sMoonLightSlashData::CreateMoonLightSlashData();
					if ( NULL != m_sMoonRightSlashDataPtr )
					{
						m_sMoonRightSlashDataPtr->SetSlotID( ASI_SA_ALH_MOONLIGHT_SLASH_READY );
						m_sMoonRightSlashDataPtr->SetSlotB( false );
					}
				} break;

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

			case CX2SkillTree::SI_A_AM_WIND_WEDGE:		/// 아라 1차 전직 - 낭아 2식 바람 쐐기
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ARA_WIND_WEDGE_START];
						ASI_SA_ARA_WIND_WEDGE_START_CreateState( stateData, ASI_SA_ARA_WIND_WEDGE_START );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ARA_WIND_WEDGE_END];
						ASI_SA_ARA_WIND_WEDGE_END_CreateState( stateData, ASI_SA_ARA_WIND_WEDGE_END );
						SetSkillLevelStateData( pSkillTemplet, stateData );						
					}

				} break;

			case CX2SkillTree::SI_A_ALD_WOLF_CLAW:			/// 아라 1차 전직 소마 - 낭아 3식 늑대발톱
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ALD_WOLF_CLAW];
						ASI_A_ALD_WOLF_CLAW_CreateState( stateData, ASI_A_ALD_WOLF_CLAW );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_ALD_WOLF_CLAW_FINISH];
						ASI_A_ALD_WOLF_CLAW_FINISH_CreateState( stateData, ASI_A_ALD_WOLF_CLAW_FINISH );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

			case CX2SkillTree::SI_SA_ALD_WOLF_SPEAR:		/// 아라 1차 전직 소마 - 낭아 4식 늑대이빨
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_WOLF_SPEAR];
						ASI_SA_ALD_WOLF_SPEAR_CreateState( stateData, ASI_SA_ALD_WOLF_SPEAR );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_WOLF_SPEAR_FINISH];
						ASI_SA_ALD_WOLF_SPEAR_FINISH_CreateState( stateData, ASI_SA_ALD_WOLF_SPEAR_FINISH );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_WOLF_BREAK];			/// 낭아 최종오의 - 극쇄
						ASI_SA_ALD_WOLF_BREAK_CreateState( stateData, ASI_SA_ALD_WOLF_BREAK );				
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

			case CX2SkillTree::SI_SA_ALD_FINGER_BULLET:		/// 아라 1차 전직 소마 - 나찰 1식 흡혼
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_FINGER_BULLET];
						ASI_SA_ALD_FINGER_BULLET_CreateState( stateData, ASI_SA_ALD_FINGER_BULLET );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_FINGER_BULLET_FINISH];
						ASI_SA_ALD_FINGER_BULLET_FINISH_CreateState( stateData, ASI_SA_ALD_FINGER_BULLET_FINISH );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

			case CX2SkillTree::SI_SA_ALD_ENERGY_BEAD:		/// 아라 1차 전직 소마 - 나찰 2식 연환장
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_ENERGY_BEAD];
						ASI_SA_ALD_ENERGY_BEAD_CreateState( stateData, ASI_SA_ALD_ENERGY_BEAD );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_ENERGY_BEAD_FINISH];
						ASI_SA_ALD_ENERGY_BEAD_FINISH_CreateState( stateData, ASI_SA_ALD_ENERGY_BEAD_FINISH );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

			case CX2SkillTree::SI_SA_ALD_PRISON_SPEAR:		/// 아라 1차 전직 소마 - 나찰 3식 창살감옥
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_PRISON_SPEAR];
						ASI_SA_ALD_PRISON_SPEAR_CreateState( stateData, ASI_SA_ALD_PRISON_SPEAR );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_ALD_PRISON_SPEAR_FINISH];
						ASI_SA_ALD_PRISON_SPEAR_FINISH_CreateState( stateData, ASI_SA_ALD_PRISON_SPEAR_FINISH );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;

			case CX2SkillTree::SI_A_AYR_STEAL_SOUL:			/// 아라 2차 전직 명왕 - 나찰 4식 탈혼
				{
					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_AYR_STEAL_SOUL];
						ASI_A_AYR_STEAL_SOUL_CreateState( stateData, ASI_A_AYR_STEAL_SOUL );
						SetSkillLevelStateData( pSkillTemplet, stateData );
						stateData.m_ptrComboAndSkillChainInfo = m_ComboAndSkillChainSystem.GetSkillChainInfoPtrBySkillID( iSkillID_ );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_A_AYR_STEAL_SOUL_FINISH];
						ASI_A_AYR_STEAL_SOUL_FINISH_CreateState( stateData, ASI_A_AYR_STEAL_SOUL_FINISH );
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}

					{
						UserUnitStateData&	stateData = m_StateList[ASI_SA_AYR_OPEN_THE_HELL];		/// 나찰 최종오의 - 연옥
						ASI_SA_AYR_OPEN_THE_HELL_CreateState( stateData, ASI_SA_AYR_OPEN_THE_HELL );				
						SetSkillLevelStateData( pSkillTemplet, stateData );
					}
				} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
			}
		}
	}
	
}

#ifdef ADD_ARA_MEMO	// 적용날짜: 2013-03-22

/** @function	: GetSkillCoolTimeValue
	@brief		: 메모 등으로 인한 쿨타임 변경값 설정 함수
	@param		: 설정할 스킬 아이디, 설정할 스킬의 디폴트 쿨타임
	@return		: 적용 시킬 최종 쿨타임
*/
float CX2GUAra::GetSkillCoolTimeValue( const CX2SkillTree::SKILL_ID eSkillID_, const float fCoolTimeLeft )
{
	float fCoolTimeValue = fCoolTimeLeft;

	switch( eSkillID_ )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AM_GAIN_FORCE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_ARA_GAIN_FORCE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO1 ) )
				fCoolTimeValue -= 1.f;
		} break;
	}

	return fCoolTimeValue;
}

/** @function	: GetActualFPConsume
	@brief		: 메모 등으로 인하여 변동된 기력 소비량 설정 함수
	@param		: 설정할 스킬 아이디, 설정할 스킬의 디폴트 기력량
	@return		: 적용 시킬 최종 소비 기력량
*/
USHORT CX2GUAra::GetActualFPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const USHORT usForceGainValue_ )
{
	USHORT usForceGainValue = usForceGainValue_;

	switch( eSkillID_ )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_AM_SHADOW_KNOT:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_SHADOW_KNOT:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			/// 메모 : 비전 그림자 창술 (그림자매듭) - 소비 기력 3
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO5 ) )
				usForceGainValue = 3;
		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AM_WIND_WEDGE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_WIND_WEDGE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			/// 메모 : 흐름을 타는 창술 (맹호 2식 : 바람쐐기) - 소비 기력 1
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO2 ) )
				usForceGainValue = 1;
		} break;
	}

	return usForceGainValue;
}

/** @function	: GetActualFPGainCount
	@brief		: 메모 등으로 인하여 변동된 기력 전환량 설정 함수
	@param		: 설정할 스킬 아이디, 설정할 스킬의 디폴트 기력 전환량
	@return		: 적용 시킬 최종 전환 기력량
*/
USHORT CX2GUAra::GetActualFPGainCount( const CX2SkillTree::SKILL_ID eSkillID_, const USHORT usForceGainValue_ )
{
	USHORT usForceGainValue = usForceGainValue_;

	switch( eSkillID_ )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AM_WIND_WEDGE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ARA_WIND_WEDGE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			/// 메모 : 흐름을 타는 창술 (맹호 2식 : 바람쐐기) - 전환 기력 1
			if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_ARA_MEMO2 ) )
				usForceGainValue = 1;
		} break;
	}

	return usForceGainValue;
}

#endif // ADD_ARA_MEMO

#ifdef RIDING_SYSTEM
/*virtual*/ bool CX2GUAra::CanRidingState()
{
	if ( GetNowStateID() == CX2GUUser::USI_WAIT || GetNowStateID() == CX2GUUser::USI_WALK || 
		GetNowStateID() == CX2GUUser::USI_DASH || GetNowStateID() == CX2GUUser::USI_DASH_END || 
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT || GetNowStateID() == CX2GUUser::USI_RIDING_WALK ||
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT_HABIT || GetNowStateID() == CX2GUUser::USI_RIDING_DASH ||
		GetNowStateID() == CX2GUUser::USI_RIDING_DASH_END || GetNowStateID() == CX2GUAra::ASI_HYPER_WAIT )
		return true;

	return false;
}
#endif //RIDING_SYSTEM

/*virtual*/ void CX2GUAra::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_  )
{
	ShowActiveSkillCutInAndLightByBerserkMode(  fTimeToShow_, bOnlyLight_ );
}

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환

#pragma region CreateEnergeBullet
/** @function	: CreateEnergeBullet
	@brief		: 기탄 데미지 이펙트 생성 함수
	@param		: 회전값, 발사 손 위치
*/
void CX2GUAra::CreateEnergeBullet( IN const float vRotZOffSet_ /*= 0.f*/, IN const bool bFireRightHand_ /*= true*/ )
{
	if( FlushMp( _CONST_ARA_::ENERGY_BULLET_MP_CONSUMPTION ) == true )		/// MP 소모율 검사
	{
		/// 기탄 생성 위치
		D3DXVECTOR3 vPos = bFireRightHand_ ? GetBonePos( L"Bip01_R_Finger2" ) : GetBonePos( L"Bip01_L_Finger2" );

		D3DXVECTOR3 vRot = GetRotateDegree();

		vRot.z = vRotZOffSet_;		/// 기탄 회전값 설정


		float fEnergyBulletPowerRate	= GetPowerRate();	/// 기탄 데미지 배율
		float fEnergyBulletForcePower	= 1.f;				/// 기탄 다운 수치 감소율

		/// 패시브 스킬 기탄 강화로 인한 기탄 데미지 증가
		if ( 1.f < m_fIncreaseEnergeBulletDamage )
			fEnergyBulletPowerRate *= m_fIncreaseEnergeBulletDamage;

		/// 패시브 스킬 기력 강화로 인한 기탄 다운 수치 감소
		if ( 0.f < m_fIncreaseEnergeBulletForceDownValue )
			fEnergyBulletForcePower *= m_fIncreaseEnergeBulletForceDownValue;

		/// 기탄 생성
		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this,
			L"EffectSet_SA_AYR_ORB_KI_FX_DAMAGE", fEnergyBulletPowerRate, vPos, vRot, vRot,
			m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, fEnergyBulletForcePower );

		if ( NULL != pDamageEffect )
		{
            CKTDGXMeshPlayer::CXMeshInstance *pMeshInstance = pDamageEffect->GetMainEffect();
            if ( pMeshInstance != NULL )
            {
			    /// 패시브 스킬 기력 강화로 인한 기탄 사거리 증가
			    if ( 1.f < m_fIncreaseEnergeBulletRange )
			    {
				    const float fMaxLifeTime = pMeshInstance->GetMaxLifeTime();
				    pMeshInstance->SetMaxLifeTime( fMaxLifeTime * m_fIncreaseEnergeBulletRange );
			    }

			    /// 기탄 다운 수치 상속
				    pDamageEffect->SetInheritForceDownRate(true);

			    /// 패시브 스킬 기탄 강화로 인한 기탄 크기 증가
			    if ( 1.f < m_fIncreaseEnergeBulletSize )
				    pDamageEffect->SetScale( D3DXVECTOR3( m_fIncreaseEnergeBulletSize, m_fIncreaseEnergeBulletSize, m_fIncreaseEnergeBulletSize ) );
            }
		}
	}
	else	/// MP 부족할 때, 
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		if( true == GetIsRight() )
		{
			pos += GetDirVector() * 100.f;
		}
		else
		{
			pos -= GetDirVector() * 100.f;
		}

		CreateNotEnoughMPEffect( pos, 0.f, 0.f, vRotZOffSet_ );
	}
}
#pragma endregion

#pragma region SetResurrection
/** @function	: ProcessResurrection
	@brief		: 아라 2차 전직 명왕 - 회광반조 동작 처리 함수
	@Praram		: 스킬 레벨
	@Return		: 적용 여부
*/
#ifdef FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE // 김태환
bool CX2GUAra::ProcessResurrection( IN const int iSkillLevel_ )
#else //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE
void CX2GUAra::SetResurrection( IN const int iSkillLevel_ )
#endif //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE
{
	/// 회강반조 사용 가능 시점이며, 기력이 3개 이상일 때
	if ( 0.f >= m_fResurrectionCoolTime && 3 <= GetForcePower() && NULL != g_pData->GetSkillTree() )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_AYR_RESURRECTION );

		if ( NULL != pSkillTemplet )
		{
			/// 쿨타임 갱신
			m_fResurrectionCoolTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_READY_TIME_REL, iSkillLevel_ );

			/// 기본 체력 회복량
			const float fDefaultAddHPValue		= ( GetMaxHp() * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TARGET_HP_REL, iSkillLevel_ ) );

			/// 기력 1개당 체력 회복량
			const float fForcePowerAddHPValue	= ( GetMaxHp() * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_HP_GAIN_REL_MAX_HP, iSkillLevel_ ) );

			/// 기력에 따른 체력 회복치 연산 ( 기본 체력 회복량 + 남은 기력에 따른 체력 회복량 )
			const float fAddHP					= fDefaultAddHPValue + ( GetForcePower() * fForcePowerAddHPValue );

			SetNowHp( GetNowHp() + fAddHP );	/// 체력 회복

			UseForcePower( GetForcePower() );	/// 기력 모두 소비

			/// 발동 이펙트 설정
			if ( NULL != g_pX2Game->GetEffectSet() )
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_AYR_HEAL_FX", this );

			/// 체력 회복 문구 표시
			if( NULL != g_pData->GetPicCharGreen() )
			{
				D3DXVECTOR3 pos	= GetPos();		/// 문구 위치 설정
				pos.y += 50.0f;

				WCHAR wszBuf[256] = {0,};		/// 문구 설정
				StringCchPrintf( wszBuf, 256, L"HP +%d", static_cast<int>( fAddHP ) );

				g_pData->GetPicCharGreen()->DrawText( wszBuf, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
			}

			/// 호신강기 사용 불가 안내 디버프 생성
			SetBuffFactorToGameUnitByBuffFactorID( BFI_DEBUFF_SELF_PROTECTION_FORTITUDE );

#ifdef FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE // 김태환
			return true;
#endif //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE
		}
	}

#ifdef FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE // 김태환
	return false;
#endif //FIX_NOT_ACTIVE_SKILL_AT_ENCHANT_DAMAGE
}
#pragma endregion

#pragma region CanUseConversionHealth
/** @function	: CanUseConversionHealth
	@brief		: 이혈공을 사용할 수 있는 상태인지 검사
	@param		: 스킬 템플릿, 스킬 레벨
	@return		: 사용 가능 여부
*/
bool CX2GUAra::CanUseConversionHealth( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN const int iSkillLevel_ )
{
	if ( NULL == pSkillTemplet_ )
		return false;

	const float fLimitHPValue = GetMaxHp() * pSkillTemplet_->GetSkillAbilityValue( CX2SkillTree::SA_HP_REL_TO_MP_ABS_1, iSkillLevel_ );

	if ( fLimitHPValue < GetNowHp()  )
		return true;

	return false;
}
#pragma endregion

#pragma region SetAttackResultDelegate
/** @function	: SetAttackResultDelegate
	@brief		: 타격시 추가되는 기력량 설정
	@param		: 데미지 정보
*/
void CX2GUAra::SetAttackResultDelegate()
{
	if ( NULL == GetUnit() )
	{
		 return;
	}

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

	/// 아라 1차 전직 소마 - 흡혼 혹은 아라 2차 전직 소마 - 탈혼 습득시, 타격 정보 저장하는 기능 설정
	if ( 0 < userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_ALD_FINGER_BULLET ) ||
		 0 < userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AYR_STEAL_SOUL ) )
		InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUAra, AddHitUnitInfoByDamageData ) );

	/// 타격시 추가되는 기력량 설정
	InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUAra, UpNowForcePowerHitOthers ) );


}		
#pragma endregion

#pragma region UpNowForcePowerHitOthers
/** @function	: UpNowForcePowerHitOthers
	@brief		: 타격시 추가되는 기력량 설정
	@param		: 데미지 정보
*/
void CX2GUAra::UpNowForcePowerHitOthers( const CX2DamageManager::DamageData& damageData_ )
{
	/// 기력 습득량이 설정되어 있지 않다면, 패스
	if ( 0 >=  damageData_.m_iHitAddForcePower )
		return;

	const int	iMaxForcePower = GetMaxForcePower();				/// 최대 기력량
	const int	iNowForcePower = GetForcePower();					/// 현재 기력량
	int			iAddForcePower = damageData_.m_iHitAddForcePower;	/// 추가될 기력량

	/// 최대 기력량을 넘는다면, 최대치로 추가 되도록 설정
	if ( iMaxForcePower < iNowForcePower + iAddForcePower )
		iAddForcePower = iMaxForcePower - iNowForcePower;

	SetForcePower( iNowForcePower + iAddForcePower );
}		
#pragma endregion

#pragma region AddHitUnitByFingerBullet
/** @function	: InsertHitUnitByFingerBullet
	@brief		: 타격된 대상의 객체 포인터 저장( 아라 1차 전직 소마 - 흡공 )
	@param		: 데미지 정보
*/
void CX2GUAra::AddHitUnitInfoByDamageData( const CX2DamageManager::DamageData& damageData_ )
{
	if ( null == damageData_.optrDefenderGameUnit )
		return;

	switch ( damageData_.m_eUsedSkillID )
	{
	case CX2SkillTree::SI_SA_ALD_FINGER_BULLET:
	case CX2SkillTree::SI_A_AYR_STEAL_SOUL:
		{
			/// 피격자의 UID ( 비교를 위해, NPC도 일단 64비트로 변환 )
			UidType uidUnitUID = static_cast<UidType>( damageData_.optrDefenderGameUnit->GetUnitUID() );

			/// 저장 벡터를 순회하여, 이미 해당 UID가 추가되었는지 검사한다.
			std::vector<CX2GameUnitoPtr>::const_iterator vit = m_vecHitUnitPointer.begin();

			for ( vit; vit != m_vecHitUnitPointer.end(); ++vit )
			{
				CX2GameUnitoPtr pGameUnit = *vit;

				/// 이미 해당 UID가 추가되어 있다면, 패스
				if ( null != pGameUnit && pGameUnit->GetUnitUID() == uidUnitUID )
					return;
			}

			/// 해당 UID가 추가되어 있지 않다면, 추가
			m_vecHitUnitPointer.push_back( damageData_.optrDefenderGameUnit );
		} break;
	}
}		
#pragma endregion

#pragma region ModifyFinalDamageByGameUnitType
/** @function	: ModifyFinalDamageByGameUnitType
	@brief		: 데미지 처리시 추가로 관련 수치 설정하는 함수( GetFinalDamage )
	@param		: 데미지 타입, 추가 물리 공격력, 추가 마법 공격력, 다운 수치
*/
/*virtual*/ void CX2GUAra::ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, 
															OUT float& fAttackerMagicDamage_, OUT float& fForceDownValue_ )
{
	if ( NULL == GetUnit() )
		return;

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

	/// 아라 1차 전직 소마 - 기력 강화
	if ( 0 < userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ALD_ENERGY_REINFORCE ) )
	{
		///  일반 공격일 때, 기력 강화로 인한 다운 수치 감소값 적용
		if ( CX2DamageManager::AT_NORMAL == damageType_ )
			fForceDownValue_ -= m_fIncreaseEnergeBulletForceDownValue;
	}
}
#pragma endregion

#ifndef SERV_9TH_NEW_CHARACTER // 김태환
/// 다른 캐릭터들 전부 똑같은 함수를 쓰고 있으니, X2GUUser로 옮기자.
#pragma region CreateNotEnoughMPEffect
/** @function	: CreateNotEnoughMPEffect
	@brief		: MP 부족시, 헛타 생성
	@param		: 위치, 회전값 X, 회전값 Y, 회전값 Z
*/
void CX2GUAra::CreateNotEnoughMPEffect( D3DXVECTOR3 vPos, float fDegreeX, float fDegreeY, float fDegreeZ )
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleCommonMajorParticleByEnum( COMMON_MAJOR_PII_MAGIC_FAIL ) );
	if( NULL != pSeq )
	{
		PlaySequenceByTriggerCount( pSeq, vPos, 100, 100, 1 );
		D3DXVECTOR3 vAngle = GetRotateDegree();
		vAngle += D3DXVECTOR3( fDegreeX, fDegreeY, fDegreeZ );

		pSeq->SetAxisAngle( vAngle );
		pSeq->SetAddRotate( vAngle );
	}
}
#pragma endregion
#endif //SERV_9TH_NEW_CHARACTER

#pragma region GetStateIDBySecretSkillStartID
/** @function	: GetAraStateIDBySecretSkillStartID
	@brief		: 스킬 슬롯 스테이트 아이디에 해당하는 스테이트 아이디 반환
	@param		: 스킬 아이디
	@return		: 스테이트 아이디
*/
/*virtual*/void CX2GUAra::GetStateIDBySecretSkillTriggerStateID( IN OUT int& iStateID_ ) const
{
	if ( NULL != GetUnit() )
	{
		/// 스킬 슬롯 StateID에 설정되어 있는 스킬 ID를 반환 ( 스킬 슬롯 State ID가 아니라면, SI_NONE 반환 )
		const CX2SkillTree::SKILL_ID	eSkillIDBySlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillIDByEquipSlotID( iStateID_ );

		/// 스킬 ID에서 해당 스킬의 State ID를 반환
		/// ( 기본 스킬 State ID는 스킬 슬롯의 인덱스 이므로, 해당 ID를 통해 실제 State ID를 반환 )
		if (  CX2SkillTree::SI_NONE != eSkillIDBySlotData )
		{
			switch ( eSkillIDBySlotData )
			{
				/// 낭아 최종오의 극쇄 시작 스테이트 - 낭아 4식 늑대이빨
			case CX2SkillTree::SI_SA_ALD_WOLF_SPEAR:
				{
					iStateID_ = static_cast<int>( ASI_SA_ALD_WOLF_SPEAR );
				} break;

				/// 나찰 최종오의 연옥 시작 스테이트 - 나찰 4식 탈혼
			case CX2SkillTree::SI_A_AYR_STEAL_SOUL:
				{
					iStateID_ = static_cast<int>( ASI_A_AYR_STEAL_SOUL );
				} break;
			}
		}
	}
}
#pragma endregion

#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef MODIFY_RIDING_PET_AWAKE
void CX2GUAra::RidingHyperModeFrameMove()
{
	CommonHyperModeFrameMove( 0.f, 0.1f, 0.1f, true );
}
void CX2GUAra::CommonHyperModeFrameMove( float fTime1_, float fTime2_, float fTime3_, bool bSound_ /*= false*/  )
{
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( true == m_pXSkinAnim->EventTimerOneshot( fTime1_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( true == m_pXSkinAnim->EventTimer( fTime1_ ) && true == EventCheck( fTime1_, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( true == m_pXSkinAnim->EventTimerOneshot( fTime2_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( true == m_pXSkinAnim->EventTimer( fTime2_ ) && true == EventCheck( fTime2_ , false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();

		ApplyHyperModeBuff();

		// 기존 코드 유지를 위해 코드에서 사운드 출력 해주는 것 유지
		if( true == IsSamef(fTime2_ ,fTime3_) )
		{
			if ( true == IsFullHyperMode() )
				PlaySound( L"AraVoice_BoostPower.ogg" );		
		}

		if( true == bSound_ )
		{
			PlaySound( L"Energy.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if ( true == m_pXSkinAnim->EventTimerOneshot( fTime3_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if ( true == m_pXSkinAnim->EventTimer( fTime3_ ) && true == EventCheck( fTime3_, false ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if ( true == IsFullHyperMode() )
			PlaySound( L"AraVoice_BoostPower.ogg" );		
	}

	CommonFrameMove();
}
#endif // MODIFY_RIDING_PET_AWAKE

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region IsNoStageChangeSkill
/** @function	: IsNoStageChangeSkill
	@brief		: 해당 스킬이 스테이트가 없는 스킬인지 검사
	@param		: 사용한 스킬의 스킬 템플릿 포인터
	@return		: 스테이트가 없는 스킬인지 여부
*/
bool CX2GUAra::IsNoStageChangeSkill( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_ ) const
{
	if ( NULL != pSkillTemplet_ )
	{
		switch( pSkillTemplet_->m_eID )
		{
		case CX2SkillTree::SI_A_ALD_REFLECTION:		/// 아라 1차 전직 소마 - 반탄공
			return true;
		}
	}

	return false;
}
#pragma endregion
#endif // SERV_ARA_CHANGE_CLASS_SECOND

//#ifdef FIX_GAIN_FORCE_BUG
/*virtual*/void CX2GUAra::AttackResultByType( CX2DamageManager::DamageData &pDamageData )
{
	if( CX2SkillTree::SI_A_AM_GAIN_FORCE == m_NowStateData.m_eSkillID )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( m_hGainForceIntakeEffect == pDamageData.hAttackerEffect )
			++m_iGainForceDamageTimeNow;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CX2DamageEffect::CEffect* pEffect = static_cast<CX2DamageEffect::CEffect*>(pDamageData.pAttackerEffect);
		if( m_hGainForceIntakeEffect == pDamageData.hAttackerEffect )
		{
			m_iGainForceDamageTimeNow = pEffect->GetDamageTimeNow();
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}
}
//#endif // FIX_GAIN_FORCE_BUG

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
/** @function	: GetHypetMpdeStateID
	@brief		: 아라 3각성 여부에 따른 스테이트 아이디 반환
	@return		: 스테이트 아이디
*/
/*virtual*/ const int CX2GUAra::GetHypetMpdeStateID()
{
	if ( 3 <= m_FrameDataFuture.syncData.m_HyperModeCount )
		return static_cast<int>( ASI_BERSERK_MODE );
	else
		return static_cast<int>( USI_HYPER_MODE );
}
#endif //SKILL_CANCEL_BY_HYPER_MODE
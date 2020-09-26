#include "StdAfx.h"
#include ".\x2guelsword_swordman.h"

#define LEAP_GRAVITY 9.8f


#ifdef SERV_APRIL_FOOLS_DAY
static const CX2GUUser::SkillCutIn s_SkillCutInSetFoolsDay = CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_Gura.dds");
#endif //APRIL_FOOLS_DAY



// 필살기 쓸 때 cut in image 
static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] = 
{
	// 엘소드, 소드나이트, 매직나이트
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_1.dds"),// D3DXVECTOR2(655.0f,655.0f), D3DXVECTOR3(192.0f,496.0f,0.0f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_2.dds"),// D3DXVECTOR2(691.0f,691.0f), D3DXVECTOR3(202.0f,423.0f,0.0f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_3.dds"),// D3DXVECTOR2(670.0f,670.0f), D3DXVECTOR3(275.0f,471.0f,0.0f) ),
	},
	
	// 로드 나이트
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_LK_1.dds"),// D3DXVECTOR2( 601.f, 601.f ), D3DXVECTOR3( 269.f, 488.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_LK_1.dds"),// D3DXVECTOR2( 601.f, 601.f ), D3DXVECTOR3( 269.f, 488.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_LK_1.dds"),// D3DXVECTOR2( 601.f, 601.f ), D3DXVECTOR3( 269.f, 488.f, 0.f ) ),
	},

	// 룬 슬레이어
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_RS_1.dds"),// D3DXVECTOR2( 615.f, 615.f ), D3DXVECTOR3( 268.f, 475.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_RS_1.dds"),// D3DXVECTOR2( 615.f, 615.f ), D3DXVECTOR3( 268.f, 475.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_RS_1.dds"),// D3DXVECTOR2( 615.f, 615.f ), D3DXVECTOR3( 268.f, 475.f, 0.f ) ),
	},

#ifdef SERV_ELSWORD_INFINITY_SWORD
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_IS_1.dds"),// D3DXVECTOR2( 615.f, 615.f ), D3DXVECTOR3( 310.f, 433.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_IS_1.dds"),// D3DXVECTOR2( 615.f, 615.f ), D3DXVECTOR3( 310.f, 433.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Elsword_IS_1.dds"),// D3DXVECTOR2( 615.f, 615.f ), D3DXVECTOR3( 310.f, 433.f, 0.f ) ),
	},
#endif //SERV_ELSWORD_INFINITY_SWORD
};





namespace _CONST_ELSWORD_
{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	const float SKILL_DEFENCE_MP_COST_PER_SEC = 3.f;		// 막기 스킬의 초당 마나 소모량
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#ifdef BALANCE_PATCH_20120329
	const float MAGIC_FIRE_BALL_MP_COST = 6.f;
	const float MAGIC_ERS_XZ_MP_COST = 4.f;
	const float MAGIC_ERS_XZZ_MP_COST = 0.f;
	const float MAGIC_ERS_DASH_JUMP_XZ_MP_COST = 4.f;
	const float MAGIC_ERS_DASH_JUMP_XZZ_MP_COST = 0.f; 
#else
	const float MAGIC_FIRE_BALL_MP_COST = 5.f;
	const float MAGIC_ERS_XZ_MP_COST = 3.f;
	const float MAGIC_ERS_XZZ_MP_COST = 6.f;
	const float MAGIC_ERS_DASH_JUMP_XZ_MP_COST = 3.f;
	const float MAGIC_ERS_DASH_JUMP_XZZ_MP_COST = 7.f; 
#endif

#ifdef ELSWORD_WAY_OF_SWORD
	const float VIGOR_FIRE_BALL_MP_COST = 5.f;			// 파괴
	const float VIGOR_ERS_XZ_MP_COST = 3.f;				// 파괴
	const float VIGOR_ERS_XZZ_MP_COST = 6.f;			// 파괴
	const float VIGOR_ERS_DASH_JUMP_XZ_MP_COST = 3.f;	// 파괴
	const float VIGOR_ERS_DASH_JUMP_XZZ_MP_COST = 3.5f;	// 활력 - MP소모 50% 감소
#endif ELSWORD_WAY_OF_SWORD

#ifdef ELSWORD_SHEATH_KNIGHT
	const float MAGIC_PHANTOM_SWORD_DELAY	= 0.2f;
#endif ELSWORD_SHEATH_KNIGHT

	const int	CONST_BRUTAL_SLAYER_LEVEL = 5;

#ifdef FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템

	const int BLADE_RAIN_SKILL_RANGE = 250;								// 블레이드 레인 거리
	const int BLADE_RAIN_GET_SUMMON_POS_CHECK_LINEMAP_COUNT = 10;		// 블레이드 레인 라인맵 체크 거리

#endif // FINALITY_SKILL_SYSTEM // 김종훈, 궁극기 시스템
}





CX2GUElsword_SwordMan::CX2GUElsword_SwordMan( int unitIndex, int teamNum, 
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
, m_fRollingSmashDuration( 0.f )
, m_bIsMagicKnightWinMotion2( false )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hRuneSlayerDashJumpXZ( INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_pRuneSlayerDashJumpXZ( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hRuneSlayerComboXZ( INVALID_EFFECTSET_HANDLE )
, m_hSwordEnchant( INVALID_EFFECTSET_HANDLE )
, m_bERS_DASH_JUMP_XZ_Rune_Created( false )
, m_bERS_COMBO_XZ_Rune_Created( false )
#ifdef BALANCE_RUNE_SLAYER_20130214
, m_bShowWeaponEnchantEffect( true )
#endif //BALANCE_RUNE_SLAYER_20130214
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hDamageEffectHarshChaserBlade( INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_pDamageEffectHarshChaserBlade( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
, m_fAutoGuardRate( 0.f )
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hDamageEffectPiercingSword( INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_pDamageEffectPiercingSword( NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_fPiercingSwordProjectileRange( 0.f )
, m_fRuneProjectileRangeIncPercent( 0.f )
, m_fRuneProjectileSizeIncPercent( 0.f )
, m_ucSaveDebuffCount( 0 )
, m_bAutoGuardBlocking( false )
, m_bIsGuardMotion( false )
, m_fIncreaseCoolTimeRel( 1.f )
, m_fActiveSkillAddPowerRate( 1.f )
, m_hEffectSwordShield( INVALID_EFFECTSET_HANDLE )
, m_fSwordShieldEffectiveTime( 0.f )
#endif //UPGRADE_SKILL_SYSTEM_2013
#ifdef BALANCE_PATCH_20131107
, m_iSkillLevelInduranceOfRevenge( 0 )
#endif //BALANCE_PATCH_20131107
{
	m_bReAttackZ1Right					= false;
	m_fReAttackZ1Time					= 0.0f;
	m_fReDashJumpXTime					= 0.0f;

#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	m_pMegaSlash						= NULL;
	m_pMegaSlashBackDust				= NULL;

	m_pFlameGager						= NULL;
	m_pFlameGagerLand					= NULL;

	m_pMagnumBlade						= NULL;
	m_pMagnumBladeFire					= NULL;

//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST


	m_hMeshInstAirSlash1				= INVALID_MESH_INSTANCE_HANDLE;

	m_hMeshInstFatalFury				= INVALID_MESH_INSTANCE_HANDLE;

	InitializeElswordMajorParticleArray();
	InitializeElswordMinorParticleArray();

	m_hSeqAssualtSlashRing				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqAssualtSlashDust				= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hSeqHandFire						= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_StartPosToLeap					= D3DXVECTOR3(0,0,0);
	m_DirVecToLeap						= D3DXVECTOR3(0,0,0);
	m_fVerticalSpeedToLeap				= 0.0f;
	m_fParallelSpeedToLeap				= 0.0f;
	m_fHeightToLeap						= 0.0f;
	m_DegreeToLeap						= 0;
	m_fTimeToLeap						= 0.0f;
	m_fElapsedTimeToLeap				= 0.0f;






	m_fAttackPowerRateFireBall = 1.f;
	
	m_fWindMillDuration = 0.f;
	m_hWindMill = INVALID_EFFECTSET_HANDLE;
	m_hWindMillLand = INVALID_EFFECTSET_HANDLE;	
	m_hSwordFire = INVALID_EFFECTSET_HANDLE;


	m_pSoundWindMill = NULL;
	m_pSoundWindMill = g_pKTDXApp->GetDeviceManager()->OpenSound( L"Elsword_WindmillSpin.ogg" );



#ifdef SKILL_CASH_10_TEST
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hDoubleSlash = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pDoubleSlash = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif // SKILL_CASH_10_TEST

#ifdef ELSWORD_WAY_OF_SWORD
	//SetWayOfSwordState( static_cast<int>( WSS_CENTER ) );
	m_hEffectVigor				= INVALID_EFFECTSET_HANDLE;
	m_hEffectDestruction		= INVALID_EFFECTSET_HANDLE;
	m_fDestructionRageTime		= 0.f;
	m_fAnimSpeedUpByVigor		= 0.f;
#endif ELSWORD_WAY_OF_SWORD

#ifdef ELSWORD_SHEATH_KNIGHT
	m_hSecondBladeSheath		= INVALID_MESH_INSTANCE_HANDLE;
	m_hThirdBladeSheathMesh		= INVALID_MESH_INSTANCE_HANDLE;

	m_fDamageDataChangeTime		= 0.f;
	m_fDamageDataChangeTime2	= 0.f;

	m_iETKComboXXXLoopCount		= 0;

	m_hSwordFallBladeFirst		= INVALID_MESH_INSTANCE_HANDLE;
	m_hSwordFallBladeSecond		= INVALID_MESH_INSTANCE_HANDLE;
	m_hSwordFallBladeThird		= INVALID_MESH_INSTANCE_HANDLE;
	m_hSwordFallBladeFourth		= INVALID_MESH_INSTANCE_HANDLE;
#ifdef ADDITIONAL_MEMO
	m_hSwordFallBladeFirst_Memo		= INVALID_MESH_INSTANCE_HANDLE;
	m_hSwordFallBladeSecond_Memo	= INVALID_MESH_INSTANCE_HANDLE;
	m_hSwordFallBladeThird_Memo		= INVALID_MESH_INSTANCE_HANDLE;
	m_hSwordFallBladeFourth_Memo	= INVALID_MESH_INSTANCE_HANDLE;
#endif


	m_fRollingHpConsumeRel			= 0.f;
	m_fRollingSpeedRel				= 0.f;
	m_fHarshChaserProjectiveRange	= 0.f;
	m_fMaelStormEffectiveTime		= 0.f;

	m_bCompactCounterEnable			= false;
	m_fCompactCounterCoolTime		= 0.f;
	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hFinalStrikeBlackHole			= INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_FinalStrikeBlackHole			= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SPECIAL_USE_ITEM
	m_fThrowStartTime		= 0.6666f;
	m_vThrowStartPosOffset	= D3DXVECTOR3(40.f, 130.f, 0.f);
	m_vThrowStartVelocity	= D3DXVECTOR3(2000.f, 100.f, 0.f);
#endif SPECIAL_USE_ITEM

#ifdef SERV_SKILL_NOTE
	m_bFirstAttackSuccess = false;
#endif
#ifdef NEW_SKILL_2010_11
	m_fMagicReflectRate = 0.f;
#endif NEW_SKILL_2010_11

#ifdef SERV_ELSWORD_INFINITY_SWORD
	m_fDashStateTime = 0.f;
	m_fMindOfFighterConsumeMpRate = 0.f;
	m_iBrutalSlayerActivatedLevel = 0;
	for(int i=0; i<3; ++i)
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hDamageSwordBlasting[i] = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pDamageSwordBlasting[i] = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    }
#endif
}




CX2GUElsword_SwordMan::~CX2GUElsword_SwordMan(void)
{
	SAFE_CLOSE( m_pSoundWindMill );
	
#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
	SAFE_DELETE_KTDGOBJECT( m_pMegaSlash );
	SAFE_DELETE_KTDGOBJECT( m_pMegaSlashBackDust );

	SAFE_DELETE_KTDGOBJECT( m_pFlameGager );
	SAFE_DELETE_KTDGOBJECT( m_pFlameGagerLand );


	SAFE_DELETE_KTDGOBJECT( m_pMagnumBlade );
	SAFE_DELETE_KTDGOBJECT( m_pMagnumBladeFire );

//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST


	DeleteElswordMajorParticle();
	DeleteElswordMinorParticle();

	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFire );
	
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMill );
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMillLand );
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hSwordFire );	
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hSwordEnchant );

#ifdef ELSWORD_SHEATH_KNIGHT
	g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hThirdBladeSheathMesh );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hFinalStrikeBlackHole != INVALID_DAMAGE_EFFECT_HANDLE )
	    g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hFinalStrikeBlackHole );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	g_pX2Game->GetDamageEffect()->DestroyInstance( m_FinalStrikeBlackHole );
    m_FinalStrikeBlackHole = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_ELSWORD_INFINITY_SWORD
	for(int i=0; i<3; ++i)
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( m_hDamageSwordBlasting[i] != INVALID_DAMAGE_EFFECT_HANDLE )
            g_pX2Game->GetDamageEffect()->DestroyInstanceHandle(m_hDamageSwordBlasting[i]);
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( m_pDamageSwordBlasting[i] != NULL )
			g_pX2Game->GetDamageEffect()->DestroyInstance(m_pDamageSwordBlasting[i] );
        m_pDamageSwordBlasting[i] = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}
#endif

}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUElsword_SwordMan::AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GUElsword_SwordMan::ReInit( bool bRandom, int startPosIndex )
{
	CX2GUUser::ReInit( bRandom, startPosIndex );
	m_fReDashJumpXTime		= 0.0f;

#ifdef ELSWORD_WAY_OF_SWORD

	if( IsMyUnit() == true && CX2Game::GT_PVP == g_pX2Game->GetGameType() )
	{
		const WAY_OF_SWORD_STATE eWayOfSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

		if( GetTeam() == CX2Room::TN_RED && g_pX2Game->GetRedTeamKill() > g_pX2Game->GetBlueTeamKill() ||
			GetTeam() == CX2Room::TN_BLUE && g_pX2Game->GetRedTeamKill() < g_pX2Game->GetBlueTeamKill() )
		{
			if( eWayOfSwordState == WSS_VIGOR )
			{
				End_SwordmanVigor();
			}
			else if( eWayOfSwordState == WSS_DESTRUCTION )
			{
				End_SwordmanDestruction();
			}
			SetWayOfSwordPoint( 0 );
			SetWayOfSwordState( static_cast<int>( WSS_CENTER ) );
			//m_GageManager.UpdateUIElswordWSP( WSS_CENTER );
		}
		else
		{
			if( eWayOfSwordState == WSS_VIGOR )
			{
				End_SwordmanVigor();
				SetWayOfSwordState( static_cast<int>( WSS_TOWARD_VIGOR ) );
				//m_GageManager.UpdateUIElswordWSP( WSS_TOWARD_VIGOR );
			}
			else if( eWayOfSwordState == WSS_DESTRUCTION )
			{
				End_SwordmanDestruction();
				SetWayOfSwordState( static_cast<int>( WSS_TOWARD_DESTRUCTION ) );
				//m_GageManager.UpdateUIElswordWSP( WSS_TOWARD_DESTRUCTION );
			}
		}
		m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
	}
	m_fDestructionRageTime = 0.f;

#endif ELSWORD_WAY_OF_SWORD
}

#ifdef ELSWORD_WAY_OF_SWORD
/*virtual*/ int	CX2GUElsword_SwordMan::GetWayOfSwordState() const
{ 
	return GetElswordGageData()->GetWayOfSwordState();
}

void CX2GUElsword_SwordMan::SetWayOfSwordState( const int iWayOfSwordState_ )
{ 
	GetElswordGageData()->SetWayOfSwordState( iWayOfSwordState_ );
}

/*virtual*/ float CX2GUElsword_SwordMan::GetWayOfSwordPoint() const
{
	return GetElswordGageData()->GetWayOfSwordPoint();
}

/*virtual*/ void CX2GUElsword_SwordMan::SetWayOfSwordPoint( float fWayOfSwordPoint_ )
{
	if( fWayOfSwordPoint_ > g_pData->GetDamageManager()->GetVigorStateThreshold() )
		fWayOfSwordPoint_ = g_pData->GetDamageManager()->GetVigorStateThreshold();

	if( fWayOfSwordPoint_ < g_pData->GetDamageManager()->GetDestStateThreshold() )
		fWayOfSwordPoint_ = g_pData->GetDamageManager()->GetDestStateThreshold();

	GetElswordGageData()->SetWayOfSwordPoint( fWayOfSwordPoint_ );
}

void CX2GUElsword_SwordMan::Start_SwordmanVigor()
{
	// 공속 증가 버프를 아이템 보정 받도록 ///////////////////////////////////////
	const float MAGIC_ANIM_UP_SPEED_FACTOR = 0.2f;
	float fAnimSpeedUpOriginal = 100.0f * CX2SocketItem::GetFinalAnimationSpeedPercent( m_SocketData.m_fAnimationSpeedRate, GetUnitLevel() );
	fAnimSpeedUpOriginal = fAnimSpeedUpOriginal / 100.f;
	
	float fAnimSpeedUpCorrection = fAnimSpeedUpOriginal;
	fAnimSpeedUpCorrection += MAGIC_ANIM_UP_SPEED_FACTOR;

	fAnimSpeedUpCorrection = CX2SocketItem::GetOptionRateCorrection( fAnimSpeedUpCorrection );

	fAnimSpeedUpOriginal = fAnimSpeedUpOriginal + 1.f;
	fAnimSpeedUpCorrection = fAnimSpeedUpCorrection + 1.f;

	m_fAnimSpeedUpByVigor = fAnimSpeedUpCorrection/fAnimSpeedUpOriginal;
	
	//////////////////////////////////////////////////////////////////////////

	// 이속 증가 버프를 아이템 보정 받도록 ///////////////////////////////////////
	const float MAGIC_MOVE_UP_SPEED_FACTOR = 0.2f;
	float fMoveSpeedUpOriginal = 1.0f + CX2SocketItem::GetFinalMoveSpeedPercent( m_SocketData.m_fMoveSpeed, GetUnitLevel() );
	float fMoveSpeedUpCorrection = fMoveSpeedUpOriginal + MAGIC_MOVE_UP_SPEED_FACTOR;
	fMoveSpeedUpCorrection = CX2SocketItem::GetOptionRateCorrection( fMoveSpeedUpCorrection );

	fMoveSpeedUpOriginal = fMoveSpeedUpOriginal + 1.f;
	fMoveSpeedUpCorrection = fMoveSpeedUpCorrection + 1.f;

	float fMoveSpeedUpRate = 1.f;
	fMoveSpeedUpRate = fMoveSpeedUpCorrection/fMoveSpeedUpOriginal;

	SetVecMoveSpeedFactor( fMoveSpeedUpRate,	99999.f, CX2GameUnit::SFI_SPEED_8, CX2GameUnit::SFST_UPDATE );
	//////////////////////////////////////////////////////////////////////////

	m_hEffectVigor = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_VigorStart", this );

	if ( NULL != g_pX2Game->GetWorld() && GetShowCutInAndChangeWorldColor() )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222255 );
		g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 4.0f );
	}

	PlaySound( L"elsword_vitality_use_VOX.ogg" );
	//PlaySound( L"elsword_vitality_loop.ogg", true, true );

	m_CommonState.m_Wait = ESSI_SWORD_WAIT;
	return;
}
void CX2GUElsword_SwordMan::End_SwordmanVigor()
{
	SetAnimSpeedFactor( 1.f,	0.f, CX2GameUnit::SFI_SPEED_8, CX2GameUnit::SFST_UPDATE );
	SetVecMoveSpeedFactor( 1.f, 0.f, CX2GameUnit::SFI_SPEED_8, CX2GameUnit::SFST_UPDATE );

	if ( NULL != g_pX2Game->GetWorld() )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectVigor );
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_VigorEnd", this ); 	
	}

	//g_pKTDXApp->GetDeviceManager()->StopSound( L"elsword_vitality_loop.ogg" );
	PlaySound( L"elsword_vitality_end.ogg" );

	m_CommonState.m_Wait = USI_WAIT;
	return;
}
void CX2GUElsword_SwordMan::Start_SwordmanDestruction()
{
	m_hEffectDestruction = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_DestructionStart", this );

	if ( NULL != g_pX2Game->GetWorld() && GetShowCutInAndChangeWorldColor() )
	{
		g_pX2Game->GetWorld()->SetWorldColor( 0xff552222 );
		g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 4.0f );
	}

	PlaySound( L"elsword_demolition_use_VOX.ogg" );
	//PlaySound( L"elsword_demolition_loop.ogg", true, true );

	m_CommonState.m_Wait = ESSI_SWORD_WAIT;
	return;
}
void CX2GUElsword_SwordMan::End_SwordmanDestruction()
{
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectDestruction );
	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_DestructionEnd", this ); 	

	//g_pKTDXApp->GetDeviceManager()->StopSound( L"elsword_demolition_loop.ogg" );
	PlaySound( L"elsword_demolition_end.ogg" );

	m_CommonState.m_Wait = USI_WAIT;
	return;
}
#endif ELSWORD_WAY_OF_SWORD
#ifdef ELSWORD_SHEATH_KNIGHT
void CX2GUElsword_SwordMan::AddComboToBrutalSlayer( const CX2DamageManager::DamageData& damageData_ )
{
	const int iComboNumber 
		= ( NULL != GetComboManager() ? GetComboManager()->GetCombo() : 0 );

	if ( iComboNumber > 0
		 && (iComboNumber % 5) == 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if( iSkillLevel > 0 )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER );
	#else // UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER, iSkillLevel );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			if( NULL != pSkillTemplet )
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}

		if ( m_iBrutalSlayerActivatedLevel > 0 )
		{
			++m_iBrutalSlayerActivatedLevel;
			if ( _CONST_ELSWORD_::CONST_BRUTAL_SLAYER_LEVEL < m_iBrutalSlayerActivatedLevel )
				m_iBrutalSlayerActivatedLevel = _CONST_ELSWORD_::CONST_BRUTAL_SLAYER_LEVEL;
		}
	}


}
#endif ELSWORD_SHEATH_KNIGHT

void CX2GUElsword_SwordMan::DamageReact( CX2DamageManager::DamageData* pDamageData )
{
	KTDXPROFILE();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	bool bCheckExtraDamage = false;

	switch( pDamageData->m_ExtraDamage.m_ExtraDamageType )
	{
	case CX2DamageManager::EDT_MANA_DAMAGE:
	case CX2DamageManager::EDT_DAMAGE_RATE:
	case CX2DamageManager::EDT_WATER_HOLD:
	case CX2DamageManager::EDT_ATTACK_ALL_TEAM:
		bCheckExtraDamage = true;
		break;
	default:
		break;
	}

	m_bAutoGuardBlocking = false;
	if( bCheckExtraDamage == false && CX2DamageManager::AT_NORMAL == pDamageData->attackType && pDamageData->m_bNoDamage == false 
		&& pDamageData->m_fHpPercentUp <= 0.f && pDamageData->m_fIncreaseMP <= 0.f 
		&& pDamageData->reActResult != CX2DamageManager::RT_DUMMY_DAMAGE
		&& pDamageData->reActResult != CX2DamageManager::RT_SENSOR && pDamageData->hitType != CX2DamageManager::HT_GROUND_HIT
		&& GetHold() == false && GetGrap() == false 
#ifdef RIDING_SYSTEM
		&& false == GetRidingOn()
#endif //RIDING_SYSTEM 
		)
	{
		if( true != IsSuperArmor() && true == GetFrameData().unitCondition.bFootOnLine )
		{

			if( true == m_FrameDataFuture.unitCondition.bAttackerFront )
			{
				if(  GetRandomFloat() < m_fAutoGuardRate )
					m_bAutoGuardBlocking = true;
			}
			else
			{
				m_bIsGuardMotion = false;
			}


			if( true == m_bIsGuardMotion )
			{
				int GuardDamageReducePercent = 0;
				if( NULL != m_GuardForElswordPtr )
					GuardDamageReducePercent = m_GuardForElswordPtr->GetDamageReducePercent();

				SetDamageReducePercent( GetDamageReducePercent() + GuardDamageReducePercent );
				CX2GUUser::DamageReact( pDamageData );
				SetDamageReducePercent( GetDamageReducePercent() - GuardDamageReducePercent );

				StateChange( CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_DAMAGE, true );

				return;
			}

		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef ELSWORD_WAY_OF_SWORD
	if( GetWayOfSwordState() == WSS_DESTRUCTION &&
		IsSuperArmor() == false &&
		m_fDestructionRageTime <= 0.f &&
		pDamageData->reActType != CX2DamageManager::RT_NO_DAMAGE &&
		pDamageData->reActType != CX2DamageManager::RT_DUMMY_DAMAGE &&
		pDamageData->reActType != CX2DamageManager::RT_SENSOR &&
		GetInvincible() == false &&
		GetNowHp() > 0.f &&
		m_FrameDataNow.unitCondition.bFootOnLine == true )
	{
		const int DAMAGE_REDUCE_PERCENT_DESTRUCTION_RAGE = 100;
		SetDamageReducePercent( GetDamageReducePercent() + DAMAGE_REDUCE_PERCENT_DESTRUCTION_RAGE );
		CX2GUUser::DamageReact( pDamageData );
		SetDamageReducePercent( GetDamageReducePercent() - DAMAGE_REDUCE_PERCENT_DESTRUCTION_RAGE );

		if(	pDamageData->reActResult == CX2DamageManager::RT_REVENGE ||
			pDamageData->reActResult == CX2DamageManager::RT_REFLECT )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			DamageReactStateChange( pDamageData, ESSI_REVENGE_ATTACK );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2GUUser::DamageReactStateChange( pDamageData, ESSI_REVENGE_ATTACK );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			SetHoldStateID( USI_DAMAGE_AIR_FALL );
			DoStateDestructionRage();
		}
	}
	else if( IsSuperArmor() )
	{
		SetDamageReducePercent( GetDamageReducePercent() );
		CX2GUUser::DamageReact( pDamageData );
		SetDamageReducePercent( GetDamageReducePercent() );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		DamageReactStateChange( pDamageData, ESSI_REVENGE_ATTACK );
#else //UPGRADE_SKILL_SYSTEM_2013
		CX2GUUser::DamageReactStateChange( pDamageData, ESSI_REVENGE_ATTACK );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
	else
	{
		CX2GUUser::DamageReact( pDamageData );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		DamageReactStateChange( pDamageData, ESSI_REVENGE_ATTACK );
#else //UPGRADE_SKILL_SYSTEM_2013
		CX2GUUser::DamageReactStateChange( pDamageData, ESSI_REVENGE_ATTACK );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	if(	IsMyUnit() == true &&
		pDamageData->reActType != CX2DamageManager::RT_NO_DAMAGE &&
		pDamageData->reActType != CX2DamageManager::RT_DUMMY_DAMAGE &&
		pDamageData->reActType != CX2DamageManager::RT_SENSOR &&
		pDamageData->reActResult != CX2DamageManager::RT_REVENGE &&
		GetInvincible() == false &&
		GetNowHp() > 0.f )
	{
		int iWayOfSwordState = GetWayOfSwordState();
		float fWSPGain = 0.f;

		fWSPGain = g_pData->GetDamageManager()->CalcWayOfSwordPointOnDamageReact( pDamageData->damage.GetAllDamage(), iWayOfSwordState );

		SetWayOfSwordPoint( GetWayOfSwordPoint() + fWSPGain );
	}


#else
	CX2GUUser::DamageReact( pDamageData );

	CX2GUUser::DamageReactStateChange( pDamageData, ESSI_REVENGE_ATTACK );
#endif ELSWORD_WAY_OF_SWORD
}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
#ifdef ARA_CHANGE_CLASS_FIRST
/*virtual*/ void CX2GUElsword_SwordMan::DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack/* = -1*/, int iAvoidance/* = -1 */)
#else
/*virtual*/ void CX2GUElsword_SwordMan::DamageReactStateChange( CX2DamageManager::DamageData* pDamageData, int iRevengeAttack/* = -1*/ )
#endif //ARA_CHANGE_CLASS_FIRST
{
	if( true == m_bAutoGuardBlocking )
	{
		m_bAutoGuardBlocking = false;

		StateChange( CX2GUElsword_SwordMan::ESSI_SI_P_ESK_AUTO_GUARD , true );

		return;
	}


	CX2GUUser::DamageReactStateChange( pDamageData,  iRevengeAttack, iAvoidance );
}

/*virtual*/ float CX2GUElsword_SwordMan::GetFinalDamageToBeChangedByActiveSkill( float fFinalDamage_ )
{

	return fFinalDamage_ * m_fActiveSkillAddPowerRate;
}

void CX2GUElsword_SwordMan::ApplyBuffToHarshSlayer( const CX2DamageManager::DamageData& damageData_ )
{
	if( CX2DamageManager::AT_NORMAL == damageData_.attackType )
	{	
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillLevelHarshSlayer = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_HARSH_SLAYER, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const int iSkillLevelHarshSlayer = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_HARSH_SLAYER );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if( iSkillLevelHarshSlayer > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_HARSH_SLAYER );
			if( NULL != pSkillTemplet )
			{
				const float SkillRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevelHarshSlayer );
				const float RandomRate = GetRandomFloat();
				if( SkillRate > RandomRate )
				{
					if( pSkillTemplet->m_vecBuffFactorPtr.size() > 2 )
					{
						SetBuffFactorToGameUnit( pSkillTemplet, 1 );
						SetBuffFactorToGameUnit( pSkillTemplet, 2 );
						CX2GameUnit*	pDefenderUnit = NULL;
						if( null != damageData_.optrDefenderGameUnit )
						{
							pDefenderUnit = damageData_.optrDefenderGameUnit.GetObservable();
							if( NULL != pDefenderUnit )
							{
								pDefenderUnit->SetBuffFactorToGameUnit( *(pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillLevelHarshSlayer )) );

								//승부사의 기질을 배웠을 경우를 체크 해줘야 함.
		#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
								const int iSkillLevelMindOfFighter = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_MIND_OF_FIGHTER, true );
		#else // UPGRADE_SKILL_SYSTEM_2013
								const int iSkillLevelMindOfFighter = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_MIND_OF_FIGHTER );
		#endif // UPGRADE_SKILL_SYSTEM_2013
								
								if( iSkillLevelMindOfFighter > 0 )
								{
									pDefenderUnit->SetBuffFactorToGameUnitByBuffFactorID(static_cast<UINT>( BFI_DEBUFF_SI_P_EIS_MIND_OF_FIGHTER ), iSkillLevelMindOfFighter );
								}
							}
						}
					}	
				}
			}
		}	
	}
}
#endif //UPGRADE_SKILL_SYSTEM_2013	

void CX2GUElsword_SwordMan::Win()
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

void CX2GUElsword_SwordMan::Lose()
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

void CX2GUElsword_SwordMan::Draw()
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


void CX2GUElsword_SwordMan::InitStateID()
{
	m_MachineGunWaitState	= ESSI_MACHINE_GUN_WAIT;
	m_ChargeMpState			= USI_WAIT;

	m_CommonState.m_Wait			= USI_WAIT;
#ifdef GRAPPLING_TEST
	m_CommonState.m_GrappledFront	= USI_DAMAGE_GRAPPLED_FRONT;
	m_CommonState.m_GrappledBack	= USI_DAMAGE_GRAPPLED_FRONT;
#endif // GRAPPLING_TEST
	m_CommonState.m_Thrown			= USI_DAMAGE_AIR_FLY_FRONT;

#ifdef CLIFF_CLIMBING_TEST
	m_CommonState.m_ClimbWait			= ESSI_CLIMB_WAIT;
	m_CommonState.m_ClimbFront			= ESSI_CLIMB_FRONT;
	m_CommonState.m_ClimbUp				= ESSI_CLIMB_UP;
	m_CommonState.m_ClimbUpFront		= ESSI_CLIMB_UP_FRONT;
	m_CommonState.m_ClimbDown			= ESSI_CLIMB_DOWN;
	m_CommonState.m_ClimbDownFront		= ESSI_CLIMB_DOWN_FRONT;
#endif CLIFF_CLIMBING_TEST

#ifdef LINKED_LINEMAP_JUMP_USER_TEST
	m_CommonState.m_LinkJumpState		= ESSI_LINK_JUMP;
#endif LINKED_LINEMAP_JUMP_USER_TEST

//#ifdef TRANSFORMER_TEST
//	m_CommonState.m_Transformed			= ESSI_TRANSFORMED;
//#endif TRANSFORMER_TEST




}


void CX2GUElsword_SwordMan::InitState()
{
	ASSERT( NULL != GetUnit() );

	const CX2Unit::UnitData* pUnitData = &GetUnit()->GetUnitData();


	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
	case CX2Unit::UC_ELSWORD_KNIGHT:
	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
#endif ELSWORD_SHEATH_KNIGHT
		{
			m_iSkillCutInSetIndex = 0;
		} break;
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
		{
			m_iSkillCutInSetIndex = 1;
		} break;
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
		{
			m_iSkillCutInSetIndex = 2;
		} break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
		{
			m_iSkillCutInSetIndex = 3;
		} break;
#endif
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
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, WinFrameMove );
	//stateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, WinCameraMove );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_LOSE;
	m_LuaManager.MakeTableReference( tableNameLoseUTF8.c_str(), stateData.stateID );
#ifdef SERV_PET_SYSTEM
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, LoseStateStart );	
#endif
	m_StateList[stateData.stateID] = stateData;



	// 매직 나이트 win모션 예외 처리
	if( 0 == tableNameWinUTF8.compare( "ESSI_EMK_WIN2" ) )
	{
		m_bIsMagicKnightWinMotion2 = true;
	}

	// 보기 쉽게 콤보 별로 variation이 있는 것에 대해서 switch 문을 따로 만들자
	// COMBO_ZZ 계열
#ifdef ELSWORD_SHEATH_KNIGHT
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
#endif
		{
			stateData.Init();
			stateData.stateID			= ESSI_ETK_COMBO_ZZZ;
			m_LuaManager.MakeTableReference( "ESSI_ETK_COMBO_ZZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_ETK_COMBO_ZZZZ;
			m_LuaManager.MakeTableReference( "ESSI_ETK_COMBO_ZZZZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_ETK_COMBO_ZZZZZ;
			m_LuaManager.MakeTableReference( "ESSI_ETK_COMBO_ZZZZZ", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZZZ_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_ZZZZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

		} break;
	}

	// COMBO_XX 계열
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
#endif
		{
			stateData.Init();
			stateData.stateID			= ESSI_ETK_COMBO_XX;
			m_LuaManager.MakeTableReference( "ESSI_ETK_COMBO_XX", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XX_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XX_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XX_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_ETK_COMBO_XXX_LOOP;
			m_LuaManager.MakeTableReference( "ESSI_ETK_COMBO_XXX_LOOP", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_LOOP_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_LOOP_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_LOOP_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_LOOP_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_ETK_COMBO_XXXZ;
			m_LuaManager.MakeTableReference( "ESSI_ETK_COMBO_XXXZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXXZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXXZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXXZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_ETK_COMBO_XXX_END;
			m_LuaManager.MakeTableReference( "ESSI_ETK_COMBO_XXX_END", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_END_StateStart );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_END_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_END_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_COMBO_XXX_END_StateEnd );
			m_StateList[stateData.stateID] = stateData;

		} break;
	}
#endif ELSWORD_SHEATH_KNIGHT




	// dash_combo_zz 계열
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
		{
			stateData.Init();
			stateData.stateID			= ESSI_DASH_COMBO_Z2;
			m_LuaManager.MakeTableReference( "ESSI_DASH_COMBO_Z2", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashComboZ2StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashComboZ2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashComboZ2EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_ELSWORD_KNIGHT:
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
		{
			stateData.Init();
			stateData.stateID			= ESSI_DASH_COMBO_Z2;
			m_LuaManager.MakeTableReference( "ESSI_KNIGHT_DASH_COMBO_Z2", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z2_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_DASH_COMBO_Z3;
			m_LuaManager.MakeTableReference( "ESSI_KNIGHT_DASH_COMBO_Z3", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z3_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z3_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_DASH_COMBO_Z3a;
			m_LuaManager.MakeTableReference( "ESSI_KNIGHT_DASH_COMBO_Z3a", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z3a_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z3a_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;


	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
		{
			stateData.Init();
			stateData.stateID			= ESSI_DASH_COMBO_Z2;
			m_LuaManager.MakeTableReference( "ESSI_KNIGHT_DASH_COMBO_Z2", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z2_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_COMBO_Z2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_DASH_COMBO_Z3;
			m_LuaManager.MakeTableReference( "ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_DASH_COMBO_Z3a;
			m_LuaManager.MakeTableReference( "ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_StateEndFuture );
			m_StateList[stateData.stateID] = stateData;
		} break;

#ifdef ELSWORD_SHEATH_KNIGHT	
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
#endif
		{
			stateData.Init();
			stateData.stateID			= ESSI_ETK_DASH_COMBO_ZZ;
			m_LuaManager.MakeTableReference( "ESSI_ETK_DASH_COMBO_ZZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_DASH_COMBO_ZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_DASH_COMBO_ZZ_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ETK_DASH_COMBO_ZZ_StateEnd );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif ELSWORD_SHEATH_KNIGHT
	}



	
	// dash_jump_combo_XX 계열
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
#endif ELSWORD_SHEATH_KNIGHT
		{
			stateData.Init();
			stateData.stateID			= ESSI_DASH_JUMP_COMBO_X2;
			m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2End );
			m_StateList[stateData.stateID] = stateData;


		} break;


	case CX2Unit::UC_ELSWORD_KNIGHT:
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
		{
			stateData.Init();
			stateData.stateID			= ESSI_DASH_JUMP_COMBO_X2;
			m_LuaManager.MakeTableReference( "ESSI_KNIGHT_DASH_JUMP_COMBO_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_JUMP_COMBO_X2_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_JUMP_COMBO_X2_FrameMove ); 
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_JUMP_COMBO_X2_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_JUMP_COMBO_X2_End );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESSI_DASH_JUMP_COMBO_X3;
			m_LuaManager.MakeTableReference( "ESSI_KNIGHT_DASH_JUMP_COMBO_X3", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_KNIGHT_DASH_JUMP_COMBO_X3_EventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;

	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
		{
			stateData.Init();
			stateData.stateID			= ESSI_DASH_JUMP_COMBO_X2;
			m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2End );
			m_StateList[stateData.stateID] = stateData;




			stateData.Init();
			stateData.stateID			= ESSI_DASH_JUMP_COMBO_X2a;
			m_LuaManager.MakeTableReference( "ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_FrameMove ); 
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_EventProcess );				
			m_StateList[stateData.stateID] = stateData;
		} break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
		{
			stateData.Init();
			stateData.stateID			= ESSI_DASH_JUMP_COMBO_X2;
			m_LuaManager.MakeTableReference( "ESSI_EIS_DASHJUMP_COMBO_XX", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_EIS_DASHJUMP_COMBO_XX_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2End );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_EIS_DASHJUMP_COMBO_XXX;
			m_LuaManager.MakeTableReference( "ESSI_EIS_DASHJUMP_COMBO_XXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_EIS_DASHJUMP_COMBO_XXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_EIS_DASHJUMP_COMBO_XXX_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_EIS_DASHJUMP_COMBO_XXX_End );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
#endif
	}







	
	
	
	
	
	// 일단 2차전직에만 있는 콤보는 요렇게 작업
	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
		{
			stateData.Init();
			stateData.stateID			= ESSI_ELK_ZZZfrontZ;
			m_LuaManager.MakeTableReference( "ESSI_ELK_ZZZfrontZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ELK_ZZZfrontZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ELK_ZZZfrontZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESSI_ELK_XXX;
			m_LuaManager.MakeTableReference( "ESSI_ELK_XXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ELK_XXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ELK_XXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESSI_ELK_XXXX;
			m_LuaManager.MakeTableReference( "ESSI_ELK_XXXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ELK_XXXX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ELK_XXXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;


	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
		{

			stateData.Init();
			stateData.stateID			= ESSI_ERS_XZ;
			m_LuaManager.MakeTableReference( "ESSI_ERS_XZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_XZ_StateStartFuture );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_XZ_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_XZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESSI_ERS_XZZ;
			m_LuaManager.MakeTableReference( "ESSI_ERS_XZZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_XZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_XZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;



			stateData.Init();
			stateData.stateID			= ESSI_ERS_DASH_JUMP_XZ;
			m_LuaManager.MakeTableReference( "ESSI_ERS_DASH_JUMP_XZ", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_DASH_JUMP_XZ_StateStartFuture );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_DASH_JUMP_XZ_StateEndFuture );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_DASH_JUMP_XZ_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_DASH_JUMP_XZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_DASH_JUMP_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESSI_ERS_DASH_JUMP_XZZ;
			m_LuaManager.MakeTableReference( "ESSI_ERS_DASH_JUMP_XZZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_DASH_JUMP_XZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_ERS_DASH_JUMP_XZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
		{
			stateData.Init();
			stateData.stateID			= ESSI_EIS_DASH_COMBO_XX;
			m_LuaManager.MakeTableReference( "ESSI_EIS_DASH_COMBO_XX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_EIS_DASH_COMBO_XX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_EIS_DASH_COMBO_XXX;
			m_LuaManager.MakeTableReference( "ESSI_EIS_DASH_COMBO_XXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_EIS_DASH_COMBO_XXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;

#endif
	}




	InitPassiveSkillState();
	InitEquippedActiveSkillState();


	CX2GUUser::InitState();
}

void CX2GUElsword_SwordMan::InitStateCommon()
{

	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "ESSI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DieBackEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "ESSI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "ESSI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, WaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, WaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, WaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "ESSI_WALK", stateData.stateID );
#ifdef TEST_MACRO_COMBO
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, WalkStart );
#endif TEST_MACRO_COMBO
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, WalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "ESSI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpReadyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESSI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser,			JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser,			JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "ESSI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "ESSI_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashEndStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpPowerLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpPowerLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpPowerLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ESSI_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, HyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, HyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, HyperModeEnd );
#endif HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageSmallFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageSmallBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageBigFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageBigFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageBigBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageBigBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageDownFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageDownFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageDownBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageDownBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DamageAirDownLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ROLLING_FRONT_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ROLLING_FRONT_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingFrontFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingFrontFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ROLLING_FRONT_BACK;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ROLLING_FRONT_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingFrontBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingFrontBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ROLLING_BACK_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ROLLING_BACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingBackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingBackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ROLLING_BACK_BACK;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ROLLING_BACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingBackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpRollingBackBackEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ATTACK_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ATTACK_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackFrontEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackFrontEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ATTACK_BACK;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ATTACK_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackBackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackBackEnd );
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ATTACK_FRONT_NO_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ATTACK_FRONT_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackFrontNoDownEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackFrontNoDownEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_STANDUP_ATTACK_BACK_NO_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_STANDUP_ATTACK_BACK_NO_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackBackNoDownEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, StandUpAttackBackNoDownEnd );
	m_StateList[stateData.stateID] = stateData;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	stateData.Init();
	stateData.stateID			= ESSI_SI_P_ESK_AUTO_GUARD;
	m_LuaManager.MakeTableReference( "ESSI_SI_P_ESK_AUTO_GUARD", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_P_ESK_AUTO_GUARD_Start );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_P_ESK_AUTO_GUARD_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_P_ESK_AUTO_GUARD_End );
	m_StateList[stateData.stateID] = stateData;
#endif //UPGRADE_SKILL_SYSTEM_2013


#ifdef ELSWORD_SHEATH_KNIGHT
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER, true );
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER );
#endif //UPGRADE_SKILL_SYSTEM_2013
	if( iSkillLevel > 0 )
	{
		stateData.Init();
		stateData.stateID			= ESSI_REVENGE_ATTACK;
		m_LuaManager.MakeTableReference( "ESSI_SI_A_ETK_COMPACT_COUNTER_GUARD_ATTACK", stateData.stateID );
		stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, CompactCounterGuardAttackStart );
		stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, CompactCounterGuardAttackStartFuture );
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, CompactCounterGuardAttackFrameMoveFuture );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, CompactCounterGuardAttackFrameMove );
		stateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, CompactCounterGuardAttackCameraMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, CompactCounterGuardAttackEventProcess );
		stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, CompactCounterGuardAttackStateEnd );
		m_StateList[stateData.stateID] = stateData;
	}
	else
	{
		stateData.Init();
		stateData.stateID			= ESSI_REVENGE_ATTACK;
		m_LuaManager.MakeTableReference( "ESSI_REVENGE_ATTACK", stateData.stateID );
		stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackStart );
		stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackFrameMoveFuture );
		stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackFrameMove );
		stateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackCameraMove );
		stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackEventProcess );
		m_StateList[stateData.stateID] = stateData;
	}
#else
	stateData.Init();
	stateData.stateID			= ESSI_REVENGE_ATTACK;
	m_LuaManager.MakeTableReference( "ESSI_REVENGE_ATTACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackFrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, RevengeAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif ELSWORD_SHEATH_KNIGHT

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_Z1;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_Z1", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ1FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ1EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_Z2;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_Z2", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ2FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ2EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_Z3;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_Z3", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ3FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ3EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_Z4;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_Z4", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ4FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ4FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ4EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_Z4a;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_Z4a", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ4aFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboZ4aEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_X1;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_X1", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX1FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX1EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_X2;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_X2", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX2FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX2FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX2EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_X3;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_X3", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX3FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX3FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX3EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_COMBO_X3a;
	m_LuaManager.MakeTableReference( "ESSI_COMBO_X3a", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX3aFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ComboX3aEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_DASH_COMBO_Z1;
	m_LuaManager.MakeTableReference( "ESSI_DASH_COMBO_Z1", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashComboZ1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashComboZ1EventProcess );
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= ESSI_DASH_ATTACK_X;
	m_LuaManager.MakeTableReference( "ESSI_DASH_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashAttackXStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashAttackXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESSI_JUMP_ATTACK_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpAttackZFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpAttackZFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "ESSI_JUMP_ATTACK_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpAttackXFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpAttackXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, JumpAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_DASH_JUMP_COMBO_Z1;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_Z1", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ1EventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= ESSI_DASH_JUMP_COMBO_Z2;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_Z2", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ2FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ2EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_DASH_JUMP_COMBO_Z3;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_Z3", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ3FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ3EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_DASH_JUMP_COMBO_Z3_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_Z3_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ3LandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ3LandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboZ3LandingEventProcess );
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= ESSI_DASH_JUMP_COMBO_X1;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_X1", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX1EventProcess );
	m_StateList[stateData.stateID] = stateData;






	stateData.Init();
	stateData.stateID			= ESSI_DASH_JUMP_COMBO_X2_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_DASH_JUMP_COMBO_X2_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2LandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2LandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DashJumpComboX2LandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef ELSWORD_WAY_OF_SWORD

	stateData.Init();
	stateData.stateID			= ESSI_SWORD_WAIT;
	m_LuaManager.MakeTableReference( "ESSI_SWORD_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, SwordWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, SwordWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, SwordWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= ESSI_DESTRUCTION_RAGE;
	m_LuaManager.MakeTableReference( "ESSI_DESTRUCTION_RAGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DestructionRageStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DestructionRageStateFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DestructionRageStateEventProcess );
	m_StateList[stateData.stateID] = stateData;


#endif ELSWORD_WAY_OF_SWORD


#ifdef WALL_JUMP_TEST
	stateData.Init();
	stateData.stateID			= ESSI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;
#endif WALL_JUMP_TEST




	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "ESSI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "ESSI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "ESSI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESSI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef SPECIAL_USE_ITEM
	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "ESSI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );	
	m_StateList[stateData.stateID] = stateData;
#else SPECIAL_USE_ITEM
	stateData.Init();
	stateData.stateID			= ESSI_THROW_WOODEN_PIECE;
	m_LuaManager.MakeTableReference( "ESSI_THROW_WOODEN_PIECE", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_WOODEN_PIECE_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, THROW_WOODEN_PIECE_FrameMove );
#ifdef CONVERSION_VS
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_WOODEN_PIECE_FrameMoveFuture );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_WOODEN_PIECE_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_WOODEN_PIECE_EventProcess );	
#else CONVERSION_VS
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, THROW_WOODEN_PIECE_FrameMoveFuture );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, THROW_WOODEN_PIECE_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, THROW_WOODEN_PIECE_EventProcess );	
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;
#endif SPECIAL_USE_ITEM

#ifdef RIDING_SYSTEM
	#pragma region ESSI_RIDING_START
	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "ESSI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_ON
	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_OFF
	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_WAIT_HABIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_WAIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_WALK
	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
	
	#pragma region ESSI_RIDING_JUMP_UP
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_DASH
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_DASH_END
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_DASH_JUMP
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_DASH_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_DAMAGE_FRONT
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_DAMAGE_BACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_DIE
	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_JUMP_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_ATTACK_X
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_SPECIAL_ATTACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region ESSI_RIDING_SPECIAL_MOVE
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "ESSI_RIDING_SPECIAL_MOVE", stateData.stateID );
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
	m_LuaManager.MakeTableReference( "ESSI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, RidingHyperModeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingHyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingHyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingHyperModeEnd );
#endif HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;
#endif // MODIFY_RIDING_PET_AWAKE

	stateData.Init();
	stateData.stateID			= ESSI_MACHINE_GUN_RIDE;
	m_LuaManager.MakeTableReference( "ESSI_MACHINE_GUN_RIDE", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.StateStartFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_RIDE_StartFuture ); 
	stateData.StateStart			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_RIDE_Start ); 
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_RIDE_FrameMoveFuture ); 
	stateData.OnFrameMove			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_RIDE_FrameMove ); 
	stateData.OnCameraMove			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_RIDE_CameraMove ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_RIDE_EventProcess ); 
#else CONVERSION_VS
	stateData.StateStartFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_RIDE_StartFuture ); 
	stateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_RIDE_Start ); 
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_RIDE_FrameMoveFuture ); 
	stateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_RIDE_FrameMove ); 
	stateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_RIDE_CameraMove ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_RIDE_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_MACHINE_GUN_WAIT;
	m_LuaManager.MakeTableReference( "ESSI_MACHINE_GUN_WAIT", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.StateStartFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_WAIT_StartFuture ); 
	stateData.StateStart			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_WAIT_Start ); 
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_WAIT_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_WAIT_EventProcess );
#else CONVERSION_VS
	stateData.StateStartFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_WAIT_StartFuture ); 
	stateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_WAIT_Start ); 
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_WAIT_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_WAIT_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_MACHINE_GUN_WALK;
	m_LuaManager.MakeTableReference( "ESSI_MACHINE_GUN_WALK", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_WALK_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_WALK_EventProcess ); 
#else CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_WALK_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_WALK_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_MACHINE_GUN_ATTACK;
	m_LuaManager.MakeTableReference( "ESSI_MACHINE_GUN_ATTACK", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_ATTACK_FrameMoveFuture ); 
	stateData.OnFrameMove			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_ATTACK_FrameMove );
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_ATTACK_EventProcess ); 
#else CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_ATTACK_FrameMoveFuture ); 
	stateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_ATTACK_FrameMove );
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_ATTACK_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= ESSI_MACHINE_GUN_LEAVE;
	m_LuaManager.MakeTableReference( "ESSI_MACHINE_GUN_LEAVE", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.StateStartFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_LEAVE_StartFuture ); 
	stateData.StateStart			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_LEAVE_Start ); 
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_LEAVE_FrameMoveFuture ); 
	stateData.OnFrameMove			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_LEAVE_FrameMove ); 
	stateData.OnCameraMove			= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_LEAVE_CameraMove ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_LEAVE_EventProcess ); 
#else CONVERSION_VS
	stateData.StateStartFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_LEAVE_StartFuture ); 
	stateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_LEAVE_Start ); 
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_LEAVE_FrameMoveFuture ); 
	stateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_LEAVE_FrameMove ); 
	stateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_LEAVE_CameraMove ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_LEAVE_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_MACHINE_GUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "ESSI_MACHINE_GUN_JUMP_UP", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_JUMP_UP_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_JUMP_UP_EventProcess ); 
#else CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_JUMP_UP_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_JUMP_UP_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_MACHINE_GUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_MACHINE_GUN_JUMP_DOWN", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_JUMP_DOWN_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, MACHINE_GUN_JUMP_DOWN_EventProcess ); 
#else CONVERSION_VS
	stateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_JUMP_DOWN_FrameMoveFuture ); 
	stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, MACHINE_GUN_JUMP_DOWN_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= ESSI_SUMMON_MAGIC_SPEAR;
	m_LuaManager.MakeTableReference( "ESSI_SUMMON_MAGIC_SPEAR", stateData.stateID );
#ifdef CONVERSION_VS
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, SUMMON_MAGIC_SPEAR_FrameMove ); 
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, SUMMON_MAGIC_SPEAR_EventProcess ); 
#else CONVERSION_VS
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, SUMMON_MAGIC_SPEAR_FrameMove ); 
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, SUMMON_MAGIC_SPEAR_EventProcess ); 
#endif CONVERSION_VS
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= ESSI_LEAP;
	m_LuaManager.MakeTableReference( "ESSI_LEAP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_LEAP_StartFuture ); 
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_LEAP_FrameMoveFuture ); 
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_LEAP_EventProcess ); 
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_LEAP_End ); 
	m_StateList[stateData.stateID] = stateData;





#ifdef GRAPPLING_TEST



	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GRAPPLED_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_GRAPPLED_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_StartFuture ); 
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_Start ); 
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_FrameMoveFuture ); 
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_EventProcess ); 
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_EndFuture ); 
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_End ); 
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_RIDING_HYPER_MODE;
	m_LuaManager.MakeTableReference( "ESSI_DAMAGE_GRAPPLED_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_StartFuture ); 
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_Start ); 
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_FrameMoveFuture ); 
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_EventProcess ); 
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_EndFuture ); 
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_DAMAGE_GRAPPLED_FRONT_End ); 
	m_StateList[stateData.stateID] = stateData;




	stateData.Init();
	stateData.stateID			= ESSI_GRAB_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_GRAB_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_GRAB_FRONT_StartFuture ); 
	stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_GRAB_FRONT_Start ); 
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_GRAB_FRONT_FrameMoveFuture ); 
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_GRAB_FRONT_EventProcess ); 
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_GRAB_FRONT_End ); 
	m_StateList[stateData.stateID] = stateData;




	stateData.Init();
	stateData.stateID			= ESSI_THROW_FORWARD;
	m_LuaManager.MakeTableReference( "ESSI_THROW_FORWARD", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_THROW_FORWARD_StartFuture ); 
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_THROW_FORWARD_FrameMoveFuture ); 
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_THROW_FORWARD_EventProcess ); 
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_THROW_FORWARD_EndFuture ); 
	stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_THROW_FORWARD_End); 
	m_StateList[stateData.stateID] = stateData;


#endif GRAPPLING_TEST




//#ifdef PVP_BOSS_COMBAT_TEST
//
//	stateData.Init();
//	stateData.stateID			= ESSI_FROZEN;
//	m_LuaManager.MakeTableReference( "ESSI_FROZEN", stateData.stateID );
//	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, Frozen_StateStart ); 
//	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, Frozen_EventProcess );
//	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, Frozen_StateEnd ); 
//	m_StateList[stateData.stateID] = stateData;
//
//
//	m_FrozenState = ESSI_FROZEN;
//
//#endif PVP_BOSS_COMBAT_TEST






#ifdef LINKED_LINEMAP_JUMP_USER_TEST
	stateData.Init();
	stateData.stateID			= ESSI_LINK_JUMP;
	m_LuaManager.MakeTableReference( "ESSI_LINK_JUMP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, LinkJumpEventProcess );	
	m_StateList[stateData.stateID] = stateData;
#endif LINKED_LINEMAP_JUMP_USER_TEST





#ifdef CLIFF_CLIMBING_TEST

	stateData.Init();
	stateData.stateID			= ESSI_CLIMB_WAIT;
	m_LuaManager.MakeTableReference( "ESSI_CLIMB_WAIT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, ClimbWaitEventProcess );	
	m_StateList[stateData.stateID] = stateData;





	stateData.Init();
	stateData.stateID			= ESSI_CLIMB_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_CLIMB_FRONT", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, ClimbFrontFrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, ClimbFrontEventProcess );	
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= ESSI_CLIMB_UP;
	m_LuaManager.MakeTableReference( "ESSI_CLIMB_UP", stateData.stateID );
	//stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, ClimbFrontFrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, ClimbUpEventProcess );	
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= ESSI_CLIMB_DOWN;
	m_LuaManager.MakeTableReference( "ESSI_CLIMB_DOWN", stateData.stateID );
	//stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, ClimbFrontFrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, ClimbDownEventProcess );	
	m_StateList[stateData.stateID] = stateData;





	stateData.Init();
	stateData.stateID			= ESSI_CLIMB_UP_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_CLIMB_UP_FRONT", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, ClimbUpFrontFrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, ClimbUpFrontEventProcess );	
	m_StateList[stateData.stateID] = stateData;



	stateData.Init();
	stateData.stateID			= ESSI_CLIMB_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "ESSI_CLIMB_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, ClimbDownFrontFrameMoveFuture );	
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, ClimbDownFrontEventProcess );	
	m_StateList[stateData.stateID] = stateData;



#endif CLIFF_CLIMBING_TEST





//#ifdef TRANSFORMER_TEST
//
//	stateData.Init();
//	stateData.stateID			= ESSI_TRANSFORMED;
//	m_LuaManager.MakeTableReference( "ESSI_TRANSFORMED", stateData.stateID );
//	m_StateList[stateData.stateID] = stateData;
//
//
//#endif TRANSFORMER_TEST


}





/*virtual*/ void CX2GUElsword_SwordMan::InitPassiveSkillState()
{
	CX2GUUser::InitPassiveSkillState();

#ifdef BALANCE_PATCH_20131107
	m_iSkillLevelInduranceOfRevenge = 0;
#endif //BALANCE_PATCH_20131107

	int iSkillLevel = 0;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP, true );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP );

		if( NULL != pSkillTemplet )
		{
			const float fMoveSpeedUp = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MOVE_SPEED, iSkillLevel );			
			m_AdditionalWalkSpeed.SetPassiveMultiple( m_AdditionalWalkSpeed.GetPassiveMultiple() * fMoveSpeedUp );
			m_AdditionalDashSpeed.SetPassiveMultiple( m_AdditionalDashSpeed.GetPassiveMultiple() * fMoveSpeedUp );
			InsertDelegateProcessToCommonFrameMove( SET_DELEGATE_PROCESS( CX2GUElsword_SwordMan, LightingStepFrameMove ) );
		}
	}
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			const float fMoveSpeedUp = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MOVE_SPEED );			
			m_AdditionalWalkSpeed.SetPassiveMultiple( m_AdditionalWalkSpeed.GetPassiveMultiple() * fMoveSpeedUp );
			m_AdditionalDashSpeed.SetPassiveMultiple( m_AdditionalDashSpeed.GetPassiveMultiple() * fMoveSpeedUp );
			InsertDelegateProcessToCommonFrameMove( SET_DELEGATE_PROCESS( CX2GUElsword_SwordMan, LightingStepFrameMove ) );
		}
	}
#endif // UPGRADE_SKILL_SYSTEM_2013
	
#endif

	m_fAttackPowerRateFireBall = 1.f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ES_POWERFUL_FIREBALL, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ES_POWERFUL_FIREBALL );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ES_POWERFUL_FIREBALL );

		if( NULL != pSkillTemplet )
		{
			m_fAttackPowerRateFireBall *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevel );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ES_POWERFUL_FIREBALL, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			m_fAttackPowerRateFireBall *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ELK_STRONG_BONE, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ELK_STRONG_BONE );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_STRONG_BONE );

		if( NULL != pSkillTemplet )
		{
			const float fDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGED_REL, iSkillLevel );
			m_AdditionalFinalDefenceMultiplier.SetPassiveMultiple( m_AdditionalFinalDefenceMultiplier.GetPassiveMultiple() * fDamageRate );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_STRONG_BONE, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			const float fDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGED_REL );
			m_AdditionalFinalDefenceMultiplier.SetPassiveMultiple( m_AdditionalFinalDefenceMultiplier.GetPassiveMultiple() * fDamageRate );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ERS_SPELL_CHAIN, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ERS_SPELL_CHAIN );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ERS_SPELL_CHAIN );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ERS_SPELL_CHAIN, iSkillLevel );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			CreateAndInsertSkillAndSocketBuffFactorToList( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
			CreateAndInsertSkillAndSocketBuffFactorToList( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

#ifdef NEW_SKILL_2010_11
	//{{ oasis907 : 김상윤 [2010.11.1] // 룬 슬레이어 - 마법저항 수련
	m_fMagicReflectRate = 0.f;
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ERS_MAGIC_RESISTANCE, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ERS_MAGIC_RESISTANCE );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ERS_MAGIC_RESISTANCE );
		if( NULL != pSkillTemplet )
		{
			m_fMagicReflectRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REFLECT_REL, iSkillLevel );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ERS_MAGIC_RESISTANCE, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			m_fMagicReflectRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_REFLECT_REL );
			//테스트
			//m_fMagicReflectRate = 1.0f;
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
	//}}
#endif NEW_SKILL_2010_11

#ifdef ELSWORD_SHEATH_KNIGHT

	/// 잔혹한 학살자는 배웠는가?
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelBrutalSlayer = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelBrutalSlayer = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevelBrutalSlayer > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김창한
		const CX2SkillTree::SkillTemplet* pSkillTempletBrutalSlayer = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER );
		if ( NULL != pSkillTempletBrutalSlayer && pSkillTempletBrutalSlayer->m_vecBuffFactorPtr.size() > 0 )
		{	
			InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUElsword_SwordMan, AddComboToBrutalSlayer ) );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTempletBrutalSlayer = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_BRUTAL_SLAYER, iSkillLevelBrutalSlayer );
		if ( NULL != pSkillTempletBrutalSlayer && pSkillTempletBrutalSlayer->m_vecBuffFactorPtr.size() > 0 )
		{
			InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUElsword_SwordMan, AddComboToBrutalSlayer ) );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

	iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ES_ROLLING );
	if( iSkillLevel > 0 )
	{

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	#ifdef PVP_SEASON2
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ES_ROLLING );
	#else
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ES_ROLLING, iSkillLevel );
	#endif

		if( NULL != pSkillTemplet )
		{
			m_fRollingHpConsumeRel	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::HP_CONSUME_REL, min( iSkillLevel, 10 ) );
			m_fRollingSpeedRel		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_SPEED_REL, min( iSkillLevel, 10 ) );
		}
#else // UPGRADE_SKILL_SYSTEM_2013
	#ifdef PVP_SEASON2
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ES_ROLLING,
			min( iSkillLevel + GetSkillLevelUpNum( CX2SkillTree::SI_A_ES_ROLLING ), 10 ) );
	#else
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ES_ROLLING, iSkillLevel );
	#endif

		if( NULL != pSkillTemplet )
		{
			m_fRollingHpConsumeRel	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::HP_CONSUME_REL );
			m_fRollingSpeedRel		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_SPEED_REL );
		}
#endif // UPGRADE_SKILL_SYSTEM_2013
	}
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_ELSWORD_INFINITY_SWORD
	/// 승부사의 기질을 배웠는가?
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelFighter = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_MIND_OF_FIGHTER, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelFighter = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_MIND_OF_FIGHTER );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if ( iSkillLevelFighter > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTempletMindOfFighter = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_MIND_OF_FIGHTER );
		if ( NULL != pSkillTempletMindOfFighter && !pSkillTempletMindOfFighter->m_vecBuffFactorPtr.empty() )
		{
			m_fMindOfFighterConsumeMpRate = pSkillTempletMindOfFighter->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillLevelFighter );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTempletMindOfFighter = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_MIND_OF_FIGHTER, iSkillLevelFighter );
		if ( NULL != pSkillTempletMindOfFighter && !pSkillTempletMindOfFighter->m_vecBuffFactorPtr.empty() )
		{
			m_fMindOfFighterConsumeMpRate = pSkillTempletMindOfFighter->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );
			CreateAndInsertSkillAndSocketBuffFactorToList( pSkillTempletMindOfFighter->m_vecBuffFactorPtr[0] );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
	else 
		m_fMindOfFighterConsumeMpRate = 0.0f;
#endif	SERV_ELSWORD_INFINITY_SWORD

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	//막기 - 액티브 스킬이지만 이 스킬의 방어율이 오토 가드에도 적용됨
	const int iSkillLevelGuard = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ES_DEFENCE );
	if( iSkillLevelGuard > 0 )
	{
		if( NULL == m_GuardForElswordPtr )
		{
			m_GuardForElswordPtr = CElswordGuardSkill::CreateElswordGuardSkill();
		}

		if( NULL != m_GuardForElswordPtr )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ES_DEFENCE );

			if( NULL != pSkillTemplet )
			{
				float ReducePercent = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGED_REL, iSkillLevelGuard );
				m_GuardForElswordPtr->SetDamageReducePercent( static_cast<int>( ReducePercent ) );
			}
		}
	}

	// 오토 가드
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelAutoGuard = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ESK_AUTO_GUARD, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelAutoGuard = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ESK_AUTO_GUARD );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevelAutoGuard > 0 && ( CX2Unit::UC_ELSWORD_LORD_KNIGHT == GetUnit()->GetClass() || CX2Unit::UC_ELSWORD_KNIGHT == GetUnit()->GetClass() ) )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ESK_AUTO_GUARD );

		if( NULL != pSkillTemplet )
		{
			m_fAutoGuardRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevelAutoGuard );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ESK_AUTO_GUARD, iSkillLevelAutoGuard );

		if( NULL != pSkillTemplet )
		{
			m_fAutoGuardRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
	else
		m_fAutoGuardRate = 0.f;

	// 룬 마스터리
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelRuneMastery = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ERS_RUNE_MASTERY, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelRuneMastery = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ERS_RUNE_MASTERY );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevelRuneMastery > 0 && CX2Unit::UC_ELSWORD_RUNE_SLAYER == GetUnit()->GetClass() )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ERS_RUNE_MASTERY );

		if( NULL != pSkillTemplet )
		{
			m_fRuneProjectileRangeIncPercent	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillLevelRuneMastery );
			m_fRuneProjectileSizeIncPercent		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE, iSkillLevelRuneMastery );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ERS_RUNE_MASTERY, iSkillLevelRuneMastery );

		if( NULL != pSkillTemplet )
		{
			m_fRuneProjectileRangeIncPercent	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
			m_fRuneProjectileSizeIncPercent		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_SIZE );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
	else
	{
		m_fRuneProjectileRangeIncPercent	= 1.f;
		m_fRuneProjectileSizeIncPercent		= 1.f;
	}

	//날카로운 검
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelCuttingSword = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_CUTTING_SWORD, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelCuttingSword = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_CUTTING_SWORD );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevelCuttingSword > 0 && CX2Unit::UC_ELSWORD_INFINITY_SWORD == GetUnit()->GetClass() )
	{	
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_CUTTING_SWORD );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_CUTTING_SWORD, iSkillLevelCuttingSword );
	#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef FINALITY_SKILL_SYSTEM //JHKang
		if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() &&
			pSkillTemplet->m_eType != CX2SkillTree::ST_SPECIAL_ACTIVE && pSkillTemplet->m_eType != CX2SkillTree::ST_HYPER_ACTIVE_SKILL )
#else //FINALITY_SKILL_SYSTEM
		if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
#endif //FINALITY_SKILL_SYSTEM
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			CreateAndInsertSkillAndSocketBuffFactorToList( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillLevelCuttingSword ) );
#else //UPGRADE_SKILL_SYSTEM_2013
			CreateAndInsertSkillAndSocketBuffFactorToList( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	//냉혹한 학살자
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelHarshSlayer = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_HARSH_SLAYER, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelHarshSlayer = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_HARSH_SLAYER );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevelHarshSlayer > 0 && ( CX2Unit::UC_ELSWORD_SHEATH_KNIGHT == GetUnit()->GetClass() || CX2Unit::UC_ELSWORD_INFINITY_SWORD == GetUnit()->GetClass() ) )
	{
		InsertDelegateProcessToAttackResult( SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(CX2GUElsword_SwordMan, ApplyBuffToHarshSlayer ) );
	}
	
	//강렬한 일격
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelIntensiveAttack = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ELK_INTENSIVE_ATTACK, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelIntensiveAttack = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ELK_INTENSIVE_ATTACK );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevelIntensiveAttack > 0 && CX2Unit::UC_ELSWORD_LORD_KNIGHT == GetUnit()->GetClass() )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_INTENSIVE_ATTACK );
		if( NULL != pSkillTemplet )
		{
			m_fIncreaseCoolTimeRel = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillLevelIntensiveAttack );
			m_fActiveSkillAddPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL, iSkillLevelIntensiveAttack );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_INTENSIVE_ATTACK, iSkillLevelIntensiveAttack );
		if( NULL != pSkillTemplet )
		{
			m_fIncreaseCoolTimeRel = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
			m_fActiveSkillAddPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DAMAGE_REL );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
	else
	{
		m_fIncreaseCoolTimeRel = 1.f;
		m_fActiveSkillAddPowerRate = 1.f;
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef BALANCE_PATCH_20131107
	m_iSkillLevelInduranceOfRevenge = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ELK_INDURANCE_OF_REVENGE, true );
#endif //BALANCE_PATCH_20131107
}

/*virtual*/ void CX2GUElsword_SwordMan::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ )
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
			m_LuaManager.MakeTableReference( "ESSI_SI_ES_COMMON_BUFF", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, COMMON_BUFF_FrameMove );	
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );	

			m_LuaManager.MakeTableReference( "ESSI_SI_ES_COMMON_BUFF_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, COMMON_BUFF_FrameMove );	
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
					m_LuaManager.MakeTableReference( "ESSI_THROW_ITEM", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	

					m_LuaManager.MakeTableReference( "ESSI_THROW_ITEM", hyperStateData.stateID );
					hyperStateData.m_bHyperState	= true;
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
		case CX2SkillTree::SI_A_ES_MEGASLASH:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ES_MEGASLASH:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_MEGA_SLASH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_CameraMove );		
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_EventProcess );	


			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_MEGA_SLASH_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_MEGA_SLASH_HYPER_EventProcess );
		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	case CX2SkillTree::SI_A_ELK_IMPACT_SMASH:
		{
			m_LuaManager.MakeTableReference( "ESSI_A_ELK_IMPACT_SMASH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_IMPACT_SMASH_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_IMPACT_SMASH_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_A_ELK_IMPACT_SMASH", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_A_ES_SWORD_WAVE:
		{
			m_LuaManager.MakeTableReference( "ESSI_A_ES_SWORD_WAVE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ES_SWORD_WAVE_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ES_SWORD_WAVE_EventProcess );	

			m_LuaManager.MakeTableReference( "ESSI_A_ES_SWORD_WAVE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_A_ES_DEFENCE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_DEFENCE_READY", normalStateData.stateID );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_READY_StartFuture );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_READY_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_DEFENCE_READY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID				= ESSI_SI_A_ES_DEFENCE_WAIT;
				m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_DEFENCE_WAIT", stateData.stateID );
				stateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_WAIT_Start );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_WAIT_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_WAIT_EventProcess );
				m_StateList[stateData.stateID]	= stateData;

				stateData.Init();
				stateData.stateID				= ESSI_SI_A_ES_DEFENCE_END;
				m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_DEFENCE_END", stateData.stateID );
				stateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_END_Start );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_END_EventProcess );
				m_StateList[stateData.stateID]	= stateData;

				stateData.Init();
				stateData.stateID				= ESSI_SI_A_ES_DEFENCE_DAMAGE;
				m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_DEFENCE_DAMAGE", stateData.stateID );
				stateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_DAMAGE_FrameMove );
				stateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_DEFENCE_DAMAGE_EventProcess );
				m_StateList[stateData.stateID]	= stateData;
			}

			if( NULL == m_GuardForElswordPtr )
				m_GuardForElswordPtr = CElswordGuardSkill::CreateElswordGuardSkill();

			if( NULL != m_GuardForElswordPtr )
			{
				m_GuardForElswordPtr->SetSkillSlotID_Guard( static_cast<ELSWORD_SWORDMAN_STATE_ID>( iNormalStateID ) );
				m_GuardForElswordPtr->SetGuardEquippedAtSlotB( bSlotB_ );
			}
		} break;

	case CX2SkillTree::SI_SA_EMK_RISING_WAVE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_EMK_RISING_SLASH_WAVE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EMK_RISING_SLASH_WAVE_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EMK_RISING_SLASH_WAVE_EventProcess );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EMK_RISING_SLASH_WAVE_FrameMove );
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_EMK_RISING_SLASH_WAVE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EMK_RISING_SLASH_WAVE_HYPER_FrameMove );
		} break;

	case CX2SkillTree::SI_A_ERS_CRITICAL_SWORD:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ERS_CRITICAL_SWORD", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_CRITICAL_SWORD_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_CRITICAL_SWORD_EventProcess );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_CRITICAL_SWORD_FrameMove );
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ERS_CRITICAL_SWORD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
		} break;

	case CX2SkillTree::SI_A_ERS_RUNE_OF_FIRE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ERS_RUNE_OF_FIRE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_RUNE_OF_FIRE_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_RUNE_OF_FIRE_EventProcess );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_RUNE_OF_FIRE_FrameMove );
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ERS_RUNE_OF_FIRE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
		} break;

	case CX2SkillTree::SI_A_ERS_RUNE_OF_ICE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ERS_RUNE_OF_ICE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_RUNE_OF_ICE_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_RUNE_OF_ICE_EventProcess );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ERS_RUNE_OF_ICE_FrameMove );
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ERS_RUNE_OF_ICE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
		} break;

	case CX2SkillTree::SI_A_ETK_PIERCING_SWORD:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ETK_PIERCING_SWORD", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ETK_PIERCING_SWORD_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ETK_PIERCING_SWORD_EventProcess );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ETK_PIERCING_SWORD_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ETK_PIERCING_SWORD_FrameMove );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ETK_PIERCING_SWORD_End );
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ETK_PIERCING_SWORD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.StateEnd				= normalStateData.StateEnd;
		} break;

	case CX2SkillTree::SI_A_EIS_FATAL_SLAP:
		{
		m_LuaManager.MakeTableReference( "ESSI_SI_A_EIS_FATAL_SLAP", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_EIS_FATAL_SLAP_Init );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_EIS_FATAL_SLAP_EventProcess );
			m_LuaManager.MakeTableReference( "ESSI_SI_A_EIS_FATAL_SLAP", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;

	case CX2SkillTree::SI_A_ETK_SWORD_SHIELD:
		{
			m_LuaManager.MakeTableReference( "ESSI_A_ETK_SWORD_SHIELD", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ETK_SWORD_SHIELD_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ETK_SWORD_SHIELD_StateStart );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ETK_SWORD_SHIELD_EventProcess );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ETK_SWORD_SHIELD_FrameMove );

			m_LuaManager.MakeTableReference( "ESSI_A_ETK_SWORD_SHIELD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
			hyperStateData.StateStart			= normalStateData.StateStart;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;

		} break;

	case CX2SkillTree::SI_A_ESK_WEAPON_BREAK:
		{
			m_LuaManager.MakeTableReference( "ESSI_A_ESK_WEAPON_BREAK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_WEAPON_BREAK_Init );
#ifndef ADD_MEMO_1ST_CLASS //김창한
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_WEAPON_BREAK_StateStart );
#endif //ADD_MEMO_1ST_CLASS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_WEAPON_BREAK_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_A_ESK_WEAPON_BREAK", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= normalStateData.StateInit;
#ifndef ADD_MEMO_1ST_CLASS //김창한
			hyperStateData.StateStart			= normalStateData.StateStart;
#endif //ADD_MEMO_1ST_CLASS
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
#endif //UPGRADE_SKILL_SYSTEM_2013
		
	case CX2SkillTree::SI_A_ESK_AIR_SLASH:
		{
#ifdef SERV_SKILL_NOTE
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO3 ) == true )
			{
				m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_MEMO", normalStateData.stateID );
			}
			else
			{
				m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH", normalStateData.stateID );
			}					
#else
			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH", normalStateData.stateID ); 
#endif
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_Init );			
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_CameraMove );		
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_EventProcess );	

#ifdef SERV_SKILL_NOTE
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO3 ) == true )
			{
				m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_MEMO_HYPER", hyperStateData.stateID );
			}
			else
			{
				m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_HYPER", hyperStateData.stateID );
			}					
#else
			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_HYPER", hyperStateData.stateID );
#endif
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_HYPER_CameraMove );		
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_HYPER_EventProcess );


			// note!! 아래 코드는 에어슬래시 필살기의 모션이 2개로 나뉘어져 있어서, 예외적으로 상태를 추가해주는 코드이다
			{
				UserUnitStateData stateData;
				stateData.Init();
				stateData.stateID			= ESSI_SA_AIR_SLASH_SECOND;
#ifdef SERV_SKILL_NOTE
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO3 ) == true )
				{
					m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_MEMO_NEXT", stateData.stateID );
				}
				else
				{
					m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_NEXT", stateData.stateID );
				}					
#else
				m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_NEXT", stateData.stateID );
#endif
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_NEXT_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_NEXT_EventProcess );	
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESSI_SA_AIR_SLASH_SECOND_HYPER;
#ifdef SERV_SKILL_NOTE
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO3 ) == true )
				{
					m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_MEMO_HYPER_NEXT", stateData.stateID );
				}
				else
				{
					m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT", stateData.stateID );
				}					
#else
				m_LuaManager.MakeTableReference( "ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT", stateData.stateID );
#endif
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_ES_ASSAULT_SLASH:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ES_ASSAULT_SLASH:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_ASSAULT_SLASH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_Start );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_CameraMove );		
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_EventProcess );	

			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_ASSAULT_SLASH_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_Start );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_HYPER_CameraMove );		
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_ASSAULT_SLASH_HYPER_EventProcess );
		} break;

	case CX2SkillTree::SI_A_EMK_WIND_BLADE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_WIND_BLADE", normalStateData.stateID );	
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_WIND_BLADE_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_WIND_BLADE_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_WIND_BLADE_EventProcess );	

			m_LuaManager.MakeTableReference( "ESSI_SI1_ES_WIND_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_WIND_BLADE_HYPER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI1_ES_WIND_BLADE_HYPER_EventProcess );	
		}
		break;	

	case CX2SkillTree::SI_SA_ES_FLAME_GEYSER:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI2_ES_FLAME_GEYSER", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_Start );
			//normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI2_ES_FLAME_GEYSER_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_HYPER_Start );
			//hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FLAME_GEYSER_HYPER_EventProcess );
		}
		break;

	case CX2SkillTree::SI_SA_ES_FATAL_FURY:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI2_ES_FATAL_FURY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI2_ES_FATAL_FURY_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_FATAL_FURY_HYPER_EventProcess );
		}
		break;

	case CX2SkillTree::SI_SA_ESK_SPIRAL_BLAST:
		{
			const char* pszStateName = "ESSI_SI2_ES_SPIRAL_BLAST";
			const char* pszHyperStateName = "ESSI_SI2_ES_SPIRAL_BLAST_HYPER";

			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO9 ) == true )
			{
				pszStateName = "ESSI_SI2_ES_SPIRAL_BLAST_MEMO";
				pszHyperStateName = "ESSI_SI2_ES_SPIRAL_BLAST_HYPER_MEMO";
			}

			m_LuaManager.MakeTableReference( pszStateName, normalStateData.stateID ); 
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_EventProcess );

			m_LuaManager.MakeTableReference( pszHyperStateName, hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_SPIRAL_BLAST_HYPER_EventProcess );
		}
		break;

	case CX2SkillTree::SI_SA_EMK_RISING_SLASH:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI2_ES_RISING_SLASH", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_RISING_SLASH_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_RISING_SLASH_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_RISING_SLASH_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI2_ES_RISING_SLASH_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_RISING_SLASH_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI2_ES_RISING_SLASH_HYPER_EventProcess );
		}
		break;
	case CX2SkillTree::SI_SA_ES_UNLIMITED_BLADE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_UNLIMITED_BLADE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_EventProcess );


			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_UNLIMITED_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_Init );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_EventProcess );

		}
		break;		

	case CX2SkillTree::SI_SA_ES_TRIPLE_GEYSER:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_TRIPLE_GEYSER", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_EventProcess );


			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_TRIPLE_GEYSER_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_ESK_ARMAGEDON_BLADE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_ARMAGEDDON_BLADE", normalStateData.stateID );	
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_ARMAGEDDON_BLADE_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_ARMAGEDDON_BLADE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_ARMAGEDDON_BLADE_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER_EventProcess );
		} break;

	case CX2SkillTree::SI_SA_ERS_STORM_BLADE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_STORM_BLADE", normalStateData.stateID ); 
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_STORM_BLADE_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_STORM_BLADE_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_STORM_BLADE_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI3_ES_STORM_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_STORM_BLADE_HYPER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI3_ES_STORM_BLADE_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_A_ES_COUNTER:
		{
#ifdef ELSWORD_SHEATH_KNIGHT

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER, true );

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER );

				if( NULL != pSkillTemplet )
				{
					m_bCompactCounterEnable = true;

					if ( NULL == GetUnit() )
						return;

					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// 스킬 레벨

					m_fCompactCounterCoolTime = pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel );
				}
#else //UPGRADE_SKILL_SYSTEM_2013
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER );

			if( iSkillLevel > 0 )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER, iSkillLevel );

				if( NULL != pSkillTemplet )
				{
					m_bCompactCounterEnable = true;

					m_bCompactCounterEnable = true;
					m_fCompactCounterCoolTime = pSkillTemplet->m_fSkillCoolTime;
				}
#endif //UPGRADE_SKILL_SYSTEM_2013
			
				m_LuaManager.MakeTableReference( "ESSI_SI_A_ETK_COMPACT_COUNTER_ATTACK", normalStateData.stateID ); 
				normalStateData.StateStart			= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackStart);
				normalStateData.StateStartFuture	= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackStartFuture);
				normalStateData.OnFrameMoveFuture	= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackFrameMoveFuture);
				normalStateData.OnFrameMove			= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackFrameMove);
				normalStateData.OnEventProcess		= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackEventProcess);
				normalStateData.StateEnd			= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackStateEnd);

				m_LuaManager.MakeTableReference( "ESSI_SI_A_ETK_COMPACT_COUNTER_ATTACK", hyperStateData.stateID );
				hyperStateData.m_bHyperState		= true;
				hyperStateData.StateStart			= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackStart);
				hyperStateData.StateStartFuture		= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackStartFuture);
				hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackFrameMoveFuture);
				hyperStateData.OnFrameMove			= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackFrameMove);
				hyperStateData.OnEventProcess		= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackEventProcess);
				hyperStateData.StateEnd				= SET_CB_FUNC(CX2GUElsword_SwordMan, CompactCounterAttackStateEnd);
			}
			else
			{
				m_LuaManager.MakeTableReference( "ESSI_REVENGE_WAIT", normalStateData.stateID ); 
				normalStateData.StateStart			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitStart);
				normalStateData.OnFrameMoveFuture	= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMoveFuture);
				normalStateData.OnFrameMove			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMove);
				normalStateData.OnEventProcess		= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitEventProcess);

				m_LuaManager.MakeTableReference( "ESSI_REVENGE_WAIT", hyperStateData.stateID );
				hyperStateData.m_bHyperState		= true;
				hyperStateData.StateStart			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitStart);
				hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMoveFuture);
				hyperStateData.OnFrameMove			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMove);
				hyperStateData.OnEventProcess		= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitEventProcess);
			}
#else
			m_LuaManager.MakeTableReference( "ESSI_REVENGE_WAIT", normalStateData.stateID ); 
			normalStateData.StateStart			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitStart);
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMoveFuture);
			normalStateData.OnFrameMove			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMove);
			normalStateData.OnEventProcess		= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitEventProcess);

			m_LuaManager.MakeTableReference( "ESSI_REVENGE_WAIT", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitStart);
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMoveFuture);
			hyperStateData.OnFrameMove			= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitFrameMove);
			hyperStateData.OnEventProcess		= SET_CB_FUNC(CX2GUElsword_SwordMan, RevengeWaitEventProcess);
#endif ELSWORD_SHEATH_KNIGHT

		} break;

	case CX2SkillTree::SI_A_ES_KICK:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_KICK", normalStateData.stateID ); 
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_KICK_Start );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_KICK_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_KICK", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_KICK_Start );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_KICK_EventProcess );	
		} break;


	case CX2SkillTree::SI_SA_ELK_WINDMILL:
		{
			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_WINDMILL_START", normalStateData.stateID ); 
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_StateEnd );



			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_WINDMILL_START_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_HYPER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_HYPER_EventProcess );
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_START_HYPER_StateEnd );




			UserUnitStateData stateData;
			stateData.Init();
			stateData.stateID			= ESSI_SA_ESK_WINDMILL;
			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_WINDMILL", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_FrameMove );	
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_EventProcess );	
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_StateEnd );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_SA_ESK_WINDMILL_HYPER;
			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_WINDMILL_HYPER", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_HYPER_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_HYPER_EventProcess );
			stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_HYPER_StateEnd );

			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ESSI_SA_ESK_WINDMILL_END;
			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_WINDMILL_END", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_END_FrameMove );	
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_END_EventProcess );	
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= ESSI_SA_ESK_WINDMILL_END_HYPER;
			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_WINDMILL_END_HYPER", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_END_HYPER_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_WINDMILL_END_HYPER_EventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;


	case CX2SkillTree::SI_SA_EMK_SWORD_FIRE:
		{

			m_LuaManager.MakeTableReference( "ESSI_SA_EMK_SWORD_FIRE", normalStateData.stateID );				
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_StartFuture );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_EndFuture );				

			m_LuaManager.MakeTableReference( "ESSI_SA_EMK_SWORD_FIRE_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_HYPER_Init );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_HYPER_StartFuture );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_HYPER_EventProcess );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_HYPER_EndFuture );				

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESSI_SA_EMK_SWORD_FIRE_LANDING;
				m_LuaManager.MakeTableReference( "ESSI_SA_EMK_SWORD_FIRE_LANDING", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_LANDING_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_LANDING_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_LANDING_End );					
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER;
				m_LuaManager.MakeTableReference( "ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_End );					
				m_StateList[stateData.stateID] = stateData;
			}
		} break;




	case CX2SkillTree::SI_A_ESK_ARMOR_BREAK:
		{
			m_LuaManager.MakeTableReference( "ESSI_A_ESK_ARMOR_BREAK", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_StateEnd );


			m_LuaManager.MakeTableReference( "ESSI_A_ESK_ARMOR_BREAK_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_ARMOR_BREAK_HYPER_StateEnd );

		} break;

	case CX2SkillTree::SI_A_EMK_BIG_BURST:
		{
			m_LuaManager.MakeTableReference( "ESSI_SA_EMK_BIG_BURST", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_End );	

			m_LuaManager.MakeTableReference( "ESSI_SA_EMK_BIG_BURST", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_Init );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_EventProcess );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_EMK_BIG_BURST_End );	

		} break;




#ifdef SKILL_CASH_10_TEST

	case CX2SkillTree::SI_SA_EMK_PHOENIX_TALON:
		{
			m_LuaManager.MakeTableReference( "ESSI_SA_EMK_PHOENIX_TALON", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_StateEnd );	

			m_LuaManager.MakeTableReference( "ESSI_SA_EMK_PHOENIX_TALON_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_Init );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_Hyper_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ESK_PHOENIX_TALON_StateEnd );	

		} break;
		//{{ 허상형 : [2009/9/22] //	더블 슬래시
	case CX2SkillTree::SI_SA_ESK_DOUBLE_SLASH:
		{
			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_DOUBLE_SLASH", normalStateData.stateID );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_DOUBLE_SLASH_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_DOUBLE_SLASH_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_DOUBLE_SLASH_StateEnd );

			m_LuaManager.MakeTableReference( "ESSI_SA_ESK_DOUBLE_SLASH_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ESK_DOUBLE_SLASH_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			hyperStateData.StateEnd				= normalStateData.StateEnd;			
		}
		break;
		//}} 허상형 : [2009/9/22] //	더블 슬래시
#endif



#ifdef ELSWORD_SECOND_CLASS_CHANGE



	case CX2SkillTree::SI_A_ELK_SONIC_BLADE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SA_ELK_SONIC_BLADE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_StateEnd );	

			m_LuaManager.MakeTableReference( "ESSI_SA_ELK_SONIC_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_HYPER_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_HYPER_EventProcess );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SONIC_BLADE_HYPER_StateEnd );	

		} break;



	case CX2SkillTree::SI_A_ERS_SPLASH_EXPLOSION:
		{

			m_LuaManager.MakeTableReference( "ESSI_SA_ERS_SPLASH_EXPLOSION", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_SPLASH_EXPLOSION_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_SPLASH_EXPLOSION_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_SPLASH_EXPLOSION_FrameMove );
#ifdef SKILL_BALANCE_PATCH
#ifdef CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );				
#else CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, GenericActiveSkillEventProcess );				
#endif CONVERSION_VS
#else
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, GenericSpecialActiveSkillEventProcess );
#endif
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_SPLASH_EXPLOSION_StateEnd );	

			m_LuaManager.MakeTableReference( "ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_FrameMove );
#ifdef SKILL_BALANCE_PATCH
#ifdef CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
#else CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, GenericActiveSkillEventProcess );
#endif CONVERSION_VS
#else				
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, GenericSpecialActiveSkillEventProcess );
#endif
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_StateEnd );	

		} break;







	case CX2SkillTree::SI_A_ELK_ROLLING_SMASH:
		{

			m_LuaManager.MakeTableReference( "ESSI_A_ELK_ROLLING_SMASH_READY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_StateStart );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_EventProcess );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_StateEnd );	

			m_LuaManager.MakeTableReference( "ESSI_A_ELK_ROLLING_SMASH_READY_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_StateStart );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_EventProcess );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_StateEnd );	




			{
				UserUnitStateData stateData;


				stateData.Init();
				stateData.stateID			= ESSI_A_ELK_ROLLING_SMASH;
				m_LuaManager.MakeTableReference( "ESSI_A_ELK_ROLLING_SMASH", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_StateStartFuture );	
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_End );					
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESSI_A_ELK_ROLLING_SMASH_HYPER;
				m_LuaManager.MakeTableReference( "ESSI_A_ELK_ROLLING_SMASH_HYPER", stateData.stateID );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_HYPER_StateStartFuture );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ELK_ROLLING_SMASH_HYPER_EventProcess );
				m_StateList[stateData.stateID] = stateData;





				stateData.Init();
				stateData.stateID			= ESSI_A_ELK_ROLLING_SMASH_LANDING;
				m_LuaManager.MakeTableReference( "ESSI_A_ELK_ROLLING_SMASH_LANDING", stateData.stateID );
#ifdef CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	
#else CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, GenericSpecialActiveSkillEventProcess );	
#endif CONVERSION_VS
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= ESSI_A_ELK_ROLLING_SMASH_LANDING_HYPER;
				m_LuaManager.MakeTableReference( "ESSI_A_ELK_ROLLING_SMASH_LANDING_HYPER", stateData.stateID );
#ifdef CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );	
#else CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, GenericSpecialActiveSkillEventProcess );	
#endif CONVERSION_VS
				m_StateList[stateData.stateID] = stateData;
			}




		} break;



	case CX2SkillTree::SI_A_ERS_SWORD_ENCHANT:
		{

			m_LuaManager.MakeTableReference( "ESSI_A_ERS_SWORD_ENCHANT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ERS_SWORD_ENCHANT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ERS_SWORD_ENCHANT_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_A_ERS_SWORD_ENCHANT_EventProcess );
			
			m_LuaManager.MakeTableReference( "ESSI_A_ERS_SWORD_ENCHANT_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;



#endif ELSWORD_SECOND_CLASS_CHANGE


#ifdef NEW_SKILL_2010_11
		//{{ oasis907 : 김상윤 [2010.11.5] // 로드 나이트 - 샌드 스톰
	case CX2SkillTree::SI_SA_ELK_SAND_STORM:
		{
			m_LuaManager.MakeTableReference( "ESSI_SA_ELK_SAND_STORM", normalStateData.stateID );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SAND_STORM_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SAND_STORM_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ELK_SAND_STORM_EventProcess );
			
			m_LuaManager.MakeTableReference( "ESSI_SA_ELK_SAND_STORM_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMoveFuture	= normalStateData.OnFrameMoveFuture;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
			
		} break;
		//}} oasis907 : 김상윤 [2010.11.5] // 로드 나이트 - 샌드 스톰
#endif NEW_SKILL_2010_11

		//{{ JHKang / 강정훈 / 2010/11/02 / 루나 블레이드 추가
#ifdef NEW_SKILL_2010_11
	case CX2SkillTree::SI_SA_ERS_LUNA_BLADE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SA_ERS_LUNA_BLADE", normalStateData.stateID );	
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_LUNA_BLADE_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_LUNA_BLADE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SA_ERS_LUNA_BLADE_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SA_ERS_LUNA_BLADE_HYPER", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
#endif NEW_SKILL_2010_11
		//}} JHKang / 강정훈 / 2010/11/02 / 루나 블레이드 추가

#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2SkillTree::SI_A_ES_ROLLING:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_ROLLING", normalStateData.stateID ); 
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_StateStart );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_StateStartFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_FrameMove );				
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_EventProcess );				

			m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_ROLLING", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_StateStartFuture );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_EventProcess );	
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESSI_SI_A_ES_ROLLING_END;
				m_LuaManager.MakeTableReference( "ESSI_SI_A_ES_ROLLING_END", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_END_StateStart );	
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_END_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_ES_ROLLING_END_EventProcess );				
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

	case CX2SkillTree::SI_A_ETK_HARSH_CHASER:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_HARSH_CHASER", normalStateData.stateID );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_StateStart );	
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_EventProcess );
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_End );

			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_HARSH_CHASER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_StateStart );	
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_EventProcess );
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_End );
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= ESSI_SI_SA_ETK_HARSH_CHASER_PULL;
				m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_HARSH_CHASER_PULL", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_PULL_FrameMove );	
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_HARSH_CHASER_PULL_EventProcess );				
				m_StateList[stateData.stateID] = stateData;
			}
		} break;

	case CX2SkillTree::SI_SA_EIS_MAELSTORM_RAGE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_MAELSTORM_RAGE", normalStateData.stateID );		
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_Init );
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_StartFuture );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_EventProcess );	
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_EndFuture );		

			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_MAELSTORM_RAGE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_Init );
			hyperStateData.StateStartFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_StartFuture );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_EventProcess );
			hyperStateData.StateEndFuture		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_MAELSTORM_RAGE_EndFuture );
		} break;
	case CX2SkillTree::SI_SA_ETK_FINAL_STRIKE:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_FINAL_STRIKE", normalStateData.stateID );		
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_EventProcess );		
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_End );

			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_FINAL_STRIKE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_FINAL_STRIKE_End );
		} break;

	case CX2SkillTree::SI_SA_EIS_SWORD_FALL:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_SWORD_FALL", normalStateData.stateID );		
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_EventProcess );		
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_End );

			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_SWORD_FALL", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_SWORD_FALL_End );

		} break;

	case CX2SkillTree::SI_A_ETK_CRESCENT_CUT:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_CRESCENT_CUT", normalStateData.stateID );		
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_EventProcess );		
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_End );

			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_CRESCENT_CUT", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_CRESCENT_CUT_End );
		} break;

	case CX2SkillTree::SI_SA_ETK_PHANTOM_SWORD:
		{
			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_PHANTOM_SWORD", normalStateData.stateID );	
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_PHANTOM_SWORD_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_PHANTOM_SWORD_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_PHANTOM_SWORD_EventProcess );

			m_LuaManager.MakeTableReference( "ESSI_SI_SA_ETK_PHANTOM_SWORD", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.StateInit			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_PHANTOM_SWORD_Init );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_PHANTOM_SWORD_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_ETK_PHANTOM_SWORD_EventProcess );
		} break;


#endif ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
		case CX2SkillTree::SI_A_EIS_MIRAGE_STING:
			{
				m_LuaManager.MakeTableReference( "ESSI_SI_A_EIS_SPIRAL_STING", normalStateData.stateID );	
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_EIS_SPIRAL_STING_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_EIS_SPIRAL_STING_EventProcess );

				m_LuaManager.MakeTableReference( "ESSI_SI_A_EIS_SPIRAL_STING", hyperStateData.stateID );
				hyperStateData.m_bHyperState		= true;
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_EIS_SPIRAL_STING_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_A_EIS_SPIRAL_STING_EventProcess );
			} break;
		case CX2SkillTree::SI_A_EIS_SWORD_BLASTING:
			{
				m_LuaManager.MakeTableReference( "ESSI_SI_SA_EIS_SWORD_BLASTING", normalStateData.stateID );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_SWORD_BLASTING_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_SWORD_BLASTING_EventProcess );

				m_LuaManager.MakeTableReference( "ESSI_SI_SA_EIS_SWORD_BLASTING", hyperStateData.stateID );
				hyperStateData.m_bHyperState		= true;
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_SWORD_BLASTING_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_SWORD_BLASTING_EventProcess );
			} break;
		case CX2SkillTree::SI_SA_EIS_RAGE_CUTTER:
			{
				m_LuaManager.MakeTableReference( "ESSI_SI_SA_EIS_RAGE_CUTTER", normalStateData.stateID );
				normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_RAGE_CUTTER_FrameMove );
				normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_RAGE_CUTTER_EventProcess );

				m_LuaManager.MakeTableReference( "ESSI_SI_SA_EIS_RAGE_CUTTER", hyperStateData.stateID );
				hyperStateData.m_bHyperState		= true;
				hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_RAGE_CUTTER_FrameMove );
				hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_SI_SA_EIS_RAGE_CUTTER_EventProcess );
			} break;

#endif //SERV_ELSWORD_INFINITY_SWORD

#ifdef FINALITY_SKILL_SYSTEM //JHKang
		#pragma	region 
		case CX2SkillTree::SI_HA_ELK_GIGANTIC_SLASH:
			{
				m_LuaManager.MakeTableReference( "ESSI_HA_ELK_GIGANTIC_SLASH", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_ELK_Gigantic_Slash_Init );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );

				m_LuaManager.MakeTableReference( "ESSI_HA_ELK_GIGANTIC_SLASH", hyperStateData.stateID );
				hyperStateData.m_bHyperState	= true;
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
			}
			break;
		#pragma endregion 기간틱 슬래시 : 궁극기

		#pragma region SI_FS_ERS_Shining_Rune_Buster
		case CX2SkillTree::SI_HA_ERS_SHINING_RUNE_BUSTER:
			{
				m_LuaManager.MakeTableReference( "ESSI_HA_ERS_SHINING_RUNE_BUSTER", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_ERS_Shining_Rune_Buster_Init );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_ERS_Shining_Rune_Buster_EventProcess );
				


				m_LuaManager.MakeTableReference( "ESSI_HA_ERS_SHINING_RUNE_BUSTER", hyperStateData.stateID );
				hyperStateData.m_bHyperState	= true;
				hyperStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_ERS_Shining_Rune_Buster_Init );
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_ERS_Shining_Rune_Buster_EventProcess );
			}
			break;
		#pragma endregion 샤이닝 룬 버스트 - 궁극기

		#pragma region SI_FS_EIS_Blade_Rain
		case CX2SkillTree::SI_HA_EIS_BLADE_RAIN:
			{
				m_LuaManager.MakeTableReference( "ESSI_HA_EIS_BLADE_RAIN", normalStateData.stateID );
				normalStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_EIS_Blade_Rain_Init );
				normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_EIS_Blade_Rain_EventProcess );

				m_LuaManager.MakeTableReference( "ESSI_HA_EIS_BLADE_RAIN", hyperStateData.stateID );
				hyperStateData.m_bHyperState	= true;
				hyperStateData.StateInit		= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_EIS_Blade_Rain_Init );
				hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUElsword_SwordMan, ESSI_HA_EIS_Blade_Rain_EventProcess );
			
		}
			break;
		#pragma endregion 블레이드 레인 - 궁극기
#endif //FINALITY_SKILL_SYSTEM
	}
}

/*virtual*/ void CX2GUElsword_SwordMan::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ESK_AIR_SLASH == eSkillID_ )
	{
		// note!! 아래의 코드는 에어슬래시를 위한 예외처리 코드 입니다
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ESK_AIR_SLASH );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_AIR_SLASH_SECOND ];
			normalStateData.m_SPLevel = iSkillTempletLevel;
			normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			normalStateData.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_AIR_SLASH_SECOND_HYPER ];
			hyperStateData.m_SPLevel = iSkillTempletLevel;
			hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_AIR_SLASH_SECOND ];
			normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			normalStateData.m_eSkillID = pSkillTemplet->m_eID;

			UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_AIR_SLASH_SECOND_HYPER ];
			hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
			hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
			hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ELK_WINDMILL == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ELK_WINDMILL );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_ESK_WINDMILL ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_ESK_WINDMILL_HYPER ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_ESK_WINDMILL_END ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_ESK_WINDMILL_END_HYPER ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
	
			m_fWindMillDuration = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_ESK_WINDMILL ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_ESK_WINDMILL_HYPER ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_ESK_WINDMILL_END ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_ESK_WINDMILL_END_HYPER ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}

			m_fWindMillDuration = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_EMK_SWORD_FIRE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EMK_SWORD_FIRE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_EMK_SWORD_FIRE_LANDING ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;

			}		
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SA_EMK_SWORD_FIRE_LANDING ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

				UserUnitStateData& hyperStateData = m_StateList[ ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;

			}		
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

#ifdef ELSWORD_SECOND_CLASS_CHANGE
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ELK_ROLLING_SMASH == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ELK_ROLLING_SMASH );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;


				UserUnitStateData& hyperStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH_HYPER ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH_LANDING ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;


				UserUnitStateData& hyperStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH_LANDING_HYPER ];
				hyperStateData.m_SPLevel = iSkillTempletLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;


				UserUnitStateData& hyperStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH_HYPER ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH_LANDING ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;


				UserUnitStateData& hyperStateData = m_StateList[ ESSI_A_ELK_ROLLING_SMASH_LANDING_HYPER ];
				hyperStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				hyperStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				hyperStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#endif ELSWORD_SECOND_CLASS_CHANGE

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	if( true == bChangeAll_ || CX2SkillTree::SI_A_ES_DEFENCE == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ES_DEFENCE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_READY ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_WAIT ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_END ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_DAMAGE ];
				normalStateData.m_SPLevel = iSkillTempletLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_READY ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_WAIT ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_END ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;

			}

			{
				UserUnitStateData& normalStateData = m_StateList[ ESSI_SI_A_ES_DEFENCE_DAMAGE ];
				normalStateData.m_SPLevel = pSkillTemplet->m_iSkillLevel;
				normalStateData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
				normalStateData.m_eSkillID = pSkillTemplet->m_eID;
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

}



#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUElsword_SwordMan::AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD



void CX2GUElsword_SwordMan::InitComponent()
{
	CX2GUUser::InitComponent();

#ifdef ELSWORD_WAY_OF_SWORD
	
	if( IsMyUnit() )
	{
		ASSERT( NULL != g_pX2Game );
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				static_cast<CX2ElswordGageData*>(m_pGageData)->SetWayOfSwordPoint( 0.f );
				static_cast<CX2ElswordGageData*>(m_pGageData)->SetWayOfSwordState( 0 );
				m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();				
			} break;

		default:
			{
				//m_GageManager.UpdateUIElswordWSP( WSS_CENTER );
				m_FrameDataFuture.syncData.m_CannonBallCount = GetWayOfSwordState();
			} break;
		}
	}
#endif ELSWORD_WAY_OF_SWORD
}


void CX2GUElsword_SwordMan::ParseCommonRandomState()
{
	if( true == m_LuaManager.BeginTable( "INIT_COMMON_RANDOM_STATE" ) )
	{

		const char* tableName = "";
		switch( GetUnit()->GetClass() )
		{
		case CX2Unit::UC_ELSWORD_SWORDMAN:
			{
				tableName = "ELSWORD_SWORDMAN";
			} break;

		case CX2Unit::UC_ELSWORD_KNIGHT:
			{
				tableName = "ELSWORD_KNIGHT";
			} break;

#ifdef ELSWORD_SHEATH_KNIGHT
		case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
			{
				tableName = "ELSWORD_SHEATH_KNIGHT";
			} break;
#endif ELSWORD_SHEATH_KNIGHT

		case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
			{
				tableName = "ELSWORD_MAGIC_KNIGHT";
			} break;

		case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
			{
				tableName = "ELSWORD_LORD_KNIGHT";
			} break;


		case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
			{
				tableName = "ELSWORD_RUNE_SLAYER";
			} break;

#ifdef SERV_ELSWORD_INFINITY_SWORD
		case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
			{
				tableName = "ELSWORD_INFINITY_SWORD";
			} break;
#endif


		default:
			{
				ASSERT( !"NO" );
			} break;
		}

		CX2GUUser::ParseCommonRandomState(tableName);

		m_LuaManager.EndTable(); // INIT_COMMON_RANDOM_STATE
	}
}


#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUElsword_SwordMan::AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	CX2GUElsword_SwordMan::AppendMajorParticleToDeviceList( listInOut_ );
	CX2GUElsword_SwordMan::AppendMinorParticleToDeviceList( listInOut_ );
	CX2GameUnit::Weapon::AppendEnchantParticleForSwordToDeviceList( listInOut_, eDamageType_ );
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GUElsword_SwordMan::InitEffect()
{
	CX2GUUser::InitEffect();

	CreateMajorParticleArray();
	CreateMinorParticleArray();

	if ( IsMyUnit() )
	{
		switch ( GetWayOfSwordState() )
		{
		case WSS_VIGOR:
			{
				Start_SwordmanVigor();
			} break;

		case WSS_DESTRUCTION:
			{
				Start_SwordmanDestruction();
			} break;

		default:
			break;
		}
	}
}



/*virtual*/ void CX2GUElsword_SwordMan::InitEnchantWeaponEffectUnitSpecific() 
{
#ifdef BALANCE_RUNE_SLAYER_20130214
	if( true == m_bShowWeaponEnchantEffect )
#endif //BALANCE_RUNE_SLAYER_20130214
	{
		for( UINT i=0; i<m_vecpWeapon.size(); ++i )
		{
			Weapon* pWeapon = m_vecpWeapon[i];
			pWeapon->InitEnchantParticleForSword();
		}
	}
}



void CX2GUElsword_SwordMan::DeleteElswordMajorParticle()
{
	CX2GUUser::DeleteGUUserMajorParticle();

	CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();

	for ( int index = 0; index < ELSWORD_MAJOR_PII_END; index++ )
	{
		pMajorParticleSystem->DestroyInstanceHandle( m_ahElswordMajorParticleInstance[index] );
	}
}

void CX2GUElsword_SwordMan::DeleteElswordMinorParticle()
{
	CX2GUUser::DeleteGUUserMinorParticle();

	CKTDGParticleSystem* pMinorParticleSystem = g_pX2Game->GetMinorParticle();

	for ( int index = 0; index < ELSWORD_MINOR_PII_END; index++ )
	{
		pMinorParticleSystem->DestroyInstanceHandle( m_ahElswordMinorParticleInstance[index] );
	}	
}

void CX2GUElsword_SwordMan::InitializeElswordMajorParticleArray()
{
	CX2GUUser::InitializeGUUserMajorParticleArray();

	for ( int index = 0; index < ELSWORD_MAJOR_PII_END; index++ )
	{
		m_ahElswordMajorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUElsword_SwordMan::AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
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


/*virtual*/ void CX2GUElsword_SwordMan::CreateMajorParticleArray()
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

CKTDGParticleSystem::CParticleEventSequence* CX2GUElsword_SwordMan::SetElswordMajorParticleByEnum( ELSWORD_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ /*= -1*/ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleElswordMajorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 0, 0, iDrawCount_, 0 );

		SetHandleElswordMajorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleElswordMajorParticleByEnum( eVal_) );

	return pSeq;
}

void CX2GUElsword_SwordMan::InitializeElswordMinorParticleArray()
{
	CX2GUUser::InitializeGUUserMinorParticleArray();

	for ( int index = 0; index < ELSWORD_MINOR_PII_END; index++ )
	{
		m_ahElswordMinorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUElsword_SwordMan::AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMinorParticle() == NULL )
		return;

	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"RevengeWait" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"GroundShockWaveBlue" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"ImpactTickBlue" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"STR_KuAAang" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"GroundShockWaveRed" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"ImpactTickRed" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"MagnumBladeFireWave" );
}

#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD



/*virtual*/ void CX2GUElsword_SwordMan::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	pSeq = SetElswordMinorParticleByEnum( ELSWORD_MINOR_PII_REVENGE_WAIT, L"RevengeWait" );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE, L"GroundShockWaveBlue", 2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE, L"ImpactTickBlue", 2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetElswordMinorParticleByEnum( ELSWORD_MINOR_PII_STR_KUAANG, L"STR_KuAAang", 1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_RED, L"GroundShockWaveRed", 2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_TICK_RED, L"ImpactTickRed", 2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetElswordMinorParticleByEnum( ELSWORD_MINOR_PII_MAGNUM_BLADE_FIRE_WAVE, L"MagnumBladeFireWave" );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
}

CKTDGParticleSystem::CParticleEventSequence* CX2GUElsword_SwordMan::SetElswordMinorParticleByEnum( ELSWORD_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ /*= -1*/ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleElswordMinorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 0, 0, iDrawCount_, 0 );

		SetHandleElswordMinorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( eVal_) );

	return pSeq;
}

void CX2GUElsword_SwordMan::CommonStateStartFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateStartFuture();
}

void CX2GUElsword_SwordMan::CommonStateStart()
{
	KTDXPROFILE();

	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME",				m_fDamageDataChangeTime,			0.f );
	LUA_GET_VALUE(	m_LuaManager, "DAMAGEDATA_CHANGE_TIME2",			m_fDamageDataChangeTime2,			0.f );

	CX2GUUser::CommonStateStart();

#ifdef SERV_SKILL_NOTE
	m_bFirstAttackSuccess = false;
#endif
}

void CX2GUElsword_SwordMan::CommonFrameMoveFuture()
{
	KTDXPROFILE();
	m_fReAttackZ1Time -= m_fElapsedTime;
	if( m_fReAttackZ1Time < 0.0f )
		m_fReAttackZ1Time = 0.0f;

	m_fReDashJumpXTime -= m_fElapsedTime;
	if( m_fReDashJumpXTime < 0.0f )
		m_fReDashJumpXTime = 0.0f;

	CX2GUUser::CommonFrameMoveFuture();
#ifdef ELSWORD_WAY_OF_SWORD
	m_FrameDataFuture.syncData.m_CannonBallCount = (int)GetWayOfSwordState();
#endif ELSWORD_WAY_OF_SWORD
}



void CX2GUElsword_SwordMan::CommonFrameMove()
{
	KTDXPROFILE();
	CX2GUUser::CommonFrameMove();
	
#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	if( m_pMegaSlash != NULL )
		m_pMegaSlash->OnFrameMove( m_fTime, m_fElapsedTime );
	if( m_pMegaSlashBackDust != NULL )
		m_pMegaSlashBackDust->OnFrameMove( m_fTime, m_fElapsedTime );

	if( m_pFlameGager != NULL )
		m_pFlameGager->OnFrameMove( m_fTime, m_fElapsedTime );
	if( m_pFlameGagerLand != NULL )
		m_pFlameGagerLand->OnFrameMove( m_fTime, m_fElapsedTime );

	if( m_pMagnumBlade != NULL )
		m_pMagnumBlade->OnFrameMove( m_fTime, m_fElapsedTime );
	if( m_pMagnumBladeFire != NULL )
		m_pMagnumBladeFire->OnFrameMove( m_fTime, m_fElapsedTime );

//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

#ifdef ELSWORD_SHEATH_KNIGHT
	if( m_hSecondBladeSheath != INVALID_MESH_INSTANCE_HANDLE )
	{

		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSecondBladeSheath );
		if( NULL != pMeshInst )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
			pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
		}
	}
#endif ELSWORD_SHEATH_KNIGHT
	// 윈드밀 사용시작 시점에 아이샤 블리자드 샤워를 맞으면 윈드밀 이펙트가 사라지지 않는 오류가 있어서, 필살기 상태 혹은 윈드밀 상태가 아니면 강제로 윈드밀 이펙트를 지워주도록 한다.
	if( INVALID_EFFECTSET_HANDLE != m_hWindMill || 
		INVALID_EFFECTSET_HANDLE != m_hWindMillLand )
	{
		switch( GetNowStateID() )
		{
		case USI_SPECIAL_ATTACK_1:
		case USI_SPECIAL_ATTACK_HYPER_1:
		case USI_SPECIAL_ATTACK_2:
		case USI_SPECIAL_ATTACK_HYPER_2:
		case USI_SPECIAL_ATTACK_3:
		case USI_SPECIAL_ATTACK_HYPER_3:
		case USI_SPECIAL_ATTACK_4:
		case USI_SPECIAL_ATTACK_HYPER_4:
		case USI_SPECIAL_ATTACK_SLOTB_1:
		case USI_SPECIAL_ATTACK_SLOTB_HYPER_1:
		case USI_SPECIAL_ATTACK_SLOTB_2:
		case USI_SPECIAL_ATTACK_SLOTB_HYPER_2:
		case USI_SPECIAL_ATTACK_SLOTB_3:
		case USI_SPECIAL_ATTACK_SLOTB_HYPER_3:
		case USI_SPECIAL_ATTACK_SLOTB_4:
		case USI_SPECIAL_ATTACK_SLOTB_HYPER_4:
		case ESSI_SA_ESK_WINDMILL:
		case ESSI_SA_ESK_WINDMILL_HYPER:
		case ESSI_SA_ESK_WINDMILL_END:
		case ESSI_SA_ESK_WINDMILL_END_HYPER:
			{
				if( m_ExtraDamagePack.m_EnchantShock.m_fTime > 0.f ||
					m_ExtraDamagePack.m_Frozen.m_fTime > 0.f )
				{
					g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMill );
					g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMillLand );

					if( NULL != m_pSoundWindMill )
					{
						m_pSoundWindMill->Stop();
					}
				}
			} break;
		default:
			{
				g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMill );
				g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMillLand );

				if( NULL != m_pSoundWindMill )
				{
					m_pSoundWindMill->Stop();
				}
			} break;
		}
	}

#ifdef ELSWORD_WAY_OF_SWORD
	WayOfSwordStateChange();
	DestructionRageFrameMove();
#endif ELSWORD_WAY_OF_SWORD

#ifdef BALANCE_RUNE_SLAYER_20130214
	if ( GetNeedToUpdateWeaponEffect() )
	{
		UpdateWeaponEnchantEffectAndExtraDamageType();
		SetNeedToUpdateWeaponEffect( false );
	}
#endif //BALANCE_RUNE_SLAYER_20130214
}

void CX2GUElsword_SwordMan::CommonStateEndFuture()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEndFuture();
}

void CX2GUElsword_SwordMan::CommonStateEnd()
{
	KTDXPROFILE();
	CX2GUUser::CommonStateEnd();
}

//{{ robobeg : 2008-10-28
//void CX2GUElsword_SwordMan::CommonRender()
/*virtual*/
RENDER_HINT CX2GUElsword_SwordMan::CommonRender_Prepare()
//}} robobeg : 2008-10-28
{
	KTDXPROFILE();
// 	if( GetRemainHyperModeTime() > 0.0f )
// 	{
// 		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
// 		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_RED;
// 		m_RenderParam.outLineColor		= 0xffff5511;
// 	}
#ifdef ELSWORD_WAY_OF_SWORD
	/*else */if( GetWayOfSwordState() == WSS_VIGOR )
	{
		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_SKYBLUE;
		m_RenderParam.outLineColor		= 0xffffffff;
	}
	else if( GetWayOfSwordState() == WSS_DESTRUCTION )
	{
		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_SCARLET;
		m_RenderParam.outLineColor		= 0xffffffff;
	}
#endif ELSWORD_WAY_OF_SWORD
// 	else
// 	{
// 		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
// 		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
// 		m_RenderParam.outLineColor		= 0xffffffff;
// 	}

//{{ robobeg : 2008-10-28
	//CX2GUUser::CommonRender();
    return CX2GUUser::CommonRender_Prepare();
//}} robobeg : 2008-10-28
}

/*virtual*/ bool CX2GUElsword_SwordMan::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition /*= CX2SkillTree::ASUT_ONLY_ON_LINE*/ )
{
	KTDXPROFILE();

	const int INVALID_SKILL_SLOT_INDEX = -1;

	int iPressedSkillSlotIndex = INVALID_SKILL_SLOT_INDEX;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;

	CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree; 	// 유저가 배운 스킬 트리

	if ( false == CommonSpecialAttackEventProcess( accessUserSkillTree, pSkillSlotData, iPressedSkillSlotIndex ) )
		return false;

	if( NULL == pSkillSlotData )
		return false;

	// 사용한 스킬
	const CX2SkillTree::SkillTemplet* pSkillTempletUsing = GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );
	if( NULL == pSkillTempletUsing )
		return false;

#ifdef ADD_MEMO_1ST_CLASS //김창한
	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTempletUsing->m_eActiveSkillUseCondtion ) )
#else //ADD_MEMO_1ST_CLASS
	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTempletUsing ) )
#endif //ADD_MEMO_1ST_CLASS
		return false;

	// 공식대전에서 사용 가능한 스킬인지 검사
	if( false == CheckSkillUsePVPOfficial( pSkillTempletUsing->m_eID ) )
		return false;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTempletUsing->m_eID ) );	/// 스킬 레벨

	float fMPConsume = GetActualMPConsume( pSkillTempletUsing->m_eID, iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
	float fMPConsume = GetActualMPConsume( pSkillTempletUsing->m_eID, pSkillTempletUsing->m_iSkillLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_ELSWORD_INFINITY_SWORD
	if( pSkillTempletUsing->m_eType == CX2SkillTree::ST_ACTIVE )
		fMPConsume -= ( fMPConsume * ( m_fMindOfFighterConsumeMpRate * m_iBrutalSlayerActivatedLevel ) );
#endif

#ifdef ELSWORD_WAY_OF_SWORD
	if( GetWayOfSwordState() == WSS_VIGOR )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iWayOfSwordType	= pSkillTempletUsing->m_iWOSType;
	#else // UPGRADE_SKILL_SYSTEM_2013
		int iWayOfSwordType = (int)pSkillTempletUsing->GetSkillAbilityValue( CX2SkillTree::SA_WAY_OF_SWORD_TYPE );
	#endif // UPGRADE_SKILL_SYSTEM_2013

		if( iWayOfSwordType == 2 )
		{
			fMPConsume *= 0.7f;
		}
	}
#endif ELSWORD_WAY_OF_SWORD

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
		if ( pSkillTempletUsing->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
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

#ifdef ELSWORD_SHEATH_KNIGHT
	if( pSkillTempletUsing->m_eID == CX2SkillTree::SI_A_ES_ROLLING )
	{//
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		// HP 가 소모될 양보다 많은지 체크.
		if( (GetMaxHp() * pSkillTempletUsing->GetSkillAbilityValue( CX2SkillTree::HP_CONSUME_REL, iSkillTempletLevel ) ) >= GetNowHp() )
		{
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == true)
			{
				return false;
			}
#endif ALWAYS_SCREEN_SHOT_TEST
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, 
				GET_STRING( STR_ID_2627 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );

			return false;
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		// HP 가 소모될 양보다 많은지 체크.
		if( (GetMaxHp() * pSkillTempletUsing->GetSkillAbilityValue( CX2SkillTree::HP_CONSUME_REL ) ) >= GetNowHp() )
		{
			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, 
				GET_STRING( STR_ID_2627 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );

			return false;
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
#endif ELSWORD_SHEATH_KNIGHT



	//{{ JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련
#ifdef DUNGEON_RANK_NEW
	CountUsedSkill( iPressedSkillSlotIndex );
#endif DUNGEON_RANK_NEW
	//}} JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련

	UpNowMp( -fMPConsume );

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	if ( pSkillTempletUsing->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && g_pMain->GetNowStateID() != CX2Main::XS_TRAINING_GAME )
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

#ifdef ADD_MEMO_1ST_CLASS //김창한
	float fAdjustCoolTimeMemo = 0.f;
	if( (pSkillTempletUsing->m_eID == CX2SkillTree::SI_SA_EMK_PHOENIX_TALON) && (GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO22 ) == true ) )
		fAdjustCoolTimeMemo = -5.f;
#endif //ADD_MEMO_1ST_CLASS

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		#ifdef ADD_MEMO_1ST_CLASS //김창한
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTempletUsing->m_eID, pSkillTempletUsing->GetSkillCoolTimeValue( iSkillTempletLevel ) + fAdjustCoolTimeMemo);
		#else //ADD_MEMO_1ST_CLASS
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTempletUsing->m_eID, pSkillTempletUsing->GetSkillCoolTimeValue( iSkillTempletLevel ) );
		#endif //ADD_MEMO_1ST_CLASS
#else // UPGRADE_SKILL_SYSTEM_2013
	accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTempletUsing->m_eID, pSkillTempletUsing->m_fSkillCoolTime );
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	if( CX2SkillTree::ST_ACTIVE == pSkillTempletUsing->m_eType )
	{
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTempletUsing->m_eID, pSkillTempletUsing->GetSkillCoolTimeValue( iSkillTempletLevel ) * m_fIncreaseCoolTimeRel );
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO6 ) == true && pSkillTempletUsing->m_eID == CX2SkillTree::SI_A_ES_ENDURANCE )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경		
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTempletUsing->m_eID, pSkillTempletUsing->GetSkillCoolTimeValue( iSkillTempletLevel ) - 5.f );
	#else // UPGRADE_SKILL_SYSTEM_2013
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTempletUsing->m_eID, pSkillTempletUsing->m_fSkillCoolTime - 5.f );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
#endif


#ifdef ELSWORD_SHEATH_KNIGHT
	if( m_bCompactCounterEnable == true && pSkillTempletUsing->m_eID == CX2SkillTree::SI_A_ES_COUNTER )
	{
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTempletUsing->m_eID, m_fCompactCounterCoolTime );
	}
#endif ELSWORD_SHEATH_KNIGHT

	if( false == SpecialAttackNoStageChange( pSkillTempletUsing ) )
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


	m_iNowSpecialAttack = iPressedSkillSlotIndex + 1;
	m_bSpecialAttackEventProcessedAtThisFrame = true;

//{{ kimhc // 2010.11.1 // 로드나이트 - 고통 억제
#ifdef	NEW_SKILL_2010_11
	ReducePain( pSkillTempletUsing->m_eType, accessUserSkillTree );
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.1 // 로드나이트 - 고통 억제

#ifdef SERV_SKILL_USE_SUBQUEST
	Send_SKILL_USE_REQ( pSkillTempletUsing->m_eID );
#endif SERV_SKILL_USE_SUBQUEST

	return true;

}




// 상태변화가 없는 필살기는 하드코딩해서 예외 처리 한다
/*virtual*/ bool CX2GUElsword_SwordMan::SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet )
{
	if( NULL == pSkillTemplet )
		return false;

	switch( pSkillTemplet->m_eID )
	{
	case CX2SkillTree::SI_A_ES_ENDURANCE:
		{
			m_FrameDataFuture.syncData.nowAction = UAI_ENDURANCE;

		} break;

		//{{ kimhc // 2010.11.1 // 로드나이트 - 고통 억제
#ifdef	NEW_SKILL_2010_11
	case CX2SkillTree::SI_P_ELK_REDUCE_PAIN:
		{
			// 극기 상태면
			if ( UAI_ENDURANCE == m_FrameDataFuture.syncData.nowAction )
				m_FrameDataFuture.syncData.nowAction = UAI_ENDURANCE_AND_REDUCE_PAIN;			
			else
				m_FrameDataFuture.syncData.nowAction = UAI_REDUCE_PAIN;			
		} break;
#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.1 // 로드나이트 - 고통 억제
		
	default:
		{
			return false;
		} break;
	}

	return true;
}



/*virtual*/ void CX2GUElsword_SwordMan::NoStateChangeActionFrameMove()
{
	switch( m_FrameDataNow.syncData.nowAction )
	{
	case UAI_ENDURANCE:
		{
			ProcessEnduranceSkill();
		} break;

		//{{ kimhc // 2010.11.1 // 로드나이트 - 고통 억제
#ifdef	NEW_SKILL_2010_11
	case UAI_REDUCE_PAIN:
		{
			CureExtraDamage();
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELK_MAGIC_RESISTANCE", this ); 	
		} break;

	case UAI_ENDURANCE_AND_REDUCE_PAIN:
		{
			ProcessEnduranceSkill();
			CureExtraDamage();
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELK_MAGIC_RESISTANCE", this ); 
		} break;
#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.1 // 로드나이트 - 고통 억제

	}
}

void CX2GUElsword_SwordMan::ProcessEnduranceSkill()
{
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ES_ENDURANCE );
	if( NULL != pSkillTemplet )
	{

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO6 ) == true )
		{
			SetBuffFactorToGameUnit( pSkillTemplet, 1 );
		}
		else
		{
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_ENDURANCE", this ); 
		PlaySound( L"Elsword_Endurance.ogg" );	
	}
}


#ifdef NEW_SKILL_2010_11
//{{ oasis907 : 김상윤 [2010.11.1] // 룬 슬레이어 - 마법 저항 수련
float CX2GUElsword_SwordMan::GetMagicReflectRate()
{
	float fMagicReflectRate = m_fMagicReflectRate;
	if ( g_pX2Game != NULL && ( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD ) )
	{
		fMagicReflectRate *= 3.f;
	}
	return fMagicReflectRate;
}
#endif NEW_SKILL_2010_11



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//							스테이트 함수 시작

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//ESSI_DIE_FRONT
void CX2GUElsword_SwordMan::DieFrontStartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GUElsword_SwordMan::DieFrontStart()
{
	CommonStateStart();
	DyingStart();

#ifdef ELSWORD_WAY_OF_SWORD
	const WAY_OF_SWORD_STATE eWayofSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

	if( eWayofSwordState == WSS_VIGOR )
		End_SwordmanVigor();
	else if( eWayofSwordState == WSS_DESTRUCTION )
		End_SwordmanDestruction();
#endif ELSWORD_WAY_OF_SWORD
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
}

void CX2GUElsword_SwordMan::DieFrontFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GUElsword_SwordMan::DieFrontEventProcess()
{
//{{ robobeg : 2008-10-21
	//if( m_bShow == false && m_pXSkinAnim->IsAnimationEnd() == true )
    if ( GetShowObject() == false && m_pXSkinAnim->IsAnimationEnd() == true )
//}} robobeg : 2008-10-21
	{
		StateChange( GUSI_DIE );
	}
}


//ESSI_DIE_BACK
void CX2GUElsword_SwordMan::DieBackStartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GUElsword_SwordMan::DieBackStart()
{
	CommonStateStart();
	DyingStart();

#ifdef ELSWORD_WAY_OF_SWORD
	const WAY_OF_SWORD_STATE eWayofSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

	if( eWayofSwordState == WSS_VIGOR )
		End_SwordmanVigor();
	else if( eWayofSwordState == WSS_DESTRUCTION )
		End_SwordmanDestruction();
#endif ELSWORD_WAY_OF_SWORD
	m_pXSkinAnim->SetPlaySpeed( 0.5f );
}


void CX2GUElsword_SwordMan::DieBackFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GUElsword_SwordMan::DieBackEventProcess()
{
//{{ robobeg : 2008-10-21
	//if( m_bShow == false && m_pXSkinAnim->IsAnimationEnd() == true )
    if ( GetShowObject() == false  && m_pXSkinAnim->IsAnimationEnd() == true )
//}} robobeg : 2008-10-21
	{
		StateChange( GUSI_DIE );
	}
}

#pragma DMLEE_NOTE( "이 함수 각 캐릭터 별로 동일하게 정리되면 GUUSER로 옮겨서 하나로 정리하자" )

void CX2GUElsword_SwordMan::WinFrameMove()
{
	if( true == m_bIsMagicKnightWinMotion2 )
	{
		if( m_hSeqHandFire != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );

			if( m_pXSkinAnim->EventTimer( 1.08f ) == true )
			{				
				if( NULL != pSeq )
				{
					pSeq->SetShowObject( true );
					pSeq->SetEmitRate( 30.0f, 50.0f );
				}
				else
				{
					m_hSeqHandFire = INVALID_PARTICLE_SEQUENCE_HANDLE;
				}
			}

			if( NULL != pSeq )
			{
				D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
				pSeq->SetPosition( vBonePos );
			}
		}
		else
		{
			m_hSeqHandFire = g_pData->GetUIMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MagicKnightHandFire", 0,0,0 );
			
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );
			if( NULL != pSeq )
			{
				pSeq->SetShowObject(false);
			}
		}
	}

	CommonFrameMove();
}

//USI_WAIT
void CX2GUElsword_SwordMan::WaitStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
}

void CX2GUElsword_SwordMan::WaitStart()
{
	CommonStateStart();
	PlayAnimationWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
}

void CX2GUElsword_SwordMan::WaitEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef GRAPPLING_TEST
	else if( m_InputData.oneF == true )
	{

#ifdef LINKED_LINEMAP_JUMP_USER_TEST
		if( true == CanLinkJumpNow() )
		{
			StateChange( ESSI_LINK_JUMP );
		}
		else 
#endif LINKED_LINEMAP_JUMP_USER_TEST
			if( NULL == GetUnitGrappledByMe( true ) )
		{
			StateChange( ESSI_GRAB_FRONT );
		}
		else
		{
			StateChange( ESSI_THROW_FORWARD );
		}
	}
#endif GRAPPLING_TEST

	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( ESSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_COMBO_X1 );
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
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 10.f, ESSI_REVENGE_WAIT )
	else if( m_InputData.oneDown == true 
		&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef ELSWORD_WAY_OF_SWORD
	else if( ( GetWayOfSwordState() == WSS_VIGOR || GetWayOfSwordState() == WSS_DESTRUCTION ) && 
			m_FrameDataNow.unitCondition.fStateTime >= 0.1f )
	{
		StateChange( ESSI_SWORD_WAIT );
	}
#endif ELSWORD_WAY_OF_SWORD
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM
	CommonEventProcess();
}




//USI_WALK
#ifdef TEST_MACRO_COMBO
void CX2GUElsword_SwordMan::WalkStart()
{
	CommonStateStart();

	m_TimerStartRun.restart();
}
#endif TEST_MACRO_COMBO

void CX2GUElsword_SwordMan::WalkEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef GRAPPLING_TEST
	else if( m_InputData.oneF == true )
	{
#ifdef LINKED_LINEMAP_JUMP_USER_TEST
		if( true == CanLinkJumpNow() )
		{
			StateChange( ESSI_LINK_JUMP );
		}
		else 
#endif LINKED_LINEMAP_JUMP_USER_TEST
			if( NULL == GetUnitGrappledByMe( true ) )
		{
			StateChange( ESSI_GRAB_FRONT );
		}
		else
		{
			StateChange( ESSI_THROW_FORWARD );
		}
	}
#endif GRAPPLING_TEST
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( ESSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_COMBO_X1 );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
#ifdef TEST_MACRO_COMBO
	else if( m_TimerStartRun.elapsed() >= 2.0 )
	{
		StateChangeDashIfPossible();
	}
#endif TEST_MACRO_COMBO
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
#ifdef RIDING_SYSTEM
	else if ( true == GetRidingOn() )
	{
		StateChange( USI_RIDING_ON );
	}
#endif //RIDING_SYSTEM

	CommonEventProcess();
}


//USI_JUMP_READY
void CX2GUElsword_SwordMan::JumpReadyEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESSI_JUMP_ATTACK_Z );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_JUMP_ATTACK_X );
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

void CX2GUElsword_SwordMan::JumpUpEventProcess()
{
	if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		if( GetEnableDash() == true )
		{
			StateChange( USI_DASH_JUMP );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESSI_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_JUMP_ATTACK_X );
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

void CX2GUElsword_SwordMan::JumpDownEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef CLIFF_CLIMBING_TEST
	else if( true == IsOnCliffFuture() && true == m_InputData.pureUp && m_PhysicParam.nowSpeed.y <= 0.f )
	{
		StateChange( m_CommonState.m_ClimbUp );
	}
#endif CLIFF_CLIMBING_TEST
#ifdef WALL_JUMP_TEST
	else if( m_FrameDataFuture.unitCondition.bFootOnWall == true &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESSI_WALL_LANDING );
	}
#endif WALL_JUMP_TEST
	else if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		if( GetEnableDash() == true )
		{
			StateChange( USI_DASH_JUMP );
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESSI_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_JUMP_ATTACK_X );
	}
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

void CX2GUElsword_SwordMan::JumpLandingEventProcess()
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
	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( ESSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_COMBO_X1 );
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXCAS
	{
		StateChange( USI_WAIT, false );
	}
	

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::DashEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef GRAPPLING_TEST
	else if( m_InputData.oneF == true )
	{
#ifdef LINKED_LINEMAP_JUMP_USER_TEST
		if( true == CanLinkJumpNow() )
		{
			StateChange( ESSI_LINK_JUMP );
		}
		else 
#endif LINKED_LINEMAP_JUMP_USER_TEST
			if( NULL == GetUnitGrappledByMe( true ) )
		{
			StateChange( ESSI_GRAB_FRONT );
		}
		else
		{
			StateChange( ESSI_THROW_FORWARD );
		}
	}
#endif GRAPPLING_TEST
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESSI_DASH_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_DASH_ATTACK_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 10.f, ESSI_REVENGE_WAIT )
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


//ESSI_DASH_END
void CX2GUElsword_SwordMan::DashEndStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GUElsword_SwordMan::DashEndStart()
{
	CommonStateStart();

}

void CX2GUElsword_SwordMan::DashEndFrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true )
	{
		CreateStepDust();

	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashEndEventProcess()
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
			StateChange( ESSI_COMBO_Z1 );
		}
		else
		{
			StateChange( ESSI_DASH_COMBO_Z1 );
		}
	}
	else if( m_InputData.oneX == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 8.0f )
		{
			StateChange( ESSI_COMBO_X1 );
		}
		else
		{
			StateChange( ESSI_DASH_ATTACK_X );
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


//ESSI_DASH_JUMP
void CX2GUElsword_SwordMan::DashJumpStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * 1.2f;
	m_bDownForce					= false;
}

void CX2GUElsword_SwordMan::DashJumpStart()
{
	CommonStateStart();


	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
	{
		CreateStepDust();
	}
}

void CX2GUElsword_SwordMan::DashJumpFrameMoveFuture()
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

void CX2GUElsword_SwordMan::DashJumpEventProcess()
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
			StateChange( ESSI_DASH_JUMP_POWER_LANDING );
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
#ifdef CLIFF_CLIMBING_TEST
	else if( true == IsOnCliffFuture() && true == m_InputData.pureUp && m_PhysicParam.nowSpeed.y <= 0.f )
	{
		StateChange( m_CommonState.m_ClimbUp );
	}
#endif CLIFF_CLIMBING_TEST
#ifdef WALL_JUMP_TEST
	else if( m_FrameDataFuture.unitCondition.bFootOnWall == true &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( ESSI_WALL_LANDING );
	}
#endif WALL_JUMP_TEST
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( ESSI_DASH_JUMP_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true && m_fReDashJumpXTime <= 0.0f )
	{
		StateChange( ESSI_DASH_JUMP_COMBO_X1 );
	}
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

void CX2GUElsword_SwordMan::DashJumpEndFuture()
{
	CommonStateEnd();
	m_PhysicParam.fDownAccel	= 0.0f;
}

void CX2GUElsword_SwordMan::DashJumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	if( m_InputData.oneZ == true )
	{
		StateChange( ESSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_COMBO_X1 );
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

	CommonEventProcess();
}


//ESSI_DASH_JUMP_POWER_LANDING
void CX2GUElsword_SwordMan::DashJumpPowerLandingStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x /= 2.0f;
}

void CX2GUElsword_SwordMan::DashJumpPowerLandingStart()
{
	CommonStateStart();


	CreateStepDust();

}

void CX2GUElsword_SwordMan::DashJumpPowerLandingEventProcess()
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
			StateChange( ESSI_COMBO_Z1 );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( ESSI_COMBO_X1 );
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



//ESSI_HYPER_MODE
void CX2GUElsword_SwordMan::HyperModeFrameMove()
{
#ifdef MODIFY_RIDING_PET_AWAKE
	CommonHyperModeFrameMove(0.05f, 0.83f);
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

////ESSI_DAMAGE_GROGGY
//void CX2GUElsword_SwordMan::ESSI_DAMAGE_GROGGY_FrameMoveFuture()
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
//void CX2GUElsword_SwordMan::ESSI_DAMAGE_GROGGY_EventProcess()
//{
//	if( m_fGroggyTime <= 0.0f )
//	{
//		StateChange( USI_WAIT );
//	}
//	CommonEventProcess();
//}

//ESSI_DAMAGE_SMALL_FRONT
void CX2GUElsword_SwordMan::DamageSmallFrontEventProcess()
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


//ESSI_DAMAGE_SMALL_BACK
void CX2GUElsword_SwordMan::DamageSmallBackEventProcess()
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


//ESSI_DAMAGE_BIG_FRONT
void CX2GUElsword_SwordMan::DamageBigFrontStart()
{
	CommonStateStart();

	CreateStepDust();

}

void CX2GUElsword_SwordMan::DamageBigFrontEventProcess()
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.93f )
		{
			StateChange( USI_WAIT );
		}
	}
	
	CommonEventProcess();
}


//ESSI_DAMAGE_BIG_BACK
void CX2GUElsword_SwordMan::DamageBigBackStart()
{
	CommonStateStart();

	CreateStepDust();

}

void CX2GUElsword_SwordMan::DamageBigBackEventProcess()
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


//ESSI_DAMAGE_DOWN_FRONT
void CX2GUElsword_SwordMan::DamageDownFrontFrameMove()
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







void CX2GUElsword_SwordMan::DamageDownFrontEventProcess()
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
				StateChange( ESSI_STANDUP_ATTACK_FRONT_NO_DOWN );
			}
			else
			{
				StateChange( ESSI_STANDUP_ATTACK_FRONT );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( ESSI_STANDUP_ATTACK_FRONT );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( ESSI_STANDUP_ROLLING_FRONT_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( ESSI_STANDUP_ROLLING_FRONT_BACK );
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





//ESSI_DAMAGE_DOWN_BACK
void CX2GUElsword_SwordMan::DamageDownBackFrameMove()
{
	if( m_pXSkinAnim->EventTimer( 0.32f ) == true )
	{
		CreateStepDust();

	}

	CommonFrameMove();
}





void CX2GUElsword_SwordMan::DamageDownBackEventProcess()
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
				StateChange( ESSI_STANDUP_ATTACK_BACK_NO_DOWN );
			}
			else
			{
				StateChange( ESSI_STANDUP_ATTACK_BACK );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( ESSI_STANDUP_ATTACK_BACK );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( ESSI_STANDUP_ROLLING_BACK_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( ESSI_STANDUP_ROLLING_BACK_BACK );
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

void CX2GUElsword_SwordMan::DamageAirDownLandingEventProcess()
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
				StateChange( ESSI_STANDUP_ATTACK_FRONT_NO_DOWN );
			}
			else
			{
				StateChange( ESSI_STANDUP_ATTACK_FRONT );
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( ESSI_STANDUP_ATTACK_FRONT );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
			StateChange( ESSI_STANDUP_ROLLING_FRONT_FRONT );
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
			StateChange( ESSI_STANDUP_ROLLING_FRONT_BACK );
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

//ESSI_STANDUP_ROLLING_FRONT_FRONT
void CX2GUElsword_SwordMan::StandUpRollingFrontFrontEventProcess()
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

void CX2GUElsword_SwordMan::StandUpRollingFrontFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


//ESSI_STANDUP_ROLLING_FRONT_BACK
void CX2GUElsword_SwordMan::StandUpRollingFrontBackEventProcess()
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


void CX2GUElsword_SwordMan::StandUpRollingFrontBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



//ESSI_STANDUP_ROLLING_BACK_FRONT
void CX2GUElsword_SwordMan::StandUpRollingBackFrontEventProcess()
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

void CX2GUElsword_SwordMan::StandUpRollingBackFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



//ESSI_STANDUP_ROLLING_BACK_BACK
void CX2GUElsword_SwordMan::StandUpRollingBackBackEventProcess()
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

void CX2GUElsword_SwordMan::StandUpRollingBackBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}




void CX2GUElsword_SwordMan::StandUpAttackFrontEventProcess()
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

void CX2GUElsword_SwordMan::StandUpAttackFrontEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}

void CX2GUElsword_SwordMan::StandUpAttackBackEventProcess()
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

void CX2GUElsword_SwordMan::StandUpAttackBackEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}



void CX2GUElsword_SwordMan::RevengeWaitStart()
{
	CommonStateStart();
	CreateStepDust();

}

void CX2GUElsword_SwordMan::RevengeWaitFrameMoveFuture()
{
	m_PhysicParam.nowSpeed.x = 0.0f;
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::RevengeWaitFrameMove()
{
#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO5 ) == true && 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		SetDefenceMemoTime(4.f);
	}	
#endif

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.09f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.09f ) == true && EventCheck( 0.09f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 pos = m_FrameDataNow.syncData.position;
		pos.y += 70.0f;
		if( m_FrameDataNow.syncData.bIsRight == true )
			pos = pos + m_FrameDataNow.unitCondition.dirVector * 100.0f;
		else
			pos = pos + m_FrameDataNow.unitCondition.dirVector * -100.0f;

		CKTDGParticleSystem::CParticleEventSequence* pSeqRevengeWait = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_REVENGE_WAIT ) );

		if( pSeqRevengeWait != NULL )
		{
			PlaySequenceByTriggerCount( pSeqRevengeWait, pos, 10,10, 5 );

			pSeqRevengeWait->SetAddRotate( D3DXVECTOR3( 0.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
		}		
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeqQuestion = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hQuestionMark );
		if( NULL != pSeqQuestion )
		{
			pSeqQuestion->CreateNewParticle( pSeqQuestion->GetPosition() );
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::RevengeWaitEventProcess()
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
#ifdef BALANCE_PATCH_20110303
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.6f )
	{
		StateChange( USI_WAIT );
	}
#endif
	CommonEventProcess();
}



//ESSI_REVENGE_ATTACK
void CX2GUElsword_SwordMan::RevengeAttackStart()
{
	CommonStateStart();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ES_COUNTER );

	if( NULL != pSkillTemplet )
	{
		int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ES_COUNTER );

		m_DamageData.damage.fPhysic += m_fRevengeAddDamage * pSkillTemplet->GetSkillPowerRateValue( iSkillLevel );

		if( GetRandomInt() < pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel ) * 100.f )
		{
			m_DamageData.reActType = CX2DamageManager::RT_UP;
			m_DamageData.backSpeed = D3DXVECTOR2( 1100.f, 1900.f );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	m_DamageData.damage.fPhysic += m_fRevengeAddDamage;

	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ES_COUNTER_UP );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ES_COUNTER_UP, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			if( GetRandomInt() < pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE ) * 100.f )
			{
				m_DamageData.reActType = CX2DamageManager::RT_UP;
				m_DamageData.backSpeed = D3DXVECTOR2( 1100.f, 1900.f );
			}
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO5 ) == true )
	{
		SetDefenceMemoTime(4.f);
	}	
#endif
	
}

void CX2GUElsword_SwordMan::RevengeAttackFrameMoveFuture()
{
	if( m_FrameDataFuture.unitCondition.fStateTime >= 0.1f && m_FrameDataFuture.unitCondition.fStateTime < 0.4 )
	{
		//m_FrameDataFuture.syncData.bFrameStop = true;
	}
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::RevengeAttackFrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::RevengeAttackCameraMove()
{
	CommonCameraMove();

	if( m_FrameDataNow.stateParam.bEventFlagList[0] == false )
		{
			switch( m_FrameDataNow.stateParam.normalCamera )
			{
				case 1:
					{
						if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
						{
							g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
							g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
							m_LuaManager.EndTable();
						}
					}
					break;

				case 2:
					{
						if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
						{
							g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
							g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
							m_LuaManager.EndTable();
						}
					}
					break;
			}
		}

	m_FrameDataNow.stateParam.bEventFlagList[0] = true;
}

void CX2GUElsword_SwordMan::RevengeAttackEventProcess()
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

#ifdef ELSWORD_WAY_OF_SWORD
void CX2GUElsword_SwordMan::WayOfSwordStateChange()
{
	int iNextWayOfSwordState = 0;
	if( IsMyUnit() == true )
	{
		const float fWayOfSwordPoint = GetWayOfSwordPoint();
		if( fWayOfSwordPoint >= g_pData->GetDamageManager()->GetVigorStateThreshold() )
			iNextWayOfSwordState = WSS_VIGOR;
		else if( fWayOfSwordPoint <= g_pData->GetDamageManager()->GetDestStateThreshold() )
			iNextWayOfSwordState = WSS_DESTRUCTION;
		else if( fWayOfSwordPoint > 0 )
			iNextWayOfSwordState = WSS_TOWARD_VIGOR;
		else if( fWayOfSwordPoint < 0 )
			iNextWayOfSwordState = WSS_TOWARD_DESTRUCTION;
		else if( fWayOfSwordPoint == 0 )
			iNextWayOfSwordState = WSS_CENTER;
	}		
	else
	{
		iNextWayOfSwordState = m_FrameDataNow.syncData.m_CannonBallCount;
	}
	
	const WAY_OF_SWORD_STATE eWayofSwordState = static_cast<const WAY_OF_SWORD_STATE>( GetWayOfSwordState() );

	if( iNextWayOfSwordState == WSS_VIGOR && 
		eWayofSwordState != WSS_VIGOR )
	{
		if( eWayofSwordState == WSS_DESTRUCTION )
			End_SwordmanDestruction();

		Start_SwordmanVigor();
		SetWayOfSwordState( static_cast<int>( WSS_VIGOR ) );
		//m_GageManager.UpdateUIElswordWSP( WSS_VIGOR );
	}
	else if( iNextWayOfSwordState == WSS_DESTRUCTION &&
		eWayofSwordState != WSS_DESTRUCTION )
	{
		if( eWayofSwordState == WSS_VIGOR )
			End_SwordmanVigor();

		Start_SwordmanDestruction();
		SetWayOfSwordState( static_cast<int>( WSS_DESTRUCTION ) );
		//m_GageManager.UpdateUIElswordWSP( WSS_DESTRUCTION );
	}
	else if( iNextWayOfSwordState == WSS_TOWARD_VIGOR &&
		eWayofSwordState != WSS_TOWARD_VIGOR && eWayofSwordState != WSS_VIGOR )
	{
		if( eWayofSwordState == WSS_DESTRUCTION )
			End_SwordmanDestruction();

		SetWayOfSwordState( static_cast<int>( WSS_TOWARD_VIGOR ) );
		//m_GageManager.UpdateUIElswordWSP( WSS_TOWARD_VIGOR );
	}
	else if( iNextWayOfSwordState == WSS_TOWARD_DESTRUCTION && 
		eWayofSwordState != WSS_TOWARD_DESTRUCTION && eWayofSwordState != WSS_DESTRUCTION )
	{
		if( eWayofSwordState == WSS_VIGOR )
			End_SwordmanVigor();

		SetWayOfSwordState( static_cast<int>( WSS_TOWARD_DESTRUCTION ) );
		//m_GageManager.UpdateUIElswordWSP( WSS_TOWARD_DESTRUCTION );
	}
	else if( iNextWayOfSwordState == WSS_CENTER && 
		eWayofSwordState != WSS_CENTER )
	{
		if( eWayofSwordState == WSS_VIGOR )
			End_SwordmanVigor();
		else if( eWayofSwordState == WSS_DESTRUCTION )
			End_SwordmanDestruction();

		SetWayOfSwordState( static_cast<int>( WSS_CENTER ) );
		//m_GageManager.UpdateUIElswordWSP( WSS_CENTER );
	}
	else // Do Nothing
	{
		;
	}

	return;
}

void CX2GUElsword_SwordMan::SwordWaitStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationSwordWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );
}

void CX2GUElsword_SwordMan::SwordWaitStart()
{
	CommonStateStart();
	PlayAnimationSwordWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
}

void CX2GUElsword_SwordMan::SwordWaitEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}

	else if( m_InputData.oneZ == true && (m_bReAttackZ1Right != m_FrameDataFuture.syncData.bIsRight || m_fReAttackZ1Time <= 0.0f) )
	{
		StateChange( ESSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( ESSI_COMBO_X1 );
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
		//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 10.f, ESSI_REVENGE_WAIT )
	else if( m_InputData.oneDown == true 
	&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, &m_FrameDataFuture.unitCondition.landPosition, &m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( GetWayOfSwordState() != WSS_VIGOR && GetWayOfSwordState() != WSS_DESTRUCTION )
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

void CX2GUElsword_SwordMan::PlayAnimationSwordWaitStart( CKTDGXSkinAnim* pXSkinAnim_, const FrameData& refFrameData_ )
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
			if( GetWayOfSwordState() == WSS_VIGOR )
				pXSkinAnim_->ChangeAnim( L"Wait_W0", false );
			else if( GetWayOfSwordState() == WSS_DESTRUCTION )
				pXSkinAnim_->ChangeAnim( L"Wait_W1", false );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", false );
		}
		else
		{
			if( GetWayOfSwordState() == WSS_VIGOR )
				pXSkinAnim_->ChangeAnim( L"Wait_W0", true );
			else if( GetWayOfSwordState() == WSS_DESTRUCTION )
				pXSkinAnim_->ChangeAnim( L"Wait_W1", true );
			else
				pXSkinAnim_->ChangeAnim( L"Wait", true );
		}
	}

	pXSkinAnim_->Play( playType );
}




void CX2GUElsword_SwordMan::DestructionRageFrameMove()
{
	if( IsSuperArmor() )
		return;

	m_fDestructionRageTime -= m_fElapsedTime;
	if( m_fDestructionRageTime < 0.0f )
	{
		m_fDestructionRageTime = 0.0f;
	}
	else 
	{
		m_FrameDataNow.stateParam.bSuperArmor		= true;
		m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
	}
	return;
}


void CX2GUElsword_SwordMan::DoStateDestructionRage()
{
	if( IsMyUnit() == true )
		SetWayOfSwordPoint( GetWayOfSwordPoint() + g_pData->GetDamageManager()->GetDestRageWSPGain() );
	
	m_FrameDataNow.stateParam.bSuperArmor		= true;
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
	m_fDestructionRageTime						= 6.0f;
	m_PhysicParam.nowSpeed.x					= 0.0f;
	m_PhysicParam.nowSpeed.y					= 0.0f;

	CureExtraDamage( CX2DamageManager::EDT_ENCHANT_SHOCK );
	CureExtraDamage( CX2DamageManager::EDT_FROZEN );
	CureExtraDamage( CX2DamageManager::EDT_ENTANGLE );
	CureExtraDamage( CX2DamageManager::EDT_DRYAD );
	CureExtraDamage( CX2DamageManager::EDT_STUN );
	CureExtraDamage( CX2DamageManager::EDT_Y_PRESSED );
	CureExtraDamage( CX2DamageManager::EDT_PANIC );

	m_fGroggyTime = 0.f;

	StateChange( ESSI_DESTRUCTION_RAGE, false );

	D3DXVECTOR3 pos = m_FrameDataNow.syncData.position;
	pos.y += 80.f;
	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MonsterAngryWind", pos);
	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Counter_Particle01", pos );



	g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_ANGRY_WIND", GetPowerRate(), pos, 
		GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );



	UpDownCrashCamera( 30.0f, 0.4f );
}

void CX2GUElsword_SwordMan::DestructionRageStateStart()
{
	CommonStateStart();
}

void CX2GUElsword_SwordMan::DestructionRageStateFrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DestructionRageStateEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
#ifdef RIDING_SYSTEM
		if( true == GetRidingOn() )
			StateChange( USI_RIDING_JUMP_DOWN );
		else
			StateChange( USI_JUMP_DOWN );
#else //RIDING_SYSTEM
		StateChange( USI_JUMP_DOWN );
#endif //RIDING_SYSTEM
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
/*	// 기획 요청으로 스킬 캔슬 제거
	else if ( true == SpecialAttackEventProcess() )
	{
	}
*/
	else if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef RIDING_SYSTEM
		if( true == GetRidingOn() )
			StateChange( USI_RIDING_WAIT );
		else
			StateChange( USI_WAIT );
#else //RIDING_SYSTEM
		StateChange( USI_WAIT );
#endif //RIDING_SYSTEM
	}
	CommonEventProcess();
}
#endif ELSWORD_WAY_OF_SWORD


//ESSI_COMBO_Z1
void CX2GUElsword_SwordMan::ComboZ1FrameMoveFuture()
{
	m_PhysicParam.nowSpeed.x = 0.0f;
	m_PhysicParam.nowSpeed.y = 0.0f;

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ComboZ1FrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboZ1EventProcess()
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
#ifdef TEST_MACRO_COMBO
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.27f )
	{
		StateChange( ESSI_COMBO_Z2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#endif TEST_MACRO_COMBO
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.27f )
	{
		StateChange( ESSI_COMBO_Z2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.27f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.27f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.4f )
	{
		StateChange( ESSI_COMBO_Z2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
		m_fCanNotAttackTime = 0.1f;
	}
	CommonEventProcess();
}


//ESSI_COMBO_Z2
void CX2GUElsword_SwordMan::ComboZ2FrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboZ2EventProcess()
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
#ifdef TEST_MACRO_COMBO
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.366f )
	{
		StateChange( ESSI_COMBO_Z3 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#endif TEST_MACRO_COMBO
#ifdef ELSWORD_SHEATH_KNIGHT
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.366f &&
#ifdef SERV_ELSWORD_INFINITY_SWORD
		CX2Unit::UC_ELSWORD_SHEATH_KNIGHT != GetUnit()->GetClass() && CX2Unit::UC_ELSWORD_INFINITY_SWORD != GetUnit()->GetClass() )
#else
		CX2Unit::UC_ELSWORD_SHEATH_KNIGHT != GetUnit()->GetClass() )
#endif //SERV_ELSWORD_INFINITY_SWORD
#else
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.366f )
#endif ELSWORD_SHEATH_KNIGHT
	{
		StateChange( ESSI_COMBO_Z3 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#ifdef ELSWORD_SHEATH_KNIGHT
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.366f && 
#ifdef SERV_ELSWORD_INFINITY_SWORD
		(CX2Unit::UC_ELSWORD_SHEATH_KNIGHT == GetUnit()->GetClass() || CX2Unit::UC_ELSWORD_INFINITY_SWORD == GetUnit()->GetClass()) )
#else
		(CX2Unit::UC_ELSWORD_SHEATH_KNIGHT == GetUnit()->GetClass()) )
#endif //SERV_ELSWORD_INFINITY_SWORD
	{
		StateChange( ESSI_ETK_COMBO_ZZZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.366f )
#else
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.366f )
#endif ELSWORD_SHEATH_KNIGHT
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.1f;
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//ESSI_COMBO_Z3
void CX2GUElsword_SwordMan::ComboZ3FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboZ3EventProcess()
{
	m_bReAttackZ1Right = m_FrameDataFuture.syncData.bIsRight;
	m_fReAttackZ1Time = 0.2f;

	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );

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
#ifdef TEST_MACRO_COMBO
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.333f )
	{
		StateChange( ESSI_COMBO_Z4 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#endif TEST_MACRO_COMBO
#ifdef ELSWORD_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_( 2, 0.333f, 0.333f, m_InputData.oneZ == true && GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_LORD_KNIGHT &&
		( ( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) || 
		( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ), ESSI_ELK_ZZZfrontZ )
#endif ELSWORD_SECOND_CLASS_CHANGE
#ifdef BALANCE_RUNE_SLAYER_20130214
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
								true == m_InputData.oneZ, ESSI_COMBO_Z4 )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
								true == m_InputData.oneX, ESSI_COMBO_Z4a )
#else
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.333f, 0.333f, m_InputData.oneZ == true, ESSI_COMBO_Z4 )
	ELSE_IF_STATE_CHANGE_ON_( 1, 0.333f, 0.333f, m_InputData.oneX == true, ESSI_COMBO_Z4a )
#endif //BALANCE_RUNE_SLAYER_20130214
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}



//ESSI_COMBO_Z4
void CX2GUElsword_SwordMan::ComboZ4FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.17f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.17f ) == true && EventCheck( 0.17f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed() * 1.3f;
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ComboZ4FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboZ4EventProcess()
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.7f )
		{
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}


//ESSI_COMBO_Z4a
void CX2GUElsword_SwordMan::ComboZ4aFrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboZ4aEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true 
		|| m_InputData.pureRight == true || m_InputData.pureLeft == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
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


//ESSI_COMBO_X1
void CX2GUElsword_SwordMan::ComboX1FrameMoveFuture()
{
	m_PhysicParam.nowSpeed.y = 0.0f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.2f ) == true && EventCheck( 0.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ComboX1FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboX1EventProcess()
{
#ifdef ELSWORD_SHEATH_KNIGHT
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );
#else
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 2 );
#endif ELSWORD_SHEATH_KNIGHT


	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#ifdef TEST_MACRO_COMBO
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.43f )
	{
		StateChange( ESSI_COMBO_X2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
#endif TEST_MACRO_COMBO
#ifdef ELSWORD_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_( 1, 0.43f, 0.43f, m_InputData.oneZ == true && GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_RUNE_SLAYER, ESSI_ERS_XZ )
#endif ELSWORD_SECOND_CLASS_CHANGE
#ifdef ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
	ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX == true && ( GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_SHEATH_KNIGHT || GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_INFINITY_SWORD ), ESSI_ETK_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.43f, 0.43f, m_InputData.oneX == true && GetUnit()->GetClass() != CX2Unit::UC_ELSWORD_SHEATH_KNIGHT && GetUnit()->GetClass() != CX2Unit::UC_ELSWORD_INFINITY_SWORD, ESSI_COMBO_X2 )
#else //SERV_ELSWORD_INFINITY_SWORD
	ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX == true && ( GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_SHEATH_KNIGHT ), ESSI_ETK_COMBO_XX )
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.43f, 0.43f, m_InputData.oneX == true && GetUnit()->GetClass() != CX2Unit::UC_ELSWORD_SHEATH_KNIGHT, ESSI_COMBO_X2 )
#endif //SERV_ELSWORD_INFINITY_SWORD
#else 
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.43f, 0.43f, m_InputData.oneX == true, ESSI_COMBO_X2 )
#endif ELSWORD_SHEATH_KNIGHT
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
		{
			m_fCanNotAttackTime = 0.2f;
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


//ESSI_COMBO_X2
void CX2GUElsword_SwordMan::ComboX2FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.31f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.31f ) == true && EventCheck( 0.31f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed() * 1.5f;
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ComboX2FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboX2EventProcess()
{
#ifndef BALANCE_RUNE_SLAYER_20130214
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
#endif //BALANCE_RUNE_SLAYER_20130214

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef BALANCE_RUNE_SLAYER_20130214
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange,
		true == m_InputData.oneX && CX2Unit::UC_ELSWORD_LORD_KNIGHT != GetUnit()->GetClass(), ESSI_COMBO_X3 )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange,
		true == m_InputData.oneZ, ESSI_COMBO_X3a )
	ELSE_IF_STATE_CHANGE_ON_EX_( 2, m_fEventTime[2].keyInputStart, m_fEventTime[2].keyInputEnd, m_fEventTime[2].stateChange,
		true == m_InputData.oneX && CX2Unit::UC_ELSWORD_LORD_KNIGHT == GetUnit()->GetClass() , ESSI_ELK_XXX )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
#else
#ifdef TEST_MACRO_COMBO
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f )
	{
		StateChange( ESSI_COMBO_X3 );
	}
#endif TEST_MACRO_COMBO
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f )
	{
#ifdef ELSWORD_SECOND_CLASS_CHANGE
		if( CX2Unit::UC_ELSWORD_LORD_KNIGHT == GetUnit()->GetClass() )
		{
			StateChange( ESSI_ELK_XXX );
		}
		else
#endif ELSWORD_SECOND_CLASS_CHANGE
		{
			StateChange( ESSI_COMBO_X3 );
		}
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f )
	{
		StateChange( ESSI_COMBO_X3a );
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.5f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.5f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.3f;
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.7f )
		{
			m_fCanNotAttackTime = 0.3f;
			StateChange( USI_WAIT );
		}
	}
#endif //BALANCE_RUNE_SLAYER_20130214


	CommonEventProcess();
}


//ESSI_COMBO_X3
void CX2GUElsword_SwordMan::ComboX3FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.3f ) == true && EventCheck( 0.3f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ComboX3FrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboX3EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.7f )
		{
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}


//ESSI_COMBO_X3a
void CX2GUElsword_SwordMan::ComboX3aFrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ComboX3aEventProcess()
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
		{
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}


//ESI_DASH_COMBO_Z1
void CX2GUElsword_SwordMan::DashComboZ1FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashComboZ1EventProcess()
{
#ifdef ELSWORD_SHEATH_KNIGHT
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.3666f
#ifdef SERV_ELSWORD_INFINITY_SWORD
		&& ( CX2Unit::UC_ELSWORD_SHEATH_KNIGHT == GetUnit()->GetClass() || CX2Unit::UC_ELSWORD_INFINITY_SWORD == GetUnit()->GetClass() )
#else
		&& ( CX2Unit::UC_ELSWORD_SHEATH_KNIGHT == GetUnit()->GetClass() )
#endif //SERV_ELSWORD_INFINITY_SWORD
		&& true == IsOnSomethingFuture() )
	{
		StateChange( ESSI_ETK_DASH_COMBO_ZZ );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.36f
#ifdef SERV_ELSWORD_INFINITY_SWORD
		&& ( CX2Unit::UC_ELSWORD_SHEATH_KNIGHT != GetUnit()->GetClass() && CX2Unit::UC_ELSWORD_INFINITY_SWORD != GetUnit()->GetClass() )
#else
		&& CX2Unit::UC_ELSWORD_SHEATH_KNIGHT != GetUnit()->GetClass()
#endif //SERV_ELSWORD_INFINITY_SWORD
		&& true == IsOnSomethingFuture() )
#else
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.36f
		&& true == IsOnSomethingFuture() )
#endif ELSWORD_SHEATH_KNIGHT
	{
		StateChange( ESSI_DASH_COMBO_Z2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.36f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
		{
			m_fCanNotAttackTime = 0.1f;
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}


//ESSI_DASH_COMBO_Z2
void CX2GUElsword_SwordMan::DashComboZ2StartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GUElsword_SwordMan::DashComboZ2FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashComboZ2EventProcess()
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

//ESSI_KNIGHT_DASH_COMBO_Z2
void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_COMBO_Z2_StateStartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_COMBO_Z2_EventProcess()
{
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.45f
		&& true == IsOnSomethingFuture() )
	{
		StateChange( ESSI_DASH_COMBO_Z3 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.45f
		&& true == IsOnSomethingFuture() )
	{
		StateChange( ESSI_DASH_COMBO_Z3a );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.45f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.45f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}
	else if( m_InputData.pureUp == true || m_InputData.pureDown == true ||
		m_InputData.pureA == true || m_InputData.pureS == true || m_InputData.pureD == true || m_InputData.pureC == true 
		|| m_InputData.pureQ == true || m_InputData.pureW == true || m_InputData.pureE == true || m_InputData.pureR == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
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


//ESSI_KNIGHT_DASH_COMBO_Z3
void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_COMBO_Z3_StateStartFuture()
{
	CommonStateStartFuture();

	/*
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
		*/
}

void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_COMBO_Z3_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.0f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();

}

//ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3
void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3_StateStartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3_EventProcess()
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
		{
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}


//ESSI_KNIGHT_DASH_COMBO_Z3a
void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_COMBO_Z3a_StateStartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_COMBO_Z3a_EventProcess()
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}



//ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a
void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.515f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.515f ) == true && EventCheck( 0.515f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( _CONST_ELSWORD_::MAGIC_FIRE_BALL_MP_COST ) == true )
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 rot = GetRotateDegree();
			rot.y += 180.0f;

			if( CX2Unit::UC_ELSWORD_RUNE_SLAYER == GetUnit()->GetClass() )
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
				CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_RUNE_SLAYER_DASH_ZZX", GetPowerRate() * m_fAttackPowerRateFireBall, posR, D3DXVECTOR3(0,0,0), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false,-1.f, 1.f, m_fRuneProjectileRangeIncPercent );
				if( NULL != pEffect )
				{
					pEffect->SetScale( m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent );
				}
#else //UPGRADE_SKILL_SYSTEM_2013
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_RUNE_SLAYER_DASH_ZZX", GetPowerRate() * m_fAttackPowerRateFireBall, posR, D3DXVECTOR3(0,0,0), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG_ELSWORD", GetPowerRate() * m_fAttackPowerRateFireBall, posR, rot, rot, m_FrameDataNow.unitCondition.landPosition.y );
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MagicFail", pos, 100, 100, 1, 1 );
			if( pSeq != NULL )
			{
				D3DXVECTOR3 rot = GetRotateDegree();
				rot.y += 180.0f;
				pSeq->SetAxisAngle( rot );
				pSeq->SetAddRotate( rot );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_EventProcess()
{
#ifdef BALANCE_RUNE_SLAYER_20130214
	if( false == IsOnSomethingFuture() )
	{
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
#else
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#endif //BALANCE_RUNE_SLAYER_20130214
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.2f )
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
				StateChange( USI_WAIT );
		}
	}
	

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_COMBO_Z3a_StateEndFuture()
{
	CommonStateEndFuture();
	m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;
}


//ESSI_DASH_ATTACK_X
void CX2GUElsword_SwordMan::DashAttackXStartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
}

void CX2GUElsword_SwordMan::DashAttackXFrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashAttackXEventProcess()
{
#ifdef SERV_ELSWORD_INFINITY_SWORD
	if( GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_INFINITY_SWORD )
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
		ELSE_IF_STATE_CHANGE_ON_EX_( 0, 0.233f, 0.63f, 0.63f, m_InputData.oneX == true, ESSI_EIS_DASH_COMBO_XX )
		ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
		{
			if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66666f )
			{
				StateChange( USI_WAIT );
			}
		}
	}
	else
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
		ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
		{
			if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
			{
				StateChange( USI_WAIT );
			}
		}	
	}
#else
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
		{
			StateChange( USI_WAIT );
		}
	}	
#endif	

	CommonEventProcess();
}


//ESSI_JUMP_ATTACK_Z
void CX2GUElsword_SwordMan::JumpAttackZFrameMoveFuture()
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

void CX2GUElsword_SwordMan::JumpAttackZFrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::JumpAttackZEventProcess()
{
//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST


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



//ESSI_JUMP_ATTACK_X
void CX2GUElsword_SwordMan::JumpAttackXFrameMoveFuture()
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

void CX2GUElsword_SwordMan::JumpAttackXFrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::JumpAttackXEventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST


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


//ESSI_DASH_JUMP_COMBO_Z1
void CX2GUElsword_SwordMan::DashJumpComboZ1FrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashJumpComboZ1EventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST



	if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( ESSI_DASH_JUMP_COMBO_Z2 );
		}
	}

	CommonEventProcess();
}


//ESI_DASH_JUMP_COMBO_Z2
void CX2GUElsword_SwordMan::DashJumpComboZ2FrameMove()
{

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashJumpComboZ2EventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST


	if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( ESSI_DASH_JUMP_COMBO_Z3 );
		}
	}

	CommonEventProcess();
}


//ESSI_DASH_JUMP_COMBO_Z3
void CX2GUElsword_SwordMan::DashJumpComboZ3FrameMove()
{

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.26f && m_pXSkinAnim->GetNowAnimationTime() < 0.37f )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.stateParam.normalCamera	= m_FrameDataNow.stateParam.hitCamera;
			m_FrameDataNow.stateParam.bResetCamera	= false;
			m_FrameDataNow.unitCondition.bHit		= true;
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashJumpComboZ3EventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST


	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataNow.unitCondition.bHit == true )
		{
			StateChange( ESSI_DASH_JUMP_COMBO_Z3_LANDING );
		}
		else
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}
	}

	CommonEventProcess();
}




//ESSI_DASH_JUMP_COMBO_Z3_LANDING
void CX2GUElsword_SwordMan::DashJumpComboZ3LandingStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed.x /= 2.0f;
}

void CX2GUElsword_SwordMan::DashJumpComboZ3LandingStart()
{
	CommonStateStart();

	m_FrameDataNow.stateParam.normalCamera = m_FrameDataNow.stateParam.hitCamera;
}

void CX2GUElsword_SwordMan::DashJumpComboZ3LandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 6.0f )
	{
		if( m_InputData.oneZ == true )
		{
			StateChange( ESSI_COMBO_Z1 );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( ESSI_COMBO_X1 );
		}
		else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
		{
			StateChangeDashIfPossible();
		}
		else if( m_InputData.pureRight == true || m_InputData.pureLeft == true )
		{
			StateChange( USI_WALK );
		}
		//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 10.f, ESSI_REVENGE_WAIT )
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



//ESSI_DASH_JUMP_COMBO_X1
void CX2GUElsword_SwordMan::DashJumpComboX1FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashJumpComboX1EventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN );
//#endif LINEMAP_FAST_WIND_TEST








	//CANCEL_COMBO_ON_DIRECTION_CHANGE( 3 );
	if( m_InputData.oneX == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
	else if( m_InputData.oneZ == true && GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_MAGIC_KNIGHT )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = false;
	}
#ifdef ELSWORD_SECOND_CLASS_CHANGE
	else if( m_InputData.oneZ == true && GetUnit()->GetClass() == CX2Unit::UC_ELSWORD_RUNE_SLAYER )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[2] = true;
	}

#endif ELSWORD_SECOND_CLASS_CHANGE


	
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( ESSI_DASH_JUMP_COMBO_X2 );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( ESSI_DASH_JUMP_COMBO_X2a );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
		{
			StateChange( ESSI_ERS_DASH_JUMP_XZ );
		}
	}

	//m_fReDashJumpXTime = 0.3f;

	CommonEventProcess();
}





//ESSI_DASH_JUMP_COMBO_X2
void CX2GUElsword_SwordMan::DashJumpComboX2FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.23f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.23f ) == true && EventCheck( 0.23f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::DashJumpComboX2FrameMove()
{
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.15f )
	{

		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.stateParam.bResetCamera	= false;
			m_FrameDataNow.unitCondition.bHit		= true;
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::DashJumpComboX2EventProcess()
{
//
//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST



	if( true == IsOnSomethingFuture() )
	{
		StateChange( ESSI_DASH_JUMP_COMBO_X2_LANDING );
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::DashJumpComboX2End()
{
	CommonStateEnd();
	UpCrashCamera( 200.0f, -3500.0f );
}


//ESSI_DASH_JUMP_COMBO_X2_LANDING
void CX2GUElsword_SwordMan::DashJumpComboX2LandingStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed.x /= 2.0f;
}

void CX2GUElsword_SwordMan::DashJumpComboX2LandingStart()
{
	CommonStateStart();

	m_FrameDataNow.stateParam.normalCamera = m_FrameDataNow.stateParam.hitCamera;
}



void CX2GUElsword_SwordMan::DashJumpComboX2LandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	//ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE( 10.f, ESSI_REVENGE_WAIT )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}


	CommonEventProcess();
}

//ESSI_KNIGHT_DASH_JUMP_COMBO_X2
void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_JUMP_COMBO_X2_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.23f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.23f ) == true && EventCheck( 0.23f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_JUMP_COMBO_X2_FrameMove()
{
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.15f )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.stateParam.bResetCamera	= false;
			m_FrameDataNow.unitCondition.bHit		= true;
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_JUMP_COMBO_X2_EventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST



	if( m_InputData.oneX == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}
	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( ESSI_DASH_JUMP_COMBO_X3 );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( ESSI_COMBO_Z1 );
		}
		else
		{
			StateChange( ESSI_DASH_JUMP_COMBO_X2_LANDING );
		}
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_JUMP_COMBO_X2_End()
{
	CommonStateEnd();
	UpCrashCamera( 200.0f, -3500.0f );
}

//ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a
void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_FrameMoveFuture()
{
	if( m_pXSkinAnim->GetNowAnimationTime() < 0.39f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}




void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( _CONST_ELSWORD_::MAGIC_FIRE_BALL_MP_COST ) == true )
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			
			angle.z = -30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN_ELSWORD", GetPowerRate() * m_fAttackPowerRateFireBall, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MagicFail", pos, 100, 100, 1, 1 );
			if( pSeq != NULL )
			{
				pSeq->SetAxisAngle( GetRotateDegree() );
				pSeq->SetAddRotate( GetRotateDegree() );
			}
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_MAGIC_KNIGHT_DASH_JUMP_COMBO_X2a_EventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef BALANCE_RUNE_SLAYER_20130214
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.59f )
	{
		StateChange( USI_JUMP_DOWN );
	}
#endif //BALANCE_RUNE_SLAYER_20130214

	CommonEventProcess();
}

//ESSI_KNIGHT_DASH_JUMP_COMBO_X3
void CX2GUElsword_SwordMan::ESSI_KNIGHT_DASH_JUMP_COMBO_X3_EventProcess()
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
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.68f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}


////////////////////////////////////////////////////////////////////////
//ESSI_SI_ES_POWER_COMBO
void CX2GUElsword_SwordMan::ESSI_SI_ES_POWER_ATTACK_FrameMoveFuture()
{
	m_PhysicParam.nowSpeed.y = 0.0f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.2f ) == true && EventCheck( 0.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI_ES_POWER_ATTACK_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI_ES_POWER_ATTACK_EventProcess()
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


//ESSI_SI_ES_POWER_ATTACK_HYPER
void CX2GUElsword_SwordMan::ESSI_SI_ES_POWER_ATTACK_HYPER_FrameMoveFuture()
{
	m_PhysicParam.nowSpeed.y = 0.0f;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.2f ) == true && EventCheck( 0.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI_ES_POWER_ATTACK_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI_ES_POWER_ATTACK_HYPER_EventProcess()
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


//ESSI_SI1_ES_MEGA_SLASH
void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_Init()
{

#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

	XSkinMeshReadyInBackground( L"Effect_ElSword_MegaSlashBackDust.x" );
	XSkinMeshReadyInBackground( L"Effect_ElSword_MegaSlash.x" );

#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	
	CKTDXDeviceXSkinMesh* pMegaSlashMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Effect_ElSword_MegaSlash.X" );
	CKTDXDeviceXSkinMesh* pMegaSlashBackDustMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Effect_ElSword_MegaSlashBackDust.X" );
	DeviceReadyInBackground( pMegaSlashMesh );
	DeviceReadyInBackground( pMegaSlashBackDustMesh );

	CKTDGXSkinAnim::DeleteKTDGObject( m_pMegaSlash );
	CKTDGXSkinAnim::DeleteKTDGObject( m_pMegaSlashBackDust );

	m_pMegaSlash			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pMegaSlashBackDust	= CKTDGXSkinAnim::CreateSkinAnim();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMegaSlash );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMegaSlashBackDust );

	m_pMegaSlash->SetAnimXSkinMesh( pMegaSlashMesh );
	m_pMegaSlash->AddModelXSkinMesh( pMegaSlashMesh );
	m_pMegaSlashBackDust->SetAnimXSkinMesh( pMegaSlashBackDustMesh );
	m_pMegaSlashBackDust->AddModelXSkinMesh( pMegaSlashBackDustMesh );

	m_pMegaSlash->SetAlphaObject( true );
	m_pMegaSlash->SetLayer( XL_EFFECT_0 );

	m_pMegaSlashBackDust->SetAlphaObject( true );
	m_pMegaSlashBackDust->SetLayer( XL_EFFECT_0 );

	m_pMegaSlash->GetRenderParam()->bAlphaBlend		= true;
	m_pMegaSlash->GetRenderParam()->bZWriteEnable	= false;
	m_pMegaSlash->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMegaSlash->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMegaSlash->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;

	m_pMegaSlashBackDust->GetRenderParam()->bAlphaBlend		= true;
	m_pMegaSlashBackDust->GetRenderParam()->bZWriteEnable	= false;
	m_pMegaSlashBackDust->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMegaSlashBackDust->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMegaSlashBackDust->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.5f ) == true && EventCheck( 0.5f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_MEGA_SLASH", this );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.2f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}

}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_EventProcess()
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



//ESSI_SI1_ES_MEGA_SLASH_HYPER
void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.5f ) == true && EventCheck( 0.5f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_MEGA_SLASH", this, NULL, true );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					if( GetIsRight() == true )
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					else
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					if( GetIsRight() == true )
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					else
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA3" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.2f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_MEGA_SLASH_HYPER_EventProcess()
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





//ESSI_SI2_ES_FLAME_GEYSER
void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_Init()
{


#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

	XSkinMeshReadyInBackground( L"Effect_ElSword_FlameGager.x" );
	XSkinMeshReadyInBackground( L"Effect_ElSword_FlameGagerLand.x" );


	
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	CKTDXDeviceXSkinMesh* pFlameGagerMesh		= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Effect_ElSword_FlameGager.X" );
	CKTDXDeviceXSkinMesh* pFlameGagerLandMesh	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Effect_ElSword_FlameGagerLand.X" );
	DeviceReadyInBackground( pFlameGagerMesh );
	DeviceReadyInBackground( pFlameGagerLandMesh );


	m_pFlameGager			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pFlameGagerLand		= CKTDGXSkinAnim::CreateSkinAnim();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pFlameGager );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pFlameGagerLand );

	m_pFlameGager->SetAnimXSkinMesh( pFlameGagerMesh );
	m_pFlameGager->AddModelXSkinMesh( pFlameGagerMesh );
	m_pFlameGagerLand->SetAnimXSkinMesh( pFlameGagerLandMesh );
	m_pFlameGagerLand->AddModelXSkinMesh( pFlameGagerLandMesh );

	m_pFlameGager->SetAlphaObject( true );
	m_pFlameGager->SetLayer( XL_EFFECT_0 );

	m_pFlameGagerLand->SetAlphaObject( false );
	m_pFlameGagerLand->SetLayer( XL_EFFECT_0 );

	m_pFlameGager->GetRenderParam()->bAlphaBlend	= true;
	//m_pFlameGager->GetRenderParam()->bZEnable		= false;
	m_pFlameGager->GetRenderParam()->bZWriteEnable	= false;
	m_pFlameGager->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pFlameGager->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pFlameGager->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;

	m_pFlameGagerLand->GetRenderParam()->bAlphaBlend	= false;
	m_pFlameGagerLand->GetRenderParam()->bZEnable		= true;
	m_pFlameGagerLand->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pFlameGagerLand->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pFlameGagerLand->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_Start()
{
	CommonStateStart();


#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
	m_DamageData.attackerType	= CX2DamageManager::AT_MESH;
	m_DamageData.bCanRevenge	= false;
	m_DamageData.pAttackerAnim	= m_pFlameGager;
	m_DamageData.attackType = CX2DamageManager::AT_SPECIAL;

	m_pFlameGager->GetRenderParam()->color = 0xffffffff;
	m_pFlameGager->GetMatrix().Scale( 1.3f,2.0f,1.3f );
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

}

//void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_FrameMoveFuture()
//{
//	CommonFrameMoveFuture();
//}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.004f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7666f ) == true && EventCheck( 0.7666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		UpDownCrashCamera( 50.0f, 0.3f );


		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE ) );


		D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );


#ifdef SKILL_BALANCE_PATCH		
		if(GetIsRight() == true)
		{
			pos -= (50.f * GetDirVector());
		}
		else
		{
			pos += (50.f * GetDirVector());
		}
#endif

		PlaySequenceByTriggerCount( pSeqGroundShockWaveBlue, pos, 10,20, 2 );
		if( pSeqImpactTickBlue != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200,200, 20 );

			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO8 ) == true )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_FLAME_GEYSER_MEMO", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), 
				true, pos, GetRotateDegree(), GetDirVector() );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_FLAME_GEYSER", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), 
				true, pos, GetRotateDegree(), GetDirVector() );

#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
		m_pFlameGager->ChangeAnim( L"FlameGager" );
		m_pFlameGager->Play( CKTDGXSkinAnim::XAP_ONE );

		m_pFlameGagerLand->ChangeAnim( L"FlameGagerLand" );
		m_pFlameGagerLand->Play( CKTDGXSkinAnim::XAP_ONE );

		m_pFlameGager->GetMatrix().Move( pos );
		m_pFlameGagerLand->GetMatrix().Move( pos );

		m_pFlameGager->GetMatrix().RotateDegree( GetRotateDegree() );
		m_pFlameGagerLand->GetMatrix().RotateDegree( GetRotateDegree() );
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

		if ( GetShowSmallGageAndName() )
		{
			D3DXVECTOR2 projPos = g_pKTDXApp->GetProj3DPos( pos );
			CKTDGParticleSystem::CParticleEventSequence* pSeqKuAAang = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_STR_KUAANG ) );
			PlaySequenceByTriggerCount( pSeqKuAAang, D3DXVECTOR3( projPos.x, projPos.y, 0.0f ), 100,100, 1 );
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					if( GetIsRight() == true )
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					else
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_EventProcess()
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



//ESSI_SI2_ES_FLAME_GEYSER_HYPER
void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_HYPER_Start()
{
	CommonStateStart();
#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	m_DamageData.attackerType	= CX2DamageManager::AT_MESH;
	m_DamageData.bCanRevenge	= false;
	m_DamageData.pAttackerAnim	= m_pFlameGager;
	m_DamageData.attackType = CX2DamageManager::AT_SPECIAL;

	m_pFlameGager->GetRenderParam()->color = 0xffff6666;
	m_pFlameGager->GetMatrix().Scale( 1.7f,2.0f,1.7f );

//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

}

//void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_HYPER_FrameMoveFuture()
//{
//	//if( m_FrameDataFuture.unitCondition.fStateTime >= 0.85f && m_FrameDataFuture.unitCondition.fStateTime < 1.1f )
//	//{
//	//	//m_FrameDataFuture.syncData.bFrameStop = true;
//	//}
//
//	CommonFrameMoveFuture();
//}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.004f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7666f ) == true && EventCheck( 0.7666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		UpDownCrashCamera( 50.0f, 0.3f );


		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveRed = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_RED ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickRed = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_TICK_RED ) );


		D3DXVECTOR3 pos = GetWeaponBonePos( 0,  L"TRACE_START0", 0 );

#ifdef SKILL_BALANCE_PATCH
		if(GetIsRight() == true)
		{
			pos -= (50.f * GetDirVector());
		}
		else
		{
			pos += (50.f * GetDirVector());
		}
#endif
		PlaySequenceByTriggerCount( pSeqGroundShockWaveRed, pos, 10,10, 2 );
		if( pSeqImpactTickRed != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickRed, pos, 200,200, 20 );

			pSeqImpactTickRed->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO8 ) == true )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_FLAME_GEYSER_MEMO", this, NULL, true, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), 
				true, pos, GetRotateDegree(), GetDirVector() );
		else
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_FLAME_GEYSER", this, NULL, true, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), 
				true, pos, GetRotateDegree(), GetDirVector() );

#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
		m_pFlameGager->ChangeAnim( L"FlameGager" );
		m_pFlameGager->Play( CKTDGXSkinAnim::XAP_ONE );

		m_pFlameGagerLand->ChangeAnim( L"FlameGagerLand" );
		m_pFlameGagerLand->Play( CKTDGXSkinAnim::XAP_ONE );

		m_pFlameGager->GetMatrix().Move( pos );
		m_pFlameGagerLand->GetMatrix().Move( pos );

		m_pFlameGager->GetMatrix().RotateDegree( GetRotateDegree() );
		m_pFlameGagerLand->GetMatrix().RotateDegree( GetRotateDegree() );
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

		if ( GetShowSmallGageAndName() )
		{
			D3DXVECTOR2 projPos = g_pKTDXApp->GetProj3DPos( pos );
			CKTDGParticleSystem::CParticleEventSequence* pSeqKuAAang = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_STR_KUAANG ) );
			PlaySequenceByTriggerCount( pSeqKuAAang, D3DXVECTOR3( projPos.x, projPos.y, 0.0f ), 100,100, 1 );
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					if( GetIsRight() == true )
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					else
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}					
		}
		break;
	}

}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FLAME_GEYSER_HYPER_EventProcess()
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





//ESSI_SI3_ES_UNLIMITED_BLADE
void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_Init()
{


#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
	
	XSkinMeshReadyInBackground( L"Effect_ElSword_MagnumBlade.X" );

#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	CKTDXDeviceXSkinMesh* pMagnumBladeMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Effect_ElSword_MagnumBlade.X" );
	DeviceReadyInBackground( pMagnumBladeMesh );

	m_pMagnumBlade			= CKTDGXSkinAnim::CreateSkinAnim();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMagnumBlade );

	m_pMagnumBlade->SetAnimXSkinMesh( pMagnumBladeMesh );
	m_pMagnumBlade->AddModelXSkinMesh( pMagnumBladeMesh );

	m_pMagnumBlade->SetAlphaObject( true );
	m_pMagnumBlade->SetLayer( XL_EFFECT_0 );

	m_pMagnumBlade->GetRenderParam()->bAlphaBlend	= true;
	//m_pMagnumBlade->GetRenderParam()->bZEnable		= false;
	m_pMagnumBlade->GetRenderParam()->bZWriteEnable	= false;
	m_pMagnumBlade->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMagnumBlade->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMagnumBlade->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.08f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.08f ) == true && EventCheck( 0.08f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.5f;
	}

	//1타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.166f ) == true && EventCheck( 0.166f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//2타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.433f ) == true && EventCheck( 0.433f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//3타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.666f ) == true && EventCheck( 0.666f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//4타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.966f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.966f ) == true && EventCheck( 0.966f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//5타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 1.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 1.3f ) == true && EventCheck( 1.3f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//6타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 1.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 1.566f ) == true && EventCheck( 1.566f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 2.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 2.0f ) == true && EventCheck( 2.0f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.25f;
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_FrameMove()
{

	ChangeWorldColorByHyperMode();
	
	ShowActiveSkillCutInAndLight( 0.004f, 2 );

	ShowActiveSkillCutInAndLight( 1.9f, 2, true );
	
	//1타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.166f ) == true && EventCheck( 0.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
		m_DamageData.hitUnitList.resize(0);
	}
	//2타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.433f ) == true && EventCheck( 0.433f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
		m_DamageData.hitUnitList.resize(0);
	}
	//3타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.666f ) == true && EventCheck( 0.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
		m_DamageData.hitUnitList.resize(0);
	}
	//4타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.966f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.966f ) == true && EventCheck( 0.966f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();

		m_DamageData.reActType = CX2DamageManager::RT_BIG_DAMAGE;
		m_DamageData.hitUnitList.resize(0);

	}
	//5타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.3f ) == true && EventCheck( 1.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();

		m_DamageData.hitUnitList.resize(0);
	}
	//6타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.566f ) == true && EventCheck( 1.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
		m_DamageData.hitUnitList.resize(0);

	}


	//최종타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.95f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.95f ) == true && EventCheck( 1.95f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();


		m_DamageData.hitType			= CX2DamageManager::HT_SWORD_SLASH;
		m_DamageData.reActType			= CX2DamageManager::RT_DOWN;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 데미지 배율 수정 6.41 -> 10.68
		m_DamageData.damage.fPhysic		= 10.68f * GetPowerRate();
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한

		m_DamageData.damage.fPhysic		= 6.41f * GetPowerRate();

#else //UPGRADE_SKILL_SYSTEM_2013

#ifdef BALANCE_PATCH_20120329
		m_DamageData.damage.fPhysic		= 11.2f * GetPowerRate();
#else
		m_DamageData.damage.fPhysic		= 7.4f * GetPowerRate();
#endif

#endif //UPGRADE_SKILL_SYSTEM_2013
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		m_DamageData.backSpeed.x		= GetDashSpeed() * 2.0f;
		m_DamageData.fCameraCrashGap	= 40.0f;
		m_DamageData.fCameraCrashTime	= 0.6f;
		m_DamageData.clearScreen		= 2;

		m_DamageData.hitUnitList.resize(0);
	}



#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.1f ) == true && EventCheck( 2.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_MAGNUM_BLADE", this );
	}



#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
	m_pMagnumBlade->GetMatrix().Move( GetPos() );
	m_pMagnumBlade->GetMatrix().RotateDegree( GetRotateDegree() );


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.2f ) == true && EventCheck( 2.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_pMagnumBlade->ChangeAnim( L"SpecialSword", false );
		m_pMagnumBlade->Play( CKTDGXSkinAnim::XAP_ONE );
	}
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.2f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 2.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					if( GetIsRight() == true )
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					else
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA3" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.2f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 2.1f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	}

}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_EventProcess()
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



//ESSI_SI3_ES_UNLIMITED_BLADE_HYPER
void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_Init()
{


#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

	XSkinMeshReadyInBackground( L"Effect_ElSword_MagnumBladeFire.X" );


#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
	CKTDXDeviceXSkinMesh* pMagnumBladeFireMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Effect_ElSword_MagnumBladeFire.X" );
	DeviceReadyInBackground( pMagnumBladeFireMesh );

	

	m_pMagnumBladeFire			= CKTDGXSkinAnim::CreateSkinAnim();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMagnumBladeFire );

	m_pMagnumBladeFire->SetAnimXSkinMesh( pMagnumBladeFireMesh );
	m_pMagnumBladeFire->AddModelXSkinMesh( pMagnumBladeFireMesh );

	m_pMagnumBladeFire->SetAlphaObject( true );
	m_pMagnumBladeFire->SetLayer( XL_EFFECT_0 );

	m_pMagnumBladeFire->GetRenderParam()->bAlphaBlend	= true;
	//m_pMagnumBladeFire->GetRenderParam()->bZEnable		= false;
	m_pMagnumBladeFire->GetRenderParam()->bZWriteEnable	= false;
	m_pMagnumBladeFire->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMagnumBladeFire->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMagnumBladeFire->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.08f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.08f ) == true && EventCheck( 0.08f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.5f;
	}

	//1타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.166f ) == true && EventCheck( 0.166f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//2타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.433f ) == true && EventCheck( 0.433f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//3타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.666f ) == true && EventCheck( 0.666f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//4타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.966f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.966f ) == true && EventCheck( 0.966f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//5타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 1.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 1.3f ) == true && EventCheck( 1.3f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	//6타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 1.5f ) == true && EventCheck( 1.5f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.004f, 2 );

	ShowActiveSkillCutInAndLight( 1.56f, 2, true );
	
	//1타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.166f ) == true && EventCheck( 0.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();

		m_DamageData.hitUnitList.resize(0);
	}
	//2타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.433f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.433f ) == true && EventCheck( 0.433f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();

		m_DamageData.hitUnitList.resize(0);
	}
	//3타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.666f ) == true && EventCheck( 0.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();

		m_DamageData.hitUnitList.resize(0);
	}
	//4타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.966f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.966f ) == true && EventCheck( 0.966f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();


		m_DamageData.reActType = CX2DamageManager::RT_BIG_DAMAGE;
		m_DamageData.hitUnitList.resize(0);

	}
	//5타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.3f ) == true && EventCheck( 1.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();

		m_DamageData.hitUnitList.resize(0);

	}
	//6타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();

		m_DamageData.hitUnitList.resize(0);

	}


#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	m_pMagnumBladeFire->GetMatrix().Move( GetPos() );
	m_pMagnumBladeFire->GetMatrix().RotateDegree( GetRotateDegree() );

	if( m_pMagnumBladeFire->GetState() == CKTDGXSkinAnim::XAS_PLAYING
		&& m_pXSkinAnim->GetNowAnimationTime() > 1.6f )
	{
		g_pData->GetDamageManager()->DamageCheck( &m_DamageData );
	}
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST


	//최종타
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.73f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.73f ) == true && EventCheck( 1.73f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_MAGNUM_BLADE_HYPER", this );

#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
		m_DamageData.SimpleInit();
		m_DamageData.damageType			= CX2DamageManager::DT_MAGIC;
		m_DamageData.optrAttackerGameUnit		= this;
		m_DamageData.attackerType		= CX2DamageManager::AT_MESH;	
		m_DamageData.pAttackerAnim		= m_pMagnumBladeFire;
		m_DamageData.hitType			= CX2DamageManager::HT_SWORD_SLASH;
		m_DamageData.reActType			= CX2DamageManager::RT_FLY;
		m_DamageData.damage.fMagic		= 2.f * GetPowerRate();
		m_DamageData.backSpeed.y		= GetJumpSpeed();
		m_DamageData.fCameraCrashGap	= 20.0f;
		m_DamageData.fCameraCrashTime	= 0.5f;
		m_DamageData.clearScreen		= 1;
		m_DamageData.bReAttack			= true;
		m_DamageData.fHitGap			= 0.1f;
		m_DamageData.bCanRevenge		= false;
		m_DamageData.bForceFly			= true;

		m_DamageData.hitUnitList.resize(0);

		m_pMagnumBladeFire->ChangeAnim( L"SpecialEffect3", false );
		m_pMagnumBladeFire->Play( CKTDGXSkinAnim::XAP_ONE );

//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

		UpDownCrashCamera( 20.0f, 1.0f );


		D3DXVECTOR3 pos = m_FrameDataNow.syncData.position;
		pos.y += 10.0f;


		CKTDGParticleSystem::CParticleEventSequence* pSeqMagnumBladeFireWave = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_MAGNUM_BLADE_FIRE_WAVE ) );
		PlaySequenceByTriggerCount( pSeqMagnumBladeFireWave, pos, 10,10, 4 );

		if ( GetShowSmallGageAndName() )
		{
			D3DXVECTOR2 projPos = g_pKTDXApp->GetProj3DPos( GetPos() );
			CKTDGParticleSystem::CParticleEventSequence* pSeqKuAAang = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_STR_KUAANG ) );
			PlaySequenceByTriggerCount( pSeqKuAAang, D3DXVECTOR3( projPos.x, projPos.y, 0.0f ), 100,100, 1 );
		}
	}



	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.2f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.8f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					if( GetIsRight() == true )
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					else
						g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 2.5f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA3" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA4" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.2f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.8f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 2.5f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA2" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA3" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	}

}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_UNLIMITED_BLADE_HYPER_EventProcess()
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



//ESSI_SI1_ES_AIR_SLASH
void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_Init()
{
	XSkinMeshReadyInBackground( L"AirSlash01.X" );
	XSkinMeshReadyInBackground( L"AirSlash02.X" );
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.2f ) == true && EventCheck( 0.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AirSlash01", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshInstAirSlash1 = pMeshInst->GetHandle();
		}
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshInstAirSlash1 );
	if( NULL != pMeshInst )
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f;
		pMeshInst->SetPos( pos );
		pMeshInst->SetRotateDegree( GetRotateDegree() );
		pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );
	}
	else if( m_hMeshInstAirSlash1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		m_hMeshInstAirSlash1 = INVALID_MESH_INSTANCE_HANDLE;
	}


	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_CameraMove()
{
	CommonCameraMove();


	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.7f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	}
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.37f )
	{
		StateChange( ESSI_SA_AIR_SLASH_SECOND );
	}
	CommonEventProcess();
}

//ESSI_SI1_ES_AIR_SLASH_NEXT
void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_NEXT_FrameMove()
{
	ChangeWorldColorByHyperMode();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.36f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true && EventCheck( 0.36f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 125.0f;
		
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AirSlash02", pos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_NEXT_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.8f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}


//ESSI_SI1_ES_AIR_SLASH_HYPER
void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.2f ) == true && EventCheck( 0.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AirSlash01_Hyper", 
			GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		if( NULL != pMeshInst )
		{
			m_hMeshInstAirSlash1 = pMeshInst->GetHandle();
		}
	}


	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshInstAirSlash1 );
	if( NULL != pMeshInst )
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f;
		pMeshInst->SetPos( pos );
		pMeshInst->SetRotateDegree( GetRotateDegree() );
		pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );
	}
	else if( m_hMeshInstAirSlash1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		m_hMeshInstAirSlash1 = INVALID_MESH_INSTANCE_HANDLE;
	}


	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_HYPER_CameraMove()
{
	CommonCameraMove();


	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.7f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	}
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.37f )
	{
		StateChange( ESSI_SA_AIR_SLASH_SECOND_HYPER );
	}
	CommonEventProcess();
}




//ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT
void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT_FrameMove()
{
	ChangeWorldColorByHyperMode();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.36f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.36f ) == true && EventCheck( 0.36f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 125.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"AirSlash02_Hyper", pos, GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_AIR_SLASH_HYPER_NEXT_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.8f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}





//ESSI_SI1_ES_ASSAULT_SLASH
void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_Init()
{
	XMeshReadyInBackground( L"Ring_Plane.y" );
	XMeshReadyInBackground( L"AssaultSlash02.Y" );
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_Start()
{
	CommonStateStart();

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO2 ) == true )
	{
		m_PhysicParam.nowSpeed.x = 1200.f;
		m_DamageData.fHitGap = 0.05f;
	}
#endif
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.37f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.37f ) == true && EventCheck( 0.37f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 tip;
		GetFramePos( &tip, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		m_hSeqAssualtSlashRing = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"AssualtSlashRingBlue", tip );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashRing );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		m_hSeqAssualtSlashDust = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"AssualtSlashDust", GetPos() );
		pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashDust );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}
	if( m_hSeqAssualtSlashRing != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashRing );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 tip;
			GetFramePos( &tip, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
			pSeq->SetPosition( tip );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}
	if( m_hSeqAssualtSlashDust != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashDust );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetPos() );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.4f )
	{
		m_DamageData.reActType = CX2DamageManager::RT_DOWN;
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.4f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.3f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	}
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.1f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}

//ESSI_SI1_ES_ASSAULT_SLASH_HYPER
void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.37f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.37f ) == true && EventCheck( 0.37f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 tip;
		GetFramePos( &tip, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		m_hSeqAssualtSlashRing = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"AssualtSlashRingRed", tip );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashRing );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}

		m_hSeqAssualtSlashDust = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"AssualtSlashDust", GetPos() );
		pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashDust );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}
	if( m_hSeqAssualtSlashRing != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashRing );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 tip;
			GetFramePos( &tip, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
			pSeq->SetPosition( tip );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}
	if( m_hSeqAssualtSlashDust != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashDust );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( GetPos() );
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}

	
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.45f )
	{
		m_DamageData.reActType = CX2DamageManager::RT_DOWN;
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.4f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.3f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
		}
		break;
	}
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_ASSAULT_SLASH_HYPER_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.1f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}


//{{ dmlee 2009.6.2 obsolete!! 사용하지 않는 필살기
////ESSI_SI2_ES_LUNA_BLADE
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_Init()
//{
//	XSkinMeshReadyInBackground( L"LunaBlade01.X" );
//	XSkinMeshReadyInBackground( L"LunaBlade02.X" );
//	XSkinMeshReadyInBackground( L"LunaBlade03.X" );
//}
//
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_FrameMoveFuture()
//{
//	if( m_pXSkinAnimFuture->EventTimer( 0.48f ) == true && EventCheck( 0.48f, true ) == true )
//	{
//		//m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.fRunSpeed * 1.5f;
//	}
//	CommonFrameMoveFuture();
//}
//
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_FrameMove()
//{
//	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
//
//	if( m_pXSkinAnim->EventTimer( 0.11f ) == true && EventCheck( 0.11f, false ) == true )
//	{
//		ActiveSkillShow( L"Dummy1_Rhand", s_SkillCutInSet[s_iSkillCutInSetIndex].cutIn[1].fileName.c_str(), s_SkillCutInSet[s_iSkillCutInSetIndex].cutIn[1].vSize, s_SkillCutInSet[s_iSkillCutInSetIndex].cutIn[1].vPosition, false );
//	}
//	if( m_pXSkinAnim->EventTimer( 0.433f ) == true && EventCheck( 0.433f, false ) == true )
//	{
//		g_pX2Game->GetDamageEffect()->CreateInstance( (CKTDGObject*) this,  this, L"LUNA_BLADE", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
//	}
//	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
//	{
//		g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"LunaBlade02", GetPos(), GetRotateDegree(), GetRotateDegree() );
//	}
//	if( m_pXSkinAnim->EventTimer( 0.533f ) == true && EventCheck( 0.533f, false ) == true )
//	{
//		g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"LunaBlade03", GetPos(), GetRotateDegree(), GetRotateDegree() );
//	}
//	CommonFrameMove();
//}
//
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_CameraMove()
//{
//	CommonCameraMove();
//}
//
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_EventProcess()
//{
//	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
//	{
//		if( false == IsOnSomethingFuture() )
//			StateChange( ESSI_JUMP_DOWN );
//		else
//			StateChange( USI_WAIT );
//	}
//	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
//	{
//		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.5f )
//		{
//			StateChange( USI_WAIT );
//		}
//	}
//	CommonEventProcess();
//}
//
//
////ESSI_SI2_ES_LUNA_BLADE_HYPER
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_HYPER_FrameMoveFuture()
//{
//	if( m_pXSkinAnimFuture->EventTimer( 0.48f ) == true && EventCheck( 0.48f, true ) == true )
//	{
//		//m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.fRunSpeed * 1.5f;
//	}
//	CommonFrameMoveFuture();
//}
//
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_HYPER_FrameMove()
//{
//	g_pX2Game->GetWorld()->SetWorldColor( 0xffff0000 );
//
//	if( m_pXSkinAnim->EventTimer( 0.06f ) == true && EventCheck( 0.06f, false ) == true )
//	{
//		ActiveSkillShow( L"Dummy1_Rhand", s_SkillCutInSet[s_iSkillCutInSetIndex].cutIn[1].fileName.c_str(), s_SkillCutInSet[s_iSkillCutInSetIndex].cutIn[1].vSize, s_SkillCutInSet[s_iSkillCutInSetIndex].cutIn[1].vPosition, true );
//	}
//	if( m_pXSkinAnim->EventTimer( 0.433f ) == true && EventCheck( 0.433f, false ) == true )
//	{
//		g_pX2Game->GetDamageEffect()->CreateInstance( (CKTDGObject*) this,  this, L"LUNA_BLADE_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
//	}
//	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
//	{
//		g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"LunaBlade02Hyper", GetPos(), GetRotateDegree(), GetRotateDegree() );
//	}
//	if( m_pXSkinAnim->EventTimer( 0.533f ) == true && EventCheck( 0.533f, false ) == true )
//	{
//		g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"LunaBlade03Hyper", GetPos(), GetRotateDegree(), GetRotateDegree() );
//	}
//	CommonFrameMove();
//}
//
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_HYPER_CameraMove()
//{
//	CommonCameraMove();
//}
//
//void CX2GUElsword_SwordMan::ESSI_SI2_ES_LUNA_BLADE_HYPER_EventProcess()
//{
//	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
//	{
//		if( false == IsOnSomethingFuture() )
//			StateChange( ESSI_JUMP_DOWN );
//		else
//			StateChange( USI_WAIT );
//	}
//	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
//	{
//		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.5f )
//		{
//			StateChange( USI_WAIT );
//		}
//	}
//	CommonEventProcess();
//}
//

//ESSI_SI2_ES_FATAL_FURY
//}} dmlee 2009.6.2 obsolete!! 사용하지 않는 필살기
void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_Init()
{
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_FrameMoveFuture()
{
	//if( m_pXSkinAnimFuture->EventTimer( 0.48f ) == true && EventCheck( 0.48f, true ) == true )
	//{
	//	//m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.fRunSpeed * 1.5f;
	//}
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.1f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO1 ) == true )
		{
			m_DamageData.damage.fPhysic *= 1.2f;
			m_DamageData.m_bNoDetonation = true;
		}
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"FatalFury02", GetPos() );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}
		
		UpDownCrashCameraNoReset( 20.0f, 0.3f );

		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"FireDamageImpactSlashRed", GetPos(), -1, -1, -1, 1 );
		
		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
		PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z,  5, 10,  1 );
		

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.95f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.95f ) == true && EventCheck( 0.95f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		D3DXVECTOR3 down;
		GetFramePos( &down, m_vecpWeapon[0]->m_pFrame_TRACE_END[0] );
		D3DXVECTOR3 dirTip	= down - up;
		D3DXVec3Normalize( &dirTip, &dirTip );
		D3DXVECTOR3 pos = up + dirTip * 70.0f;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"FatalFury02", pos );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}
		
		UpDownCrashCameraNoReset( 20.0f, 0.3f );

		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"FireDamageImpactSlashRed", pos, -1, -1, -1, 1 );

		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
		PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z,  5, 10,  2 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.54f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.54f ) == true && EventCheck( 1.54f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.2f ) == true && EventCheck( 2.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		D3DXVECTOR3 down;
		GetFramePos( &down, m_vecpWeapon[0]->m_pFrame_TRACE_END[0] );
		D3DXVECTOR3 dirTip	= down - up;
		D3DXVec3Normalize( &dirTip, &dirTip );
		D3DXVECTOR3 pos = up + dirTip * 20.0f;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"FatalFury02", pos );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}
		
		UpDownCrashCameraNoReset( 20.0f, 0.3f );
		
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"FireDamageImpactSlashRed", pos, -1, -1, -1, 1 );

		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
		PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z,  5, 10,  3 );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.3f ) == true && EventCheck( 2.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
		m_DamageData.reActType = CX2DamageManager::RT_DOWN;
		m_DamageData.backSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.5f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 3.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 3.0f ) == true && EventCheck( 3.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"FatalFury01", GetPos(), GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMeshInstFatalFury = pMeshInst->GetHandle();
		}
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshInstFatalFury );
	if( NULL != pMeshInst )
	{
		pMeshInst->SetPos( GetPos() );
		pMeshInst->SetRotateDegree( GetRotateDegree() );
		pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );
	}
	else if( m_hMeshInstAirSlash1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		m_hMeshInstAirSlash1 = INVALID_MESH_INSTANCE_HANDLE;
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_CameraMove()
{
	CommonCameraMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_EventProcess()
{
#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO1 ) == true )
	{
		if( GetAttackSuccess(false) == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.6f )
		{
			m_bFirstAttackSuccess = true;
		}

		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f && m_bFirstAttackSuccess == false )
		{
			const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( CX2SkillTree::SI_SA_ES_FATAL_FURY );
			if ( NULL != pSkillSlotData )
			{
				float fMPConsume = pSkillSlotData->m_fMPConsumption / 2.f;
				UpNowMp( fMPConsume );

				if( NULL != g_pData->GetPicCharBlue() )
				{
					WCHAR wszText[64] = L"";
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", (int)fMPConsume );

					D3DXVECTOR3 pos = GetPos();
					pos.y += 50.f;

					g_pData->GetPicCharBlue()->DrawText( wszText, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
				}
			}
			StateChange( USI_WAIT );
		}
	}
#endif

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 4.0f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}


//ESSI_SI2_ES_FATAL_FURY_HYPER
void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.48f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.48f ) == true && EventCheck( 0.48f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//m_PhysicParam.nowSpeed.x = m_OrgPhysicParam.fRunSpeed * 1.5f;
	}
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.1f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO1 ) == true )
		{
			m_DamageData.damage.fPhysic *= 1.2f;
			m_DamageData.m_bNoDetonation = true;
		}
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"FatalFury02", GetPos() );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}
		
		UpDownCrashCameraNoReset( 20.0f, 0.3f );

		//g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"DropDownRed", GetPos(), GetRotateDegree(), GetRotateDegree(), XL_EFFECT_0 );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"FireDamageImpactSlashRed", GetPos(), -1, -1, -1, 1 );

		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
		PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z,  5, 10,  1 );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.95f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.95f ) == true && EventCheck( 0.95f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		D3DXVECTOR3 down;
		GetFramePos( &down, m_vecpWeapon[0]->m_pFrame_TRACE_END[0] );
		D3DXVECTOR3 dirTip	= down - up;
		D3DXVec3Normalize( &dirTip, &dirTip );
		D3DXVECTOR3 pos = up + dirTip * 70.0f;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"FatalFury02", pos );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}
		
		UpDownCrashCameraNoReset( 20.0f, 0.3f );

		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"FireDamageImpactSlashRed", pos, -1, -1, -1, 1 );

		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
		PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z,  5, 10,  2 );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.54f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.54f ) == true && EventCheck( 1.54f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.2f ) == true && EventCheck( 2.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		D3DXVECTOR3 down;
		GetFramePos( &down, m_vecpWeapon[0]->m_pFrame_TRACE_END[0] );
		D3DXVECTOR3 dirTip	= down - up;
		D3DXVec3Normalize( &dirTip, &dirTip );
		D3DXVECTOR3 pos = up + dirTip * 20.0f;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"FatalFury02", pos );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}
		
		UpDownCrashCameraNoReset( 20.0f, 0.3f );

		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"FireDamageImpactSlashRed", pos, -1, -1, -1, 1 );
	

		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWave = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleCommonMinorParticleByEnum( COMMON_MINOR_PII_GROUND_SHOCK_WAVE ) );
		PlaySequenceByTriggerCount( pSeqGroundShockWave, m_FrameDataNow.unitCondition.landPosition.x, m_FrameDataNow.unitCondition.landPosition.y + 5.0f, m_FrameDataNow.unitCondition.landPosition.z,  5, 10,  3 );

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.3f ) == true && EventCheck( 2.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
		m_DamageData.reActType = CX2DamageManager::RT_DOWN;
		m_DamageData.backSpeed.x = m_OrgPhysicParam.GetDashSpeed() * 1.5f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 3.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 3.0f ) == true && EventCheck( 3.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"FatalFury01", GetPos(), GetRotateDegree(), GetRotateDegree() );
		if( NULL != pMeshInst )
		{
			m_hMeshInstFatalFury = pMeshInst->GetHandle();
		}
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMeshInstFatalFury );
	if( NULL != pMeshInst )
	{
		pMeshInst->SetPos( GetPos() );
		pMeshInst->SetRotateDegree( GetRotateDegree() );
		pMeshInst->SetMoveAxisAngleDegree( GetRotateDegree() );
	}
	else if( m_hMeshInstAirSlash1 != INVALID_MESH_INSTANCE_HANDLE )
	{
		m_hMeshInstAirSlash1 = INVALID_MESH_INSTANCE_HANDLE;
	}


	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_FATAL_FURY_HYPER_EventProcess()
{
#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO1 ) == true )
	{
		if( GetAttackSuccess(false) == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.6f )
		{
			m_bFirstAttackSuccess = true;
		}

		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f && m_bFirstAttackSuccess == false )
		{
			const CX2UserSkillTree::SkillSlotData* pSkillSlotData = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillSlot( CX2SkillTree::SI_SA_ES_FATAL_FURY );
			if ( NULL != pSkillSlotData )
			{
				float fMPConsume = pSkillSlotData->m_fMPConsumption / 2.f;
				UpNowMp( fMPConsume );

				if( NULL != g_pData->GetPicCharBlue() )
				{
					WCHAR wszText[64] = L"";
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", (int)fMPConsume );

					D3DXVECTOR3 pos = GetPos();
					pos.y += 50.f;

					g_pData->GetPicCharBlue()->DrawText( wszText, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
				}
			}
			StateChange( USI_WAIT );
		}
	}
#endif

	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 4.0f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}


//ESSI_SI2_ES_SPIRAL_BLAST
void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_Init()
{
	XSkinMeshReadyInBackground( L"SpiralBlast01.X" );
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		D3DXVECTOR3 vPos = GetBonePos ( L"Bip01");
		if( true == GetIsRight() )
		{
			vPos += ( 300.f * GetDirVector() );
		}
		else
		{
			vPos -= ( 300.f * GetDirVector() );
		}
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SPIRAL_BLAST", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SPIRAL_BLAST", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.93f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.93f ) == true && EventCheck( 0.93f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.07f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.07f ) == true && EventCheck( 1.07f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"SpiralBlast01", up );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.03f ) == true && EventCheck( 1.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"SpiralBlast02", up );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
		}
	}
	
	/*
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1f ) == true && EventCheck( 1.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->DestroyInstance( m_pSpiralBlastSwordFriction );
		m_pSpiralBlastSwordFriction = NULL;

		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		if( GetIsRight() == true )
			m_pSpiralBlastSwordFriction = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"SwordFrictionRight", up );
		else
			m_pSpiralBlastSwordFriction = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"SwordFrictionLeft", up );		
		m_pSpiralBlastSwordFriction->SetAxisAngle( GetRotateDegree() );

		
	}
	if( m_pSpiralBlastSwordFriction != NULL && g_pX2Game->GetMinorParticle()->IsLiveInstance(m_pSpiralBlastSwordFriction) )
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		m_pSpiralBlastSwordFriction->SetPosition( up );
		m_pSpiralBlastSwordFriction->SetAxisAngle( GetRotateDegree() );
	}
	if( m_pXSkinAnim->GetNowAnimationTime() > 1.5f && m_pSpiralBlastSwordFriction != NULL )
	{
		//m_pSpiralBlastSwordFriction->SetAutoDie();
		//m_pSpiralBlastSwordFriction = NULL;
	}
	*/

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_CameraMove()
{
	CommonCameraMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 2.4f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}


//ESSI_SI2_ES_SPIRAL_BLAST_HYPER
void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		D3DXVECTOR3 vPos = GetBonePos ( L"Bip01");
		if( true == GetIsRight() )
		{
			vPos += ( 300.f * GetDirVector() );
		}
		else
		{
			vPos -= ( 300.f * GetDirVector() );
		}
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SPIRAL_BLAST", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SPIRAL_BLAST", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.93f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.93f ) == true && EventCheck( 0.93f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.07f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.07f ) == true && EventCheck( 1.07f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.33f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.33f ) == true && EventCheck( 1.33f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"SpiralBlast01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		CKTDGParticleSystem::CParticleEventSequence* m_pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"SpiralBlast01", up );
		if( m_pSeq != NULL )
		{
			m_pSeq->SetAxisAngle( GetRotateDegree() );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.03f ) == true && EventCheck( 1.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 up;
		GetFramePos( &up, m_vecpWeapon[0]->m_pFrame_TRACE_START[0] );
		CKTDGParticleSystem::CParticleEventSequence* m_pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"SpiralBlast02", up );
		if( m_pSeq != NULL )
		{
			m_pSeq->SetAxisAngle( GetRotateDegree() );
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_SPIRAL_BLAST_HYPER_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 2.4f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}


//ESSI_SI3_ES_TRIPLE_GEYSER
void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_Init()
{
	XSkinMeshReadyInBackground( L"Effect_ElSword_FlameGager.X" );
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.004f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7666f ) == true && EventCheck( 0.7666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetWeaponBonePos( 0,  L"TRACE_START0", 0 );

#ifdef SKILL_BALANCE_PATCH
		if(GetIsRight() == true)
		{
			pos -= (50.f * GetDirVector());
		}
		else
		{
			pos += (50.f * GetDirVector());
		}

		pos.y = GetPos().y;
#endif

		UpDownCrashCamera( 50.0f, 0.3f );

		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE ) );


		PlaySequenceByTriggerCount( pSeqGroundShockWaveBlue, pos, 10,20, 2 );
		if( pSeqImpactTickBlue != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200,200, 10 );

			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRIPLE_GEYSER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		if ( GetShowSmallGageAndName() )
		{
			D3DXVECTOR2 projPos = g_pKTDXApp->GetProj3DPos( pos );
			CKTDGParticleSystem::CParticleEventSequence* pSeqKuAAang = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_STR_KUAANG ) );
			PlaySequenceByTriggerCount( pSeqKuAAang, D3DXVECTOR3( projPos.x, projPos.y, 0.0f ), 100,100, 1 );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9666f ) == true && EventCheck( 0.9666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetWeaponBonePos( 0,  L"TRACE_START0", 0 );
		if( GetIsRight() == true )
			pos += GetDirVector() * 300.0f;
		else
			pos -= GetDirVector() * 300.0f;

#ifdef SKILL_BALANCE_PATCH
		if(GetIsRight() == true)
		{
			pos -= (50.f * GetDirVector());
		}
		else
		{
			pos += (50.f * GetDirVector());
		}

		pos.y = GetPos().y;
#endif

		UpDownCrashCamera( 50.0f, 0.3f );


		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE ) );



		PlaySequenceByTriggerCount( pSeqGroundShockWaveBlue, pos, 10,20, 2 );
		if( pSeqImpactTickBlue != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200,200, 10 );

			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRIPLE_GEYSER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.1666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1666f ) == true && EventCheck( 1.1666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetWeaponBonePos( 0,  L"TRACE_START0", 0 );
		if( GetIsRight() == true )
			pos += GetDirVector() * 600.0f;
		else
			pos -= GetDirVector() * 600.0f;

#ifdef SKILL_BALANCE_PATCH
		if(GetIsRight() == true)
		{
			pos -= (50.f * GetDirVector());
		}
		else
		{
			pos += (50.f * GetDirVector());
		}

		pos.y = GetPos().y;
#endif

		UpDownCrashCamera( 50.0f, 0.3f );


		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE ) );


		PlaySequenceByTriggerCount( pSeqGroundShockWaveBlue, pos, 10,20, 2 );
		if( pSeqImpactTickBlue != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200,200, 10 );

			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRIPLE_GEYSER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_EventProcess()
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


//ESSI_SI3_ES_TRIPLE_GEYSER_HYPER
void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.004f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7666f ) == true && EventCheck( 0.7666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();

		UpDownCrashCamera( 50.0f, 0.3f );


		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE ) );


		PlaySequenceByTriggerCount( pSeqGroundShockWaveBlue, pos, 10,20, 2 );
		if( pSeqImpactTickBlue != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200,200, 10 );

			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRIPLE_GEYSER_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		
		if ( GetShowSmallGageAndName() )
		{
			D3DXVECTOR2 projPos = g_pKTDXApp->GetProj3DPos( pos );
			CKTDGParticleSystem::CParticleEventSequence* pSeqKuAAang = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_STR_KUAANG ) );
			PlaySequenceByTriggerCount( pSeqKuAAang, D3DXVECTOR3( projPos.x, projPos.y, 0.0f ), 100,100, 1 );
		}		
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9666f ) == true && EventCheck( 0.9666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos += GetDirVector() * 300.0f;
		else
			pos -= GetDirVector() * 300.0f;

		UpDownCrashCamera( 50.0f, 0.3f );


		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE ) );


		PlaySequenceByTriggerCount( pSeqGroundShockWaveBlue, pos, 10,20, 2 );
		
		if( pSeqImpactTickBlue != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200,200, 10 );
			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRIPLE_GEYSER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.1666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1666f ) == true && EventCheck( 1.1666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos += GetDirVector() * 600.0f;
		else
			pos -= GetDirVector() * 600.0f;

		UpDownCrashCamera( 50.0f, 0.3f );


		CKTDGParticleSystem::CParticleEventSequence* pSeqGroundShockWaveBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_GROUND_SHOCK_WAVE_BLUE ) );
		CKTDGParticleSystem::CParticleEventSequence* pSeqImpactTickBlue = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_IMPACT_BLUE ) );




		PlaySequenceByTriggerCount( pSeqGroundShockWaveBlue, pos, 10,20, 2 );
		if( pSeqImpactTickBlue != NULL )
		{
			PlaySequenceByTriggerCount( pSeqImpactTickBlue, pos, 200,200, 10 );

			pSeqImpactTickBlue->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"TRIPLE_GEYSER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_TRIPLE_GEYSER_HYPER_EventProcess()
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

//ESSI_SI3_ES_ARMAGEDDON_BLADE
void CX2GUElsword_SwordMan::ESSI_SI3_ES_ARMAGEDDON_BLADE_Init()
{
	XSkinMeshReadyInBackground( L"ArmageddonBlade.x" );
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_ARMAGEDDON_BLADE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Knight_ArmageddonBlade01", pos );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Knight_ArmageddonBlade02", pos );
	}	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.68f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.68f ) == true && EventCheck( 1.68f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ESK_ARMAGEDON_BLADE );
		if ( NULL != pSkillTemplet )
		{
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO18 ) == true )
				SetBuffFactorToGameUnit( pSkillTemplet, 1 );
			else
#endif //ADD_MEMO_1ST_CLASS
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.7f ) == true && EventCheck( 1.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
		D3DXVECTOR3 vDir = GetDirVector();
		if( true == GetIsRight() )
			pos += 305.f * vDir + D3DXVECTOR3( 0.f, 70.f, 0.f );
		else
			pos += -305.f * vDir + D3DXVECTOR3( 0.f, 70.f, 0.f );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_WindBlade01", pos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_ARMAGEDDON_BLADE_EventProcess()
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

//ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER
void CX2GUElsword_SwordMan::ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.06f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Knight_ArmageddonBlade01", pos );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Knight_ArmageddonBlade02", pos );
	}	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.68f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.68f ) == true && EventCheck( 1.68f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ESK_ARMAGEDON_BLADE );
		if ( NULL != pSkillTemplet )
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO18 ) == true )
				SetBuffFactorToGameUnit( pSkillTemplet, 3 );
			else
				SetBuffFactorToGameUnit( pSkillTemplet, 2 );
#else //ADD_MEMO_1ST_CLASS
			SetBuffFactorToGameUnit( pSkillTemplet, 1 );
#endif //ADD_MEMO_1ST_CLASS
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.8f ) == true && EventCheck( 1.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy1_Rhand" );
		D3DXVECTOR3 vDir = GetDirVector();
		if( true == GetIsRight() )
			pos += 305.f * vDir + D3DXVECTOR3( 0.f, 70.f, 0.f );
		else
			pos += -305.f * vDir + D3DXVECTOR3( 0.f, 70.f, 0.f );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_WindBlade01", pos );
		if( NULL != pSeq )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}


	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_ARMAGEDDON_BLADE_HYPER_EventProcess()
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


//ESSI_SI1_ES_WIND_BLADE
void CX2GUElsword_SwordMan::ESSI_SI1_ES_WIND_BLADE_Init()
{
	XSkinMeshReadyInBackground( L"WindBlade01.x" );
	XSkinMeshReadyInBackground( L"WindBlade_MagicSquare01.x" );
	XSkinMeshReadyInBackground( L"WindBlade_MagicSquare02.x" );
	XSkinMeshReadyInBackground( L"WindBlade_MagicSquare03.x" );
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_WIND_BLADE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.067f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.08f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.08f ) == true && EventCheck( 0.08f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_WindBlade01_Mesh", GetPos(), GetRotateDegree(), GetRotateDegree() );
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_WindBlade02_Mesh", GetPos(), GetRotateDegree(), GetRotateDegree() );
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_WindBlade03_Mesh", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO10 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"WIND_BLADE_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );		
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"WIND_BLADE", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 vDir = GetDirVector();
		if( true == GetIsRight() )
			pos += 200.f * vDir + D3DXVECTOR3( 0.f, 140.f, 0.f );
		else
			pos += -200.f * vDir + D3DXVECTOR3( 0.f, 140.f, 0.f );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_WindBlade01", pos );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_WIND_BLADE_EventProcess()
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


//ESSI_SI3_ES_WIND_BLADE_HYPER
void CX2GUElsword_SwordMan::ESSI_SI1_ES_WIND_BLADE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.067f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.08f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.08f ) == true && EventCheck( 0.08f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_WindBlade01_Mesh", GetPos(), GetRotateDegree(), GetRotateDegree() );
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_WindBlade02_Mesh", GetPos(), GetRotateDegree(), GetRotateDegree() );
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_WindBlade03_Mesh", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO10 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"WIND_BLADE_MEMO_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"WIND_BLADE_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 vDir = GetDirVector();
		if( true == GetIsRight() )
			pos += 200.f * vDir + D3DXVECTOR3( 0.f, 140.f, 0.f );
		else
			pos += -200.f * vDir + D3DXVECTOR3( 0.f, 140.f, 0.f );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_WindBlade01", pos );
		if( pSeq != NULL )
		{
			pSeq->SetAxisAngle( GetRotateDegree() );
			pSeq->SetAddRotate( GetRotateDegree() );
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI1_ES_WIND_BLADE_HYPER_EventProcess()
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


//ESSI_SI2_ES_RISING_SLASH
void CX2GUElsword_SwordMan::ESSI_SI2_ES_RISING_SLASH_Init()
{
	XSkinMeshReadyInBackground( L"RisingSlash_Blade01.x" );
	XSkinMeshReadyInBackground( L"RisingSlash_MagicSquare01.x" );	
	XMeshReadyInBackground( L"Arme_ChargeMp2.Y" );

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO4 ) == true )
	{
		XSkinMeshReadyInBackground( L"RisingSlash_Blade01_Memo.x" );
		XSkinMeshReadyInBackground( L"RisingSlash_MagicSquare01_Memo.x" );
	}	
#endif
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_RISING_SLASH_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.1f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.55f ) == true && EventCheck( 0.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"LizardMan_Older_PowerBooster01", GetPos() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 vec = GetDirVector();
		pos = GetPos() + vec * 170.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		pos = GetPos() - vec * 170.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		pos = GetPos() + vec * 420.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		pos = GetPos() - vec * 420.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO4 ) == true )
		{
			pos = GetPos() + vec * 670.f;
			pos.y = m_FrameDataNow.unitCondition.landPosition.y;
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
			pos = GetPos() - vec * 670.f;
			pos.y = m_FrameDataNow.unitCondition.landPosition.y;
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		}
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.12f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.12f ) == true && EventCheck( 0.12f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 2.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_RisingSlash01_Mesh", pos, GetRotateDegree(), GetRotateDegree() );
		CX2DamageEffect::CEffect *pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RISING_BLADE", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#ifdef SERV_SKILL_NOTE
		if( pEffect != NULL && GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO4 ) == true )
		{
			pEffect->GetDamageData().fHitGap = 0.03f;
		}
#endif
	}
#ifdef SERV_SKILL_NOTE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.15f ) &&
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.15f ) == true && EventCheck( 0.15f, false ) == true && 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO4 ) == true )
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 vec = GetDirVector();
		float oy = pos.y;
		float fScalePos = 670.f * GetScaleByUnit().x;

		pos = GetPos() + vec * fScalePos;
		pos.y = oy + 2.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_RisingSlash01_Mesh_Memo", pos, GetRotateDegree(), GetRotateDegree() );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RISING_BLADE_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		pos = GetPos() - vec * fScalePos;
		pos.y = oy + 2.0f;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_RisingSlash01_Mesh_Memo", pos, GetRotateDegree(), GetRotateDegree() );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RISING_BLADE_MEMO", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#endif

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_RISING_SLASH_EventProcess()
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

//ESSI_SI2_ES_RISING_SLASH_HYPER
void CX2GUElsword_SwordMan::ESSI_SI2_ES_RISING_SLASH_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.1f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.55f ) == true && EventCheck( 0.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"LizardMan_Older_PowerBooster01", GetPos() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 vec = GetDirVector();
		pos = GetPos() + vec * 170.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		pos = GetPos() - vec * 170.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		pos = GetPos() + vec * 420.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		pos = GetPos() - vec * 420.0f;
		pos.y = m_FrameDataNow.unitCondition.landPosition.y;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );

#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO4 ) == true )
		{
			pos = GetPos() + vec * 670.f;
			pos.y = m_FrameDataNow.unitCondition.landPosition.y;
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
			pos = GetPos() - vec * 670.f;
			pos.y = m_FrameDataNow.unitCondition.landPosition.y;
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_MagicKnight_RisingSlash01", pos );
		}
#endif
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.12f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.12f ) == true && EventCheck( 0.12f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 2.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_RisingSlash01_Mesh", pos, GetRotateDegree(), GetRotateDegree() );
		CX2DamageEffect::CEffect *pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RISING_BLADE_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

#ifdef SERV_SKILL_NOTE
		if( pEffect != NULL && GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO4 ) == true )
		{
			pEffect->GetDamageData().fHitGap = 0.03f;
		}
#endif
	}
#ifdef SERV_SKILL_NOTE
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.15f ) &&
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.15f ) == true && EventCheck( 0.15f, false ) == true && 
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO4 ) == true )
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 vec = GetDirVector();
		float oy = pos.y;

		pos = GetPos() + vec * 670.0f;
		pos.y = oy + 2.0f;
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_RisingSlash01_Mesh_Memo", pos, GetRotateDegree(), GetRotateDegree() );

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RISING_BLADE_MEMO_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		pos = GetPos() - vec * 670.0f;
		pos.y = oy + 2.0f;
		pMeshInstance  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Elsword_MagicKnight_RisingSlash01_Mesh_Memo", pos, GetRotateDegree(), GetRotateDegree() );

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"RISING_BLADE_MEMO_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#endif

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI2_ES_RISING_SLASH_HYPER_EventProcess()
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


//ESSI_SI3_ES_STORM_BLADE
void CX2GUElsword_SwordMan::ESSI_SI3_ES_STORM_BLADE_Init()
{
	XSkinMeshReadyInBackground( L"StormBlade_Blade01.x" );
	XSkinMeshReadyInBackground( L"StormBlade_MagicSquare00.x" );
	XSkinMeshReadyInBackground( L"StormBlade_SmallBlade01.x" );
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_STORM_BLADE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.2f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20120329
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI3_ES_STROM_BLADE", this, NULL, false, GetPowerRate() );
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"STORM_BLADE_CENTER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"STORM_BLADE", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 65.0f;
		pos.y -= 70.0f;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_MagicKnight_StormBlade01", pos );
#endif
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_STORM_BLADE_EventProcess()
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

//ESSI_SI3_ES_STORM_BLADE_HYPER
void CX2GUElsword_SwordMan::ESSI_SI3_ES_STORM_BLADE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.2f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20120329
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SI3_ES_STROM_BLADE_HYPER", this, NULL, false, GetPowerRate() );
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"STORM_BLADE_CENTER_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"STORM_BLADE_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 65.0f;
		pos.y -= 70.0f;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_MagicKnight_StormBlade01", pos );
#endif
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI3_ES_STORM_BLADE_HYPER_EventProcess()
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

/** @function : COMMON_BUFF_FrameMove
	@brief : 공통적으로 쓰는 버프 스킬의 FrameMove
*/
/*virtual*/ void CX2GUElsword_SwordMan::COMMON_BUFF_FrameMove()
{

#ifndef UPGRADE_SKILL_SYSTEM_2013 // 공통 스킬 개편, 김종훈
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( 0.004f, 0 );
#endif // UPGRADE_SKILL_SYSTEM_2013 // 공통 스킬 개편, 김종훈
	
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
#ifndef SPECIAL_USE_ITEM
void CX2GUElsword_SwordMan::THROW_WOODEN_PIECE_FrameMove()
{
	if ( IsMyUnit() )
		g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
	//g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rotate;


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		pos = GetBonePos( L"Dummy1_Rhand" );

		rotate = GetRotateDegree();

		if ( m_FrameDataNow.syncData.nowSubState == CX2Item::SST_THROW_WOODEN_PIECE )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"APPLE", GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else if ( m_FrameDataNow.syncData.nowSubState == CX2Item::SST_ICE_CRYSTAL )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ICE_CRYSTAL", GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else if ( m_FrameDataNow.syncData.nowSubState == CX2Item::SST_FIRE_CRYSTAL )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIRE_CRYSTAL", GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
		}
		else if ( m_FrameDataNow.syncData.nowSubState == CX2Item::SST_MAGIC_BULLET )
		{
			CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"WALLY_8TH_GUIDED_MISSILE", GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
			//pEffect->AttackResult 

			int uid = g_pX2Game->GetNearestNPCUnitUID( GetPos(), -1 );
			if ( pEffect != NULL && uid != 0 )
				pEffect->SetLockOnNPCUID( uid );
		}
		else if ( m_FrameDataNow.syncData.nowSubState == CX2Item::SST_POISON_POCKET )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"POISON_POCKET", GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
		}
	}
	/*
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		
		pos = GetBonePos( L"Dummy1_Rhand" );
		
		rotate = GetRotateDegree();
		CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( (CKTDGObject*) this,  this, L"THROW_WEAPON", GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );

		

	
		
		CX2Eqip* pEquipWeapon = NULL;
		for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
		{
			CX2Eqip* pCX2Eqip = m_ViewEqipList[i];

			if ( pCX2Eqip != NULL && pCX2Eqip->GetItem() != NULL && pCX2Eqip->GetItem()->GetItemTemplet() != NULL )
			{
				if( pCX2Eqip->GetItem()->GetItemTemplet()->GetEqipPosition() == CX2Unit::EP_WEAPON_HAND )
				{
					pEquipWeapon = pCX2Eqip;
					break;
				}


			}
		}

		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pEquipWeapon->GetItemID() );
		if ( pEquipWeapon != NULL && pItemTemplet != NULL )
		{
			pEffect->GetMainEffect()->ChangeXSkinMesh( pEquipWeapon->GetModelName(), pEquipWeapon->GetChangeTexName(),
				NULL, pEquipWeapon->GetAniXETName() );

			pEquipWeapon->SetShow( false );
		}

		
		SetStopTime( 1.5f );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CX2Eqip* pEquipWeapon = NULL;
		for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
		{
			CX2Eqip* pCX2Eqip = m_ViewEqipList[i];

			if ( pCX2Eqip != NULL && pCX2Eqip->GetItem() != NULL && pCX2Eqip->GetItem()->GetItemTemplet() != NULL )
			{
				if( pCX2Eqip->GetItem()->GetItemTemplet()->GetEqipPosition() == CX2Unit::EP_WEAPON_HAND )
				{
					pEquipWeapon = pCX2Eqip;
					break;
				}


			}
		}
		if ( pEquipWeapon != NULL )
			pEquipWeapon->SetShow( true );

	}
	*/
	
	CommonFrameMove();
}
#endif SPECIAL_USE_ITEM

void CX2GUElsword_SwordMan::ESSI_LEAP_StartFuture()
{
	m_bLeaping = true;
	m_fElapsedTimeToLeap = 0.0f;

	int npcUID = g_pX2Game->GetNearestNPCUnitUID( GetPos(), -1 );
	if ( npcUID == 0 )
		StateChange( m_CommonState.m_Wait );

	CX2GUNPC* pGUNPC = g_pX2Game->GetNPCUnitByUID( npcUID );
	if ( pGUNPC == NULL )
		StateChange( m_CommonState.m_Wait );

	m_fTimeToLeap = 1.0f;

	m_StartPosToLeap = GetPos();

	D3DXVECTOR3 objectPos = pGUNPC->GetPos();

	D3DXVECTOR3 tempPos = pGUNPC->GetPos() - GetPos();
	//캐릭터 위치가 원점위치라고 보고 엔피씨를 바라볼까?

	D3DXVec3Normalize( &m_DirVecToLeap, &tempPos );

	float fHypotenuseLength = D3DXVec3Length( &tempPos );

	tempPos.y = 0;
	float fBaseLineLength = D3DXVec3Length( &tempPos );

	float radian = acosf( fBaseLineLength / fHypotenuseLength );
	float degree = D3DXToDegree( radian );
	degree = abs( degree );
	//원점에서 바라본 엔피씨의 각도인가?

	//실제 날아갈 각도?
	int degreeToLeap = 0;
	if ( degree >= 45 )
	{
		degreeToLeap = 45 - (int)degree;
	}
	else
	{
		degreeToLeap = 45;
	}
	 
	if ( degreeToLeap < 10 )
		degreeToLeap = 10;


	//float fSpeedToLeap = sqrt( m_fHypotenuseLength * LEAP_GRAVITY / sin( D3DXToRadian(degreeToLeap*2) ) );

	//속력은 얼마로? 
	//float fSpeedToLeap = (m_fTimeToLeap / 2.0f) * LEAP_GRAVITY / sin( D3DXToRadian(degreeToLeap) ); 

	/*
	float fSpeedToLeap = ( (2.0f * LEAP_GRAVITY * m_fTimeToLeap/2.0f) + 
		sqrt( pow( (2.0f*LEAP_GRAVITY*m_fTimeToLeap/2.0f), 2 ) - ( 4.0f * pow( sin( D3DXToRadian(degreeToLeap) ), 2 ) * pow( LEAP_GRAVITY, 2 ) * pow( m_fTimeToLeap/2.0f, 2 ) ) ) ) /
		( 2 * pow( sin( D3DXToRadian(degreeToLeap) ), 2 ) );
*/
	//임시로 높이 100 조절..
	

	
	//m_fVerticalSpeedToLeap = sqrt( 2.0f * LEAP_GRAVITY * 100 );
	//m_fVerticalSpeedToLeap = LEAP_GRAVITY * pow( m_fTimeToLeap / 2.0f, 2 );
	

	m_fParallelSpeedToLeap =  fBaseLineLength / m_fTimeToLeap;

	//최대가 45도이고 최소가 10도인데.. 으흠.. 어떡하면 좋을까낭?
	m_fHeightToLeap = fHypotenuseLength/3.0f;
	m_fHeightToLeap *= degreeToLeap/45.0f;
	//m_fVerticalSpeedToLeap = sin( D3DXToRadian(degreeToLeap) ) * fSpeedToLeap;
	m_fVerticalSpeedToLeap = 2.0f * m_fHeightToLeap / pow( m_fTimeToLeap/2.0f, 2 ) * (m_fTimeToLeap/2.0f); // g = ( 2.0f * 100.0f / pow( m_fTimeToLeap/2.0f, 2 ) );
	
	

	m_DegreeToLeap = (int)degree;

}

void CX2GUElsword_SwordMan::ESSI_LEAP_FrameMoveFuture()
{
	CommonFrameMoveFuture();

	m_fElapsedTimeToLeap += m_fElapsedTime;
	
	//m_FrameDataFuture.syncData.position.y = m_StartPosToLeap.y + ( ( m_fVerticalSpeedToLeap * m_fElapsedTimeToLeap ) - ( 0.5f * /*LEAP_GRAVITY*/ ( 2.0f * m_fHeightToLeap / pow( m_fTimeToLeap/2.0f, 2 ) ) * pow( m_fElapsedTimeToLeap, 2 ) )  );

	//m_FrameDataFuture.syncData.position.x = ( ( m_fVerticalSpeedToLeap * m_UpVectorToLeap.x * m_fElapsedTimeToLeap ) - ( 0.5f * /*LEAP_GRAVITY*/ ( 2.0f * m_fHeightToLeap / pow( m_fTimeToLeap/2.0f, 2 ) ) * pow( m_fElapsedTimeToLeap, 2 ) )  );
	//m_FrameDataFuture.syncData.position.z = ( ( m_fVerticalSpeedToLeap * m_UpVectorToLeap.z * m_fElapsedTimeToLeap ) - ( 0.5f * /*LEAP_GRAVITY*/ ( 2.0f * m_fHeightToLeap / pow( m_fTimeToLeap/2.0f, 2 ) ) * pow( m_fElapsedTimeToLeap, 2 ) )  );
	
	m_FrameDataFuture.syncData.position = m_StartPosToLeap;


	//평행으로 달리는 부분 구해볼까?
	D3DXVECTOR3 tempPos;
	tempPos = m_DirVecToLeap;
	tempPos.y = 0;
	D3DXVec3Normalize( &tempPos, &tempPos );
	tempPos = ( /*tempPos*/m_DirVecToLeap * m_fElapsedTimeToLeap * m_fParallelSpeedToLeap);
	float fHeight = tempPos.y;

	m_FrameDataFuture.syncData.position.x += tempPos.x;
	m_FrameDataFuture.syncData.position.z += tempPos.z;
	m_FrameDataFuture.syncData.position.y += fHeight;

	D3DXVECTOR3 vecHeight = D3DXVECTOR3(0,0,0);
	vecHeight.y = ( ( m_fVerticalSpeedToLeap * m_fElapsedTimeToLeap ) - ( 0.5f * /*LEAP_GRAVITY*/ ( 2.0f * m_fHeightToLeap / pow( m_fTimeToLeap/2.0f, 2 ) ) * pow( m_fElapsedTimeToLeap, 2 ) )  );


	//x축이나 z축으로 얼마나 비틀어져서 날아가는거지?
	D3DXMATRIX matTrans;
	D3DXMatrixIdentity( &matTrans );
	D3DXMatrixRotationY( &matTrans, D3DXToRadian( 90 ) );
	D3DXVECTOR3 tempDirVec;	//바라보는 거시기랑 수직이 되는 방향벡터 과연 이게 90도로 꺾은게 맞을까?
	D3DXVec3TransformNormal( &tempDirVec, &m_DirVecToLeap, &matTrans );
	D3DXVec3Normalize( &tempDirVec, &tempDirVec );

	D3DXMatrixIdentity( &matTrans );
	D3DXMatrixRotationAxis( &matTrans, &tempDirVec, D3DXToRadian( m_DegreeToLeap ) );
	D3DXVECTOR4 transPos;
	D3DXVec3TransformCoord( &vecHeight, &vecHeight, &matTrans );

	m_FrameDataFuture.syncData.position += vecHeight;

	m_FrameDataFuture.unitCondition.dirDegree = m_DirVecToLeap; 
	m_FrameDataFuture.unitCondition.dirVector = m_DirVecToLeap;

	


	
}

void CX2GUElsword_SwordMan::ESSI_LEAP_EventProcess()
{
	/*
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( m_CommonState.m_Wait );
	}
	*/

	if ( m_fElapsedTimeToLeap >= m_fTimeToLeap )
		StateChange( m_CommonState.m_Wait );
	
	//특정 시간 후에 목표 위치로 보간 후 웨이트 모션으로 가야겠지?? 음~~?

	CommonEventProcess();

}
void CX2GUElsword_SwordMan::ESSI_LEAP_End()
{
	m_bLeaping = false;
	m_fElapsedTimeToLeap = 0.0f;
}

void CX2GUElsword_SwordMan::ESSI_SI_A_ES_KICK_Start()
{
	CommonStateStart();


#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO7 ) == true )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013
		m_NowStateData.m_fPowerRate *= 1.1f;
		m_DamageData.damage.fPhysic	*= 1.1f;
		m_DamageData.damage.fMagic	*= 1.1f;
		m_DamageData.m_bNoDetonation = true;
#else // UPGRADE_SKILL_SYSTEM_2013
		m_NowStateData.m_fPowerRate *= 4.0f;
		m_DamageData.damage.fPhysic	*= 4.0f;
		m_DamageData.damage.fMagic	*= 4.0f;
#endif // UPGRADE_SKILL_SYSTEM_2013
	}
#endif
	
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ES_KICK );
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
		    return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_DamageData.fForceDownValue = -pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_DamageData.fForceDownValue = -pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
}


void CX2GUElsword_SwordMan::ESSI_SI_A_ES_KICK_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f )
		{
			//m_fCanNotAttackTime = 0.1f;
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}












void CX2GUElsword_SwordMan::StandUpAttackFrontNoDownEventProcess()
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

void CX2GUElsword_SwordMan::StandUpAttackFrontNoDownEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}




void CX2GUElsword_SwordMan::StandUpAttackBackNoDownEventProcess()
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

void CX2GUElsword_SwordMan::StandUpAttackBackNoDownEnd()
{
	CommonStateEnd();
	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}





void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_Init()
{
	XSkinMeshReadyInBackground( L"Spin_Liner03.X" );
	XSkinMeshReadyInBackground( L"Spin_Liner_Fire02.X" );
}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}


void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hWindMill		= g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_WINDMILL", this, NULL, false, GetPowerRate() ); 
		m_hWindMillLand = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_WINDMILL_LAND", this ); 
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ESSI_SA_ESK_WINDMILL );
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_StateEnd()
{
	if( NULL != m_pSoundWindMill )
	{
		m_pSoundWindMill->Stop();
		m_pSoundWindMill->Set3DPosition( GetPos() );
		m_pSoundWindMill->Play( true );
	}

	m_bDisableGravity = false;
	CommonStateEnd();
}







void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_HYPER_StateStart()
{
	CommonStateStart();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}


void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hWindMill		= g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_WINDMILL", this, NULL, true, GetPowerRate() ); 
		m_hWindMillLand = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_WINDMILL_LAND", this ); 
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ESSI_SA_ESK_WINDMILL );
	}

	CommonEventProcess();
}




void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_START_HYPER_StateEnd()
{

	if( NULL != m_pSoundWindMill )
	{
		m_pSoundWindMill->Stop();
		m_pSoundWindMill->Set3DPosition( GetPos() );
		m_pSoundWindMill->Play( true );
	}

	m_bDisableGravity = false;
	CommonStateEnd();
}




//void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_FrameMoveFuture()
//{
//	if( m_InputData.pureRight == true )
//	{
//		m_FrameDataFuture.syncData.bIsRight = true;
//		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
//	}
//	else if( m_InputData.pureLeft == true )
//	{
//		m_FrameDataFuture.syncData.bIsRight = false;
//		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
//	}
//	else
//	{
//		m_PhysicParam.nowSpeed.x = 0.0f;
//	}

//	CommonFrameMoveFuture();
//}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_FrameMove()
{
	if( NULL != m_pSoundWindMill )
	{
		m_pSoundWindMill->Set3DPosition( GetPos() );
	}
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else	X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif	X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE	
	{
		// 매 프레임 시작마다, 데미지 감소 버프가 걸려있는지 확인하고 걸려있지 않다면 걸어준다.
		if ( false == HaveThisBuff ( BTI_BUFF_SA_ESK_WINDMILL ) )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ELK_WINDMILL );
			if ( NULL != pSkillTemplet )
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_EventProcess()
{
	if( m_FrameDataFuture.unitCondition.fStateTime > m_fWindMillDuration )
	{
		StateChange( ESSI_SA_ESK_WINDMILL_END );
	}
	else
	{
		if( true == IsOnSomethingFuture() )
		{
			if( m_InputData.pureUp == true )
			{
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();
				m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
			}
			else if( m_InputData.oneDown == true 
				&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, 
				&m_FrameDataFuture.unitCondition.landPosition, 
				&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
			{
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
				m_PhysicParam.nowSpeed.y = 0.f;
			}
		}


		if( m_InputData.pureRight == true )
		{
			m_FrameDataFuture.syncData.bIsRight = true;
			m_PhysicParam.nowSpeed.x = GetDashSpeed();
		}
		else if( m_InputData.pureLeft == true )
		{
			m_FrameDataFuture.syncData.bIsRight = false;
			m_PhysicParam.nowSpeed.x = GetDashSpeed();
		}
		else
		{
			m_PhysicParam.nowSpeed.x = 0.0f;
		}

	}


	CommonEventProcess();

}


void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_StateEnd()
{
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMill );
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMillLand );

	if( NULL != m_pSoundWindMill )
	{
		m_pSoundWindMill->Stop();
	}

	CommonStateEnd();
}




void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_HYPER_FrameMove()
{
	if( NULL != m_pSoundWindMill )
	{
		m_pSoundWindMill->Set3DPosition( GetPos() );
	}


	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_HYPER_EventProcess()
{


	if( m_FrameDataFuture.unitCondition.fStateTime > m_fWindMillDuration )
	{
		StateChange( ESSI_SA_ESK_WINDMILL_END_HYPER );
	}
	else
	{
		if( true == IsOnSomethingFuture() )
		{
			if( m_InputData.pureUp == true )
			{
				m_PhysicParam.nowSpeed.y = GetJumpSpeed();
				m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
			}
			else if( m_InputData.oneDown == true 
				&& g_pX2Game->GetWorld()->GetLineMap()->CanDown( m_FrameDataFuture.syncData.position, LINE_RADIUS, 
				&m_FrameDataFuture.unitCondition.landPosition, 
				&m_FrameDataFuture.syncData.lastTouchLineIndex ) == true )
			{
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
				m_PhysicParam.nowSpeed.y = 0.f;
			}
		}


		if( m_InputData.pureRight == true )
		{
			m_FrameDataFuture.syncData.bIsRight = true;
			m_PhysicParam.nowSpeed.x = GetDashSpeed();
		}
		else if( m_InputData.pureLeft == true )
		{
			m_FrameDataFuture.syncData.bIsRight = false;
			m_PhysicParam.nowSpeed.x = GetDashSpeed();
		}
		else
		{
			m_PhysicParam.nowSpeed.x = 0.0f;
		}
	}


	CommonEventProcess();
}


void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_HYPER_StateEnd()
{
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMill );
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hWindMillLand );

	if( NULL != m_pSoundWindMill )
	{
		m_pSoundWindMill->Stop();
	}

	CommonStateEnd();
}





void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_END_FrameMove()
{
	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_END_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.267f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_END_HYPER_FrameMove()
{
	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_SA_ESK_WINDMILL_END_HYPER_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.267f )
		{
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_SI_SA_Sword_Fire_Mesh01.X" );
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.3f )
	{
		m_bDisableGravity = false;
		m_PhysicParam.fDownAccel = m_PhysicParam.fGAccel * 2.f;
	}	

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.02f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.02f ) == true && EventCheck( 0.02f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSwordFire = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_SWORDFIRE_NODAMAGE", this ); 			
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( ESSI_SA_EMK_SWORD_FIRE_LANDING );
	}
	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_EndFuture()
{
	m_bDisableGravity = false;
	m_PhysicParam.fDownAccel = 0.f;		

	CommonStateEndFuture();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_HYPER_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_SI_SA_Sword_Fire_Mesh01.X" );
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_HYPER_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_HYPER_FrameMoveFuture()
{		
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.3f )
	{
		m_bDisableGravity = false;
		m_PhysicParam.fDownAccel = m_PhysicParam.fGAccel * 2.f;
	}

	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.02f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.02f ) == true && EventCheck( 0.02f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSwordFire = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_SWORDFIRE_NODAMAGE", this ); 
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER );
	}
	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_HYPER_EndFuture()
{
	m_bDisableGravity = false;
	m_PhysicParam.fDownAccel = 0.f;		

	CommonStateEndFuture();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_LANDING_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.02f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.02f ) == true && EventCheck( 0.02f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hSwordFire );
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_LANDING_EventProcess()
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

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_LANDING_End()
{				
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EMK_SWORD_FIRE );
	SetBuffFactorToGameUnit( pSkillTemplet, 0 );

	CommonStateEnd();
}


void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_FrameMove()
{		

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.02f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.02f ) == true && EventCheck( 0.02f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_hSwordFire );
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_EventProcess()
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

void CX2GUElsword_SwordMan::ESSI_SA_EMK_SWORD_FIRE_LANDING_HYPER_End()
{
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EMK_SWORD_FIRE );
	SetBuffFactorToGameUnit( pSkillTemplet, 0 );

	CommonStateEnd();
}







void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_Init()
{
	XMeshReadyInBackground( L"Armor_Break_Impact01.Y" );
}

void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_StateStart()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ESK_ARMOR_BREAK );

	if ( NULL != pSkillTemplet )
	{
		/// 메모를 배웠으면
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( NULL == GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

		if ( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO11 ) && pSkillTemplet->m_vecBuffFactorPtr.size() > 1 )
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[1]->GetClonePtr( iSkillTempletLevel ) );
		else
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
#else //UPGRADE_SKILL_SYSTEM_2013
		if ( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO11 ) && pSkillTemplet->m_vecBuffFactorPtr.size() > 1 )
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[1] );
		else
			m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
}


void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.22f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.22f ) == true && EventCheck( 0.22f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elsword_Armor_Break", this );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_EventProcess()
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


void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_StateEnd()
{
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_HYPER_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.22f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.22f ) == true && EventCheck( 0.22f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Elsword_Armor_Break", this );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_HYPER_EventProcess()
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

void CX2GUElsword_SwordMan::ESSI_A_ESK_ARMOR_BREAK_HYPER_StateEnd()
{
	CommonStateEnd();
}



//ESSI_SA_EMK_BIG_BURST
void CX2GUElsword_SwordMan::ESSI_SA_EMK_BIG_BURST_Init()
{
	TextureReadyInBackground( L"smoke02.dds" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"ColorBallFire.dds" );
	TextureReadyInBackground( L"DarkSmoke01.dds" );
}
void CX2GUElsword_SwordMan::ESSI_SA_EMK_BIG_BURST_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.2f ) == true && EventCheck( 0.2f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.65f ) == true && EventCheck( 0.65f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x -= 1600 * 0.8f;
	}
	CommonFrameMoveFuture();
}
void CX2GUElsword_SwordMan::ESSI_SA_EMK_BIG_BURST_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos;
		vPos = GetBonePos( L"Dummy2_Lhand" );
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO12) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMEGA_EFFECT_ELSWORD_MNK_BIG_BURST_MEMO", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), 
				m_FrameDataNow.unitCondition.landPosition.y );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMEGA_EFFECT_ELSWORD_MNK_BIG_BURST", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), 
				m_FrameDataNow.unitCondition.landPosition.y );
		vPos = GetBonePos( L"Dummy2_Lhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Elsword_SI_A_Big_Burst04", vPos );
		vPos = GetBonePos( L"Dummy2_Lhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Elsword_SI_A_Big_Burst05", vPos );
		vPos = GetBonePos( L"Dummy2_Lhand" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Smoke_Elsword_SI_A_Big_Burst06", vPos );
	}

	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_SA_EMK_BIG_BURST_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.06f )
		{
			StateChange( USI_WAIT );
		}
	}
	CommonEventProcess();
}
void CX2GUElsword_SwordMan::ESSI_SA_EMK_BIG_BURST_End()
{
	CommonStateEnd();
}







// 2차 전직
void CX2GUElsword_SwordMan::ESSI_ELK_ZZZfrontZ_FrameMove()
{
	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_ELK_ZZZfrontZ_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.867f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ELK_XXX_FrameMove()
{
	CommonFrameMove();

}




void CX2GUElsword_SwordMan::ESSI_ELK_XXX_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.5f, 0.5f, m_InputData.oneX == true, ESSI_ELK_XXXX )
	SKILL_CANCEL_AFTER( 0.67f )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();

}

void CX2GUElsword_SwordMan::ESSI_ELK_XXXX_FrameMove()
{
	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_ELK_XXXX_EventProcess()
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
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.3f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}



void CX2GUElsword_SwordMan::ESSI_ERS_XZ_StateStartFuture()
{
	CommonStateStartFuture();

	m_bERS_COMBO_XZ_Rune_Created = false;
}

void CX2GUElsword_SwordMan::ESSI_ERS_XZ_FrameMoveFuture()
{

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.3f ) == true && EventCheck( 0.3f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( GetNowMp() > _CONST_ELSWORD_::MAGIC_ERS_XZ_MP_COST )
		{
			m_bERS_COMBO_XZ_Rune_Created = true;
		}
		else
		{
			m_bERS_COMBO_XZ_Rune_Created = false;
		}
	}


	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::ESSI_ERS_XZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( _CONST_ELSWORD_::MAGIC_ERS_XZ_MP_COST ) == true )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			D3DXVECTOR3 vScale( m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent );
			m_hRuneSlayerComboXZ = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ERS_COMBO_XZ", this, NULL, false, -1.f, -1.f, vScale );
#else //UPGRADE_SKILL_SYSTEM_2013
			m_hRuneSlayerComboXZ = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ERS_COMBO_XZ", this );
#endif //UPGRADE_SKILL_SYSTEM_2013
			if ( CX2EffectSet::EffectSetInstance* pInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( m_hRuneSlayerComboXZ ) )
			{
				pInstance->SetPowerRateScale( m_fAttackPowerRateFireBall );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
				pInstance->m_fLifeTime *= m_fRuneProjectileRangeIncPercent;
				pInstance->m_bDamageEffectScale = true;
#endif //UPGRADE_SKILL_SYSTEM_2013
			}


			if( false == IsMyUnit() )
			{
				m_bERS_COMBO_XZ_Rune_Created = true;
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MagicFail", pos, 100, 100, 1, 1 );
			if( pSeq != NULL )
			{
				pSeq->SetAxisAngle( angle );
				pSeq->SetAddRotate( angle );
			}


			if( false == IsMyUnit() )
			{
				m_bERS_COMBO_XZ_Rune_Created = false;
			}
		}
	}

	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_ERS_XZ_EventProcess()
{


	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_( 0, 0.5f, 0.5f, m_InputData.oneZ == true, ESSI_ERS_XZZ )
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.8f )
		{
			StateChange( USI_WAIT );
		}
	}


	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ERS_XZZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( _CONST_ELSWORD_::MAGIC_ERS_XZZ_MP_COST ) == true &&
			true == m_bERS_COMBO_XZ_Rune_Created )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			D3DXVECTOR3 vScale( m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent );
			CX2EffectSet::Handle hComboXZZ = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ERS_COMBO_XZZ", this, NULL, false, -1.f, -1.f, vScale );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2EffectSet::Handle hComboXZZ = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ERS_COMBO_XZZ", this );
#endif //UPGRADE_SKILL_SYSTEM_2013

			if ( CX2EffectSet::EffectSetInstance* pInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hComboXZZ ) )
			{
				pInstance->SetPowerRateScale( m_fAttackPowerRateFireBall );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
				pInstance->m_fLifeTime *= m_fRuneProjectileRangeIncPercent;
				pInstance->m_bDamageEffectScale = true;
#endif //UPGRADE_SKILL_SYSTEM_2013
			}

			m_vecEffectSetToDeleteOnDamageReact.push_back( hComboXZZ );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MagicFail", pos, 100, 100, 1, 1 );
			if( pSeq != NULL )
			{
				pSeq->SetAxisAngle( angle );
				pSeq->SetAddRotate( angle );
			}

		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.38f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.38f ) == true && EventCheck( 0.38f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( INVALID_EFFECTSET_HANDLE != m_hRuneSlayerComboXZ )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hRuneSlayerComboXZ );
		}
	}

	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_ERS_XZZ_EventProcess()
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




void CX2GUElsword_SwordMan::ESSI_ERS_DASH_JUMP_XZ_StateStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.nowSpeed.y = 0.f;
	m_bDisableGravity = true;

	m_bERS_DASH_JUMP_XZ_Rune_Created = false;
}

void CX2GUElsword_SwordMan::ESSI_ERS_DASH_JUMP_XZ_StateEndFuture()
{
	m_bDisableGravity = false;

	CommonStateEndFuture();
}

void CX2GUElsword_SwordMan::ESSI_ERS_DASH_JUMP_XZ_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.26f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.26f ) == true && EventCheck( 0.26f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( GetNowMp() > _CONST_ELSWORD_::MAGIC_ERS_DASH_JUMP_XZ_MP_COST )
		{
			m_bERS_DASH_JUMP_XZ_Rune_Created = true;
		}
		else
		{
			m_bERS_DASH_JUMP_XZ_Rune_Created = false;
		}
	}



	CommonFrameMoveFuture();
}


void CX2GUElsword_SwordMan::ESSI_ERS_DASH_JUMP_XZ_FrameMove()
{

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.26f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.26f ) == true && EventCheck( 0.26f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if(	FlushMp( _CONST_ELSWORD_::MAGIC_ERS_DASH_JUMP_XZ_MP_COST ) == true )
		{
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
			posR.y += -90.f;
			if( true == GetIsRight() )
			{
				posR += GetDirVector() * 160.f;
			}
			else
			{
				posR -= GetDirVector() * 160.f;
			}
			D3DXVECTOR3 angle = GetRotateDegree();

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			D3DXVECTOR3 vSize( m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect*
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pRuneSlayerDashJumpXZ = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_RUNE_SLAYER_DASH_JUMP_XZ", 
				GetPowerRate() * m_fAttackPowerRateFireBall, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fRuneProjectileRangeIncPercent );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hRuneSlayerDashJumpXZ = ( m_pRuneSlayerDashJumpXZ != NULL ) ? m_pRuneSlayerDashJumpXZ->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( NULL != m_pRuneSlayerDashJumpXZ )
			{
				m_pRuneSlayerDashJumpXZ->SetScale( vSize );
			}
			CX2EffectSet::Handle hEffect = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_DASH_JUMP_XZ", this, NULL, false, -1.f, -1.f, vSize );
			if ( CX2EffectSet::EffectSetInstance* pEffectInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffect ) )
			{
				pEffectInstance->m_fLifeTime *= m_fRuneProjectileRangeIncPercent;
				pEffectInstance->m_bDamageEffectScale = true;
			}
#else //UPGRADE_SKILL_SYSTEM_2013
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hRuneSlayerDashJumpXZ = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pRuneSlayerDashJumpXZ = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                this, L"ELSWORD_RUNE_SLAYER_DASH_JUMP_XZ", 
				GetPowerRate() * m_fAttackPowerRateFireBall, posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );

			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_DASH_JUMP_XZ", this );
#endif //UPGRADE_SKILL_SYSTEM_2013

			


			if( false == IsMyUnit() )
			{
				m_bERS_DASH_JUMP_XZ_Rune_Created = true;
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MagicFail", pos, 100, 100, 1, 1 );
			if( pSeq != NULL )
			{
				pSeq->SetAxisAngle( angle );
				pSeq->SetAddRotate( angle );
			}


			if( false == IsMyUnit() )
			{
				m_bERS_DASH_JUMP_XZ_Rune_Created = false;
			}
		}
	}
	//else if( m_pXSkinAnim->IsAnimationEnd() == true )
	//{	
	//	m_bDisableGravity = false;
	//}

	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_ERS_DASH_JUMP_XZ_EventProcess()
{
	//CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( ESSI_ERS_DASH_JUMP_XZZ );
		}
		else /*if( m_FrameDataFuture.unitCondition.fStateTime > 0.56f )*/
		{
			StateChange( USI_JUMP_DOWN );
#ifndef BALANCE_RUNE_SLAYER_20130214
			SetCanNotInputTime_ZXArrow( 0.25f );
#endif //BALANCE_RUNE_SLAYER_20130214
		}
	}
	

	CommonEventProcess();
}





void CX2GUElsword_SwordMan::ESSI_ERS_DASH_JUMP_XZZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ELSWORD_WAY_OF_SWORD
		float fMagicERSDashJumpXZZMpCost = 0.f;
		if( GetWayOfSwordState() == WSS_VIGOR )
		{
			fMagicERSDashJumpXZZMpCost = _CONST_ELSWORD_::VIGOR_ERS_DASH_JUMP_XZZ_MP_COST;
		}
		else
		{
			fMagicERSDashJumpXZZMpCost = _CONST_ELSWORD_::MAGIC_ERS_DASH_JUMP_XZZ_MP_COST;
		}

		if( FlushMp( fMagicERSDashJumpXZZMpCost ) == true && 
			true == m_bERS_DASH_JUMP_XZ_Rune_Created )
#else
		if( FlushMp( _CONST_ELSWORD_::MAGIC_ERS_DASH_JUMP_XZZ_MP_COST ) == true && 
			true == m_bERS_DASH_JUMP_XZ_Rune_Created )
#endif ELSWORD_WAY_OF_SWORD
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_hRuneSlayerDashJumpXZ != INVALID_DAMAGE_EFFECT_HANDLE )
                g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hRuneSlayerDashJumpXZ );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( NULL != m_pRuneSlayerDashJumpXZ )
				g_pX2Game->GetDamageEffect()->DestroyInstance( m_pRuneSlayerDashJumpXZ );
            m_pRuneSlayerDashJumpXZ = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
			posR.y += -90.f;
			if( true == GetIsRight() )
			{
				posR += GetDirVector() * 160.f;
			}
			else
			{
				posR -= GetDirVector() * 160.f;
			}

			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
			CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_RUNE_SLAYER_DASH_JUMP_XZZ", GetPowerRate() * m_fAttackPowerRateFireBall, posR, D3DXVECTOR3(0,0,0), angle, m_FrameDataNow.unitCondition.landPosition.y, false,-1.f, 1.f, m_fRuneProjectileRangeIncPercent );
			if( NULL != pEffect )
			{
				pEffect->SetScale( m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent, m_fRuneProjectileSizeIncPercent );
			}
#else //UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_RUNE_SLAYER_DASH_JUMP_XZZ", GetPowerRate() * m_fAttackPowerRateFireBall, posR, D3DXVECTOR3(0,0,0), angle, m_FrameDataNow.unitCondition.landPosition.y );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MagicFail", pos, 100, 100, 1, 1 );
			if( pSeq != NULL )
			{
				pSeq->SetAxisAngle( angle );
				pSeq->SetAddRotate( angle );
			}
		}
	}

	CommonFrameMove();

}

void CX2GUElsword_SwordMan::ESSI_ERS_DASH_JUMP_XZZ_EventProcess()
{

//#ifdef LINEMAP_FAST_WIND_TEST 
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST



	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}
#ifdef BALANCE_RUNE_SLAYER_20130214
	ELSE_IF_STATE_CHANGE_ON_( 0, m_fAttackCancelAfter, m_fAttackCancelAfter, m_InputData.oneZ == true, ESSI_JUMP_ATTACK_Z )
	ELSE_IF_STATE_CHANGE_ON_( 1, m_fAttackCancelAfter, m_fAttackCancelAfter, m_InputData.oneX == true, ESSI_JUMP_ATTACK_X )
#endif //BALANCE_RUNE_SLAYER_20130214

	CommonEventProcess();

}


void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_Init()
{
}

void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_StateStart()
{
	CommonStateStart();
}


void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.f )
		{
			StateChange( USI_WAIT );
		}
	}
	
	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_StateEnd()
{
	CommonStateEnd();
}







void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_HYPER_StateStart()
{
	CommonStateStart();
}


void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

	CommonFrameMove();
}


void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_HYPER_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SA_ELK_SONIC_BLADE_HYPER_StateEnd()
{
	CommonStateEnd();
}
















void CX2GUElsword_SwordMan::ESSI_SA_ERS_SPLASH_EXPLOSION_Init()
{
}

void CX2GUElsword_SwordMan::ESSI_SA_ERS_SPLASH_EXPLOSION_StateStart()
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	D3DXVECTOR3 vSize( m_fRuneProjectileSizeIncPercent,m_fRuneProjectileSizeIncPercent,m_fRuneProjectileSizeIncPercent);
	CX2EffectSet::Handle hEffectExplosion = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_SPLASH_EXPLOSION", this, NULL,false,-1.f, -1.f, vSize );
	if ( CX2EffectSet::EffectSetInstance* pEffectInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectExplosion ) )
	{
		pEffectInstance->m_fLifeTime *= m_fRuneProjectileRangeIncPercent;
		pEffectInstance->m_bDamageEffectScale = true;
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

	CommonStateStart();
}


void CX2GUElsword_SwordMan::ESSI_SA_ERS_SPLASH_EXPLOSION_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SA_ERS_SPLASH_EXPLOSION_StateEnd()
{
	CommonStateEnd();
}







void CX2GUElsword_SwordMan::ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_StateStart()
{
	CommonStateStart();
}


void CX2GUElsword_SwordMan::ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );
	
	CommonFrameMove();
}




void CX2GUElsword_SwordMan::ESSI_SA_ERS_SPLASH_EXPLOSION_HYPER_StateEnd()
{
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_Init()
{
}

void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_StateStart()
{
	CommonStateStart();
}


void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ESSI_A_ELK_ROLLING_SMASH );
	}

	CommonEventProcess();
}




void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_StateEnd()
{
	CommonStateEnd();
}




void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_StateStart()
{
	CommonStateStart();
}

void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_FrameMove()
{
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( ESSI_A_ELK_ROLLING_SMASH_HYPER );
	}

	CommonEventProcess();
}


void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_READY_HYPER_StateEnd()
{
	CommonStateEnd();
}










void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_StateStartFuture()
{
	CommonStateStartFuture();

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ELK_ROLLING_SMASH );
	ASSERT( NULL != pSkillTemplet );
	if( NULL != pSkillTemplet )
	{	
		m_fRollingSmashDuration = 2.f * m_PhysicParam.nowSpeed.y / m_PhysicParam.fGAccel;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
		return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_PhysicParam.passiveSpeed.x = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS, iSkillTempletLevel ) * 
										100.f / m_fRollingSmashDuration;
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_PhysicParam.passiveSpeed.x = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS) * 100.f / m_fRollingSmashDuration;
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
}

void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_FrameMove()
{
	CommonFrameMove();

}


void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_EventProcess()
{
	CommonEventProcess();

	if( m_FrameDataFuture.unitCondition.fStateTime > m_fRollingSmashDuration &&
		true == IsOnSomethingFuture() )
	{
		StateChange( ESSI_A_ELK_ROLLING_SMASH_LANDING );
	}
}

void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_End()
{
	CommonStateEnd();

}





void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_HYPER_StateStartFuture()
{
	CommonStateStartFuture();

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ELK_ROLLING_SMASH );
	ASSERT( NULL != pSkillTemplet );
	m_fRollingSmashDuration = 2.f * m_PhysicParam.nowSpeed.y / m_PhysicParam.fGAccel;

	if( NULL != pSkillTemplet )
	{			
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
		return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_PhysicParam.passiveSpeed.x = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS, iSkillTempletLevel ) * 
										100.f / m_fRollingSmashDuration;
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_PhysicParam.passiveSpeed.x = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_RANGE_ABS ) * 100.f / m_fRollingSmashDuration;
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

}





void CX2GUElsword_SwordMan::ESSI_A_ELK_ROLLING_SMASH_HYPER_EventProcess()
{
	CommonEventProcess();

	if( m_FrameDataFuture.unitCondition.fStateTime > m_fRollingSmashDuration &&
		true == IsOnSomethingFuture() )
	{
		StateChange( ESSI_A_ELK_ROLLING_SMASH_LANDING_HYPER );
	}
}

















void CX2GUElsword_SwordMan::ESSI_A_ERS_SWORD_ENCHANT_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_SI_A_RS_Sword_Enchant_Mesh01.X" );
	TextureReadyInBackground( L"Condense_Pulse02.dds" );
	TextureReadyInBackground( L"Lire_RingLight01.dds" );
	TextureReadyInBackground( L"Critical_State01.dds" );
}

void CX2GUElsword_SwordMan::ESSI_A_ERS_SWORD_ENCHANT_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.333f ) == true && EventCheck( 0.333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		int iRandBuffIndex = (GetRandomInt() % 3);
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( NULL != GetUnit() && GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO19) == true )
		{
			iRandBuffIndex += 3;
			const int iSkillLevel = max( GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ERS_SWORD_ENCHANT ), 1 );
			SetBuffFactorToGameUnitByBuffFactorID( BFI_BUFF_SWORD_ENCHANT_CHARGE_MP, iSkillLevel );
		}
#endif //ADD_MEMO_1ST_CLASS

		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ERS_SWORD_ENCHANT );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
		SetBuffFactorToGameUnit( pSkillTemplet, iRandBuffIndex );
#else //UPGRADE_SKILL_SYSTEM_2013
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );
#ifdef BALANCE_RUNE_SLAYER_20130214
		//ice, fire, poison 속성 중 랜덤으로 1개 적용
		int iRandBuffIndex = (GetRandomInt() % 3) + 1 ;
		SetBuffFactorToGameUnit( pSkillTemplet, iRandBuffIndex );
#endif //BALANCE_RUNE_SLAYER_20130214
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_A_ERS_SWORD_ENCHANT_EventProcess()
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

#ifdef SKILL_CASH_10_TEST
	void CX2GUElsword_SwordMan::ESSI_A_ESK_PHOENIX_TALON_Init()
	{
		XSkinMeshReadyInBackground( L"Elsword_SI_SA_Phoenix_Talon_Mesh01.X" );
		XSkinMeshReadyInBackground( L"Elsword_SI_SA_Phoenix_Talon_Mesh02.X" );
		TextureReadyInBackground( L"Gate_Light02_Gray.dds" );
	}

	void CX2GUElsword_SwordMan::ESSI_A_ESK_PHOENIX_TALON_StateStart()
	{
		CommonStateStart();

		// to do.

	}

	void CX2GUElsword_SwordMan::ESSI_A_ESK_PHOENIX_TALON_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( 0.002f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.75f ) == true && EventCheck( 1.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EMK_PHOENIX_TALON );
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}

		CommonFrameMove();
	}

	void CX2GUElsword_SwordMan::ESSI_A_ESK_PHOENIX_TALON_Hyper_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( 0.002f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.75f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.75f ) == true && EventCheck( 1.75f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EMK_PHOENIX_TALON );
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}

		CommonFrameMove();
	}

	void CX2GUElsword_SwordMan::ESSI_A_ESK_PHOENIX_TALON_EventProcess()
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

	void CX2GUElsword_SwordMan::ESSI_A_ESK_PHOENIX_TALON_StateEnd()
	{
		CommonStateEnd();
	}

	void CX2GUElsword_SwordMan::ESSI_SA_ESK_DOUBLE_SLASH_FrameMove ()
	{
		ShowActiveSkillCutInAndLight( 0.002f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.004f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.004f ) == true && EventCheck( 0.004f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_DOUBLE_SLASH", this, NULL, false, GetPowerRate() );
		}

		if( m_pXSkinAnim->GetNowAnimationTime() <= 3.667f )
		{
			ChangeWorldColorByHyperMode();

			g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );

		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.3666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 2.3666f ) == true && EventCheck( 2.3666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			//	멤버
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* m_pDoubleSlash = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO17) == true )
				m_pDoubleSlash = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_SLASH_MEMO", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
			else
#endif //ADD_MEMO_1ST_CLASS
			m_pDoubleSlash = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_SLASH", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hDoubleSlash = ( m_pDoubleSlash != NULL ) ? m_pDoubleSlash->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			if( m_pDoubleSlash != NULL )
			{
				if( GetIsRight() == false )
				{
					m_pDoubleSlash->SetScale( D3DXVECTOR3( 1.0f, 1.0f, -1.0f ) );
				}
			}
		}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pDoubleShash = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hDoubleSlash ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pDoubleShash = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pDoubleSlash ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			//	왼쪽 방향이면 방향 틀기
			D3DXVECTOR3 vBone = GetBonePos(L"Bip01");


			//{{ 허상형 : [2009/10/21] //	보는 방향 벡터 추가
			D3DXVECTOR3 vDir = GetDirVector();
			if( GetIsRight() == true )
			{
				//vBone += D3DXVECTOR3( -250.0f, 20.0f, 0.0f );

				vBone.x = vBone.x + (-250.0f * vDir.x);
				vBone.y = vBone.y + 20.0f *	vDir.y;
				vBone.z = vBone.z + 0.0f * vDir.z;
			}
			else
			{
				//vBone += D3DXVECTOR3( 250.0f, 20.0f, 0.0f );

				vBone.x = vBone.x + 250.0f * vDir.x;
				vBone.y = vBone.y + 20.0f *	vDir.y;
				vBone.z = vBone.z + 0.0f * vDir.z;
			}

			pDoubleShash->SetPos(vBone);
		}

		//	스턴 속성 삭제
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.9f ) == true && EventCheck( 1.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_DamageData.m_ExtraDamage.m_ExtraDamageType = CX2DamageManager::EDT_NONE;
		}

		CommonFrameMove();
	}

	void CX2GUElsword_SwordMan::ESSI_SA_ESK_DOUBLE_SLASH_EventProcess ()
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
			if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 3.667f )
			{
				StateChange( USI_WAIT );
			}
		}


		CommonEventProcess();
	}

	void CX2GUElsword_SwordMan::ESSI_SA_ESK_DOUBLE_SLASH_StateEnd()
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hDoubleSlash = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pDoubleSlash = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		CommonStateEnd();
	}

	void CX2GUElsword_SwordMan::ESSI_SA_ESK_DOUBLE_SLASH_HYPER_FrameMove ()
	{
		ShowActiveSkillCutInAndLight( 0.002f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.004f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.004f ) == true && EventCheck( 0.004f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELSWORD_DOUBLE_SLASH_HYPER", this, NULL, false, GetPowerRate() );
		}
		
		if( m_pXSkinAnim->GetNowAnimationTime() <= 3.667f )
		{
			ChangeWorldColorByHyperMode();
			g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.3666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 2.3666f ) == true && EventCheck( 2.3666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			//	멤버
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* m_pDoubleSlash = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO17) == true )
					m_pDoubleSlash = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_SLASH_MEMO", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
				else
#endif //ADD_MEMO_1ST_CLASS
			m_pDoubleSlash = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DOUBLE_SLASH", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree() );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hDoubleSlash = ( m_pDoubleSlash != NULL ) ? m_pDoubleSlash->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( m_pDoubleSlash != NULL )
			{
				if( GetIsRight() == false )
				{
					m_pDoubleSlash->SetScale( D3DXVECTOR3( 1.0f, 1.0f, -1.0f ) );
				}
			}
		}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pDoubleShash = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hDoubleSlash ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pDoubleShash = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pDoubleSlash ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			//	왼쪽 방향이면 방향 틀기
			D3DXVECTOR3 vBone = GetBonePos(L"Bip01");


			//{{ 허상형 : [2009/10/21] //	보는 방향 벡터 추가
			D3DXVECTOR3 vDir = GetDirVector();
			if( GetIsRight() == true )
			{
				//vBone += D3DXVECTOR3( -250.0f, 20.0f, 0.0f );

				vBone.x = vBone.x + (-250.0f * vDir.x);
				vBone.y = vBone.y + 20.0f *	vDir.y;
				vBone.z = vBone.z + 0.0f * vDir.z;
			}
			else
			{
				//vBone += D3DXVECTOR3( 250.0f, 20.0f, 0.0f );

				vBone.x = vBone.x + 250.0f * vDir.x;
				vBone.y = vBone.y + 20.0f *	vDir.y;
				vBone.z = vBone.z + 0.0f * vDir.z;
			}

			pDoubleShash->SetPos(vBone);
		}

		//	스턴 속성 삭제
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.9f ) == true && EventCheck( 1.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_DamageData.m_ExtraDamage.m_ExtraDamageType = CX2DamageManager::EDT_NONE;
		}

		CommonFrameMove();
	}
#endif	//	SKILL_CASH_10_TEST


//{{ kimhc // 2010.11.2 // 로드나이트 - 고통 억제
#ifdef	NEW_SKILL_2010_11

void CX2GUElsword_SwordMan::ReducePain( CX2SkillTree::SKILL_TYPE eSkillType_, const IN CX2UserSkillTree& cUserSkillTree_ )
{
	// 고통억제 스킬이 있는지 판단
	const CX2SkillTree::SkillTemplet* pSkillTempletReducePain = cUserSkillTree_.GetUserSkillTemplet( CX2SkillTree::SI_P_ELK_REDUCE_PAIN );

	if ( NULL == pSkillTempletReducePain )
		return;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if ( NULL == GetUnit() )
		return;
	
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
	const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTempletReducePain->m_eID, true ) );	/// 스킬 레벨
	
	// 사용한 스킬 타입에 따라 고통억제 발동 확률을 다르게 설정
	float fRateReducePain = pSkillTempletReducePain->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
	// 사용한 스킬 타입에 따라 고통억제 발동 확률을 다르게 설정
	float fRateReducePain = pSkillTempletReducePain->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	switch ( eSkillType_ )
	{
	case CX2SkillTree::ST_SPECIAL_ACTIVE:	// 스페셜 액티브인 경우 확률 2배로!
		fRateReducePain *= 2.0f;
		break;

	default:
		break;
	}

	// 고통 억제 발동
	if ( GetRandomFloat() < fRateReducePain )
	{
		SpecialAttackNoStageChange( pSkillTempletReducePain );
	}
}

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.2 // 로드나이트 - 고통 억제




#ifdef NEW_SKILL_2010_11
//{{ oasis907 : 김상윤 [2010.11.5] // 로드 나이트 - 샌드 스톰
void CX2GUElsword_SwordMan::ESSI_SA_ELK_SAND_STORM_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.3f ) == true && EventCheck( 0.3f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.2f;


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.9333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.9333f ) == true && EventCheck( 0.9333f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 0.8f;

	CommonFrameMoveFuture();
}


void CX2GUElsword_SwordMan::ESSI_SA_ELK_SAND_STORM_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 1 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1333f ) == true && EventCheck( 0.1333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 DamageEffectPos = GetPos();

		DamageEffectPos.y += 300.f;

		if(GetIsRight() == true)
		{
			DamageEffectPos += (245.f * GetDirVector() * GetScaleByUnit().x );
		}
		else
		{
			DamageEffectPos -= (245.f * GetDirVector() * GetScaleByUnit().x );
		}

		CX2DamageEffect::CEffect* pDE = NULL;
		pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"LORD_KNIGHT_SAND_STORM", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree() );
		//pDE->SetScale(GetScale());
		if( NULL != pDE )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ELK_SAND_STORM );
			if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			{
				if ( NULL == GetUnit() )
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

void CX2GUElsword_SwordMan::ESSI_SA_ELK_SAND_STORM_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.f )
		{
			StateChange( USI_WAIT );
		}
	}

	CommonEventProcess();
}

//}} oasis907 : 김상윤 [2010.11.5] // 로드 나이트 - 샌드 스톰
#endif NEW_SKILL_2010_11

#ifdef ELSWORD_SHEATH_KNIGHT


void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZ_StateStart()
{
	CommonStateStart();
	m_hSecondBladeSheath = INVALID_MESH_INSTANCE_HANDLE;
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZ_FrameMove()
{


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ElswordSheath02", GetPos(), GetRotateDegree(), GetRotateDegree() );
		if( pMeshInst != NULL )
		{
			m_hSecondBladeSheath = pMeshInst->GetHandle();
			pMeshInst->SetBoundingRadius( 0.0f );
		}


	}
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDashCancelAfter ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDashCancelAfter ) == true && EventCheck( m_fDashCancelAfter, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSecondBladeSheath = INVALID_MESH_INSTANCE_HANDLE;
	}




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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZ_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 0 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
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
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ == true, ESSI_ETK_COMBO_ZZZZ )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZ_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZZ_FrameMove()
{
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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}

	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZZ_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	else if( m_InputData.pureDoubleRight == true || m_InputData.pureDoubleLeft == true )
	{
		StateChangeDashIfPossible();
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneZ == true, ESSI_ETK_COMBO_ZZZZZ )	
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZZ_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZZZ_StateStart()
{
	CommonStateStart();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZZZ_FrameMove()
{
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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZZZ_EventProcess()
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
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_ZZZZZ_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XX_StateStart()
{
	m_iETKComboXXXLoopCount = 0;
	CommonStateStart();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XX_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ElswordSheath02", GetPos(), GetRotateDegree(), GetRotateDegree() );
		if( pMeshInst != NULL )
		{
			m_hSecondBladeSheath = pMeshInst->GetHandle();
			pMeshInst->SetBoundingRadius( 0.0f );
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDashCancelAfter ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDashCancelAfter ) == true && EventCheck( m_fDashCancelAfter, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSecondBladeSheath = INVALID_MESH_INSTANCE_HANDLE;
	}

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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XX_EventProcess()
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
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneDoubleX == true, ESSI_ETK_COMBO_XXX_LOOP )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, m_InputData.oneX == true, ESSI_ETK_COMBO_XXX_END )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XX_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_LOOP_StateStart()
{
	m_iETKComboXXXLoopCount++;
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_LOOP_FrameMove()
{
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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_LOOP_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( ESSI_ETK_COMBO_XXX_END );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX == true && m_iETKComboXXXLoopCount < 3, ESSI_ETK_COMBO_XXX_LOOP )
	ELSE_IF_STATE_CHANGE_ON_EX_( 1, m_fEventTime[1].keyInputStart, m_fEventTime[1].keyInputEnd, m_fEventTime[1].stateChange, m_InputData.oneZ == true, ESSI_ETK_COMBO_XXXZ )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	
	CommonEventProcess();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_LOOP_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = true;
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXXZ_FrameMove()
{
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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXXZ_EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

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
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXXZ_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_END_StateStart()
{
	CommonStateStart();
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
}
void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_END_FrameMove()
{
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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_END_EventProcess()
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

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ETK_COMBO_XXX_END_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_ETK_DASH_COMBO_ZZ_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ElswordSheath02", GetPos(), GetRotateDegree(), GetRotateDegree() );
		if( pMeshInst != NULL )
		{
			m_hSecondBladeSheath = pMeshInst->GetHandle();
			pMeshInst->SetBoundingRadius( 0.0f );
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDashCancelAfter ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDashCancelAfter ) == true && EventCheck( m_fDashCancelAfter, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSecondBladeSheath = INVALID_MESH_INSTANCE_HANDLE;
	}


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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDamageDataChangeTime2 ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDamageDataChangeTime2 ) == true && EventCheck( m_fDamageDataChangeTime2, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		bool bTableOpen = m_LuaManager.BeginTableByReference( m_NowStateData.stateID );

		if ( true == bTableOpen)
		{
			m_DamageData.SimpleInit();
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_LAST" );
			m_LuaManager.EndTable();		
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_ETK_DASH_COMBO_ZZ_EventProcess()
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

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_ETK_DASH_COMBO_ZZ_StateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_SI_A_ES_ROLLING_StateStart()
{
	CommonStateStart();
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ES_ROLLING );
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
		return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_fRollingHpConsumeRel	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::HP_CONSUME_REL, iSkillTempletLevel );
		m_fRollingSpeedRel		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_SPEED_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_fRollingHpConsumeRel	= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::HP_CONSUME_REL );
		m_fRollingSpeedRel		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TELEPORT_SPEED_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
}

void CX2GUElsword_SwordMan::ESSI_SI_A_ES_ROLLING_StateStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.nowSpeed.x = m_fRollingSpeedRel * 1200.f;
}


void CX2GUElsword_SwordMan::ESSI_SI_A_ES_ROLLING_FrameMove()
{

	CommonFrameMove();
}


void CX2GUElsword_SwordMan::ESSI_SI_A_ES_ROLLING_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( ESSI_SI_A_ES_ROLLING_END );
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SI_A_ES_ROLLING_END_StateStart()
{
	CommonStateStart();
	float fUpRate = GetMaxHp() * m_fRollingHpConsumeRel;
	UpHPIfAlive( -fUpRate );
}

void CX2GUElsword_SwordMan::ESSI_SI_A_ES_ROLLING_END_FrameMove()
{

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03333f ) == true && EventCheck( 0.03333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{			
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ES_ROLLING", this );
	}

	CommonFrameMove();
}


void CX2GUElsword_SwordMan::ESSI_SI_A_ES_ROLLING_END_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_HARSH_CHASER_StateStart()
{
	CommonStateStart();
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ETK_HARSH_CHASER );
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
		return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_fHarshChaserProjectiveRange = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillTempletLevel );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO13 ) == true )
			m_fHarshChaserProjectiveRange *= 1.1f;
#endif //ADD_MEMO_1ST_CLASS
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_fHarshChaserProjectiveRange = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_HARSH_CHASER_FrameMove()
{

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDamageEffectPos = GetPos();
		vDamageEffectPos.y += 130.f;
		if(GetIsRight() == true)
		{
			vDamageEffectPos += (30.f * GetDirVector());
		}
		else
		{
			vDamageEffectPos -= (30.f * GetDirVector());
		}
		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* m_pDamageEffectHarshChaserBlade = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADDITIONAL_MEMO
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO13 ) == true )
			m_pDamageEffectHarshChaserBlade = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_HASHCHASER_BLADE_MEMO", GetPowerRate(), vDamageEffectPos, GetRotateDegree(), GetRotateDegree() );
		else
			m_pDamageEffectHarshChaserBlade = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_HASHCHASER_BLADE", GetPowerRate(), vDamageEffectPos, GetRotateDegree(), GetRotateDegree() );
#else
		m_pDamageEffectHarshChaserBlade = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_HASHCHASER_BLADE", GetPowerRate(), vDamageEffectPos, GetRotateDegree(), GetRotateDegree() );
#endif
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hDamageEffectHarshChaserBlade = ( m_pDamageEffectHarshChaserBlade != NULL )
            ? m_pDamageEffectHarshChaserBlade->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hDamageEffectHarshChaserBlade != INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( m_pDamageEffectHarshChaserBlade != NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( ( (m_fSkillCancelAfter - 0.3f) * m_fHarshChaserProjectiveRange ) + 0.3f ) == true  )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( ( (m_fSkillCancelAfter - 0.3f) * m_fHarshChaserProjectiveRange ) + 0.3f ) == true && EventCheck( ( (m_fSkillCancelAfter - 0.3f) * m_fHarshChaserProjectiveRange ) + 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hDamageEffectHarshChaserBlade );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        g_pX2Game->GetDamageEffect()->DestroyInstance( m_pDamageEffectHarshChaserBlade );
            m_pDamageEffectHarshChaserBlade = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
	}

	CommonFrameMove();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_HARSH_CHASER_EventProcess()
{
	if( m_pXSkinAnim->GetNowAnimationTime() > 0.4f && m_pXSkinAnim->GetNowAnimationTime() < ( (m_fSkillCancelAfter - 0.3f) * m_fHarshChaserProjectiveRange ) + 0.3f )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( m_hDamageEffectHarshChaserBlade != INVALID_DAMAGE_EFFECT_HANDLE )
        {
            if ( false == g_pX2Game->GetDamageEffect()->IsLiveInstanceHandle( m_hDamageEffectHarshChaserBlade ) )
            {
                m_hDamageEffectHarshChaserBlade = INVALID_DAMAGE_EFFECT_HANDLE;
				StateChange( ESSI_SI_SA_ETK_HARSH_CHASER_PULL );
            }
        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( m_pDamageEffectHarshChaserBlade != NULL )
		{
			if ( false == g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pDamageEffectHarshChaserBlade ) )
			{
                m_pDamageEffectHarshChaserBlade = NULL;
				StateChange( ESSI_SI_SA_ETK_HARSH_CHASER_PULL );
			}
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_HARSH_CHASER_End()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hDamageEffectHarshChaserBlade != INVALID_DAMAGE_EFFECT_HANDLE )
	    g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hDamageEffectHarshChaserBlade );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	g_pX2Game->GetDamageEffect()->DestroyInstance( m_pDamageEffectHarshChaserBlade );
    m_pDamageEffectHarshChaserBlade = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CommonStateEnd();
}



void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_HARSH_CHASER_PULL_FrameMove()
{
	CommonFrameMove();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_HARSH_CHASER_PULL_EventProcess()
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

	CommonEventProcess();
}



void CX2GUElsword_SwordMan::CompactCounterAttackStart()
{
	CommonStateStart();
	CreateStepDust();

#ifdef COMPACT_COUNTER_FIX
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER, true );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER );

		if( NULL != pSkillTemplet )
		{
			m_DamageData.damage.fPhysic *= pSkillTemplet->GetSkillPowerRateValue( iSkillLevel );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER );

	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			m_DamageData.damage.fPhysic *= pSkillTemplet->m_fPowerRate;
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
#endif // COMPACT_COUNTER_FIX
}

void CX2GUElsword_SwordMan::CompactCounterAttackStartFuture()
{
	CommonStateStartFuture();
	m_bApplyMotionOffset = false;
}



void CX2GUElsword_SwordMan::CompactCounterAttackFrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::CompactCounterAttackFrameMove()
{
#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO5 ) == true && 
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		SetDefenceMemoTime(4.f);
	}	
#endif

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.09f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.09f ) == true && EventCheck( 0.09f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 pos = m_FrameDataNow.syncData.position;
		pos.y += 70.0f;
		if( m_FrameDataNow.syncData.bIsRight == true )
			pos = pos + m_FrameDataNow.unitCondition.dirVector * 100.0f;
		else
			pos = pos + m_FrameDataNow.unitCondition.dirVector * -100.0f;

		CKTDGParticleSystem::CParticleEventSequence* pSeqRevengeWait = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleElswordMinorParticleByEnum( ELSWORD_MINOR_PII_REVENGE_WAIT ) );

		if( pSeqRevengeWait != NULL )
		{
			PlaySequenceByTriggerCount( pSeqRevengeWait, pos, 10,10, 5 );

			pSeqRevengeWait->SetAddRotate( D3DXVECTOR3( 0.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
		}		
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ElswordSheath02", GetPos(), GetRotateDegree(), GetRotateDegree() );
		if( pMeshInst != NULL )
		{
			m_hSecondBladeSheath = pMeshInst->GetHandle();
			pMeshInst->SetBoundingRadius( 0.0f );
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( m_fDashCancelAfter ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( m_fDashCancelAfter ) == true && EventCheck( m_fDashCancelAfter, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_hSecondBladeSheath = INVALID_MESH_INSTANCE_HANDLE;
	}

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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );

#ifdef COMPACT_COUNTER_FIX 
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER, true );

			if( iSkillLevel > 0 ) 
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER );

				if( NULL != pSkillTemplet ) 
				{ 
					m_DamageData.damage.fPhysic *= pSkillTemplet->GetSkillPowerRateValue( iSkillLevel ); 
				} 
			}
#else //UPGRADE_SKILL_SYSTEM_2013
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER );

			if( iSkillLevel > 0 ) 
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ETK_COMPACT_COUNTER, iSkillLevel );

				if( NULL != pSkillTemplet ) 
				{ 
					m_DamageData.damage.fPhysic *= pSkillTemplet->m_fPowerRate; 
				} 
			}
#endif //UPGRADE_SKILL_SYSTEM_2013
#endif // COMPACT_COUNTER_FIX

			m_LuaManager.EndTable();		
		}
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::CompactCounterAttackEventProcess()
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
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::CompactCounterAttackStateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}

//ESSI_REVENGE_ATTACK
void CX2GUElsword_SwordMan::CompactCounterGuardAttackStart()
{
	CommonStateStart();

	m_DamageData.damage.fPhysic += m_fRevengeAddDamage;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ES_COUNTER );
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_ES_COUNTER );

	if( NULL != pSkillTemplet )
	{
		if( GetRandomInt() < pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel ) * 100.f )
		{
			m_DamageData.reActType = CX2DamageManager::RT_SMALL_DAMAGE;
			m_DamageData.backSpeed = D3DXVECTOR2( 0.f, 0.f );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ES_COUNTER_UP );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ES_COUNTER_UP, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			if( GetRandomInt() < pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE ) * 100.f )
			{
				m_DamageData.reActType = CX2DamageManager::RT_SMALL_DAMAGE;
				m_DamageData.backSpeed = D3DXVECTOR2( 0.f, 0.f );
			}
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO5 ) == true )
	{
		SetDefenceMemoTime(4.f);
	}	
#endif

}

void CX2GUElsword_SwordMan::CompactCounterGuardAttackStartFuture()
{
	CommonStateStartFuture();
	m_bApplyMotionOffset = false;
}

void CX2GUElsword_SwordMan::CompactCounterGuardAttackFrameMoveFuture()
{
	if( m_FrameDataFuture.unitCondition.fStateTime >= 0.1f && m_FrameDataFuture.unitCondition.fStateTime < 0.4 )
	{
		//m_FrameDataFuture.syncData.bFrameStop = true;
	}
	CommonFrameMoveFuture();
}

void CX2GUElsword_SwordMan::CompactCounterGuardAttackFrameMove()
{
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
			m_DamageData.attackerType				= CX2DamageManager::AT_UNIT;
			m_DamageData.optrAttackerGameUnit		= this;
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_DamageData.pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SetDamageData( "DAMAGE_DATA_NEXT" );
			m_LuaManager.EndTable();		
		}
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::CompactCounterGuardAttackCameraMove()
{
	CommonCameraMove();

	if( m_FrameDataNow.stateParam.bEventFlagList[0] == false )
	{
		switch( m_FrameDataNow.stateParam.normalCamera )
		{
		case 1:
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			break;

		case 2:
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA1" );
					m_LuaManager.EndTable();
				}
			}
			break;
		}
	}

	m_FrameDataNow.stateParam.bEventFlagList[0] = true;
}

void CX2GUElsword_SwordMan::CompactCounterGuardAttackEventProcess()
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
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::CompactCounterGuardAttackStateEnd()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_MAELSTORM_RAGE_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"elsword_active_maelstromRage_sword.X" );
	TextureReadyInBackground( L"elsword_active_maelstromRage_wave.tga" );
	TextureReadyInBackground( L"elsword_active_maelstromRage_spark.DDS" );
	TextureReadyInBackground( L"elsword_active_maelstromRage_flare.DDS" );
	TextureReadyInBackground( L"elsword_active_maelstromRage_circle.dds" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"elsword_active_maelstromRage_sword.X" );
	TextureReady( L"elsword_active_maelstromRage_wave.tga" );
	TextureReady( L"elsword_active_maelstromRage_spark.DDS" );
	TextureReady( L"elsword_active_maelstromRage_flare.DDS" );
	TextureReady( L"elsword_active_maelstromRage_circle.dds" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_MAELSTORM_RAGE_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_MAELSTORM_RAGE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1333f ) == true && EventCheck( 0.1333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		float fEffectiveTime = 0.f;
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_EIS_MAELSTORM_RAGE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			m_fMaelStormEffectiveTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fMaelStormEffectiveTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ETK_MAELSTORM_RAGE", (CX2GameUnit*) this, NULL, false, GetPowerRate(), m_fMaelStormEffectiveTime );		
	}
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 8.0f, m_fMaelStormEffectiveTime + 3.f );	
	}

	CommonFrameMove();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_MAELSTORM_RAGE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_MAELSTORM_RAGE_EndFuture()
{
	m_bDisableGravity = false;
	m_PhysicParam.fDownAccel = 0.f;		

	CommonStateEndFuture();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_FINAL_STRIKE_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"elsword_active_finalStrike_sheath.X" );
	TextureReadyInBackground( L"elsword_active_finalStrike_space.DDS" );
	TextureReadyInBackground( L"elsword_active_finalStrike_smoke.DDS" );
	TextureReadyInBackground( L"elsword_active_finalStrike_particle.DDS" );
	TextureReadyInBackground( L"elsword_active_finalStrike_core.DDS" );
	TextureReadyInBackground( L"elsword_active_finalStrike_circle.dds" );
	TextureReadyInBackground( L"elsword_active_finalStrike_boomSpark.dds" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"elsword_active_finalStrike_sheath.X" );
	TextureReady( L"elsword_active_finalStrike_space.DDS" );
	TextureReady( L"elsword_active_finalStrike_smoke.DDS" );
	TextureReady( L"elsword_active_finalStrike_particle.DDS" );
	TextureReady( L"elsword_active_finalStrike_core.DDS" );
	TextureReady( L"elsword_active_finalStrike_circle.dds" );
	TextureReady( L"elsword_active_finalStrike_boomSpark.dds" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_FINAL_STRIKE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.633f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.633f ) == true && EventCheck( 0.633f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 DamageEffectPos = GetPos();

		DamageEffectPos.y += 130.f;

		if(GetIsRight() == true)
		{
			DamageEffectPos += (330.f * GetDirVector() * GetScaleByUnit().x );
		}
		else
		{
			DamageEffectPos -= (330.f * GetDirVector() * GetScaleByUnit().x );
		}

		m_vFinalStrikeBlackHolePos = DamageEffectPos;

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_FINALSTRIKE_BLACKHOLE_EFFECT", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.83333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.83333f ) == true && EventCheck( 0.83333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO25 ) == true )
			m_hFinalStrikeBlackHole = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( this, L"ELSWORD_FINALSTRIKE_BLACKHOLE_MEMO", GetPowerRate(), m_vFinalStrikeBlackHolePos, GetRotateDegree(), GetRotateDegree() );
		else
#endif //ADD_MEMO_1ST_CLASS
        m_hFinalStrikeBlackHole = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_FinalStrikeBlackHole = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            this, L"ELSWORD_FINALSTRIKE_BLACKHOLE", GetPowerRate(), m_vFinalStrikeBlackHolePos, GetRotateDegree(), GetRotateDegree() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.53333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.53333f ) == true && EventCheck( 2.53333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( m_hFinalStrikeBlackHole != INVALID_DAMAGE_EFFECT_HANDLE )
		    g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hFinalStrikeBlackHole );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		g_pX2Game->GetDamageEffect()->DestroyInstance( m_FinalStrikeBlackHole );
        m_FinalStrikeBlackHole = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.4f ) == true && EventCheck( 2.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_FINALSTRIKE", GetPowerRate(), m_vFinalStrikeBlackHolePos, GetRotateDegree(), GetRotateDegree() );
		UpDownCrashCamera( 45.0f, 0.4f );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.8f ) == true && EventCheck( 2.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 8.0f, 1.0f );
	}

	CommonFrameMove();
}



void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_FINAL_STRIKE_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_FINAL_STRIKE_End()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hFinalStrikeBlackHole != INVALID_DAMAGE_EFFECT_HANDLE )
	    g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hFinalStrikeBlackHole );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	g_pX2Game->GetDamageEffect()->DestroyInstance( m_FinalStrikeBlackHole );
    m_FinalStrikeBlackHole = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CommonStateEnd();
}



void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_SWORD_FALL_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"elsword_active_swordFall_sword01.X" );
	XSkinMeshReadyInBackground( L"elsword_active_swordFall_sword02.X" );
	XSkinMeshReadyInBackground( L"elsword_active_swordFall_spark.X" );
	XSkinMeshReadyInBackground( L"elsword_active_swordFall_downSword.X" );
	XMeshReadyInBackground( L"elsword_active_swordFall_light.Y" );
	TextureReadyInBackground( L"elsword_active_swordFall_sword.dds" );
	TextureReadyInBackground( L"elsword_active_swordFall_spark.dds" );
	TextureReadyInBackground( L"elsword_active_swordFall_light.dds" );
	TextureReadyInBackground( L"elsword_active_swordFall_downSpark.DDS" );
	TextureReadyInBackground( L"elsword_active_swordFall_crack.dds" );
	TextureReadyInBackground( L"elsword_active_crecentCut_spark.dds" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"elsword_active_swordFall_sword01.X" );
	XSkinMeshReady( L"elsword_active_swordFall_sword02.X" );
	XSkinMeshReady( L"elsword_active_swordFall_spark.X" );
	XSkinMeshReady( L"elsword_active_swordFall_downSword.X" );
	XMeshReady( L"elsword_active_swordFall_light.Y" );
	TextureReady( L"elsword_active_swordFall_sword.dds" );
	TextureReady( L"elsword_active_swordFall_spark.dds" );
	TextureReady( L"elsword_active_swordFall_light.dds" );
	TextureReady( L"elsword_active_swordFall_downSpark.DDS" );
	TextureReady( L"elsword_active_swordFall_crack.dds" );
	TextureReady( L"elsword_active_crecentCut_spark.dds" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_SWORD_FALL_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.004f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SECOND_BLADE", this );

		D3DXVECTOR3 vEffectPos = GetBonePos( L"Bip01" );

		vEffectPos.y += ( 60.f );

		if(GetIsRight() == true)
			vEffectPos += (0.f * GetDirVector() );
		else
			vEffectPos -= (0.f * GetDirVector() );

        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M01", vEffectPos, GetRotateDegree(), GetRotateDegree() );
		m_vSwordFallSheathPos = vEffectPos;
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03333f ) == true && EventCheck( 0.03333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M02", vEffectPos, GetRotateDegree(), GetRotateDegree() );
        if ( pMeshInst != NULL )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		    m_hSwordFallBladeFirst = pMeshInst->GetHandle();
        }

	}
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.06666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.06666f ) == true && EventCheck( 0.06666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M03", vEffectPos, GetRotateDegree(), GetRotateDegree() );
        if ( pMeshInst != NULL )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		    m_hSwordFallBladeSecond = pMeshInst->GetHandle();
        }
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M04", vEffectPos, GetRotateDegree(), GetRotateDegree() );
        if ( pMeshInst != NULL )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		    m_hSwordFallBladeThird = pMeshInst->GetHandle();
        }
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.15f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.15f ) == true && EventCheck( 0.15f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M05", vEffectPos, GetRotateDegree(), GetRotateDegree() );
        if ( pMeshInst != NULL )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		    m_hSwordFallBladeFourth = pMeshInst->GetHandle();
        }
	}

#ifdef ADDITIONAL_MEMO
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO15 ) == true )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.05f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.05f ) == true && EventCheck( 0.05f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M02_Memo", vEffectPos, GetRotateDegree(), GetRotateDegree() );
            if ( pMeshInst != NULL )
            {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    m_hSwordFallBladeFirst_Memo = pMeshInst->GetHandle();
            }
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.08f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.08f ) == true && EventCheck( 0.08f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M03_Memo", vEffectPos, GetRotateDegree(), GetRotateDegree() );
            if ( pMeshInst != NULL )
            {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    m_hSwordFallBladeSecond_Memo = pMeshInst->GetHandle();
            }
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.125f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.125f ) == true && EventCheck( 0.125f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M04_Memo", vEffectPos, GetRotateDegree(), GetRotateDegree() );
            if ( pMeshInst != NULL )
            {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    m_hSwordFallBladeThird_Memo = pMeshInst->GetHandle();
            }
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.17f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.17f ) == true && EventCheck( 0.17f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vEffectPos = m_vSwordFallSheathPos;
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"elsword_A_swordFall_M05_Memo", vEffectPos, GetRotateDegree(), GetRotateDegree() );
            if ( pMeshInst != NULL )
            {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    m_hSwordFallBladeFourth_Memo = pMeshInst->GetHandle();
            }
		}
	}
#endif

	float fPowerRate = GetPowerRate();

#ifndef UPGRADE_SKILL_SYSTEM_2013 //김창한 //스크립트에서 처리하기로 함.
#ifdef ADDITIONAL_MEMO
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO15 ) == true )
	{
		fPowerRate *= 0.6f;
	}
#endif
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFirst );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );

			vEffectPos.y += ( 42.f );

			if(GetIsRight() == true)
				vEffectPos += (-59.f * GetDirVector() );
			else
				vEffectPos -= (-59.f * GetDirVector() );

#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO15 ) == true )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE1_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE1", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE1", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#endif
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFirst );
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.16666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.16666f ) == true && EventCheck( 1.16666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeSecond );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );

			vEffectPos.y += ( 42.f );

			if(GetIsRight() == true)
				vEffectPos += (-59.f * GetDirVector() );
			else
				vEffectPos -= (-59.f * GetDirVector() );

#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO15 ) == true )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE2_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE2", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE2", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#endif
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeSecond );
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.33333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.33333f ) == true && EventCheck( 1.33333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeThird );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );
			vEffectPos.y += ( 42.f );

			if(GetIsRight() == true)
				vEffectPos += (-59.f * GetDirVector() );
			else
				vEffectPos -= (-59.f * GetDirVector() );

#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO15 ) == true )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE3_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE3", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE3", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#endif
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeThird );
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFourth );
		if( NULL != pMeshInst )
		{
			D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );
			vEffectPos.y += ( 42.f );

			if(GetIsRight() == true)
				vEffectPos += (-59.f * GetDirVector() );
			else
				vEffectPos -= (-59.f * GetDirVector() );

#ifdef ADDITIONAL_MEMO
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO15 ) == true )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE4_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE4", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE4", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );
#endif
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFourth );
		}
	}

#ifdef ADDITIONAL_MEMO
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO15 ) == true )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.917f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.917f ) == true && EventCheck( 0.917f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFirst_Memo );
			if( NULL != pMeshInst )
			{
				D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );

				vEffectPos.y += ( 42.f );

				if(GetIsRight() == true)
					vEffectPos += (-59.f * GetDirVector() );
				else
					vEffectPos -= (-59.f * GetDirVector() );

				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE1_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );

				g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFirst_Memo );
			}
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.083f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.083f ) == true && EventCheck( 1.083f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeSecond_Memo );
			if( NULL != pMeshInst )
			{
				D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );

				vEffectPos.y += ( 42.f );

				if(GetIsRight() == true)
					vEffectPos += (-59.f * GetDirVector() );
				else
					vEffectPos -= (-59.f * GetDirVector() );

				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE2_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );

				g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeSecond_Memo );
			}
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.25f ) == true && EventCheck( 1.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeThird_Memo );
			if( NULL != pMeshInst )
			{
				D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );
				vEffectPos.y += ( 42.f );

				if(GetIsRight() == true)
					vEffectPos += (-59.f * GetDirVector() );
				else
					vEffectPos -= (-59.f * GetDirVector() );

				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE3_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );

				g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeThird_Memo );
			}
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.416f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.416f ) == true && EventCheck( 1.416f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFourth_Memo );
			if( NULL != pMeshInst )
			{
				D3DXVECTOR3 vEffectPos = pMeshInst->GetBonePos( L"Dummy01" );
				vEffectPos.y += ( 42.f );

				if(GetIsRight() == true)
					vEffectPos += (-59.f * GetDirVector() );
				else
					vEffectPos -= (-59.f * GetDirVector() );

				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_SWORDFALL_BLADE4_MEMO", fPowerRate, vEffectPos, GetRotateDegree(), GetRotateDegree() );

				g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFourth_Memo );
			}
		}
	}	
#endif

	CommonFrameMove();
}



void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_SWORD_FALL_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_SWORD_FALL_End()
{
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstFirst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFirst );
	if( NULL != pMeshInstFirst )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFirst );
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstSecond = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeSecond );
	if( NULL != pMeshInstSecond )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeSecond );
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstThird = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeThird );
	if( NULL != pMeshInstThird )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeThird );
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstFourth = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFourth );
	if( NULL != pMeshInstFourth )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFourth );
	}

#ifdef ADDITIONAL_MEMO
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFirst_Memo );
	if( NULL != pMeshInstFirst )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFirst_Memo );
	}
	pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeSecond_Memo );
	if( NULL != pMeshInst )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeSecond_Memo );
	}
	pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeThird_Memo );
	if( NULL != pMeshInst )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeThird_Memo );
	}
	pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hSwordFallBladeFourth_Memo );
	if( NULL != pMeshInst )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hSwordFallBladeFourth_Memo );
	}
#endif

	CommonStateEnd();
}



void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_CRESCENT_CUT_Init()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReadyInBackground( L"elsword_active_crecentCut_sheath.X" );
	TextureReadyInBackground( L"elsword_active_crecentCut_trace.DDS" );
	TextureReadyInBackground( L"elsword_active_crecentCut_strike.DDS" );
	TextureReadyInBackground( L"elsword_active_crecentCut_mark.DDS" );
	TextureReadyInBackground( L"elsword_active_crecentCut_crecent.dds" );
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	XSkinMeshReady( L"elsword_active_crecentCut_sheath.X" );
	TextureReady( L"elsword_active_crecentCut_trace.DDS" );
	TextureReady( L"elsword_active_crecentCut_strike.DDS" );
	TextureReady( L"elsword_active_crecentCut_mark.DDS" );
	TextureReady( L"elsword_active_crecentCut_crecent.dds" );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_CRESCENT_CUT_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 DamageEffectPos = GetPos();

		DamageEffectPos.y += 80.f;

		if(GetIsRight() == true)
		{
			DamageEffectPos += (130.f * GetDirVector() );
		}
		else
		{
			DamageEffectPos -= (130.f * GetDirVector() );
		}
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_CRESCENT_ONE", this );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_CRESCENT_ONE", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 DamageEffectPos = GetPos();

		DamageEffectPos.y += 80.f;

		if(GetIsRight() == true)
		{
			DamageEffectPos += (130.f * GetDirVector() );
		}
		else
		{
			DamageEffectPos -= (130.f * GetDirVector() );
		}
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_CRESCENT_TWO", this );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_CRESCENT_TWO", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree() );
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.48f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.48f ) == true && EventCheck( 1.48f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 DamageEffectPos = GetPos();

		DamageEffectPos.y += 80.f;

		if(GetIsRight() == true)
		{
			DamageEffectPos += (130.f * GetDirVector() );
		}
		else
		{
			DamageEffectPos -= (130.f * GetDirVector() );
		}
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_CRESCENT_THREE", this );

#ifdef ADDITIONAL_MEMO
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO14 ) == true && GetRandomFloat() <= 0.5f )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_CRESCENT_THREE_MEMO", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree() );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_CRESCENT_THREE", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree() );
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ELSWORD_CRESCENT_THREE", GetPowerRate(), DamageEffectPos, GetRotateDegree(), GetRotateDegree() );
#endif
	}

	CommonFrameMove();
}



void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_CRESCENT_CUT_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_CRESCENT_CUT_End()
{
	CommonStateEnd();
}


void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_PHANTOM_SWORD_Init()
{
	XSkinMeshReady( L"Elsword_SisKnight_sword.x" );
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_PHANTOM_SWORD_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.05f, 0 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstSheath = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ElswordPhantomSwordSheath01", GetPos(), GetRotateDegree(), GetRotateDegree() );
		if( pMeshInstSheath != NULL )
		{
			m_hThirdBladeSheathMesh = pMeshInstSheath->GetHandle();
		}
	}

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstSheath = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hThirdBladeSheathMesh );
	if( NULL != pMeshInstSheath)
	{
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );

		D3DXMATRIX matCombineMatrix;
		D3DXMatrixScaling( &matCombineMatrix, 1.3f, 1.3f, 1.3f );
		matCombineMatrix = matCombineMatrix * pMultiAnimFrame->combineMatrix;
		pMeshInstSheath->SetUseDXMatrix( matCombineMatrix );
	}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstSheath = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hThirdBladeSheathMesh );
		if( NULL != pMeshInstSheath )
		{
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hThirdBladeSheathMesh );
		}
	}




#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.06f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.06f ) == true && EventCheck( 0.06f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_SECOND_BLADE", this );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.0333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.0333f ) == true && EventCheck( 0.0333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ETK_PHANTOM_SWORD );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if(GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO26) == true)
			SetBuffFactorToGameUnit( pSkillTemplet, 1 );
		else
#endif //ADD_MEMO_1ST_CLASS
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	}

	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_ETK_PHANTOM_SWORD_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}



#endif ELSWORD_SHEATH_KNIGHT


#ifdef SERV_ELSWORD_INFINITY_SWORD
void CX2GUElsword_SwordMan::ESSI_EIS_DASH_COMBO_XX_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	ELSE_IF_STATE_CHANGE_ON_EX_( 0, m_fEventTime[0].keyInputStart, m_fEventTime[0].keyInputEnd, m_fEventTime[0].stateChange, m_InputData.oneX == true, ESSI_EIS_DASH_COMBO_XXX )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_EIS_DASH_COMBO_XXX_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fWalkCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_EIS_DASHJUMP_COMBO_XX_EventProcess()
{
//#ifdef LINEMAP_FAST_WIND_TEST
//	IF_TIME_ELAPSED_IN_THE_AIR_THEN_STATE_CHANGE( 1.f, USI_JUMP_DOWN )
//#endif LINEMAP_FAST_WIND_TEST

	if( m_InputData.oneX == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	else if( m_InputData.oneZ == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}
	else if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( ESSI_EIS_DASHJUMP_COMBO_XXX );
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
			StateChange( ESSI_COMBO_Z1 );
		}
		else
		{
			StateChange( ESSI_DASH_JUMP_COMBO_X2_LANDING );
		}
	}

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_EIS_DASHJUMP_COMBO_XXX_FrameMove()
{
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
			m_DamageData.attackerType		= CX2DamageManager::AT_UNIT;
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
void CX2GUElsword_SwordMan::ESSI_EIS_DASHJUMP_COMBO_XXX_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		if( true == IsOnSomethingFuture() )
			StateChange( USI_WAIT );
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}

void CX2GUElsword_SwordMan::ESSI_EIS_DASHJUMP_COMBO_XXX_End()
{
	SetEnableAttackBox(L"Sword", true );
	CommonStateEnd();
}

void CX2GUElsword_SwordMan::ESSI_SI_A_EIS_SPIRAL_STING_FrameMove()
{
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
	}


	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI_A_EIS_SPIRAL_STING_EventProcess()
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
void CX2GUElsword_SwordMan::ESSI_SI_SA_EIS_SWORD_BLASTING_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( 0.01f, 0 );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vRotDegree = GetRotateDegree();
		D3DXVECTOR3 vDirVec = GetDirVector();
		D3DXVECTOR3 vZVec = GetZVector();
		vDirVec.y = 0.f;
				
		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vTargetPos = vPos;
		if( GetIsRight() == true )
		{
			vPos -= (150.f * GetScaleByUnit().x * GetDirVector() );
			vTargetPos = vTargetPos + (vDirVec * 1000.f);
		}
		else
		{
			vPos += (150.f * GetScaleByUnit().x * GetDirVector() );
			vTargetPos = vTargetPos - (vDirVec * 1000.f);
		}
				
		for(int i=0; i<3; ++i)
		{
			vPos.y += (100.f * GetScaleByUnit().y);

			float fReserveTime = 0.f;
			if( i == 1 )
				fReserveTime = 0.1f;
			else if( i == 2 )
				fReserveTime = 0.05f;
			CX2DamageEffect::CEffect* pDamageSwordBlasting = g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, L"ELSWORD_EIS_SWORDBLASTING_PHANTOMSWORD", GetPowerRate(), vPos, 
				vRotDegree, vRotDegree, m_FrameDataNow.unitCondition.landPosition.y, true, fReserveTime  );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hDamageSwordBlasting[i] = ( pDamageSwordBlasting != NULL ) ? pDamageSwordBlasting->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_pDamageSwordBlasting[i] = pDamageSwordBlasting;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( pDamageSwordBlasting != NULL )
				pDamageSwordBlasting->SetLockOnPos( vTargetPos );

		}		
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.95f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.95f ) == true && EventCheck( 0.95f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDirVec = GetDirVector();
		D3DXVECTOR3 vPos = GetPos();		
		if( GetIsRight() == true )
		{
			vPos = vPos + (vDirVec * 100.f);
		}
		else
		{
			vPos = vPos - (vDirVec * 100.f);
		}
		vPos.y += 100.f;

		for(int i=0; i<3; ++i)
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->GetInstance( m_hDamageSwordBlasting[i] );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect::CEffect* pEffect = ( g_pX2Game->GetDamageEffect()->IsLiveInstance( m_pDamageSwordBlasting[i] ) == true ) ? m_pDamageSwordBlasting[i] : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( pEffect != NULL && pEffect->GetMainEffect() != NULL )
            {
                pEffect->SetLockOnPos( vPos );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_hDamageSwordBlasting[i] = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_pDamageSwordBlasting[i] = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            }
		}		
	}
	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_SI_SA_EIS_SWORD_BLASTING_EventProcess()
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

void CX2GUElsword_SwordMan::ESSI_SI_SA_EIS_RAGE_CUTTER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet(CX2SkillTree::SI_SA_EIS_RAGE_CUTTER);
		if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
		{
			D3DXVECTOR3 vRotDegree = GetRotateDegree();		
			D3DXVECTOR3 vPos = GetPos();
			vPos.y += 10.f;

			CX2DamageEffect::CEffect *pDamageEffect 
				= g_pX2Game->GetDamageEffect()->CreateInstance( (CX2GameUnit*) this, 
				L"ELSWORD_SA_RAGECUTTER_HIT_M01", GetPowerRate(), vPos, vRotDegree, vRotDegree, 
				m_FrameDataNow.unitCondition.landPosition.y );

			if ( NULL != pDamageEffect )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			{
				if ( NULL == GetUnit() )
					return;

				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

				pDamageEffect->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
			}
#else //UPGRADE_SKILL_SYSTEM_2013
				pDamageEffect->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}		
	}
	CommonFrameMove();
}

void CX2GUElsword_SwordMan::ESSI_SI_SA_EIS_RAGE_CUTTER_EventProcess()
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
#endif //SERV_ELSWORD_INFINITY_SWORD





#ifdef GRAPPLING_TEST


	void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_FRONT_StartFuture()
	{
		CommonStateStartFuture();

		INIT_VECTOR2( m_PhysicParam.nowSpeed, 0, 0 );
		INIT_VECTOR2( m_PhysicParam.passiveSpeed, -1, -1 );

		m_FrameDataFuture.unitCondition.m_GrapplingState.m_pFrameGrappled = m_pXSkinAnim->GetCloneFrame( L"Bip01_Neck" );
		m_FrameDataNow.unitCondition.m_GrapplingState.m_pFrameGrappled = m_pXSkinAnim->GetCloneFrame( L"Bip01_Neck" );
	}

	void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_FRONT_Start()
	{
		CommonStateStart();

		m_FrameDataFuture.unitCondition.m_GrapplingState.m_pFrameGrappled = m_pXSkinAnim->GetCloneFrame( L"Bip01_Neck" );
		m_FrameDataNow.unitCondition.m_GrapplingState.m_pFrameGrappled = m_pXSkinAnim->GetCloneFrame( L"Bip01_Neck" );
	}


	void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_FRONT_FrameMoveFuture()
	{
		CommonFrameMoveFuture();
	}

	void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_FRONT_EventProcess()
	{
		//const float MAGIC_TIME = 10.f;
		//if( m_FrameDataFuture.unitCondition.fStateTime > MAGIC_TIME )
		//{
		//	StateChange( m_DamageSmallFront );
		//}
		//else if( NULL == GetUnitGrapplingMe( true ) )
		//{
		//	StateChange( m_DamageSmallFront );
		//}
	

		CommonEventProcess();
	}

	void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_FRONT_EndFuture()
	{
		//ResetGrapplingState();		// fix!! 이 부분도 다른 곳에서 처리해주기 때문에 필요없을 듯
		CommonStateEnd();
	}

	void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_FRONT_End()
	{
		//ResetGrapplingState();		// fix!! 이 부분도 다른 곳에서 처리해주기 때문에 필요없을 듯
		CommonStateEnd();
	}






	//void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_BACK_StartFuture()
	//{
	//	CommonStateStartFuture();

	//	INIT_VECTOR2( m_PhysicParam.nowSpeed, 0, 0 );
	//	INIT_VECTOR2( m_PhysicParam.passiveSpeed, -1, -1 );


	//	m_pFrameGrappled = m_pXSkinAnimFuture->GetCloneFrame( L"Bip01_R_Foot" );
	//}

	//void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_BACK_FrameMoveFuture()
	//{
	//	//CX2GameUnit* pGameUnitGrapplingMe = GetUnitGrapplingMe();
	//	//if( NULL != pGameUnitGrapplingMe )
	//	//{
	//	//	pGameUnitGrapplingMe->CalcGrapplingPosition();
	//	//	D3DXVECTOR3 vGrapplingPosition = pGameUnitGrapplingMe->GetGrapplingPosition();

	//	//	CalcGrappledPosition();
	//	//	D3DXVECTOR3 vRelativeGrappledPosition = GetPos(true) - GetGrappledPosition();

	//	//	m_FrameDataFuture.syncData.position = vGrapplingPosition + vRelativeGrappledPosition;
	//	//}

	//	CommonFrameMoveFuture();
	//}

	//void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_BACK_EventProcess()
	//{
	//	if( m_FrameDataFuture.unitCondition.fStateTime > 10.f )
	//	{

	//		StateChange( m_DamageSmallFront );
	//	}
	//	else if( NULL == GetUnitGrapplingMe() )
	//	{

	//		StateChange( m_DamageAirDown );
	//	}
	//	else if( this != GetUnitGrapplingMe()->GetUnitGrappledByMe() )
	//	{

	//		StateChange( m_DamageAirFall );
	//	}

	//	CommonEventProcess();
	//}

	//void CX2GUElsword_SwordMan::ESSI_DAMAGE_GRAPPLED_BACK_End()
	//{
	//	ResetGrapplingState();
	//	CommonStateEnd();
	//}












	void CX2GUElsword_SwordMan::ESSI_GRAB_FRONT_StartFuture()
	{
		CommonStateStartFuture();

		m_FrameDataFuture.unitCondition.m_GrapplingState.m_pFrameGrappling = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
		m_FrameDataNow.unitCondition.m_GrapplingState.m_pFrameGrappling = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
	}

	void CX2GUElsword_SwordMan::ESSI_GRAB_FRONT_Start()
	{
		CommonStateStart();

		m_FrameDataFuture.unitCondition.m_GrapplingState.m_pFrameGrappling = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
		m_FrameDataNow.unitCondition.m_GrapplingState.m_pFrameGrappling = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
	}

	void CX2GUElsword_SwordMan::ESSI_GRAB_FRONT_FrameMoveFuture()
	{
		CommonFrameMoveFuture();
	}

	void CX2GUElsword_SwordMan::ESSI_GRAB_FRONT_EventProcess()
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

	void CX2GUElsword_SwordMan::ESSI_GRAB_FRONT_End()
	{
		CommonStateEnd();
	}





	void CX2GUElsword_SwordMan::ESSI_THROW_FORWARD_StartFuture()
	{
		CommonStateStartFuture();
	}

	void CX2GUElsword_SwordMan::ESSI_THROW_FORWARD_FrameMoveFuture()
	{

		CommonFrameMoveFuture();
	}

	void CX2GUElsword_SwordMan::ESSI_THROW_FORWARD_EventProcess()
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

	void CX2GUElsword_SwordMan::ESSI_THROW_FORWARD_EndFuture()
	{
		CommonStateEndFuture();
	}



	void CX2GUElsword_SwordMan::ESSI_THROW_FORWARD_End()
	{
		//CX2GameUnit* pGrappled = GetUnitGrappledByMe();
		//if( NULL != pGrappled )
		//{

		//	switch( pGrappled->GetGameUnitType() )
		//	{
		//	case CX2GameUnit::GUT_USER:
		//		{
		//			CX2GUUser* pUser = (CX2GUUser*) pGrappled;
		//			pUser->SetSpeedX( 2000.f );
		//			pUser->SetSpeedY( 1500.f );

		//			pUser->StateChange( pUser->GetDamageAirDownStateID() );
		//		} break;

		//	case CX2GameUnit::GUT_NPC:
		//		{
		//			CX2GUNPC* pNPC = (CX2GUNPC*) pGrappled;
		//			pNPC->SetSpeedX( 2000.f );
		//			pNPC->SetSpeedY( 1500.f );
		//			pNPC->StateChange( pNPC->GetFlyDamageFrontStateID() );
		//		} break;
		//	}
		//}
		//ResetGrapplingState();
		CommonStateEnd();
	}

#endif GRAPPLING_TEST

	//{{ JHKang / 강정훈 / 2010/11/01 / 루나 블레이드 추가
#ifdef NEW_SKILL_2010_11
	/** 
		@function ESSI_SA_ERS_LUNA_BLADE_Init
		@brief 관련된 리소스 로딩
			- 메쉬 : 루나 블레이드, 섬광 등
	*/
	void CX2GUElsword_SwordMan::ESSI_SA_ERS_LUNA_BLADE_Init()
	{
		XSkinMeshReadyInBackground( L"Elsword_RS_DashJumpComboXa_Mesh01.X" );
		XSkinMeshReadyInBackground( L"SI_SA_ERS_LUNA_BLADE_BLADE.X" );
		XSkinMeshReadyInBackground( L"SI_SA_ERS_LUNA_BLADE_Light.X" );
		TextureReadyInBackground( L"RB_Slash01.tga" );
		TextureReadyInBackground( L"Mesh_LUNA_BLADE.tga" );
		TextureReadyInBackground( L"Lightning02_LB.tga" );
		TextureReadyInBackground( L"ColorBallBlue_LB.tga" );
	}

	/** 
		@function ESSI_SA_ERS_LUNA_BLADE_FrameMove
		@brief 애니메이션 및 데미지 이펙트 출력(중요)
		- 
	*/
	void CX2GUElsword_SwordMan::ESSI_SA_ERS_LUNA_BLADE_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( 0.06f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.075f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.075f ) == true && EventCheck( 0.075f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vPos = GetBonePos( L"Bip01_Head" );
			if( true == GetIsRight() )
			{
				vPos += GetDirVector() * -380.f * GetScaleByUnit().x;
			}
			else
			{
				vPos -= GetDirVector() * -380.f * GetScaleByUnit().x;
			}
			vPos.y += 470.f * GetScaleByUnit().y;

			CX2DamageEffect::CEffect* pEffect 
				= ( IsHyperState() ? g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SA_ERS_LUNA_BLADE_HYPER", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) 
				: g_pX2Game->GetDamageEffect()->CreateInstance( this, L"SA_ERS_LUNA_BLADE", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y )
				);
            			
			if ( NULL != pEffect )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet(CX2SkillTree::SI_SA_ERS_LUNA_BLADE);
				if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				{
					if ( NULL == GetUnit() )
						return;

					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

					pEffect->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
				}
#else //UPGRADE_SKILL_SYSTEM_2013
					pEffect->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
		}
 
		CommonFrameMove();
	}

	void CX2GUElsword_SwordMan::ESSI_SA_ERS_LUNA_BLADE_EventProcess()
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
			if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.98f )
			{
				StateChange( USI_WAIT );
			}
		}
		CommonEventProcess();
	}

#endif NEW_SKILL_2010_11
	//}} JHKang / 강정훈 / 2010/11/01 / 루나 블레이드 추가

void CX2GUElsword_SwordMan::ShowActiveSkillCutInAndLight( const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ /*= false */ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
 	if( m_pXSkinAnim->EventTimer( fTimeToShow_ ) == true && EventCheck( fTimeToShow_, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
 		if ( GetShowCutInAndChangeWorldColor() && GetShowActiveSkillShow() )
#ifdef SERV_APRIL_FOOLS_DAY
			( true ==  m_bIsFoolsDay )?
				ActiveSkillShow( L"Dummy1_Rhand", s_SkillCutInSetFoolsDay.fileName.c_str(), s_SkillCutInSetFoolsDay.vSize, s_SkillCutInSetFoolsDay.vPosition, IsHyperState(), bOnlyLight_ ) :
#endif //APRIL_FOOLS_DAY
 			ActiveSkillShow( L"Dummy1_Rhand", s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].fileName.c_str(), s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].vSize, s_SkillCutInSet[m_iSkillCutInSetIndex].cutIn[uiCutInIndex_].vPosition, IsHyperState(), bOnlyLight_ );
 	}
}

/*virtual*/ CX2GageData* CX2GUElsword_SwordMan::CreateGageData()
{
	if ( IsMyUnit() )
	{
		const CX2ElswordGageData* pElswordGageData 
			= static_cast<const CX2ElswordGageData*>( CX2GageManager::GetInstance()->GetMyGageData() );

		if ( NULL != pElswordGageData )
			return new CX2ElswordGageData( *pElswordGageData );
		else
			return new CX2ElswordGageData();
	}
	else
		return new CX2ElswordGageData();
	
}

bool CX2GUElsword_SwordMan::ShouldResetNotBusyTimer()
{
	if ( false == CX2GUUser::ShouldResetNotBusyTimer() )
		return false;
	else if ( ESSI_SWORD_WAIT == m_FrameDataFuture.syncData.nowState )
		return false;
	else
		return true;	// 리셋 해야 함
}

/*virtual*/ void CX2GUElsword_SwordMan::DeleteMinorParticle()
{
	CX2GUUser::DeleteMinorParticle();

	DeleteElswordMinorParticle();
}

/** @function : HyperModeBuffEffectStart
	@brief : 각성 시 나와야하는 이펙트들 시작
*/
/*virtual*/ void CX2GUElsword_SwordMan::HyperModeBuffEffectStart()
{
	CX2GUUser::HyperModeBuffEffectStart();

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
		m_hHyperBoostRFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRight",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
		m_hHyperBoostLFoot	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeft",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
		m_hHyperBoostRArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRight",	0, 0, 0, 0, 0);

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
		m_hHyperBoostLArm	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeft",	0, 0, 0, 0, 0);
}

#ifdef	SERV_ELSWORD_INFINITY_SWORD
void CX2GUElsword_SwordMan::LightingStepFrameMove()
{
	if( IsDashState() == true )		/// DASH, DASH_END, DASH_JUMP, DASH_JUMP_LANDING 인 경우
	{
		m_fDashStateTime += m_fElapsedTime;
		if( m_fDashStateTime > 1.f )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillLevelLightningStep = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
			const int iSkillLevelLightningStep = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			
			if ( iSkillLevelLightningStep > 0 )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				const CX2SkillTree::SkillTemplet* pSkillTemplet 
					= g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP );
	#else // UPGRADE_SKILL_SYSTEM_2013
				const CX2SkillTree::SkillTemplet* pSkillTemplet 
					= g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_EIS_LIGHTNING_STEP, iSkillLevelLightningStep );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				if ( NULL != pSkillTemplet  )
					SetBuffFactorToGameUnit( pSkillTemplet, 0 );
			}
			m_fDashStateTime = 0.f;
		}
	}
	else	/// 위에서 지정된 상태가 아닌 경우
	{
		m_fDashStateTime = 0.f;	/// 
	}
}
#endif	SERV_ELSWORD_INFINITY_SWORD

/** @function : SetSpecificValueByBuffTempletID
	@brief : 각 유닛마다 특정 버프가 실행 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 설정 등...)
	@param : 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUElsword_SwordMan::SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_SI_P_ETK_BRUTAL_SLAYER:
		{
			m_iBrutalSlayerActivatedLevel = 1;
		} break;

#ifdef BALANCE_RUNE_SLAYER_20130214
	case BTI_SI_SA_EMK_SWORD_FIRE:
	case BTI_BUFF_SWORD_ENCHANT_ICE:
	case BTI_BUFF_SWORD_ENCHANT_FIRE:
	case BTI_BUFF_SWORD_ENCHANT_POISON:
		{
			m_bShowWeaponEnchantEffect = false;
			SetNeedToUpdateWeaponEffect(true);
		} break;
#endif //BALANCE_RUNE_SLAYER_20130214
	default:
		break;
	}
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
#ifdef BALANCE_PATCH_20131107
	if( 0 < m_iSkillLevelInduranceOfRevenge )
	{
		// 디버프가 걸렸을때인지 체크
		CX2BuffTempletPtr ptrBuffTemplet = CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( eBuffTempletId_ );
		if( NULL != ptrBuffTemplet && true == ptrBuffTemplet->IsDeBuff() && false == ptrBuffTemplet->IsWorldBuff() )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_INDURANCE_OF_REVENGE );
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}
#else //BALANCE_PATCH_20131107
	if( m_ucSaveDebuffCount < m_ucNumOfDeBuff )
	{
		m_ucSaveDebuffCount = m_ucNumOfDeBuff;
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillLevelInduranceOfRevenge = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ELK_INDURANCE_OF_REVENGE, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const int iSkillLevelInduranceOfRevenge = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ELK_INDURANCE_OF_REVENGE );
	#endif // UPGRADE_SKILL_SYSTEM_2013

		if( iSkillLevelInduranceOfRevenge > 0 )
		{	
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_INDURANCE_OF_REVENGE );
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	#else // UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_INDURANCE_OF_REVENGE, iSkillLevelInduranceOfRevenge );
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
	else
		 m_ucSaveDebuffCount = m_ucNumOfDeBuff;
#endif //BALANCE_PATCH_20131107
#endif //UPGRADE_SKILL_SYSTEM_2013

}

/** @function : UnSetSpecificValueByBuffTempletID
	@brief : 각 유닛마다 특정 버프가 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 해제 됬다는 플래그 설정 등...)
	@param : 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUElsword_SwordMan::UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_SI_P_ETK_BRUTAL_SLAYER:
		{
			m_iBrutalSlayerActivatedLevel = 0;
		} break;
#ifdef BALANCE_RUNE_SLAYER_20130214
	case BTI_SI_SA_EMK_SWORD_FIRE:
	case BTI_BUFF_SWORD_ENCHANT_ICE:
	case BTI_BUFF_SWORD_ENCHANT_FIRE:
	case BTI_BUFF_SWORD_ENCHANT_POISON:
		{
			m_bShowWeaponEnchantEffect = true;
			SetNeedToUpdateWeaponEffect(true);
		} break;
#endif //BALANCE_RUNE_SLAYER_20130214
	default:
		break;
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	m_ucSaveDebuffCount = m_ucNumOfDeBuff;
#endif //UPGRADE_SKILL_SYSTEM_2013

}

#ifdef RIDING_SYSTEM
/*virtual*/ bool CX2GUElsword_SwordMan::CanRidingState()
{
	if ( GetNowStateID() == CX2GUUser::USI_WAIT || GetNowStateID() == CX2GUUser::USI_WALK || 
		GetNowStateID() == CX2GUUser::USI_DASH || GetNowStateID() == CX2GUUser::USI_DASH_END || 
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT || GetNowStateID() == CX2GUUser::USI_RIDING_WALK ||
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT_HABIT || GetNowStateID() == CX2GUUser::USI_RIDING_DASH ||
		GetNowStateID() == CX2GUUser::USI_RIDING_DASH_END || GetNowStateID() == CX2GUElsword_SwordMan::ESSI_SWORD_WAIT )
		return true;

	return false;
}
#endif //RIDING_SYSTEM

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한

//-----------------------ESSI_A_ELK_IMPACT_SMASH-----------------------//
void CX2GUElsword_SwordMan::ESSI_A_ELK_IMPACT_SMASH_Init()
{
	TextureReadyInBackground( L"Explosion_Light01" );
	TextureReadyInBackground( L"Whitecircle02.dds" );
	TextureReadyInBackground( L"Whitecircle03.dds" );
	TextureReadyInBackground( L"Smoke.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
	TextureReadyInBackground( L"Mesh_Raven_Event_AC_Upbody21_Effect.tga" );
	TextureReadyInBackground( L"ColorBallFire.dds" );
	XSkinMeshReadyInBackground( L"Effect_ElSword_FlameGagerLand.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	XMeshReadyInBackground( L"Impact_Wave_Half01.Y" );
}
void CX2GUElsword_SwordMan::ESSI_A_ELK_IMPACT_SMASH_EventProcess()
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


//-----------------------ESSI_A_ES_SWORD_WAVE-----------------------//
void CX2GUElsword_SwordMan::ESSI_A_ES_SWORD_WAVE_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"Elsword_Sword_Wave.tga" );
	TextureReadyInBackground( L"Mesh_Raven_Event_AC_Upbody21_Effect.tga" );
	TextureReadyInBackground( L"ColorBallFire.dds" );
}
void CX2GUElsword_SwordMan::ESSI_A_ES_SWORD_WAVE_EventProcess()
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


//-----------------------ESSI_SI_A_ES_DEFENCE-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_READY_StartFuture()
{
	CommonStateStartFuture();

	if( NULL != m_GuardForElswordPtr )
	{
		switch( m_GuardForElswordPtr->GetSkillSlotID_Guard() )
		{
		case USI_SPECIAL_ATTACK_1:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureA );
			break;

		case USI_SPECIAL_ATTACK_2:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureS );
			break;

		case USI_SPECIAL_ATTACK_3:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureD );
			break;

		case USI_SPECIAL_ATTACK_4:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureC );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_1:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureQ );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_2:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureW );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_3:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureE );
			break;

		case USI_SPECIAL_ATTACK_SLOTB_4:
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureR );
			break;

		default:
			ASSERT( !L"ESSI_SI_A_ES_DEFENCE_READY_StartFuture");
			m_GuardForElswordPtr->SetGuardKeyPointer( &m_InputData.pureA );
			break;
		}
	}
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_READY_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESSI_SI_A_ES_DEFENCE_WAIT );
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_WAIT_Start()
{
	m_bIsGuardMotion = true;
	CommonStateStart();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_WAIT_FrameMove()
{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 마나 소모 수정 4 -> 3
	UpNowMp( -_CONST_ELSWORD_::SKILL_DEFENCE_MP_COST_PER_SEC * m_fElapsedTime );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	UpNowMp(-4.f * m_fElapsedTime);
#endif // BALANCE_PATCH_20131107				// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_WAIT_EventProcess()
{
	int iCountToBeAbleToCheckKey = 1;

	if( iCountToBeAbleToCheckKey < m_pXSkinAnimFuture->GetNowPlayCount() )
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 가드 사용 중, MP 가 모두 소진되면 가드 상태 변환 수정
		if( 0 >= static_cast<int> ( GetNowMp() ) 
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		if( 0.f >= GetNowMp() 
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			 || NULL != m_GuardForElswordPtr && false == m_GuardForElswordPtr->GetGuardKeyValue() )
		{
			StateChange( ESSI_SI_A_ES_DEFENCE_END );
		}
	}

	CommonEventProcess();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_END_Start()
{
	m_bIsGuardMotion = false;
	CommonStateStart();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_END_EventProcess()
{
	const float LOWER_BOUND_INPUT_TIME_TO_CANCEL = 0.5f;

	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( USI_WAIT );
		m_fCanNotAttackTime = 0.1f;
	}
	else if( LOWER_BOUND_INPUT_TIME_TO_CANCEL < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		if( true == IsOnSomethingFuture() )
		{
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
			IF_SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
			if( true == SpecialAttackEventProcess() )
			{
			}
#endif //SKILL_CANCEL_BY_HYPER_MODE
			else if( CAN_DASH_CANCEL )
			{
				StateChangeDashIfPossible();
			}
			else if( CAN_WALK_CANCEL )
			{
				StateChange( USI_WALK );
			}
		}
	}
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )

	CommonEventProcess();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_DAMAGE_FrameMove()
{
	UpNowMp(-4.f * m_fElapsedTime);
	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ES_DEFENCE_DAMAGE_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
		StateChange( ESSI_SI_A_ES_DEFENCE_WAIT );
	}
	else if( 0.f >= GetNowMp() || 0.f >= GetNowHp() || NULL != m_GuardForElswordPtr && false == m_GuardForElswordPtr->GetGuardKeyValue() )
	{
		StateChange( ESSI_SI_A_ES_DEFENCE_END );
	}

	CommonEventProcess();
}

//-----------------------ESSI_SI_P_ESK_AUTO_GUARD-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_P_ESK_AUTO_GUARD_Start()
{
	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_CSI_CIP_IRON_WILL", this );
	int GuardDamageReducePercent = 0;
	if( NULL != m_GuardForElswordPtr )
		GuardDamageReducePercent = m_GuardForElswordPtr->GetDamageReducePercent();

	SetDamageReducePercent( GetDamageReducePercent() + GuardDamageReducePercent );
	CommonStateStart();
}
void CX2GUElsword_SwordMan::ESSI_SI_P_ESK_AUTO_GUARD_EventProcess()
{
	if( true == m_pXSkinAnimFuture->IsAnimationEnd() )
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
void CX2GUElsword_SwordMan::ESSI_SI_P_ESK_AUTO_GUARD_End()
{
	CommonStateEnd();
	int GuardDamageReducePercent = 0;
	if( NULL != m_GuardForElswordPtr )
		GuardDamageReducePercent = m_GuardForElswordPtr->GetDamageReducePercent();

	SetDamageReducePercent( GetDamageReducePercent() - GuardDamageReducePercent );
}

//-----------------------ESSI_SI_SA_EMK_RISING_SLASH_WAVE-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_SA_EMK_RISING_SLASH_WAVE_Init()
{
	XSkinMeshReadyInBackground( L"RisingSlash_MagicSquare01_memo.X" );
	XSkinMeshReadyInBackground( L"RisingSlash_Blade01_memo.x" );
	XMeshReadyInBackground( L"Arme_ChargeMp2.Y" );
	TextureReadyInBackground( L"star1.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );
}
void CX2GUElsword_SwordMan::ESSI_SI_SA_EMK_RISING_SLASH_WAVE_EventProcess()
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
void CX2GUElsword_SwordMan::ESSI_SI_SA_EMK_RISING_SLASH_WAVE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.067f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	

#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO20 ) == true )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effect_EMK_RISING_SLASH_WAVE_MEMO", this, NULL, false, GetPowerRate() );
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effect_EMK_RISING_SLASH_WAVE", this, NULL, false, GetPowerRate() );
	}

	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_SI_SA_EMK_RISING_SLASH_WAVE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( 0.067f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO20 ) == true )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effect_EMK_RISING_SLASH_WAVE_MEMO", this, NULL, true, GetPowerRate() );
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effect_EMK_RISING_SLASH_WAVE", this, NULL, true, GetPowerRate() );
	}

	CommonFrameMove();
}

//-----------------------ESSI_SI_A_ERS_CRITICAL_SWORD-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_CRITICAL_SWORD_Init()
{
		TextureReadyInBackground( L"Smoke.dds" );
		TextureReadyInBackground( L"Lire_RingLight01.dds" );
		TextureReadyInBackground( L"star2.dds" );
		TextureReadyInBackground( L"Arme_Damage_Ring.dds" );
		TextureReadyInBackground( L"Arme_Damage_Center.dds" );
		TextureReadyInBackground( L"Arme_Critical.dds" );
		XSkinMeshReadyInBackground( L"Elsword_SI_A_RS_Sword_Enchant_Mesh01.X" );
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_CRITICAL_SWORD_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.333f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.333f ) == true && EventCheck( 0.333f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{	
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ERS_CRITICAL_SWORD );
		if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty())
		{
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}

	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_CRITICAL_SWORD_EventProcess()
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


//-----------------------ESSI_SI_A_ERS_RUNE_OF_FIRE-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_RUNE_OF_FIRE_Init()
{
	TextureReadyInBackground( L"Entrance_Fire01.tga" );		
	TextureReadyInBackground( L"star1.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );
	TextureReadyInBackground( L"Rune_MagicSquare03_R.tga" );
	TextureReadyInBackground( L"Rune_MagicSquare04_LR.dds" );
	XSkinMeshReadyInBackground( L"FRIENDSHIPOFNATURE.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.x" );
	XSkinMeshReadyInBackground( L"Elsword_RS_DashJumpComboXa_Mesh01" );
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_RUNE_OF_FIRE_EventProcess()
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
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_RUNE_OF_FIRE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vSize( m_fRuneProjectileSizeIncPercent,m_fRuneProjectileSizeIncPercent,m_fRuneProjectileSizeIncPercent);
		CX2EffectSet::Handle hEffectRune = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RunSlayer_RUNE_GUARD_FIRE", this, NULL,false,-1.f, -1.f, vSize );
		if ( CX2EffectSet::EffectSetInstance* pEffectInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectRune ) )
		{
			pEffectInstance->m_fLifeTime *= m_fRuneProjectileRangeIncPercent;
			pEffectInstance->m_bDamageEffectScale = true;
		}
	}

	CommonFrameMove();
}


//-----------------------ESSI_SI_A_ERS_RUNE_OF_ICE-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_RUNE_OF_ICE_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_RS_DashJumpComboXa_Mesh01" );
	XSkinMeshReadyInBackground( L"FRIENDSHIPOFNATURE.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.x" );
	TextureReadyInBackground( L"Rune_MagicSquare03_LB.tga" );
	TextureReadyInBackground( L"Rune_MagicSquare04_LB.tga" );
	TextureReadyInBackground( L"Ice_Crystal01.dds" );
	TextureReadyInBackground( L"star1.dds" );
	TextureReadyInBackground( L"Arme_Damage_Center.dds" );
	TextureReadyInBackground( L"Arme_Critical.dds" );
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_RUNE_OF_ICE_EventProcess()
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
void CX2GUElsword_SwordMan::ESSI_SI_A_ERS_RUNE_OF_ICE_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vSize( m_fRuneProjectileSizeIncPercent,m_fRuneProjectileSizeIncPercent,m_fRuneProjectileSizeIncPercent);
		CX2EffectSet::Handle hEffectRune = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_RunSlayer_RUNE_GUARD_ICE", this, NULL,false,-1.f, -1.f, vSize );
		if ( CX2EffectSet::EffectSetInstance* pEffectInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectRune ) )
		{
			pEffectInstance->m_fLifeTime *= m_fRuneProjectileRangeIncPercent;
			pEffectInstance->m_bDamageEffectScale = true;
		}

	}

	CommonFrameMove();
}

//-----------------------ESSI_SI_A_ETK_PIERCING_SWORD-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_A_ETK_PIERCING_SWORD_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_SisKnight_sword.X" );
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ETK_PIERCING_SWORD_EventProcess()
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
void CX2GUElsword_SwordMan::ESSI_SI_A_ETK_PIERCING_SWORD_StateStart()
{
	CommonStateStart();
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ETK_PIERCING_SWORD );
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
		return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_fPiercingSwordProjectileRange = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_fPiercingSwordProjectileRange = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ETK_PIERCING_SWORD_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vDamageEffectPos = GetPos();
		vDamageEffectPos.y += 130.f;
		if(GetIsRight() == true)
		{
			vDamageEffectPos += (30.f * GetDirVector());
		}
		else
		{
			vDamageEffectPos -= (30.f * GetDirVector());
		}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect* pEffect
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pDamageEffectPiercingSword 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_PIERCING_SWORD", GetPowerRate(), vDamageEffectPos, GetRotateDegree(), GetRotateDegree(), 0.f, false, -1.f, 1.f, m_fPiercingSwordProjectileRange );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( NULL != pEffect && GetEqippedSkillMemo( CX2SkillTree::SMI_ELSWORD_MEMO24 ) )
		{
			pEffect->GetDamageData().m_bIgnoreStateDefence = true;
		}
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hDamageEffectPiercingSword = ( pEffect != NULL ) ? pEffect->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if( m_hDamageEffectPiercingSword != INVALID_DAMAGE_EFFECT_HANDLE )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( m_pDamageEffectPiercingSword != NULL )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimerOneshot( ( (m_fSkillCancelAfter - 0.3f) * m_fPiercingSwordProjectileRange ) + 0.3f ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( ( (m_fSkillCancelAfter - 0.3f) * m_fPiercingSwordProjectileRange ) + 0.3f ) == true && EventCheck( ( (m_fSkillCancelAfter - 0.3f) * m_fPiercingSwordProjectileRange ) + 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hDamageEffectPiercingSword );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	        g_pX2Game->GetDamageEffect()->DestroyInstance( m_pDamageEffectPiercingSword );
            m_pDamageEffectPiercingSword = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
	}

	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_SI_A_ETK_PIERCING_SWORD_End()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_hDamageEffectPiercingSword != INVALID_DAMAGE_EFFECT_HANDLE )
	    g_pX2Game->GetDamageEffect()->DestroyInstanceHandle( m_hDamageEffectPiercingSword );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	g_pX2Game->GetDamageEffect()->DestroyInstance( m_pDamageEffectPiercingSword );
    m_pDamageEffectPiercingSword = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CommonStateEnd();
}


//-----------------------ESSI_SI_A_EIS_FATAL_SLAP-----------------------//
void CX2GUElsword_SwordMan::ESSI_SI_A_EIS_FATAL_SLAP_Init()
{
	XSkinMeshReadyInBackground( L"Elsword_Sword_Wave_Slash.X" );
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.x" );
	TextureReadyInBackground( L"ColorBallFire.dds" );
	TextureReadyInBackground( L"Condense_Pulse02.dds" );
	TextureReadyInBackground( L"Mesh_Raven_Event_AC_Upbody21_Effect.tga" );
}
void CX2GUElsword_SwordMan::ESSI_SI_A_EIS_FATAL_SLAP_EventProcess()
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


//-----------------------ESSI_A_ETK_SWORD_SHIELD-----------------------//
void CX2GUElsword_SwordMan::ESSI_A_ETK_SWORD_SHIELD_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50_Round.X" );
	TextureReadyInBackground( L"A_ETK_SWORD_SHIELD.dds" );
}
void CX2GUElsword_SwordMan::ESSI_A_ETK_SWORD_SHIELD_StateStart()
{
	g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectSwordShield );
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ETK_SWORD_SHIELD );
	if( NULL != pSkillTemplet )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
			return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
		m_fSwordShieldEffectiveTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
		m_fSwordShieldEffectiveTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

	CommonStateStart();
}
void CX2GUElsword_SwordMan::ESSI_A_ETK_SWORD_SHIELD_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.68f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.68f ) == true && EventCheck( 0.68f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		m_hEffectSwordShield = g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_A_ETK_SWORD_SHIELD", this, NULL, false, -1.f, m_fSwordShieldEffectiveTime );
	}

	CommonFrameMove();
}
void CX2GUElsword_SwordMan::ESSI_A_ETK_SWORD_SHIELD_EventProcess()
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


//-----------------------ESSI_A_ESK_WEAPON_BREAK-----------------------//
void CX2GUElsword_SwordMan::ESSI_A_ESK_WEAPON_BREAK_Init()
{
	TextureReadyInBackground( L"Break_Impact01.dds" );
}
#ifndef ADD_MEMO_1ST_CLASS //김창한
void CX2GUElsword_SwordMan::ESSI_A_ESK_WEAPON_BREAK_StateStart()
{
	CommonStateStart();

	const CX2SkillTree::SkillTemplet* pSkillTemplet 
		= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ESK_WEAPON_BREAK );

	if ( NULL != pSkillTemplet && pSkillTemplet->m_vecBuffFactorPtr.size() > 0 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( NULL == GetUnit() )
			return;

		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );	
#else //UPGRADE_SKILL_SYSTEM_2013
		m_DamageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
}
#endif //ADD_MEMO_1ST_CLASS
void CX2GUElsword_SwordMan::ESSI_A_ESK_WEAPON_BREAK_EventProcess()
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

#endif //UPGRADE_SKILL_SYSTEM_2013
#ifdef ELSWORD_WAY_OF_SWORD
/** @function : GetFinalDamage_WayOfSword
	@brief : -GetFinalDamage 계산 할 때 검의길 타입에 따라 데미지 변경 시켜 주기 위한 함수
			 -엘소드와 엘리시스가 다른 동작을 할 수 있도록 virtual 함수로 변경
*/
/*virtual*/float CX2GUElsword_SwordMan::GetFinalDamage_WayOfSword( CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ )
{
	float fDamageMagnifications = 1.f;
	if( -2 == GetWayOfSwordState() )
	{
		switch( eWayOfSwordType_ ) 
		{
		case CX2DamageManager::WST_DESTRUCTION_NORMAL:
			{
				fDamageMagnifications = 2.f;
			} break;
		case CX2DamageManager::WST_DESTRUCTION_ACTIVE:
		case CX2DamageManager::WST_DESTRUCTION_SPECIAL:
			{
				fDamageMagnifications = 1.5f;
			} break;
		default:
			break;
		}
	}

	return fDamageMagnifications;
}
#endif //ELSWORD_WAY_OF_SWORD

#ifdef FINALITY_SKILL_SYSTEM //JHKang
#pragma region SI_FS_ELK_Gigantic_Slash
void CX2GUElsword_SwordMan::ESSI_HA_ELK_Gigantic_Slash_Init()
{
	TextureReadyInBackground(L"Arme_Critical2.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"COMET_CRASHER_Explosion_02.tga");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	TextureReadyInBackground(L"Mesh_Raven_Event_AC_Upbody21_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	XSkinMeshReadyInBackground(L"UnoHound_AttackB_Mesh01.X");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
}
#pragma endregion 기간틱 슬래시 : 궁극기

#pragma region SI_FS_ERS_Shining_Rune_Buster
void CX2GUElsword_SwordMan::ESSI_HA_ERS_Shining_Rune_Buster_Init()
{
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	XSkinMeshReadyInBackground(L"ERS_Luna_Buster_Dummy01.X");
	XSkinMeshReadyInBackground(L"Elsword_RS_DashJumpComboXa_Mesh01_2.X");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"LUNA_BLADE_Rune_02.X");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"RTW_Flame_Buster.dds");
	TextureReadyInBackground(L"Riding_Ancient_Pporu_Fireball02A.dds");
	XSkinMeshReadyInBackground(L"SI_SA_ERS_LUNA_BLADE_BLADE_02.X");
	TextureReadyInBackground(L"WhiteImage.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"elsword_Rune_Buster_Deco01.dds");
	TextureReadyInBackground(L"secretVelder_alchemystBoss_specialAttackA_outCircle_top.dds");
	TextureReadyInBackground(L"state_potion_flare.DDS");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
}

void CX2GUElsword_SwordMan::ESSI_HA_ERS_Shining_Rune_Buster_EventProcess()
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
#pragma endregion 샤이닝 룬 버스트 : 궁극기



#pragma region SI_FS_EIS_Blade_Rain
void CX2GUElsword_SwordMan::ESSI_HA_EIS_Blade_Rain_Init()
{
	TextureReadyInBackground(L"Arme_Critical2.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	XSkinMeshReadyInBackground(L"DummyAttackBox_50x50x50.X");
	TextureReadyInBackground(L"Explosion_Fire01.dds");
	TextureReadyInBackground(L"Mesh_Elsword_Event_AC_Upbody21A_Effect.tga");
	TextureReadyInBackground(L"Particle_Blur.dds");
	XSkinMeshReadyInBackground(L"SI_SA_EIS_Blade_Rain_Sheath.X");
	TextureReadyInBackground(L"cube_light_P02.dds");
	TextureReadyInBackground(L"elsword_active_crecentCut_mark.DDS");
	TextureReadyInBackground(L"elswrod_demolition_edge.dds");
	TextureReadyInBackground(L"title_StarLight_Smash_spark.dds");
	TextureReadyInBackground(L"ColorBallGray.dds");
	XSkinMeshReadyInBackground(L"Land_Dimolisher_M01.X");
	TextureReadyInBackground(L"aisha_active_heavyPress_downCircle.dds");
}


void CX2GUElsword_SwordMan::ESSI_HA_EIS_Blade_Rain_EventProcess()
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

#pragma endregion 블레이드 레인 : 궁극기
#endif //FINALITY_SKILL_SYSTEM

/*virtual*/ void CX2GUElsword_SwordMan::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ /*= false*/ )
{
	ShowActiveSkillCutInAndLight( fTimeToShow_, m_iSkillCutInSetSubIndex, bOnlyLight_ );
}
#ifdef MODIFY_RIDING_PET_AWAKE
void CX2GUElsword_SwordMan::RidingHyperModeFrameMove()
{
	CommonHyperModeFrameMove( 0.f, 0.1f, true );
}
void CX2GUElsword_SwordMan::CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ /*= false*/  )
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
		if( true == bSound_ )
		{
			PlaySound( L"Stickslash1.ogg" );
		}
	}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTime2_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( (m_pXSkinAnim->EventTimer( fTime2_ ) == true && EventCheck( fTime2_, false) == true ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 30.0f, 0.4f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();

		if( true == bSound_ )
		{
			PlaySound( L"Slash.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}
	}
	CommonFrameMove();
}
#endif // MODIFY_RIDING_PET_AWAKE

#ifdef BALANCE_PATCH_20131107
void CX2GUElsword_SwordMan::SetSpecificValueByEnchant()
{
	//역전의 인내를 배웠다면 자신에게 걸린 무기 속성을 체크해서 자신에게 버프를 건다.
	if( m_iSkillLevelInduranceOfRevenge > 0 )
	{
		if( CX2DamageManager::EDT_NONE != GetApplyWeaponExtraDamageType() && NULL != g_pData->GetSkillTree() )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ELK_INDURANCE_OF_REVENGE );
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}
}
#endif //BALANCE_PATCH_20131107

/*virtual*/ float CX2GUElsword_SwordMan::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );


	switch( eSkillID_ )
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
	case CX2SkillTree::SI_A_ERS_SWORD_ENCHANT:
		{
			if( GetEqippedSkillMemo(CX2SkillTree::SMI_ELSWORD_MEMO19) == true )
				fMpConsumption -= 5.f;
		} break;
#endif //ADD_MEMO_1ST_CLASS

	}

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
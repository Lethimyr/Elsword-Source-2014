
#include "StdAfx.h"
#include ".\x2gameunit.h"

const double CX2GameUnit::RANDOM_SEED_RESET_TIME = 0.3;




CX2GameUnit::CX2GameUnit( int unitIndex, int teamNum, const WCHAR* pScriptFileName )
//{{ robobeg : 2008-10-28
: 
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
m_LuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true ),
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
//}} robobeg : 2008-10-28
m_fNoSkillTime( 0 )
#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
, m_pUnitSlashTraceManager( NULL )
#endif UNIT_SLASH_TRACE_MANAGER_TEST
//#ifdef HEAD_INVERSE_KINEMATICS_TEST
//, m_bEnableHeadIK( false )
//, m_fHeadIKAngleChangeRate( 0.f )
//, m_fHeadIKAngleMax( 0.f )
//, m_fHeadIKAngle( 0.f )
//, m_fHeadIKRadius( 0.f )
//#endif HEAD_INVERSE_KINEMATICS_TEST
//#ifdef STEP_ON_MONSTER_TEST
//, m_bCanBeSteppedOn( false )
//#endif STEP_ON_MONSTER_TEST

//#ifdef STEP_ON_MONSTER_COLLISION_BOX_TEST
//, m_bCanBeSteppedOnCollsionBox( false )
//#endif STEP_ON_MONSTER_COLLISION_BOX_TEST

//#ifdef HEAD_ATTACHED_CAMERA_TEST
//, m_vBoneAttachedCameraOffset( 0, 0, 0)
//#endif HEAD_ATTACHED_CAMERA_TEST


#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
, m_vLinkJumpTarget( 0, 0, 0 )
, m_vLinkJumpSpeed( 0, 0, 0 )
, m_vLinkJumpAcceleration( 0, 0, 0 )
, m_fLinkJumpTime( 0.f )
, m_fElapsedTimeAfterLastLinkJump( 0.f )
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST

, m_fLastPositionValueOnAnimatedLine( -1.f )
, m_vLastPositionOnAnimatedLine( 0, 0, 0 )

#ifdef RESET_INVALID_UNIT_POSITION_TEST
, m_bMisplaced( false )
#endif RESET_INVALID_UNIT_POSITION_TEST

#ifdef OVERKILL_TEST
, m_bOverKilled( false )
#endif OVERKILL_TEST

#ifdef REVERSE_GRAVITY_TEST
, m_fReverseGravitySpeedDeltaY( 0.f )
, m_vReverseGravityRotateSpeedDelta( D3DXVECTOR3(0.f, 0.f, 0.f) ) // oasis907 : 김상윤 [2011.6.29]
#endif REVERSE_GRAVITY_TEST

#ifdef BOUNDING_COMBO_TEST
, m_bDamageBoundingReserved( false ) 
, m_fDamageBoundingSpeed( 0.f )
#endif BOUNDING_COMBO_TEST
, m_bImmuneToExtraDamage( false )
, m_vBasicScale( 1.0f, 1.0f, 1.0f )
, m_vTransScale( m_vBasicScale )
, m_vOrgUnitSize( 1.0f, 1.0f )

//#ifdef TRANSFORMER_TEST
//, m_bIsTransformed( false )
//, m_TransformerOwnerUID( -1 )
//#endif TRANSFORMER_TEST
//{{ kimhc // 2010.2.17 //
#ifdef	APPLY_MOTION_OFFSET
, m_bApplyMotionOffset( true )
#endif	APPLY_MOTION_OFFSET
//}} kimhc // 2010.2.17 //
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef SKILL_BALANCE_PATCH
, m_bCanPassUnit( false )
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef SERV_SKILL_NOTE
, m_fRevengeDefenceTime(0.f)
, m_fEnduranceAttackTime(0.f)
, m_fPoisonDeBuff(0.f)
, m_fMaxHyperModeTime( 0.0f )
//, m_fRemainHyperModeTime(  0.0f )
#endif

//{{ JHKang / 강정훈 / 2010/11/29
#ifdef NEW_SKILL_2010_11
, m_fAtomicShieldDamageRate ( 1.0f )
#endif NEW_SKILL_2010_11
//}} JHKang / 강정훈 / 2010/11/29
, m_bCheckChangeUnderWater(false)
, m_bUnderWaterHead(false) 
, m_bChangeUnderWater(false)
, m_bForceChagneColor(false)
, m_cLineUnitColor( D3DXCOLOR(0.f, 0.f, 0.f, 0.f) )
//{{ JHKang / 강정훈 / 2011/01/13
#ifdef SEASON3_MONSTER_2010_12
, m_BasicRenderType( CKTDGXRenderer::RT_CARTOON_BLACK_EDGE )
#endif SEASON3_MONSTER_2010_12
//}} JHKang / 강정훈 / 2011/01/13
, m_bScaleEffectSetByNpc( false )
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//{{ kimhc // 2011.1.21 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
, m_eHittedTypeAtState( CX2DamageManager::HTD_NO_SOUND )	// 스테이트 별 HittedType 지정
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // 청 1차 전직
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef BALANCE_PATCH_20110303
, m_fMaxDownForce(-9999.f)
#endif
//{{ 2011-04 에 패치될 청 메모 헤드샷
#ifdef	CHUNG_MEMO_01				
, m_bHittedExtraDamageHeadShot( false )	// 헤드샷 ExtraDamage에 당했는지를 알려주는 플래그
#endif	CHUNG_MEMO_01				
//}} 2011-04 에 패치될 청 메모 헤드샷
#ifdef CHUNG_SECOND_CLASS_CHANGE
, m_fAntiCriticalRate( 0.f )
, m_bEnableWeaponRenderEffect( false )
, m_fFadeTimeLeft( 0.f )
, m_fFadeChangeTime( 0.f )
, m_fFadeChangeTimeLeft( 0.f )
, m_fPulseDuration( 0.f )
, m_fPulsePeriod( 0.f )
, m_fPulseTimeLeft( 0.f )
, m_eNowPulseWeaponColorID( PWCI_NONE )
, m_WeaponRenderEffectColor( D3DXCOLOR( 0.f, 0.f, 0.f, 0.f ) )
, m_WeaponRenderEffectColorStart( D3DXCOLOR( 0.f, 0.f, 0.f, 0.f ) )
, m_WeaponRenderEffectColorEnd( D3DXCOLOR( 0.f, 0.f, 0.f, 0.f ) )
#endif CHUNG_SECOND_CLASS_CHANGE

#ifdef SERV_TRAPPING_RANGER_TEST
, m_eAllyTeam( CX2Room::TN_NONE )
#endif SERV_TRAPPING_RANGER_TEST

, m_optrGameUnitWhoAttackedMe()
, m_optrGameUnitAttackedByMe()
, m_bNeedToUpdateWeaponEffect(false)
, m_bShowSmallGageAndName( false )
, m_pGageUI( NULL )
, m_pGageData( NULL )
, m_iDieCount(0)
, m_pFrame_Bip01( NULL )
, m_pFrame_Bip01_Head( NULL )
, m_pFrame_Bip01_R_Foot( NULL )
, m_pFrame_Bip01_L_Foot( NULL )
, m_pFrame_Bip01_R_UpperArm( NULL )
, m_pFrame_Bip01_L_UpperArm( NULL )

, m_fForceHyperModeTime( 0.0f )
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//, m_dwFrameMoveCountNow( 0 )
//, m_dwFrameMoveCountFuture( 0 )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
, m_ucHittedCount( 0 )
, m_ucHitCount( 0 )
, m_ucNumOfDeBuff( 0 )
, m_ucNumOfBuff( 0 )
, m_eUnitSlashTraceConditionToDo( CX2UnitSlashTraceManager::STC_NONE )
, m_pAfterImage( NULL )

#ifdef SERV_ARME_DIMENSION_WITCH
, m_fHitCreateDamageEffectRate ( 0.f )
, m_fHitCreateDamageEffectPowerRate(1.f)
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
, m_hSeqEnchant_ArrowOfExplosion( INVALID_PARTICLE_SEQUENCE_HANDLE )			/// 폭발의 화살 이모티콘 핸들
, m_hStartOfDelayedFiringOverlap( INVALID_PARTICLE_SEQUENCE_HANDLE )			/// 지연의 신호탄 중첩수 출력 이모티콘
, m_hActiveStartOfDelayedFiringEDT( INVALID_EFFECTSET_HANDLE )	/// 지연의 신호탄 적용중 확인 이펙트
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_CHUNG_TACTICAL_TROOPER
, m_fTacticalFieldDecreaseDamageValue( 1.f )						/// 택티컬 필드에 의해 감소되는 데미지 수치
#endif SERV_CHUNG_TACTICAL_TROOPER
, m_fRunJumpSpeedRate( 1.0f )
#ifdef RIDING_SYSTEM
, m_bPassDash( false )
#endif //RIDING_SYSTEM
#ifndef X2OPTIMIZE_NPC_NONHOST_SIMULATION
, m_fCommonDamageChangeTime ( -1.f )
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef MODIFY_DUNGEON_STAGING
, m_bCanNotInput( false )
#endif //MODIFY_DUNGEON_STAGING
//{{ robobeg : 2013-11-04
, m_Stat()
//}} robobeg : 2013-11-04
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
, m_fFlickerElapsedTime( 0.f )
#endif //SERV_ADD_LUNATIC_PSYKER
{
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    m_bPositionBackup = false;
    m_vPositionBackup = D3DXVECTOR3(0,0,0);
    m_iPositionBackup_LineIndex = -1;
    m_bPositionBackup_FootOnLine = false;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
	SetLayer( XL_UNIT_0 );
	SetAlphaObject( false );
	
	m_UnitIndex					= unitIndex;
	m_TeamNum					= teamNum;
//#ifdef PVP_BOSS_COMBAT_TEST
//	m_TeamNumOriginal			= teamNum;
//#endif PVP_BOSS_COMBAT_TEST

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	if ( pScriptFileName != NULL && pScriptFileName[0] != NULL )
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	{
		m_ScriptFileName			= pScriptFileName;
	}
	m_UnitUID					= 0;

//{{ robobeg : 2008-10-21
	//m_bShow						= true;
//}} robobeg : 2008-10-21


	m_fTime						= 0.0f;
	m_fElapsedTime				= 0.0f;	

	

	m_hHyperBoostRFoot			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hHyperBoostLFoot			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hHyperBoostRArm			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hHyperBoostLArm			= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hQuestionMark				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hExclamationMark			= INVALID_PARTICLE_SEQUENCE_HANDLE;


	m_bInvisibility				= false;
	m_bAbsoluteInvisibility		= false;

	//유닛 컴포넌트
	m_pUnitShadow				= NULL;
	m_fRevengeAddDamage			= 0.0f;
	m_DamageReducePercent		= 0;
    m_DamageGuardPercent        = 0;

	m_pComboManager				= new CX2ComboManager(1.3f);
	//m_pStat						= new CX2Stat();

	m_DamageLightTime			= 0.0f;
	m_bAttackedByUserUnit		= true;
	//m_AttackerUnitUID			= -1;

// #ifdef SERV_PET_SYSTEM
// 	m_LastAttackUnitUID			= -1;
// #endif

	ResetGameUnitWhoAttackedMe();
	ResetGameUnitAttackedByMe();
	
	m_HeadBoneName				= L"Bip01_Head";
	m_fDieAfterTime				= 0.0f;
	m_bDyingStart				= false;
	m_fForceInvincibleTime		= 0.0f;
	m_fShowInvincibleTime		= 0.0f;
	m_fShowInvincibleSwapTime	= 0.05f;
	m_bShowInvincibleWhite		= false;

	m_fShadowSize				= 0.0f;
	m_ShadowFileName			= L"";
	m_bUpMPThisFrame			= false;
	m_fCanNotAttackTime			= 0.0f;
	m_fCanNotJumpTime			= 0.0f;
	m_fCanNotInputTime			= 0.0f;
	m_fCanNotInputTimeZXArrow	= 0.f;


	m_HittedType				= CX2DamageManager::HTD_DEFAULT;
	m_FlyCount					= 0;


	m_fGroggyTime					= 0.0f;

	m_fExtraBigHead				= 0.f;

	m_fScale					= 1.0f;
	m_bUseWorldColor			= true;

	m_hSeqPoisonBody				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqFireBody1					= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqFireBody2					= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCurseBody					= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqSlowBodyRight				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqSlowBodyLeft				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCold1						= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCold2						= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCold1_Right				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCold2_Right				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCold_Head					= INVALID_PARTICLE_SEQUENCE_HANDLE;
	
	m_hSeqPepperBody1				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqPepperBody2				= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hSeqStun						= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hSeqHPUp						= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPUp						= INVALID_PARTICLE_SEQUENCE_HANDLE;

	
	m_hSeqEnchant_Stun				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnchant_Exclamation		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnchant_Exclamation2		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnchant_Exclamation3		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnchant_Blaze_Second		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnchant_Frozen_Second		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnchant_Poison_Second		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnchant_Stun_Second		= INVALID_PARTICLE_SEQUENCE_HANDLE;






	m_hSeqBleeding		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqSkeletonHead	= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqStunAbnormality = INVALID_PARTICLE_SEQUENCE_HANDLE;


	m_hSeqAging			= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hStigma = INVALID_PARTICLE_SEQUENCE_HANDLE;

#ifdef SERV_SKILL_NOTE
	m_hRevengeDefence = INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hPoisonDeBuff = INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif

#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
	m_hSeqBlind			= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
	m_hSeqWaterHold1			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqWaterHold2			= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif EDT_WATER_HOLD_TEST

	//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
	m_hSeqPanic					= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_hSeqLegShot				= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
	m_hSeqEarthQuakeLeft		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEarthQuakeRight		= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

	m_hEffSetStateAbnormalityFrozen = INVALID_EFFECTSET_HANDLE;

#ifdef RAVEN_WEAPON_TAKER
	m_hSeqSmokeBlind			= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif RAVEN_WEAPON_TAKER

	m_hEffSetDryad			= INVALID_EFFECTSET_HANDLE;


	m_eWeaponEnchantExtraDamageType = CX2DamageManager::EDT_NONE;


	m_bIgnoreLineSpeed			= false;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_bDisableGravity			= false;
	m_bDisableGravityInScript	= false;
	m_vDisableGravityInScriptTime = D3DXVECTOR2( 0, 0 );
	m_vIgnoreLineTime = D3DXVECTOR2( 0, 0 );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	for( int i=0; i<ARRAY_SIZE(m_hMajorMeshPlayerList); ++i )
	{
		m_hMajorMeshPlayerList[i] = INVALID_MESH_INSTANCE_HANDLE;
	}

	for( int i=0; i<ARRAY_SIZE(m_hMinorMeshPlayerList); ++i )
	{
		m_hMinorMeshPlayerList[i] = INVALID_MESH_INSTANCE_HANDLE;
	}

	m_fBasicBoneScale			= 1.f;

	m_fTimeDirectToReturnBasicScale	 = 0.0f;
	m_fTimeToReturnBasicScale = 0.0f;


	m_bCheckWidthEnlargeSize	= true;
	m_bCheckHeightEnlargeSize	= true;

	m_bOneTimeInit				= false;



#ifdef FOG_WORLD
    m_bFog                          = false; // default Fog
#endif

	m_bWaitingForRebirthAfterBungee = false;

	m_vDisplaceUnitPosition = D3DXVECTOR3( 0, 0, 0 );
	m_vFixedUnitPosition = D3DXVECTOR3( 0, 0, 0 );


//#ifdef LINEMAP_FAST_WIND_TEST
//	m_bEnableUpsideWind	= true;
//	m_bWaitInTheAir		= false;
//	m_fTimeInTheAir		= 0.f;
//#endif LINEMAP_FAST_WIND_TEST
	
#ifndef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	m_bDoubleAttack = false;
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	m_wstrHitSound = L"";
	m_fFinalDamage = 0.f;
	m_fDoubleAttackTime = 0.f;	

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_bDeleteEffectSetOnStateEnd = false;
	m_bDeleteEffectSetOnDamageReact = false;
	m_bDeleteEffectSetOnDie = false;
	m_bHyperEffectSet = false;
	m_fEffectSetLifeTime = 0.f;
#ifdef ADDITIONAL_MEMO
    m_iMemoId = 0;

	m_bIsCustomStateDeleteEffectOnDamageReact = false;
#endif // CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT

#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
	m_bDeleteEffectSetOnCustomState = false;
#endif

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	InitEnableDash();
	m_bEntangled = false;
	
	m_bHitedDrainMana = false;

#ifdef RAVEN_SECOND_CLASS_CHANGE
	m_bChangeDamageType = false;;
	m_eDamageType		= CX2DamageManager::DT_PHYSIC;
	m_fDamageRel		= 1.f;
	m_iDamageTimes		= -1;
#endif

#ifdef DAMAGE_HISTORY
	m_bDamageHistory = false;
	m_fDamageHistory = NULL;
	m_fDamageHistory = new float(0.f);
#endif


#ifdef CHUNG_SECOND_CLASS_CHANGE
	// oasis907 : 김상윤 [2011.6.20] 청 2차 전직 - 아이언 팔라딘
	m_ExtraDamagePack.Init();
#endif CHUNG_SECOND_CLASS_CHANGE


	//{{ kimhc // 2010.4.6 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
	// 나중에 ExtraDamagePack을 map으로 바꿀때 같이 꼭!! 바꿔야함
	for( int i = 0; i < NUM_OF_EXTRA_DAMAGE_PACK_MEMBER; ++i )
		m_pExtraDamageDataList[i] = 0;

	m_pExtraDamageDataList[0] = &m_ExtraDamagePack.m_Poison;			  m_pExtraDamageDataList[1] = &m_ExtraDamagePack.m_Fire;			m_pExtraDamageDataList[2] = &m_ExtraDamagePack.m_Curse;
	m_pExtraDamageDataList[3] = &m_ExtraDamagePack.m_Slow;				  m_pExtraDamageDataList[4] = &m_ExtraDamagePack.m_Lightning;		m_pExtraDamageDataList[5] = &m_ExtraDamagePack.m_ReverseLeftRight;
	m_pExtraDamageDataList[6] = &m_ExtraDamagePack.m_Cold;				  m_pExtraDamageDataList[7] = &m_ExtraDamagePack.m_Y_Pressed;		m_pExtraDamageDataList[8] = &m_ExtraDamagePack.m_Frozen;
	m_pExtraDamageDataList[9] = &m_ExtraDamagePack.m_LegWound;			  m_pExtraDamageDataList[10] = &m_ExtraDamagePack.m_WeaponBreak;	m_pExtraDamageDataList[11] = &m_ExtraDamagePack.m_ArmorBreak;
	m_pExtraDamageDataList[12] = &m_ExtraDamagePack.m_Entangled;		  m_pExtraDamageDataList[13] = &m_ExtraDamagePack.m_Dull;			m_pExtraDamageDataList[14] = &m_ExtraDamagePack.m_TimeBomb;
	m_pExtraDamageDataList[15] = &m_ExtraDamagePack.m_Stun;				  m_pExtraDamageDataList[16] = &m_ExtraDamagePack.m_Aging;			m_pExtraDamageDataList[17] = &m_ExtraDamagePack.m_EnchantBlaze;
	m_pExtraDamageDataList[18] = &m_ExtraDamagePack.m_EnchantFrozen;	  m_pExtraDamageDataList[19] = &m_ExtraDamagePack.m_EnchantPoison;	m_pExtraDamageDataList[20] = &m_ExtraDamagePack.m_EnchantShock;
	m_pExtraDamageDataList[21] = &m_ExtraDamagePack.m_MagicDefenceDebuff; m_pExtraDamageDataList[22] = &m_ExtraDamagePack.m_StigmaDebuff;	m_pExtraDamageDataList[23] = &m_ExtraDamagePack.m_Dryad;
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
	m_pExtraDamageDataList[24] = &m_ExtraDamagePack.m_Blind;
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
	m_pExtraDamageDataList[25] = &m_ExtraDamagePack.m_WaterHold;
#endif EDT_WATER_HOLD_TEST

	//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
	m_pExtraDamageDataList[26] = &m_ExtraDamagePack.m_Panic;
	m_pExtraDamageDataList[27] = &m_ExtraDamagePack.m_Pain;
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_pExtraDamageDataList[28] = &m_ExtraDamagePack.m_LegShot;
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
	m_pExtraDamageDataList[29] = &m_ExtraDamagePack.m_EarthQuake;
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE
#ifdef RAVEN_WEAPON_TAKER
	m_pExtraDamageDataList[30] = &m_ExtraDamagePack.m_SmokeBlind;
#endif RAVEN_WEAPON_TAKER
#ifdef EVE_ELECTRA
	m_pExtraDamageDataList[31] = &m_ExtraDamagePack.m_FlashBang;
#endif EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	m_pExtraDamageDataList[32] = &m_ExtraDamagePack.m_MarkOfCommander;
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN
#ifdef SERV_RENA_NIGHT_WATCHER
	m_pExtraDamageDataList[33] = &m_ExtraDamagePack.m_StartOfDelayedFiring;
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	m_pExtraDamageDataList[34] = &m_ExtraDamagePack.m_Bleeding;
#endif SERV_RAVEN_VETERAN_COMMANDER

#endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.6 // 비밀던전 작업(셋트아이템효과)


#ifdef SERV_PVP_NEW_SYSTEM
	m_bPvpBot = false;
#endif

#ifdef NEW_HENIR_TEST

#ifndef ADD_HENIR_BUFF		/// 버프로 변경되면서 쓰이지 않게 된 구문
	m_fTimeHenirBuff_Fire		= 0.f;
	m_fTimeHenirBuff_Water		= 0.f;
	m_fTimeHenirBuff_Nature		= 0.f;
	m_fTimeHenirBuff_Wind		= 0.f;
	m_fTimeHenirBuff_Light		= 0.f;
#endif ADD_HENIR_BUFF

#endif NEW_HENIR_TEST

//#ifdef SECRET_VELDER
//	m_bGlobalSuperArmor	= false;
//	m_fGlobalTimeSuperArmor	= 0.f;
//#endif

	m_GameUnitType = GUT_USER;
#ifdef SERV_SECRET_HELL
	m_StatUpType = CX2SocketItem::SUT_NONE;
#endif

	m_bScaleEffectSetByNpc = false;
#ifdef SPECIAL_USE_ITEM
	m_fSpecialItemBuff_Wind = 0.f;
#endif

#ifdef SERV_DROP_FOR_FINISHER_ONLY
	m_iFinisherUID = 0;
#endif SERV_DROP_FOR_FINISHER_ONLY
}

CX2GameUnit::~CX2GameUnit(void)
{

	//FinishAndClearAllBuff();	/// 버프 종료 및 삭제

#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
	if( NULL != m_pUnitSlashTraceManager )
	{
		CX2UnitSlashTraceManager::DeleteUnitSlashTraceManager( m_pUnitSlashTraceManager );
	}
#endif UNIT_SLASH_TRACE_MANAGER_TEST



#ifdef CLOSE_SOUND_TEST
	m_SoundCloseManager.CloseSound();
#endif CLOSE_SOUND_TEST



	DeleteEffectSetOnDie();
	


	DeleteGameUnitMajorParticle();
	DeleteGameUnitMinorParticle();



	for( UINT i=0; i<m_vecpWeapon.size(); ++i )
	{
		Weapon* pWeapon = m_vecpWeapon[i];
		SAFE_DELETE_KTDGOBJECT( pWeapon );
	}
	m_vecpWeapon.clear();


	SAFE_DELETE_KTDGOBJECT( m_pUnitShadow );

		 
	m_CollisionListSet.clear();
	m_AttackListSet.clear();
	m_SubAttackListSet.clear();
	m_HitUnitList.clear();

	SAFE_DELETE( m_pComboManager );
	//SAFE_DELETE( m_pStat );
	m_DamagedMap.clear();




	for( int i=0; i<ARRAY_SIZE(m_hMajorMeshPlayerList); ++i )
	{
		if( m_hMajorMeshPlayerList[i] != INVALID_MESH_INSTANCE_HANDLE )
		{
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMajorMeshPlayerList[i] );
		}
		m_hMajorMeshPlayerList[i] = INVALID_MESH_INSTANCE_HANDLE;
	}

	for( int i=0; i<ARRAY_SIZE(m_hMinorMeshPlayerList); ++i )
	{
		if( m_hMinorMeshPlayerList[i] != INVALID_MESH_INSTANCE_HANDLE )
		{
			g_pX2Game->GetMinorXMeshPlayer()->DestroyInstanceHandle( m_hMinorMeshPlayerList[i] );
		}
		m_hMinorMeshPlayerList[i] = INVALID_MESH_INSTANCE_HANDLE;
	}


#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	for( int i =0; i < (int)m_ParticleEffData.size(); ++i )
	{
		ParticleEffectData* pParticleEffectData = m_ParticleEffData[i];
		SAFE_DELETE( pParticleEffectData );
	}
	m_ParticleEffData.clear();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef DAMAGE_HISTORY
	SAFE_DELETE( m_fDamageHistory );
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
	ClearStartOfDelayedFiringData();

	g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiringEDT );	/// 지연의 신호탄 적용중 확인 이펙트
#endif SERV_RENA_NIGHT_WATCHER

	SAFE_DELETE( m_pGageData );
	SAFE_DELETE( m_pGageUI );
	SAFE_DELETE_KTDGOBJECT( m_pAfterImage );

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE		/// 데미지 그룹 데이터 해제
	m_vecDamageEffectGroupDataPtr.resize(0);
#endif DAMAGE_EFFECT_GROUP_DAMAGE
}


void CX2GameUnit::ReInit( bool bUseTeamPos, int startPosIndex )
{
	SetLayer( XL_UNIT_0 );
	SetAlphaObject( false );

	SetNumOfDeBuff( 0 );
	SetNumOfBuff( 0 );
	m_vecBuffTempletPtr.resize( 0 );

	m_fTime						= 0.0f;
	m_fElapsedTime				= 0.0f;	


	SetRemainHyperModeTime( 0.0f );
	m_PhysicParam				= m_OrgPhysicParam;
#ifdef DEEP_WATER_DUNGEON_TEST
	ResetPhysicParam();
#endif DEEP_WATER_DUNGEON_TEST

	
	//유닛 컴포넌트
	m_DamageData.Init();
	m_DamageData.m_SocketExtraDamage	= m_SocketData.m_SocketExtraDamage;
	m_DamageData.m_MultipleExtraDamage	= m_SocketData.m_MultipleSocketExtraDamage;



	m_ExtraDamagePack.Init();

	m_fRevengeAddDamage			= 0.0f;	

	m_RenderParam.Init();

	m_DamageLightTime			= 0.0f;
	m_bAttackedByUserUnit		= true;
// 	m_AttackerUnitUID			= -1;
// 
// #ifdef SERV_PET_SYSTEM
// 	m_LastAttackUnitUID			= -1;
// #endif

	m_HeadBoneName				= L"Bip01_Head";
	m_fDieAfterTime				= 0.0f;
	m_bDyingStart				= false;
	m_fForceInvincibleTime		= 0.0f;
	m_fShowInvincibleTime		= 0.0f;
	m_fShowInvincibleSwapTime	= 0.05f;
	m_bShowInvincibleWhite		= false;


	m_bInvisibility				= false;
	m_bAbsoluteInvisibility		= false;
    

	m_fGroggyTime = 0.f;
    
	if( m_LuaManager.BeginTable( "INIT_SYSTEM" ) == true )
	{
		LUA_GET_VALUE_ENUM( m_LuaManager, "RENDER_PARAM", m_stOriginBuffRenderParam.m_eRenderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_CARTOON_BLACK_EDGE );
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		m_stOriginBuffRenderParam.m_fOutLineWide	= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		m_stOriginBuffRenderParam.m_fOutLineWide	= 1.7f;
#endif //UNIT_SCALE_COMBINE_ONE

		if( g_pX2Game->GetWorld() != NULL )
			m_RenderParam.lightPos	= g_pX2Game->GetWorld()->GetLightPos();

		//{{ JHKang / 강정훈 / 2011/01/13 / 전체 렌더 타입 변경 확인(유지할 렌더 타입이 지정)
#ifdef SEASON3_MONSTER_2010_12
		m_BasicRenderType = m_RenderParam.renderType;
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2011/01/13

		LUA_GET_VALUE( m_LuaManager, "ALPHA_BLEND", m_stOriginBuffRenderParam.m_bAlphaBlend, false );
		SetAlphaObject( m_stOriginBuffRenderParam.m_bAlphaBlend );

		bool bCullMode;
		LUA_GET_VALUE( m_LuaManager, "CULL_MODE", bCullMode, true );
		if( bCullMode == false )
		{
			m_RenderParam.cullMode = D3DCULL_NONE;
		}

		LUA_GET_VALUE( m_LuaManager, "Z_ENABLE", m_RenderParam.bZEnable, true );
		m_RenderParam.bZWriteEnable = m_RenderParam.bZEnable;			// fix!! 일단 요렇게

		m_LuaManager.EndTable();
	}


	if( m_LuaManager.BeginTable( "INIT_COMPONENT" ) == true )
	{
		int iBuffer = 0;
		float fBuffer = 0.0f;
		//LUA_GET_VALUE( m_LuaManager, L"HYPER_MODE_COUNT",		iBuffer,		1		);
		//m_FrameDataFuture.syncData.m_HyperModeCount = iBuffer;
		LUA_GET_VALUE( m_LuaManager, "MAX_HYPER_MODE_TIME",	fBuffer,	30.0f	);
		SetMaxHyperModeTime( fBuffer );

		m_LuaManager.EndTable();
	}

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE	
	m_AttackTimeList.resize(0);
	m_AttackProjSeqName.resize(0);
	m_StopAllList.resize(0);
	m_StopOtherList.resize(0);
	m_StopMyList.resize(0);
	m_Stop2AllList.resize(0);
	m_Stop2OtherList.resize(0);
	m_Stop2MyList.resize(0);
	m_vecSoundPlayData.resize(0);
	m_vecSoundPlayLoop.resize(0);
	m_vecLoopSound.resize(0);
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE



	m_bChangeUnderWater = false;
	m_bCheckChangeUnderWater = false;

#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_vecKDebuffFactorListToTransit.clear();
#endif CHUNG_SECOND_CLASS_CHANGE

//{{ robobeg : 2008-10-21
	//SetShow( true );
    SetShowObject( true );
//}} robobeg : 2008-10-21
}

void CX2GameUnit::InitEffect()
{
	//{{ dmlee 2009.1.19 파티클이 필요한 시점에 생성하도록 코드위치 옮김, 미리 생성 해두니까 performance에 부담이

	//if( m_hSeqPoisonBody == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqPoisonBody = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitPoison", 0,0,0 );


	//if( m_hSeqFireBody1 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqFireBody1 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"SwordFireTip", 0,0,0 );
	//if( m_hSeqFireBody2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqFireBody2 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"SwordFireTip", 0,0,0 );



	//if( m_hSeqCurseBody == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqCurseBody = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitCurse", 0,0,0 );


	//if( m_hSeqSlowBodyRight == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqSlowBodyRight = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitSlow", 0,0,0 );
	//if( m_hSeqSlowBodyLeft == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqSlowBodyLeft = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitSlow", 0,0,0 );



	//if( m_hSeqReverseLeftRight == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqReverseLeftRight = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitReverseLeftRight", 0, 0, 0 ); 


	//if( m_hSeqCold1 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqCold1 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Ice_Status_Error01", 0, 0, 0 );	
	//if( m_hSeqCold2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqCold2 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Ice_Status_Error02", 0, 0, 0 );	
	//if( m_hSeqCold1_Right == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqCold1_Right = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Ice_Status_Error01", 0, 0, 0 );	
	//if( m_hSeqCold2_Right == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqCold2_Right = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Ice_Status_Error02", 0, 0, 0 );	

	//if( m_hSeqCold_Head == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqCold_Head = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitCold", 0, 0, 0 );	


	//if( m_hSeqEmoticon_Pressed == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEmoticon_Pressed = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitPressed", 0, 0, 0 );	



	//if( m_hSeqPepperBody1 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqPepperBody1 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"SwordFireTip", 0,0,0 );
	//if( m_hSeqPepperBody2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqPepperBody2 = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"SwordFireTip", 0,0,0 );



	//if( m_hSeqStun == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqStun = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitStun", 0,0,0 );




	//if( m_hSeqEnchant_Stun == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Stun = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Stun_Emoticon", 0,0,0 );

	//if( m_hSeqEnchant_Stun_Second == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Stun_Second = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Stun_Emoticon_Second", 0,0,0 );


	//if( m_hSeqEnchant_Blaze_Second == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Blaze_Second = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Blaze_Emoticon_Second", 0,0,0 );

	//if( m_hSeqEnchant_Frozen_Second == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Frozen_Second = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Frozen_Emoticon_Second", 0,0,0 );

	//if( m_hSeqEnchant_Poison_Second == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Poison_Second = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Poison_Emoticon_Second", 0,0,0 );


	//if( m_hSeqEnchant_Exclamation == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Exclamation = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Exclamation_Emoticon", 0,0,0 );

	//if( m_hSeqEnchant_Exclamation2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Exclamation2 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Exclamation_Emoticon2", 0,0,0 );

	//if( m_hSeqEnchant_Exclamation3 == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqEnchant_Exclamation3 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Enchant_Exclamation_Emoticon3", 0,0,0 );


	//if ( m_hSeqAddOnStatDefPhysic == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqAddOnStatDefPhysic = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"BuffShield", 0,0,0, 0, 0 );

	//if ( m_hSeqAddOnStatAtkPhysic == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqAddOnStatAtkPhysic = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"BuffAttack", 0,0,0, 0, 0 );

	//if ( m_hSeqAddOnStatAtkMagic == INVALID_PARTICLE_SEQUENCE_HANDLE )
	//	m_hSeqAddOnStatAtkMagic = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"BuffAttack", 0,0,0, 0, 0 );
	//}} dmlee 2009.1.19 파티클이 필요한 시점에 생성하도록 코드위치 옮김, 미리 생성해두니까 performance에 부담이

}



void CX2GameUnit::DeleteGameUnitMajorParticle()
{
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostRFoot );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostLFoot );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostRArm );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostLArm );

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hQuestionMark );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hExclamationMark );


	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqHPUp );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqMPUp );


	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Stun );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Exclamation );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Exclamation2 );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Exclamation3 );

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Blaze_Second );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Frozen_Second );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Poison_Second );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqEnchant_Stun_Second );

#ifdef SKILL_30_TEST
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqBleeding );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqSkeletonHead );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqStunAbnormality );



	g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffSetStateAbnormalityFrozen );
#endif SKILL_30_TEST

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqAging );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hStigma );

	g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffSetDryad );

#ifdef SERV_SKILL_NOTE	
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hRevengeDefence );	
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hPoisonDeBuff );	
#endif

#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqBlind );	
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqWaterHold1 );	
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqWaterHold2 );	
#endif EDT_WATER_HOLD_TEST

	//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqPanic );	
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqLegShot );
#endif

#ifdef RAVEN_WEAPON_TAKER
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqSmokeBlind );
#endif RAVEN_WEAPON_TAKER

//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
#ifdef SERV_SECRET_HELL
	ClearAnimSpeedFactor();
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)

#ifdef HIT_PARTICLE_TRACE_UNIT
    BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequenceHandle hParticle, m_listSeqHitParticleTraceUnit )
	{
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( hParticle );
	}
    m_listSeqHitParticleTraceUnit.resize( 0 );
#endif HIT_PARTICLE_TRACE_UNIT


}

void CX2GameUnit::DeleteGameUnitMinorParticle()
{
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqPoisonBody );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqFireBody1 );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqFireBody2 );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqCurseBody );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqSlowBodyRight );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqSlowBodyLeft );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqCold1 );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqCold2 );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqCold1_Right );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqCold2_Right );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqCold_Head );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqPepperBody1 );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqPepperBody2 );

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqStun );
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqEarthQuakeLeft );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqEarthQuakeRight );
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

}





void CX2GameUnit::MultiplyStat( float fFactor )
{
    m_Stat.AccessStat().MultiplyStat( fFactor );
}




/*virtual*/ HRESULT CX2GameUnit::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fTime			= (float)fTime;
	m_fElapsedTime	= fElapsedTime;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    OnFrameMoveBuff( fElapsedTime );	/// 버프들의 FrameMove 실행
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	OnFrameMoveBuff();	/// 버프들의 FrameMove 실행
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	CheckAndUpdateAdditionalStat();		/// 스탯이 변경되어야하는지 체크하고 업데이트

#ifdef CHUNG_SECOND_CLASS_CHANGE
	// oasis907 : 김상윤 [2011.6.20] 청 2차 전직 - 아이언 팔라딘
	if ( !m_vecKDebuffFactorListToTransit.empty() )
	{
		BOOST_FOREACH( const KBuffFactor& kBuffFactor, m_vecKDebuffFactorListToTransit )
		{
			EraseBuffTempletFromGameUnit( static_cast<BUFF_TEMPLET_ID>( kBuffFactor.m_BuffIdentity.m_eBuffTempletID ) );
		}
		m_vecKDebuffFactorListToTransit.clear();
	}
#endif CHUNG_SECOND_CLASS_CHANGE


	m_PhysicParam.OnFrameMove( fTime, fElapsedTime );	

#ifdef SERV_SKILL_NOTE
	CommonFrameMoveAnimSpeedFactor();	
#endif
	CommonFrameMoveMoveSpeedFactor();
	CommonFrameMoveJumpSpeedFactor();

#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
	if( NULL != m_pUnitSlashTraceManager )
	{
		m_pUnitSlashTraceManager->OnFrameMove( m_fTime, m_fElapsedTime );
	}
#endif UNIT_SLASH_TRACE_MANAGER_TEST

	if( m_fNoSkillTime > 0.f )
	{
		m_fNoSkillTime -= fElapsedTime;
		if( m_fNoSkillTime < 0.f )
			m_fNoSkillTime = 0.f;
	}


#ifdef SERV_SKILL_NOTE	
	if( m_fEnduranceAttackTime > 0.f )
	{
		m_fEnduranceAttackTime -= m_fElapsedTime;
		if( m_fEnduranceAttackTime <= 0.f )
		{
			m_fEnduranceAttackTime = 0.f;
		}
	}

	CommonFrameMoveRevengeDefence();
	CommonFrameMovePoisonDeBuff();
#endif
	
//{{ kimhc // 2010.4.3 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
	if( m_StatUpBufferBySocketItem.m_fBufferTime > 0.f )
	{
		m_StatUpBufferBySocketItem.m_fBufferTime -= fElapsedTime;
		if( m_StatUpBufferBySocketItem.m_fBufferTime <= 0.f )
		{
			SetStatUpBufferBySocketItem( 0.0f, 0.0f );
		}
	}
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.3 // 비밀던전 작업(셋트아이템효과)

	if( m_fExtraBigHead > 0.f )
	{
		m_fExtraBigHead -= fElapsedTime;
		if( m_fExtraBigHead < 0.f )
		{
			m_fExtraBigHead = 0.f;
		}
	}
	DoScaleHeadBone();

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST

	if( CX2GameUnit::GUT_USER == GetGameUnitType() ) // 몬스터의 경우 스크립트에서 scaling을 조정한 것이 적용되는 문제가 있어서 임시로 user만 적용되도록 수정, 몬스터도 적용되게 하려면 몬스터 스크립트에서 사용하는 UNIT_SCALE변수를 따로 분리해서 처리해야함
	{
		SetScaleByUnit( m_vTransScale );
	}
	else
	{
		if( m_bScaleEffectSetByNpc == true )
			SetScaleByUnit( m_vTransScale );
	}
#endif GIANT_UNIT_GIANT_EFFECT_TEST

	if( CX2GameUnit::GUT_USER == GetGameUnitType() &&
		m_bChangeUnderWater == true && 
		g_pX2Game != NULL && g_pX2Game->GetEffectSet() != NULL )
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_UnderWater_Change", this );
		PlaySound( L"Water_Diving.ogg" );
	}

	//GetGageManager()->OnFrameMove( fTime, fElapsedTime );
	UpdateVisibilitySmallGageAndName();
	
	if ( false == IsNullGageUI() )
	{
		m_pGageUI->SetShow( GetShowSmallGageAndName() );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        m_pGageUI->OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_pGageUI->OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE		/// 그룹 데미지 정보 갱신
	if( false == m_vecDamageEffectGroupDataPtr.empty() )
	{
		ProcessGroupDamageData( m_fElapsedTime );
	}
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09
	if ( !m_vecEffectSetImpactPointByBuffPtr.empty() )	/// 타격시 이펙트 셋 생성시키는 버프에 대한 생성 간격 갱신
	{
		BOOST_FOREACH( CX2EffectSetImpactPointByBuffPtr ptr, m_vecEffectSetImpactPointByBuffPtr )
		{
			if ( NULL != ptr )
				ptr->ProcessCoolTime( m_fElapsedTime );
		}
	}

	if ( !m_vecEffectSetHittedByBuffPtr.empty() )		/// 피격시 이펙트 셋 생성시키는 버프에 대한 생성 간격 갱신
	{
		BOOST_FOREACH( CX2EffectSetImpactPointByBuffPtr ptr, m_vecEffectSetHittedByBuffPtr )
		{
			if ( NULL != ptr )
				ptr->ProcessCoolTime( m_fElapsedTime );
		}
	}

	if ( !m_vecEffectSetNormalAttackByBuffPtr.empty() )	/// 일반 공격 타격시 이펙트 셋 생성시키는 버프에 대한 생성 간격 갱신
	{
		BOOST_FOREACH( CX2EffectSetImpactPointByBuffPtr ptr, m_vecEffectSetNormalAttackByBuffPtr )
		{
			if ( NULL != ptr )
				ptr->ProcessCoolTime( m_fElapsedTime );
		}
	}
#endif // SERV_NEW_DEFENCE_DUNGEON

	return S_OK;
}



HRESULT CX2GameUnit::OnCameraMove()
{
	KTDXPROFILE();
	g_pX2Game->GetX2Camera()->NomalTrackingCamera( this, g_pMain->GetGameOption().GetCameraDistance() );
	return S_OK;
}

//{{ robobeg : 2008-10-21
//void CX2GameUnit::SetShow( bool bShow )
/*virtual*/
void CX2GameUnit::NotifyShowObjectChanged()
//}} robobeg : 2008-10-21
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq_RFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRFoot );
	CKTDGParticleSystem::CParticleEventSequence* pSeq_LFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLFoot );
	CKTDGParticleSystem::CParticleEventSequence* pSeq_RArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRArm );
	CKTDGParticleSystem::CParticleEventSequence* pSeq_LArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLArm );

//{{ robobeg : 2008-10-21

	//m_bShow = bShow;

    bool bShow = GetShowObject();

	//if( m_bShow == false )
    if ( bShow == false )
//}} robobeg : 2008-10-21
	{
		if( pSeq_RFoot != NULL )
			pSeq_RFoot->SetShowObject( false );
		if( pSeq_LFoot != NULL )
			pSeq_LFoot->SetShowObject( false );
		if( pSeq_RArm != NULL )
			pSeq_RArm->SetShowObject( false );
		if( pSeq_LArm != NULL )
			pSeq_LArm->SetShowObject( false );

		if( m_pUnitShadow != NULL )
			m_pUnitShadow->SetShowObject( false );

		if ( false == IsMyUnit() )
			SetShowGage( false );
	}
	else
	{
		if( pSeq_RFoot != NULL )
			pSeq_RFoot->SetShowObject( true );
		if( pSeq_LFoot != NULL )
			pSeq_LFoot->SetShowObject( true );
		if( pSeq_RArm != NULL )
			pSeq_RArm->SetShowObject( true );
		if( pSeq_LArm != NULL )
			pSeq_LArm->SetShowObject( true );

		if( m_pUnitShadow != NULL )
			m_pUnitShadow->SetShowObject( true );

		if ( false == IsMyUnit() )
			SetShowGage( true );
	}

	//{{ kimhc // 2010.6.16 // 해당 기능을 하는 부분을 virtual함수로 만들어서 GUUser의 NotifyShowObjectChanged에서 호출하도록 함
	//{{AFX
#ifndef	ENCHANT_BUG_TEST
	//{{ robobeg : 2008-10-28
	BOOST_TEST_FOREACH( CX2GameUnit::Weapon*, pWeapon, m_vecpWeapon )
	{
		if ( NULL != pWeapon )
			pWeapon->NotifyShowObjectChanged();
	}
	//}} robobeg : 2008-10-28
#endif	ENCHANT_BUG_TEST
	//}}AFX
	//}} kimhc // 2010.6.16 // 해당 기능을 하는 부분을 virtual함수로 만들어서 GUUser의 NotifyShowObjectChanged에서 호출하도록 함


#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
	if( NULL != m_pUnitSlashTraceManager )
	{
		m_pUnitSlashTraceManager->SetShow( bShow );
	}
#endif UNIT_SLASH_TRACE_MANAGER_TEST


}

void CX2GameUnit::SetShadow( const WCHAR* pTexName, float fSize )
{
	SAFE_DELETE_KTDGOBJECT( m_pUnitShadow );
	m_pUnitShadow = CKTDGPlanShadow::CreatePlanShadow( fSize );
	m_pUnitShadow->SetLayer( XL_EFFECT_0 );
	m_pUnitShadow->SetShadowTexture( pTexName );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitShadow );
}




float CX2GameUnit::GetSphereAttackBoxScale( const wstring& attackBoxName )
{
	KTDXPROFILE(); 


	BOOST_TEST_FOREACH( const CKTDXCollision::CollisionDataList*, pCollisionDataList, m_AttackListSet )
	{
		ASSERT( pCollisionDataList != NULL );
		BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, *pCollisionDataList )
		{
			ASSERT( pCollisionData != NULL );
			if( CKTDXCollision::CT_SPHERE == pCollisionData->m_CollisionType && 
				pCollisionData->m_Name.compare( attackBoxName ) == 0 )
			{
				return pCollisionData->m_fRadiusScale;
			}
		}
	}

	return 1.f;

}


void CX2GameUnit::SetSphereAttackBoxScale( const wstring& attackBoxName, float fScale )
{
	KTDXPROFILE(); 


	BOOST_TEST_FOREACH( const CKTDXCollision::CollisionDataList*, pCollisionDataList, m_AttackListSet )
	{
		ASSERT( pCollisionDataList != NULL );
		BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, *pCollisionDataList )
		{
			ASSERT( pCollisionData != NULL );
			if( CKTDXCollision::CT_SPHERE == pCollisionData->m_CollisionType && 
				pCollisionData->m_Name.compare( attackBoxName ) == 0 )
			{
				pCollisionData->m_fRadiusScale = fScale;
				return;
			}
		}
	}
	
}

void CX2GameUnit::SetEnableAttackBox( const WCHAR* pName, bool bEnable )
{
	KTDXPROFILE(); 

	if ( SetEnableAttackBox( m_AttackListSet, pName, bEnable ) )
		return;
	else if ( SetEnableAttackBox( m_SubAttackListSet, pName, bEnable ) )
		return;
	else if ( !m_vecPairSubAttackListSet.empty() )	/// EFFECT_SET에서 직접 추가하는 SUB_ATTACK_LIST
	{
		vector<PairSubAttackListSet>::iterator vItr = m_vecPairSubAttackListSet.begin();
		while ( m_vecPairSubAttackListSet.end()	!= vItr )
		{
			if ( SetEnableAttackBox( vItr->second, pName, bEnable ) )
				return;
			else
				++vItr;
		}
	}
}

bool CX2GameUnit::SetEnableAttackBox( const CKTDXCollision::CollisionDataListSet& setCollisionDataList_, const WCHAR* pName_, const bool bEnable_ )
{
	BOOST_TEST_FOREACH( const CKTDXCollision::CollisionDataList*, pCollisionDataList, setCollisionDataList_ )
	{
		ASSERT( pCollisionDataList != NULL );
		BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, *pCollisionDataList )
		{
			ASSERT( pCollisionData != NULL );
			if( pCollisionData->m_Name.compare( pName_ ) == 0 )
			{
				if( bEnable_ != pCollisionData->m_bEnable )
				{
					pCollisionData->m_bEnable = bEnable_;
					return true;
				}
			}
			else
			{
				if(pCollisionData->m_CollisionType == CKTDXCollision::CT_LINE)
				{
					if( pCollisionData->m_FrameName.compare( pName_ ) == 0 )
					{
						if( bEnable_ != pCollisionData->m_bEnable )
						{
							pCollisionData->m_bEnable = bEnable_;
							return true;
						}
					}
				}
			} // else
		}// BOOST
	}// BOOST

	return false;
}


void CX2GameUnit::SetEnableCollisionBox( const WCHAR* pName, bool bEnable )
{
    BOOST_TEST_FOREACH( const CKTDXCollision::CollisionDataList*, pCollisionDataList, m_CollisionListSet )
    {
        ASSERT( pCollisionDataList != NULL );
        BOOST_TEST_FOREACH( CKTDXCollision::CollisionData*, pCollisionData, *pCollisionDataList )
        {
            ASSERT( pCollisionData != NULL );
            if ( pCollisionData->m_Name.compare( pName ) == 0 )
		    {
			    pCollisionData->m_bEnable = bEnable;
			    return;
		    }
        }
    }//BOOST_TEST_FOREACH()
}


float CX2GameUnit::GetItemAttackPower( CX2GameUnit* pAttacker, CX2Item* pItem /*= NULL*/, CX2DamageManager::DAMAGE_TYPE eDamageType/* = CX2DamageManager::DT_MIX*/ )
{
	if( NULL == pAttacker )
		return 0.f;

	float fItemAttackPower = 0.f;

	switch( pAttacker->GetGameUnitType() )
	{
	default:
		{
			// nothing
		} break;
	case CX2GameUnit::GUT_USER:
		{
			CX2GUUser* pGUUSer = (CX2GUUser*)pAttacker;

//{{ kimhc // 2010.7.30 // 스탯을 제외한 무기만의 데미지
			if ( NULL == pItem )
			{
				pItem = pGUUSer->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );

				if ( NULL == pItem )
					return 0.0f;
			}
			// 2008. 11. 07 김태완 : 피어싱 보정데미지 기획대로 수정
			//fExtraAttackPower += pItem->GetItemTemplet()->GetStat().m_fAtkPhysic + pItem->GetItemTemplet()->GetStat().m_fAtkMagic;
			//fExtraAttackPower = fExtraAttackPower * 0.3f;

			float fWeaponPhysicAtk	= 0.0f;
			float fWeaponMagicAtk	= 0.0f;
			float fStatPhysicAtk	= 0.0f;
			float fStatMagicAtk		= 0.0f;

			int iAddLevel = 0;
			bool bLevelLinkedStat = false;

            unsigned uNumSO = pItem->GetItemTemplet()->GetNumSocketOption();
			for ( int i = 0; i < (int) uNumSO; ++i )
			{
                int socketOption = pItem->GetItemTemplet()->GetSocketOption(i);
				if( 0 == socketOption )
					continue;

				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
				if( pSocketData == NULL )
					continue;

				if ( NULL != pSocketData && true == pSocketData->m_bIsLevelLinkedStat && g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
				{
					fWeaponPhysicAtk += pSocketData->m_Stat.m_fAtkPhysic;
					fWeaponMagicAtk += pSocketData->m_Stat.m_fAtkMagic;
					iAddLevel = pSocketData->m_iAddLevel;
					bLevelLinkedStat = true;
				}
			}

			switch ( eDamageType )
			{
			case CX2DamageManager::DT_PHYSIC:
			case CX2DamageManager::DT_WEAPON_PHYSIC:
				{
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
						// 스탯테이블을 통해 스탯 계산
						CX2Item::KItemFormatStatData kCalculateStat;
						g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
						const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
						const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                    const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef PVP_SEASON2_SOCKET
					if ( false == bLevelLinkedStat )
						fWeaponPhysicAtk = stat.m_fAtkPhysic;
#else
					fWeaponPhysicAtk	= stat.m_fAtkPhysic;
#endif
					fStatPhysicAtk		= pGUUSer->GetStat().GetStat().m_fAtkPhysic;
				} break;

			case CX2DamageManager::DT_MAGIC:
			case CX2DamageManager::DT_WEAPON_MAGIC:
				{
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
						// 스탯테이블을 통해 스탯 계산
						CX2Item::KItemFormatStatData kCalculateStat;
						g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
						const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
						const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                    const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef PVP_SEASON2_SOCKET
					if ( false == bLevelLinkedStat )
						fWeaponMagicAtk = stat.m_fAtkMagic;
#else
					fWeaponMagicAtk		= stat.m_fAtkMagic;
#endif
					fStatMagicAtk		= pGUUSer->GetStat().GetStat().m_fAtkMagic;

				} break;

			case CX2DamageManager::DT_MIX:
			case CX2DamageManager::DT_WEAPON_MIX:
				{
#ifdef PVP_SEASON2_SOCKET
					if ( false == bLevelLinkedStat )
					{
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
							// 스탯테이블을 통해 스탯 계산
							CX2Item::KItemFormatStatData kCalculateStat;
							g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
							const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
							const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                        const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
						fWeaponPhysicAtk = stat.m_fAtkPhysic;
						fWeaponMagicAtk = stat.m_fAtkMagic;
					}
					fStatPhysicAtk		= pGUUSer->GetStat().GetStat().m_fAtkPhysic;
					fStatMagicAtk		= pGUUSer->GetStat().GetStat().m_fAtkMagic;
#else
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
						// 스탯테이블을 통해 스탯 계산
						CX2Item::KItemFormatStatData kCalculateStat;
						g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
						const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
						const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                    const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
					fWeaponPhysicAtk = stat.m_fAtkPhysic;
					fWeaponMagicAtk = stat.m_fAtkMagic;
					fStatPhysicAtk		= pGUUSer->GetStat().GetStat().m_fAtkPhysic;
					fStatMagicAtk		= pGUUSer->GetStat().GetStat().m_fAtkMagic;
#endif

				} break;

				// 초기화를 했기 때문에 default는 있을 수 없음
			}
#ifdef PVP_SEASON2_SOCKET
			if ( true == bLevelLinkedStat && g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
				fItemAttackPower = ( fWeaponPhysicAtk + fWeaponMagicAtk ) * ( pGUUSer->GetUnitLevel() + iAddLevel );
			else
				fItemAttackPower = fWeaponPhysicAtk + fWeaponMagicAtk;
#else
			fItemAttackPower = fWeaponPhysicAtk + fWeaponMagicAtk;
#endif

			switch( g_pMain->GetNowStateID() )
			{
			case CX2Main::XS_DUNGEON_GAME:
				{
					// 태완 : 특정 던전에서의 인챈트 보정 코드. NEW_YEAR_EVENT_DUNGEON 등으로 검색해서 나오는 곳에 모두 작업해줘야 한다. (총 4곳)
					// 보정던전이 더 많아진다면 던전데이타 스크립트에 보정여부를 추가하는 방법이 더 나을 것 같다.


#ifdef HENIR_TEST
					if( true == CX2Dungeon::IsHenirDungeon( static_cast<const SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) &&
						CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
					{
						if(	g_pData->GetUIManager() != NULL && 
							g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
						{
							fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;

						}
					}
#endif HENIR_TEST

					//{{ 허상형 : [2011/3/29/] //	월드미션
#ifdef SERV_INSERT_GLOBAL_SERVER
					//	헤니르시공과 같은 속성을 가진다.
					if( NULL != g_pData->GetDungeonRoom() && g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID) == true )
					{
						if(	g_pData->GetUIManager() != NULL && 
							g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
						{
							fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;

						}
					}
#endif SERV_INSERT_GLOBAL_SERVER
					//}} 허상형 : [2011/3/29/] //	월드미션


					if(g_pData->GetDungeonRoom() != NULL )
					{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
						if( g_pData->GetDungeonManager()->GetDungeonData( g_pData->GetDungeonRoom()->GetDungeonID() )->m_bDamageEqualized == true )
						{
							fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;
						}
						else
#endif SERV_DUNGEON_OPTION_IN_LUA
						switch( g_pData->GetDungeonRoom()->GetDungeonID() )
						{
							//{{ 2009.01.20 김태완
			#ifdef NEW_YEAR_EVENT_DUNGEON
						case SEnum::DI_ELDER_NEWYEAR_NORMAL:
							{
								fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;
							} break;
			#endif
							//}}
			#ifdef HALLOWEEN_EVENT
						case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
						case SEnum::DI_ELDER_HALLOWEEN_HARD:
						case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
							// 할로윈 던전 관련 임시 코드 : 공격 데미지 보정, 하드코딩
							{
								fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;
							} break;
			#endif
							//{{ 2009.04.24 김태완 : 어린이날 던전보정 및 보정코드 정리
			#ifdef CHILDRENS_DAY_EVENT_DUNGEON
						case SEnum::DI_EVENT_KIDDAY_RUBEN:
						case SEnum::DI_EVENT_KIDDAY_ELDER:
						case SEnum::DI_EVENT_KIDDAY_BESMA:
						case SEnum::DI_EVENT_KIDDAY_ALTERA:

						case SEnum::DI_EVENT_TREE_DAY_ELDER:
						case SEnum::DI_EVENT_TREE_DAY_BESMA:
						case SEnum::DI_EVENT_TREE_DAY_ALTERA:
						case SEnum::DI_EVENT_TREE_DAY_PEITA:
						case SEnum::DI_EVENT_TREE_DAY_VELDER:
						case SEnum::DI_EVENT_TREE_DAY_HAMEL:
							//{{ oasis907 : 김상윤 [2010.10.21] // 
						case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
						case SEnum::DI_ELDER_HALLOWEEN_HARD:
						case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
							//}} oasis907 : 김상윤 [2010.10.21] // 
						case SEnum::DI_EVENT_VALENTINE_DAY:
						case SEnum::DI_EVENT_HALLOWEEN_DAY:
							{
								fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;
							} break;
			#endif
							//}}
						default:
							break;
						}

					}
				}break;
				
			case CX2Main::XS_PVP_GAME:
				{
					// PVP 자유채널 피어싱 데미지 보정
					if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_FREE )
					{
						fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;
					}
#ifdef TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
					else if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_TOURNAMENT )
					{
						fItemAttackPower = fStatPhysicAtk + fStatMagicAtk;
					}
#endif TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
					else
					{
						fItemAttackPower *= 0.1f;
					}
				}break;

			default:
				{
					// nothing 
				} break;
			}
//}} kimhc // 2010.7.30 // 스탯을 제외한 무기만의 데미지
			
		} break;
	}

	return fItemAttackPower;
}




// 데미지 계산시에 레벨차이에 따른 보정수치를 계산한다
float CX2GameUnit::CalcLevelFactorForFinalDamage( CX2GameUnit* pAttacker )
{
	if( NULL == pAttacker )
		return 1.f;

	if( NULL == g_pX2Game )
		return 1.f;




#ifdef TEMP_NO_LEVEL_FACTOR_AT_EVENT_DUNGEON
	
	
	switch( g_pX2Game->GetGameType() )
	{
	case CX2Game::GT_DUNGEON:
		{
			CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
			if( NULL != pDungeonGame &&
				NULL != pDungeonGame->GetDungeon() &&
				NULL != pDungeonGame->GetDungeon()->GetDungeonData() )
			{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
				if( pDungeonGame->GetDungeon()->GetDungeonData()->m_bLevelEqualized == true )
				{
					return 1.f;
				}
#endif SERV_DUNGEON_OPTION_IN_LUA

				switch( pDungeonGame->GetDungeon()->GetDungeonData()->m_DungeonID )
				{
				case SEnum::DI_EVENT_KIDDAY_RUBEN:	
				case SEnum::DI_EVENT_KIDDAY_ELDER:	
				case SEnum::DI_EVENT_KIDDAY_BESMA:	
				case SEnum::DI_EVENT_KIDDAY_ALTERA:

				case SEnum::DI_EVENT_TREE_DAY_ELDER:
				case SEnum::DI_EVENT_TREE_DAY_BESMA:
				case SEnum::DI_EVENT_TREE_DAY_ALTERA:
				case SEnum::DI_EVENT_TREE_DAY_PEITA:
				case SEnum::DI_EVENT_TREE_DAY_VELDER:
				case SEnum::DI_EVENT_TREE_DAY_HAMEL:
					//{{ oasis907 : 김상윤 [2010.10.21] // 
				case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
				case SEnum::DI_ELDER_HALLOWEEN_HARD:
				case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
					//}} oasis907 : 김상윤 [2010.10.21] // 
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
				case SEnum::DI_EVENT_HALLOWEEN_DAY:
#endif //SERV_HALLOWEEN_EVENT_2013
					{
						return 1.f;
					} break;

				case SEnum::DI_ELDER_HENIR_SPACE:
					// 			case SEnum::DI_BESMA_HENIR_SPACE:
					// 			case SEnum::DI_ALTERA_HENIR_SPACE:
					// 			case SEnum::DI_FEITA_HENIR_SPACE:
					// 				// kimhc // 벨더 헤니르 // 2009-10-27
					// 			case SEnum::DI_VELDER_HENIR_SPACE:
					// 			case SEnum::DI_HAMEL_HENIR_SPACE:
					//{{ 허상형 : [2011/3/3/] //	월드 미션
				case SEnum::DI_EVENT_VALENTINE_DAY:
#ifdef SERV_INSERT_GLOBAL_SERVER
					CASE_DEFENCE_DUNGEON
#endif SERV_INSERT_GLOBAL_SERVER
						//}} 허상형 : [2011/3/3/] //	월드 미션
					{
#ifndef NEW_HENIR_DUNGEON // 1레벨 보정 -> 연동 레벨 던전으로 변경
						// 헤니르 시공 도전 모드에서 데미지 보정 없게
						if( CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
							return 1.f;
#endif // NEW_HENIR_DUNGEON

						//{{ 허상형 : [2011/3/29/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
						if( NULL != g_pData->GetDungeonRoom() && g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) == true )
						{
							return 1.f;
						}
#endif SERV_INSERT_GLOBAL_SERVER
						//}} 허상형 : [2011/3/29/] //	월드 미션

						// 헤니르 시공에서 몬스터가 몬스터 공격할 때는 데미지 보정 없게
						if( GUT_NPC == pAttacker->GetGameUnitType() && GUT_NPC == GetGameUnitType() )
						{
							return 1.f;
						}
					} break;
				}
			}
		} break;
	}

#endif TEMP_NO_LEVEL_FACTOR_AT_EVENT_DUNGEON





	float fLvFactor = 1.f;
	int iAttackerLevel = 0;
	int iDefencerLevel = 0;

	switch( g_pX2Game->GetGameType() )
	{
	case CX2Game::GT_DUNGEON:
		{
			if( GUT_USER == pAttacker->GetGameUnitType() && GUT_NPC == GetGameUnitType() )
			{
				CX2GUUser* pUser = (CX2GUUser*) pAttacker;
				CX2GUNPC* pMonster = (CX2GUNPC*) this;

				iAttackerLevel = pUser->GetUnit()->GetUnitData().m_Level;
				iDefencerLevel = pMonster->GetHardLevel();
			}
			else if( GUT_NPC == pAttacker->GetGameUnitType() && GUT_NPC == GetGameUnitType() ) // 이브의 소환 몬스터랑 다른 몬스터가 싸울때
			{
				CX2GUNPC* pMonster1 = (CX2GUNPC*) pAttacker;
				CX2GUNPC* pMonster = (CX2GUNPC*) this;

				iAttackerLevel = pMonster1->GetHardLevel();
				iDefencerLevel = pMonster->GetHardLevel();
			}
		} break;

	case CX2Game::GT_PVP:
		{
			// 대전 게임에서는 레벨 보정없음
		} break;
	}


	if( 0 != iAttackerLevel &&
		0 != iDefencerLevel )
	{
		if( iAttackerLevel == iDefencerLevel )
		{
			fLvFactor = 1.f;
		}
		else if( iAttackerLevel < iDefencerLevel )
		{
			fLvFactor = 1.f - ( iAttackerLevel - iDefencerLevel ) * 0.127f;
		}
		else 
		{
			fLvFactor = 1.f - ( iAttackerLevel - iDefencerLevel ) * 0.065f;
		}
	}

	if( fLvFactor <= 0.f )
	{
		fLvFactor = 0.f;
	}
	return fLvFactor;
}




float CX2GameUnit::CalcExtraAttackPower( CX2DamageManager::DamageData* pAttDamageData, CX2GameUnit* pAttacker, CX2Stat* pAttackerStat )
{
	if( NULL == pAttDamageData )
		return 0.f;

	if( pAttDamageData->attackType == CX2DamageManager::AT_SPECIAL )
		return 0.f;

	// 이펙트로 타격시 추가 데미지 적용안되는 문제 수정을 위해
	// 공격자의 damage data를 직접 들고와 계산한다.
	CX2DamageManager::DamageData& damageData = pAttacker->GetDamageData(); // pAttDamageData
	//if( pDamageData == NULL )
	//	return 0.f;

	float fExtraAttackPower = 0.f;
	// 화염강타, 냉기 충격 데미지 추가
	if( false == damageData.m_MultipleExtraDamage.empty() )
	{
		for( int i=0; i<(int)damageData.m_MultipleExtraDamage.size(); ++i )
		{
			const CX2DamageManager::ExtraDamageData& multipleExtraDamageData = damageData.m_MultipleExtraDamage[i];
			// 주의!!! 현재는 강타계열은 모두 SRO_ATTACK_EXTRA_DAMAGE_MULTIPLE 랜덤넘버를 사용하는 데, 다른 랜덤 넘버를 사용하는 extra damage가 추가될 경우에 작업 주의
			if( GetRandomFloat( CKTDXRandomNumbers::SRO_ATTACK_EXTRA_DAMAGE_MULTIPLE + i ) >= multipleExtraDamageData.m_fRate )
				continue;


			// 강타계열 추가데미지 있는지 계산
			float fSmashAttackPower = 0.f;
			switch( multipleExtraDamageData.m_ExtraDamageType )
			{
			case CX2DamageManager::EDT_FIRE_SMASH:
			case CX2DamageManager::EDT_COLD_SMASH:
			case CX2DamageManager::EDT_EVENT_SMASH:
#ifdef CAMILLA_SMASH
			case CX2DamageManager::EDT_CAMILLA_SMASH:
#endif CAMILLA_SMASH
#ifdef EDT_EVENT_SMASH_THIRD
			case CX2DamageManager::EDT_EVENT_SMASH_3:
#endif //EDT_EVENT_SMASH_THIRD
#ifdef EDT_EVENT_SMASH_SECOND
			case CX2DamageManager::EDT_EVENT_SMASH_2:
#endif EDT_EVENT_SMASH_SECOND
				{
					switch( pAttacker->GetGameUnitType() )
					{
					case CX2GameUnit::GUT_USER:
						{
							CX2GUUser* pGUUser = (CX2GUUser*)pAttacker;
							CX2Item* pItem = pGUUser->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
							fSmashAttackPower = GetItemAttackPower( pAttacker, pItem );
						} break;

					case CX2GameUnit::GUT_NPC:
						{
							fSmashAttackPower = pAttackerStat->GetStat().m_fAtkPhysic + pAttackerStat->GetStat().m_fAtkMagic;
						} break;
					}
				} break;
			}




			// 속성 저항 적용
			switch( multipleExtraDamageData.m_ExtraDamageType )
			{
			case CX2DamageManager::EDT_FIRE_SMASH:
				{
					ApplyEnchantResist( CX2EnchantItem::ET_BLAZE, fSmashAttackPower );
				} break;
			case CX2DamageManager::EDT_COLD_SMASH:
				{
					ApplyEnchantResist( CX2EnchantItem::ET_WATER, fSmashAttackPower );
				} break;
			case CX2DamageManager::EDT_EVENT_SMASH:
#ifdef CAMILLA_SMASH
			case CX2DamageManager::EDT_CAMILLA_SMASH:
#endif CAMILLA_SMASH
#ifdef EDT_EVENT_SMASH_THIRD
			case CX2DamageManager::EDT_EVENT_SMASH_3:
#endif //EDT_EVENT_SMASH_THIRD
#ifdef EDT_EVENT_SMASH_SECOND
			case CX2DamageManager::EDT_EVENT_SMASH_2:
#endif EDT_EVENT_SMASH_SECOND
				{
					ApplyEnchantResist( CX2EnchantItem::ET_DARK, fSmashAttackPower );
				} break;
			}



			// 기획 데미지 배율 수치 적용
			const float MAGIC_ATTACK_POWER_RATE			= 0.2f;
			const float MAGIC_EVENT_ATTACK_POWER_RATE	= 0.4f;
			switch( multipleExtraDamageData.m_ExtraDamageType )
			{
			case CX2DamageManager::EDT_FIRE_SMASH:
			case CX2DamageManager::EDT_COLD_SMASH:
				{
					fSmashAttackPower *= MAGIC_ATTACK_POWER_RATE;
				} break;

			case CX2DamageManager::EDT_EVENT_SMASH:
#ifdef CAMILLA_SMASH
			case CX2DamageManager::EDT_CAMILLA_SMASH:
#endif CAMILLA_SMASH
#ifdef EDT_EVENT_SMASH_THIRD
			case CX2DamageManager::EDT_EVENT_SMASH_3:
#endif //EDT_EVENT_SMASH_THIRD
#ifdef EDT_EVENT_SMASH_SECOND
			case CX2DamageManager::EDT_EVENT_SMASH_2:
#endif EDT_EVENT_SMASH_SECOND
				{
					fSmashAttackPower *= MAGIC_EVENT_ATTACK_POWER_RATE;
				} break;
			} 

			fExtraAttackPower += fSmashAttackPower;
		}
	}


	return fExtraAttackPower;
}


// 자신이 받을 damage를 계산해준다
float CX2GameUnit::GetFinalDamage( CX2DamageManager::DamageData* pAttDamageData )
{
#if defined( _SERVICE_ ) 
	// NOTE: 데미지 해킹이 된다면 그 때 여기를 themida로 보호하자
	// checked 
	ELSWORD_VIRTUALIZER_START
#endif

	if( NULL == pAttDamageData )
	{
		ErrorLog( XEM_ERROR117 );
		return 0.f;
	}

	if ( null == pAttDamageData->optrAttackerGameUnit )
		return 1.f;

	if ( null == pAttDamageData->optrDefenderGameUnit )
		return 1.f;

	if( NULL == g_pX2Game )
		return 2.f;

#ifdef APPLY_THEMIDA
	ENCODE_START
#endif

	CX2GameUnit* pAttacker = pAttDamageData->optrAttackerGameUnit.GetObservable();
#ifdef SERV_SKILL_NOTE
	CX2GameUnit* pDefender = pAttDamageData->optrDefenderGameUnit.GetObservable();
#endif

	if( pAttacker == NULL || pDefender == NULL)
		return 0.f;
	
	float fPhysicAttack = pAttacker->GetPhysicAttackValue();
	float fMagicAttack = pAttacker->GetMagicAttackValue();
	float fPhysicDefence = pDefender->GetPhysicDefenceValue();
	float fMagicDefence = pDefender->GetMagicDefenceValue();

	// 공격자, 피격자 stat 계산
	CX2Stat attackerStat;	
	CX2Stat defenderStat;	
	attackerStat.AccessStat().SetStat( (pAttacker->GetStat().GetStat()) );
	defenderStat.AccessStat().SetStat( (GetStat().GetStat()) );

#ifdef	SERV_TRAPPING_RANGER_TEST
	float	fSharpenDamageRate = 1.0f;	//	날카로운 화살로 인한 방어무시
#endif	SERV_TRAPPING_RANGER_TEST

	// 공격자의 스킬에 의한 공격자의 공격력 stat 증가
	if( CX2GameUnit::GUT_USER == pAttacker->GetGameUnitType() )
	{
		CX2GUUser* pUser = (CX2GUUser*) pAttacker;
		const CX2UserSkillTree& cUserSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RRF_CONFIDENCE_VICTORY, true );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RRF_CONFIDENCE_VICTORY );

			if( NULL != pSkillTemplet )
			{
	#ifdef BALANCE_PATCH_20120329
				if( !IsSamef( pDefender->GetMaxHp(), 0.f ) && pDefender->GetNowHp() > 0.f && pDefender->GetNowHp() / pDefender->GetMaxHp() <= 0.2f )
	#else
				if( 0.f != pAttacker->GetMaxHp() &&
					pAttacker->GetNowHp() / pAttacker->GetMaxHp() > 0.9f )
	#endif
				{
					fPhysicAttack	*= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_REL, iSkillLevel );
					fMagicAttack	*= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_REL, iSkillLevel );
				}
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		int iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_CONFIDENCE_VICTORY );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_CONFIDENCE_VICTORY, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
	#ifdef BALANCE_PATCH_20120329
				if( !IsSamef( pDefender->GetMaxHp(), 0.f ) && pDefender->GetNowHp() > 0.f && pDefender->GetNowHp() / pDefender->GetMaxHp() <= 0.2f )
	#else
				if( 0.f != pAttacker->GetMaxHp() &&
					pAttacker->GetNowHp() / pAttacker->GetMaxHp() > 0.9f )
	#endif
				{
					fPhysicAttack	*= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_REL );
					fMagicDefence	*= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_REL );
				}
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		int iSkillLevelOverddrive = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ABM_MANAFLOW_OVERDRIVE );
		if( iSkillLevelOverddrive > 0 )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ABM_MANAFLOW_OVERDRIVE );

			if( NULL != pSkillTemplet )
			{
				if( !IsSamef( pAttacker->GetMaxHp(), 0.f ) &&
					pAttacker->GetNowHp() / pAttacker->GetMaxHp() > 0.9f )
				{
					float fAtkPhysicRel = 1.f + ( pAttacker->GetNowMp() / pAttacker->GetMaxMp() ) * 0.06f * 
						pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_PHYSIC_REL, iSkillLevelOverddrive );

					fPhysicAttack *= fAtkPhysicRel;
				}
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ABM_MANAFLOW_OVERDRIVE, iSkillLevelOverddrive );

			if( NULL != pSkillTemplet )
			{
				if( !IsSamef( pAttacker->GetMaxHp(), 0.f ) &&
					pAttacker->GetNowHp() / pAttacker->GetMaxHp() > 0.9f )
				{
					float fAtkPhysicRel = 1.f + ( pAttacker->GetNowMp() / pAttacker->GetMaxMp() ) * 0.06f * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_PHYSIC_REL );
					fPhysicAttack *= fAtkPhysicRel;
				}
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
	#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LTR_SHARPEN_ARROW, true );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LTR_SHARPEN_ARROW );

			if( NULL != pSkillTemplet )
			{
				fSharpenDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_UNFIXED_DEFENSE_REL, iSkillLevel );
			}
		}
	#else //UPGRADE_SKILL_SYSTEM_2013
		iSkillLevel = cUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_RTR_SHARPEN_ARROW );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_RTR_SHARPEN_ARROW, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				fSharpenDamageRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_UNFIXED_DEFENSE_REL );
			}
		}
	#endif //UPGRADE_SKILL_SYSTEM_2013
		
#endif	SERV_TRAPPING_RANGER_TEST
	}

	//{{ kimhc // 2011.4.24 // 월드버프 2단계, 드래곤의 숨결 & 월드 버프 3단계 엘의 축복
#ifdef	SERV_INSERT_GLOBAL_SERVER
	// PVP가 아닌 경우에 적용
	if ( NULL != g_pData->GetWorldMissionManager() && CX2Game::GT_PVP != g_pX2Game->GetGameType() )
	{
		// 월드버프 2단계, 드래곤의 숨결이 적용 중이고 공격자가 USER인 경우
		if ( false == g_pData->GetWorldMissionManager()->IsNullWorldBuffDragonBreath() &&
			 CX2GameUnit::GUT_USER == pAttacker->GetGameUnitType() )
		{
			float fIncreaseRateAtk = CalculateIncreasingRate( g_pData->GetWorldMissionManager()->GetDragonBreathAttackStatRateIncreased() );
			fPhysicAttack += fPhysicAttack * fIncreaseRateAtk;
			fMagicAttack += fMagicAttack * fIncreaseRateAtk;
		}

		// 월드버프 3단계, 엘의 축복이 적용 중이고 피격자가 USER인 경우
		if ( false == g_pData->GetWorldMissionManager()->IsNULLWorldBuffElBless() &&
			 CX2GameUnit::GUT_USER == GetGameUnitType() )
		{
			float fIncreaseRateDef = CalculateIncreasingRate( g_pData->GetWorldMissionManager()->GetElBlessDefenceStatRateIncreased() );
			fPhysicDefence += fPhysicDefence * fIncreaseRateDef;
			fMagicDefence += fMagicDefence * fIncreaseRateDef;
		}		
	}
#endif	SERV_INSERT_GLOBAL_SERVER
	//}} kimhc // 2011.4.24 // 월드버프 2단계, 드래곤의 숨결 & 월드 버프 3단계 엘의 축복


#ifdef PET_AURA_SKILL
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( CX2GameUnit::GUT_USER == pAttacker->GetGameUnitType() )
			{
				CX2GUUser* pUser = (CX2GUUser*) pAttacker;
				CX2Unit* pUnit = pUser->GetUnit();
#ifdef FIX_SOCKET_OPTION_INCREASE_ATK_AND_DEF
				if( 0.f != pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkPhysicRate )
					fPhysicAttack += fPhysicAttack * pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkPhysicRate;

				if( 0.f != pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkMagicRate )
					fMagicAttack += fMagicAttack * pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkMagicRate;
#else
				if( pUnit != NULL && pUnit->GetPetInfo() != NULL && (float)pUnit->GetPetInfo()->m_sSatiety / CX2PetManager::MAX_OF_SATIETY >= CX2PetManager::SATIETY_RATIO_TO_BE_ABLE_TO_APPLY_AURA )
				{
					fPhysicAttack += fPhysicAttack * pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkPhysicRate;
					fMagicAttack += fMagicAttack * pUser->GetSocketData().m_Stat.m_ExtraStat.m_fIncreaseAtkMagicRate;
				}
#endif //FIX_SOCKET_OPTION_INCREASE_ATK_AND_DEF
			}

			if ( CX2GameUnit::GUT_USER == GetGameUnitType() )
			{
				CX2GUUser* pUser = (CX2GUUser*) this;
				CX2Unit* pUnit = pUser->GetUnit();

#ifdef FIX_SOCKET_OPTION_INCREASE_ATK_AND_DEF
				if( 0.f != m_SocketData.m_Stat.m_ExtraStat.m_fIncreaseDefPhysicRate )
					fPhysicDefence += fPhysicDefence * m_SocketData.m_Stat.m_ExtraStat.m_fIncreaseDefPhysicRate;

				if( 0.f != m_SocketData.m_Stat.m_ExtraStat.m_fIncreaseDefMagicRate )
					fMagicDefence += fMagicDefence * m_SocketData.m_Stat.m_ExtraStat.m_fIncreaseDefMagicRate;
#else
				if( pUnit != NULL && pUnit->GetPetInfo() != NULL && (float)pUnit->GetPetInfo()->m_sSatiety / CX2PetManager::MAX_OF_SATIETY >= CX2PetManager::SATIETY_RATIO_TO_BE_ABLE_TO_APPLY_AURA )
				{
					fPhysicDefence += fPhysicDefence * m_SocketData.m_Stat.m_ExtraStat.m_fIncreaseDefPhysicRate;
					fMagicDefence += fMagicDefence * m_SocketData.m_Stat.m_ExtraStat.m_fIncreaseDefMagicRate;
				}
#endif //FIX_SOCKET_OPTION_INCREASE_ATK_AND_DEF
			}
		} break;
	default:
		break;
	}
#endif	PET_AURA_SKILL

#ifdef SERV_SKILL_NOTE
	if( pDefender->GetDefenceMemoTime() > 0.f )
	{
		fPhysicDefence *= 2.f;
		fMagicDefence *= 2.f;
	}
	if( pAttacker != NULL && pAttacker->GetEnduranceAttackTime() > 0.f )
	{
		fPhysicAttack *= 1.1f; 
		fMagicAttack *= 1.1f;
	}
#endif

#ifndef DUNGEON_ITEM
	attackerStat.AccessStat().MultiplyStat( pAttacker->GetStatTimesByScaleTransform() );
	defenderStat.AccessStat().MultiplyStat( GetStatTimesByScaleTransform() );
#endif

	attackerStat.AccessAddOnStat().SetStat( pAttacker->GetStat().GetAddOnStat() );
	defenderStat.AccessAddOnStat().SetStat( GetStat().GetAddOnStat() );

//{{ kimhc // 2010.4.3 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
	ProcessStatPercentUpBuffer( pAttacker, attackerStat, defenderStat );
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.3 // 비밀던전 작업(셋트아이템효과)

	CX2Stat* pAttackerStat = &attackerStat;
	CX2Stat* pDefenderStat = &defenderStat;



	float fLvFactor			= CalcLevelFactorForFinalDamage( pAttacker );	// 레벨 보정치 계산		
	float fExtraAttackPower = CalcExtraAttackPower( pAttDamageData, pAttacker, pAttackerStat );		// 화염강타, 냉기 충격 추가 공격력 


	// 프로즌 방어도 디버프 배율
	float fFronzenDebuffDefenceRate = 1.f;				
	if( GetExtraDamagePack().m_EnchantFrozen.m_fTime > 0.f )
	{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
		const CX2EnchantItem::EnchantData* pEnchantData = GetWeaponEnchantData( CX2DamageManager::EDT_ENCHANT_FROZEN );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
        const CX2EnchantItem::EnchantData* pEnchantData = NULL;
		switch( GetGameUnitType() )
		{
		case CX2GameUnit::GUT_USER:
			{
				pEnchantData = g_pData->GetEnchantItem()->GetEnchantData( CX2DamageManager::EDT_ENCHANT_FROZEN );
			} break;

		case CX2GameUnit::GUT_NPC:
			{
				pEnchantData = g_pData->GetEnchantItem()->GetNPCEnchantData( CX2DamageManager::EDT_ENCHANT_FROZEN );
			} break;
		}
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR


		if( NULL != pEnchantData )
		{

			switch( GetExtraDamagePack().m_EnchantFrozen.m_Accumulation )
			{
			default:
				{
					// do nothing;
					ASSERT( !"invalid frozen accumulation" );
				} break;
			case 0:
				{
					fFronzenDebuffDefenceRate = 1.f - ( (float)pEnchantData->m_FirstDefenceDebuffPercent / 100.f);
				} break;
			case 1:
				{
					fFronzenDebuffDefenceRate = 1.f - ( (float)pEnchantData->m_SecondDefenceDebuffPercent / 100.f);
				} break;
			case 2:
				{
					fFronzenDebuffDefenceRate = 1.f - ( (float)pEnchantData->m_ThirdDefenceDebuffPercent / 100.f);
				} break;
			}
		}
	}


	
	// 피어싱 데미지의 경우 방어력이 0
	float fPiercingDefenceFactor = 1.f;
	if( pAttDamageData->m_bPenetrate == true || pAttDamageData->m_bIgnoreDefence == true )
	{
		fPiercingDefenceFactor = 0.f;
	}


#ifdef ENCHANT_BALANCE_101014 
	// oasis907 : 김상윤 [2010.10.5] // 속성 쇼크시 피해자 방어력 향상
	float fShockDebuffDefenceRate = 0.f;				
	if( GetExtraDamagePack().m_EnchantShock.m_fTime > 0.f )
	{
		fShockDebuffDefenceRate = 0.1f;
	}
#endif ENCHANT_BALANCE_101014

	// 공격력, 방어력 계산
	float fAttackPower	= 0.f;
	float fDefencePower = 0.f;

	float fAttackPowerPhysic = 0.f;
	float fAttackPowerMagic = 0.f;
	float fDefencePowerPhysic = 0.f;
	float fDefencePowerMagic = 0.f;

	float fAttackerPhysicDamage = pAttDamageData->damage.fPhysic;
	float fAttackerMagicDamage = pAttDamageData->damage.fMagic;
	
	if ( CX2DamageManager::AT_NORMAL == pAttDamageData->attackType )
#ifdef BALANCE_CODE_NEMESIS_20121213
		pAttacker->ModifyFinalDamageByGameUnitType( pAttDamageData->damageType, fAttackerPhysicDamage, fAttackerMagicDamage, pAttDamageData->fForceDownValue );
#else
		pAttacker->ModifyFinalDamageByGameUnitType( pAttDamageData->damageType, fAttackerPhysicDamage, fAttackerMagicDamage );
#endif //BALANCE_CODE_NEMESIS_20121213

	switch( pAttDamageData->damageType )
	{
	case CX2DamageManager::DT_PHYSIC:
		{
			fAttackPower = fPhysicAttack + pAttackerStat->GetAddOnStat().m_fAtkPhysic + fExtraAttackPower;

			fDefencePower = fPhysicDefence + pDefenderStat->GetAddOnStat().m_fDefPhysic;
			fDefencePower *= fPiercingDefenceFactor;
		} break;

	case CX2DamageManager::DT_MAGIC:
		{
			fAttackPower = fMagicAttack + pAttackerStat->GetAddOnStat().m_fAtkMagic + fExtraAttackPower;

			fDefencePower = fMagicDefence + pDefenderStat->GetAddOnStat().m_fDefMagic;
			fDefencePower *= fPiercingDefenceFactor;
		} break;
	case CX2DamageManager::DT_MIX:
		{
			fAttackPowerPhysic = fPhysicAttack + pAttackerStat->GetAddOnStat().m_fAtkPhysic + fExtraAttackPower;
			fAttackPowerMagic = fMagicAttack + pAttackerStat->GetAddOnStat().m_fAtkMagic + fExtraAttackPower;
			fDefencePowerPhysic = fPhysicDefence + pDefenderStat->GetAddOnStat().m_fDefPhysic;
			fDefencePowerMagic = fMagicDefence + pDefenderStat->GetAddOnStat().m_fDefMagic;;			           
			
			fDefencePower = (fDefencePowerPhysic + fDefencePowerMagic) * 0.5f;
			fDefencePower *= fPiercingDefenceFactor;
		}
		break;

//{{ kimhc // 2010.7.30 // 스탯을 제외한 무기만의 데미지
	case CX2DamageManager::DT_WEAPON_PHYSIC:
		{
			fAttackPower = GetItemAttackPower( pAttacker, NULL, CX2DamageManager::DT_PHYSIC ) + fExtraAttackPower;

			fDefencePower = fPhysicDefence + pDefenderStat->GetAddOnStat().m_fDefPhysic;
			fDefencePower *= fPiercingDefenceFactor;
		} break;

	case CX2DamageManager::DT_WEAPON_MAGIC:
		{
			fAttackPower = GetItemAttackPower( pAttacker, NULL, CX2DamageManager::DT_MAGIC ) + fExtraAttackPower;

			fDefencePower = fMagicDefence + pDefenderStat->GetAddOnStat().m_fDefMagic;
			fDefencePower *= fPiercingDefenceFactor;
		} break;
	case CX2DamageManager::DT_WEAPON_MIX:
		{
			fAttackPowerPhysic	= GetItemAttackPower( pAttacker, NULL, CX2DamageManager::DT_PHYSIC ) + fExtraAttackPower;
			fAttackPowerMagic	= GetItemAttackPower( pAttacker, NULL, CX2DamageManager::DT_MAGIC ) + fExtraAttackPower;

			fDefencePowerPhysic = fPhysicDefence + pDefenderStat->GetAddOnStat().m_fDefPhysic;
			fDefencePowerMagic	= fMagicDefence + pDefenderStat->GetAddOnStat().m_fDefMagic;			           

			fDefencePower = (fDefencePowerPhysic + fDefencePowerMagic) * 0.5f;
			fDefencePower *= fPiercingDefenceFactor;
		}
		break;
//}} kimhc // 2010.7.30 // 스탯을 제외한 무기만의 데미지
	}

#ifdef	SERV_TRAPPING_RANGER_TEST
	fDefencePower *= fSharpenDamageRate;	//	날카로운 화살 적용
#endif	SERV_TRAPPING_RANGER_TEST



	// 레벨 1에 해당하는 100% 방어력 & 레벨당 방어력 증가량
	const float MAGIC_DEFENCE_POWER_DELTA	= 62.f;
	const float MAGIC_DEFENCE_POWER_LV1		= 410.f - MAGIC_DEFENCE_POWER_DELTA; 



	// 공격자 레벨
	int iAttackerLevel = 1;
	switch( pAttacker->GetGameUnitType() )
	{
	case CX2GameUnit::GUT_USER:
		{
			CX2GUUser* pUser = (CX2GUUser*) pAttacker;
			if( NULL != pUser->GetUnit() )
			{
#ifdef CORRECTION_DAMAGE_FREE_CHANNEL
				if ( NULL != g_pX2Game && NULL != g_pX2Game->IsDamageFreeGame() )
					iAttackerLevel = 1;
				else
					iAttackerLevel = pUser->GetUnit()->GetUnitData().m_Level;
#else
				iAttackerLevel = pUser->GetUnit()->GetUnitData().m_Level;
#endif
			}
		} break;

	case CX2GameUnit::GUT_NPC:
		{
			CX2GUNPC* pMonster1 = (CX2GUNPC*) pAttacker;
			iAttackerLevel = pMonster1->GetHardLevel();
		} break;
	}


	// 공격자 레벨에 해당하는 100% 방어력
	const float fFullDefencePowerByAttackerLevel = MAGIC_DEFENCE_POWER_LV1 + MAGIC_DEFENCE_POWER_DELTA * (float) iAttackerLevel;



	// 공격자의 스킬에 의한 피격자 방어도 감소
	float fDefenceRateBySkill = 1.f; 
#ifdef NEW_SKILL_TREE
	if( CX2GameUnit::GUT_USER == pAttacker->GetGameUnitType() )
	{
		CX2GUUser* pUser = (CX2GUUser*) pAttacker;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADM_SHADOW_DATH, true );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ADM_SHADOW_DATH );
			if( NULL != pSkillTemplet )
			{
				fDefenceRateBySkill	 = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_UNFIXED_DEFENSE_REL, iSkillLevel );
			}
		}
	#else //UPGRADE_SKILL_SYSTEM_2013
		int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_SHADOW_DATH );

		if( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_SHADOW_DATH, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				fDefenceRateBySkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_UNFIXED_DEFENSE_REL );
			}
		}
	#endif //UPGRADE_SKILL_SYSTEM_2013
	}
#endif NEW_SKILL_TREE



	// 방어력에 의한 방어도 계산
	float fDefencePowerRate = 0.f;
	switch( g_pX2Game->GetGameType() )
	{
	case CX2Game::GT_DUNGEON:
	case CX2Game::GT_BATTLE_FIELD:
		{
			fDefencePowerRate = (fDefencePower * fLvFactor) / fFullDefencePowerByAttackerLevel;
		} break;

	case CX2Game::GT_PVP:
		{
			//피격자방어도 = 피격자방어력 / ( 기본1레벨100%방어력 + ( ( 공격자의레벨100%방어력 - 기본1레벨100%방어력 ) / 10 ) )
			fDefencePowerRate = fDefencePower / ( MAGIC_DEFENCE_POWER_LV1 + (fFullDefencePowerByAttackerLevel-MAGIC_DEFENCE_POWER_LV1) * 0.1f );

		} break;
	}

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	if( pAttDamageData->m_bLvDamage == true )
	{		
#ifdef BALANCE_PATCH_20120329
		float fLvDamageFactor = 18.f;
#else
		float fLvDamageFactor = 2.f;
#endif
		if( g_pX2Game->IsDamageFreeGame() == false )
			fLvDamageFactor = (float)pAttacker->GetUnitLevel();
	
		int iLvAttacker = (int)fLvDamageFactor;
		int iLvDefener = pDefender->GetUnitLevel();

		float fLvCorrection = 1.f;
		float fGameRate = 1.f;
		if( g_pX2Game->GetGameType() == CX2Game::GT_PVP )
		{
			if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL || KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
#ifdef BALANCE_PATCH_20120329
				fLvDamageFactor = 18.f;
#else
				fLvDamageFactor = fLvDamageFactor / 6.f;
#endif
			fGameRate = pAttDamageData->m_fPvpRate;
			fLvCorrection = 1.f;
		}
		else
		{
			fGameRate = pAttDamageData->m_fDungeonRate;
			if( iLvDefener - iLvAttacker > 5 )
			{
				fLvCorrection *= ( (iLvDefener - iLvAttacker - 5) * 0.1f);
				fLvCorrection = 1.f - fLvCorrection;				
			}
		}

		if( fLvCorrection < 0.f )
			fLvCorrection = 0.f;

#ifdef BALANCE_PATCH_20120329
		float fEnergySpurtPowerRate = 1.f;
		if( CX2GameUnit::GUT_USER == pAttacker->GetGameUnitType() )
		{
			CX2GUUser* pUser = (CX2GUUser*) pAttacker;
			fEnergySpurtPowerRate = pUser->GetPowerRate();
		}
#endif

		float fFixDamage = 0.f;
		switch( pAttDamageData->damageType )
		{
		case CX2DamageManager::DT_PHYSIC:
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 에너지 스퍼트, 마공 -> 물공으로 변경
			fFixDamage = pAttacker->GetStat().GetStat().m_fAtkPhysic * 6.16f;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			fFixDamage = fAttackerPhysicDamage;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			break;
		case CX2DamageManager::DT_MAGIC:

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 에너지 스퍼트, 마공 -> 물공으로 변경
			fFixDamage = fAttackerMagicDamage;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
#ifdef BALANCE_PATCH_20120329
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			fFixDamage = pAttacker->GetStat().GetStat().m_fAtkMagic * 7.68f;
#else //UPGRADE_SKILL_SYSTEM_2013
			fFixDamage = pAttacker->GetStat().GetStat().m_fAtkMagic * 10.f;
#endif //UPGRADE_SKILL_SYSTEM_2013
#else
			fFixDamage = fAttackerMagicDamage;
#endif
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

			break;
		default:
			fFixDamage = 2000.f;
			break;
		}

		float fFinalDamage = 0.f;
#ifdef BALANCE_PATCH_20120329
		fFinalDamage = ( powf(fLvDamageFactor, 2.f) * 35.f * fLvCorrection * fGameRate + fFixDamage ) * fEnergySpurtPowerRate;
#else
		fFinalDamage = fLvDamageFactor * fLvCorrection * fGameRate * fFixDamage;
#endif
		const float fIncreaseDamage 
			= ( pAttDamageData->m_bMeleeAttack ? 
				fFinalDamage * ( pAttacker->GetAdditionalFinalDamageMultiplierMeleeAttack() - 1.0f ) :
				fFinalDamage * ( pAttacker->GetAdditionalFinalDamageMultiplierRangeAttack() - 1.0f ) );
		
		fFinalDamage += fIncreaseDamage;	/// 양수값

		if( fFinalDamage <= 1.f )
			fFinalDamage = 1.f;

		return fFinalDamage;
	}
#endif

#ifdef ENCHANT_BALANCE_101014
	float fDamageRateByDefencePowerRate = 1.f - fDefencePowerRate * fFronzenDebuffDefenceRate * fDefenceRateBySkill - fShockDebuffDefenceRate;
	if(fDamageRateByDefencePowerRate < 0.f)
		fDamageRateByDefencePowerRate = 0.f;
#else
	float fDamageRateByDefencePowerRate = 1.f - fDefencePowerRate * fFronzenDebuffDefenceRate * fDefenceRateBySkill;
#endif ENCHANT_BALANCE_101014
		
	float fFinalDamage = 0.f;
	// 공격자의 모션상태에 따라 지정된 공격력 배율
	float fAttackerPowerRateByMotion = 0.f;
//{{ kimhc // 2010.8.2 // 스탯을 제외한 무기만의 데미지
	switch( pAttDamageData->damageType )
	{
	case CX2DamageManager::DT_PHYSIC:
	case CX2DamageManager::DT_WEAPON_PHYSIC:
		{
			fAttackerPowerRateByMotion += fAttackPower * fAttackerPhysicDamage;
		} break;

	case CX2DamageManager::DT_MAGIC:
	case CX2DamageManager::DT_WEAPON_MAGIC:
		{
			fAttackerPowerRateByMotion += fAttackPower * fAttackerMagicDamage;

		} break;
	case CX2DamageManager::DT_MIX:
	case CX2DamageManager::DT_WEAPON_MIX:
		{			
			fAttackerPowerRateByMotion 
				+= 0.5f * ( fAttackPowerPhysic * fAttackerPhysicDamage + fAttackPowerMagic * fAttackerMagicDamage );
		}
		break;
	}
//}} kimhc // 2010.8.2 // 스탯을 제외한 무기만의 데미지
	
	
	// 1. 기본적인 데미지 계산 
	fFinalDamage = fAttackerPowerRateByMotion * fDamageRateByDefencePowerRate;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	//엘소드 로드 나이트 강렬한 일격
	if( pAttacker->GetGameUnitType() == GUT_USER )
	{
		CX2GUUser* pUser = static_cast<CX2GUUser*>(pAttacker);
		if( NULL != pUser && CX2Unit::UC_ELSWORD_LORD_KNIGHT == pUser->GetUnitClass() )
		{
			if( CX2DamageManager::WST_VIGOR_ACTIVE == pAttDamageData->m_WayofSwordType || 
				CX2DamageManager::WST_DESTRUCTION_ACTIVE == pAttDamageData->m_WayofSwordType ) 
				fFinalDamage = pAttacker->GetFinalDamageToBeChangedByActiveSkill( fFinalDamage );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

	// 2. 던전게임이면 dungeon rate를 곱해준다
	if( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD )
	{
		fFinalDamage *= pAttDamageData->m_fDungeonRate;
	}

#ifdef SKILL_BALANCE_20110728
	// 2.1 대전게임이면 PVP rate를 곱해준다
	if( g_pX2Game->GetGameType() == CX2Game::GT_PVP )
	{
		fFinalDamage *= pAttDamageData->m_fPvpRate;
	}
#endif

#ifdef ELSWORD_WAY_OF_SWORD
	// 2.2 검의길에 따른 damage증가
	ASSERT( GetFinalDamage_WayOfSword(pAttDamageData->m_WayofSwordType) > 0.f );
	fFinalDamage *= pAttacker->GetFinalDamage_WayOfSword( pAttDamageData->m_WayofSwordType );
#endif ELSWORD_WAY_OF_SWORD

	// 3. 각성했다면 1.2배
	//if( pAttacker->GetRemainHyperModeTime() > 0.f )
	const float fDecreaseDamage = fFinalDamage * ( pDefender->GetAdditionalFinalDefenceMultiplier() - 1.0f );
		const float fIncreaseDamage 
			= ( pAttDamageData->m_bMeleeAttack ? 
				fFinalDamage * ( pAttacker->GetAdditionalFinalDamageMultiplierMeleeAttack() - 1.0f ) :
				fFinalDamage * ( pAttacker->GetAdditionalFinalDamageMultiplierRangeAttack() - 1.0f ) );

		fFinalDamage += fDecreaseDamage;	/// 음수값
		fFinalDamage += fIncreaseDamage;	/// 양수값

	// 3.1 방어상태이면 데미지를 %만큼 줄여준다
	float fDamageReduceRate = 0.f;
	float fFinalDamageBeforeReduce = fFinalDamage;	// 데미지 감소 이전의 데미지를 저장

#ifdef UPGRADE_RAVEN
	if( pAttDamageData->m_bPenetrate == false && pAttDamageData->m_bIgnoreStateDefence == false )
#else
	if( pAttDamageData->m_bPenetrate == false )
#endif
	{
		if( m_DamageReducePercent > 0 )
		{
			fDamageReduceRate = (float)m_DamageReducePercent;
		}

		if( pAttDamageData->m_bGuard == true )
		{
			fDamageReduceRate += m_DamageGuardPercent;
		}
	}


#ifdef CHUNG_SECOND_CLASS_CHANGE
	// oasis907 : 김상윤 [2011.7.6] 청가드, 굳건한의지등, m_DamageReducePercent 중첩으로 인한 오동작 방지
	if(fDamageReduceRate > 100.f)
		fDamageReduceRate = 100.f;
#endif CHUNG_SECOND_CLASS_CHANGE
	fDamageReduceRate = ( 100.f - fDamageReduceRate ) / 100.f;
	fFinalDamage *= fDamageReduceRate;

#ifdef DAMAGE_DOUBLE_AT_DOWN_STATE
	// 3.2 다운 상태의 적 공격시 데미지 증가
	if( GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		CX2GUNPC* pGUNPC = (CX2GUNPC*)this;
		if( true == pGUNPC->GetIsFallDowning() )
		{

			float fDamageUpRateAtDownState = pAttacker->GetDamageUpByAMADS() + 1.f;
			if( 0.f != fDamageUpRateAtDownState )
			{
				fFinalDamage *= fDamageUpRateAtDownState;
			}

			// kimhc // 2010-11-23 // 다운상태의 적을 공격하여 데미지가 증가될 시에
			// 해당 데미지가 기본 데미지(서있는 상태에서 때린 데미지) 보다 크면
			// 기본 데미지로 보정한다.
			if ( fFinalDamageBeforeReduce < fFinalDamage )
				fFinalDamage = fFinalDamageBeforeReduce;

		}
	}
#endif DAMAGE_DOUBLE_AT_DOWN_STATE


	//3.3 특정 몬스터에 한해서 데미지가 더 올라가는 경우.. 유저가 몬스터를 때렸을 경우만 생각해보자..
	if( GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		CX2GUNPC* pGUNPC = (CX2GUNPC*)this;
//		switch( pAttDamageData->attackerType )
//		{
//		case CX2DamageManager::AT_UNIT:
//		case CX2DamageManager::AT_EFFECT:
//			{
				float fIncreasePercent = pAttacker->GetDamageUpPercentBySMA( pGUNPC->GetNPCTemplet().m_nNPCUnitID );
				fFinalDamage += fFinalDamage * fIncreasePercent;
		//	} break;
		//}
	}



	// 4. 대전게임이라면 팀밸런스 적용
#ifndef PVP_SEASON2 // 서버 버프 형태로 변경
	if( g_pX2Game->GetGameType() == CX2Game::GT_PVP )
	{
		switch( GetTeam() )
		{
		case CX2Room::TN_RED:
			{
				int factor = g_pX2Game->GetRedTeamKill() - g_pX2Game->GetBlueTeamKill();
				if( factor < -5  )
					factor = -5;
				else if( factor > 10  )
					factor = 10;
				fFinalDamage += fFinalDamage * factor * 0.1f;
			} break;
		case CX2Room::TN_BLUE:
			{
				int factor = g_pX2Game->GetBlueTeamKill() - g_pX2Game->GetRedTeamKill();
				if( factor < -5  )
					factor = -5;
				else if( factor > 10  )
					factor = 10;
				fFinalDamage += fFinalDamage * factor * 0.1f;
			} break;
		}
	}
#endif

	// 5. 피어싱 공격일 경우에 추가 데미지, 무기 공격력의 0.3배
	if( true == pAttDamageData->m_bPenetrate )
	{
		float fPiercingDamage = 0.f;
		switch( pAttacker->GetGameUnitType() )
		{
		case CX2GameUnit::GUT_USER:
			{
				CX2GUUser* pGUUSer = (CX2GUUser*)pAttacker;
				CX2Item* pItem = pGUUSer->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
				fPiercingDamage = GetItemAttackPower( pAttacker, pItem );

			} break;

		case CX2GameUnit::GUT_NPC:
			{
				fPiercingDamage = fPhysicAttack + fMagicAttack;

			} break;
		}
#ifdef ENCHANT_BALANCE_101014
		fPiercingDamage *= 0.4f;
#else
		fPiercingDamage *= 0.3f;
#endif ENCHANT_BALANCE_101014
		fFinalDamage += fPiercingDamage;
	}

	// SocketOption에 의한 SuperArmor 시에 데미지 30% 감소
	if ( pDefender->GetGameUnitType() == GUT_USER )
	{
		const CX2GUUser* pUser = static_cast<CX2GUUser*>( pDefender );
		if ( pUser->GetTimeSocketSuperArmor() > 0.0f )
			fFinalDamage *= 0.7f;
	}


#ifdef RENA_SECOND_CLASS_CHANGE
	// 레나 낙인의 사격에 피해를 받은 상태인 경우에 받는 데미지가 증가한다
	if( m_ExtraDamagePack.m_StigmaDebuff.m_fTime > 0.f )
	{
		fFinalDamage *= m_ExtraDamagePack.m_StigmaDebuff.m_Damage;	// 일반 콤보 데미지 증가 비율을 곱한다
	}
#endif RENA_SECOND_CLASS_CHANGE

	//{{ JHKang / 강정훈 / 2010/11/29 / 아토믹 쉴드 데미지 감소
#ifdef NEW_SKILL_2010_11
	if ( m_fAtomicShieldDamageRate < 1.f )
	{
#ifdef SKILL_BALANCE_20110728
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_BATTLE_FIELD:
			{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 기존 던/대전에서의 데미지 감소율 기존 20% 증가 ->  50% 증가
				fFinalDamage *= m_fAtomicShieldDamageRate - (1.f - m_fAtomicShieldDamageRate) * 0.5f;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				fFinalDamage *= m_fAtomicShieldDamageRate - (1.f - m_fAtomicShieldDamageRate) * 0.2f;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

			} break;
		default:
			{
				fFinalDamage *= m_fAtomicShieldDamageRate;
			} break;
		}
#else
		fFinalDamage *= m_fAtomicShieldDamageRate;
#endif
	}
#endif NEW_SKILL_2010_11
	//}} JHKang / 강정훈 / 2010/11/29 / 아토믹 쉴드 데미지 감소


#ifdef NEW_HENIR_TEST

#ifndef ADD_HENIR_BUFF		/// 버프로 변경되면서 쓰이지 않게 된 구문
	if( m_fTimeHenirBuff_Nature > 0.f)
	{
		const float MAGIC_DAMAGE_REDUCE_RATE_HENIR_BUFF_NATURE = 0.5f;
		fFinalDamage *= MAGIC_DAMAGE_REDUCE_RATE_HENIR_BUFF_NATURE;
	}
	if( pAttacker->GetTimeHenirBuffLight() > 0.f)
	{
		const float MAGIC_DAMAGE_RATE_HENIR_BUFF_LIGHT = 1.1f;
		fFinalDamage *= MAGIC_DAMAGE_RATE_HENIR_BUFF_LIGHT;
	}
#endif ADD_HENIR_BUFF

#endif NEW_HENIR_TEST

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE		/// 데미지 데이터 파싱하는 부분에서는 데미지 배율을 조작하는 동작을 하지 않기에, GetFinalDamage로 이동
	fFinalDamage *= GetAdditionalAttackPowerRateByType( pAttDamageData );		/// 특정 공격에 따른 데미지 배율 설정 ( AT_SPECIAL, AT_NORMAL )
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

	fFinalDamage = pDefender->GetFinalDamageToBeChangedBySkill( fFinalDamage );

#ifdef SERV_9TH_NEW_CHARACTER
	// 오현빈 // 공격자 스킬에 의한 최종 데미지 변경 처리
	{
		const float fAtkDamageRateBySkill = pAttacker->GetChangeDamageRateByAttackerSkill( pAttDamageData );
		fFinalDamage *= fAtkDamageRateBySkill;
	}
#endif // SERV_9TH_NEW_CHARACTER

	// 최종데미지가 0보다 작다면 0으로 보정
	if( fFinalDamage <= 0.f )
		fFinalDamage = 0.f;
	
#ifdef APPLY_THEMIDA
	ENCODE_END
#endif


	// 크리티컬인 경우 데미지를 2배 해주는 코드는 이 함수 밖에 있다

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif

	return fFinalDamage;
}

void    CX2GameUnit::InitDeviceData::AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const
{
 	BOOST_TEST_FOREACH( const wstring&, strTexture, m_vecstrReadyTexture )
 	{
 		g_pKTDXApp->GetDeviceManager()->AppendTexture( listInOut_, strTexture );
 	}//BOOST_FOREACH()
 
 	//BOOST_TEST_FOREACH( const wstring&, strSound, m_vecstrReadySound )
 	//{
 	//    CX2UnitLoader::AppendSound( listInOut_, strSound.c_str() );
 	//}//BOOST_FOREACH()
 
 	BOOST_TEST_FOREACH( const wstring&, strXSkinMesh, m_vecstrReadyXSkinMesh )
 	{
 		g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, strXSkinMesh );
 	}//BOOST_FOREACH()
 
 	BOOST_TEST_FOREACH( const wstring&, strXMesh, m_vecstrReadyXMesh )
 	{
 		g_pKTDXApp->GetDeviceManager()->AppendXMesh( listInOut_, strXMesh );
 	}//BOOST_FOREACH()

	BOOST_TEST_FOREACH( const wstring&, strXET, m_vecstrReadyXET )
	{
		g_pKTDXApp->GetDeviceManager()->AppendXET( listInOut_, strXET );
	}

	if ( g_pData->GetGameMajorParticle() != NULL )
	{
 		BOOST_TEST_FOREACH( const wstring&, strParticle, m_vecstrReadyGameMajorParticle )
 		{
 			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, strParticle );
 		}//BOOST_FOREACH()
	}

	if ( g_pData->GetGameMinorParticle() != NULL )
	{
 		BOOST_TEST_FOREACH( const wstring&, strParticle, m_vecstrReadyGameMinorParticle )
 		{
 			g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, strParticle );
 		}//BOOST_FOREACH()
	}

}//CX2GameUnit::InitDeviceData::AppendToDeviceList()

/*static*/
void    CX2GameUnit::InitDevice( InitDeviceData& OutDevice_, KLuaManager& luaManager_ )
{
	wstring wstrFileName;

	if( luaManager_.BeginTable( "INIT_DEVICE" ) == true )
	{
		OutDevice_.m_vecstrReadyTexture.resize( 0 );
		if( luaManager_.BeginTable( "READY_TEXTURE" ) == true )
		{
			//OutDevice_.m_vecstrReadyTexture.reserve( luaManager_. );

			int index = 1; 
			while( luaManager_.GetValue( index, wstrFileName ) == true )
			{
				OutDevice_.m_vecstrReadyTexture.push_back( wstrFileName );
				index++;
			}
			luaManager_.EndTable();
		}

		OutDevice_.m_vecstrReadyXMesh.resize( 0 );
		if( luaManager_.BeginTable( "READY_XMESH" ) == true )
		{
			//OutDevice_.m_vecstrReadyXMesh.reserve( luaManager_.GetNumIndexedValues() );

			int index = 1; 
			while( luaManager_.GetValue( index, wstrFileName ) == true )
			{
				OutDevice_.m_vecstrReadyXMesh.push_back( wstrFileName );
				index++;
			}
			luaManager_.EndTable();
		}

		OutDevice_.m_vecstrReadyXSkinMesh.resize( 0 );
		if( luaManager_.BeginTable( "READY_XSKIN_MESH" ) == true )
		{
			//OutDevice_.m_vecstrReadyXSkinMesh.reserve( luaManager_.GetNumIndexedValues() );

			int index = 1; 
			while( luaManager_.GetValue( index, wstrFileName ) == true )
			{
				OutDevice_.m_vecstrReadyXSkinMesh.push_back( wstrFileName );
				index++;
			}
			luaManager_.EndTable();
		}

		OutDevice_.m_vecstrReadySound.resize( 0 );
		if( luaManager_.BeginTable( "READY_SOUND" ) == true )
		{
			//OutDevice_.m_vecstrReadySound.reserve( luaManager_.GetNumIndexedValues() );
			int index = 1; 
			while( luaManager_.GetValue( index, wstrFileName ) == true )
			{
				OutDevice_.m_vecstrReadySound.push_back( wstrFileName );
				index++;
			}
			luaManager_.EndTable();
		}
		luaManager_.EndTable(); // INIT_DEVICE
	}	
}//CX2GameUnit::InitDevice()


/** @function InitDevice
	@brief 유닛 관련 TEXTURE, XMESH, SKINMESH, SOUND Pre-Loading
*/
void CX2GameUnit::InitDevice()
{
 	wstring fileName = L"";
 
 	if( m_LuaManager.BeginTable( "INIT_DEVICE" ) == true )
 	{
 		if( m_LuaManager.BeginTable( "READY_TEXTURE" ) == true )
 		{
 			wstring texName;
 			int index = 1; 
 			while( m_LuaManager.GetValue( index, texName ) == true )
 			{
 				TextureReadyInBackground( texName.c_str() );
 				index++;
 			}
 			m_LuaManager.EndTable();
 		}
 
 		if( m_LuaManager.BeginTable( "READY_XMESH" ) == true )
 		{
 			wstring xmeshName;
 			int index = 1; 
 			while( m_LuaManager.GetValue( index, xmeshName ) == true )
 			{
 				XMeshReadyInBackground( xmeshName.c_str() );
 				index++;
 			}
 			m_LuaManager.EndTable();
 		}
 
 
 		if( m_LuaManager.BeginTable( "READY_XSKIN_MESH" ) == true )
 		{
 			wstring xskinmeshName;
 			int index = 1; 
 			while( m_LuaManager.GetValue( index, xskinmeshName ) == true )
 			{
 				XSkinMeshReadyInBackground( xskinmeshName.c_str() );
 				index++;
 			}
 			m_LuaManager.EndTable();
 		}
 
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
 		if( m_LuaManager.BeginTable( "READY_SOUND" ) == true )
 		{
 			wstring soundName;
 			int index = 1; 
 			while( m_LuaManager.GetValue( index, soundName ) == true )
 			{
 				SoundReadyInBackground( soundName.c_str() );
 				index++;
 			}
 			m_LuaManager.EndTable();
 		}
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
 
 		m_LuaManager.EndTable(); // INIT_DEVICE
 	}
}

void CX2GameUnit::InitSystem()
{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	if ( m_ScriptFileName.empty() == false )
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	{
		//스크립트 파일을 로드한다
	//{{ robobeg : 2008-10-28
		g_pKTDXApp->LoadLuaTinker( m_ScriptFileName.c_str() );
		//g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, L"Enum.lua" );
		//g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, L"Unit_Common_Device.lua" );
		if ( g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, m_ScriptFileName.c_str() ) == false )
		{
			ASSERT( !"gameunit lua script Parsing failed" );
		}
	//}} robobeg : 2008-10-28
	}

	m_DamagedMap.clear();


	if( m_LuaManager.BeginTable( "INIT_SYSTEM" ) == true )
	{
//{{ robobeg : 2008-10-17
		//m_LuaManager.GetValue( "UNIT_LAYER", m_Layer );
        int iLayer = 0;
		m_LuaManager.GetValue( "UNIT_LAYER", iLayer );
        SetLayer( iLayer );
//}} robobeg : 2008-10-17

		LUA_GET_VALUE_ENUM( m_LuaManager, "RENDER_PARAM", m_stOriginBuffRenderParam.m_eRenderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_CARTOON_BLACK_EDGE );
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		m_stOriginBuffRenderParam.m_fOutLineWide	= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		m_stOriginBuffRenderParam.m_fOutLineWide	= 1.7f;
#endif //UNIT_SCALE_COMBINE_ONE
		
		if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
			m_RenderParam.lightPos	= g_pX2Game->GetWorld()->GetLightPos();

		//{{ JHKang / 강정훈 / 2011/01/13 / 전체 렌더 타입 변경 확인(유지할 렌더 타입이 지정)
#ifdef SEASON3_MONSTER_2010_12
		m_BasicRenderType = m_RenderParam.renderType;
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / 강정훈 / 2011/01/13

		LUA_GET_VALUE( m_LuaManager, "ALPHA_BLEND", m_stOriginBuffRenderParam.m_bAlphaBlend, false );
		SetAlphaObject( m_stOriginBuffRenderParam.m_bAlphaBlend );

#ifdef SERV_HALLOWEEN_EVENT_2013 //JHKang
		D3DBLEND srcBlend = D3DBLEND_SRCALPHA;
		D3DBLEND destBlend = D3DBLEND_INVSRCALPHA;
		LUA_GET_VALUE_ENUM( m_LuaManager, "SRC_BLEND", srcBlend, D3DBLEND, D3DBLEND_SRCALPHA );
		LUA_GET_VALUE_ENUM( m_LuaManager, "DEST_BLEND", destBlend, D3DBLEND, D3DBLEND_INVSRCALPHA );
		m_RenderParam.srcBlend = srcBlend;
		m_RenderParam.destBlend = destBlend;
#endif //HALLOWEEN_EVENT_2013

		bool bCullMode;
		LUA_GET_VALUE( m_LuaManager, "CULL_MODE", bCullMode, true );
		if( bCullMode == false )
		{
			m_RenderParam.cullMode = D3DCULL_NONE;
		}
		LUA_GET_VALUE( m_LuaManager, "Z_ENABLE", m_RenderParam.bZEnable, true );
		m_RenderParam.bZWriteEnable = m_RenderParam.bZEnable;			// fix!! 일단 요렇게


		m_LuaManager.EndTable();
	}
}

void CX2GameUnit::InitStat()
{
}

/*virtual*/ void CX2GameUnit::InitPhysic()
{
	if( m_LuaManager.BeginTable( "INIT_PHYSIC" ) == true )
	{
		m_LuaManager.GetValue( "RELOAD_ACCEL",		m_OrgPhysicParam.fReloadAccel );
		m_LuaManager.GetValue( "G_ACCEL",			m_OrgPhysicParam.fGAccel );
		m_LuaManager.GetValue( "MAX_G_SPEED",		m_OrgPhysicParam.fMaxGSpeed );

		m_LuaManager.GetValue( "WALK_SPEED",		m_OrgPhysicParam.fWalkSpeed );
		m_LuaManager.GetValue( "RUN_SPEED",			m_OrgPhysicParam.fRunSpeed );
		m_LuaManager.GetValue( "JUMP_SPEED",		m_OrgPhysicParam.fJumpSpeed );
		m_LuaManager.GetValue( "DASH_JUMP_SPEED",	m_OrgPhysicParam.fDashJumpSpeed );

		LUA_GET_VALUE( m_LuaManager, "IGNORE_LINE_SPEED",	m_bIgnoreLineSpeed,		 false );

		m_LuaManager.EndTable();
	}

	ResetPhysicParam();
	//m_ScriptedPhysicParam = m_OrgPhysicParam;
}


/*virtual*/ void CX2GameUnit::ResetPhysicParam( bool bPreserveNowSpeed /*= false */ )
{
	m_PhysicParam	= m_OrgPhysicParam;

#ifdef RIDING_SYSTEM
	/// 탈것에 탑승했을 때, 물리 설정값 갱신
	if ( NULL != m_ptrRidingPet && true == GetRidingOn() )
	{
		m_PhysicParam.fWalkSpeed = m_ptrRidingPet->GetPhysicParam().fWalkSpeed;
		m_PhysicParam.fRunSpeed= m_ptrRidingPet->GetPhysicParam().fRunSpeed;
		m_PhysicParam.fJumpSpeed = m_ptrRidingPet->GetPhysicParam().fJumpSpeed;
		m_PhysicParam.fDashJumpSpeed = m_ptrRidingPet->GetPhysicParam().fDashJumpSpeed;
	}

	InitAddtionalDashAndJumpBasicStatValue();
#endif //RIDING_SYSTEM

	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetWorld() &&
		1.f != g_pX2Game->GetWorld()->GetGravityAndSpeedRate() )
	{
		const float fScale = g_pX2Game->GetWorld()->GetGravityAndSpeedRate();
		float MAGIC_GRAVITY_SCALE = 0.5f;
		if( fScale > 1.f )
		{
			MAGIC_GRAVITY_SCALE = 1.5f;
		}

		m_PhysicParam.fReloadAccel        	*= fScale;
		m_PhysicParam.fGAccel             	*= fScale * MAGIC_GRAVITY_SCALE;
		m_PhysicParam.fMaxGSpeed          	*= fScale * MAGIC_GRAVITY_SCALE;

		m_PhysicParam.fWalkSpeed          	*= fScale;
		m_PhysicParam.fRunSpeed           	*= fScale;
		m_PhysicParam.fJumpSpeed          	*= fScale;
		m_PhysicParam.fDashJumpSpeed      	*= fScale;

	}
}

/*virtual*/ void CX2GameUnit::SetSmallGage()
{
	wstring wstrSmallHPbar;

	if ( g_pData->GetPVPRoom() != NULL && g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
	{
		if( NULL != g_pX2Game->GetMyUnit() &&
			g_pX2Game->GetMyUnit()->GetTeam() == this->GetTeam() )
		{
			LUA_GET_VALUE( m_LuaManager, "SMALL_HP_BAR_RED", wstrSmallHPbar, L"" );
		}
		else
		{
			LUA_GET_VALUE( m_LuaManager, "SMALL_HP_BAR_BLUE", wstrSmallHPbar, L"" );
		}
	}
	else
	{
		if( GetTeam() == CX2Room::TN_BLUE )
		{
			LUA_GET_VALUE( m_LuaManager, "SMALL_HP_BAR_BLUE", wstrSmallHPbar, L"" );
		}
		else if( m_TeamNum == CX2Room::TN_RED )
		{
			LUA_GET_VALUE( m_LuaManager, "SMALL_HP_BAR_RED", wstrSmallHPbar, L"" );
		}
		else	// 몬스터의 경우
		{
			LUA_GET_VALUE( m_LuaManager, "SMALL_HP_BAR_YELLOW", wstrSmallHPbar, L"" );
		}
	}

	if ( false == wstrSmallHPbar.empty() )
		SetHpTexture( wstrSmallHPbar.c_str() );

// 	wstring wstrSmallMPBar;
// 	LUA_GET_VALUE( m_LuaManager,	"SMALL_MP_BAR",		wstrSmallMPBar,		L"Small_HP_bar_Yellow.TGA" );
// 	if( false == wstrSmallMPBar.empty() )
// 		SetMpTexture( wstrSmallMPBar.c_str() );

}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GameUnit::InitComponent( InitDeviceData& OutDevice_, KLuaManager& luaManager_ )
{
	if( luaManager_.BeginTable( "INIT_COMPONENT" ) == true )
	{
		wstring strName;
		if ( luaManager_.GetValue( "QUESTION_MARK_SEQ", strName ) == true )
			OutDevice_.m_vecstrReadyGameMinorParticle.push_back( strName );
		if ( luaManager_.GetValue( "EXCLAMATION_MARK_SEQ", strName ) == true )
			OutDevice_.m_vecstrReadyGameMinorParticle.push_back( strName );
		if ( luaManager_.GetValue( "SHADOW_FILE_NAME", strName ) == true )
			OutDevice_.m_vecstrReadyTexture.push_back( strName );
		luaManager_.EndTable();
	}
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GameUnit::InitComponent()
{
	if( m_LuaManager.BeginTable( "INIT_COMPONENT" ) == true )
	{
		if ( NULL == m_pGageData)
			m_pGageData = CreateGageData();

		if ( IsMyUnit() == true )
		{
			CX2GageManager* pGageManager = CX2GageManager::GetInstance();
			pGageManager->SetMyGameUnit( this );
			m_pGageUI = CreateGageUI( m_pGageData );
		}
		else
		{
			m_pGageUI = CreateGageUI( m_pGageData );
			SetSmallGage();			
		}

		if ( NULL == IsNullGageUI() )
			m_pGageUI->InitUI();

		wstring seqName;
		LUA_GET_VALUE( m_LuaManager, "QUESTION_MARK_SEQ", seqName, L"" );
		if( seqName.length() > 0 )
		{
			if ( m_hQuestionMark == INVALID_PARTICLE_SEQUENCE_HANDLE )
				m_hQuestionMark		= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  seqName.c_str(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
		}		
		LUA_GET_VALUE( m_LuaManager, "EXCLAMATION_MARK_SEQ", seqName, L"" );
		if( seqName.length() > 0 )
		{
			if ( m_hExclamationMark == INVALID_PARTICLE_SEQUENCE_HANDLE )
				m_hExclamationMark	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  seqName.c_str(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
		}

		LUA_GET_VALUE( m_LuaManager, "HEAD_BONE_NAME",	m_HeadBoneName, L"Bip01_Head" );

		CKTDGParticleSystem::CParticleEventSequence* pSeqQuestion = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hQuestionMark );
		if( pSeqQuestion != NULL )			
			pSeqQuestion->SetTrace( true );

		CKTDGParticleSystem::CParticleEventSequence* pSeqExclamation = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hExclamationMark );
		if( pSeqExclamation != NULL )
			pSeqExclamation->SetTrace( true );

		LUA_GET_VALUE_ENUM( m_LuaManager, "HITTED_TYPE", m_HittedType,		CX2DamageManager::HITTED_TYPE,		CX2DamageManager::HTD_DEFAULT );

		bool bLoadCompleteShadow = true;
		LUA_GET_VALUE_RETURN( m_LuaManager, "SHADOW_FILE_NAME", m_ShadowFileName,	L"",	bLoadCompleteShadow = false; );
		LUA_GET_VALUE( m_LuaManager, "SHADOW_SIZE",		 m_fShadowSize,		0.0f );       
		if( true == bLoadCompleteShadow )
            SetShadow( m_ShadowFileName.c_str(), m_fShadowSize );


		int iBuffer = 0;
		float fBuffer = 0.0f;
		//LUA_GET_VALUE( m_LuaManager, L"HYPER_MODE_COUNT", iBuffer, 1 );
		//m_FrameDataFuture.syncData.m_HyperModeCount = iBuffer;
		LUA_GET_VALUE( m_LuaManager, "MAX_HYPER_MODE_TIME", fBuffer, 30.0f );
		SetMaxHyperModeTime( fBuffer );



//#ifdef HEAD_INVERSE_KINEMATICS_TEST
//		LUA_GET_VALUE( m_LuaManager, "HEAD_IK_ANGLE_CHANGE_RATE",	m_fHeadIKAngleChangeRate,	1.f );
//		LUA_GET_VALUE( m_LuaManager, "HEAD_IK_MAX_ANGLE",			m_fHeadIKAngleMax,			45.f );
//		m_fHeadIKAngleMax *= D3DX_PI / 180.f;
//
//		LUA_GET_VALUE( m_LuaManager, "HEAD_IK_RADIUS",				m_fHeadIKRadius,			2000.f );
//#endif HEAD_INVERSE_KINEMATICS_TEST
//

//#ifdef STEP_ON_MONSTER_TEST
//		LUA_GET_VALUE( m_LuaManager, "CAN_BE_STEPPED_ON",			m_bCanBeSteppedOn,			false );
//#endif STEP_ON_MONSTER_TEST



//#ifdef STEP_ON_MONSTER_COLLISION_BOX_TEST
//		LUA_GET_VALUE( m_LuaManager, "CAN_BE_STEPPED_ON_COLLISION_BOX",			m_bCanBeSteppedOnCollsionBox,			false );
//#endif STEP_ON_MONSTER_COLLISION_BOX_TEST


//#ifdef HEAD_ATTACHED_CAMERA_TEST
//
//		LUA_GET_VALUE( m_LuaManager, "BONE_ATTACHED_CAMERA_OFFSET_X",				m_vBoneAttachedCameraOffset.x,			0.f );
//		LUA_GET_VALUE( m_LuaManager, "BONE_ATTACHED_CAMERA_OFFSET_Y",				m_vBoneAttachedCameraOffset.y,			0.f );
//		LUA_GET_VALUE( m_LuaManager, "BONE_ATTACHED_CAMERA_OFFSET_Z",				m_vBoneAttachedCameraOffset.z,			0.f );
//		
//#endif HEAD_ATTACHED_CAMERA_TEST


		m_LuaManager.EndTable();
	}


	m_fGroggyTime = 0.0f;

}


void CX2GameUnit::SetDamageData_LUA( const char* szTableNameUTF8 )
{
	g_pData->GetDamageManager()->SetDamageDataFromLUA( &m_DamageData, m_LuaManager, szTableNameUTF8 );
}

void CX2GameUnit::Verify()
{
	if ( IsMyUnit() )
	{
		if ( g_pInstanceData != NULL 
			&& g_pInstanceData->GetVerifyGageManagerTimer() <= 0.f )
		{
#ifdef USER_STAT_VERIFY
			if ( GetStat().Verify() == false )
			{
				if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
					g_pKTDXApp->GetFindHacking() == false )
				{
					g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				}

#ifdef ADD_COLLECT_CLIENT_INFO
				g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_07, ANTI_HACK_STRING_AntiHacking_Stat_Verify, true, true );
				g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );
#else
				g_pMain->SendHackMail_DamageHistory( ANTI_HACK_STRING_USER_STAT_VERIFY );
				g_pKTDXApp->SetFindHacking( true );
#endif
			}
#endif

#ifdef VERIFY_STAT_BY_BUFF
			if ( VerifyStatByBuff() == false 
				|| VerifyScaleByUnit() == false
				|| m_vBasicScale.Verify() == false
				|| m_vTransScale.Verify() == false 
				|| VerifyVector<CX2ChangeAttackByTypePtr>( m_AdditionalAttackByType ) == false
				|| VerifyVector<CX2ChangeEnchantAttackRatePtr>( m_AdditionalChangeEnchantAttackRate ) == false
				|| VerifyVector<CX2DoubleAttackPtr>( m_vecDoubleAttack ) == false
				|| VerifyVector<CX2ChangeUnitScaleByBuffPtr>( m_vecChangeUnitScaleByBuffPtr ) == false
				|| VerifyVector<CX2AddSkillLevelByBuffPtr>( m_vecAddSkillLevelByBuffPtr ) == false
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				|| VerifyVector<CX2ReflectMagicByBuffPtr>( m_vecReflectMagicByBuffPtr ) == false
#endif // SERV_ARA_CHANGE_CLASS_SECOND
				)
			{
				if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
					g_pKTDXApp->GetFindHacking() == false )
				{
					g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				}

#ifdef ADD_COLLECT_CLIENT_INFO
				g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_07, ANTI_HACK_STRING_AntiHacking_Buff_Verify, true, true );
				g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );
#endif	// ADD_COLLECT_CLIENT_INFO

				//g_pKTDXApp->SetFindHacking( true );
			}
#endif // VERIFY_STAT_BY_BUFF

			if( m_TeamNum.Verify()					== false 
				|| VerifyMaxHyperModeTime()		== false 
				|| VerifyRemainHyperModeTime()		== false
				|| m_fForceInvincibleTime.Verify()	== false
				|| m_fShowInvincibleTime.Verify()	== false
				|| m_ExtraDamagePack.Verify()		== false )
			{
				if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
					g_pKTDXApp->GetFindHacking() == false )
				{
					g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
				}

#ifdef ADD_COLLECT_CLIENT_INFO		// 해외팀 오류 수정
				g_pMain->SendHackInfo5( ANTIHACKING_ID::ANTIHACKING_GAME_07, ANTI_HACK_STRING_AntiHacking_Extra_Info_Verify, true, true );
				g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );
#endif //ADD_COLLECT_CLIENT_INFO
			}
		}
	}
}

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

//{{ kimhc // 2010.12.07 // 신스킬
#ifdef	NEW_SKILL_2010_11
/** @function : LoadImmunityAtThisStateFromScript
	@brief : 현재 스테이트에서만 Immune 시킬 ExtraDamage를 스크립트에서 읽어들이는 함수 
*/
void CX2GameUnit::LoadImmunityAtThisStateFromScript( IN KLuaManager& luaManager_ )
{
	m_vecImmunityAtThisState.resize( 0 );

	if ( true == luaManager_.BeginTable( "IMMUNITY_LIST_AT_THIS_STATE" ) )
	{

		int iIndex = 1;
		UINT eImmuneBuffOrDebuffType = 0;

		while ( luaManager_.GetValue( iIndex, eImmuneBuffOrDebuffType ) )
		{
			m_vecImmunityAtThisState.push_back( static_cast<BUFF_TEMPLET_ID>( eImmuneBuffOrDebuffType ) );
			++iIndex;
		}

		luaManager_.EndTable();
	}
}
#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.12.07 // 신스킬

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


bool CX2GameUnit::ProcessEnchantExtraDamage( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE basicExtraDamageType, CX2DamageManager::EXTRA_DAMAGE_TYPE miscExtraDamageType, 
											float fRandomValue )
{
	// 공격자의 발동 확률 계산
    CX2GameUnit*  pAttackerGameUnit = NULL;
    const CX2EnchantItem::EnchantData*	pWeaponEnchantData = NULL;
	if( NULL != pDamageData &&
		NULL != ( pAttackerGameUnit = pDamageData->optrAttackerGameUnit.GetObservable() ) &&
		NULL != ( pWeaponEnchantData = pAttackerGameUnit->GetWeaponEnchantData( miscExtraDamageType ) ) )
	{
#ifdef PET_AURA_SKILL
		float fRate = pWeaponEnchantData->m_fRate;
#ifdef ENCHANT_ACCUMULATION_RATE_MODIFIER
		fRate = EnchantAccumulationRateModifier(basicExtraDamageType, miscExtraDamageType, fRate);
#endif ENCHANT_ACCUMULATION_RATE_MODIFIER


		fRate += pAttackerGameUnit->GetEnchantAttack( miscExtraDamageType );

#ifdef DAMAGEDATA_RATE_MODIFIER
#ifndef CHUNG_MEMO_01
		if(pDamageData->m_fRateModifier < 1.f)
#endif CHUNG_MEMO_01
			fRate *= pDamageData->m_fRateModifier;
#endif DAMAGEDATA_RATE_MODIFIER

		/// 공격자에게 소켓 속성 공격 확률 증가 버프가 걸려있다면, 적용
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 기존 연산 방식 곱 연산 에서 곱 연산 + 합 연산 으로 변경
		// 인챈트 공격 확률 = ( 인챈트 기본 확률 * 곱 연산 최종 값 ) + 합 연산 최종 값 으로 변경
		EnchantAttackRate sEnchantAttackRate = pAttackerGameUnit->GetChangeEnchantAttackRate( basicExtraDamageType );
		fRate *= sEnchantAttackRate.fIncreaseAttackRate;
		fRate += sEnchantAttackRate.fIncreaseAttackRateFixValue;
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		const float fIncreaseRate = pAttackerGameUnit->GetChangeEnchantAttackRate();
		fRate *= fIncreaseRate;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		if( fRandomValue > fRate )
		{
			return false;
		}
#else

#ifdef DAMAGEDATA_RATE_MODIFIER
		float fRate = pWeaponEnchantData->m_fRate;

#ifndef CHUNG_MEMO_01
		if(pDamageData->m_fRateModifier < 1.f)
#endif CHUNG_MEMO_01
			fRate *= pDamageData->m_fRateModifier;

		if( fRandomValue > fRate )
		{
			return false;
		}
#else
		if( fRandomValue > pWeaponEnchantData->m_fRate )
		{
			return false;
		}
#endif DAMAGEDATA_RATE_MODIFIER
#endif
	}


	switch( basicExtraDamageType )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
		{
			return ProcessEnchantBlaze( pDamageData, miscExtraDamageType );
		} break;
	case CX2DamageManager::EDT_ENCHANT_FROZEN:
		{
			return ProcessEnchantFrozen( pDamageData, miscExtraDamageType );
		} break;
	case CX2DamageManager::EDT_ENCHANT_POISON:
		{
			return ProcessEnchantPoison( pDamageData, miscExtraDamageType );
		} break;
	case CX2DamageManager::EDT_ENCHANT_PIERCING:
		{
			return ProcessEnchantPiercing( pDamageData, miscExtraDamageType );
		} break;
	case CX2DamageManager::EDT_ENCHANT_SHOCK:
		{
			return ProcessEnchantShock( pDamageData, miscExtraDamageType );
		} break;
	case CX2DamageManager::EDT_ENCHANT_SNATCH:
		{
			return ProcessEnchantSnatch( pDamageData, miscExtraDamageType );
		} break;

	default:
		{
			ASSERT( !"impossible" );
			return false;
		} break;
	}
}


bool CX2GameUnit::ProcessEnchantBlaze( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::EXTRA_DAMAGE_TYPE damageType )
{
	if ( IsImmuneToEnchant( CX2EnchantItem::ET_BLAZE ) == true )
	{
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );

		return false;
	}

	const CX2EnchantItem::EnchantData* pEnchantData = NULL;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
    if ( null != pDamageData->optrAttackerGameUnit )
        pEnchantData = pDamageData->optrAttackerGameUnit->GetWeaponEnchantData( damageType );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
	if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetEnchantData( damageType );
	}
	else if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetNPCEnchantData( damageType );
	}
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR

	if ( pEnchantData == NULL )
		return false;

	int finalAccumulation = 0;

	if ( m_ExtraDamagePack.m_EnchantBlaze.m_fTime > 0.0f )
	{
		if ( m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation < 2 )
		{
			finalAccumulation = m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation + 1;
		}
		else
		{
			finalAccumulation = m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation;
		}
	}

	
#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_ExtraDamagePack.Init(CX2DamageManager::EDT_ENCHANT_BLAZE);
#else
	m_ExtraDamagePack.m_EnchantBlaze.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
	m_ExtraDamagePack.m_EnchantBlaze.m_fTime = (float)pEnchantData->m_Time;
#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
    m_ExtraDamagePack.m_EnchantBlaze.m_bAttackedByMyUnit = pDamageData->IsAttackedByMyUnit();
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	
	

	m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation = finalAccumulation;

	if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser* pGUUSer = static_cast<CX2GUUser*>( pDamageData->optrAttackerGameUnit.GetObservable() );
		float fFinalDamage = 0;
		CX2Item* pItem = pGUUSer->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
		if ( pItem != NULL )
		{
#ifdef PVP_SEASON2_SOCKET
			float fAtkPhysic = 0.f;
			float fAtkMagic = 0.f;
			int iAddLevel = 0;
			bool bLevelLinkedStat = false;


            unsigned uNumSO = pItem->GetItemTemplet()->GetNumSocketOption();
			for ( int i = 0; i < (int) uNumSO; ++i )
			{
                int socketOption = pItem->GetItemTemplet()->GetSocketOption(i);
				if( 0 == socketOption )
					continue;

				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
				if( pSocketData == NULL )
					continue;

				if ( NULL != pSocketData && true == pSocketData->m_bIsLevelLinkedStat )
				{
					fAtkPhysic += pSocketData->m_Stat.m_fAtkPhysic;
					fAtkMagic += pSocketData->m_Stat.m_fAtkMagic;
					iAddLevel = pSocketData->m_iAddLevel;
					bLevelLinkedStat = true;
				}
			}

			if ( true == bLevelLinkedStat && g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
				fFinalDamage = ( fAtkPhysic + fAtkMagic ) * ( pGUUSer->GetUnitLevel() + iAddLevel );
			else
            {
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
					// 스탯테이블을 통해 스탯 계산
					CX2Item::KItemFormatStatData kCalculateStat;
					g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
					const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
					const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                fFinalDamage = stat.m_fAtkPhysic + stat.m_fAtkMagic;
            }
#else
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				// 스탯테이블을 통해 스탯 계산
				CX2Item::KItemFormatStatData kCalculateStat;
				g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
				const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
				const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            fFinalDamage = stat.m_fAtkPhysic + stat.m_fAtkMagic;
#endif

			// PVP 자유채널 인첸트 데미지 보정
			if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
			{
				if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_FREE )
				{
					fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
				}
#ifdef TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
				else if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_TOURNAMENT )
				{
					fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
				}
#endif TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
				else
				{
					fFinalDamage *= 0.1f;
				}

			}
			
			else if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME)
			{


#ifdef HENIR_TEST
#ifndef NEW_HENIR_DUNGEON // 1레벨 보정 -> 연동 레벨 던전으로 변경
				if( true == CX2Dungeon::IsHenirDungeon( static_cast<const SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) &&
					CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
				{
					if(	g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
					{
						fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
					}
				}
#endif // NEW_HENIR_DUNGEON
#endif HENIR_TEST

				//{{ 허상형 : [2011/3/29/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
				if( NULL != g_pData->GetDungeonRoom() && g_pData->GetDungeonRoom()->IsDefenceDungeon( (SEnum::DUNGEON_ID)g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) == true )
				{
					if(	g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
					{
						fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
					}
				}
#endif SERV_INSERT_GLOBAL_SERVER
				//}} 허상형 : [2011/3/29/] //	월드 미션


				if(g_pData->GetDungeonRoom() != NULL )
				{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
					if( g_pData->GetDungeonManager()->GetDungeonData( g_pData->GetDungeonRoom()->GetDungeonID() )->m_bDamageEqualized == true )
					{
						fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
					}
					else
#endif SERV_DUNGEON_OPTION_IN_LUA
					// 태완 : 특정 던전에서의 인챈트 보정 코드. NEW_YEAR_EVENT_DUNGEON 등으로 검색해서 나오는 곳에 모두 작업해줘야 한다. (총 4곳)
					// 보정던전이 더 많아진다면 던전데이타 스크립트에 보정여부를 추가하는 방법이 더 나을 것 같다.
					switch(g_pData->GetDungeonRoom()->GetDungeonID())
					{
//{{ 2009.01.20 김태완
#ifdef NEW_YEAR_EVENT_DUNGEON
						// 월리성 새해맞이 던전 인챈트 보정코드

						case SEnum::DI_ELDER_NEWYEAR_NORMAL:
						{
							fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
						} break;
#endif
//}}
#ifdef HALLOWEEN_EVENT
						// 할로윈 던전 관련 임시 코드 : 인챈트 데미지 보정
						case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
						case SEnum::DI_ELDER_HALLOWEEN_HARD:
						case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
						{
							fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
						} break;
#endif
#ifdef CHILDRENS_DAY_EVENT_DUNGEON
						case SEnum::DI_EVENT_KIDDAY_RUBEN:
						case SEnum::DI_EVENT_KIDDAY_ELDER:
						case SEnum::DI_EVENT_KIDDAY_BESMA:
						case SEnum::DI_EVENT_KIDDAY_ALTERA:
						
						case SEnum::DI_EVENT_TREE_DAY_ELDER:
						case SEnum::DI_EVENT_TREE_DAY_BESMA:
						case SEnum::DI_EVENT_TREE_DAY_ALTERA:
						case SEnum::DI_EVENT_TREE_DAY_PEITA:
						case SEnum::DI_EVENT_TREE_DAY_VELDER:
						case SEnum::DI_EVENT_TREE_DAY_HAMEL:
							//{{ oasis907 : 김상윤 [2010.10.21] // 
						case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
						case SEnum::DI_ELDER_HALLOWEEN_HARD:
						case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
							//}} oasis907 : 김상윤 [2010.10.21] // 
						case SEnum::DI_EVENT_VALENTINE_DAY:
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
						case SEnum::DI_EVENT_HALLOWEEN_DAY:
#endif //SERV_HALLOWEEN_EVENT_2013
						{
							fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
						} break;
#endif
						default:
							break;
					}
				}
			}
		}

		if ( m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation == 0 )
		{
			m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = pEnchantData->m_FirstDamagePercent / 100.0f * fFinalDamage ;
		}
		else if ( m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation == 1 )
		{
			m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = pEnchantData->m_SecondDamagePercent / 100.0f * fFinalDamage;
		}
		else if ( m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation >= 2 )
		{
			m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = pEnchantData->m_ThirdDamagePercent / 100.0f * fFinalDamage;
		}

		float fTempDamage = m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec;
		ApplyEnchantResist( CX2EnchantItem::ET_BLAZE, fTempDamage );
		m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = fTempDamage;

	}
	else if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		CX2GUNPC* pGUNpc = static_cast<CX2GUNPC*>( pDamageData->optrAttackerGameUnit.GetObservable() );
		float fFinalDamage = 0;
		
		fFinalDamage = pGUNpc->GetStat().GetStat().m_fAtkPhysic + pGUNpc->GetStat().GetStat().m_fAtkMagic;

		if ( m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation == 0 )
		{
			m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = pEnchantData->m_FirstDamagePercent / 100.0f * fFinalDamage ;
		}
		else if ( m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation == 1 )
		{
			m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = pEnchantData->m_SecondDamagePercent / 100.0f * fFinalDamage;
		}
		else if ( m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation >= 2 )
		{
			m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = pEnchantData->m_ThirdDamagePercent / 100.0f * fFinalDamage;
		}

		float fTempDamage = m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec;
		ApplyEnchantResist( CX2EnchantItem::ET_BLAZE, fTempDamage );
		m_ExtraDamagePack.m_EnchantBlaze.m_DamagePerSec = fTempDamage;
	}


	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"EnchantBlazeAttack", pDamageData->impactPoint );
	UpDownCrashCamera( 20.0f, 0.5f );

	return true;

}

bool CX2GameUnit::ProcessEnchantFrozen( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::EXTRA_DAMAGE_TYPE damageType )
{
	if ( IsImmuneToEnchant( CX2EnchantItem::ET_WATER ) == true )
	{
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );


		return false;
	}

	const CX2EnchantItem::EnchantData* pEnchantData = NULL;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
    if ( null != pDamageData->optrAttackerGameUnit )
        pEnchantData = pDamageData->optrAttackerGameUnit->GetWeaponEnchantData( damageType );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
	if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetEnchantData( damageType );
	}
	else if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetNPCEnchantData( damageType );
	}
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
	if ( pEnchantData == NULL )
		return false;


	int finalAccumulation = 0;

	if ( m_ExtraDamagePack.m_EnchantFrozen.m_fTime > 0.0f )
	{
		if ( m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation < 2 )
		{
			finalAccumulation = m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation + 1;
		}
		else
		{
			finalAccumulation = m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation;
		}
	}

	
	//m_ExtraDamagePack.m_EnchantFrozen = pDamageData->pAttackerUnit->GetWeaponEnchantData()->m_ExtraDamageData;
#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_ExtraDamagePack.Init(CX2DamageManager::EDT_ENCHANT_FROZEN);
#else
	m_ExtraDamagePack.m_EnchantFrozen.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
	m_ExtraDamagePack.m_EnchantFrozen.m_fTime = (float)pEnchantData->m_Time;

	m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation = finalAccumulation;

	return true;
}

bool CX2GameUnit::ProcessEnchantPoison( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::EXTRA_DAMAGE_TYPE damageType )
{
	if ( IsImmuneToEnchant( CX2EnchantItem::ET_NATURE ) == true )
	{
		

		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );

		return false;
	}

	const CX2EnchantItem::EnchantData* pEnchantData = NULL;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
    if ( null != pDamageData->optrAttackerGameUnit )
        pEnchantData = pDamageData->optrAttackerGameUnit->GetWeaponEnchantData( damageType );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
	if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetEnchantData( damageType );
	}
	else if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetNPCEnchantData( damageType );
	}
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
	if ( pEnchantData == NULL )
		return false;

	int finalAccumulation = 0;

	if ( m_ExtraDamagePack.m_EnchantPoison.m_fTime > 0.0f )
	{
		if ( m_ExtraDamagePack.m_EnchantPoison.m_Accumulation < 2 )
		{
			finalAccumulation = m_ExtraDamagePack.m_EnchantPoison.m_Accumulation + 1;
		}
		else
		{
			finalAccumulation = m_ExtraDamagePack.m_EnchantPoison.m_Accumulation;
		}
	}

	

	//m_ExtraDamagePack.m_EnchantPoison = pDamageData->pAttackerUnit->GetWeaponEnchantData()->m_ExtraDamageData;
#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_ExtraDamagePack.Init(CX2DamageManager::EDT_ENCHANT_POISON);
#else
	m_ExtraDamagePack.m_EnchantPoison.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
	m_ExtraDamagePack.m_EnchantPoison.m_fTime = (float)pEnchantData->m_Time;
#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
    m_ExtraDamagePack.m_EnchantPoison.m_bAttackedByMyUnit = pDamageData->IsAttackedByMyUnit();
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

	m_ExtraDamagePack.m_EnchantPoison.m_Accumulation = finalAccumulation;

	if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser* pGUUSer = static_cast<CX2GUUser*>( pDamageData->optrAttackerGameUnit.GetObservable() );
		float fFinalDamage = 0;
		CX2Item* pItem = pGUUSer->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
		if ( pItem != NULL )
		{
#ifdef PVP_SEASON2_SOCKET
			float fAtkPhysic = 0.f;
			float fAtkMagic = 0.f;
			int iAddLevel = 0;
			bool bLevelLinkedStat = false;

            unsigned uNumSO = pItem->GetItemTemplet()->GetNumSocketOption();
			for ( int i = 0; i < (int) uNumSO; ++i )
			{
                int socketOption = pItem->GetItemTemplet()->GetSocketOption(i);
				if( 0 == socketOption )
					continue;

				const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
				if( pSocketData == NULL )
					continue;

				if ( NULL != pSocketData && true == pSocketData->m_bIsLevelLinkedStat )
				{
					fAtkPhysic += pSocketData->m_Stat.m_fAtkPhysic;
					fAtkMagic += pSocketData->m_Stat.m_fAtkMagic;
					iAddLevel = pSocketData->m_iAddLevel;
					bLevelLinkedStat = true;
				}
			}

			if ( true == bLevelLinkedStat && g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
				fFinalDamage = ( fAtkPhysic + fAtkMagic ) * ( pGUUSer->GetUnitLevel() + iAddLevel );
			else
            {
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
					// 스탯테이블을 통해 스탯 계산
					CX2Item::KItemFormatStatData kCalculateStat;
					g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
					const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
					const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
                fFinalDamage = stat.m_fAtkPhysic + stat.m_fAtkMagic;
            }
#else
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				// 스탯테이블을 통해 스탯 계산
				CX2Item::KItemFormatStatData kCalculateStat;
				g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
				const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
				const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            fFinalDamage = stat.m_fAtkPhysic + stat.m_fAtkMagic;
#endif

			// PVP 자유채널 인첸트 데미지 보정
			if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
			{
				if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_FREE )
				{
					fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
				}
#ifdef TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
				else if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_TOURNAMENT )
				{
					fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
				}
#endif TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
				else
				{
					fFinalDamage *= 0.1f;
				}
			}
			else if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME)
			{
#ifdef HENIR_TEST
#ifndef NEW_HENIR_DUNGEON // 1레벨 보정 -> 연동 레벨 던전으로 변경
				if( true == CX2Dungeon::IsHenirDungeon( static_cast<const SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) &&
					CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
				{
					if(	g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
					{
						fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
					}
				}
#endif // NEW_HENIR_DUNGEON
#endif HENIR_TEST

				//{{ 허상형 : [2011/3/29/] //	월드미션
#ifdef SERV_INSERT_GLOBAL_SERVER
				if( NULL != g_pData->GetDungeonRoom() && g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) == true )
				{
					if(	g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
					{
						fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
					}
				}
#endif SERV_INSERT_GLOBAL_SERVER
				//}} 허상형 : [2011/3/29/] //	월드미션



				if(g_pData->GetDungeonRoom() != NULL )
				{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
					if( g_pData->GetDungeonManager()->GetDungeonData( g_pData->GetDungeonRoom()->GetDungeonID() )->m_bDamageEqualized == true )
					{
						fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
					}
					else
#endif SERV_DUNGEON_OPTION_IN_LUA
					// 태완 : 특정 던전에서의 인챈트 보정 코드. NEW_YEAR_EVENT_DUNGEON 등으로 검색해서 나오는 곳에 모두 작업해줘야 한다. (총 4곳)
					// 보정던전이 더 많아진다면 던전데이타 스크립트에 보정여부를 추가하는 방법이 더 나을 것 같다.
					switch(g_pData->GetDungeonRoom()->GetDungeonID())
					{
						//{{ 2009.01.20 김태완
#ifdef NEW_YEAR_EVENT_DUNGEON
						// 월리성 새해맞이 던전 인챈트 보정코드

						case SEnum::DI_ELDER_NEWYEAR_NORMAL:
							{
								fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
							} break;
#endif
							//}}
#ifdef HALLOWEEN_EVENT
						case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
						case SEnum::DI_ELDER_HALLOWEEN_HARD:
						case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
							{
								fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
							} break;
#endif
#ifdef CHILDRENS_DAY_EVENT_DUNGEON
						case SEnum::DI_EVENT_KIDDAY_RUBEN:
						case SEnum::DI_EVENT_KIDDAY_ELDER:
						case SEnum::DI_EVENT_KIDDAY_BESMA:
						case SEnum::DI_EVENT_KIDDAY_ALTERA:
						
						case SEnum::DI_EVENT_TREE_DAY_ELDER:
						case SEnum::DI_EVENT_TREE_DAY_BESMA:
						case SEnum::DI_EVENT_TREE_DAY_ALTERA:
						case SEnum::DI_EVENT_TREE_DAY_PEITA:
						case SEnum::DI_EVENT_TREE_DAY_VELDER:
						case SEnum::DI_EVENT_TREE_DAY_HAMEL:
							//{{ oasis907 : 김상윤 [2010.10.21] // 
						case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
						case SEnum::DI_ELDER_HALLOWEEN_HARD:
						case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
							//}} oasis907 : 김상윤 [2010.10.21] // 
						case SEnum::DI_EVENT_VALENTINE_DAY:
							//{{ 허상형 : [2011/3/3/] //	월드 미션
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
						case SEnum::DI_EVENT_HALLOWEEN_DAY:
#endif //SERV_HALLOWEEN_EVENT_2013
#ifdef SERV_INSERT_GLOBAL_SERVER
						CASE_DEFENCE_DUNGEON
#endif SERV_INSERT_GLOBAL_SERVER
							//}} 허상형 : [2011/3/3/] //	월드 미션
							{
								fFinalDamage = pGUUSer->GetStat().GetStat().m_fAtkPhysic + pGUUSer->GetStat().GetStat().m_fAtkMagic;
							} break;
#endif
						default:
							break;
					}
				}
			}
		}

		if ( m_ExtraDamagePack.m_EnchantPoison.m_Accumulation == 0 )
		{
			m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = pEnchantData->m_FirstDamagePercent / 100.0f * fFinalDamage;
		}
		else if ( m_ExtraDamagePack.m_EnchantPoison.m_Accumulation == 1 )
		{
			m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = pEnchantData->m_SecondDamagePercent / 100.0f * fFinalDamage;
		}
		else if ( m_ExtraDamagePack.m_EnchantPoison.m_Accumulation >= 2 )
		{
			m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = pEnchantData->m_ThirdDamagePercent / 100.0f * fFinalDamage;
		}	

		float fTempDamage = m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec;
		ApplyEnchantResist( CX2EnchantItem::ET_NATURE, fTempDamage );
		m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = fTempDamage;
	}
	else if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		CX2GUNPC* pGUNpc = static_cast<CX2GUNPC*>( pDamageData->optrAttackerGameUnit.GetObservable() );
		float fFinalDamage = 0;

		fFinalDamage = pGUNpc->GetStat().GetStat().m_fAtkPhysic + pGUNpc->GetStat().GetStat().m_fAtkMagic;

		if ( m_ExtraDamagePack.m_EnchantPoison.m_Accumulation == 0 )
		{
			m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = pEnchantData->m_FirstDamagePercent / 100.0f * fFinalDamage ;
		}
		else if ( m_ExtraDamagePack.m_EnchantPoison.m_Accumulation == 1 )
		{
			m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = pEnchantData->m_SecondDamagePercent / 100.0f * fFinalDamage;
		}
		else if ( m_ExtraDamagePack.m_EnchantPoison.m_Accumulation >= 2 )
		{
			m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = pEnchantData->m_ThirdDamagePercent / 100.0f * fFinalDamage;
		}

		float fTempDamage = m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec;
		ApplyEnchantResist( CX2EnchantItem::ET_NATURE, fTempDamage );
		m_ExtraDamagePack.m_EnchantPoison.m_DamagePerSec = fTempDamage;
	}

	return true;
}



bool CX2GameUnit::ProcessEnchantPiercing( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::EXTRA_DAMAGE_TYPE damageType /*= CX2DamageManager::EDT_ENCHANT_PIERCING */ )
{

	//대상의 방어력에 영향을 받지 않고 데미지를 입힌다!!
	//모든 방어 효과 무시 해제 효과를 일으킨다!!

	if ( IsImmuneToEnchant( CX2EnchantItem::ET_WIND ) == true )
	{
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );
		return false;
	}   


	pDamageData->m_bPenetrate = true;

	/*
	D3DXVECTOR3 numPos;
	GetFramePos( &numPos, m_pFrame_Bip01_Head );
	numPos.y += 90.0f;
	if( NULL != g_pData->GetPicCharRed() )
	g_pData->GetPicCharRed()->DrawText( L"Deep", numPos, GetDirVector(), CKTDGPicChar::AT_CENTER );
	*/


	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Lightning_Piercing01", pDamageData->impactPoint );
	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Lightning_Piercing02", pDamageData->impactPoint );


	pDamageData->fCameraCrashGap = 20.0f;
	pDamageData->fCameraCrashTime = 0.3f;

	PlaySound( L"Lightning_PiercingAttack.ogg" );

	return true;

}


bool CX2GameUnit::ProcessEnchantShock( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::EXTRA_DAMAGE_TYPE damageType )
{
	if ( m_ExtraDamagePack.m_EnchantShock.m_fTime > 0.0f )
		return false;

	if ( IsImmuneToEnchant( CX2EnchantItem::ET_LIGHT ) == true )
	{


		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );

		return false;
	}

	//스턴 상태..
	const CX2EnchantItem::EnchantData* pEnchantData = NULL;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
    if ( null != pDamageData->optrAttackerGameUnit )
        pEnchantData = pDamageData->optrAttackerGameUnit->GetWeaponEnchantData( damageType );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
	if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetEnchantData( damageType );
	}
	else if ( null != pDamageData->optrAttackerGameUnit && pDamageData->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		pEnchantData = g_pData->GetEnchantItem()->GetNPCEnchantData( damageType );
	}
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
	if ( pEnchantData == NULL )
		return false;

#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_ExtraDamagePack.Init(CX2DamageManager::EDT_ENCHANT_SHOCK);
#else
	m_ExtraDamagePack.m_EnchantShock.Init();
#endif CHUNG_SECOND_CLASS_CHANGE

	// kimhc // 2010.02.23 // 아래 한줄의 코드는
	// 한개 이상의 몹이 동시에 타격될때 쇼크에 걸리지 않은 몹이 뒤로 밀리는 
	// 현상 때문에 주석 처리함
	//pDamageData->backSpeed.x		= GetDashSpeed() * 1.5f;

	pDamageData->fCameraCrashGap	= 15.0f;
	pDamageData->fCameraCrashTime	= 0.5f;
	//pDamageData->fStopTimeDef		= pEnchantData->m_fTimeStun;
	m_ExtraDamagePack.m_EnchantShock.m_fTime = pEnchantData->m_fTimeStun;

	

	float fTempTime = m_ExtraDamagePack.m_EnchantShock.m_fTime;
	ApplyEnchantResist( CX2EnchantItem::ET_LIGHT, fTempTime );
	m_ExtraDamagePack.m_EnchantShock.m_fTime = fTempTime;

#ifdef CHUNG_SECOND_CLASS_CHANGE
	// 바로 위에서 타입 설정됨. 인챈트를 pack에 넣을때는 아래와 같이 하면 안되고, 타입을 통일해야함.
#else
	m_ExtraDamagePack.m_EnchantShock.m_ExtraDamageType = pEnchantData->m_ExtraDamageType;
#endif CHUNG_SECOND_CLASS_CHANGE

	AnimStop();

	m_PhysicParam.nowSpeed.x	= 0.f;
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );


	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Raven_DashAttackX01", pDamageData->impactPoint );
	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_DashAttackX02", pDamageData->impactPoint );
	g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_Raven_DashAttackX03", pDamageData->impactPoint );
	UpDownCrashCamera( 20.0f, 0.5f );

	return true;

}

bool CX2GameUnit::ProcessEnchantSnatch( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::EXTRA_DAMAGE_TYPE damageType )
{
	if ( IsImmuneToEnchant( CX2EnchantItem::ET_DARK ) == true )
	{
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );

		return false;
	}

    CX2GameUnit*    pAttackerGameUnit = pDamageData->optrAttackerGameUnit.GetObservable();	
    if ( pAttackerGameUnit == NULL )
        return true;    // 이상하다. false 여야 하지 않나?
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
    const CX2EnchantItem::EnchantData* pEnchantData = pAttackerGameUnit->GetWeaponEnchantData( damageType );
	if ( pEnchantData == NULL )
		return false;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR

    if ( pAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
		const CX2EnchantItem::EnchantData* pEnchantData = g_pData->GetEnchantItem()->GetEnchantData( damageType );
		if ( pEnchantData == NULL )
			return false;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR

		CX2GUUser* pGUUser = static_cast<CX2GUUser*>( pAttackerGameUnit );

		CX2Item* pItem = pGUUser->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
		if ( pItem == NULL )
			return false;

#ifdef PVP_SEASON2_SOCKET
		float fOrgDamage = 0.f;
		float fAtkPhysic = 0.f;
		float fAtkMagic = 0.f;
		int iAddLevel = 0;
		bool bLevelLinkedStat = false;

        unsigned uNumSO = pItem->GetItemTemplet()->GetNumSocketOption();
		for ( int i = 0; i < (int) uNumSO; ++i )
		{
            int socketOption = pItem->GetItemTemplet()->GetSocketOption(i);
			if( 0 == socketOption )
				continue;

			const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
			if( pSocketData == NULL )
				continue;

			if ( NULL != pSocketData && true == pSocketData->m_bIsLevelLinkedStat )
			{
				fAtkPhysic += pSocketData->m_Stat.m_fAtkPhysic;
				fAtkMagic += pSocketData->m_Stat.m_fAtkMagic;
				iAddLevel = pSocketData->m_iAddLevel;
				bLevelLinkedStat = true;
			}
		}

		if ( true == bLevelLinkedStat && g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
			fOrgDamage = ( fAtkPhysic + fAtkMagic ) * ( pGUUser->GetUnitLevel() + iAddLevel );
		else
        {
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				// 스탯테이블을 통해 스탯 계산
				CX2Item::KItemFormatStatData kCalculateStat;
				g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUser->GetUnitType(), pGUUser->GetUnitClass() );
				const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
				const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            fOrgDamage = stat.m_fAtkPhysic + stat.m_fAtkMagic;
        }
#else
#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			// 스탯테이블을 통해 스탯 계산
			CX2Item::KItemFormatStatData kCalculateStat;
			g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pItem->GetItemTemplet(), pGUUSer->GetUnitType(), pGUUSer->GetUnitClass() );
			const CX2Item::KItemFormatStatData& stat = kCalculateStat;
	#else
			const CX2Item::KItemFormatStatData& stat = pItem->GetItemTemplet()->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        const CX2Stat::Stat& stat = pItem->GetItemTemplet()->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        fOrgDamage = stat.m_fAtkPhysic + stat.m_fAtkMagic;
#endif
		// 2008.11.7 김태완 : 스내치 보정 기획대로 수정
		// 이 줄은 아래쪽으로 이동
		//float fFinalDamage = fOrgDamage * pEnchantData->m_HPDrainPercent / 100.0f;

		// PVP 자유채널 인첸트 데미지 보정
		// if 문에서 switch 문으로 코드 정리 08.11.7
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
			{


#ifdef HENIR_TEST
#ifndef NEW_HENIR_DUNGEON // 1레벨 보정 -> 연동 레벨 던전으로 변경
				if( true == CX2Dungeon::IsHenirDungeon( static_cast<const SEnum::DUNGEON_ID>( g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonID ) ) &&
					CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetPartyManager()->GetMyPartyData()->m_iDungeonMode )
				{
					if(	g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
					{
						fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;

					}
				}
#endif // NEW_HENIR_DUNGEON
#endif HENIR_TEST

				//{{ 허상형 : [2011/3/29/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
				if( NULL != g_pData->GetDungeonRoom() && g_pData->GetDungeonRoom()->IsDefenceDungeon( g_pData->GetDungeonRoom()->GetDungeonID() ) == true )
				{
					if(	g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetUIQuickSlot() != NULL )
					{
						fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;

					}
				}
#endif SERV_INSERT_GLOBAL_SERVER
				//}} 허상형 : [2011/3/29/] //	월드 미션


				if(g_pData->GetDungeonRoom() != NULL )
				{
#ifdef SERV_DUNGEON_OPTION_IN_LUA
					if( g_pData->GetDungeonManager()->GetDungeonData( g_pData->GetDungeonRoom()->GetDungeonID() )->m_bDamageEqualized == true )
					{
						fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;
					}
					else
#endif SERV_DUNGEON_OPTION_IN_LUA
					// 태완 : 특정 던전에서의 인챈트 보정 코드. NEW_YEAR_EVENT_DUNGEON 등으로 검색해서 나오는 곳에 모두 작업해줘야 한다. (총 4곳)
					// 보정던전이 더 많아진다면 던전데이타 스크립트에 보정여부를 추가하는 방법이 더 나을 것 같다.
					switch(g_pData->GetDungeonRoom()->GetDungeonID())
					{
//{{ 2009.01.20 김태완
#ifdef NEW_YEAR_EVENT_DUNGEON
					// 월리성 새해맞이 던전 인챈트 보정코드
					case SEnum::DI_ELDER_NEWYEAR_NORMAL:
						{
							fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;
						} break;
#endif
			//}}
#ifdef HALLOWEEN_EVENT
					case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
					case SEnum::DI_ELDER_HALLOWEEN_HARD:
					case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
						{
							fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;
						} break;
#endif
#ifdef CHILDRENS_DAY_EVENT_DUNGEON
					case SEnum::DI_EVENT_KIDDAY_RUBEN:
					case SEnum::DI_EVENT_KIDDAY_ELDER:
					case SEnum::DI_EVENT_KIDDAY_BESMA:
					case SEnum::DI_EVENT_KIDDAY_ALTERA:
					
					case SEnum::DI_EVENT_TREE_DAY_ELDER:
					case SEnum::DI_EVENT_TREE_DAY_BESMA:
					case SEnum::DI_EVENT_TREE_DAY_ALTERA:
					case SEnum::DI_EVENT_TREE_DAY_PEITA:
					case SEnum::DI_EVENT_TREE_DAY_VELDER:
					case SEnum::DI_EVENT_TREE_DAY_HAMEL:
						//{{ oasis907 : 김상윤 [2010.10.21] // 
					case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
					case SEnum::DI_ELDER_HALLOWEEN_HARD:
					case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
						//}} oasis907 : 김상윤 [2010.10.21] // 
					case SEnum::DI_EVENT_VALENTINE_DAY:
						//{{ 허상형 : [2011/3/3/] //	월드 미션
#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
					case SEnum::DI_EVENT_HALLOWEEN_DAY:
#endif //SERV_HALLOWEEN_EVENT_2013
#ifdef SERV_INSERT_GLOBAL_SERVER
					CASE_DEFENCE_DUNGEON
#endif SERV_INSERT_GLOBAL_SERVER
						//}} 허상형 : [2011/3/3/] //	월드 미션
						{
							fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;
						} break;
#endif
					default:
						break;
					}
				}
			}break;

		case CX2Main::XS_PVP_GAME:
			{
				if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_FREE )
				{
					fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;
				}
#ifdef TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
				else if ( g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) == KPVPChannelInfo::PCC_TOURNAMENT )
				{
					fOrgDamage = pGUUser->GetStat().GetStat().m_fAtkPhysic + pGUUser->GetStat().GetStat().m_fAtkMagic;
				}
#endif TEMP_HERO_MATCH_LIKE_FREE_CHANNEL
				else
				{
					fOrgDamage *= 0.1f;
				}
			}break;
		default:
			break;
		}

		float fFinalDamage = fOrgDamage * pEnchantData->m_HPDrainPercent / 100.0f;

		float fUpHP = fFinalDamage;
		float fUpMP = (float)pEnchantData->m_MPDrain;


		

		ApplyEnchantResist( CX2EnchantItem::ET_DARK, fUpHP );
		ApplyEnchantResist( CX2EnchantItem::ET_DARK, fUpMP );


		pAttackerGameUnit->UpNowHp( fUpHP );
		pAttackerGameUnit->UpNowMp( fUpMP );


		std::wstringstream strStream;
		strStream << L"HP +" << (int)fUpHP;
		strStream << L" MP +" << (int)fUpMP;
		D3DXVECTOR3 pos = pAttackerGameUnit->GetPos();
		pos.y += 50.0f + (0 * 30.0f);
		if( NULL != g_pData->GetPicCharGreen() )
			g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pAttackerGameUnit->GetDirVector(), CKTDGPicChar::AT_CENTER );



#ifdef REFACTORING_BY_TOOL_TEAM
		UpNowHp( -fUpHP );
#else REFACTORING_BY_TOOL_TEAM
		//GetGageManager()->GetHPGage()->fNow -= fUpHP;
		float fNowHp = GetNowHp() - fUpHP;
		SetNowHp( fNowHp );

		if( GetNowHp() < 0.f )
			SetNowHp( 0.f );


#ifdef DAMAGE_HISTORY
		*m_fDamageHistory += fUpHP;
#endif

#endif REFACTORING_BY_TOOL_TEAM






		
#ifdef REFACTORING_BY_TOOL_TEAM
		UpNowMp( -fUpMP );
#else REFACTORING_BY_TOOL_TEAM
		UpNowMp( -fUpMP );
#endif REFACTORING_BY_TOOL_TEAM

		pAttackerGameUnit->ShowSnatchCureEffect();

		
		
	}
	else if ( pAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
		const CX2EnchantItem::EnchantData* pEnchantData = g_pData->GetEnchantItem()->GetNPCEnchantData( damageType );
		if ( pEnchantData == NULL )
			return false;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR

		CX2GUNPC* pGUNpc = static_cast<CX2GUNPC*>( pAttackerGameUnit );

		
		float fOrgDamage = pGUNpc->GetStat().GetStat().m_fAtkPhysic + pGUNpc->GetStat().GetStat().m_fAtkMagic;
		float fFinalDamage = fOrgDamage * pEnchantData->m_HPDrainPercent / 100.0f;


		float fUpHP = fFinalDamage;
		float fUpMP = (float)pEnchantData->m_MPDrain;




		ApplyEnchantResist( CX2EnchantItem::ET_DARK, fUpHP );
		ApplyEnchantResist( CX2EnchantItem::ET_DARK, fUpMP );



		pAttackerGameUnit->UpNowHp( fUpHP );
		pAttackerGameUnit->UpNowMp( fUpMP );




		std::wstringstream strStream;
		strStream << L"HP +" << (int)fUpHP;
		strStream << L" MP +" << (int)fUpMP;
		D3DXVECTOR3 pos = pAttackerGameUnit->GetPos();
		pos.y += 50.0f + (0 * 30.0f);
		if( NULL != g_pData->GetPicCharGreen() )
			g_pData->GetPicCharGreen()->DrawText( strStream.str().c_str(), pos, pAttackerGameUnit->GetDirVector(), CKTDGPicChar::AT_CENTER );



#ifdef REFACTORING_BY_TOOL_TEAM
		UpNowHp( -fUpHP );
#else REFACTORING_BY_TOOL_TEAM
		//GetGageManager()->GetHPGage()->fNow -= fUpHP;
		float fNowHp = GetNowHp() - fUpHP;
		SetNowHp( fNowHp );

		if( GetNowHp() < 0.f )
			SetNowHp( 0.f );
#ifdef DAMAGE_HISTORY
		*m_fDamageHistory += fUpHP;
#endif
#endif REFACTORING_BY_TOOL_TEAM



#ifdef REFACTORING_BY_TOOL_TEAM
		UpNowMp( -fUpMP );
#else REFACTORING_BY_TOOL_TEAM
		UpNowMp( -fUpMP );
#endif REFACTORING_BY_TOOL_TEAM



		pAttackerGameUnit->ShowSnatchCureEffect();
	}

	return true;
}

/*virtual*/ float CX2GameUnit::GetEnchantResist( CX2EnchantItem::ENCHANT_TYPE enchantType )
{
	float fFinalResist = 0.f;

	switch( enchantType )
	{
	case CX2EnchantItem::ET_BLAZE:
		fFinalResist = m_AdditionalChangeAttributeBlazeDefence.GetResultValue();
		break;
	case CX2EnchantItem::ET_WATER:
		fFinalResist = m_AdditionalChangeAttributeWaterDefence.GetResultValue();
		break;
	case CX2EnchantItem::ET_NATURE:
		fFinalResist = m_AdditionalChangeAttributeNatureDefence.GetResultValue();
		break;
	case CX2EnchantItem::ET_WIND:
		fFinalResist = m_AdditionalChangeAttributeWindDefence.GetResultValue();
		break;
	case CX2EnchantItem::ET_LIGHT:
		fFinalResist = m_AdditionalChangeAttributeLightDefence.GetResultValue();
		break;
	case CX2EnchantItem::ET_DARK:
		fFinalResist = m_AdditionalChangeAttributeDarkDefence.GetResultValue();
		break;
	}

	map< CX2EnchantItem::ENCHANT_TYPE, int >::iterator mit;
	mit = m_mapDefenceEnchantData.find( enchantType );

	if ( mit != m_mapDefenceEnchantData.end() )
		fFinalResist += (float)mit->second;

	return fFinalResist;
}

#ifdef PET_AURA_SKILL
/*virtual*/ float CX2GameUnit::GetEnchantAttack(  CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType )
{
	CX2EnchantItem::ENCHANT_TYPE enchantType = GetEnchantTypeFromExtraDamageType( extraDamageType );

	map< CX2EnchantItem::ENCHANT_TYPE, float >::iterator mit;
	mit = m_mapAttackEnchantData.find( enchantType );
	if ( mit == m_mapAttackEnchantData.end() )
		return 0.f;

	return (float)mit->second;
}
#endif

CX2EnchantItem::ENCHANT_TYPE CX2GameUnit::GetEnchantTypeFromExtraDamageType( CX2DamageManager::EXTRA_DAMAGE_TYPE edtType )
{
	switch(edtType)
	{

	case CX2DamageManager::EDT_POISON:	
		return CX2EnchantItem::ET_NATURE;

	case CX2DamageManager::EDT_FIRE:
		return CX2EnchantItem::ET_BLAZE;

	case CX2DamageManager::EDT_COLD:
		return CX2EnchantItem::ET_WATER;

	case CX2DamageManager::EDT_LEG_WOUND:
		return CX2EnchantItem::ET_NONE;




		// 진짜 속성 인챈트
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			return CX2EnchantItem::ET_BLAZE;
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			return CX2EnchantItem::ET_WATER;
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON:
	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			return CX2EnchantItem::ET_NATURE;
		} break;

	case CX2DamageManager::EDT_ENCHANT_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_MASTER_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_MASTER_PIERCING:
		{
			return CX2EnchantItem::ET_WIND;
		} break;

	case CX2DamageManager::EDT_ENCHANT_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_MASTER_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_MASTER_SHOCK:
		{
			return CX2EnchantItem::ET_LIGHT;
		} break;

	case CX2DamageManager::EDT_ENCHANT_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_MASTER_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_MASTER_SNATCH:
		{
			return CX2EnchantItem::ET_DARK;
		} break;

	}

	return CX2EnchantItem::ET_NONE;
}


void CX2GameUnit::ApplyEnchantResist( CX2EnchantItem::ENCHANT_TYPE enchantType, float& fDamage )
{
	float fResistRate = GetEnchantResist( enchantType ) / CX2EnchantItem::EAR_MAX_VALUE;
	if( fResistRate < 0.f )
		fResistRate = 0.f;
	if( fResistRate > 1.f )
		fResistRate = 1.f;

	float fDamageRate = 1.f - fResistRate;
	fDamage *= fDamageRate;
}


bool CX2GameUnit::IsImmuneToEnchant( CX2EnchantItem::ENCHANT_TYPE enchantType )
{
	map< CX2EnchantItem::ENCHANT_TYPE, int >::iterator mit = m_mapDefenceEnchantData.find( enchantType );
	if ( mit != m_mapDefenceEnchantData.end() )
	{
		if ( mit->second > CX2EnchantItem::EAR_MAX_VALUE )
		{
			return true;
		}
	}

	return false;
}


void CX2GameUnit::AddOnStatReq( KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT statType, float fStatVal, float fTime )
{
	DISPLAY_ERROR( L"This Function doesn't use" );
// 	KEGS_ADD_ON_STAT_REQ kEGS_ADD_ON_STAT_REQ;
// 
// 	if( GetGameUnitType() == GUT_USER )
// 	{
// 		kEGS_ADD_ON_STAT_REQ.m_bIsUnit = true;
// 	}
// 	else
// 	{
// 		kEGS_ADD_ON_STAT_REQ.m_bIsUnit = false;
// 	}
// 	kEGS_ADD_ON_STAT_REQ.m_UID			= GetUnitUID();
// 	kEGS_ADD_ON_STAT_REQ.m_cStatType	= (char)statType;
// 	kEGS_ADD_ON_STAT_REQ.m_fStatVal		= fStatVal;
// 	kEGS_ADD_ON_STAT_REQ.m_fTime		= fTime;
// 
// 	g_pData->GetServerProtocol()->SendPacket( EGS_ADD_ON_STAT_REQ, kEGS_ADD_ON_STAT_REQ );
}

void CX2GameUnit::SetAddOnStat( KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT statType, float fStatVal, float fTime )
{
	switch( statType )
	{
	case KEGS_ADD_ON_STAT_REQ::AOS_ATK_PHYSIC:
		{

// 			m_Stat.AccessAddOnStat().m_fAtkPhysic = fStatVal * m_Stat.GetStat().m_fAtkPhysic;
// 			m_Stat.AccessAddOnTime().m_fAtkPhysic = fTime;
// 
// 			if ( m_hSeqAddOnStatAtkPhysic == INVALID_PARTICLE_SEQUENCE_HANDLE )
// 				m_hSeqAddOnStatAtkPhysic = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"BuffAttack", 0,0,0, 0, 0 );
// 
// 			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqAddOnStatAtkPhysic );
// 			if( NULL != pSeq )
// 				pSeq->SetEmitRate( CMinMax<float>(1000,1000) );
		}
		break;
	case KEGS_ADD_ON_STAT_REQ::AOS_ATK_MAGIC:
		{
// 			m_Stat.AccessAddOnStat().m_fAtkMagic = fStatVal * m_Stat.GetStat().m_fAtkMagic;
// 			m_Stat.AccessAddOnTime().m_fAtkMagic = fTime;
// 
// 			if ( m_hSeqAddOnStatAtkMagic == INVALID_PARTICLE_SEQUENCE_HANDLE )
// 				m_hSeqAddOnStatAtkMagic = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"BuffMagic", 0,0,0, 0, 0 );
// 
// 			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqAddOnStatAtkMagic );
// 			if( NULL != pSeq )
// 				pSeq->SetEmitRate( CMinMax<float>(1000,1000) );
		}
		break;

	case KEGS_ADD_ON_STAT_REQ::AOS_DEF_PHYSIC:
		{
// 			m_Stat.AccessAddOnStat().m_fDefPhysic = fStatVal * m_Stat.GetStat().m_fDefPhysic;
// 			m_Stat.AccessAddOnTime().m_fDefPhysic = fTime;
// 
// 			if ( m_hSeqAddOnStatDefPhysic == INVALID_PARTICLE_SEQUENCE_HANDLE )
// 				m_hSeqAddOnStatDefPhysic = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"BuffShield", 0,0,0, 0, 0 );
// 
// 			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqAddOnStatDefPhysic );
// 			if( NULL != pSeq )
// 				pSeq->SetEmitRate( CMinMax<float>(1000,1000) );
		}
		break;

	case KEGS_ADD_ON_STAT_REQ::AOS_DEF_MAGIC:
		{
// 			m_Stat.AccessAddOnStat().m_fDefMagic = fStatVal * m_Stat.GetStat().m_fDefMagic;
// 			m_Stat.AccessAddOnTime().m_fDefMagic = fTime;
// 
// 			// fix!!! 얘는 이펙트가 없네??
		}
		break;
	}
}

void CX2GameUnit::SetSubAttackList( CKTDGXSkinAnim* pXSkinAnim )
{
	m_SubAttackListSet.clear();

	if( NULL == pXSkinAnim )
		return;

    m_SubAttackListSet.insert( &pXSkinAnim->GetAttackDataList() );
}


const CX2EnchantItem::EnchantData* CX2GameUnit::GetWeaponEnchantData( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType ) const
{
	if ( GetGameUnitType() == CX2GameUnit::GUT_USER )
		return g_pData->GetEnchantItem()->GetEnchantData( extraDamageType );
	else
		return g_pData->GetEnchantItem()->GetNPCEnchantData( extraDamageType );
}

#ifdef ENCHANT_ACCUMULATION_RATE_MODIFIER
float CX2GameUnit::EnchantAccumulationRateModifier( CX2DamageManager::EXTRA_DAMAGE_TYPE basicExtraDamageType, CX2DamageManager::EXTRA_DAMAGE_TYPE miscExtraDamageType, float fRate )
{
	switch(basicExtraDamageType)
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
		{
			switch(miscExtraDamageType)
			{
				case CX2DamageManager::EDT_ENCHANT_BLAZE:
					{
						if(m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation < 3)
						{
							fRate *= 2.0f;
						}
					}
					break;
				case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
					{
						if(m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation < 3)
						{
							fRate *= 2.0f;
						}
					}
					break;
				case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
					{
						if(m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantBlaze.m_Accumulation < 3)
						{
							fRate *= 2.0f;
						}
					}
					break;
			}
		}
		break;
	case CX2DamageManager::EDT_ENCHANT_FROZEN:
		{
			switch(miscExtraDamageType)
			{
			case CX2DamageManager::EDT_ENCHANT_FROZEN:
				{
					if(m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation < 3)
					{
						fRate *= 2.0f;
					}
				}
				break;
			case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
				{
					if(m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation < 3)
					{
						fRate *= 2.0f;
					}
				}
				break;
			case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
				{
					if(m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantFrozen.m_Accumulation < 3)
					{
						fRate *= 2.0f;
					}
				}
				break;
			}
		}
		break;
	case CX2DamageManager::EDT_ENCHANT_POISON:
		{
			switch(miscExtraDamageType)
			{
			case CX2DamageManager::EDT_ENCHANT_POISON:
				{
					if(m_ExtraDamagePack.m_EnchantPoison.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantPoison.m_Accumulation < 3)
					{
						fRate *= 2.0f;
					}
				}
				break;
			case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
				{
					if(m_ExtraDamagePack.m_EnchantPoison.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantPoison.m_Accumulation < 3)
					{
						fRate *= 2.0f;
					}
				}
				break;
			case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
				{
					if(m_ExtraDamagePack.m_EnchantPoison.m_Accumulation >= 1 && m_ExtraDamagePack.m_EnchantPoison.m_Accumulation < 3)
					{
						fRate *= 2.0f;
					}
				}
				break;
			}
		}
		break;
	}
	return fRate;
}
#endif ENCHANT_ACCUMULATION_RATE_MODIFIER


// fix!! to do: b3DSound 설정할 수 있게 수정하기
// fix!! to do: 오브젝트가 움직이는 중에 계속해서 sound 위치 업데이트 할 수 있게 수정하기
CKTDXDeviceSound* CX2GameUnit::PlaySound( const WCHAR* pName, bool b3DSound, bool bLoop )
{
	if( NULL == pName )
		return NULL; 

#ifdef NEW_CHARACTER_CHUNG
	// oasis907 : 김상윤 [2010.12.20] // 슬라이드 샷 진행중일때 캐릭터 사운드 플레이 안하기
 	if(g_pData->GetSlideShot() != NULL &&
 		g_pData->GetSlideShot()->IsPresentNow() == true)
 		return NULL;
#endif NEW_CHARACTER_CHUNG

	CKTDXDeviceSound* pSound = g_pKTDXApp->GetDeviceManager()->OpenSound( pName, 10, b3DSound );
	if( NULL != pSound )
	{
		if( b3DSound == true )
		{
			pSound->Set3DPosition( GetPos() );
			pSound->Play(bLoop);
		}
		else
		{
			pSound->Play(bLoop, false);
		}
		


#ifdef CLOSE_SOUND_TEST

		if( bLoop == false )
			m_SoundCloseManager.AddSound( pName );

#endif CLOSE_SOUND_TEST
	}

	return pSound;
}


void CX2GameUnit::PlaySound_LUA( const char* pName )
{
	if( NULL == pName )
		return; 

#ifdef X2OPTIMIZE_NPC_DIELIGHT_SOUND_BY_GAMEOPTION
	//DieLight.wav는 게임옵션에 따라 생성여부가 결정된다.
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_LOW && stricmp( pName, "DieLight.ogg" ) == 0 )
		return;
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_bEffect == false && stricmp( pName, "DieLight.ogg" ) == 0 )
		return;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
#endif//X2OPTIMIZE_NPC_DIELIGHT_SOUND_BY_GAMEOPTION

	wstring wName = L"";
	ConvertUtf8ToWCHAR( wName, pName );

	PlaySound( wName.c_str() );
}

void CX2GameUnit::StopSound_LUA( const char* pName )
{
	if( NULL == pName )
		return; 

	wstring wName = L"";
	ConvertUtf8ToWCHAR( wName, pName );

	g_pKTDXApp->GetDeviceManager()->StopSound( wName.c_str() );
}


void CX2GameUnit::DyingStart()
{
	m_bDyingStart = true; 

	DeleteEffectSetOnDie();

	FinishAndClearAllBuff();	/// 버프 종료 및 삭제
	++m_iDieCount;
}






void CX2GameUnit::AddWeaponAttackCollisionBoxForNPC( Weapon* pWeapon )
{
	if( NULL == pWeapon )
		return;

	if( NULL == pWeapon->m_pXSkinWeapon )
		return; 
	
    m_CollisionListSet.insert( &pWeapon->m_pXSkinWeapon->GetCollisionDataList() );
    m_AttackListSet.insert( &pWeapon->m_pXSkinWeapon->GetAttackDataList() );
}

void CX2GameUnit::RemoveAllWeaponAttackCollisionBoxForNPC()
{
	for ( int j = 0; j < (int)m_vecpWeapon.size(); ++j )
	{
		Weapon* pWeapon = m_vecpWeapon[j];
		if( NULL == pWeapon->m_pXSkinWeapon )
			continue;

        m_CollisionListSet.erase( &pWeapon->m_pXSkinWeapon->GetCollisionDataList() );
        m_AttackListSet.erase( &pWeapon->m_pXSkinWeapon->GetAttackDataList() );
	}
}



D3DXVECTOR3 CX2GameUnit::GetWeaponBoneRot( int iWeaponIndex, int iModelIndex )
{

	if( iWeaponIndex < 0 ||
		iWeaponIndex >= (int)m_vecpWeapon.size() )
		return D3DXVECTOR3( 0, 0, 0 );


	Weapon* pWeapon = m_vecpWeapon[iWeaponIndex];
	D3DXVECTOR3 vRot(0, 0, 0);

	if( pWeapon == NULL )
		return vRot;

	switch( GetGameUnitType() )
	{
	default:
		{
		} break;
	case CX2GameUnit::GUT_USER:
		{
			// fix!! hyper 모드 상태의 무기 GetHyperAnim()을 호출해야함 
			//vPos = pWeapon->m_pUserWeaponEquip->GetNormalAnim()->GetCloneFramePosition( pBoneName );			
			GetFrameRotateDegree(&vRot, pWeapon->m_pFrame_TRACE_START[0]);		
		} break;

	case CX2GameUnit::GUT_NPC:
		{
			//vPos = pWeapon->m_pXSkinWeapon->GetCloneFramePosition( pBoneName );
			GetFrameRotateDegree(&vRot, pWeapon->m_pFrame_TRACE_START[0]);			
		} break;
	}

	return vRot;
}


D3DXVECTOR3 CX2GameUnit::GetWeaponBonePos( int iWeaponIndex, const WCHAR* pBoneName, int iModelIndex ) const
{
	if( NULL == pBoneName || 
		true == m_vecpWeapon.empty() )
		return D3DXVECTOR3( 0, 0, 0 );


	if( iWeaponIndex < 0 ||
		iWeaponIndex >= (int)m_vecpWeapon.size() )
		return D3DXVECTOR3( 0, 0, 0 );


	Weapon* pWeapon = m_vecpWeapon[iWeaponIndex];
	D3DXVECTOR3 vPos(0, 0, 0);


	switch( GetGameUnitType() )
	{
	default:
		{
		} break;
	case CX2GameUnit::GUT_USER:
		{
			// fix!! hyper 모드 상태의 무기 GetHyperAnim()을 호출해야함 
			//vPos = pWeapon->m_pUserWeaponEquip->GetNormalAnim()->GetCloneFramePosition( pBoneName );
			vPos = pWeapon->GetBonePos( pBoneName, iModelIndex );
		} break;

	case CX2GameUnit::GUT_NPC:
		{
			//vPos = pWeapon->m_pXSkinWeapon->GetCloneFramePosition( pBoneName );
			vPos = pWeapon->GetBonePos( pBoneName, iModelIndex );
		} break;
	}

	return vPos;
}

//{{ kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)
/** @function : GetCombineMatrixFromWeaponBoneName
	@brief : 무기 이름에 해당하는 CombineMatrix 포인터를 얻어옴
	@param : int iWeaponIndex, const wstring& wstrWeaponBoneName, int iModelIndex
	@return : const D3DXMATRIX* 
*/
const D3DXMATRIX* CX2GameUnit::GetCombineMatrixFromWeaponBoneName( int iWeaponIndex, const wstring& wstrWeaponBoneName, int iModelIndex ) const
{
	if ( true == wstrWeaponBoneName.empty()
		 || true == m_vecpWeapon.empty() )
	{
		return NULL;
	} // if

	if ( 0 > iWeaponIndex
		 || static_cast<int>( m_vecpWeapon.size() ) <= iWeaponIndex )
	{
		return NULL;
	} // if

	const Weapon* pWeapon = m_vecpWeapon[iWeaponIndex];

	return pWeapon->GetCombineMatrix( wstrWeaponBoneName.c_str(), iModelIndex );
}
//}} kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)

D3DXVECTOR3 CX2GameUnit::GetWeaponBonePos_LUA( int iWeaponIndex, const char* pBoneName )
{
	if( NULL == pBoneName )
		return D3DXVECTOR3( 0, 0, 0 );

	wstring wBoneName = L"";
	ConvertUtf8ToWCHAR( wBoneName, pBoneName );

	// fix!!! 일단 무기의 0번째 메시만 되도록 하드 코딩
	return GetWeaponBonePos( iWeaponIndex, wBoneName.c_str(), 0 );
}



void CX2GameUnit::AttachWeaponParticle( int iWeaponIndex, bool bIsMajorParticle, const wstring& wstrBoneName,
										   CKTDGParticleSystem::CParticleEventSequenceHandle hSeq )
{
	if( iWeaponIndex < 0 || iWeaponIndex > (int) m_vecpWeapon.size()-1 )
		return;

	WeaponParticle* pWeaponParticle = new WeaponParticle;
	pWeaponParticle->m_bIsMajorParticle = bIsMajorParticle;
	pWeaponParticle->m_hSeq = hSeq;
	pWeaponParticle->m_wstrBoneName = wstrBoneName;

	Weapon* pWeapon = m_vecpWeapon[iWeaponIndex];
	pWeapon->m_vecpWeaponParticle.push_back( pWeaponParticle );
}



void CX2GameUnit::AttachWeaponParticle_LUA( int iWeaponIndex, bool bIsMajorParticle, char* pBoneName, 
										   CKTDGParticleSystem::CParticleEventSequenceHandle hSeq )
{
	if( NULL == pBoneName )
		return;
	
	wstring wstrBoneName = L"";
	ConvertUtf8ToWCHAR( wstrBoneName, pBoneName );
	AttachWeaponParticle( iWeaponIndex, bIsMajorParticle, wstrBoneName, hSeq );
}




//{{ kimhc // 실시간 엘소드 중 실시간 장비 교체
#ifdef REAL_TIME_ELSWORD
void CX2GameUnit::DeleteEnchantWeaponEffectUnitSpecific( int index )
{
	Weapon* pWeapon = m_vecpWeapon[index];
	
	pWeapon->DeleteEnchantParticle();		
}
#endif REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 실시간 장비 교체



void CX2GameUnit::SetMajorMeshPlayer_LUA( int index, CKTDGXMeshPlayer::CXMeshInstance* pMeshPlayer )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMajorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMajorMeshPlayerList) )
		return;

//#ifdef HEAVY_LOG_TEST 
//	debugWPrintf( L"SetMajorMeshPlayer_LUA: %d, %d, %s \n", 
//		(int) index, (int)pMeshPlayer, pMeshPlayer->GetTempletData()->templetName.c_str() );
//#endif HEAVY_LOG_TEST


	if( NULL == pMeshPlayer )
	{
		m_hMajorMeshPlayerList[index] = INVALID_MESH_INSTANCE_HANDLE;
	}
	else
	{
		m_hMajorMeshPlayerList[index] = pMeshPlayer->GetHandle();
	}
}

void CX2GameUnit::ClearMajorMeshPlayer_LUA( int index )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMajorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMajorMeshPlayerList) )
		return;


//#ifdef HEAVY_LOG_TEST 
//	debugWPrintf( L"ClearMajorMeshPlayer_LUA: %d, %d \n", 
//		(int) index, (int)m_hMajorMeshPlayerList[index] );
//#endif HEAVY_LOG_TEST

	m_hMajorMeshPlayerList[index] = INVALID_MESH_INSTANCE_HANDLE;
}

// 더블어택
void CX2GameUnit::SetDoubleAttack(wstring wstrHitSound, float fDamage, D3DXVECTOR3 vimpactPoint)
{
	m_wstrHitSound = wstrHitSound;
	m_fFinalDamage = fDamage;
	m_vImpactPoint = vimpactPoint;
	m_fDoubleAttackTime = 0.f;
}


CKTDGXMeshPlayer::CXMeshInstance* CX2GameUnit::GetMajorMeshPlayer_LUA( int index )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMajorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMajorMeshPlayerList) )
		return NULL;

//#ifdef HEAVY_LOG_TEST 
//	debugWPrintf( L"GetMajorMeshPlayer_LUA: %d, %d, %s \n", 
//		(int) index, (int)pMeshPlayer, pMeshPlayer->GetTempletData()->templetName.c_str() );
//#endif HEAVY_LOG_TEST


	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMajorXMeshPlayer() )
		return NULL;

	return g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMajorMeshPlayerList[index] );
}

CKTDGXMeshPlayer::CXMeshInstanceHandle CX2GameUnit::GetMajorMeshPlayerHandle( int index )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMajorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMajorMeshPlayerList) )
		return INVALID_MESH_INSTANCE_HANDLE;

	return m_hMajorMeshPlayerList[index];
}


void CX2GameUnit::SetMinorMeshPlayer_LUA( int index, CKTDGXMeshPlayer::CXMeshInstance* pMeshPlayer )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMinorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMinorMeshPlayerList) )
		return;


	if( NULL == pMeshPlayer )
	{
		m_hMinorMeshPlayerList[index] = INVALID_MESH_INSTANCE_HANDLE;
	}
	else
	{
		m_hMinorMeshPlayerList[index] = pMeshPlayer->GetHandle();
	}
}


void CX2GameUnit::ClearMinorMeshPlayer_LUA( int index )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMinorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMinorMeshPlayerList) )
		return;

	m_hMinorMeshPlayerList[index] = INVALID_MESH_INSTANCE_HANDLE;
}

CKTDGXMeshPlayer::CXMeshInstance* CX2GameUnit::GetMinorMeshPlayer_LUA( int index )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMinorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMinorMeshPlayerList) )
		return NULL;


	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMinorXMeshPlayer() )
		return NULL;

	return g_pX2Game->GetMinorXMeshPlayer()->GetMeshInstance( m_hMajorMeshPlayerList[index] );

}


CKTDGXMeshPlayer::CXMeshInstanceHandle CX2GameUnit::GetMinorMeshPlayerHandle( int index )
{
	ASSERT( index >= 0 && index < ARRAY_SIZE(m_hMinorMeshPlayerList) );
	if( index < 0 || index >= ARRAY_SIZE(m_hMinorMeshPlayerList) )
		return INVALID_MESH_INSTANCE_HANDLE;

	return m_hMinorMeshPlayerList[index];
}



#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef ADD_MEMO_1ST_CLASS
void CX2GameUnit::LoadEffectSetFromScript( IN KLuaManager& luaManager, IN const bool IsEqippedMemo_ /*= false*/ )
#else //ADD_MEMO_1ST_CLASS
void CX2GameUnit::LoadEffectSetFromScript( IN KLuaManager& luaManager )
#endif //ADD_MEMO_1ST_CLASS
{
	m_vecEffectSetToPlay.resize(0);

#ifndef ADD_MEMO_1ST_CLASS
#ifdef ADDITIONAL_MEMO
	LUA_GET_VALUE( m_LuaManager, "APPLY_MEMO",		m_iMemoId,		0 );
#endif

#ifdef ADDITIONAL_MEMO
	bool bApplyMemo = false;
	if( m_iMemoId > 0 && GetGameUnitType() == GUT_USER)
	{
		CX2GUUser* pUser = (CX2GUUser*) this;
		if( pUser != NULL && pUser->GetEqippedSkillMemo( (CX2SkillTree::SKILL_MEMO_ID)m_iMemoId) )
			bApplyMemo = true;
	}
#endif
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_MEMO_1ST_CLASS
	bool bIsEffectSetMemo = false;

	if( true == IsEqippedMemo_ )
	{
		if( true == luaManager.BeginTable( "EFFECT_SET_LIST_MEMO" ) )
		{
			std::wstring wstrEffectSetName = L"";
			float fStartAnimationTime = 0.f;

			for( int iValueIndex = 1; iValueIndex < 100; iValueIndex += 2 )
			{
				if( false == luaManager.GetValue( iValueIndex, wstrEffectSetName ) )
					break;

				if( false == luaManager.GetValue( iValueIndex+1, fStartAnimationTime ) )
					break;

				m_vecEffectSetToPlay.push_back( EffectSetToPlay( wstrEffectSetName, fStartAnimationTime ) );
			}


			luaManager.EndTable(); //EFFECT_SET_LIST_MEMO

			bIsEffectSetMemo = true;
		}
	}

	//APPLY_MEMO가 정의되어 있지 않거나 
	//APPLY_MEMO가 정의되어 있지만 EFFECT_SET_LIST_MEMO가 정의되어 있지 않은 경우 EFFECT_SET_LIST_MEMO를 파싱한다.(DAMAGE_DATA_MEMO를 위해 APPLY_MEMO를 정의한 경우)
	if( false == IsEqippedMemo_ || ( true == IsEqippedMemo_ && false == bIsEffectSetMemo ) )
#endif //ADD_MEMO_1ST_CLASS
	{
		if( true == luaManager.BeginTable( "EFFECT_SET_LIST" ) )
		{
			std::wstring wstrEffectSetName = L"";
			float fStartAnimationTime = 0.f;

		for( int iValueIndex = 1; iValueIndex < 100; iValueIndex += 2 )
		{
			if( false == luaManager.GetValue( iValueIndex, wstrEffectSetName ) )
				break;

			if( false == luaManager.GetValue( iValueIndex+1, fStartAnimationTime ) )
				break;

#ifndef ADD_MEMO_1ST_CLASS
				//동작 방식 수정. 기존에 이 옵션을 사용하던 아야시(2개), 청(4개) state는 수정된 방법으로 스크립트에 적용함.
#ifdef ADDITIONAL_MEMO
			if( bApplyMemo == true )
				wstrEffectSetName = wstrEffectSetName + L"_MEMO";
#endif
#endif //ADD_MEMO_1ST_CLASS
				m_vecEffectSetToPlay.push_back( EffectSetToPlay( wstrEffectSetName, fStartAnimationTime ) );
			}


			luaManager.EndTable(); //EFFECT_SET_LIST
		}
	}
	

#ifdef ADD_MEMO_1ST_CLASS
	bIsEffectSetMemo = false;
	if( true == IsEqippedMemo_ )
	{
		if( true == luaManager.BeginTable( "EFFECT_SET_LIST_EX_MEMO" ) )
		{
			std::wstring wstrEffectSetName = L"";
			float fStartAnimationTime = 0.f;




#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			vector<DELETE_CONDITION_EFFECT_SET> vecDeleteCondition;		// 종료 조건 벡터
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			DELETE_CONDITION_EFFECT_SET eDeleteCondition = DCES_NONE;
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장

			int iTableIndex = 1;

			while( true == luaManager.BeginTable( iTableIndex++  ) )
			{
				LUA_GET_VALUE( luaManager, 1, wstrEffectSetName,	 L"" );
				if( L"" == wstrEffectSetName )
					break;

				LUA_GET_VALUE( luaManager, 2, fStartAnimationTime, 0.f);


#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
				int iTableCount = 3;			// 테이블 카운터, 3번 테이블부터 읽어오기 시작함
				UINT uiDeleteConditionType = 0;	// 종료 조건
				vecDeleteCondition.clear();

				while ( luaManager.GetValue( iTableCount, uiDeleteConditionType ) == true )
				{	// 값이 있다면, 테이블 카운터 증가시키고 종료 조건을 벡터에 넣어준다.
					// 얻어온 값이 범위에 벗어난다면, ASSERT 를 띄운다.
#ifdef _IN_HOUSE_
					// 사내 일 경우, 해당 종료 조건이 중복되는지 체크한다.
					ASSERT( vecDeleteCondition.end() == find ( vecDeleteCondition.begin(), vecDeleteCondition.end(), static_cast<CX2GameUnit::DELETE_CONDITION_EFFECT_SET> (uiDeleteConditionType) ) );
#endif // _IN_HOUSE_

					iTableCount++;		
					vecDeleteCondition.push_back( static_cast<CX2GameUnit::DELETE_CONDITION_EFFECT_SET> (uiDeleteConditionType) );	
				}
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
				LUA_GET_VALUE_ENUM( luaManager, 3 , eDeleteCondition, CX2GameUnit::DELETE_CONDITION_EFFECT_SET, CX2GameUnit::DCES_NONE );
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장

				luaManager.EndTable();

#ifndef ADD_MEMO_1ST_CLASS
				//동작 방식 수정. 기존에 이 옵션을 사용하던 아야시(2개), 청(4개) state는 수정된 방법으로 스크립트에 적용함.
#ifdef ADDITIONAL_MEMO
				if( bApplyMemo == true )
					wstrEffectSetName = wstrEffectSetName + L"_MEMO";
#endif
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
				m_vecEffectSetToPlay.push_back( EffectSetToPlay( wstrEffectSetName, fStartAnimationTime, vecDeleteCondition ) );
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
				m_vecEffectSetToPlay.push_back( EffectSetToPlay( wstrEffectSetName, fStartAnimationTime, eDeleteCondition ) );
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장

			}


			luaManager.EndTable(); //EFFECT_SET_LIST_EX_MEMO

			bIsEffectSetMemo = true;
		}
	}
	
	//APPLY_MEMO가 정의되어 있지 않거나 
	//APPLY_MEMO가 정의되어 있지만 EFFECT_SET_LIST_EX_MEMO가 정의되어 있지 않은 경우 EFFECT_SET_LIST_EX를 파싱한다.(DAMAGE_DATA_MEMO를 위해 APPLY_MEMO를 정의한 경우)
	if( false == IsEqippedMemo_ || ( true == IsEqippedMemo_ && false == bIsEffectSetMemo ) )
#endif //ADD_MEMO_1ST_CLASS
#ifdef MODIFY_EFFECT_SET_DELETE
	{
		if( true == luaManager.BeginTable( "EFFECT_SET_LIST_EX" ) )
		{
			std::wstring wstrEffectSetName = L"";
			float fStartAnimationTime = 0.f;

		


#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
		vector<DELETE_CONDITION_EFFECT_SET> vecDeleteCondition;		// 종료 조건 벡터
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
		DELETE_CONDITION_EFFECT_SET eDeleteCondition = DCES_NONE;
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장

		int iTableIndex = 1;
		
		while( true == luaManager.BeginTable( iTableIndex++  ) )
		{
			LUA_GET_VALUE( luaManager, 1, wstrEffectSetName,	 L"" );
			if( L"" == wstrEffectSetName )
				break;

			LUA_GET_VALUE( luaManager, 2, fStartAnimationTime, 0.f);

			
#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			int iTableCount = 3;			// 테이블 카운터, 3번 테이블부터 읽어오기 시작함
			UINT uiDeleteConditionType = 0;	// 종료 조건
			vecDeleteCondition.clear();

			while ( luaManager.GetValue( iTableCount, uiDeleteConditionType ) == true )
			{	// 값이 있다면, 테이블 카운터 증가시키고 종료 조건을 벡터에 넣어준다.
				// 얻어온 값이 범위에 벗어난다면, ASSERT 를 띄운다.
#ifdef _IN_HOUSE_
				// 사내 일 경우, 해당 종료 조건이 중복되는지 체크한다.
				ASSERT( vecDeleteCondition.end() == find ( vecDeleteCondition.begin(), vecDeleteCondition.end(), static_cast<CX2GameUnit::DELETE_CONDITION_EFFECT_SET> (uiDeleteConditionType) ) );
#endif // _IN_HOUSE_
				
				iTableCount++;		
				vecDeleteCondition.push_back( static_cast<CX2GameUnit::DELETE_CONDITION_EFFECT_SET> (uiDeleteConditionType) );	
			}
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			LUA_GET_VALUE_ENUM( luaManager, 3 , eDeleteCondition, CX2GameUnit::DELETE_CONDITION_EFFECT_SET, CX2GameUnit::DCES_NONE );
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장

			luaManager.EndTable();

#ifndef ADD_MEMO_1ST_CLASS
#ifdef ADDITIONAL_MEMO
			if( bApplyMemo == true )
				wstrEffectSetName = wstrEffectSetName + L"_MEMO";
#endif
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			m_vecEffectSetToPlay.push_back( EffectSetToPlay( wstrEffectSetName, fStartAnimationTime, vecDeleteCondition ) );
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			m_vecEffectSetToPlay.push_back( EffectSetToPlay( wstrEffectSetName, fStartAnimationTime, eDeleteCondition ) );
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			
		}

			luaManager.EndTable(); //EFFECT_SET_LIST
		}
	}
#endif // MODIFY_EFFECT_SET_DELETE

	LUA_GET_VALUE( m_LuaManager, "DELETE_EFFECT_SET_ON_STATE_END",		m_bDeleteEffectSetOnStateEnd,		false );
	LUA_GET_VALUE( m_LuaManager, "DELETE_EFFECT_SET_ON_DAMAGE_REACT",	m_bDeleteEffectSetOnDamageReact,	false );

	LUA_GET_VALUE( m_LuaManager, "DELETE_EFFECT_SET_ON_DIE",	m_bDeleteEffectSetOnDie,	false );

#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
	LUA_GET_VALUE( m_LuaManager, "DELETE_EFFECT_SET_ON_CUSTOM_STATE",	m_bDeleteEffectSetOnCustomState,	false );
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE

#ifdef CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT
	LUA_GET_VALUE( m_LuaManager, "CUSTOM_STATE_DELETE_EFFECT_SET_ON_DAMAGE_REACT",	m_bIsCustomStateDeleteEffectOnDamageReact,	false );
#endif // CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT

	LUA_GET_VALUE( m_LuaManager, "HYPER_EFFECT_SET",		m_bHyperEffectSet,		false );
	LUA_GET_VALUE( m_LuaManager, "EFFECT_SET_LIFE_TIME",	m_fEffectSetLifeTime,	-1.f );

}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


void CX2GameUnit::DeleteEffectSetOnStateEnd()
{
	BOOST_TEST_FOREACH( CX2EffectSet::Handle, hHandle, m_vecEffectSetToDeleteOnStateEnd )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
	}
	m_vecEffectSetToDeleteOnStateEnd.resize(0);
}



void CX2GameUnit::DeleteEffectSetOnDamageReact()
{
	BOOST_TEST_FOREACH( CX2EffectSet::Handle, hHandle, m_vecEffectSetToDeleteOnDamageReact )
	{		
		g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
	}
	m_vecEffectSetToDeleteOnDamageReact.resize(0);
}

/** @function : MakeEffectSetBeDeletedWhenDie
	@brief : GameUnit이 죽었을때 사라져야할 이펙트셋 리스트에 등록하도록 만드는 함수
	@param : 이펙트셋 핸들(hEffectSet_)
*/
void CX2GameUnit::MakeEffectSetBeDeletedWhenDie( const CX2EffectSet::Handle& hEffectSet_ )
{
	if ( INVALID_EFFECTSET_HANDLE != hEffectSet_ )
		m_vecEffectSetToDeleteOnDie.push_back( hEffectSet_ );
}

void CX2GameUnit::DeleteEffectSetOnDie()
{
	BOOST_TEST_FOREACH( CX2EffectSet::Handle, hHandle, m_vecEffectSetToDeleteOnDie )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
	}
	m_vecEffectSetToDeleteOnDie.resize(0);
}

#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
void CX2GameUnit::DeleteEffectSetOnCustomState()
{
	BOOST_TEST_FOREACH( CX2EffectSet::Handle, hHandle, m_vecEffectSetToDeleteOnCustomState )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( hHandle );
	}
	m_vecEffectSetToDeleteOnCustomState.resize(0);
}
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE


#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


void CX2GameUnit::CommonFrameMove_EffectSet( float fNowAnimationTime )
{

	if( true == m_vecEffectSetToPlay.empty() )
		return;


	for( int i=0; i<(int)m_vecEffectSetToPlay.size(); ++i )
	{
		EffectSetToPlay& effectSetToPlay = m_vecEffectSetToPlay[i];
		if( 0.f == effectSetToPlay.m_fStartAnimationTime ||
			fNowAnimationTime >= effectSetToPlay.m_fStartAnimationTime )
		{
			CX2EffectSet::Handle hEffectSetInst = g_pX2Game->GetEffectSet()->PlayEffectSet( effectSetToPlay.m_wstrEffectSetName, this, NULL, m_bHyperEffectSet, GetPowerRate(), m_fEffectSetLifeTime);
			
			// 기존에 애니메이션 시간을 기준으로 이펙트를 생성하던 것을 effectSetToPlay.m_fStartAnimationTime이 추가되면서 애니메이션을 기준으로 하지 않도록 변경해준다
			if( 0.f != effectSetToPlay.m_fStartAnimationTime )
			{
				if ( CX2EffectSet::EffectSetInstance* pEffectSetInst = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectSetInst ) )
					pEffectSetInst->SetUseAnimationTiming( false );
			}
			

			if( true == m_bDeleteEffectSetOnDamageReact )
			{
				m_vecEffectSetToDeleteOnDamageReact.push_back( hEffectSetInst );
			}

			if( true == m_bDeleteEffectSetOnStateEnd )
			{
				m_vecEffectSetToDeleteOnStateEnd.push_back( hEffectSetInst );
			}

			if( true == m_bDeleteEffectSetOnDie )
			{
				m_vecEffectSetToDeleteOnDie.push_back( hEffectSetInst );
			}

#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
			if( true == m_bDeleteEffectSetOnCustomState )
			{
				m_vecEffectSetToDeleteOnCustomState.push_back( hEffectSetInst );
			}
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE

#ifdef MODIFY_EFFECT_SET_DELETE
#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			AddEffectSetDeleteCondition( hEffectSetInst, effectSetToPlay.m_vecDeleteCondition );
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
			AddEffectSetDeleteCondition( hEffectSetInst, effectSetToPlay.m_eDeleteCondition );
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장

			
#endif // MODIFY_EFFECT_SET_DELETE

			m_vecEffectSetToPlay.erase( m_vecEffectSetToPlay.begin() + i );
			--i;
		}
	}
}

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE




#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CX2GameUnit::AddEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CX2GameUnit::AddEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( hDamageEffect == INVALID_DAMAGE_EFFECT_HANDLE )
        return;
	m_setEffectiveBlackHoleDamageEffect.insert( hDamageEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL == pDamageEffect )
		return; 
	m_setEffectiveBlackHoleDamageEffect.insert( pDamageEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CX2GameUnit::RemoveEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CX2GameUnit::RemoveEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( hDamageEffect == INVALID_DAMAGE_EFFECT_HANDLE )
        return;
    m_setEffectiveBlackHoleDamageEffect.erase( hDamageEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL == pDamageEffect )
		return; 
	m_setEffectiveBlackHoleDamageEffect.erase( pDamageEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

void CX2GameUnit::ClearEffectiveBlackHoleDamageEffect()
{
	m_setEffectiveBlackHoleDamageEffect.clear();
}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
bool CX2GameUnit::IsEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
bool CX2GameUnit::IsEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( hDamageEffect == INVALID_DAMAGE_EFFECT_HANDLE )
        return false;

	if( true == m_setEffectiveBlackHoleDamageEffect.empty() )
		return false;

	if( m_setEffectiveBlackHoleDamageEffect.find( hDamageEffect ) == m_setEffectiveBlackHoleDamageEffect.end() )
		return false;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	if( NULL == pDamageEffect )
		return false;

	if( true == m_setEffectiveBlackHoleDamageEffect.empty() )
		return false;

	if( m_setEffectiveBlackHoleDamageEffect.find( pDamageEffect ) == m_setEffectiveBlackHoleDamageEffect.end() )
		return false;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	return true;
}

bool CX2GameUnit::IsThereEffectiveBlackHoleDamageEffect()
{
	if( false == m_setEffectiveBlackHoleDamageEffect.empty() )
		return true;
	else
		return false;
}

//{{ kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)
#ifdef SERV_SECRET_HELL
float	CX2GameUnit::GetStatUpBufferBySocketItem() const
{
	return ( m_StatUpBufferBySocketItem.m_fBufferTime > 0.f ? m_StatUpBufferBySocketItem.m_fBufferRate : 0.0f );
}

void	CX2GameUnit::ProcessStatPercentUpBuffer( CX2GameUnit* pAttacker, OUT CX2Stat &AttackerStat, OUT CX2Stat &DefenderStat )
{
	float fDefenderUpRate = GetStatUpBufferBySocketItem();
	float fAttackerUpRate = pAttacker->GetStatUpBufferBySocketItem();

	if ( fDefenderUpRate > 0.0f )
	{
		switch ( GetStatUpType() )
		{
		case CX2SocketItem::SUT_PHYSIC_DEF:
			{
				DefenderStat.AccessAddOnStat().m_fDefPhysic += fDefenderUpRate;
			} break;
		
		case CX2SocketItem::SUT_MAGIC_DEF:
			{
				DefenderStat.AccessAddOnStat().m_fDefMagic +=	fDefenderUpRate;
			} break;

		case CX2SocketItem::SUT_BOTH_DEF:
		case CX2SocketItem::SUT_ALL:
			{
				DefenderStat.AccessAddOnStat().m_fDefPhysic += fDefenderUpRate;
				DefenderStat.AccessAddOnStat().m_fDefMagic += fDefenderUpRate;
			} break;

		default:
			break;
		}
	}

	if ( fAttackerUpRate > 0.0f )
	{
		switch ( pAttacker->GetStatUpType() )
		{
		case CX2SocketItem::SUT_PHYSIC_ATT:
			{
				AttackerStat.AccessAddOnStat().m_fAtkPhysic +=	fAttackerUpRate;
			} break;
		case CX2SocketItem::SUT_MAGIC_ATT:
			{
				AttackerStat.AccessAddOnStat().m_fAtkMagic += fAttackerUpRate;
			} break;
		case CX2SocketItem::SUT_BOTH_ATT:
		case CX2SocketItem::SUT_ALL:
			{
				AttackerStat.AccessAddOnStat().m_fAtkPhysic += fAttackerUpRate;
				AttackerStat.AccessAddOnStat().m_fAtkMagic += fAttackerUpRate;					
			} break;
		}
	}
}


#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.2 // 비밀던전 작업(셋트아이템효과)

#ifdef SKILL_30_TEST
bool CX2GameUnit::GetEnableDash( ENABLE_DASH_STATE eState /*= EDS_END */ )
{
	if( EDS_END == eState )
	{
		for( int i=0; i<ARRAY_SIZE(m_abEnableDash); ++i )
		{
			if( false == m_abEnableDash[i] )
				return false;
		}

		return true;
	}


	if( eState < 0 || eState >= EDS_END )
		return false;

	return m_abEnableDash[eState];
}

void CX2GameUnit::InitEnableDash()
{
	memset( m_abEnableDash, true, ARRAY_SIZE(m_abEnableDash) );
}

/*virtual*/ void CX2GameUnit::SetEnableDash( bool bEnableDash, ENABLE_DASH_STATE eState /*= EDS_END*/ )
{
	if( EDS_END == eState )
	{
		for( int i=0; i<ARRAY_SIZE(m_abEnableDash); ++i )
		{
			m_abEnableDash[i] = bEnableDash;
		}
		return;
	}


	if( eState < 0 || eState >= EDS_END )
		return;
	
	m_abEnableDash[eState] = bEnableDash;
}



void CX2GameUnit::SetEntangled( const bool val )
{
	m_bEntangled = val;
	m_vEntangledPosition = GetPos();


	if( true == m_bEntangled )
	{
		SetMoveSpeedFactor( 0.f, 100.f );
		SetJumpSpeedFactor( 0.f, 100.f );

		m_PhysicParam.passiveSpeed = D3DXVECTOR2( -1, -1 );
		m_PhysicParam.nowSpeed = D3DXVECTOR2( 0, 0 );


		switch( GetGameUnitType() )
		{
		case GUT_USER:
			{
				if( true == IsMyUnit() )
				{
					g_pX2Game->SetEnableCommandKeyProcess( false );
				}
			} break;

		case GUT_NPC:
			{
				CX2GUNPC* pNPC = (CX2GUNPC*) this;
				pNPC->SetAIEnable( false );
			} break;
		}
	}
	else
	{
		SetMoveSpeedFactor( 0.f, 0.f );
		SetJumpSpeedFactor( 0.f, 0.f );

		ResetPhysicParam();

		switch( GetGameUnitType() )
		{
		case GUT_USER:
			{
				if( true == IsMyUnit() )
				{
					g_pX2Game->SetEnableCommandKeyProcess( true );
				}
			} break;

		case GUT_NPC:
			{
				CX2GUNPC* pNPC = (CX2GUNPC*) this;
				pNPC->SetAIEnable( true );
			} break;
		}

	}
}



void CX2GameUnit::CommonFrameMoveStateAbnormality()
{
	KTDXPROFILE();

#ifdef NO_MORE_ERROR_TEST

	if( true == IsImmuneToExtraDamage() )
	{
		ASSERT( m_ExtraDamagePack.m_Frozen.m_fTime <= 0.f );
		ASSERT( m_ExtraDamagePack.m_Entangled.m_fTime <= 0.f );
		ASSERT( m_ExtraDamagePack.m_TimeBomb.m_fTime <= 0.f );
		ASSERT( m_ExtraDamagePack.m_Aging.m_fTime <= 0.f );
		ASSERT( m_ExtraDamagePack.m_StigmaDebuff.m_fTime <= 0.f );
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
		ASSERT( m_ExtraDamagePack.m_Blind.m_fTime <= 0.f );
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
		ASSERT( m_ExtraDamagePack.m_WaterHold.m_fTime <= 0.f );
#endif EDT_WATER_HOLD_TEST

		//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		ASSERT( m_ExtraDamagePack.m_Panic.m_fTime <= 0.f );
		ASSERT( m_ExtraDamagePack.m_Pain.m_fTime <= 0.f );
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
		//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
		ASSERT( m_ExtraDamagePack.m_LegShot.m_fTime <= 0.f );
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		ASSERT( m_ExtraDamagePack.m_EarthQuake.m_fTime <= 0.f );
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef EVE_ELECTRA
		ASSERT( m_ExtraDamagePack.m_FlashBang.m_fTime <= 0.f );
#endif EVE_ELECTRA

		if( m_ExtraDamagePack.m_Frozen.m_fTime > 0.f ||
			m_ExtraDamagePack.m_Entangled.m_fTime > 0.f ||
			m_ExtraDamagePack.m_TimeBomb.m_fTime > 0.f ||
			m_ExtraDamagePack.m_Aging.m_fTime > 0.f ||
			m_ExtraDamagePack.m_StigmaDebuff.m_fTime > 0.f ||
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
			m_ExtraDamagePack.m_Blind.m_fTime > 0.f ||
#endif NEW_SKILL_2010_11
#ifdef CHUNG_SECOND_CLASS_CHANGE
			m_ExtraDamagePack.m_LegShot.m_fTime > 0.f ||
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
			m_ExtraDamagePack.m_EarthQuake.m_fTime > 0.f ||
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef EVE_ELECTRA
			m_ExtraDamagePack.m_FlashBang.m_fTime > 0.f
#endif EVE_ELECTRA
			)
		{
			InHouseLog( "CX2GameUnit::CommonFrameMoveStateAbnormality()에서 IsImmuneToExtraDamage() 인데 상태이상에 걸린 경우가 있었음." );

			m_ExtraDamagePack.m_Frozen.m_fTime = 0.f;
			m_ExtraDamagePack.m_Entangled.m_fTime = 0.f;
			m_ExtraDamagePack.m_TimeBomb.m_fTime = 0.f;
			m_ExtraDamagePack.m_Aging.m_fTime = 0.f;
			m_ExtraDamagePack.m_StigmaDebuff.m_fTime = 0.f;
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
			m_ExtraDamagePack.m_Blind.m_fTime = 0.f;
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
			m_ExtraDamagePack.m_WaterHold.m_fTime = 0.f;
#endif EDT_WATER_HOLD_TEST

			//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
			m_ExtraDamagePack.m_Panic.m_fTime = 0.f;
			m_ExtraDamagePack.m_Pain.m_fTime = 0.f;
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
			//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
			m_ExtraDamagePack.m_LegShot.m_fTime = 0.f;
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
			m_ExtraDamagePack.m_EarthQuake.m_fTime = 0.f;
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef EVE_ELECTRA
			m_ExtraDamagePack.m_FlashBang.m_fTime = 0.f;
#endif EVE_ELECTRA
		}

	}

#endif NO_MORE_ERROR_TEST

	if( true == IsImmuneToExtraDamage() )
		return;

	CommonFrameMoveStateAbnormalityFrozen();
	CommonFrameMoveStateAbnormalityEntangled();
	CommonFrameMoveStateAbnormalityTimeBomb();
	CommonFrameMoveStateAbnormalityAging();
	CommonFrameMoveStateAbnormalityStigmaDebuff();
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
	CommonFrameMoveStateAbnormalityBlind();
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
	CommonFrameMoveStateAbnormalityWaterHold();
#endif EDT_WATER_HOLD_TEST

	//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	CommonFrameMoveStateAbnormalityPanic();
	CommonFrameMoveStateAbnormalityPain();
#endif	CASH_SKILL_FOR_CHUNG_2011_04
	//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
	CommonFrameMoveStateAbnormalityLegShot();
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
	CommonFrameMoveStateAbnormalityEarthQuake();
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef EVE_ELECTRA
	CommonFrameMoveStateAbnormalityFlashBang();
#endif EVE_ELECTRA

}

void CX2GameUnit::CommonFrameMoveStateAbnormalityEntangled()
{
	KTDXPROFILE();


	if( m_ExtraDamagePack.m_Entangled.m_fTime > 0.f )
	{
		m_ExtraDamagePack.m_Entangled.m_fTime -= m_fElapsedTime;
		if( m_ExtraDamagePack.m_Entangled.m_fTime <= 0.f )
		{
			m_ExtraDamagePack.m_Entangled.m_fTime = 0.f;
		}
	}



	if( m_ExtraDamagePack.m_Entangled.m_fTime > 0.f )
	{
		if( false == GetEntangled() )
		{
			SetEntangled( true );
		}
	}
	else
	{
		if( true == GetEntangled() )
		{
			SetEntangled( false );
		}
	}
}

void CX2GameUnit::CommonFrameMoveStateAbnormalityTimeBomb()
{
	KTDXPROFILE();

	if( m_ExtraDamagePack.m_TimeBomb.m_fTime > 0.f )
	{
		m_ExtraDamagePack.m_TimeBomb.m_fTime -= m_fElapsedTime;
		if( m_ExtraDamagePack.m_TimeBomb.m_fTime <= 0.f )
		{
			m_ExtraDamagePack.m_TimeBomb.m_fTime = 0.f;
		}
	}




	//if( m_ExtraDamagePack.m_TimeBomb.m_fTime > 0.f )
	//{
	//	if( true == GetEnableDash( EDS_LEG_WOUND ) )
	//	{
	//		SetEnableDash( false, EDS_LEG_WOUND );
	//	}
	//}
	//else
	//{
	//	if( false == GetEnableDash( EDS_LEG_WOUND ) )
	//	{
	//		SetEnableDash( true, EDS_LEG_WOUND );
	//	}
	//}


	if( m_ExtraDamagePack.m_TimeBomb.m_fTime > 0.f )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqSkeletonHead )
		{
			m_hSeqSkeletonHead = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Death_State_Deathnote01", 0,0,0 );
		}

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqSkeletonHead )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqSkeletonHead );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
				vEmoticonPos.y += 30.f;
				pSeq->SetPosition( vEmoticonPos );
			}
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqSkeletonHead )
		{
			g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqSkeletonHead );
		}
	}
}

#endif SKILL_30_TEST

void CX2GameUnit::CommonFrameMoveStateAbnormalityAging()
{
	KTDXPROFILE();

	if( m_ExtraDamagePack.m_Aging.m_fTime > 0.f )
	{
		m_ExtraDamagePack.m_Aging.m_fTime -= m_fElapsedTime;
		if( m_ExtraDamagePack.m_Aging.m_fTime <= 0.f )
		{
			m_ExtraDamagePack.m_Aging.m_fTime = 0.f;
		}
	}

	if( m_ExtraDamagePack.m_Aging.m_fTime > 0.f )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqAging )
		{
			m_hSeqAging = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Emoticon_State_Abnormality_Aging", 0,0,0 );
		}

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqAging )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqAging );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
				vEmoticonPos.y += 100.f;
				pSeq->SetPosition( vEmoticonPos );
				pSeq->SetEmitRate( 100, 100 );
			}
			else
			{
				m_hSeqAging = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqAging )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqAging );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hSeqAging = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
}

void CX2GameUnit::CommonFrameMoveStateAbnormalityStigmaDebuff()
{
	KTDXPROFILE();

	if( m_ExtraDamagePack.m_StigmaDebuff.m_fTime > 0.f )
	{
		m_ExtraDamagePack.m_StigmaDebuff.m_fTime -= m_fElapsedTime;
		if( m_ExtraDamagePack.m_StigmaDebuff.m_fTime <= 0.f )
		{
			m_ExtraDamagePack.m_StigmaDebuff.m_fTime = 0.f;
			m_ExtraDamagePack.m_StigmaDebuff.m_fDamageRate = 0.f;
		}
	}





	if( m_ExtraDamagePack.m_StigmaDebuff.m_fTime > 0.f )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hStigma )
		{
			m_hStigma = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Emoticon_State_Abnormality_Stigma", 0,0,0 );
		}

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hStigma )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hStigma );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
				vEmoticonPos.y += 100.f;
				pSeq->SetPosition( vEmoticonPos );
				pSeq->SetEmitRate( 100, 100 );
			}
			else 
			{
				m_hStigma = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hStigma )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hStigma );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hStigma = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
}

#ifdef NEW_SKILL_2010_11
//{{ oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
void CX2GameUnit::CommonFrameMoveStateAbnormalityBlind()
{
	KTDXPROFILE();

	if( m_ExtraDamagePack.m_Blind.m_fTime > 0.f )
	{
		m_ExtraDamagePack.m_Blind.m_fTime -= m_fElapsedTime;
		if( m_ExtraDamagePack.m_Blind.m_fTime <= 0.f )
		{
			m_ExtraDamagePack.m_Blind.m_fTime = 0.f;
		}
	}

	if( m_ExtraDamagePack.m_Blind.m_fTime > 0.f )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqBlind )
		{
			m_hSeqBlind = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Emoticon_State_Abnormality_Blind", 0,0,0 );
		}

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqBlind )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqBlind );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
				vEmoticonPos.y += 100.f;
				pSeq->SetPosition( vEmoticonPos );
				pSeq->SetEmitRate( 100, 100 );
			}
			else
			{
				m_hSeqBlind = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqBlind )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqBlind );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hSeqBlind = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
}
//}} oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
#endif NEW_SKILL_2010_11

//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
/** @function : CommonFrameMoveStateAbnormalityPanic
	@brief : 상태이상 중 일정시간 마다 좌우 반전이 바뀌는 EDT_PANIC 의 FRAMEMOVE로 ExtraDamage에 걸렸을 때 파티클을 생성 하거나, 시간이 종료됬을 때 파티클을 삭제 한다.
*/
void CX2GameUnit::CommonFrameMoveStateAbnormalityPanic()
{
	// PANIC이 작동 중이면
	if( m_ExtraDamagePack.m_Panic.m_fTime > 0.f )
	{
		// 경과시간 만큼 시간을 감소
		m_ExtraDamagePack.m_Panic.m_fTime -= m_fElapsedTime;
		// 0보다 작아지면 
		if( m_ExtraDamagePack.m_Panic.m_fTime <= 0.f )
			m_ExtraDamagePack.m_Panic.m_fTime = 0.f;	// 작동 중지
	}

	// PANIC이 작동 중이면
	if( m_ExtraDamagePack.m_Panic.m_fTime > 0.f )
	{
		// 작동 중인데 파티클의 핸들이 없으면 생성
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqPanic )
			m_hSeqPanic = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Emoticon_State_Abnormality_Panic", 0,0,0 );
		
		// 핸들이 생성되었거나, 있으면 위치 조절
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPanic )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPanic );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
				vEmoticonPos.y += 100.f;
				pSeq->SetPosition( vEmoticonPos );
				pSeq->SetEmitRate( 100, 100 );
			}
			else
			{
				m_hSeqPanic = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
	// PANIC이 중지 되었으면
	else
	{
		// 패닉 파티클의 핸들이 있으면
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPanic )
		{
			// 삭제
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqPanic );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hSeqPanic = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
}

/** @function : CommonFrameMoveStateAbnormalityPain
	@brief : 상태이상 중 ExtraDamage에 걸린 유닛의 몸이 하얗게 변하면서 폭발하는 EDT_PAIN의 FRAMEMOVE로 한번 폭발 하면 폭발 할 수 있는 횟수가 감소 한다.
*/
void CX2GameUnit::CommonFrameMoveStateAbnormalityPain()
{
	// 폭발할 수 있는 횟수가 0보다 크면 (폭발 가능하면 )
	if( 0 < m_ExtraDamagePack.m_Pain.m_Accumulation )
	{
		// 캐릭터를 하얗게 변경 시키기 위해 RENDER_TYPE을 변경
		m_RenderParam.renderType = CKTDGXRenderer::RT_CARTOON_FADE;
		
		// 하얗게 변하게 되면서 폭발 하는데 까지 걸리는 시간 축적
		m_ExtraDamagePack.m_Pain.m_fElapsedTimeToEffectDamage += m_fElapsedTime;

		m_RenderParam.color.a -= m_fElapsedTime * 0.5f;

		// 하얗게 변하면서 2.0초가 되면
#ifdef SKILL_BALANCE_20110728
		if( 3.0f < m_ExtraDamagePack.m_Pain.m_fElapsedTimeToEffectDamage )
#else
		if( 2.0f < m_ExtraDamagePack.m_Pain.m_fElapsedTimeToEffectDamage )
#endif
		{
			// 카운트 감소후
			--m_ExtraDamagePack.m_Pain.m_Accumulation;
			m_ExtraDamagePack.m_Pain.m_fElapsedTimeToEffectDamage = 0.0f;
			m_RenderParam.color.a = 1.0f;

			// 폭발
			if ( null != m_ExtraDamagePack.m_Pain.m_optrGameUnit )
			{
#ifdef BALANCE_PATCH_20120329
				CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( m_ExtraDamagePack.m_Pain.m_optrGameUnit.GetObservable(), L"DAMAGE_EFFECT_CSI_SI_SA_CFG_PAIN_OF_CALADBOLG_EXTRA_DAMAGE", m_ExtraDamagePack.m_Pain.m_Damage, GetBonePos( L"Bip01" ), GetRotateDegree(), GetRotateDegree(),  GetLandPos().y );
#else
				CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( m_ExtraDamagePack.m_Pain.m_optrGameUnit.GetObservable(), L"DAMAGE_EFFECT_CSI_SI_SA_CFG_PAIN_OF_CALADBOLG_EXTRA_DAMAGE", m_ExtraDamagePack.m_Pain.m_pGameUnit->GetPowerRate(), GetBonePos( L"Bip01" ), GetRotateDegree(), GetRotateDegree(),  GetLandPos().y );
#endif

				if ( 0 < m_ExtraDamagePack.m_Pain.m_Accumulation &&
					NULL != pDamageEffect )
					pDamageEffect->GetDamageData().m_ExtraDamage = m_ExtraDamagePack.m_Pain;			
			}
		}

		// 카운트가 0 이면 extraDamage해제
		if ( 0 == m_ExtraDamagePack.m_Pain.m_Accumulation )
		{
#ifdef CHUNG_SECOND_CLASS_CHANGE
			m_ExtraDamagePack.Init(CX2DamageManager::EDT_PAIN);
#else
			m_ExtraDamagePack.m_Pain.Init();
#endif CHUNG_SECOND_CLASS_CHANGE
			// 원래의 렌더 타입으로 돌아가야 하는데... (원래의 렌더 타입이 RT_CARTOON_BLACK_EDGE 아니라면..)
			m_RenderParam.renderType = CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		}
	}
	
}
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
//}} 2011-04 에 패치될 청 캐시 스킬

/** @function 	: CommonFrameMoveStateAbnormalityLegShot
	@brief 		: 
*/
#ifdef CHUNG_SECOND_CLASS_CHANGE
void CX2GameUnit::CommonFrameMoveStateAbnormalityLegShot()
{
	if( m_ExtraDamagePack.m_LegShot.m_fTime > 0.f )
	{
		// 작동 중인데 파티클의 핸들이 없으면 생성
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqLegShot )
			m_hSeqLegShot = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Emoticon_State_Leg_Shot", 0,0,0 );

		// 핸들이 생성되었거나, 있으면 위치 조절
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqLegShot )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqLegShot );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
				vEmoticonPos.y += 100.f;
				pSeq->SetPosition( vEmoticonPos );
				pSeq->SetEmitRate( 100, 100 );
			}
			else
			{
				m_hSeqLegShot = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}

		m_ExtraDamagePack.m_LegShot.m_fTime -= m_fElapsedTime;
		if( m_ExtraDamagePack.m_LegShot.m_fTime <= 0.f )
		{
			m_ExtraDamagePack.m_LegShot.m_fTime = 0.f;
#ifdef CHUNG_SECOND_CLASS_CHANGE
			SetMoveSpeedFactor( 1.f, 0.f );
			SetJumpSpeedFactor( 1.f, 0.f );
#endif CHUNG_SECOND_CLASS_CHANGE
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqLegShot )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqLegShot );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hSeqLegShot = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
}
#endif

#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
void CX2GameUnit::CommonFrameMoveStateAbnormalityEarthQuake()
{
	if( m_ExtraDamagePack.m_EarthQuake.m_fTime > 0.f )
	{
#ifdef X2OPTIMIZE_GAMEOPTION_BUGFIX
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( g_pMain->GetGameOption().GetOptionList().m_bEffect == true )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
#endif  X2OPTIMIZE_GAMEOPTION_BUGFIX
		{
			if( m_hSeqEarthQuakeRight == INVALID_PARTICLE_SEQUENCE_HANDLE )
				m_hSeqEarthQuakeRight = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitSlow", 0,0,0 );
			if( m_hSeqEarthQuakeLeft == INVALID_PARTICLE_SEQUENCE_HANDLE )
				m_hSeqEarthQuakeLeft = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"UnitSlow", 0,0,0 );
		}

		if( m_hSeqEarthQuakeRight != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq 	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqEarthQuakeRight );
			D3DXVECTOR3 vEffectPos = GetPos();
			vEffectPos.y += 20.0f;
			vEffectPos.x += 30.0f;
			if( pSeq != NULL )
			{
				pSeq->SetPosition(vEffectPos);
				pSeq->SetEmitRate( 5,10 );
			}
			else
			{
				m_hSeqEarthQuakeRight = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}

		if( m_hSeqEarthQuakeLeft != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq  	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqEarthQuakeLeft  );
			D3DXVECTOR3 vEffectPos = GetPos();
			vEffectPos.y += 20.0f;
			vEffectPos.x -= 20.0f;
			if( pSeq != NULL )
			{
				pSeq->SetPosition(vEffectPos);
				pSeq->SetEmitRate( 5,10 );
			}
			else
			{
				m_hSeqEarthQuakeLeft = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}

		m_ExtraDamagePack.m_EarthQuake.m_fTime -= m_fElapsedTime;
		if( m_ExtraDamagePack.m_EarthQuake.m_fTime <= 0.f )
		{
			m_ExtraDamagePack.m_EarthQuake.m_fTime = 0.f;
			SetMoveSpeedFactor( 1.f, 0.f );
			SetAnimSpeedFactor( 1.f, 0.f );
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqEarthQuakeRight )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqEarthQuakeRight );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hSeqEarthQuakeRight = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqEarthQuakeLeft )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqEarthQuakeLeft );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hSeqEarthQuakeLeft = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
}
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef SERV_RENA_NIGHT_WATCHER
void CX2GameUnit::CommonFrameMoveStateAbnormalityStartOfDelayedFiring()
{
	bool bIsDeleteData = false;		/// 삭제 여부

	for( UINT i = 0; i < m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.size(); ++i )
	{
        CX2DamageManager::StartOfDelayedFiringData* pArrowData = m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData[ i ];
        ASSERT( pArrowData != NULL );
		if( 0.f < pArrowData->m_fCoolTime )			/// 적용된 화살들의 쿨타임 설정
		{
			pArrowData->m_fCoolTime -= m_fElapsedTime;

			if( 0.f >= pArrowData->m_fCoolTime )		/// 쿨타임 종료시 폭발 동작 실행( 전체 폭발 동작 적용된 화살에 한해서 )
			{
				CX2GameUnit* pAttacker = GetGameUnitByUnitIndex( pArrowData->m_iAttackerIndex );

				if( NULL != pAttacker )
				{
					/// 폭발 위치 설정
					D3DXVECTOR3 vPos	   = GetPos();		/// 현재 유닛 위치
					D3DXVECTOR3 vPosOffSet = pArrowData->m_vExplosionOffSet;		/// 폭파 위치 오프셋

					vPos.x += ( vPosOffSet.x + ( ( 0.5f - GetRandomFloat() ) * 50.f ) );
					vPos.y += ( ( vPosOffSet.y + ( ( 0.5f - GetRandomFloat() ) * 50.f ) ) + 100.f );
					vPos.z += ( vPosOffSet.z + ( ( 0.5f - GetRandomFloat() ) * 50.f ) );

					CX2DamageEffect::CEffect* pCEffect = NULL;

					if( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_READY == pArrowData->m_iType )			/// 폭발의 화살
					{
						/// 폭발 이펙트 생성( 폭발의 화살 )
						pCEffect = g_pX2Game->GetDamageEffect()->CreateInstance( pAttacker, L"ARROW_OF_EXPLOSION_BOOM_DELAY", 
							pAttacker->GetPowerRate(),vPos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0) );
					}
					else if( CX2DamageEffect::DET_START_OF_DELAYED_FIRING_READY == pArrowData->m_iType )	/// 지연의 신호탄
					{
						/// 폭발 이펙트 생성( 지연의 신호탄 )
						pCEffect = g_pX2Game->GetDamageEffect()->CreateInstance( pAttacker, L"START_OF_DELAYED_FIRING_BOOM", 
							pArrowData->m_fSkillPowerRate, vPos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0) );
					}

					if( NULL != pCEffect )
						pCEffect->GetDamageData().damage.fPhysic += pArrowData->m_fIncreaseDamage;
				}

				SAFE_DELETE( pArrowData );
				m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.erase( m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.begin() + i );

				bIsDeleteData = true;

				--i;
			}
		}
	}

	if( true == bIsDeleteData )		/// 삭제한 정보가 있다면, 갱신 처리
	{
		SetStartOfDelayedFiringEmoticon();

		if( true == GetStartOfDelayedFiringData().empty() )		/// 적용된 화살 모두 삭제될 때, 지연의 신호탄 적용중 확인 이펙트 해제
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiringEDT );
	}
}
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_SKILL_NOTE

void CX2GameUnit::ClearAnimSpeedFactor()
{
//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// #ifdef SERV_SECRET_HELL
// 	BOOST_TEST_FOREACH( SPEED_FACTOR&, val, m_PhysicParam.m_vecAnimSpeedFactor )
// 	{
// 		if ( INVALID_EFFECTSET_HANDLE != val.m_hEffectSet )
// 			g_pX2Game->GetEffectSet()->StopEffectSet( val.m_hEffectSet );
// 	}
// #endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)

//	m_PhysicParam.m_vecAnimSpeedFactor.clear();
}

void CX2GameUnit::SetAnimSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId /*= CX2GameUnit::SFI_NONE*/, SPEED_FACTOR_SET_TYPE eSetType /*= SFST_MULTY*/ )
{
// 	switch ( eSetType )
// 	{
// 	case SFST_ONLY:		// vector내에 같은 아이디 요소가 있는 경우 새로 set 하는 factor를 무시하는 타입
// 		{
// 			BOOST_TEST_FOREACH ( SPEED_FACTOR&, val, m_PhysicParam.m_vecAnimSpeedFactor )
// 			{
// 				if ( val.m_eId == eId )		// 중복 되는 게 있으면 무시
// 					return;
// 			}
// 		}
// 		break;
// 
// 	case SFST_UPDATE:	// 기존에 vector 내에 있는 요소를 갱신 시켜주는 타입 (만일 이 타입으로 Set 실행 시에 vector내에 해당 요소 가 없으면 추가 함)
// 		{
// 			BOOST_TEST_FOREACH ( SPEED_FACTOR&, val, m_PhysicParam.m_vecAnimSpeedFactor )
// 			{
// 				if ( val.m_eId == eId )		// 중복 되는 게 있으면 갱신, 없으면 추가
// 				{
// 					val.m_fDuration		= fDuration;
// 					val.m_fSpeedRate	= fSpeedRate;
// 
// 					return;
// 				}
// 			}
// 		}
// 		break;
// 
// 	default:
// 	case SFST_MULTY:	// vector내에 같은 아이디 요소가 여러개 있어도 되는 타입
// 		break;
// 	}
// 
// 	SPEED_FACTOR animSpeedFactor;
// 	animSpeedFactor.m_eId = eId;
// 	animSpeedFactor.m_fDuration = fDuration;
// 	animSpeedFactor.m_fSpeedRate = fSpeedRate;
// 	m_PhysicParam.m_vecAnimSpeedFactor.push_back( animSpeedFactor );
};

void CX2GameUnit::CommonFrameMoveAnimSpeedFactor()
{
// 	for(UINT i=0; i<m_PhysicParam.m_vecAnimSpeedFactor.size(); ++i)
// 	{		
// 		m_PhysicParam.m_vecAnimSpeedFactor[i].m_fDuration -= m_fElapsedTime;
// 
// 		CommonFrameMoveAnimSpeedFactorEffect( m_PhysicParam.m_vecAnimSpeedFactor[i] );
// 
// 		if( m_PhysicParam.m_vecAnimSpeedFactor[i].m_fDuration <= 0.f )
// 		{						
// 			m_PhysicParam.m_vecAnimSpeedFactor.erase( m_PhysicParam.m_vecAnimSpeedFactor.begin() + i );
// 			--i;					
// 		}
// 	}
}

void CX2GameUnit::CommonFrameMoveAnimSpeedFactorEffect(SPEED_FACTOR speedFactor)
{
// 	if(speedFactor.m_fDuration <= 0.f)
// 	{
// 		// 삭제
// 		switch(speedFactor.m_eId)
// 		{
// 		case SFI_SPEED_1:
// 			{
// 				if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSpeedFactor1 )
// 				{
// 					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSpeedFactor1 );
// 					ASSERT( NULL != pSeq );
// 					if( NULL != pSeq )
// 					{
// 						pSeq->SetEmitRate( 0, 0 );
// 						pSeq->ClearAllParticle();
// 					}
// 					else
// 					{
// 						m_hSpeedFactor1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
// 					}
// 				}
// 			}
// 			break;
// 
// 			//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// #ifdef SERV_SECRET_HELL
// 		case SFI_SPEED_2:
// 			{
// 				if ( GetGameUnitType() == GUT_USER )
// 				{
// 					CX2GUUser* pUser = static_cast<CX2GUUser*>( this );
// 					
// 					if ( pUser->GetAfterImage() != NULL && pUser->GetAfterImage()->GetEnable() == true )
// 					{
// 						pUser->GetAfterImage()->Disable();
// 						pUser->GetAfterImage()->SetAfterImageColor( 0xffcccccc );
// 					}
// 				}
// 			}
// 			break;
// #endif SERV_SECRET_HELL
// 			//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// 
// #ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
// 		case SFI_SPEED_10:
// 			{
// 				if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqAuraSpeedAccelerator )
// 				{
// 					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqAuraSpeedAccelerator );
// 					ASSERT( NULL != pSeq );
// 					if( NULL != pSeq )
// 					{
// 						pSeq->SetEmitRate( 0, 0 );
// 						pSeq->ClearAllParticle();
// 					}
// 					else
// 					{
// 						m_hSeqAuraSpeedAccelerator = INVALID_PARTICLE_SEQUENCE_HANDLE;
// 					}
// 				}
// 			}
// 			break;
// #endif
// 		default:
// 			break;
// 		}
// 	}
// 	else
// 	{
// 		// 생성 혹은 유지
// 		switch(speedFactor.m_eId)
// 		{
// 		case SFI_SPEED_1:
// 			{
// 				if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSpeedFactor1 )
// 				{
// 					m_hSpeedFactor1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Emoticon_State_BindingCircle_Memo_DeBuff", 0,0,0 );
// 				}
// 
// 				if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSpeedFactor1 )
// 				{
// 					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSpeedFactor1 );
// 					ASSERT( NULL != pSeq );
// 					if( NULL != pSeq )
// 					{
// 						D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
// 						vEmoticonPos.y += 100.f;
// 						pSeq->SetPosition( vEmoticonPos );
// 						pSeq->SetEmitRate( 100, 100 );
// 					}
// 					else 
// 					{
// 						m_hSpeedFactor1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
// 					}
// 				}
// 			}
// 			break;
// 
// 			//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// #ifdef SERV_SECRET_HELL
// 		case SFI_SPEED_2:
// 			{
// 				if ( GetGameUnitType() == GUT_USER )
// 				{
// 					CX2GUUser* pUser = static_cast<CX2GUUser*>( this );
// 
// 					if ( pUser->GetAfterImage() != NULL && pUser->GetAfterImage()->GetEnable() == false )
// 					{
// 						pUser->GetAfterImage()->SetAfterImageColor( D3DXCOLOR( 0.f, 0.157f, 1.f, 1.f ) );
// 						pUser->GetAfterImage()->Enable();
// 					}
// 				}
// 
// 			}
// 			break;
// #endif SERV_SECRET_HELL
// 			//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// 
// 		default:
// 			break;
// 		}
// 	}
}

void CX2GameUnit::CommonFrameMovePoisonDeBuff()
{
	KTDXPROFILE();	

	if( m_fPoisonDeBuff > 0.f )
	{
		m_fPoisonDeBuff -= m_fElapsedTime;
		if( m_fPoisonDeBuff <= 0.f )
		{
			m_fPoisonDeBuff = 0.f;
#ifdef CHUNG_SECOND_CLASS_CHANGE
			SetMoveSpeedFactor( 1.f, 0.f );
			SetJumpSpeedFactor( 1.f, 0.f );
#endif CHUNG_SECOND_CLASS_CHANGE
		}
	}

	if( m_fPoisonDeBuff > 0.f )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hPoisonDeBuff )
		{
			m_hPoisonDeBuff = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Emoticon_State_DarkCloud_Memo_DeBuff", 0,0,0 );
		}

		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hPoisonDeBuff )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hPoisonDeBuff );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetHeadBonePos();
				vEmoticonPos.y += 100.f;
				pSeq->SetPosition( vEmoticonPos );
				pSeq->SetEmitRate( 100, 100 );
			}
			else
			{
				m_hPoisonDeBuff = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hPoisonDeBuff )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hPoisonDeBuff );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
				pSeq->ClearAllParticle();
			}
			else
			{
				m_hPoisonDeBuff = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
}

void CX2GameUnit::CommonFrameMoveRevengeDefence()
{
	KTDXPROFILE();	

	if( m_fRevengeDefenceTime > 0.f )
	{
		m_fRevengeDefenceTime -= m_fElapsedTime;
		if( m_fRevengeDefenceTime <= 0.f )
		{
			m_fRevengeDefenceTime = 0.f;
		}
	}

	if( m_fRevengeDefenceTime > 0.f )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hRevengeDefence )
		{			
			m_hRevengeDefence = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"BuffShield", 0,0,0, 0, 0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hRevengeDefence );
			if( NULL != pSeq )
				pSeq->SetEmitRate( CMinMax<float>(1000,1000) );
		}
		
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hRevengeDefence )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hRevengeDefence );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 fireBodyEffPos2 = GetPos();
				fireBodyEffPos2.y += 80.0f;
				fireBodyEffPos2.x -= 20.0f;
				pSeq->SetPosition( fireBodyEffPos2 );
			}
			else
			{
				m_hRevengeDefence = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
	else
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hRevengeDefence )
		{
			g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hRevengeDefence );
			m_hRevengeDefence = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}
}

#endif

void CX2GameUnit::PhysicProcessSyncLine( const CKTDGLineMap::LineData* pLineData, D3DXVECTOR3& position )
{
	KTDXPROFILE();

	switch( pLineData->m_pLineAnimData->m_eAnimType )
	{
	default:
		{
			position += pLineData->m_pLineAnimData->m_vChildPivotPositionDisplace;
		} break;
	case CKTDGLineMap::LAT_VIKING:
		{
			if( m_fLastPositionValueOnAnimatedLine > 0.f )
			{
				D3DXVECTOR3 vMovedPosition = pLineData->startPos + pLineData->dirVector * m_fLastPositionValueOnAnimatedLine * pLineData->fLength;
				position += vMovedPosition - m_vLastPositionOnAnimatedLine;
			}
		} break;
	}
}


void CX2GameUnit::CalcLastPositionValueOnAnimatedLine( const CKTDGLineMap::LineData* pLineData, D3DXVECTOR3& position )
{
	KTDXPROFILE();

	switch( pLineData->m_pLineAnimData->m_eAnimType )
	{
	case CKTDGLineMap::LAT_VIKING:
		{
			m_vLastPositionOnAnimatedLine = position;

			D3DXVECTOR3 vRelativePosition = position - pLineData->startPos;
			m_fLastPositionValueOnAnimatedLine = D3DXVec3Length( &vRelativePosition ) / pLineData->fLength;
		} break;
	}
}

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GameUnit::ParseAnimNameFromList( wstring &animName, int iRandRate )
{
	int iAccumulatedRate = 0;

	if( true == m_LuaManager.BeginTable( "ANIM_NAME_LIST" ) )
	{
		WCHAR wszKeyName[64] = L"";
		int iRate = 0;

		for( int i=0; ; ++i )
		{
			swprintf( wszKeyName, sizeof(wszKeyName)/sizeof(WCHAR)-1, L"ANIM_NAME%d", i );
			if( true == m_LuaManager.BeginTable( wszKeyName ) )
			{
				LUA_GET_VALUE( m_LuaManager, 1, animName, L"" );
				LUA_GET_VALUE( m_LuaManager, 2, iRate, 100 );
				m_LuaManager.EndTable(); // ANIM_NAME(i)

				iAccumulatedRate += iRate;
				if( iRandRate < iAccumulatedRate )
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		m_LuaManager.EndTable(); // ANIM_NAMES
	}
}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

bool CX2GameUnit::IsDamagedReaction( const CX2DamageManager::DamageData* pDamageData )
{
	if( NULL == pDamageData )
		return false;

	switch( pDamageData->reActResult )
	{
	case CX2DamageManager::RT_NO_DAMAGE:
	case CX2DamageManager::RT_SENSOR:
	case CX2DamageManager::RT_REVENGE:
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.1] // 룬 슬레이어 - 마법저항 수련
	case CX2DamageManager::RT_REFLECT:
#endif NEW_SKILL_2010_11
	//case CX2DamageManager::RT_GRAPPLE:
	case CX2DamageManager::RT_THROW:
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2DamageManager::RT_CREATE_DAMAGE_EFFECT:
#endif
		{
			return false;
		} break;
	}

	return true;
}


#ifdef STOP_UNIT_STOP_EFFECT_TEST

	void CX2GameUnit::SetEffectStopTime( float fTime, float fRangeSq /*= 1000.f*/ )
	{

#pragma DMLEE_NOTE( "effect set 도 멈춰 있도록 수정해야" )

        D3DXVECTOR vPos = GetPos();
        CX2GameUnit* pThis = this;
#ifdef  LAMBDA_RETURN_OR_LOOP_CONTINUE      
#undef  LAMBDA_RETURN_OR_LOOP_CONTINUE      
#endif  LAMBDA_RETURN_OR_LOOP_CONTINUE     
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    #define LAMBDA_RETURN_OR_LOOP_CONTINUE return
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    #define LAMBDA_RETURN_OR_LOOP_CONTINUE continue
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        auto DamageEffect_StopWithinRangeCB = [fTime, &vPos, fRangeSq, pThis]( CX2DamageEffect::CEffect& kEffect )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		for( int i=0; i< (int)g_pX2Game->GetDamageEffect()->GetInstanceNum() ; ++i )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CX2DamageEffect::CEffect*   pEffect = g_pX2Game->GetDamageEffect()->GetInstance(i);
			if( NULL == pEffect )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;
            CX2DamageEffect::CEffect& kEffect = *pEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			if( pThis != kEffect.GetOwnerUnit() )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;

			D3DXVECTOR3 vDistance = vPos - kEffect.GetPos();
			float fDistanceSq = D3DXVec3LengthSq( &vDistance );

			if( fDistanceSq > fRangeSq )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;

			if( kEffect.GetStopTime() < fTime )
				kEffect.SetStopTime( fTime );
		};
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        g_pX2Game->GetDamageEffect()->ApplyFunctionToLiveInstances( DamageEffect_StopWithinRangeCB );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        auto MeshPlayer_StopWithinRangeCB = [fTime, &vPos, fRangeSq]( CKTDGXMeshPlayer::CXMeshInstance& kMeshInstance )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		for( int i=0; i< (int)g_pX2Game->GetMajorXMeshPlayer()->GetInstanceNum(); ++i )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->GetInstance( i );
			if( NULL == pMeshInstance )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;
            CKTDGXMeshPlayer::CXMeshInstance& kMeshInstance = *pMeshInstance;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			//if( this != pMeshInstance->_internal_element_type )

			D3DXVECTOR3 vDistance = vPos - kMeshInstance.GetPos();
			float fDistanceSq = D3DXVec3LengthSq( &vDistance );

			if( fDistanceSq > fRangeSq )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;

			if( kMeshInstance.GetStopTime() < fTime )
				kMeshInstance.SetStopTime( fTime );
		};
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        g_pX2Game->GetMajorXMeshPlayer()->ApplyFunctionToLiveInstances( MeshPlayer_StopWithinRangeCB );
        g_pX2Game->GetMinorXMeshPlayer()->ApplyFunctionToLiveInstances( MeshPlayer_StopWithinRangeCB );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		for( int i=0; i< (int)g_pX2Game->GetMinorXMeshPlayer()->GetInstanceNum(); ++i )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = g_pX2Game->GetMinorXMeshPlayer()->GetInstance( i );
			if( NULL == pMeshInstance )
				continue;

			//if( this != pMeshInstance->_internal_element_type )

			D3DXVECTOR3 vDistance = GetPos() - pMeshInstance->GetPos();
			float fDistanceSq = D3DXVec3LengthSq( &vDistance );

			if( fDistanceSq > fRangeSq )
				continue;

			if( pMeshInstance->GetStopTime() < fTime )
				pMeshInstance->SetStopTime( fTime );
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        auto ParticleSystem_StopWithinRangesSq = [ fTime, &vPos, fRangeSq ]( CKTDGParticleSystem::CParticleEventSequence& kSequence )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		for( int i=0; i< (int)g_pX2Game->GetMajorParticle()->GetParticleSequenceNum(); ++i )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetParticleSequence( i );
			if( NULL == pSeq )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;
            CKTDGParticleSystem::CParticleEventSequence& kSequence = *pSeq;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

			D3DXVECTOR3 vDistance = vPos - kSequence.GetPosition();
			float fDistanceSq = D3DXVec3LengthSq( &vDistance );

			if( fDistanceSq > fRangeSq )
				LAMBDA_RETURN_OR_LOOP_CONTINUE;

			if( kSequence.GetStopTime() < fTime )
				kSequence.SetStopTime( fTime );
		};
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        g_pX2Game->GetMajorParticle()->ApplyFunctionToLiveInstanceSequences( ParticleSystem_StopWithinRangesSq );
        g_pX2Game->GetMinorParticle()->ApplyFunctionToLiveInstanceSequences( ParticleSystem_StopWithinRangesSq );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		for( int i=0; i< (int)g_pX2Game->GetMinorParticle()->GetParticleSequenceNum(); ++i )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetParticleSequence( i );
			if( NULL == pSeq )
				continue;

			D3DXVECTOR3 vDistance = GetPos() - pSeq->GetPosition();
			float fDistanceSq = D3DXVec3LengthSq( &vDistance );

			if( fDistanceSq > fRangeSq )
				continue;

			if( pSeq->GetStopTime() < fTime )
				pSeq->SetStopTime( fTime );
		}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}

#endif STOP_UNIT_STOP_EFFECT_TEST


	void CX2GameUnit::ClearMoveSpeedFactor()
	{		
		//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// #ifdef SERV_SECRET_HELL
// 		BOOST_TEST_FOREACH( SPEED_FACTOR&, val, m_PhysicParam.m_vecAnimSpeedFactor )
// 		{
// 			if ( INVALID_EFFECTSET_HANDLE != val.m_hEffectSet )
// 				g_pX2Game->GetEffectSet()->StopEffectSet( val.m_hEffectSet );
// 		}
// #endif SERV_SECRET_HELL
		//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)

//		m_PhysicParam.m_vecMoveSpeedFactor.clear();
	}

	void CX2GameUnit::SetVecMoveSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId /*= CX2GameUnit::SFI_NONE*/, SPEED_FACTOR_SET_TYPE eSetType /*= SFST_MULTY*/ )
	{
// 		switch ( eSetType )
// 		{
// 		case SFST_ONLY:		// vector내에 같은 아이디 요소가 있는 경우 새로 set 하는 factor를 무시하는 타입
// 			{
// 				BOOST_TEST_FOREACH ( SPEED_FACTOR&, val, m_PhysicParam.m_vecMoveSpeedFactor )
// 				{
// 					if ( val.m_eId == eId )		// 중복 되는 게 있으면 무시
// 						return;
// 				}
// 			}
// 			break;
// 
// 		case SFST_UPDATE:	// 기존에 vector 내에 있는 요소를 갱신 시켜주는 타입 (만일 이 타입으로 Set 실행 시에 vector내에 해당 요소 가 없으면 추가 함)
// 			{
// 				BOOST_TEST_FOREACH ( SPEED_FACTOR&, val, m_PhysicParam.m_vecMoveSpeedFactor )
// 				{
// 					if ( val.m_eId == eId )		// 중복 되는 게 있으면 갱신, 없으면 추가
// 					{
// 						val.m_fDuration		= fDuration;
// 						val.m_fSpeedRate	= fSpeedRate;
// 
// 						return;
// 					}
// 				}
// 			}
// 			break;
// 
// 		default:
// 		case SFST_MULTY:	// vector내에 같은 아이디 요소가 여러개 있어도 되는 타입
// 			break;
// 		}
// 
// 		SPEED_FACTOR moveSpeedFactor;
// 		moveSpeedFactor.m_eId = eId;
// 		moveSpeedFactor.m_fDuration = fDuration;
// 		moveSpeedFactor.m_fSpeedRate = fSpeedRate;
// 		m_PhysicParam.m_vecMoveSpeedFactor.push_back( moveSpeedFactor );
	}

	void CX2GameUnit::CommonFrameMoveMoveSpeedFactor()
	{
// 		for(UINT i=0; i<m_PhysicParam.m_vecMoveSpeedFactor.size(); ++i)
// 		{		
// 			m_PhysicParam.m_vecMoveSpeedFactor[i].m_fDuration -= m_fElapsedTime;
// 
// 			CommonFrameMoveMoveSpeedFactorEffect( m_PhysicParam.m_vecMoveSpeedFactor[i] );
// 
// 			if( m_PhysicParam.m_vecMoveSpeedFactor[i].m_fDuration <= 0.f )
// 			{						
// 				m_PhysicParam.m_vecMoveSpeedFactor.erase( m_PhysicParam.m_vecMoveSpeedFactor.begin() + i );
// 				--i;					
// 			}
// 		}
	}

	void CX2GameUnit::CommonFrameMoveMoveSpeedFactorEffect(SPEED_FACTOR speedFactor)
	{
		// 이펙트 생성 / 소멸 처리
	}

	void CX2GameUnit::ClearJumpSpeedFactor()
	{		
		//{{ kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)
// #ifdef SERV_SECRET_HELL
// 		BOOST_TEST_FOREACH( SPEED_FACTOR&, val, m_PhysicParam.m_vecJumpSpeedFactor )
// 		{
// 			if ( INVALID_EFFECTSET_HANDLE != val.m_hEffectSet )
// 				g_pX2Game->GetEffectSet()->StopEffectSet( val.m_hEffectSet );
// 		}
// #endif SERV_SECRET_HELL
		//}} kimhc // 2010.4.28 // 비밀던전 작업(엘더 무기 효과)

//		m_PhysicParam.m_vecJumpSpeedFactor.clear();
	}

	void CX2GameUnit::SetVecJumpSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId /*= CX2GameUnit::SFI_NONE*/, SPEED_FACTOR_SET_TYPE eSetType /*= SFST_MULTY*/ )
	{
// 		switch ( eSetType )
// 		{
// 		case SFST_ONLY:		// vector내에 같은 아이디 요소가 있는 경우 새로 set 하는 factor를 무시하는 타입
// 			{
// 				BOOST_TEST_FOREACH ( SPEED_FACTOR&, val, m_PhysicParam.m_vecJumpSpeedFactor )
// 				{
// 					if ( val.m_eId == eId )		// 중복 되는 게 있으면 무시
// 						return;
// 				}
// 			}
// 			break;
// 
// 		case SFST_UPDATE:	// 기존에 vector 내에 있는 요소를 갱신 시켜주는 타입 (만일 이 타입으로 Set 실행 시에 vector내에 해당 요소 가 없으면 추가 함)
// 			{
// 				BOOST_TEST_FOREACH ( SPEED_FACTOR&, val, m_PhysicParam.m_vecJumpSpeedFactor )
// 				{
// 					if ( val.m_eId == eId )		// 중복 되는 게 있으면 갱신, 없으면 추가
// 					{
// 						val.m_fDuration		= fDuration;
// 						val.m_fSpeedRate	= fSpeedRate;
// 
// 						return;
// 					}
// 				}
// 			}
// 			break;
// 
// 		default:
// 		case SFST_MULTY:	// vector내에 같은 아이디 요소가 여러개 있어도 되는 타입
// 			break;
// 		}
// 
// 		SPEED_FACTOR jumpSpeedFactor;
// 		jumpSpeedFactor.m_eId = eId;
// 		jumpSpeedFactor.m_fDuration = fDuration;
// 		jumpSpeedFactor.m_fSpeedRate = fSpeedRate;
// 		m_PhysicParam.m_vecJumpSpeedFactor.push_back( jumpSpeedFactor );
	}

	void CX2GameUnit::RemoveVecMoveSpeedFactor( SPEED_FACTOR_ID eID )
	{
// 		for(UINT i=0; i<m_PhysicParam.m_vecMoveSpeedFactor.size(); ++i)
// 		{
// 			if ( m_PhysicParam.m_vecMoveSpeedFactor[i].m_eId == eID )
// 			{
// 				m_PhysicParam.m_vecMoveSpeedFactor.erase( m_PhysicParam.m_vecMoveSpeedFactor.begin() + i );
// 				return;
// 			}
// 		}
	}

	void CX2GameUnit::CommonFrameMoveJumpSpeedFactor()
	{
// 		for(UINT i=0; i<m_PhysicParam.m_vecJumpSpeedFactor.size(); ++i)
// 		{		
// 			m_PhysicParam.m_vecJumpSpeedFactor[i].m_fDuration -= m_fElapsedTime;
// 
// 			CommonFrameMoveJumpSpeedFactorEffect( m_PhysicParam.m_vecJumpSpeedFactor[i] );
// 
// 			if( m_PhysicParam.m_vecJumpSpeedFactor[i].m_fDuration <= 0.f )
// 			{						
// 				m_PhysicParam.m_vecJumpSpeedFactor.erase( m_PhysicParam.m_vecJumpSpeedFactor.begin() + i );
// 				--i;					
// 			}
// 		}
	}

	void CX2GameUnit::CommonFrameMoveJumpSpeedFactorEffect(SPEED_FACTOR speedFactor)
	{
		// 이펙트 생성 / 소멸 처리
	}


//{{ kimhc // 2010.6.16 
// 1. GameUnit::Weapon::SetEnchantParticleShow() 과 GUUser::SetShowEnchantedWeaponEffectAtHand()을
// 같이 쓰는 경우가 많으므로 한데 묶음
// 2. NotifyShowObjectChanged()에서는 서로 따로 호출하도록 되어있는것을 한곳에서만 호출하도록 수정
// 3. virtual로 만들어서 GameUnit에서는 GameUnit::Weapon::SetEnchantParticleShow()를 호출하고
// GUUser에서는 SetShowEnchantedWeaponEffectAtHand()를 호출하도록 변경
#ifdef	ENCHANT_BUG_TEST

/*virtual*/	void CX2GameUnit::SetShowEnchantWeaponParticle( bool bShow )
{
	BOOST_TEST_FOREACH( CX2GameUnit::Weapon*, pWeapon, m_vecpWeapon )
	{
		pWeapon->SetEnchantParticleShow( bShow );
	}
}

#endif	ENCHANT_BUG_TEST
//}} kimhc // 2010.6.16 



void CX2GameUnit::SetNowHp( float fHPNow )
{
	THEMIDA_ENCODE_START


#ifdef APPLY_THEMIDA
	ENCODE_START
	VM_START
#endif

#ifdef DAMAGE_HISTORY			
	*m_fDamageHistory = GetMaxHp() - fHPNow;
#endif
	GetGageData()->SetNowHp( fHPNow ); 

#ifdef APPLY_THEMIDA
	VM_END
	ENCODE_END
#endif

	THEMIDA_ENCODE_END
}
//{{ kimhc // 2010.11.1 // 로드나이트 - 고통 억제
bool CX2GameUnit::GreaterExtraDamageTime( CX2DamageManager::ExtraDamageData* pFirst, CX2DamageManager::ExtraDamageData *pSecond )
{
	return pFirst->m_fTime > pSecond->m_fTime;
}

void CX2GameUnit::CureExtraDamage( int iNumOfExtraDamageToBeCure_ /*= 1*/ )
{
	// 0.0으로 하지 않은 이유는 각 extra damage 마다 0.0되었을 경우에 처리해줘야 하는 기존의 코드를 수행하기 위해서임
	
	if ( false == m_vecBuffTempletPtr.empty() )
	{
		vector<CX2BuffTempletPtr>::iterator vItr
			= m_vecBuffTempletPtr.begin();
		while ( m_vecBuffTempletPtr.end() != vItr && 0 < iNumOfExtraDamageToBeCure_ )
		{
			if ( NULL != *vItr && (*vItr)->IsDeBuff() )
			{
				EraseBuffTempletFromGameUnitByIterator( vItr );
				--iNumOfExtraDamageToBeCure_;
			}
			else
				++vItr;
		}
	}

	if( 0 < iNumOfExtraDamageToBeCure_ )
	{
		const float REMAIN_EXTRA_DAMAGE_TIME_TO_SET = 0.01f;

		std::sort( &m_pExtraDamageDataList[0], 
			&m_pExtraDamageDataList[NUM_OF_EXTRA_DAMAGE_PACK_MEMBER -1 ], GreaterExtraDamageTime );

		for ( int i = 0; i < iNumOfExtraDamageToBeCure_; ++i )
		{
			if ( m_pExtraDamageDataList[i]->m_fTime > REMAIN_EXTRA_DAMAGE_TIME_TO_SET )
			{
				m_pExtraDamageDataList[i]->m_fTime = REMAIN_EXTRA_DAMAGE_TIME_TO_SET;
#ifdef ELSWORD_WAY_OF_SWORD
				m_pExtraDamageDataList[i]->m_bCured = true;
#endif ELSWORD_WAY_OF_SWORD
			}
		}
	}
}
//}} kimhc // 2010.11.1 // 로드나이트 - 고통 억제
#ifdef ELSWORD_WAY_OF_SWORD
void CX2GameUnit::CureExtraDamage( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageTypeToCure_ )
{
	const float REMAIN_EXTRA_DAMAGE_TIME_TO_SET = 0.01f;
	for ( int i = 0; i < NUM_OF_EXTRA_DAMAGE_PACK_MEMBER; ++i )
	{
		if ( m_pExtraDamageDataList[i]->m_ExtraDamageType == eExtraDamageTypeToCure_ && 
			m_pExtraDamageDataList[i]->m_fTime > REMAIN_EXTRA_DAMAGE_TIME_TO_SET )
		{
			m_pExtraDamageDataList[i]->m_fTime = REMAIN_EXTRA_DAMAGE_TIME_TO_SET;
			m_pExtraDamageDataList[i]->m_bCured = true;
			break;
		}
	}
}
#endif ELSWORD_WAY_OF_SWORD
//{{ 2011-04 에 패치될 청 메모 헤드샷
#ifdef	CHUNG_MEMO_01				

/** @function : ApplyHeadShotDamage
	@brief : 헤드샷이 적용될지를 판단하고, 헤드샷이 적용된 경우 이펙트 출력 및 데미지 증가를 수행
	@param : float* pfFinalDamage_ (데미지의 포인터)
*/
void CX2GameUnit::ApplyHeadShotDamage( float* pfFinalDamage_ )
{
	// EDT_HEAD_SHOT 에 맞았고, 확률 30% 안으로 들어왔다면 헤드샷 데미지 적용
	if ( GetHittedExtraDamageHeadShot() )
	{
		D3DXVECTOR3 vPos( GetHeadBonePos() );
		vPos.x += 130.0f;
		vPos.y += 130.0f;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Chung_Head_Shot_Memo", vPos );

		UpDownCrashCamera( 30.0f, 0.5f );			

		// 헤드샷이 적용되는 경우 2배의 데미지로 증가
		*pfFinalDamage_ *= 2;		
	}

	// 헤드샷이 적용되었다는 플래그를 원래되로 되돌림
	SetHittedExtraDamageHeadShot( false );
}

#endif	CHUNG_MEMO_01				
//}} 2011-04 에 패치될 청 메모 헤드샷

//{{ robobeg : 2008-10-28
//CX2GameUnit::Weapon::Weapon()
CX2GameUnit::Weapon::Weapon( CX2GameUnit& refInOwnerUnit_ ) : CKTDGObject( &refInOwnerUnit_ )
//}} robobeg : 2008-10-28
//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
,m_pItemTempletOrigin( NULL )
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
{
#ifdef ARA_CHARACTER_BASE
	if( refInOwnerUnit_.GetGameUnitType() == CX2GameUnit::GUT_USER
        && NULL != static_cast<CX2GUUser&>(refInOwnerUnit_).GetUnit() )
		InitWeapon( static_cast<CX2GUUser&>(refInOwnerUnit_).GetUnit()->GetType() );
	else
#endif //ARA_CHARACTER_BASE
	{
		InitWeapon();
	}
}

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
/** @function : Constructor
	@brief : SlashTraceData, EnchantData 등을 참조할 pItemTempletOringin_을 전달하여, \n
	m_pUserWeaponEquip은 다른 아이템의 메시 정보등을 참조할 수 있도록 함 (청 광폭화 시 장비교체)
	@param : CX2GameUnit& refInOwnerUnit_, CX2Item::ItemTemplet* pItemTempletOrigin_
*/
CX2GameUnit::Weapon::Weapon( CX2GameUnit& refInOwnerUnit_, 
    const CX2Item::ItemTemplet* pItemTempletOrigin_
    ) : CKTDGObject( &refInOwnerUnit_ )
, m_pItemTempletOrigin( NULL )
{
#ifdef ARA_CHARACTER_BASE
	if( refInOwnerUnit_.GetGameUnitType() == CX2GameUnit::GUT_USER
        && NULL != static_cast<CX2GUUser&>(refInOwnerUnit_).GetUnit() )
		InitWeapon( static_cast<CX2GUUser&>(refInOwnerUnit_).GetUnit()->GetType() );
	else
#endif //ARA_CHARACTER_BASE
	{
		InitWeapon();
	}
	if ( NULL != pItemTempletOrigin_ )
		m_pItemTempletOrigin = pItemTempletOrigin_;
}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

CX2GameUnit::Weapon::~Weapon()
{
	for( UINT i=0; i<m_vecpWeaponParticle.size(); ++i )
	{
		WeaponParticle* pWeaponParticle = m_vecpWeaponParticle[i];
		SAFE_DELETE( pWeaponParticle );
	}
	m_vecpWeaponParticle.clear();


	for( int i=0; i<3; ++i )
	{
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_ahEnchantWeapon[i] );		// weapon enchant particle은 major임을 가정한다.
	}


	SAFE_DELETE_KTDGOBJECT( m_pXSkinWeapon );		// NPC인 경우에 쓰임
	


	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		SAFE_DELETE_KTDGOBJECT( m_pAfterImage[i] );
		SAFE_DELETE_KTDGOBJECT( m_pSlashTrace[i] );
		SAFE_DELETE_KTDGOBJECT( m_pSlashTraceTip[i] );
#ifdef ARA_CHARACTER_BASE
		SAFE_DELETE_KTDGOBJECT( m_pSlashTraceTip2[i] );
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
		SAFE_DELETE_KTDGOBJECT( m_pSlashTraceCustomTexture[i] );
#endif // CUSTOM_SLASH_TRACE_TEXTURE
	}

	// m_pUserWeaponEquip은 여기서 delete하면 안됨, CX2Gameunit쪽에서 delete됨.

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 무기 소멸시 강화 이펙트도 같이 소멸
	DeleteUpgradeWeaponParticle();
#endif ADD_UPGRADE_WEAPON_PARTICLE
}


//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
/** @function : InitWeapon
	@brief : 각 생성자에서 불러줄 Init 함수
*/
#ifdef ARA_CHARACTER_BASE
void CX2GameUnit::Weapon::InitWeapon( CX2Unit::UNIT_TYPE eUnitType_ /*= CX2Unit::UT_NONE */)
#else 
void CX2GameUnit::Weapon::InitWeapon()
#endif //ARA_CHARACTER_BASE
{
	m_iWeaponIndex		= -1;
	//{{ robobeg : 2008-10-28
	//m_pOwnerUnit		= NULL;
	//}} robobeg : 2008-10-28

	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		m_pAfterImage[i]			= NULL;

		m_pSlashTrace[i]			= NULL;
		m_pSlashTraceTip[i]			= NULL;

		m_pFrame_TRACE_START[i]		= NULL;
		m_pFrame_TRACE_END[i]		= NULL;

#ifdef ARA_CHARACTER_BASE
		m_pSlashTraceTip2[i]		= NULL;
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
		m_pSlashTraceCustomTexture[i]	 = NULL;
#endif // CUSTOM_SLASH_TRACE_TEXTURE
	}

	//m_pUserWeaponEquip	= NULL;
	m_pUserWeaponEquip.reset();
	m_pXSkinWeapon		= NULL;
	m_pMatrixWeapon		= NULL;
	m_WeaponRotation	= D3DXVECTOR3(1,1,1);
	m_WeaponScale		= D3DXVECTOR3(1,1,1);

	m_bUseSlashTrace	= false;
	//m_SlashTraceTopBone		= L"";
	//m_SlashTraceBottomBone	= L"";
#ifdef ARA_CHARACTER_BASE
	switch( eUnitType_ )
	{
	case CX2Unit::UT_ARA:
		{
			m_coSlashTrace		= 0xffcc9933;
			m_coSlashTraceTip	= 0xffcc9933;
		} break;
	case CX2Unit::UT_ELESIS:
		{
			m_coSlashTrace		= 0xfffa7819;
			m_coSlashTraceTip	= 0xfffa7819;
		} break;
	default:
		{
			m_coSlashTrace		= 0xcc5555ff;
			m_coSlashTraceTip	= 0xff5555ff;
		} break;
	}
#else
	m_coSlashTrace				= 0xcc5555ff;
	m_coSlashTraceTip			= 0xff5555ff;
#endif //ARA_CHARACTER_BASE
	m_coSlashTraceHyper 		= 0xccff3333;
	m_coSlashTraceTipHyper		= 0xffff3333;

	//{{ robobeg : 2008-10-28
	//m_bAlphaObject = false;
	SetAlphaObject( false );
	//}} robobeg : 2008-10-28
	m_bEnable = true;

	//{{ kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07
	SetZEnable( false );
	//}} kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07

	for( int i=0; i<3; ++i )
	{
		m_ahEnchantWeapon[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}

	m_bTraceColor = false;
	m_vTraceColor = 0xcc5555ff;
	m_vTraceTipColor = 0xff5555ff;

	//{{ JHKang / 2011.03.23 / Blend 값 지정
#ifdef ADD_WEAPON_BLEND_VALUE
	m_d3dSrcBlend = D3DBLEND_SRCALPHA;
	m_d3dDestBlend = D3DBLEND_DESTALPHA;
#endif
	//}}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE			/// 무기 강화 이펙트 객체
	m_pFrame_ATTACK_LINE_END0	= NULL;
	m_pFrame_ATTACK_LINE_START0_FRONT = NULL;

	m_hSeqUpgradeWeapon	 = INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqUpgradeWeapon2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqUpgradeWeapon3 = INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqUpgradeWeapon4 = INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif ADD_UPGRADE_WEAPON_PARTICLE
}
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
#ifdef FIX_SLASH_TRACE
void CX2GameUnit::Weapon::InitSlashTrace()
{
	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		SAFE_DELETE_KTDGOBJECT( m_pAfterImage[i] );
		SAFE_DELETE_KTDGOBJECT( m_pSlashTrace[i] );
		SAFE_DELETE_KTDGOBJECT( m_pSlashTraceTip[i] );
#ifdef ARA_CHARACTER_BASE
		SAFE_DELETE_KTDGOBJECT( m_pSlashTraceTip2[i] );
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
		SAFE_DELETE_KTDGOBJECT( m_pSlashTraceCustomTexture[i] );
#endif // CUSTOM_SLASH_TRACE_TEXTURE
	}
}
#endif

void CX2GameUnit::Weapon::EnchantParticleFrameMoveForEve()
{
	KTDXPROFILE();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_ahEnchantWeapon[0] )
		return; 

	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;


	CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );

	if ( pSeqEnchant1 != NULL )
	{
#ifdef RIDING_SYSTEM
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( true == GetShowObject() && true == GetOwnerUnit().CanNotShowEnchantedWeaponEffectToRiding() )
#else RIDING_SYSTEM
		if( true == GetShowObject() )
#endif RIDING_SYSTEM
		{
			D3DXVECTOR3 vBonePos1 = GetOwnerUnit().GetBonePos( L"Dummy1_Rhand" );
			pSeqEnchant1->SetPosition( vBonePos1 );
			pSeqEnchant1->SetEmitRate( 10, 10 );
		}
		else
		{

			pSeqEnchant1->SetEmitRate( 0, 0 );
		}
	}

}


void CX2GameUnit::Weapon::EnchantParticleFrameMoveForBow()
{
	KTDXPROFILE();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_ahEnchantWeapon[0] )
		return; 

	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;


	CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );
	if( NULL != pSeqEnchant1 )
	{
#ifdef RIDING_SYSTEM
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( true == GetShowObject() && true == GetOwnerUnit().CanNotShowEnchantedWeaponEffectToRiding() )
#else RIDING_SYSTEM
		if( true == GetShowObject() )
#endif RIDING_SYSTEM
		{
			D3DXVECTOR3 vBonePos = GetOwnerUnit().GetBonePos( L"Dummy2_Lhand" );
			pSeqEnchant1->SetPosition( vBonePos );
			pSeqEnchant1->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeqEnchant1->SetEmitRate( 0, 0 );
		}
	}
}

void CX2GameUnit::Weapon::EnchantParticleFrameMoveForSword()
{
	KTDXPROFILE();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_ahEnchantWeapon[0] )
		return;

	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;


	CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );
	CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant2 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[1] );
	CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant3 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[2] );

	if( pSeqEnchant1 != NULL && 
		pSeqEnchant2 != NULL && 
		pSeqEnchant3 != NULL )
	{
#ifdef RIDING_SYSTEM
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( true == GetShowObject() && true == GetOwnerUnit().CanNotShowEnchantedWeaponEffectToRiding() )
#else RIDING_SYSTEM
		if( true == GetShowObject() )
#endif RIDING_SYSTEM
		{
			D3DXVECTOR3 weaponPos;
			GetFramePos( &weaponPos, m_pFrame_TRACE_END[0] );

			D3DXVECTOR3 traceEndPos;
			GetFramePos( &traceEndPos, m_pFrame_TRACE_START[0] );

			D3DXVECTOR3 traceCenterPos = traceEndPos - weaponPos;
			traceCenterPos.x = traceCenterPos.x/2.0f + weaponPos.x;
			traceCenterPos.y = traceCenterPos.y/2.0f + weaponPos.y;
			traceCenterPos.z = traceCenterPos.z/2.0f + weaponPos.z;


			pSeqEnchant1->SetPosition( traceCenterPos );
			pSeqEnchant2->SetPosition( weaponPos );
			pSeqEnchant3->SetPosition( traceEndPos );


			pSeqEnchant1->SetEmitRate( 10, 10 );
			pSeqEnchant2->SetEmitRate( 10, 10 );
			pSeqEnchant3->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeqEnchant1->SetEmitRate( 0, 0 );
			pSeqEnchant2->SetEmitRate( 0, 0 );
			pSeqEnchant3->SetEmitRate( 0, 0 );
		}
	}
}

#ifdef ARA_CHARACTER_BASE
void CX2GameUnit::Weapon::EnchantParticleFrameMoveForAra()
{
	KTDXPROFILE();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_ahEnchantWeapon[0] )
		return; 

	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;


	CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );
	if( NULL != pSeqEnchant1 )
	{
#ifdef RIDING_SYSTEM
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( true == GetShowObject() && true == GetOwnerUnit().CanNotShowEnchantedWeaponEffectToRiding() )
#else RIDING_SYSTEM
		if( true == GetShowObject() )
#endif RIDING_SYSTEM
		{
			D3DXVECTOR3 vBonePos = GetOwnerUnit().GetWeaponBonePos( 0, L"DummyTopPoint", 0 );
			pSeqEnchant1->SetPosition( vBonePos );
			pSeqEnchant1->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeqEnchant1->SetEmitRate( 0, 0 );
		}
	}
}
#endif


#ifdef SERV_9TH_NEW_CHARACTER // 김태환
void CX2GameUnit::Weapon::EnchantParticleFrameMoveForAdd()
{
	KTDXPROFILE();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_ahEnchantWeapon[0] )
		return; 

	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;


	CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );

	if ( pSeqEnchant1 != NULL )
	{
#ifdef RIDING_SYSTEM
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( true == GetShowObject() && true == GetOwnerUnit().CanNotShowEnchantedWeaponEffectToRiding() )
#else RIDING_SYSTEM
		if( true == GetShowObject() )
#endif RIDING_SYSTEM
		{
			D3DXVECTOR3 vBonePos1 = GetOwnerUnit().GetBonePos( L"Weapon05" );
			pSeqEnchant1->SetPosition( vBonePos1 );
			pSeqEnchant1->SetEmitRate( 10, 10 );
		}
		else
		{

			pSeqEnchant1->SetEmitRate( 0, 0 );
		}
	}

}
#endif //SERV_9TH_NEW_CHARACTER


void CX2GameUnit::Weapon::CreateEnchantParticle( const WCHAR* pName1, const WCHAR* pName2, const WCHAR* pName3 )
{
	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	if( NULL != pName1 )
	{
		if ( m_ahEnchantWeapon[0] == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_ahEnchantWeapon[0] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  pName1, 0,0,0 );
			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );
			if( pSeq != NULL )
				pSeq->SetEmitRate( 0, 0 );
		}
	}
	if( NULL != pName2 )
	{
		if ( m_ahEnchantWeapon[1] == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_ahEnchantWeapon[1] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  pName2, 0,0,0 );
			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[1] );
			if( pSeq != NULL )
				pSeq->SetEmitRate( 0, 0 );
		}
	}

	if( NULL != pName3 )
	{
		if ( m_ahEnchantWeapon[2] == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_ahEnchantWeapon[2] = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  pName3, 0,0,0 );
			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[2] );
			if( pSeq != NULL )
				pSeq->SetEmitRate( 0, 0 );
		}
	}
}



// 엘소드, 아이샤, 레이븐의 경우 무기를 오른손에 들고 칼 형태이다
void CX2GameUnit::Weapon::InitEnchantParticleForSword()
{
	// attribute prefix 에 대한 이펙트 생성

	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;

	switch( GetOwnerUnit().GetEnchantExtraDamageType() )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:
		{
			CreateEnchantParticle( L"Weapon_Elsword_Property01_Fire", 
				L"Weapon_Elsword_Property01_Fire2",
				L"Weapon_Elsword_Property01_Fire2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			CreateEnchantParticle( L"Weapon_Elsword_Property01_M_Fire", 
				L"Weapon_Elsword_Property01_M_Fire2",
				L"Weapon_Elsword_Property01_M_Fire2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:
		{
			CreateEnchantParticle( L"Weapon_Elsword_Property01_Ice", 
				L"Weapon_Elsword_Property01_Ice2",
				L"Weapon_Elsword_Property01_Ice2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			CreateEnchantParticle( L"Weapon_Elsword_Property01_M_Ice", 
				L"Weapon_Elsword_Property01_M_Ice2",
				L"Weapon_Elsword_Property01_M_Ice2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON:
	case CX2DamageManager::EDT_ENCHANT_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SNATCH:
		{
			CreateEnchantParticle( L"Weapon_Elsword_Property01_Poison", 
				L"Weapon_Elsword_Property01_Poison2",
				L"Weapon_Elsword_Property01_Poison2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			CreateEnchantParticle( L"Weapon_Elsword_Property01_M_Poison", 
				L"Weapon_Elsword_Property01_M_Poison2",
				L"Weapon_Elsword_Property01_M_Poison2" );
		} break;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GameUnit::Weapon::AppendEnchantParticleForSwordToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	if ( g_pData->GetGameMajorParticle() == NULL )
		return;

	switch( eDamageType_ )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_Fire" );
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_Fire2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_M_Fire" );
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_M_Fire2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_Ice" );
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_Ice2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_M_Ice" );
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_M_Ice2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON:
	case CX2DamageManager::EDT_ENCHANT_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SNATCH:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_Poison" );
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_Poison2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_M_Poison" );
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Elsword_Property01_M_Poison2" );
		} break;
	}
}

void CX2GameUnit::Weapon::AppendEnchantParticleForBowToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	// attribute prefix 에 대한 이펙트 생성

	if ( g_pData->GetGameMajorParticle() == NULL )
		return;

	switch( eDamageType_ )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Lire_Property01_Fire" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Lire_Property01_M_Fire" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Lire_Property01_Ice" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Lire_Property01_M_Ice" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON:
	case CX2DamageManager::EDT_ENCHANT_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SNATCH:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Lire_Property01_Poison" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"Weapon_Lire_Property01_M_Poison" );
		} break;
	}
}


#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


// 레나는 무기를 왼손에 들고 활이다
void CX2GameUnit::Weapon::InitEnchantParticleForBow()
{
	// attribute prefix 에 대한 이펙트 생성


	if( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;

	switch( GetOwnerUnit().GetEnchantExtraDamageType() )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:
		{
			CreateEnchantParticle( L"Weapon_Lire_Property01_Fire", 
				NULL, NULL );
		} break;

	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			CreateEnchantParticle( L"Weapon_Lire_Property01_M_Fire", 
				NULL, NULL );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:
		{
			CreateEnchantParticle( L"Weapon_Lire_Property01_Ice", 
				NULL, NULL );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			CreateEnchantParticle( L"Weapon_Lire_Property01_M_Ice", 
				NULL, NULL );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON:
	case CX2DamageManager::EDT_ENCHANT_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SNATCH:
		{
			CreateEnchantParticle( L"Weapon_Lire_Property01_Poison", 
				NULL, NULL );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			CreateEnchantParticle( L"Weapon_Lire_Property01_M_Poison", 
				NULL, NULL );
		} break;
	}
}




void CX2GameUnit::Weapon::SetEnchantParticleShow(bool bShow)
{
	CKTDGParticleSystem::CParticleEventSequence* pSeqWeapon;
	pSeqWeapon = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );
	if( pSeqWeapon != NULL )
		pSeqWeapon->SetShowObject(bShow);
	pSeqWeapon = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[1] );
	if( pSeqWeapon != NULL )
		pSeqWeapon->SetShowObject(bShow);
	pSeqWeapon = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[2] );
	if( pSeqWeapon != NULL )
		pSeqWeapon->SetShowObject(bShow);
}

//{{ kimhc // 실시간 엘소드 중 실시간 장비 교체
#ifdef REAL_TIME_ELSWORD
void CX2GameUnit::Weapon::DeleteEnchantParticle()
{
	for( int i = 0; i < 3; ++i )
	{
		if ( m_ahEnchantWeapon[i] != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_ahEnchantWeapon[i] );
			m_ahEnchantWeapon[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}		
	}
}
#endif REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 실시간 장비 교체

void CX2GameUnit::Weapon::UpdateSlashTraceTextureName()
{
	if( CX2GameUnit::GUT_USER != GetOwnerUnit().GetGameUnitType() ) 
		return;

	CX2GUUser* pUser = (CX2GUUser*) &GetOwnerUnit();

	if( NULL == pUser->GetMotion() )
		return;


	bool bFashion = false;
//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	// m_pItemTempletOrigin이 있으면 bFashion정보를 Templet에서 참조함
	if ( NULL != m_pItemTempletOrigin )
		bFashion = m_pItemTempletOrigin->GetFashion();
	else
	{
		if( m_pUserWeaponEquip == NULL ||
			NULL == m_pUserWeaponEquip->GetItem() ||
			NULL == m_pUserWeaponEquip->GetItem()->GetItemTemplet() 
            )
		{
			return;
		}

		bFashion = m_pUserWeaponEquip->GetItem()->GetItemTemplet()->GetFashion();
	}
#else	NEW_CHARACTER_CHUNG
	if( m_pUserWeaponEquip == NULL ||
		NULL == m_pUserWeaponEquip->GetItem() ||
			NULL == m_pUserWeaponEquip->GetItem()->GetItemTemplet() 
            )
	{
		ASSERT( !L"UpdateSlashTraceTextureName Failed" );
		return;
	}

	bFashion = m_pUserWeaponEquip->GetItem()->GetItemTemplet()->GetFashion();
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
	

	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		// 무기 slash trace 생성
		if( NULL == m_pSlashTrace[i] )
			continue;

		CX2EnchantItem::ItemEnchantedAttribute enchantedAttribute;
		if( true == bFashion )
		{
			CX2Item* pItemNotFashion = pUser->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
			if( NULL != pItemNotFashion )
			{
				enchantedAttribute = pItemNotFashion->GetItemData().m_EnchantedAttribute;
			}
		}
		else
		{
			enchantedAttribute = m_pUserWeaponEquip->GetItem()->GetItemData().m_EnchantedAttribute;
		}

		CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( enchantedAttribute );
		switch( extraDamageType )
		{
		case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
			{
				m_pSlashTrace[i]->SetDefaultSlashTraceTextureName( CX2EnchantItem::GetSlashTraceTextureName( pUser->GetUnit()->GetType(), extraDamageType ) );
			} break;
		}
	}
}

















bool CX2GameUnit::Weapon::LoadDataForUser( KLuaManager& luaManager )
{
//{{ robobeg : 2008-10-28
	//if( NULL == GetOwnerUnit() )
	//	return false;
//}} robobeg : 2008-10-28

	CX2GUUser* pUser = (CX2GUUser*) &GetOwnerUnit();

	if( NULL == pUser->GetMotion() )
		return false;

	//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	if( m_pUserWeaponEquip == NULL ||
		NULL == m_pUserWeaponEquip->GetItem() ||
		NULL == m_pUserWeaponEquip->GetItem()->GetItemTemplet()
        )
	{
		return false;
	}
#else	NEW_CHARACTER_CHUNG
	if( m_pUserWeaponEquip == NULL )
		return false;
#endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

	


	// 무기 slash trace 기본 데이타

	float fTexturedSlashTraceDisableTime;
	LUA_GET_VALUE( luaManager, "TEXTURED_SLASH_TRACE_DISABLE_TIME",		fTexturedSlashTraceDisableTime,		0.3f );

	float fSlashTraceDisableTime;
	float fSlashTraceTipDisableTime;
	LUA_GET_VALUE( luaManager, "SLASH_TRACE_DISABLE_TIME",			fSlashTraceDisableTime,		0.4f );
	LUA_GET_VALUE( luaManager, "SLASH_TRACE_TIP_DISABLE_TIME",		fSlashTraceTipDisableTime,	0.5f );
	int slashTraceDrawCount;
	int slashTraceTipDrawCount;
	LUA_GET_VALUE( luaManager, "SLASH_TRACE_DRAW_COUNT",			slashTraceDrawCount,		1 );
	LUA_GET_VALUE( luaManager, "SLASH_TRACE_TIP_DRAW_COUNT",		slashTraceTipDrawCount,		3 );

	wstring slashTraceTopBone = L"";
	wstring slashTraceBottomBone = L"";
	LUA_GET_VALUE( luaManager, "SLASH_TRACE_TOP_BONE",				slashTraceTopBone,			L"TRACE_START0" );
	LUA_GET_VALUE( luaManager, "SLASH_TRACE_BOTTOM_BONE",			slashTraceBottomBone,		L"TRACE_END0" );
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
	wstring wstrCustomSlashTexture = L"";
	LUA_GET_VALUE( luaManager, "CUSTOM_SLASH_TRACE_TEXTURE",		wstrCustomSlashTexture,		L"" )
#endif // CUSTOM_SLASH_TRACE_TEXTURE
	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		// 무기 after image 생성
		if( NULL != m_pUserWeaponEquip->GetNormalModel(i) )
		{
			// 밑에 NULL이 아닐경우에도 새로생성해서 덮어씌우는 현상에 대한 방어코드 추가
			if( m_pAfterImage[i] != NULL )
			{
				SAFE_DELETE_KTDGOBJECT( m_pAfterImage[i] );
			}

			// fix!! hyper mode일 경우 hyper model의 after image가 나와야 함
            ASSERT( m_pAfterImage[i] == NULL );

			m_pAfterImage[i] = CKTDGXSkinAfterImage::CreateSkinAfterImage( m_pUserWeaponEquip->GetNormalAnim(i), 10, XL_EFFECT_0 );
			if( m_pAfterImage[i] != NULL )
			{
				m_pAfterImage[i]->SetAfterImageColor( 0xffcccccc );
				m_pAfterImage[i]->AddMesh( m_pUserWeaponEquip->GetNormalModel(i) );
			}
		}

		if ( NULL != m_pUserWeaponEquip->GetNormalAnim(i) )
		{
			bool bTexturedSlashTrace = false;		// SlashTrace에 Texture를 사용할 것인가?
			wstring slashTraceTextureName = L"";	// 사용할 텍스쳐 명
			float slashTraceTextureStride = 0.f;	// 2010-12-13 // 현재 사용되고 있지 않음

			//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG

			// Trace에 텍스쳐 붙임
		#ifdef	TEXTURED_SLASH_TRACE_TEST

			bool bFashion = false; // 패션 아이템인가?(아바타 등)
			if ( NULL != m_pItemTempletOrigin )
				bFashion = m_pItemTempletOrigin->GetFashion();
			else
				bFashion = m_pUserWeaponEquip->GetItem()->GetItemTemplet()->GetFashion();

			CX2EnchantItem::ItemEnchantedAttribute enchantedAttribute;
#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP

			bool	   bUseChangeWeapon = false;	/// 아이샤나 청의 무기 변경 여부
			CX2GUUser* pUserUnit		= NULL;

			if( CX2GameUnit::GUT_USER == GetOwnerUnit().GetGameUnitType() )
			{
				pUserUnit = static_cast<CX2GUUser*>( &GetOwnerUnit() );
				if( NULL != pUserUnit && true == g_pX2Game->IsValidUnit( pUserUnit ) )
					bUseChangeWeapon = pUserUnit->GetUseChangeWeapon();			/// 무기 변경 여부 반환
			}

			if( true == bUseChangeWeapon || true == bFashion )		/// 아이샤나 청의 무기가 변경될 경우 해당 구문 무조건 적용( 특정 트레이스 적용 않되는 버그 때문에 )
#else  FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
			if( true == bFashion )
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
			{
				CX2Item* pItemNotFashion = pUser->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );

				if( NULL != pItemNotFashion )
				{
					enchantedAttribute = pItemNotFashion->GetItemData().m_EnchantedAttribute;
				}

#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
				if( NULL != pUserUnit && true == g_pX2Game->IsValidUnit( pUserUnit ) )
					pUserUnit->SetUseChangeWeapon( false );		/// 무기 변경 여부 초기화
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
			}
			else
			{
				enchantedAttribute = m_pUserWeaponEquip->GetItem()->GetItemData().m_EnchantedAttribute;
			}


			CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( enchantedAttribute );
			switch( extraDamageType )
			{
			case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
			case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
			case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
				{
					bTexturedSlashTrace = true;
					slashTraceTextureName = CX2EnchantItem::GetSlashTraceTextureName( pUser->GetUnit()->GetType(), extraDamageType );
					slashTraceTextureStride = 400.f;
				} break;
			}
		#endif	TEXTURED_SLASH_TRACE_TEST

#else	NEW_CHARACTER_CHUNG

		#ifdef TEXTURED_SLASH_TRACE_TEST
			if( 0 != m_pUserWeaponEquip->GetItemUID() &&
				NULL != m_pUserWeaponEquip->GetItem() )
			{

				CX2EnchantItem::ItemEnchantedAttribute enchantedAttribute;
				if( true == m_pUserWeaponEquip->GetItem()->GetItemTemplet()->GetFashion() )
				{
					CX2Item* pItemNotFashion = pUser->GetUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );

					if( NULL != pItemNotFashion )
					{
						enchantedAttribute = pItemNotFashion->GetItemData().m_EnchantedAttribute;
					}
				}
				else
				{
					enchantedAttribute = m_pUserWeaponEquip->GetItem()->GetItemData().m_EnchantedAttribute;
				}


				CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( enchantedAttribute );
				switch( extraDamageType )
				{
				case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
				case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
				case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
					{
						bTexturedSlashTrace = true;
						slashTraceTextureName = CX2EnchantItem::GetSlashTraceTextureName( pUser->GetUnit()->GetType(), extraDamageType );
						slashTraceTextureStride = 400.f;
					} break;
				}
			}


		#endif TEXTURED_SLASH_TRACE_TEST

#endif	NEW_CHARACTER_CHUNG
			
			m_pFrame_TRACE_START[i]		= m_pUserWeaponEquip->GetNormalAnim(i)->GetCloneFrame( slashTraceTopBone.c_str() );
			m_pFrame_TRACE_END[i]		= m_pUserWeaponEquip->GetNormalAnim(i)->GetCloneFrame( slashTraceBottomBone.c_str() );

			if( NULL == m_pSlashTrace[i] ) 
			{
				if( true == GetOwnerUnit().IsMyUnit() )
					m_pSlashTrace[i] = CKTDGSlashTrace::CreateSlashTrace( 400, bTexturedSlashTrace, slashTraceTextureStride, slashTraceTextureName );
				else
					m_pSlashTrace[i] = CKTDGSlashTrace::CreateSlashTrace( 150, bTexturedSlashTrace, slashTraceTextureStride, slashTraceTextureName );

				m_pSlashTrace[i]->SetCenter( pUser->GetMotion()->GetCenter() );
				m_pSlashTrace[i]->SetBoundingRadius( pUser->GetMotion()->GetBoundingRadius() * 1.5f );

				if( true == bTexturedSlashTrace )
				{
					m_pSlashTrace[i]->SetDisableTime( fTexturedSlashTraceDisableTime );
				}
				else
				{
					m_pSlashTrace[i]->SetDisableTime( fSlashTraceDisableTime );
				}

				m_pSlashTrace[i]->SetDrawCount( slashTraceDrawCount );
				m_pSlashTrace[i]->SetLayer( XL_EFFECT_0 );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_pSlashTrace[i] );
			}


			if( NULL == m_pSlashTraceTip[i] &&
				false == bTexturedSlashTrace ) 
			{
				if( true == GetOwnerUnit().IsMyUnit() )
					m_pSlashTraceTip[i] = CKTDGSlashTrace::CreateSlashTrace( 400, bTexturedSlashTrace, slashTraceTextureStride, slashTraceTextureName );
				else
					m_pSlashTraceTip[i] = CKTDGSlashTrace::CreateSlashTrace( 150, bTexturedSlashTrace, slashTraceTextureStride, slashTraceTextureName );

				m_pSlashTraceTip[i]->SetCenter( pUser->GetMotion()->GetCenter() );
				m_pSlashTraceTip[i]->SetBoundingRadius( pUser->GetMotion()->GetBoundingRadius() * 1.5f );

				m_pSlashTraceTip[i]->SetDisableTime( fSlashTraceTipDisableTime );
				m_pSlashTraceTip[i]->SetDrawCount( slashTraceTipDrawCount );
				m_pSlashTraceTip[i]->SetLayer( XL_EFFECT_0 );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_pSlashTraceTip[i] );
			}
#ifdef ARA_CHARACTER_BASE
			if( NULL == m_pSlashTraceTip2[i] ) 
			{
				if( true == GetOwnerUnit().IsMyUnit() )
					m_pSlashTraceTip2[i] = CKTDGSlashTrace::CreateSlashTrace( 100, bTexturedSlashTrace, slashTraceTextureStride, slashTraceTextureName );
				else
					m_pSlashTraceTip2[i] = CKTDGSlashTrace::CreateSlashTrace( 50, bTexturedSlashTrace, slashTraceTextureStride, slashTraceTextureName );

				m_pSlashTraceTip2[i]->SetCenter( pUser->GetMotion()->GetCenter() );
				m_pSlashTraceTip2[i]->SetBoundingRadius( pUser->GetMotion()->GetBoundingRadius() * 0.5f );

				m_pSlashTraceTip2[i]->SetDisableTime( 0.2f );
				m_pSlashTraceTip2[i]->SetDrawCount( 1 );
				m_pSlashTraceTip2[i]->SetLayer( XL_EFFECT_0 );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_pSlashTraceTip2[i] );
			}
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
			if( false == wstrCustomSlashTexture.empty() &&
				NULL == m_pSlashTraceCustomTexture[i] ) 
			{
				if( true == GetOwnerUnit().IsMyUnit() )
					m_pSlashTraceCustomTexture[i] = CKTDGSlashTrace::CreateSlashTrace( 400, true, 400.f, wstrCustomSlashTexture );
				else
					m_pSlashTraceCustomTexture[i] = CKTDGSlashTrace::CreateSlashTrace( 150, true, 150.f, wstrCustomSlashTexture );

				m_pSlashTraceCustomTexture[i]->SetCenter( pUser->GetMotion()->GetCenter() );
				m_pSlashTraceCustomTexture[i]->SetBoundingRadius( pUser->GetMotion()->GetBoundingRadius() * 1.5f );

				if( true == bTexturedSlashTrace )
				{
					m_pSlashTraceCustomTexture[i]->SetDisableTime( fTexturedSlashTraceDisableTime );
				}
				else
				{
					m_pSlashTraceCustomTexture[i]->SetDisableTime( fSlashTraceDisableTime );
				}

				m_pSlashTraceCustomTexture[i]->SetDrawCount( slashTraceDrawCount );
				m_pSlashTraceCustomTexture[i]->SetLayer( XL_EFFECT_0 );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_pSlashTraceCustomTexture[i] );
			}
#endif // CUSTOM_SLASH_TRACE_TEXTURE
		}	// 무기 slash trace 생성

	} // for(i)

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 아라는 이펙트 적용 위치가 달라서, 따로 지정해 준다
	if( GetOwnerUnit().GetGameUnitType() == CX2GameUnit::GUT_USER &&
        NULL != static_cast<CX2GUUser&>( GetOwnerUnit() ).GetUnit() &&
		NULL != static_cast<CX2GUUser&>( GetOwnerUnit() ).GetUnit()->GetUnitTemplet() &&
		NULL != m_pUserWeaponEquip && 
		NULL != m_pUserWeaponEquip->GetNormalAnim(0) )
	{
		switch ( static_cast<CX2GUUser&>( GetOwnerUnit() ).GetUnit()->GetUnitTemplet()->m_UnitType )
		{
		case CX2Unit::UT_ARA:
			if( NULL != m_pUserWeaponEquip && NULL != m_pUserWeaponEquip->GetNormalAnim(0) )
			{
				m_pFrame_ATTACK_LINE_END0	= m_pUserWeaponEquip->GetNormalAnim(0)->GetCloneFrame( L"ATTACK_LINE_END0" );
				m_pFrame_ATTACK_LINE_START0_FRONT	= m_pUserWeaponEquip->GetNormalAnim(0)->GetCloneFrame( L"ATTACK_LINE_START0_FRONT" );
			} break;
		}
	}
#endif ADD_UPGRADE_WEAPON_PARTICLE

	return true;
}

/*static*/
void	CX2GameUnit::InitWeaponDataForNPC( InitDeviceData& OutDevice_, KLuaManager& luaManager_ )
{
	std::wstring	wstrName;

	if ( luaManager_.GetValue( "WEAPON_FILE_NAME", wstrName ) == true )
		OutDevice_.m_vecstrReadyXSkinMesh.push_back( wstrName );
	if ( luaManager_.GetValue( "WEAPON_XET_NAME", wstrName ) == true )
		OutDevice_.m_vecstrReadyXET.push_back( wstrName );
}

bool CX2GameUnit::Weapon::LoadDataForNPC( KLuaManager& luaManager )
{
//{{ robobeg : 2008-10-28
	//if( NULL == GetOwnerUnit() )
	//	return false;
//}} robobeg : 2008-10-28

	CX2GUNPC* pNPC = (CX2GUNPC*) &GetOwnerUnit();

	if( pNPC->GetXSkinAnimPtr() == NULL )
		return false;


	if( NULL == pNPC->GetMotion() )
		return false;





	//{{ dmlee 2009.1.15 몬스터에 악세사리 형태로 메시를 붙이기 위해 weapon 코드를 사용하면서 임시로 랜덤하게 악세사리를 붙일 수 있게 기능제작, 각 클라이언트사이에 sync가 맞지 않는다.
	int iRate = 0;
	LUA_GET_VALUE( luaManager, "SAFE_RATE", 	iRate,		-1 );		// SAFE_RATE에 의해 무기가 하나도 없을 경우에는 반드시 로딩되고, 무기가 하나라도 있을 경우 확률에 의해 로딩된다.
	if( iRate > 0 )
	{
		if( 0 != pNPC->GetWeaponCount() )
		{
			if( rand() % 101 > iRate )
			{
				return false;
			}
		}
	}
	else 
	{
		LUA_GET_VALUE( luaManager, "RATE",		iRate,		-1 );		// RATE에 의해 무기가 로딩될지 말지 랜덤하게 결정, 
		if( iRate > 0 )
		{
			if( rand() % 101 > iRate )
			{
				return false;
			}
		}
	}
	//}} dmlee 2009.1.15 몬스터에 악세사리 형태로 메시를 붙이기 위해 weapon 코드를 사용하면서 임시로 랜덤하게 악세사리를 붙일 수 있게 기능제작, 각 클라이언트사이에 sync가 맞지 않는다.




	wstring weaponFileName = L"";
	wstring weaponBoneName = L"";
	wstring weaponXETName = L"";

	LUA_GET_VALUE( luaManager, "WEAPON_FILE_NAME", weaponFileName, L"" );
	LUA_GET_VALUE( luaManager, "WEAPON_BONE_NAME", weaponBoneName, L"" );
	LUA_GET_VALUE( luaManager, "WEAPON_XET_NAME", weaponXETName, L"" );


	CKTDXDeviceXSkinMesh* pMotion = NULL;
	if( false == weaponFileName.empty() )
	{
		pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( weaponFileName.c_str() );
		if( NULL == pMotion )
			return false;

        ASSERT( m_pXSkinWeapon == NULL );
		m_pXSkinWeapon = CKTDGXSkinAnim::CreateSkinAnim();

		CKTDXDeviceXET* pXETWeapon = g_pKTDXApp->GetDeviceManager()->OpenXET( weaponXETName );
		m_pXSkinWeapon->SetAnimXSkinMesh( pMotion, pXETWeapon );
		m_pXSkinWeapon->AddModelXSkinMesh( pMotion, pXETWeapon, NULL, pXETWeapon, true );
		m_pXSkinWeapon->ChangeAnim( 0 );
		m_pXSkinWeapon->Play( CKTDGXSkinAnim::XAP_LOOP );

		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
		pFrame			= pNPC->GetXSkinAnimPtr()->GetCloneFrame( weaponBoneName.c_str() );
		m_pMatrixWeapon = &(pFrame->combineMatrix);
		m_pXSkinWeapon->UseDXMatrix( true ); // ???

		LUA_GET_VALUE( luaManager, "WEAPON_ROTAION_X", m_WeaponRotation.x, 0 );
		LUA_GET_VALUE( luaManager, "WEAPON_ROTAION_Y", m_WeaponRotation.y, 0 );
		LUA_GET_VALUE( luaManager, "WEAPON_ROTAION_Z", m_WeaponRotation.z, 0 );

		m_WeaponRotation.x = D3DXToRadian( m_WeaponRotation.x );
		m_WeaponRotation.y = D3DXToRadian( m_WeaponRotation.y );
		m_WeaponRotation.z = D3DXToRadian( m_WeaponRotation.z );

		LUA_GET_VALUE( luaManager, "WEAPON_SCALE_X", m_WeaponScale.x, 1.0 );
		LUA_GET_VALUE( luaManager, "WEAPON_SCALE_Y", m_WeaponScale.y, 1.0 );
		LUA_GET_VALUE( luaManager, "WEAPON_SCALE_Z", m_WeaponScale.z, 1.0 );

		//{{ JHKang / 2011.03.23 / Blend 값 지정
#ifdef ADD_WEAPON_BLEND_VALUE
		LUA_GET_VALUE_ENUM( luaManager, "WEAPON_SRC_BLEND", m_d3dSrcBlend, D3DBLEND, D3DBLEND_SRCALPHA );
		LUA_GET_VALUE_ENUM( luaManager, "WEAPON_DEST_BLEND", m_d3dDestBlend, D3DBLEND, D3DBLEND_DESTALPHA );
#endif
		//}}

//{{ robobeg : 2008-10-28
		//LUA_GET_VALUE( luaManager, "ALPHA_OBJECT", m_bAlphaObject, false );
        bool bAlpha = false;
        LUA_GET_VALUE( luaManager, "ALPHA_OBJECT", bAlpha, false );
        SetAlphaObject( bAlpha );
//}} robobeg : 2008-10-28

		//{{ kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07
		bool bZEnable	= false;
		LUA_GET_VALUE( luaManager, "Z_ENABLE_FOR_THIS_WEAPON", bZEnable, false );
		SetZEnable( bZEnable );
		//}} kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07


		// fix!! 일단 npc는 무기 하나에 메시 하나만 되도록 하드코딩
		if( NULL == m_pAfterImage[0] )
		{
			m_pAfterImage[0] = CKTDGXSkinAfterImage::CreateSkinAfterImage( m_pXSkinWeapon, 10, XL_EFFECT_0 );
			if( m_pAfterImage[0] != NULL )
			{
				m_pAfterImage[0]->SetAfterImageColor( 0xffcccccc );
				m_pAfterImage[0]->AddMesh( pMotion );
			}
		}

	}


	LUA_GET_VALUE( luaManager, "USE_SLASH_TRACE",		m_bUseSlashTrace,		false );
	if( m_bUseSlashTrace == true )
	{
		float fSlashTraceDisableTime;
		float fSlashTraceTipDisableTime;
		LUA_GET_VALUE( luaManager, "SLASH_TRACE_DISABLE_TIME",			fSlashTraceDisableTime,		0.4f );
		LUA_GET_VALUE( luaManager, "SLASH_TRACE_TIP_DISABLE_TIME",		fSlashTraceTipDisableTime,	0.5f );
		int slashTraceDrawCount;
		int slashTraceTipDrawCount;
		LUA_GET_VALUE( luaManager, "SLASH_TRACE_DRAW_COUNT",			slashTraceDrawCount,		1 );
		LUA_GET_VALUE( luaManager, "SLASH_TRACE_TIP_DRAW_COUNT",		slashTraceTipDrawCount,		3 );



		wstring slashTraceTopBone = L"";
		wstring slashTraceBottomBone = L"";

		LUA_GET_VALUE( luaManager, "SLASH_TRACE_TOP_BONE",				slashTraceTopBone,		L"" );
		LUA_GET_VALUE( luaManager, "SLASH_TRACE_BOTTOM_BONE",			slashTraceBottomBone,		L"" );


		// fix!! 일단 npc는 무기 하나에 메시 하나만 되도록 하드코딩
		m_pFrame_TRACE_START[0]		= m_pXSkinWeapon->GetCloneFrame( slashTraceTopBone.c_str() );
		m_pFrame_TRACE_END[0]		= m_pXSkinWeapon->GetCloneFrame( slashTraceBottomBone.c_str() );


		if( NULL == m_pSlashTrace[0] )
		{
			m_pSlashTrace[0] = CKTDGSlashTrace::CreateSlashTrace( 100 );
			m_pSlashTrace[0]->SetCenter( pNPC->GetMotion()->GetCenter() );
			m_pSlashTrace[0]->SetBoundingRadius( pNPC->GetMotion()->GetBoundingRadius() * 1.5f );
			m_pSlashTrace[0]->SetDisableTime( fSlashTraceDisableTime );
			m_pSlashTrace[0]->SetDrawCount( slashTraceDrawCount );
			m_pSlashTrace[0]->SetLayer( XL_EFFECT_0 );
			g_pKTDXApp->GetDGManager()->AddObjectChain( m_pSlashTrace[0] );
		}


		if( NULL == m_pSlashTraceTip[0] )
		{
			m_pSlashTraceTip[0] = CKTDGSlashTrace::CreateSlashTrace( 100 );
			m_pSlashTraceTip[0]->SetCenter( pNPC->GetMotion()->GetCenter() );
			m_pSlashTraceTip[0]->SetBoundingRadius( pNPC->GetMotion()->GetBoundingRadius() * 1.5f );
			m_pSlashTraceTip[0]->SetDisableTime( fSlashTraceTipDisableTime );
			m_pSlashTraceTip[0]->SetDrawCount( slashTraceTipDrawCount );
			m_pSlashTraceTip[0]->SetLayer( XL_EFFECT_0 );
			g_pKTDXApp->GetDGManager()->AddObjectChain( m_pSlashTraceTip[0] );
		}
	}

	GetOwnerUnit().AddWeaponAttackCollisionBoxForNPC( this ); // fix!!! 음...

	return true;
}

void CX2GameUnit::Weapon::SetEnable( bool bEnable )
{
	m_bEnable = bEnable;
	SetShowObject( bEnable );
// 	for(UINT i=0; i<m_vecAttackBoxName.size(); ++i)
// 	{
// //{{ robobeg : 2008-10-28
// 		//if(GetOwnerUnit() != NULL)
// //}} robobeg : 2008-10-28
// 			GetOwnerUnit().SetEnableAttackBox( m_vecAttackBoxName[i].c_str(), bEnable );
// 	}
}

HRESULT CX2GameUnit::Weapon::OnFrameMove( double fTime, float fElapsedTime )
{
//{{ robobeg : 2008-10-28
	//if( NULL == GetOwnerUnit() )
	//	return;
//}} robobeg : 2008-10-28



	D3DXVECTOR3 vRotateDegree = GetOwnerUnit().GetRotateDegree();

	// weapon particle
	for( UINT i=0; i< m_vecpWeaponParticle.size(); ++i )
	{
		WeaponParticle* pWeaponParticle = m_vecpWeaponParticle[i];
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		if( true == pWeaponParticle->m_bIsMajorParticle )
		{
			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pWeaponParticle->m_hSeq );
		}
		else
		{
			pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( pWeaponParticle->m_hSeq );
		}

		if( NULL == pSeq )
		{
			m_vecpWeaponParticle.erase( m_vecpWeaponParticle.begin() + i );
			--i;
			continue;
		}

		// fix!! 무기의 몇번 메시에 붙을지 지정할 수 있게 바꿔야함
		D3DXVECTOR3 vWeaponBonePos = GetBonePos( pWeaponParticle->m_wstrBoneName.c_str(), 0 );
		pSeq->SetPosition( vWeaponBonePos );
		pSeq->SetAddRotate( vRotateDegree );
		pSeq->SetAxisAngle( vRotateDegree );
	}
	


	// fix!! gunpc만 요렇게 처리, 
	// fix!! guuser는 eqip쪽에서 처리하거나 다르게 처리하는 부분이 필요
	if( NULL != m_pXSkinWeapon )
	{
		// weapon 
		D3DXMATRIX matrixWeapon = *m_pMatrixWeapon;
		D3DXMATRIX matScale;
		D3DXMATRIX matRot;

		D3DXMatrixScaling( &matScale, m_WeaponScale.x, m_WeaponScale.y, m_WeaponScale.z );
		D3DXMatrixRotationYawPitchRoll( &matRot, m_WeaponRotation.y, m_WeaponRotation.x, m_WeaponRotation.z );

		matrixWeapon = matScale * matRot * matrixWeapon;
		
		CKTDGXRenderer::RenderParam* pWeaponRenderParam = m_pXSkinWeapon->GetRenderParam();
		*pWeaponRenderParam = GetOwnerUnit().GetUnitRenderParam();

//{{ robobeg : 2008-10-27
		//pWeaponRenderParam->worldMatrix = matrixWeapon; // ??
//}} robobeg : 2008-10-27
		m_pXSkinWeapon->SetDXMatrix( matrixWeapon );

		m_pXSkinWeapon->OnFrameMove( fTime, fElapsedTime );
	}

#ifdef CHUNG_SECOND_CLASS_CHANGE
	GetOwnerUnit().FrameMoveWeaponRenderEffect();
#endif


#ifdef CHUNG_SECOND_CLASS_CHANGE
	if( true == GetOwnerUnit().GetEnableWeaponRenderEffect())
	{
		for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
		{
#ifdef SECRET_VELDER
			if ( GUT_USER == GetOwnerUnit().GetGameUnitType() )
			{
				// 무기 after image 생성
				if( NULL != m_pUserWeaponEquip && NULL != m_pUserWeaponEquip->GetNormalAnim(i) )
				{
					CKTDGXRenderer::RenderParam* pRenderParam = m_pUserWeaponEquip->GetNormalAnim(i)->GetRenderParam();
					ASSERT( pRenderParam != NULL );
					*pRenderParam = GetOwnerUnit().GetUnitRenderParam();
					pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_FADE;
					pRenderParam->color =  GetOwnerUnit().GetWeaponRenderEffectColor();
				}
			}

#else
			// 무기 after image 생성
			if( NULL != m_pUserWeaponEquip->GetNormalAnim(i) )
			{
				CKTDGXRenderer::RenderParam* pRenderParam = m_pUserWeaponEquip->GetNormalAnim(i)->GetRenderParam();
				ASSERT( pRenderParam != NULL );
				*pRenderParam = GetOwnerUnit().GetUnitRenderParam();
				pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_FADE;
				pRenderParam->color =  GetOwnerUnit().GetWeaponRenderEffectColor();
			}
#endif
		}
	}
#endif CHUNG_SECOND_CLASS_CHANGE

    return S_OK;
}


//{{ robobeg : 2008-10-28
//void CX2GameUnit::Weapon::SetShow( bool bShow )
/*virtual*/
void CX2GameUnit::Weapon::NotifyShowObjectChanged()
{
    bool    bShow = GetShowObject();

	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		if( NULL != m_pSlashTrace[i] )
		{
			m_pSlashTrace[i]->SetShowObject( bShow );
		}

		if( NULL != m_pSlashTraceTip[i] )
		{
			m_pSlashTraceTip[i]->SetShowObject( bShow );
		}

#ifdef ARA_CHARACTER_BASE
		if( NULL != m_pSlashTraceTip2[i] )
		{
			m_pSlashTraceTip2[i]->SetShowObject( bShow );
		}
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
		if( NULL != m_pSlashTraceCustomTexture[i] )
		{
			m_pSlashTraceCustomTexture[i]->SetShowObject( bShow );
		}
#endif // CUSTOM_SLASH_TRACE_TEXTURE

		if( NULL != m_pAfterImage[i] )
		{                    
			if( true == bShow )
			{
				m_pAfterImage[i]->Enable();                        
			}
			else
			{
				m_pAfterImage[i]->Disable();
			}
		}  
	}

	if( m_pXSkinWeapon != NULL )
	{
		//{{ robobeg : 2008-10-20
		//m_pXSkinWeapon->SetShow(bShow);  
		m_pXSkinWeapon->SetShowObject(bShow); 
		//}} robobeg : 2008-10-20
	}

	if( m_pUserWeaponEquip != NULL )
	{
		m_pUserWeaponEquip->SetShowObject( bShow );
	}

	for( UINT i=0; i<m_vecpWeaponParticle.size(); ++i )
	{
		WeaponParticle* pWeaponParticle = m_vecpWeaponParticle[i];
		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
		if( true == pWeaponParticle->m_bIsMajorParticle )
		{
			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pWeaponParticle->m_hSeq );
		}
		else
		{
			pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( pWeaponParticle->m_hSeq );
		}

		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}

//{{ kimhc // 2010.6.16 // 같은 기능을 하는 함수가 존재하여 함수를 사용함
#ifdef	ENCHANT_BUG_TEST
	SetEnchantParticleShow( bShow );
#else	ENCHANT_BUG_TEST
	//kimhc //2010.06.16
	//{{AFX
		CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[0] );
		CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant2 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[1] );
		CKTDGParticleSystem::CParticleEventSequence* pSeqEnchant3 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_ahEnchantWeapon[2] );

		if( NULL != pSeqEnchant1 )
		{
			pSeqEnchant1->SetShowObject( bShow );
		}
		if( NULL != pSeqEnchant2 )
		{
			pSeqEnchant2->SetShowObject( bShow );
		}
		if( NULL != pSeqEnchant3 )
		{
			pSeqEnchant3->SetShowObject( bShow );
		}
	//}}AFX
#endif	ENCHANT_BUG_TEST
//}} kimhc // 2010.6.16 // 같은 기능을 하는 함수가 존재하여 함수를 사용함
	              
}//void CX2GameUnit::Weapon::SetShow( bool bShow )

//}} robobeg : 2008-10-28


//{{ robobeg : 2008-10-28
/*virtual*/ 
RENDER_HINT CX2GameUnit::Weapon::OnFrameRender_Prepare()
{
#ifdef NOT_RENDER_NPC_GAME_EDIT
	if ( NULL != g_pX2Game && false == g_pX2Game->GetShowNpcByGameEdit() )
		return RENDER_HINT_NORENDER;
#endif // NOT_RENDER_NPC_GAME_EDIT

    if ( m_pXSkinWeapon != NULL  )
    {
        m_pXSkinWeapon->SetInstantShow( false );
        if ( !m_pXSkinWeapon->GetShowObject() )
            return RENDER_HINT_NORENDER;

	    CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinWeapon->GetRenderParam();
        ASSERT( pRenderParam != NULL );
	    *pRenderParam = GetOwnerUnit().GetUnitRenderParam();

	    if( GetOwnerUnit().GetRemainHyperModeTime() > 0.0f )
	    {
		    pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
		    pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_RED;
		    pRenderParam->outLineColor		= 0xffff5511;
	    }

	    // todo!! 일단 이렇게 작업하고 나중에 다른 종류 무기 들어갈때 render param을 모두 다 입력 받는 방식으로 바꾸자
    //{{ robobeg : 2008-10-28
	    //if( true == m_bAlphaObject ) 
        if ( IsAlphaObject() )
    //}} robobeg : 2008-10-28
	    {
		    pRenderParam->bAlphaBlend = true;
		    pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR; 

			//{{ kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07
			pRenderParam->bZEnable		= GetZEnable();
			// pRenderParam->bZEnable = false;		// 기존의 코드
			//}} kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07

			pRenderParam->cullMode = D3DCULL_NONE;
			//{{ JHKang / 2011.03.23 / Blend 값 지정
#ifdef ADD_WEAPON_BLEND_VALUE
			pRenderParam->srcBlend = m_d3dSrcBlend;
			pRenderParam->destBlend = m_d3dDestBlend;
#else
		    pRenderParam->srcBlend = D3DBLEND_SRCALPHA;
		    pRenderParam->destBlend = D3DBLEND_DESTALPHA;
#endif
			//}}
	    }

#ifdef SECRET_VELDER
		if( true == GetOwnerUnit().GetEnableWeaponRenderEffect())
		{
			pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_FADE;
			pRenderParam->color =  GetOwnerUnit().GetWeaponRenderEffectColor();
		}
#endif

        RENDER_HINT renderHint = m_pXSkinWeapon->OnFrameRender_Prepare();
        if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
            m_pXSkinWeapon->SetInstantShow( true );
        return renderHint;
    }//if

    return RENDER_HINT_NORENDER;
}//CX2GameUnit::Weapon::OnFrameRender_Prepare()
//}} robobeg : 2008-10-28


//{{ robobeg : 2008-10-28
//void CX2GameUnit::Weapon::OnFrameRenderForNPC()
/*virtual*/ 
void CX2GameUnit::Weapon::OnFrameRender_Draw()
//}} robobeg : 2008-10-28
{
	if ( NULL != g_pX2Game && 
		 g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD && 
		 false == GetOwnerUnit().DidReceiveFirstSyncPacket() )
		return;

    if ( m_pXSkinWeapon != NULL && m_pXSkinWeapon->GetShowObject() && m_pXSkinWeapon->IsInstantShow() )
	{
		m_pXSkinWeapon->OnFrameRender_Draw();
	}
}

void CX2GameUnit::Weapon::CommonStateEnd()
{
	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		if( NULL != m_pAfterImage[i] )
		{                    
			m_pAfterImage[i]->Disable();
		}  
	}
}

void CX2GameUnit::Weapon::CommonFrameMoveForUser( double fTime, float fElapsedTime )
{
//{{ robobeg : 2008-10-28
	//if( NULL == GetOwnerUnit() )
	//	return;
//}} robobeg : 2008-10-28

	CX2GUUser* pUser = (CX2GUUser*) &GetOwnerUnit();

	if( NULL == pUser->GetXSkinAnim() )
		return;


	// 무기 after image 처리
	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; ++i )
	{
		const CX2GUUser::FrameData& kFrameDataNow = pUser->GetNowFrameData();

		if( NULL != m_pAfterImage[i] )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const D3DXVECTOR2&  v2AfterImage = pUser->GetNowAfterImageTime();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            const D3DXVECTOR2&  v2AfterImage = kFrameDataNow.stateParam.afterImage;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( v2AfterImage.x != -1.0f
				&& v2AfterImage.y != -1.0f )
			{
				if( v2AfterImage.x <= pUser->GetXSkinAnim()->GetNowAnimationTime()
					&& v2AfterImage.y > pUser->GetXSkinAnim()->GetNowAnimationTime() )
				{
					if( false == pUser->GetAbsoluteInvisibility() )
					{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
						if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
						if( g_pMain->GetGameOption().GetOptionList().m_bEffect == true )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
						{
							m_pAfterImage[i]->Enable();
						}
					}
				}
				else
				{
					m_pAfterImage[i]->Disable();
				}
			}

			m_pAfterImage[i]->OnFrameMove( fTime, fElapsedTime );
		}


		// 무기 slash trace 처리
		if( m_bUseSlashTrace == true )
		{
			if( NULL != m_pSlashTrace[i] )
			{
				m_pSlashTrace[i]->OnFrameMove( fElapsedTime );
				m_pSlashTrace[i]->GetMatrix().Move( GetOwnerUnit().GetMatrix().GetPos() );
			}

			if( NULL != m_pSlashTraceTip[i] )
			{
				m_pSlashTraceTip[i]->OnFrameMove( fElapsedTime );
				m_pSlashTraceTip[i]->GetMatrix().Move( GetOwnerUnit().GetMatrix().GetPos() );
			}

#ifdef ARA_CHARACTER_BASE
			if( NULL != m_pSlashTraceTip2[i] )
			{
				m_pSlashTraceTip2[i]->OnFrameMove( fElapsedTime );
				m_pSlashTraceTip2[i]->GetMatrix().Move( GetOwnerUnit().GetMatrix().GetPos() );
			}
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
			if( NULL != m_pSlashTraceCustomTexture[i] )
			{
				m_pSlashTraceCustomTexture[i]->OnFrameMove( fElapsedTime );
				m_pSlashTraceCustomTexture[i]->GetMatrix().Move( GetOwnerUnit().GetMatrix().GetPos() );
			}
#endif // CUSTOM_SLASH_TRACE_TEXTURE

			if( NULL != m_pFrame_TRACE_START[i] &&
				NULL != m_pFrame_TRACE_END[i] )
			{
				D3DXVECTOR3 up;
				GetFramePos( &up, m_pFrame_TRACE_START[i] );
				D3DXVECTOR3 down;
				GetFramePos( &down, m_pFrame_TRACE_END[i] );

				//if( up != D3DXVECTOR3(0.f,0.f,0.f) && down != D3DXVECTOR3(0.f,0.f,0.f) )
				{
					D3DXVECTOR3 dirTip	= down - up;
					D3DXVec3Normalize( &dirTip, &dirTip );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    D3DXVECTOR3 tipDown = up + dirTip * pUser->GetNowSlashTraceTipWide();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
					D3DXVECTOR3 tipDown = up + dirTip * kFrameDataNow.stateParam.fSlashTraceTipWide;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


					D3DXVECTOR3 vSlashTraceTime( -1.f, -1.f, 0.f );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    pUser->FindNowSlashTraceTimeWithWeaponIndex( m_iWeaponIndex, OUT vSlashTraceTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    std::map< int, D3DXVECTOR3 >::const_iterator it;
					it = kFrameDataNow.stateParam.m_mapAddSlashTrace.find( m_iWeaponIndex );
					if( it != kFrameDataNow.stateParam.m_mapAddSlashTrace.end() )
					{
						vSlashTraceTime = it->second;
					}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

					if( vSlashTraceTime.x <= pUser->GetXSkinAnim()->GetNowAnimationTime()
						&& vSlashTraceTime.y > pUser->GetXSkinAnim()->GetNowAnimationTime() )
					{						
						AddSlashTraceByType( (int) vSlashTraceTime.z, up, down, tipDown, i );
					}
					else
					{
						AddSlashTraceByType( (int) CKTDGSlashTrace::STT_NONE, up, down, tipDown, i );
					}
				}
			}
		}
	} // for(i)


	switch( pUser->GetUnit()->GetType() )
	{
	case CX2Unit::UT_ELSWORD:
	case CX2Unit::UT_ARME:
	case CX2Unit::UT_RAVEN:
		//{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG
	case CX2Unit::UT_ELESIS:
		{
			EnchantParticleFrameMoveForSword();
		} break;

	case CX2Unit::UT_LIRE:
		{
			EnchantParticleFrameMoveForBow();
		} break;

	case CX2Unit::UT_EVE:
		{
			EnchantParticleFrameMoveForEve();
		} break;
#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		{
			EnchantParticleFrameMoveForAra();
		} break;
#endif
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	case CX2Unit::UT_ADD:
		{
			EnchantParticleFrameMoveForAdd();
		} break;
#endif //SERV_9TH_NEW_CHARACTER
	}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 강화 무기 이펙트 처리
	ProcessUpgradeWeaponParticle();
#endif ADD_UPGRADE_WEAPON_PARTICLE
}


void CX2GameUnit::Weapon::CommonFrameMoveForNPC( double fTime, float fElapsedTime )
{

//{{ robobeg : 2008-10-28
	//if( NULL == GetOwnerUnit() )
	//	return;
//}} robobeg : 2008-10-28

	CX2GUNPC* pNPC = (CX2GUNPC*) &GetOwnerUnit();

	if( pNPC->GetXSkinAnimPtr() == NULL )
		return;



	// fix!! npc무기 하나당 메시 하나로만 처리되도록 하드코딩
	const CX2GUNPC::NPCFrameData& frameData = pNPC->GetNPCFrameData();
	if( NULL != m_pAfterImage[0] )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        const D3DXVECTOR2&  v2AfterImage = pNPC->GetAfterImageTime();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        const D3DXVECTOR2&  v2AfterImage = frameData.stateParam.afterImage;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( v2AfterImage.x != -1.0f
			&& v2AfterImage.y != -1.0f )
		{
			if( v2AfterImage.x <= pNPC->GetXSkinAnimPtr()->GetNowAnimationTime()
				&& v2AfterImage.y > pNPC->GetXSkinAnimPtr()->GetNowAnimationTime() )
			{
				if( false == pNPC->GetAbsoluteInvisibility() )
				{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
					if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
					if( g_pMain->GetGameOption().GetOptionList().m_bEffect == true )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
					{
						m_pAfterImage[0]->Enable();
					}
				}
			}
			else
			{
				m_pAfterImage[0]->Disable();
			}
		}

		m_pAfterImage[0]->OnFrameMove( fTime, fElapsedTime );
	}


	if( m_bUseSlashTrace == true )
	{

		if( NULL != m_pSlashTrace[0] )
		{
			m_pSlashTrace[0]->OnFrameMove( fElapsedTime );
			m_pSlashTrace[0]->GetMatrix().Move( GetOwnerUnit().GetMatrix().GetPos() );
		}

		if( NULL != m_pSlashTraceTip[0] )
		{
			m_pSlashTraceTip[0]->OnFrameMove( fElapsedTime );
			m_pSlashTraceTip[0]->GetMatrix().Move( GetOwnerUnit().GetMatrix().GetPos() );
		}

		if( NULL != m_pFrame_TRACE_START[0] &&
			NULL != m_pFrame_TRACE_END[0] )
		{
			D3DXVECTOR3 up;
			D3DXVECTOR3 down;

			GetFramePos( &up, m_pFrame_TRACE_START[0] );
			GetFramePos( &down, m_pFrame_TRACE_END[0] );

			//if( up != D3DXVECTOR3(0.f,0.f,0.f) && down != D3DXVECTOR3(0.f,0.f,0.f) )
			{
				D3DXVECTOR3 dirTip = down - up;
				D3DXVec3Normalize( &dirTip, &dirTip );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                D3DXVECTOR3 tipDown = up + dirTip * pNPC->GetSlashTraceTipWide();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				D3DXVECTOR3 tipDown = up + dirTip * frameData.stateParam.fSlashTraceTipWide;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

				D3DXVECTOR3 vSlashTraceTime( -1.f, -1.f, 0.f );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                pNPC->FindAddSlashTraceWithWeaponIndex( m_iWeaponIndex, OUT vSlashTraceTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				std::map< int, D3DXVECTOR3 >::const_iterator it;
				it = frameData.stateParam.m_mapAddSlashTrace.find( m_iWeaponIndex );
				if( it != frameData.stateParam.m_mapAddSlashTrace.end() )
				{
					vSlashTraceTime = it->second;
				}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

				if( vSlashTraceTime.x <= pNPC->GetXSkinAnimPtr()->GetNowAnimationTime()
					&& vSlashTraceTime.y > pNPC->GetXSkinAnimPtr()->GetNowAnimationTime() )
				{
					AddSlashTraceByType( (int) vSlashTraceTime.z, up, down, tipDown, 0 );
				}
				else
				{
					AddSlashTraceByType( (int) CKTDGSlashTrace::STT_NONE, up, down, tipDown, 0 );
				}
			}
		}
	}
}

void CX2GameUnit::Weapon::AddSlashTraceByType( int iType, D3DXVECTOR3 up, D3DXVECTOR3 down, D3DXVECTOR3 tipDown, int iModelIndex )
{
	if( false == GetShowObject() )
		return; 

	if( true == GetOwnerUnit().GetAbsoluteInvisibility() )
		return; 

	
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
	// 기본 트레이스에 텍스쳐가 지정되었다면 커스텀 텍스쳐 사용하지 않기.
	// 프로즌, 블레이즈 등 속성 트레이스를 우선시 하기 위한 작업
	if( CKTDGSlashTrace::STT_CUSTOM_TEXTURE == static_cast<CKTDGSlashTrace::SLASH_TRACE_TYPE>(iType) &&
		true == m_pSlashTrace[iModelIndex]->GetEnabledSlashTraceTexture() )
	{
		iType = CKTDGSlashTrace::STT_DEFAULT;
	}
#endif // CUSTOM_SLASH_TRACE_TEXTURE

	switch( (CKTDGSlashTrace::SLASH_TRACE_TYPE) iType )
	{
	default:
	case CKTDGSlashTrace::STT_DEFAULT:
		{
			D3DXCOLOR vTraceColor;
			D3DXCOLOR vTraceTipColor;

			if( m_bTraceColor == true )
			{
				vTraceColor = m_vTraceColor;
				vTraceTipColor = m_vTraceTipColor;
			}
			else if( GetOwnerUnit().GetRemainHyperModeTime() > 0.f )
			{
				vTraceColor = m_coSlashTraceHyper;
				vTraceTipColor = m_coSlashTraceTipHyper;
			}
			else
			{
				vTraceColor = m_coSlashTrace;
				vTraceTipColor = m_coSlashTraceTip;
			}


			if( NULL != m_pSlashTrace[iModelIndex] )
			{
				m_pSlashTrace[iModelIndex]->AddSlashData( up, down, vTraceColor );
			}

			if( NULL != m_pSlashTraceTip[iModelIndex] )
			{
				m_pSlashTraceTip[iModelIndex]->AddSlashData( up, tipDown, vTraceTipColor );
			}
			
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
			if( NULL != m_pSlashTraceCustomTexture[iModelIndex] )
			{
				m_pSlashTraceCustomTexture[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
			}
#endif // CUSTOM_SLASH_TRACE_TEXTURE

		} break;



	case CKTDGSlashTrace::STT_NONE:
		{
			// slash trace가 안보이는 동안에도 지속적으로 slash trace 데이터를 업데이트 해줘야 한다
			if( NULL != m_pSlashTrace[iModelIndex] )
			{
				m_pSlashTrace[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
			}

			if( NULL != m_pSlashTraceTip[iModelIndex] )
			{
				m_pSlashTraceTip[iModelIndex]->AddSlashData( up, tipDown, D3DXCOLOR( 0, 0, 0, 0 ) );
			}

#ifdef CUSTOM_SLASH_TRACE_TEXTURE
			if( NULL != m_pSlashTraceCustomTexture[iModelIndex] )
			{
				m_pSlashTraceCustomTexture[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
			}
#endif // CUSTOM_SLASH_TRACE_TEXTURE
		} break;
		
	case CKTDGSlashTrace::STT_CONSTANT_WIDTH:
		{
			if( NULL != m_pSlashTrace[iModelIndex] )
			{
				D3DXVECTOR3 vLineDir = GetOwnerUnit().GetDirVector();
				D3DXVec3Normalize( &vLineDir, &vLineDir );

				if( GetOwnerUnit().GetRemainHyperModeTime() > 0.f )
				{
					m_pSlashTrace[iModelIndex]->AddSlashDataConstantWidth( up, down, m_coSlashTraceHyper, vLineDir );
				}
				else
				{
					m_pSlashTrace[iModelIndex]->AddSlashDataConstantWidth( up, down, m_coSlashTrace, vLineDir );
				}

#ifdef CUSTOM_SLASH_TRACE_TEXTURE
				if( NULL != m_pSlashTraceCustomTexture[iModelIndex] )
				{
					m_pSlashTraceCustomTexture[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
				}
#endif // CUSTOM_SLASH_TRACE_TEXTURE

			}

		} break;

	case CKTDGSlashTrace::STT_CIRCULAR:
		{
			if( NULL != m_pSlashTrace[iModelIndex] )
			{
				D3DXVECTOR3 vBonePos = GetOwnerUnit().GetBonePos( L"Bip01_Spine" );
				D3DXVECTOR3 vLineDir = GetOwnerUnit().GetDirVector();
				D3DXVec3Normalize( &vLineDir, &vLineDir );

				if( GetOwnerUnit().GetRemainHyperModeTime() > 0.f )
				{
					m_pSlashTrace[iModelIndex]->AddSlashDataCircular( vBonePos, up, down, m_coSlashTraceHyper, vLineDir, 50.f, false );
				}
				else
				{
					m_pSlashTrace[iModelIndex]->AddSlashDataCircular( vBonePos, up, down, m_coSlashTrace, vLineDir, 50.f, false );
				}

#ifdef CUSTOM_SLASH_TRACE_TEXTURE
				if( NULL != m_pSlashTraceCustomTexture[iModelIndex] )
				{
					m_pSlashTraceCustomTexture[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
				}
#endif // CUSTOM_SLASH_TRACE_TEXTURE
			}

		} break;
#ifdef ARA_CHARACTER_BASE
	case CKTDGSlashTrace::STT_DOUBLE_TIP:
		{
			D3DXCOLOR vTraceColor;
			D3DXCOLOR vTraceTipColor;

			if( m_bTraceColor == true )
			{
				vTraceColor = m_vTraceColor;
				vTraceTipColor = m_vTraceTipColor;
			}
			else if( GetOwnerUnit().GetRemainHyperModeTime() > 0.f )
			{
				vTraceColor = m_coSlashTraceHyper;
				vTraceTipColor = m_coSlashTraceTipHyper;
			}
			else
			{
				vTraceColor = m_coSlashTrace;
				vTraceTipColor = m_coSlashTraceTip;
			}

			if( NULL != m_pSlashTrace[iModelIndex] )
			{
				D3DXVECTOR3 dir	= down - up;
				D3DXVec3Normalize( &dir, &dir );
				m_pSlashTrace[iModelIndex]->AddSlashData( up, up + dir * 160.f, vTraceColor );
			}

			if( NULL != m_pSlashTraceTip[iModelIndex] )
			{
				m_pSlashTraceTip[iModelIndex]->AddSlashData( up, tipDown, vTraceTipColor );
			}

			if( NULL != m_pSlashTraceTip2[iModelIndex] )
			{
				D3DXVECTOR3 dir	= up - down;
				D3DXVec3Normalize( &dir, &dir );
				m_pSlashTraceTip2[iModelIndex]->AddSlashData( down, down + dir * 40.f, vTraceColor );
			}

#ifdef CUSTOM_SLASH_TRACE_TEXTURE
			if( NULL != m_pSlashTraceCustomTexture[iModelIndex] )
			{
				m_pSlashTraceCustomTexture[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
			}
#endif // CUSTOM_SLASH_TRACE_TEXTURE

		} break;
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
	case CKTDGSlashTrace::STT_CUSTOM_TEXTURE:
		{
			ASSERT( NULL != m_pSlashTraceCustomTexture );

			// 기본 트레이스 숨기기
			if( NULL != m_pSlashTrace[iModelIndex] )
			{
				m_pSlashTrace[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
			}
			if( NULL != m_pSlashTraceTip[iModelIndex] )
			{
				m_pSlashTraceTip[iModelIndex]->AddSlashData( up, tipDown, D3DXCOLOR( 0, 0, 0, 0 ) );
			}

			if( NULL != m_pSlashTraceCustomTexture[iModelIndex] )
				m_pSlashTraceCustomTexture[iModelIndex]->AddSlashData( up, down, D3DXCOLOR( 1, 1, 1,1 ) );
		} break;
#endif // CUSTOM_SLASH_TRACE_TEXTURE
	} // switch
}

D3DXVECTOR3 CX2GameUnit::Weapon::GetBonePos( const WCHAR* pBoneName, int iModelIndex )
{
	if( NULL == pBoneName )
		return D3DXVECTOR3( 0, 0, 0 );


	if( NULL != m_pXSkinWeapon )
	{
		return m_pXSkinWeapon->GetCloneFramePosition( pBoneName );
	}
	else if( m_pUserWeaponEquip != NULL )
	{
		// warning!! hyper모드 상태인 경우, hyper모드가 가능한 eqip인 경우, GetHyperAnim()을 호출해야함 
		if( NULL != m_pUserWeaponEquip->GetNormalAnim( iModelIndex ) )
		{
			return m_pUserWeaponEquip->GetNormalAnim( iModelIndex )->GetCloneFramePosition( pBoneName );
		}
		else
		{
			return D3DXVECTOR3( 0, 0, 0 );
		}
	}
	else
	{
		return D3DXVECTOR3( 0, 0, 0 );
	}
}

//{{ kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)
/** @function : GetBoneMatrix
	@brief : 무기의 메시에서 해당하는 boneName의 Matrix 포인터를 return
	@param : const WCHAR* pBoneName, int iModelIndex
	@return : const D3DXMATRIX* (해당하는 Bone의 Matrix포인터), 실패시 NULL
*/
const D3DXMATRIX* CX2GameUnit::Weapon::GetCombineMatrix( const WCHAR* pBoneName, int iModelIndex ) const
{
	if ( NULL == pBoneName )
	{
		return NULL;
	} // if

	if ( NULL != m_pXSkinWeapon )
	{
		return &(m_pXSkinWeapon->GetCloneFrame( pBoneName )->combineMatrix);
	} // if
	else if ( m_pUserWeaponEquip != NULL )
	{
		// warning!! hyper모드 상태인 경우, hyper모드가 가능한 eqip인 경우, GetHyperAnim()을 호출해야함 
		CKTDGXSkinAnim* pSkinAnim = m_pUserWeaponEquip->GetNormalAnim( iModelIndex );
		if( NULL != pSkinAnim )
		{
			return &(pSkinAnim->GetCloneFrame( pBoneName )->combineMatrix);
		}
		else
		{
			return NULL;
		}
	} // else if
	else
	{
		return NULL;
	}
}
//}} kimhc // 2011.1.17 // 지정한 Bone의 메트릭스 값을 Trace 함 (chung 코드 참고)

//{{ kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07
bool CX2GameUnit::Weapon::GetZEnable() const
{
	return m_bZEnable;
}

void CX2GameUnit::Weapon::SetZEnable( bool bEnable )
{
	m_bZEnable	=	bEnable;
}
//}} kimhc // NPC의 웨폰에 Z_ENABLE 적용 // 2009-07-07


#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

CX2GameUnit::ParticleEffectData::~ParticleEffectData()
{
	if( m_hSeq != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		if( m_bMajor == true )
		{
			g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeq );
		}
		else
		{
			g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeq );
		}
	}
}

CX2GameUnit::MeshPlayerEffectData::~MeshPlayerEffectData()
{
	if( m_hMeshInst != INVALID_MESH_INSTANCE_HANDLE )
	{
		if( m_bMajor == true )
		{
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInst );
		}
		else
		{
			g_pX2Game->GetMinorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInst );
		}
	}
}




#ifdef BOOST_SINGLETON_POOL_TEST
	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CX2GameUnit::ParticleEffectData)> CX2GameUnit_ParticleEffectDataPool;

	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CX2GameUnit::MeshPlayerEffectData)> CX2GameUnit_MeshPlayerEffectDataPool;



	void* CX2GameUnit::ParticleEffectData::operator new(size_t s)
	{
		return CX2GameUnit_ParticleEffectDataPool::malloc();
	}

	void CX2GameUnit::ParticleEffectData::operator delete(void *p)
	{
		CX2GameUnit_ParticleEffectDataPool::free(p);
	}




	void* CX2GameUnit::MeshPlayerEffectData::operator new(size_t s)
	{
		return CX2GameUnit_MeshPlayerEffectDataPool::malloc();
	}

	void CX2GameUnit::MeshPlayerEffectData::operator delete(void *p)
	{
		CX2GameUnit_MeshPlayerEffectDataPool::free(p);
	}
#endif BOOST_SINGLETON_POOL_TEST


#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

//////////////////////////////////////////////////////////////////////////
void CX2GameUnit::ExtraDamagePack::Init()
{
#ifdef CHUNG_SECOND_CLASS_CHANGE
	Init(CX2DamageManager::EDT_POISON);
	Init(CX2DamageManager::EDT_FIRE);
	Init(CX2DamageManager::EDT_CURSE);
	Init(CX2DamageManager::EDT_SLOW);
	Init(CX2DamageManager::EDT_LIGHTNING);
	Init(CX2DamageManager::EDT_REVERSE_LEFT_RIGHT);
	Init(CX2DamageManager::EDT_COLD);
	Init(CX2DamageManager::EDT_Y_PRESSED);

	Init(CX2DamageManager::EDT_FROZEN);
	Init(CX2DamageManager::EDT_LEG_WOUND);
	Init(CX2DamageManager::EDT_WEAPON_BREAK);
	Init(CX2DamageManager::EDT_ARMOR_BREAK);
	Init(CX2DamageManager::EDT_ENTANGLE);
	Init(CX2DamageManager::EDT_DULL);
	Init(CX2DamageManager::EDT_TIME_BOMB);
	Init(CX2DamageManager::EDT_STUN);

	Init(CX2DamageManager::EDT_AGING);

	Init(CX2DamageManager::EDT_ENCHANT_BLAZE);
	Init(CX2DamageManager::EDT_ENCHANT_FROZEN);
	Init(CX2DamageManager::EDT_ENCHANT_POISON);
	Init(CX2DamageManager::EDT_ENCHANT_SHOCK);

	Init(CX2DamageManager::EDT_MAGIC_DEFENCE_DEBUFF);
	Init(CX2DamageManager::EDT_STIGMA_DEBUFF);

	Init(CX2DamageManager::EDT_DRYAD);
	Init(CX2DamageManager::EDT_BLIND);

	Init(CX2DamageManager::EDT_WATER_HOLD);

	Init(CX2DamageManager::EDT_PANIC);
	Init(CX2DamageManager::EDT_PAIN);
	Init(CX2DamageManager::EDT_LEG_SHOT);

#ifdef VELDER_SECRET_DUNGEON
	Init(CX2DamageManager::EDT_SCALE_CRITICAL_BUFF);
	Init(CX2DamageManager::EDT_BERSERKER_BUFF);
	Init(CX2DamageManager::EDT_MANA_BUFF);
	Init(CX2DamageManager::EDT_ATTACK_ALL_TEAM);
#endif

#ifdef UPGRADE_RAVEN
	Init(CX2DamageManager::EDT_RUNJUMP_SPEED);
#endif

#ifdef RAVEN_WEAPON_TAKER
	Init(CX2DamageManager::EDT_SMOKE_BLIND);
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
	Init(CX2DamageManager::EDT_FLASH_BANG);
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	Init(CX2DamageManager::EDT_MARK_OF_COMMANDER);
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_RENA_NIGHT_WATCHER
	Init(CX2DamageManager::EDT_START_OF_DELAYED_FIRING );	/// mauntain : 김태환 [2012.05.21] 레나 2차 전직 나이트 와쳐 - 폭발의 화살
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
	Init(CX2DamageManager::EDT_BLEEDING );				/// mauntain : 김태환 [2012.07.16] 레이븐 2차 전직 베테랑 커맨더 - 치명상
#endif SERV_RAVEN_VETERAN_COMMANDER

#else CHUNG_SECOND_CLASS_CHANGE
	m_Poison.Init();
	m_Fire.Init();
	m_Curse.Init();
	m_Slow.Init();
	m_Lightning.Init();
	m_ReverseLeftRight.Init();
	m_Cold.Init();
	m_Y_Pressed.Init();

#ifdef SKILL_30_TEST
	m_Frozen.Init();
	m_LegWound.Init();
	m_WeaponBreak.Init();
	m_ArmorBreak.Init();
	m_Entangled.Init();
	m_Dull.Init();
	m_TimeBomb.Init();
	m_Stun.Init();
#endif SKILL_30_TEST

	m_Aging.Init();

	m_EnchantBlaze.Init();
	m_EnchantFrozen.Init();
	m_EnchantPoison.Init();
	m_EnchantShock.Init();

	m_MagicDefenceDebuff.Init();
	m_StigmaDebuff.Init();
	
	m_Dryad.Init();
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
	m_Blind.Init();
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
	m_WaterHold.Init();
#endif EDT_WATER_HOLD_TEST

	//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
	m_Panic.Init();
	m_Pain.Init();
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_LegShot.Init();
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
	m_EarthQuake.Init();
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef RAVEN_WEAPON_TAKER
	m_SmokeBlind.Init();
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
	m_FlashBang.Init();
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	m_MarkOfCommander.Init();
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN


#endif CHUNG_SECOND_CLASS_CHANGE
}

#ifdef CHUNG_SECOND_CLASS_CHANGE
void CX2GameUnit::ExtraDamagePack::Init(CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType)
{
	switch(eExtraDamageType)
	{
	case CX2DamageManager::EDT_POISON:
		m_Poison.Init();
		m_Poison.m_ExtraDamageType				= CX2DamageManager::EDT_POISON;
		break;
	case CX2DamageManager::EDT_FIRE:
		m_Fire.Init();
		m_Fire.m_ExtraDamageType				= CX2DamageManager::EDT_FIRE;
		break;
	case CX2DamageManager::EDT_CURSE:
		m_Curse.Init();
		m_Curse.m_ExtraDamageType				= CX2DamageManager::EDT_CURSE;
		break;
	case CX2DamageManager::EDT_SLOW:
		m_Slow.Init();
		m_Slow.m_ExtraDamageType				= CX2DamageManager::EDT_SLOW;
		break;
	case CX2DamageManager::EDT_LIGHTNING:
		m_Lightning.Init();
		m_Lightning.m_ExtraDamageType			= CX2DamageManager::EDT_LIGHTNING;
		break;
	case CX2DamageManager::EDT_REVERSE_LEFT_RIGHT:
		m_ReverseLeftRight.Init();
		m_ReverseLeftRight.m_ExtraDamageType	= CX2DamageManager::EDT_REVERSE_LEFT_RIGHT;
		break;
	case CX2DamageManager::EDT_COLD:
		m_Cold.Init();
		m_Cold.m_ExtraDamageType				= CX2DamageManager::EDT_COLD;
		break;
	case CX2DamageManager::EDT_Y_PRESSED:
		m_Y_Pressed.Init();
		m_Y_Pressed.m_ExtraDamageType			= CX2DamageManager::EDT_Y_PRESSED;
		break;
	case CX2DamageManager::EDT_FROZEN:
		m_Frozen.Init();
		m_Frozen.m_ExtraDamageType				= CX2DamageManager::EDT_FROZEN;
		break;
	case CX2DamageManager::EDT_LEG_WOUND:
		m_LegWound.Init();
		m_LegWound.m_ExtraDamageType			= CX2DamageManager::EDT_LEG_WOUND;
		break;
	case CX2DamageManager::EDT_WEAPON_BREAK:
		m_WeaponBreak.Init();
		m_WeaponBreak.m_ExtraDamageType			= CX2DamageManager::EDT_WEAPON_BREAK;
		break;
	case CX2DamageManager::EDT_ARMOR_BREAK:
		m_ArmorBreak.Init();
		m_ArmorBreak.m_ExtraDamageType			= CX2DamageManager::EDT_ARMOR_BREAK;
		break;
	case CX2DamageManager::EDT_ENTANGLE:
		m_Entangled.Init();
		m_Entangled.m_ExtraDamageType			= CX2DamageManager::EDT_ENTANGLE;
		break;
	case CX2DamageManager::EDT_DULL:
		m_Dull.Init();
		m_Dull.m_ExtraDamageType				= CX2DamageManager::EDT_DULL;
		break;
	case CX2DamageManager::EDT_TIME_BOMB:
		m_TimeBomb.Init();
		m_TimeBomb.m_ExtraDamageType			= CX2DamageManager::EDT_TIME_BOMB;
		break;
	case CX2DamageManager::EDT_STUN:
		m_Stun.Init();
		m_Stun.m_ExtraDamageType				= CX2DamageManager::EDT_STUN;
		break;
	case CX2DamageManager::EDT_AGING:
		m_Aging.Init();
		m_Aging.m_ExtraDamageType				= CX2DamageManager::EDT_AGING;
		break;
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
		m_EnchantBlaze.Init();
		m_EnchantBlaze.m_ExtraDamageType		= CX2DamageManager::EDT_ENCHANT_BLAZE;
		break;
	case CX2DamageManager::EDT_ENCHANT_FROZEN:
		m_EnchantFrozen.Init();
		m_EnchantFrozen.m_ExtraDamageType		= CX2DamageManager::EDT_ENCHANT_FROZEN;
		break;
	case CX2DamageManager::EDT_ENCHANT_POISON:
		m_EnchantPoison.Init();
		m_EnchantPoison.m_ExtraDamageType		= CX2DamageManager::EDT_ENCHANT_POISON;
		break;
	case CX2DamageManager::EDT_ENCHANT_SHOCK:
		m_EnchantShock.Init();
		m_EnchantShock.m_ExtraDamageType		= CX2DamageManager::EDT_ENCHANT_SHOCK;
		break;
	case CX2DamageManager::EDT_MAGIC_DEFENCE_DEBUFF:
		m_MagicDefenceDebuff.Init();
		m_MagicDefenceDebuff.m_ExtraDamageType	= CX2DamageManager::EDT_MAGIC_DEFENCE_DEBUFF;
		break;
	case CX2DamageManager::EDT_STIGMA_DEBUFF:
		m_StigmaDebuff.Init();
		m_StigmaDebuff.m_ExtraDamageType		= CX2DamageManager::EDT_STIGMA_DEBUFF;
		break;
	case CX2DamageManager::EDT_DRYAD:
		m_Dryad.Init();
		m_Dryad.m_ExtraDamageType				= CX2DamageManager::EDT_DRYAD;
		break;
	case CX2DamageManager::EDT_BLIND:
		m_Blind.Init();
		m_Blind.m_ExtraDamageType				= CX2DamageManager::EDT_BLIND;
		break;
	case CX2DamageManager::EDT_WATER_HOLD:
		m_WaterHold.Init();
		m_WaterHold.m_ExtraDamageType			= CX2DamageManager::EDT_WATER_HOLD;
		break;
	case CX2DamageManager::EDT_PANIC:
		m_Panic.Init();
		m_Panic.m_ExtraDamageType				= CX2DamageManager::EDT_PANIC;
		break;
	case CX2DamageManager::EDT_PAIN:
		m_Pain.Init();
		m_Pain.m_ExtraDamageType				= CX2DamageManager::EDT_PAIN;
		break;
	case CX2DamageManager::EDT_LEG_SHOT:
		m_LegShot.Init();
		m_LegShot.m_ExtraDamageType				= CX2DamageManager::EDT_LEG_SHOT;
		break;
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
	case CX2DamageManager::EDT_EARTH_QUAKE:
		m_EarthQuake.Init();
		m_EarthQuake.m_ExtraDamageType			= CX2DamageManager::EDT_EARTH_QUAKE;
		break;
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE
#ifdef VELDER_SECRET_DUNGEON
	case CX2DamageManager::EDT_SCALE_CRITICAL_BUFF:
		m_ScaleCriticalBuff.Init();
		m_ScaleCriticalBuff.m_ExtraDamageType	= CX2DamageManager::EDT_SCALE_CRITICAL_BUFF;
		break;
	case CX2DamageManager::EDT_BERSERKER_BUFF:
		m_BerserkerBuff.Init();
		m_BerserkerBuff.m_ExtraDamageType	= CX2DamageManager::EDT_BERSERKER_BUFF;
		break;
	case CX2DamageManager::EDT_MANA_BUFF:
		m_ManaBuff.Init();
		m_ManaBuff.m_ExtraDamageType	= CX2DamageManager::EDT_MANA_BUFF;
		break;
	case CX2DamageManager::EDT_ATTACK_ALL_TEAM:
		m_AttackAllTeam.Init();
		m_AttackAllTeam.m_ExtraDamageType	= CX2DamageManager::EDT_ATTACK_ALL_TEAM;
		break;
#endif //VELDER_SECRET_DUNGEON
#ifdef UPGRADE_RAVEN
	case CX2DamageManager::EDT_RUNJUMP_SPEED:
		m_RunJumpSpeed.Init();
		m_RunJumpSpeed.m_ExtraDamageType = CX2DamageManager::EDT_RUNJUMP_SPEED;
		break;
#endif

#ifdef RAVEN_WEAPON_TAKER
	case CX2DamageManager::EDT_SMOKE_BLIND:
		m_SmokeBlind.Init();
		m_SmokeBlind.m_ExtraDamageType				= CX2DamageManager::EDT_SMOKE_BLIND;
		break;
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
	case CX2DamageManager::EDT_FLASH_BANG:
		m_FlashBang.Init();
		m_FlashBang.m_ExtraDamageType				= CX2DamageManager::EDT_FLASH_BANG;
		break;
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2DamageManager::EDT_MARK_OF_COMMANDER:
		m_MarkOfCommander.Init();
		m_MarkOfCommander.m_ExtraDamageType				= CX2DamageManager::EDT_MARK_OF_COMMANDER;
		break;
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2DamageManager::EDT_START_OF_DELAYED_FIRING:
		m_StartOfDelayedFiring.Init();
		m_StartOfDelayedFiring.m_ExtraDamageType		= CX2DamageManager::EDT_START_OF_DELAYED_FIRING;
		break;
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2DamageManager::EDT_BLEEDING:
		m_Bleeding.Init();
		m_Bleeding.m_ExtraDamageType					= CX2DamageManager::EDT_BLEEDING;
		break;
#endif SERV_RAVEN_VETERAN_COMMANDER
	}
	return;
}
#endif CHUNG_SECOND_CLASS_CHANGE

bool CX2GameUnit::ExtraDamagePack::Verify()
{
	if( m_Poison.Verify() == false 
		|| m_Fire.Verify() == false 
		|| m_Curse.Verify() == false 
		|| m_Slow.Verify() == false 
		|| m_Lightning.Verify() == false
		|| m_ReverseLeftRight.Verify() == false 
		|| m_Cold.Verify() == false 
		|| m_Y_Pressed.Verify() == false 
		|| m_Frozen.Verify() == false
		|| m_LegWound.Verify() == false
		|| m_WeaponBreak.Verify() == false
		|| m_ArmorBreak.Verify() == false
		|| m_Entangled.Verify() == false
		|| m_Dull.Verify() == false
		|| m_TimeBomb.Verify() == false
		|| m_Stun.Verify() == false
		|| m_Aging.Verify() == false
		|| m_EnchantBlaze.Verify() == false 
		|| m_EnchantFrozen.Verify() == false 
		|| m_EnchantPoison.Verify() == false
		|| m_EnchantShock.Verify() == false 
		|| m_MagicDefenceDebuff.Verify() == false 
		|| m_StigmaDebuff.Verify() == false
		|| m_Dryad.Verify() == false
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.12.7] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
		|| m_Blind.Verify() == false
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
		|| m_WaterHold.Verify() == false
#endif EDT_WATER_HOLD_TEST

	//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		|| m_Panic.Verify() == false
		|| m_Pain.Verify() == false
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef CHUNG_SECOND_CLASS_CHANGE
		|| m_LegShot.Verify() == false
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		|| m_EarthQuake.Verify() == false
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE
		// edit verify 너무 많아서 성능 저하 우려됨
		// 더이상 추가하지 않음!
		)
	{
		return false;
	}
	return true;
}

void CX2GameUnit::SetLockOnNearTarget_LUA( CX2DamageEffect::CEffect* pCEffect, int randomOffset )
{
	if( pCEffect != NULL )
	{
		bool bUserUnit;
#ifdef FIX_PET_ATTACK
		UidType uid = g_pX2Game->GetLockOnNearstTargetOnlyBasic( (CX2Room::TEAM_NUM)GetTeam(), GetPos(), bUserUnit );
#else
		UidType uid = g_pX2Game->GetLockOnNearstTarget( (CX2Room::TEAM_NUM)GetTeam(), GetPos(), bUserUnit );
#endif

		if( bUserUnit == true )
			pCEffect->SetLockOnUnitUID( uid );
		else
			pCEffect->SetLockOnNPCUID( (int)uid );

		pCEffect->SetLockOnRandomSeed( randomOffset );
	}
}	

#ifdef NEW_SKILL_2010_11
//{{ oasis907 : 김상윤 [2010.11.8] // 로드 나이트 - 샌드 스톰 - EDT_BLIND
float CX2GameUnit::GetAttackRateByAbnormalityBlind()
{
	if(m_ExtraDamagePack.m_Blind.m_fTime > 0)
	{
		return m_ExtraDamagePack.m_Blind.m_fDamageRate;
	}
	return 1.f;
}
//}}
#endif NEW_SKILL_2010_11

//{{ 2011-04 에 패치될 청 캐시 스킬
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				

/** @function : GetCriticalValueFromDefender
	@brief : 피격자가 크리티컬을 당할 기준 확률 계산
	@param : void
	@return : 확률 (공격자의 크리티컬 확률이 이것보다 크면 크리티컬 데미지를 받는다.)
*/
const float CX2GameUnit::GetCriticalValueFromDefender()
{
	// 공격자로부터 Critical 데미지를 받을 확률
	float fCriticalRateToGetFromAttacker = GetRandomFloat( CKTDXRandomNumbers::SRO_ATTACK_CRITICAL );
	
	// 패닉 상태인 경우
	if ( 0.0f < m_ExtraDamagePack.m_Panic.m_fTime )
	{
		fCriticalRateToGetFromAttacker -= m_ExtraDamagePack.m_Panic.m_fCriticalRateToDefender;
	}

	fCriticalRateToGetFromAttacker -= GetAntiCritialRate();

	return fCriticalRateToGetFromAttacker;
}
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
//}} 2011-04 에 패치될 청 캐시 스킬

#ifdef GRAPPLING_TEST
//////////////////////////////////////////////////////////////////////////
void CX2GameUnit::GrapplingState::ResetGrapplingState( bool OnlyMe /*= false*/ )
{
	if( false == OnlyMe )
	{
		CX2GameUnit* pGrappler = GetUnitGrapplingMe();
		if( NULL != pGrappler )
		{
			pGrappler->ResetGrapplingState( true );
		}

		CX2GameUnit* pGrappled = GetUnitGrappledByMe();
		if( NULL != pGrappled )
		{
			pGrappled->ResetGrapplingState( true );
		}
	}

	m_UnitUIDGrapplingMe = -1;
	m_UnitUIDGrappledByMe = -1;

}

void CX2GameUnit::GrapplingState::SetUnitGrapplingMe( GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
{
	m_eUnitTypeGrapplingMe = eGameUnitType;
	m_UnitUIDGrapplingMe = unitUID;
}

CX2GameUnit* CX2GameUnit::GrapplingState::GetUnitGrapplingMe()
{
	if( m_UnitUIDGrapplingMe < 0 )
		return NULL;

	switch( m_eUnitTypeGrapplingMe )
	{
	case CX2GameUnit::GUT_USER:
		{
			return (CX2GameUnit*) g_pX2Game->GetUserUnitByUID( m_UnitUIDGrapplingMe );
		} break;

	case CX2GameUnit::GUT_NPC:
		{
			return (CX2GameUnit*) g_pX2Game->GetNPCUnitByUID( (int)m_UnitUIDGrapplingMe );
		} break;
	}

	return NULL;
}

void CX2GameUnit::GrapplingState::SetUnitGrappledByMe( GAME_UNIT_TYPE eGameUnitType, UidType unitUID )
{
	m_eUnitTypeGrappledByMe = eGameUnitType;
	m_UnitUIDGrappledByMe = unitUID;
}

CX2GameUnit* CX2GameUnit::GrapplingState::GetUnitGrappledByMe()
{
	if( m_UnitUIDGrappledByMe < 0 )
		return NULL;

	switch( m_eUnitTypeGrappledByMe )
	{
	case CX2GameUnit::GUT_USER:
		{
			return (CX2GameUnit*) g_pX2Game->GetUserUnitByUID( m_UnitUIDGrappledByMe );
		} break;

	case CX2GameUnit::GUT_NPC:
		{
			return (CX2GameUnit*) g_pX2Game->GetNPCUnitByUID( (int)m_UnitUIDGrappledByMe );
		} break;
	}

	return NULL;
}

void CX2GameUnit::GrapplingState::CalcGrapplingPosition()
{
	if( NULL != m_pFrameGrappling )
	{
		GetFramePos( &m_vGrapplingPosition, m_pFrameGrappling );
		GetFrameRotateDegree( &m_vGrapplingRotation, m_pFrameGrappling );
	}
	//else
	//{
	//	//m_vGrapplingPosition = GetPos() + D3DXVECTOR3( 0.f, GetUnitHeight(false) + 150.f, 0.f );
	//	INIT_VECTOR3( m_vGrapplingPosition, 0, 0, 0 );
	//	INIT_VECTOR3( m_vGrapplingRotation, 0, 0, 0 );
	//}
}

void CX2GameUnit::GrapplingState::CalcGrappledPosition()
{
	if( NULL != m_pFrameGrappled )
	{
		GetFramePos( &m_vGrappledPosition, m_pFrameGrappled );
		GetFrameRotateDegree( &m_vGrappledRotation, m_pFrameGrappled );
	}
	//else
	//{
	//	//m_vGrappledPosition = GetPos() + D3DXVECTOR3( 0.f, GetUnitHeight(false) + 150.f, 0.f );
	//	INIT_VECTOR3( m_vGrappledPosition, 0, 0, 0 );
	//	INIT_VECTOR3( m_vGrappledRotation, 0, 0, 0 );
	//}
}

#endif GRAPPLING_TEST

#ifdef CHUNG_SECOND_CLASS_CHANGE
// oasis907 : 김상윤 [2011.6.20] 청 2차 전직 - 아이언 팔라딘
void CX2GameUnit::ProcessTransitDebuff( const CX2DamageManager::DamageData* pDamageData_, CX2GameUnit* pTargetUnit_ )
{
	if ( NULL != pTargetUnit_ && pTargetUnit_->GetNowHp() > 0.0f )
	{
		if ( m_vecKDebuffFactorListToTransit.empty() )
			GetBuffFactorOnlyDebuff( m_vecKDebuffFactorListToTransit );

#ifdef FIX_EDT_ENCHANT
		int iMaxAccumulation = 0;
		if( NULL != pDamageData_ )
			iMaxAccumulation = pDamageData_->m_ExtraDamage.m_Accumulation;

		// 전이 가능한 최대 개수 제한
		if( static_cast<int>(m_vecKDebuffFactorListToTransit.size()) > iMaxAccumulation )
			m_vecKDebuffFactorListToTransit.resize( iMaxAccumulation );
		
		int iTransitDebuffNum = static_cast<int>(m_vecKDebuffFactorListToTransit.size());
#endif // FIX_EDT_ENCHANT

		if ( !m_vecKDebuffFactorListToTransit.empty() )
			pTargetUnit_->SetKBuffFactorPacketToGameUnit( m_vecKDebuffFactorListToTransit );

#ifdef FIX_EDT_ENCHANT
		/** @function : TransitExtraDamage
			@brief : 공격한 유저의 디버프를 피격당한 유저에게로 전이
			@return : 전이 성공 : true, 전이 실패 : false
		*/
		auto TransitExtraDamage = [OUT &iTransitDebuffNum, IN iMaxAccumulation]
		( OUT CX2DamageManager::ExtraDamageData& DefenderExtraDamage, OUT CX2DamageManager::ExtraDamageData& AttackerExtraDamage)->bool
		{
			// 전이 가능한 최대 개수 검사
			if( iTransitDebuffNum >= iMaxAccumulation )
				return false;

			// EDT 존재 여부 검사
			if( AttackerExtraDamage.m_fTime > 0.f )
			{
				++iTransitDebuffNum;
				DefenderExtraDamage = AttackerExtraDamage;
				AttackerExtraDamage.Init();
			}

			return true;
		};
		// 블레이즈
 		if( false == TransitExtraDamage( pTargetUnit_->m_ExtraDamagePack.m_EnchantBlaze, m_ExtraDamagePack.m_EnchantBlaze) )
 			return;
		// 프로즌
		if( false == TransitExtraDamage( pTargetUnit_->m_ExtraDamagePack.m_EnchantFrozen, m_ExtraDamagePack.m_EnchantFrozen) )
			return;
		// 포이즌
		if( false == TransitExtraDamage( pTargetUnit_->m_ExtraDamagePack.m_EnchantPoison, m_ExtraDamagePack.m_EnchantPoison) )
			return;
		// 쇼크
		if( false == TransitExtraDamage( pTargetUnit_->m_ExtraDamagePack.m_EnchantShock, m_ExtraDamagePack.m_EnchantShock) )
			return;
#endif // FIX_EDT_ENCHANT
	}
}

void CX2GameUnit::FadeInWeaponRenderEffect(const D3DXCOLOR& _StartColor, const D3DXCOLOR& _EndColor, float _fChangeTime, float _fDuration)
{
#ifdef NEW_HENIR_TEST
	if(m_fPulseTimeLeft > _fDuration )
		return;
#endif NEW_HENIR_TEST

	SetEnableWeaponRenderEffect(true);

	m_fFadeTimeLeft			= _fDuration;
	m_fFadeChangeTimeLeft	= _fChangeTime;
	m_fFadeChangeTime		= _fChangeTime;

	m_WeaponRenderEffectColorStart	= _StartColor;
	m_WeaponRenderEffectColorEnd	= _EndColor;

	return;
}
void CX2GameUnit::PulseWeaponRenderEffect(const D3DXCOLOR& _StartColor, const D3DXCOLOR& _EndColor, float _fPeriod, float _fDuration, PULSE_WEAPON_COLOR_ID _ePulseWeaponColorID /* = PWCI_UNDEFINED */ )
{
#ifdef NEW_HENIR_TEST
	if(m_fPulseTimeLeft > _fDuration )
		return;
#endif NEW_HENIR_TEST

	SetEnableWeaponRenderEffect(true);

	m_eNowPulseWeaponColorID = _ePulseWeaponColorID;

	m_fPulseTimeLeft	= _fDuration;
	m_fPulseDuration	= _fDuration;
	m_fPulsePeriod		= _fPeriod;

	m_WeaponRenderEffectColorStart	= _StartColor;
	m_WeaponRenderEffectColorEnd	= _EndColor;


	return;
}

void CX2GameUnit::FrameMoveWeaponRenderEffect()
{
	if(GetEnableWeaponRenderEffect() == false)
		return;

	D3DXCOLOR DiffColor = m_WeaponRenderEffectColorEnd - m_WeaponRenderEffectColorStart;
	if( m_fFadeTimeLeft > 0.f )
	{
		if(m_fFadeChangeTimeLeft < 0.f)
			m_fFadeChangeTimeLeft = 0;

		m_WeaponRenderEffectColor = m_WeaponRenderEffectColorEnd - (m_fFadeChangeTimeLeft/m_fFadeChangeTime) * DiffColor;

		m_fFadeTimeLeft -= m_fElapsedTime;
		m_fFadeChangeTimeLeft -= m_fElapsedTime;
	}
	else if( m_fPulseTimeLeft  > 0.f )
	{
		float fPulseElapsedTime = m_fPulseDuration - m_fPulseTimeLeft;
		int iDiv = (int) (fPulseElapsedTime / m_fPulsePeriod);
		float fPulsePeriodElapsedTime = fPulseElapsedTime - ((float)iDiv * m_fPulsePeriod);
		if( iDiv % 2 == 0) // Up Phase
		{
			m_WeaponRenderEffectColor = m_WeaponRenderEffectColorStart + (fPulsePeriodElapsedTime/m_fPulsePeriod) * DiffColor;
		}
		else // Down Phase
		{
			m_WeaponRenderEffectColor = m_WeaponRenderEffectColorEnd - (fPulsePeriodElapsedTime/m_fPulsePeriod) * DiffColor;
		}
		m_fPulseTimeLeft -= m_fElapsedTime;
	}
	else
	{
		SetEnableWeaponRenderEffect(false);
	}
}
void CX2GameUnit::SetEnableWeaponRenderEffect(bool _bVal)
{
	m_bEnableWeaponRenderEffect = _bVal;
	if(_bVal == false)
	{
		m_fFadeTimeLeft			= 0.f;
		m_fFadeChangeTime		= 0.f;
		m_fFadeChangeTimeLeft	= 0.f;

		m_fPulseDuration		= 0.f;
		m_fPulsePeriod			= 0.f;
		m_fPulseTimeLeft		= 0.f;

		m_eNowPulseWeaponColorID	= PWCI_NONE; 
	}
}
#endif CHUNG_SECOND_CLASS_CHANGE

/*virtual*/ void CX2GameUnit::DeleteMinorParticle()
{
	DeleteGameUnitMinorParticle();
}

void CX2GameUnit::UpdateVisibilitySmallGageAndName()
{
	if ( false == GetShowObject() )
	{
		SetShowSmallGageAndName( false );
		return;
	}
	//*
	else if ( false == IsMyUnit() )
	{
		CX2GUUser* pMyUser = g_pX2Game->GetMyUnit();

		if ( NULL != pMyUser && false == pMyUser->GetWhiteOut() 
			&& GetNowHp() > 0.0f && GetColor().a > 0.5f && g_pX2Game->GetFocusUnit() )
		{
#ifdef FIELD_BOSS_RAID // 렌더 거리 체크
			if( true == GetIsBosRaidNPC() )
			{
				SetShowSmallGageAndName( false );
				return;
			}
			else
#endif // FIELD_BOSS_RAID
			{
				const D3DXVECTOR3& vFocusGameUnitPosition = g_pX2Game->GetFocusUnit()->GetPos();
				const float fDistance3Sq = GetDistance3Sq( vFocusGameUnitPosition, GetPos() );

				if ( fDistance3Sq < 1000000.0f )
				{
					CX2Camera* pCX2Camera = g_pX2Game->GetX2Camera();
					const CKTDGCamera& kCamera = pCX2Camera->GetCamera();
					const D3DXVECTOR3 lookVec = kCamera.GetLookVec();
					const D3DXVECTOR3& vEyePos = kCamera.GetEye();

					const D3DXVECTOR3 vFromCameraToOwnerUnitDirection = GetPos() - vEyePos;
					const float fDelta = D3DXVec3Dot( &lookVec, &vFromCameraToOwnerUnitDirection );

					if ( fDelta >= 0 )
					{
						SetShowSmallGageAndName( true );
						return;
					}
				}
			}
		}
		
		// 이곳에 들어왔는데 위의 fDelta >= 0 에 포함되지 못했으면 false
		SetShowSmallGageAndName( false );
	}
	else
	//*/
		SetShowSmallGageAndName( true );
}

/*virtual*/ CX2GageData* CX2GameUnit::CreateGageData()
{
	if ( IsMyUnit() )
	{
		const CX2GageData* pGageData 
			= CX2GageManager::GetInstance()->GetMyGageData();

		if ( NULL != pGageData )
			return new CX2GageData( *pGageData );
		else
			return new CX2GageData();
	}
	else
		return new CX2GageData();
}

/*virtual*/ CX2GageUI* CX2GameUnit::CreateGageUI( const CX2GageData* pOwnerGageData_ )
{
	if ( IsMyUnit() )
	{
		return new CX2MySmallGageUI( this );
	}
	else
		return new CX2SmallGageUI( this );
}

void CX2GameUnit::SetHpTexture( const WCHAR* pwszSmallHpBar_ )
{
	if ( false == IsNullGageUI() )
		m_pGageUI->SetHpTexture( pwszSmallHpBar_ );
}

void CX2GameUnit::SetMpTexture( const WCHAR* pwszSmallMpBar_ )
{
	if ( false == IsNullGageUI() )
		m_pGageUI->SetMpTexture( pwszSmallMpBar_ );
}

void CX2GameUnit::DownCrashCamera( const float fSpeed_ /*= -100.0f*/, const float fAccel_ /*= 1500.0f*/ ) const
{
#ifdef FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
		 GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ ) )
#else  FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ )
#endif FIX_OBSERVER_MODE_CRASH
	{
		g_pX2Game->GetX2Camera()->GetCamera().DownCrashCamera( fSpeed_, fAccel_ );
	}
}

void CX2GameUnit::UpCrashCamera( const float fSpeed_ /*= 100.0f*/, const float fAccel_ /*= -1500.0f*/ ) const
{
#ifdef FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
		GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ ) )
#else  FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ )
#endif FIX_OBSERVER_MODE_CRASH
	{
		g_pX2Game->GetX2Camera()->GetCamera().UpCrashCamera( fSpeed_, fAccel_ );
	}
}

void CX2GameUnit::UpDownCrashCamera( const float fGap_ /*= 10.0f*/, const float fTime_ /*= 0.2f*/ ) const
{
#ifdef FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
		GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ ) )
#else  FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ )
#endif FIX_OBSERVER_MODE_CRASH
	{
		g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( fGap_, fTime_ );
	}
}

void CX2GameUnit::UpDownCrashCameraNoReset( const float fGap_ /*= 10.0f*/, const float fTime_ /*= 0.2f */ ) const
{
#ifdef FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
		GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ ) )
#else  FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ )
#endif FIX_OBSERVER_MODE_CRASH
	{
		g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCameraNoReset( fGap_, fTime_ );
	}
}

void CX2GameUnit::LeftRightCrashCameraNoReset( const float fGap_ /*= 10.0f*/, const float fTime_ /*= 0.2f */ ) const
{
#ifdef FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
		GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ ) )
#else  FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ )
#endif FIX_OBSERVER_MODE_CRASH
	{
		g_pX2Game->GetX2Camera()->GetCamera().LeftRightCrashCameraNoReset( fGap_, fTime_ );
	}
}

void CX2GameUnit::ShakeLeftRight( const float fGap_ /*= 10.f*/, const float fTime_ /*= 0.2f*/, const float fTimeGap_ /*= 0.05f */ ) const
{
#ifdef FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
		GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ ) )
#else  FIX_OBSERVER_MODE_CRASH
	if ( IsMyUnit() || GetDistance3Sq( g_pX2Game->GetMyUnit()->GetPos(), GetPos() ) < LIMIT_DISTANCE_SQ )
#endif FIX_OBSERVER_MODE_CRASH
	{
		g_pX2Game->GetX2Camera()->GetCamera().ShakeLeftRight( fGap_, fTime_, fTimeGap_ );
	}
}

/*virtual*/ void CX2GameUnit::UpNowMpPerHitOthers( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ )
{
	switch ( g_pX2Game->GetGameType() )
	{
	case CX2Game::GT_PVP:
		{
			UpNowMp( ( fHitAddMp_ + (3.0f * iAddValueByTeam_) ) * fRel_ );

		} break;

	case CX2Game::GT_DUNGEON:
	case CX2Game::GT_BATTLE_FIELD:
		{
			UpNowMp( ( fHitAddMp_ + (3.0f * iAddValueByTeam_) ) * 0.8f * fRel_ );
		} break;

	default:
		ASSERT( !L"wrong path" );
		break;
	}
}

void CX2GameUnit::UpNowMpPerHitOthersWithDamageEffect( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ )
{
	CX2GameUnit::UpNowMpPerHitOthers( fHitAddMp_, iAddValueByTeam_, fRel_ );
}

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
/** @function	: UpNowHpPerHitOthers
	@brief		: HIT_ADD_HP_PERCENT 를 통한 공격 성공시, 공격자의 체력 회복 ( 총 HP의 배율 )
	@param		: 회복 배율
*/
void CX2GameUnit::UpNowHpPerHitOthers( IN const float fHitAddHpPer_ )
{
	const float fAddHPRate = max( GetMaxHp() * fHitAddHpPer_, 1.f );	/// 회복 배율

	UpNowHp( fAddHPRate );

	D3DXVECTOR3 pos	= GetPos();		/// 문구 위치 설정
	pos.y += 50.0f;

	WCHAR wszBuf[256] = {0,};		/// 문구 설정
	StringCchPrintf( wszBuf, 256, L"HP +%d", static_cast<int>( fAddHPRate ) );

	g_pData->GetPicCharGreen()->DrawText( wszBuf, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef ADD_MEMO_1ST_CLASS //김창한
/** @function	: UpNowHpPerHitbyAttackPower
	@brief		: HIT_ADD_HP_PERCENT_ATTACKPOWER 를 통한 공격 성공시, 공격자의 체력 회복 ( 물마공 평균에 비례 )
	@param		: 회복 배율
*/
void CX2GameUnit::UpNowHpPerHitbyAttackPower( IN const float fHitAddHpPer_ )
{
	const CX2Stat::Stat& MyStat = GetStat().GetStat();
	const float fAddHPRate = ( (MyStat.m_fAtkMagic + MyStat.m_fAtkPhysic) * 0.5f * fHitAddHpPer_ );

	UpNowHp( fAddHPRate );

	D3DXVECTOR3 pos	= GetPos();		/// 문구 위치 설정
	pos.y += 50.0f;

	WCHAR wszBuf[256] = {0,};		/// 문구 설정
	StringCchPrintf( wszBuf, 256, L"HP +%d", static_cast<int>( fAddHPRate ) );

	g_pData->GetPicCharGreen()->DrawText( wszBuf, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
}
#endif //ADD_MEMO_1ST_CLASS

/** @function : SetBuffFactorToDamageData
	@brief : 버프팩터 ID로 게임유닛의 데미지데이터에 버프팩터 추가
	@param : 버프팩터ID(uiBuffFactorId_)
	@return : 수행함(true), 안함(false)
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
/*virtual*/ bool CX2GameUnit::SetBuffFactorToDamageData( const UINT uiBuffFactorId_, IN const UINT uiLevel_/* = 1*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
/*virtual*/ bool CX2GameUnit::SetBuffFactorToDamageData( const UINT uiBuffFactorId_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( uiBuffFactorId_ );
	if ( NULL != ptrBuffFactor )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr( uiLevel_ );
		
		if ( NULL != ptrBuffFactorClone )
		{
			GetDamageData().PushBuffFactor( ptrBuffFactorClone );
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		GetDamageData().PushBuffFactor( ptrBuffFactor );
#endif //UPGRADE_SKILL_SYSTEM_2013
		return true;
	}

	return false;
}

/** @function : SetBuffFactorToGameUnitByBuffFactorID
	@brief : 버프팩터 ID로 게임유닛에 버프 적용
	@param : 버프팩터ID(uiBuffFactorId_)
	@return : 수행함(true), 안함(false)
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
/*virtual*/ bool CX2GameUnit::SetBuffFactorToGameUnitByBuffFactorID( const UINT uiBuffFactorId_, IN const UINT uiLevel_ /*= 1*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
/*virtual*/ bool CX2GameUnit::SetBuffFactorToGameUnitByBuffFactorID( const UINT uiBuffFactorId_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
	if ( CanApplyBuffToGameUnit() )
	{
		CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( uiBuffFactorId_ );
		if ( NULL != ptrBuffFactor )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr( uiLevel_ );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr();
#endif //UPGRADE_SKILL_SYSTEM_2013
			if ( NULL != ptrBuffFactorClone )
			{
				ptrBuffFactorClone->SetGameUnitBuffFactor( this );
				return SetBuffFactorToGameUnit( *ptrBuffFactorClone );
			}
		}
	}
	return false;
}

#ifdef ADD_LIVE_CREATOR_FINALIZER
/** @function : SetBuffFactorToGameUnitByBuffFactorIDAndCreatorID
	@brief : 버프팩터 ID로 게임유닛에 버프 적용 및 버프 생성자 ID 저장
	@param : 버프팩터ID(uiBuffFactorId_), 버프 생성자 ID(uidCreatorID_)
	@return : 수행함(true), 안함(false)
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
/*virtual*/ bool CX2GameUnit::SetBuffFactorToGameUnitByBuffFactorIDAndCreatorID( const UINT uiBuffFactorId_, const UidType iCreatorID_,
	const bool bIsUserID_ /*= true*/, IN const UINT uiLevel_/* = 1*/ )
#else //UPGRADE_SKILL_SYSTEM_2013
/*virtual*/ bool CX2GameUnit::SetBuffFactorToGameUnitByBuffFactorIDAndCreatorID( const UINT uiBuffFactorId_, const UidType iCreatorID_,
	const bool bIsUserID_ /*= true*/ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{

	if ( CanApplyBuffToGameUnit() )
	{
		CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( uiBuffFactorId_ );
		if ( NULL != ptrBuffFactor )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr( uiLevel_ );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr();
#endif //UPGRADE_SKILL_SYSTEM_2013
			if ( NULL != ptrBuffFactorClone )
			{
				ptrBuffFactorClone->SetGameUnitBuffFactor( this, iCreatorID_, bIsUserID_ );		/// 버프 시전자의 유닛 ID를 저장
				return SetBuffFactorToGameUnit( *ptrBuffFactorClone );
			}
		}
	}
	return false;

}
#endif ADD_LIVE_CREATOR_FINALIZER

/** @function : SetBuffFactorToGameUnit
	@brief : 스킬템플릿에 있는 버프팩터로 게임유닛에 버프 적용
	@param : 스킬트리정보포인터(pSkillTemplet_)
	@return : 수행함(true), 안함(false)
*/
/*virtual*/ bool CX2GameUnit::SetBuffFactorToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const UINT uiIndex_ )
{
	if ( CanApplyBuffToGameUnit() )
	{
		if ( NULL != pSkillTemplet_ && uiIndex_ < pSkillTemplet_->m_vecBuffFactorPtr.size() )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( GetGameUnitType() != CX2GameUnit::GUT_USER )
				return false;
            CX2Unit* pUnit = static_cast<CX2GUUser*>( this )->GetUnit();
            if ( pUnit == NULL )
                return false;

			const CX2UserSkillTree& userSkillTree = pUnit->GetUnitData().m_UserSkillTree;
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨

			CX2BuffFactorPtr ptrBuffFactorClone = pSkillTemplet_->m_vecBuffFactorPtr[uiIndex_]->GetClonePtr( iSkillTempletLevel );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2BuffFactorPtr ptrBuffFactorClone = pSkillTemplet_->m_vecBuffFactorPtr[uiIndex_]->GetClonePtr();
#endif //UPGRADE_SKILL_SYSTEM_2013
			if ( NULL != ptrBuffFactorClone )
			{
				ptrBuffFactorClone->SetGameUnitBuffFactor( this );
				return SetBuffFactorToGameUnit( *ptrBuffFactorClone );
			}
		}
		else
			return DISPLAY_ERROR( L"SkillTemplet Invalid" );
	}
	
	return false;
}

/** @function : SetBuffFactorToGameUnit
	@brief : 게임유닛에게 버프 적용
	@param : 어떤 버프를 적용할 것인가에 대한 정보(buffFactor_)
	@return : 수행함(true), 안함(false)
*/
/*virtual*/ bool CX2GameUnit::SetBuffFactorToGameUnit( const CX2BuffFactor& buffFactor_ )
{
	CX2BuffTempletPtr ptrBuffTemplet = CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( buffFactor_.GetBuffTempletID() );

	if ( NULL == ptrBuffTemplet )
		return DISPLAY_ERROR( L"BuffTemplet doesn't exist" );

	const float fRegist = ( buffFactor_.GetIgnoreRegistForRate() ? 
		0.0f : GetEnchantResist( static_cast<CX2EnchantItem::ENCHANT_TYPE>( ptrBuffTemplet->GetRegistType() ) ) / CX2EnchantItem::EAR_MAX_VALUE );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	const float fRate = buffFactor_.GetRate( 1 ); //걸릴 확률
#else //UPGRADE_SKILL_SYSTEM_2013
	const float fRate = buffFactor_.GetRate(); //걸릴 확률
#endif //UPGRADE_SKILL_SYSTEM_2013
	const float fRateAndRegist = fRate * (1.0f - fRegist); //Regist값 포함한 걸릴 확률
	float fRandomFloat = GetRandomFloat() ;

	if ( fRate < fRandomFloat )
	{
		return false;
	}
	
	if ( fRateAndRegist < fRandomFloat ) // 저항으로 인한 실패시에 RESIST출력
	{
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( L"RESIST", GetHeadBonePos(), GetDirVector(), CKTDGPicChar::AT_CENTER );
		return false;
	}

	/// 걸려있는 버프가 없으면
	if ( m_vecBuffTempletPtr.empty() )
	{
		PushNewBuffTempletToGameUnit( ptrBuffTemplet, buffFactor_, 1 );
	}
	else
	{
		const BUFF_TEMPLET_ID eBuffTempletID = ptrBuffTemplet->GetBuffTempletID();

		auto FindFunc 
			= [eBuffTempletID]( CX2BuffTempletPtr ptrBuffTemplet_ ) {
				return eBuffTempletID == ptrBuffTemplet_->GetBuffTempletID();
		};

		const UINT uiAccumulationLimit = ptrBuffTemplet->GetAccumulationLimit();

		switch ( uiAccumulationLimit )
		{
		case 0:	/// 중복
			{	
				/// 버프아이디가 같은 버프의 개수를 버프를 구분하는 uniqueNum으로 사용
				UINT uiCount = std::count_if( m_vecBuffTempletPtr.begin(), m_vecBuffTempletPtr.end(), FindFunc );

				PushNewBuffTempletToGameUnit( ptrBuffTemplet, buffFactor_, uiCount );
			} break;

		case 1:	/// 교체
			{
				vector<CX2BuffTempletPtr>::iterator vItr
					= std::find_if( m_vecBuffTempletPtr.begin(), m_vecBuffTempletPtr.end(), FindFunc );

				if ( m_vecBuffTempletPtr.end() != vItr )
					(*vItr)->SetFactor( buffFactor_, this );
				else	/// 중복이 아닌 경우 에는 BUFF_TEMPLET_ID가 같은 버프가 1개 이상 존재 할 수 없기 때문에 uniqueNum은 1이다.
					PushNewBuffTempletToGameUnit( ptrBuffTemplet, buffFactor_, 1 );
			} break;

		default:	/// 중첩
			{
				vector<CX2BuffTempletPtr>::iterator vItr
					= std::find_if( m_vecBuffTempletPtr.begin(), m_vecBuffTempletPtr.end(), FindFunc );

				if ( m_vecBuffTempletPtr.end() != vItr )
				{
					/// 최대 중첩 카운트 보다 중첩된 횟수가 적으면
					if ( (*vItr)->GetAccumulationCountNow() < uiAccumulationLimit )
						(*vItr)->SetFactor( buffFactor_, this );
					else	/// 최대 중첩이 되었으면 종료조건만 갱신
						(*vItr)->SetFactorToOnlyFinalizer( buffFactor_, this );
				}
				else	/// 중복이 아닌 경우 에는 BUFF_TEMPLET_ID가 같은 버프가 1개 이상 존재 할 수 없기 때문에 uniqueNum은 1이다.
					PushNewBuffTempletToGameUnit( ptrBuffTemplet, buffFactor_, 1 );
			} break;
		}
	}
	return true;
}

/** @function : PushNewBuffTempletToGameUnit
	@brief : 기존에 걸려있는 버프가 아닌 경우(또는 중복) 새로운 버프 적용
	@param : 적용해야할 버프의 템플릿정보(ptrBuffTemplet_), 적용해야할 버프의 요소정보(buffFactor_), 버프아이디가 같은경우 구분하기위한 Unique값(uiUniqueNum_)
*/
void CX2GameUnit::PushNewBuffTempletToGameUnit( IN CX2BuffTempletPtr ptrBuffTemplet_, const CX2BuffFactor& buffFactor_, const UINT uiUniqueNum_ )
{
	CX2BuffTempletPtr ptrCloneBuffTemplet = ptrBuffTemplet_->GetClonePtr();
	if ( NULL != ptrCloneBuffTemplet )
	{
#ifdef FIELD_BOSS_RAID // 디버프 이뮨 검사
		if( true == GetIsImmuneDebuff() &&
			true == ptrCloneBuffTemplet->IsDeBuff() )
			return;
#endif // FIELD_BOSS_RAID

		ptrCloneBuffTemplet->SetUniqueNum( uiUniqueNum_ );
		if ( ptrCloneBuffTemplet->SetFactor( buffFactor_, this ) )
		{
			/// 디버프면 디버프카운트를, 버프면 버프카운트를 증가
			( true == ptrCloneBuffTemplet->IsDeBuff() ? SetNumOfDeBuff( GetNumOfDeBuff() + 1 ) : SetNumOfBuff( GetNumOfBuff() + 1 ) );
			/// 각 유닛마다 특정 버프가 실행될 때 셋팅해줘야 하는 값(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
			SetSpecificValueByBuffTempletID( ptrCloneBuffTemplet->GetBuffTempletID() );
			m_vecBuffTempletPtr.push_back( ptrCloneBuffTemplet );
#ifdef BUFF_ICON_UI		
			if( true == ptrCloneBuffTemplet->GetUseBuffIcon() )
			{
				CX2GageManager::GetInstance()->PushBuff( m_UnitUID, CX2GageUI::BuffIcon( ptrCloneBuffTemplet->GetBuffTempletID(),ptrCloneBuffTemplet->GetIconFileName(), 
					ptrCloneBuffTemplet->GetIconKeyName(), ptrCloneBuffTemplet->GetBuffName(), ptrCloneBuffTemplet->GetBuffDesc() ), ptrCloneBuffTemplet->IsDeBuff() );
			}
#endif //BUFF_ICON_UI
		}
	}
}

/** @function : EraseBuffTempletFromGameUnit
	@brief : 걸려있는 버프 중 템플릿ID와 일치하는 버프 해제
	@param : 버프템플릿ID(eBuffTempletID_)
*/
void CX2GameUnit::EraseBuffTempletFromGameUnit( const BUFF_TEMPLET_ID eBuffTempletID_ )
{
	auto FindFunc 
		= [eBuffTempletID_]( CX2BuffTempletPtr ptrBuffTemplet_ ) {
			return eBuffTempletID_ == ptrBuffTemplet_->GetBuffTempletID();
	};

	vector<CX2BuffTempletPtr>::iterator vItr
		= std::find_if( m_vecBuffTempletPtr.begin(), m_vecBuffTempletPtr.end(), FindFunc );

	if ( m_vecBuffTempletPtr.end() != vItr )
	{
		EraseBuffTempletFromGameUnitByIterator( vItr );
	}
}

/** @function : EraseBuffTempletFromGameUnitByIterator
	@brief : 전달 받은 iterator에 해당하는 버프 해제
	@param : 해제할 버프의 iterator
*/
void CX2GameUnit::EraseBuffTempletFromGameUnitByIterator( vector<CX2BuffTempletPtr>::iterator vItr_ )
{
	( true == (*vItr_)->IsDeBuff() ? SetNumOfDeBuff( GetNumOfDeBuff() - 1 ) : SetNumOfBuff( GetNumOfBuff() - 1 ) );
	UnSetSpecificValueByBuffTempletID( (*vItr_)->GetBuffTempletID() );
	(*vItr_)->DoFinishBehavior( this );
	(*vItr_)->DoFinishDisplayer( this );

#ifdef BUFF_ICON_UI		
	if( true == (*vItr_)->GetUseBuffIcon() )
		CX2GageManager::GetInstance()->EraseBuff( m_UnitUID, (*vItr_)->GetBuffTempletID(), (*vItr_)->IsDeBuff() );
#endif //BUFF_ICON_UI
	m_vecBuffTempletPtr.erase( vItr_ );
}

/** @function : ReserveToFinishBuffTempletFromGameUnit
	@brief : 다음 종료 체크시 해당 버프가 종료 되도록 하는 함수
	@param : 종료 시킬 버프 템플릿 ID
*/
void CX2GameUnit::ReserveToFinishBuffTempletFromGameUnit( const BUFF_TEMPLET_ID eBuffTempletID_ )
{
	auto FindFunc 
		= [eBuffTempletID_]( CX2BuffTempletPtr ptrBuffTemplet_ ) {
			return eBuffTempletID_ == ptrBuffTemplet_->GetBuffTempletID();
	};

	vector<CX2BuffTempletPtr>::iterator vItr
		= std::find_if( m_vecBuffTempletPtr.begin(), m_vecBuffTempletPtr.end(), FindFunc );

	if ( m_vecBuffTempletPtr.end() != vItr && NULL != *vItr )
		(*vItr)->ReserveToFinish();
}

/** @function : GetAppliedBuffCount
	@brief : 현재 걸려있는 버프/디버프의 개수를 알아내기 위한 함수
	@param : true인 경우 디버프의 개수 리턴(bIsDebuff_)
	@return : 버프 또는 디버프의 개수
*/
UINT CX2GameUnit::GetAppliedBuffCount( const bool bIsDeBuff_ ) const
{
	auto FindFunc = [bIsDeBuff_]( CX2BuffTempletPtr ptrBuffTemplet_ ) {
		return bIsDeBuff_ == ptrBuffTemplet_->IsDeBuff();
	};

	return std::count_if( m_vecBuffTempletPtr.begin(), m_vecBuffTempletPtr.end(), FindFunc );
}

/** @function : CreateAndInsertReverseLeftRightByBuff
	@brief : 좌우반전효과 생성 및 적용
	@param : 어떤 버프에 의해서 적용 됬는지에 대한 정보
*/
void CX2GameUnit::CreateAndInsertReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ )
{
	CX2ReverseLefRightPtr ptrReverseLeftRight = CX2ReverseLefRightPtr( new CX2ReverseLefRight( BuffIdentity_ ) );
	if ( NULL != ptrReverseLeftRight )
		m_vecReverseLeftRightPtr.push_back( ptrReverseLeftRight );
}

/** @function : ToggleReverseLeftRightByBuff
	@brief : 현재 적용된 좌우 반전을 반대로 적용(BuffIdentity_)
	@param : 어떤 버프에 의해서 발동된것에 적용을 해야하는지 정보
*/
void CX2GameUnit::ToggleReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2ReverseLefRightPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2ReverseLefRightPtr>::iterator, CX2ReverseLefRightPtr>( m_vecReverseLeftRightPtr, BuffIdentity_ );

	if ( m_vecReverseLeftRightPtr.end() != vItr )
		(*vItr)->ToggleReverseLeftRightByBuff();
}


/** @function : EraseReverseLeftRightByBuff
	@brief : 특정버프에 의한 좌우반전 삭제
	@param : 특정버프를 구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2ReverseLefRightPtr>::iterator vItr 
	= FindSameBuffIdentity<vector<CX2ReverseLefRightPtr>::iterator, CX2ReverseLefRightPtr>( m_vecReverseLeftRightPtr, BuffIdentity_ );

	if ( m_vecReverseLeftRightPtr.end() != vItr )
		m_vecReverseLeftRightPtr.erase( vItr );
}

/** @function : CreateAndInsertNeverMoveByBuff
	@brief : 현재 위치에 고정 효과 생성 및 적용
	@param : 어떤 버프에 의해서 적용 됬는지에 대한 정보
*/
#ifdef NEVER_MOVE_GRAVITY_APPLY
void CX2GameUnit::CreateAndInsertNeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_, const bool vGravity_ )
#else
void CX2GameUnit::CreateAndInsertNeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_ )
#endif //NEVER_MOVE_GRAVITY_APPLY	
{
#ifdef NEVER_MOVE_GRAVITY_APPLY
		CX2NeverMoveByBuffPtr ptrNeverMove = CX2NeverMoveByBuffPtr( new CX2NeverMoveByBuff( BuffIdentity_, vPos_, vGravity_ ) );
#else
		CX2NeverMoveByBuffPtr ptrNeverMove = CX2NeverMoveByBuffPtr( new CX2NeverMoveByBuff( BuffIdentity_, vPos_ ) );
#endif //NEVER_MOVE_GRAVITY_APPLY
	if ( NULL != ptrNeverMove )
		m_vecNeverMovePtr.push_back( ptrNeverMove );
}

/** @function : EraseNeverMoveByBuff
	@brief : 특정버프에 의한 현재 위치에 고정 효과 삭제
	@param : 특정버프를 구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseNeverMoveByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2NeverMoveByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2NeverMoveByBuffPtr>::iterator, CX2NeverMoveByBuffPtr>( m_vecNeverMovePtr, BuffIdentity_ );

	if ( m_vecNeverMovePtr.end() != vItr )
		m_vecNeverMovePtr.erase( vItr );
}

/** @function : CreateAndInsertRenderParamByBuff
	@brief : 버프에 의한 RenderParam 변경 적용
	@param : 어떤 버프/디버프에 의해서 생성된 효과인지에 대한 정보(BuffIdentity_), 적용될 BuffRenderParam 값
*/
void CX2GameUnit::CreateAndInsertRenderParamByBuff( const KBuffIdentity& BuffIdentity_, const StBuffRenderParam& stBuffRenderParam_ )
{
	CX2RenderParamByBuffPtr ptrBuffRenderParam = CX2RenderParamByBuffPtr( new CX2RenderParamByBuff( BuffIdentity_, stBuffRenderParam_ ) );
	if ( NULL != ptrBuffRenderParam )
		m_vecRenderParamByBuffPtr.push_back( ptrBuffRenderParam );
}

/** @function : EraseRenderParamByBuff
	@brief : 버프에 의한 RenderParam 변경 해제
	@param : 어떤 버프/디버프에 의한 효과가 삭제되야 하는지에 대한 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseRenderParamByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2RenderParamByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2RenderParamByBuffPtr>::iterator, CX2RenderParamByBuffPtr>( m_vecRenderParamByBuffPtr, BuffIdentity_ );

	if ( m_vecRenderParamByBuffPtr.end() != vItr )
		m_vecRenderParamByBuffPtr.erase( vItr );
}
/** @function : InsertAdditionalStat
	@brief : 행동 타입별 효과 추가
	@param : 행동타입(eBehaviorType_), 변경타입(eChangeType_), 어떤버프에 의한것인지구분(BuffIdentity_), 적용값(fValue_)
*/
void CX2GameUnit::InsertAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ )
{
	switch ( eBehaviorType_ )
	{
	case BBT_CHANGE_MAX_HP:					/// 최대HP 변경
		m_AdditionalMaxHp.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

		m_AdditionalMaxHp.CheckAndUpdateResultValue();

		GetGageData()->SetMaxHp( m_AdditionalMaxHp.GetResultValue() );
		break;

	case BBT_CHANGE_MAX_MP:					/// 최대MP 변경
		m_AdditionalMaxMp.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

		m_AdditionalMaxMp.CheckAndUpdateResultValue();

		GetGageData()->SetMaxMp( m_AdditionalMaxMp.GetResultValue() );
		break;

	case BBT_CHANGE_PHYSIC_ATTACK:			/// 물리공격력
		m_AdditionalPhysicAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
			
	case BBT_CHANGE_MAGIC_ATTACK:			/// 마법공격력
		m_AdditionalMagicAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
			
	case BBT_CHANGE_PHYSIC_DEFENCE:			/// 물리방어력
		m_AdditionalPhysicDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_MAGIC_DEFENCE:			/// 마법방어력
		m_AdditionalMagicDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_ANIM_SPEED:				/// 동작속도
		m_AdditionalAnimSpeed.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_WALK_RUN_SPEED:			/// 이동속도
		m_AdditionalWalkSpeed.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		m_AdditionalDashSpeed.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
			
	case BBT_CHANGE_JUMP_POWER:				/// 점프력
		m_AdditionalJumpPower.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		m_AdditionalDashJumpPower.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_ACCURACY:				/// 명중
		m_AdditionalAccuracy.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_AVOIDANCE:					///	회피
		m_AdditionalAvoidance.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_CRITICAL_RATE:			/// 크리티컬공격확률
		m_AdditionalCriticalRate.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_CRITICAL_ATTACK:			/// 크리티컬공격력(크리티컬 시 데미지 관련)
		m_AdditionalCriticalAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_ANTI_CRITICAL_RATE:		/// 크리티컬방어확률
		m_AdditionalAntiCriticalRate.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_CRITICAL_DEFENCE:		/// 크리티컬방어력(크리티컬 시 데미지 관련)
		m_AdditionalCriticalDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_ADDITIONAL_ATTACK:		/// 추가데미지
		m_AdditionalChangeAdditionalAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
			
	case BBT_CHANGE_ADDITIONAL_DEFENCE:		/// 데미지감소
		m_AdditionalChangeAdditionalDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHARGE_MP_HIT_MELEE_ATTACK:					/// 근접 타격 시 MP 회복량
		m_AdditionalChargeMpHitMeleeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
		
	case BBT_CHARGE_MP_HIT_RANGE_ATTACK:					/// 원거리 타격 시 MP 회복량
		m_AdditionalChargeMpHitRangeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHARGE_MP_HITTED:				/// 피격 시 MP 회복량
		m_AdditionalChargeMpHitted.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK:		/// 근접공격 주는 데미지 변경
		m_AdditionalFinalDamageMultiplierMeleeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK:		/// 원거리(DAMAGE_EFFECT)공격 주는 데미지 변경
		m_AdditionalFinalDamageMultiplierRangeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER:		/// 받는 데미지 변경
		m_AdditionalFinalDefenceMultiplier.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK:		/// 근접공격 FORCE_DOWN 수치 변경
		m_AddtionalForceDownMultiplierMeleeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK:		/// 원거리(DAMAGE_EFFECT)공격 FORCE_DOWN 수치 변경
		m_AddtionalForceDownMultiplierRangeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE:
		m_AdditionalChangeAttributeBlazeDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE:
		m_AdditionalChangeAttributeWaterDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE:
		m_AdditionalChangeAttributeNatureDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE:
		m_AdditionalChangeAttributeWindDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE:
		m_AdditionalChangeAttributeLightDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE:
		m_AdditionalChangeAttributeDarkDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

	case BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE:
		{
			m_AdditionalChangeAttributeBlazeDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
			m_AdditionalChangeAttributeWaterDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
			m_AdditionalChangeAttributeNatureDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
			m_AdditionalChangeAttributeWindDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
			m_AdditionalChangeAttributeLightDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
			m_AdditionalChangeAttributeDarkDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_DRAIN_HP_NORMAL_ATTACK:
		m_AdditionalDrainHpNormalAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case BBT_CHANGE_HYPER_CHARGE_SPEED:			/// 각성 충전 속도
		m_AdditionalHyperCharge.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		break;
#endif //SERV_ADD_LUNATIC_PSYKER

	default:
		DISPLAY_ERROR( L"Behavior Type Invalid" );
		break;
	}
}

/** @function : UpdateAdditionalStat
	@brief : 행동 타입별 효과 갱신
	@param : 행동타입(eBehaviorType_), 변경타입(eChangeType_), 어떤버프에 의한것인지구분(BuffIdentity_), 적용값(fValue_)
*/
void CX2GameUnit::UpdateAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ )
{
	switch ( eBehaviorType_ )
	{
	case BBT_CHANGE_MAX_HP:					/// 최대HP 변경
		{
			if ( !m_AdditionalMaxHp.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalMaxHp.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_MAX_MP:					/// 최대MP 변경
		{
			if ( !m_AdditionalMaxMp.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalMaxMp.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_PHYSIC_ATTACK:			/// 물리공격력
		{
			if ( !m_AdditionalPhysicAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalPhysicAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_MAGIC_ATTACK:			/// 마법공격력
		{
			if ( !m_AdditionalMagicAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalMagicAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_PHYSIC_DEFENCE:			/// 물리방어력
		{
			if ( !m_AdditionalPhysicDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalPhysicDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_MAGIC_DEFENCE:			/// 마법방어력
		{
			if ( !m_AdditionalMagicDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalMagicDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_ANIM_SPEED:				/// 동작속도
		{
			if ( !m_AdditionalAnimSpeed.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalAnimSpeed.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_WALK_RUN_SPEED:			/// 이동속도
		{
			if ( !m_AdditionalWalkSpeed.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalWalkSpeed.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

			if ( !m_AdditionalDashSpeed.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalDashSpeed.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_JUMP_POWER:				/// 점프력
		{
			if ( !m_AdditionalJumpPower.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalJumpPower.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

			if ( !m_AdditionalDashJumpPower.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalDashJumpPower.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_ACCURACY:				/// 명중
		{
			if ( !m_AdditionalAccuracy.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalAccuracy.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_AVOIDANCE:					///	회피
		{
			if ( !m_AdditionalAvoidance.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalAvoidance.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_CRITICAL_RATE:			/// 크리티컬공격확률
		{
			if ( !m_AdditionalCriticalRate.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalCriticalRate.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_CRITICAL_ATTACK:			/// 크리티컬공격력(크리티컬 시 데미지 관련)
		{
			if ( !m_AdditionalCriticalAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalCriticalAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		}
		break;

	case BBT_CHANGE_ANTI_CRITICAL_RATE:		/// 크리티컬방어확률
		{
			if ( !m_AdditionalAntiCriticalRate.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
			m_AdditionalAntiCriticalRate.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_CRITICAL_DEFENCE:		/// 크리티컬방어력(크리티컬 시 데미지 관련)
		{
			if ( !m_AdditionalCriticalDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalCriticalDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_ADDITIONAL_ATTACK:		/// 추가데미지
		{
			if ( !m_AdditionalChangeAdditionalAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAdditionalAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_ADDITIONAL_DEFENCE:		/// 데미지감소
		{
			if ( !m_AdditionalChangeAdditionalDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAdditionalDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHARGE_MP_HIT_MELEE_ATTACK:					/// 근접 타격 시 MP 회복량
		{
			if ( !m_AdditionalChargeMpHitMeleeAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChargeMpHitMeleeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHARGE_MP_HIT_RANGE_ATTACK:					/// 원거리 타격 시 MP 회복량
		{
			if ( !m_AdditionalChargeMpHitRangeAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChargeMpHitRangeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHARGE_MP_HITTED:				/// 피격 시 MP 회복량
		{
			if ( !m_AdditionalChargeMpHitted.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChargeMpHitted.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;
	
	case BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK:		/// 근접공격 주는 데미지 변경
		{
			if ( !m_AdditionalFinalDamageMultiplierMeleeAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalFinalDamageMultiplierMeleeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK:		/// 원거리(DAMAGE_EFFECT)공격 주는 데미지 변경
		{
			if ( !m_AdditionalFinalDamageMultiplierRangeAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalFinalDamageMultiplierRangeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER:		/// 받는 데미지 변경
		{
			if ( !m_AdditionalFinalDefenceMultiplier.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalFinalDefenceMultiplier.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK:		/// 근접공격 FORCE_DOWN 수치 변경
		{
			if ( !m_AddtionalForceDownMultiplierMeleeAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AddtionalForceDownMultiplierMeleeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK:		/// 원거리(DAMAGE_EFFECT)공격 FORCE_DOWN 수치 변경
		{
			if ( !m_AddtionalForceDownMultiplierRangeAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AddtionalForceDownMultiplierRangeAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE:		/// 속성저항
		{
			if ( !m_AdditionalChangeAttributeBlazeDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeBlazeDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;
	case BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE:		/// 속성저항
		{
			if ( !m_AdditionalChangeAttributeWaterDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeWaterDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;
	case BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE:		/// 속성저항
		{
			if ( !m_AdditionalChangeAttributeNatureDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeNatureDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;
	case BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE:		/// 속성저항
		{
			if ( !m_AdditionalChangeAttributeWindDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeWindDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;
	case BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE:		/// 속성저항
		{
			if ( !m_AdditionalChangeAttributeLightDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeLightDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;
	case BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE:		/// 속성저항
		{
			if ( !m_AdditionalChangeAttributeDarkDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeDarkDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE:		/// 모든 속성저항
		{
			if ( !m_AdditionalChangeAttributeBlazeDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeBlazeDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

			if ( !m_AdditionalChangeAttributeWaterDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeWaterDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

			if ( !m_AdditionalChangeAttributeNatureDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeNatureDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

			if ( !m_AdditionalChangeAttributeWindDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeWindDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

			if ( !m_AdditionalChangeAttributeLightDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeLightDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );

			if ( !m_AdditionalChangeAttributeDarkDefence.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalChangeAttributeDarkDefence.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

	case BBT_DRAIN_HP_NORMAL_ATTACK:		/// 피흡
		{
			if ( !m_AdditionalDrainHpNormalAttack.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalDrainHpNormalAttack.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
		} break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case BBT_CHANGE_HYPER_CHARGE_SPEED:		/// 각성 충전 속도
		{
			if ( !m_AdditionalHyperCharge.UpdateAddtionalStatFactor( eChangeType_, BuffIdentity_, fValue_ ) )
				m_AdditionalHyperCharge.CreateAndInsertStatFactor( eChangeType_, BuffIdentity_, fValue_ );
			break;
		}
#endif //SERV_ADD_LUNATIC_PSYKER

	default:
		DISPLAY_ERROR( L"Behavior Type Invalid" );
		break;
	}
}

/** @function : EraseAdditionalStat
	@brief : 행동 타입별 효과 삭제
	@param : 행동타입(eBehaviorType_), 변경타입(eChangeType_), 어떤버프에 의한것인지구분(BuffIdentity_)
*/
void CX2GameUnit::EraseAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_ )
{
	switch ( eBehaviorType_ )
	{
	case BBT_CHANGE_MAX_HP:					/// 최대HP 변경
		m_AdditionalMaxHp.EraseStatFactor( eChangeType_, BuffIdentity_ );

		m_AdditionalMaxHp.CheckAndUpdateResultValue();

		GetGageData()->SetMaxHp( m_AdditionalMaxHp.GetResultValue() );
		break;

	case BBT_CHANGE_MAX_MP:					/// 최대MP 변경
		m_AdditionalMaxMp.EraseStatFactor( eChangeType_, BuffIdentity_ );

		m_AdditionalMaxMp.CheckAndUpdateResultValue();

		GetGageData()->SetMaxMp( m_AdditionalMaxMp.GetResultValue() );
#ifdef ADD_BUFF_FINALIZER_FINISH_HYPER
		// 버프가 해제될 때 GageManager의 m_pGageData가 GameUnit의 m_pGageData로 갱신되지 않는 문제 수정
		if( true == IsMyUnit() )
		{
			if ( NULL != CX2GageManager::GetInstance())
				CX2GageManager::GetInstance()->GetMyGageData()->UpdateDataFromGameUnit( m_pGageData );
		}
#endif
		break;

	case BBT_CHANGE_PHYSIC_ATTACK:			/// 물리공격력
		m_AdditionalPhysicAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_MAGIC_ATTACK:			/// 마법공격력
		m_AdditionalMagicAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_PHYSIC_DEFENCE:			/// 물리방어력
		m_AdditionalPhysicDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_MAGIC_DEFENCE:			/// 마법방어력
		m_AdditionalMagicDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_ANIM_SPEED:				/// 동작속도
		m_AdditionalAnimSpeed.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_WALK_RUN_SPEED:			/// 이동속도
		m_AdditionalWalkSpeed.EraseStatFactor( eChangeType_, BuffIdentity_ );
		m_AdditionalDashSpeed.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_JUMP_POWER:				/// 점프력
		m_AdditionalJumpPower.EraseStatFactor( eChangeType_, BuffIdentity_ );
		m_AdditionalDashJumpPower.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_ACCURACY:				/// 명중
		m_AdditionalAccuracy.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_AVOIDANCE:					///	회피
		m_AdditionalAvoidance.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_CRITICAL_RATE:			/// 크리티컬공격확률
		m_AdditionalCriticalRate.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_CRITICAL_ATTACK:			/// 크리티컬공격력(크리티컬 시 데미지 관련)
		m_AdditionalCriticalAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_ANTI_CRITICAL_RATE:		/// 크리티컬방어확률
		m_AdditionalAntiCriticalRate.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_CRITICAL_DEFENCE:		/// 크리티컬방어력(크리티컬 시 데미지 관련)
		m_AdditionalCriticalDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_ADDITIONAL_ATTACK:		/// 추가데미지
		m_AdditionalChangeAdditionalAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_ADDITIONAL_DEFENCE:		/// 데미지감소
		m_AdditionalChangeAdditionalDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;
	
	case BBT_CHARGE_MP_HIT_MELEE_ATTACK:					/// 근접 타격 시 MP 회복량
		m_AdditionalChargeMpHitMeleeAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHARGE_MP_HIT_RANGE_ATTACK:					/// 원거리 타격 시 MP 회복량
		m_AdditionalChargeMpHitRangeAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHARGE_MP_HITTED:				/// 피격 시 MP 회복량
		m_AdditionalChargeMpHitted.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK:		/// 근접공격 주는 데미지 변경
		m_AdditionalFinalDamageMultiplierMeleeAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK:		/// 원거리(DAMAGE_EFFECT)공격 주는 데미지 변경
		m_AdditionalFinalDamageMultiplierRangeAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER:		/// 받는 데미지 변경
		m_AdditionalFinalDefenceMultiplier.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK:		/// 근접공격 FORCE_DOWN 수치 변경
		m_AddtionalForceDownMultiplierMeleeAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK:		/// 원거리(DAMAGE_EFFECT)공격 FORCE_DOWN 수치 변경
		m_AddtionalForceDownMultiplierRangeAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE:		/// 속성저항
		m_AdditionalChangeAttributeBlazeDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE:		/// 속성저항
		m_AdditionalChangeAttributeWaterDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE:		/// 속성저항
		m_AdditionalChangeAttributeNatureDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE:		/// 속성저항
		m_AdditionalChangeAttributeWindDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE:		/// 속성저항
		m_AdditionalChangeAttributeLightDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;
	case BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE:		/// 속성저항
		m_AdditionalChangeAttributeDarkDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

	case BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE:		/// 모든 속성저항
		{
			m_AdditionalChangeAttributeBlazeDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
			m_AdditionalChangeAttributeWaterDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
			m_AdditionalChangeAttributeNatureDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
			m_AdditionalChangeAttributeWindDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
			m_AdditionalChangeAttributeLightDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
			m_AdditionalChangeAttributeDarkDefence.EraseStatFactor( eChangeType_, BuffIdentity_ );
		} break;

	case BBT_DRAIN_HP_NORMAL_ATTACK:		/// 피흡
		m_AdditionalDrainHpNormalAttack.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case BBT_CHANGE_HYPER_CHARGE_SPEED:		/// 각성 충전 속도
		m_AdditionalHyperCharge.EraseStatFactor( eChangeType_, BuffIdentity_ );
		break;
#endif //SERV_ADD_LUNATIC_PSYKER

	default:
		DISPLAY_ERROR( L"Behavior Type Invalid" );
		break;
	}
}

/** @function : SetGameStat
	@brief : 스탯 적용시 버프관련 스탯을 갱신하도록 하는 함수
	@param : 서버로 부터 받은 KStat(kGameStat_)
*/
/*virtual*/ void CX2GameUnit::SetGameStat( const KStat& kGameStat_ )
{
	CX2Stat::Stat& kStat = m_Stat.AccessStat();
	kStat.SetKStat( kGameStat_ );

	m_AdditionalMaxHp.SetBasicStatValue( kStat.m_fBaseHP );
	m_AdditionalPhysicAttack.SetBasicStatValue( kStat.m_fAtkPhysic );
	m_AdditionalMagicAttack.SetBasicStatValue( kStat.m_fAtkMagic );
	m_AdditionalPhysicDefence.SetBasicStatValue( kStat.m_fDefPhysic );
	m_AdditionalMagicDefence.SetBasicStatValue( kStat.m_fDefMagic );	
}

/** @function : SetGameStat
	@brief : 스탯 적용시 버프관련 스탯을 갱신하도록 하는 함수
	@param : 클라이언트에서 사용하는 스탯구조체(Stat_)
*/
/*virtual*/ void CX2GameUnit::SetGameStat( const CX2Stat::Stat& Stat_ )
{
	CX2Stat::Stat& kStat = m_Stat.AccessStat();
    if ( &kStat != &Stat_ )
    {
	    kStat = Stat_;
    }

	m_AdditionalMaxHp.SetBasicStatValue( kStat.m_fBaseHP );
	m_AdditionalPhysicAttack.SetBasicStatValue( kStat.m_fAtkPhysic );
	m_AdditionalMagicAttack.SetBasicStatValue( kStat.m_fAtkMagic );
	m_AdditionalPhysicDefence.SetBasicStatValue( kStat.m_fDefPhysic );
	m_AdditionalMagicDefence.SetBasicStatValue( kStat.m_fDefMagic );	
}

/** @function : CheckAndUpdateAdditionalStat
	@brief : 버프효과 또는 장비교체 등 값이 갱신되면 새로 적용하는 함수
*/
void CX2GameUnit::CheckAndUpdateAdditionalStat()
{
	m_AdditionalMaxHp.CheckAndUpdateResultValue();
	m_AdditionalMaxMp.CheckAndUpdateResultValue();

	m_AdditionalPhysicAttack.CheckAndUpdateResultValue();
	m_AdditionalMagicAttack.CheckAndUpdateResultValue();
	m_AdditionalPhysicDefence.CheckAndUpdateResultValue();
	m_AdditionalMagicDefence.CheckAndUpdateResultValue();

	m_AdditionalAnimSpeed.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalAnimationSpeedPercent, GetUnitLevel() );
	
	m_AdditionalWalkSpeed.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalMoveSpeedPercent, GetUnitLevel(), 0.5f );
	m_AdditionalDashSpeed.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalMoveSpeedPercent, GetUnitLevel(), 0.5f );

	m_AdditionalJumpPower.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalJumpSpeedPercent, GetUnitLevel(), 0.5f );
	m_AdditionalDashJumpPower.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalJumpSpeedPercent, GetUnitLevel(), 0.5f );

	m_AdditionalAccuracy.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalAnitEvadePercent, GetUnitLevel() );
	m_AdditionalAvoidance.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalEvadePercent, GetUnitLevel() );
	m_AdditionalCriticalRate.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalCriticalPercent, GetUnitLevel() );
	
	m_AdditionalCriticalAttack.CheckAndUpdateResultValue();
	m_AdditionalAntiCriticalRate.CheckAndUpdateResultValue();
	m_AdditionalCriticalDefence.CheckAndUpdateResultValue();

	if ( NULL != g_pX2Game  )	/// 일단은 이렇게 처리 하자.. /// kimhc
	{
#ifdef ADJUST_BUFF_CALCULATE
		//공식이 수정됨에 따라 보정던전만 따로 공식을 처리한 것이 필요 없어짐.
		m_AdditionalChangeAdditionalAttack.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalAdditionalAttackValue, GetUnitLevel() );
#else //ADJUST_BUFF_CALCULATE
		if ( g_pX2Game->IsDamageFreeGame() )	/// 보정 던전의 경우
			m_AdditionalChangeAdditionalAttack.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalAdditionalAttackValueInDamageFreeGame, GetUnitLevel() );
		else
			m_AdditionalChangeAdditionalAttack.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalAdditionalAttackValue, GetUnitLevel() );
#endif //ADJUST_BUFF_CALCULATE
	}

	m_AdditionalChangeAdditionalDefence.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalAdditionalDefencePercent, GetUnitLevel() );

	m_AdditionalChargeMpHitMeleeAttack.CheckAndUpdateResultValue();
	m_AdditionalChargeMpHitRangeAttack.CheckAndUpdateResultValue();

	m_AdditionalChargeMpHitted.CheckAndUpdateResultValue();

	m_AdditionalFinalDamageMultiplierMeleeAttack.CheckAndUpdateResultValue();
	m_AdditionalFinalDamageMultiplierRangeAttack.CheckAndUpdateResultValue();

	m_AdditionalFinalDefenceMultiplier.CheckAndUpdateResultValue();

	m_AddtionalForceDownMultiplierMeleeAttack.CheckAndUpdateResultValue();
	m_AddtionalForceDownMultiplierRangeAttack.CheckAndUpdateResultValue();

	m_AdditionalChangeAttributeBlazeDefence.CheckAndUpdateResultValue();
	m_AdditionalChangeAttributeWaterDefence.CheckAndUpdateResultValue();
	m_AdditionalChangeAttributeNatureDefence.CheckAndUpdateResultValue();
	m_AdditionalChangeAttributeWindDefence.CheckAndUpdateResultValue();
	m_AdditionalChangeAttributeLightDefence.CheckAndUpdateResultValue();
	m_AdditionalChangeAttributeDarkDefence.CheckAndUpdateResultValue();

	m_AdditionalDrainHpNormalAttack.CheckAndUpdateResultValue();

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	m_AdditionalHyperCharge.CheckAndUpdateResultValueWithOptionData( &CX2SocketItem::GetFinalHyperGageChargeSpeedPercent, GetUnitLevel() );
#endif //SERV_ADD_LUNATIC_PSYKER
}

/** @function : CreateAndInsertTemporaryBuffFactor
	@brief : 버프에 의해 일시적으로 발생하는 BuffFactor를 만드는 함수
	@param : 참고할 버프팩터(BuffFactor_), 어떤 버프에 의해 생긴것인지 구분(BuffIdentity_)
*/
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
void CX2GameUnit::CreateAndInsertTemporaryBuffFactor( const CX2BuffFactor& BuffFactor_, const KBuffIdentity& BuffIdentity_, UINT uiLevel )
#else //UPGRADE_SKILL_SYSTEM_2013
void CX2GameUnit::CreateAndInsertTemporaryBuffFactor( const CX2BuffFactor& BuffFactor_, const KBuffIdentity& BuffIdentity_ )
#endif //UPGRADE_SKILL_SYSTEM_2013
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	CX2BuffFactorPtr ptrBuffFactor = BuffFactor_.GetClonePtr( uiLevel );
#else //UPGRADE_SKILL_SYSTEM_2013
	CX2BuffFactorPtr ptrBuffFactor = BuffFactor_.GetClonePtr();
#endif //UPGRADE_SKILL_SYSTEM_2013
	if ( NULL != ptrBuffFactor )
	{
		ptrBuffFactor->SetBuffIdentity( BuffIdentity_ );
		m_vecTemporaryBuffFactorPtr.push_back( ptrBuffFactor );
	}
}

/** @function : EraseTemporaryBuffFactor
	@brief : 버프가 끝났을 대 일시적으로 발생한 BuffFactor를 삭제하는 함수
	@param : 어떤 버프가 끝났는지(BuffIdentity_)
*/
void CX2GameUnit::EraseTemporaryBuffFactor( const KBuffIdentity& BuffIdentity_ )
{
	if ( !m_vecTemporaryBuffFactorPtr.empty() )
	{
		auto FindFunc = [&BuffIdentity_]( CX2BuffFactorPtr ptrBuffFactor_ ) {
			return ptrBuffFactor_->GetBuffIdentity() == BuffIdentity_;
		};

		vector<CX2BuffFactorPtr>::iterator vItr
			= std::find_if( m_vecTemporaryBuffFactorPtr.begin(), m_vecTemporaryBuffFactorPtr.end(), FindFunc );
		if ( m_vecTemporaryBuffFactorPtr.end() != vItr )
		{
			m_vecTemporaryBuffFactorPtr.erase( vItr );
		}
	}
}

/** @function : CreateAndInsertAbsorbEffectAttack
	@brief : 이펙트 공격을 흡수하여 MP를 증가시키는 버프요소를 게임유닛에 적용하는 함수
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_), 흡수시 증가되는 MP량(fAbsorbValue_)
*/
void CX2GameUnit::CreateAndInsertAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_, const float fAbsorbValue_ )
{
	CX2StatFactorByBuffPtr ptrAbsorbEffectAttack = CX2StatFactorByBuffPtr( new CX2StatFactorByBuff( BuffIdentity_, fAbsorbValue_ ) );
	if ( NULL != ptrAbsorbEffectAttack )
		m_vecAbsorbEffectAttackPtr.push_back( ptrAbsorbEffectAttack );
}

/** @function : UpdateAbsorbEffectAttack
	@brief : 흡수시 증가되는 MP량 변경
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_), 흡수시 증가되는 MP량(fAbsorbValue_)
*/
void CX2GameUnit::UpdateAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_, const float fAbsorbValue_ )
{
	
	vector<CX2StatFactorByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2StatFactorByBuffPtr>::iterator, CX2StatFactorByBuffPtr>( m_vecAbsorbEffectAttackPtr, BuffIdentity_ );

	if ( m_vecAbsorbEffectAttackPtr.end() != vItr && NULL != *vItr )
		(*vItr)->SetValue( fAbsorbValue_ );
		
}

/** @function : EraseAbsorbEffectAttack
	@brief : 게임유닛에 적용된 이펙트 흡수 기능 삭제
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_)
*/
void CX2GameUnit::EraseAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2StatFactorByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2StatFactorByBuffPtr>::iterator, CX2StatFactorByBuffPtr>( m_vecAbsorbEffectAttackPtr, BuffIdentity_ );

	if ( m_vecAbsorbEffectAttackPtr.end() != vItr )
		m_vecAbsorbEffectAttackPtr.erase( vItr );
}

/** @function : AbsorbEffectAttackProcess
	@brief : 이펙트에 의해 공격받았을 때 흡수하여 MP를 증가시키는 처리 부분
	@param : 데미지데이타 포인터(pDamageData_)
	@return : 흡수성공(true), 흡수안함(false) 흡수 성공하면 유닛의 DamageReact 이후 부분 실행 안함
*/
bool CX2GameUnit::AbsorbEffectAttackProcess( CX2DamageManager::DamageData* pDamageData_ )
{
	if( !m_vecAbsorbEffectAttackPtr.empty() && pDamageData_->bCanReflexMagic == true )
	{
		/// 흡수 할 때 마다 합을 구하도록 구현했으나 현재의 방식이 좋을지, 
		/// 합산한 값을 가지고 있는 방식이 좋을지 생각해 보자
		float fSumOfAbsorbValue = 0.0f;
		auto FuncPlus = [&fSumOfAbsorbValue]( CX2StatFactorByBuffPtr ptr_ ) {
			fSumOfAbsorbValue += ptr_->GetValue();
		};
	
		for_each( m_vecAbsorbEffectAttackPtr.begin(), m_vecAbsorbEffectAttackPtr.end(), FuncPlus );
	
		// 마법흡수
		UpNowMp( fSumOfAbsorbValue );

		WCHAR wstr[10];
		ZeroMemory( wstr, ARRAY_SIZE( wstr ) );
		wsprintf( wstr, L"MP+ %d", static_cast<int>( fSumOfAbsorbValue ) );
		
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f;
		if( NULL != g_pData->GetPicCharBlue() )
			g_pData->GetPicCharBlue()->DrawText( wstr, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );

		pDamageData_->reActResult = CX2DamageManager::RT_DUMMY_DAMAGE;
		pDamageData_->m_bNoDamage = true;
		return true;
	}	
	else
		return false;
}

/** @function : CreateAndInsertChangeAttackByType
	@brief : 공격 타입에 따른 데미지 변경 버프요소를 게임유닛에 적용하는 함수
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_), 흡수시 증가되는 MP량(fAbsorbValue_)
*/
void CX2GameUnit::CreateAndInsertChangeAttackByType( const KBuffIdentity& BuffIdentity_, CX2DamageManager::ATTACK_TYPE eAttackType_, float fValue_ )
{
	CX2ChangeAttackByTypePtr ptrChangeAttackByType = CX2ChangeAttackByTypePtr( new CX2ChangeAttackByType( BuffIdentity_, eAttackType_, fValue_ ) );
	if ( NULL != ptrChangeAttackByType )
		m_AdditionalAttackByType.push_back( ptrChangeAttackByType );
}

/** @function : EraseChangeAttackByType
	@brief : 공격 타입에 따른 데미지 변경 버프 요소 해제
	@param : 공격 타입에 따른 데미지 변경 버프 요소를구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseChangeAttackByType( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2ChangeAttackByTypePtr>::iterator vItr 
	= FindSameBuffIdentity<vector<CX2ChangeAttackByTypePtr>::iterator, CX2ChangeAttackByTypePtr>( m_AdditionalAttackByType, BuffIdentity_ );

	if ( m_AdditionalAttackByType.end() != vItr )
		m_AdditionalAttackByType.erase( vItr );
}

/** @function : GetAdditionalAttackByType
	@brief : 공격 타입에 따른 데미지 증가치 반환 함수
	@return : 데미지 증가치
*/
float CX2GameUnit::GetAdditionalAttackByType( CX2DamageManager::ATTACK_TYPE eAttackType_ )
{
	float fPowerRate = 1.f;

	BOOST_FOREACH( CX2ChangeAttackByTypePtr ChangeAttackByTypePtr, m_AdditionalAttackByType )	/// 찾고자 하는 공격 타입에 해당하는 배율 검사
	{
		if( eAttackType_ ==  ChangeAttackByTypePtr->GetAttackType() )
			fPowerRate *= ChangeAttackByTypePtr->GetValue();
	}

	if( fPowerRate < 1.f )
		fPowerRate = 1.f;

	return fPowerRate;
}

/** @function : CreateAndInsertChangeEnchantAttackRate
	@brief : 소켓 속성 공격 발동 확률 변경 버프요소를 게임유닛에 적용하는 함수
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_), 흡수시 증가되는 MP량(fAbsorbValue_)
*/
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
// 인챈트 속성 타입과 변경 타입 추가
void CX2GameUnit::CreateAndInsertChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_, 
	BUFF_ENCHANT_ATTRIBUTE_TYPE eEnchantAttributeType_ /* = BEAT_ALL */, BUFF_CHANGE_TYPE eBuffChangeType_ /* = BCT_PERCENT */ )
{
	CX2ChangeEnchantAttackRatePtr ptrChangeEnchantAttackRate = CX2ChangeEnchantAttackRatePtr( new CX2ChangeEnchantAttackRate( BuffIdentity_, fRate_, eEnchantAttributeType_, eBuffChangeType_ ) );
	if ( NULL != ptrChangeEnchantAttackRate )
		m_AdditionalChangeEnchantAttackRate.push_back( ptrChangeEnchantAttackRate );
}
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
void CX2GameUnit::CreateAndInsertChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_ )
{

	CX2ChangeEnchantAttackRatePtr ptrChangeEnchantAttackRate = CX2ChangeEnchantAttackRatePtr( new CX2ChangeEnchantAttackRate( BuffIdentity_, fRate_ ) );
	if ( NULL != ptrChangeEnchantAttackRate )
		m_AdditionalChangeEnchantAttackRate.push_back( ptrChangeEnchantAttackRate );
}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

/** @function : EraseChangeEnchantAttackRate
	@brief : 소켓 속성 공격 발동 확률 변경 버프 요소 해제
	@param : 소켓 속성 공격 발동 확률 변경 버프 요소를구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2ChangeEnchantAttackRatePtr>::iterator vItr 
	= FindSameBuffIdentity<vector<CX2ChangeEnchantAttackRatePtr>::iterator, CX2ChangeEnchantAttackRatePtr>( m_AdditionalChangeEnchantAttackRate, BuffIdentity_ );

	if ( m_AdditionalChangeEnchantAttackRate.end() != vItr )
		m_AdditionalChangeEnchantAttackRate.erase( vItr );
}

/** @function : GetAdditionalAttackByType
	@brief : 소켓 속성 공격 발동 확률 반환 함수
	@return : 증가할 확률
*/
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
// 무기 강화 속성 전달해주는 인자 추가 및 반환값 추가
CX2GameUnit::EnchantAttackRate CX2GameUnit::GetChangeEnchantAttackRate( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType )
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
float CX2GameUnit::GetChangeEnchantAttackRate()
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
{
	float fIncreaseRate = 1.f;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 인챈트 공격 확률이 기존 Percent 에서 고정값이 추가됨에 따라
	// 연산 과정이 추가됨 ( 속성 공격 확률 = 무기 기본 확률 * 추가 버프 Percent -> 속성 공격 확률 = ( 무기 기본 확률 * 추가 버프 Percent ) + 추가 버프 Fix_Value
	// 아래 변수들은 추가 버프 FixValue 를 위해 사용됨

	float fIncraseAttackRateFixValue = 0.f;			// 버프 체인지 타입이 FixValue 일 때 사용됨
	EnchantAttackRate sEnchantAttackRate;			// 인챈트 공격 확률 클래스, 반환값에 쓰임
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	BOOST_FOREACH( CX2ChangeEnchantAttackRatePtr ChangeEnchantAttackRatePtr, m_AdditionalChangeEnchantAttackRate )	/// 찾고자 하는 공격 타입에 해당하는 배율 검사
	{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 속성 공격 타입이 존재한다면, 해당 속성의 공격만 강화시켜준다.
		// 디폴트는 BEAT_ALL
		BUFF_ENCHANT_ATTRIBUTE_TYPE eBuffEnchantAttributeType = ChangeEnchantAttackRatePtr->GetBuffEnchantAttributeType();

		if (
			( eBuffEnchantAttributeType == BEAT_ALL ) ||
			( eBuffEnchantAttributeType == BEAT_BLAZE && eExtraDamageType == CX2DamageManager::EDT_ENCHANT_BLAZE		) ||
			( eBuffEnchantAttributeType == BEAT_FROZEN && eExtraDamageType == CX2DamageManager::EDT_ENCHANT_FROZEN		) ||	
			( eBuffEnchantAttributeType == BEAT_PIERCING && eExtraDamageType == CX2DamageManager::EDT_ENCHANT_PIERCING	) ||
			( eBuffEnchantAttributeType == BEAT_POSION && eExtraDamageType == CX2DamageManager::EDT_ENCHANT_POISON		) ||
			( eBuffEnchantAttributeType == BEAT_SHOCK && eExtraDamageType == CX2DamageManager::EDT_ENCHANT_SHOCK		) ||
			( eBuffEnchantAttributeType == BEAT_SNATCH && eExtraDamageType == CX2DamageManager::EDT_ENCHANT_SNATCH		) 
			)
		{
			// 버프 체인지 타입에 따라서 다른 연산을 해준다.
			sEnchantAttackRate.eBuffchangeType = ChangeEnchantAttackRatePtr->GetBuffChangeType ();
			if ( BCT_PERCENT == ChangeEnchantAttackRatePtr->GetBuffChangeType () )
				fIncreaseRate *= ChangeEnchantAttackRatePtr->GetRate();
			else if ( BCT_FIX_VALUE == ChangeEnchantAttackRatePtr->GetBuffChangeType () )
				fIncraseAttackRateFixValue += ChangeEnchantAttackRatePtr->GetRate();
		}
	}
	
	sEnchantAttackRate.fIncreaseAttackRate = ( fIncreaseRate  == 0.f ? 1.f : fIncreaseRate );
	sEnchantAttackRate.fIncreaseAttackRateFixValue = fIncraseAttackRateFixValue;
	return sEnchantAttackRate;

#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		fIncreaseRate *= ChangeEnchantAttackRatePtr->GetRate();
	}


	return fIncreaseRate  == 0.f ? 1.f : fIncreaseRate;			/// 반환값이 0이면 1로 수정
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
}

/** @function : CreateAndInsertChangeEnchantAttackRate
	@brief : 더블 어택 버프요소를 게임유닛에 적용하는 함수
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_), 흡수시 증가되는 MP량(fAbsorbValue_)
*/
void CX2GameUnit::CreateAndInsertDoubleAttackByBuff( const KBuffIdentity& BuffIdentity_, float fRate_ )
{
	CX2DoubleAttackPtr ptrDoubleAttack = CX2DoubleAttackPtr( new CX2DoubleAttack( BuffIdentity_, fRate_ ) );
	if ( NULL != ptrDoubleAttack )
		m_vecDoubleAttack.push_back( ptrDoubleAttack );
}

/** @function : EraseChangeEnchantAttackRate
	@brief : 더블 어택 버프 요소 해제
	@param : 더블 어택 버프 요소를구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseDoubleAttackByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2DoubleAttackPtr>::iterator vItr 
	= FindSameBuffIdentity<vector<CX2DoubleAttackPtr>::iterator, CX2DoubleAttackPtr>( m_vecDoubleAttack, BuffIdentity_ );

	if ( m_vecDoubleAttack.end() != vItr )
		m_vecDoubleAttack.erase( vItr );
}

/** @function : IsDoubleAttackByBuff
	@brief : 더블 어택 발동 확률에 따른 성공 여부 반환
	@param : 더블 어택 버프 요소를구분할 수 있는 정보(BuffIdentity_)
*/
bool CX2GameUnit::IsDoubleAttackByBuff()
{
	BOOST_FOREACH( CX2DoubleAttackPtr ChangeEnchantAttackRatePtr, m_vecDoubleAttack )	/// 찾고자 하는 공격 타입에 해당하는 배율 검사
	{
		if( GetRandomFloat() < ChangeEnchantAttackRatePtr->GetRate() )
			return true;
	}

	return false;
}

/** @function : InitAdditionalStatOptionValue
	@brief : 소켓옵션에 의한 추가 스탯 부분 초기화
*/
void CX2GameUnit::InitAdditionalStatOptionValue()
{
	m_AdditionalAnimSpeed.SetOptionValue( m_SocketData.m_fAnimationSpeedRate );
	
	m_AdditionalWalkSpeed.SetOptionValue( m_SocketData.m_fMoveSpeed );
	m_AdditionalDashSpeed.SetOptionValue( m_SocketData.m_fMoveSpeed );

	m_AdditionalJumpPower.SetOptionValue( m_SocketData.m_fJumpSpeed );
	m_AdditionalDashJumpPower.SetOptionValue( m_SocketData.m_fJumpSpeed );

	m_AdditionalAccuracy.SetOptionValue( m_SocketData.m_fAntiEvadePercent );
	m_AdditionalAvoidance.SetOptionValue( m_SocketData.m_fEvadePercent );
	m_AdditionalCriticalRate.SetOptionValue( m_SocketData.m_fPercentCritical );
	m_AdditionalCriticalAttack.SetOptionValue( m_SocketData.m_fCritical );

	m_AdditionalAntiCriticalRate.SetOptionValue( 0.0f );
	m_AdditionalCriticalDefence.SetOptionValue( 0.0f );

	m_AdditionalChangeAdditionalAttack.SetOptionValue( m_SocketData.m_fAdditionalAttackValue );
	m_AdditionalChangeAdditionalDefence.SetOptionValue( m_SocketData.m_fAdditionalDefenceValue );

	/// 옵션수치화를 사용하지 않는 것들은 OptionValue를 사용하지 않기 때문에
	m_AdditionalChangeAttributeBlazeDefence.SetOptionValue( 0.f );
	m_AdditionalChangeAttributeWaterDefence.SetOptionValue( 0.f );
	m_AdditionalChangeAttributeNatureDefence.SetOptionValue( 0.f );
	m_AdditionalChangeAttributeWindDefence.SetOptionValue( 0.f );
	m_AdditionalChangeAttributeLightDefence.SetOptionValue( 0.f );
	m_AdditionalChangeAttributeDarkDefence.SetOptionValue( 0.f );

	m_AdditionalDrainHpNormalAttack.SetOptionValue( 0.f );

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	m_AdditionalHyperCharge.SetOptionValue( m_SocketData.m_fHyperModeChargeRate );
#endif //SERV_ADD_LUNATIC_PSYKER

// 	m_AdditionalFinalDamageMultiplier.SetOptionValue( 0.0f );
// 	m_AdditionalFinalDefenceMultiplier.SetOptionValue( 0.0f );
// 
// 	m_AddtionalForceDownMultiplierMeleeAttack.SetOptionValue( 0.0f );
// 	m_AddtionalForceDownMultiplierRangeAttack.SetOptionValue( 0.0f );
}

/** @function : InitAdditionalBasicStatValue
	@brief : 기본 스탯값 초기화
*/
void CX2GameUnit::InitAdditionalBasicStatValue()
{
	m_AdditionalAnimSpeed.SetBasicStatValue( 1.0f );
	
	InitAddtionalDashAndJumpBasicStatValue();
	
	m_AdditionalAccuracy.SetBasicStatValue( 1.0f );
	m_AdditionalAvoidance.SetBasicStatValue( 1.0f );
	m_AdditionalCriticalRate.SetBasicStatValue( 0.0f );
	m_AdditionalCriticalAttack.SetBasicStatValue( 1.5f );	/// 기본 크리티컬 데미지 배율은 1.5

	m_AdditionalAntiCriticalRate.SetBasicStatValue( 0.0f );
	m_AdditionalCriticalDefence.SetBasicStatValue( 1.0f );

	m_AdditionalChangeAdditionalAttack.SetBasicStatValue( 0.0f );
	m_AdditionalChangeAdditionalDefence.SetBasicStatValue( 0.0f );

	m_AdditionalChargeMpHitMeleeAttack.SetBasicStatValue( 6.0f );
	m_AdditionalChargeMpHitRangeAttack.SetBasicStatValue( 6.0f );
	m_AdditionalChargeMpHitted.SetBasicStatValue( 3.0f );

	m_AdditionalFinalDamageMultiplierMeleeAttack.SetBasicStatValue( 1.0f );
	m_AdditionalFinalDamageMultiplierRangeAttack.SetBasicStatValue( 1.0f );

	m_AdditionalFinalDefenceMultiplier.SetBasicStatValue( 1.0f );

	m_AddtionalForceDownMultiplierMeleeAttack.SetBasicStatValue( 1.0f );
	m_AddtionalForceDownMultiplierRangeAttack.SetBasicStatValue( 1.0f );

	m_AdditionalChangeAttributeBlazeDefence.SetBasicStatValue( 0.0f );
	m_AdditionalChangeAttributeWaterDefence.SetBasicStatValue( 0.0f );
	m_AdditionalChangeAttributeNatureDefence.SetBasicStatValue( 0.0f );
	m_AdditionalChangeAttributeWindDefence.SetBasicStatValue( 0.0f );
	m_AdditionalChangeAttributeLightDefence.SetBasicStatValue( 0.0f );
	m_AdditionalChangeAttributeDarkDefence.SetBasicStatValue( 0.0f );

	m_AdditionalDrainHpNormalAttack.SetBasicStatValue( 0.f );

	m_AdditionalMaxMp.SetBasicStatValue( ONE_CHARGE * 3.0f );

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	m_AdditionalHyperCharge.SetBasicStatValue( 0.f );
#endif //SERV_ADD_LUNATIC_PSYKER

	CheckAndUpdateAdditionalStat();
}

/** @function : InitAdditionalPassiveMultipleAndSum
	@brief : 패시브 초기화
*/
void CX2GameUnit::InitAdditionalPassive()
{
	InitAdditionalPassiveMultipleAndSum(m_AdditionalAnimSpeed);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalWalkSpeed);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalDashSpeed);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalJumpPower);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalDashJumpPower);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalAccuracy);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalAvoidance);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalCriticalRate);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalCriticalAttack);

	InitAdditionalPassiveMultipleAndSum(m_AdditionalAntiCriticalRate);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalCriticalDefence);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAdditionalAttack);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAdditionalDefence);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChargeMpHitMeleeAttack);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChargeMpHitRangeAttack);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChargeMpHitted);

	InitAdditionalPassiveMultipleAndSum(m_AdditionalFinalDamageMultiplierMeleeAttack);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalFinalDamageMultiplierRangeAttack);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalFinalDefenceMultiplier);
	InitAdditionalPassiveMultipleAndSum(m_AddtionalForceDownMultiplierMeleeAttack);
	InitAdditionalPassiveMultipleAndSum(m_AddtionalForceDownMultiplierRangeAttack);

	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAttributeBlazeDefence);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAttributeWaterDefence);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAttributeNatureDefence);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAttributeWindDefence);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAttributeLightDefence);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChangeAttributeDarkDefence);

	InitAdditionalPassiveMultipleAndSum(m_AdditionalDrainHpNormalAttack);

	InitAdditionalPassiveMultipleAndSum(m_AdditionalChargeMpHitMeleeAttack);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChargeMpHitRangeAttack);
	InitAdditionalPassiveMultipleAndSum(m_AdditionalChargeMpHitted);

	InitAdditionalPassiveMultipleAndSum(m_AdditionalMaxMp);

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	InitAdditionalPassiveMultipleAndSum(m_AdditionalHyperCharge);
#endif //SERV_ADD_LUNATIC_PSYKER

	/// 소켓옵션에 의한 것을 Passive쪽에 넣어줌(연산 로직 때문에 이렇게 함)
	m_AdditionalChargeMpHitMeleeAttack.SetPassiveSum( m_SocketData.m_fMPMultiplyByAll );
	m_AdditionalChargeMpHitRangeAttack.SetPassiveSum( m_SocketData.m_fMPMultiplyByAll );
	m_AdditionalChargeMpHitted.SetPassiveSum( m_SocketData.m_fMPMultiplyByAll );
}

/** @function : InitAdditionalPassiveMultipleAndSum
	@brief : 패시브 배율, 증감 수치 초기화
*/
void CX2GameUnit::InitAdditionalPassiveMultipleAndSum(CX2AdditionalStatByBuff& Additional_ )
{
	Additional_.SetPassiveMultiple(1.f);
	Additional_.SetPassiveSum(0.f);
}


/*virtual*/ void CX2GameUnit::InitAddtionalDashAndJumpBasicStatValue()
{
	m_AdditionalWalkSpeed.SetBasicStatValue( m_PhysicParam.GetWalkSpeed() );
	m_AdditionalDashSpeed.SetBasicStatValue( m_PhysicParam.GetDashSpeed() );

	m_AdditionalJumpPower.SetBasicStatValue( m_PhysicParam.GetJumpSpeed() );
	m_AdditionalDashJumpPower.SetBasicStatValue( m_PhysicParam.GetDashJumpSpeed() );
}

/** @function : SetBuffFactorPacketToGameUnit
	@brief : 서버로 부터 버프요소 정보를 전달 받아 게임유닛에 적용
	@param : 서버로 부터 전달받은 버프 목록
*/
void CX2GameUnit::SetKBuffFactorPacketToGameUnit( const vector<KBuffFactor>& vecBuffFactor_ )
{
	const CX2BuffTempletManager* pBuffTempletManager = CX2BuffTempletManager::GetInstance();

	BOOST_FOREACH( const KBuffFactor& kBuffFactor, vecBuffFactor_ )
	{
		auto FindFunc = [&kBuffFactor]( CX2BuffTempletPtr ptr_ ) {
			return ( ptr_->GetBuffTempletID() == kBuffFactor.m_BuffIdentity.m_eBuffTempletID
				&& ptr_->GetUniqueNum() == kBuffFactor.m_BuffIdentity.m_uiUniqueNum );
		};

		/// kimhc // 현재 적용된 버프 중 같은 것이 있는지 찾아보는 루틴인데, 이래야 할 필요가 있을까..?
		vector<CX2BuffTempletPtr>::iterator vItr =
			std::find_if( m_vecBuffTempletPtr.begin(), m_vecBuffTempletPtr.end(), FindFunc );

		if ( m_vecBuffTempletPtr.end() == vItr )
		{
			CX2BuffTempletPtr ptrBuffTemplet = pBuffTempletManager->GetBuffTempletPtr( static_cast<BUFF_TEMPLET_ID>( kBuffFactor.m_BuffIdentity.m_eBuffTempletID ) );
			if ( NULL != ptrBuffTemplet )
			{
				CX2BuffTempletPtr ptrCloneBuffTemplet = ptrBuffTemplet->GetClonePtr();
				if ( ptrCloneBuffTemplet->SetFactorFromPacket( kBuffFactor, this ) )
				{
					/// 디버프면 디버프카운트를, 버프면 버프카운트를 증가
					( true == ptrCloneBuffTemplet->IsDeBuff() ? SetNumOfDeBuff( GetNumOfDeBuff() + 1 ) : SetNumOfBuff( GetNumOfBuff() + 1 ) );
					/// 각 유닛마다 특정 버프가 실행될 때 셋팅해줘야 하는 값(ex: 매지컬메이크업이 시전중이라는 플래그 등...)
					SetSpecificValueByBuffTempletID( ptrCloneBuffTemplet->GetBuffTempletID() );
					m_vecBuffTempletPtr.push_back( ptrCloneBuffTemplet );
#ifdef BUFF_ICON_UI
					if( true == ptrCloneBuffTemplet->GetUseBuffIcon() )
					{
						CX2GageManager::GetInstance()->PushBuff( m_UnitUID, CX2GageUI::BuffIcon( ptrCloneBuffTemplet->GetBuffTempletID(),ptrCloneBuffTemplet->GetIconFileName(), 
						ptrCloneBuffTemplet->GetIconKeyName(), ptrCloneBuffTemplet->GetBuffName(), ptrCloneBuffTemplet->GetBuffDesc() ), ptrCloneBuffTemplet->IsDeBuff());
					}
#endif //BUFF_ICON_UI
				}
			}
		}
		/// kimhc // 이미 같은 버프가 존재 하는 경우에 대한 예외처리가 필요할 듯(패킷을 받기 전 해당 유닛이 같은 버프를 사용하는 경우 예외처리)
	}
}

/** @function : OnFrameMoveBuff
	@brief : OnFrameMove에서 처리할 Buff 프로세스로 버프들의 OnFrameMove, 갱신, 삭제 등을 수행
*/
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GameUnit::OnFrameMoveBuff( float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
void CX2GameUnit::OnFrameMoveBuff()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
{
	if ( !m_vecBuffTempletPtr.empty() )
	{
		vector<CX2BuffTempletPtr>::iterator vItr 
			= m_vecBuffTempletPtr.begin();

		while ( m_vecBuffTempletPtr.end() != vItr )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            (*vItr)->OnFrameMove( this, fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			(*vItr)->OnFrameMove( this );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if ( (*vItr)->GetDidFinish() )
			{
				/// 디버프면 디버프카운트를, 버프면 버프카운트를 감소
				( true == (*vItr)->IsDeBuff() ? SetNumOfDeBuff( GetNumOfDeBuff() - 1 ) : SetNumOfBuff( GetNumOfBuff() - 1 ) );
				UnSetSpecificValueByBuffTempletID( (*vItr)->GetBuffTempletID() );
				(*vItr)->DoFinishBehavior( this );
				(*vItr)->DoFinishDisplayer( this );

#ifdef BUFF_ICON_UI
				if( true == (*vItr)->GetUseBuffIcon() )
					CX2GageManager::GetInstance()->EraseBuff( m_UnitUID, (*vItr)->GetBuffTempletID(), (*vItr)->IsDeBuff() );
#endif //BUFF_ICON_UI
				vItr = m_vecBuffTempletPtr.erase( vItr );

				//g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ELK_MAGIC_RESISTANCE", this ); 	
			}
			else
				++vItr;
		}
	}
}

/** @function : FinishAndClearAllBuff
	@brief : 모든 버프를 종료후 모두 clear 함
*/
void CX2GameUnit::FinishAndClearAllBuff()
{
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	/// 자신의 유닛인 경우
	if ( IsMyUnit() )
		pGageManager->GetBuffFactorFromGameUnit( this );	/// GageManager에 버프 정보 임시 저장

	BOOST_FOREACH( CX2BuffTempletPtr ptrBuffTemplet, m_vecBuffTempletPtr )
	{
		if ( NULL != ptrBuffTemplet )
		{
			( true == ptrBuffTemplet->IsDeBuff() ? SetNumOfDeBuff( GetNumOfDeBuff() - 1 ) : SetNumOfBuff( GetNumOfBuff() - 1 ) );
			UnSetSpecificValueByBuffTempletID( ptrBuffTemplet->GetBuffTempletID() );
			ptrBuffTemplet->DoFinishBehavior( this );
			ptrBuffTemplet->DoFinishDisplayer( this );
		}
	}

	m_vecBuffTempletPtr.clear();
	m_vecBuffTempletPtr.swap( vector<CX2BuffTempletPtr>() );
#ifdef BUFF_ICON_UI
	pGageManager->ClearBuffList(m_UnitUID);
#endif //BUFF_ICON_UI
}

/** @function : ApplyRenderParam
	@brief : 버프에 의해 적용되야할 RenderParam 값이 있으면 제일 마지막 것을 적용하고 없으면 기본RenderParam을 적용하는 함수
	@param : SkinAnim의 RenderParam 포인터(pRenderParam_)
*/
/*virtual*/ void CX2GameUnit::ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
	/// kimhc // 버프를 만든 후에 이부분을 매 프레임 실행하지 않아도 될수있도록 할 개선방법을 찾아보자
	/// mauntain // 만약 이 부분을 매프레임 실행 시키지 않도록 수정했다면, 다른 곳에서 GetFlickerColorByBuff()를 매 프레임 실행 시켜 주도록 수정 해 주자.
	#ifdef RIDING_SYSTEM
	if ( m_bPassDash )
		return;
	#endif //RIDING_SYSTEM

	if ( m_vecRenderParamByBuffPtr.empty() )
	{
		ApplyRenderParamOnEmptyRenderParamByBuff( pRenderParam_ );
		ApplyWorldLightColor( pRenderParam_ );
	}
	else
	{
		CX2RenderParamByBuffPtr ptrBuffRenderParam = m_vecRenderParamByBuffPtr.back();
		if ( NULL != ptrBuffRenderParam )
		{
			const StBuffRenderParam& stBuffRenderParam = ptrBuffRenderParam->GetBuffRenderParam();

			pRenderParam_->renderType = stBuffRenderParam.m_eRenderType;
			pRenderParam_->cartoonTexType = stBuffRenderParam.m_eCartoonTexType;
			pRenderParam_->fOutLineWide = stBuffRenderParam.m_fOutLineWide;
			pRenderParam_->outLineColor = stBuffRenderParam.m_d3dxColorOutLine;
			pRenderParam_->bAlphaBlend = stBuffRenderParam.m_bAlphaBlend;

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			/// 반짝이는 색 간격이 설정되어 있다면, 색 설정 해주자.
			if ( 0.f < stBuffRenderParam.m_fFlickerColorGap )
				pRenderParam_->color	= GetFlickerColorByBuff( stBuffRenderParam.m_d3dxColor, stBuffRenderParam.m_fFlickerColorGap );
			else
#endif //SERV_ADD_LUNATIC_PSYKER
				pRenderParam_->color	= stBuffRenderParam.m_d3dxColor;
		}
	}
}

/** @function : ApplyHyperModeBuff
	@brief : 각성버프 적용
*/
/*virtual*/ void CX2GameUnit::ApplyHyperModeBuff()
{
	SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_HYPER_MODE ), 1 );

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

/** @function : HyperModeBuffEffectEnd
	@brief : 각성 종료시 각성이펙트 해제를 위해 수행되는 함수
*/
/*virtual*/ void CX2GameUnit::HyperModeBuffEffectEnd()
{
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostRFoot );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostLFoot );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostRArm );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hHyperBoostLArm );
}

/** @function : HyperModeBuffEffectOnFrameMove
	@brief : 각성 이펙트를 매 프레임 갱신하는 함수
*/
/*virtual*/ void CX2GameUnit::HyperModeBuffEffectOnFrameMove()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq_RFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRFoot );
	CKTDGParticleSystem::CParticleEventSequence* pSeq_LFoot = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLFoot );
	CKTDGParticleSystem::CParticleEventSequence* pSeq_RArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostRArm );
	CKTDGParticleSystem::CParticleEventSequence* pSeq_LArm = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hHyperBoostLArm );

	if( pSeq_RFoot	!= NULL
		&& pSeq_LFoot	!= NULL
		&& pSeq_RArm 	!= NULL
		&& pSeq_LArm 	!= NULL )
	{
		/// kimhc // 좀더 효율적으로 정리 해보자
		if( GetRemainHyperModeTime() < GetMaxHyperModeTime() / 3.0f )
		{
			CMinMax<float> emit( 15, 20 );
			pSeq_RFoot->SetEmitRate( emit );
			pSeq_LFoot->SetEmitRate( emit );
			pSeq_RArm ->SetEmitRate( emit );
			pSeq_LArm ->SetEmitRate( emit );
		}
		else if( GetRemainHyperModeTime() < GetMaxHyperModeTime() / 2.0f )
		{
			CMinMax<float> emit( 20, 30 );
			pSeq_RFoot->SetEmitRate( emit );
			pSeq_LFoot->SetEmitRate( emit );
			pSeq_RArm ->SetEmitRate( emit );
			pSeq_LArm ->SetEmitRate( emit );
		}
		else
		{
			CMinMax<float> emit( 30, 40 );
			pSeq_RFoot->SetEmitRate( emit );
			pSeq_LFoot->SetEmitRate( emit );
			pSeq_RArm ->SetEmitRate( emit );
			pSeq_LArm ->SetEmitRate( emit );
		}

		if( GetIsRight() == true )
		{
			D3DXVECTOR3 pos;
			GetFramePos( &pos, m_pFrame_Bip01_R_Foot );
			pSeq_RFoot->SetPosition( pos );
			GetFramePos( &pos, m_pFrame_Bip01_L_Foot );
			pSeq_LFoot->SetPosition( pos );
			GetFramePos( &pos, m_pFrame_Bip01_R_UpperArm );
			pSeq_RArm->SetPosition( pos );
			GetFramePos( &pos, m_pFrame_Bip01_L_UpperArm );
			pSeq_LArm->SetPosition( pos );
		}
		else
		{
			D3DXVECTOR3 pos;
			GetFramePos( &pos, m_pFrame_Bip01_L_Foot );
			pSeq_RFoot->SetPosition( pos );
			GetFramePos( &pos, m_pFrame_Bip01_R_Foot );
			pSeq_LFoot->SetPosition( pos );
			GetFramePos( &pos, m_pFrame_Bip01_L_UpperArm );
			pSeq_RArm->SetPosition( pos );
			GetFramePos( &pos, m_pFrame_Bip01_R_UpperArm );
			pSeq_LArm->SetPosition( pos );
		}	

		pSeq_RFoot->SetTrace( true );
		pSeq_LFoot->SetTrace( true );
		pSeq_RArm ->SetTrace( true );
		pSeq_LArm ->SetTrace( true );

// 		if( IsMyUnit() == true )
// 		{
// 			float rotate = m_PhysicParam.nowSpeed.x / 20.0f;
// 			if( GetIsRight() == true )
// 			{			
// 				pSeq_RFoot->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, rotate) );
// 				pSeq_LFoot->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, rotate) );
// 				pSeq_RArm ->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, rotate) );
// 				pSeq_LArm ->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, rotate) );
// 			}
// 			else
// 			{
// 				pSeq_RFoot->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, -rotate) );
// 				pSeq_LFoot->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, -rotate) );
// 				pSeq_RArm ->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, -rotate) );
// 				pSeq_LArm ->SetAddRotate( D3DXVECTOR3( 0.0f, 0.0f, -rotate) );
// 			}
// 		}
	}
}

/** @function : CanApplyBuffToGameUnit
	@brief : 버프에 걸릴수 있는 상태인지 알아내는 함수
	@return : 걸릴수 있으면 true, 없으면 false
*/
bool CX2GameUnit::CanApplyBuffToGameUnit() const
{
	if ( IsImmuneToExtraDamage() || !(GetNowHp() > 0.0f) || GetNowStateID() == GUSI_DIE )
		return false;
	else
		return true;
}

/** @function : CreateAndInsertBuffIdentity
	@brief : 버프에 의한 효과 생성 및 적용(대시불가, 공격불가, 수퍼아머 등)
	@param : 버프 행동 타입(eType_), 버프 종료를 식별할 수 있는 요소(BuffIdentity_)
*/
void CX2GameUnit::CreateAndInsertBuffIdentity( const BUFF_BEHAVIOR_TYPE eType_, const KBuffIdentity& BuffIdentity_ )
{
	CX2BuffIdentityPtr ptrBuffIdentity( new CX2BuffIdentity( BuffIdentity_ ) );

	if ( NULL != ptrBuffIdentity )
	{
		switch ( eType_ )
		{
		case BBT_DOWN_IMPOSSIBLE:
			m_vecDownImpossiblePtr.push_back( ptrBuffIdentity );
			break;

		case BBT_DASH_IMPOSSIBLE:
			m_vecDashImpossiblePtr.push_back( ptrBuffIdentity );
			break;

		case BBT_ATTACK_IMPOSSIBLE:
			m_vecAttackImpossiblePtr.push_back( ptrBuffIdentity );
			break;

		case BBT_SUPER_ARMOR:
			m_vecSuperArmorPtr.push_back( ptrBuffIdentity );
			break;

		case BBT_ATTACK_ALL_TEAM:
			m_vecAttackAllTeamPtr.push_back( ptrBuffIdentity );
			break;

		case BBT_SKILL_ATTACK_IMPOSSIBLE:
			m_vecSkillImpossiblePtr.push_back( ptrBuffIdentity );
			break;

		case BBT_ZX_ATTACK_IMPOSSIBLE:
			m_vecZXImpossiblePtr.push_back( ptrBuffIdentity );
			break;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// Z 공격 불가, X 공격 불가 버프 타입 추가
		case BBT_Z_ATTACK_IMPOSSIBLE:
			m_vecZImpossiblePtr.push_back( ptrBuffIdentity );
			break;
		case BBT_X_ATTACK_IMPOSSIBLE:
			m_vecXImpossiblePtr.push_back( ptrBuffIdentity );
			break;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		default:
			break;
		}
	}
}

/** @function : EraseBuffIdentity
	@brief : 버프에 의한 효과 삭제(대시불가, 공격불가, 수퍼아머 등)
	@param : 버프 행동 타입(eType_), 버프 종료를 식별할 수 있는 요소(BuffIdentity_)
*/
void CX2GameUnit::EraseBuffIdentity( const BUFF_BEHAVIOR_TYPE eType_, const KBuffIdentity& BuffIdentity_ )
{
	switch ( eType_ )
	{
	case BBT_DOWN_IMPOSSIBLE:
		ERASE_BUFF_IDENTITY( m_vecDownImpossiblePtr, BuffIdentity_ );
		break;

	case BBT_DASH_IMPOSSIBLE:
		ERASE_BUFF_IDENTITY( m_vecDashImpossiblePtr, BuffIdentity_ );
		break;

	case BBT_ATTACK_IMPOSSIBLE:
		ERASE_BUFF_IDENTITY( m_vecAttackImpossiblePtr, BuffIdentity_ );
		break;

	case BBT_SUPER_ARMOR:
		ERASE_BUFF_IDENTITY( m_vecSuperArmorPtr, BuffIdentity_ );
		break;

	case BBT_ATTACK_ALL_TEAM:
		ERASE_BUFF_IDENTITY( m_vecAttackAllTeamPtr, BuffIdentity_ );
		break;

	case BBT_SKILL_ATTACK_IMPOSSIBLE:
		ERASE_BUFF_IDENTITY( m_vecSkillImpossiblePtr, BuffIdentity_ );
		break;

	case BBT_ZX_ATTACK_IMPOSSIBLE:
		ERASE_BUFF_IDENTITY( m_vecZXImpossiblePtr, BuffIdentity_ );
		break;

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// Z 공격 불가, X 공격 불가 버프 타입 추가
	case BBT_Z_ATTACK_IMPOSSIBLE:
		ERASE_BUFF_IDENTITY( m_vecZImpossiblePtr, BuffIdentity_ );
		break;
	case BBT_X_ATTACK_IMPOSSIBLE:
		ERASE_BUFF_IDENTITY( m_vecXImpossiblePtr, BuffIdentity_ );
		break;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}	
}

/** @function : DoDelegateProcess
	@brief : 전달 받은 델리게이터들의 모음을 실행 하는 함수
	@param : 델리게이터 모음(vecDelegateProcess_)
*/
void CX2GameUnit::DoDelegateProcess( const vector<DelegateProcess>& vecDelegateProcess_ )
{
	if ( !vecDelegateProcess_.empty() )
	{
		BOOST_FOREACH( const DelegateProcess& delegateProcess, vecDelegateProcess_ )
		{
			if ( delegateProcess )
				delegateProcess();
		}
	}
}

/** @function : DoDelegateProcessWithDamageData
	@brief : 전달 받은 델리게이터를 DamageData를 인자로 받아 실행하는 함수
	@param : 델리게이터 모음(vecDelegateProcess_), const DamageData&
*/
void CX2GameUnit::DoDelegateProcessWithDamageData( const vector<DelegateProcessWithDamageData>& vecDelegateProcess_, const CX2DamageManager::DamageData& damageData_ )
{
	if ( !vecDelegateProcess_.empty() )
	{
		BOOST_FOREACH( const DelegateProcessWithDamageData& delegateProcess, vecDelegateProcess_ )
		{
			if ( delegateProcess )
				delegateProcess( damageData_ );
		}
	}
}


/** @function : DoUnitSlashTraceInCommonStateStart
	@brief : CommonStateStart에서 실행될 유닛에 붙은 SlashTrace를 실행하는 함수
*/
void CX2GameUnit::DoUnitSlashTraceInCommonStateStart()
{
#ifdef UNIT_SLASH_TRACE_MANAGER_TEST

	int iTableIndex = 0; 
	while( true == m_LuaManager.BeginTable( "UNIT_SLASH_TRACE", iTableIndex ) )
	{
		CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition = CX2UnitSlashTraceManager::STC_NONE;
		LUA_GET_VALUE_ENUM( m_LuaManager, 5, eSlashTraceCondition, CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION, CX2UnitSlashTraceManager::STC_NONE );

		if ( GetUnitSlashTraceConditionToDo() == eSlashTraceCondition )
			AddSlashTraceTiming();
		
		m_LuaManager.EndTable(); // UNIT_SLASH_TRACE%d
		iTableIndex++;
	}

#endif UNIT_SLASH_TRACE_MANAGER_TEST
}

/** @function : AddSlashTraceTiming
	@brief : 유닛의 스테이트에 정의되어있는 인덱스, 타이밍 대로 트레이스를 실행
*/
void CX2GameUnit::AddSlashTraceTiming()
{
	int iSlashTraceIndex = 0;
	LUA_GET_VALUE( m_LuaManager, 1,	iSlashTraceIndex, 0 );

	float fStartAnimTime = 0.f;
	LUA_GET_VALUE( m_LuaManager, 2,	fStartAnimTime, 0.f );

	float fEndAnimTime = 0.f;
	LUA_GET_VALUE( m_LuaManager, 3, fEndAnimTime, 0.f );

	CKTDGSlashTrace::SLASH_TRACE_TYPE eSlashTraceType = CKTDGSlashTrace::STT_NONE;
	LUA_GET_VALUE_ENUM( m_LuaManager, 4, eSlashTraceType, CKTDGSlashTrace::SLASH_TRACE_TYPE, CKTDGSlashTrace::STT_NONE );

	m_pUnitSlashTraceManager->AddSlashTraceTiming( iSlashTraceIndex, fStartAnimTime, fEndAnimTime, eSlashTraceType );
}

/** @function : CreateAndInsertEffectSetImpactPointByBuff
	@brief : 타격시 타격점에 출력할 이펙트셋을 지정
	@param : 해당 이펙트셋을 출력 시키는 버프의 식별자(BuffIdentity_), 이펙트셋이름(pwszEffectSetName_), 사운드이름(pwszSoundName_)
*/

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09		생성 간격 추가

void CX2GameUnit::CreateAndInsertEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCoolTime )
{
	CX2EffectSetImpactPointByBuffPtr ptrBuffEffectSet= CX2EffectSetImpactPointByBuffPtr( new CX2EffectSetImpactPointByBuff( BuffIdentity_, pwszEffectSetName_, pwszSoundName_, fCoolTime ) );
	if ( NULL != ptrBuffEffectSet )
		m_vecEffectSetImpactPointByBuffPtr.push_back( ptrBuffEffectSet );
}

#else // SERV_NEW_DEFENCE_DUNGEON

void CX2GameUnit::CreateAndInsertEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_ )
{
	CX2EffectSetImpactPointByBuffPtr ptrBuffEffectSet= CX2EffectSetImpactPointByBuffPtr( new CX2EffectSetImpactPointByBuff( BuffIdentity_, pwszEffectSetName_, pwszSoundName_ ) );
	if ( NULL != ptrBuffEffectSet )
		m_vecEffectSetImpactPointByBuffPtr.push_back( ptrBuffEffectSet );
}

#endif // SERV_NEW_DEFENCE_DUNGEON

/** @function : EraseEffectSetImpactPointByBuff
	@brief : 타격시 타격점에 출력할 이펙트셋이 지정된것을 삭제
	@param : 해당 이펙트셋을 출력 시키는 버프의 식별자(BuffIdentity_)
*/
void CX2GameUnit::EraseEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2EffectSetImpactPointByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2EffectSetImpactPointByBuffPtr>::iterator, CX2EffectSetImpactPointByBuffPtr>( m_vecEffectSetImpactPointByBuffPtr, BuffIdentity_ );

	if ( m_vecEffectSetImpactPointByBuffPtr.end() != vItr )
		m_vecEffectSetImpactPointByBuffPtr.erase( vItr );
}

/** @function : PlayEffectSetImpactPointByBuff
	@brief : 타격시 타격점에 리스트에 있는 이펙트셋들을 출력
*/
void CX2GameUnit::PlayEffectSetImpactPointByBuff()
{
	if ( !m_vecEffectSetImpactPointByBuffPtr.empty() )
	{
		BOOST_FOREACH( CX2EffectSetImpactPointByBuffPtr ptr, m_vecEffectSetImpactPointByBuffPtr )
		{
			if ( NULL != ptr )
				ptr->PlayEffectSetImpactPointByBuff( this, m_DamageData.impactPoint );
		}
	}
}

/** @function : InsertPairSubAttackListSet
	@brief : 이펙트의 타격이 일반타격과 같이 이루어지게 하기 위해 이펙트의 타격박스를 유닛에 직접 추가하는 함수
	@param : Pair의 키로 사용할 메시핸들(hMeshHandle_), 타격박스를 가지고 있는 메시의 SkinAnim 포인터(pSkinAnim_)
*/
void CX2GameUnit::InsertPairSubAttackListSet( const CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshHandle_, const CKTDGXSkinAnim* pSkinAnim_ )
{
	if ( NULL != pSkinAnim_ )
	{
		CKTDXCollision::CollisionDataListSet setCollisionDataList;
		setCollisionDataList.insert( &pSkinAnim_->GetAttackDataList() );

		m_vecPairSubAttackListSet.push_back( std::make_pair( hMeshHandle_, setCollisionDataList ) );
	}
}

/** @function : ErasePairSubAttackListSet
	@brief : 이펙트의 타격이 일반타격과 같이 이루어지게 하기 위한 이펙트의 타격박스를 유닛에서 제거
	@param : 삭제할 Key값인 메시핸들(hMeshHandle_)
*/
void CX2GameUnit::ErasePairSubAttackListSet( const CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshHandle_ )
{
	if ( !m_vecPairSubAttackListSet.empty() )
	{
		auto FindFunc = [hMeshHandle_](const PairSubAttackListSet& pairSubAttackListSet_) {
			return pairSubAttackListSet_.first == hMeshHandle_;
		};

		vector<PairSubAttackListSet>::iterator vItr =
			std::find_if( m_vecPairSubAttackListSet.begin(), m_vecPairSubAttackListSet.end(), FindFunc );

		if ( m_vecPairSubAttackListSet.end() != vItr )
			m_vecPairSubAttackListSet.erase( vItr );
	}
}

/** @function : GetSubAttackListGetFromPair
	@brief : 인덱스에 해당하는 SubAttackListSet을 얻어오는 함수
	@param : 값이 있으면 true, 없으면 false
*/
bool CX2GameUnit::GetSubAttackListGetFromPair( const UINT uiIndex_, OUT const CKTDXCollision::CollisionDataListSet** ppCollisionDataListSet_ )
{
	if ( uiIndex_ < m_vecPairSubAttackListSet.size() )
	{
		*ppCollisionDataListSet_ = &(m_vecPairSubAttackListSet[uiIndex_].second);
		return true;
	}
	else
	{
		*ppCollisionDataListSet_ = NULL;
		return false;
	}
}

/** @function : CreateAndInsertChangeUnitScaleByBuff
	@brief : 버프에 의한 유닛의 스케일 변경 적용
	@param : 버프 식별자(BuffIdentity_), 크기(vScale_)
*/
void CX2GameUnit::CreateAndInsertChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vScale_ )
{
#ifdef POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE
	const D3DXVECTOR3 vChangeScale( m_vBasicScale.x * vScale_.x, m_vBasicScale.y * vScale_.y, m_vBasicScale.z * vScale_.z );		/// 현재 스케일에 변화될 수치를 연산

	CX2ChangeUnitScaleByBuffPtr ptrChangeUnitScaleByBuff( new CX2ChangeUnitScaleByBuff( BuffIdentity_, vChangeScale ) );
#else  POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE
	CX2ChangeUnitScaleByBuffPtr ptrChangeUnitScaleByBuff( new CX2ChangeUnitScaleByBuff( BuffIdentity_, vScale_ ) );
#endif POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

	if ( NULL != ptrChangeUnitScaleByBuff )
	{
		m_vecChangeUnitScaleByBuffPtr.push_back( ptrChangeUnitScaleByBuff );
		TransformScale( ptrChangeUnitScaleByBuff->GetScale() );
	}
}

/** @function : EraseChangeUnitScaleByBuff
	@brief : 버프에 의한 유닛의 스케일 변경 해제
	@param : 버프 식별자(BuffIdentity_), 헤제 시에 돌아가는데 정해진 시간을 쓸것인가(bUseTimeToReturn_)
*/
void CX2GameUnit::EraseChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const bool bUseTimeToReturn_ )
{
	vector<CX2ChangeUnitScaleByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2ChangeUnitScaleByBuffPtr>::iterator, CX2ChangeUnitScaleByBuffPtr>( m_vecChangeUnitScaleByBuffPtr, BuffIdentity_ );

	if ( m_vecChangeUnitScaleByBuffPtr.end() != vItr )
	{
		m_vecChangeUnitScaleByBuffPtr.erase( vItr );

		m_fTimeDirectToReturnBasicScale = 0.1f;	/// 원래 스케일로 돌아가는데 걸리는 시간
		m_delegateProcessReturnFromScaleChange = SET_DELEGATE_PROCESS( CX2GameUnit, ProcessReturnFromScaleChange );

		if ( bUseTimeToReturn_ )	/// 좌우로 울렁울렁 거리는 효과를 사용
		{
			/// 체크를 안했다라고 지정해주어 다시 체크 하도록 셋팅함
			m_bCheckHeightEnlargeSize = false;
			m_bCheckWidthEnlargeSize = false;			
		}
	}
}

/** @function : ProcessReturnFromScaleChange
	@brief : 스케일 변경이 있은 후 돌아가는 처리르 하는 함수로 m_delegateProcessReturnFromScaleChange 에 이 함수가 지정되어 있어야만 처리된다
*/
void CX2GameUnit::ProcessReturnFromScaleChange()
{
	if ( 0.0f < m_fTimeToReturnBasicScale )
	{
		m_fTimeToReturnBasicScale -= m_fElapsedTime;	/// 치트 용...
		return;
	}
	else
		m_fTimeToReturnBasicScale = 0.0f;

#ifdef VERIFY_STAT_BY_BUFF
	PROTECT_VECTOR3 vTransePos = D3DXVECTOR3(1,1,1);
#else	// VERIFY_STAT_BY_BUFF
	D3DXVECTOR3 vTransePos = D3DXVECTOR3(1,1,1);
#endif // VERIFY_STAT_BY_BUFF

	if ( m_bCheckHeightEnlargeSize == false )
	{
		vTransePos = D3DXVECTOR3( 0.7f, 1.4f, 0.7f );
		vTransePos += (m_vBasicScale - D3DXVECTOR3( 1.0f, 1.0f, 1.0f ));
	}
	else if ( m_bCheckWidthEnlargeSize == false )
	{
		vTransePos = D3DXVECTOR3( 1.8f, 0.5f, 1.8f );
		vTransePos += (m_vBasicScale - D3DXVECTOR3( 1.0f, 1.0f, 1.0f ));
	}
	else
	{
		vTransePos = m_vBasicScale;
	}

	if ( m_fTimeDirectToReturnBasicScale > 0.0f )
	{
		m_fTimeDirectToReturnBasicScale -= m_fElapsedTime;

#ifdef VERIFY_STAT_BY_BUFF
		PROTECT_VECTOR3 remainScale		= vTransePos - m_vTransScale;
#else	// VERIFY_STAT_BY_BUFF
		D3DXVECTOR3 remainScale		= vTransePos - m_vTransScale;
#endif // VERIFY_STAT_BY_BUFF
		m_vTransScale				+= (remainScale / m_fTimeDirectToReturnBasicScale) * m_fElapsedTime;
	}

	if ( m_bCheckHeightEnlargeSize == false )
	{
		if ( m_fTimeDirectToReturnBasicScale <= 0.0f )
		{
			m_fTimeDirectToReturnBasicScale = 0.2f;
			m_bCheckHeightEnlargeSize = true;
		}
	}
	else if ( m_bCheckWidthEnlargeSize == false )
	{
		if ( m_fTimeDirectToReturnBasicScale <= 0.0f )
		{
			m_fTimeDirectToReturnBasicScale = 0.2f;
			m_bCheckWidthEnlargeSize = true;
		}
	}

	if ( m_fTimeDirectToReturnBasicScale <= 0.0f )
	{
		m_fTimeDirectToReturnBasicScale = 0.0f;
		
#ifdef FIX_RETURN_FROM_SCALE_CHANGE
		if( true == m_vecChangeUnitScaleByBuffPtr.empty() )
			TransformScale( m_vBasicScale );
		else
#endif // FIX_RETURN_FROM_SCALE_CHANGE
		{
			TransformScale( GetChangeUnitScaleByBuff() );
		}

		if( GUT_USER == m_GameUnitType )
		{ //오현빈//2012-13-02//유저의 경우에만 사운드 출력하도록 변경
			PlaySound( L"Pressed_Return.ogg" );
		}

		m_delegateProcessReturnFromScaleChange = DelegateProcess();
	}
}

/** @function : IsChangedGameUnitScaleByBuff
	@brief : 버프에 의해 스케일이 변경된 상태 인가?
	@return : 변경된 상태면 true, 아니면 false
*/
bool CX2GameUnit::IsChangedGameUnitScaleByBuff() const
{
	return !m_vecChangeUnitScaleByBuffPtr.empty();	/// 무언가 들어있으면 스케일이 변경된 것임
}

/** @function : TransformScale_LUA
	@brief : 루아스크립트에서 치트를 통해 Scale을 변경할때 사용 (난입유저와 동기화 안됨)
	@param : 크기(fScale_), 유지시간(fTime_)
*/
void CX2GameUnit::TransformScale_LUA( const float fScale_, const float fTime_ )
{
	m_fTimeToReturnBasicScale = fTime_;
	m_delegateProcessReturnFromScaleChange = SET_DELEGATE_PROCESS( CX2GameUnit, ProcessReturnFromScaleChange );

	m_fTimeDirectToReturnBasicScale = 0.1f;

	m_bCheckHeightEnlargeSize = false;
	m_bCheckWidthEnlargeSize = false;

#ifdef VERIFY_STAT_BY_BUFF
	TransformScale( PROTECT_VECTOR3( fScale_, fScale_, fScale_ ) );
#else	// VERIFY_STAT_BY_BUFF
	TransformScale( D3DXVECTOR3( fScale_, fScale_, fScale_ ) );
#endif // VERIFY_STAT_BY_BUFF
}

#ifdef POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

/** @function : UpdateChangeUnitScaleByBuff
	@brief : 버프에 의한 유닛의 스케일 변경 갱신
	@param : 버프 식별자(BuffIdentity_), 크기(vScale_)
*/
void CX2GameUnit::UpdateChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vScale_, const UINT uiAccumulationLimit_ )
{
	if( !m_vecChangeUnitScaleByBuffPtr.empty() )
	{
		
		BOOST_FOREACH( CX2ChangeUnitScaleByBuffPtr ptrChangeUnitScaleByBuff , m_vecChangeUnitScaleByBuffPtr ) 
		{
			if( NULL != ptrChangeUnitScaleByBuff && 
				true == ptrChangeUnitScaleByBuff->IsSame(BuffIdentity_) )
			{
				if( uiAccumulationLimit_ > 1 ) //중첩
				{
#ifdef VERIFY_STAT_BY_BUFF
					const PROTECT_VECTOR3 vNowScale = ptrChangeUnitScaleByBuff->GetScale();		/// 현재 적용중인 스케일
					const PROTECT_VECTOR3 vChangeScale( vNowScale.x * vScale_.x, vNowScale.y * vScale_.y, vNowScale.z * vScale_.z );			/// 현재 스케일에 변화될 수치를 연산
#else	// VERIFY_STAT_BY_BUFF
					const D3DXVECTOR3 vNowScale = ptrChangeUnitScaleByBuff->GetScale();		/// 현재 적용중인 스케일
					const D3DXVECTOR3 vChangeScale( vNowScale.x * vScale_.x, vNowScale.y * vScale_.y, vNowScale.z * vScale_.z );			/// 현재 스케일에 변화될 수치를 연산
#endif // VERIFY_STAT_BY_BUFF


					ptrChangeUnitScaleByBuff->SetScale( vChangeScale );
					TransformScale( vChangeScale );
				}
				else //교체
				{
#ifdef VERIFY_STAT_BY_BUFF
					const PROTECT_VECTOR3 vChangeScale( m_vBasicScale.x * vScale_.x, m_vBasicScale.y * vScale_.y, m_vBasicScale.z * vScale_.z );/// 현재 스케일에 변화될 수치를 연산
#else	// VERIFY_STAT_BY_BUFF
					const D3DXVECTOR3 vChangeScale( m_vBasicScale.x * vScale_.x, m_vBasicScale.y * vScale_.y, m_vBasicScale.z * vScale_.z );/// 현재 스케일에 변화될 수치를 연산
#endif // VERIFY_STAT_BY_BUFF
					ptrChangeUnitScaleByBuff->SetScale( vChangeScale );
					TransformScale( vChangeScale );
				}
			}
		}
	}
	else	/// 혹시라도 벡터가 비어있다면, 새로 삽입
	{
		CreateAndInsertChangeUnitScaleByBuff( BuffIdentity_, vScale_ );
	}
}

/** @function : GetChangeUnitScaleByBuff
	@brief : 버프에 의한 유닛의 스케일 변경 수치 반환
	@return : 증가 수치 벡터
*/
D3DXVECTOR3 CX2GameUnit::GetChangeUnitScaleByBuff()
{
	if( !m_vecChangeUnitScaleByBuffPtr.empty() )
	{
		CX2ChangeUnitScaleByBuffPtr ptrChangeUnitScaleByBuff = m_vecChangeUnitScaleByBuffPtr.back();

#ifdef VERIFY_STAT_BY_BUFF
		return ptrChangeUnitScaleByBuff->GetScale().GetVector3();
#else	// VERIFY_STAT_BY_BUFF
		return ptrChangeUnitScaleByBuff->GetScale();
#endif // VERIFY_STAT_BY_BUFF
	}

	return D3DXVECTOR3( 1.f, 1.f, 1.f );
}

#endif POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

/** @function : CreateAndInsertCustomFunctionByBuff
	@brief : 버프에 의한 효과 중 StateStart, CommonFrameMove, StageEnd에서 처리해야하는 하드코딩을 수행하는 instance 생성 및 추가
	@param : 행동팩터(kBehaviorFactor_), 버프식별자(BuffIdentity_)
*/
void CX2GameUnit::CreateAndInsertCustomFunctionByBuff( const KBuffIdentity& BuffIdentity_, const KBuffBehaviorFactor& kBehaviorFactor_ )
{
	switch ( BuffIdentity_.m_eBuffTempletID )
	{
	case BTI_SI_SA_ETK_PHANTOM_SWORD:
		{
			CX2CustomFunctionByBuffPtr ptrCustomFunctionByBuff( new CX2CustomFunctionByBuffPhantomSword( BuffIdentity_, this ) );
			if ( NULL != ptrCustomFunctionByBuff )
				m_vecCustomFunctionByBuffPtr.push_back( ptrCustomFunctionByBuff );
		} break;

	case BTI_BUFF_UNFIXED_CLIP:
		{
			CX2CustomFunctionByBuffPtr ptrCustomFunctionByBuff( new CX2CustomFunctionByBuffBigBangStream( BuffIdentity_, this ) );
			if ( NULL != ptrCustomFunctionByBuff )
				m_vecCustomFunctionByBuffPtr.push_back( ptrCustomFunctionByBuff );
		} break;

	case BTI_DEBUFF_MARK_OF_COMMANDER:
		{
			CX2CustomFunctionByBuffPtr ptrCustomFunctionByBuff( new CX2CustomFunctionByBuffMarkOfCommand( BuffIdentity_, kBehaviorFactor_ ) );
			if ( NULL != ptrCustomFunctionByBuff )
				m_vecCustomFunctionByBuffPtr.push_back( ptrCustomFunctionByBuff );
		} break;

	default:
		break;
	}
}

/** @function : EraseCustomFunctionByBuff
	@brief : 버프에 의한 효과 중 StateStart, CommonFrameMove, StageEnd에서 처리해야하는 하드코딩을 수행하는 instance 삭제
	@param : 버프식별자(BuffIdentity_)
*/
void CX2GameUnit::EraseCustomFunctionByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2CustomFunctionByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2CustomFunctionByBuffPtr>::iterator, CX2CustomFunctionByBuffPtr>( m_vecCustomFunctionByBuffPtr, BuffIdentity_ );

	if ( m_vecCustomFunctionByBuffPtr.end() != vItr )
		m_vecCustomFunctionByBuffPtr.erase( vItr );
}

/** @function : DoCustomFunctionByBuffInCommonStateStart
	@brief : StateStart에서 실행 되어야 하는 버프에 의한 CustomFunction를 실행
*/
void CX2GameUnit::DoCustomFunctionByBuffInCommonStateStart()
{
	if ( !m_vecCustomFunctionByBuffPtr.empty() )
	{
		BOOST_FOREACH( CX2CustomFunctionByBuffPtr ptrCustomFunction, m_vecCustomFunctionByBuffPtr )
		{
			if ( NULL != ptrCustomFunction )
				ptrCustomFunction->FunctionInCommonStateStart( this, m_LuaManager );
		}
	}
}

/** @function : DoCustomFunctionByBuffInCommonFrameMove
	@brief : CommonFrameMove에서 실행 되어야 하는 버프에 의한 CustomFunction를 실행
*/
void CX2GameUnit::DoCustomFunctionByBuffInCommonFrameMove()
{
	if ( !m_vecCustomFunctionByBuffPtr.empty() )
	{
		BOOST_FOREACH( CX2CustomFunctionByBuffPtr ptrCustomFunction, m_vecCustomFunctionByBuffPtr )
		{
			if ( NULL != ptrCustomFunction )
				ptrCustomFunction->FunctionInCommonFrameMove( this, m_LuaManager );
		}
	}
}

/** @function : DoCustomFunctionByBuffInCommonStateEnd
	@brief : StateEnd에서 실행 되어야 하는 버프에 의한 CustomFunction를 실행
*/
void CX2GameUnit::DoCustomFunctionByBuffInCommonStateEnd()
{
	if ( !m_vecCustomFunctionByBuffPtr.empty() )
	{
		BOOST_FOREACH( CX2CustomFunctionByBuffPtr ptrCustomFunction, m_vecCustomFunctionByBuffPtr )
		{
			if ( NULL != ptrCustomFunction )
				ptrCustomFunction->FunctionInCommonStateEnd( this, m_LuaManager );
		}
	}
}

/** @function : DoCustomFunctionByBuffInDamageReact
	@brief : DamageReact에서 실행 되어야 하는 버프에 의한 DamageReact 실행
*/
void CX2GameUnit::DoCustomFunctionByBuffInDamageReact( IN CX2DamageManager::DamageData& damageData_ )
{
	if ( !m_vecCustomFunctionByBuffPtr.empty() )
	{
		BOOST_FOREACH( CX2CustomFunctionByBuffPtr ptrCustomFunction, m_vecCustomFunctionByBuffPtr )
		{
			if ( NULL != ptrCustomFunction )
				ptrCustomFunction->FunctionInDamageReact( this, damageData_ );
		}
	}
}

/** @function : CreateAndInsertEffectSetHittedByBuff
	@brief : 피격시  출력할 이펙트셋을 지정
	@param : 해당 이펙트셋을 출력 시키는 버프의 식별자(BuffIdentity_), 이펙트셋이름(pwszEffectSetName_), 사운드이름(pwszSoundName_)
*/
#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09		생성 간격 추가

void CX2GameUnit::CreateAndInsertEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap )
{
	CX2EffectSetImpactPointByBuffPtr ptrBuffEffectSet= CX2EffectSetImpactPointByBuffPtr( new CX2EffectSetImpactPointByBuff( BuffIdentity_, pwszEffectSetName_, pwszSoundName_, fCreateGap ) );
	if ( NULL != ptrBuffEffectSet )
		m_vecEffectSetHittedByBuffPtr.push_back( ptrBuffEffectSet );
}

#else // SERV_NEW_DEFENCE_DUNGEON

void CX2GameUnit::CreateAndInsertEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_ )
{
	CX2EffectSetImpactPointByBuffPtr ptrBuffEffectSet= CX2EffectSetImpactPointByBuffPtr( new CX2EffectSetImpactPointByBuff( BuffIdentity_, pwszEffectSetName_, pwszSoundName_ ) );
	if ( NULL != ptrBuffEffectSet )
		m_vecEffectSetHittedByBuffPtr.push_back( ptrBuffEffectSet );
}

#endif // SERV_NEW_DEFENCE_DUNGEON

/** @function : EraseEffectSetHittedByBuff
	@brief : 피격시 출력할 이펙트셋이 지정된것을 삭제
	@param : 해당 이펙트셋을 출력 시키는 버프의 식별자(BuffIdentity_)
*/
void CX2GameUnit::EraseEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2EffectSetImpactPointByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2EffectSetImpactPointByBuffPtr>::iterator, CX2EffectSetImpactPointByBuffPtr>( m_vecEffectSetHittedByBuffPtr, BuffIdentity_ );

	if ( m_vecEffectSetHittedByBuffPtr.end() != vItr )
		m_vecEffectSetHittedByBuffPtr.erase( vItr );
}

/** @function : PlayEffectSetHittedByBuff
	@brief : 피격시 리스트에 있는 이펙트셋들을 출력
*/
void CX2GameUnit::PlayEffectSetHittedByBuff()
{
	if ( !m_vecEffectSetHittedByBuffPtr.empty() )
	{
		BOOST_FOREACH( CX2EffectSetImpactPointByBuffPtr ptr, m_vecEffectSetHittedByBuffPtr )
		{
			if ( NULL != ptr )
				ptr->PlayEffectSetHittedByBuff( this );
		}
	}
}

/** @function : ApplyBuffToGameUnitInDamageReact
	@brief : DamageReact에서의 Buff적용 루틴
	@param : 적용할 Buff가 담긴 DamageData 포인터
	@return : Buff가 적용 됫으면 true 리턴
*/
bool CX2GameUnit::ApplyBuffToGameUnitInDamageReact( const CX2DamageManager::DamageData* pDamageData_ )
{
    if ( pDamageData_ == NULL )
        return false;

	bool bBeApplied = false;

	if ( !pDamageData_->m_vecBuffFactorPtr.empty() && CanApplyBuffToGameUnit() )
	{
		vector<CX2BuffFactorPtr>::const_iterator vItr 
			= pDamageData_->m_vecBuffFactorPtr.begin();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        const std::vector<BUFF_TEMPLET_ID>& vecImmunityAtThisState = GetVecImmunityAtThisState();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        const std::vector<BUFF_TEMPLET_ID>& vecImmunityAtThisState = m_vecImmunityAtThisState;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		while ( pDamageData_->m_vecBuffFactorPtr.end() != vItr )
		{
			if ( NULL != (*vItr) )
			{
				/// 현재상태의 이뮨이 없거나, 적용하려는 버프/디버프가 이뮨대상이 아닌지 확인
				if ( vecImmunityAtThisState.empty() ||
					vecImmunityAtThisState.end() == std::find( vecImmunityAtThisState.begin(), vecImmunityAtThisState.end(), (*vItr)->GetBuffTempletID() ) )
				{
					/// 시전자가 DamageData에 BuffFactor를 담기 전에 SetGameUnitBuffFactor를 실행했어야 하므로
					/// 이곳에서는 하지 않음
					SetBuffFactorToGameUnit( **vItr );

					bBeApplied = true;
				}
			}
			++vItr;
		}
	}

	return bBeApplied;
}



/** @function : CreateAndInsertAfterImageByBuff
	@brief : 버프에 의한 AfterImage 실행
	@param : 버프식별자(BuffIdentity_), AfterImage 색깔(d3dxColor_)
*/
void CX2GameUnit::CreateAndInsertAfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ )
{
	CX2AfterImageByBuffPtr ptrAfterImageByBuff( new CX2AfterImageByBuff( BuffIdentity_, d3dxColor_ ) );
	if ( NULL != ptrAfterImageByBuff )
		m_vecAfterImageByBuff.push_back( ptrAfterImageByBuff );

	EnableAfterImage();
}

/** @function : EraseAfterImageByBuff
	@brief : 버프에 의한 AfterImage 종료
	@param : 버프식별자(BuffIdentity_)
*/
void CX2GameUnit::EraseAfterImageByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2AfterImageByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2AfterImageByBuffPtr>::iterator, CX2AfterImageByBuffPtr>( m_vecAfterImageByBuff, BuffIdentity_ );

	if ( m_vecAfterImageByBuff.end() != vItr )
	{
		m_vecAfterImageByBuff.erase( vItr );
		DisableAfterImage();
	}
}

void CX2GameUnit::EnableAfterImage()
{
	if ( NULL != m_pAfterImage )
	{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH &&
			false == GetAbsoluteInvisibility() )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( g_pMain->GetGameOption().GetOptionList().m_bEffect &&
			false == GetAbsoluteInvisibility() )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		{
			m_pAfterImage->Enable();
			if ( !m_vecAfterImageByBuff.empty() )
				m_pAfterImage->SetAfterImageColor( m_vecAfterImageByBuff.back()->GetColor() );
		}
	}
}

void CX2GameUnit::DisableAfterImage( const bool bForce_ /*= false*/ )
{
	if ( NULL != m_pAfterImage )
	{
		if ( bForce_ || 
			m_vecAfterImageByBuff.empty() || 
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_LOW )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			false == g_pMain->GetGameOption().GetOptionList().m_bEffect )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		{
			m_pAfterImage->SetAfterImageColor( 0xffcccccc );
			m_pAfterImage->Disable();
		}
	}
}



#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
/** @function : CreateAndInsertAfterImageByBuff
	@brief : 버프에 의한 AfterImage 실행
	@param : 버프식별자(BuffIdentity_), AfterImage 색깔(d3dxColor_)
*/
void CX2GameUnit::CreateAndInsertWeaponAfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ )
{
	CX2WeaponAfterImageByBuffPtr ptrWeaponAfterImageByBuff( new CX2WeaponAfterImageByBuff( BuffIdentity_, d3dxColor_ ) );
	if ( NULL != ptrWeaponAfterImageByBuff )
		m_vecWeaponAfterImageByBuff.push_back( ptrWeaponAfterImageByBuff );

	EnableWeaponAfterImage();
}



/** @function : EraseAfterImageByBuff
	@brief : 버프에 의한 AfterImage 종료
	@param : 버프식별자(BuffIdentity_)
*/
void CX2GameUnit::EraseWeaponAfterImageByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2WeaponAfterImageByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2WeaponAfterImageByBuffPtr>::iterator, CX2WeaponAfterImageByBuffPtr>( m_vecWeaponAfterImageByBuff, BuffIdentity_ );

	if ( m_vecWeaponAfterImageByBuff.end() != vItr )
	{
		m_vecWeaponAfterImageByBuff.erase( vItr );
		DisableWeaponAfterImage();
	}
}

void CX2GameUnit::EnableWeaponAfterImage()
{
	if ( !m_vecWeaponAfterImageByBuff.empty() )
	{
		D3DXCOLOR cWeaponColor = m_vecWeaponAfterImageByBuff.back()->GetColor();

		PulseWeaponRenderEffect( D3DXCOLOR( cWeaponColor.r, cWeaponColor.g, cWeaponColor.b, 1.f ), 
								 D3DXCOLOR( cWeaponColor.r, cWeaponColor.g, cWeaponColor.b, 0.1f ), 0.5f, 3600.f );
	}
}

void CX2GameUnit::DisableWeaponAfterImage( const bool bForce_ /*= false*/ )
{
	if ( NULL != m_pAfterImage )
	{
		SetEnableWeaponRenderEffect(false);
	}
}
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE



/** @function : CreateAndInsertStunByBuff
	@brief : 그로기 스테이트 고정 생성 및 적용
	@param : 어떤 버프에 의해서 적용 됬는지에 대한 정보
*/
void CX2GameUnit::CreateAndInsertStunByBuff( const KBuffIdentity& BuffIdentity_ )
{
	CX2StunPtr ptrStun = CX2StunPtr( new CX2Stun( BuffIdentity_ ) );
	if ( NULL != ptrStun )
		m_vecStunPtr.push_back( ptrStun );
}

/** @function : EraseStunByBuff
	@brief : 특정버프에 의한 그로기 스테이트 고정 해제
	@param : 특정버프를 구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseStunByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2StunPtr>::iterator vItr 
	= FindSameBuffIdentity<vector<CX2StunPtr>::iterator, CX2StunPtr>( m_vecStunPtr, BuffIdentity_ );

	if ( m_vecStunPtr.end() != vItr )
		m_vecStunPtr.erase( vItr );
}





/** @function : CreateAndInsertFreezeByBuff
	@brief : 특정버프에 의한 고정 적용
	@param : 어떤 버프에 의해서 적용 됬는지에 대한 정보
*/
void CX2GameUnit::CreateAndInsertFreezeByBuff( const KBuffIdentity& BuffIdentity_ )
{
	CX2FreezePtr ptrFreeze = CX2FreezePtr( new CX2Freeze( BuffIdentity_ ) );
	if ( NULL != ptrFreeze )
		m_vecFreezePtr.push_back( ptrFreeze );
}

/** @function : EraseFreezeByBuff
	@brief : 특정버프에 의한 스테이트 고정 해제
	@param : 특정버프를 구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseFreezeByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2FreezePtr>::iterator vItr 
	= FindSameBuffIdentity<vector<CX2FreezePtr>::iterator, CX2FreezePtr>( m_vecFreezePtr, BuffIdentity_ );

	if ( m_vecFreezePtr.end() != vItr )
		m_vecFreezePtr.erase( vItr );
}

/** @function : AddToDamagedMap
	@brief : 자신을 타격한 유저의 UID 및 데미지를 DamagedMap에 저장
	@param : 자신을 타격한 게임유닛(pAttackerGameUnit_, 데미지(fFinalDamage_)
*/
void CX2GameUnit::AddToDamagedMap( const CX2GameUnit* pAttackerGameUnit_, const float fFinalDamage_ )
{
	if ( NULL != pAttackerGameUnit_ )
	{
		pair<map<UidType, float>::iterator, bool> pairReturn = m_DamagedMap.insert( std::make_pair( pAttackerGameUnit_->GetUnitUID(), fFinalDamage_ ) );
		if ( false == pairReturn.second )	// 이미 같은 키를 가진 element가 있다면
			pairReturn.first->second += fFinalDamage_;
	}
}

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void CX2GameUnit::SetToDamagedMap ( const UidType uidType_, const float Damage_ )
{	
	pair<map<UidType, float>::iterator, bool> pairReturn = m_DamagedMap.insert( std::make_pair( uidType_, Damage_ ) );
	if ( false == pairReturn.second )	// 이미 같은 키를 가진 element가 있다면
		pairReturn.first->second = Damage_;		
}
void CX2GameUnit::AddToDamagedMap ( const UidType uidType_, const float Damage_ )
{	
	pair<map<UidType, float>::iterator, bool> pairReturn = m_DamagedMap.insert( std::make_pair( uidType_, Damage_ ) );
	if ( false == pairReturn.second )	// 이미 같은 키를 가진 element가 있다면
		pairReturn.first->second += Damage_;		
}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

/** @function : GetGameUnitUIDMaxDamage
	@brief : 자신에게 제일 데미지를 많이 준 유닛의 UID를 얻어옴
	@return : 유닛 UID
*/
UidType CX2GameUnit::GetGameUnitUIDMaxDamage() const
{
	UidType fMaxDamageKillerUserUnitUID = -1;
	map<UidType,float>::const_iterator iter;
	float fMaxDamage = 0.0f;
	for ( iter = m_DamagedMap.begin(); iter != m_DamagedMap.end(); ++iter )
	{
		if ( fMaxDamage < iter->second )
		{
			fMaxDamage = iter->second;
			fMaxDamageKillerUserUnitUID = iter->first;
		}
	}

	return fMaxDamageKillerUserUnitUID;
}

/*virtual*/ void CX2GameUnit::ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
	pRenderParam_->renderType = m_stOriginBuffRenderParam.m_eRenderType;
	pRenderParam_->cartoonTexType = m_stOriginBuffRenderParam.m_eCartoonTexType;
	pRenderParam_->fOutLineWide = m_stOriginBuffRenderParam.m_fOutLineWide;
	pRenderParam_->outLineColor = m_stOriginBuffRenderParam.m_d3dxColorOutLine;
	pRenderParam_->color	= m_stOriginBuffRenderParam.m_d3dxColor;
	pRenderParam_->bAlphaBlend = m_stOriginBuffRenderParam.m_bAlphaBlend;
}

/** @function : GetBuffFactor
	@brief : 게임 유닛이 걸린 버프 정보를 vector에 담아 알려주는 함수
	@param : 버프정보를 담아낼 컨테이너
*/
void CX2GameUnit::GetBuffFactor( OUT vector<KBuffFactor>& vecBuffFactor_ ) const
{
	BOOST_FOREACH( CX2BuffTempletPtr ptr, m_vecBuffTempletPtr )
	{
		if ( NULL != ptr && false == ptr->GetDidFinish() && false == ptr->IsWorldBuff() )
		{
			vecBuffFactor_.push_back( KBuffFactor() );
			KBuffFactor& kFactor = vecBuffFactor_.back();
			ptr->GetFactor( kFactor, this );
		}
	}
}

/** @function : GetBuffFactorOnlyDebuff
	@brief : 게임 유닛이 걸린 디버프 정보만 vector에 담아 알려주는 함수 (월드 버프 제외)
	@param : 디버프정보를 담아낼 컨테이너
*/
void CX2GameUnit::GetBuffFactorOnlyDebuff( OUT vector<KBuffFactor>& vecBuffFactor_ ) const
{
	BOOST_FOREACH( CX2BuffTempletPtr ptr, m_vecBuffTempletPtr )
	{
		if ( NULL != ptr && false == ptr->GetDidFinish() && ptr->IsDeBuff() && false == ptr->IsWorldBuff() )
		{
			vecBuffFactor_.push_back( KBuffFactor() );
			KBuffFactor& kFactor = vecBuffFactor_.back();
			ptr->GetFactor( kFactor, this );
		}
	}
}

/** @function : IsApplyBuffByBuffFactorID
	@brief : 특정 버프 팩터가 적용되어 있는지 체크
	@param : eBuffFactorID_( 체크할 버프 ID )
*/
bool CX2GameUnit::IsApplyBuffByBuffTempletID( BUFF_TEMPLET_ID eBuffTempletID_ ) const
{
	BOOST_FOREACH( CX2BuffTempletPtr ptr, m_vecBuffTempletPtr )
	{
		if ( NULL != ptr && false == ptr->GetDidFinish() )
		{
			const KBuffIdentity& BuffIdentity = ptr->GetBuffIdentity();
			if( eBuffTempletID_ == BuffIdentity.m_eBuffTempletID )
				return true;
		}
	}

	return false;
}
#ifdef SERV_RENA_NIGHT_WATCHER
void CX2GameUnit::SetStartOfDelayedFiringEmoticon()		/// 지연의 신호탄 이모티콘 설정
{
	CKTDGParticleSystem::CParticleEventSequence* pSeqArrowOfExplosionEmoticon = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqEnchant_ArrowOfExplosion );

	//{{ 오현빈 // 2012-08-20 // 이모티콘 매번 재생성 하지 않고 필요할 때만 지우도록 수정
#ifdef MODIFY_OVERLAP_EDT_EMOTICON
	if( true == GetStartOfDelayedFiringData().empty() )	/// 적용중인 화살이 없다면, 이모티콘 생성하지 않음
	{
		if( NULL !=  pSeqArrowOfExplosionEmoticon )			/// 기존에 있던 이모티콘은 해제
		{
			pSeqArrowOfExplosionEmoticon->SetEmitRate( 0, 0 );
			pSeqArrowOfExplosionEmoticon->ClearAllParticle();
			m_hSeqEnchant_ArrowOfExplosion = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}

		return;
	}
#else
	if( NULL !=  pSeqArrowOfExplosionEmoticon )			/// 기존에 있던 이모티콘은 해제
	{
		pSeqArrowOfExplosionEmoticon->SetEmitRate( 0, 0 );
		pSeqArrowOfExplosionEmoticon->ClearAllParticle();
		m_hSeqEnchant_ArrowOfExplosion = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}

	if( true == GetStartOfDelayedFiringData().empty() )	/// 적용중인 화살이 없다면, 이모티콘 생성하지 않음
		return;
#endif //MODIFY_OVERLAP_EDT_EMOTICON
	//}} 오현빈 // 2012-08-20 // 이모티콘 매번 재생성 하지 않고 필요할 때만 지우도록 수정

	int  iLocalUnitArrowCount	= 0;					/// 로컬 유닛이 적용한 화살 갯수

	for( UINT i = 0; i < m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.size(); ++i)
	{
        CX2DamageManager::StartOfDelayedFiringData* pArrowData = m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData[ i ];
        ASSERT( pArrowData != NULL );

		/// 폭발 예정인 화살 정보는 표시하지 않는다.
		if( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_READY == pArrowData->m_iType )
			continue;

		/// 만약 로컬 유닛의 화살이 적용중이라면, 적용 횟수가 표시된 이모티콘 생성
		CX2GameUnit* pUnit = GetGameUnitByUnitIndex( pArrowData->m_iAttackerIndex );

		if( NULL != pUnit )
		{
			if( true == pUnit->IsLocalUnit() )
				++iLocalUnitArrowCount;
		}
	}

	/// 중첩중인 EDT 횟수 표시 이모티콘
	SetOverlapEDTEmoticon( m_hStartOfDelayedFiringOverlap, iLocalUnitArrowCount );

	//{{ 오현빈 // 2012-08-20 // 이모션 매번 재생성 하지 않고 필요할 때만 지우도록 수정
#ifdef MODIFY_OVERLAP_EDT_EMOTICON
	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqEnchant_ArrowOfExplosion )
#endif //MODIFY_OVERLAP_EDT_EMOTICON
	//}} 오현빈 // 2012-08-20 // 이모션 매번 재생성 하지 않고 필요할 때만 지우도록 수정
	{
		m_hSeqEnchant_ArrowOfExplosion = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*)this,  L"Enchant_Arrow_Of_Explosion_Emoticon", 0, 0, 0 );
		pSeqArrowOfExplosionEmoticon  = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqEnchant_ArrowOfExplosion );

		if( NULL != pSeqArrowOfExplosionEmoticon )
		{
			pSeqArrowOfExplosionEmoticon->SetEmitRate( 20, 40 );
			pSeqArrowOfExplosionEmoticon->SetShowObject( false );
		}
	}
}

void CX2GameUnit::CheckDamageEffectType( CX2DamageEffect::CEffect* pCEffect )	/// DamageEffect에서 Type이 있는 이펙트에 대해 검사
{
	switch( pCEffect->GetType() )
	{
	case CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY:				/// 폭발의 화살이 땅에 적용될 때
		{
			pCEffect->SetType( CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND );
			pCEffect->SetOrderNum( 100 );

			if ( null != pCEffect->GetDamageData().optrAttackerGameUnit )
			{
				CX2GUUser* pAttacker = static_cast<CX2GUUser*>( pCEffect->GetDamageData().optrAttackerGameUnit.GetObservable() );
				pAttacker->UpdateStartOfDelayedFiring( false );					
			}
		} break;

	case CX2DamageEffect::DET_START_OF_DELAYED_FIRING:		/// 지연의 신호탄이 땅에 적용될 때
		{
			pCEffect->SetType( CX2DamageEffect::DET_START_OF_DELAYED_FIRING_GROUND );
		} break;

	case CX2DamageEffect::DET_EXPLOSION_TRAP:
		{
			pCEffect->SetType( CX2DamageEffect::DET_EXPLOSION_TRAP_GROUND );
		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //김창한
	case CX2DamageEffect::DET_TRAPPING_ARROW_BURSTR:
		{
			pCEffect->SetType( CX2DamageEffect::DET_TRAPPING_ARROW_BURSTR_GROUND );
		} break;

	case CX2DamageEffect::DET_TRAPPING_ARROW_FUNGUS:
		{
			pCEffect->SetType( CX2DamageEffect::DET_TRAPPING_ARROW_FUNGUS_GROUND );
		} break;
		
	case CX2DamageEffect::DET_SEED_OF_RUIN:
		{
			pCEffect->SetType( CX2DamageEffect::DET_SEED_OF_RUIN_GROUND );
		} break;

	case CX2DamageEffect::DET_SEED_OF_RUIN_GROUND:
	case CX2DamageEffect::DET_TRAPPING_ARROW_BURSTR_GROUND:
	case CX2DamageEffect::DET_TRAPPING_ARROW_FUNGUS_GROUND:
#endif //UPGRADE_SKILL_SYSTEM_2013
	case CX2DamageEffect::DET_ARROW_OF_EXPLOSION_DELAY_GROUND:
	case CX2DamageEffect::DET_START_OF_DELAYED_FIRING_GROUND:
	case CX2DamageEffect::DET_EXPLOSION_TRAP_GROUND:
		{
			pCEffect->SetType( CX2DamageEffect::DET_NONE );
		} break;
	}
}

CX2GameUnit* CX2GameUnit::GetGameUnitByUnitIndex( int iUnitIndex )	/// UnitIndex에 해당하는 GameUnit 객체를 반환
{
	const vector<CX2GameUnit*>&		   vecUnitList = g_pX2Game->GetUnitList();
	vector<CX2GameUnit*>::const_iterator vIt		   = vecUnitList.begin();

	while( vIt != vecUnitList.end() )
	{
		CX2GameUnit* pUnit = *vIt;

		if( NULL != pUnit && iUnitIndex == pUnit->GetUnitIndex() )
			return pUnit;

		++vIt;
	}

	return NULL;
}

void CX2GameUnit::ClearStartOfDelayedFiringData()		/// 지연의 신호탄 정보 삭제 함수
{
	for( UINT i = 0; i <m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.size(); ++i )
	{
		CX2DamageManager::StartOfDelayedFiringData* pData = m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData[i];
		SAFE_DELETE( pData );
	}
	m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.erase( m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.begin(), 
																				  m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.end() );

	g_pX2Game->GetEffectSet()->StopEffectSet( m_hActiveStartOfDelayedFiringEDT );	/// 지연의 신호탄 적용중 확인 이펙트
}

void CX2GameUnit::SetOverlapEDTEmoticon( CKTDGParticleSystem::CParticleEventSequenceHandle& hEmoticon, int iActiveEDTNum )		/// 중첩된 EDT에 대한 중첩 횟수 이모티콘 설정
{
	CKTDGParticleSystem::CParticleEventSequence* pEmoticon = g_pX2Game->GetMajorParticle()->GetInstanceSequence( hEmoticon );

	if( NULL !=  pEmoticon )			/// 기존에 있던 이모티콘은 해제
	{
		pEmoticon->SetEmitRate( 0, 0 );
		pEmoticon->ClearAllParticle();
		hEmoticon = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}

	if( 0 >= iActiveEDTNum || 10 < iActiveEDTNum )		/// 1 ~ 10 안에 해당하는 중첩수만 표시
		return;

	WCHAR wszControlName[50] = L"";
	StringCchPrintf( wszControlName, ARRAY_SIZE(wszControlName), L"Overlap_EDT_Emoticon_%d", iActiveEDTNum );		/// 적용된 수치에 해당하는 이모티콘 생성

	hEmoticon = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*)this, wszControlName, 0, 0, 0 );
	pEmoticon  = g_pX2Game->GetMajorParticle()->GetInstanceSequence( hEmoticon );

	if( NULL != pEmoticon )
	{
		pEmoticon->SetEmitRate( 20, 40 );
		pEmoticon->SetShowObject(false);
	}
}

void CX2GameUnit::SetExtraDamagePack( ExtraDamagePack *pExtraDamagePack )
{
	if( false == pExtraDamagePack->m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.empty() )
	{
		m_ExtraDamagePack = *pExtraDamagePack;

		m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData.clear();
	}
	else
	{
		m_ExtraDamagePack = *pExtraDamagePack;
	}
}
#endif SERV_RENA_NIGHT_WATCHER

/** @function : GetBuffFinalizerTempletPtrList
	@brief : 걸려있는 버프중 ID에 해당하는 것의 종료조건을 얻어옴
	@param : 종료조건을 얻어올 BuffTempletID_, 종료조건
*/
bool CX2GameUnit::GetBuffFinalizerTempletPtrList( const BUFF_TEMPLET_ID eBuffTempletID_, OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ )
{
	if ( !m_vecBuffTempletPtr.empty() )
	{
		BOOST_FOREACH( CX2BuffTempletPtr ptr, m_vecBuffTempletPtr )
		{
			if ( NULL != ptr && ptr->GetBuffTempletID() == eBuffTempletID_ )
			{
				ptr->GetFinalizerTempletPtrList( vecFinalizerPtr_ );
				return true;
			}
		}
	}
	return false;
}

/** @function : SetFinalizerTempletPtrList
	@brief : 걸려있는 버프중 ID에 해당하는 것의 종료조건을 변경
	@param : 종료조건을 변경할 BuffTempletID_, 종료조건
*/
void CX2GameUnit::ChangeBuffFinalizerTempletPtrList( const BUFF_TEMPLET_ID eBuffTempletID_, OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ )
{
#ifdef FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG
	BOOST_FOREACH( OUT CX2BuffFinalizerTempletPtr& FinalizePtr, vecFinalizerPtr_ )
	{
		FinalizePtr->SetBuffTempletID(eBuffTempletID_);
	}
#endif // FIX_BUFF_FINALIZE_DURATION_TIME_CONTROL_BUG

	if ( !m_vecBuffTempletPtr.empty() )
	{
		BOOST_FOREACH( CX2BuffTempletPtr ptr, m_vecBuffTempletPtr )
		{
			if ( NULL != ptr && ptr->GetBuffTempletID() == eBuffTempletID_ )
			{
				ptr->ChangeFinalizerTempletPtrList( vecFinalizerPtr_ );
				break;
			}
		}
	}
}

/** @function : HaveThisBuff
	@brief : 해당 버프를 가지고 있는지를 조사 하는 함수
	@param : 버프 템플릿 아이디
	@return : 가지고 있으면 true, 없으면 false
*/
bool CX2GameUnit::HaveThisBuff( const BUFF_TEMPLET_ID eBuffTempletID_ )
{
	if ( !m_vecBuffTempletPtr.empty() )
	{
		BOOST_FOREACH( CX2BuffTempletPtr ptr, m_vecBuffTempletPtr )
		{
			if ( NULL != ptr && ptr->GetBuffTempletID() == eBuffTempletID_ )
				return true;
		}
	}

	return false;
}

/** @function : GetShowOnMiniMap
	@brief : 미니맵 상에서 보이는지 여부를 조사하는 함수
	@return : 보이면 true, 안보이면 false
*/
/*virtual*/ bool CX2GameUnit::GetShowOnMiniMap() const
{
	if ( IsCulled() == false &&
		GetNowHp() > 0.0f &&
		GetGameUnitState() != CX2GameUnit::GUSI_DIE &&
		GetShowObject() )
		return true;
	else
		return false;
}

/** @function : CreateAndInsertSkillAndSocketBuffFactorToList
	@brief : 장비, 스킬 등에 의해 남에게 적용할 디버프 또는 버프 등을 셋팅
	@param : 남에게 적용할 버프팩터(BuffFactor_)
*/
void CX2GameUnit::CreateAndInsertSkillAndSocketBuffFactorToList( CX2BuffFactorPtr ptrBuffFactor_ )
{
	CX2BuffFactorPtr ptrBuffFactor = ptrBuffFactor_->GetClonePtr();
	if ( NULL != ptrBuffFactor )
		m_vecBuffFactorPtr.push_back( ptrBuffFactor );
}

/** @function : SetKBuffFactorFromGageManagerToGameUnit
	@brief : GageManager에 임시저장 시킨 BuffFactor를 GameUnit에 적용 하는 함수
*/
void CX2GameUnit::SetKBuffFactorFromGageManagerToGameUnit()
{
	if ( NULL != g_pX2Game && CX2Game::GT_PVP != g_pX2Game->GetGameType() )
	{
		CX2GageManager* pGageManager = CX2GageManager::GetInstance();
		SetKBuffFactorPacketToGameUnit( pGageManager->GetBuffFactor() ); 
		pGageManager->ClearTempBuffFactor();
	}
}


#ifdef MODIFY_SET_DAMAGE_DATA
void CX2GameUnit::SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_, OUT float& fPowerRate)
{		
	if( null == pDamageData_->optrAttackerGameUnit )
		return;

	if ( pDamageData_->optrAttackerGameUnit->ShouldAttackAllTeam() )
			pDamageData_->m_bAttackAllTeam = true;

	if ( pDamageData_->m_bMeleeAttack )	/// 근접 공격으로 간주
		pDamageData_->fForceDownValue *= pDamageData_->optrAttackerGameUnit->GetAdditionalForceDownMultiplierMeleeAttack();
	else	/// 원거리 공격으로 간주
		pDamageData_->fForceDownValue *= pDamageData_->optrAttackerGameUnit->GetAdditionalForceDownMultiplierRangeAttack();

#ifdef BALANCE_PATCH_20120329	//PowerRate가 변경될 수 있으므로, 위에 있던 구문을 이곳으로 이동하여 변경된 PowerRate를 적용

	if( pDamageData_->m_ExtraDamage.m_ExtraDamageType == CX2DamageManager::EDT_PAIN )
		pDamageData_->m_ExtraDamage.m_Damage = fPowerRate;
#endif

	if ( pDamageData_->m_bMeleeAttack )
	{
		pDamageData_->optrAttackerGameUnit->SetBasicStatValueChargeMpHitMeleeAttack( pDamageData_->fHitAddMP );
		pDamageData_->optrAttackerGameUnit->CheckAndUpdateAdditionalChargeMpHitMeleeAttack();
		pDamageData_->fHitAddMP = pDamageData_->optrAttackerGameUnit->GetChargeMpHitMeleeAttack();

	}
	else
	{
		pDamageData_->optrAttackerGameUnit->SetBasicStatValueChargeMpHitRangeAttack( pDamageData_->fHitAddMP );
		pDamageData_->optrAttackerGameUnit->CheckAndUpdateAdditionalChargeMpHitRangeAttack();
		pDamageData_->fHitAddMP = pDamageData_->optrAttackerGameUnit->GetChargeMpHitRangeAttack();
	}

	pDamageData_->damage.fPhysic					*= fPowerRate;
	pDamageData_->damage.fMagic					*= fPowerRate;		

	if( pDamageData_->m_ExtraDamage.m_bFixedDamage == false )
	{
		pDamageData_->m_ExtraDamage.m_DamagePerSec	*= fPowerRate;
		pDamageData_->m_ExtraDamage.m_Damage			*= fPowerRate;
	}	
}
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

float CX2GameUnit::CalcAdditionalAttackDamage  ( const CX2DamageManager::DamageData* pAttackDamageData_ ) const
{
	float fAdditionalAttackValue = GetAdditionalAttack();

	if ( 0.0f < fAdditionalAttackValue )
	{
#ifdef ADJUST_BUFF_CALCULATE
		//크리티컬과의 경쟁에서 밀린다는 판단(기획)으로
		//추가데미지 소켓이 유저 스탯에 영향을 받도록 수정.
		float fStat = 0.0f;
		float fPower = 0.0f;
		switch ( pAttackDamageData_->damageType )
		{
		case CX2DamageManager::DT_PHYSIC:
		case CX2DamageManager::DT_WEAPON_PHYSIC:
			fPower = pAttackDamageData_->damage.fPhysic;
			fStat = GetStat().GetStat().m_fAtkPhysic;
			break;

		case CX2DamageManager::DT_MAGIC:
		case CX2DamageManager::DT_WEAPON_MAGIC:
			fPower = pAttackDamageData_->damage.fMagic;
			fStat = GetStat().GetStat().m_fAtkMagic;
			break;

		case CX2DamageManager::DT_MIX:
		case CX2DamageManager::DT_WEAPON_MIX:
			fPower = ( pAttackDamageData_->damage.fPhysic + pAttackDamageData_->damage.fMagic ) * 0.5f;
			fStat = ( GetStat().GetStat().m_fAtkPhysic + GetStat().GetStat().m_fAtkMagic ) * 0.5f;
			break;

#ifdef ADD_DAMAGE_TYPE_BUFF
		case CX2DamageManager::DT_BUFF:
			break;
#endif //ADD_DAMAGE_TYPE_BUFF

		default:
			ASSERT( !"Worng Path" );
			break;
		}

		if ( g_pX2Game->IsDamageFreeGame() )
			return 1.5f * pow( fPower, 0.65f ) * fAdditionalAttackValue * fStat;
		else if ( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
			return 1.5f * pow( fPower, 0.65f ) * fAdditionalAttackValue * fStat;
		else
			return 1.5f * pow( fPower, 0.65f ) * fAdditionalAttackValue * fStat;
#else //ADJUST_BUFF_CALCULATE
		float fPower = 0.0f;
		switch ( pAttackDamageData_->damageType )
		{
		case CX2DamageManager::DT_PHYSIC:
		case CX2DamageManager::DT_WEAPON_PHYSIC:
			fPower = pAttackDamageData_->damage.fPhysic;
			break;

		case CX2DamageManager::DT_MAGIC:
		case CX2DamageManager::DT_WEAPON_MAGIC:
			fPower = pAttackDamageData_->damage.fMagic;
			break;

		case CX2DamageManager::DT_MIX:
		case CX2DamageManager::DT_WEAPON_MIX:
			fPower = ( pAttackDamageData_->damage.fPhysic + pAttackDamageData_->damage.fMagic ) * 0.5f;
			break;

#ifdef ADD_DAMAGE_TYPE_BUFF
		case CX2DamageManager::DT_BUFF:
			break;
#endif //ADD_DAMAGE_TYPE_BUFF

		default:
			ASSERT( !"Worng Path" );
			break;
		}

		if ( g_pX2Game->IsDamageFreeGame() )
			return 1.5f * pow( fPower, 0.65f ) * fAdditionalAttackValue;
		else if ( CX2Game::GT_PVP == g_pX2Game->GetGameType() )
			return 0.15f * pow( fPower, 0.65f ) * fAdditionalAttackValue;	/// 대전의 경우 1/10 데미지
		else
			return 1.5f * pow( fPower, 0.65f ) * fAdditionalAttackValue;
#endif //ADJUST_BUFF_CALCULATE
	}
	return 0.0f;
}

#endif // FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

#ifdef ADD_UPGRADE_WEAPON_PARTICLE	/// 강화 무기에 대한 이펙트 출력 처리

/// 강화 무기 이펙트 초기화
void CX2GameUnit::Weapon::InitUpgradeWeaponParticle()
{
	if ( GetOwnerUnit().GetGameUnitType() != CX2GameUnit::GUT_USER )
		return;
    CX2Unit* pUnit = static_cast<CX2GUUser&>( GetOwnerUnit() ).GetUnit();
    if ( pUnit == NULL )
        return;

	{
		CX2Item* pItem = pUnit->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );	/// 해당 무기의 아이템 정보

		if( NULL != pUnit->GetUnitTemplet() &&
			NULL != pItem &&
			( (	pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && pItem->GetItemData().m_Endurance > 0) || 
			pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY ) )
		{
			const int			iUpgradeLevel	= pItem->GetItemData().m_EnchantLevel;		/// 현재 무기의 강화 레벨
			CX2Unit::UNIT_TYPE	eUnitType		= pUnit->GetUnitTemplet()->m_UnitType;		/// 현재 유저의 유닛 타입


			if( iUpgradeLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_2 )		/// 11강화 이상
			{
				switch( eUnitType )
				{
				/// 길쭉한 모양의 무기
				case CX2Unit::UT_ELSWORD:
				case CX2Unit::UT_ARME:
				case CX2Unit::UT_RAVEN:
				case CX2Unit::UT_CHUNG:
				case CX2Unit::UT_ARA:
				case CX2Unit::UT_LIRE:
		#ifdef NEW_CHARACTER_EL
				case CX2Unit::UT_ELESIS:
		#endif // NEW_CHARACTER_EL
					{
						CreateUpgradeWeaponParticle( L"11Gang_Weapon_Effect_Lightning", L"11Gang_Weapon_Effect_Lightning02", 
													 L"11Gang_Weapon_Effect_Lightning03", L"11Gang_Weapon_Effect_Lightning04" );
					} break;

				/// 무기가 2개인 이브의 기어
				case CX2Unit::UT_EVE:
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UT_ADD:
#endif //SERV_9TH_NEW_CHARACTER
					{
						CreateUpgradeWeaponParticle( L"11Gang_Weapon_Effect_Lightning02", L"11Gang_Weapon_Effect_Lightning04",
													 L"11Gang_Weapon_Effect_Lightning02", L"11Gang_Weapon_Effect_Lightning04" );
					} break;
				}
			}
		}
	}
}

/// 강화 무기 이펙트 생성
void CX2GameUnit::Weapon::CreateUpgradeWeaponParticle( const WCHAR* pName1, const WCHAR* pName2 /*= NULL*/, const WCHAR* pName3 /*= NULL*/, const WCHAR* pName4 /*= NULL*/ )
{
	if( NULL != g_pX2Game && NULL != g_pX2Game->GetMajorParticle() )
	{
		/// 1번 파티클
		if( NULL != pName1 && m_hSeqUpgradeWeapon == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL, pName1, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon );

			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}

		/// 2번 파티클
		if( NULL != pName2 && m_hSeqUpgradeWeapon2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon2 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL, pName2, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon2 );

			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}

		/// 3번 파티클
		if( NULL != pName3 && m_hSeqUpgradeWeapon3 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon3 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL, pName3, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon3 );

			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}

		/// 4번 파티클
		if( NULL != pName3 && m_hSeqUpgradeWeapon4 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon4 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL, pName3, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon4 );

			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}
}

/// 강화 무기 이펙트 갱신
void CX2GameUnit::Weapon::ProcessUpgradeWeaponParticle()
{
	if( NULL != g_pX2Game && NULL != g_pX2Game->GetMajorParticle() )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon  = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon );
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon2 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon2 );
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon3 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon3 );
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon4 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon4 );

		/// 보이지 말아야 할 때는 숨기자
#ifdef RIDING_SYSTEM
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( false == GetShowObject() ||
			 ( NULL != CX2RidingPetManager::GetInstance() && true == CX2RidingPetManager::GetInstance()->GetRidingOn() ) )
#else RIDING_SYSTEM
		if( false == GetShowObject() )
#endif RIDING_SYSTEM
		{
			if( NULL != pSeqUpgradeWeapon )
				pSeqUpgradeWeapon->SetEmitRate( 0, 0 );

			if( NULL != pSeqUpgradeWeapon2 )
				pSeqUpgradeWeapon2->SetEmitRate( 0, 0 );

			if( NULL != pSeqUpgradeWeapon3 )
				pSeqUpgradeWeapon3->SetEmitRate( 0, 0 );

			if( NULL != pSeqUpgradeWeapon4 )
				pSeqUpgradeWeapon4->SetEmitRate( 0, 0 );

			return;
		}

		/// 유닛 타입에 따른 이펙트 갱신
        if( GetOwnerUnit().GetGameUnitType() == CX2GameUnit::GUT_USER )
        {
            CX2Unit* pUnit = static_cast<CX2GUUser&>(GetOwnerUnit()).GetUnit();
		    if( pUnit != NULL && NULL != pUnit->GetUnitTemplet() )
		    {
			    switch( pUnit->GetUnitTemplet()->m_UnitType )
			    {
			    /// 길쭉한 모양의 무기
			    case CX2Unit::UT_ELSWORD:
			    case CX2Unit::UT_ARME:
			    case CX2Unit::UT_RAVEN:
			    case CX2Unit::UT_CHUNG:
			    case CX2Unit::UT_ELESIS:
				    {
					    if( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL && pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )
					    {
						    D3DXVECTOR3 weaponPos( 0.f, 0.f, 0.f );
						    GetFramePos( &weaponPos, m_pFrame_TRACE_END[0] );

						    D3DXVECTOR3 traceEndPos( 0.f, 0.f, 0.f );
						    GetFramePos( &traceEndPos, m_pFrame_TRACE_START[0] );

						    D3DXVECTOR3 traceCenterPos = traceEndPos - weaponPos;
						    traceCenterPos.x = traceCenterPos.x/2.0f + weaponPos.x;
						    traceCenterPos.y = traceCenterPos.y/2.0f + weaponPos.y;
						    traceCenterPos.z = traceCenterPos.z/2.0f + weaponPos.z;


						    pSeqUpgradeWeapon->SetPosition( traceCenterPos );
						    pSeqUpgradeWeapon2->SetPosition( traceCenterPos );
						    pSeqUpgradeWeapon3->SetPosition( traceCenterPos );
						    pSeqUpgradeWeapon4->SetPosition( traceCenterPos );


						    pSeqUpgradeWeapon->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon2->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon3->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon4->SetEmitRate( 10, 10 );
					    }
				    } break;

			    /// 길이가 긴 아라의 창
			    case CX2Unit::UT_ARA:
				    {
					    if( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL && pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )
					    {
						    D3DXVECTOR3 weaponPos( 0.f, 0.f, 0.f );
						    GetFramePos( &weaponPos, m_pFrame_ATTACK_LINE_END0 );

						    D3DXVECTOR3 traceEndPos( 0.f, 0.f, 0.f );
						    GetFramePos( &traceEndPos, m_pFrame_ATTACK_LINE_START0_FRONT );

						    D3DXVECTOR3 traceCenterPos = traceEndPos - weaponPos;
						    traceCenterPos.x = traceCenterPos.x/2.0f + weaponPos.x;
						    traceCenterPos.y = traceCenterPos.y/2.0f + weaponPos.y;
						    traceCenterPos.z = traceCenterPos.z/2.0f + weaponPos.z;


						    pSeqUpgradeWeapon->SetPosition( traceCenterPos );
						    pSeqUpgradeWeapon2->SetPosition( traceCenterPos );
						    pSeqUpgradeWeapon3->SetPosition( traceCenterPos );
						    pSeqUpgradeWeapon4->SetPosition( traceCenterPos );


						    pSeqUpgradeWeapon->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon2->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon3->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon4->SetEmitRate( 10, 10 );
					    }
				    } break;

			    /// 더미가 하나인 레나의 활
			    case CX2Unit::UT_LIRE:
				    {
					    if( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL && pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )
					    {
						    D3DXVECTOR3 vBonePos1 = GetOwnerUnit().GetBonePos( L"Dummy2_Lhand" );
						    pSeqUpgradeWeapon->SetPosition( vBonePos1 );
						    pSeqUpgradeWeapon2->SetPosition( vBonePos1 );
						    pSeqUpgradeWeapon3->SetPosition( vBonePos1 );
						    pSeqUpgradeWeapon4->SetPosition( vBonePos1 );

						    pSeqUpgradeWeapon->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon2->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon3->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon4->SetEmitRate( 10, 10 );
					    }
				    } break;

			    /// 무기가 2개인 이브의 기어
			    case CX2Unit::UT_EVE:
				    {
					    if ( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL )
					    {
						    D3DXVECTOR3 vBonePos1 = GetOwnerUnit().GetBonePos( L"Dummy1_Rhand" );
						    pSeqUpgradeWeapon->SetPosition( vBonePos1 );
						    pSeqUpgradeWeapon2->SetPosition( vBonePos1 );

						    pSeqUpgradeWeapon->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon2->SetEmitRate( 10, 10 );
					    }

					    if ( pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )
					    {
						    D3DXVECTOR3 vBonePos1 = GetOwnerUnit().GetBonePos( L"Dummy2_Lhand" );
						    pSeqUpgradeWeapon3->SetPosition( vBonePos1 );
						    pSeqUpgradeWeapon4->SetPosition( vBonePos1 );

						    pSeqUpgradeWeapon3->SetEmitRate( 10, 10 );
						    pSeqUpgradeWeapon4->SetEmitRate( 10, 10 );
					    }
				    } break;
	#ifdef SERV_9TH_NEW_CHARACTER // 김태환
				case CX2Unit::UT_ADD:
					{
						if ( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL )
						{
							D3DXVECTOR3 vBonePos1 = GetOwnerUnit().GetBonePos( L"Weapon02" );
							pSeqUpgradeWeapon->SetPosition( vBonePos1 );
							pSeqUpgradeWeapon2->SetPosition( vBonePos1 );

							pSeqUpgradeWeapon->SetEmitRate( 10, 10 );
							pSeqUpgradeWeapon2->SetEmitRate( 10, 10 );
						}

						if ( pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )
						{
							D3DXVECTOR3 vBonePos1 = GetOwnerUnit().GetBonePos( L"Weapon05" );
							pSeqUpgradeWeapon3->SetPosition( vBonePos1 );
							pSeqUpgradeWeapon4->SetPosition( vBonePos1 );

							pSeqUpgradeWeapon3->SetEmitRate( 10, 10 );
							pSeqUpgradeWeapon4->SetEmitRate( 10, 10 );
						}
					} break;
	#endif //SERV_9TH_NEW_CHARACTER
			    }
		    }
        }
	}
}

/// 강화 무기 이펙트 제거
void CX2GameUnit::Weapon::DeleteUpgradeWeaponParticle()
{
	if( NULL != g_pX2Game && NULL != g_pX2Game->GetMajorParticle() )
	{
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon );
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon2 );
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon3 );
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon4 );
	}
}

#endif ADD_UPGRADE_WEAPON_PARTICLE

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE

/** @function : ProcessGroupDamageData
	@brief : 그룹 데미지 데이터 처리 함수
	@param : 프레임당 시간(fElapsedTime_)
*/
void CX2GameUnit::ProcessGroupDamageData( float fElapsedTime_ )
{
	int iCount = 0;

	BOOST_FOREACH( DamageEffectGroupDataPtr pDamageEffectGroupData, m_vecDamageEffectGroupDataPtr )
	{
		if( NULL != pDamageEffectGroupData )
		{
			float& fHitGap = pDamageEffectGroupData->m_fHitGFap;		/// 그룹 데미지 피격 간격

			fHitGap -= fElapsedTime_;		/// 피격 간격 갱신

			if( 0.f >= fHitGap )	/// 만약 피격 간격이 0보다 작다면, 피격 정보 지워주자
			{
				m_vecDamageEffectGroupDataPtr.erase( m_vecDamageEffectGroupDataPtr.begin() + iCount );
			}
				
		}

		++iCount;
	}
}

#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE		/// 데미지 데이터 파싱하는 부분에서는 데미지 배율을 조작하는 동작을 하지 않기에, GetFinalDamage로 이동

/** @function	: GetAdditionalAttackPowerRateByType
	@brief		: 특정 공격 타입에 따른 추가 데미지 배율 설정
	@param		: 공격자의 데미지 데이터
	@return		: 데미지 배율
*/
float CX2GameUnit::GetAdditionalAttackPowerRateByType( CX2DamageManager::DamageData* pDamageData_ )		/// 특정 공격에 따른 데미지 배율 반환 함수 ( AT_SPECIAL, AT_NORMAL )
{
	float fPowerRate = 1.f;

	if ( null != pDamageData_->optrAttackerGameUnit && 
		CX2GameUnit::GUT_USER == pDamageData_->optrAttackerGameUnit->GetGameUnitType() )
	{
		if( pDamageData_->attackType == CX2DamageManager::AT_SPECIAL ) /// 스페셜 액티브 스킬일 때
		{
			CX2GUUser *pAttackerUser = static_cast<CX2GUUser*>( pDamageData_->optrAttackerGameUnit.GetObservable() );

			const float	fSkillDamage  = pAttackerUser->GetSocketData().m_fSkillDamageUpRate;

			fPowerRate *= 1.f + fSkillDamage;

			fPowerRate *= pDamageData_->optrAttackerGameUnit->GetAdditionalAttackByType( CX2DamageManager::AT_SPECIAL );
		}
		else		/// 일반 스킬일 때
		{
			fPowerRate *= pDamageData_->optrAttackerGameUnit->GetAdditionalAttackByType( CX2DamageManager::AT_NORMAL );
		}
	}

	return fPowerRate;
}
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜 : 2013-04-09		생성 간격 추가

/** @function : CreateAndInsertEffectSetNormalAttackByBuff
	@brief : 일반 공격 타격시 타격점에 출력할 이펙트셋을 지정
	@param : 해당 이펙트셋을 출력 시키는 버프의 식별자(BuffIdentity_), 이펙트셋이름(pwszEffectSetName_), 사운드이름(pwszSoundName_)
*/
void CX2GameUnit::CreateAndInsertEffectSetNormalAttackByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCoolTime )
{
	CX2EffectSetImpactPointByBuffPtr ptrBuffEffectSet= CX2EffectSetImpactPointByBuffPtr( new CX2EffectSetImpactPointByBuff( BuffIdentity_, pwszEffectSetName_, pwszSoundName_, fCoolTime ) );
	if ( NULL != ptrBuffEffectSet )
		m_vecEffectSetNormalAttackByBuffPtr.push_back( ptrBuffEffectSet );
}

/** @function : EraseEffectSetNormalAttackByBuff
	@brief : 일반 공격 타격시 이펙트셋이 지정된것을 삭제
	@param : 해당 이펙트셋을 출력 시키는 버프의 식별자(BuffIdentity_)
*/
void CX2GameUnit::EraseEffectSetNormalAttackByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2EffectSetImpactPointByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2EffectSetImpactPointByBuffPtr>::iterator, CX2EffectSetImpactPointByBuffPtr>( m_vecEffectSetNormalAttackByBuffPtr, BuffIdentity_ );

	if ( m_vecEffectSetNormalAttackByBuffPtr.end() != vItr )
		m_vecEffectSetNormalAttackByBuffPtr.erase( vItr );
}

/** @function : PlayEffectSetNormalAttackByBuff
	@brief : 일반 공격 타격시 타격점에 리스트에 있는 이펙트셋들을 출력
	@param : 공격 타입
*/
void CX2GameUnit::PlayEffectSetNormalAttackByBuff( CX2DamageManager::ATTACK_TYPE eAttackType )
{
	if ( !m_vecEffectSetNormalAttackByBuffPtr.empty() )
	{
		BOOST_FOREACH( CX2EffectSetImpactPointByBuffPtr ptr, m_vecEffectSetNormalAttackByBuffPtr )
		{
			if ( NULL != ptr )
				ptr->PlayEffectSetNormalAttackByBuff( this, eAttackType );
		}
	}
}

#endif // SERV_NEW_DEFENCE_DUNGEON

/** @function : CreateAndInsertAddSkillLevelByBuff
	@brief : 버프에 의한 스킬 레벨 증감을 게임유닛에 적용하는 함수
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_), 흡수시 증가되는 MP량(fAbsorbValue_)
*/
void CX2GameUnit::CreateAndInsertAddSkillLevelByBuff( const KBuffIdentity& BuffIdentity_, const int iAddSkillLevel_ )
{
	CX2AddSkillLevelByBuffPtr ptrAddSkillLevelByBuff = CX2AddSkillLevelByBuffPtr( new CX2AddSkillLevelByBuff( BuffIdentity_, iAddSkillLevel_ ) );
	if ( NULL != ptrAddSkillLevelByBuff )
		m_vecAddSkillLevelByBuffPtr.push_back( ptrAddSkillLevelByBuff );

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if ( GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
        CX2Unit* pUnit = static_cast<CX2GUUser*>( this )->GetUnit();
        if ( pUnit != NULL )
		/// 스킬 레벨 증가 버프에 대한 효과 설정
		    pUnit->ResetIncreaseSkillLevelByBuff();
	}
#endif // UPGRADE_SKILL_SYSTEM_2013
}

/** @function : EraseAddSkillLevelByBuff
	@brief : 스킬레벨 증감 버프 요소 해제
	@param : 스킬레벨 증감 버프 요소를구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseAddSkillLevelByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2AddSkillLevelByBuffPtr>::iterator vItr 
	= FindSameBuffIdentity<vector<CX2AddSkillLevelByBuffPtr>::iterator, CX2AddSkillLevelByBuffPtr>( m_vecAddSkillLevelByBuffPtr, BuffIdentity_ );

	if ( m_vecAddSkillLevelByBuffPtr.end() != vItr )
		m_vecAddSkillLevelByBuffPtr.erase( vItr );

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if ( GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
        CX2Unit* pUnit = static_cast<CX2GUUser*>( this )->GetUnit();
        if ( pUnit != NULL )
		/// 스킬 레벨 증가 버프에 대한 효과 설정
		    pUnit->ResetIncreaseSkillLevelByBuff();
	}
#endif // UPGRADE_SKILL_SYSTEM_2013
}

/** @function : GetAddSkillLevelByBuff
	@brief : 버프에 의해 증감할 스킬 레벨을 얻어 오는 함수
	@return : 버프에의해 증감 할 스킬 레벨
*/
int CX2GameUnit::GetAddSkillLevelByBuff() const
{
	int iSumOfAddSkillLevelByBuff = 0;

	BOOST_FOREACH( CX2AddSkillLevelByBuffPtr addSkillLevelByBuffPtr, m_vecAddSkillLevelByBuffPtr )	/// 찾고자 하는 공격 타입에 해당하는 배율 검사
	{
		if ( NULL != addSkillLevelByBuffPtr )
			iSumOfAddSkillLevelByBuff += addSkillLevelByBuffPtr->GetAddSkillLevel();
	}

	return iSumOfAddSkillLevelByBuff;
}

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-17

/** @function	: ShowIncreaseValue
	@brief		: HP 및 MP 변동치 출력 함수
	@param		: 위치, 출력할 수치 선택, 값
*/
void CX2GameUnit::ShowIncreaseValue( D3DXVECTOR3 vecPos, bool IsHP, float fValue )		/// HP 및 MP 변동치 출력 함수
{
	WCHAR wszText[64]	= L"";
	vecPos.y			+= 30.0f;

	if( true == IsHP )
	{
		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"HP+ %d", static_cast<int>( fValue ) );

		if( NULL != g_pData && NULL != g_pData->GetPicCharGreen() )
			g_pData->GetPicCharGreen()->DrawText( wszText, vecPos, GetDirVector(), CKTDGPicChar::AT_CENTER );
	}
	else
	{
		StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", static_cast<int>( fValue ) );

		if( NULL != g_pData && NULL != g_pData->GetPicCharGreen() )
			g_pData->GetPicCharBlue()->DrawText(wszText, vecPos, GetDirVector(), CKTDGPicChar::AT_CENTER );
	}
}

#endif ADD_HALLOWEEN_PET_SYSTEM


#ifdef VERIFY_STAT_BY_BUFF
bool CX2GameUnit::VerifyStatByBuff()
{
	if ( m_AdditionalMaxHp.Verify()
		 && m_AdditionalPhysicAttack.Verify()
		 && m_AdditionalMagicAttack.Verify()
		 && m_AdditionalPhysicDefence.Verify()
		 && m_AdditionalMagicDefence.Verify()
		 && m_AdditionalAnimSpeed.Verify()
		 && m_AdditionalWalkSpeed.Verify()
		 && m_AdditionalDashSpeed.Verify()
		 && m_AdditionalJumpPower.Verify()
		 && m_AdditionalDashJumpPower.Verify()
		 && m_AdditionalAccuracy.Verify()
		 && m_AdditionalAvoidance.Verify()
		 && m_AdditionalCriticalRate.Verify()
		 && m_AdditionalCriticalAttack.Verify()
		 && m_AdditionalAntiCriticalRate.Verify()
		 && m_AdditionalCriticalDefence.Verify()
		 && m_AdditionalChangeAdditionalAttack.Verify()
		 && m_AdditionalChangeAdditionalDefence.Verify()
		 && m_AdditionalChargeMpHitMeleeAttack.Verify()
		 && m_AdditionalChargeMpHitRangeAttack.Verify()
		 && m_AdditionalChargeMpHitted.Verify()
		 && m_AdditionalFinalDamageMultiplierMeleeAttack.Verify()
		 && m_AdditionalFinalDamageMultiplierRangeAttack.Verify()
		 && m_AdditionalFinalDefenceMultiplier.Verify()
		 && m_AddtionalForceDownMultiplierMeleeAttack.Verify()
		 && m_AddtionalForceDownMultiplierRangeAttack.Verify()
		 && m_AdditionalChangeAttributeBlazeDefence.Verify()
		 && m_AdditionalChangeAttributeWaterDefence.Verify()
		 && m_AdditionalChangeAttributeNatureDefence.Verify()
		 && m_AdditionalChangeAttributeWindDefence.Verify()
		 && m_AdditionalChangeAttributeLightDefence.Verify()
		 && m_AdditionalChangeAttributeDarkDefence.Verify()
		 && m_AdditionalDrainHpNormalAttack.Verify()
		 && m_AdditionalMaxMp.Verify()
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		 && m_AdditionalHyperCharge.Verify()
#endif //SERV_ADD_LUNATIC_PSYKER
		 )
		 return true;
	else
		return false;
}
#endif // VERIFY_STAT_BY_BUFF

#ifdef RIDING_SYSTEM
bool CX2GameUnit::CanNotShowEnchantedWeaponEffectToRiding()
{
	if ( NULL != CX2RidingPetManager::GetInstance() && true != CX2RidingPetManager::GetInstance()->GetRidingOn() )
		return true;
	else
		return false;
}

/*virtual*/const D3DXMATRIX* CX2GameUnit::GetCombineMatrixFromRidingPetBoneName( const wstring& wstrBoneName )
{
	if ( true == wstrBoneName.empty() )
		return NULL;

	if ( false == GetRidingOn() )
		return NULL;

	if ( NULL == GetRidingPet() )
		return NULL;

	CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = GetRidingPet()->GetXSkinAnimPtr()->GetCloneFrame( wstrBoneName.c_str() );
	if ( NULL == pMultiAnimFrame )
		return NULL;

	return &(pMultiAnimFrame->combineMatrix);
}
#endif //RIDING_SYSTEM

#ifdef MODIFY_EFFECT_SET_DELETE
#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
void CX2GameUnit::AddEffectSetDeleteCondition( CX2EffectSet::Handle hEffectSetHandle_, const vector<DELETE_CONDITION_EFFECT_SET> & vecDeleteCondition )
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
void CX2GameUnit::AddEffectSetDeleteCondition( CX2EffectSet::Handle hEffectSetHandle_, DELETE_CONDITION_EFFECT_SET eDeleteCondition )
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
{
#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
	BOOST_FOREACH ( DELETE_CONDITION_EFFECT_SET eDeleteCondition, vecDeleteCondition )
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // 김종훈, EFFECT_SET_LIST_EX 테이블의 종료 조건 추가 확장
	{
		switch ( eDeleteCondition )
		{
		case DCES_DAMAGE_REACT:
			{
				m_vecEffectSetToDeleteOnDamageReact.push_back( hEffectSetHandle_ );
			} break;
		case DCES_STATE_END:
			{
				m_vecEffectSetToDeleteOnStateEnd.push_back( hEffectSetHandle_ );
			} break;
		case DCES_DIE:
			{
				m_vecEffectSetToDeleteOnDie.push_back( hEffectSetHandle_ );
			} break;
		case DCES_CUSTOM_STATE:
			{
	#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
				m_vecEffectSetToDeleteOnCustomState.push_back( hEffectSetHandle_ );
	#endif // DELETE_EFFECTSET_ON_CUSTOM_STATE
			} break;
		default:
			break;
		}
	}
}
#endif //MODIFY_EFFECT_SET_DELETE 

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
/** @function : CreateAndInsertChangeAttackByType
	@brief : 공격 타입에 따른 데미지 변경 버프요소를 게임유닛에 적용하는 함수
	@param : 어떤 버프에 의한 것인지 구분(BuffIdentity_), 흡수시 증가되는 MP량(fAbsorbValue_)
*/
void CX2GameUnit::CreateAndInsertReflectMagicByBuff( const KBuffIdentity& BuffIdentity_, float fValue_ )
{
	CX2ReflectMagicByBuffPtr ptrReflectMagic = CX2ReflectMagicByBuffPtr( new CX2ReflectMagicByBuff( BuffIdentity_, fValue_ ) );
	if ( NULL != ptrReflectMagic )
		m_vecReflectMagicByBuffPtr.push_back( ptrReflectMagic );
}

/** @function : EraseChangeAttackByType
	@brief : 공격 타입에 따른 데미지 변경 버프 요소 해제
	@param : 공격 타입에 따른 데미지 변경 버프 요소를구분할 수 있는 정보(BuffIdentity_)
*/
void CX2GameUnit::EraseReflectMagicByBuff( const KBuffIdentity& BuffIdentity_ )
{
	vector<CX2ReflectMagicByBuffPtr>::iterator vItr 
		= FindSameBuffIdentity<vector<CX2ReflectMagicByBuffPtr>::iterator, CX2ReflectMagicByBuffPtr>( m_vecReflectMagicByBuffPtr, BuffIdentity_ );

	if ( m_vecReflectMagicByBuffPtr.end() != vItr )
		m_vecReflectMagicByBuffPtr.erase( vItr );
}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

/** @function : PostProcessEnchantDamage
	@brief : EDT 적용 후 처리해야 하는 내용
*/
/*virtual*/	void CX2GameUnit::PostProcessEnchantDamage( OUT CX2DamageManager::DamageData* pDamageData_ )
{
	if( NULL != pDamageData_ )
	{
		// 오현빈 // 2013-09-23 // 연타 공격 일 때 피어싱 효과가 확률과 상관없이 적용 되는 문제 수정
		pDamageData_->m_bPenetrate = false;
	}
}

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
/** @function : GetExtraDamageType
	@brief : 현재 자신에게 걸린 무기 인챈트 타입을 반환. 제일 시간이 긴 것을 반환한다.
	@param : -
*/
const CX2DamageManager::EXTRA_DAMAGE_TYPE		CX2GameUnit::GetApplyWeaponExtraDamageType()
{
	float fCheckTime = 0.f;
	CX2DamageManager::EXTRA_DAMAGE_TYPE eEDTType = CX2DamageManager::EDT_NONE;

	if( m_ExtraDamagePack.m_EnchantBlaze.m_fTime > fCheckTime )
	{
		eEDTType = CX2DamageManager::EDT_ENCHANT_BLAZE;
		fCheckTime = m_ExtraDamagePack.m_EnchantBlaze.m_fTime;
	}

	if( m_ExtraDamagePack.m_EnchantFrozen.m_fTime > fCheckTime )
	{
		eEDTType = CX2DamageManager::EDT_ENCHANT_FROZEN;
		fCheckTime = m_ExtraDamagePack.m_EnchantFrozen.m_fTime;
	}

	if( m_ExtraDamagePack.m_EnchantPoison.m_fTime > fCheckTime )
	{
		eEDTType = CX2DamageManager::EDT_ENCHANT_POISON;
		fCheckTime = m_ExtraDamagePack.m_EnchantPoison.m_fTime;
	}
	
	if( m_ExtraDamagePack.m_EnchantShock.m_fTime > fCheckTime )
	{
		eEDTType = CX2DamageManager::EDT_ENCHANT_SHOCK;
	}

	return eEDTType;
}
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

bool CX2GameUnit::IsCanNotIntput() const 
{ 
	if( 0.0f < m_fCanNotInputTime || true == m_bCanNotInput ) 
		return true; 
	
	return false;
}


#ifdef WRITE_SCRIPT_LOG_AT_CONSOLE_WINDOW  // 김태환
void CX2GameUnit::WriteLogByConsoleWindow_LUA( const char* wstrLog_ ) 
{
	wstring wstrStateName = L"";
	wstrStateName += L"\n";

	ConvertUtf8ToWCHAR( wstrStateName, wstrLog_ );

	wstrStateName += L"\n";

	dbg::clog << wstrStateName << dbg::endl;
}
#endif // WRITE_SCRIPT_LOG_AT_CONSOLE_WINDOW

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
/** @function	: IsRightTarget
	@brief		: 특정위치가 나의 오른쪽인지 왼쪽인지 판별한다.
	@param		: 대상 위치
	@return		: 위치 여부
*/
bool CX2GameUnit::IsRightTarget(D3DXVECTOR3 vPos)
{
	bool bRight = false;	

	if(g_pX2Game == NULL || g_pX2Game->GetWorld() == NULL || g_pX2Game->GetWorld()->GetLineMap() == NULL)
		return true;

	const CKTDGLineMap::LineData* pCurrLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( GetLastTouchLineIndex( true ) );

	// pCurrLineData가 NULL일경우 처리는....????
	if(pCurrLineData == NULL)
		return false;			

	D3DXVECTOR3 vFinalDestDir = vPos - GetPos();
	float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
	D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
	const D3DXVECTOR3 vUpVec(0,1,0);
	D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
	D3DXVec3Normalize( &vRightVec, &vRightVec );

	float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
	float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
	if( fIsRight > 0.f )
		bRight = true;
	else
		bRight = false;

	return bRight;
}
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
/** @function	: GetFlickerColor
	@brief		: 버프를 통해반짝이는 렌더 컬러값 연산 함수
	@param		: 버프에 설정된 색, 반짝이는 간격
	@return		: 랜더에 적용할 색
*/
const D3DXCOLOR CX2GameUnit::GetFlickerColorByBuff( IN const D3DXCOLOR& coRenderColor_, IN const float fFlickerGap_ )
{
	D3DXCOLOR coResultColor = coRenderColor_;

	if ( 0.f < coResultColor )
	{
		/// 반짝이는 시간 갱신
		m_fFlickerElapsedTime += m_fElapsedTime;

		if ( m_fFlickerElapsedTime >= fFlickerGap_ )
			m_fFlickerElapsedTime = 0.f;

		/// 반짝이는 간격에 대한 진행률
		float fFlickerRate = m_fFlickerElapsedTime / fFlickerGap_;

		/// 진행률이 절반 이상이면, 색상 감소
		if ( ( fFlickerGap_ / 2 ) < m_fFlickerElapsedTime )
		{
			fFlickerRate = ( fFlickerRate - 0.5f ) * 2.f;	/// 0 ~ 1.f 로 진행률 변환

			coResultColor.r = max( 1.f - ( ( 1.f - coResultColor.r ) * fFlickerRate ), coResultColor.r );
			coResultColor.g = max( 1.f - ( ( 1.f - coResultColor.g ) * fFlickerRate ), coResultColor.g );
			coResultColor.b = max( 1.f - ( ( 1.f - coResultColor.b ) * fFlickerRate ), coResultColor.b );
		}
		///  진행률이 절반 이하면, 색상 증가
		else
		{
			fFlickerRate *= 2.f;	/// 0 ~ 1.f 로 진행률 변환

			coResultColor.r = min( coResultColor.r + ( ( 1.f - coResultColor.r ) * fFlickerRate ), 1.f );
			coResultColor.g = min( coResultColor.g + ( ( 1.f - coResultColor.g ) * fFlickerRate ), 1.f );
			coResultColor.b = min( coResultColor.b + ( ( 1.f - coResultColor.b ) * fFlickerRate ), 1.f );
		}
	}

	return coResultColor;
}
#endif //SERV_ADD_LUNATIC_PSYKER
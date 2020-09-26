#include "StdAfx.h"
#include ".\x2guarme_violetmage.h"


#ifdef SERV_APRIL_FOOLS_DAY
static const CX2GUUser::SkillCutIn s_SkillCutInSetFoolsDay = CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_Gura.dds");
#endif //APRIL_FOOLS_DAY

// 필살기 쓸 때 cut in image 
static const CX2GUUser::SkillCutInSet s_SkillCutInSet[] = 
{
	// 아이샤, 하이매지션, 다크매지션
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_1.dds"),// D3DXVECTOR2(619.0f,619.0f), D3DXVECTOR3(176.0f,497.0f,0.0f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_2.dds"),// D3DXVECTOR2(674.0f,674.0f), D3DXVECTOR3(155.0f,523.0f,0.0f) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_3.dds"),// D3DXVECTOR2(670.0f,670.0f), D3DXVECTOR3(275.0f,471.0f,0.0f) ),
	},


	// elemental master
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_EM_1.dds"),// D3DXVECTOR2( 663.f, 663.f ), D3DXVECTOR3( 319.f, 459.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_EM_1.dds"),// D3DXVECTOR2( 663.f, 663.f ), D3DXVECTOR3( 319.f, 459.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_EM_1.dds"),// D3DXVECTOR2( 663.f, 663.f ), D3DXVECTOR3( 319.f, 459.f, 0.f ) ),
	},

	// void princess
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_VP_1.dds"),// D3DXVECTOR2( 722.f, 722.f ), D3DXVECTOR3( 353.f, 420.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_VP_1.dds"),// D3DXVECTOR2( 722.f, 722.f ), D3DXVECTOR3( 353.f, 420.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_VP_1.dds"),// D3DXVECTOR2( 722.f, 722.f ), D3DXVECTOR3( 353.f, 420.f, 0.f ) ),
	},

#ifdef SERV_ARME_DIMENSION_WITCH
	{
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_DW_1.dds"),//D3DXVECTOR2( 722.f, 722.f ), D3DXVECTOR3( 353.f, 420.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_DW_1.dds"),//D3DXVECTOR2( 722.f, 722.f ), D3DXVECTOR3( 353.f, 420.f, 0.f ) ),
		CX2GUUser::SkillCutIn( L"HQ_CutIn_Aisha_DW_1.dds"),//D3DXVECTOR2( 722.f, 722.f ), D3DXVECTOR3( 353.f, 420.f, 0.f ) ),
	},
#endif
};







namespace _CONST_AISHA_
{

	const std::string UFO_IDLE			= "UFO_AISHA_IDLE";		
	const std::string UFO_GO_LEFT		= "UFO_AISHA_GO_LEFT";		
	const std::string UFO_GO_RIGHT 	= "UFO_AISHA_GO_RIGHT";	
	const std::string UFO_GO_UP		= "UFO_AISHA_GO_UP";		
	const std::string UFO_GO_DOWN		= "UFO_AISHA_GO_DOWN";		

	const std::string UFO_GO_UP_LEFT		= "UFO_AISHA_GO_UP_LEFT";		
	const std::string UFO_GO_UP_RIGHT 		= "UFO_AISHA_GO_UP_RIGHT";	
	const std::string UFO_GO_DOWN_LEFT		= "UFO_AISHA_GO_DOWN_LEFT";		
	const std::string UFO_GO_DOWN_RIGHT 	= "UFO_AISHA_GO_DOWN_RIGHT";	

	const std::string UFO_FIRE			= "UFO_AISHA_FIRE";
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 헬 드롭용 사망 State Parsing 을 위해 추가
	const std::string UFO_DYING		= "UFO_AISHA_DYING";
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편


	const D3DXCOLOR coSpecialActive( 0xff222222 );
	const D3DXCOLOR coSpecialActiveHyper( 0xffff0000 );

	const float AISHA_INITIAL_MP( 50.f ); 

	const float AISHA_LEVITATION_CONSUME_MP_TIME( 0.2f ); 
	const float MAGICAL_MAKEUP_TRANSFORMING_TIME = 1.3f;	/// 매지컬메이크업 변신동작을 수행하는 시간

#ifdef BALANCE_ELEMENTAL_MASTER_20130117
	const float AEM_COMBOX4_MAX_CHARGING_TIME( 2.f );					/// 4번째 파이어볼 최대 차지 시간
	const float AEM_METEO_SHOWER_USE_MP( 5.f );							/// 메테오 샤워 1발당 소모 MP량
	const float AEM_MEDITATION_REDUCE_COOLTIME_VALUE( 15.f );			/// 메디테이션 메모 쿨타임 감소치
	const float AHM_CHAIN_BURST_MEMO_INCREASE_POWER_RATE_VALUE( 1.1f );	/// 체인 버스트 메모 데미지 증감치
#endif BALANCE_ELEMENTAL_MASTER_20130117

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	const float CREATE_HELL_DROP_DAMAGE_EFFECT_TIME_GAP ( 0.25f );		/// 헬 드롭, 떨어지는 시간 차
	const float CREATE_HELL_DROP_DAMAGE_EFFECT_START_TIME ( 1.0f );		/// 헬 드롭, 떨어지기 시작하는 시간 
	const float BIDING_CIRCLE_BASIC_DISTANCE ( 500.f );					/// 바인딩 서클, 기본 범위 ( 레벨 1 범위 )
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
}





CX2GUArme_VioletMage::CX2GUArme_VioletMage( int unitIndex, int teamNum, 
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
#ifdef AISHA_SECOND_CLASS_CHANGE
, m_optrUFOGameUnit()
, m_bIsSummoningUFO( false )
#endif AISHA_SECOND_CLASS_CHANGE
, m_bElementalFriendShip( false )
, m_pStrongMindData( NULL )
, m_ptrTransformWeapon( CX2EqipPtr() )
, m_fTransformTime( 0.0f )
#ifdef SERV_ARME_DIMENSION_WITCH
	#ifndef MODIFY_DISTORTION_DESTRUCTION
		, m_fDistortionLifeTime( 0.f )
	#endif // MODIFY_DISTORTION_DESTRUCTION
#endif
, m_fLevitationElapsedTime( 0.f )
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
, m_bCanChangeState( false )		/// 특정 스테이트로 넘어갈 수 있는지 저장하는 변수
, m_fPushXChargeTime( 0.f )			/// 4번째 파이어볼 차지 시간
, m_iMaxFireCount( 10 )				/// 메테오 샤워 스테이트에서 최대 발사할 수 있는 개수 저장
, m_iMaxFireCountByLoopState( 10 )	/// 메테오 샤워 루프 스테이트에서 최대 발사할 수 있는 개수 저장
, m_iElapsedFireCount( 0 )			/// 메테오 샤워 루프 스테이트에서 누적된 발사량 저장
, m_fUseMPConsume( 5.f )			/// 메테오 샤워 루프 스테이트에서 한발당 소모되는 MP량
, m_fFireGap( 0.2f )				/// 메테오 샤워 한발당 발사되는 간격
#endif BALANCE_ELEMENTAL_MASTER_20130117
#ifdef BALANCE_PATCH_20131107				// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
, m_fHellDropCreateEffectCooltime ( 0.f )		/// 헬 드롭, 생성한 Effect 갯수
, m_bIsHellDropTimerStart ( false )				// 헬 드롭 타이머가 켜졌는가?
#endif // BALANCE_PATCH_20131107			// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

{
	for( int i=0; i<EffSetID_END; i++ )
	{
		m_ahEffectSet[i] = INVALID_EFFECTSET_HANDLE;
	}


	m_hSeqArmeAirStepRight	= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqArmeAirStepLeft	= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_pMPChargeA			= NULL;
	m_pMPChargeB			= NULL;
	m_pMPChargeC			= NULL;

	m_pEvasionAttackAnim	= NULL;

	m_hSeqMPEnergy				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPEnergyCenter		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPChargeWave			= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hSeqMPCharge2MagicCircle	= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPCharge2Up			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqMPCharge2Up2			= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_pComboX3Up1			= NULL;
	m_pComboX3Up2			= NULL;
	m_pComboX3Up3			= NULL;


#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
	m_pLightningBolt		= NULL;
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST


	//m_pMeteoCall			= NULL;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_hHellStoneEffect		    = INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser			= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser1		= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser2		= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser3		= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser4		= INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pHellStoneEffect		= NULL;
	m_pCatastropheLaser			= NULL;
	m_pCatastropheLaser1		= NULL;
	m_pCatastropheLaser2		= NULL;
	m_pCatastropheLaser3		= NULL;
	m_pCatastropheLaser4		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	m_bZ4up					= false;



	InitializeArmeMajorParticleArray();
	InitializeArmeMinorParticleArray();

	//메이져
	
#ifdef SKILL_CASH_10_TEST
	m_hSeqLight_Arme_Aging1				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqLight_Arme_Aging2				= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqLight_Arme_Aging3				= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif


	//마이너
	m_fMPConsumeRateTeleport 	= 1.f;
	m_fMPConsumeRateFireBall 	= 1.f;
	m_fFireBallRangeRate		= 1.f;	
	m_bInMeditation				= false;
	m_fMeditationMPChangeRate	= 0.f;
	m_fAddMPChangeRate			= 0.f;

#ifdef NEW_SKILL_2010_11
	m_iHyperChargeMP = 0.f;
	m_iHyperAddMP = 0.f;
	#ifndef FIX_LIMITED_MANA_MANAGEMENT
	m_bLimitManaManagement = false;
	#endif //FIX_LIMITED_MANA_MANAGEMENT
	m_fRateLimitManaManagement = 0.f;
	m_iHyperChargeMP = 100.f;
#endif NEW_SKILL_2010_11

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	m_iDrainEnergyTarget = -1;	
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	m_hEffectMorningStar = INVALID_EFFECTSET_HANDLE;
	m_fMagicStaffExtraRate = 0.f;
	m_fMagicStaffSize = 0.f;
	m_hMagicalStaff	= INVALID_MESH_INSTANCE_HANDLE;
#endif

#ifdef SPECIAL_USE_ITEM
	m_fThrowStartTime		= 0.6666f;
	m_vThrowStartPosOffset	= D3DXVECTOR3(40.f, 130.f, 0.f);
	m_vThrowStartVelocity	= D3DXVECTOR3(1600.f, 200.f, 0.f);
#endif SPECIAL_USE_ITEM

	m_bDashJumpZ = false;
#ifdef NEW_SKILL_2010_11
	m_bIsOnSomethingStart = false;
#endif
	m_fMPChargeSoundTime = 0.f;
	m_fMPChargeWaitTime = 0.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	m_bIceBall = false;
	m_fIceBallPower = 1.f;
	m_fElementalResearchRate = 0.f;
	m_bResearchGravityBall = false;
	m_fGravityBallPower = 1.f;
#endif //UPGRADE_SKILL_SYSTEM_2013
}

CX2GUArme_VioletMage::~CX2GUArme_VioletMage(void)
{
	SAFE_DELETE( m_pStrongMindData );

	if( NULL != g_pX2Game )
	{
		BOOST_TEST_FOREACH( StoneWallData&, stoneWallData, m_vecStoneWallData )
		{
			CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( (int) stoneWallData.m_NPCUID );
			if( NULL != pNPC )
			{
				pNPC->SetNowHp( 0.f );
			}
		}
	}
	

	for( int i=0; i<EffSetID_END; i++ )
	{
		g_pX2Game->GetEffectSet()->StopEffectSet( m_ahEffectSet[i] );
	}
	

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqArmeAirStepRight );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqArmeAirStepLeft );



	SAFE_DELETE_KTDGOBJECT( m_pMPChargeA );
	SAFE_DELETE_KTDGOBJECT( m_pMPChargeB );
	SAFE_DELETE_KTDGOBJECT( m_pMPChargeC );



#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	SAFE_DELETE_KTDGOBJECT( m_pLightningBolt );
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST
	
	//SAFE_DELETE_KTDGOBJECT( m_pMeteoCall );
	SAFE_DELETE_KTDGOBJECT( m_pEvasionAttackAnim );



	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPEnergy );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPEnergyCenter );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPChargeWave );

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPCharge2MagicCircle );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPCharge2Up );
	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqMPCharge2Up2 );

	
	SAFE_DELETE_KTDGOBJECT( m_pComboX3Up1 );
	SAFE_DELETE_KTDGOBJECT( m_pComboX3Up2 );
	SAFE_DELETE_KTDGOBJECT( m_pComboX3Up3 );


	DeleteArmeMajorParticle();
	DeleteArmeMinorParticle();

	
	
#ifdef SKILL_CASH_10_TEST
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqLight_Arme_Aging1 );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqLight_Arme_Aging2 );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeqLight_Arme_Aging3 );
#endif

#ifdef SERV_ARME_DIMENSION_WITCH
	if( m_hMagicalStaff != INVALID_MESH_INSTANCE_HANDLE )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMagicalStaff );
		m_SubAttackListSet.clear();		
	}
#endif

	g_pX2Game->GetMinorParticle()->DestroyInstanceHandle( m_hSeqLight_Arme_Circle_Flame	);	
}


void CX2GUArme_VioletMage::ReInit( bool bRandom, int startPosIndex )
{
	CX2GUUser::ReInit( bRandom, startPosIndex );
	

	if( GetNowMp() < _CONST_AISHA_::AISHA_INITIAL_MP )
		SetNowMp( _CONST_AISHA_::AISHA_INITIAL_MP );
	
	m_bZ4up					= false;
}

void CX2GUArme_VioletMage::DamageReact( CX2DamageManager::DamageData* pDamageData )
{
	KTDXPROFILE();	
	CX2GUUser::DamageReact( pDamageData );
	CX2GUUser::DamageReactStateChange( pDamageData );
	
#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 피격시 메디테이션 동작중인데 메모가 없으면, 메디테이션 버프 해제

	if( true == m_bInMeditation && false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO11 ) )
		EraseBuffTempletFromGameUnit( BTI_BUFF_SI_A_AHM_MEDITATION );


#else  BALANCE_ELEMENTAL_MASTER_20130117
	if( true == IsDamagedReaction( pDamageData ) && pDamageData->m_fHpPercentUp <= 0.f && pDamageData->m_fMpPercentUp <= 0.f )
	{
#ifdef BALANCE_PATCH_20120329
		if ( true == m_bInMeditation )
		{
			const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;
			const CX2UserSkillTree& cUserSkillTree =  GetUnit()->GetUnitData().m_UserSkillTree;
			bool bFindMeditation = false;

			// 스킬 슬롯 A
			for ( int i = 0; i < 4; ++i )
			{
				pSkillSlotData = cUserSkillTree.GetSkillSlot( i, false );

				if ( pSkillSlotData->m_eID == CX2SkillTree::SI_A_AHM_MEDITATION )
				{
					bFindMeditation = true;
					break;
				}	
				else
				{
					pSkillSlotData = NULL;
				}
			}

			if ( false == bFindMeditation )
			{
				// 스킬 슬롯 B
				for ( int i = 0; i < 4; ++i )
				{
					pSkillSlotData = cUserSkillTree.GetSkillSlot( i, true );

					if ( pSkillSlotData->m_eID == CX2SkillTree::SI_A_AHM_MEDITATION )
					{
						bFindMeditation = true;
						break;
					}	
					else
					{
						pSkillSlotData = NULL;
					}
				}
			}

			if( NULL != pSkillSlotData )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );
				if( NULL != pSkillTemplet )
					cUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );

				StartMeditation( false );
			}
		}
#else
		StartMeditation( false );
#endif
	}
#endif BALANCE_ELEMENTAL_MASTER_20130117
}

void CX2GUArme_VioletMage::Win()
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

void CX2GUArme_VioletMage::Lose()
{
	CX2GUUser::Lose();

	if( IsMyUnit() == true && GetNowHp() > 0.0f )
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


void CX2GUArme_VioletMage::Draw()
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

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUArme_VioletMage::AppendComponentToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMinorParticle() != NULL )
	{
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_,  L"ArmeAirStep" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_,  L"MPEnergyArme" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_,  L"MPEnergyCenter" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_,  L"MPChargeWave" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_,  L"MPCharge2MagicCircle" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_,  L"MPCharge2Up" );
		g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_,  L"MPCharge2Up2" );
	}
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GUArme_VioletMage::InitComponent()
{
	CX2GUUser::InitComponent();

	if ( IsMyUnit() )
	{
		ASSERT( NULL != g_pX2Game );
		switch ( g_pX2Game->GetGameType() )
		{
		case CX2Game::GT_PVP:
			{
				m_pGageData->SetNowMp( _CONST_AISHA_::AISHA_INITIAL_MP );
			} break;
		default:
			break;
		}
	}
	

	m_hSeqArmeAirStepRight	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"ArmeAirStep", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	m_hSeqArmeAirStepLeft	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"ArmeAirStep", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );

	m_hSeqMPEnergy			= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPEnergyArme", 0,0,0, 0,0 );
	m_hSeqMPEnergyCenter		= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPEnergyCenter", 0,0,0, 0,0 );


	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
	if( NULL != pSeq )
	{
		pSeq->UseLookPoint( true );
	}

	m_hSeqMPChargeWave			= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPChargeWave", 0,0,0, 0,0 );

	m_hSeqMPCharge2MagicCircle	= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPCharge2MagicCircle", 0,0,0, 0,0 );
	m_hSeqMPCharge2Up			= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPCharge2Up", 0,0,0, 0,0 );
	m_hSeqMPCharge2Up2			= g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"MPCharge2Up2", 0,0,0, 0,0 );
}


#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUArme_VioletMage::AppendEffectToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ )
{
	CX2GUArme_VioletMage::AppendMajorParticleToDeviceList( listInOut_ );
	CX2GUArme_VioletMage::AppendMinorParticleToDeviceList( listInOut_ );
	CX2GameUnit::Weapon::AppendEnchantParticleForSwordToDeviceList( listInOut_, eDamageType_);
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

void CX2GUArme_VioletMage::InitEffect()
{
	CX2GUUser::InitEffect();

	CreateMajorParticleArray();
	CreateMinorParticleArray();
}	




/*virtual*/ void CX2GUArme_VioletMage::InitEnchantWeaponEffectUnitSpecific() 
{
	for( UINT i=0; i<m_vecpWeapon.size(); ++i )
	{
		Weapon* pWeapon = m_vecpWeapon[i];
		pWeapon->InitEnchantParticleForSword();
	}
}


void CX2GUArme_VioletMage::DeleteArmeMajorParticle()
{
	CX2GUUser::DeleteGUUserMajorParticle();

	CKTDGParticleSystem* pMajorParticleSystem = g_pX2Game->GetMajorParticle();

	for ( int index = 0; index < AISHA_PII_MAJOR_END; index++ )
	{
		pMajorParticleSystem->DestroyInstanceHandle( m_ahAishaMajorParticleInstance[index] );
	}
}

void CX2GUArme_VioletMage::DeleteArmeMinorParticle()
{
	CX2GUUser::DeleteGUUserMinorParticle();

	CKTDGParticleSystem* pMinorParticleSystem = g_pX2Game->GetMinorParticle();

	for ( int index = 0; index < AISHA_PII_MINOR_END; index++ )
	{
		pMinorParticleSystem->DestroyInstanceHandle( m_ahAishaMinorParticleInstance[index] );
	}
}

void CX2GUArme_VioletMage::InitializeArmeMajorParticleArray()
{
	CX2GUUser::InitializeGUUserMajorParticleArray();

	for ( int index = 0; index < AISHA_PII_MAJOR_END; index++ )
	{
		m_ahAishaMajorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUArme_VioletMage::AppendMajorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMajorParticle() == NULL )
		return;

	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpact" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactCore" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactSlash" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactRingRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactCoreRed" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"FireDamageImpactSlashRed" );

	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"ArmeAirShockRing" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"ArmeAirShockCenter" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"ArmeAirBlastCenter" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt1" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt2" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt3" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt4" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt5" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt6" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt7" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt8" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt9" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt10" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt11" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt12" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBoltCore" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt1Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt2Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt3Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt4Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt5Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt6Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt7Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt8Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt9Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt11Red" );
	g_pData->GetGameMajorParticle()->AppendToDeviceList( listInOut_, L"LightningBolt12Red" );

}

#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

/*virtual*/ void CX2GUArme_VioletMage::CreateMajorParticleArray()
{
	CX2GUUser::CreateMajorParticleArray();

	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	// small damage 타격을 줬을 때 나오는 파티클
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT,			L"FireDamageImpact",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE,		L"FireDamageImpactCore",		2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH,		L"FireDamageImpactSlash",		2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
    
	// AttackResult의 switch case에서 default:로 처리되는 타격에 대한 파티클 (현재는 small 이외의 파티클)
	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RED,		L"FireDamageImpactRed",			2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_RING_RED,	L"FireDamageImpactRingRed",		1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_CORE_RED,	L"FireDamageImpactCoreRed",		1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetCommonMajorParticleByEnum( COMMON_MAJOR_PII_DAMAGE_IMPACT_SLASH_RED,	L"FireDamageImpactSlashRed",	1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	// 콤보용
	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_RING, L"ArmeAirShockRing", 2 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_CENTER, L"ArmeAirShockCenter", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_BLAST_CENTER, L"ArmeAirBlastCenter", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	//{{ 라이트닝볼트, 인퍼널웨이브에서 쓰이는 파티클
	// kimhc // need refactoring 두 스킬을 EFFECT_pSeq = Set으로 정리하면 이부분을 삭제 해도 될듯
	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_01, L"LightningBolt1", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_02, L"LightningBolt2", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_03, L"LightningBolt3", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_04, L"LightningBolt4", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_05, L"LightningBolt5", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_06, L"LightningBolt6", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_07, L"LightningBolt7", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_08, L"LightningBolt8", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_09, L"LightningBolt9", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_10, L"LightningBolt10", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_11, L"LightningBolt11", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_12, L"LightningBolt12", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_CORE, L"LightningBoltCore", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_01, L"LightningBolt1Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_02, L"LightningBolt2Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_03, L"LightningBolt3Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_04, L"LightningBolt4Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_05, L"LightningBolt5Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_06, L"LightningBolt6Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_07, L"LightningBolt7Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_08, L"LightningBolt8Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_09, L"LightningBolt9Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_10, L"LightningBolt10Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_11, L"LightningBolt11Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_12, L"LightningBolt12Red", -1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
	//}} 라이트닝볼트, 인퍼널웨이브에서 쓰이는 파티클
}

CKTDGParticleSystem::CParticleEventSequence* CX2GUArme_VioletMage::SetAishaMajorParticleByEnum( AISHA_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleAishaMajorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

		SetHandleAishaMajorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( eVal_) );

	return pSeq;
}

CKTDGParticleSystem::CParticleEventSequence* CX2GUArme_VioletMage::SetAishaMajorParticleByEnum( AISHA_MAJOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleAishaMajorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 0, 0, iDrawCount_ );

		SetHandleAishaMajorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( eVal_) );

	return pSeq;
}

void CX2GUArme_VioletMage::InitializeArmeMinorParticleArray()
{
	CX2GUUser::InitializeGUUserMinorParticleArray();

	for ( int index = 0; index < AISHA_PII_MINOR_END; index++ )
	{
		m_ahAishaMinorParticleInstance[index] = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}

	m_hSeqLight_Arme_Circle_Flame = INVALID_PARTICLE_SEQUENCE_HANDLE;
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUArme_VioletMage::AppendMinorParticleToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	if ( g_pData->GetGameMinorParticle() == NULL )
		return;

	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"ArmeAirBlastRing" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"TimeStop1" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"TimeStop2" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"TimeStop3" );
	g_pData->GetGameMinorParticle()->AppendToDeviceList( listInOut_, L"TimeStopRed" );

}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

/*virtual*/ void CX2GUArme_VioletMage::CreateMinorParticleArray()
{
	CX2GUUser::CreateMinorParticleArray();


	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;

	pSeq = SetAishaMinorParticleByEnum( AISHA_PII_MINOR_AIR_BLAST_RING,	L"ArmeAirBlastRing", 1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_01,		L"TimeStop1", 1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_02,		L"TimeStop2", 1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_03,		L"TimeStop3", 1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );

	pSeq = SetAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_RED,		L"TimeStopRed", 1 );
	if( pSeq != NULL )
		pSeq->SetTriggerWait( true );
}

CKTDGParticleSystem::CParticleEventSequence* CX2GUArme_VioletMage::SetAishaMinorParticleByEnum( AISHA_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ )
{
	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == GetHandleAishaMinorParticleByEnum( eVal_ ) )
	{
		ParticleEventSequenceHandle hHandle = 
			g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName_.c_str(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 0, 0, iDrawCount_ );

		SetHandleAishaMinorParticleByEnum( eVal_, hHandle );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeq =
		g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAishaMinorParticleByEnum( eVal_) );

	return pSeq;
}

void CX2GUArme_VioletMage::ParseCommonRandomState()
{


	if( true == m_LuaManager.BeginTable( "INIT_COMMON_RANDOM_STATE" ) )
	{

		const char* tableName = "";
		switch( GetUnit()->GetClass() )
		{
		case CX2Unit::UC_ARME_VIOLET_MAGE:
			{
				tableName = "ARME_VIOLET_MAGE";
			} break;
		case CX2Unit::UC_ARME_HIGH_MAGICIAN:
			{
				tableName = "ARME_HIGH_MAGICIAN";
			} break;
		case CX2Unit::UC_ARME_DARK_MAGICIAN:
			{
				tableName = "ARME_DARK_MAGICIAN";
			} break;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
			{
				tableName = "ARME_BATTLE_MAGICIAN";
			} break;
#endif

		case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
			{
				tableName = "ARME_ELEMENTAL_MASTER";
			} break;
		case CX2Unit::UC_ARME_VOID_PRINCESS:
			{
				tableName = "ARME_VOID_PRINCESS";
			} break;
#ifdef SERV_ARME_DIMENSION_WITCH
		case CX2Unit::UC_ARME_DIMENSION_WITCH:
			{
				tableName = "ARME_DIMENSION_WITCH";
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


void CX2GUArme_VioletMage::InitState()
{
	ASSERT( NULL != GetUnit() );

	const CX2Unit::UnitData* pUnitData = &GetUnit()->GetUnitData();


	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ARME_VIOLET_MAGE:
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_DARK_MAGICIAN:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#endif
		{
			m_iSkillCutInSetIndex = 0;
		} break;
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			m_iSkillCutInSetIndex = 1;
		} break;
	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			m_iSkillCutInSetIndex = 2;
		} break;
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
		{
			m_iSkillCutInSetIndex = 3;
		} break;
#endif
	}

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
		{
			m_iSkillCutInSetSubIndex = 1;
		} break;
	case CX2Unit::UC_ARME_DARK_MAGICIAN:
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
	m_StateList[stateData.stateID] = stateData;






	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ARME_VIOLET_MAGE:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
#endif
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X2;
			m_LuaManager.MakeTableReference( "AVSI_COMBO_X2", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX2EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X2;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICIAN_COMBO_X2", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X2_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X3back;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICIAN_COMBO_X3back", stateData.stateID );				
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X3back_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X3back_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X3back_EndFuture );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_ARME_DARK_MAGICIAN: 
	case CX2Unit::UC_ARME_VOID_PRINCESS: 
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X2;
			m_LuaManager.MakeTableReference( "AVSI_DARK_MAGICIAN_COMBO_X2", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X2_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X2_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X3back;
			m_LuaManager.MakeTableReference( "AVSI_DARK_MAGICIAN_COMBO_X3back", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X3back_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X3back_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
	}



	switch( pUnitData->m_UnitClass )
	{
#ifdef BALANCE_PATCH_20120329
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X3down;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICIAN_COMBO_X3down", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X3down_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X3down_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X4down;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICIAN_COMBO_X4down", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X4down_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_COMBO_X4down_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
	case CX2Unit::UC_ARME_VIOLET_MAGE:
#else
	case CX2Unit::UC_ARME_VIOLET_MAGE:
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
#endif
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
#endif
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X3down;
			m_LuaManager.MakeTableReference( "AVSI_COMBO_X3down", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3downFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3downEventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

	case CX2Unit::UC_ARME_DARK_MAGICIAN:
	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X3down;
			m_LuaManager.MakeTableReference( "AVSI_DARK_MAGICIAN_COMBO_X3down", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X3down_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X3down_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X4down;
			m_LuaManager.MakeTableReference( "AVSI_DARK_MAGICIAN_COMBO_X4down", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X4down_Start );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X4down_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_COMBO_X4down_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
	}









	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ARME_VIOLET_MAGE:
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
#ifndef BALANCE_ELEMENTAL_MASTER_20130117		/// 엘리멘탈 마스터 대시 콤보 ZX
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
#endif BALANCE_ELEMENTAL_MASTER_20130117
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_COMBO_Z2a;
			m_LuaManager.MakeTableReference( "AVSI_DASH_COMBO_Z2a", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ2aStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ2aFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ2aEventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 엘리멘탈 마스터 대시 콤보 ZX
		case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			stateData.Init();
			stateData.stateID			= AVSI_EM_DASH_COMBO_Z2a;
			m_LuaManager.MakeTableReference( "AVSI_EM_DASH_COMBO_Z2a", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EM_DASH_COMBO_Z2a_StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EM_DASH_COMBO_Z2a_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EM_DASH_COMBO_Z2a_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif BALANCE_ELEMENTAL_MASTER_20130117


	case CX2Unit::UC_ARME_DARK_MAGICIAN:
	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_COMBO_Z2a;
			m_LuaManager.MakeTableReference( "AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_Start );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_DASH_COMBO_Z3up;
			m_LuaManager.MakeTableReference( "AVSI_DARK_MAGICIAN_DASH_COMBO_Z3up", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_DASH_COMBO_Z3up_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
#endif
		{
			stateData.Init();
			stateData.stateID			= AVSI_ABM_DASH_COMBO_Z2a;
			m_LuaManager.MakeTableReference( "AVSI_ABM_DASH_COMBO_Z2a", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_Start );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_ABM_DASH_COMBO_Z2a_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_ABM_DASH_COMBO_Z2a_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ABM_DASH_COMBO_ZXX;
			m_LuaManager.MakeTableReference( "ABM_DASH_COMBO_ZXX", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_ABM_DASH_COMBO_ZXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= ABM_DASH_COMBO_ZXXX;
			m_LuaManager.MakeTableReference( "ABM_DASH_COMBO_ZXXX", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_ABM_DASH_COMBO_ZXXX_FrameMove );			
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_ABM_DASH_COMBO_ZXXX_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
#endif
	}





	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ARME_VIOLET_MAGE:
	case CX2Unit::UC_ARME_DARK_MAGICIAN:
	case CX2Unit::UC_ARME_VOID_PRINCESS:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#endif
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_DASH_ATTACK_X", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashAttackXStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashAttackXFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashAttackXEventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashAttackXEndFuture );
			m_StateList[stateData.stateID] = stateData;
		} break;


	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICION_DASH_ATTACK_X", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICION_DASH_ATTACK_X_StartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICION_DASH_ATTACK_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICION_DASH_ATTACK_X_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICION_DASH_ATTACK_X_EndFuture );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_DASH_ATTACK_X2;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICION_DASH_ATTACK_X2", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICION_DASH_ATTACK_X2_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICION_DASH_ATTACK_X2_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;

#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_DIMENSION_WITCH_DASH_ATTACK_X", stateData.stateID );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashAttackXStartFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashAttackXFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DIMENSION_WITCH_DASH_ATTACK_X_EventProcess );
			stateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashAttackXEndFuture );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_DASH_ATTACK_X2;
			m_LuaManager.MakeTableReference( "AVSI_DIMENSION_WITCH_DASH_ATTACK_X2", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_DIMENSION_WITCH_DASH_ATTACK_X2_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif
	}

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ARME_VIOLET_MAGE:
	case CX2Unit::UC_ARME_DARK_MAGICIAN:
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_ATTACK_X", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXEventProcess );
			m_StateList[stateData.stateID] = stateData;

#ifdef BALANCE_PATCH_20120329
			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X2;
			m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_ATTACK_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackX2FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackX2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackX2EventProcess );
			m_StateList[stateData.stateID] = stateData;
#endif
		} break;


	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
#ifdef BALANCE_PATCH_20120329
			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_ATTACK_X", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXEventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X2;
			m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_ATTACK_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackX2FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackX2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackX2EventProcess );
			m_StateList[stateData.stateID] = stateData;
#else
			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X2;
			m_LuaManager.MakeTableReference( "AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2", stateData.stateID );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2_FrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2_EventProcess );
			m_StateList[stateData.stateID] = stateData;
#endif
		} break;


#ifdef AISHA_SECOND_CLASS_CHANGE
	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_AVP_DASH_JUMP_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_DASH_JUMP_X_Init );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_DASH_JUMP_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_DASH_JUMP_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;
#endif AISHA_SECOND_CLASS_CHANGE

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
#endif
		{
			stateData.Init();
			stateData.stateID			= AVSI_DASH_JUMP_ATTACK_X;
			m_LuaManager.MakeTableReference( "AVSI_ABM_DASH_JUMP_ATTACK_X", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_DASH_JUMP_X_Init );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackXFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_DASH_JUMP_X_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_DASH_JUMP_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;
#endif

	}




	switch( pUnitData->m_UnitClass )
	{
	default:
	case CX2Unit::UC_ARME_VIOLET_MAGE:
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_DARK_MAGICIAN:
	case CX2Unit::UC_ARME_VOID_PRINCESS:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
#endif
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z4up;
			m_LuaManager.MakeTableReference( "AVSI_COMBO_Z4up", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ4upStart );
			stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ4upFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ4upFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ4upEventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;

#ifdef AISHA_SECOND_CLASS_CHANGE
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			//stateData.Init();
			//stateData.stateID			= AVSI_COMBO_Z3up;
			//m_LuaManager.MakeTableReference( "AVSI_AEM_ZZupZ", stateData.stateID );
			//stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_ZZupZ_EventProcess );
			//m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z4up;
			m_LuaManager.MakeTableReference( "AVSI_AEM_ZZupZZ", stateData.stateID );
			//stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ4upStart );
			//stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ4upFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_ZZupZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_ZZupZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif AISHA_SECOND_CLASS_CHANGE
	}


	switch( pUnitData->m_UnitClass )
	{
	default:
	case CX2Unit::UC_ARME_VIOLET_MAGE:
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
	case CX2Unit::UC_ARME_DARK_MAGICIAN:
	case CX2Unit::UC_ARME_VOID_PRINCESS:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
#endif
		{

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X3front;
			m_LuaManager.MakeTableReference( "AVSI_COMBO_X3front", stateData.stateID );
			stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3frontStart );
			stateData.OnFrameMoveFuture = SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3frontFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3frontFrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3frontEventProcess );
			m_StateList[stateData.stateID] = stateData;

		} break;

#ifdef AISHA_SECOND_CLASS_CHANGE
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_X3front;
			m_LuaManager.MakeTableReference( "AVSI_AEM_XXfrontX", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XXfrontX_Init );
			//stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3frontStart );
			//stateData.OnFrameMoveFuture = SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3frontFrameMoveFuture );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XXfrontX_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XXfrontX_EventProcess );
			m_StateList[stateData.stateID] = stateData;


		} break;
#endif AISHA_SECOND_CLASS_CHANGE
	}







#ifdef AISHA_SECOND_CLASS_CHANGE

	switch( pUnitData->m_UnitClass )
	{
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{

			stateData.Init();
			stateData.stateID			= AVSI_AEM_XZ;
			m_LuaManager.MakeTableReference( "AVSI_AEM_XZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XZ_Init );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= AVSI_AEM_XZZ;
			m_LuaManager.MakeTableReference( "AVSI_AEM_XZZ", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XZZ_FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// XZZ 콤보 종료 스테이트 추가
			stateData.Init();
			stateData.stateID			= AVSI_AEM_XZZ_END;
			m_LuaManager.MakeTableReference( "AVSI_AEM_XZZ_END", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AEM_XZZ_END_EventProcess );
			m_StateList[stateData.stateID] = stateData;
#endif BALANCE_ELEMENTAL_MASTER_20130117

		} break;


	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{

			stateData.Init();
			stateData.stateID			= AVSI_AVP_ZZfrontZ;
			m_LuaManager.MakeTableReference( "AVSI_AVP_ZZfrontZ", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_ZZfrontZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;



			stateData.Init();
			stateData.stateID			= AVSI_AVP_ZZfrontZZ;
			m_LuaManager.MakeTableReference( "AVSI_AVP_ZZfrontZZ", stateData.stateID );
			stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_ZZfrontZZ_Init );
			stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_ZZfrontZZ_StateStartFuture );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_ZZfrontZZ_EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= AVSI_AVP_ZZfrontZZ_FALL;
			m_LuaManager.MakeTableReference( "AVSI_AVP_ZZfrontZZ_FALL", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_AVP_ZZfrontZZ_FALL_EventProcess );
			m_StateList[stateData.stateID] = stateData;


		} break;
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
		{
			stateData.Init();
			stateData.stateID			= AVSI_ADW_COMBO_ADD_X;
			m_LuaManager.MakeTableReference( "AVSI_ADW_COMBO_ADD_X", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_ADW_COMBO_ADD_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_ADW_COMBO_ADD_X2;
			m_LuaManager.MakeTableReference( "AVSI_ADW_COMBO_ADD_X2", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_ADW_COMBO_ADD_X_EventProcess );
			m_StateList[stateData.stateID] = stateData;
		} break;
#endif
	}

#endif AISHA_SECOND_CLASS_CHANGE
	

	InitPassiveSkillState();
	InitEquippedActiveSkillState();


	CX2GUUser::InitState();
}



void CX2GUArme_VioletMage::InitStateID()
{
	m_CommonState.m_Wait	= USI_WAIT;
	m_ChargeMpState			= AVSI_CHARGE_MP;

//#ifdef TRANSFORMER_TEST
//	m_CommonState.m_Transformed			= AVSI_TRANSFORMED;
//#endif TRANSFORMER_TEST




}

void CX2GUArme_VioletMage::InitStateCommon()
{
	UserUnitStateData stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_FRONT;
	m_LuaManager.MakeTableReference( "AVSI_DIE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DIE_BACK;
	m_LuaManager.MakeTableReference( "AVSI_DIE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DieBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DieBackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DieBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DieBackEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_START_INTRUDE;
	m_LuaManager.MakeTableReference( "AVSI_START_INTRUDE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, StartIntrudeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, StartIntrudeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, StartIntrudeEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= USI_WAIT;
	m_LuaManager.MakeTableReference( "AVSI_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, WaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, WaitStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, WaitFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, WaitEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_WALK;
	m_LuaManager.MakeTableReference( "AVSI_WALK", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser,			WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, WalkEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_READY;
	m_LuaManager.MakeTableReference( "AVSI_JUMP_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpReadyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_UP;
	m_LuaManager.MakeTableReference( "AVSI_JUMP_UP", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "AVSI_JUMP_DOWN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= AVSI_JUMP_LEVITATION;
	m_LuaManager.MakeTableReference( "AVSI_JUMP_LEVITATION", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpLevitationStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpLevitationFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, JumpLevitationFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpLevitationEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, JumpLevitationEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "AVSI_JUMP_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH;
	m_LuaManager.MakeTableReference( "AVSI_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_END;
	m_LuaManager.MakeTableReference( "AVSI_DASH_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DashEndStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashEndFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP;
	m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= AVSI_DASH_JUMP_LEVITATION;
	m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_LEVITATION", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpLevitationStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpLevitationFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpLevitationFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpLevitationEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpLevitationEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, JumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= AVSI_DASH_JUMP_POWER_LANDING;
	m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_POWER_LANDING", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpPowerLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpPowerLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_HYPER_MODE;
	m_LuaManager.MakeTableReference( "AVSI_HYPER_MODE", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, HyperModeInit );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser,			 HyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, HyperModeFrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, HyperModeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser,			 HyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser,			 HyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser,			 HyperModeEnd );
#endif HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= AVSI_CHARGE_MP;
	m_LuaManager.MakeTableReference( "AVSI_CHARGE_MP", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, ChargeMPInit );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, ChargeMPStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, ChargeMPStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ChargeMPFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ChargeMPEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, ChargeMPEndFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, ChargeMPEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_GROGGY;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_GROGGY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DAMAGE_GROGGY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_FRONT;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_SMALL_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DamageSmallFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_SMALL_BACK;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_SMALL_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DamageSmallBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_FRONT;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_BIG_FRONT", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DamageBigFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DamageBigFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_BIG_BACK;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_BIG_BACK", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DamageBigBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DamageBigBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_FRONT;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_DOWN_FRONT", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DamageDownFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DamageDownFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_DOWN_BACK;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_DOWN_BACK", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DamageDownBackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DamageDownBackEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_FRONT;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_STANDUP_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_STANDUP_BACK;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_STANDUP_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageStandUpEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, DamageStandUpEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_SMALL;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_SMALL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DamageAirSmallStartFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirSmallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_DOWN", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_INVINCIBLE;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_DOWN_INVINCIBLE", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirDownInvincibleEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_DOWN_LANDING;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_DOWN_LANDING", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, DamageAirDownLandingInit );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, DamageAirDownLandingFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DamageAirDownLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FALL;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_FALL", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFallEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_UP;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_UP", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirUpEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_FRONT;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_FLY_FRONT", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_AIR_FLY_BACK;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_AIR_FLY_BACK", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageAirFlyEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_DAMAGE_REVENGE;
	m_LuaManager.MakeTableReference( "AVSI_DAMAGE_REVENGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, DamageRevengeStart );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, DamageRevengeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, DamageRevengeEventProcess );
	m_StateList[stateData.stateID] = stateData;





	stateData.Init();
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	stateData.stateID			= AVSI_TELEPORT_END;
	m_LuaManager.MakeTableReference( "AVSI_A_AV_TELEPORT_END", stateData.stateID );
#else //UPGRADE_SKILL_SYSTEM_2013
	stateData.stateID			= AVSI_EVASION_END;
	m_LuaManager.MakeTableReference( "AVSI_EVASION_END", stateData.stateID );
#endif //UPGRADE_SKILL_SYSTEM_2013
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, EvasionEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, EvasionEndEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	stateData.stateID			= AVSI_TELEPORT_STAND_UP_START;
	m_LuaManager.MakeTableReference( "AVSI_TELEPORT_STAND_UP_START", stateData.stateID );
#else //UPGRADE_SKILL_SYSTEM_2013
	stateData.stateID			= AVSI_EVASION_STAND_UP_START;
	m_LuaManager.MakeTableReference( "AVSI_EVASION_STAND_UP_START", stateData.stateID );
#endif //UPGRADE_SKILL_SYSTEM_2013
	stateData.StateStart 		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EVASION_STAND_UP_START_Start );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EVASION_STAND_UP_START_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EVASION_STAND_UP_START_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	stateData.stateID			= AVSI_TELEPORT_STAND_UP_END;
	m_LuaManager.MakeTableReference( "AVSI_TELEPORT_STAND_UP_END", stateData.stateID );
#else //UPGRADE_SKILL_SYSTEM_2013
	stateData.stateID			= AVSI_EVASION_STAND_UP_END;
	m_LuaManager.MakeTableReference( "AVSI_EVASION_STAND_UP_END", stateData.stateID );
#endif //UPGRADE_SKILL_SYSTEM_2013
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EVASION_STAND_UP_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EVASION_STAND_UP_END_EventProcess );
	//stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_EVASION_STAND_UP_END_End );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	stateData.stateID			= AVSI_TELEPORT_STANDUP_ATTACK;
	m_LuaManager.MakeTableReference( "AVSI_TELEPORT_STANDUP_ATTACK", stateData.stateID );
#else //UPGRADE_SKILL_SYSTEM_2013
	stateData.stateID			= AVSI_EVASION_STANDUP_ATTACK;
	m_LuaManager.MakeTableReference( "AVSI_EVASION_STANDUP_ATTACK", stateData.stateID );
#endif //UPGRADE_SKILL_SYSTEM_2013
	stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, EvasionStandupAttackInit );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, EvasionStandupAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, EvasionStandupAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, EvasionStandupAttackEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, EvasionStandupAttackEnd );
	m_StateList[stateData.stateID] = stateData;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	switch( GetUnit()->GetClass() )
	{
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
#endif
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z1;
			m_LuaManager.MakeTableReference( "AVSI_ABM_COMBO_Z1", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ABM_ComboZ1EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z2;
			m_LuaManager.MakeTableReference( "AVSI_ABM_COMBO_Z2", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ABM_ComboZ2EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z3;
			m_LuaManager.MakeTableReference( "AVSI_ABM_COMBO_Z3", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ABM_ComboZ3EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z4;
			m_LuaManager.MakeTableReference( "AVSI_ABM_COMBO_Z4", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ABM_ComboZ4EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;
	default:
		{
			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z1;
			m_LuaManager.MakeTableReference( "AVSI_COMBO_Z1", stateData.stateID );
			stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ1FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ1EventProcess );
			m_StateList[stateData.stateID] = stateData;

			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z2;
			m_LuaManager.MakeTableReference( "AVSI_COMBO_Z2", stateData.stateID );
			//stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ2FrameMove );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ2EventProcess );
			m_StateList[stateData.stateID] = stateData;


			stateData.Init();
			stateData.stateID			= AVSI_COMBO_Z3;
			m_LuaManager.MakeTableReference( "AVSI_COMBO_Z3", stateData.stateID );
			stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ3EventProcess );
			m_StateList[stateData.stateID] = stateData;
		}
		break;	
	}	
#else
	stateData.Init();
	stateData.stateID			= AVSI_COMBO_Z1;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_Z1", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ1EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= AVSI_COMBO_Z2;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_Z2", stateData.stateID );
	//stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ2FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ2EventProcess );
	m_StateList[stateData.stateID] = stateData;

	
	stateData.Init();
	stateData.stateID			= AVSI_COMBO_Z3;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_Z3", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ3EventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif


	stateData.Init();
	stateData.stateID			= AVSI_COMBO_X1;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_X1", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX1EventProcess );
	m_StateList[stateData.stateID] = stateData;






	stateData.Init();
	stateData.stateID			= AVSI_COMBO_Z3up;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_Z3up", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboZ3upEventProcess );
	m_StateList[stateData.stateID] = stateData;






	stateData.Init();
	stateData.stateID			= AVSI_COMBO_X3;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_X3", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3EventProcess );
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 차지 스테이트로 갈 수 있는지에 대한 변수 초기화를 위해 추가
	stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX3StateEnd );
#endif BALANCE_ELEMENTAL_MASTER_20130117
	m_StateList[stateData.stateID] = stateData;

#ifdef BALANCE_ELEMENTAL_MASTER_20130117
	stateData.Init();		/// 파이어볼 발사 직전 차지 스테이트
	stateData.stateID			= AVSI_COMBO_X4_CHARGE;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_X4_CHARGE", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4ChargeStateStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4ChargeFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4ChargeEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4ChargeStateEnd );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();		/// 차지 파이어볼 발사 스테이트
	stateData.stateID			= AVSI_COMBO_X4_CHARGE_FIRE;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_X4_CHARGE_FIRE", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4ChargeFireFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4ChargeFireEventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif BALANCE_ELEMENTAL_MASTER_20130117

#ifdef BALANCE_PATCH_20120329
	stateData.Init();
	stateData.stateID			= AVSI_COMBO_X4;
	m_LuaManager.MakeTableReference( "AVSI_COMBO_X4", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, ComboX4EventProcess );
	m_StateList[stateData.stateID] = stateData;
#endif




	stateData.Init();
	stateData.stateID			= AVSI_DASH_COMBO_Z1;
	m_LuaManager.MakeTableReference( "AVSI_DASH_COMBO_Z1", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ1StartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ1FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ1EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= AVSI_DASH_COMBO_Z2;
	m_LuaManager.MakeTableReference( "AVSI_DASH_COMBO_Z2", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ2StartFuture );
	//stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ2FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashComboZ2EventProcess );
	m_StateList[stateData.stateID] = stateData;




	stateData.Init();
	stateData.stateID			= AVSI_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "AVSI_JUMP_ATTACK_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpAttackZFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, JumpAttackZFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= AVSI_JUMP_ATTACK_X;
	m_LuaManager.MakeTableReference( "AVSI_JUMP_ATTACK_X", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpAttackXFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, JumpAttackXFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, JumpAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;


	stateData.Init();
	stateData.stateID			= AVSI_DASH_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "AVSI_DASH_JUMP_ATTACK_Z", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackZFrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackZFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DashJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;




#ifdef WALL_JUMP_TEST
	stateData.Init();
	stateData.stateID			= AVSI_WALL_LANDING;
	m_LuaManager.MakeTableReference( "AVSI_WALL_LANDING", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, WallLandingEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, WallLandingEndFuture );
	m_StateList[stateData.stateID] = stateData;
#endif WALL_JUMP_TEST





	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_READY;
	m_LuaManager.MakeTableReference( "AVSI_PEPPER_RUN_READY", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_READY_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN;
	m_LuaManager.MakeTableReference( "AVSI_PEPPER_RUN", stateData.stateID );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_END;
	m_LuaManager.MakeTableReference( "AVSI_PEPPER_RUN_END", stateData.stateID );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_FrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_END_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_UP;
	m_LuaManager.MakeTableReference( "AVSI_PEPPER_RUN_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_UP_EventProcess );
	m_StateList[stateData.stateID] = stateData;

	stateData.Init();
	stateData.stateID			= USI_PEPPER_RUN_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "AVSI_PEPPER_RUN_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_StateStartFuture );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_FrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, PEPPER_RUN_JUMP_DOWN_EventProcess );
	m_StateList[stateData.stateID] = stateData;

#ifdef SPECIAL_USE_ITEM
	stateData.Init();
	stateData.stateID			= USI_THROW_ITEM;
	m_LuaManager.MakeTableReference( "AVSI_THROW_ITEM", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMoveFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_FrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUUser, THROW_ITEM_CameraMove );		
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, THROW_ITEM_EventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, THROW_ITEM_StateEnd );
	m_StateList[stateData.stateID] = stateData;
#endif SPECIAL_USE_ITEM

#ifdef RIDING_SYSTEM
	#pragma region AVSI_RIDING_START
	stateData.Init();
	stateData.stateID			= GetRidingStartStateID();
	m_LuaManager.MakeTableReference( "AVSI_RIDING_START", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingStartStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingStartStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingStartEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, StartEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_ON
	stateData.Init();
	stateData.stateID			= USI_RIDING_ON;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_ON", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingOnStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingOnStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOnEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOnEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_OFF
	stateData.Init();
	stateData.stateID			= USI_RIDING_OFF;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_OFF", stateData.stateID );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingOffEventProcess );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, RidingOffEnd );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_WAIT_HABIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT_HABIT;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_WAIT_HABIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitHabitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitHabitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_WAIT
	stateData.Init();
	stateData.stateID			= USI_RIDING_WAIT;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_WAIT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWaitStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWaitStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWaitEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_WALK
	stateData.Init();
	stateData.stateID			= USI_RIDING_WALK;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_WALK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingWalkStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingWalkStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, WalkFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingWalkEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion
	
	#pragma region AVSI_RIDING_JUMP_UP
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_UP;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_JUMP_UP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpUpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpUpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpUpEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_JUMP_DOWN
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_DOWN;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_JUMP_DOWN", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpDownStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpDownStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, JumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpDownEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_DASH
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_DASH", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, DashFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_DASH_END
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_END;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_DASH_END", stateData.stateID );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashEndStart );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashEndStartFuture );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingDashEndFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashEndEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_DASH_JUMP
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_DASH_JUMP", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEventProcess );
	stateData.StateEndFuture	= SET_CB_FUNC( CX2GUUser, RidingDashJumpEndFuture );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_DASH_JUMP_LANDING
	stateData.Init();
	stateData.stateID			= USI_RIDING_DASH_JUMP_LANDING;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_DASH_JUMP_LANDING", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, DashJumpLandingStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDashJumpLandingEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_DAMAGE_FRONT
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_FRONT;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_DAMAGE_FRONT", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageFrontStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_DAMAGE_BACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_DAMAGE_BACK;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_DAMAGE_BACK", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingDamageBackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingDamageBackStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingDamageBackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_DIE
	stateData.Init();
	stateData.stateID			= USI_RIDING_DIE;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_DIE", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, DieFrontEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_Z;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackZStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_JUMP_ATTACK_Z
	stateData.Init();
	stateData.stateID			= USI_RIDING_JUMP_ATTACK_Z;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_JUMP_ATTACK_Z", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZStart );
	stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZFrameMoveFuture );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingJumpAttackZEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_ATTACK_X
	stateData.Init();
	stateData.stateID			= USI_RIDING_ATTACK_X;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_ATTACK_X", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingAttackXStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingAttackXStart );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingAttackXEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_SPECIAL_ATTACK
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_ATTACK;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_ATTACK_SPECIAL", stateData.stateID );
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStartFuture );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackFrameMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, RidingSpecialAttackEventProcess );
	m_StateList[stateData.stateID] = stateData;
	#pragma endregion

	#pragma region AVSI_RIDING_SPECIAL_MOVE
	stateData.Init();
	stateData.stateID			= USI_RIDING_SPECIAL_MOVE;
	m_LuaManager.MakeTableReference( "AVSI_RIDING_SPECIAL_MOVE", stateData.stateID );
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
	m_LuaManager.MakeTableReference( "AVSI_RIDING_HYPER_MODE", stateData.stateID );
	stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, RidingHyperModeInit );
	stateData.StateStart		= SET_CB_FUNC( CX2GUUser,			 RidingHyperModeStart );
	stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, RidingHyperModeFrameMove );
	stateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, RidingHyperModeCameraMove );
	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser,			 RidingHyperModeEventProcess );
#ifdef HYPER_MODE_FIX
	stateData.StateStartFuture	= SET_CB_FUNC( CX2GUUser,			 RidingHyperModeStartFuture );
	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser,			 RidingHyperModeEnd );
#endif //#ifdef HYPER_MODE_FIX
	m_StateList[stateData.stateID] = stateData;
#endif // MODIFY_RIDING_PET_AWAKE

//#ifdef PVP_BOSS_COMBAT_TEST
//
//	stateData.Init();
//	stateData.stateID			= AVSI_FROZEN;
//	m_LuaManager.MakeTableReference( "AVSI_FROZEN", stateData.stateID );
//	stateData.StateStart		= SET_CB_FUNC( CX2GUUser, Frozen_StateStart ); 
//	stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, Frozen_EventProcess );
//	stateData.StateEnd			= SET_CB_FUNC( CX2GUUser, Frozen_StateEnd ); 
//	m_StateList[stateData.stateID] = stateData;
//
//	m_FrozenState = AVSI_FROZEN;
//
//#endif PVP_BOSS_COMBAT_TEST



//#ifdef TRANSFORMER_TEST
//
//	stateData.Init();
//	stateData.stateID			= AVSI_TRANSFORMED;
//	m_LuaManager.MakeTableReference( "AVSI_TRANSFORMED", stateData.stateID );
//	m_StateList[stateData.stateID] = stateData;
//
//
//#endif TRANSFORMER_TEST


}






void CX2GUArme_VioletMage::InitEquippedActiveSkillState(bool bOnlySkillLevel /* = false  */)
{
	CX2GUUser::InitEquippedActiveSkillState(bOnlySkillLevel);

#ifdef SKILL_CASH_10_TEST
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AHM_BLAZE_STEP );
	if( NULL != pSkillTemplet )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if ( NULL == GetUnit() )
			return;
	
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
		const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_AHM_BLAZE_STEP ) );	/// 스킬 레벨
#else // UPGRADE_SKILL_SYSTEM_2013	
		m_sBlazeStepData.m_fPowerRate = pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );

		m_sBlazeStepData.m_fPowerRate = pSkillTemplet->m_fPowerRate;
#endif // UPGRADE_SKILL_SYSTEM_2013
	}
#endif SKILL_CASH_10_TEST
}

/*virtual*/ void CX2GUArme_VioletMage::InitEtc()
{
	CX2GUUser::InitEtc();

// #ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
// 	m_TransformWeapon = CX2ItemManager::CreateDummyEquip( 40930, GetUnit(), m_pXSkinAnim );
// 	m_TransformWeapon->SetManagerObject( this );
// 	m_TransformWeapon->SetShowObject(false);
// 
// 	m_pOrgMotion = m_pMotion;
// #endif
}





/*virtual*/ void CX2GUArme_VioletMage::InitPassiveSkillState()
{
	CX2GUUser::InitPassiveSkillState();
	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

	// 마나 회복량 증가 값 초기화.
	SetAdditionalMPChangeRateByPassive(0.f);

	m_fMagicStaffSize = 0.f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	CX2SkillTree* pSkillTree = g_pData->GetSkillTree();
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree* const pSkillTree = g_pData->GetSkillTree();
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AVP_SPRINTER, true );
#else //UPGRADE_SKILL_SYSTEM_2013
	int iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_SPRINTER );
#endif //UPGRADE_SKILL_SYSTEM_2013
	if( iSkillLevel > 0 )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AVP_SPRINTER );

		if( NULL != pSkillTemplet )
		{
			const float fMoveSpeed = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MOVE_SPEED, iSkillLevel );


#ifdef FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정
			m_AdditionalWalkSpeed.SetPassiveMultiple( m_AdditionalWalkSpeed.GetPassiveMultiple() * fMoveSpeed );
			m_AdditionalDashSpeed.SetPassiveMultiple( m_AdditionalDashSpeed.GetPassiveMultiple() * fMoveSpeed );
#else	// FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정
			m_PhysicParam.fWalkSpeed *= fMoveSpeed;
			m_PhysicParam.fRunSpeed *= fMoveSpeed;
#endif // FIXED_INCREASE_MOVE_SPEED_INITIALIZING // 130925 김종훈, 이동속도 증가 효과가 적용되지 않는 문제 수정


		}
#else //UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_SPRINTER, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			const float fMoveSpeed = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MOVE_SPEED );
			m_PhysicParam.fWalkSpeed *= fMoveSpeed;
			m_PhysicParam.fRunSpeed *= fMoveSpeed;
		}
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	m_fMPConsumeRateTeleport = 1.f;
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AV_TELEPORT_UP );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AV_TELEPORT_UP, iSkillLevel );
		if( NULL != pSkillTemplet )
		{
			m_fMPConsumeRateTeleport *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

	m_fMPConsumeRateFireBall = 1.f;
	m_fFireBallRangeRate = 1.f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AV_FIREBALL_UP, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AV_FIREBALL_UP );
#endif // UPGRADE_SKILL_SYSTEM_2013
	if( iSkillLevel > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AV_FIREBALL_UP );

		if( NULL != pSkillTemplet )
		{
			m_fMPConsumeRateFireBall *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillLevel );
			m_fFireBallRangeRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillLevel );
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AV_FIREBALL_UP, iSkillLevel );

		if( NULL != pSkillTemplet )
		{
			m_fMPConsumeRateFireBall *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );
			m_fFireBallRangeRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

#ifdef AISHA_SECOND_CLASS_CHANGE
	const CX2SkillTree::SkillTemplet* pSkillTemplet = NULL;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AEM_ELEMENTAL_FRIENDSHIP, true );
	if( iSkillLevel > 0 )
	{
		pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_AEM_ELEMENTAL_FRIENDSHIP );
		if( NULL != pSkillTemplet )
		{
			const float fAddMpChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS, iSkillLevel );
			SetAdditionalMPChangeRateByPassive( fAddMpChangeRate );
			if ( GetNowStateID() != AVSI_CHARGE_MP )
				ResetMPChangeRate( GetOriginalMPChangeRate());

			m_bElementalFriendShip = true;
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_AEM_ELEMENTAL_FRIENDSHIP );
	if( NULL != pSkillTemplet )
		m_bElementalFriendShip = true;
#endif //UPGRADE_SKILL_SYSTEM_2013

	pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_AVP_STRONG_MIND );
	if( NULL != pSkillTemplet )
	{
		if( NULL == m_pStrongMindData )
		{
			m_pStrongMindData = new StrongMindData;


	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AVP_STRONG_MIND, true ) );	/// 스킬 레벨
	
			m_pStrongMindData->m_fActiveSkillRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_STRONG_MIND_ACTIVE_REL, iSkillTempletLevel );
			m_pStrongMindData->m_fSpecialActiveSkillRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_STRONG_MIND_SPECIAL_ACTIVE_REL, iSkillTempletLevel );
			m_pStrongMindData->m_fMPRestoreRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_RETAKE_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_pStrongMindData->m_fActiveSkillRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_STRONG_MIND_ACTIVE_REL );
			m_pStrongMindData->m_fSpecialActiveSkillRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_STRONG_MIND_SPECIAL_ACTIVE_REL );
			m_pStrongMindData->m_fMPRestoreRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_RETAKE_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013

		}
	}
#endif AISHA_SECOND_CLASS_CHANGE

#ifdef SERV_ARME_DIMENSION_WITCH
	m_iAdvancedTeleportationLv = 0;

	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADW_ADVANCED_TELEPORTATION, true );
	if( iSkillLevel > 0 )
	{
		pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_ADW_ADVANCED_TELEPORTATION );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			m_iAdvancedTeleportationLv = iSkillLevel;
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_iAdvancedTeleportationLv = iSkillLevel;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}	


	m_fMagicStaffExtraRate = 0.f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADW_MAGICAL_STAFF, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADW_MAGICAL_STAFF );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
		pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_ADW_MAGICAL_STAFF );
		if( NULL != pSkillTemplet )
		{			
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			m_fMagicStaffExtraRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE, iSkillLevel );
			SetHitCreateDamageEffectRate( m_fMagicStaffExtraRate );
			SetHitCreateDamageEffectPowerRate( pSkillTemplet->GetSkillPowerRateValue( iSkillLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fMagicStaffExtraRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_RATE );
			SetHitCreateDamageEffectRate( m_fMagicStaffExtraRate );
			SetHitCreateDamageEffectPowerRate( pSkillTemplet->m_fPowerRate );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}	

	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_ABM_MAGICAL_MAKEUP );
	if( iSkillLevel > 0 )
	{		
		m_fMagicStaffSize = 1.f; //pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS );
	}
#endif

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 풍요의 기운 MP 자연 회복 증가 효과 추가

#if 0 // 스킬 개편으로 마나 회복량 증가 제거
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AEM_ENERGY_OF_THE_PLENTY, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AEM_ENERGY_OF_THE_PLENTY );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevel > 0 )
	{
		pSkillTemplet = userSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_AEM_ENERGY_OF_THE_PLENTY );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const float fAddMpChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS, iSkillLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			const float fAddMpChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013

			SetAdditionalMPChangeRateByPassive( fAddMpChangeRate );

#ifdef FIX_AISHA_MP_CHARGE
			if ( GetNowStateID() != AVSI_CHARGE_MP )
				ResetMPChangeRate( GetOriginalMPChangeRate());
#else //FIX_AISHA_MP_CHARGE
			ResetMPChangeRate( GetOriginalMPChangeRate());
#endif //FIX_AISHA_MP_CHARGE
		}
	}
#endif
#endif BALANCE_ELEMENTAL_MASTER_20130117

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	// 빙점 연구
	m_bIceBall = false;
	m_fIceBallPower = 1.f;
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AHM_FREEZING_POINT_RESEARCH, true );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AHM_FREEZING_POINT_RESEARCH );
		if( NULL != pSkillTemplet )
		{
			m_bIceBall = true;
			m_fIceBallPower *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_REL, iSkillLevel );
		}
	}

	// 재앙의 그림자
	m_fShadowOfDisasterRate = 0.f;
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADM_SHADOW_OF_DISASTER, true );
	if( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_ADM_SHADOW_OF_DISASTER );
		if( NULL != pSkillTemplet )
		{
			m_fShadowOfDisasterRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_CRITICAL_RATE_ABS, iSkillLevel );
		}
	}

	// 원소학 연구
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AEM_ELEMENTAL_RESEARCH, true );
	m_fElementalResearchRate = 0.f;
	if ( 0 < iSkillLevel )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AEM_ELEMENTAL_RESEARCH );

		if ( NULL != pSkillTemplet )
		{
			m_fElementalResearchRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_DECREASE_REL, iSkillLevel );
			const float fAddMpChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS, iSkillLevel );
			SetAdditionalMPChangeRateByPassive( fAddMpChangeRate );
			if ( GetNowStateID() != AVSI_CHARGE_MP )
				ResetMPChangeRate( GetOriginalMPChangeRate());
		}
	}

	// 그라비티 볼 연구
	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AVP_GRAVITY_BALL_RESEARCH, true );
	m_bResearchGravityBall = false;
	m_fGravityBallPower = 1.f;
	if ( 0 < iSkillLevel )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = pSkillTree->GetSkillTemplet( CX2SkillTree::SI_P_AVP_GRAVITY_BALL_RESEARCH );

		if ( NULL != pSkillTemplet )
		{
			m_bResearchGravityBall = true;
			m_fGravityBallPower *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_ATK_REL, iSkillLevel );
		}
	}

	iSkillLevel = userSkillTree.GetSkillLevel( CX2SkillTree::SI_SA_ABM_SUPER_NOVA );
	if( iSkillLevel > 0 )
	{		
		m_fMagicStaffSize = 1.f; //pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SIZE_ABS );
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
}



/*virtual*/ void CX2GUArme_VioletMage::SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ )
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
			m_LuaManager.MakeTableReference( "AVSI_SI_AV_COMMON_BUFF", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, COMMON_BUFF_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_BUFF_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI_AV_COMMON_BUFF_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, COMMON_BUFF_FrameMove);
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
					m_LuaManager.MakeTableReference( "AVSI_THROW_ITEM", normalStateData.stateID );
					normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_FrameMove );	
					normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, COMMON_RELATIONSHIP_SKILL_EventProcess );	

					m_LuaManager.MakeTableReference( "AVSI_THROW_ITEM", hyperStateData.stateID );
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
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_AV_POWER_ATTACK1", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_AV_POWER_ATTACK_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_AV_POWER_ATTACK_EventProcess );
			m_LuaManager.MakeTableReference( "AVSI_SI_AV_POWER_ATTACK1", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_AV_POWER_ATTACK_HYPER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_AV_POWER_ATTACK_HYPER_EventProcess );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AV_CHAIN_FIRE_BALL:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_AV_CHAIN_FIRE_BALL:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_CHAIN_FIRE_BALL", normalStateData.stateID );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_FIRE_BALL_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_FIRE_BALL_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_FIRE_BALL_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER_FrameMove );
			hyperStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER_CameraMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER_EventProcess );
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AV_BINDING_CIRCLE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_AV_BINDING_CIRCLE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_BINDING_CIRCLE", normalStateData.stateID );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_End );

			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_BINDING_CIRCLE_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_HYPER_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_BINDING_CIRCLE_HYPER_End );

		} break;



	case CX2SkillTree::SI_SA_AV_GUST_SCREW:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_GUST_SCREW", normalStateData.stateID );
			normalStateData.StateInit				= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_Init );
			normalStateData.OnFrameMoveFuture		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_FrameMoveFuture );
			normalStateData.OnFrameMove				= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_FrameMove );
			normalStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_GUST_SCREW_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUST_SCREW_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_AV_LIGHTNING_BOLT:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_LIGHTNING_BOLT", normalStateData.stateID );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_End );

			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_LIGHTNING_BOLT_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_End );

		} break;

	case CX2SkillTree::SI_SA_AV_GUST_STORM:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI3_AV_GUST_STORM", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI3_AV_GUST_STORM_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_GUST_STORM_HYPER_EventProcess );			
		} break;

#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case CX2SkillTree::SI_SA_AV_METEO_CALL:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI3_AV_METEO_CALL", normalStateData.stateID );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_End );

			m_LuaManager.MakeTableReference( "AVSI_SI3_AV_METEO_CALL_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_HYPER_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_METEO_CALL_HYPER_End );

		} break;
#endif //UPGRADE_SKILL_SYSTEM_2013

	case CX2SkillTree::SI_A_AEM_CHAIN_BURST:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_CHAIN_BURST", normalStateData.stateID );
			//normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_BURST_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_BURST_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_BURST_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_CHAIN_BURST_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_BURST_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_CHAIN_BURST_HYPER_EventProcess );
		} break;

	case CX2SkillTree::SI_SA_AHM_MAGIC_MISSILE:		//** Code 내의 이름과 사용되는 이름이 다름 : 함수명 수정 필요
		{
			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_GUIDE_FIRE_BALL", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_EventProcess );
		} break;

#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_AHM_INFERNAL_WAVE:		//** Code 내의 이름과 사용되는 이름이 다름 : 함수명 수정 필요
		{
			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_BIND_ATTACK", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_BIND_ATTACK_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_BIND_ATTACK_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_BIND_ATTACK_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_BIND_ATTACK_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_BIND_ATTACK_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_BIND_ATTACK_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_BIND_ATTACK_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_BIND_ATTACK_HYPER_EventProcess );

		} break;
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_ADM_HELL_STONE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ADM_HELL_STONE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_HELL_STONE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_FrameMove );
			normalStateData.OnCameraMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_CameraMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI1_AV_HELL_STONE_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_HYPER_FrameMove );
			hyperStateData.OnCameraMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_HYPER_CameraMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI1_AV_HELL_STONE_HYPER_EventProcess );
		} break;

	case CX2SkillTree::SI_SA_ADM_DARK_CLOUD:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_DARK_CLOUD", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_DARK_CLOUD_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_DARK_CLOUD_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_DARK_CLOUD_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI2_AV_DARK_CLOUD_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_DARK_CLOUD_HYPER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI2_AV_DARK_CLOUD_HYPER_EventProcess );

		} break;

	case CX2SkillTree::SI_SA_ADM_PLASMA_CUTTER:		//** Code 내의 이름과 사용되는 이름이 다름 : 함수명 수정 필요
		{
			m_LuaManager.MakeTableReference( "AVSI_SI3_AV_CATASTROPHE", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_Init );
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_FrameMoveFuture );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_End );

			m_LuaManager.MakeTableReference( "AVSI_SI3_AV_CATASTROPHE_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_HYPER_FrameMoveFuture );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_HYPER_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_HYPER_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI3_AV_CATASTROPHE_HYPER_End );

		} break;



	case CX2SkillTree::SI_A_AV_TELEPORT:
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			m_LuaManager.MakeTableReference( "AVSI_A_AV_TELEPORT_START", normalStateData.stateID );
#else //UPGRADE_SKILL_SYSTEM_2013
			m_LuaManager.MakeTableReference( "AVSI_EVASION_START", normalStateData.stateID );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_NOTE
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Teleport_StateStart );
#endif
			normalStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Teleport_FrameMoveFuture );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Teleport_EventProcess );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			m_LuaManager.MakeTableReference( "AVSI_A_AV_TELEPORT_START", hyperStateData.stateID );
#else //UPGRADE_SKILL_SYSTEM_2013
			m_LuaManager.MakeTableReference( "AVSI_EVASION_START", hyperStateData.stateID );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_NOTE
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Teleport_StateStart );
#endif
			hyperStateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Teleport_FrameMoveFuture );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Teleport_EventProcess );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			m_fMPConsumeRateTeleport *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fMPConsumeRateTeleport *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			
#endif //UPGRADE_SKILL_SYSTEM_2013

		} break;


	case CX2SkillTree::SI_A_AV_CIRCLE_FLAME:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_A_AV_CIRCLE_FLAME_READY", normalStateData.stateID ); 				
			normalStateData.StateStartFuture= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_StartFuture );				
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_EventProcess );				
			normalStateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_EndFuture );				

			m_LuaManager.MakeTableReference( "AVSI_SI_A_AV_CIRCLE_FLAME_READY", hyperStateData.stateID ); 
			hyperStateData.StateStartFuture = SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_StartFuture );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_FrameMove );				
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_EventProcess );
			hyperStateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_READY_EndFuture );


			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= AVSI_SI_A_AV_CIRCLE_FLAME;
#ifdef SERV_SKILL_NOTE
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO7 ) == true )
				{
					m_LuaManager.MakeTableReference( "AVSI_SI_A_AV_CIRCLE_FLAME_MEMO", stateData.stateID );
				}
				else
				{
					m_LuaManager.MakeTableReference( "AVSI_SI_A_AV_CIRCLE_FLAME", stateData.stateID );
				}

#else
				m_LuaManager.MakeTableReference( "AVSI_SI_A_AV_CIRCLE_FLAME", stateData.stateID );
#endif
				stateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_Init );
				stateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_StartFuture );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_StateStart );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_FrameMove );
				stateData.OnFrameMoveFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_FrameMoveFuture );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_EventProcess );
				stateData.StateEndFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_AV_CIRCLE_FLAME_EndFuture );
				m_StateList[stateData.stateID] = stateData;
			}

		} break;



#ifdef SKILL_30_TEST


	case CX2SkillTree::SI_SA_AHM_BLIZZARDSHOWER:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_BLIZZARDSHOWER", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_StateEnd );


			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_BLIZZARDSHOWER_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_StateEnd );

		} break;

	case CX2SkillTree::SI_SA_ADM_DEATHFIELD:
		{

			m_LuaManager.MakeTableReference( "AVSI_SA_ADM_DEATHFIELD", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_StateEnd );


			m_LuaManager.MakeTableReference( "AVSI_SA_ADM_DEATHFIELD_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_DEATHFIELD_HYPER_StateEnd );


		} break;

	case CX2SkillTree::SI_A_ADM_MANA_INTAKE:
		{
			m_LuaManager.MakeTableReference( "AVSI_A_ADM_MANA_INTAKE", normalStateData.stateID ); 	

			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_EventProcess );


			m_LuaManager.MakeTableReference( "AVSI_A_ADM_MANA_INTAKE_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_HYPER_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_HYPER_StateStart );	
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_HYPER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADM_MANA_INTAKE_HYPER_EventProcess );	
		} break;

#endif SKILL_30_TEST

#ifdef SKILL_CASH_10_TEST
	case CX2SkillTree::SI_SA_AHM_BLAZE_STEP:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_BLAZE_STEP", normalStateData.stateID ); 	

			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLAZE_STEP_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLAZE_STEP_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLAZE_STEP_EventProcess );


			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_BLAZE_STEP_HYPER", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLAZE_STEP_HYPER_Init );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLAZE_STEP_HYPER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_BLAZE_STEP_HYPER_EventProcess );	
		} break;

	case CX2SkillTree::SI_SA_ADM_AGING:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_ADM_AGING", normalStateData.stateID ); 	

			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_AGING_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_AGING_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_ADM_AGING_EventProcess );


			m_LuaManager.MakeTableReference( "AVSI_SA_ADM_AGING_HYPER", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
#endif SKILL_CASH_10_TEST




#ifdef AISHA_SECOND_CLASS_CHANGE
	case CX2SkillTree::SI_A_AEM_STONE_WALL:
		{
			m_LuaManager.MakeTableReference( "AVSI_A_AEM_STONE_WALL", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AEM_STONE_WALL_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AEM_STONE_WALL_FrameMove );
#ifdef CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
#else CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, GenericActiveSkillEventProcess );
#endif CONVERSION_VS

			m_LuaManager.MakeTableReference( "AVSI_A_AEM_STONE_WALL", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AEM_STONE_WALL_FrameMove );
#ifdef CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
#else CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, GenericActiveSkillEventProcess );
#endif CONVERSION_VS

		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AVP_SUMMON_BAT_TIME_BOMB:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_A_AVP_SUMMON_BAT:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_A_AVP_SUMMON_BAT", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AVP_SUMMON_BAT_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AVP_SUMMON_BAT_FrameMove );
#ifdef CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
#else CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, GenericActiveSkillEventProcess );
#endif CONVERSION_VS

			m_LuaManager.MakeTableReference( "AVSI_A_AVP_SUMMON_BAT", hyperStateData.stateID );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AVP_SUMMON_BAT_FrameMove );
#ifdef CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
#else CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, GenericActiveSkillEventProcess );
#endif CONVERSION_VS
			
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_AHM_CYCLONE:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_AEM_CYCLONE:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AEM_CYCLONE", normalStateData.stateID );
			normalStateData.m_bHyperState = false;
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_CYCLONE_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_CYCLONE_FrameMove );
#ifdef CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
#else CONVERSION_VS
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, GenericSpecialActiveSkillEventProcess );
#endif CONVERSION_VS

			m_LuaManager.MakeTableReference( "AVSI_SA_AEM_CYCLONE", hyperStateData.stateID );
			hyperStateData.m_bHyperState = true;
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_CYCLONE_FrameMove );
#ifdef CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
#else CONVERSION_VS
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, GenericSpecialActiveSkillEventProcess );
#endif CONVERSION_VS

		} break;


#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case CX2SkillTree::SI_A_ADM_HELL_DROP:
#else //UPGRADE_SKILL_SYSTEM_2013
	case CX2SkillTree::SI_SA_AVP_HELL_DROP:		
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AVP_HELL_DROP", normalStateData.stateID );
			normalStateData.m_bHyperState = false;
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_Init );
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_Start );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_EventProcess );
			normalStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_StateEnd );

			m_LuaManager.MakeTableReference( "AVSI_SA_AVP_HELL_DROP", hyperStateData.stateID );
			hyperStateData.m_bHyperState = true;
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_Start );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_EventProcess );
			hyperStateData.StateEnd				= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_HELL_DROP_StateEnd );
			
			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_IDLE;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_IDLE", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_IDLE_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_LEFT;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_LEFT", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_LEFT_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_RIGHT;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_RIGHT", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_RIGHT_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_UP;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_UP", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_UP_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_DOWN;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_DOWN", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_DOWN_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_UP_LEFT;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_UP_LEFT", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_UP_LEFT_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_UP_RIGHT;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_UP_RIGHT", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_UP_RIGHT_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_DOWN_LEFT;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_DOWN_LEFT", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_DOWN_LEFT_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_DOWN_RIGHT;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_DOWN_RIGHT", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_DOWN_RIGHT_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FrameMove );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_EventProcess );
				m_StateList[stateData.stateID] = stateData;
				
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_FIRE;

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 헬 드롭, 발사 이후 바로 사망 에서 발사 시간 지나면 사망으로 변경에 따라
				// State 함수들 변경
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_FIRE", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FIRE_StateStart );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FIRE_EventProcess );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_FIRE", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HELL_DROP_CONTROL_FIRE_FrameMove );
#ifdef CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
#else CONVERSION_VS
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, GenericSpecialActiveSkillEventProcess );
#endif CONVERSION_VS

#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				m_StateList[stateData.stateID] = stateData;


#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 헬 드랍 Dying State 추가
				stateData.Init();
				stateData.stateID			= AVSI_HELL_DROP_CONTROL_DYING;
				m_LuaManager.MakeTableReference( "AVSI_HELL_DROP_CONTROL_DYING", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
				m_StateList[stateData.stateID] = stateData;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			}

		} break;


		// 김상훈 2010.11.1   : 메테오 샤워
#ifdef NEW_SKILL_2010_11
	case CX2SkillTree::SI_SA_AEM_METEOR_SHOWER:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_METEOSHOWER", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_StateEnd );


			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_METEOSHOWER_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
			hyperStateData.StateEnd			= normalStateData.StateEnd;
			
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 메테오 샤워 루프 스테이트추가
			{
				UserUnitStateData stateData;

				/// 일반
				stateData.Init();
				stateData.stateID			= AVSI_SA_AHM_METEOSHOWER_LOOP;
				m_LuaManager.MakeTableReference( "AVSI_SA_AHM_METEOSHOWER_LOOP", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				/// 각성
				stateData.Init();
				stateData.stateID			= AVSI_SA_AHM_METEOSHOWER_HYPER_LOOP;
				m_LuaManager.MakeTableReference( "AVSI_SA_AHM_METEOSHOWER_LOOP", stateData.stateID );
				stateData.m_bHyperState		= true;
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_LOOP_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				/// 종료
				stateData.Init();
				stateData.stateID			= AVSI_SA_AHM_METEOSHOWER_END;
				m_LuaManager.MakeTableReference( "AVSI_SA_AHM_METEOSHOWER_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_END_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_METEOSHOWER_END_StateEnd );	
				m_StateList[stateData.stateID] = stateData;
			}

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			m_fFireGap					= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_CHARGING_TIME, iSkillTempletLevel );			/// 한발당 발사 쿨타임
			m_iMaxFireCount				= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel ) );	/// 최대 발사량
			m_iMaxFireCountByLoopState	= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillTempletLevel ) );			/// 최대 발사량( 루프 스테이트 )
			m_fUseMPConsume				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillTempletLevel );			/// 한발당 소비 엠피
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fFireGap					= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_CHARGING_TIME );								/// 한발당 발사 쿨타임
			m_iMaxFireCount				= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT ) );	/// 최대 발사량
			m_iMaxFireCountByLoopState	= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT ) );			/// 최대 발사량( 루프 스테이트 )
			m_fUseMPConsume				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );								/// 한발당 소비 엠피
	#endif // UPGRADE_SKILL_SYSTEM_2013
			
#endif BALANCE_ELEMENTAL_MASTER_20130117
		} break;

		// 김상훈 2010.11.14 : 팬텀 브리딩
	case CX2SkillTree::SI_SA_AVP_PHANTOM_BREATHING_DARK_FALL:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AVP_PHANTOM_BREATHING", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_StateEnd );


			m_LuaManager.MakeTableReference( "AVSI_SA_AVP_PHANTOM_BREATHING_HYPER", hyperStateData.stateID ); 
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_FrameMove );		
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_EventProcess );	
			hyperStateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_StateEnd );
		} break;
#endif NEW_SKILL_2010_11
		// 김상훈 2010.11.1   : 메테오 샤워

#endif AISHA_SECOND_CLASS_CHANGE


#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2SkillTree::SI_A_AV_FIRE_ROAD:	
		{
			m_LuaManager.MakeTableReference( "AVSI_A_AV_FIRE_ROAD", normalStateData.stateID );				
			normalStateData.StateStart			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_FIRE_ROAD_Start );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_FIRE_ROAD_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_FIRE_ROAD_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_A_AV_FIRE_ROAD", hyperStateData.stateID );
			hyperStateData.StateStart			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_FIRE_ROAD_Start );
			hyperStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_FIRE_ROAD_FrameMove );
			hyperStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_FIRE_ROAD_EventProcess );
		} break;			
	case CX2SkillTree::SI_A_ABM_ENERGY_DRAIN:
		{
			m_LuaManager.MakeTableReference( "AVSI_A_ABM_ENERGY_DRAIN_READY", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_ENERGY_DRAIN_READY_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_ENERGY_DRAIN_READY_EventProcess );	

			m_LuaManager.MakeTableReference( "AVSI_A_ABM_ENERGY_DRAIN_READY", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_ENERGY_DRAIN_READY_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_ENERGY_DRAIN_READY_EventProcess );

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= AVSI_A_ABM_ENERGY_DRAIN_SUCCESS;
				m_LuaManager.MakeTableReference( "AVSI_A_ABM_ENERGY_DRAIN_SUCCESS", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_ENERGY_DRAIN_SUCCESS_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_ENERGY_DRAIN_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= AVSI_A_ABM_ENERGY_DRAIN_FAIL;
				m_LuaManager.MakeTableReference( "AVSI_A_ABM_ENERGY_DRAIN_FAIL", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_ENERGY_DRAIN_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_ADW_HEAVY_PRESS:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ABM_HEAVY_PRESS:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_HEAVY_PRESS", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_HEAVY_PRESS_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );			

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_HEAVY_PRESS", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_HEAVY_PRESS_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
		}
		break;
	case CX2SkillTree::SI_SA_ABM_ENERGY_SPURT:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_ENERGY_SPURT", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_ENERGY_SPURT_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );			

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_ENERGY_SPURT", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_ENERGY_SPURT_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );			
		}
		break;
	case CX2SkillTree::SI_SA_ABM_GUILLOTINE_PRESS:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_GUILLOTINE_PRESS_START", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_GUILLOTINE_PRESS_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_GUILLOTINE_PRESS_START_EventProcess );			

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_GUILLOTINE_PRESS_START", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_GUILLOTINE_PRESS_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_GUILLOTINE_PRESS_START_EventProcess );

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP;
				m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= AVSI_SI_SA_ABM_GUILLOTINE_PRESS_END;
				m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_GUILLOTINE_PRESS_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;	
	case CX2SkillTree::SI_SA_ABM_SUPER_NOVA:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_SUPER_NOVA", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_SUPER_NOVA_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );			

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_SUPER_NOVA", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_SUPER_NOVA_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );			
		}
		break;
	case CX2SkillTree::SI_SA_ABM_MAGICAL_MAKEUP:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_MAGICAL_MAKEUP", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_MAGICAL_MAKEUP_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_MAGICAL_MAKEUP_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );			
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_MAGICAL_MAKEUPR_StateEnd );

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ABM_MAGICAL_MAKEUP", hyperStateData.stateID );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_MAGICAL_MAKEUP_Init );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_MAGICAL_MAKEUP_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
			hyperStateData.StateEnd		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ABM_MAGICAL_MAKEUPR_StateEnd );
		}
		break;
#endif //SERV_ADD_ARME_BATTLE_MAGICIAN

#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION
	case CX2SkillTree::SI_A_ADW_DISTORTION: // 공간 왜곡
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_A_ADW_DISTORTION", normalStateData.stateID );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_ADW_DISTORTION_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_ADW_DISTORTION_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_ADW_DISTORTION_FrameMove );

			m_LuaManager.MakeTableReference( "AVSI_SI_A_ADW_DISTORTION", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_ADW_DISTORTION_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_ADW_DISTORTION_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_A_ADW_DISTORTION_FrameMove );				
		}
		break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_A_ADW_IMPACT_HAMMER:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ADW_MORNING_STAR:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ADW_MORNING_STAR", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ADW_MORNING_STAR_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ADW_MORNING_STAR", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ADW_MORNING_STAR_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
		}
		break;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case CX2SkillTree::SI_SA_ADW_SCREWDRIVER_TORNADO:
#else //UPGRADE_SKILL_SYSTEM_2013
		case CX2SkillTree::SI_SA_ADW_SCREWDRIVER:
#endif //UPGRADE_SKILL_SYSTEM_2013
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ADW_SCREWDRIVER", normalStateData.stateID );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ADW_SCREWDRIVER_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ADW_SCREWDRIVER", hyperStateData.stateID );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ADW_SCREWDRIVER_FrameMove );
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
		}
		break;
#endif
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	#pragma region SI_A_AV_ICICLE_WAVE
	case CX2SkillTree::SI_A_AV_ICICLE_WAVE:
		{
			m_LuaManager.MakeTableReference( "SI_A_AV_ICICLE_WAVE", normalStateData.stateID );		
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Icicle_Wave_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Icicle_Wave_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AV_Icicle_Wave_EventProcess );

			m_LuaManager.MakeTableReference( "SI_A_AV_ICICLE_WAVE", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	#pragma endregion 아이시클 웨이브

	#pragma region SI_SA_AV_ICE_STORM
	case CX2SkillTree::SI_SA_AV_ICE_STORM:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AV_ICE_STORM", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AV_Ice_Storm_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AV_Ice_Storm_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AV_Ice_Storm_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SA_AV_ICE_STORM", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion 아이스 스톰

	#pragma region SI_A_AHM_AQUA_SHOWER
	case CX2SkillTree::SI_A_AEM_AQUA_SHOWER:
		{
			m_LuaManager.MakeTableReference( "AVSI_A_AHM_AQUA_SHOWER", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AHM_Aqua_Shower_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AHM_Aqua_Shower_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AHM_Aqua_Shower_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_A_AHM_AQUA_SHOWER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion 아쿠아 샤워

	#pragma region SI_SA_AHM_LIGHTNING_SHOWER
	case CX2SkillTree::SI_SA_AEM_LIGHTNING_SHOWER:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_AHM_LIGHTNING_SHOWER", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_FrameMove );				
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_EventProcess );				
			normalStateData.StateEnd		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_StateEnd );


			m_LuaManager.MakeTableReference( "AVSI_SA_AHM_AHM_LIGHTNING_SHOWER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
			hyperStateData.StateEnd			= normalStateData.StateEnd;

			{
				UserUnitStateData stateData;

				/// 일반
				stateData.Init();
				stateData.stateID			= AVSI_SA_AHM_LIGHTNING_SHOWER_LOOP;
				m_LuaManager.MakeTableReference( "AVSI_SA_AHM_LIGHTNING_SHOWER_LOOP", stateData.stateID );
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				/// 각성
				stateData.Init();
				stateData.stateID			= AVSI_SA_AHM_LIGHTNING_SHOWER_HYPER_LOOP;
				m_LuaManager.MakeTableReference( "AVSI_SA_AHM_LIGHTNING_SHOWER_LOOP", stateData.stateID );
				stateData.m_bHyperState		= true;
				stateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_StateStart );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_EventProcess );
				stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_Loop_StateEnd );
				m_StateList[stateData.stateID] = stateData;

				/// 종료
				stateData.Init();
				stateData.stateID			= AVSI_SA_AHM_LIGHTNING_SHOWER_END;
				m_LuaManager.MakeTableReference( "AVSI_SA_AHM_LIGHTNING_SHOWER_END", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_End_EventProcess );	
				stateData.StateEnd			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AHM_Lightning_Shower_End_StateEnd );	
				m_StateList[stateData.stateID] = stateData;
			}

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
			return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			m_fFireGap					= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_CHARGING_TIME, iSkillTempletLevel );								/// 한발당 발사 쿨타임
			m_iMaxFireCount				= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel ) );	/// 최대 발사량
			m_iMaxFireCountByLoopState	= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillTempletLevel ) );			/// 최대 발사량( 루프 스테이트 )
			m_fUseMPConsume				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillTempletLevel );								/// 한발당 소비 엠피
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fFireGap					= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_CHARGING_TIME );								/// 한발당 발사 쿨타임
			m_iMaxFireCount				= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT ) );	/// 최대 발사량
			m_iMaxFireCountByLoopState	= static_cast<int>( pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT ) );			/// 최대 발사량( 루프 스테이트 )
			m_fUseMPConsume				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );								/// 한발당 소비 엠피
	#endif // UPGRADE_SKILL_SYSTEM_2013
		} break;
		#pragma endregion 라이트닝 샤워

	#pragma region SI_SA_AEM_CHAIN_LIGHTNING
	case CX2SkillTree::SI_SA_AHM_CHAIN_LIGHTNING:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AEM_CHAIN_LIGHTNING", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_Chain_Lightning_Init );
#ifdef ADD_MEMO_1ST_CLASS //김창한
			normalStateData.StateStartFuture	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_Chain_Lightning_StartFuture );
			normalStateData.StateEndFuture		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_Chain_Lightning_EndFuture );
#endif //ADD_MEMO_1ST_CLASS
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_Chain_Lightning_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AEM_Chain_Lightning_EventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SA_AEM_CHAIN_LIGHTNING", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
#ifdef ADD_MEMO_1ST_CLASS //김창한
			hyperStateData.StateStartFuture		= normalStateData.StateStartFuture;
			hyperStateData.StateEndFuture		= normalStateData.StateEndFuture;	
#endif //ADD_MEMO_1ST_CLASS
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;
		} break;
	#pragma endregion 체인 라이트닝

	#pragma region SI_A_AVP_SUMMON_BAT_HEAVY
	case CX2SkillTree::SI_A_AVP_SUMMON_BAT_HEAVY_DOLL:
		{
			m_LuaManager.MakeTableReference( "AVSI_A_AVP_SUMMON_BAT_HEAVY", normalStateData.stateID );
			normalStateData.StateInit			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AVP_Summon_Bat_Heavy_Init );
			normalStateData.OnFrameMove			= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_AVP_Summon_Bat_Heavy_FrameMove );
			normalStateData.OnEventProcess		= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );

			m_LuaManager.MakeTableReference( "AVSI_A_AVP_SUMMON_BAT_HEAVY", hyperStateData.stateID );
			hyperStateData.m_bHyperState		= true;
			hyperStateData.OnFrameMove			= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess		= normalStateData.OnEventProcess;

		} break;
	#pragma endregion 쁘띠 앙고르 - 헤비 돌

	#pragma region SI_SA_AVP_PHANTOM_BREATHING_DARK_HOLE
	case CX2SkillTree::SI_SA_AVP_PHANTOM_BREATHING_DARK_HOLE:
		{
			m_LuaManager.MakeTableReference( "AVSI_SA_AVP_PHANTOM_BREATHING_DARK_HOLE", normalStateData.stateID ); 				
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_Phantom_Breathing_Dark_Hole_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_Phantom_Breathing_Dark_Hole_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SA_AVP_Phantom_Breathing_Dark_Hole_EventProcess );
			
			m_LuaManager.MakeTableReference( "AVSI_SA_AVP_PHANTOM_BREATHING_DARK_HOLE", hyperStateData.stateID ); 
			hyperStateData.m_bHyperState	= true;
			hyperStateData.StateStart		= normalStateData.StateStart;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= normalStateData.OnEventProcess;
		} break;
	#pragma endregion 팬텀 브리딩 - 다크 홀

	#pragma region SI_A_ABM_VITAL_DRAIN
	case CX2SkillTree::SI_A_ABM_VITAL_DRAIN:
		{
			m_LuaManager.MakeTableReference( "AVSI_A_ABM_VITAL_DRAIN_READY", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_Vital_Drain_Ready_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_Vital_Drain_Ready_FrameMove );

			m_LuaManager.MakeTableReference( "AVSI_A_ABM_VITAL_DRAIN_READY", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;

			{
				UserUnitStateData stateData;

				stateData.Init();
				stateData.stateID			= AVSI_A_ABM_VITAL_DRAIN_SUCCESS;
				m_LuaManager.MakeTableReference( "AVSI_A_ABM_VITAL_DRAIN_SUCCESS", stateData.stateID );
				stateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_Vital_Drain_Success_FrameMove );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_Vital_Drain_EventProcess );
				m_StateList[stateData.stateID] = stateData;

				stateData.Init();
				stateData.stateID			= AVSI_A_ABM_VITAL_DRAIN_FAIL;
				m_LuaManager.MakeTableReference( "AVSI_A_ABM_VITAL_DRAIN_FAIL", stateData.stateID );
				stateData.OnEventProcess	= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ABM_Vital_Drain_EventProcess );
				m_StateList[stateData.stateID] = stateData;
			}
		} break;
	#pragma endregion 바이탈 드레인

	#pragma region SI_A_ADW_WORM_HOLE
	case CX2SkillTree::SI_A_ADW_WORM_HOLE:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_A_ADW_WORM_HOLE", normalStateData.stateID );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADW_Worm_Hole_Init );
			normalStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADW_Worm_Hole_StateStart );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADW_Worm_Hole_FrameMove );

			m_LuaManager.MakeTableReference( "AVSI_SI_A_ADW_WORM_HOLE", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
			hyperStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADW_Worm_Hole_Init );
			hyperStateData.StateStart		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADW_Worm_Hole_StateStart );
			hyperStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_A_ADW_Worm_Hole_FrameMove );				
		} break;
	#pragma endregion 웜홀

	#pragma region SI_SA_ADW_SCREWDRIVER_DRILLER
	case CX2SkillTree::SI_SA_ADW_SCREWDRIVER_DRILLER:
		{
			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ADW_SCREWDRIVER_DRILLER", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ADW_Screwdriver_Driller_Init );
			normalStateData.OnFrameMove		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_SI_SA_ADW_Screwdriver_Driller_FrameMove );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );

			m_LuaManager.MakeTableReference( "AVSI_SI_SA_ADW_SCREWDRIVER_DRILLER", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnFrameMove		= normalStateData.OnFrameMove;
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericActiveSkillEventProcess );
		}
		break;
		
	#pragma endregion 스크류 드라이버 - 드릴러
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	#pragma region SI_SA_AEM_Elemental_Storm
	case CX2SkillTree::SI_HA_AEM_ELEMENTAL_STORM:
		{
			m_LuaManager.MakeTableReference( "AVSI_HA_AEM_ELEMENTAL_STORM", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HA_AEM_Elemental_Storm_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );

			m_LuaManager.MakeTableReference( "AVSI_HA_AEM_ELEMENTAL_STORM", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
		} break;
	#pragma endregion 엘리멘탈 스톰 : 궁극기

	#pragma region SI_FS_AVP_Abyss_Angor
	case CX2SkillTree::SI_HA_AVP_ABYSS_ANGOR:
		{
			m_LuaManager.MakeTableReference( "AVSI_HA_AVP_ABYSS_ANGOR", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HA_AVP_Abyss_Angor_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );

			m_LuaManager.MakeTableReference( "AVSI_HA_AVP_ABYSS_ANGOR", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
		} break;
	#pragma endregion 어비스 앙고르 : 궁극기

	#pragma region SI_FS_ADW_Fate_space
		case CX2SkillTree::SI_HA_ADW_FATE_SPACE:
		{
			m_LuaManager.MakeTableReference( "AVSI_HA_ADW_FATE_SPACE", normalStateData.stateID );
			normalStateData.StateInit		= SET_CB_FUNC( CX2GUArme_VioletMage, AVSI_HA_ADW_Fate_space_Init );
			normalStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );

			m_LuaManager.MakeTableReference( "AVSI_HA_ADW_FATE_SPACE", hyperStateData.stateID );
			hyperStateData.m_bHyperState	= true;
			hyperStateData.OnEventProcess	= SET_CB_FUNC( CX2GUUser, GenericSpecialActiveSkillEventProcess );
		} break;
	#pragma endregion 페이트 스페이스 : 궁극기
#endif //FINALITY_SKILL_SYSTEM

	}
}

/*virtual*/ void CX2GUArme_VioletMage::SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if ( NULL == GetUnit() )
		return;

	const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
#endif // UPGRADE_SKILL_SYSTEM_2013

	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AV_CIRCLE_FLAME == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AV_CIRCLE_FLAME );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AV_CIRCLE_FLAME ) );	/// 스킬 레벨
	
			UserUnitStateData& stateData = m_StateList[ AVSI_SI_A_AV_CIRCLE_FLAME ];
			stateData.m_SPLevel		= iSkillTempletLevel;
			stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateData.m_eSkillID	= pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateData = m_StateList[ AVSI_SI_A_AV_CIRCLE_FLAME ];
			stateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			stateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			stateData.m_eSkillID	= pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
	
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_AHM_MEDITATION == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AHM_MEDITATION );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AHM_MEDITATION ) );	/// 스킬 레벨
	
			m_fMeditationMPChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_fMeditationMPChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
	
#ifdef BALANCE_PATCH_20120329
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ABM_ENERGY_DRAIN == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ABM_ENERGY_DRAIN );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_A_ABM_ENERGY_DRAIN ) );	/// 스킬 레벨
	
			UserUnitStateData& stateData = m_StateList[ AVSI_A_ABM_ENERGY_DRAIN_SUCCESS ];
			stateData.m_SPLevel		= iSkillTempletLevel;
			stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateData.m_eSkillID	= pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& stateData = m_StateList[ AVSI_A_ABM_ENERGY_DRAIN_SUCCESS ];
			stateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			stateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			stateData.m_eSkillID	= pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
	
	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_ABM_GUILLOTINE_PRESS == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ABM_GUILLOTINE_PRESS );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& loopStateData = m_StateList[ AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP ];
			loopStateData.m_SPLevel		= iSkillTempletLevel;
			loopStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			loopStateData.m_eSkillID	= pSkillTemplet->m_eID;

			UserUnitStateData& endStateData = m_StateList[ AVSI_SI_SA_ABM_GUILLOTINE_PRESS_END ];
			endStateData.m_SPLevel		= iSkillTempletLevel;
			endStateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			endStateData.m_eSkillID	= pSkillTemplet->m_eID;
	#else // UPGRADE_SKILL_SYSTEM_2013
			UserUnitStateData& loopStateData = m_StateList[ AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP ];
			loopStateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			loopStateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			loopStateData.m_eSkillID	= pSkillTemplet->m_eID;

			UserUnitStateData& endStateData = m_StateList[ AVSI_SI_SA_ABM_GUILLOTINE_PRESS_END ];
			endStateData.m_SPLevel		= pSkillTemplet->m_iSkillLevel;
			endStateData.m_fPowerRate	= pSkillTemplet->m_fPowerRate;
			endStateData.m_eSkillID	= pSkillTemplet->m_eID;
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ABM_VITAL_DRAIN == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ABM_VITAL_DRAIN );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			UserUnitStateData& stateData = m_StateList[ AVSI_A_ABM_VITAL_DRAIN_SUCCESS ];
			stateData.m_SPLevel		= iSkillTempletLevel;
			stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
			stateData.m_eSkillID	= pSkillTemplet->m_eID;
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_AEM_METEOR_SHOWER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AEM_METEOR_SHOWER );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_SA_AHM_METEOSHOWER_LOOP ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_SA_AHM_METEOSHOWER_HYPER_LOOP ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}
		}
	}

	if ( true == bChangeAll_ || CX2SkillTree::SI_SA_AEM_LIGHTNING_SHOWER == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AEM_LIGHTNING_SHOWER );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_SA_AHM_LIGHTNING_SHOWER_LOOP ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_SA_AHM_LIGHTNING_SHOWER_HYPER_LOOP ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}
		}
	}
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 헬 드롭 조준 State 추가
	if ( true == bChangeAll_ || CX2SkillTree::SI_A_ADM_HELL_DROP == eSkillID_ )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADM_HELL_DROP );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_IDLE ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_LEFT ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_RIGHT ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_UP ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_DOWN ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_UP_LEFT ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_UP_RIGHT ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_DOWN_LEFT ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_DOWN_RIGHT ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}

			{
				UserUnitStateData& stateData = m_StateList[ AVSI_HELL_DROP_CONTROL_FIRE ];
				stateData.m_SPLevel		= iSkillTempletLevel;
				stateData.m_fPowerRate	= pSkillTemplet->GetSkillPowerRateValue( iSkillTempletLevel );
				stateData.m_eSkillID	= pSkillTemplet->m_eID;
			}
		}
	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
}

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
/*static*/
void	CX2GUArme_VioletMage::AppendDeviceToDeviceList( CKTDXDeviceDataList& listInOut_ )
{
	g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, L"Arme_ComboX3Up-1.X" );
	g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, L"Arme_ComboX3Up-2.X" );
	g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, L"Arme_ComboX3Up-3.X" );
#ifndef NO_MORE_EFFECT_HARD_CODING_TEST
	g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, L"Arme_SpecialAttack_LightningBolts2a-0.X" );
#endif	NO_MORE_EFFECT_HARD_CODING_TEST
}
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD


void CX2GUArme_VioletMage::InitDevice()
{
	CX2GUUser::InitDevice();

	CKTDXDeviceXSkinMesh* pMesh1 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Arme_ComboX3Up-1.X" );
	CKTDXDeviceXSkinMesh* pMesh2 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Arme_ComboX3Up-2.X" );
	CKTDXDeviceXSkinMesh* pMesh3 = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Arme_ComboX3Up-3.X" );
	DeviceReady( pMesh1 );
	DeviceReady( pMesh2 );
	DeviceReady( pMesh3 );

	m_pComboX3Up1			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pComboX3Up2			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pComboX3Up3			= CKTDGXSkinAnim::CreateSkinAnim();

	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pComboX3Up1 );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pComboX3Up2 );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pComboX3Up3 );

	m_pComboX3Up1->SetAnimXSkinMesh( pMesh1 );
	m_pComboX3Up1->AddModelXSkinMesh( pMesh1 );
	m_pComboX3Up2->SetAnimXSkinMesh( pMesh2 );
	m_pComboX3Up2->AddModelXSkinMesh( pMesh2 );
	m_pComboX3Up3->SetAnimXSkinMesh( pMesh3 );
	m_pComboX3Up3->AddModelXSkinMesh( pMesh3 );

	m_pComboX3Up1->SetAlphaObject( true );
	m_pComboX3Up1->SetLayer( XL_EFFECT_0 );
	m_pComboX3Up2->SetAlphaObject( true );
	m_pComboX3Up2->SetLayer( XL_EFFECT_0 );
	m_pComboX3Up3->SetAlphaObject( true );
	m_pComboX3Up3->SetLayer( XL_EFFECT_0 );

	m_pComboX3Up1->GetRenderParam()->bAlphaBlend	= true;
	//m_pComboX3Up1->GetRenderParam()->bZEnable		= false;
	m_pComboX3Up1->GetRenderParam()->bZWriteEnable	= false;
	m_pComboX3Up1->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pComboX3Up1->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pComboX3Up1->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;

	m_pComboX3Up2->GetRenderParam()->bAlphaBlend	= true;
	//m_pComboX3Up2->GetRenderParam()->bZEnable		= false;
	m_pComboX3Up2->GetRenderParam()->bZWriteEnable	= false;
	m_pComboX3Up2->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pComboX3Up2->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pComboX3Up2->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;

	m_pComboX3Up3->GetRenderParam()->bAlphaBlend	= true;
	//m_pComboX3Up3->GetRenderParam()->bZEnable		= false;
	m_pComboX3Up3->GetRenderParam()->bZWriteEnable	= false;
	m_pComboX3Up3->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pComboX3Up3->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pComboX3Up3->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;

#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
	
	CKTDXDeviceXSkinMesh* pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Arme_SpecialAttack_LightningBolts2a-0.X" );
	DeviceReadyInBackground( pMesh );

	m_pLightningBolt = CKTDGXSkinAnim::CreateSkinAnim();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pLightningBolt );

	m_pLightningBolt->SetAnimXSkinMesh( pMesh );
	m_pLightningBolt->AddModelXSkinMesh( pMesh );

	m_pLightningBolt->SetAlphaObject( true );
	m_pLightningBolt->SetLayer( XL_EFFECT_0 );

	m_pLightningBolt->GetRenderParam()->bAlphaBlend		= true;
	//m_pLightningBolt->GetRenderParam()->bZEnable		= false;
	m_pLightningBolt->GetRenderParam()->bZWriteEnable	= false;
	m_pLightningBolt->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pLightningBolt->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pLightningBolt->GetRenderParam()->destBlend		= D3DBLEND_DESTALPHA;
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

}

void CX2GUArme_VioletMage::CommonStateStartFuture()
{
	CX2GUUser::CommonStateStartFuture();
}

void CX2GUArme_VioletMage::CommonStateStart()
{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편 
	switch ( GetNowStateID() )
	{
		case AVSI_HELL_DROP_CONTROL_IDLE :
		case AVSI_HELL_DROP_CONTROL_LEFT :
		case AVSI_HELL_DROP_CONTROL_RIGHT :
		case AVSI_HELL_DROP_CONTROL_UP :
		case AVSI_HELL_DROP_CONTROL_DOWN :
		case AVSI_HELL_DROP_CONTROL_UP_LEFT :
		case AVSI_HELL_DROP_CONTROL_UP_RIGHT :
		case AVSI_HELL_DROP_CONTROL_DOWN_LEFT :
		case AVSI_HELL_DROP_CONTROL_DOWN_RIGHT :
			if ( false == m_bIsHellDropTimerStart )
			{
				// 헬 드롭 시전 중 난입한 유저의 경우, 타이머가 설정이 안되어 있다.
				// 즉, 해당 State 인데 타이머 설정이 안되있는 경우에는 타이머를 켜주고
				// 이펙트를 바로 생성하게 한다.
				m_TimerUFOControl.restart();
				m_bIsHellDropTimerStart = true;
				m_fHellDropCreateEffectCooltime = _CONST_AISHA_::CREATE_HELL_DROP_DAMAGE_EFFECT_START_TIME;
			}
			break;
		default:
			break;
	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	CX2GUUser::CommonStateStart();
}

void CX2GUArme_VioletMage::CommonFrameMoveFuture()
{
	CX2GUUser::CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::CommonFrameMove()
{
	CX2GUUser::CommonFrameMove();



#ifdef AISHA_SECOND_CLASS_CHANGE

	if( NULL != m_pStrongMindData )
	{
		m_pStrongMindData->OnFrameMove( m_fTime, m_fElapsedTime );
	}



	for( int i=0; i< (int)m_vecStoneWallData.size(); i++ )
	{
		StoneWallData& stoneWallData = m_vecStoneWallData[i];
		stoneWallData.OnFrameMove( m_fTime, m_fElapsedTime );

		if( stoneWallData.m_fTimeLeft <= 0.f )
		{
			if( NULL != g_pX2Game )
			{
				CX2GUNPC* pNPC = g_pX2Game->GetNPCUnitByUID( (int) stoneWallData.m_NPCUID );
				
				if( NULL != pNPC )
				{
					pNPC->SetNowHp( 0.f );
				}

				
			}

			m_vecStoneWallData.erase( m_vecStoneWallData.begin() + i );
			--i;
		}
	}

	//*
	if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
		GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO21) == false &&
#endif //ADD_MEMO_1ST_CLASS
		NULL != GetUFO() )
	{
		//const double MAGIC_HELL_DROP_CHECK_TIME = 1.0;
		//if( m_TimerCheckInvalidUFO.elapsed() > MAGIC_HELL_DROP_CHECK_TIME )
		{
			//m_TimerCheckInvalidUFO.restart();

			//if( false == m_bIsSummoningUFO )
			{
				switch( GetNowStateID() )
				{
				case AVSI_HELL_DROP_CONTROL_IDLE:
				case AVSI_HELL_DROP_CONTROL_LEFT:
				case AVSI_HELL_DROP_CONTROL_RIGHT:
				case AVSI_HELL_DROP_CONTROL_UP:
				case AVSI_HELL_DROP_CONTROL_DOWN:
				case AVSI_HELL_DROP_CONTROL_UP_LEFT:
				case AVSI_HELL_DROP_CONTROL_UP_RIGHT:
				case AVSI_HELL_DROP_CONTROL_DOWN_LEFT:
				case AVSI_HELL_DROP_CONTROL_DOWN_RIGHT:
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 헬 드롭, 발사 이후 바로 사망 에서 발사 시간 지나면 사망으로 변경에 따라 변경
				case AVSI_HELL_DROP_CONTROL_FIRE:			
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				case USI_SPECIAL_ATTACK_1:				/// A슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_HYPER_1:			/// A슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 각성 스테이트
				case USI_SPECIAL_ATTACK_2:				/// A슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_HYPER_2:			/// A슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 각성 스테이트
				case USI_SPECIAL_ATTACK_3:				/// A슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_HYPER_3:			/// A슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 각성 스테이트
				case USI_SPECIAL_ATTACK_4:				/// A슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_HYPER_4:			/// A슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 각성 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_1:			/// B슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_HYPER_1:	/// B슬롯의 첫번째 스킬슬롯(현재는A키)에 해당하는 각성 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_2:			/// B슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_HYPER_2:	/// B슬롯의 두번째 스킬슬롯(현재는S키)에 해당하는 각성 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_3:			/// B슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_HYPER_3:	/// B슬롯의 세번째 스킬슬롯(현재는D키)에 해당하는 각성 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_4:			/// B슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 스테이트
				case USI_SPECIAL_ATTACK_SLOTB_HYPER_4:	/// B슬롯의 네번째 스킬슬롯(현재는C키)에 해당하는 각성 스테이트
					{
						// do nothing;
					} break;

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 헬 드롭, 발사 이후 바로 사망 에서 발사 시간 지나면 사망으로 변경에 따라 변경
				case AVSI_HELL_DROP_CONTROL_DYING:
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				case AVSI_HELL_DROP_CONTROL_FIRE:
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편				

				default:
					{
						if( NULL != GetUFO()->GetNPCAI() && CX2NPCAI::NAT_ALLY == GetUFO()->GetNPCAI()->GetAIType() 
							&& static_cast<CX2AllyNPCAI*>( GetUFO()->GetNPCAI() )->GetAllyUnitUID() != GetUnitUID() )
						{
							ResetUFO();
						}
						else if( GetUFO()->GetNowHp() <= 0.f )
						{
							ResetUFO();
						}
						else
						{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
							// 헬 드롭, 발사 이후 바로 사망 에서 발사 시간 지나면 사망으로 변경에 따라 변경
							GetUFO()->StateChange( _CONST_AISHA_::UFO_DYING.c_str(), true );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
							GetUFO()->StateChange( _CONST_AISHA_::UFO_FIRE.c_str(), true );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
							ResetUFO();
						}
					} break;
				}
			}
		}
	}
	//*/

#endif AISHA_SECOND_CLASS_CHANGE

	
	if( m_pMPChargeA != NULL && m_pMPChargeB != NULL && m_pMPChargeC != NULL )
	{
		if( m_pMPChargeA->GetRenderParam()->color.a > 0.0f )
		{
			if( m_FrameDataNow.syncData.nowState == AVSI_CHARGE_MP )
			{
				m_pMPChargeA->GetMatrix().Move( GetMatrix().GetPos() );
				m_pMPChargeA->GetMatrix().Rotate( GetMatrix().GetRotate() );
			}
			else
			{
				m_pMPChargeA->GetRenderParam()->color.a = m_pMPChargeA->GetRenderParam()->color.a - (m_fElapsedTime * 2.0f);
			}
			m_pMPChargeA->OnFrameMove( m_fTime, m_fElapsedTime );
		}
		else
		{
			m_pMPChargeA->Stop();
		}
		if( m_pMPChargeB->GetRenderParam()->color.a > 0.0f )
		{
			if( m_FrameDataNow.syncData.nowState == AVSI_CHARGE_MP )
			{
				m_pMPChargeB->GetMatrix().Move( GetMatrix().GetPos() );
				m_pMPChargeB->GetMatrix().Rotate( GetMatrix().GetRotate() );
			}
			else
			{
				m_pMPChargeB->GetRenderParam()->color.a = m_pMPChargeB->GetRenderParam()->color.a - (m_fElapsedTime * 2.0f);
			}
			m_pMPChargeB->OnFrameMove( m_fTime, m_fElapsedTime );
		}
		else
		{
			m_pMPChargeB->Stop();
		}
		if( m_pMPChargeC->GetRenderParam()->color.a )
		{
			if( m_FrameDataNow.syncData.nowState == AVSI_CHARGE_MP )
			{
				m_pMPChargeC->GetMatrix().Move( GetMatrix().GetPos() );
				m_pMPChargeC->GetMatrix().Rotate( GetMatrix().GetRotate() );
			}
			else
			{
				m_pMPChargeC->GetRenderParam()->color.a = m_pMPChargeC->GetRenderParam()->color.a - (m_fElapsedTime * 2.0f);
			}
			m_pMPChargeC->OnFrameMove( m_fTime, m_fElapsedTime );
		}
		else
		{
			m_pMPChargeC->Stop();
		}
	}

	if( m_pEvasionAttackAnim != NULL )
		m_pEvasionAttackAnim->OnFrameMove( m_fTime, m_fElapsedTime );

	if( m_pComboX3Up1 != NULL )
	{
		float fAlpha = m_pComboX3Up1->GetMaxAnimationTime() - m_pComboX3Up1->GetNowAnimationTime();
		fAlpha = fAlpha / m_pComboX3Up1->GetMaxAnimationTime();
		m_pComboX3Up1->GetRenderParam()->color.a = fAlpha;
		m_pComboX3Up1->OnFrameMove( m_fTime, m_fElapsedTime );
	}
	if( m_pComboX3Up2 != NULL )
	{
		float fAlpha = 1.0f;
		if( m_pComboX3Up2->GetNowAnimationTime() > 0.2f )
		{
			fAlpha = 0.35f - 0.2f;
			fAlpha = ( 0.35f - m_pComboX3Up2->GetNowAnimationTime() ) / fAlpha;
		}
		m_pComboX3Up2->GetRenderParam()->color.a = fAlpha;
		m_pComboX3Up2->OnFrameMove( m_fTime, m_fElapsedTime );
	}
	if( m_pComboX3Up3 != NULL )
	{
		float fAlpha = 1.0f;
		if( m_pComboX3Up3->GetNowAnimationTime() > 0.3f )
		{
			fAlpha = 0.5f - 0.3f;
			fAlpha = ( 0.5f - m_pComboX3Up3->GetNowAnimationTime() ) / fAlpha;
		}
		m_pComboX3Up3->GetRenderParam()->color.a = fAlpha;
		m_pComboX3Up3->OnFrameMove( m_fTime, m_fElapsedTime );
	}
#ifdef NO_MORE_EFFECT_HARD_CODING_TEST
#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX

	if( m_pLightningBolt != NULL )
	{
		m_pLightningBolt->OnFrameMove( m_fTime, m_fElapsedTime );
	}
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST


	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

	if( pSeqArmeAirStepRight != NULL )
	{
		D3DXVECTOR3 rFoot;
		GetFramePos( &rFoot, m_pFrame_Bip01_R_Foot );
		pSeqArmeAirStepRight->SetPosition( rFoot );
	}
	if( pSeqArmeAirStepLeft != NULL )
	{
		D3DXVECTOR3 lFoot;
		GetFramePos( &lFoot, m_pFrame_Bip01_L_Foot );
		pSeqArmeAirStepLeft->SetPosition( lFoot );
	}

#ifdef SKILL_CASH_10_TEST
	

#ifndef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	if( m_sBlazeStepData.GetEnable() == true )
	{
		// 발 밑에 붙어있는 이펙트의 좌표를 유지시킨다.
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_sBlazeStepData.m_ahMajorMeshInstance );
		if( NULL != pMeshInst )
			pMeshInst->SetPos( GetBonePos(L"Bip01_Footsteps") );
		else
			m_sBlazeStepData.m_ahMajorMeshInstance = INVALID_MESH_INSTANCE_HANDLE;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_sBlazeStepData.m_ahMajorParticleInstance );
		if( NULL != pSeq )
			pSeq->SetPosition(GetBonePos(L"Bip01_Footsteps"));
		else
			m_sBlazeStepData.m_ahMajorParticleInstance = INVALID_PARTICLE_SEQUENCE_HANDLE;

		// 화염 이펙트를 붙인다.
		if( m_sBlazeStepData.CheckRefresh() == true )
		{
			// 캐릭터가 서 있는 위치의 라인맵 좌료플 얻어와서 이펙트를 생성.
			// (□□■□□)
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Arme_Blaze_Step_UnderStep01", m_FrameDataNow.unitCondition.landPosition );
			D3DXVECTOR3	vecTempPos;
			// (□■□■□)
			vecTempPos = m_FrameDataNow.unitCondition.landPosition;
			vecTempPos -= (GetDirVector() * 70.f);
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Arme_Blaze_Step_UnderStep02", vecTempPos );
			vecTempPos -= (GetDirVector() * 30.f);

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DE_ARME_BLAZE_STEP_03", m_sBlazeStepData.m_fPowerRate, 
				vecTempPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

			vecTempPos = m_FrameDataNow.unitCondition.landPosition;
			vecTempPos += (GetDirVector() * 70.f);
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Arme_Blaze_Step_UnderStep02", vecTempPos );
			vecTempPos += (GetDirVector() * 30.f);

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DE_ARME_BLAZE_STEP_03", m_sBlazeStepData.m_fPowerRate,
				vecTempPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

			// (■□□□■)
			vecTempPos = m_FrameDataNow.unitCondition.landPosition;
			vecTempPos -= (GetDirVector() * 140.f);
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Arme_Blaze_Step_UnderStep03", vecTempPos );
			vecTempPos = m_FrameDataNow.unitCondition.landPosition;
			vecTempPos += (GetDirVector() * 140.f);
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Flare_Arme_Blaze_Step_UnderStep03", vecTempPos );
		}
		//죽었으면 이펙트를 강제 종료한다.
		else if( GetNowHp() <= 0 )
		{
			m_sBlazeStepData.SetEnable(false);
			m_sBlazeStepData.DestroyEffect();
		}
		m_sBlazeStepData.CheckEnd();

	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편		
#endif SKILL_CASH_10_TEST

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	if( IsTransformed() )
	{		
		if( m_ptrTransformWeapon != NULL )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_ptrTransformWeapon->SetRenderParam( m_fTime, m_fElapsedTime, m_pXSkinAnim->GetRenderParam() );
			m_ptrTransformWeapon->OnFrameMove( m_fTime, m_fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_ptrTransformWeapon->SetRenderParam( m_fTime, m_fElapsedTime * m_AdvanceTimeCount, m_pXSkinAnim->GetRenderParam() );
			m_ptrTransformWeapon->OnFrameMove( m_fTime, m_fElapsedTime * m_AdvanceTimeCount );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		}
	}

	if ( GetTransformTime() > 0.f )
	{
		SetTransformTime( GetTransformTime() - m_fElapsedTime );
		if( GetTransformTime() < 0.f )
			SetTransformTime( 0.f );
	}
#endif SERV_ADD_ARME_BATTLE_MAGICIAN

#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION

#ifndef MODIFY_DISTORTION_DESTRUCTION
	if ( 0.f != m_fDistortionLifeTime )
	{
		if( m_timerDistortionSummoned.elapsed() > (double) m_fDistortionLifeTime )
		{
			m_fDistortionLifeTime = 0.f;
			SetSelfDestructSummonedNPC( CX2UnitManager::NUI_ADW_DISTORTION );
		}
	}
#endif // MODIFY_DISTORTION_DESTRUCTION

	if( m_fMagicStaffSize > 0.f && GetNowHp() > 0.f && !IsTransformed() )
	{
		if( m_hMagicalStaff == INVALID_MESH_INSTANCE_HANDLE )
		{		
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"aisha_P_magical_staff_M01", GetPos(), GetRotateDegree(), GetRotateDegree() );
			if( pMeshInst != NULL )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				m_hMagicalStaff = pMeshInst->GetHandle();
				pMeshInst->SetBoundingRadius( 0.0f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
				pMeshInst->OnFrameMove( g_pKTDXApp->GetTime(), 0.01f );

				SetSubAttackList( pMeshInst->GetXSkinAnim() );
			}
		}
		else
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMagicalStaff );
			if( NULL != pMeshInst )
			{
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
			}
			else
			{
				m_hMagicalStaff = INVALID_MESH_INSTANCE_HANDLE;
				m_SubAttackListSet.clear();
			}
		}		
	}
	else
	{
		if( m_hMagicalStaff != INVALID_MESH_INSTANCE_HANDLE )
		{
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMagicalStaff );
			m_hMagicalStaff = INVALID_MESH_INSTANCE_HANDLE;
			m_SubAttackListSet.clear();
		}
	}
#endif
}

void CX2GUArme_VioletMage::CommonStateEndFuture()
{
	CX2GUUser::CommonStateEndFuture();
}

void CX2GUArme_VioletMage::CommonStateEnd()
{
	CX2GUUser::CommonStateEnd();
}

//{{ robobeg : 2008-10-28
//void CX2GUArme_VioletMage::CommonRender()
/*virtual*/
RENDER_HINT CX2GUArme_VioletMage::CommonRender_Prepare()
//}} robobeg : 2008-10-28
{
// 	if( GetRemainHyperModeTime() > 0.0f )
// 	{
// 		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
// 		m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_RED;
// 		m_RenderParam.outLineColor		= 0xff993388;
// 	}
// 	else
// 	{
// 		m_RenderParam.renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
// 		if( m_NowStateData.stateID == AVSI_CHARGE_MP )
// 			m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL_COUNTER;
// 		else
// 			m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
// 		m_RenderParam.outLineColor		= 0xffffffff;
// 	}

//{{ robobeg : 2008-10-28
	//CX2GUUser::CommonRender();
    return CX2GUUser::CommonRender_Prepare();
//}} robobeg : 2008-10-28
}

/*virtual*/ bool CX2GUArme_VioletMage::SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition /*= CX2SkillTree::ASUT_ONLY_ON_LINE*/ )
{
	KTDXPROFILE();

	const int INVALID_SKILL_SLOT_INDEX = -1;

	int iPressedSkillSlotIndex = INVALID_SKILL_SLOT_INDEX;
	const CX2UserSkillTree::SkillSlotData* pSkillSlotData = NULL;

	CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;// 유저가 배운 스킬 트리

	if ( false == CommonSpecialAttackEventProcess( accessUserSkillTree, pSkillSlotData, iPressedSkillSlotIndex ) )
		return false;

	if( NULL == pSkillSlotData )
		return false;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( pSkillSlotData->m_eID );
	if( NULL == pSkillTemplet )
		return false;

#ifdef ADD_MEMO_1ST_CLASS //김창한
	const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseConditionBySkillTemplet = GetSkillUseCondition( pSkillTemplet );

	if( false == CheckSkillUseCondition( eActiveSkillUseCondition, eActiveSkillUseConditionBySkillTemplet ) )
		return false;	

#else //ADD_MEMO_1ST_CLASS
	{
		if( false == CheckSkillUseCondition( eActiveSkillUseCondition, pSkillTemplet ) )
			return false;	
	}
#endif //ADD_MEMO_1ST_CLASS

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

	//{{ JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련
#ifdef DUNGEON_RANK_NEW
	CountUsedSkill( iPressedSkillSlotIndex );
#endif DUNGEON_RANK_NEW
	//}} JHKang / 강정훈 / 2011/02/14 / 던전 랭크 개선 관련

	UpNowMp( -fMPConsume );

#ifdef BALANCE_PATCH_20120329

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

	if ( pSkillTemplet->m_eID == CX2SkillTree::SI_A_AHM_MEDITATION )
	{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 메디테이션 메모 - 메디테이션 쿨타임 감소
		float fReduceCoolTime = 0.f;	/// 쿨타임 감소치

		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO11 ) )
			fReduceCoolTime = _CONST_AISHA_::AEM_MEDITATION_REDUCE_COOLTIME_VALUE;		/// -15초 감소

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( 0.f < m_fElementalResearchRate )
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, ( pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) - fReduceCoolTime ) * m_fElementalResearchRate );
		else
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) - fReduceCoolTime );
#else //UPGRADE_SKILL_SYSTEM_2013
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime - fReduceCoolTime );
#endif //UPGRADE_SKILL_SYSTEM_2013
#else  BALANCE_ELEMENTAL_MASTER_20130117
		if ( IsInMeditation() )
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
#endif BALANCE_ELEMENTAL_MASTER_20130117
	}
	else
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( 0.f < m_fElementalResearchRate )
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) * m_fElementalResearchRate );
		else
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) );

#else //UPGRADE_SKILL_SYSTEM_2013
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

#else
	accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
#endif

#ifdef SERV_SKILL_NOTE
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO6 ) == true && pSkillTemplet->m_eID == CX2SkillTree::SI_A_AV_MANA_SHIELD )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		if ( 0.f < m_fElementalResearchRate )
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) + 5.f - m_fElementalResearchRate );
		else
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) + 5.f );

#else //UPGRADE_SKILL_SYSTEM_2013
		accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime + 5.f );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
#endif


#ifdef AISHA_SECOND_CLASS_CHANGE

	if ( m_bElementalFriendShip )
	{
		switch( pSkillTemplet->m_eType )
		{
		case CX2SkillTree::ST_SPECIAL_ACTIVE:
			{
				m_FrameDataFuture.syncData.nowAction = UAI_ELEMENTAL_FRIENDSHIP_UP;

			} break;
		}
	}


	if( NULL != m_pStrongMindData && 
		m_pStrongMindData->m_fTimeLeft <= 0.f ) 
	{
		float fRestoreMPRate = 0.f;
		switch( pSkillTemplet->m_eType )
		{
		case CX2SkillTree::ST_ACTIVE:
			{
				fRestoreMPRate = m_pStrongMindData->m_fActiveSkillRate;
			} break;

		case CX2SkillTree::ST_BUFF:
		case CX2SkillTree::ST_SPECIAL_ACTIVE:
			{
				fRestoreMPRate = m_pStrongMindData->m_fSpecialActiveSkillRate;
			} break;
		}


		if( GetRandomFloat() < fRestoreMPRate )
		{
			m_pStrongMindData->m_fTimeLeft = 3.f; // cool time 3초

			float fRestoredMP = fMPConsume * m_pStrongMindData->m_fMPRestoreRate;
			if( fRestoredMP > 0.f )
			{
				UpNowMp( fRestoredMP ); 

				if( NULL != g_pData->GetPicCharBlue() )
				{
					WCHAR wszText[64] = L"";
					StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"MP+ %d", (int) fRestoredMP );

					D3DXVECTOR3 pos = GetPos();
					pos.y += 50.f;

					g_pData->GetPicCharBlue()->DrawText( wszText, pos, GetDirVector(), CKTDGPicChar::AT_CENTER );

					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Strong_Mind_MP_Up", this );


					PlaySound( L"AISHA_VP_STRONG_MIND.ogg" );

				}
			}
		}
	}

#endif AISHA_SECOND_CLASS_CHANGE


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
#ifdef SERV_SKILL_USE_SUBQUEST
	Send_SKILL_USE_REQ( pSkillTemplet->m_eID );
#endif SERV_SKILL_USE_SUBQUEST

	return true;
}


// 상태변화가 없는 필살기는 하드코딩해서 예외 처리 한다
/*virtual*/ bool CX2GUArme_VioletMage::SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet )
{
	if( NULL == pSkillTemplet )
		return false;

	switch( pSkillTemplet->m_eID )
	{
	case CX2SkillTree::SI_A_AV_MANA_SHIELD:
		{
			m_FrameDataFuture.syncData.nowAction = UAI_MANA_SHIELD;
		} break;

#ifdef SKILL_30_TEST
	case CX2SkillTree::SI_A_AHM_MEDITATION:
		{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 메디테이션 즉시 기능 동작으로 수정
			m_FrameDataFuture.syncData.nowAction = UAI_AHM_MEDITATION_START;
#else  BALANCE_ELEMENTAL_MASTER_20130117
			if( IsInMeditation() )
			{
				m_FrameDataFuture.syncData.nowAction = UAI_AHM_MEDITATION_STOP;
			}
			else
			{
				m_FrameDataFuture.syncData.nowAction = UAI_AHM_MEDITATION_START;
			}
#endif BALANCE_ELEMENTAL_MASTER_20130117
		} break;
#endif SKILL_30_TEST

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case CX2SkillTree::SI_A_AEM_SHINING_BODY:
		{
			m_FrameDataFuture.syncData.nowAction = UAI_AHM_SHINING_BODY;
		} break;

	case CX2SkillTree::SI_A_AVP_SHADOW_BODY:
		{
			m_FrameDataFuture.syncData.nowAction = UAI_AVP_SHADOW_BODY;
		} break;

	case CX2SkillTree::SI_A_ADW_ENERGETIC_BODY:
		{
			m_FrameDataFuture.syncData.nowAction = UAI_ADW_ENERGETIC_BODY;
		} break;
#endif //UPGRADE_SKILL_SYSTEM_2013

	default:
		{
			return false;
		} break;
	}

	return true;
}



/*virtual*/ void CX2GUArme_VioletMage::NoStateChangeActionFrameMove()
{
	switch( m_FrameDataNow.syncData.nowAction )
	{
	case UAI_MANA_SHIELD:
		{
#ifdef RIDING_SYSTEM
			if ( false == GetRidingOn() )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AV_MANA_SHIELD );
				if( NULL != pSkillTemplet )
				{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					// 마나 실드 메모 버프 팩터 추가
					if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO6 ) == true )
					{
						SetBuffFactorToGameUnit( pSkillTemplet, 1 );
					}
					else
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					{
						SetBuffFactorToGameUnit( pSkillTemplet, 0 );
					}
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ManaShield", this ); 
					PlaySound( L"Aisha_ManaShield.ogg" );
				}
			}
#else //RIDING_SYSTEM
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AV_MANA_SHIELD );
			if( NULL != pSkillTemplet )
			{
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ManaShield", this ); 
				PlaySound( L"Aisha_ManaShield.ogg" );
			}
#endif //RIDING_SYSTEM

		} break;

#ifdef SKILL_30_TEST
	case UAI_AHM_MEDITATION_START:
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AHM_MEDITATION );

			if ( g_pX2Game->GetGameType() == CX2Game::GT_PVP )
				SetBuffFactorToGameUnit( pSkillTemplet, 2 );
			else
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		///  메디테이션 바로 동작으로 수정 - 취소 기능 제거
			m_bInMeditation = true;		/// 메디테이션 동작중 여부 설정
#endif BALANCE_ELEMENTAL_MASTER_20130117
		} break;

#ifndef BALANCE_ELEMENTAL_MASTER_20130117		/// 메디테이션 바로 동작으로 수정 - 취소 기능 제거
	case UAI_AHM_MEDITATION_STOP:
		{
			EraseBuffTempletFromGameUnit( BTI_BUFF_SI_A_AHM_MEDITATION );

			vector<CX2BuffFinalizerTempletPtr>	vecFinalizerPtr;
			GetBuffFinalizerTempletPtrList( BTI_DEBUF_SI_A_AHM_MEDITATION, vecFinalizerPtr );

			if ( !vecFinalizerPtr.empty() )
			{
				EraseBuffTempletFromGameUnit( BTI_DEBUF_SI_A_AHM_MEDITATION );
			}
		} break;
#endif  BALANCE_ELEMENTAL_MASTER_20130117

#endif SKILL_30_TEST



	case UAI_ELEMENTAL_FRIENDSHIP_UP:
		{
			if ( NULL != GetUnit()  )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_AEM_ELEMENTAL_FRIENDSHIP );
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				SetBuffFactorToGameUnit( pSkillTemplet, 1 );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
		} break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case UAI_AHM_SHINING_BODY:
		{
			ProcessShiningBodySkill();
		} break;

	case UAI_AVP_SHADOW_BODY:
		{
			ProcessShadowBodySkill();
		} break;

	case UAI_ADW_ENERGETIC_BODY:
		{
			ProcessEnergeticBodySkill();
		} break;
#endif //UPGRADE_SKILL_SYSTEM_2013

	}
}

/** @function : CX2GetActualMPConsume
	@brief : 사용한 스킬의 MP 소모량을 얻어옴
	@param : 사용한 스킬의 ID, 사용한 스킬의 레벨
	@return : 소모될 MP 수치
*/
/*virtual*/ float CX2GUArme_VioletMage::GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const
{
	float fMpConsumption = CX2GUUser::GetActualMPConsume( eSkillID_, iSkillLevel_ );

	switch( eSkillID_ )
	{
	case CX2SkillTree::SI_SA_COMMON_AURA_POWER_ACCEL:
	case CX2SkillTree::SI_SA_COMMON_AURA_MAGIC_ACCEL:				
	case CX2SkillTree::SI_SA_COMMON_AURA_SHIELD_ACCEL:	
	case CX2SkillTree::SI_SA_COMMON_AURA_SPEED_ACCEL:
		{
			const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_SUPPORT_AURA );
			if ( iSkillLevel > 0 )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_SUPPORT_AURA );

				if( NULL != pSkillTemplet )
				{
					const float fAuraMpConsumptionMultiplier 
						= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL, iSkillLevel );

					fMpConsumption *= fAuraMpConsumptionMultiplier;
				}
	#else // UPGRADE_SKILL_SYSTEM_2013
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_SUPPORT_AURA, iSkillLevel );

				if( NULL != pSkillTemplet )
				{
					const float fAuraMpConsumptionMultiplier 
						= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_CONSUME_REL );

					fMpConsumption *= fAuraMpConsumptionMultiplier;
				}
	#endif // UPGRADE_SKILL_SYSTEM_2013
			}
		} break;

	case CX2SkillTree::SI_A_AV_TELEPORT:
		{
			fMpConsumption *= m_fMPConsumeRateTeleport;

#ifndef BALANCE_ELEMENTAL_MASTER_20130117		/// 메디테이션 패널티 삭제로 인한 미사용 구문
			//메디테이션 사용중 텔레포트를 사용하면 mp 소모를 3배이상 많게 한다.
			if( IsInMeditation() )
				fMpConsumption *= 3.f;
#endif BALANCE_ELEMENTAL_MASTER_20130117

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
			vector<KBuffFactor> VecDebuffFactor;
			GetBuffFactorOnlyDebuff( VecDebuffFactor );

			BOOST_FOREACH( const KBuffFactor& kFactor_, VecDebuffFactor )
			{
				BOOST_FOREACH( const KBuffBehaviorFactor& kBehavior_, kFactor_.m_vecBuffBehaviorFactors )
				{
					if ( kBehavior_.m_uiType == BBT_CHANGE_CONSUME_MP_RATE
						&& static_cast<UINT>( kBehavior_.m_vecValues[1] ) == CX2SkillTree::SI_A_AV_TELEPORT )
						fMpConsumption = kBehavior_.m_vecValues[2];
				}
			}
#endif //FIX_SKILL_BALANCE_AISHA_LENA

#ifdef SERV_SKILL_NOTE
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO5 ) == true )
			{
				fMpConsumption *= 1.4f;
			}
#endif
		} break;
	case CX2SkillTree::SI_A_ADM_MANA_INTAKE:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO12 ) == true )
			{
				fMpConsumption -= 10.f;
			}
		}
		break;

#ifdef ADDITIONAL_MEMO
	case CX2SkillTree::SI_SA_ABM_ENERGY_SPURT:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO15 ) == true )
			{
				fMpConsumption += 20.f;
			}
		}
		break;
#endif
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 체인 파이어볼이 파이어 볼 강화 패시브 영향을 받게 됨
	case CX2SkillTree::SI_A_AV_CHAIN_FIRE_BALL :
		{
			// 파이어볼 강화 패시브 영향을 받게 됨, 파볼 MP 감소율의 절반을 받음
			fMpConsumption -= fMpConsumption * (1 - m_fMPConsumeRateFireBall) / 2;
		}
		break;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
#ifdef ADD_MEMO_1ST_CLASS //김창한
	case CX2SkillTree::SI_SA_ADM_AGING:
		{
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO22 ) == true )
				fMpConsumption -= 20.f;
		}
		break;
#endif //ADD_MEMO_1ST_CLASS
	default:
		break;
	}

	if( fMpConsumption < 0.f )
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

void CX2GUArme_VioletMage::StartMeditation( bool bStartMeditation )
{
	if ( bStartMeditation )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AHM_MEDITATION );
		if ( NULL != pSkillTemplet )
		{
			m_bInMeditation = bStartMeditation;

#ifndef BALANCE_ELEMENTAL_MASTER_20130117		/// 엠피 회복을 버프로 변경
			m_fMeditationMPChangeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_REGENERATION_ABS );
			ResetMPChangeRate( (GetOriginalMPChangeRate() + GetMeditationMPChangeRate() + m_fAddMPChangeRate) * (1.f + m_SocketData.m_fSpeedUpManaGather) );
#endif BALANCE_ELEMENTAL_MASTER_20130117

			if( false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO11 ) )
				SetBuffFactorToGameUnit( pSkillTemplet, 1 );
		}			
		PlaySound( L"Aisha_Meditation.ogg" );
	}
	else
	{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 엠피 회복을 버프 기능으로 변경 및 쿨타임 초기화
		/// 쿨타임 초기화
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AHM_MEDITATION );

		if( NULL != pSkillTemplet && NULL != GetUnit() )
		{
			CX2UserSkillTree& accessUserSkillTree =  GetUnit()->AccessUnitData().m_UserSkillTree;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillTempletLevel = max( 1, accessUserSkillTree.GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// 스킬 레벨
	
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel ) );
	#else // UPGRADE_SKILL_SYSTEM_2013
			accessUserSkillTree.SetSkillCoolTimeLeft( pSkillTemplet->m_eID, pSkillTemplet->m_fSkillCoolTime );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

		m_bInMeditation = bStartMeditation;
#else  BALANCE_ELEMENTAL_MASTER_20130117

		m_bInMeditation = bStartMeditation;		// 해외팀 메디테이션 추가
		m_fMeditationMPChangeRate = 0.0f;
		ResetMPChangeRate( (GetOriginalMPChangeRate() + m_fAddMPChangeRate) * (1.f + m_SocketData.m_fSpeedUpManaGather) );
#endif BALANCE_ELEMENTAL_MASTER_20130117
	}		
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//							스테이트 함수 시작

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//AVSI_DIE_FRONT
void CX2GUArme_VioletMage::DieFrontStartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GUArme_VioletMage::DieFrontStart()
{
	CommonStateStart();
	DyingStart();

	m_pXSkinAnim->SetPlaySpeed( 0.5f );
}

void CX2GUArme_VioletMage::DieFrontFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GUArme_VioletMage::DieFrontEventProcess()
{
	if( EventTimer( 4.0f, true ) == true )
	{
		StateChange( GUSI_DIE );
	}
}


//AVSI_DIE_BACK
void CX2GUArme_VioletMage::DieBackStartFuture()
{
	CommonStateStartFuture();

	m_pXSkinAnimFuture->SetPlaySpeed( 0.5f );
}

void CX2GUArme_VioletMage::DieBackStart()
{
	CommonStateStart();
	DyingStart();

	m_pXSkinAnim->SetPlaySpeed( 0.5f );
}

void CX2GUArme_VioletMage::DieBackFrameMove()
{
	const float TIME_CREATE_STEP_DUST = 0.19f;		// 연기가 발생되야하는 애니메이션 시간
	const float TIME_SACRIFICE_OF_HERO = 1.12f;		// 영웅의 희생이 실행되어야 하는 애니메이션 시간

	DieFrameMove( TIME_CREATE_STEP_DUST, TIME_SACRIFICE_OF_HERO );
}

void CX2GUArme_VioletMage::DieBackEventProcess()
{
	if( EventTimer( 4.0f, true ) == true )
	{
		StateChange( GUSI_DIE );
	}
}

void CX2GUArme_VioletMage::WaitStartFuture()
{
	CommonStateStartFuture();
	PlayAnimationWaitStart( m_pXSkinAnimFuture, m_FrameDataFuture );

	m_fMPChargeWaitTime = 0.0f;
}

void CX2GUArme_VioletMage::WaitStart()
{
	CommonStateStart();
	PlayAnimationWaitStart( m_pXSkinAnim.get(), m_FrameDataNow );
}

void CX2GUArme_VioletMage::WaitFrameMoveFuture()
{
	CommonFrameMoveFuture();

	m_fMPChargeWaitTime += m_fElapsedTime;
}

void CX2GUArme_VioletMage::WaitEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_COMBO_X1 );
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
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
	else if( m_fMPChargeWaitTime >= 0.5f && GetNowMp() != GetMaxMp() && g_pX2Game->GetSlideShot()->IsPresentNow() == false )
	{
#ifdef UNIT_EMOTION
		// 이모션 동작시에는 charge_mp 취소시키고 이모션 동작시킨다.
		if(m_ePlayedEmotion == CX2Unit::ET_NONE)
#endif
			StateChange( AVSI_CHARGE_MP );
	}
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

void CX2GUArme_VioletMage::WalkEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_COMBO_X1, false );
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
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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


//AVSI_JUMP_READY
void CX2GUArme_VioletMage::JumpReadyEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_JUMP_ATTACK_Z );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_JUMP_ATTACK_X );
		m_PhysicParam.nowSpeed.y = GetJumpSpeed();
		m_FrameDataFuture.syncData.position.y += LINE_RADIUS;
	}
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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

void CX2GUArme_VioletMage::JumpUpEventProcess()
{
	if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_JUMP_ATTACK_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
#ifdef WALL_JUMP_TEST
	else if( m_InputData.oneUp == true && m_FrameDataFuture.unitCondition.bFootOnWall == false )
#else
	else if( m_InputData.oneUp == true ) 
#endif WALL_JUMP_TEST
	{
		StateChange( AVSI_JUMP_LEVITATION );
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

void CX2GUArme_VioletMage::JumpDownEventProcess()
{
	if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			if( m_InputData.pureLeft == true )
			{
				StateChangeDashIfPossible();
			}
			else if( m_InputData.pureRight == true )
			{
				StateChangeDashIfPossible();
			}
			else
			{
				StateChange( USI_JUMP_LANDING );
			}
		}
		else
			StateChange( USI_JUMP_LANDING );
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_JUMP_ATTACK_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
#ifdef WALL_JUMP_TEST
	else if( m_FrameDataFuture.unitCondition.bFootOnWall == true &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( AVSI_WALL_LANDING );
	}
#endif WALL_JUMP_TEST
	else if( m_InputData.oneUp == true )
	{
		StateChange( AVSI_JUMP_LEVITATION );
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

//AVSI_JUMP_LEVITATION
void CX2GUArme_VioletMage::JumpLevitationStartFuture()
{
	CommonStateStartFuture();
	m_fLevitationElapsedTime		= _CONST_AISHA_::AISHA_LEVITATION_CONSUME_MP_TIME;
}


void CX2GUArme_VioletMage::JumpLevitationFrameMoveFuture()
{
	m_fLevitationElapsedTime	+= m_fElapsedTime;

	if( m_InputData.pureRight == true )
	{
		m_FrameDataFuture.syncData.bIsRight = true;
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	else if( m_InputData.pureLeft == true )
	{
		m_FrameDataFuture.syncData.bIsRight = false;
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	if( IsTransformed() )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
	}
	else
	{
		if( m_fLevitationElapsedTime > _CONST_AISHA_::AISHA_LEVITATION_CONSUME_MP_TIME )
		{
			if( FlushMp( 5.0f * _CONST_AISHA_::AISHA_LEVITATION_CONSUME_MP_TIME ) == true )
			{
				m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
				m_fLevitationElapsedTime = 0.f;
			}
		}
		else
		{
			m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
		}
	}
#else
	if( FlushMp( 20.0f * m_fElapsedTime ) == true )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
	}
#endif
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::JumpLevitationFrameMove()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

	if( pSeqArmeAirStepRight != NULL )
	{
		pSeqArmeAirStepRight->SetEmitRate(10,20); 
	}

	if( pSeqArmeAirStepLeft != NULL )
	{
		pSeqArmeAirStepLeft->SetEmitRate(10,20); 
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::JumpLevitationEventProcess()
{
	if( m_InputData.pureDoubleLeft == true || m_InputData.pureDoubleRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( true == IsOnSomethingFuture() )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			if( m_InputData.pureLeft == true )
			{
				StateChangeDashIfPossible();
			}
			else if( m_InputData.pureRight == true )
			{
				StateChangeDashIfPossible();
			}
			else
			{
				StateChange( USI_JUMP_LANDING );
			}
		}
		else
			StateChange( USI_JUMP_LANDING );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_JUMP_ATTACK_X );
	}
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
	else if( m_InputData.pureUp == false )
	{
		StateChange( USI_JUMP_DOWN );
	}
	else if( m_PhysicParam.nowSpeed.y >= 0.0f )
	{
		StateChange( USI_JUMP_UP );
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::JumpLevitationEnd()
{
	CommonStateEnd();
	
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

	if( pSeqArmeAirStepRight != NULL )
	{
		pSeqArmeAirStepRight->SetEmitRate(0,0); 
	}

	if( pSeqArmeAirStepLeft != NULL )
	{
		pSeqArmeAirStepLeft->SetEmitRate(0,0); 
	}
}

void CX2GUArme_VioletMage::JumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_COMBO_X1 );
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
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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
		StateChange( USI_WAIT );
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::DashEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_DASH_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_DASH_ATTACK_X );
	}
	else if( SpecialAttackEventProcess() == true )
	{
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
	else if( m_InputData.oneUp == true )
	{
		StateChange( USI_DASH_JUMP );
		m_PhysicParam.nowSpeed.y = GetDashJumpSpeed();
	}	
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


//AVSI_DASH_END
void CX2GUArme_VioletMage::DashEndStart()
{
	CommonStateStart();

	//m_pSoundStep->Set3DPosition( m_FrameDataNow.syncData.position );

}

void CX2GUArme_VioletMage::DashEndFrameMoveFuture()
{
	if( m_FrameDataFuture.unitCondition.fStateTime < 0.36f )
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 0.7f;

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::DashEndFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.23f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.23f ) == true && EventCheck( 0.23f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		CreateStepDust();
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashEndEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 3.0f )
		{
			StateChange( AVSI_COMBO_Z1 );
		}
		else
		{
			StateChange( AVSI_DASH_COMBO_Z1 );
		}
	}
	else if( m_InputData.oneX == true )
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 3.0f )
		{
			StateChange( AVSI_COMBO_X1 );
		}
		else
		{
			StateChange( AVSI_DASH_ATTACK_X );
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
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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





//AVSI_DASH_JUMP
void CX2GUArme_VioletMage::DashJumpStartFuture()
{
	CommonStateStartFuture();

	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * 1.1f;
	m_bDownForce					= false;
}

void CX2GUArme_VioletMage::DashJumpStart()
{
	CommonStateStart();

	//m_pSoundStep->Set3DPosition( m_FrameDataNow.syncData.position );


	if( GetMatrix().GetYPos() == m_FrameDataNow.unitCondition.landPosition.y )
	{
		CreateStepDust();
	}
}

void CX2GUArme_VioletMage::DashJumpFrameMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataNow.syncData.bIsRight	== true )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.2f;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataNow.syncData.bIsRight	== false )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.2f;
	}

	if( m_bDownForce == true )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::DashJumpEventProcess()
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
			StateChange( AVSI_DASH_JUMP_POWER_LANDING );
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
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_DASH_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_DASH_JUMP_ATTACK_X );
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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
	if( m_InputData.oneUp == true && m_FrameDataFuture.syncData.position.y - m_FrameDataFuture.unitCondition.landPosition.y < LINE_RADIUS * 2.0f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#ifdef WALL_JUMP_TEST
	else if( m_FrameDataFuture.unitCondition.bFootOnWall == true &&
		( true == m_InputData.oneUp || true == m_InputData.oneDoubleUp ) )
	{
		m_bDisableGravity = true;
		StateChange( AVSI_WALL_LANDING );
	}
#endif WALL_JUMP_TEST
	else if( m_InputData.oneUp == true )
	{
		StateChange( AVSI_DASH_JUMP_LEVITATION );
	}
	if( m_InputData.oneDown == true )
	{
		m_PhysicParam.fMaxGSpeed = m_OrgPhysicParam.fMaxGSpeed * 2.0f;
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

void CX2GUArme_VioletMage::DashJumpEndFuture()
{
	CommonStateEnd();
	m_PhysicParam.fDownAccel	= 0.0f;
}




//AVSI_DASH_JUMP_LEVITATION
void CX2GUArme_VioletMage::DashJumpLevitationStartFuture()
{
	CommonStateStartFuture();
	m_PhysicParam.fDownAccel		= m_PhysicParam.fGAccel * 1.1f;
	m_fLevitationElapsedTime		= _CONST_AISHA_::AISHA_LEVITATION_CONSUME_MP_TIME;
}

void CX2GUArme_VioletMage::DashJumpLevitationFrameMoveFuture()
{
	m_fLevitationElapsedTime	+= m_fElapsedTime;

	if( m_InputData.pureRight == true && m_FrameDataNow.syncData.bIsRight	== true )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.2f;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataNow.syncData.bIsRight	== false )
	{
		m_PhysicParam.nowSpeed.x			= GetDashSpeed() * 1.2f;
	}

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	if( IsTransformed() )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
	}
	else
	{
		if( m_fLevitationElapsedTime > _CONST_AISHA_::AISHA_LEVITATION_CONSUME_MP_TIME )
		{
			if( FlushMp( 5.0f * _CONST_AISHA_::AISHA_LEVITATION_CONSUME_MP_TIME ) == true )
			{
				m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
				m_fLevitationElapsedTime = 0.f;
			}
		}
		else
		{
			m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
		}
	}
#else
	if( FlushMp( 30.0f * m_fElapsedTime ) == true )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed / 40.0f;
	}
#endif

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::DashJumpLevitationFrameMove()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

	if( pSeqArmeAirStepRight != NULL )
	{
		pSeqArmeAirStepRight->SetEmitRate(10,20); 
	}

	if( pSeqArmeAirStepLeft != NULL )
	{
		pSeqArmeAirStepLeft->SetEmitRate(10,20); 
	}


	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashJumpLevitationEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		if( m_bDownForce == true )
		{
			StateChange( AVSI_DASH_JUMP_POWER_LANDING );
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
		}
	}
	else if( true == SpecialAttackEventProcess( CX2SkillTree::ASUT_AIR ) )
	{
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_DASH_JUMP_ATTACK_Z );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_DASH_JUMP_ATTACK_X );
	}
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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
	if( m_InputData.pureUp == false )
	{
		StateChange( USI_DASH_JUMP );
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::DashJumpLevitationEnd()
{

	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

	if( pSeqArmeAirStepRight != NULL )
	{
		pSeqArmeAirStepRight->SetEmitRate(0, 0); 
	}

	if( pSeqArmeAirStepLeft != NULL )
	{
		pSeqArmeAirStepLeft->SetEmitRate(0, 0); 
	}

	CommonStateEnd();
}

void CX2GUArme_VioletMage::DashJumpLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_COMBO_X1 );
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
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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


//AVSI_DASH_JUMP_POWER_LANDING
void CX2GUArme_VioletMage::DashJumpPowerLandingStart()
{
	CommonStateStart();

	CreateStepDust();
}

void CX2GUArme_VioletMage::DashJumpPowerLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > m_pXSkinAnimFuture->GetMaxAnimationTime() / 2.0f )
	{
		if( m_InputData.oneZ == true )
		{
			StateChange( AVSI_COMBO_Z1 );
		}
		else if( m_InputData.oneX == true )
		{
			StateChange( AVSI_COMBO_X1 );
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
		////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
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

//AVSI_HYPER_MODE
void CX2GUArme_VioletMage::HyperModeInit()
{
	TextureReadyInBackground( L"ColorBallBlue.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
	TextureReadyInBackground( L"WhiteCircle.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"ToKang.dds" );
	TextureReadyInBackground( L"Energy_Violet.dds" );
}

void CX2GUArme_VioletMage::HyperModeFrameMove()
{
#ifdef MODIFY_RIDING_PET_AWAKE
	CommonHyperModeFrameMove( 0.47f, 0.63f );
#else
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.47f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.47f ) == true && EventCheck(0.47f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{		
		ShowMinorParticleHyperModeChange();
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.63f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.63f ) == true && EventCheck(0.63f, false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		UpDownCrashCamera( 20.0f, 0.3f );
		g_pKTDXApp->GetDGManager()->ClearScreen();
		
		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();

//{{ 김상훈 2010.10.29 : 극한의 마나 운용
#ifdef NEW_SKILL_2010_11
#ifdef HYPER_MODE_FIX
		m_fRateLimitManaManagement = GetLastStateHyperModeCount() * 0.33f;
#else
		m_fRateLimitManaManagement = m_FrameDataNow.syncData.m_HyperModeCount * 0.33f;
#endif HYPER_MODE_FIX
		float rdf = GetRandomFloat();
		if ( rdf <= m_fRateLimitManaManagement )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT, true  );
	#else // UPGRADE_SKILL_SYSTEM_2013
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			
 			if( iSkillLevel > 0 )
 			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT );

				if( NULL != pSkillTemplet )
				{
					m_iHyperChargeMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_RECOVER_MP_ABS, iSkillLevel );
					m_iHyperAddMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_MP_ABS, iSkillLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT, iSkillLevel );

				if( NULL != pSkillTemplet )
				{
					m_iHyperChargeMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_RECOVER_MP_ABS );
					m_iHyperAddMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_MP_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
	#ifdef FIX_LIMITED_MANA_MANAGEMENT
					SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	#else
					SetLimitManaManagement( true );
					ResetMaxMP();
	#endif //FIX_LIMITED_MANA_MANAGEMENT
					//GetGageManager()->UpdateMyManaGuageUI();
					UpNowMp( m_iHyperChargeMP );
				


					std::wstringstream strStream;
					std::wstringstream strStream2;
					strStream << L"MAXMP+ "<< static_cast<int>(m_iHyperAddMP);
					D3DXVECTOR3 pos = GetPos();
					pos.y += 50.0f; 
					if( NULL != g_pData->GetPicCharBlue() )
						g_pData->GetPicCharBlue()->DrawText( strStream.str().c_str(), pos, GetDirVector(), CKTDGPicChar::AT_CENTER );

								
					pos.y += 30.0f; 
					strStream2 <<  "MP+ " << static_cast< int >( m_iHyperChargeMP );
					if( NULL != g_pData->GetPicCharBlue() )
						g_pData->GetPicCharBlue()->DrawText( strStream2.str().c_str(), pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
				}
 			}
		}
#endif NEW_SKILL_2010_11
//}} 김상훈 2010.10.29 : 극한의 마나 운용
	}
	CommonFrameMove();	
#endif // MODIFY_RIDING_PET_AWAKE
}

void CX2GUArme_VioletMage::HyperModeCameraMove()
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
	}
}


//AVSI_CHARGE_MP
void CX2GUArme_VioletMage::ChargeMPInit()
{
	CKTDXDeviceXSkinMesh* pChargeMPA = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"ChargeMPA.X" );
	CKTDXDeviceXSkinMesh* pChargeMPB = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"ChargeMPB.X" );
	DeviceReady( pChargeMPA );
	DeviceReady( pChargeMPB );

	m_pMPChargeA			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pMPChargeB			= CKTDGXSkinAnim::CreateSkinAnim();
	m_pMPChargeC			= CKTDGXSkinAnim::CreateSkinAnim();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMPChargeA );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMPChargeB );
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pMPChargeC );

	m_pMPChargeA->SetAnimXSkinMesh( pChargeMPA );
	m_pMPChargeA->AddModelXSkinMesh( pChargeMPA );
	m_pMPChargeB->SetAnimXSkinMesh( pChargeMPB );
	m_pMPChargeB->AddModelXSkinMesh( pChargeMPB );
	m_pMPChargeC->SetAnimXSkinMesh( pChargeMPA );
	m_pMPChargeC->AddModelXSkinMesh( pChargeMPA );

	m_pMPChargeA->SetAlphaObject( true );
	m_pMPChargeA->SetLayer( XL_EFFECT_0 );

	m_pMPChargeB->SetAlphaObject( true );
	m_pMPChargeB->SetLayer( XL_EFFECT_0 );

	m_pMPChargeC->SetAlphaObject( true );
	m_pMPChargeC->SetLayer( XL_EFFECT_0 );


	m_pMPChargeA->GetRenderParam()->bAlphaBlend		= true;
	//m_pMPChargeA->GetRenderParam()->bZEnable		= false;
	m_pMPChargeA->GetRenderParam()->bZWriteEnable	= false;
	m_pMPChargeA->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMPChargeA->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMPChargeA->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;

	m_pMPChargeB->GetRenderParam()->bAlphaBlend		= true;
	//m_pMPChargeB->GetRenderParam()->bZEnable		= false;
	m_pMPChargeB->GetRenderParam()->bZWriteEnable	= false;
	m_pMPChargeB->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMPChargeB->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMPChargeB->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;

	m_pMPChargeC->GetRenderParam()->bAlphaBlend		= true;
	//m_pMPChargeC->GetRenderParam()->bZEnable		= false;
	m_pMPChargeC->GetRenderParam()->bZWriteEnable	= false;
	m_pMPChargeC->GetRenderParam()->cullMode		= D3DCULL_NONE;
	m_pMPChargeC->GetRenderParam()->srcBlend		= D3DBLEND_SRCALPHA;
	m_pMPChargeC->GetRenderParam()->destBlend		= D3DBLEND_INVSRCALPHA;
}

void CX2GUArme_VioletMage::ChargeMPStartFuture()
{
	CommonStateStartFuture();

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 버프에 있는 엠피 회복 기능을 사용하므로, 해당 부분은 필요 없다.
	const float fMPChangeRate = GetOriginalMPChangeRate();
#else  BALANCE_ELEMENTAL_MASTER_20130117
	const float fMPChangeRate = ( IsInMeditation() ? GetMeditationMPChangeRate() : GetOriginalMPChangeRate() );
#endif  BALANCE_ELEMENTAL_MASTER_20130117
	LUA_GET_VALUE( m_LuaManager, "ADD_MP_CHANGE_RATE", m_fAddMPChangeRate, 0.f );

	ResetMPChangeRate( (fMPChangeRate + m_fAddMPChangeRate) * (1.f + m_SocketData.m_fSpeedUpManaGather) );

// 	if( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON )
// 	{
// 		SetChangeRateMp( GetChangeRateMp() * 0.8f );		
// 	}

}

void CX2GUArme_VioletMage::ChargeMPStart()
{
	CommonStateStart();

	m_pMPChargeA->ChangeAnim( L"ChargeSoulA" );
	m_pMPChargeA->SetPlaySpeed( 0.7f );
	//m_pMPChargeA->Play( CKTDGXSkinAnim::XAP_LOOP );

	m_pMPChargeB->ChangeAnim( L"ChargeSoulB" );
	//m_pMPChargeB->Play( CKTDGXSkinAnim::XAP_LOOP );

	m_pMPChargeC->ChangeAnim( L"ChargeSoulA" );
	m_pMPChargeC->SetPlaySpeed( 0.4f );
	//m_pMPChargeC->Play( CKTDGXSkinAnim::XAP_LOOP );

	m_fMPChargeSoundTime = 1.5f;

	PlaySound( L"wind.ogg" );

	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2MagicCircle	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2MagicCircle );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up2	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up2 );

	if( pSeqMPCharge2MagicCircle != NULL )
	{
		pSeqMPCharge2MagicCircle->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		pSeqMPCharge2MagicCircle->SetEmitRate( CMinMax<float>(1.0f,1.0f) );
	}	

	if( pSeqMPCharge2Up != NULL )
	{
		pSeqMPCharge2Up->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		pSeqMPCharge2Up->SetEmitRate( CMinMax<float>(10.0f,10.0f) );
	}

	if( pSeqMPCharge2Up2 != NULL )
	{
		pSeqMPCharge2Up2->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		pSeqMPCharge2Up2->SetEmitRate( CMinMax<float>(10.0f,10.0f) );
	}

	m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL_COUNTER;
}

void CX2GUArme_VioletMage::ChargeMPFrameMove()
{
	D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"Center_Jewel", 0 );

	if( m_hSeqMPEnergy != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqMPEnergyCenter != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqMPChargeWave != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergy		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergyCenter = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergyCenter );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeWave	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeWave );

		CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2MagicCircle	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2MagicCircle );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up2	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up2 );


		if( NULL != pSeqMPEnergy )
		{
			pSeqMPEnergy->SetPosition( pos );
			pSeqMPEnergy->SetBlackHolePosition( pos );
			pSeqMPEnergy->SetLookPoint( pos );
		}

		if( NULL != pSeqMPEnergyCenter )
		{
			pSeqMPEnergyCenter->SetPosition( pos );
		}


		D3DXVECTOR3 pos2 = GetMatrix().GetPos();
		pos2.y += 10.0f;
		if( NULL != pSeqMPChargeWave )
		{
			pSeqMPChargeWave->SetPosition( pos2 );
		}

		if( pSeqMPCharge2MagicCircle != NULL )
			pSeqMPCharge2MagicCircle->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		if( pSeqMPCharge2Up != NULL )
			pSeqMPCharge2Up->SetPosition( m_FrameDataNow.unitCondition.landPosition );
		if( pSeqMPCharge2Up2 != NULL )
			pSeqMPCharge2Up2->SetPosition( m_FrameDataNow.unitCondition.landPosition );
	}

	float alpha = m_FrameDataNow.unitCondition.fStateTime * 2.0f;
	if( alpha > 0.7f )
		alpha = 0.7f;
	m_pMPChargeA->GetRenderParam()->color.a = alpha;

	if( m_FrameDataNow.unitCondition.fStateTime < 5.0f )
	{
		alpha = m_FrameDataNow.unitCondition.fStateTime;
		if( alpha > 1.0f )
			alpha = 1.0f;
		m_pMPChargeA->GetMatrix().Scale( 1.0f, alpha, 1.0f );

		alpha = m_FrameDataNow.unitCondition.fStateTime / 2.0f;
		if( alpha > 1.0f )
			alpha = 1.0f;
		m_pMPChargeB->GetMatrix().Scale( 1.0f, alpha, 1.0f );
	}
	else
	{
		alpha = m_FrameDataNow.unitCondition.fStateTime - 4.0f;
		if( alpha > 2.0f )
			alpha = 2.0f;
		m_pMPChargeA->GetMatrix().Scale( 1.0f, alpha, 1.0f );

		alpha = m_FrameDataNow.unitCondition.fStateTime - 4.0f;
		if( alpha > 2.0f )
			alpha = 2.0f;
		m_pMPChargeB->GetMatrix().Scale( 1.0f, alpha, 1.0f );
	}

	alpha = m_FrameDataNow.unitCondition.fStateTime;
	if( alpha > 1.0f )
		alpha = 1.0f;
	m_pMPChargeB->GetRenderParam()->color.a = alpha;

	alpha = m_pMPChargeC->GetNowAnimationTime() / m_pMPChargeC->GetMaxAnimationTime();
	m_pMPChargeC->GetRenderParam()->color.a = 1.0f - alpha;

	if( m_FrameDataNow.unitCondition.fStateTime < 5.0f )
	{
		m_pMPChargeC->GetMatrix().Scale( alpha * 2.5f, alpha * 2.0f, alpha * 2.5f );
	}
	else
	{
		m_pMPChargeC->GetMatrix().Scale( alpha * 3.0f, alpha * 3.0f, alpha * 3.0f );
		//if( GetDistance(GetPos(), g_pX2Game->GetMyUnit()->GetPos() ) < 500.0f )
		//	g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCameraNoReset( 2.0f, 0.1f );
	}

	if( m_FrameDataNow.unitCondition.fStateTime < 1.0f )
	{
		m_pMPChargeC->GetMatrix().Scale( 0.0f, 0.0f, 0.0f );
	}

	if( EventTimer(5.0f, false) == true )
	{
		m_pMPChargeB->SetPlaySpeed( 1.5f );
		m_pMPChargeC->SetPlaySpeed( 0.3f );
		//g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( 5.0f, 0.2f );
		//g_pX2Game->GetWorld()->SetWorldColor( 0xff333333 );
		//g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 2.0f );

		//m_pMPChargeWave->SetEmitRate( CMinMax<float>(3.0f,5.0f) );




		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergy		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
		CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergyCenter = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergyCenter );
		


		if( pSeqMPEnergy != NULL )
			pSeqMPEnergy->SetEmitRate( CMinMax<float>(5.0f,15.0f) );
		if( pSeqMPEnergyCenter != NULL )
			pSeqMPEnergyCenter->SetEmitRate( CMinMax<float>(5.0f,15.0f) );

		

		//CKTDGParticleSystem::CParticleEventSequence* pSeq;
		//pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"ImpactTickBlue", pos, 200, 200, 2, 20 );
		//if( pSeq != NULL )
		//	pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );

		//g_pKTDXApp->GetDGManager()->ClearScreen();
		//m_pSoundBreak->Set3DPosition( GetMatrix().GetPos() );
		//m_pSoundBreak->Play();

		PlaySound( L"Charge.ogg" );
	}

	m_fMPChargeSoundTime -= m_fElapsedTime;
	if( m_fMPChargeSoundTime <= 0.0f )
	{
		if( m_FrameDataNow.unitCondition.fStateTime > 5.0f )
		{
			PlaySound( L"Charge.ogg" );
		}
		PlaySound( L"wind.ogg" );

		m_fMPChargeSoundTime = 1.5f;
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ChargeMPEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_InputData.oneZ == true )
	{
		StateChange( AVSI_COMBO_Z1 );
	}
	else if( m_InputData.oneX == true )
	{
		StateChange( AVSI_COMBO_X1 );
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
		StateChange( USI_JUMP_READY );
	}
	ELSE_IF_CAN_HYPER_MODE_THEN_STATE_CHANGE( USI_HYPER_MODE )
	////ELSE_IF_CAN_USE_SPECIAL_ABILITY_THEN_STATE_CHANGE_OR_GUIDE( 10.f * m_fMPConsumeRateTeleport, AVSI_EVASION_START, ShowMagicFailGuide() )
	else if( GetNowMp() == GetMaxMp() ) 
	{	
		StateChange( USI_WAIT );
	}
	else if( g_pX2Game->GetSlideShot()->IsPresentNow() == true )
	{
		StateChange( USI_WAIT );
	}
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

void CX2GUArme_VioletMage::ChargeMPEndFuture()
{
	CommonStateEndFuture();

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 버프에 있는 엠피 회복 기능을 사용하므로, 해당 부분은 필요 없다.
	m_fAddMPChangeRate = 0.f;
	const float fMPChangeRate = GetOriginalMPChangeRate();
#else  BALANCE_ELEMENTAL_MASTER_20130117
	m_fAddMPChangeRate = 0.f;
	const float fMPChangeRate = ( IsInMeditation() ? GetMeditationMPChangeRate() : GetOriginalMPChangeRate() );
#endif  BALANCE_ELEMENTAL_MASTER_20130117
	ResetMPChangeRate( fMPChangeRate );
}

void CX2GUArme_VioletMage::ChargeMPEnd()
{
	CommonStateEnd();

	CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergy		= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergy );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPEnergyCenter = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPEnergyCenter );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPChargeWave	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPChargeWave );

	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2MagicCircle	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2MagicCircle );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up );
	CKTDGParticleSystem::CParticleEventSequence* pSeqMPCharge2Up2	= g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqMPCharge2Up2 );


	if( pSeqMPEnergy != NULL )
		pSeqMPEnergy->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( pSeqMPEnergyCenter != NULL )
		pSeqMPEnergyCenter->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( pSeqMPChargeWave != NULL )
		pSeqMPChargeWave->SetEmitRate( CMinMax<float>(0.0f,0.0f) );

	if( pSeqMPCharge2MagicCircle != NULL )
		pSeqMPCharge2MagicCircle->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( pSeqMPCharge2Up != NULL )
		pSeqMPCharge2Up->SetEmitRate( CMinMax<float>(0.0f,0.0f) );
	if( pSeqMPCharge2Up2 != NULL )
		pSeqMPCharge2Up2->SetEmitRate( CMinMax<float>(0.0f,0.0f) );


	//m_pSoundChargeWind->Stop();
	g_pKTDXApp->GetDeviceManager()->StopSound( L"wind.ogg" );
	//m_pSoundCharge->Stop();
	g_pKTDXApp->GetDeviceManager()->StopSound( L"Charge.ogg" );

	/*
	if( GetGageManager()->GetMPGage()->fNow == GetGageManager()->GetMPGage()->fMax )
	{
		D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"Center_Jewel", 0 );

		CKTDGParticleSystem::CParticleEventSequence* pSeq;
		pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"ImpactTickBlue", 
			pos, 200, 200, 2, 20 );
		if( pSeq != NULL )
			pSeq->SetLandPosition( m_FrameDataNow.unitCondition.landPosition.y );

		g_pKTDXApp->GetDGManager()->ClearScreen();
		m_pSoundBreak->Play();
		g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( 5.0f, 0.2f );
		g_pX2Game->GetWorld()->SetWorldColor( 0xff333333 );
		g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 2.0f );
	}
	*/

	m_RenderParam.cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
}


////AVSI_DAMAGE_GROGGY
//void CX2GUArme_VioletMage::AVSI_DAMAGE_GROGGY_FrameMoveFuture()
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
//void CX2GUArme_VioletMage::AVSI_DAMAGE_GROGGY_EventProcess()
//{
//	if( m_fGroggyTime <= 0.0f )
//	{
//		StateChange( AVSI_WAIT );
//	}
//	CommonEventProcess();
//}


//AVSI_DAMAGE_SMALL_FRONT
void CX2GUArme_VioletMage::DamageSmallFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}


//AVSI_DAMAGE_SMALL_BACK
void CX2GUArme_VioletMage::DamageSmallBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}


//AVSI_DAMAGE_BIG_FRONT
void CX2GUArme_VioletMage::DamageBigFrontStart()
{
	CommonStateStart();
	
	CreateStepDust();

}

void CX2GUArme_VioletMage::DamageBigFrontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}


//AVSI_DAMAGE_BIG_BACK
void CX2GUArme_VioletMage::DamageBigBackStart()
{
	CommonStateStart();

	CreateStepDust();
}

void CX2GUArme_VioletMage::DamageBigBackEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_SMALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}	
	CommonEventProcess();
}


//AVSI_DAMAGE_DOWN_FRONT
void CX2GUArme_VioletMage::DamageDownFrontFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.19f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.19f ) == true && EventCheck( 0.19f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//m_pSoundDown->Set3DPosition( m_FrameDataNow.syncData.position );

		CreateStepDust();
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::DamageDownFrontEventProcess()
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
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				StateChange( AVSI_TELEPORT_STANDUP_ATTACK ); 
#else //UPGRADE_SKILL_SYSTEM_2013
				StateChange( AVSI_EVASION_STANDUP_ATTACK ); 
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
			else 
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				StateChange( AVSI_TELEPORT_STANDUP_ATTACK ); 
#else //UPGRADE_SKILL_SYSTEM_2013
				StateChange( AVSI_EVASION_STANDUP_ATTACK ); 
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STANDUP_ATTACK ); 
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STANDUP_ATTACK ); 
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
		{
			m_FrameDataFuture.syncData.bIsRight = true;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STAND_UP_START );
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STAND_UP_START );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
		{
			m_FrameDataFuture.syncData.bIsRight = false;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STAND_UP_START );
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STAND_UP_START );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
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
		else if( m_InputData.pureRight == true )
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

	CommonEventProcess();
}


//AVSI_DAMAGE_DOWN_BACK
void CX2GUArme_VioletMage::DamageDownBackFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.32f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.32f ) == true && EventCheck( 0.32f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//m_pSoundDown->Set3DPosition( m_FrameDataFuture.syncData.position );

		CreateStepDust();
	}

	CommonFrameMove();
}


void CX2GUArme_VioletMage::DamageDownBackEventProcess()
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
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				StateChange( AVSI_TELEPORT_STANDUP_ATTACK ); 
#else //UPGRADE_SKILL_SYSTEM_2013
				StateChange( AVSI_EVASION_STANDUP_ATTACK ); 
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
			else
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				StateChange( AVSI_TELEPORT_STANDUP_ATTACK ); 
#else //UPGRADE_SKILL_SYSTEM_2013
				StateChange( AVSI_EVASION_STANDUP_ATTACK ); 
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STANDUP_ATTACK ); 
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STANDUP_ATTACK ); 
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
		{
			m_FrameDataFuture.syncData.bIsRight = true;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STAND_UP_START );
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STAND_UP_START );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
		{
			m_FrameDataFuture.syncData.bIsRight = false;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STAND_UP_START );
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STAND_UP_START );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
			StateChange( USI_DAMAGE_STANDUP_BACK );
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
		else if( m_InputData.pureRight == true )
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

	CommonEventProcess();
}

//AVSI_DAMAGE_AIR_DOWN_LANDING
void CX2GUArme_VioletMage::DamageAirDownLandingInit()
{
	TextureReadyInBackground( L"stone.dds" );
	XSkinMeshReadyInBackground( L"Effect_ElSword_dropdown.X" );
}

void CX2GUArme_VioletMage::DamageAirDownLandingEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_DAMAGE_AIR_FALL );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STANDUP_ATTACK ); 
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STANDUP_ATTACK ); 
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[2] == true )
		{
			m_FrameDataFuture.syncData.bIsRight = true;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STAND_UP_START );
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STAND_UP_START );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[3] == true )
		{
			m_FrameDataFuture.syncData.bIsRight = false;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			StateChange( AVSI_TELEPORT_STAND_UP_START );
#else //UPGRADE_SKILL_SYSTEM_2013
			StateChange( AVSI_EVASION_STAND_UP_START );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
			StateChange( USI_DAMAGE_STANDUP_FRONT );
	}
	else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.66f )
	{
		if( m_InputData.oneZ == true || m_InputData.oneX == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= true;
		}
		else if( m_InputData.pureRight == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= true;
		}
		else if( m_InputData.pureLeft == true )
		{	
			m_FrameDataFuture.stateParam.bEventFlagList[1]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[2]	= false;
			m_FrameDataFuture.stateParam.bEventFlagList[3]	= true;
		}
	}

	CommonEventProcess();
}

//AVSI_EVASION_START
#ifdef SERV_SKILL_NOTE
void CX2GUArme_VioletMage::AVSI_A_AV_Teleport_StateStart()
{
	CommonStateStart();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO5 ) == true )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013
		D3DXVECTOR2 superArmorTime( 0.0f, 0.5f);
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		AccessVecNowSuperArmorTime().push_back(superArmorTime);
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_FrameDataNow.stateParam.m_vecSuperArmorTime.push_back(superArmorTime);
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
#else // UPGRADE_SKILL_SYSTEM_2013
		if( GetForceInvincible() < 0.5f )
			SetForceInvincible( 0.5f );
		if( GetShowInvincible() < 0.5f )
			SetShowInvincible( 0.5f );
#endif // UPGRADE_SKILL_SYSTEM_2013
	}

#ifdef SERV_ARME_DIMENSION_WITCH
	AdaptAdvancedTeleportationBuff();
#endif

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
	const int iSkillLevel 
		= GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_A_AV_TELEPORT, true );

	if ( iSkillLevel > 0 )
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet 
			= GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_A_AV_TELEPORT );

		if ( NULL != pSkillTemplet )
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	}
#endif //FIX_SKILL_BALANCE_AISHA_LENA
}
#endif

void CX2GUArme_VioletMage::AVSI_A_AV_Teleport_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		//{{ dmlee 2009/08/10 석화, 블리자드 샤워 맞았을 때 하늘높이 올라가버리는 오류를 막기위해 
		if( m_ExtraDamagePack.m_EnchantShock.m_fTime <= 0.f &&
			m_ExtraDamagePack.m_Frozen.m_fTime <= 0.f )
		//}} dmlee 2009/08/10 석화, 블리자드 샤워 맞았을 때 하늘높이 올라가버리는 오류를 막기위해 
		{
			m_FrameDataFuture.stateParam.bCanPushUnit = false;
			m_FrameDataFuture.stateParam.bCanPassUnit = true;

			float fMoveTime;
			LUA_GET_VALUE( m_LuaManager, "MOVE_TIME", fMoveTime, 0.0f );

#ifdef SERV_ARME_DIMENSION_WITCH			
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if( m_iAdvancedTeleportationLv > 0 && m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
#else //UPGRADE_SKILL_SYSTEM_2013
			if( m_iAdvancedTeleportationLv >= 3 && m_FrameDataFuture.stateParam.bEventFlagList[1] == true )
#endif //UPGRADE_SKILL_SYSTEM_2013
			{
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
				m_PhysicParam.nowSpeed.y = -1.f *  GetJumpSpeed();				
			}
			else
			{
				m_FrameDataFuture.syncData.position.y += LINE_RADIUS * 1.5f;
				m_PhysicParam.nowSpeed.y =  GetJumpSpeed();
			}
			float y = m_FrameDataFuture.syncData.position.y;
#else
			m_FrameDataFuture.syncData.position.y += LINE_RADIUS * 1.5f;
			float y = m_FrameDataFuture.syncData.position.y;
			m_PhysicParam.nowSpeed.y = GetJumpSpeed();
#endif

			for( int i = 0; i < (int)(fMoveTime * 85.0f); i++ )
			{
				m_PhysicParam.nowSpeed.x = 700.f;
				CX2GUUser::PhysicProcess();

				if( true == IsOnSomethingFuture() )
					m_FrameDataFuture.syncData.position.y = m_FrameDataFuture.unitCondition.landPosition.y + LINE_RADIUS * 1.5f;
			}

#ifdef SERV_ARME_DIMENSION_WITCH
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if( m_iAdvancedTeleportationLv < 1 || m_FrameDataFuture.stateParam.bEventFlagList[1] == false )
#else //UPGRADE_SKILL_SYSTEM_2013
			if( m_iAdvancedTeleportationLv < 3 || m_FrameDataFuture.stateParam.bEventFlagList[1] == false )
#endif //UPGRADE_SKILL_SYSTEM_2013
			{
				if( m_FrameDataFuture.syncData.position.y <= y )
					m_FrameDataFuture.syncData.position.y = y + 100.0f;
			}
#else
			if( m_FrameDataFuture.syncData.position.y <= y )
				m_FrameDataFuture.syncData.position.y = y + 100.0f;
#endif
			m_PhysicParam.nowSpeed.x = 0.0f;

			m_FrameDataFuture.stateParam.bCanPushUnit = true;
			m_FrameDataFuture.stateParam.bCanPassUnit = false;

			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_A_AV_Teleport_EventProcess()
{
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		StateChange( AVSI_TELEPORT_END );
#else //UPGRADE_SKILL_SYSTEM_2013
		StateChange( AVSI_EVASION_END );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
#ifdef SERV_ARME_DIMENSION_WITCH
	else if( m_InputData.pureDown == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}
#endif
	CommonEventProcess();
}



//AVSI_EVASION_END
void CX2GUArme_VioletMage::EvasionEndFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
		pos.y -= 200.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ArmeEvasion", 
			pos, 
			m_FrameDataNow.unitCondition.dirDegree,
			m_FrameDataNow.unitCondition.dirDegree,
			XL_EFFECT_0 );

	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::EvasionEndEventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.4f )
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


//AVSI_EVASION_STAND_UP_START
void CX2GUArme_VioletMage::AVSI_EVASION_STAND_UP_START_Start()
{
	CommonStateStart();
#ifdef SERV_SKILL_NOTE

#ifdef UPGRADE_SKILL_SYSTEM_2013
	D3DXVECTOR2 superArmorTime( 0.0f, 0.6f + 0.5f);
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	AccessVecNowSuperArmorTime().push_back(superArmorTime);
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_FrameDataNow.stateParam.m_vecSuperArmorTime.push_back(superArmorTime);
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
#else // UPGRADE_SKILL_SYSTEM_2013
	if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO5 ) == true )
	{
		SetForceInvincible( 0.6f + 0.5f );
		SetShowInvincible( 0.6f + 0.5f );
	}
	else
	{
		SetForceInvincible( 0.6f );
		SetShowInvincible( 0.6f );
	}
#endif // UPGRADE_SKILL_SYSTEM_2013
	
#else
	SetForceInvincible( 0.6f );
	SetShowInvincible( 0.6f );
#endif

#ifdef SERV_ARME_DIMENSION_WITCH
	AdaptAdvancedTeleportationBuff();
#endif
}

void CX2GUArme_VioletMage::AVSI_EVASION_STAND_UP_START_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{

		//{{ dmlee 2009/08/10 석화, 블리자드 샤워 맞았을 때 하늘높이 올라가버리는 오류를 막기위해 
		if( m_ExtraDamagePack.m_EnchantShock.m_fTime <= 0.f &&
			m_ExtraDamagePack.m_Frozen.m_fTime <= 0.f )
		//}} dmlee 2009/08/10 석화, 블리자드 샤워 맞았을 때 하늘높이 올라가버리는 오류를 막기위해 
		{
			m_FrameDataFuture.stateParam.bCanPushUnit = false;
			m_FrameDataFuture.stateParam.bCanPassUnit = true;

			float fMoveTime;
			LUA_GET_VALUE( m_LuaManager, "MOVE_TIME", fMoveTime, 0.0f );


			m_FrameDataFuture.syncData.position.y += LINE_RADIUS * 1.5f;
			float y = m_FrameDataFuture.syncData.position.y;
			m_PhysicParam.nowSpeed.y = GetJumpSpeed();

			for( int i = 0; i < (int)(fMoveTime * 85.0f); i++ )
			{
				m_PhysicParam.nowSpeed.x = 700.f;
				CX2GUUser::PhysicProcess();

				if( true == IsOnSomethingFuture() )
					m_FrameDataFuture.syncData.position.y = m_FrameDataFuture.unitCondition.landPosition.y + LINE_RADIUS * 1.5f;
			}

			if( m_FrameDataFuture.syncData.position.y <= y )
				m_FrameDataFuture.syncData.position.y = y + 100.0f;
			m_PhysicParam.nowSpeed.x = 0.0f;

			m_FrameDataFuture.stateParam.bCanPushUnit = true;
			m_FrameDataFuture.stateParam.bCanPassUnit = false;

			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;

		}
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_EVASION_STAND_UP_START_EventProcess()
{
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		StateChange( AVSI_TELEPORT_END );
#else //UPGRADE_SKILL_SYSTEM_2013
		StateChange( AVSI_EVASION_END );
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
	CommonEventProcess();
}



//AVSI_EVASION_STAND_UP_END
void CX2GUArme_VioletMage::AVSI_EVASION_STAND_UP_END_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
		pos.y -= 200.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ArmeEvasion", 
			pos, 
			m_FrameDataNow.unitCondition.dirDegree,
			m_FrameDataNow.unitCondition.dirDegree,
			XL_EFFECT_0 );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_EVASION_STAND_UP_END_EventProcess()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.4f )
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


//void CX2GUArme_VioletMage::AVSI_EVASION_STAND_UP_END_End()
//{
//	CommonStateEnd();
//}

//AVSI_EVASION_STANDUP_ATTACK
void CX2GUArme_VioletMage::EvasionStandupAttackInit()
{
	CKTDXDeviceXSkinMesh* pEvasionAttackMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Arme_EvasionAttack.X" );
	CKTDXDeviceXSkinMesh* pEvasionMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Arme_Evasion.X" );
	DeviceReady( pEvasionAttackMesh );
	DeviceReady( pEvasionMesh );

	m_pEvasionAttackAnim = CKTDGXSkinAnim::CreateSkinAnim();
	m_pEvasionAttackAnim->SetAnimXSkinMesh( pEvasionAttackMesh );
	m_pEvasionAttackAnim->AddModelXSkinMesh( pEvasionAttackMesh );
}

void CX2GUArme_VioletMage::EvasionStandupAttackStart()
{
	CommonStateStart();

	m_DamageData.attackerType	= CX2DamageManager::AT_MESH;
	m_DamageData.pAttackerAnim	= m_pEvasionAttackAnim;
}

void CX2GUArme_VioletMage::EvasionStandupAttackFrameMove()
{
	if( m_pEvasionAttackAnim->GetState() == CKTDGXSkinAnim::XAS_PLAYING )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataFuture.unitCondition.bHit = true;
			m_FrameDataNow.unitCondition.bHit = true;
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
		pos.y -= 200.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ArmeEvasion", 
														pos, 
														m_FrameDataNow.unitCondition.dirDegree,
														m_FrameDataNow.unitCondition.dirDegree,
														XL_EFFECT_0 );
	

		m_pEvasionAttackAnim->GetMatrix().Move( pos );
		m_pEvasionAttackAnim->GetMatrix().RotateDegree( m_FrameDataNow.unitCondition.dirDegree );
		m_pEvasionAttackAnim->ChangeAnim( L"EvasionAttack", false );
		m_pEvasionAttackAnim->SetPlaySpeed( 2.0f );
		m_pEvasionAttackAnim->Play( CKTDGXSkinAnim::XAP_ONE );
	}
	

	m_pEvasionAttackAnim->OnFrameMove( m_fTime, m_fElapsedTime );
	CommonFrameMove();
}

void CX2GUArme_VioletMage::EvasionStandupAttackEventProcess()
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

void CX2GUArme_VioletMage::EvasionStandupAttackEnd()
{
	CommonStateEnd();

	SetForceInvincible( 0.3f );
	SetShowInvincible( 0.3f );
}


//AVSI_COMBO_Z1
void CX2GUArme_VioletMage::ComboZ1FrameMove()
{

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboZ1EventProcess()
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
	else if( true == SpecialAttackEventProcess() )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.27f )
	{
		StateChange( AVSI_COMBO_Z2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.27f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.5f )
		{
			m_fCanNotAttackTime = 0.2f;
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		m_fCanNotAttackTime = 0.1f;
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_COMBO_Z2
//void CX2GUArme_VioletMage::ComboZ2FrameMove()
//{
//
//	CommonFrameMove();
//}

void CX2GUArme_VioletMage::ComboZ2EventProcess()
{
	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )		/// 스킬 캔슬 구간 추가
	ELSE_IF_STATE_CHANGE_ON_( 1, 0.43f, 0.43f, m_InputData.oneX == true && 
	( GetUnit()->GetClass() == CX2Unit::UC_ARME_HIGH_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_ELEMENTAL_MASTER ), AVSI_COMBO_X3 )	/// 파이어볼 연계기 추가
#endif BALANCE_ELEMENTAL_MASTER_20130117

	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.47f )
	{

		if( CX2Unit::UC_ARME_VOID_PRINCESS == GetUnitClass() &&
			( ( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false ) ||
			( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true ) ) )
		{
			StateChange( AVSI_AVP_ZZfrontZ );
		}
		else if( m_InputData.pureUp == true )
			StateChange( AVSI_COMBO_Z3up );
		else
			StateChange( AVSI_COMBO_Z3 );

		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.47f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.6f )
		{
			m_fCanNotAttackTime = 0.2f;
			StateChange( USI_WAIT );
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		m_fCanNotAttackTime = 0.2f;
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}




void CX2GUArme_VioletMage::ComboZ3EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}
	else if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.65f )
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_DOWN );
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}



void CX2GUArme_VioletMage::ComboZ3upEventProcess()
{
	if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}

	
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.4f )
	{
		StateChange( AVSI_COMBO_Z4up );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.4f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.7f )
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

	CommonEventProcess();
}



//AVSI_COMBO_Z4up
void CX2GUArme_VioletMage::ComboZ4upStart()
{
	CommonStateStart();

	m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
	m_DamageData.bCanRevenge	= false;

	if( FlushMp( 10.0f ) == true )
		m_bZ4up = true;
	else
		m_bZ4up = false;
}

void CX2GUArme_VioletMage::ComboZ4upFrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.2f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.8f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::ComboZ4upFrameMove()
{
	if( m_bZ4up == true )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.2f ) && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{

			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_RING ) );
			if( NULL != pSeq )
			{
				PlaySequenceByTriggerTime( pSeq, pos, 5, 10, 0.4f );
				pSeq->SetAddRotate( D3DXVECTOR3( 0.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
			}

			if( GetIsRight() == true )
				pos += 50.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 50.0f * m_FrameDataNow.unitCondition.dirVector;

			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_CENTER ) );
			PlaySequenceByTriggerTime( pSeq, pos, 5, 10, 0.4f );
		}

		if( m_pXSkinAnim->GetNowAnimationTime() > 0.2f && m_pXSkinAnim->GetNowAnimationTime() < 0.7f )
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

		if( EventTimer( 0.71f, false ) == true )
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
			m_DamageData.backSpeed.x	= GetDashSpeed() * 2.0f;
			m_DamageData.backSpeed.y	= GetJumpSpeed() * 0.3f;
			m_DamageData.bReAttack		= false;
			m_DamageData.hitUnitList.resize(0);


			if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
			{
				m_FrameDataNow.unitCondition.bHit = true;
			}
			m_bZ4up = false;
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

		CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboZ4upEventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.4f )
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




//AVSI_COMBO_X1
void CX2GUArme_VioletMage::ComboX1FrameMove()
{

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
			if( (GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH) && 
				IsTransformed() )
#else
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN && IsTransformed() )
#endif //SERV_ARME_DIMENSION_WITCH
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MAGICBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
			}
			else
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if ( m_bIceBall )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Short", GetPowerRate() * m_fIceBallPower, posR, 
					GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small", GetPowerRate() * m_fIceBallPower, posR, 
						GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				else
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(),
						GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#else //UPGRADE_SKILL_SYSTEM_2013
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif
		}
		else
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
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboX1EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( true == SpecialAttackEventProcess() )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
#ifdef AISHA_SECOND_CLASS_CHANGE
	ELSE_IF_STATE_CHANGE_ON_( 1, 0.43f, 0.43f, GetUnitClass() == CX2Unit::UC_ARME_ELEMENTAL_MASTER && m_InputData.oneZ == true, AVSI_AEM_XZ )
#endif AISHA_SECOND_CLASS_CHANGE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.43f )
	{
		StateChange( AVSI_COMBO_X2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.43f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#ifdef BALANCE_PATCH_20120329
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.65f )
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
#ifndef SKILL_BALANCE_20110728
				m_fCanNotAttackTime = 0.3f;
#endif
				StateChange( USI_WAIT );
			}
		}
	}
#endif
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifndef SKILL_BALANCE_20110728
		m_fCanNotAttackTime = 0.2f;
#endif
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_COMBO_X2
void CX2GUArme_VioletMage::ComboX2FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
			if( (GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH) && 
				IsTransformed() )
#else
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN && IsTransformed() )
#endif //SERV_ARME_DIMENSION_WITCH
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MAGICBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
			}
			else
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if ( m_bIceBall )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Short", GetPowerRate() * m_fIceBallPower, posR,
					GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small", GetPowerRate() * m_fIceBallPower, posR,
					GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				else
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(),
						GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#else //UPGRADE_SKILL_SYSTEM_2013
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
#else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboX2EventProcess()
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
	else if( true == SpecialAttackEventProcess() )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
	{
		if( m_InputData.pureDown == true )
			StateChange( AVSI_COMBO_X3down );
		else if( m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureRight == true )
			StateChange( AVSI_COMBO_X3front );
		else if( m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureLeft == true )
			StateChange( AVSI_COMBO_X3front );
		else
		{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
			m_bCanChangeState = true;		/// 파이어볼 차지 스테이트 사용 가능
#endif BALANCE_ELEMENTAL_MASTER_20130117

			StateChange( AVSI_COMBO_X3 );
		}
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.9f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#ifdef BALANCE_PATCH_20120329
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
#ifdef SKILL_BALANCE_20110728
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.966f )
#else
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.0f )
#endif
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
#ifndef SKILL_BALANCE_20110728
					m_fCanNotAttackTime = 0.2f;
#endif
				StateChange( USI_WAIT );
			}
		}
	}
#endif
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifndef SKILL_BALANCE_20110728
		m_fCanNotAttackTime = 0.2f;
#endif
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}



//AVSI_COMBO_X3
void CX2GUArme_VioletMage::ComboX3FrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 6.0f  * m_fMPConsumeRateFireBall ) == true )
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.27f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.27f ) == true && EventCheck( 0.27f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 10.0f  * m_fMPConsumeRateFireBall ) == true )
#endif
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			bool bBattleMagician = false;
#ifdef SERV_ARME_DIMENSION_WITCH
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH )
				bBattleMagician = true;
#else
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN )
				bBattleMagician = true;
#endif //SERV_ARME_DIMENSION_WITCH
#ifdef BALANCE_PATCH_20120329
			if( bBattleMagician && IsTransformed() )
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MAGICBALL_BIG2", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			}
			else
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if ( m_bIceBall )
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Big2", GetPowerRate() * m_fIceBallPower, posR, GetRotateDegree(),
						GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
				else
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG2", GetPowerRate(), posR, GetRotateDegree(),
						GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG2", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
				
			}
#else
			if( bBattleMagician && IsTransformed() )
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MAGICBALL_BIG", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			}
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			}
#endif
#else			
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboX3EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef BALANCE_PATCH_20120329
	
#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 4번째 파이어볼 공격 전 차지 스테이트 변경
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.735f )
	{
		if( true == m_bCanChangeState && false == IsTransformed() )		/// 차지 스테이트로 변경이 가능하고, 변신중이 아닐 때
			StateChange( AVSI_COMBO_X4_CHARGE );
		else
			StateChange( AVSI_COMBO_X4 );

		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.4f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#else  BALANCE_ELEMENTAL_MASTER_20130117
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.735f )
	{
		StateChange( AVSI_COMBO_X4 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.4f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#endif BALANCE_ELEMENTAL_MASTER_20130117

	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
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
#endif
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 차지 스테이트로 갈 수 있는지에 대한 변수 초기화를 위해 추가
void CX2GUArme_VioletMage::ComboX3StateEnd()
{
	m_bCanChangeState = false;

	CommonStateEnd();
}
#endif BALANCE_ELEMENTAL_MASTER_20130117

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 4번째 파이어볼 공격에 차지 스테이트 추가
//AVSI_COMBO_X4_CHARGE
void CX2GUArme_VioletMage::ComboX4ChargeStateStart()
{
	CommonStateStart();

	m_fPushXChargeTime = 0.f;
	SetNoDetonation( true );		/// 기폭 제한 설정
}

void CX2GUArme_VioletMage::ComboX4ChargeFrameMove()
{
	CommonFrameMove();

	if ( _CONST_AISHA_::AEM_COMBOX4_MAX_CHARGING_TIME >= m_fPushXChargeTime )
		m_fPushXChargeTime += m_fElapsedTime;

	if ( _CONST_AISHA_::AEM_COMBOX4_MAX_CHARGING_TIME <= m_fPushXChargeTime )
		m_fPushXChargeTime = _CONST_AISHA_::AEM_COMBOX4_MAX_CHARGING_TIME;
}

void CX2GUArme_VioletMage::ComboX4ChargeEventProcess()
{
	if ( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	/// X를 누르고 있지 않거나, 최대 차지 시간을 넘으면 발사 스테이트로 이동
	else if ( false == m_InputData.pureX || _CONST_AISHA_::AEM_COMBOX4_MAX_CHARGING_TIME <= m_FrameDataNow.unitCondition.fStateTime )
	{
		StateChange( AVSI_COMBO_X4_CHARGE_FIRE );
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::ComboX4ChargeStateEnd()
{
	CommonStateEnd();

	SetNoDetonation( false );		/// 기폭 제한 해제
}

//AVSI_COMBO_X4_CHARGE_FIRE
void CX2GUArme_VioletMage::ComboX4ChargeFireFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.166f ) == true && EventCheck( 0.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 6.0f  * m_fMPConsumeRateFireBall ) == true )
		{	
			/// 차지된 시간을 파워 레이트에 연산
			const float fComboX4PowerRate = GetPowerRate() *  ( min( m_fPushXChargeTime / 2.f, _CONST_AISHA_::AEM_COMBOX4_MAX_CHARGING_TIME ) + 1.f );
			/// 차지된 시간에 따른 스케일 연산
			const float fChargeFireBallScale = ( m_fPushXChargeTime / 2.f ) + 1.f;

			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			/// 데미지 이펙트 생성
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			CX2DamageEffect::CEffect* pChargeFireBall = NULL;

			if ( m_bIceBall )
				pChargeFireBall = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Big1",
					fComboX4PowerRate * m_fIceBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			else if ( m_bResearchGravityBall )
				pChargeFireBall = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_GRAVITY_BALL_BIG", 
					fComboX4PowerRate + m_fGravityBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			else
				pChargeFireBall = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG", fComboX4PowerRate, posR, GetRotateDegree(), GetRotateDegree(), 
				m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2DamageEffect::CEffect* pChargeFireBall = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG", fComboX4PowerRate, posR, GetRotateDegree(), GetRotateDegree(), 
				m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
			/// 스케일 조정
			if( NULL != pChargeFireBall )
			{
				pChargeFireBall->SetScale( fChargeFireBallScale, fChargeFireBallScale, fChargeFireBallScale );
			}

			m_fPushXChargeTime = 0.f;	/// 차지 시간 초기화
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboX4ChargeFireEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

#endif BALANCE_ELEMENTAL_MASTER_20130117

#ifdef BALANCE_PATCH_20120329
//AVSI_COMBO_X4
void CX2GUArme_VioletMage::ComboX4FrameMove()
{
#ifdef BALANCE_PATCH_20120329
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.166f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.166f ) == true && EventCheck( 0.166f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 6.0f  * m_fMPConsumeRateFireBall ) == true )
#else
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.169f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.169f ) == true && EventCheck( 0.169f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 10.0f  * m_fMPConsumeRateFireBall ) == true )
#endif
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
			if( (GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH) && 
				IsTransformed() )
#else
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN && IsTransformed() )
#endif //SERV_ARME_DIMENSION_WITCH
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MAGICBALL_BIG", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			}
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			else if ( m_bIceBall )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Big1", GetPowerRate() * m_fIceBallPower, posR,
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif //UPGRADE_SKILL_SYSTEM_2013
			else
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			}
#else			
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboX4EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	WALK_CANCEL_AFTER( m_fWalkCancelAfter )
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#endif	BALANCE_PATCH_20120329

//AVSI_COMBO_X3front
void CX2GUArme_VioletMage::ComboX3frontStart()
{
	CommonStateStart();
	m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
	m_DamageData.bCanRevenge	= false;

	if( FlushMp( 10.0f ) == true )
		m_bZ4up = true;
	else
		m_bZ4up = false;
}

void CX2GUArme_VioletMage::ComboX3frontFrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.46f && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.95f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::ComboX3frontFrameMove()
{
	if( m_bZ4up == true )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.47f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.47f ) && EventCheck( 0.47f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{

			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_RING ) );
			if( NULL != pSeq )
			{
				PlaySequenceByTriggerTime( pSeq, pos, 5, 10, 0.4f );
				pSeq->SetAddRotate( D3DXVECTOR3( 0.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
			}

			if( GetIsRight() == true )
				pos += 20.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 20.0f * m_FrameDataNow.unitCondition.dirVector;

			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_CENTER ) );
			PlaySequenceByTriggerTime( pSeq, pos, 5, 10, 0.4f );
		}

		if( m_pXSkinAnim->GetNowAnimationTime() > 0.47f && m_pXSkinAnim->GetNowAnimationTime() < 0.82f )
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

		if( EventTimer( 0.83f, false ) == true )
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
			m_bZ4up = false;
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboX3frontEventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.4f )
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
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

//AVSI_COMBO_X3down
void CX2GUArme_VioletMage::ComboX3downFrameMove()
{
	if( m_pComboX3Up1->GetState() == CKTDGXSkinAnim::XAS_PLAYING )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 10.0f ) == true )
		{
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
			if( (GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH) && 
				IsTransformed() )
#else
			if(  GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN && IsTransformed() )
#endif //SERV_ARME_DIMENSION_WITCH
			{
				m_pComboX3Up1->SetScale( D3DXVECTOR3(2.f, 1.f, 2.f) );
				m_pComboX3Up2->SetScale( D3DXVECTOR3(2.f, 1.f, 2.f) );
				m_pComboX3Up3->SetScale( D3DXVECTOR3(2.f, 1.f, 2.f) );
			}
			else
			{
				m_pComboX3Up1->SetScale( D3DXVECTOR3(1.f, 1.f, 1.f) );
				m_pComboX3Up2->SetScale( D3DXVECTOR3(1.f, 1.f, 1.f) );
				m_pComboX3Up3->SetScale( D3DXVECTOR3(1.f, 1.f, 1.f) );
			}
#endif
			m_pComboX3Up1->GetRenderParam()->color = 0xffffffff;
			m_pComboX3Up2->GetRenderParam()->color = 0xffffffff;
			m_pComboX3Up3->GetRenderParam()->color = 0xffffffff;

			D3DXVECTOR3 pos = GetPos();
			pos.y += 5.0f;
			m_pComboX3Up1->GetMatrix().Move( pos );
			m_pComboX3Up1->GetMatrix().RotateDegree( GetRotateDegree() );
			m_pComboX3Up2->GetMatrix().Move( pos );
			m_pComboX3Up2->GetMatrix().RotateDegree( GetRotateDegree() );
			m_pComboX3Up3->GetMatrix().Move( pos );
			m_pComboX3Up3->GetMatrix().RotateDegree( GetRotateDegree() );

			m_pComboX3Up1->ChangeAnim( L"ComboX3Up-1", false );
			m_pComboX3Up2->ChangeAnim( L"ComboX3Up-2", false );
			m_pComboX3Up3->ChangeAnim( L"ComboX3Up-3", false );

			m_pComboX3Up1->Play( CKTDGXSkinAnim::XAP_ONE );
			m_pComboX3Up2->Play( CKTDGXSkinAnim::XAP_ONE );
			m_pComboX3Up3->Play( CKTDGXSkinAnim::XAP_ONE );
			m_pComboX3Up3->SetPlaySpeed( 0.8f );

			m_DamageData.attackerType	= CX2DamageManager::AT_MESH;
			m_DamageData.bCanRevenge	= false;
			m_DamageData.pAttackerAnim	= m_pComboX3Up1;
		}
		else
		{
			CreateStepDust();
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::ComboX3downEventProcess()
{
#ifdef SERV_ARME_DIMENSION_WITCH
	if( GetUnit()->GetUnitData().m_UnitClass == CX2Unit::UC_ARME_DIMENSION_WITCH )
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.801f )
		{
			StateChange( AVSI_ADW_COMBO_ADD_X );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}
		else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.3f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.801f )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
	}
#endif

	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		//m_fCanNotAttackTime = 0.5f;
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_DARK_MAGICIAN_COMBO_X3down
void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X3down_FrameMove()
{
	if( m_pComboX3Up1->GetState() == CKTDGXSkinAnim::XAS_PLAYING )
	{
		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;
		}
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 10.0f ) == true )
		{
			m_pComboX3Up1->GetRenderParam()->color = 0xffffffff;
			m_pComboX3Up2->GetRenderParam()->color = 0xffffffff;
			m_pComboX3Up3->GetRenderParam()->color = 0xffffffff;

			D3DXVECTOR3 pos = GetPos();
			pos.y += 5.0f;
			m_pComboX3Up1->GetMatrix().Move( pos );
			m_pComboX3Up1->GetMatrix().RotateDegree( GetRotateDegree() );
			m_pComboX3Up2->GetMatrix().Move( pos );
			m_pComboX3Up2->GetMatrix().RotateDegree( GetRotateDegree() );
			m_pComboX3Up3->GetMatrix().Move( pos );
			m_pComboX3Up3->GetMatrix().RotateDegree( GetRotateDegree() );

			m_pComboX3Up1->ChangeAnim( L"ComboX3Up-1", false );
			m_pComboX3Up2->ChangeAnim( L"ComboX3Up-2", false );
			m_pComboX3Up3->ChangeAnim( L"ComboX3Up-3", false );

			m_pComboX3Up1->Play( CKTDGXSkinAnim::XAP_ONE );
			m_pComboX3Up2->Play( CKTDGXSkinAnim::XAP_ONE );
			m_pComboX3Up3->Play( CKTDGXSkinAnim::XAP_ONE );
			m_pComboX3Up3->SetPlaySpeed( 0.8f );

			m_DamageData.attackerType	= CX2DamageManager::AT_MESH;
			m_DamageData.bCanRevenge	= false;
			m_DamageData.pAttackerAnim	= m_pComboX3Up1;
		}
		else
		{
			CreateStepDust();
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X3down_EventProcess()
{
	if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() < 0.85f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.85f && m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
	{
		StateChange( AVSI_COMBO_X4down );
	}
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		//m_fCanNotAttackTime = 0.5f;
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_DARK_MAGICIAN_COMBO_X4down
void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X4down_Start()
{
	CommonStateStart();
	m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
	m_DamageData.bCanRevenge	= false;

	if( FlushMp( 10.0f ) == true )
		m_bZ4up = true;
	else
		m_bZ4up = false;
}

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X4down_FrameMove()
{
	if( m_bZ4up == true )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.44f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.44f ) && EventCheck( 0.44f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{

			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );


			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_RING ) );
			if( NULL != pSeq )
			{
				PlaySequenceByTriggerTime( pSeq, pos, 5, 10, 0.4f );
				pSeq->SetAddRotate( D3DXVECTOR3( 0.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
			}

			if( GetIsRight() == true )
				pos += 20.0f * m_FrameDataNow.unitCondition.dirVector;
			else
				pos -= 20.0f * m_FrameDataNow.unitCondition.dirVector;

			pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_SHOCK_CENTER ) );
			PlaySequenceByTriggerTime( pSeq, pos, 5, 10, 0.4f );
		}

		if( m_pXSkinAnim->GetNowAnimationTime() > 0.44f && m_pXSkinAnim->GetNowAnimationTime() < 0.9f )
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

		if( EventTimer( 0.91f, false ) == true )
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
			m_bZ4up = false;
		}
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		CreateNotEnoughMPEffect( pos, 0.f, 0.f, 30.f );

	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X4down_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.4f )
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
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_HIGH_MAGICIAN_COMBO_X2
void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( m_bIceBall )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Short", GetPowerRate() * m_fIceBallPower, posR,
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small", GetPowerRate() * m_fIceBallPower, posR,
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(),
					GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X2_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( true == SpecialAttackEventProcess() )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
	{
		if( m_InputData.pureDown == true )
			StateChange( AVSI_COMBO_X3down );
		else if( m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureRight == true )
			StateChange( AVSI_COMBO_X3front );
		else if( m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureLeft == true )
			StateChange( AVSI_COMBO_X3front );
		else if( m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureRight == true )
			StateChange( AVSI_COMBO_X3back );
		else if( m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureLeft == true )
			StateChange( AVSI_COMBO_X3back );
		else
		{
			StateChange( AVSI_COMBO_X3 );
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
			m_bCanChangeState = true;		/// 파이어볼 차지 스테이트 사용 가능
#endif BALANCE_ELEMENTAL_MASTER_20130117
		}
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.9f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 번개를 3번까지 내리칠 수 있는 커맨드 추가
	ELSE_IF_STATE_CHANGE_ON_( 1, 0.86f, 0.86f, true == m_InputData.oneZ && GetUnit()->GetClass() == CX2Unit::UC_ARME_ELEMENTAL_MASTER , AVSI_AEM_XZ )
#endif BALANCE_ELEMENTAL_MASTER_20130117

	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
#ifdef SKILL_BALANCE_20110728
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.966f )
#else
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.0f )
#endif
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
#ifndef SKILL_BALANCE_20110728
				m_fCanNotAttackTime = 0.2f;
#endif
				StateChange( USI_WAIT );
			}
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifndef SKILL_BALANCE_20110728
		m_fCanNotAttackTime = 0.2f;
#endif
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_HIGH_MAGICIAN_COMBO_X3back
void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X3back_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.34f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.34f ) == true && EventCheck( 0.34f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 6.0f * m_fMPConsumeRateFireBall ) == true )
#else
		if( FlushMp( 10.0f * m_fMPConsumeRateFireBall ) == true )
#endif
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 rot = GetRotateDegree();
			rot.y += 180.0f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( m_bIceBall )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 데미지 이펙트 이름 변경 ( 중복된 데미지 이펙트 수정 )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ICEBALL_BIG_AISHA_HIGH_MAGICIAN", GetPowerRate() * m_fIceBallPower, posR,
				rot, rot, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );				
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Big1", GetPowerRate() * m_fIceBallPower, posR,
					rot, rot, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG_AISHA_HIGH_MAGICIAN", GetPowerRate(), posR,
					rot, rot, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_BIG_AISHA_HIGH_MAGICIAN", GetPowerRate(), posR, rot, rot, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X3back_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
#ifndef SKILL_BALANCE_20110728
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
#endif
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.4f )
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
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X3back_EndFuture()
{
	CommonStateEndFuture();

	m_FrameDataFuture.syncData.bIsRight = !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;
}


//AVSI_DARK_MAGICIAN_COMBO_X2
void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.53f ) == true && EventCheck( 0.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SHORT", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X2_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
	SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
	else if( true == SpecialAttackEventProcess() )
	{
	}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
	{
		if( m_InputData.pureDown == true )
			StateChange( AVSI_COMBO_X3down );
		else if( m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureRight == true )
			StateChange( AVSI_COMBO_X3front );
		else if( m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureLeft == true )
			StateChange( AVSI_COMBO_X3front );
		else if( m_FrameDataFuture.syncData.bIsRight == false && m_InputData.pureRight == true )
			StateChange( AVSI_COMBO_X3back );
		else if( m_FrameDataFuture.syncData.bIsRight == true && m_InputData.pureLeft == true )
			StateChange( AVSI_COMBO_X3back );
		else
		{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
			m_bCanChangeState = true;		/// 파이어볼 차지 스테이트 사용 가능
#endif BALANCE_ELEMENTAL_MASTER_20130117
			StateChange( AVSI_COMBO_X3 );
		}

		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.9f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
#ifdef SKILL_BALANCE_20110728
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.966f )
#else
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.0f )
#endif
		{
			if( false == IsOnSomethingFuture() )
			{
				StateChange( USI_JUMP_DOWN );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
			{
#ifndef SKILL_BALANCE_20110728
				m_fCanNotAttackTime = 0.2f;
#endif
				StateChange( USI_WAIT );
			}
		}
	}
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
#ifndef SKILL_BALANCE_20110728
		m_fCanNotAttackTime = 0.2f;
#endif	SKILL_BALANCE_20110728
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_DARK_MAGICIAN_COMBO_X3back
void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X3back_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.71f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.71f ) == true && EventCheck( 0.71f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef SKILL_BALANCE_20110728
		if( FlushMp( 10.0f ) == true )
#else	SKILL_BALANCE_20110728
		if( FlushMp( 20.0f ) == true )
#endif	SKILL_BALANCE_20110728
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DARK_SHOCK_BALL", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_COMBO_X3back_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.4f )
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
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_DASH_COMBO_Z1
void CX2GUArme_VioletMage::DashComboZ1StartFuture()
{
	CommonStateStartFuture();

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
	if( (GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH) &&
		IsTransformed() )
#else	SERV_ARME_DIMENSION_WITCH
	if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN && IsTransformed() )
#endif //SERV_ARME_DIMENSION_WITCH
		m_PhysicParam.nowSpeed.x = 1000.f;
#endif	SERV_ADD_ARME_BATTLE_MAGICIAN
}

void CX2GUArme_VioletMage::DashComboZ1FrameMove()
{
	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashComboZ1EventProcess()
{
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.46f
		&& true == IsOnSomethingFuture() )
	{
		StateChange( AVSI_DASH_COMBO_Z2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_FrameDataFuture.stateParam.bEventFlagList[1] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.46f
		&& true == IsOnSomethingFuture() )
	{
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
		if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH )
#else	SERV_ARME_DIMENSION_WITCH
		if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN )
#endif //SERV_ARME_DIMENSION_WITCH
			StateChange( AVSI_ABM_DASH_COMBO_Z2a );

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 엘리멘탈 마스터 대시 콤보 ZX
		else if( GetUnit()->GetClass() == CX2Unit::UC_ARME_ELEMENTAL_MASTER )
			StateChange( AVSI_EM_DASH_COMBO_Z2a );
#endif BALANCE_ELEMENTAL_MASTER_20130117

		else
			StateChange( AVSI_DASH_COMBO_Z2a );
#else	SERV_ADD_ARME_BATTLE_MAGICIAN
		StateChange( AVSI_DASH_COMBO_Z2a );
#endif	SERV_ADD_ARME_BATTLE_MAGICIAN
		
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.46f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.46f )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		m_FrameDataFuture.stateParam.bEventFlagList[1] = true;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_C
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.65f )
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

//AVSI_DASH_COMBO_Z2
void CX2GUArme_VioletMage::DashComboZ2StartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.2f;
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.2f;
	}
}

//void CX2GUArme_VioletMage::DashComboZ2FrameMove()
//{
//
//	CommonFrameMove();
//}

void CX2GUArme_VioletMage::DashComboZ2EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.55f )
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

//AVSI_DASH_COMBO_Z2a
void CX2GUArme_VioletMage::DashComboZ2aStartFuture()
{
	CommonStateStartFuture();

	bool bMove = false;
	D3DXVECTOR3 pos = GetPos();
	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
		if( pUnit->GetTeam() == GetTeam() )
			continue;

		if( pUnit->GetInvincible() == true )
			continue;

		if( pUnit->GetNowHp() <= 0.f )
			continue;
	
		//{{ oasis907 : 김상윤 [2011.3.21] 유저가 밟을 수 있는 라인 체크
		const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( pUnit->GetLastTouchLineIndex() );
		if( NULL == pLineData )
			continue;

		if( pLineData->lineType != CKTDGLineMap::LT_NORMAL || pLineData->bEnable == false )
			continue;
		//}} oasis907 : 김상윤 [2011.3.21]

		if( GetDistance( pos, pUnit->GetPos() ) < 700.0f )
		{
#ifdef FIX_AISHA_TELEPORT_POSITON
			if( pUnit->GetPos().y < pLineData->startPos.y )
				continue; 
#endif //FIX_AISHA_TELEPORT_POSITON

			m_FrameDataFuture.syncData.position		= pUnit->GetPos();
			m_FrameDataFuture.syncData.position.y	+= 350.0f;
			bMove = true;
			break;
		}
	}

	if( bMove == false )
		m_FrameDataFuture.syncData.position.y	+= 350.0f;

	m_FrameDataFuture.stateParam.bCanPushUnit = false;
	m_FrameDataFuture.stateParam.bCanPassUnit = true;

	for( int i = 0; i < (int)(0.05f * 85.0f); i++ )
	{
		m_PhysicParam.nowSpeed.x = -700.f;
		CX2GUUser::PhysicProcess();
	}

	m_FrameDataFuture.stateParam.bCanPushUnit = true;
	m_FrameDataFuture.stateParam.bCanPassUnit = false;
}

void CX2GUArme_VioletMage::DashComboZ2aFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
		pos.y -= 200.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ArmeEvasion", 
			pos, 
			m_FrameDataNow.unitCondition.dirDegree,
			m_FrameDataNow.unitCondition.dirDegree,
			XL_EFFECT_0 );

	}
	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashComboZ2aEventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange(USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}

	CommonEventProcess();
}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 엘리멘탈 마스터 대시 콤보 ZX
void CX2GUArme_VioletMage::AVSI_EM_DASH_COMBO_Z2a_StartFuture()
{
	CommonStateStartFuture();

	bool bMove = false;
	D3DXVECTOR3 pos = GetPos();
	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
		if( pUnit->GetTeam() == GetTeam() )
			continue;

		if( pUnit->GetInvincible() == true )
			continue;

		if( pUnit->GetNowHp() <= 0.f )
			continue;

		//{{ oasis907 : 김상윤 [2011.3.21] 유저가 밟을 수 있는 라인 체크
		const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( pUnit->GetLastTouchLineIndex() );
		if( NULL == pLineData )
			continue;

		if( pLineData->lineType != CKTDGLineMap::LT_NORMAL || pLineData->bEnable == false )
			continue;
		//}} oasis907 : 김상윤 [2011.3.21]

		if( GetDistance( pos, pUnit->GetPos() ) < 700.0f )
		{
#ifdef FIX_AISHA_TELEPORT_POSITON
			if( pUnit->GetPos().y < pLineData->startPos.y )
				continue; 
#endif //FIX_AISHA_TELEPORT_POSITON

			m_FrameDataFuture.syncData.position		= pUnit->GetPos();
			m_FrameDataFuture.syncData.position.y	+= 350.0f;
			bMove = true;
			break;
		}
	}

	if( bMove == false )
		m_FrameDataFuture.syncData.position.y	+= 350.0f;

	m_FrameDataFuture.stateParam.bCanPushUnit = false;
	m_FrameDataFuture.stateParam.bCanPassUnit = true;

	for( int i = 0; i < (int)(0.05f * 85.0f); i++ )
	{
		m_PhysicParam.nowSpeed.x = -700.f;
		CX2GUUser::PhysicProcess();
	}

	m_FrameDataFuture.stateParam.bCanPushUnit = true;
	m_FrameDataFuture.stateParam.bCanPassUnit = false;
}

void CX2GUArme_VioletMage::AVSI_EM_DASH_COMBO_Z2a_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
		pos.y -= 200.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ArmeEvasion", 
			pos, 
			m_FrameDataNow.unitCondition.dirDegree,
			m_FrameDataNow.unitCondition.dirDegree,
			XL_EFFECT_0 );

	}
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_EM_DASH_COMBO_Z2a_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		if( false == IsOnSomethingFuture() )
		{
			StateChange(USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( USI_WAIT );
	}
	else if( true == m_FrameDataFuture.stateParam.bEventFlagList[0] && 0.608f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		StateChange( AVSI_COMBO_X3down );
	}
	else if( true == m_InputData.oneX && 0.468f < m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

		CommonEventProcess();
}
#endif BALANCE_ELEMENTAL_MASTER_20130117

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_Start()
{
	CommonStateStart();

#ifdef SERV_ARME_DIMENSION_WITCH
	AdaptAdvancedTeleportationBuff();
#endif	SERV_ARME_DIMENSION_WITCH
}

//AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a
void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_StartFuture()
{
	CommonStateStartFuture();

	bool bMove = false;
	D3DXVECTOR3 pos = GetPos();
	for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
	{
		CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
		if( pUnit->GetTeam() == GetTeam() )
			continue;

		if( pUnit->GetInvincible() == true )
			continue;

		if( pUnit->GetNowHp() <= 0.f )
			continue;

		//{{ oasis907 : 김상윤 [2011.3.21] 유저가 밟을 수 있는 라인 체크
		const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( pUnit->GetLastTouchLineIndex() );
		if( NULL == pLineData )
			continue;

		if( pLineData->lineType != CKTDGLineMap::LT_NORMAL || pLineData->bEnable == false )
			continue;
		//}} oasis907 : 김상윤 [2011.3.21]

		if( GetDistance( pos, pUnit->GetPos() ) < 700.0f )
		{
#ifdef FIX_AISHA_TELEPORT_POSITON
			if( pUnit->GetPos().y < pLineData->startPos.y )
				continue; 
#endif //FIX_AISHA_TELEPORT_POSITON

			m_FrameDataFuture.syncData.position		= pUnit->GetPos();
			m_FrameDataFuture.syncData.position.y	+= 350.0f;
			bMove = true;

			break;
		}
	}

	if( bMove == false )
		m_FrameDataFuture.syncData.position.y	+= 350.0f;

	m_FrameDataFuture.stateParam.bCanPushUnit = false;
	m_FrameDataFuture.stateParam.bCanPassUnit = true;

	for( int i = 0; i < (int)(0.05f * 85.0f); i++ )
	{
		m_PhysicParam.nowSpeed.x = -700.f;
		CX2GUUser::PhysicProcess();
	}

	m_FrameDataFuture.stateParam.bCanPushUnit = true;
	m_FrameDataFuture.stateParam.bCanPassUnit = false;
}

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
		pos.y -= 200.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ArmeEvasion", 
			pos, 
			m_FrameDataNow.unitCondition.dirDegree,
			m_FrameDataNow.unitCondition.dirDegree,
			XL_EFFECT_0 );

	}
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_DASH_COMBO_Z2a_EventProcess()
{
	if( m_InputData.oneX == true )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.47f )
	{
		StateChange( AVSI_DASH_COMBO_Z3up );
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


//AVSI_DARK_MAGICIAN_DASH_COMBO_Z3up
void CX2GUArme_VioletMage::AVSI_DARK_MAGICIAN_DASH_COMBO_Z3up_EventProcess()
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
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.75f )
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


//AVSI_DASH_ATTACK_X
void CX2GUArme_VioletMage::DashAttackXStartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
}

void CX2GUArme_VioletMage::DashAttackXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파볼
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else	BALANCE_PATCH_20120329
		if( FlushMp( 5.0f * m_fMPConsumeRateFireBall ) == true )
#endif	BALANCE_PATCH_20120329
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.y += 180.0f;

			switch( GetUnit()->GetClass() )
			{
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
				{
					if( IsTransformed() )
					{
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MAGICBALL_SMALL", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
					}
					else
					{
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
					}
				}
				break;
#endif	SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
			case CX2Unit::UC_ARME_DIMENSION_WITCH:
				{
					if( IsTransformed() )
					{
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ADW_MAGICBALL_BIG_SLOW", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
					}
					else
					{
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ADW_MAGICBALL_SMALL_SLOW", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
					}
				}
				break;
#endif	SERV_ARME_DIMENSION_WITCH
			default:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					if ( m_bIceBall )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small", GetPowerRate() * m_fIceBallPower, posR, angle, angle,
						m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small_Down", GetPowerRate() * m_fIceBallPower, posR, angle, angle,
						m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					else
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle,
							m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle,
						m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
				}
				break;
			}
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashAttackXEventProcess()
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

void CX2GUArme_VioletMage::DashAttackXEndFuture()
{
	CommonStateEndFuture();

	m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;
}


//AVSI_HIGH_MAGICION_DASH_ATTACK_X
void CX2GUArme_VioletMage::AVSI_HIGH_MAGICION_DASH_ATTACK_X_StartFuture()
{
	CommonStateStartFuture();

	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed();
	}
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICION_DASH_ATTACK_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파볼
		if( FlushMp( 5.0f * m_fMPConsumeRateFireBall ) == true )
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.y += 180.0f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( m_bIceBall )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small", GetPowerRate() * m_fIceBallPower, posR, angle, angle,
				m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small_Down", GetPowerRate() * m_fIceBallPower, posR, angle, angle,
				m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );

#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 180.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICION_DASH_ATTACK_X_EventProcess()
{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 입력 타임 뒤로 밀음
	static float fChangeStateTime = 0.7f;
#else  BALANCE_ELEMENTAL_MASTER_20130117
	static float fChangeStateTime = 0.5f;
#endif BALANCE_ELEMENTAL_MASTER_20130117
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > fChangeStateTime )
	{
		StateChange( AVSI_DASH_ATTACK_X2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= fChangeStateTime )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
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

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICION_DASH_ATTACK_X_EndFuture()
{
	CommonStateEndFuture();

	m_FrameDataFuture.syncData.bIsRight	= !m_FrameDataFuture.syncData.bIsRight;
	m_FrameDataFuture.unitCondition.dirDegree.y += 180.0f;
}


//AVSI_HIGH_MAGICION_DASH_ATTACK_X2
void CX2GUArme_VioletMage::AVSI_HIGH_MAGICION_DASH_ATTACK_X2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.27f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.27f ) == true && EventCheck( 0.27f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 10.0f * m_fMPConsumeRateFireBall ) == true )
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang

			if ( m_bIceBall )
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HIGH_MAGICIAN_DASH_COMBO_XX_ICEBALL", GetPowerRate() * m_fIceBallPower, posR,
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Big2", GetPowerRate() * m_fIceBallPower, posR,
				GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HIGH_MAGICIAN_DASH_COMBO_XX_FIREBALL", GetPowerRate(), posR,
					GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HIGH_MAGICIAN_DASH_COMBO_XX_FIREBALL", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICION_DASH_ATTACK_X2_EventProcess()
{
	if( false == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_DOWN );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
	ELSE_IF_KEY_PRESSED_AT_SKIP_POINT_ZXC
	{
		if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.9f )
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
	else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{	
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}


//AVSI_JUMP_ATTACK_Z
void CX2GUArme_VioletMage::JumpAttackZFrameMoveFuture()
{
	if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
	{
		m_PhysicParam.nowSpeed.x = GetWalkSpeed();
	}
	if( m_InputData.pureDown == true  && m_InputData.pureUp == false )
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}


	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::JumpAttackZFrameMove()
{
	CommonFrameMove();
}

void CX2GUArme_VioletMage::JumpAttackZEventProcess()
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

//AVSI_JUMP_ATTACK_X
void CX2GUArme_VioletMage::JumpAttackXFrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.13f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.26f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}
	else
	{
		if( m_InputData.pureRight == true && m_FrameDataFuture.syncData.bIsRight == true )
		{
			m_PhysicParam.nowSpeed.x = GetWalkSpeed();
		}
		else if( m_InputData.pureLeft == true && m_FrameDataFuture.syncData.bIsRight == false )
		{
			m_PhysicParam.nowSpeed.x = GetWalkSpeed();
		}
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::JumpAttackXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.16f ) == true && EventCheck( 0.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파볼
#ifdef SKILL_BALANCE_20110728
#ifdef BALANCE_PATCH_20120329
		if(  FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else	BALANCE_PATCH_20120329
		if(  FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif	BALANCE_PATCH_20120329
#else	SKILL_BALANCE_20110728
		if(  FlushMp( 5.0f * m_fMPConsumeRateFireBall ) == true )
#endif	SKILL_BALANCE_20110728
		{
			//m_pSoundSlash->Play();
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
			if( (GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH) &&
				IsTransformed() )
#else	SERV_ARME_DIMENSION_WITCH
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN && IsTransformed() )
#endif //SERV_ARME_DIMENSION_WITCH
			{
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"MAGICBALL_SMALL_DOWN", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			}
			else
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if ( m_bIceBall )
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small_Down", GetPowerRate() * m_fIceBallPower, posR,
						angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
				else if ( m_bResearchGravityBall )
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_GRAVITY_BALL", m_fGravityBallPower, posR, angle,
						angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
				else
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN", GetPowerRate(), posR, angle,
					angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}
#else	SERV_ADD_ARME_BATTLE_MAGICIAN
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif	SERV_ADD_ARME_BATTLE_MAGICIAN
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"MagicFail", pos, 100, 100, -1, 1 );
			pSeq->SetAxisAngle( angle );
			pSeq->SetAddRotate( angle );
		}

	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::JumpAttackXEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_JUMP_LANDING );
	}


	CommonEventProcess();
}

//AVSI_DASH_JUMP_ATTACK_Z
void CX2GUArme_VioletMage::DashJumpAttackZFrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.26f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.4f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.41f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.41f ) == true && EventCheck( 0.41f, true) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x -= GetDashSpeed();
		m_PhysicParam.nowSpeed.y = GetJumpSpeed() / 2.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::DashJumpAttackZFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.27f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.27f ) && EventCheck( 0.27f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( FlushMp( 5.0f ) == true )
		{
			PlaySound( L"flame.ogg" );
			PlaySound( L"Sound_Arme_shout2.ogg" );

			D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );


			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAishaMinorParticleByEnum( AISHA_PII_MINOR_AIR_BLAST_RING ) );
			if( NULL != pSeq )
			{
				PlaySequenceByTriggerCount( pSeq, pos, 1000, 1000,  1 );

				if( GetIsRight() == true )
					pSeq->SetAddRotate( D3DXVECTOR3( 30.0f, m_FrameDataNow.unitCondition.dirDegree.y + 70.0f, 0.0f ) );
				else
					pSeq->SetAddRotate( D3DXVECTOR3( -30.0f, m_FrameDataNow.unitCondition.dirDegree.y - 70.0f, 0.0f ) );
			}
			
			
			pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_AIR_BLAST_CENTER ) );
			if( NULL != pSeq )
			{
				PlaySequenceByTriggerTime( pSeq, pos, 10, 20, 0.4f );
			}

			m_bDashJumpZ = true;
			m_bDashJumpZPos = pos;
		}
		else
		{
			D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -30.f );


			m_bDashJumpZ = false;
		}
	}

	if( m_pXSkinAnim->GetNowAnimationTime() > 0.28f && m_pXSkinAnim->GetNowAnimationTime() < 0.7f && m_bDashJumpZ == true )
	{
		m_DamageData.attackerType	= CX2DamageManager::AT_MANUAL_COLLISION_DATA;
		m_DamageData.bCanRevenge	= false;
		CKTDXCollision::CollisionData data;		
		data.m_fRadius = 100.0f;

		D3DXMATRIX mat;
		D3DXMatrixIdentity( &mat );
		mat._41 = m_bDashJumpZPos.x;
		mat._42 = m_bDashJumpZPos.y;
		mat._43 = m_bDashJumpZPos.z;
		data.m_pCombineMatrix = &mat;
		m_DamageData.pCollisionData = &data;

		if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
		{
			m_FrameDataNow.unitCondition.bHit = true;
			m_FrameDataFuture.unitCondition.bHit = m_FrameDataNow.unitCondition.bHit;
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashJumpAttackZEventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}

//AVSI_DASH_JUMP_ATTACK_X
void CX2GUArme_VioletMage::DashJumpAttackXFrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.13f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.27f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::DashJumpAttackXFrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.16f ) == true && EventCheck( 0.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파볼쏘기
#ifdef SKILL_BALANCE_20110728
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else	BALANCE_PATCH_20120329
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif	BALANCE_PATCH_20120329
#else	SKILL_BALANCE_20110728
		if( FlushMp( 5.0f * m_fMPConsumeRateFireBall ) == true )
#endif	SKILL_BALANCE_20110728
		{
			//m_pSoundSlash->Play();
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;
#ifdef BALANCE_PATCH_20120329
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( m_bIceBall )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small", GetPowerRate() * m_fIceBallPower, posR, angle,
					angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle,
					m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
#else	BALANCE_PATCH_20120329
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif	BALANCE_PATCH_20120329

		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -30.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashJumpAttackXEventProcess()
{
#ifdef BALANCE_PATCH_20120329
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.4f )
	{
		StateChange( AVSI_DASH_JUMP_ATTACK_X2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}
#endif	BALANCE_PATCH_20120329

	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}
	//else if( m_pXSkinAnimFuture->IsAnimationEnd() == true && false == IsOnSomethingFuture() )
	//{
	//	m_pXSkinAnimFuture->Stop();
	//	m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	//}

	CommonEventProcess();
}

#ifdef BALANCE_PATCH_20120329
void CX2GUArme_VioletMage::DashJumpAttackX2FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.13f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.27f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::DashJumpAttackX2FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.16f ) == true && EventCheck( 0.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파볼쏘기
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( m_bIceBall )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_IceBall_Small_Down", GetPowerRate() * m_fIceBallPower, posR, angle,
					angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
			else
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN", GetPowerRate(), posR, angle, angle,
					m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -30.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::DashJumpAttackX2EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}
#endif	BALANCE_PATCH_20120329

#ifndef BALANCE_PATCH_20120329
//AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X
void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.13f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.27f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.16f ) == true && EventCheck( 0.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파볼쏘기
#ifdef SKILL_BALANCE_20110728
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else	BALANCE_PATCH_20120329
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif	BALANCE_PATCH_20120329
#else	SKILL_BALANCE_20110728
		if( FlushMp( 5.0f * m_fMPConsumeRateFireBall ) == true )
#endif	SKILL_BALANCE_20110728
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );

		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -30.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X_EventProcess()
{
	if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.4f )
	{
		StateChange( AVSI_DASH_JUMP_ATTACK_X2 );
		m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
	}
	else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() )
	{
		m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
	}

	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}


//AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2
void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.13f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.27f )
	{
		m_PhysicParam.nowSpeed.x = 0.0f;
		m_PhysicParam.nowSpeed.y = 0.0f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.16f ) == true && EventCheck( 0.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파볼쏘기
#ifdef SKILL_BALANCE_20110728
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif	BALANCE_PATCH_20120329
#else
		if( FlushMp( 5.0f * m_fMPConsumeRateFireBall ) == true )
#endif	SKILL_BALANCE_20110728
		{
			//파이어볼쏘기
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			D3DXVECTOR3 angle = GetRotateDegree();
			angle.z = -30.0f;
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_SMALL_DOWN", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );

		}
		else
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			CreateNotEnoughMPEffect( pos, 0.f, 0.f, -30.f );

		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_DASH_JUMP_ATTACK_X2_EventProcess()
{
	if( true == IsOnSomethingFuture() )
	{
		StateChange( USI_DASH_JUMP_LANDING );
	}

	CommonEventProcess();
}
#endif	BALANCE_PATCH_20120329

////////////////////////////////////////////////////////////////////////
//AVSI_SI_AV_POWER_ATTACK
void CX2GUArme_VioletMage::AVSI_SI_AV_POWER_ATTACK_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );
	
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI_AV_POWER_ATTACK_EventProcess()
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


//AVSI_SI_AV_POWER_ATTACK_HYPER
void CX2GUArme_VioletMage::AVSI_SI_AV_POWER_ATTACK_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.06f, 0 );

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI_AV_POWER_ATTACK_HYPER_EventProcess()
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

//AVSI_SI1_AV_BINDING_CIRCLE
void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Bip01_Spine1", 0.001f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
// 		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_Spine1" );
// 
// 		CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
// 
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_AV_BINDING_CIRCLE );
		if ( NULL != pSkillTemplet && NULL != g_pX2Game->GetEffectSet() )
		{
			const int iSkillTempletLevel = userSkillTree.GetSkillLevel( pSkillTemplet->m_eID );	/// 스킬 레벨
			float m_fBindingCircleDistance				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SCOPE_ABS, iSkillTempletLevel );			/// 한발당 소비 엠피
			CX2EffectSet::Handle hBindingCircleEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_BINDING_CIRCLE_LEVEL_01", this );			
			CX2EffectSet::EffectSetInstance * pBindingCircleEffectSetInst = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hBindingCircleEffectSet );
			if ( NULL !=  pBindingCircleEffectSetInst )
			{
				float fScale = m_fBindingCircleDistance / _CONST_AISHA_::BIDING_CIRCLE_BASIC_DISTANCE;
				D3DXVECTOR3 vScale ( fScale, fScale, fScale );
				pBindingCircleEffectSetInst->SetEffectScale( vScale );
			}			
		}
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		switch( m_NowStateData.m_SPLevel )
		{
		case 1:
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_BINDING_CIRCLE_LEVEL_01", this );
			// 			pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_01 ) );
			// 			PlaySequenceByTriggerCount( pSeq, pos, 20, 40, 5 );
			break;

		case 2:
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_BINDING_CIRCLE_LEVEL_02", this );
			// 			pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_02 ) );
			// 
			// 			PlaySequenceByTriggerCount( pSeq, pos, 20, 40, 5 );
			break;

		default:
		case 3:
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_BINDING_CIRCLE_LEVEL_03", this );
			// 			pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_03 ) );
			// 
			// 			PlaySequenceByTriggerCount( pSeq, pos, 20, 40, 5 );
			break;
		}		
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편


	
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();

		int nCount = 0;         		
		for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
		{
			CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
			if( pUnit->GetTeam() == GetTeam() )
				continue;

			if( pUnit->GetInvincible() == true )
				continue;

			float fLength = 0.0f;

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_AV_BINDING_CIRCLE );
			if ( NULL != pSkillTemplet )
			{
				const int iSkillTempletLevel = userSkillTree.GetSkillLevel( pSkillTemplet->m_eID );	/// 스킬 레벨
				fLength				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SCOPE_ABS, iSkillTempletLevel );			/// 한발당 소비 엠피
				fLength *= fLength;
			}
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			switch( m_NowStateData.m_SPLevel )
			{
			case 1:
				fLength = 500.0f;
				break;

			case 2:
				fLength = 600.0f;
				break;

			default:
			case 3:
				fLength = 700.0f;
				break;
			}

#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

			if( GetDistance3Sq( pos, pUnit->GetPos() ) < fLength )
			{
				nCount++;
// 				pUnit->SetStopTime( 2.0f );
// 				D3DXVECTOR3 pos2 = pUnit->GetBonePos( L"Bip01" );
// 				g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"TimeStopRingX", pos2, 100, 100, -1, 1 );
// 				g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"TimeStopRingY", pos2, 100, 100, -1, 1 );
// 				g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"TimeStopRingZ", pos2, 100, 100, -1, 1 );

				pUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_DEBUFF_BIND ) );
#ifdef SERV_SKILL_NOTE
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO2 ) == true )
				{
					pUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_DEBUFF_BIND_WITH_MEMO ) );
					//pUnit->SetAnimSpeedFactor( 0.8f, 10.f, CX2GameUnit::SFI_SPEED_1 );
				}
#endif	SERV_SKILL_NOTE
			}
		}

		if( nCount > 0 )
		{
			PlaySound( L"Aisha_special_attack1.ogg" );
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
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

void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_End()
{
	CommonStateEnd();
}

//AVSI_SI1_AV_BINDING_CIRCLE_HYPER
void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Bip01_Spine1", 0.001f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

#ifdef BALANCE_PATCH_20131107
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_AV_BINDING_CIRCLE );
		if ( NULL != pSkillTemplet && NULL != g_pX2Game->GetEffectSet() )
		{
			const int iSkillTempletLevel = userSkillTree.GetSkillLevel( pSkillTemplet->m_eID );
			float m_fBindingCircleDistance				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SCOPE_ABS, iSkillTempletLevel );
			CX2EffectSet::Handle hBindingCircleEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_BINDING_CIRCLE_HYPER_LEVEL_01", this );			
			CX2EffectSet::EffectSetInstance * pBindingCircleEffectSetInst = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hBindingCircleEffectSet );
			if ( NULL !=  pBindingCircleEffectSetInst )
			{
				float fScale = m_fBindingCircleDistance / _CONST_AISHA_::BIDING_CIRCLE_BASIC_DISTANCE;
				D3DXVECTOR3 vScale ( fScale, fScale, fScale );
				pBindingCircleEffectSetInst->SetEffectScale( vScale );
			}			
		}
#else // BALANCE_PATCH_20131107		
		D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_Spine1" );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( GetHandleAishaMinorParticleByEnum( AISHA_PII_MINOR_TIME_STOP_RED ) );
		PlaySequenceByTriggerCount( pSeq, pos, 20, 40, 5 );
#endif // BALANCE_PATCH_20131107

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();

#ifdef BALANCE_PATCH_20131107
		float fLength = 0.0f;
		const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_A_AV_BINDING_CIRCLE );
		if ( NULL != pSkillTemplet )
		{
			const int iSkillTempletLevel = userSkillTree.GetSkillLevel( pSkillTemplet->m_eID );	/// 스킬 레벨
			fLength				= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SCOPE_ABS, iSkillTempletLevel );			/// 한발당 소비 엠피
			fLength *= fLength;
		}
#endif // BALANCE_PATCH_20131107		

		int nCount = 0;   		
		for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
		{
			CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
			if( pUnit->GetTeam() == GetTeam() )
				continue;

			if( pUnit->GetInvincible() == true )
				continue;

#ifdef BALANCE_PATCH_20131107
			if( GetDistance3Sq( pos, pUnit->GetPos() ) < fLength )
#else // BALANCE_PATCH_20131107		
			if( GetDistance( pos, pUnit->GetPos() ) < 700.0f )
#endif // BALANCE_PATCH_20131107		
			{
				nCount++;
				pUnit->SetStopTime( 3.0f );
				D3DXVECTOR3 pos2 = pUnit->GetBonePos( L"Bip01" );
				g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"TimeStopRingXRed", pos2, 100, 100, -1, 1 );
				g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"TimeStopRingYRed", pos2, 100, 100, -1, 1 );
				g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"TimeStopRingZRed", pos2, 100, 100, -1, 1 );

#ifdef SERV_SKILL_NOTE
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO2 ) == true )
				{
					pUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_DEBUFF_BIND_WITH_MEMO ) );
					//pUnit->SetAnimSpeedFactor( 0.8f, 10.f, CX2GameUnit::SFI_SPEED_1 );
				}
#endif	SERV_SKILL_NOTE
			}
		}
		if( nCount > 0 )
		{
			PlaySound( L"Aisha_special_attack1.ogg" );
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{
	case 1:
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

void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_HYPER_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI1_AV_BINDING_CIRCLE_HYPER_End()
{
	CommonStateEnd();
}



//AVSI_SI2_AV_LIGHTNING_BOLT
void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.2f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO8 ) == true )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Lightning_Bolt_Memo", this );
		}
		else
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Lightning_Bolt", this );
		}		

#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
		m_pLightningBolt->GetMatrix().Move( GetPos() );
		m_pLightningBolt->GetMatrix().RotateDegree( GetRotateDegree() );
		m_pLightningBolt->ChangeAnim( L"LightningBolts2a-0", false );
		m_pLightningBolt->SetPlaySpeed( 0.6f );
		m_pLightningBolt->Play( CKTDGXSkinAnim::XAP_ONE );

//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST


		UpDownCrashCamera( 20.0f, 0.7f );
		g_pKTDXApp->GetDGManager()->ClearScreen();


		D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_01 ) );
		if( NULL != pSeqLightning1 )
		{
			PlaySequenceByTriggerTime( pSeqLightning1, pos,  5,9, 0.7f );
			pSeqLightning1->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning1->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning2 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_02 ) );
		if( NULL != pSeqLightning2 )
		{
			PlaySequenceByTriggerTime( pSeqLightning2, pos,  5,9, 0.6f );
			pSeqLightning2->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning2->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning3 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_03 ) );
		if( NULL != pSeqLightning3 )
		{
			PlaySequenceByTriggerTime( pSeqLightning3, pos, 5,7, 0.8f );
			pSeqLightning3->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning3->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning4 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_04 ) );
		if( NULL != pSeqLightning4 )
		{
			PlaySequenceByTriggerTime( pSeqLightning4, pos, 5,7, 0.7f  );
			pSeqLightning4->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning4->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning5 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_05 ) );
		if( NULL != pSeqLightning5 )
		{
			PlaySequenceByTriggerTime( pSeqLightning5, pos,5,9, 0.65f  );
			pSeqLightning5->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning5->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning6 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_06 ) );
		if( NULL != pSeqLightning6 )
		{
			PlaySequenceByTriggerTime( pSeqLightning6, pos, 5,7, 0.75f );
			pSeqLightning6->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning6->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning7 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_07 ) );
		if( NULL != pSeqLightning7 )
		{
			PlaySequenceByTriggerTime( pSeqLightning7, pos, 1,2, 0.7f );
			pSeqLightning7->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning7->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}



		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning8 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_08 ) );
		if( NULL != pSeqLightning8 )
		{
			PlaySequenceByTriggerTime( pSeqLightning8, pos, 1,2, 0.6f );
			pSeqLightning8->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning8->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning9 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_09 ) );
		if( NULL != pSeqLightning9 )
		{
			PlaySequenceByTriggerTime( pSeqLightning9, pos, 1,2, 0.8f );
			pSeqLightning9->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning9->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning10 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_10 ) );
		if( NULL != pSeqLightning10 )
		{
			PlaySequenceByTriggerTime( pSeqLightning10, pos, 1,2, 0.7f  );
			pSeqLightning10->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning10->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning11 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_11 ) );
		if( NULL != pSeqLightning11 )
		{
			PlaySequenceByTriggerTime( pSeqLightning11, pos,  1,2, 0.65f  );
			pSeqLightning11->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning11->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning12 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_12 ) );
		if( NULL != pSeqLightning12 )
		{
			PlaySequenceByTriggerTime( pSeqLightning12, pos, 1,2, 0.75f );
			pSeqLightning12->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning12->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightningCore = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_CORE ) );

		if( NULL != pSeqLightningCore )
		{

			PlaySequenceByTriggerTime( pSeqLightningCore, pos, 50,50, 0.75f );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST

			pSeqLightningCore->SetScaleByUnit( GetScaleByUnit() );

#endif GIANT_UNIT_GIANT_EFFECT_TEST
		}


	}



	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{	
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

void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_End()
{
	CommonStateEnd();
}

//AVSI_SI2_AV_LIGHTNING_BOLT_HYPER
void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.2f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO8 ) == true )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Lightning_Bolt_Memo", this, NULL, true );
		}
		else
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Lightning_Bolt", this, NULL, true );
		}		

		UpDownCrashCamera( 20.0f, 0.7f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_01 ) );
		if( NULL != pSeqLightning1 )
		{
			PlaySequenceByTriggerTime( pSeqLightning1, pos,   8,12, 0.6f );
			pSeqLightning1->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning1->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning2 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_02 ) );
		if( NULL != pSeqLightning2 )
		{
			PlaySequenceByTriggerTime( pSeqLightning2, pos,  8,10, 0.8f );
			pSeqLightning2->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning2->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning3 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_03 ) );
		if( NULL != pSeqLightning3 )
		{
			PlaySequenceByTriggerTime( pSeqLightning3, pos,  8,10, 0.7f );
			pSeqLightning3->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning3->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning4 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_04 ) );
		if( NULL != pSeqLightning4 )
		{
			PlaySequenceByTriggerTime( pSeqLightning4, pos, 8,10, 0.7f );
			pSeqLightning4->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning4->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning5 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_05 ) );
		if( NULL != pSeqLightning5 )
		{
			PlaySequenceByTriggerTime( pSeqLightning5, pos,  8,12, 0.7f  );
			pSeqLightning5->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning5->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}
		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning6 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_06 ) );
		if( NULL != pSeqLightning6 )
		{
			PlaySequenceByTriggerTime( pSeqLightning6, pos,8,10, 0.65f );
			pSeqLightning6->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning6->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}
		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning7 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_07 ) );
		if( NULL != pSeqLightning7 )
		{
			PlaySequenceByTriggerTime( pSeqLightning7, pos, 4,6, 0.75f  );
			pSeqLightning7->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning7->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}






		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning8 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_08 ) );
		if( NULL != pSeqLightning8 )
		{
			PlaySequenceByTriggerTime( pSeqLightning8, pos,  4,5, 0.75f );
			pSeqLightning8->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning8->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST



		}
		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning9 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_09 ) );
		if( NULL != pSeqLightning9 )
		{
			PlaySequenceByTriggerTime( pSeqLightning9, pos,  4,5, 0.75f );
			pSeqLightning9->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning9->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST



		}
		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning10 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_10 ) );
		if( NULL != pSeqLightning10 )
		{
			PlaySequenceByTriggerTime( pSeqLightning10, pos,  4,5, 0.75f  );
			pSeqLightning10->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning10->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}
		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning11 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_11 ) );
		if( NULL != pSeqLightning11 )
		{
			PlaySequenceByTriggerTime( pSeqLightning11, pos,   4,5, 0.75f  );
			pSeqLightning11->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning11->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning12 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_12 ) );
		if( NULL != pSeqLightning12 )
		{
			PlaySequenceByTriggerTime( pSeqLightning12, pos,  4,5, 0.75f );
			pSeqLightning12->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning12->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}





		CKTDGParticleSystem::CParticleEventSequence* pSeqLightningCore = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_CORE ) );

		if( NULL != pSeqLightningCore )
		{
			PlaySequenceByTriggerTime( pSeqLightningCore, pos, 50,50, 0.75f );
#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightningCore->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


	}



	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{	
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

void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI2_AV_LIGHTNING_BOLT_HYPER_End()
{
	CommonStateEnd();
}



#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
//AVSI_SI3_AV_METEO_CALL
void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.05f, 2 );

	ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 1.766f, 2, true );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.34f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.34f ) == true && EventCheck( 0.34f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = GetPos();
		D3DXVECTOR3 degree = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"METEO_CALL", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
	}
	
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{	
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.34f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.1f )
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

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.34f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.1f )
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

void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_End()
{
	CommonStateEnd();
}

//AVSI_SI3_AV_METEO_CALL_HYPER
void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.05f, 2 );

	ShowActiveSkillCutInAndLight( L"Bip01", 1.766f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.34f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.34f ) == true && EventCheck( 0.34f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = GetPos();
		D3DXVECTOR3 degree = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"METEO_BLAST", GetPowerRate(), posR, degree, degree, m_FrameDataNow.unitCondition.landPosition.y );
	}
	
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_HYPER_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_HYPER_CameraMove()
{
	CommonCameraMove();

	switch( m_FrameDataNow.stateParam.normalCamera )
	{	
	case 1:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.34f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.1f )
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

	case 2:
		{
			if( m_pXSkinAnim->GetNowAnimationTime() < 0.34f )
			{
				if( m_LuaManager.BeginTable( "CAMERA", m_FrameDataNow.stateParam.normalCamera ) == true )
				{
					g_pX2Game->GetX2Camera()->PlayLuaCamera( this, m_LuaManager, "SUB_CAMERA0" );
					m_LuaManager.EndTable();
				}
			}
			else if( m_pXSkinAnim->GetNowAnimationTime() < 1.1f )
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

void CX2GUArme_VioletMage::AVSI_SI3_AV_METEO_CALL_HYPER_End()
{
	CommonStateEnd();
}
#endif //UPGRADE_SKILL_SYSTEM_2013

//AVSI_SI1_AV_CHAIN_FIRE_BALL
void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_FIRE_BALL_FrameMove()
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	ChangeWorldColorByHyperMode();
#ifdef CHAIN_FIRE_BALL_SOUND_BUG_FIX
	if(g_pKTDXApp != NULL && g_pKTDXApp->GetDeviceManager() != NULL)
	{
		g_pKTDXApp->GetDeviceManager()->StopSound( L"Sound_Arme_shout2.ogg" );
	}
#endif CHAIN_FIRE_BALL_SOUND_BUG_FIX

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.19f, 0 );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파이어볼쏘기
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 빙점 연구 패시브를 습득했다면 이펙트 변경 추가
		// 파이어 볼 강화 패시브를 습득했다면 데미지 배율 변경 추가
		wstring wstrDamageEffectName = L"FIREBALL_CHAIN";			
		if ( true == m_bIceBall )
			wstrDamageEffectName = L"FIREBALL_CHAIN_Ice";
		
		if ( NULL != g_pX2Game->GetDamageEffect() )
		{
			CX2DamageEffect::CEffect * pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageEffectName.c_str(), GetPowerRate() * m_fIceBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			if ( CKTDGXMeshPlayer::CXMeshInstance* pManiEffect = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			{
				float fMaxLifeTime = pManiEffect->GetMaxLifeTime();
				pManiEffect->SetMaxLifeTime( fMaxLifeTime * m_fFireBallRangeRate);
			}
		}
#else	// BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편


	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파이어볼쏘기
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
		
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 빙점 연구 패시브를 습득했다면 이펙트 변경 추가
		// 파이어 볼 강화 패시브를 습득했다면 데미지 배율 변경 추가

		wstring wstrDamageEffectName = L"FIREBALL_CHAIN";			
		if ( true == m_bIceBall )
			wstrDamageEffectName = L"FIREBALL_CHAIN_Ice";

		if ( NULL != g_pX2Game->GetDamageEffect() )
		{
			CX2DamageEffect::CEffect * pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageEffectName.c_str(), GetPowerRate() * m_fIceBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			if ( CKTDGXMeshPlayer::CXMeshInstance* pManiEffect = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			{
				float fMaxLifeTime = pManiEffect->GetMaxLifeTime();
				pManiEffect->SetMaxLifeTime( fMaxLifeTime * m_fFireBallRangeRate);
			}
		}
#else	// BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파이어볼쏘기
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 빙점 연구 패시브를 습득했다면 이펙트 변경 추가
		// 파이어 볼 강화 패시브를 습득했다면 데미지 배율 변경 추가

		wstring wstrDamageEffectName = L"FIREBALL_CHAIN_DOWN_Ice";
		if ( true == m_bIceBall )												// 빙점 연구를 배웠고
		{
			if ( true == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO1 ) )	// 체인 파이어 볼 메모가 있다면
			{
				wstrDamageEffectName =  L"FIREBALL_CHAIN_DOWN_Ice_MEMO";		
			}
		}
		else																	// 빙점 연구를 배우지 않았고
		{
			wstrDamageEffectName = L"FIREBALL_CHAIN_DOWN";
			if ( true == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO1 ) )	// 체인 파이어 볼 메모가 있다면
			{
				wstrDamageEffectName = L"FIREBALL_CHAIN_DOWN_MEMO";
			}
		}

		if ( NULL != g_pX2Game->GetDamageEffect() )
		{
			CX2DamageEffect::CEffect * pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageEffectName.c_str(), GetPowerRate() * m_fIceBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			if ( CKTDGXMeshPlayer::CXMeshInstance* pMainEffect = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			{
				float fMaxLifeTime = pMainEffect->GetMaxLifeTime();
				pMainEffect->SetMaxLifeTime( fMaxLifeTime * m_fFireBallRangeRate);
			}
		}
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO1 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN_DOWN_MEMO", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN_DOWN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN_DOWN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif	SERV_SKILL_NOTE


#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_FIRE_BALL_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_FIRE_BALL_EventProcess()
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


//AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER
void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER_FrameMove()
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	ChangeWorldColorByHyperMode();
#ifdef CHAIN_FIRE_BALL_SOUND_BUG_FIX
	if(g_pKTDXApp != NULL && g_pKTDXApp->GetDeviceManager() != NULL)
	{
		g_pKTDXApp->GetDeviceManager()->StopSound( L"Sound_Arme_shout2.ogg" );
	}
#endif CHAIN_FIRE_BALL_SOUND_BUG_FIX

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.19f, 0 );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파이어볼쏘기
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 빙점 연구 패시브를 습득했다면 이펙트 변경 추가
		// 파이어 볼 강화 패시브를 습득했다면 데미지 배율 변경 추가

		wstring wstrDamageEffectName = L"FIREBALL_CHAIN";			
		if ( true == m_bIceBall )
			wstrDamageEffectName = L"FIREBALL_CHAIN_Ice";

		if ( NULL != g_pX2Game->GetDamageEffect() )
		{
			CX2DamageEffect::CEffect * pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageEffectName.c_str(), GetPowerRate() * m_fIceBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			if ( CKTDGXMeshPlayer::CXMeshInstance* pMainEffect = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			{
				float fMaxLifeTime = pMainEffect->GetMaxLifeTime();
				pMainEffect->SetMaxLifeTime( fMaxLifeTime * m_fFireBallRangeRate);
			}
		}
#else	// BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파이어볼쏘기
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 빙점 연구 패시브를 습득했다면 이펙트 변경 추가
		// 파이어 볼 강화 패시브를 습득했다면 데미지 배율 변경 추가

		wstring wstrDamageEffectName = L"FIREBALL_CHAIN";			
		if ( true == m_bIceBall )
			wstrDamageEffectName = L"FIREBALL_CHAIN_Ice";

		if ( NULL != g_pX2Game->GetDamageEffect() )
		{
			CX2DamageEffect::CEffect * pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageEffectName.c_str(), GetPowerRate() * m_fIceBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			if ( CKTDGXMeshPlayer::CXMeshInstance* pMainEffect = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			{
				float fMaxLifeTime = pMainEffect->GetMaxLifeTime();
				pMainEffect->SetMaxLifeTime( fMaxLifeTime * m_fFireBallRangeRate);
			}
		}
#else	// BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		//파이어볼쏘기
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 빙점 연구 패시브를 습득했다면 이펙트 변경 추가
		// 파이어 볼 강화 패시브를 습득했다면 데미지 배율 변경 추가

		wstring wstrDamageEffectName = L"FIREBALL_CHAIN_DOWN_Ice";
		if ( true == m_bIceBall )												// 빙점 연구를 배웠고
		{
			if ( true == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO1 ) )	// 체인 파이어 볼 메모가 있다면
			{
				wstrDamageEffectName =  L"FIREBALL_CHAIN_DOWN_Ice_MEMO";		
			}
		}
		else																	// 빙점 연구를 배우지 않았고
		{
			wstrDamageEffectName = L"FIREBALL_CHAIN_DOWN";
			if ( true == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO1 ) )	// 체인 파이어 볼 메모가 있다면
			{
				wstrDamageEffectName = L"FIREBALL_CHAIN_DOWN_MEMO";
			}
		}

		if ( NULL != g_pX2Game->GetDamageEffect() )
		{
			CX2DamageEffect::CEffect * pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageEffectName.c_str(), GetPowerRate() * m_fIceBallPower, posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
			if ( CKTDGXMeshPlayer::CXMeshInstance* pMainEffect = ( pEffect != NULL ) ? pEffect->GetMainEffect() : NULL )
			{
				float fMaxLifeTime = pMainEffect->GetMaxLifeTime();
				pMainEffect->SetMaxLifeTime( fMaxLifeTime * m_fFireBallRangeRate);
			}
		}
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO1 ) == true )
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN_DOWN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN_DOWN_MEMO", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#else
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"FIREBALL_CHAIN_DOWN", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif	SERV_SKILL_NOTE


#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_FIRE_BALL_HYPER_EventProcess()
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



//AVSI_SI1_AV_HELL_STONE
void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_Init()
{
	XSkinMeshReadyInBackground( L"HellStone_Normal.X" );
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect*								m_pHellStoneEffect = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO10 ) == true && GetRandomFloat() <= 0.3f )
		{
			m_pHellStoneEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HELL_STONE_MEMO", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			m_pHellStoneEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HELL_STONE", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hHellStoneEffect = ( m_pHellStoneEffect != NULL ) ? m_pHellStoneEffect->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_EventProcess()
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


//AVSI_SI1_AV_HELL_STONE_HYPER
void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.001f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CX2DamageEffect::CEffect*								m_pHellStoneEffect = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO10 ) == true && GetRandomFloat() <= 0.3f )
		{
			m_pHellStoneEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HELL_STONE_HYPER_MEMO", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
		else
		{
			m_pHellStoneEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"HELL_STONE_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hHellStoneEffect = ( m_pHellStoneEffect != NULL ) ? m_pHellStoneEffect->GetHandle() : INVALID_DAMAGE_EFFECT_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_HELL_STONE_HYPER_EventProcess()
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


//AVSI_SI2_AV_GUIDE_FIRE_BALL
void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_Init()
{

	TextureReadyInBackground( L"Black_Light.tga" );
	TextureReadyInBackground( L"Black_pulse.dds" );
	TextureReadyInBackground( L"Particle_Blur.dds" );
	XSkinMeshReadyInBackground( L"GuideArrow_AttackBox.X" );
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.3f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.55f ) == true && EventCheck( 0.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = GetWeaponBonePos( 0, L"TRACE_START0", 0 );
		D3DXVECTOR3 degree = GetRotateDegree();

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 가장 근처에 있는 대상을 목표로 잡을수 있게 수정
	#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO3 ) == true )
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_MEMO", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NEARST_UID_VECTOR );
		else
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NEARST_UID_VECTOR );
	#else  SERV_SKILL_NOTE
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NEARST_UID_VECTOR );
	#endif SERV_SKILL_NOTE
#else  BALANCE_ELEMENTAL_MASTER_20130117
	#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO3 ) == true )
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_MEMO", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
		else
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
	#else	SERV_SKILL_NOTE
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
	#endif	SERV_SKILL_NOTE
#endif BALANCE_ELEMENTAL_MASTER_20130117
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_EventProcess()
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


//AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER
void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.3f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.55f ) == true && EventCheck( 0.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 posR = GetWeaponBonePos( 0, L"TRACE_START0", 0 );
		D3DXVECTOR3 degree = GetRotateDegree();

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 가장 근처에 있는 대상을 목표로 잡을수 있게 수정
	#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO3 ) == true )
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_MEMO_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NEARST_UID_VECTOR );
		else
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NEARST_UID_VECTOR );
	#else  SERV_SKILL_NOTE
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ), 0, CX2DamageEffect::LOT_NEARST_UID_VECTOR );
	#endif SERV_SKILL_NOTE
#else  BALANCE_ELEMENTAL_MASTER_20130117
	#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO3 ) == true )
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_MEMO_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
		else
			SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
	#else	SERV_SKILL_NOTE
		SetLockOn( g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUIDE_FIRE_BALL_HYPER", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y ) );
	#endif	SERV_SKILL_NOTE
#endif BALANCE_ELEMENTAL_MASTER_20130117
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUIDE_FIRE_BALL_HYPER_EventProcess()
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


//AVSI_SI2_AV_GUST_SCREW
void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_Init()
{
	XSkinMeshReadyInBackground( L"GustScrew01.X" );
	XSkinMeshReadyInBackground( L"GustScrew02.X" );
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.27f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.466f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.466f ) == true && EventCheck( 0.466f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUST_SCREW", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.51f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.51f ) == true && EventCheck( 0.51f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustScrew01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.46f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.46f ) == true && EventCheck( 0.46f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustScrew02", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.52f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.52f ) == true && EventCheck( 0.52f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 15.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustScrew02", pos, GetRotateDegree(), GetRotateDegree() );
	}
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_EventProcess()
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


//AVSI_SI2_AV_GUST_SCREW_HYPER
void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.27f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.466f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.466f ) == true && EventCheck( 0.466f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUST_SCREW_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.51f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.51f ) == true && EventCheck( 0.51f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustScrew01", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.46f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.46f ) == true && EventCheck( 0.46f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustScrew02", GetPos(), GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.52f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.52f ) == true && EventCheck( 0.52f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 15.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustScrew02", pos, GetRotateDegree(), GetRotateDegree() );
	}
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_GUST_SCREW_HYPER_EventProcess()
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


//AVSI_SI3_AV_GUST_STORM
void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_Init()
{
	XSkinMeshReadyInBackground( L"GustStorm01.x" );
	XSkinMeshReadyInBackground( L"GustStorm01_1.x" );
	XSkinMeshReadyInBackground( L"GustStorm02.x" );
	XSkinMeshReadyInBackground( L"GustStorm03.x" );
	XSkinMeshReadyInBackground( L"GustStorm04.x" );
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.12f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm03", pos, GetRotateDegree(), GetRotateDegree() );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        if ( pMeshInstance != NULL )    pMeshInstance->SetPerFrameSimulation( false);
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		pos.y += 2;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm02", pos, GetRotateDegree(), GetRotateDegree() );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        if ( pMeshInstance != NULL )    pMeshInstance->SetPerFrameSimulation( false);
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm04", pos, GetRotateDegree(), GetRotateDegree() );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        if ( pMeshInstance != NULL )    pMeshInstance->SetPerFrameSimulation( false);
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.666f ) == true && EventCheck( 0.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUST_STORM_LOW", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUST_STORM_TOWER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.733f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.733f ) == true && EventCheck( 0.733f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm01", pos, GetRotateDegree(), GetRotateDegree() );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        if ( pMeshInstance != NULL )    pMeshInstance->SetPerFrameSimulation( false);
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
	}
	
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_EventProcess()
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


//AVSI_SI3_AV_GUST_STORM_HYPER
void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_HYPER_FrameMoveFuture()
{
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.12f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm03", pos, GetRotateDegree(), GetRotateDegree() );
		pos.y += 2;
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm02", pos, GetRotateDegree(), GetRotateDegree() );
		pMeshInstance = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm04", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.666f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.666f ) == true && EventCheck( 0.666f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUST_STORM_LOW", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"GUST_STORM_TOWER_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.733f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.733f ) == true && EventCheck( 0.733f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
        CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = NULL;
		pMeshInstance  = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"GustStorm01", pos, GetRotateDegree(), GetRotateDegree() );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_HYPER_CameraMove()
{
	CommonCameraMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_GUST_STORM_HYPER_EventProcess()
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


// 플라즈마 커터
void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_Init()
{
	XSkinMeshReadyInBackground( L"Catastrophe_Laser.X" );
	XSkinMeshReadyInBackground( L"Catastrophe01.X" );
	XSkinMeshReadyInBackground( L"Catastrophe02.X" );
	XSkinMeshReadyInBackground( L"Catastrophe03.X" );
	XSkinMeshReadyInBackground( L"Catastrophe04.X" );
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 2.6f )
	{
		if( m_InputData.pureRight == true )
		{
			//m_FrameDataFuture.syncData.bIsRight = true;
		}
		else if( m_InputData.pureLeft == true )
		{
			//m_FrameDataFuture.syncData.bIsRight = false;
		}
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.3f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.587f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.587f ) == true && EventCheck( 0.587f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		wstring wstrEffectName = L"CATASTROPHE_LASER";

		if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO23) == true )
			wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hCatastropheLaser = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pCatastropheLaser = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
			this, wstrEffectName.c_str(), GetPowerRate(), GetBonePos(L"Bip01_L_Finger0"), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else //ADD_MEMO_1ST_CLASS
            this, L"CATASTROPHE_LASER", GetPowerRate(), GetBonePos(L"Bip01_L_Finger0"), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pCatastropheLaser = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hCatastropheLaser ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( CX2DamageEffect::CEffect* pCatastropheLaser = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pCatastropheLaser ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
		if( CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pCatastropheLaser->GetMainEffect() )
		{
			pMeshInst->SetPos( GetBonePos(L"Bip01_L_Finger0") );
			pMeshInst->SetRotateDegree( GetRotateDegree() );
		}
	}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 60.0f;
		else
			pos = pos - GetDirVector() * 60.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe01", pos, GetRotateDegree(), GetRotateDegree() );

		pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 55.0f;
		else
			pos = pos - GetDirVector() * 55.0f;

		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Catastrophe_P01", pos );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Catastrophe_P02", pos );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Catastrophe_P03", pos );

		pos.y += 2.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe03", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * -10.0f;
		else
			pos = pos - GetDirVector() * -10.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe01", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.566f ) == true && EventCheck( 0.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * -100.0f;
		else
			pos = pos - GetDirVector() * -100.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe02", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 80.0f;
		else
			pos = pos - GetDirVector() * 80.0f;
		pos.y -= 26.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe02-1", pos, GetRotateDegree(), GetRotateDegree() );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_End()
{
	CommonStateEnd();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hCatastropheLaser = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pCatastropheLaser = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}


//AVSI_SI3_AV_CATASTROPHE_HYPER
void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_HYPER_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 2.6f )
	{
		if( m_InputData.pureRight == true )
		{
			//m_FrameDataFuture.syncData.bIsRight = true;
		}
		else if( m_InputData.pureLeft == true )
		{
			//m_FrameDataFuture.syncData.bIsRight = false;
		}
	}	
	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.3f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.587f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.587f ) == true && EventCheck( 0.587f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		wstring wstrEffectName = L"CATASTROPHE_LASER_HYPER";

		if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO23) == true )
			wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hCatastropheLaser = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pCatastropheLaser = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
			this, wstrEffectName.c_str(), GetPowerRate(), GetBonePos(L"Bip01_L_Finger0"), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else //ADD_MEMO_1ST_CLASS
            this, L"CATASTROPHE_LASER_HYPER", GetPowerRate(), GetBonePos(L"Bip01_L_Finger0"), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");

		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 50.0f;
		else
			pos = pos - GetDirVector() * 50.0f;
		pos.y += 50.0f;

		D3DXVECTOR3 angle = GetRotateDegree();
		angle.x += 70.0f;

#ifdef ADD_MEMO_1ST_CLASS //김창한
		wstring wstrEffectName = L"CATASTROPHE_LASER_HYPER";

		if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO23) == true )
			wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hCatastropheLaser1 = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pCatastropheLaser1 = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
			this, wstrEffectName.c_str(), GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#else //ADD_MEMO_1ST_CLASS
            this, L"CATASTROPHE_LASER_HYPER", GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 100.0f;
		else
			pos = pos - GetDirVector() * 100.0f;
		pos.y += 150.0f;

		D3DXVECTOR3 angle = GetRotateDegree();
		angle.x += 180.0f;
		angle.z -= 30.0f;

#ifdef ADD_MEMO_1ST_CLASS //김창한
		wstring wstrEffectName = L"CATASTROPHE_LASER_HYPER";

		if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO23) == true )
			wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hCatastropheLaser2 = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pCatastropheLaser2 = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
			this, wstrEffectName.c_str(), GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#else //ADD_MEMO_1ST_CLASS
            this, L"CATASTROPHE_LASER_HYPER", GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 50.0f;
		else
			pos = pos - GetDirVector() * 50.0f;
		pos.y -= 30.0f;

		D3DXVECTOR3 angle = GetRotateDegree();
		angle.x += -70.0f;

#ifdef ADD_MEMO_1ST_CLASS //김창한
		wstring wstrEffectName = L"CATASTROPHE_LASER_HYPER";

		if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO23) == true )
			wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hCatastropheLaser3 = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pCatastropheLaser3 = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
			this, wstrEffectName.c_str(), GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#else //ADD_MEMO_1ST_CLASS
            this, L"CATASTROPHE_LASER_HYPER", GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.7f ) == true && EventCheck( 1.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 100.0f;
		else
			pos = pos - GetDirVector() * 100.0f;
		pos.y -= 80.0f;

		D3DXVECTOR3 angle = GetRotateDegree();
		angle.x += 180.0f;
		angle.z += 20.0f;

#ifdef ADD_MEMO_1ST_CLASS //김창한
		wstring wstrEffectName = L"CATASTROPHE_LASER_HYPER";

		if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO23) == true )
			wstrEffectName += L"_MEMO";
#endif //ADD_MEMO_1ST_CLASS

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hCatastropheLaser4 = g_pX2Game->GetDamageEffect()->CreateInstanceHandle( 
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pCatastropheLaser4 = g_pX2Game->GetDamageEffect()->CreateInstance( 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef ADD_MEMO_1ST_CLASS //김창한
			this, wstrEffectName.c_str(), GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#else //ADD_MEMO_1ST_CLASS
            this, L"CATASTROPHE_LASER_HYPER", GetPowerRate(), pos, angle, angle, m_FrameDataNow.unitCondition.landPosition.y );
#endif //ADD_MEMO_1ST_CLASS
	}

	if( m_pXSkinAnimFuture->GetNowAnimationTime() < 2.6f )
	{

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hCatastropheLaser ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if ( CX2DamageEffect::CEffect* pCatastropheLaser = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pCatastropheLaser ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			if( CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pCatastropheLaser->GetMainEffect() )
			{
				pMeshInst->SetPos( GetBonePos(L"Bip01_L_Finger0") );
				pMeshInst->SetRotateDegree( GetRotateDegree() );
			}
		}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser1 = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hCatastropheLaser1 ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser1 = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pCatastropheLaser1 ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			if( CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pCatastropheLaser1->GetMainEffect() )
			{
				D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");
				if( GetIsRight() == true )
					pos = pos + GetDirVector() * 50.0f;
				else
					pos = pos - GetDirVector() * 50.0f;
				pos.y += 50.0f;

				D3DXVECTOR3 angle = GetRotateDegree();
				angle.x += 70.0f;

				pMeshInst->SetPos( pos );
				pMeshInst->SetRotateDegree( angle );
			}
		}


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser2 = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hCatastropheLaser2 ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser2 = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pCatastropheLaser2 ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pCatastropheLaser2->GetMainEffect();
			if( NULL != pMeshInst )
			{
				D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");
				if( GetIsRight() == true )
					pos = pos + GetDirVector() * 100.0f;
				else
					pos = pos - GetDirVector() * 100.0f;
				pos.y += 150.0f;

				D3DXVECTOR3 angle = GetRotateDegree();
				angle.x += 180.0f;
				angle.z -= 30.0f;

				pMeshInst->SetPos( pos );
				pMeshInst->SetRotateDegree( angle );
			}
		}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser3 = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hCatastropheLaser3 ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser3 = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pCatastropheLaser3 ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			if( CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pCatastropheLaser3->GetMainEffect() )
			{
				D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");
				if( GetIsRight() == true )
					pos = pos + GetDirVector() * 50.0f;
				else
					pos = pos - GetDirVector() * 50.0f;
				pos.y -= 30.0f;

				D3DXVECTOR3 angle = GetRotateDegree();
				angle.x += -70.0f;

				pMeshInst->SetPos( pos );
				pMeshInst->SetRotateDegree( angle );
			}
		}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser4 = g_pX2Game->GetDamageEffect()->ValidateInstanceHandle( m_hCatastropheLaser4 ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if ( CX2DamageEffect::CEffect* pCatastropheLaser4 = g_pX2Game->GetDamageEffect()->ValidateLiveInstance( m_pCatastropheLaser4 ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
			if( CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = pCatastropheLaser4->GetMainEffect() )
			{
				D3DXVECTOR3 pos = GetBonePos(L"Bip01_L_Finger0");
				if( GetIsRight() == true )
					pos = pos + GetDirVector() * 100.0f;
				else
					pos = pos - GetDirVector() * 100.0f;
				pos.y -= 80.0f;

				D3DXVECTOR3 angle = GetRotateDegree();
				angle.x += 180.0f;
				angle.z += 20.0f;

				pMeshInst->SetPos( pos );
				pMeshInst->SetRotateDegree( angle );
			}
		}

	}
#ifdef FIX_PLASMA_CUTTER_BUG
	else
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	    m_hCatastropheLaser			= INVALID_DAMAGE_EFFECT_HANDLE;
	    m_hCatastropheLaser1		= INVALID_DAMAGE_EFFECT_HANDLE;
	    m_hCatastropheLaser2		= INVALID_DAMAGE_EFFECT_HANDLE;
	    m_hCatastropheLaser3		= INVALID_DAMAGE_EFFECT_HANDLE;
	    m_hCatastropheLaser4		= INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	    m_pCatastropheLaser			= NULL;
	    m_pCatastropheLaser1		= NULL;
	    m_pCatastropheLaser2		= NULL;
	    m_pCatastropheLaser3		= NULL;
	    m_pCatastropheLaser4		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}
#endif FIX_PLASMA_CUTTER_BUG
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 60.0f;
		else
			pos = pos - GetDirVector() * 60.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe01", pos, GetRotateDegree(), GetRotateDegree() );

		pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 55.0f;
		else
			pos = pos - GetDirVector() * 55.0f;

		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Catastrophe_P01", pos );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Catastrophe_P02", pos );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Catastrophe_P03", pos );

		pos.y += 2.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe03", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * -10.0f;
		else
			pos = pos - GetDirVector() * -10.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe01", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.566f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.566f ) == true && EventCheck( 0.566f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * -100.0f;
		else
			pos = pos - GetDirVector() * -100.0f;
		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe02", pos, GetRotateDegree(), GetRotateDegree() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 80.0f;
		else
			pos = pos - GetDirVector() * 80.0f;
		pos.y -= 26.0f;

		g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"Catastrophe02-1", pos, GetRotateDegree(), GetRotateDegree() );
	}
	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_HYPER_EventProcess()
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

void CX2GUArme_VioletMage::AVSI_SI3_AV_CATASTROPHE_HYPER_End()
{
	CommonStateEnd();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_hCatastropheLaser			= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser1		= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser2		= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser3		= INVALID_DAMAGE_EFFECT_HANDLE;
	m_hCatastropheLaser4		= INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_pCatastropheLaser			= NULL;
	m_pCatastropheLaser1		= NULL;
	m_pCatastropheLaser2		= NULL;
	m_pCatastropheLaser3		= NULL;
	m_pCatastropheLaser4		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}


//AVSI_SI1_AV_CHAIN_BURST
//void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_BURST_Init()
//{
//
//}

void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_BURST_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.16f, 0 );

	D3DXVECTOR3 pos = GetPos();
	if( GetIsRight() == true )
		pos = pos + GetDirVector() * 50.0f;
	else
		pos = pos - GetDirVector() * 50.0f;
	pos.y += 50.0f;
	D3DXVECTOR3 rotate;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.13f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.13f ) == true && EventCheck( 0.13f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst01", GetBonePos( L"Dummy1_Rhand" ) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.8f ) == true && EventCheck( 1.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst01", GetBonePos( L"Dummy1_Rhand" ) );
	}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 체인 버스트 메모 - 파이어 볼 데미지 증가 효과
	float fIncreasePowerRate = 1.f;	/// 파이어 볼 데미지 증감치
#endif BALANCE_ELEMENTAL_MASTER_20130117

	wstring wstrDamageName = L"AISHA_CHAIN_BURST_FIREBALL_SMALL";
	wstring wstrDamageBigName = L"AISHA_CHAIN_BURST_FIREBALL_BIG";

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO9 ) == true )
	{
		wstrDamageName = L"AISHA_CHAIN_BURST_FIREBALL_SMALL_MEMO";
		wstrDamageBigName = L"AISHA_CHAIN_BURST_FIREBALL_BIG_MEMO";

#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 메모가 적용되어 있을 때, 크기 증가치 설정
		fIncreasePowerRate = _CONST_AISHA_::AHM_CHAIN_BURST_MEMO_INCREASE_POWER_RATE_VALUE;
#endif BALANCE_ELEMENTAL_MASTER_20130117
	}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 모든 데미지 이펙트 생성에 디파인 달면 알아보기 힘들것 같아서, 로직 수정
	bool bCreatFireBall = false;	/// 파이어볼 생성 여부

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst02", GetBonePos( L"Dummy1_Rhand" ) );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst03", GetBonePos( L"Dummy1_Rhand" ) );
		rotate = GetRotateDegree();
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 10.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 20.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 10.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1f ) == true && EventCheck( 1.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 20.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 40.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.3f ) == true && EventCheck( 1.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 40.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 30.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		bCreatFireBall = true;
		wstrDamageName = wstrDamageBigName;		/// 큰 파이어볼 설정
	}

	if( true == bCreatFireBall )
	{
		CX2DamageEffect::CEffect* pDe = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, 
			rotate, m_FrameDataNow.unitCondition.landPosition.y );

		if( 1.f < fIncreasePowerRate && NULL != pDe )		/// 메모로 인한 스케일 증가값이 적용되어 있다면, 스케일 수정
		{
			pDe->SetScale( fIncreasePowerRate, fIncreasePowerRate, fIncreasePowerRate );
		}
	}
#else //UPGRADE_SKILL_SYSTEM_2013
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst02", GetBonePos( L"Dummy1_Rhand" ) );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst03", GetBonePos( L"Dummy1_Rhand" ) );
		rotate = GetRotateDegree();
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 10.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 10.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 40.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 30.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		bCreatFireBall = true;
		wstrDamageName = wstrDamageBigName;		/// 큰 파이어볼 설정
	}

	if( true == bCreatFireBall )		/// 파이어볼 생성시 데미지 증감치 설정
	{
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate() * fIncreasePowerRate, pos, rotate, 
			rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
#else  BALANCE_ELEMENTAL_MASTER_20130117
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst02", GetBonePos( L"Dummy1_Rhand" ) );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst03", GetBonePos( L"Dummy1_Rhand" ) );
		rotate = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 10.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 10.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 40.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 30.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageBigName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#endif BALANCE_ELEMENTAL_MASTER_20130117

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_BURST_EventProcess()
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


//AVSI_SI1_AV_CHAIN_BURST_HYPER
void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_BURST_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.16f, 0 );

	D3DXVECTOR3 pos = GetPos();
	if( GetIsRight() == true )
		pos = pos + GetDirVector() * 50.0f;
	else
		pos = pos - GetDirVector() * 50.0f;
	pos.y += 50.0f;

	D3DXVECTOR3 rotate;

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.13f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.13f ) == true && EventCheck( 0.13f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst01", GetBonePos( L"Dummy1_Rhand" ) );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.8f ) == true && EventCheck( 1.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst01", GetBonePos( L"Dummy1_Rhand" ) );
	}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117				/// 체인 버스트 메모 - 크기 증가 효과 적용
	float fIncreaseScale = 1.f;		/// 크기 증가치
#endif BALANCE_ELEMENTAL_MASTER_20130117

	wstring wstrDamageName = L"AISHA_CHAIN_BURST_FIREBALL_SMALL";
	wstring wstrDamageBigName = L"AISHA_CHAIN_BURST_FIREBALL_BIG";

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO9 ) == true )
	{
		wstrDamageName = L"AISHA_CHAIN_BURST_FIREBALL_SMALL_MEMO";
		wstrDamageBigName = L"AISHA_CHAIN_BURST_FIREBALL_BIG_MEMO";

#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 메모가 적용되어 있을 때, 크기 증가치 설정
		fIncreaseScale = _CONST_AISHA_::AHM_CHAIN_BURST_MEMO_INCREASE_POWER_RATE_VALUE;
#endif BALANCE_ELEMENTAL_MASTER_20130117
	}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 모든 데미지 이펙트 생성에 디파인 달면 알아보기 힘들것 같아서, 로직 수정
	bool bCreatFireBall = false;	/// 파이어볼 생성 여부

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst02", GetBonePos( L"Dummy1_Rhand" ) );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst03", GetBonePos( L"Dummy1_Rhand" ) );
		rotate = GetRotateDegree();
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 10.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 20.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 10.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1f ) == true && EventCheck( 1.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 20.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 40.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.3f ) == true && EventCheck( 1.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 40.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 30.0f;
		bCreatFireBall = true;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		bCreatFireBall = true;
		wstrDamageName = wstrDamageBigName;		/// 큰 파이어볼 설정
	}

	if( true == bCreatFireBall )
	{
		CX2DamageEffect::CEffect* pDe = g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, 
										rotate, m_FrameDataNow.unitCondition.landPosition.y );

		if( 1.f < fIncreaseScale && NULL != pDe )		/// 메모로 인한 스케일 증가값이 적용되어 있다면, 스케일 수정
		{
			pDe->SetScale( fIncreaseScale, fIncreaseScale, fIncreaseScale );
		}
	}
#else  BALANCE_ELEMENTAL_MASTER_20130117
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst02", GetBonePos( L"Dummy1_Rhand" ) );
		g_pX2Game->GetMinorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_ChainBurst03", GetBonePos( L"Dummy1_Rhand" ) );
		rotate = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 10.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 20.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.9f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.9f ) == true && EventCheck( 0.9f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 10.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.1f ) == true && EventCheck( 1.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 20.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.2f ) == true && EventCheck( 1.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z += 40.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.3f ) == true && EventCheck( 1.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 40.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		rotate.z -= 30.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		rotate = GetRotateDegree();
		g_pX2Game->GetDamageEffect()->CreateInstance( this, wstrDamageBigName.c_str(), GetPowerRate(), pos, rotate, rotate, m_FrameDataNow.unitCondition.landPosition.y );
	}
#endif BALANCE_ELEMENTAL_MASTER_20130117

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI1_AV_CHAIN_BURST_HYPER_EventProcess()
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

#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION
void CX2GUArme_VioletMage::SetSelfDestructSummonedNPC( CX2UnitManager::NPC_UNIT_ID eNPCID /*= CX2UnitManager::NUI_NONE*/ )
{
	if( NULL == g_pX2Game )
		return;

	for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
	{
		CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
		if( NULL == pUnit )
			continue;

		if( CX2GameUnit::GUT_NPC != pUnit->GetGameUnitType() )
			continue;


		CX2GUNPC *pNPC = (CX2GUNPC*) pUnit;
		if( CX2UnitManager::NUI_NONE != eNPCID &&
			pNPC->GetNPCTemplet().m_nNPCUnitID != eNPCID )
		{
			continue;
		}

		if( pNPC->GetNowHp() <= 0.f )
			continue;

#ifndef MODIFY_DISTORTION_DESTRUCTION // 소멸 시간 초기화를 위해 제거
		// 이미 자폭중이면 
		if( true == pNPC->GetSelfDestruction() )
			continue;
#endif // MODIFY_DISTORTION_DESTRUCTION
		
		if( NULL != pNPC->GetNPCAI() &&
			CX2NPCAI::NAT_ALLY == pNPC->GetNPCAI()->GetAIType() )
		{
			CX2AllyNPCAI* pAllyAI = (CX2AllyNPCAI*) pNPC->GetNPCAI();

			if( GetUnitUID() == pAllyAI->GetAllyUnitUID() )
			{
				switch ( eNPCID )
				{
				case CX2UnitManager::NUI_ADW_DISTORTION:
					pNPC->StartSelfDestruction( 0.f );
					break;
				}
			}
		}
	}
}
#endif	SERV_ARME_DIMENSION_WITCH

/** @function : COMMON_BUFF_FrameMove
@brief : 공통적으로 쓰는 버프 스킬의 FrameMove
*/
/*virtual*/ void CX2GUArme_VioletMage::COMMON_BUFF_FrameMove()
{
#ifndef UPGRADE_SKILL_SYSTEM_2013 // 공통 스킬 개편, 김종훈
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.047f, 0 );
#endif // UPGRADE_SKILL_SYSTEM_2013 // 공통 스킬 개편, 김종훈
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.63f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.63f ) == true && EventCheck( 0.63f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		PlayCommonBuffMinorParticle();
		UpDownCrashCamera( 20.0f, 0.3f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
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

//AVSI_SI2_AV_DARK_CLOUD
void CX2GUArme_VioletMage::AVSI_SI2_AV_DARK_CLOUD_Init()
{
	TextureReadyInBackground( L"Poison01.dds" );
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_DARK_CLOUD_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.18f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy2_Lhand" );
		pos.y += 30.0f;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"DarkCloud_HandLight01", pos );		
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_DarkCloud03", pos );		
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"DarkCloud_PowerBooster01", GetPos() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO4 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DARK_CLOUD_MEMO", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DARK_CLOUD", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else	SERV_SKILL_NOTE
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DARK_CLOUD", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif	SERV_SKILL_NOTE
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_DARK_CLOUD_EventProcess()
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


//AVSI_SI2_AV_DARK_CLOUD_HYPER
void CX2GUArme_VioletMage::AVSI_SI2_AV_DARK_CLOUD_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.18f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetBonePos( L"Dummy2_Lhand" );
		pos.y += 30.0f;
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"DarkCloud_HandLight01", pos );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Light_DarkCloud03", pos );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.65f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.65f ) == true && EventCheck( 0.65f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"DarkCloud_PowerBooster01", GetPos() );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 1.0f ) == true && EventCheck( 1.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef SERV_SKILL_NOTE
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO4 ) == true )
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DARK_CLOUD_MEMO_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
		else
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DARK_CLOUD_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#else	SERV_SKILL_NOTE
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DARK_CLOUD_HYPER", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
#endif	SERV_SKILL_NOTE
	}


	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_DARK_CLOUD_HYPER_EventProcess()
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
//AVSI_SI2_AV_BIND_ATTACK  // 인퍼널 웨이브
void CX2GUArme_VioletMage::AVSI_SI2_AV_BIND_ATTACK_Init()
{
	
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_BIND_ATTACK_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.17f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.17f ) == true && EventCheck( 0.17f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.7f;
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.51f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.51f ) == true && EventCheck( 0.51f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.2f;
		m_DamageData.hitUnitList.resize(0);
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_BIND_ATTACK_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.1f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
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
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.6f ) == true && EventCheck( 1.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef NO_MORE_EFFECT_HARD_CODING_TEST

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Bind_Attack_Lightning_Bolt", this );


#else NO_MORE_EFFECT_HARD_CODING_TEST
//{{AFX
		m_DamageData.attackerType	= CX2DamageManager::AT_MESH;
		m_DamageData.bCanRevenge	= false;
		m_DamageData.pAttackerAnim	= m_pLightningBolt;
		m_DamageData.damageType		= CX2DamageManager::DT_MAGIC;
		m_DamageData.attackType		= CX2DamageManager::AT_SPECIAL;

		m_DamageData.fHitGap		= 0.1f;
		m_DamageData.bReAttack		= true;
		m_DamageData.backSpeed.x	= 0.0f;
		m_DamageData.backSpeed.y	= 0.0f;
		m_DamageData.damage.fPhysic = 0.f;
		m_DamageData.damage.fMagic	= 0.5f * GetPowerRate();

		m_pLightningBolt->GetMatrix().Move( GetPos() );
		m_pLightningBolt->GetMatrix().RotateDegree( GetRotateDegree() );
		m_pLightningBolt->ChangeAnim( L"LightningBolts2a-0", false );
		m_pLightningBolt->SetPlaySpeed( 0.6f );
		m_pLightningBolt->Play( CKTDGXSkinAnim::XAP_ONE );
//}}AFX
#endif NO_MORE_EFFECT_HARD_CODING_TEST

		UpDownCrashCamera( 20.0f, 0.7f );
		g_pKTDXApp->GetDGManager()->ClearScreen();
		D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_01 ) );
		if( NULL != pSeqLightning1 )
		{
			PlaySequenceByTriggerTime( pSeqLightning1, pos, 5,9, 0.7f );
			pSeqLightning1->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning1->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning3 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_03 ) );
		if( NULL != pSeqLightning3 )
		{
			PlaySequenceByTriggerTime( pSeqLightning3, pos, 5,7, 0.8f );
			pSeqLightning3->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning3->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning5 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_05 ) );
		if( NULL != pSeqLightning5 )
		{
			PlaySequenceByTriggerTime( pSeqLightning5, pos, 5,9, 0.65f );
			pSeqLightning5->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning5->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning8 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_08 ) );
		if( NULL != pSeqLightning8 )
		{
			PlaySequenceByTriggerTime( pSeqLightning8, pos, 1,2, 0.6f  );
			pSeqLightning8->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning8->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning10 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_10 ) );
		if( NULL != pSeqLightning10 )
		{
			PlaySequenceByTriggerTime( pSeqLightning10, pos, 1,2, 0.7f );
			pSeqLightning10->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning10->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning12 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_12 ) );
		if( NULL != pSeqLightning12 )
		{
			PlaySequenceByTriggerTime( pSeqLightning12, pos, 1,2, 0.75f );
			pSeqLightning12->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning12->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightningCore = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_CORE ) );

		if( NULL != pSeqLightningCore )
		{
			PlaySequenceByTriggerTime( pSeqLightningCore, pos, 50,50, 0.75f );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightningCore->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

	}

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 2.4f, 2 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.8f ) == true && EventCheck( 2.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 20.0f, 0.8f );
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 200.0f;
		else
			pos = pos - GetDirVector() * 200.0f;
		pos.y += 100.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"BIND_ATTACK", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_BIND_ATTACK_EventProcess()
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


//AVSI_SI2_AV_BIND_ATTACK_HYPER
void CX2GUArme_VioletMage::AVSI_SI2_AV_BIND_ATTACK_HYPER_FrameMoveFuture()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnimFuture->EventTimerOneshot( 0.17f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnimFuture->EventTimer( 0.17f ) == true && EventCheck( 0.17f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.7f;
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnimFuture->EventTimerOneshot( 0.51f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnimFuture->EventTimer( 0.51f ) == true && EventCheck( 0.51f, true ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed.x = GetDashSpeed() * 1.2f;
	}

	CommonFrameMoveFuture();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_BIND_ATTACK_HYPER_FrameMove()
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.1f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_DamageData.hitUnitList.resize(0);
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.6f ) == true && EventCheck( 0.6f, false ) == true )
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
	}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.6f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 1.6f ) == true && EventCheck( 1.6f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Bind_Attack_Lightning_Bolt", this );

		UpDownCrashCamera( 20.0f, 0.7f );
		g_pKTDXApp->GetDGManager()->ClearScreen();
		D3DXVECTOR3 pos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning1 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_01 ) );
		if( NULL != pSeqLightning1 )
		{
			PlaySequenceByTriggerTime( pSeqLightning1, pos, 5,9, 0.7f );
			pSeqLightning1->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning1->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning3 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_03 ) );
		if( NULL != pSeqLightning3 )
		{
			PlaySequenceByTriggerTime( pSeqLightning3, pos, 5,7, 0.8f );
			pSeqLightning3->SetAddRotate( GetRotateDegree() );


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning3->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning5 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_05 ) );
		if( NULL != pSeqLightning5 )
		{
			PlaySequenceByTriggerTime( pSeqLightning5, pos, 5,9, 0.65f );
			pSeqLightning5->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning5->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning8 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_08 ) );
		if( NULL != pSeqLightning8 )
		{
			PlaySequenceByTriggerTime( pSeqLightning8, pos, 1,2, 0.6f  );
			pSeqLightning8->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning8->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning10 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_10 ) );
		if( NULL != pSeqLightning10 )
		{
			PlaySequenceByTriggerTime( pSeqLightning10, pos, 1,2, 0.7f );
			pSeqLightning10->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning10->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightning12 = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_RED_12 ) );
		if( NULL != pSeqLightning12 )
		{
			PlaySequenceByTriggerTime( pSeqLightning12, pos, 1,2, 0.75f );
			pSeqLightning12->SetAddRotate( GetRotateDegree() );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightning12->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

		}


		CKTDGParticleSystem::CParticleEventSequence* pSeqLightningCore = g_pX2Game->GetMajorParticle()->GetInstanceSequence( GetHandleAishaMajorParticleByEnum( AISHA_PII_MAJOR_LIGHTNING_BOLT_CORE ) );
		if( NULL != pSeqLightningCore )
		{
			PlaySequenceByTriggerTime( pSeqLightningCore, pos, 50,50, 0.75f );
#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
			pSeqLightningCore->SetScaleByUnit( GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}
		
	}

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 2.4f, 2 );
	
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 2.8f ) == true && EventCheck( 2.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		UpDownCrashCamera( 20.0f, 0.8f );
		D3DXVECTOR3 pos = GetPos();
		if( GetIsRight() == true )
			pos = pos + GetDirVector() * 250.0f;
		else
			pos = pos - GetDirVector() * 250.0f;
		pos.y += 100.0f;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"BIND_ATTACK_HYPER", GetPowerRate(), pos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SI2_AV_BIND_ATTACK_HYPER_EventProcess()
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
#endif //UPGRADE_SKILL_SYSTEM_2013
//
////AVSI_SI2_AV_LAND_OF_FLAME
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_Init()
//{
//
//}
//
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_FrameMoveFuture()
//{
//	CommonFrameMoveFuture();
//}
//
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_FrameMove()
//{
//	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );
//
//	if( m_pXSkinAnim->EventTimer( 0.27f ) == true && EventCheck( 0.27f, false ) == true )
//	{
//	}
//	else if( m_pXSkinAnim->EventTimer( 0.5f ) == true && EventCheck( 0.5f, false ) == true )
//	{
//		D3DXVECTOR3 pos = GetBonePos( L"Dummy2_Lhand" );
//		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"LandOfFlameShot", m_FrameDataNow.unitCondition.landPosition );
//		if( pSeq != NULL )
//		{
//			pSeq->SetBlackHolePosition( m_FrameDataNow.unitCondition.landPosition );
//		}
//	}
//
//
//	CommonFrameMove();
//}
//
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_CameraMove()
//{
//	CommonCameraMove();
//}
//
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_EventProcess()
//{
//	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
//	{
//		if( false == IsOnSomethingFuture() )
//			StateChange( AVSI_JUMP_DOWN );
//		else
//			StateChange( AVSI_WAIT );
//	}
//
//	CommonEventProcess();
//}
//
//
////AVSI_SI2_AV_LAND_OF_FLAME_HYPER
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_HYPER_FrameMoveFuture()
//{
//
//}
//
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_HYPER_FrameMove()
//{
//
//}
//
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_HYPER_CameraMove()
//{
//
//}
//
//void CX2GUArme_VioletMage::AVSI_SI2_AV_LAND_OF_FLAME_HYPER_EventProcess()
//{
//
//}

#ifndef SERV_9TH_NEW_CHARACTER // 김태환
/// 다른 캐릭터들 전부 똑같은 함수를 쓰고 있으니, X2GUUser로 옮기자.
void CX2GUArme_VioletMage::CreateNotEnoughMPEffect( D3DXVECTOR3 vPos, float fDegreeX, float fDegreeY, float fDegreeZ )
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
#endif // SERV_9TH_NEW_CHARACTER

#ifdef AISHA_SECOND_CLASS_CHANGE

void CX2GUArme_VioletMage::AddStoneWall( UidType uid, float fTimeLeft )
{
	StoneWallData stoneWallData;
	stoneWallData.m_NPCUID = uid;
	stoneWallData.m_fTimeLeft = fTimeLeft;

	m_vecStoneWallData.push_back( stoneWallData );
}

#endif AISHA_SECOND_CLASS_CHANGE

void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_READY_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_READY_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vBonePos = GetWeaponBonePos( 0, L"Center_Jewel", 0 );
		m_hSeqLight_Arme_Circle_Flame = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Light_Arme_Circle_Flame01", vBonePos );
		//CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqAssualtSlashRing );			
	}		
	if( m_hSeqLight_Arme_Circle_Flame != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqLight_Arme_Circle_Flame );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetWeaponBonePos( 0, L"Center_Jewel", 0 );
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqLight_Arme_Circle_Flame = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	CommonFrameMove();
}	
void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_READY_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( AVSI_SI_A_AV_CIRCLE_FLAME );
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_READY_EndFuture()
{
	m_bDisableGravity = false;
	CommonStateEndFuture();
}

void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_Init()
{
	TextureReadyInBackground( L"CenterLight_Gray01.dds" );
	TextureReadyInBackground( L"SpreadLight02_Gray.dds" );
	TextureReadyInBackground( L"SpreadLight02_Gray.dds" );		
	XSkinMeshReadyInBackground( L"Arme_SI_A_AV_Circle_Flame_CMesh01.X" );
	XSkinMeshReadyInBackground( L"Arme_SI_A_AV_Circle_Flame_CMesh02.X" );
	XSkinMeshReadyInBackground( L"Arme_SI_A_AV_Circle_Flame_CMesh03.X" );
}
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_StateStart()
{
	CommonStateStart();
	wstring wstrEffectSetName = L"EffectSet_Aisha_Circle_Flame_Ice";
	if ( true == m_bIceBall )
	{
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO7 ) == true )
		{
			wstrEffectSetName = L"EffectSet_Aisha_Circle_Flame_Ice_Memo";
		}
	}
	
	else
	{
		wstrEffectSetName = L"EffectSet_Aisha_Circle_Flame";
		if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO7 ) == true )
		{
			wstrEffectSetName = L"EffectSet_Aisha_Circle_Flame_Memo";
		}
	}

	if ( NULL != g_pX2Game->GetEffectSet() )
		g_pX2Game->GetEffectSet()->PlayEffectSet( wstrEffectSetName.c_str(), this );
}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_StartFuture()
{
	CommonStateStartFuture();

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_FrameMove()
{		
	if( m_hSeqLight_Arme_Circle_Flame != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeqLight_Arme_Circle_Flame );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = GetWeaponBonePos( 0, L"Center_Jewel", 0 );
			pSeq->SetPosition( vBonePos );
		}
		else
		{
			m_hSeqLight_Arme_Circle_Flame = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	CommonFrameMove();
}	
void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_EventProcess()
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
void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_FrameMoveFuture()
{
	if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.66f )
	{			
		m_bDisableGravity = false;
	}

	CommonFrameMoveFuture();
}
void CX2GUArme_VioletMage::AVSI_SI_A_AV_CIRCLE_FLAME_EndFuture()
{
	m_bDisableGravity = false;
	CommonStateEndFuture();
}






#ifdef SKILL_30_TEST

// 여기
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_Init()
	{
		TextureReadyInBackground( L"Ice_Particle01.dds" );
		XSkinMeshReadyInBackground( L"FrozenMissile01.X" );
		XSkinMeshReadyInBackground( L"Arme_SI_SA_BlizzardShower_Spin_Mesh01.X" );
		XMeshReadyInBackground( L"Lightning02.Y" );
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_StateStart()
	{
		CommonStateStart();

		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
		m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_BlizzardShowerNoDamage", this ); 

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 블리자드 샤워 발사 전 주위를 얼리는 공격 추가
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_BLIZZARD_SHOWER_FROZEN_ATTACK", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(),  GetLandPos().y );
#endif BALANCE_ELEMENTAL_MASTER_20130117

		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.93f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.93f ) == true && EventCheck( 0.93f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{

			m_bDisableGravity = false;



			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AHM_BLIZZARDSHOWER );
			if( NULL != pSkillTemplet )
			{
				const float MAGIC_MAX_RADIUS = 1000.f; // 10 meter
				const float MAGIC_MIN_RADIUS = 100.f;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
			#ifdef ADD_MEMO_1ST_CLASS //김창한
				int iPlusFireCount = 0;
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO19) == true )
					iPlusFireCount = 5;

				for( int i=0; i< (pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel ) + iPlusFireCount); i++ )
			#else //ADD_MEMO_1ST_CLASS
				for( int i=0; i< pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel ); i++ )
			#endif //ADD_MEMO_1ST_CLASS
	#else // UPGRADE_SKILL_SYSTEM_2013
				for( int i=0; i< pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT ); i++ )
	#endif // UPGRADE_SKILL_SYSTEM_2013
				{
					float fRadius = (MAGIC_MAX_RADIUS-MAGIC_MIN_RADIUS) * (float) GetRandomInt( i*4 + 0 ) / 100.f + MAGIC_MIN_RADIUS;
					D3DXVECTOR3 vDirVector = GetDirVector() * (float) ( GetRandomInt( i*4 + 1 ) - 50 ) + D3DXVECTOR3(0, 1, 0 ) * (float) ( GetRandomInt( i*4 + 2 ) - 50 );
					D3DXVec3Normalize( &vDirVector, &vDirVector );

					D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
					D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0, 850, 0 );
					const float MAGIC_TIME_RANGE = 1.f;


					D3DXVECTOR3 vRotation = GetRotateDegree();
					vRotation.z -= 45.f;
					if( true == GetIsRight() )
					{
						vEffectStartPosition -= GetDirVector() * 750;
					}
					else
					{
						vEffectStartPosition += GetDirVector() * 750;
					}

					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_BLIZZARD_SHOWER", GetPowerRate(), vEffectStartPosition, vRotation, vRotation, 
						vEffectEndPosition.y, true, GetRandomInt( i*4 + 3 ) / 100.f * 0.8f );
				}
			}
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_EventProcess()
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

	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_StateEnd()
	{
		m_bDisableGravity = false;
		CommonStateEnd();
	}


	
	
	
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_StateStart()
	{
		CommonStateStart();

		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
		m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_BlizzardShowerNoDamage", this ); 

#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 블리자드 샤워 발사 전 주위를 얼리는 공격 추가
			D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_BLIZZARD_SHOWER_FROZEN_ATTACK", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(),  GetLandPos().y );
#endif BALANCE_ELEMENTAL_MASTER_20130117
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.93f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.93f ) == true && EventCheck( 0.93f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_bDisableGravity = false;


			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AHM_BLIZZARDSHOWER );
			if( NULL != pSkillTemplet )
			{
				const float MAGIC_MAX_RADIUS = 1000.f; // 10 meter
				const float MAGIC_MIN_RADIUS = 100.f;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
		
		#ifdef ADD_MEMO_1ST_CLASS //김창한
				int iPlusFireCount = 0;
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO19) == true )
					iPlusFireCount = 5;

				for( int i=0; i< (pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel ) + iPlusFireCount); i++ )
		#else //ADD_MEMO_1ST_CLASS
				for( int i=0; i< pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT, iSkillTempletLevel ); i++ )
		#endif //ADD_MEMO_1ST_CLASS
	#else // UPGRADE_SKILL_SYSTEM_2013
				for( int i=0; i< pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT ); i++ )
	#endif // UPGRADE_SKILL_SYSTEM_2013
				{
					float fRadius = (MAGIC_MAX_RADIUS-MAGIC_MIN_RADIUS) * (float) GetRandomInt( i*4 + 0 ) / 100.f + MAGIC_MIN_RADIUS;
					D3DXVECTOR3 vDirVector = GetDirVector() * (float) ( GetRandomInt( i*4 + 1 ) - 50 ) + D3DXVECTOR3(0, 1, 0 ) * (float) ( GetRandomInt( i*4 + 2 ) - 50 );
					D3DXVec3Normalize( &vDirVector, &vDirVector );

					D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
					D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0, 850, 0 );
					const float MAGIC_TIME_RANGE = 1.f;


					D3DXVECTOR3 vRotation = GetRotateDegree();
					vRotation.z -= 45.f;
					if( true == GetIsRight() )
					{
						vEffectStartPosition -= GetDirVector() * 750;
					}
					else
					{
						vEffectStartPosition += GetDirVector() * 750;
					}

					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_BLIZZARD_SHOWER", GetPowerRate(), vEffectStartPosition, vRotation, vRotation, 
						vEffectEndPosition.y, true, GetRandomInt( i*4 + 3 ) / 100.f * 0.8f );
				}
			}
		}


		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_EventProcess()
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



	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLIZZARDSHOWER_HYPER_StateEnd()
	{
		m_bDisableGravity = false;
		CommonStateEnd();
	}



	
	
	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_Init()
	{
		TextureReadyInBackground( L"Skull_Death_Field01.dds" );
		
		XSkinMeshReadyInBackground( L"Arme_SI_SA_DeathNote_Mesh01.X" );
		XSkinMeshReadyInBackground( L"Arme_SI_SA_DeathNote_Mesh02.X" );
	}
	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_StateStart()
	{
		CommonStateStart();

		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
		m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

	}
	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 pos = GetPos();
  		
			for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
			{
				CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
				if( pUnit->GetTeam() == GetTeam() )
					continue;

				if( pUnit->GetInvincible() == true )
					continue;

#ifdef ADD_MEMO_1ST_CLASS //김창한
				float MAGIC_RANGE = 160000.f;
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO20) == true )
					MAGIC_RANGE *= 1.3f;
#else //ADD_MEMO_1ST_CLASS
				const float MAGIC_RANGE = 160000.f;
#endif //ADD_MEMO_1ST_CLASS

				if( GetDistance3Sq( pos, pUnit->GetPos() ) < MAGIC_RANGE )
				{
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_DeathField_FirstImpact", this, pUnit, false, GetPowerRate() );

					pUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_DEBUFF_DEATH_SENTENCE ) );						

					//{BFI_DEBUFF_DEATH_SENTENCE의 지속시간 얻어오기 
					float fDeathSentenceDurationTime = 0.f;

					vector<CX2BuffFinalizerTempletPtr>	vecFinalizerPtr;
					pUnit->GetBuffFinalizerTempletPtrList(BTI_DEBUFF_DEATH_SENTENCE, vecFinalizerPtr);

					if( !vecFinalizerPtr.empty())
					{
						vector<KBuffFinalizerFactor> vecFactors;
						vecFinalizerPtr[0]->GetFactor(vecFactors, this);

						if( !vecFactors.empty() && !vecFactors[0].m_vecValues.empty())
							fDeathSentenceDurationTime = vecFactors[0].m_vecValues[0];
					}
					//}BFI_DEBUFF_DEATH_SENTENCE의 지속시간 얻어오기 

					CX2EffectSet::Handle hEffectBomb = 	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_DeathFieldBomb", this, pUnit, true, GetPowerRate() );

					if( INVALID_EFFECTSET_HANDLE != hEffectBomb )
					{
						if ( CX2EffectSet::EffectSetInstance* pBomb = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectBomb ) )
							pBomb->m_fDelayTime = fDeathSentenceDurationTime;
					}
				}
			}
		}


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_bDisableGravity = false;
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_EventProcess()
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

	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_StateEnd()
	{
		m_bDisableGravity = false;
		CommonStateEnd();
	}


	
	
	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_HYPER_StateStart()
	{
		CommonStateStart();
		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
		m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

	}


	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_HYPER_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.06f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.8f ) == true && EventCheck( 0.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 pos = GetPos();
     		
			for( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
			{
				CX2GameUnit* pUnit = g_pX2Game->GetUnit( i );
				if( pUnit->GetTeam() == GetTeam() )
					continue;

				if( pUnit->GetInvincible() == true )
					continue;

#ifdef ADD_MEMO_1ST_CLASS //김창한
				float MAGIC_RANGE = 160000.f;
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO20) == true )
					MAGIC_RANGE *= 1.3f;
#else //ADD_MEMO_1ST_CLASS
				const float MAGIC_RANGE = 160000.f;
#endif //ADD_MEMO_1ST_CLASS
				if( GetDistance3Sq( pos, pUnit->GetPos() ) < MAGIC_RANGE )
				{
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_DeathField_FirstImpact", this, pUnit, true, GetPowerRate() );

					pUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_DEBUFF_DEATH_SENTENCE ) );

					//{BFI_DEBUFF_DEATH_SENTENCE의 지속시간 얻어오기 
					float fDeathSentenceDurationTime = 0.f;

					vector<CX2BuffFinalizerTempletPtr>	vecFinalizerPtr;
					pUnit->GetBuffFinalizerTempletPtrList(BTI_DEBUFF_DEATH_SENTENCE, vecFinalizerPtr);

					if( !vecFinalizerPtr.empty())
					{
						vector<KBuffFinalizerFactor> vecFactors;
						vecFinalizerPtr[0]->GetFactor(vecFactors, this);
						  
						if( !vecFactors.empty() && !vecFactors[0].m_vecValues.empty())
							fDeathSentenceDurationTime = vecFactors[0].m_vecValues[0];
					}
					//}BFI_DEBUFF_DEATH_SENTENCE의 지속시간 얻어오기 

					CX2EffectSet::Handle hEffectBomb = 	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_DeathFieldBomb", this, pUnit, true, GetPowerRate() );

					if( INVALID_EFFECTSET_HANDLE != hEffectBomb )
					{
						if ( CX2EffectSet::EffectSetInstance* pBomb = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectBomb ) )
							pBomb->m_fDelayTime = fDeathSentenceDurationTime;
					}		
				}
			}
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 1.4f ) == true && EventCheck( 1.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_bDisableGravity = false;
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_HYPER_EventProcess()
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

	void CX2GUArme_VioletMage::AVSI_SA_ADM_DEATHFIELD_HYPER_StateEnd()
	{
		m_bDisableGravity = false;
		CommonStateEnd();
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_Init()
	{
		TextureReadyInBackground( L"AeroTornado04.dds" );
		TextureReadyInBackground( L"Explosion_Sphere.dds" );
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_StateStart()
	{
		CommonStateStart();
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			float fPowerRate = 1.f;
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet(CX2SkillTree::SI_A_ADM_MANA_INTAKE);
			if( NULL != pSkillTemplet )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				fPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				fPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARME_MANA_INTAKE", fPowerRate, GetBonePos(L"Bip01"), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
				PlaySound( L"Aisha_Mana_Intake.ogg" );
			}
		}




		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_EventProcess()
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
			if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.68f )
			{
				StateChange( USI_WAIT );
			}
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_HYPER_Init()
	{
		TextureReadyInBackground( L"AeroTornado04.dds" );
		TextureReadyInBackground( L"Explosion_Sphere.dds" );
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_HYPER_StateStart()
	{
		CommonStateStart();		
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_HYPER_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			float fPowerRate = 1.f;			
			
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet(CX2SkillTree::SI_A_ADM_MANA_INTAKE);
			if( NULL != pSkillTemplet )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				fPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				fPowerRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARME_MANA_INTAKE", fPowerRate, GetBonePos(L"Bip01"), GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
				PlaySound( L"Aisha_Mana_Intake.ogg" );
			}			
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_A_ADM_MANA_INTAKE_HYPER_EventProcess()
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
			if( m_pXSkinAnimFuture->GetNowAnimationTime() > 0.68f )
			{
				StateChange( USI_WAIT );
			}
		}

		CommonEventProcess();
	}

#endif SKILL_30_TEST

#ifdef SKILL_CASH_10_TEST

	// SI_SA_AHM_BLAZE_STEP
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLAZE_STEP_Init()
	{
		TextureReadyInBackground( L"Fire_Flower01.dds" );
		TextureReadyInBackground( L"smoke02.dds" );
		TextureReadyInBackground( L"Condense_Pulse02.dds" );
		TextureReadyInBackground( L"SpreadLight02_Gray.dds" );
		TextureReadyInBackground( L"GroundShockwave02.dds" );
	}
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLAZE_STEP_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.53f ) == true && EventCheck( 1.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{


#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AHM_BLAZE_STEP );
			if ( NULL != pSkillTemplet )
			{
#ifdef ADD_MEMO_1ST_CLASS //김창한
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO18 ) == true )
					SetBuffFactorToGameUnit( pSkillTemplet, 1 );
				else
#endif //ADD_MEMO_1ST_CLASS
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
			}
			
			if ( NULL != GetUnit() )
			{				
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
			}
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			m_sBlazeStepData.Begin();


			
			CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DE_ARME_BLAZE_STEP_02", GetPowerRate(), 
				GetBonePos(L"Bip01_Footsteps"), GetRotateDegree(), GetRotateDegree() );

			if( NULL != pEffect )
			{
				m_sBlazeStepData.m_ahMajorMeshInstance = pEffect->GetMainEffectHandle();
			}

			CKTDGParticleSystem::CParticleEventSequence* pSeq =
				g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Fire_Arme_Blaze_Step01_UnderStep", GetBonePos(L"Bip01_Footsteps") );
			if( NULL != pSeq )
			{
				m_sBlazeStepData.m_ahMajorParticleInstance = pSeq->GetHandle();
			}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		}

		CommonFrameMove();
	}
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLAZE_STEP_EventProcess()
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
	// SI_SA_AHM_BLAZE_STEP HYPER
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLAZE_STEP_HYPER_Init()
	{
		TextureReadyInBackground( L"Fire_Flower01.dds" );
		TextureReadyInBackground( L"smoke02.dds" );
		TextureReadyInBackground( L"Condense_Pulse02.dds" );
		TextureReadyInBackground( L"SpreadLight02_Gray.dds" );
		TextureReadyInBackground( L"GroundShockwave02.dds" );
	}
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLAZE_STEP_HYPER_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.04f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.53f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.53f ) == true && EventCheck( 1.53f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
	
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AHM_BLAZE_STEP );
			if ( NULL != pSkillTemplet )
			{
#ifdef ADD_MEMO_1ST_CLASS //김창한
				if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO18 ) == true )
					SetBuffFactorToGameUnit( pSkillTemplet, 1 );
				else
#endif //ADD_MEMO_1ST_CLASS
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
			}
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			m_sBlazeStepData.Begin();
			CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DE_ARME_BLAZE_STEP_02", GetPowerRate(), 
				GetBonePos(L"Bip01_Footsteps"), GetRotateDegree(), GetRotateDegree() );

			if( NULL != pEffect )
			{
				m_sBlazeStepData.m_ahMajorMeshInstance = pEffect->GetMainEffectHandle();
			}

			CKTDGParticleSystem::CParticleEventSequence* pSeq =
				g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Fire_Arme_Blaze_Step01_UnderStep", GetBonePos(L"Bip01_Footsteps") );
			if( NULL != pSeq )
			{
				m_sBlazeStepData.m_ahMajorParticleInstance = pSeq->GetHandle();
			}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

		}

		CommonFrameMove();
	}
	void CX2GUArme_VioletMage::AVSI_SA_AHM_BLAZE_STEP_HYPER_EventProcess()
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

	void CX2GUArme_VioletMage::AVSI_SA_ADM_AGING_Init()
	{
		XSkinMeshReadyInBackground( L"Arme_Aging_UpLight_Mesh02.X" );
		XSkinMeshReadyInBackground( L"Effect_SI_SA_ADM_AGING_BALL01.X" );
		XSkinMeshReadyInBackground( L"Effect_SI_SA_ADM_AGING_BALL.X" );
		XMeshReadyInBackground( L"Arme_Aging_UnderPlane_Mesh01.Y" );
		XMeshReadyInBackground( L"Arme_Aging_UpLight_Mesh01.Y" );
		TextureReadyInBackground( L"Gate_Light02_Gray.dds" );
	}

	void CX2GUArme_VioletMage::AVSI_SA_ADM_AGING_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.06f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.366f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.366f ) == true && EventCheck( 1.366f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vBonePos = GetWeaponBonePos( 0, L"Center_Jewel", 0 );
			m_hSeqLight_Arme_Aging1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Ring_Arme_SI_SA_Aging01", vBonePos );
			m_hSeqLight_Arme_Aging2 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Ring_Arme_SI_SA_Aging02", vBonePos );			
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.37f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.37f ) == true && EventCheck( 1.37f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vBonePos = GetWeaponBonePos( 0, L"Center_Jewel", 0 );
			m_hSeqLight_Arme_Aging3 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"Impact_Arme_SI_SA_Aging01", vBonePos );
		}		

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 0.266f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if ( m_pXSkinAnim->EventTimer( 0.266f ) == true && EventCheck( 0.266f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vBonePos = GetBonePos( L"Bip01" );

			if ( GetIsRight() )
				vBonePos.x += 20;
			else
				vBonePos.x -= 20;

			vBonePos.y += 200;

			CX2DamageEffect::CEffect* pDamageEffect 
				= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ADM_AGING_THROW", GetPowerRate(),
				vBonePos, GetRotateDegree() , GetRotateDegree() ,m_FrameDataNow.unitCondition.landPosition.y  );

			if ( NULL != pDamageEffect )
			{
#ifdef ADD_MEMO_1ST_CLASS //김창한
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO22) == true )
				{
					CX2DamageEffect::DamageEffectDataInLua& DamageEffectData = pDamageEffect->GetDamageEffectDataInLua();
					if( 0 < DamageEffectData.m_vecDieDamageEffect.size() )
					{
						DamageEffectData.m_vecDieDamageEffect[0]->damageEffectName = L"ADM_AGING_THROW_BALL_BROKEN_MEMO";
					}
				}
#endif //ADD_MEMO_1ST_CLASS

				const CX2SkillTree::SkillTemplet* pSkillTemplet 
					= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ADM_AGING );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
				{
					if ( NULL == GetUnit() )
						return;

					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

					pDamageEffect->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
				}
#else //UPGRADE_SKILL_SYSTEM_2013
				if ( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					pDamageEffect->GetDamageData().PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
			}			
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_ADM_AGING_EventProcess()
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

#ifndef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	// 블레이즈 스탭 구조체는 더 이상 사용되지 않습니다. ( 버프로 변경 )
	void CX2GUArme_VioletMage::BLAZE_STEP_DATA::DestroyEffect()
	{
		if(m_ahMajorMeshInstance != INVALID_MESH_INSTANCE_HANDLE)
			g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle(m_ahMajorMeshInstance);

		if(m_ahMajorParticleInstance != INVALID_PARTICLE_SEQUENCE_HANDLE)
			g_pX2Game->GetMajorParticle()->DestroyInstanceHandle(m_ahMajorParticleInstance);
	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	

#endif SKILL_CASH_10_TEST






#ifdef AISHA_SECOND_CLASS_CHANGE


	void CX2GUArme_VioletMage::AVSI_A_AEM_STONE_WALL_Init()
	{
		XSkinMeshReadyInBackground( L"EM_Stone_Wall_Effect.x" );
		XSkinMeshReadyInBackground( L"EM_Stone_Wall_Piece.X" );
		XSkinMeshReadyInBackground( L"EM_Stone_Wall_Bottom.X" );


		TextureReadyInBackground( L"AEM_Effect01.tga" );
		TextureReadyInBackground( L"AEM_Effect02.tga" );
		TextureReadyInBackground( L"AEM_Stone_Wall_Bottom.tga" );
	}


	void CX2GUArme_VioletMage::AVSI_A_AEM_STONE_WALL_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			// 새로운 몬스터 소환
			if( true == g_pX2Game->IsHost() )
			{
				D3DXVECTOR3 vPos = GetPos();
				if( true == GetIsRight() )
				{
					vPos += GetDirVector() * 200.f;
				}
				else
				{
					vPos -= GetDirVector() * 200.f;
				}

				int iHyperMode = 0; // 각성 아님, 1이면 각성임
#ifdef FIX_HELL_DROP_DAMAGE
				if ( 0.f < GetRemainHyperModeTime() )
					iHyperMode = 1;
#endif	//FIX_HELL_DROP_DAMAGE
				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_STONE_WALL_AISHA, iHyperMode, true, vPos, 
					GetIsRight(), 0.f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );

				vPos.y += 20.f;
				vPos += GetZVector() * -50.f; 
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Stone_Wall_Summon_Smoke", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), true, vPos, GetRotateDegree(), GetDirVector() );
			}
		}

		CommonFrameMove();
	}



	void CX2GUArme_VioletMage::AVSI_A_AVP_SUMMON_BAT_Init()
	{
		XSkinMeshReadyInBackground( L"AVP_SUMMON_BAT_MESH_EFFECT.X" );
	}


	void CX2GUArme_VioletMage::AVSI_A_AVP_SUMMON_BAT_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
		
			D3DXVECTOR3 vPos = GetPos();
			if( true == GetIsRight() )
			{
				vPos += GetDirVector() * 100.f;
			}
			else
			{
				vPos -= GetDirVector() * 100.f;
			}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AVP_SUMMON_BAT_TIME_BOMB );
#else //UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AVP_SUMMON_BAT );
#endif //UPGRADE_SKILL_SYSTEM_2013
			if( NULL != pSkillTemplet )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				float fLifeTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				float fLifeTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_SUMMON_BAT_FLY", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), GetLandPos().y, false, -1.f, 
					1.f, fLifeTime );
			}



			vPos.y += 150.f;
			vPos += GetZVector() * -50.f; 
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Summon_Smoke", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), true, vPos, GetRotateDegree(), GetDirVector() );

		}

		CommonFrameMove();
	}




	void CX2GUArme_VioletMage::AVSI_SA_AEM_CYCLONE_Init()
	{
		XSkinMeshReadyInBackground( L"EM_Cyclone_Effect.x" );
		TextureReadyInBackground( L"EM_Cyclone.tga" );
	}

	void CX2GUArme_VioletMage::AVSI_SA_AEM_CYCLONE_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.04f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.733f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.733f ) == true && EventCheck( 0.733f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AHM_CYCLONE );
#else //UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AEM_CYCLONE );
#endif //UPGRADE_SKILL_SYSTEM_2013
			if( NULL != pSkillTemplet )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				float fLifeTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
				int iHitCount = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				float fLifeTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
				int iHitCount = (int)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_COUNT );
	#endif // UPGRADE_SKILL_SYSTEM_2013


				D3DXVECTOR3 vPos = GetPos();
				if( true == GetIsRight() )
				{
					vPos += GetDirVector() * 150.f;
				}
				else
				{
					vPos -= GetDirVector() * 150.f;
				}


#ifdef ADD_MEMO_1ST_CLASS //김창한
				CX2DamageEffect::CEffect* pEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_ELEMENTAL_MASTER_CYCLONE_OUTSIDE", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), GetLandPos().y, false, -1.f, 
					1.f, fLifeTime, iHitCount );

				if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO16) == true && pEffect != NULL )
				{
					pEffect->GetDamageData().fHitAddMP *= 1.25f;
				}
#else //ADD_MEMO_1ST_CLASS
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_ELEMENTAL_MASTER_CYCLONE_OUTSIDE", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), GetLandPos().y, false, -1.f, 
					1.f, fLifeTime, iHitCount );
#endif //ADD_MEMO_1ST_CLASS

			}
		}


		CommonFrameMove();
	}
	

	//void CX2GUArme_VioletMage::AVSI_SA_AEM_CYCLONE_EventProcess()
	//{
	//	

	//}



	void CX2GUArme_VioletMage::AVSI_SA_AVP_HELL_DROP_Init()
	{

		XSkinMeshReadyInBackground( L"AVP_Hell_Drop_Body.X" );
		XSkinMeshReadyInBackground( L"AVP_Hell_Drop_Light.x" );
		XSkinMeshReadyInBackground( L"AVP_Hell_Drop_Ring.x" );
		XSkinMeshReadyInBackground( L"AVP_Hell_Drop_Stone.x" );
		XSkinMeshReadyInBackground( L"Arme_SI_SA_Hell_Drop_Broken_Mesh01.x" );

		XMeshReadyInBackground( L"Split_Light01.Y" );

		TextureReadyInBackground( L"VP_Hell_Drop_Light.tga" );
		TextureReadyInBackground( L"VP_Hell_Drop_Ring.tga" );
		TextureReadyInBackground( L"VP_Hell_Drop_Stone.tga" );


	}



	void CX2GUArme_VioletMage::AVSI_SA_AVP_HELL_DROP_Start()
	{
		CommonStateStart();
		m_bIsSummoningUFO = true;
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		m_fHellDropCreateEffectCooltime = _CONST_AISHA_::CREATE_HELL_DROP_DAMAGE_EFFECT_START_TIME;		// 헬 드롭 생성 쿨타임, 1초부터 시작
		m_bIsHellDropTimerStart = false;		// 헬 드롭 타이머가 켜졌는가?
#endif // BALANCE_PATCH_20131107				// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}





	void CX2GUArme_VioletMage::AVSI_SA_AVP_HELL_DROP_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.04f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.45f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.45f ) == true && EventCheck( 0.45f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vPos = GetPos();
			if( true == GetIsRight() )
			{
				vPos += GetDirVector() * 200.f;
			}
			else
			{
				vPos -= GetDirVector() * 200.f;
			}
			vPos.y += 240.f;

			ResetUFO();

			if( true == g_pX2Game->IsHost() )
			{
				int iNewLineIndex = m_FrameDataNow.syncData.lastTouchLineIndex;

				D3DXVECTOR3 vLandPos = vPos;
				if( false == g_pX2Game->GetWorld()->GetLineMap()->CanDown( vPos, LINE_RADIUS, &vLandPos, &iNewLineIndex ) )
				{
					vPos = GetPos();
					vPos.y += 240.f;
					iNewLineIndex = m_FrameDataNow.syncData.lastTouchLineIndex;
				}
	
				int iHyperMode = 0; // 각성 아님, 1이면 각성임
#ifdef FIX_HELL_DROP_DAMAGE
				if ( 0.f < GetRemainHyperModeTime() )
					iHyperMode = 1;
#endif	//FIX_HELL_DROP_DAMAGE
#ifdef ADD_MEMO_1ST_CLASS //김창한
				if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO21) == true )
					g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_UFO_AISHA_DROPPER_MEMO, iHyperMode, true, vPos, 
					GetIsRight(), 0.3f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
				else
#endif //ADD_MEMO_1ST_CLASS
				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_UFO_AISHA, iHyperMode, true, vPos, 
					GetIsRight(), 0.3f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
			}


			vPos += GetZVector() * -50.f; 
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Summon_Smoke", this, NULL, false, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), true, vPos, GetRotateDegree(), GetDirVector() );

		}


		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_HELL_DROP_EventProcess()
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( true == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) )
		{
			if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
			{	
				m_TimerUFOControl.restart();
				if( false == IsOnSomethingFuture() )
					StateChange( USI_JUMP_DOWN );
				else
					StateChange( USI_WAIT );
			}
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		{
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			const float MAGIC_TIME = 3.f;		// 지속 시간 변경 5초 -> 3초
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			const float MAGIC_TIME = 5.f;
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

			if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
			{
				if( NULL != GetUFO() )
				{
					StateChange( AVSI_HELL_DROP_CONTROL_IDLE );
#ifndef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					m_TimerUFOControl.restart();
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				}
				else if( m_FrameDataFuture.unitCondition.fStateTime > MAGIC_TIME ) // time out 되면 wait 상태로 일단 돌아가자
				{
					StateChange( USI_WAIT );
				}
			}


			m_InputData.Init(); // 캐릭터는 조작 할 수 없고 UFO만 조작할 수 있다
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_HELL_DROP_StateEnd()
	{
		m_bIsSummoningUFO = false;
		CommonStateEnd();
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		m_TimerUFOControl.restart();			// 헬 드롭 타이머를 시작한다.
		m_bIsHellDropTimerStart = true;			// 헬 드롭 타이머가 켜졌는가?
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	}


#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_FrameMove()
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) )
#endif //ADD_MEMO_1ST_CLASS
		{
			// 스킬 키를 누른 상태에서, 1초가 지났고 ( 생성 선 쿨타임 ), 생성 한 지 0.25초가 지났다면
			// 헬 드롭을 발사한다.
			if ( m_TimerUFOControl.elapsed() > m_fHellDropCreateEffectCooltime )
			{
				m_fHellDropCreateEffectCooltime = m_fHellDropCreateEffectCooltime + _CONST_AISHA_::CREATE_HELL_DROP_DAMAGE_EFFECT_TIME_GAP;
				// StateChange( AVSI_HELL_DROP_CONTROL_FIRE );
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Hell_Drop_Stone", GetUFO(), NULL, IsHyperState(), GetPowerRate() );
			}
		}
		CommonFrameMove();
	}

#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_EventProcess()
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) )
#endif //ADD_MEMO_1ST_CLASS
		{
			bool bSpecialAttackKeyPressed = true;
			switch( m_eSpecialAttackKeyPressed )
			{
			case SAKP_A:
				{
					if( m_InputData.pureA == false )
					{
						bSpecialAttackKeyPressed = false;
					}
				} break;
		
			case SAKP_S:
				{
					if( m_InputData.pureS == false )
					{
						bSpecialAttackKeyPressed = false;
					}
				} break;
		
			case SAKP_D:
				{
					if( m_InputData.pureD == false )
					{
						bSpecialAttackKeyPressed = false;
					}
				} break;
		
			case SAKP_C:
				{
					if( m_InputData.pureC == false )
					{
						bSpecialAttackKeyPressed = false;
						TRACE( L"HIT C\n" );
					}
				} break;
			case SAKP_Q:
				{
					if( m_InputData.pureQ == false )
					{
						bSpecialAttackKeyPressed = false;
					}
				} break;
		
			case SAKP_W:
				{
					if( m_InputData.pureW == false )
					{
						bSpecialAttackKeyPressed = false;
					}
				} break;
		
			case SAKP_E:
				{
					if( m_InputData.pureE == false )
					{
						bSpecialAttackKeyPressed = false;
					}
				} break;
		
			case SAKP_R:
				{
					if( m_InputData.pureR == false )
					{
						bSpecialAttackKeyPressed = false;
					}
				} break;
			default:
				{
					bSpecialAttackKeyPressed = false;
				} break;
			}
		
		
			const float MAGIC_CONTROL_RADIUS = 1500.f;
	#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			const double MAGIC_CONTROL_TIME = 2.9;	// 지속 시간 변경 5초 -> 3.1초
	#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			const double MAGIC_CONTROL_TIME = 5.0;
	#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		
			{
				if( ( true == bSpecialAttackKeyPressed &&
					m_TimerUFOControl.elapsed() < MAGIC_CONTROL_TIME ) 
	#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					|| m_TimerUFOControl.elapsed() < _CONST_AISHA_::CREATE_HELL_DROP_DAMAGE_EFFECT_START_TIME + 0.1f 
	#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					)
				{
					D3DXVECTOR3 vDirection(0,0,0);
					if( NULL != GetUFO() )
					{
						if( GetDistance( GetUFO()->GetPos(), GetPos() ) > MAGIC_CONTROL_RADIUS )
						{
							vDirection = GetUFO()->GetPos() - GetPos(); 
						}
					}
					if( vDirection.x >= 0.f && vDirection.y <= 0.f && 
						true == m_InputData.pureUp && true == m_InputData.pureLeft )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_UP_LEFT )
							StateChange( AVSI_HELL_DROP_CONTROL_UP_LEFT );
					}
					else if( vDirection.x <= 0.f && vDirection.y <= 0.f && 
						true == m_InputData.pureUp && true == m_InputData.pureRight )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_UP_RIGHT )
							StateChange( AVSI_HELL_DROP_CONTROL_UP_RIGHT );
					}
					else if( vDirection.x >= 0.f && vDirection.y >= 0.f &&  
						true == m_InputData.pureDown && true == m_InputData.pureLeft )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_DOWN_LEFT )
							StateChange( AVSI_HELL_DROP_CONTROL_DOWN_LEFT );
					}
					else if( vDirection.x <= 0.f && vDirection.y >= 0.f &&  
						true == m_InputData.pureDown && true == m_InputData.pureRight )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_DOWN_RIGHT )
							StateChange( AVSI_HELL_DROP_CONTROL_DOWN_RIGHT );
					}
					else if( vDirection.x >= 0.f &&
						true == m_InputData.pureLeft )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_LEFT )
							StateChange( AVSI_HELL_DROP_CONTROL_LEFT );
					}
					else if( vDirection.x <= 0.f &&
						true == m_InputData.pureRight )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_RIGHT )
							StateChange( AVSI_HELL_DROP_CONTROL_RIGHT );
					}
					else if( vDirection.y <= 0.f &&
						true == m_InputData.pureUp )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_UP )
							StateChange( AVSI_HELL_DROP_CONTROL_UP );
					}
					else if( vDirection.y >= 0.f &&
						true == m_InputData.pureDown )
					{
						if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_DOWN )
							StateChange( AVSI_HELL_DROP_CONTROL_DOWN );
					}
					else if( GetNowStateID() != AVSI_HELL_DROP_CONTROL_IDLE )
					{
						StateChange( AVSI_HELL_DROP_CONTROL_IDLE );
					}
				}
				else
				{
		#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					// 기존 Fire 이후 사망 -> 일정 시간 혹은 조건에 따라 사망으로 변경
					// 조건, 헬 드롭 타이머가 켜져있는가?
					if ( true == m_bIsHellDropTimerStart )
					{	
						TRACE( L"AVSI_HELL_DROP_CONTROL_DYING\n" );
						StateChange( AVSI_HELL_DROP_CONTROL_DYING, true );
					}
		#else  BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
					TRACE( L"AVSI_HELL_DROP_CONTROL_FIRE\n" );
					StateChange( AVSI_HELL_DROP_CONTROL_FIRE, true );
		#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				}
			}
			m_InputData.Init(); // 캐릭터는 조작 할 수 없고 UFO만 조작할 수 있다
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_IDLE_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_IDLE.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_LEFT_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_LEFT.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_RIGHT_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_RIGHT.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_UP_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_UP.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_DOWN_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_DOWN.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_UP_LEFT_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_UP_LEFT.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_UP_RIGHT_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_UP_RIGHT.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_DOWN_LEFT_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_DOWN_LEFT.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_DOWN_RIGHT_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			GetUFO()->StateChange( _CONST_AISHA_::UFO_GO_DOWN_RIGHT.c_str(), true );
		}
	}

	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_FIRE_StateStart()
	{
		CommonStateStart();
		if( 
#ifdef ADD_MEMO_1ST_CLASS //김창한
			false == GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO21 ) &&
#endif //ADD_MEMO_1ST_CLASS
			NULL != GetUFO() )
		{
			TRACE( L"Fire StateStart\n" );
			GetUFO()->StateChange( _CONST_AISHA_::UFO_FIRE.c_str(), true );
#ifndef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
			// 기존 Fire 이후 사망 -> 일정 시간 혹은 조건에 따라 사망으로 변경
			ResetUFO();
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		}
	}


	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_FIRE_FrameMove()
	{
// 		if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
// 		{
// 			if( NULL != GetUFO() )
// 			{
// 				TRACE( L"Fire StateStart\n" );
// 				GetUFO()->StateChange( _CONST_AISHA_::UFO_FIRE.c_str(), true );
// 				ResetUFO();
// 			}
// 		}

		CommonFrameMove();
	}
#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	void CX2GUArme_VioletMage::AVSI_HELL_DROP_CONTROL_FIRE_EventProcess()
	{
		StateChange ( AVSI_HELL_DROP_CONTROL_IDLE );
		CommonEventProcess();
	}
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	//void CX2GUArme_VioletMage::AVSI_AEM_ZZupZ_EventProcess()
	//{
	//	CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );


	//	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	//	{	
	//		if( false == IsOnSomethingFuture() )
	//		{
	//			StateChange( AVSI_JUMP_DOWN );
	//			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	//		}
	//		else
	//			StateChange( AVSI_WAIT );
	//	}
	//	ELSE_IF_STATE_CHANGE_ON_( 0, 0.4f, 0.4f, m_InputData.oneZ == true, AVSI_COMBO_Z4up )
	//	ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.7f )
	//	{
	//		if( false == IsOnSomethingFuture() )
	//			StateChange( AVSI_JUMP_DOWN );
	//		else
	//			StateChange( AVSI_WAIT );
	//	}

	//	CommonEventProcess();
	//}


	void CX2GUArme_VioletMage::AVSI_AEM_ZZupZZ_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( FlushMp( 10.f ) == true )
			{
				CX2EffectSet::Handle hEffSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_EM_ZZupZZ", this );
				
				m_vecEffectSetToDeleteOnDamageReact.push_back( hEffSet );
			}
			else
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

				CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
			}
		}


		CommonFrameMove();
	}




	void CX2GUArme_VioletMage::AVSI_AEM_ZZupZZ_EventProcess()
	{

		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			if( false == IsOnSomethingFuture() )
				StateChange( USI_JUMP_DOWN );
			else
				StateChange( USI_WAIT );
		}
		ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 1.4f )
		{
			if( false == IsOnSomethingFuture() )
				StateChange( USI_JUMP_DOWN );
			else
				StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}





	void CX2GUArme_VioletMage::AVSI_AEM_XXfrontX_Init()
	{
		XSkinMeshReadyInBackground( L"Arme_EM_Combo_X2F_Mesh01.X" );
		TextureReadyInBackground( L"Arme_Ice_Spiner01.tga" );


		XSkinMeshReadyInBackground( L"Arme_EM_Combo_Z4UP_Mesh01.X" );

	}





	void CX2GUArme_VioletMage::AVSI_AEM_XXfrontX_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( FlushMp( 10.f ) == true )
			{
				CX2EffectSet::Handle hEffSet = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_EM_XXfrontX", this );

				m_vecEffectSetToDeleteOnDamageReact.push_back( hEffSet );

			}
			else
			{
				D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
				if( true == GetIsRight() )
				{
					pos += GetDirVector() * 150.f;
				}
				else
				{
					pos -= GetDirVector() * 150.f;
				}

				CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
			}
		}


		CommonFrameMove();
	}


	void CX2GUArme_VioletMage::AVSI_AEM_XXfrontX_EventProcess()
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			StateChange( USI_WAIT );
		}
		else if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
		ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 1.33f )
		{
			if( false == IsOnSomethingFuture() )
				StateChange( USI_JUMP_DOWN );
			else
				StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}



	void CX2GUArme_VioletMage::AVSI_AEM_XZ_Init()
	{
		XSkinMeshReadyInBackground( L"Arme_EM_Combo_XZZ_Mesh01.X" );
		XSkinMeshReadyInBackground( L"Wind_Liner03.X" );

		TextureReadyInBackground( L"Arme_Lightning01.tga" );
	}


	void CX2GUArme_VioletMage::AVSI_AEM_XZ_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 엠피 소모율 감소
			if( FlushMp( 3.f ) == true )
#else  BALANCE_ELEMENTAL_MASTER_20130117
			if( FlushMp( 10.f ) == true )
#endif BALANCE_ELEMENTAL_MASTER_20130117
			{
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Elemental_Master_XZ", this );
			}
			else
			{
				D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
				//D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( "Bip01" );
				if( true == GetIsRight() )
				{
					pos += GetDirVector() * 120.f;
				}
				else
				{
					pos -= GetDirVector() * 120.f;
				}
				pos.y += 40.f;
				CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
			}
		}

		CommonFrameMove();
	}




	void CX2GUArme_VioletMage::AVSI_AEM_XZ_EventProcess()
	{
		CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );


		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			StateChange( USI_WAIT );
		}
		else if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
		ELSE_IF_STATE_CHANGE_ON_( 0, 0.89f, 0.89f, m_InputData.oneZ == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.54f, AVSI_AEM_XZZ )

		CommonEventProcess();
	}


	void CX2GUArme_VioletMage::AVSI_AEM_XZZ_FrameMove()
	{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 투명의 데미지 이펙트를 발사해서, 적에게 닿으면 번개 생성되는 구조로 변경 ( 닿지 않으면 Die Damage Effect로 발생 )
		/// 3타 공격
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimerOneshot( 0.333f ) == true ||
			m_pXSkinAnim->EventTimerOneshot( 1.133f ) == true ||
			m_pXSkinAnim->EventTimerOneshot( 1.933f ) == true )

#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( ( m_pXSkinAnim->EventTimer( 0.333f ) == true && EventCheck( 0.333f, false ) == true ) ||
			( m_pXSkinAnim->EventTimer( 1.133f ) == true && EventCheck( 1.133f, false ) == true ) ||
			( m_pXSkinAnim->EventTimer( 1.933f ) == true && EventCheck( 1.933f, false ) == true ) )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#else  BALANCE_ELEMENTAL_MASTER_20130117
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#endif BALANCE_ELEMENTAL_MASTER_20130117
		{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
			if( FlushMp( 3.f ) == true )	/// 마나 소비 감소
			{
				/// 투명의 데미지 이펙트를 발사해서, 적에게 닿으면 번개 생성되는 구조로 변경 ( 닿지 않으면 Die Damage Effect로 발생 )
				D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

				/// 기존의 EffectSet으로 구현되어 있던것을 Damage Effect로 변경 ( Die Damage Effect를 사용하기 위해서 )
				g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ELEMENTAL_MASTER_XXZ_INVISIBLE_ATTACKBOX", GetPowerRate(), GetPos(), GetRotateDegree(), GetRotateDegree(),  GetLandPos().y );
#else  BALANCE_ELEMENTAL_MASTER_20130117
			if( FlushMp( 15.f ) == true )
			{
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Elemental_Master_XZZ", this );
#endif BALANCE_ELEMENTAL_MASTER_20130117
			}
			else
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

				CreateNotEnoughMPEffect( pos, 0.f, 0.f, 0.f );
			}
		}

		CommonFrameMove();
	}




	void CX2GUArme_VioletMage::AVSI_AEM_XZZ_EventProcess()
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 종료 스테이트 추가
			StateChange( AVSI_AEM_XZZ_END );
#else  BALANCE_ELEMENTAL_MASTER_20130117	
			StateChange( USI_WAIT );
#endif BALANCE_ELEMENTAL_MASTER_20130117	
		}
		else if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 공격키 입력 캔슬 대신 스킬, 대시 캔슬 추가
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
#else  BALANCE_ELEMENTAL_MASTER_20130117
		ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.67f )
		{
			if( false == IsOnSomethingFuture() )
				StateChange( USI_JUMP_DOWN );
			else
				StateChange( USI_WAIT );
		}
#endif BALANCE_ELEMENTAL_MASTER_20130117

		CommonEventProcess();
	}

#ifdef BALANCE_ELEMENTAL_MASTER_20130117
	/// XZZ 콤보 종료 스테이트추가
	void CX2GUArme_VioletMage::AVSI_AEM_XZZ_END_EventProcess()
	{
		if( false == IsOnSomethingFuture() )
			StateChange( USI_JUMP_LANDING );
		else
			StateChange( USI_WAIT );

		CommonEventProcess();
	}
#endif BALANCE_ELEMENTAL_MASTER_20130117






	void CX2GUArme_VioletMage::AVSI_AVP_ZZfrontZ_EventProcess()
	{
		CANCEL_COMBO_ON_DIRECTION_CHANGE( 1 );


		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			StateChange( USI_WAIT );
		}
		else if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
		}
		ELSE_IF_STATE_CHANGE_ON_( 0, 0.8f, 0.8f, m_InputData.oneZ == true, AVSI_AVP_ZZfrontZZ )
		ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 0.94f )
		{
			if( false == IsOnSomethingFuture() )
				StateChange( USI_JUMP_DOWN );
			else
				StateChange( USI_WAIT );
		}

		CommonEventProcess();

	}


	void CX2GUArme_VioletMage::AVSI_AVP_ZZfrontZZ_Init()
	{

		XSkinMeshReadyInBackground( L"VP_ComboZ3_Effect.X" );
		XSkinMeshReadyInBackground( L"VP_ComboZ4_Effect.X" );


		TextureReadyInBackground( L"AVP_Effect128.tga" );
		TextureReadyInBackground( L"AVP_Phantom_Blade_Arc.tga" );
		TextureReadyInBackground( L"AVP_Phantom_Blade_Weapon.tga" );

	}



	void CX2GUArme_VioletMage::AVSI_AVP_ZZfrontZZ_StateStartFuture()
	{
		CommonStateStartFuture();

		m_FrameDataFuture.syncData.position.y += LINE_RADIUS * 1.5f;
	}



	void CX2GUArme_VioletMage::AVSI_AVP_ZZfrontZZ_EventProcess()
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			if( false == IsOnSomethingFuture() )
				StateChange( AVSI_AVP_ZZfrontZZ_FALL );
			else
				StateChange( USI_WAIT );
		}
		ELSE_IF_ARROW_ASDC_PRESSED_AFTER( 1.65f )
		{
			if( false == IsOnSomethingFuture() )
				StateChange( AVSI_AVP_ZZfrontZZ_FALL );
			else
				StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}




	void CX2GUArme_VioletMage::AVSI_AVP_ZZfrontZZ_FALL_EventProcess()
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_LANDING );
		}

		CommonEventProcess();
	}


	void CX2GUArme_VioletMage::AVSI_AVP_DASH_JUMP_X_Init()
	{
		TextureReadyInBackground( L"Black_FireBall01.dds" );
	}


	void CX2GUArme_VioletMage::AVSI_AVP_DASH_JUMP_X_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.16f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.16f ) == true && EventCheck( 0.16f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			//파볼쏘기
#ifdef SKILL_BALANCE_20110728
#ifdef BALANCE_PATCH_20120329
		if( FlushMp( 4.0f * m_fMPConsumeRateFireBall ) == true )
#else
		if( FlushMp( 7.0f * m_fMPConsumeRateFireBall ) == true )
#endif	//BALANCE_PATCH_20120329
#else
		if( FlushMp( 5.0f * m_fMPConsumeRateFireBall ) == true )
#endif	//SKILL_BALANCE_20110728
			{
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
				if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH )
#else
				if( GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN )
#endif //SERV_ARME_DIMENSION_WITCH
				{
					D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
					if( IsTransformed() )
					{
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARME_ACTIVE_MAGIC_BALL2", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
					}
					else
					{
						g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARME_ACTIVE_MAGIC_BALL1", GetPowerRate(), posR, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
					}					
				}
				else
#endif	//SERV_ADD_ARME_BATTLE_MAGICIAN
				{
					//파이어볼쏘기
					D3DXVECTOR3 posR = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
					D3DXVECTOR3 angle = GetRotateDegree();
					angle.z = -30.0f;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_GRAVITY_BALL", m_fGravityBallPower,
						posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#else //UPGRADE_SKILL_SYSTEM_2013
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_GRAVITY_BALL", GetPowerRate(), posR, angle, angle, m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, 1.f, m_fFireBallRangeRate  );
#endif //UPGRADE_SKILL_SYSTEM_2013
				}				

			}
			else
			{
				D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
				CreateNotEnoughMPEffect( pos, 0.f, 0.f, -30.f );

			}
		}

		CommonFrameMove();
	}



	void CX2GUArme_VioletMage::AVSI_AVP_DASH_JUMP_X_EventProcess()
	{
		if( true == IsOnSomethingFuture() )
		{
			StateChange( USI_DASH_JUMP_LANDING );
		}

		CommonEventProcess();
	}


	//{{ 김상훈 2010.11.1 : 메테오 샤워
#ifdef  NEW_SKILL_2010_11

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_Init()
	{
		XSkinMeshReadyInBackground( L"MeteoStone01.X" );
		XSkinMeshReadyInBackground( L"MeteoStone02.X" );
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_StateStart()
	{
		CommonStateStart();

		m_bIsOnSomethingStart =false;

//#ifdef STEP_ON_MONSTER_TEST
//		if (m_FrameDataNow.unitCondition.bFootOnLine == true || 
//			m_FrameDataNow.unitCondition.bFootOnUnit == true )
//#else STEP_ON_MONSTER_TEST
		if ( m_FrameDataNow.unitCondition.bFootOnLine == true )
//#endif STEP_ON_MONSTER_TEST
			m_bIsOnSomethingStart = true;

		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 500 );
		m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

#ifdef BALANCE_ELEMENTAL_MASTER_20130117
		m_iElapsedFireCount			= 0;		/// 메테오 샤워 누적 소환 개수 초기화
#endif BALANCE_ELEMENTAL_MASTER_20130117
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_METEOR_SHOWER_MOTION_EFFECT", this ); 
		}
#ifdef BALANCE_ELEMENTAL_MASTER_20130117	/// 정해진 최소 발사 횟수를 다 채울때 까지 발사

		if( m_iElapsedFireCount >= m_iMaxFireCount )	/// 최대 발사 횟수를 넘겼다면, 패스
			return;

		const int fCheckTime = static_cast<int>( m_FrameDataNow.unitCondition.fStateTime / m_fFireGap );

		if( fCheckTime >= m_iElapsedFireCount )
		{
			++m_iElapsedFireCount;	/// 발사 카운트 증가

			/// 전체 범위 : -850 ~ 850, 1그룹 : -850 ~ 340, 2그룹 : -340  ~ 850		-> 1, 2 그룹을 반복하며 중앙을 겹치는 공격 범위 연산
			float fRadius = static_cast<float>( ( GetRandomInt( m_iElapsedFireCount * 4 + 1 ) - 50 ) * 12 );

			if( 0 == m_iElapsedFireCount % 2 )
				fRadius += 250.f;
			else
				fRadius -= 250.f;

			if( true == GetIsRight() )		/// 방향에 따른 위치 보정
				fRadius += 500.f;
			else
				fRadius -= 500.f;

			D3DXVECTOR3 vDirVector = GetDirVector();

			D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
			D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0.f , 850.f, 0.f );
			const float MAGIC_TIME_RANGE = 1.f;

			D3DXVECTOR3 vRotation = GetRotateDegree();

			vRotation.z -= 45.f;
			if( true == GetIsRight() )
			{
				vEffectStartPosition -= GetDirVector() * 900;
			}
			else
			{
				vEffectStartPosition += GetDirVector() * 900;
			}

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_METEO_SHOWER01", GetPowerRate(), vEffectStartPosition, vRotation+D3DXVECTOR3(0,0,45), vRotation, 
				vEffectEndPosition.y );
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_METEO_SHOWERO2", GetPowerRate(), vEffectStartPosition, vRotation+D3DXVECTOR3(0,0,45), vRotation, 
				vEffectEndPosition.y );
		}

		CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
		CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

		if( pSeqArmeAirStepRight != NULL )
		{
			pSeqArmeAirStepRight->SetEmitRate(10,20); 
		}

		if( pSeqArmeAirStepLeft != NULL )
		{
			pSeqArmeAirStepLeft->SetEmitRate(10,20); 
		}
#else  BALANCE_ELEMENTAL_MASTER_20130117
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.93f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.93f ) == true && EventCheck( 0.93f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AEM_METEOR_SHOWER );
			if( NULL != pSkillTemplet )
			{
				//const float MAGIC_MAX_RADIUS = 700.f; // 10 meter
				//const float MAGIC_MIN_RADIUS = 100.f;


				for( int i=0; i< pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_FIRE_COUNT ); i++ )
				{
					float fRadius = (float)( GetRandomInt( i*4+1 ) - 50 ) * 17.0f;
					float fRandom = GetRandomInt( i*4 + 2 ) / 100.f * 0.8f;
					D3DXVECTOR3 vDirVector = GetDirVector();

					D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
					D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0.f , 770.0f + GetRandomInt( i*4+3 ) * 1.6f, 0.f );
					const float MAGIC_TIME_RANGE = 1.f;

					D3DXVECTOR3 vRotation = GetRotateDegree();

					vRotation.z -= 45.f;
					if( true == GetIsRight() )
					{
						vEffectStartPosition -= GetDirVector() * 900;
					}
					else
					{
						vEffectStartPosition += GetDirVector() * 900;
					}

					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_METEO_SHOWER01", GetPowerRate(), vEffectStartPosition, vRotation+D3DXVECTOR3(0,0,45), vRotation, 
						vEffectEndPosition.y, true, fRandom );
					g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_METEO_SHOWERO2", GetPowerRate(), vEffectStartPosition, vRotation+D3DXVECTOR3(0,0,45), vRotation, 
						vEffectEndPosition.y, true, fRandom );
				}
			}

		}
#endif BALANCE_ELEMENTAL_MASTER_20130117
			
		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_EventProcess()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
		}
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 최대 발사 횟수를 넘겼다면, 루프 스테이트로 이동
		else if ( m_iElapsedFireCount >= m_iMaxFireCount )
		{
			StateChange( AVSI_SA_AHM_METEOSHOWER_LOOP );
		}
#else  BALANCE_ELEMENTAL_MASTER_20130117
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if ( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if ( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_bDisableGravity = false;
		}

		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			if( false == IsOnSomethingFuture() )
				StateChange( USI_JUMP_DOWN );
			else if ( m_bIsOnSomethingStart )
				StateChange( USI_WAIT );
			else
				StateChange( USI_JUMP_LANDING );
		}
#endif BALANCE_ELEMENTAL_MASTER_20130117

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_StateEnd()
	{
		m_bDisableGravity = false;
		CommonStateEnd();
	}


#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 메테오 샤워 루프 스테이트추가
	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_LOOP_StateStart()
	{
		CommonStateStart();

		SetNoDetonation( true );			/// 기폭 제한 설정

		m_iElapsedFireCount			= 0;	///	발사 카운트 초기화
		m_bDisableGravity			= true;	/// 중력 해제
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
		m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_LOOP_FrameMove ()
	{
		ChangeWorldColorByHyperMode();

#ifdef FIXED_CHANGE_THE_LOOP_STATE_FORCE_WHEN_SHOCK_DEBUFF_HITED	// 메테오 샤워, 라이트닝 샤워 사용 중 쇼크 공격을 받았을 때 State 가 굳는 문제
		if ( m_ExtraDamagePack.m_EnchantShock.m_fTime > 0.0f ) 
		{
			m_ExtraDamagePack.m_EnchantShock.m_fTime = 0;			
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
#endif // FIXED_CHANGE_THE_LOOP_STATE_FORCE_WHEN_HITED // 메테오 샤워, 라이트닝 샤워 사용 중 쇼크 공격을 받았을 때 State 가 굳는 문제


		if( m_iElapsedFireCount >= m_iMaxFireCountByLoopState )	/// 최대 발사 횟수를 넘겼다면, 패스
			return;

		const int fCheckTime = static_cast<int>( m_FrameDataNow.unitCondition.fStateTime / m_fFireGap );

		if( fCheckTime >= m_iElapsedFireCount )
		{
			++m_iElapsedFireCount;				/// 루프 카운트 증가

			if( false == FlushMp( m_fUseMPConsume ) )
				return;

			/// 전체 범위 : -850 ~ 850, 1그룹 : -850 ~ 340, 2그룹 : -340  ~ 850		-> 1, 2 그룹을 반복하며 중앙을 겹치는 공격 범위 연산
			float fRadius = static_cast<float>( ( GetRandomInt( m_iElapsedFireCount * 4 + 1 ) - 50 ) * 12 );

			if( 0 == m_iElapsedFireCount % 2 )
				fRadius += 250.f;
			else
				fRadius -= 250.f;

			if( true == GetIsRight() )		/// 방향에 따른 위치 보정
				fRadius += 500.f;
			else
				fRadius -= 500.f;

			D3DXVECTOR3 vDirVector = GetDirVector();

			D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
			D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0.f , 850.f, 0.f );
			const float MAGIC_TIME_RANGE = 1.f;


			D3DXVECTOR3 vRotation = GetRotateDegree();

			vRotation.z -= 45.f;
			if( true == GetIsRight() )
			{
				vEffectStartPosition -= GetDirVector() * 900;
			}
			else
			{
				vEffectStartPosition += GetDirVector() * 900;
			}

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_METEO_SHOWER01", GetPowerRate(), vEffectStartPosition, vRotation+D3DXVECTOR3(0,0,45), vRotation, 
				vEffectEndPosition.y );
			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"AISHA_METEO_SHOWERO2", GetPowerRate(), vEffectStartPosition, vRotation+D3DXVECTOR3(0,0,45), vRotation, 
				vEffectEndPosition.y );
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_LOOP_EventProcess()
	{
		if ( m_fUseMPConsume > GetNowMp() ||						/// 현재 MP가 메테오 소환 비용보다 적거나
			 m_iElapsedFireCount >= m_iMaxFireCountByLoopState ||	/// 메테오 샤워를 소환하는데 사용한 총 누적 MP 소모수가 한계치를 넘거나
			 0.f >= GetNowHp() ||									/// 피가 없거나
			 ( m_InputData.pureA != true && m_InputData.pureS != true && m_InputData.pureD != true && m_InputData.pureC != true &&	/// 스킬 키를 누르고 있지 않다면 종료
			   m_InputData.pureQ != true && m_InputData.pureW != true && m_InputData.pureE != true && m_InputData.pureR != true ) )
		{
			StateChange( AVSI_SA_AHM_METEOSHOWER_END );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_LOOP_StateEnd()
	{
		m_bDisableGravity = false;		/// 중력 설정
		m_iElapsedFireCount = 0;		/// 발사 카운트 초기화

		CommonStateEnd();
		SetNoDetonation( false );		/// 기폭 제한 해제
	}
#endif BALANCE_ELEMENTAL_MASTER_20130117
	
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
	/// 메테오 샤워 종료 스테이트추가
	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_END_EventProcess()
	{
		if( true == IsOnSomethingFuture() )
			StateChange( USI_JUMP_LANDING );

		CommonEventProcess();
	}
	void CX2GUArme_VioletMage::AVSI_SA_AHM_METEOSHOWER_END_StateEnd()
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
		CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

		if( pSeqArmeAirStepRight != NULL )
		{
			pSeqArmeAirStepRight->SetEmitRate(0,0); 
		}

		if( pSeqArmeAirStepLeft != NULL )
		{
			pSeqArmeAirStepLeft->SetEmitRate(0,0); 
		}
	}
#endif BALANCE_ELEMENTAL_MASTER_20130117

#endif  NEW_SKILL_2010_11
	//}} 김상훈 2010.11.1 : 메테오 샤워


//{{ 김상훈 2010.11.1 : 팬텀브리딩
#ifdef NEW_SKILL_2010_11

	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_Init()
	{
		XSkinMeshReadyInBackground( L"AVP_PHANTOM_BREATHING_Body.X" );
		XSkinMeshReadyInBackground( L"AVP_PHANTOM_BREATHING_Attack.X" );
	}


	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_StateStart()
	{
		CommonStateStart();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_FrameMove()
	{
		ChangeWorldColorByHyperMode();
		ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.04f, 0 );
		
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if (m_pXSkinAnim->EventTimer( 0.25f ) == true && EventCheck( 0.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_PHANTOM_BREATHING", this );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.815f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.815f ) == true && EventCheck( 0.815f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 degree = GetRotateDegree();
			D3DXVECTOR3 pos = GetPos();
			D3DXVECTOR3 Dirvec = GetDirVector();

			Dirvec.y = 0;
			D3DXVec3Normalize( &Dirvec , &Dirvec );

			if ( GetIsRight() == true )
				pos += Dirvec*310.0f;
			else
			{
				degree.y += 180.0f;
				pos -= Dirvec*310.0f;
			}

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHANTOM_BREATHING_NO_DAMAGE", GetPowerRate(), pos, degree , degree ,m_FrameDataNow.unitCondition.landPosition.y );

			CX2DamageEffect::CEffect* pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHANTOM_BREATHING", GetPowerRate(), pos, degree , degree ,m_FrameDataNow.unitCondition.landPosition.y );

			if( NULL != pDE )		/// 스킬 레벨에 따른 버프 펙터 삽입
			{
				CX2DamageManager::DamageData& damageData = pDE->GetDamageData();

				//if( NULL != pDamageData )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AVP_PHANTOM_BREATHING_DARK_FALL );
					//pDE->GetDamageData().m_ExtraDamage.m_DamagePerSec = -1 * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TARGET_MANA );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					{
						if ( NULL == GetUnit() )
							return;

						const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

						damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
					}
#else //UPGRADE_SKILL_SYSTEM_2013
					if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
						damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
				}
			}
			
			pos = GetBonePos( L"Bip01_L_Finger11" );
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Pa_AVP_PHANTOM_BREATHING_Finger", pos );
		}
		
		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_EventProcess()
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_StateEnd()
	{
		CommonStateEnd();
	}



// 팬텀브리딩 hyper mode
	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_Init()
	{
		XSkinMeshReadyInBackground( L"AVP_PHANTOM_BREATHING_Body.X" );
		XSkinMeshReadyInBackground( L"AVP_PHANTOM_BREATHING_Attack.X" );
	}


	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_StateStart()
	{
		CommonStateStart();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.04f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if (m_pXSkinAnim->EventTimer( 0.25f ) == true && EventCheck( 0.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{	
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_PHANTOM_BREATHING", this );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.815f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 0.815f ) == true && EventCheck( 0.815f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 degree = GetRotateDegree();
			D3DXVECTOR3 pos = GetPos();
			D3DXVECTOR3 Dirvec = GetDirVector();

			Dirvec.y = 0;
			D3DXVec3Normalize( &Dirvec , &Dirvec );

			if ( GetIsRight() == true )
				pos += Dirvec*310.0f;
			else
			{
				degree.y += 180.0f;
				pos -= Dirvec*310.0f;
			}

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHANTOM_BREATHING_NO_DAMAGE", GetPowerRate(), pos, degree , degree ,m_FrameDataNow.unitCondition.landPosition.y );

			CX2DamageEffect::CEffect* pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"PHANTOM_BREATHING", GetPowerRate(), pos, degree , degree ,m_FrameDataNow.unitCondition.landPosition.y );

			if( NULL != pDE )		/// 스킬 레벨에 따른 버프 펙터 삽입
			{
				CX2DamageManager::DamageData& damageData = pDE->GetDamageData();

				//if( NULL != pDamageData )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_AVP_PHANTOM_BREATHING_DARK_FALL );
					//float ff = pDE->GetDamageData().m_ExtraDamage.m_DamagePerSec = -1 * pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_TARGET_MANA );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
					{
						if ( NULL == GetUnit() )
							return;

						const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

						damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
					}
#else //UPGRADE_SKILL_SYSTEM_2013
					if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
						damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013
				}
			}

			pos = GetBonePos( L"Bip01_L_Finger11" );
			g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Pa_AVP_PHANTOM_BREATHING_Finger", pos );
		}
		

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_EventProcess()
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{
			StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_SA_AVP_PHANTOM_BREATHING_HYPER_StateEnd()
	{
		CommonStateEnd();
	}

#endif NEW_SKILL_2010_11
//}} 김상훈 2010.11.1 : 팬텀브리딩

#endif AISHA_SECOND_CLASS_CHANGE


#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	HRESULT CX2GUArme_VioletMage::OnFrameMove( double fTime, float fElapsedTime )
	{
		CX2GUUser::OnFrameMove( fTime, fElapsedTime );

		if( m_bNeedToUpdateWeaponEffect == true )
		{
			UpdateWeaponEnchantEffectAndExtraDamageType();
			m_bNeedToUpdateWeaponEffect = false;
		}

		return S_OK;
	}

	void CX2GUArme_VioletMage::NotifyDGManagerChanged( CKTDGManager& refInManager_ )		// DGManager에 등록
	{
		CX2GUUser::NotifyDGManagerChanged( refInManager_ );

// 		if ( IsRecursivelyManagedByDGManager() )
// 		{	
// 			if ( m_TransformWeapon != NULL )
// 				refInManager_.AddObjectChain( m_TransformWeapon.get() );
// 		}
// 		else
// 		{			
// 			if( m_TransformWeapon != NULL )
// 				refInManager_.RemoveObjectChain( m_TransformWeapon.get() );
// 		}+
	}

	void CX2GUArme_VioletMage::TransformStart( const WCHAR* pWstrMeshName_, const bool bChangeWeapon_ /*= false */ )
	{	
		if ( NULL == m_pOrgMotion && NULL != m_pMotion )
			m_pOrgMotion = m_pMotion;

#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
		SetUseChangeWeapon( true );		/// 무기 변경 설정
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
		
		if ( m_pMotion != NULL )
			m_pXSkinAnim->RemoveModelXSkinMesh( m_pMotion );
		
		m_pMotion			= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pWstrMeshName_ );			

		m_CollisionListSet.clear();
		m_AttackListSet.clear();
				
 		CKTDXDeviceXET *pAnimXet = g_pKTDXApp->GetDeviceManager()->OpenXET( L"Mesh_Arme_ABM_Transform_Face.XET" );
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if(GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO25 ) == true )
		{
			CKTDXDeviceXET *pChangeTextureXet = NULL;
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH )
				pChangeTextureXet = g_pKTDXApp->GetDeviceManager()->OpenXET( L"Mesh_Arme_ADW_Cash_M_A.xet");
			else
				pChangeTextureXet = g_pKTDXApp->GetDeviceManager()->OpenXET( L"Mesh_Arme_BM_Cash_M_A.xet");

			m_pXSkinAnim->AddModelXSkinMesh( m_pMotion, pAnimXet, NULL, pChangeTextureXet );
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		m_pXSkinAnim->AddModelXSkinMesh( m_pMotion, pAnimXet, NULL, NULL );

 		m_pXSkinAnim->SetModelDetailPercent( g_pData->GetModelDetailPercent() );

		m_CollisionListSet.insert( &m_pXSkinAnim->GetCollisionDataList() );		
		m_AttackListSet.insert( &m_pXSkinAnim->GetAttackDataList() );		

		CX2GUUser::SetShowViewList(false);

		if( bChangeWeapon_ == true )
		{
			ChangeWeapon(m_ptrTransformWeapon);

			CKTDGXSkinAnim* pAnim = NULL;
			for( int j=0; j<CX2Item::MAX_MODEL_COUNT_A_ITEM; j++ )
			{
				pAnim = m_ptrTransformWeapon->GetNormalAnim(j);
				if( pAnim != NULL)
				{
					if ( false == pAnim->GetCollisionDataList().empty() )
						m_CollisionListSet.insert( &pAnim->GetCollisionDataList() );

					if ( false == pAnim->GetAttackDataList().empty() )
						m_AttackListSet.insert( &pAnim->GetAttackDataList() );
				}
			}
		}
		else
		{
			for(UINT i=0; i<m_ViewEqipList.size(); ++i)
			{
				CX2EqipPtr pViewEquipPtr = m_ViewEqipList[i];
				if ( NULL != pViewEquipPtr )
				{	
					if ( pViewEquipPtr->GetEqipPos() == CX2Unit::EP_WEAPON_HAND )
					{
						pViewEquipPtr->SetShowObject(true);
						break;
					}
				}
			}
		}

		SetEnableAttackBox( L"Rfoot", false );
		SetEnableAttackBox( L"Lfoot", false );
	}

	void CX2GUArme_VioletMage::TransformEnd( const bool bChangeWeapon_ /*= false */ )
	{
#ifdef FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP
		SetUseChangeWeapon( true );		/// 무기 변경 설정
#endif FIX_ENCHANT_FROZEN_TRACE_BUG_BY_MAGICAL_MAKEUP

		if( m_pMotion != NULL && m_pOrgMotion != NULL && m_pMotion != m_pOrgMotion )
		{
			m_pXSkinAnim->RemoveModelXSkinMesh( m_pMotion );
			SAFE_CLOSE( m_pMotion );
		}		

		m_pMotion = m_pOrgMotion;
		m_pOrgMotion = NULL;
		m_RenderParam.color = D3DXCOLOR( 1.f, 1.f, 1.f, 1.f );

		if ( bChangeWeapon_ )
		{
			for(UINT i=0; i<m_ViewEqipList.size(); ++i)
			{
				CX2EqipPtr pViewEquipPtr = m_ViewEqipList[i];
				if ( NULL != pViewEquipPtr )
				{
					if ( pViewEquipPtr->GetEqipPos() == CX2Unit::EP_WEAPON_HAND )
					{
						ChangeWeapon( pViewEquipPtr );
						m_ptrTransformWeapon->SetShowObject(false);
						break;
					}
				}
			}
		}

		CX2GUUser::SetShowViewList( true );
		CX2GUUser::UpdateNotRenderingOtherEquipList();
		CX2GUUser::UpdateEquipCollisionData( m_ViewEqipList );

		SetEnableAttackBox( L"Rfoot", false );
		SetEnableAttackBox( L"Lfoot", false );
		
		if( m_ahEffectSet[EffSetID_Wing] != INVALID_EFFECTSET_HANDLE )
		{
			g_pX2Game->GetEffectSet()->StopEffectSet( m_ahEffectSet[EffSetID_Wing] );
		}

		// 그냥 무조건 수행 하도록...
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_MagicalMakeup_End", this );
		
	}

 	void CX2GUArme_VioletMage::ChangeWeapon( CX2EqipPtr ptrWeaon )
 	{	
 		CX2GameUnit::Weapon* pOldWeapon = m_vecpWeapon[0];

 		if ( NULL != pOldWeapon &&
 			pOldWeapon->m_pUserWeaponEquip != NULL &&
 			pOldWeapon->m_pUserWeaponEquip->GetItem() != NULL )
 		{
 			int iEnchantLevel = pOldWeapon->m_pUserWeaponEquip->GetEnchantLevel();
 			const CX2Item::ItemTemplet* pOldItemTemplet = 
                pOldWeapon->m_pUserWeaponEquip->GetItem()->GetItemTemplet();
 
 			pOldWeapon->m_pUserWeaponEquip.reset();
 			SAFE_DELETE_KTDGOBJECT( pOldWeapon );
 			m_vecpWeapon.resize(0);
 
 			// 강화 이펙트를 위해 Enchant level 변경
 			ptrWeaon->ChangeEnchantLevel( iEnchantLevel );
 			CreateWeapon( pOldItemTemplet, ptrWeaon );
			ptrWeaon->SetShowObject(true);

			m_bNeedToUpdateWeaponEffect = true;
		}		
 	}

	void CX2GUArme_VioletMage::ABM_ComboZ1EventProcess()
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.oneZ == true && 
			m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.43f )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.43f )
		{
			StateChange( AVSI_COMBO_Z2 );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
		else if( SpecialAttackEventProcess() == true )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}
#endif //SKILL_CANCEL_BY_HYPER_MODE
		else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			m_fCanNotAttackTime = 0.1f;
			StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}
	void CX2GUArme_VioletMage::ABM_ComboZ2EventProcess()
	{		
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.oneZ == true && 
			m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.433f )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.433f )
		{
			if( m_InputData.pureUp == true )
				StateChange( AVSI_COMBO_Z3up );
			else
				StateChange( AVSI_COMBO_Z3 );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}				
		else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			m_fCanNotAttackTime = 0.1f;
			StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}
	void CX2GUArme_VioletMage::ABM_ComboZ3EventProcess()
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else if( m_InputData.oneZ == true && 
			m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.251f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.466f )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
		else if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.466f )
		{
			StateChange( AVSI_COMBO_Z4 );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}		
		else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.647f && CAN_WALK_CANCEL )
		{			
			StateChange( USI_WALK );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;			
		}
		else if( CAN_DASH_CANCEL )
		{
			StateChangeDashIfPossible();
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		SKILL_CANCEL_AFTER( 0.01f )
#else // SKILL_CANCEL_BY_HYPER_MODE
		else if( SpecialAttackEventProcess() == true )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}
#endif //SKILL_CANCEL_BY_HYPER_MODE
		else if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			m_fCanNotAttackTime = 0.1f;
			StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}
	void CX2GUArme_VioletMage::ABM_ComboZ4EventProcess()
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
		else if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 1.133f && CAN_WALK_CANCEL )
		{			
			StateChange( USI_WALK );		
		}
#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		SKILL_CANCEL_AFTER( 1.345f )
#else // SKILL_CANCEL_BY_HYPER_MODE
		else if( m_pXSkinAnimFuture->GetNowAnimationTime() > 1.345f && SpecialAttackEventProcess() == true )
		{
		}
#endif //SKILL_CANCEL_BY_HYPER_MODE

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_ABM_DASH_COMBO_Z2a_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 pos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01" );
			pos.y -= 200.0f;

			g_pX2Game->GetMinorXMeshPlayer()->CreateInstance( (CKTDGObject*) this,  L"ArmeEvasion", 
				pos, 
				m_FrameDataNow.unitCondition.dirDegree,
				m_FrameDataNow.unitCondition.dirDegree,
				XL_EFFECT_0 );

#ifdef SERV_ARME_DIMENSION_WITCH
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if( m_iAdvancedTeleportationLv > 0 && GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO5 ) == true )
#else //UPGRADE_SKILL_SYSTEM_2013
			if( m_iAdvancedTeleportationLv >= 5 && GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO5 ) == true )
#endif //UPGRADE_SKILL_SYSTEM_2013
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013
				D3DXVECTOR2 superArmorTime( 0.0f, 0.5f);
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				AccessVecNowSuperArmorTime().push_back(superArmorTime);
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				m_FrameDataNow.stateParam.m_vecSuperArmorTime.push_back(superArmorTime);
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				m_FrameDataNow.stateParam.bSuperArmorNotRed = false;
#else // UPGRADE_SKILL_SYSTEM_2013
				if( GetForceInvincible() < 0.5f )
					SetForceInvincible( 0.5f );
				if( GetShowInvincible() < 0.5f )
					SetShowInvincible( 0.5f );
#endif // UPGRADE_SKILL_SYSTEM_2013
			}
#endif	//SERV_ARME_DIMENSION_WITCH
		}		

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_ABM_DASH_COMBO_Z2a_EventProcess()
	{
		if( m_InputData.oneX == true )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.633f )
		{
			StateChange( ABM_DASH_COMBO_ZXX );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
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

	void CX2GUArme_VioletMage::AVSI_ABM_DASH_COMBO_ZXX_EventProcess()
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
		else
		{
			if( m_InputData.oneX == true && 
				m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.26f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.566f )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
			}

			if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.566f )
			{
				StateChange( ABM_DASH_COMBO_ZXXX );
				m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
			}
			else if ( CAN_WALK_CANCEL && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.466f )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
				StateChange( USI_WALK );
			}
			else if ( CAN_DASH_CANCEL && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.39f )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
				StateChangeDashIfPossible();
			}						
		}
		

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_ABM_DASH_COMBO_ZXXX_FrameMove()
	{
		if( m_pComboX3Up1->GetState() == CKTDGXSkinAnim::XAS_PLAYING )
		{
			if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
			{
				m_FrameDataNow.unitCondition.bHit = true;
			}
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.833f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.833f ) == true && EventCheck( 0.833f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( FlushMp( 10.0f ) == true )
			{
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_ARME_DIMENSION_WITCH
				if( (GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN || GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH) &&
					IsTransformed() )
#else
				if(  GetUnit()->GetClass() == CX2Unit::UC_ARME_BATTLE_MAGICIAN && IsTransformed() )
#endif //SERV_ARME_DIMENSION_WITCH
				{
					m_pComboX3Up1->SetScale( D3DXVECTOR3(2.f, 1.f, 2.f) );
					m_pComboX3Up2->SetScale( D3DXVECTOR3(2.f, 1.f, 2.f) );
					m_pComboX3Up3->SetScale( D3DXVECTOR3(2.f, 1.f, 2.f) );
				}
				else
				{
					m_pComboX3Up1->SetScale( D3DXVECTOR3(1.f, 1.f, 1.f) );
					m_pComboX3Up2->SetScale( D3DXVECTOR3(1.f, 1.f, 1.f) );
					m_pComboX3Up3->SetScale( D3DXVECTOR3(1.f, 1.f, 1.f) );
				}
#endif	SERV_ADD_ARME_BATTLE_MAGICIAN
				m_pComboX3Up1->GetRenderParam()->color = 0xffffffff;
				m_pComboX3Up2->GetRenderParam()->color = 0xffffffff;
				m_pComboX3Up3->GetRenderParam()->color = 0xffffffff;

				D3DXVECTOR3 pos = GetPos();
				pos.y += 5.0f;
				m_pComboX3Up1->GetMatrix().Move( pos );
				m_pComboX3Up1->GetMatrix().RotateDegree( GetRotateDegree() );
				m_pComboX3Up2->GetMatrix().Move( pos );
				m_pComboX3Up2->GetMatrix().RotateDegree( GetRotateDegree() );
				m_pComboX3Up3->GetMatrix().Move( pos );
				m_pComboX3Up3->GetMatrix().RotateDegree( GetRotateDegree() );

				m_pComboX3Up1->ChangeAnim( L"ComboX3Up-1", false );
				m_pComboX3Up2->ChangeAnim( L"ComboX3Up-2", false );
				m_pComboX3Up3->ChangeAnim( L"ComboX3Up-3", false );

				m_pComboX3Up1->Play( CKTDGXSkinAnim::XAP_ONE );
				m_pComboX3Up2->Play( CKTDGXSkinAnim::XAP_ONE );
				m_pComboX3Up3->Play( CKTDGXSkinAnim::XAP_ONE );
				m_pComboX3Up3->SetPlaySpeed( 0.8f );

				m_DamageData.attackerType	= CX2DamageManager::AT_MESH;
				m_DamageData.bCanRevenge	= false;
				m_DamageData.pAttackerAnim	= m_pComboX3Up1;
			}
			else
			{
				CreateStepDust();
			}
		}

		CommonFrameMove();
	}
	void CX2GUArme_VioletMage::AVSI_ABM_DASH_COMBO_ZXXX_EventProcess()
	{
#ifdef SERV_ARME_DIMENSION_WITCH
		if( GetUnit()->GetUnitData().m_UnitClass == CX2Unit::UC_ARME_DIMENSION_WITCH )
		{
			if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 1.014f )
			{
				StateChange( AVSI_ADW_COMBO_ADD_X2 );
				m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
			}
			else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 1.014f )
			{
				m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
			}
		}
#endif	//SERV_ARME_DIMENSION_WITCH
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

	void CX2GUArme_VioletMage::AVSI_A_AV_FIRE_ROAD_Start()
	{
		CommonStateStart();

		// To Do...
	}

	void CX2GUArme_VioletMage::AVSI_A_AV_FIRE_ROAD_EventProcess()
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
		else if ( CAN_DASH_CANCEL && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.25f )
		{
			StateChangeDashIfPossible();
		}	
		else if ( CAN_WALK_CANCEL && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.638f )
		{			
			StateChange( USI_WALK );
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_A_AV_FIRE_ROAD_FrameMove()
	{		
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.24f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			D3DXVECTOR3 vPos = GetPos();
			vPos.y += 70.f;

			float fLifeTimeRate = 1.f;
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AV_FIRE_ROAD );
			if( NULL != pSkillTemplet )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				fLifeTimeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				fLifeTimeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
			}

			g_pX2Game->GetDamageEffect()->CreateInstance( this, L"ARME_FIRE_ROAD", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, -1.f, fLifeTimeRate);


		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_A_ABM_ENERGY_DRAIN_READY_FrameMove()
	{
		// To Do...
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.47f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.47f ) == true && EventCheck( 0.47f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			float fDistance = 2000.f;
			m_iDrainEnergyTarget = -1;
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ABM_ENERGY_DRAIN );
			if( NULL != pSkillTemplet )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				float fDistanceTemp = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				float fDistanceTemp = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				fDistance = fDistanceTemp;

				CX2GameUnit *pFrontNearestUnit = NULL;
				for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
				{
					CX2GameUnit *pUnit = g_pX2Game->GetUnit( i );

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
						CX2GUNPC *pNpc = (CX2GUNPC*)pUnit;
						if( pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
							continue;

						fOffset = ( pNpc->GetOrgUnitSize().x + pNpc->GetOrgUnitSize().y ) / 4.f;
					}					

					D3DXVECTOR3 vDistance = pUnit->GetPos() - GetPos();

					float fDistanceUnit = D3DXVec3Length( &vDistance ) - fOffset;
					if( fDistance >= fDistanceUnit )
					{
						fDistance = fDistanceUnit;
						m_iDrainEnergyTarget = i;
					}
				}
			}

			if( m_iDrainEnergyTarget >= 0 )
			{	
				StateChange( AVSI_A_ABM_ENERGY_DRAIN_SUCCESS, true );
			}
			else
			{
				StateChange( AVSI_A_ABM_ENERGY_DRAIN_FAIL, true );
			}
		}	
		else if( m_iDrainEnergyTarget < 0 && m_pXSkinAnim->IsAnimationEnd() == true )
		{
			StateChange( AVSI_A_ABM_ENERGY_DRAIN_FAIL, true );
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_A_ABM_ENERGY_DRAIN_READY_EventProcess()
	{
// 		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
// 		{	
// 			if( false == IsOnSomethingFuture() )
// 			{
// 				StateChange( USI_JUMP_DOWN );
// 				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
// 			}
// 			else
// 				StateChange( AVSI_A_ABM_ENERGY_DRAIN_FAIL );
// 		}
// 
// 		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_A_ABM_ENERGY_DRAIN_SUCCESS_FrameMove()
	{
		// To Do...
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( m_iDrainEnergyTarget >= 0 )
			{
				CX2GameUnit *pUnit = g_pX2Game->GetUnit( m_iDrainEnergyTarget );

				if( pUnit != NULL && g_pX2Game->IsValidUnit( pUnit ) == true )
				{					
					const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ABM_ENERGY_DRAIN );
					if( NULL != pSkillTemplet )
					{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
						if ( NULL == GetUnit() )
							return;
	
						const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
						float fDrainRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
						float fDrainRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
						
#ifdef ADDITIONAL_MEMO
						if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO13 ) == true && GetRandomFloat() <= 0.3f )
							fDrainRate *= 2.f;
#endif	//ADDITIONAL_MEMO
						m_DamageData.optrAttackerGameUnit	= this;
						m_DamageData.impactPoint			= pUnit->GetPos();
						m_DamageData.optrDefenderGameUnit	= pUnit;
						m_DamageData.reActResult			= m_DamageData.reActType;
						m_DamageData.m_fDrainHpRate		= fDrainRate;
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
						m_DamageData.m_fDrainMpByReact		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
						m_DamageData.m_fDrainMpByReact		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
						m_DamageData.m_fDrainMpRate		= fDrainRate;

						pUnit->DamageReact(&m_DamageData);

						// 공격자 이펙트
						g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_EnegryDrain01", this );
						

						// 피격자 이펙트
						g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_EnegryDrain02", pUnit, pUnit );						
					}					
				}
				
				m_iDrainEnergyTarget = -1;
			}
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_A_ABM_ENERGY_DRAIN_EventProcess()
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
		else if ( CAN_WALK_CANCEL && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.451f )
		{			
			StateChange( USI_WALK );
		}
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_HEAVY_PRESS_FrameMove()
	{
		// 각성 인지 아닌지에 따라 World색 변경
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.1f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			ClearHitUnitList_LUA();
		}		

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_ENERGY_SPURT_FrameMove()
	{
		// 각성 인지 아닌지에 따라 World색 변경
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_Spine1", 0.03f, 1 );

		CommonFrameMove();
	}
	
	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_GUILLOTINE_PRESS_FrameMove()
	{
		// 각성 인지 아닌지에 따라 World색 변경
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_Spine1", 0.03f, 1 );

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_GUILLOTINE_PRESS_START_EventProcess()
	{
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			if( false == IsOnSomethingFuture() )
			{
				StateChange( AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP );
				m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
			}
			else
				StateChange( AVSI_SI_SA_ABM_GUILLOTINE_PRESS_END );
		}

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP_EventProcess()
	{			
		if( false == IsOnSomethingFuture() )
		{
			StateChange( AVSI_SI_SA_ABM_GUILLOTINE_PRESS_LOOP );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		else
			StateChange( AVSI_SI_SA_ABM_GUILLOTINE_PRESS_END );

		CommonEventProcess();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_SUPER_NOVA_FrameMove()
	{
		// 각성 인지 아닌지에 따라 World색 변경
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Bip01_Spine1", 0.02f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_SuperNova01", this );
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 1.8f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 1.8f ) == true && EventCheck( 1.8f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			int nCount = 0;
			for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
			{
				CX2GameUnit *pUnit = g_pX2Game->GetUnit( i );

				if( g_pX2Game->IsValidUnit( pUnit ) == false )
					continue;				
				if( pUnit->GetInvincible() == true )
					continue;
				if( pUnit->GetGameUnitState() == CX2GameUnit::GUSI_DIE || pUnit->GetNowHp() <= 0.f )
					continue;					

				float fOffset = 0.f;
				if(pUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC)
				{
					CX2GUNPC *pNpc = (CX2GUNPC*)pUnit;
					if( pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
						continue;

					fOffset = ( pNpc->GetUnitWidth(false) + pNpc->GetUnitHeight(false) ) / 2.f;
				}
				else
				{
					CX2GUUser *pUser = (CX2GUUser*)pUnit;
					if( pUser->IsMyUnit() == true )
						continue;
				}

				D3DXVECTOR3 vDistance = GetPos() - pUnit->GetPos();
				float fDistanceUnit = D3DXVec3Length( &vDistance ) - fOffset;
				if( fDistanceUnit <= 700.f )
				{
					++nCount;
				}
			}

			float fScale = 1.f + (nCount / 10.f);
			if( fScale < 1.f )
				fScale = 1.f;
			else if( fScale > 2.f )
				fScale = 2.f;
			float fDamageScale = 1.f + ( nCount * 0.05f );
			if( fDamageScale < 1.f )
				fDamageScale = 1.f;
			else if( fDamageScale > 1.5f )
				fDamageScale = 1.5f;

			CX2EffectSet::Handle hSuperNova = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_SuperNova02", this );
 			if( hSuperNova != INVALID_EFFECTSET_HANDLE )
 			{
 				if ( CX2EffectSet::EffectSetInstance* pSuperNova = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hSuperNova ) )
				{
					pSuperNova->SetPowerRateScale( fDamageScale );
 					pSuperNova->SetEffectScale( D3DXVECTOR3( fScale * GetScaleByUnit().x, fScale * GetScaleByUnit().y, fScale * GetScaleByUnit().z ), true );
				}
 			}
		}
		
		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_MAGICAL_MAKEUP_Init()
	{
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		XSkinMeshReadyInBackground(  L"Mesh_Arme_BM_Cash_Upbody_Transforming_Sorted.X" );
		XSkinMeshReadyInBackground(  L"Mesh_Arme_BM_Cash_Upbody_Transform_Sorted.X" );

#ifdef SERV_ARME_DIMENSION_WITCH
		if( GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH ) 
		{
			XSkinMeshReadyInBackground(  L"Mesh_Arme_ADW_Cash_Upbody_Transforming_Sorted.X" );
			XSkinMeshReadyInBackground(  L"Mesh_Arme_ADW_Cash_Upbody_Transform_Sorted.X" );
		}
#endif	// SERV_ARME_DIMENSION_WITCH

#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

		XSkinMeshReady(  L"Mesh_Arme_BM_Cash_Upbody_Transforming_Sorted.X" );
		XSkinMeshReady(  L"Mesh_Arme_BM_Cash_Upbody_Transform_Sorted.X" );

#ifdef SERV_ARME_DIMENSION_WITCH
		if( GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH ) 
		{
			XSkinMeshReady(  L"Mesh_Arme_ADW_Cash_Upbody_Transforming_Sorted.X" );
			XSkinMeshReady(  L"Mesh_Arme_ADW_Cash_Upbody_Transform_Sorted.X" );
		}
#endif	// SERV_ARME_DIMENSION_WITCH

#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_MAGICAL_MAKEUP_FrameMove()
	{
		// 각성 인지 아닌지에 따라 World색 변경
		ChangeWorldColorByHyperMode();
		ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.03f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.7f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.7f ) == true && EventCheck( 0.7f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			SetTransformTime( _CONST_AISHA_::MAGICAL_MAKEUP_TRANSFORMING_TIME );	/// 변신 동작을 수행하는 시간
			SetUseWorldColor( true );

#ifdef SERV_ARME_DIMENSION_WITCH
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH ) 
				TransformStart(  L"Mesh_Arme_ADW_Cash_Upbody_Transforming_Sorted.X" );
			else
				TransformStart(  L"Mesh_Arme_BM_Cash_Upbody_Transforming_Sorted.X" );
#else
			TransformStart(  L"Mesh_Arme_BM_Cash_Upbody_Transforming_Sorted.X" );
#endif
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 2.0f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 2.0f ) == true && EventCheck( 2.0f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet 
				= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ABM_MAGICAL_MAKEUP );
#ifdef ADD_MEMO_1ST_CLASS //김창한
			if(GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO25 ) == true )
				SetBuffFactorToGameUnit( pSkillTemplet, 1 );
#endif //ADD_MEMO_1ST_CLASS
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ABM_MAGICAL_MAKEUPR_StateEnd()
	{
		// To Do...

		if ( !IsTransformed() )
		{
			SetTransformTime( 0.0f );	/// 변신 동작 효과 진행을 끝낸다.
			const CX2SkillTree::SkillTemplet* pSkillTemplet 
				= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ABM_MAGICAL_MAKEUP );
			SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}

		SetForceInvincible( 1.f );
		CommonStateEnd();
	}
#endif // SERV_ADD_ARME_BATTLE_MAGICIAN

/*virtual*/ void CX2GUArme_VioletMage::DeleteMinorParticle()
{
	CX2GUUser::DeleteMinorParticle();

	DeleteArmeMinorParticle();
}

void CX2GUArme_VioletMage::ShowActiveSkillCutInAndLight( const WCHAR* szBoneName_, const float fTimeToShow_, const UINT uiCutInIndex_, const bool bOnlyLight_ /*= false */ )
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( fTimeToShow_ ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
 	if( m_pXSkinAnim->EventTimer( fTimeToShow_ ) == true && EventCheck( fTimeToShow_, false ) == true )
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

#ifdef BALANCE_PATCH_20120329
	//AVSI_HIGH_MAGICIAN_COMBO_X3down
	void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X3down_FrameMove()
	{
		if( m_pComboX3Up1->GetState() == CKTDGXSkinAnim::XAS_PLAYING )
		{
			if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
			{
				m_FrameDataNow.unitCondition.bHit = true;
			}
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.06f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.06f ) == true && EventCheck( 0.06f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( FlushMp( 10.0f ) == true )
			{
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ComboXXDownX", this );
			}
			else
			{
				CreateStepDust();
			}
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X3down_EventProcess()
	{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 입력 딜레이 뒤로 밀음
		if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f )
#else  BALANCE_ELEMENTAL_MASTER_20130117
		if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.266f )
#endif BALANCE_ELEMENTAL_MASTER_20130117
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
		}
		if( m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.6f && m_FrameDataFuture.stateParam.bEventFlagList[0] == true )
		{
			StateChange( AVSI_COMBO_X4down );
		}
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			//m_fCanNotAttackTime = 0.5f;
			StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}

	//AVSI_HIGH_MAGICIAN_COMBO_X4down
	void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X4down_FrameMove()
	{
		if( m_pComboX3Up1->GetState() == CKTDGXSkinAnim::XAS_PLAYING )
		{
			if( g_pData->GetDamageManager()->DamageCheck( &m_DamageData ) == true )
			{
				m_FrameDataNow.unitCondition.bHit = true;
			}
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.35f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.35f ) == true && EventCheck( 0.35f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( FlushMp( 10.0f ) == true )
			{
				g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ComboXXDownXX", this );
			}
			else
			{
				CreateStepDust();
			}
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_HIGH_MAGICIAN_COMBO_X4down_EventProcess()
	{
		if( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
		}
		ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( 0.8333 )
		{
			StateChange( USI_WAIT );
		}
		WALK_CANCEL_AFTER( m_fWalkCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )
		
		if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
		{	
			StateChange( USI_WAIT );
		}

		CommonEventProcess();
	}
#endif	// BALANCE_PATCH_20120329

#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION
	void CX2GUArme_VioletMage::AVSI_DIMENSION_WITCH_DASH_ATTACK_X_EventProcess()
	{
		if( m_FrameDataFuture.stateParam.bEventFlagList[0] == true && m_pXSkinAnimFuture->GetNowAnimationTime() > 0.604f )
		{
			StateChange( AVSI_DASH_ATTACK_X2 );
			m_FrameDataFuture.stateParam.bEventFlagList[0] = false;
		}
		else if( m_InputData.oneX == true && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.3f && m_pXSkinAnimFuture->GetNowAnimationTime() <= 0.604f )
		{
			m_FrameDataFuture.stateParam.bEventFlagList[0] = true;
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
	void CX2GUArme_VioletMage::AVSI_DIMENSION_WITCH_DASH_ATTACK_X2_EventProcess()
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
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
	}
	void CX2GUArme_VioletMage::AVSI_ADW_COMBO_ADD_X_EventProcess()
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
		SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
		DASH_CANCEL_AFTER( m_fDashCancelAfter )

		CommonEventProcess();
	}


	void CX2GUArme_VioletMage::AVSI_SI_A_ADW_DISTORTION_Init()
	{
		XSkinMeshReady( L"aisha_A_DISTORTION_dummy.X" );
	}

	void CX2GUArme_VioletMage::AVSI_SI_A_ADW_DISTORTION_StateStart()
	{
		CommonStateStart();

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ADW_DISTORTION_WEAPON", this );
	}


	void CX2GUArme_VioletMage::AVSI_SI_A_ADW_DISTORTION_FrameMove()
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.01f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.01f ) == true && EventCheck( 0.01f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
#ifdef MODIFY_DISTORTION_DESTRUCTION
			// 소환 유지시간은CX2Game::SetUserSummonedNPCInfo 에서 처리하도록 변경
			if( true == g_pX2Game->IsHost() )
			{
				SetSelfDestructSummonedNPC( CX2UnitManager::NUI_ADW_DISTORTION );

				// 위치 지정
				D3DXVECTOR3 vPosOffset = GetDirVector() * 150.f * GetScaleByUnit().x;
				D3DXVECTOR3 vPos = (true == GetIsRight()) ? GetPos() + vPosOffset : GetPos()  - vPosOffset ;
				vPos.y += 100.f * GetScaleByUnit().x;

				// 각성 : 1 , 미각성 : 0
				int iHyperMode = ( 0.f < GetRemainHyperModeTime() ) ? 1 : 0;

				g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_ADW_DISTORTION, iHyperMode, true, vPos, 
					GetIsRight(), 0.0f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );
			}
#else
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADW_DISTORTION );
			if( NULL != pSkillTemplet )
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				float fEffectTime = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
				float fEffectTime = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				D3DXVECTOR3 vPos = GetPos();

				if( true == GetIsRight() )
				{
					vPos += GetDirVector() * 150.f * GetScaleByUnit().x;
				}
				else
				{
					vPos -= GetDirVector() * 150.f * GetScaleByUnit().x;
				}

				vPos.y += 100.f * GetScaleByUnit().x;
			
				if( true == g_pX2Game->IsHost() )
				{
					int iHyperMode = 0; // 각성 아님, 1이면 각성임

					if ( 0.f < GetRemainHyperModeTime() )
						iHyperMode = 1;

					g_pX2Game->CreateNPCReq( CX2UnitManager::NUI_ADW_DISTORTION, iHyperMode, true, vPos, 
						GetIsRight(), 0.0f, true, -1, (CX2Room::TEAM_NUM) GetTeam(), CX2NPCAI::NAT_ALLY, GetUnitUID() );

					m_fDistortionLifeTime = fEffectTime;
					m_timerDistortionSummoned.restart();
				}
			}
#endif // MODIFY_DISTORTION_DESTRUCTION
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ADW_MORNING_STAR_FrameMove()
	{
		ChangeWorldColorByHyperMode();

		ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.03f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( 0.03f ) == true && EventCheck( 0.03f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			m_hEffectMorningStar = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ADW_MORNING_STAR_HAMMER", this );
			m_vecEffectSetToDeleteOnDamageReact.push_back( m_hEffectMorningStar );
		}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 1.49f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		else if( m_pXSkinAnim->EventTimer( 1.49f ) == true && EventCheck( 1.49f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( m_hEffectMorningStar != INVALID_EFFECTSET_HANDLE )
			{
				if ( CX2EffectSet::EffectSetInstance* pEffectSetInstance 
					= g_pX2Game->GetEffectSet()->GetEffectSetInstance( m_hEffectMorningStar ) )
				{
					vector<CX2EffectSet::EffectInstance*>& vecEffectInstance = pEffectSetInstance->m_vecpEffectInstance;
					for ( UINT i=0; i < vecEffectInstance.size(); ++i )
					{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                        CX2DamageEffect::CEffect *pDmageEffect = g_pX2Game->GetDamageEffect()->GetInstance( vecEffectInstance[i]->m_hDamageEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
						CX2DamageEffect::CEffect *pDmageEffect = vecEffectInstance[i]->m_pDamageEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
						if ( CKTDGXMeshPlayer::CXMeshInstance *pMeshInstance = ( pDmageEffect != NULL ) ? pDmageEffect->GetMainEffect() : NULL )
						{
							D3DXVECTOR3 vPos 
								= pMeshInstance->GetBonePos( L"effect_Dummy03" );

							CX2EffectSet::Handle hMorningStarHit = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ADW_MORNING_STAR_HIT", this, NULL, false, -1.f, -1.f, D3DXVECTOR3(1.f, 1.f, 1.f), true, vPos );

							CX2DamageEffect::CEffect *pDmageHitEffect 
								= g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DAMAGE_EFFECT_ADW_MORNING_STAR_HIT", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );
							if ( NULL != pDmageHitEffect )
							{
								CX2DamageManager::DamageData& damageData = pDmageHitEffect->GetDamageData();
								//if ( NULL != pDamageData )
								{
									const CX2SkillTree::SkillTemplet* pSkillTemplet 
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
										= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADW_IMPACT_HAMMER );

									if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
									{
										if ( NULL == GetUnit() )
											return;

										const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
										const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

										damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel ) );
									}
#else //UPGRADE_SKILL_SYSTEM_2013
										= GetEquippedActiveSkillTemplet( CX2SkillTree::SI_SA_ADW_MORNING_STAR );

									if( NULL != pSkillTemplet && !pSkillTemplet->m_vecBuffFactorPtr.empty() )
										damageData.PushBuffFactor( pSkillTemplet->m_vecBuffFactorPtr[0] );
#endif //UPGRADE_SKILL_SYSTEM_2013

									break;
								}
							}
						}
					}	/// for
				}	/// if
			}	/// if
		}

		CommonFrameMove();
	}

	void CX2GUArme_VioletMage::AVSI_SI_SA_ADW_SCREWDRIVER_FrameMove()
	{
		ChangeWorldColorByHyperMode();
		ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

		CommonFrameMove();
	}
	
#endif	// SERV_ARME_DIMENSION_WITCH

/*virtual*/ void CX2GUArme_VioletMage::HyperModeBuffEffectStart()
{
	CX2GUUser::HyperModeBuffEffectStart();

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRFoot )
		m_hHyperBoostRFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightViolet",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLFoot )
		m_hHyperBoostLFoot = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftViolet",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostRArm )
		m_hHyperBoostRArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostRightViolet",	0,0,0, 0, 0 );

	if ( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hHyperBoostLArm )
		m_hHyperBoostLArm = g_pX2Game->GetMinorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  L"HyperBoostLeftViolet",	0,0,0, 0, 0 );
}

/** @function : GetAdditionalAccelBuffTime
	@brief : 엑셀레이터류 버프의 추가시간을 얻어오는 함수
	@param : 어떤 엑셀레이터가 발동됐는지에 대한 스킬아이디(eSkillID_)
	@return : 추가시간(float)
*/
/*virtual*/ float CX2GUArme_VioletMage::GetAdditionalAccelBuffTime( const CX2SkillTree::SKILL_ID eSkillID_ ) const
{
	float fAdditionalAccelBuffTime = 0.0f;

	/// 배메, 기력가속
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang


#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// 기력 가속은 패시브 이므로 기본 레벨만 적용 받도록 수정
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADW_SPIRIT_ACCELERATION, true );
#else // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADW_SPIRIT_ACCELERATION );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편


		if ( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ADW_SPIRIT_ACCELERATION );

			if( NULL != pSkillTemplet )
			{
				fAdditionalAccelBuffTime += pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillLevel );
			}
		}
#else //UPGRADE_SKILL_SYSTEM_2013
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ABM_SPIRIT_ACCELERATION );

		if ( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ABM_SPIRIT_ACCELERATION, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				fAdditionalAccelBuffTime += pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
			}
		}
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

	/// 하이매지션, 격려의 기운
	{
		const int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_SUPPORT_AURA );
		if ( iSkillLevel > 0 )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_SUPPORT_AURA );

			if( NULL != pSkillTemplet )
			{
				fAdditionalAccelBuffTime += pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillLevel );
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_SUPPORT_AURA, iSkillLevel );

			if( NULL != pSkillTemplet )
			{
				fAdditionalAccelBuffTime += pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	return fAdditionalAccelBuffTime;
}

/** @function : ApplyAuraAcceleratorBuffToGameUnit
	@brief : 액셀러레이터버프를 특정 범위내의 유닛에게 적용, 아이샤는 부가적인 효과 처리를 위해 overriding
	@param : 앨셀러레이터스킬템플릿(pSkillTemplet_), 외부요인(fAddtionalAccelBuffTime_)
*/
/*virtual*/ void CX2GUArme_VioletMage::ApplyAuraAcceleratorBuffToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const float fAddtionalAccelBuffTime_, const D3DXVECTOR3& vMyPos_ )
{
	CX2BuffFactorPtr ptrEnergyOfThePlentyFactor 
		= GetEnergyOfThePlentyBuffFactorClonePtr( pSkillTemplet_->m_eID );

	/// 풍요의 기운을 안 배웠으면
	if ( NULL == ptrEnergyOfThePlentyFactor )
		CX2GUUser::ApplyAuraAcceleratorBuffToGameUnit( pSkillTemplet_, fAddtionalAccelBuffTime_, vMyPos_ );
	else	/// 풍요의 기운을 배웠으면
	{
		if ( false == pSkillTemplet_->m_vecBuffFactorPtr.empty() )	/// 액셀러레이터검사
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( NULL == GetUnit() )
				return;

			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨

			CX2BuffFactorPtr ptrBuffFactorClone = pSkillTemplet_->m_vecBuffFactorPtr[0]->GetClonePtr( iSkillTempletLevel );
#else //UPGRADE_SKILL_SYSTEM_2013
			CX2BuffFactorPtr ptrBuffFactorClone = pSkillTemplet_->m_vecBuffFactorPtr[0]->GetClonePtr();
#endif //UPGRADE_SKILL_SYSTEM_2013
			if ( NULL != ptrBuffFactorClone )
			{
				ptrBuffFactorClone->SetGameUnitBuffFactor( this );	/// 엑셀러레이터 버프팩터 셋팅
				ptrBuffFactorClone->ApplyExternalFactorToFinalizer( BFT_TIME, fAddtionalAccelBuffTime_ );

				ptrEnergyOfThePlentyFactor->SetGameUnitBuffFactor( this );	/// 풍요의 기운 버프팩터 셋팅

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == GetUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet_->m_eID ) );	/// 스킬 레벨
	
				ptrEnergyOfThePlentyFactor->ApplyExternalFactorToFinalizer( BFT_TIME, 	/// 풍요의 기운은 외부요인에 의한 추가 시간 1/3 적용
					( pSkillTemplet_->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel ) + fAddtionalAccelBuffTime_ ) / 3 );
	#else // UPGRADE_SKILL_SYSTEM_2013
				ptrEnergyOfThePlentyFactor->ApplyExternalFactorToFinalizer( BFT_TIME, 	/// 풍요의 기운은 외부요인에 의한 추가 시간 1/3 적용
					( pSkillTemplet_->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME ) + fAddtionalAccelBuffTime_ ) / 3 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
				const WCHAR* pwsCoreParticleName = CX2SkillTree::GetAccelCoreParticleName( pSkillTemplet_->m_eID );	
				ASSERT( NULL != pwsCoreParticleName );

				for ( int i = 0; i < g_pX2Game->GetUnitNum(); i++ )
				{
					CX2GameUnit* pGameUnit = g_pX2Game->GetUnit( i );
					if( pGameUnit != NULL && 
						GetDistance3Sq( vMyPos_, pGameUnit->GetPos() ) < 490000.f &&	/// 거리 제한
						pGameUnit->CanApplyBuffToGameUnit() )
					{
						if ( pGameUnit->GetTeam() == GetTeam() )	/// 같은 팀
						{
							if ( g_pX2Game->GetGameType() == CX2Game::GT_PVP || pGameUnit->GetUnitUID() == GetUnitUID() ||
								( NULL != g_pData && NULL != g_pData->GetPartyManager() && true == g_pData->GetPartyManager()->IsMyPartyMember( pGameUnit->GetUnitUID() ) ) ) 
							{
								if ( pGameUnit->SetBuffFactorToGameUnit( *ptrBuffFactorClone ) )
								{
									D3DXVECTOR3 vGameUnitPos = pGameUnit->GetPos();
									vGameUnitPos.y += 100.0f;
									g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  pwsCoreParticleName, vGameUnitPos );
								}
							}
						}
						else	/// 다른 팀에게는 풍요의기운 디버프
						{
							pGameUnit->SetBuffFactorToGameUnit( *ptrEnergyOfThePlentyFactor );
						}
					}
				}	// for
			} // if ( NULL != ptrBuffFactorClone )
		}
	}
}

/** @function : GetEnergyOfThePlentyBuffFactorClonePtr
	@brief : 풍요의기운(엘마) 패시브 스킬이 가지고 있는 버프팩터를 얻어오는 함수
	@param : 사용한 액셀러레이터스킬 ID(eSkillIdAccelerator_)
	@return : 풍요의기운을 배우지 않았으면 빈 스마트포인터 리턴
*/
CX2BuffFactorPtr CX2GUArme_VioletMage::GetEnergyOfThePlentyBuffFactorClonePtr( const CX2SkillTree::SKILL_ID eSkillIdAccelerator_ ) const
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const int iSkillLevelEnergyOfThePlenty = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AEM_ENERGY_OF_THE_PLENTY, true );
#else // UPGRADE_SKILL_SYSTEM_2013
	const int iSkillLevelEnergyOfThePlenty = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AEM_ENERGY_OF_THE_PLENTY );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( iSkillLevelEnergyOfThePlenty > 0 )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTempletEnergyOfThePlenty 
			= g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_AEM_ENERGY_OF_THE_PLENTY );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTempletEnergyOfThePlenty 
			= g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_AEM_ENERGY_OF_THE_PLENTY, iSkillLevelEnergyOfThePlenty );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		if ( NULL != pSkillTempletEnergyOfThePlenty )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
			if ( 0 < pSkillTempletEnergyOfThePlenty->m_vecBuffFactorID.size() )
			{
				int iValue = 0;

				switch ( eSkillIdAccelerator_ )	/// 사용한 오오라 액셀러레이터 스킬이 무엇인가에 따라서
				{
				case CX2SkillTree::SI_SA_COMMON_AURA_POWER_ACCEL:
					iValue = 0;
					break;
				case CX2SkillTree::SI_SA_COMMON_AURA_MAGIC_ACCEL:
					iValue = 1;
					break;
				case CX2SkillTree::SI_SA_COMMON_AURA_SHIELD_ACCEL:
					iValue = 2;
					break;
				default:
					return CX2BuffFactorPtr();
					break;
				}
				
				return pSkillTempletEnergyOfThePlenty->m_vecBuffFactorPtr[iValue]->GetClonePtr( iSkillLevelEnergyOfThePlenty );
			}

#else //UPGRADE_SKILL_SYSTEM_2013
			switch ( eSkillIdAccelerator_ )	/// 사용한 오오라 액셀러레이터 스킬이 무엇인가에 따라서
			{
			case CX2SkillTree::SI_SA_COMMON_AURA_POWER_ACCEL:
				if ( 0 < pSkillTempletEnergyOfThePlenty->m_vecBuffFactorPtr.size() )	/// PowerAccel에 해당하는 패시브
					return pSkillTempletEnergyOfThePlenty->m_vecBuffFactorPtr[0]->GetClonePtr();
				break;

			case CX2SkillTree::SI_SA_COMMON_AURA_MAGIC_ACCEL:
				if ( 1 < pSkillTempletEnergyOfThePlenty->m_vecBuffFactorPtr.size() )	/// MagicAccel에 해당하는 패시브
					return pSkillTempletEnergyOfThePlenty->m_vecBuffFactorPtr[1]->GetClonePtr();
				break;

			case CX2SkillTree::SI_SA_COMMON_AURA_SHIELD_ACCEL:
				if ( 2 < pSkillTempletEnergyOfThePlenty->m_vecBuffFactorPtr.size() )	/// MagicAccel에 해당하는 패시브
					return pSkillTempletEnergyOfThePlenty->m_vecBuffFactorPtr[2]->GetClonePtr();
				break;

			default:
				break;
			}

#endif //UPGRADE_SKILL_SYSTEM_2013
		}
	}

	return CX2BuffFactorPtr();
}

/** @function : SetSpecificValueByBuffTempletID
	@brief : 각 유닛마다 특정 버프가 실행 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 시전중이라는 플래그 설정 등...)
	@param : 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUArme_VioletMage::SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::SetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_SI_SA_ABM_MAGICAL_MAKEUP:
		{
			SetUseWorldColor( true );

			if( m_ahEffectSet[EffSetID_Wing] == INVALID_EFFECTSET_HANDLE )
			{
				if( GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH ) 
				{
#ifdef ADD_MEMO_1ST_CLASS //김창한
					if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO25) == true )
						m_ahEffectSet[EffSetID_Wing] = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ADW_MagicalMakeup_Wing_memo", this );
					else
#endif //ADD_MEMO_1ST_CLASS
					m_ahEffectSet[EffSetID_Wing] = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ADW_MagicalMakeup_Wing", this );
				}
				else
				{
#ifdef ADD_MEMO_1ST_CLASS //김창한
					if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO25) == true )
						m_ahEffectSet[EffSetID_Wing] = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_MagicalMakeup_Wing_memo", this );
					else
#endif //ADD_MEMO_1ST_CLASS
					m_ahEffectSet[EffSetID_Wing] = g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_MagicalMakeup_Wing", this );
				}
			}

			//TransformEnd();

			if ( NULL == m_ptrTransformWeapon )
			{
#ifdef SERV_ARME_DIMENSION_WITCH
				if( GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH )
				{
#ifdef ADD_MEMO_1ST_CLASS //김창한
					if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO25) == true )
						m_ptrTransformWeapon = CX2ItemManager::CreateDummyEquip( 41321, GetUnit(), m_pXSkinAnim );
					else
#endif //ADD_MEMO_1ST_CLASS
					m_ptrTransformWeapon = CX2ItemManager::CreateDummyEquip( 41320, GetUnit(), m_pXSkinAnim );
				}
				else
				{
#ifdef ADD_MEMO_1ST_CLASS //김창한
					if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO25) == true )
						m_ptrTransformWeapon = CX2ItemManager::CreateDummyEquip( 40931, GetUnit(), m_pXSkinAnim );
					else
#endif //ADD_MEMO_1ST_CLASS
					m_ptrTransformWeapon = CX2ItemManager::CreateDummyEquip( 40930, GetUnit(), m_pXSkinAnim );
				}
#else
				m_ptrTransformWeapon = CX2ItemManager::CreateDummyEquip( 40930, GetUnit(), m_pXSkinAnim );
#endif	//SERV_ARME_DIMENSION_WITCH
				m_ptrTransformWeapon->SetManagerObject( this );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_ptrTransformWeapon.get() );
			}

			m_bIsTransformed = true;

#ifdef SERV_ARME_DIMENSION_WITCH
			if( GetUnit()->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH ) 
				TransformStart(  L"Mesh_Arme_ADW_Cash_Upbody_Transform_Sorted.X" , true );
			else
				TransformStart(  L"Mesh_Arme_BM_Cash_Upbody_Transform_Sorted.X" , true );
#else
			TransformStart(  L"Mesh_Arme_BM_Cash_Upbody_Transform_Sorted.X" , true );
#endif	//SERV_ARME_DIMENSION_WITCH

			SetUnitReForm();
		} break;

#ifndef BALANCE_ELEMENTAL_MASTER_20130117		/// 메디테이션 버프 해제 기능 삭제로 인하여, StartMeditation 함수 이용 않함 + 디버프 효과 삭제
	case BTI_BUFF_SI_A_AHM_MEDITATION:
		{
			StartMeditation( true );
		} break;
#endif  BALANCE_ELEMENTAL_MASTER_20130117

	default:
		break;
	}
}

/** @function : UnSetSpecificValueByBuffTempletID
	@brief : 각 유닛마다 특정 버프가 해제 될 때 셋팅해야 하는 함수 실행(ex: 매지컬메이크업이 해제 됬다는 플래그 설정 등...)
	@param : 버프템플릿ID(eBuffTempletId_)
*/
/*virtual*/ void CX2GUArme_VioletMage::UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ )
{
	CX2GUUser::UnSetSpecificValueByBuffTempletID( eBuffTempletId_ );

	switch ( eBuffTempletId_ )
	{
	case BTI_SI_SA_ABM_MAGICAL_MAKEUP:
		{
			m_bIsTransformed = false;
			TransformEnd( true );
			SetUnitReForm();
			if ( NULL != m_ptrTransformWeapon )
			{
				g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_ptrTransformWeapon.get() );
				m_ptrTransformWeapon.reset();
			}
		} break;

	case BTI_BUFF_SI_A_AHM_MEDITATION:
		{
#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 메디테이션 해제 기능 삭제
			m_bInMeditation = false;			/// 메디테이션 동작중 여부 해제
#else  BALANCE_ELEMENTAL_MASTER_20130117
			StartMeditation( false );
#endif  BALANCE_ELEMENTAL_MASTER_20130117
		} break;

	default:
		break;
	}
}

/** @function : ApplyRenderParamOnEmptyRenderParamByBuff
	@brief : 버프에 의한 RenderParam 값이 없을 경우 적용되는 RenderParam 처리
	@param : 변경사항이 적용될 렌더파람 구조체 포인터(pRenderParam_)
*/
/*virtual*/ void CX2GUArme_VioletMage::ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ )
{
#ifdef RIDING_SYSTEM
	if ( m_bPassDash )
		return;
#endif //RIDING_SYSTEM

	if( GetTransformTime() > 0 )
	{
		float fAlpha = GetTransformTime() / _CONST_AISHA_::MAGICAL_MAKEUP_TRANSFORMING_TIME;
		m_RenderParam.renderType			= CKTDGXRenderer::RT_CARTOON_FADE;
		m_RenderParam.color = D3DXCOLOR( 1.f, 1.f, 1.f, fAlpha );
	}
	else
	{
		CX2GameUnit::ApplyRenderParamOnEmptyRenderParamByBuff( pRenderParam_ );
	}
}

#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION
/** @function : AdaptAdvancedTeleportationBuff
	@brief : 공간도약 버프 적용
*/
void CX2GUArme_VioletMage::AdaptAdvancedTeleportationBuff()
{
	if ( 0 < m_iAdvancedTeleportationLv )
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillLevel 
			= GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADW_ADVANCED_TELEPORTATION, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const int iSkillLevel 
			= GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ADW_ADVANCED_TELEPORTATION );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if ( iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet 
				= GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_P_ADW_ADVANCED_TELEPORTATION );

			if ( NULL != pSkillTemplet )
				SetBuffFactorToGameUnit( pSkillTemplet, 0 );
		}
	}
}

#endif //SERV_ARME_DIMENSION_WITCH

#ifdef MODIFY_SET_DAMAGE_DATA
void CX2GUArme_VioletMage::SetDamageExceptionProcess( CX2DamageManager::DamageData* pDamageData_,OUT float& fPowerRate)
{
	CX2GUUser::SetDamageExceptionProcess(pDamageData_, fPowerRate);

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	if( null != pDamageData_->optrAttackerGameUnit && pDamageData_->optrAttackerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = static_cast<CX2GUUser*>( pDamageData_->optrAttackerGameUnit.GetObservable() );

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		int iSkillLevelFitness = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ABM_FITNESS, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
		int iSkillLevelFitness = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ABM_FITNESS );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if( iSkillLevelFitness > 0 )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const CX2SkillTree::SkillTemplet* pSkillTempletFitness = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ABM_FITNESS );

			if( NULL != pSkillTempletFitness && pDamageData_->damageType == CX2DamageManager::DT_PHYSIC )
			{
				pDamageData_->fForceDownValue = pDamageData_->fForceDownValue * 
					pSkillTempletFitness->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_RANGE_REL, iSkillLevelFitness );
				pDamageData_->fHitAddMP = pDamageData_->fHitAddMP * 
					pSkillTempletFitness->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillLevelFitness );
			}
	#else // UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTempletFitness = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ABM_FITNESS, iSkillLevelFitness );

			if( NULL != pSkillTempletFitness && pDamageData_->damageType == CX2DamageManager::DT_PHYSIC )
			{
				pDamageData_->fForceDownValue = pDamageData_->fForceDownValue * pSkillTempletFitness->GetSkillAbilityValue( CX2SkillTree::SA_FORCE_DOWN_RANGE_REL );
				pDamageData_->fHitAddMP = pDamageData_->fHitAddMP * pSkillTempletFitness->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL );
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	} 
#endif
}
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef RIDING_SYSTEM
/*virtual*/ bool CX2GUArme_VioletMage::CanRidingState()
{
	if ( GetNowStateID() == CX2GUUser::USI_WAIT || GetNowStateID() == CX2GUUser::USI_WALK || 
		GetNowStateID() == CX2GUUser::USI_DASH || GetNowStateID() == CX2GUUser::USI_DASH_END || 
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT || GetNowStateID() == CX2GUUser::USI_RIDING_WALK ||
		GetNowStateID() == CX2GUUser::USI_RIDING_WAIT_HABIT || GetNowStateID() == CX2GUUser::USI_RIDING_DASH ||
		GetNowStateID() == CX2GUUser::USI_RIDING_DASH_END || GetNowStateID() == CX2GUArme_VioletMage::AVSI_CHARGE_MP )
		return true;

	return false;
}
#endif //RIDING_SYSTEM

#ifdef UPGRADE_SKILL_SYSTEM_2013
#pragma region SI_A_AV_ICICLE_WAVE
void CX2GUArme_VioletMage::AVSI_A_AV_Icicle_Wave_Init()
{
	XSkinMeshReadyInBackground( L"WindWard_XMesh01.X" );
	XSkinMeshReadyInBackground( L"IceNova_Effect_Mesh.X" );
	TextureReadyInBackground( L"Wind_WindWard01.tga" );
	TextureReadyInBackground( L"crystal_Nova.tga" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"Ice_Particle01.dds" );
	TextureReadyInBackground( L"Ice_Crystal_Ring01.dds" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
}

void CX2GUArme_VioletMage::AVSI_A_AV_Icicle_Wave_FrameMove()
{		
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.24f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.24f ) == true && EventCheck( 0.24f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetPos();
		vPos.y += 70.f;

		float fLifeTimeRate = 1.f;
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AV_ICICLE_WAVE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			fLifeTimeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			fLifeTimeRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		}

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Aisha_Icicle_Wave", GetPowerRate(), vPos,
			GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y, false, -1.f, -1.f, fLifeTimeRate);
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_A_AV_Icicle_Wave_EventProcess()
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
	SKILL_CANCEL_AFTER( m_fSkillCancelAfter )
	DASH_CANCEL_AFTER( m_fDashCancelAfter )

	CommonEventProcess();
}
#pragma endregion 아이시클 웨이브

#pragma region SI_SA_AV_ICE_STORM
void CX2GUArme_VioletMage::AVSI_SA_AV_Ice_Storm_Init()
{
	XSkinMeshReadyInBackground( L"ICE_Storm01.x" );
	XSkinMeshReadyInBackground( L"ICE_Storm01_1.x" );
	XSkinMeshReadyInBackground( L"ICE_Storm02.x" );
	XSkinMeshReadyInBackground( L"ICE_Storm03.x" );
	XSkinMeshReadyInBackground( L"ICE_Storm04.x" );
	TextureReadyInBackground( L"ICE_Storm03.tga" );
	TextureReadyInBackground( L"ICE_Storm02.tga" );
	TextureReadyInBackground( L"GroundShockWave.dds" );
	TextureReadyInBackground( L"Ice_Particle01.dds" );
	TextureReadyInBackground( L"steam_BP.dds" );
	TextureReadyInBackground( L"GroundShockWave02.dds" );
	TextureReadyInBackground( L"Ice_Crystal_Ring01.dds" );
	TextureReadyInBackground( L"Particle_blur.dds" );
}

void CX2GUArme_VioletMage::AVSI_SA_AV_Ice_Storm_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.12f, 2 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.2f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.2f ) == true && EventCheck( 0.2f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 pos = GetPos();
		bool bHyper = false;
		if ( GetRemainHyperModeTime() > 0.f )
			bHyper = true;

		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_SA_AV_ICE_STORM", this, NULL, bHyper );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SA_AV_Ice_Storm_EventProcess()
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
#pragma endregion 아이스 스톰

#pragma region SI_SA_AHM_AQUA_SHOWER
void CX2GUArme_VioletMage::AVSI_A_AHM_Aqua_Shower_Init()
{
	XSkinMeshReadyInBackground( L"Arme_Fireballs.x" );
	XMeshReadyInBackground( L"Ring_Plane.y" );
	XMeshReadyInBackground( L"SA_AHM_AQUA_SHOWER.y" );
	TextureReadyInBackground( L"AHM_FREEZING_POINT_01.tga" );
	TextureReadyInBackground( L"ColorBallGray_Gear.tga" );
	TextureReadyInBackground( L"Arme_ICEballS2.tga" );
	TextureReadyInBackground( L"AQUA_SHOWER_WAVE.tga" );
	TextureReadyInBackground( L"Ring.dds" );
	TextureReadyInBackground( L"PlusHPMP.dds" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"aisha_active_heavyPress_energy.dds" );
	TextureReadyInBackground( L"PlusHPMP.dds" );
	TextureReadyInBackground( L"RingAlpha.dds" );
	TextureReadyInBackground( L"Ice_Particle01.dds" );
	TextureReadyInBackground( L"RS_New_Sword_ICE.dds" );
	TextureReadyInBackground( L"Steam_BP.dds" );
	TextureReadyInBackground( L"acce_userAvatar_201205_bubble.dds" );
}

void CX2GUArme_VioletMage::AVSI_A_AHM_Aqua_Shower_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.13f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim->EventTimerOneshot( 0.13f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if ( m_pXSkinAnim->EventTimer( 0.13f ) == true && EventCheck( 0.13f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_SA_AHM_AQUA_SHOWER", this, NULL );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_A_AHM_Aqua_Shower_EventProcess()
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
#pragma endregion 아쿠아 샤워

#pragma region SI_SA_AHM_LIGHTNING_SHOWER
void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_Init()
{
	XSkinMeshReadyInBackground( L"SA_AEM_CHAIN_LIGHTNING_P01.x" );
	XMeshReadyInBackground( L"Up_Light01_Mesh.y" );
	XMeshReadyInBackground( L"raven_powrUp_M_lightning02.y" );
	TextureReadyInBackground( L"LIGHTNING_SHOWER_Emblem.tga" );
	TextureReadyInBackground( L"LIGHTNING_SHOWER_Square.tga" );
	TextureReadyInBackground( L"Arme_LightningBolts2a2.tga" );
	TextureReadyInBackground( L"LIGHTNING_SHOWER_Square.tga" );
	TextureReadyInBackground( L"Colorballgray.dds" );
	TextureReadyInBackground( L"aisha_SA_MorningStar_circle.dds" );
	TextureReadyInBackground( L"aisha_C_DashComboXX_circle.dds" );
	TextureReadyInBackground( L"eve_thousandsOfStars_gearLight02.dds" );
	TextureReadyInBackground( L"Bust_Wolf_P02.dds" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"WhitePointSmall.dds" );
	TextureReadyInBackground( L"Lightning_Piercing01.dds" );
	TextureReadyInBackground( L"Arme_ICEballS2.dds" );
	TextureReadyInBackground( L"ColorBallRed.dds" );
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_StateStart()
{
	CommonStateStart();

	m_bIsOnSomethingStart =false;
	if ( m_FrameDataNow.unitCondition.bFootOnLine == true )
		m_bIsOnSomethingStart = true;

	m_bDisableGravity = true;
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 500 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );

	m_iElapsedFireCount	= 0;		/// 라이트닝 샤워 누적 소환 개수 초기화
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_FrameMove()				
{
	ChangeWorldColorByHyperMode();

	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.01f, 1 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.1f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.1f ) == true && EventCheck( 0.1f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_SA_AHM_LIGHTNING_SHOWER", this ); 
	}

	if( m_iElapsedFireCount >= m_iMaxFireCount )	/// 최대 발사 횟수를 넘겼다면, 패스
		return;

	const int fCheckTime = static_cast<int>( m_FrameDataNow.unitCondition.fStateTime / m_fFireGap );

	if( fCheckTime >= m_iElapsedFireCount )
	{
		++m_iElapsedFireCount;	/// 발사 카운트 증가

		/// 전체 범위 : -850 ~ 850, 1그룹 : -850 ~ 340, 2그룹 : -340  ~ 850		-> 1, 2 그룹을 반복하며 중앙을 겹치는 공격 범위 연산
#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
		float fRadius = static_cast<float>( ( GetRandomInt( m_iElapsedFireCount * 4 + 1 ) - 50 ) * 5 );

		if( 0 == m_iElapsedFireCount % 2 )
			fRadius += 250.f;
		else
			fRadius -= 250.f;

		if( true == GetIsRight() )		/// 방향에 따른 위치 보정
			fRadius += 250.f;
		else
			fRadius -= 250.f;

		D3DXVECTOR3 vDirVector = GetDirVector();

		D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
		D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0.f , 850.f, 0.f );
		const float MAGIC_TIME_RANGE = 1.f;

		D3DXVECTOR3 vRotation = GetRotateDegree();

		if( true == GetIsRight() )
		{
			vEffectStartPosition -= GetDirVector() * 500;
		}
		else
		{
			vEffectStartPosition += GetDirVector() * 500;
		}
#else //FIX_SKILL_BALANCE_AISHA_LENA
		float fRadius = static_cast<float>( ( GetRandomInt( m_iElapsedFireCount * 4 + 1 ) - 50 ) * 12 );

		if( 0 == m_iElapsedFireCount % 2 )
			fRadius += 250.f;
		else
			fRadius -= 250.f;

		if( true == GetIsRight() )		/// 방향에 따른 위치 보정
			fRadius += 500.f;
		else
			fRadius -= 500.f;

		D3DXVECTOR3 vDirVector = GetDirVector();

		D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
		D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0.f , 850.f, 0.f );
		const float MAGIC_TIME_RANGE = 1.f;

		D3DXVECTOR3 vRotation = GetRotateDegree();

		if( true == GetIsRight() )
		{
			vEffectStartPosition -= GetDirVector() * 900;
		}
		else
		{
			vEffectStartPosition += GetDirVector() * 900;
		}
#endif //FIX_SKILL_BALANCE_AISHA_LENA
		

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Lightning_Shower_LineCrash", GetPowerRate(), vEffectStartPosition,
			vRotation, vRotation, vEffectEndPosition.y );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

	if( pSeqArmeAirStepRight != NULL )
	{
		pSeqArmeAirStepRight->SetEmitRate(10,20); 
	}

	if( pSeqArmeAirStepLeft != NULL )
	{
		pSeqArmeAirStepLeft->SetEmitRate(10,20); 
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_EventProcess()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	}
	else if ( m_iElapsedFireCount >= m_iMaxFireCount )
	{
		StateChange( AVSI_SA_AHM_LIGHTNING_SHOWER_LOOP );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if ( m_pXSkinAnim->EventTimerOneshot( 1.5f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if ( m_pXSkinAnim->EventTimer( 1.5f ) == true && EventCheck( 1.5f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		m_bDisableGravity = false;
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_StateEnd()
{
	m_bDisableGravity = false;
	CommonStateEnd();
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_Loop_StateStart()
{
	CommonStateStart();

	SetNoDetonation( true );			/// 기폭 제한 설정

	m_iElapsedFireCount			= 0;	///	발사 카운트 초기화
	m_bDisableGravity			= true;	/// 중력 해제
	m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
	m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_Loop_FrameMove()
{
	ChangeWorldColorByHyperMode();


#ifdef FIXED_CHANGE_THE_LOOP_STATE_FORCE_WHEN_SHOCK_DEBUFF_HITED	// 메테오 샤워, 라이트닝 샤워 사용 중 쇼크 공격을 받았을 때 State 가 굳는 문제
	if ( m_ExtraDamagePack.m_EnchantShock.m_fTime > 0.0f ) 
	{
		m_ExtraDamagePack.m_EnchantShock.m_fTime = 0;			
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}
#endif // FIXED_CHANGE_THE_LOOP_STATE_FORCE_WHEN_HITED // 메테오 샤워, 라이트닝 샤워 사용 중 쇼크 공격을 받았을 때 State 가 굳는 문제



	if( m_iElapsedFireCount >= m_iMaxFireCountByLoopState )	/// 최대 발사 횟수를 넘겼다면, 패스
		return;

	const int fCheckTime = static_cast<int>( m_FrameDataNow.unitCondition.fStateTime / m_fFireGap );

	if( fCheckTime >= m_iElapsedFireCount )
	{
		++m_iElapsedFireCount;				/// 루프 카운트 증가

		if( false == FlushMp( m_fUseMPConsume ) )
			return;

		/// 전체 범위 : -850 ~ 850, 1그룹 : -850 ~ 340, 2그룹 : -340  ~ 850		-> 1, 2 그룹을 반복하며 중앙을 겹치는 공격 범위 연산
#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
		float fRadius = static_cast<float>( ( GetRandomInt( m_iElapsedFireCount * 4 + 1 ) - 50 ) * 5 );

		if( 0 == m_iElapsedFireCount % 2 )
			fRadius += 250.f;
		else
			fRadius -= 250.f;

		if( true == GetIsRight() )		/// 방향에 따른 위치 보정
			fRadius += 250.f;
		else
			fRadius -= 250.f;

		D3DXVECTOR3 vDirVector = GetDirVector();

		D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
		D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0.f , 850.f, 0.f );
		const float MAGIC_TIME_RANGE = 1.f;

		D3DXVECTOR3 vRotation = GetRotateDegree();

		if( true == GetIsRight() )
		{
			vEffectStartPosition -= GetDirVector() * 500;
		}
		else
		{
			vEffectStartPosition += GetDirVector() * 500;
		}
#else //FIX_SKILL_BALANCE_AISHA_LENA
		float fRadius = static_cast<float>( ( GetRandomInt( m_iElapsedFireCount * 4 + 1 ) - 50 ) * 12 );

		if( 0 == m_iElapsedFireCount % 2 )
			fRadius += 250.f;
		else
			fRadius -= 250.f;

		if( true == GetIsRight() )		/// 방향에 따른 위치 보정
			fRadius += 500.f;
		else
			fRadius -= 500.f;

		D3DXVECTOR3 vDirVector = GetDirVector();

		D3DXVECTOR3 vEffectEndPosition = GetPos() + vDirVector * fRadius;
		D3DXVECTOR3 vEffectStartPosition = vEffectEndPosition + D3DXVECTOR3( 0.f , 850.f, 0.f );
		const float MAGIC_TIME_RANGE = 1.f;
		
		D3DXVECTOR3 vRotation = GetRotateDegree();

		if( true == GetIsRight() )
		{
			vEffectStartPosition -= GetDirVector() * 900;
		}
		else
		{
			vEffectStartPosition += GetDirVector() * 900;
		}
#endif //FIX_SKILL_BALANCE_AISHA_LENA
		
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Lightning_Shower_LineCrash", GetPowerRate(), vEffectStartPosition, vRotation, vRotation, 
			vEffectEndPosition.y );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_Loop_EventProcess()
{
	if ( m_fUseMPConsume > GetNowMp() ||						/// 현재 MP가 메테오 소환 비용보다 적거나
		m_iElapsedFireCount >= m_iMaxFireCountByLoopState ||	/// 메테오 샤워를 소환하는데 사용한 총 누적 MP 소모수가 한계치를 넘거나
		0.f >= GetNowHp() ||									/// 피가 없거나
		( m_InputData.pureA != true && m_InputData.pureS != true && m_InputData.pureD != true && m_InputData.pureC != true &&	/// 스킬 키를 누르고 있지 않다면 종료
		m_InputData.pureQ != true && m_InputData.pureW != true && m_InputData.pureE != true && m_InputData.pureR != true ) )
	{
		StateChange( AVSI_SA_AHM_LIGHTNING_SHOWER_END );
		m_FrameDataFuture.syncData.position.y -= LINE_RADIUS * 1.5f;
	}

	CommonEventProcess();
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_Loop_StateEnd()
{
	m_bDisableGravity = false;		/// 중력 설정
	m_iElapsedFireCount = 0;		/// 발사 카운트 초기화

	CommonStateEnd();
	SetNoDetonation( false );		/// 기폭 제한 해제
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_End_EventProcess()
{
	if( true == IsOnSomethingFuture() )
		StateChange( USI_JUMP_LANDING );

	CommonEventProcess();
}

void CX2GUArme_VioletMage::AVSI_SA_AHM_Lightning_Shower_End_StateEnd()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepRight = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepRight );
	CKTDGParticleSystem::CParticleEventSequence* pSeqArmeAirStepLeft  = g_pX2Game->GetMinorParticle()->GetInstanceSequence( m_hSeqArmeAirStepLeft );

	if( pSeqArmeAirStepRight != NULL )
	{
		pSeqArmeAirStepRight->SetEmitRate(0,0); 
	}

	if( pSeqArmeAirStepLeft != NULL )
	{
		pSeqArmeAirStepLeft->SetEmitRate(0,0); 
	}

	CommonStateEnd();
}
#pragma endregion 라이트닝 샤워

#pragma region SI_SA_AEM_CHAIN_LIGHTNING
void CX2GUArme_VioletMage::AVSI_SA_AEM_Chain_Lightning_Init()
{
	XSkinMeshReadyInBackground( L"SA_AEM_CHAIN_LIGHTNING_P01.X" );
	TextureReadyInBackground( L"aisha_SA_MorningStar_circle.dds" );
	TextureReadyInBackground( L"aisha_C_DashComboXX_circle.dds" );
	TextureReadyInBackground( L"eve_thousandsOfStars_gearLight02.dds" );
	TextureReadyInBackground( L"Arme_LightningBolts2a2.tga" );
}
#ifdef ADD_MEMO_1ST_CLASS //김창한
void CX2GUArme_VioletMage::AVSI_SA_AEM_Chain_Lightning_StartFuture()
{
	CommonStateStartFuture();

	if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO17 ) == true && false == IsOnSomethingFuture())
	{
		m_bDisableGravity = true;
		m_PhysicParam.nowSpeed		= D3DXVECTOR2( 0, 0 );
		m_PhysicParam.passiveSpeed	= D3DXVECTOR2( -1, -1 );
	}
}
void CX2GUArme_VioletMage::AVSI_SA_AEM_Chain_Lightning_EndFuture()
{
	CommonStateEndFuture();

	m_bDisableGravity = false;
}
#endif //ADD_MEMO_1ST_CLASS
void CX2GUArme_VioletMage::AVSI_SA_AEM_Chain_Lightning_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy2_Lhand", 0.13f, 0 );

	// 효과
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.001f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.001f ) == true && EventCheck( 0.001f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO17) == true )
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_SA_AEM_CHAIN_LIGHTNING_MEMO", this );
		else
#endif //ADD_MEMO_1ST_CLASS
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_SA_AEM_CHAIN_LIGHTNING", this );
	}

	// 발사
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.55f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.55f ) == true && EventCheck( 0.55f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 vPos = GetWeaponBonePos( 0, L"TRACE_START0", 0 );
		bool bHyper = false;
		D3DXVECTOR3 vScale( GetScale(), GetScale(), GetScale() );
		
		if ( GetRemainHyperModeTime() > 0.f )
			bHyper = true;
				
		CX2DamageEffect::CEffect* pDamageEffect = g_pX2Game->GetDamageEffect()->CreateInstance( this, 
			L"DamageEffect_AHM_Chain_Lightning", GetPowerRate(), vPos, GetRotateDegree(), GetRotateDegree(), m_FrameDataNow.unitCondition.landPosition.y );

		if ( NULL != pDamageEffect )
		{
			bool bUserUnit = false;
			UidType gameUnitUID = -1;
			
			if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
				gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( GetTeam() ), vPos, D3DXVECTOR2(150, 300), bUserUnit );
			else
				gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( GetTeam() ), vPos, D3DXVECTOR2(150, 750), bUserUnit );

			if( bUserUnit == true )
			{
				pDamageEffect->SetLockOnUnitUID( gameUnitUID );
				pDamageEffect->SetLockOnRandomSeed( (int)GetRandomInt() );
			}
			else
			{
				pDamageEffect->SetLockOnNPCUID( static_cast<int>( gameUnitUID ) );
				pDamageEffect->SetLockOnRandomSeed( (int)GetRandomInt() );
			}
		}	
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SA_AEM_Chain_Lightning_EventProcess()
{
	if ( true == m_pXSkinAnimFuture->IsAnimationEnd() )
	{
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if ( false == IsOnSomethingFuture() )
		{
			StateChange( USI_JUMP_DOWN );
			m_FrameDataFuture.syncData.position.y -= LINE_RADIUS;
		}
		else
#endif //ADD_MEMO_1ST_CLASS
		StateChange( USI_WAIT );
		m_fCanNotAttackTime = 0.1f;
	} // if

#ifdef ADD_MEMO_1ST_CLASS //김창한
	if( false == m_bDisableGravity )
#endif //ADD_MEMO_1ST_CLASS
	{
		const float LOWER_BOUND_INPUT_TIME_CANCEL = 1.2f;

#ifdef SKILL_CANCEL_BY_HYPER_MODE // 김태환
		IF_SKILL_CANCEL_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		WALK_CANCEL_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		DASH_CANCEL_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
		{
			StateChange( USI_WALK );
		}
#else // SKILL_CANCEL_BY_HYPER_MODE
		if (  LOWER_BOUND_INPUT_TIME_CANCEL <= m_pXSkinAnimFuture->GetNowAnimationTime() )
		{		
			if ( true == SpecialAttackEventProcess() )
			{
			}
			else if ( CAN_WALK_CANCEL )
			{
				StateChange( USI_WALK );
			}
			else if ( CAN_DASH_CANCEL )
			{
				StateChangeDashIfPossible();
			}
			ELSE_IF_ARROW_ZXASDC_PRESSED_AFTER( LOWER_BOUND_INPUT_TIME_CANCEL )
			{
				StateChange( USI_WALK );
			}
		}
#endif //SKILL_CANCEL_BY_HYPER_MODE
	}

	CommonEventProcess();
}
#pragma endregion 체인 라이트닝

#pragma region SI_A_AVP_SUMMON_BAT_HEAVY
void CX2GUArme_VioletMage::AVSI_A_AVP_Summon_Bat_Heavy_Init()
{
	XSkinMeshReadyInBackground( L"AVP_SUMMON_BAT_MESH_EFFECT.X" );
	TextureReadyInBackground( L"AVP_Effect128.tga" );
}

void CX2GUArme_VioletMage::AVSI_A_AVP_Summon_Bat_Heavy_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{

		D3DXVECTOR3 vPos = GetPos();
		if( true == GetIsRight() )
			vPos += GetDirVector() * 100.f;
		else
			vPos -= GetDirVector() * 100.f;
		
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AVP_SUMMON_BAT_HEAVY_DOLL );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
			float fLifeTime = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
			
			bool bHyper = false;
			if ( GetRemainHyperModeTime() > 0.f )
				bHyper = true;

			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Summon_Bat_Heavy", this, NULL, bHyper, GetPowerRate(),
				fLifeTime );
		}
		
		vPos.y += 150.f;
		vPos += GetZVector() * -50.f; 
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_Summon_Smoke", this, NULL, false, -1.f, -1.f,
			D3DXVECTOR3( 1, 1, 1 ), true, vPos, GetRotateDegree(), GetDirVector() );
	}

	CommonFrameMove();
}
#pragma endregion 쁘띠 앙고르 - 헤비돌

#pragma region SI_SA_AVP_Phantom_Breathing_Dark_Hole
void CX2GUArme_VioletMage::AVSI_SA_AVP_Phantom_Breathing_Dark_Hole_Init()
{
	XSkinMeshReadyInBackground( L"Spin_Liner02.X" );
	XSkinMeshReadyInBackground( L"SA_AEM_CHAIN_LIGHTNING_P01.X" );
	TextureReadyInBackground( L"Spin_Liner02_Gray.tga" );
	TextureReadyInBackground( L"Arme_LightningBolts2a2.tga" );
	TextureReadyInBackground( L"Wind_WindWard01.tga" );
	TextureReadyInBackground( L"Condense_pulse01.dds" );
	TextureReadyInBackground( L"Arme_Ring2.dds" );
	TextureReadyInBackground( L"secretVelder_alchemystBoss_magicAttackC_circleLine.dds" );
	TextureReadyInBackground( L"Black_Circle01.dds" );
	TextureReadyInBackground( L"Arme01_Frost_Ring_LV2_effect_01.dds" );
}

void CX2GUArme_VioletMage::AVSI_SA_AVP_Phantom_Breathing_Dark_Hole_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Bip01_L_Finger1Nub", 0.04f, 0 );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if (m_pXSkinAnim->EventTimerOneshot( 0.25f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if (m_pXSkinAnim->EventTimer( 0.25f ) == true && EventCheck( 0.25f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_PHANTOM_BREATHING", this );
	}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    else if( m_pXSkinAnim->EventTimerOneshot( 0.815f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	else if( m_pXSkinAnim->EventTimer( 0.815f ) == true && EventCheck( 0.815f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		D3DXVECTOR3 degree = GetRotateDegree();
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 Dirvec = GetDirVector();

		Dirvec.y = 0;
		D3DXVec3Normalize( &Dirvec , &Dirvec );

		if ( GetIsRight() == true )
			pos += Dirvec*310.0f;
		else
		{
			degree.y += 180.0f;
			pos -= Dirvec*310.0f;
		}

		bool bHyper = false;
		if ( GetRemainHyperModeTime() > 0.f )
			bHyper = true;
		D3DXVECTOR3 vScale( GetScale(), GetScale(), GetScale() );

		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Phantom_Breathing_DarkHole_NoDamage", GetPowerRate(),
			pos, degree , degree, m_FrameDataNow.unitCondition.landPosition.y );
		
		CX2DamageEffect::CEffect* pDE = g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Phantom_Breathing_DarkHole_Damage", GetPowerRate(),
			pos, degree , degree ,m_FrameDataNow.unitCondition.landPosition.y );

		pos.y += 255;
		g_pX2Game->GetDamageEffect()->CreateInstance( this, L"DamageEffect_Phantom_Breathing_DarkHole_BlackHole", GetPowerRate(),
			pos, degree , degree ,m_FrameDataNow.unitCondition.landPosition.y );
		
		pos = GetBonePos( L"Bip01_L_Finger11" );
		g_pX2Game->GetMajorParticle()->CreateSequence( (CKTDGObject*) this,  L"Pa_AVP_PHANTOM_BREATHING_Finger", pos );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_SA_AVP_Phantom_Breathing_Dark_Hole_EventProcess()
{
	if( m_pXSkinAnimFuture->IsAnimationEnd() == true )
	{
		StateChange( USI_WAIT );
	}

	CommonEventProcess();
}
#pragma endregion 팬텀 브리딩 - 다크 홀

#pragma region Active Body Skill
void CX2GUArme_VioletMage::ProcessShiningBodySkill()
{
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AEM_SHINING_BODY );
	if( NULL != pSkillTemplet )
	{
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	}
}

void CX2GUArme_VioletMage::ProcessShadowBodySkill()
{
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_AVP_SHADOW_BODY );
	if( NULL != pSkillTemplet )
	{
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	}
}

void CX2GUArme_VioletMage::ProcessEnergeticBodySkill()
{
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADW_ENERGETIC_BODY );
	if( NULL != pSkillTemplet )
	{
		SetBuffFactorToGameUnit( pSkillTemplet, 0 );
	}
}
#pragma endregion 바디 시리즈

#pragma region SI_A_ABM_VITAL_DRAIN
void CX2GUArme_VioletMage::AVSI_A_ABM_Vital_Drain_Ready_Init()
{
	TextureReadyInBackground( L"aisha_active_guillotinePress_lightBlur.dds" );
}

void CX2GUArme_VioletMage::AVSI_A_ABM_Vital_Drain_Ready_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.47f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.47f ) == true && EventCheck( 0.47f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		float fDistance = 2000.f;
		m_iDrainEnergyTarget = -1;
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ABM_VITAL_DRAIN );
		if( NULL != pSkillTemplet )
		{
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			float fDistanceTemp = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_PROJECTILE_RANGE_REL, iSkillTempletLevel );
	
			fDistance = fDistanceTemp;

#ifdef ADD_MEMO_1ST_CLASS //김창한
			if( GetEqippedSkillMemo( CX2SkillTree::SMI_AISHA_MEMO26 ) == true )
				fDistance *= 1.5f;
#endif //ADD_MEMO_1ST_CLASS

			CX2GameUnit *pFrontNearestUnit = NULL;
			for( int i = 0; i < g_pX2Game->GetUnitNum(); ++i )
			{
				CX2GameUnit *pUnit = g_pX2Game->GetUnit( i );

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
					CX2GUNPC *pNpc = (CX2GUNPC*)pUnit;
					if( pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC )
						continue;

					fOffset = ( pNpc->GetOrgUnitSize().x + pNpc->GetOrgUnitSize().y ) / 4.f;
				}					

				D3DXVECTOR3 vDistance = pUnit->GetPos() - GetPos();

				float fDistanceUnit = D3DXVec3Length( &vDistance ) - fOffset;
				if( fDistance >= fDistanceUnit )
				{
					fDistance = fDistanceUnit;
					m_iDrainEnergyTarget = i;
				}
			}
		}

		if( m_iDrainEnergyTarget >= 0 )
		{	
			StateChange( AVSI_A_ABM_VITAL_DRAIN_SUCCESS, true );
		}
		else
		{
			StateChange( AVSI_A_ABM_VITAL_DRAIN_FAIL, true );
		}
	}	
	else if( m_iDrainEnergyTarget < 0 && m_pXSkinAnim->IsAnimationEnd() == true )
	{
		StateChange( AVSI_A_ABM_VITAL_DRAIN_FAIL, true );
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_A_ABM_Vital_Drain_Success_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.3f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.3f ) == true && EventCheck( 0.3f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		if( m_iDrainEnergyTarget >= 0 )
		{
			CX2GameUnit *pUnit = g_pX2Game->GetUnit( m_iDrainEnergyTarget );

			if( pUnit != NULL && g_pX2Game->IsValidUnit( pUnit ) == true )
			{					
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ABM_VITAL_DRAIN );
				if( NULL != pSkillTemplet )
				{
					if ( NULL == GetUnit() )
						return;
	
					const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
					const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
					float fDrainRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_INTAKE_ABS, iSkillTempletLevel );
					
					m_DamageData.optrAttackerGameUnit	= this;
					m_DamageData.impactPoint			= pUnit->GetPos();
					m_DamageData.optrDefenderGameUnit	= pUnit;
					m_DamageData.reActResult			= m_DamageData.reActType;
					m_DamageData.m_fDrainHpRate			= fDrainRate;
					m_DamageData.m_fDrainMpByReact		= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillTempletLevel );
					m_DamageData.m_fDrainMpRate			= fDrainRate;

					pUnit->DamageReact( &m_DamageData );
					// 공격자 이펙트
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_EnegryDrain01", this );
					// 피격자 이펙트
					g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Aisha_ABM_EnegryDrain02", pUnit, pUnit );						
				}					
			}

			m_iDrainEnergyTarget = -1;
		}
	}

	CommonFrameMove();
}

void CX2GUArme_VioletMage::AVSI_A_ABM_Vital_Drain_EventProcess()
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
	else if ( CAN_WALK_CANCEL && m_pXSkinAnimFuture->GetNowAnimationTime() >= 0.451f )
	{			
		StateChange( USI_WALK );
	}

	CommonEventProcess();
}
#pragma endregion 바이탈 드레인

#pragma region SI_A_ADW_Worm_Hole
void CX2GUArme_VioletMage::AVSI_A_ADW_Worm_Hole_Init()
{
	XSkinMeshReadyInBackground( L"DummyAttackBox_50x50x50.X" );
	TextureReadyInBackground( L"Arme_Aging_Gate01.tga" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
}

void CX2GUArme_VioletMage::AVSI_A_ADW_Worm_Hole_StateStart()
{
	CommonStateStart();

	g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ADW_DISTORTION_WEAPON", this );
}


void CX2GUArme_VioletMage::AVSI_A_ADW_Worm_Hole_FrameMove()
{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if( m_pXSkinAnim->EventTimerOneshot( 0.4f ) )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_pXSkinAnim->EventTimer( 0.4f ) == true && EventCheck( 0.4f, false ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		const CX2SkillTree::SkillTemplet* pSkillTemplet = GetEquippedActiveSkillTemplet( CX2SkillTree::SI_A_ADW_WORM_HOLE );
		if( NULL != pSkillTemplet )
		{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL == GetUnit() )
				return;
	
			const CX2UserSkillTree& userSkillTree = GetUnit()->GetUnitData().m_UserSkillTree;
	
			const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
			float fEffectTime = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
			float fEffectTime = (float)pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_EFFECTIVE_TIME );
	#endif // UPGRADE_SKILL_SYSTEM_2013

			D3DXVECTOR3 vPos = GetBonePos( L"Bip01_Footsteps" );

			if( true == GetIsRight() )
				vPos += GetDirVector() * 150.f * GetScaleByUnit().x;
			else
				vPos -= GetDirVector() * 150.f * GetScaleByUnit().x;
			
			bool bHyper = false;
			if ( GetRemainHyperModeTime() > 0.f )
				bHyper = true;
			D3DXVECTOR3 vScale( GetScale(), GetScale(), GetScale() );

			g_pX2Game->GetEffectSet()->PlayEffectSet( L"Effectset_A_ADW_WORM_HOLE", this, NULL, bHyper, GetPowerRate(),
				fEffectTime, vScale, true, vPos );
		}
	}

	CommonFrameMove();
}
#pragma endregion 웜홀

#pragma region SI_SA_ADW_SCREWDRIVER_DRILLER
void CX2GUArme_VioletMage::AVSI_SI_SA_ADW_Screwdriver_Driller_Init()
{
	XSkinMeshReadyInBackground( L"aisha_SA_SCREWDRIVER_AttackSphere.X" );
	XSkinMeshReadyInBackground( L"aisha_SA_SCREWDRIVER_circle.X" );
	XSkinMeshReadyInBackground( L"aisha_SA_SCREWDRIVER_screw.X" );
	XSkinMeshReadyInBackground( L"elsword_SA_SwordBlastiong_slash.X" );
	XSkinMeshReadyInBackground( L"aisha_SA_SCREWDRIVER_weaponCircle.X" );
	TextureReadyInBackground( L"aisha_SA_SCREWDRIVER_screw.dds" );
	TextureReadyInBackground( L"aisha_SA_SCREWDRIVER_circle.dds" );
	TextureReadyInBackground( L"aisha_SA_SCREWDRIVER_screw.dds" );
	TextureReadyInBackground( L"elsword_SA_SwordBlastiong_slash.dds" );
	TextureReadyInBackground( L"Mesh_Common_AC_Face1_81160_Sorted_P02.dds" );
	TextureReadyInBackground( L"Explosion_Sphere.dds" );
	TextureReadyInBackground( L"WhitePoint.dds" );
	TextureReadyInBackground( L"aisha_SA_SCREWDRIVER_weaponCircle.dds" );
	TextureReadyInBackground( L"soket_earth_quake_smoke02.dds" );
	TextureReadyInBackground( L"stone.dds" );
	TextureReadyInBackground( L"rena_A_fatality_moveSpark.dds" );
}

void CX2GUArme_VioletMage::AVSI_SI_SA_ADW_Screwdriver_Driller_FrameMove()
{
	ChangeWorldColorByHyperMode();
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", 0.01f, 0 );

	CommonFrameMove();
}
#pragma endregion 스크류 드라이버 - 드릴러
#endif //UPGRADE_SKILL_SYSTEM_2013_JHKang

#ifdef FINALITY_SKILL_SYSTEM //JHKang
#pragma region SI_SA_AEM_Elemental_Storm
void CX2GUArme_VioletMage::AVSI_HA_AEM_Elemental_Storm_Init()
{
	TextureReadyInBackground(L"AHM_FREEZING_POINT_01.tga");
	TextureReadyInBackground(L"Arme_ring2.dds");
	TextureReadyInBackground(L"Bust_Wolf_P02.dds");
	TextureReadyInBackground(L"CDC_Bullet_01.dds");
	TextureReadyInBackground(L"ColorBallFire.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"EL_FIRE_03.dds");
	TextureReadyInBackground(L"EL_FIRE_05.dds");
	TextureReadyInBackground(L"EL_FIRE_06.dds");
	TextureReadyInBackground(L"EL_FIRE_09.dds");
	XSkinMeshReadyInBackground(L"Elsword_SI_SA_Sonic_Blade_Mesh01.X");
	TextureReadyInBackground(L"Explosion_Sphere.dds");
	TextureReadyInBackground(L"Fire_Flower01.dds");
	TextureReadyInBackground(L"GroundShockWave.dds");
	TextureReadyInBackground(L"GroundShockWave02.dds");
	XSkinMeshReadyInBackground(L"GustStorm01.X");
	XSkinMeshReadyInBackground(L"GustStorm01_1.X");
	XSkinMeshReadyInBackground(L"GustStorm02.X");
	XSkinMeshReadyInBackground(L"GustStorm03.X");
	XSkinMeshReadyInBackground(L"GustStorm04.X");
	XSkinMeshReadyInBackground(L"ICE_Storm02.X");
	XSkinMeshReadyInBackground(L"ICE_Storm03.X");
	XSkinMeshReadyInBackground(L"ICE_Storm04.X");
	TextureReadyInBackground(L"Ice_Crystal_Ring01.dds");
	TextureReadyInBackground(L"Ice_Particle01.dds");
	TextureReadyInBackground(L"LIGHTNING_SHOWER_Emblem.tga");
	TextureReadyInBackground(L"LIGHTNING_SHOWER_Square.tga");
	XSkinMeshReadyInBackground(L"Lightning01.X");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"Particle_blur.dds");
	XSkinMeshReadyInBackground(L"SA_AEM_ELEMENTAL_STORM_FIRE01.X");
	XSkinMeshReadyInBackground(L"SA_AEM_ELEMENTAL_STORM_FIRE02.X");
	XSkinMeshReadyInBackground(L"SA_AEM_ELEMENTAL_STORM_ICE.X");
	XSkinMeshReadyInBackground(L"SA_AEM_ELEMENTAL_STORM_ICE_DUMMY.X");
	TextureReadyInBackground(L"SA_EL_MEGA_SLASH_FIRE04.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	XSkinMeshReadyInBackground(L"aisha_active_energySpurt_circle.X");
	TextureReadyInBackground(L"soket_wind_stom_wind01.dds");
	TextureReadyInBackground(L"steam_BP.dds");
}
#pragma endregion 엘리멘탈 스톰 : 궁극기

#pragma region SI_FS_AVP_Abyss_Angor
void CX2GUArme_VioletMage::AVSI_HA_AVP_Abyss_Angor_Init()
{
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Aging_Gate01.tga");
	TextureReadyInBackground(L"Arme_Critical2.dds");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Condense_Light01.dds");
	TextureReadyInBackground(L"Condense_Pulse01.dds");
	TextureReadyInBackground(L"Condense_Pulse02.dds");
	TextureReadyInBackground(L"Explosion_Sphere.dds");
	TextureReadyInBackground(L"Gear_Water.tga");
	TextureReadyInBackground(L"Inspector_State_Shield.tga");
	TextureReadyInBackground(L"Lire_Kick_Impact01.dds");
	TextureReadyInBackground(L"Particle_Blur.dds");
	TextureReadyInBackground(L"Sprriggan_Light_Ball_01.Tga");
	TextureReadyInBackground(L"Steam_Bp.dds");
	TextureReadyInBackground(L"WhiteImage.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"blackImage.dds");
	TextureReadyInBackground(L"secretVelder_Unohound_fire.dds");
	TextureReadyInBackground(L"secretVelder_alchemystBoss_magicAttackE_light.dds");
	TextureReadyInBackground(L"smoke.dds");
	XSkinMeshReadyInBackground(L"Dullahan_Special_Laser_Mesh01.X");
	XSkinMeshReadyInBackground(L"Lire_SI_SA_Gungnir_Mesh05.X");
	XSkinMeshReadyInBackground(L"Mirror_Left_light.X");
	XSkinMeshReadyInBackground(L"NEPHILIM_AttackB_02.X");
	XSkinMeshReadyInBackground(L"SA_AVP_ABYSS_ANGOR_ANGOR.X");
	XSkinMeshReadyInBackground(L"Taranvash_SpecialAttackA_Mesh01.X");
}
#pragma endregion 어비스 앙고르 : 궁극기

#pragma region SI_FS_ADW_Fate_space
void CX2GUArme_VioletMage::AVSI_HA_ADW_Fate_space_Init()
{
	TextureReadyInBackground(L"AeroTornado04.dds");
	TextureReadyInBackground(L"Arme_Ring2.DDS");
	TextureReadyInBackground(L"Arme_Ring2.dds");
	TextureReadyInBackground(L"CenterLight_Gray01.dds");
	TextureReadyInBackground(L"Colorballgray.dds");
	TextureReadyInBackground(L"Explosion_Sphere.dds");
	TextureReadyInBackground(L"Sudden_Buster_P05.dds");
	TextureReadyInBackground(L"WhitePoint.dds");
	TextureReadyInBackground(L"WhitePointSmall.dds");
	TextureReadyInBackground(L"aisha_active_guillotinePress_lightBlur.dds");
	TextureReadyInBackground(L"eve_A_blink_light.DDS");
	TextureReadyInBackground(L"secretVelder_Unohound_fire.dds");
}
#pragma endregion 페이트 스페이스 : 궁극기
#endif //FINALITY_SKILL_SYSTEM

/*virtual*/ void CX2GUArme_VioletMage::ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ )
{
	ShowActiveSkillCutInAndLight( L"Dummy1_Rhand", fTimeToShow_, m_iSkillCutInSetSubIndex, bOnlyLight_ );
}

#ifdef MODIFY_RIDING_PET_AWAKE

//AVSI_HYPER_MODE
void CX2GUArme_VioletMage::RidingHyperModeInit()
{
	HyperModeInit();
}

void CX2GUArme_VioletMage::RidingHyperModeFrameMove()
{
	CommonHyperModeFrameMove( 0.f, 0.1f, true );
}

void CX2GUArme_VioletMage::CommonHyperModeFrameMove( float fTime1_, float fTime2_, bool bSound_ /*= false*/  )
{
	g_pX2Game->GetWorld()->SetWorldColor( 0xff222222 );

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
	if( m_pXSkinAnim->EventTimer( fTime2_ ) == true && EventCheck( fTime2_ , false) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	{
		ShowMinorParticleHyperModeShock();

		if( true == bSound_ )
		{
			PlaySound( L"Energy.ogg" );
			PlaySound( L"Break.ogg" );
			PlaySound( L"Change.ogg" );
		}

		UpDownCrashCamera( 20.0f, 0.3f );
		g_pKTDXApp->GetDGManager()->ClearScreen();

		ShowMinorParticleHyperModeTrace();
		ApplyHyperModeBuff();

		//{{ 김상훈 2010.10.29 : 극한의 마나 운용
#ifdef NEW_SKILL_2010_11
#ifdef HYPER_MODE_FIX
		m_fRateLimitManaManagement = GetLastStateHyperModeCount() * 0.33f;
#else
		m_fRateLimitManaManagement = m_FrameDataNow.syncData.m_HyperModeCount * 0.33f;
#endif HYPER_MODE_FIX
		float rdf = GetRandomFloat();
		if ( rdf <= m_fRateLimitManaManagement )
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT, true  );
#else // UPGRADE_SKILL_SYSTEM_2013
			int iSkillLevel = GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT );
#endif // UPGRADE_SKILL_SYSTEM_2013

			if( iSkillLevel > 0 )
			{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT );

				if( NULL != pSkillTemplet )
				{
					m_iHyperChargeMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_RECOVER_MP_ABS, iSkillLevel );
					m_iHyperAddMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_MP_ABS, iSkillLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_AVP_LIMITED_MANA_MANAGEMENT, iSkillLevel );

				if( NULL != pSkillTemplet )
				{
					m_iHyperChargeMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_RECOVER_MP_ABS );
					m_iHyperAddMP = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MAX_MP_ABS );
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef FIX_LIMITED_MANA_MANAGEMENT
					SetBuffFactorToGameUnit( pSkillTemplet, 0 );
#else
					SetLimitManaManagement( true );
					ResetMaxMP();
#endif //FIX_LIMITED_MANA_MANAGEMENT
					//GetGageManager()->UpdateMyManaGuageUI();
					UpNowMp( m_iHyperChargeMP );



					std::wstringstream strStream;
					std::wstringstream strStream2;
					strStream << L"MAXMP+ "<< static_cast<int>(m_iHyperAddMP);
					D3DXVECTOR3 pos = GetPos();
					pos.y += 50.0f; 
					if( NULL != g_pData->GetPicCharBlue() )
						g_pData->GetPicCharBlue()->DrawText( strStream.str().c_str(), pos, GetDirVector(), CKTDGPicChar::AT_CENTER );


					pos.y += 30.0f; 
					strStream2 <<  "MP+ " << static_cast< int >( m_iHyperChargeMP );
					if( NULL != g_pData->GetPicCharBlue() )
						g_pData->GetPicCharBlue()->DrawText( strStream2.str().c_str(), pos, GetDirVector(), CKTDGPicChar::AT_CENTER );
				}
			}
		}
#endif NEW_SKILL_2010_11
		//}} 김상훈 2010.10.29 : 극한의 마나 운용
	}

	CommonFrameMove();	
}

void CX2GUArme_VioletMage::RidingHyperModeCameraMove()
{
	HyperModeCameraMove();
}
#endif // MODIFY_RIDING_PET_AWAKE

#ifdef FIX_NO_STATE_SKILL_BUG
/** @function : IsValideSkillRidingOn
	@brief : 탈 것 탑승중에 사용 할 수 있는 스킬인지 체크.
	@return : 사용 가능 : true 사용 불가 falle
*/
/*virtual*/ bool CX2GUArme_VioletMage::IsValideSkillRidingOn( CX2SkillTree::SKILL_ID eSkill_ID_ )
{
	if( true == GetRidingOn() )
	{
		switch( eSkill_ID_ ) 
		{
		case CX2SkillTree::SI_A_AV_MANA_SHIELD: // 마나 실드
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

#ifdef ADD_MEMO_1ST_CLASS //김창한
const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION CX2GUArme_VioletMage::GetSkillUseCondition(const CX2SkillTree::SkillTemplet* pSkillTemplet_)
{
	CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition = pSkillTemplet_->m_eActiveSkillUseCondtion;

	switch (pSkillTemplet_->m_eID)
	{
	case CX2SkillTree::SI_SA_AHM_CHAIN_LIGHTNING:
		{
			if( GetEqippedSkillMemo(CX2SkillTree::SMI_AISHA_MEMO17) == true )
				eActiveSkillUseCondition = CX2SkillTree::ASUT_AIR;
		} break;
	}


	return eActiveSkillUseCondition;
}
#endif //ADD_MEMO_1ST_CLASS
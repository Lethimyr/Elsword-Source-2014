#include "StdAfx.h"

#ifdef SERV_PET_SYSTEM

//#include ".\x2pet.h"
//#include ".\x2petai.h"

CX2PET::CX2PET() : 
#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	m_LuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true ), 
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	m_optrMasterGameUnit(), 
	m_bOnPopUp( false ), 
	m_TimerAuraSuspended( 0.0f )
{
#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
	m_ScriptFileName			= L"";
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
	m_wstrMotionName			= L"";
	m_wstrChangeTex				= L"";
	m_wstrAniTex				= L"";

	m_pMotion					= NULL;
	m_pXSkinAnim				= NULL;
	m_pAniXET					= NULL;
	m_pChangeTexXET				= NULL;
	
	m_AIMessage					= SCT_NO_CHANGE;
	m_UnUseAIMessage			= SCT_NO_CHANGE;

	m_bAIEnabled				= true;

	m_petCondition.Init();
	m_petSyncData.Init();

	m_MasterUid					= -1;
	m_pAI						= NULL;	

	m_bOldLoaded				= false;
	m_bLoaded					= false;

	m_fScale					= 1.3f;
	m_fLostRange				= 2000.f;

	m_fExtraJumpSpeed			= 0.f;
	m_bGame						= false;

	m_PreState					= 0;
	m_bInit						= false;

	m_bAllowDirChange			= true;
	m_bSendReserveStateChange	= false;

	m_fComboIntervalTime		= 1.3f;
	m_fTimeAttack				= 0.f;
	m_fTimeDefence				= 0.f;
	m_AttackComboNum			= 0;
	m_DefenceComboNum			= 0;	

	m_bEvolution				= false;
	m_bMyPet					= false;
	m_bInitPos					= false;
	m_bAttacking				= false;
	m_bAttackTrace				= false;
	m_fAttackTraceTime			= 0.f;

	m_fNowMp					= 0.f;
	m_fNowStateTimer			= 0.f;

	m_pDlgPetGage			= NULL;	

	m_vecSoundPlayData.resize(0);

	m_fIconTimer = 0.f;
	m_hEffectCheer = INVALID_EFFECTSET_HANDLE;
	m_hStateEffect = INVALID_EFFECTSET_HANDLE;
	m_hSeqIcon	= INVALID_PARTICLE_SEQUENCE_HANDLE;

#ifdef PET_ATTACH_PARTICLE
	m_vecSeqPetParticle.resize(0);
#endif //PET_ATTACH_PARTICLE

	m_bUnderWaterHead = false;
	m_bForceChagneColor = false;
	m_cLineUnitColor = D3DXCOLOR( 0.f, 0.f, 0.f, 0.f );

#ifdef GHOST_PET
	m_bForceColor = false;	
	
#endif

	m_bFlyUnit = false;

	m_AfraidState		= PSI_NONE;
	m_AttackState		= PSI_NONE;
	m_CheerState		= PSI_NONE;
	m_Cute1State		= PSI_NONE;
	m_Cute2State		= PSI_NONE;
	m_EatState			= PSI_NONE;
	m_Happy1State		= PSI_NONE;
	m_Happy2State		= PSI_NONE;
	m_HelloState		= PSI_NONE;
	m_HungryState		= PSI_NONE;
	m_NotKnowState		= PSI_NONE;
	m_SadState			= PSI_NONE;
	m_SitEndState		= PSI_NONE;
	m_SitFallState		= PSI_NONE;
	m_SitLoopState		= PSI_NONE;
	m_SitStartState		= PSI_NONE;
	m_SleepEndState		= PSI_NONE;
	m_SleepFallState	= PSI_NONE;
	m_SleepLoopState	= PSI_NONE;
	m_SleepStartState	= PSI_NONE;
	m_SpecialState		= PSI_NONE;
	m_StartState		= PSI_NONE;
	m_StuffedState		= PSI_NONE;
	m_WaitHabitState	= PSI_NONE;
	m_WaitState			= PSI_NONE;

	m_bAIIsRight		= false;
	m_bCheat			= false;
	m_bFixFlyHeight		= false;
	m_bForceBillBoard	= false;
	m_bLandConnect		= false;
	m_bSetData			= false;

	m_fElapsedTime		= 0.f;

	m_pDamageData = NULL;

#ifdef ADD_2013_CHRISTMAS_PET // 김태환
	for( int i = 0; i < 10; i++ )
	{
	#ifdef SET_IN_PET_LUA
		m_FlagList[i]		= false;
		m_IntList[i]		= 0;
		INIT_VECTOR3( m_VecList[i], 0, 0, 0 ); 
		m_NumberList[i]     = 0.;
	#endif SET_IN_PET_LUA

		m_hEffectSetHandle[i] = INVALID_EFFECTSET_HANDLE;
	}
#endif // ADD_2013_CHRISTMAS_PET
}

CX2PET::~CX2PET()
{	
	if( GetLoaded() == true )
	{
		SAFE_DELETE_KTDGOBJECT( m_pXSkinAnim );	
		m_pXSkinAnim = NULL;
		SAFE_CLOSE( m_pMotion );
	}
	
	SAFE_CLOSE( m_pAniXET );
	SAFE_CLOSE( m_pChangeTexXET );

	SAFE_DELETE( m_pAI );

	SAFE_DELETE_DIALOG_HANDLE( m_pDlgPetGage );

#ifdef CLOSE_SOUND_TEST
	m_SoundCloseManager.CloseSound();
#endif CLOSE_SOUND_TEST

	if( g_pData != NULL && g_pData->GetUIEffectSet() != NULL && m_hEffectCheer != INVALID_EFFECTSET_HANDLE )
		g_pData->GetUIEffectSet()->StopEffectSet( m_hEffectCheer );

	if( g_pData != NULL && g_pData->GetUIEffectSet() != NULL && m_hStateEffect != INVALID_EFFECTSET_HANDLE )
		g_pData->GetUIEffectSet()->StopEffectSet( m_hStateEffect );
	

	if( g_pData != NULL && g_pData->GetUIMajorParticle() != NULL )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqIcon );	

#ifdef ADD_2013_CHRISTMAS_PET // 김태환
	for( int i = 0; i < 10; i++ )
	{
		if ( m_hEffectSetHandle[i] != INVALID_EFFECTSET_HANDLE )
			g_pX2Game->GetEffectSet()->StopEffectSet( m_hEffectSetHandle[i] );
	}
#endif // ADD_2013_CHRISTMAS_PET

#ifdef PET_ATTACH_PARTICLE
	if( g_pData != NULL && g_pData->GetUIMajorParticle() != NULL )
	{
		BOOST_TEST_FOREACH( AttachedPetParticleData, kSeq, m_vecSeqPetParticle )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( kSeq.hSeq );
		}
	}
#endif //PET_ATTACH_PARTICLE
}

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
void CX2PET::SetPet( lua_State* pPetLuaState, UidType masterUid )
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
void CX2PET::SetPet( wstring wstrPetScriptName, UidType masterUid )
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
{
	m_bGame = false;

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	if ( pPetLuaState != NULL )
		m_LuaManager.AssignNewLuaState( pPetLuaState, 0, false );
	else
		m_LuaManager.AssignNewLuaState( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	m_ScriptFileName = wstrPetScriptName;
	g_pKTDXApp->LoadLuaTinker( m_ScriptFileName.c_str() );
	if ( g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, m_ScriptFileName.c_str() ) == false )
	{
		ASSERT( !"gameunit lua script Parsing failed" );
	}
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	m_MasterUid = masterUid;

	if( m_pAI != NULL )
		SAFE_DELETE( m_pAI );

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->GetUID() == m_MasterUid )
	{
		m_bMyPet = true;
		m_fNowMp = g_pData->GetPetManager()->GetSummonedPetMp(); 
	}

	m_petCondition.Init();
	m_petSyncData.Init();

	InitSystem();
	InitDevice();
	InitMotion();
	InitPhysic();
	InitState();	

	if( GetLoaded() == false && m_pXSkinAnim == NULL )
	{		
		m_pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMeshInBackground( m_wstrMotionName.c_str() );
		//background!!!
// 		if( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() == true )
// 		{
// 			HRESULT hr = g_pKTDXApp->GetDeviceManager()->ThreadReq_OpenDevice(
// 				this, m_wstrMotionName, CKTDXDeviceManager::THREAD_REQUEST_UNKNOWN );
// 			ASSERT( SUCCEEDED(hr) );
// 		}
	}

#ifdef PET_ATTACH_PARTICLE
	if( NULL != g_pData &&
		NULL != g_pData->GetUIMajorParticle() )	// 아직 메쉬 로드 안되서 일단 0,0,0 에 세팅
	{
		std::vector< AttachedPetParticleData >::iterator vit;
		for( vit = m_vecSeqPetParticle.begin(); vit != m_vecSeqPetParticle.end(); ++vit )
		{
			D3DXVECTOR3 vPos = GetBonePos( vit->wstrBoneName.c_str() );
			vit->hSeq = g_pData->GetUIMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  vit->wstrParticleName.c_str(), vPos, 3.f, 3.f );		
		}
	}
#endif //PET_ATTACH_PARTICLE
	
	m_pAI = new CX2PETAI( this );
	if( NULL != m_pAI )
	{
		m_pAI->SetMasterUnitData();
		m_pAI->LoadAIDataFromLUA( m_LuaManager );

		m_fLostRange = (float)m_pAI->GetLostRange();

		if( true == m_pAI->GetAIFly())
		{
			m_OrgPhysicParam.fReloadAccel = 1400.f;
			m_ScriptedPhysicParam.fReloadAccel = 1400.f;
			m_PhysicParam.fReloadAccel = 1400.f;
			m_bFlyUnit = true;
		}
	}
	m_bSetData = false;
}


#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
void CX2PET::SetGamePet( lua_State* pPetLuaState, UidType masterUid )
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
void CX2PET::SetGamePet( wstring wstrPetScriptName, UidType masterUid )
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD	
{
	m_bGame = true;

#if 0
	m_ScriptFileName = wstrPetScriptName;
	g_pKTDXApp->LoadLuaTinker( m_ScriptFileName.c_str() );
	if ( g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, m_ScriptFileName.c_str() ) == false )
	{
		ASSERT( !"gameunit lua script Parsing failed" );
	}

	m_MasterUid = masterUid;

	if( g_pX2Game != NULL )
	{
		m_pMasterGame = g_pX2Game->GetUserUnitByUID( m_MasterUid );
	}

	if( m_pAI != NULL )
		SAFE_DELETE( m_pAI );

	m_petCondition.Init();
	m_petSyncData.Init();

	InitSystem();
	InitMotion();
	InitPhysic();
	InitState();	

	if( m_bLoaded == false && m_pXSkinAnim == NULL )
	{		

		if( g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() == true )
		{
			HRESULT hr = g_pKTDXApp->GetDeviceManager()->ThreadReq_OpenDevice(
				this, m_wstrMotionName, CKTDXDeviceManager::THREAD_REQUEST_UNKNOWN );
			ASSERT( SUCCEEDED(hr) );
		}
	}

	m_pAI = new CX2PETAI( this );
	if( NULL != m_pAI )
	{
		m_pAI->SetMasterUnitData();
		m_pAI->LoadAIDataFromLUA( m_LuaManager );
	}
#endif

#if 1

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	if ( pPetLuaState != NULL )
		m_LuaManager.AssignNewLuaState( pPetLuaState, 0, false );
	else
		m_LuaManager.AssignNewLuaState( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	m_ScriptFileName = wstrPetScriptName;
	g_pKTDXApp->LoadLuaTinker( m_ScriptFileName.c_str() );
	if ( g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, m_ScriptFileName.c_str() ) == false )
	{
		ASSERT( !"gameunit lua script Parsing failed" );
	}
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	m_MasterUid = masterUid;

	if( g_pX2Game != NULL )
	{
		m_optrMasterGameUnit = g_pX2Game->GetUserUnitByUID( m_MasterUid );
	}

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->GetUID() == m_MasterUid )
	{
		m_bMyPet = true;
		m_fNowMp = g_pData->GetPetManager()->GetSummonedPetMp(); 
	}

	if( m_pAI != NULL )
		SAFE_DELETE( m_pAI );

	m_petCondition.Init();
	m_petSyncData.Init();

	InitSystem();
#ifdef READY_PET_DEVICES_IN_DUNGEON
	InitDevice();
#endif READY_PET_DEVICES_IN_DUNGEON
	InitMotion();
	InitPhysic();
	InitState();	

	if( GetLoaded() == false && m_pXSkinAnim == NULL )
	{
		if( m_pMotion == NULL && m_wstrMotionName != L"")
		{
			//m_pMotion			= g_pKTDXApp->GetDeviceManager()->OpenXSkinMeshInBackground( m_wstrMotionName.c_str() );
			m_pMotion			= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_wstrMotionName.c_str() );
			ASSERT( m_pMotion );
			if ( m_pMotion == NULL )
			{
#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				StateLog( m_ScriptFileName.c_str() );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				StateLog( m_wstrMotionName.c_str() );
				//MessageBox( g_pKTDXApp->GetHWND(), motion.c_str(), L"모션 파일 읽어오기 실패!", MB_OK );
			}
		}

		//*
		if( m_pMotion != NULL )
		{
			//SAFE_CLOSE( m_pChangeTexXET );
			//m_pChangeTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_wstrChangeTex.c_str() );
			//SAFE_CLOSE( m_pAniXET );
			//m_pAniXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_wstrAniTex.c_str() );

			//m_pXSkinAnim.reset();
			m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
			m_pXSkinAnim->SetAnimXSkinMesh( m_pMotion );
			m_pXSkinAnim->AddModelXSkinMesh( m_pMotion, m_pAniXET, NULL, m_pChangeTexXET );
			m_pXSkinAnim->SetModelDetailPercent( g_pData->GetModelDetailPercent() );			

			m_pXSkinAnim->SetShowObject(false);

			SetCenter( m_pXSkinAnim->GetAnimXSkinMesh()->GetCenter() );
			SetBoundingRadius( m_pXSkinAnim->GetAnimXSkinMesh()->GetBoundingRadius() );

			//InitPos();			
			//m_pXSkinAnim->GetMatrix().Scale( D3DXVECTOR3( m_fScale, m_fScale, m_fScale ) );	
			//m_pXSkinAnim->SetShowObject(true);
			//StateChange( m_StartState );
		}

		//m_bLoaded = true;
		//*/
	}

#ifdef PET_ATTACH_PARTICLE
	if( NULL != g_pData &&
		NULL != g_pData->GetUIMajorParticle() )
	{
		std::vector< AttachedPetParticleData >::iterator vit;
		for( vit = m_vecSeqPetParticle.begin(); vit != m_vecSeqPetParticle.end(); ++vit )
		{
			D3DXVECTOR3 vPos = GetBonePos( vit->wstrBoneName.c_str() );
			vit->hSeq = g_pData->GetUIMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  vit->wstrParticleName.c_str(), vPos, 3.f, 3.f );		
		}
	}
#endif //PET_ATTACH_PARTICLE

	m_pAI = new CX2PETAI( this );
	if( NULL != m_pAI )
	{
		m_pAI->SetMasterUnitData();
		m_pAI->LoadAIDataFromLUA( m_LuaManager );

		if( true == m_pAI->GetAIFly())
		{
			m_OrgPhysicParam.fReloadAccel = 1400.f;
			m_ScriptedPhysicParam.fReloadAccel = 1400.f;
			m_PhysicParam.fReloadAccel = 1400.f;
			m_bFlyUnit = true;
		}
	}		
#endif

	m_bSetData = false;
}

void CX2PET::SetInfo( PetInfo petData)
{
	m_petData = petData;
	m_bSetData = true;

	if( m_bMyPet == true && ( g_pX2Game != NULL || g_pTFieldGame != NULL ) && g_pMain->GetIsPlayingTutorial() == false )
	{		
		if( m_pDlgPetGage == NULL )
		{
			m_pDlgPetGage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_PET_GAGE_BAR_NEW.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPetGage );
		}

		if( m_pDlgPetGage != NULL )
		{
			CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)petData.m_PetId );


			CKTDGUIControl::CPictureData *pPicture = NULL;	
			CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDlgPetGage->GetControl( L"g_pStatic_PET_GAGE_BAR" );
			if( pStatic != NULL )
				pPicture = pStatic->GetPicture( 1 );

			if( pTemplet != NULL && pPicture != NULL )
			{
				CX2PetManager::PetStepImage petStepImage = pTemplet->m_Evolution_Step_Game_Image[ petData.m_Evolution_Step ];

				wstring wstrImageName = petStepImage.m_wstrImageName;
				if( petStepImage.m_wstrKeyName == L"" )
				{
					pPicture->SetTex( wstrImageName.c_str() );
				}
				else
				{
					pPicture->SetTex( wstrImageName.c_str(), petStepImage.m_wstrKeyName.c_str() );
				}
			}

			CKTDGUIControl::CPictureData *pPicture1 = NULL;
			CKTDGUIControl::CPictureData *pPicture2 = NULL;
			if( pStatic != NULL )
			{
				pPicture1 = pStatic->GetPictureIndex( 1 );
				pPicture2 = pStatic->GetPictureIndex( 2 );
			}

			if( pPicture1 != NULL )
			{				
				D3DXVECTOR2 vOriSize = pPicture1->GetOriginalSize();
				pPicture1->SetSizeX( vOriSize.x *  0.f );
			}
			if( pPicture2 != NULL )
			{				
				D3DXVECTOR2 vOriSize = pPicture2->GetOriginalSize();
				pPicture2->SetSizeX( vOriSize.x *  0.f );
			}
		}
	}	
}

void CX2PET::DoFrameMove()
{
	KTDXPROFILE();

	while( (int)m_ReceivePacketBufList.size() > 0 )
	{
		KXPT_UNIT_PET_SYNC xPT_UNIT_PET_SYNC = m_ReceivePacketBufList[0];
		m_ReceivePacketBufList.erase( m_ReceivePacketBufList.begin() );

		// 싱크 테스트
		//bool bFind = false;
		//if( xPT_UNIT_PET_SYNC.nextState == m_AttackState || xPT_UNIT_PET_SYNC.nowState == m_AttackState )
		//{
		//	bFind = true;
		//}

		//m_petSyncData.nowState							= xPT_UNIT_PET_SYNC.nowState;
		m_petCondition.nextState						= xPT_UNIT_PET_SYNC.nextState;
		m_petSyncData.m_usRandomTableIndex				= xPT_UNIT_PET_SYNC.m_usRandomTableIndex;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		m_PhysicParam.nowSpeed.x						= halfToFloat( xPT_UNIT_PET_SYNC.usNowSpeedX );
		m_PhysicParam.nowSpeed.y						= halfToFloat( xPT_UNIT_PET_SYNC.usNowSpeedY );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		m_PhysicParam.nowSpeed.x						= xPT_UNIT_PET_SYNC.nowSpeedX;
//		m_PhysicParam.nowSpeed.y						= xPT_UNIT_PET_SYNC.nowSpeedY;		
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		//GetGageManager()->GetMPGage()->fNow					= halfToFloat(xPT_UNIT_PET_SYNC.fNowMP);

		
		m_petSyncData.SetData( &xPT_UNIT_PET_SYNC );

		StateChange( xPT_UNIT_PET_SYNC.nowState, true, false );
	}
}

HRESULT	CX2PET::OnFrameMove( double fTime, float fElapsedTime )
{
	if( GetLoaded() == false && NULL != m_pMotion )
	{
		const CKTDXDevice::EDeviceState eStateXMesh = m_pMotion->GetDeviceState();

		if ( CKTDXDevice::DEVICE_STATE_LOADED == eStateXMesh )
		{
			m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
			m_pXSkinAnim->SetAnimXSkinMesh( m_pMotion );
			m_pXSkinAnim->AddModelXSkinMesh( m_pMotion, m_pAniXET, NULL, m_pChangeTexXET );
			m_pXSkinAnim->SetModelDetailPercent( g_pData->GetModelDetailPercent() );

			SetShowObject( true );

			m_pXSkinAnim->GetMatrix().Move( GetPos() );
			m_pXSkinAnim->GetMatrix().Scale( D3DXVECTOR3( m_fScale, m_fScale, m_fScale ) );	
			m_pXSkinAnim->SetShowObject(true);

			SetCenter( m_pXSkinAnim->GetAnimXSkinMesh()->GetCenter() );
			SetBoundingRadius( m_pXSkinAnim->GetAnimXSkinMesh()->GetBoundingRadius() );

			m_pXSkinAnim->OnFrameMove(g_NowTime, 0.f);

			m_bLoaded = true;
		}
	}

	if( m_pMotion == NULL || m_pXSkinAnim == NULL )
		return S_OK;	

	if( GetLoaded() == false )
		return S_OK;

	if ( m_pDlgPetGage != NULL && m_pDlgPetGage->GetShow() == true )
	{
		switch ( g_pMain->GetNowStateID() )
		{
		default:
			{
				SetShowGageBar( false );
				return S_OK;
			} break;

		case CX2Main::XS_TRAINING_GAME:
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_VILLAGE_MAP:
			{
				if( true == g_pData->GetCashShop()->GetOpen() )
				{
					SetShowGageBar( false );
					return S_OK;
				}
				if ( g_pKTDXApp->GetDIManager()->GetMouse()->GetPureButtonState( MOUSE_RBUTTON ) == TRUE
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
					 && g_pData->GetUIManager() != NULL &&	g_pData->GetUIManager()->GetUICharInfo() != NULL
					 && g_pData->GetUIManager()->GetUICharInfo()->GetShow() == false
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
					)
				{
					D3DXVECTOR2 vResoultion( g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY() );
					D3DXVECTOR2 leftTop = m_pDlgPetGage->GetPos();
					leftTop.x *= vResoultion.x;
					leftTop.y *= vResoultion.y;
					D3DXVECTOR2 RigthBottom( leftTop.x + m_pDlgPetGage->GetSize().x * vResoultion.x, leftTop.y + m_pDlgPetGage->GetSize().y * vResoultion.y );
					D3DXVECTOR2 mousePoint( (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(), (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );

					if ( mousePoint.x >= leftTop.x && mousePoint.x <= RigthBottom.x && 
						mousePoint.y >= leftTop.y && mousePoint.y <= RigthBottom.y )
					{
						m_bOnPopUp = true;
						g_pData->GetUserMenu()->ClosePopupMenu();						
						g_pData->GetPetManager()->OpenPetPopupMenu();
					}
					else
					{
						if ( true == m_bOnPopUp )
							g_pData->GetPetManager()->ClosePetPopupMenu();

						m_bOnPopUp = false;
					}
				}
#ifndef PET_AURA_BUFF
				if ( m_TimerAuraSuspended.GetTargetTime() > 0.0f )
                {
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                    m_TimerAuraSuspended.OnFrameMove( fElapsedTime);
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
					m_TimerAuraSuspended.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                }
#endif
			}
			break;
		}
	}
	
	if( m_bInitPos == false )
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3( 0.f, 0.f, 0.f );
		bool IsRight = true;
		vPos.y += 30.f;
		GetMasterInfo( vPos, IsRight );
		SetPosition( vPos, IsRight );
		m_bInitPos = true;
	}
	
	if( m_bOldLoaded == false && GetLoaded() == true )
	{
		StateChange( m_StartState );
		if( m_bEvolution == true )
		{
			PlayCommonEffect( PCE_EVOLUTION );
			m_bEvolution = false;
		}
		else
		{
			if( m_bMyPet == true )
				PlayCommonEffect( PCE_SUMMON );
		}
	}
	m_bOldLoaded = GetLoaded();

	bool bCheerAction = false;
	bool bAttackAction = false;
	bool bCuteAction = false;
	bool bAfraidAction = false;
	bool bUnKnown = false;
	bool bHungry = false;

	m_fElapsedTime = fElapsedTime;
	m_fNowStateTimer += fElapsedTime;
	if( m_fIconTimer > 0.f )
	{
		m_fIconTimer -= fElapsedTime;
		if( m_fIconTimer < 0.f )
		{
			if( m_hSeqIcon != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqIcon );
				m_hSeqIcon = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}

			m_fIconTimer = 0.f;
		}
	}	

	CX2GUUser* pMasterUserUnit = static_cast<CX2GUUser*>( GetMaster() );
	if( NULL == pMasterUserUnit)
	{
		if( NULL != g_pX2Game && -1 != m_MasterUid )
		{
			m_optrMasterGameUnit = g_pX2Game->GetUserUnitByUID( m_MasterUid );
		}
	}

	if( NULL != pMasterUserUnit )
	{
#ifdef PET_AURA_BUFF
		if ( m_TimerAuraSuspended.GetTargetTime() > 0.0f )
		{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_TimerAuraSuspended.OnFrameMove( fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_TimerAuraSuspended.OnFrameMove();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

			if( m_TimerAuraSuspended.GetTargetTime() <= m_TimerAuraSuspended.GetSumOfElapsedTime() )
			{
				pMasterUserUnit->EraseBuffTempletFromGameUnit( BTI_BUFF_PET_AURA_SKILL );	/// 걸려있던 버프 삭제

				if( m_hEffectCheer != INVALID_EFFECTSET_HANDLE )
				{
					g_pData->GetUIEffectSet()->StopEffectSet( m_hEffectCheer );	/// 이펙트 삭제
					m_hEffectCheer = INVALID_EFFECTSET_HANDLE;
				}

				m_TimerAuraSuspended.SetTargetTime( -1.f );		/// 시간 초기화
				m_TimerAuraSuspended.ResetSumOfElapsedTime();
			}
		}
#else  PET_AURA_BUFF
		if( m_hEffectCheer != INVALID_EFFECTSET_HANDLE && pMasterUserUnit->GetPetCheer().m_fTime <= 0.f )
		{
			g_pData->GetUIEffectSet()->StopEffectSet( m_hEffectCheer );
			m_hEffectCheer = INVALID_EFFECTSET_HANDLE;
		}
#endif PET_AURA_BUFF
	}

	if( NULL != g_pX2Game != NULL && NULL != pMasterUserUnit )
	{
		if( pMasterUserUnit->GetSyncData().bFrameStop == true )
		{
			return S_OK;
		}
	}

#ifdef _IN_HOUSE_
	if( m_bCheat == true )
	{
		m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );

		if( false == m_PetStateData.Lua_FrameMove.empty() )
		{
			//string func;
			//ConvertWCHARToChar( func, m_PetStateData.Lua_FrameMove.c_str() );
#ifdef LEAVE_LAST_ERROR_LOG_TEST
			LastErrorLog( m_PetStateData.Lua_FrameMove.c_str() );
#endif LEAVE_LAST_ERROR_LOG_TEST

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
			lua_tinker::call<void>( m_LuaManager.GetLuaState(), m_PetStateData.Lua_FrameMove.c_str(), g_pKTDXApp, g_pX2Game, this );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
			lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_PetStateData.Lua_FrameMove.c_str(), g_pKTDXApp, g_pX2Game, this );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		}

		for( int i=0; i< (int)m_vecSoundPlayData.size(); i++ )
		{
			float fTime = m_vecSoundPlayData[i].m_SoundPlayTime;
			int iRate	= m_vecSoundPlayData[i].m_SoundPlayRate;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            if( m_pXSkinAnim->EventTimerOneshot( fTime ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			if( m_pXSkinAnim->EventTimer( fTime ) == true && EventCheck( fTime ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			{
				if( rand()%100 < iRate )
				{
					CKTDXDeviceSound* pSound = PlaySound( m_vecSoundPlayData[i].m_SoundPlayName.c_str() );
					if( NULL != pSound &&
						-1 != m_vecSoundPlayData[i].m_fMaxSoundDistance )
					{
						pSound->SetMax3DDistance( m_vecSoundPlayData[i].m_fMaxSoundDistance );
					}
				}
			}
		}
		return S_OK;
	}
#endif

	bool bChangeStateDummy = false;
	if( g_pData->GetPetManager()->GetPetStatus( m_petData ) > 0 && 
		//GetPetInfo().m_Evolution_Step > 0 &&
		m_petSyncData.nowState == m_WaitState && m_fNowStateTimer >= 90.f &&
		g_pData != NULL && g_pData->GetPetManager() != NULL )
	{
		if( g_pX2Game != NULL && g_pX2Game->IsHost() == true )
		{
			if( GetFlyUnit() == true && GetFootOnLine() == false )
			{
				StateChange( m_SleepFallState, false, false );
			}
			else
			{
				StateChange( m_SleepStartState, false, false );
			}
			bChangeStateDummy = true;
		}
		else if( g_pTFieldGame != NULL )
		{
			if( m_MasterUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				g_pData->GetPetManager()->SendPetActionCommand( PAC_FORCE_SLEEP );
				bChangeStateDummy = true;
			}
		}
	}

	if( bChangeStateDummy == false && 
		g_pData->GetPetManager()->GetPetStatus( m_petData ) > 0 && 
		//GetPetInfo().m_Evolution_Step > 0 &&
		m_petSyncData.nowState == m_WaitState && m_fNowStateTimer >= 5.f)
	{
		if( g_pX2Game != NULL && g_pX2Game->IsHost() == true && 
			null != m_optrMasterGameUnit && m_optrMasterGameUnit->GetRandomFloat() < 0.02f )
		{
			StateChange( m_WaitHabitState, false, false );
		}
		else if( (g_pTFieldGame != NULL || g_pX2Room != NULL ) && rand() % 1000 < 10 )
		{
			StateChange( m_WaitHabitState, false, false );
		}
	}
		
	if( g_pX2Game != NULL && null != m_optrMasterGameUnit && 
		m_optrMasterGameUnit->GetNowHp() <= 0.f && GetNowState() != m_SadState )
	{
		StateChange(m_SadState, false, true );
	}

	if( m_bSetData == true && g_pData != NULL && g_pX2Game != NULL && g_pX2Game->IsHost() == true &&
		null != m_optrMasterGameUnit && m_optrMasterGameUnit->GetNowHp() > 0.f &&
		g_pData->GetPetManager()->GetPetStatus( m_petData ) > 0 )
	{
		bool bDone = false;

		// 마스터 피격/타격 상태에 따른 상황별 펫상태	
		if( m_fTimeAttack >= m_fComboIntervalTime )
		{
			m_fTimeAttack = 0.f;
			m_AttackComboNum = 0;
		}
		else
		{
			m_fTimeAttack += fElapsedTime;
		}
		if( m_fTimeDefence >= m_fComboIntervalTime )
		{
			m_fTimeDefence = 0.f;
			m_DefenceComboNum = 0;
		}
		else
		{
			m_fTimeDefence += fElapsedTime;
		}

		int iExtroversion = m_petData.m_Extroversion;
		int iEmotion = m_petData.m_Emotion;
		int iDispositionY = g_pData->GetPetManager()->GetDisposition( iExtroversion ); // 외향성
		int iDispositionX = g_pData->GetPetManager()->GetDisposition( iEmotion ); // 감성
		int iActionStateId = m_CheerState;

		// 펫 성향에 따른 행동 결정		
		// 격력	(포만도 40%이상일 경우만 기능발동함)
		if( GetNowSatiety() >= 0.4f )
		{
			if( g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON || g_pX2Game->GetGameType() == CX2Game::GT_BATTLE_FIELD )
			{
				// 던전
				if( m_DefenceComboNum >= g_pData->GetPetManager()->GetDispositionVal( 2, iEmotion, iExtroversion ) ||
					m_AttackComboNum >= g_pData->GetPetManager()->GetDispositionVal( 2, iEmotion, iExtroversion ) )
				{				
					if( iDispositionY > 0 )
					{
						if( m_optrMasterGameUnit->GetNowHp() / m_optrMasterGameUnit->GetMaxHp() <= 0.2f )
					{
							if( GetActionSuccess(iActionStateId, false) == true )
							{
								bCheerAction = true;
							}
							else if( iActionStateId == m_NotKnowState )
							{							
								bCheerAction = true;
							}
						}				
					}
					else
					{
						if( m_optrMasterGameUnit->GetNowHp() / m_optrMasterGameUnit->GetMaxHp() <= 0.3f )
						{
							if( GetActionSuccess(iActionStateId) == true )
							{
								bCheerAction = true;
							}
							else if( iActionStateId == m_NotKnowState )
							{							
								bCheerAction = true;
							}
						}
					}
				}			
			}
			else
			{
				// 대전
				int nCount = 0;
				if( iDispositionX > 0 )
				{
					// 자신이 xxx

					if( iDispositionY > 0 )
					{
						// 킬
						nCount = m_optrMasterGameUnit->m_KillUserUnitList.size();

					}
					else
					{
						// 데스
						nCount = static_cast<CX2GUUser*>( m_optrMasterGameUnit.GetObservable() )->GetDieCount();
					}				
				}
				else
				{
					// 팀이 xxx				
					if( iDispositionY > 0 )
					{
						// 킬
						int iTeam = m_optrMasterGameUnit->GetTeam();
						int nUnitNum = g_pX2Game->GetUnitNum();
						for( int i=0; i<nUnitNum; ++i)
						{
							CX2GUUser *pUser = g_pX2Game->GetUserUnit(i);
							if( pUser != NULL && pUser->GetTeam() == iTeam )
							{
								nCount += pUser->m_KillUserUnitList.size();
							}
						}
					}
					else
					{
						// 킬
						int iTeam = m_optrMasterGameUnit->GetTeam();
						int nUnitNum = g_pX2Game->GetUnitNum();
						for( int i=0; i<nUnitNum; ++i)
						{
							CX2GUUser *pUser = g_pX2Game->GetUserUnit(i);
							if( pUser != NULL && pUser->GetTeam() == iTeam )
							{
								nCount += pUser->GetDieCount();
							}
						}
					}
				}
				if( nCount > 0 && nCount % g_pData->GetPetManager()->GetDispositionVal( 3, iEmotion, iExtroversion ) == 0 )
				{
					iActionStateId = m_CheerState;
					if( GetActionSuccess( iActionStateId ) == true )
					{
						bCheerAction = true;
					}
					else
					{
						if( iActionStateId == m_NotKnowState )
							bUnKnown = true;
						else if( iActionStateId == m_HungryState )
							bHungry = true;
					}
				}
			}
		}
		

		// 공격		
		bool bDefencer = false;		
		float fNeedMp = 300.f;
		if( iDispositionY <= 0 )
		{
			fNeedMp = 250.f;
		}
		
		if( m_fNowMp >= fNeedMp )
		{
			if( m_DefenceComboNum >= g_pData->GetPetManager()->GetDispositionVal( 1, iEmotion, iExtroversion ) )
			{
				bAttackAction = true;
				bDefencer = true;			
			}
			else if( m_AttackComboNum >= g_pData->GetPetManager()->GetDispositionVal( 1, iEmotion, iExtroversion ) )
			{
				bAttackAction = true;
				bDefencer = false;			
			}
		}		
		
		bool bAttackFail = false;
		if( bAttackAction == true )
		{
			iActionStateId = m_AttackState;
			if( GetActionSuccess( iActionStateId ) == false )
			{
				bAttackAction = false;
				bAttackFail = true;
			
				if( iActionStateId == m_NotKnowState )
					bUnKnown = true;
				else if( iActionStateId == m_HungryState )
					bHungry = true;
			
			}			
		}

		if( m_fAttackTraceTime > 0.f )
		{
			m_fAttackTraceTime -= fElapsedTime;
			if( m_fAttackTraceTime <= 0.f )
			{				
				bAttackAction = false;
				bAttackFail = true;
			}
		}

		// 피격 피드백
		bool bEmotionUnKnown = false;
		bool bEmotionHungry = false;
		if( IsActionState( GetNowState() ) == false )
		{				
			if( m_AttackComboNum > 0 && m_AttackComboNum % g_pData->GetPetManager()->GetDispositionVal( 4, iEmotion, iExtroversion ) == 0 )
			{			
				iActionStateId = m_Happy1State;
				if( GetActionSuccess( iActionStateId ) == true )
				{
					bCuteAction = true;
				}
				else
				{
					if( iActionStateId == m_NotKnowState )
						bUnKnown = true;
					else if( iActionStateId == m_HungryState )
						bHungry = true;
				}				
			}		
			else if( m_DefenceComboNum > 0 && m_DefenceComboNum % g_pData->GetPetManager()->GetDispositionVal( 4, iEmotion, iExtroversion ) == 0 )
			{
				iActionStateId = m_AfraidState;
				if( GetActionSuccess( iActionStateId ) == true )
				{
					bAfraidAction = true;
				}
				else
				{
					if( iActionStateId == m_NotKnowState )
						bUnKnown = true;
					else if( iActionStateId == m_HungryState )
						bHungry = true;
				}
			}
		}


		// 상태 결정
		if( bAttackFail == true )
		{
			m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();

			m_fNowMp -= 50.f;
			if( m_fNowMp < 0.f )
				m_fNowMp = 0.f;

			m_bAttackTrace = false;
			m_bAttacking = false;
			m_fAttackTraceTime = 0.f;
		}

		bDone = false;		
		if( bCheerAction == true && m_bAttackTrace == false)
		{	
			bDone = StateChange(m_CheerState, false, false );
			m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();

			if( bDone == true )
			{
				m_AttackComboNum = 0;
				m_DefenceComboNum = 0;
			}
		}

		if( bDone == false && bAttackAction == true && m_bAttackTrace == false )
		{			
			if( bDefencer == false )
			{
				// 내가 마지막으로 공격한 유닛 타겟팅					
				if ( null != m_optrMasterGameUnit && 
					 NULL != m_optrMasterGameUnit->GetGameUnitAttackedByMe() && 
					 CX2GameUnit::GUT_NPC == m_optrMasterGameUnit->GetGameUnitAttackedByMe()->GetGameUnitType() )
				{
					CX2GUNPC* pNpcUnitAttackedByMe = static_cast<CX2GUNPC*>( m_optrMasterGameUnit->GetGameUnitAttackedByMe() );
					bool bEnableAttack = true;
#ifdef FIX_PET_ATTACK					
					if( NULL == pNpcUnitAttackedByMe ||
						( pNpcUnitAttackedByMe->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC ) ||
						pNpcUnitAttackedByMe->GetNowHp() <= 0.f )
						bEnableAttack = false;		
#endif
					if( bEnableAttack == true && m_pAI != NULL && m_bAttacking == false && m_bAttackTrace == false && m_fNowMp >= fNeedMp )
					{
						m_pAI->SetGameUnitAttackMeOrBeAttackedByMe( pNpcUnitAttackedByMe );
						m_bAttackTrace = true;
						m_fAttackTraceTime = 30.f;
					}
					else if( m_pAI != NULL )
					{
						m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
						m_bAttackTrace = false;
					}
				}
			}						
			else
			{
				// 나를 마지막으로 공격한 유닛 타겟팅
				if ( null != m_optrMasterGameUnit )
				{
					CX2GameUnit* pGameUnitWhoAttackedMe = m_optrMasterGameUnit->GetGameUnitWhoAttackedMe();
					bool bEnableAttack = true;
#ifdef FIX_PET_ATTACK
					if( g_pX2Game != NULL )
					{
#ifdef  X2OPTIMIZE_UNITTYPE_BUG_FIX
                        CX2GUNPC* pNpc = ( pGameUnitWhoAttackedMe != NULL && pGameUnitWhoAttackedMe->GetGameUnitType() == CX2GameUnit::GUT_NPC )
                            ? static_cast<CX2GUNPC*>( pGameUnitWhoAttackedMe ) : NULL;
#else   X2OPTIMIZE_UNITTYPE_BUG_FIX
						CX2GUNPC *pNpc = static_cast<CX2GUNPC*>( pGameUnitWhoAttackedMe );
#endif  X2OPTIMIZE_UNITTYPE_BUG_FIX

						if ( NULL != pNpc )
						{
							if ( pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC || pNpc->GetNowHp() <= 0.f )
								bEnableAttack = false;
						}
						else
						{
							bEnableAttack = false;
						}
						/*
						if( NULL == pNpc || pNpc->GetNPCTemplet().m_ClassType != CX2UnitManager::NCT_BASIC || pNpc->GetNowHp() <= 0.f )
						{
							bEnableAttack = false;
						}
						*/
					}
#endif
					if( bEnableAttack == true && m_pAI != NULL && m_bAttacking == false && m_bAttackTrace == false && m_fNowMp >= fNeedMp )
					{
						m_pAI->SetGameUnitAttackMeOrBeAttackedByMe( pGameUnitWhoAttackedMe );
						m_bAttackTrace = true;
						m_fAttackTraceTime = 30.f;
					}
					else if( m_pAI != NULL )
					{
						m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
						m_bAttackTrace = false;
					}
				}
			}
			m_AttackComboNum = 0;
			m_DefenceComboNum = 0;

			bDone = true;
		}
	    
		if( bDone == false && bCuteAction == true && m_bAttackTrace == false )
		{			
			int irand = m_optrMasterGameUnit->GetRandomInt(12) % 2;
			if( irand == 0 )
			{
				bDone = StateChange( m_Happy1State, false, false );					
				m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
			}
			else
			{
				bDone = StateChange( m_Happy2State, false, false );
				m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
			}
		}
		
		if( bDone == false && bAfraidAction == true )
		{
			bDone = StateChange( m_SadState, false, false );
			m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
		}		
		
		if( bDone == false && bHungry == true )
		{
			StateChange( m_HungryState, true, false );
			m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
			m_bAttackTrace = false;
			m_bAttacking = false;
			m_fAttackTraceTime = 0.f;
		}
		else if( bUnKnown == true )
		{
			StateChange( m_NotKnowState, true, false );
			m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
			m_bAttackTrace = false;
			m_bAttacking = false;
			m_fAttackTraceTime = 0.f;
		}
	}
	

	map<char,PetStateData>::iterator mit;
	for ( mit = m_StateList.begin(); mit != m_StateList.end(); mit++ )
	{
		PetStateData& petStateData = (mit->second);

		if ( petStateData.fStateCoolTime > 0.f )
			petStateData.fStateElapsedTime += fElapsedTime;
	}


#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	map<float,bool>::iterator iter;
	for( iter = m_EventTimeStamp.begin(); iter != m_EventTimeStamp.end(); iter++ )
	{
		iter->second = true;
	}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	m_petCondition.fStateTimeBack	= m_petCondition.fStateTime;
	m_petCondition.fStateTime		+= m_fElapsedTime;

	if( g_pX2Game != NULL )
		DoFrameMove();
	
#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    bool    bXSkinAnimOnFrameMoveCalled = false;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( GetLoaded() == true && 		
		m_pMotion != NULL && m_pXSkinAnim != NULL &&
		( g_pX2Game != NULL || g_pTFieldGame != NULL ) )
	{
		if( m_pAI != NULL && 
			( g_pX2Game == NULL || (g_pX2Game != NULL && g_pX2Game->IsHost() == true) ) )
		{
			if( (m_petSyncData.nowState == m_SleepLoopState || IsActionState(m_petSyncData.nowState) == false) && m_pAI->GetGameUnitAttackMeOrBeAttackedByMe() == NULL )
			{
				D3DXVECTOR3 vStart = GetPos();
				D3DXVECTOR3 vEnd = D3DXVECTOR3( 0.f, 0.f, 0.f );
				bool IsRight = true;
				GetMasterInfo( vEnd, IsRight );
				D3DXVECTOR3 vDist = vEnd - vStart;
				float fDist = D3DXVec3LengthSq( &vDist );

				if( fDist >= m_fLostRange * m_fLostRange )
				{
					SetPosition( vEnd, true );
					StateChange( m_WaitState );
				}
			}			
		}

		GetMatrix().Move( m_petSyncData.position );
		GetMatrix().RotateDegree( m_petCondition.dirDegree );

		if( m_bForceBillBoard == true )
		{		
			m_pXSkinAnim->SetBillBoardType( CKTDGMatrix::BT_ALL );
		}	
		else
		{
			m_pXSkinAnim->GetMatrix().RotateDegree( m_petCondition.dirDegree );
		}

		m_pXSkinAnim->GetMatrix().Move( m_petSyncData.position );		

		m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );
#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        bXSkinAnimOnFrameMoveCalled = true;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	else if( GetLoaded() == true && 		
		m_pMotion != NULL && m_pXSkinAnim != NULL &&
		g_pX2Room != NULL )
	{
		CX2Room::SlotData *pSlot = g_pX2Room->GetSlotDataByUnitUID( m_MasterUid );
		if( pSlot != NULL )
		{
			SetShowObject( true );
			CX2UnitViewerUI *pUnitView = pSlot->m_pUnitViewer;
			if( pUnitView != NULL )
			{
				D3DXVECTOR3 vOffset = D3DXVECTOR3(0.f, 0.f, -100.f);
				int iSlotIndex = pSlot->m_Index;

				switch( iSlotIndex )
				{
				case 0:					
				case 1:					
				case 2:					
				case 3:
					vOffset = D3DXVECTOR3(-40.f, 50.f, -100.f);
					break;
				case 4:					
				case 5:					
				case 6:					
				case 7:
					vOffset = D3DXVECTOR3(-20.f, 40.f, -100.f);
					break;
				}

				D3DXVECTOR3 vLightPos = pUnitView->GetLightPos();
				D3DXVECTOR3 vPetPos = pUnitView->GetMatrix().GetPos();
				D3DXVECTOR3 vPetRotDegree = pUnitView->GetMatrix().GetRotateDegree();

				CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
				pRenderParam->lightPos	= vLightPos;
				pRenderParam->color = 0xffffffff;

				vPetPos = vPetPos + vOffset;

				GetMatrix().Move( vPetPos );
				GetMatrix().RotateDegree( vPetRotDegree );

				if( m_bForceBillBoard == true )
				{		
					m_pXSkinAnim->SetBillBoardType( CKTDGMatrix::BT_ALL );
				}	
				else
				{
					m_pXSkinAnim->GetMatrix().RotateDegree( vPetRotDegree );
				}

				m_pXSkinAnim->GetMatrix().Move( vPetPos );		

				m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );
#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
                bXSkinAnimOnFrameMoveCalled = true;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			}
		}
		else 
		{
			SetShowObject( false );
		}		
	}
#ifdef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( bXSkinAnimOnFrameMoveCalled == false )
    {
        if ( m_pXSkinAnim != NULL )
            m_pXSkinAnim->UpdateBeforeAnimationTime();
    }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE	

	if( false == m_PetStateData.Lua_FrameMove.empty() )
	{
		//string func;
		//ConvertWCHARToChar( func, m_PetStateData.Lua_FrameMove.c_str() );
#ifdef LEAVE_LAST_ERROR_LOG_TEST
		LastErrorLog( m_PetStateData.Lua_FrameMove.c_str() );
#endif LEAVE_LAST_ERROR_LOG_TEST

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		lua_tinker::call<void>( m_LuaManager.GetLuaState(), m_PetStateData.Lua_FrameMove.c_str(), g_pKTDXApp, g_pX2Game, this );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_PetStateData.Lua_FrameMove.c_str(), g_pKTDXApp, g_pX2Game, this );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	}

	for( int i = 0; i < (int)m_EventIntervalList.size(); i++ )
	{
		D3DXVECTOR3* pIntervel = &m_EventIntervalList[i];
		pIntervel->y += fElapsedTime;
		if( pIntervel->x < pIntervel->y )
		{
			pIntervel->z = 1.0f;
			pIntervel->y = 0.0f;
		}
		else
		{
			pIntervel->z = 0.0f;
		}
	}	

	if( m_pAI != NULL )
	{
		m_pAI->OnFrameMove( fTime, fElapsedTime );
	}
	
	if( m_PhysicParam.passiveSpeed.x != -1.0f )
	{
		m_PhysicParam.nowSpeed.x = m_PhysicParam.passiveSpeed.x * m_PhysicParam.fSpeedFactor;
	}
	if( m_PhysicParam.passiveSpeed.y != -1.0f )
	{
		m_PhysicParam.nowSpeed.y = m_PhysicParam.passiveSpeed.y * m_PhysicParam.fSpeedFactor;
	}

	if( g_pX2Game != NULL || g_pTFieldGame != NULL )
	{
		PhysicProcess();
	}	

	if( g_pX2Game == NULL || (g_pX2Game != NULL && g_pX2Game->IsHost()) )
		DoProcessEvent();

	for( int i=0; i< (int)m_vecSoundPlayData.size(); i++ )
	{
		float fTime = m_vecSoundPlayData[i].m_SoundPlayTime;
		int iRate	= m_vecSoundPlayData[i].m_SoundPlayRate;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        if( m_pXSkinAnim->EventTimerOneshot( fTime ) == true )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		if( m_pXSkinAnim->EventTimer( fTime ) == true && EventCheck( fTime ) == true )
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{
			if( rand()%100 < iRate )
			{
				CKTDXDeviceSound* pSound = PlaySound( m_vecSoundPlayData[i].m_SoundPlayName.c_str() );
				if( NULL != pSound &&
					-1 != m_vecSoundPlayData[i].m_fMaxSoundDistance )
				{
					pSound->SetMax3DDistance( m_vecSoundPlayData[i].m_fMaxSoundDistance );
				}
			}
		}
	}

	if( m_hSeqIcon != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqIcon );
		ASSERT( NULL != pSeq );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vEmoticonPos = GetBonePos(L"Bip01_Head");
			if( GetNowState() != m_SleepLoopState )
			{
				vEmoticonPos.x -= 50.f;
				vEmoticonPos.y += 50.f;
			}			
			pSeq->SetPosition( vEmoticonPos );
		}
		else
		{
			m_hSeqIcon = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

#ifdef PET_ATTACH_PARTICLE
	std::vector< AttachedPetParticleData >::iterator vit;
	for( vit = m_vecSeqPetParticle.begin(); vit != m_vecSeqPetParticle.end(); ++vit )
	{
		if( vit->hSeq != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( vit->hSeq );
			ASSERT( NULL != pSeq );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vEmoticonPos = GetBonePos( vit->wstrBoneName.c_str() );
				pSeq->SetPosition( vEmoticonPos );
			}
			else
			{
				vit->hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
			}
		}
	}
#endif //PET_ATTACH_PARTICLE

	// game gage 
	if( m_bMyPet == true && m_pDlgPetGage != NULL )
	{		
		CKTDGUIControl::CPictureData *pPicture1 = NULL;
		CKTDGUIControl::CPictureData *pPicture2 = NULL;
		CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDlgPetGage->GetControl( L"g_pStatic_PET_GAGE_BAR" );
		if( pStatic != NULL )
		{
			pPicture1 = pStatic->GetPictureIndex( 1 );
			pPicture2 = pStatic->GetPictureIndex( 2 );
		}

		if( pPicture1 != NULL )
		{
			float fSize = GetPetInfo().m_Satiety / 4800.f;
			D3DXVECTOR2 vOriSize = pPicture1->GetOriginalSize();
			pPicture1->SetSizeX( vOriSize.x *  fSize );
		}
		if( pPicture2 != NULL )
		{
			float fSize = m_fNowMp / 300.f;
			D3DXVECTOR2 vOriSize = pPicture2->GetOriginalSize();
			pPicture2->SetSizeX( vOriSize.x *  fSize );
		}

		g_pData->GetPetManager()->SetSummonedPetMp( m_fNowMp );
	}

	if( g_pX2Game != NULL && 
		g_pX2Game->GetWorld() != NULL &&
		g_pX2Game->GetLineMap() != NULL )
	{
		const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetLineMap()->GetLineData( m_petSyncData.lastTouchLineIndex );
		if( pLineData != NULL && pLineData->m_bUnderWater == true && GetPos().y <= pLineData->m_fWaterHeight )
		{			
			m_bUnderWaterHead = true;
			float fMinY = min( pLineData->startPos.y, pLineData->endPos.y );			
			float fColorRate = (pLineData->m_fWaterHeight - GetPos().y) / (pLineData->m_fWaterHeight - fMinY); 
			m_cLineUnitColor = D3DXCOLOR( pLineData->m_cLineColor.r * fColorRate, pLineData->m_cLineColor.g * fColorRate, pLineData->m_cLineColor.b * fColorRate, 0.f );
		}
		else if( pLineData != NULL && pLineData->m_bForceChangeColor == true )
		{
			m_bForceChagneColor = true;
			m_cLineUnitColor = D3DXCOLOR( pLineData->m_cLineColor.r, pLineData->m_cLineColor.g, pLineData->m_cLineColor.b, 0.f );
		}
		else
		{
			m_bUnderWaterHead = false;
			m_bForceChagneColor = false;
			m_cLineUnitColor = D3DXCOLOR( 0.f, 0.f, 0.f, 0.f );
		}
	}
	return S_OK;
}

RENDER_HINT CX2PET::OnFrameRender_Prepare()
{
	if ( GetLoaded() == false || m_pXSkinAnim == NULL )
		return RENDER_HINT_NORENDER;

	m_pXSkinAnim->SetInstantShow( false );
	if ( !m_pXSkinAnim->GetShowObject() )
		return RENDER_HINT_NORENDER;

	CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();

	CX2World *pWorld =  NULL;

	if( g_pX2Game != NULL )
	{
		pWorld = g_pX2Game->GetWorld();
	}
	else
	{
		if( g_pTFieldGame != NULL )
		{
			pWorld = g_pTFieldGame->GetWorld();
		}
		else if( g_pSquareGame != NULL )
		{
			pWorld = g_pSquareGame->GetWorld();
		}
	}

	if( pWorld != NULL )
		pRenderParam->lightPos	= pWorld->GetLightPos();

#if 0
	// 라이트값 제거 (의도적으로 보이는데 뭤때문에 넣었는지 기억안남...)
	if( pWorld != NULL )
	{
		pRenderParam->lightPos	= pWorld->GetLightPos();
		pRenderParam->color.r	= pWorld->GetLightColor().r;
		pRenderParam->color.g	= pWorld->GetLightColor().g;
		pRenderParam->color.b	= pWorld->GetLightColor().b;
	}
#endif

	pRenderParam->color = m_cPetColor;

#if defined(GHOST_PET)
	if( m_bForceColor == false && (m_bUnderWaterHead == true || m_bForceChagneColor == true) )
	{
		pRenderParam->color.r -= m_cLineUnitColor.r;
		pRenderParam->color.g -= m_cLineUnitColor.g;
		pRenderParam->color.b -= m_cLineUnitColor.b;
	}	
#else //defined(GHOST_PET)
	if( m_bUnderWaterHead == true || m_bForceChagneColor == true )
	{
		pRenderParam->color.r -= m_cLineUnitColor.r;
		pRenderParam->color.g -= m_cLineUnitColor.g;
		pRenderParam->color.b -= m_cLineUnitColor.b;
	}	
#endif //defined(GHOST_PET)

	pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;

	if( g_pMain->GetGameOption().GetOptionList().m_UnitDetail == CX2GameOption::OL_MEDIUM )
	{
		pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON;
	}
	else if( g_pMain->GetGameOption().GetOptionList().m_UnitDetail == CX2GameOption::OL_LOW )
	{
		pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
	}

	if( GetDistanceToCamera() > 700.0f )
	{
		pRenderParam->fOutLineWide = GetDistanceToCamera() / 1000.0f * 1.f;
	}
	else
	{
		pRenderParam->fOutLineWide = GetDistanceToCamera() / 500.0f * 1.f;
	}

	pRenderParam->cartoonTexType = m_RenderParam.cartoonTexType;
	pRenderParam->renderType	= m_RenderParam.renderType;
	pRenderParam->fOutLineWide	= m_RenderParam.fOutLineWide;	
	pRenderParam->bAlphaBlend	= m_RenderParam.bAlphaBlend;
	pRenderParam->cullMode		= m_RenderParam.cullMode;
	pRenderParam->bZEnable		= m_RenderParam.bZEnable;

	RENDER_HINT renderHintResult = m_pXSkinAnim->OnFrameRender_Prepare();

	if ( renderHintResult.m_eHint != RENDER_HINT::NORENDER )
		m_pXSkinAnim->SetInstantShow( true );

	return renderHintResult;
}

void CX2PET::OnFrameRender_Draw()
{
	KTDXPROFILE();
	
	if( m_pXSkinAnim == NULL || GetLoaded() == false )
		return;

	if( m_pXSkinAnim != NULL && m_pXSkinAnim->GetShowObject() && m_pXSkinAnim->IsInstantShow() )
	{
		m_pXSkinAnim->OnFrameRender_Draw();
	}
}

HRESULT CX2PET::OnResetDevice()
{
	return S_OK; 
}

HRESULT CX2PET::OnLostDevice()
{
	return S_OK; 
}


// bool CX2PET::OnThreadLoad(DWORD dwParam_, DWORD dwParam2_)
// {
// 	if( m_bGame == true )
// 	{
// 		bool bLoadWord = true;
// 
// 		if( g_pX2Game == NULL || g_pX2Game->GetWorld() == NULL || g_pX2Game->GetWorld()->GetLineMap() == NULL )
// 			bLoadWord = false;
// 
// 		while( !bLoadWord )
// 		{
// 			if( g_pX2Game == NULL || g_pX2Game->GetWorld() == NULL || g_pX2Game->GetWorld()->GetLineMap() == NULL )
// 				bLoadWord = false;
// 			else
// 				bLoadWord = true;
// 		}
// 	}
// 
// 	if( m_pMotion == NULL && m_wstrMotionName != L"")
// 	{
// 		m_pMotion			= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_wstrMotionName.c_str() );
// 		ASSERT( m_pMotion );
// 		if ( m_pMotion == NULL )
// 		{
// 			StateLog( m_ScriptFileName.c_str() );
// 			StateLog( m_wstrMotionName.c_str() );
// 			//MessageBox( g_pKTDXApp->GetHWND(), motion.c_str(), L"모션 파일 읽어오기 실패!", MB_OK );
// 		}
// 	}
// 
// 	if( m_pMotion != NULL )
// 	{
// 		//SAFE_CLOSE( m_pChangeTexXET );
// 		//m_pChangeTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_wstrChangeTex.c_str() );
// 		//SAFE_CLOSE( m_pAniXET );
// 		//m_pAniXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_wstrAniTex.c_str() );
// 
// 		//m_pXSkinAnim.reset();
// 		m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
// 		m_pXSkinAnim->SetAnimXSkinMesh( m_pMotion );
// 		m_pXSkinAnim->AddModelXSkinMesh( m_pMotion, m_pAniXET, NULL, m_pChangeTexXET );
// 		m_pXSkinAnim->SetModelDetailPercent( g_pData->GetModelDetailPercent() );
// 			
// 		//Sleep(30);
// 		
// 		SetShowObject( true );
// 
// 		m_pXSkinAnim->GetMatrix().Move( GetPos() );
// 		m_pXSkinAnim->GetMatrix().Scale( D3DXVECTOR3( m_fScale, m_fScale, m_fScale ) );	
// 		m_pXSkinAnim->SetShowObject(true);
// 
// 		SetCenter( m_pXSkinAnim->GetAnimXSkinMesh()->GetCenter() );
// 		SetBoundingRadius( m_pXSkinAnim->GetAnimXSkinMesh()->GetBoundingRadius() );
// 
// 		m_pXSkinAnim->OnFrameMove(g_NowTime, 0.f);
// 
// 		m_bLoaded = true;
// 
// 		return true;
// 	}
// 
// 	return false;
// }

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

/*static*/
void	CX2PET::InitInit( InitData& OutInit_, KLuaManager& luaManager_ )
{
	InitDevice( OutInit_.m_device, luaManager_ );
	InitMotion( OutInit_.m_device, luaManager_ );
}//CX2GUUser::InitInit()

void    CX2PET::InitData::AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const
{
	m_device.AppendToDeviceList( listInOut_ );
}//CX2GUUser::InitData::AppendToDeviceList()

void	CX2PET::InitDeviceData::AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const
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

}

/*static*/
void CX2PET::InitDevice( InitDeviceData& OutDevice_, KLuaManager& luaManager_ )
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

	//OutDevice_.m_vecstrReadySound.( L"Pet_Evolution.ogg" );
	//OutDevice_.m_vecstrReadySound.( L"Pet_Summon.ogg" );

}

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD


void CX2PET::InitDevice()
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
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				TextureReadyInBackground( texName.c_str() );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				TextureReady( texName.c_str() );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
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
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				XMeshReadyInBackground( xmeshName.c_str() );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				XMeshReady( xmeshName.c_str() );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
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
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				XSkinMeshReadyInBackground( xskinmeshName.c_str() );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				XSkinMeshReady( xskinmeshName.c_str() );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				index++;
			}
			m_LuaManager.EndTable();
		}



		if( m_LuaManager.BeginTable( "READY_SOUND" ) == true )
		{
			wstring soundName;
			int index = 1; 
			while( m_LuaManager.GetValue( index, soundName ) == true )
			{
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
				SoundReadyInBackground( soundName.c_str() );
#else	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
				SoundReady( soundName.c_str() );
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
				index++;
			}
			m_LuaManager.EndTable();
		}

		m_LuaManager.EndTable(); // INIT_DEVICE
	}
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
	SoundReadyInBackground( L"Pet_Evolution.ogg" );
	SoundReadyInBackground( L"Pet_Summon.ogg" );
#else	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
	SoundReady( L"Pet_Evolution.ogg" );
	SoundReady( L"Pet_Summon.ogg" );
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
	
}

void CX2PET::InitSystem()
{
	m_RenderParam.Init();
	m_bForceBillBoard = false; 

	if( m_LuaManager.BeginTable( "INIT_SYSTEM" ) == true )
	{	
		int iLayer = 0;
		m_LuaManager.GetValue( "UNIT_LAYER", iLayer );
		SetLayer( iLayer );

		LUA_GET_VALUE( m_LuaManager, "UNIT_SCALE", m_fScale, 1.f );		
		
		LUA_GET_VALUE_ENUM( m_LuaManager, "RENDER_PARAM", m_RenderParam.renderType, CKTDGXRenderer::RENDER_TYPE, CKTDGXRenderer::RT_CARTOON_BLACK_EDGE );
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		m_RenderParam.fOutLineWide	= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		m_RenderParam.fOutLineWide	= 1.7f;
#endif //UNIT_SCALE_COMBINE_ONE
		



		LUA_GET_VALUE( m_LuaManager, "ALPHA_BLEND", m_RenderParam.bAlphaBlend, false );
		if( m_RenderParam.bAlphaBlend == true )
		{
			SetAlphaObject( true );
		}
		bool bCullMode;
		LUA_GET_VALUE( m_LuaManager, "CULL_MODE", bCullMode, true );
		if( bCullMode == false )
		{
			m_RenderParam.cullMode = D3DCULL_NONE;
		}
		LUA_GET_VALUE( m_LuaManager, "Z_ENABLE", m_RenderParam.bZEnable, true );
		m_RenderParam.bZWriteEnable = m_RenderParam.bZEnable;			// fix!! 일단 요렇게

		LUA_GET_VALUE( m_LuaManager, "FORCE_BILLBOARD", m_bForceBillBoard, false );

#ifdef GHOST_PET

		CKTDGXRenderer::CARTOON_TEX_TYPE cartoonTexType = CKTDGXRenderer::CTT_NORMAL;
		LUA_GET_VALUE_ENUM( m_LuaManager, "CARTOON_TEX_TYPE", cartoonTexType, CKTDGXRenderer::CARTOON_TEX_TYPE, CKTDGXRenderer::CTT_NORMAL );
		m_RenderParam.cartoonTexType = cartoonTexType;

		D3DBLEND srcBlend = D3DBLEND_SRCALPHA;
		D3DBLEND destBlend = D3DBLEND_INVSRCALPHA;
		LUA_GET_VALUE_ENUM( m_LuaManager, "SRC_BLEND", srcBlend, D3DBLEND, D3DBLEND_SRCALPHA );
		LUA_GET_VALUE_ENUM( m_LuaManager, "DEST_BLEND", destBlend, D3DBLEND, D3DBLEND_INVSRCALPHA );
		m_RenderParam.srcBlend = srcBlend;
		m_RenderParam.destBlend = destBlend;

		LUA_GET_VALUE( m_LuaManager, "NOT_CHANGE_COLOR", m_bForceColor, false );
		float fColor = 255.f;
		LUA_GET_VALUE( m_LuaManager, "PET_COLOR_A", fColor, 255.f );
		m_cPetColor.a = fColor/255.f;
		LUA_GET_VALUE( m_LuaManager, "PET_COLOR_R", fColor, 255.f );
		m_cPetColor.r = fColor/255.f;
		LUA_GET_VALUE( m_LuaManager, "PET_COLOR_G", fColor, 255.f );
		m_cPetColor.g = fColor/255.f;
		LUA_GET_VALUE( m_LuaManager, "PET_COLOR_B", fColor, 255.f );
		m_cPetColor.b = fColor/255.f;
#endif

		m_LuaManager.EndTable();
	}
}

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

/*static*/
void	CX2PET::InitMotion( InitDeviceData& OutDevice_, KLuaManager& luaManager_ )
{
	if( luaManager_.BeginTable( "INIT_MOTION" ) == true )
	{		
		std::wstring	wstrName;

		if ( luaManager_.GetValue( "MOTION_FILE_NAME", wstrName ) == true )
			OutDevice_.m_vecstrReadyXSkinMesh.push_back( wstrName );
		if ( luaManager_.GetValue( "MOTION_CHANGE_TEX_XET", wstrName ) == true )
			OutDevice_.m_vecstrReadyXET.push_back( wstrName );
		if ( luaManager_.GetValue( "MOTION_ANI_TEX_XET", wstrName ) == true )
			OutDevice_.m_vecstrReadyXET.push_back( wstrName );

		luaManager_.EndTable();
	}
}


#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD


void CX2PET::InitMotion()
{
	if( m_LuaManager.BeginTable( "INIT_MOTION" ) == true )
	{		
		LUA_GET_VALUE( m_LuaManager, "MOTION_FILE_NAME", m_wstrMotionName, L"" );		
		LUA_GET_VALUE( m_LuaManager, "MOTION_CHANGE_TEX_XET", m_wstrChangeTex, L"" );
		LUA_GET_VALUE( m_LuaManager, "MOTION_ANI_TEX_XET", m_wstrAniTex, L"" );			

		m_LuaManager.EndTable();
	}

	SAFE_CLOSE( m_pChangeTexXET );
	m_pChangeTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_wstrChangeTex.c_str() );
	SAFE_CLOSE( m_pAniXET );
	m_pAniXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( m_wstrAniTex.c_str() );

#ifdef PET_ATTACH_PARTICLE
	m_vecSeqPetParticle.resize(0);
	if( m_LuaManager.BeginTable( "ATTACHED_PARTICLE" ) == true )
	{
		int index = 1;
		while( m_LuaManager.BeginTable( index ) == true )
		{
			AttachedPetParticleData particleData;

			LUA_GET_VALUE( m_LuaManager, "PARTICLE_NAME", particleData.wstrParticleName, L"" );
			LUA_GET_VALUE( m_LuaManager, "BONE_NAME",	particleData.wstrBoneName, L"" );

			particleData.hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;

			if( false == particleData.wstrParticleName.empty() &&
				false == particleData.wstrBoneName.empty() )
			{
				m_vecSeqPetParticle.push_back( particleData );
			}

			m_LuaManager.EndTable();
			index++;
		}
		m_LuaManager.EndTable();	// ATTACHED_PARTICLE
	}
#endif //PET_ATTACH_PARTICLE
}

void CX2PET::InitPhysic()
{
	m_OrgPhysicParam.fReloadAccel = 2000.f;
	m_OrgPhysicParam.fGAccel = 4000.f;
	m_OrgPhysicParam.fMaxGSpeed = -2000.f;

	m_OrgPhysicParam.fWalkSpeed = 600.f;
	m_OrgPhysicParam.fRunSpeed = 900.f;
	m_OrgPhysicParam.fJumpSpeed = 1600.f;
	m_OrgPhysicParam.fDashJumpSpeed = 2300.f;

	m_PhysicParam = m_OrgPhysicParam;
	m_ScriptedPhysicParam = m_OrgPhysicParam;
	m_PhysicParam = m_ScriptedPhysicParam;
}

void CX2PET::InitState()
{
	if( m_LuaManager.BeginTable( "INIT_STATE" ) == true )
	{
		PetStateData stateData;

		m_StateList.clear();
		m_StateNameIDMap.clear();


		m_StateList.insert( std::make_pair( PSI_NONE, stateData ) );
		m_StateNameIDMap.insert( std::make_pair( "NONE", PSI_NONE ) );

		m_StateList.insert( std::make_pair( PSI_LOADING, stateData ) );
		m_StateNameIDMap.insert( std::make_pair( "LOADING", PSI_LOADING ) );

		m_StateList.insert( std::make_pair( PSI_READY, stateData ) );
		m_StateNameIDMap.insert( std::make_pair( "READY", PSI_READY ) );

		m_StateList.insert( std::make_pair( PSI_PLAY, stateData ) );
		m_StateNameIDMap.insert( std::make_pair( "PLAY", PSI_PLAY ) );
		
		m_StateList.insert( std::make_pair( PSI_END, stateData ) );
		m_StateNameIDMap.insert( std::make_pair( "END", PSI_END ) );

		int index = 1;
		while( m_LuaManager.BeginTable( index ) == true )
		{
			PetStateData stateData;
			string stateTableName;
			LUA_GET_VALUE_UTF8( m_LuaManager, "STATE_NAME", stateTableName, "" );
			stateData.stateID = PSI_END + index;

#ifdef GHOST_PET
			LUA_GET_VALUE_UTF8( m_LuaManager, "LUA_STATE_START_FUNC",		stateData.Lua_Start,	"" );
#endif
			LUA_GET_VALUE_UTF8( m_LuaManager, "LUA_FRAME_MOVE_FUNC",		stateData.Lua_FrameMove,	"" );
			LUA_GET_VALUE( m_LuaManager, "STATE_COOL_TIME",			stateData.fStateCoolTime,	0.0f );
			stateData.fStateElapsedTime = stateData.fStateCoolTime;

			m_StateList.insert( std::make_pair( stateData.stateID, stateData ) );
			m_StateNameIDMap.insert( std::make_pair( stateTableName, stateData.stateID ) );

			index++;
			m_LuaManager.EndTable();
		}
		
		
		string			stateName = "";

		LUA_GET_VALUE_UTF8( m_LuaManager, "START_STATE", stateName, "" );
		m_StartState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "WAIT_HABIT", stateName, "" );
		m_WaitHabitState				= GetStateID( stateName );		
		LUA_GET_VALUE_UTF8( m_LuaManager, "WAIT_STATE", stateName, "" );
		m_WaitState					= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "EAT_STATE", stateName, "" );
		m_EatState					= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "HUNGRY_STATE", stateName, "" );
		m_HungryState				= GetStateID( stateName );		
		LUA_GET_VALUE_UTF8( m_LuaManager, "STUFFED_STATE", stateName, "" );
		m_StuffedState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SLEEP_START_STATE", stateName, "" );
		m_SleepStartState			= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SLEEP_LOOP_STATE", stateName, "" );
		m_SleepLoopState			= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SLEEP_END_STATE", stateName, "" );
		m_SleepEndState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "AFRAID_STATE", stateName, "" );
		m_AfraidState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "HELLO_STATE", stateName, "" );
		m_HelloState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "HAPPY1_STATE", stateName, "" );
		m_Happy1State				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "HAPPY2_STATE", stateName, "" );
		m_Happy2State				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "CUTE1_STATE", stateName, "" );
		m_Cute1State				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "CUTE2_STATE", stateName, "" );
		m_Cute2State				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SAD_STATE", stateName, "" );
		m_SadState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "CHEER_STATE", stateName, "" );
		m_CheerState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SPECIAL_STATE", stateName, "" );
		m_SpecialState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "ATTACK_STATE", stateName, "" );
		m_AttackState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SIT_START_STATE", stateName, "" );
		m_SitStartState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SIT_LOOP_STATE", stateName, "" );
		m_SitLoopState				= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SIT_END_STATE", stateName, "" );
		m_SitEndState				= GetStateID( stateName );		       
		LUA_GET_VALUE_UTF8( m_LuaManager, "NOTKNOW_STATE", stateName, "" );
		m_NotKnowState				= GetStateID( stateName );		 
		LUA_GET_VALUE_UTF8( m_LuaManager, "SLEEP_FALL_STATE", stateName, "" );
		m_SleepFallState			= GetStateID( stateName );
		LUA_GET_VALUE_UTF8( m_LuaManager, "SIT_FALL_STATE", stateName, "" );
		m_SitFallState				= GetStateID( stateName );		

		m_LuaManager.EndTable();
	}

	// state table 을 state ID 로 참조하기 위해 m_LuaManager 에 reference 로 등록한다.
	// state table 이 global table에 있기 때문에 위의 BeginTable, EndTable 블럭을 빠져나와서 등록함.

	for( std::map<string,char>::const_iterator iter = m_StateNameIDMap.begin(); iter != m_StateNameIDMap.end(); iter++ )
	{
		m_LuaManager.MakeTableReference( iter->first.c_str(), iter->second );
	}//
	//}} robobeg : 2008-10-28

}

bool CX2PET::GetActionSuccess(int &iStateId, bool bCheckSatietyRand)
{
	bool bResult = true;

	PetStateData& stateData = m_StateList[iStateId];
	if ( stateData.fStateElapsedTime < stateData.fStateCoolTime )
	{
		iStateId = -1;
		return false;
	}

	CX2PetManager::PetTemplet *petTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)m_petData.m_PetId );
	float fIntimacy = m_petData.m_Intimacy / petTemplet->m_Evolution_Step_Max[m_petData.m_Evolution_Step];	// 친밀도
	float fSatiety = m_petData.m_Satiety / 4800.f;
	float fRand = m_optrMasterGameUnit->GetRandomFloat(0);

	if( fIntimacy < 0.2f )
		fIntimacy = 0.2f;

	if( bCheckSatietyRand == true )
	{
		// 포만도 & 친밀도 검사
		if( fSatiety <= 0.4f && m_optrMasterGameUnit->GetRandomFloat(3) <= 1.f - fSatiety )
		{
			iStateId = m_HungryState;
			bResult = false;
		}
		else
		{
			if( fRand > fIntimacy )
			{
				iStateId = m_NotKnowState;
				bResult = false;
			}
		}
	}
	else
	{
		if( fSatiety <= 0.4f )
		{
			iStateId = m_HungryState;
			bResult = false;
		}
		else
		{
			if( fRand > fIntimacy )
			{
				iStateId = m_NotKnowState;
				bResult = false;
			}
		}
	}
	

	return bResult;
}

void CX2PET::PlayCommonEffect( PET_COMMON_EFFECT eType )
{
	if( g_pData == NULL )
		return;

	if( g_pX2Game == NULL && g_pTFieldGame == NULL )
		return;

	D3DXVECTOR3 vPos = GetLandPosition_LUA();
	switch( eType )
	{
	case PCE_SUMMON:
		{
			if( g_pData != NULL && g_pData->GetUIEffectSet() != NULL )
			{
				CX2EffectSet::Handle hEffect =  g_pData->GetUIEffectSet()->PlayEffectSetByPet( L"EffectSet_Summon_Pet", this );
				if ( CX2EffectSet::EffectSetInstance* pEffect = g_pData->GetUIEffectSet()->GetEffectSetInstance( hEffect ) )
                {
					pEffect->m_vPosition = vPos;
                }
			}			
			PlaySound( L"Pet_Summon.ogg" );
		}
		break;
	case PCE_EVOLUTION:
		{
			if( g_pData != NULL && g_pData->GetUIEffectSet() != NULL )
			{
				CX2EffectSet::Handle hEffect =  g_pData->GetUIEffectSet()->PlayEffectSetByPet( L"EffectSet_Evolution_Pet", this );
				if ( CX2EffectSet::EffectSetInstance* pEffect = g_pData->GetUIEffectSet()->GetEffectSetInstance( hEffect ) )
                {
					pEffect->m_vPosition = vPos;
                }
			}			
			PlaySound( L"Pet_Evolution.ogg" );
		}
		break;
	case PCE_CHEER:
		{
			if( g_pData != NULL && g_pData->GetUIEffectSet() != NULL )
			{
				if( m_hEffectCheer != INVALID_EFFECTSET_HANDLE )
				{
					g_pData->GetUIEffectSet()->StopEffectSet( m_hEffectCheer );
				}
				m_hEffectCheer = g_pData->GetUIEffectSet()->PlayEffectSetByPet( L"EffectSet_Cheer_Pet", this );
			}			
		}
		break;
	default:
		break;
	}
}

bool CX2PET::StateChange( int iStateId, bool bForce, bool bCheck )
{
	bool bTableOpen = false;
	bool bResult = true;

	//if( IsActionState(iStateId) == true || iStateId == m_HungryState || iStateId == m_NotKnowState )
	//{
	//	if( iStateId == GetNowState() )
	//		return false;
	//}
	
	if( IsActionState(GetNowState()) && (iStateId == m_HungryState || iStateId == m_NotKnowState) )
		return false;

	if( bCheck == true && IsActionState(iStateId) == true && null != m_optrMasterGameUnit && g_pX2Game != NULL && g_pX2Game->IsHost() == true )
	{
		// 포만도나 친밀도에 따라 iStateId를 변경함
		bResult = GetActionSuccess(iStateId);
	}

	// 친밀도/포만도 실패시 쿨타임 검사 하지 않고 다음 액션 나올 수 있도록 한다.
	if( bForce == false )
	{
		PetStateData& stateData = m_StateList[iStateId];
		if ( stateData.fStateElapsedTime >= stateData.fStateCoolTime )
		{
			if( bResult == true )
				stateData.fStateElapsedTime = 0.0f;
		}
		else
		{
			return false;
		}
	}


	if( iStateId == m_CheerState )
	{
		DoCheer();
	}
	else if( iStateId == m_AttackState )
	{
		if( m_bMyPet == true && g_pChatBox != NULL )
		{
			if( g_pX2Game != NULL )
			{
				int iStep = GetPetInfo().m_Evolution_Step;
				if( iStep > 0 )
					iStep -= 1; 
				wstring wstrSkillName = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)GetPetInfo().m_PetId )->m_vecSkillName[ iStep ];
				wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_6548, "LL", GetPetInfo().m_wstrPetName, wstrSkillName ) );
				g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );			
			}
		}
		
		m_bAttacking = true;
		m_bAttackTrace = false;
		m_fNowMp = 0.f;
		m_fAttackTraceTime = 0.f;
	}	
	
	DrawPetIcon( iStateId );

	bool bPassTimeState = false;
	if( g_pData->GetPetManager()->GetPetStatus( m_petData ) > 0 )
	{
		if( iStateId == m_WaitHabitState )
		{
			bPassTimeState = true;
		}
		if( m_petSyncData.nowState == m_WaitHabitState && iStateId == m_WaitState )
		{
			bPassTimeState = true;
		}
	}	

	if( m_petSyncData.nowState != iStateId && bPassTimeState == false )
		m_fNowStateTimer = 0.f;

	if( g_pData != NULL && g_pData->GetUIEffectSet() != NULL && m_hStateEffect != INVALID_EFFECTSET_HANDLE )
	{
		g_pData->GetUIEffectSet()->StopEffectSet( m_hStateEffect );
		m_hStateEffect = INVALID_EFFECTSET_HANDLE;
	}

	//스테이트를 바꾼다
	m_petSyncData.nowState		= iStateId;
	m_petSyncData.bStateChange	= true;
	m_PetStateData = m_StateList[m_petSyncData.nowState];

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    if ( m_pXSkinAnim != NULL )
        m_pXSkinAnim->ResetOneshotPerformed();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_EventTimeStamp.clear();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	//다음 스테이트를 시작한다
	bTableOpen = m_LuaManager.BeginTableByReference( m_PetStateData.stateID );
	{

		wstring animName;
		LUA_GET_VALUE( m_LuaManager, "ANIM_NAME", animName, L"" );
		

		CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType;
		LUA_GET_VALUE_ENUM ( m_LuaManager, "PLAY_TYPE", playType, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE, CKTDGXSkinAnim::XAP_ONE_WAIT );
		bool bTransition = true;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );

		if( m_pXSkinAnim != NULL )
		{
			if( m_petCondition.bTransitionChange == false || bTransition == false )
				m_pXSkinAnim->ChangeAnim( animName.c_str(), false );
			else
				m_pXSkinAnim->ChangeAnim( animName.c_str(), true );

			m_pXSkinAnim->Play( playType );
		}
		

		float fAnimSpeed = 1.f;
		LUA_GET_VALUE( m_LuaManager, "ANIM_SPEED", fAnimSpeed, 1.f );
		if( m_pXSkinAnim != NULL )
		{
			m_pXSkinAnim->SetPlaySpeed( fAnimSpeed );
		}

		LUA_GET_VALUE( m_LuaManager, "LAND_CONNECT",	m_bLandConnect,	true );
		

		m_LuaManager.GetValue( "G_ACCEL", m_PhysicParam.fGAccel );

		m_LuaManager.GetValue( "DOWN_ACCEL", m_PhysicParam.fDownAccel );

		D3DXVECTOR2 speed = m_PhysicParam.nowSpeed;
		if( m_LuaManager.GetValue( "SPEED_X", speed.x ) == true )
			m_PhysicParam.nowSpeed.x = speed.x;
		if( m_LuaManager.GetValue( "SPEED_Y", speed.y ) == true )
			m_PhysicParam.nowSpeed.y = speed.y;

#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
		int index = 0;
		m_vecSpeedFactor.clear();
		while( m_LuaManager.BeginTable( "SPEED_TIME", index ) == true )
		{
			TIME_SPEED timeSpeed;

			LUA_GET_VALUE( m_LuaManager, 1, timeSpeed.vSpeed.x, 0.0f );		/// X축 속도
			LUA_GET_VALUE( m_LuaManager, 2, timeSpeed.vSpeed.y, 0.0f );		/// Y축 속도
			LUA_GET_VALUE( m_LuaManager, 3, timeSpeed.vSpeed.z, -1.0f );	/// 적용될 애니메이션 타임
			LUA_GET_VALUE( m_LuaManager, 4, timeSpeed.iFlag, 0 );			/// m_PhysicParam.nowSpeed, 1이면 +=, 2이면 = 연산을 수행함

			m_vecSpeedFactor.push_back(timeSpeed);
			index++;
			m_LuaManager.EndTable();
		}
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET

		LUA_GET_VALUE( m_LuaManager, "PASSIVE_SPEED_X", m_PhysicParam.passiveSpeed.x, -1.f );		
		LUA_GET_VALUE( m_LuaManager, "PASSIVE_SPEED_Y", m_PhysicParam.passiveSpeed.y, -1.f );		

		float fSpeedY = -1.f;
		float fPassiveSpeedX = -1.f;
		int iMoveType = 0; // 0: wait, 1: walk, 2: dash, 3: jump, 4: jump dir

		LUA_GET_VALUE( m_LuaManager, "MOVE_TYPE", iMoveType, -1 );
		LUA_GET_VALUE( m_LuaManager, "PASSIVE_SPEED_X_BY_USER", fPassiveSpeedX, 0.f );		
		LUA_GET_VALUE( m_LuaManager, "SPEED_Y_BY_USER", fSpeedY, 0.f );

		UpdateSpeedByUser();		
		
		switch( iMoveType )
		{
		case 0: // wait
			{
				m_PhysicParam.nowSpeed.x = 0.f;
				m_PhysicParam.nowSpeed.y = 0.f;
			}
			break;
		case 1:	// walk
			{
				m_PhysicParam.nowSpeed.x = 0.f;				
				m_PhysicParam.passiveSpeed.x = m_PhysicParam.fWalkSpeed * fPassiveSpeedX;
			}
			break;
		case 2: // dash / fly
			{
				m_PhysicParam.nowSpeed.x = 0.f;
				m_PhysicParam.passiveSpeed.x = m_PhysicParam.fRunSpeed * fPassiveSpeedX;
			}
			break;
		case 3: // jump
			{
				m_PhysicParam.nowSpeed.x = 0.f;
				m_PhysicParam.nowSpeed.y = m_PhysicParam.fJumpSpeed * fSpeedY;
			}
			break;
		case 4: // jump dir
			{
				m_PhysicParam.passiveSpeed.x = m_PhysicParam.fRunSpeed * fPassiveSpeedX;
				m_PhysicParam.nowSpeed.y = m_PhysicParam.fJumpSpeed * fSpeedY;
			}
			break;
		case 5: // back fly
			{
				m_PhysicParam.nowSpeed.x = 0.f;
				m_PhysicParam.passiveSpeed.x = m_PhysicParam.fRunSpeed * fPassiveSpeedX * -1.f;
			}
			break;
		default:
			break;
		}

		if( m_PhysicParam.nowSpeed.y > 0.f )
		{
			D3DXVECTOR3 vStart = GetPos();
			D3DXVECTOR3 vEnd = D3DXVECTOR3( 0.f, 0.f, 0.f );
			bool IsRight = true;
			GetMasterInfo( vEnd, IsRight );
			float fDist = vEnd.y - vStart.y;

			if( fDist >= 300 )					
				m_fExtraJumpSpeed = fDist;
			else 
				m_fExtraJumpSpeed = 0.f;

			m_PhysicParam.nowSpeed.y += m_fExtraJumpSpeed;
		}
		else
		{
			m_fExtraJumpSpeed = 0.f;
		}
		
		if( m_pAI != NULL )
		{
			m_pAI->UpdateJumpSpeed();
		}

		LUA_GET_VALUE( m_LuaManager, "ALLOW_DIR_CHANGE", m_bAllowDirChange, true );

		if( g_pX2Game != NULL && g_pX2Game->IsHost() == true )
		{
			bool bViewTarget;
			LUA_GET_VALUE( m_LuaManager, "VIEW_TARGET", bViewTarget, false );
			if( bViewTarget == true )
			{
				if( NULL != m_pAI && m_pAI->IsTargetFront() == false )
				{
					m_bSendReserveStateChange = true;
					m_petSyncData.bIsRight = !m_petSyncData.bIsRight;
				}
			}
		}		

#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
		index = 0;
#else //EXPAND_SPEED_TIME_SCRIPT_ON_PET
		int index = 0;
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET
		m_EventIntervalList.resize(0);

		while( true )
		{
			char key[100] = {0,};
			StringCchPrintfA( key, 100, "EVENT_INTERVAL_TIME%d", index );
			//wsprintf( key, L"EVENT_INTERVAL_TIME%d", index );

			D3DXVECTOR3 Interval;
			if( m_LuaManager.GetValue( key, Interval.x ) == false )
				break;

			Interval.y = Interval.x;
			Interval.z = 0.0f;
			m_EventIntervalList.push_back( Interval );

			index++;
		}

		for(UINT i = 0; i < m_PetStateData.m_vecpPetEventProcessData.size(); ++i)
		{
			SAFE_DELETE( m_PetStateData.m_vecpPetEventProcessData[i] );
		}
		m_PetStateData.m_vecpPetEventProcessData.clear();

		if( m_LuaManager.BeginTable( "EVENT_PROCESS" ) == true )
		{
			int index = 1;
			while( m_LuaManager.BeginTable( index ) == true )
			{
				PetEventProcessData* pEventProcessData = new PetEventProcessData;

				LUA_GET_VALUE_ENUM( m_LuaManager,	1,	pEventProcessData->m_StateChangeType,		STATE_CHANGE_TYPE, SCT_NO_CHANGE );
				string strNextState;
				LUA_GET_VALUE_UTF8( m_LuaManager,		2,	strNextState,			"" );
				pEventProcessData->m_NextStateID = GetStateID( strNextState );
				ASSERT( pEventProcessData->m_NextStateID >= 0 );
				LUA_GET_VALUE_UTF8( m_LuaManager,		3,	pEventProcessData->m_strCustomCondition,	"" );

				// condition table 삭제함...

				m_PetStateData.m_vecpPetEventProcessData.push_back( pEventProcessData );
				m_LuaManager.EndTable(); // index 
				index++;
			}
			m_LuaManager.EndTable(); // EVENT_PROCESS
		}
	

#ifdef AI_FLY
		bool bFlyAI = false;
		if( m_LuaManager.GetValue( "FLY_AI", bFlyAI ) == true)
		{
			if(m_pAI != NULL)
			{
				m_pAI->SetAiFlyNowState(bFlyAI);
			}		
		}
		else
		{
			if(m_pAI != NULL)
			{
				m_pAI->SetAiFlyNowState( m_pAI->GetAIFly() );
			}	
		}

		LUA_GET_VALUE( m_LuaManager, "ENABLE_HEIGHT_FIX",		m_bFixFlyHeight,		false );	

#endif

		m_vecSoundPlayData.resize(0);
		while( m_LuaManager.BeginTable( "SOUND_PLAY", index ) == true )
		{
			SoundPlayData sndPlayData;

			LUA_GET_VALUE( m_LuaManager, 1, sndPlayData.m_SoundPlayTime,		-1.0f	);
			LUA_GET_VALUE( m_LuaManager, 2, sndPlayData.m_SoundPlayName,		L""		);
			LUA_GET_VALUE( m_LuaManager, 3, sndPlayData.m_SoundPlayRate,		100		);
			LUA_GET_VALUE( m_LuaManager, 4, sndPlayData.m_bOnlyIfMyUnit,		false	);
			LUA_GET_VALUE( m_LuaManager, 5, sndPlayData.m_fMaxSoundDistance,	-1.f	);

			m_vecSoundPlayData.push_back( sndPlayData );

			index++;
			m_LuaManager.EndTable();
		}
	

		string strTalkBox = "TALK_BOX";
		float fNowIntimacy = GetNowIntimacy();
		if( fNowIntimacy < 0.2f )
			fNowIntimacy = 0.2f;

		if( fNowIntimacy < 0.3f )
		{
			strTalkBox = "TALK_BOX_1";
		}
		else if( fNowIntimacy < 0.7f)
		{
			strTalkBox = "TALK_BOX_2";
		}
		else
		{
			strTalkBox = "TALK_BOX";
		}

		if( m_LuaManager.BeginTable( strTalkBox.c_str() ) == true )
		{
			int iRateLowerBound = 0, iRateUpperBound = 0;
			int iRate = 0;
			int randomNumber = rand()%100;

			wstring wstrMessage;				
			int index = 1;
			while( m_LuaManager.BeginTable( index ) == true )
			{	
				LUA_GET_VALUE( m_LuaManager, "RATE",		iRate,			0 );
				int iStringIndex;
				LUA_GET_VALUE( m_LuaManager, "MESSAGE",	iStringIndex,	STR_ID_EMPTY );
				wstrMessage = GET_STRING( iStringIndex );

				bool bApplyIntimacy = false;
				LUA_GET_VALUE( m_LuaManager, "APPLY_INTIMACY",	bApplyIntimacy,	false );
				
				if( bApplyIntimacy == true )
				{
					iRateUpperBound += (int)(iRate * (fNowIntimacy * 100.f));
				}
				else
				{
					iRateUpperBound += iRate;
				}				
				
#ifdef DIALOG_SHOW_TOGGLE
				if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
					randomNumber = iRateLowerBound - 1;
#endif
				if( iRateLowerBound < randomNumber && randomNumber <= iRateUpperBound )
				{
					CX2TalkBoxManager* pTalkBoxMgr = NULL;
					pTalkBoxMgr = g_pData->GetPetManager()->GetTalkBoxManager();

					if( NULL != pTalkBoxMgr )
					{
						CX2TalkBoxManagerImp::TalkBox talkBox;							
						talkBox.m_OwnerUnitUID		= m_MasterUid;
						talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;						
						talkBox.m_bTraceUnit		= true;
						talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
						talkBox.m_wstrTalkContent	= wstrMessage;
						talkBox.m_bPetTalk			= true;
						talkBox.m_BackTexColor		= D3DXCOLOR( 1.f, 0.957f, 0.765f, 1.f );
						pTalkBoxMgr->Push( talkBox );

						m_LuaManager.EndTable();
						break;
					}
				}

				iRateLowerBound += iRate;

				m_LuaManager.EndTable();
				index++;
			}
			m_LuaManager.EndTable(); // TALK_BOX
		}
	}
	if( bTableOpen == true )
		m_LuaManager.EndTable();	
	
#ifdef GHOST_PET
	if( false == m_PetStateData.Lua_Start.empty() )
	{
		//string func;
		//ConvertWCHARToChar( func, m_PetStateData.Lua_Start.c_str() );
#ifdef LEAVE_LAST_ERROR_LOG_TEST
		LastErrorLog( m_PetStateData.Lua_Start.c_str() );
#endif LEAVE_LAST_ERROR_LOG_TEST

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		lua_tinker::call<void>( m_LuaManager.GetLuaState(), m_PetStateData.Lua_Start.c_str(), g_pKTDXApp, g_pX2Game, this );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_PetStateData.Lua_Start.c_str(), g_pKTDXApp, g_pX2Game, this );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	}
#endif

	return bResult;
}

void CX2PET::PushAIMessage( STATE_CHANGE_TYPE message, bool bIsRight )
{
	m_AIMessage = message; 
	m_bAIIsRight = bIsRight;
}

void CX2PET::DoProcessEvent()
{	
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = NULL;

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	}
	else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
	{
		if( g_pTFieldGame != NULL )
		{
			pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
		}
		else if( g_pSquareGame != NULL )
		{
			pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
		}
	}

	if( m_AIMessage == SCT_AI_DIR_CHANGE )
	{
		if( m_bAllowDirChange == true )
		{			
			m_bSendReserveStateChange = true;
			m_petSyncData.bIsRight = m_bAIIsRight;
		}
	}

	for( int i = 0; i < (int)m_PetStateData.m_vecpPetEventProcessData.size(); i++ )
	{
		PetEventProcessData* pEventProcessData = m_PetStateData.m_vecpPetEventProcessData[i];

		if ( pEventProcessData == NULL )
			continue;


		switch( pEventProcessData->m_StateChangeType )
		{				
		case SCT_MOTION_END:
			{						
				if( m_pXSkinAnim != NULL && m_pXSkinAnim->IsAnimationEnd() == true )
				{					
					if( m_bAttacking == true )
					{
						m_bAttacking = false;
						m_bAttackTrace = false;
						m_fAttackTraceTime = 0.f;
						InitOtherTarget();
					}
					StateChange( pEventProcessData->m_NextStateID );
				}
			}
			break;

		case SCT_FOOT_ON_LINE_FALSE_DOWN:
			{
				if( m_petCondition.bFootOnLine == false )
				{					
					m_petSyncData.position.y -= LINE_RADIUS * 1.5f;	
					StateChange( pEventProcessData->m_NextStateID );					
				}
			}
			break;

		case SCT_FOOT_ON_LINE_TRUE:
			{
				if( m_petCondition.bFootOnLine == true )
				{					
					StateChange( pEventProcessData->m_NextStateID );
				}
			}
			break;

		case SCT_POSITIVE_Y_SPEED:
			{
				if( m_PhysicParam.nowSpeed.y > 0.0f == true )
				{					
					StateChange( pEventProcessData->m_NextStateID );
				}
			}
			break;

		case SCT_NEGATIVE_Y_SPEED:
			{
				if( m_PhysicParam.nowSpeed.y <= 0.0f == true )
				{					
					StateChange( pEventProcessData->m_NextStateID );
				}
			}
			break;

		case SCT_AI_WAIT:
			{
				if( m_AIMessage == SCT_AI_WAIT || m_UnUseAIMessage == SCT_AI_WAIT )
				{					
					StateChange( pEventProcessData->m_NextStateID );
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;
				}
			}
			break;

		case SCT_AI_WALK:
			{
				if( m_AIMessage == SCT_AI_WALK || m_UnUseAIMessage == SCT_AI_WALK )
				{
					PetStateData stateData = m_StateList[ pEventProcessData->m_NextStateID ];
					if( stateData.stateID != m_petSyncData.nowState || m_petSyncData.bIsRight != m_bAIIsRight )
					{						
						m_petSyncData.bIsRight = m_bAIIsRight;
						m_bSendReserveStateChange = true;
						StateChange( pEventProcessData->m_NextStateID );
					}							
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;

		case SCT_AI_DASH:
			{
				if( m_AIMessage == SCT_AI_DASH || m_UnUseAIMessage == SCT_AI_DASH )
				{					
					m_petSyncData.bIsRight = m_bAIIsRight;
					m_bSendReserveStateChange = true;
					StateChange( pEventProcessData->m_NextStateID );
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;

		case SCT_AI_JUMP:
			{
				if( m_AIMessage == SCT_AI_JUMP || m_UnUseAIMessage == SCT_AI_JUMP )
				{					

					StateChange( pEventProcessData->m_NextStateID );
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;
				}
			} 
			break;

		case SCT_AI_DOWN:
			{
				if( pLineMap != NULL )
				{
					if( m_AIMessage == SCT_AI_DOWN || m_UnUseAIMessage == SCT_AI_DOWN )
					{
						D3DXVECTOR3 vTempLandPosition = m_petCondition.landPosition;
						int iTempLastLineIndex = m_petSyncData.lastTouchLineIndex;

						bool bCanDown = pLineMap->CanDown( m_petSyncData.position, 
							LINE_RADIUS, &vTempLandPosition, &iTempLastLineIndex, true, true );

						if( true == bCanDown )
						{						
							m_petSyncData.position.y -= LINE_RADIUS * 1.5f;											
							StateChange( pEventProcessData->m_NextStateID );
							m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;
						}
					}
				}				
			} 
			break;	

		case SCT_AI_JUMP_DIR:
			{
				if( m_AIMessage == SCT_AI_JUMP_DIR || m_UnUseAIMessage == SCT_AI_JUMP_DIR )
				{					
					m_petSyncData.bIsRight = m_bAIIsRight;
					m_bSendReserveStateChange = true;
					StateChange( pEventProcessData->m_NextStateID );
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;
				}
			}
			break;

		case SCT_AI_DOWN_DIR:
			{
				if( m_AIMessage == SCT_AI_DOWN_DIR || m_UnUseAIMessage == SCT_AI_DOWN_DIR )
				{					
					m_petSyncData.bIsRight = m_bAIIsRight;
					m_bSendReserveStateChange = true;
					m_petSyncData.position.y -= LINE_RADIUS * 1.5f;													
					StateChange( pEventProcessData->m_NextStateID );
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;
				}
			}
			break;

#ifdef AI_FLY
		case SCT_AI_FLY_FRONT:
			{
				if( m_AIMessage == SCT_AI_FLY_FRONT || m_UnUseAIMessage == SCT_AI_FLY_FRONT )
				{
					PetStateData stateData = m_StateList[ pEventProcessData->m_NextStateID ];
					if( stateData.stateID != m_petSyncData.nowState || m_petSyncData.bIsRight != m_bAIIsRight )
					{
						m_petSyncData.bIsRight = m_bAIIsRight;
						m_bSendReserveStateChange = true;
						StateChange( pEventProcessData->m_NextStateID );
					}							
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;
		case SCT_AI_FLY_FRONT_UP:
			{
				if( m_AIMessage == SCT_AI_FLY_FRONT_UP || m_UnUseAIMessage == SCT_AI_FLY_FRONT_UP )
				{
					PetStateData stateData = m_StateList[ pEventProcessData->m_NextStateID ];
					if( stateData.stateID != m_petSyncData.nowState || m_petSyncData.bIsRight != m_bAIIsRight )
					{
						m_petSyncData.bIsRight = m_bAIIsRight;
						m_bSendReserveStateChange = true;
						StateChange( pEventProcessData->m_NextStateID );
					}							
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;
		case SCT_AI_FLY_FRONT_DOWN:
			{
				if( m_AIMessage == SCT_AI_FLY_FRONT_DOWN || m_UnUseAIMessage == SCT_AI_FLY_FRONT_DOWN )
				{
					PetStateData stateData = m_StateList[ pEventProcessData->m_NextStateID ];
					if( stateData.stateID != m_petSyncData.nowState || m_petSyncData.bIsRight != m_bAIIsRight )
					{
						m_petSyncData.bIsRight = m_bAIIsRight;
						m_bSendReserveStateChange = true;
						StateChange( pEventProcessData->m_NextStateID );
					}							
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;
		case SCT_AI_FLY_BACK:
			{
				if( m_AIMessage == SCT_AI_FLY_BACK || m_UnUseAIMessage == SCT_AI_FLY_BACK )
				{
					PetStateData stateData = m_StateList[ pEventProcessData->m_NextStateID ];
					if( stateData.stateID != m_petSyncData.nowState || m_petSyncData.bIsRight != m_bAIIsRight )
					{
						m_petSyncData.bIsRight = m_bAIIsRight;
						m_bSendReserveStateChange = true;
						StateChange( pEventProcessData->m_NextStateID );
					}							
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;
		case SCT_AI_FLY_BACK_UP:
			{
				if( m_AIMessage == SCT_AI_FLY_BACK_UP || m_UnUseAIMessage == SCT_AI_FLY_BACK_UP )
				{
					PetStateData stateData = m_StateList[ pEventProcessData->m_NextStateID ];
					if( stateData.stateID != m_petSyncData.nowState || m_petSyncData.bIsRight != m_bAIIsRight )
					{
						m_petSyncData.bIsRight = m_bAIIsRight;
						m_bSendReserveStateChange = true;
						StateChange( pEventProcessData->m_NextStateID );
					}							
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;
		case SCT_AI_FLY_BACK_DOWN:
			{
				if( m_AIMessage == SCT_AI_FLY_BACK_DOWN || m_UnUseAIMessage == SCT_AI_FLY_BACK_DOWN )
				{
					PetStateData stateData = m_StateList[ pEventProcessData->m_NextStateID ];
					if( stateData.stateID != m_petSyncData.nowState || m_petSyncData.bIsRight != m_bAIIsRight )
					{
						m_petSyncData.bIsRight = m_bAIIsRight;
						m_bSendReserveStateChange = true;
						StateChange( pEventProcessData->m_NextStateID );
					}							
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;
		case SCT_AI_FLY_UP:
			{
				if( m_AIMessage == SCT_AI_FLY_UP || m_UnUseAIMessage == SCT_AI_FLY_UP )
				{					
					m_petSyncData.bIsRight = m_bAIIsRight;
					m_bSendReserveStateChange = true;
					StateChange( pEventProcessData->m_NextStateID );
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;								
				}
			}
			break;
		case SCT_AI_FLY_DOWN:
			{
				if( m_AIMessage == SCT_AI_FLY_DOWN || m_UnUseAIMessage == SCT_AI_FLY_DOWN )
				{
					m_petSyncData.bIsRight = m_bAIIsRight;
					m_bSendReserveStateChange = true;
					StateChange( pEventProcessData->m_NextStateID );
					m_AIMessage = m_UnUseAIMessage = SCT_NO_CHANGE;					
				}
			}
			break;

#endif

		case SCT_CONDITION_FUNCTION:
			{
#ifdef LEAVE_LAST_ERROR_LOG_TEST
				LastErrorLog( pEventProcessData->m_strCustomCondition.c_str() );
#endif LEAVE_LAST_ERROR_LOG_TEST

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				bool bRt = lua_tinker::call<bool>( m_LuaManager.GetLuaState(), pEventProcessData->m_strCustomCondition.c_str(), g_pKTDXApp, g_pX2Game, this );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
				bool bRt = lua_tinker::call<bool>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), pEventProcessData->m_strCustomCondition.c_str(), g_pKTDXApp, g_pX2Game, this );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

				if( true == bRt )
				{
					StateChange( pEventProcessData->m_NextStateID, true, false );													
				}
			}
			break;

		case SCT_CONDITION_TABLE:
			{
			}
			break;


		default:
			if( m_AIMessage > SCT_AI_MESSAGE_BEGIN && m_AIMessage < SCT_AI_MESSAGE_END )
			{						
				m_UnUseAIMessage = m_AIMessage;
			}
			break;

		} // end of switch( stateChangeType )

		if( m_petSyncData.bStateChange == true )
		{
			m_petSyncData.bStateChange = false;
			break;
		}
	}




//	if( false == m_PetStateData.Lua_EventProcess.empty() )
//	{
//		string func;
//		ConvertWCHARToChar( func, m_PetStateData.Lua_EventProcess.c_str() );
//#ifdef LEAVE_LAST_ERROR_LOG_TEST
//		LastErrorLog( func.c_str() );
//#endif LEAVE_LAST_ERROR_LOG_TEST
//
//		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), func.c_str(), g_pKTDXApp, g_pX2Game, this );
//	}
}

bool CX2PET::SetPosition( D3DXVECTOR3& vPos, bool bIsRight )
{
	KTDXPROFILE();

	CKTDGLineMap* pLineMap = NULL;

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	}
	else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
	{
		if( g_pTFieldGame != NULL && g_pTFieldGame->GetWorld() != NULL )
		{
			pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
		}
		else if( g_pSquareGame != NULL && g_pSquareGame->GetWorld() != NULL )
		{
			pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
		}
	}

	if( NULL == pLineMap )
		return false;	

	if( pLineMap->GetAnyEnabledLine() == false )
		return false;

	D3DXVECTOR3		startPos;

	startPos = vPos;
	m_petSyncData.bIsRight = bIsRight;	

	//정확한 라인 보정위치와 라인 인덱스를 구한다
	int lineIndex = -1;
	D3DXVECTOR3	landPos = startPos;
	landPos = pLineMap->GetLandPosition( startPos, LINE_RADIUS, &lineIndex );
	//pLineMap->IsOnLineConnect( startPos, &lineIndex, LINE_RADIUS, &landPos );
	pLineMap->IsOnLine( startPos, LINE_RADIUS, &startPos, &lineIndex, true );

	const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( lineIndex );
	if( NULL == pLineData )
	{
		ASSERT( !"CX2GUNPC::SetPosition, null linedata" );
		return false; 
	}
	//startPos = landPos;

	//최초 프레임 데이타에 업데이트 한다.
	m_petSyncData.position				= startPos;
	m_petCondition.dirDegree			= pLineData->dirDegree;
	m_petCondition.dirVector			= pLineData->dirVector;
	m_petCondition.landPosition			= landPos;
	m_petSyncData.lastTouchLineIndex	= lineIndex;
	if( m_petSyncData.bIsRight == false )
		m_petCondition.dirDegree.y		+= 180.0f;

	return true;
}

void CX2PET::PhysicProcess()
{
	KTDXPROFILE();


	//물리 파라메터 계산//////////////////////////////////////////////////////////
	D3DXVECTOR3 position;
	D3DXVECTOR3 positionOrg;
	D3DXVECTOR3 angleDegree;


	//현재 정보 받아오기
	positionOrg = position = m_petSyncData.position;
	angleDegree	= m_petCondition.dirDegree;


	CKTDGLineMap* pLineMap = NULL;

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	}
	else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
	{
		if( g_pTFieldGame != NULL )
		{
			pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
		}
		else if( g_pSquareGame != NULL )
		{
			pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
		}
		
	}

	if( NULL == pLineMap )
		return;	


	const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( m_petSyncData.lastTouchLineIndex );
	if( pLineData == NULL )
	{		
		pLineData = pLineMap->GetLineData( m_petSyncData.lastTouchLineIndex );
	}
	
	if( NULL == pLineData )
		return; 

	//#pragma NOTE( "움직이지 않는 몬스터의 경우에 여기 코드 실행되지 않도록 고치기, 코드 정리, 함수 나누기도 같이 하면 좋음" )
	//if( false == m_bNeverMove )
	//{
#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
	// 스크립트 상에서 특정 시간에 특정스피드를 지정해 줄수 있도록한다.
	bSpeedTime= false;
	float fAnimTime = -1.f;
	if( m_pXSkinAnim != NULL )
	{
		fAnimTime = m_pXSkinAnim->GetNowAnimationTime();
	}

	if( m_pXSkinAnim != NULL && m_vecSpeedFactor.size() > 0)
	{
		bSpeedTime = true;
		for(UINT i=0; i<m_vecSpeedFactor.size(); ++i)
		{
			TIME_SPEED timeSpeed = m_vecSpeedFactor[i];

			if( timeSpeed.iFlag > 0 && timeSpeed.vSpeed.z >= 0.f && timeSpeed.vSpeed.z <= fAnimTime )
			{
				switch(timeSpeed.iFlag)
				{
				case 1:
					m_PhysicParam.nowSpeed.x += timeSpeed.vSpeed.x;
					m_PhysicParam.nowSpeed.y += timeSpeed.vSpeed.y;
					break;
				case 2:
					m_PhysicParam.nowSpeed.x = timeSpeed.vSpeed.x;
					m_PhysicParam.nowSpeed.y = timeSpeed.vSpeed.y;
					break;
				default:
					break;
				}				

				m_vecSpeedFactor.erase(m_vecSpeedFactor.begin() + i);
				--i;
			}
		}
	}
	else
	{
		bSpeedTime = false;
	}
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET
	//X감쇄
	bool positive;
	if( m_PhysicParam.nowSpeed.x >= 0.0f )
		positive = true;
	else
		positive = false;
	if( positive == true )
	{
		m_PhysicParam.nowSpeed.x -= m_PhysicParam.fReloadAccel * m_fElapsedTime;
		if( m_PhysicParam.nowSpeed.x <= 0.0f )
			m_PhysicParam.nowSpeed.x = 0.0f;
	}
	else
	{
		m_PhysicParam.nowSpeed.x += m_PhysicParam.fReloadAccel * m_fElapsedTime;
		if( m_PhysicParam.nowSpeed.x > 0.0f )
			m_PhysicParam.nowSpeed.x = 0.0f;
	}


#ifdef AI_FLY
	if( m_bLandConnect == false && m_pAI != NULL && m_pAI->GetAIFlyNowState() == false )
#endif
	{
		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fGAccel * m_fElapsedTime;
		if( m_PhysicParam.nowSpeed.y <= m_PhysicParam.fMaxGSpeed )
			m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;

		m_PhysicParam.nowSpeed.y -= m_PhysicParam.fDownAccel * m_fElapsedTime;
		if( m_PhysicParam.nowSpeed.y <= m_PhysicParam.fMaxGSpeed )
			m_PhysicParam.nowSpeed.y = m_PhysicParam.fMaxGSpeed;
	}
	else
	{
		m_PhysicParam.nowSpeed.y = 0.0f;
	}



	//X이동
	m_petCondition.dirVector = pLineData->dirVector;
	
	if( m_bAllowDirChange == true )
	{
		if( m_petCondition.bFootOnLine == false )
		{
			D3DXVECTOR3 dirVec = m_petCondition.dirVector;
			dirVec.y = 0.0f;

			if( m_petSyncData.bIsRight == true )
				position += dirVec * m_PhysicParam.nowSpeed.x * m_fElapsedTime;
			else
				position -= dirVec * m_PhysicParam.nowSpeed.x * m_fElapsedTime;
		}
		else
		{
			if( m_petSyncData.bIsRight == true )
				position += m_petCondition.dirVector * m_PhysicParam.nowSpeed.x * m_fElapsedTime;
			else
				position -= m_petCondition.dirVector * m_PhysicParam.nowSpeed.x * m_fElapsedTime;
		}
	}


	//라인 스피드 적용
	//if( false == m_bIgnoreLineSpeed )
	//{
	//	if( true == m_petCondition.bFootOnLine )
	//	{
	//		position += pLineData->dirVector * pLineData->m_fSpeed * m_fElapsedTime;
	//	}
	//}
	

	//Y이동
#ifdef AI_FLY
	if( NULL == m_pAI ||
		(m_pAI != NULL && m_pAI->GetAIFlyNowState() == false) )
#endif
	{
		if( m_PhysicParam.nowSpeed.y > 0.f &&
			false == pLineMap->CanPassUp( m_petSyncData.position ) )
		{
			position.y = m_petSyncData.position.y;
			m_PhysicParam.nowSpeed.y = 0.f;
		}
		else
		{			
			position.y += m_PhysicParam.nowSpeed.y * m_fElapsedTime;
		}
	}


	//지면 계산////////////////////////////////////////////////////////////////////
	bool bTouch	= false;
	int lastTouchLineIndex		= m_petSyncData.lastTouchLineIndex;
	D3DXVECTOR3 landPosition	= m_petCondition.landPosition;

#ifdef AI_FLY
	CX2PETAI* pNormalAI = NULL;
	if( m_pAI != NULL )
	{
		pNormalAI = (CX2PETAI*) m_pAI;
	}

	if(pNormalAI != NULL && pNormalAI->GetAIFlyNowState() == true)
	{
		bTouch = pLineMap->IsOnLine( position, LINE_RADIUS, &landPosition, &lastTouchLineIndex );

		if(bTouch == true)
		{
			//pLineMap->IsOnLineConnect( position, &lastTouchLineIndex, LINE_RADIUS, &landPosition, true );
			position.x = landPosition.x;
			position.z = landPosition.z;
		}
		else
		{
			const CKTDGLineMap::LineData* pDwnLineData = pLineMap->GetLineData( lastTouchLineIndex );
			bool bMeOnStart = false;
			bool bMeOnEnd = false;
			if( GetDistance(landPosition,pDwnLineData->startPos) < LINE_RADIUS )
				bMeOnStart = true;
			else if( GetDistance(landPosition,pDwnLineData->endPos) < LINE_RADIUS )
				bMeOnEnd = true;

#ifdef UNIT_ROAD
			if(pDwnLineData != NULL && pDwnLineData->lineType == CKTDGLineMap::LT_UNIT_ROAD)
			{
				bMeOnStart = bMeOnEnd = false;
			}
#endif
			if( bMeOnStart == true && pDwnLineData->beforeLine == -2 )
			{				
				//if( position.y < landPosition.y )
				//	position.y = landPosition.y;

				landPosition = pLineMap->GetLandPosition( position, LINE_RADIUS, &lastTouchLineIndex );
				position.x = landPosition.x;
				position.z = landPosition.z;
			}
			else if( bMeOnEnd == true && pDwnLineData->nextLine == -2 )
			{
				//if( position.y < landPosition.y )
				//	position.y = landPosition.y;

				landPosition = pLineMap->GetLandPosition( position, LINE_RADIUS, &lastTouchLineIndex );
				position.x = landPosition.x;
				position.z = landPosition.z;
			}
			else
			{
				bool bCanDown = pLineMap->CanDown( position, LINE_RADIUS, &landPosition, &lastTouchLineIndex, true, false, false );

				if( bCanDown == true ) //아래쪽에 발판이 있어서 내려갈 수가 있다면
				{
					//수평 위치를 아래쪽 선에 싱크한다.
					position.x = landPosition.x;
					position.z = landPosition.z;

					//if( m_PhysicParam.nowSpeed.y <= 0.0f )
					//{
					//	m_petCondition.bFall = true;
					//}
				}
				else //더이상 아래쪽에 발판이 없어 떨어질 상황이면
				{
					//position = positionOrg;	//이번 프레임의 이동을 캔슬한다.
					position.x = positionOrg.x;
					position.z = positionOrg.z;
					//if( position.y < landPosition.y )
					//	position.y = landPosition.y;
				}
			}
		}

		if( m_bFixFlyHeight == false ) //&& (speedY <= -10.f || speedY >= 10.f) )
		{				
			position.y += m_PhysicParam.passiveSpeed.y;
		}		
	}
	else
#endif AI_FLY
	{
		bTouch = pLineMap->IsOnLine( position, LINE_RADIUS, &landPosition, &lastTouchLineIndex );

		if( bTouch == true ) //이동 위치가 선 위라면
		{
			if( m_PhysicParam.nowSpeed.y > 0.0f )
			{
				//상승 중이면 수평 위치만 선에 싱크한다.
				position.x = landPosition.x;
				position.z = landPosition.z;
			}
			else
			{
				//하강이나 정지상태면 위치를 선에 싱크한다.
				position = landPosition;

				//땅을 밟았으므로 Y속도를 없앤다.
				m_PhysicParam.nowSpeed.y = 0.0f;
				m_PhysicParam.fDownAccel = 0.0f;
			}
		}
		else //이동 위치가 선 위가 아니라면
		{
			const CKTDGLineMap::LineData* pDwnLineData = pLineMap->GetLineData( lastTouchLineIndex );
			bool bMeOnStart = false;
			bool bMeOnEnd = false;
			if( GetDistance(landPosition,pDwnLineData->startPos) < LINE_RADIUS )
				bMeOnStart = true;
			else if( GetDistance(landPosition,pDwnLineData->endPos) < LINE_RADIUS )
				bMeOnEnd = true;

#ifdef UNIT_ROAD
			if(pDwnLineData != NULL && pDwnLineData->lineType == CKTDGLineMap::LT_UNIT_ROAD)
			{
				bMeOnStart = bMeOnEnd = false;
			}
#endif
			if( bMeOnStart == true && pDwnLineData->beforeLine == -2 )
			{				
				if( position.y < landPosition.y )
					position.y = landPosition.y;

				landPosition = pLineMap->GetLandPosition( position, LINE_RADIUS, &lastTouchLineIndex );
				position.x = landPosition.x;
				position.z = landPosition.z;
			}
			else if( bMeOnEnd == true && pDwnLineData->nextLine == -2 )
			{
				if( position.y < landPosition.y )
					position.y = landPosition.y;

				landPosition = pLineMap->GetLandPosition( position, LINE_RADIUS, &lastTouchLineIndex );
				position.x = landPosition.x;
				position.z = landPosition.z;
			}
			else
			{
				bool bCanDown = pLineMap->CanDown( position, LINE_RADIUS, &landPosition, &lastTouchLineIndex, true, false, false );

				if( bCanDown == true ) //아래쪽에 발판이 있어서 내려갈 수가 있다면
				{
					//수평 위치를 아래쪽 선에 싱크한다.
					position.x = landPosition.x;
					position.z = landPosition.z;

					//if( m_PhysicParam.nowSpeed.y <= 0.0f )
					//{
					//	m_petCondition.bFall = true;
					//}
				}
				else //더이상 아래쪽에 발판이 없어 떨어질 상황이면
				{

					//position = positionOrg;	//이번 프레임의 이동을 캔슬한다.
					position.x = positionOrg.x;
					position.z = positionOrg.z;
					if( position.y < landPosition.y )
						position.y = landPosition.y;
				}
			}
		}
	}



	//if( lastTouchLineIndex < 0 || pLineMap->GetLineData( m_petSyncData.lastTouchLineIndex )->bEnable == false )
	//{
	//	lastTouchLineIndex = pLineMap->GetNearestLine( position, &landPosition );
	//}

	m_petSyncData.lastTouchLineIndex	= lastTouchLineIndex;
	m_petCondition.landPosition	= landPosition;


	//라인정보를 가지고 온다
	pLineData = pLineMap->GetLineData( m_petSyncData.lastTouchLineIndex );


	D3DXVECTOR3 dirDegree( 0, 0, 0 );

	if ( pLineData != NULL )
	{
		//싱크를 맞출 발판이 정해졌다면 앵글 정보를 맞춘다.

		if( m_petSyncData.bIsRight == true )
		{
			dirDegree.x = pLineData->dirDegree.x;
			dirDegree.y = pLineData->dirDegree.y;
			//angle.z = D3DXToRadian(lineData.dirDegree.z);
		}
		else
		{
			dirDegree.x = pLineData->dirDegree.x;
			dirDegree.y = pLineData->dirDegree.y + 180.0f;
			//angle.z = D3DXToRadian(-lineData.dirDegree.z);
		}
	}


	//위치 업데이트////////////////////////////////////////////////////////////////

	m_petSyncData.position			= position;
	m_petCondition.dirDegree		= dirDegree;



	if( m_petSyncData.position.y == m_petCondition.landPosition.y )
		m_petCondition.bFootOnLine = true;
	else
		m_petCondition.bFootOnLine = false;

	//}
}


void CX2PET::GetMasterInfo( D3DXVECTOR3 &vPos, bool &IsRight )
{
	vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	IsRight = true;

	if( m_MasterUid <= -1 )	
		return;

	if( g_pX2Game != NULL )
	{
		CX2GUUser* pGUUser = (CX2GUUser*)g_pX2Game->GetUserUnitByUID( m_MasterUid );
		if( NULL != pGUUser )
		{
			vPos = pGUUser->GetPos();
			IsRight = pGUUser->GetIsRight();
		}			
	}
	else if( g_pTFieldGame != NULL )
	{
		CX2SquareUnit* pUser = (CX2SquareUnit*)g_pTFieldGame->GetSquareUnitByUID( m_MasterUid );
		if( NULL != pUser )
		{			
			vPos = pUser->GetPos();
			IsRight =  pUser->GetIsRight();
		}			
	}
	else if( g_pSquareGame != NULL )
	{
		CX2SquareUnit* pUser = (CX2SquareUnit*)g_pSquareGame->GetSquareUnitByUID( m_MasterUid );
		if( NULL != pUser )
		{
			vPos = pUser->GetPos();
			IsRight =  pUser->GetIsRight();
		}			
	}	
}

void CX2PET::UpdateSpeedByUser()
{	
	if( m_MasterUid <= -1 )	
		return;

	if( g_pX2Game != NULL )
	{
		CX2GUUser* pGUUser = (CX2GUUser*)g_pX2Game->GetUserUnitByUID( m_MasterUid );
		if( NULL != pGUUser )
		{
			m_PhysicParam.fWalkSpeed = pGUUser->GetWalkSpeed();
			m_PhysicParam.fRunSpeed = pGUUser->GetDashSpeed();
			m_PhysicParam.fJumpSpeed = pGUUser->GetJumpSpeed();
			m_PhysicParam.fDashJumpSpeed = pGUUser->GetDashJumpSpeed();
		}			
	}
	else if( g_pTFieldGame != NULL )
	{
		CX2SquareUnit* pUser = (CX2SquareUnit*)g_pTFieldGame->GetSquareUnitByUID( m_MasterUid );
		if( NULL != pUser )
		{			
			m_PhysicParam.fWalkSpeed = pUser->GetPhysicParam().GetWalkSpeed();
			m_PhysicParam.fRunSpeed = pUser->GetPhysicParam().GetDashSpeed();
			m_PhysicParam.fJumpSpeed = pUser->GetPhysicParam().GetJumpSpeed();
			m_PhysicParam.fDashJumpSpeed = pUser->GetPhysicParam().GetDashJumpSpeed();
		}			
	}
	else if( g_pSquareGame != NULL )
	{
		CX2SquareUnit* pUser = (CX2SquareUnit*)g_pSquareGame->GetSquareUnitByUID( m_MasterUid );
		if( NULL != pUser )
		{
			m_PhysicParam.fWalkSpeed = pUser->GetPhysicParam().GetWalkSpeed();
			m_PhysicParam.fRunSpeed = pUser->GetPhysicParam().GetDashSpeed();
			m_PhysicParam.fJumpSpeed = pUser->GetPhysicParam().GetJumpSpeed();
			m_PhysicParam.fDashJumpSpeed = pUser->GetPhysicParam().GetDashJumpSpeed();
		}			
	}	
}

#ifndef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

void CX2PET::InitGamePet()
{
	/*
	if( m_bInit == true )
		return;

	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL && g_pX2Game->GetWorld()->GetLineMap() != NULL )
	{
		if( InitPos() == false )
			return;

		m_bLoaded = true;

		if( m_pXSkinAnim != NULL )
		{
			m_pXSkinAnim->GetMatrix().Scale( D3DXVECTOR3( m_fScale, m_fScale, m_fScale ) );	
			m_pXSkinAnim->SetShowObject(true);
			StateChange( m_StartState );

			SetShowObject(true);

			if( m_bMyPet == true )
				PlayCommonEffect( PCE_SUMMON );

			m_pXSkinAnim->OnFrameMove(g_NowTime, 0.f);
		}

		m_bInit = true;
	}
	//*/
}

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

bool CX2PET::InitPos()
{
	if( m_bInitPos == true )
		return true;

	D3DXVECTOR3 vPos;
	bool IsRight;
	GetMasterInfo( vPos, IsRight );
	
	if( m_bInit == false )
	{
		CKTDGLineMap* pLineMap = NULL;

		if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
		{
			pLineMap = g_pX2Game->GetWorld()->GetLineMap();
		}
		else if( g_pTFieldGame != NULL || g_pSquareGame != NULL ) 
		{
			if( g_pTFieldGame != NULL )
			{
				pLineMap = g_pTFieldGame->GetWorld()->GetLineMap();
			}
			else if( g_pSquareGame != NULL )
			{
				pLineMap = g_pSquareGame->GetWorld()->GetLineMap();
			}
		}

		if( pLineMap == NULL )
			return false;	

		if( pLineMap->GetAnyEnabledLine() == false )
			return false;

		//정확한 라인 보정위치와 라인 인덱스를 구한다
		int lineIndex = -1;
		D3DXVECTOR3 startPos = vPos;
		D3DXVECTOR3	landPos = startPos;

		landPos = pLineMap->GetLandPosition( startPos, LINE_RADIUS, &lineIndex );	
		pLineMap->IsOnLine( startPos, LINE_RADIUS, &startPos, &lineIndex, true );

		const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( lineIndex );
		if( NULL == pLineData )
		{
			return false;
		}
	}
	
	vPos.y += 30.f;
	SetPosition( vPos, IsRight );
	m_bInitPos = true;
	
	if( m_bMyPet == true )
		PlayCommonEffect( PCE_SUMMON );

	return true;
}


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
bool CX2PET::SendPacketImmediateForce( CX2FrameUDPPack& kFrameUDPPack )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void CX2PET::SendPacketImmediateForce( vector<KXPT_UNIT_PET_SYNC>& syncList )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
	KTDXPROFILE();	

	if( m_petSyncData.nowState == m_PreState && m_bSendReserveStateChange == false )
    {
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
		return false;
//#else	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
//		return;
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
    }

	if( g_pX2Game->IsHost() == false )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
		return false;
//#else	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
//		return;
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
	}

	// 싱크 테스트용
	//bool bFind = false;
	//if( m_petSyncData.nowState == m_AttackState )
	//{
	//	bFind = true;
	//}

	KXPT_UNIT_PET_SYNC kXPT_UNIT_PET_SYNC;

	kXPT_UNIT_PET_SYNC.unitUID				= m_MasterUid;
	kXPT_UNIT_PET_SYNC.nextState			= m_petCondition.nextState;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	kXPT_UNIT_PET_SYNC.usNowSpeedX			= floatToHalf( m_PhysicParam.nowSpeed.x );
	kXPT_UNIT_PET_SYNC.usNowSpeedY			= floatToHalf( m_PhysicParam.nowSpeed.y );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	kXPT_UNIT_PET_SYNC.nowSpeedX			= m_PhysicParam.nowSpeed.x;
//	kXPT_UNIT_PET_SYNC.nowSpeedY			= m_PhysicParam.nowSpeed.y;	
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	kXPT_UNIT_PET_SYNC.fNowMP				= floatToHalf( 100.f );
	kXPT_UNIT_PET_SYNC.nowState				= m_petSyncData.nowState;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    _EncodePetSyncPos( kXPT_UNIT_PET_SYNC.ucPosX, kXPT_UNIT_PET_SYNC.ucPosY, kXPT_UNIT_PET_SYNC.ucPosZ, m_petSyncData.position, m_petSyncData.lastTouchLineIndex );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	kXPT_UNIT_PET_SYNC.posX					= m_petSyncData.position.x;
//	kXPT_UNIT_PET_SYNC.posY					= m_petSyncData.position.y;
//	kXPT_UNIT_PET_SYNC.posZ					= m_petSyncData.position.z;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	kXPT_UNIT_PET_SYNC.lastTouchLineIndex	= m_petSyncData.lastTouchLineIndex;
	kXPT_UNIT_PET_SYNC.bIsRight				= m_petSyncData.bIsRight;

	kXPT_UNIT_PET_SYNC.m_usRandomTableIndex	= (USHORT) ( rand() % CKTDXRandomNumbers::RANDOM_NUMBER_COUNT );
	m_petSyncData.m_usRandomTableIndex = kXPT_UNIT_PET_SYNC.m_usRandomTableIndex;
	

	m_PreState = m_petSyncData.nowState;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    kFrameUDPPack.AddFrameUDPPack_IDPack( XPT_UNIT_PET_SYNC_PACK, &kXPT_UNIT_PET_SYNC, sizeof(KXPT_UNIT_PET_SYNC) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	syncList.push_back( kXPT_UNIT_PET_SYNC );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	m_bSendReserveStateChange = false;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
    return true;
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
}

void CX2PET::ReceiveFrameData( const KXPT_UNIT_PET_SYNC& kXPT_UNIT_PET_SYNC )
{
	KTDXPROFILE();
	if( g_pX2Game->IsHost() )
		return;

	//m_ReceivePacketBufList.clear();
	m_ReceivePacketBufList.push_back( kXPT_UNIT_PET_SYNC );	
}

bool CX2PET::EventTimer( float fTime )
{
	KTDXPROFILE();
	if( m_petCondition.fStateTimeBack < fTime
		&& m_petCondition.fStateTime >= fTime )
	{
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        return  true;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		map<float,bool>::iterator iter;
		iter = m_EventTimeStamp.find( fTime );
		if( iter == m_EventTimeStamp.end() )
		{
			m_EventTimeStamp.insert( std::make_pair(fTime,false) );
			return true;
		}
		else
		{
			bool bFramePass = iter->second;

			if( bFramePass == false )
				return true;
			else
				return false;
		}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	}
	else
		return false;
}

#ifndef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
bool CX2PET::EventCheck( float fTime )
{
	KTDXPROFILE();
	map<float,bool>::iterator iter;
	iter = m_EventTimeStamp.find( fTime );
	if( iter == m_EventTimeStamp.end() )
	{
		m_EventTimeStamp.insert( std::make_pair(fTime,false) );
		return true;
	}
	else
	{
		bool bFramePass = iter->second;

		if( bFramePass == false )
			return true;
		else
			return false;
	}
}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

void CX2PET::AttackResult( float fMp )
{
	++m_AttackComboNum;
	m_fTimeAttack = 0.f;

	m_fNowMp += fMp;
	if( m_fNowMp > 300.f )
		m_fNowMp = 300.f;
}

void CX2PET::DamageReact( float fMp )
{
	++m_DefenceComboNum;
	m_fTimeDefence = 0.f;

	m_fNowMp += fMp;
	if( m_fNowMp > 300.f )
		m_fNowMp = 300.f;
}

bool CX2PET::IsActionState(int iState)
{
	if( iState == m_SleepStartState ||		
		iState == m_AfraidState ||
		iState == m_HelloState ||
		iState == m_Happy1State ||
		iState == m_Happy2State ||
		iState == m_Cute1State ||
		iState == m_Cute2State ||
		iState == m_SadState ||
		iState == m_CheerState ||
		iState == m_SpecialState ||
		iState == m_AttackState ||
		iState == m_SitStartState )
		return true;


	return false;
}

float CX2PET::GetDistTarget()
{
	D3DXVECTOR3 vDist = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vTargetPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	bool bIsRight = true;

	if( IsAttacking() == true )
	{
		if( g_pX2Game != NULL )
		{
			CX2GameUnit* pGameUnit = m_pAI->GetGameUnitAttackMeOrBeAttackedByMe();

			if ( NULL != pGameUnit )
				vTargetPos = pGameUnit->GetPos();
		}
	}
	else
	{
		GetMasterInfo(vTargetPos, bIsRight);
	}

	vDist = vTargetPos - GetPos();
	return D3DXVec3Length( &vDist );
}

bool CX2PET::IsAttacking() 
{ 
	if( NULL != m_pAI && NULL != m_pAI->GetGameUnitAttackMeOrBeAttackedByMe() )
	{
		return true;
	}
	return false;
}

bool CX2PET::IsTargetFront()
{
	if( m_pAI != NULL )
	{
		return m_pAI->IsTargetFront();
	}
	return false;
}

void CX2PET::InitOtherTarget()
{
	if( m_pAI != NULL )
	{
		m_pAI->ResetGameUnitAttackMeOrBeAttackedByMe();
		//m_pAI->Targeting(m_fElapsedTime);
		//m_AIMessage	= SCT_NO_CHANGE;
	}
}

void CX2PET::SetEnableAi( bool bVal )
{
	if( m_pAI != NULL )
	{
		m_pAI->SetEnabled( bVal );
	}
}

bool CX2PET::GetEnableAi()
{
	if( m_pAI != NULL )
		return m_pAI->GetEnabled();

	return false;
}

#ifdef EXPAND_SPEED_TIME_SCRIPT_ON_PET
bool CX2PET::IsSpeedTime() 
{ 
	return bSpeedTime;
}
#endif //EXPAND_SPEED_TIME_SCRIPT_ON_PET

void CX2PET::DoCheer()
{
	if( null == m_optrMasterGameUnit || NULL == g_pData || NULL == g_pData->GetPetManager() )
		return;

#ifdef PET_AURA_BUFF

	float fTime = (float)g_pData->GetPetManager()->GetDispositionVal( 0, m_petData.m_Emotion, m_petData.m_Extroversion );

	int iPetStatus = g_pData->GetPetManager()->GetPetStatus( m_petData );	/// 팻 성장도

	/// 팻 성장도에 따른 버프 출력
	switch( iPetStatus )
	{
	case 1:
		m_optrMasterGameUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_PET_AURA_SKILL_LEVEL_1_DEFAULT ) + GetBuffLevelByIntimacy() );/// 친밀도에 따른 버프 적용 - 유체
		break;
	case 2:
		m_optrMasterGameUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_PET_AURA_SKILL_LEVEL_2_DEFAULT ) + GetBuffLevelByIntimacy() );/// 친밀도에 따른 버프 적용 - 성체
		break;
	case 3:
		m_optrMasterGameUnit->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( BFI_BUFF_PET_AURA_SKILL_LEVEL_3_DEFAULT ) + GetBuffLevelByIntimacy() );/// 친밀도에 따른 버프 적용 - 완전체
		break;
	}

	m_TimerAuraSuspended.SetTargetTime( fTime );		/// 격려 유지 시간 설정
	m_TimerAuraSuspended.ResetSumOfElapsedTime();

#else  PET_AURA_BUFF

	float fSpeedBuff = 1.f;
	float fChageMp = 0.f;
	float fCriticalRate = 0.f;
	float fTime = (float)g_pData->GetPetManager()->GetDispositionVal( 0, m_petData.m_Emotion, m_petData.m_Extroversion );
	float fIncreaseHp = 1.f;
	float fChargeHp = 0.f;

	int iPetStatus = g_pData->GetPetManager()->GetPetStatus( m_petData );
	//switch( m_petData.m_Evolution_Step )
	switch( iPetStatus )
	{
	case 1:
		fSpeedBuff = 1.05f;
		fChageMp = 6.f;
		fCriticalRate = 0.06f;
		fIncreaseHp = 1.2f;
		fChargeHp = 0.005f;
		break;
	case 2:
		fSpeedBuff = 1.1f;
		fChageMp = 12.f;
		fCriticalRate = 0.12f;
		fIncreaseHp = 1.3f;
		fChargeHp = 0.01f;
		break;
	case 3:
		fSpeedBuff = 1.15f;
		fChageMp = 18.f;
		fCriticalRate = 0.18f;
		fIncreaseHp = 1.5f;
		fChargeHp = 0.02f;
		break;
	default:
		break;
	}
	
	if( GetNowIntimacy() >= 0.7f )
	{
		float fUpRate = GetNowIntimacy() - 0.6f + 1.f;
	
		fIncreaseHp = 1.f + ( (fIncreaseHp - 1.f) * fUpRate );
		fSpeedBuff = 1.f + ( (fSpeedBuff - 1.f) * fUpRate );
		fCriticalRate = fCriticalRate * fUpRate;
		fChageMp = fChageMp * fUpRate;
		fChargeHp = fChargeHp * fUpRate;
	}

	m_optrMasterGameUnit->SetAnimSpeedFactor( fSpeedBuff, fTime, CX2GameUnit::SFI_SPEED_3, CX2GameUnit::SFST_UPDATE );
	m_optrMasterGameUnit->SetVecMoveSpeedFactor( fSpeedBuff, fTime, CX2GameUnit::SFI_SPEED_3, CX2GameUnit::SFST_UPDATE );

	m_optrMasterGameUnit->SetMaxHp( m_optrMasterGameUnit->GetMaxHp() * fIncreaseHp );
	if( m_optrMasterGameUnit->GetNowHp() > 0.f )
		m_optrMasterGameUnit->SetNowHp( m_optrMasterGameUnit->GetNowHp() * fIncreaseHp );

	//m_pMasterGame->ResetMPChangeRate( m_pMasterGame->GetGageManager()->GetMPGage()->fChangeRate + fChageMp );
	m_optrMasterGameUnit->AddMPFactor( fChageMp, fTime );

	CX2GUUser* pMasterGUUser = static_cast<CX2GUUser*>( m_optrMasterGameUnit.GetObservable() );
	pMasterGUUser->AccessPetCheer().m_fTime = fTime;
	pMasterGUUser->AccessPetCheer().m_fChangeHp = fIncreaseHp;
	pMasterGUUser->AccessPetCheer().m_fSpeed = fSpeedBuff;
	pMasterGUUser->AccessPetCheer().m_fMoveSpeed = fSpeedBuff;
	pMasterGUUser->AccessPetCheer().m_fCriticalRate = fCriticalRate;
	pMasterGUUser->AccessPetCheer().m_fChargeMp = fChageMp;
	pMasterGUUser->AccessPetCheer().m_fChargeHp = fChargeHp;

#endif PET_AURA_BUFF

	PlayCommonEffect( PCE_CHEER );

	if( g_pChatBox != NULL && m_bMyPet == true )
	{
		wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_6549, "L", GetPetInfo().m_wstrPetName ) );
		g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
	}
}



void CX2PET::RenderName()
{
	KTDXPROFILE();

	if( m_pXSkinAnim == NULL || GetLoaded() == false )
		return;

	if( GetShowObject() == false )
		return;

#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif

	D3DXVECTOR3 vPos = GetPos();
	vPos.y -= 44.0f;
#ifdef CLIENT_COUNTRY_EU //유럽은 '특별한' 문장이 펫 이름 뒤에 나오도록 수정 임홍락
	wstring wstrPetName = L"";
	wstrPetName += m_petData.m_wstrPetName;
	wstrPetName += L" ";

	if( m_bMyPet == true )
	{
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( m_petData.m_PetId, m_petData.m_Evolution_Step, m_petData.m_Intimacy );
		if( fIntimacy >= 0.7f )
		{
			wstrPetName += GET_STRING( STR_ID_5612 );
		}
	}	
#else //CLIENT_COUNTRY_EU
	wstring wstrPetName = L"";

	if( m_bMyPet == true )
	{
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( m_petData.m_PetId, m_petData.m_Evolution_Step, m_petData.m_Intimacy );
		if( fIntimacy >= 0.7f )
		{
			wstrPetName = GET_STRING( STR_ID_5612 );
			wstrPetName += L" ";
		}
	}	
	wstrPetName += m_petData.m_wstrPetName;
#endif //CLIENT_COUNTRY_EU

	if( g_pTFieldGame != NULL )
	{
		if( m_bMyPet == true )
		{			
			g_pTFieldGame->GetFontForUnitName()->OutProjectionText( vPos, wstrPetName.c_str(), 0xffffaa77, 
				CKTDGFontManager::FS_SHELL, 0xffff0000, NULL, DT_CENTER  );
		}
		else
		{			
			g_pTFieldGame->GetFontForUnitName()->OutProjectionText( vPos , wstrPetName.c_str(), 0xff54ddff, 
				CKTDGFontManager::FS_SHELL, 0xff0000ff, NULL, DT_CENTER  );
		}
	}
	else
	{
		if( NULL != g_pData->GetPVPRoom() &&
			g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
		{
			if ( m_bMyPet == true )
			{
				g_pX2Game->GetFontForUnitName()->OutProjectionText( vPos, wstrPetName.c_str(), 0xffffaa77, 
					CKTDGFontManager::FS_SHELL, 0xffff0000, NULL, DT_CENTER  );
			}
			else
			{
				g_pX2Game->GetFontForUnitName()->OutProjectionText( vPos, wstrPetName.c_str(), 0xff54ddff, 
					CKTDGFontManager::FS_SHELL, 0xff0000ff, NULL, DT_CENTER  );
			}
		}
		else
		{
			CX2GUUser *pMasterUser = g_pX2Game->GetUserUnitByUID( m_MasterUid );
			if( pMasterUser != NULL &&  pMasterUser->GetTeam() == CX2Room::TN_BLUE )
			{
				g_pX2Game->GetFontForUnitName()->OutProjectionText( vPos, wstrPetName.c_str(), 0xff54ddff, 
					CKTDGFontManager::FS_SHELL, 0xff0000ff, NULL, DT_CENTER  );
			}
			else
			{
				g_pX2Game->GetFontForUnitName()->OutProjectionText( vPos, wstrPetName.c_str(), 0xffffaa77, 
					CKTDGFontManager::FS_SHELL, 0xffff0000 , NULL, DT_CENTER  );
			}
		}
	}

#if 0
#ifdef _IN_HOUSE_
	const int iStep = 1600;
	int iX = GetPetInfo().m_Emotion + 4800;	// 감성지수
	int iY = GetPetInfo().m_Extroversion + 4800;	// 외향지수

	if( iX < 0 )
		iX = 0;
	if( iX > 9600 )
		iX = 9600;
	if( iY < 0 )
		iY = 0;
	if( iY > 9600 )
		iY = 9600;

	iX /= iStep;
	iY /= iStep;

	if( iX > 5 )
		iX = 5;
	if( iY > 5 )
		iY = 5;

	WCHAR wPetInfo[124];
	StringCchPrintf(wPetInfo, 124, L"mp: %d, 진화: %d, 포만도: %d, 친밀도: %d, 감성: %d(%d), 외향성: %d(%d)", 
		(int)m_fNowMp,
		(int)GetPetInfo().m_Evolution_Step, (int)GetPetInfo().m_Satiety, (int)GetPetInfo().m_Intimacy,
		GetPetInfo().m_Emotion, iX, GetPetInfo().m_Extroversion, iY );

	/*wsprintf(wPetInfo, L"mp: %d, 진화: %d, 포만도: %d, 친밀도: %d, 감성: %d(%d), 외향성: %d(%d)", 
		(int)m_fNowMp,
		(int)GetPetInfo().m_Evolution_Step, (int)GetPetInfo().m_Satiety, (int)GetPetInfo().m_Intimacy,
		GetPetInfo().m_Emotion, iX, GetPetInfo().m_Extroversion, iY );
	*/	
	vPos.y -= 23.0f;

	if( g_pTFieldGame != NULL )
	{
		g_pTFieldGame->GetFontForUnitName()->OutProjectionText( vPos, wPetInfo,	0xff96ff00, 
			CKTDGFontManager::FS_SHELL, 0xff062507, NULL, DT_CENTER  );
	}
	else
	{
		g_pX2Game->GetFontForUnitName()->OutProjectionText( vPos, wPetInfo,	0xff96ff00, 
			CKTDGFontManager::FS_SHELL, 0xff062507, NULL, DT_CENTER  );
	}	
#endif
#endif
}

float CX2PET::GetNowIntimacy()
{
	CX2PetManager::PetTemplet *petTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)m_petData.m_PetId );
	float fIntimacy = m_petData.m_Intimacy / petTemplet->m_Evolution_Step_Max[m_petData.m_Evolution_Step];	// 친밀도
	return fIntimacy;
}

float CX2PET::GetNowSatiety()
{
	CX2PetManager::PetTemplet *petTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)m_petData.m_PetId );	
	float fSatiety = m_petData.m_Satiety / 4800.f;
	return fSatiety;
}

void CX2PET::SetShowEffect(bool bVal)
{
	if( m_hSeqIcon != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqIcon );
		if( pSeq != NULL )
		{
			pSeq->SetShowObject( bVal );
		}
	}

#ifdef PET_ATTACH_PARTICLE
	BOOST_TEST_FOREACH( AttachedPetParticleData, kSeq, m_vecSeqPetParticle )
	{
		if( kSeq.hSeq != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( kSeq.hSeq );
			if( pSeq != NULL )
			{
				pSeq->SetShowObject( bVal );
			}
		}
	}
#endif //PET_ATTACH_PARTICLE
}

void CX2PET::SetShowGageBar( bool bVal )
{
	if( m_pDlgPetGage != NULL )
		m_pDlgPetGage->SetShow( bVal );
}

CKTDGParticleSystem* CX2PET::GetUIMajorParticle()
{
	if( g_pData != NULL )
	{
		return g_pData->GetUIMajorParticle();
	}

	return NULL;
}

CX2EffectSet* CX2PET::GetUIEffectSet()
{
	if( g_pData != NULL )
	{
		return g_pData->GetUIEffectSet();
	}

	return NULL;
}

CKTDXDeviceSound* CX2PET::PlaySound( const WCHAR* pName )
{
	if( NULL == pName )
		return NULL; 


	CKTDXDeviceSound* pSound = g_pKTDXApp->GetDeviceManager()->OpenSound( pName );
	if( NULL != pSound )
	{
		pSound->Set3DPosition( GetPos() );
		pSound->Play();
		
#ifdef CLOSE_SOUND_TEST
		m_SoundCloseManager.AddSound( pName );
#endif CLOSE_SOUND_TEST
	}

	return pSound;
}

void CX2PET::DrawPetIcon( int iStateId )
{
	if( m_pXSkinAnim == NULL )
		return;

	bool bFlag = true;
	if( GetNowState() == m_EatState || GetNowState() == m_StuffedState )
	{
		bFlag = true;
	}
	else
	{
		bFlag = false;
	}

	bool bFlag1 = true;
	if( iStateId != m_EatState && iStateId != m_StuffedState && g_pData->GetPetManager()->GetPetStatus( m_petData ) <= 0 )
	{
		bFlag1 = false;
	}
	else
	{
		bFlag1 = true;
	}

	if( bFlag == false && bFlag1 == false )
		return;

	bool bCreate = true;
	bool bDelete = true;
	wstring wstrParticleName = L"";

	if( m_fIconTimer > 0.f )
		bDelete = false;

	if( iStateId == m_NotKnowState )
	{
		wstrParticleName = L"Pet_DoNowKnow";
		m_fIconTimer = 0.f;
	}
	else if( iStateId == m_EatState )
	{
		wstrParticleName = L"Pet_Eat";
		m_fIconTimer = 0.f;
	}
	else if( iStateId == m_HungryState )
	{
		wstrParticleName = L"Pet_Hungry";
		m_fIconTimer = 0.f;
	}
	else if( iStateId == m_StuffedState )
	{
		wstrParticleName = L"Pet_Stuffed";
		m_fIconTimer = 0.f;
	}
	else if( iStateId == m_SleepLoopState )
	{
		wstrParticleName = L"Pet_Sleep";
		m_fIconTimer = 0.f;
	}
	else if( iStateId == m_AfraidState || iStateId == m_SadState )
	{
		wstrParticleName = L"Pet_Afraid";
		m_fIconTimer = 0.f;
	}
	else if( iStateId == m_Happy1State || iStateId == m_Happy2State )
	{
		wstrParticleName = L"Pet_Happy";
		m_fIconTimer = 5.f;
	}
	else if( iStateId == m_CheerState )
	{
		wstrParticleName = L"Pet_Cheer";
		m_fIconTimer = 0.f;
	}
	else
	{
		bCreate = false;	
	}
	
	if( bDelete == true )
	{
		if( m_hSeqIcon != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqIcon );
			m_hSeqIcon = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	if( bCreate == true )
	{
		if( m_hSeqIcon != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqIcon );
			m_hSeqIcon = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqIcon )
		{
			D3DXVECTOR3 vPos = GetBonePos(L"Bip01_Head");
			if( wstrParticleName != L"Pet_Sleep" )
			{
				vPos.x -= 50.f;
				vPos.y += 50.f;
			}
			
			m_hSeqIcon = g_pData->GetUIMajorParticle()->CreateSequenceHandle( (CKTDGObject*) this,  wstrParticleName.c_str(), vPos, 3.f, 3.f );			
		}
	}		
}

CX2GameUnit* CX2PET::GetTargetUnit()
{	
	if( IsAttacking() == true && g_pX2Game != NULL )
	{
		CX2GameUnit* pTargetGameUnit = m_pAI->GetGameUnitAttackMeOrBeAttackedByMe();
		if  ( NULL != pTargetGameUnit )
			return pTargetGameUnit;
		else
			return GetMaster();
	}
	else
		return GetMaster();
}

#ifdef ADD_PET_NINE_TAIL_FOX
void CX2PET::SetLockOnDamageEffect( CX2DamageEffect::CEffect* pEffect )		/// 현재 팻이 목표로 정한 유닛을 인자로 들어온 데미지 이펙트에 록온 시킨다.
{
	/// 해당 펫 소환자와 인자로 받아온 데미지 이펙트가 존재할 때 동작
	if( NULL != pEffect )
	{		
		CX2GameUnit* pMasterGameUnit = GetMaster();
		if ( NULL != pMasterGameUnit && 
			 NULL != pMasterGameUnit->GetGameUnitAttackedByMe() && 
			 CX2GameUnit::GUT_NPC == pMasterGameUnit->GetGameUnitAttackedByMe()->GetGameUnitType() )
		{
			CX2GUNPC* pNpc = static_cast<CX2GUNPC*>( pMasterGameUnit->GetGameUnitAttackedByMe() );
			/// 타겟이 유효할 때 동작
			if( NULL != pNpc && 
				CX2UnitManager::NCT_BASIC == pNpc->GetNPCTemplet().m_ClassType && 0.f < pNpc->GetNowHp() )
			{
				pEffect->SetLockOnNPCUID( pNpc->GetUID() );		/// 해당 데미지 이펙트에 록온 데이터 설정
			}
		}
	}
}
#endif ADD_PET_NINE_TAIL_FOX

#ifdef ADD_HALLOWEEN_PET_SYSTEM
void CX2PET::ShowIncreaseValue( D3DXVECTOR3 vecPos, bool IsHP, float fValue )		/// HP or MP 회복 수치 표시
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

#ifdef PET_AURA_BUFF

UINT CX2PET::GetBuffLevelByIntimacy()
{
	const float fNowIntimacy = GetNowIntimacy();	/// 팻 친밀도

	/// 팻 친밀도에 따른 Enum 인덱스 추가 값
	if( fNowIntimacy >= 1.0f )
		return 4;
	else if( fNowIntimacy >= 0.9f )
		return 3;
	else if( fNowIntimacy >= 0.8f )
		return 2;
	else if( fNowIntimacy >= 0.7f )
		return 1;
	else
		return 0;
}

#endif PET_AURA_BUFF

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
void CX2PET::UpdateRandomTableIndex()
{
	if( m_petSyncData.nowState == m_PreState )
		return;

	if( g_pX2Game->IsHost() == false )
		return;

	m_petSyncData.m_usRandomTableIndex = (USHORT) ( rand() % CKTDXRandomNumbers::RANDOM_NUMBER_COUNT );
	m_PreState = m_petSyncData.nowState;
}

void CX2PET::_EncodePetSyncPos( USHORT& usPosX, USHORT& usPosY, USHORT& usPosZ, const D3DXVECTOR3& position, unsigned char lastTouchLineIndex )
{
    const CKTDGLineMap::LineData* pLineData = NULL;
	if( !g_pX2Game || !g_pX2Game->GetLineMap() 
        || ( pLineData = g_pX2Game->GetLineMap()->GetLineData( ( int )lastTouchLineIndex ) ) == NULL )
    {
		StateLog( L"lastTouchLineIndex가 이상한듯" );
    	usPosX = floatToHalf( position.x );
	    usPosY = floatToHalf( position.y );
	    usPosZ = floatToHalf( position.z );
		return;
    }

	usPosX = floatToHalf( position.x - pLineData->startPos.x );
	usPosY = floatToHalf( position.y - pLineData->startPos.y );
	usPosZ = floatToHalf( position.z - pLineData->startPos.z );

}

void CX2PET::PetSyncData::_DecodePetSyncPos( D3DXVECTOR3& vPosition, USHORT usPosX, USHORT usPosY, USHORT usPosZ, unsigned char ucLastTouchLineIndex )
{
    const CKTDGLineMap::LineData* pLineData = NULL;
	if( !g_pX2Game || !g_pX2Game->GetLineMap() 
        || ( pLineData = g_pX2Game->GetLineMap()->GetLineData( ( int )ucLastTouchLineIndex ) ) == NULL )
    {
		StateLog( L"라인 포인터 이상." );
	    vPosition.x = halfToFloat( usPosX );
	    vPosition.y = halfToFloat( usPosY );
	    vPosition.z = halfToFloat( usPosZ );
		return;
    }

	vPosition.x = pLineData->startPos.x + halfToFloat( usPosX );
	vPosition.y = pLineData->startPos.y + halfToFloat( usPosY );
	vPosition.z = pLineData->startPos.z + halfToFloat( usPosZ );
}

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


#endif //SERV_PET_SYSTEM




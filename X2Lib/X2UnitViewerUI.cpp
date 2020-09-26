#include "StdAfx.h"
#include ".\x2unitviewerui.h"

bool CX2UnitViewerUI::m_sbShow = true;
bool CX2UnitViewerUI::m_sbUnitClicked = false;

//{{ robobeg : 2008-10-18

static const CX2Item::ItemTemplet* _GetSafeEquipItemTemplet( CX2EqipPtr pX2Eqip )
{
    if ( pX2Eqip != NULL )
    {
#ifdef GET_ITEM_NULL_CHECK
		CX2Item* pX2Item = pX2Eqip->GetItem();
		if ( NULL != pX2Item )
		{
			if ( const CX2Item::ItemTemplet* pItemTemplet = pX2Item->GetItemTemplet() )
			{
				if ( pItemTemplet->GetEqipPosition() >= CX2Unit::EQIP_POSITION( 0 ) && pItemTemplet->GetEqipPosition() < CX2Unit::EP_END )
					return pItemTemplet;
			}//if
		}//if
#else
		if ( CX2Item* pX2Item = pX2Eqip->GetItem() )
		{
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
			if ( NULL != pX2Item ) 
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
			{
				if ( const CX2Item::ItemTemplet* pItemTemplet = pX2Item->GetItemTemplet() )
				{
					if ( pItemTemplet->GetEqipPosition() >= CX2Unit::EQIP_POSITION( 0 ) && pItemTemplet->GetEqipPosition() < CX2Unit::EP_END )
						return pItemTemplet;
				}//if
			}//if
		}
#endif //GET_ITEM_NULL_CHECK     
    }//if
    ASSERT( !"internal error" );
    return NULL;
}//



CX2UnitViewerUI::CX2UnitViewerUI(void) 
: m_bEnableEquipSlotVisibilityUpdate( true )
#ifdef TAKE_OFF_ALL_ITEM
, m_bShowInvenEquip ( true )
#endif // TAKE_OFF_ALL_ITEM
, m_eUnitViewerState( UVS_NONE )

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
, m_bIsPlayAvatarEmotionSoundWithoutEmotion ( false )
#endif // CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
{
	SetAlphaObject( false );

	m_pUnit					= NULL;
	m_pMotion				= NULL;
	//m_pXSkinAnim			= NULL;
	INIT_VECTOR3( m_LightPos, 1000,1000,1000 );

//{{ robobeg : 2008-10-17
    ZeroMemory( m_ViewEquipNormal, sizeof(m_ViewEquipNormal) );
    ZeroMemory( m_ViewEquipFashion, sizeof(m_ViewEquipFashion) );
	

	InitEquipVisibility();

//}} robobeg : 2008-10-17

	m_BillBoardType			= CKTDGMatrix::BT_NONE;
	m_hSeqHandFire			= INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifdef NEW_CHARACTER_EL
	m_hSeqHandFireSPK1			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqHandFireSPK2			= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	m_hSeqEnergyBullet1			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnergyBullet2			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqEnergyBullet3			= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

	m_hSeqWeapon_Fire		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqWeapon_Fire2		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqWeapon_Fire3		= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hSeqWeapon_Wind		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqWeapon_Wind2		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqWeapon_Wind3		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqWeapon_Wind4		= INVALID_PARTICLE_SEQUENCE_HANDLE;



	for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
	{
		m_pFrame_TRACE_START[i] = NULL;
		m_pFrame_TRACE_END[i]	= NULL;
#ifdef ARA_CHARACTER_BASE
		m_pFrame_WEAPON_BONE[i] = NULL;
#endif
	}

	
	m_ExtraDamageType		= 0;

//#ifdef TITLE_SYSTEM
    m_hSeqEmblem		= INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hPart_Emblem_200 = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_pPart_Emblem_200 = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_iTitleId              = 0;
//#endif
#ifdef NEW_VILLAGE_UI
	m_bFixed = false;
#endif

	m_bPickUnit = false;

	m_pSound_Eve_LevelUp = NULL;

#ifdef UNIT_EMOTION
	m_eOldMotionType = UVUMT_WAIT;
	m_ePlayedEmotion = CX2Unit::ET_NONE;
#endif

	for(int i=0; i < AVATAR_EMOTION_NUM; ++i)
		m_bMixedEmotion[i] = false;
	m_pAvatarEmotionSound = NULL;

	m_fScale = 1.f;

#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_bShowGun			= false;
	m_hChungMiniGunL	= INVALID_MESH_INSTANCE_HANDLE;
	m_hChungMiniGunR	= INVALID_MESH_INSTANCE_HANDLE;
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
	m_bShowSword			= false;
	m_hElswordSword			= INVALID_MESH_INSTANCE_HANDLE;
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_TRAPPING_RANGER_TEST
	m_bShowLTrapSword			= false;
	m_hInstLTrapSwordReady		= INVALID_MESH_INSTANCE_HANDLE;
#endif SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
	m_bShowRNWTrapSword			= false;
	m_hInstRNWTrapSwordReady	= INVALID_MESH_INSTANCE_HANDLE;
	m_bChangeClass				= false;
#endif SERV_RENA_NIGHT_WATCHER

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 강화 무기 이펙트 객체
	m_hSeqUpgradeWeapon			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqUpgradeWeapon2		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqUpgradeWeapon3		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqUpgradeWeapon4		= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_pFrame_ATTACK_LINE_START0_FRONT	= NULL;
	m_pFrame_ATTACK_LINE_END0			= NULL;
#endif ADD_UPGRADE_WEAPON_PARTICLE

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	m_hSeqHandFireSBH1		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqHandFireSBH2		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqHandFireSBH3		= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	m_hSeqHandElectricALP1	= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqHandElectricALP2	= INVALID_PARTICLE_SEQUENCE_HANDLE;
#endif //SERV_ADD_LUNATIC_PSYKER
}

CX2UnitViewerUI::~CX2UnitViewerUI(void)
{
//{{ robobeg : 2008-10-17
	//for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
	//{
	//	CX2Eqip* pX2Eqip = m_ViewEqipList[i];
	//	SAFE_DELETE( pX2Eqip );
	//}
	//m_ViewEqipList.clear();
    BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value_type, m_ViewEquipMap )
    {
        CX2EqipPtr pX2Eqip = value_type.second;
		if( pX2Eqip != NULL )
		{
			pX2Eqip->SetManagerObject( NULL );
		}
    }//BOOST_TEST_FOREACH()
    m_ViewEquipMap.clear();
//}} robobeg : 2008-10-17


	m_pXSkinAnim.reset();
	SAFE_CLOSE( m_pMotion );

	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFire );

#ifdef NEW_CHARACTER_EL
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSPK1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSPK2 );
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEnergyBullet1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEnergyBullet2 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEnergyBullet3 );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSBH1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSBH2 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSBH3 );
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandElectricALP1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandElectricALP2 );
#endif //SERV_ADD_LUNATIC_PSYKER

	DeleteEnchantWeaponEffect();

//#ifdef TITLE_SYSTEM
    if(m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE)
        g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEmblem );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hPart_Emblem_200 = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_pPart_Emblem_200 = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
//#endif

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if( m_hChungMiniGunL != INVALID_MESH_INSTANCE_HANDLE )
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hChungMiniGunL );
	if( m_hChungMiniGunR != INVALID_MESH_INSTANCE_HANDLE )
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hChungMiniGunR );
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
	if( m_hElswordSword != INVALID_MESH_INSTANCE_HANDLE )
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hElswordSword );
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_TRAPPING_RANGER_TEST
	if( m_hInstLTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hInstLTrapSwordReady );
#endif SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
	if( m_hInstRNWTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
		g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hInstRNWTrapSwordReady );
#endif SERV_RENA_NIGHT_WATCHER

#ifdef CRASH_LOG
	if( m_pSound_Eve_LevelUp != NULL )
	{
		std::wstringstream strstm;
		wstring wstrSoundEveLevelDeviceID = m_pSound_Eve_LevelUp->GetDeviceID();
		strstm << "wstrSoundEveLevelDeviceID = " << wstrSoundEveLevelDeviceID;
		ErrorLogMsg( 0, strstm.str().c_str() );
	}
#endif CRASH_LOG

	SAFE_CLOSE( m_pSound_Eve_LevelUp );

	if( m_pAvatarEmotionSound != NULL )
	{
		m_pAvatarEmotionSound->Stop();
		SAFE_CLOSE( m_pAvatarEmotionSound );
	}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE
	DeleteUpgradeWeaponParticle();
#endif ADD_UPGRADE_WEAPON_PARTICLE
}


HRESULT CX2UnitViewerUI::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if ( m_pUnit == NULL )
		return false;

#ifdef UNIT_EMOTION
	EmotionFrameMove();
#endif

	if( m_pAvatarEmotionSound != NULL && 
		( m_pXSkinAnim->GetNowAnimName() != L"Emotion_BIGBANG" && 
		m_pXSkinAnim->GetNowAnimName() != L"Emotion_APINK" && 
		m_pXSkinAnim->GetNowAnimName() != L"Emotion_APINK_LOVE" &&
		m_pXSkinAnim->GetNowAnimName() != L"Emotion_CRAYONPOP" 
#ifdef CRAYONPOP_SECOND_EMOTION
		&& m_pXSkinAnim->GetNowAnimName() != L"Emotion_BbaBbaBba"
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		&& false == m_bIsPlayAvatarEmotionSoundWithoutEmotion 
#endif // CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		))
	{
		m_pAvatarEmotionSound->Stop();
		SAFE_CLOSE( m_pAvatarEmotionSound );
	}

	m_pXSkinAnim->SetBillBoardType( m_BillBoardType );


//2008.11.21 김태완 : 화면에 고정해서 그리기 (m_bFixed)
#ifdef NEW_VILLAGE_UI
	if(false == m_bFixed) 
	{		
		m_pXSkinAnim->GetMatrix().Move( GetMatrix().GetPos() );		
	}
#else
	
	m_pXSkinAnim->GetMatrix().Move( GetMatrix().GetPos() );
#endif
	m_pXSkinAnim->GetMatrix().Rotate( GetMatrix().GetRotate() );
	m_pXSkinAnim->GetMatrix().Scale( GetMatrix().GetScale() );

	m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );


//{{ robobeg : 2008-10-17
	//for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
    BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
//}} robobeg : 2008-10-17
	{
//{{ robobeg : 2008-10-17
		//CX2Eqip* pX2Eqip = m_ViewEqipList[i];
        CX2EqipPtr pX2Eqip = value.second;
        const CX2Item::ItemTemplet* pItemTemplet = _GetSafeEquipItemTemplet( pX2Eqip );
        if ( pItemTemplet == NULL )
            continue;
        ASSERT( pX2Eqip->GetItemUID() == value.first );
        CX2Unit::EQIP_POSITION ePos = pItemTemplet->GetEqipPosition();

        if ( m_ViewEquipFashion[ ePos ] == value.first )
        {
            ASSERT( pItemTemplet->GetFashion() );

			CX2Item* pTempItem = m_pUnit->GetInventory().GetEquippingItemByEquipPos( ePos, false );
			
			//{{ kimhc // 실시간 엘소드 중 실시간 내구도 감소
#ifdef	REAL_TIME_ELSWORD
			if( pTempItem != NULL && 
				pX2Eqip->GetEnchantLevel() != pTempItem->GetItemData().m_EnchantLevel && 
				//pTempItem->GetItemData().m_Endurance > 0
				( ( pTempItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && pTempItem->GetItemData().m_Endurance > 0 ) ||
					pTempItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY )	 )
			{
				pX2Eqip->ChangeEnchantLevel( pTempItem->GetItemData().m_EnchantLevel );
			}

			if( pX2Eqip->GetEnchantLevel() != 0 )
			{
				if ( pTempItem == NULL || 
					( pTempItem != NULL && 
						pTempItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && pTempItem->GetItemData().m_Endurance <= 0 ) )
					pX2Eqip->ChangeEnchantLevel( 0 );
			}
#else	REAL_TIME_ELSWORD
			if( pTempItem != NULL && pX2Eqip->GetEnchantLevel() != pTempItem->GetItemData().m_EnchantLevel )
			{
				pX2Eqip->ChangeEnchantLevel( pTempItem->GetItemData().m_EnchantLevel );
			}

			if( pX2Eqip->GetEnchantLevel() != 0 && pTempItem == NULL )
			{
				pX2Eqip->ChangeEnchantLevel( 0 );
			}
#endif	REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 실시간 내구도 감소
			
        }
        else if ( m_ViewEquipNormal[ ePos ] == value.first )
        {
            ASSERT( !pItemTemplet->GetFashion() );
		    //// 패션이 아니고, 패션이 이미 장착해있을 경우 제외
      //      if ( m_ViewEquipFashion[ ePos ] != 0 )
      //          continue;
        }
        else
        {
            ASSERT( !"internal error" );
            continue;
        }//if.. else..

//{{ robobeg : 2008-10-17
    	//pX2Eqip->OnFrameMove( fTime, fElapsedTime, m_pXSkinAnim->GetRenderParam() );
		pX2Eqip->SetRenderParam( fTime, fElapsedTime, m_pXSkinAnim->GetRenderParam() );
		pX2Eqip->OnFrameMove( fTime, fElapsedTime );
//}} robobeg : 2008-10-17
	}

	CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();

	pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;

	if( g_pMain->GetGameOption().GetOptionList().m_UnitDetail == CX2GameOption::OL_MEDIUM )
	{
		pRenderParam->renderType = CKTDGXRenderer::RT_CARTOON;
	}
	else if( g_pMain->GetGameOption().GetOptionList().m_UnitDetail == CX2GameOption::OL_LOW )
	{
		pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
	}
//{{ robobeg : 2008-10-24
	//pRenderParam->worldMatrix		= GetMatrix().GetMatrix( m_BillBoardType );
//}} robobeg : 2008-10-24
		
	if( g_pMain->GetGameOption().GetOptionList().m_UnitDetail == CX2GameOption::OL_HIGH )
	{
		if(m_bPickUnit == true)
		{
			pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
			//pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_RED;
			pRenderParam->outLineColor		= 0xffff5511;
		}
		else
		{
			pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
			//pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
			pRenderParam->outLineColor		= 0xffffffff;
		}	

		pRenderParam->lightPos			= m_LightPos;
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		pRenderParam->fOutLineWide		= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		pRenderParam->fOutLineWide		= 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
		pRenderParam->bAlphaBlend		= false;
	}
	else
	{
		pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
		pRenderParam->lightPos			= m_LightPos;
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		pRenderParam->fOutLineWide		= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		pRenderParam->fOutLineWide		= 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
		pRenderParam->color				= 0xffffffff;
		pRenderParam->bAlphaBlend		= false;
	}	
	
	if( m_hSeqHandFire != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			vBonePos.y += 15.f;

			pSeq->SetPosition( vBonePos );
			pSeq->SetShowObject( GetShowObject() );
		}
		else
		{
			m_hSeqHandFire = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

#ifdef NEW_CHARACTER_EL
	if( m_hSeqHandFireSPK1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK1 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			vBonePos.y += 20.f;
			pSeq->SetPosition( vBonePos );
			pSeq->SetShowObject( GetShowObject() );
		}
		else
		{
			m_hSeqHandFireSPK1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}
	if( m_hSeqHandFireSPK2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK2 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			vBonePos.y += 20.f;
			pSeq->SetPosition( vBonePos );
			pSeq->SetShowObject( GetShowObject() );
		}
		else
		{
			m_hSeqHandFireSPK2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	if( m_hSeqEnergyBullet1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet1 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			vBonePos.y += 20.f;
			pSeq->SetPosition( vBonePos );
			pSeq->SetShowObject( GetShowObject() );
		}
		else
			m_hSeqEnergyBullet1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
	if( m_hSeqEnergyBullet2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet2 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			vBonePos.y += 20.f;
			pSeq->SetPosition( vBonePos );
			pSeq->SetShowObject( GetShowObject() );
		}
		else
			m_hSeqEnergyBullet2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
	if( m_hSeqEnergyBullet3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet3 );
		if( NULL != pSeq )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			vBonePos.y += 20.f;
			pSeq->SetPosition( vBonePos );
			pSeq->SetShowObject( GetShowObject() );
		}
		else
			m_hSeqEnergyBullet3 = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	if ( m_pUnit->GetClass() == CX2Unit::UC_ELESIS_BLAZING_HEART )
	{
		if ( m_pXSkinAnim->GetNowAnimName() == L"SBH_LobbyWait" || m_pXSkinAnim->GetNowAnimName() == L"SBH_LobbyReadyAction" )
		{
			float fShowParticleSeqStartTime = 0.f;
			float fShowParticleSeqEndTime = 0.f;
			float fParticleSeqOffsetY = 0.f;
			if ( m_pXSkinAnim->GetNowAnimName() ==  L"SBH_LobbyWait" )
			{
				fShowParticleSeqStartTime = 2.5; 
				fShowParticleSeqEndTime = 2.95f;
				fParticleSeqOffsetY = -10.f;
			}
			else if ( m_pXSkinAnim->GetNowAnimName() ==  L"SBH_LobbyReadyAction" ) 
			{
				fShowParticleSeqStartTime = 1.82f;
				fShowParticleSeqEndTime = 2.19f;
				fParticleSeqOffsetY = 0.f;
			}

			if ( (  fShowParticleSeqStartTime <= m_pXSkinAnim->GetNowAnimationTime () && m_pXSkinAnim->GetNowAnimationTime () <= fShowParticleSeqEndTime ) ) 
			{
				if ( m_hSeqHandFireSBH1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH1 );
					if( NULL != pSeq )
					{
						D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Finger2" );
						vBonePos.y += fParticleSeqOffsetY;
						pSeq->SetPosition( vBonePos );
						pSeq->SetShowObject( GetShowObject() );
						pSeq->SetEmitRate( 300.0f, 300.0f );
					}
					else
					{
						m_hSeqHandFireSBH1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
					}
				}
				if ( m_hSeqHandFireSBH2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH2 );
					if( NULL != pSeq )
					{
						D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Finger2" );
						vBonePos.y += fParticleSeqOffsetY;
						pSeq->SetPosition( vBonePos );
						pSeq->SetShowObject( GetShowObject() );
						pSeq->SetEmitRate( 50.0f, 50.0f );
					}
					else
					{
						m_hSeqHandFireSBH2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
					}
				}
				if ( m_hSeqHandFireSBH3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH3 );
					if( NULL != pSeq )
					{
						D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Finger2" );
						vBonePos.y += fParticleSeqOffsetY;
						pSeq->SetPosition( vBonePos );
						pSeq->SetShowObject( GetShowObject() );
						pSeq->SetEmitRate( 300.0f, 300.0f );
					}
					else
					{
						m_hSeqHandFireSBH3 = INVALID_PARTICLE_SEQUENCE_HANDLE;
					}
				}
			}
			else
			{
				if ( m_hSeqHandFireSBH1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH1 );
					if( NULL != pSeq )
					{
						pSeq->SetEmitRate( 0.0f, 0.0f );
					}
				}
				if ( m_hSeqHandFireSBH2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH2 );
					if( NULL != pSeq )
					{
						pSeq->SetShowObject( false );
						pSeq->SetEmitRate( 0.0f, 0.0f );
					}
				}
				if ( m_hSeqHandFireSBH3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH3 );
					if( NULL != pSeq )
					{
						pSeq->SetEmitRate( 0.0f, 0.0f );
					}
				}
			}
		}
		else
		{

			if ( m_hSeqHandFireSBH1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH1 );
				if( NULL != pSeq )
				{
					pSeq->SetEmitRate( 0.0f, 0.0f );
				}
			}
			if ( m_hSeqHandFireSBH2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH2 );
				if( NULL != pSeq )
				{
					pSeq->SetShowObject( false );
					pSeq->SetEmitRate( 0.0f, 0.0f );
				}
			}
			if ( m_hSeqHandFireSBH3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH3 );
				if( NULL != pSeq )
				{
					pSeq->SetEmitRate( 0.0f, 0.0f );
				}
			}
		}
	}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	if ( m_pUnit->GetClass() == CX2Unit::UC_ADD_LUNATIC_PSYKER &&
		 m_pXSkinAnim->GetNowAnimName() == L"ALP_LobbyWait" &&
		 3.6f <= m_pXSkinAnim->GetNowAnimationTime () && 
		 4.5f > m_pXSkinAnim->GetNowAnimationTime () )
	{
		if( m_hSeqHandElectricALP1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP1 );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_R_Hand" );
				vBonePos.y += 10.f;
				pSeq->SetPosition( vBonePos );
				pSeq->SetShowObject( true );
				pSeq->SetEmitRate( 50.0f, 50.0f );
			}
			else
				m_hSeqHandElectricALP1 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
		if( m_hSeqHandElectricALP2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP2 );
			if( NULL != pSeq )
			{
				D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_R_Hand" );
				vBonePos.y += 10.f;
				pSeq->SetPosition( vBonePos );
				pSeq->SetShowObject( true );
				pSeq->SetEmitRate( 80.0f, 80.0f );
			}
			else
				m_hSeqHandElectricALP2 = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}
	else
	{
		if ( m_hSeqHandElectricALP1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP1 );
			if( NULL != pSeq )
			{
				pSeq->SetShowObject( false );
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		if ( m_hSeqHandElectricALP2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP2 );
			if( NULL != pSeq )
			{
				pSeq->SetShowObject( false );
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
	}
#endif //SERV_ADD_LUNATIC_PSYKER

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if ( m_hChungMiniGunL != INVALID_MESH_INSTANCE_HANDLE && m_hChungMiniGunR != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstL = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunL );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstR = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunR );

		if ( pMeshInstL != NULL && pMeshInstR != NULL )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame2 = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Hand" );

			pMeshInstL->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );
			pMeshInstR->SetUseDXMatrix( pMultiAnimFrame2->combineMatrix );

			pMeshInstL->SetShowObject( m_bShowGun, m_bShowGun );
			pMeshInstR->SetShowObject( m_bShowGun, m_bShowGun );
		}
	}
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
	if ( m_hElswordSword != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hElswordSword );

		if ( pMeshInst != NULL )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );

			if( pMultiAnimFrame != NULL )
				pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );

			pMeshInst->SetShowObject( m_bShowSword );
		}
	}
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_TRAPPING_RANGER_TEST
	if ( m_hInstLTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstLTrapSwordReady );

		if ( pMeshInst != NULL )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );

			if( pMultiAnimFrame != NULL )
				pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );

			pMeshInst->SetShowObject( m_bShowLTrapSword );
		}
	}
#endif SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
	if ( m_hInstRNWTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstRNWTrapSwordReady );

		if ( pMeshInst != NULL )
		{
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pMultiAnimFrame = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );

			if( pMultiAnimFrame != NULL )
				pMeshInst->SetUseDXMatrix( pMultiAnimFrame->combineMatrix );

			pMeshInst->SetShowObject( m_bShowRNWTrapSword );
		}
	}
#endif SERV_RENA_NIGHT_WATCHER


	if ( m_pUnit != NULL && m_pUnit->GetUnitTemplet() != NULL )
	{
		if ( m_ExtraDamageType != GetEnchantExtraDamageType() )
		{
			m_ExtraDamageType = GetEnchantExtraDamageType();
			DeleteEnchantWeaponEffect();

			if ( m_pUnit != NULL && m_pUnit->GetUnitTemplet() != NULL )
			{
				InitEnchantedWeaponEffectAtHand();

				switch( m_pUnit->GetUnitTemplet()->m_UnitType )
				{
				default:
					{
						InitEnchantWeaponEffectForSword();
					} break;

				case CX2Unit::UT_LIRE:
				case CX2Unit::UT_EVE:
#ifdef ARA_CHARACTER_BASE
				case CX2Unit::UT_ARA:
#endif
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
				case CX2Unit::UT_ADD:
#endif //SERV_9TH_NEW_CHARACTER
					{
						InitEnchantWeaponEffectForBow();
					} break;
				}
			}
			//{{ 2009.2.4 김태완 : Fixed 되었을 때의 이펙트 처리
			if(m_bFixed)
			{
				SetShowAttribEnchant( false );
			}	
			//}}
		}



		ProcessEnchantedWeaponEffectAtHand( m_pUnit->GetUnitTemplet()->m_UnitType );

		switch( m_pUnit->GetUnitTemplet()->m_UnitType )
		{
		default:
			{
				ProcessEnchantWeaponEffectForSword();
			} break;

		case CX2Unit::UT_LIRE:
			{
				ProcessEnchantWeaponEffectForBow();
			} break;


		case CX2Unit::UT_EVE:
			{
				ProcessEnchantWeaponEffectForEve();

			} break;
#ifdef ARA_CHARACTER_BASE
		case CX2Unit::UT_ARA:
			{
				ProcessEnchantWeaponEffectForAra();
			} break;
#endif
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	case CX2Unit::UT_ADD:
			{
				ProcessEnchantWeaponEffectForAdd();
			} break;
#endif //SERV_9TH_NEW_CHARACTER
		}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE
		ProcessUpgradeWeaponParticle();
#endif ADD_UPGRADE_WEAPON_PARTICLE
	}

//#ifdef TITLE_SYSTEM
    D3DXVECTOR3 markerPos, vHeadPos;
    GetFramePos( &vHeadPos, GetXSkinAnim()->GetCloneFrame( L"Bip01_Head" ) );
    markerPos = GetMatrix().GetPos(); //D3DXVECTOR3(0.f, 0.f, 0.f); // GetPos();

    CKTDGParticleSystem::CParticleEventSequence* pSeq = ( m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE )
        ? g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem ) : NULL;

	if(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_ROOM )
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( CKTDGParticleSystem::CParticle* pPart_Emblem_200 = ( pSeq != NULL ) ? pSeq->ValidateParticleHandle( m_hPart_Emblem_200 ) : NULL )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( CKTDGParticleSystem::CParticle* pPart_Emblem_200 = m_pPart_Emblem_200 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        {
            pPart_Emblem_200->SetPos( markerPos + D3DXVECTOR3( -75, 310, 0 ) );
            pPart_Emblem_200->SetSize( D3DXVECTOR3(0.6f, 0.6f, 1.0f) );
		}
    }
    else if(g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM && m_bFixed == false)
    {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( CKTDGParticleSystem::CParticle* pPart_Emblem_200 = ( pSeq != NULL ) ? pSeq->ValidateParticleHandle( m_hPart_Emblem_200 ) : NULL )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( CKTDGParticleSystem::CParticle* pPart_Emblem_200 = m_pPart_Emblem_200 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        {
            pPart_Emblem_200->SetPos( markerPos + D3DXVECTOR3( -60, 250, 0 ) );

			float fSacleX = 0.7f;
			float fSacleY = 0.7f;
			float fSacleZ = 1.0f;

            if ( pSeq != NULL )
            {
			    switch(pSeq->GetParticleType()) 
			    {
			    case CKTDGParticleSystem::PT_MESH: 
			    case CKTDGParticleSystem::PT_SKINMESH:
				    break;
			    case CKTDGParticleSystem::PT_3D_PLANE:
					
				    fSacleX = pPart_Emblem_200->GetSize().x;
				    fSacleY = pPart_Emblem_200->GetSize().y;
				    fSacleZ = pPart_Emblem_200->GetSize().z;
				    break;
			    default:
				    break;
			    }
            }
            pPart_Emblem_200->SetSize( D3DXVECTOR3(fSacleX, fSacleY, fSacleZ) );
        }        
    }
    else //if(g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT)
    {
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( false );
		}
    }   
    

    UpdateTitle();
//#endif

//#ifndef _SERVICE_
//	if( m_pXSkinAnim != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV && g_pInstanceData->GetFrameScale() == true)
//	{
//		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
//		if(pFrameHead != NULL && pFrameHead->m_bScale == false)
//			SetFrameScale(&D3DXVECTOR3(1.5f, 1.5f, 1.5f), false, pFrameHead);
//	}
//	else if( m_pXSkinAnim != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV && g_pInstanceData->GetFrameScale() == false)
//	{
//		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
//		if(pFrameHead != NULL && pFrameHead->m_bScale == true)
//			SetFrameScaleRestore(pFrameHead);
//	}
//#endif

// 엘소드 2차 키키우기 실시간 테스트용

#ifndef _SERVICE_
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_R) == TRUE )
	{
		SetUnitReForm();
	}	
#endif

	return S_OK;
}


void CX2UnitViewerUI::SetPickUnit(bool val)
{
	m_bPickUnit = val;	
}


/*virtual*/
void CX2UnitViewerUI::NotifyShowObjectChanged()
{
	//CKTDGObject::SetShowObject( bShow );

    BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
    {
        CX2EqipPtr pX2Eqip = value.second;

        if ( pX2Eqip == NULL )
            continue;

        pX2Eqip->NotifyShowObjectChanged();

    }//BOOST_TEST_FOREACH()


    bool bShow = GetShowObject();



	if( m_hSeqHandFire != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
#ifdef NEW_CHARACTER_EL
	if( m_hSeqHandFireSPK1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK1 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
	if( m_hSeqHandFireSPK2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK2 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	if( m_hSeqEnergyBullet1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet1 );

		if( NULL != pSeq )
			pSeq->SetShowObject( bShow );
	}
	if( m_hSeqEnergyBullet2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet2 );

		if( NULL != pSeq )
			pSeq->SetShowObject( bShow );
	}
	if( m_hSeqEnergyBullet3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet3 );

		if( NULL != pSeq )
			pSeq->SetShowObject( bShow );
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	if( m_hSeqHandFireSBH1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH1 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
	if( m_hSeqHandFireSBH2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH2 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
	if( m_hSeqHandFireSBH3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH3 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	if( m_hSeqHandElectricALP1 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP1 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
	if( m_hSeqHandElectricALP2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP2 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
#endif //SERV_ADD_LUNATIC_PSYKER

//#ifdef TITLE_SYSTEM
    if( m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
		}
	}
//#endif

	SetShowAttribEnchant( bShow );
}

CKTDGXRenderer::RenderParam* CX2UnitViewerUI::GetRenderParam()
{	
	if(m_pXSkinAnim != NULL)
		return m_pXSkinAnim->GetRenderParam();
	return NULL;
}

//{{ robobeg : 2008-10-17

/*virtual*/
RENDER_HINT   CX2UnitViewerUI::OnFrameRender_Prepare()
{
    if ( !m_sbShow || m_pXSkinAnim == NULL || m_pUnit == NULL)
        return RENDER_HINT_NORENDER;

    m_pXSkinAnim->SetInstantShow( false );
    if ( !m_pXSkinAnim->GetShowObject() )
        return RENDER_HINT_NORENDER;

    RENDER_HINT renderHintResult = m_pXSkinAnim->OnFrameRender_Prepare();
    
    if ( renderHintResult.m_eHint != RENDER_HINT::NORENDER )
        m_pXSkinAnim->SetInstantShow( true );

    if ( !IsRecursivelyManagedByDGManager() && !m_ViewEquipMap.empty() )
    {
        RENDER_HINT renderHint;
        BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
        {
            CX2EqipPtr pX2Eqip = value.second;
            ASSERT( pX2Eqip != NULL && !pX2Eqip->IsManagedByDGManager() );
            if ( pX2Eqip == NULL || pX2Eqip->IsManagedByDGManager() )
                continue;
            pX2Eqip->SetInstantShow( false );

            const CX2Item::ItemTemplet* pItemTemplet = _GetSafeEquipItemTemplet( pX2Eqip );
            if ( pItemTemplet == NULL )
                continue;
            ASSERT( pX2Eqip->GetItemUID() == value.first );
            CX2Unit::EQIP_POSITION ePos = pItemTemplet->GetEqipPosition();
            if ( m_ViewEquipFashion[ ePos ] == value.first )
            {
                ASSERT( pItemTemplet->GetFashion() );
            }
            else if ( m_ViewEquipNormal[ ePos ] == value.first )
            {
                ASSERT( !pItemTemplet->GetFashion() );
            }
            else
            {
                ASSERT( !"internal error" );
            }//if.. else..

            if ( !pX2Eqip->GetShowObject() )
                continue;
            renderHint = pX2Eqip->OnFrameRender_Prepare();
            if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
            {
                pX2Eqip->SetInstantShow( true );
                renderHintResult.MergeRenderHint( renderHint );
            }//if
        }//BOOST_TEST_FOREACH()
    }//if

#ifdef FIX_CLASS_CHANGE_MESH
	if ( true == m_bChangeClass )
	{
	#ifdef CHUNG_SECOND_CLASS_CHANGE
		if ( m_hChungMiniGunL != INVALID_MESH_INSTANCE_HANDLE && m_hChungMiniGunR != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstL = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunL );
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstR = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunR );

			if ( pMeshInstL != NULL && pMeshInstR != NULL )
			{			
				pMeshInstL->OnFrameRender_Prepare();
				pMeshInstR->OnFrameRender_Prepare();
			}
		}
	#endif

	#ifdef ELSWORD_SHEATH_KNIGHT
		if ( m_hElswordSword != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hElswordSword );

			if ( pMeshInst != NULL )
			{
				pMeshInst->OnFrameRender_Prepare();
			}
		}
	#endif ELSWORD_SHEATH_KNIGHT

	#ifdef SERV_TRAPPING_RANGER_TEST
		if ( m_hInstLTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstLTrapSwordReady );

			if ( pMeshInst != NULL )
			{
				pMeshInst->OnFrameRender_Prepare();
			}
		}
	#endif SERV_TRAPPING_RANGER_TEST

	#ifdef SERV_RENA_NIGHT_WATCHER
		if ( m_hInstRNWTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstRNWTrapSwordReady );

			if ( pMeshInst != NULL )
			{
				pMeshInst->OnFrameRender_Prepare();
			}
		}
	#endif SERV_RENA_NIGHT_WATCHER
	}
#else
#ifdef CHUNG_SECOND_CLASS_CHANGE
	if ( m_hChungMiniGunL != INVALID_MESH_INSTANCE_HANDLE && m_hChungMiniGunR != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstL = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunL );
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInstR = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunR );

		if ( pMeshInstL != NULL && pMeshInstR != NULL )
		{			
			pMeshInstL->OnFrameRender_Prepare();
			pMeshInstR->OnFrameRender_Prepare();
		}
	}
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
	if ( m_hElswordSword != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hElswordSword );

		if ( pMeshInst != NULL )
		{
			pMeshInst->OnFrameRender_Prepare();
		}
	}
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_TRAPPING_RANGER_TEST
	if ( m_hInstLTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstLTrapSwordReady );

		if ( pMeshInst != NULL )
		{
			pMeshInst->OnFrameRender_Prepare();
		}
	}
#endif SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
	if ( m_hInstRNWTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstRNWTrapSwordReady );

		if ( pMeshInst != NULL )
		{
			pMeshInst->OnFrameRender_Prepare();
		}
	}
#endif SERV_RENA_NIGHT_WATCHER
#endif

	return renderHintResult;
}//CX2UnitViewerUI::OnFrameRender_Prepare()


//HRESULT CX2UnitViewerUI::OnFrameRender()
/*virtual*/
void    CX2UnitViewerUI::OnFrameRender_Draw()
{
	KTDXPROFILE();

	if(m_pUnit == NULL)
		return;

	// 유효하지 않는 frame을 참조하는 부분을 찾기위한 로그추가
	//WCHAR wszLastErrorLog[255] = {0,};
	////wsprintf(wszLastErrorLog, L"DeviceId: %s", m_pMotion->GetDeviceID().c_str() );
	//StringCchPrintf( wszLastErrorLog, 255, L"DeviceId: %s", m_pMotion->GetDeviceID().c_str() );
	//g_CKTDXLog.EnQueLastErrorLog( wszLastErrorLog );

    if ( m_pXSkinAnim != NULL && m_pXSkinAnim->GetShowObject() && m_pXSkinAnim->IsInstantShow() )
	    m_pXSkinAnim->OnFrameRender_Draw();


// Equip들은 CKTDGObject 로 정의되어 본체가 CKTDGManager 에서 매니지되는 경우 CKTDGManager 에서 렌더링되도록 수정됨.
// CX2UnitViewerUI 에서는 Equip 탈착시에 기존 Equip들의 렌더링 여부를 SetShowObject 로 업데이트해준다.

#ifdef UNIT_VIEWER_EQUIP_ALPHA_RENDER_FIX
	vector<CX2EqipPtr> vecAlphaEqip;
#endif UNIT_VIEWER_EQUIP_ALPHA_RENDER_FIX

    if ( !IsRecursivelyManagedByDGManager() && !m_ViewEquipMap.empty() )
    {
        BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
        {
            CX2EqipPtr pX2Eqip = value.second;
            ASSERT( pX2Eqip != NULL && !pX2Eqip->IsManagedByDGManager() );
            if ( pX2Eqip == NULL || pX2Eqip->IsManagedByDGManager() )
                continue;

            if ( pX2Eqip->GetShowObject() && pX2Eqip->IsInstantShow() )
#ifdef UNIT_VIEWER_EQUIP_ALPHA_RENDER_FIX
			{
				if( pX2Eqip->IsAlphaObject() == false )
					pX2Eqip->OnFrameRender_Draw();
				else
					vecAlphaEqip.push_back( pX2Eqip );
			}
#else
                pX2Eqip->OnFrameRender_Draw();
#endif UNIT_VIEWER_EQUIP_ALPHA_RENDER_FIX
        }//BOOST_TEST_FOREACH()
#ifdef UNIT_VIEWER_EQUIP_ALPHA_RENDER_FIX
		for(UINT i=0; i< vecAlphaEqip.size(); ++i)
		{
			CX2EqipPtr pX2Eqip = vecAlphaEqip[i];
			pX2Eqip->OnFrameRender_Draw();
		}
#endif UNIT_VIEWER_EQUIP_ALPHA_RENDER_FIX
    }//if



	//{{ 2009.02.10 김태완 : Fixed의 경우 유닛뷰어가 그려질 때 이펙트를 그린다.
	if(m_bFixed)
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq;

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

#ifdef NEW_CHARACTER_EL
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK1 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK2 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet1 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet2 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet3 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH1 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH2 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH3 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP1 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP2 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
#endif //SERV_ADD_LUNATIC_PSYKER

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire2 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire3 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind );
		if(pSeq != NULL)
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}		

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind2 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind3 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}


		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind4 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE			/// 무기 강화 파티클
		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon2 );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon3 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon4 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}
#endif ADD_UPGRADE_WEAPON_PARTICLE

#ifdef CHUNG_SECOND_CLASS_CHANGE
		if ( m_hChungMiniGunL != INVALID_MESH_INSTANCE_HANDLE && m_hChungMiniGunR != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstL = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunL );
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstR = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hChungMiniGunR );

			if ( pMeshInstL != NULL && pMeshInstR != NULL )
			{
				if ( pMeshInstL->GetShowObject() )
					pMeshInstL->OnFrameRender_Draw();

				if ( pMeshInstR->GetShowObject() )
					pMeshInstR->OnFrameRender_Draw();
			}
		}
#endif


#ifdef ELSWORD_SHEATH_KNIGHT
		if ( m_hElswordSword != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hElswordSword );

			if ( pMeshInst != NULL )
			{
				if ( pMeshInst->GetShowObject() )
					pMeshInst->OnFrameRender_Draw();
			}
		}
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_TRAPPING_RANGER_TEST
		if ( m_hInstLTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstLTrapSwordReady );

			if ( pMeshInst != NULL )
			{
				if ( pMeshInst->GetShowObject() )
					pMeshInst->OnFrameRender_Draw();
			}
		}
#endif SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
		if ( m_hInstRNWTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hInstRNWTrapSwordReady );

			if ( pMeshInst != NULL )
			{
				if ( pMeshInst->GetShowObject() )
					pMeshInst->OnFrameRender_Draw();
			}
		}
#endif SERV_RENA_NIGHT_WATCHER
	}
	//}}
}

//}} robobeg : 2008-10-17

//{{ kimhc // 2010.9.9 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
#ifdef	AVATAR_IMAGE_TEST

/*virtual*/
RENDER_HINT   CX2UnitViewerUI::OnRender_PrepareForAvartaScreenCapture()
{
	if ( !m_sbShow || m_pXSkinAnim == NULL || m_pUnit == NULL)
		return RENDER_HINT_NORENDER;

	m_pXSkinAnim->SetInstantShow( false );
	if ( !m_pXSkinAnim->GetShowObject() )
		return RENDER_HINT_NORENDER;

	RENDER_HINT renderHintResult = m_pXSkinAnim->OnFrameRender_Prepare();

	if ( renderHintResult.m_eHint != RENDER_HINT::NORENDER )
		m_pXSkinAnim->SetInstantShow( true );

	if ( !m_ViewEquipMap.empty() )
	{
		RENDER_HINT renderHint;
		BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
		{
			CX2EqipPtr pX2Eqip = value.second;
			ASSERT( pX2Eqip != NULL );
			if ( pX2Eqip == NULL )
				continue;
			pX2Eqip->SetInstantShow( false );

			const CX2Item::ItemTemplet* pItemTemplet = _GetSafeEquipItemTemplet( pX2Eqip );
			if ( pItemTemplet == NULL )
				continue;
			ASSERT( pX2Eqip->GetItemUID() == value.first );
			CX2Unit::EQIP_POSITION ePos = pItemTemplet->GetEqipPosition();
			if ( m_ViewEquipFashion[ ePos ] == value.first )
			{
				ASSERT( pItemTemplet->GetFashion() );
			}
			else if ( m_ViewEquipNormal[ ePos ] == value.first )
			{
				ASSERT( !pItemTemplet->GetFashion() );
			}
			else
			{
				ASSERT( !"internal error" );
			}//if.. else..

			if ( !pX2Eqip->GetShowObject() )
				continue;
			renderHint = pX2Eqip->OnFrameRender_Prepare();
			if ( renderHint.m_eHint != RENDER_HINT::NORENDER )
			{
				pX2Eqip->SetInstantShow( true );
				renderHintResult.MergeRenderHint( renderHint );
			}//if
		}//BOOST_TEST_FOREACH()
	}//if

	return renderHintResult;
}//CX2UnitViewerUI::OnFrameRender_Prepare()

void	CX2UnitViewerUI::Render_DrawForAvartaScreenCapture()
{
	KTDXPROFILE();

	if(m_pUnit == NULL)
		return;

	// 유효하지 않는 frame을 참조하는 부분을 찾기위한 로그추가
	//WCHAR wszLastErrorLog[255];
	//wsprintf(wszLastErrorLog, L"DeviceId: %s", m_pMotion->GetDeviceID().c_str() );
	//g_CKTDXLog.EnQueLastErrorLog( wszLastErrorLog );

	if ( m_pXSkinAnim != NULL )
	{
		bool bShowObject	= m_pXSkinAnim->GetShowObject();
		bool bInstanceShow	= m_pXSkinAnim->IsInstantShow();

		if ( bShowObject == true && bInstanceShow == true )
			m_pXSkinAnim->OnFrameRender_Draw();
	}


	// Equip들은 CKTDGObject 로 정의되어 본체가 CKTDGManager 에서 매니지되는 경우 CKTDGManager 에서 렌더링되도록 수정됨.
	// CX2UnitViewerUI 에서는 Equip 탈착시에 기존 Equip들의 렌더링 여부를 SetShowObject 로 업데이트해준다.

	if ( !m_ViewEquipMap.empty() )
	{
		BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
		{
			CX2EqipPtr pX2Eqip = value.second;
			ASSERT( pX2Eqip != NULL );
			if ( pX2Eqip == NULL )
				continue;

			if ( pX2Eqip->GetShowObject() && pX2Eqip->IsInstantShow() )
				pX2Eqip->OnFrameRender_Draw();
		}//BOOST_TEST_FOREACH()
	}//if



	//{{ 2009.02.10 김태완 : Fixed의 경우 유닛뷰어가 그려질 때 이펙트를 그린다.
	if(m_bFixed)
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq;

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
		if( pSeq != NULL )
		{	
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire2 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire3 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind );
		if(pSeq != NULL)
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}		

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind2 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}

		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind3 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}


		pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind4 );
		if( pSeq != NULL )
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	FALSE );
			pSeq->OnFrameRender_Draw();
		}
	}
	//}}
}

#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.9.9 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트

HRESULT CX2UnitViewerUI::OnResetDevice()
{
	
	m_pXSkinAnim->OnResetDevice();

//{{ robobeg : 2008-10-17

	//for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
	//{
	//	CX2Eqip* pX2Eqip = m_ViewEqipList[i];
	//	pX2Eqip->OnResetDevice();
	//}

// 본체가 CKTDGManager 에 등록되어 있지 않는 경우에만 호출됨.

    if ( !IsRecursivelyManagedByDGManager() )
    {
        BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
        {
            CX2EqipPtr pX2Eqip = value.second;

            if ( pX2Eqip == NULL )
                continue;
            ASSERT( !pX2Eqip->IsManagedByDGManager() );
            pX2Eqip->OnResetDevice();
        }//BOOST_TEST_FOREACH()
    }//if
//}} robobeg : 2008-10-17
	
	return S_OK;
}

HRESULT CX2UnitViewerUI::OnLostDevice()
{
	if(m_pXSkinAnim != NULL)
		m_pXSkinAnim->OnLostDevice();

//{{ robobeg : 2008-10-17

// 본체가 CKTDGManager 에 등록되어 있지 않는 경우에만 호출됨.

	//for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
	//{
	//	CX2Eqip* pX2Eqip = m_ViewEqipList[i];
	//	pX2Eqip->OnLostDevice();
	//}

    if ( !IsRecursivelyManagedByDGManager() )
    {
        BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
        {
            CX2EqipPtr pX2Eqip = value.second;

            if ( pX2Eqip == NULL )
                continue;
            ASSERT( !pX2Eqip->IsManagedByDGManager() );
            pX2Eqip->OnLostDevice();
        }//BOOST_TEST_FOREACH()
    }//if

//}} robobeg : 2008-10-17

	return S_OK;
}


//{{ robobeg : 2008-10-17

// X2UnitVIewerUI 가 KTDGManager 에 등록, 삭제될 때 장착 부품들도 같이 등록, 삭제하기 위해 사용됨

/*virtual*/
void    CX2UnitViewerUI::NotifyDGManagerChanged( CKTDGManager& refInManager_ )
{
    if ( IsRecursivelyManagedByDGManager() )
    {
        BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
        {
            if ( value.second != NULL )
                refInManager_.AddObjectChain( value.second.get() );
        }//BOOST_TEST_FOREACH()
    }
    else
    {
        BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
        {
            if ( value.second != NULL )
                refInManager_.RemoveObjectChain( value.second.get()  );
        }//BOOST_TEST_FOREACH()
    }//if.. else..
}//CX2UnitViewerUI::NotifyDGManagerChanged()


//}} robobeg : 2008-10-17



void CX2UnitViewerUI::PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType, bool bTransition )
{
	if ( m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->ChangeAnim( pName, bTransition );
		m_pXSkinAnim->Play( playType );
	}

	if( m_hSeqHandFire != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );

		if( NULL != pSeq )
		{
			if( wcscmp( pName, L"MagicKnight_LobbyWait" ) == 0
				|| wcscmp( pName, L"MagicKnight_LobbyReadyAction" ) == 0 )
			{
				pSeq->SetEmitRate( 30.0f, 50.0f );
			}
			else
			{	
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
	}

#ifdef NEW_CHARACTER_EL
	if( m_hSeqHandFireSPK1 != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqHandFireSPK2 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq1 = 
			g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK1 );

		CKTDGParticleSystem::CParticleEventSequence* pSeq2 = 
			g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK2 );

		if( NULL != pSeq1 && NULL != pSeq2 )
		{
			if( wcscmp( pName, L"SPK_LobbyWait" ) == 0 ||
				wcscmp( pName, L"SPK_LobbyReadyaction") == 0 )
			{
				pSeq1->SetEmitRate( 30.0f, 50.0f );
				pSeq2->SetEmitRate( 30.0f, 50.0f );
			}
			else
			{
				pSeq1->SetEmitRate( 0.0f, 0.0f );
				pSeq2->SetEmitRate( 0.0f, 0.0f );
			}
		}
	}
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	if( m_hSeqEnergyBullet1 != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqEnergyBullet2 != INVALID_PARTICLE_SEQUENCE_HANDLE &&
		m_hSeqEnergyBullet3 != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq1 = 
			g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet1 );

		CKTDGParticleSystem::CParticleEventSequence* pSeq2 = 
			g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet2 );

		CKTDGParticleSystem::CParticleEventSequence* pSeq3 = 
			g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet3 );

		if( NULL != pSeq1 && NULL != pSeq2 && NULL != pSeq3 )
		{
			if( wcscmp( pName, L"ALD_lobbyWait" ) == 0  )
			{
				pSeq1->SetEmitRate( 30.0f, 50.0f );
				pSeq2->SetEmitRate( 30.0f, 50.0f );
				pSeq3->SetEmitRate( 30.0f, 50.0f );
			}
			else
			{
				pSeq1->SetEmitRate( 0.0f, 0.0f );
				pSeq2->SetEmitRate( 0.0f, 0.0f );
				pSeq3->SetEmitRate( 0.0f, 0.0f );
			}
		}
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if( wcscmp( pName, L"CDC_LobbyReadyAction" ) == 0 )
		m_bShowGun = true;
	else
		m_bShowGun = false;
#endif


#ifdef ELSWORD_SHEATH_KNIGHT
	if( wcscmp( pName, L"ETK_LobbyReadyAction" ) == 0 
#ifdef SERV_ELSWORD_INFINITY_SWORD
		|| wcscmp( pName, L"EIS_LobbyReadyAction" ) == 0 
#endif //SERV_ELSWORD_INFINITY_SWORD
		)
	{
		m_bShowSword = true;
		D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
		g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"UI_elsword_active_crecentCut_C01", vBonePos );
		g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"UI_elsword_active_crecentCut_C02", vBonePos );
		g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"UI_elsword_active_crecentCut_C03", vBonePos );
		g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"UI_elsword_active_crecentCut_C04", vBonePos );
	}
	else
		m_bShowSword = false;
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_TRAPPING_RANGER_TEST
	if( wcscmp( pName, L"RTR_LobbyReadyAction" ) == 0 )
		m_bShowLTrapSword = true;
	else
		m_bShowLTrapSword = false;
#endif SERV_TRAPPING_RANGER_TEST

#ifdef SERV_RENA_NIGHT_WATCHER
	if( wcscmp( pName, L"RNW_LobbyReadyAction" ) == 0 )
		m_bShowRNWTrapSword = true;
	else
		m_bShowRNWTrapSword = false;
#endif SERV_RENA_NIGHT_WATCHER
}



void CX2UnitViewerUI::SetUnit( CX2Unit* pUnit, CX2UnitViewerUI::UNIT_VIEWER_STATE eState )
{
	ASSERT( pUnit != NULL );
	if( NULL == pUnit )
		return;


	m_pUnit = pUnit;

	RemoveAllEquips();

	// - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef SEPARATION_MOTION
	m_eUnitViewerState = eState;
	bool bIsEquipWeapone = true;
	const CX2Unit::UnitTemplet *pTemplet = m_pUnit->GetUnitTemplet();
	if( NULL == pTemplet )
	{
		ASSERT(!"UnitTemplet Is NULL!");
		return;
	}

	std::vector<wstring> vecMotion;	
	switch( eState )
	{
	case UVS_CASHSHOP:
		{
			vecMotion.push_back( pTemplet->m_CommonMotion );			
		}
		break;
	case UVS_ROOM:
		{
			vecMotion.push_back( pTemplet->m_CommonMotion );
			vecMotion.push_back( pTemplet->m_LobbyMotion );
			vecMotion.push_back( pTemplet->m_EmotionMotion );
		}
		break;
	case UVS_FIELD:
		{
			vecMotion.push_back( pTemplet->m_CommonMotion );
			vecMotion.push_back( pTemplet->m_FieldMotion );
			vecMotion.push_back( pTemplet->m_EmotionMotion );
		}
		break;
	case UVS_CLASSCHANGE:
		{
			vecMotion.push_back( pTemplet->m_CommonMotion );
			vecMotion.push_back( pTemplet->m_LobbyMotion );
		}
		break;
	case UVS_CREATEUNIT:
		{
			vecMotion.push_back( pTemplet->m_CommonMotion );
			vecMotion.push_back( pTemplet->m_LobbyMotion );
			vecMotion.push_back( pTemplet->m_EmotionMotion );
		}
		break;
	case UVS_SELECTUNIT:
		{
			vecMotion.push_back( pTemplet->m_CommonMotion );
			vecMotion.push_back( pTemplet->m_LobbyMotion );
		}
		break;
	case UVS_CHARINFO:
		{
			bIsEquipWeapone = false;
			vecMotion.push_back( pTemplet->m_CommonMotion );
			//vecMotion.push_back( pTemplet->m_LobbyMotion );
		}
		break;
	case UVS_WATCH:
		{
			bIsEquipWeapone = false;
			vecMotion.push_back( pTemplet->m_CommonMotion );
			//vecMotion.push_back( pTemplet->m_LobbyMotion );
		}
		break;
	default:
		{
			vecMotion.push_back( pTemplet->m_CommonMotion );
			vecMotion.push_back( pTemplet->m_LobbyMotion );
			vecMotion.push_back( pTemplet->m_EmotionMotion );
			vecMotion.push_back( pTemplet->m_FieldMotion );
		}
		break;
	}

	if( pTemplet->m_CommonMotion == L"" )
	{
		vecMotion.clear();
	}

	CKTDXDeviceXSkinMesh* pMotion = NULL;
	CKTDXDeviceXSkinMesh* pMixMotion = NULL;

	if( vecMotion.size() > 0)
	{
		pMotion	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( vecMotion[0] );
	}
	

#else	// SEPARATION_MOTION

#ifdef UNIT_NEW_MOTION
	CKTDXDeviceXSkinMesh* pMotion = NULL;
	if(g_pTFieldGame != NULL || g_pSquareGame != NULL)
	{
		pMotion	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_pUnit->GetUnitTemplet()->m_FieldMotionFile );
	}
	else
	{
		pMotion	= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_pUnit->GetUnitTemplet()->m_MotionFile );
	}
#else
	CKTDXDeviceXSkinMesh* pMotion		= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_pUnit->GetUnitTemplet()->m_MotionFile );
#endif

#endif	// SEPARATION_MOTION

	ASSERT( NULL != pMotion );

	if( pMotion != NULL && m_pMotion != pMotion )
	{
		SAFE_CLOSE( m_pMotion );
		m_pXSkinAnim.reset();

		m_pMotion = pMotion;
		m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnimPtr();
		m_pXSkinAnim->SetAnimXSkinMesh( m_pMotion, m_pMotion->GetXET() );

#ifdef SEPARATION_MOTION
		if( vecMotion.size() > 1 )
		{
			for( UINT iMotion = 1; iMotion < vecMotion.size(); ++iMotion )
			{
				pMixMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( vecMotion[iMotion] );
				if(  pMixMotion != NULL )
				{
					m_pXSkinAnim->MixAnim( pMixMotion );
					SAFE_CLOSE( pMixMotion );
				}		
			}
		}		
#endif


#ifdef ELSWORD_SECOND_CLASS_CHANGE
		SetUnitReForm();
#endif
	}

	if ( m_hSeqHandFire != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFire );
	}

	if( m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_MAGIC_KNIGHT )
	{
		m_hSeqHandFire = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"MagicKnightHandFire", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFire );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
			}
		}
	}
#ifdef NEW_CHARACTER_EL
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSPK1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSPK2 );

	if( CX2Unit::UC_ELESIS_PYRO_KNIGHT == m_pUnit->GetClass() )
	{
		m_hSeqHandFireSPK1 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SPK_LobbyWait_P01", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK1 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK1 );
			if ( pSeq  != NULL )
			{
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		m_hSeqHandFireSPK2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SPK_LobbyWait_P02", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK2 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSPK2 );
			if ( pSeq  != NULL )
			{
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
	}
#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEnergyBullet1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEnergyBullet2 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEnergyBullet3 );

	if( CX2Unit::UC_ARA_LITTLE_DEVIL == m_pUnit->GetClass() )
	{
		m_hSeqEnergyBullet1 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SA_AYR_ORB_KI_Main_S_P01", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet1 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet1 );
			if ( pSeq  != NULL )
				pSeq->SetEmitRate( 0.0f, 0.0f );
		}

		m_hSeqEnergyBullet2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SA_AYR_ORB_KI_Main_S_P02", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet2 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet2 );
			if ( pSeq  != NULL )
				pSeq->SetEmitRate( 0.0f, 0.0f );
		}

		m_hSeqEnergyBullet3 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SA_AYR_ORB_KI_Main_S_P04", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet3 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEnergyBullet3 );
			if ( pSeq  != NULL )
				pSeq->SetEmitRate( 0.0f, 0.0f );
		}
	}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSBH1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSBH2 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandFireSBH3 );

	if( CX2Unit::UC_ELESIS_BLAZING_HEART == m_pUnit->GetClass() )
	{
		m_hSeqHandFireSBH1 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SBH_LobbyWait01", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH1 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH1 );
			if ( pSeq  != NULL )
			{
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		m_hSeqHandFireSBH2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SBH_LobbyWait02", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH2 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH2 );
			if ( pSeq  != NULL )
			{
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		m_hSeqHandFireSBH3 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"SBH_LobbyWait03", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH3 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandFireSBH3 );
			if ( pSeq  != NULL )
			{
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
	}
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandElectricALP1 );
	g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqHandElectricALP2 );

	if( CX2Unit::UC_ADD_LUNATIC_PSYKER == m_pUnit->GetClass() )
	{
		m_hSeqHandElectricALP1 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"ADD_LP_LobbyWait01", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP1 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP1 );
			if ( pSeq  != NULL )
			{
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}

		m_hSeqHandElectricALP2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"ADD_LP_LobbyWait02", 0,0,0 );
		if(m_bFixed)
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP2 );
			if ( pSeq  != NULL )
			{
				// DGManager Chain에서 안 그려지게 하기 위함. 직접 Render 해 준다.
				pSeq->SetShowObject(false);
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqHandElectricALP2 );
			if ( pSeq  != NULL )
			{
				pSeq->SetEmitRate( 0.0f, 0.0f );
			}
		}
	}
#endif //SERV_ADD_LUNATIC_PSYKER

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if( true == bIsEquipWeapone && m_pUnit->GetClass() == CX2Unit::UC_CHUNG_DEADLY_CHASER )
	{
		if ( NULL != m_pXSkinAnim )
		{
			D3DXVECTOR3 vBonePosL = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );
			D3DXVECTOR3 vBonePosR = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_R_Hand" );

			if( m_hChungMiniGunL != INVALID_MESH_INSTANCE_HANDLE )
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hChungMiniGunL );
			if( m_hChungMiniGunR != INVALID_MESH_INSTANCE_HANDLE )
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hChungMiniGunR );

			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstL = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( this, 
				L"Mesh_UI_CSI_CDC_Start_Mini_Gun_L", vBonePosL, GetMatrix().GetRotate(), GetMatrix().GetRotate() );
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInstR = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( this,
				L"Mesh_UI_CSI_CDC_Start_Mini_Gun_R", vBonePosR, GetMatrix().GetRotate(), GetMatrix().GetRotate() );

			if( true == g_pData->GetUIMajorXMeshPlayer()->IsLiveInstance( pMeshInstL ) )
			{
				m_hChungMiniGunL = pMeshInstL->GetHandle();
				pMeshInstL->SetShowObject( false );
			}
			
			if( true == g_pData->GetUIMajorXMeshPlayer()->IsLiveInstance( pMeshInstR ) )
			{
				m_hChungMiniGunR = pMeshInstR->GetHandle();
				pMeshInstR->SetShowObject( false );
			}			
		}
	}
#endif

#ifdef ELSWORD_SHEATH_KNIGHT
	if( true == bIsEquipWeapone && (m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_SHEATH_KNIGHT 
	#ifdef SERV_ELSWORD_INFINITY_SWORD
		|| m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_INFINITY_SWORD 
	#endif //SERV_ELSWORD_INFINITY_SWORD
		))
	{
		if ( NULL != m_pXSkinAnim )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );

			if( m_hElswordSword != INVALID_MESH_INSTANCE_HANDLE )
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hElswordSword );

			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( this, 
				L"Mesh_UI_SheathKnight_SecondSword", vBonePos, GetMatrix().GetRotate(), GetMatrix().GetRotate() );

			if( true == g_pData->GetUIMajorXMeshPlayer()->IsLiveInstance( pMeshInst ) )
			{
				m_hElswordSword = pMeshInst->GetHandle();
	#ifdef FIX_CLASS_CHANGE_MESH
				if ( true == m_bChangeClass )
					pMeshInst->SetOutlineWide( 500 * GetDistanceToCamera() );
	#endif
				
				pMeshInst->SetShowObject( false );
			}
		}
	}
#endif ELSWORD_SHEATH_KNIGHT

#ifdef	SERV_TRAPPING_RANGER_TEST
	if( true == bIsEquipWeapone && m_pUnit->GetClass() == CX2Unit::UC_LIRE_TRAPPING_RANGER )
	{
		if ( m_pXSkinAnim != NULL )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_L_Hand" );

			if( m_hInstLTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hInstLTrapSwordReady );

			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( this, 
				L"Mesh_RTR_Lire_Sword_Ready", vBonePos, GetMatrix().GetRotate(), GetMatrix().GetRotate() );

			if ( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstance( pMeshInst ) )
			{
				m_hInstLTrapSwordReady = pMeshInst->GetHandle();
#ifdef FIX_CLASS_CHANGE_MESH
				if ( true == m_bChangeClass )
					pMeshInst->SetOutlineWide(460 * GetDistanceToCamera() );
#endif
				
				pMeshInst->SetShowObject( false );
			}
		}
	}
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef	SERV_RENA_NIGHT_WATCHER
	if( true == bIsEquipWeapone && m_pUnit->GetClass() == CX2Unit::UC_LIRE_NIGHT_WATCHER )
	{
		if ( m_pXSkinAnim != NULL )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );

			if( m_hInstRNWTrapSwordReady != INVALID_MESH_INSTANCE_HANDLE )
				g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle( m_hInstRNWTrapSwordReady );

			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( this, 
				L"Mesh_RNW_Lire_Sword_Ready", vBonePos, GetMatrix().GetRotate(), GetMatrix().GetRotate() );

			if ( g_pData->GetUIMajorXMeshPlayer()->IsLiveInstance( pMeshInst ) )
			{
				m_hInstRNWTrapSwordReady = pMeshInst->GetHandle();
#ifdef FIX_CLASS_CHANGE_MESH
				if ( true == m_bChangeClass )
					pMeshInst->SetOutlineWide(460 * GetDistanceToCamera() );
#endif
				
				pMeshInst->SetShowObject( false );
			}
		}
	}
#endif	SERV_RENA_NIGHT_WATCHER

	DeleteEnchantWeaponEffect();
	if ( m_pUnit != NULL && m_pUnit->GetUnitTemplet() != NULL )
	{
		switch( m_pUnit->GetUnitTemplet()->m_UnitType )
		{
		default:
			{
				InitEnchantWeaponEffectForSword();
			} break;

		case CX2Unit::UT_LIRE:
			{
				InitEnchantWeaponEffectForBow();
			} break;


		case CX2Unit::UT_EVE:
			{
				InitEnchantWeaponEffectForBow();

				if( NULL == m_pSound_Eve_LevelUp )
				{
					m_pSound_Eve_LevelUp = g_pKTDXApp->GetDeviceManager()->OpenSound( L"EveVoice_LobbyLevelUp.ogg", 10, false );
				}
			} break;

		}

		//{{ 2009.2.4 김태완 : Fixed 되었을 때의 이펙트 처리
		if( true == m_bFixed )
		{
			SetShowAttribEnchant( false );
		}	
		//}}
	}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 강화 무기 이펙트 설정
	DeleteUpgradeWeaponParticle();	/// 설정되어 있을지도 모르니, 삭제

	InitUpgradeWeaponParticle();	/// 강화 무기 이펙트 초기화
#endif ADD_UPGRADE_WEAPON_PARTICLE

	UpdateEqip();	
}

/*static*/
void CX2UnitViewerUI::AppendToDeviceList( CKTDXDeviceDataList& listInOut_, CX2Unit* pUnit, CX2UnitViewerUI::UNIT_VIEWER_STATE eState )
{
	ASSERT( pUnit != NULL );
	if( NULL == pUnit )
		return;
	// - - - - - - - - - - - - - - - - - - - - - - - - -

	const CX2Unit::UnitTemplet *pTemplet = pUnit->GetUnitTemplet();
	if ( pTemplet != NULL )
	{

#ifdef SEPARATION_MOTION

		if ( pTemplet->m_CommonMotion.empty() == false )
		{
			g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_CommonMotion );
			switch( eState )
			{
			case UVS_CASHSHOP:
				break;
			case UVS_ROOM:
				{
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_LobbyMotion );
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_EmotionMotion );
				}
				break;
			case UVS_FIELD:
				{
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_FieldMotion );
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_EmotionMotion );
				}
				break;
			case UVS_CLASSCHANGE:
				{
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_LobbyMotion );
				}
				break;
			case UVS_CREATEUNIT:
				{
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_LobbyMotion );
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_EmotionMotion );
				}
				break;
			case UVS_SELECTUNIT:
				{
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_LobbyMotion );
				}
				break;
			case UVS_CHARINFO:
				{
				}
				break;
			case UVS_WATCH:
				{
				}
				break;
			default:
				{
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_LobbyMotion );
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_EmotionMotion );
					g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_FieldMotion );
				}
				break;
			}
		}//if

#else	// SEPARATION_MOTION

#ifdef UNIT_NEW_MOTION
		if( g_pTFieldGame != NULL || g_pSquareGame != NULL)
		{
			g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_FieldMotionFile );
		}
		else
		{
			g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_MotionFile );
		}
#else
		g_pKTDXApp->GetDeviceManager()->AppendXSkinMesh( listInOut_, pTemplet->m_MotionFile );
#endif

#endif	// SEPARATION_MOTION
	}

	vector<CX2EqipPtr> viewEqipList;
	//#ifdef EQUIP_STRIP_TEST
	//	CX2ItemManager::EquipPositionMap mapBasicEquip;
	//	g_pData->GetItemManager()->UpdateEqip( pUnit, CKTDGXSkinAnimPtr(), NULL, NULL, OUT viewEqipList, OUT mapBasicEquip, false );
	//#else EQUIP_STRIP_TEST
	g_pData->GetItemManager()->UpdateEqip( pUnit, CKTDGXSkinAnimPtr(), NULL, NULL, IN OUT viewEqipList, false );
	//#endif EQUIP_STRIP_TEST

	BOOST_TEST_FOREACH( CX2EqipPtr, pCX2Eqip, viewEqipList )
	{
		if ( pCX2Eqip == NULL )
			continue;
		pCX2Eqip->AppendToDeviceList( listInOut_ );
	}

	//BOOST_FOREACH( CX2EqipRSPtr pCX2Eqip, viewEqipList )
	//{
	//    SAFE_DELETE_KTDGOBJECT( pCX2Eqip );
	//}//BOOST_FOREACH()

	viewEqipList.clear();
}

void CX2UnitViewerUI::UpdateEqip( bool bEconomical /* = false */ )
{
	m_bEnableEquipSlotVisibilityUpdate = false;




	if ( bEconomical == true )
	{
//{{ robobeg : 2008-10-17

        std::set<UidType>       setItemUID;
        std::vector<UidType>    vecItemUIDToDelete;

#ifdef TAKE_OFF_ALL_ITEM
		if( true == m_bShowInvenEquip )
#endif // TAKE_OFF_ALL_ITEM

		{
			const CX2Inventory& kInventory = m_pUnit->GetInventory();

			for ( int i = 0; i < kInventory.GetItemMaxNum(CX2Inventory::ST_E_EQUIP); i++ )
			{
				CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_E_EQUIP, i );

				//{{ kimhc // 실시간 엘소드 중 실시간 내구도 감소
#ifdef	REAL_TIME_ELSWORD
				if ( pItem == NULL )
					continue;

				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && 
					pItem->GetItemData().m_Endurance <= 0 )
					continue;
#else	REAL_TIME_ELSWORD

				if ( pItem != NULL )
#endif	REAL_TIME_ELSWORD
					setItemUID.insert( pItem->GetUID() );
				//}} kimhc // 실시간 엘소드 중 실시간 내구도 감소

			}//for
		}

        BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
        {
            if ( CX2EqipPtr pX2Eqip = value.second )
            {
// AddItemByTID 혹은 ReplaceEmptyToBasic 로 장착한 더미 장비들은 실제 장착 장비와 비교하기 전에 먼저 지워야 한다.
// by robobeg, 2008-10-18
                if ( pX2Eqip->GetItemUID() < 0  )
                {
                    vecItemUIDToDelete.push_back( value.first );
                    continue;
                }//if
            }
            if ( setItemUID.find( value.first ) == setItemUID.end() )
                vecItemUIDToDelete.push_back( value.first );
        }//BOOST_TEST_FOREACH()

        BOOST_TEST_FOREACH( UidType, itemUID, vecItemUIDToDelete )
        {
// 부품을 지울 때마다 빈 위치에 Basic 장비가 장착되는 것을 막는다. 맨 마지막에 ReplaceEmptyToBasic 명령으로 일괄 장착!
// by robobeg, 2008-10-18
            RemoveEqip( itemUID, true );
        }//BOOST_TEST_FOREACH()

        BOOST_TEST_FOREACH( UidType, itemUID, setItemUID )
        {
            if ( m_ViewEquipMap.find( itemUID ) == m_ViewEquipMap.end() )
                AddEqip( itemUID );
        }//BOOST_TEST_FOREACH()

//}} robobeg : 2008-10-17
	}
	else
	{
		RemoveAllEquips();

#ifdef TAKE_OFF_ALL_ITEM
		if( true == m_bShowInvenEquip )
#endif // TAKE_OFF_ALL_ITEM

		{
			const CX2Inventory& kInventory = m_pUnit->GetInventory();
			for ( int i = 0; i < kInventory.GetItemMaxNum(CX2Inventory::ST_E_EQUIP); i++ )
			{
				CX2Item* pItem = kInventory.GetItem( CX2Inventory::ST_E_EQUIP, i );

				//{{ kimhc // 실시간 엘소드 중 실시간 내구도 감소
#ifdef	REAL_TIME_ELSWORD
				if ( pItem == NULL )
					continue;

				if ( pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && 
					pItem->GetItemData().m_Endurance <= 0 )
					continue;
#else	REAL_TIME_ELSWORD

				if ( pItem != NULL )
#endif	REAL_TIME_ELSWORD
				{
					AddEqip( pItem->GetUID() );

				}
			}
		}
	}

	ReplaceEmptyToBasic();

//{{ robobeg : 2008-10-18
	//InitWeaponTrace();
//}} robobeg : 2008-10-18

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
	m_bEnableEquipSlotVisibilityUpdate = true;
	UpdateEquipSlotVisibility();
#endif	NOT_RENDERING_OTHER_EQUIP_POS

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 강화 무기 이펙트 설정
	DeleteUpgradeWeaponParticle();	/// 설정되어 있을지도 모르니, 삭제

#ifdef TAKE_OFF_ALL_ITEM
	if( true == m_bShowInvenEquip )
#endif // TAKE_OFF_ALL_ITEM
	{
		InitUpgradeWeaponParticle();		/// 강화 무기 이펙트 초기화
	}
#endif ADD_UPGRADE_WEAPON_PARTICLE
}

//{{ robobeg : 2008-10-18

void CX2UnitViewerUI::RemoveEqip( UidType itemUID, bool bInNoReplaceEmptyToBasic )
{
    ViewEquipMap::iterator iter = m_ViewEquipMap.find( itemUID );
    if ( iter == m_ViewEquipMap.end() )
        return;
    CX2EqipPtr pX2Eqip = iter->second;
    ASSERT( pX2Eqip != NULL );
    const CX2Item::ItemTemplet* pItemTemplet = _GetSafeEquipItemTemplet( pX2Eqip );
    if ( pItemTemplet == NULL )
    {
        ASSERT( !"internal error" );
        m_ViewEquipMap.erase( iter );
        return;
    }//if

    ASSERT( pX2Eqip->GetItemUID() == itemUID );

    CX2Unit::EQIP_POSITION ePos = pItemTemplet->GetEqipPosition();

	if ( m_ViewEquipFashion[ ePos ] == itemUID )
	{
        ASSERT( pItemTemplet->GetFashion() );
        m_ViewEquipFashion[ ePos ] = 0;
        if ( m_ViewEquipNormal[ ePos ] != 0 )
        {
            ViewEquipMap::iterator iter = m_ViewEquipMap.find( m_ViewEquipNormal[ ePos ] );
            if ( iter != m_ViewEquipMap.end() )
            {
                CX2EqipPtr pX2EqipNormal = iter->second;
                ASSERT( pX2EqipNormal != NULL );
                if ( pX2EqipNormal != NULL )
                    pX2EqipNormal->SetShowObject( true );
            }//if
        }//if
	}
    else if ( m_ViewEquipNormal[ ePos ] == itemUID )
    {
        ASSERT( !pItemTemplet->GetFashion() );
        m_ViewEquipNormal[ ePos ] = 0;
    }
    else
    {
        ASSERT( !"internal error" );
    }//if.. else..

    m_ViewEquipMap.erase( iter );

    if ( !bInNoReplaceEmptyToBasic )
	    ReplaceEmptyToBasic();
    else
	    InitWeaponTrace();

//{{ robobeg : 2008-10-18
	//InitWeaponTrace();
//}} robobeg : 2008-10-18

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
	UpdateEquipSlotVisibility();
#endif	NOT_RENDERING_OTHER_EQUIP_POS

}


//{{ robobeg : 2008-10-17
// 현재 사용하지 않고 있으며, itemTID 에 해당되는 여러개의 아이템을 지워야 하는지, 한개만
// 지워야하는지 애매하여 일단 comment out
//void CX2UnitViewerUI::RemoveEquipByTID( int itemTID )
//{
//	for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
//	{	
//		CX2Eqip* pX2Eqip = m_ViewEqipList[i];
//		if ( pX2Eqip == NULL )
//			continue;
//		
//		CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( pX2Eqip->GetItemID() );
//		if( pItemTemplet != NULL )
//		{
//			if ( pItemTemplet->m_ItemID == itemTID )
//			{			
//				CX2Unit::EQIP_POSITION ePos = pItemTemplet->GetEqipPosition();
//				bool bFashion = pItemTemplet->GetFashion();
//				SAFE_DELETE( pX2Eqip );
//				m_ViewEqipList.erase( m_ViewEqipList.begin() + i );
//
//				if ( bFashion == true )
//				{
//					m_ViewEquipedFashion[ePos] = false;
//				}
//
//				break;
//			}
//		}
//	}
//
//	ReplaceEmptyToBasic();
//
//	/*
//	for( int i = 0; i < (int)m_ViewEqipList.size(); i++ )
//	{	
//		CX2Eqip* pX2Eqip = m_ViewEqipList[i];
//		if( pX2Eqip != NULL && pX2Eqip->GetItem() != NULL && pX2Eqip->GetItem()->GetItemTemplet() != NULL )
//		{
//			if ( pX2Eqip->GetItem()->GetItemTemplet()->m_ItemID == itemTID )
//			{			
//				CX2Unit::EQIP_POSITION ePos = pX2Eqip->GetItem()->GetItemTemplet()->GetEqipPosition();
//				bool bFashion = pX2Eqip->GetItem()->GetItemTemplet()->GetFashion();
//				SAFE_DELETE( pX2Eqip );
//				m_ViewEqipList.erase( m_ViewEqipList.begin() + i );
//
//				if ( bFashion == true )
//				{
//					m_ViewEquipedFashion[ePos] = false;
//				}
//
//				break;
//			}
//		}
//	}
//
//	ReplaceEmptyToBasic();
//	*/
//
//	InitWeaponTrace();
//}
//}} robobeg : 2008-10-17

void CX2UnitViewerUI::RemoveAllEquips()
{
    m_ViewEquipMap.clear();

    ZeroMemory( m_ViewEquipNormal, sizeof( m_ViewEquipNormal ) );
    ZeroMemory( m_ViewEquipFashion, sizeof( m_ViewEquipFashion ) );

	InitWeaponTrace();

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
	InitEquipVisibility();
#endif	NOT_RENDERING_OTHER_EQUIP_POS
}


//{{ robobeg : 2008-10-18

void CX2UnitViewerUI::AddEquipByTID( int itemTID, bool bBasicItem  )
{
    if ( itemTID <= 0 )
        return;

	// 아이템 생성
	CX2Item* pItem = NULL;

	const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );
	if ( pItemTemplet == NULL )
		return;

    CX2Unit::EQIP_POSITION ePos = pItemTemplet->GetEqipPosition();

    ASSERT( ePos >= CX2Unit::EQIP_POSITION( 0 ) && ePos < CX2Unit::EP_END );
    if ( !( ePos >= CX2Unit::EQIP_POSITION( 0 ) && ePos < CX2Unit::EP_END ) )
        return;


    {
	    CX2Item::ItemData kItemData;

	    kItemData.m_ItemID = (int) itemTID;

	    // ViewEqipList 가 UID를 키로 하는 ViewEquipMap 으로 변경되어서 동일한 UID 가 중복 등록될 수 없게 되었다.
	    // 따라서 - 값으로 unique 하게 결정한다.
	    // by robobeg, 2008-10-18

	    if ( m_ViewEquipMap.empty() )
		    kItemData.m_ItemUID = -1;
	    else
		    kItemData.m_ItemUID = min( m_ViewEquipMap.begin()->first, -1 ) - 1;

	    kItemData.m_Endurance = 1;

	    // AddEquipByTID 를 ReplaceEmptyToBasic 에서도 사용할 수 있도록 수정
	    if ( !bBasicItem )
	    {
		    kItemData.m_PeriodType = CX2Item::PT_QUANTITY;
		    kItemData.m_Quantity = 1;
	    }//if

	    pItem = new CX2Item( kItemData, m_pUnit );
	    ASSERT( pItem != NULL );
	    if ( pItem == NULL )
	    {
		    return;
	    }//if
    }

	int enchantLevel = pItem->GetItemData().m_EnchantLevel;

	/*
	if( m_pUnit != NULL && pItem != NULL && pItem->GetItemTemplet() != NULL )
	{
		if( pItem->GetItemTemplet()->GetFashion() == true )
		{
			CX2Item* pTempItem = m_pUnit->GetInventory().GetEquippingItemByEquipPos( pItem->GetItemTemplet()->GetEqipPosition(), false );
			if( pTempItem != NULL )
			{
				enchantLevel = pTempItem->GetItemData().m_EnchantLevel;
			}				 
		}
		else
		{
			enchantLevel = pItem->GetItemData().m_EnchantLevel;
		}
	}
	*/



// 패션 장착 여부와 상관없이 항상 부품을 달아준다.
// 대신 패션 - 일반 부품이 동시에 달렸을 경우에는 일반 부품은 표시되지 않게 한다.
// 메모리릭 오류 수정 - by robobeg, 2008-10-18


	CX2EqipPtr pX2Eqip;// = NULL;
#ifdef COMMON_ITEM_TEMPLET_TEST
	if( pItem->GetItemTemplet()->GetUseCondition() == CX2Item::UC_ANYONE )
	{
		//pX2Eqip = new CX2Eqip( pItem, m_pXSkinAnim, enchantLevel, true, true, m_pUnit->GetType() );
		pX2Eqip = CX2Eqip::CreateEquip( pItem, m_pXSkinAnim, enchantLevel, true, true, m_pUnit->GetType() );
	}
	else
#endif COMMON_ITEM_TEMPLET_TEST
	{
		//pX2Eqip = new CX2Eqip( pItem, m_pXSkinAnim, enchantLevel, true );
		pX2Eqip = CX2Eqip::CreateEquip( pItem, m_pXSkinAnim, enchantLevel, true );
	}


    ASSERT( pX2Eqip != NULL );
    if ( pX2Eqip == NULL )
    {
        SAFE_DELETE( pItem );
        return;
    }//if

    pX2Eqip->SetManagerObject( this );
    if ( IsRecursivelyManagedByDGManager() )
        g_pKTDXApp->GetDGManager()->AddObjectChain( pX2Eqip.get() );

    ASSERT( pItemTemplet == pItem->GetItemTemplet() );
	pItemTemplet = pItem->GetItemTemplet();
    ASSERT( pItemTemplet != NULL );
    ePos = pItemTemplet->GetEqipPosition();
    ASSERT( ePos >= CX2Unit::EQIP_POSITION( 0 ) && ePos < CX2Unit::EP_END );

    ViewEquipMap::iterator iter;

    if ( pItemTemplet->GetFashion() )
    {
        if ( UidType tempUID = m_ViewEquipFashion[ ePos ] )
        {
            if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
            {
                ASSERT( iter->second != NULL );
                m_ViewEquipMap.erase( iter );
            }
            else
            {
                ASSERT( !"internal error" );
            }//if.. else..
            m_ViewEquipFashion[ ePos ] = 0;
        }//if
        m_ViewEquipMap.insert( ViewEquipMap::value_type( pItem->GetUID(), pX2Eqip ) );
        m_ViewEquipFashion[ ePos ] = pItem->GetUID();

        if ( UidType tempUID = m_ViewEquipNormal[ ePos ]  )
        {
            if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
            {
                CX2EqipPtr pX2EqipTemp = iter->second;
                if ( pX2EqipTemp != NULL )
                    pX2EqipTemp->SetShowObject( false );
            }
            else
            {
                ASSERT( !"internal error" );
                m_ViewEquipNormal[ ePos ] = 0;
            }//if.. else..
        }//if
    }
    else
    {
        if ( UidType tempUID = m_ViewEquipNormal[ ePos ] )
        {
            if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
            {
                ASSERT( iter->second != NULL );
                m_ViewEquipMap.erase( iter );
            }
            else
            {
                ASSERT( !"internal error" );
            }//
            m_ViewEquipNormal[ ePos ] = 0;
        }//if
        m_ViewEquipMap.insert( ViewEquipMap::value_type( pItem->GetUID(), pX2Eqip ) );
        m_ViewEquipNormal[ ePos ] = pItem->GetUID();

		// 오현빈 // 2012-13-17 // 캐시샵에서 카테고리가 다른 액세서리의 미리보기가 정상적으로 동작 하도록 하기 위해 추가
		if ( NULL != g_pData->GetCashShop() && 
			 true == g_pData->GetCashShop()->GetOpen() )
		{
			if ( UidType tempUID = m_ViewEquipFashion[ ePos ] )
			{
				if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
				{
					ASSERT( iter->second != NULL );
					m_ViewEquipMap.erase( iter );
				}
				else
				{
					ASSERT( !"internal error" );
				}//
				m_ViewEquipFashion[ ePos ] = 0;
			}//if
		}
		// 오현빈 // 2012-13-17 // 캐시샵에서 카테고리가 다른 액세서리의 미리보기가 정상적으로 동작 하도록 하기 위해 추가

        if ( m_ViewEquipFashion[ ePos ] != 0 )
        {
            pX2Eqip->SetShowObject( false );
        }//if
    }//if.. else..

	InitWeaponTrace();

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
	UpdateEquipSlotVisibility();
#endif	NOT_RENDERING_OTHER_EQUIP_POS

}

void CX2UnitViewerUI::AddEqip( UidType itemUID )
{
    if ( itemUID <= 0 )
        return;

    if ( m_ViewEquipMap.find( itemUID ) != m_ViewEquipMap.end() )
        return;

	CX2Item* pItem = m_pUnit->GetInventory().GetItem( itemUID );
	if( pItem == NULL )
		return;

    ASSERT( pItem->GetUID() == itemUID );
    if ( pItem->GetUID() != itemUID )
        return;

	if ( m_pUnit != NULL )
	{
		if ( m_pUnit->EqipAbility( pItem ) == false )
			return;
	}//if

	const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
    ASSERT( pItemTemplet != NULL );
	if ( pItemTemplet == NULL )
		return;

    CX2Unit::EQIP_POSITION ePos = pItemTemplet->GetEqipPosition();
    ASSERT( ePos >= CX2Unit::EQIP_POSITION( 0 ) && ePos < CX2Unit::EP_END );
    if ( !( ePos >= CX2Unit::EQIP_POSITION( 0 ) && ePos < CX2Unit::EP_END ) )
        return;
	
	int enchantLevel = pItem->GetItemData().m_EnchantLevel;
	/*
	if( m_pUnit != NULL && pItem != NULL && pItem->GetItemTemplet() != NULL )
	{
		if( pItem->GetItemTemplet()->GetFashion() == true )
		{
			CX2Item* pTempItem = m_pUnit->GetInventory().GetEquippingItemByEquipPos( pItem->GetItemTemplet()->Get_EqipPosition(), false );
			if( pTempItem != NULL )
			{
				enchantLevel = pTempItem->GetItemData().m_EnchantLevel;
			}				 
		}
		else
		{
			enchantLevel = pItem->GetItemData().m_EnchantLevel;
		}
	}
	*/

// 패션 장착 여부와 상관없이 항상 부품을 달아준다.
// 대신 패션 - 일반 부품이동시에 달렸을 경우에는 일반 부품은 표시되지 않게 한다.
// by robobeg, 2008-10-18


	CX2EqipPtr pX2Eqip;// = NULL;
#ifdef COMMON_ITEM_TEMPLET_TEST
	if( pItemTemplet->GetUseCondition() == CX2Item::UC_ANYONE )
	{
		//pX2Eqip = new CX2Eqip( pItem, m_pXSkinAnim, enchantLevel, false, true, m_pUnit->GetType() );
		pX2Eqip = CX2Eqip::CreateEquip( pItem, m_pXSkinAnim, enchantLevel, false, true, m_pUnit->GetType() );
	}
	else
#endif COMMON_ITEM_TEMPLET_TEST
	{
		//pX2Eqip = new CX2Eqip( pItem, m_pXSkinAnim, enchantLevel );
		pX2Eqip = CX2Eqip::CreateEquip( pItem, m_pXSkinAnim, enchantLevel );
	}




    ASSERT( pX2Eqip != NULL );
    if ( pX2Eqip == NULL )
        return;
    pX2Eqip->SetManagerObject( this );
    if ( IsRecursivelyManagedByDGManager() )
        g_pKTDXApp->GetDGManager()->AddObjectChain( pX2Eqip.get() );

#ifdef NEW_VILLAGE_UI
	if(m_bFixed && (pX2Eqip->GetEqipType() == CX2Item::UT_ATTACH_ANIM || pX2Eqip->GetEqipType() == CX2Item::UT_ATTACH_NORMAL ) )
		pX2Eqip->SetFixed();
#endif

    ViewEquipMap::iterator iter;

    if ( pItemTemplet->GetFashion() )
    {
        if ( UidType tempUID = m_ViewEquipFashion[ ePos ] )
        {
            if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
            {
                ASSERT( iter->second != NULL );
                m_ViewEquipMap.erase( iter );
            }
            else
            {
                ASSERT( !"internal error" );
            }//if.. else..
            m_ViewEquipFashion[ ePos ] = 0;
        }//if
        m_ViewEquipMap.insert( ViewEquipMap::value_type( itemUID, pX2Eqip ) );
        m_ViewEquipFashion[ ePos ] = itemUID;

        if ( UidType tempUID = m_ViewEquipNormal[ ePos ] )
        {
            if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
            {
                CX2EqipPtr pX2EqipTemp = iter->second;
                ASSERT( pX2EqipTemp != NULL );
                if ( pX2EqipTemp != NULL )
                    pX2EqipTemp->SetShowObject( false );
            }
            else
            {
                ASSERT( !"internal error" );
                m_ViewEquipNormal[ ePos ] = 0;
            }//if.. else..
        }//if
    }
    else
    {
        if ( UidType tempUID = m_ViewEquipNormal[ ePos ] )
        {
            if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
            {
                ASSERT( iter->second != NULL );
                m_ViewEquipMap.erase( iter );
            }
            else
            {
                ASSERT( !"internal error" );
            }//
            m_ViewEquipNormal[ ePos ] = 0;
        }//if
        m_ViewEquipMap.insert( ViewEquipMap::value_type( itemUID, pX2Eqip ) );
        m_ViewEquipNormal[ ePos ] = itemUID;

		// 오현빈 // 2012-13-17 // 캐시샵에서 카테고리가 다른 액세서리의 미리보기가 정상적으로 동작 하도록 하기 위해 추가
		if ( NULL != g_pData->GetCashShop() && 
			true == g_pData->GetCashShop()->GetOpen() )
		{
			if ( UidType tempUID = m_ViewEquipFashion[ ePos ] )
			{
				if ( ( iter = m_ViewEquipMap.find( tempUID ) ) != m_ViewEquipMap.end() )
				{
					ASSERT( iter->second != NULL );
					m_ViewEquipMap.erase( iter );
				}
				else
				{
					ASSERT( !"internal error" );
				}//
				m_ViewEquipFashion[ ePos ] = 0;
			}//if
		}
		// 오현빈 // 2012-13-17 // 캐시샵에서 카테고리가 다른 액세서리의 미리보기가 정상적으로 동작 하도록 하기 위해 추가

        if ( m_ViewEquipFashion[ ePos ] != 0 )
        {
            pX2Eqip->SetShowObject( false );
        }//if
    }//if.. else..

	InitWeaponTrace();

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
	UpdateEquipSlotVisibility();
#endif	NOT_RENDERING_OTHER_EQUIP_POS
	
}



void CX2UnitViewerUI::ReplaceEmptyToBasic()
{
	//빠진 부위가 있으면 기본 아이템으로 대체한다
//{{ robobeg : 2008-10-17
	//CheckEmptyEqipPosition();
//}} robobeg : 2008-10-17


	ReplaceEmptyToBasic( CX2Unit::EP_DEFENCE_HAIR,	m_pUnit->GetUnitTemplet()->m_BasicHairItemID		);
	ReplaceEmptyToBasic( CX2Unit::EP_DEFENCE_FACE,	m_pUnit->GetUnitTemplet()->m_BasicFaceItemID		);
	ReplaceEmptyToBasic( CX2Unit::EP_DEFENCE_BODY,	m_pUnit->GetUnitTemplet()->m_BasicBodyItemID		);
	ReplaceEmptyToBasic( CX2Unit::EP_DEFENCE_LEG,	m_pUnit->GetUnitTemplet()->m_BasicLegItemID			);
	ReplaceEmptyToBasic( CX2Unit::EP_DEFENCE_HAND,	m_pUnit->GetUnitTemplet()->m_BasicHandItemID		);
	ReplaceEmptyToBasic( CX2Unit::EP_DEFENCE_FOOT,	m_pUnit->GetUnitTemplet()->m_BasicFootItemID		);
	ReplaceEmptyToBasic( CX2Unit::EP_WEAPON_HAND,	m_pUnit->GetUnitTemplet()->m_BasicWeaponItemID		);

	switch( m_pUnit->GetUnitTemplet()->m_UnitType )
	{
	case CX2Unit::UT_RAVEN:
		{
			ReplaceEmptyToBasic( CX2Unit::EP_RAVEN_LEFT_ARM,	m_pUnit->GetUnitTemplet()->m_RavenLeftArmItemID		);
		} break;
	}

	

//{{ robobeg : 2008-10-18
	InitWeaponTrace();
//}} robobeg : 2008-10-18
}


//}} robobeg : 2008-10-18

#ifdef	NOT_RENDERING_OTHER_EQUIP_POS
void CX2UnitViewerUI::InitEquipVisibility()
{
	for( int i=0; i<ARRAY_SIZE( m_bViewEquipVisible ); i++ )
	{
		m_bViewEquipVisible[i] = true; 
	}
}


void CX2UnitViewerUI::UpdateEquipSlotVisibility()
{
	KTDXPROFILE();

	if( false == m_bEnableEquipSlotVisibilityUpdate )
		return; 
	

	InitEquipVisibility(); // fix!! 함수 이름 바꾸기

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
	bool bOnePieceAvatarWithNotRenderHairItem = false;
	BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value2, m_ViewEquipMap )
	{
		if( IsOnePieceAvatarWithNotRenderHairItem( value2.second->GetItemID() ) == true && NULL != value2.second->GetItem())
		{
			bOnePieceAvatarWithNotRenderHairItem = true;
		}
	}
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

#ifdef ONE_PIECE_AVATAR	
	bool bOnePieceAvatarCheck = false;
	BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
	{
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		if ( IsOnePieceAvatarRenderHair( value.second->GetItemID() ) == true && NULL != value.second->GetItem() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				if( bOnePieceAvatarWithNotRenderHairItem )
				{
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_BODY ] = false;
					bOnePieceAvatarCheck = true;
					break;
				}
				else
				{
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_BODY ] = false;
					bOnePieceAvatarCheck = true;
					break;
				}
			}
		}
#endif SERV_NEW_ONE_PIECE_AVATAR_SLOT

		if ( IsOnePieceAvatar( value.second->GetItemID() ) == true && NULL != value.second->GetItem() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ] = false;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_BODY ] = false;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				bOnePieceAvatarCheck = true;
				break;
			}
			
		}
		if ( IsOnePieceAvatarNotRenderFace1( value.second->GetItemID() ) == true )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE1 ] = false;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_BODY ] = false;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				bOnePieceAvatarCheck = true;
				break;
			}
		}
		if ( IsOnePieceAvatarNotRenderFace1Face2( value.second->GetItemID() ) == true )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE1 ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ] = false;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_BODY ] = false;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				bOnePieceAvatarCheck = true;
				break;
			}
		}
		if ( IsOnePieceAvatarNotRenderFace( value.second->GetItemID() ) == true )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FACE ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE3 ] = false;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_BODY ] = false;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				bOnePieceAvatarCheck = true;
				break;
			}

		}
		/// 한벌 아바타 외에 모든 악세서리를 꺼야 하는 아이템
		if ( IsOnePieceAvatarNotRenderHairAndAllAccessory( value.second->GetItemID() ) == true )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE1 ]		= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ]		= false;
				//m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE3 ]		= false;	/// 요청으로 인한 얼굴 (하) 제외
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_BODY ]		= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_LEG ]			= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_ARM ]			= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_NECKLESS ]	= false;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_BODY ]	= false;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				bOnePieceAvatarCheck = true;

				break;
			}
		}
		if ( IsOnePieceAvatarRenderingHairAndFace1( value.second->GetItemID() ) == true && NULL != value.second->GetItem() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ] = false;
				break;
			}

		}
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		/// 한벌 아바타 외에 모든 악세서리를 꺼야 하는 아이템
		if ( IsOnePieceAvatarOnlyRenderFace1Face2Face3Equip( value.second->GetItemID() ) == true )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_LEG ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FOOT ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_BODY ]		= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_LEG ]			= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_ARM ]			= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_NECKLESS ]	= false;
				bOnePieceAvatarCheck = true;

				break;
			}
		}

#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		// 2012.12.02 darkstarbt_조성욱 // 한벌 아바타 인데 상의랑 하의가 비어 있어서 기존 입고 있는 아바타 들이 나와야 하는 경우
		if ( IsOnePieceAvatarWithRenderBodyItem( value.second->GetItemID() ) == true )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ]	= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE1 ]		= false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ]		= false;
				//m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE3 ]		= false;	/// 요청으로 인한 얼굴 (하) 제외
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_NECKLESS ]	= false;
				bOnePieceAvatarCheck = true;

				break;
			}
		}
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT		
	}
#endif ONE_PIECE_AVATAR

	BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
	{
		if ( IsNoRenderNasodHand( value.second->GetItemID() ) == true && NULL != value.second->GetItem() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_RAVEN_LEFT_ARM ] = false;
				break;
			}
		}
	} 

#ifdef FACE_OFF_MONSTER_HEAD_TEST
	BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
	{
		if ( IsNoRenderHandEquip( value.second->GetItemID() ) == true && NULL != value.second->GetItem() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAND ] = false;
				break;
			}
		}
	}
#endif FACE_OFF_MONSTER_HEAD_TEST


	BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
	{
		// kimhc // 이 이상 안보여야 할 분류가 달라 진다면
		// 안보여야할 부위 및 그런 기능을 가지고 있는 아이템을 스크립트로 빼야할듯
		// 현재는 헤어만 안보이게 하는 아이템과 헤어, 얼굴 악세(중,하)를 안보이게 하는 아이템이 같은 위치 이므로
		// 한개만 착용 할 수 있음
		if ( IsNoRenderHairFace2Face3Equip( value.second->GetItemID() ) == true && NULL != value.second->GetItem() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
					m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
				//m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE3 ] = false;

				break;
			}
		}
		else if ( IsNoRenderHairFace2Equip( value.second->GetItemID() ) == true && NULL != value.second->GetItem() )
		{
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
				//m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ] = false;		
				break;
			}
		} 
		else if ( IsNoRenderHair( value.second->GetItemID() ) == true )
		{
#ifdef ONE_PIECE_AVATAR
			if(bOnePieceAvatarCheck == true)
			{
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
				if(bOnePieceAvatarWithNotRenderHairItem)
				{

				}
				else
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
				{
					m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE1 ] = false;
					m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ] = false;
					break;
				}
			}
#endif ONE_PIECE_AVATAR
						
			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
				//m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;

				break;
			}
		}
#ifdef FACE_OFF_MONSTER_HEAD_TEST
		else if ( IsNoRenderHairFaceFace2Face3Equip( value.second->GetItemID() ) == true )
		{
#ifdef ONE_PIECE_AVATAR
			if(bOnePieceAvatarCheck == true)
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE1 ] = false;
				break;
			}
#endif ONE_PIECE_AVATAR

			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
				//m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FACE ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE3 ] = false;

				break;
			}
		}
		else if ( IsNoRenderHairFaceFace1Face3Equip( value.second->GetItemID() ) == true )
		{
#ifdef ONE_PIECE_AVATAR
			if(bOnePieceAvatarCheck == true)
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE2 ] = false;
				break;
			}
#endif ONE_PIECE_AVATAR

			const CX2Item::ItemTemplet* pItemTemplet = value.second->GetItem()->GetItemTemplet();
			if ( pItemTemplet == NULL )
			{
				ASSERT( L"Wrong Path!" );
				return;
			}

			if ( pItemTemplet->GetFashion() == true ||	// 패션이거나
				m_ViewEquipFashion[pItemTemplet->GetEqipPosition()] == 0 )	// 패션이 아닌데 패션 자리에 아무것도 없으면
				//m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
			{
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_HAIR ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_DEFENCE_FACE ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE1 ] = false;
				m_bViewEquipVisible[ (int) CX2Unit::EP_AC_FACE3 ] = false;

				break;
			}
		}
#endif FACE_OFF_MONSTER_HEAD_TEST
	}


	for( int i=0; i<ARRAY_SIZE( m_bViewEquipVisible ); i++ )
	{
		CX2EqipPtr pEquipFashion;
		CX2EqipPtr pEquipNormal;

		if( UidType tempUID = m_ViewEquipFashion[ i ] )
		{
			ViewEquipMap::iterator it = m_ViewEquipMap.find( tempUID );
			if( it != m_ViewEquipMap.end() )
			{
				pEquipFashion = it->second;
			}
			else
			{
				ASSERT( !"internal error" );
				m_ViewEquipFashion[ i ] = 0;
			}
		}

		if( UidType tempUID = m_ViewEquipNormal[ i ] )
		{
			ViewEquipMap::iterator it = m_ViewEquipMap.find( tempUID );
			if( it != m_ViewEquipMap.end() )
			{
				pEquipNormal = it->second;
			}
			else
			{
				ASSERT( !"internal error" );
				m_ViewEquipNormal[ i ] = 0;
			}
		}



		if( true == m_bViewEquipVisible[i] )
		{
			bool bFashionItemEquipped = false;
			if( pEquipFashion != NULL )
			{
				bFashionItemEquipped = true;
				pEquipFashion->SetShowObject( true );
			}

			if( pEquipNormal != NULL )
			{
				pEquipNormal->SetShowObject( !bFashionItemEquipped );
			}
		}
		else
		{
			if( pEquipFashion != NULL )
			{
				pEquipFashion->SetShowObject( false );
			}

			if( pEquipNormal != NULL )
			{
				pEquipNormal->SetShowObject( false );
			}
		}
	}
}
#endif	NOT_RENDERING_OTHER_EQUIP_POS

void CX2UnitViewerUI::PlayByMotionType( CX2UnitViewerUI::UNIT_VIEWER_UI_MOTION_TYPE motionType )
{
#ifdef UNIT_EMOTION
	m_eOldMotionType = motionType;	
#endif
	// note!! 전직 추가되면 수정
	if ( m_pUnit != NULL )
	{
		switch(motionType)
		{
		case CX2UnitViewerUI::UVUMT_WAIT:
			{
				switch( m_pUnit->GetUnitData().m_UnitClass ) 
				{
				case CX2Unit::UC_ELSWORD_KNIGHT:
					PlayAnim( L"Knight_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
					PlayAnim( L"MagicKnight_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#ifdef ELSWORD_SHEATH_KNIGHT
				case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
					PlayAnim( L"ETK_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif ELSWORD_SHEATH_KNIGHT
				case CX2Unit::UC_LIRE_COMBAT_RANGER:
					PlayAnim( L"CR_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_LIRE_SNIPING_RANGER:
					PlayAnim( L"SR_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#ifdef	SERV_TRAPPING_RANGER_TEST
				case CX2Unit::UC_LIRE_TRAPPING_RANGER:
					PlayAnim( L"RTR_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif	SERV_TRAPPING_RANGER_TEST
				case CX2Unit::UC_ARME_HIGH_MAGICIAN:
					PlayAnim( L"HM_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_ARME_DARK_MAGICIAN:
					PlayAnim( L"DM_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
				case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
					PlayAnim( L"BM_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif
				case CX2Unit::UC_RAVEN_SOUL_TAKER:
					PlayAnim( L"ST_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_RAVEN_OVER_TAKER:
					PlayAnim( L"OT_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#ifdef RAVEN_WEAPON_TAKER
				case CX2Unit::UC_RAVEN_WEAPON_TAKER:
					PlayAnim( L"RWT_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif RAVEN_WEAPON_TAKER
				case CX2Unit::UC_EVE_EXOTIC_GEAR:				
					PlayAnim( L"EG_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_EVE_ARCHITECTURE:				
					PlayAnim( L"AT_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true ); 		
					break;
#ifdef EVE_ELECTRA
				case CX2Unit::UC_EVE_ELECTRA:
					PlayAnim( L"PR_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif EVE_ELECTRA
				case CX2Unit::UC_ELSWORD_LORD_KNIGHT:			
					PlayAnim( L"LK_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true ); 		
					break;
				case CX2Unit::UC_ELSWORD_RUNE_SLAYER:			
					PlayAnim( L"RS_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true ); 		
					break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
				case CX2Unit::UC_ELSWORD_INFINITY_SWORD:			
					PlayAnim( L"EIS_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true ); 		
					break;
#endif

				case CX2Unit::UC_LIRE_WIND_SNEAKER:				
					PlayAnim( L"LWS_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );		
					break;
				case CX2Unit::UC_LIRE_GRAND_ARCHER:				
					PlayAnim( L"LGA_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#ifdef SERV_RENA_NIGHT_WATCHER
				case CX2Unit::UC_LIRE_NIGHT_WATCHER:				
					PlayAnim( L"RNW_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif SERV_RENA_NIGHT_WATCHER

				case CX2Unit::UC_ARME_ELEMENTAL_MASTER:			
					PlayAnim( L"EM_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );		
					break;
				case CX2Unit::UC_ARME_VOID_PRINCESS:			
					PlayAnim( L"VP_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );		
					break;
#ifdef SERV_ARME_DIMENSION_WITCH
				case CX2Unit::UC_ARME_DIMENSION_WITCH:			
					PlayAnim( L"DW_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );		
					break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
				case CX2Unit::UC_RAVEN_BLADE_MASTER:
					PlayAnim( L"RBM_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_RAVEN_RECKLESS_FIST:
					PlayAnim( L"RRF_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:				
					PlayAnim( L"RVC_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif SERV_RAVEN_VETERAN_COMMANDER

				//{{ kimhc // 2010.1.22 // 이브 2차전직 LOBBY_WAIT 모션
#ifdef EVE_SECOND_CLASS_CHANGE
				case CX2Unit::UC_EVE_CODE_NEMESIS:				
					PlayAnim( L"NS_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_EVE_CODE_EMPRESS:				
					PlayAnim( L"EP_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true ); 		
					break;
#endif EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.1.22 // 이브 2차전직 LOBBY_WAIT 모션

#ifdef SERV_EVE_BATTLE_SERAPH
				case CX2Unit::UC_EVE_BATTLE_SERAPH:				
					PlayAnim( L"BS_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif

				//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_FURY_GUARDIAN:				
					PlayAnim( L"CFG_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:				
					PlayAnim( L"CSG_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true ); 		
					break;
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.3 // 청 1차 전직

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
					PlayAnim( L"CHG_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif

					//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_IRON_PALADIN:				
					PlayAnim( L"CIP_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_CHUNG_DEADLY_CHASER:				
					PlayAnim( L"CDC_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true ); 		
					break;
#endif
						//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
				case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:				
					PlayAnim( L"CTT_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
				case CX2Unit::UC_ARA_LITTLE_HSIEN:
					PlayAnim( L"ALH_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_ARA_SAKRA_DEVANAM:
					PlayAnim( L"ASD_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef NEW_CHARACTER_EL
				case CX2Unit::UC_ELESIS_SABER_KNIGHT:
					PlayAnim( L"SSK_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
					PlayAnim( L"SPK_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				case CX2Unit::UC_ARA_LITTLE_DEVIL:
					PlayAnim( L"ALD_lobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_ARA_YAMA_RAJA:
					PlayAnim( L"AYR_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
				case CX2Unit::UC_ELESIS_GRAND_MASTER:
					PlayAnim( L"SGM_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
				case CX2Unit::UC_ELESIS_BLAZING_HEART:
					PlayAnim( L"SBH_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );

					break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UC_ADD_PSYCHIC_TRACER:
					PlayAnim( L"APT_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				case CX2Unit::UC_ADD_LUNATIC_PSYKER:
					PlayAnim( L"ALP_LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					break;
#endif //SERV_ADD_LUNATIC_PSYKER

				default:
					{
						PlayAnim( L"LobbyWait", CKTDGXSkinAnim::XAP_LOOP, true );
					} break;
				}
			} break;

		case CX2UnitViewerUI::UVUMT_WAIT1:
			{
				switch( m_pUnit->GetUnitData().m_UnitClass ) 
				{
				case CX2Unit::UC_ELSWORD_KNIGHT:
					{
						PlayAnim( L"Knight_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;

				case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
					{
						PlayAnim( L"MagicKnight_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

#ifdef ELSWORD_SHEATH_KNIGHT
				case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
					{
						PlayAnim( L"ETK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;
#endif ELSWORD_SHEATH_KNIGHT

				case CX2Unit::UC_LIRE_COMBAT_RANGER:
					{
						PlayAnim( L"CR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

				case CX2Unit::UC_LIRE_SNIPING_RANGER:
					{
						PlayAnim( L"SR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

#ifdef SERV_TRAPPING_RANGER_TEST
				case CX2Unit::UC_LIRE_TRAPPING_RANGER:
					{
						PlayAnim( L"RTR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;
#endif SERV_TRAPPING_RANGER_TEST

				case CX2Unit::UC_RAVEN_SOUL_TAKER:
					{
						PlayAnim( L"ST_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	

					} break;
				case CX2Unit::UC_RAVEN_OVER_TAKER:
					{
						PlayAnim( L"OT_LobbyWait1", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	

					} break;

				case CX2Unit::UC_EVE_EXOTIC_GEAR:
					PlayAnim( L"EG_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;

				case CX2Unit::UC_EVE_ARCHITECTURE:
					PlayAnim( L"AT_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;

#ifdef EVE_ELECTRA
				case CX2Unit::UC_EVE_ELECTRA:
					PlayAnim( L"PR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;
#endif EVE_ELECTRA

				case CX2Unit::UC_ELSWORD_LORD_KNIGHT:			PlayAnim( L"LK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
				case CX2Unit::UC_ELSWORD_RUNE_SLAYER:			PlayAnim( L"RS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
				case CX2Unit::UC_ELSWORD_INFINITY_SWORD:		PlayAnim( L"EIS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif

				case CX2Unit::UC_LIRE_WIND_SNEAKER:				PlayAnim( L"LWS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
				case CX2Unit::UC_LIRE_GRAND_ARCHER:				PlayAnim( L"LGA_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
#ifdef SERV_RENA_NIGHT_WATCHER
				case CX2Unit::UC_LIRE_NIGHT_WATCHER:			PlayAnim( L"RNW_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
#endif SERV_RENA_NIGHT_WATCHER

				case CX2Unit::UC_ARME_ELEMENTAL_MASTER:			PlayAnim( L"EM_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
				case CX2Unit::UC_ARME_VOID_PRINCESS:			PlayAnim( L"VP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
#ifdef SERV_ARME_DIMENSION_WITCH
				case CX2Unit::UC_ARME_DIMENSION_WITCH:			PlayAnim( L"DW_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
				case CX2Unit::UC_RAVEN_BLADE_MASTER:
					PlayAnim( L"RBM_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_RAVEN_RECKLESS_FIST:
					PlayAnim( L"RRF_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:		PlayAnim( L"RVC_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
#endif SERV_RAVEN_VETERAN_COMMANDER

					//{{ kimhc // 2010.1.22 // 이브 2차전직 LOBBY_WAIT 모션
#ifdef EVE_SECOND_CLASS_CHANGE
				case CX2Unit::UC_EVE_CODE_NEMESIS:				
					PlayAnim( L"NS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_EVE_CODE_EMPRESS:				
					PlayAnim( L"EP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif EVE_SECOND_CLASS_CHANGE
					//}} kimhc // 2010.1.22 // 이브 2차전직 LOBBY_WAIT 모션
#ifdef SERV_EVE_BATTLE_SERAPH
				case CX2Unit::UC_EVE_BATTLE_SERAPH:				
					PlayAnim( L"BS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif

					//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_FURY_GUARDIAN:				
					PlayAnim( L"CFG_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:				
					PlayAnim( L"CSG_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif	CHUNG_FIRST_CLASS_CHANGE
					//}} kimhc // 2011.1.3 // 청 1차 전직

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
					PlayAnim( L"CHG_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	
					break;
#endif

					//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_IRON_PALADIN:				
					PlayAnim( L"CIP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_DEADLY_CHASER:				
					PlayAnim( L"CDC_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif
					//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
				case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:		
					PlayAnim( L"CTT_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	
					break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
				case CX2Unit::UC_ARA_LITTLE_HSIEN:
					PlayAnim( L"ALH_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_SAKRA_DEVANAM:
					PlayAnim( L"ASD_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef NEW_CHARACTER_EL
				case CX2Unit::UC_ELESIS_SABER_KNIGHT:
					PlayAnim( L"SSK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
					PlayAnim( L"SPK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				case CX2Unit::UC_ARA_LITTLE_DEVIL:
					PlayAnim( L"ALD_lobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_YAMA_RAJA:
					PlayAnim( L"AYR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				case CX2Unit::UC_ELESIS_GRAND_MASTER:
					PlayAnim( L"SGM_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ELESIS_BLAZING_HEART:
					PlayAnim( L"SBH_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UC_ADD_PSYCHIC_TRACER:
					PlayAnim( L"APT_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				case CX2Unit::UC_ADD_LUNATIC_PSYKER:
					PlayAnim( L"ALP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ADD_LUNATIC_PSYKER

				default:
					{
						PlayAnim( L"LobbyWait1", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					} break;
				}
			} break;

		case CX2UnitViewerUI::UVVMT_WAIT2:
			{
				switch( m_pUnit->GetUnitData().m_UnitClass ) 
				{
				case CX2Unit::UC_ELSWORD_KNIGHT:
					{
						PlayAnim( L"Knight_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;

				case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
					{
						PlayAnim( L"MagicKnight_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

#ifdef ELSWORD_SHEATH_KNIGHT
				case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
					{
						PlayAnim( L"ETK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;
#endif ELSWORD_SHEATH_KNIGHT

				case CX2Unit::UC_LIRE_COMBAT_RANGER:
					{
						PlayAnim( L"CR_LobbyWait2", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;


				case CX2Unit::UC_LIRE_SNIPING_RANGER:
					{
						PlayAnim( L"SR_LobbyWait2", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

#ifdef SERV_TRAPPING_RANGER_TEST
				case CX2Unit::UC_LIRE_TRAPPING_RANGER:
					{
						PlayAnim( L"RTR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;
#endif SERV_TRAPPING_RANGER_TEST

				case CX2Unit::UC_RAVEN_SOUL_TAKER:
					{
						PlayAnim( L"ST_LobbyWait2", CKTDGXSkinAnim::XAP_ONE_WAIT, true );

					} break;
				case CX2Unit::UC_RAVEN_OVER_TAKER:
					{
						PlayAnim( L"OT_LobbyWait2", CKTDGXSkinAnim::XAP_ONE_WAIT, true );

					} break;

				case CX2Unit::UC_EVE_EXOTIC_GEAR:
					PlayAnim( L"EG_LobbyWait2", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;

				case CX2Unit::UC_EVE_ARCHITECTURE:
					PlayAnim( L"AT_LobbyWait2", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;

#ifdef EVE_ELECTRA
				case CX2Unit::UC_EVE_ELECTRA:				
					PlayAnim( L"PR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;
#endif EVE_ELECTRA

				case CX2Unit::UC_ELSWORD_LORD_KNIGHT:			PlayAnim( L"LK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
				case CX2Unit::UC_ELSWORD_RUNE_SLAYER:			PlayAnim( L"RS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
				case CX2Unit::UC_ELSWORD_INFINITY_SWORD:		PlayAnim( L"EIS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif

				case CX2Unit::UC_LIRE_WIND_SNEAKER:				PlayAnim( L"LWS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
				case CX2Unit::UC_LIRE_GRAND_ARCHER:				PlayAnim( L"LGA_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#ifdef SERV_RENA_NIGHT_WATCHER
				case CX2Unit::UC_LIRE_NIGHT_WATCHER:			PlayAnim( L"RNW_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif SERV_RENA_NIGHT_WATCHER

				case CX2Unit::UC_ARME_ELEMENTAL_MASTER:			PlayAnim( L"EM_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
				case CX2Unit::UC_ARME_VOID_PRINCESS:			PlayAnim( L"VP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
#ifdef SERV_ARME_DIMENSION_WITCH
				case CX2Unit::UC_ARME_DIMENSION_WITCH:			PlayAnim( L"DW_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
				case CX2Unit::UC_RAVEN_BLADE_MASTER:
					PlayAnim( L"RBM_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_RAVEN_RECKLESS_FIST:
					PlayAnim( L"RRF_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:		PlayAnim( L"RVC_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif SERV_RAVEN_VETERAN_COMMANDER

					//{{ kimhc // 2010.1.22 // 이브 2차전직 LOBBY_WAIT2 모션
#ifdef EVE_SECOND_CLASS_CHANGE
				case CX2Unit::UC_EVE_CODE_NEMESIS:				
					PlayAnim( L"NS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_EVE_CODE_EMPRESS:				
					PlayAnim( L"EP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif EVE_SECOND_CLASS_CHANGE
					//}} kimhc // 2010.1.22 // 이브 2차전직 LOBBY_WAIT2 모션
#ifdef SERV_EVE_BATTLE_SERAPH
				case CX2Unit::UC_EVE_BATTLE_SERAPH:				
					PlayAnim( L"BS_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif

					//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_FURY_GUARDIAN:				
					PlayAnim( L"CFG_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:				
					PlayAnim( L"CSG_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif	CHUNG_FIRST_CLASS_CHANGE
					//}} kimhc // 2011.1.3 // 청 1차 전직

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
					PlayAnim( L"CHG_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	
					break;
#endif

					//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_IRON_PALADIN:				
					PlayAnim( L"CIP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_DEADLY_CHASER:				
					PlayAnim( L"CDC_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif
					//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
				case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:		
					PlayAnim( L"CTT_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	
					break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
				case CX2Unit::UC_ARA_LITTLE_HSIEN:
					PlayAnim( L"ALH_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_SAKRA_DEVANAM:
					PlayAnim( L"ASD_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef NEW_CHARACTER_EL
				case CX2Unit::UC_ELESIS_SABER_KNIGHT:
					PlayAnim( L"SSK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
					PlayAnim( L"SPK_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				case CX2Unit::UC_ARA_LITTLE_DEVIL:
					PlayAnim( L"ALD_lobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_YAMA_RAJA:
					PlayAnim( L"AYR_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				case CX2Unit::UC_ELESIS_GRAND_MASTER:
					PlayAnim( L"SGM_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ELESIS_BLAZING_HEART:
					PlayAnim( L"SBH_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UC_ADD_PSYCHIC_TRACER:
					PlayAnim( L"APT_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				case CX2Unit::UC_ADD_LUNATIC_PSYKER:
					PlayAnim( L"ALP_LobbyWait", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ADD_LUNATIC_PSYKER

				default:
					{
						PlayAnim( L"LobbyWait2", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					} break;
				}
			} break;
		case CX2UnitViewerUI::UVVMT_DOWN_LANDING:
			{
				switch( m_pUnit->GetUnitData().m_UnitClass ) 
				{
				case CX2Unit::UC_ELSWORD_KNIGHT:
					{
						PlayAnim( L"Knight_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}
					break;

				case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
					{
						PlayAnim( L"MagicKnight_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}	
					break;

#ifdef ELSWORD_SHEATH_KNIGHT
				case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
					{
						PlayAnim( L"ETK_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}
					break;
#endif ELSWORD_SHEATH_KNIGHT

				case CX2Unit::UC_LIRE_COMBAT_RANGER:
					{
						PlayAnim( L"CR_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}	
					break;

				case CX2Unit::UC_LIRE_SNIPING_RANGER:
					{
						PlayAnim( L"SR_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}	
					break;

#ifdef	SERV_TRAPPING_RANGER_TEST
				case CX2Unit::UC_LIRE_TRAPPING_RANGER:
					{
						PlayAnim( L"RTR_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}	
					break;
#endif	SERV_TRAPPING_RANGER_TEST

				case CX2Unit::UC_ARME_HIGH_MAGICIAN:
					{
						PlayAnim( L"HM_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}
					break;

				case CX2Unit::UC_ARME_DARK_MAGICIAN:
					{
						PlayAnim( L"DM_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}
					break;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
				case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
					{
						PlayAnim( L"BM_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}
					break;
#endif

				case CX2Unit::UC_RAVEN_SOUL_TAKER:
					{
						PlayAnim( L"ST_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	

					} break;
				case CX2Unit::UC_RAVEN_OVER_TAKER:
					{
						PlayAnim( L"OT_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	

					} break;

#ifdef RAVEN_WEAPON_TAKER
				case CX2Unit::UC_RAVEN_WEAPON_TAKER:
					{
						PlayAnim( L"RWT_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}
					break;
#endif RAVEN_WEAPON_TAKER

				case CX2Unit::UC_EVE_EXOTIC_GEAR:
					PlayAnim( L"EG_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT ); break;

				case CX2Unit::UC_EVE_ARCHITECTURE:
					PlayAnim( L"AT_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT ); break;

#ifdef EVE_ELECTRA
				case CX2Unit::UC_EVE_ELECTRA:
					PlayAnim( L"PR_LobbyDownlanding", CKTDGXSkinAnim::XAP_ONE_WAIT ); break;
#endif EVE_ELECTRA
				case CX2Unit::UC_ELSWORD_LORD_KNIGHT:			PlayAnim( L"LK_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );		break;
				case CX2Unit::UC_ELSWORD_RUNE_SLAYER:			PlayAnim( L"RS_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );		break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
				case CX2Unit::UC_ELSWORD_INFINITY_SWORD:		PlayAnim( L"EIS_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );		break;
#endif

				case CX2Unit::UC_LIRE_WIND_SNEAKER:				PlayAnim( L"LWS_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );		break;
				case CX2Unit::UC_LIRE_GRAND_ARCHER:				PlayAnim( L"LGA_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );		break;
#ifdef SERV_RENA_NIGHT_WATCHER
				case CX2Unit::UC_LIRE_NIGHT_WATCHER:			PlayAnim( L"RNW_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );		break;
#endif SERV_RENA_NIGHT_WATCHER

				case CX2Unit::UC_ARME_ELEMENTAL_MASTER:			PlayAnim( L"EM_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );	break;
				case CX2Unit::UC_ARME_VOID_PRINCESS:			PlayAnim( L"VP_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );	break;
#ifdef SERV_ARME_DIMENSION_WITCH
				case CX2Unit::UC_ARME_DIMENSION_WITCH:			PlayAnim( L"DW_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );	break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
				case CX2Unit::UC_RAVEN_BLADE_MASTER:
					PlayAnim( L"RBM_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_RAVEN_RECKLESS_FIST:
					PlayAnim( L"RRF_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:		PlayAnim( L"RVC_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );		break;
#endif SERV_RAVEN_VETERAN_COMMANDER

					//{{ kimhc // 2010.1.22 // 이브 2차전직 LobbyDownLanding 모션
#ifdef EVE_SECOND_CLASS_CHANGE
				case CX2Unit::UC_EVE_CODE_NEMESIS:				
					PlayAnim( L"NS_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_EVE_CODE_EMPRESS:				
					PlayAnim( L"EP_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif EVE_SECOND_CLASS_CHANGE
					//}} kimhc // 2010.1.22 // 이브 2차전직 LOBBY_WAIT2 모션
#ifdef SERV_EVE_BATTLE_SERAPH
				case CX2Unit::UC_EVE_BATTLE_SERAPH:				
					PlayAnim( L"BS_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif

					//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_FURY_GUARDIAN:				
					PlayAnim( L"CFG_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:				
					PlayAnim( L"CSG_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif	CHUNG_FIRST_CLASS_CHANGE
					//}} kimhc // 2011.1.3 // 청 1차 전직

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
					PlayAnim( L"CHG_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif

					//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_IRON_PALADIN:				
					PlayAnim( L"CIP_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_DEADLY_CHASER:				
					PlayAnim( L"CDC_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif
					//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
				case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:		
					PlayAnim( L"CTT_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	
					break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
				case CX2Unit::UC_ARA_LITTLE_HSIEN:
					PlayAnim( L"ALH_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_SAKRA_DEVANAM:
					PlayAnim( L"ASD_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef NEW_CHARACTER_EL
				case CX2Unit::UC_ELESIS_SABER_KNIGHT:
					PlayAnim( L"SSK_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
					PlayAnim( L"SPK_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				case CX2Unit::UC_ARA_LITTLE_DEVIL:
					PlayAnim( L"ALD_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_YAMA_RAJA:
					PlayAnim( L"AYR_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
				case CX2Unit::UC_ELESIS_GRAND_MASTER:
					PlayAnim( L"SGM_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ELESIS_BLAZING_HEART:
					PlayAnim( L"SBH_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UC_ADD_PSYCHIC_TRACER:
					PlayAnim( L"APT_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				case CX2Unit::UC_ADD_LUNATIC_PSYKER:
					PlayAnim( L"ALP_LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ADD_LUNATIC_PSYKER

				default:
					{
						PlayAnim( L"LobbyDownLanding", CKTDGXSkinAnim::XAP_ONE_WAIT );
					} break;
				}
			} break;



		case CX2UnitViewerUI::UVUMT_READY:
			{
				switch( m_pUnit->GetUnitData().m_UnitClass ) 
				{
				case CX2Unit::UC_ELSWORD_KNIGHT:
					{
						PlayAnim( L"Knight_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;

				case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
					{
						PlayAnim( L"MagicKnight_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

#ifdef ELSWORD_SHEATH_KNIGHT
				case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
					{
						PlayAnim( L"ETK_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;
#endif ELSWORD_SHEATH_KNIGHT

				case CX2Unit::UC_LIRE_COMBAT_RANGER:
					{
						PlayAnim( L"CR_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

				case CX2Unit::UC_LIRE_SNIPING_RANGER:
					{
						PlayAnim( L"SR_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;

#ifdef	SERV_TRAPPING_RANGER_TEST
				case CX2Unit::UC_LIRE_TRAPPING_RANGER:
					{
						PlayAnim( L"RTR_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}	
					break;
#endif	SERV_TRAPPING_RANGER_TEST

				case CX2Unit::UC_ARME_HIGH_MAGICIAN:
					{
						PlayAnim( L"HM_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;

				case CX2Unit::UC_ARME_DARK_MAGICIAN:
					{
						PlayAnim( L"DM_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
				case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
					{
						PlayAnim( L"BM_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					break;
#endif

				case CX2Unit::UC_RAVEN_SOUL_TAKER:
					{
						PlayAnim( L"ST_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	

					} break;
				case CX2Unit::UC_RAVEN_OVER_TAKER:
					{
						PlayAnim( L"OT_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );	

					} break;

#ifdef RAVEN_WEAPON_TAKER
				case CX2Unit::UC_RAVEN_WEAPON_TAKER:
					{
						PlayAnim( L"RWT_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT );
					}
					break;
#endif RAVEN_WEAPON_TAKER

				case CX2Unit::UC_EVE_EXOTIC_GEAR:
					PlayAnim( L"EG_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;

				case CX2Unit::UC_EVE_ARCHITECTURE:
					PlayAnim( L"AT_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;
#ifdef EVE_ELECTRA
				case CX2Unit::UC_EVE_ELECTRA:
					PlayAnim( L"PR_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); break;
#endif EVE_ELECTRA

				case CX2Unit::UC_ELSWORD_LORD_KNIGHT:			PlayAnim( L"LK_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
				case CX2Unit::UC_ELSWORD_RUNE_SLAYER:			PlayAnim( L"RS_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
				case CX2Unit::UC_ELSWORD_INFINITY_SWORD:		PlayAnim( L"EIS_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif

				case CX2Unit::UC_LIRE_WIND_SNEAKER:				PlayAnim( L"LWS_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
				case CX2Unit::UC_LIRE_GRAND_ARCHER:				PlayAnim( L"LGA_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#ifdef SERV_RENA_NIGHT_WATCHER
				case CX2Unit::UC_LIRE_NIGHT_WATCHER:			PlayAnim( L"RNW_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif SERV_RENA_NIGHT_WATCHER

				case CX2Unit::UC_ARME_ELEMENTAL_MASTER:			PlayAnim( L"EM_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
				case CX2Unit::UC_ARME_VOID_PRINCESS:			PlayAnim( L"VP_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#ifdef SERV_ARME_DIMENSION_WITCH
				case CX2Unit::UC_ARME_DIMENSION_WITCH:			PlayAnim( L"DW_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
				case CX2Unit::UC_RAVEN_BLADE_MASTER:
					PlayAnim( L"RBM_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_RAVEN_RECKLESS_FIST:
					PlayAnim( L"RRF_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
				case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:		PlayAnim( L"RVC_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		break;
#endif SERV_RAVEN_VETERAN_COMMANDER

					//{{ kimhc // 2010.1.22 // 이브 2차전직 LobbyReadyAction 모션
#ifdef EVE_SECOND_CLASS_CHANGE
				case CX2Unit::UC_EVE_CODE_NEMESIS:				
					PlayAnim( L"NS_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_EVE_CODE_EMPRESS:				
					PlayAnim( L"EP_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif EVE_SECOND_CLASS_CHANGE
					//}} kimhc // 2010.1.22 // 이브 2차전직 LobbyReadyAction 모션
#ifdef SERV_EVE_BATTLE_SERAPH
				case CX2Unit::UC_EVE_BATTLE_SERAPH:				
					PlayAnim( L"BS_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif

					//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_FURY_GUARDIAN:				
					PlayAnim( L"CFG_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:				
					PlayAnim( L"CSG_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif	CHUNG_FIRST_CLASS_CHANGE
					//}} kimhc // 2011.1.3 // 청 1차 전직

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
				case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
					PlayAnim( L"CHG_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif

					//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
				case CX2Unit::UC_CHUNG_IRON_PALADIN:				
					PlayAnim( L"CIP_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_CHUNG_DEADLY_CHASER:				
					PlayAnim( L"CDC_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true ); 		
					break;
#endif
					//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
				case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
					PlayAnim( L"CTT_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
				case CX2Unit::UC_ARA_LITTLE_HSIEN:
					PlayAnim( L"ALH_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_SAKRA_DEVANAM:
					PlayAnim( L"ASD_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //ARA_CHANGE_CLASS_FIRST
#ifdef NEW_CHARACTER_EL
				case CX2Unit::UC_ELESIS_SABER_KNIGHT:
					PlayAnim( L"SSK_LobbyReadyaction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
					PlayAnim( L"SPK_LobbyReadyaction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
				case CX2Unit::UC_ARA_LITTLE_DEVIL:
					PlayAnim( L"ALD_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
				case CX2Unit::UC_ARA_YAMA_RAJA:
					PlayAnim( L"AYR_LobbyReadyAtcion", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND


#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
				case CX2Unit::UC_ELESIS_GRAND_MASTER:
					PlayAnim( L"SGM_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break; 
				case CX2Unit::UC_ELESIS_BLAZING_HEART:
					PlayAnim( L"SBH_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
				case CX2Unit::UC_ADD_PSYCHIC_TRACER:
					PlayAnim( L"APT_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
				case CX2Unit::UC_ADD_LUNATIC_PSYKER:
					PlayAnim( L"ALP_LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					break;
#endif //SERV_ADD_LUNATIC_PSYKER

				default:
					{
						PlayAnim( L"LobbyReadyAction", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					} break;
				}
			} break;
		case CX2UnitViewerUI::UVUMT_LEVEL_UP:
			{
				// 레벨업 포즈는 공통으로 
				PlayAnim( L"LobbyLevelUp", CKTDGXSkinAnim::XAP_ONE_WAIT, true );


				if( NULL != m_pUnit &&
					CX2Unit::UT_EVE == m_pUnit->GetType() &&
					NULL != m_pSound_Eve_LevelUp )
				{
					m_pSound_Eve_LevelUp->Play( false, false );
				}
			} break;
#ifdef SERV_UNIT_WAIT_DELETE //2012.03.07 lygan_조성욱 // 캐릭터별 삭제 대기시에 애니메이션 설정
		case CX2UnitViewerUI::UVUMT_WAIT_DELETE:
			{
				PlayAnim( L"LobbyLose", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
			}
			break;
#endif //SERV_UNIT_WAIT_DELETE
		default:
			break;
		}
	}
}

int CX2UnitViewerUI::GetEnchantExtraDamageType()
{
	CX2DamageManager::EXTRA_DAMAGE_TYPE finalExtraDamageType = CX2DamageManager::EDT_NONE;

	if ( m_pUnit == NULL )
		return finalExtraDamageType;



	CX2Item* pItem = m_pUnit->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );
	if ( pItem != NULL )
	{

		//{{ kimhc // 실시간 엘소드 중 실시간 장비교체, 실시간 내구도 감소
#ifdef REAL_TIME_ELSWORD
		if ( g_pData->GetMyUser()->GetSelectUnit() != NULL &&
			g_pData->GetMyUser()->GetSelectUnit()->GetUID() == m_pUnit->GetUID() &&
			pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && pItem->GetItemData().m_Endurance <= 0  )
		{
			return finalExtraDamageType;
		}
#endif REAL_TIME_ELSWORD
		//}} kimhc // 실시간 엘소드 중 실시간 장비교체, 실시간 내구도 감소
		
		
		finalExtraDamageType = g_pData->GetEnchantItem()->GetExtraDamageType( pItem->GetItemData().m_EnchantedAttribute );
	}

	return finalExtraDamageType;
}

//{{ robobeg : 2008-10-18

void CX2UnitViewerUI::ReplaceEmptyToBasic( CX2Unit::EQIP_POSITION eqipPos, int itemID )
{
	////헤어
	//if( m_ViewEqipPos[eqipPos] == false
	//	&& itemID != 0 )
	//{
	//	CX2Item::ItemData kItemData;
	//	kItemData.m_ItemID				= itemID;
	//	//kItemData.m_OwnerUnitUID		= m_pUnit->GetUID();
	//	//kItemData.m_OwnerUserUID		= m_pUnit->GetOwnerUserUID();
	//	kItemData.m_Endurance			= 1;

	//	CX2Item* pX2Item				= new CX2Item( kItemData, m_pUnit );

	//	int enchantLevel = pX2Item->GetItemData().m_EnchantLevel;
	//	/*
	//	if( m_pUnit != NULL && pX2Item != NULL && pX2Item->GetItemTemplet() != NULL )
	//	{
	//		if( pX2Item->GetItemTemplet()->GetFashion() == true )
	//		{
	//			CX2Item* pTempItem = m_pUnit->GetInventory().GetEquippingItemByEquipPos( pX2Item->GetItemTemplet()->GetEqipPosition(), false );
	//			if( pTempItem != NULL )
	//			{
	//				enchantLevel = pTempItem->GetItemData().m_EnchantLevel;
	//			}				 
	//		}
	//		else
	//		{
	//			enchantLevel = pX2Item->GetItemData().m_EnchantLevel;
	//		}
	//	}
	//	*/

	//	CX2Eqip* pX2Eqip = new CX2Eqip( pX2Item, m_pXSkinAnim, enchantLevel, true );
	//	m_ViewEqipPos[eqipPos] = true;
	//	m_ViewEqipList.push_back( pX2Eqip );
	//}

    ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
    if ( !(eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END ) )
        return;

    if ( itemID != 0 && !IsItemEquippedAt( eqipPos ) )
    {
        AddEquipByTID( itemID, true );
    }//if
}

//}} robobeg : 2008-10-18

//{{ robobeg : 2008-10-17
//void CX2UnitViewerUI::CheckEmptyEqipPosition()
//{
//	ZeroMemory( m_ViewEqipPos, sizeof(bool) * CX2Unit::EP_END );
//	for( int i =0; i < (int)m_ViewEqipList.size(); i++ )
//	{
//		CX2Eqip* pEqip = m_ViewEqipList[i];
//		if( pEqip != NULL && pEqip->GetItem() != NULL && pEqip->GetItem()->GetItemTemplet() != NULL )
//		{
//			m_ViewEqipPos[pEqip->GetItem()->GetItemTemplet()->GetEqipPosition()] = true;
//		}
//	}
//}
//}} robobeg : 2008-10-17

void CX2UnitViewerUI::InitEnchantWeaponEffectForSword()
{
	// attribute prefix 에 대한 이펙트 생성

	switch( GetEnchantExtraDamageType() )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Elsword_Property01_Fire", 
				L"Weapon_Elsword_Property01_Fire2",
				L"Weapon_Elsword_Property01_Fire2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Elsword_Property01_M_Fire", 
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
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Elsword_Property01_Ice", 
				L"Weapon_Elsword_Property01_Ice2",
				L"Weapon_Elsword_Property01_Ice2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Elsword_Property01_M_Ice", 
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
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Elsword_Property01_Poison", 
				L"Weapon_Elsword_Property01_Poison2",
				L"Weapon_Elsword_Property01_Poison2" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Elsword_Property01_M_Poison", 
				L"Weapon_Elsword_Property01_M_Poison2",
				L"Weapon_Elsword_Property01_M_Poison2" );
		} break;
	}
	
}


void CX2UnitViewerUI::InitEnchantWeaponEffectForBow()
{
	// attribute prefix 에 대한 이펙트 생성

	switch( GetEnchantExtraDamageType() )
	{
	case CX2DamageManager::EDT_ENCHANT_BLAZE:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Lire_Property01_Fire" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_BLAZE_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_MASTER:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Lire_Property01_M_Fire" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Lire_Property01_Ice" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_FROZEN_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_MASTER:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Lire_Property01_M_Ice" );
		} break;


	case CX2DamageManager::EDT_ENCHANT_POISON:
	case CX2DamageManager::EDT_ENCHANT_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SNATCH:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Lire_Property01_Poison" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_POISON_MASTER:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_MASTER:
		{
			CreateEnchantedWeaponParticleAtWeapon( L"Weapon_Lire_Property01_M_Poison" );
		} break; 
	}
	
}





void CX2UnitViewerUI::CreateEnchantedWeaponParticleAtWeapon( const WCHAR* pName1, const WCHAR* pName2 /*= NULL*/, const WCHAR* pName3 /*= NULL*/ )
{
	if( NULL != pName1 )
	{
		if ( m_hSeqWeapon_Fire == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqWeapon_Fire = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  pName1, 0,0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}


	if( NULL != pName2 )
	{
		if ( m_hSeqWeapon_Fire2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqWeapon_Fire2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  pName2, 0,0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire2 );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}

	if( NULL != pName3 )
	{
		if ( m_hSeqWeapon_Fire3 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqWeapon_Fire3 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  pName3, 0,0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire3 );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}
}




void CX2UnitViewerUI::CreateEnchantedWeaponParticleAtHand( const WCHAR* pName1, const WCHAR* pName2 /*= NULL*/, const WCHAR* pName3 /*= NULL*/, const WCHAR* pName4 /*= NULL */ )
{
	if( NULL != pName1 )
	{
		if ( m_hSeqWeapon_Wind == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqWeapon_Wind = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  pName1, 0,0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}


	if( NULL != pName2 )
	{
		if ( m_hSeqWeapon_Wind2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqWeapon_Wind2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  pName2, 0,0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind2 );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}

	if( NULL != pName3 )
	{
		if ( m_hSeqWeapon_Wind3 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqWeapon_Wind3 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  pName3, 0,0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind3 );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}


	if( NULL != pName4 )
	{
		if ( m_hSeqWeapon_Wind4 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqWeapon_Wind4 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  pName4, 0,0,0 );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind4 );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( 0, 0 );
			}
		}
	}
}



void CX2UnitViewerUI::InitEnchantedWeaponEffectAtHand()
{
	// attribute postfix 에 대한 이펙트 생성

	switch( GetEnchantExtraDamageType() )
	{
	case CX2DamageManager::EDT_ENCHANT_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_PIERCING:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_Wind01",
				L"Weapon_Elsword_Property03_Wind01" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_MASTER_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_PIERCING:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_PIERCING:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_M_Wind01",
				L"Weapon_Elsword_Property03_Wind01",
				L"Weapon_Elsword_Property04_Wind01" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_DOUBLE_MASTER_PIERCING:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_M_Wind01",
				L"Weapon_Elsword_Property03_Wind01",
				L"Weapon_Elsword_Property04_Wind01",
				L"Triple_Weapon_Property_Wind01" );
		} break;



	case CX2DamageManager::EDT_ENCHANT_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SHOCK:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_Light01",
				L"Weapon_Elsword_Property03_Light01" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_MASTER_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SHOCK:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SHOCK:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_M_Light01",
				L"Weapon_Elsword_Property03_Light01",
				L"Weapon_Elsword_Property04_Light01" );

		} break;


	case CX2DamageManager::EDT_ENCHANT_DOUBLE_MASTER_SHOCK:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_M_Light01",
				L"Weapon_Elsword_Property03_Light01",
				L"Weapon_Elsword_Property04_Light01",
				L"Triple_Weapon_Property_Light01" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_BLAZE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_POISON_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_DOUBLE_FROZEN_SNATCH:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_Curse01",
				L"Weapon_Elsword_Property03_Curse01" );
		} break;

	case CX2DamageManager::EDT_ENCHANT_MASTER_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_BLAZE_DOUBLE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_FROZEN_DOUBLE_SNATCH:
	case CX2DamageManager::EDT_ENCHANT_POISON_DOUBLE_SNATCH:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_M_Curse01",
				L"Weapon_Elsword_Property03_Curse01",
				L"Weapon_Elsword_Property04_Curse01" );
		} break;


	case CX2DamageManager::EDT_ENCHANT_DOUBLE_MASTER_SNATCH:
		{
			CreateEnchantedWeaponParticleAtHand( L"Weapon_Elsword_Property02_M_Curse01",
				L"Weapon_Elsword_Property03_Curse01",
				L"Weapon_Elsword_Property04_Curse01",
				L"Triple_Weapon_Property_Curse01" );
		} break;
	}
}


void CX2UnitViewerUI::DeleteEnchantWeaponEffect()
{
	if( NULL != g_pData &&
		NULL != g_pData->GetUIMajorParticle() )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqWeapon_Fire );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqWeapon_Fire2 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqWeapon_Fire3 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqWeapon_Wind );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqWeapon_Wind2 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqWeapon_Wind3 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqWeapon_Wind4 );
	}
}




void CX2UnitViewerUI::ProcessEnchantedWeaponEffectAtHand( CX2Unit::UNIT_TYPE unitType )
{
	KTDXPROFILE();


	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqWeapon_Wind )	// 이 파티클이 없으면 손에 붙는 파티클은 없다고 보면 됨
		return;

	if( m_pXSkinAnim == NULL )
		return;

	D3DXVECTOR3 vHandPos( 0, 0, 0 );
	float aEmitRate[4] = { 0.f, };
	switch( unitType )
	{
	case CX2Unit::UT_ELSWORD:
	case CX2Unit::UT_ARME:
	case CX2Unit::UT_RAVEN:
		//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
#endif	NEW_CHARACTER_CHUNG
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS:
#endif // NEW_CHARACTER_EL
		//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG
		{
			vHandPos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			aEmitRate[0] = 10.f;
			aEmitRate[1] = 30.f;
			aEmitRate[2] = 60.f;
			aEmitRate[3] = 25.f;
		} break;

	case CX2Unit::UT_LIRE:
		{
			vHandPos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			aEmitRate[0] = 15.f;
			aEmitRate[1] = 30.f;
			aEmitRate[2] = 60.f;
			aEmitRate[3] = 25.f;
		} break;

	case CX2Unit::UT_EVE:
		{
			vHandPos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );				aEmitRate[0] = 10.f;
			aEmitRate[1] = 10.f;
			aEmitRate[2] = 10.f;
			aEmitRate[3] = 25.f;
		} break;

	case CX2Unit::UT_ARA:
		{
			vHandPos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy3_Weapon" );
			aEmitRate[0] = 15.f;
			aEmitRate[1] = 30.f;
			aEmitRate[2] = 60.f;
			aEmitRate[3] = 25.f;
		} break;

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CX2Unit::UT_ADD:
		{
			vHandPos = m_pXSkinAnim->GetCloneFramePosition( L"Weapon02" );
			aEmitRate[0] = 15.f;
			aEmitRate[1] = 30.f;
			aEmitRate[2] = 60.f;
			aEmitRate[3] = 25.f;
		} break;
#endif //SERV_9TH_NEW_CHARACTER
	}


	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire1 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind );
	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire2 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind2 );
	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire3 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind3 );
	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire4 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind4 );

#ifdef RIDING_SYSTEM
	#ifdef FIX_UPGRADE_WEAPON_PARTICLE
		if ( (true == GetShowObject() &&	
			 true == CanNotShowEnchantedWeaponEffectToRiding() )||
			 UVS_CHARINFO == m_eUnitViewerState )
	#else
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( true == GetShowObject() &&	true == CanNotShowEnchantedWeaponEffectToRiding() || UVS_FIELD != m_eUnitViewerState )
	#endif // FIX_UPGRADE_WEAPON_PARTICLE
#else //RIDING_SYSTEM
	if( true == GetShowObject() )
#endif //RIDING_SYSTEM
	{
		if ( pSeqWeaponFire1 != NULL )
		{	
			pSeqWeaponFire1->SetPosition( vHandPos );
			pSeqWeaponFire1->SetEmitRate( aEmitRate[0], aEmitRate[0] );
		}

		if ( pSeqWeaponFire2 != NULL )
		{
			pSeqWeaponFire2->SetPosition( vHandPos );
			pSeqWeaponFire2->SetEmitRate( aEmitRate[1], aEmitRate[1] );
		}

		if ( pSeqWeaponFire3 != NULL )
		{
			pSeqWeaponFire3->SetPosition( vHandPos );
			pSeqWeaponFire3->SetEmitRate( aEmitRate[2], aEmitRate[2] );
		}

		if ( pSeqWeaponFire4 != NULL )
		{
			pSeqWeaponFire4->SetPosition( vHandPos );
			pSeqWeaponFire4->SetEmitRate( aEmitRate[3], aEmitRate[3] );
		}
	}
	else
	{
		if ( pSeqWeaponFire1 != NULL )
		{
			pSeqWeaponFire1->ClearAllParticle();
			pSeqWeaponFire1->SetEmitRate( 0, 0 );
		}

		if ( pSeqWeaponFire2 != NULL )
		{
			pSeqWeaponFire2->ClearAllParticle();
			pSeqWeaponFire2->SetEmitRate( 0, 0 );
		}


		if ( pSeqWeaponFire3 != NULL )
		{
			pSeqWeaponFire3->ClearAllParticle();
			pSeqWeaponFire3->SetEmitRate( 0, 0 );
		}


		if ( pSeqWeaponFire4 != NULL )
		{
			pSeqWeaponFire4->ClearAllParticle();
			pSeqWeaponFire4->SetEmitRate( 0, 0 );
		}

	}

}



void CX2UnitViewerUI::ProcessEnchantWeaponEffectForEve()
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqWeapon_Fire )
		return;

	if( m_pXSkinAnim == NULL )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire1 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
	if ( pSeqWeaponFire1 != NULL )
	{
#ifdef RIDING_SYSTEM
	#ifdef FIX_UPGRADE_WEAPON_PARTICLE
			if ( (true == GetShowObject() &&	
				 true == CanNotShowEnchantedWeaponEffectToRiding()) ||
				 UVS_CHARINFO == m_eUnitViewerState )
	#else
			/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
			if ( true == GetShowObject() &&	true == CanNotShowEnchantedWeaponEffectToRiding() || UVS_FIELD != m_eUnitViewerState )
	#endif // FIX_UPGRADE_WEAPON_PARTICLE
#else //RIDING_SYSTEM
		if( GetShowObject() == true )
#endif //RIDING_SYSTEM
		{
			D3DXVECTOR3 vBonePos1 = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
			pSeqWeaponFire1->SetPosition( vBonePos1 );

			pSeqWeaponFire1->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeqWeaponFire1->SetEmitRate( 0, 0 );
		}
	}
}

#ifdef ARA_CHARACTER_BASE
void CX2UnitViewerUI::ProcessEnchantWeaponEffectForAra()
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqWeapon_Fire )
		return;

	if( m_pXSkinAnim == NULL )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire1 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
	if ( pSeqWeaponFire1 != NULL )
	{
#ifdef RIDING_SYSTEM
	#ifdef FIX_UPGRADE_WEAPON_PARTICLE
			if ( (true == GetShowObject() && 
				 true == CanNotShowEnchantedWeaponEffectToRiding())||
				 UVS_CHARINFO == m_eUnitViewerState )
	#else
			/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
			if ( true == GetShowObject() && true == CanNotShowEnchantedWeaponEffectToRiding() || UVS_FIELD != m_eUnitViewerState )
	#endif // FIX_UPGRADE_WEAPON_PARTICLE
#else //RIDING_SYSTEM
		if( GetShowObject() == true )
#endif //RIDING_SYSTEM
		{
			D3DXVECTOR3 weaponPos = D3DXVECTOR3(0,0,0);
			GetFramePos( &weaponPos, m_pFrame_WEAPON_BONE[0] );

			pSeqWeaponFire1->SetPosition( weaponPos );

			pSeqWeaponFire1->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeqWeaponFire1->SetEmitRate( 0, 0 );
		}
	}
}
#endif


#ifdef SERV_9TH_NEW_CHARACTER // 김태환
void CX2UnitViewerUI::ProcessEnchantWeaponEffectForAdd()
{
	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqWeapon_Fire )
		return;

	if( m_pXSkinAnim == NULL )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire1 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
	if ( pSeqWeaponFire1 != NULL )
	{
#ifdef RIDING_SYSTEM
#ifdef FIX_UPGRADE_WEAPON_PARTICLE
		if ( (true == GetShowObject() && 
			true == CanNotShowEnchantedWeaponEffectToRiding())||
			UVS_CHARINFO == m_eUnitViewerState )
#else
		/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
		if ( true == GetShowObject() && true == CanNotShowEnchantedWeaponEffectToRiding() || UVS_FIELD != m_eUnitViewerState )
#endif // FIX_UPGRADE_WEAPON_PARTICLE
#else //RIDING_SYSTEM
		if( GetShowObject() == true )
#endif //RIDING_SYSTEM
		{
			D3DXVECTOR3 weaponPos = m_pXSkinAnim->GetCloneFramePosition( L"Weapon05" );

			pSeqWeaponFire1->SetPosition( weaponPos );

			pSeqWeaponFire1->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeqWeaponFire1->SetEmitRate( 0, 0 );
		}
	}
}
#endif //SERV_9TH_NEW_CHARACTER


void CX2UnitViewerUI::ProcessEnchantWeaponEffectForSword()
{
	KTDXPROFILE();


	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqWeapon_Fire )
		return;

	if( NULL == m_pFrame_TRACE_START[0] || 
		NULL == m_pFrame_TRACE_END[0] )
	{
		// 백그라운드 로딩 때문에 frame 포인터를 못 찾았을 수 있다. 일정 시간 간격으로 메시가 로딩되었는지 체크를 한다
		if( m_TimerResetWeaponTraceFrame.elapsed() > 2.f )
		{
			InitWeaponTrace();
		}
		return;
	}


	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire1 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire2 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire2 );
	CKTDGParticleSystem::CParticleEventSequence* pSeqWeaponFire3 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire3 );


	if ( pSeqWeaponFire1 != NULL && 
		pSeqWeaponFire2 != NULL && 
		pSeqWeaponFire3 != NULL )
	{
#ifdef RIDING_SYSTEM
	#ifdef FIX_UPGRADE_WEAPON_PARTICLE
			if ( (true == GetShowObject() &&	
				 true == CanNotShowEnchantedWeaponEffectToRiding() ) ||
				 UVS_CHARINFO == m_eUnitViewerState )
	#else
			/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
			if ( true == GetShowObject() &&	true == CanNotShowEnchantedWeaponEffectToRiding() || UVS_FIELD != m_eUnitViewerState )
	#endif // FIX_UPGRADE_WEAPON_PARTICLE
#else //RIDING_SYSTEM
		if( GetShowObject() == true )
#endif //RIDING_SYSTEM
		{
			D3DXVECTOR3 weaponPos = D3DXVECTOR3(0,0,0);
			GetFramePos( &weaponPos, m_pFrame_TRACE_END[0] );
			D3DXVECTOR3 traceEndPos = D3DXVECTOR3(0,0,0);
			GetFramePos( &traceEndPos, m_pFrame_TRACE_START[0] );

			D3DXVECTOR3 traceCenterPos = traceEndPos - weaponPos;
			traceCenterPos.x = traceCenterPos.x/2.0f + weaponPos.x;
			traceCenterPos.y = traceCenterPos.y/2.0f + weaponPos.y;
			traceCenterPos.z = traceCenterPos.z/2.0f + weaponPos.z;

			pSeqWeaponFire1->SetPosition( traceCenterPos );
			pSeqWeaponFire2->SetPosition( weaponPos );
			pSeqWeaponFire3->SetPosition( traceEndPos );


			pSeqWeaponFire1->SetEmitRate( 10, 10 );
			pSeqWeaponFire2->SetEmitRate( 10, 10 );
			pSeqWeaponFire3->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeqWeaponFire1->SetEmitRate( 0, 0 );
			pSeqWeaponFire2->SetEmitRate( 0, 0 );
			pSeqWeaponFire3->SetEmitRate( 0, 0 );
		}
	}
}

void CX2UnitViewerUI::ProcessEnchantWeaponEffectForBow()
{
	KTDXPROFILE();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hSeqWeapon_Fire )
		return;

	if( m_pXSkinAnim == NULL )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
	if( NULL != pSeq )
	{
#ifdef RIDING_SYSTEM
	#ifdef FIX_UPGRADE_WEAPON_PARTICLE
			if ( (true == GetShowObject() &&	
				 true == CanNotShowEnchantedWeaponEffectToRiding()) || 
				 UVS_CHARINFO == m_eUnitViewerState )
	#else
			/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
			if ( true == GetShowObject() &&	true == CanNotShowEnchantedWeaponEffectToRiding() || UVS_FIELD != m_eUnitViewerState )
	#endif // FIX_UPGRADE_WEAPON_PARTICLE
#else //RIDING_SYSTEM
		if( true == GetShowObject() )
#endif //RIDING_SYSTEM
		{
			D3DXVECTOR3 weaponBonPos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
			pSeq->SetPosition( weaponBonPos );
			pSeq->SetEmitRate( 10, 10 );
		}
		else
		{
			pSeq->SetEmitRate( 0, 0 );
		}
	}
}

//{{ robobeg : 2008-10-18

// 실제 weapon trace는 사용하지 않지만, 속성 인챈트 이펙트 위치를 지정하기 위해서 사용된다.
void CX2UnitViewerUI::InitWeaponTrace()
{
	if ( m_ViewEquipFashion[ CX2Unit::EP_WEAPON_HAND ] != 0 )
	{
		ViewEquipMap::iterator iter = m_ViewEquipMap.find( m_ViewEquipFashion[ CX2Unit::EP_WEAPON_HAND ] );
		if ( iter != m_ViewEquipMap.end() )
		{
			CX2EqipPtr pX2Eqip = iter->second;
			const CX2Item::ItemTemplet* pItemTemplet = _GetSafeEquipItemTemplet( pX2Eqip );
			if ( pItemTemplet != NULL )
			{
				ASSERT( pItemTemplet->GetEqipPosition() == CX2Unit::EP_WEAPON_HAND );

				for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
				{
					if ( pX2Eqip->GetNormalAnim(i) != NULL )
					{
			//{{ kimhc // 2010.12.21 // 2010-12-23 New Character CHUNG
		#ifdef	NEW_CHARACTER_CHUNG
						wstring	wstrTraceStartName, wstrTraceEndName;
						GetWeaponTraceStartAndEndName( &wstrTraceStartName, &wstrTraceEndName );
						m_pFrame_TRACE_START[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( wstrTraceStartName.c_str() );
						m_pFrame_TRACE_END[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( wstrTraceEndName.c_str() );

		#ifdef ARA_CHARACTER_BASE
						m_pFrame_WEAPON_BONE[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( L"DummyTopPoint" );
		#endif

		#else	NEW_CHARACTER_CHUNG
						m_pFrame_TRACE_START[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( L"TRACE_START0" );
						m_pFrame_TRACE_END[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( L"TRACE_END0" );
		#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.21 //  2010-12-23 New Character CHUNG		
					}
					else
					{
						m_pFrame_TRACE_START[i]		= NULL;
						m_pFrame_TRACE_END[i]		= NULL;
#ifdef ARA_CHARACTER_BASE
						m_pFrame_WEAPON_BONE[i]		= NULL;
#endif
					}
				}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 아라의 무기는 이펙트 붙는 위치가 달라서, 따로 처리
				if( NULL != GetUnit() && NULL != pX2Eqip->GetNormalAnim(0) )
				{
					switch ( GetUnit()->GetType() )
					{
					case CX2Unit::UT_ARA:
						{
							m_pFrame_ATTACK_LINE_END0	= pX2Eqip->GetNormalAnim(0)->GetCloneFrame( L"ATTACK_LINE_END0" );
							m_pFrame_ATTACK_LINE_START0_FRONT = pX2Eqip->GetNormalAnim(0)->GetCloneFrame( L"ATTACK_LINE_START0_FRONT" );
						} break;
					}
				}
#endif ADD_UPGRADE_WEAPON_PARTICLE

			}//if
		}//if
	}//if
	else if ( m_ViewEquipNormal[ CX2Unit::EP_WEAPON_HAND ] != 0 )
	{
		ViewEquipMap::iterator iter = m_ViewEquipMap.find( m_ViewEquipNormal[ CX2Unit::EP_WEAPON_HAND ] );
		if ( iter != m_ViewEquipMap.end() )
		{
			CX2EqipPtr pX2Eqip = iter->second;
			const CX2Item::ItemTemplet* pItemTemplet = _GetSafeEquipItemTemplet( pX2Eqip );
			if ( pItemTemplet != NULL )
			{
				ASSERT( pItemTemplet->GetEqipPosition() == CX2Unit::EP_WEAPON_HAND );
				for( int i=0; i<CX2Item::MAX_MODEL_COUNT_A_ITEM; i++ )
				{
					if ( pX2Eqip->GetNormalAnim(i) != NULL )
					{
						//{{ kimhc // 2010.12.21 // 2010-12-23 New Character CHUNG
			#ifdef	NEW_CHARACTER_CHUNG
						wstring	wstrTraceStartName, wstrTraceEndName;
						GetWeaponTraceStartAndEndName( &wstrTraceStartName, &wstrTraceEndName );
						m_pFrame_TRACE_START[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( wstrTraceStartName.c_str() );
						m_pFrame_TRACE_END[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( wstrTraceEndName.c_str() );

#ifdef ARA_CHARACTER_BASE
						m_pFrame_WEAPON_BONE[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( L"DummyTopPoint" );
#endif

			#else	NEW_CHARACTER_CHUNG
						m_pFrame_TRACE_START[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( L"TRACE_START0" );
						m_pFrame_TRACE_END[i]		= pX2Eqip->GetNormalAnim(i)->GetCloneFrame( L"TRACE_END0" );
			#endif	NEW_CHARACTER_CHUNG
						//}} kimhc // 2010.12.21 //  2010-12-23 New Character CHUNG
					}
					else
					{
						m_pFrame_TRACE_START[i]		= NULL;
						m_pFrame_TRACE_END[i]		= NULL;
#ifdef ARA_CHARACTER_BASE
						m_pFrame_WEAPON_BONE[i]		= NULL;
#endif
					}
				}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 아라의 무기는 이펙트 붙는 위치가 달라서, 따로 처리
				if( NULL != GetUnit() && NULL != pX2Eqip->GetNormalAnim(0) )
				{
					switch ( GetUnit()->GetType() )
					{
					case CX2Unit::UT_ARA:
						{
							m_pFrame_ATTACK_LINE_END0	= pX2Eqip->GetNormalAnim(0)->GetCloneFrame( L"ATTACK_LINE_END0" );
							m_pFrame_ATTACK_LINE_START0_FRONT = pX2Eqip->GetNormalAnim(0)->GetCloneFrame( L"ATTACK_LINE_START0_FRONT" );
						} break;
					}
				}
#endif ADD_UPGRADE_WEAPON_PARTICLE

			}//if
		}//if
	}//if

}
//}} robobeg : 2008-10-18

//#ifdef TITLE_SYSTEM
void CX2UnitViewerUI::UpdateTitle()
{
    int titleId = m_pUnit->GetTitleId();

    if(g_pData == NULL)
        return;

    if(titleId != m_iTitleId)
        m_iTitleId = titleId;
    else
        return;

    if(titleId > 0)
    {        
        if(m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE )
            g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEmblem );
		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hPart_Emblem_200 = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_pPart_Emblem_200 = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        if( m_hSeqEmblem == INVALID_PARTICLE_SEQUENCE_HANDLE )
        {
#ifdef SERV_GROW_UP_TITLE
			// 해당 타이틀이 갖고 있는 소켓의 타입을 이용하여 레벨을 찾아내도록 수정하였음 by 박진웅
			int iLevel = 0;
			if( m_pUnit != NULL )
				iLevel = m_pUnit->AccessUnitData().GetGrowUpLevelByTitle( titleId );
			wstring titleName = g_pData->GetTitleManager()->GetTitleModel( titleId, iLevel );
#else
			wstring titleName = g_pData->GetTitleManager()->GetTitleModel(titleId);            
#endif  
            m_hSeqEmblem = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  titleName.c_str(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );            
        }

        if( m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE )
        {
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_hPart_Emblem_200 = pSeq->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				m_pPart_Emblem_200 = pSeq->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				//pSeq->SetShow(false);
				pSeq->SetShowObject( true );				
			}
        }

    }
    else
    {
		if( m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
			if( NULL != pSeq )
			{
				pSeq->SetShowObject(false);            
				g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEmblem );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_hPart_Emblem_200 = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_pPart_Emblem_200 = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}
    }
}
//#endif


// @bClearParticle : ClearAllParticle() 함수를 호출할지 말지
void CX2UnitViewerUI::SetShowAttribEnchant( bool bShow )
{
	KTDXPROFILE();


	if( m_hSeqWeapon_Fire != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if( m_hSeqWeapon_Fire2 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire2 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if( m_hSeqWeapon_Fire3 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Fire3 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if ( m_hSeqWeapon_Wind != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}


	if ( m_hSeqWeapon_Wind2 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind2 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if ( m_hSeqWeapon_Wind3 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind3 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if ( m_hSeqWeapon_Wind4 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqWeapon_Wind4 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 무기 강화 이펙트 표시 설정
	if ( m_hSeqUpgradeWeapon != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if ( m_hSeqUpgradeWeapon2 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon2 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if ( m_hSeqUpgradeWeapon3 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon3 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}

	if ( m_hSeqUpgradeWeapon4 != INVALID_PARTICLE_SEQUENCE_HANDLE ) 
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon4 );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bShow );
			if ( bShow == false  )
			{
				pSeq->ClearAllParticle();
				pSeq->SetEmitRate( 0,0 );
			}
		}
	}
#endif ADD_UPGRADE_WEAPON_PARTICLE

}

#ifdef NEW_VILLAGE_UI

// 화면상에 유닛뷰어 고정해서 그릴 때 그 위치
// 현재는 강제로 Matrix들을 리셋해서 그려주는 형식이라 유닛뷰어에 달린 이펙트 등등을 모두 예외처리 해서 그려줘야 한다.
// 장기적으로 좋지 않으므로 특정 3D 모델을 화면상의 고정 위치에 그려주는 새로운 방법이 필요함.
void CX2UnitViewerUI::SetPositionOnScr(float x, float y, float z, float scale)	// xyz값은 화면 중앙점으로부터의 offset이다.
{
	// xyz 값을 화면 Position 기반으로 받게 하고, 그걸 -1~1 좌표로 변환해서 Move해 주자.
	// 현재 Position 기본은 1024 * 768이다.

	D3DXVECTOR3 vScrPos;
	D3DXVECTOR3 vScale( scale, scale, scale );

	vScrPos.x = x;
	vScrPos.y = y;
	vScrPos.z = z;
	vScale = vScale * m_fScale;

	m_bFixed = true;
	GetMatrix().Move( vScrPos.x, vScrPos.y, vScrPos.z );
	GetMatrix().Scale( vScale );
#ifdef FIX_REFORM_ENTRY_POINT_3RD		// 김종훈, 진입 구조 개편 3차 ( 크래시 ) 수정
	if ( NULL != m_pXSkinAnim )
#endif // FIX_REFORM_ENTRY_POINT_3RD	// 김종훈, 진입 구조 개편 3차 ( 크래시 ) 수정
		m_pXSkinAnim->GetMatrix().Move( GetMatrix().GetPos() );
	
}


#endif


// Fixed 할 때의 주의점 : m_bFixed이면 자신의 OnframeRender에서 자기에게 붙어있는 모든 파티클, 장비 등을 그려줘야 한다.
// ( DGManager에서 그리게 하면 안 된다 )
void CX2UnitViewerUI::SetFixed(bool val)
{
	m_bFixed = val; 

	if( true == m_bFixed )
	{
		BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
		{
			CX2EqipPtr pX2Eqip = value.second;

			if( pX2Eqip->GetEqipType() == CX2Item::UT_ATTACH_ANIM || 
				pX2Eqip->GetEqipType() == CX2Item::UT_ATTACH_NORMAL )
			{
				pX2Eqip->SetFixed();
			}
		}

		SetShowAttribEnchant( false );
	}
	else 
	{
		if( GetShowObject() == true )
		{
			SetShowAttribEnchant( true );
		}
	}

}


#ifdef UNIT_EMOTION
void CX2UnitViewerUI::EmotionFrameMove()
{
	if(m_ePlayedEmotion == CX2Unit::ET_NONE)
		return;


	if(m_pXSkinAnim != NULL && m_pXSkinAnim->IsAnimationEnd() == true)
	{
		switch(m_ePlayedEmotion)
		{
		case CX2Unit::ET_SITREADY:
			{
				wstring wstrEmotionName = L"Emotion_SitWait";
				PlayEmotion(CX2Unit::ET_SITWAIT, wstrEmotionName);
			}			
			break;
		case CX2Unit::ET_SITWAIT:
			break;
		default:
			{				
				m_ePlayedEmotion = CX2Unit::ET_NONE;
				//if( m_eOldMotionType == CX2UnitViewerUI::UVUMT_READY )
				//	PlayByMotionType( CX2UnitViewerUI::UVUMT_READY );
				//else
				//	PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );				
				//StateChange(GetWaitStateID(), true);				
			}			
			break;
		}
	}
}

bool CX2UnitViewerUI::PlayEmotion(CX2Unit::EMOTION_TYPE eEmotionId, wstring &wstrEmotionType)
{
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
	if ( false == IsPlayAvatarEmotionSoundWithouEmotion( eEmotionId ) )
		return false;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨


	if( CX2Unit::IsAvatarEmotion( eEmotionId ) 
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
		&& false == m_bIsPlayAvatarEmotionSoundWithoutEmotion
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력
		)
	{		
		wstring wstrEmotionName = L"";
		CX2Unit::EMOTION_TYPE eEmotion = CX2Unit::ET_NONE;

		bool bMixEmotion = m_pUnit->GetAvatarEmotion(wstrEmotionName,eEmotion);
		if( bMixEmotion == false )
			return false;
		if( eEmotionId != eEmotion )
			return false;

		if( bMixEmotion == true && wstrEmotionName.empty() != true && 
			( (eEmotion == CX2Unit::ET_EMOTION_AVATAR1 && m_bMixedEmotion[0] == false) ||
			(eEmotion == CX2Unit::ET_EMOTION_AVATAR2 && m_bMixedEmotion[1] == false) ||
			(eEmotion == CX2Unit::ET_EMOTION_AVATAR3 && m_bMixedEmotion[2] == false) || 
			(eEmotion == CX2Unit::ET_EMOTION_AVATAR4 && m_bMixedEmotion[3] == false) ||
			(eEmotion == CX2Unit::ET_EMOTION_AVATAR5 && m_bMixedEmotion[4] == false) 
#ifdef CRAYONPOP_SECOND_EMOTION
			|| (eEmotion == CX2Unit::ET_EMOTION_AVATAR6 && m_bMixedEmotion[5] == false) 
#endif // CRAYONPOP_SECOND_EMOTION
			) )
		//if( bMixEmotion == true && m_bMixedEmotion == false && wstrEmotionName.empty() != true )
		{		
			CKTDXDeviceXSkinMesh *pMixMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( wstrEmotionName );
			ASSERT( pMixMotion );			
			m_pXSkinAnim->MixAnim( pMixMotion );
			//m_bMixedEmotion = true;
			switch( eEmotion )
			{
			case CX2Unit::ET_EMOTION_AVATAR1:
				m_bMixedEmotion[0] = true;
				break;
			case CX2Unit::ET_EMOTION_AVATAR2:
				m_bMixedEmotion[1] = true;
				break;
			case CX2Unit::ET_EMOTION_AVATAR3:
				m_bMixedEmotion[2] = true;
				break;
			case CX2Unit::ET_EMOTION_AVATAR4:
				m_bMixedEmotion[3] = true;
				break;
			case CX2Unit::ET_EMOTION_AVATAR5:
				m_bMixedEmotion[4] = true;
				break;
#ifdef CRAYONPOP_SECOND_EMOTION
			case CX2Unit::ET_EMOTION_AVATAR6:
				m_bMixedEmotion[5] = true;
				break;
#endif // CRAYONPOP_SECOND_EMOTION
			}
			SAFE_CLOSE( pMixMotion );
		}
	}	

	if(m_eOldMotionType == CX2UnitViewerUI::UVUMT_READY)
		return false;

	if(eEmotionId == CX2Unit::ET_NONE || wstrEmotionType.empty() == true)
		return false;		

	if( eEmotionId == CX2Unit::ET_SITREADY &&
		(m_ePlayedEmotion == CX2Unit::ET_SITREADY || m_ePlayedEmotion == CX2Unit::ET_SITWAIT) )
		return false;

	if( eEmotionId == CX2Unit::ET_STANDUP && m_ePlayedEmotion != CX2Unit::ET_SITWAIT )
		return false;



	if(m_pXSkinAnim != NULL)
	{		
		if(eEmotionId == CX2Unit::ET_SITWAIT)
			PlayAnim( wstrEmotionType.c_str(), CKTDGXSkinAnim::XAP_LOOP, true );
		else
		{
			if( CX2Unit::IsAvatarEmotion( eEmotionId ) )
				PlayAnim( wstrEmotionType.c_str(), CKTDGXSkinAnim::XAP_ONE_WAIT, false );
			else
				PlayAnim( wstrEmotionType.c_str(), CKTDGXSkinAnim::XAP_ONE_WAIT, true );
		}
		
		m_ePlayedEmotion = eEmotionId;

		if( eEmotionId == CX2Unit::ET_EMOTION_AVATAR1 || eEmotionId == CX2Unit::ET_EMOTION_AVATAR3  || 
			eEmotionId == CX2Unit::ET_EMOTION_AVATAR4 || eEmotionId == CX2Unit::ET_EMOTION_AVATAR5
#ifdef CRAYONPOP_SECOND_EMOTION
			|| eEmotionId == CX2Unit::ET_EMOTION_AVATAR6
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
			|| eEmotionId == CX2Unit::ET_EMOTION_AVATAR7
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

			)
		{
			wstring wstrEmotionSoundName = L"";
			switch( eEmotionId )
			{
			case CX2Unit::ET_EMOTION_AVATAR1:
				wstrEmotionSoundName = L"Emotion_BigBangDance.ogg";
				break;
			case CX2Unit::ET_EMOTION_AVATAR3:
				wstrEmotionSoundName = L"Emotion_APinkDance.ogg";
				break;
			case CX2Unit::ET_EMOTION_AVATAR4:
				wstrEmotionSoundName = L"Emotion_APink_Love.ogg";
				break;
			case CX2Unit::ET_EMOTION_AVATAR5:
				wstrEmotionSoundName = L"DancingQueen_Music.ogg";
				break;
#ifdef CRAYONPOP_SECOND_EMOTION
			case CX2Unit::ET_EMOTION_AVATAR6:
				wstrEmotionSoundName = L"Emotion_Bbabbabba.ogg";
				break;
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
			case CX2Unit::ET_EMOTION_AVATAR7:
				wstrEmotionSoundName = L"Emotion_Crayonpop_NoMotion.ogg";
				break;
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨

			default:
				break;
			}

			if( m_pAvatarEmotionSound != NULL )
			{
				m_pAvatarEmotionSound->Stop();
				SAFE_CLOSE( m_pAvatarEmotionSound );
			}
			const float MAGIC_NUMBER = 500.f;
			const float fMaxDist = g_pKTDXApp->GetDGManager()->GetCamera().GetCameraDistance() + MAGIC_NUMBER;
			m_pAvatarEmotionSound = g_pKTDXApp->GetDeviceManager()->OpenSound( wstrEmotionSoundName, 10, true, fMaxDist );
			if( m_pAvatarEmotionSound != NULL )
				m_pAvatarEmotionSound->Play( false, false );
		}
		else
		{
			if( NULL != g_pChatBox && NULL != m_pUnit )
			{
				g_pChatBox->PlayEmotionSound( m_pUnit->GetType(), eEmotionId, D3DXVECTOR3( 0, 0, 0 ), false );
			}
		}

		if ( eEmotionId == CX2Unit::ET_EMOTION_AVATAR4 )
		{
			D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Bip01_Head" );
			g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, L"Emotion_APINK_LOVE_P01", vBonePos );
		}

		return true;
	}

	return false;
}

bool CX2UnitViewerUI::GetPlayerEmotion()
{
	if(m_ePlayedEmotion == CX2Unit::ET_NONE)
		return false;
	return true;
}
#endif

#ifdef ELSWORD_SECOND_CLASS_CHANGE
void CX2UnitViewerUI::SetUnitReForm()
{
	if( m_pXSkinAnim == NULL )
		return;

	float fMoveBip01 = 10.f;
	float fMoveFootStep = -10.f;
	D3DXVECTOR3 vHead = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vThigh = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vCalf = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vFoot = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXVECTOR3 vHand = D3DXVECTOR3(1.f, 1.f, 1.f);	
	D3DXVECTOR3 vSpine = D3DXVECTOR3(1.f, 1.f, 1.f);
	float fScale = 1.f;

	// 실시간 테스트용
#ifndef _SERVICE_

	FILE *fp = fopen("reform.txt", "r");	
	if(fp != NULL)
	{
		fscanf(fp, "%f%f%f", &fMoveBip01, &fMoveFootStep, &fScale);
		fscanf(fp, "%f%f%f", &vHead.x, &vHead.y, &vHead.z);
		fscanf(fp, "%f%f%f", &vThigh.x, &vThigh.y, &vThigh.z);
		fscanf(fp, "%f%f%f", &vCalf.x, &vCalf.y, &vCalf.z);
		fscanf(fp, "%f%f%f", &vFoot.x, &vFoot.y, &vFoot.z);
		fscanf(fp, "%f%f%f", &vHand.x, &vHand.y, &vHand.z);
		fscanf(fp, "%f%f%f", &vSpine.x, &vSpine.y, &vSpine.z);		
		fclose(fp);
	}
#endif

#ifdef UNIT_SCALE_COMBINE_ONE
	if ( m_pUnit != NULL )
	{
		#ifdef MOVE_BONE
		switch ( m_pUnit->GetType() )
		{
		case CX2Unit::UT_ELSWORD:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), 10.f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), -10.f );

				D3DXVECTOR3 vHead	= D3DXVECTOR3( 0.93f, 0.9f, 0.93f );
				D3DXVECTOR3 vThigh	= D3DXVECTOR3( 1.08f, 1.f, 0.98f );
				D3DXVECTOR3 vCalf	= D3DXVECTOR3( 1.12f, 0.97f, 0.94f );
				D3DXVECTOR3 vFoot	= D3DXVECTOR3( 0.895f, 0.94f, 0.94f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
				if ( pFrame_R_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_R_Leg );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
				if ( pFrame_L_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_L_Leg );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
				if ( pFrame_R_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_R_Foot );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
				if ( pFrame_L_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_L_Foot );
			} break;

		case CX2Unit::UT_ARME:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), 4.f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), -4.f );

				D3DXVECTOR3 vHead		= D3DXVECTOR3( 0.95f, 0.93f, 0.93f );
				D3DXVECTOR3 vThigh		= D3DXVECTOR3( 1.f, 1.f, 1.f );
				D3DXVECTOR3 vCalf		= D3DXVECTOR3( 1.1f, 1.f, 1.f );
				D3DXVECTOR3 vHand		= D3DXVECTOR3( 0.9f, 0.93f, 0.93f );
				D3DXVECTOR3 vFoot		= D3DXVECTOR3( 0.9f, 1.f, 1.f );
				D3DXVECTOR3 vDummyHand	= D3DXVECTOR3( 1.f, 1.f, 1.f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
				if ( pFrame_R_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_R_Leg );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
				if ( pFrame_L_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_L_Leg );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
				if ( pFrame_R_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_R_Foot );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
				if ( pFrame_L_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_L_Foot );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Hand" );
				if ( pFrame_R_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_R_Hand );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
				if ( pFrame_L_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_L_Hand );

				vDummyHand.x = 1.f / vHand.x;
				vDummyHand.y = 1.f / vHand.y;
				vDummyHand.z = 1.f / vHand.z;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				if ( pFrame_R_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
				if ( pFrame_L_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );
			}

		case CX2Unit::UT_LIRE:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), 1.0f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), -1.0f );

				D3DXVECTOR3 vHead = D3DXVECTOR3( 0.94f, 0.92f, 0.90f );
				D3DXVECTOR3 vThigh = D3DXVECTOR3( 1.02f, 1.f, 1.f );
				D3DXVECTOR3 vCalf = D3DXVECTOR3( 1.02f, 1.f, 1.f );
				D3DXVECTOR3 vFoot = D3DXVECTOR3( 0.93f, 0.93f, 0.93f );
				D3DXVECTOR3 vHand = D3DXVECTOR3( 0.9f, 0.9f, 0.9f );
				D3DXVECTOR3 vDummyHand = D3DXVECTOR3( 1.f, 1.f, 1.f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
				if ( pFrame_R_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_R_Leg );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
				if ( pFrame_L_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_L_Leg );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
				if ( pFrame_R_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_R_Foot );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
				if ( pFrame_L_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_L_Foot );		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Hand");
				if ( pFrame_R_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_R_Hand );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
				if ( pFrame_L_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_L_Hand );

				vDummyHand.x = 1.f / vHand.x;
				vDummyHand.y = 1.f / vHand.y;
				vDummyHand.z = 1.f / vHand.z;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				if ( pFrame_R_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
				if ( pFrame_L_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );	
			} break;

		case CX2Unit::UT_RAVEN:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), 5.0f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), -5.0f );

				D3DXVECTOR3 vHead = D3DXVECTOR3( 0.88f, 0.9f, 0.92f );
				D3DXVECTOR3 vThigh = D3DXVECTOR3( 1.05f, 0.94f, 0.92f );
				D3DXVECTOR3 vCalf = D3DXVECTOR3( 1.065f, 0.93f, 0.9f );
				D3DXVECTOR3 vFoot = D3DXVECTOR3( 0.93f, 0.85f, 0.83f );
				D3DXVECTOR3 vDummyHand = D3DXVECTOR3( 1.f, 1.f, 1.f );
				D3DXVECTOR3 vSpine = D3DXVECTOR3( 0.94f, 0.97f, 1.f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
				if ( pFrame_R_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_R_Leg );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
				if ( pFrame_L_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_L_Leg );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
				if ( pFrame_R_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_R_Foot );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
				if ( pFrame_L_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_L_Foot );		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameSpine = m_pXSkinAnim->GetCloneFrame( L"Bip01_Spine" );
				if ( pFrameSpine != NULL )
					SetFrameScale( &vSpine, false, pFrameSpine );
			} break;

		case CX2Unit::UT_EVE:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), -0.8f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), 0.8f );

				D3DXVECTOR3 vHead = D3DXVECTOR3( 0.94f, 0.97f, 0.98f );
				D3DXVECTOR3 vThigh = D3DXVECTOR3( 1.0f, 0.98f, 0.98f );
				D3DXVECTOR3 vCalf = D3DXVECTOR3( 1.0f, 0.98f, 0.98f );
				D3DXVECTOR3 vFoot = D3DXVECTOR3( 0.95f, 0.95f, 0.95f );
				D3DXVECTOR3 vHand = D3DXVECTOR3( 0.9f, 0.96f, 0.96f );		
				D3DXVECTOR3 vDummyHand = D3DXVECTOR3( 1.f, 1.f, 1.f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Thigh" );
				if ( pFrame_R_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_R_Leg );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Thigh" );
				if ( pFrame_L_Leg != NULL )
					SetFrameScale( &vThigh, true, pFrame_L_Leg );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Foot");
				if(pFrame_R_Foot != NULL)
					SetFrameScale(&vFoot, true, pFrame_R_Foot);
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Foot");
				if(pFrame_L_Foot != NULL)
					SetFrameScale(&vFoot, true, pFrame_L_Foot);		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Hand" );
				if ( pFrame_R_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_R_Hand );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
				if ( pFrame_L_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_L_Hand );		

				vDummyHand.x = 1.f;
				vDummyHand.y = 1.f;
				vDummyHand.z = 1.f;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				if ( pFrame_R_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
				if ( pFrame_L_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );
			} break;

		case CX2Unit::UT_CHUNG:
			{
				D3DXVECTOR3 vHead = D3DXVECTOR3( 0.93f, 0.93f, 0.93f );
				D3DXVECTOR3 vDummyHand = D3DXVECTOR3( 0.92f, 0.92f, 0.92f );
				m_fScale = 1.08f;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );		

				vDummyHand = vDummyHand / m_fScale;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				if ( pFrame_R_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
				if ( pFrame_L_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );
			} break;

		case CX2Unit::UT_ARA:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), 2.0f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), -2.0f );

				D3DXVECTOR3 vHead = D3DXVECTOR3( 0.96f, 0.96f, 0.96f );
				D3DXVECTOR3 vCalf = D3DXVECTOR3( 1.02f, 1.f, 1.f );
				D3DXVECTOR3 vFoot = D3DXVECTOR3( 1.0f, 0.96f, 0.96f );
				D3DXVECTOR3 vHand = D3DXVECTOR3( 0.96f, 0.96f, 0.96f );	
				D3DXVECTOR3 vDummyHand = D3DXVECTOR3( 1.f, 1.f, 1.f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
				if ( pFrame_R_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_R_Foot );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
				if ( pFrame_L_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_L_Foot );		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Hand");
				if ( pFrame_R_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_R_Hand );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
				if ( pFrame_L_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_L_Hand );

				vDummyHand.x = 1.f / vHand.x;
				vDummyHand.y = 1.f / vHand.y;
				vDummyHand.z = 1.f / vHand.z;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				if ( pFrame_R_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
				if ( pFrame_L_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );	
			} break;

		case CX2Unit::UT_ELESIS:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), 2.0f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), -2.0f );

				D3DXVECTOR3 vHead = D3DXVECTOR3( 0.96f, 0.96f, 0.96f );
				D3DXVECTOR3 vCalf = D3DXVECTOR3( 1.02f, 1.f, 1.f );
				D3DXVECTOR3 vFoot = D3DXVECTOR3( 1.0f, 0.96f, 0.96f );
				D3DXVECTOR3 vHand = D3DXVECTOR3( 0.96f, 0.96f, 0.96f );	
				D3DXVECTOR3 vDummyHand = D3DXVECTOR3( 1.f, 1.f, 1.f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
				if ( pFrame_R_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_R_Foot );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
				if ( pFrame_L_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_L_Foot );		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Hand");
				if ( pFrame_R_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_R_Hand );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
				if ( pFrame_L_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_L_Hand );

				vDummyHand.x = 1.f / vHand.x;
				vDummyHand.y = 1.f / vHand.y;
				vDummyHand.z = 1.f / vHand.z;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				if ( pFrame_R_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
				if ( pFrame_L_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );	
			} break;

		#ifdef SERV_9TH_NEW_CHARACTER // JHKang
		case CX2Unit::UT_ADD:
			{
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01" ), 2.0f );
				SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame( L"Bip01_Footsteps" ), -2.0f );

				D3DXVECTOR3 vHead = D3DXVECTOR3( 0.93f, 0.93f, 0.93f );
				D3DXVECTOR3 vCalf = D3DXVECTOR3( 1.02f, 1.f, 1.f );
				D3DXVECTOR3 vFoot = D3DXVECTOR3( 1.0f, 0.96f, 0.96f );
				D3DXVECTOR3 vHand = D3DXVECTOR3( 0.96f, 0.96f, 0.96f );	
				D3DXVECTOR3 vDummyHand = D3DXVECTOR3( 1.f, 1.f, 1.f );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame( L"Bip01_Head" );
				if ( pFrameHead != NULL )
					SetFrameScale( &vHead, true, pFrameHead );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Calf" );
				if ( pFrame_R_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_R_Calf );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Calf" );
				if ( pFrame_L_Calf != NULL )
					SetFrameScale( &vCalf, true, pFrame_L_Calf );

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_R_Foot" );
				if ( pFrame_R_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_R_Foot );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Foot" );
				if ( pFrame_L_Foot != NULL )
					SetFrameScale( &vFoot, true, pFrame_L_Foot );		

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Hand");
				if ( pFrame_R_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_R_Hand );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame( L"Bip01_L_Hand" );
				if ( pFrame_L_Hand != NULL )
					SetFrameScale( &vHand, true, pFrame_L_Hand );

				vDummyHand.x = 1.f / vHand.x;
				vDummyHand.y = 1.f / vHand.y;
				vDummyHand.z = 1.f / vHand.z;

				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy1_Rhand" );
				if ( pFrame_R_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_R_Dummy );
				CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame( L"Dummy2_Lhand" );
				if ( pFrame_L_Dummy != NULL )
					SetFrameScale( &vDummyHand, true, pFrame_L_Dummy );	
			} break;
		#endif

		default:
			break;
		}
		#endif //MOVE_BONE
	}
#else //UNIT_SCALE_COMBINE_ONE

#ifdef MOVE_BONE
if( m_pUnit != NULL )
{
	switch( m_pUnit->GetType() )
	{
	case CX2Unit::UT_LIRE:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 1.0f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -1.0f);
		}
		break;
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UT_RAVEN:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 7.f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -7.f);
		}
		break;
#endif
#ifdef EVE_SECOND_CLASS_CHANGE
	case CX2Unit::UT_EVE:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), -0.8f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), 0.8f);
		}
		break;
#endif
	default:
		break;
	}

	switch( m_pUnit->GetClass() )
	{
#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_SWORDMAN:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 2.f );
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -2.f );
		}
		break;
	case CX2Unit::UC_ELSWORD_KNIGHT:
	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
#endif ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
#endif //SERV_ELSWORD_INFINITY_SWORD
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 10.f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -10.f);
		}
		break;
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01"), 3.f);
			SetFrameMoveBoneY( m_pXSkinAnim->GetCloneFrame(L"Bip01_Footsteps"), -3.f);
		}
		break;
	default:
		break;
	}
}
#endif

	#ifdef ELSWORD_SHEATH_KNIGHT
		if( m_pUnit != NULL && 
			( m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_SWORDMAN ) )
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(1.f, 0.95f, 0.97f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.f, 1.f, 1.f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.05f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.9f, 1.f, 0.9f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
			if(pFrameHead != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vHead, true, pFrameHead);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Thigh");
			if(pFrame_R_Leg != NULL) // && pFrame_R_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_R_Leg);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Thigh");
			if(pFrame_L_Leg != NULL) // && pFrame_L_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_L_Leg);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Calf");
			if(pFrame_R_Calf != NULL) // && pFrame_R_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_R_Calf);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Calf");
			if(pFrame_L_Calf != NULL) // && pFrame_L_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_L_Calf);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Foot");
			if(pFrame_R_Foot != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_R_Foot);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Foot");
			if(pFrame_L_Foot != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_L_Foot);		
		}
		else if( m_pUnit != NULL && 
			( m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_KNIGHT || 
			m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_MAGIC_KNIGHT || 
			m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_SHEATH_KNIGHT || 
			m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_LORD_KNIGHT || 
			m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_RUNE_SLAYER
	#ifdef SERV_ELSWORD_INFINITY_SWORD
			|| m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_INFINITY_SWORD
	#endif //SERV_ELSWORD_INFINITY_SWORD
			) )
	#else
		if( m_pUnit != NULL && 
			( m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_LORD_KNIGHT || m_pUnit->GetClass() == CX2Unit::UC_ELSWORD_RUNE_SLAYER ) )
	#endif ELSWORD_SHEATH_KNIGHT
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(1.f, 0.95f, 0.97f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.07f, 1.f, 1.f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.12f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.9f, 1.f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
			if(pFrameHead != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vHead, true, pFrameHead);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Thigh");
			if(pFrame_R_Leg != NULL) // && pFrame_R_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_R_Leg);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Thigh");
			if(pFrame_L_Leg != NULL) // && pFrame_L_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_L_Leg);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Calf");
			if(pFrame_R_Calf != NULL) // && pFrame_R_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_R_Calf);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Calf");
			if(pFrame_L_Calf != NULL) // && pFrame_L_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_L_Calf);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Foot");
			if(pFrame_R_Foot != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_R_Foot);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Foot");
			if(pFrame_L_Foot != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_L_Foot);		
		}
		else if( m_pUnit != NULL && m_pUnit->GetType() == CX2Unit::UT_LIRE )
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.96f, 0.94f, 0.92f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.02f, 1.f, 1.f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.02f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.93f, 0.93f, 0.93f);
			D3DXVECTOR3 vHand = D3DXVECTOR3(0.9f, 0.9f, 0.9f);	
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
			if(pFrameHead != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vHead, true, pFrameHead);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Thigh");
			if(pFrame_R_Leg != NULL) // && pFrame_R_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_R_Leg);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Thigh");
			if(pFrame_L_Leg != NULL) // && pFrame_L_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_L_Leg);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Calf");
			if(pFrame_R_Calf != NULL) // && pFrame_R_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_R_Calf);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Calf");
			if(pFrame_L_Calf != NULL) // && pFrame_L_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_L_Calf);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Foot");
			if(pFrame_R_Foot != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_R_Foot);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Foot");
			if(pFrame_L_Foot != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_L_Foot);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Hand");
			if(pFrame_R_Hand != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vHand, true, pFrame_R_Hand);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Hand");
			if(pFrame_L_Hand != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vHand, true, pFrame_L_Hand);		

			vDummyHand.x = 1.f / vHand.x;
			vDummyHand.y = 1.f / vHand.y;
			vDummyHand.z = 1.f / vHand.z;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy1_Rhand");
			if(pFrame_R_Dummy != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_R_Dummy);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy2_Lhand");
			if(pFrame_L_Dummy != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_L_Dummy);	
		}
		else if( m_pUnit != NULL && 
			(	m_pUnit->GetClass() == CX2Unit::UC_ARME_ELEMENTAL_MASTER || 
				m_pUnit->GetClass() == CX2Unit::UC_ARME_VOID_PRINCESS 
	#ifdef SERV_ARME_DIMENSION_WITCH
				|| m_pUnit->GetClass() == CX2Unit::UC_ARME_DIMENSION_WITCH 
	#endif //SERV_ARME_DIMENSION_WITCH
			) )
		{
			D3DXVECTOR3 vHead	= D3DXVECTOR3(0.97f, 0.95f, 0.95f);
			D3DXVECTOR3 vThigh	= D3DXVECTOR3(1.f, 1.f, 1.f);
			D3DXVECTOR3 vCalf	= D3DXVECTOR3(1.06f, 1.f, 1.f);
			D3DXVECTOR3 vFoot	= D3DXVECTOR3(1.f, 1.f, 1.f);
			D3DXVECTOR3 vHand	= D3DXVECTOR3(0.9f, 0.93f, 0.93f);	
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
			if(pFrameHead != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vHead, true, pFrameHead);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Thigh");
			if(pFrame_R_Leg != NULL) // && pFrame_R_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_R_Leg);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Thigh");
			if(pFrame_L_Leg != NULL) // && pFrame_L_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_L_Leg);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Calf");
			if(pFrame_R_Calf != NULL) // && pFrame_R_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_R_Calf);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Calf");
			if(pFrame_L_Calf != NULL) // && pFrame_L_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_L_Calf);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Foot");
			if(pFrame_R_Foot != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_R_Foot);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Foot");
			if(pFrame_L_Foot != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_L_Foot);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Hand");
			if(pFrame_R_Hand != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vHand, true, pFrame_R_Hand);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Hand");
			if(pFrame_L_Hand != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vHand, true, pFrame_L_Hand);		

			vDummyHand.x = 1.f / vHand.x;
			vDummyHand.y = 1.f / vHand.y;
			vDummyHand.z = 1.f / vHand.z;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy1_Rhand");
			if(pFrame_R_Dummy != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_R_Dummy);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy2_Lhand");
			if(pFrame_L_Dummy != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_L_Dummy);	
		}
	#ifdef RAVEN_SECOND_CLASS_CHANGE
		else if( m_pUnit != NULL && m_pUnit->GetType() == CX2Unit::UT_RAVEN )
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.9f, 0.92f, 0.94f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.05f, 0.98f, 0.98f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.05f, 1.f, 1.f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.95f, 0.95f, 0.85f);
			D3DXVECTOR3 vHand = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);
			D3DXVECTOR3 vSpine = D3DXVECTOR3(0.93f, 0.97f, 1.f);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
			if(pFrameHead != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vHead, true, pFrameHead);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Thigh");
			if(pFrame_R_Leg != NULL) // && pFrame_R_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_R_Leg);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Thigh");
			if(pFrame_L_Leg != NULL) // && pFrame_L_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_L_Leg);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Calf");
			if(pFrame_R_Calf != NULL) // && pFrame_R_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_R_Calf);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Calf");
			if(pFrame_L_Calf != NULL) // && pFrame_L_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_L_Calf);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Foot");
			if(pFrame_R_Foot != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_R_Foot);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Foot");
			if(pFrame_L_Foot != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_L_Foot);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameSpine = m_pXSkinAnim->GetCloneFrame(L"Bip01_Spine");
			if(pFrameSpine != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vSpine, false, pFrameSpine);			
		}
	#endif
	#ifdef EVE_SECOND_CLASS_CHANGE
		else if( m_pUnit != NULL && m_pUnit->GetType() == CX2Unit::UT_EVE )
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.94f, 0.97f, 0.98f);
			D3DXVECTOR3 vThigh = D3DXVECTOR3(1.0f, 0.98f, 0.98f);
			D3DXVECTOR3 vCalf = D3DXVECTOR3(1.0f, 0.98f, 0.98f);
			D3DXVECTOR3 vFoot = D3DXVECTOR3(0.95f, 0.95f, 0.95f);
			D3DXVECTOR3 vHand = D3DXVECTOR3(0.9f, 0.96f, 0.96f);		
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
			if(pFrameHead != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vHead, true, pFrameHead);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Thigh");
			if(pFrame_R_Leg != NULL) // && pFrame_R_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_R_Leg);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Leg = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Thigh");
			if(pFrame_L_Leg != NULL) // && pFrame_L_Leg->m_bScale == false)
				SetFrameScale(&vThigh, true, pFrame_L_Leg);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Calf");
			if(pFrame_R_Calf != NULL) // && pFrame_R_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_R_Calf);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Calf = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Calf");
			if(pFrame_L_Calf != NULL) // && pFrame_L_Calf->m_bScale == false)
				SetFrameScale(&vCalf, true, pFrame_L_Calf);

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Foot");
			if(pFrame_R_Foot != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_R_Foot);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Foot = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Foot");
			if(pFrame_L_Foot != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vFoot, true, pFrame_L_Foot);		

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_R_Hand");
			if(pFrame_R_Hand != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vHand, true, pFrame_R_Hand);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Hand = m_pXSkinAnim->GetCloneFrame(L"Bip01_L_Hand");
			if(pFrame_L_Hand != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vHand, true, pFrame_L_Hand);		

			vDummyHand.x = 1.f;// / vHand.x;
			vDummyHand.y = 1.f;// / vHand.y;
			vDummyHand.z = 1.f;// / vHand.z;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy1_Rhand");
			if(pFrame_R_Dummy != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_R_Dummy);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy2_Lhand");
			if(pFrame_L_Dummy != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_L_Dummy);

		}
	#endif
	#ifdef CHUNG_SECOND_CLASS_CHANGE
	#ifdef SERV_CHUNG_TACTICAL_TROOPER
		else if( m_pUnit != NULL && 
			( m_pUnit->GetClass() == CX2Unit::UC_CHUNG_IRON_PALADIN || m_pUnit->GetClass() == CX2Unit::UC_CHUNG_DEADLY_CHASER || m_pUnit->GetClass() == CX2Unit::UC_CHUNG_TACTICAL_TROOPER) )
	#else
		else if( m_pUnit != NULL && 
			( m_pUnit->GetClass() == CX2Unit::UC_CHUNG_IRON_PALADIN || m_pUnit->GetClass() == CX2Unit::UC_CHUNG_DEADLY_CHASER ) )
	#endif SERV_CHUNG_TACTICAL_TROOPER
		{
			D3DXVECTOR3 vHead = D3DXVECTOR3(0.93f, 0.96f, 0.96f);
			D3DXVECTOR3 vDummyHand = D3DXVECTOR3(1.f, 1.f, 1.f);
			//m_fScale = fScale;
			m_fScale = 1.13f;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrameHead = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
			if(pFrameHead != NULL) // && pFrameHead->m_bScale == false)
				SetFrameScale(&vHead, true, pFrameHead);		
				
			vDummyHand = vDummyHand / m_fScale;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_R_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy1_Rhand");
			if(pFrame_R_Dummy != NULL) // && pFrame_R_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_R_Dummy);
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame_L_Dummy = m_pXSkinAnim->GetCloneFrame(L"Dummy2_Lhand");
			if(pFrame_L_Dummy != NULL) // && pFrame_L_Foot->m_bScale == false)
				SetFrameScale(&vDummyHand, true, pFrame_L_Dummy);		
		}
	#endif

#endif //UNIT_SCALE_COMBINE_ONE

}
#endif

//{{ kimhc // 2010-01-05 // PC방 프리미엄 서비스
#ifdef	PC_BANG_WORK
void CX2UnitViewerUI::RemovePCBangEquips()
{
	BOOST_TEST_FOREACH( const ViewEquipMap::value_type&, value, m_ViewEquipMap )
	{
		if ( CX2EqipPtr pX2Eqip = value.second )
		{
			const CX2Item::ItemTemplet* pItemTemplet = _GetSafeEquipItemTemplet( pX2Eqip );
			if ( pItemTemplet != NULL && pItemTemplet->GetIsPcBang() == true )
			{
				RemoveEqip( pX2Eqip->GetItemUID() );
			}
		}
		
	}//BOOST_TEST_FOREACH()
}
#endif	PC_BANG_WORK
//}} kimhc // 2010-01-05 // PC방 프리미엄 서비스

//{{ kimhc // 2010.12.21 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
/** @function : GetWeaponTraceStartAndEndName
	@brief : 캐릭터 마다 다른 트레이스 네임을 가져오는 함수
	@param :  OUT wstring* pwstrTraceStartName_, OUT wstring* pwstrTraceEndName_ (트레이스가 붙을 시작점과 끝점)
*/

void CX2UnitViewerUI::GetWeaponTraceStartAndEndName( OUT wstring* pwstrTraceStartName_, OUT wstring* pwstrTraceEndName_ )
{
	switch ( GetUnit()->GetType() )
	{
	case CX2Unit::UT_CHUNG:
		{
			*pwstrTraceStartName_ = L"TRACE_START0";
			*pwstrTraceEndName_ = L"ATTACK_SPHERE3_Back01";
		} break;

	default:
		{
			*pwstrTraceStartName_ = L"TRACE_START0";
			*pwstrTraceEndName_ = L"TRACE_END0";
		} break;
	}	// switch
}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.21 //  2010-12-23 New Character CHUNG

void CX2UnitViewerUI::SetHideSelectUnitWeapon()			/// 케릭터 선택 화면등에서 꺼내든 무기 숨기는 함수
{
	CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_NONE;

	if( NULL != m_pUnit )
	{
		eUnitClass = m_pUnit->GetClass();
	}
	switch(eUnitClass)
	{
	//데들리체이서
	#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		{
			m_bShowGun = false;
			SetShowObjectMeshPlayer(m_hChungMiniGunL, m_bShowGun);
			SetShowObjectMeshPlayer(m_hChungMiniGunR, m_bShowGun);
		} break;
	#endif //CHUNG_SECOND_CLASS_CHANGE
	//시스나이트
	#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
	#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
	#endif
		{
			m_bShowSword = false;
			SetShowObjectMeshPlayer(m_hElswordSword, m_bShowSword);
		} break;
	#endif //ELSWORD_SHEATH_KNIGHT
	//트래핑레인져
	#ifdef SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
		{
			m_bShowLTrapSword = false;
			SetShowObjectMeshPlayer(m_hInstLTrapSwordReady, m_bShowLTrapSword);
		} break;
	#endif //SERV_TRAPPING_RANGER_TEST
	//나이트와쳐
	#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
		{
			m_bShowRNWTrapSword = false;
			SetShowObjectMeshPlayer(m_hInstRNWTrapSwordReady, m_bShowRNWTrapSword);
		} break;
	#endif //SERV_RENA_NIGHT_WATCHER
	default:
		break;
	}
}

void CX2UnitViewerUI::SetShowObjectMeshPlayer(CKTDGXMeshPlayer::CXMeshInstanceHandle MeshInstHandle_, bool bShow_)
{
	if( INVALID_MESH_INSTANCE_HANDLE != MeshInstHandle_ )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( MeshInstHandle_ );

		if ( NULL != pMeshInst )
		{
			pMeshInst->SetShowObject( bShow_, true );
		}
	}
}

#ifdef ADD_UPGRADE_WEAPON_PARTICLE	/// 강화 무기에 대한 이펙트 출력 처리

/// 강화 무기 이펙트 초기화
void CX2UnitViewerUI::InitUpgradeWeaponParticle()
{
    if ( m_pUnit == NULL )
        return;

	const CX2Inventory& kInventory = m_pUnit->GetInventory();
	{
		CX2Item* pItem = kInventory.GetEquippingItemByEquipPos( CX2Unit::EP_WEAPON_HAND, false );

		if( NULL != m_pUnit->GetUnitTemplet() &&
			NULL != pItem &&
			( (	pItem->GetItemData().m_PeriodType == CX2Item::PT_ENDURANCE && pItem->GetItemData().m_Endurance > 0) || 
			pItem->GetItemData().m_PeriodType == CX2Item::PT_INFINITY ) )
		{
			const int			iUpgradeLevel	= pItem->GetItemData().m_EnchantLevel;		/// 현재 무기의 강화 레벨
			CX2Unit::UNIT_TYPE	eUnitType		= m_pUnit->GetUnitTemplet()->m_UnitType;	/// 현재 유저의 유닛 타입

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
					
				/// 2개인 이브의 기어
				case CX2Unit::UT_EVE:
	#ifdef SERV_9TH_NEW_CHARACTER // 김태환
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
void CX2UnitViewerUI::CreateUpgradeWeaponParticle( const WCHAR* pName1, const WCHAR* pName2 /*= NULL*/, const WCHAR* pName3 /*= NULL*/, const WCHAR* pName4 /*= NULL*/ )
{
	if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
	{
		/// 1번 파티클
		if( NULL != pName1 && m_hSeqUpgradeWeapon == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, pName1, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon );

			if( NULL != pSeq )
				pSeq->SetEmitRate( 0, 0 );
		}

		/// 2번 파티클
		if( NULL != pName2 && m_hSeqUpgradeWeapon2 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon2 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, pName2, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon2 );

			if( NULL != pSeq )
				pSeq->SetEmitRate( 0, 0 );
		}

		/// 3번 파티클
		if( NULL != pName3 && m_hSeqUpgradeWeapon3 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon3 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, pName3, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon3 );

			if( NULL != pSeq )
				pSeq->SetEmitRate( 0, 0 );
		}

		/// 4번 파티클
		if( NULL != pName3 && m_hSeqUpgradeWeapon4 == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_hSeqUpgradeWeapon4 = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL, pName3, 0.f, 0.f ,0.f );
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon4 );

			if( NULL != pSeq )
				pSeq->SetEmitRate( 0, 0 );
		}
	}
}

/// 강화 무기 이펙트 갱신
void CX2UnitViewerUI::ProcessUpgradeWeaponParticle()
{
	if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon );
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon2 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon2 );
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon3 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon3 );
		CKTDGParticleSystem::CParticleEventSequence* pSeqUpgradeWeapon4 = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqUpgradeWeapon4 );

		/// 보이지 말아야 할 때는 숨기자
#ifdef RIDING_SYSTEM
	#ifdef FIX_UPGRADE_WEAPON_PARTICLE
			if ( false == GetShowObject() || 
				 false == CanNotShowEnchantedWeaponEffectToRiding() )
	#else
			/// 탈것을 타고 있으면, 무기 인첸트 효과 모두 꺼주자.
			if ( true == GetShowObject() &&	true == CanNotShowEnchantedWeaponEffectToRiding() || UVS_FIELD != m_eUnitViewerState )
	#endif // FIX_UPGRADE_WEAPON_PARTICLE
#else //RIDING_SYSTEM
		if( false == GetShowObject() )
#endif //RIDING_SYSTEM
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
		if( NULL != m_pUnit && NULL != m_pUnit->GetUnitTemplet() )
		{
			switch( m_pUnit->GetUnitTemplet()->m_UnitType )
			{
			/// 길쭉한 모양의 무기
			case CX2Unit::UT_ELSWORD:
			case CX2Unit::UT_ARME:
			case CX2Unit::UT_RAVEN:
			case CX2Unit::UT_CHUNG:
	#ifdef NEW_CHARACTER_EL
			case CX2Unit::UT_ELESIS:
	#endif // NEW_CHARACTER_EL
				{
					if( NULL == m_pFrame_TRACE_START[0] || NULL == m_pFrame_TRACE_END[0] )
					{
						// 백그라운드 로딩 때문에 frame 포인터를 못 찾았을 수 있다. 일정 시간 간격으로 메시가 로딩되었는지 체크를 한다
						if( m_TimerResetWeaponTraceFrame.elapsed() > 2.f )
						{
							InitWeaponTrace();
						}
						return;
					}

					/// 무기 트레이스 더미 위치로 설정해 주자
					if ( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL && pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )
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
					if( NULL == m_pFrame_ATTACK_LINE_START0_FRONT || NULL == m_pFrame_ATTACK_LINE_END0 )
					{
						// 백그라운드 로딩 때문에 frame 포인터를 못 찾았을 수 있다. 일정 시간 간격으로 메시가 로딩되었는지 체크를 한다
						if( m_TimerResetWeaponTraceFrame.elapsed() > 2.f )
						{
							InitWeaponTrace();
						}
						return;
					}

					/// 무기 트레이스 더미 위치로 설정해 주자
					if ( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL && pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )
					{
						D3DXVECTOR3 weaponPos( 0.f, 0.f, 0.f );
						GetFramePos( &weaponPos, m_pFrame_ATTACK_LINE_END0 );

						D3DXVECTOR3 traceEndPos( 0.f, 0.f, 0.f );
						GetFramePos( &traceEndPos,m_pFrame_ATTACK_LINE_START0_FRONT );

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
						D3DXVECTOR3 vBonePos1 = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
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
					if ( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL )		/// 오른쪽
					{
						D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy1_Rhand" );
						pSeqUpgradeWeapon->SetPosition( vBonePos );
						pSeqUpgradeWeapon2->SetPosition( vBonePos );

						pSeqUpgradeWeapon->SetEmitRate( 10, 10 );
						pSeqUpgradeWeapon2->SetEmitRate( 10, 10 );
					}

					if ( pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )		/// 왼쪽
					{
						D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Dummy2_Lhand" );
						pSeqUpgradeWeapon3->SetPosition( vBonePos );
						pSeqUpgradeWeapon4->SetPosition( vBonePos );

						pSeqUpgradeWeapon3->SetEmitRate( 10, 10 );
						pSeqUpgradeWeapon4->SetEmitRate( 10, 10 );
					}
				} break;
	#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			case CX2Unit::UT_ADD:
				{
					if ( pSeqUpgradeWeapon != NULL && pSeqUpgradeWeapon2 != NULL )		/// 오른쪽
					{
						D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Weapon02" );
						pSeqUpgradeWeapon->SetPosition( vBonePos );
						pSeqUpgradeWeapon2->SetPosition( vBonePos );

						pSeqUpgradeWeapon->SetEmitRate( 10, 10 );
						pSeqUpgradeWeapon2->SetEmitRate( 10, 10 );
					}

					if ( pSeqUpgradeWeapon3 != NULL && pSeqUpgradeWeapon4 != NULL )		/// 왼쪽
					{
						D3DXVECTOR3 vBonePos = m_pXSkinAnim->GetCloneFramePosition( L"Weapon05" );
						pSeqUpgradeWeapon3->SetPosition( vBonePos );
						pSeqUpgradeWeapon4->SetPosition( vBonePos );

						pSeqUpgradeWeapon3->SetEmitRate( 10, 10 );
						pSeqUpgradeWeapon4->SetEmitRate( 10, 10 );
					}
				} break;
	#endif //SERV_9TH_NEW_CHARACTER
			}
		}
	}
}

/// 강화 무기 이펙트 제거
void CX2UnitViewerUI::DeleteUpgradeWeaponParticle()
{
	if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon2 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon3 );
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqUpgradeWeapon4 );
	}
}

#endif ADD_UPGRADE_WEAPON_PARTICLE

#ifdef RIDING_SYSTEM
bool CX2UnitViewerUI::CanNotShowEnchantedWeaponEffectToRiding()
{
	if ( NULL != CX2RidingPetManager::GetInstance() && true != CX2RidingPetManager::GetInstance()->GetRidingOn() )
		return true; // 탈 것 미탑승 상태
	else
		return false; // 탈 것 탑승 상태
}
#endif //RIDING_SYSTEM

#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
bool CX2UnitViewerUI::IsPlayAvatarEmotionSoundWithouEmotion ( CX2Unit::EMOTION_TYPE eEmotionId )
{
	m_bIsPlayAvatarEmotionSoundWithoutEmotion = false;
	switch ( eEmotionId )
	{
	case CX2Unit::ET_EMOTION_AVATAR7 :
		for( int i=0; i<ARRAY_SIZE( ITEM_ID_EMOTION_AVATAR1 ); ++i )
		{
			const CX2Item::ItemTemplet* pItemTempet = g_pData->GetItemManager()->GetItemTemplet( ITEM_ID_EMOTION_AVATAR1[i] );

			if( NULL != pItemTempet )
			{
				CX2Item* pEquipedItem = m_pUnit->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_DEFENCE_BODY, true ); 
				if ( NULL != pEquipedItem && NULL != pEquipedItem->GetItemTemplet() )
				{
					if ( pEquipedItem->GetItemTemplet()->GetItemID () == pItemTempet->GetItemID() )
					{
						m_bIsPlayAvatarEmotionSoundWithoutEmotion = true;
						return true;
					}
				}
			}			
		}
		break;
	default :
		return true;
	}

	return false;
}
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// 크래용 팝 한벌 아바타 이모션, 사운드가 출력됨
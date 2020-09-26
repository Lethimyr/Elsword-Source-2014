#include "StdAfx.h"

#include ".\x2TFieldGame.h"


// constructor.
CX2TFieldNpc::CX2TFieldNpc( bool bBackgroundLoad ) :
m_bBackgroundLoad( bBackgroundLoad ),
m_bLoaded( false ),
m_bReservedShowObject( false ), 
m_bReservedCanTalkNpc( false ),
m_pDlgNpcMessage( NULL ), // kimhc // 2009-08-13 // NULL 초기화
m_bRanking( false )
//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
, m_bExchange( false )
#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
, m_bPrivateBank( false )
#endif PRIVATE_BANK
//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef GUILD_BOARD
, m_bGuild( false ) // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
#endif GUILD_BOARD

#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
, m_bWebEvent( false )
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD // oasis907 : 김상윤 [2010.3.12] //
, m_bPersonalShop( false )
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
, m_bAgencyTraderRegister( false )
, m_bAgencyTraderReceive( false )
#endif
#ifdef TALK_BOX_VILLAGE_NPC
, m_fElapsedTimeAfterDisableNPCTalkBox ( 0.f )
, m_bEnableNPCTalkBox( false )
, m_sTalkNum( 0 )
#endif
#ifdef NPC_EVENT_BUTTON
, m_bEvent1( false )
, m_bEvent2( false )
, m_bEvent3( false )
, m_bEvent4( false )
#endif NPC_EVENT_BUTTON
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
, m_bExchangeNewItem( false )
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_NEAR // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가
, m_bIsPlayNearSound ( false )
, m_wstrNearSoundFileName (L"")
, m_fPlayMaxNearSoundCoolTime ( 0.f )
, m_fPlayNowNearSoundCoolTime ( 0.f )
, m_fPlayNearSoundDistance ( 0.f )
, m_pNearPlaySound ( NULL )
#endif // ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_CLOSE // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가


{	
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );

	//RegisterLuaBind();

	m_pWorld	= NULL;	
	m_pHouseTemplet = NULL;
	m_pFontForUnitName		= NULL;

	m_pFontForUnitName = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );
    ASSERT( m_pFontForUnitName != NULL );

	m_NpcHouseID = 0;
	m_bTalk = false;
	m_bBye	= false;
	m_pXSkinAnim = NULL;
	m_pMesh = NULL;				
    m_pAniXET = NULL;
	m_iMotionId = 0;
	m_bPlay = false;
	m_bInArea = false;
	m_bJoinNpc = false;
	m_bOpenQuestReceive = false;

	m_NPCJoinArea = 500;

	m_vRot		= D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vScale	= D3DXVECTOR3(1.f, 1.f, 1.f);

	m_fJoinNpcTime = 0.f;
	m_fNpcMarkTime = 0.f;

	InitParticle();	
	m_pDlgNpcMessage = new CX2TFieldNpcShop();
    ASSERT( m_pDlgNpcMessage != NULL );
    if ( m_pDlgNpcMessage != NULL )
	    m_pDlgNpcMessage->SetShow(false);
	m_pUINPC = NULL;
	m_pUnitShadow				= NULL;

	m_bOpenedShop = false;


	m_bFirstAnim = true;
	m_vBasePos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_bStaticMesh = false;
	m_bShowPost = false;
	m_iNpcIndex = -1;

	m_eHouseType = CX2LocationManager::HT_INVALID;
	
//#ifdef NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST	
//	m_NPCModelName	= "";
//	m_bTryModelLoad = false;
//#endif NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST


	m_bEnable	= true;
	m_bPickNpc = false;


	m_nNewQuestRepeat = 0;
	m_nNewQuestNormal = 0;
	m_nEventQuest = 0;
	m_nDoQuest = 0;
	m_nCompleteQuestRepeat = 0;
	m_nCompleteQuestNormal = 0;
	m_nFairLvQuest = 0;

	m_wstrPlaySoundName = L"";

	m_bEventNpc = false;
	m_bCanTalkNpc = true;

	m_pAniXET	= NULL;

	m_bAttribute = false;
	m_bEnchant = false;
	m_bFreeTraining = false;
	m_bPvpNpc = false;
	m_bShopNpc = false;
	m_bMakingNpc = false;
	m_bTraining = false;
	
	//g_pKTDXApp->SkipFrame();	
}//CX2TFieldNpc::CX2TFieldNpc()


CX2TFieldNpc::~CX2TFieldNpc(void)
{
    ASSERT( ::GetCurrentThreadId() == g_pKTDXApp->GetMainThreadID() );
	
	m_pFontForUnitName = NULL;

	SAFE_DELETE_KTDGOBJECT( m_pXSkinAnim );

	SAFE_CLOSE( m_pMesh );		
	SAFE_CLOSE( m_pAniXET );

	SAFE_DELETE_KTDGOBJECT( m_pUnitShadow );
		
	g_pData->GetUIUnitManager()->ClearAllUIUnit();			// 왜 다 지우는걸까?

	SAFE_DELETE( m_pDlgNpcMessage );

	GetUiParticle()->DestroyInstanceHandle( m_hNpcInArea );
	//GetUiParticle()->DestroyInstanceHandle( m_hNpcInAreaBg );	
	GetUiParticle()->DestroyInstanceHandle( m_hNpcEvent );
	GetUiParticle()->DestroyInstanceHandle( m_hNpcNewQuest1 );
	GetUiParticle()->DestroyInstanceHandle( m_hNpcNewQuest2 );
	GetUiParticle()->DestroyInstanceHandle( m_hNpcDoQuest );
	GetUiParticle()->DestroyInstanceHandle( m_hNpcCompleteQuest1 );
	GetUiParticle()->DestroyInstanceHandle( m_hNpcCompleteQuest2 );

#ifdef ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_NEAR // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가
	if ( NULL != m_pNearPlaySound )
	{
		m_pNearPlaySound->Stop();
		SAFE_CLOSE ( m_pNearPlaySound );		
	}
#endif // ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_NEAR // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가

}//CX2TFieldNpc::~CX2TFieldNpc()

CKTDGParticleSystem* CX2TFieldNpc::GetUiParticle()
{
	return g_pTFieldGame->GetUiParticle();
}

void CX2TFieldNpc::InitParticle()
{

	//m_pInAreaParticle			= NULL;
	//m_pInAreaBgParticle			= NULL;
	//m_pEventParticle			= NULL;
	//m_pNewQuestParticle1		= NULL;
	//m_pNewQuestParticle2		= NULL;
	//m_pDoQuestParticle			= NULL;
	//m_pCompleteQuestParticle1	= NULL;
	//m_pCompleteQuestParticle2	= NULL;

	m_hNpcInArea			= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcInArea", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	//m_hNpcInAreaBg			= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcInAreaBg", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );	
	m_hNpcEvent				= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcEvent", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	m_hNpcNewQuest1			= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcNewQuest1", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	m_hNpcNewQuest2			= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcNewQuest2", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	m_hNpcDoQuest			= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcDoQuest", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	m_hNpcCompleteQuest1	= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcCompleteQuest1", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	m_hNpcCompleteQuest2	= GetUiParticle()->CreateSequenceHandle( NULL,  L"NpcCompleteQuest2", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	
	CKTDGParticleSystem::CParticleEventSequence* pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcInArea );
	if( pSeqPortal != NULL )
	{		
    	pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}
	//pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcInAreaBg );
	//if( pSeqPortal != NULL )
	//{		
	//	if( m_pInAreaBgParticle == NULL )
	//	{
	//		m_pInAreaBgParticle = pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
	//	}
	//	pSeqPortal->SetShowObject(false);
	//}

	
	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcEvent );
	if( pSeqPortal != NULL )
	{		
		pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}

	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest1 );
	if( pSeqPortal != NULL )
	{		
		pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}

	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest2 );
	if( pSeqPortal != NULL )
	{		
		pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}

	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcDoQuest );
	if( pSeqPortal != NULL )
	{		
		pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}

	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest1 );
	if( pSeqPortal != NULL )
	{		
		pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}

	pSeqPortal	= GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest2 );
	if( pSeqPortal != NULL )
	{		
		pSeqPortal->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
		pSeqPortal->SetShowObject(false);
	}
}

void CX2TFieldNpc::ResetNpc()
{
	m_bTalk = false;
	m_bBye	= false;		
	//m_bPlay = false;
	m_bJoinNpc = false;
	m_bInArea = false;
	m_bOpenQuestReceive = false;
	if(m_pUINPC != NULL)
		m_pUINPC->SetShowObject( false );
	if(m_pDlgNpcMessage != NULL)
		m_pDlgNpcMessage->SetShow(false);
}

void CX2TFieldNpc::SetShowField(bool val)
{
	SetShowObject(false);
	CKTDGParticleSystem::CParticleEventSequence* pSeq	= GetUiParticle()->GetInstanceSequence( m_hNpcInArea );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	//pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcInAreaBg );		
	//if(pSeq != NULL)
	//	pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcEvent );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest1 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest2 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcDoQuest );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest1 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest2 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);		

	if(m_pUINPC != NULL)
		m_pUINPC->SetShowObject( false );
	if(m_pDlgNpcMessage != NULL)
		m_pDlgNpcMessage->SetShow(false);
}

void CX2TFieldNpc::GetQuestInfo()
{
	// 퀘스트 마크 처리
	if(g_pData->GetQuestManager() != NULL)
	{
		int myLevel = g_pData->GetSelectUnitLevel();
		m_nFairLvQuest = g_pData->GetQuestManager()->GetNewQuest((CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex, m_nNewQuestRepeat, m_nNewQuestNormal, m_nEventQuest, myLevel );
		// oasis907 : 김상윤 [2010.8.3] // 수행중, 완료 가능 퀘스트 마크 없애기 (에픽퀘스트 개편으로 End NPC가 필요없어짐)
		//g_pData->GetQuestManager()->GetDoQuest((CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex, m_nDoQuest);
		//g_pData->GetQuestManager()->GetCompleteQuest((CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex, m_nCompleteQuestRepeat, m_nCompleteQuestNormal);
#ifdef QUEST_GUIDE
		g_pData->GetQuestManager()->GetCompleteTalkQuest( static_cast<CX2UnitManager::NPC_UNIT_ID>(m_iNpcIndex), m_nCompleteQuestNormal );
#endif //QUEST_GUIDE
	}
}

void CX2TFieldNpc::InitQuestMark()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq;
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcEvent );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest1 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest2 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcDoQuest );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest1 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);	
	pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest2 );		
	if(pSeq != NULL)
		pSeq->SetShowObject(false);
}

HRESULT	CX2TFieldNpc::OnFrameMove( double fTime, float fElapsedTime )
{	
	m_fJoinNpcTime += fElapsedTime;
	m_fNpcMarkTime += fElapsedTime;
    if ( m_bLoaded == false && m_pMesh != NULL )
    {
        CKTDXDevice::EDeviceState eState = m_pMesh->GetDeviceState();
        CKTDXDevice::EDeviceState eState2 = CKTDXDevice::DEVICE_STATE_LOADED;
        if ( m_pAniXET != NULL )
            eState2 = m_pAniXET->GetDeviceState();
        if ( eState == CKTDXDevice::DEVICE_STATE_LOADED 
            && eState2 == CKTDXDevice::DEVICE_STATE_LOADED )
        {
            SAFE_DELETE_KTDGOBJECT( m_pXSkinAnim );
            m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
            if ( m_pXSkinAnim != NULL )
            {
			    m_pXSkinAnim->SetApplyMotionOffset( false );
			    m_pXSkinAnim->SetAnimXSkinMesh( m_pMesh, m_pAniXET );
			    m_pXSkinAnim->AddModelXSkinMesh( m_pMesh, m_pAniXET );
			    m_pXSkinAnim->SetShowObject( m_bReservedShowObject );
			    m_pXSkinAnim->GetMatrix().Move( m_vPos );
			    m_pXSkinAnim->GetMatrix().RotateDegree( m_vRot );
			    m_pXSkinAnim->GetMatrix().Scale( m_vScale );	
            }//if
		    float fRadius = m_pMesh->GetBoundingRadius() * m_vScale.y;
		    SetBoundingRadius( fRadius );
	    	m_bLoaded = true;

            if ( m_bReservedCanTalkNpc == true )
            {
                SetCanTalkNpc( m_bCanTalkNpc, true );
            }//if

        }
        else if ( eState == CKTDXDevice::DEVICE_STATE_FAILED 
            || eState2 == CKTDXDevice::DEVICE_STATE_FAILED )
        {
            SAFE_CLOSE( m_pMesh );
            SAFE_CLOSE( m_pAniXET );
            CKTDGXSkinAnim::DeleteKTDGObject( m_pXSkinAnim );
        }//if.. else..
    }//if

	if(m_pXSkinAnim == NULL)
		return S_OK;

	if( false == m_bEnable )
		return S_OK;

	// 이벤트 관리	
	if(m_pHouseTemplet != NULL && m_pHouseTemplet->m_bEventNpc == true)
	{
		__time64_t t0 = g_pData->GetServerCurrentTime();
#ifdef	CONVERSION_VS
        struct tm   ktm;
        struct tm* ptm = &ktm;
        bool bOK = _localtime64_s( ptm, &t0 ) == 0;
#else   CONVERSION_VS
		struct tm* ptm = _localtime64( &t0 );
		bool bOK = ( ptm != NULL );
#endif  CONVERSION_VS
		if ( bOK == true )
		{
			unsigned long fNowSecond = (unsigned long)((float)ptm->tm_hour * 3600.f + (float)ptm->tm_min * 60.f + (float)ptm->tm_sec); 
		
			if( //m_pHouseTemplet->m_fStartYear >= ptm->tm_year && 
				m_pHouseTemplet->m_fStartMonth <= ptm->tm_mon+1 && 
				m_pHouseTemplet->m_fStartDay <= ptm->tm_mday &&
				m_pHouseTemplet->m_fStartHour <= ptm->tm_hour )
			{
				unsigned long ulActiveStep = (unsigned long)((m_pHouseTemplet->m_fActiveMin * 60.f) + (m_pHouseTemplet->m_fWaitMin * 60.f));
				unsigned long ulChunkTime = fNowSecond % ulActiveStep;
				if(ulChunkTime < (unsigned long)(m_pHouseTemplet->m_fActiveMin * 60.f))
				{
					// active
					SetCanTalkNpc(true);
				}
				else
				{
					// deactive & wait
					SetCanTalkNpc(false);
				}
			}
		}
	}


	D3DXVECTOR3 vInAreaPos = m_vBasePos;		
	vInAreaPos.y += 70.f;
	D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( vInAreaPos );

	// 퀘스트 정보 출력
	CKTDGParticleSystem::CParticleEventSequence* pSeqQuest = NULL;
	CKTDGParticleSystem::CParticleEventSequence* pSeqEvent = NULL;

	CKTDGParticleSystem::CParticleEventSequence* pSeqNpcInArea	= GetUiParticle()->GetInstanceSequence( m_hNpcInArea );

    if ( pSeqNpcInArea != NULL )
    {
	    if(m_NpcHouseID == CX2LocationManager::HI_POSTBOX)
		    pSeqNpcInArea->SetPosition( D3DXVECTOR3( finalPos.x, finalPos.y - 40.f, 0 ) );
	    else
		    pSeqNpcInArea->SetPosition( D3DXVECTOR3( finalPos.x, finalPos.y - 37.f, 0 ) );
    }//if


	InitQuestMark();

#ifdef DIALOG_SHOW_TOGGLE
	if( GetShowObject() == true && g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == false )
#else
	if(GetShowObject() == true)
#endif
	{		
		// 우편함은 퀘스트를 주지 않으므로 처리하지 않는다.
		if( m_NpcHouseID != CX2LocationManager::HI_POSTBOX 
//{{ kimhc // 2010.5.3 // 비밀던전 작업(퀘스트)
#ifndef SERV_SECRET_HELL
			&& m_NpcHouseID != CX2LocationManager::HI_BILLBOARD 
#endif SERV_SECRET_HELL
//}} kimhc // 2010.5.3 // 비밀던전 작업(퀘스트)
			)
		{
			bool bQuest = false;		


#ifdef SERV_EPIC_QUEST
//{{ 오현빈 // 2012-04-02 // 완료 퀘스트에 대한 안내가 부족하다는 피드백에 의해 퀘스트 버튼 노출 규칙 수정
			if ( false == m_pHouseTemplet->m_NPCList.empty() )
			{
				CX2UnitManager::NPC_UNIT_ID npcID = m_pHouseTemplet->m_NPCList[0];
				vector<int> vecCompletableQuestID;			
				//해당 NPC에게 완료 가능한 퀘스트가 있다면 퀘스트 버튼 노출
				g_pData->GetQuestManager()->GetCompletableQuest( static_cast<CX2UnitManager::NPC_UNIT_ID>(npcID), vecCompletableQuestID );
				if( false == vecCompletableQuestID.empty() )
				{
					bQuest = true;
				}
			}
//}} 오현빈 // 2012-04-02 // 완료 퀘스트에 대한 안내가 부족하다는 피드백에 의해 퀘스트 버튼 노출 규칙 수정
#endif SERV_EPIC_QUEST

			if(m_nCompleteQuestNormal > 0)
			{
				// 완료한 일반퀘스트가 존재
				pSeqQuest = GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest1 );
				bQuest = true;
			}
			else if(m_nCompleteQuestRepeat > 0)
			{
				// 완료한 반복퀘스트가 존재
				pSeqQuest = GetUiParticle()->GetInstanceSequence( m_hNpcCompleteQuest2 );
				bQuest = true;
			}
			else if(m_nFairLvQuest > 0) //if(m_nNewQuestNormal > 0)
			{
				// 수행가능한 일반퀘스트가 존재 (적정렙)
				pSeqQuest = GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest1 );
				bQuest = true;
			}
			else if(m_nNewQuestRepeat > 0)
			{
				// 수행가능한 반복퀘스트가 존재
				pSeqQuest = GetUiParticle()->GetInstanceSequence( m_hNpcNewQuest2 );
				bQuest = true;
			}
			else if(m_nDoQuest > 0)
			{
				// 수행중인 퀘스트가 존재
				pSeqQuest = GetUiParticle()->GetInstanceSequence( m_hNpcDoQuest );
				bQuest = true;
			}

			if(bQuest == true)
			{
				if(pSeqQuest != NULL)
				{
					pSeqQuest->SetPosition( D3DXVECTOR3( finalPos.x + 60.f, finalPos.y + 30.f, 0 ) );
					pSeqQuest->SetShowObject(true);
				}			
			}

			if(m_nEventQuest > 0)		
			{
				// 이벤트 퀘스트가 존재할 경우
				bQuest = true;
				pSeqEvent = GetUiParticle()->GetInstanceSequence( m_hNpcEvent );
                if ( pSeqEvent != NULL )
                {
				    pSeqEvent->SetPosition( D3DXVECTOR3( finalPos.x - 50.f, finalPos.y + 20.f, 0 ) );
				    pSeqEvent->SetShowObject(true);	
                }//if
			}

			// 퀘스트버튼 활성화를 위해 적정렙 무시한 수행가능퀘들을 검사한다.
			if(m_nNewQuestNormal > 0)
				bQuest = true;
#ifdef SERV_EPIC_QUEST
			if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_RECEIVE) == false &&
#else
			// 퀘스트가 열려있을경우 npc message 포커스 처리 하지 않는다.
			if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_RECEIVE) == false &&
#endif SERV_EPIC_QUEST
				NULL != m_pDlgNpcMessage
// #ifdef MODIFY_ACCEPT_QUEST
// 				 && CX2LocationManager::HI_BILLBOARD != m_NpcHouseID 
// #endif // MODIFY_ACCEPT_QUEST
				)
			{
				bool bQuestButton = m_pDlgNpcMessage->GetShopType(CX2TFieldNpcShop::NSBT_QUEST);
				if(bQuest == true)
				{	
					if(bQuestButton == false)
					{
						m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_QUEST);
						m_pDlgNpcMessage->SetKeyEvent();
					}			
				}
				else
				{
					if(bQuestButton == true)
					{
						m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_QUEST, false);
						m_pDlgNpcMessage->SetKeyEvent();
					}
				}
			}
		}		
	}




	if(m_bInArea == true && GetShowObject() == true)
	{
		
		//CKTDGParticleSystem::CParticleEventSequence* pSeq1	= GetUiParticle()->GetInstanceSequence( m_hNpcInAreaBg );		
		//pSeq1->SetPosition( D3DXVECTOR3( finalPos.x + 75.f, finalPos.y, 0 ) );			

		if(m_bJoinNpc == true)		
		{
			// npc에게 말건상태에서는 npc 마크 표시안함
            if ( pSeqNpcInArea != NULL )
			    pSeqNpcInArea->SetShowObject(false);
			//pSeq1->SetShowObject(false);

			if(pSeqQuest != NULL)
				pSeqQuest->SetShowObject(false);
			if(pSeqEvent != NULL)
				pSeqEvent->SetShowObject(false);
		}
		else
		{
			//npc 범위내에 존재할경우 'z대화하기' 표시
			if( m_bEventNpc == false || (m_bEventNpc == true && m_bCanTalkNpc == true) )
			{
				if(m_fNpcMarkTime <= 0.5f)
				{
                    if ( pSeqNpcInArea != NULL )
                    {
						if( m_NpcHouseID == CX2LocationManager::HI_POSTBOX ||
							m_NpcHouseID == CX2LocationManager::HI_BILLBOARD )
							pSeqNpcInArea->ChangeTexForce(L"One_Use_Up.dds");
						else
							pSeqNpcInArea->ChangeTexForce(L"Npc_Talk_UP.dds");
                    }//if
				}
				else if(m_fNpcMarkTime <= 1.f)
				{
                    if ( pSeqNpcInArea != NULL )
                    {
					    if( m_NpcHouseID == CX2LocationManager::HI_POSTBOX ||
						    m_NpcHouseID == CX2LocationManager::HI_BILLBOARD )
						    pSeqNpcInArea->ChangeTexForce( L"One_Use_Down.dds");
					    else
						    pSeqNpcInArea->ChangeTexForce( L"Npc_Talk_Down.dds");
                    }//if
				}
				else
				{
					m_fNpcMarkTime = 0.f;
                    if ( pSeqNpcInArea != NULL )
                    {
					    if( m_NpcHouseID == CX2LocationManager::HI_POSTBOX ||
						    m_NpcHouseID == CX2LocationManager::HI_BILLBOARD )
						    pSeqNpcInArea->ChangeTexForce( L"One_Use_Up.dds");
					    else
						    pSeqNpcInArea->ChangeTexForce( L"Npc_Talk_UP.dds");
                    }//if
				}

                if ( pSeqNpcInArea != NULL )
                {
				    if(m_bCanTalkNpc == true)
					    pSeqNpcInArea->SetShowObject(true);
				    else
					    pSeqNpcInArea->SetShowObject(false);
                }//if

				//pSeq1->SetShowObject(true);
				if(pSeqQuest != NULL)
					pSeqQuest->SetShowObject(true);
				if(pSeqEvent != NULL)
					pSeqEvent->SetShowObject(true);
			}
			else
			{
                if ( pSeqNpcInArea != NULL )
                {
				    if(m_bCanTalkNpc == true)
					    pSeqNpcInArea->SetShowObject(true);
				    else
					    pSeqNpcInArea->SetShowObject(false);
                }//if
			}
		}		
	}
	else
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq	= GetUiParticle()->GetInstanceSequence( m_hNpcInArea );		
        if ( pSeq != NULL )
		    pSeq->SetShowObject(false);
		//pSeq = GetUiParticle()->GetInstanceSequence( m_hNpcInAreaBg );		
		//pSeq->SetShowObject(false);
	}

	if(m_bJoinNpc == true)
	{
		// 유저가 벗어났는지 검사한다.

		if( m_fJoinNpcTime > 1.2f )
		{
			if(m_pUINPC != NULL && m_pUINPC->GetShowObject() == false)
			{
				// npc ui 표시					

				m_pUINPC->SetShowObject( true );				
				m_pUINPC->SetLayer(XL_LENS_FLARE); //SetOverUI(true);

				m_pUINPC->StateChange( m_pUINPC->GetWaitStateID() );			
				
				if ( m_pDlgNpcMessage != NULL )
				{
					m_pDlgNpcMessage->SetShow(true);				
	#ifdef SERV_PSHOP_AGENCY
		#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
					if( static_cast<CX2UnitManager::NPC_UNIT_ID>( m_iNpcIndex ) == CX2UnitManager::NUI_BILLBOARD)
		#else // SERV_UPGRADE_TRADE_SYSTEM
					if( (CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex == CX2UnitManager::NUI_MU)
		#endif // SERV_UPGRADE_TRADE_SYSTEM
					{
			#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
						wstring wstrRemainTime = GET_STRING( STR_ID_28380 );
			#else // SERV_UPGRADE_TRADE_SYSTEM
						wstring wstrRemainTime = GET_STRING( STR_ID_12235 );
			#endif // SERV_UPGRADE_TRADE_SYSTEM
						
						wstring wstrAgencyShopExpirationDate = g_pInstanceData->GetAgencyShopExpirationDate();
						if( wstrAgencyShopExpirationDate.empty() == false )
						{
							CTime	tCurrentTime	= CTime::GetCurrentTime();
							CTime	cAgencyTime;
							KncUtil::ConvertStringToCTime( wstrAgencyShopExpirationDate, cAgencyTime );
							if( tCurrentTime < cAgencyTime )
							{
		#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
								// 남은 시간에 따른 안내 문구 출력
								if ( NULL != g_pMain && g_pMain->GetNowState() )
									wstrRemainTime = static_cast<CX2State*>( g_pMain->GetNowState() )->GetExpirationDataDesc( g_pInstanceData->GetAgencyShopType(), wstrAgencyShopExpirationDate );
		#else // SERV_UPGRADE_TRADE_SYSTEM
								// 남은 시간
								wstrRemainTime = GET_REPLACED_STRING( ( STR_ID_12234, "L", GetExpirationDateDesc( wstrAgencyShopExpirationDate, g_pData->GetServerCurrentTime(), false ) ) );
		#endif // SERV_UPGRADE_TRADE_SYSTEM
							}	
						}		

						m_pDlgNpcMessage->SetNpcMessageWithEtc( m_strWaitTalk, wstrRemainTime );
					}
					else
					{
						m_pDlgNpcMessage->SetNpcMessage(m_strWaitTalk);
					}
	#else
					m_pDlgNpcMessage->SetNpcMessage(m_strWaitTalk);	
	#endif
					m_pDlgNpcMessage->SetKeyEvent();
				}

				m_bOpenedShop = false;	

#ifdef NPC_SOUND // npc 음성		
				if(m_pHouseTemplet != NULL)
				{
					wstring wstrSoundName = L"";
					if( m_pHouseTemplet->m_vecNPCJoinTalkSound.size() > 0 )
					{
						int iInx = 0;
						iInx = rand() % m_pHouseTemplet->m_vecNPCJoinTalkSound.size();
						wstrSoundName = m_pHouseTemplet->m_vecNPCJoinTalkSound[iInx];
						NpcPlaySound(wstrSoundName);				
					}					
				}
#endif

				Handler_EGS_TALK_WITH_NPC_REQ((CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex);
			}			

			if( m_NpcHouseID == CX2LocationManager::HI_POSTBOX )
			{
				if(g_pMain->GetPostBox()->GetDialogShow() == false && m_bShowPost == false)
				{
					g_pMain->GetPostBox()->SetDialogShow(true);
					m_bShowPost = true;
				}
				else if(g_pMain->GetPostBox()->GetDialogShow() == false && m_bShowPost == true)
				{
					m_bBye = true;
					m_bShowPost = false;
				}
				else if(g_pMain->GetPostBox()->GetDialogShow() == true && m_bShowPost == false)
				{
					m_bShowPost = true;
				}

				if( g_pMain->GetPostBox()->GetOpendSendWindow() == true && 
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
				{
					m_bShowPost = true;
				}
				else if( g_pMain->GetPostBox()->GetOpendSendWindow() == false && 
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
				{
					g_pMain->GetPostBox()->SetDialogShow(false);
					g_pData->GetUIManager()->CloseAllNPCDlg();
					m_bBye = true;
					m_bShowPost = false;
				}

				if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PERSONAL_TRADE ) == true ||		// 개인거래
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_CHARINFO ) == true ||			// 캐릭터정보
#ifdef SERV_EPIC_QUEST
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST_NEW ) == true ||
#endif SERV_EPIC_QUEST
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_QUEST ) == true ||				// 퀘스트
					g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_PARTY ) == true )				// 파티 (캐시샵 추가해야함)
				{					
					if(g_pMain->GetPostBox()->GetShow() == true)
					{
						g_pMain->GetPostBox()->SetDialogShow(false);
						g_pData->GetUIManager()->CloseAllNPCDlg();
						m_bBye = true;
						m_bShowPost = false;
					}				
				}					
			}

			//if(g_pData->GetCharacterRoom()->GetOpen() == true)
			////if(g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_SKILL ) == true)
			//{
			//	g_pData->GetUIManager()->CloseAll();				
			//	//ToggleMyInfo( CX2CharacterRoom::CRS_SKILL_TREE );
			//	m_bBye = true;				
			//}
		}


		if(m_pUINPC != NULL && m_pUINPC->GetShowObject() == true)
		{
			bool bOpendShop = false;
			
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SHOP) == true)
			{
                if ( m_pDlgNpcMessage != NULL )
				    m_pDlgNpcMessage->SetNpcMessage( m_strShopTalk );
				m_bOpenedShop = true;
				bOpendShop = true;
			}
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_MANUFACTURE) == true)
			{
                if ( m_pDlgNpcMessage != NULL )
				    m_pDlgNpcMessage->SetNpcMessage( m_strMakeTalk );
				m_bOpenedShop = true;
				bOpendShop = true;
			}

			//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PRIVATE_BANK) == true)
			{	
                if ( m_pDlgNpcMessage != NULL )
				    m_pDlgNpcMessage->SetNpcMessage( m_strPrivateBankTalk );
				m_bOpenedShop = true;
				bOpendShop = true;
			}
#endif PRIVATE_BANK
			//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef	ITEM_EXCHANGE_SHOP
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_ITEM_EXCHANGE_SHOP) == true)
			{	
                if ( m_pDlgNpcMessage != NULL )
				    m_pDlgNpcMessage->SetNpcMessage( m_strExchangeTalk );
				m_bOpenedShop = true;
				bOpendShop = true;
			}
#endif


#ifdef GUILD_BOARD
			//{{ oasis907 : 김상윤 [2010.2.2] // 길드 게시판
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_GUILD_BOARD) == true)
			{	
                if ( m_pDlgNpcMessage != NULL )
				    m_pDlgNpcMessage->SetNpcMessage( m_strGuildADTalk );
				m_bOpenedShop = true;
				bOpendShop = true;
			}
			//}}
#endif GUILD_BOARD

#ifdef DEF_TRADE_BOARD
			//{{ oasis907 : 김상윤 [2010.3.12] // 개인 상점 검색 시스템
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP_BOARD) == true)
			{	
                if ( m_pDlgNpcMessage != NULL )
                    m_pDlgNpcMessage->SetNpcMessage( m_strPersonalShopTalk );
				m_bOpenedShop = true;
				bOpendShop = true;
			}
			//}}
#endif DEF_TRADE_BOARD

#ifdef SERV_PSHOP_AGENCY
			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_PERSONAL_SHOP) == true)
			{	
				CX2UIPersonalShop *pPersonalShop = g_pData->GetUIManager()->GetUIPersonalShop();
				
				if( pPersonalShop != NULL )
				{
					switch( pPersonalShop->GetPersonalShopState() )
					{
					case CX2UIPersonalShop::XPSS_AGENCY_WAIT:
						{
							m_pDlgNpcMessage->SetNpcMessage( m_strRegisterTalk );
							m_bOpenedShop = true;
							bOpendShop = true;
						}
						break;
					case CX2UIPersonalShop::XPSS_AGENCY_SELL:
						{
							m_pDlgNpcMessage->SetNpcMessage( m_strRegisterTalk );
							m_bOpenedShop = true;
							bOpendShop = true;
						}
						break;
					case CX2UIPersonalShop::XPSS_AGENCY_TAKE:
						{
							m_pDlgNpcMessage->SetNpcMessage( m_strReceiveTalk );
							m_bOpenedShop = true;
							bOpendShop = true;
						}
						break;
					default:
						break;
					}
				}
			}
#endif

			if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_RANKING_INFO) == true)
			{	
                if ( m_pDlgNpcMessage != NULL )
				    m_pDlgNpcMessage->SetNpcMessage( m_strRankingTalk );
				m_bOpenedShop = true;
				bOpendShop = true;
			}
			if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_RECEIVE) == true ||
#ifdef SERV_EPIC_QUEST
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_NEW) == true 
#endif
				)
			{				
				if(m_bOpenQuestReceive == false)
				{
					m_bOpenQuestReceive = true;
                    if ( m_pDlgNpcMessage != NULL )
					    m_pDlgNpcMessage->SetNpcMessage( m_strQuestTalk );
				}				
				m_bOpenedShop = true;
				bOpendShop = true;
			}			
			else
			{
				m_bOpenQuestReceive = false;
			}
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			CX2Cursor* pCursor = static_cast< CX2State* >( g_pMain->GetNowState() )->GetCursor();
			if( NULL != pCursor )
			{
				if( CX2Cursor::XCS_EXCHANGE_NEW_ITEM == pCursor->GetCurorState() )
				{
					if( NULL != m_pDlgNpcMessage )
					{
						m_pDlgNpcMessage->SetNpcMessage( m_strExchangeNewItemTalk );
					}
				}
			}
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

			if(m_bOpenedShop == true && bOpendShop == false)
			{
				if ( m_pDlgNpcMessage != NULL )
				{
					m_pDlgNpcMessage->SetHouseName(m_strHouseName);

	#ifdef SERV_PSHOP_AGENCY

		#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
					if( static_cast<CX2UnitManager::NPC_UNIT_ID>( m_iNpcIndex ) == CX2UnitManager::NUI_BILLBOARD)
		#else // SERV_UPGRADE_TRADE_SYSTEM
					if( (CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex == CX2UnitManager::NUI_MU)
		#endif // SERV_UPGRADE_TRADE_SYSTEM
					{
			#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
						wstring wstrRemainTime = GET_STRING( STR_ID_28380 );
			#else // SERV_UPGRADE_TRADE_SYSTEM
						wstring wstrRemainTime = GET_STRING( STR_ID_12235 );
			#endif // SERV_UPGRADE_TRADE_SYSTEM
						
						wstring wstrAgencyShopExpirationDate = g_pInstanceData->GetAgencyShopExpirationDate();
						if( wstrAgencyShopExpirationDate.empty() == false )
						{
							CTime	tCurrentTime	= CTime::GetCurrentTime();
							CTime	cAgencyTime;
							KncUtil::ConvertStringToCTime( wstrAgencyShopExpirationDate, cAgencyTime );
							if( tCurrentTime < cAgencyTime )
							{
			#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
								// 남은 시간에 따른 안내 문구 출력
								if ( NULL != g_pMain && g_pMain->GetNowState() )
									wstrRemainTime = static_cast<CX2State*>( g_pMain->GetNowState() )->GetExpirationDataDesc( g_pInstanceData->GetAgencyShopType(), wstrAgencyShopExpirationDate );
			#else // SERV_UPGRADE_TRADE_SYSTEM
								// 남은 시간
								wstrRemainTime = GET_REPLACED_STRING( ( STR_ID_12234, "L", GetExpirationDateDesc( wstrAgencyShopExpirationDate, g_pData->GetServerCurrentTime(), false ) ) );
			#endif // SERV_UPGRADE_TRADE_SYSTEM
							}	
						}		
						m_pDlgNpcMessage->SetNpcMessageWithEtc( m_strByeTalk, wstrRemainTime );
					}
					else
					{
						m_pDlgNpcMessage->SetNpcMessage( m_strByeTalk );
					}
	#else
					m_pDlgNpcMessage->SetNpcMessage( m_strByeTalk );
	#endif
				}
				
				m_bOpenedShop = false;
			}
		}	
#ifdef QUEST_GUIDE
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetNpcIndicator() )
				{
					g_pTFieldGame->GetNpcIndicator()->SetShow(false);
				}
			} break;
		case CX2Main::XS_BATTLE_FIELD:
			{
				if( NULL != g_pX2Game && NULL != static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator() )
				{
					static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator()->SetShow(false);
				}
			} break;
		}
#endif //QUEST_GUIDE
	}

	if(m_bTalk == true )
	{
		if(m_bEventNpc == false)
		{
#ifdef EVENT_NPC_IN_VILLAGE
			if(m_NpcHouseID == CX2LocationManager::HI_EVENT_BENDERS )
			{
				m_pXSkinAnim->ChangeAnim( L"Wait", true );
			}
			else
#endif //EVENT_NPC_IN_VILLAGE
			if(m_bStaticMesh == false)
			{			
				m_pXSkinAnim->ChangeAnim( L"Talk", true );
			}
			else
			{			
				m_pXSkinAnim->ChangeAnim( L"normal", true );
			}

			m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );			
			m_bPlay = true;	
		}			
		m_bTalk = false;
		m_bJoinNpc = true;
		m_fJoinNpcTime = 0.f;
		// 상점진입

		//NPC대화중 숨겨야 하는 UI 설정
		g_pData->GetUIManager()->SetShowUI(false, true);
		// 파티창을 강제로 꺼준다
#ifdef SERV_PVP_NEW_SYSTEM
		g_pMain->GetPartyUI()->OpenPartyMenu( false );
#endif
		g_pMain->GetPartyUI()->OpenPartyDLG( false );

		// UI Unit 생성
		if( m_pUINPC == NULL )
		{
			if(m_bStaticMesh == false)
				m_pUINPC = g_pData->GetUIUnitManager()->GetUIUnit( m_iNpcIndex );
			else
				m_pUINPC = NULL;

			if( NULL != m_pUINPC )
			{			
				//m_pUINPC->Init();
				m_pUINPC->SetAlphaObject(true);								
				m_pUINPC->SetShowObject( false );
				//m_pUINPC->SetLayer(XDL_MESSENGER);
				//m_pUINPC->StateChange( m_pUINPC->GetWaitStateID() );
				g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUINPC );		
			}
		}

		if(m_pDlgNpcMessage != NULL)
		{
	#ifdef SERV_PSHOP_AGENCY
		#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			if( static_cast<CX2UnitManager::NPC_UNIT_ID>( m_iNpcIndex == CX2UnitManager::NUI_BILLBOARD ) )
		#else // SERV_UPGRADE_TRADE_SYSTEM
			if( (CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex == CX2UnitManager::NUI_MU)
		#endif // SERV_UPGRADE_TRADE_SYSTEM
			{
			#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
				wstring wstrRemainTime = GET_STRING( STR_ID_28380 );
			#else // SERV_UPGRADE_TRADE_SYSTEM
				wstring wstrRemainTime = GET_STRING( STR_ID_12235 );
			#endif // SERV_UPGRADE_TRADE_SYSTEM
				
				wstring wstrAgencyShopExpirationDate = g_pInstanceData->GetAgencyShopExpirationDate();
				if( wstrAgencyShopExpirationDate.empty() == false )
				{
					CTime	tCurrentTime	= CTime::GetCurrentTime();
					CTime	cAgencyTime;
					KncUtil::ConvertStringToCTime( wstrAgencyShopExpirationDate, cAgencyTime );
					if( tCurrentTime < cAgencyTime )
					{
			#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
						// 남은 시간에 따른 안내 문구 출력
						if ( NULL != g_pMain && g_pMain->GetNowState() )
							wstrRemainTime = static_cast<CX2State*>( g_pMain->GetNowState() )->GetExpirationDataDesc( g_pInstanceData->GetAgencyShopType(), wstrAgencyShopExpirationDate );
			#else // SERV_UPGRADE_TRADE_SYSTEM
						// 남은 시간
						wstrRemainTime = GET_REPLACED_STRING( ( STR_ID_12234, "L", GetExpirationDateDesc( wstrAgencyShopExpirationDate, g_pData->GetServerCurrentTime(), false ) ) );
			#endif // SERV_UPGRADE_TRADE_SYSTEM
					}	
				}

				m_pDlgNpcMessage->SetNpcMessageWithEtc( m_strWaitTalk, wstrRemainTime );
			}
			else
				m_pDlgNpcMessage->SetNpcMessage(m_strWaitTalk);
	#else
			m_pDlgNpcMessage->SetNpcMessage(m_strWaitTalk);
	#endif
		}

		// NPC ZoomIn Sound
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Camera_ZoomIn.ogg", false, false );
#ifdef SERV_EPIC_QUEST
		g_pData->GetUIManager()->GetUIQuestNew()->SetTalkingQuestNPC(true, (CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex);
#endif SERV_EPIC_QUEST
	}

	// NPC를 떠날 때
	if(m_bBye == true )
	{
		if(m_bEventNpc == false)
		{
#ifdef EVENT_NPC_IN_VILLAGE
			if(m_NpcHouseID == CX2LocationManager::HI_EVENT_BENDERS )
			{
				m_pXSkinAnim->ChangeAnim( L"Wait", true );
			}
			else
#endif //EVENT_NPC_IN_VILLAGE
			if(m_bStaticMesh == false)
				m_pXSkinAnim->ChangeAnim( L"Bye", true );
			else
				m_pXSkinAnim->ChangeAnim( L"normal", true );
			m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
			m_bPlay = true;
		}				
		m_bBye = false;
		m_bJoinNpc = false;
		// 상점나감

#ifdef NPC_SOUND // npc 음성
		if(m_pHouseTemplet != NULL)
		{
			wstring wstrSoundName = L"";

			if(m_pHouseTemplet->m_vecNPCByeTalkSound.size() > 0)
			{
				int iInx = 0;
				iInx = rand() % m_pHouseTemplet->m_vecNPCByeTalkSound.size();

				wstrSoundName = m_pHouseTemplet->m_vecNPCByeTalkSound[iInx];
				NpcPlaySound(wstrSoundName);
			}			
		}
#endif

		if( m_pUINPC != NULL )
		{
			//g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pUINPC );
			//m_pUINPC->SetShowObject( false );
			g_pData->GetUIUnitManager()->ClearAllUIUnit();
			m_pUINPC = NULL;
		}

        if ( g_pTFieldGame->GetNpcShadow() != NULL )
		    g_pTFieldGame->GetNpcShadow()->CloseShadow();

        if ( g_pTFieldGame->GetTalkBoxManager() != NULL )
		    g_pTFieldGame->GetTalkBoxManager()->SetShowTalk(true);
		//g_pTFieldGame->GetMinimap()->SetShowObject(true);


#ifndef REFORM_NOVICE_GUIDE
        if ( g_pTFieldGame->GetNoviceGuide() != NULL )
			g_pTFieldGame->GetNoviceGuide()->SetHide(false);
#endif //REFORM_NOVICE_GUIDE


#ifdef SERV_PVP_NEW_SYSTEM
		g_pMain->GetPartyUI()->OpenPartyMenu( true );
#endif
		if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 2 )
		{
			g_pMain->GetPartyUI()->OpenPartyFeverDLG( true );
			g_pMain->GetPartyUI()->UpdatePartyFeverDLG();
		}

		g_pData->GetUIManager()->SetShowUI(true);
		//{{ 허상형 : [2011/3/24/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
		//	월드미션 관련 창 켜기
// 		if( g_pData->GetWorldMissionManager()->IsEXPBonus() == true )
// 		{
// 			g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowEXPBonusDlg( true );
// 		}
		if( g_pData->GetWorldMissionManager()->IsActiveDefenceDungeon() == true )
		{
			g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( true );
		}
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 허상형 : [2011/3/24/] //	월드 미션

#ifdef SERV_GLOBAL_MISSION_MANAGER
		if( g_pData->GetGlobalMissionManager()->CheckShowGlobalMissionUI() == true )
		{
			g_pData->GetGlobalMissionManager()->GetUIGlobalMission()->SetShowGlobalMissionDlg( true );
		}
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef ADDED_RELATIONSHIP_SYSTEM
		// 종훈 : 이 부분 수정해야 된다.
		if ( NULL != g_pData->GetRelationshipEffectManager() )
			g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect(CX2RelationshipEffectManager::REST_SHOW_ALL);
#endif // ADDED_RELATIONSHIP_SYSTEM

		g_pChatBox->SetChatBoxLayerUp(false);
		g_pChatBox->OpenChatWindow();		
		g_pChatBox->CloseChatWindowButton();		
		g_pChatBox->HideChatEditBox();

		if( NULL != g_pData->GetMessenger() )
		{
			g_pData->GetMessenger()->SetHideWindow(false);
		}
        
		if( m_NpcHouseID == CX2LocationManager::HI_POSTBOX )
		{
			if(g_pMain->GetPostBox() != NULL)
				g_pMain->GetPostBox()->SetDialogShow(false);
		}
		
		//GetQuestInfo();
		if(m_pDlgNpcMessage != NULL)
		{
			m_pDlgNpcMessage->SetShow(false);
		}

#ifndef REFORM_NOVICE_GUIDE
		// 초보자 가이드
		if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL )
		{
			g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(0);
			if(g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP6)
			{
				if( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true)
				{
					g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(8);
				}
			}
		}
#endif //REFORM_NOVICE_GUIDE

		//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
		if ( g_pData->GetUIManager()->GetUITempInventory() != NULL &&
			g_pData->GetUIManager()->GetUITempInventory()->GetNumberOfItem() > 0 )
				g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_TEMP_INVENTORY, true );
#endif	REAL_TIME_ELSWORD
		//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

		// NPC ZoomOut Sound
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Camera_ZoomOut.ogg", false, false );

#ifdef SERV_EPIC_QUEST
		g_pData->GetUIManager()->GetUIQuestNew()->SetTalkingQuestNPC(false);
#endif SERV_EPIC_QUEST
#ifdef QUEST_GUIDE
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				if( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetNpcIndicator() )
				{
					g_pTFieldGame->GetNpcIndicator()->SetShow(true);
				}
			} break;
		case CX2Main::XS_BATTLE_FIELD:
			{
				if( NULL != g_pX2Game && NULL != static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator() )
				{
					static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetNpcIndicator()->SetShow(true);
				}
			} break;
		}
#endif //QUEST_GUIDE
	}

#ifdef TALK_BOX_VILLAGE_NPC
	if ( false == m_bEnableNPCTalkBox )
	{
		m_fElapsedTimeAfterDisableNPCTalkBox += fElapsedTime;

		if( m_fElapsedTimeAfterDisableNPCTalkBox > 10.f )
		{
			m_bEnableNPCTalkBox = true;
			m_fElapsedTimeAfterDisableNPCTalkBox = 0.f;
		}
	}
#endif

	if(m_pUINPC != NULL)
		m_pUINPC->OnFrameMove( fTime, fElapsedTime );

	if( m_bJoinNpc == true && m_bStaticMesh == false && m_pDlgNpcMessage != NULL)
	{
		m_pDlgNpcMessage->OnFrameMove( fTime, fElapsedTime );
	}

	if(m_bPlay == false && m_bStaticMesh == false)
	{
		if(m_bEventNpc == false)
		{
			switch(m_iMotionId)
			{
			case 0:
				m_pXSkinAnim->ChangeAnim( L"Wait01", true );
				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
#ifdef TALK_BOX_VILLAGE_NPC
				if ( m_vecNPCTalkBox.size() > 0 && true == m_bEnableNPCTalkBox && GetDistanceToCamera() < 3800 )
				{
					CX2TalkBoxManager* pTalkBoxMgr = NULL;
					if ( NULL != g_pTFieldGame )
					{
						pTalkBoxMgr = g_pTFieldGame->GetTalkBoxManager();

						if( NULL != pTalkBoxMgr )
						{
							CX2TalkBoxManagerImp::TalkBox talkBox;							
							talkBox.m_OwnerUnitUID		= m_NpcHouseID;
							//talkBox.m_GameUnitType		= CX2GameUnit::GUT_HOUSE;					

							talkBox.m_GameUnitType		= CX2GameUnit::GUT_NPC;
							talkBox.m_bExtraVariable	= true;
							
							talkBox.m_bTraceUnit		= true;
							talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_RIGHT;
							talkBox.m_wstrTalkContent	= m_vecNPCTalkBox[m_sTalkNum];
							pTalkBoxMgr->Push( talkBox );
						}
					}
					
					++m_sTalkNum;
					m_bEnableNPCTalkBox = false;
					if ( m_sTalkNum > (int) m_vecNPCTalkBox.size() - 1 )
						m_sTalkNum = 0;
				}
#endif

				break;
			case 1:
				m_pXSkinAnim->ChangeAnim( L"Wait02", true );
				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
				break;
			case 2:
				m_pXSkinAnim->ChangeAnim( L"Wait03", true );
				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
				break;
			default:
				m_pXSkinAnim->ChangeAnim( L"Wait01", true );
				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
				break;
			}			
		}
		else
		{
			// 이벤트NPC가 취할 모션
			if(m_bCanTalkNpc == false)
			{
				m_pXSkinAnim->ChangeAnim( L"Wait01", true );
				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );			
			}
			else
			{
				m_pXSkinAnim->ChangeAnim( L"Active_Wait01", true );
				m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );			
			}
			
		}
		
		m_bPlay = true;		
	}		
	else if(m_bPlay == false && m_bStaticMesh == true)
	{
		m_pXSkinAnim->ChangeAnim( L"Normal", true );
		m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );		
	}

#ifdef TALK_BOX_VILLAGE_NPC
	if ( m_bJoinNpc == false && m_bStaticMesh == true && 
		m_vecNPCTalkBox.size() > 0 && true == m_bEnableNPCTalkBox && 
		GetDistanceToCamera() < 3800 )
	{		 
		CX2TalkBoxManager* pTalkBoxMgr = NULL;
		{
			pTalkBoxMgr = g_pTFieldGame->GetTalkBoxManager();

			if( NULL != pTalkBoxMgr )
			{
				CX2TalkBoxManagerImp::TalkBox talkBox;							
				talkBox.m_OwnerUnitUID		= m_NpcHouseID;
				//talkBox.m_GameUnitType		= CX2GameUnit::GUT_HOUSE;					

				talkBox.m_GameUnitType		= CX2GameUnit::GUT_NPC;
				talkBox.m_bExtraVariable	= true;

				talkBox.m_bTraceUnit		= true;
				talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_RIGHT;
				talkBox.m_wstrTalkContent	= m_vecNPCTalkBox[m_sTalkNum];
				pTalkBoxMgr->Push( talkBox );
			}
		}

		++m_sTalkNum;
		m_bEnableNPCTalkBox = false;
		if ( m_sTalkNum > (int) m_vecNPCTalkBox.size() - 1 )
			m_sTalkNum = 0;
	}
#endif

	m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );

	if(m_bFirstAnim == true)
	{
		m_bFirstAnim = false;
		if(m_bStaticMesh == false)
		{
			m_vBasePos = GetHeadBonePos();
		}
		else
		{
			m_vBasePos = GetPos();
			m_vBasePos.y += 210.f;
		}
	}

	if( m_bPlay == true && m_pXSkinAnim->IsAnimationEnd() == true && m_bStaticMesh == false)
	{	
		m_bPlay = false;

		// 다음 애니메이션 설정
		if(m_bEventNpc == false)
		{
			int motionId = rand() % 9;	

			switch(motionId)
			{
			case 0:
				if(m_iMotionId == 1)
					m_iMotionId = 0;
				else
					m_iMotionId = 1;
				break;
			case 1:
				if(m_iMotionId == 2)
					m_iMotionId = 0;
				else
					m_iMotionId = 2;
				break;
			default:
				m_iMotionId = 0;
				break;
			}
		}
		else
		{
			// 이벤트NPC가 취할 모션결정
			m_iMotionId = 0;
		}		
	}


#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )

#if 0
	// 무조건 npc 머리 크게
	CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
	if(pFrame != NULL && pFrame->m_bScale == false)
	{
		D3DXVECTOR3 vScale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
		SetFrameScale(&vScale, true, pFrame);
	}
#else
	// 설정된 값에 따라 npc 머리 조절
	if( m_pXSkinAnim != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV && g_pInstanceData->GetFrameScale() == true)
	{
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
		if(pFrame != NULL && pFrame->m_bScale == false)
		{
			D3DXVECTOR3 vScale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
			SetFrameScale(&vScale, false, pFrame);
		}
	}
	else if( m_pXSkinAnim != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_DEV && g_pInstanceData->GetFrameScale() == false)
	{
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = m_pXSkinAnim->GetCloneFrame(L"Bip01_Head");
		if(pFrame != NULL && pFrame->m_bScale == true)
		{
			SetFrameScaleRestore(pFrame);
		}
	}
#endif
#endif

#ifdef ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_NEAR // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가
	if ( true == m_bIsPlayNearSound )
	{
		m_fPlayNowNearSoundCoolTime += fElapsedTime;
		if ( NULL != g_pTFieldGame->GetMyUnit() )
		{
			float fDistance3Sq = GetDistance3Sq( GetPos(), g_pTFieldGame->GetMyUnit()->GetPos() );
			if ( fDistance3Sq < m_fPlayNearSoundDistance * m_fPlayNearSoundDistance )
			{
				if ( m_fPlayNowNearSoundCoolTime >= m_fPlayMaxNearSoundCoolTime )
				{
					m_fPlayNowNearSoundCoolTime = 0;
					if ( NULL != m_pNearPlaySound && true == m_pNearPlaySound->IsPlaying() )
					{						
						;
					}
					else
					{
						m_pNearPlaySound = g_pKTDXApp->GetDeviceManager()->PlaySound( m_wstrNearSoundFileName.c_str(), false, false );
						if( m_pNearPlaySound != NULL )
						{
							m_pNearPlaySound->Set3DPosition( GetPos() );
							m_pNearPlaySound->SetMax3DDistance( 3000.f );
						}
					}
				}
				
				if ( NULL != m_pNearPlaySound && m_pNearPlaySound->IsPlaying() )
				{
					if ( true == g_pKTDXApp->GetDSManager()->GetSoundMute() )
					{
						g_pKTDXApp->GetDeviceManager()->StopSound( m_wstrNearSoundFileName.c_str() );
					}						
					else
					{
						const float MAGIC_BOUND = 3000.f;
						float fVolume = MAGIC_BOUND + g_pKTDXApp->GetDSManager()->GetSoundVolume();
						fVolume = (float) ( ( (int)fVolume ) % (int)MAGIC_BOUND );
						fVolume /= MAGIC_BOUND;
						m_pNearPlaySound->SetVolume( fVolume );
					}
				}
				
			}
			else
			{
				if ( NULL != m_pNearPlaySound )
				{
					g_pKTDXApp->GetDeviceManager()->StopSound( m_wstrNearSoundFileName.c_str() );
				}
			}
		}
	}
#endif // ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_CLOSE // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가

	return S_OK;
}


void CX2TFieldNpc::OnFrameRender_Draw()
{

	if( false == m_bEnable )
		return; 

    if ( m_pWorld == NULL )
        return;


	if( m_pXSkinAnim != NULL && GetShowObject() == true )
	{		
		CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();			
		pRenderParam->bZEnable			= true;
		//pRenderParam->bZWriteEnable		= true;
		pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
		//pRenderParam->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
		pRenderParam->lightPos			= m_pWorld->GetLightPos();
#ifdef UNIT_SCALE_COMBINE_ONE		// 해외팀 오류 수정
		pRenderParam->fOutLineWide		= CARTOON_OUTLINE_WIDTH;
#else //UNIT_SCALE_COMBINE_ONE
		pRenderParam->fOutLineWide		= 1.5f;
#endif //UNIT_SCALE_COMBINE_ONE
		pRenderParam->color				= 0xffffffff;
		pRenderParam->bAlphaBlend		= false;			

		if(m_bPickNpc == true && m_bJoinNpc == false)
		{
			pRenderParam->renderType		= CKTDGXRenderer::RT_CARTOON_COLOR_EDGE;
			pRenderParam->outLineColor		= 0xff11ff11;
		}
//  #ifdef ADDED_RELATIONSHIP_SYSTEM
//  		if ( CX2LocationManager::HI_BILLBOARD == m_NpcHouseID )
//  		{
//  			pRenderParam->renderType		= CKTDGXRenderer::RT_REAL_COLOR;
//  			pRenderParam->fOutLineWide		= 1.5f;
//  			pRenderParam->bAlphaBlend		= true;
//  			pRenderParam->srcBlend			= D3DBLEND_DESTALPHA;
//  			pRenderParam->destBlend			= D3DBLEND_INVSRCALPHA;
//  		}
//  #endif //ADDED_RELATIONSHIP_SYSTEM
// #ifdef DARKMOON_NPC
// 		if ( CX2LocationManager::HI_DARKMOON == m_NpcHouseID )
// 		{
// 			pRenderParam->renderType		= CKTDGXRenderer::RT_REAL_COLOR;
// 			pRenderParam->fOutLineWide		= 1.5f;
// 			pRenderParam->bAlphaBlend		= true;
// 			pRenderParam->srcBlend			= D3DBLEND_DESTALPHA;
// 			pRenderParam->destBlend			= D3DBLEND_INVSRCALPHA;
// 		}
// #endif	// DARKMOON_NPC

		m_pXSkinAnim->OnFrameRender();

		if( GetDistanceToCamera() <= 2300.f )
			RenderName();
	}
//#ifdef NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
//	else if( NULL == m_pXSkinAnim &&
//		false == m_bTryModelLoad )
//	{
//		m_bTryModelLoad = true;
//
//		//if( true == g_pKTDXApp->GetDeviceManager()->IsEnableThreadLoading() )
//		//{
//			HRESULT hr = g_pKTDXApp->GetDeviceManager()->ThreadReq_OpenDevice(
//				shared_from_this(), m_NPCModelName.c_str(), CKTDXDeviceManager::THREAD_REQUEST_UNKNOWN );
//			ASSERT( SUCCEEDED(hr) );
//		//}
//		//else
//		//{
//			//if(m_bStaticMesh == false)
//			//	m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_NPCModelName.c_str(), L"Npc_Pivot" );
//			//else
//			//	m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_NPCModelName.c_str() );				
//
//			//if( m_pMesh != NULL )
//			//{
//			//	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
//			//	m_pXSkinAnim->SetApplyMotionOffset( true );
//			//	m_pXSkinAnim->SetAnimXSkinMesh( m_pMesh, NULL, true );
//			//	m_pXSkinAnim->AddModelXSkinMesh( m_pMesh );
//			//	m_pXSkinAnim->SetShowObject(false);		
//
//			//	m_pXSkinAnim->GetMatrix().Move( m_vPos );
//			//	m_pXSkinAnim->GetMatrix().RotateDegree( m_vRot );
//			//	m_pXSkinAnim->GetMatrix().Scale( m_vScale );		
//			//}
//		//}
//	}
//#endif NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
}

bool CX2TFieldNpc::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
	
	return false;
}

HRESULT	CX2TFieldNpc::OnResetDevice()
{	
	return S_OK;
}

HRESULT	CX2TFieldNpc::OnLostDevice()
{
	return S_OK;
}


// #ifdef NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST
// 	/*virtual*/ bool CX2TFieldNpc::OnThreadLoad(DWORD dwParam_, DWORD dwParam2_)
// 	{
// 		if(m_bStaticMesh == false)
// 			m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_NPCModelName.c_str(), L"Npc_Pivot" );
// 		else
// 			m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_NPCModelName.c_str(), L"postbox");
// 
// 		if( m_pMesh != NULL )
// 		{
// 			//CKTDGXSkinAnim* pSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
// 			CKTDGXSkinAnim* pSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
// 
// 			pSkinAnim->SetApplyMotionOffset( true );
// 			pSkinAnim->SetAnimXSkinMesh( m_pMesh, NULL, true );
// 			pSkinAnim->AddModelXSkinMesh( m_pMesh );
// 			pSkinAnim->SetShowObject(false);
// 
// 			pSkinAnim->GetMatrix().Move( m_vPos );
// 			pSkinAnim->GetMatrix().RotateDegree( m_vRot );
// 			pSkinAnim->GetMatrix().Scale( m_vScale );
// 
//             ASSERT( m_pXSkinAnim == NULL );
// 			m_pXSkinAnim = pSkinAnim;
// 		}
// 
// 		return true;
// 	}
// #endif NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST


void CX2TFieldNpc::CreateFieldNpc(CX2World *pWorld, CX2LocationManager::HouseTemplet *pHouseTemplate)
{	
    ASSERT( pWorld != NULL );
    ASSERT( pHouseTemplate != NULL );

	m_pWorld = pWorld;
	m_pHouseTemplet = pHouseTemplate;

	m_eHouseType = pHouseTemplate->m_Type;

	m_NpcHouseID = (int)pHouseTemplate->m_ID;
	m_bTalk		= false;
	m_bBye		= false;
	
	m_vRot		= pHouseTemplate->m_NPCMeshRot;
	m_vScale	= pHouseTemplate->m_NPCMeshScal;
#ifdef LOCAL_NPC_JOIN_AREA
	bool bUseLocalJoinArea = false;
#endif LOCAL_NPC_JOIN_AREA


	if( pHouseTemplate->m_vecCommonPos.empty() )
	{
		m_vPos		= pHouseTemplate->m_NPCMeshPos;
	}
	else
	{
		//int villageID = g_pData->GetLocationManager()->GetCurrentVillageID();
		//CX2World::WORLD_ID eWorldID = g_pData->GetLocationManager()->GetWorldID( (SEnum::VILLAGE_MAP_ID) villageID );
		CX2World::WORLD_ID eWorldID = pWorld->GetWorldData()->worldID;
		
		for(UINT i=0; i<pHouseTemplate->m_vecCommonPos.size(); ++i)
		{
			if( eWorldID == pHouseTemplate->m_vecCommonPos[i].m_eWorldID )
			{
				m_vPos = pHouseTemplate->m_vecCommonPos[i].m_vNpcPos;
				m_vRot = pHouseTemplate->m_vecCommonPos[i].m_vNpcRot;
#ifdef LOCAL_NPC_JOIN_AREA
				if(pHouseTemplate->m_vecCommonPos[i].m_fLocalJoinArea > 0.f)
				{
					bUseLocalJoinArea = true;
					m_NPCJoinArea = (int)pHouseTemplate->m_vecCommonPos[i].m_fLocalJoinArea;
				}
#endif LOCAL_NPC_JOIN_AREA
				break;
			}
		}
	}
#ifdef LOCAL_NPC_JOIN_AREA
	if(bUseLocalJoinArea == false)
		m_NPCJoinArea = pHouseTemplate->m_NPCJoinArea;	
#else
	m_NPCJoinArea = pHouseTemplate->m_NPCJoinArea;		
#endif LOCAL_NPC_JOIN_AREA

	switch( pHouseTemplate->m_ID )
	{
	case CX2LocationManager::HI_POSTBOX:
	case CX2LocationManager::HI_CHRISTMAS_TREE:
	//case CX2LocationManager::HI_BILLBOARD:
		{
			m_bStaticMesh = true;
		} break;
	}

    m_bLoaded = false;

	if ( true == m_bBackgroundLoad )
    {
	    if( m_bStaticMesh == false)
			m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMeshInBackground( pHouseTemplate->m_NPCMeshName.c_str(), CKTDXDeviceManager::PRIORITY_HIGH, L"Npc_Pivot" );
	    else
		    m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMeshInBackground( pHouseTemplate->m_NPCMeshName.c_str(), CKTDXDeviceManager::PRIORITY_HIGH/*, "postbox"*/ );				

	    if( m_pMesh != NULL )
	    {
            //{{ seojt // 2009-1-12, 16:07
            //ASSERT( m_pXSkinAnim != NULL );
            CKTDGXSkinAnim::DeleteKTDGObject( m_pXSkinAnim );
            //}} seojt // 2009-1-12, 16:07
		    m_pXSkinAnim = NULL;
            m_pAniXET = g_pKTDXApp->GetDeviceManager()->OpenXETInBackground( pHouseTemplate->m_NPCAniTex.c_str(), CKTDXDeviceManager::PRIORITY_HIGH );

		    SetCenter( m_vPos );
	    }
    }
    else
    {
	    if(m_bStaticMesh == false)
			m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pHouseTemplate->m_NPCMeshName.c_str(), L"Npc_Pivot" );
	    else
		    m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pHouseTemplate->m_NPCMeshName.c_str()/*, "postbox"*/ );				

	    if( m_pMesh != NULL )
	    {
            //{{ seojt // 2009-1-12, 16:07
            //ASSERT( m_pXSkinAnim != NULL );
            CKTDGXSkinAnim::DeleteKTDGObject( m_pXSkinAnim );
            //}} seojt // 2009-1-12, 16:07
		    m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
		    if( m_pXSkinAnim != NULL )
		    {
			    m_pAniXET = g_pKTDXApp->GetDeviceManager()->OpenXET( pHouseTemplate->m_NPCAniTex.c_str() );

			    m_pXSkinAnim->SetApplyMotionOffset( false );
			    m_pXSkinAnim->SetAnimXSkinMesh( m_pMesh, m_pAniXET );
			    m_pXSkinAnim->AddModelXSkinMesh( m_pMesh, m_pAniXET );
			    m_pXSkinAnim->SetShowObject(false);		

			    m_pXSkinAnim->GetMatrix().Move( m_vPos );
			    m_pXSkinAnim->GetMatrix().RotateDegree( m_vRot );
			    m_pXSkinAnim->GetMatrix().Scale( m_vScale );	
		    }	

		    //SetCenter( m_pXSkinAnim->GetCenter() );
		    //SetCenter( m_pMesh->GetCenter() );
    		
		    SetCenter( m_vPos );
		    float fRadius = m_pMesh->GetBoundingRadius() * m_vScale.y;
		    SetBoundingRadius( fRadius );

            m_bLoaded = true;
	    }
    }//if.. else..

	if(m_bStaticMesh == false)
	{
		m_iNpcIndex = pHouseTemplate->m_NPCList[0];		
		m_strNpcName = g_pData->GetUnitManager()->GetNPCUnitTemplet( (CX2UnitManager::NPC_UNIT_ID)m_iNpcIndex )->m_Name;
	}
	else
	{
		m_iNpcIndex = -1;
		m_strNpcName = L"";
	}
	
	m_bEventNpc = pHouseTemplate->m_bEventNpc;

	m_bPvpNpc = pHouseTemplate->m_bPvpNpc;
	m_bMakingNpc = pHouseTemplate->m_bMakingNpc;
	m_bTraining = pHouseTemplate->m_bTraining;
	m_bFreeTraining = pHouseTemplate->m_bFreeTraining;
	m_bEnchant = pHouseTemplate->m_bEnchant;
	m_bAttribute = pHouseTemplate->m_bAttribute;	
#ifdef GUILD_BOARD
	m_bGuild = pHouseTemplate->m_bGuild;
#endif GUILD_BOARD
	m_bRanking= pHouseTemplate->m_bRanking;

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	m_bExchange = pHouseTemplate->m_bExchange;
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
	m_bPrivateBank	= pHouseTemplate->m_bPrivateBank;
#endif PRIVATE_BANK
	//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
	m_bWebEvent	= pHouseTemplate->m_bWebEvent;
#endif WEB_POINT_EVENT

#ifdef DEF_TRADE_BOARD
	m_bPersonalShop	= pHouseTemplate->m_bPersonalShop;
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
	m_bAgencyTraderRegister = pHouseTemplate->m_bAgencyTraderRegister;
	m_bAgencyTraderReceive = pHouseTemplate->m_bAgencyTraderReceive;
#endif
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	m_bSynthesisNpc	= pHouseTemplate->m_bSynthesis;
#endif SERV_SYNTHESIS_AVATAR
	//}}
	//{{ kimhc // 2009-12-08 // 판매하는 아이템 별로 카테고리가 자동으로 정해지도록 수정
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
	m_bShopNpc = pHouseTemplate->m_bSell;
#else
	if(pHouseTemplate->m_ShopCategoryTypeList.size() > 0)
		m_bShopNpc = true;
	else
		m_bShopNpc = false;
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
	//}} kimhc // 2009-12-08 // 판매하는 아이템 별로 카테고리가 자동으로 정해지도록 수정
#ifdef NPC_EVENT_BUTTON
	m_bEvent1 = pHouseTemplate->m_bEvent1;
	m_bEvent2 = pHouseTemplate->m_bEvent2;
	m_bEvent3 = pHouseTemplate->m_bEvent3;
	m_bEvent4 = pHouseTemplate->m_bEvent4;
#endif NPC_EVENT_BUTTON
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	 m_bExchangeNewItem = pHouseTemplate->m_bExchangeNewItem;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	// NPC UI 설정
	if(m_pDlgNpcMessage != NULL && m_bStaticMesh == false)
	{
		// npc 명찰 설정
		m_pDlgNpcMessage->CreateHouseNPCName(pHouseTemplate); 
		m_strHouseName = pHouseTemplate->m_HouseName;
		// npc가 가지는 event설정
		m_pDlgNpcMessage->SetNpcId(m_iNpcIndex, m_strHouseName);
		if(m_bPvpNpc)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_PVP);
		if(m_bShopNpc)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_SHOP);
		if(m_bMakingNpc)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_MAKE);
		if(m_bTraining)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_TRAINING);
		if(m_bFreeTraining)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_FREETRAIN);
		if(m_bEnchant)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_ENCHANT);
		if(m_bAttribute)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_ATTRIBUTE);
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] //
		if( m_bGuild )
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_GUILD);
#endif GUILD_BOARD
		if( m_bRanking )
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_RANKING);

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		if( m_bRanking ) //결혼식 이벤트 버튼은 랭킹 게시판과 함께 함.
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_WEDDING_EVENT);
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		if ( m_bExchange )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EXCHANGE );
		}
		
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
		if ( m_bPrivateBank )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_PRIVATE_BANK );
		}
#endif PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
		if ( m_bWebEvent )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_WEB_EVENT );
		}
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD // oasis907 : 김상윤 [2010.3.12] // 
		if ( m_bPersonalShop )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_PERSONAL_SHOP );
		}
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
		if( m_bAgencyTraderRegister )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_AGENCY_TRADER_REGISTER );
		}
		if( m_bAgencyTraderReceive )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_AGENCY_TRADER_RECEIVE );
		}
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		if ( m_bSynthesisNpc )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_SYNTHESIS );
		}
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
		if( m_bEvent1 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT1 );
		}

		if( m_bEvent2 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT2 );
		}

		if( m_bEvent3 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT3 );
		}

		if( m_bEvent4 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT4 );
		}
#endif NPC_EVENT_BUTTON

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		if( m_bExchangeNewItem )
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EXCHANGE_NEW_ITEM );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		m_strWaitTalk		= pHouseTemplate->m_strWaitTalk;
		m_strMakeTalk		= pHouseTemplate->m_strMakeTalk;
		m_strShopTalk		= pHouseTemplate->m_strShopTalk;
		m_strPvpTalk		= pHouseTemplate->m_strPvpTalk;
		m_strQuestTalk		= pHouseTemplate->m_strQuestTalk;
		m_strByeTalk		= pHouseTemplate->m_strByeTalk;
		m_strRankingTalk	= pHouseTemplate->m_strRankingTalk;
		m_strExchangeTalk	= pHouseTemplate->m_strExchangeTalk;
		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
		m_strPrivateBankTalk	= pHouseTemplate->m_strPrivateBank;
#endif PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef GUILD_BOARD
		//{{ oasis907 : 김상윤 [2010.2.2] // 
		m_strGuildADTalk = pHouseTemplate->m_strGuildADTalk;
		//}}
#endif GUILD_BOARD
#ifdef DEF_TRADE_BOARD
		//{{ oasis907 : 김상윤 [2010.3.31] // 
		m_strPersonalShopTalk = pHouseTemplate->m_strPersonalShopTalk;
		//}}
#endif DEF_TRADE_BOARD
	
#ifdef SERV_PSHOP_AGENCY
		m_strRegisterTalk = pHouseTemplate->m_strRegisterTalk;
		m_strReceiveTalk = pHouseTemplate->m_strReceiveTalk;
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		m_strSynthesisTalk		= pHouseTemplate->m_strSynthesisTalk;
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		m_strExchangeNewItemTalk = pHouseTemplate->m_strExchangeNewItemTalk;
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
		m_pDlgNpcMessage->SetKeyEvent();
	}
	

//#if 0
//	/*if( m_pUINPC != NULL )
//	{
//		m_pUINPC->SetShowObject( false );
//		g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pUINPC );		
//	}*/
//	//g_pData->GetUIUnitManager()->ClearAllUIUnit(); // 실시간 루아테스트를 위해서 추가...
//
//	if(m_bStaticMesh == false)
//		m_pUINPC = g_pData->GetUIUnitManager()->GetUIUnit( m_iNpcIndex );
//	else
//		m_pUINPC = NULL;
//	
//	if( NULL != m_pUINPC )
//	{			
//		//m_pUINPC->Init();
//		m_pUINPC->SetAlphaObject(true);								
//		m_pUINPC->SetShowObject( false );
//		//m_pUINPC->SetLayer(XDL_MESSENGER);
//		//m_pUINPC->StateChange( m_pUINPC->GetWaitStateID() );
//		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUINPC );		
//	}
//#endif

	// 그림자 설정
	if ( m_NpcHouseID != CX2LocationManager::HI_BILLBOARD )
	{
		//m_pUnitShadow = new CKTDGPlanShadow( 200 );
		m_pUnitShadow = CKTDGPlanShadow::CreatePlanShadow( 200 );
        ASSERT( m_pUnitShadow != NULL );
        if ( m_pUnitShadow != NULL )
        {
		    m_pUnitShadow->SetLayer( XL_EFFECT_0 );
		    m_pUnitShadow->SetShadowTexture( L"shadow.dds" );
		    m_pUnitShadow->UpdatePosition(m_vPos, m_vPos, m_vRot);
		    g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitShadow );	
        }

	}

	//{{ kimhc // 2009-12-04 // 크리스마스 이벤트
#ifdef	CHRISTMAS_TREE
	SetCanTalkNpc( pHouseTemplate->m_bCanTalkNpc );
#endif	CHRISTMAS_TREE
	//}} kimhc // 2009-12-04 // 크리스마스 이벤트
	
	GetQuestInfo();

#ifdef TALK_BOX_VILLAGE_NPC
	std::vector<wstring>::iterator itrBegin = pHouseTemplate->m_vecNPCTalkBox.begin();
	std::vector<wstring>::iterator itrEnd = pHouseTemplate->m_vecNPCTalkBox.end();
	
	for ( std::vector<wstring>::iterator itr = itrBegin; itr != itrEnd; ++itr )
	{
		m_vecNPCTalkBox.push_back( *itr );
	}
#endif

#ifdef ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_NEAR // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가
	if ( L"" != pHouseTemplate->m_wstrNearSoundFileName )
	{
		if ( NULL != g_pKTDXApp->GetDeviceManager() )
		{
//			bool bResult = g_pKTDXApp->GetDeviceManager()->SoundReadyInBackground( pHouseTemplate->m_wstrNearSoundFileName.c_str(), CKTDXDeviceManager::PRIORITY_HIGH );
	
//			if( true == bResult )
			{
				m_bIsPlayNearSound = true;
				m_wstrNearSoundFileName = pHouseTemplate->m_wstrNearSoundFileName;			
				m_fPlayNowNearSoundCoolTime = m_fPlayMaxNearSoundCoolTime = pHouseTemplate->m_fPlayNearSoundCoolTime;
				m_fPlayNearSoundDistance = pHouseTemplate->m_fPlayNearSoundDistance;
			}
		}
	}	

#endif // ADD_PLAY_MUSIC_WHEN_VILLAGE_NPC_CLOSE // 마을 NPC 에 일정 거리 이상 가까워지면 n초 간격으로 사운드를 출력하는 기능 추가

}

void CX2TFieldNpc::SetShowObject(bool val)
{
	if(m_pXSkinAnim != NULL)
		m_pXSkinAnim->SetShowObject(val);
    m_bReservedShowObject = val;
}

bool CX2TFieldNpc::GetShowObject()
{
	if( m_pXSkinAnim != NULL)
		return m_pXSkinAnim->GetShowObject();
	return m_bReservedShowObject;
}

void CX2TFieldNpc::RenderName()
{
#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif

	if ( g_pTFieldGame != NULL && GetShowObject() == true )
	{
	
		if( m_bStaticMesh == false )
		{		
			D3DXVECTOR3 vFontPos = m_vPos;
			vFontPos.y -= 15.f;
			//g_pTFieldGame->GetFont()->DrawProjectsText((WCHAR *)m_strNpcName.c_str(), vFontPos, 0xffffaa77, 0xff000000, DT_CENTER, CKTDGFontManager::FS_SHELL);
			//g_pTFieldGame->GetFont()->Draw3DText((WCHAR *)m_strNpcName.c_str(), vFontPos, 0xffffaa77, 0xff000000, DT_CENTER, CKTDGFontManager::FS_SHELL);
		
	        if ( g_pTFieldGame->GetFontForUnitName() != NULL )
			{
			    g_pTFieldGame->GetFontForUnitName()->OutProjectionText( vFontPos, m_strNpcName.c_str(), 0xffffaa77, 
				    CKTDGFontManager::FS_SHELL, 0xff000000, NULL, DT_CENTER  );

#ifdef NPCNAME_PLUS_US
				if ( m_NpcHouseID != CX2LocationManager::HI_BILLBOARD)
				{
					vFontPos.y -= 25.f;
					g_pTFieldGame->GetFontForUnitName()->OutProjectionText( vFontPos, m_strHouseName.c_str(), 0xff20B7ff, 
						CKTDGFontManager::FS_SHELL, 0xff000000, NULL, DT_CENTER );
				}
#endif //NPCNAME_PLUS_US
			}
		}
		else if( m_bStaticMesh == true )
		{
			D3DXVECTOR3 vFontPos = m_vPos;
			vFontPos.y -= 15.f;
			//g_pTFieldGame->GetFont()->DrawProjectsText((WCHAR *)m_strNpcName.c_str(), vFontPos, 0xffffaa77, 0xff000000, DT_CENTER, CKTDGFontManager::FS_SHELL);
			//g_pTFieldGame->GetFont()->Draw3DText((WCHAR *)m_strNpcName.c_str(), vFontPos, 0xffffaa77, 0xff000000, DT_CENTER, CKTDGFontManager::FS_SHELL);

			if ( m_NpcHouseID == CX2LocationManager::HI_POSTBOX )
	        {
	            if ( g_pTFieldGame->GetFontForUnitName() != NULL )
				    g_pTFieldGame->GetFontForUnitName()->OutProjectionText( vFontPos, GET_STRING( STR_ID_754 ), 0xffffaa77, 
				    CKTDGFontManager::FS_SHELL, 0xff000000, NULL, DT_CENTER  );
	        }//if
		}
	}
}

bool CX2TFieldNpc::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pDlgNpcMessage != NULL)
	{
		switch( m_pDlgNpcMessage->UICustomEventProc(hWnd, uMsg, wParam, lParam) )
		{
		case 1:  // NPC 대화 종료
			m_bBye = true;
			return true;
		case 2:
			return true;
		}
	}

	return false;
}

bool CX2TFieldNpc::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pDlgNpcMessage != NULL)
	{
		if(m_pDlgNpcMessage->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}

	switch( wParam)
	{
	case EGS_TALK_WITH_NPC_ACK:
		return Handler_EGS_TALK_WITH_NPC_ACK( hWnd, uMsg, wParam, lParam );
	}	

	return false;
}

CX2TFieldNpc::NPCTYPE CX2TFieldNpc::GetHouseType()
{
	switch(m_eHouseType)
	{
	case CX2LocationManager::HT_NPC_HOUSE:
		{
			if(m_bPvpNpc)
				return NT_PVP;
			if(m_bMakingNpc && m_NpcHouseID != CX2LocationManager::HI_GLAVE_HOUSE &&
				m_NpcHouseID != CX2LocationManager::HI_ARIEL_EVENT_HOUSE && 
				m_NpcHouseID != CX2LocationManager::HI_EVENT_FAIRY_GUARDIAN && 
				m_NpcHouseID != CX2LocationManager::HI_MANDRASSIL && 
				m_NpcHouseID != CX2LocationManager::HI_LUTO &&
				m_NpcHouseID != CX2LocationManager::HI_DAO && 
				m_NpcHouseID != CX2LocationManager::HI_APINK_ARCHANGEL
#ifdef RNW_NPC_IN_VILLAGE
				&& m_NpcHouseID != CX2LocationManager::HI_APINK
#endif
#ifdef EVENT_NPC_IN_VILLAGE
				&& m_NpcHouseID != CX2LocationManager::HI_EVENT_BENDERS
#endif //EVENT_NPC_IN_VILLAGE
#ifdef DARKMOON_NPC
				&& m_NpcHouseID != CX2LocationManager::HI_DARKMOON
#endif
#ifdef EVENT_NPC_STANDING_VILLAGE
				&& m_NpcHouseID != CX2LocationManager::HI_EVENT_SHEATH_NIGHT
				&& m_NpcHouseID != CX2LocationManager::HI_EVENT_INFINITY_SWORD
				&& m_NpcHouseID != CX2LocationManager::HI_EVENT_DIMENSION_WITCH
#endif EVENT_NPC_STANDING_VILLAGE
#ifdef ALWAYS_EVENT_LIRE_NIGHT_WATCHER_NPC
				&& m_NpcHouseID != CX2LocationManager::HI_EVENT_LIRE_NIGHT_WATCHER
#endif ALWAYS_EVENT_LIRE_NIGHT_WATCHER_NPC
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
				&& m_NpcHouseID != CX2LocationManager::HI_EVENT_ADAMS_UI_SHOP
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
				)
				return NT_ALCHEMIST;
			if(m_bTraining || m_bFreeTraining)
				return NT_TRAINNING;
		}
		return NT_NORMAL;
	case CX2LocationManager::HT_EQUIP_SHOP:
		return NT_EQUIP;
	case CX2LocationManager::HT_ACCESSORY_SHOP:
		return NT_ACCESSORY;
	case CX2LocationManager::HT_TRAINNING_CENTER:
		return NT_TRAINNING;
	case CX2LocationManager::HT_ALCHEMIST_HOUSE:
		return NT_ALCHEMIST;
	case CX2LocationManager::HT_POSTBOX:
		return NT_POSTBOX;        
	case CX2LocationManager::HT_BILLBOARD:		// 미니맵에서 사용
		return NT_RANKING;
	case CX2LocationManager::HT_PRIVATE_BANK:		// 미니맵에서 사용
		return NT_PRIVATE_BANK;

		//{{ kimhc // 2009-12-08 // 미니맵 Render 안함
	case CX2LocationManager::HT_EVENT:
		return NT_NO_RENDER_EVENT;
		//}} kimhc // 2009-12-08 // 미니맵 Render 안함

	case CX2LocationManager::HT_INVALID:
	default:
		return NT_NORMAL;
	}

	return NT_NORMAL;
}

bool CX2TFieldNpc::Handler_EGS_TALK_WITH_NPC_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID )
{
	KEGS_TALK_WITH_NPC_REQ kPacket;
	kPacket.m_iNPCID	= (int)questNPCID;	

	g_pData->GetServerProtocol()->SendPacket( EGS_TALK_WITH_NPC_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_TALK_WITH_NPC_ACK );

	return true;
}



bool CX2TFieldNpc::Handler_EGS_TALK_WITH_NPC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
    ASSERT( pBuff != NULL );
    if ( pBuff == NULL )
        return false;

	KEGS_TALK_WITH_NPC_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_TALK_WITH_NPC_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}

	return false;
}

void CX2TFieldNpc::ChangeType(CX2TFieldNpcShop::NPC_SHOP_BUTTON_TYPE eType)
{
	if(m_pDlgNpcMessage == NULL)
		return;

	m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_NONE, true);
	m_pDlgNpcMessage->SetShopType(eType, true);
}

void CX2TFieldNpc::SetCanTalkNpc(bool val, bool bForce )
{
	KTDXPROFILE();

	if( bForce == false && m_bCanTalkNpc == val)
		return;	

	//{{ kimhc // 2009-12-07 // 크리스마스 이벤트용
#ifdef	CHRISTMAS_TREE

	if ( m_NpcHouseID == CX2LocationManager::HI_CHRISTMAS_TREE
#ifdef DARKMOON_NPC
		|| m_NpcHouseID == CX2LocationManager::HI_DARKMOON
#endif
		)
	{
		m_bCanTalkNpc = val;
        if ( bForce == false )
            m_bReservedCanTalkNpc = true;
        else
            m_bReservedCanTalkNpc = false;
		return;
	}
#endif	CHRISTMAS_TREE
	//}} kimhc // 2009-12-07 // 크리스마스 이벤트용
    

	if(val == true)
	{
        if ( m_pXSkinAnim != NULL )
        {
		    m_pXSkinAnim->ChangeAnim( L"Active", true );
		    m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
		    m_bPlay = true;
        }//if
	}
	else
	{
		CKTDGUIDialogType pDialog = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"OkAndCancelMsgBox");
		if(pDialog != NULL && pDialog->GetShow() == true)		
			return;		

        if ( m_pXSkinAnim != NULL )
        {
		    m_pXSkinAnim->ChangeAnim( L"DeActive", true );
		    m_pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
		    m_bPlay = true;
        }//if

		if(g_pData != NULL && g_pData->GetUIManager() != NULL)
		{
			if(m_bJoinNpc == true )
			{
				g_pData->GetUIManager()->CloseAll();
#ifdef SERV_LOCAL_RANKING_SYSTEM
				if( NULL != g_pData->GetProfileManager() )
					g_pData->GetProfileManager()->CloseAll();
#endif //SERV_LOCAL_RANKING_SYSTEM
				m_bBye = true;
			}			
		}
	}

	m_bCanTalkNpc = val;

    if ( bForce == false )
        m_bReservedCanTalkNpc = true;
    else
        m_bReservedCanTalkNpc = false;
		
	return;
}

void CX2TFieldNpc::ResetNpcType()
{
	if( NULL == m_pDlgNpcMessage )
	{
		return;
	}
	m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_NONE, true);

	// NPC UI 설정
	if( false == m_bStaticMesh )
	{
		// npc 명찰 설정
		if(m_bPvpNpc)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_PVP);
		if(m_bShopNpc)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_SHOP);
		if(m_bMakingNpc)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_MAKE);
		if(m_bTraining)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_TRAINING);
		if(m_bFreeTraining)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_FREETRAIN);
		if(m_bEnchant)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_ENCHANT);
		if(m_bAttribute)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_ATTRIBUTE);
#ifdef GUILD_BOARD
		if( m_bGuild )
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_GUILD);
#endif GUILD_BOARD
		if( m_bRanking )
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_RANKING);

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		if( m_bRanking ) //결혼식 이벤트 버튼은 랭킹 게시판과 함께 함.
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_WEDDING_EVENT);
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		if ( m_bExchange )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EXCHANGE );
		}

#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
		if ( m_bPrivateBank )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_PRIVATE_BANK );
		}
#endif PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef WEB_POINT_EVENT
		if ( m_bWebEvent )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_WEB_EVENT );
		}
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
		if ( m_bPersonalShop )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_PERSONAL_SHOP );
		}
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
		if( m_bAgencyTraderRegister )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_AGENCY_TRADER_REGISTER );
		}
		if( m_bAgencyTraderReceive )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_AGENCY_TRADER_RECEIVE );
		}
#endif

		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		if(m_bMakingNpc)
			m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_MAKE);
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
		if( m_bEvent1 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT1 );
		}

		if( m_bEvent2 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT2 );
		}

		if( m_bEvent3 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT3 );
		}

		if( m_bEvent4 )
		{
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EVENT4 );
		}
#endif NPC_EVENT_BUTTON

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		if( m_bExchangeNewItem )
			m_pDlgNpcMessage->SetShopType( CX2TFieldNpcShop::NSBT_EXCHANGE_NEW_ITEM );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
	}
}

#ifdef NPC_SOUND
void CX2TFieldNpc::NpcPlaySound(wstring &wstrSoundName)
{
	g_pKTDXApp->GetDeviceManager()->StopSound( m_wstrPlaySoundName.c_str() );
	if(wstrSoundName.empty() == false)
	{	
#ifndef ONLY_NORMAL_SOUND_BY_NPC	/// 마을 NPC 목소리는 집중해서 들려줘야 하기 때문에, 3D 사운드 제거
		if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
			true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() )
		{
			CKTDXDeviceSound *pSound = g_pKTDXApp->GetDeviceManager()->PlaySound( wstrSoundName.c_str(), false, true );
			if( pSound != NULL )
			{
				pSound->Set3DPosition( GetPos() );
				pSound->SetMax3DDistance( 2000.f );
			}
		}
		else
#endif ONLY_NORMAL_SOUND_BY_NPC
		{
			g_pKTDXApp->GetDeviceManager()->PlaySound( wstrSoundName.c_str(), false, false );
		}

		

		m_wstrPlaySoundName = wstrSoundName;
	}
}
#endif

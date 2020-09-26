#include "StdAfx.h"
#ifndef REFORM_NOVICE_GUIDE
#include ".\x2NoviceGuide.h"

CX2NoviceGuide::CX2NoviceGuide() : 
m_pUiParticle(NULL),
m_pUiXMeshPlayer(NULL),
m_bNoviceGuide(false),
m_eGuideStep(NGS_END),
m_eOldGuideStep(NGS_END),
m_fElapsedTime(0.f),
m_eGuideState(GS_END),
m_fGuideTimer(0.f),
m_bShowArrow(true),
m_hGuideLeft(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_hGuideRight(INVALID_PARTICLE_SEQUENCE_HANDLE),
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
m_hGuideLeftParticle(INVALID_PARTICLE_HANDLE),
m_hGuideRightParticle(INVALID_PARTICLE_HANDLE),
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
m_pGuideLeftParticle(NULL),
m_pGuideRightParticle(NULL),
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
m_pUINPC(NULL),
m_pDLGNpcName(NULL),
m_pDlgNpcMessage(NULL),
m_iGuideMouseClick(0),
m_iPrevStep(0),
m_bHideMouseClick(false),
m_bPlayEventScene( false ),
m_pDlgEpicGuide( NULL ),
m_pDlgItemGuide( NULL )
{	
	if(g_pTFieldGame != NULL)
	{
		m_pUiParticle = g_pTFieldGame->GetUiParticle();
		m_pUiXMeshPlayer = g_pTFieldGame->GetUiXMeshPlayer();
	}

	m_bNoviceGuide = true;
	m_eGuideStep = NGS_STEP3;
	m_eOldGuideStep = NGS_STEP3;
	m_eGuideState = GS_START;
	
	CX2QuestManager::QuestInst* pQuestInstDungeon = NULL;
	pQuestInstDungeon = g_pData->GetQuestManager()->GetUnitQuest( 11010 );
	if( pQuestInstDungeon != NULL )		
	{		
		m_bNoviceGuide = true;
		m_eGuideStep = NGS_STEP4;		
		m_eGuideState = GS_START;
	}		

	if( m_bNoviceGuide == true && m_eGuideStep == NGS_STEP4 && 
		g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )
	{
		// 파티생성 및 던전플레이 시작
		m_eGuideStep = NGS_STEP5;
		m_eOldGuideStep = NGS_STEP5;
		m_eGuideState = GS_START;
	}

#ifdef SERV_EPIC_QUEST
	bool bFindQuest = false;
	std::vector<int> vecQuest;
	g_pData->GetQuestManager()->GetCompletableQuestEpic( vecQuest);
	for(UINT i=0; i<vecQuest.size(); ++i)
	{
		if(vecQuest[i] == 11010)
		{
			bFindQuest = true;
			break;
		}
	}
	
	if( bFindQuest == true )
	{			
		m_eGuideStep = NGS_STEP7;
		m_eOldGuideStep = NGS_STEP7;
		m_eGuideState = GS_START;		
	}
#endif
	
	// 가이드 완료
	if( g_pTFieldGame->GetMyUnit()->GetUnit()->GetUnitData().m_Level >= 5 ||
		g_pData->GetQuestManager()->GetUnitCompleteQuest(11010) == true )
	{
		m_bNoviceGuide = false;
		m_eGuideState = GS_END;
	}
}

CX2NoviceGuide::~CX2NoviceGuide(void)
{
	GetUiParticle()->DestroyInstanceHandle( m_hGuideLeft );	
	GetUiParticle()->DestroyInstanceHandle( m_hGuideRight );

	SAFE_DELETE(m_pDlgNpcMessage);
	SAFE_DELETE_DIALOG( m_pDLGNpcName );

	g_pMain->GetPartyUI()->SetNoviceGuideStep(3);
	
	g_pData->GetUIUnitManager()->ClearAllUIUnit();

	DeleteGuideDlg();
}


HRESULT	CX2NoviceGuide::OnFrameMove( double fTime, float fElapsedTime )
{		
	KTDXPROFILE();

	m_fElapsedTime = fElapsedTime;

	if( m_pDlgItemGuide != NULL && m_pDlgItemGuide->GetShow() == false )
	{
		SAFE_DELETE_DIALOG( m_pDlgItemGuide );
	}
	if( m_pDlgEpicGuide != NULL && m_pDlgEpicGuide->GetShow() == false )
	{
		SAFE_DELETE_DIALOG( m_pDlgEpicGuide );
	}	

	if(m_bNoviceGuide == true)
		ProcessGuide( fTime, fElapsedTime );	



	CX2State* pState = (CX2State*) g_pMain->GetNowState();
	if( true == pState->GetShowMouseClickUI() )
	{
		pState->MoveMouseClickUIToTop();
	}

	//if( g_pKTDXApp->GetDGManager()->GetDialogManager() != NULL )
	//{
	//	CKTDGUIDialogType pDlgMouseClick = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialog(L"DLG_Mouse_Click");
	//	if(pDlgMouseClick != NULL && pDlgMouseClick->GetShow() == true)
	//	{			
	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pDlgMouseClick, true );
	//	}
	//}

	return S_OK;
}

HRESULT	CX2NoviceGuide::OnFrameRender()
{	

	KTDXPROFILE();

	return S_OK;
}

bool CX2NoviceGuide::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	return false;
}

bool CX2NoviceGuide::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_pDlgNpcMessage != NULL && m_bNoviceGuide == true)
	{
		switch(m_pDlgNpcMessage->UICustomEventProc( hWnd, uMsg, wParam, lParam ))
		{
		case 0:
			return false;
		case 1:
		case 2:
			{
				// close
				if( m_pUINPC != NULL )
					m_pUINPC->SetShowObject(false);

				if( NULL != m_pDLGNpcName )
				{
					m_pDLGNpcName->SetShow(false);
				}

				//SAFE_DELETE(m_pDlgNpcMessage);
				//SAFE_DELETE_DIALOG( m_pDLGNpcName );

				g_pTFieldGame->SetEnableKeyProcess(true);
				g_pTFieldGame->SetUserHide(false);


				g_pData->GetUIUnitManager()->ClearAllUIUnit();
				m_pUINPC = NULL;

				if(m_eGuideStep == NGS_STEP1)
				{				
					m_eGuideState = GS_END;

#ifdef CHECK_INHERIT_STATE_MENU
					if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
					{
						CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
						if( NULL != pStateMenu )
						{
							SetMouseClickIndex(0);
						}
					}
				}
				else if(m_eGuideStep == NGS_STEP5)
				{
					SetNotifyGuide(GE_NEXT);
				}
			}
			return true;
		default:
			return false;	
		}
	}

	return false;
}

bool CX2NoviceGuide::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	

	return false;
}

void CX2NoviceGuide::SetNotifyGuide(CX2NoviceGuide::GUIDE_EVENT event)
{
	if(m_bNoviceGuide == false)
		return;

	switch(m_eGuideStep)
	{
	case NGS_STEP1:
		{
			if(event == GE_ACCEPT)
			{
				m_iPrevStep = 4;
				m_eGuideState = GS_END;
				return;
			}
			else if(event == GE_PREV)
			{				
				m_iPrevStep = -1;
				m_eGuideState = GS_END;
				return;
			}
			// 1단계 퀘스트 수락		

			// 초기화
			ASSERT( NULL != m_pUINPC );		// dmlee, 2009.03.21 헬렌 상점에서 빠져나올때 뻗는 문제 때문에 null check 추가
			if( NULL != m_pUINPC )
			{
				m_pUINPC->SetShowObject(false);
			}
			if( NULL != m_pDLGNpcName )
			{
				m_pDLGNpcName->SetShow(false);		
			}

			g_pTFieldGame->SetEnableKeyProcess(true);
			g_pTFieldGame->SetUserHide(false);

			
			g_pData->GetUIUnitManager()->ClearAllUIUnit();
			m_pUINPC = NULL;
			m_eGuideState = GS_END;
		}
		break;
	case NGS_STEP2:
		{
#if 0 
			if(event == GE_ACCEPT)
			{
				m_iPrevStep = 4;
				m_eGuideState = GS_END;
				return;
			}
			else if(event == GE_PREV)
			{				
				m_iPrevStep = -1;
				m_eGuideState = GS_END;
				return;
			}	
#endif		
			if(event == GE_NEXT)
			{
				int MAGIC_QUEST_ID = 11000;
				CX2QuestManager::QuestInst* pQuestInst = NULL;
				pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( MAGIC_QUEST_ID );
				if( NULL == pQuestInst )
				{
					SetMouseClickIndex(0);
					m_eGuideState = GS_END;
					return;
				}
				else
				{
					return;
				}
			}

			// 뽀루윌리엄의 장난 퀘 완료시 가이드 종료
			if(g_pData->GetQuestManager()->GetUnitCompleteQuest(11010) == true)
			{
				m_bNoviceGuide = false;
				m_eGuideState = GS_END;
			}
			else
			{
				//g_pTFieldGame->ByeNpc();
				g_pData->GetUIManager()->CloseAllNPCDlg();
				g_pData->GetUIManager()->CloseAll();

				SetMouseClickIndex(5);
				g_pTFieldGame->ChangeNpcType(CX2TFieldNpcShop::NSBT_CLOSE);
				g_pTFieldGame->SetNpcMessage(CX2UnitManager::NUI_ARIEL, GET_STRING( STR_ID_369 ) );

				SetHide(true);
			}						
		}
		break;
	case NGS_STEP3:
		{
			switch(event)
			{
			case GE_ACCEPT:
				{
					m_iPrevStep = 4;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = -1;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_COMPLETE:
				return;
			}

			SetMouseClickIndex(0);
			g_pTFieldGame->ByeNpc();
			g_pData->GetUIManager()->CloseAllNPCDlg();
			g_pData->GetUIManager()->CloseAll();
			m_eGuideState = GS_END;
		}
		break;
	case NGS_STEP4:
		{
			switch(event)
			{
			case GE_ACCEPT:
				{					
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = -1;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_COMPLETE:
				return;				
			}			
		}
		break;
	case NGS_STEP5:
		{
			switch(event)
			{
			case GE_COMPLETE:
				{
					// 초기화				
					g_pMain->GetPartyUI()->SetNoviceGuideStep(4);
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_ACCEPT:
				{
					SAFE_DELETE(m_pDlgNpcMessage);
					SAFE_DELETE_DIALOG( m_pDLGNpcName );					
					return;
				}
				break;
			case GE_NEXT:
				{
					// 초기화			
					if(g_pData != NULL && g_pData->GetPartyManager() != NULL && g_pData->GetPartyManager()->DoIHaveParty() == true)
					{
						// 참가해있다면 시작하기/준비하기 가이드
						m_eGuideStep = NGS_STEP6;
						m_eOldGuideStep = NGS_STEP6;
						m_eGuideState = GS_START;
						g_pMain->GetPartyUI()->SetNoviceGuideStep(1);
					}
					else
					{
						m_eGuideState = GS_END;
						g_pMain->GetPartyUI()->SetNoviceGuideStep(4);
					}					
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = 3;
					m_eGuideState = GS_END;
					return;
				}
				break;
			

			default:
				break;
			}
		}
		break;
	case NGS_STEP6_1:
		{
			switch(event)
			{			
			case GE_ACCEPT:
				{			
					// 임시로 GE_ACCEPT를 back으로 처리한다.
					m_iPrevStep = 6;	// 맵선택					
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = 3;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_NEXT:
				{
					// 초기화		
					m_eGuideState = GS_END;
					g_pMain->GetPartyUI()->SetNoviceGuideStep(1);
					return;
				}
				break;				
			case GE_COMPLETE:
				{
					// 초기화				
					g_pMain->GetPartyUI()->SetNoviceGuideStep(5);
					m_eGuideState = GS_END;
					return;
				}
				break;		
			}			
		}
		break;
	case NGS_STEP6_2:
		{
			switch(event)
			{			
			case GE_ACCEPT:
				{			
					// 임시로 GE_ACCEPT를 back으로 처리한다.
					m_iPrevStep = 6;	// 맵선택
					g_pMain->GetPartyUI()->SetNoviceGuideStep(4);
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = 3;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_NEXT:
				{
					// 초기화			
					m_eGuideState = GS_END;
					g_pMain->GetPartyUI()->SetNoviceGuideStep(1);
					return;
				}
				break;
			case GE_COMPLETE:
				{
					// 초기화				
					g_pMain->GetPartyUI()->SetNoviceGuideStep(6);
					m_eGuideState = GS_END;
					return;
				}
				break;				
			}			
		}
		break;
	case NGS_STEP6_3:
		{
			switch(event)
			{			
			case GE_ACCEPT:
				{			
					// 임시로 GE_ACCEPT를 back으로 처리한다.
					m_iPrevStep = 7;	// 맵선택
					g_pMain->GetPartyUI()->SetNoviceGuideStep(5);
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = 3;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_NEXT:
				{
					// 초기화
					m_eGuideState = GS_END;
					g_pMain->GetPartyUI()->SetNoviceGuideStep(1);
					return;
				}
				break;
			case GE_COMPLETE:
				{
					// 초기화				
					g_pMain->GetPartyUI()->SetNoviceGuideStep(1);
					m_eGuideState = GS_END;
					return;
				}
				break;				
			}			
		}
		break;
	case NGS_STEP6:
		{
			switch(event)
			{
			case GE_ACCEPT:
				{			
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = 3;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_COMPLETE:
				m_eGuideState = GS_END;
				return;				
			}			
		}
		break;
	case NGS_STEP7:
		{
			switch(event)
			{
			case GE_PREV:
				{
					m_iPrevStep = 3;					
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_COMPLETE:
				SetMouseClickIndex(0);
				//m_bNoviceGuide = false;
				m_eGuideState = GS_END;				
				return;				
			}			
		}
		break;
	case NGS_STEP8:
		{
			switch(event)
			{
			case GE_ACCEPT:
				{					
					return;
				}
				break;
			case GE_PREV:
				{
					m_iPrevStep = 3;
					m_eGuideState = GS_END;
					return;
				}
				break;
			case GE_COMPLETE:
				{
					SetMouseClickIndex(0);
					m_bNoviceGuide = false;
					m_eGuideState = GS_END;
				}				
				return;				
			}			
		}
		break;
	}
}

void CX2NoviceGuide::SetPrevStep()
{
	//가이드 이전 단계로 설정
	if(m_iPrevStep == -1)
	{
		// 이전가이드로 돌려야하는 상황		
		m_eGuideStep = m_eOldGuideStep;
		m_eGuideState = GS_START;
		m_iPrevStep = 0;
		return;
	}

	if(m_eGuideStep == NGS_STEP1 || m_bNoviceGuide == false)
		return;
	int tempStep = m_eGuideStep;
	m_eOldGuideStep = m_eGuideStep;
	m_eGuideStep = (CX2NoviceGuide::NOVICE_GUIDE_STEP)(--tempStep);

	m_eGuideState = GS_START;
}

void CX2NoviceGuide::SetNextStep()
{
	if(m_iPrevStep == -1)
	{
		// 이전가이드로 돌려야하는 상황		
		SetPrevStep();
		return;
	}
	else if(m_iPrevStep > 0)
	{
		// 가이드 단계 건너뛴다.
		m_eOldGuideStep = m_eGuideStep;
		m_eGuideStep = (CX2NoviceGuide::NOVICE_GUIDE_STEP)m_iPrevStep;
		m_iPrevStep = 0;
		m_eGuideState = GS_START;
		return;
	}

	// 가이드 다음 단계로 설정
	if(m_eGuideStep == NGS_END || m_bNoviceGuide == false)
		return;
	int tempStep = m_eGuideStep;
	m_eOldGuideStep = m_eGuideStep;
	m_eGuideStep = (CX2NoviceGuide::NOVICE_GUIDE_STEP)(++tempStep);
	
	m_eGuideState = GS_START;
}

bool CX2NoviceGuide::ProcessGuide(double fTime, float fElapsedTime)
{
	if(m_bNoviceGuide == false)
		return false;

	switch(m_eGuideStep)
	{
	case NGS_STEP1:
		return DoGuideStep1( fTime, fElapsedTime );
	case NGS_STEP2:
		return DoGuideStep2();
	case NGS_STEP3:
		return DoGuideStep3();
	case NGS_STEP4:
		return DoGuideStep4();
	case NGS_STEP5:
		return DoGuideStep5( fTime, fElapsedTime );
	case NGS_STEP6:
		return DoGuideStep6();
	case NGS_STEP6_1:
		return DoGuideStep6_1();
	case NGS_STEP6_2:
		return DoGuideStep6_2();
	case NGS_STEP6_3:
		return DoGuideStep6_3();
	case NGS_STEP7:
		return DoGuideStep7( fTime, fElapsedTime );
	case NGS_STEP8:
		return DoGuideStep8();
	}

	return false;
}

void CX2NoviceGuide::CreateNpc()
{
	SAFE_DELETE(m_pDlgNpcMessage);
	SAFE_DELETE_DIALOG( m_pDLGNpcName );

	m_pUINPC = g_pData->GetUIUnitManager()->GetUIUnit( CX2UnitManager::NUI_ARIEL );
	if( NULL != m_pUINPC )
	{
		m_pUINPC->SetAlphaObject(true);										
		m_pUINPC->StateChange( m_pUINPC->GetWaitStateID() );
		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUINPC );		
		m_pUINPC->SetShowObject( true );
		m_pUINPC->SetCameIn( false );
						
		m_pUINPC->SetLayer(XL_LENS_FLARE);
		m_pUINPC->StateChange( m_pUINPC->GetWaitStateID() );			
	}	

	// 대사창 생성
	m_pDlgNpcMessage = new CX2TFieldNpcShop();	

	// NPC 명찰
	SAFE_DELETE_DIALOG( m_pDLGNpcName );
	m_pDLGNpcName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_House_NPC_Name_Templete.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGNpcName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGNpcName, false );		

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGNpcName->GetControl(L"StaticHouseNPCName");
	if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
	{
		// 원래는 HouseTemplate에서 가져와야 하지만 뜬금없이 나타나는 NPC는 아리엘 뿐이므로 하드코딩해둔다.
		pStatic->GetPicture(0)->SetTex( L"dlg_common_new_texture72.tga", L"NPC_ARIEL" );
	}	

	// NPC 명찰 위치 지정
	D3DXVECTOR2 vPos = D3DXVECTOR2(134.f + 600.f, 641.f);
	m_pDLGNpcName->SetPos( vPos );

	m_pDLGNpcName->SetShowEnable(true, true);
}

void CX2NoviceGuide::CreateArrow()
{
	m_fGuideTimer = 0.f;		
	if( m_hGuideLeft == INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hGuideLeftParticle = INVALID_PARTICLE_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_hGuideLeft = GetUiParticle()->CreateSequenceHandle( NULL,  L"LeftGuide", 0.0f, 0.0f, 0.0f);
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideL	= GetUiParticle()->GetInstanceSequence( m_hGuideLeft );
		if( pSeqGuideL != NULL )
		{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hGuideLeftParticle = pSeqGuideL->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pGuideLeftParticle = pSeqGuideL->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			pSeqGuideL->SetShowObject(false);
		}
	}

	if( m_hGuideRight == INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        m_hGuideRightParticle = INVALID_PARTICLE_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_hGuideRight = GetUiParticle()->CreateSequenceHandle( NULL,  L"RightGuide", 0.0f, 0.0f, 0.0f);
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideR	= GetUiParticle()->GetInstanceSequence( m_hGuideRight );
		if( pSeqGuideR != NULL )
		{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hGuideRightParticle = pSeqGuideR->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pGuideRightParticle = pSeqGuideR->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			pSeqGuideR->SetShowObject(false);
		}	
	}		

	if(g_pTFieldGame != NULL && g_pTFieldGame->GetJoinNpc() == true)
		SetHide(true);
	else
		SetHide(false);
}

void CX2NoviceGuide::SetHide(bool val)
{
	m_bShowArrow = !val;
	
	if(m_bNoviceGuide == true)
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideL	= GetUiParticle()->GetInstanceSequence( m_hGuideLeft );
		if( pSeqGuideL != NULL )
		{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( m_hGuideLeftParticle == INVALID_PARTICLE_HANDLE )
                m_hGuideLeftParticle = pSeqGuideL->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( m_pGuideLeftParticle == NULL )
                m_pGuideLeftParticle = pSeqGuideL->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideR	= GetUiParticle()->GetInstanceSequence( m_hGuideRight );
		if( pSeqGuideR != NULL )
		{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( m_hGuideRightParticle == INVALID_PARTICLE_HANDLE )
				m_hGuideRightParticle = pSeqGuideR->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( m_pGuideRightParticle == NULL )
				m_pGuideRightParticle = pSeqGuideR->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}

		if(m_bShowArrow == true && pSeqGuideL != NULL && pSeqGuideR != NULL)
		{
			pSeqGuideL->SetShowObject(true);
			pSeqGuideR->SetShowObject(true);
		}
		else if(pSeqGuideL != NULL && pSeqGuideR != NULL)
		{
			pSeqGuideL->SetShowObject(false);
			pSeqGuideR->SetShowObject(false);
		}
	}	
}

void CX2NoviceGuide::SetMouseClickIndex(int val)
{ 
	if(val != 0 && m_iGuideMouseClick == val || m_bNoviceGuide == false)
		return;	 

	m_bHideMouseClick = false;

#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
		if(pStateMenu != NULL && val == 0)
		{
			pStateMenu->DeleteMouseClickUI();
			m_iGuideMouseClick = val;
			return;
		}

		D3DXVECTOR2 vPos = D3DXVECTOR2(0.f, 0.f);
		CX2State::MouseClickUI::MOUSE_CLICK_STYLE arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;

		switch(val)
		{
		case 1:	// npc 대화창에서 제일 오른쪽 버튼
			{
				arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;

				//{{ kimhc // 2009-08
				//if ( m_eGuideStep == NGS_STEP3 || m_eGuideStep == NGS_STEP8 )
				//{
				//	vPos = D3DXVECTOR2(507, 760);
				//}
				//else
				//}} kimhc // 2009-08
				vPos = D3DXVECTOR2(657, 760);
			}
			break;
		case 2:	// 아리엘 퀘스트 버튼
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
			vPos = D3DXVECTOR2(417, 760); //557, 760
			break;
		case 3:	// 퀘스트 목록 버튼
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
			vPos = D3DXVECTOR2(300, 140);
			break;
		case 4:	// 퀘스트 완료 or 수락 위치
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
			vPos = D3DXVECTOR2(230, 515);
			break;
		case 5:
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
			vPos = D3DXVECTOR2(657, 760);
			break;
		case 6:	// 퀘스트 수락 다이얼로그 확인
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
			vPos = D3DXVECTOR2(230, 460);
			break;
		case 7:	// 준비하기 / 시작하기
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
#ifdef SERV_PVP_NEW_SYSTEM
			vPos = D3DXVECTOR2(120, 300);		
#else
			vPos = D3DXVECTOR2(523, 100);		
#endif
			break;
		case 8:	// 던전나가기
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
#ifdef SERV_PVP_NEW_SYSTEM
			vPos = D3DXVECTOR2(120, 340);
#else
			vPos = D3DXVECTOR2(523, 151);
#endif
			break;
		case 9: // 맵선택
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
			vPos = D3DXVECTOR2(123, 170);
			break;
		case 10: // 던전선택
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
			vPos = D3DXVECTOR2(493, 510);
			break;
		case 11: // 파티만들기
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
#ifdef SERV_PVP_NEW_SYSTEM
			vPos = D3DXVECTOR2(120, 310);
#else
			vPos = D3DXVECTOR2(523, 127);
#endif
			break;
		case 12: // 퀘스트 메뉴
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
#ifdef SERV_PET_SYSTEM
			vPos = D3DXVECTOR2(880, 820);
#else
			vPos = D3DXVECTOR2(440, 820);
#endif
			break;
		case 13: // 에픽퀘스트 탭
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_TOP;
			vPos = D3DXVECTOR2(135, 150);
			break;	
		case 14: // 에픽퀘스트 완료목록2
			arrowStyle = CX2State::MouseClickUI::MCS_FROM_RIGHT_BOTTOM;
			vPos = D3DXVECTOR2(300, 135);
			break;
		default:
			pStateMenu->DeleteMouseClickUI();
			m_iGuideMouseClick = 0;
			return;
		}

		if( pStateMenu != NULL )
		{
			pStateMenu->CreateMouseClickUI( (CKTDXStage*) pStateMenu, vPos, 0.5f, 1000, arrowStyle );
			m_iGuideMouseClick = val;
			return;
		}
	}
}

void CX2NoviceGuide::SetGuideStep(int val)
{
	m_eOldGuideStep = m_eGuideStep;
	m_eGuideStep = (NOVICE_GUIDE_STEP)val;
	m_eGuideState = GS_START;
}

void CX2NoviceGuide::InitPartyGuide(bool bFlag)
{
	// 파티탈퇴/
	if(m_bNoviceGuide == false)
		return;

	if(g_pTFieldGame == NULL)
		return;

	if( g_pData->GetLocationManager()->GetCurrentVillageID() != SEnum::VMI_DUNGEON_GATE_RUBEN )
	{
		if( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true)
		{
			m_eGuideStep = NGS_STEP6;
			m_eOldGuideStep = NGS_STEP6;
			m_eGuideState = GS_START;
		}
		else
		{
			m_eGuideStep = NGS_STEP4;
			SetMouseClickIndex(0);
		}		
		return;
	}

	bool bFindQuest = false;
	CX2QuestManager::QuestInst* pQuestInstDungeon = NULL;
	pQuestInstDungeon = g_pData->GetQuestManager()->GetUnitQuest( 11010 );
	if( pQuestInstDungeon != NULL )		
	{
		bFindQuest = true;
	}

	if( bFindQuest == false )
		return;

	SetMouseClickIndex(0);
	if(bFlag == true)
	{
		// 이미 파티에 참가해 있는지 검사
		if(g_pData != NULL && g_pData->GetPartyManager() != NULL && g_pData->GetPartyManager()->DoIHaveParty() == true)
		{
			// 참가해있다면 시작하기/준비하기 가이드
			m_eGuideStep = NGS_STEP6;
			m_eOldGuideStep = NGS_STEP6;
			m_eGuideState = GS_START;
			g_pMain->GetPartyUI()->SetNoviceGuideStep(1);
		}
		else
		{
			// 참가해 있지 않다면 맵선택 가이드		
			m_eGuideStep = NGS_STEP6_1;
			m_eOldGuideStep = NGS_STEP6_1;
			m_eGuideState = GS_START;
			g_pMain->GetPartyUI()->SetNoviceGuideStep(4);
		}
	}
	else
	{
		m_eGuideStep = NGS_STEP6_1;
		m_eOldGuideStep = NGS_STEP6_1;
		m_eGuideState = GS_START;
		g_pMain->GetPartyUI()->SetNoviceGuideStep(4);
	}
	
}

// 가이드 단계 수행
bool CX2NoviceGuide::DoGuideStep1(double fTime, float fElapsedTime)
{
	if(m_eGuideState == GS_START)
	{
		if( g_pData->GetLocationManager()->GetCurrentVillageID() == SEnum::VMI_RUBEN )
		{
			CX2QuestManager::QuestInst* pQuestInst = NULL;
			CX2QuestManager::QuestInst* pQuestInstDungeon = NULL;
			
			pQuestInstDungeon = g_pData->GetQuestManager()->GetUnitQuest( 11010 );
			if(pQuestInstDungeon != NULL)
			{
				m_eOldGuideStep = m_eGuideStep;
				m_eGuideStep = NGS_STEP4;
				return true;
			}

			// 아리엘과의 대화1  
			int MAGIC_QUEST_ID = 11010;
			pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( MAGIC_QUEST_ID );
			if( NULL == pQuestInst )		
			{
				m_eGuideState = GS_END;
				m_bNoviceGuide = false;
				return true;
			}
		}
		else
		{
			return true;
		}

		// 시작시 현 가이드에 필요한 것들을 초기화한다.
		// NPC 생성
		CreateNpc();
		m_pDlgNpcMessage->SetPlayGuide(1);
		m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_AGREE);
		m_pDlgNpcMessage->SetHouseName( GET_STRING( STR_ID_370 ) );
		m_pDlgNpcMessage->SetNpcMessage( GET_STRING( STR_ID_371 ) );
		m_pDlgNpcMessage->SetShow(true);
		m_pDlgNpcMessage->SetStage(g_pMain->GetNowState());

		if( g_pTFieldGame  != NULL )
		{
			g_pTFieldGame->SetEnableKeyProcess(false);
			g_pTFieldGame->SetUserHide(true);
		}		

		// 가이드 진행시킨다.
		SetMouseClickIndex(1);

		m_eGuideState = GS_PLAY;
		return true;
	}

	if(m_eGuideState == GS_END)
	{		
		// 완료처리한다.
		if(g_pTFieldGame != NULL)
			g_pTFieldGame->SetEnableKeyProcess(true);

		// 완료후 현가이드에서 사용한 것들을 해제한다.
		SAFE_DELETE(m_pDlgNpcMessage);
		SAFE_DELETE_DIALOG( m_pDLGNpcName );
		
		//g_pData->GetUIUnitManager()->ClearAllUIUnit();
		//m_pUINPC = NULL;				
		//g_pTFieldGame->SetUserHide(false);

		SetMouseClickIndex(0);

		// 다음단계로 넘어간다.
		SetNextStep();
		return true;
	}

	// 현재 가이드 단계 진행중
	if(m_pUINPC != NULL && m_pUINPC->GetShowObject() == true)
		m_pUINPC->OnFrameMove( fTime, fElapsedTime );
	if(m_pDlgNpcMessage != NULL && m_pDlgNpcMessage->GetShow() == true)
		m_pDlgNpcMessage->OnFrameMove( fTime, fElapsedTime );

	return true;
}

bool CX2NoviceGuide::DoGuideStep2()
{
	if(m_eGuideState == GS_START)
	{
		// 시작시 현 가이드에 필요한 것들을 초기화한다.
		CreateArrow();			
		SetMouseClickIndex(0);
		m_eGuideState = GS_PLAY;
		return true;
	}

	if(m_eGuideState == GS_END)
	{		
		// 완료처리한다.

		// 완료후 현가이드에서 사용한 것들을 해제한다.
		GetUiParticle()->DestroyInstanceHandle( m_hGuideLeft );	
		GetUiParticle()->DestroyInstanceHandle( m_hGuideRight );

		m_hGuideLeft = INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hGuideRight = INVALID_PARTICLE_SEQUENCE_HANDLE;		

		// 다음단계로 넘어간다.
		SetNextStep();

		if ( g_pTFieldGame != NULL )
		{
			g_pTFieldGame->ResetNpcType( CX2LocationManager::HI_ARIEL_EVENT_HOUSE );
			g_pTFieldGame->AddShopType( CX2UnitManager::NUI_ARIEL, CX2TFieldNpcShop::NSBT_QUEST );
		}
		return true;
	}

	// 현재 가이드 단계 진행중
	m_fGuideTimer += m_fElapsedTime;
	if(m_bNoviceGuide == true && g_pTFieldGame != NULL)
	{
		bool bInArea = g_pTFieldGame->GetInArea(CX2LocationManager::HI_ARIEL_EVENT_HOUSE);
		bool bRight = g_pTFieldGame->GetIsRightNpc(CX2LocationManager::HI_ARIEL_EVENT_HOUSE);
		D3DXVECTOR3 myUnitPos = g_pTFieldGame->GetMyUnit()->GetPos();

		D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( myUnitPos );
		//float fOffsetX	= 25.f * g_pKTDXApp->GetResolutionScaleX();
		float fOffsetY	= 300.f * g_pKTDXApp->GetResolutionScaleY();
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideL	= GetUiParticle()->GetInstanceSequence( m_hGuideLeft );
		pSeqGuideL->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - fOffsetY, 0.f));
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideR	= GetUiParticle()->GetInstanceSequence( m_hGuideRight );
		pSeqGuideR->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - fOffsetY, 0.f));

		if(bInArea == false && m_bShowArrow == true)
		{
			if(bRight == true)
			{
				pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
				pSeqGuideL->SetShowObject(false);
				pSeqGuideR->SetShowObject(true);
				if(pSeqGuideR != NULL)
				{
					if(m_fGuideTimer <= 0.5f)
						pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
					else if(m_fGuideTimer <= 1.f)
						pSeqGuideR->ChangeTexForce(L"R_Over.dds");
					else
					{
						m_fGuideTimer = 0.f;
						pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
					}
				}
			}
			else
			{
				pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
				pSeqGuideR->SetShowObject(false);
				pSeqGuideL->SetShowObject(true);
				if(pSeqGuideL != NULL)
				{
					if(m_fGuideTimer <= 0.5f)
						pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
					else if(m_fGuideTimer <= 1.f)
						pSeqGuideL->ChangeTexForce(L"L_Over.dds");
					else
					{
						m_fGuideTimer = 0.f;
						pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
					}
				}
			}					
		}
		else
		{
			pSeqGuideR->SetShowObject(false);
			pSeqGuideL->SetShowObject(false);
		}
	}

	// 뽀루윌리엄의 장난 퀘 완료시 가이드 종료
	if(g_pData->GetQuestManager()->GetUnitCompleteQuest(11010) == true)
	{
		//g_pData->GetUIManager()->CloseAllNPCDlg();
		//g_pData->GetUIManager()->CloseAll();

		//SetMouseClickIndex(5);
		g_pTFieldGame->ChangeNpcType(CX2TFieldNpcShop::NSBT_CLOSE);
		g_pTFieldGame->SetNpcMessage(CX2UnitManager::NUI_ARIEL, GET_STRING( STR_ID_369 ) );

		SetHide(true);
	}
	

	return true;
}

bool CX2NoviceGuide::DoGuideStep3()
{
	CX2QuestManager::QuestInst* pQuestInstDungeon = NULL;
	pQuestInstDungeon = g_pData->GetQuestManager()->GetUnitQuest( 11010 );
	if( pQuestInstDungeon != NULL )		
	{		
		m_bNoviceGuide = true;
		m_eGuideStep = NGS_STEP4;		
		m_eGuideState = GS_START;
		return true;
	}	

	if(m_eGuideState == GS_START)
	{
		// 시작시 현 가이드에 필요한 것들을 초기화한다.
		CreateArrow();			
		m_eGuideState = GS_PLAY;
		SetMouseClickIndex(0);
		return true;
	}

	if(m_eGuideState == GS_END)
	{		
		// 완료처리한다.

		// 완료후 현가이드에서 사용한 것들을 해제한다.
		GetUiParticle()->DestroyInstanceHandle( m_hGuideLeft );	
		GetUiParticle()->DestroyInstanceHandle( m_hGuideRight );

		m_hGuideLeft = INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hGuideRight = INVALID_PARTICLE_SEQUENCE_HANDLE;		

		// 다음단계로 넘어간다.
		SetNextStep();
		return true;
	}

	// 현재 가이드 단계 진행중
	m_fGuideTimer += m_fElapsedTime;
	if(m_bNoviceGuide == true && g_pTFieldGame != NULL)
	{
		bool bInArea = g_pTFieldGame->GetInArea(CX2LocationManager::HI_HAGERS_HOUSE);
		bool bRight = g_pTFieldGame->GetIsRightNpc(CX2LocationManager::HI_HAGERS_HOUSE);
		D3DXVECTOR3 myUnitPos = g_pTFieldGame->GetMyUnit()->GetPos();

		D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( myUnitPos );
		//float fOffsetX	= 25.f * g_pKTDXApp->GetResolutionScaleX();
		float fOffsetY	= 300.f * g_pKTDXApp->GetResolutionScaleY();
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideL	= GetUiParticle()->GetInstanceSequence( m_hGuideLeft );
		pSeqGuideL->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - fOffsetY, 0.f));
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideR	= GetUiParticle()->GetInstanceSequence( m_hGuideRight );
		pSeqGuideR->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - fOffsetY, 0.f));

		if(bInArea == false && m_bShowArrow == true)
		{
			if(bRight == true)
			{
				pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
				pSeqGuideL->SetShowObject(false);
				pSeqGuideR->SetShowObject(true);
				if(pSeqGuideR != NULL)
				{
					if(m_fGuideTimer <= 0.5f)
						pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
					else if(m_fGuideTimer <= 1.f)
						pSeqGuideR->ChangeTexForce(L"R_Over.dds");
					else
					{
						m_fGuideTimer = 0.f;
						pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
					}
				}
			}
			else
			{
				pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
				pSeqGuideR->SetShowObject(false);
				pSeqGuideL->SetShowObject(true);
				if(pSeqGuideL != NULL)
				{
					if(m_fGuideTimer <= 0.5f)
						pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
					else if(m_fGuideTimer <= 1.f)
						pSeqGuideL->ChangeTexForce(L"L_Over.dds");
					else
					{
						m_fGuideTimer = 0.f;
						pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
					}
				}
			}
		}
		else
		{
			pSeqGuideR->SetShowObject(false);
			pSeqGuideL->SetShowObject(false);
		}
	}

	return true;
}

bool CX2NoviceGuide::DoGuideStep4()
{
	if(m_eGuideState == GS_START)
	{
		SetMouseClickIndex(0);

		if( g_pData->GetLocationManager()->GetCurrentVillageID() != SEnum::VMI_RUBEN )
			return false;

		// 시작시 현 가이드에 필요한 것들을 초기화한다.
		CreateArrow();			
		m_eGuideState = GS_PLAY;		
		return true;
	}

	if(m_eGuideState == GS_END)
	{		
		// 완료처리한다.

		// 완료후 현가이드에서 사용한 것들을 해제한다.
		GetUiParticle()->DestroyInstanceHandle( m_hGuideLeft );	
		GetUiParticle()->DestroyInstanceHandle( m_hGuideRight );

		m_hGuideLeft = INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hGuideRight = INVALID_PARTICLE_SEQUENCE_HANDLE;		

		// 다음단계로 넘어간다.
		SetNextStep();
		return true;
	}

	// 현재 가이드 단계 진행중
	m_fGuideTimer += m_fElapsedTime;
	if(m_bNoviceGuide == true && g_pTFieldGame != NULL)
	{
		bool bInArea = false;
		bool bRight = true;
		D3DXVECTOR3 myUnitPos = g_pTFieldGame->GetMyUnit()->GetPos();
		D3DXVECTOR2 finalPos = g_pKTDXApp->GetProj3DPos( myUnitPos );
		//float fOffsetX	= 25.f * g_pKTDXApp->GetResolutionScaleX();
		float fOffsetY	= 300.f * g_pKTDXApp->GetResolutionScaleY();
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideL	= GetUiParticle()->GetInstanceSequence( m_hGuideLeft );
		pSeqGuideL->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - fOffsetY, 0.f));
		CKTDGParticleSystem::CParticleEventSequence* pSeqGuideR	= GetUiParticle()->GetInstanceSequence( m_hGuideRight );
		pSeqGuideR->SetPosition(D3DXVECTOR3(finalPos.x, finalPos.y - fOffsetY, 0.f));

		if(bInArea == false && m_bShowArrow == true)
		{
			if(bRight == true)
			{
				pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
				pSeqGuideL->SetShowObject(false);
				pSeqGuideR->SetShowObject(true);
				if(pSeqGuideR != NULL)
				{
					if(m_fGuideTimer <= 0.5f)
						pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
					else if(m_fGuideTimer <= 1.f)
						pSeqGuideR->ChangeTexForce(L"R_Over.dds");
					else
					{
						m_fGuideTimer = 0.f;
						pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
					}
				}
			}
			else
			{
				pSeqGuideR->ChangeTexForce(L"R_Normal.dds");
				pSeqGuideR->SetShowObject(false);
				pSeqGuideL->SetShowObject(true);
				if(pSeqGuideL != NULL)
				{
					if(m_fGuideTimer <= 0.5f)
						pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
					else if(m_fGuideTimer <= 1.f)
						pSeqGuideL->ChangeTexForce(L"L_Over.dds");
					else
					{
						m_fGuideTimer = 0.f;
						pSeqGuideL->ChangeTexForce(L"L_Normal.dds");
					}
				}
			}
		}
		else
		{
			pSeqGuideR->SetShowObject(false);
			pSeqGuideL->SetShowObject(false);
		}
	}

	return true;
}

bool CX2NoviceGuide::DoGuideStep5(double fTime, float fElapsedTime)
{
	if(m_eGuideState == GS_START)
	{
		if( g_pData->GetLocationManager()->GetCurrentVillageID() != SEnum::VMI_DUNGEON_GATE_RUBEN )
		{
			m_eGuideStep = NGS_STEP4;
			SetMouseClickIndex(0);
			return true;
		}

		// 시작시 현 가이드에 필요한 것들을 초기화한다.
		// NPC 생성
		CreateNpc();
		m_pDlgNpcMessage->SetPlayGuide(5);
		m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_CLOSE);
		m_pDlgNpcMessage->SetHouseName( GET_STRING( STR_ID_370 ) );
		m_pDlgNpcMessage->SetNpcMessage( GET_STRING( STR_ID_372 ) );
		m_pDlgNpcMessage->SetShow(true);
		m_pDlgNpcMessage->SetStage(g_pMain->GetNowState());

		g_pTFieldGame->SetEnableKeyProcess(false);
		g_pTFieldGame->SetUserHide(true);

		// 가이드 진행시킨다.
		SetMouseClickIndex(1);

		m_eGuideState = GS_PLAY;
		return true;
	}

	if(m_eGuideState == GS_END)
	{		
		// 완료처리한다.
		g_pTFieldGame->SetEnableKeyProcess(true);

		// 완료후 현가이드에서 사용한 것들을 해제한다.
		SAFE_DELETE(m_pDlgNpcMessage);
		SAFE_DELETE_DIALOG( m_pDLGNpcName );

		//g_pData->GetUIUnitManager()->ClearAllUIUnit();
		//m_pUINPC = NULL;				
		//g_pTFieldGame->SetUserHide(false);

		SetMouseClickIndex(0);

		// 다음단계로 넘어간다.
		SetNextStep();
		return true;
	}

	// 현재 가이드 단계 진행중
	if(m_pUINPC != NULL && m_pUINPC->GetShowObject() == true)
		m_pUINPC->OnFrameMove( fTime, fElapsedTime );
	if(m_pDlgNpcMessage != NULL && m_pDlgNpcMessage->GetShow() == true)
		m_pDlgNpcMessage->OnFrameMove( fTime, fElapsedTime );

	if( g_pData->GetLocationManager()->GetCurrentVillageID() != SEnum::VMI_DUNGEON_GATE_RUBEN )
	{		
		SetMouseClickIndex(0);
	}

	return true;
}

bool CX2NoviceGuide::DoGuideStep6()
{
	if(m_eGuideState == GS_START)
	{
		// 시작시 현 가이드에 필요한 것들을 초기화한다.
		
		// 가이드 진행시킨다.
		if( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true)
		{
			//SetMouseClickIndex(8);
			g_pMain->GetPartyUI()->SetNoviceGuideStep(2);
		}

		m_eGuideState = GS_PLAY;
		return true;
	}

	if(m_eGuideState == GS_END)
	{
		// 완료처리한다.

		// 완료후 현가이드에서 사용한 것들을 해제한다.
		if( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true)
		{
			//SetMouseClickIndex(0);
			g_pMain->GetPartyUI()->SetNoviceGuideStep(3);
		}

		// 다음단계로 넘어간다.
		SetNextStep();
		return true;
	}	

	return true;
}

bool CX2NoviceGuide::DoGuideStep6_0(int iMouse)
{
	//if(m_eGuideState == GS_START)
	//{
	//	// 시작시 현 가이드에 필요한 것들을 초기화한다.

	//	// 가이드 진행시킨다.
	//	if( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true)
	//	{
	//		SetMouseClickIndex(iMouse);
	//	}

	//	m_eGuideState = GS_PLAY;
	//	return true;
	//}

	if(m_eGuideState == GS_END)
	{
		// 완료처리한다.	
		// 다음단계로 넘어간다.
		SetNextStep();
		return true;
	}	

	return true;
}

bool CX2NoviceGuide::DoGuideStep7(double fTime, float fElapsedTime)
{
	if(m_eGuideState == GS_START)
	{
		// 가이드 진행시킨다.
		if( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )
		{
			SetMouseClickIndex(8);
		}
		else if( g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )				
		{
			SetMouseClickIndex(12);
		}
		
		m_eGuideState = GS_PLAY;
		return true;
	}

	if(m_eGuideState == GS_END)
	{		
		SetMouseClickIndex(0);

		// 다음단계로 넘어간다.
		SetNextStep();
		return true;
	}
	
	if( g_pData->GetLocationManager()->IsDungeonLounge( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true )
	{
		if( g_pData->GetUIManager() != NULL &&
			g_pData->GetUIManager()->GetUIQuestNew() != NULL &&
			g_pData->GetUIManager()->GetUIQuestNew()->GetShow() == true )
		{
			SetMouseClickIndex(0);
		}
		else
		{
			SetMouseClickIndex(8);
		}
		
	}
	else if( g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() ) == true &&
		g_pData->GetUIManager()->GetUIQuestNew()->GetShow() == false &&
		g_pData->GetQuestManager()->GetUnitCompleteQuest(11010) == false )				
	{
		SetMouseClickIndex(12);
	}

	return true;
}

bool CX2NoviceGuide::DoGuideStep8()
{
	if(m_eGuideState == GS_START)
	{		
		// 시작시 현 가이드에 필요한 것들을 초기화한다.

		// 가이드 진행시킨다.

		m_eGuideState = GS_PLAY;
		return true;
	}

	if(m_eGuideState == GS_END)
	{		
		// 완료처리한다.

		// 완료후 현가이드에서 사용한 것들을 해제한다.		
		m_bNoviceGuide = false;
		
		return true;
	}

	// 현재 가이드 단계 진행중
#ifdef EVENT_SCENE
	if( g_pData != NULL &&
		g_pData->GetEventScene() != NULL &&
		g_pData->GetEventScene()->GetIsPlayingEventScene() == true)
	{
		m_bPlayEventScene = true;
	}

	if( m_bPlayEventScene == true && g_pData->GetEventScene()->GetIsPlayingEventScene() == false)
	{
		m_eGuideState = GS_END;
		m_bNoviceGuide = false;

		// 아이템장착 가이드 팝업
		m_pDlgItemGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Help_Item.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgItemGuide );
		m_pDlgItemGuide->SetFront(true);
		m_pDlgItemGuide->SetDisableUnderWindow(true);		

		return true;
	}
#endif EVENT_SCENE



	return true;
}

void CX2NoviceGuide::HideGuideDlg()
{
	if( m_pDlgEpicGuide != NULL )
		m_pDlgEpicGuide->SetShow(false);

	if( m_pDlgItemGuide != NULL )
		m_pDlgItemGuide->SetShow(false);	
}

void CX2NoviceGuide::DeleteGuideDlg()
{	
	SAFE_DELETE_DIALOG( m_pDlgEpicGuide );
	m_pDlgEpicGuide = NULL;

	SAFE_DELETE_DIALOG( m_pDlgItemGuide );
	m_pDlgItemGuide = NULL;
}

void CX2NoviceGuide::CreateEpicGuideDlg()
{	
	m_pDlgEpicGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Help_Epic.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgEpicGuide );
	m_pDlgEpicGuide->SetFront(true);
	m_pDlgEpicGuide->SetDisableUnderWindow(true);
}

bool CX2NoviceGuide::GetShowGuideDlg()
{
	if( m_pDlgEpicGuide != NULL && m_pDlgEpicGuide->GetShow() == true )
		return true;

	if( m_pDlgItemGuide != NULL && m_pDlgItemGuide->GetShow() == true )
		return true;

	return false;
}



#endif //REFORM_NOVICE_GUIDE
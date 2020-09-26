#include "StdAfx.h"

#ifdef SERV_PET_SYSTEM
#include ".\x2uiPetInfo.h"
#include ".\X2NPCUnitViewerUI.h"

CX2UIPetInfo::CX2UIPetInfo( CKTDXStage* pNowStage ) :
m_bShow(false),
m_MovedPosition(0,0),						// D3DXVECTOR2
m_DLGPosition(0,0),							// D3DXVECTOR2
m_DLGSize(0,0),								// D3DXVECTOR2
m_pNPCViewerUI( NULL ),
m_pDLGUIPetList(NULL),
m_pDLGUIPetInfo(NULL),
m_pDLGUIPetDetail(NULL),
m_fRemainRotY(0.0f),
m_fMouseSensitivity(0.01f),
m_bRotateReturn(true),
m_UnitClickPos(100,220),				// D3DXVECTOR2
m_UnitClickSize(240,150),			// D3DXVECTOR2
m_bUnitClicked(false)
#ifdef PET_DROP_ITEM_PICKUP
,m_UseItemUID(0)
#endif //PET_DROP_ITEM_PICKUP
#ifdef RIDING_SYSTEM
,m_pRidingPetViewerUI( NULL )
,m_iSelectButtonIndex( -1 )
,m_pDLGUIRidingPetReleasePopup( NULL )
,m_fListStaminaUpdateCooltime(5.0f)
#endif //RIDING_SYSTEM
{
	m_pDLGUIPetList = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_List.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIPetList );
	m_pDLGUIPetList->SetShowEnable(false, false);
	m_pDLGUIPetList->SetDisableUnderWindow(true);

	m_pDLGUIPetInfo = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Basic_Infor.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIPetInfo );
	m_pDLGUIPetInfo->SetShowEnable(false, false);
	m_pDLGUIPetInfo->SetDisableUnderWindow(true);

	m_pDLGUIPetDetail = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Propensity.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIPetDetail );
	m_pDLGUIPetDetail->SetShowEnable(false, false);
	m_pDLGUIPetDetail->SetDisableUnderWindow(true);
	



	D3DXVECTOR3 tmp;
	tmp = m_pDLGUIPetList->GetDummyPos(0);
	m_DLGPosition.x = tmp.x;
	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGUIPetList->GetDummyPos(1);
	m_DLGSize.x = tmp.x;
	m_DLGSize.y = tmp.y;

#ifdef RIDING_SYSTEM
	CKTDGUIRadioButton* pRadioButtonPetTab = static_cast<CKTDGUIRadioButton*>( m_pDLGUIPetList->GetControl( L"PET_LIST" ) );
	if( NULL != pRadioButtonPetTab )
		pRadioButtonPetTab->SetChecked( true );

	m_eTabSelect	= PET_TAB_SELECT;
	m_eRidingPetTab = RPIT_LIST;
	
	m_pDLGUIRidingPetList = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_RIDING_Pet_List.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIRidingPetList );
	m_pDLGUIRidingPetList->SetShowEnable(false, false);
	m_pDLGUIRidingPetList->SetDisableUnderWindow(true);

	m_pDLGUIRidingPetInfo = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_RIDING_Pet_Basic_Infor.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIRidingPetInfo );
	m_pDLGUIRidingPetInfo->SetShowEnable(false, false);
	m_pDLGUIRidingPetInfo->SetDisableUnderWindow(true);

	for(int i = 0; i < 6; i++ )
	{
		Enable_ButtonBG( i, true );
		SetRidingPetRow( i, -1, 20 );
	}

	m_ViewRidingPetUid = -1;
	m_iNowPageRiding = 1;
	m_iMaxPageRiding = 1;

	m_fMoveViewerPetPositionX = 0.f;
	m_fMaxStamina = 1.f;

	if( NULL != m_pDLGUIPetInfo)
	{
		m_vecViewerRidingPetSize = m_pDLGUIPetInfo->GetDummyPos(1);
		m_vecViewerPetSize = m_pDLGUIPetInfo->GetDummyPos(1);
	}
	else
	{
		m_vecViewerRidingPetSize = D3DXVECTOR3(1.f,1.f,1.f);
		m_vecViewerPetSize = D3DXVECTOR3(1.f,1.f,1.f);
	}

	//탑승 버튼 툴팁
	bool IsSetKey = (0 == StrCmp(g_pMain->GetKeyPad()->GetKeyString( GA_RIDING ).c_str(), L"" ))? false : true;
	UpdateRidingButtonGuideDesc( IsSetKey );

	m_eKindRideButton = BUTTON_RIDE_ON;
#endif //RIDING_SYSTEM

	m_bProcessPetList = false;
	m_iNowPage = 1;
	m_iMaxPage = 1;
	
	m_ePetTab = PIT_LIST;
	m_ViewPetUid = -1;

	m_fSummonCoolTime = 0.f;

	m_bDetailPoint = false;
	m_vDetailPoint = D3DXVECTOR2( 0.f, 0.f );
	m_hParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;	
}


CX2UIPetInfo::~CX2UIPetInfo(void)
{
	SAFE_DELETE_KTDGOBJECT( m_pNPCViewerUI );
	SAFE_DELETE_DIALOG( m_pDLGUIPetList );
	SAFE_DELETE_DIALOG( m_pDLGUIPetInfo );
	SAFE_DELETE_DIALOG( m_pDLGUIPetDetail );

#ifdef RIDING_SYSTEM
	SAFE_DELETE_DIALOG( m_pDLGUIRidingPetList );
	SAFE_DELETE_DIALOG( m_pDLGUIRidingPetInfo );
	SAFE_DELETE_DIALOG( m_pDLGUIRidingPetReleasePopup );
	SAFE_DELETE_KTDGOBJECT( m_pRidingPetViewerUI );
#endif //RIDING_SYSTEM
}

void CX2UIPetInfo::SetLayer( X2_DIALOG_LAYER layer )
{	
	if(m_pDLGUIPetList != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIPetList, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIPetList, false );
	}
	if(m_pDLGUIPetInfo != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIPetInfo, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIPetInfo, false );
	}
	if(m_pDLGUIPetDetail != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIPetDetail, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIPetDetail, false );
	}

#ifdef RIDING_SYSTEM
	if( m_pDLGUIRidingPetInfo != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGUIRidingPetInfo, layer );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIRidingPetInfo, false );
	}
	if( m_pDLGUIRidingPetList != NULL )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGUIRidingPetList, layer );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIRidingPetList, false );
	}
#endif //RIDING_SYSTEM
}

void CX2UIPetInfo::SetPosition( D3DXVECTOR2 vec )
{
#ifdef RIDING_SYSTEM
	vec.x += 15;
	vec.y += 46;
#endif //RIDING_SYSTEM

	// 슬롯 다이얼로그 유닛뷰어
	if(m_pDLGUIPetList != NULL) m_pDLGUIPetList->SetPos(vec);
	if(m_pDLGUIPetInfo != NULL) m_pDLGUIPetInfo->SetPos(vec);
	if(m_pDLGUIPetDetail != NULL) m_pDLGUIPetDetail->SetPos(vec);

#ifdef RIDING_SYSTEM
	if( m_pDLGUIRidingPetInfo != NULL ) m_pDLGUIRidingPetInfo->SetPos(vec);
	if( m_pDLGUIRidingPetList != NULL ) m_pDLGUIRidingPetList->SetPos(vec);
#endif //RIDING_SYSTEM

	m_MovedPosition = vec;
}

bool CX2UIPetInfo::IsInUnitClickReact( D3DXVECTOR2 mousePos )
{
	D3DXVECTOR2 m_ConvertPos	= g_pKTDXApp->ConvertByResolution( m_UnitClickPos + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( m_UnitClickSize );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;
}

#ifdef RIDING_SYSTEM
void CX2UIPetInfo::UnitHandling( CX2NPCUnitViewerUI* pViewerUI, CKTDGUIDialogType pDLGType, double fTime, float fElapsedTime )
{
	if( pDLGType != NULL && pDLGType->GetShow() == true )
	{
		if( pViewerUI != NULL )
		{
			D3DXVECTOR3 vScale;
			if( true == m_pDLGUIPetInfo->GetShow() )
				vScale = m_vecViewerPetSize * pViewerUI->GetScale();
			else if( true == m_pDLGUIRidingPetInfo->GetShow() )
				vScale = m_vecViewerRidingPetSize * pViewerUI->GetScale();
			else
				vScale = D3DXVECTOR3(0.7f,0.7f,0.7f) * pViewerUI->GetScale();

			pViewerUI->SetScale( vScale );

			D3DXVECTOR3 vPos = pDLGType->GetDummyPos(0);
			vPos.x += pDLGType->GetPos().x;
			vPos.y += pDLGType->GetPos().y;
#ifdef RIDING_SYSTEM
			vPos.x += m_fMoveViewerPetPositionX;
#endif //RIDING_SYSTEM
			pViewerUI->SetPosition( vPos );

		
			float fRotSpeed = 13.0f;


			if ( m_bUnitClicked == true )
			{
				D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
				pViewerUI->GetMatrix().RotateRel(RotateY);
				m_bRotateReturn = false;

				fRotSpeed = 13.0f;
			}
			else
			{
				D3DXVECTOR3 rot = pViewerUI->GetMatrix().GetRotate();



				pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
				int addSpeed = 20;

				if ( m_bRotateReturn == false )
				{
					fRotSpeed -= (fElapsedTime * addSpeed);

					if ( fRotSpeed <= 0 )
					{
						fRotSpeed = fElapsedTime;
					}

					if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
					{

						if ( m_fRemainRotY <= 0.0f )
						{
							pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							m_fRemainRotY = 0.0f;
							m_bRotateReturn = true;
						}
						else
						{
							m_fRemainRotY -= (fElapsedTime*fRotSpeed);
							pViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY <= 0.0f )
							{
								pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
								m_fRemainRotY = 0.0f;
								m_bRotateReturn = true;
							}
						}
					}
					else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
					{
						if ( m_fRemainRotY >= 6.18f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
						else
						{
							m_fRemainRotY += (fElapsedTime*fRotSpeed);
							pViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY >= 6.18f )
							{
								m_bRotateReturn = true;
								m_fRemainRotY = 0.0f;
								pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							}
						}
					}
					else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
					{
						if ( m_fRemainRotY >= 0.0f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
						else
						{
							m_fRemainRotY += (fElapsedTime*fRotSpeed);
							pViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY >= 0.0f )
							{
								m_bRotateReturn = true;
								m_fRemainRotY = 0.0f;
								pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							}
						}
					}
					else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
					{
						if ( m_fRemainRotY <= -6.28f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
						else
						{
							m_fRemainRotY -= (fElapsedTime*fRotSpeed);
							pViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY <= -6.28f )
							{
								m_bRotateReturn = true;
								m_fRemainRotY = 0.0f;
								pViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							}
						}

					}
				}
			}

			pViewerUI->OnFrameMove( fTime, fElapsedTime );
		}
	}
}
#else
void CX2UIPetInfo::UnitHandling( double fTime, float fElapsedTime )
{
	if( m_pDLGUIPetInfo != NULL && m_pDLGUIPetInfo->GetShow() == true )
	{
		D3DXVECTOR3 vScale = m_pDLGUIPetInfo->GetDummyPos(1) * m_pNPCViewerUI->GetScale();
		m_pNPCViewerUI->SetScale( vScale );

		D3DXVECTOR3 vPos = m_pDLGUIPetInfo->GetDummyPos(0);
		vPos.x += m_pDLGUIPetInfo->GetPos().x;
		vPos.y += m_pDLGUIPetInfo->GetPos().y;
		m_pNPCViewerUI->SetPosition( vPos );

		if( m_pNPCViewerUI != NULL )
		{
			float fRotSpeed = 13.0f;


			if ( m_bUnitClicked == true )
			{
				D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
				m_pNPCViewerUI->GetMatrix().RotateRel(RotateY);
				m_bRotateReturn = false;

				fRotSpeed = 13.0f;
			}
			else
			{
				D3DXVECTOR3 rot = m_pNPCViewerUI->GetMatrix().GetRotate();



				m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
				int addSpeed = 20;

				if ( m_bRotateReturn == false )
				{
					fRotSpeed -= (fElapsedTime * addSpeed);

					if ( fRotSpeed <= 0 )
					{
						fRotSpeed = fElapsedTime;
					}

					if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
					{

						if ( m_fRemainRotY <= 0.0f )
						{
							m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							m_fRemainRotY = 0.0f;
							m_bRotateReturn = true;
						}
						else
						{
							m_fRemainRotY -= (fElapsedTime*fRotSpeed);
							m_pNPCViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY <= 0.0f )
							{
								m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
								m_fRemainRotY = 0.0f;
								m_bRotateReturn = true;
							}
						}
					}
					else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
					{
						if ( m_fRemainRotY >= 6.18f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
						else
						{
							m_fRemainRotY += (fElapsedTime*fRotSpeed);
							m_pNPCViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY >= 6.18f )
							{
								m_bRotateReturn = true;
								m_fRemainRotY = 0.0f;
								m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							}
						}
					}
					else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
					{
						if ( m_fRemainRotY >= 0.0f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
						else
						{
							m_fRemainRotY += (fElapsedTime*fRotSpeed);
							m_pNPCViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY >= 0.0f )
							{
								m_bRotateReturn = true;
								m_fRemainRotY = 0.0f;
								m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							}
						}
					}
					else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
					{
						if ( m_fRemainRotY <= -6.28f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
						else
						{
							m_fRemainRotY -= (fElapsedTime*fRotSpeed);
							m_pNPCViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

							if ( m_fRemainRotY <= -6.28f )
							{
								m_bRotateReturn = true;
								m_fRemainRotY = 0.0f;
								m_pNPCViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							}
						}

					}
				}
			}

			m_pNPCViewerUI->OnFrameMove( fTime, fElapsedTime );
		}
	}
}
#endif //RIDING_SYSTEM

HRESULT CX2UIPetInfo::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();	

	if( m_fSummonCoolTime > 0.f )
	{
		m_fSummonCoolTime -= fElapsedTime;
		if( m_fSummonCoolTime < 0.f )
			m_fSummonCoolTime = 0.f;
	}

#ifdef RIDING_SYSTEM
	switch( m_eTabSelect )
	{
	case PET_TAB_SELECT:
		UnitHandling( m_pNPCViewerUI, m_pDLGUIPetInfo, fTime, fElapsedTime );
		break;

	case RIDING_PET_TAB_SELECT:
		{
			switch( m_eRidingPetTab )
			{
			case RPIT_LIST:
				{
					if( m_fListStaminaUpdateCooltime > 0.f )
					{
						m_fListStaminaUpdateCooltime -= fElapsedTime;
						if( m_fListStaminaUpdateCooltime < 0.f )
							m_fListStaminaUpdateCooltime = 0.f;
					}
					else
					{
						UpdateStamina_List();
						UpdatePeriod_List();
						m_fListStaminaUpdateCooltime = 5.f;
					}
				}
				break;

			case RPIT_INFO:
				{
					if( NULL != CX2RidingPetManager::GetInstance() )
					{			
						UnitHandling( m_pRidingPetViewerUI, m_pDLGUIRidingPetInfo, fTime, fElapsedTime );

						CX2RidingPetManager::sRidingPetInfo sInfo;
						if( false == CX2RidingPetManager::GetInstance()->GetRidingPetInfoByUID( m_ViewRidingPetUid, sInfo ) )
							return S_OK;

						UpdateStamina_Info( sInfo.m_fStamina, m_fMaxStamina );

						//탑승 버튼 처리
						float fEnableStamina = CX2RidingPetManager::GetInstance()->GetEnableStamina();
						bool bRide = ( sInfo.m_fStamina < fEnableStamina )? false : true;
						if( true == bRide )
						{
							SelectKindRideButton( BUTTON_RIDE_NONE, true );
						}
						else
						{
							SelectKindRideButton( BUTTON_RIDE_NONE, false );
						}
					}
				}
				break;
			}
		}	
		break;

	default:
		break;
	}

#else
	UnitHandling(fTime, fElapsedTime);
#endif //RIDING_SYSTEM

	if( m_bDetailPoint == true )
	{
		D3DXVECTOR2 vPos = m_vDetailPoint + m_pDLGUIPetDetail->GetPos();		
		vPos = vPos + D3DXVECTOR2(32.5f, 32.5f);

		CKTDGParticleSystem::CParticleEventSequence *pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hParticle );
		if( pSeq != NULL )
		{
			D3DXVECTOR3 vPosDetail = D3DXVECTOR3( vPos.x, vPos.y, 0.f );
			pSeq->SetPosition( vPosDetail );
		}
		else
		{
			CKTDGParticleSystem::CParticleEventSequence* pParticle = 
				g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Pet_Ui_Effect01", vPos.x, vPos.y, 0  );
			if( pParticle != NULL )
			{
				if( m_hParticle != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle);
					m_hParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;
				}

				pParticle->SetOverUI( true );
				m_hParticle = pParticle->GetHandle();
			}	
		}
	}
	else
	{
		if( m_hParticle != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle);
			m_hParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	return S_OK;
}

bool CX2UIPetInfo::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bFlag = false;
	
	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	//case WM_MOUSEWHEEL:
		{
			if( (m_pDLGUIPetList->GetShow() == true && m_pDLGUIPetList->GetIsMouseOver() == false) || 
				(m_pDLGUIPetInfo->GetShow() == true && m_pDLGUIPetInfo->GetIsMouseOver() == false) ||
				(m_pDLGUIPetDetail->GetShow() == true && m_pDLGUIPetDetail->GetIsMouseOver() == false) 
#ifdef RIDING_SYSTEM
				|| (m_pDLGUIRidingPetList->GetShow() == true && m_pDLGUIRidingPetList->GetIsMouseOver() == false )
				|| (m_pDLGUIRidingPetInfo->GetShow() == true && m_pDLGUIRidingPetInfo->GetIsMouseOver() == false )
#endif //RIDING_SYSTEM
				)
			{
				return false;
			}
			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if( (m_pDLGUIPetList->GetShow() == true && m_pDLGUIPetList->GetIsMouseOver() == false) || 
				(m_pDLGUIPetInfo->GetShow() == true && m_pDLGUIPetInfo->GetIsMouseOver() == false) ||
				(m_pDLGUIPetDetail->GetShow() == true && m_pDLGUIPetDetail->GetIsMouseOver() == false) 
#ifdef RIDING_SYSTEM
				|| (m_pDLGUIRidingPetList->GetShow() == true && m_pDLGUIRidingPetList->GetIsMouseOver() == false )
				|| (m_pDLGUIRidingPetInfo->GetShow() == true && m_pDLGUIRidingPetInfo->GetIsMouseOver() == false )
#endif //RIDING_SYSTEM
				)
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_PET_LIST, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	if( m_pDLGUIPetInfo != NULL && m_pDLGUIPetInfo->GetShow() == true 
#ifdef RIDING_SYSTEM
		|| ( m_pDLGUIRidingPetInfo != NULL && m_pDLGUIRidingPetInfo->GetShow() == true )
#endif //RIDING_SYSTEM
		)
	{
		switch( uMsg )
		{
		case WM_LBUTTONUP:
			{
				if( m_pNPCViewerUI != NULL && m_bUnitClicked == true )
				{
					D3DXVECTOR3 rot = m_pNPCViewerUI->GetMatrix().GetRotate();
					int degree = (int)D3DXToDegree( rot.y );
					m_fRemainRotY = D3DXToRadian( degree % 360 );
				}

#ifdef RIDING_SYSTEM
				if( m_pRidingPetViewerUI != NULL && m_bUnitClicked == true )
				{
					D3DXVECTOR3 rot = m_pRidingPetViewerUI->GetMatrix().GetRotate();
					int degree = (int)D3DXToDegree( rot.y );
					m_fRemainRotY = D3DXToRadian( degree % 360 );
				}
#endif //RIDING_SYSTEM
				m_bUnitClicked = false;
			}
			break;

		case WM_LBUTTONDOWN:
			{
				POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
				D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

				if( IsInUnitClickReact( mousePos ) == true )
				{
					m_bUnitClicked = true ;
					return true;
				}
			}
			break;
		}
	}	

	return bFlag;
}


bool CX2UIPetInfo::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case PLUCM_CLOSE:
		{
			SetShow(false);
		}
		return true;
	case PLUCM_VIEW_LIST:
		{
			ViewListTab( m_iNowPage );
		}
		return true;
	case PLUCM_VIEW_INFO:
		{
			for(UINT i=0; i<m_vecPetList.size(); ++i)
			{
				if( m_vecPetList[i].m_iPetUID == m_ViewPetUid )
				{
					ViewInfoTab( &m_vecPetList[i] );
					return true;
				}
			}
		}
		return true;
	case PLUCM_VIEW_DETAIL:
		{
			for(UINT i=0; i<m_vecPetList.size(); ++i)
			{
				if( m_vecPetList[i].m_iPetUID == m_ViewPetUid )
				{
					ViewDetailTab( &m_vecPetList[i] );
					return true;
				}
			}
		}
		return true;
	case PLUCM_LIST_SELECT:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			if( pButton == NULL )
				return true;

			int iSelectedIndex = pButton->GetDummyInt( 0 );
			if( iSelectedIndex >= (int)m_vecPetList.size() )
				return true;

			ViewInfoTab( &m_vecPetList[iSelectedIndex] );
		}
		return true;
	case PLUCM_INFO_SUMMON:
		{
			CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
			CX2Cursor*	pCursor = pState->GetCursor();
			if( pCursor->GetCurorState() == CX2Cursor::XCS_PET_FOOD )
			{
				return true;
			}

			if( m_fSummonCoolTime <= 0.f )
			{
				g_pData->GetPetManager()->Handler_EGS_SUMMON_PET_REQ( m_ViewPetUid );
				m_fSummonCoolTime = 1.f;
			}
		}
		return true;
	case PLUCM_INFO_EVOLUTION:
		{
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_5351 ), 
				PLUCM_INFO_EVOLUTION_OK, (CKTDXStage*) g_pMain->GetNowState() ); 
		}
		return true;
	case PLUCM_INFO_SUMMON_CANCEL:
		{
			CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
			CX2Cursor*	pCursor = pState->GetCursor();
			if( pCursor->GetCurorState() == CX2Cursor::XCS_PET_FOOD )
			{
				return true;
			}

			if( m_fSummonCoolTime <= 0.f )
			{
				g_pData->GetPetManager()->Handler_EGS_SUMMON_PET_REQ( 0 );
				m_fSummonCoolTime = 1.f;
			}			
		}
		return true;
	case PLUCM_INFO_EVOLUTION_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			g_pData->GetPetManager()->Handler_EGS_PET_EVOLUTION_REQ( m_ViewPetUid );
		}
		return true;
	case PLUCM_LIST_PREV:
		{
			PrevPage();
		}
		return true;
	case PLUCM_LIST_NEXT:
		{
			NextPage();
		}
		return true;
#ifdef PET_DROP_ITEM_PICKUP
	case PLUCM_ITEM_PICKUP_SKILL:
		{		
			BOOST_FOREACH( KPetInfo kPetInfoData, m_vecPetList )
			{	
				if( kPetInfoData.m_iPetUID == m_ViewPetUid &&//현재 노출되어있는 펫의 UID 체크
					false == kPetInfoData.m_bAutoLooting )// 스킬 잠금 상태일 때만 동작
				{	
						
					if( NULL != g_pData->GetMyUser() && 
						NULL != g_pData->GetMyUser()->GetSelectUnit() &&
						NULL != g_pData->GetUIManager() && 
						NULL != g_pData->GetUIManager()->GetUIInventory() ) //인벤토리 UI 널체크
					{
						const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
						m_UseItemUID = kInventory.GetItemUIDBySortTypeAndItemID( CX2Inventory::ST_SPECIAL, ACTIVATION_DROP_ITEM_PICKUP_SKILL);
#ifdef SERV_PET_AUTO_LOOTING_ITEM_CN
						if( 0 == m_UseItemUID )
							m_UseItemUID = kInventory.GetItemUIDBySortTypeAndItemID( CX2Inventory::ST_SPECIAL, ACTIVATION_DROP_ITEM_PICKUP_SKILL_CN);
#endif // SERV_PET_AUTO_LOOTING_ITEM_CN
						//잠금 해제 아이템을 가지고 있지 않다면 캐시샵으로 이동
						if( 0 == m_UseItemUID )
						{
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_21661),
								PLUCM_CASH_SHOP_OPEN_OK, g_pMain->GetNowState() );
						}
						else
						{
							//현재 소환된 '@1'펫의 아이템 줍기 스킬을 해제 하시겠습니까?
							g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
								GET_REPLACED_STRING( ( STR_ID_21659, "L", kPetInfoData.m_wstrPetName ) ),
								PLUCM_ITEM_PICKUP_SKILL_OK, g_pMain->GetNowState() );
						}
					}
					break;
				}
			}
		} 
		return true;
	case PLUCM_ITEM_PICKUP_SKILL_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl == NULL )
				return true;

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );
			g_pData->GetUIManager()->GetUIInventory()->Handler_EGS_USE_ITEM_IN_INVENTORY_REQ(m_UseItemUID, m_ViewPetUid);
			m_UseItemUID = 0;
		}
		return true;
	case PLUCM_CASH_SHOP_OPEN_OK:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl == NULL )
				return true;

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			//마을이 아니면 채팅 로그 추가
			if( g_pX2Game != NULL && g_pMain->GetNowStateID() != CX2Main::XS_BATTLE_FIELD )
			{
				g_pChatBox->AddChatLog(  GET_STRING( STR_ID_16478 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				return false;
			}
	#ifdef CHECK_INHERIT_STATE_MENU
			if( g_pMain->IsInheritStateMenu() )
	#endif //CHECK_INHERIT_STATE_MENU
			{
				CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
				if( NULL != pStateMenu)
					pStateMenu->ToggleCashShop();
			}
		}
		return true;
#endif //PET_DROP_ITEM_PICKUP

#ifdef RIDING_SYSTEM
	case PLUCM_PET_TAP_SELECT:
		{
			m_eTabSelect = PET_TAB_SELECT;
			Select_Tab( m_ePetTab );
		}
		return true;

	case PLUCM_RIDING_PET_TAP_SELECT:
		{
			m_eTabSelect = RIDING_PET_TAB_SELECT;
			Select_Tab( m_eRidingPetTab );
		}
		return true;

	case PLUCM_RIDING_PET_INFO:
		{
			m_eRidingPetTab = RPIT_INFO;
			Select_Tab( m_eRidingPetTab );
		}
		return true;

	case PLUCM_RIDING_PET_LIST:
		{
			m_eRidingPetTab = RPIT_LIST;
			Select_Tab( m_eRidingPetTab );
		}
		return true;

	case PLUCM_RIDING_PET_RIDE:
		{
			bool bSuccess = false;
			if ( NULL != CX2RidingPetManager::GetInstance() )
			{
				CX2RidingPetManager* pRidingManager = CX2RidingPetManager::GetInstance();

				if( pRidingManager->CanPushSummonButton() 
					&& ( NULL != g_pMain->GetNowState() && static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() ) )
				{
					pRidingManager->RefreshSummonButtonElpasedTime();
					if( true == pRidingManager->GetRidingOnState() )
					{
						if( true == pRidingManager->GetRidingOn() )
							pRidingManager->Handler_EGS_UNSUMMON_RIDING_PET_REQ( true, m_ViewRidingPetUid );
						else if( false == pRidingManager->GetDirectSummon() )
							pRidingManager->Handler_EGS_SUMMON_RIDING_PET_REQ( m_ViewRidingPetUid );

						bSuccess = true;
					}	
				}
			}

			if( false == bSuccess )
			{   //탑승 할 수 없을때에 대한 안내 문구
				if( NULL != g_pX2Game )
				{
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_24526 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				}
			}
		}
		return true;

	case PLUCM_RIDING_PET_RIDE_OFF:
		{
			bool bSuccess = false;
			if ( NULL != CX2RidingPetManager::GetInstance() )
			{
				CX2RidingPetManager* pRidingManager = CX2RidingPetManager::GetInstance();
				if( pRidingManager->CanPushSummonButton() && 
					( NULL != g_pMain->GetNowState() && static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() ) )
				{
					pRidingManager->RefreshSummonButtonElpasedTime();
					if( true == pRidingManager->GetRidingOnState() )
					{
						if( true == pRidingManager->GetRidingOn() )
						{
							pRidingManager->Handler_EGS_UNSUMMON_RIDING_PET_REQ();
							bSuccess = true;
						}			
					}
				}
			}	

			if( false == bSuccess )
			{   //탑승 할 수 없을때에 대한 안내 문구
				if( NULL != g_pX2Game )
					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_24527 ), D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			}
		}
		return true;


	case PLUCM_RIDING_LIST_BUTTON_CLICK:
		{
			if( true == Select_Button( lParam ) )
			{
				m_eRidingPetTab = RPIT_INFO;
				Select_Tab( m_eRidingPetTab );
			}
		}
		return true;

	case PLUCM_RIDING_LIST_BUTTON_SELECT:
		{
			if ( NULL != CX2RidingPetManager::GetInstance() && CX2RidingPetManager::GetInstance()->CanPushSummonButton() 
				 && ( NULL != g_pMain->GetNowState() && static_cast<CX2State*>(g_pMain->GetNowState())->GetEnableShortCutKey() ) )
			{
				CX2RidingPetManager* pRidingManager = CX2RidingPetManager::GetInstance();
				pRidingManager->RefreshSummonButtonElpasedTime();

				if( true == Select_Button( lParam ) )
				{	
					//이미 탑승하고 있는 펫에 대한 탑승 요청은 할 필요가 없음.바로 return.
					if( m_ViewRidingPetUid == pRidingManager->GetRidingPetId() )
						return true;

					if( true == pRidingManager->GetRidingOnState() )
					{
						if( true == pRidingManager->GetRidingOn() )
							pRidingManager->Handler_EGS_UNSUMMON_RIDING_PET_REQ( true, m_ViewRidingPetUid );
						else if( false == pRidingManager->GetRidingOn() && false == pRidingManager->GetDirectSummon() )
							pRidingManager->Handler_EGS_SUMMON_RIDING_PET_REQ( m_ViewRidingPetUid );
					}
				}
			}	
		}
		return true;

	case PLUCM_RIDING_PET_RELEASE:
		{
			if(m_pDLGUIRidingPetReleasePopup != NULL)
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUIRidingPetReleasePopup, NULL, false );
				m_pDLGUIRidingPetReleasePopup = NULL;
			}
#ifdef RIDINGPET_INFO_ADD_LINEBREAK
			wstring wstrRidingPetReleaseDesc = CWordLineHandler::GetStrByLineBreakInX2Main(GET_STRING( STR_ID_24382 ), 360, 1);
			m_pDLGUIRidingPetReleasePopup = 
				g_pMain->KTDGUIOkAndCancelEditBox2( D3DXVECTOR2( -999, -999 ), wstrRidingPetReleaseDesc.c_str(), PLUCM_RIDING_PET_RELEASE_OK, g_pMain->GetNowState(), 10, false, PLUCM_RIDING_PET_RELEASE_CANCLE );
#else RIDINGPET_INFO_ADD_LINEBREAK
			m_pDLGUIRidingPetReleasePopup = 
				g_pMain->KTDGUIOkAndCancelEditBox2( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_24382 ),	PLUCM_RIDING_PET_RELEASE_OK, g_pMain->GetNowState(), 10, false, PLUCM_RIDING_PET_RELEASE_CANCLE );
#endif RIDINGPET_INFO_ADD_LINEBREAK			
			if( NULL == m_pDLGUIRidingPetReleasePopup )
				return true;

			m_pDLGUIRidingPetReleasePopup->SetEnableMoveByDrag_LUA( false );
		}
		return true;

	case PLUCM_RIDING_PET_RELEASE_OK:
		{
			if ( NULL != CX2RidingPetManager::GetInstance() )
			{
				CKTDGUIIMEEditBox* pIMEEditBox = static_cast<CKTDGUIIMEEditBox*>(m_pDLGUIRidingPetReleasePopup->GetControl(L"IMEEditBoxMessage"));
				if( NULL != pIMEEditBox )
				{
					wstring wstrMessage = pIMEEditBox->GetText();
					if( 0 == StrCmp(wstrMessage.c_str(), GET_STRING(STR_ID_24517)) )
					{
						m_eTabSelect = RIDING_PET_TAB_SELECT;
						CX2RidingPetManager::GetInstance()->Handler_EGS_RELEASE_RIDING_PET_REQ( m_ViewRidingPetUid );
						g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUIRidingPetReleasePopup, NULL, false );
						m_pDLGUIRidingPetReleasePopup = NULL;
					}
					else
					{
						CKTDGUIDialogType pMsgBox = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_23609 ), g_pMain->GetNowState() );
						if( NULL != pMsgBox )
							pMsgBox->SetEnableMoveByDrag_LUA(false);
					}

				}
			}
		}
		return true;
			
	case PLUCM_RIDING_PET_RELEASE_CANCLE:
		{
			if(m_pDLGUIRidingPetReleasePopup != NULL)
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUIRidingPetReleasePopup, NULL, false );
				m_pDLGUIRidingPetReleasePopup = NULL;
			}
		}
		return true;

	case PLUCM_RIDING_LIST_PREV:
		{
			if( m_iNowPageRiding - 1 < 1 )
				m_iNowPageRiding = 1;
			else
				m_iNowPageRiding -= 1;

		//	Handler_EGS_GET_RIDING_PET_LIST_REQ( m_iNowPageRiding );

			ViewRidingPetList( m_iNowPageRiding );
		}
		return true;

	case PLUCM_RIDING_LIST_NEXT:
		{
			if( m_iNowPageRiding + 1 > m_iMaxPageRiding )
				m_iNowPageRiding = m_iMaxPageRiding;
			else
				m_iNowPageRiding += 1;

		//	Handler_EGS_GET_RIDING_PET_LIST_REQ( m_iNowPageRiding );

			ViewRidingPetList( m_iNowPageRiding );
		}
		return true;

#endif //RIDING_SYSTEM

	default:
		break;
	}
	return false;
}

bool CX2UIPetInfo::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
#ifdef RIDING_SYSTEM
	case EGS_GET_RIDING_PET_LIST_ACK:
		return Handler_EGS_GET_RIDING_PET_LIST_ACK( hWnd, uMsg, wParam, lParam );

	/*case EGS_SUMMON_RIDING_PET_ACK:
		return Handler_EGS_SUMMON_RIDING_PET_ACK( hWnd, uMsg, wParam, lParam );

	case EGS_UNSUMMON_RIDING_PET_ACK:
		return Handler_EGS_UNSUMMON_RIDING_PET_ACK( hWnd, uMsg, wParam, lParam );*/
#endif //RIDING_SYSTEM
	case EGS_GET_PET_LIST_ACK:
		return Handler_EGS_GET_PET_LIST_ACK( hWnd, uMsg, wParam, lParam );
	default:
		break;
	}
	return false;
}

void CX2UIPetInfo::UpdateDLGNPCViewerUI()
{	
	if( NULL == m_pNPCViewerUI )
	{
		m_pNPCViewerUI = CX2NPCUnitViewerUI::Create();
	}

	if( NULL == m_pNPCViewerUI )
		return;

	if( m_ViewPetUid <= 0 )
		return; 

	bool bFind = false;
	KPetInfo kPetInfo;
	for(UINT i=0; i<m_vecPetList.size(); ++i)
	{
		if( m_vecPetList[i].m_iPetUID == m_ViewPetUid )
		{
			kPetInfo = m_vecPetList[i];
			bFind = true;
			break;
		}
	}	
	
	if( bFind == false )
	{
		if( NULL != g_pData &&
			NULL != g_pData->GetMyUser() &&
			NULL != g_pData->GetMyUser()->GetSelectUnit() &&
			NULL != g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() )
		{
			kPetInfo = *g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo();
		}
	}

#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	m_pNPCViewerUI->ResetPet( kPetInfo.m_iPetID, kPetInfo.m_cEvolutionStep );
#else //SERV_PETID_DATA_TYPE_CHANGE
	m_pNPCViewerUI->ResetPet( kPetInfo.m_cPetID, kPetInfo.m_cEvolutionStep );
#endif //SERV_PETID_DATA_TYPE_CHANGE
	m_pNPCViewerUI->SetShowObject( true );

	m_pNPCViewerUI->PlayAnim( L"Wait", CKTDGXSkinAnim::XAP_LOOP, true );	// 0 번째 index animation을 play

	if( m_pDLGUIPetInfo != NULL )
	{
		m_pDLGUIPetInfo->SetHasUnit( (CKTDGObject*) m_pNPCViewerUI );

		D3DXVECTOR3 vScale = m_pDLGUIPetInfo->GetDummyPos(1);
#ifdef RIDING_SYSTEM
		m_vecViewerPetSize = vScale;
#endif //RIDING_SYSTEM
		m_pNPCViewerUI->SetScale( vScale );

		D3DXVECTOR3 vPos = m_pDLGUIPetInfo->GetDummyPos(0);
		vPos.x += m_pDLGUIPetInfo->GetPos().x;
		vPos.y += m_pDLGUIPetInfo->GetPos().y;
#ifdef RIDING_SYSTEM
		m_fMoveViewerPetPositionX = 0.f;
#endif //RIDING_SYSTEM
		m_pNPCViewerUI->SetPosition( vPos );
	}

	// JHKang / 강정훈 / 2010.10.27 / 펫 정보 보기 시 변환 정보 강제 갱신
	m_pNPCViewerUI->OnFrameMove( g_pKTDXApp->GetTime(), g_pKTDXApp->GetElapsedTime() );
}

void CX2UIPetInfo::SetShow(bool val, UidType eTab)
{
	m_bShow = val;

	if(val)	// 켜질 때 처리해야 할 부분
	{
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_PET_LIST);

		if( eTab == NULL )
		{
			if( g_pData->GetPetManager()->GetMyPet() != NULL )
			{
				m_ePetTab = PIT_INFO;
			}
			else
			{
				m_ePetTab = PIT_LIST;
			}
		}
		else
		{
			SetViewSummonedPetUid();
			m_ePetTab = (PET_INFO_TAB) eTab;
		}

		m_iNowPage = 1;

#ifdef RIDING_SYSTEM
		m_iNowPageRiding = 1;

		if( RIDING_PET_TAB_SELECT == m_eTabSelect )
		{
			Select_Tab( m_eRidingPetTab );
		}
		else if ( PET_TAB_SELECT == m_eTabSelect )
		{
#endif //RIDING_SYSTEM
			switch( m_ePetTab )
			{
			case PIT_LIST:
				Handler_EGS_GET_PET_LIST_REQ( 1 );
				m_pDLGUIPetList->SetShowEnable(true, true);
				m_pDLGUIPetInfo->SetShowEnable(false, false);
				m_pDLGUIPetDetail->SetShowEnable(false, false);
				break;
			case PIT_INFO:
				m_vecPetList.clear();
				UpdatePetInfo( g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() );			
				m_pDLGUIPetList->SetShowEnable(false, false);
				m_pDLGUIPetInfo->SetShowEnable(true, true);
				m_pDLGUIPetDetail->SetShowEnable(false, false);
				UpdateDLGNPCViewerUI();
				break;
			case PIT_DETAIL:
				UpdatePetDetail( g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() );
				m_pDLGUIPetList->SetShowEnable(false, false);
				m_pDLGUIPetInfo->SetShowEnable(false, false);
				m_pDLGUIPetDetail->SetShowEnable(true, true);
				break;
			default:
				break;
			}
#ifdef RIDING_SYSTEM
		}
#endif //RIDING_SYSTEM
	}
	else	// 꺼질 때 처리해야 할 부분
	{			
		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_PET_LIST);

		m_pDLGUIPetList->SetShowEnable(false, false);
		m_pDLGUIPetInfo->SetShowEnable(false, false);
		m_pDLGUIPetDetail->SetShowEnable(false, false);

#ifdef RIDING_SYSTEM
		m_pDLGUIRidingPetInfo->SetShowEnable( false, false );
		m_pDLGUIRidingPetList->SetShowEnable( false, false );
		if(m_pDLGUIRidingPetReleasePopup != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUIRidingPetReleasePopup, NULL, false );
			m_pDLGUIRidingPetReleasePopup = NULL;
		}
#endif //RIDING_SYSTEM

		m_bDetailPoint = false;
		if( m_hParticle != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle);
			m_hParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}
	}

	
}

void CX2UIPetInfo::Handler_EGS_GET_PET_LIST_REQ( UINT iViewPage )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		    g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
            )
		return;  

	if( m_bProcessPetList == true )
		return;

	KEGS_GET_PET_LIST_REQ kPacket;
	kPacket.m_nViewPage = iViewPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_PET_LIST_REQ, kPacket );

	m_bProcessPetList = true;

	return;
}

bool CX2UIPetInfo::Handler_EGS_GET_PET_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_PET_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bProcessPetList = false;

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		
		// 펫 생성 성공
		
		m_iMaxPage = kEvent.m_nTotalPage;
		m_iNowPage = kEvent.m_nViewPage;

		m_vecPetList.clear();
		m_vecPetList = kEvent.m_vecPetList;
		
		UpdatePetList();
		return true;
	}

	return false;
}

#ifdef SERV_PERIOD_PET
void CX2UIPetInfo::SetPetRow( int rowIndex, int petId, char petLv, wstring petName, float fSatiety, float fIntimacy, bool bSummoned, wstring wstrDestroyDate )
#else SERV_PERIOD_PET
void CX2UIPetInfo::SetPetRow(int rowIndex, int petId, char petLv, wstring petName, float fSatiety, float fIntimacy, bool bSummoned)
#endif SERV_PERIOD_PET
{
	WCHAR cControlName[30] = {0,};
	CKTDGUIButton *pButton = NULL;		// Pet_Btn_List_Name1
	CKTDGUIStatic *pStaticName = NULL;		// Pet_List_Name1
	CKTDGUIStatic *pStaticSatiety = NULL;		// Pet_List_Satiety1
	CKTDGUIStatic *pStaticIntimacy = NULL;		// Pet_List_Intimacy1
	CKTDGUIControl::CPictureData *pPicture = NULL;	//g_pStatic_Pet_List_Character_Win
	CKTDGUIControl::CPictureData *pPictureEvolution = NULL; 
	
	// row는 1부터...
	if( rowIndex <= 0 )
	{
		return;
	}

	if( m_pDLGUIPetList != NULL )
	{
		//wsprintf( cControlName, L"Pet_Btn_List_Name%d", rowIndex );
		StringCchPrintf( cControlName, 30, L"Pet_Btn_List_Name%d", rowIndex );

		pButton = (CKTDGUIButton*)m_pDLGUIPetList->GetControl( cControlName );
	
		//wsprintf( cControlName, L"Pet_List_Name%d", rowIndex );
		StringCchPrintf( cControlName, 30, L"Pet_List_Name%d", rowIndex );
		pStaticName = (CKTDGUIStatic*)m_pDLGUIPetList->GetControl( cControlName );

		//wsprintf( cControlName, L"Pet_List_Satiety%d", rowIndex );
		StringCchPrintf( cControlName, 30, L"Pet_List_Satiety%d", rowIndex );
		pStaticSatiety = (CKTDGUIStatic*)m_pDLGUIPetList->GetControl( cControlName );

		//wsprintf( cControlName, L"Pet_List_Intimacy%d", rowIndex );
		StringCchPrintf( cControlName, 30, L"Pet_List_Intimacy%d", rowIndex );
		pStaticIntimacy = (CKTDGUIStatic*)m_pDLGUIPetList->GetControl( cControlName );
		
		CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGUIPetList->GetControl( L"Pet_List_Character_Win" );
		if( pStatic != NULL )
			pPicture = pStatic->GetPicture( rowIndex-1 );

		CKTDGUIStatic *pStaticEvolution = (CKTDGUIStatic*)m_pDLGUIPetList->GetControl( L"Pet_List_Character_Evolution" );
		if( pStaticEvolution != NULL )
			pPictureEvolution = pStaticEvolution->GetPicture( rowIndex-1 );

		bool bShow = true;
		if( petId < 0 )
		{
			bShow = false;
		}
		
		CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)petId );
		if( pTemplet == NULL )
		{
			bShow = false;
		}

		if( bShow == true )
		{			
			if( pStaticName != NULL )
			{
#ifdef CLIENT_COUNTRY_EU
// 유럽은 펫 이름이 먼저 나오고 특수한 상태 문장이 뒤에 나오도록 수정 2011.10.15 임홍락
				wstring wstrPetName = L"";
				wstrPetName += petName;

				float fIntimacyPer = g_pData->GetPetManager()->GetIntimacyPercent( petId, petLv, fIntimacy );
				if( fIntimacyPer >= 0.7f )
				{
					wstrPetName += GET_STRING( STR_ID_5612 );
				}
#else
				wstring wstrPetName = L"";

				float fIntimacyPer = g_pData->GetPetManager()->GetIntimacyPercent( petId, petLv, fIntimacy );
				if( fIntimacyPer >= 0.7f )
				{
					wstrPetName = GET_STRING( STR_ID_5612 );
					wstrPetName += L" ";
				}

				wstrPetName += petName;
#endif CLIENT_COUNTRY_EU
				int iStatus = g_pData->GetPetManager()->GetPetStatus( petId, petLv );
				switch(iStatus)
				{
				case 0:
					wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5344 ) + L")";
					break;
				case 1:
					wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5345 ) + L")";
					break;
				case 2:
					wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5346 ) + L")";
					break;
				case 3:
					wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5347 ) + L")";
					break;
				default:
					break;
				}
				
#ifdef SERV_PERIOD_PET
				if( wstrDestroyDate != L"" )
				{
					CTime tDestroyTime;
					CTime tCurrentTime( g_pData->GetServerCurrentTime() );

					if( true == KncUtil::ConvertStringToCTime( wstrDestroyDate, tDestroyTime ) )
					{
						CTimeSpan temp = tDestroyTime - tCurrentTime; 
						int day = static_cast<int>( temp.GetDays() );
						int hour = static_cast<int>( temp.GetHours() );
						int minute = static_cast<int>( temp.GetMinutes() );

						wstring wstrExpirationDate;
						if( day > 0 )
						{
							wstrExpirationDate = GET_REPLACED_STRING( (STR_ID_22727, "i", day) );
						}
						else if( day == 0 && hour > 0 )
						{
							wstrExpirationDate = GET_REPLACED_STRING( (STR_ID_22728, "i", hour) );
						}
						else if( day == 0 && hour == 0 && minute > 0 )
						{
							wstrExpirationDate = GET_REPLACED_STRING( (STR_ID_366, "i", minute) );
						}
						else
						{
							wstrExpirationDate = GET_REPLACED_STRING( (STR_ID_366, "i", 0) );
						}
#ifdef CLIENT_COUNTRY_US
						wstrPetName += wstring( L"\n(" ) + GET_STRING(STR_ID_309) + wstrExpirationDate + L")";
#else //CLIENT_COUNTRY_US
						wstrPetName += wstring( L"(" ) + GET_STRING(STR_ID_309) + wstrExpirationDate + L")";
#endif //CLIENT_COUNTRY_US
					}
				}
#endif SERV_PERIOD_PET

				pStaticName->GetString(0)->msg = wstrPetName;
			}
			if( pStaticSatiety != NULL )
			{
				wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_5340, "i", (int)(fSatiety / 4800.f * 100.f) ) );	// 포만도				
				pStaticSatiety->GetString(0)->msg = wstrBuff;
			}
			if( pStaticIntimacy != NULL )
			{
				{
					wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_5341, "i", (int)(fIntimacy / pTemplet->m_Evolution_Step_Max[ petLv ] * 100.f) ) );	// 친밀도				
					pStaticIntimacy->GetString(0)->msg = wstrBuff;
				}
			}
			if( pPicture != NULL )
			{
#ifdef DEFENCE_CODE_FOR_CRASH
				if( petLv < pTemplet->m_Evolution_Step_Image.size() )
				{
#endif //DEFENCE_CODE_FOR_CRASH
					CX2PetManager::PetStepImage petStepImage = pTemplet->m_Evolution_Step_Image[ petLv ];

					wstring wstrImageName = petStepImage.m_wstrImageName;
					if( petStepImage.m_wstrKeyName == L"" )
					{
						pPicture->SetTex( wstrImageName.c_str() );
					}
					else
					{
						pPicture->SetTex( wstrImageName.c_str(), petStepImage.m_wstrKeyName.c_str() );
					}
#ifdef DEFENCE_CODE_FOR_CRASH
				}
#endif //DEFENCE_CODE_FOR_CRASH
			}		
		}

		if( pButton != NULL )
		{
			pButton->SetShowEnable(bShow, bShow);
			if( bSummoned == true )
				pButton->SetDownStateAtNormal(true);
			else
				pButton->SetDownStateAtNormal(false);
		}
		if( pStaticName != NULL )
			pStaticName->SetShowEnable(bShow, bShow);
		if( pStaticSatiety != NULL )
			pStaticSatiety->SetShowEnable(bShow, bShow);
		if( pStaticIntimacy != NULL )
			pStaticIntimacy->SetShowEnable(bShow, bShow);
		if( pPicture != NULL )
			pPicture->SetShow(bShow);

		if( pPictureEvolution != NULL )
		{
			
			if( bShow == true && petLv < pTemplet->m_Evolution_All_Step && (int)(fIntimacy / pTemplet->m_Evolution_Step_Max[ petLv ] * 100) >= 100 )
			{
				pPictureEvolution->SetFlicker( 1.2f, 1.4f, 0.3f );
				pPictureEvolution->SetShow(true);
			}
			else
			{
				pPictureEvolution->SetShow(false);
			}
		}
	}

}

void CX2UIPetInfo::UpdatePetList()
{
	m_bDetailPoint = false;

	for( int i=0; i<6; ++i)
	{
		SetPetRow( i+1, -1 );
	}

	for(UINT i=0; i<m_vecPetList.size(); ++i)
	{
		KPetInfo kPetInfo = m_vecPetList[i];
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)kPetInfo.m_iPetID );
		
		char petLv = kPetInfo.m_cEvolutionStep;
		wstring petName = kPetInfo.m_wstrPetName;		
		bool bSummoned = false;
		if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL && 
			g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() != NULL &&
			g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_iPetUID == kPetInfo.m_iPetUID )
			bSummoned = true;
#ifdef SERV_PERIOD_PET
		SetPetRow( i+1, (int)kPetInfo.m_iPetID, petLv, petName, (float)kPetInfo.m_sSatiety, (float)kPetInfo.m_iIntimacy, bSummoned, kPetInfo.m_wstrDestroyDate );
#else SERV_PERIOD_PET
		SetPetRow( i+1, (int)kPetInfo.m_iPetID, petLv, petName, (float)kPetInfo.m_sSatiety, (float)kPetInfo.m_iIntimacy, bSummoned );
#endif SERV_PERIOD_PET
#else //SERV_PETID_DATA_TYPE_CHANGE
		CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)kPetInfo.m_cPetID );

		char petLv = kPetInfo.m_cEvolutionStep;
		wstring petName = kPetInfo.m_wstrPetName;		
		bool bSummoned = false;
		if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL && 
			g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() != NULL &&
			g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_iPetUID == kPetInfo.m_iPetUID )
			bSummoned = true;
#ifdef SERV_PERIOD_PET
		SetPetRow( i+1, (int)kPetInfo.m_cPetID, petLv, petName, (float)kPetInfo.m_sSatiety, (float)kPetInfo.m_iIntimacy, bSummoned, kPetInfo.m_wstrDestroyDate );
#else SERV_PERIOD_PET
		SetPetRow( i+1, (int)kPetInfo.m_cPetID, petLv, petName, (float)kPetInfo.m_sSatiety, (float)kPetInfo.m_iIntimacy, bSummoned );
#endif SERV_PERIOD_PET
#endif //SERV_PETID_DATA_TYPE_CHANGE
	}

	
	if( m_pDLGUIPetList != NULL )
	{
		CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGUIPetList->GetControl( L"Pet_Page_Num" );
		WCHAR pageBuf[100] = {0,};
		//wsprintf(pageBuf, L"%d / %d", m_iNowPage, m_iMaxPage);
		StringCchPrintf( pageBuf, 100, L"%d / %d", m_iNowPage, m_iMaxPage);
		if( pStatic != NULL )
			pStatic->GetString(0)->msg = pageBuf;
	}	
}

void CX2UIPetInfo::ChangeInfo()
{
	if( g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() != NULL )
	{
		KPetInfo *pPetInfo = g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo();

		SetPetInfo( *pPetInfo );

		if( m_bShow == true )
			UpdateView();
	}
}

void CX2UIPetInfo::SetPetInfo( const KPetInfo& kPetInfo )
{
	if( m_vecPetList.size() == 0 )
		return;

	for(UINT i=0; i<m_vecPetList.size(); ++i)
	{
		KPetInfo &petInfo = m_vecPetList[i];
		if( petInfo.m_iPetUID == kPetInfo.m_iPetUID)
		{
			petInfo.m_iPetUID		 = kPetInfo.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			petInfo.m_iPetID		 = kPetInfo.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
			petInfo.m_cPetID		 = kPetInfo.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
			petInfo.m_cEvolutionStep = kPetInfo.m_cEvolutionStep;
			petInfo.m_sSatiety		 = kPetInfo.m_sSatiety;
			petInfo.m_iIntimacy		 = kPetInfo.m_iIntimacy;
			petInfo.m_sExtroversion	 = kPetInfo.m_sExtroversion;
			petInfo.m_sEmotion		 = kPetInfo.m_sEmotion;
			petInfo.m_bAutoFeed		 = kPetInfo.m_bAutoFeed;
#ifdef PET_DROP_ITEM_PICKUP
			petInfo.m_bAutoLooting	 = kPetInfo.m_bAutoLooting;
#endif //PET_DROP_ITEM_PICKUP
#ifdef SERV_PERIOD_PET
			petInfo.m_wstrDestroyDate	= kPetInfo.m_wstrDestroyDate;
#endif SERV_PERIOD_PET

			return;
		}
	}
}
#ifdef PET_DROP_ITEM_PICKUP
void CX2UIPetInfo::SetPetAutoLooting( UidType uiPetUID, bool bAutoLooting)
{
	if( m_vecPetList.size() == 0 )
		return;

	for(UINT i=0; i<m_vecPetList.size(); ++i)
	{
		KPetInfo &petInfo = m_vecPetList[i];
		if( petInfo.m_iPetUID == uiPetUID )
		{
			petInfo.m_bAutoLooting = bAutoLooting;
			return;
		}
	}
}
#endif //PET_DROP_ITEM_PICKUP

void CX2UIPetInfo::SetViewSummonedPetUid( )
{
	m_vecPetList.clear();
	m_vecPetList.push_back(  *(g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()) );
	m_ViewPetUid = g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_iPetUID;
}

void CX2UIPetInfo::UpdateView()
{
	switch( m_ePetTab )
	{
	case PIT_LIST:
		UpdatePetList();
		return;
	case PIT_INFO:
		{
			for(UINT i=0; i<m_vecPetList.size(); ++i)
			{
				if( m_vecPetList[i].m_iPetUID == m_ViewPetUid )
				{
					UpdatePetInfo( &m_vecPetList[i] );
					UpdateDLGNPCViewerUI();
					return;
				}
			}

			if( g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() != NULL )
			{
				UpdatePetInfo( g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() );
				UpdateDLGNPCViewerUI();
				return;
			}
		}		
		break;
	case PIT_DETAIL:
		{
			for(UINT i=0; i<m_vecPetList.size(); ++i)
			{
				if( m_vecPetList[i].m_iPetUID == m_ViewPetUid )
				{
					UpdatePetDetail( &m_vecPetList[i] );
					return;
				}
			}

			if( g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() != NULL )
			{
				UpdatePetDetail( g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() );
				return;
			}
		}
		return;;
	default:
		break;
	}
}

void CX2UIPetInfo::ViewListTab(int iPage)
{	
	int iViewPage = iPage;
	if( iViewPage == 0 )
		iViewPage = m_iNowPage;

	Handler_EGS_GET_PET_LIST_REQ( iViewPage );

	m_ePetTab = PIT_LIST;

	UpdatePetList();
	m_pDLGUIPetList->SetShowEnable(true, true);
	m_pDLGUIPetInfo->SetShowEnable(false, false);
	m_pDLGUIPetDetail->SetShowEnable(false, false);
}

void CX2UIPetInfo::ViewInfoTab( KPetInfo *kPetInfo )
{
	if( kPetInfo == NULL )
		return;

	m_ePetTab = PIT_INFO;

	UpdatePetInfo( kPetInfo );	
	m_pDLGUIPetList->SetShowEnable(false, false);
	m_pDLGUIPetInfo->SetShowEnable(true, true);
	m_pDLGUIPetDetail->SetShowEnable(false, false);
	UpdateDLGNPCViewerUI();
}

void CX2UIPetInfo::ViewDetailTab( KPetInfo *kPetInfo )
{
	if( kPetInfo == NULL )
		return;

	m_ePetTab = PIT_DETAIL;

	UpdatePetDetail( kPetInfo );
	m_pDLGUIPetList->SetShowEnable(false, false);
	m_pDLGUIPetInfo->SetShowEnable(false, false);
	m_pDLGUIPetDetail->SetShowEnable(true, true);
}

void CX2UIPetInfo::NextPage()
{	
	if( m_iNowPage + 1 > m_iMaxPage )
		m_iNowPage = m_iMaxPage;
	else
		m_iNowPage += 1;

	Handler_EGS_GET_PET_LIST_REQ( m_iNowPage );
}

void CX2UIPetInfo::PrevPage()
{
	if( m_iNowPage - 1 < 1 )
		m_iNowPage = 1;
	else
		m_iNowPage -= 1;

	Handler_EGS_GET_PET_LIST_REQ( m_iNowPage );
}

void CX2UIPetInfo::UpdatePetInfo( KPetInfo *kPetInfo )
{
	if( m_pDLGUIPetInfo == NULL )
		return;

	if( kPetInfo == NULL )
		return;

#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)kPetInfo->m_iPetID );
#else //SERV_PETID_DATA_TYPE_CHANGE
	CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)kPetInfo->m_cPetID );
#endif //SERV_PETID_DATA_TYPE_CHANGE
	if( pTemplet == NULL )
		return;

	CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGUIPetInfo->GetControl( L"g_pStatic_Pet_Basic" );
	if( pStatic == NULL )
		return;

	m_bDetailPoint = false;

	m_ViewPetUid = kPetInfo->m_iPetUID;
	
	if( m_vecPetList.size() == 0)
	{
		KPetInfo kPetInfoDummy = *kPetInfo;
		m_vecPetList.push_back( kPetInfoDummy );
	}


	CKTDGUIControl::CPictureData *pPicture1 = NULL;	//포만도
	CKTDGUIControl::CPictureData *pPictureTip1 = NULL;	//포만도
	CKTDGUIControl::CPictureData *pPicture2 = NULL;	//친밀도
	CKTDGUIControl::CPictureData *pPictureTip2 = NULL;	//친밀도
	CKTDGUIControl::CPictureData *pPictureIcon = NULL;	//펫 아이콘
	CKTDGUIControl::CPictureData *pPictureBarEffect = NULL;	//친밀도 효과

	CKTDGUIButton *pButton1			= (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Recall" );				// 소환버튼
	CKTDGUIButton *pButton2			= (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Evolution" );			// 진화버튼
	CKTDGUIButton *pButton3			= (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Recall_Cancel" );		// 소환해제버튼
	CKTDGUIStatic *pStaticName		=  (CKTDGUIStatic*)m_pDLGUIPetInfo->GetControl( L"Pet_Basic_Name" );			// 펫이름
	CKTDGUIStatic *pStaticSatiety	=  (CKTDGUIStatic*)m_pDLGUIPetInfo->GetControl( L"Pet_Basic_Poman" );		// 포만도 풍선말
	CKTDGUIStatic *pStaticIntimacy	=  (CKTDGUIStatic*)m_pDLGUIPetInfo->GetControl( L"Pet_Basic_Chinmil" );		// 친밀도 풍선말
	CKTDGUIStatic *pStaticCondition =  (CKTDGUIStatic*)m_pDLGUIPetInfo->GetControl( L"Pet_Basic_Condition" );		// 펫 상태
	
	CKTDGUIButton *pButtonAttack	= (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Attack_Skill" );				// 소환버튼
	CKTDGUIButton *pButtonCheer		= (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Cheer_Skill" );				// 소환버튼

#ifdef PET_AURA_SKILL
	CKTDGUIButton *pButtonAura[3]	= {NULL,};
	pButtonAura[0] = (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Aura_Skill_1" );				// aura1
	pButtonAura[1] = (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Aura_Skill_2" );				// aura2
	pButtonAura[2] = (CKTDGUIButton*)m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Aura_Skill_3" );				// aura3
#endif

#ifdef RIDING_SYSTEM
	pPictureIcon = pStatic->GetPicture( 19 );
	pPicture1 = pStatic->GetPicture( 22 );
	pPicture2 = pStatic->GetPicture( 23 );
	pPictureTip1 = pStatic->GetPicture( 24 );
	pPictureTip2 = pStatic->GetPicture( 25 );
	pPictureBarEffect = pStatic->GetPictureIndex(1);
#else
	pPictureIcon = pStatic->GetPicture( 13 ); //19 );
	pPicture1 = pStatic->GetPicture( 16 ); //22 );
	pPicture2 = pStatic->GetPicture( 17 ); //23 );
	pPictureTip1 = pStatic->GetPicture( 18 ); //24 );
	pPictureTip2 = pStatic->GetPicture( 19 );//25 );
	pPictureBarEffect = pStatic->GetPictureIndex(1);
#endif //RIDING_SYSTEM

	// 스킬 정보
	if( pButtonAttack != NULL && pTemplet->m_vecPetStatus[ kPetInfo->m_cEvolutionStep ] > 0 )
	{
		int iStep = (int)kPetInfo->m_cEvolutionStep;
		if( iStep > 0 )
			iStep -= 1; 

		CX2PetManager::PetSkillInfo petSkillInfo = pTemplet->m_AttackSkill_Step[iStep];
		pButtonAttack->SetNormalTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );
		pButtonAttack->SetOverTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );
		pButtonAttack->SetDownTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring wstrSkillDesc = CWordLineHandler::GetStrByLineBreakInX2Main(petSkillInfo.m_wstrSkillDesc.c_str(), 290, 1);
		pButtonAttack->SetGuideDesc( wstrSkillDesc.c_str() );
#else //CLIENT_GLOBAL_LINEBREAK
		pButtonAttack->SetGuideDesc( petSkillInfo.m_wstrSkillDesc.c_str() );
#endif //CLIENT_GLOBAL_LINEBREAK
		//pButtonAttack->SetGuideDescAbsolutePos( pButtonAttack->GetPos() );
		pButtonAttack->SetGuideDescOffsetPos( D3DXVECTOR2( 50.f, 40.f ) );
		//pButtonAttack->SetDownStateAtNormal( true );		
	}
	else
	{
		if( pButtonAttack != NULL )
		{
			pButtonAttack->SetGuideDesc( L"" );
			pButtonAttack->SetNormalTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_ATTACK" );
			pButtonAttack->SetOverTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_ATTACK" );
			pButtonAttack->SetDownTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_ATTACK" );
			//pButtonAttack->SetDownStateAtNormal( false );
		}
	}
	
	if( pButtonCheer != NULL && pTemplet->m_vecPetStatus[ kPetInfo->m_cEvolutionStep ] > 0 )
	{		
		int iStep = (int)kPetInfo->m_cEvolutionStep;
		if( iStep > 0 )
			iStep -= 1; 

		CX2PetManager::PetSkillInfo petSkillInfo = pTemplet->m_CheerSkill[iStep];
		pButtonCheer->SetNormalTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );
		pButtonCheer->SetOverTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );
		pButtonCheer->SetDownTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );		
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring wstrSkillDesc = CWordLineHandler::GetStrByLineBreakInX2Main(petSkillInfo.m_wstrSkillDesc.c_str(), 300, 1);
		pButtonCheer->SetGuideDesc( wstrSkillDesc.c_str() );
#else //CLIENT_GLOBAL_LINEBREAK
		pButtonCheer->SetGuideDesc( petSkillInfo.m_wstrSkillDesc.c_str() );
#endif //CLIENT_GLOBAL_LINEBREAK
		//pButtonCheer->SetGuideDescAbsolutePos( pButtonCheer->GetPos() );
		pButtonCheer->SetGuideDescOffsetPos( D3DXVECTOR2( 50.f, 40.f ) );
		//pButtonAttack->SetDownStateAtNormal( true );

		
		
		// 격려스킬 아이콘 활성/비활성화
		if( (float)kPetInfo->m_sSatiety / 4800.f >= 0.4f )
			pButtonCheer->SetColor(D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ));
		else
			pButtonCheer->SetColor(D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.f ));
	}
	else
	{
		if( pButtonCheer != NULL )
		{
			pButtonCheer->SetGuideDesc( L"" );
			pButtonCheer->SetNormalTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_CHEER" );
			pButtonCheer->SetOverTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_CHEER" );
			pButtonCheer->SetDownTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_CHEER" );
			//pButtonAttack->SetDownStateAtNormal( false );
		}
	}	

#ifdef PET_AURA_SKILL
	for(int i=0; i<3; ++i)
	{
		if( pButtonAura[i] != NULL )
		{
			pButtonAura[i]->SetGuideDesc( L"" );
			pButtonAura[i]->SetNormalTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_AURA" );
			pButtonAura[i]->SetOverTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_AURA" );
			pButtonAura[i]->SetDownTex( L"DLG_UI_Common_Texture42_NEW.tga", L"PET_SKILL_SLOT_AURA" );

			if( (float)kPetInfo->m_sSatiety / CX2PetManager::MAX_OF_SATIETY >= CX2PetManager::SATIETY_RATIO_TO_BE_ABLE_TO_APPLY_AURA )
				pButtonAura[i]->SetColor(D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ));
			else
				pButtonAura[i]->SetColor(D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.f ));
		}

		int iStep = pTemplet->m_vecPetStatus[ kPetInfo->m_cEvolutionStep ];
		if( iStep > 0 && i < iStep && i < (int)pTemplet->m_AuraSkill.size() )
		{
			if( pButtonAura[i] != NULL )
			{
				wstring wstrAuraDesc = L"";
				
				if( pTemplet->m_vecAuraSkillOption[i].size() > 0 )
				{
					wstrAuraDesc += L"\n";
					wstrAuraDesc += pTemplet->m_AuraSkill[i].m_wstrSkillDesc.c_str();
					wstrAuraDesc += L"\n\n";

					//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
					const int iLevel_ = g_pData->GetSelectUnitLevel();
					//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

					const std::vector<int> &vecSocketOption = pTemplet->m_vecAuraSkillOption[i];
#ifdef SERV_GROW_UP_SOCKET
					std::vector<int> vecTempSocketOption;
					g_pData->GetSocketItem()->GetGrowUpSocketData( vecSocketOption, vecTempSocketOption );
					for each( const int& socketOption in vecTempSocketOption )
#else SERV_GROW_UP_SOCKET
					for(UINT j=0; j<vecSocketOption.size(); ++j)
#endif SERV_GROW_UP_SOCKET
					{
#ifdef SERV_GROW_UP_SOCKET
#else SERV_GROW_UP_SOCKET
						const int socketOption = vecSocketOption[j];
#endif SERV_GROW_UP_SOCKET
						if( 0 == socketOption )
							continue;

						const CX2SocketItem::SocketData* pSocketData = g_pData->GetSocketItem()->GetSocketData( socketOption );
						if( pSocketData == NULL )
							continue;

						//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
						wstrAuraDesc += pSocketData->GetSocketDesc( iLevel_ );
						//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

						wstrAuraDesc += L"\n";
					}
				}

				CX2PetManager::PetSkillInfo petSkillInfo = pTemplet->m_AuraSkill[i];
				pButtonAura[i]->SetNormalTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );
				pButtonAura[i]->SetOverTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );
				pButtonAura[i]->SetDownTex( petSkillInfo.m_wstrImageName.c_str(), petSkillInfo.m_wstrKeyName.c_str() );		
				pButtonAura[i]->SetGuideDesc( wstrAuraDesc.c_str() );			
				pButtonAura[i]->SetGuideDescOffsetPos( D3DXVECTOR2( 50.f, 40.f ) );
			}			
		}
	}
#endif

#ifdef PET_DROP_ITEM_PICKUP	
	//펫 루팅 스킬 아이콘
	CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDLGUIPetInfo->GetControl( L"Pet_Btn_Item_Pickup" ));
	if( NULL != pButton )
	{
		//빈 슬롯으로 아이콘 초기화
		wstring BtnTextureFileName = L"DLG_UI_Common_Texture42_NEW.tga";
		wstring BtnTextureKeyName = L"PET_SKILL_SLOT_AURA";
		wstring BtnGuideDesc = L"";	

		//스킬을 사용 할 수 있는 유체 이상일 경우 스킬 봉인 상태에 따라 아이콘 설정
		int iStep = pTemplet->m_vecPetStatus[ kPetInfo->m_cEvolutionStep ];
		bool bIsAble = ( 1 <= iStep) ? true : false;

		if( true == bIsAble ) //유체 이상일 경우
		{
			//루팅 스킬에 대한 설명
			BtnGuideDesc = GET_STRING(STR_ID_21694) ;

			//루팅 스킬을 잠금 해제 한 상태라면
			if( true == kPetInfo->m_bAutoLooting ) 
			{
				BtnTextureFileName = L"DLG_SKILL_Pet_P_04.tga";
				BtnTextureKeyName = L"PET_LOOTING_ACTIVE";
				
			}
			else
			{
				BtnTextureFileName = L"DLG_SKILL_Pet_P_04.tga";
				BtnTextureKeyName = L"PET_LOOTING_ENACTIVE";
				//봉인 해제에 대한 설명
				BtnGuideDesc += GET_STRING(STR_ID_21695);
			}
		}

		pButton->SetEnable( bIsAble );
		pButton->SetNormalTex(BtnTextureFileName.c_str(), BtnTextureKeyName.c_str() );
		pButton->SetOverTex(BtnTextureFileName.c_str(), BtnTextureKeyName.c_str() );
		pButton->SetDownTex(BtnTextureFileName.c_str(), BtnTextureKeyName.c_str() );
		pButton->SetGuideDesc( BtnGuideDesc.c_str() );

		pButton->SetGuideDescOffsetPos( D3DXVECTOR2( 50.f, 40.f ) );

#ifdef SERV_EVENT_VC
		// 용병 뽀루는 먹이주기 버튼 비활성화
		if( kPetInfo->m_iPetID == CX2PetManager::PUI_PET_MERCENARY_PPORU_EVENT_INT )
		{
			pButton->SetEnable( false );
		}
#endif //SERV_EVENT_VC
	}
#endif //PET_DROP_ITEM_PICKUP

	float fSatiety = (float)kPetInfo->m_sSatiety / CX2PetManager::MAX_OF_SATIETY;
	float fIntimacy = (float)kPetInfo->m_iIntimacy / pTemplet->m_Evolution_Step_Max[ kPetInfo->m_cEvolutionStep ];
	
	bool bEvolution = true;
	// 소환버튼 활성화 여부 검사
	bool bSummoned = true;
	KPetInfo *petInfo = g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo();
	if( petInfo == NULL )
		bSummoned = false;
		
	if( bSummoned == false || ( petInfo != NULL && kPetInfo->m_iPetUID != petInfo->m_iPetUID) )
	{
		pButton1->SetShowEnable(true, true);
		pButton3->SetShowEnable(false, false);
		bEvolution = false;
	}
	else
	{
		pButton1->SetShowEnable(false, false);
		pButton3->SetShowEnable(true, true);
	}	

	// 진화버튼 활성화 여부 검사
	if( fIntimacy >= 1.f && kPetInfo->m_cEvolutionStep < pTemplet->m_Evolution_All_Step)
	{
		pButton2->SetShowEnable(true, true);
		pButton2->SetGuideDesc_LUA( STR_ID_5362 );
	}
	else
		pButton2->SetShowEnable(false, false);

	if( bEvolution == false )
	{
		pButton2->SetEnable( false );		
	}

	if( g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME || 
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY ||
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_RESULT ||
		g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_RESULT ||
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
	{
		pButton1->SetShowEnable(false, false);		
		pButton3->SetShowEnable(false, false);

		pButton2->SetEnable( false );	// 던전내에서는 진화할 없습니다.
		pButton2->SetGuideDesc_LUA( STR_ID_5609 );
	}

	// 내가 dungeon ready상태이면 소환/소환해제/진화 막는다.		
	if( g_pData->GetPartyManager()->DoIHaveParty() == true )
	{
		bool bMyReady = false;
		UidType iUnitUid = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		CX2PartyManager::PartyMemberData* pMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iUnitUid );
		if( NULL != pMemberData )
		{
			bMyReady = pMemberData->m_bGameReady;
		}
		if( bMyReady == true )
		{
			pButton1->SetShowEnable(false, false);
			pButton2->SetEnable( false );	
			pButton3->SetShowEnable(false, false);			
		}
	}		
	
	if( g_pX2Game == NULL && g_pX2Room != NULL && 
		g_pX2Room->GetMySlot() != NULL && 
		g_pX2Room->GetMySlot()->m_bHost == false && 
		g_pX2Room->GetMySlot()->m_bReady == true )
	{
		pButton1->SetShowEnable(false, false);		
		pButton3->SetShowEnable(false, false);

		pButton2->SetEnable( false );
	}
	

	// 펫이름
	if( pStaticName != NULL )
	{
		wstring wstrPetName = L"";

#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( (int)kPetInfo->m_iPetID, kPetInfo->m_cEvolutionStep, (float)kPetInfo->m_iIntimacy );
#else //SERV_PETID_DATA_TYPE_CHANGE
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( (int)kPetInfo->m_cPetID, kPetInfo->m_cEvolutionStep, (float)kPetInfo->m_iIntimacy );
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifdef CLIENT_COUNTRY_EU
		// 유럽은 펫 이름이 먼저 나오고 특수한 상태 문장이 뒤에 나오도록 수정 2011.10.15 임홍락
		wstrPetName += kPetInfo->m_wstrPetName;

		if( fIntimacy >= 0.7f )
		{
			wstrPetName += GET_STRING( STR_ID_5612 );
		}
#else
		if( fIntimacy >= 0.7f )
		{
			wstrPetName = GET_STRING( STR_ID_5612 );
			wstrPetName += L" ";
		}
		wstrPetName += kPetInfo->m_wstrPetName;
#endif CLIENT_COUNTRY_EU
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		int iStatus = g_pData->GetPetManager()->GetPetStatus( kPetInfo->m_iPetID, kPetInfo->m_cEvolutionStep );
#else //SERV_PETID_DATA_TYPE_CHANGE
		int iStatus = g_pData->GetPetManager()->GetPetStatus( kPetInfo->m_cPetID, kPetInfo->m_cEvolutionStep );
#endif //SERV_PETID_DATA_TYPE_CHANGE
		switch(iStatus)
		{
#ifdef CLIENT_COUNTRY_EU
		case 0:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5344 ) + L")";
			break;
		case 1:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5345 ) + L")";
			break;
		case 2:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5346 ) + L")";
			break;
		case 3:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5347 ) + L")";
			break;
#else
		case 0:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5344 ) + L")";
			break;
		case 1:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5345 ) + L")";
			break;
		case 2:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5346 ) + L")";
			break;
		case 3:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5347 ) + L")";
			break;
#endif CLIENT_COUNTRY_EU
		default:
			break;
		}

		pStaticName->GetString(0)->msg = wstrPetName;
	}
	
	// 펫 아이콘
	if( pPictureIcon != NULL )
	{
#ifdef DEFENCE_CODE_FOR_CRASH
		if( kPetInfo->m_cEvolutionStep < pTemplet->m_Evolution_Step_Image.size() )
		{
#endif //DEFENCE_CODE_FOR_CRASH
			CX2PetManager::PetStepImage petStepImage = pTemplet->m_Evolution_Step_Image[ kPetInfo->m_cEvolutionStep ];

			wstring wstrImageName = petStepImage.m_wstrImageName;
			if( petStepImage.m_wstrKeyName == L"" )
			{
				pPictureIcon->SetTex( wstrImageName.c_str() );
			}
			else
			{
				pPictureIcon->SetTex( wstrImageName.c_str(), petStepImage.m_wstrKeyName.c_str() );
			}
#ifdef DEFENCE_CODE_FOR_CRASH
		}
#endif //DEFENCE_CODE_FOR_CRASH
	}

	// 포만도 바
	if( pPicture1 != NULL )
	{
		D3DXVECTOR2 vOriSize = pPicture1->GetOriginalSize();
		pPicture1->SetSizeX( vOriSize.x * fSatiety );

		D3DXVECTOR2 vPos = pPicture1->GetPos();
		D3DXVECTOR2 vSize = pPicture1->GetSize();
		float xPos = vPos.x + vSize.x - 38.f;

		pStaticSatiety->SetOffsetPos( D3DXVECTOR2( vSize.x, 0.f ) );
#ifdef ALWAYS_TEMP_LINEBREAK
		pPictureTip1->SetPos( D3DXVECTOR2( xPos, 390.f ) );
#else //ALWAYS_TEMP_LINEBREAK
		pPictureTip1->SetPos( D3DXVECTOR2( xPos, pPictureTip1->GetPos().y ) );
#endif //ALWAYS_TEMP_LINEBREAK

		wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_5340, "i", (int)(fSatiety * 100.f) ) );	// 포만도	
#ifdef ALWAYS_TEMP_LINEBREAK
		int TempPos = wstrBuff.find(L":");
		if(TempPos != std::wstring::npos)
		{
			wstrBuff.insert(TempPos,L"\n");
		}
#endif ALWAYS_TEMP_LINEBREAK
		pStaticSatiety->GetString(0)->msg = wstrBuff;
	}

	// 친밀도 바
	if( pPicture2 != NULL )
	{
		D3DXVECTOR2 vOriSize = pPicture2->GetOriginalSize();
		pPicture2->SetSizeX( vOriSize.x * fIntimacy );

		D3DXVECTOR2 vPos = pPicture2->GetPos();
		D3DXVECTOR2 vSize = pPicture2->GetSize();
#ifdef ALWAYS_MISS_DEFINE_CHECK
		float xPos = vPos.x + vSize.x - 42.f;
#else //ALWAYS_MISS_DEFINE_CHEC
		float xPos = vPos.x + vSize.x - 38.f;
#endif ALWAYS_MISS_DEFINE_CHECK

		pStaticIntimacy->SetOffsetPos( D3DXVECTOR2( vSize.x, 0.f ) );
#ifdef ALWAYS_TEMP_LINEBREAK
		pPictureTip2->SetPos( D3DXVECTOR2( xPos, pPictureTip2->GetPos().y ) );
#else ALWAYS_TEMP_LINEBREAK
		pPictureTip2->SetPos( D3DXVECTOR2( xPos, 470.f ) );
#endif ALWAYS_TEMP_LINEBREAK

		wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_5341, "i", (int)(fIntimacy * 100.f) ) );	// 포만도	
#ifdef ALWAYS_TEMP_LINEBREAK
		int TempPos = wstrBuff.find(L":");
		if(TempPos != std::wstring::npos)
		{
			wstrBuff.insert(TempPos,L"\n");
		}
#endif ALWAYS_TEMP_LINEBREAK
		pStaticIntimacy->GetString(0)->msg = wstrBuff;
	}	

	// 친밀도 효과
	if( pPictureBarEffect != NULL )
	{
		if( fIntimacy >= 0.7 )		
		{
			pPictureBarEffect->SetFlicker( 0.9f, 1.4f, 0.3f );
			pPictureBarEffect->SetShow(true);
		}
		else
		{
			pPictureBarEffect->SetShow(false);
		}
	}

	// 펫 상태
	if( pStaticCondition != NULL )
	{
		wstring wstrComment1 = GET_STRING( STR_ID_5363 ); // 포만도
		wstring wstrComment2 = GET_STRING( STR_ID_5364 ); // 친밀도
#ifdef CLIENT_COUNTRY_EU
		wstrComment1 += L" ";
		wstrComment2 += L" ";
#endif //CLIENT_COUNTRY_EU

		// 포만도 상태 
		if( kPetInfo->m_sSatiety >= 0 && kPetInfo->m_sSatiety <= 960 )
		{
			// 굶주림
			wstrComment1 += GET_STRING( STR_ID_5353 );
		}
		else if( kPetInfo->m_sSatiety > 960 && kPetInfo->m_sSatiety <= 1920 )
		{
			// 배고픔
			wstrComment1 += GET_STRING( STR_ID_5354 );
		}
		else if( kPetInfo->m_sSatiety > 1920 && kPetInfo->m_sSatiety <= 2880 )
		{
			// 허전함
			wstrComment1 += GET_STRING( STR_ID_5355 );
		}
		else if( kPetInfo->m_sSatiety > 2880 && kPetInfo->m_sSatiety <= 3840 )
		{
			// 만족함
			wstrComment1 += GET_STRING( STR_ID_5356 );
		}
		else
		{
			// 가득참
			wstrComment1 += GET_STRING( STR_ID_5357 );
		}
		wstrComment1 += L"\n";

		if( fIntimacy <= 0.2f )
		{
			wstrComment2 += GET_STRING( STR_ID_5358 );
		}
		else if( fIntimacy > 0.2f && fIntimacy <= 0.5f )
		{
			wstrComment2 += GET_STRING( STR_ID_5359 );
		}
		else if( fIntimacy > 0.5f && fIntimacy <= 0.7f )
		{
			wstrComment2 += GET_STRING( STR_ID_5360 );
		}
		else
		{
			wstrComment2 += GET_STRING( STR_ID_5361 );
		}
#ifdef PET_SPECIAL_ACTION_INFO
		//진화 단계가 성체 이상이거나, 진화 하지 않는 완전체가 기본인 펫이라면
		if( kPetInfo->m_cEvolutionStep > 1 || pTemplet->m_Evolution_All_Step == 0 )
		{ 	
			//PetTemplet.lua의 SPECIAL_ACTION_COMMAND 값이 있으면
			if( false == pTemplet->m_SpecialActionCommand.empty() )
			{
				wstring wstrComment3 = GET_STRING(STR_ID_17423);
				vector<wstring>::iterator it = pTemplet->m_SpecialActionCommand.begin();
				for( ; it != pTemplet->m_SpecialActionCommand.end(); it++)
				{
					wstrComment3 += it->c_str();
					wstrComment3 += L" ";
				}
				pStaticCondition->SetOffsetPos(D3DXVECTOR2(0.f,-5.f));
#ifdef CLIENT_GLOBAL_LINEBREAK
				wstring wstrComment = wstrComment1 + wstrComment2 + wstrComment3;
				wstring wstrPetCondition = CWordLineHandler::GetStrByLineBreakInX2Main(wstrComment.c_str(), 450, 1);
				pStaticCondition->GetString(0)->msg = wstrPetCondition;
#else //CLIENT_GLOBAL_LINEBREAK
#ifdef ALWAYS_TEMP_LINEBREAK
				wstring wstrComment = wstrComment1 + wstrComment2 + wstrComment3;
				wstring TempStr;
				TempStr = g_pMain->GetStrByLienBreak( wstrComment.c_str(), 330, XUF_DODUM_11_NORMAL );
				pStaticCondition->GetString(0)->msg = TempStr;
#else //ALWAYS_TEMP_LINEBREAK
				pStaticCondition->GetString(0)->msg = wstrComment1 + wstrComment2 + wstrComment3;
#endif ALWAYS_TEMP_LINEBREAK
#endif //CLIENT_GLOBAL_LINEBREAK	
			}
			else
			{
				pStaticCondition->SetOffsetPos(D3DXVECTOR2(0.f,0.f));
#ifdef CLIENT_GLOBAL_LINEBREAK
				wstring wstrComment = wstrComment1 + wstrComment2;
				wstring wstrPetCondition = CWordLineHandler::GetStrByLineBreakInX2Main(wstrComment.c_str(), 450, 1);
				pStaticCondition->GetString(0)->msg = wstrPetCondition;
#else //CLIENT_GLOBAL_LINEBREAK
#ifdef ALWAYS_TEMP_LINEBREAK
				wstring wstrComment = wstrComment1 + wstrComment2;
				wstring TempStr;
				TempStr = g_pMain->GetStrByLienBreak( wstrComment.c_str(), 330, XUF_DODUM_11_NORMAL );
				pStaticCondition->GetString(0)->msg = TempStr;
#else //ALWAYS_TEMP_LINEBREAK
				pStaticCondition->GetString(0)->msg = wstrComment1 + wstrComment2;
#endif ALWAYS_TEMP_LINEBREAK
#endif //CLIENT_GLOBAL_LINEBREAK
			}
		}
		else
		{
			pStaticCondition->SetOffsetPos(D3DXVECTOR2(0.f,0.f));
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrComment = wstrComment1 + wstrComment2;
			wstring wstrPetCondition = CWordLineHandler::GetStrByLineBreakInX2Main(wstrComment.c_str(), 450, 1);
			pStaticCondition->GetString(0)->msg = wstrPetCondition;
#else //CLIENT_GLOBAL_LINEBREAK
#ifdef ALWAYS_TEMP_LINEBREAK
			wstring wstrComment = wstrComment1 + wstrComment2;
			wstring TempStr;
			TempStr = g_pMain->GetStrByLienBreak( wstrComment.c_str(), 330, XUF_DODUM_11_NORMAL );
			pStaticCondition->GetString(0)->msg = TempStr;
#else //ALWAYS_TEMP_LINEBREAK
			pStaticCondition->GetString(0)->msg = wstrComment1 + wstrComment2;
#endif ALWAYS_TEMP_LINEBREAK
#endif //CLIENT_GLOBAL_LINEBREAK
		}
#else
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring wstrComment = wstrComment1 + wstrComment2;
		wstring wstrPetCondition = CWordLineHandler::GetStrByLineBreakInX2Main(wstrComment.c_str(), 450, 1);
		pStaticCondition->GetString(0)->msg = wstrPetCondition;
#else //CLIENT_GLOBAL_LINEBREAK
#ifdef ALWAYS_TEMP_LINEBREAK
		wstring wstrComment = wstrComment1 + wstrComment2;
		wstring TempStr;
		TempStr = g_pMain->GetStrByLienBreak( wstrComment.c_str(), 330, XUF_DODUM_11_NORMAL );
		pStaticCondition->GetString(0)->msg = TempStr;
#else //ALWAYS_TEMP_LINEBREAK
		pStaticCondition->GetString(0)->msg = wstrComment1 + wstrComment2;
#endif ALWAYS_TEMP_LINEBREAK
#endif //CLIENT_GLOBAL_LINEBREAK
#endif //PET_SPECIAL_ACTION_INFO
	}
}

void CX2UIPetInfo::UpdatePetDetail( KPetInfo *kPetInfo )
{
	if( m_pDLGUIPetDetail == NULL )
		return;

	if( kPetInfo == NULL )
		return;

#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)kPetInfo->m_iPetID );
#else //SERV_PETID_DATA_TYPE_CHANGE
	CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)kPetInfo->m_cPetID );
#endif //SERV_PETID_DATA_TYPE_CHANGE
	if( pTemplet == NULL )
		return;

	CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGUIPetDetail->GetControl( L"g_pStatic_Pet_Propensity" );
	if( pStatic == NULL )
		return;

	m_bDetailPoint = true;

	CKTDGUIControl::CPictureData *pPicture1 = NULL;	//펫 아이콘		
	CKTDGUIControl::CPictureData *pPicture3 = NULL;	//성향 말풍선

	CKTDGUIButton *pButton1				= (CKTDGUIButton*)m_pDLGUIPetDetail->GetControl( L"Pet_Btn_Recall" );				// 소환버튼
	CKTDGUIButton *pButton2				= (CKTDGUIButton*)m_pDLGUIPetDetail->GetControl( L"Pet_Btn_Recall_Cancel" );		// 소환해제버튼

	CKTDGUIStatic *pStaticName			=  (CKTDGUIStatic*)m_pDLGUIPetDetail->GetControl( L"Pet_Propensity_Name" );			// 펫이름
	CKTDGUIStatic *pStaticGuide			=  (CKTDGUIStatic*)m_pDLGUIPetDetail->GetControl( L"Pet_Propensity_Guide" );		// 펫 성향
	CKTDGUIStatic *pStaticCoordinate	=  (CKTDGUIStatic*)m_pDLGUIPetDetail->GetControl( L"Pet_Propensity_Coordinate" );	// 친밀도 풍선말

	pPicture1 = pStatic->GetPictureIndex( 1 );	
	pPicture3 = pStatic->GetPictureIndex( 3 );



	// 펫 아이콘
	if( pPicture1 != NULL )
	{
#ifdef DEFENCE_CODE_FOR_CRASH
		if( kPetInfo->m_cEvolutionStep < pTemplet->m_Evolution_Step_Image.size() )
		{
#endif //DEFENCE_CODE_FOR_CRASH
			CX2PetManager::PetStepImage petStepImage = pTemplet->m_Evolution_Step_Image[ kPetInfo->m_cEvolutionStep ];

			wstring wstrImageName = petStepImage.m_wstrImageName;
			if( petStepImage.m_wstrKeyName == L"" )
			{
				pPicture1->SetTex( wstrImageName.c_str() );
			}
			else
			{
				pPicture1->SetTex( wstrImageName.c_str(), petStepImage.m_wstrKeyName.c_str() );
			}
#ifdef DEFENCE_CODE_FOR_CRASH
		}
#endif //DEFENCE_CODE_FOR_CRASH
	}

	// 펫이름
	if( pStaticName != NULL )
	{
#ifdef CLIENT_COUNTRY_EU
		// 유럽은 펫 이름이 먼저 나오고 특수한 상태 문장이 뒤에 나오도록 수정 2011.10.15 임홍락
		wstring wstrPetName = L"";
		wstrPetName += kPetInfo->m_wstrPetName;
#ifdef SERV_PETID_DATA_TYPE_CHANGE
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( (int)kPetInfo->m_iPetID, kPetInfo->m_cEvolutionStep, (float)kPetInfo->m_iIntimacy );
#else 
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( (int)kPetInfo->m_cPetID, kPetInfo->m_cEvolutionStep, (float)kPetInfo->m_iIntimacy );
#endif SERV_PETID_DATA_TYPE_CHANGE
		if( fIntimacy >= 0.7f )
		{
			wstrPetName += GET_STRING( STR_ID_5612 );
		}
#else	//CLIENT_COUNTRY_EU
		wstring wstrPetName = L"";

#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( (int)kPetInfo->m_iPetID, kPetInfo->m_cEvolutionStep, (float)kPetInfo->m_iIntimacy );
#else //SERV_PETID_DATA_TYPE_CHANGE
		float fIntimacy = g_pData->GetPetManager()->GetIntimacyPercent( (int)kPetInfo->m_cPetID, kPetInfo->m_cEvolutionStep, (float)kPetInfo->m_iIntimacy );
#endif //SERV_PETID_DATA_TYPE_CHANGE
		if( fIntimacy >= 0.7f )
		{
			wstrPetName = GET_STRING( STR_ID_5612 );
			wstrPetName += L" ";
		}
		wstrPetName += kPetInfo->m_wstrPetName;
#endif	//CLIENT_COUNTRY_EU
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		int iStatus = g_pData->GetPetManager()->GetPetStatus( kPetInfo->m_iPetID, kPetInfo->m_cEvolutionStep );
#else //SERV_PETID_DATA_TYPE_CHANGE
		int iStatus = g_pData->GetPetManager()->GetPetStatus( kPetInfo->m_cPetID, kPetInfo->m_cEvolutionStep );
#endif //SERV_PETID_DATA_TYPE_CHANGE
		switch(iStatus)
		{
#ifdef CLIENT_COUNTRY_EU
		case 0:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5344 ) + L")";
			break;
		case 1:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5345 ) + L")";
			break;
		case 2:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5346 ) + L")";
			break;
		case 3:
			wstrPetName = wstrPetName + L"\n(" + GET_STRING( STR_ID_5347 ) + L")";
			break;
#else //CLIENT_COUNTRY_EU
		case 0:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5344 ) + L")";
			break;
		case 1:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5345 ) + L")";
			break;
		case 2:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5346 ) + L")";
			break;
		case 3:
			wstrPetName = wstrPetName + L" (" + GET_STRING( STR_ID_5347 ) + L")";
			break;
#endif //CLIENT_COUNTRY_EU
		default:
			break;
		}

		pStaticName->GetString(0)->msg = wstrPetName;
	}

	// 소환버튼 활성화 여부 검사
	bool bSummoned = true;
	KPetInfo *petInfo = g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo();
	if( petInfo == NULL )
		bSummoned = false;
	if( bSummoned == false || 
		(petInfo != NULL && kPetInfo->m_iPetUID != petInfo->m_iPetUID) )
	{
		pButton1->SetShowEnable(true, true);
		pButton2->SetShowEnable(false, false);
	}
	else
	{
		pButton1->SetShowEnable(false, false);
		pButton2->SetShowEnable(true, true);
	}	
	
	if( g_pMain->GetNowStateID() == CX2Main::XS_SQUARE_GAME || 
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_LOBBY ||
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_RESULT ||
		g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_RESULT ||
		g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
	{
		pButton1->SetShowEnable(false, false);
		pButton2->SetShowEnable(false, false);
	}

	if( g_pX2Game == NULL && g_pX2Room != NULL && 
		g_pX2Room->GetMySlot() != NULL && 
		g_pX2Room->GetMySlot()->m_bHost == false && 
		g_pX2Room->GetMySlot()->m_bReady == true )
	{
		pButton1->SetShowEnable(false, false);
		pButton2->SetShowEnable(false, false);
	}

	// 내가 dungeon ready상태이면 소환/소환해제/진화 막는다.		
	if( g_pData->GetPartyManager()->DoIHaveParty() == true )
	{
		bool bMyReady = false;
		UidType iUnitUid = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		CX2PartyManager::PartyMemberData* pMemberData = g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData( iUnitUid );
		if( NULL != pMemberData )
		{
			bMyReady = pMemberData->m_bGameReady;
		}
		if( bMyReady == true )
		{
			pButton1->SetShowEnable(false, false);
			pButton2->SetShowEnable(false, false);
		}
	}		
	

	// 성향정보
	int iEmotion = kPetInfo->m_sEmotion;
	int iExtroversion = kPetInfo->m_sExtroversion;

	if( iEmotion < -4800 )
		iEmotion = -4800;
	else if( iEmotion > 4800 )
		iEmotion = 4800;

	if( iExtroversion < -4800 )
		iExtroversion = -4800;
	else if( iExtroversion > 4800 )
		iExtroversion = 4800;

	int iX = g_pData->GetPetManager()->GetDisposition(iEmotion  );
	int iY = g_pData->GetPetManager()->GetDisposition(iExtroversion  );

	wstring wstrGuide = GET_REPLACED_STRING( ( STR_ID_5371, "L", kPetInfo->m_wstrPetName ) );
	wstrGuide += L" ";

	if( iY == 0 )		
		wstrGuide += GET_STRING( STR_ID_5373 );	// 내향
	else		
		wstrGuide += GET_STRING( STR_ID_5372 );	// 외향
	wstrGuide += L" ";

	if( iX == 0 )		
		wstrGuide += GET_STRING( STR_ID_5375 );	// 이성
	else		
		wstrGuide += GET_STRING( STR_ID_5374 );	// 감성
	wstrGuide += L"\n";
	wstrGuide += GET_STRING( STR_ID_5376 );

	if( pStaticGuide != NULL )
	{
#ifdef CLIENT_GLOBAL_LINEBREAK
		// 펫 성향 정보 길어서 라인 브레이크 넣음 임홍락
		wstring wstrGuide2 = CWordLineHandler::GetStrByLineBreakInX2Main(wstrGuide.c_str(), 450, 1);
		pStaticGuide->GetString(0)->msg = wstrGuide2;
#else //CLIENT_GLOBAL_LINEBREAK
		pStaticGuide->GetString(0)->msg = wstrGuide;
#endif //CLIENT_GLOBAL_LINEBREAK
	}
	// UI Size : 330 (18+14, 162+14+55+55+55+55+55+55)
	float fXPos = 330.f * (float)(iEmotion + 4800) / 9600.f;
	float fYPos = 330.f * (float)(iExtroversion + 4800) / 9600.f;

	// 성향 좌표 
	WCHAR wcCoordinate[20] = {0,};	
	//wsprintf( wcCoordinate, L"(%d,%d)", iEmotion/48, iExtroversion/48 );
	StringCchPrintf( wcCoordinate, 20, L"(%d,%d)", iEmotion/48, iExtroversion/48 );
	if( pStaticCoordinate != NULL )
		pStaticCoordinate->GetString(0)->msg = wcCoordinate;	
	pStaticCoordinate->SetOffsetPos( D3DXVECTOR2(fXPos, -fYPos) );
	
	// 성향 말풍선 
	D3DXVECTOR2 vPos = pPicture3->GetOriginalPos();
	vPos.x += fXPos;
	vPos.y -= fYPos;
	pPicture3->SetPos( vPos );

	m_vDetailPoint = vPos;

	D3DXVECTOR2 vPoint = vPos + m_pDLGUIPetDetail->GetPos();


	CKTDGParticleSystem::CParticleEventSequence* pParticle = 
		g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"Pet_Ui_Effect01", vPoint.x, vPoint.y, 0  );
	if( pParticle != NULL )
	{
		if( m_hParticle != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hParticle);
			m_hParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;
		}

		pParticle->SetOverUI( true );
		m_hParticle = pParticle->GetHandle();
	}	
}

#ifdef RIDING_SYSTEM
bool CX2UIPetInfo::Select_Button( LPARAM lParam )
{
	if( NULL == m_pDLGUIRidingPetList )
		return false;

	CKTDGUIButton* pNewButton = reinterpret_cast<CKTDGUIButton*>( lParam );
	bool bSelectButton = false;

	if( NULL != pNewButton )
	{
		m_iSelectButtonIndex = pNewButton->GetDummyInt( 0 );
		UINT offsetIndex = m_iSelectButtonIndex + ( 6 * ( m_iNowPageRiding - 1 ) );
		if ( NULL != CX2RidingPetManager::GetInstance() )
		{
			std::vector< CX2RidingPetManager::sRidingPetInfo > vecRidingPetList;
			CX2RidingPetManager::GetInstance()->GetRidingPetList( vecRidingPetList );
			offsetIndex = min( offsetIndex, vecRidingPetList.size() - 1 );
			m_ViewRidingPetUid = vecRidingPetList[offsetIndex].m_iRidingPetUID;
			bSelectButton = true;
		}
		else
		{
			m_ViewRidingPetUid = -1;
			bSelectButton = false;
		}
	}

	return bSelectButton;
}

void CX2UIPetInfo::UpdateDLGRidingPetViewerUI()
{
	if( NULL == m_pRidingPetViewerUI )
	{
		m_pRidingPetViewerUI = CX2NPCUnitViewerUI::Create();
	}

	if( NULL == CX2RidingPetManager::GetInstance() )
		return;

	CX2RidingPetManager::sRidingPetInfo sInfo;
	if( false == CX2RidingPetManager::GetInstance()->GetRidingPetInfoByUID( m_ViewRidingPetUid, sInfo ) )
		return;

	CX2RidingPetManager::RidingPetTemplet *pTemplet = CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(sInfo.m_usRindingPetID) );
	if( NULL == pTemplet )
		return;

	m_pRidingPetViewerUI->ResetRidingPet( sInfo.m_usRindingPetID );
	m_pRidingPetViewerUI->SetShowObject( true );
	m_pRidingPetViewerUI->PlayAnim( L"Wait", CKTDGXSkinAnim::XAP_LOOP, true );

	if( m_pDLGUIRidingPetInfo != NULL )
	{
		m_pDLGUIRidingPetInfo->SetHasUnit( static_cast<CKTDGObject*>(m_pRidingPetViewerUI));

		m_vecViewerRidingPetSize.x = pTemplet->m_fViewerSizeX;
		m_vecViewerRidingPetSize.y = pTemplet->m_fViewerSizeY;
		m_vecViewerRidingPetSize.z = pTemplet->m_fViewerSizeZ;
		m_pRidingPetViewerUI->SetScale( m_vecViewerRidingPetSize );

		D3DXVECTOR3 vPos = m_pDLGUIRidingPetInfo->GetDummyPos(0);
		vPos.x += m_pDLGUIRidingPetInfo->GetPos().x;
		vPos.y += m_pDLGUIRidingPetInfo->GetPos().y;

		//뽀루일 경우만 위치 이동.
		if( CX2RidingPetManager::RPUI_ANCIENT_PPORU == pTemplet->m_Uid || CX2RidingPetManager::RPUI_ANCIENT_PPORU_NAVER == pTemplet->m_Uid )
			m_fMoveViewerPetPositionX = -40.f;
		else
			m_fMoveViewerPetPositionX = 0.f;

		m_pRidingPetViewerUI->SetPosition( vPos );
	}

	m_pRidingPetViewerUI->OnFrameMove( g_pKTDXApp->GetTime(), g_pKTDXApp->GetElapsedTime() );
}

void CX2UIPetInfo::Select_Tab( UidType eTab )
{
	if( NULL == m_pDLGUIPetList )
		return;
	if( NULL == m_pDLGUIPetInfo )
		return;
	if( NULL == m_pDLGUIPetDetail )
		return;
	if( NULL == m_pDLGUIRidingPetInfo )
		return;
	if( NULL == m_pDLGUIRidingPetList )
		return;

	m_pDLGUIPetList->SetShowEnable(false, false);
	m_pDLGUIPetInfo->SetShowEnable(false, false);
	m_pDLGUIPetDetail->SetShowEnable(false, false);
	m_pDLGUIRidingPetInfo->SetShowEnable( false, false );
	m_pDLGUIRidingPetList->SetShowEnable( false, false );

	m_bDetailPoint = false;
	CKTDGUIRadioButton* pRadioButtonTab = NULL;

	if( RIDING_PET_TAB_SELECT == m_eTabSelect )
	{
		pRadioButtonTab = static_cast<CKTDGUIRadioButton*>( m_pDLGUIRidingPetList->GetControl( L"RIDING_PET_LIST" ) );
		if( NULL != pRadioButtonTab )
			pRadioButtonTab->SetChecked( true );
		pRadioButtonTab = static_cast<CKTDGUIRadioButton*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_PET_LIST" ) );
		if( NULL != pRadioButtonTab )
			pRadioButtonTab->SetChecked( true );

		m_eRidingPetTab = static_cast<RIDING_PET_INFO_TAB>(eTab);

		switch ( m_eRidingPetTab )
		{
		case RPIT_LIST:
			{
				ViewRidingPetList( m_iNowPageRiding );
				m_pDLGUIRidingPetList->SetShowEnable( true, true );
			}
			break;

		case RPIT_INFO:
			{
				m_pDLGUIRidingPetInfo->SetShowEnable( true, true );

				SetRidingPetInfo();

				UpdateDLGRidingPetViewerUI();
			}
			break;

		default:
			break;
		}

	}
	else if( PET_TAB_SELECT == m_eTabSelect )
	{
		pRadioButtonTab = static_cast<CKTDGUIRadioButton*>( m_pDLGUIPetList->GetControl( L"PET_LIST" ) );
		if( NULL != pRadioButtonTab )
			pRadioButtonTab->SetChecked( true );
		pRadioButtonTab = static_cast<CKTDGUIRadioButton*>( m_pDLGUIPetInfo->GetControl( L"PET_LIST" ) );
		if( NULL != pRadioButtonTab )
			pRadioButtonTab->SetChecked( true );
		pRadioButtonTab = static_cast<CKTDGUIRadioButton*>( m_pDLGUIPetDetail->GetControl( L"PET_LIST" ) );
		if( NULL != pRadioButtonTab )
			pRadioButtonTab->SetChecked( true );

		switch( m_ePetTab )
		{
		case PIT_LIST:
			ViewListTab( m_iNowPage );
			break;
		case PIT_INFO:
			for(UINT i=0; i<m_vecPetList.size(); ++i)
			{
				if( m_vecPetList[i].m_iPetUID == m_ViewPetUid )
				{
					ViewInfoTab( &m_vecPetList[i] );
					return;
				}
			}
			break;
		case PIT_DETAIL:
			for(UINT i=0; i<m_vecPetList.size(); ++i)
			{
				if( m_vecPetList[i].m_iPetUID == m_ViewPetUid )
				{
					ViewDetailTab( &m_vecPetList[i] );
					return;
				}
			}
			break;
		default:
			break;
		}
	}
}

void CX2UIPetInfo::SetRidingPetRow( const UINT rowIndex, const int ridingpetId, const float stamina/* = 0*/ )
{
	if( NULL == m_pDLGUIRidingPetList )
		return;

	CKTDGUIButton* pPetButton			= NULL;
	CKTDGUIStatic* pName				= NULL;
	CKTDGUIStatic* pStamina				= NULL;
	CKTDGUIStatic* pCharacterPicture	= NULL;
	CKTDGUIStatic* pPage				= NULL;

	WCHAR cControlName[30] = {0,};

	bool bShow = ( ridingpetId < 0 )? false : true;

	if( NULL == CX2RidingPetManager::GetInstance() )
		return;

	CX2RidingPetManager::RidingPetTemplet *pTemplet = NULL;

	if( true == bShow )
	{
		pTemplet = CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(ridingpetId) );
		if( NULL == pTemplet )
			return;	
	}
	
	StringCchPrintf( cControlName, 30, L"Pet_Btn_List_Name%d", rowIndex );
	pPetButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetList->GetControl( cControlName ) );
	if( NULL != pPetButton )
		pPetButton->SetShowEnable( bShow, bShow );
	
	StringCchPrintf( cControlName, 30, L"Pet_List_Name%d", rowIndex );
	pName = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl( cControlName ) );
	if( NULL != pName && NULL != pName->GetString(0) )
	{
		if( true == bShow )
			pName->SetString( 0, pTemplet->m_wstrName.c_str() );
		pName->SetShowEnable( bShow, bShow );
	}

	StringCchPrintf( cControlName, 30, L"Pet_List_Stamina%d", rowIndex );
	pStamina = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl( cControlName ) );
	if( NULL != pStamina && NULL != pStamina->GetString(0) )
	{
		if( true == bShow )
		{
			float fMaxStamina = pTemplet->m_fMaxStamina;
			float fStaminaPercent =  ((stamina / fMaxStamina) * 100.f );
			pStamina->SetString( 0,GET_REPLACED_STRING( (STR_ID_24343, "i", static_cast<int>(fStaminaPercent) ) ) );
		}
		pStamina->SetShowEnable( bShow, bShow );
	}

	pCharacterPicture = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl( L"Pet_List_Character_Win" ) );
	if( NULL != pCharacterPicture )
	{
		if( NULL != pCharacterPicture->GetPicture( rowIndex ) )
		{
			pCharacterPicture->GetPicture( rowIndex )->SetShow( bShow );
			if( true == bShow )
				pCharacterPicture->GetPicture( rowIndex )->SetTex( pTemplet->m_FaceImage.m_wstrImageName.c_str(), pTemplet->m_FaceImage.m_wstrKeyName.c_str() );	
		}
	}

	pPage = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl( L"Pet_Page_Num" ) );
	if( NULL != pPage )
	{
		if( NULL != pPage->GetString(0) )
		{
			StringCchPrintf( cControlName, 30, L"%d / %d", m_iNowPageRiding, m_iMaxPageRiding );
				pPage->GetString(0)->msg = cControlName;
		}
	}
}

void CX2UIPetInfo::Enable_ButtonBG( int rowIndex, bool _enable )
{
	if( NULL == m_pDLGUIRidingPetList )
		return;

	CKTDGUIStatic* pEnable			= NULL;

	pEnable = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl( L"ButtonBG_Disable" ) );
	if( NULL != pEnable )
	{
		if( NULL != pEnable->GetPicture( rowIndex ) )
			pEnable->GetPicture( rowIndex )->SetShow( _enable );
	}
}

void CX2UIPetInfo::UpdateStamina_List()
{
	if( NULL == m_pDLGUIRidingPetList )
		return;

	if ( NULL == CX2RidingPetManager::GetInstance() )
		return;

	std::vector< CX2RidingPetManager::sRidingPetInfo > vecRidingPetList;
	CX2RidingPetManager::GetInstance()->GetRidingPetList( vecRidingPetList );

	if( vecRidingPetList.size() > 0 ) //탈 것이 있을때만
	{
		CKTDGUIStatic* pStamina = NULL;
		WCHAR cControlName[30] = {0,};

		int staticIndex = 0;
		UINT minIndex = ( 6 * ( m_iNowPageRiding - 1 ) );
		UINT maxIndex = ( 6 * m_iNowPageRiding ) - 1;
		maxIndex = min( maxIndex, vecRidingPetList.size() - 1 );

		CX2RidingPetManager::RidingPetTemplet *pTemplet = NULL; 
		float fMaxStamina = 120.f;
		float fEnableStamina = CX2RidingPetManager::GetInstance()->GetEnableStamina();

		for( minIndex; minIndex <= maxIndex; ++minIndex )
		{
			StringCchPrintf( cControlName, 30, L"Pet_List_Stamina%d", staticIndex );
			if( vecRidingPetList[minIndex].m_fStamina < fEnableStamina )
				Enable_ButtonBG( staticIndex, true );
			else
				Enable_ButtonBG( staticIndex, false );

			++staticIndex;

			pTemplet = CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(vecRidingPetList[minIndex].m_usRindingPetID) );
			if( NULL != pTemplet )
			{
				fMaxStamina = pTemplet->m_fMaxStamina;
			}

			pStamina = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl( cControlName ) );
			if( NULL != pStamina && NULL != pStamina->GetString(0) )
			{
				float fStaminaPercent =  ((vecRidingPetList[minIndex].m_fStamina / fMaxStamina) * 100.f );
				pStamina->SetString( 0,GET_REPLACED_STRING( (STR_ID_24343, "i", static_cast<int>(fStaminaPercent) ) ) );
				pStamina->SetShowEnable( true, true );
			}
		
		}
	
		for( staticIndex; staticIndex < 6; ++staticIndex )
		{
			StringCchPrintf( cControlName, 30, L"Pet_List_Stamina%d", staticIndex );

			pStamina = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl( cControlName ) );
			if( NULL != pStamina && NULL != pStamina->GetString(0) )
			{
				pStamina->SetShowEnable( false, false );
			}
		}
	}
}

void CX2UIPetInfo::UpdatePeriod_List()
{
	if( NULL == m_pDLGUIRidingPetList )
		return;

	if ( NULL == CX2RidingPetManager::GetInstance() )
		return;

	UINT i = 0;

	std::vector< CX2RidingPetManager::sRidingPetInfo > vecRidingPetList;
	CX2RidingPetManager::GetInstance()->GetRidingPetList( vecRidingPetList );

	if( vecRidingPetList.size() > 0 ) //탈 것이 있을때만
	{
		m_iMaxPageRiding = static_cast<int>( ceil( vecRidingPetList.size() / 6.f ) );
		m_iMaxPageRiding = max( 1, m_iMaxPageRiding );

		UINT iMin,iMax,iRow = 0;
		iMin = 6 * ( m_iNowPageRiding - 1 );
		iMax = min( static_cast<UINT>(6 * m_iNowPageRiding), vecRidingPetList.size() );

		int day,hour,minute,index=0;
		CTime DestroyTime;
		CTime ctCurrentTime( g_pData->GetServerCurrentTime() );

		CKTDGUIStatic* pStaticPeriod = NULL;
		WCHAR wcharName[100] = {0,};
		wstring period;

		for( i = iMin; i < iMax; ++i )
		{
			StringCchPrintf( wcharName, 100, L"Pet_List_period%d", index );
			pStaticPeriod = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl(wcharName) );
			if( NULL != pStaticPeriod )
			{
				if( true == KncUtil::ConvertStringToCTime(vecRidingPetList[i].m_wstrDestroyDate, DestroyTime) )
				{
					CTimeSpan temp = DestroyTime - ctCurrentTime; 
					day = static_cast<int>(temp.GetDays());
					hour = static_cast<int>(temp.GetHours());
					minute = static_cast<int>(temp.GetMinutes());

					if( day > 0 )
					{
						if( NULL != pStaticPeriod->GetString(1) )
						{
							StringCchPrintf( wcharName, 100, L"%s%s", GET_STRING(STR_ID_309), GET_REPLACED_STRING( (STR_ID_24533, "i", day) ) );
							pStaticPeriod->SetString(1,wcharName);
						}
					}
					else if( day == 0 && hour > 0 )
					{
						if( NULL != pStaticPeriod->GetString(1) )
						{
							StringCchPrintf( wcharName, 100, L"%s%s", GET_STRING(STR_ID_309), GET_REPLACED_STRING( (STR_ID_24534, "i", hour) ) );
							pStaticPeriod->SetString(1,wcharName);
						}
					}
					else if( day == 0 && hour == 0 && minute > 0 )
					{
						if( NULL != pStaticPeriod->GetString(1) )
						{
							StringCchPrintf( wcharName, 100, L"%s%s", GET_STRING(STR_ID_309), GET_REPLACED_STRING( (STR_ID_366, "i", minute) ) );
							pStaticPeriod->SetString(1,wcharName);
						}
					}
					else
					{
						if( NULL != pStaticPeriod->GetString(1) )
						{
							StringCchPrintf( wcharName, 100, L"%s%s", GET_STRING(STR_ID_309), GET_REPLACED_STRING( (STR_ID_366, "i", 0) ) );
							pStaticPeriod->SetString(1,wcharName);
						}
					}

					pStaticPeriod->SetShow(true);
				}
				else
				{
					pStaticPeriod->SetShow(false);
				}
			}

			day = 0; hour = 0; minute = 0;
			++index;
		}

		for(index; index < 6; ++index)
		{
			StringCchPrintf( wcharName, 30, L"Pet_List_period%d", index );
			pStaticPeriod = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl(wcharName) );
			if( NULL != pStaticPeriod )
			{
				pStaticPeriod->SetShow(false);
			}
		}
	}
	else
	{
		WCHAR wcharName[100] = {0,};
		CKTDGUIStatic* pStaticPeriod = NULL;
		for(int index = 0; index < 6; ++index)
		{
			StringCchPrintf( wcharName, 30, L"Pet_List_period%d", index );
			pStaticPeriod = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetList->GetControl(wcharName) );
			if( NULL != pStaticPeriod )
			{
				pStaticPeriod->SetShow(false);
			}
		}
	}
}

void CX2UIPetInfo::UpdateStamina_Info( const float _Stamina, const float _MaxStamina )
{
	if( NULL == m_pDLGUIRidingPetInfo )
		return;

	CKTDGUIStatic* pStamina = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_Stamina" ) );
	if( NULL == pStamina )
		return;
	CKTDGUIStatic* pStaminaNum = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_StaminaNum" ) );
	if( NULL == pStaminaNum )
		return;

	CKTDGUIControl::CPictureData* pPictureStamina = pStamina->GetPicture(1);
	if( NULL == pPictureStamina )
		 return;

	CKTDGUIControl::CPictureData* pPicturePungsun = pStamina->GetPicture(2);
	if( NULL == pPicturePungsun )
		return;

	float fStamina = _Stamina;
	float fMaxStamina = _MaxStamina;
	float fStaminaPercent = fStamina / fMaxStamina;
	D3DXVECTOR2 vec2Size = pPictureStamina->GetOriginalSize();
	pPictureStamina->SetSizeX( vec2Size.x * fStaminaPercent );

	D3DXVECTOR2 vPos = pPictureStamina->GetPos();
	float fSize = pPictureStamina->GetSize().x - 2.f;
	float fPosX = vPos.x + fSize - 43.f;

	pStaminaNum->SetOffsetPos( D3DXVECTOR2( fSize+3.f, 0.f ) );
	pPicturePungsun->SetPos( D3DXVECTOR2( fPosX, 390.f ) );

	wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_24343, "i", static_cast<int>(fStaminaPercent*100.f) ) );

	if( NULL != pStaminaNum->GetString(0) )
		pStaminaNum->SetString( 0, wstrBuff.c_str() );


	pStaminaNum = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_StaminaNum_Bar" ) );
	if( NULL != pStaminaNum )
	{
		WCHAR Stamina[30] = {0,};
		if( NULL != pStaminaNum->GetString(0) )
		{
			StringCchPrintf( Stamina, 30, L"%d / %d", static_cast<int>(fStamina), static_cast<int>(fMaxStamina) );
			pStaminaNum->SetString( 0, Stamina);
		}
	}
	
}

void CX2UIPetInfo::SetRidingPetInfo()
{
	if( NULL == m_pDLGUIRidingPetInfo )
		return;

	if( NULL != CX2RidingPetManager::GetInstance() )
	{
		float fEnableStamina = CX2RidingPetManager::GetInstance()->GetEnableStamina();


		CX2RidingPetManager::sRidingPetInfo sInfo;
		if( false == CX2RidingPetManager::GetInstance()->GetRidingPetInfoByUID( m_ViewRidingPetUid, sInfo ) )
			return;

		CX2RidingPetManager::RidingPetTemplet *pTemplet = CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(sInfo.m_usRindingPetID) );
		if( NULL != pTemplet )
		{
			CKTDGUIStatic* pStatic = NULL;
			CKTDGUIButton* pButton = NULL;

			pStatic = static_cast<CKTDGUIStatic*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_Basic_Name" ) );
			if( NULL != pStatic )
			{
				//이름
				if( NULL != pStatic->GetString(0) )
					pStatic->SetString( 0, pTemplet->m_wstrName.c_str() );

				//캐릭터 이미지
				if( NULL != pStatic->GetPicture(0) )
					pStatic->GetPicture(0)->SetTex( pTemplet->m_FaceImage.m_wstrImageName.c_str(), pTemplet->m_FaceImage.m_wstrKeyName.c_str() );	

				//설명
				if( NULL != pStatic->GetString(1) )
				{
					pStatic->SetString( 1, pTemplet->m_wstrDescription.c_str() );
#ifdef RIDINGPET_INFO_ADD_LINEBREAK
					wstring wstrSkillDesc = CWordLineHandler::GetStrByLineBreakInX2Main(pStatic->GetString(1)->msg.c_str(), 450, 1);
					pStatic->GetString(1)->msg = wstrSkillDesc;
#endif //RIDINGPET_INFO_ADD_LINEBREAK
				}
			}

			//버프스킬 1,2,3,
			pButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_Btn_BUFF_Skill_1" ) );
			if( NULL != pButton )
			{
				pButton->SetNormalTex( pTemplet->m_vecBuff[0].m_wstrImageName.c_str(), pTemplet->m_vecBuff[0].m_wstrKeyName.c_str() );
				pButton->SetOverTex( pTemplet->m_vecBuff[0].m_wstrImageName.c_str(), pTemplet->m_vecBuff[0].m_wstrKeyName.c_str() );
				pButton->SetDownTex( pTemplet->m_vecBuff[0].m_wstrImageName.c_str(), pTemplet->m_vecBuff[0].m_wstrKeyName.c_str() );
				pButton->SetGuideDesc( pTemplet->m_vecBuff[0].m_wstrSkillDesc.c_str() );
			}

			pButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_Btn_BUFF_Skill_2" ) );
			if( NULL != pButton )
			{
				pButton->SetNormalTex( pTemplet->m_vecBuff[1].m_wstrImageName.c_str(), pTemplet->m_vecBuff[1].m_wstrKeyName.c_str() );
				pButton->SetOverTex( pTemplet->m_vecBuff[1].m_wstrImageName.c_str(), pTemplet->m_vecBuff[1].m_wstrKeyName.c_str() );
				pButton->SetDownTex( pTemplet->m_vecBuff[1].m_wstrImageName.c_str(), pTemplet->m_vecBuff[1].m_wstrKeyName.c_str() );
				pButton->SetGuideDesc( pTemplet->m_vecBuff[1].m_wstrSkillDesc.c_str() );
			}


			//공격스킬 z,x,c
			pButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_Btn_Skill_Z" ) );
			if( NULL != pButton )
			{
				pButton->SetNormalTex( pTemplet->m_vecAttackSkill[0].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[0].m_wstrKeyName.c_str() );
				pButton->SetOverTex( pTemplet->m_vecAttackSkill[0].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[0].m_wstrKeyName.c_str() );
				pButton->SetDownTex( pTemplet->m_vecAttackSkill[0].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[0].m_wstrKeyName.c_str() );
				pButton->SetGuideDesc( pTemplet->m_vecAttackSkill[0].m_wstrSkillDesc.c_str() );
			}

			pButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_Btn_Skill_X" ) );
			if( NULL != pButton )
			{
				pButton->SetNormalTex( pTemplet->m_vecAttackSkill[1].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[1].m_wstrKeyName.c_str() );
				pButton->SetOverTex( pTemplet->m_vecAttackSkill[1].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[1].m_wstrKeyName.c_str() );
				pButton->SetDownTex( pTemplet->m_vecAttackSkill[1].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[1].m_wstrKeyName.c_str() );
				pButton->SetGuideDesc( pTemplet->m_vecAttackSkill[1].m_wstrSkillDesc.c_str() );
			}

			pButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetInfo->GetControl( L"RIDING_Pet_Btn_Skill_C" ) );
			if( NULL != pButton )
			{
				pButton->SetNormalTex( pTemplet->m_vecAttackSkill[2].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[2].m_wstrKeyName.c_str() );
				pButton->SetOverTex( pTemplet->m_vecAttackSkill[2].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[2].m_wstrKeyName.c_str() );
				pButton->SetDownTex( pTemplet->m_vecAttackSkill[2].m_wstrImageName.c_str(), pTemplet->m_vecAttackSkill[2].m_wstrKeyName.c_str() );
				pButton->SetGuideDesc( pTemplet->m_vecAttackSkill[2].m_wstrSkillDesc.c_str() );
			}

			//탑승 버튼 처리
			bool bRide = ( sInfo.m_fStamina < fEnableStamina )? false : true;

			if( m_ViewRidingPetUid == CX2RidingPetManager::GetInstance()->GetRidingPetId() )
				SelectKindRideButton( BUTTON_RIDE_OFF, bRide );
			else
				SelectKindRideButton( BUTTON_RIDE_ON, bRide );

			//스테미너 처리
			m_fMaxStamina = pTemplet->m_fMaxStamina;
			UpdateStamina_Info( sInfo.m_fStamina, m_fMaxStamina );
		}
	}
}

void CX2UIPetInfo::ViewRidingPetList( int iPage )
{
	int iViewPage = iPage;
	if( iViewPage < 1 )
		iViewPage = m_iNowPageRiding;

	float fEnalbeStamina;

	CX2RidingPetManager* pRidingPetManager = CX2RidingPetManager::GetInstance();

	if( NULL != pRidingPetManager )
		fEnalbeStamina = pRidingPetManager->GetEnableStamina();
	else
		fEnalbeStamina = 10.f; //ckdgksdl

	UINT i = 0;
	for( i = 0; i < 6; ++i )
	{
		Enable_ButtonBG( i, false );
		SetRidingPetRow( i, -1, 10 );
	}

	// 정렬된 리스트 얻기
	std::vector< CX2RidingPetManager::sRidingPetInfo > vecRidingPetList;
	if( NULL != pRidingPetManager )
		pRidingPetManager->GetSortedRidingPetList(vecRidingPetList);
	

	m_iMaxPageRiding = static_cast<int>( ceil( vecRidingPetList.size() / 6.f ) );
	m_iMaxPageRiding = max( 1, m_iMaxPageRiding );

	UINT iMin,iMax,iRow = 0;
	iMin = 6 * ( m_iNowPageRiding - 1 );
	iMax = min( static_cast<UINT>(6 * m_iNowPageRiding), vecRidingPetList.size() );
	for( i = iMin; i < iMax; ++i )
	{
		SetRidingPetRow( iRow, vecRidingPetList[i].m_usRindingPetID, vecRidingPetList[i].m_fStamina );
		if( vecRidingPetList[i].m_fStamina < fEnalbeStamina )
			Enable_ButtonBG( iRow, true );
		++iRow;
	}

	for( ; iRow < 6; ++iRow )
	{
		Enable_ButtonBG( iRow, true );
	}

	UpdatePeriod_List();
}

void CX2UIPetInfo::Handler_EGS_GET_RIDING_PET_LIST_REQ( UINT iViewPage )
{
	KEGS_GET_RIDING_PET_LIST_REQ kPacket;
	kPacket.m_byteViewPage = iViewPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_RIDING_PET_LIST_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_RIDING_PET_LIST_ACK, 60.f );
}

bool CX2UIPetInfo::Handler_EGS_GET_RIDING_PET_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_GET_RIDING_PET_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_GET_RIDING_PET_LIST_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			if ( NULL != CX2RidingPetManager::GetInstance() )
			{
				CX2RidingPetManager* pRidingPetManager = CX2RidingPetManager::GetInstance();
				SortRidingPetListByServerData( kEvent.m_vecRidingPetList );

				// 클라에서 사용 할 자료형으로 변경
				std::vector<CX2RidingPetManager::sRidingPetInfo> vecRidingPetInfo;
				BOOST_FOREACH( KRidingPetInfo RidingPetInfo, kEvent.m_vecRidingPetList )
				{
					CX2RidingPetManager::sRidingPetInfo sPetInfo = RidingPetInfo;
					vecRidingPetInfo.push_back( sPetInfo );
				}

				if ( NULL != pRidingPetManager )
				{
					pRidingPetManager->SetRidingPetList( vecRidingPetInfo );
					pRidingPetManager->SortRidingPetList();
				}
			}			
		}
	}
	return true;
}
void CX2UIPetInfo::SortRidingPetListByServerData( OUT std::vector<KRidingPetInfo>& vecRidingPetList_ )
{
	// Index로 바꾸기 전 처음 서버에서 받은 date를 가지고 있을 때의 정렬 함수.
	// 큰 값이 앞으로 가도록 정렬
	auto UnSummonDateFirstSortFunc = [](const KRidingPetInfo& lhs_, const KRidingPetInfo& rhs_ )->bool
	{
		return lhs_.m_iLastUnSummonDate > rhs_.m_iLastUnSummonDate ;
	};

	std::sort( vecRidingPetList_.begin(), vecRidingPetList_.end(), UnSummonDateFirstSortFunc );

	int i = 0;
	BOOST_FOREACH( KRidingPetInfo& RidingPetInfo, vecRidingPetList_ )
	{
		RidingPetInfo.m_iLastUnSummonDate = i++;
	}
}

void CX2UIPetInfo::SelectKindRideButton( KIND_OF_RIDE_BUTTON  _select, bool _enable /*= true*/  )
{
	if( NULL != m_pDLGUIRidingPetInfo )
	{
		if( true == m_pDLGUIRidingPetInfo->GetShow() )
		{
			// 마을에서는 무조건 탑승 가능하도록 변경
			if( NULL != g_pTFieldGame && NULL == g_pX2Game )
				_enable = true;

			if( _select != BUTTON_RIDE_NONE )
				m_eKindRideButton = _select;

			//탑승 버튼 처리
			CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetInfo->GetControl( L"Pet_Btn_Ride" ) );

			if( NULL != pButton )
			{
				switch (m_eKindRideButton)
				{
				case BUTTON_RIDE_ON:
					{
						pButton->SetNormalTex( L"DLG_UI_Button_RidingPet_01.tga", L"RIDING_GET_ON_NORMAL" );
						pButton->SetOverTex( L"DLG_UI_Button_RidingPet_01.tga", L"RIDING_GET_ON_OVER" );
						pButton->SetDownTex( L"DLG_UI_Button_RidingPet_01.tga", L"RIDING_GET_ON_OVER" );
						pButton->SetCustomMsgMouseUp( PLUCM_RIDING_PET_RIDE );
					}
					break;

				case BUTTON_RIDE_OFF:
					{
						pButton->SetNormalTex( L"DLG_UI_Button_RidingPet_01.tga", L"RIDING_GET_OFF_NORMAL" );
						pButton->SetOverTex( L"DLG_UI_Button_RidingPet_01.tga", L"RIDING_GET_OFF_OVER" );
						pButton->SetDownTex( L"DLG_UI_Button_RidingPet_01.tga", L"RIDING_GET_OFF_OVER" );
						pButton->SetCustomMsgMouseUp( PLUCM_RIDING_PET_RIDE_OFF );
					}
					break;

				default:
					break;
				}

				pButton->SetEnable( _enable );
			}
		}
	}
}

const bool CX2UIPetInfo::GetShowRPInfo()
{
	if( NULL != m_pDLGUIRidingPetInfo )
		return m_pDLGUIRidingPetInfo->GetShow();
	else
		return false;
}

const bool CX2UIPetInfo::GetShowRPList()
{
	if( NULL != m_pDLGUIRidingPetList )
		return m_pDLGUIRidingPetList->GetShow();
	else 
		return false;
}
void CX2UIPetInfo::UpdateRidingButtonGuideDesc( bool IsSetKey_ )
{
	if( NULL == m_pDLGUIRidingPetInfo )
		return;

	CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>( m_pDLGUIRidingPetInfo->GetControl( L"Pet_Btn_Ride" ) );
	if( NULL != pButton )
	{
		if( true == IsSetKey_)
		{
			pButton->SetGuideDesc( GET_REPLACED_STRING( ( STR_ID_24524, "L", g_pMain->GetKeyPad()->GetKeyString( GA_RIDING ) ) ) );
		}
		else
		{
			pButton->SetGuideDesc( GET_STRING(STR_ID_24531) );
		}
	}
}

#endif //RIDING_SYSTEM

#endif

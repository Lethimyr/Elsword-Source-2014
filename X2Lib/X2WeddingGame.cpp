#include "StdAfx.h"
#include ".\x2WeddingGame.h"

#ifdef ADDED_RELATIONSHIP_SYSTEM
CX2WeddingGame::CX2WeddingGame( UDPData* pUdpData, SEnum::WEDDING_HALL_TYPE eHallType, SEnum::WEDDING_OFFICIANT_TYPE eOfficiantType_ ):
m_fOfficiantCameraTime( -1.f ),
m_pWeddingManager( NULL ),
m_pWorld( NULL ),
m_pCamera( NULL ),
m_pMajorParticle( NULL ),	
m_pMinorParticle( NULL ),	
m_pMajorXMeshPlayer( NULL ),	
m_pMinorXMeshPlayer( NULL ),	
m_pFontForUnitName( NULL ),	
m_fP2PSendGap(0.f),		
m_fReserveTime(0.f),
m_bFreeCamera(false),
m_bEnableKeyProcess(true),
m_bEnableCameraProcess(true),
m_bLeaveWeddingGame(false),
m_iReturnStateID(CX2Main::XS_WEDDING_GAME),
m_iWeddingHeroineNum( 0 ),
m_iNessesaryHeroineNum( 2 )
{
	g_pWeddingGame			= this;

	// 결혼식 매니져 설정.
	m_pWeddingManager = new CX2WeddingManager;
	m_pWeddingManager->OpenScriptFile( L"Wedding_OfficiantTemplet.lua", 
										L"Wedding_HallTemplet.lua",
										L"Wedding_Manager.lua");

	m_pWeddingManager->SetOfficiant( eOfficiantType_ );
	m_pWeddingManager->SetWeddingHallAndOfficiantInfo( eHallType );

	m_udpData = *pUdpData;
	
	//m_SquareData.CopySquareData( pSquareData );
	m_pMyUnit.reset();

	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger( CX2TalkBoxManager::TBMT_DEFAULT );

	m_pWorld = g_pData->GetWorldManager()->CreateWorld( m_pWeddingManager->GetCurrentWorldID(), NULL, true );
	m_pCamera = new CX2Camera();
	if( m_pWorld != NULL )
	{
		m_pWorld->PlayBGM();
		m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );
	}

	srand((unsigned int)time(NULL));

	m_pFontForUnitName = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );


	g_pData->GetGameUDP()->ClearPeer();

	g_pData->GetGameUDP()->SetMyUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    g_pData->GetGameUDP()->SetRelayIPAddress( CKTDNUDP::ConvertIPToAddress( m_udpData.m_RelayIP.c_str() ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	g_pData->GetGameUDP()->SetRelayIP( m_udpData.m_RelayIP.c_str() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	g_pData->GetGameUDP()->SetRelayPort( m_udpData.m_Port );
	g_pData->GetGameUDP()->ConnectTestToRelay();

	g_pKTDXApp->GetDIManager()->SetEnable( true );


	g_pKTDXApp->SkipFrame();
}

CX2WeddingGame::~CX2WeddingGame(void)
{
	m_pFontForUnitName = NULL;

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );

	SAFE_DELETE( m_pWorld );
	SAFE_DELETE( m_pCamera );
	SAFE_DELETE( m_pWeddingManager );
	if( null != m_pMyUnit )
		m_pMyUnit.reset();

	m_UserUnitList.clear();

	if( NULL != g_pData->GetGameUDP() )
		g_pData->GetGameUDP()->DisconnectToRelay();
	
	if( NULL != g_pKTDXApp->GetDIManager() )
		g_pKTDXApp->GetDIManager()->SetEnable( true );

	if( NULL != g_pData->GetUIEffectSet() )
		g_pData->GetUIEffectSet()->StopEffectSetAll();

	g_pWeddingGame = NULL;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_NONE ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

}

void CX2WeddingGame::JoinSquareUnit( KSquareUserInfo* pKSquareUserInfo )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( pKSquareUserInfo->m_iUnitUID );
	if( pCX2SquareUnit != NULL )
		return;

	if( pKSquareUserInfo->m_iUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	{
		pCX2SquareUnit = AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true );
	}
	else
	{			
		CX2Unit* pCX2Unit = new CX2Unit( *pKSquareUserInfo );
		if( NULL != pCX2Unit )
		{
			pCX2Unit->ResetEqip();
			pCX2SquareUnit = AddUnit( pCX2Unit, false, false );
		}
	}
}

void CX2WeddingGame::JoinSquareUnit( KRoomUserInfo* pKRoomUserInfo )
{
	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( pKRoomUserInfo->m_nUnitUID );
	if( pCX2SquareUnit != NULL )
		return;

	if( pKRoomUserInfo->m_nUnitUID == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	{
		pCX2SquareUnit = AddUnit( g_pData->GetMyUser()->GetSelectUnit(), true, true, pKRoomUserInfo->m_cWeddingStatus );
	}
	else
	{			
		CX2Unit* pCX2Unit = new CX2Unit( *pKRoomUserInfo );
		if( NULL != pCX2Unit )
		{
			pCX2Unit->ResetEqip();
			pCX2SquareUnit = AddUnit( pCX2Unit, false, false, pKRoomUserInfo->m_cWeddingStatus );
		}
	}
}

CX2SquareUnit* CX2WeddingGame::AddUnit( CX2Unit* pUnit, bool bMyUnit, bool bInit, char cWeddingStatus/* = 0*/ )
{
	if( NULL == pUnit )
		return NULL;

#ifdef RIDING_SYSTEM
	CX2SquareUnitPtr pCX2SquareUnit = 
		CX2SquareUnit::CreateSquareUnit( bMyUnit, pUnit, CX2SquareUnit::GetSquareUnitScriptFileName( pUnit->GetClass() ), 
										false, cWeddingStatus );
#else //RIDING_SYSTEM
	CX2SquareUnitPtr pCX2SquareUnit = 
		CX2SquareUnit::CreateSquareUnit( bMyUnit, pUnit, CX2SquareUnit::GetSquareUnitScriptFileName( pUnit->GetClass() ).c_str(), 
		false, cWeddingStatus );
#endif //RIDING_SYSTEM

	if( bInit == true )
		pCX2SquareUnit->Init();

	pCX2SquareUnit->StateChange( pCX2SquareUnit->GetStateID().m_Wait  );

	m_UserUnitList.push_back( pCX2SquareUnit );
	g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2SquareUnit.get() );

	if( bMyUnit == true )
	{
		m_pMyUnit = pCX2SquareUnit;

        m_pMyUnit->OnFrameMove( g_NowTime, 0.f );
        m_pCamera->SquareCameraUpdate( m_pMyUnit.get(), 1200.0f, 200.0f, 0.0f, 0.0f, 0.0f );
	}//if

	const CX2Unit::UnitData* pUnitData = &pUnit->GetUnitData();
	//if( pUnitData != NULL )
	{
		if( g_pData->GetMyUser()->GetUID() == pUnit->GetOwnerUserUID() )
		{
			g_pData->GetGameUDP()->SetMyUID( pUnitData->m_UnitUID );
			g_pData->GetGameUDP()->AddPeer( pUnitData->m_UnitUID, 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                g_pData->GetGameUDP()->GetMyIPAddress(), 
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                g_pData->GetGameUDP()->GetMyIP(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                g_pData->GetGameUDP()->GetMyExtPort() );
		}
		else
		{
			g_pData->GetGameUDP()->AddPeer( pUnitData->m_UnitUID, 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                CKTDNUDP::ConvertIPToAddress( pUnitData->m_IP.c_str() ),
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                pUnitData->m_IP.c_str(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                pUnitData->m_Port );
		}
	}
	g_pData->GetGameUDP()->ConnectTestToPeer();

	return pCX2SquareUnit.get();
}

void CX2WeddingGame::RemoveUnit( UidType unitUID )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetUID() == unitUID )
		{
			m_UserUnitList.erase( m_UserUnitList.begin() + i );
			g_pData->GetGameUDP()->RemovePeer( unitUID );
			break;
		}
	}
}

HRESULT	CX2WeddingGame::OnFrameMove( double fTime, float fElapsedTime )
{
	if( NULL != m_pWeddingManager )
		m_pWeddingManager->OnFrameMove( fTime, fElapsedTime );

	if( NULL != m_pWorld )
		m_pWorld->OnFrameMove( fTime, fElapsedTime );

	if( NULL != m_pTalkBoxManager )
		m_pTalkBoxManager->OnFrameMove( fTime, fElapsedTime );
	
	KeyProcess();

	P2PPacketHandler();

	OnFrameMove_SyncPacket();
	OnFrameMove_UnitFrameMove( fTime, fElapsedTime );
	OnFrameMove_Camera( fTime, fElapsedTime );


	// 하트비트용 싱크패킷
	m_fP2PSendGap -= fElapsedTime;
	if( m_fP2PSendGap <= 0.0f )
	{
		m_fP2PSendGap = 5.0f;
		if( m_pMyUnit != NULL && m_bLeaveWeddingGame == false )
		{
			m_pMyUnit->SendPacket( 100 );
		}
	}

	return S_OK;
}
void CX2WeddingGame::OnFrameMove_UnitFrameMove( double fTime, float fElapsedTime )
{
	m_fReserveTime -= fElapsedTime;
	if( m_fReserveTime < 0.0f )
	{
		m_fReserveTime = 0.0f;
		for( int i = 0; i < static_cast<int>(m_UserUnitList.size()); i++ )
		{
			CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];	
			if( pCX2SquareUnit->GetInit() == false )
			{	
				pCX2SquareUnit->Init();
				m_fReserveTime = 0.5f;
				break;
			}
		}
	}

	for( int i = 0; i < static_cast<int>(m_UserUnitList.size()); ++i )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( null != pCX2SquareUnit && true == pCX2SquareUnit->GetInit() )
		{
			pCX2SquareUnit->SetShowObject( true );

			// SD 모드
				pCX2SquareUnit->SetPlanRender( g_pMain->GetGameOption().GetFieldSD() );

			pCX2SquareUnit->OnFrameMove( fTime, fElapsedTime );
		}
	}
}
void CX2WeddingGame::OnFrameMove_SyncPacket()
{
	// user sync packet을 전송할 User uid 리스트 갱신
	m_vecUserUIDforSyncPacket.resize(0);
	for( int i = 0; i < GetSquareUnitNum(); ++i )
	{
		CX2SquareUnit* pCX2SquareUnit = GetSquareUnit(i);	
		if( NULL != pCX2SquareUnit )
		{
			m_vecUserUIDforSyncPacket.push_back( pCX2SquareUnit->GetUID() );
		}			
	}

	if( m_pMyUnit != NULL && m_bLeaveWeddingGame == false )
		m_pMyUnit->SendPacketP2PForWedding( m_vecUserUIDforSyncPacket );
}
void CX2WeddingGame::OnFrameMove_Camera( double fTime, float fElapsedTime )
{
	if( m_bEnableCameraProcess == true )
	{
		if( m_fOfficiantCameraTime > 0.f )
		{
			m_pCamera->OnFrameMove( fTime, fElapsedTime );
			m_fOfficiantCameraTime -= fElapsedTime;
		}
		else if( false == m_bFreeCamera )
		{
			if( NULL != m_pMyUnit &&
				NULL != m_pWorld &&
				NULL != m_pWorld->GetLineMap() )
			{
				const CKTDGLineMap::CameraData* pCameraData = m_pWorld->GetLineMap()->GetCameraData( m_pMyUnit->GetLastTouchLineIndex() );
				if( NULL != pCameraData )
				{
					switch( pCameraData->m_eType )
					{
					case CKTDGLineMap::LCT_RELATIE_POSITION:
						{
							if ( m_pCamera != NULL )
							{
								m_pCamera->GetLineScriptedCameraData().m_vFinalRelativeEyePosition = pCameraData->m_vRelativeEye;
								m_pCamera->GetLineScriptedCameraData().m_fSpeed = pCameraData->m_fCameraRepositionSpeed;
								m_pCamera->NomalTrackingCamera( GetMyUnit(), g_pMain->GetGameOption().GetCameraDistance(), 200, 0.f, 0.f , 0.f, 0.3f );					
							}
						}
						break;
					default:					
						if ( m_pCamera != NULL )
							m_pCamera->SquareLineCamera( m_pMyUnit.get(), pCameraData->m_vEye, pCameraData->m_vLookAt, pCameraData->m_bFocusUnit, 0.4f );			
						break;
					}
				}
				else
				{
					if ( m_pCamera != NULL )
						m_pCamera->FieldCamera( m_pMyUnit.get(), 1500.f, 300.f, 230.f, 30.f);
				}
			}
			m_pCamera->OnFrameMove( fTime, fElapsedTime );
		}
		else
		{
			m_FPSCamera.SetEnablePositionMovement( true );

			m_FPSCamera.FrameMove( fElapsedTime * 300.f );			
			m_pCamera->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
			D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * 500.f + *m_FPSCamera.GetEyePt();
			m_pCamera->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
			m_pCamera->GetCamera().UpdateCamera( fElapsedTime );
		}
	}
}
HRESULT	CX2WeddingGame::OnFrameRender()
{
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
	g_pKTDXApp->GetDGManager()->ObjectChainSort();
	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
    g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
	
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			pCX2SquareUnit->RenderName();
		}
	}
	if( NULL != m_pTalkBoxManager )
	{
		m_pTalkBoxManager->OnFrameRender();
	}

	if( NULL != m_pWeddingManager )
		m_pWeddingManager->OnFrameRender();

	return S_OK;
}

bool CX2WeddingGame::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( true == m_bFreeCamera )
	{
		m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);		
	}

	return false;
}

HRESULT	CX2WeddingGame::OnResetDevice()
{
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if ( pCX2SquareUnit != NULL )
			pCX2SquareUnit->OnResetDevice();
	}

	if( m_pWorld != NULL )
		m_pWorld->OnResetDevice();

	return S_OK;
}

HRESULT	CX2WeddingGame::OnLostDevice()
{
	for( UINT i = 0; i < m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if ( pCX2SquareUnit != NULL )
			pCX2SquareUnit->OnLostDevice();
	}

	if( m_pWorld != NULL )
		m_pWorld->OnLostDevice();

	return S_OK;
}

void CX2WeddingGame::P2PPacketHandler()
{
	KTDXPROFILE();

	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
	while( pRecvData != NULL )
	{
		switch( pRecvData->m_ID )
		{
		case XPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL:
			{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				if ( pRecvData->m_Size != sizeof(KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL) )
					return;
				KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL* pkPacket = (KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL*) pRecvData->m_pRecvBuffer;
				if( !pkPacket )
					return;

				CX2SquareUnit* pUserUnit = GetSquareUnitByUID( pkPacket->m_iUnitUID );
				if( NULL != pUserUnit )
					pUserUnit->RecvPacketP2PForWedding( *pkPacket );
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				KSerBuffer ksBuff;
//				ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//				KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL kPacket;
//				DeSerialize( &ksBuff, &kPacket );
//				CX2SquareUnit* pUserUnit = GetSquareUnitByUID( kPacket.m_iUnitUID );
//				if( NULL != pUserUnit )
//					pUserUnit->RecvPacketP2PForWedding( kPacket );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			}
			break;
		}
		SAFE_DELETE( pRecvData );
		pRecvData = g_pData->GetGameUDP()->PopRecvData();
	}
}


void CX2WeddingGame::KeyProcess()
{
	if( false == GetEnableKey() || true == m_bFreeCamera)
		return;

	KTDXPROFILE();

	m_InputData.Init();

	if ( GET_DOUBLEKEYPURE_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.pureDoubleLeft = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.oneDoubleLeft = true;
	}
	if ( GET_PUREKEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.pureLeft = true;
	}
	if ( GET_KEY_STATE( GA_LEFT ) == TRUE )
	{
		m_InputData.oneLeft = true;
	}

	if ( GET_DOUBLEKEYPURE_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.pureDoubleRight = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.oneDoubleRight = true;
	}
	if ( GET_PUREKEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.pureRight = true;
	}
	if ( GET_KEY_STATE( GA_RIGHT ) == TRUE )
	{
		m_InputData.oneRight = true;
	}

	if ( GET_DOUBLEKEYPURE_STATE( GA_UP ) == TRUE )
	{
		m_InputData.pureDoubleUp = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_UP ) == TRUE )
	{
		m_InputData.oneDoubleUp = true;
	}
	if ( GET_PUREKEY_STATE( GA_UP ) == TRUE )
	{
		m_InputData.pureUp = true;
	}
	if ( GET_KEY_STATE( GA_UP ) == TRUE )
	{
		m_InputData.oneUp = true;
	}

	if ( GET_DOUBLEKEYPURE_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.pureDoubleDown = true;
	}
	if ( GET_DOUBLEKEY_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.oneDoubleDown = true;
	}
	if ( GET_PUREKEY_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.pureDown = true;
	}
	if ( GET_KEY_STATE( GA_DOWN ) == TRUE )
	{
		m_InputData.oneDown = true;
	}
	if ( NULL != g_pChatBox && false == g_pChatBox->GetFocusChatEditBox() && GET_KEY_STATE( GA_SIT ) == TRUE &&
		NULL != m_pMyUnit)
	{
		if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
			return;

		if ( m_pMyUnit->GetNowState() != m_pMyUnit->GetStateID().m_Wait )
			return;

		if( m_pMyUnit->GetNowEmotionId() == CX2Unit::ET_SITREADY )
			return;

		g_pChatBox->SendEmotionId( GET_STRING( STR_ID_2501 ) );
	}

#ifdef _IN_HOUSE_
	// 사내 테스트용 위치 변경 치트
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_1) == TRUE )
	{
		if( NULL != GetMyUnit() )
		{
			D3DXVECTOR3 vPos;
			GetStartPosByWeddingStatus( SEnum::WS_GROOM, vPos );
			GetMyUnit()->SetPos(vPos);
		}
	}
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_2) == TRUE )
	{
		if( NULL != GetMyUnit() )
		{
			D3DXVECTOR3 vPos;
			GetStartPosByWeddingStatus( SEnum::WS_BRIDE, vPos );
			GetMyUnit()->SetPos(vPos);
		}
	}
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_3) == TRUE )
	{
		if( NULL != GetMyUnit() )
		{
			D3DXVECTOR3 vPos;
			GetStartPosByWeddingStatus( SEnum::WS_GUEST, vPos );
			GetMyUnit()->SetPos(vPos);
		}
	}
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_4) == TRUE )
	{
		StartWedding();
	}
#endif //_IN_HOUSE_

	m_pMyUnit->SetInputData( &m_InputData );
}

void CX2WeddingGame::Handler_EGS_JOIN_WEDDING_HALL_NOT( KEGS_JOIN_WEDDING_HALL_NOT& kEGS_JOIN_WEDDING_HALL_NOT )
{
	/// NOT는 내 유닛이 아닌 경우에만 처리 한다.
	if( kEGS_JOIN_WEDDING_HALL_NOT.m_JoinSlot.m_kRoomUserInfo.m_nUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	{
		JoinSquareUnit( &kEGS_JOIN_WEDDING_HALL_NOT.m_JoinSlot.m_kRoomUserInfo );

		switch( static_cast<SEnum::WEDDING_STATUS>(kEGS_JOIN_WEDDING_HALL_NOT.m_JoinSlot.m_kRoomUserInfo.m_cWeddingStatus) )
		{
		case SEnum::WS_GROOM:
		case SEnum::WS_BRIDE:
			{// 신랑이나 신부가 들어오면 Create
				++m_iWeddingHeroineNum;
			} break;
		default:
			break;
		}
	}
}

void CX2WeddingGame::Handler_EGS_LEAVE_WEDDING_HALL_NOT( KEGS_LEAVE_WEDDING_HALL_NOT& kEGS_LEAVE_WEDDING_HALL_NOT )
{
	if( true == IsWeddingHeroineByUID( kEGS_LEAVE_WEDDING_HALL_NOT.m_iUnitUID ) )
		--m_iWeddingHeroineNum;

	RemoveUnit(	kEGS_LEAVE_WEDDING_HALL_NOT.m_iUnitUID );
}

void CX2WeddingGame::GetUnitPageInfo(int &iMaxPage, int iUnitNumPerPage)
{
	int nNumUnit = (int)m_UserUnitList.size();
	iMaxPage = (nNumUnit + iUnitNumPerPage - 1) / iUnitNumPerPage; 
}

vector<CX2SquareUnit*> CX2WeddingGame::GetUnitList( int& iPage, int iUnitNumPerPage /*= 10 */ )
{
	if( iUnitNumPerPage <= 0 )
		iUnitNumPerPage = 1;

	vector<CX2SquareUnit*> vecUnitList;

	int nNumUnit = (int)m_UserUnitList.size();
	int iMaxPage = (nNumUnit + iUnitNumPerPage - 1) / iUnitNumPerPage; 
	if( iMaxPage < 1 )
		return vecUnitList;

	if( iPage > iMaxPage )
		iPage = iMaxPage;

	if( iPage < 1 )
		iPage = 1;

	int iStartIndex = (iPage-1) * iUnitNumPerPage; 
	int iEndIndex;

	if( iStartIndex < (int)m_UserUnitList.size() )
	{
		iEndIndex = min( (int)m_UserUnitList.size(), iStartIndex + iUnitNumPerPage );
		for( int i=iStartIndex; i<iEndIndex; i++ )
		{
			CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
			vecUnitList.push_back( pCX2SquareUnit.get() );
		}
	}

	return vecUnitList;
}

void CX2WeddingGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
	const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{

	CX2SquareUnit* pCX2SquareUnit = GetSquareUnitByUID( iUnitUID_ );
	if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetInit() == true && pCX2SquareUnit->GetShowObject() == true &&
		pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_NONE )
	{
#ifdef UNIT_EMOTION
		bool bCommandEmotion = g_pChatBox->IsEmotionID(pWstrMsg_);
		if( g_pChatBox != NULL && bCommandEmotion == false )
#endif
		{
            if( pCX2SquareUnit->GetBoundingRadius() > 0 )
            {
				D3DXVECTOR3 center;
				pCX2SquareUnit->GetTransformCenter( &center );
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
                float   fScaledBoundingRadius =pCX2SquareUnit->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
				//컬링
				float fScale;
				if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetYScale() )
				{
					if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
					{
						//X가 제일 큼
						fScale = pCX2SquareUnit->GetMatrix().GetXScale();
					}
					else
					{
						//Z가 제일 큼
						fScale = pCX2SquareUnit->GetMatrix().GetZScale();
					}
				}
				else
				{
					if( pCX2SquareUnit->GetMatrix().GetYScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
					{
						//Y가 제일 큼
						fScale = pCX2SquareUnit->GetMatrix().GetYScale();
					}
					else
					{
						//Z가 제일 큼
						fScale = pCX2SquareUnit->GetMatrix().GetZScale();
					}
				}
                float   fScaledBoundingRadius = pCX2SquareUnit->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

				if( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, fScaledBoundingRadius ) == false )
					return;
            }


			CX2TalkBoxManagerImp::TalkBox talkBox;
			talkBox.m_OwnerUnitUID		= iUnitUID_;
			talkBox.m_wstrTalkContent	= pWstrMsg_;
			talkBox.m_bTraceUnit		= true;
			talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
			talkBox.m_coTextColor		= coTextColor_;
			talkBox.m_BackTexColor		= coBackColor_;

			talkBox.m_bUseOutLine		 = true;
			if( coOutColor_ == 0x00000000 )	//	입력 없을 경우는 글씨색 그대로 쓰기
			{
				talkBox.m_bUseOutLine = false;
			}

			m_pTalkBoxManager->Push( talkBox );
		}

#ifdef UNIT_EMOTION
		// 이모션 연결시켜준다.
		if(g_pChatBox != NULL)
		{
			// 앉은 상태에서는 채팅필터링 먹히지 않도록 한다.(명령어로만 이모션동작취함)
			if( bCommandEmotion == false && pCX2SquareUnit->GetNowEmotionId() == CX2Unit::ET_SITWAIT )
				return;

			pCX2SquareUnit->PlayEmotion( g_pChatBox->GetEmotionID(pWstrMsg_) );			
		}
#endif
	}
}

void CX2WeddingGame::PopOfficiantTalkBox( int iStrID_, const D3DXVECTOR3& vPos_, const D3DXCOLOR& color_ /*= 0xff000000*/, const D3DXCOLOR& outcolor_ /*= (DWORD)0x00000000*/, const D3DXCOLOR& backColor_ /*= (DWORD)0xffffffff*/ )
{
	CX2TalkBoxManagerImp::TalkBox talkBox;
	talkBox.m_OwnerUnitUID		= -1;
	talkBox.m_wstrTalkContent	= GET_STRING( iStrID_ );
	talkBox.m_bTraceUnit		= true;
	talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
	talkBox.m_coTextColor		= color_;
	talkBox.m_BackTexColor		= backColor_;
	talkBox.m_bUseOutLine		= true;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	talkBox.m_vTalkBoxPosition  = vPos_;
#endif ADDED_RELATIONSHIP_SYSTEM

	if( outcolor_ == 0x00000000 )	//	입력 없을 경우는 글씨색 그대로 쓰기
	{
		talkBox.m_bUseOutLine = false;
	}

	m_pTalkBoxManager->Push( talkBox );
}

CX2SquareUnit* CX2WeddingGame::GetSquareUnit( int index )
{
	if( index >= (int)m_UserUnitList.size() )
		return NULL;
	return m_UserUnitList[index].get();
}


CX2SquareUnit* CX2WeddingGame::GetSquareUnitByUID( UidType UID )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			if( pCX2SquareUnit->GetUID() == UID )
			{
				return pCX2SquareUnit.get();
			}
		}
	}

	return NULL;
}

CX2SquareUnit* CX2WeddingGame::GetSquareUnitByNickName( const WCHAR* wszNickName )
{
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			if( 0 == pCX2SquareUnit->GetNickName().compare( wszNickName ) )
			{
				return pCX2SquareUnit.get();
			}
		}
	}

	return NULL;
}

void CX2WeddingGame::SetShowUserUnit( bool bShow )
{
	for( UINT i=0; i<m_UserUnitList.size(); i++ )
	{
		CX2SquareUnitPtr pCX2SquareUnit = m_UserUnitList[i];
		if( pCX2SquareUnit != NULL )
		{
			pCX2SquareUnit->SetShowObject( bShow );
		}
	}
}

void CX2WeddingGame::SetFreeCamera( bool bFreeCamera )
{
	m_bFreeCamera = bFreeCamera;

	if( true == m_bFreeCamera )
	{
		D3DXVECTOR3 vEye	= m_pCamera->GetCamera().GetEye();
		D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();

		m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	}
}

bool CX2WeddingGame::GetEnableKey()
{
	return m_bEnableKeyProcess;
}

CKTDGParticleSystem* CX2WeddingGame::GetMajorParticle()
{ 
	return (NULL != g_pData ) ? g_pData->GetGameMajorParticle() : NULL; 
}
CKTDGParticleSystem* CX2WeddingGame::GetMinorParticle()			
{ 
	return (NULL != g_pData ) ? g_pData->GetGameMinorParticle() : NULL; 
}
CKTDGXMeshPlayer* CX2WeddingGame::GetMajorXMeshPlayer()		
{ 
	return (NULL != g_pData ) ? g_pData->GetGameMajorXMeshPlayer() : NULL; 
}
CKTDGXMeshPlayer* CX2WeddingGame::GetMinorXMeshPlayer()		
{ 
	return (NULL != g_pData ) ? g_pData->GetGameMinorXMeshPlayer() : NULL; 
}
void CX2WeddingGame::StartWedding()
{
	if( NULL != m_pWeddingManager )
		m_pWeddingManager->StartWedding();
}
void CX2WeddingGame::GetStartPosByWeddingStatus( IN SEnum::WEDDING_STATUS eWeddingStatus_,  OUT D3DXVECTOR3& vPos_ ) const
{
	if( NULL != m_pWeddingManager )
	{
		UINT uiStartPosIndex = m_pWeddingManager->GetStartPosIndexByWeddingStatus( eWeddingStatus_ ) ;
		if( NULL != m_pWorld && NULL != m_pWorld->GetLineMap() )
		{
			vPos_ = m_pWorld->GetLineMap()->GetStartPosition( uiStartPosIndex );
		}
	}
}
void CX2WeddingGame::PlayEffectSet_WeddingGame( const WCHAR* pEffectSetName_, const D3DXVECTOR3& vPos_ ,CX2WeddingBehavior::WEDDING_EFFECTSET_POS_TYPE ePosType_/* = CX2WeddingBehavior::WEPT_CUSTOM */)
{
	if( NULL == g_pData->GetUIEffectSet() )
		return;

	switch ( ePosType_ )
	{
	case CX2WeddingBehavior::WEPT_CUSTOM:
		{
			g_pData->GetUIEffectSet()->PlayEffectSetCustomPos( pEffectSetName_, m_pMyUnit, vPos_ );
		} break;
	case CX2WeddingBehavior::WEPT_BRIDE:
		{
			BOOST_FOREACH ( CX2SquareUnitPtr pUnitPtr, m_UserUnitList )
			{
				if( null != pUnitPtr &&
					SEnum::WS_BRIDE == pUnitPtr->GetWeddingStatus() )
				{
					g_pData->GetUIEffectSet()->PlayEffectSet( pEffectSetName_, pUnitPtr );
					break;
				}
			}
		} break;
	case CX2WeddingBehavior::WEPT_GROOM:
		{
			BOOST_FOREACH ( CX2SquareUnitPtr pUnitPtr, m_UserUnitList )
			{
				if( null != pUnitPtr &&
					SEnum::WS_GROOM == pUnitPtr->GetWeddingStatus() )
				{
					g_pData->GetUIEffectSet()->PlayEffectSet( pEffectSetName_, pUnitPtr );
					break;
				}
			}
		} break;
	default:
		break;
	}
}
bool CX2WeddingGame::IsWeddingHeroineMyUnit() const
{
	if( NULL != m_pMyUnit )
	{
		switch( static_cast<SEnum::WEDDING_STATUS>(m_pMyUnit->GetWeddingStatus()) )
		{
		case SEnum::WS_GROOM:
		case SEnum::WS_BRIDE:
			{
				return true;
			} break;
		default:
			break;
		}
	}

	return false;
}
bool CX2WeddingGame::IsWeddingHeroineByUID( UidType uiUserUID ) const
{
	BOOST_FOREACH( CX2SquareUnitPtr pSquareUnit, m_UserUnitList )
	{
		if( uiUserUID == pSquareUnit->GetUID() )
		{
			switch( static_cast<SEnum::WEDDING_STATUS>(pSquareUnit->GetWeddingStatus()) )
			{
			case SEnum::WS_GROOM:
			case SEnum::WS_BRIDE:
				{
					return true;
				} break;
			default:
				break;
			}
		}
	}

	return false;
}
/** @function : GetIsCanWeddingStart
	@brief : 결혼식 시작 할 수 있는 상태인지 체크
*/
bool CX2WeddingGame::GetIsCanWeddingStart() const
{
	// 결혼식 주인공 접속 체크
	if( m_iNessesaryHeroineNum != GetWeddingHeroineNum() )
		return false;

	return true;
}
#endif //ADDED_RELATIONSHIP_SYSTEM

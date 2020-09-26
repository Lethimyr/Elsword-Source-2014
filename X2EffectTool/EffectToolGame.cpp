#include "dxstdafx.h"
#include "EffectToolGame.h"



///////////////////////////////////////////////////////////////////////////////////////////////
CEffectToolGame::CEffectToolGame(CX2EffectToolMain* pEffectToolMain_):
m_pEffectToolMain(pEffectToolMain_)
{
	m_GameType			= CX2Game::GT_PVP;

	m_pPVPRoom			= NULL;
	m_bSquareCamera		= false;
    m_bOrthoFreeCamera  = false;
    m_fOrthoScale       = 2.0f;
	m_bFixedCamera		= false;
	//m_WinTeam			= CX2Room::TN_RED;

	//m_bTimeStop			= false;
	//m_fRemainPlayTime	= 0.0f;
	//m_pFontForTime		= g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUKFont( L"돋움체", 50, 1, true );
}

CEffectToolGame::~CEffectToolGame(void)
{
}

HRESULT CEffectToolGame::OnFrameMove( double fTime, float fElapsedTime )
{
	SetAllowFrameCount( (int)(85 * 5.3) );

	m_fTime			= (float)fTime;
	m_fElapsedTime	= fElapsedTime;


	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RCONTROL) == TRUE && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( m_bGameWait == true )
			m_bGameWait = false;
		else
			m_bGameWait = true;
	}

	if( m_bGameWait == true )
		return S_OK;

	P2PPacketHandler();	

	if( m_GameState == GS_PLAY )
	{		
		m_fGameTime += m_fElapsedTime;
		m_AllowFrameCount--;
		if( m_AllowFrameCount < 0 )
		{
			m_AllowFrameCount = 0;
		}
		UpdateUnitPointer();

		if( false == m_bFreeCamera )
		{
			KeyProcess();
		}

//		//{{ 2009.02.25 김태완 : 유닛 안 움직이는 현상 수정
//		// user, npc sync packet을 전송할 User uid 리스트 갱신

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		m_vecUserUIDforSyncPacket.resize(0);
//		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
//		{
//			CX2GUUser* pCX2GUUser = m_UserUnitList[i];
//			if( pCX2GUUser != NULL )
//			{
//				m_vecUserUIDforSyncPacket.push_back( pCX2GUUser->GetUnitUID() );
//			}
//		}
//#endif // SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//		//}}

		if( m_bStopUnitChain == false )
		{
			for( UINT i = 0; i < m_UnitList.size(); i++ )
			{
				CX2GameUnit* pCX2GameUnit = m_UnitList[i];

				if ( pCX2GameUnit != NULL && 
					CX2EffectToolMain::MODE_GAMEPLAY == m_pEffectToolMain->GetEffectToolMode()	)
				{
					pCX2GameUnit->OnFrameMove( fTime, fElapsedTime );
				}

				// 내 유닛 HP는 늘 100%되게 치트
				if( true == pCX2GameUnit->IsMyUnit() )
				{
					pCX2GameUnit->SetNowHp( pCX2GameUnit->GetMaxHp() );
				}
			}
		}
/*
        if(g_pKTDXApp->GetDGManager()->GetPerspective() == false)
        {
            const D3DXVECTOR3 *vEye;
            vEye = m_FPSCamera.GetEyePt();
            D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * g_pKTDXApp->GetDGManager()->GetFar()*0.1f + *m_FPSCamera.GetEyePt();
            //vLookAt = m_FPSCamera.GetLookAtPt();

            m_pCamera->GetCamera().Point(vEye->x, vEye->y, vEye->z, vLookAt.x, vLookAt.y, vLookAt.z);
            //m_pWorld->SetEye(*vEye);
            //m_pWorld->SetLookAt(vLookAt);
        }
*/

		m_pWorld->OnFrameMove( fTime, fElapsedTime );

		//m_pMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
		//m_pMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

		m_pDamageEffect->OnFrameMove( fTime, fElapsedTime );

		//m_pMajorParticle->OnFrameMove( fTime, fElapsedTime );
		//m_pMinorParticle->OnFrameMove( fTime, fElapsedTime );

		//GetMajorXMeshPlayer()->OnFrameMove( fTime, fElapsedTime );
		//GetMinorXMeshPlayer()->OnFrameMove( fTime, fElapsedTime );
		//GetMajorParticle()->OnFrameMove( fTime, fElapsedTime );
		//GetMinorParticle()->OnFrameMove( fTime, fElapsedTime );
		
		m_UnitLoader.OnFrameMove( fElapsedTime );

		if(false == m_bFixedCamera)
		{
			if ( GetFocusUnit() != NULL )
			//if( m_pFocusUnit != NULL )
			{
				if( true == m_bSquareCamera )
				{
					//SquareCamera( (CX2GUUser*)m_pFocusUnit, m_SquareCameraParam.m_fEyeDistance, m_SquareCameraParam.m_fEyeHeight, m_SquareCameraParam.m_fLookAtHeight );
					SquareCamera( (CX2GUUser*)GetFocusUnit(), m_SquareCameraParam.m_fEyeDistance, m_SquareCameraParam.m_fEyeHeight, m_SquareCameraParam.m_fLookAtHeight );
				}
				else
				{                
					if(g_pKTDXApp->GetDGManager()->GetPerspective() == false)
						g_pMain->GetGameOption().GetOptionList().m_iZoomLevel = 1;
					else
						g_pMain->GetGameOption().GetOptionList().m_iZoomLevel = -1;

					//{{ 09.02.26 김태완 :: 다이나믹 카메라 On/OFF 기능
					// 원래 코드
					//m_pCamera->NomalTrackingCamera( m_pFocusUnit, m_SquareCameraParam.m_fEyeDistance );
					// m_pFocusUnit->OnCameraMove(); 원하는 동작을 하므로 이런 식으로 구현.
					GetFocusUnit()->OnCameraMove();				
					//}}

				}
			}                
			m_pCamera->OnFrameMove( fTime, fElapsedTime );

		}
		

		//if( IsHost() == true )
		//{
		//	m_NPCPacketSendIntervalNow += m_fElapsedTime;
		//	if( m_NPCPacketSendIntervalNow >= m_NPCPacketSendInterval )
		//	{
		//		KXPT_UNIT_NPC_SYNC_PACK kXPT_UNIT_NPC_SYNC_PACK;
		//		for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
		//		{
		//			CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		//			if ( pCX2GUNPC != NULL )
		//			{
		//				pCX2GUNPC->SendPacket( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
		//				if( sizeof(KXPT_UNIT_NPC_SYNC) * kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() >  MTU_SIZE )
		//				{
		//					KSerBuffer buff;
		//					Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

		//					std::vector<UidType> toUIDList;
		//					for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		//					{
		//						CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		//						if( pCX2GUUser != NULL )
		//						{
		//							toUIDList.push_back( pCX2GUUser->GetUnitUID() );
		//						}
		//					}
		//					g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
		//				}
		//			}
		//		}

		//		if( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > 0 )
		//		{
		//			KSerBuffer buff;
		//			Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

		//			std::vector<UidType> toUIDList;
		//			for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		//			{
		//				CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		//				if( pCX2GUUser != NULL )
		//				{
		//					toUIDList.push_back( pCX2GUUser->GetUnitUID() );
		//				}
		//			}
		//			g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
		//		}			

		//		m_NPCPacketSendIntervalNow = 0.0f;
		//	}

		//	//즉시처리
		//	KXPT_UNIT_NPC_SYNC_PACK kXPT_UNIT_NPC_SYNC_PACK;
		//	for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
		//	{
		//		CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
		//		if ( pCX2GUNPC != NULL )
		//		{
		//			pCX2GUNPC->SendPacketImmediate( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
		//			if( sizeof(KXPT_UNIT_NPC_SYNC) * kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() >  MTU_SIZE )
		//			{
		//				KSerBuffer buff;
		//				Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

		//				std::vector<UidType> toUIDList;
		//				for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		//				{
		//					CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		//					if( pCX2GUUser != NULL )
		//					{
		//						toUIDList.push_back( pCX2GUUser->GetUnitUID() );
		//					}
		//				}
		//				g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
		//			}
		//		}
		//	}

		//	if( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > 0 )
		//	{
		//		KSerBuffer buff;
		//		Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

		//		std::vector<UidType> toUIDList;
		//		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		//		{
		//			CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		//			if( pCX2GUUser != NULL )
		//			{
		//				toUIDList.push_back( pCX2GUUser->GetUnitUID() );
		//			}
		//		}
		//		g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
		//	}
		//}

	}

	return S_OK;
}

HRESULT CEffectToolGame::OnFrameRender()
{
	if( m_GameState == GS_PLAY )
	{
		m_pWorld->OnFrameRender();
		g_pKTDXApp->GetDGManager()->ObjectChainSort();
		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
		g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();

#ifndef WORLD_TOOL	// 090519 태완 임시수정코드
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Flush();
		g_pKTDXApp->GetDGManager()->FrontUIRender();		
#endif

	}

	return S_OK;
}

void CEffectToolGame::GameLoading( CX2Room* pRoom )
{
	m_pPVPRoom			= (CX2PVPRoom*)pRoom;

	m_GameState			= GS_LOADING;
	CX2Game::GameLoading( pRoom );
	m_GameState			= GS_LOADING;

	m_GameState			= GS_PLAY;
}

void CEffectToolGame::SystemLoading()
{
	m_GameState = GS_LOADING;

	srand((unsigned int)time(NULL));

	g_pData->ResetGameMajorParticle( L"TempMajorParticle.txt" );
	g_pData->ResetGameMinorParticle( L"TempMinorParticle.txt" );
	//게임 파티클 시스템 생성
//	g_pData->ResetGameMajorParticle( L"GameMajorParticle.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle2.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle3.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle4.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle5.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle6.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle7.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle8.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle9.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle10.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle11.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle12.txt" );
// 	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle13.txt" );

//	g_pData->ResetGameMinorParticle( L"GameMinorParticle.txt" );

	//m_pMajorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
	//m_pMinorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
	//m_pMajorXMeshPlayer	= new CKTDGXMeshPlayer();
	//m_pMinorXMeshPlayer	= new CKTDGXMeshPlayer();


	//m_pMajorParticle->OpenScriptFile( L"GameMajorParticle.txt" );
	//m_pMinorParticle->OpenScriptFile( L"GameMinorParticle.txt" );
	//m_pMajorXMeshPlayer->OpenScriptFile( L"GameMajorXMeshPlayer.txt" );
	//m_pMinorXMeshPlayer->OpenScriptFile( L"GameMinorXMeshPlayer.txt" );

	ResetDamageEffect();
}

void CEffectToolGame::WorldLoading()
{
    m_pWorld = g_pData->GetWorldManager()->CreateWorld( (CX2World::WORLD_ID)m_pPVPRoom->GetWorldID() );
}




void CEffectToolGame::EtcLoading()
{
	m_pCamera		= new CX2Camera();
	if( m_pWorld != NULL )
		m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );
	m_pCamera->NomalDirectCamera( GetFocusUnit() );

	m_pDropItemManager = NULL;


	D3DXVECTOR3 vEye = m_pCamera->GetCamera().GetEye();
	D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();
	m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	m_FPSCamera.SetProjParams( D3DX_PI/4, 1.f, g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar() );
	m_FPSCamera.SetRotateButtons( false, false, true, false );

}


void CEffectToolGame::ResetUnitPosition()
{
	GetMyUnit()->ClearRecieveSyncData();
	GetMyUnit()->InitPosition( true, 0 );

}

void CEffectToolGame::ReleaseCurrentWorld()
{
	if( NULL == m_pWorld )
		return;

	vector<CX2WorldObjectPtr> vecpWorldObjectList = m_pWorld->GetWorldObjectList();
	for( unsigned i=0; i<vecpWorldObjectList.size(); i++ )
	{
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( vecpWorldObjectList[i].get() );
	}

	vector<CKTDGSkyDome*>& vecpSkyDomeList = m_pWorld->GetSkyDomeList();
	for( unsigned i=0; i<vecpSkyDomeList.size(); i++ )
	{
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( vecpSkyDomeList[i] );
	}

	SAFE_DELETE( m_pWorld );
}

void CEffectToolGame::ReloadWorld( const WCHAR* pwszFileName )
{
	ReleaseCurrentWorld();

	CX2World::WorldData worldData;
	worldData.dataFileName = pwszFileName;

	m_pWorld = new CX2World( &worldData );
	m_pWorld->OpenScriptFile( pwszFileName );
}


void CEffectToolGame::SquareCamera( CX2GUUser* pFocusUnit, float distance, float eyeHeight /*= 200.f*/, float lookAtHeight /*= 100.f*/, float angleDegree, 
								  float eyeDistance, float lookatDistance, float trackingTime )
{
	if( NULL == m_pCamera )
		return;

	//m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();
	lookAt.y += lookAtHeight;

	if( pFocusUnit->GetIsRight() == true )
	{
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}


	if( eye.y < m_pCamera->GetLandHeight() + 200.0f )
		eye.y = m_pCamera->GetLandHeight() + 200.0f;
	if( lookAt.y < m_pCamera->GetLandHeight() + 200.0f )
		lookAt.y = m_pCamera->GetLandHeight() + 200.0f;
	m_pCamera->GetCamera().SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}


void CEffectToolGame::SetCamera()
{
    if( m_bSquareCamera == false )
	{
        m_pCamera->NomalDirectCamera( (CX2GUUser*)GetFocusUnit(), m_SquareCameraParam.m_fEyeDistance );
	}
	else 
	{
		SquareCamera( (CX2GUUser*)GetFocusUnit(), m_SquareCameraParam.m_fEyeDistance, m_SquareCameraParam.m_fEyeHeight, m_SquareCameraParam.m_fLookAtHeight );
	}
}
/*FORCEINLINE*/ CKTDGParticleSystem* CEffectToolGame::GetMajorParticle()
{
	if( NULL == m_pEffectToolMain )
		return NULL;

	return m_pEffectToolMain->m_pParticleSystem;
}

/*FORCEINLINE*/ CKTDGParticleSystem* CEffectToolGame::GetMinorParticle()
{
	if( NULL == m_pEffectToolMain )
		return NULL;

	return m_pEffectToolMain->m_pMinorParticleSystem;
}

/*FORCEINLINE*/ CKTDGXMeshPlayer* CEffectToolGame::GetMajorXMeshPlayer()
{
	if( NULL == m_pEffectToolMain )
		return NULL;

	return m_pEffectToolMain->m_pXMeshPlayer;
}

/*FORCEINLINE*/ CKTDGXMeshPlayer* CEffectToolGame::GetMinorXMeshPlayer()
{
	if( NULL == m_pEffectToolMain )
		return NULL;

	return m_pEffectToolMain->m_pMinorxMeshPlayer;
}

//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include ".\x2statesquaregame.h"

CX2StateSquareGame::CX2StateSquareGame(void) :
// 2009.01.09 김태완 : 초기화 코드정리
m_pDLGExitMsgBox(NULL),
m_pDLGFront(NULL),
m_pDLGBack(NULL),
//m_pDLGSquareTitle(NULL),
m_vOldEyePt(0.0f,0.0f,0.0f),				// D3DXVECTOR3
m_vOldLookAtPt(0.0f,0.0f,0.0f)				// D3DXVECTOR3
{

	// 초기화 ////////////////////////////////////////////////////////////////////////
// 	m_pDLGExitMsgBox	= NULL;
// 	m_pDLGFront			= NULL;
// 	m_pDLGBack			= NULL;	

	//////////////////////////////////////////////////////////////////////////
	

	g_pKTDXApp->GetDIManager()->SetEnable(true);
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(),	true );


	//{{ dmlee 2009.3.25 거래광장과 마을을 왔다갔다 할 때 크래시 나는 문제때문에 일단 comment out
	//    //{{ seojt // 2008-10-22, 17:22
	//#ifdef BACKGROUND_LOADING_TEST // 2008-10-23
	//
	//    /** urgent data의 로딩 처리:
	//        현재 카메라 시점에서 렌더링될 데이터를 urgent data로 설정하여 모두 로드한다.
	//        ObjectChainSort()를 호출하면 현재 카메라 설정에 대해서 culling 작업과 함께,
	//        마지막 접근 시간 time stamp를 g_NowTime으로 갱신한다.
	//        마지막 카메라 설정은 X2SquareGame.cpp에서 m_pCamera->SquareCameraUpdate()를
	//        호출해서 설정해 두었다.
	//        LoadUrgentData()는 resource loading thread의 요청 큐에 들어 있는 노드를 검사하여
	//        마지막 접근 시간이 fTimeStamp보다 크거나 같은 모든 노드에 대해서 로드를 실행한다.
	//        LoadUrgentData()는 urgent resource가 모두 로드될 때까지 block상태가 된다.
	//
	//        @note   아래 블록을 주석처리하면, urgent data의 로드없이 바로 광장으로 진행하므로
	//                게임 객체들이 실시간 로드되는 것을 눈으로 확인할 수 있다.
	//
	//        - jintaeks on 2008-10-24, 9:36 */
	//    {
	//        double  fTimeStamp = g_NowTime;
	//        // frustum에 들어가는 게임 객체들의 time stamp를 g_NowTime으로 설정한다.
	//        // urgent data의 로드를 위한 전처리 작업이다.
	//        g_pKTDXApp->GetDGManager()->ObjectChainSort();
	//        // last accesstime값이 fTimeStamp값 보다 크거나 같은 모든 리소스를 로드하라고 요구한다.
	//        g_pKTDXApp->GetDeviceManager()->LoadUrgentData( fTimeStamp );
	//    }//block
	//
	//#endif // BACKGROUND_LOADING_TEST // 2008-10-22
	//    //}} seojt // 2008-10-22, 17:22
	//}} dmlee 2009.3.25 거래광장과 마을을 왔다갔다 할 때 크래시 나는 문제때문에 일단 comment out

	//g_pKTDXApp->GetDGManager()->GetCamera().Point( 6705, 1164, 4121, 6701, 1013, 4907 ); 
	//g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( 1.0f );


	// 광장 제목 다이얼로그 
// 	m_pDLGSquareTitle = new CKTDGUIDialog( this, L"DLG_SquareUI_Title.lua" );
// 	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSquareTitle );

//	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGSquareTitle->GetControl( L"Static_SquareTitle" );
//	pStatic->GetString(0)->msg = g_pSquareGame->GetSquareData().m_SquareName;
	


	SetOpenCommonBG( false );

	//if ( m_pDLGMenuWindow != NULL )
	//{
	//	CKTDGUIControl* pControl = m_pDLGMenuWindow->GetControl( L"Exit" );
	//	if ( pControl != NULL )
	//	{
	//		pControl->SetEnable( false );
	//	}
	//}

	SoundReady( L"X2_Button_Mouse_Up.ogg" );

	XSkinMeshReady( L"Mesh_Arme_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Elsword_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Eve_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Lire_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Raven_Base_SD.x" );

//{{ kimhc // 2010.12.9 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	XSkinMeshReady( L"Mesh_CHUNG_Base_SD.x" );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.9 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	XSkinMeshReady( L"Mesh_Ara_Base_SD.x" );
#endif

#ifdef NEW_CHARACTER_EL
	XSkinMeshReady( L"Mesh_EL_Base_SD.x" );
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	XSkinMeshReady( L"Mesh_Add_Base_SD.x" );
#endif //SERV_9TH_NEW_CHARACTER

}//CX2StateSquareGame::CX2StateSquareGame()


CX2StateSquareGame::~CX2StateSquareGame(void)
{
	SAFE_DELETE_DIALOG( m_pDLGExitMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGFront );
	SAFE_DELETE_DIALOG( m_pDLGBack );

//	SAFE_DELETE_DIALOG( m_pDLGSquareTitle );	
	
	SAFE_DELETE( g_pSquareGame );



	g_pKTDXApp->GetDIManager()->SetEnable(true);

	//이 부분 나중에 함수로 빼자
	SetShowMenuByDefault();
}


HRESULT CX2StateSquareGame::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );


	if( 
#ifndef NEW_SKILL_TREE
		false == g_pInstanceData->GetSkillTreeUI()->GetShow() &&
#endif
        m_pCashShop != NULL &&
        m_pCashShop->GetOpen() == false )
	{
		if( g_pSquareGame != NULL )
		{
			g_pSquareGame->OnFrameMove( fTime, fElapsedTime );

#ifdef POSTBOX
            if(g_pMain->GetPostBox()->GetEnable() == false && g_pData->GetUIManager()->IsOpenedWindow() == false)
            {
#endif
			//마우스 체크
			if( g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState( MOUSE_RBUTTON ) == TRUE )
			{
				for( int i = 0; i < g_pSquareGame->GetSquareUnitNum(); i++ )
				{
					CX2SquareUnit* pCX2SquareUnit = g_pSquareGame->GetSquareUnit(i);	
					if( pCX2SquareUnit != NULL )
					{
						D3DXVECTOR3 pos = pCX2SquareUnit->GetPos();
						pos.y += 100.0f;

						D3DXVECTOR3 startRay, endRay;
						g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
													g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

						if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, pos,	pCX2SquareUnit->GetBoundingRadius()/2.0f ) == true )
						{
							if(g_pData->GetUserMenu() != NULL)
							{
								CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
								bool bIsParty = false;

								if ( NULL != pPartyManager && true == pPartyManager->DoIHaveParty() )
									bIsParty = true;

								g_pData->GetUserMenu()->OpenUserPopupMenu( pCX2SquareUnit->GetUnit()->GetUID(), bIsParty );
							}
							break;
						}
					}			
				}
			}

#ifdef POSTBOX
            }
#endif

		}
	}
	//{{ 허상형 : [2009/10/21] //	캐쉬샵이 열려 있을 경우 서버에 패킷을 보내기 위해 FrameMove 호출
	else if( m_pCashShop != NULL &&	m_pCashShop->GetOpen() == true )
	{
		if( g_pSquareGame != NULL )
		{
			g_pSquareGame->OnFrameMove( fTime, fElapsedTime );
		}
	}
	//}} 허상형 : [2009/10/21] //	캐쉬샵이 열려 있을 경우 서버에 패킷을 보내기 위해 FrameMove 호출


	//{{ 09.03.11 김태완 : 미니맵 최소화 기능 제거
// 	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_TAB) == TRUE )
// 	{
// 		if( NULL != g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap() )
// 		{
// 			g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->SetMinimized( !g_pInstanceData->GetMiniMapUI()->GetDungeonMiniMap()->GetMinimized() );
// 		}
// 		return true;
// 	}
	//}}

#ifdef SERV_KEY_MAPPING_INT
	if( GET_KEY_STATE( GAMEACTION_ATTACK_FAST ) == TRUE )
#else // SERV_KEY_MAPPING_INT
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_Z) == TRUE )
#endif // SERV_KEY_MAPPING_INT
	{
		if( g_pChatBox->GetFocus() == false && g_pSquareGame->ExitMarket() == true)
		{
			Handler_EGS_LEAVE_SQUARE_REQ();
			//Handler_EGS_STATE_CHANGE_FIELD_REQ();
		}
	}

#ifdef FRAME_MOVE_CULL

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_Y) == TRUE )
	{
		CKTDGXSkinAnim::SetTestUpdate( !CKTDGXSkinAnim::GetTestUpdate() );
	}

#endif //FRAME_MOVE_CULL


	return S_OK;
}

HRESULT CX2StateSquareGame::OnFrameRender()
{
	KTDXPROFILE();

	if( 
#ifndef NEW_SKILL_TREE
		false == g_pInstanceData->GetSkillTreeUI()->GetShow() && 
#endif
		/*false == m_pCharacterRoom->GetOpen() && */
        m_pCashShop != NULL &&
        m_pCashShop->GetOpen() == false )
	{
		if( g_pSquareGame != NULL )
		{
			g_pSquareGame->OnFrameRender();
		}
	}
	else
	{
//{{ robobeg : 2008-10-18
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( true );
#endif  X2OPTIMIZE_CULLING_PARTICLE
		g_pKTDXApp->GetDGManager()->ObjectChainSort();
        g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
        g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
    CKTDGParticleSystem::EnableParticleCulling( false );
#endif  X2OPTIMIZE_CULLING_PARTICLE
//}} robobeg : 2008-10-18
	}

	g_pKTDXApp->GetDGManager()->FrontUIRender();

	if ( g_pMain->GetTextManager() != NULL )
		g_pMain->GetTextManager()->OnFrameRender();

	if ( g_pMain->GetGameEdit() != NULL )
		g_pMain->GetGameEdit()->OnFrameRender();

	//{{ 09.03.23 김태완 : 이벤트 타이머 위치 이동
	if( g_pInstanceData != NULL &&
		g_pInstanceData->GetMiniMapUI() != NULL )
	{
		g_pInstanceData->GetMiniMapUI()->UpdateEventNotice();
#ifdef EVENT_CARNIVAL_DECORATION
		g_pInstanceData->GetMiniMapUI()->UpdateCarnivalDeco();
#endif //EVENT_CARNIVAL_DECORATION
	}
	//RenderMarketingEventTimer();
	//}}


	if( m_pCursor != NULL )
		m_pCursor->OnFrameRender();


	return S_OK;
}

HRESULT CX2StateSquareGame::OnResetDevice()
{
	if( g_pSquareGame != NULL )
		g_pSquareGame->OnResetDevice();

	return S_OK;
}

HRESULT CX2StateSquareGame::OnLostDevice()
{
	if( g_pSquareGame != NULL )
		g_pSquareGame->OnLostDevice();

	return S_OK;
}

bool CX2StateSquareGame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
	if( g_pSquareGame != NULL )
	{
		if( g_pSquareGame->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	if( CX2StateMenu::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	// 개인상점 Join
	if ( uMsg == WM_LBUTTONDOWN )
	{
		if (
#ifndef NEW_SKILL_TREE
			g_pInstanceData->GetSkillTreeUI()->GetShow() == false &&
#endif
			m_pCashShop != NULL && m_pCashShop->GetOpen() == false )
		{
#ifdef POSTBOX
            if(g_pMain->GetPostBox()->GetEnable() == true)
                return false;
#endif
			UidType unitUIDToFind = 0;
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			if ( g_pSquareGame->GetTalkBoxManagerPShop() != NULL && 
				g_pSquareGame->GetTalkBoxManagerPShop()->CheckMousePointInTalkbox( mousePos, unitUIDToFind ) == true )
			{
				// 체험 아이디 제한 
				if( true == g_pData->GetMyUser()->GetUserData().m_bIsGuestUser )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
					return true;
				}

				//join
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"X2_Button_Mouse_Up.ogg", false, false );
				if(g_pData->GetUIManager() != NULL)
					g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, unitUIDToFind);
				return true;
			}
		}
	}

	return false;
}


bool CX2StateSquareGame::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
	{
		return true;
	}

	
	
	switch( wParam )
	{
	case SQGUCM_LEAVE_SQUARE_GAME:
		{
			return Handler_EGS_LEAVE_SQUARE_REQ();
		}
		break;

	case SQGUCM_LEAVE_SQUARE_GAME_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGExitMsgBox, NULL, false );
			m_pDLGExitMsgBox = NULL;
		}
        break;

	}

	return false;
}

bool CX2StateSquareGame::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{


	if( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;		

	switch( wParam )
	{
#if 0 
		case EGS_STATE_CHANGE_VILLAGE_MAP_ACK:
			{
				return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;
#endif
		case EGS_JOIN_SQUARE_NOT:
			{
				return Handler_EGS_JOIN_SQUARE_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_LEAVE_SQUARE_ACK:
			{
				return Handler_EGS_LEAVE_SQUARE_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_LEAVE_SQUARE_NOT:
			{
				return Handler_EGS_LEAVE_SQUARE_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_BAN_USER_NOT:
			{
				return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( 0 ); // fix!! 마을 ID ??
			}
			break;

		case EGS_SQUARE_UNIT_SYNC_DATA_NOT:
			{
				return Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_REPAIR_ITEM_NOT:
		case EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT:
			{
				return Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT(  hWnd, uMsg, wParam, lParam );

			} break;

		case EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT:
			{
				return Handler_EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;

	}

	return false;
}


bool CX2StateSquareGame::UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	g_pSquareGame->MsgProc( hWnd, uMsg, wParam, lParam );

	return false;
}

bool CX2StateSquareGame::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( UINT nMapID )
{
#if 0 
	KStateChangeReq kPacket;
	kPacket.m_nMapID = nMapID;		

	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_VILLAGE_MAP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_743 ), this );
#endif
	return true;
}

bool CX2StateSquareGame::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
#if 0 
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KStateChangeAck kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, kEvent.m_nMapID, false );
			return true;
		}
	}
#endif
	return false;
}

bool CX2StateSquareGame::Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SQUARE_UNIT_SYNC_DATA_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pSquareGame->Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( kEvent );
	return true;
}



bool CX2StateSquareGame::Handler_EGS_JOIN_SQUARE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_SQUARE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pSquareGame->Handler_EGS_JOIN_SQUARE_NOT( kEvent );


	return true;
}

bool CX2StateSquareGame::Handler_EGS_LEAVE_SQUARE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_SQUARE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pSquareGame->Handler_EGS_LEAVE_SQUARE_NOT( kEvent );


	return true;
}







bool CX2StateSquareGame::Handler_EGS_LEAVE_SQUARE_REQ( int leaveRoomReason /* = NetError::NOT_LEAVE_ROOM_REASON_00 */ )
{
	KEGS_LEAVE_SQUARE_REQ kPacket;
	kPacket.m_iReason = leaveRoomReason;

	if ( g_pSquareGame != NULL )
		g_pSquareGame->Handler_EGS_LEAVE_SQUARE_REQ();

	g_pData->GetServerProtocol()->SendPacket( EGS_LEAVE_SQUARE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_LEAVE_SQUARE_ACK );

	return true;
}

bool CX2StateSquareGame::Handler_EGS_LEAVE_SQUARE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_SQUARE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_LEAVE_SQUARE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef DISABLE_CHANNEL_CHANGE_IN_SQUARE
			if( NULL != g_pInstanceData->GetMiniMapUI() )
			{
				g_pInstanceData->GetMiniMapUI()->SetEnableChannelComboBox(true);
			}
#endif //DISABLE_CHANNEL_CHANGE_IN_SQUARE

			if(g_pData->GetStateArg() >= SEnum::VMI_RUBEN)
			{
				// 거래광장에서 워프로 마을 이동
				return Handler_EGS_STATE_CHANGE_FIELD_REQ( g_pData->GetStateArg() );
			}

			// 거래광장에서 정상적인 루트로 마을 이동
			return Handler_EGS_STATE_CHANGE_FIELD_REQ();
			//return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( 0 ); // fix!! 마을 ID ??
		}
	}

	return true;
}

bool CX2StateSquareGame::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	return g_pSquareGame->Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT( kEvent );
}

bool CX2StateSquareGame::Handler_EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pSquareGame != NULL )
		g_pSquareGame->Handler_EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT( kEvent );


	return true;
}



void CX2StateSquareGame::SetShowStateDLG( bool bShow )
{
	if ( bShow == true )
	{
// 		if ( m_pDLGSquareTitle != NULL )
// 			m_pDLGSquareTitle->SetShowEnable( true, true );

		g_pKTDXApp->GetDGManager()->GetCamera().Move( m_vOldEyePt.x, m_vOldEyePt.y, m_vOldEyePt.z );
		g_pKTDXApp->GetDGManager()->GetCamera().LookAt( m_vOldLookAtPt.x, m_vOldLookAtPt.y, m_vOldLookAtPt.z );

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), true );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->UnHideChatWindow();
		}
	

		g_pSquareGame->GetWorld()->SetShowObject( true ); 
		g_pSquareGame->GetWorld()->SetMapDetail( g_pMain->GetGameOption().GetOptionList().m_MapDetail );

		g_pSquareGame->SetShowUserUnit( true );

		g_pSquareGame->SetEnableCameraProcess( true );

		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, true );

		if ( g_pSquareGame->GetTalkBoxManager() != NULL )
		{
			g_pSquareGame->GetTalkBoxManager()->SetShowObject( true );
		}

		if ( g_pSquareGame->GetTalkBoxManagerPShop() != NULL )
		{
			g_pSquareGame->GetTalkBoxManagerPShop()->SetShowObject( true );
		}
	}
	else
	{
//		if ( m_pDLGSquareTitle != NULL )
//			m_pDLGSquareTitle->SetShowEnable( false, false );

		m_vOldEyePt		= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
		m_vOldLookAtPt	= g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();

		g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->HideChatWindow();
		}


		g_pSquareGame->GetWorld()->SetShowObject( false ); 
		g_pSquareGame->SetShowUserUnit( false );
		g_pSquareGame->SetEnableCameraProcess( false );

		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, false );


		if ( g_pSquareGame->GetTalkBoxManager() != NULL )
		{
			g_pSquareGame->GetTalkBoxManager()->SetShowObject( false );
		}

		if ( g_pSquareGame->GetTalkBoxManagerPShop() != NULL )
		{
			g_pSquareGame->GetTalkBoxManagerPShop()->SetShowObject( false );
		}
	}

	CX2GageManager::GetInstance()->SetShow( bShow );
}



bool CX2StateSquareGame::ToggleLobbyMenuWindow()
{
	m_bShowWindowMenu = !m_bShowWindowMenu;

	if ( m_bShowWindowMenu == true )
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
		m_pDLGMenuWindow = new CKTDGUIDialog( this, L"DLG_Menu_Window.lua", 0.07f );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMenuWindow );
	}
	else
	{
		SAFE_DELETE_DIALOG( m_pDLGMenuWindow );
	}

	return true;
}


void CX2StateSquareGame::OpenExitDLG()
{
	SAFE_DELETE_DIALOG( m_pDLGExitMsgBox );
	m_pDLGExitMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_562 ), 
		SQGUCM_LEAVE_SQUARE_GAME, this, SQGUCM_LEAVE_SQUARE_GAME_CANCEL );
}

void CX2StateSquareGame::SetShowCommonBG()
{
	if (
#ifndef NEW_SKILL_TREE
		g_pInstanceData->GetSkillTreeUI()->GetShow() == true || 
#endif
		/*m_pCharacterRoom->GetOpen() == true ||*/
        (m_pCashShop != NULL && m_pCashShop->GetOpen() == true) )
	{
		SetOpenCommonBG( true );
	}
	else
	{
		SetOpenCommonBG( false );
	}

}


void CX2StateSquareGame::SetShowMenuByTrade()
{
	m_pDLGMenu->SetShow( true );
	m_pDLGMenu->SetEnable( false );

	m_bEnableShortCutKey = false;
}

void CX2StateSquareGame::SetShowMenuByPShop()
{
	m_pDLGMenu->SetShow( true );
	m_pDLGMenu->SetEnable( true );

	m_bEnableShortCutKey = false;
#ifndef NEW_VILLAGE_UI
	CKTDGUIControl* pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Button" );
	if ( pControl != NULL )
		pControl->SetEnable( false );


	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_My_Bag" );
	if ( pControl != NULL )
		pControl->SetEnable( false );


	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Skill_Tree" );
	if ( pControl != NULL )
		pControl->SetEnable( false );


	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Cash_Shop" );
	if ( pControl != NULL )
		pControl->SetEnable( false );



	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Coupon" );
	if ( pControl != NULL )
		pControl->SetEnable( false );


	pControl = m_pDLGMenu->GetControl( L"ButtonBack" );
	if ( pControl != NULL )
		pControl->SetEnable( false );		

#ifdef POSTBOX
    pControl = m_pDLGMenu->GetControl(L"PostBox_Button");
    if( pControl != NULL )
        pControl->SetEnable( false );
    g_pMain->GetPostBox()->SetDialogShow(false);
#endif
#endif
}

void CX2StateSquareGame::SetShowMenuByDefault()
{
	m_pDLGMenu->SetShow( true );
	m_pDLGMenu->SetEnable( true );

	m_bEnableShortCutKey = true;
#ifndef NEW_VILLAGE_UI
	CKTDGUIControl* pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Button" );
	if ( pControl != NULL )
		pControl->SetEnable( true );


	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_My_Bag" );
	if ( pControl != NULL )
		pControl->SetEnable( true );

	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Skill_Tree" );
	if ( pControl != NULL )
		pControl->SetEnable( true );

	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Cash_Shop" );
	if ( pControl != NULL )
		pControl->SetEnable( true );

	pControl = m_pDLGMenu->GetControl( L"ButtonMenu_Coupon" );
	if ( pControl != NULL )
		pControl->SetEnable( true );

#ifndef NEW_VILLAGE_UI
	pControl = m_pDLGMenu->GetControl( L"ButtonBack" );
	if ( pControl != NULL )
		pControl->SetEnable( true );	
#endif

#ifdef POSTBOX
    pControl = m_pDLGMenu->GetControl(L"PostBox_Button");
    if( pControl != NULL )
        pControl->SetEnable( true );
#endif
#endif
}

/*virtual*/
void CX2StateSquareGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	if( NULL != g_pSquareGame )
		g_pSquareGame->PopTalkBox( iUnitUID_, pWstrMsg_, coTextColor_, coOutColor_, coBackColor_ );
}





#include "StdAfx.h"
#include ".\X2StateWeddingGame.h"

#ifdef ADDED_RELATIONSHIP_SYSTEM
CX2StateWeddingGame::CX2StateWeddingGame(void) :
m_pDLGExitMsgBox(NULL),
m_vOldEyePt(0.0f,0.0f,0.0f),	
m_vOldLookAtPt(0.0f,0.0f,0.0f)
{

	g_pKTDXApp->GetDIManager()->SetEnable(true);
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(),	true );

	SetOpenCommonBG( false );


	SoundReady( L"X2_Button_Mouse_Up.ogg" );

	XSkinMeshReady( L"Mesh_Arme_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Elsword_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Eve_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Lire_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Raven_Base_SD.x" );
	XSkinMeshReady( L"Mesh_CHUNG_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Ara_Base_SD.x" );
	XSkinMeshReady( L"Mesh_Add_Base_SD.x" );
	

	SetEnableShortCutKey(false);

	CreateWeddingStartButton();
}//CX2StateWeddingGame::CX2StateWeddingGame()

CX2StateWeddingGame::~CX2StateWeddingGame(void)
{
	SAFE_DELETE_DIALOG( m_pDLGExitMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGWeddingStart );
	SAFE_DELETE( g_pWeddingGame );

	g_pKTDXApp->GetDIManager()->SetEnable(true);
}

HRESULT CX2StateWeddingGame::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );

	if( NULL != g_pWeddingGame )
		g_pWeddingGame->OnFrameMove( fTime, fElapsedTime );

	if( NULL != g_pData && NULL != g_pData->GetUIManager() )
		g_pData->GetUIManager()->SetShowUI(false);

	return S_OK;
}

HRESULT CX2StateWeddingGame::OnFrameRender()
{
	KTDXPROFILE();

	if( g_pWeddingGame != NULL )
	{
		g_pWeddingGame->OnFrameRender();
	}

	g_pKTDXApp->GetDGManager()->FrontUIRender();

	if ( g_pMain->GetTextManager() != NULL )
		g_pMain->GetTextManager()->OnFrameRender();

	if ( g_pMain->GetGameEdit() != NULL )
		g_pMain->GetGameEdit()->OnFrameRender();

	if( m_pCursor != NULL )
		m_pCursor->OnFrameRender();

	return S_OK;
}

HRESULT CX2StateWeddingGame::OnResetDevice()
{
	if( g_pWeddingGame != NULL )
		g_pWeddingGame->OnResetDevice();

	return S_OK;
}

HRESULT CX2StateWeddingGame::OnLostDevice()
{
	if( g_pWeddingGame != NULL )
		g_pWeddingGame->OnLostDevice();

	return S_OK;
}

bool CX2StateWeddingGame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pWeddingGame != NULL )
	{
		if( g_pWeddingGame->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}

	if( CX2StateMenu::MsgProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	return false;
}


bool CX2StateWeddingGame::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
	{
		return true;
	}

	switch( wParam )
	{
	case SWGUCM_LEAVE_WEDDING_GAME:
		{
			Handler_EGS_LEAVE_WEDDING_HALL_REQ();
			return true;
		}
		break;

	case SWGUCM_LEAVE_WEDDING_GAME_CANCEL:
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGExitMsgBox, NULL, false );
			m_pDLGExitMsgBox = NULL;
			return true;
		}
		break;


	case SWGUCM_WEDDING_START:
		{ // 결혼식 시작 요청 메세지
			if( NULL != g_pWeddingGame && true == g_pWeddingGame->GetIsCanWeddingStart() )
			{
				Handler_EGS_START_WEDDING_REQ();
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_24676 ), g_pMain->GetNowState() );
			}

			return true;
		} break;

	case SWGUCM_WEDDING_START_OK:
		{ // 시작 수락 ( 신랑, 신부 모두 OK 해야지 시작 됨 )
			if( NULL != g_pWeddingGame && true == g_pWeddingGame->GetIsCanWeddingStart() )
			{
				Handler_EGS_START_WEDDING_ASK_ACK(true);
			}
			else
			{
				Handler_EGS_START_WEDDING_ASK_ACK(false);
			}

			return true;
		} break;

	case SWGUCM_WEDDING_START_CANCEL:
		{
			Handler_EGS_START_WEDDING_ASK_ACK(false);

			return true;
		} break;
	}
	return false;
}

bool CX2StateWeddingGame::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;		

	switch( wParam )
	{
		case EGS_LEAVE_WEDDING_HALL_ACK:
			{
				return Handler_EGS_LEAVE_WEDDING_HALL_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;

		case EGS_LEAVE_WEDDING_HALL_NOT:
			{
				return Handler_EGS_LEAVE_WEDDING_HALL_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
		case EGS_START_WEDDING_ACK:
			{
				return Handler_EGS_START_WEDDING_ACK( hWnd, uMsg, wParam, lParam );
			}
			break;
		case EGS_START_WEDDING_ASK_REQ:
			{
				return Handler_EGS_START_WEDDING_ASK_REQ();
			}
			break;
		case EGS_START_WEDDING_NOT:
			{
				return Handler_EGS_START_WEDDING_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
		case EGS_START_WEDDING_AGREE_NOT:
			{
				return Handler_EGS_START_WEDDING_AGREE_NOT( hWnd, uMsg, wParam, lParam );
			}
			break;
	}

	return false;
}


bool CX2StateWeddingGame::UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL != g_pWeddingGame )
		g_pWeddingGame->MsgProc( hWnd, uMsg, wParam, lParam );

	return false;
}

bool CX2StateWeddingGame::Handler_EGS_LEAVE_WEDDING_HALL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_WEDDING_HALL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pWeddingGame )
		g_pWeddingGame->Handler_EGS_LEAVE_WEDDING_HALL_NOT( kEvent );

	return true;
}

void CX2StateWeddingGame::Handler_EGS_LEAVE_WEDDING_HALL_REQ()
{
	KEGS_LEAVE_WEDDING_HALL_REQ kPacket;
	if ( g_pWeddingGame != NULL )
	{
		g_pWeddingGame->Handler_EGS_LEAVE_WEDDING_HALL_REQ();
		switch ( static_cast<CX2Main::X2_STATE>(g_pWeddingGame->GetReturnStateID()) )
		{
		case CX2Main::XS_VILLAGE_MAP:
			{
				kPacket.m_iReason = NetError::NOT_LEAVE_FIELD_REASON_00;	//다른 필드로 이동하기 위한 이탈입니다.
			} break;
		case CX2Main::XS_BATTLE_FIELD:
			{
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;	//배틀필드 입장을 위한 방 이탈 입니다.
			} break;
		default:
			 break;
		}
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_LEAVE_WEDDING_HALL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_LEAVE_WEDDING_HALL_ACK );
}

bool CX2StateWeddingGame::Handler_EGS_LEAVE_WEDDING_HALL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_WEDDING_HALL_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_LEAVE_WEDDING_HALL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{
			switch( kPacket.m_iOK )
			{
			//배틀필드 입장을 위한 방 이탈 입니다.
			case NetError::NOT_LEAVE_ROOM_REASON_27:
				{
					CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
					if( NULL != pState )
					{
						CX2BattleFieldManager& battleFieldManager = g_pData->GetBattleFieldManager();
						pState->Handler_EGS_JOIN_BATTLE_FIELD_REQ( battleFieldManager.GetBattleFieldIdWhereIam(),
																battleFieldManager.GetBattleFieldPositionIndexWhereIShouldBe(),
																battleFieldManager.GetBattleFieldPositionValue());
					}
				} break;
			//다른 필드로 이동하기 위한 이탈입니다.
			// IsValidPacket에서 걸리지 않은 경우 OK라고 판단하고 defluat에도 마을로 보내기.
			case NetError::NOT_LEAVE_FIELD_REASON_00:
			default:
				{
					if(g_pData->GetStateArg() >= SEnum::VMI_RUBEN)
					{
						// 거래광장에서 워프로 마을 이동
						return Handler_EGS_STATE_CHANGE_FIELD_REQ( g_pData->GetStateArg() );
					}
					else
					{
						// 거래광장에서 정상적인 루트로 마을 이동
						return Handler_EGS_STATE_CHANGE_FIELD_REQ();
					}
				} break;
			}
		}
	}

	return true;
}

// 결혼 시작 요청 
void CX2StateWeddingGame::Handler_EGS_START_WEDDING_REQ()
{
	if( true == g_pMain->IsThereTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE ) )
		return;

	if( true == g_pMain->IsThereTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_OK_CANCEL ) )
		return;

	g_pData->GetServerProtocol()->SendID( EGS_START_WEDDING_REQ );
	g_pMain->AddServerPacket( EGS_START_WEDDING_ACK );
}
bool CX2StateWeddingGame::Handler_EGS_START_WEDDING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_START_WEDDING_ACK kPacket;
	DeSerialize( pBuff, &kPacket );

	if( g_pMain->DeleteServerPacket( EGS_START_WEDDING_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kPacket.m_iOK ) == true )
		{
			// 결혼 수락 대기중 타임 메세지
			if( NULL != g_pWeddingGame && 
				NULL != g_pWeddingGame->GetMyUnit())
			{
				CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
				userData.iMyUID = g_pWeddingGame->GetMyUnit()->GetUID();
				g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
												CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 5.f,
												GET_STRING(STR_ID_24600),
												g_pMain->GetNowState(), -1, -1 );
			}
		}
	}

	return true;
}
// 서버로부터 받는 REQ, 서버로 응답하는 ACK
bool CX2StateWeddingGame::Handler_EGS_START_WEDDING_ASK_REQ()
{
	// 결혼시작 확인/취소 버튼 출력
	if( NULL != g_pWeddingGame && 
		NULL != g_pWeddingGame->GetMyUnit())
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID = g_pWeddingGame->GetMyUnit()->GetUID();
		g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_OK_CANCEL,
										CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 5.f,
										GET_STRING(STR_ID_24601), g_pMain->GetNowState(), 
										SWGUCM_WEDDING_START_OK, SWGUCM_WEDDING_START_CANCEL, SWGUCM_WEDDING_START_CANCEL);
	}

	return true;
}
void CX2StateWeddingGame::Handler_EGS_START_WEDDING_ASK_ACK( bool bOk_ )
{
	if( NULL != g_pWeddingGame && 
		NULL != g_pWeddingGame->GetMyUnit())
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID = g_pWeddingGame->GetMyUnit()->GetUID();
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_OK_CANCEL, userData );
	}

	KEGS_START_WEDDING_ASK_ACK kPacket;
	kPacket.m_bAgree = bOk_;

	g_pData->GetServerProtocol()->SendPacket( EGS_START_WEDDING_ASK_ACK, kPacket );
}
bool CX2StateWeddingGame::Handler_EGS_START_WEDDING_AGREE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_START_WEDDING_AGREE_NOT kPacket;
	DeSerialize( pBuff, &kPacket );

	if( NULL != g_pWeddingGame && 
		NULL != g_pWeddingGame->GetMyUnit())
	{
		CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
		userData.iMyUID = g_pWeddingGame->GetMyUnit()->GetUID();
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE, userData );
		g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_OK_CANCEL, userData );
	}

	if( false == kPacket.m_bAgree )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_24602 ), g_pMain->GetNowState() );
	}
	return true;
}

bool CX2StateWeddingGame::Handler_EGS_START_WEDDING_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_START_WEDDING_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		// 결혼식 시작
		if( NULL != g_pWeddingGame )
			g_pWeddingGame->StartWedding();

		DeleteWeddingStartButton();
		
		if ( true == g_pWeddingGame->IsWeddingHeroineMyUnit() )
		{
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() &&
				NULL != g_pData->GetMessenger() )
			{
				g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType = SEnum::RT_MARRIED;
				g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_ctsDurationDay = 0;
				g_pData->GetMessenger()->SetRelationshipUI ( SEnum::RT_MARRIED );
			}
		}	
	}

	return true;
}
void CX2StateWeddingGame::SetShowStateDLG( bool bShow )
{
	if ( bShow == true )
	{
		g_pKTDXApp->GetDGManager()->GetCamera().Move( m_vOldEyePt.x, m_vOldEyePt.y, m_vOldEyePt.z );
		g_pKTDXApp->GetDGManager()->GetCamera().LookAt( m_vOldLookAtPt.x, m_vOldLookAtPt.y, m_vOldLookAtPt.z );

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), true );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->UnHideChatWindow();
		}

		if( NULL != g_pWeddingGame )
		{
			g_pWeddingGame->SetShowUserUnit( true );
			g_pWeddingGame->SetEnableCameraProcess( true );

			if( NULL != g_pWeddingGame->GetWorld()	)			
			{
				g_pWeddingGame->GetWorld()->SetShowObject( true ); 
				g_pWeddingGame->GetWorld()->SetMapDetail( g_pMain->GetGameOption().GetOptionList().m_MapDetail );
			}

			if ( g_pWeddingGame->GetTalkBoxManager() != NULL )
			{
				g_pWeddingGame->GetTalkBoxManager()->SetShowObject( true );
			}
		}

		if( NULL != g_pInstanceData &&
			NULL != g_pInstanceData->GetMiniMapUI() )
		{
			g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, true );
		}
	}
	else
	{
		m_vOldEyePt		= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
		m_vOldLookAtPt	= g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();

		g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );

		if( NULL != g_pChatBox )
		{
			g_pChatBox->HideChatWindow();
		}

		if( NULL != g_pWeddingGame )
		{
			g_pWeddingGame->SetShowUserUnit( false );
			g_pWeddingGame->SetEnableCameraProcess( false );

			if( NULL != g_pWeddingGame->GetWorld()	)			
			{
				g_pWeddingGame->GetWorld()->SetShowObject( false ); 
			}
			if ( g_pWeddingGame->GetTalkBoxManager() != NULL )
			{
				g_pWeddingGame->GetTalkBoxManager()->SetShowObject( false );
			}
		}

		if( NULL != g_pInstanceData &&
			NULL != g_pInstanceData->GetMiniMapUI() )
		{
			g_pInstanceData->GetMiniMapUI()->SetShowMiniMap( CX2MiniMapUI::MMT_DUNGEON, false );
		}
	}

	CX2GageManager::GetInstance()->SetShow( bShow );
}

bool CX2StateWeddingGame::ToggleLobbyMenuWindow()
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

void CX2StateWeddingGame::OpenExitDLG()
{
	SAFE_DELETE_DIALOG( m_pDLGExitMsgBox );
	m_pDLGExitMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_562 ), 
		SWGUCM_LEAVE_WEDDING_GAME, this, SWGUCM_LEAVE_WEDDING_GAME_CANCEL );
}

void CX2StateWeddingGame::SetShowCommonBG()
{
	if ((m_pCashShop != NULL && m_pCashShop->GetOpen() == true) )
	{
		SetOpenCommonBG( true );
	}
	else
	{
		SetOpenCommonBG( false );
	}
}

bool CX2StateWeddingGame::ShortCutKeyProcess()
{
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F6 ) == TRUE )
	{
			g_pMain->GetGameOption().ToggleFieldSD();
		return true;
	}
	else if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_DELETE) == TRUE )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ToggleShow();
		return true;
	}
	return false;
}

void CX2StateWeddingGame::CreateWeddingStartButton()
{
	if( NULL != g_pWeddingGame &&
		true == g_pWeddingGame->IsWeddingHeroineMyUnit() )
	{
		if( NULL == m_pDLGWeddingStart )
		{
			m_pDLGWeddingStart = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_WeddingHall.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWeddingStart );
			m_pDLGWeddingStart->SetShow(true);
		}
		else
		{
			m_pDLGWeddingStart->SetShow(true);
		}
	}
}
void CX2StateWeddingGame::DeleteWeddingStartButton()
{
	if( NULL != m_pDLGWeddingStart )
		m_pDLGWeddingStart->SetShow(false);
}

/*virtual*/
void CX2StateWeddingGame::PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ )
{
	if( NULL != g_pWeddingGame )
	{
		g_pWeddingGame->PopTalkBox( iUnitUID_, pWstrMsg_, coTextColor_, coOutColor_, coBackColor_ );
	}
}

#endif //ADDED_RELATIONSHIP_SYSTEM


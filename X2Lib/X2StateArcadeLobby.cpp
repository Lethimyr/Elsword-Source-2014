#include "StdAfx.h"
#include ".\x2statearcadelobby.h"

//CX2StateArcadeLobby::CX2StateArcadeLobby(void)
//{
//	m_pDLGArcadeLobbyBack = NULL;
//	m_pDLGArcadeCreateRoom = NULL;
//
//	m_bEnableRefreshRoomList = true;
//	m_bEnableResetDLGRoomList = true;
//
//	m_NowPage				= 1;
//	m_EndPage				= 1;
//
//	m_iMaxNumOfRoom			= 14;
//
//	m_bCreateRoomObserver	= false;
//
//	m_fRefreshTime			= 2.5f;
//
//	m_TempChannelID			= 0;
//
//	m_pDLGJoinPasswordRoom	= NULL;
//	m_JoinRoomUid			= 0;
//
//	m_pPicCharTodayRankStage = NULL;
//	m_pPicCharTodayRankTime	= NULL;
//
//	m_pPicCharTodayRankStageSmall = NULL;
//	m_pPicCharTodayRankTimeSmall = NULL;
//
//	InitArcadeLobbyUI();
//
//	Handler_EGS_ROOM_LIST_REQ( m_NowPage );
//
//	g_pData->DeleteDungeonRoom();
//	g_pData->DeleteDungeonResultInfo();
//
//	g_pKTDXApp->SkipFrame();
//
//}
//
//CX2StateArcadeLobby::~CX2StateArcadeLobby(void)
//{
//	SAFE_DELETE_DIALOG( m_pDLGArcadeLobbyBack );
//	SAFE_DELETE_DIALOG( m_pDLGArcadeCreateRoom );
//	SAFE_DELETE_DIALOG( m_pDLGJoinPasswordRoom );
//
//	for( int i = 0; i < (int)m_RoomList.size(); i++ )
//	{
//		CX2DungeonRoom::DungeonRoomSimpleInfo* pRoomData = m_RoomList[i];
//		SAFE_DELETE( pRoomData );
//	}
//	m_RoomList.clear();
//
//	for ( int i = 0; i < (int)m_SimpleRoomList.size(); i++ )
//	{
//		//CKTDGUIDialogType pDialog = NULL;
//		SimpleRoom* pSimpleRoom = m_SimpleRoomList[i];
//		SAFE_DELETE( pSimpleRoom );
//	}
//	m_SimpleRoomList.clear();
//
//	if ( m_pPicCharTodayRankStage != NULL )
//	{
//		m_pPicCharTodayRankStage->Clear();
//		SAFE_DELETE( m_pPicCharTodayRankStage );
//	}
//
//	if ( m_pPicCharTodayRankTime != NULL )
//	{
//		m_pPicCharTodayRankTime->Clear();
//		SAFE_DELETE( m_pPicCharTodayRankTime );
//	}
//
//	if ( m_pPicCharTodayRankStageSmall != NULL )
//	{
//		m_pPicCharTodayRankStageSmall->Clear();
//		SAFE_DELETE( m_pPicCharTodayRankStageSmall );
//	}
//
//	if ( m_pPicCharTodayRankTimeSmall != NULL )
//	{
//		m_pPicCharTodayRankTimeSmall->Clear();
//		SAFE_DELETE( m_pPicCharTodayRankTimeSmall );
//	}
//}
//
//HRESULT CX2StateArcadeLobby::OnFrameMove(double fTime, float fElapsedTime )
//{
//	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );
//
//	if( true == g_pMain->GetIsPlayingTutorial() )
//		return S_OK;
//
//	m_fRefreshTime += fElapsedTime;
//
//	if ( 3.0f <= m_fRefreshTime )
//	{
//		m_fRefreshTime = 0.0f;
//		Handler_EGS_ROOM_LIST_REQ( m_NowPage );
//	}
//
//	DrawPicChar( fElapsedTime );
//
//	return S_OK;
//}
//
//bool CX2StateArcadeLobby::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	if ( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
//		return true;
//
//	switch(wParam)
//	{
//	case ALUCM_OPEN_CREATE_ROOM_DLG:
//		{
//			OpenCreateRoomDLG();	
//		}
//		break;
//
//	case ALUCM_OPEN_CREATE_ROOM_DLG_OK:
//		{
//			Handler_EGS_CREATE_ROOM_REQ();
//			CloseCreateRoomDLG();
//		}
//		break;
//
//	case ALUCM_OPEN_CREATE_ROOM_DLG_CANCEL:
//		{
//			CloseCreateRoomDLG();
//		}
//		break;
//
//	case ALUCM_ROOM_BACK_PAGE:
//		{
//			if( m_NowPage <= 1 )
//				return true;
//
//			m_NowPage--;
//			Handler_EGS_ROOM_LIST_REQ( m_NowPage );
//		}
//		break;
//
//	case ALUCM_ROOM_NEXT_PAGE:
//		{
//			if( m_NowPage >= m_EndPage )
//				return true;
//
//			m_NowPage++;
//			Handler_EGS_ROOM_LIST_REQ( m_NowPage );
//		}
//		break;
//
//	case ALUCM_QUICK_JOIN:
//		{
//			return Handler_EGS_QUICK_JOIN_REQ();
//		}
//		break;
//
//	case ALUCM_CHANGE_CHANNEL:
//		{
//			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
//			int channelID = pControl->GetDummyInt(0);
//			return Handler_EGS_CHANGE_ARCADE_CHANNEL_REQ( channelID );
//		}
//		break;
//
//	case ALUCM_ROOM_PASSWORD_CHECK_OK:
//		{
//			if ( m_pDLGJoinPasswordRoom != NULL )
//			{
//				CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGJoinPasswordRoom->GetControl( L"EditBoxPassword" );
//				wstring passWord = pIMEEditBox->GetText();
//				Handler_EGS_JOIN_ROOM_REQ( m_JoinRoomUid, false, passWord );
//			}
//
//			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGJoinPasswordRoom, NULL, true );
//			m_pDLGJoinPasswordRoom = NULL;
//		}
//		break;
//
//	case ALUCM_ROOM_PASSWORD_CHECK_CANCEL:
//		{
//			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGJoinPasswordRoom, NULL, true );
//			m_pDLGJoinPasswordRoom = NULL;
//
//			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
//			{
//				g_pMain->GetStateAutoChanger().EndStateChange();
//			}
//		}
//		break;
//
//	case ALUCM_JOIN_ROOM:
//		{
//			CKTDGUIButton* pJoinButton	= (CKTDGUIButton*)lParam;
//			CKTDGUIDialogType pDialog		= pJoinButton->GetDialog();
//
//			int simpleRoomIndex = -1;
//
//			WCHAR tempSTR[10] = {0};
//			CKTDGUIStatic* pStaticRoomNum = (CKTDGUIStatic*)pDialog->GetControl( L"Arcade_Lobby_Room_Number" );
//
//			simpleRoomIndex				= _wtoi( pStaticRoomNum->GetString(0)->msg.c_str() );
//			SimpleRoom*	pSimpleRoom		= m_SimpleRoomList[simpleRoomIndex];
//			m_JoinRoomUid				= pSimpleRoom->roomUid;
//
//			if( simpleRoomIndex != -1 )
//			{
//				for ( int i = 0; i < (int)m_RoomList.size(); i++ )
//				{
//					CX2DungeonRoom::DungeonRoomSimpleInfo* pTempData = m_RoomList[i];
//
//					if ( pTempData->m_RoomUID == m_JoinRoomUid )
//					{
//						if ( pTempData->m_bPublic == true )
//						{
//							return Handler_EGS_JOIN_ROOM_REQ( m_JoinRoomUid );
//						}
//						else
//						{
//							OpenDLGJoinPasswordRoom( m_JoinRoomUid );
//							return true;
//						}
//					}
//				}					
//			}
//			else
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"입장할 수 없습니다.", this );
//		}
//		break;
//	}
//
//	return false;
//}
//
//bool CX2StateArcadeLobby::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	if ( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
//		return true;
//
//	switch(wParam)
//	{
//	case EGS_STATE_CHANGE_VILLAGE_MAP_ACK:
//		return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( hWnd, uMsg, wParam, lParam );
//		break;
//
//	case EGS_JOIN_ROOM_ACK:
//		return Handler_EGS_JOIN_ROOM_ACK( hWnd, uMsg, wParam, lParam );
//		break;
//
//	case EGS_ROOM_LIST_ACK:
//		return Handler_EGS_ROOM_LIST_ACK( hWnd, uMsg, wParam, lParam );
//		break;
//
//	case EGS_QUICK_JOIN_ACK:
//		return Handler_EGS_QUICK_JOIN_ACK( hWnd, uMsg, wParam, lParam );
//		break;
//
//	case EGS_CREATE_ROOM_ACK:
//		return Handler_EGS_CREATE_ROOM_ACK( hWnd, uMsg, wParam, lParam );
//		break;
//
//	case EGS_CHANGE_ARCADE_CHANNEL_ACK:
//		return Handler_EGS_CHANGE_ARCADE_CHANNEL_ACK( hWnd, uMsg, wParam, lParam );
//		break;
//	}
//
//	return false;
//}
//
//bool CX2StateArcadeLobby::ClickMenuBackButton()
//{
//	return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//}
//
//void CX2StateArcadeLobby::SetShowStateDLG( bool bShow )
//{
//	if ( m_pDLGArcadeLobbyBack != NULL )
//		m_pDLGArcadeLobbyBack->SetShowEnable( bShow, bShow );
//
//	m_bEnableResetDLGRoomList = bShow;
//
//	if ( bShow == false )
//	{
//		if ( m_bEnableResetDLGRoomList == false )
//		{
//			for( int i=0; i<m_iMaxNumOfRoom; i++ )
//			{
//				SimpleRoom* pSimpleRoom = m_SimpleRoomList[i];
//				CKTDGUIDialogType pDialog = pSimpleRoom->pDialog;
//				if ( pDialog != NULL )
//					pDialog->SetShow( false );
//
//			}
//		}
//	}
//
//	if (m_bEnableResetDLGRoomList == true )
//		Handler_EGS_ROOM_LIST_REQ( m_NowPage );
//	
//}
//
//bool CX2StateArcadeLobby::LastMsgByESC()
//{
//	OpenLastMsgPopUp( L"이미르시공에서 나가시겠습니까?" );
//	return true;
//}
//
//void CX2StateArcadeLobby::LastMsgPopOk()
//{
//	ClickMenuBackButton();
//}
//
//void CX2StateArcadeLobby::InitArcadeLobbyUI()
//{
//	SAFE_DELETE_DIALOG( m_pDLGArcadeLobbyBack );
//	m_pDLGArcadeLobbyBack = new CKTDGUIDialog( this, L"DLG_Dungeon_Arcade_Lobby.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGArcadeLobbyBack );
//
//	const float MAGIC_X_POS = 77;
//	const float MAGIC_Y_POS = 159;
//	const float MAGIC_HEIGHT = 33.f;
//
//	for( int i = 0; i < m_iMaxNumOfRoom; i++ )
//	{
//		CKTDGUIDialogType pDLGPVPLobbyRoom = new CKTDGUIDialog( this, L"DLG_Dungeon_Arcade_Lobby_Slot.lua" );
//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDLGPVPLobbyRoom );
//		SimpleRoom* pSimpleRoom = new SimpleRoom();
//		pSimpleRoom->pDialog = pDLGPVPLobbyRoom;
//
//		pDLGPVPLobbyRoom->SetPos( D3DXVECTOR2( MAGIC_X_POS, MAGIC_Y_POS + MAGIC_HEIGHT * i ) );
//
//		m_SimpleRoomList.push_back( pSimpleRoom );
//	}
//	ResetRoomListUI();
//
//	ResetChannelRadioButtonUI( g_pData->GetAracdeChannelData().m_nChannelID );
//
//	ResetRankingUI();
//
//	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"TodayRankPickChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetDynamicChain( false );
//	m_pPicCharTodayRankStage = new CKTDGPicChar( L"HQ_RankPC_", pSeq );
//	m_pPicCharTodayRankStage->SetWidth( 15.0f );
//
//	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"TodayRankPickCharTime", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetDynamicChain( false );
//	m_pPicCharTodayRankTime = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
//	m_pPicCharTodayRankTime->SetWidth( 16.0f );
//
//	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"TodayRankPickCharSmall", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetDynamicChain( false );
//	m_pPicCharTodayRankStageSmall = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
//	m_pPicCharTodayRankStageSmall->SetWidth( 12.0f );
//
//	pSeq = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"TodayRankPickCharTimeSmall", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
//	pSeq->SetDynamicChain( false );
//	m_pPicCharTodayRankTimeSmall = new CKTDGPicChar( g_pData->GetPicChar(), pSeq );
//	m_pPicCharTodayRankTimeSmall->SetWidth( 12.0f );
//}
//
//void CX2StateArcadeLobby::OpenCreateRoomDLG()
//{
//	SAFE_DELETE_DIALOG( m_pDLGArcadeCreateRoom );
//	m_pDLGArcadeCreateRoom = new CKTDGUIDialog( this, L"DLG_Dungeon_Arcade_Lobby_Create_Room.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGArcadeCreateRoom );
//
//	CKTDGUIIMEEditBox* pControl = (CKTDGUIIMEEditBox*)m_pDLGArcadeCreateRoom->GetControl( L"IME_Arcade_Room_Name" );
//	if ( pControl != NULL )
//	{
//		int iRandomNameIndex = rand()%5;
//		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
//		switch(iRandomNameIndex)
//		{
//		case 0:
//			{
//				pControl->SetText( L"끝판깨고 좋은 보상받으실분!!", true );
//			} break;
//		case 1:
//			{
//				pControl->SetText( L"X-MODE의 비밀을 밝혀라~~", true );
//			} break;
//		case 2:
//			{
//				pControl->SetText( L"열정과 헌신으로 순위권 진입!!", true );
//			} break;
//		case 3:
//			{
//				pControl->SetText( L"이미르의 시공 무한도전!!", true );
//			} break;
//		default:
//			{
//				pControl->SetText( L"숨막히는 타임어택~~", true );
//			} break;
//		}
//		//}}
//		
//		m_pDLGArcadeCreateRoom->RequestFocus( pControl );
//	}
//
//	
//
//}
//
//void CX2StateArcadeLobby::CloseCreateRoomDLG()
//{
//	if ( m_pDLGArcadeCreateRoom != NULL )
//		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGArcadeCreateRoom, NULL, false );
//
//	m_pDLGArcadeCreateRoom = NULL;
//}
//
//bool CX2StateArcadeLobby::Handler_EGS_ROOM_LIST_REQ( int pageNum )
//{
//	if ( m_bEnableRefreshRoomList == false )
//		return true;
//
//	KEGS_ROOM_LIST_REQ	kPacket;
//	kPacket.m_nViewPage			= pageNum;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_ROOM_LIST_REQ, kPacket );
//
//	return true;
//}
//
//bool CX2StateArcadeLobby::Handler_EGS_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_ROOM_LIST_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
//
//	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//	{
//		m_EndPage = kEvent.m_nTotalPage;
//
//		for( int i = 0; i < (int)m_RoomList.size(); i++ )
//		{
//			CX2DungeonRoom::DungeonRoomSimpleInfo* pPVPRoomSimpleInfo = m_RoomList[i];
//			SAFE_DELETE( pPVPRoomSimpleInfo );
//		}
//		m_RoomList.resize(0);
//
//		for( int i = 0; i < (int)kEvent.m_vRoomInfo.size(); i++ )
//		{
//			KRoomInfo* pRoomInfo = &kEvent.m_vRoomInfo[i];
//			CX2DungeonRoom::DungeonRoomSimpleInfo* pPVPRoomSimpleInfo = new CX2DungeonRoom::DungeonRoomSimpleInfo;
//			pPVPRoomSimpleInfo->Set_KRoomInfo( pRoomInfo );
//
//			m_RoomList.push_back( pPVPRoomSimpleInfo );
//		}
//
//		ResetRoomListUI();
//		
//	}
//	return true;
//}
//
//void CX2StateArcadeLobby::ResetRoomListUI()
//{
//	if ( m_bEnableResetDLGRoomList == false )
//	{
//		for( int i=0; i<m_iMaxNumOfRoom; i++ )
//		{
//			SimpleRoom* pSimpleRoom = m_SimpleRoomList[i];
//			CKTDGUIDialogType pDialog = pSimpleRoom->pDialog;
//			if ( pDialog != NULL )
//				pDialog->SetShow( false );
//
//		}
//
//		return;
//	}
//
//	for( int i=0; i<m_iMaxNumOfRoom; i++ )
//	{
//		SimpleRoom* pSimpleRoom = m_SimpleRoomList[i];
//		CKTDGUIDialogType pDialog = pSimpleRoom->pDialog;
//		if ( pDialog == NULL )
//			continue;
//
//		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( pDialog ) == false )
//		{
//			ErrorLog( XEM_ERROR95 );
//			continue;
//		}
//
//		pDialog->SetShow( false );
//
//		//all put-off
//
//		//state , public, peopleNum, title
//
//		CKTDGUIStatic* pStaticState = (CKTDGUIStatic*)pDialog->GetControl(L"Arcade_Lobby_State");
//		if ( pStaticState != NULL )
//		{
//			for ( int i = 0; i < 3; i++ )
//			{
//				if ( pStaticState->GetPicture(i) != NULL )
//					pStaticState->GetPicture(i)->SetShow( false );
//			}
//		}
//
//		CKTDGUIStatic* pRoomStateLock = (CKTDGUIStatic*)pDialog->GetControl( L"Arcade_Lobby_State_Lock" );
//		if ( pRoomStateLock != NULL )
//		{
//			pRoomStateLock->SetShow( false );
//		}
//	}
//
//
//
//	int iRoomCount = 0;
//	for( UINT i=0; i<m_RoomList.size(); i++ )
//	{
//		CX2DungeonRoom::DungeonRoomSimpleInfo* pRoomData = m_RoomList[i];
//		if ( pRoomData == NULL )
//			continue;
//
//		CKTDGUIDialogType pDialog		= NULL;
//		SimpleRoom* pSimpleRoom		= m_SimpleRoomList[iRoomCount];
//		pDialog						= pSimpleRoom->pDialog;
//		if ( pDialog == NULL )
//			continue;
//		pSimpleRoom->roomUid		= pRoomData->m_RoomUID;
//		pDialog->SetShow( true );
//
//
//		if ( pRoomData->m_bPublic == false )
//		{
//			CKTDGUIStatic* pRoomStateLock = (CKTDGUIStatic*)pDialog->GetControl( L"Arcade_Lobby_State_Lock" );
//			if ( pRoomStateLock != NULL )
//			{
//				pRoomStateLock->SetShow( true );
//			}
//
//		}
//
//
//		WCHAR userNumBuff[256] = {0};
//		wsprintf( userNumBuff, L"%d/%d", pRoomData->m_JoinSlot, pRoomData->m_MaxSlot );
//		CKTDGUIStatic* pStaticJoinUserNum = (CKTDGUIStatic*)pDialog->GetControl( L"Arcade_Lobby_Player_Number" );
//		pStaticJoinUserNum->SetShow( true );
//		pStaticJoinUserNum->GetString(0)->msg = userNumBuff;
//
//		
//
//		CKTDGUIStatic* pStaticState = (CKTDGUIStatic*)pDialog->GetControl(L"Arcade_Lobby_State");
//
//
//		if ( pStaticState != NULL )
//		{
//			if( pRoomData->m_RoomState == CX2Room::RS_PLAY || pRoomData->m_RoomState == CX2Room::RS_LOADING )
//			{
//				// Play_Mark
//
//				if ( pStaticState->GetPicture(1) != NULL )
//					pStaticState->GetPicture(1)->SetShow( true );
//			}
//			else
//			{
//				if( pRoomData->m_JoinSlot >= pRoomData->m_MaxSlot )
//				{
//					// Full_Mark
//					if ( pStaticState->GetPicture(2) != NULL )
//						pStaticState->GetPicture(2)->SetShow( true );
//				}
//				else
//				{
//					// Wait_Mark
//					if ( pStaticState->GetPicture(0) != NULL )
//						pStaticState->GetPicture(0)->SetShow( true );
//				}
//			}
//		}
//
//
//
//
//		WCHAR tempSTR[256] = {0};
//		CKTDGUIStatic* pStaticRoomNum = (CKTDGUIStatic*)pDialog->GetControl( L"Arcade_Lobby_Room_Number" );
//		pStaticRoomNum->SetShow( true );
//		//	wsprintf( tempSTR, 	L"%d" ,pRoomData->m_RoomUID );
//		//	pStaticRoomNum->GetString(0)->msg = tempSTR;
//
//		wsprintf( tempSTR, 	L"%d" ,iRoomCount );
//		pStaticRoomNum->GetString(0)->msg = tempSTR;
//
//		CKTDGUIStatic* pStaticRoomName = (CKTDGUIStatic*)pDialog->GetControl( L"Arcade_Lobby_Room_Title" );
//		pStaticRoomName->SetShow( true );
//		pStaticRoomName->GetString(0)->msg = L"";
//		if ( pRoomData->m_RoomName.size() < 15 )
//		{
//			pStaticRoomName->GetString(0)->msg = pRoomData->m_RoomName;
//		}
//		else
//		{
//			pStaticRoomName->GetString(0)->msg.assign( pRoomData->m_RoomName.begin(), pRoomData->m_RoomName.begin() + 15 );
//		}
//
//
//
//		iRoomCount++;
//	}
//}
//
//
//bool CX2StateArcadeLobby::Handler_EGS_JOIN_ROOM_REQ( UidType roomUid, bool bIsObserver /*= false*/, wstring roomPassword /*= L""*/ )
//{
//
//#ifdef DISABLE_REDUDANT_PACKET_TEST
//	if( true == g_pMain->IsWaitingServerPacket( EGS_JOIN_ROOM_ACK ) )
//	{
//		return false;
//	}
//#endif DISABLE_REDUDANT_PACKET_TEST
//
//
//
//
//	KEGS_JOIN_ROOM_REQ kPacket;
//	kPacket.m_iRoomUID			= roomUid;
//	kPacket.m_wstrPassword		= roomPassword;
//	kPacket.m_bIsObserver		= bIsObserver;
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"방에 들어가는 중입니다", this );
//	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_ROOM_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_JOIN_ROOM_ACK );
//
//	return true;
//}
//
//bool CX2StateArcadeLobby::Handler_EGS_JOIN_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_JOIN_ROOM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	if( g_pMain->DeleteServerPacket( EGS_JOIN_ROOM_ACK ) == true )
//	{
//		
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			//방 데이터 제작
//			CX2DungeonRoom* pCX2DungeonRoom = g_pData->ResetDungeonRoom();
//			pCX2DungeonRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
//			pCX2DungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot, false );
//			pCX2DungeonRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
//
//			pCX2DungeonRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );
//
//			const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)kEvent.m_RoomInfo.m_iDungeonID );
//			if ( pDungeonData != NULL )
//			{
//
//				pCX2DungeonRoom->SetRecommendLevel( 0, D3DXVECTOR2( (float)pDungeonData->m_MinLevel, (float)pDungeonData->m_MaxLevel ) );
//			}
//
//
//			pCX2DungeonRoom->SetMapTextureName( pDungeonData->m_UIData.m_TextureName );
//			pCX2DungeonRoom->SetMapTexturePieceName( pDungeonData->m_UIData.m_PieceName );
//
//			pCX2DungeonRoom->SetDungeonEX( pDungeonData->m_UIData.m_Explanation.c_str() );
//
//			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
//			{
//				if( CX2Main::XS_ARCADE_ROOM == g_pMain->GetStateAutoChanger().GetTargetStateID() )
//				{
//					g_pMain->GetStateAutoChanger().SetTryEnterRoomCount( 0 );
//					g_pMain->GetStateAutoChanger().EndStateChange();
//				}
//			}
//
//			//방으로 스테이트 이동
//			g_pMain->CreateStateChangeDLG( L"아케이드방으로 이동중입니다." );
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_ROOM, NULL, false );	
//
//			return true;
//		}
//		else
//		{
//			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
//			{
//				if( g_pMain->GetStateAutoChanger().GetTryEnterRoomCount() > 1 )
//				{
//					g_pMain->GetStateAutoChanger().SetTryEnterRoomCount( 0 );
//					g_pMain->GetStateAutoChanger().EndStateChange();
//
//					if( NetError::ERR_ROOM_04 == kEvent.m_iOK )
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"비밀번호가 틀립니다.", this );
//					}
//					else
//					{
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"방에 들어갈 수 없습니다.", this );
//					}
//				}
//				else
//				{
//					if( NetError::ERR_ROOM_04 == kEvent.m_iOK )
//					{
//						g_pMain->GetStateAutoChanger().TryEnterArcadeRoom( false );
//					}
//					else
//					{
//						g_pMain->GetStateAutoChanger().SetTryEnterRoomCount( 0 );
//						g_pMain->GetStateAutoChanger().EndStateChange();
//					}
//				}
//			}
//		}
//		
//		
//		return true;
//	}
//
//
//
//	return false;
//}
//
//void CX2StateArcadeLobby::OpenDLGJoinPasswordRoom( UidType roomUID )
//{
//	m_JoinRoomUid = roomUID;
//
//	SAFE_DELETE_DIALOG( m_pDLGJoinPasswordRoom );
//
//	m_pDLGJoinPasswordRoom = new CKTDGUIDialog( this, L"DLG_PVP_Lobby_Password_Check.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGJoinPasswordRoom );
//
//	CKTDGUIButton* pButtonOK = (CKTDGUIButton*)m_pDLGJoinPasswordRoom->GetControl( L"Ok" );
//	if ( pButtonOK != NULL )
//		pButtonOK->SetCustomMsgMouseUp( ALUCM_ROOM_PASSWORD_CHECK_OK );
//
//	CKTDGUIButton* pButtonCancel = (CKTDGUIButton*)m_pDLGJoinPasswordRoom->GetControl( L"Cancel" );
//	if ( pButtonCancel != NULL )
//		pButtonCancel->SetCustomMsgMouseUp( ALUCM_ROOM_PASSWORD_CHECK_CANCEL );
//
//	
//
//	CKTDGUIIMEEditBox* pTempPasswordEditBox = (CKTDGUIIMEEditBox*)m_pDLGJoinPasswordRoom->GetControl(L"EditBoxPassword");
//	if ( pTempPasswordEditBox != NULL )
//	{
//		pTempPasswordEditBox->SetCustomMsgEnter( ALUCM_ROOM_PASSWORD_CHECK_OK );
//		pTempPasswordEditBox->RequestFocus();
//	}
//}
//
//
//bool CX2StateArcadeLobby::Handler_EGS_QUICK_JOIN_REQ()
//{
//
//	KEGS_QUICK_JOIN_REQ kPacket;
//	kPacket.m_iOption = (int)0;
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	g_pData->GetServerProtocol()->SendPacket( EGS_QUICK_JOIN_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_QUICK_JOIN_ACK );
//	
//
//	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"방접속중", this );
//	return true;
//}
//
//bool CX2StateArcadeLobby::Handler_EGS_QUICK_JOIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{	
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_JOIN_ROOM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//	//CX2PacketLog::PrintLog( &kEvent );
//
//	if( g_pMain->DeleteServerPacket( EGS_QUICK_JOIN_ACK ) == true )
//	{
//		if( kEvent.m_iOK == NetError::NET_OK )
//		{
//			SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//
//			//방 데이터 제작
//			CX2DungeonRoom* pCX2DungeonRoom = g_pData->ResetDungeonRoom();
//			pCX2DungeonRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
//			pCX2DungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot, false );
//			pCX2DungeonRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
//
//			pCX2DungeonRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );
//
//			const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)kEvent.m_RoomInfo.m_iDungeonID );
//			if ( pDungeonData != NULL )
//			{
//				
//				pCX2DungeonRoom->SetRecommendLevel( 0, D3DXVECTOR2( (float)pDungeonData->m_MinLevel, (float)pDungeonData->m_MaxLevel ) );
//			}
//
//
//			pCX2DungeonRoom->SetMapTextureName( pDungeonData->m_UIData.m_TextureName );
//			pCX2DungeonRoom->SetMapTexturePieceName( pDungeonData->m_UIData.m_PieceName );
//
//			pCX2DungeonRoom->SetDungeonEX( pDungeonData->m_UIData.m_Explanation.c_str() );
//
//			//방으로 스테이트 이동
//			g_pMain->CreateStateChangeDLG( L"아케이드방으로 이동중입니다." );
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_ROOM, NULL, false );	
//
//			return true;
//		}
//		else
//		{
//			KEGS_CREATE_ROOM_REQ kEGS_CREATE_ROOM_REQ;
//
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomName			= L"한판 더.?";
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic			= true;
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_wstrPassword		= L"";
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_MaxSlot			= 4;
//			if ( g_pData->GetConnectedArcadeChannelInfo() != NULL )
//			{
//				kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID = g_pData->GetConnectedArcadeChannelInfo()->m_iDungeonID;
//			}
//			else
//			{
//				SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"입장조건(레벨/선행던전)이 만족하지 못합니다!", this );
//				return true;
//			}
//					
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_DifficultyLevel	= 0;
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime			= 9999.0f;
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_cGetItemType = CX2DungeonRoom::DGIT_RANDOM;
//
//			g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_ROOM_REQ, kEGS_CREATE_ROOM_REQ );
//			g_pMain->AddServerPacket( EGS_CREATE_ROOM_ACK );
//
//			return true;
//
//		}
//	}
//
//	return false;
//}
//
//bool CX2StateArcadeLobby::Handler_EGS_CREATE_ROOM_REQ()
//{
//#ifdef DISABLE_REDUDANT_PACKET_TEST
//	if( true == g_pMain->IsWaitingServerPacket( EGS_CREATE_ROOM_ACK ) )
//	{
//		return false;
//	}
//#endif DISABLE_REDUDANT_PACKET_TEST
//
//	KEGS_CREATE_ROOM_REQ kEGS_CREATE_ROOM_REQ;
//	CreateRoomPacket( kEGS_CREATE_ROOM_REQ );
//	kEGS_CREATE_ROOM_REQ.m_bObserver = m_bCreateRoomObserver;
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"방을 생성하는 중입니다", this );
//	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_ROOM_REQ, kEGS_CREATE_ROOM_REQ );
//	g_pMain->AddServerPacket( EGS_CREATE_ROOM_ACK );
//
//	return true;
//}
//
//bool CX2StateArcadeLobby::Handler_EGS_CREATE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CREATE_ROOM_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
//
//	
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//
//	if( g_pMain->DeleteServerPacket( EGS_CREATE_ROOM_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			//방 데이터 제작
//			CX2DungeonRoom* pCX2DungeonRoom = g_pData->ResetDungeonRoom();
//			pCX2DungeonRoom->Set_KRoomInfo( kEvent.m_RoomInfo );
//			pCX2DungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot, false );
//			pCX2DungeonRoom->Set_KRoomObserverSlotInfoList( kEvent.m_vecObserverSlot, false );
//			pCX2DungeonRoom->ConnectRelayServer( kEvent.m_RoomInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_RoomInfo.m_usUDPRelayPort );
//
//			pCX2DungeonRoom->SetCenterServerIP( kEvent.m_wstrCNIP.c_str() );
//
//			const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)kEvent.m_RoomInfo.m_iDungeonID );
//			if ( pDungeonData != NULL )
//			{
//
//				pCX2DungeonRoom->SetRecommendLevel( 0, D3DXVECTOR2( (float)pDungeonData->m_MinLevel, (float)pDungeonData->m_MaxLevel ) );
//			}
//
//
//			pCX2DungeonRoom->SetMapTextureName( pDungeonData->m_UIData.m_TextureName );
//			pCX2DungeonRoom->SetMapTexturePieceName( pDungeonData->m_UIData.m_PieceName );
//
//			pCX2DungeonRoom->SetDungeonEX( pDungeonData->m_UIData.m_Explanation.c_str() );
//
//			//방으로 스테이트 이동
//			g_pMain->CreateStateChangeDLG( L"아케이드방으로 이동중입니다." );
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_ROOM, NULL, false );
//
//			return true;
//		}
//	}
//
//
//
//	return true;
//}
//
//
//
//bool CX2StateArcadeLobby::Handler_EGS_CHANGE_ARCADE_CHANNEL_REQ( UINT channelID )
//{
//	if( g_pData->GetAracdeChannelData().m_nChannelID == channelID )
//		return true;
//
//	KArcadeChannelInfo* pChannelInfo = g_pData->GetArcadeChannelInfo( channelID );
//	if ( pChannelInfo != NULL )
//	{
//		const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)pChannelInfo->m_iDungeonID );
//		if ( pDungeonData != NULL )
//		{
//			if ( g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_OPERATOR )
//			{
//				if ( pDungeonData->m_MinLevel > g_pData->GetMyUser()->GetSelectUnit()->GetUnitData()->m_Level )
//				{	
//					wstringstream wstrstm;
//					wstrstm << pDungeonData->m_MinLevel << L"레벨 이상만 입장할 수 있습니다.";
//					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );
//					ResetChannelRadioButtonUI( g_pData->GetAracdeChannelData().m_nChannelID );
//					return true;
//				}
//
//				if ( g_pData->GetMyUser()->GetSelectUnit()->IsClearDungeon( pDungeonData->m_RequireDungeonID ) == false )
//				{
//					const CX2Dungeon::DungeonData* pRequireDungeon = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)pDungeonData->m_RequireDungeonID );
//					if ( pRequireDungeon != NULL )
//					{
//						wstringstream wstrstm;
//						wstrstm << pRequireDungeon->m_DungeonName.c_str();
//						if ( pRequireDungeon->m_eDifficulty == CX2Dungeon::DL_NORMAL )
//						{
//							wstrstm << L"[보통]";
//						}
//						else if ( pRequireDungeon->m_eDifficulty == CX2Dungeon::DL_HARD )
//						{
//							wstrstm << L"[어려움]";
//						}
//						else
//						{
//							wstrstm << L"[매우 어려움]";
//						}	
//						wstrstm << L"을 먼저 깨셔야 입장할 수 있습니다.";
//						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrstm.str().c_str(), this );
//						ResetChannelRadioButtonUI( g_pData->GetAracdeChannelData().m_nChannelID );
//						return true;
//					}
//
//				}
//			}
//
//			
//		}
//		
//	}
//
//	m_TempChannelID = channelID;
//
//	KEGS_CHANGE_ARCADE_CHANNEL_REQ kPacket;
//	kPacket = channelID;
//
//	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_ARCADE_CHANNEL_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_CHANGE_ARCADE_CHANNEL_ACK );
//
//	return true;
//}
//
//
//bool CX2StateArcadeLobby::Handler_EGS_CHANGE_ARCADE_CHANNEL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_CHANGE_ARCADE_CHANNEL_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	if( g_pMain->DeleteServerPacket( EGS_CHANGE_ARCADE_CHANNEL_ACK ) == true )
//	{
//		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
//		{
//			g_pData->GetAracdeChannelData().m_nChannelID = m_TempChannelID;
//			g_pData->GetAracdeChannelData().m_kTopRankingInfo = kEvent.m_kTopRankingInfo;
//			g_pData->GetAracdeChannelData().m_vecTodoyRankingInfo = kEvent.m_vecTodoyRankingInfo;
//
//			ResetRankingUI();
//
//			ResetChannelRadioButtonUI( m_TempChannelID );
//			m_NowPage = 1;
//			Handler_EGS_ROOM_LIST_REQ( m_NowPage );
//
//
//			/*
//			//{{ dmlee 2008.7.13 play channel
//			{
//				CKTDGUIButton* pButton_TeamMatch	= (CKTDGUIButton*)m_pDLGPVPLobbyFront->GetControl( L"ButtonPVP_Lobby_TeamButton" );
//				CKTDGUIRadioButton* pRadioButton_TeamMatch = (CKTDGUIRadioButton*)m_pDLGPVPLobbyFront->GetControl( L"RadioButtonTeam_Button_Button" );
//				CKTDGUIStatic* pStatic_Inactive = (CKTDGUIStatic*) m_pDLGPVPLobbyBack->GetControl( L"Inactive_Controls" );
//
//				if( KPVPChannelInfo::PCC_PLAY == g_pMain->GetPVPChannelClass( g_pMain->GetConnectedChannelID() ) )
//				{
//					pButton_TeamMatch->SetShowEnable( false, false );
//					pRadioButton_TeamMatch->SetShowEnable( false, false );
//					pStatic_Inactive->SetShowEnable( true, true );
//				}
//				else 
//				{
//					pButton_TeamMatch->SetShowEnable( true, true );
//					pRadioButton_TeamMatch->SetShowEnable( true, true );
//					pStatic_Inactive->SetShowEnable( false, false );
//				}
//			}
//			//}} dmlee 2008.7.13 play channel
//			*/
//
//
//			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
//			{
//				g_pMain->GetStateAutoChanger().DoStateChange( (int)g_pMain->GetNowStateID() );
//			}
//		}
//		else
//		{
//			ResetChannelRadioButtonUI( g_pData->GetAracdeChannelData().m_nChannelID );
//			//고쳐야해 인마럼니아러민ㅇ라ㅓㅁ니아럼니ㅏㅇ러민아ㅓ린아ㅓㄹ니ㅏㅇ러나ㅣ어림ㄴ어리만어리마넝리ㅏㅁ넝리ㅏㅁ넝리ㅏㅓ
//
//			if( true == g_pMain->GetStateAutoChanger().GetIsAutoChangingState() )
//			{
//				g_pMain->GetStateAutoChanger().EndStateChange();
//			}
//		}
//		return true;
//	}
//	return false;
//
//}
//
//void CX2StateArcadeLobby::ResetChannelRadioButtonUI( UINT selectedChannelID )
//{
//	if ( m_pDLGArcadeLobbyBack == NULL )
//		return;
//
//	const int maxSize = 3;
//
//	for ( int i = 0; i < maxSize; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pRadioButtonArcade_Lobby_Tab" << i;
//		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDLGArcadeLobbyBack->GetControl( wstrstm.str().c_str() );
//		if ( pRadioButton != NULL )
//		{
//			KArcadeChannelInfo* pChannelInfo = g_pData->GetArcadeChannelInfo( pRadioButton->GetDummyInt(0) );
//			if ( pChannelInfo != NULL )
//			{
//				const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( (SEnum::DUNGEON_ID)pChannelInfo->m_iDungeonID );
//				if( pDungeonData != NULL )
//				{
//					wstringstream wstrstm;
//					wstrstm << L"제한 레벨 : " << pDungeonData->m_MinLevel << L", 선행 던전 : ";
//					const CX2Dungeon::DungeonData* pRequireDungeonData = g_pData->GetDungeonManager()->GetDungeonData( pDungeonData->m_RequireDungeonID );
//					if ( pRequireDungeonData != NULL )
//					{
//						wstrstm << g_pData->GetDungeonManager()->MakeDungeonNameString( pDungeonData->m_RequireDungeonID, pRequireDungeonData->m_eDifficulty );
//						pRadioButton->SetGuideDesc( wstrstm.str().c_str() );
//					}				
//				}
//			}
//			
//			
//			pRadioButton->SetChecked( false );
//		}
//	}
//
//	for ( int i = 0; i < maxSize; i++ )
//	{
//		wstringstream wstrstm;
//		wstrstm << L"g_pRadioButtonArcade_Lobby_Tab" << i;
//		CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)m_pDLGArcadeLobbyBack->GetControl( wstrstm.str().c_str() );
//		if ( pRadioButton != NULL && pRadioButton->GetDummyInt(0) == selectedChannelID )
//		{
//			pRadioButton->SetChecked( true );
//			break;
//		}
//	}
//	
//}
//
//void CX2StateArcadeLobby::CreateRoomPacket( KEGS_CREATE_ROOM_REQ& kEGS_CREATE_ROOM_REQ )
//{
//	if ( m_pDLGArcadeCreateRoom == NULL )
//		return;
//
//	CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)m_pDLGArcadeCreateRoom->GetControl( L"IME_Arcade_Room_Name" );
//	if ( pIMEEdit != NULL )
//	{
//		wstring tempName = pIMEEdit->GetText();
//		kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_RoomName			= tempName.substr( 0, 20 );
//	}
//
//	CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGArcadeCreateRoom->GetControl( L"g_pCheckBoxArceade_Create_Room" );
//	if ( pCheckBox != NULL )
//	{
//		kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_bPublic			= !pCheckBox->GetChecked();
//
//		CKTDGUIIMEEditBox* pIMEEdit = (CKTDGUIIMEEditBox*)m_pDLGArcadeCreateRoom->GetControl( L"IME_Arcade_Room_Password" );
//		if ( pIMEEdit != NULL )
//		{
//			wstring tempPassword = pIMEEdit->GetText();
//			kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_wstrPassword = tempPassword.substr( 0, 20 );
//		}
//
//		
//
//
//	}
//	
//
//	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_MaxSlot			= 4;
//	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_DifficultyLevel	= (CX2Dungeon::DL_NORMAL);
//	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_fPlayTime			= 9999.0f;
//
//
//	if ( g_pData->GetConnectedArcadeChannelInfo() != NULL )
//		kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_iDungeonID	= (SEnum::DUNGEON_ID)g_pData->GetConnectedArcadeChannelInfo()->m_iDungeonID;
//
//	kEGS_CREATE_ROOM_REQ.m_RoomInfo.m_cGetItemType = CX2DungeonRoom::DGIT_RANDOM;
//}
//
//
//
//
//
//
//bool CX2StateArcadeLobby::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ()
//{
//	KStateChangeReq kPacket;
//	kPacket.m_nMapID = g_pMain->GetPrevVillageStateID();
//
//	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
//	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_VILLAGE_MAP_REQ, kPacket );
//	g_pMain->AddServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK );
//	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"마을맵으로 이동하고 있습니다.", this );
//
//	return true;
//}
//
//
//bool CX2StateArcadeLobby::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{	
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KStateChangeAck kEvent;
//	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK ) == true )
//	{
//		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
//		{
//			g_pMain->CreateStateChangeDLG( L"마을맵으로 이동중입니다." );
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, kEvent.m_nMapID, false );
//			return true;
//		}
//	}
//
//	return true;
//}
//
//bool CX2StateArcadeLobby::Handler_EGS_GET_RANKING_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;	
//	KEGS_GET_RANKING_INFO_ACK kEvent;
//	DeSerialize( pBuff, &kEvent );
//
//	//if( g_pMain->DeleteServerPacket( EGS_GET_RANKING_INFO_ACK ) == true )
//
//	g_pData->GetAracdeChannelData().m_kTopRankingInfo = kEvent.m_kTopRankingInfo;
//	g_pData->GetAracdeChannelData().m_vecTodoyRankingInfo = kEvent.m_vecTodayRankingInfo;
//
//	ResetRankingUI();
//
//	return true;
//}
//
//void CX2StateArcadeLobby::ResetRankingUI()
//{
//	if ( m_pDLGArcadeLobbyBack == NULL )
//		return;
//
//	CKTDGUIStatic* pStatic_Ranking_Server_1st = (CKTDGUIStatic*)m_pDLGArcadeLobbyBack->GetControl(L"Ranking_Server_1st");
//	if ( pStatic_Ranking_Server_1st != NULL )
//	{
//		int topRankerNum = (int)g_pData->GetAracdeChannelData().m_kTopRankingInfo.m_vecRankerList.size();
//		if ( topRankerNum > 0 )
//		{
//			wstring hostName = g_pData->GetAracdeChannelData().m_kTopRankingInfo.m_vecRankerList[0].m_wstrNickName;
//			if ( topRankerNum > 1 )
//			{
//				wstringstream wstrstm;
//				wstrstm << L" (+" << topRankerNum - 1 << L")";
//				hostName += wstrstm.str().c_str();
//			}
//			pStatic_Ranking_Server_1st->SetString( 0, hostName.c_str() );
//
//			wstringstream wstrstmNames;
//			for ( int i = 0; i < (int)g_pData->GetAracdeChannelData().m_kTopRankingInfo.m_vecRankerList.size(); i++ )
//			{
//				wstrstmNames << g_pData->GetAracdeChannelData().m_kTopRankingInfo.m_vecRankerList[i].m_wstrNickName;
//				if ( i != (int)g_pData->GetAracdeChannelData().m_kTopRankingInfo.m_vecRankerList.size() - 1 )
//				{
//					wstrstmNames << L", ";
//				}
//			}
//
//			CKTDGUIButton* pButtonTopRankingAllUser = (CKTDGUIButton*)m_pDLGArcadeLobbyBack->GetControl( L"TopRankingAllUser" );
//			if ( pButtonTopRankingAllUser != NULL )
//			{
//				pButtonTopRankingAllUser->SetGuideDesc( wstrstmNames.str().c_str() );
//			}
//		}
//		else
//		{
//			pStatic_Ranking_Server_1st->SetString( 0, L"-" );
//
//			CKTDGUIButton* pButtonTopRankingAllUser = (CKTDGUIButton*)m_pDLGArcadeLobbyBack->GetControl( L"TopRankingAllUser" );
//			if ( pButtonTopRankingAllUser != NULL )
//			{
//				pButtonTopRankingAllUser->SetGuideDesc( L"-" );
//			}
//		}
//	}
//
//	CKTDGUIStatic* pStatic_Ranking_Today = (CKTDGUIStatic*)m_pDLGArcadeLobbyBack->GetControl( L"Ranking_Today" );
//
//	for ( int i = 0; i < 5; i++ )
//	{
//		pStatic_Ranking_Today->SetString( i, L"-" );
//
//		wstringstream wstrstmTodayRank;
//		wstrstmTodayRank << L"TodayRankingAllUser" << i;
//
//		CKTDGUIButton* pButtonTodayRankingAllUser = (CKTDGUIButton*)m_pDLGArcadeLobbyBack->GetControl( wstrstmTodayRank.str().c_str() );
//		if ( pButtonTodayRankingAllUser != NULL )
//		{
//			pButtonTodayRankingAllUser->SetGuideDesc( L"-" );
//		}
//	}
//
//	
//	
//
//	if ( pStatic_Ranking_Today != NULL )
//	{
//		for ( int i = 0; i < (int)g_pData->GetAracdeChannelData().m_vecTodoyRankingInfo.size(); i++ )
//		{
//			KRankingInfo& rankingInfo = g_pData->GetAracdeChannelData().m_vecTodoyRankingInfo[i];
//
//			int topRankerNum = (int)rankingInfo.m_vecRankerList.size();
//			if ( topRankerNum > 0 )
//			{
//				wstring hostName = rankingInfo.m_vecRankerList[0].m_wstrNickName;
//				if ( topRankerNum > 1 )
//				{
//					wstringstream wstrstm;
//					wstrstm << L" (+" << topRankerNum - 1  << L")";
//					hostName += wstrstm.str().c_str();
//				}
//				pStatic_Ranking_Today->SetString( i, hostName.c_str() );
//
//				wstringstream wstrstmNames;
//				for ( int j = 0; j < (int)rankingInfo.m_vecRankerList.size(); j++ )
//				{
//					wstrstmNames << rankingInfo.m_vecRankerList[j].m_wstrNickName;
//					if ( j != (int)rankingInfo.m_vecRankerList.size() - 1 )
//					{
//						wstrstmNames << L", ";
//					}
//				}
//
//				wstringstream wstrstmTodayRank;
//				wstrstmTodayRank << L"TodayRankingAllUser" << i;
//
//				CKTDGUIButton* pButtonTodayRankingAllUser = (CKTDGUIButton*)m_pDLGArcadeLobbyBack->GetControl( wstrstmTodayRank.str().c_str() );
//				if ( pButtonTodayRankingAllUser != NULL )
//				{
//					pButtonTodayRankingAllUser->SetGuideDesc( wstrstmNames.str().c_str() );
//				}
//			}
//		}
//	}
//
//	
//}
//
//void CX2StateArcadeLobby::DrawPicChar( float fElapsedTime )
//{
//	if ( m_pPicCharTodayRankStage != NULL )
//		m_pPicCharTodayRankStage->OnFrameMove( fElapsedTime );
//
//	if ( m_pPicCharTodayRankTime != NULL )
//		m_pPicCharTodayRankTime->OnFrameMove( fElapsedTime );
//
//	if ( m_pPicCharTodayRankStageSmall != NULL )
//		m_pPicCharTodayRankStageSmall->OnFrameMove( fElapsedTime );
//
//	if ( m_pPicCharTodayRankTimeSmall != NULL )
//		m_pPicCharTodayRankTimeSmall->OnFrameMove( fElapsedTime );
//
//	if ( m_pPicCharTodayRankStage != NULL && m_pPicCharTodayRankTime != NULL &&
//		 m_pPicCharTodayRankStageSmall != NULL && m_pPicCharTodayRankTimeSmall != NULL )
//	{
//		m_pPicCharTodayRankStage->Clear();
//		m_pPicCharTodayRankTime->Clear();
//
//		m_pPicCharTodayRankStageSmall->Clear();
//		m_pPicCharTodayRankTimeSmall->Clear();
//
//		// 788 373 65
//
//		if ( m_bEnableResetDLGRoomList == true )
//		{
//			for ( int i = 0; i < (int)g_pData->GetAracdeChannelData().m_vecTodoyRankingInfo.size(); i++ )
//			{
//				KRankingInfo& kKRankingInfo = g_pData->GetAracdeChannelData().m_vecTodoyRankingInfo[i];
//				m_pPicCharTodayRankStageSmall->DrawText( kKRankingInfo.m_iStageCount, D3DXVECTOR3( 788 + 11, 373.0f + (i*66), 0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
//
//				m_pPicCharTodayRankTimeSmall->DrawText( GetRecordTimeEx( kKRankingInfo.m_ulPlayTime ).c_str(), 
//					D3DXVECTOR3( 922 + 11, 373.0f + (i*66), 0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
//			}
//
//
//			KRankingInfo& kKTopRankingInfo = g_pData->GetAracdeChannelData().m_kTopRankingInfo;
//			m_pPicCharTodayRankStage->DrawText( kKTopRankingInfo.m_iStageCount, D3DXVECTOR3( 788 + 11, 194, 0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
//
//			m_pPicCharTodayRankTime->DrawText( GetRecordTimeEx( kKTopRankingInfo.m_ulPlayTime ).c_str(), 
//				D3DXVECTOR3( 921 + 11, 194, 0 ), D3DXVECTOR3(1,0,0), CKTDGPicChar::AT_CENTER, 0.00f );
//		}
//
//		
//		
//	}
//}
//
//
//wstring CX2StateArcadeLobby::GetRecordTime( int iPlayTime )
//{
//	float fTime = iPlayTime/100.0f;
//
//	//int iHour	= (int)(fTime / 3600.f);
//	int iMinute = (int)( fTime / 60.f  );
//	int iSecond = (int)(fTime-(float)iMinute*60.f );
//	float fMilli = fTime - (int)fTime;
//	fMilli = fMilli * 100;
//
//	wstring recordTime;
//	wstringstream wstrstm;
//	wstrstm << iMinute << L"=" << iSecond << L"=" << (int)fMilli;
//	recordTime = wstrstm.str().c_str();
//	return recordTime;
//}
//
//wstring CX2StateArcadeLobby::GetRecordTimeEx( int iPlayTime )
//{
//	float fTime = iPlayTime/100.0f;
//
//	//int iHour	= (int)(fTime / 3600.f);
//	int iMinute = (int)( fTime / 60.f  );
//	int iSecond = (int)(fTime-(float)iMinute*60.f );
//	float fMilli = fTime - (int)fTime;
//	fMilli = fMilli * 100;
//
//	wstring recordTime;
//	wstringstream wstrstm;
//	wstrstm << iMinute << L";" << iSecond << L";" << (int)fMilli;
//	recordTime = wstrstm.str().c_str();
//	return recordTime;
//}

#include "StdAfx.h"

#include ".\x2statepvpresult.h"

CX2StatePVPResult::CX2StatePVPResult(void) :
// 2009.01.09 김태완 : 초기화 코드정리
m_pDLGBack(NULL),
m_pDLGFront(NULL),
#ifdef SERV_PVP_REMATCH
m_pDLGRematch( NULL ),
m_bCanRematch( false ),
#endif SERV_PVP_REMATCH
//m_pDLGResultInfoList;
//m_pDLGResultCardBackList;
//m_pDLGResultCardFrontList;
m_pDLGFrontFreeChannel(NULL),
m_bInitUIFail(false),
m_fPlayBGM(0.5f),
#ifdef SERV_PVP_REMATCH
m_fRemainTime(7.0f),
#else
m_fRemainTime(5.0f),
#endif
m_bSendRefreshRoomReq(false)
{
	// 초기화 //////////////////////////////////////////////////////////////////////////
// 
// 	m_pDLGBack		= NULL;
// 	m_pDLGFront		= NULL;
// 	m_bInitUIFail	= false;
// 	m_pDLGFrontFreeChannel = NULL;
// 	m_fPlayBGM = 0.5f;
// 	m_bSendRefreshRoomReq = false;
// 	m_fRemainTime = 5.0f; 

	m_pDLGResultInfoList.clear();
	m_pDLGResultCardBackList.clear();
	m_pDLGResultCardFrontList.clear();

	//////////////////////////////////////////////////////////////////////////

	DialogLog( "\nCX2StatePVPResult::CX2StatePVPResult Start\n" );



	
	if ( g_pKTDXApp->GetDSManager()->GetInit() == true && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL )
		g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( L"MusicWin.ogg" );

	if ( InitUI() == false )
	{
		m_bInitUIFail = true;
		m_bSendRefreshRoomReq = true;
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300), GET_STRING( STR_ID_693 ), this );
		ErrorLogMsg( XEM_ERROR35, "PVP결과창 초기화 실패!" );
		//Handler_EGS_REFRESH_ROOM_REQ();
		Handler_EGS_RESULT_SUCCESS_REQ();
	}

	
	
	wstringstream buf;
	if ( g_pMain->GetDungeonRewardItem().empty() == false )
	{
		int nCount = 0;
		wstring getExplanation = L"";

		
		for( UINT i=0; i<g_pMain->GetDungeonRewardItem().size(); i++ )
		{
			const KItemInfo& itemInfo = g_pMain->GetDungeonRewardItem().at(i);
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemInfo.m_iItemID );
			if ( pItemTemplet != NULL )
			{
				if ( nCount > 0 )
				{
					getExplanation += L", ";
				}

				buf.str( L"" );
                buf << pItemTemplet->GetFullName_();
				
				getExplanation += buf.str().c_str();

				nCount++;
			}
		}


        wstring wstrExplanation = GET_REPLACED_STRING( ( STR_ID_694, "LI", getExplanation, KHanSungPostWordUnicodeWrapper( (WCHAR*)getExplanation.c_str(), STR_ID_198, STR_ID_199 ) ) );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrExplanation.c_str(), this );
	}
	

	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1300, 0,0,0 );
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
		g_pKTDXApp->GetDGManager()->GetFar(), false );

	m_bEnableQuitGame = false;

	g_pKTDXApp->SkipFrame();

#ifdef REFORM_UI_SKILLSLOT
	if( NULL != g_pData && 
		NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->GetDLGGameSkillSLot()->SetShow(false);
	}
#endif //REFORM_UI_SKILLSLOT

	DialogLog( "\nCX2StatePVPResult::CX2StatePVPResult End\n" );
}

CX2StatePVPResult::~CX2StatePVPResult(void)
{
	DialogLog( "\nCX2StatePVPResult::~CX2StatePVPResult Start\n" );

	SAFE_DELETE_DIALOG( m_pDLGBack );
	SAFE_DELETE_DIALOG( m_pDLGFront );
#ifdef SERV_PVP_REMATCH
	SAFE_DELETE_DIALOG( m_pDLGRematch );
#endif SERV_PVP_REMATCH

	for ( int i = 0; i < (int)m_pDLGResultInfoList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_pDLGResultInfoList[i];
		SAFE_DELETE_DIALOG( pDialog );
	}
	m_pDLGResultInfoList.clear();

	for ( int i = 0; i < (int)m_pDLGResultCardBackList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_pDLGResultCardBackList[i];
		SAFE_DELETE_DIALOG( pDialog );
	}
	m_pDLGResultCardBackList.clear();

	for ( int i = 0; i < (int)m_pDLGResultCardFrontList.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_pDLGResultCardFrontList[i];
		SAFE_DELETE_DIALOG( pDialog );
	}
	m_pDLGResultCardFrontList.clear();

	CX2PVPRoom* pPvpRoom = g_pData->GetPVPRoom();
	if ( NULL != pPvpRoom )
	{
		for ( int i = 0; i < (int)pPvpRoom->GetSlotNum(); i++ )
		{
			CX2Room::SlotData* pSlotData = pPvpRoom->GetSlotData( i ); 

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
				continue;

			pSlotData->m_pUnit->SetIsLevelUp( false );
		}
	}	

	SAFE_DELETE_DIALOG( m_pDLGFrontFreeChannel );

	g_pData->DeletePVPResultInfo();

	g_pData->PlayLobbyBGM( L"Lobby.ogg", true );

	DialogLog( "\nCX2StatePVPResult::~CX2StatePVPResult End\n" );
}


HRESULT CX2StatePVPResult::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( m_bInitUIFail == true )
		return S_OK;

	CX2StateCommonBG::OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	m_fRemainTime -= fElapsedTime;

#ifdef SERV_PVP_REMATCH
	if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL && m_bCanRematch == true )
	{
		int iCount = (int)( m_fRemainTime - 1.01f );

		if( iCount >= 1 )
		{
			// 예전 카운트만 지우는 코드-굳이 다음 카운트 것도 지울 필요는 없다.
			for( int iPreviousCountIndex = 5; iPreviousCountIndex > iCount; --iPreviousCountIndex )
			{
				std::wstringstream wstrstmCount;
				wstrstmCount << L"Count" << iPreviousCountIndex;
				m_pDLGRematch->GetControl( wstrstmCount.str().c_str() )->SetShowEnable( false, false );
			}

			std::wstringstream wstrstmCount;
			wstrstmCount << L"Count" << iCount;
			m_pDLGRematch->GetControl( wstrstmCount.str().c_str() )->SetShowEnable( true, false );
		}
		else
		{
			m_pDLGRematch->GetControl( L"Popup" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"RematchAsk1" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"RematchAsk2" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Count1" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Count2" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Count3" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Yes" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"No" )->SetShowEnable( false, false );
		}
	}
#endif SERV_PVP_REMATCH

	if ( m_fRemainTime <= 0.0f && m_bSendRefreshRoomReq == false )
	{
		m_bSendRefreshRoomReq = true;
		if ( g_pX2Room->GetBan() == true )
		{
#ifdef SERV_PVP_NEW_SYSTEM
			if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
			{				
				CX2State* pState = (CX2State*)g_pMain->GetNowState();
				if( pState != NULL )
					pState->ReturnToPlaceWhereBeforeDungeonStart();
			}
			else
			{
				g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_438 ) );
				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );
			}
#else
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_438 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );
#endif
		}
		else
		{
			//Handler_EGS_REFRESH_ROOM_REQ();
			Handler_EGS_RESULT_SUCCESS_REQ();
		}
	}

	if ( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || 
		g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
	{
		CX2PVPGame::PVPResultInfo* pPVPResultInfo = g_pData->GetPVPResultInfo();
		int winTeam = pPVPResultInfo->m_WinTeam;
		int winTeamNum = 0;
		int loseTeamNum = 4;
#ifdef SERV_PVP_NEW_SYSTEM
		int iMyTeamNum = 0;
#endif

		if( true == pPVPResultInfo->m_bIsDrawn )
		{
			winTeam = (int)CX2Room::TN_RED;
		}

		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pPVPResultUnitInfo->m_UnitUid );

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
				continue;

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				continue;

#ifdef SERV_PVP_NEW_SYSTEM
			if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
			{
				if( g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum == pSlotData->m_TeamNum )
				{
					if( pSlotData->m_pUnitViewer != NULL )
						UnitViewerProcess( pSlotData->m_pUnitViewer, iMyTeamNum++ );
				}
			}
			else
			{
				if( winTeam == pSlotData->m_TeamNum )
				{
					if( pSlotData->m_pUnitViewer != NULL )
						UnitViewerProcess( pSlotData->m_pUnitViewer, winTeamNum++ );
				}
				else
				{
					if( pSlotData->m_pUnitViewer != NULL )
						UnitViewerProcess( pSlotData->m_pUnitViewer, loseTeamNum++ );
				}
			}
			
#else
			if( winTeam == pSlotData->m_TeamNum )
			{
				if( pSlotData->m_pUnitViewer != NULL )
					UnitViewerProcess( pSlotData->m_pUnitViewer, winTeamNum++ );
			}
			else
			{
				if( pSlotData->m_pUnitViewer != NULL )
					UnitViewerProcess( pSlotData->m_pUnitViewer, loseTeamNum++ );
			}
#endif
		}	
	}
	else
	{
		CX2PVPGame::PVPResultInfo* pPVPResultInfo = g_pData->GetPVPResultInfo();
		int winTeam = pPVPResultInfo->m_WinTeam;
		int winTeamNum = 0;
		int loseTeamNum = 4;

		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
			if( i >= 4 )
				break;

			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pPVPResultUnitInfo->m_UnitUid );

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
				continue;

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				continue;

			if( pSlotData->m_pUnitViewer != NULL )
				UnitViewerProcess( pSlotData->m_pUnitViewer, i );
		}	
	}

	if( m_fPlayBGM > 0.0f  )
	{
		m_fPlayBGM -= m_fElapsedTime;
		if( m_fPlayBGM < 0.0f )
		{
			if ( g_pKTDXApp->GetDSManager()->GetInit() == true && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL )
				g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( L"MusicWin.ogg", false );

			m_fPlayBGM = 0.0f;
		}
	}

	return S_OK;
}

HRESULT CX2StatePVPResult::OnFrameRender()
{
	if ( m_bInitUIFail == true )
		return S_OK;

	CX2State::OnFrameRender();

	return S_OK;
}


bool CX2StatePVPResult::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( CX2StateCommonBG::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	switch( wParam )
	{
	//case EGS_REFRESH_ROOM_ACK:
	//	{
	//		return Handler_EGS_REFRESH_ROOM_ACK( hWnd, uMsg, wParam, lParam );
	//	}
	//	break;

	case EGS_RESULT_SUCCESS_ACK:
		{
			return Handler_EGS_RESULT_SUCCESS_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_RESULT_ALL_UNIT_SUCCESS_NOT:
		{
			return Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_LEAVE_ROOM_NOT:
		{
			return Handler_EGS_LEAVE_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_BAN_USER_NOT:
		{
			g_pX2Room->SetBan( true );
			return true;
		}
		break;

	case EGS_JOIN_ROOM_NOT:
		{
			return Handler_EGS_JOIN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;

	case EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT:
		{
			return Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#ifdef SERV_PVP_REMATCH
	case EGS_PVP_REMATCH_ACK:
		{
			return Handler_EGS_PVP_REMATCH_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
	case EGS_PVP_REMATCH_NOT:
		{
			return Handler_EGS_PVP_REMATCH_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif SERV_PVP_REMATCH
	}

	return false;
}

#ifdef SERV_PVP_REMATCH
bool CX2StatePVPResult::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case PRUCM_REMATCH_ACCEPT:
		{
			Handler_EGS_PVP_REMATCH_REQ( true );
			return true;
		}
		break;
	case PRUCM_REMATCH_REJECT:
		{
			Handler_EGS_PVP_REMATCH_REQ( false );
			return true;
		}
		break;
	}

	if ( CX2StateOption::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	return false;
}
#endif SERV_PVP_REMATCH

/** @function : Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT
	@brief : 같은 Room(개념으로써의)내에 있는 유저중에서 장비를 변경 한 경우에 처리하는 패킷
	@param : HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
	@return : 패킷 처리에 성공한 경우 true, 실패한 경웨 false
*/
bool CX2StatePVPResult::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pX2Room )
		return g_pX2Room->Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( kEvent );
	else
		return false;
}

bool CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_REQ()
{
	DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_REQ Start" );

	KEGS_RESULT_SUCCESS_REQ kPacket;

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	// 맵에 가지고 있는 GamePlayNetworkInfo 를 vector에 담는다.
	if ( NULL != g_pX2Room )
		g_pX2Room->MoveGamePlayNetworkInfoFromMapToVector( kPacket.m_kGamePlayNetworkInfo.m_vecGamePlayNetworkInfo );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

	g_pData->GetServerProtocol()->SendPacket( EGS_RESULT_SUCCESS_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESULT_SUCCESS_ACK );

	DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_REQ End" );

	return true;
}

bool CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_ACK Start" );

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESULT_SUCCESS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if( g_pMain->DeleteServerPacket( EGS_RESULT_SUCCESS_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_SUCCESS_ACK End" );

			return true;
		}
	}
	return false;
}

bool CX2StatePVPResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT Start" );

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	g_pX2Room->Set_KRoomInfo( kEvent.m_RoomInfo );
	g_pX2Room->Set_KRoomSlotInfoList( kEvent.m_vecSlot );

#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
	{				
		CX2State* pState = (CX2State*)g_pMain->GetNowState();
		if( pState != NULL )
			pState->ReturnToPlaceWhereBeforeDungeonStart();
	}
	else
	{
		g_pMain->CreateStateChangeDLG( GET_STRING(STR_ID_438  ) );
		DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false" );
		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );		
		DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT End" );
	}
#else
	DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false" );
	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_ROOM, NULL, false );		
	DialogLog( "CX2StatePVPResult::Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT End" );
#endif

	return true;
}

bool CX2StatePVPResult::Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LEAVE_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	return g_pX2Room->Handler_EGS_LEAVE_ROOM_NOT( kEvent );
}

bool CX2StatePVPResult::Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_JOIN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	if ( g_pX2Room != NULL )
		g_pX2Room->Handler_EGS_JOIN_ROOM_NOT( kEvent );

	return false;
}

#ifdef SERV_PVP_REMATCH
bool CX2StatePVPResult::Handler_EGS_PVP_REMATCH_REQ( bool bAcceptRematch )
{
	KEGS_PVP_REMATCH_REQ kPacket;
	kPacket.m_bAcceptRematch = bAcceptRematch;

	g_pData->GetServerProtocol()->SendPacket( EGS_PVP_REMATCH_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_PVP_REMATCH_ACK, 60.f );

	return true;
}

bool CX2StatePVPResult::Handler_EGS_PVP_REMATCH_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_REMATCH_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->DeleteServerPacket( EGS_PVP_REMATCH_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// 메세지창 지우기
			m_bCanRematch = false;
			m_pDLGRematch->GetControl( L"Popup" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"RematchAsk1" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"RematchAsk2" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Count1" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Count2" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Count3" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"Yes" )->SetShowEnable( false, false );
			m_pDLGRematch->GetControl( L"No" )->SetShowEnable( false, false );

			return true;
		}
	}
	return false;
}
bool CX2StatePVPResult::Handler_EGS_PVP_REMATCH_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PVP_REMATCH_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	// 다른 유저의 선택UI 부분에 표시를 해 주자.
	std::wstringstream wstrstmControlName;
	if( kEvent.m_bAcceptRematch == true )
		wstrstmControlName << L"Yes";
	else
		wstrstmControlName << L"No";

	int iSlotIndex = m_mapSlotIndexByUnitUID[ kEvent.m_iUnitUID ];
	wstrstmControlName << iSlotIndex;

	CKTDGUIStatic* pStaticYesNo = (CKTDGUIStatic*)m_pDLGRematch->GetControl( wstrstmControlName.str().c_str() );
	pStaticYesNo->SetShowEnable( true, false );

	return false;
}
#endif SERV_PVP_REMATCH

bool CX2StatePVPResult::InitUI()
{
	DialogLog( "CX2StatePVPResult::InitUI() Start" );

#ifdef SERV_PVP_NEW_SYSTEM
	bool bMatchPvp = false;
	if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
	{
		bMatchPvp = true;
	}
#endif

#ifdef SERV_PVP_NEW_SYSTEM
	if( bMatchPvp == true )
		m_pDLGBack = new CKTDGUIDialog( this, L"DLG_PVP_Result_Back_MatchPvp.lua" );
	else
		m_pDLGBack = new CKTDGUIDialog( this, L"DLG_PVP_Result_Back_practice.lua" );
#else
	m_pDLGBack = new CKTDGUIDialog( this, L"DLG_PVP_Result_Back.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg(m_pDLGBack);
	if ( m_pDLGBack->GetIsSuccessInitOpenScript() == false )
	{
		ErrorLog(XEM_ERROR36);
		return false;
	}

#ifdef SERV_PVP_NEW_SYSTEM
	if( bMatchPvp == true )
		m_pDLGFront = new CKTDGUIDialog( this, L"DLG_PVP_Result_Front_MatchPvp.lua" );
	else
		m_pDLGFront = new CKTDGUIDialog( this, L"DLG_PVP_Result_Front.lua" );
#else
	m_pDLGFront = new CKTDGUIDialog( this, L"DLG_PVP_Result_Front.lua" );
#endif
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg(m_pDLGFront);
	if ( m_pDLGFront->GetIsSuccessInitOpenScript() == false )
	{
		ErrorLog(XEM_ERROR37);
		return false;
	}

#ifdef SERV_PVP_REMATCH
	if( bMatchPvp == true )
		m_pDLGRematch = new CKTDGUIDialog( this, L"DLG_PVP_Rematch.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGRematch );
#endif SERV_PVP_REMATCH

	D3DXCOLOR myColor = D3DXCOLOR(1,1,0,1);

	const float X_OFFSET = 141.0f;
	const float Y_OFFSET = 482.0f; //원래는 484여야지 맞다
	const float MAGIC_HEIGHT = 32;


	if ( g_pData->GetPVPRoom() == NULL || g_pX2Room == NULL )
	{
		ErrorLog(XEM_ERROR38);
		return false;
	}

	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
	{
		//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
		ErrorLogMsg( XEM_ERROR39, "m_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
		return false;
	}
/*
	CKTDGUIStatic* pStaticResultWin = m_pDLGFront->GetStatic_LUA( "PVP_Result_Window_Win" );
	pStaticResultWin->SetOffsetPos( D3DXVECTOR2(0, -200) );
	pStaticResultWin->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.2f );
*/
	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGBack ) == false )
	{
		//ResultLog << "m_pDLGBack 메모리 침범당함" << fileout;
		ErrorLogMsg( XEM_ERROR40, "m_pDLGBack 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGBack ) == false" );
		return false;
	}

/*
	CKTDGUIStatic* pStaticResultLose = m_pDLGBack->GetStatic_LUA( "PVP_Result_Window_Lose" );
	if( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_SURVIVAL )
	{
		pStaticResultLose->SetShow(false);
	}
	else
	{
		pStaticResultLose->SetOffsetPos( D3DXVECTOR2(0, 200) );
		pStaticResultLose->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), 0.2f );
	}
*/
	int iMaxSlot = 4;
	wstring wstrCardBack = L"DLG_PVP_Result_Card_Back.lua";
	wstring wstrCardFront = L"DLG_PVP_Result_Card_Front.lua";

#ifdef SERV_PVP_NEW_SYSTEM
	if( bMatchPvp == true )
	{
		iMaxSlot = 3;
		wstrCardBack = L"DLG_PVP_Result_Card_Back_MatchPvp.lua";
		wstrCardFront = L"DLG_PVP_Result_Card_Front_MatchPvp.lua";
	}
#endif
	for ( int i = 0; i < iMaxSlot; i++ )
	{

		CKTDGUIDialogType pDialogBack = new CKTDGUIDialog( this,wstrCardBack.c_str() );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialogBack );
		pDialogBack->SetPos( D3DXVECTOR2( pDialogBack->GetDummyPos( i ).x, pDialogBack->GetDummyPos( i ).y ) );		
		m_pDLGResultCardBackList.push_back( pDialogBack );

		CKTDGUIDialogType pDialogFront = new CKTDGUIDialog( this, wstrCardFront.c_str() );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialogFront );
		pDialogFront->SetPos( D3DXVECTOR2( pDialogFront->GetDummyPos( i ).x, pDialogFront->GetDummyPos( i ).y ) );
		m_pDLGResultCardFrontList.push_back( pDialogFront );		
	}

	if ( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || 
		g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
	{
		CX2PVPGame::PVPResultInfo* pPVPResultInfo = g_pData->GetPVPResultInfo();
		
		if ( pPVPResultInfo == NULL )
		{		
			ErrorLog(XEM_ERROR41);
			return false;
		}

#ifdef SERV_PVP_REMATCH
		if( bMatchPvp == true )
		{
			if( pPVPResultInfo->m_iRematchCount == 0 )
			{
				m_bCanRematch = true;
				m_pDLGRematch->GetControl( L"Popup" )->SetShowEnable( true, false );
				m_pDLGRematch->GetControl( L"RematchAsk2" )->SetShowEnable( true, false );
				m_pDLGRematch->GetControl( L"Yes" )->SetShowEnable( true, true );
				m_pDLGRematch->GetControl( L"No" )->SetShowEnable( true, true );
			}
			else if( pPVPResultInfo->m_iRematchCount == 1 )
			{
				m_bCanRematch = true;
				m_pDLGRematch->GetControl( L"Popup" )->SetShowEnable( true, false );
				m_pDLGRematch->GetControl( L"RematchAsk1" )->SetShowEnable( true, false );
				m_pDLGRematch->GetControl( L"Yes" )->SetShowEnable( true, true );
				m_pDLGRematch->GetControl( L"No" )->SetShowEnable( true, true );
			}
			else
			{
				m_bCanRematch = false;
			}
		}
#endif SERV_PVP_REMATCH

		int winTeam = pPVPResultInfo->m_WinTeam;
		int winTeamNum = 0;
		int loseTeamNum = 4;
#ifdef SERV_PVP_NEW_SYSTEM
		if( bMatchPvp == true )
			loseTeamNum = 3;
#endif

#ifdef SERV_PVP_NEW_SYSTEM
		if( bMatchPvp == true )
		{
			CKTDGUIStatic* pStaticRedTeamDraw = m_pDLGBack->GetStatic_LUA( "PVP_Result_Survivar_Draw" );
			if( NULL != pStaticRedTeamDraw )
				pStaticRedTeamDraw->SetShow(false);
			m_pDLGBack->GetControl(L"PVP_Result_RedTeamWin")->SetShow( false );
			m_pDLGBack->GetControl(L"PVP_Result_BlueTeamWin")->SetShow( false );

			if( true == pPVPResultInfo->m_bIsDrawn )
			{
				if( m_pDLGBack->GetControl(L"MatchPvp_Win") != NULL )
					m_pDLGBack->GetControl(L"MatchPvp_Win")->SetShow( false );
				if( m_pDLGBack->GetControl(L"MatchPvp_lose") != NULL )
					m_pDLGBack->GetControl(L"MatchPvp_lose")->SetShow( false );
				if( m_pDLGBack->GetControl(L"MatchPvp_draw") != NULL )
					m_pDLGBack->GetControl(L"MatchPvp_draw")->SetShow( true );
								
				if( NULL != pStaticRedTeamDraw )
					pStaticRedTeamDraw->SetShow( true );
			}
			else
			{
				if( winTeam ==  g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum )
				{
					if( m_pDLGBack->GetControl(L"MatchPvp_Win") != NULL )
						m_pDLGBack->GetControl(L"MatchPvp_Win")->SetShow( true );
					if( m_pDLGBack->GetControl(L"MatchPvp_lose") != NULL )
						m_pDLGBack->GetControl(L"MatchPvp_lose")->SetShow( false );
					if( m_pDLGBack->GetControl(L"MatchPvp_draw") != NULL )
						m_pDLGBack->GetControl(L"MatchPvp_draw")->SetShow( false );
				}
				else
				{
					if( m_pDLGBack->GetControl(L"MatchPvp_Win") != NULL )
						m_pDLGBack->GetControl(L"MatchPvp_Win")->SetShow( false );
					if( m_pDLGBack->GetControl(L"MatchPvp_lose") != NULL )
						m_pDLGBack->GetControl(L"MatchPvp_lose")->SetShow( true );
					if( m_pDLGBack->GetControl(L"MatchPvp_draw") != NULL )
						m_pDLGBack->GetControl(L"MatchPvp_draw")->SetShow( false );
				}

				if ( winTeam == CX2Room::TN_RED )
				{
					if ( m_pDLGBack->GetControl(L"PVP_Result_RedTeamWin") != NULL )
						m_pDLGBack->GetControl(L"PVP_Result_RedTeamWin")->SetShow( true );				
				}
				else
				{
					if ( m_pDLGBack->GetControl(L"PVP_Result_BlueTeamWin") != NULL )
						m_pDLGBack->GetControl(L"PVP_Result_BlueTeamWin")->SetShow( true );
				}
			}
		}
		else
#endif
		{
			if( true == pPVPResultInfo->m_bIsDrawn )
			{
				if ( m_pDLGBack->GetControl(L"Team_Win") != NULL )
					m_pDLGBack->GetControl(L"Team_Win")->SetShow( false );
				if ( m_pDLGBack->GetControl(L"Red_Win") != NULL )
					m_pDLGBack->GetControl(L"Red_Win")->SetShow( false );
				if ( m_pDLGBack->GetControl(L"Blue_Win") != NULL )
					m_pDLGBack->GetControl(L"Blue_Win")->SetShow( false );

				CKTDGUIStatic* pStaticDraw = m_pDLGBack->GetStatic_LUA( "Survival_Draw" );
				if( NULL != pStaticDraw )
					pStaticDraw->SetShow( true );

				CKTDGUIStatic* pStaticRedTeamDraw = m_pDLGBack->GetStatic_LUA( "PVP_Result_RedTeamDraw" );
				if( NULL != pStaticRedTeamDraw )
					pStaticRedTeamDraw->SetShow( true );
			}
			else
			{
				if ( m_pDLGBack->GetControl(L"Team_Win") != NULL )
					m_pDLGBack->GetControl(L"Team_Win")->SetShow( true );
				if ( winTeam == CX2Room::TN_RED )
				{
					if ( m_pDLGBack->GetControl(L"Red_Win") != NULL )
						m_pDLGBack->GetControl(L"Red_Win")->SetShow( true );

					if ( m_pDLGBack->GetControl(L"PVP_Result_RedTeamWin") != NULL )
						m_pDLGBack->GetControl(L"PVP_Result_RedTeamWin")->SetShow( true );				
				}
				else
				{
					if ( m_pDLGBack->GetControl(L"Blue_Win") != NULL )
						m_pDLGBack->GetControl(L"Blue_Win")->SetShow( true );

					if ( m_pDLGBack->GetControl(L"PVP_Result_BlueTeamWin") != NULL )
						m_pDLGBack->GetControl(L"PVP_Result_BlueTeamWin")->SetShow( true );
				}
			}
		}

#ifdef SERV_PVP_NEW_SYSTEM
		if( bMatchPvp == true )
		{
			// 정상종료인지 비정상 종료인지 판단 (m_bIsAbnormalEndPlay == false : 정상 종료)
			CKTDGUIStatic *pNotice = (CKTDGUIStatic*)m_pDLGBack->GetControl(L"StaticNOTICE");
#ifdef CLIENT_GLOBAL_LINEBREAK
			wstring wstrNotice;
			if( pPVPResultInfo->m_bIsAbnormalEndPlay == false )
			{
				wstrNotice = GET_STRING( STR_ID_13565 );
				pNotice->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2Main(wstrNotice.c_str(), 140, 0);
			}
			else
			{
				wstrNotice = GET_STRING( STR_ID_13566 );
				pNotice->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2Main(wstrNotice.c_str(), 140, 0);
			}
#else //CLIENT_GLOBAL_LINEBREAK
			if( pPVPResultInfo->m_bIsAbnormalEndPlay == false )
			{
				pNotice->GetString(0)->msg = GET_STRING( STR_ID_13565 );
			}
			else
			{
				pNotice->GetString(0)->msg = GET_STRING( STR_ID_13566 );
			}
#endif //CLIENT_GLOBAL_LINEBREAK
		}		
#endif

		// note!! 게임이 무승부이면 RED팀을 승리팀으로 간주해서 UI 처리
		if( true == pPVPResultInfo->m_bIsDrawn )
		{
			winTeam = CX2Room::TN_RED;
		}


		//이긴팀 부터 일단 등록시킨다.
		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			if ( pPVPResultUnitInfo == NULL )
				continue;

			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pPVPResultUnitInfo->m_UnitUid );

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
				continue;

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				continue;

			if ( winTeam == pSlotData->m_TeamNum )
			{
#ifdef SERV_PVP_NEW_SYSTEM
				CKTDGUIDialogType pDLGResultInfo = NULL;
				if( bMatchPvp == true )
				{
					pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info_MatchPvp.lua" );
				}
				else
				{
					pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info_practice.lua" );
				}
#else
				CKTDGUIDialogType pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info.lua" );
#endif
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDLGResultInfo );
				m_pDLGResultInfoList.push_back( pDLGResultInfo );

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == false )
#endif
				{
#ifndef SERV_PVP_NEW_SYSTEM
					if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
					{
						CKTDGUIStatic* pStaticLevelUp = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"LevelUpString" );
						if ( pStaticLevelUp != NULL )
						{
							pStaticLevelUp->SetShow( true );
						}

						CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticSp" );
						if ( pStaticSP != NULL && pStaticSP->GetString(0) != NULL )
						{
							pStaticSP->GetString( 0 )->msg = L"+2";

							if ( pSlotData->m_bMySlot == true )	
								pStaticSP->GetString( 0 )->color = myColor;
						}
					}

					if ( pPVPResultUnitInfo->m_bIsMVP == true )
					{
						CKTDGUIStatic* pStaticMVPTalkBox = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"MVPTalkBox" );
						if ( pStaticMVPTalkBox != NULL )
						{
							pStaticMVPTalkBox->SetShow( true );
						}
					}
#endif
				}				


				if ( pSlotData->m_pUnit->GetUnitData().m_bIsGameBang == true )
				{
					CKTDGUIStatic* pStaticPCRoomMark = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PCRoomMark" );
					if ( pStaticPCRoomMark != NULL )
					{
						pStaticPCRoomMark->SetShow( true );
					}
				}


#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
#endif
				{
					if ( pSlotData->m_pUnit->GetUnitData().m_fAddExpRate > 0.f )
					{
						CKTDGUIStatic* pStaticPremiumBonus = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PremiumBonusMark" );
						if ( pStaticPremiumBonus != NULL )
						{
							pStaticPremiumBonus->SetShow( true );
						}
					}
				}

		
				CKTDGUIStatic* pStaticEmblem = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"Emblem" );
				if ( pStaticEmblem != NULL )
				{
					if ( pStaticEmblem->GetPicture( 0 ) != NULL )
					{
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
						CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( pSlotData->m_cRank ) );
#else
						CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetUnitData().m_iRating );
#endif
#else
						CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetUnitData().m_PVPEmblem );
#endif
						if ( pEmblemData != NULL )
							pStaticEmblem->GetPicture( 0 )->SetTex( pEmblemData->m_TextureName.c_str(), pEmblemData->m_TextureKey.c_str() );
					}
				}
				
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == false || (bMatchPvp == true && g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum == pSlotData->m_TeamNum) )
#endif
				{
					if( pSlotData->m_pUnitViewer == NULL )
						pSlotData->MakeUnitViewer();
					pSlotData->m_pUnitViewer->SetShowObject( true );

					if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
					{
						pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_LEVEL_UP );
					}
					else
					{
						pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
				}
				

				//52차이
				//pDLGResultInfo->SetPos( D3DXVECTOR2( 0, winTeamNum * 52.0f ) );
#ifdef SERV_PVP_NEW_SYSTEM
				float xPosWinTeam = 0.f;
#else
				float xPosWinTeam = X_OFFSET;
#endif
				float yPosWinTeam = Y_OFFSET + ( winTeamNum * MAGIC_HEIGHT );
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
				{
					xPosWinTeam = 0.f;
					yPosWinTeam = -2.f + winTeamNum * MAGIC_HEIGHT;
				}
#endif

				pDLGResultInfo->SetPos( D3DXVECTOR2( -X_OFFSET, yPosWinTeam ) );
				pDLGResultInfo->Move( D3DXVECTOR2( xPosWinTeam, yPosWinTeam ), D3DXCOLOR(1,1,1,1), 0.3f + (winTeamNum * 0.1f) );

				D3DXCOLOR redTeamColor = D3DXCOLOR( 1.0f,0.0f,0.43f,1.0f );
				D3DXCOLOR blueTeamColor = D3DXCOLOR( 0.1f, 0.6f, 1.0f, 1.0f );
/*
				CKTDGUIStatic* pStaticDecision = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PVP_Result_Decision" );

				if( true == pPVPResultInfo->m_bIsDrawn )
				{
					pStaticDecision->GetString( 0 )->msg = L"DRAW";
				}
				else
				{
					pStaticDecision->GetString( 0 )->msg = L"WIN";
				}
				
				if ( pSlotData->m_TeamNum == CX2Room::TN_RED )
					pStaticDecision->GetString( 0 )->color = redTeamColor;
				else
					pStaticDecision->GetString( 0 )->color = blueTeamColor;
*/
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true && pSlotData->m_TeamNum == g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum )
					SetUnitCard( winTeamNum, pSlotData->m_pUnit, pPVPResultUnitInfo->m_Rank );
				else if( bMatchPvp == false )
					SetUnitCard( winTeamNum, pSlotData->m_pUnit, pPVPResultUnitInfo->m_Rank );
#else
				SetUnitCard( winTeamNum, pSlotData->m_pUnit, pPVPResultUnitInfo->m_Rank );
#endif

				

				WCHAR buff[256] = {0,};

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == false )
#endif
				{
					CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"Static_Result_Rank" );
					_itow( pPVPResultUnitInfo->m_Rank, buff, 10 );
					pStaticRank->GetString(0)->msg = buff;
					if ( pSlotData->m_TeamNum == CX2Room::TN_RED )
						pStaticRank->GetString( 0 )->outlineColor = redTeamColor;
					else
						pStaticRank->GetString( 0 )->outlineColor = blueTeamColor;

					if ( pSlotData->m_bMySlot == true )	
						pStaticRank->GetString( 0 )->color = myColor;
				}

				CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticID" );
				pStaticName->GetString( 0 )->msg = pSlotData->m_pUnit->GetNickName();

				if ( pSlotData->m_bMySlot == true )	
					pStaticName->GetString( 0 )->color = myColor;

				CKTDGUIStatic* pStaticKill = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticKILL" );
				//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_KillNum );
				StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_KillNum );
				pStaticKill->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticKill->GetString( 0 )->color = myColor;

				CKTDGUIStatic* pStaticAssist = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticASSIS1T2" );
				//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_MDKillNum );
				StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_MDKillNum );
				pStaticAssist->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticAssist->GetString( 0 )->color = myColor;

				CKTDGUIStatic* pStaticDEATH = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticDEATH" );
				wsprintf( buff, L"%d", pPVPResultUnitInfo->m_DieNum );
				StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_DieNum );
				pStaticDEATH->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticDEATH->GetString( 0 )->color = myColor;

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
#endif
				{
					CKTDGUIStatic* pStaticVS_POINT = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticVS_POINT" );
#ifdef SERV_PVP_NEW_SYSTEM 
					// 같은걸 왜 두번씩 하지??????
					//wsprintf( buff, L"+%d", pPVPResultUnitInfo->m_iAPoint );
					if( pPVPResultUnitInfo->m_iAPoint > 0 )
						StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_iAPoint );
					else
						StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_iAPoint );
#else
					wsprintf( buff, L"+%d", pPVPResultUnitInfo->m_PVPPoint );
					StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_PVPPoint );
#endif
					pStaticVS_POINT->GetString( 0 )->msg = buff;

					if ( pSlotData->m_bMySlot == true )	
						pStaticVS_POINT->GetString( 0 )->color = myColor;

					CKTDGUIStatic* pStaticED = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticEXP" );
					if( pPVPResultUnitInfo->m_EXP > 0)
						StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_EXP );
					else
						StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_EXP );

					pStaticED->GetString( 0 )->msg = buff;

					if ( pSlotData->m_bMySlot == true )	
						pStaticED->GetString( 0 )->color = myColor;
				}

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
				{
					CKTDGUIStatic* pStaticRANK = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticRANK" );
					if( pStaticRANK != NULL )
					{
						if( pPVPResultUnitInfo->m_iRPoint > 0 )
							StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_iRPoint );
						else
							StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_iRPoint );

						pStaticRANK->GetString(0)->msg = buff;
						if( pSlotData->m_bMySlot == true )
							pStaticRANK->GetString(0)->color = myColor;
					}
				}
#endif				

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == false )
#endif
				{
#ifndef SERV_PVP_NEW_SYSTEM
					CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticSp" );
					if ( pStaticSP != NULL && pStaticSP->GetString(0) != NULL )
					{

						if ( pSlotData->m_bMySlot == true )	
							pStaticSP->GetString( 0 )->color = myColor;
					}
#endif
				}				

				winTeamNum++;
#ifdef SERV_PVP_REMATCH
				m_mapSlotIndexByUnitUID[ pPVPResultUnitInfo->m_UnitUid ] = winTeamNum;
#endif SERV_PVP_REMATCH
			}
		}

		//진팀 등록시킨다.
		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			if ( pPVPResultUnitInfo == NULL )
				continue;

			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pPVPResultUnitInfo->m_UnitUid );

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
				continue;

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				continue;

			if ( winTeam != pSlotData->m_TeamNum )
			{
#ifdef SERV_PVP_NEW_SYSTEM
				CKTDGUIDialogType pDLGResultInfo = NULL;
				if( bMatchPvp == true )
				{
					pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info_MatchPvp.lua" );
				}
				else
				{
					pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info_practice.lua" );
				}
#else
				CKTDGUIDialogType pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info.lua" );
#endif
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDLGResultInfo );
				m_pDLGResultInfoList.push_back( pDLGResultInfo );

				CKTDGUIStatic* pStaticEmblem = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"Emblem" );
				if ( pStaticEmblem != NULL )
				{
					if ( pStaticEmblem->GetPicture( 0 ) != NULL )
					{
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
						CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( pSlotData->m_cRank ) );
#else
						CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetUnitData().m_iRating );
#endif
#else
						CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetUnitData().m_PVPEmblem );
#endif
						if ( pEmblemData != NULL )
							pStaticEmblem->GetPicture( 0 )->SetTex( pEmblemData->m_TextureName.c_str(), pEmblemData->m_TextureKey.c_str() );
					}
				}

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == false )
#endif
				{
#ifndef SERV_PVP_NEW_SYSTEM
					if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
					{
						CKTDGUIStatic* pStaticLevelUp = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"LevelUpString" );
						if ( pStaticLevelUp != NULL )
						{
							pStaticLevelUp->SetShow( true );
						}


						CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticSp" );
						if ( pStaticSP != NULL && pStaticSP->GetString(0) != NULL )
						{
							pStaticSP->GetString( 0 )->msg = L"+2";

							if ( pSlotData->m_bMySlot == true )	
								pStaticSP->GetString( 0 )->color = myColor;
						}
					}

					if ( pPVPResultUnitInfo->m_bIsMVP == true )
					{
						CKTDGUIStatic* pStaticMVPTalkBox = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"MVPTalkBox" );
						if ( pStaticMVPTalkBox != NULL )
						{
							pStaticMVPTalkBox->SetShow( true );
						}
					}
#endif
				}				

				

				if ( pSlotData->m_pUnit->GetUnitData().m_bIsGameBang == true )
				{
					CKTDGUIStatic* pStaticPCRoomMark = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PCRoomMark" );
					if ( pStaticPCRoomMark != NULL )
					{
						pStaticPCRoomMark->SetShow( true );
					}
				}

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
#endif
				{
					if ( pSlotData->m_pUnit->GetUnitData().m_fAddExpRate > 0.f )
					{
						CKTDGUIStatic* pStaticPremiumBonus = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PremiumBonusMark" );
						if ( pStaticPremiumBonus != NULL )
						{
							pStaticPremiumBonus->SetShow( true );
						}
					}
				}

			
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true && g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum == pSlotData->m_TeamNum )
				{
					if( pSlotData->m_pUnitViewer == NULL )
						pSlotData->MakeUnitViewer();
					pSlotData->m_pUnitViewer->SetShowObject( true );

					if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
					{

						pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLevelUp", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					else
					{
						if( true == pPVPResultInfo->m_bIsDrawn )
						{
							pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
						}
						else
						{
							pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLose", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
						}
					}
				}
#endif

				//52차이
#ifdef SERV_PVP_NEW_SYSTEM
				float xPosLoseTeam = 0.f;
#else
				float xPosLoseTeam = X_OFFSET;
#endif
				float yPosLoseTeam = Y_OFFSET + ( loseTeamNum * MAGIC_HEIGHT );
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
				{
					xPosLoseTeam = 0.f;
					yPosLoseTeam = 15.f + loseTeamNum * MAGIC_HEIGHT;
				}
#endif
				//pDLGResultInfo->SetPos( D3DXVECTOR2( 0, winTeamNum * 52.0f ) );
				pDLGResultInfo->SetPos( D3DXVECTOR2(-X_OFFSET, yPosLoseTeam ) );
				pDLGResultInfo->Move( D3DXVECTOR2( xPosLoseTeam, yPosLoseTeam ), D3DXCOLOR(1,1,1,1), 0.3f + (loseTeamNum * 0.1f) );

				D3DXCOLOR redTeamColor = D3DXCOLOR( 1.0f,0.0f,0.43f,1.0f );
				D3DXCOLOR blueTeamColor = D3DXCOLOR( 0.1f, 0.6f, 1.0f, 1.0f );
/*
				CKTDGUIStatic* pStaticDecision = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PVP_Result_Decision" );

				if( true == pPVPResultInfo->m_bIsDrawn )
				{
					pStaticDecision->GetString( 0 )->msg = L"DRAW";
				}
				else
				{
					pStaticDecision->GetString( 0 )->msg = L"LOSE";
				}

				if ( pSlotData->m_TeamNum == CX2Room::TN_RED )
					pStaticDecision->GetString( 0 )->color = redTeamColor;
				else
					pStaticDecision->GetString( 0 )->color = blueTeamColor;
*/
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true && pSlotData->m_TeamNum == g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum )
					SetUnitCard( loseTeamNum-3, pSlotData->m_pUnit, pPVPResultUnitInfo->m_Rank );
#endif

				WCHAR buff[256] = {0,};

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == false )
#endif
				{
					CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"Static_Result_Rank" );
					_itow( pPVPResultUnitInfo->m_Rank, buff, 10 );
					pStaticRank->GetString(0)->msg = buff;
					if ( pSlotData->m_TeamNum == CX2Room::TN_RED )
						pStaticRank->GetString( 0 )->outlineColor = redTeamColor;
					else
						pStaticRank->GetString( 0 )->outlineColor = blueTeamColor;

					if ( pSlotData->m_bMySlot == true )	
						pStaticRank->GetString( 0 )->color = myColor;
				}				

				CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticID" );
				pStaticName->GetString( 0 )->msg = pSlotData->m_pUnit->GetNickName();

				if ( pSlotData->m_bMySlot == true )	
					pStaticName->GetString( 0 )->color = myColor;

				CKTDGUIStatic* pStaticKill = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticKILL" );
				//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_KillNum );
				StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_KillNum );
				pStaticKill->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticKill->GetString( 0 )->color = myColor;


				CKTDGUIStatic* pStaticAssist = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticASSIS1T2" );
				//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_MDKillNum );
				StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_MDKillNum );
				pStaticAssist->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticAssist->GetString( 0 )->color = myColor;



				CKTDGUIStatic* pStaticDEATH = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticDEATH" );
				//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_DieNum );
				StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_DieNum );
				pStaticDEATH->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticDEATH->GetString( 0 )->color = myColor;

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
#endif
				{
					CKTDGUIStatic* pStaticVS_POINT = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticVS_POINT" );
#ifdef SERV_PVP_NEW_SYSTEM
					if( pPVPResultUnitInfo->m_iAPoint > 0 )
						StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_iAPoint );
					else
						StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_iAPoint );
#else
					StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_PVPPoint );
#endif
					pStaticVS_POINT->GetString( 0 )->msg = buff;

					if ( pSlotData->m_bMySlot == true )	
						pStaticVS_POINT->GetString( 0 )->color = myColor;

					CKTDGUIStatic* pStaticED = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticEXP" );
					if( pPVPResultUnitInfo->m_EXP > 0 )
						StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_EXP );
					else
						StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_EXP );

					pStaticED->GetString( 0 )->msg = buff;

					if ( pSlotData->m_bMySlot == true )	
						pStaticED->GetString( 0 )->color = myColor;
				}
				

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
				{
					CKTDGUIStatic* pStaticRANK = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticRANK" );
					if( pStaticRANK != NULL )
					{
						if( pPVPResultUnitInfo->m_iRPoint > 0)
							StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_iRPoint );
						else
							StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_iRPoint );

						pStaticRANK->GetString(0)->msg = buff;
						if( pSlotData->m_bMySlot == true )
							pStaticRANK->GetString(0)->color = myColor;
					}
				}
#endif				

#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == false )
#endif
				{
#ifndef SERV_PVP_NEW_SYSTEM
					CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticSp" );
					if ( pStaticSP != NULL && pStaticSP->GetString(0) != NULL )
					{

						if ( pSlotData->m_bMySlot == true )	
							pStaticSP->GetString( 0 )->color = myColor;
					}
#endif
				}				

				loseTeamNum++;
#ifdef SERV_PVP_REMATCH
				m_mapSlotIndexByUnitUID[ pPVPResultUnitInfo->m_UnitUid ] = loseTeamNum;
#endif SERV_PVP_REMATCH
			}
		}


	}
	else // 난입 서바이벌 
	{
		CX2PVPGame::PVPResultInfo* pPVPResultInfo = g_pData->GetPVPResultInfo();
		const int nNumPlayer = (int)pPVPResultInfo->m_PVPResultUnitInfoList.size();
		int tempMiddleValue = nNumPlayer/2;
		if ( tempMiddleValue <= 0 )
			tempMiddleValue = 1;

		int winTeamNum = 0;
		int loseTeamNum = 4;
/*
		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			if ( pPVPResultUnitInfo == NULL )
				continue;

			if( pPVPResultUnitInfo->m_UnitUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				CKTDGUIStatic* pStaticWin = m_pDLGBack->GetStatic_LUA( "Survival_Win" );
				CKTDGUIStatic* pStaticLose = m_pDLGBack->GetStatic_LUA( "Survival_Lose" );
				
				if( i < tempMiddleValue ) // win 
				{
					pStaticWin->SetShow( true );
				}
				else
				{
					pStaticLose->SetShow( true );
				}
				
				break;
			}
		}
	*/

		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			if ( pPVPResultUnitInfo == NULL )
				continue;

			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pPVPResultUnitInfo->m_UnitUid );

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
				continue;

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				continue;


			if( pPVPResultUnitInfo->m_UnitUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			{
				CKTDGUIStatic* pStaticWin = m_pDLGBack->GetStatic_LUA( "Survival_Win" );
				CKTDGUIStatic* pStaticLose = m_pDLGBack->GetStatic_LUA( "Survival_Lose" );
				CKTDGUIStatic* pStaticDraw = m_pDLGBack->GetStatic_LUA( "Survival_Draw" );


				if( true == pPVPResultInfo->m_bIsDrawn )
				{
					pStaticWin->SetShow( false );
					pStaticLose->SetShow( false );
					pStaticDraw->SetShow( true );

					CKTDGUIStatic* pStaticSuvaDraw = m_pDLGBack->GetStatic_LUA( "PVP_Result_Survivar_Draw" );
					if( NULL != pStaticSuvaDraw )
						pStaticSuvaDraw->SetShow( true );

				}
				else
				{
					if( i < tempMiddleValue ) // win 
					{
						pStaticWin->SetShow( true );
					}
					else
					{
						pStaticLose->SetShow( true );
					}

					CKTDGUIStatic* pStaticSuva = m_pDLGBack->GetStatic_LUA( "PVP_Result_Survivar" );
					if( NULL != pStaticSuva )
						pStaticSuva->SetShow( true );
					
				}
			}



			if( i < 4 )
			{
				if( pSlotData->m_pUnitViewer == NULL )
					pSlotData->MakeUnitViewer();
				pSlotData->m_pUnitViewer->SetShowObject( true );

				if ( pSlotData == g_pData->GetPVPRoom()->GetMySlot() )
				{

				}

				if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
				{
					
					pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_LEVEL_UP );
				}
				else
				{
					if( true == pPVPResultInfo->m_bIsDrawn )
					{
						pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
					}
					else
					{
						if( i < tempMiddleValue ) // win 
						{
							pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
						}
						else
						{
							pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLose", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
						}		
					}
				}

				SetUnitCard( i, pSlotData->m_pUnit, pPVPResultUnitInfo->m_Rank );
			}
			else
			{
				if( pSlotData->m_pUnitViewer != NULL )
				{
					CX2UnitViewerUI::DeleteKTDGObject( pSlotData->m_pUnitViewer );
					pSlotData->m_pUnitViewer = NULL;
				}
			}

#ifdef SERV_PVP_NEW_SYSTEM
			CKTDGUIDialogType pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info_practice.lua" );			
#else
			CKTDGUIDialogType pDLGResultInfo = new CKTDGUIDialog( this, L"DLG_PVP_Result_Info.lua" );
#endif
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDLGResultInfo );
			m_pDLGResultInfoList.push_back( pDLGResultInfo );

			

			if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
			{
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
#endif
				{
					CKTDGUIStatic* pStaticLevelUp = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"LevelUpString" );
					if ( pStaticLevelUp != NULL )
					{
						pStaticLevelUp->SetShow( true );
					}
				}

#ifndef SERV_PVP_NEW_SYSTEM
				CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticSp" );
				if ( pStaticSP != NULL && pStaticSP->GetString(0) != NULL )
				{
					pStaticSP->GetString( 0 )->msg = L"+2";

					if ( pSlotData->m_bMySlot == true )	
						pStaticSP->GetString( 0 )->color = myColor;
				}
#endif
			}

#ifndef SERV_PVP_NEW_SYSTEM
			if ( pPVPResultUnitInfo->m_bIsMVP == true )
			{
				CKTDGUIStatic* pStaticMVPTalkBox = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"MVPTalkBox" );
				if ( pStaticMVPTalkBox != NULL )
				{
					pStaticMVPTalkBox->SetShow( true );
				}
			}
#endif

			if ( pSlotData->m_pUnit->GetUnitData().m_bIsGameBang == true )
			{
				CKTDGUIStatic* pStaticPCRoomMark = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PCRoomMark" );
				if ( pStaticPCRoomMark != NULL )
				{
					pStaticPCRoomMark->SetShow( true );
				}
			}

#ifdef SERV_PVP_NEW_SYSTEM
			if( bMatchPvp == true )
#endif
			{
				if ( pSlotData->m_pUnit->GetUnitData().m_fAddExpRate > 0.f )
				{
					CKTDGUIStatic* pStaticPremiumBonus = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PremiumBonusMark" );
					if ( pStaticPremiumBonus != NULL )
					{
						pStaticPremiumBonus->SetShow( true );
					}
				}
			}

			CKTDGUIStatic* pStaticEmblem = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"Emblem" );
			if ( pStaticEmblem != NULL )
			{
				if ( pStaticEmblem->GetPicture( 0 ) != NULL )
				{
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
					CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( pSlotData->m_cRank ) );
#else
					CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetUnitData().m_iRating );
#endif
#else
					CX2PVPEmblem::PVPEmblemData* pEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( pSlotData->m_pUnit->GetUnitData().m_PVPEmblem );
#endif
					if ( pEmblemData != NULL )
						pStaticEmblem->GetPicture( 0 )->SetTex( pEmblemData->m_TextureName.c_str(), pEmblemData->m_TextureKey.c_str() );
				}
			}


			if( i < tempMiddleValue )
			{
				//52차이
#ifdef SERV_PVP_NEW_SYSTEM
				float xPosWinTeam = 0.f;
#else
				float xPosWinTeam = X_OFFSET;
#endif
				float yPosWinTeam = Y_OFFSET + ( winTeamNum * MAGIC_HEIGHT );
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
				{
					xPosWinTeam = 0.f;
					yPosWinTeam = winTeamNum * MAGIC_HEIGHT;
				}
#endif
				//pDLGResultInfo->SetPos( D3DXVECTOR2( 0, i * 52.0f ) );
				pDLGResultInfo->SetPos( D3DXVECTOR2( -X_OFFSET, yPosWinTeam ) );
				pDLGResultInfo->Move( D3DXVECTOR2( xPosWinTeam, yPosWinTeam ), D3DXCOLOR(1,1,1,1), 0.3f + (winTeamNum * 0.1f) );

				winTeamNum++;	
			}	
			else
			{
				//52차이
#ifdef SERV_PVP_NEW_SYSTEM
				float xPosLoseTeam = 0.f;
#else
				float xPosLoseTeam = X_OFFSET;
#endif
				float yPosLoseTeam = Y_OFFSET + ( loseTeamNum * MAGIC_HEIGHT );
#ifdef SERV_PVP_NEW_SYSTEM
				if( bMatchPvp == true )
				{
					xPosLoseTeam = 0.f;
					yPosLoseTeam = loseTeamNum * MAGIC_HEIGHT;
				}
#endif
				//pDLGResultInfo->SetPos( D3DXVECTOR2( 0, i * 52.0f ) );
				pDLGResultInfo->SetPos( D3DXVECTOR2( -X_OFFSET,yPosLoseTeam ) );
				pDLGResultInfo->Move( D3DXVECTOR2( xPosLoseTeam, yPosLoseTeam ), D3DXCOLOR(1,1,1,1), 0.3f + (loseTeamNum * 0.1f) );

				loseTeamNum++;
			}
				




/*
			CKTDGUIStatic* pStaticDecision = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"PVP_Result_Decision" );

			if( true == pPVPResultInfo->m_bIsDrawn )
			{
				pStaticDecision->GetString( 0 )->msg = L"DRAW";
			}
			else
			{
				if( i < tempMiddleValue )
					pStaticDecision->GetString( 0 )->msg = L"WIN";
				else
					pStaticDecision->GetString( 0 )->msg = L"LOSE";
			}
*/


			WCHAR buff[256] = {0,};
			CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"Static_Result_Rank" );
			_itow( pPVPResultUnitInfo->m_Rank, buff, 10 );
			pStaticRank->GetString(0)->msg = buff;

			if ( pSlotData->m_bMySlot == true )	
				pStaticRank->GetString( 0 )->color = myColor;

			CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticID" );
			pStaticName->GetString( 0 )->msg = pSlotData->m_pUnit->GetNickName();

			if ( pSlotData->m_bMySlot == true )	
				pStaticName->GetString( 0 )->color = myColor;

			CKTDGUIStatic* pStaticKill = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticKILL" );
			//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_KillNum );
			StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_KillNum );
			pStaticKill->GetString( 0 )->msg = buff;

			if ( pSlotData->m_bMySlot == true )	
				pStaticKill->GetString( 0 )->color = myColor;


			CKTDGUIStatic* pStaticAssist = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticASSIS1T2" );
			//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_MDKillNum );
			StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_KillNum );
			pStaticAssist->GetString( 0 )->msg = buff;

			if ( pSlotData->m_bMySlot == true )	
				pStaticAssist->GetString( 0 )->color = myColor;


			CKTDGUIStatic* pStaticDEATH = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticDEATH" );
			//wsprintf( buff, L"%d", pPVPResultUnitInfo->m_DieNum );
			StringCchPrintf( buff, 256, L"%d", pPVPResultUnitInfo->m_DieNum );
			pStaticDEATH->GetString( 0 )->msg = buff;

			if ( pSlotData->m_bMySlot == true )	
				pStaticDEATH->GetString( 0 )->color = myColor;

#ifdef SERV_PVP_NEW_SYSTEM
			if( bMatchPvp == true )
#endif
			{
				CKTDGUIStatic* pStaticVS_POINT = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticVS_POINT" );
				//wsprintf( buff, L"+%d", pPVPResultUnitInfo->m_PVPPoint );
#ifdef SERV_PVP_NEW_SYSTEM
				StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_iAPoint );
#else
				StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_PVPPoint );
#endif
				pStaticVS_POINT->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticVS_POINT->GetString( 0 )->color = myColor;

				CKTDGUIStatic* pStaticED = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticEXP" );
				//wsprintf( buff, L"+%d", pPVPResultUnitInfo->m_EXP );
				StringCchPrintf( buff, 256, L"+%d", pPVPResultUnitInfo->m_EXP );
				pStaticED->GetString( 0 )->msg = buff;

				if ( pSlotData->m_bMySlot == true )	
					pStaticED->GetString( 0 )->color = myColor;

#ifndef SERV_PVP_NEW_SYSTEM
				CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*)pDLGResultInfo->GetControl( L"StaticSp" );
				if ( pStaticSP != NULL && pStaticSP->GetString(0) != NULL )
				{

					if ( pSlotData->m_bMySlot == true )	
						pStaticSP->GetString( 0 )->color = myColor;
				}
#endif
			}
		}

	}


	if ( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || 
		g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
	{
		CX2PVPGame::PVPResultInfo* pPVPResultInfo = g_pData->GetPVPResultInfo();
		int winTeam = pPVPResultInfo->m_WinTeam;
		int winTeamNum = 0;
		int loseTeamNum = 4;

#ifdef SERV_PVP_NEW_SYSTEM
		if( bMatchPvp == true )
			loseTeamNum = 3;
#endif

		// note!! 게임이 무승부이면 RED팀을 승리팀으로 간주해서 UI 처리
		if( true == pPVPResultInfo->m_bIsDrawn )
		{
			winTeam = CX2Room::TN_RED;
		}


		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pPVPResultUnitInfo->m_UnitUid );

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL || pSlotData->m_pUnitViewer == NULL )
				continue;

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				continue;

#ifdef SERV_PVP_NEW_SYSTEM
			if( bMatchPvp == true && pSlotData->m_TeamNum != g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum )
				continue;
#endif

			if( winTeam == pSlotData->m_TeamNum )
			{
				if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
				{
					
					pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_LEVEL_UP );
					g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LevelUp", m_pDLGBack->GetDummyPos(winTeamNum).x , m_pDLGBack->GetDummyPos(winTeamNum).y, 0, 1000, 1000, 1, 1, 1 );
				}
				else
				{
					pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
				}

				winTeamNum++;
			}
			else
			{
#ifdef SERV_PVP_NEW_SYSTEM				
				if( bMatchPvp == true )
				{
					if( pSlotData->m_pUnit->GetIsLevelUp() == true )
					{

						pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLevelUp", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
						g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LevelUp", m_pDLGBack->GetDummyPos(loseTeamNum).x , m_pDLGBack->GetDummyPos(loseTeamNum).y, 0, 1000, 1000, 1, 1, 1 );
					}
					else
					{
						if( true == pPVPResultInfo->m_bIsDrawn )
						{
							pSlotData->m_pUnitViewer->PlayAnim( L"LobbyWin", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
						}
						else
						{
							pSlotData->m_pUnitViewer->PlayAnim( L"LobbyLose", CKTDGXSkinAnim::XAP_ONE_WAIT, true );
						}						
					}
				}				
#endif
				
				loseTeamNum++;
			}
		}	
	}
	else
	{
		CX2PVPGame::PVPResultInfo* pPVPResultInfo = g_pData->GetPVPResultInfo();
		int winTeam = pPVPResultInfo->m_WinTeam;
		int winTeamNum = 0;
		int loseTeamNum = 4;
		const int nNumPlayer = (int)pPVPResultInfo->m_PVPResultUnitInfoList.size();

		for ( int i = 0; i < (int)pPVPResultInfo->m_PVPResultUnitInfoList.size(); i++ )
		{
	
			CX2PVPGame::PVPResultUnitInfo* pPVPResultUnitInfo = pPVPResultInfo->m_PVPResultUnitInfoList[i];
			CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( pPVPResultUnitInfo->m_UnitUid );

			if ( pSlotData == NULL || pSlotData->m_pUnit == NULL || pSlotData->m_pUnitViewer == NULL )
				continue;

			if ( pSlotData->GetSlotState() == CX2Room::SS_CLOSE || pSlotData->GetSlotState() == CX2Room::SS_EMPTY )
				continue;

			

			if( i >= 4 )
				continue;

			if ( pSlotData->m_pUnit->GetIsLevelUp() == true )
			{
				
				pSlotData->m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_LEVEL_UP );
				g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"LevelUp", m_pDLGBack->GetDummyPos(i).x , m_pDLGBack->GetDummyPos(i).y, 0, 1000, 1000, 1, 1, 1 );
			}			

		}	
	}
	
	std::vector<KPVPChannelInfo> channelList = g_pMain->GetChannelList();
	for ( int j = 0; j < (int)channelList.size(); j++ )
	{
		KPVPChannelInfo& channelInfo = channelList[j];
		if ( channelInfo.m_iChannelID == g_pMain->GetConnectedChannelID() )
		{
			if ( channelInfo.m_cPVPChannelClass == KPVPChannelInfo::PCC_FREE || channelInfo.m_cPVPChannelClass == KPVPChannelInfo::PCC_PLAY )
			{
				m_pDLGFrontFreeChannel = new CKTDGUIDialog( this, L"DLG_PVP_Result_Free_Channel_Notice.lua" );
				g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFrontFreeChannel );
			}
		}
	}

	DialogLog( "CX2StatePVPResult::InitUI() End" );

	return true;
}

void CX2StatePVPResult::UnitViewerProcess( CX2UnitViewerUI* pViewer, int slotNum )
{
	if ( pViewer == NULL )
	{
		ErrorLogMsg( XEM_ERROR42, "pViewer == NULL" );
		return;
	}

	//pViewer->SetLightPos( 1000, 1000, -200 );
	//pViewer->SetLightPos( 300, 300, -500 );	// 캐릭터뷰어 라이트 위치 변경
	pViewer->SetLightPos( -250, 100, -600 );	// 캐릭터뷰어 라이트 위치 변경


	if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
	{
		//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
		ErrorLogMsg( XEM_ERROR43, "_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
		return;
	}

	pViewer->GetMatrix().Move( m_pDLGFront->GetDummyPos(slotNum) );

	D3DXVECTOR3 vScale1 = D3DXVECTOR3(1.4f, 1.4f, 1.4f);
#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
		vScale1 = D3DXVECTOR3(1.4f, 1.4f, 1.4f);
#endif

	//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
	switch(slotNum)
	{
	case 0:
		{
			pViewer->GetMatrix().Scale( vScale1 );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 1:
		{
			pViewer->GetMatrix().Scale( vScale1 );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 2:
		{
			pViewer->GetMatrix().Scale( vScale1 );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 3:
		{
			pViewer->GetMatrix().Scale( vScale1 );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		} break;
	case 4:
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(100),D3DXToRadian(0));
		} break;
	case 5:
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		} break;
	case 6:
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(100),D3DXToRadian(0));
		} break;
	case 7:
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		} break;
	default:
		{
			ASSERT( !"PVPresult : More then 8 users" );
		} break;

	}//}}
	


/*
	if( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM || 
		g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
	{

		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
		{
			//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
			ErrorLogMsg( XEM_ERROR43, "_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
			return;
		}

		pViewer->GetMatrix().Move( m_pDLGFront->GetDummyPos(slotNum) );

		if ( slotNum == 0 )
		{

			pViewer->GetMatrix().Scale( 1.5f,1.5f,1.5f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}
		else if ( slotNum == 1 )
		{
			pViewer->GetMatrix().Scale( 1.5f,1.5f,1.5f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}
		else if ( slotNum == 2 )
		{
			pViewer->GetMatrix().Scale( 1.7f,1.7f,1.7f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}
		else if ( slotNum == 3 )
		{
			pViewer->GetMatrix().Scale( 1.7f,1.7f,1.7f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}
		else if ( slotNum == 4 )
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(100),D3DXToRadian(0));
		}
		else if ( slotNum == 5 )
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		}
		else if ( slotNum == 6 )
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(100),D3DXToRadian(0));
		}
		else if ( slotNum == 7 )
		{
			pViewer->GetMatrix().Scale( 1.f, 1.f, 1.f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
			//pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(90),D3DXToRadian(0));
		}

	}	
	else
	{
		if ( g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false )
		{
			//ResultLog << "m_pDLGFront 메모리 침범당함" << fileout;
			ErrorLogMsg( XEM_ERROR44, "_pDLGFront 메모리 침범당함, g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckDlg( m_pDLGFront ) == false" );
			return;
		}

		pViewer->GetMatrix().Move( m_pDLGFront->GetDummyPos(slotNum ) );

		if ( slotNum == 0 )
		{
			pViewer->GetMatrix().Scale( 1.9f,1.9f,1.9f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}
		else if ( slotNum == 1 )
		{
			pViewer->GetMatrix().Scale( 1.5f,1.5f,1.5f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}
		else if ( slotNum == 2 )
		{
			pViewer->GetMatrix().Scale( 1.5f,1.5f,1.5f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}
		else if ( slotNum == 3 )
		{
			pViewer->GetMatrix().Scale( 1.5f,1.5f,1.5f );
			pViewer->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		}

	}
*/


	pViewer->OnFrameMove( m_fTime, m_fElapsedTime );
}


void CX2StatePVPResult::SetUnitCard( int index, CX2Unit* pUnit, int rankNum )
{
	if ( index < 0 || index >= (int)m_pDLGResultCardBackList.size() )
		return;

	if ( index < 0 || index >= (int)m_pDLGResultCardFrontList.size() )
		return;
	
	if ( pUnit == NULL )
		return;

	CKTDGUIDialogType pDialog = m_pDLGResultCardBackList[index];
	if ( pDialog == NULL )
		return;

	CKTDGUIDialogType pDialogFront = m_pDLGResultCardFrontList[index];
	if ( pDialogFront == NULL )
		return;

	CKTDGUIStatic* pStaticChar = (CKTDGUIStatic*)pDialog->GetControl( L"PVP_Result_Info_Bg_Character" );
	if ( pStaticChar != NULL )
	{
		pStaticChar->SetShow( true );
		for ( int i = 0; i < CX2Unit::UT_END - 1; i++ )
		{
			if ( pStaticChar->GetPicture(i) != NULL )
			{
				pStaticChar->GetPicture(i)->SetShow( false );		// note!! 신캐릭터 추가시 스크립트에 추가
			}
		}

		
		if ( pUnit->GetUnitTemplet() != NULL )
		{
			switch( pUnit->GetType() )
			{
			case CX2Unit::UT_ELSWORD:
				{
					if ( pStaticChar->GetPicture(0) != NULL )
						pStaticChar->GetPicture(0)->SetShow( true );
				} break;

			case CX2Unit::UT_ARME:
				{
					if ( pStaticChar->GetPicture(1) != NULL )
						pStaticChar->GetPicture(1)->SetShow( true );
				} break;
		
			case CX2Unit::UT_LIRE:
				{
					if ( pStaticChar->GetPicture(2) != NULL )
						pStaticChar->GetPicture(2)->SetShow( true );
				} break;
			
			case CX2Unit::UT_RAVEN:
				{
					if ( pStaticChar->GetPicture(3) != NULL )
						pStaticChar->GetPicture(3)->SetShow( true );
				} break;

			case CX2Unit::UT_EVE:
				{
					if ( pStaticChar->GetPicture(4) != NULL )				
						pStaticChar->GetPicture(4)->SetShow( true );		
				} break;
		
//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UT_CHUNG:
				{
					if ( pStaticChar->GetPicture(5) != NULL )
						pStaticChar->GetPicture(5)->SetShow( true );
				} break;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UT_ARA:
				{
					if ( pStaticChar->GetPicture(6) != NULL )
						pStaticChar->GetPicture(6)->SetShow( true );
				} break;
#endif
#ifdef NEW_CHARACTER_EL
			case CX2Unit::UT_ELESIS:
				{
					if ( pStaticChar->GetPicture(7) != NULL )
						pStaticChar->GetPicture(7)->SetShow( true );
				} break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			case CX2Unit::UT_ADD:
				{
					if ( pStaticChar->GetPicture(8) != NULL )
						pStaticChar->GetPicture(8)->SetShow( true );
				} break;
#endif //SERV_9TH_NEW_CHARACTER
			default:
				{
					ASSERT( !"Unexpected UnitClass" );
                } break;

			}//}}
			
		}
	}

	CKTDGUIStatic* pStaticID = (CKTDGUIStatic*)pDialogFront->GetControl( L"PVP_Result_User_Box_Id");
	if ( pStaticID != NULL )
	{
		pStaticID->SetShow( true );
		if ( pStaticID->GetString(0) != NULL )
		{
			pStaticID->GetString(0)->msg = pUnit->GetNickName();
		}
	}

#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL )
	{
		if ( pDialogFront->GetControl(L"PVP_Result_User_Box_Level_Up") != NULL )
		{
			pDialogFront->GetControl(L"PVP_Result_User_Box_Level_Up")->SetShow( false );
		}
	}
	else
#endif // SERV_PVP_NEW_SYSTEM
	{
		if ( pUnit->GetIsLevelUp() == true )
		{
			if ( pDialogFront->GetControl(L"PVP_Result_User_Box_Level_Up") != NULL )
			{
				pDialogFront->GetControl(L"PVP_Result_User_Box_Level_Up")->SetShow( true );
			}
		}
	}	

	CKTDGUIStatic* pStaticRank = (CKTDGUIStatic*)pDialog->GetControl( L"PVP_Result_Info_Bg_Rank" );
	if ( pStaticRank != NULL )
	{
#ifdef SERV_PVP_NEW_SYSTEM
		if( g_pMain->GetConnectedChannelID() == KPVPChannelInfo::PCC_OFFICIAL &&
			g_pData->GetPVPRoom()->GetMySlot()->m_TeamNum != g_pData->GetPVPResultInfo()->m_WinTeam )
		{
			pStaticRank->SetShow(false);
		}
		else
#endif //SERV_PVP_NEW_SYSTEM
		{
			pStaticRank->SetShow( true );

			for ( int i = 0; i < 4; i++ )
			{
				if ( pStaticRank->GetPicture(i) != NULL )
				{
					pStaticRank->GetPicture(i)->SetShow( false );
				}
			}

			if ( pStaticRank->GetPicture(rankNum - 1) != NULL )
			{
				pStaticRank->GetPicture(rankNum - 1)->SetShow( true );
			}
		}
	}
}

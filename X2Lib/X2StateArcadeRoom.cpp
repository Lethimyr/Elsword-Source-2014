#include "StdAfx.h"
#include ".\x2statearcaderoom.h"

//CX2StateArcadeRoom::CX2StateArcadeRoom(void)
//{
//	m_pDLGArcadeRoomFront = new CKTDGUIDialog( this, L"DLG_Dungeon_Arcade_Room_Front.lua" );
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGArcadeRoomFront );
//
//	CKTDGUIStatic* pStaticRoomTitle = (CKTDGUIStatic*)m_pDLGArcadeRoomFront->GetControl( L"Arcade_Room_Title" );
//	if ( pStaticRoomTitle != NULL )
//	{
//		pStaticRoomTitle->SetString( 0, m_pCX2DungeonRoom->GetRoomName() );
//	}
//
//	CKTDGUIStatic* pStaticRoomLock = (CKTDGUIStatic*)m_pDLGArcadeRoomFront->GetControl( L"Arcade_Room_State_Lock" );
//	if ( pStaticRoomLock != NULL )
//	{
//		if ( m_pCX2DungeonRoom->GetRoomPublic() == true )
//		{
//			pStaticRoomLock->SetShow( false );
//		}
//		else
//		{
//			pStaticRoomLock->SetShow( true );
//		}
//	}
//	
//
//	if ( m_pDLGDungeonRoomFront != NULL )
//	{
//		CKTDGUIControl* pControls = m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_Title");
//		if ( pControls != NULL )
//			pControls->SetShow( false );
//
//		pControls = m_pDLGDungeonRoomFront->GetControl(L"DungeonRoom_RoomName");
//		if ( pControls != NULL )
//			pControls->SetShow( false );
//
//		pControls = m_pDLGDungeonRoomFront->GetControl(L"Dungeon_Room_State_Lock");
//		if ( pControls != NULL )
//			pControls->SetShow( false );
//
//		m_pDLGDungeonRoomFront->GetControl( L"RadioButtonRandom" )->SetShowEnable(false, false);
//		m_pDLGDungeonRoomFront->GetControl( L"RadioButtonPersonal" )->SetShowEnable(false, false);
//		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->SetShowEnable(false, false);
//		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->SetShowEnable(false, false);	
//	}
//}
//
//CX2StateArcadeRoom::~CX2StateArcadeRoom(void)
//{
//	SAFE_DELETE_DIALOG( m_pDLGArcadeRoomFront );
//}
//
//void CX2StateArcadeRoom::SetShowStateDLG( bool bShow )
//{
//	CX2StateDungeonRoom::SetShowStateDLG( bShow );
//
//	if ( m_pDLGArcadeRoomFront != NULL )
//		m_pDLGArcadeRoomFront->SetShowEnable( bShow, bShow );
//}
//
//void CX2StateArcadeRoom::UIFrameMove()
//{
//	CX2StateDungeonRoom::UIFrameMove();
//
//	if(m_pDLGDungeonRoomFront != NULL)
//	{
//		m_pDLGDungeonRoomFront->GetControl( L"RadioButtonRandom" )->SetShowEnable(false, false);
//		m_pDLGDungeonRoomFront->GetControl( L"RadioButtonPersonal" )->SetShowEnable(false, false);
//		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_LeftArrow")->SetShowEnable(false, false);
//		m_pDLGDungeonRoomFront->GetControl(L"Dungeon_RightArrow")->SetShowEnable(false, false);
//	}
//
//	if ( m_pDLGArcadeRoomFront != NULL )
//	{
//		const CX2Dungeon::DungeonData* pDungeonData = g_pData->GetDungeonManager()->GetDungeonData( m_pCX2DungeonRoom->GetDungeonID() );
//		if ( pDungeonData != NULL )
//		{
//			CKTDGUIStatic* g_pStaticArcade_Rank_Not = (CKTDGUIStatic*)m_pDLGArcadeRoomFront->GetControl( L"g_pStaticArcade_Rank_Not" );
//			if ( g_pStaticArcade_Rank_Not != NULL )
//			{
//				wstringstream wstrstm;
//				wstrstm << L"랭킹 등록 가능 레벨 : ";
//				wstrstm << pDungeonData->m_MinLevel;
//				wstrstm << L" ~ ";
//				wstrstm << pDungeonData->m_MaxLevel;
//				g_pStaticArcade_Rank_Not->SetString( 0, wstrstm.str().c_str() );
//			}
//
//			CKTDGUIControl* g_pStaticArcade_Rank_Not2 = m_pDLGArcadeRoomFront->GetControl( L"g_pStaticArcade_Rank_Not2" );
//			CKTDGUIControl* g_pStaticArcade_Rank_Not3 = m_pDLGArcadeRoomFront->GetControl( L"g_pStaticArcade_Rank_Not3" );
//			if ( g_pStaticArcade_Rank_Not2 != NULL && g_pStaticArcade_Rank_Not3 != NULL )
//			{
//				g_pStaticArcade_Rank_Not2->SetShow( false );
//				g_pStaticArcade_Rank_Not3->SetShow( false );
//
//				bool bCheckImpossible = true;
//				for ( int i = 0; i < (int)m_pCX2DungeonRoom->GetSlotNum(); i++ )
//				{
//					CX2Room::SlotData* pSlotData = m_pCX2DungeonRoom->GetSlotData( i );
//					if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
//					{
//						if ( pSlotData->m_pUnit->GetUnitData()->m_Level > pDungeonData->m_MaxLevel )
//						{
//							bCheckImpossible = false;
//							break;
//						}
//					}
//				}
//
//				if ( bCheckImpossible == true )
//				{
//					g_pStaticArcade_Rank_Not2->SetShow( true );
//				}
//				else
//				{
//					g_pStaticArcade_Rank_Not3->SetShow( true );
//				}
//				
//			}
//		}		
//	}
//}
//
//bool CX2StateArcadeRoom::Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ()
//{
//	g_pMain->CreateStateChangeDLG( L"아케이드 로비로 이동중입니다." );
//	g_pData->DeleteDungeonRoom();
//	g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_LOBBY, NULL, false );
//
//	return true;
//}
//
//
//bool CX2StateArcadeRoom::Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KPacketOK kPacket;
//	DeSerialize( pBuff, &kPacket );
//	CX2PacketLog::PrintLog( &kPacket );
//
//	bool bRetVal = false;
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//
//	if( NetError::ERR_ROOM_34 == kPacket.m_iOK )
//	{
//		bRetVal = true;
//		g_pMain->CreateStateChangeDLG( L"아케이드 로비로 이동중입니다." );
//		g_pData->DeleteDungeonRoom(); 
//		Handler_EGS_GET_RANKING_INFO_REQ();
//		g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_ARCADE_LOBBY, NULL, false );
//	}
//	else
//	{
//		if( g_pMain->DeleteServerPacket( EGS_LEAVE_ROOM_ACK ) == true )
//		{
//			if( g_pMain->IsValidPacket(kPacket.m_iOK) == true )
//			{
//				bRetVal= true;
//				g_pMain->CreateStateChangeDLG( L"아케이드 로비로 이동중입니다." );
//				g_pData->DeleteDungeonRoom(); 
//				Handler_EGS_GET_RANKING_INFO_REQ();
//				g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, /*CX2Main::XS_UNIT_SELECT*/ CX2Main::XS_ARCADE_LOBBY, NULL, false );
//			}
//		}
//	}
//
//	return bRetVal;
//}
//
//bool CX2StateArcadeRoom::Handler_EGS_STATE_CHANGE_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	KEGS_STATE_CHANGE_GAME_START_NOT kEvent;
//	DeSerialize( pBuff, &kEvent );
//	CX2PacketLog::PrintLog( &kEvent );
//
//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//
//	g_pData->GetMyUser()->GetSelectUnit()->GetInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
//
//
//	if( g_pData->GetGameUDP()->ConnectRelayTestResult() == true || m_pCX2DungeonRoom->GetUserNum() == 1 )
//	{
//		if( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true	|| m_pCX2DungeonRoom->GetMySlot()->m_bReady == true )
//		{
//			g_pMain->CreateStateChangeDLG( L"아케이드 게임으로 이동중입니다." );
//		}
//
//		m_pCX2DungeonRoom->SetRoomState( (CX2Room::ROOM_STATE)kEvent.m_cRoomState );
//		m_pCX2DungeonRoom->Set_KRoomSlotInfoList( kEvent.m_vecSlot, true );
//		//{{ 2007. 10. 5  최육사  감소된 근성도 받기
//		g_pData->GetMyUser()->GetSelectUnit()->SetSpirit( kEvent.m_iSpirit );
//		//}}
//
//		if( m_pCX2DungeonRoom->GetMySlot()->m_bHost == true	|| m_pCX2DungeonRoom->GetMySlot()->m_bReady == true )
//		{
//			DialogLog( "XGM_STATE_CHANGE, CX2Main::XS_DUNGEON_GAME, NULL, false" );
//			Handler_EGS_GET_RANKING_INFO_REQ();
//			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_ARCADE_GAME, NULL, false );
//		}
//	}
//	else
//	{
//		Handler_EGS_LEAVE_ROOM_REQ();
//		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"네트워크를 연결할 수 없습니다", this );
//		ErrorLogMsg( XEM_ERROR92, L"Dungeon" );
//	}
//
//	return true;	
//}
